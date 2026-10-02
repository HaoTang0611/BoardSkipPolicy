// AlgParam.h: interface for the CAlgParam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALGPARAM_H__23046515_E7B6_4B93_9DEE_E439E9840A50__INCLUDED_)
#define AFX_ALGPARAM_H__23046515_E7B6_4B93_9DEE_E439E9840A50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "AlgParamDef.h"
#include "AOIModelDef.h"
#include "PatternParam.h"
#include "AlgBinaryParam.h"
//-------------------------------------------------------------------------------------//
class CAOIWnd;
class CAOILand;
class CAOIModel;
class CAOIFileIO;
//-------------------------------------------------------------------------------------//
class CAlgParam  
{
public:
	//---------------------------------------------------------------------------------//
	static bool                CheckAlgUseExtendBox(ALG_TYPE Type);//確認演算法需要外擴檢測框	
	//---------------------------------------------------------------------------------//	
	static bool                BuildAlgTypeList(std::vector<ALG_TYPE> &List);//建立演算法樣式列表
	//---------------------------------------------------------------------------------//	
	static bool                CheckImageSourceGrayMode(IMAGE_SRC_MODE Mode);//確認影像來源為灰階模式
	//---------------------------------------------------------------------------------//
	static bool                CheckSortAscMode(BOX_TOWARD Toward, ALG_SEARCH_DIRECTION Direction);
	static BOX_TOWARD          ChangeTowardByDirection(BOX_TOWARD Toward, ALG_SEARCH_DIRECTION Direction);
	//---------------------------------------------------------------------------------//	
	static bool                AdjustAlgBinaryParam(FRAME_TYPE FrameType, CAlgBinaryParam &BinaryParam);
	//---------------------------------------------------------------------------------//	
	static CString             GetAlgTiltCellModeText(ALG_TILE_CELL_MODE CellMode);
	static ALG_TILE_CELL_MODE  GetAlgTiltCellModeByText(LPCTSTR DirText);
	//---------------------------------------------------------------------------------//		
	static bool                DefaultAlgBarcodeStepParam(TALG_BARCODE_STEP_PARAM &Param);		
	//---------------------------------------------------------------------------------//	
	static bool                CheckAlgGroupCompareSupported(WND_DEFECT_ID WndDefectID);
	//---------------------------------------------------------------------------------//		
	static bool                CheckAlgPatternFileUsed(ALG_TYPE Type);
	static int                 ExtractAlgPatternIndex(LPCTSTR ImageName);	
	//---------------------------------------------------------------------------------//	
	static bool                BuildPixelCompareParam(const TALG_PARAM_IMAGE_MATCH &imParam, const TPOINT2D &Scale, TPixelCompareParam &tpcParam);
	static bool                BuildPixelCompareParam(const TALG_PARAM_PIXEL_COMPARE &pcParam, const TPOINT2D &Scale, TPixelCompareParam &tpcParam);
	//---------------------------------------------------------------------------------//
	//演算法單步計算-樣板比對
	static bool                CheckOK_PatternMatchOffsetX(const CAlgParam &Param);
	static bool                CheckOK_PatternMatchOffsetXUSL(const CAlgParam &Param);
	static bool                CheckOK_PatternMatchOffsetXLSL(const CAlgParam &Param);

	static bool                CheckOK_PatternMatchOffsetY(const CAlgParam &Param);
	static bool                CheckOK_PatternMatchOffsetYUSL(const CAlgParam &Param);
	static bool                CheckOK_PatternMatchOffsetYLSL(const CAlgParam &Param);

	static bool                CheckOK_PatternMatchOffsetL(const CAlgParam &Param);
	static bool                CheckOK_PatternMatchOffsetLUSL(const CAlgParam &Param);
	static bool                CheckOK_PatternMatchOffsetLLSL(const CAlgParam &Param);

	static bool                CheckOK_PatternMatchOffsetA(const CAlgParam &Param);
	static bool                CheckOK_PatternMatchOffsetAUSL(const CAlgParam &Param);
	static bool                CheckOK_PatternMatchOffsetALSL(const CAlgParam &Param);

	static bool                CheckOK_PatternMatchSkewAngle(const CAlgParam &Param);
	static bool                CheckOK_PatternMatchSkewAngleUSL(const CAlgParam &Param);
	static bool                CheckOK_PatternMatchSkewAngleLSL(const CAlgParam &Param);

	static bool                CheckOK_PatternMatchScale(const CAlgParam &Param);
	static bool                CheckOK_PatternMatchScaleUSL(const CAlgParam &Param);
	static bool                CheckOK_PatternMatchScaleLSL(const CAlgParam &Param);

	static bool                CheckOK_PatternMatchScore(const CAlgParam &Param);
	static bool                CheckOK_PatternMatchScoreUSL(const CAlgParam &Param);
	static bool                CheckOK_PatternMatchScoreLSL(const CAlgParam &Param);
	//---------------------------------------------------------------------------------//		
	//演算法單步計算-亮度比例
	static bool                CheckOK_BrightRatioTolerance(const TALG_PARAM_BRIGHT_RATIO &Param);
	static bool                CheckOK_BrightRatioToleranceUSL(const TALG_PARAM_BRIGHT_RATIO &Param);
	static bool                CheckOK_BrightRatioToleranceLSL(const TALG_PARAM_BRIGHT_RATIO &Param);

	static bool                CheckOK_BrightRatioLimit(const TALG_PARAM_BRIGHT_RATIO &Param);
	static bool                CheckOK_BrightRatioLimitMin(const TALG_PARAM_BRIGHT_RATIO &Param);
	static bool                CheckOK_BrightRatioLimitMax(const TALG_PARAM_BRIGHT_RATIO &Param);	

	static bool                CheckOK_BrightRatioRatio(const TALG_PARAM_BRIGHT_RATIO &Param);
	static bool                CheckOK_BrightRatioRatioUSL(const TALG_PARAM_BRIGHT_RATIO &Param);
	static bool                CheckOK_BrightRatioRatioLSL(const TALG_PARAM_BRIGHT_RATIO &Param);

	static bool                CheckOK_BrightRatioRange(const TALG_PARAM_BRIGHT_RATIO &Param);
	static bool                CheckOK_BrightRatioRangeUSL(const TALG_PARAM_BRIGHT_RATIO &Param);
	static bool                CheckOK_BrightRatioRangeLSL(const TALG_PARAM_BRIGHT_RATIO &Param);

	static bool                CheckOK_BrightRatioContrast(const TALG_PARAM_BRIGHT_RATIO &Param);
	static bool                CheckOK_BrightRatioContrastUSL(const TALG_PARAM_BRIGHT_RATIO &Param);
	static bool                CheckOK_BrightRatioContrastLSL(const TALG_PARAM_BRIGHT_RATIO &Param);

	static bool                CheckOK_BrightRatioLineX(const TALG_PARAM_BRIGHT_RATIO &Param);
	static bool                CheckOK_BrightRatioLineXUSL(const TALG_PARAM_BRIGHT_RATIO &Param);
	static bool                CheckOK_BrightRatioLineXLSL(const TALG_PARAM_BRIGHT_RATIO &Param);

	static bool                CheckOK_BrightRatioLineY(const TALG_PARAM_BRIGHT_RATIO &Param);
	static bool                CheckOK_BrightRatioLineYUSL(const TALG_PARAM_BRIGHT_RATIO &Param);
	static bool                CheckOK_BrightRatioLineYLSL(const TALG_PARAM_BRIGHT_RATIO &Param);
	//---------------------------------------------------------------------------------//		
	//演算法單步計算-外部連接
	static bool                CheckOK_OuterShort_R(const TALG_PARAM_OUTER_SHORT &Param);
	static bool                CheckOK_OuterShortUSL_R(const TALG_PARAM_OUTER_SHORT &Param);
	static bool                CheckOK_OuterShortLSL_R(const TALG_PARAM_OUTER_SHORT &Param);
	static bool                CheckOK_OuterShort_T(const TALG_PARAM_OUTER_SHORT &Param);
	static bool                CheckOK_OuterShortUSL_T(const TALG_PARAM_OUTER_SHORT &Param);
	static bool                CheckOK_OuterShortLSL_T(const TALG_PARAM_OUTER_SHORT &Param);
	static bool                CheckOK_OuterShort_L(const TALG_PARAM_OUTER_SHORT &Param);
	static bool                CheckOK_OuterShortUSL_L(const TALG_PARAM_OUTER_SHORT &Param);
	static bool                CheckOK_OuterShortLSL_L(const TALG_PARAM_OUTER_SHORT &Param);
	static bool                CheckOK_OuterShort_B(const TALG_PARAM_OUTER_SHORT &Param);
	static bool                CheckOK_OuterShortUSL_B(const TALG_PARAM_OUTER_SHORT &Param);
	static bool                CheckOK_OuterShortLSL_B(const TALG_PARAM_OUTER_SHORT &Param);
	//---------------------------------------------------------------------------------//	
	//演算法單步計算-區塊數量
	static bool                CheckOK_BlobCount(const TALG_PARAM_BLOB_COUNT &Param);
	static bool                CheckOK_BlobCountUSL(const TALG_PARAM_BLOB_COUNT &Param);
	static bool                CheckOK_BlobCountLSL(const TALG_PARAM_BLOB_COUNT &Param);
	//---------------------------------------------------------------------------------//	
	//演算法單步計算-字元驗證
	static bool                CheckOK_CharVerify(const TALG_PARAM_CHAR_VERIFY &Param);
	static bool                CheckOK_CharVerifyUSL(const TALG_PARAM_CHAR_VERIFY &Param);
	static bool                CheckOK_CharVerifyLSL(const TALG_PARAM_CHAR_VERIFY &Param);
	//---------------------------------------------------------------------------------//		
	//演算法單步計算-色環電阻
	static bool                CheckOK_ColorCode(const TALG_PARAM_COLOR_CODE &Param);
	static bool                CheckOK_ColorCodeUSL(const TALG_PARAM_COLOR_CODE &Param);
	static bool                CheckOK_ColorCodeLSL(const TALG_PARAM_COLOR_CODE &Param);
	//---------------------------------------------------------------------------------//			
	//演算法單步計算-條碼驗證
	static bool                CheckOK_BarcodeVerify(const TALG_PARAM_BARCODE_RECOGNIZE &Param);
	static bool                CheckOK_BarcodeVerifyUSL(const TALG_PARAM_BARCODE_RECOGNIZE &Param);
	static bool                CheckOK_BarcodeVerifyLSL(const TALG_PARAM_BARCODE_RECOGNIZE &Param);
	static bool                BuildBarcodeDecodeStepList(std::vector<ALG_BARCODE_STEP_MODE> &List);
	//---------------------------------------------------------------------------------//			
	//演算法單步計算-物件量測
	static bool                CheckOK_ObjectMeasureSizeX(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureSizeXDiff(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureSizeXDiffUSL(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureSizeXDiffLSL(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureSizeXRatio(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureSizeXRatioUSL(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureSizeXRatioLSL(const TALG_PARAM_OBJECT_MEASURE &Param);

	static bool                CheckOK_ObjectMeasureSizeY(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureSizeYDiff(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureSizeYDiffUSL(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureSizeYDiffLSL(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureSizeYRatio(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureSizeYRatioUSL(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureSizeYRatioLSL(const TALG_PARAM_OBJECT_MEASURE &Param);

	static bool                CheckOK_ObjectMeasureHeight(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureHeightDiff(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureHeightDiffUSL(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureHeightDiffLSL(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureHeightRatio(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureHeightRatioUSL(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureHeightRatioLSL(const TALG_PARAM_OBJECT_MEASURE &Param);

	static bool                CheckOK_ObjectMeasureArea(const TALG_PARAM_OBJECT_MEASURE &Param);	
	static bool                CheckOK_ObjectMeasureAreaRatioUSL(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureAreaRatioLSL(const TALG_PARAM_OBJECT_MEASURE &Param);

	static bool                CheckOK_ObjectMeasureVolume(const TALG_PARAM_OBJECT_MEASURE &Param);	
	static bool                CheckOK_ObjectMeasureVolumeRatioUSL(const TALG_PARAM_OBJECT_MEASURE &Param);
	static bool                CheckOK_ObjectMeasureVolumeRatioLSL(const TALG_PARAM_OBJECT_MEASURE &Param);
	//---------------------------------------------------------------------------------//	
	static bool                CheckOK_ShapeVerifyOuterTol(const TALG_PARAM_SHAPE_VERIFY &Param);
	static bool                CheckOK_ShapeVerifyInnerTol(const TALG_PARAM_SHAPE_VERIFY &Param);
	static bool                CheckOK_ShapeVerifyRangeTol(const TALG_PARAM_SHAPE_VERIFY &Param);
	static bool                CheckOK_ShapeVerifyErrorTol(const TALG_PARAM_SHAPE_VERIFY &Param);
	//---------------------------------------------------------------------------------//	
	static bool                CheckOK_AngleMeasureTolUSL(const TALG_PARAM_ANGLE_MEASURE &Param);
	static bool                CheckOK_AngleMeasureTolLSL(const TALG_PARAM_ANGLE_MEASURE &Param);	
	//---------------------------------------------------------------------------------//	
	//演算法單步計算-像素比較
	static bool                CheckOK_PixelCompare(const TALG_PARAM_PIXEL_COMPARE &Param);
	static bool                CheckOK_PixelCompareUSL(const TALG_PARAM_PIXEL_COMPARE &Param);
	static bool                CheckOK_PixelCompareLSL(const TALG_PARAM_PIXEL_COMPARE &Param);
	//---------------------------------------------------------------------------------//			
	//演算法單步計算-焊接檢測
	static bool                CheckOK_SolderWettingSectorAngle(const TALG_PARAM_SOLDER_WETTING &Param);
	static bool                CheckOK_SolderWettingSectorAngleUSL(const TALG_PARAM_SOLDER_WETTING &Param);
	static bool                CheckOK_SolderWettingSectorAngleLSL(const TALG_PARAM_SOLDER_WETTING &Param);
	//---------------------------------------------------------------------------------//	
	//演算法單步計算-群組比較
	static bool                CheckOK_GroupCompare2DGray(const TALG_PARAM_GROUP_COMPARE &Param);
	static bool                CheckOK_GroupCompare2DGrayUSL(const TALG_PARAM_GROUP_COMPARE &Param);
	static bool                CheckOK_GroupCompare2DGrayLSL(const TALG_PARAM_GROUP_COMPARE &Param);

	static bool                CheckOK_GroupCompare3DHeight(const TALG_PARAM_GROUP_COMPARE &Param);
	static bool                CheckOK_GroupCompare3DHeightUSL(const TALG_PARAM_GROUP_COMPARE &Param);
	static bool                CheckOK_GroupCompare3DHeightLSL(const TALG_PARAM_GROUP_COMPARE &Param);

	static bool                CheckOK_GroupCompareTiltAngle(const TALG_PARAM_GROUP_COMPARE &Param);
	static bool                CheckOK_GroupCompareTiltAngleUSL(const TALG_PARAM_GROUP_COMPARE &Param);
	static bool                CheckOK_GroupCompareTiltAngleLSL(const TALG_PARAM_GROUP_COMPARE &Param);
	//---------------------------------------------------------------------------------//		
	static bool                CheckOK_AngleMeasureTolUSL(const TALG_PARAM_MEASURE_SIP &Param);
	static bool                CheckOK_AngleMeasureTolLSL(const TALG_PARAM_MEASURE_SIP &Param);
private:
	//---------------------------------------------------------------------------------//	
	ALG_TYPE                   m_AlgType;                    //演算法-演算法樣式
	int                        m_AlgGroupID;                 //演算法-演算法群組編號
	CAOIWnd*                   m_AlgWndPtr;                  //演算法-檢測框指標		
	CAlgBinaryParam            m_AlgMaskBinParam;            //演算法-遮罩二值化參數-輔助二值化
	CAlgBinaryParam            m_AlgImageBinParam;           //演算法-影像二值化參數-主要影像
	BIN_PARAM_BELONG_TO        m_AlgBinParamActived;         //演算法-作業中的二值化參數	
	//---------------------------------------------------------------------------------//	
	bool                       m_AlgBaseValueEnabled;        //演算法-基準值啟用
	int                        m_AlgBaseValueGroupID;        //演算法-基準值群組編號
	double                     m_AlgBaseValueReading;        //演算法-基準值讀值
	//---------------------------------------------------------------------------------//	
	//Pattern Match Param
	bool                        m_AlgPatternTestAll;          //演算法-樣板每個都測試
	unsigned int                m_AlgPatternCount;            //演算法-樣板圖數量
	int                         m_AlgPatternPolarity;         //演算法-樣板圖方向
	unsigned int                m_AlgPatternResultIndex;      //演算法-樣板圖結果引數	
	double                      m_AlgPatternSimilarityUSL;    //演算法-樣板相似度上限
	double                      m_AlgPatternSimilarityLSL;    //演算法-樣板相似度下限
	double                      m_AlgPatternSimilarityReading;//演算法-樣板相似度讀值
	double                      m_AlgPatternAngleExpand;      //演算法-樣板角度外擴值
	double                      m_AlgPatternScaleExpand;      //演算法-樣板縮放外擴值	
	bool                        m_AlgPatternScaleIsotropic;   //演算法-樣板縮放等方向性
	int                         m_AlgPatternMinReducedArea;   //演算法-樣板最小保留面積	
	int                         m_AlgPatternFinalReduction;   //演算法-樣板最末殘餘層數	
	bool                        m_AlgPatternAdvancedLearning; //演算法-樣板進階學習
	bool                        m_AlgPatternFileUsed;         //演算法-是否使用樣板圖檔案		
	CString                     m_AlgPatternFolder;           //演算法-樣板圖資料夾	
	std::vector<CPatternParam>  m_AlgPatternParamList;       //演算法-樣板圖二值化列表
	//---------------------------------------------------------------------------------//
	TALG_PARAM_BRIGHT_RATIO    m_AlgParamBrightRatio;        //演算法-亮度比例參數	
	TALG_PARAM_OUTER_SHORT     m_AlgParamOuterShort;         //演算法-外部短路參數
	TALG_PARAM_BLOB_COUNT      m_AlgParamBlobCount;          //演算法-區塊數量參數
	TALG_PARAM_BODY_TILT       m_AlgParamBodyTilt;           //演算法-本體傾斜參數
	TALG_PARAM_MODEL_MATCH     m_AlgParamModelMatch;         //演算法-模板匹配參數
	TALG_PARAM_IMAGE_MATCH     m_AlgParamImageMatch;         //演算法-影像匹配參數
	TALG_PARAM_CHAR_VERIFY     m_AlgParamCharVerify;         //演算法-文字驗證參數
	TALG_PARAM_GROUP_COMPARE   m_AlgParamGroupCompare;       //演算法-群組比較參數
	TALG_PARAM_BARCODE_RECOGNIZE m_AlgParamBarcodeRecognize; //演算法-條碼辨識
	TALG_PARAM_OBJECT_MEASURE  m_AlgParamObjectMeasure;      //演算法-物體量測
	TALG_PARAM_COLOR_CODE      m_AlgParamColorCode;          //演算法-色碼檢測
	TALG_PARAM_FD_MATCH        m_AlgParamFdMatch;            //演算法-定位點搜尋
	TALG_PARAM_EDGE_SEARCH     m_AlgParamEdgeSearch;         //演算法-邊緣搜尋
	TALG_PARAM_SHAPE_VERIFY    m_AlgParamShapeVerify;        //演算法-外形驗證
	TALG_PARAM_ANGLE_MEASURE   m_AlgParamAngleMeasure;       //演算法-角度量測
	TALG_PARAM_PIXEL_COMPARE   m_AlgParamPixelCompare;       //演算法-像素比較
	TALG_PARAM_IPC_PRODUCT     m_AlgParamIPC;                // IPC產品
	TALG_PARAM_RESIN_HEIGHT	   m_AlgParamResinHight;		 // Resin Tin高度
	TALG_PARAM_WIRE_WIDTH	   m_AlgParamWireWidth;		     // 金線寬度
	TALG_PARAM_AI_MODEL        m_AlgParamAiModel;            //演算法-AI模型	
	TALG_PARAM_SOLDER_WETTING  m_AlgParamSolderWetting;      //演算法-焊錫焊接		
	TALG_PARAM_MEASURE_BLACK_GLUE m_AlgParamMeasureBlackGlue;//演算法-量測黑膠-軍達
	TALG_PARAM_MEASURE_FLUX_AREA m_AlgParamMeasureFluxArea;  //演算法-量測Flux面積-軍達
	TALG_PARAM_MEASURE_CPU_PIN m_AlgParamMeasureCpuPin;      //演算法-量測CPU接腳-軍達
	TALG_PARAM_MEASURE_SIP     m_AlgParamMeasureSIP;         //演算法-量測SIP-Alan
	TALG_PARAM_MEASURE_CONNECTOR m_AlgParamMeasureConnector; //演算法-量測Connector-Alan
	//---------------------------------------------------------------------------------//
	//演算法偏移量參數
	bool                       m_AlgOffsetXEnabled;          //演算法-偏移X-啟用
	double                     m_AlgOffsetXUSL;              //演算法-偏移X 最大偏移量
	double                     m_AlgOffsetXLSL;              //演算法-偏移X 最小偏移量
	bool                       m_AlgOffsetYEnabled;          //演算法-偏移Y-啟用
	double                     m_AlgOffsetYUSL;              //演算法-偏移Y 最大偏移量
	double                     m_AlgOffsetYLSL;              //演算法-偏移Y 最小偏移量
	bool                       m_AlgOffsetLEnabled;          //演算法-偏移L-啟用-sqrt(XX+YY)
	double                     m_AlgOffsetLUSL;              //演算法-偏移L 最大偏移量
	double                     m_AlgOffsetLLSL;              //演算法-偏移L 最小偏移量
	bool                       m_AlgOffsetAEnabled;          //演算法-偏移角-啟用
	double                     m_AlgOffsetAUSL;              //演算法-偏移角 最大偏移量
	double                     m_AlgOffsetALSL;              //演算法-偏移角 最小偏移量
	//---------------------------------------------------------------------------------//
	bool                       m_AlgSkewEnabled;             //演算法-角度-啟用
	double                     m_AlgSkewUSL;                 //演算法-角度 最大偏移量
	double                     m_AlgSkewLSL;                 //演算法-角度 最小偏移量	
	bool                       m_AlgScaleEnabled;            //演算法-縮放 啟用
	double                     m_AlgScaleUSL;                //演算法-縮放 最大縮放值
	double                     m_AlgScaleLSL;                //演算法-縮放 最小縮放值

	double                     m_AlgImageOffsetX;            //演算法-影像偏移X 讀值
	double                     m_AlgImageOffsetY;            //演算法-影像偏移Y 讀值

	double                     m_AlgOffsetXReading;          //演算法-Cad偏移X 讀值
	double                     m_AlgOffsetYReading;          //演算法-Cad偏移Y 讀值
	double                     m_AlgOffsetLReading;          //演算法-Cad偏移L 讀值
	double                     m_AlgOffsetAReading;          //演算法-Cad偏移角 讀值
	double                     m_AlgSkewReading;             //演算法-Cad角度 讀值
	double                     m_AlgScaleXReading;           //演算法-Cad縮放X 讀值
	double                     m_AlgScaleYReading;           //演算法-Cad縮放X 讀值
	//---------------------------------------------------------------------------------//	
	CString                    m_AlgSaveDefectImageName;     //演算法-儲存瑕疵圖像-檔名
	bool                       m_AlgSaveDefectImageDone;     //演算法-儲存瑕疵圖像-已存
	bool                       m_AlgSaveDefectImageEnabled;  //演算法-儲存瑕疵圖像-啟用
	//---------------------------------------------------------------------------------//
	RESULT_ID                  m_AlgResultID;                //演算法-結果
	double                     m_AlgResultReading1;          //演算法-結果數據-1
	double                     m_AlgResultReading2;          //演算法-結果數據-2
	double                     m_AlgResultReading3;          //演算法-結果數據-3	
	CString                    m_AlgResultText;              //演算法-結果文字
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitAlgParam();
	void                       InitialAlgParam();
	void                       CloneAlgParam(const CAlgParam &Param);
	//---------------------------------------------------------------------------------//
	void                       SwapAlgParam(int &v1, int &v2);
	void                       SwapAlgParam(bool &v1, bool &v2);
	void                       SwapAlgParam(double &v1, double &v2);
	void                       SwapAlgParam(ALG_CALC_UNIT_MODE &v1, ALG_CALC_UNIT_MODE &v2);
	//---------------------------------------------------------------------------------//	
	void                       RotateAlgParam_BrightRatio(int AngleLabel);
	void                       RotateAlgParam_OuterShort(int AngleLabel);
	void                       RotateAlgParam_BlobCount(int AngleLabel);
	void                       RotateAlgParam_BodyTilt(int AngleLabel);
	void                       RotateAlgParam_ImageMatch(int AngleLabel);
	void                       RotateAlgParam_ObjectMeasure(int AngleLabel);
	//---------------------------------------------------------------------------------//
	CString                    GetAlgParamPlugInFilename(LPCTSTR AlgName) const;//取得演算法外掛檔名
	//---------------------------------------------------------------------------------//
	bool                       MemsetByRect(int ImageW, int ImageH, int ImageStep, const RECT &CalcRect, const RECT &MaskRect, unsigned char val, unsigned char *pImage);
	//---------------------------------------------------------------------------------//	
	bool                       BuildWndUniFrameList(const std::vector<TUNI_FRAME> &ModelUniFrameList, const RECT &RoiRect, std::vector<TUNI_FRAME> &WndUniFrameList);
	bool                       BuildAlgGrayImage(const CAlgBinaryParam &Param, const RECT &RoiRect, std::vector<TUNI_FRAME> &UniFrameList, IMAGE_SIZE &GrayW, IMAGE_SIZE &GrayH, IMAGE_SIZE &GrayStep, IMAGE_PTR &GrayPtr, bool &bCloned);//建立灰階影像
	bool                       GetInspectionImage(std::vector<TUNI_FRAME> &UniFrameList, unsigned int FrameUniqueID, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, MASK_PTR &MaskPtr, SPACE_PTR &SpacePtr);	
	bool                       ExecAlgInspection_BrightRatio(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);	
	bool                       ExecAlgInspection_OuterShort(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_BlobCount(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_BodyTilt(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_BarcodeRecognize(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_ModelMatch(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_ImageMatch(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_ImageMatch_v1(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_ImageMatch_v2(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_CharVerify(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_CharVerify_v1(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_CharVerify_v2(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_GroupCompare(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_ObjectMeasure(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_FdMatch(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_ColorCode(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);	
	bool                       ExecAlgInspection_ColorCodeImage(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_EdgeSearch(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_EdgeSearch_V47(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_EdgeSearch_V50(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_EdgeSearch_V61(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_ShapeVerify(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_AngleMeasure(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool				       ExecAlgInspection_IPC(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool					   ExecAlgInspection_PartAlign(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool					   ExecAlgInspection_ICLeadAlign(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool					   ExecAlgInspection_WidthRatio(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool					   ExecAlgInspection_Height(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool					   ExecAlgInspection_WireWidth(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_PixelCompare(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_PixelCompare_v1(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_PixelCompare_v2(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_AiModel(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);		
	bool                       ExecAlgInspection_SolderWetting(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList, bool bTestWnd);	
	bool                       ExecAlgInspection_MeasureBlackGlue(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList, bool bTestWnd);		
	bool                       ExecAlgInspection_MeasureFluxArea(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, const std::vector<TUNI_FRAME> &UniFrameList, bool bTestWnd);		
	bool                       ExecAlgInspection_MeasureCpuPin(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, const std::vector<TUNI_FRAME> &UniFrameList, bool bTestWnd);
	bool                       ExecAlgInspection_MeasureSIP(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
	bool                       ExecAlgInspection_MeasureConnector(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, const std::vector<TUNI_FRAME> &UniFrameList);
	//---------------------------------------------------------------------------------//
	bool                       BuildBodyTiltCellRectList(const RECT &RoiRect, ALG_TILE_CELL_MODE CellMode, std::vector<RECT> &CellList);
	//---------------------------------------------------------------------------------//
	bool                       WriteAlgParamFile_PatternParam(CAlgParam *AlgParamPtr, CAOIFileIO &FileIO);//儲存樣板參數參數
	bool                       WriteAlgParamFile_BrightRatio(const TALG_PARAM_BRIGHT_RATIO &brParam, CAOIFileIO &FileIO);//儲存亮度比例參數
	bool                       WriteAlgParamFile_OuterShort(const TALG_PARAM_OUTER_SHORT &osParam, CAOIFileIO &FileIO);//儲存外接短路參數
	bool                       WriteAlgParamFile_BlobCount(const TALG_PARAM_BLOB_COUNT &blobParam, CAOIFileIO &FileIO);//儲存區塊數量參數
	bool                       WriteAlgParamFile_ModelMatch(const TALG_PARAM_MODEL_MATCH &mmParam, CAOIFileIO &FileIO);//儲存模板匹配參數
	bool                       WriteAlgParamFile_ImageMatch(const TALG_PARAM_IMAGE_MATCH &imParam, CAOIFileIO &FileIO);//儲存影像匹配參數
	bool                       WriteAlgParamFile_CharVerify(const TALG_PARAM_CHAR_VERIFY &cvParam, CAOIFileIO &FileIO);//儲存字元驗證參數
	bool                       WriteAlgParamFile_GroupCompare(const TALG_PARAM_GROUP_COMPARE &gcParam, CAOIFileIO &FileIO);//儲存群組比較參數
	bool                       WriteAlgParamFile_BarcodeRecognize(const TALG_PARAM_BARCODE_RECOGNIZE &barParam, CAOIFileIO &FileIO);//儲存條碼辨識參數
	bool                       WriteAlgParamFile_ColorCode(const TALG_PARAM_COLOR_CODE &ccParam, CAOIFileIO &FileIO);//儲存色碼檢測參數
	bool                       WriteAlgParamFile_FdMatch(const TALG_PARAM_FD_MATCH &fdParam, CAOIFileIO &FileIO);//儲存定位點匹配參數
	bool                       WriteAlgParamFile_EdgeSearch(const TALG_PARAM_EDGE_SEARCH &esParam, CAOIFileIO &FileIO);//儲存邊緣搜尋參數
	bool                       WriteAlgParamFile_ObjectMeasure(const TALG_PARAM_OBJECT_MEASURE &omParam, CAOIFileIO &FileIO);//儲存物件量測參數
	bool                       WriteAlgParamFile_ShapeVerify(const TALG_PARAM_SHAPE_VERIFY &svParam, CAOIFileIO &FileIO);//儲存外形驗證參數
	bool                       WriteAlgParamFile_AngleMeasure(const TALG_PARAM_ANGLE_MEASURE &amParam, CAOIFileIO &FileIO);//儲存角度量測參數	
	bool                       WriteAlgParamFile_PixelCompare(const TALG_PARAM_PIXEL_COMPARE &pcParam, CAOIFileIO &FileIO);//儲存像素比較參數
	bool					   WriteAlgParamFile_IPC(const TALG_PARAM_IPC_PRODUCT &Param, CAOIFileIO &FileIO);//儲存IPC檢測參數
	bool					   WriteAlgParamFile_Height(const TALG_PARAM_RESIN_HEIGHT &Param, CAOIFileIO &FileIO);
	bool					   WriteAlgParamFile_Wire(const TALG_PARAM_WIRE_WIDTH &Param, CAOIFileIO &FileIO);
	bool					   WriteAlgParamFile_AiModel(const TALG_PARAM_AI_MODEL &Param, CAOIFileIO &FileIO);//儲存AI模型參數	
	bool                       WriteAlgParamFile_SolderWetting(const TALG_PARAM_SOLDER_WETTING &Param, CAOIFileIO &FileIO);//儲存焊接檢測參數
	bool                       WriteAlgParamFile_MeasureBlackGlue(const TALG_PARAM_MEASURE_BLACK_GLUE &Param, CAOIFileIO &FileIO);//儲存量測黑膠參數-軍達	
	bool                       WriteAlgParamFile_MeasureFluxArea(const TALG_PARAM_MEASURE_FLUX_AREA &Param, CAOIFileIO &FileIO);//儲存量測Flux面積參數-軍達	
	bool                       WriteAlgParamFile_MeasureCpuPin(const TALG_PARAM_MEASURE_CPU_PIN &Param, CAOIFileIO &FileIO);//儲存量測CPU接腳參數-軍達	
	bool                       WriteAlgParamFile_MeasureSIP(const TALG_PARAM_MEASURE_SIP &Param, CAOIFileIO &FileIO);//儲存量測SIP
	bool                       WriteAlgParamFile_MeasureConnector(const TALG_PARAM_MEASURE_CONNECTOR &Param, CAOIFileIO &FileIO);//儲存量測Connector
	//---------------------------------------------------------------------------------//	
	bool                       ReadAlgParamFile_PatternParam(CAlgParam *AlgParamPtr, CAOIFileIO &FileIO);//載入樣板參數參數
	bool                       ReadAlgParamFile_BrightRatio(TALG_PARAM_BRIGHT_RATIO &brParam, CAOIFileIO &FileIO);//載入亮度比例參數
	bool                       ReadAlgParamFile_OuterShort(TALG_PARAM_OUTER_SHORT &osParam, CAOIFileIO &FileIO);//載入外接短路參數
	bool                       ReadAlgParamFile_BlobCount(TALG_PARAM_BLOB_COUNT &blobParam, CAOIFileIO &FileIO);//載入區塊數量參數
	bool                       ReadAlgParamFile_ModelMatch(TALG_PARAM_MODEL_MATCH &mmParam, CAOIFileIO &FileIO);//載入模板匹配參數
	bool                       ReadAlgParamFile_ImageMatch(TALG_PARAM_IMAGE_MATCH &imParam, CAOIFileIO &FileIO);//載入影像匹配參數
	bool                       ReadAlgParamFile_CharVerify(TALG_PARAM_CHAR_VERIFY &cvParam, CAOIFileIO &FileIO);//載入字元驗證參數
	bool                       ReadAlgParamFile_GroupCompare(TALG_PARAM_GROUP_COMPARE &gcParam, CAOIFileIO &FileIO);//載入群組比較參數
	bool                       ReadAlgParamFile_BarcodeRecognize(TALG_PARAM_BARCODE_RECOGNIZE &barParam, CAOIFileIO &FileIO);//載入條碼辨識參數
	bool                       ReadAlgParamFile_ColorCode(TALG_PARAM_COLOR_CODE &ccParam, CAOIFileIO &FileIO);//載入色碼檢測參數
	bool                       ReadAlgParamFile_FdMatch(TALG_PARAM_FD_MATCH &fdParam, CAOIFileIO &FileIO);//載入定位點匹配參數
	bool                       ReadAlgParamFile_EdgeSearch(TALG_PARAM_EDGE_SEARCH &esParam, CAOIFileIO &FileIO);//載入邊緣搜尋參數
	bool                       ReadAlgParamFile_ObjectMeasure(TALG_PARAM_OBJECT_MEASURE &omParam, CAOIFileIO &FileIO);//載入物件量測參數
	bool                       ReadAlgParamFile_ShapeVerify(TALG_PARAM_SHAPE_VERIFY &svParam, CAOIFileIO &FileIO);//載入外形驗證參數
	bool                       ReadAlgParamFile_AngleMeasure(TALG_PARAM_ANGLE_MEASURE &amParam, CAOIFileIO &FileIO);//載入角度量測參數
	bool                       ReadAlgParamFile_PixelCompare(TALG_PARAM_PIXEL_COMPARE &pcParam, CAOIFileIO &FileIO);//載入像素比較參數	
	bool					   ReadAlgParamFile_IPC(TALG_PARAM_IPC_PRODUCT &Param, CAOIFileIO &FileIO);//載入IPC檢測參數	
	bool					   ReadAlgParamFile_Height(TALG_PARAM_RESIN_HEIGHT &Param, CAOIFileIO &FileIO);
	bool					   ReadAlgParamFile_Wire(TALG_PARAM_WIRE_WIDTH &Param, CAOIFileIO &FileIO);
	bool					   ReadAlgParamFile_AiModel(TALG_PARAM_AI_MODEL &Param, CAOIFileIO &FileIO);//載入AI模型參數	
	bool                       ReadAlgParamFile_SolderWetting(TALG_PARAM_SOLDER_WETTING &Param, CAOIFileIO &FileIO);//載入焊接檢測參數
	bool                       ReadAlgParamFile_MeasureBlackGlue(TALG_PARAM_MEASURE_BLACK_GLUE &Param, CAOIFileIO &FileIO);//載入量測黑膠參數-軍達	
	bool                       ReadAlgParamFile_MeasureFluxArea(TALG_PARAM_MEASURE_FLUX_AREA &Param, CAOIFileIO &FileIO);//載入量測Flux面積參數-軍達	
	bool                       ReadAlgParamFile_MeasureCpuPin(TALG_PARAM_MEASURE_CPU_PIN &Param, CAOIFileIO &FileIO);//載入量測CPU接腳參數-軍達	
	bool                       ReadAlgParamFile_MeasureSIP(TALG_PARAM_MEASURE_SIP &Param, CAOIFileIO &FileIO);//載入量測SIP接腳參數
	bool                       ReadAlgParamFile_MeasureConnector(TALG_PARAM_MEASURE_CONNECTOR &Param, CAOIFileIO &FileIO);//載入量測SIP接腳參數
	//---------------------------------------------------------------------------------//
	bool                       WriteAlgSpcFile_AIModel_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr);
	bool                       WriteAlgSpcFile_Parameter_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr);	
	bool                       WriteAlgSpcFile_GroupCompare_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr);
	//---------------------------------------------------------------------------------//
	unsigned int               GetAlgFrameIndex(unsigned int UniqueID, unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CAlgParam();
	CAlgParam(const CAlgParam &Param);
	virtual ~CAlgParam();
	CAlgParam&                 operator=(const CAlgParam &Param);
	//---------------------------------------------------------------------------------//	
	bool                       WriteAlgParamFile(CAOIFileIO &FileIO);//儲存演算法參數
	bool                       ReadAlgParamFile(CAOIFileIO &FileIO);//載入演算法參數
	//---------------------------------------------------------------------------------//
	bool                       WriteAlgSpcFile_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr);
	bool                       WriteAlgSpcFile_Parameter_JSON_ALG_ParaProperty(FILE *pfile);//0411_add
	//---------------------------------------------------------------------------------//
	bool                       BuildAlgParamStringList(LPCTSTR Title, ALG_TYPE AlgType, std::vector<CString> &strList) const;//建立演算法參數字串
	//---------------------------------------------------------------------------------//
	CString                    GetAlgDebugFolder() const;
	//---------------------------------------------------------------------------------//
	void                       SetAlgType(ALG_TYPE value) { m_AlgType = value; }
	ALG_TYPE                   GetAlgType() const { return m_AlgType; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgGroupID(int value) { m_AlgGroupID = value; }
	int                        GetAlgGroupID() const { return m_AlgGroupID; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgWndPtr(CAOIWnd *value) { m_AlgWndPtr = value; }
	CAOIWnd*                   GetAlgWndPtr() const { return m_AlgWndPtr; }
	//---------------------------------------------------------------------------------//
	//基準值啟用
	void                       SetAlgBaseValueEnabled(bool val) { m_AlgBaseValueEnabled = val; }
	bool                       GetAlgBaseValueEnabled() const { return m_AlgBaseValueEnabled; }
	//---------------------------------------------------------------------------------//
	//基準值群組編號
	void                       SetAlgBaseValueGroupID(int val) { m_AlgBaseValueGroupID = val; }
	int                        GetAlgBaseValueGroupID() const { return m_AlgBaseValueGroupID; }
	//---------------------------------------------------------------------------------//	
	//基準值讀值
	void                       SetAlgBaseValueReading(double val) { m_AlgBaseValueReading = val; }
	double                     GetAlgBaseValueReading() const { return m_AlgBaseValueReading; }
	//---------------------------------------------------------------------------------//		
	//演算法圖檔
	bool                       CheckAlgPatternFileUsed();
	bool                       GetAlgPatternFileUsed() const { return m_AlgPatternFileUsed; }
	void                       SetAlgPatternFileUsed(bool val) { m_AlgPatternFileUsed = val; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgPatternFolder(LPCTSTR value) { m_AlgPatternFolder = value; }
	LPCTSTR                    GetAlgPatternFolder() const { return m_AlgPatternFolder; } 	
	//---------------------------------------------------------------------------------//	
	//演算法-樣板每個都測試
	void                       SetAlgPatternTestAll(bool val) { m_AlgPatternTestAll=val; }
	bool                       GetAlgPatternTestAll() const { return m_AlgPatternTestAll; }
	//---------------------------------------------------------------------------------//
	unsigned int               GetAlgPatternCount() const { return m_AlgPatternCount; }
	void                       SetAlgPatternCount(unsigned int val) { m_AlgPatternCount = val; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgPatternPolarity(int val) { m_AlgPatternPolarity=val; }
	int                        GetAlgPatternPolarity() const { return m_AlgPatternPolarity; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgPatternResultIndex(unsigned int val) { m_AlgPatternResultIndex=val; }
	unsigned int               GetAlgPatternResultIndex() const { return m_AlgPatternResultIndex; }
	//---------------------------------------------------------------------------------//	
	//演算法-樣板相似度上限
	void                       SetAlgPatternSimilarityUSL(double value) { m_AlgPatternSimilarityUSL = value; }
	double                     GetAlgPatternSimilarityUSL() const { return m_AlgPatternSimilarityUSL; } 	
	//---------------------------------------------------------------------------------//
	//演算法-樣板相似度下限
	void                       SetAlgPatternSimilarityLSL(double value) { m_AlgPatternSimilarityLSL = value; }
	double                     GetAlgPatternSimilarityLSL() const { return m_AlgPatternSimilarityLSL; } 	
	//---------------------------------------------------------------------------------//
	//演算法-樣板相似度讀值
	void                       SetAlgPatternSimilarityReading(double value) { m_AlgPatternSimilarityReading = value; }
	double                     GetAlgPatternSimilarityReading() const { return m_AlgPatternSimilarityReading; } 	
	//---------------------------------------------------------------------------------//		
	//演算法-樣板角度外擴值
	void                       SetAlgPatternAngleExpand(double value) { m_AlgPatternAngleExpand = value; }
	double                     GetAlgPatternAngleExpand() const { return m_AlgPatternAngleExpand; } 	
	//---------------------------------------------------------------------------------//
	//演算法-樣板縮放外擴值
	void                       SetAlgPatternScaleExpand(double value) { m_AlgPatternScaleExpand = value; }
	double                     GetAlgPatternScaleExpand() const { return m_AlgPatternScaleExpand; } 	
	//---------------------------------------------------------------------------------//	
	//演算法-樣板縮放等方向性
	void                       SetAlgPatternScaleIsotropic(bool value) { m_AlgPatternScaleIsotropic = value; }
	bool                       GetAlgPatternScaleIsotropic() const { return m_AlgPatternScaleIsotropic; } 		
	//---------------------------------------------------------------------------------//
	//影像匹配最小保留面積
	void                       SetAlgPatternMinReducedArea(int value) { m_AlgPatternMinReducedArea = value; }
	int                        GetAlgPatternMinReducedArea() const { return m_AlgPatternMinReducedArea; } 	
	//---------------------------------------------------------------------------------//
	//影像匹配最末殘餘層數
	void                       SetAlgPatternFinalReduction(int value) { m_AlgPatternFinalReduction = value; }
	int                        GetAlgPatternFinalReduction() const { return m_AlgPatternFinalReduction; } 	
	//---------------------------------------------------------------------------------//
	//影像匹配樣板進階學習
	void                       SetAlgPatternAdvancedLearning(bool value) { m_AlgPatternAdvancedLearning = value; }
	bool                       GetAlgPatternAdvancedLearning() const { return m_AlgPatternAdvancedLearning; } 	
	//---------------------------------------------------------------------------------//
	bool                       CheckAlgPatternMatrchInterpolate() const;//確認是否樣板需使用內插補正
	//bool                       SaveAlgImageFiles(LPCTSTR ImageFolder);//儲存演算法樣板圖片至專案樣板資料夾
	bool                       DeleteAlgPattern(unsigned int Index);//刪除演算法圖片
	bool                       ClearAlgPatternFiles();	//清除演算法圖片
	bool                       RemoveAlgPatternFolder();//	清除演算法圖片資料夾	
	bool                       LoadAlgPatternImage(unsigned int PatternIndex, BOX_TOWARD Toward, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr);	
	bool                       ReplaceAlgPatternImage(unsigned int PatternIndex, BOX_TOWARD Toward, IMAGE_SIZE BeforeW, IMAGE_SIZE BeforeH, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, LPCTSTR ModelFolder);
	bool                       AddAlgPatternImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const IMAGE_PTR ImagePtr, LPCTSTR ModelFolder, BOX_TOWARD Toward, int PolarityIdx, const CAlgBinaryParam &BinaryParam);	
	//---------------------------------------------------------------------------------//		
	size_t                     GetAlgPatternParamCount() const;
	CPatternParam*             GetAlgPatternParamPtr(size_t index, bool Check);	
	CPatternParam*             GetAlgPatternParamPtrByMaxPatRoiCount(BOX_TOWARD Toward, int PolarityIdx);//取得最多小框的樣板參數指標
	bool                       ClearAlgPatternParamList();//清除樣板參數列表
	bool                       AddAlgPatternParam(CPatternParam &PatParam);//加入樣板參數列表
	bool                       ReplaceAlgPatternParam(size_t index, CPatternParam &PatParam);//取代樣板參數列表
	bool                       GetAlgPatternParamPatTextList(std::vector<std::wstring> &List);//取得樣板參數內的樣板文字列表 
	//---------------------------------------------------------------------------------//
	bool                       CheckAlgParamUsing3D() const;//確認演算法使用3D
	//---------------------------------------------------------------------------------//
	bool                       CheckAlgShowSkewByAlgType(ALG_TYPE AlgType) const;
	bool                       CheckAlgShowScaleByAlgType(ALG_TYPE AlgType) const;	
	bool                       CheckAlgShowOffsetByAlgType(ALG_TYPE AlgType) const;		
	//---------------------------------------------------------------------------------//
	void                       SetAlgOffsetXEnabled(bool value) { m_AlgOffsetXEnabled = value; }
	bool                       GetAlgOffsetXEnabled() const { return m_AlgOffsetXEnabled; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgOffsetXUSL(double value) { m_AlgOffsetXUSL = value; }
	double                     GetAlgOffsetXUSL() const { return m_AlgOffsetXUSL; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgOffsetXLSL(double value) { m_AlgOffsetXLSL = value; }
	double                     GetAlgOffsetXLSL() const { return m_AlgOffsetXLSL; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgOffsetYEnabled(bool value) { m_AlgOffsetYEnabled = value; }
	bool                       GetAlgOffsetYEnabled() const { return m_AlgOffsetYEnabled; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgOffsetYUSL(double value) { m_AlgOffsetYUSL = value; }
	double                     GetAlgOffsetYUSL() const { return m_AlgOffsetYUSL; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgOffsetYLSL(double value) { m_AlgOffsetYLSL = value; }
	double                     GetAlgOffsetYLSL() const { return m_AlgOffsetYLSL; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgOffsetLEnabled(bool value) { m_AlgOffsetLEnabled = value; }
	bool                       GetAlgOffsetLEnabled() const { return m_AlgOffsetLEnabled; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgOffsetLUSL(double value) { m_AlgOffsetLUSL = value; }
	double                     GetAlgOffsetLUSL() const { return m_AlgOffsetLUSL; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgOffsetLLSL(double value) { m_AlgOffsetLLSL = value; }
	double                     GetAlgOffsetLLSL() const { return m_AlgOffsetLLSL; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgOffsetAEnabled(bool value) { m_AlgOffsetAEnabled = value; }
	bool                       GetAlgOffsetAEnabled() const { return m_AlgOffsetAEnabled; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgOffsetAUSL(double value) { m_AlgOffsetAUSL = value; }
	double                     GetAlgOffsetAUSL() const { return m_AlgOffsetAUSL; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgOffsetALSL(double value) { m_AlgOffsetALSL = value; }
	double                     GetAlgOffsetALSL() const { return m_AlgOffsetALSL; }
	//---------------------------------------------------------------------------------//
	bool                       CalcAlgOffsetL();//計算演算法XY偏移長度
	bool                       CheckAlgOffsetAngle(double Height);//確認演算法XY偏移角度
	bool                       CheckAlgOffset(double &SelfX, double &SelfY, double &SelfA, double &OhtersX, double &OhtersY, double &OhtersA, bool ChkDefect);
	//---------------------------------------------------------------------------------//
	//角度範圍
	double                     GetAlgSkewCalcRange() const;//取得角度計算的範圍
	bool                       CheckRotateImage(double Angle) const;//確認是否旋轉影像
	void                       SetAlgSkewEnabled(bool value) { m_AlgSkewEnabled = value; }
	bool                       GetAlgSkewEnabled() const { return m_AlgSkewEnabled; }	
	void                       SetAlgSkewUSL(double value) { m_AlgSkewUSL = value; }
	double                     GetAlgSkewUSL() const { return m_AlgSkewUSL; }	
	void                       SetAlgSkewLSL(double value) { m_AlgSkewLSL = value; }
	double                     GetAlgSkewLSL() const { return m_AlgSkewLSL; }	
	//---------------------------------------------------------------------------------//
	//縮放比例	
	bool                       GetAlgScaleCalcRange(double &Min, double &Max) const;//取得縮放比例的計算範圍
	void                       SetAlgScaleEnabled(bool value) { m_AlgScaleEnabled = value; }
	bool                       GetAlgScaleEnabled() const { return m_AlgScaleEnabled; }	
	void                       SetAlgScaleUSL(double value) { m_AlgScaleUSL = value; }
	double                     GetAlgScaleUSL() const { return m_AlgScaleUSL; }
	void                       SetAlgScaleLSL(double value) { m_AlgScaleLSL = value; }
	double                     GetAlgScaleLSL() const { return m_AlgScaleLSL; }	
	//---------------------------------------------------------------------------------//	
	void                       SetAlgImageOffsetX(double value) { m_AlgImageOffsetX = value; }
	double                     GetAlgImageOffsetX() const { return m_AlgImageOffsetX; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgImageOffsetY(double value) { m_AlgImageOffsetY = value; }
	double                     GetAlgImageOffsetY() const { return m_AlgImageOffsetY; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgOffsetXReading(double value) { m_AlgOffsetXReading = value; }
	double                     GetAlgOffsetXReading() const { return m_AlgOffsetXReading; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgOffsetYReading(double value) { m_AlgOffsetYReading = value; }
	double                     GetAlgOffsetYReading() const { return m_AlgOffsetYReading; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgOffsetLReading(double value) { m_AlgOffsetLReading = value; }
	double                     GetAlgOffsetLReading() const { return m_AlgOffsetLReading; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgOffsetAReading(double value) { m_AlgOffsetAReading = value; }
	double                     GetAlgOffsetAReading() const { return m_AlgOffsetAReading; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgSkewReading(double value) { m_AlgSkewReading = value; }
	double                     GetAlgSkewReading() const { return m_AlgSkewReading; }
	//---------------------------------------------------------------------------------//	
	void                       SetAlgScaleXReading(double value) { m_AlgScaleXReading = value; }
	double                     GetAlgScaleXReading() const { return m_AlgScaleXReading; }
	//---------------------------------------------------------------------------------//	
	void                       SetAlgScaleYReading(double value) { m_AlgScaleYReading = value; }
	double                     GetAlgScaleYReading() const { return m_AlgScaleYReading; }
	//---------------------------------------------------------------------------------//
	double                     GetAlgScaleMinReading() const { return MIN(m_AlgScaleXReading, m_AlgScaleYReading); }
	double                     GetAlgScaleMaxReading() const { return MAX(m_AlgScaleXReading, m_AlgScaleYReading); }	
	//---------------------------------------------------------------------------------//
	//演算法-儲存瑕疵圖像-檔名
	void                       SetAlgSaveDefectImageName(LPCTSTR value) { m_AlgSaveDefectImageName = value; }
	LPCTSTR                    GetAlgSaveDefectImageName() const { return m_AlgSaveDefectImageName; }	
	//演算法-儲存瑕疵圖像-已存
	void                       SetAlgSaveDefectImageDone(bool value) { m_AlgSaveDefectImageDone = value; }
	bool                       GetAlgSaveDefectImageDone() const { return m_AlgSaveDefectImageDone; }
	//演算法-儲存瑕疵圖像-啟用
	void                       SetAlgSaveDefectImageEnabled(bool value) { m_AlgSaveDefectImageEnabled = value; }
	bool                       GetAlgSaveDefectImageEnabled() const { return m_AlgSaveDefectImageEnabled; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgResultID(RESULT_ID value) { m_AlgResultID = value; }
	RESULT_ID                  GetAlgResultID() const { return m_AlgResultID; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgResultReading1(double value) { m_AlgResultReading1 = value; }
	double                     GetAlgResultReading1() const { return m_AlgResultReading1; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgResultReading2(double value) { m_AlgResultReading2 = value; }
	double                     GetAlgResultReading2() const { return m_AlgResultReading2; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgResultReading3(double value) { m_AlgResultReading3 = value; }
	double                     GetAlgResultReading3() const { return m_AlgResultReading3; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgResultText(LPCTSTR  value);// { m_AlgResultText = value; }
	LPCTSTR                    GetAlgResultText() const { return m_AlgResultText; }
	//---------------------------------------------------------------------------------//
	bool                       CheckAlgAISupported() const { return (ALG_CHAR_VERIFY==m_AlgType) ? true:false; }		
	//---------------------------------------------------------------------------------//
	bool                       ChangeAlgType(ALG_TYPE AlgType, bool bChkPatFolder);//變更演算法樣式
	//---------------------------------------------------------------------------------//
	void                       MirrorAlgParamXAxis();
	void                       MirrorAlgParamYAxis();
	void                       RotateAlgParam(double Angle);
	//---------------------------------------------------------------------------------//
	//演算法-作業中的二值化參數
	BIN_PARAM_BELONG_TO        GetAlgBinParamActived() const { return m_AlgBinParamActived; }
	void                       SetAlgBinParamActived(const BIN_PARAM_BELONG_TO val) { m_AlgBinParamActived = val; }	
	//---------------------------------------------------------------------------------//
	//操作中的2值化參數
	CAlgBinaryParam*           GetAlgActiveBinParamPtr();
	bool                       SetAlgActiveBinParam(const CAlgBinaryParam &BinParam);	
	//---------------------------------------------------------------------------------//
	//套用同一套2值化參數到相似WndRoi
	//bool                       CheckAlgActiveBinParamShared(const CAlgBinaryParam &BinParam);
	//bool                       SetAlgActiveBinParamShared(const CAlgBinaryParam &BinParam);
	//---------------------------------------------------------------------------------//
	//輔助遮罩的2值化參數
	bool                       CheckAlgMaskBinFrameUsed() const;
	CAlgBinaryParam&           GetAlgMaskBinParam() { return m_AlgMaskBinParam; }
	CAlgBinaryParam*           GetAlgMaskBinParamPtr() { return &m_AlgMaskBinParam; }	
	const CAlgBinaryParam&     GetAlgMaskBinParam() const { return m_AlgMaskBinParam; }
	void                       SetAlgMaskBinParam(const CAlgBinaryParam &value) 
	{ 
		m_AlgMaskBinParam = value; 
		m_AlgMaskBinParam.SetBinaryBelongToWho(BIN_PARAM_BELONG_TO_MASK_IMAGE);
	}	
	//---------------------------------------------------------------------------------//
	//演算法的2值化參數
	CAlgBinaryParam&           GetAlgImageBinParam() { return m_AlgImageBinParam; }
	CAlgBinaryParam*           GetAlgImageBinParamPtr() { return &m_AlgImageBinParam; }
	const CAlgBinaryParam&     GetAlgImageBinParam() const { return m_AlgImageBinParam; }
	void                       SetAlgImageBinParam(const CAlgBinaryParam &value) 
	{ 
		m_AlgImageBinParam = value; 
		m_AlgImageBinParam.SetBinaryBelongToWho(BIN_PARAM_BELONG_TO_ALG_IMAGE);
	}	
	//---------------------------------------------------------------------------------//	
	bool                       ExecAlgUniFrameBinary_Rect(CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, TUNI_FRAME UniFrame, int nAlign, MASK_PTR &MaskPtr);
	bool                       ExecAlgGrayEdge_Rect(CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR ImagePtr);
	bool                       ExecAlgGrayFilter_Rect(CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR ImagePtr);
	bool                       ExecAlgBinaryFilter_Rect(CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, MASK_PTR MaskPtr);
	bool                       ExecAlgImageGray_Rect(CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR FrameImagePtr, MASK_PTR FrameMaskPtr, SPACE_PTR FrameSpacePtr, int nAlign, IMAGE_PTR &GrayPtr);
	bool                       ExecAlgImageBinary_Rect(CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR FrameImagePtr, MASK_PTR FrameMaskPtr, SPACE_PTR FrameSpacePtr, int nAlign, MASK_PTR &MaskPtr);	
	bool                       ExecAlgImageBinary(CAlgBinaryParam &Param, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR FrameImagePtr, MASK_PTR FrameMaskPtr, SPACE_PTR FrameSpacePtr, int nAlign, IMAGE_PTR &GrayPtr, MASK_PTR &MaskPtr);
	bool                       ExecAlgImageBinary_Loc(CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR FrameImagePtr, MASK_PTR FrameMaskPtr, SPACE_PTR FrameSpacePtr, int nAlign, IMAGE_PTR &GrayPtr, MASK_PTR &MaskPtr);	
	bool                       ExecAlgUniFrameBinary(CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, const std::vector<TUNI_FRAME> &UniFrameList, IMAGE_SIZE &MaskW, IMAGE_SIZE &MaskH, IMAGE_SIZE &MaskStep, IMAGE_SIZE &MaskBitCount, MASK_PTR &MaskPtr, IMAGE_PTR &GrayPtr, bool bTestWnd);		
	bool                       ExecAlgUniFrameShapeMask(CAOIWnd *WndPtr, const RECT &WndRect, IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, IMAGE_SIZE MaskBitCount, MASK_PTR MaskPtr);//合併檢測框外型2值化
	bool                       ExecAlgUniFrameMaskBinary(CAOIModel *ModelPtr, CAlgBinaryParam &Param, const RECT &CalcRect, const RECT &MaskRect, std::vector<TUNI_FRAME> &UniFrameList, IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, IMAGE_SIZE MaskBitCount, MASK_PTR MaskPtr, bool bTestWnd);//合併檢算法遮罩2值化	
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamBrightRatio(const TALG_PARAM_BRIGHT_RATIO &value) { m_AlgParamBrightRatio = value; }
	TALG_PARAM_BRIGHT_RATIO&   GetAlgParamBrightRatio() { return m_AlgParamBrightRatio; }	
	const TALG_PARAM_BRIGHT_RATIO& GetAlgParamBrightRatio() const { return m_AlgParamBrightRatio; }	
	//---------------------------------------------------------------------------------//	
	void                       SetAlgParamOuterShort(const TALG_PARAM_OUTER_SHORT &value) { m_AlgParamOuterShort = value; }
	TALG_PARAM_OUTER_SHORT&    GetAlgParamOuterShort() { return m_AlgParamOuterShort; }	
	const TALG_PARAM_OUTER_SHORT& GetAlgParamOuterShort() const { return m_AlgParamOuterShort; }	
	//---------------------------------------------------------------------------------//	
	void                       SetAlgParamBlobCount(const TALG_PARAM_BLOB_COUNT &value) { m_AlgParamBlobCount = value; }
	TALG_PARAM_BLOB_COUNT&     GetAlgParamBlobCount() { return m_AlgParamBlobCount; }
	const TALG_PARAM_BLOB_COUNT&  GetAlgParamBlobCount() const { return m_AlgParamBlobCount; }
	//---------------------------------------------------------------------------------//	
	void                       SetAlgParamBodyTilt(const TALG_PARAM_BODY_TILT &value) { m_AlgParamBodyTilt = value; }
	TALG_PARAM_BODY_TILT&      GetAlgParamBodyTilt() { return m_AlgParamBodyTilt; }
	const TALG_PARAM_BODY_TILT&   GetAlgParamBodyTilt() const { return m_AlgParamBodyTilt; }
	//---------------------------------------------------------------------------------//	
	void                       SetAlgParamModelMatch(const TALG_PARAM_MODEL_MATCH &value) { m_AlgParamModelMatch = value; }
	TALG_PARAM_MODEL_MATCH&    GetAlgParamModelMatch() { return m_AlgParamModelMatch; }
	const TALG_PARAM_MODEL_MATCH& GetAlgParamModelMatch() const { return m_AlgParamModelMatch; }
	//---------------------------------------------------------------------------------//	
	void                       SetAlgParamImageMatch(const TALG_PARAM_IMAGE_MATCH &value) { m_AlgParamImageMatch = value; }
	TALG_PARAM_IMAGE_MATCH&    GetAlgParamImageMatch() { return m_AlgParamImageMatch; }
	const TALG_PARAM_IMAGE_MATCH& GetAlgParamImageMatch() const { return m_AlgParamImageMatch; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamCharVerify(const TALG_PARAM_CHAR_VERIFY &value) { m_AlgParamCharVerify = value; }
	TALG_PARAM_CHAR_VERIFY&    GetAlgParamCharVerify() { return m_AlgParamCharVerify; }
	const TALG_PARAM_CHAR_VERIFY& GetAlgParamCharVerify() const { return m_AlgParamCharVerify; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamGroupCompare(const TALG_PARAM_GROUP_COMPARE &value) { m_AlgParamGroupCompare = value; }
	TALG_PARAM_GROUP_COMPARE&  GetAlgParamGroupCompare() { return m_AlgParamGroupCompare; }
	const TALG_PARAM_GROUP_COMPARE&  GetAlgParamGroupCompare() const { return m_AlgParamGroupCompare; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamBarcodeRecognize(const TALG_PARAM_BARCODE_RECOGNIZE &value) { m_AlgParamBarcodeRecognize = value; }
	TALG_PARAM_BARCODE_RECOGNIZE& GetAlgParamBarcodeRecognize() { return m_AlgParamBarcodeRecognize; }
	const TALG_PARAM_BARCODE_RECOGNIZE& GetAlgParamBarcodeRecognize() const { return m_AlgParamBarcodeRecognize; }
	//---------------------------------------------------------------------------------//	
	void                       SetAlgParamObjectMeasure(const TALG_PARAM_OBJECT_MEASURE &value) { m_AlgParamObjectMeasure = value; }
	TALG_PARAM_OBJECT_MEASURE& GetAlgParamObjectMeasure() { return m_AlgParamObjectMeasure; }
	const TALG_PARAM_OBJECT_MEASURE& GetAlgParamObjectMeasure() const { return m_AlgParamObjectMeasure; }
	//---------------------------------------------------------------------------------//	
	void                       SetAlgParamColorCode(const TALG_PARAM_COLOR_CODE &value) { m_AlgParamColorCode = value; }
	TALG_PARAM_COLOR_CODE&     GetAlgParamColorCode() { return m_AlgParamColorCode; }
	const TALG_PARAM_COLOR_CODE&  GetAlgParamColorCode() const { return m_AlgParamColorCode; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamFdMatch(const TALG_PARAM_FD_MATCH &value) { m_AlgParamFdMatch = value; }
	TALG_PARAM_FD_MATCH&       GetAlgParamFdMatch() { return m_AlgParamFdMatch; }
	const TALG_PARAM_FD_MATCH&    GetAlgParamFdMatch() const { return m_AlgParamFdMatch; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamEdgeSearch(const TALG_PARAM_EDGE_SEARCH &value) { m_AlgParamEdgeSearch = value; }
	TALG_PARAM_EDGE_SEARCH&    GetAlgParamEdgeSearch() { return m_AlgParamEdgeSearch; }
	const TALG_PARAM_EDGE_SEARCH& GetAlgParamEdgeSearch() const { return m_AlgParamEdgeSearch; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamShapeVerify(const TALG_PARAM_SHAPE_VERIFY &value) { m_AlgParamShapeVerify = value; }
	TALG_PARAM_SHAPE_VERIFY&   GetAlgParamShapeVerify() { return m_AlgParamShapeVerify; }
	const TALG_PARAM_SHAPE_VERIFY& GetAlgParamShapeVerify() const { return m_AlgParamShapeVerify; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamAngleMeasure(const TALG_PARAM_ANGLE_MEASURE &value) { m_AlgParamAngleMeasure = value; }
	TALG_PARAM_ANGLE_MEASURE&  GetAlgParamAngleMeasure() { return m_AlgParamAngleMeasure; }
	const TALG_PARAM_ANGLE_MEASURE& GetAlgParamAngleMeasure() const { return m_AlgParamAngleMeasure; }
	//---------------------------------------------------------------------------------//	
	void                       SetAlgParamPixelCompare(const TALG_PARAM_PIXEL_COMPARE &value) { m_AlgParamPixelCompare = value; }
	TALG_PARAM_PIXEL_COMPARE&  GetAlgParamPixelCompare() { return m_AlgParamPixelCompare; }
	const TALG_PARAM_PIXEL_COMPARE& GetAlgParamPixelCompare() const { return m_AlgParamPixelCompare; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamIPC(const TALG_PARAM_IPC_PRODUCT &value) { m_AlgParamIPC = value; }
	TALG_PARAM_IPC_PRODUCT&     GetAlgParamIPC() { return m_AlgParamIPC; }
	const TALG_PARAM_IPC_PRODUCT&  GetAlgParamIPC() const { return m_AlgParamIPC; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamResinHight(const TALG_PARAM_RESIN_HEIGHT &value) { m_AlgParamResinHight = value; }
	TALG_PARAM_RESIN_HEIGHT&     GetAlgParamResinHeight() { return m_AlgParamResinHight; }
	const TALG_PARAM_RESIN_HEIGHT&  GetAlgParamResinHeight() const { return m_AlgParamResinHight; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamWireWidth(const TALG_PARAM_WIRE_WIDTH &value) { m_AlgParamWireWidth = value; }
	TALG_PARAM_WIRE_WIDTH&     GetAlgParamWireWidth() { return m_AlgParamWireWidth; }
	const TALG_PARAM_WIRE_WIDTH&  GetAlgParamWireWidth() const { return m_AlgParamWireWidth; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamAiModel(const TALG_PARAM_AI_MODEL &value) { m_AlgParamAiModel = value; }
	TALG_PARAM_AI_MODEL&       GetAlgParamAiModel() { return m_AlgParamAiModel; }
	const TALG_PARAM_AI_MODEL& GetAlgParamAiModel() const { return m_AlgParamAiModel; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamSolderWetting(const TALG_PARAM_SOLDER_WETTING &value) { m_AlgParamSolderWetting = value; }
	TALG_PARAM_SOLDER_WETTING&       GetAlgParamSolderWetting() { return m_AlgParamSolderWetting; }
	const TALG_PARAM_SOLDER_WETTING& GetAlgParamSolderWetting() const { return m_AlgParamSolderWetting; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamMeasureBlackGlue(const TALG_PARAM_MEASURE_BLACK_GLUE &value) { m_AlgParamMeasureBlackGlue = value; }
	TALG_PARAM_MEASURE_BLACK_GLUE&       GetAlgParamMeasureBlackGlue() { return m_AlgParamMeasureBlackGlue; }
	const TALG_PARAM_MEASURE_BLACK_GLUE& GetAlgParamMeasureBlackGlue() const { return m_AlgParamMeasureBlackGlue; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamMeasureFluxArea(const TALG_PARAM_MEASURE_FLUX_AREA &value) { m_AlgParamMeasureFluxArea = value; }
	TALG_PARAM_MEASURE_FLUX_AREA&       GetAlgParamMeasureFluxArea() { return m_AlgParamMeasureFluxArea; }
	const TALG_PARAM_MEASURE_FLUX_AREA& GetAlgParamMeasureFluxArea() const { return m_AlgParamMeasureFluxArea; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamMeasureCpuPin(const TALG_PARAM_MEASURE_CPU_PIN &value) { m_AlgParamMeasureCpuPin = value; }
	TALG_PARAM_MEASURE_CPU_PIN&       GetAlgParamMeasureCpuPin() { return m_AlgParamMeasureCpuPin; }
	const TALG_PARAM_MEASURE_CPU_PIN& GetAlgParamMeasureCpuPin() const { return m_AlgParamMeasureCpuPin; }
	//---------------------------------------------------------------------------------//
	void                       SetAlgParamMeasureSIP(const TALG_PARAM_MEASURE_SIP &value) { m_AlgParamMeasureSIP = value; }
	TALG_PARAM_MEASURE_SIP&  GetAlgParamMeasureSIP() { return m_AlgParamMeasureSIP; }
	const TALG_PARAM_MEASURE_SIP& GetAlgParamMeasureSIP() const { return m_AlgParamMeasureSIP; }
	//---------------------------------------------------------------------------------//	
	void                       SetAlgParamMeasureConnector(const TALG_PARAM_MEASURE_CONNECTOR&value) { m_AlgParamMeasureConnector = value; }
	TALG_PARAM_MEASURE_CONNECTOR&  GetAlgParamMeasureConnector() { return m_AlgParamMeasureConnector; }
	const TALG_PARAM_MEASURE_CONNECTOR& GetAlgParamMeasureConnector() const { return m_AlgParamMeasureConnector; }
	//---------------------------------------------------------------------------------//	
	bool                       CopyAlgParam_BinyParam(const CAlgParam &AlgParam);
	bool                       CopyAlgParam_GenParam(const CAlgParam &AlgParam, bool bKeepSpec);	

	bool                       CopyAlgParamResultValue(CAlgParam &AlgParam);//複製演算法檢測值
	bool                       CopyAlgParamIsolatedValue(CAlgParam &AlgParam);//複製演算法獨立值
	bool                       InitAlgInspection(bool bModelInit);//初始化演算法檢測
	bool                       ExecAlgInspection(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList, bool bTestWnd);//執行演算法檢測
	//---------------------------------------------------------------------------------//
	double                     CalcRotateAngle(const std::vector<TPOINT4D> &PosList, double AngleRange);//計算旋轉角度
	//---------------------------------------------------------------------------------//	
	bool                       UpdateAlgFrameUniqueID(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList);
	//---------------------------------------------------------------------------------//	
	bool                       ExecAlgInspection_AiModel_Public(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);	
	//---------------------------------------------------------------------------------//	
	//---------------------------------------------------------------------------------//	
	bool                       CheckAlgSaveDefectImage() const;//確認演算法儲存瑕疵圖檔
	bool                       SaveAlgDefectImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR GrayPtr, IMAGE_PTR BinPtr, const RECT &WndRect, bool bFixImg=false);//儲存演算法瑕疵圖片
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_ALGPARAM_H__23046515_E7B6_4B93_9DEE_E439E9840A50__INCLUDED_)
