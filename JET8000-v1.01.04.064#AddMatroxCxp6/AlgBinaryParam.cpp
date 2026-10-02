// AlgBinaryParam.cpp: implementation of the CAlgBinaryParam class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AlgBinaryParam.h"
//-------------------------------------------------------------------------------------//
#include "AOIFileIO.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CAlgBinaryParam::CAlgBinaryParam()
{
	CAlgBinaryParam::PreInitBinaryParam();
	CAlgBinaryParam::InitialBinaryParam();
}
//-------------------------------------------------------------------------------------//
CAlgBinaryParam::CAlgBinaryParam(const CAlgBinaryParam &Binary)
{
	CAlgBinaryParam::PreInitBinaryParam();
	CAlgBinaryParam::CloneBinaryParam(Binary);
}
//-------------------------------------------------------------------------------------//
CAlgBinaryParam::~CAlgBinaryParam()
{

}
//-------------------------------------------------------------------------------------//
CAlgBinaryParam& CAlgBinaryParam::operator=(const CAlgBinaryParam &Binary)
{
	if ( this == &Binary ) { return *this; }
	CAlgBinaryParam::CloneBinaryParam(Binary);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::PreInitBinaryParam()
{
	m_BinaryColorGroup.BuildColorGroup_Test();	
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::InitialBinaryParam()
{
	m_FrameIndex = 0;		
	m_FrameUniqueID = FRAME_UNIQUE_ID_DEFAULT;

	m_BelongIndex = -1;
	m_BelongToWho = BIN_PARAM_BELONG_TO_NONE;

	m_MaskFuncMode = MASK_FUNC_CALC;

	m_ImageSourceMode = IMAGE_SRC_COLOR;
	m_BinaryMode = BINARY_DISABLE;		
	m_BinaryInvert = false;

	m_GrayInvert = false;
	m_GrayGainValue = 1.0;
	m_GrayGainEnabled = false;	

	m_GrayFilter1 = TBINARY_FILTER();
	m_GrayFilter2 = TBINARY_FILTER();
	m_BinaryFilter1 = TBINARY_FILTER();
	m_BinaryFilter2 = TBINARY_FILTER();

	m_EdgeEnhanceMode = EDGE_ENHANCE_DISABLE;
	m_EdgeEnhanceFilter1 = TBINARY_FILTER();
	m_EdgeEnhanceFilter2 = TBINARY_FILTER();

	m_SynthesisWR = 30;//100
	m_SynthesisWG = 59;//100
	m_SynthesisWB = 11;//100
		
	m_FixedThresholdHigh = 255;
	m_FixedThresholdLow = 128;

	m_RatioThresholdTarget = 255.0;
	m_RatioThresholdRatioHigh = 100;
	m_RatioThresholdRatioLow = 50;		

	m_DynamicThresholdRatio = 30;
	m_DynamicThresholdValue = 0;		

	m_RelativeAveThresholdValue = 128;
	m_RelativeAveThresholdBias = 0;
	m_RelativeAveThresholdAbove = 10;
	m_RelativeAveThresholdBelow = 10;	
	
	m_AdaptiveThresholdGap = 5;//続莱┦恢
	m_AdaptiveThresholdCalcSize = 21;//続莱┦璸衡絛瞅

	m_BinaryColorGroupLinkIndex = -1;
	m_BinaryColorGroup.ResetColorGroupColorList();
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::CloneBinaryParam(const CAlgBinaryParam &Binary)
{
	m_FrameIndex = Binary.m_FrameIndex;		
	m_FrameUniqueID = Binary.m_FrameUniqueID;

	m_BelongIndex = Binary.m_BelongIndex;
	m_BelongToWho = Binary.m_BelongToWho;

	m_MaskFuncMode = Binary.m_MaskFuncMode;

	m_ImageSourceMode = Binary.m_ImageSourceMode;
	m_BinaryMode = Binary.m_BinaryMode;		
	m_BinaryInvert = Binary.m_BinaryInvert;	
	
	m_GrayInvert = Binary.m_GrayInvert;
	m_GrayGainValue = Binary.m_GrayGainValue;
	m_GrayGainEnabled = Binary.m_GrayGainEnabled;	

	m_GrayFilter1 = Binary.m_GrayFilter1;
	m_GrayFilter2 = Binary.m_GrayFilter2;

	m_BinaryFilter1 = Binary.m_BinaryFilter1;
	m_BinaryFilter2 = Binary.m_BinaryFilter2;

	m_EdgeEnhanceMode = Binary.m_EdgeEnhanceMode;		
	m_EdgeEnhanceFilter1 = Binary.m_EdgeEnhanceFilter1;		
	m_EdgeEnhanceFilter2 = Binary.m_EdgeEnhanceFilter2;		

	m_SynthesisWR = Binary.m_SynthesisWR;
	m_SynthesisWG = Binary.m_SynthesisWG;
	m_SynthesisWB = Binary.m_SynthesisWB;
		
	m_FixedThresholdHigh = Binary.m_FixedThresholdHigh;
	m_FixedThresholdLow = Binary.m_FixedThresholdLow;

	m_RatioThresholdTarget = Binary.m_RatioThresholdTarget;
	m_RatioThresholdRatioHigh = Binary.m_RatioThresholdRatioHigh;
	m_RatioThresholdRatioLow = Binary.m_RatioThresholdRatioLow;

	m_DynamicThresholdRatio = Binary.m_DynamicThresholdRatio;
	m_DynamicThresholdValue = Binary.m_DynamicThresholdValue;	
	
	m_RelativeAveThresholdValue = Binary.m_RelativeAveThresholdValue;
	m_RelativeAveThresholdBias = Binary.m_RelativeAveThresholdBias;
	m_RelativeAveThresholdAbove = Binary.m_RelativeAveThresholdAbove;
	m_RelativeAveThresholdBelow = Binary.m_RelativeAveThresholdBelow;	
	
	m_AdaptiveThresholdGap = Binary.m_AdaptiveThresholdGap;	
	m_AdaptiveThresholdCalcSize = Binary.m_AdaptiveThresholdCalcSize;	
	
	m_BinaryColorGroupLinkIndex = Binary.m_BinaryColorGroupLinkIndex;
	m_BinaryColorGroup = Binary.m_BinaryColorGroup;
}
//-------------------------------------------------------------------------------------//
bool CAlgBinaryParam::WriteAlgBinaryParamFile(CAOIFileIO &FileIO)//纗2て把计
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	CAlgBinaryParam *BinParamPtr = this;
	FileIO.SetFnName(_T("CAlgBinaryParam::WriteAlgBinaryParamFile"));
	//----------------------------------------------------------------------------------------//

	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_START, 0) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_FRAME_UNIQUE_ID, BinParamPtr->GetBinaryFrameUniqueID()) == false ) { return false; }
	
	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_IMAGE_SOURCE_MODE, BinParamPtr->GetBinaryImageSourceMode()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_BINARY_MODE, BinParamPtr->GetBinaryMode()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_BINARY_PARAM_BINARY_INVERT, BinParamPtr->GetBinaryInvert()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_MASK_FUNC_MODE, BinParamPtr->GetMaskFuncMode()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_SYNTHESIS_WR, BinParamPtr->GetBinarySynthesisWR()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_SYNTHESIS_WG, BinParamPtr->GetBinarySynthesisWG()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_SYNTHESIS_WB, BinParamPtr->GetBinarySynthesisWB()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_FIXED_THRESHOLD_HIGH, BinParamPtr->GetFixedThresholdHigh()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_FIXED_THRESHOLD_LOW, BinParamPtr->GetFixedThresholdLow()) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_BINARY_PARAM_DYNAMIC_THRESHOLD_RATIO, BinParamPtr->GetDynamicThresholdRatio()) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_RELATIVE_AVE_THRESHOLD_ABOVE, BinParamPtr->GetRelativeAveThresholdAbove()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_RELATIVE_AVE_THRESHOLD_BELOW, BinParamPtr->GetRelativeAveThresholdBelow()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_RELATIVE_AVE_THRESHOLD_BIAS, BinParamPtr->GetRelativeAveThresholdBias()) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_COLOR_FILTER_LINK_INDEX, BinParamPtr->GetBinaryColorGroupLinkIndex()) == false ) { return false; }
	
	if ( FileIO.SaveChunk_BOL(FILE_IO_BINARY_PARAM_GRAY_GAIN_ENB, BinParamPtr->GetGrayGainEnabled()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BINARY_PARAM_GRAY_GAIN_VALUE, BinParamPtr->GetGrayGainValue()) == false ) { return false; }	            
	if ( FileIO.SaveChunk_BOL(FILE_IO_BINARY_PARAM_GRAY_INVERT, BinParamPtr->GetGrayInvert()) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_ADAPTIVE_THRESHOLD_GAP, BinParamPtr->GetAdaptiveThresholdGap()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_ADAPTIVE_THRESHOLD_CALC_SIZE, BinParamPtr->GetAdaptiveThresholdCalcSize()) == false ) { return false; }	

	if ( FileIO.SaveChunk_DBL(FILE_IO_BINARY_PARAM_RATIO_THRESHOLD_TARGET, BinParamPtr->GetRatioThresholdTarget()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BINARY_PARAM_RATIO_THRESHOLD_HIGH, BinParamPtr->GetRatioThresholdRatioHigh()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BINARY_PARAM_RATIO_THRESHOLD_LOW, BinParamPtr->GetRatioThresholdRatioLow()) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_EDGE_ENHANCE_MODE, BinParamPtr->GetEdgeEnhanceMode()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_EDGE_ENHANCE_FILTER_1, 0) == false ) { return false; }
	if ( WriteAlgBinaryFilterFile(FileIO, &(BinParamPtr->m_EdgeEnhanceFilter1)) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_EDGE_ENHANCE_FILTER_2, 0) == false ) { return false; }
	if ( WriteAlgBinaryFilterFile(FileIO, &(BinParamPtr->m_EdgeEnhanceFilter2)) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_GRAY_FILTER_1, 0) == false ) { return false; }
	if ( WriteAlgBinaryFilterFile(FileIO, &(BinParamPtr->m_GrayFilter1)) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_GRAY_FILTER_2, 0) == false ) { return false; }
	if ( WriteAlgBinaryFilterFile(FileIO, &(BinParamPtr->m_GrayFilter2)) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_BINARY_FILTER_1, 0) == false ) { return false; }
	if ( WriteAlgBinaryFilterFile(FileIO, &(BinParamPtr->m_BinaryFilter1)) == false ) { return false; }
	
	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_BINARY_FILTER_2, 0) == false ) { return false; }
	if ( WriteAlgBinaryFilterFile(FileIO, &(BinParamPtr->m_BinaryFilter2)) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_COLOR_FILTER_GROUP, 0) == false ) { return false; }
	if ( BinParamPtr->m_BinaryColorGroup.WriteColorGroupFile(FileIO) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_BINARY_PARAM_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgBinaryParam::ReadAlgBinaryParamFile(CAOIFileIO &FileIO)//更2て把计
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG	
	int    index = 0;
	CAlgBinaryParam *BinParamPtr = this;
	FileIO.SetFnName(_T("CAlgBinaryParam::ReadAlgBinaryParamFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_BINARY_PARAM_START://2て把计-癬翴
			break;
		case FILE_IO_BINARY_PARAM_END://2て把计-沧翴			
			return true;
			break;
		case FILE_IO_BINARY_PARAM_FRAME_UNIQUE_ID://2て把计-Frame斑絏
			BinParamPtr->SetBinaryFrameUniqueID(FileIO.GetData_INT());			
			break;
		case FILE_IO_BINARY_PARAM_IMAGE_SOURCE_MODE://2て把计-紇钩ㄓ方			
			BinParamPtr->SetBinaryImageSourceMode((IMAGE_SRC_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_BINARY_PARAM_BINARY_MODE://2て把计-2て家Α	
			BinParamPtr->SetBinaryMode((BINARY_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_BINARY_PARAM_BINARY_INVERT://2て把计-2ては
			BinParamPtr->SetBinaryInvert(FileIO.GetData_BOL());
			break;
		case FILE_IO_BINARY_PARAM_MASK_FUNC_MODE://2て把计-綛竛家Α
			BinParamPtr->SetMaskFuncMode((MASK_FUNC_MODE)FileIO.GetData_INT());
			break;
		case FILE_IO_BINARY_PARAM_SYNTHESIS_WR://2て把计-Θ紇钩-︹	
			BinParamPtr->SetBinarySynthesisWR(FileIO.GetData_INT());
			break;
		case FILE_IO_BINARY_PARAM_SYNTHESIS_WG://2て把计-Θ紇钩-厚︹	
			BinParamPtr->SetBinarySynthesisWG(FileIO.GetData_INT());
			break;
		case FILE_IO_BINARY_PARAM_SYNTHESIS_WB://2て把计-Θ紇钩-屡︹	
			BinParamPtr->SetBinarySynthesisWB(FileIO.GetData_INT());
			break;
		case FILE_IO_BINARY_PARAM_FIXED_THRESHOLD_HIGH://2て把计-㏕﹚恢-
			BinParamPtr->SetFixedThresholdHigh(FileIO.GetData_INT());
			break;
		case FILE_IO_BINARY_PARAM_FIXED_THRESHOLD_LOW://2て把计-㏕﹚恢-
			BinParamPtr->SetFixedThresholdLow(FileIO.GetData_INT());
			break;
		case FILE_IO_BINARY_PARAM_DYNAMIC_THRESHOLD_RATIO://2て把计-笆篈ゑㄒ
			BinParamPtr->SetDynamicThresholdRatio(FileIO.GetData_DBL());
			break;
		case FILE_IO_BINARY_PARAM_RELATIVE_AVE_THRESHOLD_ABOVE://2て把计-癸キА恢-
			BinParamPtr->SetRelativeAveThresholdAbove(FileIO.GetData_INT());
			break;
		case FILE_IO_BINARY_PARAM_RELATIVE_AVE_THRESHOLD_BELOW://2て把计-癸キА恢-	
			BinParamPtr->SetRelativeAveThresholdBelow(FileIO.GetData_INT());
			break;
		case FILE_IO_BINARY_PARAM_RELATIVE_AVE_THRESHOLD_BIAS://2て把计-癸キА恢-干纕
			BinParamPtr->SetRelativeAveThresholdBias(FileIO.GetData_INT());
			break;
		case FILE_IO_BINARY_PARAM_COLOR_FILTER_LINK_INDEX://2て把计-眒︹竤舱硈笆腹
			BinParamPtr->SetBinaryColorGroupLinkIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_BINARY_PARAM_GRAY_GAIN_ENB://2て把计-η顶糤痲币ノ
			BinParamPtr->SetGrayGainEnabled(FileIO.GetData_BOL());			
			break;
		case FILE_IO_BINARY_PARAM_GRAY_GAIN_VALUE://2て把计-η顶糤痲计
			BinParamPtr->SetGrayGainValue(FileIO.GetData_DBL());
			break;
		case FILE_IO_BINARY_PARAM_GRAY_INVERT://2て把计-η顶は-璽
			BinParamPtr->SetGrayInvert(FileIO.GetData_BOL());			
			break;

		case FILE_IO_BINARY_PARAM_ADAPTIVE_THRESHOLD_GAP://2て把计-続莱Α恢丁禯
			BinParamPtr->SetAdaptiveThresholdGap(FileIO.GetData_INT());
			break;
		case FILE_IO_BINARY_PARAM_ADAPTIVE_THRESHOLD_CALC_SIZE://2て把计-続莱Α恢璸衡へ
			BinParamPtr->SetAdaptiveThresholdCalcSize(FileIO.GetData_INT());
			break;

		case FILE_IO_BINARY_PARAM_RATIO_THRESHOLD_TARGET://2て把计-ゑㄒ恢-ヘ夹
			BinParamPtr->SetRatioThresholdTarget(FileIO.GetData_DBL());
			break;
		case FILE_IO_BINARY_PARAM_RATIO_THRESHOLD_HIGH://2て把计-ゑㄒ恢-ゑㄒ
			BinParamPtr->SetRatioThresholdRatioHigh(FileIO.GetData_DBL());
			break;
		case FILE_IO_BINARY_PARAM_RATIO_THRESHOLD_LOW://2て把计-ゑㄒ恢-ゑㄒ
			BinParamPtr->SetRatioThresholdRatioLow(FileIO.GetData_DBL());
			break;

		case FILE_IO_BINARY_PARAM_EDGE_ENHANCE_MODE://2て把计-娩絫眏て家Α
			BinParamPtr->SetEdgeEnhanceMode((EDGE_ENHANCE_MODE)FileIO.GetData_INT());
			break;
		case FILE_IO_BINARY_PARAM_EDGE_ENHANCE_FILTER_1://2て把计-娩絫筁耾竤舱-1
			if ( ReadAlgBinaryFilterFile(FileIO, &(BinParamPtr->m_EdgeEnhanceFilter1)) == false )
			{	return false; }
			break;
		case FILE_IO_BINARY_PARAM_EDGE_ENHANCE_FILTER_2://2て把计-娩絫筁耾竤舱-2
			if ( ReadAlgBinaryFilterFile(FileIO, &(BinParamPtr->m_EdgeEnhanceFilter2)) == false )
			{	return false; }
			break;

		case FILE_IO_BINARY_PARAM_GRAY_FILTER_1://2て把计-η顶筁耾竤舱-1
			if ( ReadAlgBinaryFilterFile(FileIO, &(BinParamPtr->m_GrayFilter1)) == false )
			{	return false; }
			break;
		case FILE_IO_BINARY_PARAM_GRAY_FILTER_2://2て把计-η顶筁耾竤舱-2
			if ( ReadAlgBinaryFilterFile(FileIO, &(BinParamPtr->m_GrayFilter2)) == false )
			{	return false; }
			break;

		case FILE_IO_BINARY_PARAM_BINARY_FILTER_1://2て把计-馒癟筁耾竤舱-1
			if ( ReadAlgBinaryFilterFile(FileIO, &(BinParamPtr->m_BinaryFilter1)) == false )
			{	return false; }
			break;
		case FILE_IO_BINARY_PARAM_BINARY_FILTER_2://2て把计-馒癟筁耾竤舱-2
			if ( ReadAlgBinaryFilterFile(FileIO, &(BinParamPtr->m_BinaryFilter2)) == false )
			{	return false; }
			break;

		case FILE_IO_BINARY_PARAM_COLOR_FILTER_GROUP://2て把计-眒︹筁耾竤舱
			if ( BinParamPtr->m_BinaryColorGroup.ReadColorGroupFile(FileIO) == false )
			{	return false; }			
			break;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgBinaryParam::WriteAlgBinaryFilterFile(CAOIFileIO &FileIO, TBINARY_FILTER *BinFilterPtr)//纗2て筁耾竟把计
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG
	if ( NULL == BinFilterPtr ) 
	{ 
		FileIO.SetErrorString(_T("BinFilterPtr == NULL"));
		return false; 
	}
	
	FileIO.SetFnName(_T("CAlgBinaryParam::WriteAlgBinaryFilterFile"));
	//----------------------------------------------------------------------------------------//
	if ( FileIO.SaveChunk_INT(FILE_IO_NOISE_FILTER_START, 0) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_NOISE_FILTER_MODE, BinFilterPtr->FilterMode) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_NOISE_FILTER_PARAM_1, BinFilterPtr->FilterParam1) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_NOISE_FILTER_PARAM_2, BinFilterPtr->FilterParam2) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_NOISE_FILTER_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgBinaryParam::ReadAlgBinaryFilterFile(CAOIFileIO &FileIO, TBINARY_FILTER *BinFilterPtr)//更2て筁耾竟把计
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG
	if ( NULL == BinFilterPtr ) 
	{ 
		FileIO.SetErrorString(_T("BinFilterPtr == NULL"));
		return false; 
	}
	
	int    index = 0;
	FileIO.SetFnName(_T("CAlgBinaryParam::ReadAlgBinaryFilterFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{		
		case FILE_IO_NOISE_FILTER_START://馒癟筁耾把计-癬翴
			break;
		case FILE_IO_NOISE_FILTER_END://馒癟筁耾把计-沧翴
			return true;
		case FILE_IO_NOISE_FILTER_MODE://馒癟筁耾把计-家Α
			BinFilterPtr->FilterMode = (NOISE_FILTER_MODE)(FileIO.GetData_INT());
			break;
		case FILE_IO_NOISE_FILTER_PARAM_1://馒癟筁耾把计-把计1
			BinFilterPtr->FilterParam1 = FileIO.GetData_INT();
			break;
		case FILE_IO_NOISE_FILTER_PARAM_2://馒癟筁耾把计-把计2
			BinFilterPtr->FilterParam2 = FileIO.GetData_INT();
		default:
			break;
		}
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::SetBinaryFrameIndex(unsigned int val)
{ 
	m_FrameIndex = val; 
	m_BinaryColorGroup.SetColorGroupFrameIndex(val);
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::SetBinaryFrameUniqueID(unsigned int val)
{ 
	m_FrameUniqueID = val; 
	m_BinaryColorGroup.SetColorGroupFrameUniqueID(val);
}
//-------------------------------------------------------------------------------------//
bool CAlgBinaryParam::CheckGrayGainEnabed() const//絋粄琌ㄏノ紇钩糤痲
{
	if ( GetGrayGainEnabled() == false ) { return false; }
	const double GainValue= GetGrayGainValue();
	if ( fabs(GainValue-1.0) < 0.001) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::CopyGrayFilterParam(const CAlgBinaryParam &Param)//狡籹η顶筁耾把计
{	
	SetGrayNoiseFilter1(Param.GetGrayNoiseFilter1());	
	SetGrayNoiseFilter2(Param.GetGrayNoiseFilter2());	
	return;
}
//-------------------------------------------------------------------------------------//
bool CAlgBinaryParam::CheckUseGrayFilterParam(const TBINARY_FILTER &Filter) const//絋粄琌ㄏノη顶筁耾把计
{
	bool bUseFilter=false;
	const int Param = Filter.FilterParam1;	
	const NOISE_FILTER_MODE Mode = Filter.FilterMode;		
	switch ( Mode )
	{
	case NOISE_FILTER_SMOOTH:
	case NOISE_FILTER_MEDIAN:
	case NOISE_FILTER_OPEN:
	case NOISE_FILTER_CLOSE:
		if ( Param > 1)		
		{	bUseFilter = true; }
		break;
	default:			
		break;
	}
	return bUseFilter;
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::CopyBinaryFilterParam(const CAlgBinaryParam &Param)//狡籹2て筁耾把计
{
	SetBinaryNoiseFilter1(Param.GetBinaryNoiseFilter1());	
	SetBinaryNoiseFilter2(Param.GetBinaryNoiseFilter2());	
}
//-------------------------------------------------------------------------------------//
bool CAlgBinaryParam::CheckUseBinaryFilterParam(const TBINARY_FILTER &Filter) const//絋粄琌ㄏノ2て筁耾把计
{
	bool bUseFilter=false;
	const int Param = Filter.FilterParam1;	
	const NOISE_FILTER_MODE Mode = Filter.FilterMode;		
	switch ( Mode )
	{
	case NOISE_FILTER_OPEN:
	case NOISE_FILTER_CLOSE:
	case NOISE_FILTER_EROSION:
	case NOISE_FILTER_DILATION:
	case NOISE_FILTER_GRADIENT:
		if ( Param > 1)		
		{	bUseFilter = true; }
		break;
	default:			
		break;
	}
	return bUseFilter;
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::CopyEdgeEnhanceParam(const CAlgBinaryParam &Param)
{
	SetEdgeEnhanceMode(Param.GetEdgeEnhanceMode());	
	SetEdgeEnhanceFilter1(Param.GetEdgeEnhanceFilter1());	
	SetEdgeEnhanceFilter2(Param.GetEdgeEnhanceFilter2());	
	return;
}
//-------------------------------------------------------------------------------------//
bool CAlgBinaryParam::CheckUseEdgeEnhanceParam(const TBINARY_FILTER &Filter) const//絋粄琌ㄏノ娩絫眏て把计
{	
	bool bUseFilter= true;	
	const int Param1=Filter.FilterParam1;
	const int Param2=Filter.FilterParam2;
	switch ( Filter.FilterMode )
	{
	case NOISE_FILTER_OPEN:		
	case NOISE_FILTER_CLOSE:		
	case NOISE_FILTER_EROSION:		
	case NOISE_FILTER_DILATION:		
	case NOISE_FILTER_GRADIENT:
		if ( Param1 < 2 )
		{	bUseFilter = false;	}
		break;
	default:
		bUseFilter = false;
		break;
	}	
	if ( false == bUseFilter )
	{	return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::SetBinaryColorGroup(const CColorGroup &val)
{ 	
	m_BinaryColorGroup = val; 

	BINARY_MODE BinaryMode = GetBinaryMode();	
	if ( BINARY_COLOR_FILTER == BinaryMode )
	{
		unsigned int FrameIndex = val.GetColorGroupFrameIndex();
		unsigned int FrameUniqueID = val.GetColorGroupFrameUniqueID();		
		SetBinaryFrameIndex(FrameIndex);
		SetBinaryFrameUniqueID(FrameUniqueID);
	}
	else
	{
		unsigned int FrameIndex = GetBinaryFrameIndex();
		unsigned int FrameUniqueID = GetBinaryFrameUniqueID();
		m_BinaryColorGroup.SetColorGroupFrameIndex(FrameIndex);
		m_BinaryColorGroup.SetColorGroupFrameUniqueID(FrameUniqueID);
	}
	return;
}
//-------------------------------------------------------------------------------------//
size_t CAlgBinaryParam::GetBinaryColorCount() const
{
	return m_BinaryColorGroup.GetColorGroupColorCount();
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::AddBinaryColor(const CColorRGBV &rgbv)
{
	m_BinaryColorGroup.AddColorGroupColor(rgbv);
}
//-------------------------------------------------------------------------------------//
bool CAlgBinaryParam::SetBinaryColor(size_t idx, const CColorRGBV &rgbv)//砞﹚┾︹竤舱肅︹
{
	return m_BinaryColorGroup.SetColorGroupColor(idx, rgbv);
}
//-------------------------------------------------------------------------------------//
CColorRGBV* CAlgBinaryParam::GetBinaryColorPtr(size_t index, bool Check)
{
	return m_BinaryColorGroup.GetColorGroupColorPtr(index, Check);
}
//-------------------------------------------------------------------------------------//
const CColorRGBV* CAlgBinaryParam::GetBinaryColorPtr(size_t index, bool Check) const
{
	return m_BinaryColorGroup.GetColorGroupColorPtr(index, Check);
}
//-------------------------------------------------------------------------------------//
CColorRGBV* CAlgBinaryParam::GetBinaryColorActivePtr()//眔璶巨肅︹
{
	return m_BinaryColorGroup.GetColorGroupActiveColorPtr();
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::SetBinaryColorActivePtr(CColorRGBV* Ptr)//砞﹚璶巨肅︹	
{
	m_BinaryColorGroup.SetColorGroupActiveColorPtr(Ptr);
}
//-------------------------------------------------------------------------------------//
size_t CAlgBinaryParam::GetBinaryColorActiveIndex() const//砞﹚璶巨肅︹	
{
	return m_BinaryColorGroup.GetColorGroupActiveColorIndex();
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::SetBinaryColorActiveIndex(size_t index)//砞﹚璶巨肅︹	
{
	m_BinaryColorGroup.SetColorGroupActiveColorIndex(index);
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::ClearBinaryColorList()//睲埃┾︹竤舱肅︹
{
	m_BinaryColorGroup.ClearColorGroupColorList();
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::ResetBinaryColorList()//確耴┾︹竤舱肅︹
{
	m_BinaryColorGroup.ResetColorGroupColorList();
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::UpdateBinaryColorUsed()//穝┾︹竤舱肅︹琌ㄏノ
{
	m_BinaryColorGroup.UpdateColorGroupUsed();
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::UpdateBinaryColorShowColor()//穝┾︹竤舱肅︹琌ㄏノ
{
	m_BinaryColorGroup.UpdateColorGroupShowColor();
}
//-------------------------------------------------------------------------------------//
bool CAlgBinaryParam::MergeBinaryColor(bool IncClr, bool ExcClr)//ㄖ┾︹竤舱ず︹
{
	return m_BinaryColorGroup.MergeColorGroupColor(IncClr, ExcClr);
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::MirrorBinaryParamXAxis()
{
	EDGE_ENHANCE_MODE EdgeEnhanceMode=GetEdgeEnhanceMode();
	EDGE_ENHANCE_MODE EdgeEnhanceMode2=EdgeEnhanceMode;	
	switch ( EdgeEnhanceMode )
	{
	case EDGE_ENHANCE_DARK_TOP: EdgeEnhanceMode2=EDGE_ENHANCE_DARK_BOT;	break;	
	case EDGE_ENHANCE_DARK_BOT: EdgeEnhanceMode2=EDGE_ENHANCE_DARK_TOP;	break;	
	}	
	SetEdgeEnhanceMode(EdgeEnhanceMode2);
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::MirrorBinaryParamYAxis()
{
	EDGE_ENHANCE_MODE EdgeEnhanceMode=GetEdgeEnhanceMode();
	EDGE_ENHANCE_MODE EdgeEnhanceMode2=EdgeEnhanceMode;	
	switch ( EdgeEnhanceMode )
	{
	case EDGE_ENHANCE_DARK_LEFT: EdgeEnhanceMode2=EDGE_ENHANCE_DARK_RIGHT;	break;	
	case EDGE_ENHANCE_DARK_RIGHT: EdgeEnhanceMode2=EDGE_ENHANCE_DARK_LEFT;	break;	
	}	
	SetEdgeEnhanceMode(EdgeEnhanceMode2);
}
//-------------------------------------------------------------------------------------//
void CAlgBinaryParam::RotateBinaryParam(int AngleLabel)
{
	EDGE_ENHANCE_MODE EdgeEnhanceMode=GetEdgeEnhanceMode();
	EDGE_ENHANCE_MODE EdgeEnhanceMode2=EdgeEnhanceMode;
	switch ( AngleLabel )
	{	
	case 90:
		switch ( EdgeEnhanceMode )
		{
		case EDGE_ENHANCE_DARK_TOP: EdgeEnhanceMode2=EDGE_ENHANCE_DARK_LEFT;	break;
		case EDGE_ENHANCE_DARK_LEFT: EdgeEnhanceMode2=EDGE_ENHANCE_DARK_BOT;	break;
		case EDGE_ENHANCE_DARK_BOT: EdgeEnhanceMode2=EDGE_ENHANCE_DARK_RIGHT;	break;
		case EDGE_ENHANCE_DARK_RIGHT: EdgeEnhanceMode2=EDGE_ENHANCE_DARK_TOP;	break;
		}
		break;
	case 180:
		switch ( EdgeEnhanceMode )
		{
		case EDGE_ENHANCE_DARK_TOP: EdgeEnhanceMode2=EDGE_ENHANCE_DARK_BOT;	break;
		case EDGE_ENHANCE_DARK_LEFT: EdgeEnhanceMode2=EDGE_ENHANCE_DARK_RIGHT;	break;
		case EDGE_ENHANCE_DARK_BOT: EdgeEnhanceMode2=EDGE_ENHANCE_DARK_TOP;	break;
		case EDGE_ENHANCE_DARK_RIGHT: EdgeEnhanceMode2=EDGE_ENHANCE_DARK_LEFT;	break;
		}
		break;
	case 270:
		switch ( EdgeEnhanceMode )
		{
		case EDGE_ENHANCE_DARK_TOP: EdgeEnhanceMode2=EDGE_ENHANCE_DARK_RIGHT;	break;
		case EDGE_ENHANCE_DARK_LEFT: EdgeEnhanceMode2=EDGE_ENHANCE_DARK_TOP;	break;
		case EDGE_ENHANCE_DARK_BOT: EdgeEnhanceMode2=EDGE_ENHANCE_DARK_LEFT;	break;
		case EDGE_ENHANCE_DARK_RIGHT: EdgeEnhanceMode2=EDGE_ENHANCE_DARK_BOT;	break;
		}
		break;
	case 0:
	default:
		break;
	}
	SetEdgeEnhanceMode(EdgeEnhanceMode2);
	return;
}
//-------------------------------------------------------------------------------------//
bool CAlgBinaryParam::UpdateBinaryFrameUniqueID(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList)
{	
	unsigned int FrameIndex = GetBinaryFrameIndex();		
	unsigned int FrameUniqueID = GetBinaryFrameUniqueID();		
	const size_t FrameIndexMapSize = FrameIndexMapList.size();
	if ( FrameUniqueID<0 || FrameUniqueID>=FrameIndexMapSize )
	{	
		FrameIndex = DefaultIndex;	
		FrameUniqueID = DefaultUniqueID;
	}
	else
	{	
		FrameIndex = (FrameIndexMapList[FrameUniqueID]);
		if ( -1 == FrameIndex )
		{ 
			FrameIndex = DefaultIndex;	
			FrameUniqueID = DefaultUniqueID;
		}
	}	
	SetBinaryFrameIndex(FrameIndex);
	SetBinaryFrameUniqueID(FrameUniqueID);	
	return true;
}
//-------------------------------------------------------------------------------------//