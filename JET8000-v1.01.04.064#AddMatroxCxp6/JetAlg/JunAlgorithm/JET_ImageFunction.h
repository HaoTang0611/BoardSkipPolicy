#pragma once

#define JET_MACHINE_7000 7000
#define JET_MACHINE_8000 8000
#define JET_MEAUSREMENT_UI 9999

#define FOR_MACHINE JET_MACHINE_8000

#ifdef FOR_MACHINE
	#if (FOR_MACHINE == JET_MACHINE_7000)
		#include "Jet_Struc.h"
	#elif (FOR_MACHINE == JET_MEAUSREMENT_UI)
		#include <opencv2/opencv.hpp>
		#ifdef _DEBUG
		#pragma comment(lib, "C:\\opencv_24136\\x64\\vc14\\lib\\opencv_imgproc2413d.lib")
		#pragma comment(lib, "C:\\opencv_24136\\x64\\vc14\\lib\\opencv_core2413d.lib")
		#pragma comment(lib, "C:\\opencv_24136\\x64\\vc14\\lib\\opencv_highgui2413d.lib")
		#else
		#pragma comment(lib, "C:\\opencv_24136\\x64\\vc14\\lib\\opencv_imgproc2413.lib")
		#pragma comment(lib, "C:\\opencv_24136\\x64\\vc14\\lib\\opencv_core2413.lib")
		#pragma comment(lib, "C:\\opencv_24136\\x64\\vc14\\lib\\opencv_highgui2413.lib")
		#endif
	#elif ( FOR_MACHINE == JET_MACHINE_8000 )
	#include "OpenCV_Def.h"
	#else
		#include "opencv.hpp"

		#ifdef _DEBUG
		#pragma comment(lib, "C:/OpenCV_3_4_16/x64/Lib/vc14/opencv_world3416d.lib")
		#else
		#pragma comment(lib, "C:/OpenCV_3_4_16/x64/Lib/vc14/opencv_world3416.lib")
		#endif
	#endif // FOR_MACHINE == JET_MACHINE_7000
#endif // FOR_MACHINE

#include <vector>
#include <string>
#include <thread>
#include <sstream>
#include <iomanip>
#include "JET_ImageModule.h"

using namespace JET::mod;
using namespace std;
using namespace cv;


class CPU_Times
{

public:
	CPU_Times();
	~CPU_Times();


private:
	LARGE_INTEGER	nFreq;
	LARGE_INTEGER	nStartTime;
	LARGE_INTEGER	nEndTime;

	double			dDiffTimes;
	int				nMode;

public:
	void Start();
	bool End();
	double GetTime();
};

// 色彩的資料結構
struct stCOLOR_RGB
{
	uchar blue;
	uchar green;
	uchar red;
};

namespace jet_imagefunction
{
#pragma region ProcessMode

	struct SProcessModeData
	{
	public:
		Mat matImage;
		Mat matMask;
		Rect cvROI;
		SContoursParam* psContours;

		vector<Mat>		vtmatImage;
		vector<Mat>		vtmatMask;
		vector<Rect>*	pvtcvROI;
		vector<SContoursParam*> vtpsContours;
		SProcessModeData() :cvROI(0, 0, 0, 0), psContours(nullptr) {}
		~SProcessModeData() = default;
	};

	struct SProcessModeInput
	{
	public:
		string strInfo;
		SProcessModeData sData1;
		SProcessModeData sData2;

		SProcessModeInput() : sData1(), sData2() {}
		~SProcessModeInput() = default;
	};

	struct SProcessModeOutput
	{
	public:
		bool bResult;
		Mat matImage;
		Rect cvROI;
		SContoursParam sContours;

		vector<Mat> vtmatImage;
		vector<Rect> vtcvROI;
		vector<SContoursParam> vtsContours;

		SProcessModeOutput() : bResult(false), cvROI(0, 0, 0, 0) {}
		~SProcessModeOutput() = default;

		bool Check() const {
			if (matImage.empty() && (cvROI.width == 0 || cvROI.height == 0) && !sContours.Check() && vtmatImage.size() <= 0 && vtcvROI.size() <= 0 && vtsContours.size() <= 0) {
				return false;
			}
			return true;
		}
	};

	bool SetInputData(const vector<Mat>& vtmatSrc, vector<SProcessModeOutput>& vtsProcessResult, const int nIndex, vector<BaseParameter*> vtpsBaseParam, SProcessModeInput& sInputData);
	bool SetInputData_Node(const vector<Mat>& vtmatSrc, vector<SProcessModeOutput>& vtsProcessResult, const int nIndex, SProcessModeParam& sPMParam, SProcessModeInput& sInputData);

	bool RunProcessMode(const vector<Mat>& vtmatSrc, const SProcessModeParam& sPMParam, vector<SProcessModeOutput>& vtsResult);
	bool RunProcessMode_Node(const vector<Mat>& vtmatSrc, SProcessModeParam& sPMParam, vector<SProcessModeOutput>& vtsResult);
	bool RunCustomProcessMode(const int& nStartIndex, const int& nEndIndex, const vector<Mat>& vtmatSrc, SProcessModeParam& sPMParam, vector<SProcessModeOutput>& vtsResult);

	// nExtensionType = 1, jpg
	//                = 2, bmp
	//                = 3, png
	bool SaveProcessResult(const string& strPathName, const string& strSubName, const SProcessModeParam& sPMParam, vector<Mat>& vtmatImage, const vector<SProcessModeOutput>& vtsResult, int nExtensionType = 1, bool bDrawRect = false);
	bool SaveProcessResult(const string& strPathName, const string& strSubName, const SProcessModeParam& sPMParam, vector<Mat>& vtmatImage, const vector<bool>& vtbSaveInput, const vector<SProcessModeOutput>& vtsResult, const vector<bool>& vtbSaveResult, int nExtensionType = 1);

	// nExtensionType = 1, jpg
	//                = 2, bmp
	//                = 3, png
	bool SaveProcessResult_Node(const string& strPathName, const string& strSubName, SProcessModeParam& sPMParam, vector<Mat>& vtmatImage, const vector<SProcessModeOutput>& vtsResult, int nExtensionType = 1, bool bDrawRect = false);

	bool SaveProcessModeImage(const string& strPathName, const string& strSubName, const SProcessModeOutput& sResult, int nExtensionType = 1, bool bDrawRect = false);
	string GetColorTransformInfo(const SColorTransformParam& sParam);
	string GetImageEnhanceInfo(const SImageEnhanceParam& sParam);
	string GetFilterParamInfo(const SFilterParam& sParam);
	string GetThresholdParamInfo(const SThresholdParam& sParam);
	string GetMorphologParamInfo(const SMorphologParam& sParam);
	string GetContoursShapeParamInfo(const SContoursShapeParam& sParam);
	string GetDeleteObjectParamInfo(const SDeleteObjectParam& sParam);
	string GetImageCalculatorParamInfo(const SImageCalculatorParam& sParam);
	string GetFeatureAnalyzeParamInfo(const SFeatureAnalyzeParam& sParam);
#pragma endregion

#pragma region ColorTransform

	// 色彩轉換模式 : COLOR_TO_NONE = 0, 不處理 
	//              : GRAY_TO_BGR = 1, 灰階轉彩色(BGR)
	//              : COLOR_TO_NEGATIVE = 2, 負片處理
	//              : COLOR_TO_GRAY_AVERAGE = 3, BRG to Gray, 用 (R+G+B)/3 轉灰階	 
	//              : COLOR_TO_GRAY_COEFFICIENT = 4, BRG to Gray, 用 R*0.299 + G*0.587 + B*0.114 轉灰階		 
	//              : COLOR_TO_GRAY_MINIMUM = 5, BRG to Gray, 用 min(R,G,B) 轉灰階
	//              : COLOR_TO_GRAY_MAXIMUM = 6, BRG to Gray, 用 max(R,G,B) 轉灰階
	//              : COLOR_TO_GRAY_STD = 7, BRG to Gray, 計算 R G B 影像各自的標準差, 選標準差最大的分量
	//              : COLOR_TO_GRAY_PCA = 8, BRG To Gray, 使用 PCA轉灰階	nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值, 須設定sPca中的nType_Eigenvectors
	//              : COLOR_TO_GRAY_PCA_MULTIPLE = 9, BRG To Gray, 多張彩色影像, 使用 PCA轉灰階 nID=[0,2]會輸出最大-第三大主成分的影像, nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值, 須設定sPca中的nType_Eigenvectors
	//              : COLOR_TO_GRAY_MASKPCA = 10, BRG To Gray, 使用 Mask + PCA轉灰階	nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值, 須設定sPca中的nType_Eigenvectors
	//				: COLOR_TO_GRAY_KMEANPCA = 11, BRG To Gray, 使用 Kmean + PCA 轉灰階 nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值, 須設定sPca中的nType_Eigenvectors 與 sKmean中的nGroup以及fScale	
	//              : COLOR_TO_GRAY_NEGATIVE_PCA = 12, 彩色影像轉負片再使用 PCA轉灰階 nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值, 須設定sPca中的nType_Eigenvectors
	//              : COLOR_TO_GRAY_NEGATIVE_PCA_MULTIPLE = 13, 多張彩色影像轉負片再使用 PCA轉灰階 nID=[0,2]會輸出最大-第三大主成分的影像, nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值,須設定sPca中的nType_Eigenvectors	
	//              : COLOR_TO_GRAY_NEGATIVE_MASKPCA = 14, 彩色影像轉負片再使用 Mask + PCA轉灰階 nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值, 須設定sPca中的nType_Eigenvectors
	//				: COLOR_TO_GRAY_NEGATIVE_KMEANPCA = 15, 彩色影像轉負片再使用 Kmean + PCA轉灰階 nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值, 須設定sPca中的nType_Eigenvectors 與 sKmean中的nGroup以及fScale			
	//              : COLOR_TO_GRAY_MINCROSSENTROPY = 16, BRG To Gray, 對每個 Channel 計算最小交叉熵, 若nID設定為 0則輸出熵最大的Channel
	//              : COLOR_TO_GRAY_BGR = 17, BGR To BGR, 指定 B G R其中一個分量		 
	//              : COLOR_TO_XYZ = 18, BRG To XYZ	
	//              : COLOR_TO_HSV = 19, BRG To HSV	
	//              : COLOR_TO_LUV = 20, BRG To LUV	
	//              : COLOR_TO_HLS = 21, BRG To HLS	
	//              : COLOR_TO_LAB = 22, BRG To LAB	
	//              : COLOR_TO_YUV = 23, BRG To YUV	
	//				: COLOR_FINAL = 24,
	bool ColorTransform(const SProcessModeInput sInput, SColorTransformParam& sParam, SProcessModeOutput& sOutput);
	
	// 將彩色影像以pixels為單位, 對不同channel取最小值
	bool BGRToGray_Min(const Mat& matBGR, Mat& matOutput);

	// 將彩色影像以pixels為單位, 對不同channel取最大值
	bool BGRToGray_Max(const Mat& matBGR, Mat& matOutput);

	// 將彩色影像以channel為單位, 計算標準差, 再取標準差最大的channel當灰階
	bool BGRToGray_Std(const Mat& matBGR, Mat& matOutput);

	// 將彩色影像乘以特定係數轉灰階
	// [Bx0.2989, Gx0.5870, Rx0.1140]
	bool BGRToGray_Coefficient(const Mat& matBGR, Mat& matOutput);

	// 將彩色影像以channel為單位, 計算最小交叉熵, 再依設定取對應的channel當灰階
	// nMode = 0, 輸出最大熵對應的channel
	// nMode = 1, 輸出中間的熵對應的channel
	// nMode = 2, 輸出最小熵對應的channel
	bool BGRToGray_MinCrossEntropy(const Mat& matBGR, const int& nMode, Mat& matOutput);

	// 輸入三維向量, 將彩色影像轉灰階
	// alpha(R), beta(G), gamma(B) : 三維空間的方向
	bool BGRToGray_Vector(const Mat& matBGR, const float& alpha, const float& beta, const float& gamma, Mat& matOutput);

	// 彩色影像使用PCA轉灰階
	// nMode=1 => 依 nId(0,1,2)設定來輸出, nId = 0是最大主成分, nId = 3是三個主成分都輸出
	//      =2 => 對3個主成分影像取最小值輸出
	//      =3 => 對3個主成分影像取最大值輸出
	bool BGRToGray_PCA(const Mat& matColor, const int nMode, const int nId, Mat& matOutput);
	bool BGRToGray_PCA(const Mat& matColor, const SPcaParameter& sPca, const int nMode, const int nId, Mat& matOutput);
	bool BGRToGray_PCA(const vector<Mat>& vtmatColor, const bool& bNegative, const SPcaParameter& sPca, const int nMode, const int nId, Mat& matOutput);

	// 彩色影像使用 Mask + PCA 轉灰階
	// nEigenvectorsType : 設定特徵向量模式, 0 => 不使用
	//										 1 => 主成分特徵向量必須為正
	//										 2 => 主成分特徵向量必須為負
	// nMode=1 => 依 nId(0,1,2)設定來輸出, nId = 0是最大主成分, nId = 3是三個主成分都輸出
	//      =2 => 對3個主成分影像取最小值輸出
	//      =3 => 對3個主成分影像取最大值輸出
	bool BGRToGray_MaskPCA(const Mat& matColor, const Mat& matMask, const int& nEigenvectorsType, const int nMode, const int nId, Mat& matOutput);

	// 彩色影像使用 Kmean + PCA轉灰階
	// nGroup : 群數(至少為2)
	// nMode=1 => 依 nId(0,1,2)設定來輸出, nId = 0是最大主成分, nId = 3是三個主成分都輸出
	//      =2 => 對3個主成分影像取最小值輸出
	//      =3 => 對3個主成分影像取最大值輸出
	bool BGRToGray_KmeanPCA(const Mat& matColor, const int& nEigenvectorsType, const int nMode, const int nId, SKmeanParameter& skmean, Mat& matOutput);

	// 彩色+負片使用PCA轉灰階
	// nMode=1 => 依 nId(0,1,2)設定來輸出, nId = 0是最大主成分, nId = 3是三個主成分都輸出
	//      =2 => 對3個主成分影像取最小值輸出
	//      =3 => 對3個主成分影像取最大值輸出
	bool NegativeToGray_PCA(const Mat& matColor, const int nMode, const int nId, Mat& matOutput);
	bool NegativeToGray_PCA(const Mat& matColor, const int& nEigenvectorsType, const int nMode, const int nId, Mat& matOutput);

	// 彩色+負片使用 Mask + PCA 轉灰階
	// nMode=1 => 依 nId(0,1,2)設定來輸出, nId = 0是最大主成分, nId = 3是三個主成分都輸出
	//      =2 => 對3個主成分影像取最小值輸出
	//      =3 => 對3個主成分影像取最大值輸出
	// nEigenvectorsType = 設定特徵向量模式, 0 => 不使用
	//                                       1 => 主成分特徵向量必須為正
	//                                       2 => 主成分特徵向量必須為負
	bool NegativeToGray_MaskPCA(const Mat& matColor, const Mat& matMask, const int& nEigenvectorsType, const int nMode, const int nId, Mat& matOutput);

	// 彩色+負片使用 Kmean + PCA 轉灰階
	// nGroup : 群數(至少為2)
	// nMode=1 => 依 nId(0,1,2)設定來輸出, nId = 0是最大主成分, nId = 3是三個主成分都輸出
	//      =2 => 對3個主成分影像取最小值輸出
	//      =3 => 對3個主成分影像取最大值輸出
	bool NegativeToGray_KmeanPCA(const Mat& matColor, const int& nEigenvectorsType, const int& nMode, const int& nId, SKmeanParameter& skmean, Mat& matOutput);

	// 彩色影像轉灰階使用 RBM
	bool BGRToGray_RBM(const Mat& matColor, const int& nnPatchSize, const int& nnStride, Mat& matOutput);

	// 彩色轉灰階
	// nMode 若為0, 灰階 轉 彩色(BRG)
	// nMode 若為1, 用 (R+G+B)/3 轉灰階
	// nMode 若為2, 用 R*0.299 + G*0.587 + B*0.114 轉灰階
	// nMode 若為3, BGR to BGR, nOutputID=0輸出B分量, nOutputID=1輸出G分量, nOutputID=2輸出R分量
	// nMode 若為4, BGR to XYZ, nOutputID=0輸出X分量, nOutputID=1輸出Y分量, nOutputID=2輸出Z分量
	// nMode 若為5, BGR to HSV
	// nMode 若為6, BGR to Luv
	// nMode 若為7, BGR to HLS  
	// nMode 若為8, BGR to Lab						
	// nMode 若為9, BGR to YUV
	// nMode 若為10,用 min(R,G,B) 轉灰階
	// nMode 若為11,用 max(R,G,B) 轉灰階
	// nMode 若為12,計算 R G B 影像各自的標準差, 選標準差最大的分量
	// nOutputID : 若nMode=5, nOutputID=0,色彩轉換後會輸出 H 分量;
	//             若nMode=5, nOutputID=1,色彩轉換後會輸出 S 分量;
	//             若nMode=5, nOutputID=2,色彩轉換後會輸出 V 分量;
	//             若nMode=5, nOutputID=3,色彩轉換後會將 HSV 合併成3 channel的影像;
	int ColorToGray(int nMode, Mat &matInput, int nOutputID, Mat &matOutput);
#pragma endregion

#pragma region ImageEnhan

	// 影像強化
	// matSrc : 彩色或灰階皆可
	// 影像增強模式 : ENHANCE_NONE = 0, 不處理
	//              : ENHANCE_GAUSSIANBLUR = 1, 高斯模糊
	//              : ENHANCE_LOG = 2, Log轉換1
	//              : ENHANCE_LOG_STD = 3, Log轉換2
	//              : ENHANCE_EXP = 4, 指數轉換
	//              : ENHANCE_EQUALIZE = 5, 直方圖均化
	//              : ENHANCE_EQUALIZE_MASK = 6, 直方圖均化, 搭配遮罩	
	//              : ENHANCE_NORMALIZE = 7, 拉伸到0-255
	//              : ENHANCE_NORMALIZE_MASK = 8, 拉伸到0-255, 搭配遮罩	
	//				: ENHANCE_CONTRAST_KMEAN = 9, 目前只支援灰階影像, 使用k-mean做對比強化, 須設定 sKmean
	//              : ENHANCE_CONTRAST_KMEAN_MASK = 10, 使用k-mean做對比強化, 搭配遮罩	
	//              : ENHANCE_CONTRAST_DBSCAN = 11, 目前只支援灰階影像, 使用DBScan做對比強化(fEps = nRadius ; fSigma = nMinNumber)
	//              : ENHANCE_CONTRAST_DBSCAN_MASK = 12, 使用DBScan做對比強化, 搭配遮罩	
	//              : ENHANCE_EDGE = 13, 邊界強化(要設定 fEps=增強係數, fSigma=模糊係數)
	//              : ENHANCE_RETINEX = 14, Retinex 影像強化(要設定fEps, fSigma, nOffset=偏移量)
	//              : ENHANCE_UNIFORMITY_PCA = 15, 使用 pca 進行均勻度校正(nOffset = nInterval)
	//              : ENHANCE_UNIFORMITY_CLAHE = 16, 使用 CLAHE 進行亮度均衡 (nOffset=nBlockW, fEps=nBlockH, fSigma=fClipLimit)
	//              : ENHANCE_NONLINEAR = 17, 使用非線性轉換 nOffset=模式, fEps = alpha
	//              : ENHANCE_DECENTRALIZATION_PCA = 18, 使用 PCA 對原始影像進行去中心化 (nOffset=nMode, fEps=fCoefficient, fSigma=nEigenvectorsType)
	//				: ENHANCE_DECENTRALIZATION_KMEANPCA = 19, 使用 Kmean + PCA 對原始影像進行去中心化 (nOffset=nMode, fEps=fCoefficient, fSigma=nEigenvectorsType)
	//              : ENHANCE_FINAL = 20,
	bool ImageEnhan(const SProcessModeInput sInput, SImageEnhanceParam& sParam, SProcessModeOutput& sOutput);
	
	// 直方圖均化, 使用遮罩
	bool Equalize_Mask(const Mat &matImage, const Mat& matMask, Mat &matDst);

	// 影像拉伸到0-255, 使用遮罩
	bool Normalize_Mask(const Mat &matImage, const Mat& matMask, Mat &matDst);

	// Log 轉換
	bool LogTransform(const Mat& matSrc, Mat& matDst);
	bool LogTransform_STD(const Mat& matSrc, Mat& matDst);

	// Exp 轉換
	bool ExpTransform(const Mat& matSrc, Mat& matDst);

	// 強化影像邊緣
	// nModel : 0 => 使用高斯模糊
	//        : 1 => 使用平均濾波
	// fEps = 強化系數, 值越大效果越強
	// fSigma = 當nModel=0時, fSigma為高斯函數的標準差, 值越大效果越強
	//          當nModel=1時, fSigma為平均濾波的 filter Size
	int EdgeEnhan(const Mat &matSrc, Mat &matDst, int nModel, float eps, float fSigma);

	// PCA 均勻度校正
	// matSrc : 彩色影像和灰階影像皆可
	// nInterval : 取樣間隔
	// bUseGrayForPCA : true=>輸入影像是彩色時會先轉灰階再計算pca ;  false=>輸入影像是彩色時會每個channel各自進行pca
	bool BrightnessUniformityCorrection_PCA(const cv::Mat& matSrc, cv::Mat& matOutput);

	// nBlockRows : Y方向區塊數
	// nBlockCols : X方向區塊數
	// fOverlapRatio : 區塊間的重疊率
	bool BrightnessUniformityCorrection_BlockPCA(const cv::Mat& matSrc, cv::Mat& matOutput, int nBlockRows = 4, int nBlockCols = 4, float fOverlapRatio = 0.25f);
	bool BrightnessUniformityCorrection(const cv::Mat& matSrc, const bool bUseGrayForPCA, cv::Mat& matOut);

	// CLAHE（Contrast Limited Adaptive Histogram Equalization）亮度均勻校正
	// matSrc : 彩色影像和灰階影像皆可
	// nBlockW : 區塊寬度, 建議值 8,16
	// nBlockH : 區塊高度, 建議值 8,16
	// fClipLimit : 值越大效果越好, 建議值[1.0 , 4.0], 不可小於0
	// matOutput : 結果影像
	bool BrightnessUniformityCorrection_CLAHE(const Mat& matSrc, const int& nBlockW, const int& nBlockH, const float& fClipLimit, Mat& matOutput);
	
	// 使用k-mean做對比強化(將原圖縮小後計算強化參數, 再對原圖做強化)
	// nCount_Group = 分群數, 必需 >=2
	// nMinId = 最小值的 Id, 若為1, 則以vtdMean[1]為最小值
	// nMaxId = 最大值的 Id, 若為4, 則以vtdMean[4]為最大值
	bool EnhanContrast_Kmean(const Mat &matGray, const int& nCount_Group, const int& nMinId, const int& nMaxId, vector<double>& vtdMean, Mat &matDst);
	bool EnhanContrast_Kmean(const Mat &matGray, SKmeanParameter& sKmean, Mat &matDst);
	bool EnhanContrast_Kmean_Mask(const Mat &matGray, const Mat& matMask, SKmeanParameter& sKmean, Mat &matDst);

	// 使用DBScan做對比強化
	bool EnhanContrast_DBScan(const Mat &matGray, const int &nRadius, const int& nMinNumber, Mat &matDst);
	bool EnhanContrast_DBScan(const Mat& matImage, SDbscanParameter& sDbscan, Mat &matDst);
	bool EnhanContrast_DBScan_Mask(const Mat& matImage, const Mat& matMask, SDbscanParameter& sDbscan, Mat &matDst);

	// 彩色影像強化
	// Retinex-SSR ; 來源 : https://blog.csdn.net/ajianyingxiaoqinghan/article/details/71435098
	// matSrc : 輸入影像
	// matDst : 結果影像
	// bLog : 若為 true, 會進行log轉換
	// nSmoothSize : 高斯濾波大小
	// nGain : 增益大小
	// nOffset : 偏移距離
	int Retinex(const Mat &matSrc, Mat &matDst, const bool& bLog, const int& nSmoothSizeX, const int& nSmoothSizeY, const int& nGain = 128, const int& nOffset = 128);

	// 將 matImage2 的平均值 標準差 校正成 與 matImage1 一樣
	// matImage1 彩色 或 灰階 皆可
	// matImage2 格式要與 matImage1 一致
	// 校正後的影像會覆蓋 matImage2
	// nMaxError : 誤差大於此值不被計算
	bool MatchMeanStd(const int& nMaxDiff, const Mat& matImage1, Mat& matImage2);

	// 直方圖規定化---ok
	// 讓matInput2的直方圖與matInput1的直方圖一樣
	// bContrastEnhan : 若為true, 則matInput1與matInput2都會先進行均化,再做規定化
	int HistMatch(const Mat &matInput1, const Mat &matInput2, Mat &matOutput, const bool bContrastEnhan = false);

	// 區域直方圖均化.
	int ROI_Equalization(int iImageW, int iImageH, RECT rect, unsigned char *InputImage, unsigned char *OutputImage);

	// 多重區域直方圖均化.
	// nGapW = window 水平間隔距離.
	// nGapH = window 垂直間隔距離.
	int MulROI_Equalization(int nSumArea, int nAreaID, int nWindowSize_W, int nWindowSize_H, int nGapW, int nGapH, Mat &Input, Mat &matSumData, Mat &matSumCount);

	// 多重區域直方圖均化_平行處理版
	// nGridCount_X : X方向區塊數
	// nGridCount_Y : Y方向區塊數
	// fScale : 縮放倍率
	// fRatio : 與原始影像的融合比率
	int MulROI_Equalization_Parallel(int nGridCount_X, int nGridCount_Y, float fScale, float fRatio, Mat matInput, Mat &matOutput);

	// 去中心化---使用PCA
	// nEigenvectorsType : 設定特徵向量模式, 0 => 不使用
	//										 1 => 主成分特徵向量必須為正
	//										 2 => 主成分特徵向量必須為負
	// nMode : 1=>去除最大主成分
	//       : 2=>去除第2大主成分
	//       : 3=>去除最小主成分
	//       : 4=>去除(1+2)主成分
	//       : 5=>去除(1+3)主成分
	//       : 6=>去除(2+3)主成分
	bool Decentralization_PCA(const cv::Mat& matInput, const int nMode, const int& nEigenvectorsType, cv::Mat& matOutput, const float fCoefficient = 0.0f);

	// 去中心化---使用kmeanPCA
	// nEigenvectorsType : 設定特徵向量模式, 0 => 不使用
	//										 1 => 主成分特徵向量必須為正
	//										 2 => 主成分特徵向量必須為負
	// nMode : 1=>去除最大主成分
	//       : 2=>去除第2大主成分
	//       : 3=>去除最小主成分
	//       : 4=>去除(1+2)主成分
	//       : 5=>去除(1+3)主成分
	//       : 6=>去除(2+3)主成分
	bool Decentralization_KmeanPCA(const cv::Mat& matInput, int nMode, const int& nEigenvectorsType, SKmeanParameter& skmean, cv::Mat& matOutput, const float fCoefficient = 0.0f);

	// 非線性方法列舉定義
	enum NonLinearMethod
	{
		NL_SIGMOID = 1,     // S 型壓縮
		NL_TANH,            // 雙曲正切，對比增強
		NL_RELU,            // 截斷負值
		NL_LEAKY_RELU,      // 保留負值斜率
		NL_SOFTPLUS,        // 平滑 ReLU
		NL_GAMMA,           // gamma 強化
	};

	// 非線性色彩轉換
	// matInput : 輸入影像，可為灰階或彩色
	// fCoefficient : Sigmoid=>[2.0 , 10.0] ; Tanh=>[1.0 , 5.0] ; Leaky ReLU=>[0.01 , 0.2] ; Gamma=>[0.3 , 2.5]
	// fCoefficient : ReLU=>無需設置, Softplus=>無需設置
	// fMean : 若小於0 => 使用圖像本身的灰階平均值去中心化, 若大於0=>fMean就是指定的去中心值
	bool ApplyAdvancedNonLinearActivation(const cv::Mat& matInput, const NonLinearMethod eMethod, cv::Mat& matOutput, const float fCoefficient = 1.0f, const float fMean = -1.0f);
#pragma endregion

#pragma region CreateStructuringElement

	// 建立結構
	bool CreateStructuringElement(const EElementShape& eElement, const int& nSizeX, const int& nSizeY, Mat& matElement);

	// 建立斜線結構
	// nMode : 1=45度斜線 ; 2=135度斜線
	// nSize : 結構大小, 須為奇數
	// nWidth : 線的寬度
	bool CreateStructuringElement_Tilted(const int& nMode, const int& nSize, const int& nWidth, Mat& matElement);

#pragma endregion

#pragma region ImageFilter

	// 濾波的結構元素 : ELEMENT_NONE = 0, 不處理
	//                : ELEMENT_RECT = 1, 矩形
	//                : ELEMENT_CROSS = 2, 交叉
	//                : ELEMENT_ELLIPSE = 3, 橢圓
	//                : ELEMENT_RECT_TILTED_45 = 4, 矩形---45度斜線(nSizeX=矩形大小, nSizeY=線寬)
	//                : ELEMENT_RECT_TILTED_135 = 5, 矩形---135度斜線(nSizeX=矩形大小, nSizeY=線寬)
	// 影像濾波模式     : FILTER_NONE = 0, 不處理, 
	//                  : FILTER_AVERAGE = 1, 均值濾波
	//                  : FILTER_GAUSSIAN_BLUR = 2, 高斯平滑濾波 (不需設定 EElementShape, 須輸入nSizeX 與 nSizeY)
	//                  : FILTER_GABOR = 3, Gabor 濾波 (不需設定 EElementShape, 須輸入nSizeX=nGaborRadius, nSizeY=nModel)
	//                  : FILTER_MEDIAN = 4, 中值濾波, (不需設定 EElementShape, 須輸入nSizeX)
	//					: FILTER_MIN = 5, 最小值濾波
	//					: FILTER_MAX = 6, 最大值濾波
	//				    : FILTER_MINMAX = 7, 先最小值再最大值
	//					: FILTER_MAXMIN = 8, 先最大值再最小值
	//					: FILTER_MINMAXMAXMIN = 9, 4 + 5
	//					: FILTER_MAXMINMINMAX = 10, 5 + 4
	//					: FILTER_GRADIENT = 11, 梯度
	//                  : FILTER_EDGE = 12, 邊界 = 原始影像 - 最小濾波影像
	//                  : FILTER_SOBEL = 13, Sobel 濾波	
	//					: FILTER_PCA = 14, PCA 反投影(nSizeX:樣本排列 0=Rows, 1=Cols ; nSizeY:成分百分比 1=10%, 2=20%...10=100%, nTimes無作用)
	//                  : FILTER_FINAL = 15
	bool ImageFilter(const SProcessModeInput sInput, JET::mod::SFilterParam& sParam, SProcessModeOutput& sOutput);
	
	// 平均濾波
	// matMask : 遮罩, 可為空=>整張都會處理
	// nFilterSizeX, nFilterSizeY : 濾波的大小 
	// nTimes : 執行次數
	bool Average_Filter(const cv::Mat& matInput, const Mat& matMask, const int& nFilterSizeX, const int& nFilterSizeY, const int& nTimes, Mat& matOutput);

	// 高斯模糊
	// matMask : 遮罩, 可為空=>整張都會處理
	// nFilterSizeX, nFilterSizeY : 濾波的大小 
	// nTimes : 執行次數
	bool GaussianBlur_Filter(const cv::Mat& matInput, const Mat& matMask, const int& nFilterSizeX, const int& nFilterSizeY, const int& nTimes, Mat& matOutput);

	// 中值濾波
	// matMask : 遮罩, 可為空=>整張都會處理
	// nFilterSizeX, nFilterSizeY : 濾波的大小 
	// nTimes : 執行次數
	bool Median_Filter(const cv::Mat& matInput, const Mat& matMask, const int& nFilterSize, const int& nTimes, Mat& matOutput);

	// 最小值濾波
	// matMask : 遮罩, 可為空=>整張都會處理
	// matElement : 濾波結構 
	// nTimes : 執行次數
	bool Min_Filter(const cv::Mat& matInput, const Mat& matMask, const Mat& matElement, const int& nTimes, Mat& matOutput);

	// 最大值濾波
	// matMask : 遮罩, 可為空=>整張都會處理
	// matElement : 濾波結構 
	// nTimes : 執行次數
	bool Max_Filter(const cv::Mat& matInput, const Mat& matMask, const Mat& matElement, const int& nTimes, Mat& matOutput);

	// 最小最大值濾波
	// matMask : 遮罩, 可為空=>整張都會處理
	// matElement : 濾波結構 
	bool MinMax_Filter(const cv::Mat& matInput, const Mat& matMask, const Mat& matElement, Mat& matOutput);

	// 最大最小值濾波
	// matMask : 遮罩, 可為空=>整張都會處理
	// matElement : 濾波結構 
	bool MaxMin_Filter(const cv::Mat& matInput, const Mat& matMask, const Mat& matElement, Mat& matOutput);

	// 最小最大值 + 最大最小值 濾波
	// matMask : 遮罩, 可為空=>整張都會處理
	// matElement : 濾波結構 
	bool MinMaxMaxMin_Filter(const cv::Mat& matInput, const Mat& matMask, const Mat& matElement, Mat& matOutput);

	// 最大最小值 + 最小最大值 濾波
	// matMask : 遮罩, 可為空=>整張都會處理
	// matElement : 濾波結構 
	bool MaxMinMinMax_Filter(const cv::Mat& matInput, const Mat& matMask, const Mat& matElement, Mat& matOutput);

	// 梯度濾波
	// matMask : 遮罩, 可為空=>整張都會處理
	// matElement : 濾波結構 
	// nTimes : 執行次數
	bool Gradient_Filter(const cv::Mat& matInput, const Mat& matMask, const Mat& matElement, const int& nTimes, Mat& matOutput);

	// 找邊濾波
	// matMask : 遮罩, 可為空=>整張都會處理
	// matElement : 濾波結構 
	bool FindEdge_Filter(const cv::Mat& matInput, const Mat& matMask, const Mat& matElement, Mat& matOutput);

	// PCA 影像濾波
	// 設定 樣本排列 與 主成分比列 進行 PCA反投影
	bool PCA_Filter(const cv::Mat& matGray, const int& nDataArrangement, const float& fRetainedVariance, Mat& matOutput);

	bool Sobel(const Mat &matSrc, const int &nFilterSize, Mat &matDst);

#pragma region Gabor

	// Gabor濾波
	// matInput : 輸入影像, 必須為灰階
	// nGaborRadius : 濾波半徑, 值域 1-3 
	// nModel : 1 => 紋路方向平滑
	//        : 2 => 紋路方向 Min濾波
	//        : 3 => 紋路方向 Max濾波
	//        : 4 => 紋路法線方向平滑
	//        : 5 => 紋路法線方向 Min濾波
	//        : 6 => 紋路法線方向 Max濾波
	//        : 7 => 紋路方向平滑, 紋路法線方向銳化
	//        : 8 => 只計算紋路方向(角度)
	// nTimes : 執行次數
	// matOutput : 輸出影像
	bool Gabor(const Mat& matInput, const int nGaborRadius, const int nModel, const int nTimes, Mat& matOutput);

	// Gabor濾波
	// matInput : 輸入影像, 必須為灰階
	// matMask : 遮罩, 只會對白色區域進行 Gabor
	// nGaborRadius : 濾波半徑, 值域 1-3 
	// nModel : 1 => 紋路方向平滑
	//        : 2 => 紋路方向 Min濾波
	//        : 3 => 紋路方向 Max濾波
	//        : 4 => 紋路法線方向平滑
	//        : 5 => 紋路法線方向 Min濾波
	//        : 6 => 紋路法線方向 Max濾波
	//        : 7 => 紋路方向平滑, 紋路法線方向銳化
	//        : 8 => 只計算紋路方向(角度)
	// nTimes : 執行次數
	// matOutput : 輸出影像(輸入時不可為空,與matInput格式相同)
	bool Gabor(const Mat& matInput, const Mat& matMask, const int nGaborRadius, const int nModel, const int nTimes, Mat& matOutput);

	// 計算紋路角度
	// nGaborRadius : 濾波半徑, 值域 1-3 
	// matInput : 輸入影像, 必須為灰階
	// matOutput : 輸出影像, 紀錄角度的影像
	bool GradientDirection(const int nGaborRadius, const Mat& matInput, Mat& matOutput);

	// 計算紋路角度
	// nGaborRadius : 濾波半徑, 值域 1-3 
	// matInput : 輸入影像, 必須為灰階
	// matMask : 遮罩, 只會對白色區域 計算紋路角度
	// matOutput : 輸出影像, 紀錄角度的影像(輸入時不可為空,與matInput格式相同)
	bool GradientDirection(const int nGaborRadius, const Mat& matInput, const Mat& matMask, Mat& matOutput);

	// 使用Gabor濾波強化影像
	// nModel : 1 => 紋路方向平滑
	//        : 2 => 紋路方向 Min濾波
	//        : 3 => 紋路方向 Max濾波
	//        : 4 => 紋路法線方向平滑
	//        : 5 => 紋路法線方向 Min濾波
	//        : 6 => 紋路法線方向 Max濾波
	//        : 7 => 紋路方向平滑, 紋路法線方向銳化
	//        : 8 => 只計算紋路方向(角度)
	// matInput : 輸入影像, 必須為灰階
	// matGradientDir : 輸入影像, 紀錄角度的影像
	// matOutput : 輸出影像
	bool GaborEnhance(const int nModel, const Mat& matInput, const Mat& matGradientDir, Mat& matOutput);

	// 使用Gabor濾波強化影像
	// nModel : 1 => 紋路方向平滑
	//        : 2 => 紋路方向 Min濾波
	//        : 3 => 紋路方向 Max濾波
	//        : 4 => 紋路法線方向平滑
	//        : 5 => 紋路法線方向 Min濾波
	//        : 6 => 紋路法線方向 Max濾波
	//        : 7 => 紋路方向平滑, 紋路法線方向銳化
	//        : 8 => 只計算紋路方向(角度)
	// matInput : 輸入影像, 必須為灰階
	// matMask : 遮罩, 只會對白色區域 進行 GaborEnhance
	// matGradientDir : 輸入影像, 紀錄角度的影像
	// matOutput : 輸出影像(輸入時不可為空,與matInput格式相同)
	bool GaborEnhance(const int nModel, const Mat& matInput, const Mat& matMask, const Mat& matGradientDir, Mat& matOutput);

#pragma endregion
#pragma endregion

#pragma region Threshold

	// 二值化模式 : THRESHOLD_NONE = 0, 不處理
	//
	//            : THRESHOLD_SINGLE = 1, 單閥值, 若需要遮罩輔助, 可設定 sMask1 
	//              nThreshold_Low : 閥值
	//              bDark = true  ; 值域 [0 , nThreshold_Low]
	//				bDark = false ; 值域 (nThreshold_Low, 255]
	//
	//            : THRESHOLD_DOUBLE = 2, 雙閥值, 若需要遮罩輔助, 可設定 sMask1 
	//              nThreshold_Low : 低閥值
	//              nThreshold_High : 高閥值
	//              bDark = true  ; 值域 [nThreshold_Low , nThreshold_High]
	//              bDark = false ; 值域 [0 , nThreshold_Low) + (nThreshold_High , 255]
	//
	//            : THRESHOLD_AVERAGE = 3, 自動閥值, 灰階平均值(Average), 若需要遮罩輔助, 可設定 sMask1
	//              fAlpha : Average 的縮放係數, 不可為 0, 預設為 1
	//              sLimit : 為 Average 設定範圍區間, 只須設定 Left相關參數
	//              bDark = true  ; 值域 [0 , Average]
	//				bDark = false ; 值域 (Average , 255]
	//
	//            : THRESHOLD_OTSU = 4, 自動閥值, Otsu, 若需要遮罩輔助, 可設定 sMask1
	//              fAlpha : Otsu 的縮放係數, 不可為 0, 預設為 1
	//              sLimit : 為 Otsu 設定範圍區間, 只須設定 Left相關參數
	//              bDark = true  ; 值域 [0 , Otsu]
	//				bDark = false ; 值域 (Otsu , 255]
	//
	//            : THRESHOLD_TRIANGLE = 5, 自動閥值, Triangle, 若需要遮罩輔助, 可設定 sMask1
	//              fAlpha: Triangle 的縮放係數, 不可為 0, 預設為 1
	//              sLimit : 為 Triangle 設定範圍區間, 只須設定 Left相關參數
	//              bDark = true  ; 值域 [0 , Triangle]
	//				bDark = false ; 值域 (Triangle , 255]
	//
	//            : THRESHOLD_MAXENTROPY = 6, 自動閥值, MaxEntropy, 若需要遮罩輔助, 可設定 sMask1
	//              fAlpha: MaxEntropy 的縮放係數, 不可為 0, 預設為 1
	//              sLimit : 為 MaxEntropy 設定範圍區間, 只須設定 Left相關參數
	//              bDark = true  ; 值域 [0 , MaxEntropy]
	//				bDark = false ; 值域 (MaxEntropy , 255]
	//
	//            : THRESHOLD_MINCROSSENTROPY = 7, 自動閥值, MinCrossEntropy, 若需要遮罩輔助, 可設定 sMask1	
	//              fAlpha: MinCrossEntropy 的縮放係數, 不可為 0, 預設為 1
	//              sLimit : 為 MinCrossEntropy 設定範圍區間, 只須設定 Left相關參數
	//              bDark = true  ; 值域 [0 , MinCrossEntropy]
	//				bDark = false ; 值域 (MinCrossEntropy , 255]
	//
	//			  : THRESHOLD_RENYIENTROPY = 8, 自動閥值, RenyiEntropy, 若需要遮罩輔助, 可設定 sMask1	
	//              fAlpha : RenyiEntropy的參數, 不可為 1,
	//              sLimit : 為 MinCrossEntropy 設定範圍區間, 只須設定 Left相關參數
	//              bDark = true  ; 值域 [0 , RenyiEntropy]
	//				bDark = false ; 值域 (RenyiEntropy , 255]
	//
	//            : THRESHOLD_KMEAN = 9, 自動閥值 Kmean, 若需要遮罩輔助, 可設定 sMask1	
	//              sKmean : 必須設定
	//              sLimit : 為 Kmean的輸出結果 設定範圍區間
	//              bDark = true  ; 值域 [nThreshold_Low , nThreshold_High]
	//              bDark = false ; 值域 [0 , nThreshold_Low) + (nThreshold_High , 255]
	//	
	//            : THRESHOLD_DBSCAN = 10, 自動閥值 DBSCAN, 若需要遮罩輔助, 可設定 sMask1	
	//              sDbscan : 必須設定 
	//              bDark = true  ; 值域 [nThreshold_Low , nThreshold_High]
	//              bDark = false ; 值域 [0 , nThreshold_Low) + (nThreshold_High , 255]
	//
	//            : THRESHOLD_MULTIPLE = 11, 自動閥值 多重區間二值化---使用遮罩, 須設定 sMask1 
	//              nAutoThreshold = nMode, 1=>使用kmean計算二值化區間, 2=>使用Dbscan計算二值化區間
	//              sKmean : nAutoThreshold 設定為1時, 必須設定 
	//              sDbscan : nAutoThreshold 設定為2時, 必須設定 
	//              bDark : true=>代表抓取 [nThres_Low,nThres_Height]之間的區域 (包含nThres_Low與nThres_Height); 
	//                    : false=>代表抓取 [0,nThres_Low) 與 (nThres_Height,255]的區域 (不含nThres_Low與nThres_Height)
	//
	//            : THRESHOLD_ADAPTIVE_WELLNER = 12, 自動閥值 Adaptive---Wellner	
	//              nThreshold_Low = nRadius
	//              nThreshold_High = nDiff_Threshold
	//
	//            : THRESHOLD_GLCM = 13, 灰階共生矩陣二值化
	//              nThreshold_Low : X 方向偏移量
	//              nThreshold_High : Y 方向偏移量
	//              fAlpha : 多少灰階值為一層
	//              nAutoThreshold : 絕對距離差
	//              bDark = true  ; 值域 [0 , nAutoThreshold]
	//              bDark = false ; 值域 [nAutoThreshold , (256/fAlpha)-1]
	//
	//            : THRESHOLD_COLOR_TARGET = 14, 彩色影像二值化---設定目標值與容許誤差進行二值化
	//              nThreshold_Low=目標值 B
	//              nThreshold_High=目標值G
	//              nAutoThreshold=目標值 R
	//              fAlpha = 容許誤差
	//              bDark = true  : 目標值區域為 255
	//              bDark = false : 非目標值區域為 255
	//
	//            : THRESHOLD_HSV_TARGET = 15, HSV影像二值化---設定H的平均值與範圍值, SV的最小值與最大值進行二值化
	//              vtnLow = [0]=Mean H ; [1]=Min S ; [2]=Min V
	//              vtnUpper = [0]=Range H; [1]=Max S ; [2]=Max V
	bool Threshold(const SProcessModeInput sInput, SThresholdParam& sParam, SProcessModeOutput& sOutput);

	// 單閥值二值化
	// matSrc : 必須是灰階圖像 
	// matMask : 遮罩 , 若為空 或 Mat() 代表整張都要做
	// nThres : 閥值
	// bBright : true => 代表抓取 (nThres,255]之間的區域 (不包含 nThres); 
	//         : false=> 代表抓取 [0,nThres]之間的區域 (包含 nThres); 
	// matOutput : 二值化結果
	bool Threshold_Single(const Mat &matSrc, const Mat &matMask, const int &nThres, const bool& bBright, Mat &matDst);

	// 雙閥值二值化
	// matInput : 輸入影像
	// matMask : 遮罩 , 若為空 或 Mat() 代表整張都要做
	// nThres_Low : 低閥值
	// nThres_Height : 高閥值
	// bBetween = true 代表抓取 [nThres_Low , nThres_Height] 之間的區域 (包含nThres_Low與nThres_Height); 
	//            false代表抓取 [0 , nThres_Low) + (nThres_Height , 255] 的區域 (不含nThres_Low與nThres_Height)
	// matOutput : 二值化結果
	bool Threshold_Double(const cv::Mat& matInput, const cv::Mat& matMask, const int& nThres_Low, const int& nThres_Height, const bool& bBetween, Mat& matOutput);
	
	// 平均值二值化
	// matSrc : 必須是灰階圖像 
	// matMask : 遮罩 , 若為空 或 Mat() 代表整張都要做
	// sLimit : 自動閥值的極限限制
	// fCoefficient : 自動閥值的縮放係數, 1代表不縮放, 不可<=0
	// bBright : true => 代表抓取 (Mean,255] 之間的區域 (不含 Mean); 
	//         : false=> 代表抓取 [0,Mean] 之間的區域 (包含 Mean); 
	// matOutput : 二值化結果
	// return : 灰階平均值 Mean
	int Threshold_Mean(const Mat& matSrc, const Mat& matMask, const SLimitModeParameter& sLimit, const float fCoefficient, const bool& bBright, Mat& matDst);

	// Otsu 二值化
	// matSrc : 必須是灰階圖像 
	// matMask : 遮罩 , 若為空 或 Mat() 代表整張都要做
	// sLimit : 自動閥值的極限限制
	// fCoefficient : 自動閥值的縮放係數, 1代表不縮放, 不可<=0
	// bBright : true => 代表抓取 (Otsu,255] 之間的區域 (不含 Otsu); 
	//         : false=> 代表抓取 [0,Otsu] 之間的區域 (包含 Otsu); 
	// matOutput : 二值化結果
	// return : Otsu閥值
	int Threshold_Otsu(const Mat& matSrc, const Mat& matMask, const SLimitModeParameter& sLimit, const float fCoefficient, const bool& bBright, Mat& matDst);
	int Otsu(const Mat& matSrc, const Mat& matMask = Mat());

	// 三角閥值
	// matSrc : 必須是灰階圖像 
	// matMask : 遮罩 , 若為空 或 Mat() 代表整張都要做
	// sLimit : 自動閥值的極限限制
	// fCoefficient : 自動閥值的縮放係數, 1代表不縮放, 不可<=0
	// bBright : true => 代表抓取 (Triangle,255] 之間的區域 (不含 Triangle); 
	//         : false=> 代表抓取 [0,Triangle] 之間的區域 (包含 Triangle); 
	// matOutput : 二值化結果
	// return : Triangle閥值
	int Threshold_Triangle(const Mat& matSrc, const Mat& matMask, const SLimitModeParameter& sLimit, const float fCoefficient, const bool& bBright, Mat& matDst);
	int Triangle(const Mat& matSrc, const Mat& matMask = Mat());

	// 最大熵二值化
	// matSrc : 必須是灰階圖像 
	// matMask : 遮罩 , 若為空 或 Mat() 代表整張都要做
	// sLimit : 自動閥值的極限限制
	// fCoefficient : 自動閥值的縮放係數, 1代表不縮放, 不可<=0
	// bBright : true => 代表抓取 (MaxEntropy,255] 之間的區域 (不含 MaxEntropy); 
	//         : false=> 代表抓取 [0,MaxEntropy] 之間的區域 (包含 MaxEntropy); 
	// matOutput : 二值化結果
	// return : MaxEntropy閥值
	int Threshold_MaxEntropy(const Mat& matSrc, const Mat& matMask, const SLimitModeParameter& sLimit, const float fCoefficient, const bool& bBright, Mat& matDst);
	int MaxEntropy(const Mat& matSrc, const Mat& matMask = Mat());

	// 最小交叉熵二值化
	// matSrc : 必須是灰階圖像 
	// matMask : 遮罩 , 若為空 或 Mat() 代表整張都要做
	// sLimit : 自動閥值的極限限制
	// fCoefficient : 自動閥值的縮放係數, 1代表不縮放, 不可<=0
	// bBright : true => 代表抓取 (MinCrossEntropy,255] 之間的區域 (不含 MinCrossEntropy); 
	//         : false=> 代表抓取 [0,MinCrossEntropy] 之間的區域 (包含 MinCrossEntropy); 
	// matOutput : 二值化結果
	// return : MinCrossEntropy閥值
	int Threshold_MinCrossEntropy(const Mat& matSrc, const Mat& matMask, const SLimitModeParameter& sLimit, const float fCoefficient, const bool& bBright, Mat& matDst);
	int MinCrossEntropy(const Mat& matSrc, const Mat& matMask = Mat());

	// 瑞麗熵
	// matSrc : 必須是灰階圖像 
	// matMask : 遮罩 , 若為空 或 Mat() 代表整張都要做
	// sLimit : 自動閥值的極限限制
	// fAlpha : RenyiEntropy 的係數, 不可為1
	// bBright : true => 代表抓取 (RenyiEntropy,255] 之間的區域 (不含 RenyiEntropy); 
	//         : false=> 代表抓取 [0,RenyiEntropy] 之間的區域 (包含 RenyiEntropy); 
	// matOutput : 二值化結果
	// return : RenyiEntropy閥值
	int Threshold_RenyiEntropy(const Mat& matSrc, const Mat& matMask, const SLimitModeParameter& sLimit, const float& fAlpha, const bool& bBright, Mat& matDst);
	int RenyiEntropy(const Mat& matSrc, const float& fAlpha, const Mat& matMask = Mat());

	// Kmean 二值化, 使用 kmean 自動計算 nThres_Low 與 nThres_Height
	// matSrc : 必須是灰階圖像 
	// matMask : 遮罩 , 若為空 或 Mat() 代表整張都要做
	// sKmean : kmean 的設定值
	// sLimit : 自動閥值的極限限制
	// bBetween = true 代表抓取 [nThres_Low , nThres_Height] 之間的區域 (包含nThres_Low與nThres_Height); 
	//            false代表抓取 [0 , nThres_Low) 與 (nThres_Height , 255] 的區域 (不含nThres_Low與nThres_Height)
	// matOutput : 二值化結果
	bool Threshold_Kmean(const Mat& matSrc, const Mat& matMask, SKmeanParameter& sKmean, const SLimitModeParameter& sLimit, const bool& bBright, Mat& matDst);

	// DBSCAN 二值化, 使用 DBSCAN自 動計算 nThres_Low 與 nThres_Height
	// matSrc : 必須是灰階圖像 
	// matMask : 遮罩 , 若為空 或 Mat() 代表整張都要做
	// sDbscan : DBSCAN 的設定值
	// sLimit : 自動閥值的極限限制
	// bBetween = true 代表抓取 [nThres_Low , nThres_Height] 之間的區域 (包含nThres_Low與nThres_Height); 
	//            false代表抓取 [0 , nThres_Low) 與 (nThres_Height , 255] 的區域 (不含nThres_Low與nThres_Height)
	// matOutput : 二值化結果
	bool Threshold_DBSCAN(const Mat& matSrc, const Mat& matMask, SDbscanParameter& sDbscan, const SLimitModeParameter& sLimit, const bool& bBright, Mat& matDst);

	// Wellner 自適應二值化
	bool AdaptiveThreshold_Wellner(const Mat& matSrc, const int nRadius, const int& nDiff_Threshold, const bool& bDark, Mat& matThres);

	// 多重區間二值化
	// nMode : 1=>使用 kmean 計算多組二值化區間
	//         2=>使用 Dbscan 計算多組二值化區間
	// bBetween : true=>代表抓取 [nThres_Low,nThres_Height]之間的區域 (包含nThres_Low與nThres_Height); 
	//          : false=>代表抓取 [0,nThres_Low) 與 (nThres_Height,255]的區域 (不含nThres_Low與nThres_Height)
	// sKmean : kmean 的設定值
	// sDbscan : DBSCAN 的設定值
	// matMask : 遮罩 , 若為空 或 Mat() 代表整張都要做
	// vtmatThreshold : 二值化結果
	bool Threshold_Multiple(const Mat& matSrc, const int& nMode, const bool& bBetween, SKmeanParameter& sKmean, SDbscanParameter& sDbscan, vector<Mat>& vtmatThreshold, const Mat& matMask=Mat());

	// 灰階共生矩陣二值化
	bool Threshold_GLCM(const Mat& matSrc, const int& nInterval_X, const int& nInterval_Y, const int& nGrayLevel, const int& nThresX, const int& nThresY, Mat& matThres);

	// nDiffDist = 灰階值距離差
	bool Threshold_GLCM(const Mat& matSrc, const int& nInterval_X, const int& nInterval_Y, const int& nGrayLevel, const int& nDiffDist, const bool& bGreaterThan, Mat& matThres);

	bool Threshold_GLCM(const Mat& matSrc, const int& GrayLevel, const vector<pair<POINT, int>>& vtptInterval_Dist, const vector<bool>& vtbGreaterThan, Mat& matThres);

	// 二維最大熵
	bool Threshold_MaxEntropy_Two(const vector<vector<int>>& vtnGLCM, POINT& ptThres);

	// 彩色影像二值化
	// matColor : 彩色影像
	// vtnMinTargetValue : 最小值 ; [0]=B ; [1]=G ; [2]=R
	// vtnMaxTargetValue : 最大值 ; [0]=B ; [1]=G ; [2]=R
	// bTarget : 為true => 目標值為255; 為false=>非目標值為255
	// matMask : 遮罩 , 若為空 或 Mat() 代表整張都要做
	bool Threshold_Color(const Mat& matColor, const bool& bTarget, const vector<int>& vtnMinTargetValue, const vector<int>& vtnMaxTargetValue, Mat& matThreshold, const Mat& matMask = Mat());

	// HSV影像二值化
	// matColor : 彩色影像
	// vtnMinTargetValue ; [0]=Mean H ; [1]=Min S ; [2]=Min V
	// vtnMaxTargetValue ; [0]=Range H; [1]=Max S ; [2]=Max V
	// matThreshold : 二值化結果
	// matMask : 遮罩 , 若為空 或 Mat() 代表整張都要做
	bool Threshold_HSV(const Mat& matColor, const bool& bTarget, const vector<int>& vtnMinTargetValue, const vector<int>& vtnMaxTargetValue, Mat& matThreshold, const Mat& matMask = Mat());

	// 單一值使用左值設定來判斷
	// return 判斷後的値
	int CalLimit(const SLimitModeParameter& sLimit, const int& nValue);

	// 輸入兩個值, nSmall用左值設定, nBig用右值設定
	// return 判斷後的値, POINT.x=nSmall的更新值, POINT.y=nBig的更新值
	POINT CalLimit(const SLimitModeParameter& sLimit, int& nSmall, int& nBig);
#pragma endregion

#pragma region ContoursConvert

	// CONTOUR_NONE = 0,					不處理
	// CONTOUR_THRESHOLD_TO_EDGE = 1,		二值化影像取邊
	// CONTOUR_THRESHOLD_TO_THINNING = 2,	二值化影像取邊---細線化
	// CONTOUR_SEARCH = 3,					從邊界影像(二值化影像)取出每個物件的輪廓
	// CONTOUR_GRAY_SEARCH = 4,             灰階搜尋, 相鄰pixels的灰階值差小於設定值, 則歸為同一群, 須設定 nSelectType(相鄰灰階值差), 若有設定SetMask1()則會以Mask為搜尋範圍, 若沒有設定SetMask1()則會以整張影像為搜尋範圍
	// CONTOUR_GRAYSEARCH_TO_EDGE = 5,      只保留灰階搜尋的結果的邊界
	// CONTOUR_BOUNDING_RECTANGLE = 6,		計算每個物件的外接矩形
	// CONTOUR_UNION_1 = 7,					Union 聯集---單一群體內的ROI, 須設定 nMinPerimeter(聯集模式),1=>全部全疊(包含ROI中有ROI); 2=>局部重疊(不包含ROI中有ROI)
	// CONTOUR_UNION_2 = 8,					Union 聯集---兩個群體的ROI
	// CONTOUR_INTERSECTION_1 = 9,			Intersection 交集---單一群體內的ROI
	// CONTOUR_INTERSECTION_2 = 10,			Intersection 交集---兩個群體的ROI	
	// CONTOUR_SELECT = 11,					依設定選擇輪廓與ROI
	// CONTOUR_ANGLE = 12,                  計算輪廓線每一點的夾角角度, 須定 nMinPerimeter(最小間隔) 與	nMaxPerimeter(最大間隔)
	// CONTOUR_TEXTURE_DIRECTION = 13,      計算輪廓紋路的方向(0-180度)	
	// CONTOUR_AREA = 14,                   計算輪廓線形成的面積
	// CONTOUR_FILL = 15,                   輪廓線填充(邊緣向中心逼近)
	// CONTOUR_FLOOD_FILL = 16,             輪廓線填充(使用洪水填充), 需設定SetMask1() bToEdge為true,代表要完全填滿
	// CONTOUR_CONVEX = 17,                 輪廓線轉凸集合
	// CONTOUR_CONVEX_FILL = 18,            凸集合輪廓填充
	// CONTOUR_DRAW = 19,					畫出輪廓點與ROI
	// CONTOUR_CONVERT_MASK = 20,			將輪廓轉成 Mask,
	// CONTOUR_MERGE = 21,					將2個輪廓結構合併成ㄧ個
	// CONTOUR_CHANGE_SIZE = 22,            改變輪廓外接矩形的大小, 須定 nMinPerimeter(X的方向的改變量) 與 nMaxPerimeter(Y的方向的改變量)
	// CONTOUR_DISPLAY = 23,                顯示輪廓跟矩形(彩色影像), 設定SetInput1()決定背景影像, bToEdge決定是否畫輪廓線, bRectangle決定是否畫矩形, bBoundingToEdge決定是否顯示bLabel
	// CONTOUR_FINAL = 24,
	bool ContoursConvert(const SProcessModeInput sInput, SContoursShapeParam& sParam, SProcessModeOutput& sOutput);

	// 二值化影像取邊---使用四相鄰 20230130
	// matThres : 二值影像化
	// bWhite : 若為true => 會將matThres的白色區域當作目標 ; 反之以黑色區域當目標
	// bBoundary : 若為true => 會處理影像邊界 ; 反之會忽略影像邊界
	// matEdge : 邊界影像(一律黑底白線)
	bool GetEdgeImage(const Mat &matThres, const bool& bWhite, const bool& bBoundary, Mat &matEdge);
	bool GetEdgeImage_Iteration(const Mat &matThres, const bool& bWhite, Mat &matEdge);

	// 針對輪廓中的 ROI進行邊界搜尋
	bool GetEdgeImage(SContoursParam& sContours, SContoursParam& sContours_Edge);

	// 輪廓搜尋
	bool SearchContours(const cv::Mat& matThres, SContoursShapeParam& sParam);
	bool SearchContours(const cv::Mat& matThres, const SContoursShapeParam& sParam, SContoursParam& sContours);

	// 輪廓搜尋---不排序
	// matImage : 須為二值化影像或邊界影像
	// nSelectType = 選擇要保留的輪廓 : 1 => 全部
	//                                  2 => 選擇白色區域形成的輪廓
	//                                  3 => 選擇黑色區域形成的輪廓	
	// nMinPerimeter = 輪廓的最小周長
	// nMaxPerimeter = 輪廓的最大周長
	// bToEdge = 是否將 matImage 轉成 邊界影像, true代表要轉成邊界影像
	// bBoundary = 是否將影像邊界也當做邊界影像
	// bRectangle = 是否計算輪廓的外接矩形, true代表要計算外接矩形
	// bArea = 是否計算輪廓形成的面積, true代表要計算面積(bRectangle也必須為true才能計算)
	bool SearchContours_Type1(const cv::Mat& matImage, const bool bToEdge, const bool bBoundary, const bool bRectangle, const int nSelectType, const int nMinPerimeter, const int nMaxPerimeter, SContoursParam& sContours);
	bool SearchContours_Type1(const cv::Mat& matImage, const bool bToEdge, const bool bBoundary, const bool bRectangle, const bool bArea, const int nSelectType, const int nMinPerimeter, const int nMaxPerimeter, SContoursParam& sContours);
	bool SearchContours_Type2(const cv::Mat& matImage, const bool bToEdge, const bool bBoundary, const bool bRectangle, const int nSelectType, const int nSortType, const int nMinPerimeter, const int nMaxPerimeter, SContoursParam& sContours);
	bool SearchContours_Type2(const cv::Mat& matImage, const bool bToEdge, const bool bBoundary, const bool bRectangle, const bool bArea, const int nSelectType, const int nSortType, const int nMinPerimeter, const int nMaxPerimeter, SContoursParam& sContours);
	
	// 灰階搜尋
	// matGray : 灰階影像
	// matMask : 遮罩影像, 若為空代表整張對要搜尋
	// nDiffValue : 相鄰灰階值差, 相鄰灰階值小於此值會歸類為同一群
	// nMinPerimeter : 輪廓的最小周長
	// nMaxPerimeter : 輪廓的最大周長
	bool SearchContours_Gray(const cv::Mat& matGray, const cv::Mat& matMask, const bool bRectangle, const int& nDiffValue, const int nMinPerimeter, const int nMaxPerimeter, SContoursParam& sContours);

	// 8 方向 BFS (Breadth-First Search)（廣度優先），回傳同一連通區所有點
	// matMask : 255是未拜訪, 0是以拜訪
	// ptSeed : 起始點
	// nDiffValue = 最大允許灰階差
	bool BFS_Search(const cv::Mat& matGray, cv::Mat& matMask, const POINT& ptSeed, const int nDiffValue, std::vector<POINT>& vtptComponent);
	bool BFS_Search_Constant(const cv::Mat& matGray, cv::Mat& matMask, const POINT& ptSeed, const int nDiffValue, std::vector<POINT>& vtptComponent);
	bool BFS_Search_ROI(const cv::Mat& matGray, const SJRect* psjROI, const POINT& ptSeed, const int nDiffValue, std::vector<POINT>& vtptComponent, bool bConstant = true);
	bool FloodFill_BFS_Mask(const vector<vector<POINT>>* pvt2ptEdge, Mat& matMask, Mat& matFill);

	bool SearchContours_Gray_BFS(const cv::Mat& matGray, const cv::Mat& matMask, const bool bRectangle, const int& nDiffValue, const int nMinPerimeter, const int nMaxPerimeter, SContoursParam& sContours);

	// 只計算ROI範圍內
	bool SearchContours_Gray_ROI(const cv::Mat& matGray, const SJRect* psjROI, const bool bRectangle, SContoursParam& sContours);

	// 選擇輪廓
	// nSelectType : 1 => 依 On 狀態
	//               2 => On + 白色區域形成的輪廓
	//               3 => On + 黑色區域形成的輪廓	
	bool SelectContours(const cv::Mat& matThres, const int nSelectType, SContoursParam& sContours);
	bool SelectContours(const cv::Mat& matThres, const int nSelectType, const SContoursParam& sContours_In, SContoursParam& sContours_Out);

	// 計算輪廓線的夾角
	bool ContoursLine_Angle(const int &nMinLength, const int &nMaxLength, SContoursParam& sContours);

	// 將 sContours 轉成 凸集合
	bool ConvertToConvex(SContoursParam& sContours, SContoursParam& sContours_Convex);

	// 填充凸集合
	bool Convex_Fill(SContoursParam& sContours_Convex, Mat& matDisplay);
	bool Convex_Fill(const vector<POINT>& vtptContoursPos, Mat& matDisplay);

	// 將輪廓線轉成Mask
	bool ContoursConvertToMask(SContoursParam& sContours);

	// 將 SContoursParam 的Mask Buffer轉成 Mat
	// return false 時, matMask為空
	bool ContoursMaskConvertToMat(SContoursParam& sContours, const int& nId, Mat& matMask);

	// 畫 Rectangle
	// matDisplay 為灰階影像---背景為0, 輪廓為255
	bool DrawRectangle(const SContoursParam& sContours, Mat& matDisplay);

	// 將輪廓畫出
	// bRectangle => true 會畫出外接矩形(如果沒有外接矩形則不會顯示)
	// matDisplay : 輸出結果為灰階影像---背景為0, 輪廓區為255
	bool DrawContours(const SContoursParam& sContours, const bool bRectangle, Mat& matDisplay);

	// 若 matDisplay 不為空, 則會連續畫在matDisplay上
	bool DrawContours_Continuous(const SContoursParam& sContours, Mat& matDisplay);

	bool SetROI(SContoursParam& sContours, vector<Rect>& vtcvROI, Rect& cvROI);

	// 將 sContours id 狀態為 on 的輪廓填滿
	// matDisplay : 輸出結果為灰階影像---背景為0, 輪廓區為255
	bool FillContours_All(SContoursParam& sContours, Mat& matDisplay);

	// 將 sContours id 狀態為 on 的輪廓填滿
	// matDisplay : 輸出結果為灰階影像---背景為0, 輪廓區為255
	bool FillContours_Search(SContoursParam& sContours, Mat& matDisplay);
	bool FillContours_Flood(SContoursParam& sContours, const Mat& matMask, const bool& bFull, Mat& matDisplay);

	// 將輪廓填滿---指定sContours的Id
	// bFill => true 會將輪廓區域填入255
	// matDisplay : 輸出結果為灰階影像---背景為0, 輪廓區為255
	bool FillContours(const int& nId, const bool& bFill, SContoursParam& sContours, Mat& matDisplay);

	// 計算輪廓面積
	bool CalculateContoursArea(SContoursParam& sContours);

	// 計算邊界內的面積
	// matEdge : 邊界影像(灰階)邊的灰階值必須為 255
	bool CalculateArea(Mat& matEdge, int& nArea);

	// 計算輪廓的外接矩形
	bool CalBoundingRectangle(SContoursParam& sContours);

	// 刪除輪廓並重新排序
	bool DeleteContours(SContoursParam& sContours);
	bool DeleteContours(const SContoursParam& sContours_Src, SContoursParam& sContours_Dst);

	// 2個ROI聯集
	// 聯集結果在 sROI1
	// return值為0代表沒有合併, 大於0代表有合併
	int ROI_Union(SJRect* psROI1, SJRect* psROI2);

	// 將sContours內相鄰的ROI合併 
	// nMode : 1 => 全部全疊(包含ROI中有ROI)
	//       : 2 => 局部重疊(不包含ROI中有ROI)
	bool UnionEdge(const int& nMode, SContoursParam& sContours);

	// 回傳2個ROI是否重疊
	// nMode : 1 => 全部全疊(包含ROI中有ROI)
	//       : 2 => 局部重疊(不包含ROI中有ROI)
	bool IsRoiOverlap(const int& nMode, const SJRect* psROI1, const SJRect* psROI2);

	// 將 vtpsjRect 中的ROI合併起來
	bool CalculateMaxROI(const int& nCount, const vector<SJRect*>& vtpsjRect, SJRect& sjMaxROI);

	// matGray : 不可為空, 須為灰階
	bool Draw_SJRect(const SJRect* psROI1, Mat& matGray);

	// 畫輪廓線
	// matDisplay 不可為空, 且為灰階
	bool Draw_ContoursLine(const vector<vector<POINT>>& vt2ptLinePoint, Mat& matDisplay);

	// 將相鄰的邊聯集
	bool UnionEdge(const vector<vector<POINT>>& vtEdgePoint, const vector<vector<POINT>>& vtCrossPoint, Mat& matLineID, vector<vector<int>>& vtnLineIndex);
	bool UnionEdge_Sort(const vector<vector<POINT>>& vtEdgePoint, const vector<vector<POINT>>& vtCrossPoint, Mat& matLineID, vector<vector<int>>& vtnLineIndex);

	// 計算輪廓的紋路角度
	// matGray : 灰階影像
	// matTextureDirection : 紋路角度影像(角度範圍0-180度)
	bool CalculateContoursTextureDirection(const Mat& matGray, SContoursParam& sContours, Mat& matTextureDirection);

	// 基於紋路角度的輪廓聯集
	// 會在nDistance範圍內, 挑出紋路角度差異最小的輪廓當作要進行聯集的目標
	// matGray : 灰階影像
	// nDistance : 相鄰輪廓的距離在此設定值內都會被當作聯集候選
	// nAngleDiff : 多個候選輪廓會再判斷紋路角度, 差值在nAngleDiff範圍內才會被當作聯集候選, 若有多個, 會選角度差異最小的
	bool UnionContours_TextureDirection(const Mat& matGray, const int& nDistance, const int& nAngleDiff, SContoursParam& sContours);

	// 顯示輪廓資訊
	// matDisplay : 必須是彩色影像
	bool DisplayContours(SContoursParam& sContours, Mat& matDisplay, const bool bContours, const bool bRectangle, const bool bLabel, const int nContoursWidth, const int& nROIWidth, const int& nTxtWidth, const float& fFontSize, 
		const Scalar& scContours = Scalar(), const Scalar& scRectangle = Scalar(), const Scalar& scLabel = Scalar());
	
	// 不同輪廓用不同顏色標示---顯示用
	// matDisplay 為彩色影像
	bool DisplayContours(SContoursParam& sContours, const bool bContours, const bool bRectangle, Mat& matDisplay, const bool bLabel = true);
	bool DisplayContours(SContoursParam& sContours, const bool bContours, const bool bRectangle, const Scalar& scContours, const Scalar& scROI, Mat& matDisplay, const bool bLabel = false);

	// 不同輪廓用不同顏色標示---顯示用(偏移)
	// matDisplay 為彩色影像
	bool DisplayContours_Shift(SContoursParam& sContours, const vector<POINT>& vtptShift, const bool bContours, const bool bRectangle, Mat& matDisplay, const bool bLabel = true);

	// 只顯示其中一條輪廓
	bool DisplayContours_Single(SContoursParam& sContours, const int& nId, const POINT& ptShift, const Vec3b& v3bColor, const bool bContours, const bool bRectangle, Mat& matDisplay, const bool bLabel = true);
#pragma endregion

#pragma region Morphology

	// 型態學處理 
	// 濾波的結構元素 : ELEMENT_NONE = 0, 不處理
	//                : ELEMENT_RECT = 1, 矩形
	//                : ELEMENT_CROSS = 2, 交叉
	//                : ELEMENT_ELLIPSE = 3, 橢圓
	//                : ELEMENT_RECT_TILTED_45 = 4, 矩形---45度斜線(nSizeX=矩形大小, nSizeY=線寬)
	//                : ELEMENT_RECT_TILTED_135 = 5, 矩形---135度斜線(nSizeX=矩形大小, nSizeY=線寬)
	// 二值化影像形態學模式 : MORPHOLOG_NONE = 0, 不處理, 
	//                      : MORPHOLOG_EROSION = 1, 侵蝕, 
	//						: MORPHOLOG_DILATE = 2, 膨脹
	//						: MORPHOLOG_OPENING = 3, Opening(先侵蝕再膨脹)
	//					    : MORPHOLOG_CLOSING = 4, Closing(先膨脹再侵蝕)
	//						: MORPHOLOG_OPENCLOSE = 5, 先 Opening 再 Closing
	//						: MORPHOLOG_CLOSEOPEN = 6, 先 Closing 再 Opening
	bool Morphology(const SProcessModeInput sInput, SMorphologParam& sParam, SProcessModeOutput& sOutput);

	int FillEdge(Mat& matEdge, Mat& matFill);

	bool FillEdge(Mat& matEdge);

	// 細線化
	void ThinSubiteration1(Mat & pSrc, Mat & pDst);
	void ThinSubiteration2(Mat & pSrc, Mat & pDst);

#pragma endregion

#pragma region DeleteObject

	// 刪除物件 : 
	// 若 eMode_Width 為 DELETE_SIZE_XXXX, 且 eMode_Height 也為 DELETE_SIZE_XXXX, 則會聯合判斷
	// 若 eMode_Width 為 DELETE_DENSITY_XXXX, 且 eMode_Height 也為 DELETE_DENSITY_XXXX, 則只會執行 eMode_Width
	// 若 eMode_Width 為 DELETE_DISTANCE_XXXX, 且 eMode_Height 也為 DELETE_DISTANCE_XXXX, 則會各自執行
	// 若 eMode_Width 為 KEEP_ROI_XXXX, 且 eMode_Height 也為 KEEP_ROI_XXXX, 則只會執行 eMode_Width
	// matThres : 必須為二值化影像 或 邊界影像
	// SDeleteObjectParam : 物件刪除模式
	// DELETE_NONE = 0 : 不處理
	// DELETE_SIZE_MORETHAN = 1 :				刪除長度大於 nMin 的物件
	// DELETE_SIZE_LESSTHAN = 2 :				刪除長度小於 nMin 的物件
	// DELETE_SIZE_BETWEEN = 3 :				刪除長度在 [nMin , nMax] 之間的物件
	// DELETE_SIZE_LESSTHAN_MORETHAN = 4 :		刪除長度小於 nMin 或 大於 nMax 的物件
	// DELETE_DENSITY_MORETHAN = 5 :			ROI中的物件數大於 nMin 即全刪除 , 須用 SetROI1() 設定ROI大小	
	// DELETE_DENSITY_LESSTHAN = 6 :			ROI中的物件數小於 nMin 即全刪除 , 須用 SetROI1() 設定ROI大小
	// DELETE_DENSITY_BETWEEN = 7 :				ROI中的物件數在 [nMin , nMax] 之間 即全刪除 , 須用 SetROI1() 設定ROI大小
	// DELETE_DENSITY_LESSTHAN_MORETHAN = 8 :	ROI中的物件數小於 nMin 或 大於 nMax 即全刪除 , 須用 SetROI1() 設定ROI大小
	// DELETE_DISTANCE_MORETHAN = 9 :			物件與目標影像物件的距離差大於 nMin, 即刪除 , 須用 SetContours2() 設定目標輪廓
	// DELETE_DISTANCE_LESSTHAN = 10 :			物件與目標影像物件距離差小於 nMin, 即刪除 , 須用 SetContours2() 設定目標輪廓
	// DELETE_DISTANCE_BETWEEN = 11 :			物件與目標影像物件距離差在 [nMin , nMax] 之間, 即刪除 , 須用 SetContours2() 設定目標輪廓
	// DELETE_DISTANCE_LESSTHAN_MORETHAN = 12 : 物件與目標影像物件距離差小於 nMin 或 大於 nMax, 即刪除 , 須用 SetContours2() 設定目標輪廓
	// KEEP_DISTANCE_MORETHAN = 13 :            保留物件的距離差大於設定值的輪廓, bEdge為true=>取絕對值為距離差
	// KEEP_DISTANCE_LESSTHAN = 14 :            保留物件的距離差小於設定值的輪廓, bEdge為true=>取絕對值為距離差
	// KEEP_DISTANCE_BETWEEN = 15 :             保留物件的距離差在設定值範圍之間的輪廓, bEdge為true=>取絕對值為距離差
	// KEEP_DISTANCE_LESSTHAN_MORETHAN = 16 :   保留物件的距離差小於設定值1 或 大於設定值2的輪廓, bEdge為true=>取絕對值為距離差	
	// KEEP_ROI_MAXSIZE = 17 :					保留最大尺寸的 ROI, nHeight_Max=保留的數量, 若為3=>保留前3大尺寸的ROI, 不可小於1
	// KEEP_ROI_MINSIZE = 18 :					保留最小尺寸的 ROI, nHeight_Max=保留的數量, 若為3=>保留前3小尺寸的ROI, 不可小於1
	// KEEP_ROI_MAXAREA = 19 :					保留最大面積的 ROI, nHeight_Max=保留的數量, 若為3=>保留前3大面積的ROI, 不可小於1
	// KEEP_ROI_MINAREA = 20 :					保留最小面積的 ROI, nHeight_Max=保留的數量, 若為3=>保留前3小面積的ROI, 不可小於1
	// KEEP_ROI_NEAR_IMAGECRNTER = 21 :         保留最靠近影像中心的 ROI, nHeight_Max=保留的數量, 若為3=>保留前3靠近的ROI, 不可小於1
	// KEEP_ROI_NEAR_BORDER = 22 :              保留靠近影像邊界的 ROI, nWidth_Min=邊的位置(1=上, 2=下, 3=左, 4=右, 5=全部), nWidth_Max=距離
	// KEEP_ROI_AWAY_BORDER = 23 :              保留遠離影像邊界的 ROI,	nWidth_Min=邊的位置(1=上, 2=下, 3=左, 4=右, 5=全部), nWidth_Max=距離
	// KEEP_ROI_NEAREST_BORDER = 24 :           保留最靠近影像邊界的 ROI, nWidth_Min=邊的位置(1=上, 2=下, 3=左, 4=右, 5=全部), nHeight_Max=保留的數量, 若為3=>保留前3靠近的ROI, 不可小於1
	// KEEP_ROI_THE_SIMILAR_SIZE = 25 :         保留 外接矩形大小最接近的 ROI, nWidth_Min=目標ROI寬; nHeight_Min=目標ROI高; nWidth_Max=模式,1=>不限制,2=>長寬都不能大於目標值,3=>長寬都不能小於目標值, nHeight_Max=保留的數量
	// KEEP_SHAPE_CIRCLE = 26 :					保留圓形物件, nWidth_Min = 最小圓形度(0-100)
	// KEEP_SHAPE_RECTANGLE = 27 :				保留矩形物件, nWidth_Min = 最小矩形度(0-100)
	// 結果輸出模式, 1=> 輸出輪廓影像
	//               2=> 輸出輪廓結構
	//               3=> 輸出ROI
	//               4=> 輸出輪廓影像 & ROI
	bool DeleteObject(const SProcessModeInput sInput, SDeleteObjectParam& sParam, SProcessModeOutput& sOutput);
	
	// 二值化影像取邊----ok
	// bWhite : 若 等於true 則只處裡灰階值 255; 若等於false 則只處裡灰階值 0
	int GetEdgeImage(const Mat &matInput, Mat &Output, bool bWhite);

	// 依據設定值刪除ROI
	// nMode : 1=>ROI寬度, 2=>ROI高度, 3=>寬度+高度 
	// nType : 1=> ROI尺寸大於nMin, 2=>ROI尺寸小於nMin, 3=>ROI尺寸介於[nMin , nMax], 4=>ROI尺寸小於nMin, 或大於nMax
	bool DeleteROI_Size(SContoursParam& sContours, const int nMode, const int nType, const int nMin, const int nMax);

	// 依據設定值刪除ROI
	// nType :  1 => W小於nMinW_H小於nMinH,                   2 => W小於nMinW_H大於nMaxH,                   3 => W小於nMinW_H介於[nMinH,nMaxH],                   4=> W小於nMinW_H小於nMinH或H大於nMaxH 
	//          5 => W大於nMaxW_H小於nMinH,                   6 => W大於nMaxW_H大於nMaxH,                   7 => W大於nMaxW_H介於[nMinH,nMaxH],                   8=> W大於nMaxW_H小於nMinH或H大於nMaxH 
	//          9 => (W介於[nMinW,nMaxW])_(H小於nMinH),      10 => (W介於[nMinW,nMaxW])_(H大於nMaxH),      11 => (W介於[nMinW,nMaxW])_(H介於[nMinH,nMaxH]),      12=> (W介於[nMinW,nMaxW])_(H小於nMinH與H大於nMaxH) 
	//         13 => (W小於nMinW或W大於nMaxW)_(H小於nMinH),  14 => (W小於nMinW或W大於nMaxW)_(H大於nMaxH),  15 => (W小於nMinW或W大於nMaxW)_(H介於[nMinH,nMaxH]),  16=> (W小於nMinW或W大於nMaxW)_(H小於nMinH或H大於nMaxH) 
	bool DeleteROI_Size(SContoursParam& sContours, const EDeleteMode eWidthMode, const EDeleteMode eHeightMode, const int nMinW, const int nMaxW, const int nMinH, const int nMaxH);

	// 以每個vtcvObj的中心點展開搜尋範圍, 並計算範圍內有多少ROI中心點, 再依設定的數量判斷是否刪除
	// nImageW, nImageH = 影像寬高
	// nROIW, nROIH = 搜尋範圍的寬高
	// nMode : 達成以下條件即刪除, 1=> ROI內數量大於nMin, 2=>ROI內數量小於nMin, 3=>ROI內數量介於[nMin , nMax], 4=>ROI內數量小於nMin, 與大於nMax
	// nMin : 最小數量
	// nMax = 最大數量
	// vtcvObj => 輸入的點(取中心點來計算)
	// vtbKeep => 若 vtbKeep[k]為false, 代表此點被刪除
	bool DeleteROI_Density(const int& nImageW, const int& nImageH, const int& nROIW, const int& nROIH, const int nMode, const int nMin,const int nMax, const vector<Rect>& vtcvObj, vector<bool>& vtbKeep);
	
	// 以每個vtcvObj的中心點展開搜尋範圍, 並計算範圍內有多少ROI中心點, 再依設定的數量判斷是否刪除
	// nImageW, nImageH = 影像寬高
	// nROIW, nROIH = 搜尋範圍的寬高
	// nMode : 達成以下條件即刪除, 1=> ROI內數量大於nMin, 2=>ROI內數量小於nMin, 3=>ROI內數量介於[nMin , nMax], 4=>ROI內數量小於nMin, 與大於nMax
	// nMin : 最小數量
	// nMax = 最大數量
	// vtcvObj => 輸入的點(取中心點來計算)
	bool DeleteROI_Density(SContoursParam& sContours, const int nMode, const int& nROIW, const int& nROIH, const int nMin, const int nMax);

	// 計算與目標輪廓中特定物件的水平方向的距離, 在依 nType設定 來刪除(目前只支援目標影像中只有一個物件)
	// nType : 達成以下條件即刪除, 1=> 距離大於nMin, 2=>距離小於nMin, 3=>距離介於[nMin , nMax], 4=>距離小於nMin, 或大於nMax
	bool DeleteROI_Distance_Horizontal(SContoursParam& sContours, SContoursParam& sTargetsContours, const int nType, const int nMin, const int nMax);

	// 計算與目標輪廓中特定物件的垂直方向的距離, 在依 nType設定 來刪除(目前只支援目標影像中只有一個物件)
	// nType : 達成以下條件即刪除, 1=> 距離大於nMin, 2=>距離小於nMin, 3=>距離介於[nMin , nMax], 4=>距離小於nMin, 或大於nMax
	bool DeleteROI_Distance_Vertical(SContoursParam& sContours, SContoursParam& sTargetsContours, const int nType, const int nMin, const int nMax);

	// 計算與目標輪廓中特定物件的水平方向的距離, 在依 nType設定 來保留(目前只支援目標影像中只有一個物件)
	// nType : 達成以下條件即保留, 1=> 距離大於nMin, 2=>距離小於nMin, 3=>距離介於[nMin , nMax], 4=>距離小於nMin, 或大於nMax
	// bAbs : 為true=> 取絕對值為距離差
	bool KeepROI_Distance_Horizontal(SContoursParam& sContours, SContoursParam& sTargetsContours, const int nType, const bool bAbs, const int nMin, const int nMax);

	// 計算與目標輪廓中特定物件的垂直方向的距離, 在依 nType設定 來保留(目前只支援目標影像中只有一個物件)
	// nType : 達成以下條件即保留, 1=> 距離大於nMin, 2=>距離小於nMin, 3=>距離介於[nMin , nMax], 4=>距離小於nMin, 或大於nMax
	// bAbs : 為true=> 取絕對值為距離差
	bool KeepROI_Distance_Vertical(SContoursParam& sContours, SContoursParam& sTargetsContours, const int nType, const bool bAbs, const int nMin, const int nMax);

	// 依 ROI寬度 高度 面積 來判斷
	// nMode : 1=>寬度, 2=>高度, 3=面積
	// nType : 1=>最大, 2=>最小
	bool KeepROI_Size(const int nMode, const int nType, SContoursParam& sContours, Rect& cvResultROI);

	// 依 ROI寬度 高度 面積 來判斷
	// nMode : 1=>寬度, 2=>高度, 3=面積
	// nType : 1=>最大, 2=>最小
	// nRoiCount : 輸出的ROI數量
	bool KeepROI_Size(const int nMode, const int nType, const int nRoiCount, SContoursParam& sContours, vector<Rect>& vtcvResultROI);
	
	// 保留最靠近影像中心的 ROI
	bool KeepROI_ImageCenter(SContoursParam& sContours, Rect& cvResultROI);

	// 保留最靠近影像中心的 nRoiCount 個 ROI
	// nRoiCount : 輸出的 ROI 數量
	bool KeepROI_ImageCenter(const int nRoiCount, SContoursParam& sContours, vector<Rect>& vtcvResultROI);

	// 依 ROI 位置來判斷
	// nMode : 1=> 靠近邊界, 2=>遠離邊界
	// nLocation : 1=>上, 2=>下, 3=>左, 4=>右, 5=>四個邊
	// nDistance : 距離
	bool KeepROI_Border(SContoursParam& sContours, const int nMode, const int nLocation, const int nDistance);

	// 保留(遠離)最接近邊界的ROI
	// nMode : 1=> 靠近邊界, 2=>遠離邊界
	// nLocation : 1=>上, 2=>下, 3=>左, 4=>右, 5=>四個邊
	bool KeepROI_NearestBorder(SContoursParam& sContours, const int nMode, const int nLocation);

	// 保留(遠離)最接近邊界的ROI
	// nMode : 1=> 靠近邊界, 2=>遠離邊界
	// nLocation : 1=>上, 2=>下, 3=>左, 4=>右, 5=>四個邊
	// nRoiCount : 輸出的ROI數量
	bool KeepROI_NearestBorder(SContoursParam& sContours, const int nMode, const int nLocation, const int nRoiCount);

	// 保留最接近大小的ROI
	// nMode : 1=>不限制,2=>長寬都不能大於目標值,3=>長寬都不能小於目標值
	// nRoiCount : 輸出的ROI數量
	// nROI_Width : 目標ROI的寬度
	// nTargetHeight : 目標ROI的高度
	bool KeepROI_TheSimilar_Size(SContoursParam& sContours, const int& nMode, const int& nRoiCount, const int& nTargetROI_Width, const int& nTargetHeight);

	// 依真圓度來決定是否保留
	// nMinRoundness : 最小真圓度 , 值域(0,100)
	bool KeepShape_Circle(const int& nMinRoundness, SContoursParam& sContours);

	// nMinRectangularity : 最小矩形度 , 值域(0,100)
	bool KeepShape_Rectangle(const int& nMinRectangularity, SContoursParam& sContours);

	bool Output_VectorPOINT(const vector<vector<POINT>>& vt2ptEdgePoint, const vector<vector<int>>& vt2nLineIndex, vector<bool>& vtbShow, vector<vector<POINT>>& vt2ptOutput);
#pragma endregion

#pragma region ImageCalculate

	// 影像計算 (若是單張影像計算, matInput1須給值, matInput2設定為 Mat()即可)
	// 影像計算處理模式 : CALCULATOR_NONE = 0, 不處理
	//                  : CALCULATOR_ADD = 1, 影像相加
	//                  : CALCULATOR_SUBTRACT = 2, 影像相減
	//                  : CALCULATOR_ABS_SUBTRACT = 3, 影像相減取絕對值
	//                  : CALCULATOR_MULTIPLY = 4, 影像相乘
	//                  : CALCULATOR_DIVIDE = 5, 影像相除	
	//                  : CALCULATOR_MAXIMUM = 6, 兩張影像同一pixels取最大值 (bSingle 須為 false)
	//                  : CALCULATOR_MINIMUM = 7, 兩張影像同一pixels取最小值 (bSingle 須為 false)	
	//                  : CALCULATOR_AVERAGE = 8, 兩張影像取平均 (bSingle 須為 false)
	//                  : CALCULATOR_RESIZE = 9, 影像縮放 nGrayValue=模式 1=>指定倍率 dValue , 2=>指定大小 nDstValue1=Dst_Width ; nDstValue2=Dst_Height
	//                  : CALCULATOR_COPY = 10, 影像Copy, 可設定 Mask 或 ROI, dValue值可當背景色
	//                  : CALCULATOR_AND = 11, 兩張影像取交集
	//                  : CALCULATOR_OR = 12, 兩張影像取聯集
	//                  : CALCULATOR_XOR = 13, 兩張影像取 互斥或
	//                  : CALCULATOR_EXTRACT_ROI = 14, 取出ROI範圍的影像, 須設定ROI1
	//                  : CALCULATOR_FILL = 15, 影像填充, 指定顏色填入影像(須設定Mask1) 或 ROI(須設定ROI1), dValue值為填充的顏色
	//                  : CALCULATOR_FILL_BOUNDARY = 16, 指定顏色填入影像外圍邊界, 須設定 ROI1(上下邊界) 與 ROI2(左右邊界)
	//                  : CALCULATOR_FILL_VALUE = 17, 取代指定的顏色 (須設定 nGrayValue 與 dValue 以及 eQModel)	
	//                  : CALCULATOR_FINAL = 18,
	bool ImageCalculate(const SProcessModeInput sInput, SImageCalculatorParam& sParam, SProcessModeOutput& sOutput);
	
	bool CalMean_Mat(const vector<Mat>& vtmatInput, const vector<bool>& vtbOn,  Mat& matMean);

	// 影像填充---基於值
	bool FillImage_Value(const Mat& matInput, const int& GrayValue, const EQualification& eQMode, const int& nFillValue, Mat& matOutput);
#pragma endregion

#pragma region ScaleImage

	// 影像縮放---ok
	// nInterType : 內插模式
	//				0 = CV_INTER_NN,
	//				1 = CV_INTER_LINEAR,
	//				2 = CV_INTER_CUBIC,
	//				3 = CV_INTER_AREA,
	//				4 = CV_INTER_LANCZOS4
	//fScale_X, fScale_Y : 縮放比例
	// matSrc : 原始影像
	// matDst : 縮放後的影像
	int ScaleImage(int nInterType, const float &fScale_X, const float &fScale_Y, const Mat &matSrc, Mat &matDst);
	int ScaleImage(int nInterType, const int &nDst_Width, const int &nDst_Height, const Mat &matSrc, Mat &matDst);

#pragma endregion

#pragma region SQualificationParam

	// 邏輯判斷模式 : QUALIFICATION_NONE = 0, 不處理
	//  		    : QUALIFICATION_GREATER = 1, 大於
	//              : QUALIFICATION_LESS = 2, 小於
	//              : QUALIFICATION_EQUAL = 3, 等於
	//              : QUALIFICATION_GREATER_EQUAL = 4, 大於等於
	//              : QUALIFICATION_LESS_EQUAL = 5, 小於等於
	//              : QUALIFICATION_AND = 6, 邏輯運算符號 &&
	//              : QUALIFICATION_OR = 7, 邏輯運算符號 ||
	//              : QUALIFICATION_NOT = 8, 邏輯運算符號 !	
	//              : QUALIFICATION_FINAL = 9,
	bool Qualification(const SProcessModeInput sInput, SQualificationParam& sParam, SProcessModeOutput& sOutput);
	bool SetValue(const SProcessModeInput sInput, const SQualification_Parameter& sQualification, int& nValue);
	bool Judge(const int& nOutputType, const int& nL, const int& nR, const EQualification& eQualification, bool& bResult);
	bool Judge_Bool(const int& nL, const int& nR, const EQualification& eQualification, bool& bResult);
	bool Judge_Bool(const bool& bL, const bool& bR, const EQualification& eQualification, bool& bResult);
	bool Judge_LInt(const int& nL, const int& nR, const EQualification& eQualification, int& nResult);
	bool Judge_RInt(const int& nL, const int& nR, const EQualification& eQualification, int& nResult);
#pragma endregion

#pragma region FeatureAnalyze

	// FEATURE_NONE = 0, 不處理
	// FEATURE_GLCM = 1, 灰階共生矩陣
	// FEATURE_ANGLE = 2, 計算物件的平面旋轉角度
	// FEATURE_CHAIN_CODE = 3, 計算物件的鍊碼
	// FEATURE_ANALYZE = 4, 特徵分析
	// FEATURE_FINAL = 5,
	bool FeatureAnalyze(const Mat& matImage, const Mat& matMask, Rect cvROI, SFeatureAnalyzeParam& sParam, Mat& matFeature);

	bool GLCM_Analyze(const Mat& matImage, const Mat& matMask, SFeature_GLCM& sParam, Mat& matFeature);

	// 從GLCM計算特徵
	// vtbOn : 目前有14項, 0=熵, 1=一階矩, 2=二階矩, 3=三階矩, 4=四階矩, 5=逆一階矩, 6=逆二階矩, 7=逆三階矩, 8=逆四階矩
	//                     9=對比度, 10=均勻性/能量, 11=同質性, 12=相關性, 13=方差
	bool GLCM_FeatureCalculation(SFeature_GLCM& sParam);
	bool GLCM_Mask(const Mat& matSrc, const Mat& matMask, const SContoursParam* psContours, SGLCM_Param& sParam);
	bool GLCM_ROI(const Mat& matSrc, const SContoursParam* psContours, SGLCM_Param& sParam);

	// 將輪廓畫出
	// matDisplay 為灰階影像---背景為0, 輪廓為255
	bool DrawContours(const SContoursParam& sContours, const vector<bool>& vtbKeep, Mat& matDisplay);

	bool DisplayFeature_GLCM(SFeature_GLCM& sParam, Mat& matDisplay);

	// 計算區域 GLCM
	// matSrc : 灰階影像
	// matMask : 若為空 => 計算vtcvROI範圍
	// vtcvROI : 不可為空, 若matMask不為空 => 只會計算vtcvROI範圍內matMask為255的區域
	bool GLCM(const Mat& matSrc, const Mat& matMask, const vector<Rect>& vtcvROI, const int& nInterval_X, const int& nInterval_Y, const int& nGrayLevel, vector<bool>& vtbKeep, vector<vector<vector<float>>>& vt3fPdf);

	// sParam.first  =>  X Y 偏移
	// sParam.second => GrayLevel
	bool GLCM(const Mat& matSrc, const Mat& matMask, const vector<Rect>& vtcvROI, const vector<pair<POINT, int>>& sParam, vector<vector<vector<vector<float>>>>& vt4fPdf);
	
	// 灰階共生矩陣
	bool GLCM_Kernel(const Mat& matSrc, const int& nInterval_X, const int& nInterval_Y, const int& GrayLevel, bool& bOK, vector<vector<int>>& vtnGLCM, vector<vector<float>>& vtfPdf);
	bool GLCM_Kernel_Mask(const Mat& matSrc, const Mat& matMask, const int& nInterval_X, const int& nInterval_Y, const int& GrayLevel, bool& bOK, vector<vector<int>>& vtnGLCM, vector<vector<float>>& vtfPdf);

	// 從GLCM計算特徵
	// vtbOn : 目前有14項, 0=熵, 1=一階矩, 2=二階矩, 3=三階矩, 4=四階矩, 5=逆一階矩, 6=逆二階矩, 7=逆三階矩, 8=逆四階矩
	//                     9=對比度, 10=均勻性/能量, 11=同質性, 12=相關性, 13=方差
	bool FeatureCalculation_GLCM(const vector<vector<vector<float>>>& vt3fPdf, const vector<bool>& vtbFeatureOn, vector<vector<float>>& vt2fEigenvalues, vector<bool>& vtbOK);
	bool FeatureThreshold_GLCM(const vector<Rect>& vtcvROI, const Mat& matMask, const vector<bool>& vtbFeatureOn, const vector<float>& vtfThres, const vector<bool>& vtbGreaterThan, const vector<vector<float>>& vt2fEigenvalues, vector<bool>& vtbOK, Mat& matThres);

	// 顯示 GLCM 結果
	// vtbProcessOn : 目前有14項, 0=熵, 1=一階矩, 2=二階矩, 3=三階矩, 4=四階矩, 5=逆一階矩, 6=逆二階矩, 7=逆三階矩, 8=逆四階矩
	//                     9=對比度, 10=均勻性/能量, 11=同質性, 12=相關性, 13=方差
	bool FeatureResult_GLCM(const vector<bool>& vtbFeatureOn, const vector<float>& vtfThres, const vector<bool>& vtbGreaterThan, vector<vector<float>>& vt2fEigenvalues, vector<bool>& vtbOK);
	bool OutputReport_GLCM(const Mat& matMask, const string& strPathName, const vector<Rect> vtcvROI, const vector<vector<float>>& vt2fEigenvalues);
	bool OutputReport_GLCM_All(const string& strPath, const vector<string>& vtstrFileName, const vector<vector<Rect>> vt2cvROI, const vector<vector<vector<float>>>& vt3fEigenvalues);

	bool ShowFeature_GLCM(const vector<Rect>& vtcvROI, const vector<bool>& vtbProcessOn, const vector<float>& vtfThres, const vector<bool>& vtbGreaterThan, vector<vector<float>>& vt2fEigenvalues, const vector<bool>& vtbOK, const float& fFontSize, Mat& matShow);
	bool ShowFeature_GLCM(const vector<Rect>& vtcvROI, const vector<bool>& vtbProcessOn, const vector<float>& vtfThres, const vector<bool>& vtbGreaterThan, vector<vector<int>>& vt2nEigenvalues, const vector<bool>& vtbOK, const float& fFontSize, Mat& matShow);

	// 計算最大均勻度
	bool CalMaxUniformity(const int numGrayscaleLevels, float& maxUniformity);

	// 熵
	bool GLCM_Entropy(const vector<vector<float>>& vtfPdf, float& fEntropy);

	// 矩
	bool GLCM_Moment(const vector<vector<float>>& vtfPdf, const int& nOrder, float& fMoment);

	// 逆矩
	bool GLCM_Moment_Inv(const vector<vector<float>>& vtfPdf, const int& nOrder, float& fMoment);

	// 對比度
	bool GLCM_Contrast(const vector<vector<float>>& vtfPdf, float& fContrast);

	// 均勻性/能量
	bool GLCM_Uniformity(const vector<vector<float>>& vtfPdf, float& fUniformity);
	bool GLCM_Uniformity_STD(const vector<vector<float>>& vtfPdf, const int numGrayscaleLevels, float& fUniformity);

	// 同質性
	bool GLCM_Homogeneity(const vector<vector<float>>& vtfPdf, float& fHomogeneity);

	// 相關性
	bool GLCM_Correlation(const vector<vector<float>>& vtfPdf, float& fCorrelation);

	// 方差
	bool GLCM_Variance(const vector<vector<float>>& vtfPdf, float& fVariance);
	
#pragma endregion

#pragma region Pattern Match

	// 預設為 0
	// 圖像比對模式 : PATTERN_NONE = 0,				不處理
	//              : PATTERN_CREATE_PATTERN = 1,	建立 Pattern, 在sInput1中 輸入影像與 ROI; nMethod=1時, pattern 輸出在 sOutput.matImage;
	//                                              nMethod=2時, Mask輸出在sOutput.vtmatImage[0], pattern輸出在sOutput.vtmatImage[1], Pattern ROI輸出在 sOutput.cvROI
	//              : PATTERN_MATCH = 2,			Match, 在sInput1中輸入Pattern , 在sInput2中輸入搜尋影像
	//              : PATTERN_FINAL = 3,
	bool PatternMatch(const SProcessModeInput sInput, SPatternMathParameter& sParam, SProcessModeOutput& sOutput);

	// 從 matImage 中 取出 cvPatternROI 的範圍當作 Pattern
	bool CreatePattern(const Mat& matImage, const int& nMethod, const SRotationParametar& sRotation, const SPatternParameter& sPattern, const vector<Rect>& vtcvPatternROI, vector<Mat>& vtmatPattern, Rect cvHistogramPatternROI = Rect());

	// 
	bool Match(const int& nMethod, const SRotationParametar& sRotation, const SPatternParameter& sPattern, const vector<Mat>& vtmatPattern, const Mat& matSeach, vector<Rect>& vtcvMatchROI, vector<float>& vtfScore);

	// int nS = vtmatPattern.size();
	// vtmatPattern[nS-2] = Histogram Mask, 倒數第二個
	// vtmatPattern[nS-1] = Histogram Pattern, 倒數第一個
	bool CreatePattern_Histogram(const Mat& matImage, const int& nMethod, const Rect& cvPatternROI, const SPatternParameter& sPattern, vector<Mat>& vtmatPattern, Rect& cvHistPatternROI);

	// 使用相關係數 Match
	bool Match_Correlation(const int& nMethod, const SRotationParametar& sRotation, const SPatternParameter& sPattern, const vector<Mat>& vtmatPattern, const Mat& matSeach, vector<Rect>& vtcvMatchROI, vector<float>& vtfScore);

	// 使用直方圖 Match
	bool Match_Histogram(const Mat& matSearch, Mat& matMask_Histogram, Mat& matPattern_Histogram, const Mat& matPattern, const int& nPatternNumber, const bool& bRotate, float& fRotatingAngle, Point2f& cvfptMatchPoint, double& dScore);

	// 使用直方圖 Match --- 大約找出 pattern 位置, matSearch有旋轉時也可以使用
	bool Match_Histogram_First(const Mat& matPattern, const Mat& matSearch, const Mat& matMask, const int& nShiftX, const int& nShiftY, Point2f& cvfptMatchPoint, double& dScore);

	// 使用直方圖 Match --- 一個Pattern 旋轉
	bool Match_Histogram_One_Rotate(const Mat& matPattern, const Mat& matSearch, const Mat& matMask, const int& nShiftX, const int& nShiftY, float& fRotatingAngle, Point2f& cvfptMatchPoint, double& dScore);

	// 輸入角度逆時針旋轉影像(不改變原始圖像大小)
	// matInput : 想要旋轉的影像
	// fDegree : 想要旋轉的角度(正值為逆時針旋轉, 負值為順時針旋轉)
	// matRotate : matInput旋轉後的影像
	bool RotateImage(const Mat& matInput, const float& fDegree, Mat& matRotate);

	// 比較2張影像的 直方圖相似度
	bool CompareHistogram(const Mat& matInput1, const Mat& matInput2, const Mat& matMask, const int& nMethod, double& dScore);

	// 計算旋轉刻度
	// nWidth : 影像的寬
	// nHeight : 影像的高
	// fMinScale : 角度計算的最小刻度
	// fMinShiftX : X方向最小偏移量
	// fMinShiftY : Y方向最小偏移量
	// fRotatingScale : 旋轉刻度
	bool CalculateRotationScale(const int& nWidth, const int& nHeight, const float& fMinScale, const float& fMinShiftX, const float& fMinShiftY, float& fRotatingScale);

	// 將 matPatterm 依旋轉範圍fAngleRange 與旋轉刻度fAngleScale 進行旋轉, 並輸出至 vtmatPattern
	bool RotatePattern(const Mat& matPatterm, const float& fAngleRange, const float& fAngleScale, vector<Mat>& vtmatPattern);

#pragma endregion

#pragma region FeatureFunction

	// 交叉熵---計算兩張影像是否有相同分機率分布
	// dCrossEntropy : 越接近0代表兩張影像越相似
	// matInput1 與 matInput2 須為 灰階影像
	// matMask : 遮罩, 不使用的話輸入 Mat()
	bool CrossEntropy(const Mat& matInput1, const Mat& matInput2, const Mat& matMask, double& dCrossEntropy);

	// 結構相似性指數---計算兩張影像的相似度
	// dSSIM : 值域 [-1,1] 越接近 1 代表兩張影像越相似
	// matInput1 與 matInput2 須為 灰階影像
	bool SSIM(const Mat& matInput1, const Mat& matInput2, double& dSSIM, const int nFilterSize = 11, const float& fSigma = 1.5);

	// 次像素計算
	// ptPoint : 輸入座標
	// matGray : 灰階影像
	// cvptfSubPixel : 次像素座標
	bool CalSubPixel_Gauss(const vector<cv::Point>& vtcvptPoint, const Mat& matGray, vector<cv::Point2d>& vtcvptfSubPixel);
	bool CalSubPixel_Scale(const vector<cv::Point>& vtcvptPoint, const Mat& matGray, vector<cv::Point2d>& vtcvptfSubPixel);

	// 使用PCA計算目標物旋轉角度
	// vtEdgePoint : 目標物邊點(輪廓)
	// dSlope = 目標物的斜率
	// dAngle = 目標物的角度
	// ptcvdCenter = 目標物的中心點
	bool CalAngle_PCA(const vector<vector<POINT>>& vt2ptEdgePoint, double& dSlope, double& dAngle, cv::Point2d& ptcvdCenter);
	bool CalAngle_PCA(const vector<POINT>& vtEdgePoint, double& dSlope, double& dAngle, cv::Point2d& ptcvdCenter);
	bool CalAngle_PCA(const Mat& matEdge, double& dSlope, double& dAngle, cv::Point2d& ptcvdCenter);

	// 使用最小外接矩形計算目標物旋轉角度
	// vtEdgePoint : 目標物邊點(輪廓)
	// fAngle = 目標物的角度
	// ptcvfCorner = 目標物的4個角點, 0=左上, 1=右上, 2=右下, 3=左下
	bool CalAngle_MinAreaRect(const vector<cv::Point>& vtEdgePoint, float& fAngle, cv::Point2f& ptcvfCenter, vector<cv::Point2f>& vtcvptfCorner);

	// bSort = true, 角點位置會重新排序 0=左上, 1=右上, 2=右下, 3=左下
	bool CalAngle_MinAreaRect(const vector<vector<POINT>>& vt2ptEdgePoint, float& fAngle, cv::Point2f& ptcvfCenter, vector<cv::Point2f>& vtcvptfCorner, const bool& bSort=false);
	bool CalAngle_MinAreaRect(const Mat& matEdge, float& fAngle, Point2f& ptcvfCenter, vector<Point2f>& vtcvptfCorner);

	// 計算長寬比
	// vtcvptfCorner : 矩形的四個角點
	// fLongAxis : 長邊
	// fShortAxis : 短邊
	// fAspectRatio : 長寬比
	bool Calculate_AspectRatio(const vector<Point2f>& vtcvptfCorner, float& fLongAxis, float& fShortAxis, float& fAspectRatio);

	int SearchContours_Link(const Mat &matThres, const int &nMaxLength, const int &nMinLength, vector<vector<POINT>> &vtptContours);
	int SearchContours(const Mat &matThres, const bool &bClose, const int &nMaxLength, const int &nMinLength, vector<vector<POINT>> &vtptContours);
	int SearchContours(const Mat& matThres, const bool& bEdge, const bool& bClose, const int& nMinPerimeter, const int& nMaxPerimeter, vector<vector<POINT>>& vtptContours);

	// matImage 須為二值化影像或邊界影像---ok
	// 若bEdge為true => 會進行取邊界
	// 若bClose為true => 只保留封閉邊界的物件
	// 若bSort為true => 會重新進行位置排序
	int SearchEdge(const Mat &matImage, const bool &bEdge, const bool &bClose, const bool &bSort, const int &nMinLength, const int &nMaxLength, vector<int> &vtnPerimeter, vector<vector<vector<POINT>>> &vtEdgePoint);
	bool SearchEdge(const Mat &matImage, const bool &bEdge, const int &nMinLength, const int &nMaxLength, vector<int> &vtnPerimeter, vector<vector<vector<POINT>>> &vtEdgePoint);

	// matEdge : 邊界影像
	// nLineMode : 0 => 搜尋到的邊界全部輸出(相鄰的vtEdgePoint會被記錄到同一個vtnLineIndex) ; 
	//           : 1 => 輸出週長最長的線 , 其餘線的分支都忽略(未完成)
	// vtEdgePoint : 搜尋到的邊界(可能發生一條線被分割成n條邊)
	// vtFeaturePoint : 紀錄邊界的特徵點(端點, 交叉點)
	// vtnLineIndex : 會將屬於同一條線的vtEdgePoint Index 紀錄在一起 ; vtnLineIndex.size()=線的數量
	// vtnPerimeter : 紀錄線的週長
	bool SearchEdge(const int& nLineMode, Mat& matEdge, vector<vector<POINT>>& vtEdgePoint, vector<vector<POINT>>& vtFeaturePoint, vector<vector<int>>& vtnLineIndex, vector<int>& vtnPerimeter);

	// 計算 vtEdgePoint 的外接矩形與中心點---ok
	int GetROI_Rectangle(vector<vector<POINT>>& vtEdgePoint, vector<Rect>& vtcvRectangle, vector<Point>& vtcvptCenter);

	// 計算矩形度
	bool CalRectangularity(const cv::Mat& matThres, const cv::Rect& cvROI, float& fRectangularity);
	bool CalRectangularity(const cv::Mat& matThres, const float& fAngle, const vector<cv::Point2f>& vtcvptfCorner, float& fRectangularity);

	// 計算每條線的 上 下 左 右
	int CalLinePos(const vector<vector<POINT>> &vtptContours, vector<RECT> &vtrtROI);

	// 計算邊的外接矩形
	int GetEdgeROI(const vector<vector<vector<POINT>>>& vtEdgePoint, vector<Rect>& vtcvROI);
	int GetEdgeROI(const vector<vector<POINT>>& vtEdgePoint, vector<Rect>& vtcvROI);

	// 計算最小外接矩形(minimum bounding rectangle, MBR)
	bool CalMBR(const vector<vector<POINT>>& vtEdgePoint, const vector<vector<int>>& vtnLineIndex, vector<Rect>& vtcvROI);

	// 計算輪廓中 矩形 的座標
	int GrabRectangle(vector<vector<POINT>> &vtptContours, vector<RECT>& ROI);

	// 輪廓合併 20210511 Jun+
	int Merge_Contours(vector<vector<POINT>> &vtptContours);

	// 比對2群 POINT的 旋轉角度 偏移量 與互相對應點的數量
	// Input : Std_X=標準影像特徵點的 X座標, Std_Y=標準影像特徵點的 Y座標, Std_Num=標準影像特徵點數量; 
	// Input : InputX=輸入影像特徵點的 X座標, InputY=輸入影像特徵點的 Y座標, InputNum=輸入影像特徵點的數量;
	// Output : angle = 旋轉角度;
	// return : 2張影像相互對應特徵點的數量
	int FeatureMatch(const int &nSearchImageW, const int &nSearchImageH, const vector<POINT> &vtPoint_STD, const vector<POINT> &vtPoint_Input, const float &fAngleRange, const float &fAngleStep, int &nMatch_Num, float &fAngleRotate, int &nShift_X, int &nShift_Y);
	int FeatureMatch(const int &nSearchImageW, const int &nSearchImageH, const vector<pair<string, POINT>> &vtSTD, const vector<pair<string, POINT>> &vtInput, const float &fAngleRange, const float &fAngleStep, vector<int>& vtnIndex);

	// 灰階垂直投影 : matSrc須為灰階格式, bBright為true=> 找大於nThres的部分做投影, vtnProjection_V=垂直投影向量
	// nMode : 1, 單閥值, bDark = true  ; 值域 [0    ,  nLow]
	//					  bDark = false ; 值域 [nLow ,   255]
	//       : 2, 雙閥值, bDark = true  ; 值域 [nLow , nHigh]
	//       :			  bDark = false ; 值域 [0    ,  nLow] + [nHigh , 255]
	bool Projection_Gray_V(const cv::Mat& matGray, const int& nMode, const bool& bDark, const int& nLow, const int& nHeigh, vector<int>& vtnProjection_V);

	// 灰階水平投影 : matSrc須為灰階格式, bBright為true=> 找大於nThres的部分做投影, vtnProjection_H=水平投影向量
	// nMode : 1, 單閥值, bDark = true  ; 值域 [0    ,  nLow]
	//					  bDark = false ; 值域 [nLow ,   255]
	//       : 2, 雙閥值, bDark = true  ; 值域 [nLow , nHigh]
	//       :			  bDark = false ; 值域 [0    ,  nLow] + [nHigh , 255]
	bool Projection_Gray_H(const cv::Mat& matGray, const int& nMode, const bool& bDark, const int& nLow, const int& nHeigh, vector<int>& vtnProjection_H);

	// 區域灰階垂直投影 : matSrc須為灰階格式, bBright為true=> 找大於nThres的部分做投影, vtnProjection_V=垂直投影向量---ok
	int ROI_Projection_Gray_V(const CRect &crectROI, const Mat &matSrc, const int &nThres, const bool &bBright, vector<int> &vtnProjection_V);

	// 區域灰階水平投影 : matSrc須為灰階格式, bBright為true=> 找大於nThres的部分做投影, vtnProjection_H=水平投影向量---ok
	int ROI_Projection_Gray_H(const CRect &crectROI, const Mat &matSrc, const int &nThres, const bool &bBright, vector<int> &vtnProjection_H);

	// 自然對數
	int cvlog(const Mat& matIntput, Mat& matOutput);

	// 計算影像的梯度值---ok
	// matInput : CV_8U
	// matOutput : CV_16S
	int CalcGradient(const Mat &matInput, Mat &matOutput);
	int CalcGradient(const Mat &matInput, const int nSize, Mat &matOutput);
	int Sobel_New(const Mat &matSrc, const int &nFilterSize, Mat &matDst);
	int Sobel(const Mat &matSrc, const int &nFilterSizeX, const int &nFilterSizeY, Mat &matGrad_X, Mat &matGrad_Y);

	// 計算影像的紋路角度
	// bNormal : 若為true => matAngle=法線斜率
	int CalAngleImage(const int nRadius, const Mat &matInput, const Mat &matMask, Mat &matAngle, bool bNormal = false);
	int CalAngleImage(const int nRadius, const Mat &matInput, Mat &matAngle, bool bNormal = false);
	int CalAngleImage(const int nRadius, const Mat &matInput, Mat& matGradX, Mat& matGradY, Mat &matAngle, bool bNormal = false);
	// 計算PSNR
	double GetPSNR(const Mat& matSrc1, const Mat& matSrc2);

	// 鍊碼
	int ChainCode(const vector<POINT>& vtptContours, vector<int>& vtnChainCode);

	// 計算直方圖
	int CalHistogram(const Mat& matSrc, const Mat& matMask, vector<vector<float>>& vtfHist);

	// 二維直方圖
	// nMode = 0 => 區塊平均值
	//       = 1 => 梯度
	int CalHistogram_2D(const Mat& matSrc, const int& nMode, const int& nFilterSizeX, const int& nFilterSizeY, vector<vector<vector<float>>>& vtnHist_Two);

	// 對matSrc進行區塊 GLCM
	bool GLCM_Block(const Mat& matSrc, const int& nCountX, const int& nCountY, const int& nInterval_X, const int& nInterval_Y, const int& GrayLevel, vector<vector<Rect>>& vtcvRange, vector<vector<vector<vector<int>>>>& vt4nGLCM, vector<vector<vector<vector<float>>>>& v4tfPdf);

	// nMode=1 =>矩(GLCM_Moment)
	// nMode=2 =>熵(GLCM_Entropy)
	bool Analyze_GLCM_Block(const vector<vector<vector<vector<float>>>>& vt4fPdf, const int& nMode, const int& nOrder, vector<vector<float>>& vtfEigenvalues);
	bool Threshold_GLCM_Block(const Mat& matSrc, const vector<vector<Rect>>& vtcvRange, const vector<vector<float>>& vtfEigenvalues, const int& nMode, const float& fThres, const bool& bGreaterThan, Mat& matThres);

	void DBSCAN(std::vector<cv::Point>& points, double eps, int minPts, std::vector<int>& labels);
	void DBSCAN(std::vector<cv::Rect>& rects, double eps, int minPts, std::vector<int>& labels);

	// 計算分形維度
	double calculateFractalDimension(const Mat& image, int minBoxSize, int maxBoxSize);

	// 計算分形維度特徵圖
	Mat calculateFractalDimensionMap(const Mat& image, int windowSize, int step);

	bool Threshold_FractalDimension(const Mat& image, int windowSize, int step, float epsilon, int minPoints, Mat& segmented);
#pragma endregion

#pragma region Rotate

	// 影像旋轉---ok
	// bRight : 若為true 代表順時針旋轉, false代表逆時針 
	// nMode : 1=旋轉0度, 2=旋轉90度, 3=旋轉180度, 4=旋轉270度
	void ImageRotate(bool bRight, int nMode, const Mat &matInput, Mat &matRota);

	// 影像 旋轉 偏移 縮放---ok
	// matInput : 想要旋轉的影像
	// dDegree : 想要旋轉的角度(正值為逆時針旋轉, 負值為順時針旋轉)
	// dScale : 影像的縮放比例
	// ptShift : 旋轉後偏移的距離
	// matRotate : matInput旋轉後的影像
	int ImageAffine(const Mat &matInput, const float &fDegree, const float &fScale, const POINT &ptShift, Mat &matRotate);

	// Point旋轉---ok
	// bRight : 若為true 代表順時針旋轉, false代表逆時針 
	// nRoateMode : 1=旋轉0度, 2=旋轉90度, 3=旋轉180度, 4=旋轉270度
	void Rotate_Point(const bool bRight, const int nRoateMode, const int &nImageW, const int &nImageH, const POINT &Point_Befoer, POINT &Point_After);

	// ROI旋轉---ok
	// bRight : 若為true 代表順時針旋轉, false代表逆時針 
	// nMode : 1=旋轉0度, 2=旋轉90度, 3=旋轉180度, 4=旋轉270度
	void Rotate_OCR_ROI(const bool bRight, const int nRoateMode, const int &nImageW, const int &nImageH, const RECT &rectROI_Befoer, RECT &rectROI_After);

	// 點旋轉
	void PointRotate(const int& nImageW, const int& nImageH, const cv::Point2f& cvfCenter, const Point2f& ptInput, const float& fRadian, Point2f& ptOutput);

	//  使用 SSE
	int ImageTransform_SIMD(const bool &bRight, const int &nMode, const int &nH, const int &nW, const int &nC, unsigned char* pu8Src, unsigned char *pu8Dst);
	int ImageTransform_SIMD_4(bool bRight, int nMode, const Mat &matInput, Mat &matRota);
	int ImageTransform_180_SSE_4(const int &nH, const int &nW, const int &nC, const int &nStartX, const int &nEndX, const int &nStartY, const int &nEndY, unsigned char* pu8Src, unsigned char *pu8Dst);
	int ImageTransform_90_SSE_8(bool bRight, const int &nH, const int &nW, const int &nC, const int &nStartX, const int &nEndX, const int &nStartY, const int &nEndY, unsigned char* pu8Src, unsigned char *pu8Dst);
	int ImageTransform_90_SSE_16(const bool &bRight, const int &nH, const int &nW, const int &nC, unsigned char* pu8Src, unsigned char *pu8Dst);
	int ImageTransform_180_SSE_8(const int &nH, const int &nW, const int &nC, const int &nStartX, const int &nEndX, const int &nStartY, const int &nEndY, unsigned char* pu8Src, unsigned char *pu8Dst);
	int ImageTransform_180_SSE_16(const int &nH, const int &nW, const int &nC, unsigned char* pu8Src, unsigned char *pu8Dst);
	
	// 使用 AVX
	int ImageTransform_SIMD_AVX(bool bRight, int nMode, const Mat &matInput, Mat &matRota);

	// 使用多執行緒
	int ImageTransform_Thread(bool bRight, int nMode, const int &nThreadCount, const Mat &matInput, Mat &matRota);
	void ImageTransform_Thread_Part(const bool &bRight, const int &nMode, const int &nH, const int &nW, const int &nC, const int &nStartY, const int &nEndY, unsigned char* pu8Src, unsigned char *pu8Dst);

	// 使用 OP MP
	int ImageTransform_OMP(const bool &bRight, const int &nMode, const int &nH, const int &nW, const int &nC, unsigned char* pu8Src, unsigned char *pu8Dst, const int &nThreadCount);
	void ImageTransform_OMP_Part(const bool &bRight, const int &nMode, const int &nH, const int &nW, const int &nC, const int &nSumThread, const int &nId, unsigned char* pu8Src, unsigned char *pu8Dst);

#pragma endregion

#pragma region LinearEquation

	// 已知 dSlope 與 ptPoint1 以及 ptPoint2.x 計算 ptPoint2.y
	bool CalculatePointY(const double& dSlope, const POINT& ptPoint1, POINT& ptPoint2);

	// 已知 dSlope 與 ptPoint1 以及 ptPoint2.y 計算 ptPoint2.x
	bool CalculatePointX(const double& dSlope, const POINT& ptPoint1, POINT& ptPoint2);

	// 兩條線 L1, L2 正交; 已知 L1斜率與捷距, 與 L2上一點 P2; 計算兩線的交點P1, 且L2需通過P2
	bool CalculateIntersection(const double& dSlope, const double& dIntercept, const POINT& ptP2, POINT& ptP1);

	// 計算2直線方程式的交點--ok
	int GetCrossPoint(const double &dA1, const double &dB1, const double &dA2, const double &dB2, Point &cvCrossPoint);
	int GetCrossPoint(const double &dA1, const double &dB1, const double &dA2, const double &dB2, Point2f &cvCrossPoint);

	// 點到線的直線(最短)距離---ok
	// P(x0, y0)  L:ax + by + c = 0
	// pt = 點的位置
	// dSlope = 線的斜率
	// dIntercept = 線的截距
	// dDist = 點到線的直線(最短)距離
	int DistPointToLine(const Point &pt, const double &dSlope, const double &dIntercept, double &dDist);
	int DistPointToLine(const vector<Point> &vtptPoint, const double &dSlope, const double &dIntercept, vector<double> &vtdDist);
	int DistPointToLine(const Point2f &pt, const double &dSlope, const double &dIntercept, double &dDist);
	int DistPointToLine(const vector<Point2f> &vtptPoint, const double &dSlope, const double &dIntercept, vector<double> &vtdDist);
	double DistPointToLine(const double& dX, const double& dY, const double& dSlope, const double& dIntercept);
	bool DistPointToLine(const vector<double>& vtdX, const vector<double>& vtdY, const double& dSlope, const double& dIntercept, vector<double> &vtdDist);

	// 求 兩圓交點
	// C1 = 圓1 的圓心
	// R1 = 圓1 的半徑
	// C2 = 圓2 的圓心
	// R2 = 圓2 的半徑
	// flP1x = 交點1的 X座標
	// flP1y = 交點1的 Y座標
	// flP2x = 交點2的 X座標
	// flP2y = 交點2的 Y座標
	// return =1代表有一解; =2有2解; =0無解; =-1程式異常
	int IntersectionOfTwoCircles(POINT C1, int R1, POINT C2, int R2, float &flP1x, float &flP1xy, float &flP2x, float &flP2y);

	// 不共線3點 算夾角角度---ok
	int Angle_3Point(const int &nMinLength, const int &nMaxLength, const vector<POINT> &vtPoint, vector<double> &vtdAngle);
	int Angle_3Point(const int &nMinLength, const int &nMaxLength, const vector<POINT> &vtPoint, vector<vector<float>> &vtfAllLength_Angle);

	int Matrix_Multiplication(const Mat &matrixL, const Mat &matrixR, Mat &matResult);

	int Matrix_Transpose(const Mat &matSrc, Mat &matDst);
	// T = S * M
	// M = (ST * S)^-1 * (ST * T)
	int Matrix_LeastSquares(const Mat &matrix_T, const Mat &matrix_S, Mat &matrix_M);

	// 計算矩陣相乘---ok
	int Matrix_Multiplication(double *pdInputMatrix_Left, double *pdInputMatrix_Right, double *pdOutputMatrix, int m, int n, int p);

	// 計算反矩陣---ok
	int Matrix_Inverse(double *pdInputMatrix, double *pdOutputMatrix, int n);

	int Matrix_Transpose(double *pdInputMatrix, double *pdOutputMatrix, int m, int n);

#pragma endregion

#pragma region Regression

	// 計算線性方程式 y = dSlope * x + dIntercept(單次)
	// vtdCoordinate_X = 每一點的X座標
	// vtdCoordinate_Y = 每一點的Y座標
	// dSlope = 輸出方程式的斜率
	// dIntercept = 輸出方程式的截距
	bool SimpleLinearRegr(const vector<double>& vtdCoordinate_X, const vector<double>& vtdCoordinate_Y, double &dSlope, double &dIntercept);
	bool SimpleLinearRegr(const vector<double>& vtdCoordinate_X, const vector<double>& vtdCoordinate_Y, const int& nSumTimes, const double& dSlopeDiff, const double& dErrorPercentage, double &dSlope, double &dIntercept);

	// 計算線性方程式(不包含垂直線) y = dSlope * x + dIntercept(遞迴)
	// vtdCoordinate_X = 點的X座標
	// vtdCoordinate_Y = 點的Y座標
	// nSumTimes = 遞迴的最大次數
	// dDiff = 前後2次的斜率差若小於 dDiff, 則不再計算 
	// dSlope = 輸出方程式的斜率
	// dIntercept = 輸出方程式的截距
	bool SimpleLinearRegr(const vector<POINT>& vtptPoint, const int nSumTimes, const double dDiff, double& dSlope, double& dIntercept);
	bool SimpleLinearRegr_Vertical(const vector<POINT>& vtptPoint, const int nSumTimes, const double dDiff, double& dSlope, double& dIntercept);

	// 計算線性方程式 y = dSlope * x + dIntercept---ok
	// vtcvPoint = 每一點的座標
	// dSlope = 輸出方程式的斜率
	// dIntercept = 輸出方程式的截距
	int SimpleLinearRegr(vector<Point> &vtcvPoint, double &dSlope, double &dIntercept);
	int SimpleLinearRegr(vector<Point2f> &vtcvPoint, double &dSlope, double &dIntercept);

	// 遞迴 計算線性方程式(不包含垂直線) y = dSlope * x + dIntercept---ok
	// vtcvPoint = 每一點的座標
	// nSumTimes = 遞迴的最大次數
	// dDiff = 前後2次的斜率差若小於 dDiff, 則不再計算 
	// dSlope = 輸出方程式的斜率
	// dIntercept = 輸出方程式的截距
	int SimpleLinearRegr(vector<Point> &vtcvPoint, int nSumTimes, double dDiff, double &dSlope, double &dIntercept, double dErrorThres = 0.0);
	int SimpleLinearRegr(vector<Point2f> &vtcvPoint, int nSumTimes, double dDiff, double &dSlope, double &dIntercept, double dErrorThres = 0.0);

	// 計算垂直線的線性方程式 y = dSlope * x + dIntercept---ok
	// vtcvPoint = 每一點的座標
	// nSumTimes = 遞迴的最大次數
	// dDiff = 前後2次的斜率差若小於 dDiff, 則不再計算 
	// dSlope = 輸出方程式的斜率
	// dIntercept = 輸出方程式的截距
	int SimpleLinearRegr_Vertical(vector<Point> &vtcvPoint, int nSumTimes, double dDiff, double &dSlope, double &dIntercept);
	int SimpleLinearRegr_Vertical(vector<Point2f> &vtcvPoint, int nSumTimes, double dDiff, double &dSlope, double &dIntercept);

	// 計算線性方程式(不包含垂直線) y = dSlope * x + dIntercept(遞迴)
	// vtdCoordinate_X = 每一點的X座標
	// vtdCoordinate_Y = 每一點的X座標
	// nSumTimes = 遞迴的最大次數
	// dDiff = 前後2次的斜率差若小於 dDiff, 則不再計算 
	// dPercentage = 刪除誤差數量的百分比
	// dSlope = 輸出方程式的斜率
	// dIntercept = 輸出方程式的截距
	bool SimpleLinearRegr(const vector<Point>& vtcvptPoint, const int& nSumTimes, const double& dDiff, const double& dPercentage,  double& dSlope, double& dIntercept);
	bool SimpleLinearRegr_Vertical(const vector<Point>& vtcvptPoint, const int& nSumTimes, const double& dDiff, const double& dPercentage, double& dSlope, double& dIntercept);

	// 多項式回歸
	bool PolynomailRegression(const vector<Point2f>& vtcvPoint, const int& nPower, vector<double>& vtdCoefficient);

	// 圓迴歸
	// vtcvpPoint = 圓的邊界
	// dRate = 縮放倍率
	// (vtdB[0],vtdB[1])  = 圓心(x, y)
	// vtdB[2] = 半徑
	void CircularRegression(const vector<Point> &vtcvpPoint, double dRate, vector<double> &vtdB);
	void CircularRegression(const vector<POINT> &vtPoint, const double& dRate, Point2d& Center, double& dRadius);
	void CircularRegression(const vector<POINT> &vtPoint, const double& dRate, const int& nMaxTimes, Point2d& Center, double& dRadius);

	// 橢圓回歸 Ax^2 + Bxy + Cy^2 + Dx + Ey = -1 
	// (x^2 / A^2) + (y^2 / B^2) = 1, A > B > 0 (左右型)
	// (x^2 / B^2) + (y^2 / A^2) = 1, A > B > 0 (上下型)
	int EllipseRegression(const std::vector<POINT>& vtPoint, double& dCenterPX, double& dCenterPY, double &dThta, double &dAxis1, double &dAxis2);
	int EllipseRegression(const std::vector<POINT>& vtPoint, const double& dRate, Point2d& Center, double &dThta, double &dAxis1, double &dAxis2);
	bool EllipseRegression(const std::vector<POINT>& vtPoint, const int& nMaxTimes, const double& dError, const double& dRate, double& dCenterPX, double& dCenterPY, double& dThta, double& dAxis1, double& dAxis2);

	// Ax^2 + Bxy + Cy^2 + Dx + Ey + F = 0
	bool EllipseRegression_New(const std::vector<POINT>& vtPoint, double& dCenterX, double& dCenterY, double& dThta, double& dLongAxis, double& dShortAxis);

	// 橢圓迴歸---遞迴
	bool RecursiveEllipseRegression(std::vector<POINT>& vtPoint, double& dCenterX, double& dCenterY, double& dThta, double& dLongAxis, double& dShortAxis, double errorThreshold, int maxIterations);

	// 計算仿射轉換係數---ok
	// U = a11*x + a12*y + tx
	// V = a21*x + a22*y + ty
	// vtInputPoint.first : 原始座標
	// vtInputPoint.second : 目的座標
	// a11=vtdCoefficient[0], a12=vtdCoefficient[1], tx=vtdCoefficient[2]
	// a21=vtdCoefficient[3], a22=vtdCoefficient[4], ty=vtdCoefficient[5]
	int GetAffine(const vector<pair<POINT, POINT>> &vtInputPoint, double dRate, vector<double> &vtdCoefficient);
	int GetAffine(const vector<pair<Point2f, Point2f>> &vtInputPoint, double dRate, vector<double> &vtdCoefficient);

	// 進行仿射轉換---ok
	// U = a11*x + a12*y + tx
	// V = a21*x + a22*y + ty
	// a11=vtdCoefficient[0], a12=vtdCoefficient[1], tx=vtdCoefficient[2]
	// a21=vtdCoefficient[3], a22=vtdCoefficient[4], ty=vtdCoefficient[5]
	int AffineTransform(const vector<double> &vtdCoefficient, const POINT &ptInput, POINT &poOutPt);
	int AffineTransform(const vector<double> &vtdCoefficient, const Point2f &ptInput, Point2f &ptOutPt);

	// 3維 仿射轉換
	// U = a11*x + a12*y + a13*z + tx
	// V = a21*x + a22*y + a23*z + ty
	int GetAffine_3D(const vector<pair<Point3f, Point3f>> &vtInputPoint, double dRate, vector<double> &vtdCoefficient);

#pragma endregion

#pragma region ImageFusion

	// 多焦點影像融合---速度快效果差(雜訊多)
	bool MultiFocusImageFusion(const vector<Mat>& vtmatImage, Mat& matReasult);

	// 基於 Gauss Laplacian 金字塔的影像融合---速度慢效果好
	// nLevels : Number of pyramid levels
	bool LaplacianPyramidImageFusion(const vector<Mat>& vtmatImage, const int& nLevels, const int& nShift, vector<Mat>& vtmatIndex, Mat& matReasult);
#pragma endregion

#pragma region ClusterOutput
	// 分群輸出
	bool ClusterOutput(const vector<pair<float, float>>& vtsResult, const SClusterOutputParameter& sClusterOutput, float& fMin, float& fMax);
#pragma endregion

#pragma region Kmean
	// k-mean分群
	// nCount_Group = 分群數, 必需 >=2
	// fScale = 影像縮小比例(0.1-1.0)
	// vtsInfo.first = 平均值
	// vtsInfo.second = 數量
	bool Image_Kmean(const Mat &matImage, const int& nCount_Group, const float& fScale, vector<pair<float, float>>& vtsResult);
	bool Image_Kmean(const Mat &matImage, const Mat &matMask, const int& nCount_Group, const float& fScale, vector<pair<float, float>>& vtsResult);

	bool Image_Kmean(const Mat &matImage, SKmeanParameter& skmean, const Mat &matMask=Mat());
#pragma endregion

#pragma region DBSCAN
	// nMode : 計算距離的模式, 1=>僅使用灰度值
	//                         2=>使用灰度值和頻率的平方距離
	bool Image_DBSCAN(const Mat &matImage, SDbscanParameter& sDbscan, const Mat &matMask = Mat());

#pragma endregion

#pragma region PCA Info

	// 顯示 PCA 特徵向量
	bool DisplayPcaInfo(const string& strSavePath, const string& strName, const cv::Mat& matPcaData, const cv::Mat& matEigenvectors, const cv::Mat& matMean, const int nEigenvectorIndex, const int nSampleFlags, const vector<POINT>& vtptPlaneIndex, vector<cv::Mat>& vtmatDisplay);
	bool DisplayPcaInfo(const Mat& matPcaData, const Mat& matEigenvectors, const int nEigenvectorIndex, const int nSampleFlags, vector<Mat>& vtmatDisplay);

	// 紀錄 PCA 的資訊
	bool SavePcaInfo(const string& strPath, const string& strName, const PCA& pca);
#pragma endregion

#pragma region ScanningLine

	// 直線掃描轉換
	// nImageW : 影像寬
	// nImageH : 影像高
	// ptStart : 直線起始點
	// ptEnd : 直線終點
	// vtptLine : 起始點到終點所經過的點位
	int ScanningLine(const int &nImageW, const int &nImageH, const POINT &ptStart, const POINT &ptEnd, vector<POINT> &vtptLine);

#pragma endregion

#pragma region Display

	// 在 matImage上畫出 vtPoint 的每一點---ok
	void DrawEdgePoint(Mat matImage, vector<cv::Point> vtPoint, Scalar scColor, const int nPointRadius = 0);
	void DrawEdgePoint(Mat matImage, vector<cv::Point2f>& vtPoint, Scalar scColor, const int nPointRadius = 0);
	// 在 matImage上畫出 vtptLine 的每一點---ok
	// nPointRadius = 0, 代表一個點
	// nPointRadius = 1, 代表 3x3
	void DrawEdgePoint(Mat &matImage, const vector<POINT> &vtptLine, const Scalar &scPointColor, const int &nPointRadius);

	// 顯示邊界點
	bool DisplayEdge(const vector<vector<POINT>> &vtptContours, const int &nImageWidth, const int &nImageHeight, Mat &matContours);

	// 依點位的順序來顯示
	void DrawContour_Sequence(Mat &matImage, const vector<POINT> &vtptLine, const int &nPointRadius);

	bool ShowEdge(const vector<vector<POINT>>& vtEdgePoint, const vector<vector<int>>& vtnLineIndex, Mat& matShow);
	
	// 標記每一條線段
	// matLabel : 參數輸入時若有資料, 則會當做背景影像
	//          : 參數輸入時若無資料, 則會當做全黑影像
	//          : 輸出時為結果影像  
	// nImageWidth : 輸出影像的寬
	// nImageHeight : 輸出影像的高
	int LabelLine(const vector<vector<POINT>> &vtptLine, const Scalar &scColor, const int &nPointRadius, const int &nImageWidth, const int &nImageHeight, Mat &matLabel);
	int LabelLine(const vector<vector<vector<POINT>>> &vtptLine, const Scalar &scColor, const int &nPointRadius, const int &nImageWidth, const int &nImageHeight, Mat &matLabel);
	bool LabelLine(Mat& matImage, Mat& matLine, const Scalar &scColor, const int &nPointRadius, Mat& matShow);

	// 取出標籤影像
	// matImage : 原始影像(彩色 灰階皆可)
	// bEnhan : 若為true 會增強 matLabel 影像
	// cvROI : 輸出標籤影像在matImage中的範圍
	// matLabel : 輸出標籤影像(灰階)
	int GrabLabelImage(const Mat& matEdge, const bool& bEnhan, Rect& cvROI, Mat& matLabel);

	// 畫格線---ok
	void DrawGridLine(int nGrid_Width, int nGrid_Height, int nLineWidth, int nBackgroundValues, int nLineValue, Mat &matImage);

	// 在指定位置畫交叉線---ok
	bool DrawCrossPoint(const POINT &ptPosition, const int &nLength, const int &nLineWidth, const Scalar &scLineColor, Mat &matImage);
	void DrawCrossLine(const POINT &ptPosition, const int &nLength, const int &nLineWidth, const Scalar &scLineColor, Mat &matImage);
	void DrawCrossLine(const Point2f &cvptfPosition, const int &nLength, const int &nLineWidth, const Scalar &scLineColor, Mat &matImage);

	// 畫矩形
	int DrawRectangle(const Mat& matImage, const Rect& cvROI, const Scalar& scColor, const int& nLineWidth, Mat& matResult);
	int DrawRectangle(const Mat& matImage, const POINT& ptCenter, const int& nRH, const int& nRW, const Scalar& scColor, const int& nLineWidth, Mat& matResult);
	int DrawRectangle(const Mat& matImage, const vector<POINT>& vtptCenter, const int& nRH, const int& nRW, const Scalar& scColor, const int& nLineWidth, Mat& matResult);

	// 畫出投影Data
	void Draw_Projection(const bool& bVertical, const vector<int>& vtnProjection, Mat& matImage);
	void Draw_Projection(const vector<int>& vtnProjectionV, const vector<int>& vtnProjectionH, Mat& matImage);

	// 畫鍊碼
	int DrawChainCode(const vector<POINT>& vtptContours, const vector<int>& vtnChainCode, Mat& matColor);

	// 畫直方圖
	int DrawHistogram(const Mat& matSrc, const Mat& matMask, Mat& matHistogram);

	// 畫二維直方圖
	int DrawHistogram_2D(vector<vector<vector<float>>>& vtnHist_Two, vector<Mat>& vtmatHistogram);

	// 畫霍式轉換
	int HoughTransform_Line(Mat& matSrc, const double dError_Dist, const double& dError_Angle, const int& nError_Count, std::vector<double>& vtdSlope, std::vector<double>& vtdIntercept);

	// 顯示邊界影像
	bool DisplayEdge(const Mat& matEdge, const Scalar& scColor, const Mat& matBackground, Mat& matDisplay);

	// 將 vtEdgePoint 依 vtbShow 設定畫在 matImage 上
	// matImage : 不得為空, 且為灰階
	bool DrawMask(const vector<Rect>& vtcvROI, const vector<bool>& vtbShow, Mat& matImage);
	bool DrawMask(const vector<Rect>& vtcvROI, const vector<bool>& vtbShow, const Mat& matThres, Mat& matMask);
	bool DrawEdge(const vector<vector<POINT>>& vtEdgePoint, const vector<vector<int>>& vtnLineIndex, vector<bool>& vtbShow, Mat& matImage);
	bool DrawEdge_Color(const vector<vector<POINT>>& vtEdgePoint, const vector<vector<int>>& vtnLineIndex, vector<bool>& vtbShow, Mat& matImage);

#pragma endregion

#pragma region Sort

	// POINT 以 X 來排序 小->大
	bool SortPointX(const POINT &A, const POINT &B);

	// POINT 以 Y 來排序 小->大
	bool SortPointY(const POINT &A, const POINT &B);
#pragma endregion

#pragma region etc
	bool Add_POINT(const POINT& ptAdd, int nCurrectId, int nAddCount, vector<POINT>& vtptData);
#pragma endregion

#pragma region Rectangle

	// cv::Rect 擴大 或 縮小
	// nAddY nAddX : 外擴或內縮的距離(pixels), nAdd>0 => 外擴 ; nAdd<0 => 內縮
	// nImageH nImageW : 影像寬高, 避免外擴時超出範圍, nImageH nImageW皆為0時不做任何檢察
	bool Change_Rect(Rect& cvROI, const int& nAddY, const int& nAddX, const int& nImageH=0, const int& nImageW=0);

	// SJRect 轉 Rect
	// nAdd : 外擴或內縮距離(pixels), nAdd>0 => 外擴 ; nAdd<0 => 內縮
	// nImageH nImageW : 影像寬高, 檢察外擴(內縮)後是否有超出範圍, nImageH nImageW皆為0時不做任何檢察
	bool SJRectToRect(const SJRect& sjRect, const int& nAddY, const int& nAddX, Rect& cvROI, const int& nImageH = 0, const int& nImageW = 0);

	// RECT 轉 Rect 
	// nAdd : 外擴或內縮距離(pixels), nAdd>0 => 外擴 ; nAdd<0 => 內縮
	// nImageH nImageW : 影像寬高, 檢察外擴(內縮)後是否有超出範圍, nImageH nImageW皆為0時不做任何檢察
	bool RECTToRect(const RECT& reROI, const int& nAddY, const int& nAddX, Rect& cvROI, const int& nImageH = 0, const int& nImageW = 0);

	// 檢察 RECT 內容是否正確
	// nImageH nImageW : 影像寬高, 檢察是否有超出範圍
	bool Check_RECT(const int& nImageH, const int& nImageW, const RECT& reROI);

	// 如果 ROI 範圍異常, 則會修改 ROI
	bool Fix_RECT(const int& nImageH, const int& nImageW, RECT& rectROI);

	// 檢察 Rect 內容是否正確
	bool Check_Rect(const int& nImageH, const int& nImageW, const cv::Rect& cvROI);

	// 如果 ROI 範圍異常, 則會修改 ROI
	bool Fix_Rect(const int& nImageH, const int& nImageW, cv::Rect& cvROI);

	// 如果 sjROI 範圍異常, 則會修改 sjROI
	bool Fix_SJRect(const int& nImageH, const int& nImageW, SJRect& sjROI);

	// 輸入中心點與範圍, 重新建立 cvROI
	bool ReCreateRect(const POINT& ptCenter, const int& nImageH, const int& nImageW, const int& nROI_Height, const int& nROI_Width, Rect& cvROI);

#pragma endregion

#pragma region DataStruct

	// 將3個channel的源圖拆成3個單channel
	// matSrc :channel必須 >= 3
	// nType : 1=>使用opencv split(), 2=>使用opencv extractChannel()與insertChannel(), 3=>直接記憶體複製
	// vtmatChannel : 拆解後單 Channel的圖
	bool SplitChannel(const Mat& matSrc, vector<Mat>& vtmatChannel, const int& nType = 1);

	// 將3個單channel的源圖合併成ㄧ個3 channel的圖
	// vtmatChannel : 單channel的源圖
	// matDst :合併後 3個channel的圖
	// nType : 1=>使用opencv split(), 2=>使用opencv extractChannel()與insertChannel(), 3=>直接記憶體複製
	bool MergeChannel(const vector<Mat>& vtmatChannel, Mat& matDst, const int& nType = 1);

	// 將 matInput中的區塊 建立一個新的 Mat變數---ok
	int CreateROIMat(const Mat &matInput, const CvRect &cvROI, Mat &matOutput);

	// 將 pu8Image 影像轉成 Mat型態(共用同一塊記憶體)---ok
	// nImageWidth = pu8Image影像的寬
	// nImageHeight = pu8Image影像的高
	// nBit = pu8Image影像每一pixels的 bit數(8bit = 256)
	// nChannel = pu8Image影像若為灰階則為1, 彩色為3
	// matImage = pu8Image型態轉換後的變數
	int ImageBufferToMat(int nImageWidth, int nImageHeight, int nBit, int nChannel, const unsigned char* pu8Image, Mat &matImage);

	// vtpu8Image[0] = B---ok
	// vtpu8Image[1] = G
	// vtpu8Image[2] = R
	int ImageBufferToMat(int nImageWidth, int nImageHeight, int nBit, vector<const unsigned char*> vtpu8Image, Mat &matImage);

	// 20230324
	wstring string2wstring(const string& strData);
	string wstring2string(const wstring& wstrData);

	/**
	 * 將浮點數格式化為字串，整數、小數部分可指定長度，並可選擇四捨五入或截斷，不足時補零，最後可加字串後綴。
	 * @param intLength   整數部分的最小長度（左補0）。
	 * @param fracLength  小數部分長度（右補0）。
	 * @param value       要格式化的數值。
	 * @param suffix      字串後綴（如單位）。
	 * @param bRound      true: 四捨五入，false: 截斷。
	 * @return            格式化後的字串。
	 */
	std::string FloatingToString(const int intLength, const int fracLength, const double value, const std::string& suffix = "", const bool bRound = false);

	// 將浮點數格式化為字串
	// format：格式化字串，例如 "%04.3f"
	// value：要格式化的浮點數值
	string FloatingToString(const char* format, double value);

#pragma endregion

	// 只畫一點---ok
	void DrawPoint(POINT ptPosition, Scalar scValue, Mat &matImage);

#ifdef FOR_MACHINE
	#if FOR_MACHINE == JET_MACHINE_7000

	// 計算 vtEdgePoint 的外接矩形與中心點---ok
	int GetROI_Rectangle(vector<vector<cv::Point> > &vtEdgePoint, vector<CRect> &vtcretRectangle, vector<Point> &vtcvptCenter);


#pragma region CameraCalibrate

	// 彩色棋盤影像轉成灰階影像
	// nMode : 彩色轉灰階模式,  0 => 由 sPM 決定
	//                          1 => 使用預設模式1
	// matCheckerboard : 棋盤影像, 如果是灰階影像會直接輸出給 matGray
	//                             如果是彩色影像會依 nMode 設定轉灰階
	// matGray : 最終輸出的灰階影像
	bool CheckerboardImageConvertGray(const int nMode, const Mat& matCheckerboard, Mat& matGray, SProcessModeParam& sPM = SProcessModeParam());

	// 找出棋盤影像中的角點
	// vtmatGray : 棋盤影像, 須為灰階
	// siChessboard : 棋盤上每列(水平方向) 每行(垂直方向)的角點數
	// siSquare : 棋盤格在影像中的大小(pixels)
	// nSubpixType : 次像素的方法 : 1 => cornerSubPix() : 通用的角點精煉算法，適用於廣泛的角點特徵
	//                              2 => find4QuadCornerSubpix() : 專為棋盘格這樣的四邊形（正方形或矩形模式）特徵進行了優化
	// vt2ptfChessboardCorners : 角點座標
	bool FindChessboardCorners(const Mat& matGray, const Size& siChessboard, const int& nSubpixType, vector<Point2f>& vtptfChessboardCorners);
	bool FindChessboardCorners(const vector<Mat>& vtmatGray, const Size& siChessboard, const Size& siSquare, const int& nSubpixType, vector<vector<Point2f>>& vt2ptfChessboardCorners);

	// 計算相機內參
	bool CameraCalibrate(const vector<vector<Point2f>>& vt2ptfChessboardCorners, const Size& siChessboard, const Size& siImage, Mat& matMameraMatrix, Mat& matDistCoeffs, vector<Mat>& vtmatRvecs, vector<Mat>& vtmatTvecs, double& dRms);

	// 校正傾斜影像
	bool CorrectPerspective(const Mat& matImage, const Size& siChessboard, float fSquareRatio, const Mat& matCameraMatrix, const Mat& matDistCoeffs, const Mat& matRvecs, const Mat& matTvecs, Mat& matCorrectedImage);

	// 計算投影矩陣 : 相機1 投影到 相機2
	// vtptfChessboard1 : 相機1拍的棋盤影像
	// vtptfChessboard2 : 相機2拍的棋盤影像
	bool CalPerspectiveMatrix(const Size& siChessboard, const vector<Point2f>& vtptfChessboard1, const vector<Point2f>& vtptfChessboard2, Mat& matPerspectiveMatrix);

	// 輸入影像與投影矩陣進行投影
	bool PerspectiveImage(const Mat& matImage, const Mat& matPerspectiveMatrix, Mat& matPerspectiveImage);

	// 鏡頭變形校正 + 傾斜校正
	bool CalibrateImage(const Mat& matImage, const Size& siChessboard, float fSquareRatio, const Mat& matCameraMatrix, const Mat& matDistCoeffs, const Mat& matRvec, const Mat& matTvec, Mat& matCorrectedImage);

	// 立體視覺校正
	bool StereoCalibrate(const Size& siChessboard, const vector<vector<Point2f>>& vtpt2fPoints1, const vector<vector<Point2f>>& vtpt2fPoints2, const Size& siImage, Mat& matCameraMatrix1, Mat& matDistCoeffs1,
		Mat& matCameraMatrix2, Mat& matDistCoeffs2, Mat& matR, Mat& matT, Mat& matE, Mat& matF);

	// 立體視覺處理函數
	bool StereoVision(const Mat& matImageL, const Mat& matImageR, const Mat& matCameraMatrix1, const Mat& matDistCoeffs1, const Mat& matCameraMatrix2, const Mat& matDistCoeffs2, const Mat& matR, const Mat& matT);

	// 顯示棋盤影像的角點
	bool DisplayCorners(const vector<Point2f>& vtptfCorners, const int& nLength, const int& nLineWidth, const Scalar& scLineColor, const double& dFontScale, const Scalar& scFontColor, Mat& matImage, bool bInfo = true);

	// 儲存相機校正參數
	bool SaveCameraCalibrate(const string& strPath, const string& strName, const Mat& matCameraMatrix, const Mat& distCoeffs, vector<Mat>& rvecs, vector<Mat>& tvecs, const double& dRms);

	// 讀取相機校正參數
	bool LoadCameraCalibrate(const string& strPath, const string& strName, Mat& matCameraMatrix, Mat& distCoeffs, vector<Mat>& rvecs, vector<Mat>& tvecs, double& dRms);

	bool SaveCornersData(const string& strPath, const string& strName, const Size& sizeChessboardCorners, const vector<vector<Point2f>>& vt2ptfChessboardCorners);

	// 畫棋盤影像
	// fWidth = 要列印的寬度(mm)
	// fHeight = 要列印的高度(mm)
	// fDPI = 列印的精度
	// nCount_X = 水平方向的方格數
	// nCount_Y = 垂直方向的方格數
	// matImage : 棋盤影像
	bool DarwCheckerboardImage(const float& fWidth, const float& fHeight, const float fDPI, const int& nCount_X, const int& nCount_Y, Mat& matImage);
#pragma endregion

	#pragma region AI
		// 讀取影像與txt檔, 將 OK 或 NG 寫入 txt
		// matLabel : 標記影像
		// v3bCancel : 取消ROI的顏色
		// v3bNG : NG ROI的顏色
		bool CheckLabelResult(const Mat& matLabel, const Vec3b& v3bCancel, const Vec3b& v3bNG, const string& strPath, const string& strName);

		// 在影像中手動加入NG的ROI框, 再寫入Txt檔
		// matImage : Label影像
		// v3bNG : NG顏色
		// strPath, strName : 輸出的Txt, 將Label的資訊加入這個檔案再存檔
		bool AddLabelToTxt(const Mat& matImage, const Vec3b& v3bNG, const string& strPath, const string& strName);

		// 將 Label的 txt檔 畫在影像上
		bool ShowLabelROI(const string& strPath, const string& strName, Mat& matImage);

		// 輸入相關參數取得AI檔名
		bool GetAI_FileName(const string& strYear, const string& strMonth, const string& strDay, const string& strHour, const string& strMin, const string& strSec,
			const string& strPanelID, const string& strBoardID, const string& strComponentName, const string& strWindowID, string& strAIFileName);

		// 傳送檔案給AI
		// nFormat : 1=> json
		bool SaveAI_File(const Mat& matImage, const string& strFolderPath, const string& strFileName, const string& strVerJson, const string& strSender, const int& nAI_Model_ID, const int& nFormat);
		bool SaveAI_File_Json(const Mat& matImage, const string& strFolderPath, const string& strFileName, const string& strVerJson, const string& strSender, const int& nAI_Model_ID);

		// 儲存Pin點位
		// nFormat : 1=> json
		bool SaveAI_Result(const string& strFolderPath, const string& strFileName, const int& nFormat, vector<Rect>& vtPinROI);
		bool SaveAI_Result_Json(const string& strFolderPath, const string& strFileName, vector<Rect>& vtPinROI);

		// 讀取傳送給AI的檔案
		// nFormat : 1=> json
		bool ReadAI_File(const string& strFolderPath, const string& strFileName, const int& nFormat, const bool& bDelete, string& strVerJson, string& strSender, int& nAI_Model_ID, Rect& cvROI);
		bool ReadAI_File_Json(const string& strFolderPath, const string& strFileName, const bool& bDelete, string& strVerJson, string& strSender, int& nAI_Model_ID, Rect& cvROI);

		// 讀取AI回傳的結果, 超過 nWaitTime_ms 時間還沒接收到檔案 直接 return false
		// nMaxWaitTime_ms : 設置等待AI回傳檔案的最大等待時間(ms)
		// dSumTime : Function的總執行時間
		// dWaitTime : 實際等待時間
		// nCount_Pin : Pin回傳的數量
		// nFormat : 1=> json
		bool ReadAI_Result(const string& strFolderPath, const string& strFileName, const int& nMaxWaitTime_ms, const int& nForma, const bool& bDelete, int& nCount_Pint, vector<Rect>& vtPinROI, double& dSumTime, double& dWaitTime);
		bool ReadAI_Result_Json(const string& strFolderPath, const string& strFileName, const int& nMaxWaitTime_ms, const bool& bDelete, int& nCount_Pin, vector<Rect>& vtPinROI, double& dSumTime, double& dWaitTime);
	#pragma endregion
	#endif // FOR_MACHINE == JET_MACHINE_7000

//#if FOR_MACHINE  != JET_MACHINE_8000
	#pragma region FileIO
		int OutputData(Mat& matSrc, const Rect& cvRoi, string& strOutputPathName);
		bool Read_TXT(const string& strPathName, vector<string>& vtstrData);
		bool Read_TXT(const string& strPathName, const char& chDelimiter, vector<vector<string>>& vtstrData);
		bool Read_CSV(const string& strPathName, vector<string>& vtstrData);
		bool Read_CSV(const string& strPathName, const char& chDelimiter, vector<vector<string>>& vtstrData);

		// 20230331 Jun+ 刪除資料內的檔案
		// strFolderPath : 資料夾路徑.
		bool DeleteFolderFile(const string& strFolderPath);
	#pragma endregion 
//#endif // FOR_MACHINE != JET_MACHINE_8000
	}
#endif // FOR_MACHINE



