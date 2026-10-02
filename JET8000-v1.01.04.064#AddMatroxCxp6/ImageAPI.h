// ImageAPI.h: interface for the CImageAPI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEAPI_H__394DF268_82DA_43FF_86D1_D0B377EB55CA__INCLUDED_)
#define AFX_IMAGEAPI_H__394DF268_82DA_43FF_86D1_D0B377EB55CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "OpenCV_Def.h"
//-------------------------------------------------------------------------------------//
//ColorImage
//ImageW::影像寬度-水平長度
//ImageH::影像長度-垂直長度
//ImageStep::每條掃描線實際占有寬度數量
//-------------------------------------------------------------------------------------//
#include "dib.h"
#include "JetImage.h"
#include "ColorGroup.h"
#include "JetGroundEquation.h"
//-------------------------------------------------------------------------------------//
#define ARCTANGENT_TABLE_COL        1024
#define ARCTANGENT_TABLE_ROW        1024
#define ARCTANGENT_TABLE_SIZE    1048576

#define ARCTANGENT_TABLE_COL2        512
#define ARCTANGENT_TABLE_ROW2        512
//-------------------------------------------------------------------------------------//
const int CREATE_OPENCV_IMAGE_NULL   = 0;//建立OpenCV影像-空
const int CREATE_OPENCV_IMAGE_HEADER = 1;//建立OpenCV影像-擋頭
const int CREATE_OPENCV_IMAGE_BODY   = 2;//建立OpenCV影像-影像物件
//-------------------------------------------------------------------------------------//
#define MORPH_OPEN                   2//先侵蝕再膨脹
#define MORPH_CLOSE                  3//先膨脹再侵蝕
#define MORPH_GRADIENT               4//膨脹扣除侵蝕
#define MORPH_TOPHAT                 5//膨脹扣除本身
#define MORPH_BLACKHAT               6//本身扣除侵蝕
//-------------------------------------------------------------------------------------//
#define MORPH_SHAPE_RECT             0//方形遮罩
#define MORPH_SHAPE_CROSS            1//十字遮罩
#define MORPH_SHAPE_ELLIPSE          2//圓形遮罩
//-------------------------------------------------------------------------------------//
#define SOBEL_NORMAL                 0//正常的Sobel
#define SOBEL_DARK_TOP               1//上黑下白的Sobel
#define SOBEL_DARK_LEFT              2//左黑右白的Sobel
#define SOBEL_DARK_BOT               3//下黑上白的Sobel
#define SOBEL_DARK_RIGHT             4//右黑左白的Sobel
//-------------------------------------------------------------------------------------//
const int FLIP_X_AXIS                = 1;//對X軸翻轉
const int FLIP_Y_AXIS                = 2;//對Y軸翻轉
const int FLIP_XY_AXIS               = 3;//對XY軸翻轉
//-------------------------------------------------------------------------------------//
#define IMG_TYPE_000                   0//undefined
#define IMG_TYPE_08U                   1//unsigned char
#define IMG_TYPE_16U                   2//unsigned short
#define IMG_TYPE_32S                   4//int
#define IMG_TYPE_32F                   5//float
//-------------------------------------------------------------------------------------//
#define MERGE_MASK_OR                  1
#define MERGE_MASK_AND                 2
//-------------------------------------------------------------------------------------//
#define IMAGE_FILE_MODE_BMP            1
#define IMAGE_FILE_MODE_JPG            2
#define IMAGE_FILE_MODE_PNG            3
//-------------------------------------------------------------------------------------//
typedef INT32 CPUIDFIELD;
#define CPUIDFIELD_MASK_POS	           0x0000001F		// 位偏移. 0~31.
#define CPUIDFIELD_MASK_LEN	           0x000003E0		// 位長. 1~32
#define CPUIDFIELD_MASK_REG	           0x00000C00		// 寄存器. 0=EAX, 1=EBX, 2=ECX, 3=EDX.
#define CPUIDFIELD_MASK_FIDSUB         0x000FF000		// 子功能號(低8位).
#define CPUIDFIELD_MASK_FID	           0xFFF00000		// 功能號(最高4位 和 低8位).
#define CPUIDFIELD_SHIFT_POS           0
#define CPUIDFIELD_SHIFT_LEN           5
#define CPUIDFIELD_SHIFT_REG           10
#define CPUIDFIELD_SHIFT_FIDSUB        12
#define CPUIDFIELD_SHIFT_FID           20

#define CPUIDFIELD_MAKE(fid,fidsub,reg,pos,len)	(((fid)&0xF0000000) \
		| ((fid)<<CPUIDFIELD_SHIFT_FID & 0x0FF00000) \
		| ((fidsub)<<CPUIDFIELD_SHIFT_FIDSUB & CPUIDFIELD_MASK_FIDSUB) \
		| ((reg)<<CPUIDFIELD_SHIFT_REG & CPUIDFIELD_MASK_REG) \
		| ((pos)<<CPUIDFIELD_SHIFT_POS & CPUIDFIELD_MASK_POS) \
		| (((len)-1)<<CPUIDFIELD_SHIFT_LEN & CPUIDFIELD_MASK_LEN) \
		)
#define CPUIDFIELD_FID(cpuidfield)	( ((cpuidfield)&0xF0000000) | (((cpuidfield) & 0x0FF00000)>>CPUIDFIELD_SHIFT_FID) )
#define CPUIDFIELD_FIDSUB(cpuidfield)	( ((cpuidfield) & CPUIDFIELD_MASK_FIDSUB)>>CPUIDFIELD_SHIFT_FIDSUB )
#define CPUIDFIELD_REG(cpuidfield)	( ((cpuidfield) & CPUIDFIELD_MASK_REG)>>CPUIDFIELD_SHIFT_REG )
#define CPUIDFIELD_POS(cpuidfield)	( ((cpuidfield) & CPUIDFIELD_MASK_POS)>>CPUIDFIELD_SHIFT_POS )
#define CPUIDFIELD_LEN(cpuidfield)	( (((cpuidfield) & CPUIDFIELD_MASK_LEN)>>CPUIDFIELD_SHIFT_LEN) + 1 )

#ifndef __GETBITS32
#define __GETBITS32(src,pos,len)	( ((src)>>(pos)) & (((UINT32)-1)>>(32-len)) )
#endif

#define CPUF_AVX						CPUIDFIELD_MAKE(1,0,2,28,1)
#define CPUF_AVX2						CPUIDFIELD_MAKE(7,0,1,5,1)
#define CPUF_OSXSAVE					CPUIDFIELD_MAKE(1,0,2,27,1)
#define CPUF_XFeatureSupportedMaskLo	CPUIDFIELD_MAKE(0xD,0,0,0,32)
#define SIMD_AVX_NONE	0	// 不支持
#define SIMD_AVX_1		1	// AVX
#define SIMD_AVX_2		2	// AVX2
//-------------------------------------------------------------------------------------//
#pragma region Histogram
typedef uint16_t HT_DATA;
const int HIST_VAL_MIN = -500;
const int HIST_VAL_MAX = 256 * 256 * 2 - 1 + HIST_VAL_MIN;
const int HIST_RANGE = HIST_VAL_MAX - HIST_VAL_MIN;		//Range:: 0-65535,   0-16383,    0-4095,    0-1023,    0-255				
														//依循 HIST_RANGE 設定合適的 HIST_SHIFT_RIGHT、HIST_RATIO
const int HIST_SHIFT_RIGHT = 8;							//              8,         7,         6,         5,         4 
const int HIST_SIZE = 1 << HIST_SHIFT_RIGHT;			//            256,       128,        64,        32,        16      ---- Bin
const int HIST_MAX = HIST_SIZE - 1;						//            255,       127,        63,        31,        15
														/*
														Ratio: 0, 1, 2, 3,  4,  6,   8...
														Unit : 1, 2, 4, 8, 16, 64, 256...
														*/
const int HIST_RATIO = 1;

struct __declspec(align(HIST_SIZE)) SHistogram
{
	HT_DATA coarse[HIST_SIZE];
	HT_DATA fine[HIST_SIZE][HIST_SIZE];
};
#pragma endregion
//-------------------------------------------------------------------------------------//
typedef struct tagPixelCompareParam
{
	//for Source Image
	int nImgBlurSize;//模糊尺寸

	//for Pattern Image
	int nPatDarkLevel;//過暗定義
	int nPatLightLevel;//過亮定義	
	int nPatOpenSize;//開運算尺寸
	int nPatCloseSize;//閉運算尺寸
	int nPatErodeSize;//侵蝕尺寸
	bool bPatPureColor;//純色模式

	//for Compare Image
	float fCmpDarkGain;//暗部增益
	float fCmpLightGain;//亮部增益

	int nCmpDarkLevel;//過低定義
	int nCmpTolerance;//誤差閥值
	int nCmpGaussianSize;//高斯濾波
	int nCmpOpenSize;//開運算
	int nCmpCloseSize;//閉運算
	int nCmpEraseBoundary;//刪除邊界	

	int nBlobMinW;//最小區塊尺寸
	int nBlobMinH;//最小區塊尺寸
	int nBlobMinD;//最小區塊尺寸
	int nBlobMinArea;//最小區塊面積

	tagPixelCompareParam()
	{
		nImgBlurSize = 3;

		nPatDarkLevel = 60;
		nPatLightLevel = 255;
		nPatOpenSize = 0;
		nPatCloseSize = 0;
		nPatErodeSize = 0;

		fCmpDarkGain = 1.0f;
		fCmpLightGain = 1.0f;

		nCmpDarkLevel = 60;
		nCmpTolerance = 30;
		nCmpGaussianSize = 5;
		nCmpOpenSize = 3;
		nCmpCloseSize = 0;
		nCmpEraseBoundary = 0;

		nBlobMinW = 2;
		nBlobMinH = 2;
		nBlobMinD = 2;
		nBlobMinArea = 10;
	}
	bool CheckPatUseFilter() const
	{
		if ( nPatOpenSize > 0 ) { return true; }
		if ( nPatCloseSize > 0 ) { return true; }
		if ( nPatErodeSize > 0 ) { return true; }
		return false;
	}
} TPixelCompareParam, *PPixelCompareParam;
//-------------------------------------------------------------------------------------//
typedef struct tagLineEquation2D//ax+by+c=0;
{
	double    a;
	double    b;
	double    c;
	tagLineEquation2D(double ia=0, double ib=0, double ic=0):a(ia), b(ib), c(ic)
	{	}
} TLineEquation2D, *PLineEquation2D;
//-------------------------------------------------------------------------------------//
typedef struct _IMAGE
{
private:
	int              m_Type;
	int              m_Channels;//影像通道
	IMAGE_SIZE       m_ImageW;//影像寬度
	IMAGE_SIZE       m_ImageH;//影像長度
	IMAGE_SIZE       m_RowStep;//影像寬度步長
	IMAGE_SIZE       m_BitCount;//位元數	
	void            *m_Ptr; //影像指標-Gray-Color
	void            *m_Ptr1;//影像指標-R
	void            *m_Ptr2;//影像指標-G
	void            *m_Ptr3;//影像指標-B	
public:
	_IMAGE():m_Type(IMG_TYPE_000),m_Channels(1),m_ImageW(0),m_ImageH(0),m_RowStep(0),m_BitCount(0),m_Ptr(NULL),m_Ptr1(NULL),m_Ptr2(NULL),m_Ptr3(NULL)
	{}

	_IMAGE(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE RowStep, IMAGE_SIZE BitCount, unsigned char* ImagePtr):m_Type(IMG_TYPE_08U),m_Channels(1),m_ImageW(ImageW),m_ImageH(ImageH),m_RowStep(RowStep),m_BitCount(BitCount),m_Ptr(ImagePtr),m_Ptr1(NULL),m_Ptr2(NULL),m_Ptr3(NULL)
	{}

	_IMAGE(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE RowStep, IMAGE_SIZE BitCount, unsigned short* ImagePtr):m_Type(IMG_TYPE_16U),m_Channels(1),m_ImageW(ImageW),m_ImageH(ImageH),m_RowStep(RowStep),m_BitCount(BitCount),m_Ptr(ImagePtr),m_Ptr1(NULL),m_Ptr2(NULL),m_Ptr3(NULL)
	{}

	_IMAGE(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE RowStep, IMAGE_SIZE BitCount, float* ImagePtr):m_Type(IMG_TYPE_32F),m_Channels(1),m_ImageW(ImageW),m_ImageH(ImageH),m_RowStep(RowStep),m_BitCount(BitCount),m_Ptr(ImagePtr),m_Ptr1(NULL),m_Ptr2(NULL),m_Ptr3(NULL)
	{}

	_IMAGE(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE RowStep, IMAGE_SIZE BitCount, int Type, void* ImagePtr):m_Channels(1),m_ImageW(ImageW),m_ImageH(ImageH),m_RowStep(RowStep),m_BitCount(BitCount),m_Type(Type),m_Ptr(ImagePtr),m_Ptr1(NULL),m_Ptr2(NULL),m_Ptr3(NULL)
	{}

	_IMAGE(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE RowStep, IMAGE_SIZE BitCount, unsigned char* PtrR, unsigned char* PtrG, unsigned char* PtrB):m_Type(IMG_TYPE_08U),m_Channels(3),m_ImageW(ImageW),m_ImageH(ImageH),m_RowStep(RowStep),m_BitCount(BitCount),m_Ptr(NULL),m_Ptr1(PtrR),m_Ptr2(PtrG),m_Ptr3(PtrB)
	{}

	_IMAGE(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE RowStep, IMAGE_SIZE BitCount, unsigned short* PtrR, unsigned short* PtrG, unsigned short* PtrB):m_Type(IMG_TYPE_16U),m_Channels(3),m_ImageW(ImageW),m_ImageH(ImageH),m_RowStep(RowStep),m_BitCount(BitCount),m_Ptr(NULL),m_Ptr1(PtrR),m_Ptr2(PtrG),m_Ptr3(PtrB)
	{}

	_IMAGE(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE RowStep, IMAGE_SIZE BitCount, float* PtrR, float* PtrG, float* PtrB):m_Type(IMG_TYPE_32F),m_Channels(3),m_ImageW(ImageW),m_ImageH(ImageH),m_RowStep(RowStep),m_BitCount(BitCount),m_Ptr(NULL),m_Ptr1(PtrR),m_Ptr2(PtrG),m_Ptr3(PtrB)
	{}

	_IMAGE(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE RowStep, IMAGE_SIZE BitCount, int Type, void* PtrR, void* PtrG, void* PtrB):m_Channels(3),m_ImageW(ImageW),m_ImageH(ImageH),m_RowStep(RowStep),m_BitCount(BitCount),m_Type(Type),m_Ptr(NULL),m_Ptr1(PtrR),m_Ptr2(PtrG),m_Ptr3(PtrB)
	{}

	int              GetType() const { return m_Type; }
	int              GetChannels() const { return m_Channels; }
	IMAGE_SIZE       GetImageW() const { return m_ImageW; }
	IMAGE_SIZE       GetImageH() const { return m_ImageH; }
	IMAGE_SIZE       GetRowStep() const { return m_RowStep; }
	IMAGE_SIZE       GetBitCount() const { return m_BitCount; }

	void*            GetPtr() const { return m_Ptr; }//影像指標-Gray-Color
	void*            GetPtrR() const { return m_Ptr1; }//影像指標-R
	void*            GetPtrG() const { return m_Ptr2; }//影像指標-G
	void*            GetPtrB() const { return m_Ptr3; }//影像指標-B

	unsigned char*   Get08uPtr()  const { return (unsigned char*)m_Ptr; }//影像指標-Gray-Color
	unsigned char*   Get08uPtrR() const { return (unsigned char*)m_Ptr1; }//影像指標-R
	unsigned char*   Get08uPtrG() const { return (unsigned char*)m_Ptr2; }//影像指標-G
	unsigned char*   Get08uPtrB() const { return (unsigned char*)m_Ptr3; }//影像指標-B

	unsigned short*  Get16uPtr()  const { return (unsigned short*)m_Ptr; }//影像指標-Gray-Color
	unsigned short*  Get16uPtrR() const { return (unsigned short*)m_Ptr1; }//影像指標-R
	unsigned short*  Get16uPtrG() const { return (unsigned short*)m_Ptr2; }//影像指標-G
	unsigned short*  Get16uPtrB() const { return (unsigned short*)m_Ptr3; }//影像指標-B	

	float*           Get32fPtr()  const { return (float*)m_Ptr; }//影像指標-Gray-Color
	float*           Get32fPtrR() const { return (float*)m_Ptr1; }//影像指標-R
	float*           Get32fPtrG() const { return (float*)m_Ptr2; }//影像指標-G
	float*           Get32fPtrB() const { return (float*)m_Ptr3; }//影像指標-B	

} TIMAGE, *PIMAGE;
//-------------------------------------------------------------------------------------//
class CImageAPI : public CObject  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CImageAPI)
	//---------------------------------------------------------------------------------//
	static long                m_nRef;
	static CRITICAL_SECTION    m_csImageAPI;//同步化	
	static PHASE_PTR           m_pArctanTable;
	static unsigned char      *m_RGBToRedTable;
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//
	HWND                       m_hWnd;
	CString                    m_ErrorString;
	//---------------------------------------------------------------------------------//	
	bool                       IsHW_SupportAVX;
	bool                       IsHW_SupportAVX2;
	bool                       IsOS_SupportAVX;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CImageAPI(const CImageAPI &ImageAPI);
	CImageAPI& operator=(const CImageAPI &ImageAPI);
	//---------------------------------------------------------------------------------//
	void                       LockThread();
	void                       UnlockThread();
	//---------------------------------------------------------------------------------//
	void                       LockImageAPI();//鎖住影像處理同步化
	void                       UnlockImageAPI();//釋放影像處理同步化
	//---------------------------------------------------------------------------------//
	bool                       ReturnOpenMPDisableException();//回傳OpenMP的未啟用錯誤訊息
	//---------------------------------------------------------------------------------//
	double                     AdjustRotateAngle(double Angle);//調整旋轉角度
	bool                       FloodFillGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, int SeedX, int SeedY, unsigned char nNull, unsigned char nFilled, int &Level, int &Finish, std::vector<POINT> &PtList);
	//---------------------------------------------------------------------------------//				
	bool                       GetUseNewContrast() const;//20190530
	int                        CalcPhaseContrast(int a, int b, int c);		
	int                        CalcFilterCount(int FilterSize, int UseSize);
	bool                       DeductBasePhase(PHASE_DATA BasePhase, int &Phase);//扣除相平面
	float                      GetPhaseNoiseMinB(int val) const;//取得相位雜訊-最小B
	float                      GetPhaseNoiseMinCV(int val) const;//取得相位雜訊-最小CV
	float                      GetPhaseNoiseMinB(const TPhaseNoiseParam &NoiseParam) const;//取得相位雜訊-最小B
	float                      GetPhaseNoiseMinCV(const TPhaseNoiseParam &NoiseParam) const;//取得相位雜訊-最小CV
	bool                       ConvertPhaseToSpace(int CvtMode, int idx, float Phase, const float* p1, const float *p2, float &Space);//相位轉成高度值
	bool                       ConvertSpaceToPhase(int CvtMode, int idx, float Space, const float* p1, const float *p2, int &Phase);//高度轉成相位值
	bool                       ConvertPhaseToSpaceMapFunc(int CvtMode, double X, double Y, int Phase, const double* T0, const double* T1, const double* T2, float &Space);//相位轉成高度值
	bool                       ConvertSpaceToPhaseMapFunc(int CvtMode, double X, double Y, float Space, const double* T0, const double* T1, const double* T2, int &Phase);//高度轉成相位值
	MASK_DATA                  CombinePhaseMask(MASK_DATA Mask1, MASK_DATA Mask2);//合併大小週期遮罩	
	int                        CombinePhasePeriod_MaxMin(int Phase1, int Phase2, double Ratio);//合併大小週期
	int                        CombinePhasePeriod_Sibling(int Phase1, int Phase2, double Ratio);//合併鄰近週期
	int                        CombinePhasePeriod(int Mode, int Phase1, int Phase2, double Ratio);//合併兩個週期		
	bool                       CalcCombinePhasePeriodRatio(int Mode, double P1, double P2, double &Ratio);//計算合併比例
	int                        CombinePhasePeriod_GrayCode(int Phase, int Period, int NPeriod, int k1, int k2);//合併相位與GrayCode
	int                        Calc3PhaseContrast(PHASE_DATA P1, PHASE_DATA P2, PHASE_DATA P3);
	int                        Calc21PhaseContrast(PHASE_DATA P1, PHASE_DATA P2, PHASE_DATA P3);
	int                        Calc4PhaseContrast(PHASE_DATA P1, PHASE_DATA P2, PHASE_DATA P3, PHASE_DATA P4);
	int                        Calc3PhasePotential(PHASE_DATA P1, PHASE_DATA P2, PHASE_DATA P3);
	int                        Calc21PhasePotential(PHASE_DATA P1, PHASE_DATA P2, PHASE_DATA P3);
	int                        Calc4PhasePotential(PHASE_DATA P1, PHASE_DATA P2, PHASE_DATA P3, PHASE_DATA P4);
	int                        Calc5PhasePotential(PHASE_DATA P1, PHASE_DATA P2, PHASE_DATA P3, PHASE_DATA P4, PHASE_DATA P5);
	int                        Calc3PhaseSaturated(PHASE_DATA P1, PHASE_DATA P2, PHASE_DATA P3, int SatTh);
	int                        Calc4PhaseSaturated(PHASE_DATA P1, PHASE_DATA P2, PHASE_DATA P3, PHASE_DATA P4, int SatTh);	
	int                        Calc5PhaseSaturated(PHASE_DATA P1, PHASE_DATA P2, PHASE_DATA P3, PHASE_DATA P4, PHASE_DATA P5, int SatTh);
	bool                       Calc4PhaseMeanContrastRatio(int P1, int P2, int P3, int P4, float &A, float &B, float &CV);
	bool                       Check3PhaseSaturated(PHASE_DATA P1, PHASE_DATA P2, PHASE_DATA P3, int SatTh);
	bool                       Check4PhaseSaturated(PHASE_DATA P1, PHASE_DATA P2, PHASE_DATA P3, PHASE_DATA P4, int SatTh);
	bool                       Check5PhaseSaturated(PHASE_DATA P1, PHASE_DATA P2, PHASE_DATA P3, PHASE_DATA P4, PHASE_DATA P5, int SatTh);		
	bool                       Calc3PhaseParam(PHASE_DATA P1, PHASE_DATA P2, PHASE_DATA P3, int &a, int &b, PHASE_DATA &c);
	bool                       Calc21PhaseParam(PHASE_DATA P1, PHASE_DATA P2, PHASE_DATA P3, int &a, int &b, PHASE_DATA &c);//2+1
	bool                       Calc4PhaseParam(PHASE_DATA P1, PHASE_DATA P2, PHASE_DATA P3, PHASE_DATA P4, int &a, int &b, PHASE_DATA &c);
	bool                       Calc5PhaseParam(int P1, int P2, int P3, int P4, int P5, int &a, int &b, PHASE_DATA &c);
	bool                       Calc3PhaseParamMask(int P1, int P2, int P3, int NoiseMask, int LowContrast, int LowPotential, int OverSaturated, PHASE_DATA &Phase, MASK_DATA &Mask);	
	bool                       Calc21PhaseParamMask(int P1, int P2, int P3, int NoiseMask, int LowContrast, int LowPotential, int OverSaturated, PHASE_DATA &Phase, MASK_DATA &Mask);	
	bool                       Calc4PhaseParamMask(int P1, int P2, int P3, int P4, int NoiseMask, int LowContrast, int LowPotential, int OverSaturated, PHASE_DATA &Phase, MASK_DATA &Mask, int Mode);	
	bool                       Calc4PhaseParamMask_1(int P1, int P2, int P3, int P4, int NoiseMask, int LowContrast, int LowPotential, int OverSaturated, PHASE_DATA &Phase, MASK_DATA &Mask);	
	bool                       Calc4PhaseParamMask_2(int P1, int P2, int P3, int P4, int NoiseMask, int LowContrast, int LowPotential, int OverSaturated, PHASE_DATA &Phase, MASK_DATA &Mask);	
	bool                       Calc5PhaseParamMask(int P1, int P2, int P3, int P4, int P5, int NoiseMask, int LowContrast, int LowPotential, int OverSaturated, PHASE_DATA &Phase, MASK_DATA &Mask);	
	bool                       Calc4GrayCodeParam(int P1, int P2, int P3, int P4, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//4階GrayCode解碼
	bool                       Calc4GrayCodeParam_1(int P1, int P2, int P3, int P4, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//4階GrayCode解碼
	bool                       Calc4GrayCodeParam_2(int P1, int P2, int P3, int P4, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//4階GrayCode解碼
	bool                       Calc5GrayCodeParam(int P1, int P2, int P3, int P4, int P5, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//5階GrayCode解碼
	bool                       Calc5GrayCodeParam_1(int P1, int P2, int P3, int P4, int P5, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//5階GrayCode解碼
	bool                       Calc5GrayCodeParam_2(int P1, int P2, int P3, int P4, int P5, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//5階GrayCode解碼
	bool                       Calc6GrayCodeParam(int P1, int P2, int P3, int P4, int P5, int P6, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//6階GrayCode解碼
	bool                       Calc6GrayCodeParam_1(int P1, int P2, int P3, int P4, int P5, int P6, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//6階GrayCode解碼
	bool                       Calc6GrayCodeParam_2(int P1, int P2, int P3, int P4, int P5, int P6, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//6階GrayCode解碼
	bool                       AssignCastParamGCPtr(const unsigned char* MapPtr3, const unsigned char* MapPtr4, const unsigned char* MapPtr5, const unsigned char* MapPtr6,  TCastParam &CastParam);//指派投光參數的GrayCode映射指標
	bool                       Merge2ExpPhaseData(MASK_DATA Mask1, int Phase1, MASK_DATA Mask2, int Phase2, MASK_DATA &Mask, int &Phase);//合併2個曝光的相位	
	bool                       Merge2ExpPhaseData_II(MASK_DATA Mask1, int Phase1, MASK_DATA Mask2, int Phase2, MASK_DATA &Mask, int &Phase, int ZeroPhase);//合併2個曝光的相位	
	bool                       Merge2ExpPhaseDataByMean(MASK_DATA Mask1, int Phase1, float Mean1, MASK_DATA Mask2, int Phase2, float Mean2, MASK_DATA &Mask, int &Phase);//合併2個曝光的相位	
	bool                       Merge2ExpPhaseDataByMaxB(MASK_DATA Mask1, int Phase1, float A1, float B1, MASK_DATA Mask2, int Phase2, float A2, float B2, MASK_DATA &Mask, int &Phase);//合併2個曝光的相位	
	bool                       Merge2ExpPhaseDataByMaxCV_Jun(float A[], float B[], float CV[], int nBrightness, int nSaturated, int &nIdx);//合併2個曝光的相位-軍達提供
	bool                       Merge2ExpPhaseDataByMaxCV(MASK_DATA Mask1, int Phase1, float A1, float B1, MASK_DATA Mask2, int Phase2, float A2, float B2, MASK_DATA &Mask, int &Phase);//合併2個曝光的相位		
	bool                       Merge2SpaceData(MASK_DATA Mask1, float Space1, MASK_DATA Mask2, float Space2, MASK_DATA &Mask, float &Space);//合併2個高度
	bool                       Merge2SpaceData_Joe(const TPhaseNoiseParam &NoiseParam, int Raw1[], MASK_DATA Mask1, float Space1, int Raw2[], MASK_DATA Mask2, float Space2, MASK_DATA &Mask, float &Space);//合併2個高度	
	bool                       Merge2SpaceData_Joe_01(const TPhaseNoiseParam &NoiseParam, int Raw1[], MASK_DATA Mask1, float Space1, int Raw2[], MASK_DATA Mask2, float Space2, MASK_DATA &Mask, float &Space);//合併2個高度	
	bool                       Merge2SpaceData_Joe_02(const TPhaseNoiseParam &NoiseParam, int Raw1[], MASK_DATA Mask1, float Space1, int Raw2[], MASK_DATA Mask2, float Space2, MASK_DATA &Mask, float &Space);//合併2個高度	
	bool                       Merge3SpaceData(MASK_DATA Mask1, float Space1, MASK_DATA Mask2, float Space2, MASK_DATA Mask3, float Space3, MASK_DATA &Mask, float &Space);//合併3個高度
	bool                       Merge3SpaceData_Joe(const TPhaseNoiseParam &NoiseParam, int Raw1[], MASK_DATA Mask1, float Space1, int Raw2[], MASK_DATA Mask2, float Space2, int Raw3[], MASK_DATA Mask3, float Space3, MASK_DATA &Mask, float &Space);//合併3個高度	
	bool                       Merge3SpaceData_Joe_01(const TPhaseNoiseParam &NoiseParam, int Raw1[], MASK_DATA Mask1, float Space1, int Raw2[], MASK_DATA Mask2, float Space2, int Raw3[], MASK_DATA Mask3, float Space3, MASK_DATA &Mask, float &Space);//合併3個高度	
	bool                       Merge3SpaceData_Joe_02(const TPhaseNoiseParam &NoiseParam, int Raw1[], MASK_DATA Mask1, float Space1, int Raw2[], MASK_DATA Mask2, float Space2, int Raw3[], MASK_DATA Mask3, float Space3, MASK_DATA &Mask, float &Space);//合併3個高度	
	bool                       Merge4SpaceData(MASK_DATA Mask1, float Space1, MASK_DATA Mask2, float Space2, MASK_DATA Mask3, float Space3, MASK_DATA Mask4, float Space4, MASK_DATA &Mask, float &Space);//合併4個高度
	bool                       Merge4SpaceData_Joe(const TPhaseNoiseParam &NoiseParam, int Raw1[], MASK_DATA Mask1, float Space1, int Raw2[], MASK_DATA Mask2, float Space2, int Raw3[], MASK_DATA Mask3, float Space3, int Raw4[], MASK_DATA Mask4, float Space4, MASK_DATA &Mask, float &Space);//合併4個高度	
	bool                       Merge4SpaceData_Joe_01(const TPhaseNoiseParam &NoiseParam, int Raw1[], MASK_DATA Mask1, float Space1, int Raw2[], MASK_DATA Mask2, float Space2, int Raw3[], MASK_DATA Mask3, float Space3, int Raw4[], MASK_DATA Mask4, float Space4, MASK_DATA &Mask, float &Space);//合併4個高度	
	bool                       Merge4SpaceData_Joe_02(const TPhaseNoiseParam &NoiseParam, int Raw1[], MASK_DATA Mask1, float Space1, int Raw2[], MASK_DATA Mask2, float Space2, int Raw3[], MASK_DATA Mask3, float Space3, int Raw4[], MASK_DATA Mask4, float Space4, MASK_DATA &Mask, float &Space);//合併4個高度	
	bool                       MergeNSpaceData(const MASK_DATA MaskList[], const float SpaceList[], int SpaceCount, MASK_DATA &Mask, float &Space);//合併多個高度
	bool                       MergeNSpaceData_General(const MASK_DATA MaskList[], const float SpaceList[], int SpaceCount, MASK_DATA &Mask, float &Space);//合併多個高度
	bool                       MergeNSpaceData_Median(const MASK_DATA MaskList[], const float SpaceList[], int SpaceCount, MASK_DATA &Mask, float &Space);//合併多個高度
	bool                       MergeNSpaceData(const std::vector<MASK_DATA> &MaskList, const std::vector<float> &SpaceList, MASK_DATA &Mask, float &Space);//合併多個高度
	bool                       Compare4OppositeGray(int P1, int P2, int P3, int P4, MASK_DATA &Mask1, MASK_DATA &Mask2, MASK_DATA &Mask3, MASK_DATA &Mask4);//比較相對打燈灰階
	bool                       Compare2SpaceHeight(MASK_DATA &Mask1, float Space1, MASK_DATA &Mask2, float Space2, MASK_DATA &Mask, float &Space);//比較空間高度
	bool                       Compare3SpaceHeight(MASK_DATA &Mask1, float Space1, MASK_DATA &Mask2, float Space2, MASK_DATA &Mask3, float Space3, MASK_DATA &Mask, float &Space);//比較空間高度
	bool                       Compare4SpaceHeight(MASK_DATA &Mask1, float Space1, MASK_DATA &Mask2, float Space2, MASK_DATA &Mask3, float Space3, MASK_DATA &Mask4, float Space4, MASK_DATA &Mask, float &Space);//比較空間高度	
	bool                       Compare4SpaceHeight_1(MASK_DATA &Mask1, float Space1, MASK_DATA &Mask2, float Space2, MASK_DATA &Mask3, float Space3, MASK_DATA &Mask4, float Space4, MASK_DATA &Mask, float &Space);//比較空間高度
	bool                       Compare4SpaceHeight_2(MASK_DATA &Mask1, float Space1, MASK_DATA &Mask2, float Space2, MASK_DATA &Mask3, float Space3, MASK_DATA &Mask4, float Space4, MASK_DATA &Mask, float &Space, bool bChkMask);//比較空間高度	
	bool                       Compare4SpaceHeight_3(MASK_DATA &Mask1, float Space1, MASK_DATA &Mask2, float Space2, MASK_DATA &Mask3, float Space3, MASK_DATA &Mask4, float Space4, MASK_DATA &Mask, float &Space);//比較空間高度	
	bool                       CompareNSpaceHeight(const MASK_DATA MaskList[], const float SpaceList[], int SpaceCount, MASK_DATA &Mask, float &Space);//比較空間高度
	bool                       CompareNSpaceHeight(const std::vector<MASK_DATA> &MaskList, const std::vector<float> &SpaceList, MASK_DATA &Mask, float &Space);//比較空間高度	
	int                        GetMassGroupIndex(int GroupCnt[], int Count);//取得群組數量最多的引數
	int                        GetMassGroupIndex(const std::vector<int> &GroupCntList);//取得群組數量最多的引數
	bool                       AddGroupSpcaeHeightByMean(float Space, float Range, float &Group, int &GroupCnt);//加入高度分群-依照平均值
	bool                       AddGroupSpcaeHeightByRange(float Space, float Range, float &GroupMin, float &GroupMax, float &GroupAve, int &GroupCnt);//加入高度分群-依照範圍值
	int                        GetMinValueIndex(float SpaceList[], int StartIdx, int SpaceCnt);//取得最低值引數	
	bool                       CheckArraySorted(float SpaceList[], int SpaceCnt);//確認陣列排序過
	bool                       SortSpcaeHeight(std::vector<float> &SpaceList);//高度排序
	bool                       SortSpcaeHeight(float SpaceList[], int SpaceCnt);//高度排序
	bool                       SortSpcaeHeight_Insertion(float SpaceList[], int SpaceCnt);//高度排序-插入法
	bool                       SortSpcaeHeight_Selection(float SpaceList[], int SpaceCnt);//高度排序-選擇法
	void                       QuickSort_Swap(float &a, float &b);//快速排序-交換
	int                        QuickSort_Partition(float array[], int front, int end);//快速排序-分割
	bool                       QuickSort_Recursion(float array[], int front, int end);//快速排序-遞迴
	bool                       QuickSort_Iterative(float array[], int front, int end);//快速排序-疊代
	bool                       SortSpcaeHeight_QuickSort(float SpaceList[], int SpaceCnt);//高度排序-快速排序
	bool                       MergeSort_Merge(float array[], int front, int mid, int end);//合併排序-合併
	bool                       MergeSort_Recursion(float array[], int front, int end);//合併排序-遞迴
	bool                       MergeSort_Iterative(float array[], int count);//合併排序-疊代
	bool                       SortSpcaeHeight_MergeSort(float SpaceList[], int SpaceCnt);//高度排序-合併排序
	float                      AverageSpaceHeight(const float SpaceList[],int SpaceCnt);//平均高度
	float                      AverageSpaceHeight(const std::vector<float> &SpaceList);//平均高度
	bool                       GeometryMedianSpaceHeight(const float SpaceList[], int SpaceCnt, float ZeroPlane, float &Space);//幾何中值法
	bool                       GeometryMedianSpaceHeightKernel(const float SpaceList[], int SpaceCnt, float &Space);//幾何中值法
	bool                       GroupSpcaeHeightByMean(const float SpaceList[], float Range, float GroupAveList[], int GroupCntList[], int SpaceCnt, int &GoupCount);//多高度分群
	bool                       GroupSpcaeHeightByMean(const std::vector<float> &SpaceList, float Range, std::vector<float> &GroupAveList, std::vector<int> &GroupCntList);//多高度分群
	bool                       GroupSpcaeHeightByRange(const float SpaceList[], float Range, float GroupMinList[], float GroupMaxList[], float GroupAveList[], int GroupCntList[], int SpaceCnt, int &GoupCount);//多高度分群	
	bool                       GroupSpcaeHeightByRange(const std::vector<float> &SpaceList, float Range, std::vector<float> &GroupMinList, std::vector<float> &GroupMaxList, std::vector<float> &GroupAveList, std::vector<int> &GroupCntList);//多高度分群	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CImageAPI();
	virtual ~CImageAPI();
	//---------------------------------------------------------------------------------//
	void                       SetErrorString(LPCSTR str);
	void                       SetErrorString(LPCWSTR str);
	LPCTSTR                    GetImageApiErrorString() const;
	//---------------------------------------------------------------------------------//
	bool                       CheckEven(int val);//確認偶數
	bool                       CheckEven(unsigned int val);//確認偶數
	//---------------------------------------------------------------------------------//	
	bool                       CheckRGBToRedTable();		
	int                        CalcGamma(int Gray, float Gamma);//計算Gamma
	bool                       CheckIsColor(IMAGE_SIZE BitCount);//確認是否彩色
	bool                       CheckBitCount(IMAGE_SIZE BitCount);//確認位元數
	bool                       CheckGrayBitCount(IMAGE_SIZE BitCount);//確認灰階位元數
	bool                       CheckSpaceMaskValid(MASK_DATA Mask);//確認是否為有效值
	bool                       CheckSpaceMaskValidBest(MASK_DATA Mask);//確認是否為有效值
	bool                       CheckBayerPattern(BAYER_PATTERN_MODE Mode);//確認Bayer樣板
	BAYER_PATTERN_MODE         ShiftBayerPattern(BAYER_PATTERN_MODE Bayer, int PosX, int PosY);//取得Bayer樣板
	//---------------------------------------------------------------------------------//	
	size_t                     CalcBufferSize(IMAGE_SIZE ImageStep, IMAGE_SIZE ImageH) const;//計算記憶體大小
	//---------------------------------------------------------------------------------//	
	bool                       CheckPtr(const void *Ptr1);//確認指標
	bool                       CheckPtr2(const void *Ptr1, const void *Ptr2);//確認指標-2
	bool                       CheckPtr3(const void *Ptr1, const void *Ptr2, const void *Ptr3);//確認指標-3
	bool                       CheckPtr4(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4);//確認指標-4
	bool                       CheckPtr5(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5);//確認指標-5
	bool                       CheckPtr6(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6);//確認指標-6
	bool                       CheckPtr7(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7);//確認指標-7
	bool                       CheckPtr8(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8);//確認指標-8	
	bool                       CheckPtr9(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9);//確認指標-9
	bool                       CheckPtr10(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10);//確認指標-10
	bool                       CheckPtr11(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11);//確認指標-11
	bool                       CheckPtr12(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12);//確認指標-12
	bool                       CheckPtr13(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13);//確認指標-13
	bool                       CheckPtr14(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14);//確認指標-14
	bool                       CheckPtr15(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15);//確認指標-15
	bool                       CheckPtr16(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16);//確認指標-16
	bool                       CheckPtr17(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17);//確認指標-17
	bool                       CheckPtr18(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18);//確認指標-18
	bool                       CheckPtr19(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18, const void *Ptr19);//確認指標-19
	bool                       CheckPtr20(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18, const void *Ptr19, const void *Ptr20);//確認指標-20
	bool                       CheckPtr21(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18, const void *Ptr19, const void *Ptr20, const void *Ptr21);//確認指標-21
	bool                       CheckPtr22(const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18, const void *Ptr19, const void *Ptr20, const void *Ptr21, const void *Ptr22);//確認指標-22
	//---------------------------------------------------------------------------------//
	bool                       CheckPtrNoRepeat2(const void *Ptr1, const void *Ptr2);//確認指標重複
	//---------------------------------------------------------------------------------//	
	bool                       CheckRoiSize(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, int RoiW, int RoiH);//確認Roi尺寸 
	bool                       CheckRoiRect(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &RoiRect);//確認Roi範圍
	bool                       CheckRoiRect(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const TRECT4D &RoiRect);//確認Roi範圍	
	bool                       CheckPoint2(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const POINT &pt1, const POINT &pt2);//確認兩點位置範圍
	bool                       CheckImageSize(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH);//確認影像尺寸
	bool                       CheckGraySize(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep);//確認灰階尺寸
	bool                       CheckColorSize(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep);//確認彩色尺寸
	//---------------------------------------------------------------------------------//	
	IMAGE_SIZE                 GetImageChannels(IMAGE_SIZE BitCount);//取得影像通道數	
	IMAGE_SIZE                 GetImageRealWidth(IMAGE_SIZE ImageW, IMAGE_SIZE BitCount);//取得影像實際寬度
	IMAGE_SIZE                 GetImageAlignedWidth(IMAGE_SIZE ImageW, IMAGE_SIZE BitCount, int Align);//取得影像對齊寬度
	//---------------------------------------------------------------------------------//
	bool                       CheckImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const void *pImage);//確認影像	
	bool                       CheckImage2(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const void *Ptr1, const void *Ptr2);//確認影像	
	bool                       CheckImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const void *Ptr1, const void *Ptr2, const void *Ptr3);//確認影像	

	bool                       CheckGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *pImage);//確認灰階影像	
	bool                       CheckGrayImage2(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2);//確認灰階影像-2
	bool                       CheckGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3);//確認灰階影像-3
	bool                       CheckGrayImage4(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4);//確認灰階影像-4
	bool                       CheckGrayImage5(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5);//確認灰階影像-5
	bool                       CheckGrayImage6(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6);//確認灰階影像-6
	bool                       CheckGrayImage7(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7);//確認灰階影像-7
	bool                       CheckGrayImage8(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8);//確認灰階影像-8
	bool                       CheckGrayImage9(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9);//確認灰階影像-9
	bool                       CheckGrayImage10(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10);//確認灰階影像-10
	bool                       CheckGrayImage11(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11);//確認灰階影像-11
	bool                       CheckGrayImage12(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12);//確認灰階影像-12
	bool                       CheckGrayImage13(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13);//確認灰階影像-13
	bool                       CheckGrayImage14(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14);//確認灰階影像-14
	bool                       CheckGrayImage15(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15);//確認灰階影像-15
	bool                       CheckGrayImage16(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16);//確認灰階影像-16
	bool                       CheckGrayImage17(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17);//確認灰階影像-17
	bool                       CheckGrayImage18(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18);//確認灰階影像-18
	bool                       CheckGrayImage19(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18, const void *Ptr19);//確認灰階影像-19
	bool                       CheckGrayImage20(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18, const void *Ptr19, const void *Ptr20);//確認灰階影像-20
	bool                       CheckGrayImage21(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18, const void *Ptr19, const void *Ptr20, const void *Ptr21);//確認灰階影像-21
	bool                       CheckGrayImage22(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18, const void *Ptr19, const void *Ptr20, const void *Ptr21, const void *Ptr22);//確認灰階影像-22

	bool                       CheckColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *pImage);//確認彩色影像
	bool                       CheckColorImage2(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2);//確認彩色影像-2
	bool                       CheckColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3);//確認彩色影像-3
	bool                       CheckColorImage4(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4);//確認彩色影像-4
	bool                       CheckColorImage5(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5);//確認彩色影像-5
	bool                       CheckColorImage6(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6);//確認彩色影像-6
	bool                       CheckColorImage7(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7);//確認彩色影像-7
	bool                       CheckColorImage8(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8);//確認彩色影像-8
	bool                       CheckColorImage9(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9);//確認彩色影像-9
	bool                       CheckColorImage10(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10);//確認彩色影像-10
	bool                       CheckColorImage11(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11);//確認彩色影像-11
	bool                       CheckColorImage12(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12);//確認彩色影像-12
	bool                       CheckColorImage13(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13);//確認彩色影像-13
	bool                       CheckColorImage14(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14);//確認彩色影像-14
	bool                       CheckColorImage15(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15);//確認彩色影像-15
	bool                       CheckColorImage16(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16);//確認彩色影像-16
	bool                       CheckColorImage17(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17);//確認彩色影像-17
	bool                       CheckColorImage18(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18);//確認彩色影像-18

	bool                       CheckGrayImageList(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const std::vector<unsigned char*> &PtrList);//確認灰階影像列表
	bool                       CheckColorImageList(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const std::vector<unsigned char*> &PtrList);//確認灰階影像列表
	bool                       CheckSpaceMaskList(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const std::vector<float*> &PtrList, const std::vector<unsigned char*> &MskList);//確認高度列表

	bool                       CheckCastParamExp2(const TCastParam &CastParam);//確認投光2曝光資料
	bool                       CheckCastParam(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam);//確認投光資料
	//---------------------------------------------------------------------------------//	
	bool                       BuildRGBVTable();//建立RGBV表
	bool                       DeleteRGBVTable();//刪除RGBV表
	bool                       BuildArctangentTable();//建立arctangent表-加速用	
	bool                       DeleteArctangentTable();//刪除arctangent表-加速用	
	unsigned char*             BuildLocalGammaTable(float ScaleVal);//建立局部Gamma的表格
	bool                       CalcArctangent(int y, int x, PHASE_DATA &val);//利用查表來取得atan
	double                     Calc2PhaseNewPeriode(double P1, double P2);//計算新週期
	double                     CalcBestPeriode3(double P1, double P2, double P3);//計算最好的新週期
	bool                       BuildGainOffsetTable(double Offset, double Gain, int nTable, unsigned char *Table);
	//---------------------------------------------------------------------------------//	
	bool                       LoadHeightCorrectFile();//載入高度校正參數檔案
	bool                       LoadHeightCorrectFile(LPCTSTR filename);//載入高度校正參數檔案
	//---------------------------------------------------------------------------------//
	bool                       CreateBMPInfoBuffer(BITMAPINFO *&pBMPInfo, size_t &BufferSize);//建立BMP的資訊檔案	
	bool                       SetBMPInfo(BITMAPINFO *pBMPInfo, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE BitCount);//建立BMP的資訊檔案	
	bool                       CreateBMPInfo(BITMAPINFO *&pBMPInfo, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE BitCount);//建立BMP的資訊檔案		
	//---------------------------------------------------------------------------------//	
	bool                       CalcImageWndZoom(double OldZoom, double NewZoom, TPOINT2D &OffsetPt);//計算影像視窗縮放參數
	bool                       CalcImageWndFitZoom(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const double Ratio, double &Zoom);//計算影像視窗縮放參數
	//---------------------------------------------------------------------------------//	
	bool                       DrawImageToDC(HDC hDC, const BITMAPINFO *pImageInfo, const unsigned char *pImage, const RECT &ImageWndRect, const TPOINT2D &OffsetPt2D, double ZoomScale, COLORREF clrBK);
	bool                       DrawImageToDC(HDC hDC, const BITMAPINFO *pImageInfo, const unsigned char *pImage, const RECT &ImageWndRect, const TPOINT2D &OffsetPt2D, double ZoomScale, COLORREF clrBK, int BltMode);
	bool                       DrawImageToDC(HDC hDC, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, const RECT &ImageWndRect, const TPOINT2D &OffsetPt2D, double ZoomScale, COLORREF clrBK);
	bool                       DrawImageToDC(HDC hDC, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, const RECT &ImageWndRect, const TPOINT2D &OffsetPt2D, double ZoomScale, COLORREF clrBK, int BltMode);
	//---------------------------------------------------------------------------------//	
	bool                       MapWndPtToImagePt_INT(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const POINT &OffsetPt, double ZoomScale, const POINT &WndPt, POINT &ImagePt);	
	bool                       MapWndPtToImagePt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &WndPt, TPOINT2D &ImagePt);
	bool                       MapWndRgnToImageRgn_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TREGION4D &WndRgn, TREGION4D &ImageRgn);
	bool                       MapWndRectToImageRect_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TRECT4D &TWndRect, TRECT4D &ImageRect);
	//---------------------------------------------------------------------------------//	
	bool                       MapImagePtToWndPt_INT(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const POINT &OffsetPt, double ZoomScale, const POINT &ImagePt, POINT &WndPt);
	bool                       MapImagePtToWndPt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &ImagePt, TPOINT2D &WndPt);
	bool                       MapImageRectToWndRect_INT(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const POINT &OffsetPt, double ZoomScale, const RECT &ImageRc, RECT &WndRc);
	bool                       MapImageRectToWndRect_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TRECT4D &ImageRc, TRECT4D &WndRc);
	bool                       MapImageRgnToWndRgn_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TREGION4D &ImageRgn, TREGION4D &WndRgn);
	bool                       MapImageCornerPtToWndCornerPt_INT(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const POINT &OffsetPt, double ZoomScale, const POINT ImagePt[], POINT WndPt[]);
	bool                       MapImageCornerPtToWndCornerPt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D ImagePt[], TPOINT2D WndPt[]);
	//---------------------------------------------------------------------------------//	
	bool                       DrawRectLine(HDC hDC, const RECT &Rect);//繪製矩形線段		
	bool                       DrawRectLine(HDC hDC, const POINT &pt1, const POINT &pt2);//繪製矩形線段		
	bool                       DrawRectLine(HDC hDC, const POINT &pt, int HalfW, int HalfH);//繪製矩形線段	
	bool                       DrawRectRoughLine(HDC hDC, const RECT &Rect, COLORREF color, int Gap);//繪製矩形粗糙線段
	//---------------------------------------------------------------------------------//	
	bool                       DrawEllipseLine(HDC hDC, const RECT &Rect);//繪製圓形線段		
	//---------------------------------------------------------------------------------//	
	bool                       DrawPolyLine(HDC hDC, POINT *PtList, int NPts);//繪製多邊形
	bool                       DrawPolyLine(HDC hDC, TPOINT2D *PtList, int NPts);//繪製多邊形
	//---------------------------------------------------------------------------------//
	bool                       DrawCrosshair(HDC hDC, const POINT &Pt, const RECT &Rect, COLORREF clr);//繪製十字線
	//---------------------------------------------------------------------------------//	
	void                       SetCallBackWnd(HWND hWnd) { m_hWnd = hWnd; }
	HWND                       GetCallBackWnd() const { return m_hWnd; }
	//---------------------------------------------------------------------------------//
	int                        Round(double val);//四捨五入
	int                        Ceil(double val);//無條件進入(2.1=>3)
	int                        Floor(double val);//無條件捨去(2.9=>2)
	//---------------------------------------------------------------------------------//
	int                        GetKernelSize(int val);//取得核心尺寸
	double                     CalcImageWndScale(IMAGE_SIZE ImageSize, IMAGE_SIZE WndSize);//計算圖像與視窗比例
	double                     CalcImageWndFitScale(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect);//計算圖像與視窗比例
	//---------------------------------------------------------------------------------//
	bool                       LoadImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, unsigned char *&pImage, int Align, bool Reverse);//讀取圖檔

	bool                       SaveImage(LPCTSTR pfilename, const TUNI_FRAME &UniFrame, bool Reverse);//儲存圖檔
	bool                       SaveImage(LPCTSTR pfilename, const TUNI_FRAME &UniFrame, const unsigned char *pImage, bool Reverse);//儲存圖檔
	bool                       SaveImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, bool Reverse);//儲存圖檔
	bool                       SaveRGBImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, bool Reverse);//儲存圖檔
	//---------------------------------------------------------------------------------//		
	bool                       SaveCastParamImage(LPCTSTR Folder, const TCastParam CastParamList[], int CastCnt);//儲存3D投光收到的圖檔
	//---------------------------------------------------------------------------------//
	bool                       SaveUniFrameImage_Offline(LPCTSTR pfilename, const std::vector<TUNI_FRAME> &UniFrameList, bool UseRawSpace);//儲存通用圖檔-離線編程使用
	//---------------------------------------------------------------------------------//
	bool                       SaveUniFrameImage(LPCTSTR pfilename, const std::vector<TUNI_FRAME> &UniFrameList, bool Reverse, bool Enhance, bool Save3D, bool Append, double SpaceRatio);//儲存通用圖檔
	bool                       LoadUniFrameImage(LPCTSTR pfilename, std::vector<TUNI_FRAME> &UniFrameList, bool Reverse, size_t Count);//載入通用圖檔
	//---------------------------------------------------------------------------------//	
	//BMP圖檔
	bool                       LoadBMPFile(LPCTSTR pfilename, CDib &dib);//載入BMP圖檔
	bool                       LoadBMPImage(LPCTSTR pfilename, int Align, bool Reverse, TIMAGE &Image);//讀取BMP圖檔	
	bool                       LoadBMPImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, unsigned char *&pImage, int Align, bool Reverse);//讀取BMP圖檔	
	
	bool                       LoadBMPGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage, int Align, bool Reverse);//讀取BMP灰階圖檔
	bool                       LoadBMPColorImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage, int Align, bool Reverse);//讀取BMP彩色圖檔
	bool                       LoadBMPRGBImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pR, unsigned char *&pG, unsigned char *&pB, int Align, bool Reverse);//讀取BMP-RGB圖檔
	
	bool                       SaveBMPImage(LPCTSTR pfilename, TIMAGE &Image, bool Reverse);//儲存BMP圖檔
	bool                       SaveBMPImage(LPCTSTR pfilename, const TUNI_FRAME &UniFrame, bool Reverse);//儲存BMP圖檔
	bool                       SaveBMPImage(LPCTSTR pfilename, const TUNI_FRAME &UniFrame, const unsigned char *pImage, bool Reverse);//儲存BMP圖檔
	bool                       SaveBMPImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, bool Reverse);//儲存BMP圖檔
	bool                       SaveBMPGrayImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, bool Reverse);//儲存BMP灰階圖檔
	bool                       SaveBMPColorImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, bool Reverse);//儲存BMP彩色圖檔
	bool                       SaveBMPRGBImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, bool Reverse);//儲存BMP-RGB圖檔
	//---------------------------------------------------------------------------------//	
	//JPEG圖檔
	bool                       LoadJPGImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, unsigned char *&pImage, int Align, bool Reverse);//讀取JPG灰階圖檔
	bool                       LoadJPGGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage, int Align, bool Reverse);//讀取JPG灰階圖檔
	bool                       LoadJPGColorImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage, int Align, bool Reverse);//讀取JPG彩色圖檔

	bool                       SaveJPGImage(LPCTSTR pfilename, const TUNI_FRAME &UniFrame, bool Reverse);//儲存JPG圖檔
	bool                       SaveJPGImage(LPCTSTR pfilename, const TUNI_FRAME &UniFrame, const unsigned char *pImage, bool Reverse);//儲存JPG圖檔
	bool                       SaveJPGImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, bool Reverse);//儲存JPG圖檔
	bool                       SaveJPGGrayImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, bool Reverse);//儲存JPG灰階圖檔
	bool                       SaveJPGColorImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, bool Reverse);//儲存JPG彩色圖檔	
	bool                       SaveJPGRGBImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, bool Reverse);//儲存JPG-RGB圖檔
	//---------------------------------------------------------------------------------//	
	//PNG圖檔
	bool                       LoadPNGImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, unsigned char *&pImage, int Align, bool Reverse);//讀取PNG灰階圖檔
	bool                       LoadPNGGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage, int Align, bool Reverse);//讀取PNG灰階圖檔
	bool                       LoadPNGColorImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage, int Align, bool Reverse);//讀取PNG彩色圖檔

	bool                       SavePNGImage(LPCTSTR pfilename, const TUNI_FRAME &UniFrame, bool Reverse);//儲存PNG圖檔
	bool                       SavePNGImage(LPCTSTR pfilename, const TUNI_FRAME &UniFrame, const unsigned char *pImage, bool Reverse);//儲存PNG圖檔
	bool                       SavePNGImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, bool Reverse);//儲存PNG圖檔
	bool                       SavePNGGrayImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, bool Reverse);//儲存PNG灰階圖檔
	bool                       SavePNGColorImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, bool Reverse);//儲存PNG彩色圖檔	
	bool                       SavePNGRGBImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, bool Reverse);//儲存PNG-RGB圖檔
	//---------------------------------------------------------------------------------//	
	//Mask圖檔	
	bool                       LoadMaskGrayImage(LPCTSTR pfilename, IMAGE_SIZE &MaskW, IMAGE_SIZE &MaskH, IMAGE_SIZE &MaskStep, MASK_PTR &MaskPtr, bool Reverse);//讀取Mask灰階圖檔
	bool                       SaveMaskGrayImage(LPCTSTR pfilename, const TUNI_FRAME &UniFrame, bool Reverse);//儲存Mask灰階圖檔	
	bool                       SaveMaskGrayImage(LPCTSTR pfilename, IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, const MASK_PTR MaskPtr, bool Reverse);//儲存Mask灰階圖檔	
	//---------------------------------------------------------------------------------//	
	//Float圖檔
	bool                       GrayImageConvertToFloat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR ImagePtr, float *&FloatPtr, bool Reverse);//灰階圖檔轉浮點數
	bool                       GrayImageConvertToFloat3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR ImagePtr, float *FloatPtr, bool Reverse);//灰階圖檔轉浮點數

	bool                       GrayPhaseConvertToFloat(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, PHASE_PTR PhasePtr, float *&FloatPtr, bool Reverse);//灰階相位轉浮點數
	bool                       GrayPhaseConvertToFloat3(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, PHASE_PTR PhasePtr, float *FloatPtr, bool Reverse);//灰階相位轉浮點數

	bool                       GraySpaceConvertToFloat(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, SPACE_PTR SpacePtr, float *&FloatPtr, bool Reverse);//灰階相位轉浮點數
	bool                       GraySpaceConvertToFloat3(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, SPACE_PTR SpacePtr, float *FloatPtr, bool Reverse);//灰階相位轉浮點數

	bool                       SaveGrayImageFloatFile(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_PTR ImagePtr, bool Reverse);//儲存浮點數灰階圖檔	
	bool                       SaveGrayPhaseFloatFile(LPCTSTR pfilename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, PHASE_PTR PhasePtr, bool Reverse);//儲存浮點數灰階相位
	bool                       SaveGraySpaceFloatFile(LPCTSTR pfilename, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, SPACE_PTR SpacePtr, bool Reverse);//儲存浮點數灰階相位
	bool                       SaveGrayFloatFile(LPCTSTR pfilename, IMAGE_SIZE FloatW, IMAGE_SIZE FloatH, IMAGE_SIZE FloatStep, SPACE_PTR FloatPtr, bool Reverse);//儲存浮點數灰階浮點數
	bool                       LoadGrayFloatFile(LPCTSTR pfilename, IMAGE_SIZE &FloatW, IMAGE_SIZE &FloatH, IMAGE_SIZE &FloatStep, IMAGE_SIZE &BitCount, SPACE_PTR &FloatPtr, bool Reverse, int nAlign);//載入浮點數灰階浮點數
	//---------------------------------------------------------------------------------//		
	//Space數據
	bool                       SaveSpaceValueFile(LPCTSTR pfilename, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr);//儲存Space數據檔案
	//---------------------------------------------------------------------------------//	
	//Space圖檔	
	bool                       LoadSpaceGrayImage(LPCTSTR pfilename, IMAGE_SIZE &SpaceW, IMAGE_SIZE &SpaceH, IMAGE_SIZE &SpaceStep, SPACE_PTR &SpacePtr, bool Reverse);//讀取Space灰階圖檔
	bool                       SaveSpaceGrayImage(LPCTSTR pfilename, const TUNI_FRAME &UniFrame, bool Reverse);//儲存Space灰階圖檔	
	bool                       SaveSpaceGrayImage(LPCTSTR pfilename, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, bool Reverse);//儲存Space灰階圖檔	
	//---------------------------------------------------------------------------------//	
	//Phase Bin檔案	
	bool                       LoadPhaseBinFile(LPCTSTR pfilename, IMAGE_SIZE &PhaseW, IMAGE_SIZE &PhaseH, IMAGE_SIZE &PhaseStep, PHASE_PTR &PhasePtr);//讀取Phase Bin檔案
	bool                       SavePhaseBinFile(LPCTSTR pfilename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr);//儲存Phase Bin檔案
	bool                       Compare2PhaseBinFile(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr1, const PHASE_PTR PhasePtr2, int &ErrCnt);//比較2個Phase Bin檔案
	//---------------------------------------------------------------------------------//	
	//Space Bin檔案
	bool                       LoadSpaceBinFile(LPCTSTR pfilename, IMAGE_SIZE &SpaceW, IMAGE_SIZE &SpaceH, IMAGE_SIZE &SpaceStep, SPACE_PTR &SpacePtr);//讀取Space Bin檔案
	bool                       Compare2SpaceBinFile(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr1, const SPACE_PTR SpacePtr2, int &ErrCnt);//比較2個Space Bin檔案
	//---------------------------------------------------------------------------------//	
	//圖像複製
	bool                       CloneImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned char *&pDest, bool Reverse);//複製圖像
	bool                       CloneImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned char *pDest, bool Reverse);//複製圖像
	bool                       CloneGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *&pDest, bool Reverse);//複製圖像
	bool                       CloneGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *pDest, bool Reverse);//複製圖像
	bool                       ClonePhaseGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR pImage, PHASE_PTR &pDest, bool Reverse);//複製圖像
	bool                       ClonePhaseGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR pImage, PHASE_PTR pDest, bool Reverse);//複製圖像
	bool                       CloneSpaceGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR pImage, SPACE_PTR &pDest, bool Reverse);//複製圖像
	bool                       CloneSpaceGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR pImage, SPACE_PTR pDest, bool Reverse);//複製圖像

	bool                       CloneColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *&pDest, bool Reverse);//複製圖像
	bool                       CloneColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *pDest, bool Reverse);//複製圖像
	bool                       ClonePhaseColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR pImage, PHASE_PTR &pDest, bool Reverse);//複製圖像
	bool                       CloneShortColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR pImage, PHASE_PTR pDest, bool Reverse);//複製圖像

	bool                       CloneRGBImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, unsigned char *&pDestR, unsigned char *&pDestG, unsigned char *&pDestB, bool Reverse);//複製圖像
	bool                       CloneRGBImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, unsigned char *pDestR, unsigned char *pDestG, unsigned char *pDestB, bool Reverse);//複製圖像
	bool                       ClonePhaseRGBImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR pR, const PHASE_PTR pG, const PHASE_PTR pB, PHASE_PTR &pDestR, PHASE_PTR &pDestG, PHASE_PTR &pDestB, bool Reverse);//複製圖像
	bool                       ClonePhaseRGBImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR pR, const PHASE_PTR pG, const PHASE_PTR pB, PHASE_PTR pDestR, PHASE_PTR pDestG, PHASE_PTR pDestB, bool Reverse);//複製圖像
	//---------------------------------------------------------------------------------//	
	//灰階彩色圖形轉換-I
	bool                       ColorImageToGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE StepClr, const unsigned char *pColor, int StepGry, unsigned char *&pGray, float wR=0.299f, float wG=0.587f, float wB=0.114f);//彩色轉灰階
	bool                       ColorImageToGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE StepClr, const unsigned char *pColor, int StepGry, unsigned char *pGray, float wR=0.299f, float wG=0.587f, float wB=0.114f);//彩色轉灰階
	bool                       RGBImageToGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, int StepGry, unsigned char *&pGray, float wR=0.299f, float wG=0.587f, float wB=0.114f);//RGB轉灰階
	bool                       RGBImageToGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, int StepGry, unsigned char *pGray, float wR=0.299f, float wG=0.587f, float wB=0.114f);//RGB轉灰階
	//---------------------------------------------------------------------------------//	
	//灰階彩色圖形轉換-2
	bool                       ColorImageToGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE StepClr, const unsigned char *pColor, const RECT &RoiRect, int StepGry, unsigned char *&pGray, IMAGE_SRC_MODE Mode, int WR, int WG, int WB, bool Reverse);//彩色轉灰階
	bool                       ColorImageToGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE StepClr, const unsigned char *pColor, const RECT &RoiRect, int StepGry, unsigned char *pGray, IMAGE_SRC_MODE Mode, int WR, int WG, int WB, bool Reverse);//彩色轉灰階

	bool                       RGBImageToGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE StepClr, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const RECT &RoiRect, int StepGry, unsigned char *&pGray, IMAGE_SRC_MODE Mode, int WR, int WG, int WB, bool Reverse);//彩色轉灰階
	bool                       RGBImageToGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE StepClr, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const RECT &RoiRect, int StepGry, unsigned char *pGray, IMAGE_SRC_MODE Mode, int WR, int WG, int WB, bool Reverse);//彩色轉灰階

	bool                       RGBImageToColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE StepGry, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, int StepClr, unsigned char *&pColor, bool Reverse);//RGB轉彩色
	bool                       RGBImageToColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE StepGry, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, int StepClr, unsigned char *pColor, bool Reverse);//RGB轉彩色	

	bool                       ColorImageToRGBImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE StepClr, const unsigned char *pColor, unsigned char *&pR, unsigned char *&pG, unsigned char *&pB, int StepGry, bool Reverse);//彩色轉RGB
	bool                       ColorImageToRGBImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE StepClr, const unsigned char *pColor, unsigned char *pR, unsigned char *pG, unsigned char *pB, int StepGry, bool Reverse);//彩色轉RGB
	//---------------------------------------------------------------------------------//	
	//unsigned char影像和unsigned short影像轉換
	bool                       GrayImageConvertToPhase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int StepShort, PHASE_PTR &PhasePtr, bool Reverse);//灰階影像轉unsigned short
	bool                       GrayImageConvertToPhase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int StepShort, PHASE_PTR PhasePtr, bool Reverse);//灰階影像轉unsigned short

	bool                       ColorImageConvertToPhase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int StepShort, PHASE_PTR &PhasePtr, bool Reverse);//彩色影像轉unsigned short
	bool                       ColorImageConvertToPhase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int StepShort, PHASE_PTR PhasePtr, bool Reverse);//彩色影像轉unsigned short

	bool                       RGBImageConvertToPhase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, int StepShort, PHASE_PTR &pSR, PHASE_PTR &pSG, PHASE_PTR &pSB, bool Reverse);//RGB影像轉unsigned short
	bool                       RGBImageConvertToPhase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, int StepShort, PHASE_PTR pSR, PHASE_PTR pSG, PHASE_PTR pSB, bool Reverse);//RGB影像轉unsigned short

	bool                       PhaseGrayImageConvertToGray(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR pPhase, int ImageStep, unsigned char *&pImage, bool Reverse);//unsigned short影像轉灰階
	bool                       PhaseGrayImageConvertToGray3(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR pPhase, int ImageStep, unsigned char *pImage, bool Reverse);//unsigned short影像轉灰階

	bool                       PhaseGrayImageConvertToColor(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR pPhase, const MASK_PTR pMask, int ImageStep, unsigned char *&pImage, bool Reverse);//unsigned short影像轉彩色階
	bool                       PhaseGrayImageConvertToColor3(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR pPhase, const MASK_PTR pMask, int ImageStep, unsigned char *pImage, bool Reverse);//unsigned short影像轉彩色階

	bool                       PhaseColorImageConvertToColor(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR pImage, int StepChar, unsigned char *&pChar, bool Reverse);//unsigned short影像轉彩色
	bool                       PhaseColorImageConvertToColor3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR pImage, int StepChar, unsigned char *pChar, bool Reverse);//unsigned short影像轉彩色

	bool                       PhaseRGBImageConvertToRGB(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR pR, const PHASE_PTR pG, const PHASE_PTR pB, int StepChar, unsigned char *&pCR, unsigned char *&pCG, unsigned char *&pCB, bool Reverse);//unsigned short影像轉RGB
	bool                       PhaseRGBImageConvertToRGB3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR pR, const PHASE_PTR pG, const PHASE_PTR pB, int StepChar, unsigned char *pCR, unsigned char *pCG, unsigned char *pCB, bool Reverse);//unsigned short影像轉RGB

	bool                       SpaceGrayImageConvertToGray(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR pSpace, unsigned char *&pImage);//float影像轉灰階
	bool                       SpaceGrayImageConvertToGray3(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR pSpace, unsigned char *pImage);//float影像轉灰階

	bool                       SpaceGrayImageConvertToGray(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR pSpace, const MASK_PTR pMask, const RECT &RoiRect, int ImageStep, unsigned char *&pImage, double Ratio, bool Reverse);//unsigned short影像轉灰階
	bool                       SpaceGrayImageConvertToGray3(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR pSpace, const MASK_PTR pMask, const RECT &RoiRect, int ImageStep, unsigned char *pImage, double Ratio, bool Reverse);//unsigned short影像轉灰階

	bool                       SpaceGrayImageConvertToColor(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR pSpace, const MASK_PTR pMask, int ImageStep, unsigned char *&pImage, double Ratio, bool Reverse);//unsigned short影像轉彩色
	bool                       SpaceGrayImageConvertToColor3(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR pSpace, const MASK_PTR pMask, int ImageStep, unsigned char *pImage, double Ratio, bool Reverse);//unsigned short影像轉彩色

	bool                       SpaceGrayImageConvertToShort(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR SpacePtr, short *&pImage, bool Reverse);//空間資料轉成Short影像
	bool                       SpaceGrayImageConvertToShort3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR SpacePtr, short *pImage, bool Reverse);//空間資料轉成Short影像	

	bool                       SpaceGrayImageConvertToUShort(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR SpacePtr, unsigned short *&pImage, bool Reverse);//空間資料轉成unsigned Short影像
	bool                       SpaceGrayImageConvertToUShort3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR SpacePtr, unsigned short *pImage, bool Reverse);//空間資料轉成unsigned Short影像

	bool                       ShortGrayImageConvertToSpace(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const short *pImage, SPACE_PTR &SpacePtr, bool Reverse);//Short影像轉成空間資料
	bool                       ShortGrayImageConvertToSpace3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const short *pImage, SPACE_PTR SpacePtr, bool Reverse);//Short影像轉成空間資料	

	bool                       UShortGrayImageConvertToSpace(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, SPACE_PTR &SpacePtr, bool Reverse);//unsigned Short影像轉成空間資料
	bool                       UShortGrayImageConvertToSpace3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, SPACE_PTR SpacePtr, bool Reverse);//unsigned Short影像轉成空間資料	
	//---------------------------------------------------------------------------------//	
	//色彩轉換
	bool                       RGBConvertXYZ(unsigned char R, unsigned char G, unsigned char B, float &X, float &Y, float &Z);//RGB轉成XYZ
	bool                       XYZConvertRGB(float X, float Y, float Z, unsigned char &R, unsigned char &G, unsigned char &B);//XYZ轉成RGB

	bool                       XYZConvertLab(float X, float Y, float Z, float &L, float &a, float &b);
	bool                       LabConvertXYZ(float L, float a, float b, float &X, float &Y, float &Z);
	//---------------------------------------------------------------------------------//	
	//原始圖還原成圖
	bool                       DebayerGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, BAYER_PATTERN_MODE BayerMode, unsigned char *&pDst);//原圖還原成灰階圖
	bool                       DebayerGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, BAYER_PATTERN_MODE BayerMode, unsigned char *pDst);//原圖還原成灰階圖

	bool                       DebayerColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, BAYER_PATTERN_MODE BayerMode, IMAGE_SIZE &DstStep, unsigned char *&pDst);//原圖還原成彩色圖
	bool                       DebayerColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, BAYER_PATTERN_MODE BayerMode, IMAGE_SIZE DstStep, unsigned char *pDst);//原圖還原成彩色圖	

	bool                       DebayerRawColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, BAYER_PATTERN_MODE BayerMode, IMAGE_SIZE &DstStep, unsigned char *&pDst);//原圖變成彩色圖
	bool                       DebayerRawColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, BAYER_PATTERN_MODE BayerMode, IMAGE_SIZE DstStep, unsigned char *pDst);//原圖變成彩色圖
	//---------------------------------------------------------------------------------//	
	//彩色過濾
	bool                       RGBVConvertToRGB(int IR, int IG, int IB, int IV, unsigned char &R, unsigned char &G, unsigned char &B);//彩色影像轉RGBV色域

	bool                       RGBConvertToRGBV(unsigned char R, unsigned char G, unsigned char B, int &IR, int &IG, int &IB, int &IV);//彩色影像轉RGBV色域
	bool                       RGBConvertToRGBV(unsigned char R, unsigned char G, unsigned char B, unsigned char &IR, unsigned char &IG, unsigned char &IB, unsigned char &IV);//彩色影像轉RGBV色域

	bool                       CalcColorImageColorFilter(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, CColorRGBV &rgbv);//計算彩色影像抽色
	bool                       CalcRGBImageColorFilter(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const RECT &RoiRect, CColorRGBV &rgbv);//計算RGB影像抽色

	bool                       ColorImageConvertToRGBV(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE DstStep, unsigned char *&pDstR, unsigned char *&pDstG, unsigned char *&pDstB, unsigned char *&pDstV, bool FullSize);//彩色影像轉RGBV色域
	bool                       ColorImageConvertToRGBV3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE DstStep, unsigned char *pDstR, unsigned char *pDstG, unsigned char *pDstB, unsigned char *pDstV, bool FullSize);//彩色影像轉RGBV色域

	bool                       RGBImageConvertToRGBV(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const RECT &RoiRect, IMAGE_SIZE DstStep, unsigned char *&pDstR, unsigned char *&pDstG, unsigned char *&pDstB, unsigned char *&pDstV, bool FullSize);//彩色影像轉RGBV色域
	bool                       RGBImageConvertToRGBV3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const RECT &RoiRect, IMAGE_SIZE DstStep, unsigned char *pDstR, unsigned char *pDstG, unsigned char *pDstB, unsigned char *pDstV, bool FullSize);//彩色影像轉RGBV色域

	bool                       ColorImageColorFilter(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const CColorGroup &ColorGoup, const RECT &RoiRect, IMAGE_SIZE MaskStep, unsigned char *&MaskPtr, bool FullMask, bool bOpenMP);//彩色影像抽色
	bool                       ColorImageColorFilter3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const CColorGroup &ColorGoup, const RECT &RoiRect, IMAGE_SIZE MaskStep, unsigned char *MaskPtr, bool FullMask, bool bOpenMP);//彩色影像抽色
	bool                       ColorImageColorFilter3_SP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const CColorGroup &ColorGoup, const RECT &RoiRect, IMAGE_SIZE MaskStep, unsigned char *MaskPtr, bool FullMask);//彩色影像抽色
	bool                       ColorImageColorFilter3_MP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const CColorGroup &ColorGoup, const RECT &RoiRect, IMAGE_SIZE MaskStep, unsigned char *MaskPtr, bool FullMask);//彩色影像抽色-OpenMP
	bool                       ColorImageColorFilter3_MP_Fn(int nX, int nY, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const CColorGroup &ColorGoup, const RECT &RoiRect, IMAGE_SIZE MaskStep, unsigned char *MaskPtr, bool FullMask);//彩色影像抽色-OpenMP

	bool                       RGBImageColorFilter(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const CColorGroup &ColorGoup, const RECT &RoiRect, IMAGE_SIZE MaskStep, unsigned char *&MaskPtr, bool FullMask);//彩色影像抽色
	bool                       RGBImageColorFilter3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const CColorGroup &ColorGoup, const RECT &RoiRect, IMAGE_SIZE MaskStep, unsigned char *MaskPtr, bool FullMask);//彩色影像抽色
	//---------------------------------------------------------------------------------//	
	bool                       FillBarcodeImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *&pDst);//填滿條碼影像	
	bool                       FillBarcodeImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *pDst);//填滿條碼影像	
	//---------------------------------------------------------------------------------//	
	bool                       FillBarcode2DImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *&pDst);//填滿條碼2D影像	
	bool                       FillBarcode2DImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *pDst);//填滿條碼2D影像	
	//---------------------------------------------------------------------------------//	
	bool                       FillImageRoi(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, unsigned char *pImage, const RECT &RoiRect, unsigned char Red, unsigned char Grn=255, unsigned char Blu=255);//填滿區域影像
	bool                       FillGrayImageRoi(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, const RECT &RoiRect, unsigned char Gray);//填滿區域影像
	bool                       FillColorImageRoi(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, const RECT &RoiRect, unsigned char Red, unsigned char Grn, unsigned char Blu);//填滿區域影像
	//---------------------------------------------------------------------------------//		
	bool                       ExtractUniRoiImage(const TUNI_FRAME &UniFrame, const RECT &RoiRect, int nAlign, const char *fnName, TUNI_FRAME &RoiFrame);
	//---------------------------------------------------------------------------------//	
	//挖取局部圖像
	bool                       ExtractRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *&pRoi, bool Reverse);//切割子影像
	bool                       ExtractRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *pRoi, bool Reverse);//切割子影像	
	bool                       ExtractGrayRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *&pRoi, bool Reverse);//灰階切割子影像
	bool                       ExtractGrayRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *pRoi, bool Reverse);//灰階切割子影像
	bool                       ExtractColorRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *&pRoi, bool Reverse);//彩色切割子影像
	bool                       ExtractColorRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *pRoi, bool Reverse);//彩色切割子影像

	bool                       ExtractRGBRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *&pRoiClr, bool Reverse);//RGB切割子影像
	bool                       ExtractRGBRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *pRoiClr, bool Reverse);//RGB切割子影像
	bool                       ExtractRGBRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *&pRoiR, unsigned char *&pRoiG, unsigned char *&pRoiB, bool Reverse);//RGB切割子影像
	bool                       ExtractRGBRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *pRoiR, unsigned char *pRoiG, unsigned char *pRoiB, bool Reverse);//RGB切割子影像

	bool                       ExtractPhaseRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const PHASE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, PHASE_PTR &pRoi, bool Reverse);//切割子影像
	bool                       ExtractPhaseRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const PHASE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, PHASE_PTR pRoi, bool Reverse);//切割子影像
	bool                       ExtractPhaseGrayRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, PHASE_PTR &pRoi, bool Reverse);//灰階切割子影像
	bool                       ExtractPhaseGrayRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, PHASE_PTR pRoi, bool Reverse);//灰階切割子影像
	bool                       ExtractPhaseColorRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, PHASE_PTR &pRoi, bool Reverse);//彩色切割子影像
	bool                       ExtractPhaseColorRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, PHASE_PTR pRoi, bool Reverse);//彩色切割子影像

	bool                       ExtractSpaceRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const SPACE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, SPACE_PTR &pRoi, bool Reverse);//切割子影像
	bool                       ExtractSpaceRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const SPACE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, SPACE_PTR pRoi, bool Reverse);//切割子影像
	bool                       ExtractSpaceGrayRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, SPACE_PTR &pRoi, bool Reverse);//灰階切割子影像
	bool                       ExtractSpaceGrayRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, SPACE_PTR pRoi, bool Reverse);//灰階切割子影像
	bool                       ExtractSpaceColorRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, SPACE_PTR &pRoi, bool Reverse);//彩色切割子影像
	bool                       ExtractSpaceColorRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, SPACE_PTR pRoi, bool Reverse);//彩色切割子影像

	//挖取局部圖像-Sub Pixel
	bool                       ExtractRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, const TRECT4D &RoiRect, IMAGE_SIZE RoiStep, unsigned char *&pRoi, bool Reverse);//切割子影像
	bool                       ExtractRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, const TRECT4D &RoiRect, IMAGE_SIZE RoiStep, unsigned char *pRoi, bool Reverse);//切割子影像	
	bool                       ExtractGrayRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const TRECT4D &RoiRect, IMAGE_SIZE RoiStep, unsigned char *&pRoi, bool Reverse);//灰階切割子影像
	bool                       ExtractGrayRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const TRECT4D &RoiRect, IMAGE_SIZE RoiStep, unsigned char *pRoi, bool Reverse);//灰階切割子影像
	bool                       ExtractColorRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const TRECT4D &RoiRect, IMAGE_SIZE RoiStep, unsigned char *&pRoi, bool Reverse);//彩色切割子影像
	bool                       ExtractColorRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const TRECT4D &RoiRect, IMAGE_SIZE RoiStep, unsigned char *pRoi, bool Reverse);//彩色切割子影像

	bool                       ExtractRGBRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const TRECT4D &RoiRect, IMAGE_SIZE RoiStep, unsigned char *&pRoiClr, bool Reverse);//RGB切割子影像
	bool                       ExtractRGBRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const TRECT4D &RoiRect, IMAGE_SIZE RoiStep, unsigned char *pRoiClr, bool Reverse);//RGB切割子影像
	bool                       ExtractRGBRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const TRECT4D &RoiRect, IMAGE_SIZE RoiStep, unsigned char *&pRoiR, unsigned char *&pRoiG, unsigned char *&pRoiB, bool Reverse);//RGB切割子影像
	bool                       ExtractRGBRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const TRECT4D &RoiRect, IMAGE_SIZE RoiStep, unsigned char *pRoiR, unsigned char *pRoiG, unsigned char *pRoiB, bool Reverse);//RGB切割子影像

	//挖取動態局部圖像
	bool                       ExtractDynamicRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *&pRoi);//動態切割子影像
	bool                       ExtractDynamicRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *pRoi);//動態切割子影像	
	bool                       ExtractDynamicGrayRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *&pRoi);//動態灰階切割子影像
	bool                       ExtractDynamicGrayRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *pRoi);//動態灰階切割子影像
	bool                       ExtractDynamicColorRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *&pRoi);//動態彩色切割子影像
	bool                       ExtractDynamicColorRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *pRoi);//動態彩色切割子影像

	bool                       ExtractDynamicMaskRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *&pRoi, unsigned char DefMask);//動態灰階切割子遮罩
	bool                       ExtractDynamicMaskRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *pRoi, unsigned char DefMask);//動態灰階切割子遮罩

	bool                       ExtractDynamicSpaceRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const SPACE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, SPACE_PTR &pRoi);//動態切割子影像
	bool                       ExtractDynamicSpaceRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const SPACE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, SPACE_PTR pRoi);//動態切割子影像
	bool                       ExtractDynamicSpaceGrayRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, SPACE_PTR &pRoi);//動態灰階切割子影像
	bool                       ExtractDynamicSpaceGrayRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, SPACE_PTR pRoi);//動態灰階切割子影像
	bool                       ExtractDynamicSpaceColorRoiImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, SPACE_PTR &pRoi);//動態彩色切割子影像
	bool                       ExtractDynamicSpaceColorRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, SPACE_PTR pRoi);//動態彩色切割子影像
	//---------------------------------------------------------------------------------//	
	//圖像反向 y = -y
	bool                       ReverseImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned char *&pDst);//反向影像
	bool                       ReverseImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned char *pDst);//反向影像
	bool                       ReverseGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *&pDst);//反向灰階影像
	bool                       ReverseGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *pDst);//反向灰階影像
	bool                       ReverseColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *&pDst);//反向彩色影像
	bool                       ReverseColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *pDst);//反向彩色影像
	bool                       ReversePhaseImage(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR pSrc, PHASE_PTR &pDst);//反向相位資料
	bool                       ReversePhaseImage3(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR pSrc, PHASE_PTR pDst);//反向相位資料
	bool                       ReverseSpaceImage(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR pSrc, SPACE_PTR &pDst);//反向空間資料
	bool                       ReverseSpaceImage3(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR pSrc, SPACE_PTR pDst);//反向空間資料
	//---------------------------------------------------------------------------------//	
	//圖像移動
	bool                       MoveImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int sX, int sY, unsigned char *&DstPtr);//移動影像
	bool                       MoveImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int sX, int sY, unsigned char *DstPtr);//移動影像
	bool                       MoveGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int sX, int sY, unsigned char *&DstPtr);//移動灰階影像
	bool                       MoveGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int sX, int sY, unsigned char *DstPtr);//移動灰階影像
	bool                       MoveColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int sX, int sY, unsigned char *&DstPtr);//移動彩色影像
	bool                       MoveColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int sX, int sY, unsigned char *DstPtr);//移動彩色影像
	//---------------------------------------------------------------------------------//	
	bool                       RotateUniImage(double Angle, const TUNI_FRAME &UniFrame, int nAlign, const char *fnName, TUNI_FRAME &DstFrame);//影像旋轉
	bool                       RotateUniImageList(double Angle, const std::vector<TUNI_FRAME> &UniFrameList, int nAlign, const char *fnName, std::vector<TUNI_FRAME> &DstFrameList);//影像旋轉
	//---------------------------------------------------------------------------------//	
	//圖像旋轉
	IMAGE_SIZE                 CalcRotateImageSize(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH);//計算旋轉影像的尺寸
	bool                       CalcRotateImageSize(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH);//計算旋轉影像的尺寸
	bool                       RotateImageFile(double Angle, LPCTSTR SrcFile, LPCTSTR DstFile);//旋轉影像檔案
	bool                       RotateImage(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//影像旋轉
	bool                       RotateImage3(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//影像旋轉
	bool                       RotateImage3_090(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//影像旋轉-090
	bool                       RotateImage3_180(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//影像旋轉-180
	bool                       RotateImage3_270(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//影像旋轉-270
	bool                       RotateImage3_Any(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//影像旋轉

	bool                       RotateGrayImage(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//灰階影像旋轉	
	bool                       RotateGrayImage3(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//灰階影像旋轉
	bool                       RotateGrayImage_000(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//灰階影像旋轉
	bool                       RotateGrayImage3_000(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//灰階影像旋轉
	bool                       RotateGrayImage_090(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//灰階影像旋轉
	bool                       RotateGrayImage3_090(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//灰階影像旋轉
	bool                       RotateGrayImage_180(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//灰階影像旋轉
	bool                       RotateGrayImage3_180(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//灰階影像旋轉
	bool                       RotateGrayImage_270(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//灰階影像旋轉
	bool                       RotateGrayImage3_270(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//灰階影像旋轉
	bool                       RotateGrayImage_Any(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//灰階影像旋轉
	bool                       RotateGrayImage3_Any(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//灰階影像旋轉

	bool                       RotateColorImage(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//彩色影像旋轉
	bool                       RotateColorImage3(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//彩色影像旋轉
	bool                       RotateColorImage_000(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//彩色影像旋轉
	bool                       RotateColorImage3_000(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//彩色影像旋轉
	bool                       RotateColorImage_090(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//彩色影像旋轉
	bool                       RotateColorImage3_090(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//彩色影像旋轉
	bool                       RotateColorImage_180(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//彩色影像旋轉
	bool                       RotateColorImage3_180(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//彩色影像旋轉
	bool                       RotateColorImage_270(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//彩色影像旋轉
	bool                       RotateColorImage3_270(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//彩色影像旋轉
	bool                       RotateColorImage_Any(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//彩色影像旋轉
	bool                       RotateColorImage3_Any(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//彩色影像旋轉

	bool                       RotateRGBImage(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDstR, unsigned char *&pDstG, unsigned char *&pDstB);//RGB影像旋轉
	bool                       RotateRGBImage3(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDstR, unsigned char *pDstG, unsigned char *pDstB);//RGB影像旋轉
	bool                       RotateRGBImage_000(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDstR, unsigned char *&pDstG, unsigned char *&pDstB);//RGB影像旋轉
	bool                       RotateRGBImage3_000(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDstR, unsigned char *pDstG, unsigned char *pDstB);//RGB影像旋轉
	bool                       RotateRGBImage_090(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDstR, unsigned char *&pDstG, unsigned char *&pDstB);//RGB影像旋轉
	bool                       RotateRGBImage3_090(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDstR, unsigned char *pDstG, unsigned char *pDstB);//RGB影像旋轉
	bool                       RotateRGBImage_180(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDstR, unsigned char *&pDstG, unsigned char *&pDstB);//RGB影像旋轉
	bool                       RotateRGBImage3_180(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDstR, unsigned char *pDstG, unsigned char *pDstB);//RGB影像旋轉
	bool                       RotateRGBImage_270(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDstR, unsigned char *&pDstG, unsigned char *&pDstB);//RGB影像旋轉
	bool                       RotateRGBImage3_270(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDstR, unsigned char *pDstG, unsigned char *pDstB);//RGB影像旋轉
	bool                       RotateRGBImage_Any(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDstR, unsigned char *&pDstG, unsigned char *&pDstB);//RGB影像旋轉
	bool                       RotateRGBImage3_Any(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDstR, unsigned char *pDstG, unsigned char *pDstB);//RGB影像旋轉

	bool                       RotateSpace(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, float *&pDst);//空間高度旋轉
	bool                       RotateSpace3(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, float *pDst);//空間高度旋轉
	bool                       RotateSpace_000(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, float *&pDst);//空間高度旋轉
	bool                       RotateSpace3_000(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, float *pDst);//空間高度旋轉
	bool                       RotateSpace_090(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, float *&pDst);//空間高度旋轉
	bool                       RotateSpace3_090(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, float *pDst);//空間高度旋轉
	bool                       RotateSpace_180(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, float *&pDst);//空間高度旋轉
	bool                       RotateSpace3_180(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, float *pDst);//空間高度旋轉
	bool                       RotateSpace_270(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, float *&pDst);//空間高度旋轉
	bool                       RotateSpace3_270(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, float *pDst);//空間高度旋轉
	bool                       RotateSpace_Any(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, float *&pDst);//空間高度旋轉
	bool                       RotateSpace3_Any(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, float *pDst);//空間高度旋轉
	bool                       RotateSpace3_OpenCV(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, float *pDst);//空間高度旋轉

	bool                       RotateMask(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pMask, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//遮罩影像旋轉
	bool                       RotateMask3(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pMask, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//遮罩影像旋轉
	bool                       RotateMask_Any(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pMask, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//遮罩影像旋轉
	bool                       RotateMask3_Any(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pMask, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//遮罩影像旋轉
	bool                       RotateMask3_OpenCV(double Angle, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pMask, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//遮罩影像旋轉
	//---------------------------------------------------------------------------------//	
	bool                       RotateImage(double Angle, double PivotX, double PivotY, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *&pDst);//影像旋轉
	bool                       RotateImage3(double Angle, double PivotX, double PivotY, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//影像旋轉
	bool                       RotateGrayImage(double Angle, double PivotX, double PivotY, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *&pDst);//影像旋轉
	bool                       RotateGrayImage3(double Angle, double PivotX, double PivotY, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//影像旋轉
	bool                       RotateColorImage(double Angle, double PivotX, double PivotY, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *&pDst);//影像旋轉
	bool                       RotateColorImage3(double Angle, double PivotX, double PivotY, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//影像旋轉
	//---------------------------------------------------------------------------------//	
	//繪製圖像邊框
	bool                       DrawImageBoundary3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int Size, unsigned char Red, unsigned char Grn, unsigned char Blu, unsigned char *pDst);//繪製圖像邊框
	bool                       DrawGrayImageBoundary3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int Size, unsigned char Gry, unsigned char *pDst);//繪製灰階圖像邊框
	bool                       DrawColorImageBoundary3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int Size, unsigned char Red, unsigned char Grn, unsigned char Blu, unsigned char *pDst);//繪製彩色圖像邊框
	//---------------------------------------------------------------------------------//	
	//高度合併
	bool                       CombineHorSpaceImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pImageFixed, const float *pImage, int Offset, float *&pDst);//兩張圖左右合併
	bool                       CombineHorSpaceImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pImageFixed, const float *pImage, int Offset, float *pDst);//兩張圖左右合併
	bool                       CombineVerSpaceImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pImageFixed, const float *pImage, int Offset, float *&pDst);//兩張圖上下合併
	bool                       CombineVerSpaceImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pImageFixed, const float *pImage, int Offset, float *pDst);//兩張圖上下合併
	//---------------------------------------------------------------------------------//	
	//圖像合併
	bool                       CombineHorGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImageFixed, const unsigned char *pImage, int Offset, unsigned char *&pDst);//兩張圖左右合併
	bool                       CombineHorGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImageFixed, const unsigned char *pImage, int Offset, unsigned char *pDst);//兩張圖左右合併
	bool                       CombineVerGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImageFixed, const unsigned char *pImage, int Offset, unsigned char *&pDst);//兩張圖上下合併
	bool                       CombineVerGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImageFixed, const unsigned char *pImage, int Offset, unsigned char *pDst);//兩張圖上下合併
	//---------------------------------------------------------------------------------//	
	//圖像回貼
	bool                       PasteRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, const unsigned char *pRoi, bool Reverse, bool theSameSize);//回貼子影像
	bool                       PasteGrayRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, const unsigned char *pRoi, bool Reverse, bool theSameSize);//灰階回貼子影像
	bool                       PasteColorRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, const unsigned char *pRoi, bool Reverse, bool theSameSize);//彩色回貼子影像
	bool                       PasteColorRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, const RECT &RoiRect, IMAGE_SIZE RoiStep, const unsigned char *pRoiR, const unsigned char *pRoiG, const unsigned char *pRoiB, bool Reverse, bool theSameSize);//彩色回貼子影像
	bool                       PasteRGBRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pR, unsigned char *pG, unsigned char *pB, const RECT &RoiRect, IMAGE_SIZE RoiStep, const unsigned char *pRoiClr, bool Reverse, bool theSameSize);//RGB回貼子影像
	bool                       PasteRGBRoiImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pR, unsigned char *pG, unsigned char *pB, const RECT &RoiRect, IMAGE_SIZE RoiStep, const unsigned char *pRoiR, const unsigned char *pRoiG, const unsigned char *pRoiB, bool Reverse, bool theSameSize);//RGB回貼子影像
	//---------------------------------------------------------------------------------//	
	bool                       AlignImageBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int NewStep, unsigned char *&pDest, bool Reverse);//影像記憶體重新排列
	bool                       AlignImageBuffer3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int NewStep, unsigned char *pDest, bool Reverse);//影像記憶體重新排列

	bool                       AlignGrayImageBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int NewStep, unsigned char *&pDest, bool Reverse);//灰階影像記憶體重新排列
	bool                       AlignGrayImageBuffer3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int NewStep, unsigned char *pDest, bool Reverse);//灰階影像記憶體重新排列
	bool                       AlignGrayImageBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, int NewStep, unsigned char *&pDest, bool Reverse);//灰階影像記憶體重新排列
	bool                       AlignGrayImageBuffer3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, int NewStep, unsigned char *pDest, bool Reverse);//灰階影像記憶體重新排列

	bool                       AlignColorImageBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int NewStep, unsigned char *&pDest, bool Reverse);//彩色影像記憶體重新排列
	bool                       AlignColorImageBuffer3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int NewStep, unsigned char *pDest, bool Reverse);//彩色影像記憶體重新排列

	bool                       AlignSpaceImageBuffer(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, int NewStep, float *&pDest, bool Reverse);//空間影像記憶體重新排列
	bool                       AlignSpaceImageBuffer3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, int NewStep, float *pDest, bool Reverse);//空間影像記憶體重新排列
	//---------------------------------------------------------------------------------//	
	//影像平移與增益
	bool                       ImageOffsetGain(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pSrc, unsigned char *&pDst, double offset=0.0, double gain=1.0);//灰階影像增益與平移
	bool                       ImageOffsetGain3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pSrc, unsigned char *pDst, double offset=0.0, double gain=1.0);//灰階影像增益與平移
	bool                       GrayImageOffsetGain(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pSrc, unsigned char *&pDst, double offset=0.0, double gain=1.0);//灰階影像增益與平移
	bool                       GrayImageOffsetGain3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pSrc, unsigned char *pDst, double offset=0.0, double gain=1.0);//灰階影像增益與平移
	bool                       ColorImageOffsetGain(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pSrc, unsigned char *&pDst, double offset=0.0, double gain=1.0);//彩色影像增益與平移
	bool                       ColorImageOffsetGain3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pSrc, unsigned char *pDst, double offset=0.0, double gain=1.0);//彩色影像增益與平移
	bool                       RGBImageOffsetGain(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, unsigned char *&pDestR, unsigned char *&pDestG, unsigned char *&pDestB, double offset=0.0, double gain=1.0);//灰階影像增益與平移
	bool                       RGBImageOffsetGain3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, unsigned char *pDestR, unsigned char *pDestG, unsigned char *pDestB, double offset=0.0, double gain=1.0);//灰階影像增益與平移
	//---------------------------------------------------------------------------------//
	//局部影像平移與增益
	bool                       ImageOffsetGain(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pSrc, const RECT &RoiRect, unsigned char *&pDst, double offset=0.0, double gain=1.0);//灰階影像增益與平移
	bool                       ImageOffsetGain3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pSrc, const RECT &RoiRect, unsigned char *pDst, double offset=0.0, double gain=1.0);//灰階影像增益與平移
	bool                       GrayImageOffsetGain(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pSrc, const RECT &RoiRect, unsigned char *&pDst, double offset=0.0, double gain=1.0);//灰階影像增益與平移
	bool                       GrayImageOffsetGain3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pSrc, const RECT &RoiRect, unsigned char *pDst, double offset=0.0, double gain=1.0);//灰階影像增益與平移
	bool                       ColorImageOffsetGain(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pSrc, const RECT &RoiRect, unsigned char *&pDst, double offset=0.0, double gain=1.0);//彩色影像增益與平移
	bool                       ColorImageOffsetGain3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pSrc, const RECT &RoiRect, unsigned char *pDst, double offset=0.0, double gain=1.0);//彩色影像增益與平移
	bool                       RGBImageOffsetGain(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const RECT &RoiRect, unsigned char *&pDestR, unsigned char *&pDestG, unsigned char *&pDestB, double offset=0.0, double gain=1.0);//灰階影像增益與平移
	bool                       RGBImageOffsetGain3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const RECT &RoiRect, unsigned char *pDestR, unsigned char *pDestG, unsigned char *pDestB, double offset=0.0, double gain=1.0);//灰階影像增益與平移
	//---------------------------------------------------------------------------------//
	//飽和度調整
	bool                       SaturateImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pSrc, unsigned char *&pDst, double Wr=1.0, double Wb=1.0);//影像飽和調整
	bool                       SaturateImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pSrc, unsigned char *pDst, double Wr=1.0, double Wb=1.0);//影像飽和調整
	bool                       SaturateGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pSrc, unsigned char *&pDst, double Wr=1.0, double Wb=1.0);//灰階影像飽和調整-無意義
	bool                       SaturateGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pSrc, unsigned char *pDst, double Wr=1.0, double Wb=1.0);//灰階影像飽和調整-無意義
	bool                       SaturateColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pSrc, unsigned char *&pDst, double Wr=1.0, double Wb=1.0);//彩色影像飽和調整
	bool                       SaturateColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pSrc, unsigned char *pDst, double Wr=1.0, double Wb=1.0);//彩色影像飽和調整
	bool                       SaturateRGBImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, unsigned char *&pDstR, unsigned char *&pDstG, unsigned char *&pDstB, double Wr=1.0, double Wb=1.0);//三色影像飽和調整
	bool                       SaturateRGBImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, unsigned char *pDstR, unsigned char *pDstG, unsigned char *pDstB, double Wr=1.0, double Wb=1.0);//三色影像飽和調整
	//---------------------------------------------------------------------------------//
	//設定數值	
	bool                       SetGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, const RECT &RoiRect, unsigned char val);//灰階影像設定
	//---------------------------------------------------------------------------------//
	//LUT
	bool                       ImageApplyLUT(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned char LUT[], unsigned char *&pDst);//灰階影像套用LUT
	bool                       ImageApplyLUT3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned char LUT[], unsigned char *pDst);//灰階影像套用LUT
	bool                       GrayImageApplyLUT(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char LUT[], unsigned char *&pDst);//灰階影像套用LUT
	bool                       GrayImageApplyLUT3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char LUT[], unsigned char *pDst);//灰階影像套用LUT
	bool                       ColorImageApplyLUT(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char LUT[], unsigned char *&pDst);//彩色影像套用LUT
	bool                       ColorImageApplyLUT3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char LUT[], unsigned char *pDst);//彩色影像套用LUT
	bool                       RGBImageApplyLUT(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, unsigned char LUT[], unsigned char *&pDestR, unsigned char *&pDestG, unsigned char *&pDestB);//RGB影像套用LUT
	bool                       RGBImageApplyLUT3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, unsigned char LUT[], unsigned char *pDestR, unsigned char *pDestG, unsigned char *pDestB);//RGB影像套用LUT
	//---------------------------------------------------------------------------------//
	//Gamma增益	
	bool                       BuildGammaLookUpTable(double Gamma, unsigned char GammaTable[]);//建立Gamma的LUT
	bool                       GammaGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, double Gamma, unsigned char *&pDest);//灰階影像Gamma增益
	bool                       GammaGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, double Gamma, unsigned char *pDest);//灰階影像Gamma增益
	bool                       GammaColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, double Gamma, unsigned char *&pDest);//彩色影像Gamma增益
	bool                       GammaColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, double Gamma, unsigned char *pDest);//彩色影像Gamma增益
	bool                       GammaRGBImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, double Gamma, unsigned char *&pDestR, unsigned char *&pDestG, unsigned char *&pDestB);//RGB影像Gamma增益
	bool                       GammaRGBImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, double Gamma, unsigned char *pDestR, unsigned char *pDestG, unsigned char *pDestB);//RGB影像Gamma增益		
	//-----------------------------------------------------------------------------//
	//Local Gamma增益
	bool                       LocalGammaImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int CalcSize, double Gamma, unsigned char *&pDest);//影像Local-Gamma增益
	bool                       LocalGammaImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int CalcSize, double Gamma, unsigned char *pDest);//影像Local-Gamma增益
	bool                       LocalGammaGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int CalcSize, double Gamma, unsigned char *&pDest);//灰階影像Local-Gamma增益
	bool                       LocalGammaGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int CalcSize, double Gamma, unsigned char *pDest);//灰階影像Local-Gamma增益
	bool                       LocalGammaColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int CalcSize, double Gamma, unsigned char *&pDest);//彩色影像Local-Gamma增益
	bool                       LocalGammaColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int CalcSize, double Gamma, unsigned char *pDest);//彩色影像Local-Gamma增益
	bool                       LocalGammaRGBImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, int CalcSize, double Gamma, unsigned char *&pDstR, unsigned char *&pDstG, unsigned char *&pDstB);//RGB影像Local-Gamma增益
	bool                       LocalGammaRGBImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, int CalcSize, double Gamma, unsigned char *pDstR, unsigned char *&pDstG, unsigned char *&pDstB);//RGB影像Local-Gamma增益
	//-----------------------------------------------------------------------------//	
	COLORREF                   InvertColor(COLORREF clr);//負片顏色
	COLORREF                   InterpolateColorValue(COLORREF clrHigh, COLORREF clrLow, double Ratio);//計算中間顏色	
	//-----------------------------------------------------------------------------//
	//影像負片處理
	bool                       InvertImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned char *&pDst);//影像反相
	bool                       InvertImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned char *pDst);//影像反相
	bool                       InvertGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *&pDst);//灰階影像反相
	bool                       InvertGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *pDst);//灰階影像反相
	bool                       InvertGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, unsigned char *&pDst);//灰階影像反相
	bool                       InvertGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &RoiRect, unsigned char *pDst);//灰階影像反相
	bool                       InvertColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *&pDst);//彩色影像反相
	bool                       InvertColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *pDst);//彩色影像反相
	//-----------------------------------------------------------------------------//
	//影像翻轉(鏡射)處理
	bool                       FlipImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int FlipMode, unsigned char *&pDst);//影像翻轉
	bool                       FlipImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int FlipMode, unsigned char *pDst);//影像翻轉
	bool                       FlipGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int FlipMode, unsigned char *&pDst);//灰階影像翻轉
	bool                       FlipGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int FlipMode, unsigned char *pDst);//灰階影像翻轉
	bool                       FlipColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int FlipMode, unsigned char *&pDst);//彩色影像翻轉
	bool                       FlipColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int FlipMode, unsigned char *pDst);//彩色影像翻轉
	//-----------------------------------------------------------------------------//
	//影像匹配
	bool                       MatchImage(IMAGE_SIZE PatW, IMAGE_SIZE PatH, IMAGE_SIZE PatStep, IMAGE_SIZE PatBit, const unsigned char *PatPtr, IMAGE_SIZE RoiW, IMAGE_SIZE RoiH, IMAGE_SIZE RoiStep, IMAGE_SIZE RoiBit, const unsigned char *RoiPtr, float &Score, float &PosX, float &PosY, float &Skew);
	bool                       MatchGrayImage(IMAGE_SIZE PatW, IMAGE_SIZE PatH, IMAGE_SIZE PatStep, const unsigned char *PatPtr, IMAGE_SIZE RoiW, IMAGE_SIZE RoiH, IMAGE_SIZE RoiStep, const unsigned char *RoiPtr, float &Score, float &PosX, float &PosY, float &Skew);
	bool                       MatchColorImage(IMAGE_SIZE PatW, IMAGE_SIZE PatH, IMAGE_SIZE PatStep, const unsigned char *PatPtr, IMAGE_SIZE RoiW, IMAGE_SIZE RoiH, IMAGE_SIZE RoiStep, const unsigned char *RoiPtr, float &Score, float &PosX, float &PosY, float &Skew);
	//-----------------------------------------------------------------------------//
	//兩張影像比較
	bool                       Compare2Image(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage1, const unsigned char *pImage2, int nGridX, int nGridY, double &Score);
	bool                       Compare2GrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage1, const unsigned char *pImage2, int nGridX, int nGridY, double &Score);
	bool                       Compare2ColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage1, const unsigned char *pImage2, int nGridX, int nGridY, double &Score);
	bool                       Compare2ColorImage_1(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage1, const unsigned char *pImage2, int nGridX, int nGridY, double &Score);
	bool                       Compare2ColorImage_2(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage1, const unsigned char *pImage2, int nGridX, int nGridY, double &Score);

	bool                       Compare2ImageRoi(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage1, const unsigned char *pImage2, int nRoiSizeW, int nRoiSizeH, IMAGE_SIZE &ScoreStep, unsigned char *&ScorePtr);
	bool                       Compare2ImageRoi3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage1, const unsigned char *pImage2, int nRoiSizeW, int nRoiSizeH, IMAGE_SIZE ScoreStep, unsigned char *ScorePtr);
	bool                       Compare2GrayImageRoi(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage1, const unsigned char *pImage2, int nRoiSizeW, int nRoiSizeH, IMAGE_SIZE &ScoreStep, unsigned char *&ScorePtr);
	bool                       Compare2GrayImageRoi3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage1, const unsigned char *pImage2, int nRoiSizeW, int nRoiSizeH, IMAGE_SIZE ScoreStep, unsigned char *ScorePtr);
	bool                       Compare2ColorImageRoi(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage1, const unsigned char *pImage2, int nRoiSizeW, int nRoiSizeH, IMAGE_SIZE &ScoreStep, unsigned char *&ScorePtr);
	bool                       Compare2ColorImageRoi3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage1, const unsigned char *pImage2, int nRoiSizeW, int nRoiSizeH, IMAGE_SIZE ScoreStep, unsigned char *ScorePtr);
	//-----------------------------------------------------------------------------//
	//兩張影像像素比較
	bool                       FilterComparePatMask(IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, const unsigned char *pImage, const TPixelCompareParam &pcParam, unsigned char *pMask);//過濾樣板的遮罩圖
	bool                       Compare2ImagePixel(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pMask, TPixelCompareParam &pcParam, std::vector<RECT> &RectList);	
	bool                       Compare2GrayImagePixel(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pMask, TPixelCompareParam &pcParam, std::vector<RECT> &RectList);	
	bool                       Compare2GrayImagePixel_v1(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pMask, TPixelCompareParam &pcParam, std::vector<RECT> &RectList);	
	bool                       Compare2GrayImagePixel_v2(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pMask, TPixelCompareParam &pcParam, std::vector<RECT> &RectList);	
	bool                       Compare2ColorImagePixel(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pMask, TPixelCompareParam &pcParam, std::vector<RECT> &RectList);	
	bool                       Compare2ColorImagePixel_v1(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pMask, TPixelCompareParam &pcParam, std::vector<RECT> &RectList);	
	bool                       Compare2ColorImagePixel_v2(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pMask, TPixelCompareParam &pcParam, std::vector<RECT> &RectList);	
	bool                       Compare2ImagePixelBlob(IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, const unsigned char *pMask, TPixelCompareParam &pcParam, std::vector<RECT> &RectList);
	//-----------------------------------------------------------------------------//
	//計算影像統計資料
	bool                       CalcGrayImageStatistics(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, TImageStat &Statistics);//計算影像統計資料
	bool                       CalcGrayImageGridStatistics(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, std::vector<TImageStat> &GridList);//計算影像格子統計資料
	bool                       CalcGrayImageGridStatistics(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE NRow, IMAGE_SIZE NCol, std::vector<TImageStat> &GridList);//計算影像格子統計資料

	bool                       CalcShortGrayImageStatistics(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR pImage, TImageStat &Statistics);//計算影像統計資料
	bool                       CalcFloatGrayImageStatistics(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR pImage, TImageStat &Statistics);//計算影像統計資料
	//-----------------------------------------------------------------------------//
	//計算影像對焦數據
	bool                       CalcGrayImageFocusValue_Amplitude(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const IMAGE_PTR pImage, const RECT &RoiRect, double &val);//計算影像對焦數據
	bool                       CalcGrayImageFocusValue_Variance(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const IMAGE_PTR pImage, const RECT &RoiRect, double &val);//計算影像對焦數據
	bool                       CalcGrayImageFocusValue_SumModulesDifference(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const IMAGE_PTR pImage, const RECT &RoiRect, double &val);//計算影像對焦數據
	bool                       CalcGrayImageFocusValue_SquaredGradient(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const IMAGE_PTR pImage, const RECT &RoiRect, double &val);//計算影像對焦數據	
	bool                       CalcGrayImageFocusValue_Tenengrad(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const IMAGE_PTR pImage, const RECT &RoiRect, double &val);//計算影像對焦數據
	bool                       CalcGrayImageFocusValue_Laplacian(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const IMAGE_PTR pImage, const RECT &RoiRect, double &val);//計算影像對焦數據
	bool                       CalcGrayImageFocusValue_PixelDifference(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const IMAGE_PTR pImage, const IMAGE_PTR pDst, const RECT &RoiRect, double &val);//計算影像對焦數據
	//-----------------------------------------------------------------------------//
	//影像縮放	
	bool                       ScaleImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, double Scale, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//縮放影像
	bool                       ScaleImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, double Scale, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//縮放影像

	bool                       CalcScaleSize(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, double Scale, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep);//計算縮放尺寸
	bool                       ScaleGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, double Scale, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//縮放灰階影像
	bool                       ScaleGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, double Scale, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//縮放灰階影像
	bool                       ScaleColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, double Scale, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//縮放彩色影像
	bool                       ScaleColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, double Scale, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//縮放彩色影像
	//-----------------------------------------------------------------------------//
	//遮罩縮放
	bool                       ScaleMask(IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, const unsigned char *MaskPtr, double Scale, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//縮放遮罩
	bool                       ScaleMask3(IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, const unsigned char *MaskPtr, double Scale, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, unsigned char *pDst);//縮放遮罩
	//-----------------------------------------------------------------------------//
	//高度縮放
	bool                       ScaleSpace(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const float *SpacePtr, double Scale, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, float *&pDst);//縮放高度值
	bool                       ScaleSpace3(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const float *SpacePtr, double Scale, IMAGE_SIZE DstW, IMAGE_SIZE DstH, IMAGE_SIZE DstStep, float *pDst);//縮放高度值
	//-----------------------------------------------------------------------------//
	//快速縮小-取最近值     
	bool                       FastScaleImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE NPixels, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//快速縮小影像
	bool                       FastScaleImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE NPixels, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *pDst);//快速縮小影像

	bool                       FastScaleGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE NPixels, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//快速縮小灰階影像
	bool                       FastScaleGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE NPixels, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *pDst);//快速縮小灰階影像
	bool                       FastScaleColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE NPixels, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//快速縮小彩色影像
	bool                       FastScaleColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE NPixels, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *pDst);//快速縮小彩色影像
	//-----------------------------------------------------------------------------//
	//銳利化
	bool                       Sharpness4Image(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int ThL, int Amount, unsigned char *&pDst);//灰階影像銳利化-4鄰近
	bool                       Sharpness4Image3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int ThL, int Amount, unsigned char *pDst);//灰階影像銳利化-4鄰近
	bool                       Sharpness8Image(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int ThL, int Amount, unsigned char *&pDst);//灰階影像銳利化-8鄰近
	bool                       Sharpness8Image3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int ThL, int Amount, unsigned char *pDst);//灰階影像銳利化-8鄰近
	bool                       SharpnessGausImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int Radius, int ThL, int Amount, unsigned char *&pDst);//灰階影像銳利化-高斯比較
	bool                       SharpnessGausImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int Radius, int ThL, int Amount, unsigned char *pDst);//灰階影像銳利化-高斯比較
	bool                       SharpnessLapsImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int ThL, int Amount, unsigned char *&pDst);//灰階影像銳利化-拉普拉斯
	bool                       SharpnessLapsImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int ThL, int Amount, unsigned char *pDst);//灰階影像銳利化-拉普拉斯
	bool                       SharpnessLaps2Image(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned char *&pDst, int KerSize);//灰階影像銳利化-拉普拉斯2
	bool                       SharpnessLaps2Image3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned char *pDst, int KerSize);//灰階影像銳利化-拉普拉斯2

	bool                       Sharpness4GrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int ThL, int Amount, unsigned char *&pDst);//灰階影像銳利化-4鄰近
	bool                       Sharpness4GrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int ThL, int Amount, unsigned char *pDst);//灰階影像銳利化-4鄰近
	bool                       Sharpness8GrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int ThL, int Amount, unsigned char *&pDst);//灰階影像銳利化-8鄰近
	bool                       Sharpness8GrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int ThL, int Amount, unsigned char *pDst);//灰階影像銳利化-8鄰近		
	bool                       SharpnessGausGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int Radius, int ThL, int Amount, unsigned char *&pDst);//灰階影像銳利化-高斯比較
	bool                       SharpnessGausGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int Radius, int ThL, int Amount, unsigned char *pDst);//灰階影像銳利化-高斯比較
	bool                       SharpnessLapsGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int ThL, int Amount, unsigned char *&pDst);//灰階影像銳利化-拉普拉斯
	bool                       SharpnessLapsGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int ThL, int Amount, unsigned char *pDst);//灰階影像銳利化-拉普拉斯
	bool                       SharpnessLaps2GrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *&pDst, int KerSize);//灰階影像銳利化-拉普拉斯2
	bool                       SharpnessLaps2GrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *pDst, int KerSize);//灰階影像銳利化-拉普拉斯2

	bool                       Sharpness4ColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int ThL, int Amount, unsigned char *&pDst);//灰階影像銳利化-4鄰近
	bool                       Sharpness4ColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int ThL, int Amount, unsigned char *pDst);//灰階影像銳利化-4鄰近
	bool                       Sharpness8ColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int ThL, int Amount, unsigned char *&pDst);//灰階影像銳利化-8鄰近
	bool                       Sharpness8ColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int ThL, int Amount, unsigned char *pDst);//灰階影像銳利化-8鄰近
	bool                       SharpnessGausColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int Radius, int ThL, int Amount, unsigned char *&pDst);//灰階影像銳利化-高斯比較
	bool                       SharpnessGausColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int Radius, int ThL, int Amount, unsigned char *pDst);//灰階影像銳利化-高斯比較
	bool                       SharpnessLapsColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int ThL, int Amount, unsigned char *&pDst);//灰階影像銳利化-拉普拉斯
	bool                       SharpnessLapsColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int ThL, int Amount, unsigned char *pDst);//灰階影像銳利化-拉普拉斯
	bool                       SharpnessLaps2ColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *&pDst, int KerSize);//灰階影像銳利化-拉普拉斯2
	bool                       SharpnessLaps2ColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *pDst, int KerSize);//灰階影像銳利化-拉普拉斯2	
	//-----------------------------------------------------------------------------//
	//拉卜拉斯處理
	bool                       Laplacian8Image(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, const RECT &Roi, IMAGE_SIZE DstStep, unsigned char *&pDst);//Laplacian-8
	bool                       Laplacian8Image3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, const RECT &Roi, IMAGE_SIZE DstStep, unsigned char *pDst);//Laplacian-8

	bool                       Laplacian8GrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, IMAGE_SIZE DstStep, unsigned char *&pDst);//Laplacian-8
	bool                       Laplacian8GrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, IMAGE_SIZE DstStep, unsigned char *pDst);//Laplacian-8
	
	bool                       Laplacian8ColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, IMAGE_SIZE DstStep, unsigned char *&pDst);//Laplacian-8
	bool                       Laplacian8ColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, IMAGE_SIZE DstStep, unsigned char *pDst);//Laplacian-8
	//-----------------------------------------------------------------------------//
	//影像灰階統計
	bool                       CalcImageAverage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, const RECT &Roi, double &AveR, double &AveG, double &AveB);//計算影像平均灰階	
	bool                       CalcGrayImageAverage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, double &Ave);//計算灰階影像平均灰階	
	bool                       CalcColorImageAverage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, double &AveR, double &AveG, double &AveB);//計算彩色影像平均灰階
	bool                       CalcColorImageAverage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, IMAGE_SRC_MODE ImageSrc, double WR, double WG, double WB, double &Ave);//計算彩色影像平均灰階
	bool                       CalcRGBImageAverage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const RECT &Roi, double &AveR, double &AveG, double &AveB);//計算彩色影像平均灰階
	bool                       CalcRGBImageAverage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const RECT &Roi, IMAGE_SRC_MODE ImageSrc, int WR, int WG, int WB, double &Ave);//計算彩色影像平均灰階
	bool                       CalcSpaceImageAverage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, const RECT &Roi, double &Ave);//計算高度影像平均灰階	
	bool                       CalcSpaceImageAverage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, const unsigned char *pMask, const RECT &Roi, double &Ave);//計算高度影像平均灰階	
	//-----------------------------------------------------------------------------//
	//相對亮閥值
	bool                       CalcGrayImageRelativeBrightThreshold(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, double ShowRatio, int &Threshold);//計算灰階影像亮的相對閥值
	bool                       CalcColorImageRelativeBrightThreshold(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, IMAGE_SRC_MODE ImageSrc, int WR, int WG, int WB, double ShowRatio, int &Threshold);//計算灰階影像亮的相對閥值
	bool                       CalcRGBImageRelativeBrightThreshold(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const RECT &Roi, IMAGE_SRC_MODE ImageSrc, int WR, int WG, int WB, double ShowRatio, int &Threshold);//計算灰階影像亮的相對閥值
	//-----------------------------------------------------------------------------//
	//相對暗閥值
	bool                       CalcGrayImageRelativeBlackThreshold(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, double ShowRatio, int &Threshold);//計算灰階影像亮的相對閥值
	bool                       CalcColorImageRelativeBlackThreshold(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, IMAGE_SRC_MODE ImageSrc, int WR, int WG, int WB, double ShowRatio, int &Threshold);//計算灰階影像亮的相對閥值
	bool                       CalcRGBImageRelativeBlackThreshold(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const RECT &Roi, IMAGE_SRC_MODE ImageSrc, int WR, int WG, int WB, double ShowRatio, int &Threshold);//計算灰階影像亮的相對閥值
	//-----------------------------------------------------------------------------//
	//OTSU-2值化, (大津演算法)
	bool                       CalcGrayImageOTSUThreshold(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, int &Threshold);//計算灰階的OTSU閥值
	bool                       CalcPhaseImageOTSUThreshold(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr, const MASK_PTR MaskPtr, const RECT &Roi, int &Threshold);//計算灰階的OTSU閥值	
	bool                       CalcSpaceImageOTSUThreshold(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const RECT &Roi, SPACE_DATA &Threshold);//計算灰階的OTSU閥值	

	//Iso-Data-2值化
	bool                       CalcGrayImageIsoDataThreshold(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, int &Threshold);//計算灰階的IsoData閥值
	bool                       CalcPhaseImageIsoDataThreshold(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr, const MASK_PTR MaskPtr, const RECT &Roi, int &Threshold);//計算灰階的IsoData閥值
	bool                       CalcSpaceImageIsoDataThreshold(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const RECT &Roi, SPACE_DATA &Threshold);//計算灰階的IsoData閥值	
	//-----------------------------------------------------------------------------//
	bool                       CalcPointListBlurMinRect(std::vector<POINT> &PtList, const RECT &Rect, int BandSize, double &SizeX, double &SizeY, double &PosX, double &PosY);//平滑最小矩形
	bool                       CalcPointListBlurMaxRect(std::vector<POINT> &PtList, const RECT &Rect, int BandSize, double &SizeX, double &SizeY, double &PosX, double &PosY);//平滑最大矩形
	bool                       CalcPointListBlurMinMaxRect(std::vector<POINT> &PtList, const RECT &Rect, int BandSize, double &MinX, double &MinY, double &MaxX, double &MaxY, double &minPosX, double &minPosY, double &maxPosX, double &maxPosY);//平滑矩形
	//-----------------------------------------------------------------------------//
	bool                       CalcPointListAveSizeByRect(std::vector<POINT> &PtList, const RECT &Rect, double &SizeX, double &SizeY);//整體平均	
	bool                       CalcPointListAveSizeByCircle(std::vector<POINT> &PtList, double CpX, double CpY, double &SizeX, double &SizeY);	
	//-----------------------------------------------------------------------------//
	bool                       CalcGrayImageAveSizeByRect(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, int Threshold, double &SizeX, double &SizeY);	
	//-----------------------------------------------------------------------------//
	bool                       CalcPointListMomentAngle(const std::vector<POINT> &PtList, double &CpX, double &CpY, double &W, double &H, double &Angle);//計算最小面積區域
	bool                       CalcPointListMomentAngle(const std::vector<TPOINT2D> &PtList, double &CpX, double &CpY, double &W, double &H, double &Angle);//計算最小面積區域
	bool                       CalcPointListMinAreaRect(const std::vector<POINT> &PtList, int AngleMin, int AngleMax, int Precs, double &CpX, double &CpY, double &W, double &H, double &Angle);//計算最小面積區域
	bool                       CalcPointListMinAreaRect(const std::vector<TPOINT2D> &PtList, int AngleMin, int AngleMax, int Precs, double &CpX, double &CpY, double &W, double &H, double &Angle);//計算最小面積區域
	bool                       CreatePointListImage(const std::vector<POINT> &PtList, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &Ptr);//建立點列表圖像
	//-----------------------------------------------------------------------------//
	//填滿
	bool                       GrayImageFloodFill(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, const RECT &Roi, int SeedX, int SeedY, unsigned char nNull, unsigned nFilled);//灰階影像填滿	
	//-----------------------------------------------------------------------------//
	//灰階範圍化
	bool                       RangeGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *&pMask, int ThL, int ThH);//灰階影像範圍化
	bool                       RangeGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *pMask, int ThL, int ThH);//灰階影像範圍化

	bool                       RangeGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, IMAGE_SIZE StepMask, unsigned char *&pMask, int ThL, int ThH);//灰階影像範圍化
	bool                       RangeGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, IMAGE_SIZE StepMask, unsigned char *pMask, int ThL, int ThH);//灰階影像範圍化
	//-----------------------------------------------------------------------------//
	//固定二值化	
	bool                       BinaryGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, int StepMask, unsigned char *&pMask, int ThL, int ThH);//灰階影像2值化
	bool                       BinaryGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, int StepMask, unsigned char *pMask, int ThL, int ThH);//灰階影像2值化

	bool                       BinarySpaceImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, const RECT &Roi, int StepMask, unsigned char *&pMask, int ThL, int ThH);//高度影像2值化
	bool                       BinarySpaceImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, const RECT &Roi, int StepMask, unsigned char *pMask, int ThL, int ThH);//高度影像2值化
	//-----------------------------------------------------------------------------//
	//適應性二值化
	bool                       AdaptiveBinaryGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, int StepMask, unsigned char *&pMask, int CalcSize, int Gap);//灰階影像適應性2值化
	bool                       AdaptiveBinaryGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, int StepMask, unsigned char *pMask, int CalcSize, int Gap);//灰階影像適應性2值化
	bool                       AdaptiveBinaryGrayImage3_Integral(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, int StepMask, unsigned char *pMask, int CalcSize, int Gap);//灰階影像適應性2值化

	bool                       AdaptiveBinarySpaceImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, const RECT &Roi, int StepMask, unsigned char *&pMask, int CalcSize, int Gap);//高度影像適應性2值化
	bool                       AdaptiveBinarySpaceImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, const RECT &Roi, int StepMask, unsigned char *pMask, int CalcSize, int Gap);//高度影像適應性2值化
	bool                       AdaptiveBinarySpaceImage3_Integral(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pSpace, const RECT &Roi, int StepMask, unsigned char *pMask, int CalcSize, int Gap);//高度影像適應性2值化
	//-----------------------------------------------------------------------------//	
	//積分圖
	bool                       BuildIntegralGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, float *&IntegralPtr);//建立灰階的積分圖
	bool                       BuildIntegralGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, float *IntegralPtr);//建立灰階的積分圖
	bool                       BuildIntegralGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, int RoiStep, float *&IntegralPtr);//建立灰階的積分圖
	bool                       BuildIntegralGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, int RoiStep, float *IntegralPtr);//建立灰階的積分圖

	bool                       BuildIntegralColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, float *&IntegralPtr);//建立彩色的積分圖
	bool                       BuildIntegralColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, float *IntegralPtr);//建立彩色的積分圖
	bool                       BuildIntegralColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, int RoiStep, float *&IntegralPtr);//建立彩色的積分圖
	bool                       BuildIntegralColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, int RoiStep, float *IntegralPtr);//建立彩色的積分圖

	bool                       BuildIntegralGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const short *pImage, float *&IntegralPtr);//建立灰階的積分圖
	bool                       BuildIntegralGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const short *pImage, float *IntegralPtr);//建立灰階的積分圖
	bool                       BuildIntegralGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const short *pImage, const RECT &Roi, int RoiStep, float *&IntegralPtr);//建立灰階的積分圖
	bool                       BuildIntegralGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const short *pImage, const RECT &Roi, int RoiStep, float *IntegralPtr);//建立灰階的積分圖

	bool                       BuildIntegralGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, float *&IntegralPtr);//建立灰階的積分圖
	bool                       BuildIntegralGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, float *IntegralPtr);//建立灰階的積分圖
	bool                       BuildIntegralGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, const RECT &Roi, int RoiStep, float *&IntegralPtr);//建立灰階的積分圖
	bool                       BuildIntegralGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, const RECT &Roi, int RoiStep, float *IntegralPtr);//建立灰階的積分圖

	bool                       BuildIntegralGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pImage, float *&IntegralPtr);//建立高度的積分圖-Float
	bool                       BuildIntegralGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pImage, float *IntegralPtr);//建立高度的積分圖-Float
	bool                       BuildIntegralGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pImage, const RECT &Roi, int RoiStep, float *&IntegralPtr);//建立高度的積分圖-Float
	bool                       BuildIntegralGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pImage, const RECT &Roi, int RoiStep, float *IntegralPtr);//建立高度的積分圖-Float

	bool                       BuildIntegralGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pImage, double *&IntegralPtr);//建立高度的積分圖-Double
	bool                       BuildIntegralGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pImage, double *IntegralPtr);//建立高度的積分圖-Double
	bool                       BuildIntegralGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pImage, const RECT &Roi, int RoiStep, double *&IntegralPtr);//建立高度的積分圖-Double
	bool                       BuildIntegralGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pImage, const RECT &Roi, int RoiStep, double *IntegralPtr);//建立高度的積分圖-Double
	//-----------------------------------------------------------------------------//	
	//遮罩
	bool                       BuildAlphaMaskTable(float Alpha, int Table[], int TableSize);//建立Alpha(透明度)查表)
	bool                       MergeMaskImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *MaskPtr1, const unsigned char *MaskPtr2, const RECT &Roi, int Mode, unsigned char *&MaskDst);//邏輯遮罩
	bool                       MergeMaskImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *MaskPtr1, const unsigned char *MaskPtr2, const RECT &Roi, int Mode, unsigned char *MaskDst);//邏輯遮罩

	bool                       InvertMaskImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pMask);//遮罩反向 Hight->Low

	bool                       InvertMaskImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, const RECT &Roi, IMAGE_SIZE StepMask, unsigned char *&pMask);//遮罩反向 Hight->Low
	bool                       InvertMaskImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, const RECT &Roi, IMAGE_SIZE StepMask, unsigned char *pMask);//遮罩反向 Hight->Low

	bool                       GrayImageApplyMask(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, const RECT &Roi, int StepMask, unsigned char *pMask, unsigned char Mask, unsigned char mskGray, unsigned char Alpha);//灰階影像合併遮罩
	bool                       ColorImageApplyMask(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, const RECT &Roi, int StepMask, unsigned char *pMask, unsigned char Mask, unsigned char mskR, unsigned char mskG, unsigned char mskB, unsigned char Alpha);//彩色影像合併遮罩
	bool                       RGBImageApplyMask(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pR, unsigned char *pG, unsigned char *pB, const RECT &Roi, int StepMask, unsigned char *pMask, unsigned char Mask, unsigned char mskR, unsigned char mskG, unsigned char mskB, unsigned char Alpha);//彩色影像合併遮罩
	//-----------------------------------------------------------------------------//
	bool                       Union2MaskImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *SrcMaskPtr1, const unsigned char *SrcMaskPtr2, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *&DstMaskPtr, unsigned char High, unsigned char Low, bool FullSize);//合併兩個遮罩-聯集
	bool                       Union2MaskImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *SrcMaskPtr1, const unsigned char *SrcMaskPtr2, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *DstMaskPtr, unsigned char High, unsigned char Low, bool FullSize);//合併兩個遮罩-聯集
	bool                       Intersection2MaskImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *SrcMaskPtr1, const unsigned char *SrcMaskPtr2, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *&DstMaskPtr, unsigned char High, unsigned char Low, bool FullSize);//合併兩個遮罩-交集
	bool                       Intersection2MaskImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *SrcMaskPtr1, const unsigned char *SrcMaskPtr2, const RECT &RoiRect, IMAGE_SIZE RoiStep, unsigned char *DstMaskPtr, unsigned char High, unsigned char Low, bool FullSize);//合併兩個遮罩-交集
	//-----------------------------------------------------------------------------//
	bool                       CalcLineEquation2D(const std::vector<POINT> &List, TLineEquation2D &Line);//取得灰階影像的線段方程式ax+by+c=0
	double                     CalcLineIncludedAngle(const TLineEquation2D &L1, const TLineEquation2D &L2) const;//計算2線段夾角
	bool                       CalcGrayImageLineEquation2D(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *ImagePtr, const RECT &RoiRect, TLineEquation2D &Line);//取得灰階影像的線段方程式ax+by+c=0
	bool                       CalcGrayImageLineEquation2D_v2(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *ImagePtr, const RECT &RoiRect, TLineEquation2D &Line);//取得灰階影像的線段方程式ax+by+c=0
	//-----------------------------------------------------------------------------//
	bool                       CalcImageProfile(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, const POINT &pt1, const POINT &pt2, std::vector<TPIXEL_GRY> &Profile);//取得影像的Profile

	bool                       CalcGrayImageProfile(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const POINT &pt1, const POINT &pt2, std::vector<TPIXEL_GRY> &Profile);//取得灰階影像的Profile
	bool                       CalcColorImageProfile(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const POINT &pt1, const POINT &pt2, std::vector<TPIXEL_GRY> &Profile);//取得彩色影像的Profile

	bool                       CalcColorImageProfile(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const POINT &pt1, const POINT &pt2, std::vector<TPIXEL_RGB> &Profile);//取得彩色影像的Profile
	bool                       CalcRGBImageProfile(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, const POINT &pt1, const POINT &pt2, std::vector<TPIXEL_RGB> &Profile);//取得彩色影像的Profile
	//-----------------------------------------------------------------------------//
	//依據Sine波形計算影像Gamma
	bool                       CalcGammaBySineProfile(const std::vector<unsigned char> &Profile, float LowRatio, int PeriodGap, int &SineCount, double &Gamma);
	bool                       CalcGrayImageGammaBySine(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, bool bVer, int nBand, float LowRatio, int PeriodGap, int &SineCount, double &Gamma);
	bool                       CalcGrayImageGammaBySine_H(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, int nBand, float LowRatio, int PeriodGap, int &SineCount, double &Gamma);
	bool                       CalcGrayImageGammaBySine_V(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, int nBand, float LowRatio, int PeriodGap, int &SineCount, double &Gamma);	
	//-----------------------------------------------------------------------------//

	//細線化
	bool                       ThinningGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, int MaskStep, unsigned char *&pMask, int ThL, int ThH);
	bool                       ThinningGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, const RECT &Roi, int MaskStep, unsigned char *pMask, int ThL, int ThH);
	bool                       ThinningGrayImageKernel(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, const RECT &Roi);
	//-----------------------------------------------------------------------------//
	//邊緣搜尋-對2值化後的圖
	bool                       EdgeSearchImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, BOX_TOWARD Toward, const RECT &Limit, int Threshold, int &Pos, int &Range);//影像邊緣搜尋
	bool                       EdgeSearchImageRoi(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, BOX_TOWARD Toward, const RECT &RoiRect, const RECT &Limit, int Threshold, int &Pos, int &Range);//影像邊緣搜尋
	//-----------------------------------------------------------------------------//
	//邊緣強化-Sobel
	bool                       SobelImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned char *&pDst);//影像邊緣強化-Sobel
	bool                       SobelImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned char *pDst);//影像邊緣強化-Sobel

	bool                       SobelGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *&pDst);//灰階影像邊緣強化-Sobel
	bool                       SobelGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *pDst);//灰階影像邊緣強化-Sobel
	bool                       SobelColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *&pDst);//彩色影像邊緣強化-Sobel
	bool                       SobelColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *pDst);//彩色影像邊緣強化-Sobel
	//-----------------------------------------------------------------------------//
	//邊緣強化-Sobel-單向
	bool                       EdgeImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned char *&pDst, int Mode);//影像邊緣強化-Sobel-單邊
	bool                       EdgeImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned char *pDst, int Mode);//影像邊緣強化-Sobel-單邊

	bool                       EdgeGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *&pDst, int Mode);//灰階影像邊緣強化-Sobel-單邊
	bool                       EdgeGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *pDst, int Mode);//灰階影像邊緣強化-Sobel-單邊
	bool                       EdgeColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *&pDst, int Mode);//彩色影像邊緣強化-Sobel-單邊
	bool                       EdgeColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *pDst, int Mode);//彩色影像邊緣強化-Sobel-單邊
	//-----------------------------------------------------------------------------//	
	//邊緣強化-Scharr
	bool                       ScharrImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned char *&pDst);//影像邊緣強化-Scharr
	bool                       ScharrImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned char *pDst);//影像邊緣強化-Scharr

	bool                       ScharrGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *&pDst);//灰階影像邊緣強化-Scharr
	bool                       ScharrGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *pDst);//灰階影像邊緣強化-Scharr
	bool                       ScharrColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *&pDst);//彩色影像邊緣強化-Scharr
	bool                       ScharrColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned char *pDst);//彩色影像邊緣強化-Scharr
	//-----------------------------------------------------------------------------//
	//單張平均化處理
	bool                       SmoothImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *&pDst);//單張影像平均濾波
	bool                       SmoothImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *pDst);//單張影像平均濾波

	bool                       SmoothGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *&pDst);//單張灰階影像平均濾波
	bool                       SmoothGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *pDst);//單張灰階影像平均濾波
	bool                       SmoothColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *&pDst);//單張彩色影像平均濾波
	bool                       SmoothColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *pDst);//單張彩色影像平均濾波

	bool                       SmoothShortGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const short *pImage, IMAGE_SIZE KerSize, short *&pDst);//單張灰階影像平均濾波
	bool                       SmoothShortGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const short *pImage, IMAGE_SIZE KerSize, short *pDst);//單張灰階影像平均濾波

	bool                       SmoothShortGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, IMAGE_SIZE KerSize, unsigned short *&pDst);//單張灰階影像平均濾波
	bool                       SmoothShortGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, IMAGE_SIZE KerSize, unsigned short *pDst);//單張灰階影像平均濾波
	//-----------------------------------------------------------------------------//
	//單張平均化處理-積分圖
	bool                       SmoothImageByIntegral(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *&pDst);//單張影像平均濾波
	bool                       SmoothImageByIntegral3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *pDst);//單張影像平均濾波

	bool                       SmoothGrayImageByIntegral(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *&pDst);//單張灰階影像平均濾波
	bool                       SmoothGrayImageByIntegral3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *pDst);//單張灰階影像平均濾波
	bool                       SmoothColorImageByIntegral(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *&pDst);//單張彩色影像平均濾波
	bool                       SmoothColorImageByIntegral3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *pDst);//單張彩色影像平均濾波

	bool                       SmoothShortGrayImageByIntegral(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const short *pImage, IMAGE_SIZE KerSize, short *&pDst);//單張灰階影像平均濾波
	bool                       SmoothShortGrayImageByIntegral3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const short *pImage, IMAGE_SIZE KerSize, short *pDst);//單張灰階影像平均濾波

	bool                       SmoothShortGrayImageByIntegral(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, IMAGE_SIZE KerSize, unsigned short *&pDst);//單張灰階影像平均濾波
	bool                       SmoothShortGrayImageByIntegral3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, IMAGE_SIZE KerSize, unsigned short *pDst);//單張灰階影像平均濾波
	//-----------------------------------------------------------------------------//
	//中通濾波處理	
	bool                       MedianImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *&pDst);//單張影像中通濾波
	bool                       MedianImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *pDst);//單張影像中通濾波
	bool                       MedianGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *&pDst);//單張灰階影像中通濾波
	bool                       MedianGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *pDst);//單張灰階影像中通濾波
	bool                       MedianColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *&pDst);//單張彩色影像中通濾波
	bool                       MedianColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *pDst);//單張彩色影像中通濾波

	bool                       MedianShortGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const short *pImage, IMAGE_SIZE KerSize, short *&pDst);//單張灰階影像中通濾波
	bool                       MedianShortGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const short *pImage, IMAGE_SIZE KerSize, short *pDst);//單張灰階影像中通濾波
	bool                       MedianShortGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, IMAGE_SIZE KerSize, unsigned short *&pDst);//單張灰階影像中通濾波
	bool                       MedianShortGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, IMAGE_SIZE KerSize, unsigned short *pDst);//單張灰階影像中通濾波

	bool                       MedianSpaceGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pImage, IMAGE_SIZE KerSize, float *&pDst);//單張灰階影像中通濾波
	bool                       MedianSpaceGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pImage, IMAGE_SIZE KerSize, float *pDst);//單張灰階影像中通濾波
	//-----------------------------------------------------------------------------//
	//高斯濾波
	bool                       GaussianImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *&pDst);//單張影像中通濾波
	bool                       GaussianImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *pDst);//單張影像中通濾波
	bool                       GaussianGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *&pDst);//單張灰階影像中通濾波
	bool                       GaussianGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *pDst);//單張灰階影像中通濾波
	bool                       GaussianColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *&pDst);//單張彩色影像中通濾波
	bool                       GaussianColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, IMAGE_SIZE KerSize, unsigned char *pDst);//單張彩色影像中通濾波

	bool                       GaussianShortGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, IMAGE_SIZE KerSize, unsigned short *&pDst);//單張灰階影像中通濾波
	bool                       GaussianShortGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, IMAGE_SIZE KerSize, unsigned short *pDst);//單張灰階影像中通濾波
	//-----------------------------------------------------------------------------//
	//Non-Local Means
	bool                       NonLocalMeanGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, float h, int szPatch, int szSearch, unsigned char *&pDst);//單張灰階影像NL-Filter
	bool                       NonLocalMeanGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, float h, int szPatch, int szSearch, unsigned char *pDst);//單張灰階影像NL-Filter
	//-----------------------------------------------------------------------------//
	//侵蝕
	bool                       ErodeImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned int KenSize, unsigned int IterCount, unsigned char *&pDst);//單張影像侵蝕濾波
	bool                       ErodeImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned int KenSize, unsigned int IterCount, unsigned char *pDst);//單張影像侵蝕濾波
	bool                       ErodeGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned int KenSize, unsigned int IterCount, unsigned char *&pDst);//單張灰階影像侵蝕濾波
	bool                       ErodeGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned int KenSize, unsigned int IterCount, unsigned char *pDst);//單張灰階影像侵蝕濾波
	bool                       ErodeColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned int KenSize, unsigned int IterCount, unsigned char *&pDst);//單張彩色影像侵蝕濾波
	bool                       ErodeColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned int KenSize, unsigned int IterCount, unsigned char *pDst);//單張彩色影像侵蝕濾波

	//膨脹
	bool                       DilateImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned int KenSize, unsigned int IterCount, unsigned char *&pDst);//單張影像膨脹濾波
	bool                       DilateImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, unsigned int KenSize, unsigned int IterCount, unsigned char *pDst);//單張影像膨脹濾波
	bool                       DilateGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned int KenSize, unsigned int IterCount, unsigned char *&pDst);//單張灰階影像膨脹濾波
	bool                       DilateGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned int KenSize, unsigned int IterCount, unsigned char *pDst);//單張灰階影像膨脹濾波
	bool                       DilateColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned int KenSize, unsigned int IterCount, unsigned char *&pDst);//單張彩色影像膨脹濾波
	bool                       DilateColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, unsigned int KenSize, unsigned int IterCount, unsigned char *pDst);//單張彩色影像膨脹濾波

	bool                       DilateNoiseMaskImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const MASK_PTR MaskPtr, unsigned int KenSize, unsigned int IterCount, MASK_PTR &DstPtr);//遮罩影像膨脹濾波
	bool                       DilateNoiseMaskImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const MASK_PTR MaskPtr, unsigned int KenSize, unsigned int IterCount, MASK_PTR DstPtr);//遮罩影像膨脹濾波
	
	//Morh形態操作 MORPH_OPEN, MORPH_CLOSE, MORPH_GRADIENT, MORPH_TOPHAT, MORPH_BLACKHAT
	bool                       MorphImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int MorphMode, int ShpaeMode, unsigned int KenSize, unsigned int IterCount, unsigned char *&pDst);//單張影像形態操作
	bool                       MorphImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int MorphMode, int ShpaeMode, unsigned int KenSize, unsigned int IterCount, unsigned char *pDst);//單張影像形態操作
	bool                       MorphGrayImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int MorphMode, int ShpaeMode, unsigned int KenSize, unsigned int IterCount, unsigned char *&pDst);//單張灰階影像形態操作
	bool                       MorphGrayImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int MorphMode, int ShpaeMode, unsigned int KenSize, unsigned int IterCount, unsigned char *pDst);//單張灰階影像形態操作
	bool                       MorphColorImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int MorphMode, int ShpaeMode, unsigned int KenSize, unsigned int IterCount, unsigned char *&pDst);//單張彩色影像形態操作
	bool                       MorphColorImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, int MorphMode, int ShpaeMode, unsigned int KenSize, unsigned int IterCount, unsigned char *pDst);//單張彩色影像形態操作
	//-----------------------------------------------------------------------------//		
	//多張平均化處理
	bool                       AverageImage2Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *Ptr1, const unsigned char *Ptr2, unsigned char *&pDst);//2張影像平均濾波
	bool                       AverageImage2Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *Ptr1, const unsigned char *Ptr2, unsigned char *pDst);//2張影像平均濾波
	bool                       AverageImage3Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, unsigned char *&pDst);//3張影像平均濾波
	bool                       AverageImage3Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, unsigned char *pDst);//3張影像平均濾波
	bool                       AverageImage4Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, unsigned char *&pDst);//4張影像平均濾波
	bool                       AverageImage4Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, unsigned char *pDst);//4張影像平均濾波

	bool                       AverageGrayImage2Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, unsigned char *&pDst);//2張灰階影像平均濾波
	bool                       AverageGrayImage2Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, unsigned char *pDst);//2張灰階影像平均濾波
	bool                       AverageGrayImage3Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, unsigned char *&pDst);//3張灰階影像平均濾波
	bool                       AverageGrayImage3Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, unsigned char *pDst);//3張灰階影像平均濾波
	bool                       AverageGrayImage4Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, unsigned char *&pDst);//4張灰階影像平均濾波
	bool                       AverageGrayImage4Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, unsigned char *pDst);//4張灰階影像平均濾波

	bool                       AverageColorImage2Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, unsigned char *&pDst);//2張彩色影像平均濾波
	bool                       AverageColorImage2Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, unsigned char *pDst);//2張彩色影像平均濾波
	bool                       AverageColorImage3Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, unsigned char *&pDst);//3張彩色影像平均濾波
	bool                       AverageColorImage3Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, unsigned char *pDst);//3張彩色影像平均濾波
	bool                       AverageColorImage4Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, unsigned char *&pDst);//4張彩色影像平均濾波
	bool                       AverageColorImage4Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, unsigned char *pDst);//4張彩色影像平均濾波
	//-----------------------------------------------------------------------------//
	//多張最亮灰階
	bool                       MaxValueImage2Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *Ptr1, const unsigned char *Ptr2, unsigned char *&pDst);//2張影像最高亮度
	bool                       MaxValueImage2Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *Ptr1, const unsigned char *Ptr2, unsigned char *pDst);//2張影像最高亮度
	bool                       MaxValueGrayImage2Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, unsigned char *&pDst);//2張灰階影像最高亮度
	bool                       MaxValueGrayImage2Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, unsigned char *pDst);//2張灰階影像最高亮度
	bool                       MaxValueColorImage2Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, unsigned char *&pDst);//2張彩色影像最高亮度
	bool                       MaxValueColorImage2Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, unsigned char *pDst);//2張彩色影像最高亮度

	bool                       MaxValueImage3Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, unsigned char *&pDst);//3張影像最高亮度
	bool                       MaxValueImage3Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, unsigned char *pDst);//3張影像最高亮度
	bool                       MaxValueGrayImage3Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, unsigned char *&pDst);//3張灰階影像最高亮度
	bool                       MaxValueGrayImage3Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, unsigned char *pDst);//3張灰階影像最高亮度
	bool                       MaxValueColorImage3Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, unsigned char *&pDst);//3張彩色影像最高亮度
	bool                       MaxValueColorImage3Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, unsigned char *pDst);//3張彩色影像最高亮度

	bool                       MaxValueImage4Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr4, const unsigned char *Ptr3, unsigned char *&pDst);//4張影像最高亮度
	bool                       MaxValueImage4Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr4, const unsigned char *Ptr3, unsigned char *pDst);//4張影像最高亮度
	bool                       MaxValueGrayImage4Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, unsigned char *&pDst);//4張灰階影像最高亮度
	bool                       MaxValueGrayImage4Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, unsigned char *pDst);//4張灰階影像最高亮度
	bool                       MaxValueColorImage4Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, unsigned char *&pDst);//4張彩色影像最高亮度
	bool                       MaxValueColorImage4Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, unsigned char *pDst);//4張彩色影像最高亮度
	//-----------------------------------------------------------------------------//		
	//合併2D影像
	bool                       MergeGrayImage2Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, unsigned char *&pDst);//合併2張灰階影像
	bool                       MergeGrayImage2Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, unsigned char *pDst);//合併2張灰階影像
	bool                       MergeGrayImage3Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, unsigned char *&pDst);//合併3張灰階影像
	bool                       MergeGrayImage3Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, unsigned char *pDst);//合併3張灰階影像
	bool                       MergeGrayImage4Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, unsigned char *&pDst);//合併4張灰階影像
	bool                       MergeGrayImage4Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, unsigned char *pDst);//合併4張灰階影像
	bool                       MergeGrayImage8Frame(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, const unsigned char *Ptr5, const unsigned char *Ptr6, const unsigned char *Ptr7, const unsigned char *Ptr8, unsigned char *&pDst);//合併8張灰階影像
	bool                       MergeGrayImage8Frame3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, const unsigned char *Ptr5, const unsigned char *Ptr6, const unsigned char *Ptr7, const unsigned char *Ptr8, unsigned char *pDst);//合併8張灰階影像
	//-----------------------------------------------------------------------------//
	//合併2D影像-依據像素焦點
	bool                       MergeImage2FrameByFocus(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const IMAGE_PTR Ptr1, const IMAGE_PTR Ptr2, IMAGE_SIZE FocusStep, const IMAGE_PTR FocusPtr1, const IMAGE_PTR FocusPtr2, IMAGE_PTR &pDst);//合併2張影像
	bool                       MergeImage2FrameByFocus3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const IMAGE_PTR Ptr1, const IMAGE_PTR Ptr2, IMAGE_SIZE FocusStep, const IMAGE_PTR FocusPtr1, const IMAGE_PTR FocusPtr2, IMAGE_PTR pDst);//合併2張影像
	bool                       MergeGrayImage2FrameByFocus(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const IMAGE_PTR Ptr1, const IMAGE_PTR Ptr2, IMAGE_SIZE FocusStep, const IMAGE_PTR FocusPtr1, const IMAGE_PTR FocusPtr2, IMAGE_PTR &pDst);//合併2張灰階影像
	bool                       MergeGrayImage2FrameByFocus3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const IMAGE_PTR Ptr1, const IMAGE_PTR Ptr2, IMAGE_SIZE FocusStep, const IMAGE_PTR FocusPtr1, const IMAGE_PTR FocusPtr2, IMAGE_PTR pDst);//合併2張灰階影像
	bool                       MergeColorImage2FrameByFocus(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const IMAGE_PTR Ptr1, const IMAGE_PTR Ptr2, IMAGE_SIZE FocusStep, const IMAGE_PTR FocusPtr1, const IMAGE_PTR FocusPtr2, IMAGE_PTR &pDst);//合併2張彩色影像
	bool                       MergeColorImage2FrameByFocus3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const IMAGE_PTR Ptr1, const IMAGE_PTR Ptr2, IMAGE_SIZE FocusStep, const IMAGE_PTR FocusPtr1, const IMAGE_PTR FocusPtr2, IMAGE_PTR pDst);//合併2張彩色影像

	bool                       MergeSpace2FrameByFocus(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR sPtr1, const SPACE_PTR sPtr2, const MASK_PTR mPtr1, const MASK_PTR mPtr2, IMAGE_SIZE FocusStep, const IMAGE_PTR FocusPtr1, const IMAGE_PTR FocusPtr2, SPACE_PTR &sDst, MASK_PTR &mDst);//合併2張空間資料
	bool                       MergeSpace2FrameByFocus3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR sPtr1, const SPACE_PTR sPtr2, const MASK_PTR mPtr1, const MASK_PTR mPtr2, IMAGE_SIZE FocusStep, const IMAGE_PTR FocusPtr1, const IMAGE_PTR FocusPtr2, SPACE_PTR sDst, MASK_PTR mDst);//合併2張空間資料
	//-----------------------------------------------------------------------------//	
	//圖像金字塔
	bool                       PyramidUpImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//圖像金字塔變大
	bool                       PyramidDownImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//圖像金字塔變小
	bool                       PyramidUpSpace(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR pSpace, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, SPACE_PTR &pDst, bool UsingOpenCV = true);//空間金字塔變大
	bool                       PyramidDownSpace(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR pSpace, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, SPACE_PTR &pDst, bool UsingOpenCV = false);//空間金字塔變小
	bool                       PyramidDownSpace(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR pSpace, const MASK_PTR pMask, const MASK_PTR pMaskC, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, SPACE_PTR &pDst, MASK_PTR &pDstM, MASK_PTR &pDstMC);//空間金字塔變小
	bool                       PyramidUpMask(IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, const MASK_PTR pMask, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, MASK_PTR &pDst, bool UsingOpenCV = true);//Mask金字塔變大
	bool                       PyramidDownMask(IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, const MASK_PTR pMask, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, MASK_PTR &pDst, bool UsingOpenCV = false);//Mask金字塔變小
	//-----------------------------------------------------------------------------//
	//建立空間中物體列表
	bool                       BuildSpaceObjectList(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, int KenSize, std::vector<RECT> &ObjList, std::vector<float> &ObjHList);
	//-----------------------------------------------------------------------------//
	//建立對焦影像-計算出每個像素的對焦值
	bool                       BuildFocusPixelImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const IMAGE_PTR ImagePtr, int BlurSize, int RoiSize, IMAGE_PTR &DstPtr);	
	bool                       BuildFocusPixelImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const IMAGE_PTR ImagePtr, int BlurSize, int RoiSize, IMAGE_PTR DstPtr);
	//-----------------------------------------------------------------------------//
	//比較兩張對焦影像
	bool                       Compare2FocusPixelImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const IMAGE_PTR Ptr1, const IMAGE_PTR Ptr2, IMAGE_DATA v1, IMAGE_DATA v2, IMAGE_PTR &DstPtr);
	bool                       Compare2FocusPixelImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const IMAGE_PTR Ptr1, const IMAGE_PTR Ptr2, IMAGE_DATA v1, IMAGE_DATA v2, IMAGE_PTR DstPtr);
	//-----------------------------------------------------------------------------//
	//影像混色
	bool                       BlendImage(IMAGE_SIZE W1, IMAGE_SIZE H1, IMAGE_SIZE Step1, IMAGE_SIZE Bit1, const unsigned char *Ptr1, IMAGE_SIZE W2, IMAGE_SIZE H2, IMAGE_SIZE Step2, IMAGE_SIZE Bit2, const unsigned char *Ptr2, IMAGE_SIZE Wm, IMAGE_SIZE Hm, IMAGE_SIZE Stepm, const unsigned char *PtrM, const RECT &Roi, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//兩張影像混色
	bool                       BlendGrayImage(IMAGE_SIZE W1, IMAGE_SIZE H1, IMAGE_SIZE Step1, const unsigned char *Ptr1, IMAGE_SIZE W2, IMAGE_SIZE H2, IMAGE_SIZE Step2, const unsigned char *Ptr2, IMAGE_SIZE Wm, IMAGE_SIZE Hm, IMAGE_SIZE Stepm, const unsigned char *PtrM, const RECT &Roi, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//兩張影像混色
	bool                       BlendColorImage(IMAGE_SIZE W1, IMAGE_SIZE H1, IMAGE_SIZE Step1, const unsigned char *Ptr1, IMAGE_SIZE W2, IMAGE_SIZE H2, IMAGE_SIZE Step2, const unsigned char *Ptr2, IMAGE_SIZE Wm, IMAGE_SIZE Hm, IMAGE_SIZE Stepm, const unsigned char *PtrM, const RECT &Roi, IMAGE_SIZE &DstW, IMAGE_SIZE &DstH, IMAGE_SIZE &DstStep, unsigned char *&pDst);//兩張影像混色
	//-----------------------------------------------------------------------------//
	//建立相位測試圖片
	bool                       GrayImageCreateCosPattern(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *&pImage, int NPixelPeriod, double PhaseShift, bool bVer);//建立灰階sin樣板圖像
	bool                       GrayImageCreateCosPattern3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, int NPixelPeriod, double PhaseShift, bool bVer);//建立灰階sin樣板圖像

	bool                       GrayImageCreateSinPatternII(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *&pImage, int NPixelPeriod, double PhaseShift, bool bVer);//建立灰階sin樣板圖像
	bool                       GrayImageCreateSinPatternII3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, int NPixelPeriod, double PhaseShift, bool bVer);//建立灰階sin樣板圖像

	bool                       ColorImageCreateSinPattern(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *&pImage, int NPixelPeriod, double PhaseShift, bool bVer);//建立彩色sin樣板圖像
	bool                       ColorImageCreateSinPattern3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pImage, int NPixelPeriod, double PhaseShift, bool bVer);//建立彩色sin樣板圖像	
	//-----------------------------------------------------------------------------//
	unsigned int               DecimaltoGrayCode(unsigned int x);//10進位轉GrayCode
	unsigned int               GrayCodetoDecimal(unsigned int x);//GrayCode轉10進位
	bool                       BuildGrayCodeMapTable(int Order, std::vector<int> &List);//取得GrayCode的映射表	
	bool                       BuildGrayCodeMapTable(int Order, std::vector<unsigned char> &List);//取得GrayCode的映射表
	//-----------------------------------------------------------------------------//	
	bool                       SwapImagePtr(IMAGE_PTR Ptr[], IMAGE_SIZE Count, bool bCheck);//指標反轉, 1->5, 2->4, 注意因部分指標為NULL, 反轉後須跳過NULL指標
	bool                       SwapImagePtr5(IMAGE_PTR &Ptr1, IMAGE_PTR &Ptr2, IMAGE_PTR &Ptr3, IMAGE_PTR &Ptr4, IMAGE_PTR &Ptr5, bool bCheck);//指標反轉, 1->5, 2->4, 注意因部分指標為NULL, 反轉後須跳過NULL指標	
	//-----------------------------------------------------------------------------//
	//解相位
	bool                       GrayImage21FrameToPhase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, int PatternID, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &pMask, PHASE_PTR &PhasePtr);//2+1張灰階影像解相位
	bool                       GrayImage21FrameToPhase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, int PatternID, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR pMask, PHASE_PTR PhasePtr);//2+1張灰階影像解相位	
	bool                       GrayImage21FrameToPhase2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, int PatternID, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &pMask, PHASE_PTR &PhasePtr);//2+1張灰階影像解相位-2曝光
	bool                       GrayImage21FrameToPhase2Exp3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, int PatternID, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR pMask, PHASE_PTR PhasePtr);//2+1張灰階影像解相位-2曝光

	bool                       GrayImage3FrameToPhase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, int PatternID, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &pMask, PHASE_PTR &PhasePtr);//3張灰階影像解相位
	bool                       GrayImage3FrameToPhase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, int PatternID, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR pMask, PHASE_PTR PhasePtr);//3張灰階影像解相位		
	bool                       GrayImage3FrameToPhase2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, int PatternID, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &pMask, PHASE_PTR &PhasePtr);//3張灰階影像解相位-2曝光
	bool                       GrayImage3FrameToPhase2Exp3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, int PatternID, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR pMask, PHASE_PTR PhasePtr);//3張灰階影像解相位-2曝光
	
	bool                       GrayImage4FrameToPhase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, int PatternID, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr);//4張灰階影像解相位
	bool                       GrayImage4FrameToPhase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, int PatternID, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr);//4張灰階影像解相位	
	bool                       GrayImage4FrameToPhase2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, int PatternID, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr);//4張灰階影像解相位-2曝光
	bool                       GrayImage4FrameToPhase2Exp3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, int PatternID, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr);//4張灰階影像解相位-2曝光	
	
	bool                       GrayImage5FrameToPhase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, const unsigned char *Ptr5, int PatternID, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr);//5張灰階影像解相位
	bool                       GrayImage5FrameToPhase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *Ptr1, const unsigned char *Ptr2, const unsigned char *Ptr3, const unsigned char *Ptr4, const unsigned char *Ptr5, int PatternID, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr);//5張灰階影像解相位
	bool                       GrayImage5FrameToPhase2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrA5, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrC5, int PatternID, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr);//5張灰階影像解相位-2曝光	
	bool                       GrayImage5FrameToPhase2Exp3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrA5, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrC5, int PatternID, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr);//5張灰階影像解相位-2曝光			

	//雙週期
	bool                       PhaseImage2PeriodeToPhase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR PPtr1, const MASK_PTR MPtr1, double P1, const PHASE_PTR PPtr2, const MASK_PTR MPtr2, double P2, const PHASE_PTR BasePhasePtr, PHASE_PTR &PPtrM, MASK_PTR &MPtrM, double &NewP, int CombineMode);//雙相位算出新相位
	bool                       PhaseImage2PeriodeToPhase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR PPtr1, const MASK_PTR MPtr1, double P1, const PHASE_PTR PPtr2, const MASK_PTR MPtr2, double P2, const PHASE_PTR BasePhasePtr, PHASE_PTR PPtrM, MASK_PTR MPtrM, double &NewP, int CombineMode);//雙相位算出新相位
	bool                       PhaseImage2PeriodeToPhase3_SiblingPeriod(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR PPtr1, const MASK_PTR MPtr1, double P1, const PHASE_PTR PPtr2, const MASK_PTR MPtr2, double P2, const PHASE_PTR BasePhasePtr, PHASE_PTR PPtrM, MASK_PTR MPtrM, double &NewP);//雙相位算出新相位-鄰近週期
	bool                       PhaseImage2PeriodeToPhase3_MaxMinPeriod(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR PPtr1, const MASK_PTR MPtr1, double P1, const PHASE_PTR PPtr2, const MASK_PTR MPtr2, double P2, const PHASE_PTR BasePhasePtr, PHASE_PTR PPtrM, MASK_PTR MPtrM, double &NewP);//雙相位算出新相位-大小週期
	
	//3步-2週期
	bool                       GrayImage3FrameTo2Phase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//2個3張灰階影像解相位
	bool                       GrayImage3FrameTo2Phase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個3張灰階影像解相位
	bool                       GrayImage3FrameTo2Phase3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個3張灰階影像解相位
	bool                       GrayImage3FrameTo2Phase3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個3張灰階影像解相位

	//3步-2週期-2曝光
	bool                       GrayImage3FrameTo2Phase2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//2個3張灰階影像解相位-2曝光
	bool                       GrayImage3FrameTo2Phase2Exp3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個3張灰階影像解相位-2曝光
	bool                       GrayImage3FrameTo2Phase2Exp3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個3張灰階影像解相位-2曝光
	bool                       GrayImage3FrameTo2Phase2Exp3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個3張灰階影像解相位-2曝光

	//4步-2週期
	bool                       GrayImage4FrameTo2Phase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//2個4張灰階影像解相位
	bool                       GrayImage4FrameTo2Phase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個4張灰階影像解相位
	bool                       GrayImage4FrameTo2Phase3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個4張灰階影像解相位
	bool                       GrayImage4FrameTo2Phase3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個4張灰階影像解相位	

	//4步-2週期-2曝光 
	bool                       GrayImage4FrameTo2Phase2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//2個4張灰階影像解相位-2曝光
	bool                       GrayImage4FrameTo2Phase2Exp3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個4張灰階影像解相位-2曝光
	bool                       GrayImage4FrameTo2Phase2Exp3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個4張灰階影像解相位-2曝光
	bool                       GrayImage4FrameTo2Phase2Exp3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個4張灰階影像解相位-2曝光
	
	//5步-2週期
	bool                       GrayImage5FrameTo2Phase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrA5, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//2個5張灰階影像解相位
	bool                       GrayImage5FrameTo2Phase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrA5, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個5張灰階影像解相位
	bool                       GrayImage5FrameTo2Phase3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrA5, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個5張灰階影像解相位
	bool                       GrayImage5FrameTo2Phase3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrA5, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個5張灰階影像解相位

	//5步-2週期-2曝光 
	bool                       GrayImage5FrameTo2Phase2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrA5, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrC5, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, const unsigned char *PtrD5, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//2個5張灰階影像解相位-2曝光 
	bool                       GrayImage5FrameTo2Phase2Exp3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrA5, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrC5, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, const unsigned char *PtrD5, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個5張灰階影像解相位-2曝光 
	bool                       GrayImage5FrameTo2Phase2Exp3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrA5, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrC5, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, const unsigned char *PtrD5, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個5張灰階影像解相位-2曝光 
	bool                       GrayImage5FrameTo2Phase2Exp3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrA5, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrC5, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, const unsigned char *PtrD5, double PerB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個5張灰階影像解相位-2曝光 
	
	//2+2步-2週期
	bool                       GrayImage221FrameTo2Phase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, double PerB, int ExpTimeB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//2+2張灰階影像解相位
	bool                       GrayImage221FrameTo2Phase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, double PerB, int ExpTimeB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2+2張灰階影像解相位		

	//2+2步-2週期-2曝光
	bool                       GrayImage221FrameTo2Phase2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrC1, const unsigned char *PtrC2, double PerA, int ExpTimeA, int ExpTimeC, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, double PerB, int ExpTimeB, int ExpTimeD, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//2+2張灰階影像解相位-2曝光
	bool                       GrayImage221FrameTo2Phase2Exp3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrC1, const unsigned char *PtrC2, double PerA, int ExpTimeA, int ExpTimeC, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, double PerB, int ExpTimeB, int ExpTimeD, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2+2張灰階影像解相位-2曝光	

	//4+2步-2週期
	bool                       GrayImage42FrameTo2Phase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, double PerB, int ExpTimeB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//4+2張灰階影像解相位
	bool                       GrayImage42FrameTo2Phase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, double PerB, int ExpTimeB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//4+2張灰階影像解相位
	bool                       GrayImage42FrameTo2Phase3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, double PerB, int ExpTimeB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//4+2張灰階影像解相位
	

	//4+2步-2週期-2曝光
	bool                       GrayImage42FrameTo2Phase2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerA, int ExpTimeA, int ExpTimeC, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrD1, const unsigned char *PtrD2, double PerB, int ExpTimeB, int ExpTimeD, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//4+2張灰階影像解相位-2曝光
	bool                       GrayImage42FrameTo2Phase2Exp3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerA, int ExpTimeA, int ExpTimeC, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrD1, const unsigned char *PtrD2, double PerB, int ExpTimeB, int ExpTimeD, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//4+2張灰階影像解相位-2曝光
	bool                       GrayImage42FrameTo2Phase2Exp3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerA, int ExpTimeA, int ExpTimeC, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrD1, const unsigned char *PtrD2, double PerB, int ExpTimeB, int ExpTimeD, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//4+2張灰階影像解相位-2曝光

	//4步+4GC-2週期
	bool                       GrayImage44GCFrameTo2Phase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PerB, int ExpTimeB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//4+4GC張灰階影像解相位
	bool                       GrayImage44GCFrameTo2Phase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PerB, int ExpTimeB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//4+4GC張灰階影像解相位
	bool                       GrayImage44GCFrameTo2Phase3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PerB, int ExpTimeB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//4+4GC張灰階影像解相位

	//4步+4GC-2週期-2曝光
	bool                       GrayImage44GCFrameTo2Phase2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerA, int ExpTimeA, int ExpTimeC, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, double PerB, int ExpTimeB, int ExpTimeD, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//2個4張+4GC灰階影像解相位-2曝光
	bool                       GrayImage44GCFrameTo2Phase2Exp3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerA, int ExpTimeA, int ExpTimeC, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, double PerB, int ExpTimeB, int ExpTimeD, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個4張+4GC灰階影像解相位-2曝光
	bool                       GrayImage44GCFrameTo2Phase2Exp3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerA, int ExpTimeA, int ExpTimeC, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, double PerB, int ExpTimeB, int ExpTimeD, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個4張+4GC灰階影像解相位-2曝光

	//4步+5GC-2週期
	bool                       GrayImage45GCFrameTo2Phase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, double PerB, int ExpTimeB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//4+5GC張灰階影像解相位
	bool                       GrayImage45GCFrameTo2Phase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, double PerB, int ExpTimeB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//4+5GC張灰階影像解相位
	bool                       GrayImage45GCFrameTo2Phase3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, double PerB, int ExpTimeB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//4+5GC張灰階影像解相位

	//4步+5GC-2週期-2曝光
	bool                       GrayImage45GCFrameTo2Phase2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerA, int ExpTimeA, int ExpTimeC, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, const unsigned char *PtrD5, double PerB, int ExpTimeB, int ExpTimeD, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//2個4張+5GC灰階影像解相位-2曝光
	bool                       GrayImage45GCFrameTo2Phase2Exp3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerA, int ExpTimeA, int ExpTimeC, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, const unsigned char *PtrD5, double PerB, int ExpTimeB, int ExpTimeD, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個4張+5GC灰階影像解相位-2曝光
	bool                       GrayImage45GCFrameTo2Phase2Exp3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerA, int ExpTimeA, int ExpTimeC, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, const unsigned char *PtrD5, double PerB, int ExpTimeB, int ExpTimeD, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個4張+5GC灰階影像解相位-2曝光

	//4步+6GC-2週期
	bool                       GrayImage46GCFrameTo2Phase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, const unsigned char *PtrB6, double PerB, int ExpTimeB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//4+6GC張灰階影像解相位
	bool                       GrayImage46GCFrameTo2Phase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, const unsigned char *PtrB6, double PerB, int ExpTimeB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//4+6GC張灰階影像解相位
	bool                       GrayImage46GCFrameTo2Phase3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, const unsigned char *PtrB6, double PerB, int ExpTimeB, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//4+6GC張灰階影像解相位

	//4步+6GC-2週期-2曝光
	bool                       GrayImage46GCFrameTo2Phase2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerA, int ExpTimeA, int ExpTimeC, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, const unsigned char *PtrB6, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, const unsigned char *PtrD5, const unsigned char *PtrD6, double PerB, int ExpTimeB, int ExpTimeD, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//2個4張+6GC灰階影像解相位-2曝光
	bool                       GrayImage46GCFrameTo2Phase2Exp3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerA, int ExpTimeA, int ExpTimeC, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, const unsigned char *PtrB6, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, const unsigned char *PtrD5, const unsigned char *PtrD6, double PerB, int ExpTimeB, int ExpTimeD, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個4張+6GC灰階影像解相位-2曝光
	bool                       GrayImage46GCFrameTo2Phase2Exp3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerA, int ExpTimeA, int ExpTimeC, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, const unsigned char *PtrB6, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, const unsigned char *PtrD5, const unsigned char *PtrD6, double PerB, int ExpTimeB, int ExpTimeD, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//2個4張+6GC灰階影像解相位-2曝光

	//三週期, 使用大小週期, 無需三張相位週期//20161012
	//3步-3週期
	bool                       GrayImage3FrameTo3Phase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, double PerB, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, double PerC, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//3個3張灰階影像解相位
	bool                       GrayImage3FrameTo3Phase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, double PerB, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, double PerC, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//3個3張灰階影像解相位
	bool                       GrayImage3FrameTo3Phase3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, double PerB, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, double PerC, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//3個3張灰階影像解相位
	bool                       GrayImage3FrameTo3Phase3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, double PerB, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, double PerC, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//3個3張灰階影像解相位

	//4步-3週期
	bool                       GrayImage4FrameTo3Phase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PerB, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerC, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//3個4張灰階影像解相位
	bool                       GrayImage4FrameTo3Phase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PerB, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerC, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//3個4張灰階影像解相位
	bool                       GrayImage4FrameTo3Phase3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PerB, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerC, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//3個4張灰階影像解相位
	bool                       GrayImage4FrameTo3Phase3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PerB, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, double PerC, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//3個4張灰階影像解相位	

	//5步-3週期
	bool                       GrayImage5FrameTo3Phase(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrA5, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, double PerB, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrC5, double PerC, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR &MaskPtr, PHASE_PTR &PhasePtr, double &PerD);//3個5張灰階影像解相位
	bool                       GrayImage5FrameTo3Phase3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrA5, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, double PerB, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrC5, double PerC, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//3個5張灰階影像解相位
	bool                       GrayImage5FrameTo3Phase3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrA5, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, double PerB, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrC5, double PerC, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//3個5張灰階影像解相位
	bool                       GrayImage5FrameTo3Phase3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrA5, double PerA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, double PerB, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrC5, double PerC, float Gamma, const PHASE_PTR BasePhasePtr, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, PHASE_PTR PhasePtr, double &PerD);//3個5張灰階影像解相位
	//-----------------------------------------------------------------------------//
	bool                       CorrectPhaseZero_Joe(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR SrcPtr, PHASE_PTR &DstPtr, int Times);//相平面濾波
	bool                       CorrectPhaseZero_Joe3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR SrcPtr, PHASE_PTR DstPtr, int Times);//相平面濾波
	//-----------------------------------------------------------------------------//
	bool                       CorrectHeightFactor_Joe(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR SrcPtr, SPACE_PTR &DstPtr, int Times);//修正高度係數
	bool                       CorrectHeightFactor_Joe3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const SPACE_PTR SrcPtr, SPACE_PTR DstPtr, int Times);//修正高度係數
	//-----------------------------------------------------------------------------//
	bool                       PatternCast1ToSpace(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR &MaskPtr, SPACE_PTR &SpacePtr);//1投光影像轉成高度資料
	bool                       PatternCast1ToSpace3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光影像轉成高度資料
	bool                       PatternCast1ToSpace3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光影像轉成高度資料
	bool                       PatternCast1ToSpace3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光影像轉成高度資料
	bool                       PatternCast1ToSpace3_II_SP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光影像轉成高度資料
	bool                       PatternCast1ToSpace3_II_MP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光影像轉成高度資料
	bool                       PatternCast1ToSpace3_II_MP_Fn(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光影像轉成高度資料
	bool                       PatternCast1ToSpace3_II_MP_Fn_2X2(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光影像轉成高度資料
	bool                       PatternCast1ToSpace3_II_MP_Fn_4X2(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光影像轉成高度資料
	bool                       PatternCast1ToSpace3_II_MP_Fn_4X4(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光影像轉成高度資料
	bool                       PatternCast1ToSpace3_II_MP_Fn_4X4GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光影像轉成高度資料
	bool                       PatternCast1ToSpace3_II_MP_Fn_4X5GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光影像轉成高度資料
	bool                       PatternCast1ToSpace3_II_MP_Fn_4X6GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光影像轉成高度資料
	//-----------------------------------------------------------------------------//
	bool                       PatternCast1ToSpace2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR &MaskPtr, SPACE_PTR &SpacePtr);//1投光2曝光影像轉成高度資料
	bool                       PatternCast1ToSpace2Exp3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光2曝光影像轉成高度資料
	bool                       PatternCast1ToSpace2Exp3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光2曝光影像轉成高度資料
	bool                       PatternCast1ToSpace2Exp3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光2曝光影像轉成高度資料
	bool                       PatternCast1ToSpace2Exp3_II_SP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光2曝光影像轉成高度資料
	bool                       PatternCast1ToSpace2Exp3_II_MP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光2曝光影像轉成高度資料
	bool                       PatternCast1ToSpace2Exp3_II_MP_Fn(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光2曝光影像轉成高度資料
	bool                       PatternCast1ToSpace2Exp3_II_MP_Fn_2X2(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光2曝光影像轉成高度資料
	bool                       PatternCast1ToSpace2Exp3_II_MP_Fn_4X2(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光2曝光影像轉成高度資料
	bool                       PatternCast1ToSpace2Exp3_II_MP_Fn_4X4(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光2曝光影像轉成高度資料
	bool                       PatternCast1ToSpace2Exp3_II_MP_Fn_4X4GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光2曝光影像轉成高度資料
	bool                       PatternCast1ToSpace2Exp3_II_MP_Fn_4X5GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光2曝光影像轉成高度資料	
	bool                       PatternCast1ToSpace2Exp3_II_MP_Fn_4X6GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//1投光2曝光影像轉成高度資料
	//-----------------------------------------------------------------------------//
	bool                       PatternCast2ToSpace(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR &MaskPtr, SPACE_PTR &SpacePtr);//2投光影像轉成高度資料
	bool                       PatternCast2ToSpace3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光影像轉成高度資料
	bool                       PatternCast2ToSpace3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光影像轉成高度資料
	bool                       PatternCast2ToSpace3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光影像轉成高度資料
	bool                       PatternCast2ToSpace3_II_SP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光影像轉成高度資料
	bool                       PatternCast2ToSpace3_II_MP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光影像轉成高度資料
	bool                       PatternCast2ToSpace3_II_MP_Fn(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光影像轉成高度資料
	bool                       PatternCast2ToSpace3_II_MP_Fn_2X2(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光影像轉成高度資料
	bool                       PatternCast2ToSpace3_II_MP_Fn_4X2(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光影像轉成高度資料
	bool                       PatternCast2ToSpace3_II_MP_Fn_4X4(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光影像轉成高度資料
	bool                       PatternCast2ToSpace3_II_MP_Fn_4X4GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光影像轉成高度資料
	bool                       PatternCast2ToSpace3_II_MP_Fn_4X5GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光影像轉成高度資料
	bool                       PatternCast2ToSpace3_II_MP_Fn_4X6GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光影像轉成高度資料
	//-----------------------------------------------------------------------------//
	bool                       PatternCast2ToSpace2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR &MaskPtr, SPACE_PTR &SpacePtr);//2投光2曝光影像轉成高度資料
	bool                       PatternCast2ToSpace2Exp3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光2曝光影像轉成高度資料
	bool                       PatternCast2ToSpace2Exp3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光2曝光影像轉成高度資料
	bool                       PatternCast2ToSpace2Exp3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光2曝光影像轉成高度資料
	bool                       PatternCast2ToSpace2Exp3_II_SP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光2曝光影像轉成高度資料
	bool                       PatternCast2ToSpace2Exp3_II_MP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光2曝光影像轉成高度資料
	bool                       PatternCast2ToSpace2Exp3_II_MP_Fn(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光2曝光影像轉成高度資料
	bool                       PatternCast2ToSpace2Exp3_II_MP_Fn_2X2(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光2曝光影像轉成高度資料
	bool                       PatternCast2ToSpace2Exp3_II_MP_Fn_4X2(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光2曝光影像轉成高度資料
	bool                       PatternCast2ToSpace2Exp3_II_MP_Fn_4X4(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光2曝光影像轉成高度資料
	bool                       PatternCast2ToSpace2Exp3_II_MP_Fn_4X4GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光2曝光影像轉成高度資料
	bool                       PatternCast2ToSpace2Exp3_II_MP_Fn_4X5GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光2曝光影像轉成高度資料	
	bool                       PatternCast2ToSpace2Exp3_II_MP_Fn_4X6GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//2投光2曝光影像轉成高度資料
	//-----------------------------------------------------------------------------//
	bool                       PatternCast3ToSpace(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR &MaskPtr, SPACE_PTR &SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace3_II_SP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace3_II_MP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace3_II_MP_Fn(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace3_II_MP_Fn_2X2(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace3_II_MP_Fn_4X2(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace3_II_MP_Fn_4X4(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace3_II_MP_Fn_4X4GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace3_II_MP_Fn_4X5GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace3_II_MP_Fn_4X6GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	//-----------------------------------------------------------------------------//
	bool                       PatternCast3ToSpace2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR &MaskPtr, SPACE_PTR &SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace2Exp3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace2Exp3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace2Exp3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace2Exp3_II_SP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace2Exp3_II_MP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace2Exp3_II_MP_Fn(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace2Exp3_II_MP_Fn_2X2(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace2Exp3_II_MP_Fn_4X2(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace2Exp3_II_MP_Fn_4X4(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace2Exp3_II_MP_Fn_4X4GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace2Exp3_II_MP_Fn_4X5GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料
	bool                       PatternCast3ToSpace2Exp3_II_MP_Fn_4X6GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//3投光影像轉成高度資料		
	//-----------------------------------------------------------------------------//
	bool                       PatternCast4ToSpace(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR &MaskPtr, SPACE_PTR &SpacePtr);//4投光影像轉成高度資料
	bool                       PatternCast4ToSpace3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料
	bool                       PatternCast4ToSpace3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料
	bool                       PatternCast4ToSpace3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料
	bool                       PatternCast4ToSpace3_II_SP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料-序列計算
	bool                       PatternCast4ToSpace3_II_MP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料-平行計算
	bool                       PatternCast4ToSpace3_II_MP_Fn(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, const TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料-平行計算
	bool                       PatternCast4ToSpace3_II_MP_Fn_2X2(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, const TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料-平行計算
	bool                       PatternCast4ToSpace3_II_MP_Fn_4X2(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, const TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料-平行計算
	bool                       PatternCast4ToSpace3_II_MP_Fn_4X4(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, const TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料-平行計算
	bool                       PatternCast4ToSpace3_II_MP_Fn_4X4GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, const TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4X4GC投光影像轉成高度資料-平行計算
	bool                       PatternCast4ToSpace3_II_MP_Fn_4X5GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, const TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4X5GC投光影像轉成高度資料-平行計算
	bool                       PatternCast4ToSpace3_II_MP_Fn_4X6GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, const TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4X6GC投光影像轉成高度資料-平行計算
	bool                       PatternCast4ToSpace3_II_MP_Fn_4_2Step(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, const TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料-平行計算
	bool                       PatternCast4ToSpace3_II_MP_Fn_4_3Step(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, const TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4+3投光影像轉成高度資料-平行計算
	//-----------------------------------------------------------------------------//
	bool                       PatternCast4ToSpace2Exp(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR &MaskPtr, SPACE_PTR &SpacePtr);//4投光影像轉成高度資料
	bool                       PatternCast4ToSpace2Exp3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料
	bool                       PatternCast4ToSpace2Exp3_I(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料
	bool                       PatternCast4ToSpace2Exp3_II(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, bool bOpenMP, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料
	bool                       PatternCast4ToSpace2Exp3_II_SP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料-序列計算
	bool                       PatternCast4ToSpace2Exp3_II_MP(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料-平行計算
	bool                       PatternCast4ToSpace2Exp3_II_MP_Fn(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, const TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料-平行計算
	bool                       PatternCast4ToSpace2Exp3_II_MP_Fn_2X2(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, const TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料-平行計算
	bool                       PatternCast4ToSpace2Exp3_II_MP_Fn_4X2(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, const TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料-平行計算
	bool                       PatternCast4ToSpace2Exp3_II_MP_Fn_4X4(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, const TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料-平行計算
	bool                       PatternCast4ToSpace2Exp3_II_MP_Fn_4X4GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, const TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料-平行計算
	bool                       PatternCast4ToSpace2Exp3_II_MP_Fn_4X5GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, const TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料-平行計算	
	bool                       PatternCast4ToSpace2Exp3_II_MP_Fn_4X6GC(int index, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const TCastParam &CastParam1, const TCastParam &CastParam2, const TCastParam &CastParam3, const TCastParam &CastParam4, TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr);//4投光影像轉成高度資料-平行計算
	//-----------------------------------------------------------------------------//
	//相位轉空間
	bool                       PhaseImageToSpaceImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR PhasePtr, const SPACE_PTR KPtr, MASK_PTR MaskPtr, SPACE_PTR &DstPtr);//將相位影像轉成空間相對位置
	bool                       PhaseImageToSpaceImage3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR PhasePtr, const SPACE_PTR KPtr, MASK_PTR MaskPtr, SPACE_PTR DstPtr);//將相位影像轉成空間相對位置	
	//-----------------------------------------------------------------------------//
	//相位轉空間-映射函式
	bool                       PhaseImageToSpaceImageByMapping(int CvtMode, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR PhasePtr, int SX, int SY, const double *HeightFactor0, const double *HeightFactor1, const double *HeightFactor2, MASK_PTR MaskPtr, SPACE_PTR &DstPtr);//將相位影像轉成空間相對位置
	bool                       PhaseImageToSpaceImageByMapping3(int CvtMode, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const PHASE_PTR PhasePtr, int SX, int SY, const double *HeightFactor0, const double *HeightFactor1, const double *HeightFactor2, MASK_PTR MaskPtr, SPACE_PTR DstPtr);//將相位影像轉成空間相對位置	
	//-----------------------------------------------------------------------------//	
	bool                       BuildSpaceDataByGroundEquation(const CJetGroundEquation &GroundEquation, IMAGE_SIZE DataW, IMAGE_SIZE DataH, IMAGE_SIZE DataStep, SPACE_PTR &SpacePtr);//依照面參數建立空間資料
	bool                       BuildSpaceDataByGroundEquation3(const CJetGroundEquation &GroundEquation, IMAGE_SIZE DataW, IMAGE_SIZE DataH, IMAGE_SIZE DataStep, SPACE_PTR SpacePtr);//依照面參數建立空間資料
	//-----------------------------------------------------------------------------//
	bool                       BuildDataModel(IMAGE_SIZE DataW, IMAGE_SIZE DataH, IMAGE_SIZE DataStep, SPACE_PTR DstPtr, MASK_PTR DstMaskPtr, const TDataModelParam &ModelParam);//建立資料模型
	bool                       BuildDataModel_MinErrPlane(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const MASK_PTR PlanePtr, const MASK_PTR ShapePtr, SPACE_PTR SpacePtr, MASK_PTR MaskPtr);//建立資料模型-最小誤差平面
	bool                       BuildDataModel_SurfaceFitting_Jun(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const RECT &RoiRect, SPACE_PTR SpacePtr, MASK_PTR MaskPtr);//建立資料模型-表面擬合
	//-----------------------------------------------------------------------------//	
	bool                       BuildSpaceData(IMAGE_SIZE DataW, IMAGE_SIZE DataH, IMAGE_SIZE DataStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR Mask2DPtr, int nOpenMPCnt,  TNoiseFilterParam &NoiseParam, SPACE_PTR &DstPtr, MASK_PTR &DstMaskPtr, const IMAGE_PTR GuidedImagePtr = NULL);//建立空間資料
	bool                       BuildSpaceData3(IMAGE_SIZE DataW, IMAGE_SIZE DataH, IMAGE_SIZE DataStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR Mask2DPtr, int nOpenMPCnt, TNoiseFilterParam &NoiseParam, SPACE_PTR DstPtr, MASK_PTR DstMaskPtr, const IMAGE_PTR GuidedImagePtr = NULL);//建立空間資料
	bool                       BuildSpaceData3_I(IMAGE_SIZE DataW, IMAGE_SIZE DataH, IMAGE_SIZE DataStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR Mask2DPtr, int nOpenMPCnt, TNoiseFilterParam &NoiseParam, SPACE_PTR DstPtr, MASK_PTR DstMaskPtr, const IMAGE_PTR GuidedImagePtr = NULL);//建立空間資料-I
	bool                       BuildSpaceData3_II(IMAGE_SIZE DataW, IMAGE_SIZE DataH, IMAGE_SIZE DataStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR Mask2DPtr, int nOpenMPCnt, TNoiseFilterParam &NoiseParam, SPACE_PTR DstPtr, MASK_PTR DstMaskPtr, const IMAGE_PTR GuidedImagePtr = NULL);//建立空間資料-II
	//-----------------------------------------------------------------------------//
	//重建雜訊資料
	bool                       ReContructVoidData(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, SPACE_PTR &DstPtr);//重建雜訊資料
	bool                       ReContructVoidData3(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, SPACE_PTR DstPtr);//重建雜訊資料
	//-----------------------------------------------------------------------------//
	bool                       CalcSpaceLevelPlane(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const int *SpacePtr, const MASK_PTR MaskPtr, int *&DstPtr);//計算空間切平面
	bool                       CalcSpaceLevelPlane3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const int *SpacePtr, const MASK_PTR MaskPtr, int *DstPtr);//計算空間切平面
	bool                       CalcSpaceLevelPlane(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *SpacePtr, const MASK_PTR MaskPtr, float *&DstPtr);//計算空間切平面
	bool                       CalcSpaceLevelPlane3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *SpacePtr, const MASK_PTR MaskPtr, float *DstPtr);//計算空間切平面
	bool                       CalcSpaceLevelPlane(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TBasePlaneParam &CalcParam, const MASK_PTR Mask2DPtr, CJetGroundEquation &GroudEquation);//計算空間切平面
	bool                       CalcSpaceLevelPlane_Panel(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TBasePlaneParam &CalcParam, const MASK_PTR Mask2DPtr, float &PlaneL, float &PlaneH, std::vector<TRECT6I> &CalcRectList);//計算空間切平面-整板
	bool                       CalcSpaceLevelPlane_Average(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TBasePlaneParam &CalcParam, const MASK_PTR Mask2DPtr, float &PlaneL, float &PlaneH);//計算空間切平面-平均值
	bool                       CalcSpaceLevelPlane_Corner(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TBasePlaneParam &CalcParam, const MASK_PTR Mask2DPtr, float &PlaneL, float &PlaneH);//計算空間切平面-端點
	bool                       CalcSpaceLevelPlane_IsoData(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TBasePlaneParam &CalcParam, const MASK_PTR Mask2DPtr, float &PlaneL, float &PlaneH);//計算空間切平面-IsoData
	bool                       CalcSpaceLevelPlane_OTSU(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TBasePlaneParam &CalcParam, const MASK_PTR Mask2DPtr, float &PlaneL, float &PlaneH);//計算空間切平面-OTSU
	bool                       CalcSpaceLevelPlane_CornerOnly(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TBasePlaneParam &CalcParam, const MASK_PTR Mask2DPtr, float &PlaneL, float &PlaneH, std::vector<TRECT6I> &CalcRectList);//計算空間切平面-只有端點
	bool                       CalcSpaceLevelPlane_Surround(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TBasePlaneParam &CalcParam, const MASK_PTR Mask2DPtr, float &PlaneL, float &PlaneH, std::vector<TRECT6I> &CalcRectList);//計算空間切平面-周圍四邊
	bool                       CalcSpaceLevelPlane_AutoLower(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TBasePlaneParam &CalcParam, const MASK_PTR Mask2DPtr, bool bLocakRange, float &PlaneL, float &PlaneH, std::vector<TRECT6I> &CalcRectList);//計算空間切平面-較低的
	bool                       CalcSpaceLevelPlane_AutoSelectRgn(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TBasePlaneParam &CalcParam, const MASK_PTR Mask2DPtr, bool bLocakRange, int RoiSizeCx, int RoiSizeCy, int SpaceW2, int SpaceH2, DWORD &NewUseSideMode);//計算空間切平面-自動選取四區域特殊高度
	//-----------------------------------------------------------------------------//
	//空間資料扣除基準面
	bool                       ExecSpaceImageLeveling3(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, double nX, double nY, double nZ, double OffsetZ, double OverHigh, double OverLow, SPACE_PTR DstPtr);//將空間資料進行切平面轉換
	bool                       ExecSpaceImageLeveling(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const CJetGroundEquation &GroundEquation, double OffsetZ, double OverHigh, double OverLow, SPACE_PTR &DstPtr);//將空間資料進行切平面轉換
	bool                       ExecSpaceImageLeveling3(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const CJetGroundEquation &GroundEquation, double OffsetZ, double OverHigh, double OverLow, SPACE_PTR DstPtr);//將空間資料進行切平面轉換
	//-----------------------------------------------------------------------------//
	//修整空間資料
	bool                       RefineSpaceImage(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, int nOpenMPCnt, const TImageFilterParam &FilterParam, SPACE_PTR &DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-去雜訊等等
	bool                       RefineSpaceImage3(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, int nOpenMPCnt, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-去雜訊等等
	bool                       RefineSpaceImage3_Char(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-去雜訊等等
	bool                       RefineSpaceImage3_Short(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-去雜訊等等
	bool                       RefineSpaceImage3_Real(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, int nOpenMPCnt, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-去雜訊等等
	bool                       RefineSpaceImage3_Real_SP_ShiftPitch(int Pitch, int Shift, int Tolerance, bool SkipValidPixel, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, SPACE_PTR DstPtr);//回填步長資料
	bool                       RefineSpaceImage3_Real_SP(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-去雜訊等等
	bool                       RefineSpaceImage3_Real_SP_3Level(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-分等濾波
	bool                       RefineSpaceImage3_Real_SP_Median(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-中值濾波	
	bool                       RefineSpaceImage3_Real_SP_Median_01(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-中值濾波-一般
	bool                       RefineSpaceImage3_Real_SP_Median_02(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-中值濾波-減少Kernel時間
	bool                       RefineSpaceImage3_Real_SP_Median2(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-中值濾波2
	bool                       RefineSpaceImage3_Real_SP_Smooth(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-平滑濾波
	bool                       RefineSpaceImage3_Real_SP_PyramidMedian(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-金字塔中值濾波, Joe, 20191024
	bool                       RefineSpaceImage3_Real_SP_ContentAware(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-Content Aware濾波, Joe, 20191125
	bool                       RefineSpaceImage3_Real_MP_ShiftPitch(int Pitch, int Shift, int Tolerance, bool SkipValidPixel, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, int nOpenMP, SPACE_PTR DstPtr);//回填步長資料
	bool                       RefineSpaceImage3_Real_MP(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, int nOpenMP, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-去雜訊等等	
	bool                       RefineSpaceImage3_Real_MP_Median(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, int nOpenMP, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-中值過濾
	bool                       RefineSpaceImage3_Real_MP_Fn(int Index, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-去雜訊等等
	bool                       RefineSpaceImage3_Real_MP_Fn_3Level(int Index, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-分等過濾
	bool                       RefineSpaceImage3_Real_MP_Fn_Median(int Index, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-中值過濾
	bool                       RefineSpaceImage3_Real_MP_Fn_Median_Y(int YIndex, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-中值過濾	
	bool                       RefineSpaceImage3_Real_MP_Fn_Median2(int Index, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-中值過濾2
	bool                       RefineSpaceImage3_Real_MP_Fn_Smooth(int Index, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-平滑過濾	
	bool                       RefineSpaceImage3_Real_MP_Fn_PyramidMedian(int Index, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-金字塔中值過濾, Joe, 20191024
	bool                       RefineSpaceImage3_Real_MP_Fn_ContentAware(int Index, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr);//將空間資料修整-Content Aware濾波	
	//-----------------------------------------------------------------------------//
	bool                       RefineSpaceImage3_Real_SP_CTMF_AVX2(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr, const MASK_PTR GuidedImagePtr = NULL);//將空間資料修整-Constant Time Median Filter, Joe, 20200331
	bool                       RefineSpaceImage3_Real_SP_CTMF_Kernel(IMAGE_SIZE StripeSize, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, HT_DATA* SrcPtr, float* DstPtr, size_t Radius, bool PadLeft, bool PadRight);//將空間資料修整-Constant Time Median Filter, Joe, 20200331	
	void					   Hist_Add(const HT_DATA x[HIST_SIZE], HT_DATA y[HIST_SIZE]);
	void					   Hist_Sub(const HT_DATA x[HIST_SIZE], HT_DATA y[HIST_SIZE]);
	void					   Hist_MulAdd(const HT_DATA a, const HT_DATA x[HIST_SIZE], HT_DATA y[HIST_SIZE]);
	template<typename T> void  Hist_RefineRange(const T* src, HT_DATA* data, size_t buffersize);
	//-----------------------------------------------------------------------------//
	bool                       RefineSpaceImage3_Real_SP_Smooth_AVX(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr);//AVX Smooth Filter, only size :3, 5, 7	
	bool                       RefineSpaceImage3_Real_SP_Median_AVX2(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr);//AVX Median Filter, only size :3, 5, 7
	bool                       RefineSpaceImage3_Real_SP_Median_AVX_NoMask(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr);//AVX Median Filter, only size :3, 5, 7
	//-----------------------------------------------------------------------------//
	bool                       RefineSpaceImage3_Real_MP_Smooth_AVX(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, int OpenMPCount, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr);//AVX Smooth Filter, only size :3, 5, 7	
	bool                       RefineSpaceImage3_Real_MP_Median_AVX2(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, int OpenMPCount, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr);//AVX Median Filter, only size :3, 5, 7
	bool                       RefineSpaceImage3_Real_MP_CTMF_AVX2(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr, const MASK_PTR MaskPtr, const MASK_PTR CalcMaskPtr, int OpenMPCount, const TImageFilterParam &FilterParam, SPACE_PTR DstPtr);//將空間資料修整-Constant Time Median Filter, Joe, 20200331
	//-----------------------------------------------------------------------------//
	template<typename T>
	inline void                m_Op_AVX2(T& x, T& y);//x = min, y = max
	template<typename T>
	inline void                m_Op(T& x, T& y);
	inline void                m_Sum_AVX(__m256& x, __m256& y);//x = x + y
	//-----------------------------------------------------------------------------//
	int                        CheckSupportAVX2(int* phwavx);
	UINT32                     GetCpuIdField(CPUIDFIELD cpuf);// 根據CPUIDFIELD獲取CPUID字段.
	UINT32				       GetCpuIdField_Buf(const INT32 dwBuf[4], CPUIDFIELD cpuf);// 根據CPUIDFIELD從緩衝區中獲取字段.
	//-----------------------------------------------------------------------------//
	bool                       GetGuidedImage(const int ImgW, const int ImgH, const int ImgStep, const int BitCount, const IMAGE_PTR ImagePtr, IMAGE_PTR &DstPtr);
	//合併相位
	bool                       Merge2ExpPhaseImage(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr1, MASK_PTR MaskPtr1, const PHASE_PTR PhasePtr2, MASK_PTR MaskPtr2, bool bOpenMP, PHASE_PTR &PhasePtr, MASK_PTR &MaskPtr);//合併兩個曝光的相位
	bool                       Merge2ExpPhaseImage3(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr1, MASK_PTR MaskPtr1, const PHASE_PTR PhasePtr2, MASK_PTR MaskPtr2, bool bOpenMP, PHASE_PTR PhasePtr, MASK_PTR MaskPtr);//合併兩個曝光的相位
	//-----------------------------------------------------------------------------//
	//合併空間資料
	bool                       Merge2SpaceImage(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr1, MASK_PTR MaskPtr1, const SPACE_PTR SpacePtr2, MASK_PTR MaskPtr2, bool bOpenMP, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr);//將2個空間合併同一個空間 
	bool                       Merge2SpaceImage3(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr1, MASK_PTR MaskPtr1, const SPACE_PTR SpacePtr2, MASK_PTR MaskPtr2, bool bOpenMP, SPACE_PTR SpacePtr, MASK_PTR MaskPtr);//將2個空間合併同一個空間 

	bool                       Merge3SpaceImage(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr1, MASK_PTR MaskPtr1, const SPACE_PTR SpacePtr2, MASK_PTR MaskPtr2, const SPACE_PTR SpacePtr3, MASK_PTR MaskPtr3, bool bOpenMP, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr);//將3個空間合併同一個空間 
	bool                       Merge3SpaceImage3(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr1, MASK_PTR MaskPtr1, const SPACE_PTR SpacePtr2, MASK_PTR MaskPtr2, const SPACE_PTR SpacePtr3, MASK_PTR MaskPtr3, bool bOpenMP, SPACE_PTR SpacePtr, MASK_PTR MaskPtr);//將3個空間合併同一個空間 

	bool                       Merge4SpaceImage(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr1, MASK_PTR MaskPtr1, const SPACE_PTR SpacePtr2, MASK_PTR MaskPtr2, const SPACE_PTR SpacePtr3, MASK_PTR MaskPtr3, const SPACE_PTR SpacePtr4, MASK_PTR MaskPtr4, bool bOpenMP, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr);//將4個空間合併同一個空間 
	bool                       Merge4SpaceImage3(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr1, MASK_PTR MaskPtr1, const SPACE_PTR SpacePtr2, MASK_PTR MaskPtr2, const SPACE_PTR SpacePtr3, MASK_PTR MaskPtr3, const SPACE_PTR SpacePtr4, MASK_PTR MaskPtr4, bool bOpenMP, SPACE_PTR SpacePtr, MASK_PTR MaskPtr);//將4個空間合併同一個空間 
	bool                       Merge4SpaceImageRecheck(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr1, MASK_PTR MaskPtr1, const SPACE_PTR SpacePtr2, MASK_PTR MaskPtr2, const SPACE_PTR SpacePtr3, MASK_PTR MaskPtr3, const SPACE_PTR SpacePtr4, MASK_PTR MaskPtr4, MASK_DATA ChkMask, MASK_PTR RefMaskPtr, bool bForceSet, bool bOpenMP, SPACE_PTR SpacePtr, MASK_PTR MaskPtr);//將4個空間合併同一個空間 
	bool                       Merge4SpaceImageRecheck_SP(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr1, MASK_PTR MaskPtr1, const SPACE_PTR SpacePtr2, MASK_PTR MaskPtr2, const SPACE_PTR SpacePtr3, MASK_PTR MaskPtr3, const SPACE_PTR SpacePtr4, MASK_PTR MaskPtr4, MASK_DATA ChkMask, MASK_PTR RefMaskPtr, bool bForceSet, SPACE_PTR SpacePtr, MASK_PTR MaskPtr);//將4個空間合併同一個空間 
	bool                       Merge4SpaceImageRecheck_MP(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr1, MASK_PTR MaskPtr1, const SPACE_PTR SpacePtr2, MASK_PTR MaskPtr2, const SPACE_PTR SpacePtr3, MASK_PTR MaskPtr3, const SPACE_PTR SpacePtr4, MASK_PTR MaskPtr4, MASK_DATA ChkMask, MASK_PTR RefMaskPtr, bool bForceSet, SPACE_PTR SpacePtr, MASK_PTR MaskPtr);//將4個空間合併同一個空間 
	bool                       Merge4SpaceImageRecheck_MP_Fn(int index, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr1, MASK_PTR MaskPtr1, const SPACE_PTR SpacePtr2, MASK_PTR MaskPtr2, const SPACE_PTR SpacePtr3, MASK_PTR MaskPtr3, const SPACE_PTR SpacePtr4, MASK_PTR MaskPtr4, MASK_DATA ChkMask, MASK_PTR RefMaskPtr, bool bForceSet, SPACE_PTR SpacePtr, MASK_PTR MaskPtr);//將4個空間合併同一個空間 
	//-----------------------------------------------------------------------------//	
	bool                       DeductBasePhase_Public(PHASE_DATA BasePhase, int &Phase);//扣除相平面
	bool                       CalcCombinePhasePeriodRatio_Public(int Mode, double P1, double P2, double &Ratio);//計算合併比例
	//-----------------------------------------------------------------------------//	
#ifndef OPENCV_DISABLE
	//-----------------------------------------------------------------------------//
	bool                       InitOpenCV();
	//-----------------------------------------------------------------------------//
	bool                       GetOpenCVDataTypeText(int nType, char Text[]);//取得 OpenCV-資料型態文字
	//-----------------------------------------------------------------------------//
	bool                       AdjustSaveLoadImageReverse(bool Reverse);//調整存檔開檔的影像反向變數, 為了與CDib吻合
	//-----------------------------------------------------------------------------//
	bool                       ReturnOpenCVDisableException();//回傳關閉OpenCV的錯誤
	bool                       CheckMatIsValid(const cv::Mat &M);//確認cv::Mat是有效的指標		
	bool                       CatchOpenCVException(const cv::Exception &e);//取得OpenCV的例外物件	
	bool                       CheckMatIdentity(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const void *Ptr);//確認Mat相同
	//-----------------------------------------------------------------------------//		
	void                       RectToCVRect(const RECT &Rect, cv::Rect &cvRect);
	void                       CvPointToCVRect(const CvPoint &P1, const CvPoint &P2, cv::Rect &cvRect);
	//-----------------------------------------------------------------------------//	
	bool                       WriteMatData(LPCTSTR filename, cv::Mat &M);
	//-----------------------------------------------------------------------------//		
	cv::Mat                    CreateMatImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, int Type, int Channel, const char *fnName);//創建OpenCV Image指標	
	cv::Mat                    CreateMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const int *Ptr);	
	cv::Mat                    CreateMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const char *Ptr);	
	cv::Mat                    CreateMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const short *Ptr);	
	cv::Mat                    CreateMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const float *Ptr);	
	cv::Mat                    CreateMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const double *Ptr);	
	cv::Mat                    CreateMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *Ptr);	
	cv::Mat                    CreateMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned short *Ptr);	
	cv::Mat                    CreateMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB);
	//-----------------------------------------------------------------------------//			
	bool                       CloneMatData(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, int *Ptr, bool Reverse=false);
	bool                       CloneMatData(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, char *Ptr, bool Reverse=false);
	bool                       CloneMatData(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, short *Ptr, bool Reverse=false);
	bool                       CloneMatData(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, float *Ptr, bool Reverse=false);
	bool                       CloneMatData(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, double *Ptr, bool Reverse=false);
	bool                       CloneMatData(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, unsigned char *Ptr, bool Reverse=false);
	bool                       CloneMatData(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, unsigned short *Ptr, bool Reverse=false);
	bool                       CloneMatData(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pR, unsigned char *pG, unsigned char *pB, bool Reverse=false);
	//-----------------------------------------------------------------------------//	
	bool                       DestroyCVMat(int CreateMode, CvMat *&cvMat);//刪除OpenCV Mat指標
	CvMat*                     CreateCVMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, int Type, const char *fnName);//創建OpenCV Mat指標	
	CvMat*                     CreateCVMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, int Type, int Channel, const char *fnName);//創建OpenCV Mat指標	
	CvMat*                     CreateCVMatHeader(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, int Type, int Channel);//創建OpenCV Mat標頭指標	
	//-----------------------------------------------------------------------------//
	bool                       DestroyCVImage(int CreateMode, IplImage *&cvImage);//刪除Open CV影像
	IplImage*                  CreateCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, int Type, int Channel, const char *fnName);//創建OpenCV Image指標	
	IplImage*                  CreateCVImageHeader(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, int Type, int Channel);//創建OpenCV Image標頭指標	
	IplImage*                  ImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int &CreateMode);//灰階影像轉成cv影像	
	//-----------------------------------------------------------------------------//
	CvMat*                     ImageToCVMat(const TIMAGE &Image, bool Reverse, const char *fnName);
	IplImage*                  ImageToCVImage(const TIMAGE &Image, bool Reverse, const char *fnName);
	//-----------------------------------------------------------------------------//
	IplImage*                  GrayImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, bool Reverse, int &CreateMode);//灰階影像轉成cv影像
	IplImage*                  ColorImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, bool Reverse, int &CreateMode);//彩色影像轉成cv影像
	IplImage*                  RGBImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, bool Reverse);//RGB影像轉成cv影像

	IplImage*                  ShortGrayImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const short *pImage, bool Reverse, int &CreateMode);//灰階影像轉成cv影像
	IplImage*                  ShortGrayImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, bool Reverse, int &CreateMode);//灰階影像轉成cv影像
	IplImage*                  ShortColorImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, bool Reverse);//彩色影像轉成cv影像
	IplImage*                  ShortRGBImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pR, const unsigned short *pG, const unsigned short *pB, bool Reverse);//RGB影像轉成cv影像	

	IplImage*                  FloatGrayImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pImage, bool Reverse, int &CreateMode);//浮點數影像轉成cv影像
	//-----------------------------------------------------------------------------//
	bool                       LoadCVImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, unsigned char *&pImage, int Align, bool Reverse);//讀取V支援圖檔
	bool                       LoadCVGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage, int Align, bool Reverse);//讀取V支援灰階圖檔
	bool                       LoadCVColorImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage, int Align, bool Reverse);//讀取V支援彩色圖檔

	bool                       SaveCVImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, bool Reverse, const int* Params=NULL);//儲存PNG圖檔
	bool                       SaveCVGrayImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, bool Reverse, const int* Params=NULL);//儲存灰階圖檔
	bool                       SaveCVColorImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, bool Reverse, const int* Params=NULL);//儲存彩色圖檔	
	bool                       SaveCVRGBImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, bool Reverse);//儲存RGB彩色圖檔	
	//-----------------------------------------------------------------------------//	
	bool                       SaveMatImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, bool Reverse, const std::vector<int> &Params=std::vector<int>());//儲存Mat圖檔
	bool                       LoadMatImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, unsigned char *&pImage, int Align, bool Reverse);//讀取Mat支援圖檔
	bool                       LoadMatGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage, int Align, bool Reverse);//讀取Mat支援灰階圖檔
	bool                       LoadMatColorImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage, int Align, bool Reverse);//讀取Mat支援彩色圖檔
	//-----------------------------------------------------------------------------//
	int                        GetCVMorphMode(int MorphMode);
	int                        GetCVMorphShapeMode(int ShapeMode);
	int                        GetCVBayerModeGray(BAYER_PATTERN_MODE Bayer);//Bayer to Gray
	int                        GetCVBayerModeBGR(BAYER_PATTERN_MODE Bayer);//Bayer to BGR
	//-----------------------------------------------------------------------------//
	bool                       DrawBoxImage_Rect(cv::Mat &Img, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect);
	bool                       DrawBoxImage_Rect(IplImage *pImg, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect);	
	//-----------------------------------------------------------------------------//
	bool                       DrawBoxImage_RroundRect(cv::Mat &Img, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect, int Radius);
	bool                       DrawBoxImage_RroundRect(IplImage *pImg, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect, int Radius);	
	//-----------------------------------------------------------------------------//
	bool                       DrawBoxImage_Ellipse(cv::Mat &Img, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect);
	bool                       DrawBoxImage_Ellipse(IplImage *pImg, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect);
	//-----------------------------------------------------------------------------//
	bool                       DrawBoxImage_Capsule(cv::Mat &Img, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect);
	bool                       DrawBoxImage_Capsule(IplImage *pImg, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect);		
	//-----------------------------------------------------------------------------//
	bool                       DrawBoxImage_Bullet(cv::Mat &Img, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect);
	bool                       DrawBoxImage_Bullet(IplImage *pImg, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect);			
	//-----------------------------------------------------------------------------//
	bool                       DrawBoxImage_HalfRroundRect(cv::Mat &Img, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect, int Radius);
	bool                       DrawBoxImage_HalfRroundRect(IplImage *pImg, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect, int Radius);	
	//-----------------------------------------------------------------------------//
	bool                       DrawBoxImage_TShapeRect(cv::Mat &Img, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect, int wRatio, int hRatio);
	bool                       DrawBoxImage_TShapeRect(IplImage *pImg, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect, int wRatio, int hRatio);	
	//-----------------------------------------------------------------------------//
	bool                       DrawBoxImage_RotatedRect(cv::Mat &Img, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, double Angle, const RECT &MaskRect);
	bool                       DrawBoxImage_RotatedRect(IplImage *pImg, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, double Angle, const RECT &MaskRect);	
	//-----------------------------------------------------------------------------//
	bool                       ExecFindAbnormal(IMAGE_SIZE SrcImageW, IMAGE_SIZE SrcImageH, IMAGE_SIZE SrcImageStep, IMAGE_SIZE SrcBitCount, const unsigned char *SrcPtr, IMAGE_SIZE RefImageW, IMAGE_SIZE RefImageH, IMAGE_SIZE RefImageStep, IMAGE_SIZE RefBitCount, const unsigned char *RefPtr, POINT ResStart, const int ColorExpand, size_t WndSizeW, size_t WndSizeH, const int MinEdge, unsigned char *DstPtr);
	bool                       TrainColorCalibrationCoeff(const cv::Mat& Src, const cv::Mat& Ref, const int sampleStep, cv::Mat &Coeff);
	void                       BuildPolynomial13Matrices(const cv::Mat & Src, const cv::Mat & Ref, cv::Mat& Coeff, int sampleStep);
	bool                       ApplyColorCalibrationCoeff(const cv::Mat& Src, const cv::Mat& Coeff, cv::Mat& Dst);
	bool                       ExecSearchContours(const cv::Mat& SrcMask, std::vector<std::vector<cv::Point>>& contours, std::vector<cv::Rect>& boundingRects, bool bToEdge = false, bool bBoundary = false, bool bRectangle = true, double nMinPerimeter = 0, double nMaxPerimeter = 1e6);
	bool                       EdgeWithDirectionConsistency(const cv::Mat& Src, cv::Mat& Dst, int bilateral_d = 9, double sigmaColor = 75, double sigmaSpace = 75, int ksize = 5, double consistencyThresh = 0.6, bool bEnhance = false);
	bool                       CheckIsShiftError(const cv::Mat & Src, const cv::Mat & Ref, const cv::Point AnchorPoint, const cv::Rect & rect, std::vector<cv::Point> contour, size_t WndSizeW, size_t WndSizeH, double thresholdColor);
	bool                       CheckIsShiftError_vEdge(const cv::Mat & Src, const cv::Mat & RefEdge, const cv::Point AnchorPoint, const cv::Rect & rect, std::vector<cv::Point> contour, size_t WndSizeW, size_t WndSizeH, double ratio);
	//-----------------------------------------------------------------------------//
	bool                       MultiFocusImageFusionFn(const std::vector<cv::Mat> srcs, cv::Mat& dst);//MFIF//20260304-Joe
	bool                       MultiFocusImageFusion(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const std::vector<IMAGE_PTR> &ImagePtrList, IMAGE_PTR &DstPtr);//MFIF//20260304-Joe
	bool                       MultiFocusImageFusion3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const std::vector<IMAGE_PTR> &ImagePtrList, IMAGE_PTR DstPtr);//MFIF//20260304-Joe
	//-----------------------------------------------------------------------------//
	bool                       ExecAnglePCA_Binary(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR &pImage, double& dSlope, double& dAngle, TPOINT2D& cvdCenterPt);
	bool                       ExecGetPCAAxisEndPoint_Binary(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR &pImage, const TPOINT2D& cvdCenterPt, double dAngle, TPOINT2D& cvdAxisPt1, TPOINT2D& cvdAxisPt2);

#endif//OPENCV_DISABLE
};
//-------------------------------------------------------------------------------------//
extern CImageAPI ImageAPI;
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_IMAGEAPI_H__394DF268_82DA_43FF_86D1_D0B377EB55CA__INCLUDED_)
