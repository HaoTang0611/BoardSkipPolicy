// AlgParam.cpp: implementation of the CAlgParam class.
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
typedef struct tagAlgParamNode//演算法參數
{
	CString       sName;//名稱
	double        fSTD; //標準
	double        fUSL; //上限
	double        fLSL; //下限	
	double        fReading;//讀直

	tagAlgParamNode(LPCTSTR Name, double Std, double Usl, double Lsl, double Reading)
	{
		sName = Name;
		fSTD = Std;
		fUSL = Usl;
		fLSL = Lsl;
		fReading = Reading;
	}
} TAlgParamNode, *PAlgParamNode;
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckAlgUseExtendBox(ALG_TYPE Type)//確認演算法需要外擴檢測框	
{
	bool Used=false;
	switch ( Type )
	{	
	case ALG_OUTER_SHORT:
		Used=true;
		break;	
	case ALG_OBJECT_MEASURE:
	case ALG_SHAPE_VERIFY://外形驗證	
		Used=true;
		break;
	case ALG_MODEL_MATCH:
	case ALG_IMAGE_MATCH:
	case ALG_CHAR_VERIFY:
	case ALG_FD_MATCH:
	case ALG_EDGE_SEARCH:
	case ALG_PIXEL_COMPARE:
	case ALG_MEASURE_SIP_DISTANCE:
		Used=true;
		break;	
	case ALG_ANGLE_MEASURE:
	case ALG_SOLDER_WETTING:
	//case ALG_MEASURE_SIP_DISTANCE:
		break;
	case ALG_MEASURE_BLACK_GLUE:
	case ALG_MEASURE_FLUX_AREA:
	case ALG_MEASURE_CPU_PIN:
	case ALG_MEASURE_CONNECTOR:
		break;
	}
	return Used;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::BuildAlgTypeList(std::vector<ALG_TYPE> &List)//建立演算法樣式列表
{
	List.clear();
	List.push_back(ALG_BRIGHT_RATIO);
	List.push_back(ALG_OUTER_SHORT);
	List.push_back(ALG_BLOB_COUNT);
	//List.push_back(ALG_BODY_TILT);
	List.push_back(ALG_BARCODE_RECOGNIZE);
	List.push_back(ALG_OBJECT_MEASURE);
	List.push_back(ALG_COLOR_CODE);
	List.push_back(ALG_SHAPE_VERIFY);
	List.push_back(ALG_ANGLE_MEASURE);
	List.push_back(ALG_WIDTH_RATIO);
	List.push_back(ALG_SOLDER_WETTING);

	List.push_back(ALG_MODEL_MATCH);
	List.push_back(ALG_IMAGE_MATCH);
	List.push_back(ALG_CHAR_VERIFY);
	//List.push_back(ALG_FD_MATCH);
	List.push_back(ALG_EDGE_SEARCH);
	List.push_back(ALG_PIXEL_COMPARE);
	List.push_back(ALG_HEIGHT);
	List.push_back(ALG_WIRE_WIDTH);

#ifdef ALG_MEASURE_BLACK_GLUE_USE
	List.push_back(ALG_MEASURE_BLACK_GLUE);	
#endif//ALG_MEASURE_BLACK_GLUE_USE

#ifdef ALG_MEASURE_FLUX_AREA_USE
	List.push_back(ALG_MEASURE_FLUX_AREA);	
#endif//ALG_MEASURE_FLUX_AREA_USE

#ifdef ALG_MEASURE_CPU_PIN_USE
	List.push_back(ALG_MEASURE_CPU_PIN);	
#endif//ALG_MEASURE_CPU_PIN_USE
	
#define ALG_MEASURE_SIP_DISTANCE_USE
#ifdef ALG_MEASURE_SIP_DISTANCE_USE
	List.push_back(ALG_MEASURE_SIP_DISTANCE);
#endif//ALG_MEASURE_SIP_DISTANCE

#define ALG_MEASURE_CONNECT_USE
#ifdef ALG_MEASURE_CONNECT_USE
	List.push_back(ALG_MEASURE_CONNECTOR);
	List.push_back(ALG_MEASURE_CONNECTOR_PIN);
#endif//ALG_MEASURE_CONNECT_USE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckImageSourceGrayMode(IMAGE_SRC_MODE Mode)//確認影像來源為灰階模式
{
	bool bGray=false;
	switch ( Mode )
	{
	case IMAGE_SRC_RED:
	case IMAGE_SRC_GREEN:
	case IMAGE_SRC_BLUE:
	case IMAGE_SRC_LIGHTNESS:
	case IMAGE_SRC_SYNTHESIS:
	case IMAGE_SRC_DARKNESS:
	case IMAGE_SRC_SATURATION:
	case IMAGE_SRC_RED_RATIO:
	case IMAGE_SRC_GREEN_RATIO:
	case IMAGE_SRC_BLUE_RATIO:
	case IMAGE_SRC_MAX_GRN_BLU:	
		bGray=true;
		break;
	default:
		bGray=false;
		break;
	}
	return bGray;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckSortAscMode(BOX_TOWARD Toward, ALG_SEARCH_DIRECTION Direction)
{
	bool bAsc=true;
	switch ( Toward )
	{
	case BOX_TOWARD_UP:
		if ( SEARCH_DIRECTION_FORWARD == Direction ) 
		{	bAsc = false; }
		else
		{	bAsc = true; }
		break;
	case BOX_TOWARD_LEFT:
		if ( SEARCH_DIRECTION_FORWARD == Direction ) 
		{	bAsc = false; }
		else
		{	bAsc = true; }
		break;
	case BOX_TOWARD_DOWN:
		if ( SEARCH_DIRECTION_FORWARD == Direction ) 
		{	bAsc = true; }
		else
		{	bAsc = false; }
		break;
	case BOX_TOWARD_RIGHT:
		if ( SEARCH_DIRECTION_FORWARD == Direction ) 
		{	bAsc = true; }
		else
		{	bAsc = false; }
		break;
	}
	return bAsc;
}
//-------------------------------------------------------------------------------------//
BOX_TOWARD CAlgParam::ChangeTowardByDirection(BOX_TOWARD Toward, ALG_SEARCH_DIRECTION Direction)
{
	BOX_TOWARD TowardDirection=Toward;
	if ( SEARCH_DIRECTION_BACKWARD == Direction )
	{
		switch ( Toward )
		{
		case BOX_TOWARD_UP:		TowardDirection = BOX_TOWARD_DOWN;	break;
		case BOX_TOWARD_LEFT:	TowardDirection = BOX_TOWARD_RIGHT;	break;
		case BOX_TOWARD_DOWN:	TowardDirection = BOX_TOWARD_UP;	break;
		case BOX_TOWARD_RIGHT:	TowardDirection = BOX_TOWARD_LEFT;	break;
		}
	}
	return TowardDirection;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::AdjustAlgBinaryParam(FRAME_TYPE FrameType, CAlgBinaryParam &BinaryParam)
{
	TBINARY_FILTER  BinaryFilter;
	int FixThresholdMax = FIXED_THRESHOLD_MAX_2D;
	int FixThresholdMin = FIXED_THRESHOLD_MIN_2D;
	int Relative_BiasMax = RELATIVE_BIAS_MAX_2D;
	int Relative_BiasMin = RELATIVE_BIAS_MIN_2D;
	int Relative_ThresholdMax = RELATIVE_THRESHOLD_MAX_2D;
	int Relative_ThresholdMin = RELATIVE_THRESHOLD_MIN_2D;
	switch ( FrameType )
	{
	case FRAME_GRAY:
		BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_GRAY);
		break;
	case FRAME_BAYER:
	case FRAME_COLOR:	
		if ( IMAGE_SRC_GRAY == BinaryParam.GetBinaryImageSourceMode() ) 
		{	BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_COLOR); }
		break;
	case FRAME_SPACE:		
		FixThresholdMax = FIXED_THRESHOLD_MAX_3D;
		FixThresholdMin = FIXED_THRESHOLD_MIN_3D;
		Relative_BiasMax = RELATIVE_BIAS_MAX_3D;
		Relative_BiasMin = RELATIVE_BIAS_MIN_3D;
		Relative_ThresholdMax = RELATIVE_THRESHOLD_MAX_3D;
		Relative_ThresholdMin = RELATIVE_THRESHOLD_MIN_3D;

		BinaryParam.SetBinaryImageSourceMode(IMAGE_SRC_GRAY);
		switch ( BinaryParam.GetBinaryMode() ) 
		{
		case BINARY_COLOR_FILTER:
		case BINARY_DYNAMIC_THRESHOLD:
			BinaryParam.SetBinaryMode(BINARY_DISABLE);
			break;
		}
		BinaryParam.SetBinaryNoiseFilter1(BinaryFilter);
		BinaryParam.SetBinaryNoiseFilter2(BinaryFilter);
		BinaryParam.SetFixedThresholdHigh(FixThresholdMax);
		break;
	}	

	if ( BinaryParam.GetFixedThresholdLow() <  FixThresholdMin ) { BinaryParam.SetFixedThresholdLow(FixThresholdMin); }
	if ( BinaryParam.GetFixedThresholdLow() >  FixThresholdMax ) { BinaryParam.SetFixedThresholdLow(FixThresholdMax); }
	if ( BinaryParam.GetFixedThresholdHigh() <  FixThresholdMin ) { BinaryParam.SetFixedThresholdHigh(FixThresholdMin); }
	if ( BinaryParam.GetFixedThresholdHigh() >  FixThresholdMax ) { BinaryParam.SetFixedThresholdHigh(FixThresholdMax); }
	if ( BinaryParam.GetRelativeAveThresholdAbove() <  Relative_ThresholdMin ) { BinaryParam.SetRelativeAveThresholdAbove(Relative_ThresholdMin); }
	if ( BinaryParam.GetRelativeAveThresholdBelow() >  Relative_ThresholdMax ) { BinaryParam.SetRelativeAveThresholdBelow(Relative_ThresholdMax); }
	if ( BinaryParam.GetRelativeAveThresholdBias() < Relative_BiasMin ) { BinaryParam.SetRelativeAveThresholdBias(Relative_BiasMin); }
	if ( BinaryParam.GetRelativeAveThresholdBias() > Relative_BiasMax ) { BinaryParam.SetRelativeAveThresholdBias(Relative_BiasMax); }
	return true;
}
//-------------------------------------------------------------------------------------//
CAlgParam::CAlgParam()
{
	CAlgParam::PreInitAlgParam();
	CAlgParam::InitialAlgParam();
}
//-------------------------------------------------------------------------------------//
CAlgParam::CAlgParam(const CAlgParam &Param)
{
	CAlgParam::PreInitAlgParam();
	CAlgParam::CloneAlgParam(Param);
}
//-------------------------------------------------------------------------------------//
CAlgParam::~CAlgParam()
{

}
//-------------------------------------------------------------------------------------//
CAlgParam& CAlgParam::operator=(const CAlgParam &Param)
{
	if ( &Param == this ) { return *this; }
	CAlgParam::CloneAlgParam(Param);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CAlgParam::PreInitAlgParam()
{
}
//-------------------------------------------------------------------------------------//
inline void CAlgParam::InitialAlgParam()
{
	m_AlgType = ALG_BRIGHT_RATIO;
	m_AlgGroupID = -1;
	m_AlgWndPtr = NULL;

	m_AlgBaseValueEnabled = false;
	m_AlgBaseValueGroupID = 0;
	m_AlgBaseValueReading = 0.0;

	m_AlgPatternCount = 0;	
	m_AlgPatternTestAll = false;
	m_AlgPatternPolarity = 1;
	m_AlgPatternResultIndex = -1;	
	m_AlgPatternSimilarityUSL = 100;    //演算法-樣板相似度上限
	m_AlgPatternSimilarityLSL =  60;    //演算法-樣板相似度下限
	m_AlgPatternSimilarityReading = 0;//演算法-樣板相似度讀值	
	m_AlgPatternAngleExpand = 5.0;      //演算法-樣板角度外擴值
	m_AlgPatternScaleExpand = 5.0;      //演算法-樣板縮放外擴值
	m_AlgPatternScaleIsotropic = true;   //演算法-樣板縮放等方向性
	m_AlgPatternMinReducedArea = 128; //For iMatch 
	m_AlgPatternFinalReduction = 0;   //演算法-樣板最末殘餘層數	
	m_AlgPatternAdvancedLearning = true; //演算法-樣板進階學習
	m_AlgPatternFolder = _T("");
	m_AlgPatternFileUsed = false;	
	m_AlgPatternParamList.clear();
	
	m_AlgOffsetXEnabled = true;           //演算法-偏移X-啟用
	m_AlgOffsetXUSL =  250;              //演算法-偏移X 最大偏移量
	m_AlgOffsetXLSL = -250;              //演算法-偏移X 最小偏移量
	m_AlgOffsetYEnabled = true;           //演算法-偏移Y-啟用
	m_AlgOffsetYUSL =  250;              //演算法-偏移Y 最大偏移量
	m_AlgOffsetYLSL = -250;              //演算法-偏移Y 最小偏移量
	m_AlgOffsetLEnabled = false;
	m_AlgOffsetLUSL = 250;
	m_AlgOffsetLLSL = 0;
	m_AlgOffsetAEnabled = false;;          //演算法-偏移角-啟用-垂直偏移角
	m_AlgOffsetAUSL = 10;              //演算法-偏移角 最大偏移量
	m_AlgOffsetALSL =-10;              //演算法-偏移角 最小偏移量	
	m_AlgSkewEnabled = false;              //演算法-角度-啟用
	m_AlgSkewUSL =  10;                 //演算法-角度 最大偏移量
	m_AlgSkewLSL = -10;                 //演算法-角度 最小偏移量	
	m_AlgScaleEnabled=false;            //演算法-縮放 啟用
	m_AlgScaleUSL = 125;                //演算法-縮放 最大縮放值
	m_AlgScaleLSL =  75;                //演算法-縮放 最小縮放值

	m_AlgImageOffsetX = 0;            //演算法-影像偏移X 讀值
	m_AlgImageOffsetY = 0;            //演算法-影像偏移Y 讀值

	m_AlgOffsetXReading = 0;          //演算法-偏移X 讀值
	m_AlgOffsetYReading = 0;          //演算法-偏移Y 讀值
	m_AlgOffsetLReading = 0;
	m_AlgOffsetAReading = 0;          //演算法-Cad偏移角 讀值	
	m_AlgSkewReading = 0;             //演算法-角度 讀值		
	m_AlgScaleXReading = 100.0;           //演算法-縮放X 讀值
	m_AlgScaleYReading = 100.0;           //演算法-縮放X 讀值
	
	m_AlgSaveDefectImageName = _T("");
	m_AlgSaveDefectImageDone = false;
	m_AlgSaveDefectImageEnabled = false;

	m_AlgResultID  = RESULT_ID_NONE;
	m_AlgResultReading1 = 0;
	m_AlgResultReading2 = 0;
	m_AlgResultReading3 = 0;
	m_AlgResultText = _T("");
	
	m_AlgMaskBinParam.SetBinaryBelongToWho(BIN_PARAM_BELONG_TO_MASK_IMAGE);
	m_AlgImageBinParam.SetBinaryBelongToWho(BIN_PARAM_BELONG_TO_ALG_IMAGE);
	m_AlgBinParamActived = BIN_PARAM_BELONG_TO_ALG_IMAGE;

	m_AlgParamBrightRatio = TALG_PARAM_BRIGHT_RATIO();	
	m_AlgParamOuterShort = TALG_PARAM_OUTER_SHORT();	
	m_AlgParamBlobCount = TALG_PARAM_BLOB_COUNT();
	m_AlgParamBodyTilt = TALG_PARAM_BODY_TILT();
	m_AlgParamModelMatch = TALG_PARAM_MODEL_MATCH();
	m_AlgParamImageMatch = TALG_PARAM_IMAGE_MATCH();
	m_AlgParamCharVerify = TALG_PARAM_CHAR_VERIFY();	
	m_AlgParamGroupCompare = TALG_PARAM_GROUP_COMPARE();	
	m_AlgParamBarcodeRecognize = TALG_PARAM_BARCODE_RECOGNIZE();
	m_AlgParamObjectMeasure = TALG_PARAM_OBJECT_MEASURE();	
	m_AlgParamColorCode = TALG_PARAM_COLOR_CODE();
	m_AlgParamFdMatch = TALG_PARAM_FD_MATCH();
	m_AlgParamEdgeSearch = TALG_PARAM_EDGE_SEARCH();
	m_AlgParamShapeVerify = TALG_PARAM_SHAPE_VERIFY();	
	m_AlgParamAngleMeasure = TALG_PARAM_ANGLE_MEASURE();	
	m_AlgParamPixelCompare = TALG_PARAM_PIXEL_COMPARE();
	m_AlgParamIPC = TALG_PARAM_IPC_PRODUCT();
	m_AlgParamResinHight = TALG_PARAM_RESIN_HEIGHT();
	m_AlgParamWireWidth = TALG_PARAM_WIRE_WIDTH();
	m_AlgParamAiModel = TALG_PARAM_AI_MODEL();	
	m_AlgParamSolderWetting = TALG_PARAM_SOLDER_WETTING();
	m_AlgParamMeasureBlackGlue = TALG_PARAM_MEASURE_BLACK_GLUE();	
	m_AlgParamMeasureFluxArea = TALG_PARAM_MEASURE_FLUX_AREA();		
	m_AlgParamMeasureCpuPin = TALG_PARAM_MEASURE_CPU_PIN(); 
	m_AlgParamMeasureSIP = TALG_PARAM_MEASURE_SIP();
	m_AlgParamMeasureConnector = TALG_PARAM_MEASURE_CONNECTOR();
	return;
}
//-------------------------------------------------------------------------------------//
inline void CAlgParam::CloneAlgParam(const CAlgParam &Param)
{
	m_AlgType = Param.m_AlgType;
	m_AlgGroupID = Param.m_AlgGroupID;
	m_AlgWndPtr = Param.m_AlgWndPtr;

	m_AlgBaseValueEnabled = Param.m_AlgBaseValueEnabled;
	m_AlgBaseValueGroupID = Param.m_AlgBaseValueGroupID;
	m_AlgBaseValueReading = Param.m_AlgBaseValueReading;

	m_AlgPatternCount = Param.m_AlgPatternCount;
	m_AlgPatternTestAll = Param.m_AlgPatternTestAll;
	m_AlgPatternPolarity = Param.m_AlgPatternPolarity;
	m_AlgPatternResultIndex = Param.m_AlgPatternResultIndex;	
	m_AlgPatternSimilarityUSL = Param.m_AlgPatternSimilarityUSL;    //演算法-樣板相似度上限
	m_AlgPatternSimilarityLSL = Param.m_AlgPatternSimilarityLSL;    //演算法-樣板相似度下限
	m_AlgPatternSimilarityReading = Param.m_AlgPatternSimilarityReading;//演算法-樣板相似度讀值	
	m_AlgPatternAngleExpand = Param.m_AlgPatternAngleExpand;      //演算法-樣板角度外擴值
	m_AlgPatternScaleExpand = Param.m_AlgPatternScaleExpand;      //演算法-樣板縮放外擴值
	m_AlgPatternScaleIsotropic = Param.m_AlgPatternScaleIsotropic;      //演算法-樣板縮放等方向性 	
	m_AlgPatternMinReducedArea = Param.m_AlgPatternMinReducedArea;
	m_AlgPatternFinalReduction = Param.m_AlgPatternFinalReduction;   //演算法-樣板最末殘餘層數
	m_AlgPatternAdvancedLearning = Param.m_AlgPatternAdvancedLearning; //演算法-樣板進階學習
	m_AlgPatternFolder = Param.m_AlgPatternFolder;
	m_AlgPatternFileUsed = Param.m_AlgPatternFileUsed;	
	m_AlgPatternParamList = Param.m_AlgPatternParamList;

	m_AlgOffsetXEnabled = Param.m_AlgOffsetXEnabled;       //演算法-偏移X-啟用
	m_AlgOffsetXUSL =  Param.m_AlgOffsetXUSL;              //演算法-偏移X 最大偏移量
	m_AlgOffsetXLSL =  Param.m_AlgOffsetXLSL;              //演算法-偏移X 最小偏移量
	m_AlgOffsetYEnabled = Param.m_AlgOffsetYEnabled;       //演算法-偏移Y-啟用
	m_AlgOffsetYUSL =  Param.m_AlgOffsetYUSL;              //演算法-偏移Y 最大偏移量
	m_AlgOffsetYLSL = Param.m_AlgOffsetYLSL;               //演算法-偏移Y 最小偏移量
	m_AlgOffsetLEnabled = Param.m_AlgOffsetLEnabled;
	m_AlgOffsetLUSL = Param.m_AlgOffsetLUSL;
	m_AlgOffsetLLSL = Param.m_AlgOffsetLLSL;
	m_AlgOffsetAEnabled = Param.m_AlgOffsetAEnabled;          //演算法-偏移角-啟用-垂直偏移角
	m_AlgOffsetAUSL = Param.m_AlgOffsetAUSL;              //演算法-偏移角 最大偏移量
	m_AlgOffsetALSL = Param.m_AlgOffsetALSL;              //演算法-偏移角 最小偏移量

	m_AlgSkewEnabled = Param.m_AlgSkewEnabled;             //演算法-角度-啟用
	m_AlgSkewUSL =  Param.m_AlgSkewUSL;                    //演算法-角度 最大偏移量
	m_AlgSkewLSL =  Param.m_AlgSkewLSL;                    //演算法-角度 最小偏移量	

	m_AlgScaleEnabled =  Param.m_AlgScaleEnabled;          //演算法-縮放 啟用
	m_AlgScaleUSL =  Param.m_AlgScaleUSL;                  //演算法-縮放 最大縮放值
	m_AlgScaleLSL =  Param.m_AlgScaleLSL;                  //演算法-縮放 最小縮放值
	

	m_AlgImageOffsetX = 0;            //演算法-影像偏移X 讀值
	m_AlgImageOffsetY = 0;            //演算法-影像偏移Y 讀值

	m_AlgOffsetXReading = 0;          //演算法-偏移X 讀值
	m_AlgOffsetYReading = 0;          //演算法-偏移Y 讀值
	m_AlgOffsetLReading = 0;
	m_AlgOffsetAReading = 0;          //演算法-Cad偏移角 讀值	
	m_AlgSkewReading = 0;             //演算法-角度 讀值	
	m_AlgScaleXReading = 100.0;       //演算法-縮放X 讀值
	m_AlgScaleYReading = 100.0;       //演算法-縮放X 讀值

	m_AlgSaveDefectImageName = Param.m_AlgSaveDefectImageName;
	m_AlgSaveDefectImageDone = Param.m_AlgSaveDefectImageDone;	
	m_AlgSaveDefectImageEnabled = Param.m_AlgSaveDefectImageEnabled;

	m_AlgResultID = Param.m_AlgResultID;
	m_AlgResultReading1 = Param.m_AlgResultReading1;
	m_AlgResultReading2 = Param.m_AlgResultReading2;
	m_AlgResultReading3 = Param.m_AlgResultReading3;	
	m_AlgResultText = Param.m_AlgResultText;	

	m_AlgMaskBinParam = Param.m_AlgMaskBinParam;
	m_AlgImageBinParam = Param.m_AlgImageBinParam;
	m_AlgBinParamActived = Param.m_AlgBinParamActived;	

	m_AlgParamBrightRatio = Param.m_AlgParamBrightRatio;	
	m_AlgParamOuterShort = Param.m_AlgParamOuterShort;		
	m_AlgParamBlobCount = Param.m_AlgParamBlobCount;	
	m_AlgParamBodyTilt = Param.m_AlgParamBodyTilt;
	m_AlgParamModelMatch = Param.m_AlgParamModelMatch;
	m_AlgParamImageMatch = Param.m_AlgParamImageMatch;
	m_AlgParamCharVerify = Param.m_AlgParamCharVerify;
	m_AlgParamGroupCompare = Param.m_AlgParamGroupCompare;
	m_AlgParamBarcodeRecognize = Param.m_AlgParamBarcodeRecognize;
	m_AlgParamObjectMeasure = Param.m_AlgParamObjectMeasure;	
	m_AlgParamColorCode = Param.m_AlgParamColorCode;
	m_AlgParamFdMatch = Param.m_AlgParamFdMatch;
	m_AlgParamEdgeSearch  = Param.m_AlgParamEdgeSearch;
	m_AlgParamShapeVerify  = Param.m_AlgParamShapeVerify;	
	m_AlgParamAngleMeasure  = Param.m_AlgParamAngleMeasure;	
	m_AlgParamPixelCompare  = Param.m_AlgParamPixelCompare;
	m_AlgParamIPC = Param.m_AlgParamIPC;
	m_AlgParamResinHight = Param.m_AlgParamResinHight;
	m_AlgParamWireWidth = Param.m_AlgParamWireWidth;
	m_AlgParamAiModel  = Param.m_AlgParamAiModel;		
	m_AlgParamSolderWetting = Param.m_AlgParamSolderWetting;
	m_AlgParamMeasureBlackGlue = Param.m_AlgParamMeasureBlackGlue;
	m_AlgParamMeasureFluxArea = Param.m_AlgParamMeasureFluxArea;	
	m_AlgParamMeasureCpuPin = Param.m_AlgParamMeasureCpuPin;
	m_AlgParamMeasureSIP = Param.m_AlgParamMeasureSIP;
	m_AlgParamMeasureConnector = Param.m_AlgParamMeasureConnector;
}
//-------------------------------------------------------------------------------------//
void CAlgParam::SwapAlgParam(int &v1, int &v2)
{
	int tmp=v1;
	v1=v2;
	v2=tmp;
}
//-------------------------------------------------------------------------------------//
void CAlgParam::SwapAlgParam(bool &v1, bool &v2)
{
	bool tmp=v1;
	v1=v2;
	v2=tmp;
}
//-------------------------------------------------------------------------------------//
void CAlgParam::SwapAlgParam(double &v1, double &v2)
{
	double tmp=v1;
	v1=v2;
	v2=tmp;
}
//-------------------------------------------------------------------------------------//
void CAlgParam::SwapAlgParam(ALG_CALC_UNIT_MODE &v1, ALG_CALC_UNIT_MODE &v2)
{
	ALG_CALC_UNIT_MODE tmp=v1;
	v1=v2;
	v2=tmp;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile(CAOIFileIO &FileIO)//儲存演算法參數
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG	
	
	int        i=0;
	CAOIWnd   *WndPtr = NULL;
	CAOIModel *ModelPtr = NULL;
	CAlgParam *AlgParamPtr = this;		
	FileIO.SetFnName(_T("CAlgParam::WriteAlgParamFile"));	
	//----------------------------------------------------------------------------------------//	
	CAlgBinaryParam                  *MaskBinaryParamPtr = AlgParamPtr->GetAlgMaskBinParamPtr();
	CAlgBinaryParam                  *ImageBinaryParamPtr = AlgParamPtr->GetAlgImageBinParamPtr();	
	const TALG_PARAM_BRIGHT_RATIO    &brParam=AlgParamPtr->GetAlgParamBrightRatio();//演算法-亮度比例參數	
	const TALG_PARAM_OUTER_SHORT     &osParam=AlgParamPtr->GetAlgParamOuterShort();//演算法-外接短路參數
	const TALG_PARAM_BLOB_COUNT      &blobParam=AlgParamPtr->GetAlgParamBlobCount();//演算法-區塊數量參數
	const TALG_PARAM_MODEL_MATCH     &mmParam=AlgParamPtr->GetAlgParamModelMatch();//演算法-模板匹配數
	const TALG_PARAM_IMAGE_MATCH     &imParam=AlgParamPtr->GetAlgParamImageMatch();//演算法-影像匹配參數
	const TALG_PARAM_CHAR_VERIFY     &cvParam=AlgParamPtr->GetAlgParamCharVerify();//演算法-文字驗證參數
	const TALG_PARAM_GROUP_COMPARE   &gcParam=AlgParamPtr->GetAlgParamGroupCompare();//演算法-群組比較參數	
	const TALG_PARAM_BARCODE_RECOGNIZE &barParam = AlgParamPtr->GetAlgParamBarcodeRecognize();//演算法條碼辨識
	const TALG_PARAM_COLOR_CODE      &ccParam = AlgParamPtr->GetAlgParamColorCode();//演算法色碼辨識
	const TALG_PARAM_FD_MATCH        &fdParam = AlgParamPtr->GetAlgParamFdMatch();//演算法定位點匹配
	const TALG_PARAM_EDGE_SEARCH     &esParam = AlgParamPtr->GetAlgParamEdgeSearch();//演算法邊緣搜尋
	const TALG_PARAM_OBJECT_MEASURE  &omParam = AlgParamPtr->GetAlgParamObjectMeasure();//演算法物件量測
	const TALG_PARAM_SHAPE_VERIFY    &svParam = AlgParamPtr->GetAlgParamShapeVerify();//演算法外形驗證
	const TALG_PARAM_ANGLE_MEASURE   &amParam = AlgParamPtr->GetAlgParamAngleMeasure();//演算法角度量測	
	const TALG_PARAM_PIXEL_COMPARE   &pcParam = AlgParamPtr->GetAlgParamPixelCompare();//演算法像素比較	
	const TALG_PARAM_IPC_PRODUCT	 &ipcParam = AlgParamPtr->GetAlgParamIPC();//IPC產品
	const TALG_PARAM_RESIN_HEIGHT	 &heightParam = AlgParamPtr->GetAlgParamResinHeight();
	const TALG_PARAM_WIRE_WIDTH      &wireWidthParm = AlgParamPtr->GetAlgParamWireWidth();
	const TALG_PARAM_AI_MODEL        &aiParam = AlgParamPtr->GetAlgParamAiModel();//AI模型	
	const TALG_PARAM_SOLDER_WETTING	 &swParam = AlgParamPtr->GetAlgParamSolderWetting();
	const TALG_PARAM_MEASURE_BLACK_GLUE	 &bgParam = AlgParamPtr->GetAlgParamMeasureBlackGlue();	
	const TALG_PARAM_MEASURE_FLUX_AREA	 &faParam = AlgParamPtr->GetAlgParamMeasureFluxArea();	
	const TALG_PARAM_MEASURE_SIP	 &msParam = AlgParamPtr->GetAlgParamMeasureSIP();
	const TALG_PARAM_MEASURE_CONNECTOR	 &mcParam = AlgParamPtr->GetAlgParamMeasureConnector();
	//----------------------------------------------------------------------------------------//		
	if ( AlgParamPtr->GetAlgPatternFileUsed() == true )
	{
		WndPtr = AlgParamPtr->GetAlgWndPtr();
		const int  PatternFolderIndex = AlgParamPtr->GetAlgGroupID();
		if ( NULL != WndPtr )
		{
			ModelPtr = WndPtr->GetWndModelPtr();
			if ( NULL != ModelPtr )
			{			
				CString ModelFolder = ModelPtr->GetModelFolder();						
				CString PatternFolder = AOIDataDefine.GetAlgPatternFolder(PatternFolderIndex);
				CString AlgPatternFolder = ModelFolder+CString(_T("\\"))+PatternFolder;
				AlgParamPtr->SetAlgPatternFolder(AlgPatternFolder);			
			}
		}
	}
	//----------------------------------------------------------------------------------------//
	//基本參數
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_START, 0) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_ALG_TYPE, AlgParamPtr->GetAlgType()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_GROUP_ID, AlgParamPtr->GetAlgGroupID()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_SAVE_DEFECT_IMAGE_ENABLED, AlgParamPtr->GetAlgSaveDefectImageEnabled()) == false ) { return false; }

	//基準值
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BASE_VALUE_ENABLED, AlgParamPtr->GetAlgBaseValueEnabled()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BASE_VALUE_GROUP_ID, AlgParamPtr->GetAlgBaseValueGroupID()) == false ) { return false; }	

	//偏移量-X, Y, Angle
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OFFSET_X_USL, AlgParamPtr->GetAlgOffsetXUSL()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OFFSET_X_LSL, AlgParamPtr->GetAlgOffsetXLSL()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_OFFSET_X_ENABLED, AlgParamPtr->GetAlgOffsetXEnabled()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OFFSET_Y_USL, AlgParamPtr->GetAlgOffsetYUSL()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OFFSET_Y_LSL, AlgParamPtr->GetAlgOffsetYLSL()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_OFFSET_Y_ENABLED, AlgParamPtr->GetAlgOffsetYEnabled()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_SKEW_ANGLE_USL, AlgParamPtr->GetAlgSkewUSL()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_SKEW_ANGLE_LSL, AlgParamPtr->GetAlgSkewLSL()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_SKEW_ANGLE_ENABLED, AlgParamPtr->GetAlgSkewEnabled()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_SCALE_USL, AlgParamPtr->GetAlgScaleUSL()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_SCALE_LSL, AlgParamPtr->GetAlgScaleLSL()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_SCALE_ENABLED, AlgParamPtr->GetAlgScaleEnabled()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OFFSET_A_USL, AlgParamPtr->GetAlgOffsetAUSL()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OFFSET_A_LSL, AlgParamPtr->GetAlgOffsetALSL()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_OFFSET_A_ENABLED, AlgParamPtr->GetAlgOffsetAEnabled()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OFFSET_L_USL, AlgParamPtr->GetAlgOffsetLUSL()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_OFFSET_L_LSL, AlgParamPtr->GetAlgOffsetLLSL()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_OFFSET_L_ENABLED, AlgParamPtr->GetAlgOffsetLEnabled()) == false ) { return false; }

	//2值化參數
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MASK_BINARY_PARAM, 0) == false ) { return false; }
	if ( MaskBinaryParamPtr->WriteAlgBinaryParamFile(FileIO) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IMAGE_BINARY_PARAM, 0) == false ) { return false; }	
	if ( ImageBinaryParamPtr->WriteAlgBinaryParamFile(FileIO) == false ) { return false; }	

	//樣板匹配
	if ( WriteAlgParamFile_PatternParam(AlgParamPtr, FileIO) == false )
	{	return false; }

	//演算法-亮度比例
	if ( WriteAlgParamFile_BrightRatio(brParam, FileIO) == false )
	{	return false; }

	//演算法-外接短路參數	
	if ( WriteAlgParamFile_OuterShort(osParam, FileIO) == false )
	{	return false; }	
	
	//演算法參數-區塊數量參數	
	if ( WriteAlgParamFile_BlobCount(blobParam, FileIO) == false )
	{	return false; }	
	
	//演算法參數-模板匹配參數	
	if ( WriteAlgParamFile_ModelMatch(mmParam, FileIO) == false )
	{	return false; }	

	//演算法參數-影像匹配參數	
	if ( WriteAlgParamFile_ImageMatch(imParam, FileIO) == false )
	{	return false; }	

	//演算法參數-文字驗證參數	
	if ( WriteAlgParamFile_CharVerify(cvParam, FileIO) == false )
	{	return false; }	

	//演算法參數-群組比較參數	
	if ( WriteAlgParamFile_GroupCompare(gcParam, FileIO) == false )
	{	return false; }				

	//演算法參數-條碼辨識參數	
	if ( WriteAlgParamFile_BarcodeRecognize(barParam, FileIO) == false )
	{	return false; }	

	//色碼檢測	
	if ( WriteAlgParamFile_ColorCode(ccParam, FileIO) == false )
	{	return false; }	
		
	//演算法定位點匹配
	if ( WriteAlgParamFile_FdMatch(fdParam, FileIO) == false )
	{	return false; }	
	
	//演算法物件量測
	if ( WriteAlgParamFile_ObjectMeasure(omParam, FileIO) == false )
	{	return false; }	
	
	//演算法邊緣搜尋
	if ( WriteAlgParamFile_EdgeSearch(esParam, FileIO) == false )
	{	return false; }

	//演算法外形驗證
	if ( WriteAlgParamFile_ShapeVerify(svParam, FileIO) == false )
	{	return false; }

	//演算法角度量測
	if ( WriteAlgParamFile_AngleMeasure(amParam, FileIO) == false )
	{	return false; }

	//演算法像素比較
	if ( WriteAlgParamFile_PixelCompare(pcParam, FileIO) == false )
	{	return false; }
	
	if (WriteAlgParamFile_IPC(ipcParam, FileIO) == false )
	{	return false; }

	if (WriteAlgParamFile_Height(heightParam, FileIO) == false)
	{	return false; }

	if (WriteAlgParamFile_Wire(wireWidthParm, FileIO) == false)
	{	return false; }

	//演算法AI模型
	if (WriteAlgParamFile_AiModel(aiParam, FileIO) == false )
	{	return false; }

	//演算法焊接檢測
	if (WriteAlgParamFile_SolderWetting(swParam, FileIO) == false )
	{	return false; }

	//演算法量測黑膠
	if ( WriteAlgParamFile_MeasureBlackGlue(bgParam, FileIO) == false )
	{	return false; }

	//演算法量測Flux面積	
	if ( WriteAlgParamFile_MeasureFluxArea(faParam, FileIO) == false )
	{	return false; }

	//演算法量測SIP	
	if (WriteAlgParamFile_MeasureSIP(msParam, FileIO) == false)
	{	return false; }

	//演算法量測Connector	
	if (WriteAlgParamFile_MeasureConnector(mcParam, FileIO) == false)
	{	return false; }

	//最末結束
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile(CAOIFileIO &FileIO)//載入演算法參數
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	
	int       index = 0;
	int       nValue = 0;
	double    dValue = 0.0;
	size_t    szCount = 0;	
	CAlgParam *AlgParamPtr = this;
	FileIO.SetFnName(_T("CAlgParam::ReadAlgParamFile"));	
	//----------------------------------------------------------------------------------------//
	CPatternParam               PatParam;
	CAlgBinaryParam            *MaskBinaryParamPtr = AlgParamPtr->GetAlgMaskBinParamPtr();
	CAlgBinaryParam            *ImageBinaryParamPtr = AlgParamPtr->GetAlgImageBinParamPtr();
	TALG_PARAM_BRIGHT_RATIO    &brParam=AlgParamPtr->GetAlgParamBrightRatio();//演算法-亮度比例參數		
	TALG_PARAM_OUTER_SHORT     &osParam=AlgParamPtr->GetAlgParamOuterShort();//演算法-外接短路參數
	TALG_PARAM_BLOB_COUNT      &blobParam=AlgParamPtr->GetAlgParamBlobCount();//演算法-區塊數量參數	
	TALG_PARAM_MODEL_MATCH     &mmParam=AlgParamPtr->GetAlgParamModelMatch();//演算法-模板匹配參數	
	TALG_PARAM_IMAGE_MATCH     &imParam=AlgParamPtr->GetAlgParamImageMatch();//演算法-影像匹配參數	
	TALG_PARAM_CHAR_VERIFY     &cvParam=AlgParamPtr->GetAlgParamCharVerify();//演算法-文字驗證參數	
	TALG_PARAM_GROUP_COMPARE   &gcParam=AlgParamPtr->GetAlgParamGroupCompare();//演算法-群組比較參數
	TALG_PARAM_BARCODE_RECOGNIZE &barParam = AlgParamPtr->GetAlgParamBarcodeRecognize();//演算法條碼辨識	
	TALG_PARAM_COLOR_CODE      &ccParam = AlgParamPtr->GetAlgParamColorCode();//演算法色碼辨識	
	TALG_PARAM_FD_MATCH        &fdParam = AlgParamPtr->GetAlgParamFdMatch();//演算法定位點匹配
	TALG_PARAM_EDGE_SEARCH     &esParam = AlgParamPtr->GetAlgParamEdgeSearch();//演算法邊緣搜尋		
	TALG_PARAM_OBJECT_MEASURE  &omParam = AlgParamPtr->GetAlgParamObjectMeasure();//演算法物件量測	
	TALG_PARAM_SHAPE_VERIFY    &svParam = AlgParamPtr->GetAlgParamShapeVerify();//演算法外形驗證
	TALG_PARAM_ANGLE_MEASURE   &amParam = AlgParamPtr->GetAlgParamAngleMeasure();//演算法角度量測
	TALG_PARAM_PIXEL_COMPARE   &pcParam = AlgParamPtr->GetAlgParamPixelCompare();//演算法像素比較	
	TALG_PARAM_IPC_PRODUCT	   &ipcParam = AlgParamPtr->GetAlgParamIPC();		//IPC產品
	TALG_PARAM_RESIN_HEIGHT	   &heightParam = AlgParamPtr->GetAlgParamResinHeight();
	TALG_PARAM_WIRE_WIDTH      &wireWidthParm = AlgParamPtr->GetAlgParamWireWidth();
	TALG_PARAM_AI_MODEL	       &aiParam = AlgParamPtr->GetAlgParamAiModel();		//AI模型	
	TALG_PARAM_SOLDER_WETTING  &swParam = AlgParamPtr->GetAlgParamSolderWetting();	//焊接檢測
	TALG_PARAM_MEASURE_BLACK_GLUE  &bgParam = AlgParamPtr->GetAlgParamMeasureBlackGlue();//量測黑膠	
	TALG_PARAM_MEASURE_FLUX_AREA  &faParam = AlgParamPtr->GetAlgParamMeasureFluxArea();//量測Flux面積
	TALG_PARAM_MEASURE_SIP  &msParam = AlgParamPtr->GetAlgParamMeasureSIP();//量測SIP
	TALG_PARAM_MEASURE_CONNECTOR  &mcParam = AlgParamPtr->GetAlgParamMeasureConnector();//量測Connector
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }

		switch ( index )
		{
		case FILE_IO_ALG_PARAM_START://演算法參數-起點			
			SetAlgPatternCount(0);
			ClearAlgPatternParamList();//清除樣板參數列表			
			break;
		case FILE_IO_ALG_PARAM_END://演算法參數-終點
			szCount = GetAlgPatternParamCount();
			m_AlgPatternFileUsed = CAlgParam::CheckAlgPatternFileUsed(m_AlgType);
			if ( m_AlgPatternCount!=szCount || false==m_AlgPatternFileUsed)
			{
				SetAlgPatternCount(0);
				ClearAlgPatternParamList();//清除樣板參數列表
			}
			if ( 0 == m_AlgPatternCount )
			{	SetAlgPatternResultIndex(-1);	}
			else
			{	SetAlgPatternResultIndex(0);	}
			//LandPtr->UpdateWndExtendBox();			
			return true;
			break;
		case FILE_IO_ALG_PARAM_ALG_TYPE://演算法參數-演算法樣式
			AlgParamPtr->SetAlgType((ALG_TYPE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_ALG_PARAM_GROUP_ID://演算法參數-群組編號
			AlgParamPtr->SetAlgGroupID(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_SAVE_DEFECT_IMAGE_ENABLED://演算法參數-存瑕疵圖啟用
			AlgParamPtr->SetAlgSaveDefectImageEnabled(FileIO.GetData_BOL());
			break;
		case FILE_IO_ALG_PARAM_BASE_VALUE_ENABLED://演算法參數-基準值啟用
			AlgParamPtr->SetAlgBaseValueEnabled(FileIO.GetData_BOL());
			break;
		case FILE_IO_ALG_PARAM_BASE_VALUE_GROUP_ID://演算法參數-基準值群組編號
			AlgParamPtr->SetAlgBaseValueGroupID(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_OFFSET_X_USL://演算法參數-偏移上限-X
			AlgParamPtr->SetAlgOffsetXUSL(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_OFFSET_X_LSL://演算法參數-偏移下限-X
			AlgParamPtr->SetAlgOffsetXLSL(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_OFFSET_X_ENABLED://演算法參數-偏移啟用-X
			AlgParamPtr->SetAlgOffsetXEnabled(FileIO.GetData_BOL());
			break;
		case FILE_IO_ALG_PARAM_OFFSET_Y_USL://演算法參數-偏移上限-Y
			AlgParamPtr->SetAlgOffsetYUSL(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_OFFSET_Y_LSL://演算法參數-偏移下限-Y
			AlgParamPtr->SetAlgOffsetYLSL(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_OFFSET_Y_ENABLED://演算法參數-偏移啟用-Y
			AlgParamPtr->SetAlgOffsetYEnabled(FileIO.GetData_BOL());
			break;
		case FILE_IO_ALG_PARAM_SKEW_ANGLE_USL://演算法參數-偏移上限-角度
			AlgParamPtr->SetAlgSkewUSL(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_SKEW_ANGLE_LSL://演算法參數-偏移下限-角度
			AlgParamPtr->SetAlgSkewLSL(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_SKEW_ANGLE_ENABLED://演算法參數-偏移啟用-角度
			AlgParamPtr->SetAlgSkewEnabled(FileIO.GetData_BOL());
			break;
		case FILE_IO_ALG_PARAM_SCALE_USL://演算法參數-偏移上限-縮放
			AlgParamPtr->SetAlgScaleUSL(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_SCALE_LSL://演算法參數-偏移下限-縮放
			AlgParamPtr->SetAlgScaleLSL(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_SCALE_ENABLED://演算法參數-偏移啟用-縮放
			AlgParamPtr->SetAlgScaleEnabled(FileIO.GetData_BOL());
			break;
		case FILE_IO_ALG_PARAM_OFFSET_A_USL://演算法參數-偏移上限-XY角度
			AlgParamPtr->SetAlgOffsetAUSL(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_OFFSET_A_LSL://演算法參數-偏移下限-XY角度
			AlgParamPtr->SetAlgOffsetALSL(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_OFFSET_A_ENABLED://演算法參數-偏移啟用-XY角度
			AlgParamPtr->SetAlgOffsetAEnabled(FileIO.GetData_BOL());
			break;
		case FILE_IO_ALG_PARAM_OFFSET_L_USL://演算法參數-偏移上限-L
			AlgParamPtr->SetAlgOffsetLUSL(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_OFFSET_L_LSL://演算法參數-偏移下限-L
			AlgParamPtr->SetAlgOffsetLLSL(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_OFFSET_L_ENABLED://演算法參數-偏移啟用-L
			AlgParamPtr->SetAlgOffsetLEnabled(FileIO.GetData_BOL());
			break;
		case FILE_IO_ALG_PARAM_MASK_BINARY_PARAM://演算法參數-二值化參數
			if ( MaskBinaryParamPtr->ReadAlgBinaryParamFile(FileIO) == false )			
			{	return false; }
			MaskBinaryParamPtr->SetBinaryBelongToWho(BIN_PARAM_BELONG_TO_MASK_IMAGE);	
			break;
		case FILE_IO_ALG_PARAM_IMAGE_BINARY_PARAM://演算法參數-影像參數
			if ( ImageBinaryParamPtr->ReadAlgBinaryParamFile(FileIO) == false )			
			{	return false; }
			ImageBinaryParamPtr->SetBinaryBelongToWho(BIN_PARAM_BELONG_TO_ALG_IMAGE);
			break;
		//---------------------------------------------------------------------------------------//
		case FILE_IO_ALG_PARAM_PATTERN_COUNT://演算法參數-樣板圖數量
			AlgParamPtr->SetAlgPatternCount(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_PATTERN_ELABLED://演算法參數-是否使用樣板圖檔案			
			AlgParamPtr->SetAlgPatternFileUsed(FileIO.GetData_BOL());
			break;
		case FILE_IO_ALG_PARAM_PATTERN_SIMILARITY_USL://演算法參數-樣板相似度上限
			AlgParamPtr->SetAlgPatternSimilarityUSL(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_PATTERN_SIMILARITY_LSL://演算法參數-樣板相似度下限
			AlgParamPtr->SetAlgPatternSimilarityLSL(FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_PATTERN_MIN_AREA://演算法參數-樣板最小保留面積
			AlgParamPtr->SetAlgPatternMinReducedArea(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_PATTERN_POLARITY://演算法參數-樣板極性
			nValue = FileIO.GetData_INT();
			switch ( nValue ) 
			{
			case 1:
			case 2:
				AlgParamPtr->SetAlgPatternPolarity(nValue);
				break;
			}			
			break;
		case FILE_IO_ALG_PARAM_PATTERN_FINAL_REDUCTION://演算法參數-樣板最末殘餘層數
			nValue = FileIO.GetData_INT();
			if ( nValue >= 0 ) 
			{	AlgParamPtr->SetAlgPatternFinalReduction(nValue); }
			break;
		case FILE_IO_ALG_PARAM_PATTERN_ANGLE_EXPAND://演算法參數-樣板角度外擴			
			dValue = FileIO.GetData_DBL();
			if ( dValue >= 1.0 ) 
			{	AlgParamPtr->SetAlgPatternAngleExpand(dValue); }
			break;
		case FILE_IO_ALG_PARAM_PATTERN_SCALE_EXPAND://演算法參數-樣板縮放外擴
			dValue = FileIO.GetData_DBL();			
			AlgParamPtr->SetAlgPatternScaleExpand(dValue);
			break;
		case FILE_IO_ALG_PARAM_PATTERN_SCALE_ISOTROPIC://演算法參數-樣板縮放等方向性			
			AlgParamPtr->SetAlgPatternScaleIsotropic(FileIO.GetData_BOL());
			break;
		case FILE_IO_ALG_PARAM_PATTERN_ADVANCED_LEARNING://演算法參數-樣板進階學習
			AlgParamPtr->SetAlgPatternAdvancedLearning(FileIO.GetData_BOL());
			break;
		case FILE_IO_ALG_PARAM_PATTERN_NODE://演算法參數-樣板圖二值化列表			
			break;
		case FILE_IO_PATTERN_PARAM_START:
			PatParam = CPatternParam();
			if ( PatParam.ReadPatternParamFile(FileIO) == false )			
			{	return false; }
			AddAlgPatternParam(PatParam);
			break;
		//---------------------------------------------------------------------------------------//
		case FILE_IO_ALG_PARAM_BRIGHT_RATIO_START://亮度比例參數-起點
			if ( ReadAlgParamFile_BrightRatio(brParam, FileIO) == false )
			{	return false; }
			break;		
		//---------------------------------------------------------------------------------------//			
		case FILE_IO_ALG_PARAM_OUTER_SHORT_START://外接短路參數-起點
			if ( ReadAlgParamFile_OuterShort(osParam, FileIO) == false )
			{	return false; }
			break;		
		//---------------------------------------------------------------------------------------//
		case FILE_IO_ALG_PARAM_BLOB_COUNT_START://區塊數量參數-起點
			if ( ReadAlgParamFile_BlobCount(blobParam, FileIO) == false )
			{	return false; }
			break;
		//---------------------------------------------------------------------------------------//
		case FILE_IO_ALG_PARAM_MODEL_MATCH_START://模板匹配參數-起點
			if ( ReadAlgParamFile_ModelMatch(mmParam, FileIO) == false )
			{	return false; }
			break;		
		//---------------------------------------------------------------------------------------//
		case FILE_IO_ALG_PARAM_IMAGE_MATCH_START://影像匹配參數-起點
			if ( ReadAlgParamFile_ImageMatch(imParam, FileIO) == false )
			{	return false; }	
			break;		
		//---------------------------------------------------------------------------------------//
		case FILE_IO_ALG_PARAM_CHAR_VERIFY_START://文字驗證參數-起點
			if ( ReadAlgParamFile_CharVerify(cvParam, FileIO) == false )
			{	return false; }			
			break;		
		//---------------------------------------------------------------------------------------//
		case FILE_IO_ALG_PARAM_GROUP_COMPARE_START://群組比較參數-起點
			if ( ReadAlgParamFile_GroupCompare(gcParam, FileIO) == false )
			{	return false; }
			break;		
		//---------------------------------------------------------------------------------------//	
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_START://條碼辨識參數-起點
			if ( ReadAlgParamFile_BarcodeRecognize(barParam, FileIO) == false )
			{	return false; }
			break;		
		//---------------------------------------------------------------------------------------//
		case FILE_IO_ALG_PARAM_COLOR_CODE_START://色碼檢測參數-起點
			if ( ReadAlgParamFile_ColorCode(ccParam, FileIO) == false )
			{	return false; }
			break;
		//---------------------------------------------------------------------------------------//
		case FILE_IO_ALG_PARAM_FD_MATCH_START://定位點匹配參數-起點
			if ( ReadAlgParamFile_FdMatch(fdParam, FileIO) == false )
			{	return false; }			
			break;		
		//---------------------------------------------------------------------------------------//			
		case FILE_IO_ALG_PARAM_OBJECT_MEASURE_START://物件量測參數-起點
			if ( ReadAlgParamFile_ObjectMeasure(omParam, FileIO) == false )
			{	return false; }
			break;		
		//---------------------------------------------------------------------------------------//			
		case FILE_IO_ALG_PARAM_EDGE_SEARCH_START://邊緣搜尋參數-起點
			if ( ReadAlgParamFile_EdgeSearch(esParam, FileIO) == false )
			{	return false; }
			break;
		//---------------------------------------------------------------------------------------//			
		case FILE_IO_ALG_PARAM_SHAPE_VERIFY_START://外形驗證參數-起點
			if ( ReadAlgParamFile_ShapeVerify(svParam, FileIO) == false )
			{	return false; }
			break;
		//---------------------------------------------------------------------------------------//			
		case FILE_IO_ALG_PARAM_ANGLE_MEASURE_START://角度量測參數-起點
			if ( ReadAlgParamFile_AngleMeasure(amParam, FileIO) == false )
			{	return false; }
			break;
		//---------------------------------------------------------------------------------------//
		case FILE_IO_ALG_PARAM_PIEXEL_COMPARE_START://像素比較參數-起點
			if ( ReadAlgParamFile_PixelCompare(pcParam, FileIO) == false )
			{	return false; }
			break;			
		//---------------------------------------------------------------------------------------//
		case FILE_IO_ALG_PARAM_IPC_START://IPC對位參數-起點
			if (ReadAlgParamFile_IPC(ipcParam, FileIO) == false)
			{	return false; }
			break;
		//---------------------------------------------------------------------------------------//
		case FILE_IO_ALG_PARAM_HEIGHT_START:
			if (ReadAlgParamFile_Height(heightParam, FileIO) == false)
			{	return false; }
			break;
		//---------------------------------------------------------------------------------------//
		case FILE_IO_ALG_PARAM_WIRE_START:
			if (ReadAlgParamFile_Wire(wireWidthParm, FileIO) == false)
			{	return false; }
			break;
		//---------------------------------------------------------------------------------------//
		case FILE_IO_ALG_PARAM_AI_MODEL_START://AI模型參數-起點
			if (ReadAlgParamFile_AiModel(aiParam, FileIO) == false)
			{	return false; }
			break;
		//---------------------------------------------------------------------------------------//		
		case FILE_IO_ALG_PARAM_SOLDER_WETTING_START://焊接檢測參數-起點
			if (ReadAlgParamFile_SolderWetting(swParam, FileIO) == false)
			{	return false; }
			break;
		//---------------------------------------------------------------------------------------//			
		case FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_START://量測黑膠-起點
			if (ReadAlgParamFile_MeasureBlackGlue(bgParam, FileIO) == false)
			{	return false; }
			break;
		//---------------------------------------------------------------------------------------//			
		case FILE_IO_ALG_PARAM_MEASURE_FLUX_AREA_START://量測Flux面積-起點			
			if (ReadAlgParamFile_MeasureFluxArea(faParam, FileIO) == false)
			{	return false; }
			break;
		//---------------------------------------------------------------------------------------//
		case FILE_IO_ALG_PARAM_MEASURE_SIP_START://量測SIP面積-起點			
			if (ReadAlgParamFile_MeasureSIP(msParam, FileIO) == false)
			{	return false;	}
			break;
		//---------------------------------------------------------------------------------------//
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_START://量測SIP面積-起點			
			if (ReadAlgParamFile_MeasureConnector(mcParam, FileIO) == false)
			{	return false;	}
			break;
		//---------------------------------------------------------------------------------------//
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
bool CAlgParam::WriteAlgSpcFile_AIModel_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr)
{
	CAOIWnd *WndPtr = GetAlgWndPtr();
	if ( NULL == WndPtr ) { return true; }
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	const ALG_TYPE AlgType = GetAlgType();
	const BOX_TOWARD WndToward = WndPtr->GetWndToward();
	const TALG_PARAM_AI_MODEL &aiParam=GetAlgParamAiModel();	
	if ( ALG_AI_MODEL_NONE == aiParam.aiModelID ) { return true; }
	::fwprintf(pfile, L"            \"%s\": %d,\n", L"AImodelID", aiParam.aiModelID);

	const int nPatternAngle=(int)(JetAPI::AdjustRotationAngle(aiParam.aiPatternAngle));
	::fwprintf(pfile, L"            \"%s\": %d,\n", L"OCRImageRotateAngle", nPatternAngle);

	const int nPolarity=GetAlgPatternPolarity();
	::fwprintf(pfile, L"            \"%s\": %d,\n", L"WindowDirection", nPolarity);

	::fwprintf(pfile, L"            \"%s\": %.2f,\n", L"ConfidenceThreshold", aiParam.aiConfidenceThreshold/100.0f);
	::fwprintf(pfile, L"            \"%s\": %d,\n", L"CharMatchNumThreshold", aiParam.aiCharMatchNumThreshold);	
	::fwprintf(pfile, L"            \"%s\": %d,\n", L"CharNumUpperThreshold", aiParam.aiCharNumUpperThreshold);		

	bool bInputStringMode=false;
	std::vector<RECT> CharRectList;
	std::vector<std::wstring> OcrTextList;
	if ( ALG_CHAR_VERIFY == AlgType )
	{	
		const int PolarityIdx = 0;
		GetAlgPatternParamPatTextList(OcrTextList);
		CPatternParam *PatternParamPtr=GetAlgPatternParamPtrByMaxPatRoiCount(WndToward, PolarityIdx);			
		if ( NULL != PatternParamPtr )
		{	
			bInputStringMode = PatternParamPtr->GetPatInputStringMode();
			const std::vector<TPATTERN_ROI> &PatRoiList=PatternParamPtr->GetPatRoiList(WndToward, PolarityIdx);				
			const size_t PatRoiCount=PatRoiList.size();
			for ( size_t j=0; j<PatRoiCount; j++ )
			{
				const TPATTERN_ROI &PatRoi=PatRoiList[j];
				CharRectList.push_back(PatRoi.RoiRect);
			}				
		}			
	}

	const size_t OcrTextCount=OcrTextList.size();
	//"OCRGroundTruthStr": []	
	::fwprintf(pfile, L"            \"%s\":\n", L"OCRGroundTruthStr");	
	::fwprintf(pfile, L"            [\n");//OCRGroundTruthStr Begin
	for ( size_t j=0; j<OcrTextCount; j++ )
	{
		if ( 0 != j )
		{	::fwprintf(pfile, L",\n");	}
		::fwprintf(pfile, L"              \"%s\"", OcrTextList[j].c_str());				
	}
	::fwprintf(pfile, L"\n");		
	::fwprintf(pfile, L"            ],\n");//OCRGroundTruthStr End

	size_t CharRectCount=0;
	if ( false == bInputStringMode )
	{	CharRectCount=CharRectList.size(); }
	::fwprintf(pfile, L"            \"%s\":\n", L"OCRGroundTruthBox");
	::fwprintf(pfile, L"            [\n");//OCRGroundTruthBox Begin
	for ( size_t j=0; j<CharRectCount; j++ )
	{
		if ( 0 != j )
		{	::fwprintf(pfile, L",\n");	}
		const RECT &rc=CharRectList[j];
		::fwprintf(pfile, L"              [%d, %d, %d, %d]", rc.left, rc.top, rc.right, rc.bottom);
	}
	::fwprintf(pfile, L"\n");		
	::fwprintf(pfile, L"            ],\n");//OCRGroundTruthBox End	
	
	//"ROI":	
	RECT WndRect;
	if ( WndPtr->GetWndExtendBoxUsed() == false )
	{	WndRect = WndPtr->GetWndImageRect_Raw(); }
	else
	{	WndRect = WndPtr->GetWndExtendImageRect_Raw(); }
	::fwprintf(pfile, L"            \"%s\":\n", L"Rect");
	::fwprintf(pfile, L"            [\n");//Rect Begin
	//"window rect": [0,0,342,282],		
	::fwprintf(pfile, L"              [%d, %d, %d, %d],\n", WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);
	
	const RECT &ComponentRect=ModelPtr->GetModelBodyImageRect_Raw();
	//"Component rect": [0,0,342,282],		
	::fwprintf(pfile, L"              [%d, %d, %d, %d]\n", ComponentRect.left, ComponentRect.top, ComponentRect.right, ComponentRect.bottom);
	::fwprintf(pfile, L"            ],\n");//Rect End	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgSpcFile_Parameter_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr)
{
	CString str, strKey;
	ALG_TYPE AlgType = GetAlgType();	
	double STD=0, USL=0, LSL=0, Reading=0;	
	std::vector<TAlgParamNode> AlgParamList;
	const bool bShowXY = CheckAlgShowOffsetByAlgType(AlgType);	
	const unsigned int FrameUniqueID=GetAlgImageBinParam().GetBinaryFrameUniqueID();
	
	if ( GetAlgOffsetXEnabled() == true && true==bShowXY )
	{	AlgParamList.push_back(TAlgParamNode(_T("OffsetX"), 0, GetAlgOffsetXUSL(), GetAlgOffsetXLSL(), GetAlgOffsetXReading()));	}
	if ( GetAlgOffsetYEnabled() == true && true==bShowXY  )
	{	AlgParamList.push_back(TAlgParamNode(_T("OffsetY"), 0, GetAlgOffsetYUSL(), GetAlgOffsetYLSL(), GetAlgOffsetYReading()));	}
	if ( GetAlgOffsetAEnabled() == true )
	{	AlgParamList.push_back(TAlgParamNode(_T("OffsetA"), 0, GetAlgOffsetAUSL(), GetAlgOffsetALSL(), GetAlgOffsetAReading()));	}
	if ( GetAlgSkewEnabled() == true )
	{	AlgParamList.push_back(TAlgParamNode(_T("Skew"), 0, GetAlgSkewUSL(), GetAlgSkewLSL(), GetAlgSkewReading()));	}

	switch ( AlgType )
	{	
	case ALG_BRIGHT_RATIO:
	{
		const TALG_PARAM_BRIGHT_RATIO &brParam=GetAlgParamBrightRatio();
		if ( brParam.brToleranceEnabled == true )
		{
			if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID )
			{	strKey = _T("Height");	}
			else
			{	strKey = _T("Gray");	}
			STD = brParam.brTargetValue;			
			LSL = brParam.brToleranceLSL+STD;
			USL = brParam.brToleranceUSL+STD;
			Reading = brParam.brToleranceReading+STD;
			AlgParamList.push_back(TAlgParamNode(strKey, STD, USL, LSL, Reading));
		}
		if ( brParam.brLimitMinEnabled == true )
		{
			if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID )
			{	strKey = _T("Min-Height");	}
			else
			{	strKey = _T("Min-Gray");	}
			STD = brParam.brTargetValue;
			LSL = brParam.brLimitTolLSL+STD;
			USL = brParam.brLimitTolUSL+STD;
			Reading = brParam.brLimitReadingMin+STD;
			AlgParamList.push_back(TAlgParamNode(strKey, STD, USL, LSL, Reading));
		}		
		if ( brParam.brLimitMaxEnabled == true )
		{
			if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID )
			{	strKey = _T("Max-Height");	}
			else
			{	strKey = _T("Max-Gray");	}
			STD = brParam.brTargetValue;
			LSL = brParam.brLimitTolLSL+STD;
			USL = brParam.brLimitTolUSL+STD;
			Reading = brParam.brLimitReadingMax+STD;
			AlgParamList.push_back(TAlgParamNode(strKey, STD, USL, LSL, Reading));
		}		
		if ( brParam.brRatioEnabled == true )
		{	AlgParamList.push_back(TAlgParamNode(_T("Ratio"), 0, brParam.brRatioUSL, brParam.brRatioLSL, brParam.brRatioReading));	}		
		if ( brParam.brRangeEnabled == true )
		{	AlgParamList.push_back(TAlgParamNode(_T("Range"), 0, brParam.brRangeUSL, brParam.brRangeLSL, brParam.brRangeReading));	}		
		if ( brParam.brContrastEnabled == true )
		{	AlgParamList.push_back(TAlgParamNode(_T("Contrast"), 0, brParam.brContrastUSL, brParam.brContrastLSL, brParam.brContrastReading));	}		
		if ( brParam.brXLineEnabled == true )
		{	AlgParamList.push_back(TAlgParamNode(_T("X-Line"), 0, brParam.brXLineUSL, brParam.brXLineLSL, brParam.brXLineReading));	}
		if ( brParam.brYLineEnabled == true )
		{	AlgParamList.push_back(TAlgParamNode(_T("Y-Line"), 0, brParam.brYLineUSL, brParam.brYLineLSL, brParam.brYLineReading));	}
	}
		break;
	case ALG_OUTER_SHORT:
	{
		const TALG_PARAM_OUTER_SHORT &osParam=GetAlgParamOuterShort();	
		if ( osParam.osLine_T.olEnabled == true )
		{
			const TALG_PARAM_OUTER_LINE &OuterLine=osParam.osLine_T;			
			AlgParamList.push_back(TAlgParamNode(_T("Up"), 0, OuterLine.olUSL, OuterLine.olLSL, OuterLine.olReading));
		}		
		if ( osParam.osLine_L.olEnabled == true )
		{	
			const TALG_PARAM_OUTER_LINE &OuterLine=osParam.osLine_L;			
			AlgParamList.push_back(TAlgParamNode(_T("Left"), 0, OuterLine.olUSL, OuterLine.olLSL, OuterLine.olReading));
		}		
		if ( osParam.osLine_B.olEnabled == true )
		{		
			const TALG_PARAM_OUTER_LINE &OuterLine=osParam.osLine_B;			
			AlgParamList.push_back(TAlgParamNode(_T("Down"), 0, OuterLine.olUSL, OuterLine.olLSL, OuterLine.olReading));
		}		
		if ( osParam.osLine_R.olEnabled == true )
		{	
			const TALG_PARAM_OUTER_LINE &OuterLine=osParam.osLine_R;			
			AlgParamList.push_back(TAlgParamNode(_T("Right"), 0, OuterLine.olUSL, OuterLine.olLSL, OuterLine.olReading));
		}
	}
		break;
	case ALG_BLOB_COUNT:
	{
		const TALG_PARAM_BLOB_COUNT  &blobParam=GetAlgParamBlobCount();
		AlgParamList.push_back(TAlgParamNode(_T("Count"), 0, blobParam.bcCountUSL, blobParam.bcCountLSL, blobParam.bcCountNum));
	}
		break;
	case ALG_BODY_TILT:
		break;
	case ALG_BARCODE_RECOGNIZE:
		break;
	case ALG_OBJECT_MEASURE:		
	{
		const TALG_PARAM_OBJECT_MEASURE &omParam=GetAlgParamObjectMeasure();	
		if ( omParam.omSizeXEnabled == true )
		{
			strKey = _T("X-Size");
			STD =omParam.omSizeXSpec;
			if ( ALG_CALC_UNIT_RATIO == omParam.omSizeCalcUnitMode )
			{	AlgParamList.push_back(TAlgParamNode(strKey, 0, omParam.omSizeXRatioUSL, omParam.omSizeXRatioLSL, omParam.omSizeXReading));	}
			else
			{	AlgParamList.push_back(TAlgParamNode(strKey, 0, omParam.omSizeXDiffUSL+STD, omParam.omSizeXDiffLSL+STD, omParam.omSizeXReading));	}
		}		
		if ( omParam.omSizeYEnabled == true )
		{
			strKey = _T("Y-Size");
			STD =omParam.omSizeYSpec;
			if ( ALG_CALC_UNIT_RATIO == omParam.omSizeCalcUnitMode )
			{	AlgParamList.push_back(TAlgParamNode(strKey, 0, omParam.omSizeYRatioUSL, omParam.omSizeYRatioLSL, omParam.omSizeYReading));	}
			else
			{	AlgParamList.push_back(TAlgParamNode(strKey, 0, omParam.omSizeYDiffUSL+STD, omParam.omSizeYDiffLSL+STD, omParam.omSizeYReading));	}
		}		
		if ( omParam.omHeightEnabled == true )
		{
			strKey = _T("Height");
			STD =omParam.omHeightSpec;
			if ( ALG_CALC_UNIT_RATIO == omParam.omHeightCalcUnitMode )
			{	AlgParamList.push_back(TAlgParamNode(strKey, 0, omParam.omHeightRatioUSL, omParam.omHeightRatioLSL, omParam.omHeightReading));	}
			else
			{	AlgParamList.push_back(TAlgParamNode(strKey, 0, omParam.omHeightDiffUSL+STD, omParam.omHeightDiffLSL+STD, omParam.omHeightReading));	}
		}		
		if ( omParam.omAreaEnabled == true )
		{	AlgParamList.push_back(TAlgParamNode(_T("Area"), 0, omParam.omAreaUSL, omParam.omAreaLSL, omParam.omAreaReading));	}		
		if ( omParam.omVolumeEnabled == true )
		{	AlgParamList.push_back(TAlgParamNode(_T("Volume"), 0, omParam.omVolumeUSL, omParam.omVolumeLSL, omParam.omVolumeReading));	}
	}
		break;		
	case ALG_COLOR_CODE:
	{
		const TALG_PARAM_COLOR_CODE  &ccParam=GetAlgParamColorCode();
		AlgParamList.push_back(TAlgParamNode(_T("Ratio"), 0, ccParam.ccPassRatioUSL, ccParam.ccPassRatioLSL, ccParam.ccPassRatioReading));
	}
		break;
	case ALG_MODEL_MATCH:
		AlgParamList.push_back(TAlgParamNode(_T("Score"), 0, GetAlgPatternSimilarityUSL(), GetAlgPatternSimilarityLSL(), GetAlgPatternSimilarityReading()));
		break;
	case ALG_IMAGE_MATCH:
		AlgParamList.push_back(TAlgParamNode(_T("Score"), 0, GetAlgPatternSimilarityUSL(), GetAlgPatternSimilarityLSL(), GetAlgPatternSimilarityReading()));
		break;
	case ALG_CHAR_VERIFY:
	{
		const TALG_PARAM_CHAR_VERIFY &cvParam=GetAlgParamCharVerify();	
		AlgParamList.push_back(TAlgParamNode(_T("Ratio"), 0, cvParam.cvPassRatioUSL, cvParam.cvPassRatioLSL, cvParam.cvPassRatioReading));
	}
		break;
	case ALG_SHAPE_VERIFY:
	{
		const TALG_PARAM_SHAPE_VERIFY &svParam=GetAlgParamShapeVerify();
		LSL = svParam.svCircleR-svParam.svOuterTol;
		USL = svParam.svCircleR+svParam.svOuterTol;
		AlgParamList.push_back(TAlgParamNode(_T("Outer"), 0, USL, LSL, svParam.svReadingROuter));		
		
		LSL = svParam.svCircleR-svParam.svInnerTol;
		USL = svParam.svCircleR+svParam.svInnerTol;
		AlgParamList.push_back(TAlgParamNode(_T("Inner"), 0, USL, LSL, svParam.svReadingRInner));
		
		LSL = svParam.svCircleR-svParam.svErrorTol;
		USL = svParam.svCircleR+svParam.svErrorTol;
		AlgParamList.push_back(TAlgParamNode(_T("Average"), 0, USL, LSL, svParam.svReadingRAverage));

		LSL =-svParam.svRangeTol;
		USL = svParam.svRangeTol;
		Reading = svParam.svReadingROuter-svParam.svReadingRInner;
		AlgParamList.push_back(TAlgParamNode(_T("Range"), 0, USL, LSL, Reading));
	}
		break;
	case ALG_ANGLE_MEASURE:
	{
		const TALG_PARAM_ANGLE_MEASURE &amParam=GetAlgParamAngleMeasure();
		Reading = amParam.amAngleReading-amParam.amAngleSpec;
		AlgParamList.push_back(TAlgParamNode(_T("Range"), 0, amParam.amAngleTolUSL, amParam.amAngleTolLSL, Reading));
	}
		break;
	case ALG_PIXEL_COMPARE:
	{
		const TALG_PARAM_PIXEL_COMPARE &pcParam=GetAlgParamPixelCompare();
		AlgParamList.push_back(TAlgParamNode(_T("Count"), 0, pcParam.pcBlobCountUSL, pcParam.pcBlobCountLSL, pcParam.pcBlobCountNum));
	}
		break;
	case ALG_WIDTH_RATIO:
	{
		const TALG_PARAM_IPC_PRODUCT &ipcParam=GetAlgParamIPC();
		AlgParamList.push_back(TAlgParamNode(_T("Width"), 0, 100.0, ipcParam.ipcWidthRatio, ipcParam.ipcResult));
	}
		break;
	case ALG_HEIGHT:
	{
		const TALG_PARAM_RESIN_HEIGHT &hightDetectParam=GetAlgParamResinHeight();
		if (hightDetectParam.nPartHeightMeasureMode == 0) 
		{	
			strKey = _T("Ratio");
			USL = 100.0;			
		}
		else 
		{
			strKey = _T("Height");
			USL = FIXED_THRESHOLD_MAX_3D;			
		}
		AlgParamList.push_back(TAlgParamNode(strKey, 0, USL, hightDetectParam.nPartHeightThreshold, hightDetectParam.resultH));	
	}
		break;
	case ALG_WIRE_WIDTH:
	{
		const TALG_PARAM_WIRE_WIDTH &wireWidthParam=GetAlgParamWireWidth();	
		AlgParamList.push_back(TAlgParamNode(_T("Width"), 0, wireWidthParam.widthUSL, wireWidthParam.widthLSL, wireWidthParam.fWidth));
	}
		break;
	case ALG_SOLDER_WETTING:
	{
		const TALG_PARAM_SOLDER_WETTING &swParam=GetAlgParamSolderWetting();
		if ( swParam.swCircleAngleEnabled == true )
		{	AlgParamList.push_back(TAlgParamNode(_T("Circle-Angle"), 0, swParam.swCircleAngleUSL, swParam.swCircleAngleLSL, swParam.swReadingCircleAngle));	}
	}
		break;	
	case ALG_FD_MATCH:
		break;
	case ALG_EDGE_SEARCH:
		break;	
	case ALG_MEASURE_BLACK_GLUE:
		break;
	case ALG_MEASURE_FLUX_AREA:
		break;
	case ALG_MEASURE_CPU_PIN:
		break;
	}
	
	const TALG_PARAM_GROUP_COMPARE  &gcParam=GetAlgParamGroupCompare();
	if ( true == gcParam.gc3DHeightEnabled )
	{	AlgParamList.push_back(TAlgParamNode(_T("3DHeight"), 0, gcParam.gc3DHeightUSL, gcParam.gc3DHeightLSL, gcParam.gc3DHeightReading));	}
	if ( true == gcParam.gcTiltAngleEnabled )
	{	AlgParamList.push_back(TAlgParamNode(_T("3DTilt"), 0, gcParam.gcTiltAngleUSL, gcParam.gcTiltAngleLSL, gcParam.gcTiltAngleReading));	}

	std::wstring wstr;
	const size_t AlgParamCount=AlgParamList.size();
	if ( AlgParamCount > 0 )
	{
		for ( size_t i=0; i<AlgParamCount; i++ )
		{
			const TAlgParamNode &rParamNode=AlgParamList[i];
			JetAPI::TCHAR2wstring(rParamNode.sName, wstr);
			if ( i > 0 )
			{	::fwprintf(pfile, L"            ,\n");	}
			::fwprintf(pfile, L"            {\n");
			::fwprintf(pfile, L"            \"%s\": \"%s\",\n", L"Parameter", wstr.c_str());
			::fwprintf(pfile, L"            \"%s\": %.2f,\n", L"Reading", rParamNode.fReading);
			::fwprintf(pfile, L"            \"%s\": %.2f,\n", L"LSL", rParamNode.fLSL);
			::fwprintf(pfile, L"            \"%s\": %.2f\n", L"USL", rParamNode.fUSL);
			::fwprintf(pfile, L"            }");
		}	
		::fwprintf(pfile, L"            \n");
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgSpcFile_GroupCompare_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr)
{
	const TALG_PARAM_GROUP_COMPARE  &gcParam = GetAlgParamGroupCompare();
	if ( false == gcParam.gc3DHeightEnabled ) { return true; }
	
	::fwprintf(pfile, L"            \"%s\": %.2f,\n", L"Group_H", gcParam.gc3DHeightBase);
	::fwprintf(pfile, L"            \"%s\": %.2f,\n", L"Upper_Limits", gcParam.gc3DHeightUSL);
	::fwprintf(pfile, L"            \"%s\": %.2f,\n", L"Lower_Limits", gcParam.gc3DHeightLSL);
	//::fwprintf(pfile, L"            \"%s\": \"%s\",\n", L"Lower_Limits", L"STR");

	int Benchmark_Mode=0;
	switch ( gcParam.gc3DHeightBaseMode )
	{
	case ALG_3D_BASE_HEIGHT_MIN:	Benchmark_Mode = 1;	break;
	case ALG_3D_BASE_HEIGHT_MAX:	Benchmark_Mode = 0;	break;
	case ALG_3D_BASE_HEIGHT_AVE:	Benchmark_Mode = 0;	break;
	case ALG_3D_BASE_HEIGHT_MID:	Benchmark_Mode = 0;	break;
	}
	::fwprintf(pfile, L"            \"%s\": %d\n", L"Benchmark_Mode", Benchmark_Mode);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgSpcFile_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr)
{	
	CString          strText;	
	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";
	const ALG_TYPE AlgType = GetAlgType();

	//JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	//::fwprintf(pfile, L"          \"%s\": \"%s\",\n", strTag, strBuffer);

	::wcscpy(strTag, L"Frame_Index");//"Frame Index":1~8	// 影像編號(整數)
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, GetAlgImageBinParam().GetBinaryFrameIndex()+1);	

	::wcscpy(strTag, L"ALG_ID");//20210906
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, GetAlgType());

	::wcscpy(strTag, L"Group_ID");//20211025
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, GetAlgGroupID());

	::wcscpy(strTag, L"N_Patterns");//20210906
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, GetAlgPatternCount());	
	
	::wcscpy(strTag, L"IsAdditionalImageEnabled");//20230421
	if ( GetAlgSaveDefectImageDone() )
	{	::wcscpy(strBuffer, L"true");	}
	else
	{	::wcscpy(strBuffer, L"false");	}
	::fwprintf(pfile, L"          \"%s\": %s,\n", strTag, strBuffer);

	::wcscpy(strTag, L"ALGParaProperty");//"ALGParaProperty"
	::fwprintf(pfile, L"          \"%s\":\n", strTag);
	::fwprintf(pfile, L"          [\n");
	if ( WriteAlgSpcFile_Parameter_JSON_RSM(pfile, ProjectPtr) == false ) { return false; }
	::fwprintf(pfile, L"          ],\n");	

	::wcscpy(strTag, L"ALG_Properties");//"ALG_Properties"	
	::fwprintf(pfile, L"          \"%s\":\n", strTag);
	::fwprintf(pfile, L"          {\n");
	if ( ALG_BRIGHT_RATIO == AlgType )
	{
		if ( WriteAlgSpcFile_GroupCompare_JSON_RSM(pfile, ProjectPtr) == false ) { return false; }		
	}
	::fwprintf(pfile, L"          },\n");

	if ( ALG_AI_MODEL_NONE == GetAlgParamAiModel().aiModelID )
	{	::fwprintf(pfile, L"          \"%s\": %s,\n", L"IsAIEnabled", L"false");	}
	else
	{	::fwprintf(pfile, L"          \"%s\": %s,\n", L"IsAIEnabled", L"true");	}
	::wcscpy(strTag, L"AIProperties");//"AIProperties"	
	::fwprintf(pfile, L"          \"%s\":\n", strTag);
	::fwprintf(pfile, L"          {\n");
	if ( WriteAlgSpcFile_AIModel_JSON_RSM(pfile, ProjectPtr) == false ) { return false; }
	::fwprintf(pfile, L"          },\n");
	return true;
}

bool CAlgParam::WriteAlgSpcFile_Parameter_JSON_ALG_ParaProperty(FILE *pfile)
{
	CString str, strKey;
	ALG_TYPE AlgType = GetAlgType();
	double STD = 0, USL = 0, LSL = 0, Reading = 0;
	std::vector<TAlgParamNode> AlgParamList;
	const bool bShowXY = CheckAlgShowOffsetByAlgType(AlgType);
	const unsigned int FrameUniqueID = GetAlgImageBinParam().GetBinaryFrameUniqueID();

	if (GetAlgOffsetXEnabled() == true && true == bShowXY)
	{
		AlgParamList.push_back(TAlgParamNode(_T("OffsetX"), 0, GetAlgOffsetXUSL(), GetAlgOffsetXLSL(), GetAlgOffsetXReading()));
	}
	if (GetAlgOffsetYEnabled() == true && true == bShowXY)
	{
		AlgParamList.push_back(TAlgParamNode(_T("OffsetY"), 0, GetAlgOffsetYUSL(), GetAlgOffsetYLSL(), GetAlgOffsetYReading()));
	}
	if (GetAlgOffsetAEnabled() == true)
	{
		AlgParamList.push_back(TAlgParamNode(_T("OffsetA"), 0, GetAlgOffsetAUSL(), GetAlgOffsetALSL(), GetAlgOffsetAReading()));
	}
	if (GetAlgSkewEnabled() == true)
	{
		AlgParamList.push_back(TAlgParamNode(_T("Skew"), 0, GetAlgSkewUSL(), GetAlgSkewLSL(), GetAlgSkewReading()));
	}

	switch (AlgType)
	{
	case ALG_BRIGHT_RATIO:
	{
		const TALG_PARAM_BRIGHT_RATIO &brParam = GetAlgParamBrightRatio();
		if (brParam.brToleranceEnabled == true)
		{
			if (FRAME_UNIQUE_ID_DLP == FrameUniqueID)
			{
				strKey = _T("Height");
			}
			else
			{
				strKey = _T("Gray");
			}
			STD = brParam.brTargetValue;
			LSL = brParam.brToleranceLSL + STD;
			USL = brParam.brToleranceUSL + STD;
			Reading = brParam.brToleranceReading + STD;
			AlgParamList.push_back(TAlgParamNode(strKey, STD, USL, LSL, Reading));
		}
		if (brParam.brLimitMinEnabled == true)
		{
			if (FRAME_UNIQUE_ID_DLP == FrameUniqueID)
			{
				strKey = _T("Min-Height");
			}
			else
			{
				strKey = _T("Min-Gray");
			}
			STD = brParam.brTargetValue;
			LSL = brParam.brLimitTolLSL + STD;
			USL = brParam.brLimitTolUSL + STD;
			Reading = brParam.brLimitReadingMin + STD;
			AlgParamList.push_back(TAlgParamNode(strKey, STD, USL, LSL, Reading));
		}
		if (brParam.brLimitMaxEnabled == true)
		{
			if (FRAME_UNIQUE_ID_DLP == FrameUniqueID)
			{
				strKey = _T("Max-Height");
			}
			else
			{
				strKey = _T("Max-Gray");
			}
			STD = brParam.brTargetValue;
			LSL = brParam.brLimitTolLSL + STD;
			USL = brParam.brLimitTolUSL + STD;
			Reading = brParam.brLimitReadingMax + STD;
			AlgParamList.push_back(TAlgParamNode(strKey, STD, USL, LSL, Reading));
		}
		if (brParam.brRatioEnabled == true)
		{
			AlgParamList.push_back(TAlgParamNode(_T("Ratio"), 0, brParam.brRatioUSL, brParam.brRatioLSL, brParam.brRatioReading));
		}
		if (brParam.brRangeEnabled == true)
		{
			AlgParamList.push_back(TAlgParamNode(_T("Range"), 0, brParam.brRangeUSL, brParam.brRangeLSL, brParam.brRangeReading));
		}
		if (brParam.brContrastEnabled == true)
		{
			AlgParamList.push_back(TAlgParamNode(_T("Contrast"), 0, brParam.brContrastUSL, brParam.brContrastLSL, brParam.brContrastReading));
		}
		if (brParam.brXLineEnabled == true)
		{
			AlgParamList.push_back(TAlgParamNode(_T("X-Line"), 0, brParam.brXLineUSL, brParam.brXLineLSL, brParam.brXLineReading));
		}
		if (brParam.brYLineEnabled == true)
		{
			AlgParamList.push_back(TAlgParamNode(_T("Y-Line"), 0, brParam.brYLineUSL, brParam.brYLineLSL, brParam.brYLineReading));
		}
	}
	break;
	case ALG_OUTER_SHORT:
	{
		const TALG_PARAM_OUTER_SHORT &osParam = GetAlgParamOuterShort();
		if (osParam.osLine_T.olEnabled == true)
		{
			const TALG_PARAM_OUTER_LINE &OuterLine = osParam.osLine_T;
			AlgParamList.push_back(TAlgParamNode(_T("Up"), 0, OuterLine.olUSL, OuterLine.olLSL, OuterLine.olReading));
		}
		if (osParam.osLine_L.olEnabled == true)
		{
			const TALG_PARAM_OUTER_LINE &OuterLine = osParam.osLine_L;
			AlgParamList.push_back(TAlgParamNode(_T("Left"), 0, OuterLine.olUSL, OuterLine.olLSL, OuterLine.olReading));
		}
		if (osParam.osLine_B.olEnabled == true)
		{
			const TALG_PARAM_OUTER_LINE &OuterLine = osParam.osLine_B;
			AlgParamList.push_back(TAlgParamNode(_T("Down"), 0, OuterLine.olUSL, OuterLine.olLSL, OuterLine.olReading));
		}
		if (osParam.osLine_R.olEnabled == true)
		{
			const TALG_PARAM_OUTER_LINE &OuterLine = osParam.osLine_R;
			AlgParamList.push_back(TAlgParamNode(_T("Right"), 0, OuterLine.olUSL, OuterLine.olLSL, OuterLine.olReading));
		}
	}
	break;
	case ALG_BLOB_COUNT:
	{
		const TALG_PARAM_BLOB_COUNT  &blobParam = GetAlgParamBlobCount();
		AlgParamList.push_back(TAlgParamNode(_T("Count"), 0, blobParam.bcCountUSL, blobParam.bcCountLSL, blobParam.bcCountNum));
	}
	break;
	case ALG_BODY_TILT:
		break;
	case ALG_BARCODE_RECOGNIZE:
		break;
	case ALG_OBJECT_MEASURE:
	{
		const TALG_PARAM_OBJECT_MEASURE &omParam = GetAlgParamObjectMeasure();
		if (omParam.omSizeXEnabled == true)
		{
			strKey = _T("X-Size");
			STD = omParam.omSizeXSpec;
			if (ALG_CALC_UNIT_RATIO == omParam.omSizeCalcUnitMode)
			{
				AlgParamList.push_back(TAlgParamNode(strKey, 0, omParam.omSizeXRatioUSL, omParam.omSizeXRatioLSL, omParam.omSizeXReading));
			}
			else
			{
				AlgParamList.push_back(TAlgParamNode(strKey, 0, omParam.omSizeXDiffUSL + STD, omParam.omSizeXDiffLSL + STD, omParam.omSizeXReading));
			}
		}
		if (omParam.omSizeYEnabled == true)
		{
			strKey = _T("Y-Size");
			STD = omParam.omSizeYSpec;
			if (ALG_CALC_UNIT_RATIO == omParam.omSizeCalcUnitMode)
			{
				AlgParamList.push_back(TAlgParamNode(strKey, 0, omParam.omSizeYRatioUSL, omParam.omSizeYRatioLSL, omParam.omSizeYReading));
			}
			else
			{
				AlgParamList.push_back(TAlgParamNode(strKey, 0, omParam.omSizeYDiffUSL + STD, omParam.omSizeYDiffLSL + STD, omParam.omSizeYReading));
			}
		}
		if (omParam.omHeightEnabled == true)
		{
			strKey = _T("Height");
			STD = omParam.omHeightSpec;
			if (ALG_CALC_UNIT_RATIO == omParam.omHeightCalcUnitMode)
			{
				AlgParamList.push_back(TAlgParamNode(strKey, 0, omParam.omHeightRatioUSL, omParam.omHeightRatioLSL, omParam.omHeightReading));
			}
			else
			{
				AlgParamList.push_back(TAlgParamNode(strKey, 0, omParam.omHeightDiffUSL + STD, omParam.omHeightDiffLSL + STD, omParam.omHeightReading));
			}
		}
		if (omParam.omAreaEnabled == true)
		{
			AlgParamList.push_back(TAlgParamNode(_T("Area"), 0, omParam.omAreaUSL, omParam.omAreaLSL, omParam.omAreaReading));
		}
		if (omParam.omVolumeEnabled == true)
		{
			AlgParamList.push_back(TAlgParamNode(_T("Volume"), 0, omParam.omVolumeUSL, omParam.omVolumeLSL, omParam.omVolumeReading));
		}
	}
	break;
	case ALG_COLOR_CODE:
	{
		const TALG_PARAM_COLOR_CODE  &ccParam = GetAlgParamColorCode();
		AlgParamList.push_back(TAlgParamNode(_T("Ratio"), 0, ccParam.ccPassRatioUSL, ccParam.ccPassRatioLSL, ccParam.ccPassRatioReading));
	}
	break;
	case ALG_MODEL_MATCH:
		AlgParamList.push_back(TAlgParamNode(_T("Score"), 0, GetAlgPatternSimilarityUSL(), GetAlgPatternSimilarityLSL(), GetAlgPatternSimilarityReading()));
		break;
	case ALG_IMAGE_MATCH:
		AlgParamList.push_back(TAlgParamNode(_T("Score"), 0, GetAlgPatternSimilarityUSL(), GetAlgPatternSimilarityLSL(), GetAlgPatternSimilarityReading()));
		break;
	case ALG_CHAR_VERIFY:
	{
		const TALG_PARAM_CHAR_VERIFY &cvParam = GetAlgParamCharVerify();
		AlgParamList.push_back(TAlgParamNode(_T("Ratio"), 0, cvParam.cvPassRatioUSL, cvParam.cvPassRatioLSL, cvParam.cvPassRatioReading));
	}
	break;
	case ALG_SHAPE_VERIFY:
	{
		const TALG_PARAM_SHAPE_VERIFY &svParam = GetAlgParamShapeVerify();
		LSL = svParam.svCircleR - svParam.svOuterTol;
		USL = svParam.svCircleR + svParam.svOuterTol;
		AlgParamList.push_back(TAlgParamNode(_T("Outer"), 0, USL, LSL, svParam.svReadingROuter));

		LSL = svParam.svCircleR - svParam.svInnerTol;
		USL = svParam.svCircleR + svParam.svInnerTol;
		AlgParamList.push_back(TAlgParamNode(_T("Inner"), 0, USL, LSL, svParam.svReadingRInner));

		LSL = svParam.svCircleR - svParam.svErrorTol;
		USL = svParam.svCircleR + svParam.svErrorTol;
		AlgParamList.push_back(TAlgParamNode(_T("Average"), 0, USL, LSL, svParam.svReadingRAverage));

		LSL = -svParam.svRangeTol;
		USL = svParam.svRangeTol;
		Reading = svParam.svReadingROuter - svParam.svReadingRInner;
		AlgParamList.push_back(TAlgParamNode(_T("Range"), 0, USL, LSL, Reading));
	}
	break;
	case ALG_ANGLE_MEASURE:
	{
		const TALG_PARAM_ANGLE_MEASURE &amParam = GetAlgParamAngleMeasure();
		Reading = amParam.amAngleReading - amParam.amAngleSpec;
		AlgParamList.push_back(TAlgParamNode(_T("Range"), 0, amParam.amAngleTolUSL, amParam.amAngleTolLSL, Reading));
	}
	break;
	case ALG_PIXEL_COMPARE:
	{
		const TALG_PARAM_PIXEL_COMPARE &pcParam = GetAlgParamPixelCompare();
		AlgParamList.push_back(TAlgParamNode(_T("Count"), 0, pcParam.pcBlobCountUSL, pcParam.pcBlobCountLSL, pcParam.pcBlobCountNum));
	}
	break;
	case ALG_WIDTH_RATIO:
	{
		const TALG_PARAM_IPC_PRODUCT &ipcParam = GetAlgParamIPC();
		AlgParamList.push_back(TAlgParamNode(_T("Width"), 0, 100.0, ipcParam.ipcWidthRatio, ipcParam.ipcResult));
	}
	break;
	case ALG_HEIGHT:
	{
		const TALG_PARAM_RESIN_HEIGHT &hightDetectParam = GetAlgParamResinHeight();
		if (hightDetectParam.nPartHeightMeasureMode == 0)
		{
			strKey = _T("Ratio");
			USL = 100.0;
		}
		else
		{
			strKey = _T("Height");
			USL = FIXED_THRESHOLD_MAX_3D;
		}
		AlgParamList.push_back(TAlgParamNode(strKey, 0, USL, hightDetectParam.nPartHeightThreshold, hightDetectParam.resultH));
	}
	break;
	case ALG_WIRE_WIDTH:
	{
		const TALG_PARAM_WIRE_WIDTH &wireWidthParam = GetAlgParamWireWidth();
		AlgParamList.push_back(TAlgParamNode(_T("Width"), 0, wireWidthParam.widthUSL, wireWidthParam.widthLSL, wireWidthParam.fWidth));
	}
	break;
	case ALG_SOLDER_WETTING:
	{
		const TALG_PARAM_SOLDER_WETTING &swParam = GetAlgParamSolderWetting();
		if (swParam.swCircleAngleEnabled == true)
		{
			AlgParamList.push_back(TAlgParamNode(_T("Circle-Angle"), 0, swParam.swCircleAngleUSL, swParam.swCircleAngleLSL, swParam.swReadingCircleAngle));
		}
	}
	break;
	case ALG_FD_MATCH:
		break;
	case ALG_EDGE_SEARCH:
		break;
	}

	const TALG_PARAM_GROUP_COMPARE  &gcParam = GetAlgParamGroupCompare();
	if (true == gcParam.gc3DHeightEnabled)
	{
		AlgParamList.push_back(TAlgParamNode(_T("3DHeight"), 0, gcParam.gc3DHeightUSL, gcParam.gc3DHeightLSL, gcParam.gc3DHeightReading));
	}
	if (true == gcParam.gcTiltAngleEnabled)
	{
		AlgParamList.push_back(TAlgParamNode(_T("3DTilt"), 0, gcParam.gcTiltAngleUSL, gcParam.gcTiltAngleLSL, gcParam.gcTiltAngleReading));
	}

	std::wstring wstr;
	const size_t AlgParamCount = AlgParamList.size();
	/*::wcscpy(strTag, L"ALG_ID");*/

	::fwprintf(pfile, L"          \"%s\":%d ,\n", L"ALG_ID", GetAlgType());
	::fwprintf(pfile, L"          \"%s\":[\n", L"ALGParaProperty");
	if (AlgParamCount > 0)
	{
		for (size_t i = 0; i<AlgParamCount; i++)
		{
			const TAlgParamNode &rParamNode = AlgParamList[i];
			JetAPI::TCHAR2wstring(rParamNode.sName, wstr);

			::fwprintf(pfile, L"            {\n");
			::fwprintf(pfile, L"              \"%s\": \"%s\",\n", L"Parameter", wstr.c_str());
			//::fwprintf(pfile, L"              \"%s\": {\n", L"ParaProperty");
			::fwprintf(pfile, L"              \"%s\": %.2f,\n", L"LSL", rParamNode.fLSL);
			::fwprintf(pfile, L"              \"%s\": %.2f\n", L"USL", rParamNode.fUSL);
			//::fwprintf(pfile, L"              }\n");
			::fwprintf(pfile, L"            }");
			if (i == AlgParamCount - 1) {
				::fwprintf(pfile, L"\n");
			}
			else {
				::fwprintf(pfile, L",\n");
			}
		}
	}
	::fwprintf(pfile, L"          ],\n");
	return true;
}

//-------------------------------------------------------------------------------------//
bool CAlgParam::BuildAlgParamStringList(LPCTSTR Title, ALG_TYPE AlgType, std::vector<CString> &strList) const//建立演算法參數字串
{	
	CString str;
	bool    bOutGroupCompare=false;
	CString strAlgName=AOIDataDefine.GetAlgTypeText(AlgType);
	//USL, LSL	
	if ( GetAlgOffsetXEnabled() == true )
	{
		str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#OffsetX"), GetAlgOffsetXUSL(), GetAlgOffsetXLSL());
		strList.push_back(str);
	}
	if ( GetAlgOffsetYEnabled() == true )
	{
		str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#OffsetY"), GetAlgOffsetYUSL(), GetAlgOffsetYLSL());
		strList.push_back(str);
	}
	if ( GetAlgSkewEnabled() == true )
	{
		str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#Skew"), GetAlgSkewUSL(), GetAlgSkewLSL());
		strList.push_back(str);
	}
	if ( ALG_BRIGHT_RATIO == AlgType )
	{
		bOutGroupCompare = true;
		const TALG_PARAM_BRIGHT_RATIO &brParam=GetAlgParamBrightRatio();
		if ( true == brParam.brToleranceEnabled )
		{
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#Tolerance"), brParam.brToleranceUSL, brParam.brToleranceLSL);
			strList.push_back(str);
		}
		if ( true==brParam.brLimitMinEnabled || true==brParam.brLimitMaxEnabled)
		{
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#Limit"), brParam.brLimitTolUSL, brParam.brLimitTolLSL);
			strList.push_back(str);
		}
		if ( true == brParam.brRatioEnabled )
		{
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#Ratio"), brParam.brRatioUSL, brParam.brRatioLSL);
			strList.push_back(str);
		}
		if ( true == brParam.brRangeEnabled )
		{
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#Range"), brParam.brRangeUSL, brParam.brRangeLSL);
			strList.push_back(str);
		}
		if ( true == brParam.brContrastEnabled )
		{
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#Contrast"), brParam.brContrastUSL, brParam.brContrastLSL);
			strList.push_back(str);
		}
		if ( true == brParam.brXLineEnabled )
		{
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#X-Line"), brParam.brXLineUSL, brParam.brXLineLSL);
			strList.push_back(str);
		}
		if ( true == brParam.brYLineEnabled )
		{
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#Y-Line"), brParam.brYLineUSL, brParam.brYLineLSL);
			strList.push_back(str);
		}
	}

	if ( ALG_OUTER_SHORT == AlgType )
	{
		const TALG_PARAM_OUTER_SHORT &osParam=GetAlgParamOuterShort();
		if ( true == osParam.osLine_R.olEnabled )
		{
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#R-Line"), osParam.osLine_R.olUSL, osParam.osLine_R.olLSL);
			strList.push_back(str);
		}
		if ( true == osParam.osLine_T.olEnabled )
		{
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#T-Line"), osParam.osLine_T.olUSL, osParam.osLine_T.olLSL);
			strList.push_back(str);
		}
		if ( true == osParam.osLine_L.olEnabled )
		{
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#L-Line"), osParam.osLine_L.olUSL, osParam.osLine_L.olLSL);
			strList.push_back(str);
		}
		if ( true == osParam.osLine_B.olEnabled )
		{
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#B-Line"), osParam.osLine_B.olUSL, osParam.osLine_B.olLSL);
			strList.push_back(str);
		}
	}

	if ( ALG_BLOB_COUNT == AlgType )
	{
		const TALG_PARAM_BLOB_COUNT &bcParam=GetAlgParamBlobCount();
		str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T(""), bcParam.bcCountUSL, bcParam.bcCountLSL);
		strList.push_back(str);		
	}

	if ( ALG_BODY_TILT == AlgType )
	{
		const TALG_PARAM_BODY_TILT &btParam=GetAlgParamBodyTilt();
		str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#Gap"), btParam.btTiltGapUSL, btParam.btTiltGapLSL);
		strList.push_back(str);		
		str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#Angle"), btParam.btTiltAngleUSL, btParam.btTiltAngleLSL);
		strList.push_back(str);
	}

	if ( ALG_BARCODE_RECOGNIZE == AlgType )
	{
		const TALG_PARAM_BARCODE_RECOGNIZE &brParam=GetAlgParamBarcodeRecognize();
		str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T(""), brParam.brVerifyUSL, brParam.brVerifyLSL);
		strList.push_back(str);
	}

	if ( ALG_OBJECT_MEASURE == AlgType )
	{
		const TALG_PARAM_OBJECT_MEASURE &omParam=GetAlgParamObjectMeasure();
		if ( true == omParam.omSizeXEnabled )
		{
			if ( ALG_CALC_UNIT_DIFF == omParam.omSizeCalcUnitMode )
			{
				str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#X-Size"), omParam.omSizeXDiffUSL, omParam.omSizeXDiffLSL);
				strList.push_back(str);
			}
			if ( ALG_CALC_UNIT_RATIO == omParam.omSizeCalcUnitMode )
			{
				str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#X-Size"), omParam.omSizeXRatioUSL, omParam.omSizeXRatioLSL);
				strList.push_back(str);
			}			
		}
		if ( true == omParam.omSizeYEnabled )
		{
			if ( ALG_CALC_UNIT_DIFF == omParam.omSizeCalcUnitMode )
			{
				str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#Y-Size"), omParam.omSizeYDiffUSL, omParam.omSizeYDiffLSL);
				strList.push_back(str);
			}
			if ( ALG_CALC_UNIT_RATIO == omParam.omSizeCalcUnitMode )
			{
				str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#Y-Size"), omParam.omSizeYRatioUSL, omParam.omSizeYRatioLSL);
				strList.push_back(str);
			}			
		}

		if ( true == omParam.omHeightEnabled )
		{
			if ( ALG_CALC_UNIT_DIFF == omParam.omHeightCalcUnitMode )
			{
				str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#Height"), omParam.omHeightDiffUSL, omParam.omHeightDiffLSL);
				strList.push_back(str);
			}
			if ( ALG_CALC_UNIT_RATIO == omParam.omHeightCalcUnitMode )
			{
				str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#Height"), omParam.omHeightRatioUSL, omParam.omHeightRatioLSL);
				strList.push_back(str);
			}			
		}

		if ( true == omParam.omAreaEnabled )
		{			
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#Area"), omParam.omAreaUSL, omParam.omAreaLSL);
			strList.push_back(str);			
		}

		if ( true == omParam.omVolumeEnabled )
		{			
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#Volume"), omParam.omVolumeUSL, omParam.omVolumeLSL);
			strList.push_back(str);			
		}		
	}

	if ( ALG_COLOR_CODE == AlgType )
	{
		const TALG_PARAM_COLOR_CODE &ccParam=GetAlgParamColorCode();
		str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T(""), ccParam.ccPassRatioUSL, ccParam.ccPassRatioLSL);
		strList.push_back(str);
	}

	if ( ALG_MODEL_MATCH == AlgType )
	{
		const TALG_PARAM_MODEL_MATCH &mmParam=GetAlgParamModelMatch();
	}

	if ( ALG_IMAGE_MATCH == AlgType )
	{
		const TALG_PARAM_IMAGE_MATCH &imParam=GetAlgParamImageMatch();
		if ( true == imParam.imPxlCmpEnabled )
		{			
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T(""), imParam.imPxlCmpCountUSL, imParam.imPxlCmpCountLSL);
			strList.push_back(str);			
		}	
	}	

	if ( ALG_CHAR_VERIFY == AlgType )
	{
		const TALG_PARAM_CHAR_VERIFY &cvParam=GetAlgParamCharVerify();
		str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T(""), cvParam.cvPassRatioUSL, cvParam.cvPassRatioLSL);
		strList.push_back(str);				
	}

	if ( ALG_FD_MATCH == AlgType )
	{
		const TALG_PARAM_FD_MATCH &fdParam=GetAlgParamFdMatch();
	}

	if ( ALG_EDGE_SEARCH == AlgType )
	{
		const TALG_PARAM_EDGE_SEARCH &esParam=GetAlgParamEdgeSearch();						
	}
	
	if ( ALG_SHAPE_VERIFY == AlgType )
	{
		const TALG_PARAM_SHAPE_VERIFY &svParam=GetAlgParamShapeVerify();				
		str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("Outer"), svParam.svOuterTol, -svParam.svOuterTol);
		strList.push_back(str);	
		str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("Inner"), svParam.svInnerTol, -svParam.svInnerTol);
		strList.push_back(str);	
		str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("Range"), svParam.svRangeTol, -svParam.svRangeTol);
		strList.push_back(str);	
		str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("Average"), svParam.svErrorTol, -svParam.svErrorTol);
		strList.push_back(str);			
	}

	if ( ALG_ANGLE_MEASURE == AlgType )	
	{
		const TALG_PARAM_ANGLE_MEASURE &amParam=GetAlgParamAngleMeasure();
		str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("Angle"), amParam.amAngleSpec+amParam.amAngleTolUSL, amParam.amAngleSpec+amParam.amAngleTolLSL);
		strList.push_back(str);	
	}

	if ( ALG_PIXEL_COMPARE == AlgType )	
	{
		const TALG_PARAM_PIXEL_COMPARE &pcParam=GetAlgParamPixelCompare();
		str.Format(_T("%s, %s%s, %d, %d"), Title, strAlgName, _T("Count"), pcParam.pcBlobCountUSL, pcParam.pcBlobCountLSL);
		strList.push_back(str);	
	}

	if ( ALG_SOLDER_WETTING == AlgType )
	{		
		const TALG_PARAM_SOLDER_WETTING &swParam=GetAlgParamSolderWetting();
		if ( true == swParam.swCircleAngleEnabled )
		{
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("CircleAngle"), swParam.swCircleAngleUSL, swParam.swCircleAngleLSL);
			strList.push_back(str);	
		}
	}

	if ( ALG_MEASURE_BLACK_GLUE == AlgType )
	{
	}	

	if ( ALG_MEASURE_FLUX_AREA == AlgType )
	{
	}

	if ( ALG_MEASURE_CPU_PIN == AlgType )
	{
	}

	if ( true == bOutGroupCompare )
	{
		strAlgName=_T("Group Compare");
		const TALG_PARAM_GROUP_COMPARE &gcParam=GetAlgParamGroupCompare();		
		if ( true == gcParam.gc2DGrayEnabled )
		{			
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#2D-Gray"), gcParam.gc2DGrayUSL, gcParam.gc2DGrayLSL);
			strList.push_back(str);			
		}
		if ( true == gcParam.gc3DHeightEnabled )
		{			
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#3D-Height"), gcParam.gc3DHeightUSL, gcParam.gc3DHeightLSL);
			strList.push_back(str);			
		}
		if ( true == gcParam.gcTiltAngleEnabled )
		{			
			str.Format(_T("%s, %s%s, %.2f, %.2f"), Title, strAlgName, _T("#3D-Tilt"), gcParam.gcTiltAngleUSL, gcParam.gcTiltAngleLSL);
			strList.push_back(str);			
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAlgParam::GetAlgDebugFolder() const
{
	return CString(AOIDataCollect.GetAOITempDirectory());
}
//-------------------------------------------------------------------------------------//
//bool CAlgParam::CheckAlgActiveBinParamShared(const CAlgBinaryParam &BinParam)
//{
//	BIN_PARAM_BELONG_TO BelongToWho = BinParam.GetBinaryBelongToWho();
//	if (BIN_PARAM_BELONG_TO_ROI_IMAGE != BelongToWho) { return false; }
//	ALG_TYPE AlgType = GetAlgType();
//	if (ALG_MEASURE_SIP_DISTANCE != AlgType) { return false; }
//	const size_t WndRoiIndex = BinParam.GetBinaryBelongIndex();
//	if (WndRoiIndex == 0) { return false; }
//	return true;
//}
////-------------------------------------------------------------------------------------//
//bool CAlgParam::SetAlgActiveBinParamShared(const CAlgBinaryParam & BinParam)
//{
//	bool            IsOK = true;
//	size_t          Index=0;
//	CAOIWnd        *WndPtr = GetAlgWndPtr();
//	CAOIWndRoi     *WndRoiPtr = NULL;
//	CPatternParam  *PatParamPtr=NULL; 
//	BIN_PARAM_BELONG_TO BelongToWho = BinParam.GetBinaryBelongToWho();
//	if (NULL == WndPtr) { return false; }
//	if (m_AlgType == ALG_MEASURE_SIP_DISTANCE) {
//		Index = BinParam.GetBinaryBelongIndex();
//		const TALG_PARAM_MEASURE_SIP msParam = GetAlgParamMeasureSIP();
//		const size_t MarkCount = 1;
//		const size_t InspectionEdgeCount = msParam.msInspecEdgeCount;
//		const size_t RefEdgeCount = msParam.msInspecEdgeCount;
//		size_t i, StartIdx, EndIdx;
//		CAOIWndRoi *WndRoiPtr = NULL;
//		if (Index < InspectionEdgeCount + MarkCount) {
//			StartIdx = MarkCount;
//			EndIdx = InspectionEdgeCount + MarkCount;
//		}
//		else {
//			StartIdx = InspectionEdgeCount + MarkCount;
//			EndIdx = InspectionEdgeCount + MarkCount + RefEdgeCount;
//		}
//		for (i = StartIdx; i < EndIdx; i++) {
//			if (i == Index) { continue; }
//			WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, true);
//			if (WndRoiPtr == NULL) { continue; }
//			WndRoiPtr->SetWndRoiBinaryParam(BinParam);
//		}
//	}
//	else { IsOK = false; }
//	return IsOK;
//}
//-------------------------------------------------------------------------------------//
CAlgBinaryParam* CAlgParam::GetAlgActiveBinParamPtr()
{
	size_t            Index=0;	
	CAOIWnd          *WndPtr = GetAlgWndPtr();
	CAOIWndRoi       *WndRoiPtr = NULL;
	CAlgBinaryParam  *BinParamPtr = NULL;
	CPatternParam    *PatParamPtr = NULL; 
	switch ( m_AlgBinParamActived )
	{
	case BIN_PARAM_BELONG_TO_MASK_IMAGE:
		BinParamPtr = &(m_AlgMaskBinParam);
		break;
	case BIN_PARAM_BELONG_TO_PATTERN_IMAGE:
		//PatIdx = BinParam.PatternIndex;
		//PatParamPtr = GetAlgPatternParamPtr(PatIdx, true);
		//if ( NULL != PatParamPtr )		
		//{	BinParamPtr = &(PatParamPtr->BinaryParam); }
		break;
	case BIN_PARAM_BELONG_TO_ROI_IMAGE:
		if ( NULL != WndPtr )
		{
			WndRoiPtr = WndPtr->GetWndRoiWndActived();
			if ( NULL != WndRoiPtr )
			{	BinParamPtr = WndRoiPtr->GetWndRoiBinaryParamPtr();	}
		}
		break;
	case BIN_PARAM_BELONG_TO_ALG_IMAGE:
	default:
		BinParamPtr = &(m_AlgImageBinParam);
		break;
	}	

	if ( NULL == BinParamPtr )
	{
		BinParamPtr = &(m_AlgImageBinParam);
		m_AlgBinParamActived = BIN_PARAM_BELONG_TO_ALG_IMAGE;
	}
	return BinParamPtr;
}
//-------------------------------------------------------------------------------------//
bool  CAlgParam::SetAlgActiveBinParam(const CAlgBinaryParam &BinParam)
{
	bool            IsOK = true;
	size_t          Index = 0, i;
	CAOIWnd        *WndPtr = GetAlgWndPtr();
	CAOIWndRoi     *WndRoiPtr = NULL;
	std::vector<CAOIWndRoi*> WndRoiPtrList;
	CPatternParam  *PatParamPtr=NULL; 
	BIN_PARAM_BELONG_TO BelongToWho = BinParam.GetBinaryBelongToWho();
	switch ( BelongToWho )
	{
	case BIN_PARAM_BELONG_TO_ALG_IMAGE:
		m_AlgImageBinParam = BinParam;
		break;
	case BIN_PARAM_BELONG_TO_MASK_IMAGE:
		m_AlgMaskBinParam = BinParam;
		break;
	case BIN_PARAM_BELONG_TO_PATTERN_IMAGE:
		Index = BinParam.GetBinaryBelongIndex();
		PatParamPtr = GetAlgPatternParamPtr(Index, true);
		if ( NULL == PatParamPtr )
		{	IsOK = false; }
		else
		{	PatParamPtr->SetBinaryParam(BinParam); }
		break;
	case BIN_PARAM_BELONG_TO_ROI_IMAGE:
		if ( NULL != WndPtr )
		{
			Index = BinParam.GetBinaryBelongIndex();
			WndPtr->GetWndRoiWndSelectedList(WndRoiPtrList);
			IsOK = false;
			for (i = 0; i < WndRoiPtrList.size(); i++) {
				WndRoiPtr = WndRoiPtrList[i];
				if (NULL == WndRoiPtr) { continue; }
				if (Index == WndRoiPtr->GetWndRoiIndex()) { IsOK = true; }
				WndRoiPtr->SetWndRoiBinaryParam(BinParam);
			}
			//Index = BinParam.GetBinaryBelongIndex();
			//WndRoiPtr = WndPtr->GetWndRoiWndPtr(Index, true);
			//if ( NULL == WndRoiPtr )
			//{	IsOK = false; }
			//else
			//{	WndRoiPtr->SetWndRoiBinaryParam(BinParam);	}
		}
		break;
	default:
		IsOK = false;
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckAlgMaskBinFrameUsed() const
{
	ALG_TYPE AlgType=GetAlgType();
	if ( ALG_BLOB_COUNT == AlgType )
	{	return true; }
	if ( ALG_OBJECT_MEASURE == AlgType )
	{	return true; }	
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckAlgPatternFileUsed()
{
	m_AlgPatternFileUsed = CAlgParam::CheckAlgPatternFileUsed(m_AlgType);	
	return m_AlgPatternFileUsed;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckAlgPatternMatrchInterpolate() const//確認是否樣板需使用內插補正
{
	if ( true==m_AlgOffsetXEnabled || 
		 true==m_AlgOffsetYEnabled || 
		 true==m_AlgOffsetLEnabled || 
		 true==m_AlgSkewEnabled )
	{	return true;	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::DeleteAlgPattern(unsigned int Index)//刪除演算法圖片
{
	CString Filename, FullFilename;
	CString PatternFolder = CAlgParam::GetAlgPatternFolder();
	const unsigned int PatternCount = (unsigned int)(CAlgParam::GetAlgPatternCount());
	if ( JetAPI::IsFolderExist(PatternFolder) == false ) 
	{	return true; }
	if ( Index>=PatternCount ) { return true; }

	Filename = AOIDataDefine.GetAlgPatternName(Index, BOX_TOWARD_UP);
	FullFilename.Format(_T("%s\\%s"), PatternFolder, Filename);
	if ( ::DeleteFile(FullFilename) == FALSE )
	{	return false;	}

	Filename = AOIDataDefine.GetAlgPatternName(Index, BOX_TOWARD_LEFT);
	FullFilename.Format(_T("%s\\%s"), PatternFolder, Filename);	
	if ( ::DeleteFile(FullFilename) == FALSE )
	{	return false;	}

	Filename = AOIDataDefine.GetAlgPatternName(Index, BOX_TOWARD_DOWN);
	FullFilename.Format(_T("%s\\%s"), PatternFolder, Filename);	
	if ( ::DeleteFile(FullFilename) == FALSE )
	{	return false;	}

	Filename = AOIDataDefine.GetAlgPatternName(Index, BOX_TOWARD_RIGHT);
	FullFilename.Format(_T("%s\\%s"), PatternFolder, Filename);	
	if ( ::DeleteFile(FullFilename) == FALSE )
	{	return false;	}

	BOX_TOWARD BoxToward;
	unsigned int i=0;
	CString FilenameDst, FullFilenameDst;
	for ( i=Index; i<(PatternCount-1); i++ )
	{
		BoxToward = BOX_TOWARD_UP;
		Filename = AOIDataDefine.GetAlgPatternName(i+1, BoxToward);
		FullFilename.Format(_T("%s\\%s"), PatternFolder, Filename);
		FilenameDst = AOIDataDefine.GetAlgPatternName(i, BoxToward);
		FullFilenameDst.Format(_T("%s\\%s"), PatternFolder, FilenameDst);
		::CopyFile(FullFilename, FullFilenameDst, FALSE);
		::DeleteFile(FullFilename);

		BoxToward = BOX_TOWARD_LEFT;
		Filename = AOIDataDefine.GetAlgPatternName(i+1, BoxToward);
		FullFilename.Format(_T("%s\\%s"), PatternFolder, Filename);
		FilenameDst = AOIDataDefine.GetAlgPatternName(i, BoxToward);
		FullFilenameDst.Format(_T("%s\\%s"), PatternFolder, FilenameDst);
		::CopyFile(FullFilename, FullFilenameDst, FALSE);
		::DeleteFile(FullFilename);

		BoxToward = BOX_TOWARD_DOWN;
		Filename = AOIDataDefine.GetAlgPatternName(i+1, BoxToward);
		FullFilename.Format(_T("%s\\%s"), PatternFolder, Filename);
		FilenameDst = AOIDataDefine.GetAlgPatternName(i, BoxToward);
		FullFilenameDst.Format(_T("%s\\%s"), PatternFolder, FilenameDst);
		::CopyFile(FullFilename, FullFilenameDst, FALSE);
		::DeleteFile(FullFilename);

		BoxToward = BOX_TOWARD_RIGHT;
		Filename = AOIDataDefine.GetAlgPatternName(i+1, BoxToward);
		FullFilename.Format(_T("%s\\%s"), PatternFolder, Filename);
		FilenameDst = AOIDataDefine.GetAlgPatternName(i, BoxToward);
		FullFilenameDst.Format(_T("%s\\%s"), PatternFolder, FilenameDst);
		::CopyFile(FullFilename, FullFilenameDst, FALSE);
		::DeleteFile(FullFilename);
	}
	
	unsigned int PatParamCount = (unsigned int)(m_AlgPatternParamList.size());	
	if ( PatParamCount > Index )
	{
		std::vector<CPatternParam> PatternParamList = m_AlgPatternParamList;
		m_AlgPatternParamList.clear();
		for ( i=0; i<Index; i++ )
		{	AddAlgPatternParam(PatternParamList[i]);	}
		for ( i=Index; i<(PatParamCount-1); i++ )
		{	AddAlgPatternParam(PatternParamList[i+1]); }
	}

	unsigned int AlgPatternCount = GetAlgPatternCount();
	unsigned int AlgPatternResultIndex = GetAlgPatternResultIndex();
	if ( AlgPatternResultIndex > Index )
	{	AlgPatternResultIndex = AlgPatternResultIndex-1;	}
	else if ( AlgPatternResultIndex == Index )
	{	AlgPatternResultIndex = -1;	}

	if ( AlgPatternCount > 0 )
	{	AlgPatternCount = AlgPatternCount - 1; }
	SetAlgPatternCount(AlgPatternCount);
	SetAlgPatternResultIndex(AlgPatternResultIndex);	
	CAlgParam::GetAlgWndPtr()->SetWndModified(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ClearAlgPatternFiles()
{
	CAOIWnd *WndPtr = GetAlgWndPtr();	
	if ( false == m_AlgPatternFileUsed )
	{ 
		if ( NULL != WndPtr )
		{	WndPtr->SetWndModified(true); }
		SetAlgPatternCount(0);
		SetAlgPatternResultIndex(-1);
		m_AlgPatternParamList.clear();
		return true; 
	}	
	int   i=0;	
	CString Filename, FullFilename;
	CString PatternFolder = GetAlgPatternFolder();
	const int PatternCount = (int)(GetAlgPatternCount());
	if ( JetAPI::IsFolderExist(PatternFolder) == false ) 
	{
		if ( NULL != WndPtr )
		{	WndPtr->SetWndModified(true); }
		SetAlgPatternCount(0);
		SetAlgPatternResultIndex(-1);		
		m_AlgPatternParamList.clear();
		return true; 
	}
	JetAPI::ClearFolder(PatternFolder);
	
	/*
	for ( i=PatternCount-1; i!=-1; i-- )
	{
		Filename = AOIDataDefine.GetAlgPatternName(i, BOX_TOWARD_UP);
		FullFilename.Format(_T("%s\\%s"), PatternFolder, Filename);		
		::DeleteFile(FullFilename);

		Filename = AOIDataDefine.GetAlgPatternName(i, BOX_TOWARD_LEFT);
		FullFilename.Format(_T("%s\\%s"), PatternFolder, Filename);		
		::DeleteFile(FullFilename);

		Filename = AOIDataDefine.GetAlgPatternName(i, BOX_TOWARD_DOWN);
		FullFilename.Format(_T("%s\\%s"), PatternFolder, Filename);		
		::DeleteFile(FullFilename);

		Filename = AOIDataDefine.GetAlgPatternName(i, BOX_TOWARD_RIGHT);
		FullFilename.Format(_T("%s\\%s"), PatternFolder, Filename);		
		::DeleteFile(FullFilename);
	}
	if ( -1 != i )
	{
		int ResultCount = i-1;
		if ( ResultCount < 0 ) { ResultCount = 0; }
		unsigned int uResultCount = (unsigned int)(ResultCount);
		
		std::vector<CPatternParam> PatternParamList = m_AlgPatternParamList;
		m_AlgPatternParamList.clear();
		unsigned int PatParamCount = (unsigned int)(PatternParamList.size());
		if ( uResultCount > PatParamCount )
		{	uResultCount = PatParamCount;	}
		for ( i=0; i<uResultCount; i++ )
		{	AddAlgPatternParam(PatternParamList[i]);	}
		SetAlgPatternCount(uResultCount);		
		return false;
	}	
	*/
	SetAlgPatternCount(0);
	SetAlgPatternResultIndex(-1);	
	m_AlgPatternParamList.clear();
	if ( NULL != WndPtr )
	{	WndPtr->SetWndModified(true); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::RemoveAlgPatternFolder()
{
	if ( false == CAlgParam::m_AlgPatternFileUsed ) { return true; }		
	CString AlgPatternFolder = CAlgParam::GetAlgPatternFolder();
	if ( CAlgParam::ClearAlgPatternFiles() == false ) { return false; }
	::RemoveDirectory(AlgPatternFolder);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::LoadAlgPatternImage(unsigned int PatternIndex, BOX_TOWARD Toward, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr)
{
	bool    IsOK = false;
	CString FileName;
	CString PatternFolder = CAlgParam::GetAlgPatternFolder();
	CString ImageName = AOIDataDefine.GetAlgPatternName(PatternIndex, Toward);
	CString ExtName;

	JetAPI::ExtractExtendFileName(ImageName, ExtName);
	FileName.Format(_T("%s\\%s"), PatternFolder, ImageName);

	if ( ExtName.CompareNoCase(_T("PNG")) == 0 ) 
	{	IsOK = ImageAPI.LoadPNGImage(FileName, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true);	}
	else if ( ExtName.CompareNoCase(_T("JPG")) == 0 ) 
	{	IsOK = ImageAPI.LoadJPGImage(FileName, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true);	}
	else if ( ExtName.CompareNoCase(_T("BMP")) == 0 ) 
	{	IsOK = ImageAPI.LoadBMPImage(FileName, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true);	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReplaceAlgPatternImage(unsigned int PatternIndex, BOX_TOWARD Toward, IMAGE_SIZE BeforeW, IMAGE_SIZE BeforeH, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, LPCTSTR ModelFolder)
{
	bool IsOK = false;	
	RECT    Rect;
	RECT    RectL;
	RECT    RectT;
	RECT    RectR;
	RECT    RectB;
	CString filename;
	CString ImageName;
	CString ExtName;	
	
	size_t       i=0, j=0, k=0;
	CAOIWnd     *WndPtr=NULL;	
	IMAGE_SIZE   ImageW2 = 0;
	IMAGE_SIZE   ImageH2 = 0;
	IMAGE_SIZE   ImageStep2 = 0;
	IMAGE_PTR    ImagePtr2 = NULL;
	BOX_TOWARD   Toward2 = BOX_TOWARD_NULL;	
	const int    PatternCount = (int)(GetAlgPatternCount());		
	const int    PatternFolderIndex = GetAlgGroupID();
	CString      PatternFolder = AOIDataDefine.GetAlgPatternFolder(PatternFolderIndex);
	CString      AlgPatternFolder = CString(ModelFolder)+CString(_T("\\"))+PatternFolder;	
	if ( PatternIndex >= PatternCount ) { return false; }
	CPatternParam *PatternParamPtr = GetAlgPatternParamPtr(PatternIndex, true);
	if ( NULL == PatternParamPtr ) { return false; }	
	
	std::vector<CString> FileNameList;	

	WndPtr = GetAlgWndPtr();
	if ( JetAPI::CreateFolder(AlgPatternFolder) == false )
	{	return false; }
	CAlgParam::SetAlgPatternFolder(AlgPatternFolder);

	//存入四個方向
	Toward2 = Toward;
	ImageName = AOIDataDefine.GetAlgPatternName(PatternIndex, Toward2);	
	filename.Format(_T("%s\\%s"), AlgPatternFolder, ImageName);
	JetAPI::ExtractExtendFileName(ImageName, ExtName);
	IsOK = ImageAPI.SaveImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);	
	if ( false == IsOK )
	{	return false; }
	FileNameList.push_back(filename);
	
	Toward2 = JetAPI::RotateToward(90, Toward);	
	ImageName = AOIDataDefine.GetAlgPatternName(PatternIndex, Toward2);
	filename.Format(_T("%s\\%s"), AlgPatternFolder, ImageName);	
	if ( ImageAPI.RotateImage(90, ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImageW2, ImageH2, ImageStep2, ImagePtr2) == false )
	{	
		JetAPI::DeleteFileList(FileNameList);
		return false;	
	}
	JetAPI::ExtractExtendFileName(ImageName, ExtName);
	IsOK = ImageAPI.SaveImage(filename, ImageW2, ImageH2, ImageStep2, BitCount, ImagePtr2, true);	
	JetMemory.free_func(ImagePtr2);
	if ( false == IsOK )
	{	
		JetAPI::DeleteFileList(FileNameList);
		return false; 
	}
	FileNameList.push_back(filename);

	Toward2 = JetAPI::RotateToward(180, Toward);
	ImageName = AOIDataDefine.GetAlgPatternName(PatternIndex, Toward2);
	filename.Format(_T("%s\\%s"), AlgPatternFolder, ImageName);
	if ( ImageAPI.RotateImage(180, ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImageW2, ImageH2, ImageStep2, ImagePtr2) == false )
	{
		JetAPI::DeleteFileList(FileNameList);
		return false;	
	}
	JetAPI::ExtractExtendFileName(ImageName, ExtName);
	IsOK = ImageAPI.SaveImage(filename, ImageW2, ImageH2, ImageStep2, BitCount, ImagePtr2, true);	
	JetMemory.free_func(ImagePtr2);
	if ( false == IsOK )
	{
		JetAPI::DeleteFileList(FileNameList);
		return false; 
	}
	FileNameList.push_back(filename);

	Toward2 = JetAPI::RotateToward(270, Toward);
	ImageName = AOIDataDefine.GetAlgPatternName(PatternIndex, Toward2);
	filename.Format(_T("%s\\%s"), AlgPatternFolder, ImageName);	
	if ( ImageAPI.RotateImage(270, ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImageW2, ImageH2, ImageStep2, ImagePtr2) == false )
	{
		JetAPI::DeleteFileList(FileNameList);
		return false; 
	}
	JetAPI::ExtractExtendFileName(ImageName, ExtName);
	IsOK = ImageAPI.SaveImage(filename, ImageW2, ImageH2, ImageStep2, BitCount, ImagePtr2, true);	
	JetMemory.free_func(ImagePtr2);
	if ( false == IsOK )
	{
		JetAPI::DeleteFileList(FileNameList);
		return false; 
	}
	FileNameList.push_back(filename);
	
	PatternParamPtr->ModifyPatternSize(Toward, BeforeW, BeforeH, ImageW, ImageH);
	GetAlgWndPtr()->SetWndModified(true);
	//m_AlgPatternModified = TRUE;
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::AddAlgPatternImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const IMAGE_PTR ImagePtr, LPCTSTR ModelFolder, BOX_TOWARD Toward, int PolarityIdx, const CAlgBinaryParam &BinaryParam)
{	
	bool IsOK = false;	
	RECT    Rect;
	RECT    RectL;
	RECT    RectT;
	RECT    RectR;
	RECT    RectB;
	CString filename;
	CString ImageName;
	CString ExtName;	
	
	size_t       i=0, j=0, k=0;
	CAOIWnd     *WndPtr=NULL;	
	IMAGE_SIZE   ImageW2 = 0;
	IMAGE_SIZE   ImageH2 = 0;
	IMAGE_SIZE   ImageStep2 = 0;
	IMAGE_PTR    ImagePtr2 = NULL;
	BOX_TOWARD   Toward2 = BOX_TOWARD_NULL;	
	const int    PatternIndex = (int)(GetAlgPatternCount());
	const int    PatternFolderIndex = GetAlgGroupID();
	CString      PatternFolder = AOIDataDefine.GetAlgPatternFolder(PatternFolderIndex);
	CString      AlgPatternFolder = CString(ModelFolder)+CString(_T("\\"))+PatternFolder;	
	
	TPATTERN_ROI              PatternRoiL;
	TPATTERN_ROI              PatternRoiT;
	TPATTERN_ROI              PatternRoiR;
	TPATTERN_ROI              PatternRoiB;	
	std::vector<TPATTERN_ROI> PatRoiListL;
	std::vector<TPATTERN_ROI> PatRoiListT;
	std::vector<TPATTERN_ROI> PatRoiListR;
	std::vector<TPATTERN_ROI> PatRoiListB;
	std::vector<TPATTERN_ROI> RoiList;
	std::vector<CString> FileNameList;	

	WndPtr = GetAlgWndPtr();
	if ( JetAPI::CreateFolder(AlgPatternFolder) == false )
	{	return false; }
	CAlgParam::SetAlgPatternFolder(AlgPatternFolder);

	if ( NULL != WndPtr )
	{	
		double       ImageCpX=0;
		double       ImageCpY=0;
		double       RotateCpX=0;
		double       RotateCpY=0;
		TREGION4D    WndRgn;
		TREGION4D    WndRoiRgn;
		TREGION4D    WndRoiImageRgn;
		TREGION4D    WndRoiImageRgnL;
		TREGION4D    WndRoiImageRgnT;
		TREGION4D    WndRoiImageRgnR;
		TREGION4D    WndRoiImageRgnB;
		TPOINT2D     WndRgnCp;
		TPOINT2D     ImageRes;		
		POINT        ImageOffsetR, ImageOffsetT, ImageOffsetB, ImageOffsetL;		
		TPOINT2D     WndCornerPt[4];
		TPOINT2D     WndRoiCornerPt[4];
		
		CAOIWndRoi  *WndRoiPtr=NULL;
		size_t       WndRoiWndCount=0;	
		double       AttachedAngle = WndPtr->GetWndAttachedAngle();
		const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
		WndRoiWndCount = WndPtr->GetWndRoiWndCount();
		
		if ( false == IsExceptionAngle )
		{	WndPtr->GetWndRegion(WndRgn); }
		else
		{
			WndPtr->GetWndCornerPos(WndCornerPt);
			JetAPI::RotateCornerPos(-AttachedAngle, 0, 0, WndCornerPt);
			JetAPI::CornerPtToRegion(WndCornerPt, WndRgn);
		}		
		WndRgnCp.x = WndRgn.GetCpX();
		WndRgnCp.y = WndRgn.GetCpY();
		const double RegionW = WndRgn.GetWidth();
		const double RegionH = WndRgn.GetHeight();
		RotateCpX = 0.0;
		RotateCpY = 0.0;
		ImageCpX   = ImageW*0.5;
		ImageCpY   = ImageH*0.5;
		ImageRes.x = ImageW;
		ImageRes.y = ImageH;
		ImageRes.x = RegionW/ImageRes.x;
		ImageRes.y = RegionH/ImageRes.y;		
		//將區域轉成朝右為主		
		switch ( Toward )
		{
		case BOX_TOWARD_UP:
			ImageOffsetT.x = 0;			ImageOffsetT.y = 0;
			ImageOffsetL.x = 0;			ImageOffsetL.y = ImageW;
			ImageOffsetB.x = ImageW;	ImageOffsetB.y = ImageH;
			ImageOffsetR.x = ImageH;	ImageOffsetR.y = 0;
			break;
		case BOX_TOWARD_LEFT:
			ImageOffsetL.x = 0;			ImageOffsetL.y = 0;
			ImageOffsetB.x = 0;			ImageOffsetB.y = ImageW;
			ImageOffsetR.x = ImageW;	ImageOffsetR.y = ImageH;
			ImageOffsetT.x = ImageH;	ImageOffsetT.y = 0;
			break;
		case BOX_TOWARD_DOWN:
			ImageOffsetB.x = 0;			ImageOffsetB.y = 0;
			ImageOffsetR.x = 0;			ImageOffsetR.y = ImageW;
			ImageOffsetT.x = ImageW;	ImageOffsetT.y = ImageH;
			ImageOffsetL.x = ImageH;	ImageOffsetL.y = 0;
			break;
		case BOX_TOWARD_RIGHT:
			ImageOffsetR.x = 0;			ImageOffsetR.y = 0;
			ImageOffsetT.x = 0;			ImageOffsetT.y = ImageW;
			ImageOffsetL.x = ImageW;	ImageOffsetL.y = ImageH;
			ImageOffsetB.x = ImageH;	ImageOffsetB.y = 0;
			break;
		}		
		for ( i=0; i<WndRoiWndCount; i++ )
		{
			WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
			if ( NULL == WndRoiPtr ) { continue; }

			if ( false == IsExceptionAngle )
			{	WndRoiPtr->GetWndRoiRegion(WndRoiRgn); }
			else
			{
				WndRoiPtr->GetWndRoiCornerPos(WndRoiCornerPt);
				JetAPI::RotateCornerPos(-AttachedAngle, 0, 0, WndRoiCornerPt);
				JetAPI::CornerPtToRegion(WndRoiCornerPt, WndRoiRgn);
			}			
			AOIDataCollect.MapCadRegionToCamera(ImageW, ImageH, ImageRes, WndRoiRgn, WndRgnCp, WndRoiImageRgn);
			//將區域轉成朝右為主
			switch ( Toward )
			{
			case BOX_TOWARD_UP:
				WndRoiImageRgnT = WndRoiImageRgn;				
				JetAPI::RotateRegion(270.0, RotateCpX, RotateCpY, WndRoiImageRgn, WndRoiImageRgnL);
				JetAPI::RotateRegion(180.0, RotateCpX, RotateCpY, WndRoiImageRgn, WndRoiImageRgnB);
				JetAPI::RotateRegion(090.0, RotateCpX, RotateCpY, WndRoiImageRgn, WndRoiImageRgnR);
				break;
			case BOX_TOWARD_LEFT:
				WndRoiImageRgnL = WndRoiImageRgn;
				JetAPI::RotateRegion(270.0, RotateCpX, RotateCpY, WndRoiImageRgn, WndRoiImageRgnB);
				JetAPI::RotateRegion(180.0, RotateCpX, RotateCpY, WndRoiImageRgn, WndRoiImageRgnR);
				JetAPI::RotateRegion(090.0, RotateCpX, RotateCpY, WndRoiImageRgn, WndRoiImageRgnT);				
				break;
			case BOX_TOWARD_DOWN:
				WndRoiImageRgnB = WndRoiImageRgn;
				JetAPI::RotateRegion(270.0, RotateCpX, RotateCpY, WndRoiImageRgn, WndRoiImageRgnR);
				JetAPI::RotateRegion(180.0, RotateCpX, RotateCpY, WndRoiImageRgn, WndRoiImageRgnT);
				JetAPI::RotateRegion(090.0, RotateCpX, RotateCpY, WndRoiImageRgn, WndRoiImageRgnL);
				break;
			case BOX_TOWARD_RIGHT:
				WndRoiImageRgnR = WndRoiImageRgn;
				JetAPI::RotateRegion(270.0, RotateCpX, RotateCpY, WndRoiImageRgn, WndRoiImageRgnT);
				JetAPI::RotateRegion(180.0, RotateCpX, RotateCpY, WndRoiImageRgn, WndRoiImageRgnL);
				JetAPI::RotateRegion(090.0, RotateCpX, RotateCpY, WndRoiImageRgn, WndRoiImageRgnB);
				break;
			}
			
			JetAPI::Region4DToRect(WndRoiImageRgnR, RectR, false);
			JetAPI::Region4DToRect(WndRoiImageRgnT, RectT, false);
			JetAPI::Region4DToRect(WndRoiImageRgnL, RectL, false);
			JetAPI::Region4DToRect(WndRoiImageRgnB, RectB, false);
			
			RectR.left   += ImageOffsetR.x;
			RectR.top    += ImageOffsetR.y;
			RectR.right  += ImageOffsetR.x;
			RectR.bottom += ImageOffsetR.y;

			RectT.left   += ImageOffsetT.x;
			RectT.top    += ImageOffsetT.y;
			RectT.right  += ImageOffsetT.x;
			RectT.bottom += ImageOffsetT.y;

			RectL.left   += ImageOffsetL.x;
			RectL.top    += ImageOffsetL.y;
			RectL.right  += ImageOffsetL.x;
			RectL.bottom += ImageOffsetL.y;

			RectB.left   += ImageOffsetB.x;
			RectB.top    += ImageOffsetB.y;
			RectB.right  += ImageOffsetB.x;
			RectB.bottom += ImageOffsetB.y;

			PatternRoiL.RoiRect = RectL;
			PatternRoiT.RoiRect = RectT;
			PatternRoiR.RoiRect = RectR;
			PatternRoiB.RoiRect = RectB;

			PatRoiListL.push_back(PatternRoiL);			
			PatRoiListT.push_back(PatternRoiT);
			PatRoiListR.push_back(PatternRoiR);
			PatRoiListB.push_back(PatternRoiB);			
		}
	}

	//存入四個方向
	Toward2 = Toward;
	ImageName = AOIDataDefine.GetAlgPatternName(PatternIndex, Toward2);	
	filename.Format(_T("%s\\%s"), AlgPatternFolder, ImageName);
	JetAPI::ExtractExtendFileName(ImageName, ExtName);
	IsOK = ImageAPI.SaveImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);	
	if ( false == IsOK )
	{	return false; }
	FileNameList.push_back(filename);
	
	Toward2 = JetAPI::RotateToward(90, Toward);	
	ImageName = AOIDataDefine.GetAlgPatternName(PatternIndex, Toward2);
	filename.Format(_T("%s\\%s"), AlgPatternFolder, ImageName);	
	if ( ImageAPI.RotateImage(90, ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImageW2, ImageH2, ImageStep2, ImagePtr2) == false )
	{	
		JetAPI::DeleteFileList(FileNameList);
		return false;	
	}
	JetAPI::ExtractExtendFileName(ImageName, ExtName);
	IsOK = ImageAPI.SaveImage(filename, ImageW2, ImageH2, ImageStep2, BitCount, ImagePtr2, true);	
	JetMemory.free_func(ImagePtr2);
	if ( false == IsOK )
	{	
		JetAPI::DeleteFileList(FileNameList);
		return false; 
	}
	FileNameList.push_back(filename);

	Toward2 = JetAPI::RotateToward(180, Toward);
	ImageName = AOIDataDefine.GetAlgPatternName(PatternIndex, Toward2);
	filename.Format(_T("%s\\%s"), AlgPatternFolder, ImageName);
	if ( ImageAPI.RotateImage(180, ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImageW2, ImageH2, ImageStep2, ImagePtr2) == false )
	{
		JetAPI::DeleteFileList(FileNameList);
		return false;	
	}
	JetAPI::ExtractExtendFileName(ImageName, ExtName);
	IsOK = ImageAPI.SaveImage(filename, ImageW2, ImageH2, ImageStep2, BitCount, ImagePtr2, true);	
	JetMemory.free_func(ImagePtr2);
	if ( false == IsOK )
	{
		JetAPI::DeleteFileList(FileNameList);
		return false; 
	}
	FileNameList.push_back(filename);

	Toward2 = JetAPI::RotateToward(270, Toward);
	ImageName = AOIDataDefine.GetAlgPatternName(PatternIndex, Toward2);
	filename.Format(_T("%s\\%s"), AlgPatternFolder, ImageName);	
	if ( ImageAPI.RotateImage(270, ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImageW2, ImageH2, ImageStep2, ImagePtr2) == false )
	{
		JetAPI::DeleteFileList(FileNameList);
		return false; 
	}
	JetAPI::ExtractExtendFileName(ImageName, ExtName);
	IsOK = ImageAPI.SaveImage(filename, ImageW2, ImageH2, ImageStep2, BitCount, ImagePtr2, true);	
	JetMemory.free_func(ImagePtr2);
	if ( false == IsOK )
	{
		JetAPI::DeleteFileList(FileNameList);
		return false; 
	}
	FileNameList.push_back(filename);		
	
	unsigned int AlgPatternCount = GetAlgPatternCount();
	AlgPatternCount ++;
	SetAlgPatternCount(AlgPatternCount);

	CPatternParam PatternParam;	
	PatternParam.SetBinaryParam(BinaryParam);	
	PatternParam.SetPatRoiListL(PatRoiListL);
	PatternParam.SetPatRoiListT(PatRoiListT);
	PatternParam.SetPatRoiListR(PatRoiListR);
	PatternParam.SetPatRoiListB(PatRoiListB);
	PatternParam.SetResultPolarityIdx(PolarityIdx);

	//RoiList
	AddAlgPatternParam(PatternParam);
	GetAlgWndPtr()->SetWndModified(true);
	//m_AlgPatternModified = TRUE;
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAlgParam::GetAlgPatternParamCount() const
{
	return m_AlgPatternParamList.size();
}
//-------------------------------------------------------------------------------------//
CPatternParam* CAlgParam::GetAlgPatternParamPtr(size_t index, bool Check)
{
	if ( true == Check )
	{
		const size_t Count = m_AlgPatternParamList.size();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return &(m_AlgPatternParamList[index]);
}
//-------------------------------------------------------------------------------------//
CPatternParam* CAlgParam::GetAlgPatternParamPtrByMaxPatRoiCount(BOX_TOWARD Toward, int PolarityIdx)//取得最多小框的樣板參數指標
{
	size_t MaxPatRoiCount=0;
	CPatternParam *Ptr=NULL;
	CPatternParam *MaxPtr=NULL;
	const size_t Count=GetAlgPatternParamCount();
	for ( size_t i=0; i<Count; i++ )
	{
		Ptr = GetAlgPatternParamPtr(i, false);
		if ( NULL == Ptr ) { continue; }
		const std::vector<TPATTERN_ROI> &PatRoiList=Ptr->GetPatRoiList(Toward, PolarityIdx);
		if ( NULL==MaxPtr || MaxPatRoiCount<PatRoiList.size() )
		{
			MaxPtr = Ptr;
			MaxPatRoiCount=PatRoiList.size();
		}
	}
	return MaxPtr;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ClearAlgPatternParamList()//清除樣板參數列表
{
	m_AlgPatternParamList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::AddAlgPatternParam(CPatternParam &PatParam)//加入樣板參數列表
{
	unsigned int PatIndex = (unsigned int)(m_AlgPatternParamList.size());
	PatParam.GetBinaryParam().SetBinaryBelongToWho(BIN_PARAM_BELONG_TO_PATTERN_IMAGE);
	PatParam.GetBinaryParam().SetBinaryBelongIndex(PatIndex);
	m_AlgPatternParamList.push_back(PatParam);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReplaceAlgPatternParam(size_t index, CPatternParam &PatParam)//取代樣板參數列表
{
	const size_t PatCount = m_AlgPatternParamList.size();
	if ( index >= PatCount ) { return false; }	
	PatParam.GetBinaryParam().SetBinaryBelongToWho(BIN_PARAM_BELONG_TO_PATTERN_IMAGE);
	PatParam.GetBinaryParam().SetBinaryBelongIndex((unsigned int)(index));
	m_AlgPatternParamList[index] = PatParam;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::GetAlgPatternParamPatTextList(std::vector<std::wstring> &List)//取得樣板參數內的樣板文字列表 
{	
	List.clear();
	CPatternParam *Ptr=NULL;
	const size_t Count=GetAlgPatternParamCount();	
	for ( size_t i=0; i<Count; i++ )
	{
		Ptr = GetAlgPatternParamPtr(i, false);
		if ( NULL == Ptr ) { continue; }
		std::wstring ws=Ptr->GetPatText();
		if ( ws.length() == 0 ) { continue; }//樣板文字未設定也不能用其餘文字
		List.push_back(ws);

		const size_t SimilarTextCount=Ptr->GetPatSimilarTextCount();
		for ( size_t j=0; j<SimilarTextCount; j++ )
		{
			ws = Ptr->GetPatSimilarText(j, false);
			if ( 0 == ws.length() ) { continue; }
			List.push_back(ws);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckAlgParamUsing3D() const//確認演算法使用3D
{
	const int ResinHeightAlignEnabled = AOIDataCollect.GetSystemParameter().m_ResinHeightAlignEnabled;	
	if ( ALG_HEIGHT == GetAlgType() )
	{	return true; }
	if ( FN_ENABLE == ResinHeightAlignEnabled )
	{
		if ( ALG_MODEL_MATCH == GetAlgType() )
		{
			if ( GetAlgParamResinHeight().enabled )
			{	return true;	}
		}
	}		
	if ( FRAME_UNIQUE_ID_DLP == GetAlgMaskBinParam().GetBinaryFrameUniqueID() )
	{	return true; }

	if ( FRAME_UNIQUE_ID_DLP == GetAlgImageBinParam().GetBinaryFrameUniqueID() )
	{	return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckAlgShowSkewByAlgType(ALG_TYPE AlgType) const
{
	bool bEnabled=false;
	switch ( AlgType )
	{
	case ALG_OBJECT_MEASURE:
	//case ALG_SHAPE_VERIFY:
	case ALG_MODEL_MATCH:
	case ALG_IMAGE_MATCH:	
	case ALG_CHAR_VERIFY:	
	//case ALG_FD_MATCH:
	case ALG_EDGE_SEARCH:	
	case ALG_PIXEL_COMPARE:
		bEnabled = true;
		break;
	}	
	return bEnabled;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckAlgShowScaleByAlgType(ALG_TYPE AlgType) const
{
	bool bEnabled=false;
	switch ( AlgType )
	{
	//case ALG_OBJECT_MEASURE:
	//case ALG_SHAPE_VERIFY:	
	case ALG_MODEL_MATCH:
	case ALG_IMAGE_MATCH:	
	//case ALG_CHAR_VERIFY:	
	case ALG_FD_MATCH:
	//case ALG_EDGE_SEARCH:	
	//case ALG_PIXEL_COMPARE:
	case ALG_MEASURE_SIP_DISTANCE:
		bEnabled = true;
		break;
	}	
	return bEnabled;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckAlgShowOffsetByAlgType(ALG_TYPE AlgType) const
{
	bool bEnabled=false;
	switch ( AlgType )
	{
	case ALG_OBJECT_MEASURE:
	//case ALG_SHAPE_VERIFY:	
	case ALG_MODEL_MATCH:
	case ALG_IMAGE_MATCH:	
	//case ALG_CHAR_VERIFY:	
	case ALG_FD_MATCH:
	case ALG_EDGE_SEARCH:	
	case ALG_PIXEL_COMPARE:
	case ALG_MEASURE_SIP_DISTANCE:
	case ALG_MEASURE_CONNECTOR:
	case ALG_MEASURE_CONNECTOR_PIN:
		bEnabled = true;
		break;
	}	
	return bEnabled;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CalcAlgOffsetL()//計算演算法XY偏移長度
{
	const double dX=GetAlgOffsetXReading();
	const double dY=GetAlgOffsetYReading();
	const double dL=sqrt((dX*dX)+(dY*dY));
	SetAlgOffsetLReading(dL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckAlgOffsetAngle(double Height)//確認演算法XY偏移角度
{	
	const bool bEnable=GetAlgOffsetAEnabled();
	if ( false == bEnable ) { return true; }	
	if ( DBL_MAX == Height ) { return true; }

	const double OffsetH = Height;
	const double LSL = GetAlgOffsetALSL();
	const double USL = GetAlgOffsetAUSL();
	const double OffsetX = GetAlgOffsetXReading();
	const double OffsetY = GetAlgOffsetYReading();	
	const double OffsetR = JetAPI::CalcDistance(OffsetX, OffsetY);
	const double OffsetL = JetAPI::CalcDistance(OffsetR, OffsetH);
	//const double CosAngle= JetAPI::CalcCos(OffsetR, OffsetL, OffsetH)-90;
	const double CosAngle= JetAPI::CalcCos(OffsetH, OffsetL, OffsetR);
	SetAlgOffsetAReading(CosAngle);
	RESULT_ID ResultID=GetAlgResultID();
	if ( RESULT_ID_OK == ResultID )
	{
		if ( CosAngle<LSL || CosAngle>USL )
		{
			CString strKey, strResult;
			ResultID = RESULT_ID_NG;			
			strKey = AOIDataDefine.GetOffsetText();
			strResult.Format(_T("A-%s:%.2f (%.2f~%.2f)"), strKey, CosAngle, LSL, USL);						
			SetAlgResultID(ResultID);
			SetAlgResultText(strResult);
			SetAlgResultReading1(CosAngle);
			CAOIWnd *WndPtr=GetAlgWndPtr();
			if ( NULL != WndPtr )
			{	
				WndPtr->SetWndResultID(ResultID);
				WndPtr->SetWndResultValue(CosAngle);
				WndPtr->SetWndResultText(strResult);

				WND_LOGIC_TYPE WndLogicType = WndPtr->GetWndLogicType();
				if ( WND_LOGIC_NONE == WndLogicType )
				{	WndPtr->SetWndLogicResultID(ResultID);	}
			}
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckAlgOffset(double &SelfX, double &SelfY, double &SelfA, double &OhtersX, double &OhtersY, double &OhtersA, bool ChkDefect)
{
	CString      strResult, strKey;
	const bool   ChangeOffsetParam=!ChkDefect;
	const double SkewA   = GetAlgSkewReading();
	const double OffsetX = GetAlgOffsetXReading();
	const double OffsetY = GetAlgOffsetYReading();
	const double OffsetL = GetAlgOffsetLReading();
	const double CadScaleX = GetAlgScaleXReading();
	const double CadScaleY = GetAlgScaleYReading();
	const bool OrgOffsetXEnabled = GetAlgOffsetXEnabled();
	const bool OrgOffsetYEnabled = GetAlgOffsetYEnabled();	

	if ( true == ChangeOffsetParam )
	{
		SetAlgOffsetXEnabled(true);
		SetAlgOffsetYEnabled(true);
	}
	const bool AlgSkewEnabled = GetAlgSkewEnabled();
	const bool AlgOffsetXEnabled = GetAlgOffsetXEnabled();
	const bool AlgOffsetYEnabled = GetAlgOffsetYEnabled();	
	const bool AlgOffsetLEnabled = GetAlgOffsetLEnabled();	
	const bool AlgScaleEnabled = GetAlgScaleEnabled();
	
	double USL=0;
	double LSL=0;
	double Reading=0;
	double CadSkew_Self=0;
	double CadSkew_Others=0;
	double CadScaleX_Self=0;
	double CadScaleY_Self=0;
	double CadScaleX_Others=0;
	double CadScaleY_Others=0;
	double CadOffsetX_Self=0;
	double CadOffsetY_Self=0;
	double CadOffsetX_Others=0;
	double CadOffsetY_Others=0;	
	if ( true==AlgOffsetXEnabled )	
	{
		Reading = OffsetX;
		USL = GetAlgOffsetXUSL();
		LSL = GetAlgOffsetXLSL();
		CadOffsetX_Self = OffsetX;
		CadOffsetX_Others = OffsetX;		
		if ( Reading<LSL || Reading>USL )
		{
			//CadOffsetX_Others = 0;			
			if ( true == ChkDefect )
			{
				strKey = AOIDataDefine.GetOffsetText();
				strResult.Format(_T("X-%s:%.0fum (%.0f~%.0f)"), strKey, Reading, LSL, USL);
				SetAlgResultID(RESULT_ID_NG);
				SetAlgResultText(strResult);
			}
		}		
	}
	if ( true==AlgOffsetYEnabled )	
	{
		Reading = OffsetY;
		USL = GetAlgOffsetYUSL();
		LSL = GetAlgOffsetYLSL();
		CadOffsetY_Self = OffsetY;
		CadOffsetY_Others = OffsetY;
		if ( Reading<LSL || Reading>USL )
		{
			//CadOffsetY_Others = 0;			
			if ( true == ChkDefect )
			{
				strKey = AOIDataDefine.GetOffsetText();
				strResult.Format(_T("Y-%s:%.0fum (%.0f~%.0f)"), strKey, Reading, LSL, USL);						
				SetAlgResultID(RESULT_ID_NG);
				SetAlgResultText(strResult);
			}
		}		
	}
	if ( true==AlgOffsetLEnabled )
	{
		Reading = OffsetL;
		USL = GetAlgOffsetLUSL();
		LSL = GetAlgOffsetLLSL();		
		if ( Reading<LSL || Reading>USL )
		{
			if ( true == ChkDefect )
			{
				strKey = AOIDataDefine.GetOffsetText();
				strResult.Format(_T("L-%s:%.0fum (%.0f~%.0f)"), strKey, Reading, LSL, USL);						
				SetAlgResultID(RESULT_ID_NG);
				SetAlgResultText(strResult);
			}
		}	
	}
	if ( true==AlgSkewEnabled )	
	{
		Reading = SkewA;
		USL = GetAlgSkewUSL();
		LSL = GetAlgSkewLSL();
		CadSkew_Self = SkewA;
		CadSkew_Others = SkewA;
		if ( Reading<LSL || Reading>USL )
		{
			//CadSkew_Others = 0;			
			if ( true == ChkDefect )
			{
				strKey = AOIDataDefine.GetSkewText();
				strResult.Format(_T("%s:%.2f (%.2f~%.2f)"), strKey, Reading, LSL, USL);
				SetAlgResultID(RESULT_ID_NG);
				SetAlgResultText(strResult);
			}			
		}		
	}
	if ( true == AlgScaleEnabled )
	{
		USL = GetAlgScaleUSL()+1;
		LSL = GetAlgScaleLSL()-1;
		CadScaleX_Self = CadScaleX;
		CadScaleY_Self = CadScaleY;
		CadScaleX_Others = CadScaleX;
		CadScaleY_Others = CadScaleY;
		if ( CadScaleX<LSL || CadScaleY<LSL || CadScaleX>USL || CadScaleY>USL )
		{
			//CadScaleX_Others = 100;
			//CadScaleY_Others = 100;
			if ( true == ChkDefect )
			{
				strKey = AOIDataDefine.GetRatioText();
				strResult.Format(_T("%s:(%.0f, %.0f) (%.0f~%.0f)"), strKey, CadScaleX, CadScaleY, LSL, USL);
				SetAlgResultID(RESULT_ID_NG);
				SetAlgResultText(strResult);
			}
		}		
	}

	SelfA = CadSkew_Self;
	SelfX = CadOffsetX_Self;
	SelfY = CadOffsetY_Self;

	OhtersA = CadSkew_Others;
	OhtersX = CadOffsetX_Others;
	OhtersY = CadOffsetY_Others;

	if ( true == ChangeOffsetParam )
	{
		SetAlgOffsetXEnabled(OrgOffsetXEnabled);
		SetAlgOffsetYEnabled(OrgOffsetYEnabled);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
double CAlgParam::GetAlgSkewCalcRange() const//取得角度計算的範圍
{	
	const double absSkewUSL = ::fabs(GetAlgSkewUSL());
	const double absSkewLSL = ::fabs(GetAlgSkewLSL());
	const double AngleExpand = GetAlgPatternAngleExpand();	
	const double dSkew = MAX(absSkewUSL, absSkewLSL)+AngleExpand;
	return dSkew;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckRotateImage(double Angle) const//確認是否旋轉影像
{
	if ( Angle<-0.5 || Angle>0.5 )
	{	return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
 bool CAlgParam::GetAlgScaleCalcRange(double &Min, double &Max) const//取得縮放比例的計算範圍
{
	const double ScaleUSL = GetAlgScaleUSL();
	const double ScaleLSL = GetAlgScaleLSL();
	const double dScaleMax = MAX(ScaleUSL, ScaleLSL);
	const double dScaleMin = MIN(ScaleUSL, ScaleLSL);
	const double ScaleExpand = GetAlgPatternScaleExpand();
	Max = (dScaleMax+ScaleExpand)/100.0;
	Min = (dScaleMin-ScaleExpand)/100.0;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::GetInspectionImage(std::vector<TUNI_FRAME> &UniFrameList, unsigned int FrameUniqueID, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, MASK_PTR &MaskPtr, SPACE_PTR &SpacePtr)
{
	size_t i=0;
	const size_t ListSize = UniFrameList.size();
	for ( i=0; i<ListSize; i++ )
	{
		if ( FrameUniqueID != UniFrameList[i].FrameUniqueID ) { continue; }

		ImageW    = UniFrameList[i].ImageW;
		ImageH    = UniFrameList[i].ImageH;
		ImageStep = UniFrameList[i].ImageStep;
		BitCount  = UniFrameList[i].BitCount;
		ImagePtr  = UniFrameList[i].ImagePtr;
		MaskPtr   = UniFrameList[i].MaskPtr;
		SpacePtr  = UniFrameList[i].SpacePtr;		
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------//
void CAlgParam::SetAlgResultText(LPCTSTR  value)
{ 
	m_AlgResultText = value; 
	//if ( m_AlgResultText == _T("OK") )
	//{	m_AlgResultText = value; }
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ChangeAlgType(ALG_TYPE AlgType, bool bChkPatFolder)//變更演算法樣式
{
	TREGION4D  WndRgn;
	CAOILand  *LandPtr = NULL;	
	CAOIModel *ModelPtr = NULL;
	CAOIWnd   *WndPtr = GetAlgWndPtr();		
	const bool ImageFileUsed = CheckAlgPatternFileUsed(AlgType);
	if ( true == bChkPatFolder )
	{
		if ( CheckAlgPatternFileUsed()==true && false==ImageFileUsed )
		{	RemoveAlgPatternFolder();	}	
	}
	
	bool   AlgSkewEnabled=false;
	bool   AlgOffsetXEnabled=false;
	bool   AlgOffsetYEnabled=false;
	double WndSizeX=0;
	double WndSizeY=0;
	double WndHeight=0;
	double WndArea=0;
	double WndVolume=0;		
	WND_DEFECT_ID WndDefectID=WND_DEFECT_NONE;
	double CadOffsetXUSL = GetAlgOffsetXUSL();
	double CadOffsetXLSL = GetAlgOffsetXLSL();
	double CadOffsetYUSL = GetAlgOffsetYUSL();
	double CadOffsetYLSL = GetAlgOffsetYLSL();	
	BINARY_MODE BinaryMode=GetAlgImageBinParam().GetBinaryMode();
	double PatternSimilarityUSL = GetAlgPatternSimilarityUSL();
	double PatternSimilarityLSL = GetAlgPatternSimilarityLSL();

	if ( NULL != WndPtr )
	{		
		WndPtr->GetWndRegion(WndRgn); 		
		WndSizeX = WndRgn.GetWidth();
		WndSizeY = WndRgn.GetHeight();
		WndArea = WndSizeX*WndSizeY;
		WndDefectID=WndPtr->GetWndDefectID();
	}

	switch ( AlgType )
	{
	case ALG_OUTER_SHORT:
	case ALG_BLOB_COUNT:
		if ( BINARY_DISABLE == BinaryMode )
		{	BinaryMode=BINARY_FIXED_THRESHOLD; }
		break;
	case ALG_OBJECT_MEASURE:
		AlgSkewEnabled = true;
		AlgOffsetXEnabled = true;
		AlgOffsetYEnabled = true;		
		BinaryMode=BINARY_FIXED_THRESHOLD;
		GetAlgParamObjectMeasure().omSizeXSpec = WndSizeX;
		GetAlgParamObjectMeasure().omSizeYSpec = WndSizeY;		
		//GetAlgParamObjectMeasure().omHeightSpec = WndHeight;		
		//GetAlgParamObjectMeasure().omAreaSpec  = WndArea;
		//GetAlgParamObjectMeasure().omVolumeSpec = WndVolume;				
		break;
	case ALG_COLOR_CODE:
		if ( BINARY_DISABLE == BinaryMode )
		{	BinaryMode=BINARY_COLOR_FILTER; }
		break;
	case ALG_MODEL_MATCH:		
		AlgOffsetXEnabled = true;
		AlgOffsetYEnabled = true;
		PatternSimilarityUSL = 100;
		PatternSimilarityLSL =  20;
		BinaryMode = BINARY_DISABLE;
		break;
	case ALG_IMAGE_MATCH:
		AlgOffsetXEnabled = true;
		AlgOffsetYEnabled = true;
		PatternSimilarityUSL = 100;
		PatternSimilarityLSL =  75;
		BinaryMode = BINARY_DISABLE;
		break;
	case ALG_FD_MATCH:
		AlgOffsetXEnabled = true;
		AlgOffsetYEnabled = true;
		PatternSimilarityUSL = 100;
		PatternSimilarityLSL =  75;
		CadOffsetXUSL =  2000;
		CadOffsetXLSL = -2000;
		CadOffsetYUSL =  2000;
		CadOffsetYLSL = -2000;	
		break;
	case ALG_CHAR_VERIFY:
		AlgOffsetXEnabled = false;
		AlgOffsetYEnabled = false;
		BinaryMode = BINARY_DISABLE;
		GetAlgParamAiModel().aiModelID = ALG_AI_MODEL_OCR_01;
		break;
	case ALG_EDGE_SEARCH:		
		AlgOffsetXEnabled = true;
		AlgOffsetYEnabled = true;
		if ( BINARY_DISABLE == BinaryMode )
		{	BinaryMode = BINARY_FIXED_THRESHOLD; }
		break;	
	case ALG_SHAPE_VERIFY:
		AlgOffsetXEnabled = true;
		AlgOffsetYEnabled = true;
		if ( BINARY_DISABLE == BinaryMode )
		{	BinaryMode = BINARY_FIXED_THRESHOLD; }
		break;
	case ALG_ANGLE_MEASURE:
		AlgOffsetXEnabled = false;
		AlgOffsetYEnabled = false;
		break;	
	default:
	case ALG_SOLDER_WETTING:
		AlgOffsetXEnabled = false;
		AlgOffsetYEnabled = false;
		BinaryMode = BINARY_DISABLE;
		break;
	}

	const bool WndCanToAlign=AOIDataDefine.CheckWndDefectIDCanToAlign(WndDefectID);
	if ( false == WndCanToAlign )
	{
		//if ( true == AlgOffsetXEnabled )
		{
			CadOffsetXUSL =  1000;
			CadOffsetXLSL = -1000;
		}
		//if ( true == AlgOffsetYEnabled )
		{
			CadOffsetYUSL =  1000;
			CadOffsetYLSL = -1000;
		}
	}

	SetAlgType(AlgType);		
	SetAlgOffsetXUSL(CadOffsetXUSL);
	SetAlgOffsetXLSL(CadOffsetXLSL);
	SetAlgOffsetYUSL(CadOffsetYUSL);
	SetAlgOffsetYLSL(CadOffsetYLSL);
	SetAlgPatternFileUsed(ImageFileUsed);		
	SetAlgPatternSimilarityUSL(PatternSimilarityUSL);
	SetAlgPatternSimilarityLSL(PatternSimilarityLSL);	
	
	AlgSkewEnabled = CheckAlgShowSkewByAlgType(AlgType);
	AlgOffsetXEnabled = CheckAlgShowOffsetByAlgType(AlgType);
	AlgOffsetYEnabled = CheckAlgShowOffsetByAlgType(AlgType);
	SetAlgSkewEnabled(AlgSkewEnabled);
	SetAlgOffsetXEnabled(AlgOffsetXEnabled);
	SetAlgOffsetYEnabled(AlgOffsetYEnabled);	

	GetAlgImageBinParam().SetBinaryMode(BinaryMode);

	if ( NULL != WndPtr )
	{
		double ExtendX = 0;
		double ExtendY = 0;
		double ExtendXBefore = WndPtr->GetWndExtendRangeX();
		double ExtendYBefore = WndPtr->GetWndExtendRangeY();
		bool UseExtendBoxBefore = WndPtr->GetWndExtendBoxUsed();
		WND_DEFECT_ID WndDefectID = WndPtr->GetWndDefectID();
		bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
		const bool bUsedMaskWnd = WndPtr->CheckWndAlgUsedMaskWnd(AlgType);
		if ( true == UseExtendBox )
		{
			switch ( WndDefectID )
			{
			case WND_DEFECT_PAD_ALIGN:
				ExtendX = 800;
				ExtendY = 800;
				break;
			default:
				ExtendX = 400;
				ExtendY = 400;
				break;
			}
		}				
		if ( UseExtendBoxBefore != UseExtendBox )
		{
			WndPtr->SetWndExtendRangeX(ExtendX);
			WndPtr->SetWndExtendRangeY(ExtendY);
			WndPtr->SetWndExtendBoxUsed(UseExtendBox);
			WndPtr->UpdateWndExtendBox();
		}
		WndPtr->ClearWndRoiWndList();
		if ( false == bUsedMaskWnd )
		{	WndPtr->ClearWndMaskWndList(); }
		WndPtr->SetWndRgnLinkAuto(false);
		WndPtr->SetWndRgnLinkMode(WND_RGN_LINK_NONE);				
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgParam::MirrorAlgParamXAxis()
{
	m_AlgMaskBinParam.MirrorBinaryParamXAxis();
	m_AlgImageBinParam.MirrorBinaryParamXAxis();
}
//-------------------------------------------------------------------------------------//
void CAlgParam::MirrorAlgParamYAxis()
{
	m_AlgMaskBinParam.MirrorBinaryParamYAxis();
	m_AlgImageBinParam.MirrorBinaryParamYAxis();
}
//-------------------------------------------------------------------------------------//
void CAlgParam::RotateAlgParam(double Angle)
{	
	int          nTemp1=0, nTemp2=0;	
	double       dTemp1=0, dTemp2=0;
	double       dCPX=0.0, dCPY=0.0;
	double       dReading1=0, dReading2=0;
	bool         bTemp1=false, bTemp2=false;
	//const double AngleRadius = Angle*DEG_TO_RAD_DBL;
	//const double COS = cos(AngleRadius);
	//const double SIN = sin(AngleRadius);	
	double       COS=0.0, SIN=0.0, AngleRadius=0.0;
	const int    AngleLabel = JetAPI::GetAngleLabel(Angle);
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(Angle);	
	/*
	dTemp1 = GetAlgOffsetXUSL()-dCPX;
	dTemp2 = GetAlgOffsetYUSL()-dCPY;
	dReading1 = (dTemp1*COS)-(dTemp2*SIN)+dCPX;
	dReading2 = (dTemp1*SIN)+(dTemp2*COS)+dCPY;
	SetAlgOffsetXUSL(dReading1);
	SetAlgOffsetYUSL(dReading2);

	dTemp1 = GetAlgOffsetXLSL()-dCPX;
	dTemp2 = GetAlgOffsetYLSL()-dCPY;
	dReading1 = (dTemp1*COS)-(dTemp2*SIN)+dCPX;
	dReading2 = (dTemp1*SIN)+(dTemp2*COS)+dCPY;
	SetAlgOffsetXLSL(dReading1);
	SetAlgOffsetYLSL(dReading2);
	dReading1 = MIN(GetAlgOffsetXUSL(), GetAlgOffsetXLSL());
	dReading2 = MAX(GetAlgOffsetXUSL(), GetAlgOffsetXLSL());
	SetAlgOffsetXLSL(dReading1);
	SetAlgOffsetXUSL(dReading2);
	dReading1 = MIN(GetAlgOffsetYUSL(), GetAlgOffsetYLSL());
	dReading2 = MAX(GetAlgOffsetYUSL(), GetAlgOffsetYLSL());
	SetAlgOffsetYLSL(dReading1);
	SetAlgOffsetYUSL(dReading2);
	*/
	switch ( AngleLabel )
	{
	case 90:
	case 270:
		SwapAlgParam(m_AlgOffsetXUSL, m_AlgOffsetYUSL);
		SwapAlgParam(m_AlgOffsetXLSL, m_AlgOffsetYLSL);
		SwapAlgParam(m_AlgOffsetXEnabled, m_AlgOffsetYEnabled);				
		break;
	default:
		break;
	}

	m_AlgMaskBinParam.RotateBinaryParam(AngleLabel);
	m_AlgImageBinParam.RotateBinaryParam(AngleLabel);	

	RotateAlgParam_BrightRatio(AngleLabel);
	RotateAlgParam_OuterShort(AngleLabel);
	RotateAlgParam_BlobCount(AngleLabel);
	RotateAlgParam_BodyTilt(AngleLabel);	
	RotateAlgParam_ImageMatch(AngleLabel);
	RotateAlgParam_ObjectMeasure(AngleLabel);
	if ( true == IsExceptionAngle )
	{	
		//AngleRadius = Angle*DEG_TO_RAD_DBL;
		AngleRadius = AngleLabel*DEG_TO_RAD_DBL;//為了與參數對齊, 所以使用此方式
		COS = cos(AngleRadius);
		SIN = sin(AngleRadius);

		dCPX = 0.0;
		dCPY = 0.0;
		dTemp1 = GetAlgOffsetXReading()-dCPX;
		dTemp2 = GetAlgOffsetYReading()-dCPY;
		dReading1 = (dTemp1*COS)-(dTemp2*SIN)+dCPX;
		dReading2 = (dTemp1*SIN)+(dTemp2*COS)+dCPY;
		SetAlgOffsetXReading(dReading1);
		SetAlgOffsetYReading(dReading2);

		dCPX = 0.0;
		dCPY = 0.0;
		dTemp1 = GetAlgImageOffsetX()-dCPX;
		dTemp2 = GetAlgImageOffsetY()-dCPY;
		dReading1 = (dTemp1*COS)-(dTemp2*SIN)+dCPX;
		dReading2 = (dTemp1*SIN)+(dTemp2*COS)+dCPY;
		SetAlgImageOffsetX(dReading1);
		SetAlgImageOffsetY(dReading2);
	}	

	/*
	TALG_PARAM_MODEL_MATCH     m_AlgParamModelMatch;           //演算法-模板匹配參數
	TALG_PARAM_IMAGE_MATCH     m_AlgParamImageMatch;         //演算法-影像匹配參數
	TALG_PARAM_CHAR_VERIFY     m_AlgParamCharVerify;         //演算法-文字驗證參數
	TALG_PARAM_PIXEL_COMPARE   m_AlgParamPixelCompare;
	*/
}
//-------------------------------------------------------------------------------------//
void CAlgParam::RotateAlgParam_BrightRatio(int AngleLabel)
{		
	switch ( AngleLabel )
	{
	case 0:
		break;
	case 90:
	case 270:
		SwapAlgParam(m_AlgParamBrightRatio.brXLineMode, m_AlgParamBrightRatio.brYLineMode);
		SwapAlgParam(m_AlgParamBrightRatio.brXLineLSL, m_AlgParamBrightRatio.brYLineLSL);
		SwapAlgParam(m_AlgParamBrightRatio.brXLineUSL, m_AlgParamBrightRatio.brYLineUSL);
		SwapAlgParam(m_AlgParamBrightRatio.brXLineRange, m_AlgParamBrightRatio.brYLineRange);
		SwapAlgParam(m_AlgParamBrightRatio.brXLineEnabled, m_AlgParamBrightRatio.brYLineEnabled);
		SwapAlgParam(m_AlgParamBrightRatio.brXLineReading, m_AlgParamBrightRatio.brYLineReading);
		SwapAlgParam(m_AlgParamBrightRatio.brXLineUnitMode, m_AlgParamBrightRatio.brYLineUnitMode);
		break;
	}
}
//-------------------------------------------------------------------------------------//
void CAlgParam::RotateAlgParam_OuterShort(int AngleLabel)
{
	int       nTemp1=0, nTemp2=0;	
	double    dTemp1=0, dTemp2=0;
	bool      bTemp1=false, bTemp2=false;	
	TALG_PARAM_OUTER_SHORT TempOuterShort=m_AlgParamOuterShort;
	switch ( AngleLabel )
	{
	case 0:
		break;
	case 90:		
		m_AlgParamOuterShort.osLine_T = TempOuterShort.osLine_R;//R - > T		
		m_AlgParamOuterShort.osLine_L = TempOuterShort.osLine_T;//T - > L		
		m_AlgParamOuterShort.osLine_B = TempOuterShort.osLine_L;//L - > B		
		m_AlgParamOuterShort.osLine_R = TempOuterShort.osLine_B;//B - > R
		break;
	case 180:
		m_AlgParamOuterShort.osLine_L = TempOuterShort.osLine_R;//R - > L		
		m_AlgParamOuterShort.osLine_B = TempOuterShort.osLine_T;//T - > B		
		m_AlgParamOuterShort.osLine_R = TempOuterShort.osLine_L;//L - > R		
		m_AlgParamOuterShort.osLine_T = TempOuterShort.osLine_B;//B - > T	
		break;
	case 270:
		m_AlgParamOuterShort.osLine_B = TempOuterShort.osLine_R;//R - > B
		m_AlgParamOuterShort.osLine_R = TempOuterShort.osLine_T;//T - > R
		m_AlgParamOuterShort.osLine_T = TempOuterShort.osLine_L;//L - > T
		m_AlgParamOuterShort.osLine_L = TempOuterShort.osLine_B;//B - > L		
		break;
	}
}
//-------------------------------------------------------------------------------------//
void CAlgParam::RotateAlgParam_BlobCount(int AngleLabel)
{	
	switch ( AngleLabel )
	{
	case 0:
		break;
	case 90:
	case 270:		
		SwapAlgParam(m_AlgParamBlobCount.bcXSizeMax, m_AlgParamBlobCount.bcYSizeMax);
		SwapAlgParam(m_AlgParamBlobCount.bcXSizeMin, m_AlgParamBlobCount.bcYSizeMin);
		SwapAlgParam(m_AlgParamBlobCount.bcXSizeMaxEnabled, m_AlgParamBlobCount.bcYSizeMaxEnabled);		
		SwapAlgParam(m_AlgParamBlobCount.bcXSizeMinEnabled, m_AlgParamBlobCount.bcYSizeMinEnabled);
		break;
	}
}
//-------------------------------------------------------------------------------------//
void CAlgParam::RotateAlgParam_BodyTilt(int AngleLabel)
{	
	switch ( AngleLabel )
	{
	case 0:
		break;
	case 90:
	case 270:
		SwapAlgParam(m_AlgParamBodyTilt.btSizeRatioX, m_AlgParamBodyTilt.btSizeRatioY);
		switch ( m_AlgParamBodyTilt.btCellMode  ) 
		{
		case ALG_TILE_CELL_HOR: m_AlgParamBodyTilt.btCellMode = ALG_TILE_CELL_VER; break;
		case ALG_TILE_CELL_VER: m_AlgParamBodyTilt.btCellMode = ALG_TILE_CELL_HOR; break;
		}		
		break;
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgParam::RotateAlgParam_ImageMatch(int AngleLabel)
{	
	switch ( AngleLabel )
	{
	case 0:
		break;
	case 90:
	case 270:
		SwapAlgParam(m_AlgParamImageMatch.imPxlCmpXSizeMin, m_AlgParamImageMatch.imPxlCmpYSizeMin);		
		break;
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgParam::RotateAlgParam_ObjectMeasure(int AngleLabel)
{
	switch ( AngleLabel )
	{
	case 0:
		break;
	case 90:
	case 270:
		SwapAlgParam(m_AlgParamObjectMeasure.omSizeXSpec, m_AlgParamObjectMeasure.omSizeYSpec);
		SwapAlgParam(m_AlgParamObjectMeasure.omSizeXDiffUSL, m_AlgParamObjectMeasure.omSizeYDiffUSL);
		SwapAlgParam(m_AlgParamObjectMeasure.omSizeXDiffLSL, m_AlgParamObjectMeasure.omSizeYDiffLSL);
		SwapAlgParam(m_AlgParamObjectMeasure.omSizeXRatioUSL, m_AlgParamObjectMeasure.omSizeYRatioUSL);
		SwapAlgParam(m_AlgParamObjectMeasure.omSizeXRatioLSL, m_AlgParamObjectMeasure.omSizeYRatioLSL);
		SwapAlgParam(m_AlgParamObjectMeasure.omSizeXEnabled, m_AlgParamObjectMeasure.omSizeYEnabled);
		SwapAlgParam(m_AlgParamObjectMeasure.omSizeXReading, m_AlgParamObjectMeasure.omSizeYReading);		
		break;
	}
	return;
}
//-------------------------------------------------------------------------------------//
CString CAlgParam::GetAlgParamPlugInFilename(LPCTSTR AlgName) const//取得演算法外掛檔名
{
	CString Filename;
	CAOIWnd *WndPtr = GetAlgWndPtr();
	if ( NULL == WndPtr ) { return Filename; }
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr ) { return Filename; }
	
	unsigned int WndIdx = WndPtr->GetWndIndex();	
	CString ModelFolder = ModelPtr->GetModelFolder();
	Filename.Format(_T("%s\\Wnd%06d_%s.TXT"), ModelFolder, WndIdx+1, AlgName);
	return Filename;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CopyAlgParam_BinyParam(const CAlgParam &AlgParam)
{
	m_AlgMaskBinParam=AlgParam.m_AlgMaskBinParam;
	m_AlgImageBinParam=AlgParam.m_AlgImageBinParam;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CopyAlgParam_GenParam(const CAlgParam &AlgParam, bool bKeepSpec)
{
	m_AlgType = AlgParam.m_AlgType;
	
	m_AlgMaskBinParam = AlgParam.m_AlgMaskBinParam;
	m_AlgImageBinParam = AlgParam.m_AlgImageBinParam;

	m_AlgPatternSimilarityUSL = AlgParam.m_AlgPatternSimilarityUSL;
	m_AlgPatternSimilarityLSL = AlgParam.m_AlgPatternSimilarityLSL;
	m_AlgPatternAngleExpand = AlgParam.m_AlgPatternAngleExpand;
	m_AlgPatternScaleExpand = AlgParam.m_AlgPatternScaleExpand;
	m_AlgPatternScaleIsotropic = AlgParam.m_AlgPatternScaleIsotropic;

	m_AlgPatternMinReducedArea = AlgParam.m_AlgPatternMinReducedArea;
	m_AlgPatternFinalReduction = AlgParam.m_AlgPatternFinalReduction;
	m_AlgPatternAdvancedLearning = AlgParam.m_AlgPatternAdvancedLearning;

	double brTargetValue=m_AlgParamBrightRatio.brTargetValue;
	double omSizeXSpec=m_AlgParamObjectMeasure.omSizeXSpec;
	double omSizeYSpec=m_AlgParamObjectMeasure.omSizeYSpec;
	double omHeightSpec=m_AlgParamObjectMeasure.omHeightSpec;
	double omAreaSpec=m_AlgParamObjectMeasure.omAreaSpec;
	double omVolumeSpec=m_AlgParamObjectMeasure.omVolumeSpec;
	double amAngleSpec=m_AlgParamAngleMeasure.amAngleSpec;	

	m_AlgParamBrightRatio = AlgParam.m_AlgParamBrightRatio;
	m_AlgParamOuterShort = AlgParam.m_AlgParamOuterShort;
	m_AlgParamBlobCount = AlgParam.m_AlgParamBlobCount;
	m_AlgParamBodyTilt = AlgParam.m_AlgParamBodyTilt;
	m_AlgParamModelMatch = AlgParam.m_AlgParamModelMatch;
	m_AlgParamImageMatch = AlgParam.m_AlgParamImageMatch;
	m_AlgParamCharVerify = AlgParam.m_AlgParamCharVerify;
	m_AlgParamGroupCompare = AlgParam.m_AlgParamGroupCompare;
	m_AlgParamBarcodeRecognize = AlgParam.m_AlgParamBarcodeRecognize;
	m_AlgParamObjectMeasure = AlgParam.m_AlgParamObjectMeasure;
	m_AlgParamColorCode = AlgParam.m_AlgParamColorCode;
	m_AlgParamFdMatch = AlgParam.m_AlgParamFdMatch;
	m_AlgParamEdgeSearch = AlgParam.m_AlgParamEdgeSearch;
	m_AlgParamShapeVerify = AlgParam.m_AlgParamShapeVerify;
	m_AlgParamAngleMeasure = AlgParam.m_AlgParamAngleMeasure;
	m_AlgParamPixelCompare = AlgParam.m_AlgParamPixelCompare;
	m_AlgParamIPC = AlgParam.m_AlgParamIPC;
	m_AlgParamResinHight = AlgParam.m_AlgParamResinHight;
	m_AlgParamWireWidth = AlgParam.m_AlgParamWireWidth;
	m_AlgParamAiModel = AlgParam.m_AlgParamAiModel;		
	m_AlgParamSolderWetting = AlgParam.m_AlgParamSolderWetting;
	m_AlgParamMeasureBlackGlue = AlgParam.m_AlgParamMeasureBlackGlue;	
	m_AlgParamMeasureFluxArea = AlgParam.m_AlgParamMeasureFluxArea;		
	m_AlgParamMeasureCpuPin = AlgParam.m_AlgParamMeasureCpuPin;		

	if ( true == bKeepSpec )//保持標準值
	{
		m_AlgParamBrightRatio.brTargetValue = brTargetValue;

		m_AlgParamObjectMeasure.omSizeXSpec = omSizeXSpec;
		m_AlgParamObjectMeasure.omSizeYSpec = omSizeYSpec;
		m_AlgParamObjectMeasure.omHeightSpec = omHeightSpec;
		m_AlgParamObjectMeasure.omAreaSpec = omAreaSpec;
		m_AlgParamObjectMeasure.omVolumeSpec = omVolumeSpec;

		m_AlgParamAngleMeasure.amAngleSpec =amAngleSpec;		
	}

	m_AlgOffsetXEnabled = AlgParam.m_AlgOffsetXEnabled;
	m_AlgOffsetXUSL = AlgParam.m_AlgOffsetXUSL;
	m_AlgOffsetXLSL = AlgParam.m_AlgOffsetXLSL;

	m_AlgOffsetYEnabled = AlgParam.m_AlgOffsetYEnabled;	
	m_AlgOffsetYUSL = AlgParam.m_AlgOffsetYUSL;
	m_AlgOffsetYLSL = AlgParam.m_AlgOffsetYLSL;

	m_AlgOffsetLEnabled = AlgParam.m_AlgOffsetLEnabled;	
	m_AlgOffsetLUSL = AlgParam.m_AlgOffsetLUSL;
	m_AlgOffsetLLSL = AlgParam.m_AlgOffsetLLSL;

	m_AlgOffsetAEnabled = AlgParam.m_AlgOffsetAEnabled;	
	m_AlgOffsetAUSL = AlgParam.m_AlgOffsetAUSL;
	m_AlgOffsetALSL = AlgParam.m_AlgOffsetALSL;

	m_AlgSkewEnabled = AlgParam.m_AlgSkewEnabled;
	m_AlgSkewUSL = AlgParam.m_AlgSkewUSL;
	m_AlgSkewLSL = AlgParam.m_AlgSkewLSL;

	m_AlgScaleEnabled = AlgParam.m_AlgScaleEnabled;
	m_AlgScaleUSL = AlgParam.m_AlgScaleUSL;
	m_AlgScaleLSL = AlgParam.m_AlgScaleLSL;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CopyAlgParamResultValue(CAlgParam &AlgParam)
{
	size_t i=0;
	SetAlgImageOffsetX(AlgParam.GetAlgImageOffsetX());
	SetAlgImageOffsetY(AlgParam.GetAlgImageOffsetY());

	SetAlgOffsetXReading(AlgParam.GetAlgOffsetXReading());
	SetAlgOffsetYReading(AlgParam.GetAlgOffsetYReading());
	SetAlgOffsetLReading(AlgParam.GetAlgOffsetLReading());
	SetAlgOffsetAReading(AlgParam.GetAlgOffsetAReading());
	SetAlgSkewReading(AlgParam.GetAlgSkewReading());
	
	SetAlgResultReading1(AlgParam.GetAlgResultReading1());
	SetAlgResultReading2(AlgParam.GetAlgResultReading2());
	SetAlgResultReading3(AlgParam.GetAlgResultReading3());
	SetAlgResultText(AlgParam.GetAlgResultText());
	
	SetAlgBaseValueReading(AlgParam.GetAlgBaseValueReading());	

	GetAlgParamBrightRatio().brRatioArea = AlgParam.GetAlgParamBrightRatio().brRatioArea;
	GetAlgParamBrightRatio().brRatioReading = AlgParam.GetAlgParamBrightRatio().brRatioReading;
	GetAlgParamBrightRatio().brAverageReading = AlgParam.GetAlgParamBrightRatio().brAverageReading;
	GetAlgParamBrightRatio().brAverageReadingMin = AlgParam.GetAlgParamBrightRatio().brAverageReadingMin;
	GetAlgParamBrightRatio().brAverageReadingMax = AlgParam.GetAlgParamBrightRatio().brAverageReadingMax;
	GetAlgParamBrightRatio().brToleranceReading = AlgParam.GetAlgParamBrightRatio().brToleranceReading;
	GetAlgParamBrightRatio().brLimitReadingMin = AlgParam.GetAlgParamBrightRatio().brLimitReadingMin;
	GetAlgParamBrightRatio().brLimitReadingMax = AlgParam.GetAlgParamBrightRatio().brLimitReadingMax;
	GetAlgParamBrightRatio().brRangeReading = AlgParam.GetAlgParamBrightRatio().brRangeReading;
	GetAlgParamBrightRatio().brContrastReading = AlgParam.GetAlgParamBrightRatio().brContrastReading;
	
	GetAlgParamOuterShort().osLine_L.olReading = AlgParam.GetAlgParamOuterShort().osLine_L.olReading;
	GetAlgParamOuterShort().osLine_T.olReading = AlgParam.GetAlgParamOuterShort().osLine_T.olReading;
	GetAlgParamOuterShort().osLine_R.olReading = AlgParam.GetAlgParamOuterShort().osLine_R.olReading;
	GetAlgParamOuterShort().osLine_B.olReading = AlgParam.GetAlgParamOuterShort().osLine_B.olReading;

	GetAlgParamBlobCount().bcCountNum = AlgParam.GetAlgParamBlobCount().bcCountNum;

	GetAlgParamBarcodeRecognize().brCodeCountReading = AlgParam.GetAlgParamBarcodeRecognize().brCodeCountReading;
	GetAlgParamBarcodeRecognize().brVerifyReading = AlgParam.GetAlgParamBarcodeRecognize().brVerifyReading;
	GetAlgParamBarcodeRecognize().brBarcodeResult = AlgParam.GetAlgParamBarcodeRecognize().brBarcodeResult;

	for ( i=0; i<MAX_BARCODE_DECODE_COUNT; i++ )
	{	GetAlgParamBarcodeRecognize().brDecodeStep[i].Decoded = AlgParam.GetAlgParamBarcodeRecognize().brDecodeStep[i].Decoded;	}
	
	GetAlgParamObjectMeasure().omHeightReading = AlgParam.GetAlgParamObjectMeasure().omHeightReading;
	GetAlgParamObjectMeasure().omAreaReading = AlgParam.GetAlgParamObjectMeasure().omAreaReading;
	GetAlgParamObjectMeasure().omVolumeReading = AlgParam.GetAlgParamObjectMeasure().omVolumeReading;	
	
	GetAlgParamCharVerify().cvPassRatioReading = AlgParam.GetAlgParamCharVerify().cvPassRatioReading;	

	GetAlgParamColorCode().ccPassRatioReading = AlgParam.GetAlgParamColorCode().ccPassRatioReading;	

	GetAlgParamShapeVerify().svReadingRInner = AlgParam.GetAlgParamShapeVerify().svReadingRInner;	
	GetAlgParamShapeVerify().svReadingROuter = AlgParam.GetAlgParamShapeVerify().svReadingROuter;	
	GetAlgParamShapeVerify().svReadingRAverage = AlgParam.GetAlgParamShapeVerify().svReadingRAverage;

	GetAlgParamAngleMeasure().amHeightA = AlgParam.GetAlgParamAngleMeasure().amHeightA;
	GetAlgParamAngleMeasure().amHeightB = AlgParam.GetAlgParamAngleMeasure().amHeightB;
	GetAlgParamAngleMeasure().amAngleReading = AlgParam.GetAlgParamAngleMeasure().amAngleReading;
	GetAlgParamAngleMeasure().amLineResultPtA_1 = AlgParam.GetAlgParamAngleMeasure().amLineResultPtA_1;
	GetAlgParamAngleMeasure().amLineResultPtB_1 = AlgParam.GetAlgParamAngleMeasure().amLineResultPtB_1;
	GetAlgParamAngleMeasure().amLineResultPtA_2 = AlgParam.GetAlgParamAngleMeasure().amLineResultPtA_2;
	GetAlgParamAngleMeasure().amLineResultPtB_2 = AlgParam.GetAlgParamAngleMeasure().amLineResultPtB_2;

	GetAlgParamPixelCompare().pcBlobCountNum = AlgParam.GetAlgParamPixelCompare().pcBlobCountNum;

	GetAlgParamIPC().ipcResult = AlgParam.m_AlgParamIPC.ipcResult;
	GetAlgParamResinHeight().resultH = AlgParam.m_AlgParamResinHight.resultH;
	GetAlgParamWireWidth().fWidth = AlgParam.m_AlgParamWireWidth.fWidth;

	GetAlgParamSolderWetting().swReadingCircleAngle = AlgParam.GetAlgParamSolderWetting().swReadingCircleAngle;	
	GetAlgParamSolderWetting().swCircleAngleCpX = AlgParam.GetAlgParamSolderWetting().swCircleAngleCpX;	
	GetAlgParamSolderWetting().swCircleAngleCpY = AlgParam.GetAlgParamSolderWetting().swCircleAngleCpY;	
	GetAlgParamSolderWetting().swCircleAngleRadius = AlgParam.GetAlgParamSolderWetting().swCircleAngleRadius;		
	GetAlgParamSolderWetting().swCircleAngleRadiusStart = AlgParam.GetAlgParamSolderWetting().swCircleAngleRadiusStart;	
	GetAlgParamSolderWetting().swResultCircleAngleRadius = AlgParam.GetAlgParamSolderWetting().swResultCircleAngleRadius;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CopyAlgParamIsolatedValue(CAlgParam &AlgParam)//複製演算法獨立值
{
	SetAlgBaseValueEnabled(AlgParam.GetAlgBaseValueEnabled());
	SetAlgBaseValueGroupID(AlgParam.GetAlgBaseValueGroupID());
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::InitAlgInspection(bool bModelInit)//初始化演算法檢測
{		
	size_t            i=0, j=0;	
	size_t            RoiRectCount=0;	
	CPatternParam    *PatParamPtr=NULL;
	CAOIWnd          *WndPtr = GetAlgWndPtr();
	BOX_TOWARD        WndToward=BOX_TOWARD_NULL;	
	const size_t PatParamCount = GetAlgPatternParamCount();

	SetAlgPatternTestAll(false);
	SetAlgPatternResultIndex(-1);	

	if ( NULL != WndPtr )
	{	WndToward = WndPtr->GetWndToward();	}
	for ( i=0; i<PatParamCount; i++ )
	{
		PatParamPtr = CAlgParam::GetAlgPatternParamPtr(i, false);
		if ( NULL == PatParamPtr ) { continue; }
		PatParamPtr->ResetResultValue();
	}
	
	SetAlgSaveDefectImageDone(false);
	SetAlgSaveDefectImageName(_T(""));
	SetAlgImageOffsetX(0.0);//演算法-影像偏移X 讀值
	SetAlgImageOffsetY(0.0);//演算法-影像偏移Y 讀值
	SetAlgOffsetXReading(0.0);//演算法-偏移X 讀值
	SetAlgOffsetYReading(0.0);//演算法-偏移Y 讀值
	SetAlgOffsetLReading(0.0);
	SetAlgOffsetAReading(0.0);//演算法-偏移角度 讀值
	SetAlgSkewReading(0.0);//演算法-角度 讀值	
	SetAlgScaleXReading(100.0);//演算法-縮放X 讀值
	SetAlgScaleYReading(100.0);//演算法-縮放Y 讀值

	SetAlgResultID(RESULT_ID_NONE);//演算法-結果
	SetAlgResultReading1(0.0);//演算法-結果數據-1
	SetAlgResultReading2(0.0);//演算法-結果數據-2
	SetAlgResultReading3(0.0);//演算法-結果數據-3
	SetAlgResultText(_T(""));//演算法-結果文字		

	SetAlgBaseValueReading(0.0);

	//演算法-遮罩二值化參數-輔助二值化
	m_AlgMaskBinParam.SetBinaryColorActivePtr(NULL);

	//演算法-影像二值化參數-主要影像
	m_AlgImageBinParam.SetBinaryColorActivePtr(NULL);

	 //演算法-亮度比例參數
	m_AlgParamBrightRatio.brRatioArea = 0.0;
	m_AlgParamBrightRatio.brRatioReading = 0.0;		
	m_AlgParamBrightRatio.brAverageReading = 0.0;
	m_AlgParamBrightRatio.brAverageReadingMin = 0.0;
	m_AlgParamBrightRatio.brAverageReadingMax = 0.0;
	m_AlgParamBrightRatio.brToleranceReading = 0.0;
	m_AlgParamBrightRatio.brLimitReadingMin = 0.0;
	m_AlgParamBrightRatio.brLimitReadingMax = 0.0;
	m_AlgParamBrightRatio.brRangeReading = 0.0;	
	m_AlgParamBrightRatio.brContrastReading = 0.0;
	m_AlgParamBrightRatio.brXLineReading = 0.0;
	m_AlgParamBrightRatio.brYLineReading = 0.0;
	
	//演算法-外接短路參數
	m_AlgParamOuterShort.osLine_R.olReading = 0.0;
	m_AlgParamOuterShort.osLine_T.olReading = 0.0;
	m_AlgParamOuterShort.osLine_L.olReading = 0.0;
	m_AlgParamOuterShort.osLine_B.olReading = 0.0;

	//演算法-區塊數量參數
	m_AlgParamBlobCount.bcCountNum = 0;          

	//演算法-本體傾斜參數
	m_AlgParamBodyTilt.btTiltGapReading = 0.0;
	m_AlgParamBodyTilt.btTiltAngleReading = 0.0;

	//演算法-模板匹配參數
	
	//演算法-影像匹配參數
	m_AlgParamImageMatch.imPxlCmpCountNum = 0;
	
	//演算法-文字驗證參數	
	m_AlgParamCharVerify.cvPassRatioReading = 0.0;//通過比例讀值	

	//演算法-群組比較		
	m_AlgParamGroupCompare.gc2DGrayValue = 0.0;   //演算法-群組比較-2D讀值	
	m_AlgParamGroupCompare.gc3DHeightValue = 0.0;   //演算法-群組比較-3D讀值		
	if ( true == bModelInit ) 
	{		
		m_AlgParamGroupCompare.gc2DGrayReading = 0.0;   //演算法-群組比較-2D讀值
		m_AlgParamGroupCompare.gc3DHeightBase = 0.0;
		m_AlgParamGroupCompare.gc3DHeightReading = 0.0;   //演算法-群組比較-3D讀值
		m_AlgParamGroupCompare.gcTiltAngleReading = 0.0;//演算法-群組比較-傾斜角度讀值		
	}
	//群組比較
	m_AlgParamGroupCompare.gcResultID = RESULT_ID_NONE;		

	//演算法-條碼辨識
	m_AlgParamBarcodeRecognize.brVerifyReading = 0;
	m_AlgParamBarcodeRecognize.brCodeCountReading = 0;
	m_AlgParamBarcodeRecognize.brBarcodeResult = L"";
	for ( i=0; i<MAX_BARCODE_DECODE_COUNT; i++ )
	{	m_AlgParamBarcodeRecognize.brDecodeStep[i].Decoded = false;	}
	
	//演算法-物體量測
	m_AlgParamObjectMeasure.omHeightReading = 0.0;
	m_AlgParamObjectMeasure.omAreaReading = 0.0;
	m_AlgParamObjectMeasure.omVolumeReading = 0.0;

	//演算法-色碼檢測
	m_AlgParamColorCode.ccPassRatioReading = 0.0;

	//外形驗證	
	m_AlgParamShapeVerify.svReadingRInner = 0.0;
	m_AlgParamShapeVerify.svReadingROuter = 0.0;
	m_AlgParamShapeVerify.svReadingRAverage = 0.0;	

	//邊緣搜尋
	m_AlgParamEdgeSearch.esResultID_F = RESULT_ID_NONE;
	m_AlgParamEdgeSearch.esResultID_B = RESULT_ID_NONE;

	//角度量測
	m_AlgParamAngleMeasure.amHeightA = 0.0;
	m_AlgParamAngleMeasure.amHeightB = 0.0;
	m_AlgParamAngleMeasure.amAngleReading = 0.0;
	m_AlgParamAngleMeasure.amLineResultPtA_1 = TPOINT2D();
	m_AlgParamAngleMeasure.amLineResultPtB_1 = TPOINT2D();
	m_AlgParamAngleMeasure.amLineResultPtA_2 = TPOINT2D();
	m_AlgParamAngleMeasure.amLineResultPtB_2 = TPOINT2D();

	//像素比較
	m_AlgParamPixelCompare.pcBlobCountNum = 0;	

	//演算法-AI模型
	m_AlgParamAiModel.aiResultID = AI_RESULT_ID_NONE;
	m_AlgParamAiModel.aiConfidence = 0.0f;
	m_AlgParamAiModel.aiResultText = L"";
	m_AlgParamAiModel.aiOcrCharList.clear();
	::SetRectEmpty(&m_AlgParamAiModel.aiResultRect);	

	//演算法-焊接檢測
	m_AlgParamSolderWetting.swReadingCircleAngle = 0.0f;		
	m_AlgParamSolderWetting.swCircleAngleCpX = 0.0f;
	m_AlgParamSolderWetting.swCircleAngleCpY = 0.0f;
	m_AlgParamSolderWetting.swCircleAngleRadius = 0.0f;	
	m_AlgParamSolderWetting.swCircleAngleRadiusStart = 0.0f;
	m_AlgParamSolderWetting.swResultCircleAngleRadius.clear();

	//演算法-量測黑膠
#ifdef ALG_MEASURE_BLACK_GLUE_USE
	m_AlgParamMeasureBlackGlue.bgResult.Initial();
#endif//ALG_MEASURE_BLACK_GLUE_USE

	//演算法-量測Flux面積
#ifdef ALG_MEASURE_FLUX_AREA_USE
	m_AlgParamMeasureFluxArea.faResult.Initial();
#endif//ALG_MEASURE_FLUX_AREA_USE	

	//演算法-量測CPU接腳
#ifdef ALG_MEASURE_CPU_PIN_USE
	m_AlgParamMeasureCpuPin.cpResult.Initial();
#endif//ALG_MEASURE_CPU_PIN_USE

	//m_AlgParamMeasureSIP
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList, bool bTestWnd)
{
	/*
#ifdef _DEBUG
	return true;
#endif//_DEBUG*/

	/*
#ifdef _DEBUG
	if ( ALG_FD_MATCH == m_AlgType )
	{	return ExecAlgInspection_FdMatch(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);; }
	return true;
#endif//_DEBUG*/

	bool IsOK = true;
#ifdef TEST_FOV_SIZE_USE
	return true;
#endif//TEST_FOV_SIZE_USE

	//提前進出板測試用
	//if ( ALG_FD_MATCH != m_AlgType )
	//{	::Sleep(450);	}
	
	switch ( m_AlgType )
	{
	case ALG_BRIGHT_RATIO:	 IsOK = ExecAlgInspection_BrightRatio(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;	
	case ALG_OUTER_SHORT:    IsOK = ExecAlgInspection_OuterShort(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_BLOB_COUNT:	 IsOK = ExecAlgInspection_BlobCount(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_BODY_TILT:      IsOK = ExecAlgInspection_BodyTilt(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_BARCODE_RECOGNIZE: IsOK = ExecAlgInspection_BarcodeRecognize(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_MODEL_MATCH:     IsOK = ExecAlgInspection_ModelMatch(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_IMAGE_MATCH:	 IsOK = ExecAlgInspection_ImageMatch(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_CHAR_VERIFY:	 IsOK = ExecAlgInspection_CharVerify(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_OBJECT_MEASURE: IsOK = ExecAlgInspection_ObjectMeasure(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_FD_MATCH:       IsOK = ExecAlgInspection_FdMatch(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_COLOR_CODE:     IsOK = ExecAlgInspection_ColorCode(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_EDGE_SEARCH:    IsOK = ExecAlgInspection_EdgeSearch(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_SHAPE_VERIFY:   IsOK = ExecAlgInspection_ShapeVerify(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_ANGLE_MEASURE:  IsOK = ExecAlgInspection_AngleMeasure(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_WIDTH_RATIO:	 IsOK = ExecAlgInspection_WidthRatio(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_PIXEL_COMPARE:  IsOK = ExecAlgInspection_PixelCompare(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_HEIGHT:		 IsOK = ExecAlgInspection_Height(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_WIRE_WIDTH:     IsOK = ExecAlgInspection_WireWidth(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_SOLDER_WETTING: IsOK = ExecAlgInspection_SolderWetting(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList, bTestWnd);	break;
	case ALG_MEASURE_BLACK_GLUE: IsOK = ExecAlgInspection_MeasureBlackGlue(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList, bTestWnd); break;
	case ALG_MEASURE_FLUX_AREA: IsOK = ExecAlgInspection_MeasureFluxArea(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList, bTestWnd); break;
	case ALG_MEASURE_CPU_PIN: IsOK = ExecAlgInspection_MeasureCpuPin(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList, bTestWnd); break;	
	case ALG_MEASURE_SIP_DISTANCE: IsOK = ExecAlgInspection_MeasureSIP(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	case ALG_MEASURE_CONNECTOR: //共用ExecAlgInspection_MeasureConnector
	case ALG_MEASURE_CONNECTOR_PIN:	IsOK = ExecAlgInspection_MeasureConnector(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);	break;
	}	
	if ( false == IsOK ) { return false; }
	IsOK = ExecAlgInspection_GroupCompare(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
double CAlgParam::CalcRotateAngle(const std::vector<TPOINT4D> &PosList, double AngleRange)//計算旋轉角度
{
	double RotatedAngle = 0.0;
	const size_t Count = PosList.size();
	if ( Count < 2 ) { return RotatedAngle; }

	int    i=0;
	size_t j=0;
	int    MinI=0;
	double ErrorCheck=0.0;
	double Error=0.0, MinError=0.0, MaxError=0.0;
	const double Step=0.1;//0.1度
	const int nAngleRange = (int)(AngleRange/Step);
	const int nAngleCount = nAngleRange*2;
	double COS=0.0, SIN=0.0;
	double PosCX=0, PosCY=0;
	double ResCX=0, ResCY=0;
	double DiffX=0, DiffY=0, DiffL=0;
	double TempX=0, TempY=0;
	double TempX2=0, TempY2=0;	
	for ( j=0; j<Count; j++ )
	{
		PosCX += PosList[j].x;
		PosCY += PosList[j].y;
		ResCX += PosList[j].u;
		ResCY += PosList[j].v;
	}
	PosCX /= Count;
	PosCY /= Count;
	ResCX /= Count;
	ResCY /= Count;
	const double OffsetX=ResCX-PosCX;
	const double OffsetY=ResCY-PosCY;

	MinError =  DBL_MAX;
	MaxError = -DBL_MAX;
	RotatedAngle = 0.0;
	for ( i=0; i<nAngleCount; i++ )
	{
		if ( 0 == i%2 )
		{	RotatedAngle = 0+(i/2)*Step;	}
		else
		{	RotatedAngle = 0-((i+1)/2)*Step;	}		
		RotatedAngle = RotatedAngle*DEG_TO_RAD_DBL;
		COS = ::cos(RotatedAngle);
		SIN = ::sin(RotatedAngle);

		Error = 0.0;
		for ( j=0; j<Count; j++ )
		{
			TempX = PosList[j].x-PosCX;
			TempY = PosList[j].y-PosCY;
			TempX2 = (TempX*COS)-(TempY*SIN);
			TempY2 = (TempX*SIN)+(TempY*COS);
			TempX2 += (PosCX+OffsetX);
			TempY2 += (PosCY+OffsetY);
			DiffX = TempX2-PosList[j].u;
			DiffY = TempY2-PosList[j].v;
			DiffL = sqrt((DiffX*DiffX)+(DiffY*DiffY));
			Error += DiffL;
		}
		//ErrorCheck = MAX(MinError+10, 2*MinError);
		//if ( Error > ErrorCheck )
		//{	break; }
		
		if ( Error < MinError )
		{
			MinI = i;
			MinError = Error;
		}
		if ( Error > MaxError )
		{	MaxError = Error; }
	}	
	if ( 0 == MinI%2 )
	{	RotatedAngle = 0+(MinI/2)*Step;	}
	else
	{	RotatedAngle = 0-((MinI+1)/2)*Step;	}
	return RotatedAngle;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::MemsetByRect(int ImageW, int ImageH, int ImageStep, const RECT &CalcRect, const RECT &MaskRect, unsigned char val, unsigned char *pImage)
{
	RECT RoiRect;	
	RoiRect.left=MAX(0, MaskRect.left);
	RoiRect.top=MAX(0, MaskRect.top);
	RoiRect.right=MIN(ImageW, MaskRect.right+1);
	RoiRect.bottom=MIN(ImageH, MaskRect.bottom+1);
	return ImageAPI.SetGrayImage(ImageW, ImageH, ImageStep, pImage, RoiRect, val);	
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::BuildWndUniFrameList(const std::vector<TUNI_FRAME> &ModelUniFrameList, const RECT &RoiRect, std::vector<TUNI_FRAME> &WndUniFrameList)
{
	size_t i=0;
	const int nAlign = 4;
	const IMAGE_SIZE MaskBitCount = 8;
	const IMAGE_SIZE RoiW = RoiRect.right-RoiRect.left;
	const IMAGE_SIZE RoiH = RoiRect.bottom-RoiRect.top;
	const size_t FrameCount=ModelUniFrameList.size();
	for ( i=0; i<FrameCount; i++ )
	{	
		IMAGE_SIZE    MaskStep=0;
		IMAGE_SIZE    WndImageW=0;
		IMAGE_SIZE    WndImageH=0;	
		IMAGE_SIZE    WndImageStep=0;
		IMAGE_SIZE    WndImageBitCount=0;	
		MASK_PTR      WndMaskPtr   = NULL;
		SPACE_PTR     WndSpacePtr  = NULL;
		IMAGE_PTR     WndImagePtr = NULL;
		
		TUNI_FRAME  TempUniFrame  = ModelUniFrameList[i];
		IMAGE_SIZE  TempFrameW    = TempUniFrame.ImageW;
		IMAGE_SIZE  TempFrameH    = TempUniFrame.ImageH;
		IMAGE_SIZE  TempFrameStep = TempUniFrame.ImageStep;
		IMAGE_SIZE  TempBitCount  = TempUniFrame.BitCount;
		IMAGE_PTR   TempImagePtr  = TempUniFrame.ImagePtr;
		MASK_PTR    TempMaskPtr   = TempUniFrame.MaskPtr;
		SPACE_PTR   TempSpacePtr  = TempUniFrame.SpacePtr;

		WndImageW = RoiW;
		WndImageH = RoiH;
		MaskStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, MaskBitCount, nAlign);		
		WndImageStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, TempBitCount, nAlign);
		if ( NULL != TempMaskPtr )
		{	
			if ( ImageAPI.ExtractGrayRoiImage(TempFrameW, TempFrameH, TempFrameStep, TempMaskPtr, RoiRect, MaskStep, WndMaskPtr, false) == false )
			{
				JetMemory.free_func(WndMaskPtr);
				JetMemory.free_func(WndSpacePtr);
				JetMemory.free_func(WndImagePtr);
				JetAPI::ClearUniFrameList(WndUniFrameList);
				return false;	
			}
		}	
		if ( NULL != TempSpacePtr )
		{	
			if ( ImageAPI.ExtractSpaceGrayRoiImage(TempFrameW, TempFrameH, TempFrameStep, TempSpacePtr, RoiRect, MaskStep, WndSpacePtr, false) == false )
			{
				JetMemory.free_func(WndMaskPtr);
				JetMemory.free_func(WndSpacePtr);
				JetMemory.free_func(WndImagePtr);
				JetAPI::ClearUniFrameList(WndUniFrameList);
				return false;	
			}
		}	
		if ( NULL != TempImagePtr )
		{
			if ( ImageAPI.ExtractRoiImage(TempFrameW, TempFrameH, TempFrameStep, TempBitCount, TempImagePtr, RoiRect, WndImageStep, WndImagePtr, false) == false )
			{
				JetMemory.free_func(WndMaskPtr);
				JetMemory.free_func(WndSpacePtr);
				JetMemory.free_func(WndImagePtr);
				JetAPI::ClearUniFrameList(WndUniFrameList);
				return false;	
			}		
		}

		TUNI_FRAME  WndUniFrame;
		WndUniFrame.ImageW = WndImageW;
		WndUniFrame.ImageH = WndImageH;
		WndUniFrame.ImageStep = JetAPI::GetBMPImagePixelsPerLine(WndUniFrame.ImageW, TempBitCount, nAlign);
		WndUniFrame.BitCount = TempBitCount;
		WndUniFrame.MaskPtr = WndMaskPtr;
		WndUniFrame.ImagePtr = WndImagePtr;
		WndUniFrame.SpacePtr = WndSpacePtr;
		WndUniFrameList.push_back(WndUniFrame);

		WndMaskPtr = NULL;
		WndImagePtr = NULL;
		WndSpacePtr = NULL;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::BuildAlgGrayImage(const CAlgBinaryParam &Param, const RECT &RoiRect, std::vector<TUNI_FRAME> &UniFrameList, IMAGE_SIZE &GrayW, IMAGE_SIZE &GrayH, IMAGE_SIZE &GrayStep, IMAGE_PTR &GrayPtr, bool &bCloned)//建立灰階影像
{
	const char fnName[] = "CAlgParam::BuildAlgGrayImage";
	const size_t FrameCount = UniFrameList.size();		
	const size_t FrameIndex = Param.GetBinaryFrameIndex();
	if ( FrameIndex >= FrameCount ) 
	{	return false; }

	TUNI_FRAME      *UniFramePtr  = &(UniFrameList[FrameIndex]);
	const IMAGE_SIZE ImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE ImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE ImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        FrameImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         FrameMaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        FrameSpacePtr  = UniFramePtr->SpacePtr;

	GrayW = ImageW;
	GrayH = ImageH;
	const IMAGE_SIZE GrayBitCount = 8;
	GrayStep = JetAPI::GetBMPImagePixelsPerLine(GrayW, GrayBitCount, 4);
	const size_t     BufferSize = ImageAPI.CalcBufferSize(GrayStep, GrayH);
	//const double     BaseHeight = AOIDataCollect.GetSpaceBaseHeight();
	const double     BaseHeight = 0;
	const double     SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
	
	if ( 8 == FrameBitCount )//Gray Image
	{
		GrayStep = ImageStep;
		GrayPtr = FrameImagePtr;
	}
	else
	{
		JetMemory.free_func(GrayPtr);
		if ( JetMemory.alloc_func(BufferSize, GrayPtr, fnName, "GrayPtr") == false )
		{	return false;	}
		bCloned = true;

		if ( NULL!=FrameSpacePtr &&  NULL!=FrameMaskPtr )//Space Value
		{	
			if ( ImageAPI.SpaceGrayImageConvertToGray3(ImageW, ImageH, ImageStep, FrameSpacePtr, FrameMaskPtr, RoiRect, GrayStep, GrayPtr, SpaceRatio, false) == false )
			{
				JetMemory.free_func(GrayPtr);			
				return false;
			}
		}
		else if ( 24 == FrameBitCount )//Color Image
		{			
			if ( ImageAPI.ColorImageToGrayImage3(ImageW, ImageH, ImageStep, FrameImagePtr, RoiRect, GrayStep, GrayPtr, 
				Param.GetBinaryImageSourceMode(), 
				Param.GetBinarySynthesisWR(), 
				Param.GetBinarySynthesisWG(), 
				Param.GetBinarySynthesisWB(), 
				false) == false )
			{
				JetMemory.free_func(GrayPtr);			
				return false;
			}			
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgUniFrameBinary_Rect(CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, TUNI_FRAME UniFrame, int nAlign, MASK_PTR &MaskPtr)
{
	return ExecAlgImageBinary_Rect(Param, CalcRect, MaskRect, UniFrame.ImageW, UniFrame.ImageH, UniFrame.ImageStep, UniFrame.BitCount, UniFrame.ImagePtr, UniFrame.MaskPtr, UniFrame.SpacePtr, nAlign, MaskPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgImageGray_Rect(CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR FrameImagePtr, MASK_PTR FrameMaskPtr, SPACE_PTR FrameSpacePtr, int nAlign, IMAGE_PTR &GrayPtr)
{
	MASK_PTR MaskPtr = NULL;
	BINARY_MODE OldBinaryMode=Param.GetBinaryMode();
	Param.SetBinaryMode(BINARY_DISABLE);	
	if ( ExecAlgImageBinary_Loc(Param, CalcRect, MaskRect, ImageW, ImageH, ImageStep, BitCount, FrameImagePtr, FrameMaskPtr, FrameSpacePtr, nAlign, GrayPtr, MaskPtr) == false )
	{
		Param.SetBinaryMode(OldBinaryMode);
		return false; 
	}	
	JetMemory.free_func(MaskPtr);
	Param.SetBinaryMode(OldBinaryMode);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgImageBinary_Rect(CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR FrameImagePtr, MASK_PTR FrameMaskPtr, SPACE_PTR FrameSpacePtr, int nAlign, MASK_PTR &MaskPtr)
{
	IMAGE_PTR GrayPtr = NULL;	
	if ( ExecAlgImageBinary_Loc(Param, CalcRect, MaskRect, ImageW, ImageH, ImageStep, BitCount, FrameImagePtr, FrameMaskPtr, FrameSpacePtr, nAlign, GrayPtr, MaskPtr) == false )
	{	return false; }
	JetMemory.free_func(GrayPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgGrayEdge_Rect(CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR ImagePtr)
{
	const char fnName[] = "CAlgParam::ExecAlgGrayEdge_Rect";
	EDGE_ENHANCE_MODE EdgeEnhanceMode = Param.GetEdgeEnhanceMode();	
	if ( EDGE_ENHANCE_DISABLE == EdgeEnhanceMode ) { return true; }
	
	IMAGE_PTR PtrTmp = NULL;
	IMAGE_PTR PtrSrc = NULL;
	IMAGE_PTR PtrDst = NULL;
	IMAGE_PTR PtrSwap = NULL;
	IMAGE_PTR TempPtr1 = NULL;
	IMAGE_PTR TempPtr2 = NULL;			
	std::vector<IMAGE_PTR> PtrList;

	const int nAlign = 4;
	const IMAGE_SIZE BitCount = 8;
	const unsigned int IterCount = 1;
	const int TempW = MaskRect.right-MaskRect.left;
	const int TempH = MaskRect.bottom-MaskRect.top;
	const int TempStep = JetAPI::GetBMPImagePixelsPerLine(TempW, 8, nAlign);
	const size_t TempSize = ImageAPI.CalcBufferSize(TempStep, TempH);	
	if ( TempW<0 || TempH<0 )
	{	return false; }
	if ( JetMemory.alloc_func(TempSize, TempPtr1, fnName, "TempPtr1") == false ||
		 JetMemory.alloc_func(TempSize, TempPtr2, fnName, "TempPtr2") == false )
	{	
		JetMemory.free_func(TempPtr1);
		JetMemory.free_func(TempPtr2);
		return false;
	}
	PtrList.push_back(TempPtr1);
	PtrList.push_back(TempPtr2);
	if ( ImageAPI.ExtractGrayRoiImage3(ImageW, ImageH, ImageStep, ImagePtr, MaskRect, TempStep, TempPtr1, false) == false )
	{		
		JetMemory.free_list(PtrList);
		return false;
	}
	PtrSrc = TempPtr1;
	PtrDst = TempPtr2;		
	
	int ImageEdgeMode=SOBEL_NORMAL;
	switch ( EdgeEnhanceMode )
	{
	case EDGE_ENHANCE_DARK_TOP: ImageEdgeMode=SOBEL_DARK_TOP; break;
	case EDGE_ENHANCE_DARK_LEFT: ImageEdgeMode=SOBEL_DARK_LEFT; break;
	case EDGE_ENHANCE_DARK_BOT: ImageEdgeMode=SOBEL_DARK_BOT; break;
	case EDGE_ENHANCE_DARK_RIGHT: ImageEdgeMode=SOBEL_DARK_RIGHT; break;	
	}
	if ( SOBEL_NORMAL == ImageEdgeMode )
	{
		if ( ImageAPI.SobelImage3(TempW, TempH, TempStep, BitCount, PtrSrc, PtrDst) == false )
		{
			JetMemory.free_list(PtrList);
			return false;
		}
	}
	else
	{
		if ( ImageAPI.EdgeImage3(TempW, TempH, TempStep, BitCount, PtrSrc, PtrDst, ImageEdgeMode) == false )
		{
			JetMemory.free_list(PtrList);
			return false;
		}
	}
	PtrSwap= PtrDst;	PtrDst = PtrSrc;	PtrSrc = PtrSwap;

	size_t i=0;
	std::vector<TBINARY_FILTER> FilterList;
	if ( Param.CheckUseEdgeEnhanceParam(Param.GetEdgeEnhanceFilter1()) == true )
	{	FilterList.push_back(Param.GetEdgeEnhanceFilter1());	}
	if ( Param.CheckUseEdgeEnhanceParam(Param.GetEdgeEnhanceFilter2()) == true )
	{	FilterList.push_back(Param.GetEdgeEnhanceFilter2());	}
	const size_t FilterCount=FilterList.size();

	for ( i=0; i<FilterCount; i++ )
	{
		const TBINARY_FILTER &Filter = FilterList[i];		
	
		bool IsOK = true;		
		const int  KenSize=Filter.FilterParam1;
		const NOISE_FILTER_MODE FilterMode = Filter.FilterMode;
		switch ( FilterMode )
		{
		case NOISE_FILTER_OPEN:	 IsOK=ImageAPI.MorphImage3(TempW, TempH, TempStep, BitCount, PtrSrc, MORPH_OPEN, MORPH_SHAPE_ELLIPSE, KenSize, 1, PtrDst);	break;
		case NOISE_FILTER_CLOSE: IsOK=ImageAPI.MorphImage3(TempW, TempH, TempStep, BitCount, PtrSrc, MORPH_CLOSE, MORPH_SHAPE_ELLIPSE, KenSize, 1, PtrDst);	break;
		case NOISE_FILTER_EROSION:  IsOK=ImageAPI.ErodeImage3(TempW, TempH, TempStep, BitCount, PtrSrc, KenSize, 1, PtrDst);	break;
		case NOISE_FILTER_DILATION: IsOK=ImageAPI.DilateImage3(TempW, TempH, TempStep, BitCount, PtrSrc, KenSize, 1, PtrDst);	break;		
		case NOISE_FILTER_GRADIENT: IsOK=ImageAPI.MorphImage3(TempW, TempH, TempStep, BitCount, PtrSrc, MORPH_GRADIENT, MORPH_SHAPE_ELLIPSE, KenSize, 1, PtrDst);	break;
		}	
		if ( false == IsOK )
		{
			JetMemory.free_list(PtrList);
			return false;
		}		
		PtrSwap= PtrDst;	PtrDst = PtrSrc;	PtrSrc = PtrSwap;	
	}	

	const bool theSameSize=false;
	if ( ImageAPI.PasteGrayRoiImage3(ImageW, ImageH, ImageStep, ImagePtr, MaskRect, TempStep, PtrSrc, false, theSameSize) == false )
	{	
		JetMemory.free_list(PtrList);
		return false;
	}		
	JetMemory.free_list(PtrList);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgGrayFilter_Rect(CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR ImagePtr)
{
	const char fnName[] = "CAlgParam::ExecAlgGrayFilter_Rect";

	std::vector<TBINARY_FILTER> FilterList;
	if ( Param.CheckUseGrayFilterParam(Param.GetGrayNoiseFilter1()) == true )
	{	FilterList.push_back(Param.GetGrayNoiseFilter1()); }
	if ( Param.CheckUseGrayFilterParam(Param.GetGrayNoiseFilter2()) == true )
	{	FilterList.push_back(Param.GetGrayNoiseFilter2()); }
	const size_t FilterCount=FilterList.size();
	if ( 0 == FilterCount )
	{	return true; }	
	
	IMAGE_PTR  PtrTmp = NULL;
	IMAGE_PTR  PtrSrc = NULL;
	IMAGE_PTR  PtrDst = NULL;
	IMAGE_PTR  TempPtr1 = NULL;
	IMAGE_PTR  TempPtr2 = NULL;		
	const int nAlign = 4;
	const IMAGE_SIZE BitCount = 8;
	std::vector<IMAGE_PTR> PtrList;	
	const unsigned int IterCount = 1;
	const int TempW = MaskRect.right-MaskRect.left;
	const int TempH = MaskRect.bottom-MaskRect.top;
	const int TempStep = JetAPI::GetBMPImagePixelsPerLine(TempW, 8, nAlign);
	const size_t TempSize = ImageAPI.CalcBufferSize(TempStep, TempH);
	if ( TempW<0 || TempH<0 )
	{	return false; }
	if ( JetMemory.alloc_func(TempSize, TempPtr1, fnName, "TempPtr1") == false ||
		 JetMemory.alloc_func(TempSize, TempPtr2, fnName, "TempPtr2") == false )
	{	
		JetMemory.free_func(TempPtr1);
		JetMemory.free_func(TempPtr2);
		return false;
	}
	PtrList.push_back(TempPtr1);
	PtrList.push_back(TempPtr2);
	if ( ImageAPI.ExtractGrayRoiImage3(ImageW, ImageH, ImageStep, ImagePtr, MaskRect, TempStep, TempPtr1, false) == false )
	{		
		JetMemory.free_list(PtrList);
		return false;
	}
	PtrSrc = TempPtr1;
	PtrDst = TempPtr2;
	
	for ( size_t i=0; i<FilterCount; i++ )
	{
		const TBINARY_FILTER &NoiseFilter = FilterList[i];

		bool      IsOK=true;
		const int unsigned IterCount = 1;
		const int Param1 = NoiseFilter.FilterParam1;	
		const NOISE_FILTER_MODE Mode = NoiseFilter.FilterMode;
		switch ( Mode )
		{
		case NOISE_FILTER_SMOOTH:	IsOK = ImageAPI.SmoothGrayImage3(TempW, TempH, TempStep, PtrSrc, Param1, PtrDst);	break;
		case NOISE_FILTER_MEDIAN:	IsOK = ImageAPI.MedianGrayImage3(TempW, TempH, TempStep, PtrSrc, Param1, PtrDst);	break;
		case NOISE_FILTER_OPEN:		IsOK = ImageAPI.MorphGrayImage3(TempW, TempH, TempStep, PtrSrc, MORPH_OPEN, MORPH_SHAPE_RECT, Param1, IterCount, PtrDst);	break;
		case NOISE_FILTER_CLOSE:	IsOK = ImageAPI.MorphGrayImage3(TempW, TempH, TempStep, PtrSrc, MORPH_CLOSE, MORPH_SHAPE_RECT, Param1, IterCount, PtrDst);	break;
		default:
			IsOK = false;
			break;
		}
		if ( false == IsOK )
		{	
			JetMemory.free_list(PtrList);
			return false;
		}			
		PtrTmp = PtrSrc;
		PtrSrc = PtrDst;
		PtrDst = PtrTmp;
	}
	
	const bool theSameSize=false;
	if ( ImageAPI.PasteGrayRoiImage3(ImageW, ImageH, ImageStep, ImagePtr, MaskRect, TempStep, PtrSrc, false, theSameSize) == false )
	{	
		JetMemory.free_list(PtrList);
		return false;
	}	
	JetMemory.free_list(PtrList);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgBinaryFilter_Rect(CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, MASK_PTR MaskPtr)
{
	const char fnName[] = "CAlgParam::ExecAlgBinaryFilter_Rect";

	std::vector<TBINARY_FILTER> FilterList;
	if ( Param.CheckUseBinaryFilterParam(Param.GetBinaryNoiseFilter1()) == true )
	{	FilterList.push_back(Param.GetBinaryNoiseFilter1()); }
	if ( Param.CheckUseBinaryFilterParam(Param.GetBinaryNoiseFilter2()) == true )
	{	FilterList.push_back(Param.GetBinaryNoiseFilter2()); }
	const size_t FilterCount=FilterList.size();
	if ( 0 == FilterCount )
	{	return true; }	

	MASK_PTR  PtrTmp = NULL;
	MASK_PTR  PtrSrc = NULL;
	MASK_PTR  PtrDst = NULL;
	MASK_PTR  TempPtr1 = NULL;
	MASK_PTR  TempPtr2 = NULL;		
	const int nAlign = 4;
	std::vector<MASK_PTR> PtrList;
	const int TempW = MaskRect.right-MaskRect.left;
	const int TempH = MaskRect.bottom-MaskRect.top;
	const int TempStep = JetAPI::GetBMPImagePixelsPerLine(TempW, 8, nAlign);
	const size_t TempSize = ImageAPI.CalcBufferSize(TempStep, TempH);
	if ( TempW<0 || TempH<0 )
	{	return false; }
	if ( JetMemory.alloc_func(TempSize, TempPtr1, fnName, "TempPtr1") == false ||
		 JetMemory.alloc_func(TempSize, TempPtr2, fnName, "TempPtr2") == false )
	{	
		JetMemory.free_func(TempPtr1);
		JetMemory.free_func(TempPtr2);
		return false;
	}
	PtrList.push_back(TempPtr1);
	PtrList.push_back(TempPtr2);
	if ( ImageAPI.ExtractGrayRoiImage3(MaskW, MaskH, MaskStep, MaskPtr, MaskRect, TempStep, TempPtr1, false) == false )
	{		
		JetMemory.free_list(PtrList);
		return false;
	}
	PtrSrc = TempPtr1;
	PtrDst = TempPtr2;
	
	for ( size_t i=0; i<FilterCount; i++ )
	{
		const TBINARY_FILTER &NoiseFilter = FilterList[i];

		bool      IsOK=true;
		const int unsigned IterCount = 1;
		const int Param1 = NoiseFilter.FilterParam1;	
		const NOISE_FILTER_MODE Mode = NoiseFilter.FilterMode;
		switch ( Mode )
		{		
		case NOISE_FILTER_OPEN:		IsOK = ImageAPI.MorphGrayImage3(TempW, TempH, TempStep, PtrSrc, MORPH_OPEN, MORPH_SHAPE_RECT, Param1, IterCount, PtrDst); break;
		case NOISE_FILTER_CLOSE:	IsOK = ImageAPI.MorphGrayImage3(TempW, TempH, TempStep, PtrSrc, MORPH_CLOSE, MORPH_SHAPE_RECT, Param1, IterCount, PtrDst); break;
		case NOISE_FILTER_EROSION:	IsOK = ImageAPI.ErodeGrayImage3(TempW, TempH, TempStep, PtrSrc, Param1, IterCount, PtrDst);	break;
		case NOISE_FILTER_DILATION:	IsOK = ImageAPI.DilateGrayImage3(TempW, TempH, TempStep, PtrSrc, Param1, IterCount, PtrDst);	break;
		case NOISE_FILTER_GRADIENT: IsOK = ImageAPI.MorphGrayImage3(TempW, TempH, TempStep, PtrSrc, MORPH_GRADIENT, MORPH_SHAPE_ELLIPSE, Param1, 1, PtrDst);	break;
		default:
			IsOK = false;
			break;
		}
		if ( false == IsOK )
		{	
			JetMemory.free_list(PtrList);
			return false;
		}			
		PtrTmp = PtrSrc;
		PtrSrc = PtrDst;
		PtrDst = PtrTmp;
	}	
	
	const bool theSameSize=false;
	if ( ImageAPI.PasteGrayRoiImage3(MaskW, MaskH, MaskStep, MaskPtr, MaskRect, TempStep, PtrSrc, false, theSameSize) == false )
	{	
		JetMemory.free_list(PtrList);
		return false;
	}	
	JetMemory.free_list(PtrList);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgImageBinary(CAlgBinaryParam &Param, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR FrameImagePtr, MASK_PTR FrameMaskPtr, SPACE_PTR FrameSpacePtr, int nAlign, IMAGE_PTR &GrayPtr, MASK_PTR &MaskPtr)
{
	bool IsOK=true;
	RECT RoiRect={0,0,0,0};
	RoiRect.right = (int)(ImageW);
	RoiRect.bottom = (int)(ImageH);
	IsOK = ExecAlgImageBinary_Loc(Param, RoiRect, RoiRect, ImageW, ImageH, ImageStep, BitCount, FrameImagePtr, FrameMaskPtr, FrameSpacePtr, nAlign, GrayPtr, MaskPtr);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgImageBinary_Loc(CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR FrameImagePtr, MASK_PTR FrameMaskPtr, SPACE_PTR FrameSpacePtr, int nAlign, IMAGE_PTR &GrayPtr, MASK_PTR &MaskPtr)
{
	const char fnName[] = "CAlgParam::ExecAlgImageBinary_Loc";	
	const IMAGE_SIZE MaskBitCount = 8;
	const IMAGE_SIZE FrameBitCount = BitCount;
	const IMAGE_SIZE GrayStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, MaskBitCount, nAlign);
	const IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, MaskBitCount, nAlign);
	const size_t     BufferSize = ImageAPI.CalcBufferSize(MaskStep, ImageH);

	JetMemory.free_func(MaskPtr);
	if ( JetMemory.alloc_func(BufferSize, GrayPtr, fnName, "GrayPtr")==false ||
		 JetMemory.alloc_func(BufferSize, MaskPtr, fnName, "MaskPtr")==false )
	{
		JetMemory.free_func(GrayPtr);
		JetMemory.free_func(MaskPtr);
		return false;	
	}

	IMAGE_SIZE ClrStep=0;	
	IMAGE_PTR  ImageClrPtr = NULL;	

	double    ImageAve=0.0;
	bool      DynInverse=false;
	double    DynRatio = 0.0;	
	int       Threshold  = 0;
	int       ThresholdL = 0;
	int       ThresholdH = 0;
	int       ThresholdSize=0;
	int       AveBias = 0;
	int       AveAbove = 0;
	int       AveBelow = 0;	
	const int WR = Param.GetBinarySynthesisWR();
	const int WG = Param.GetBinarySynthesisWG();
	const int WB = Param.GetBinarySynthesisWB();
	const bool GrayInvert = Param.GetGrayInvert();
	const bool BinaryInvert = Param.GetBinaryInvert();	
	//const double     BaseHeight = AOIDataCollect.GetSpaceBaseHeight();
	const double  BaseHeight = 0;
	const bool    BaseValueEnabled = GetAlgBaseValueEnabled();
	const double  BaseValueReading = GetAlgBaseValueReading();
	const double  SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
	const BINARY_MODE    BinaryMode = Param.GetBinaryMode();
	const IMAGE_SRC_MODE ImageSrcMode = Param.GetBinaryImageSourceMode();	
	const unsigned char Mask = 0x80;
	const bool bOpenMP = false;
	if ( NULL!=FrameSpacePtr && NULL!=FrameMaskPtr )//No Image
	{	
		if ( ImageAPI.SpaceGrayImageConvertToGray3(ImageW, ImageH, ImageStep, FrameSpacePtr, FrameMaskPtr, MaskRect, GrayStep, GrayPtr, SpaceRatio, false) == false )
		{
			JetMemory.free_func(GrayPtr);
			JetMemory.free_func(MaskPtr);
			return false;	
		}
	}
	else 
	{	
		if ( 8 == FrameBitCount )//Gray Image
		{	
			if ( ImageAPI.AlignGrayImageBuffer3(ImageW, ImageH, ImageStep, FrameImagePtr, MaskRect, GrayStep, GrayPtr, false) == false )
			{
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}
		}
		else if ( 24 == FrameBitCount )//Color Image
		{
			if ( ImageAPI.ColorImageToGrayImage3(ImageW, ImageH, ImageStep, FrameImagePtr, MaskRect, GrayStep, GrayPtr, ImageSrcMode, WR, WG, WB, false) == false )
			{
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}			
		}
	}
	//灰階處理
	if ( true == GrayInvert )
	{
		if ( ImageAPI.InvertGrayImage3(ImageW, ImageH, GrayStep, GrayPtr, MaskRect, GrayPtr) == false )		
		{
			JetMemory.free_func(GrayPtr);
			JetMemory.free_func(MaskPtr);
			return false;
		}
	}
	const double OffsetValue=0.0;
	const double GainValue = Param.GetGrayGainValue();
	const bool  bGainEnabled=Param.CheckGrayGainEnabed();
	if ( true == bGainEnabled )
	{	ImageAPI.ImageOffsetGain3(ImageW, ImageH, GrayStep, 8, GrayPtr, MaskRect, GrayPtr, OffsetValue, GainValue);	}	
	if ( ExecAlgGrayFilter_Rect(Param, CalcRect, MaskRect, ImageW, ImageH, GrayStep, GrayPtr) == false )
	{
		JetMemory.free_func(GrayPtr);
		JetMemory.free_func(MaskPtr);
		return false;
	}
	if ( ExecAlgGrayEdge_Rect(Param, CalcRect, MaskRect, ImageW, ImageH, GrayStep, GrayPtr) == false )
	{
		JetMemory.free_func(GrayPtr);
		JetMemory.free_func(MaskPtr);
		return false;
	}
	switch ( BinaryMode )
	{
	case BINARY_COLOR_FILTER:		
		MemsetByRect(ImageW, ImageH, GrayStep, CalcRect, MaskRect, Mask, MaskPtr);//::memset(MaskPtr, Mask, sizeof(MASK_DATA)*BufferSize);	
		if ( 24 == FrameBitCount )//Color Image
		{
			if ( ImageAPI.ColorImageColorFilter3(ImageW, ImageH, ImageStep, FrameImagePtr, Param.GetBinaryColorGroup(), MaskRect, MaskStep, MaskPtr, true, bOpenMP) == false )
			{
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}
		}
		else
		{
			if ( ImageAPI.RGBImageColorFilter3(ImageW, ImageH, GrayStep, GrayPtr, GrayPtr, GrayPtr, Param.GetBinaryColorGroup(), MaskRect, MaskStep, MaskPtr, true) == false )
			{
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}
		}		
		break;
	case BINARY_FIXED_THRESHOLD:
		MemsetByRect(ImageW, ImageH, GrayStep, CalcRect, MaskRect, Mask, MaskPtr);//::memset(MaskPtr, Mask, sizeof(MASK_DATA)*BufferSize);	
		if ( NULL == FrameSpacePtr )
		{
			ThresholdL = Param.GetFixedThresholdLow();
			ThresholdH = Param.GetFixedThresholdHigh();
			if ( true == BaseValueEnabled )
			{
				ThresholdL = (int)(ThresholdL+BaseValueReading);
				ThresholdH = (int)(ThresholdH+BaseValueReading);
			}
			if ( ImageAPI.BinaryGrayImage3(ImageW, ImageH, GrayStep, GrayPtr, MaskRect, MaskStep, MaskPtr, ThresholdL, ThresholdH) == false )
			{	
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}		
		}
		else
		{
			ThresholdL = (int)(Param.GetFixedThresholdLow()+BaseHeight);
			ThresholdH = (int)(Param.GetFixedThresholdHigh()+BaseHeight);
			if ( true == BaseValueEnabled )
			{
				ThresholdL = (int)(ThresholdL+BaseValueReading);
				ThresholdH = (int)(ThresholdH+BaseValueReading);
			}
			//::memset(MaskPtr, Mask, sizeof(MASK_DATA)*BufferSize);	
			if ( ImageAPI.BinarySpaceImage3(ImageW, ImageH, GrayStep, FrameSpacePtr, MaskRect, MaskStep, MaskPtr, ThresholdL, ThresholdH) == false )
			{	
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}
		}
		break;
	case BINARY_DYNAMIC_THRESHOLD:
		DynRatio = Param.GetDynamicThresholdRatio();
		DynInverse = false;//Param.DynamicThresholdInverse;
		if ( DynInverse == false )
		{
			if ( ImageAPI.CalcGrayImageRelativeBrightThreshold(ImageW, ImageH, GrayStep, GrayPtr, CalcRect, DynRatio, Threshold) == false )
			{
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}			
			ThresholdL = Threshold;
			ThresholdH = FIXED_THRESHOLD_MAX_2D;			
		}
		else
		{
			if ( ImageAPI.CalcGrayImageRelativeBlackThreshold(ImageW, ImageH, GrayStep, GrayPtr, CalcRect, DynRatio, Threshold) == false )
			{
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}
			ThresholdL = FIXED_THRESHOLD_MIN_2D;
			ThresholdH = Threshold;
		}
		Param.SetDynamicThresholdValue(Threshold);
		MemsetByRect(ImageW, ImageH, GrayStep, CalcRect, MaskRect, Mask, MaskPtr);//::memset(MaskPtr, Mask, sizeof(MASK_DATA)*BufferSize);	
		if ( ImageAPI.BinaryGrayImage3(ImageW, ImageH, GrayStep, GrayPtr, MaskRect, MaskStep, MaskPtr, ThresholdL, ThresholdH) == false )
		{	
			JetMemory.free_func(GrayPtr);
			JetMemory.free_func(MaskPtr);
			return false;
		}
		break;
	case BINARY_RELATIVE_AVE_THRESHOLD:
		AveBias = Param.GetRelativeAveThresholdBias();
		AveAbove = Param.GetRelativeAveThresholdAbove();
		AveBelow = Param.GetRelativeAveThresholdBelow();
		MemsetByRect(ImageW, ImageH, GrayStep, CalcRect, MaskRect, Mask, MaskPtr);//::memset(MaskPtr, Mask, sizeof(MASK_DATA)*BufferSize);	
		if ( NULL == FrameSpacePtr )
		{	
			if ( ImageAPI.CalcGrayImageAverage(ImageW, ImageH, GrayStep, GrayPtr, CalcRect, ImageAve) == false )
			{
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}			
			Threshold  = (int)(ImageAve+AveBias+0.5);
			ThresholdL = (int)(Threshold - AveBelow);
			ThresholdH = (int)(Threshold + AveAbove);
			Param.SetRelativeAveThresholdValue(ImageAve);
			if ( ThresholdL < FIXED_THRESHOLD_MIN_2D ) { ThresholdL = FIXED_THRESHOLD_MIN_2D; }
			if ( ThresholdL > FIXED_THRESHOLD_MAX_2D ) { ThresholdL = FIXED_THRESHOLD_MAX_2D; }
			if ( ThresholdH < FIXED_THRESHOLD_MIN_2D ) { ThresholdH = FIXED_THRESHOLD_MIN_2D; }
			if ( ThresholdH > FIXED_THRESHOLD_MAX_2D ) { ThresholdH = FIXED_THRESHOLD_MAX_2D; }
			
			if ( ImageAPI.BinaryGrayImage3(ImageW, ImageH, GrayStep, GrayPtr, MaskRect, MaskStep, MaskPtr, ThresholdL, ThresholdH) == false )
			{	
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}
		}
		else
		{	
			if ( ImageAPI.CalcSpaceImageAverage(ImageW, ImageH, GrayStep, FrameSpacePtr, CalcRect, ImageAve) == false )
			{
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}			
			Threshold  = (int)(ImageAve+AveBias+0.5);
			ThresholdL = (int)(Threshold - AveBelow);
			ThresholdH = (int)(Threshold + AveAbove);
			Param.SetRelativeAveThresholdValue(ImageAve);
			if ( ThresholdL < FIXED_THRESHOLD_MIN_3D ) { ThresholdL = FIXED_THRESHOLD_MIN_3D; }
			if ( ThresholdL > FIXED_THRESHOLD_MAX_3D ) { ThresholdL = FIXED_THRESHOLD_MAX_3D; }
			if ( ThresholdH < FIXED_THRESHOLD_MIN_3D ) { ThresholdH = FIXED_THRESHOLD_MIN_3D; }
			if ( ThresholdH > FIXED_THRESHOLD_MAX_3D ) { ThresholdH = FIXED_THRESHOLD_MAX_3D; }
			//::memset(MaskPtr, Mask, sizeof(MASK_DATA)*BufferSize);	
			if ( ImageAPI.BinarySpaceImage3(ImageW, ImageH, GrayStep, FrameSpacePtr, MaskRect, MaskStep, MaskPtr, ThresholdL, ThresholdH) == false )
			{	
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}
		}
		break;
	case BINARY_ADAPTIVE_THRESHOLD:
		Threshold = Param.GetAdaptiveThresholdGap();
		ThresholdSize = Param.GetAdaptiveThresholdCalcSize();
		MemsetByRect(ImageW, ImageH, GrayStep, CalcRect, MaskRect, Mask, MaskPtr);//::memset(MaskPtr, Mask, sizeof(MASK_DATA)*BufferSize);	
		if ( NULL == FrameSpacePtr )
		{				
			if ( ImageAPI.AdaptiveBinaryGrayImage3(ImageW, ImageH, GrayStep, GrayPtr, MaskRect, MaskStep, MaskPtr, ThresholdSize, Threshold) == false )
			{	
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}
		}
		else
		{	
			if ( ImageAPI.AdaptiveBinarySpaceImage3(ImageW, ImageH, GrayStep, FrameSpacePtr, MaskRect, MaskStep, MaskPtr, ThresholdSize, Threshold) == false )
			{	
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}
		}
		break;	
	default://BINARY_DISABLE
		MemsetByRect(ImageW, ImageH, GrayStep, CalcRect, MaskRect, 0xFF, MaskPtr);//::memset(MaskPtr, 0xFF, sizeof(MASK_DATA)*BufferSize);
		break;
	}
	if ( BINARY_DISABLE != BinaryMode )	
	{
		if ( true == BinaryInvert )
		{
			if ( ImageAPI.InvertMaskImage3(ImageW, ImageH, MaskStep, MaskPtr, MaskRect, MaskStep, MaskPtr) == false )
			{
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}
		}

		if ( ExecAlgBinaryFilter_Rect(Param, CalcRect, MaskRect, ImageW, ImageH, MaskStep, MaskPtr) == false )
		{
			JetMemory.free_func(GrayPtr);
			JetMemory.free_func(MaskPtr);
			return false;
		}		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgUniFrameBinary(CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, const std::vector<TUNI_FRAME> &UniFrameList, IMAGE_SIZE &MaskW, IMAGE_SIZE &MaskH, IMAGE_SIZE &MaskStep, IMAGE_SIZE &MaskBitCount, MASK_PTR &MaskPtr, IMAGE_PTR &GrayPtr, bool bTestWnd)
{
	const char fnName[] = "CAlgParam::ExecAlgUniFrameBinary";
	const size_t FrameCount = UniFrameList.size();	
	const size_t FrameIndex = Param.GetBinaryFrameIndex();
	if (FrameIndex >= FrameCount)
	{	return false;	}

	const TUNI_FRAME &UniFrameRef = UniFrameList[FrameIndex];
	const int nAlign = 4;
	const IMAGE_SIZE ImageW = UniFrameRef.ImageW;
	const IMAGE_SIZE ImageH = UniFrameRef.ImageH;
	const IMAGE_SIZE ImageStep = UniFrameRef.ImageStep;
	const IMAGE_SIZE FrameBitCount = UniFrameRef.BitCount;
	IMAGE_PTR        FrameImagePtr = UniFrameRef.ImagePtr;
	MASK_PTR         FrameMaskPtr = UniFrameRef.MaskPtr;
	SPACE_PTR        FrameSpacePtr = UniFrameRef.SpacePtr;

	MaskW = ImageW;
	MaskH = ImageH;
	MaskBitCount = 8;
	MaskStep = JetAPI::GetBMPImagePixelsPerLine(MaskW, MaskBitCount, nAlign);
	const IMAGE_SIZE GrayStep = MaskStep;
	const size_t     BufferSize = ImageAPI.CalcBufferSize(MaskStep, MaskH);
	
	JetMemory.free_func(MaskPtr);
	JetMemory.free_func(GrayPtr);
	if ( JetMemory.alloc_func(BufferSize, MaskPtr, fnName, "MaskPtr")==false || 
		 JetMemory.alloc_func(BufferSize, GrayPtr, fnName, "GrayPtr") == false )
	{	
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return false;	
	}
#define _DEBUG
#ifdef _DEBUG
	::memset(MaskPtr, 0x00, sizeof(MASK_DATA)*BufferSize);
	::memset(GrayPtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);
#endif//_DEBUG

	IMAGE_SIZE ClrStep=0;	
	IMAGE_PTR  ImageClrPtr = NULL;	

	double    ImageAve=0.0;
	bool      DynInverse=false;
	double    DynRatio = 0.0;
	int       Threshold  = 0;
	int       ThresholdL = 0;
	int       ThresholdH = 0;
	int       ThresholdSize = 0;
	int       AveBias = 0;
	int       AveAbove = 0;
	int       AveBelow = 0;	
	const int WR = Param.GetBinarySynthesisWR();
	const int WG = Param.GetBinarySynthesisWG();
	const int WB = Param.GetBinarySynthesisWB();
	const bool GrayInvert = Param.GetGrayInvert();
	const bool BinaryInvert = Param.GetBinaryInvert();	
	//const double     BaseHeight = AOIDataCollect.GetSpaceBaseHeight();
	const double  BaseHeight = 0;
	const bool    BaseValueEnabled = GetAlgBaseValueEnabled();
	const double  BaseValueReading = GetAlgBaseValueReading();
	const double  SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
	const BINARY_MODE    BinaryMode = Param.GetBinaryMode();
	const IMAGE_SRC_MODE ImageSrcMode = Param.GetBinaryImageSourceMode();
	const unsigned char Mask = 0x80;
	const bool bOpenMP = false;
	if ( NULL!=FrameSpacePtr && NULL!=FrameMaskPtr )//No Image
	{	
		if ( ImageAPI.SpaceGrayImageConvertToGray3(ImageW, ImageH, ImageStep, FrameSpacePtr, FrameMaskPtr, MaskRect, GrayStep, GrayPtr, SpaceRatio, false) == false )
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			return false;
		}		
	}
	else 
	{	
		if ( 8 == FrameBitCount )//Gray Image
		{	
			if ( ImageAPI.AlignGrayImageBuffer3(ImageW, ImageH, ImageStep, FrameImagePtr, MaskRect, GrayStep, GrayPtr, false) == false )
			{
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
				return false;
			}					
		}
		else if ( 24 == FrameBitCount )//Color Image
		{
			if ( ImageAPI.ColorImageToGrayImage3(ImageW, ImageH, ImageStep, FrameImagePtr, MaskRect, GrayStep, GrayPtr, ImageSrcMode, WR, WG, WB, false) == false )
			{
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
				return false;
			}
		}
	}	

	//灰階處理
	if ( true == GrayInvert )
	{
		if ( ImageAPI.InvertGrayImage3(ImageW, ImageH, GrayStep, GrayPtr, MaskRect, GrayPtr) == false )		
		{
			JetMemory.free_func(GrayPtr);
			JetMemory.free_func(MaskPtr);
			return false;
		}
	}
	const double OffsetValue=0.0;
	const double GainValue = Param.GetGrayGainValue();
	const bool  bGainEnabled=Param.CheckGrayGainEnabed();
	if ( true == bGainEnabled )
	{	ImageAPI.ImageOffsetGain3(ImageW, ImageH, GrayStep, 8, GrayPtr, MaskRect, GrayPtr, OffsetValue, GainValue);	}	
	if ( ExecAlgGrayFilter_Rect(Param, CalcRect, MaskRect, ImageW, ImageH, GrayStep, GrayPtr) == false )
	{
		JetMemory.free_func(GrayPtr);
		JetMemory.free_func(MaskPtr);
		return false;
	}
	if ( ExecAlgGrayEdge_Rect(Param, CalcRect, MaskRect, ImageW, ImageH, GrayStep, GrayPtr) == false )
	{
		JetMemory.free_func(GrayPtr);
		JetMemory.free_func(MaskPtr);
		return false;
	}
	switch ( BinaryMode )
	{
	case BINARY_COLOR_FILTER:		
		MemsetByRect(ImageW, ImageH, GrayStep, CalcRect, MaskRect, Mask, MaskPtr);//::memset(MaskPtr, Mask, sizeof(MASK_DATA)*BufferSize);	
		if ( 24 == FrameBitCount )//Color Image
		{
			if ( ImageAPI.ColorImageColorFilter3(ImageW, ImageH, ImageStep, FrameImagePtr, Param.GetBinaryColorGroup(), MaskRect, MaskStep, MaskPtr, true, bOpenMP) == false )
			{
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
				return false;
			}
		}
		else
		{
			if ( ImageAPI.RGBImageColorFilter3(ImageW, ImageH, GrayStep, GrayPtr, GrayPtr, GrayPtr, Param.GetBinaryColorGroup(), MaskRect, MaskStep, MaskPtr, true) == false )
			{
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
				return false;
			}
		}		
		break;
	case BINARY_FIXED_THRESHOLD:
		MemsetByRect(ImageW, ImageH, GrayStep, CalcRect, MaskRect, Mask, MaskPtr);//::memset(MaskPtr, Mask, sizeof(MASK_DATA)*BufferSize);	
		if ( NULL == FrameSpacePtr )
		{
			ThresholdL = Param.GetFixedThresholdLow();
			ThresholdH = Param.GetFixedThresholdHigh();
			if ( true == BaseValueEnabled )
			{
				ThresholdL = (int)(ThresholdL+BaseValueReading);
				ThresholdH = (int)(ThresholdH+BaseValueReading);
			}
			if ( ImageAPI.BinaryGrayImage3(ImageW, ImageH, GrayStep, GrayPtr, MaskRect, MaskStep, MaskPtr, ThresholdL, ThresholdH) == false )
			{	
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
				return false;
			}		
		}
		else
		{
			ThresholdL = (int)(Param.GetFixedThresholdLow()+BaseHeight);
			ThresholdH = (int)(Param.GetFixedThresholdHigh()+BaseHeight);	
			if ( true == BaseValueEnabled )
			{
				ThresholdL = (int)(ThresholdL+BaseValueReading);
				ThresholdH = (int)(ThresholdH+BaseValueReading);
			}
			if ( ImageAPI.BinarySpaceImage3(ImageW, ImageH, GrayStep, FrameSpacePtr, MaskRect, MaskStep, MaskPtr, ThresholdL, ThresholdH) == false )
			{	
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
				return false;
			}
		}
		break;
	case BINARY_DYNAMIC_THRESHOLD:
		DynRatio = Param.GetDynamicThresholdRatio();
		DynInverse = false;//Param.DynamicThresholdInverse;		
		if ( DynInverse == false )
		{
			if ( ImageAPI.CalcGrayImageRelativeBrightThreshold(ImageW, ImageH, GrayStep, GrayPtr, CalcRect, DynRatio, Threshold) == false )
			{
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
				return false;
			}
			ThresholdL = Threshold;
			ThresholdH = FIXED_THRESHOLD_MAX_2D;			
		}
		else
		{
			if ( ImageAPI.CalcGrayImageRelativeBlackThreshold(ImageW, ImageH, GrayStep, GrayPtr, CalcRect, DynRatio, Threshold) == false )
			{
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
				return false;
			}			
			ThresholdL = FIXED_THRESHOLD_MIN_2D;
			ThresholdH = Threshold;
		}
		Param.SetDynamicThresholdValue(Threshold);
		MemsetByRect(ImageW, ImageH, GrayStep, CalcRect, MaskRect, Mask, MaskPtr);//::memset(MaskPtr, Mask, sizeof(MASK_DATA)*BufferSize);	
		if ( ImageAPI.BinaryGrayImage3(ImageW, ImageH, GrayStep, GrayPtr, MaskRect, MaskStep, MaskPtr, ThresholdL, ThresholdH) == false )
		{	
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			return false;
		}
		break;
	case BINARY_RELATIVE_AVE_THRESHOLD:
		AveBias = Param.GetRelativeAveThresholdBias();
		AveAbove = Param.GetRelativeAveThresholdAbove();
		AveBelow = Param.GetRelativeAveThresholdBelow();
		MemsetByRect(ImageW, ImageH, GrayStep, CalcRect, MaskRect, Mask, MaskPtr);//::memset(MaskPtr, Mask, sizeof(MASK_DATA)*BufferSize);	
		if ( NULL == FrameSpacePtr )
		{	
			if ( ImageAPI.CalcGrayImageAverage(ImageW, ImageH, GrayStep, GrayPtr, CalcRect, ImageAve) == false )
			{
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);				
				return false;
			}			
			Threshold  = (int)(ImageAve+AveBias+0.5);
			ThresholdL = (int)(Threshold - AveBelow);
			ThresholdH = (int)(Threshold + AveAbove);
			Param.SetRelativeAveThresholdValue(ImageAve);
			if ( ThresholdL < FIXED_THRESHOLD_MIN_2D ) { ThresholdL = FIXED_THRESHOLD_MIN_2D; }
			if ( ThresholdL > FIXED_THRESHOLD_MAX_2D ) { ThresholdL = FIXED_THRESHOLD_MAX_2D; }
			if ( ThresholdH < FIXED_THRESHOLD_MIN_2D ) { ThresholdH = FIXED_THRESHOLD_MIN_2D; }
			if ( ThresholdH > FIXED_THRESHOLD_MAX_2D ) { ThresholdH = FIXED_THRESHOLD_MAX_2D; }			
			if ( ImageAPI.BinaryGrayImage3(ImageW, ImageH, GrayStep, GrayPtr, MaskRect, MaskStep, MaskPtr, ThresholdL, ThresholdH) == false )
			{	
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
				return false;
			}
		}
		else
		{	
			if ( ImageAPI.CalcSpaceImageAverage(ImageW, ImageH, GrayStep, FrameSpacePtr, CalcRect, ImageAve) == false )
			{
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
				return false;
			}			
			Threshold  = (int)(ImageAve+AveBias+0.5);
			ThresholdL = (int)(Threshold - AveBelow);
			ThresholdH = (int)(Threshold + AveAbove);
			Param.SetRelativeAveThresholdValue(ImageAve);
			if ( ThresholdL < FIXED_THRESHOLD_MIN_3D ) { ThresholdL = FIXED_THRESHOLD_MIN_3D; }
			if ( ThresholdL > FIXED_THRESHOLD_MAX_3D ) { ThresholdL = FIXED_THRESHOLD_MAX_3D; }
			if ( ThresholdH < FIXED_THRESHOLD_MIN_3D ) { ThresholdH = FIXED_THRESHOLD_MIN_3D; }
			if ( ThresholdH > FIXED_THRESHOLD_MAX_3D ) { ThresholdH = FIXED_THRESHOLD_MAX_3D; }			
			if ( ImageAPI.BinarySpaceImage3(ImageW, ImageH, GrayStep, FrameSpacePtr, MaskRect, MaskStep, MaskPtr, ThresholdL, ThresholdH) == false )
			{	
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
				return false;
			}
		}
		break;
	case BINARY_ADAPTIVE_THRESHOLD:
		Threshold = Param.GetAdaptiveThresholdGap();
		ThresholdSize = Param.GetAdaptiveThresholdCalcSize();
		MemsetByRect(ImageW, ImageH, GrayStep, CalcRect, MaskRect, Mask, MaskPtr);//::memset(MaskPtr, Mask, sizeof(MASK_DATA)*BufferSize);	
		if ( NULL == FrameSpacePtr )
		{				
			if ( ImageAPI.AdaptiveBinaryGrayImage3(ImageW, ImageH, GrayStep, GrayPtr, MaskRect, MaskStep, MaskPtr, ThresholdSize, Threshold) == false )
			{	
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}
		}
		else
		{	
			if ( ImageAPI.AdaptiveBinarySpaceImage3(ImageW, ImageH, GrayStep, FrameSpacePtr, MaskRect, MaskStep, MaskPtr, ThresholdSize, Threshold) == false )
			{	
				JetMemory.free_func(GrayPtr);
				JetMemory.free_func(MaskPtr);
				return false;
			}
		}
		break;	
	default://BINARY_DISABLE
		MemsetByRect(ImageW, ImageH, GrayStep, CalcRect, MaskRect, 0xFF, MaskPtr);//::memset(MaskPtr, 0xFF, sizeof(MASK_DATA)*BufferSize);
		break;
	}
	if ( BINARY_DISABLE != BinaryMode )	
	{
		if ( true == BinaryInvert )
		{
			if ( ImageAPI.InvertMaskImage3(ImageW, ImageH, MaskStep, MaskPtr, MaskRect, MaskStep, MaskPtr) == false )
			{
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
				return false;
			}
		}
		
		if ( ExecAlgBinaryFilter_Rect(Param, CalcRect, MaskRect, ImageW, ImageH, MaskStep, MaskPtr) == false )
		{
			JetMemory.free_func(GrayPtr);
			JetMemory.free_func(MaskPtr);
			return false;
		}		
	}

#ifdef _DEBUG	
	CString str;
	bool    bSaved = true;
	CString ComponentName;
	CString DebugFolder=GetAlgDebugFolder();	
	CAOIWnd *WndPtr=GetAlgWndPtr();	
	if ( true==bSaved && false==bTestWnd && NULL!=WndPtr )
	{
		CAOIModel  *ModelPtr = WndPtr->GetWndModelPtr();
		const unsigned int WndIndex = WndPtr->GetWndIndex();
		if ( NULL != ModelPtr )
		{
			CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
			if ( NULL != ComponentPtr )
			{	ComponentName = ComponentPtr->GetComponentFullName(); }
		}
		str.Format(_T("%s\\%s_ModelWndAlgBin#%d.BMP"), DebugFolder, ComponentName, WndIndex+1);
		ImageAPI.SaveBMPImage(str, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);

		if ( NULL != GrayPtr )
		{
			str.Format(_T("%s\\%s_ModelWndAlgGry#%d.BMP"), DebugFolder, ComponentName, WndIndex+1);
			ImageAPI.SaveBMPImage(str, MaskW, MaskH, GrayStep, MaskBitCount, GrayPtr, true);
		}
	}
#endif//_DEBUG
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgUniFrameShapeMask(CAOIWnd *WndPtr, const RECT &WndRect, IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, IMAGE_SIZE MaskBitCount, MASK_PTR MaskPtr)//合併檢測框外型2值化
{
	if ( NULL == WndPtr )
	{	return true; }
	if ( NULL == MaskPtr )
	{	return true; }
	if ( WndPtr->CheckWndNeedShapeMask() == false )
	{	return true; }

	MASK_PTR      ShapePtr = NULL;
	IMAGE_SIZE    ShapeW = MaskW;
	IMAGE_SIZE    ShapeH = MaskH;
	IMAGE_SIZE    ShpaeBitCount=8;
	IMAGE_SIZE    ShapeStep = MaskStep;
	const size_t  ShapeBufferSize = ImageAPI.CalcBufferSize(ShapeStep, ShapeH);
	if ( JetMemory.alloc_func(ShapeBufferSize, ShapePtr, "CAlgParam::ExecAlgUniFrameShapeMask", "ShapePtr") == false )
	{	return false;	}
	ImageAPI.FillImageRoi(ShapeW, ShapeH, ShapeStep, ShpaeBitCount, ShapePtr, WndRect, 0, 0, 0);
	if ( WndPtr->BuildWndShapeMask3(ShapeW, ShapeH, ShapeStep, ShapePtr, WndRect) == false )
	{	
		JetMemory.free_func(ShapePtr);
		return false;
	}	
	if ( ImageAPI.Intersection2MaskImage3(MaskW, MaskH, MaskStep, MaskPtr, ShapePtr, WndRect, MaskStep, MaskPtr, 255, 0, true) == false )
	{	
		JetMemory.free_func(ShapePtr);
		return false;
	}
	JetMemory.free_func(ShapePtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgUniFrameMaskBinary(CAOIModel *ModelPtr, CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, std::vector<TUNI_FRAME> &UniFrameList, IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, IMAGE_SIZE MaskBitCount, MASK_PTR MaskPtr, bool bTestWnd)
{
	if ( NULL == ModelPtr ) { return false; }
	IMAGE_SIZE    MaskW2=0;
	IMAGE_SIZE    MaskH2=0;	
	IMAGE_SIZE    MaskStep2=0;
	IMAGE_SIZE    MaskBitCount2=8;
	MASK_PTR      MaskPtr2  = NULL;
	IMAGE_PTR     GrayPtr2 = NULL;		
	const size_t FrameCount = UniFrameList.size();	
	const size_t FrameIndex = Param.GetBinaryFrameIndex();	
	if ( FrameIndex >= FrameCount ) 
	{	return true; }
	if ( BINARY_DISABLE == Param.GetBinaryMode() ) 
	{	return true;	}
	if ( ExecAlgUniFrameBinary(Param, CalcRect, MaskRect, UniFrameList, MaskW2, MaskH2, MaskStep2, MaskBitCount2, MaskPtr2, GrayPtr2, bTestWnd) == false )
	{		
		JetMemory.free_func(MaskPtr2);
		JetMemory.free_func(GrayPtr2);
		return false; 
	}

	if ( MaskW!=MaskW2 || MaskH!=MaskH2 || MaskStep!=MaskStep2 )
	{		
		JetMemory.free_func(MaskPtr2);
		JetMemory.free_func(GrayPtr2);
		return false;
	}

	//合併遮罩
	if ( ImageAPI.MergeMaskImage3(MaskW, MaskH, MaskStep, MaskPtr, MaskPtr2, MaskRect, MERGE_MASK_AND, MaskPtr) == false ) 
	{		
		JetMemory.free_func(MaskPtr2);
		JetMemory.free_func(GrayPtr2);
		return false;
	}	
	JetMemory.free_func(MaskPtr2);
	JetMemory.free_func(GrayPtr2);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::UpdateAlgFrameUniqueID(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList)
{
	//Mask Image
	GetAlgMaskBinParam().UpdateBinaryFrameUniqueID(DefaultIndex, DefaultUniqueID, FrameIndexMapList);

	//Alg Image
	GetAlgImageBinParam().UpdateBinaryFrameUniqueID(DefaultIndex, DefaultUniqueID, FrameIndexMapList);	

#ifdef ALG_MEASURE_BLACK_GLUE_USE
	TALG_PARAM_MEASURE_BLACK_GLUE &bgParam = GetAlgParamMeasureBlackGlue();
	bgParam.bgFrameIndex1 = GetAlgFrameIndex(bgParam.bgFrameUniqueID1, DefaultIndex, DefaultUniqueID, FrameIndexMapList);
	bgParam.bgFrameIndex2 = GetAlgFrameIndex(bgParam.bgFrameUniqueID2, DefaultIndex, DefaultUniqueID, FrameIndexMapList);
	bgParam.bgFrameIndex3 = GetAlgFrameIndex(bgParam.bgFrameUniqueID3, DefaultIndex, DefaultUniqueID, FrameIndexMapList);
	bgParam.bgFrameIndex4 = GetAlgFrameIndex(bgParam.bgFrameUniqueID4, DefaultIndex, DefaultUniqueID, FrameIndexMapList);
#endif//ALG_MEASURE_BLACK_GLUE_USE	

#ifdef ALG_MEASURE_FLUX_AREA_USE
	TALG_PARAM_MEASURE_FLUX_AREA &faParam = GetAlgParamMeasureFluxArea();
	faParam.faFrameIndex1 = GetAlgFrameIndex(faParam.faFrameUniqueID1, DefaultIndex, DefaultUniqueID, FrameIndexMapList);	
#endif//ALG_MEASURE_FLUX_AREA_USE	
	return true;
}
//-------------------------------------------------------------------------------------//
unsigned int CAlgParam::GetAlgFrameIndex(unsigned int UniqueID, unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList)
{
	unsigned int FrameIndex = -1;		
	unsigned int FrameUniqueID = UniqueID;		
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
	return FrameIndex;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckAlgSaveDefectImage() const//確認演算法儲存瑕疵圖檔
{	
	if ( AOIDataCollect.GetSaveDefectImage() == false )
	{	return false; }
	if ( GetAlgSaveDefectImageEnabled() == false )
	{	return false; }
	RESULT_ID Result=GetAlgResultID();
	if ( RESULT_ID_NONE   == Result ||
		 RESULT_ID_OK     == Result ||
		 RESULT_ID_BYPASS == Result ||
		 RESULT_ID_SKIP    == Result )
	{	return false;	}		             
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::SaveAlgDefectImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR GrayPtr, IMAGE_PTR BinPtr, const RECT &WndRect, bool bFixImg)//儲存演算法瑕疵圖片
{
	const char fnName[]= "CAlgParam::SaveAlgDefectImage";
	if ( NULL==GrayPtr || NULL==BinPtr ) { return true; }
	if ( CheckAlgSaveDefectImage() == false ) { return true; }			

	CAOIWnd *WndPtr = GetAlgWndPtr();
	if ( NULL == WndPtr) { return true; }	
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	if ( NULL == ModelPtr) { return true; }	
	if ( ModelPtr->GetModelSelfTest() == true )//模組自我測試
	{	return true; }	
	CAOIRgn *AttachedPtr=ModelPtr->GetModelAttachedPtr();
	if ( NULL == AttachedPtr ) { return true; }
	CAOIProject *ProjectPtr = AttachedPtr->GetRgnProjectPtr();
	if ( NULL == ProjectPtr ) { return true; }
	const AOI_OBJ_TYPE AttachedType=AttachedPtr->GetObjType();	
	if ( AOI_OBJ_COMPONENT != AttachedType )
	{	return true; }		
	
	CString Filename;	
	CString AttachedName=WndPtr->GetWndFullName();
	CString SpcImageFolder=ProjectPtr->GetProjectSpcImageFolder();
	if ( ProjectPtr->GetProjectUseLocalFolder() )
	{	SpcImageFolder = ProjectPtr->GetProjectSpcImageFolderLocal();	}	
	if ( SpcImageFolder.GetLength() == 0 )
	{	SpcImageFolder = AOIDataCollect.GetAOITempDirectory();	}
	Filename.Format(_T("%s\\%s.JPG"), SpcImageFolder, AttachedName);	

	IMAGE_PTR    RoiPtr=NULL;	
	IMAGE_SIZE   RoiBitCnt=24;
	IMAGE_SIZE   RoiW=WndRect.right-WndRect.left;
	IMAGE_SIZE   RoiH=WndRect.bottom-WndRect.top;	
	IMAGE_SIZE   RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, RoiBitCnt, 4);		
	if ( true == bFixImg )
	{		
		RoiW = ImageW;
		RoiH = ImageH;
		RoiStep = ImageStep;
		RoiBitCnt = BitCount;
		RoiPtr = GrayPtr;
	}
	else
	{
		RoiPtr=NULL;	
		RoiBitCnt=24;
		RoiW=WndRect.right-WndRect.left;
		RoiH=WndRect.bottom-WndRect.top;	
		RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, RoiBitCnt, 4);	
		const size_t RoiSize=ImageAPI.CalcBufferSize(RoiStep, RoiH);	
		const CAlgBinaryParam &BinaryParam = GetAlgImageBinParam();	
		BINARY_MODE BinaryMode = BinaryParam.GetBinaryMode();
		if ( JetMemory.alloc_func(RoiSize, RoiPtr, fnName, "RoiPtr") == false )
		{	
			JetMemory.free_func(RoiPtr);		
			return false; 
		}		
	
		if ( BINARY_DISABLE == BinaryMode )
		{	
			RoiBitCnt = BitCount;
			RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, RoiBitCnt, 4);
			if ( ImageAPI.ExtractRoiImage3(ImageW, ImageH, ImageStep, BitCount, GrayPtr, WndRect, RoiStep, RoiPtr, false) == false )
			{
				JetMemory.free_func(RoiPtr);
				return false;
			}		
			AOIDataCollect.ExecEnhanceDisplayImage(RoiW, RoiH, RoiStep, BitCount, RoiPtr, RoiPtr);				
		}
		else
		{
			IMAGE_PTR    TmpPtr=NULL;
			IMAGE_SIZE   TmpBitCnt=BitCount;
			IMAGE_SIZE   TmpStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, TmpBitCnt, 4);
			const size_t TmpSize=ImageAPI.CalcBufferSize(TmpStep, RoiH);
			if ( JetMemory.alloc_func(TmpSize, TmpPtr, fnName, "TmpPtr") == false )
			{
				JetMemory.free_func(RoiPtr);
				return false;
			}
			if ( ImageAPI.ExtractRoiImage3(ImageW, ImageH, ImageStep, BitCount, BinPtr, WndRect, TmpStep, TmpPtr, false) == false )
			{
				JetMemory.free_func(TmpPtr);
				JetMemory.free_func(RoiPtr);			
				return false;
			}

			IMAGE_DATA mask = 0xFF;
			IMAGE_DATA Red=0, Grn=0, Blu=0;
			IMAGE_DATA mskR=0xFF, mskG=0xFF, mskB=0x00, mskV=0xFF, Alpha=0;
			const unsigned int FrameUniqueID = BinaryParam.GetBinaryFrameUniqueID();		
			AOIDataCollect.GetMaskImageColor(FrameUniqueID, mskR, mskG, mskB, mskV, Alpha);

			for ( int i=0; i<RoiH; i++ )
			{
				for ( int j=0; j<RoiW; j++ )
				{
					const int TmpIdx=(i*TmpStep)+j;
					const int RoiIdx=(i*RoiStep)+(j*3);
					if ( 0 == TmpPtr[TmpIdx] )
					{	Red = Grn = Blu = 0;	}
					else
					{
						Red = mskR;
						Grn = mskG;
						Blu = mskB;
					}
					RoiPtr[RoiIdx] = Blu;
					RoiPtr[RoiIdx+1] = Grn;
					RoiPtr[RoiIdx+2] = Red;
				}
			}
			JetMemory.free_func(TmpPtr);
		}	
	}
	const bool bSucc=ImageAPI.SaveImage(Filename, RoiW, RoiH, RoiStep, RoiBitCnt, RoiPtr, true);
	if ( true == bFixImg )
	{	RoiPtr = NULL;	}
	else
	{	JetMemory.free_func(RoiPtr);	}
	SetAlgSaveDefectImageDone(bSucc);
	if ( true == bSucc )
	{	SetAlgSaveDefectImageName(Filename);	}
	return true;
}
//-------------------------------------------------------------------------------------//