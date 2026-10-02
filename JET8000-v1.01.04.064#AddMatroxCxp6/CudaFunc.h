//CudaFunc.h
//------------------------------------------------------------------------------//
#ifndef __CUDA_FUNC__H_
#define __CUDA_FUNC__H_
//------------------------------------------------------------------------------//

//#ifdef  CUDA_USE 	//chia 1050127	

//#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "CudaKernel.h"
//#include "BlockSet.h"
//------------------------------------------------------------------------------//
#define  CUDA_MEM_NULL                     0
#define  CUDA_MEM_UNLOCK                   1
#define  CUDA_MEM_LOCK                     2
//------------------------------------------------------------------------------//
class CCudaFunc
{
public:
	CCudaFunc();
	~CCudaFunc();

	const char *GetErrorString();
	bool    GetIsCudaDeviceUse() const;

	//解相位
//	bool CudaSolvePhase_FullImage(const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pImage3, const unsigned char *pImage4, unsigned char *pMergeImage, short *pPhaseData, unsigned char *pMaskMap, const int W, const int H, const int ThreadNO);
	
	//解相位 走停
	bool CudaSetBasePhaseData(short *pBasePhasePlane, const int DataSize, const int LightSide, const int FrequenceMode);
	bool CudaSetFFCBaseData(float *pGain, unsigned char *pBase, const int DataSize, const int LightSide);
	bool CudaSolvePhase_Three(const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pImage3, unsigned char *pMergeImage, short *pPhaseData, unsigned char *pMaskMap, const int W, const int H, int ImageSmoothMaskSize, bool IsUseFFC, bool IsUseBasePhase, const int LightSide, const int FrequenceMode, const int ThreadNO);
	bool CudaSolvePhase_Four(const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pImage3, const unsigned char *pImage4, unsigned char *pMergeImage, short *pPhaseData, unsigned char *pMaskMap, const int W, const int H, int ImageSmoothMaskSize, bool IsUseFFC, bool IsUseBasePhase, const int LightSide, const int FrequenceMode, const int ThreadNO);
	bool CudaSolvePhase_Five(const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pImage3, const unsigned char *pImage4, const unsigned char *pImage5, unsigned char *pMergeImage, short *pPhaseData, unsigned char *pMaskMap, const int W, const int H, int ImageSmoothMaskSize, bool IsUseFFC, bool IsUseBasePhase, const int LightSide, const int FrequenceMode, const int ThreadNO);
	bool CudaPhaseSmooth(short *pSrcPhase, short *pDestPhase, unsigned char *pMaskMap, const int W, const int H, const int MaskSize, const int ThreadNO);

	bool CudaSolvePhase221Frames2Period(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, double PA, double PB, int ExpTimeA, int ExpTimeB, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap);//2+2步雙相位
	bool CudaSolvePhase221Frames2Period2Exp(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, double PA, double PB, int ExpTimeA, int ExpTimeB, int ExpTimeC, int ExpTimeD, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap);//2+2步雙相位2曝光

	//Kai 20161017
	bool CudaSolvePhase42Frames2Period(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrB1, const unsigned char *PtrB2, double PA, double PB, int ExpTimeA, int ExpTimeB, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap);//4+2步雙相位
	bool CudaSolvePhase42Frames2Period2Exp(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrD1, const unsigned char *PtrD2, double PA, double PB, int ExpTimeA, int ExpTimeB, int ExpTimeC, int ExpTimeD, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap);//4+2步雙相位2曝光

	//Kai 20210226
	bool CudaSolvePhase44GCFrames2Period(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PerB, int ExpTimeB, float Gamma, const short *BasePhasePtr, TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap);//4+4GC張灰階影像解相位

	//Kai 20210217	
	bool CudaSolvePhase45GCFrames2Period(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, double PerB, int ExpTimeB, float Gamma, const short *BasePhasePtr, TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap);//4+5GC張灰階影像解相位

	//Kai 20210225
	bool CudaSolvePhase46GCFrames2Period(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, const unsigned char *PtrB6, double PerB, int ExpTimeB, float Gamma, const short *BasePhasePtr, TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap);//4+6GC張灰階影像解相位

	bool CudaSolvePhase4Frames(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pImage3, const unsigned char *pImage4, int PatternID, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap);//4步相位
	bool CudaSolvePhase4Frames2Period(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PA, double PB, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap);//4步雙相位	
	bool CudaSolvePhase4Frames2Period_Separate(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PA, double PB, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap);//4步雙相位-個別算
	bool CudaSolvePhase4Frames2Period_Combine(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PA, double PB, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap);//4步雙相位-一起算
	bool CudaPhaseToSpace(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const short *PhasePtr, const float* KPtr, float *DstPtr );//相位乘上K值變成空間數值	
	bool CudaSolvePhase4Frames2Period2Exp(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, double PA, double PB, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap);//4步雙相位2曝光
	bool CudaSolvePhase4Frames2Period2Exp_Separate(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, double PA, double PB, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap);//4步雙相位2曝光
	bool CudaSolvePhase4Frames2Period2Exp_Combine(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, double PA, double PB, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap);//4步雙相位2曝光

	bool CudaSolveSpace4Frames2Period1Cast(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber=192, int BlockNumber=961);//1投光計算出高度值
	bool CudaSolveSpace4Frames2Period1CastFn(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber=192, int BlockNumber=961);//1投光計算出高度值
	bool CudaSolveSpace4Frames2Period2Cast(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber=192, int BlockNumber=961);//2投光計算出高度值
	bool CudaSolveSpace4Frames2Period2CastFn(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber=192, int BlockNumber=961);//2投光計算出高度值
	bool CudaSolveSpace4Frames2Period3Cast(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber=192, int BlockNumber=961);//3投光計算出高度值	
	bool CudaSolveSpace4Frames2Period3CastFn(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber=192, int BlockNumber=961);//3投光計算出高度值	
	bool CudaSolveSpace4Frames2Period4Cast(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber=192, int BlockNumber=961);//4投光計算出高度值
	bool CudaSolveSpace4Frames2Period4CastFn(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber=192, int BlockNumber=961);//4投光計算出高度值

	bool CudaSolveSpace4Frames2Period1Cast2Exp(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber=192, int BlockNumber=961);//1投光2曝光計算出高度值
	bool CudaSolveSpace4Frames2Period1Cast2ExpFn(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber=192, int BlockNumber=961);//1投光2曝光計算出高度值
	bool CudaSolveSpace4Frames2Period2Cast2Exp(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber=192, int BlockNumber=961);//2投光2曝光計算出高度值
	bool CudaSolveSpace4Frames2Period2Cast2ExpFn(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber=192, int BlockNumber=961);//2投光2曝光計算出高度值
	bool CudaSolveSpace4Frames2Period3Cast2Exp(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber=192, int BlockNumber=961);//3投光2曝光計算出高度值	
	bool CudaSolveSpace4Frames2Period3Cast2ExpFn(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber=192, int BlockNumber=961);//3投光2曝光計算出高度值	
	bool CudaSolveSpace4Frames2Period4Cast2Exp(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber=192, int BlockNumber=961);//4投光2曝光計算出高度值
	bool CudaSolveSpace4Frames2Period4Cast2ExpFn(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber=192, int BlockNumber=961);//4投光2曝光計算出高度值

	//chia 1041130
	/*
	bool CudaSolveMultiPhase_Four(PBlockInfoST pBlockInfo, const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pImage3, const unsigned char *pImage4
									, const unsigned char *pImage1_F2, const unsigned char *pImage2_F2, const unsigned char *pImage3_F2, const unsigned char *pImage4_F2
									, int ImageW, int ImageH, bool IsColor, unsigned char *&p2DImage, short *&pPhaseData, unsigned char *&pMask, int ThreadNO);
									*/

	//RecursiveGaussian
	bool CudaRecursiveGaussian(unsigned char *pSrc, unsigned char *pDest, const int W, const int H, float sigma);//Do Cuda
	//ImageConvolution
	bool CudaImageAverageSmooth(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *pSrc,unsigned char *pDest, int KernelSize);
	//2DFilter Gaussian
	bool CudaDataGaussianSmooth(float *pSrc,float *pDest,int W,int H,int DataSmoothMask);
	//ContentAware
	bool CudaContentAwareFilter(int SpaceW, int SpaceH, int SpaceStep, const float *pSrc, const unsigned char *pMask, const unsigned char *GuidedImagePtr, int KerSize, int UseSize, bool FilterSearchOn, int FilterAlphaF, int FilterAlphaS, int FilterAlphaM, int FilterAlphaI, int FilterThresdhold_Outlier, float *pDst, int ThreadNumber = 192, int BlockNumber = 961);//影像融合


	bool   SaveCudaBufferList(LPCTSTR filename);//輸出文件
	bool   SaveCudaBufferListFn(LPCTSTR filename);//輸出文件
	void   ClearCudeBufferSizeList();//清除Cude記憶體尺寸列表
	bool   LayoutCudeBufferSizeList();//排列Cude記憶體尺寸列表
	void   AddCudeBufferSize(size_t size);//新增Cude記憶體尺寸
	bool   TestCudaBuffer(size_t size, size_t Count);//測試Cuda記憶體, 同時也是一次初始化, 

protected:
	//------------------------------------------------------------------------------------------//
	int     m_CudaDeviceIndex;
	cudaDeviceProp m_CudaDevProp;
	char    m_ErrorString[256];
	bool    m_IsCudaDeviceUse;
	void    SetCudaDeviceIndex(int val);
	void    SetIsCudaDeviceUse(bool val);
	//------------------------------------------------------------------------------------------//
	struct CudaPtr
	{
		void         *ptr;     //記憶體指標
		size_t        size;    //記憶體大小		
		short         states;  //記憶體狀態		
#ifdef _DEBUG
		std::string   fnName;  //呼叫的函式名稱
		std::string   vaName;  //呼叫的變數名稱
#endif 
		CudaPtr(void *ptr, size_t size, short stats, const char*_fnName, const char*_vaName)
		{
			this->ptr = ptr;
			this->size = size;			
			this->states = stats;		
		#ifdef _DEBUG
			this->fnName = _fnName;
			this->vaName = _vaName;
		#endif
		}
	};   
	//------------------------------------------------------------------------------------------//
	static CRITICAL_SECTION    m_csCuda;//同步機制-關鍵區間
	static void                InitCudaLock();//初始化Cuda的關鍵區間
	static void                DeleteCudaLock(); //刪除Cuda的關鍵區間
	static void                LockCuda();        //進入Cuda的關鍵區間
	static void                UnlockCuda();      //離開Cuda的關鍵區間
	//------------------------------------------------------------------------------------------//
	static CRITICAL_SECTION    m_csCudaFunc;//同步機制-關鍵區間函式呼叫
	static void                InitCudaFuncLock();//初始化Cuda函式呼叫的關鍵區間
	static void                DeleteCudaFuncLock(); //刪除Cuda函式呼叫的關鍵區間	
	static void                LockCudaFunc();        //進入Cuda函式呼叫的關鍵區間
	static void                UnlockCudaFunc();      //離開Cuda函式呼叫的關鍵區間
	//------------------------------------------------------------------------------------------//
	size_t                     CalcBufferSize(IMAGE_SIZE Step, IMAGE_SIZE H) const;//計算記憶體大小
	//------------------------------------------------------------------------------------------//
	bool CheckPtr(const char fnName[], const void *Ptr);
	bool CheckPtr2(const char fnName[], const void *Ptr1, const void *Ptr2);
	bool CheckPtr3(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3);
	bool CheckPtr4(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4);
	bool CheckPtr5(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5);
	bool CheckPtr6(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6);
	bool CheckPtr7(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7);
	bool CheckPtr8(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8);
	bool CheckPtr9(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9);
	bool CheckPtr10(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10);
	bool CheckPtr11(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11);
	bool CheckPtr12(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12);
	bool CheckPtr13(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13);
	bool CheckPtr14(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14);
	bool CheckPtr15(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15);
	bool CheckPtr16(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16);
	bool CheckPtr17(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17);
	bool CheckPtr18(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18);
	bool CheckPtr19(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18, const void *Ptr19);
	bool CheckPtr20(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18, const void *Ptr19, const void *Ptr20);
	bool CheckPtr21(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18, const void *Ptr19, const void *Ptr20, const void *Ptr21);
	bool CheckPtr22(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18, const void *Ptr19, const void *Ptr20, const void *Ptr21, const void *Ptr22);

	bool CheckCastParam2Exp(const char fnName[], const TCastParam &CastParam);
	bool CheckCastParam(const char fnName[], unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const TCastParam &CastParam);
	bool CheckCastParamFn(const char fnName[], unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const TCastParam &CastParam);
	bool CloneCastParamToDevice(const char fnName[], unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const TCastParam &CastParam, TCastParam &CastParamD);
	bool CloneCastParamPtrToDevice(const char fnName[], unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const TCastParam &CastParam, TCastParam &CastParamD, PCastParam &CastParamDPtr);	
	bool FreeCastParamBuffer(TCastParam &CastParamD);
	bool FreeCastParamPtrBuffer(PCastParam &CastParamDPtr);
	bool CloneCastParamResultToHost(const TCastParam &CastParamD, TCastParam &CastParam);	
	//------------------------------------------------------------------------------------------//
	std::vector<CudaPtr>  m_CudaBufferList;
	std::vector<size_t>   m_BufferSizeList;	
	size_t                CalcFitBufferSize(size_t size);//計算適合的記憶體尺寸
	CudaPtr*              GetFreeCudaBuffer(size_t size_min);//取得可用的Cuda記憶體   
	void                  AddCudaBufferPtr(size_t size, void *Ptr, const char *fnName, const char *vaName);//新增加cuda記憶體
	void*                 allocBuffer_Cuda(size_t size, const char *fnName, const char *vaName);//建立cuda記憶體	
	bool                  UnlockBuffer_Cuda(void *Ptr);//釋放cuda記憶體
	bool                  freeBufferKernel_Cuda(void *Ptr);//釋放cuda記憶體
	bool                  ReleaseCudaBufferList(bool ShowMsg);//歸還整個Cuda記憶體
	
	bool                  output_Cuda(FILE* fp);//輸出文件
	bool                  allocBuffer_Cuda(size_t size, float *&Ptr, const char *fnName, const char *vaName);//建立cuda記憶體
	bool                  allocBuffer_Cuda(size_t size, short *&Ptr, const char *fnName, const char *vaName);//建立cuda記憶體
	bool                  allocBuffer_Cuda(size_t size, unsigned char *&Ptr, const char *fnName, const char *vaName);//建立cuda記憶體
	bool                  allocBuffer_Cuda(size_t size, TCastParam *&Ptr, const char *fnName, const char *vaName);//建立cuda記憶體	
	bool                  freeBuffer_Cuda(float *&Ptr);//釋放cuda記憶體
	bool                  freeBuffer_Cuda(short *&Ptr);//釋放cuda記憶體
	bool                  freeBuffer_Cuda(unsigned char *&Ptr);//釋放cuda記憶體
	bool                  freeBuffer_Cuda(TCastParam *&Ptr);//釋放cuda記憶體	
	//------------------------------------------------------------------------------------------//
	bool                  CudaMedianFilter_Internal(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *pMaskD, const TPhaseNoiseParam &NoiseParam, float *pSpaceD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);//中值濾波器
	//------------------------------------------------------------------------------------------//
	//解相位
//THREAD_00
	int m_PhaseBufferSizeD;
	unsigned char *m_ImgD1, *m_ImgD2, *m_ImgD3, *m_ImgD4, *m_ImgD5;
	unsigned char *m_pMergeD; short *m_pPhaseD; unsigned char *m_pMaskD;
	unsigned char *m_pMergeD_F2; short *m_pPhaseD_F2; unsigned char *m_pMaskD_F2;
//THREAD_01
	int m_PhaseBufferSizeD_01;
	unsigned char *m_ImgD1_01, *m_ImgD2_01, *m_ImgD3_01, *m_ImgD4_01, *m_ImgD5_01;
	unsigned char *m_pMergeD_01; short *m_pPhaseD_01; unsigned char *m_pMaskD_01;
	unsigned char *m_pMergeD_F2_01; short *m_pPhaseD_F2_01; unsigned char *m_pMaskD_F2_01;
//THREAD_02
	int m_PhaseBufferSizeD_02;
	unsigned char *m_ImgD1_02, *m_ImgD2_02, *m_ImgD3_02, *m_ImgD4_02, *m_ImgD5_02;
	unsigned char *m_pMergeD_02; short *m_pPhaseD_02; unsigned char *m_pMaskD_02;
	unsigned char *m_pMergeD_F2_02; short *m_pPhaseD_F2_02; unsigned char *m_pMaskD_F2_02;
//THREAD_03
	int m_PhaseBufferSizeD_03;
	unsigned char *m_ImgD1_03, *m_ImgD2_03, *m_ImgD3_03, *m_ImgD4_03, *m_ImgD5_03;
	unsigned char *m_pMergeD_03; short *m_pPhaseD_03; unsigned char *m_pMaskD_03;
	unsigned char *m_pMergeD_F2_03; short *m_pPhaseD_F2_03; unsigned char *m_pMaskD_F2_03;
//THREAD_04
	int m_PhaseBufferSizeD_04;
	unsigned char *m_ImgD1_04, *m_ImgD2_04, *m_ImgD3_04, *m_ImgD4_04, *m_ImgD5_04;
	unsigned char *m_pMergeD_04; short *m_pPhaseD_04; unsigned char *m_pMaskD_04;
	unsigned char *m_pMergeD_F2_04; short *m_pPhaseD_F2_04; unsigned char *m_pMaskD_F2_04;
	
	void GetSolveF1PhaseDataDevicePtr(unsigned char *&pMergeD, short *&pPhaseD, unsigned char *&pMaskD, const int ThreadNO);
	void GetSolveF2PhaseDataDevicePtr(unsigned char *&pMergeD, short *&pPhaseD, unsigned char *&pMaskD, const int ThreadNO);
	void GetSolvePhaseImageDevicePtr(unsigned char *&pImgD1, unsigned char *&pImgD2, unsigned char *&pImgD3, unsigned char *&pImgD4, unsigned char *&pImgD5, const int ThreadNO);
	void GetSolvePhaseDataDevicePtr(unsigned char *&pImgD1, unsigned char *&pImgD2, unsigned char *&pImgD3, unsigned char *&pImgD4, unsigned char *&pImgD5, unsigned char *&pMergeD, short *&pPhaseD, unsigned char *&pMaskD, const int ThreadNO);
	bool CreateSolvePhaseDataBuffer(const int nStep, int W,int H, const int ThreadNO);
	void ReleaseSolvePhaseDataBuffer();

	int m_BasePhaseBufferSizeD_A, m_BasePhaseBufferSizeD_A_F2;
	int m_BasePhaseBufferSizeD_B, m_BasePhaseBufferSizeD_B_F2;
	int m_BasePhaseBufferSizeD_C, m_BasePhaseBufferSizeD_C_F2;
	int m_BasePhaseBufferSizeD_D, m_BasePhaseBufferSizeD_D_F2;
	short *m_BasePhaseD_A, *m_BasePhaseD_A_F2;		
	short *m_BasePhaseD_B, *m_BasePhaseD_B_F2;
	short *m_BasePhaseD_C, *m_BasePhaseD_C_F2;		
	short *m_BasePhaseD_D, *m_BasePhaseD_D_F2;
	bool CreateBasePhasePlaneBuffer(const int DataSize, const int LightSide, const int FrequenceMode);
	void ReleaseBasePhasePlaneBuffer(const int FrequenceMode);
	void GetBasePhasePlaneDevicePtr(short *&pBasePhaseD, const int LightSide, const int FrequenceMode);

	int m_FFCBaseBufferSizeD_A;
	int m_FFCBaseBufferSizeD_B;
	int m_FFCBaseBufferSizeD_C;
	int m_FFCBaseBufferSizeD_D;
	float *m_FFCGainD_A;
	float *m_FFCGainD_B;
	float *m_FFCGainD_C;
	float *m_FFCGainD_D;
	unsigned char *m_FFCBaseD_A;
	unsigned char *m_FFCBaseD_B;
	unsigned char *m_FFCBaseD_C;
	unsigned char *m_FFCBaseD_D;
	bool CreateFFCBaseBuffer(const int DataSize, const int LightSide);
	void ReleaseFFCBaseBuffer();
	void GetFFCBaseBufferDevicePtr(float *&pGainD, unsigned char *&pBaseD, const int LightSide);


	//PhaseSmooth
//THREAD_00
	int m_PhaseSmoothBufferSizeD;
	short *m_PhaseSmoothDestD;
	float *m_PhaseSmoothTempSinBufferD;
	float *m_PhaseSmoothTempCosBufferD;
	unsigned char *m_PhaseSmoothTempMaskBufferD;
//THREAD_01
	int m_PhaseSmoothBufferSizeD_01;
	short *m_PhaseSmoothDestD_01;
	float *m_PhaseSmoothTempSinBufferD_01;
	float *m_PhaseSmoothTempCosBufferD_01;
	unsigned char *m_PhaseSmoothTempMaskBufferD_01;
//THREAD_02
	int m_PhaseSmoothBufferSizeD_02;
	short *m_PhaseSmoothDestD_02;
	float *m_PhaseSmoothTempSinBufferD_02;
	float *m_PhaseSmoothTempCosBufferD_02;
	unsigned char *m_PhaseSmoothTempMaskBufferD_02;
//THREAD_03
	int m_PhaseSmoothBufferSizeD_03;
	short *m_PhaseSmoothDestD_03;
	float *m_PhaseSmoothTempSinBufferD_03;
	float *m_PhaseSmoothTempCosBufferD_03;
	unsigned char *m_PhaseSmoothTempMaskBufferD_03;
//THREAD_04
	int m_PhaseSmoothBufferSizeD_04;
	short *m_PhaseSmoothDestD_04;
	float *m_PhaseSmoothTempSinBufferD_04;
	float *m_PhaseSmoothTempCosBufferD_04;
	unsigned char *m_PhaseSmoothTempMaskBufferD_04;
	void GetPhaseSmoothDataPtr(short *&pPhaseSmoothDestD, float *&pPhaseSmoothTempSinBuffer, float *&pPhaseSmoothTempCosBuffer, unsigned char *&pPhaseSmoothTempMaskBuffer, const int ThreadNO);
	bool CreatePhaseSmoothDataBuffer(int W, int H, const int ThreadNO);
	void ReleasePhaseSmoothDataBuffer();

	//ImageAverageSmooth
	size_t          m_MaxAverageSmoothSize;
	unsigned short *m_AverageSmoothBufferD;
	unsigned char  *m_AverageSmoothSrcD;
	unsigned char  *m_AverageSmoothDestD;	
	bool            CreateAverageSmoothDataBuffer(size_t size);
	void            ReleaseAverageSmoothDataBuffer();

	//2D Fileter Data Gaussian Smooth
	float *m_GaussianSmoothSrcD;
	float *m_GaussianSmoothDestD;
	float *m_GaussianSmoothBufferD;
	int m_MaxGaussianSmoothSize;
	void CreateGaussianSmoothDataBuffer(int W,int H);
	void ReleaseGaussianSmoothDataBuffer();

	bool InitCUDA();
	bool InitCUDAFn();
	void PreInitial();	
	void CudaExceptionClean();
	bool ReturnCudaDisabled();

public:
	void Initial();
	void ReleaseAll();

	void SetCudaExceptionCode_Param(LPCTSTR Err=NULL);	
	void SetCudaExceptionCode_FileRead(LPCTSTR Err=NULL);	
	void SetCudaExceptionCode_FileWrite(LPCTSTR Err=NULL);	
	void SetCudaExceptionCode_ExeFunc(LPCTSTR Err=NULL);	
	void SetCudaExceptionCode_MemCopy(LPCTSTR Err=NULL);		
	void SetCudaExceptionCode(DWORD Code, LPCTSTR Err=NULL);
};
//------------------------------------------------------------------------------//
extern CCudaFunc CudaFunc;
//#endif//CUDA_USE
#endif



