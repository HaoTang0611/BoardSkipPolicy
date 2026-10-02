#ifndef _LIGHT3D_DEF_H_
#define _LIGHT3D_DEF_H_
//-------------------------------------------------------------------------------------//
//#include "JetAOI3DDefine.h"
//-------------------------------------------------------------------------------------//
#define LIGHT_3D_TI_DLP         1

#define LIGHT_3D_MODE           LIGHT_3D_TI_DLP
//-------------------------------------------------------------------------------------//
#if LIGHT_3D_MODE == LIGHT_3D_TI_DLP	
	//#define LIGHT_3D_TI_DLP_4500_USE
	#define LIGHT_3D_TI_DLP_4710_USE
	//#define LIGHT_3D_TI_DLP_IMP_USE//有臭蟲
#endif//LIGHT_3D_MODE

#ifdef LIGHT_3D_TI_DLP_IMP_USE		
	#define LIGHT_3D_TI_DLP_USE_IMP_4500
	#define LIGHT_3D_TI_DLP_USE_IMP_4710
#endif//LIGHT_3D_TI_DLP_IMP_USE
//-------------------------------------------------------------------------------------//
//用來定義PMP(Phase Measuring Profilometry)的參數
//-------------------------------------------------------------------------------------//
//FLT:://取小數點7位數
//DBL:://取小數點15位數
#if PHASE_TYPE == DATA_SHORT
	#define IMG_SHIFT_PHASE_BIT       8//影像轉相位的位元差
	#define PHASE_MIN			 -32768//15bit	
	#define PHASE_MAX			  32767//15bit		
	#define PHASE_PERIOD          32767//PHASE_MAX(32767)
	#define PHASE_PERIOD_HALF     PHASE_PERIOD/2//PHASE_PERIOD/2	
	#define PHASE_RAD2SHORT	       5215.030020293451//((double)PHASE_MAX/(double)(PERIOD_RAD))
	#define PHASE_RAD2SHORT_FLT    5215.030020f//((double)PHASE_MAX/(double)(PERIOD_RAD))

	#define PHASE_DEG2SHORT		     91.019444444444//((double)PHASE_MAX/360.0)
	#define PHASE_DEG2SHORT_FLT	     91.019444f//((double)PHASE_MAX/360.0)

	#define PHASE_SHORT2RAD		      0.000191753450337//(PERIOD_RAD/(double)PHASE_MAX)
	#define PHASE_SHORT2DEG		      0.010986663411359//360.0/(double)PHASE_MAX
	//#define PHASE_SHORT2ONE		      0.000030518509476//1.0/(double)PHASE_MAX

#elif PHASE_TYPE == DATA_USORT
	#define IMG_SHIFT_PHASE_BIT       8//影像轉相位的位元差
	#define PHASE_MIN			      0//16bit-65535
	#define PHASE_MAX			  65535//16bit-65535	
	#define PHASE_PERIOD          65535//PHASE_MAX+1
	#define PHASE_PERIOD_HALF     PHASE_PERIOD/2	
	#define PHASE_RAD2SHORT		  10430.378350470452724//10430.219195527360820//((double)PHASE_MAX/(double)(PERIOD_RAD))
	#define PHASE_RAD2SHORT_FLT   10430.3783504f//10430.2191955f//((double)PHASE_MAX/(double)(PERIOD_RAD))

	#define PHASE_DEG2SHORT		    182.044444444444444//182.041666666666667//((double)PHASE_MAX/360.0)
	#define PHASE_DEG2SHORT_FLT	    182.0444444f//182.0416667f//((double)PHASE_MAX/360.0)

	#define PHASE_SHORT2RAD		      0.000095873799243//0.000095875262183//(PERIOD_RAD/(double)PHASE_MAX)
	#define PHASE_SHORT2DEG		      0.0054931640625//0.005493247882811//360.0/(double)PHASE_MAX
	//#define PHASE_SHORT2ONE		      0.0000152587890625//0.000015259021897//1.0/(double)PHASE_MAX
#else
	#error Not Define Phase Type
#endif//PHASE_TYPE
//-------------------------------------------------------------------------------------//
#if SPACE_TYPE == DATA_FLOAT
	#define UNWRAP_PERIOD_HALF          0.5f
	#define UNWRAP_PERIOD_FULL          1.0f	

	#define SPACE_MAX           						FLT_MAX
	#define SPACE_MIN           					   -FLT_MAX

	#define UNWRAP_DATA_AVOID_PHASE						FLT_MAX
	#define UNWRAP_DATA_AVOID_PHASE_OVER_SATURATION		FLT_MAX-1.0f
	#define UNWRAP_DATA_AVOID_PHASE_LOW_CONTRAST		FLT_MAX-2.0f
	#define UNWRAP_DATA_AVOID_PHASE_BASEPHASE_VOID		FLT_MAX-3.0f
	#define UNWRAP_DATA_AVOID_PHASE_EXTEND_VOID 		FLT_MAX-4.0f
	#define UNWRAP_DATA_AVOID_PHASE_OTHER_VOID			FLT_MAX-5.0f
	#define UNWRAP_DATA_AVOID_BRANCH_CUT				FLT_MAX-6.0f
	#define UNWRAP_DATA_AVOID_OVER_HEIGHT				FLT_MAX-7.0f
	#define UNWRAP_DATA_AVOID_OVER_LOW					FLT_MAX-8.0f
	#define UNWRAP_DATA_AVOID_OTHER						FLT_MAX-9.0f
	#define UNWRAP_DATA_AVOID							FLT_MAX-10.0f

	#define PHASE_SHORT2ONE		                        UNWRAP_PERIOD_FULL/(double)(PHASE_MAX)//1.0/(double)PHASE_MAX
	#define PHASE_HEIGHT_FACTOR_SCALE                   10000.0f//相位高度縮放值
#elif   SPACE_TYPE == DATA_INT
	#define UNWRAP_PERIOD_HALF          5000
	#define UNWRAP_PERIOD_FULL          10000	
	
	#define SPACE_MAX           						INT_MAX
	#define SPACE_MIN           					   -INT_MIN

	#define UNWRAP_DATA_AVOID_PHASE						INT_MAX
	#define UNWRAP_DATA_AVOID_PHASE_OVER_SATURATION		INT_MAX-1
	#define UNWRAP_DATA_AVOID_PHASE_LOW_CONTRAST		INT_MAX-2
	#define UNWRAP_DATA_AVOID_PHASE_BASEPHASE_VOID		INT_MAX-3
	#define UNWRAP_DATA_AVOID_PHASE_EXTEND_VOID 		INT_MAX-4
	#define UNWRAP_DATA_AVOID_PHASE_OTHER_VOID			INT_MAX-5
	#define UNWRAP_DATA_AVOID_BRANCH_CUT				INT_MAX-6
	#define UNWRAP_DATA_AVOID_OVER_HEIGHT				INT_MAX-7
	#define UNWRAP_DATA_AVOID_OVER_LOW					INT_MAX-8
	#define UNWRAP_DATA_AVOID_OTHER						INT_MAX-9
	#define UNWRAP_DATA_AVOID							INT_MAX-10

	#define PHASE_SHORT2ONE		                        (double)UNWRAP_PERIOD_FULL/(double)(PHASE_MAX)//1.0/(double)PHASE_MAX
	#define PHASE_HEIGHT_FACTOR_SCALE                   10000//相位高度縮放值
#else
	#error Not Define Phase Type
#endif
//-------------------------------------------------------------------------------------//

//Frequence Mode----------------------------------------------
#define	FREQUENCE_MODE_P1		0	//第一週期
#define	FREQUENCE_MODE_P2		1	//第二週期
#define	FREQUENCE_MODE_MF		2	//合併週期
//-------------------------------------------------------------------------------------//
#define USE_GAMMA_CALIBRATION             false
//-------------------------------------------------------------------------------------//
#define COMBINE_PHASE_SIBLING_PERIOD         1//鄰近週期
#define COMBINE_PHASE_MAX_MIN_PERIOD         2//大小週期
#define COMBINE_PHASE_PERIOD_MODE            COMBINE_PHASE_SIBLING_PERIOD;//COMBINE_PHASE_SIBLING_PERIOD, COMBINE_PHASE_MAX_MIN_PERIOD
//-------------------------------------------------------------------------------------//
#define PHASE_CONVERT_HEIGHT_SCALE           1//相位轉換高度-比例
#define PHASE_CONVERT_HEIGHT_MAPPING_FUNC_3  3//相位轉換高度-映射函式
#define PHASE_CONVERT_HEIGHT_MAPPING_FUNC_4  4//相位轉換高度-映射函式
#define PHASE_CONVERT_HEIGHT_MAPPING_FUNC_5  5//相位轉換高度-映射函式
#define PHASE_CONVERT_HEIGHT_MAPPING_FUNC_6  6//相位轉換高度-映射函式
#define PHASE_CONVERT_HEIGHT_MAPPING_FUNC_7  7//相位轉換高度-映射函式
//-------------------------------------------------------------------------------------//
//多投光合併最可靠模式
#define MULTI_CAST_MERGE_BEST_MODE_01     1//舊的模式
#define MULTI_CAST_MERGE_BEST_MODE_02     2//阿成的模式
#define MULTI_CAST_MERGE_BEST_MODE_03     3//阿凱的模式-與阿成模式相同, 但有雜訊就不算最可靠高度
//-------------------------------------------------------------------------------------//
//多投光合併模式
#define MULTI_CAST_MERGE_MODE_MASS       1//眾數合併
#define MULTI_CAST_MERGE_MODE_LOWEST     2//最低合併
#define MULTI_CAST_MERGE_MODE_MASS_2     3//眾數合併2
#define MULTI_CAST_MERGE_MODE_MASS_3     4//眾數合併3
#define MULTI_CAST_MERGE_MODE_MASS_4     5//眾數合併4
#define MULTI_CAST_MERGE_MODE_MEDIAN     6//空間中位數合併
#define MULTI_CAST_MERGE_MODE_JOE_1      7//義仁與軍達方法1
//-------------------------------------------------------------------------------------//
#define MULTI_INTENSITY_MERGE_MODE_MEAN  1//以平均灰階為主
#define MULTI_INTENSITY_MERGE_MODE_MAXB  2//以MaxB為主
#define MULTI_INTENSITY_MERGE_MODE_MAXCV 3//以MaxCV為主
//-------------------------------------------------------------------------------------//
//投光後濾波模式
#define CAST_SPACE_FILTER_MODE_DEFAULT		1//default
#define CAST_SPACE_FILTER_MODE_SPOT			2//最小雜訊模式
#define CAST_SPACE_FILTER_MODE_SMALL		3//小雜訊模式
#define CAST_SPACE_FILTER_MODE_MEDIUM		4//中雜訊模式
#define CAST_SPACE_FILTER_MODE_LARGE		5//大雜訊模式
//-------------------------------------------------------------------------------------//
//Phase Mask---------------------------------------------------
//0x00 為空值
// 1~ 9為相位異常(1:LowContras, 2:LowPotential, 3:OverSaturated, 4:ExtendVoid)
//10~15為高度異常(11:OverLow, 12:HeightUnexpected)
//0x10 (16), 大於(16)為有效值, 16~31是有效的範圍
//0x20, 0x40, 0x80是特殊的遮罩使用
//-------------------------------------------------------------------------------------//
#define PHASE_NOISE_DEF_MODE_1          1//預設方式
#define PHASE_NOISE_DEF_MODE_2          2//Joe提供
//-------------------------------------------------------------------------------------//
#define PHASE_MASK_VALID				0x00		//有效點
#define PHASE_MASK_LOW_CONTRAST			0x01		//低對比
#define PHASE_MASK_LOW_POTENTIAL		0x02		//低能量
#define PHASE_MASK_OVER_SATURATED		0x04		//過飽合
#define PHASE_MASK_EXTEND_VOID			0x08		//外擴無效點
#define PHASE_MASK_HEIGHT_UNEXPECTED	0x10		//高度異常
#define PHASE_MASK_OVER_LOW				0x20		//PHASE_MASK_DARK暗點-JET8000無使用
#define PHASE_MASK_VALID_BEST			0x80		//有效點-相當好
#define PHASE_MASK_NOISE_ONLY           PHASE_MASK_LOW_CONTRAST+PHASE_MASK_LOW_POTENTIAL+PHASE_MASK_OVER_SATURATED
#define PHASE_MASK_NOISE                PHASE_MASK_LOW_CONTRAST+PHASE_MASK_LOW_POTENTIAL+PHASE_MASK_OVER_SATURATED+PHASE_MASK_EXTEND_VOID+PHASE_MASK_HEIGHT_UNEXPECTED+PHASE_MASK_OVER_LOW
//-------------------------------------------------------------------------------------//
//相位周期樣板
#define PHASE_PATTERN_A          1//相位周期樣板-第1周期
#define PHASE_PATTERN_B          2//相位周期樣板-第2周期
#define PHASE_PATTERN_C          3//相位周期樣板-第3周期
//-------------------------------------------------------------------------------------//
const int PHASE_NOSIE_LOW_CONTRAST = 0x00000001;//對比不足
const int PHASE_NOSIE_LOW_POTENTIAL = 0x00000002;//能量不足
const int PHASE_NOSIE_OVER_SATURATED = 0x00000004;//過飽和
const int PHASE_NOSIE_VOID_EXPAND = 0x00000008;//雜訊外擴
const int PHASE_NOSIE_SMOOTH_FILTER = 0x00000010;//平滑處理
//-------------------------------------------------------------------------------------//
enum LIGHT_3D_DEVICE_TYPE//3D燈源裝置型號
{
	LIGHT_3D_DEVICE_NULL       = 0,
	LIGHT_3D_DEVICE_DLP4500    = 1,
	LIGHT_3D_DEVICE_DLP4710    = 2,
	LIGHT_3D_DEVICE_RETURN
};
//-------------------------------------------------------------------------------------//
enum DECODE_PHASE_MODE
{
	DECODE_PHASE_NONE          =    0,//未定義
	DECODE_PHASE_3STEP_1       =    3,//3步1相位
	DECODE_PHASE_4STEP_1       =    4,//4步1相位
	DECODE_PHASE_5STEP_1       =    5,//4步1相位
	DECODE_PHASE_3_3STEP_2     =   33,//3+3步2相位
	DECODE_PHASE_4_4STEP_2     =   44,//4+4步2相位
	DECODE_PHASE_5_5STEP_2     =   55,//5+5步2相位	
	DECODE_PHASE_2_1STEP_1     =   21,//2+1步1相位	
	DECODE_PHASE_2_2STEP_2     =   22,//2+2步2相位//Joe
	DECODE_PHASE_4_2STEP_2     =  421,//4+2+1步2相位	
	DECODE_PHASE_4STEP_5GC_2   =   45,//4步+5GC2相位
	DECODE_PHASE_4STEP_6GC_2   =   46,//4步+6GC2相位	
	DECODE_PHASE_4STEP_4GC_2   =   47,//4步+4GC2相位	

	DECODE_PHASE_3_3STEP_3     =  333,//3+3步2相位
	DECODE_PHASE_4_4STEP_3     =  444,//4+4步2相位
	DECODE_PHASE_5_5STEP_3     =  555,//5+5步2相位
	DECODE_PHASE_RETURN
};
//-------------------------------------------------------------------------------------//
typedef struct tagPhaseNoiseParam
{
	int                  PhaseConvertHeightMode;//相位轉高度模式
	int                  PhaseCombinePeriodMode;//相位合併週期方式
	int                  PhaseNoiseDef;//雜訊定義	
	int                  PhaseNoiseDefMode;//雜訊定義模式
	int                  PhaseOverSaturatedA;//過飽和
	int                  PhaseLowContrastA;//對比不足
	int                  PhaseLowPotentialA;//Potential過低

	int                  PhaseOverSaturatedB;//過飽和
	int                  PhaseLowContrastB;//對比不足
	int                  PhaseLowPotentialB;//Potential過低

	int                  PhaseOverSaturatedC;//過飽和
	int                  PhaseLowContrastC;//對比不足
	int                  PhaseLowPotentialC;//Potential過低

	int                  PhaseExtendVoid;//無效點外擴
	int                  PhaseSmoothFilter;//平滑處理	
	float                PhaseContrastRatio;//相位對比計算比例
	float                PhaseZeroPlaneOffsetPosZ;//相位基準面的偏移量
	float                SpaceSingleCastLowLimit;//高度雜訊單投光高度最低極限-um
	int                  SpaceMultiCastPatchSize;//多投光合併Patch尺寸
	int                  SpaceMultiCastMergeMode;//多投光合併模式
	int                  SpaceMultiCastMergeBestMode;//多投光合併最可靠模式
	int                  SpaceMultiIntensityMergeMode;//多亮度合併模式
	int                  SpaceMultiCastMinValidCount;//多投光最少多少有效值
	double               SpaceMultiCastMaxDifference;//多投光最大高度差距um
	double               SpaceMultiCastLimitDifference;//多投光極限高度差距um
	double               SpaceMultiCastValidBestRatio;//多投光最好高度比例%-不做濾波
	double               SpaceMultiCastValidDifference;//多投光有效高度差距um-重啟有效
	int                  SpaceMultiCastOppositeMaxGray;//多投光對邊亮度差距

	int                  SpaceCastFilterMode;//投光後濾波模式//20240904-Joe
	int                  SpaceCastMedianFilterSize;//中值濾波尺寸
	int                  SpaceCastMedianFilterUseSize;//中值濾波使用尺寸
	
	tagPhaseNoiseParam()
	{
		PhaseConvertHeightMode = PHASE_CONVERT_HEIGHT_SCALE;
		PhaseCombinePeriodMode = COMBINE_PHASE_MAX_MIN_PERIOD;
		PhaseNoiseDef = 0xFFFFFFFF;		
		PhaseNoiseDefMode=PHASE_NOISE_DEF_MODE_1;
		PhaseOverSaturatedA = 255;
		PhaseLowContrastA = 0;
		PhaseLowPotentialA = 0;

		PhaseOverSaturatedB = 255;
		PhaseLowContrastB = 0;
		PhaseLowPotentialB = 0;

		PhaseOverSaturatedC = 255;
		PhaseLowContrastC = 0;
		PhaseLowPotentialC = 0;

		PhaseExtendVoid = 0;//無效點外擴
		PhaseSmoothFilter = 5;//平滑處理
		PhaseContrastRatio = 100.0f;//相位對比計算比例
		PhaseZeroPlaneOffsetPosZ = 3000;
		SpaceSingleCastLowLimit = 1000;
		SpaceMultiCastPatchSize = 0;
		SpaceMultiCastMergeMode = MULTI_CAST_MERGE_MODE_MASS;
		SpaceMultiCastMergeBestMode=MULTI_CAST_MERGE_BEST_MODE_02;
		SpaceMultiIntensityMergeMode = MULTI_INTENSITY_MERGE_MODE_MEAN;
		SpaceMultiCastMinValidCount = 1;//多投光最少多少有效值
		SpaceMultiCastMaxDifference = 500;
		SpaceMultiCastLimitDifference = 2000;
		SpaceMultiCastValidBestRatio = 100;
		SpaceMultiCastValidDifference = 200;
		SpaceMultiCastOppositeMaxGray = 20;

		SpaceCastFilterMode = CAST_SPACE_FILTER_MODE_DEFAULT;
		SpaceCastMedianFilterSize = 0;
		SpaceCastMedianFilterUseSize = 0;//中值濾波使用尺寸
	}
} TPhaseNoiseParam, *PPhaseNoiseParam;
//-------------------------------------------------------------------------------------//
//相機圖像格式, 8Bit or 10/12Bit
#define DATA_UCHAR                 1//使用unsigned char*
#define DATA_SHORT                 2//使用short*
#define DATA_USORT                 3//使用unsigned short*
#define DATA_INT                   4//使用int*
#define DATA_UINT                  5//使用unsigned int*
#define DATA_FLOAT                 6//使用float*

#define  _uint                     unsigned int//size_t 在32與64定義不同
#define  IMAGE_SIZE                unsigned int

#define IMAGE_TYPE               DATA_UCHAR
#if   IMAGE_TYPE == DATA_UCHAR 
#define IMAGE_DATA              unsigned char //for 8Bit
#elif IMAGE_TYPE == DATA_USORT
#define IMAGE_DATA              unsigned short //for 8Bit
#else
#error IMAGE_DATA Not Defined.
#endif//IMAGE_TYPE
#define IMAGE_PTR              IMAGE_DATA*

#define PHASE_TYPE             DATA_SHORT
#if PHASE_TYPE == DATA_SHORT
#define PHASE_DATA              short //for 15Bit
#elif   PHASE_TYPE == DATA_USORT 
#define PHASE_DATA              unsigned short //for 16Bit
#elif PHASE_TYPE == DATA_UINT
#define PHASE_DATA              unsigned int //for 32Bit-尚未支援
#else
#error PHASE_DATA Not Defined.
#endif//PHASE_TYPE
#define PHASE_PTR                  PHASE_DATA*  //16Bit

#define MASK_DATA                  unsigned char
#define MASK_PTR                   MASK_DATA*

//SPACE_TYPE
#define SPACE_TYPE                 DATA_FLOAT//DATA_FLOAT, DATA_INT
#if SPACE_TYPE == DATA_FLOAT
#define SPACE_DATA                 float
#elif   SPACE_TYPE == DATA_INT
#define SPACE_DATA                 int
#else
#error SPACE_DATA Not Defined.
#endif
#define SPACE_PTR                  SPACE_DATA*
//-------------------------------------------------------------------------------------//
typedef struct tagUNI_FRAME
{
	unsigned int               FrameUniqueID;//識別碼
	IMAGE_SIZE                 ImageW;
	IMAGE_SIZE                 ImageH;
	IMAGE_SIZE                 ImageStep;
	IMAGE_SIZE                 BitCount;
	IMAGE_PTR                  ImagePtr;
	MASK_PTR                   MaskPtr;
	PHASE_PTR                  PhasePtr;
	SPACE_PTR                  SpacePtr;

	MASK_PTR                   RawMaskPtr;//原始未處理的遮罩圖
	SPACE_PTR                  RawSpacePtr;//原始未處理的高度圖
	tagUNI_FRAME()
	{
		FrameUniqueID = 0;
		ImageW = 0;
		ImageH = 0;
		ImageStep = 0;
		BitCount = 0;
		ImagePtr = NULL;
		MaskPtr = NULL;
		PhasePtr = NULL;
		SpacePtr = NULL;
		RawMaskPtr = NULL;
		RawSpacePtr = NULL;
	}
	bool CheckFrameValid() const
	{
		if ( 0 == ImageW ) { return false; }
		if ( 0 == ImageH ) { return false; }
		if ( 0 == ImageStep ) { return false; }
		if ( 0 == BitCount ) { return false; }
		if ( NULL != ImagePtr ) { return true; }//2D影像
		if ( NULL != PhasePtr ) { return true; }//相位
		if ( NULL==MaskPtr || NULL==SpacePtr ) { return false; }//3D高度
		return true;
	}
	void SetUniFrame(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE BitCnt, IMAGE_PTR Ptr)
	{	
		ImageW = W;
		ImageH = H;
		ImageStep = Step;
		BitCount = BitCnt;
		ImagePtr = Ptr;		
	}
} TUNI_FRAME, *PUNI_FRAME;
//-------------------------------------------------------------------------------------//
typedef struct tagCastParam
{
	double        PerA;
	double        PerB;
	double        Gamma;
	int           CastID;
	int           ExpCount;
	int           ExpTimeA;
	int           ExpTimeB;
	int           ExpTimeC;
	int           ExpTimeD;
	int           ImageCount;
	int           HeightBuildMode;
	int           DebugIndex;//指定輸出引數
	int           DebugExpID;//指定輸出AB/CD/Merge	
	DECODE_PHASE_MODE DecodeMode;

	IMAGE_SIZE    ImageW;
	IMAGE_SIZE    ImageH;
	IMAGE_SIZE    ImageStep;

	IMAGE_PTR     PtrA1;
	IMAGE_PTR     PtrA2;
	IMAGE_PTR     PtrA3;
	IMAGE_PTR     PtrA4;
	IMAGE_PTR     PtrA5;

	IMAGE_PTR     PtrB1;
	IMAGE_PTR     PtrB2;
	IMAGE_PTR     PtrB3;
	IMAGE_PTR     PtrB4;
	IMAGE_PTR     PtrB5;
	IMAGE_PTR     PtrB6;

	IMAGE_PTR     PtrC1;
	IMAGE_PTR     PtrC2;
	IMAGE_PTR     PtrC3;
	IMAGE_PTR     PtrC4;
	IMAGE_PTR     PtrC5;

	IMAGE_PTR     PtrD1;
	IMAGE_PTR     PtrD2;
	IMAGE_PTR     PtrD3;
	IMAGE_PTR     PtrD4;
	IMAGE_PTR     PtrD5;
	IMAGE_PTR     PtrD6;

	MASK_PTR       PtrMask;	
	SPACE_PTR      PtrSpace;

	PHASE_PTR      ZeroPhasePtr;
	SPACE_PTR      HeightFactorPtr;		
	unsigned char *GammaTablePtr;
	unsigned char *GCK1MapPtr;//GrayCode K1 Map
	unsigned char *GCK2MapPtr;//GrayCode K2 Map
	double         HeightFactor0[8];//HEIGHT_FACTOR_PARAM_COUNT
	double         HeightFactor1[8];
	double         HeightFactor2[8];
	tagCastParam()
	{
		PerA = 16;
		PerB = 128;
		Gamma = 1.0;
		CastID = 0;
		ExpCount = 1;
		ImageCount = 0;
		DebugIndex =-1;
		DebugExpID = 0;
		ExpTimeA = 5000;
		ExpTimeB = 4000;
		ExpTimeC = 5000;
		ExpTimeD = 4000;
		HeightBuildMode = 1;
		DecodeMode = DECODE_PHASE_NONE;

		ImageW = 0;
		ImageH = 0;
		ImageStep = 0;

		PtrA1 = NULL;
		PtrA2 = NULL;
		PtrA3 = NULL;
		PtrA4 = NULL;
		PtrA5 = NULL;

		PtrB1 = NULL;
		PtrB2 = NULL;
		PtrB3 = NULL;
		PtrB4 = NULL;
		PtrB5 = NULL;
		PtrB6 = NULL;

		PtrC1 = NULL;
		PtrC2 = NULL;
		PtrC3 = NULL;
		PtrC4 = NULL;
		PtrC5 = NULL;

		PtrD1 = NULL;
		PtrD2 = NULL;
		PtrD3 = NULL;
		PtrD4 = NULL;
		PtrD5 = NULL;
		PtrD6 = NULL;

		PtrMask = NULL;		
		PtrSpace = NULL;

		ZeroPhasePtr = NULL;
		HeightFactorPtr = NULL;		
		GammaTablePtr = NULL;
		GCK1MapPtr = NULL;
		GCK2MapPtr = NULL;

		::memset(HeightFactor0, 0x00, sizeof(HeightFactor0));
		::memset(HeightFactor1, 0x00, sizeof(HeightFactor1));
		::memset(HeightFactor2, 0x00, sizeof(HeightFactor2));	
	};
} TCastParam, *PCastParam;
//-------------------------------------------------------------------------------------//
#endif//_LIGHT3D_DEF_H_