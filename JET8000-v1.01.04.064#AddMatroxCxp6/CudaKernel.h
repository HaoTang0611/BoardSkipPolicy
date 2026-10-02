#ifndef  _CUDA_KERNEL__H_
#define  _CUDA_KERNEL__H_

#include "StdAfx.h"
//#include <stdio.h>
#include <stdlib.h>

#ifdef  CUDA_USE
#if _MSC_VER >= 1900//VS_2015_NET
	#include "..\\JET8000_Library\\CUDA_9_1\\include\\cuda_runtime.h"	
#elif _MSC_VER >= 1500//VS_2008_NET
	#ifdef _X64
		#include "..\\JET8000_Library\\CUDA\\include\\x64\\cuda_runtime.h"		
	#else
		#include "..\\JET8000_Library\\CUDA\\include\\x32\\cuda_runtime.h"
	#endif//_X64
#else
#endif//_MSC_VER

#define GPU_BLOCK_SIZE	191

int iDivUp(int a, int b);
//------------------------------------------------------------------------------//
//Host function//---------------------------------------------------------------//
//------------------------------------------------------------------------------//
__global__ void DoCudaSolvePhaseKernel(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *ImgD4, unsigned char *pMergeD, short *pPhaseD,unsigned char *pMaskD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);
__global__ void DoCudaSolvePhase_Three_Kernel(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);
__global__ void DoCudaSolvePhase_Four_Kernel(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *ImgD4, unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);
__global__ void DoCudaSolvePhase_Five_Kernel(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *ImgD4, unsigned char *ImgD5, unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);
__global__ void DoCudaSolvePhaseTexture_Three_Kernel(unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);
__global__ void DoCudaSolvePhaseTexture_Four_Kernel(unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);
__global__ void DoCudaSolvePhaseTexture_Five_Kernel(unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);

__global__ void DoCudaSolvePhaseAndBasePhaseTexture_Four_Kernel2(unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);
__global__ void DoCudaSolvePhaseAndBasePhase_Four_Kernel2(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *ImgD4, unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, short *pBasePhaseD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);
__global__ void DoCudaSolvePhaseTexture_Four_Kernel2(unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);
__global__ void DoCudaSolvePhase_Four_Kernel2(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *ImgD4, unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);
__global__ void DoCudaSolveMultiPhase_Kernel2(double TempRatio, short *pPhaseData1, unsigned char *pMaskMap1, short *pPhaseData2, unsigned char *pMaskMap2, const unsigned int ImageSize );
__global__ void DoCudaSolveMultiPhase_Smooth_Kernel2(short *pMultiPhase, unsigned char *pMultiMaskMap, short *pSmoothPhase, unsigned char *pSmoothMaskMap, const unsigned int ImageSize, const unsigned int W, const unsigned int H );

__global__ void DoCudaSolvePhaseAndFFC_Three_Kernel(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, float *pFFCGainD ,unsigned char *pFFCBaseD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);
__global__ void DoCudaSolvePhaseAndFFC_Four_Kernel(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *ImgD4, unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, float *pFFCGainD ,unsigned char *pFFCBaseD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);
__global__ void DoCudaSolvePhaseAndFFC_Five_Kernel(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *ImgD4, unsigned char *ImgD5, unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, float *pFFCGainD ,unsigned char *pFFCBaseD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);
__global__ void DoCudaSolvePhaseAndFFCTexture_Three_Kernel(unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);
__global__ void DoCudaSolvePhaseAndFFCTexture_Four_Kernel(unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);
__global__ void DoCudaSolvePhaseAndFFCTexture_Five_Kernel(unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, const unsigned int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);

__global__ void DoCudaSubtractBasePhasePlane_Kernel(short *pPhaseD, short *pBasePhaseD, const unsigned int DataSize);
__global__ void DoCudaSubtractBasePhasePlaneTexture_Kernel(short *pPhaseD, const unsigned int DataSize);


//------------------------------------------------------------------------------//
__global__ void DoCudaPhaseSmoothRowsKernel(short *pSrcD, unsigned char *pMaskD, float *pSinDestD, float *pCosDestD, unsigned char *pTempMaskBufferD, unsigned int DataW, unsigned int DataH, unsigned int pitch, int Kernel_Radius);
__global__ void DoCudaPhaseSmoothColumnsKernel(float *pSinSrcD, float *pCosSrcD, unsigned char *pTempMaskBufferD, short *pDestD, unsigned char *pMaskD, unsigned int DataW, unsigned int DataH, unsigned int pitch, int Kernel_Radius);

__global__ void DoCudaPhaseSmoothRowsTextureKernel(float *pSinDestD, float *pCosDestD, unsigned char *pTempMaskBufferD, unsigned int DataW, unsigned int DataH, unsigned int pitch, int Kernel_Radius);
__global__ void DoCudaPhaseSmoothColumnsTextureKernel(short *pDestD, unsigned char *pMaskD, unsigned int DataW, unsigned int DataH, unsigned int pitch, int Kernel_Radius);

__global__ void DoCudaImageSmooth_Kernel(unsigned char *pImage, unsigned char *pSmoothImage, const unsigned int ImageW, const unsigned int ImageH, const int MaskSize);
__global__ void DoCudaImageSmoothTexture_Kernel(unsigned char *pSmoothImage, const unsigned int ImageW, const unsigned int ImageH, const int MaskSize);


__global__ void DoCudaFFCProcessKernel(unsigned char *pSrc, unsigned char *pDest, float *pGain ,unsigned char *pBase,unsigned int ImageSize);
//------------------------------------------------------------------------------//
__global__ void DoCudaRecursiveGaussianKernel(unsigned char *pSrc, unsigned char *pDest, const unsigned int W, const unsigned int H, float a0, float a1, float a2, float a3, float b1, float b2, float coefp, float coefn);
__global__ void DoTransposeKernel(unsigned char *pDest, unsigned char *pSrc, unsigned int width, unsigned int height);
//------------------------------------------------------------------------------//
__global__ void DoCudaAverageSmoothRowsKernel(unsigned short *d_Dst,const unsigned char *d_Src,unsigned int imageW,unsigned int imageH,unsigned int pitch,int Kernel_Radius);
__global__ void DoCudaAverageSmoothColumnsKernel(unsigned char *d_Dst, const unsigned short *d_Src,unsigned int imageW,unsigned int imageH, unsigned int pitch,int Kernel_Radius,int SumNumber);

//2DFilter Convolution
__global__ void DoDataConvolutionRowsKernel(float *d_Dst,float *d_Src,unsigned int imageW,unsigned int imageH,unsigned int pitch,int Kernel_Radius);
__global__ void DoDataConvolutionColumnsKernel(float *d_Dst,float *d_Src,unsigned int imageW,unsigned int imageH, unsigned int pitch,int Kernel_Radius);

//Kai 20161017
//在.cu上的函式有順序
//Solve Phase
//short CalcArctangent(int y, int x);//Kai-20161015
__device__ bool  GetCudaUseNewContrast_Kernel();//20190530
__device__ int   CalcCudaGamma_Kernel(int Gray, float Gamma);//計算像素Gamma結果
__device__ bool  DeductBasePhase_Kernel(int BasePhase, int &Phase);//扣除相平面
__device__ unsigned int CudaPow_Kernel(int Base, int Order);//計算指數
__device__ unsigned int CudaGrayCodetoDecimal_Kernel(unsigned int x);//GrayCode轉10進位//Kai-20210217
__device__ bool  BuildCudaGrayCodeMapTable_Kernel(int Order, int List[], int size);//取得GrayCode的映射表//Kai-20210217
__device__ bool  BuildCudaGrayCodeMapTable_Kernel(int Order, unsigned char List[], int size);//取得GrayCode的映射表//Kai-20210217
__device__ bool  ConvertPhaseToSpace_Kernel(int CvtMode, int Phase, float k1, float k2, float &Space);//相位轉成高度值
__device__ bool  ConvertSpaceToPhase_Kernel(int CvtMode, float Space, float k1, float k2, int &Phase);//高度轉成相位值
__device__ bool  ConvertPhaseToSpaceMapFunc_Kernel(int CvtMode, double X, double Y, int Phase, const double* T0, const double* T1, const double* T2, float &Space);//相位轉成高度值
__device__ bool  ConvertSpaceToPhaseMapFunc_Kernel(int CvtMode, double X, double Y, float Space, const double* T0, const double* T1, const double* T2, int &Phase);//高度轉成相位值
__device__ float GetPhaseNoiseMinB_Kernel(int val);//取得相位雜訊定義MinB
__device__ float GetPhaseNoiseMinCV_Kernel(int val);//取得相位雜訊定義MinCV
__device__ float GetPhaseNoiseMinB_Kernel(const TPhaseNoiseParam &NoiseParam);//取得相位雜訊定義MinB
__device__ float GetPhaseNoiseMinCV_Kernel(const TPhaseNoiseParam &NoiseParam);//取得相位雜訊定義MinCV
__device__ bool CalcCudaArctangent_Kernel(int a, int b, int &Phase);//計算反三角
__device__ int  CheckCudaSpaceMaskValid_Kernel(unsigned char Mask);//確認是否為有效值
__device__ int  CheckCudaSpaceMaskValidBest_Kernel(unsigned char Mask);//確認是否為有效值
__device__ int  CalcCudaPhaseContrast_Kernel(int a, int b, int c);//計算相位對比
__device__ int  CalcCuda3PhaseContrast_Kernel(int P1, int P2, int P3);//計算相位對比
__device__ int  CalcCuda21PhaseContrast_Kernel(int P1, int P2, int P3);//計算相位對比
__device__ int  CalcCuda4PhaseContrast_Kernel(int P1, int P2, int P3, int P4);//計算相位對比
__device__ int  CalcCuda3PhasePotential_Kernel(int P1, int P2, int P3);//計算相位潛能
__device__ int  CalcCuda21PhasePotential_Kernel(int P1, int P2, int P3);//計算相位潛能
__device__ int  CalcCuda4PhasePotential_Kernel(int P1, int P2, int P3, int P4);//計算相位潛能
__device__ int  CalcCuda3PhaseSaturated_Kernel(int P1, int P2, int P3, int SatTh);//計算相位過飽和
__device__ bool CheckCuda3PhaseSaturated_Kernel(int P1, int P2, int P3, int SatTh);//確認相位過飽和
__device__ int  CalcCuda4PhaseSaturated_Kernel(int P1, int P2, int P3, int P4, int SatTh);//計算相位過飽和
__device__ bool CheckCuda4PhaseSaturated_Kernel(int P1, int P2, int P3, int P4, int SatTh);//確認相位過飽和
__device__ bool CalcCuda4PhaseMeanContrastRatio_Kernel(int P1, int P2, int P3, int P4, float &A, float &B, float &CV);
__device__ bool CalcCuda3PhaseParam_kernel(int P1, int P2, int P3, int &a, int &b, int &c);//3Step
__device__ bool CalcCuda21PhaseParam_kernel(int P1, int P2, int P3, int &a, int &b, int &c);//2+1Step
__device__ bool CalcCuda4PhaseParam_kernel(int P1, int P2, int P3, int P4, int &a, int &b, int &c);//4Step
__device__ bool CalcCuda3PhaseParamMask_kernel(int P1, int P2, int P3, int NoiseMask, int LowContrast, int LowPotential, int OverSaturated, int &Phase, unsigned char &Mask);//3Step
__device__ bool CalcCuda21PhaseParamMask_kernel(int P1, int P2, int P3, int NoiseMask, int LowContrast, int LowPotential, int OverSaturated, int &Phase, unsigned char &Mask);//2+1Step
__device__ bool CalcCuda4PhaseParamMask_kernel_1(int P1, int P2, int P3, int P4, int NoiseMask, int LowContrast, int LowPotential, int OverSaturated, int &Phase, unsigned char &Mask);//4Step
__device__ bool CalcCuda4PhaseParamMask_kernel_2(int P1, int P2, int P3, int P4, int NoiseMask, int LowContrast, int LowPotential, int OverSaturated, int &Phase, unsigned char &Mask);//4Step
__device__ bool CalcCuda4PhaseParamMask_kernel(int P1, int P2, int P3, int P4, int NoiseMask, int LowContrast, int LowPotential, int OverSaturated, int &Phase, unsigned char &Mask, int Mode);//4Step
__device__ bool CalcCuda4GrayCodeParam_kernel(int P1, int P2, int P3, int P4, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//4階GrayCode解碼
__device__ bool CalcCuda4GrayCodeParam_kernel_1(int P1, int P2, int P3, int P4, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//4階GrayCode解碼
__device__ bool CalcCuda4GrayCodeParam_kernel_2(int P1, int P2, int P3, int P4, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//4階GrayCode解碼
__device__ bool CalcCuda5GrayCodeParam_kernel(int P1, int P2, int P3, int P4, int P5, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//5階GrayCode解碼
__device__ bool CalcCuda5GrayCodeParam_kernel_1(int P1, int P2, int P3, int P4, int P5, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//5階GrayCode解碼
__device__ bool CalcCuda5GrayCodeParam_kernel_2(int P1, int P2, int P3, int P4, int P5, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//5階GrayCode解碼
__device__ bool CalcCuda6GrayCodeParam_kernel(int P1, int P2, int P3, int P4, int P5, int P6, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//6階GrayCode解碼
__device__ bool CalcCuda6GrayCodeParam_kernel_1(int P1, int P2, int P3, int P4, int P5, int P6, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//6階GrayCode解碼
__device__ bool CalcCuda6GrayCodeParam_kernel_2(int P1, int P2, int P3, int P4, int P5, int P6, float Th, const unsigned char *Map1, const unsigned char *Map2, int &k1, int &k2);//6階GrayCode解碼
__device__ unsigned char CombineCudaPhaseMask_Kernel(unsigned char Mask1, unsigned char Mask2);//合併大小週期遮罩
__device__ int  CombineCudaPhasePeriod_MaxMin_Kernel(int Phase1, int Phase2, double Ratio);//合併大小週期
__device__ int  CombineCudaPhasePeriod_Sibling_Kernel(int Phase1, int Phase2, double Ratio);//合併鄰近週期
__device__ int  CombineCudaPhasePeriod_Kernel(int Mode, int Phase1, int Phase2, double Ratio);//合併兩個週期
__device__ bool CalcCudaCombinePhasePeriodRatio_Kernel(int Mode, float P1, float P2, float &Ratio);//計算合併兩個週期比例
__device__ int  CombineCudaPhasePeriod_GrayCode_Kernel(int Phase, int Period, int NPeriod, int k1, int k2);//合併相位與GrayCode
__device__ bool MergeCuda2ExpPhaseData_Kernel(unsigned char Mask1, int Phase1, unsigned char Mask2, int Phase2, unsigned char &Mask, int &Phase);//合併雙曝光相位
__device__ bool MergeCuda2ExpPhaseData_Kernel_II(unsigned char Mask1, int Phase1, unsigned char Mask2, int Phase2, unsigned char &Mask, int &Phase, int ZeroPhase);//合併雙曝光相位
__device__ bool MergeCuda2ExpPhaseDataByMean_Kernel(unsigned char Mask1, int Phase1, float Mean1, unsigned char Mask2, int Phase2, float Mean2, unsigned char &Mask, int &Phase);//合併雙曝光相位
__device__ bool MergeCuda2ExpPhaseDataByMaxB_Kernel(unsigned char Mask1, int Phase1, float A1, float B1, unsigned char Mask2, int Phase2, float A2, float B2, unsigned char &Mask, int &Phase);//合併雙曝光相位
__device__ bool MergeCuda2ExpPhaseDataByMaxCV_Jun_Kernel(float A[], float B[], float CV[], int nBrightness, int nSaturated, int &nIdx);//合併2個曝光的相位-軍達提供
__device__ bool MergeCuda2ExpPhaseDataByMaxCV_Kernel(unsigned char Mask1, int Phase1, float A1, float B1, unsigned char Mask2, int Phase2, float A2, float B2, unsigned char &Mask, int &Phase);//合併雙曝光相位

__device__ bool GeometryMedianCudaSpaceHeightIter_Kernel(const float SpaceList[], int SpaceCnt, float &Space);//幾何中值法
__device__ bool GeometryMedianCudaSpaceHeight_Kernel(const float SpaceList[], int SpaceCnt, float ZeroPlane, float &Space);//幾何中值法

__device__ int  GetCudaMassGroupIndex_Kernel(int GroupCnt[], int Count);//取得群組數量最多的引數
__device__ bool GetCudaMinValueIndex_Kernel(float SpaceList[], int StartIdx, int SpaceCnt);//取得最低值引數
__device__ bool CudaMergeSortMerge_Kernel(float array[], int front, int mid, int end);//合併排序-合併
__device__ bool CudaMergeSortRecursion_Kernel(float array[], int front, int end);//合併排序-遞迴
__device__ bool CudaMergeSortIterative_Kernel(float array[], int count);//合併排序-疊代
__device__ void CudaQuickSortSwap_Kernel(float &a, float &b);//快速排序-交換
__device__ int  CudaQuickSortPartition_Kernel(float array[], int front, int end);//快速排序-分割
__device__ bool CudaQuickSortRecursion_Kernel(float array[], int front, int end);//快速排序-遞迴
__device__ bool CudaQuickSortIterative_Kernel(float array[], int front, int end);//快速排序-疊代
__device__ bool SortCudaSpcaeHeightMergeSort_Kernel(float SpaceList[], int SpaceCnt);//高度排序-合併排序
__device__ bool SortCudaSpcaeHeightQuickSort_Kernel(float SpaceList[], int SpaceCnt);//高度排序-快速排序
__device__ bool SortCudaSpcaeHeightInsertion_Kernel(float SpaceList[], int SpaceCnt);//高度排序-插入法
__device__ bool SortCudaSpcaeHeightSelection_Kernel(float SpaceList[], int SpaceCnt);//高度排序-選擇法
__device__ bool SortCudaSpcaeHeight_Kernel(float SpaceList[], int SpaceCnt);//高度排序
__device__ float AverageCudaSpcaeHeight_Kernel(const float SpaceList[], int SpaceCnt);//平均高度
__device__ bool AddCudaGroupSpcaeHeightByMean_Kernel(float Space, float Range, float &Group, int &GroupCnt);//加入高度分群-依照平均值
__device__ bool AddCudaGroupSpcaeHeightByRange_Kernel(float Space, float Range, float &GroupMin, float &GroupMax, float &GroupAve, int &GroupCnt);//加入高度分群-依照範圍值
__device__ bool GroupCudaSpcaeHeightByMean_Kernel(float SpaceList[], float Range, float GroupAveList[], int GroupCntList[], int SpaceCnt, int &GoupCount);//高度分群-依照平均值
__device__ bool GroupCudaSpcaeHeightByRange_Kernel(float SpaceList[], float Range, float GroupMinList[], float GroupMaxList[], float GroupAveList[], int GroupCntList[], int SpaceCnt, int &GoupCount);//高度分群-依照範圍值
__device__ bool CompareCuda4OppositeGray_Kernel(const TPhaseNoiseParam &NoiseParam, int P1, int P2, int P3, int P4, unsigned char &Mask1, unsigned char &Mask2, unsigned char &Mask3, unsigned char &Mask4);//比較相對打燈灰階
__device__ bool CompareCuda2SpaceHeight_Kernel(const TPhaseNoiseParam &NoiseParam, unsigned char &Mask1, float Space1, unsigned char &Mask2, float Space2, unsigned char &Mask, float &Space);//比較空間高度
__device__ bool CompareCuda3SpaceHeight_Kernel(const TPhaseNoiseParam &NoiseParam, unsigned char &Mask1, float Space1, unsigned char &Mask2, float Space2, unsigned char &Mask3, float Space3, unsigned char &Mask, float &Space);//比較空間高度
__device__ bool CompareCuda4SpaceHeight_Kernel_1(const TPhaseNoiseParam &NoiseParam, unsigned char &Mask1, float Space1, unsigned char &Mask2, float Space2, unsigned char &Mask3, float Space3, unsigned char &Mask4, float Space4, unsigned char &Mask, float &Space);//比較空間高度
__device__ bool CompareCuda4SpaceHeight_Kernel_2(const TPhaseNoiseParam &NoiseParam, unsigned char &Mask1, float Space1, unsigned char &Mask2, float Space2, unsigned char &Mask3, float Space3, unsigned char &Mask4, float Space4, unsigned char &Mask, float &Space, bool bChkMask);//比較空間高度
__device__ bool CompareCuda4SpaceHeight_Kernel_3(const TPhaseNoiseParam &NoiseParam, unsigned char &Mask1, float Space1, unsigned char &Mask2, float Space2, unsigned char &Mask3, float Space3, unsigned char &Mask4, float Space4, unsigned char &Mask, float &Space);//比較空間高度
__device__ bool CompareCuda4SpaceHeight_Kernel(const TPhaseNoiseParam &NoiseParam, unsigned char &Mask1, float Space1, unsigned char &Mask2, float Space2, unsigned char &Mask3, float Space3, unsigned char &Mask4, float Space4, unsigned char &Mask, float &Space);//比較空間高度
__device__ bool CompareCudaNSpaceHeight_Kernel_1(const TPhaseNoiseParam &NoiseParam, const unsigned char MaskList[], const float SpaceList[], int SpaceCount, unsigned char &Mask, float &Space);//比較空間高度
__device__ bool CompareCudaNSpaceHeight_Kernel_2(const TPhaseNoiseParam &NoiseParam, const unsigned char MaskList[], const float SpaceList[], int SpaceCount, unsigned char &Mask, float &Space);//比較空間高度
__device__ bool CompareCudaNSpaceHeight_Kernel(const TPhaseNoiseParam &NoiseParam, const unsigned char MaskList[], const float SpaceList[], int SpaceCount, unsigned char &Mask, float &Space);//比較空間高度

__device__ bool MergeCuda2SpaceData_Kernel(const TPhaseNoiseParam &NoiseParam, unsigned char Mask1, float Space1, unsigned char Mask2, float Space2, unsigned char &Mask, float &Space);//合併2個高度
__device__ bool MergeCuda2SpaceData_Joe_Kernel_01(const TPhaseNoiseParam &NoiseParam, int Raw1[], unsigned char Mask1, float Space1, int Raw2[], unsigned char Mask2, float Space2, unsigned char &Mask, float &Space);//合併2個高度
__device__ bool MergeCuda2SpaceData_Joe_Kernel_02(const TPhaseNoiseParam &NoiseParam, int Raw1[], unsigned char Mask1, float Space1, int Raw2[], unsigned char Mask2, float Space2, unsigned char &Mask, float &Space);//合併2個高度
__device__ bool MergeCuda2SpaceData_Joe_Kernel(const TPhaseNoiseParam &NoiseParam, int Raw1[], unsigned char Mask1, float Space1, int Raw2[], unsigned char Mask2, float Space2, unsigned char &Mask, float &Space);//合併2個高度
__device__ bool MergeCuda3SpaceData_Kernel(const TPhaseNoiseParam &NoiseParam, unsigned char Mask1, float Space1, unsigned char Mask2, float Space2, unsigned char Mask3, float Space3, unsigned char &Mask, float &Space);//合併3個高度
__device__ bool MergeCuda3SpaceData_Joe_Kernel_01(const TPhaseNoiseParam &NoiseParam, int Raw1[], unsigned char Mask1, float Space1, int Raw2[], unsigned char Mask2, float Space2, int Raw3[], unsigned char Mask3, float Space3, unsigned char &Mask, float &Space);//合併3個高度
__device__ bool MergeCuda3SpaceData_Joe_Kernel_02(const TPhaseNoiseParam &NoiseParam, int Raw1[], unsigned char Mask1, float Space1, int Raw2[], unsigned char Mask2, float Space2, int Raw3[], unsigned char Mask3, float Space3, unsigned char &Mask, float &Space);//合併3個高度
__device__ bool MergeCuda3SpaceData_Joe_Kernel(const TPhaseNoiseParam &NoiseParam, int Raw1[], unsigned char Mask1, float Space1, int Raw2[], unsigned char Mask2, float Space2, int Raw3[], unsigned char Mask3, float Space3, unsigned char &Mask, float &Space);//合併3個高度
__device__ bool MergeCuda4SpaceData_Kernel(const TPhaseNoiseParam &NoiseParam, unsigned char Mask1, float Space1, unsigned char Mask2, float Space2, unsigned char Mask3, float Space3, unsigned char Mask4, float Space4, unsigned char &Mask, float &Space);//合併4個高度
__device__ bool MergeCuda4SpaceData_Joe_Kernel_01(const TPhaseNoiseParam &NoiseParam, int Raw1[], unsigned char Mask1, float Space1, int Raw2[], unsigned char Mask2, float Space2, int Raw3[], unsigned char Mask3, float Space3, int Raw4[], unsigned char Mask4, float Space4, unsigned char &Mask, float &Space);//合併4個高度
__device__ bool MergeCuda4SpaceData_Joe_Kernel_02(const TPhaseNoiseParam &NoiseParam, int Raw1[], unsigned char Mask1, float Space1, int Raw2[], unsigned char Mask2, float Space2, int Raw3[], unsigned char Mask3, float Space3, int Raw4[], unsigned char Mask4, float Space4, unsigned char &Mask, float &Space);//合併4個高度
__device__ bool MergeCuda4SpaceData_Joe_Kernel(const TPhaseNoiseParam &NoiseParam, int Raw1[], unsigned char Mask1, float Space1, int Raw2[], unsigned char Mask2, float Space2, int Raw3[], unsigned char Mask3, float Space3, int Raw4[], unsigned char Mask4, float Space4, unsigned char &Mask, float &Space);//合併4個高度
__device__ bool MergeCudaNSpaceData_Debug_Kernel(unsigned int idx, const TPhaseNoiseParam &NoiseParam, const unsigned char MaskList[], const float SpaceList[], int SpaceCount, unsigned char &Mask, float &Space);//合併N個高度
__device__ bool MergeCudaNSpaceData_General_Kernel(unsigned int idx, const TPhaseNoiseParam &NoiseParam, const unsigned char MaskList[], const float SpaceList[], int SpaceCount, unsigned char &Mask, float &Space);//合併N個高度
__device__ bool MergeCudaNSpaceData_Median_Kernel(unsigned int idx, const TPhaseNoiseParam &NoiseParam, const unsigned char MaskList[], const float SpaceList[], int SpaceCount, unsigned char &Mask, float &Space);//合併N個高度
__device__ bool MergeCudaNSpaceData_Kernel(unsigned int idx, const TPhaseNoiseParam &NoiseParam, const unsigned char MaskList[], const float SpaceList[], int SpaceCount, unsigned char &Mask, float &Space);//合併N個高度

__global__ void ExecCudaBuildAtanTable_Kernel();//Kai-20161015
__global__ void ExecCudaSolvePhase221Frames2Phase_Kernel(const unsigned char *ImgDA1, const unsigned char *ImgDA2, const unsigned char *ImgDB1, const unsigned char *ImgDB2, const unsigned char *ImgDB3, const short *pBasePhaseD, double PA, double PB, int ExpTimeA, int ExpTimeB, unsigned int ImageSize, float Gamma, TPhaseNoiseParam NoiseParam, unsigned char *pMaskD, short *pPhaseD);//2+2步雙相位
__global__ void ExecCudaSolvePhase42Frames2Phase_Kernel(const unsigned char *ImgDA1, const unsigned char *ImgDA2, const unsigned char *ImgDA3, const unsigned char *ImgDA4, const unsigned char *ImgDB1, const unsigned char *ImgDB2, const short *pBasePhaseD, double PA, double PB, int ExpTimeA, int ExpTimeB, unsigned int ImageSize, float Gamma, TPhaseNoiseParam NoiseParam, unsigned char *pMaskD, short *pPhaseD);//4+2步雙相位
__global__ void ExecCudaSolvePhase44GCFrames2Phase_Kernel(const unsigned char *ImgDA1, const unsigned char *ImgDA2, const unsigned char *ImgDA3, const unsigned char *ImgDA4, const unsigned char *ImgDB1, const unsigned char *ImgDB2, const unsigned char *ImgDB3, const unsigned char *ImgDB4, const short *pBasePhaseD, double PA, double PB, int ExpTimeA, int ExpTimeB, unsigned int ImageSize, float Gamma, TPhaseNoiseParam NoiseParam, unsigned char *pMaskD, short *pPhaseD);//4步+4GC雙相位
__global__ void ExecCudaSolvePhase45GCFrames2Phase_Kernel(const unsigned char *ImgDA1, const unsigned char *ImgDA2, const unsigned char *ImgDA3, const unsigned char *ImgDA4, const unsigned char *ImgDB1, const unsigned char *ImgDB2, const unsigned char *ImgDB3, const unsigned char *ImgDB4, const unsigned char *ImgDB5, const short *pBasePhaseD, double PA, double PB, int ExpTimeA, int ExpTimeB, unsigned int ImageSize, float Gamma, TPhaseNoiseParam NoiseParam, unsigned char *pMaskD, short *pPhaseD);//4步+5GC雙相位
__global__ void ExecCudaSolvePhase46GCFrames2Phase_Kernel(const unsigned char *ImgDA1, const unsigned char *ImgDA2, const unsigned char *ImgDA3, const unsigned char *ImgDA4, const unsigned char *ImgDB1, const unsigned char *ImgDB2, const unsigned char *ImgDB3, const unsigned char *ImgDB4, const unsigned char *ImgDB5, const unsigned char *ImgDB6, const short *pBasePhaseD, double PA, double PB, int ExpTimeA, int ExpTimeB, unsigned int ImageSize, float Gamma, TPhaseNoiseParam NoiseParam, unsigned char *pMaskD, short *pPhaseD);//4步+6GC雙相位

__global__ void ExecCudaSolvePhase4Frames_Kernel(const unsigned char *ImgD1, const unsigned char *ImgD2, const unsigned char *ImgD3, const unsigned char *ImgD4, short *pPhaseD, unsigned char *pMaskD, unsigned int ImageSize, int PatternID, float Gamma, TPhaseNoiseParam NoiseParam);//四步單相位
__global__ void ExecCudaSolvePhase4Frames2Phase_Kernel(const unsigned char *ImgDA1, const unsigned char *ImgDA2, const unsigned char *ImgDA3, const unsigned char *ImgDA4, const unsigned char *ImgDB1, const unsigned char *ImgDB2, const unsigned char *ImgDB3, const unsigned char *ImgDB4, const short *pBasePhaseD, double PA, double PB, unsigned int ImageSize, float Gamma, TPhaseNoiseParam NoiseParam, unsigned char *pMaskD, short *pPhaseD);//四步雙相位

__global__ void ExecCudaMergeSpace4Frames2Period2Cast_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//合併2投光
__global__ void ExecCudaMergeSpace4Frames2Period3Cast_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//合併3投光
__global__ void ExecCudaMergeSpace4Frames2Period4Cast_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, TCastParam *pCastParamD4, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//合併4投光

__global__ void ExecCudaSolveSpace221Frames2Period1Cast_2_2_Kernel(TCastParam *pCastParamD, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//2+2步雙相位-單投光
__global__ void ExecCudaSolveSpace4Frames2Period1Cast_4_2_Kernel(TCastParam *pCastParamD, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+2步雙相位-單投光
__global__ void ExecCudaSolveSpace4Frames2Period1Cast_4_4_Kernel(TCastParam *pCastParamD, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+4步雙相位-單投光
__global__ void ExecCudaSolveSpace4Frames2Period1Cast_4_4GC_Kernel(TCastParam *pCastParamD, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+4GC雙相位-單投光
__global__ void ExecCudaSolveSpace4Frames2Period1Cast_4_5GC_Kernel(TCastParam *pCastParamD, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+5GC雙相位-單投光
__global__ void ExecCudaSolveSpace4Frames2Period1Cast_4_6GC_Kernel(TCastParam *pCastParamD, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+6GC雙相位-單投光

__global__ void ExecCudaSolveSpace221Frames2Period2Cast_2_2_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//2+2步雙相位-2投光
__global__ void ExecCudaSolveSpace4Frames2Period2Cast_4_2_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+2步雙相位-2投光
__global__ void ExecCudaSolveSpace4Frames2Period2Cast_4_4_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+4步雙相位-2投光
__global__ void ExecCudaSolveSpace4Frames2Period2Cast_4_4GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+4GC雙相位-2投光
__global__ void ExecCudaSolveSpace4Frames2Period2Cast_4_5GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+5GC雙相位-2投光
__global__ void ExecCudaSolveSpace4Frames2Period2Cast_4_6GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+6GC雙相位-2投光

__global__ void ExecCudaSolveSpace221Frames2Period3Cast_2_2_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//2+2步雙相位-3投光
__global__ void ExecCudaSolveSpace4Frames2Period3Cast_4_2_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+2步雙相位-3投光
__global__ void ExecCudaSolveSpace4Frames2Period3Cast_4_4_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+4步雙相位-3投光
__global__ void ExecCudaSolveSpace4Frames2Period3Cast_4_4GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+4GC雙相位-3投光
__global__ void ExecCudaSolveSpace4Frames2Period3Cast_4_5GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+5GC雙相位-3投光
__global__ void ExecCudaSolveSpace4Frames2Period3Cast_4_6GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+6GC雙相位-3投光

__global__ void ExecCudaSolveSpace221Frames2Period4Cast_2_2_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, TCastParam *pCastParamD4, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//2+2步雙相位-4投光
__global__ void ExecCudaSolveSpace4Frames2Period4Cast_4_2_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, TCastParam *pCastParamD4, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+2步雙相位-4投光
__global__ void ExecCudaSolveSpace4Frames2Period4Cast_4_4_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, TCastParam *pCastParamD4, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+4步雙相位-4投光
__global__ void ExecCudaSolveSpace4Frames2Period4Cast_4_4GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, TCastParam *pCastParamD4, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+4GC雙相位-4投光
__global__ void ExecCudaSolveSpace4Frames2Period4Cast_4_5GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, TCastParam *pCastParamD4, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+5GC雙相位-4投光
__global__ void ExecCudaSolveSpace4Frames2Period4Cast_4_6GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, TCastParam *pCastParamD4, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+6GC雙相位-4投光

__global__ void ExecCudaSolveSpace221Frames2Period1Cast2Exp_2_2_Kernel(TCastParam *pCastParamD, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//2+2步雙相位-單投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period1Cast2Exp_4_2_Kernel(TCastParam *pCastParamD, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+2步雙相位-單投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period1Cast2Exp_4_4_Kernel(TCastParam *pCastParamD, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+4步雙相位-單投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period1Cast2Exp_4_4GC_Kernel(TCastParam *pCastParamD, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+4GC雙相位-單投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period1Cast2Exp_4_5GC_Kernel(TCastParam *pCastParamD, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+5GC雙相位-單投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period1Cast2Exp_4_6GC_Kernel(TCastParam *pCastParamD, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+6GC雙相位-單投光2曝光

__global__ void ExecCudaSolveSpace221Frames2Period2Cast2Exp_2_2_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//2+2步雙相位-2投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period2Cast2Exp_4_2_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+2步雙相位-2投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period2Cast2Exp_4_4_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+4步雙相位-2投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period2Cast2Exp_4_4GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+4GC雙相位-2投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period2Cast2Exp_4_5GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+5GC雙相位-2投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period2Cast2Exp_4_6GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+6GC雙相位-2投光2曝光

__global__ void ExecCudaSolveSpace221Frames2Period3Cast2Exp_2_2_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//2+2步雙相位-3投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period3Cast2Exp_4_2_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+2步雙相位-3投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period3Cast2Exp_4_4_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+4步雙相位-3投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period3Cast2Exp_4_4GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+4GC步雙相位-3投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period3Cast2Exp_4_5GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+5GC步雙相位-3投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period3Cast2Exp_4_6GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+6GC步雙相位-3投光2曝光

__global__ void ExecCudaSolveSpace221Frames2Period4Cast2Exp_2_2_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, TCastParam *pCastParamD4, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//2+2步雙相位-4投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period4Cast2Exp_4_2_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, TCastParam *pCastParamD4, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+2步雙相位-4投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period4Cast2Exp_4_4_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, TCastParam *pCastParamD4, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4+4步雙相位-4投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period4Cast2Exp_4_4GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, TCastParam *pCastParamD4, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+4GC雙相位-4投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period4Cast2Exp_4_5GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, TCastParam *pCastParamD4, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+5GC雙相位-4投光2曝光
__global__ void ExecCudaSolveSpace4Frames2Period4Cast2Exp_4_6GC_Kernel(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, TCastParam *pCastParamD4, unsigned int ImageSize, TPhaseNoiseParam NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//4步+6GC雙相位-4投光2曝光

__global__ void ExecCudaContentAwareFilter_Kernel(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const SPACE_PTR pSpace, const MASK_PTR pMask, const IMAGE_PTR GuidedImagePtr, int KerSize, int UseSize, bool FilterSearchOn, int FilterAlphaF, int FilterAlphaS, int FilterAlphaM, int FilterAlphaI, int FilterThresdhold_Outlier, SPACE_PTR pDst);//影像濾波濾波
__global__ void ExecCudaMedianFilter_Kernel(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const SPACE_PTR pSpace, const MASK_PTR pMask, int KerSize, int UseSize, SPACE_PTR pDst);//中值濾波

__global__ void ExecCudaSolve2PhasePeriod_Kernel(double PA, const short *pPhaseDA, const unsigned char *pMaskDA, double PB, const short *pPhaseDB, const unsigned char *pMaskDB, const short *pBasePhaseD, unsigned char *pMaskD, short *pPhaseD, unsigned int ImageSize, int CombineMode);//解雙相位週期
__global__ void ExecCudaSubtractBasePhasePlane_Kernel(const short *pBasePhaseD, unsigned int ImageSize, short *pPhaseD);
__global__ void ExecCudaPhaseToSpace_Kernel(unsigned int ImageSize, const short *PhasePtrD, const float* KPtrD, float *DstPtrD);
__global__ void ExecCudaMerge2Exp2Phase_Kernel(const unsigned char *pMaskD1, const short *pPhaseD1, const unsigned char *pMaskD2, const short *pPhaseD2, unsigned char *pMaskD, short *pPhaseD, unsigned int ImageSize);//合併2曝光雙相位

__global__ void ExecCudaCopyDebugParam_Kernel(MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD);//複製Cuda偵錯參數
//------------------------------------------------------------------------------//
//Device function//-------------------------------------------------------------//
//------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------//
//Interface----------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
//解相位-------------------------------------------------------------------------//
extern "C" void DoCudaSolvePhase(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *ImgD4, unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, const int W,const int H, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue);

extern "C" void UnLinkTextureSolvePhase();

extern "C" void DoCudaSolvePhase_Three(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, const int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue, const bool IsUseTexture, const int ThreadNumber = 96, const int BlockNumber = 16);
extern "C" void DoCudaSolvePhase_Four(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *ImgD4, unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, const int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue, const bool IsUseTexture, const int ThreadNumber = 96, const int BlockNumber = 16);
extern "C" void DoCudaSolvePhase_Five(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *ImgD4, unsigned char *ImgD5, unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, const int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue, const bool IsUseTexture, const int ThreadNumber = 96, const int BlockNumber = 16);

extern "C" void DoCudaSolvePhaseAndBasePhase_Four(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *ImgD4, unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, short *pBasePhaseD, const int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue, const bool IsUseTexture, const int ThreadNumber = 96, const int BlockNumber = 16);
extern "C" void DoCudaSolveMultiPhase(int P1, int P2, short *pPhaseData1, unsigned char *pMaskMap1, short *pPhaseData2, unsigned char *pMaskMap2, int W, int H, const bool IsUseTexture, const int ThreadNumber = 96, const int BlockNumber = 16);

extern "C" void DoCudaSolvePhaseAndFFC_Three(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, float *pFFCGainD ,unsigned char *pFFCBaseD, const int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue, const bool IsUseTexture, const int ThreadNumber = 96, const int BlockNumber = 16);
extern "C" void DoCudaSolvePhaseAndFFC_Four(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *ImgD4, unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, float *pFFCGainD ,unsigned char *pFFCBaseD, const int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue, const bool IsUseTexture, const int ThreadNumber = 96, const int BlockNumber = 16);
extern "C" void DoCudaSolvePhaseAndFFC_Five(unsigned char *ImgD1, unsigned char *ImgD2, unsigned char *ImgD3, unsigned char *ImgD4, unsigned char *ImgD5, unsigned char *pMergeD, short *pPhaseD, unsigned char *pMaskD, float *pFFCGainD ,unsigned char *pFFCBaseD, const int ImageSize, unsigned char SaturationValue, unsigned char LowContrastValue, unsigned char PotentialValue, bool IsUsePotentialValue, const bool IsUseTexture, const int ThreadNumber = 96, const int BlockNumber = 16);

extern "C" void DoCudaSubtractBasePhasePlane(short *pPhaseD, short *pBasePhaseD, const int DataSize, const bool IsUseTexture, const int ThreadNumber = 96, const int BlockNumber = 16);

//影像平滑
extern "C" void DoCudaImageSmooth(unsigned char *pImage, unsigned char *pSmoothImage, const int ImageW, const int ImageH, const int MaskSize, const bool IsUseTexture, const int ThreadNumber = 96, const int BlockNumber = 16);

//相位平滑	PhaseSmooth----------------------------------------------------------//
extern "C" void DoCudaPhaseSmoothRows(short *pSrcD, unsigned char *pMaskD, float *pSinDestD, float *pCosDestD, unsigned char *pTempMaskBufferD, int DataW, int DataH, int DataSmoothMask, const bool IsUseTexture);
extern "C" void DoCudaPhaseSmoothColumns(float *pSinSrcD, float *pCosSrcD, unsigned char *pTempMaskBufferD, short *pDestD, unsigned char *pMaskD, int DataW, int DataH, int DataSmoothMask, const bool IsUseTexture);


//FFC---------------------------------------------------------------------------//
extern "C" void DoCudaFFCProcess(unsigned char *pSrc, unsigned char *pDest, float *pGain ,unsigned char *pBase,const int ImageW,int ImageH);

//Recursive Gaussian -----------------------------------------------------------//
extern "C" void DoCudaRecursiveGaussian(unsigned char *pSrc, unsigned char *pDest, unsigned char *pTemp, const int W, const int H, float sigma);
extern "C" void DoTranspose(unsigned char *pSrc, unsigned char *pDest, int width, int height);

//Average smooth
extern "C" void DoCudaAverageSmoothRows(unsigned int ImageW,unsigned int ImageH, unsigned int ImageStep, const unsigned char *d_Src, unsigned short *d_Dst, int KernelSize);
extern "C" void DoCudaAverageSmoothColumns(unsigned int ImageW,unsigned int ImageH, unsigned int ImageStep, const unsigned short *d_Src, unsigned char *d_Dst, int KernelSize);

//Image2DFilter-----------------------------------------------------------------//
extern "C" void SetConvolutionKernel(float *h_KernelRow,float *h_KernelCol,int Kernel_Length);

//2DFilter Convolution----------------------------------------------------------//
extern "C" void DoDataConvolutionRows(float *d_Dst,float *d_Src,unsigned int imageW,unsigned int imageH,int DataMask);
extern "C" void DoDataConvolutionColumns(float *d_Dst, float *d_Src,int imageW,int imageH,int DataMask);

//Kai 20161017
//解相位
extern "C" void ExecCudaBuildAtanTable(const int ThreadNumber = 96, const int BlockNumber = 16);
extern "C" void ExecCudaSolvePhase221Frames2Phase(const unsigned char *ImgDA1, const unsigned char *ImgDA2, const unsigned char *ImgDB1, const unsigned char *ImgDB2, const unsigned char *ImgDB3, const short *BasePhaseD, double PA, double PB, int ExpTimeA, int ExpTimeB, unsigned int ImageSize, float Gamma, TPhaseNoiseParam NoiseParam, unsigned char *pMaskD, short *pPhaseD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);
extern "C" void ExecCudaSolvePhase42Frames2Phase(const unsigned char *ImgDA1, const unsigned char *ImgDA2, const unsigned char *ImgDA3, const unsigned char *ImgDA4, const unsigned char *ImgDB1, const unsigned char *ImgDB2, const short *BasePhaseD, double PA, double PB, int ExpTimeA, int ExpTimeB, unsigned int ImageSize, float Gamma, TPhaseNoiseParam NoiseParam, unsigned char *pMaskD, short *pPhaseD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);

//Kai 20210226
extern "C" void ExecCudaSolvePhase44GCFrames2Phase(const unsigned char *ImgDA1, const unsigned char *ImgDA2, const unsigned char *ImgDA3, const unsigned char *ImgDA4, const unsigned char *ImgDB1, const unsigned char *ImgDB2, const unsigned char *ImgDB3, const unsigned char *ImgDB4, const short *BasePhaseD, double PA, double PB, int ExpTimeA, int ExpTimeB, unsigned int ImageSize, float Gamma, TPhaseNoiseParam NoiseParam, unsigned char *pMaskD, short *pPhaseD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);

//Kai 20210217
extern "C" void ExecCudaSolvePhase45GCFrames2Phase(const unsigned char *ImgDA1, const unsigned char *ImgDA2, const unsigned char *ImgDA3, const unsigned char *ImgDA4, const unsigned char *ImgDB1, const unsigned char *ImgDB2, const unsigned char *ImgDB3, const unsigned char *ImgDB4, const unsigned char *ImgDB5, const short *BasePhaseD, double PA, double PB, int ExpTimeA, int ExpTimeB, unsigned int ImageSize, float Gamma, TPhaseNoiseParam NoiseParam, unsigned char *pMaskD, short *pPhaseD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);

//Kai 20210225
extern "C" void ExecCudaSolvePhase46GCFrames2Phase(const unsigned char *ImgDA1, const unsigned char *ImgDA2, const unsigned char *ImgDA3, const unsigned char *ImgDA4, const unsigned char *ImgDB1, const unsigned char *ImgDB2, const unsigned char *ImgDB3, const unsigned char *ImgDB4, const unsigned char *ImgDB5, const unsigned char *ImgDB6, const short *BasePhaseD, double PA, double PB, int ExpTimeA, int ExpTimeB, unsigned int ImageSize, float Gamma, TPhaseNoiseParam NoiseParam, unsigned char *pMaskD, short *pPhaseD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);

extern "C" void ExecCudaSolvePhase4Frames(const unsigned char *ImgD1, const unsigned char *ImgD2, const unsigned char *ImgD3, const unsigned char *ImgD4, short *pPhaseD, unsigned char *pMaskD, unsigned int ImageSize, int PatternID, float Gamma, TPhaseNoiseParam NoiseParam, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);
extern "C" void ExecCudaSolvePhase4Frames2Phase(const unsigned char *ImgDA1, const unsigned char *ImgDA2, const unsigned char *ImgDA3, const unsigned char *ImgDA4, const unsigned char *ImgDB1, const unsigned char *ImgDB2, const unsigned char *ImgDB3, const unsigned char *ImgDB4, const short *BasePhaseD, double PA, double PB, unsigned int ImageSize, float Gamma, TPhaseNoiseParam NoiseParam, unsigned char *pMaskD, short *pPhaseD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);

extern "C" void ExecCudaMergeSpace4Frames2Period2Cast(TCastParam *pCastParamD1, TCastParam *pCastParamD2, unsigned int ImageSize, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);
extern "C" void ExecCudaMergeSpace4Frames2Period3Cast(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, unsigned int ImageSize, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);
extern "C" void ExecCudaMergeSpace4Frames2Period4Cast(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, TCastParam *pCastParamD4, unsigned int ImageSize, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);

extern "C" void ExecCudaSolveSpace4Frames2Period1Cast(TCastParam *pCastParamD, unsigned int ImageSize, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber, int DecodeMode);
extern "C" void ExecCudaSolveSpace4Frames2Period2Cast(TCastParam *pCastParamD1, TCastParam *pCastParamD2, unsigned int ImageSize, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber, int DecodeMode);
extern "C" void ExecCudaSolveSpace4Frames2Period3Cast(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, unsigned int ImageSize, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber, int DecodeMode);
extern "C" void ExecCudaSolveSpace4Frames2Period4Cast(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, TCastParam *pCastParamD4, unsigned int ImageSize, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber, int DecodeMode);

extern "C" void ExecCudaSolveSpace4Frames2Period1Cast2Exp(TCastParam *pCastParamD, unsigned int ImageSize, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber, int DecodeMode);
extern "C" void ExecCudaSolveSpace4Frames2Period2Cast2Exp(TCastParam *pCastParamD1, TCastParam *pCastParamD2, unsigned int ImageSize, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber, int DecodeMode);
extern "C" void ExecCudaSolveSpace4Frames2Period3Cast2Exp(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, unsigned int ImageSize, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber, int DecodeMode);
extern "C" void ExecCudaSolveSpace4Frames2Period4Cast2Exp(TCastParam *pCastParamD1, TCastParam *pCastParamD2, TCastParam *pCastParamD3, TCastParam *pCastParamD4, unsigned int ImageSize, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber, int DecodeMode);

extern "C" void ExecCudaContentAwareFilter(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const float *pSpace, const unsigned char *pMask, const unsigned char *GuidedImagePtr, int KerSize, int UseSize, bool FilterSearchOn, int FilterAlphaF, int FilterAlphaS, int FilterAlphaM, int FilterAlphaI, int FilterThresdhold_Outlier, float *pDst, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);
extern "C" void ExecCudaMedianFilter(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const float *pSpace, const unsigned char *pMask, int KerSize, int UseSize, float *pDst, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);
// KernelSize : 3 ~ 11
extern "C" void ExecCudaMedianFilter_Joe(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const float *pSpace, const unsigned char *pMask, int KerSize, int UseSize, float *pDst, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);
extern "C" void ExecCudaSpaceFilter_Joe(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, float *pSpace, const unsigned char *pMask, int Mode, int KerSize, int UseSize, float *pDst, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);

extern "C" void ExecCudaSolve2PhasePeriod(double PA, const short *pPhaseDA, const unsigned char *pMaskDA, double PB, const short *pPhaseDB, const unsigned char *pMaskDB, const short *pBasePhaseD, unsigned char *pMaskD, short *pPhaseD, unsigned int ImageSize, int  CombineMode, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);
extern "C" void ExecCudaSubtractBasePhasePlane(const short *pBasePhaseD, unsigned int DataSize, short *pPhaseD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);
extern "C" void ExecCudaPhaseToSpace(unsigned int ImageSize, const short *PhasePtrD, const float* KPtrD, float *DstPtrD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);//相位乘上K值變成空間數值	
extern "C" void ExecCudaMerge2Exp2Phase(const unsigned char *pMaskD1, const short *pPhaseD1, const unsigned char *pMaskD2, const short *pPhaseD2, unsigned char *pMaskD, short *pPhaseD, unsigned int ImageSize, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);

extern "C" void ExecCudaCopyDebugParam(MASK_PTR MaskPtrD, SPACE_PTR SpacePtrD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber);
#endif //#ifndef CUDA_USE

#endif // #ifndef _CUDA_KERNEL_H_


