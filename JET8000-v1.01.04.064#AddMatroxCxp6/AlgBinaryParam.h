// AlgBinaryParam.h: interface for the CAlgBinaryParam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALGBINARYPARAM_H__DA0CC040_B6A0_49DF_83F4_CA2E0BD0538E__INCLUDED_)
#define AFX_ALGBINARYPARAM_H__DA0CC040_B6A0_49DF_83F4_CA2E0BD0538E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "ColorGroup.h"
//-------------------------------------------------------------------------------------//
enum BIN_PARAM_BELONG_TO//參數屬於哪個變數內
{
	BIN_PARAM_BELONG_TO_NONE          = 0,
	BIN_PARAM_BELONG_TO_ALG_IMAGE     = 1,//屬於演算法影像參數
	BIN_PARAM_BELONG_TO_MASK_IMAGE    = 2,//屬於遮罩影像參數
	BIN_PARAM_BELONG_TO_PATTERN_IMAGE = 3,//屬於樣板影像參數
	BIN_PARAM_BELONG_TO_ROI_IMAGE     = 4,//屬於子框影像參數
	BIN_PARAM_BELONG_TO_RETURN
};
//-------------------------------------------------------------------------------------//
#define SYNTHESIS_WEIGHTING_MAX          1000
#define SYNTHESIS_WEIGHTING_MIN         -1000
#define FIXED_THRESHOLD_MAX_2D            255
#define FIXED_THRESHOLD_MIN_2D              0
#define DYNAMIC_THRESHOLD_MAX           100.0
#define DYNAMIC_THRESHOLD_MIN             0.0
#define RELATIVE_BIAS_MAX_2D              128
#define RELATIVE_BIAS_MIN_2D             -128
#define RELATIVE_THRESHOLD_MAX_2D         255
#define RELATIVE_THRESHOLD_MIN_2D           0	
#define FIXED_THRESHOLD_MAX_3D          40000//40 mm
#define FIXED_THRESHOLD_MIN_3D          -1000
#define RELATIVE_BIAS_MAX_3D             1000
#define RELATIVE_BIAS_MIN_3D            -1000
#define RELATIVE_THRESHOLD_MAX_3D       40000
#define RELATIVE_THRESHOLD_MIN_3D           0	
#define ADAPTIVE_THRESHOLD_GAP_MAX_2D      127
#define ADAPTIVE_THRESHOLD_GAP_MIN_2D     -127	
#define ADAPTIVE_THRESHOLD_GAP_MAX_3D    20000
#define ADAPTIVE_THRESHOLD_GAP_MIN_3D   -20000	
#define ADAPTIVE_THRESHOLD_CALC_SIZE_MAX   255
#define ADAPTIVE_THRESHOLD_CALC_SIZE_MIN     4
//-------------------------------------------------------------------------------------//
typedef struct tagBINARY_FILTER
{
	NOISE_FILTER_MODE          FilterMode;
	int                        FilterParam1;
	int                        FilterParam2;
	tagBINARY_FILTER()
	{
		FilterMode   = NOISE_FILTER_DISABLE;
		FilterParam1 = 3;
		FilterParam2 = 3;
	}
} TBINARY_FILTER, *PBINARY_FILTER;
//-------------------------------------------------------------------------------------//
class CAlgBinaryParam  
{
private:
	//---------------------------------------------------------------------------------//
	unsigned int               m_FrameIndex;//畫面編號
	unsigned int               m_FrameUniqueID;////畫面唯一碼
	bool                       m_FrameSpaceEnabled;//3D使用

	unsigned int               m_BelongIndex;//樣板編號
	BIN_PARAM_BELONG_TO        m_BelongToWho;//參數屬於誰

	MASK_FUNC_MODE             m_MaskFuncMode;//遮罩功能模式

	IMAGE_SRC_MODE             m_ImageSourceMode;
	BINARY_MODE                m_BinaryMode;	
	bool                       m_BinaryInvert;//二值化反向

	bool                       m_GrayInvert;//灰階負片
	double                     m_GrayGainValue;//灰階增益數值
	bool                       m_GrayGainEnabled;//灰階增益啟用	

	TBINARY_FILTER             m_GrayFilter1;//第1組灰階濾波
	TBINARY_FILTER             m_GrayFilter2;//第2組灰階濾波

	TBINARY_FILTER             m_BinaryFilter1;//第1組雜訊濾波
	TBINARY_FILTER             m_BinaryFilter2;//第2組雜訊濾波

	EDGE_ENHANCE_MODE          m_EdgeEnhanceMode;//邊緣強化模式
	TBINARY_FILTER             m_EdgeEnhanceFilter1;//第1組邊緣強化濾波
	TBINARY_FILTER             m_EdgeEnhanceFilter2;//第2組邊緣強化濾波

	//相關參數
	int                        m_SynthesisWR;//影像合成權重-紅色
	int                        m_SynthesisWG;//影像合成權重-綠色
	int                        m_SynthesisWB;//影像合成權重-藍色

	//二值化
	int                        m_BinaryColorGroupLinkIndex;//彩色過濾的連動編號
	CColorGroup                m_BinaryColorGroup;//彩色過濾的顏色群組

	int                        m_FixedThresholdHigh;//固定閥值-上
	int                        m_FixedThresholdLow;//固定閥值-下

	double                     m_RatioThresholdTarget;//比例閥值-目標
	double                     m_RatioThresholdRatioHigh;//比例閥值-比例-上
	double                     m_RatioThresholdRatioLow;//比例閥值-比例-下	

	double                     m_DynamicThresholdRatio;//動態比例
	int                        m_DynamicThresholdValue;//動態閥值	

	double                     m_RelativeAveThresholdValue;//相對平均閥值
	int                        m_RelativeAveThresholdBias;//相對平均閥值-補償
	int                        m_RelativeAveThresholdAbove;//相對平均閥值-上
	int                        m_RelativeAveThresholdBelow;//相對平均閥值-下	

	int                        m_AdaptiveThresholdGap;//適應性閥值
	int                        m_AdaptiveThresholdCalcSize;//適應性計算範圍
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitBinaryParam();
	void                       InitialBinaryParam();
	void                       CloneBinaryParam(const CAlgBinaryParam &Binary);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CAlgBinaryParam();
	CAlgBinaryParam(const CAlgBinaryParam &Binary);
	virtual ~CAlgBinaryParam();
	CAlgBinaryParam&           operator=(const CAlgBinaryParam &Binary);
	//---------------------------------------------------------------------------------//
	bool                       WriteAlgBinaryParamFile(CAOIFileIO &FileIO);//儲存2值化參數
	bool                       ReadAlgBinaryParamFile(CAOIFileIO &FileIO);//載入2值化參數
	//---------------------------------------------------------------------------------//
	bool                       WriteAlgBinaryFilterFile(CAOIFileIO &FileIO, TBINARY_FILTER *BinFilterPtr);//儲存2值化過濾器參數
	bool                       ReadAlgBinaryFilterFile(CAOIFileIO &FileIO, TBINARY_FILTER *BinFilterPtr);//載入2值化過濾器參數
	//---------------------------------------------------------------------------------//		
	//畫面編號
	void                       SetBinaryFrameIndex(unsigned int val);
	unsigned int               GetBinaryFrameIndex() const { return m_FrameIndex; }
	//---------------------------------------------------------------------------------//	
	//畫面唯一碼
	void                       SetBinaryFrameUniqueID(unsigned int val);
	unsigned int               GetBinaryFrameUniqueID() const { return m_FrameUniqueID; }	
	//---------------------------------------------------------------------------------//	
	//3D使用
	void                       SetBinaryFrameSpaceEnabled(bool val) { m_FrameSpaceEnabled = val; }
	bool                       GetBinaryFrameSpaceEnabled() const { return m_FrameSpaceEnabled; }	
	//---------------------------------------------------------------------------------//		
	//樣板編號
	void                       SetBinaryBelongIndex(unsigned int val) { m_BelongIndex = val; }
	unsigned int               GetBinaryBelongIndex() const { return m_BelongIndex; }		
	//---------------------------------------------------------------------------------//	
	//參數屬於誰
	void                       SetBinaryBelongToWho(BIN_PARAM_BELONG_TO val) { m_BelongToWho = val; }
	BIN_PARAM_BELONG_TO        GetBinaryBelongToWho() const { return m_BelongToWho; }		        
	//---------------------------------------------------------------------------------//	
	//遮罩功能模式
	void                       SetMaskFuncMode(MASK_FUNC_MODE val) { m_MaskFuncMode = val; }
	MASK_FUNC_MODE             GetMaskFuncMode() const { return m_MaskFuncMode; }
	//---------------------------------------------------------------------------------//
	//影像來源
	void                       SetBinaryImageSourceMode(IMAGE_SRC_MODE val) { m_ImageSourceMode = val; }
	IMAGE_SRC_MODE             GetBinaryImageSourceMode() const { return m_ImageSourceMode; }		        
	//---------------------------------------------------------------------------------//	
	//2值化模式
	void                       SetBinaryMode(BINARY_MODE val) { m_BinaryMode = val; }
	BINARY_MODE                GetBinaryMode() const { return m_BinaryMode; }
	//---------------------------------------------------------------------------------//	
	//二值化反向
	void                       SetBinaryInvert(bool val) { m_BinaryInvert = val; }
	bool                       GetBinaryInvert() const { return m_BinaryInvert; }
	//---------------------------------------------------------------------------------//
	//灰階負片
	void                       SetGrayInvert(bool val) { m_GrayInvert = val; }
	bool                       GetGrayInvert() const { return m_GrayInvert; }
	//---------------------------------------------------------------------------------//	
	//影像增益數值
	void                       SetGrayGainValue(double val) { m_GrayGainValue = val; }
	double                     GetGrayGainValue() const { return m_GrayGainValue; }
	//---------------------------------------------------------------------------------//
	//影像增益啟用	
	void                       SetGrayGainEnabled(bool val) { m_GrayGainEnabled = val; }
	bool                       GetGrayGainEnabled() const { return m_GrayGainEnabled; }
	//---------------------------------------------------------------------------------//
	bool                       CheckGrayGainEnabed() const;//確認是否使用影像增益
	//---------------------------------------------------------------------------------//
	void                       CopyGrayFilterParam(const CAlgBinaryParam &Param);//複製灰階過濾參數
	bool                       CheckUseGrayFilterParam(const TBINARY_FILTER &Filter) const;//確認是否使用灰階過濾參數
	//---------------------------------------------------------------------------------//
	//第1組灰階濾波
	void                       SetGrayNoiseFilter1(TBINARY_FILTER val) { m_GrayFilter1 = val; }
	const TBINARY_FILTER&      GetGrayNoiseFilter1() const { return m_GrayFilter1; }
	//---------------------------------------------------------------------------------//		
	//第2組灰階濾波
	void                       SetGrayNoiseFilter2(TBINARY_FILTER val) { m_GrayFilter2 = val; }
	const TBINARY_FILTER&      GetGrayNoiseFilter2() const { return m_GrayFilter2; }
	//---------------------------------------------------------------------------------//	
	void                       CopyBinaryFilterParam(const CAlgBinaryParam &Param);//複製2值化過濾參數
	bool                       CheckUseBinaryFilterParam(const TBINARY_FILTER &Filter) const;//確認是否使用2值化過濾參數
	//---------------------------------------------------------------------------------//	
	//第1組雜訊濾波
	void                       SetBinaryNoiseFilter1(TBINARY_FILTER val) { m_BinaryFilter1 = val; }
	const TBINARY_FILTER&      GetBinaryNoiseFilter1() const { return m_BinaryFilter1; }
	//---------------------------------------------------------------------------------//	
	//第2組雜訊濾波
	void                       SetBinaryNoiseFilter2(TBINARY_FILTER val) { m_BinaryFilter2 = val; }
	const TBINARY_FILTER&      GetBinaryNoiseFilter2() const { return m_BinaryFilter2; }
	//---------------------------------------------------------------------------------//	
	void                       CopyEdgeEnhanceParam(const CAlgBinaryParam &Param);
	bool                       CheckUseEdgeEnhanceParam(const TBINARY_FILTER &Filter) const;//確認是否使用邊緣強化參數
	//---------------------------------------------------------------------------------//
	void                       SetEdgeEnhanceMode(EDGE_ENHANCE_MODE val) { m_EdgeEnhanceMode = val; }
	EDGE_ENHANCE_MODE          GetEdgeEnhanceMode() const { return m_EdgeEnhanceMode; }
	//---------------------------------------------------------------------------------//	
	//第1組邊緣強化濾波
	void                       SetEdgeEnhanceFilter1(TBINARY_FILTER val) { m_EdgeEnhanceFilter1 = val; }
	const TBINARY_FILTER&      GetEdgeEnhanceFilter1() const { return m_EdgeEnhanceFilter1; }
	//---------------------------------------------------------------------------------//	
	//第2組邊緣強化濾波
	void                       SetEdgeEnhanceFilter2(TBINARY_FILTER val) { m_EdgeEnhanceFilter2 = val; }
	const TBINARY_FILTER&      GetEdgeEnhanceFilter2() const { return m_EdgeEnhanceFilter2; }
	//---------------------------------------------------------------------------------//
	//影像合成權重
	void                       SetBinarySynthesisWR(int val) { m_SynthesisWR = val; }
	int                        GetBinarySynthesisWR() const { return m_SynthesisWR; }
	
	void                       SetBinarySynthesisWG(int val) { m_SynthesisWG = val; }
	int                        GetBinarySynthesisWG() const { return m_SynthesisWG; }

	void                       SetBinarySynthesisWB(int val) { m_SynthesisWB = val; }
	int                        GetBinarySynthesisWB() const { return m_SynthesisWB; }
	//---------------------------------------------------------------------------------//		
	//彩色過濾的顏色群組	
	void                       SetBinaryColorGroupLinkIndex(int val) { m_BinaryColorGroupLinkIndex = val; }
	int                        GetBinaryColorGroupLinkIndex() const { return m_BinaryColorGroupLinkIndex; }
	
	void                       SetBinaryColorGroup(const CColorGroup &val);
	CColorGroup&               GetBinaryColorGroup() { return m_BinaryColorGroup; }	
	CColorGroup*               GetBinaryColorGroupPtr() { return &m_BinaryColorGroup; }
	const CColorGroup&         GetBinaryColorGroup() const { return m_BinaryColorGroup; }
	//---------------------------------------------------------------------------------//	
	size_t                     GetBinaryColorCount() const;
	void                       AddBinaryColor(const CColorRGBV &rgbv);
	bool                       SetBinaryColor(size_t idx, const CColorRGBV &rgbv);//設定抽色群組的顏色
	CColorRGBV*                GetBinaryColorPtr(size_t index, bool Check);
	const CColorRGBV*          GetBinaryColorPtr(size_t index, bool Check) const;
	CColorRGBV*                GetBinaryColorActivePtr();//取得主要操作顏色	
	void                       SetBinaryColorActivePtr(CColorRGBV* Ptr);//設定主要操作顏色	
	size_t                     GetBinaryColorActiveIndex() const;//取得主要操作顏色	
	void                       SetBinaryColorActiveIndex(size_t index);//設定主要操作顏色		
	void                       ClearBinaryColorList();//清除抽色群組的顏色列表
	void                       ResetBinaryColorList();//復歸抽色群組的顏色列表
	void                       UpdateBinaryColorUsed();//更新抽色群組的顏色列表是否使用
	void                       UpdateBinaryColorShowColor();//更新抽色群組的顏色列表是否使用
	bool                       MergeBinaryColor(bool IncClr=true, bool ExcClr=true);//合併抽色群組內的色
	//---------------------------------------------------------------------------------//	
	//固定閥值
	void                       SetFixedThresholdHigh(int val) { m_FixedThresholdHigh = val; }
	int                        GetFixedThresholdHigh() const { return m_FixedThresholdHigh; }

	void                       SetFixedThresholdLow(int val) { m_FixedThresholdLow = val; }
	int                        GetFixedThresholdLow() const { return m_FixedThresholdLow; }
	//---------------------------------------------------------------------------------//		
	//比例閥值
	void                       SetRatioThresholdTarget(double val) { m_RatioThresholdTarget = val; }
	double                     GetRatioThresholdTarget() const { return m_RatioThresholdTarget; }

	void                       SetRatioThresholdRatioHigh(double val) { m_RatioThresholdRatioHigh = val; }
	double                     GetRatioThresholdRatioHigh() const { return m_RatioThresholdRatioHigh; }

	void                       SetRatioThresholdRatioLow(double val) { m_RatioThresholdRatioLow = val; }
	double                     GetRatioThresholdRatioLow() const { return m_RatioThresholdRatioLow; }
	//---------------------------------------------------------------------------------//
	//動態比例
	void                       SetDynamicThresholdRatio(double val) { m_DynamicThresholdRatio = val; }
	double                     GetDynamicThresholdRatio() const { return m_DynamicThresholdRatio; }
	//---------------------------------------------------------------------------------//	
	//動態閥值	
	void                       SetDynamicThresholdValue(int val) { m_DynamicThresholdValue = val; }
	int                        GetDynamicThresholdValue() const { return m_DynamicThresholdValue; }
	//---------------------------------------------------------------------------------//
	//相對平均閥值	
	void                       SetRelativeAveThresholdValue(double val) { m_RelativeAveThresholdValue = val; }
	double                     GetRelativeAveThresholdValue() const { return m_RelativeAveThresholdValue; }

	void                       SetRelativeAveThresholdBias(int val) { m_RelativeAveThresholdBias = val; }
	int                        GetRelativeAveThresholdBias() const { return m_RelativeAveThresholdBias; }

	void                       SetRelativeAveThresholdAbove(int val) { m_RelativeAveThresholdAbove = val; }
	int                        GetRelativeAveThresholdAbove() const { return m_RelativeAveThresholdAbove; }
	
	void                       SetRelativeAveThresholdBelow(int val) { m_RelativeAveThresholdBelow = val; }
	int                        GetRelativeAveThresholdBelow() const { return m_RelativeAveThresholdBelow; }
	//---------------------------------------------------------------------------------//	
	//適應性閥值
	void                       SetAdaptiveThresholdGap(int val) { m_AdaptiveThresholdGap = val; }
	int                        GetAdaptiveThresholdGap() const { return m_AdaptiveThresholdGap; }

	void                       SetAdaptiveThresholdCalcSize(int val) { m_AdaptiveThresholdCalcSize = val; }
	int                        GetAdaptiveThresholdCalcSize() const { return m_AdaptiveThresholdCalcSize; }
	//---------------------------------------------------------------------------------//	
	void                       MirrorBinaryParamXAxis();
	void                       MirrorBinaryParamYAxis();
	void                       RotateBinaryParam(int AngleLabel);	
	//---------------------------------------------------------------------------------//
	bool                       UpdateBinaryFrameUniqueID(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList);
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_ALGBINARYPARAM_H__DA0CC040_B6A0_49DF_83F4_CA2E0BD0538E__INCLUDED_)
