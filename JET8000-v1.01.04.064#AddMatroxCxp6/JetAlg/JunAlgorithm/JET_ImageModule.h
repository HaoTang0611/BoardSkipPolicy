#pragma once
//========================= Image Process Mode Parameter =========================//
// Author : Jun
// Data : 2025/02/06
// Version : 2.0.1
// Describe : 提供給 "JET_ImageFunction.h" 中的 
//            bool RunProcessMode(const vector<Mat>& vtmatSrc, const SProcessModeParam& sPMParam, vector<SProcessModeOutput>& vtsResult); 以及 
//            bool RunProcessMode_Node(const vector<Mat>& vtmatSrc, SProcessModeParam& sPMParam, vector<SProcessModeOutput>& vtsResult);
//            的相關設定參數, 用來組成各式演算法流程
//================================================================================//

#include <windows.h>
#include <vector>
#include <algorithm>
#include <functional>  
#include <map>
#include <set>
#include <stdexcept>
#include <memory>
#include <string>

namespace JET 
{
	namespace mod 
	{
		using namespace std;

#if 0
		// 通用包裝器的定義
		template<typename Func, typename... Args>
		auto callWithExceptionHandling(Func func, Args... args) -> decltype(func(args...)) 
		{
			try 
			{
				return func(args...); // 嘗試調用函數
			}
			catch (const std::exception& e) 
			{
				// 處理標準異常
				std::cerr << "標準異常被捕捉: " << e.what() << std::endl;
			}
			catch (...) 
			{
				// 處理非標準異常
				std::cerr << "非標準異常被捕捉" << std::endl;
			}
			return decltype(func(args...))(); // 返回函數返回類型的默認值
		}
#endif

		// 色彩轉換模式
		enum EColorMode
		{
			// 不處理 
			COLOR_TO_NONE = 0,

			// 灰階轉彩色(BGR)
			GRAY_TO_BGR = 1,

			// 負片處理
			COLOR_TO_NEGATIVE = 2,

			// BRG to Gray, 用 (R+G+B)/3 轉灰階	 
			COLOR_TO_GRAY_AVERAGE = 3,

			// BRG to Gray, 用 R*0.299 + G*0.587 + B*0.114 轉灰階		 
			COLOR_TO_GRAY_COEFFICIENT = 4,

			// BRG to Gray, 用 min(R,G,B) 轉灰階
			COLOR_TO_GRAY_MINIMUM = 5,

			// BRG to Gray, 用 max(R,G,B) 轉灰階
			COLOR_TO_GRAY_MAXIMUM = 6,

			// BRG to Gray, 計算 R G B 影像各自的標準差, 選標準差最大的分量
			COLOR_TO_GRAY_STD = 7,

			// BRG To Gray, 單張彩色影像, 使用 PCA轉灰階 nID=[0,2]會輸出最大-第三大主成分的影像, nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值
			// 須設定sPca中的nType_Eigenvectors
			COLOR_TO_GRAY_PCA = 8,

			// BRG To Gray, 多張彩色影像, 使用 PCA轉灰階 nID=[0,2]會輸出最大-第三大主成分的影像, nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值
			// 須設定sPca中的nType_Eigenvectors
			COLOR_TO_GRAY_PCA_MULTIPLE = 9,

			// BRG To Gray, 使用 Mask + PCA 轉灰階 nID=[0,2]會輸出最大-第三大主成分的影像, nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值
			// 須設定sPca中的nType_Eigenvectors
			COLOR_TO_GRAY_MASKPCA = 10,

			// BRG To Gray, 使用 Kmean + PCA 轉灰階 nID=[0,2]會輸出最大-第三大主成分的影像, nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值
			// 須設定sPca中的nType_Eigenvectors 與 sKmean中的nGroup以及fScale
			COLOR_TO_GRAY_KMEANPCA = 11,

			// 單張彩色影像轉負片再使用 PCA轉灰階 nID=[0,2]會輸出最大-第三大主成分的影像, nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值	
			// 須設定sPca中的nType_Eigenvectors
			COLOR_TO_GRAY_NEGATIVE_PCA = 12,

			// 多張彩色影像轉負片再使用 PCA轉灰階 nID=[0,2]會輸出最大-第三大主成分的影像, nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值	
			// 須設定sPca中的nType_Eigenvectors
			COLOR_TO_GRAY_NEGATIVE_PCA_MULTIPLE = 13,

			// 彩色影像轉負片再使用 Mask + PCA轉灰階 nID=[0,2]會輸出最大-第三大主成分的影像, nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值
			// 須設定sPca中的nType_Eigenvectors
			COLOR_TO_GRAY_NEGATIVE_MASKPCA = 14,

			// 彩色影像轉負片再使用 Kmean + PCA轉灰階 nID=[0,2]會輸出最大-第三大主成分的影像, nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值	
			// 須設定sPca中的nType_Eigenvectors 與 sKmean中的nGroup以及fScale
			COLOR_TO_GRAY_NEGATIVE_KMEANPCA = 15,

			// BGR To Gray 用 對每個 Channel 計算 MinCrossEntropy 再選擇 Channel
			COLOR_TO_GRAY_MINCROSSENTROPY = 16,

			// BGR To BGR		 
			COLOR_TO_GRAY_BGR = 17,

			// BRG To XYZ	
			COLOR_TO_XYZ = 18,

			// BRG To HSV	
			COLOR_TO_HSV = 19,

			// BRG To LUV	
			COLOR_TO_LUV = 20,

			// BRG To HLS	
			COLOR_TO_HLS = 21,

			// BRG To LAB	
			COLOR_TO_LAB = 22,

			// BRG To YUV	
			COLOR_TO_YUV = 23,

			COLOR_FINAL = 24,
		};

		// 影像增強模式
		enum EEnhanceMode
		{
			// 不處理
			ENHANCE_NONE = 0,

			// 高斯模糊
			ENHANCE_GAUSSIANBLUR = 1,

			// Log轉換1		 
			ENHANCE_LOG = 2,

			// Log轉換2		 
			ENHANCE_LOG_STD = 3,

			// 指數轉換
			ENHANCE_EXP = 4,

			// 直方圖均化		 
			ENHANCE_EQUALIZE = 5,

			// 直方圖均化, 搭配遮罩		 
			ENHANCE_EQUALIZE_MASK = 6,

			// 拉伸到0-255
			ENHANCE_NORMALIZE = 7,

			// 拉伸到0-255, 搭配遮罩
			ENHANCE_NORMALIZE_MASK = 8,

			// 使用k-mean做對比強化
			ENHANCE_CONTRAST_KMEAN = 9,

			// 使用k-mean做對比強化, 搭配遮罩
			ENHANCE_CONTRAST_KMEAN_MASK = 10,

			// 使用DBScan做對比強化
			ENHANCE_CONTRAST_DBSCAN = 11,

			// 使用DBScan做對比強化, 搭配遮罩
			ENHANCE_CONTRAST_DBSCAN_MASK = 12,

			// 邊界強化		 
			ENHANCE_EDGE = 13,

			// 使用 Retinex
			ENHANCE_RETINEX = 14,

			// 使用 pca 進行均勻度校正
			ENHANCE_UNIFORMITY_PCA = 15,

			// 使用 CLAHE 進行亮度均衡
			ENHANCE_UNIFORMITY_CLAHE = 16,

			// 使用 非線性轉換
			ENHANCE_NONLINEAR = 17,

			// 使用 PCA 對原始影像進行去中心化
			ENHANCE_DECENTRALIZATION_PCA = 18,
			
			// 使用 Kmean + PCA 對原始影像進行去中心化
			ENHANCE_DECENTRALIZATION_KMEANPCA = 19,
			
			ENHANCE_FINAL = 20,
		};

		// 濾波的結構元素
		enum EElementShape
		{
			// 不處理
			ELEMENT_NONE = 0,

			// 矩形
			ELEMENT_RECT = 1,

			// 交叉
			ELEMENT_CROSS = 2,

			// 橢圓
			ELEMENT_ELLIPSE = 3,

			// 矩形---45度斜線
			ELEMENT_RECT_TILTED_45 = 4,
			
			// 矩形---135度斜線
			ELEMENT_RECT_TILTED_135= 5,

			ELEMENT_FINAL = 6,
		};

		// 影像濾波模式
		enum EFilterMode
		{
			// 不處理
			FILTER_NONE = 0,

			// 均值濾波
			FILTER_AVERAGE = 1,
			
			// 高斯平滑濾波
			FILTER_GAUSSIAN_BLUR = 2,

			// Gabor 濾波
			FILTER_GABOR = 3,

			// 中值濾波		 
			FILTER_MEDIAN = 4,

			// 最小值濾波		 
			FILTER_MIN = 5,

			// 最大值濾波		 
			FILTER_MAX = 6,

			// 先最小值再最大值		 
			FILTER_MINMAX = 7,

			// 先最大值再最小值	 
			FILTER_MAXMIN = 8,

			// 先6再7
			FILTER_MINMAXMAXMIN = 9,

			// 先7再6
			FILTER_MAXMINMINMAX = 10,

			// 梯度
			FILTER_GRADIENT = 11,

			// 邊界 = 原始影像 - 最小濾波影像
			FILTER_EDGE = 12,

			// Sobel 濾波
			FILTER_SOBEL = 13,

			// PCA 反投影
			FILTER_PCA = 14,

			FILTER_FINAL = 15,
		};

		// 二值化模式
		enum EThresholdMode
		{
			// 不處理
			THRESHOLD_NONE = 0,

			// 單閥值	 
			THRESHOLD_SINGLE = 1,

			// 雙閥值(單區間)	 
			THRESHOLD_DOUBLE = 2,

			// 自動閥值 平均值
			THRESHOLD_AVERAGE = 3,	

			// 自動閥值 Otsu		 
			THRESHOLD_OTSU = 4,

			// 自動閥值 Triangle		 
			THRESHOLD_TRIANGLE = 5,

			// 自動閥值 MaxEntropy	 
			THRESHOLD_MAXENTROPY = 6,

			// 自動閥值 MinCrossEntropy	 
			THRESHOLD_MINCROSSENTROPY = 7,

			// 自動閥值 RenyiEntropy	 
			THRESHOLD_RENYIENTROPY = 8,

			// 自動閥值 Kmean, 須設定 sKmean
			THRESHOLD_KMEAN = 9,

			// 自動閥值 DBSCAN, 須設定 sDbscan
			THRESHOLD_DBSCAN = 10,

			// 自動閥值 多重區間二值化---使用Kmean與DBSCAN自動計算二值化區間
			THRESHOLD_MULTIPLE = 11,

			// 自動閥值 Adaptive---Wellner
			THRESHOLD_ADAPTIVE_WELLNER = 12,

			// 使用 灰階共生矩陣 二值化
			THRESHOLD_GLCM = 13,

			// 彩色影像二值化---設定目標值
			THRESHOLD_COLOR_TARGET = 14,

			// HSV影像二值化---設定目標值
			THRESHOLD_HSV_TARGET = 15,

			THRESHOLD_FINAL = 16,
		};

		// 形態學模式 
		enum EMorphologMode
		{
			// 不處理
			MORPHOLOG_NONE = 0,

			// 侵蝕
			MORPHOLOG_EROSION = 1,

			// 膨脹
			MORPHOLOG_DILATE = 2,

			// Opening(先侵蝕再膨脹)	 
			MORPHOLOG_OPENING = 3,

			// Closing(先膨脹再侵蝕)		 
			MORPHOLOG_CLOSING = 4,

			// 先 Opening 再 Closing
			MORPHOLOG_OPENCLOSE = 5,

			// 先 Closing 再 Opening
			MORPHOLOG_CLOSEOPEN = 6,

			MORPHOLOG_FINAL = 7,
		};

		// 物件刪除模式
		enum EDeleteMode
		{
			// 不處理
			DELETE_NONE = 0,

			// 刪除長度大於設定值的物件
			DELETE_SIZE_MORETHAN = 1,

			// 刪除長度小於設定值的物件
			DELETE_SIZE_LESSTHAN = 2,

			// 刪除長度在設定值範圍之間的物件
			DELETE_SIZE_BETWEEN = 3,

			// 刪除長度小於設定值1 或 大於設定值2的物件
			DELETE_SIZE_LESSTHAN_MORETHAN = 4,

			// ROI中的物件數大於 N 即全刪除
			DELETE_DENSITY_MORETHAN = 5,

			// ROI中的物件數小於 N 即全刪除
			DELETE_DENSITY_LESSTHAN = 6,

			// ROI中的物件數在設定值範圍之間 即全刪除
			DELETE_DENSITY_BETWEEN = 7,

			// ROI中的物件數小於設定值1 或 大於設定值2 即全刪除
			DELETE_DENSITY_LESSTHAN_MORETHAN = 8,

			// 物件的距離差大於設定值, 即刪除
			DELETE_DISTANCE_MORETHAN = 9,

			// 物件的距離差小於設定值, 即刪除
			DELETE_DISTANCE_LESSTHAN = 10,

			// 物件的距離差在設定值範圍之間, 即刪除
			DELETE_DISTANCE_BETWEEN = 11,

			// 物件的距離差小於設定值1 或 大於設定值2, 即刪除
			DELETE_DISTANCE_LESSTHAN_MORETHAN = 12,

			// 保留物件的距離差大於設定值的輪廓
			KEEP_DISTANCE_MORETHAN = 13,

			// 保留物件的距離差小於設定值的輪廓
			KEEP_DISTANCE_LESSTHAN = 14,

			// 保留物件的距離差在設定值範圍之間的輪廓
			KEEP_DISTANCE_BETWEEN = 15,

			// 保留物件的距離差小於設定值1 或 大於設定值2的輪廓
			KEEP_DISTANCE_LESSTHAN_MORETHAN = 16,

			// 保留最大尺寸的 ROI
			KEEP_ROI_MAXSIZE = 17,

			// 保留最小尺寸的 ROI
			KEEP_ROI_MINSIZE = 18,

			// 保留最大面積的 ROI
			KEEP_ROI_MAXAREA = 19,

			// 保留最小面積的 ROI
			KEEP_ROI_MINAREA = 20,

			// 保留最靠近影像中心的 ROI
			KEEP_ROI_NEAR_IMAGECRNTER = 21,

			// 保留靠近影像邊界的 ROI
			KEEP_ROI_NEAR_BORDER = 22,

			// 保留遠離影像邊界的 ROI
			KEEP_ROI_AWAY_BORDER = 23,

			// 保留最靠近影像邊界的 ROI
			KEEP_ROI_NEAREST_BORDER = 24,
		
			// 保留 外接矩形大小最接近的 ROI
			KEEP_ROI_THE_SIMILAR_SIZE = 25,

			// 保留圓形物件
			KEEP_SHAPE_CIRCLE = 26,

			// 保留矩形物件
			KEEP_SHAPE_RECTANGLE = 27,

			DELETE_FINAL = 28,
		};

		// 影像計算模式
		enum ECalculatorMode
		{
			// 不處理
			CALCULATOR_NONE = 0,

			// 影像相加
			CALCULATOR_ADD = 1,

			// 影像相減
			CALCULATOR_SUBTRACT = 2,

			// 影像相減取絕對值
			CALCULATOR_ABS_SUBTRACT = 3,

			// 影像相乘
			CALCULATOR_MULTIPLY = 4,

			// 影像相除
			CALCULATOR_DIVIDE = 5,

			// 兩張影像同一pixels取最大值
			CALCULATOR_MAXIMUM = 6,

			// 兩張影像同一pixels取最小值
			CALCULATOR_MINIMUM = 7,

			// 兩張影像取平均
			CALCULATOR_AVERAGE = 8,

			// 影像縮放
			CALCULATOR_RESIZE = 9,

			// 影像 Copy 不改變影像 Size
			CALCULATOR_COPY = 10,

			// 兩張影像取交集
			CALCULATOR_AND = 11,

			// 兩張影像取聯集
			CALCULATOR_OR = 12,

			// 兩張影像取 互斥或
			CALCULATOR_XOR = 13,

			// 取出 ROI 範圍的影像, 會改變影像 Size
			CALCULATOR_EXTRACT_ROI = 14,

			// 影像填充, 指定顏色填入影像或 ROI(須設定ROI1 或 Mask1)
			CALCULATOR_FILL = 15,

			// 指定顏色填入影像外圍邊界, 須設定 ROI1的寬高(ROI1的高=上下邊界的距離, ROI1的寬=左右邊界的距離) 
			CALCULATOR_FILL_BOUNDARY = 16,

			// 取代指定的顏色 (須設定 nGrayValue 與 dValue 以及 eQModel)
			CALCULATOR_FILL_VALUE = 17,

			CALCULATOR_FINAL = 18,
		};

		// 輪廓轉換模式
		enum EContoursMode
		{
			// 不處理
			CONTOUR_NONE = 0,

			// 二值化影像取邊
			CONTOUR_THRESHOLD_TO_EDGE = 1,

			// 二值化影像取邊---細線化
			CONTOUR_THRESHOLD_TO_THINNING = 2,

			// 從邊界影像(二值化影像)取出每個物件的輪廓
			CONTOUR_SEARCH = 3,

			// 灰階搜尋---相鄰pixels的灰階值差小於設定值, 則歸為同一群
			CONTOUR_GRAY_SEARCH = 4,

			// 只保留灰階搜尋的結果的邊界
			CONTOUR_GRAYSEARCH_TO_EDGE = 5,

			// 計算每個物件的外接矩形
			CONTOUR_BOUNDING_RECTANGLE = 6,

			// Union 聯集---單一群體內的ROI
			CONTOUR_UNION_1 = 7,

			// Union 聯集---兩個群體的ROI
			CONTOUR_UNION_2 = 8,

			// Intersection 交集---單一群體內的ROI
			CONTOUR_INTERSECTION_1 = 9,

			// Intersection 交集---兩個群體的ROI
			CONTOUR_INTERSECTION_2 = 10,

			// 依設定選擇輪廓與ROI
			CONTOUR_SELECT = 11,

			// 計算輪廓線每一點的夾角角度
			CONTOUR_ANGLE = 12,

			// 計算輪廓紋路的方向(0-180度)
			CONTOUR_TEXTURE_DIRECTION = 13,

			// 計算輪廓線形成的面積
			CONTOUR_AREA = 14,

			// 輪廓線填充(用輪廓線製做不規則遮罩)
			CONTOUR_FILL = 15,

			// 輪廓線填充(使用洪水填充)
			CONTOUR_FLOOD_FILL = 16,

			// 輪廓線轉凸集合
			CONTOUR_CONVEX = 17,

			// 凸集合輪廓填充
			CONTOUR_CONVEX_FILL = 18,

			// 畫出輪廓點與ROI
			CONTOUR_DRAW = 19,

			// 將輪廓轉成 Mask 
			CONTOUR_CONVERT_MASK = 20,

			// 將2個輪廓結構合併成ㄧ個
			CONTOUR_MERGE = 21,

			// 改變輪廓外接矩形的大小
			CONTOUR_CHANGE_SIZE = 22,

			// 顯示輪廓跟矩形(彩色影像)
			CONTOUR_DISPLAY = 23,
	
			CONTOUR_FINAL = 24,
		};

		// 特徵模式
		enum EFeatureMode
		{
			// 不處理
			FEATURE_NONE = 0,

			// 灰階共生矩陣
			FEATURE_GLCM = 1,

			// 計算物件的平面旋轉角度
			FEATURE_ANGLE = 2,

			// 計算物件的鍊碼
			FEATURE_CHAIN_CODE = 3,

			// 特徵分析
			FEATURE_ANALYZE = 4,

			FEATURE_FINAL = 5,
		};

		// 判斷條件 
		enum EQualification
		{
			// 不處理
			QUALIFICATION_NONE = 0,

			// 大於
			QUALIFICATION_GREATER = 1,

			// 小於
			QUALIFICATION_LESS = 2,

			// 等於
			QUALIFICATION_EQUAL = 3,

			// 大於等於
			QUALIFICATION_GREATER_EQUAL = 4,

			// 小於等於
			QUALIFICATION_LESS_EQUAL = 5,

			// 邏輯運算符號 &&
			QUALIFICATION_AND = 6,

			// 邏輯運算符號 ||
			QUALIFICATION_OR = 7,

			// 邏輯運算符號 !
			QUALIFICATION_NOT = 8,

			QUALIFICATION_FINAL = 9,
		};

		// 圖型比對模式
		enum EPatternMatch
		{
			// 不處理
			PATTERN_NONE = 0,

			// 建立 pattern
			PATTERN_CREATE_PATTERN = 1,

			// Match
			PATTERN_MATCH = 2,

			PATTERN_FINAL = 3,
		};

		// 處理模式
		enum EProcessMode
		{
			PROCESS_NONE = 0,
			PROCESS_COLOR = 1,
			PROCESS_ENHANCE = 2,
			PROCESS_FILTER = 3,
			PROCESS_THRESHOLD = 4,
			PROCESS_MORPHOLOG = 5,
			PROCESS_CONTOUR = 6,
			PROCESS_DELETE = 7,
			PROCESS_CALCULATOR = 8,
			PROCESS_FEATURE = 9,
			PROCESS_QUALIFICATION = 10,
			PROCESS_FINAL = 11,
		};

		// PCA參數
		struct SPcaParameter
		{
			// 預設為 2
			// 資料的排列方式
			// 0 = Row , 以每一個 Row 為 一個樣本
			// 1 = Col
			// 2 = Channel , 以每一個 Channel 為 一個樣本
			int nDataPermutation;

			// 預設為 1
			// PCA的樣本排列方式
			// 0 = Row , 以每一個 Row 為 一個樣本
			// 1 = Col
			int nSampleOrientation;

			// 預設為 1
			// 主成分數量設定方式
			// 1 = 直接設定數量 (nMaxComponents)
			// 2 = 設定方差百分比 (dRetainedVariance)
			int nSetComponentsType;

			// 預設為 1
			// 輸出的主成分數量, 0代表全部都輸出
			int nMaxComponents;

			// 預設為 0.5
			// 輸出主成分的百分比
			double dRetainedVariance;

			// 預設為 1
			// PCA 結果類型
			// 1 = 投影(降維)
			// 2 = 反投影(原始數據維度)
			int nPcaResultType;

			// 預設為 0
			// 設定特徵向量模式, 0 => 不使用
			//                   1 => 主成分特徵向量必須為正
			//                   2 => 主成分特徵向量必須為負
			int	nType_Eigenvectors;

			SPcaParameter() :nDataPermutation(2), nSampleOrientation(1), nSetComponentsType(1),
				nMaxComponents(1), dRetainedVariance(0.5), nPcaResultType(1), nType_Eigenvectors(0) {}

			bool Check() const
			{
				if (nDataPermutation < 0 || nDataPermutation>2 || nSampleOrientation < 0 || nSampleOrientation>1 || nSetComponentsType < 1 || nSetComponentsType>2 ||
					nMaxComponents<0 || dRetainedVariance <= 0 || dRetainedVariance>1.0 || nPcaResultType < 1 || nPcaResultType>2 || nType_Eigenvectors < 0 || nType_Eigenvectors>2)
				{
					return false;
				}
				return true;
			}
		};

		// 極限參數
		struct SLimitModeParameter
		{
		public:
			// 預設為 false;
			// 極限功能, true 為啟用
			bool bEnable;

			// 左值的極限類型
			// bEnable 為 true 時 才有用, 預設為 0
			// 0 => 不啟用
			// 1 => 限制最小值
			// 2 => 限制最大值
			// 3=> 最大最小值都限制
			int nLeft_Type;

			// 左值下限, 預設為 0
			int nLeft_Min;

			// 左值上限, 預設為 0
			int nLeft_Max;

			// 右值的極限類型
			// bEnable 為 true 時 才有用, 預設為 0
			// 0 => 不啟用
			// 1 => 限制最小值
			// 2 => 限制最大值
			// 3=> 最大最小值都限制
			int nRight_Type;

			// 右值下限, 預設為 0
			int nRight_Min;

			// 右值上限, 預設為 0
			int nRight_Max;

			SLimitModeParameter() : bEnable(false), nLeft_Type(0), nLeft_Min(0), nLeft_Max(0), nRight_Type(0), nRight_Min(0), nRight_Max(0) {}
			~SLimitModeParameter() = default;

			bool Check() const
			{
				if (bEnable && (nLeft_Type < 0 || nLeft_Type > 3 || nRight_Type < 0 || nRight_Type > 3 || (nLeft_Type == 1 && (nLeft_Min<0 || nLeft_Min>255)) ||
					(nLeft_Type == 2 && (nLeft_Max<0 || nLeft_Max>255)) || (nLeft_Type == 3 && (nLeft_Min > 255 || nLeft_Max < 0 || nLeft_Min >= nLeft_Max)) ||
					(nRight_Type == 1 && (nRight_Min<0 || nRight_Min>255)) || (nRight_Type == 2 && (nRight_Max<0 || nRight_Max>255)) ||
					(nRight_Type == 3 && (nRight_Min > 255 || nRight_Max < 0 || nRight_Min >= nRight_Max))))
				{
					return false;
				}
				return true;
			}
		};

		// 分群結果輸出參數結構
		struct SClusterOutputParameter
		{
			// 預設為 0, 
			// nOutputType值為1時, 為最小值的Id, 值域[0 , nGroupNumber-1]
			// nOutputType值為2-3時, 為最小值的灰階(面積)百分比, 值域[0,100]
			// nOutputType值為4-5時, 為計算差值的間隔數, 值域[1 , nGroupNumber-1]
			int nMinId;

			// 預設為 1, 
			// nOutputType值為1時, 為最大值的Id, 值域[1 , nGroupNumber-1]
			// nOutputType值為2-3時, 為最大值的灰階(面積)百分比, 值域[1,100]
			// nOutputType值為4-5時, 且nOutputNumber為1時, 可設定輸出Id, 1=>輸出較小值, 2=>輸出較大值, 3=>輸出平均值
			int nMaxId;

			// 預設為 1
			// 輸出值的數量, 值域[1,2] 
			int nOutputNumber;

			// 預設為 1, 值域[1,2]
			// 當 nOutputNumber為1時, 設定要取代最大值還是最小值, 1=>最小值
			//                                                    2=>最大值
			int nOutputDirection;

			// 預設為 1, 值域[1,5]
			// 輸出模式 : 1=>依 nMinId 與 nMaxId 的設定輸出, 若nOutputNumber為1=>只輸出 nMinId
			//          : 2=>將nMinId 與 nMaxId轉成灰階百分比, 自動計算對應的id, 若nOutputNumber為1=>只計算 nMinId
			//          : 3=>將nMinId 與 nMaxId轉成面積百分比, 自動計算對應的id, 若nOutputNumber為1=>只計算 nMinId
			//          : 4=>自動計算最大平均值差的 id, nMinId為間隔數, nOutputNumber為1時 nMaxId可設定輸出值大小
			//          : 5=>自動計算最大面積差的 id, nMinId為間隔數, nOutputNumber為1時 nMaxId可設定輸出值大小
			int nOutputType;

			// 預設值為 0.0, (0代表不使用)
			// 相臨群組的平均值差, 小於 fGroupAbsDiff值 將合併為同一群
			float fGroupAbsDiff;

			SLimitModeParameter sLimit;

			SClusterOutputParameter() : nMinId(0), nMaxId(1), nOutputNumber(1), nOutputDirection(1), nOutputType(1), fGroupAbsDiff(0.0) {}

			// 合併后的 Check 函数
			bool Check(int nGroupNumber = -1) const
			{
				if (nMinId < 0 || nOutputNumber < 1 || nOutputNumber > 2 || nOutputType < 1 || nOutputType > 5 || 
					(nOutputNumber == 1 && (nOutputDirection < 1 || nOutputDirection > 2)) ||
					(nOutputNumber == 2 && nOutputType >= 1 && nOutputType <= 3 && (nMinId >= nMaxId)) ||
					(nOutputType >= 2 && nOutputType <= 3 && nMinId > 100) ||
					(nOutputType >= 2 && nOutputType <= 3 && nOutputNumber == 2 && nMaxId > 100) ||
					(nGroupNumber != -1 && (nOutputType == 1 && nMinId >= nGroupNumber)) ||
					(nGroupNumber != -1 && (nOutputType == 1 && nOutputNumber == 2 && nMaxId >= nGroupNumber)) ||
					(nGroupNumber != -1 && (nOutputType >= 4 && nOutputType <= 5 && (nMinId < 1 || nMinId > nGroupNumber - 1))) ||
					!sLimit.Check())
				{
					return false;
				}
				return true;
			}
		};

		// Kmean參數
		struct SKmeanParameter
		{
			// 預設為 1.0
			// 縮小係數, 值域 [0.1 , 1.0]
			float fScale;

			// 預設為 1.0
			// 分群的收斂精度, 值域 (0.0 , 255.0)
			float fEps;

			// 預設為 10
			// 執行最大次數
			int nMaxTimes;

			// 預設為 2
			// 分群數, 不可小於 2
			int nGroupNumber;

			// 預設為 1
			// 分群方式, 1=>只用灰階值, 2=>使用灰階值與數量
			int nGroupMode;

			// 輸出參數
			SClusterOutputParameter sOutputParams; 

			// 分群結果
			vector<pair<float, float>> vtsResult; 

			SKmeanParameter() : fScale(1.0), fEps(1.0), nMaxTimes(10), nGroupNumber(2), nGroupMode(1) {}

			bool Check() const
			{
				if (fScale < 0.1 || fScale > 1.0 || nGroupNumber < 2 || nGroupMode < 1 || nGroupMode > 2 ||
					fEps <= 0.0 || fEps >= 255.0 || nMaxTimes < 1 ||
					!sOutputParams.Check(nGroupNumber))
				{
					return false;
				}
				return true;
			}
		};

		// DBSCAN參數
		struct SDbscanParameter
		{
			// 預設為 false(不使用)
			// 是否使用噪點
			bool bUseNoise;

			// 預設為 1
			// 分群方式, 1=>只用灰階值, 2=>使用灰階值與數量
			int nGroupMode;

			// 預設為 1
			// 群的最低數量(小於此設定值會被當作雜訊)
			int nMinQuantity;

			// 預設為 1.0
			// 縮小係數, 值域 0.1 - 1.0
			float fScale;

			// 預設為 1.0
			// 分群的最小距離
			double dMinDist;

			// 分群結果輸出參數結構
			SClusterOutputParameter sOutputParams;

			// vtsResult.first = 平均值
			// vtsResult.second = 數量
			vector<pair<float, float>> vtsResult;

			SDbscanParameter() : bUseNoise(false), nGroupMode(1), nMinQuantity(1), fScale(1.0), dMinDist(1.0) {}

			bool Check() const
			{
				if (fScale < 0.1 || fScale > 1.0 || nGroupMode < 1 || nGroupMode > 2 || !sOutputParams.Check())
				{
					return false;
				}
				return true;
			}
		};

		// 指定影像來源
		// (nType == 0 && nId == -1) 代表使用上一個處理的結果影像
		struct SInputType
		{
		protected :
			// 是否使用
			bool bOn;

			// 來源是否是多個
			bool bMultiple;

			// 影像來源
			// nType = 0 ; 不使用
			//       = 1 ; Function輸入
			//       = 2 ; 處理結果
			int nType;

			// 影像 id 從1開始
			int nId;

			// pair<int, POINT> first=目的id, second=>POINT.x=來源Type, POINT.y=來源id
			int nCount_Mult;
			vector<pair<int, POINT>> vtpaMultId;

		private:
			mutable string strErrorMessage;

		public:
			SInputType() : bOn(false), bMultiple(false), nType(0), nId(-1), nCount_Mult(0), strErrorMessage("") {}

			SInputType(const int ntype, const int nid, const bool bturnon)
				: bOn(bturnon), bMultiple(false)
			{
				if (ntype < 0 || ntype > 2)
				{
					throw std::invalid_argument("nType值超出範圍 (必須為 0, 1, 或 2)");
				}
				if ((ntype == 1 || ntype == 2) && nid < 1)
				{
					throw std::invalid_argument("nId值錯誤 (當 nType 為 1 或 2 時, nId 必須 ≥ 1)");
				}
				nType = ntype;
				nId = nid;
			}

			// ntype = 1 ; Function輸入
			//       = 2 ; 處理結果
			// nid = 從1開始
			// multiple = 設定來源是否是多個
			// bturnon = 設定是否使用
			SInputType(const int ntype, const int nid, const bool multiple, const bool bturnon) : nType(ntype), nId(nid), bMultiple(multiple), bOn(bturnon) {}

			void Initial()
			{
				bOn = false;
				bMultiple = false;
				nType = 0;
				nId = -1;
				strErrorMessage.clear();
			}

			// 設定使否啟用
			void SetState(const bool& bON) {bOn = bON;}

			// 設定是否使用多個輸入
			void SetMultipleEnable(const bool& bMult) { bMultiple = bMult; }

			// 讀取啟用狀態
			bool GetState() const { return bOn; }

			bool GetMultiple() const { return bMultiple; }
			int GetType() const { return nType; }
			int GetId() const { return nId; }
			string GetErrorMessage() const {return strErrorMessage;}

			// 設定多個數入的數量
			bool SetMultipleQuantity(const int nQ) {
				if (nQ < 1) {
					return false;
				}
				nCount_Mult = nQ;
				vtpaMultId.resize(nCount_Mult);
				return true;
			}

			bool SetType(const int& ntype)
			{
				if (ntype < 0 || ntype > 2) return false;
				nType = ntype;
				return true;
			}

			bool SetId(const int& nid)
			{
				if (nid < 0) return false;
				nId = nid;
				return true;
			}

			bool Check() const
			{
				if (bOn)
				{
					if (nType == 0 && nId != -1)
					{
						strErrorMessage = "Type為0,且Id不為-1";
						return false;
					}
					else if (nType < 0 || nType > 2)
					{
						strErrorMessage = "Type值超出範圍";
						return false;
					}
					else if ((nType == 1 || nType == 2) && nId < 1)
					{
						strErrorMessage = "Id值超出範圍";
						return false;
					}
				}
				return true;
			}

			// nCount_Image = 輸入影像的張數
			// nId_Queue = 執行順序 , 從1開始
			bool Check(const int nCount_Image, const int nId_Queue) const
			{
				if (bOn)
				{
					if (nCount_Image < 1)
					{
						strErrorMessage = "輸入影像張數錯誤";
						return false;
					}
					if (nId_Queue < 1)
					{
						strErrorMessage = "執行順序設定錯誤";
						return false;
					}
					if (!Check())
					{
						return false;
					}

					switch (nType)
					{
					case 0:
						if (nId == -1 && nId_Queue < 2)
						{
							strErrorMessage = "Id或執行順序設定錯誤";
							return false;
						}
						break;
					case 1:
						if (nId > nCount_Image)
						{
							strErrorMessage = "Id大於來源影像數量";
							return false;
						}
						break;
					case 2:
						if (nId >= nId_Queue)
						{
							strErrorMessage = "Id大於執行順序";
							return false;
						}
						break;
					default:
						strErrorMessage = "未知的 Type 值";
						return false;
					}
				}
				return true;
			}
		};

		struct SJRect
		{
		public:
			// 是否使用
			bool bOn;

			int nX;		//左上角 X 座標
			int nY;		//左上角 Y 座標
			int nWidth;	//寬度
			int nHeight;// 高度

		private:
			mutable string strErrorMessage;

		public:
			SJRect(int x=0, int y=0, int width=0, int height=0, bool bon = false) : nX(x), nY(y), nWidth(width), nHeight(height), bOn(bon) {}
			inline int GetR() const { return (nX + nWidth - 1); }
			inline int GetB() const { return (nY + nHeight - 1); }
			const int GetArea() const { return (nWidth*nHeight); }
			const POINT GetLeftTop() const 
			{ 
				POINT ptLT;
				ptLT.x = nX;
				ptLT.y = nY;
				return ptLT;
			}
			const POINT GetLeftBottom() const
			{
				POINT ptLB;
				ptLB.x = nX;
				ptLB.y = GetB();
				return ptLB;
			}
			const POINT GetRightBottom() const
			{
				POINT ptRB;
				ptRB.x = GetR();
				ptRB.y = GetB();
				return ptRB;
			}
			const POINT GetRightTop() const
			{
				POINT ptRT;
				ptRT.x = GetR();
				ptRT.y = nY;
				return ptRT;
			}
			const POINT GetCenter() const 
			{
				POINT ptCenter;
				ptCenter.x = nX + nWidth / 2;
				ptCenter.y = nY + nHeight / 2;
				return ptCenter;
			}

			// 判斷點是否在 SJRect 中
			bool IsPointInRect(const POINT& pt)
			{
				return (pt.x >= nX && pt.x <= GetR() && pt.y >= nY && pt.y <= GetB());
			}

			// 外擴或內縮矩形大小
			// nDistX, nDistY : >0=>外擴, <0=>內縮
			// nImageW, nImageH : 影像寬高
			bool ChangeSize(const int& nDistX, const int& nDistY, const int& nImageW, const int& nImageH)
			{
				//if (nImageW <= 0 || nImageH <= 0) return false;
				AdjustSize_Single(nDistX, nImageW, nX, nWidth);
				AdjustSize_Single(nDistY, nImageH, nY, nHeight);
				return true;
			}

			string GetErrorMessage() const { return strErrorMessage; }

			void Initial() 
			{ 
				bOn = false;
				nX = 0;		//左上角 X 座標
				nY = 0;		//左上角 Y 座標
				nWidth = 0;	//寬度
				nHeight = 0;// 高度
			}

			// 判斷啟用狀態
			bool GetState() const { return bOn; }

			bool Check() const
			{
				if (bOn)
				{
					if (nX < 0)
					{
						strErrorMessage = "X小於0";
						return false;
					}

					if (nY < 0)
					{
						strErrorMessage = "Y小於0";
						return false;
					}

					if (nWidth <= 0)
					{
						strErrorMessage = "Width小於等於0";
						return false;
					}

					if (nHeight < 0)
					{
						strErrorMessage = "Height小於等於0";
						return false;
					}
				}
				return true;
			}

		private:
			// nDelta : 改變量, >0代表外擴, <0代表內縮
			// nLimit : 影像邊界
			// nCoord : 左上點的X或Y座標
			// nSize : 矩形的寬或高
			void AdjustSize_Single(const int& nDelta, const int& nLimit, int& nCoord, int& nSize)
			{
				// 調整位置和尺寸
				nCoord -= nDelta;
				nSize += 2 * nDelta;

				// 確保坐標不小於0
				if (nCoord < 0)
				{
					nSize += nCoord; // 減去超出的部分
					nCoord = 0;
				}

				// 確保矩形不超過圖像範圍
				if (nLimit > 0)
				{
					if (nCoord + nSize > nLimit)
					{
						nSize = nLimit - nCoord; // 調整大小以適應範圍
					}
				}

				// 確保尺寸至少為1
				nSize = max(nSize, 1);
			}
		};

		struct BaseParameter
		{
		protected:

			// 影像處理模式
			EProcessMode eProcessMode;

			// 預設值為 1
			// 執行順序 , 從1開始
			int nId_Queue;

			//RollbackStep
			// 輸入來源
			SInputType sInput1;
			SInputType sInput2;
			SInputType sMask1;
			SInputType sMask2;
			SInputType sContours1;
			SInputType sContours2;
			SInputType sROI1_TypeId;
			SInputType sROI2_TypeId;

			// 影像 ROI
			SJRect sROI1;
			SJRect sROI2;

			mutable string strErrorMessage;

		public:
			BaseParameter() : sInput1(0, -1, true), eProcessMode(PROCESS_NONE), nId_Queue(1) {}

			virtual ~BaseParameter() = default;

			virtual std::unique_ptr<BaseParameter> Clone() const = 0; // 純虛擬函式

			void SetErrorMessage(string strMessage) {strErrorMessage = strMessage;}

			// 設定執行順序 , 從1開始
			// nQueueId = 執行的順序 , 
			void SetQueueId(const int nQueueId) { nId_Queue = nQueueId; }

			// 設定影像來源
			// nSrcType : 影像來源, 0 => 不使用
			//                    , 1 => Function輸入
			//                    , 2 => 處理結果
			// nIndex : 來源處的 Index, 起始值是 1
			// bMultiple : true=>使用多張影像
			//           : false=>只用一張影像
			bool SetInput1(const int nSrcType, const int nIndex, bool bMultiple = false)
			{
				SInputType sTemp(nSrcType, nIndex, bMultiple, true);
				if (!sTemp.Check())
				{
					strErrorMessage = "SetInput1_" + sTemp.GetErrorMessage();
					return false;
				}
				sInput1 = sTemp;
				return true;
			}

			// 輸入指令 psBase 的處理結果
			// bMultiple : true=>使用多張影像
			//           : false=>只用一張影像
			bool SetInput1(BaseParameter& psBase, bool bMultiple=false)
			{
				if (!psBase.Check())
				{
					strErrorMessage = "SetInput1_" + psBase.GetErrorMessage();
					return false;
				}
				SetInput1(2, psBase.GetQueueId(), bMultiple);
				return true;
			}

			// 不使用 Input1
			void Input1_TurnOff() { sInput1.SetState(false); }

			// 設定影像來源
			// nSrcType : 影像來源, 0 => 不使用
			//                    , 1 => Function輸入
			//                    , 2 => 處理結果
			// nIndex : 來源處的 Index, 起始值是 1
			// bMultiple : true=>使用多張影像
			//           : false=>只用一張影像
			bool SetInput2(const int nSrcType, const int nIndex, bool bMultiple = false)
			{
				SInputType sTemp(nSrcType, nIndex, bMultiple, true);
				if (!sTemp.Check())
				{
					strErrorMessage = "SetInput2_" + sTemp.GetErrorMessage();
					return false;
				}
				sInput2 = sTemp;
				return true;
			}

			bool SetInput2(BaseParameter& psBase, bool bMultiple = false)
			{
				if (!psBase.Check())
				{
					strErrorMessage = "SetInput2_" + psBase.GetErrorMessage();
					return false;
				}
				SetInput2(2, psBase.GetQueueId(), bMultiple);
				return true;
			}

			// 不使用 Input2
			void Input2_TurnOff() { sInput2.SetState(false); }

			// 設定影像來源
			// nSrcType : 影像來源, 0 => 不使用
			//                    , 1 => Function輸入
			//                    , 2 => 處理結果
			// nIndex : 來源處的 Index, 起始值是 1
			// bMultiple : true=>使用多張影像
			//           : false=>只用一張影像
			bool SetMask1(const int nSrcType, const int nIndex, bool bMultiple = false)
			{
				SInputType sTemp(nSrcType, nIndex, bMultiple, true);
				if (!sTemp.Check())
				{
					strErrorMessage = "SetMask1_" + sTemp.GetErrorMessage();
					return false;
				}
				sMask1 = sTemp;
				return true;
			}

			bool SetMask1(BaseParameter& psBase, bool bMultiple = false)
			{
				if (!psBase.Check())
				{
					strErrorMessage = "SetMask1_" + psBase.GetErrorMessage();
					return false;
				}
				SetMask1(2, psBase.GetQueueId(), bMultiple);
				return true;
			}

			// 不使用 Mask1
			void Mask1_TurnOff() { sMask1.SetState(false); }

			// 設定影像來源
			// nSrcType : 影像來源, 0 => 不使用
			//                    , 1 => Function輸入
			//                    , 2 => 處理結果
			// nIndex : 來源處的 Index, 起始值是 1
			// bMultiple : true=>使用多張影像
			//           : false=>只用一張影像
			bool SetMask2(const int nSrcType, const int nIndex, bool bMultiple = false)
			{
				SInputType sTemp(nSrcType, nIndex, bMultiple, true);
				if (!sTemp.Check())
				{
					strErrorMessage = "SetMask2_" + sTemp.GetErrorMessage();
					return false;
				}
				sMask2 = sTemp;
				return true;
			}

			bool SetMask2(BaseParameter& psBase, bool bMultiple = false)
			{
				if (!psBase.Check())
				{
					strErrorMessage = "SetMask2_" + psBase.GetErrorMessage();
					return false;
				}
				SetMask2(2, psBase.GetQueueId(), bMultiple);
				return true;
			}

			// 不使用 Mask2
			void Mask2_TurnOff() { sMask2.SetState(false); }

			// 設定 ROI
			// nX , nY = ROI 左上角
			// nWidth = ROI 寬
			// nHeight = ROI 高
			bool SetROI1(const int nX, const int nY, const int nWidth, const int nHeight)
			{
				SJRect sRect(nX, nY, nWidth, nHeight, true);
				if (!sRect.Check())
				{
					strErrorMessage = "SetROI1_" + sRect.GetErrorMessage();
					return false;
				}
				sROI1 = sRect;
				return true;
			}

			// 設定影像來源
			// nSrcType : 影像來源, 0 => 不使用
			//                    , 1 => Function輸入
			//                    , 2 => 處理結果
			// nIndex : 來源處的 Index, 起始值是 1
			// bMultiple : true=>使用多張影像
			//           : false=>只用一張影像
			bool SetROI1(const int nSrcType, const int nIndex, bool bMultiple = false)
			{
				SInputType sTemp(nSrcType, nIndex, bMultiple, true);
				if (!sTemp.Check())
				{
					strErrorMessage = "SetROI1_" + sTemp.GetErrorMessage();
					return false;
				}
				sROI1_TypeId = sTemp;
				return true;
			}

			bool SetROI1(BaseParameter& psBase, bool bMultiple = false)
			{
				if (!psBase.Check())
				{
					strErrorMessage = "SetROI1_" + psBase.GetErrorMessage();
					return false;
				}
				SetROI1(2, psBase.GetQueueId(), bMultiple);
				return true;
			}

			// 不使用 sROI1
			void ROI1_TurnOff() { sROI1_TypeId.SetState(false); }

			// 設定 ROI
			// nX , nY = ROI 左上角
			// nWidth = ROI 寬
			// nHeight = ROI 高
			bool SetROI2(const int nX, const int nY, const int nWidth, const int nHeight)
			{
				SJRect sRect(nX, nY, nWidth, nHeight, true);
				if (!sRect.Check())
				{
					strErrorMessage = "SetROI2_" + sRect.GetErrorMessage();
					return false;
				}
				sROI2 = sRect;
				return true;
			}

			// 設定影像來源
			// nSrcType : 影像來源, 0 => 不使用
			//                    , 1 => Function輸入
			//                    , 2 => 處理結果
			// nIndex : 來源處的 Index, 起始值是 1
			// bMultiple : true=>使用多張影像
			//           : false=>只用一張影像
			bool SetROI2(const int nSrcType, const int nIndex, bool bMultiple = false)
			{
				SInputType sTemp(nSrcType, nIndex, bMultiple, true);
				if (!sTemp.Check())
				{
					strErrorMessage = "SetROI2_" + sTemp.GetErrorMessage();
					return false;
				}
				sROI2_TypeId = sTemp;
				return true;
			}

			bool SetROI2(BaseParameter& psBase, bool bMultiple = false)
			{
				if (!psBase.Check())
				{
					strErrorMessage = "SetROI2_" + psBase.GetErrorMessage();
					return false;
				}
				SetROI2(2, psBase.GetQueueId(), bMultiple);
				return true;
			}

			// 不使用 sROI2
			void ROI2_TurnOff() { sROI2_TypeId.SetState(false); }

			// 設定影像來源
			// nSrcType : 影像來源, 0 => 不使用
			//                    , 1 => Function輸入
			//                    , 2 => 處理結果
			// nIndex : 來源處的 Index, 起始值是 1
			// bMultiple : true=>使用多張影像
			//           : false=>只用一張影像
			bool SetContours1(const int nSrcType, const int nIndex, bool bMultiple = false)
			{
				SInputType sTemp(nSrcType, nIndex, bMultiple, true);
				if (!sTemp.Check())
				{
					strErrorMessage = "SetContours1_" + sTemp.GetErrorMessage();
					return false;
				}
				sContours1 = sTemp;
				return true;
			}

			bool SetContours1(BaseParameter& psBase, bool bMultiple = false)
			{
				if (!psBase.Check())
				{
					strErrorMessage = "SetContours1_" + psBase.GetErrorMessage();
					return false;
				}
				SetContours1(2, psBase.GetQueueId(), bMultiple);
				return true;
			}

			// 不使用 sContours1
			void Contours1_TurnOff() { sContours1.SetState(false); }

			// 設定影像來源
			// nSrcType : 影像來源, 0 => 不使用
			//                    , 1 => Function輸入
			//                    , 2 => 處理結果
			// nIndex : 來源處的 Index, 起始值是 1
			// bMultiple : true=>使用多張影像
			//           : false=>只用一張影像
			bool SetContours2(const int nSrcType, const int nIndex, bool bMultiple=false)
			{
				SInputType sTemp(nSrcType, nIndex, bMultiple, true);
				if (!sTemp.Check())
				{
					strErrorMessage = "SetContours2_" + sTemp.GetErrorMessage();
					return false;
				}
				sContours2 = sTemp;
				return true;
			}

			bool SetContours2(BaseParameter& psBase, bool bMultiple = false)
			{
				if (!psBase.Check())
				{
					strErrorMessage = "SetContours2_" + psBase.GetErrorMessage();
					return false;
				}
				SetContours2(2, psBase.GetQueueId(), bMultiple);
				return true;
			}

			// 不使用 sContours2
			void Contours2_TurnOff() { sContours2.SetState(false); }

			EProcessMode GetProcessMode() const { return eProcessMode; }
			int GetQueueId() const { return nId_Queue; }
#if 1

			SInputType*  GetInput1() { return &sInput1; }
			SInputType*  GetInput2() { return &sInput2; }
			SInputType*  GetMask1() { return &sMask1; }
			SInputType*  GetMask2() { return &sMask2; }
			SInputType*  GetContours1() { return &sContours1; }
			SInputType*  GetContours2() { return &sContours2; }
			SInputType*  GetROI1_TypeId() { return &sROI1_TypeId; }
			SInputType*  GetROI2_TypeId() { return &sROI2_TypeId; }
			SJRect*  GetROI1() { return &sROI1; }
			SJRect*  GetROI2() { return &sROI2; }

			const SInputType*  GetInput1() const { return &sInput1; }
			const SInputType*  GetInput2() const { return &sInput2; }
			const SInputType*  GetMask1() const { return &sMask1; }
			const SInputType*  GetMask2() const { return &sMask2; }
			const SInputType*  GetContours1() const { return &sContours1; }
			const SInputType*  GetContours2() const { return &sContours2; }
			const SInputType*  GetROI1_TypeId() const { return &sROI1_TypeId; }
			const SInputType*  GetROI2_TypeId() const { return &sROI2_TypeId; }
			const SJRect*  GetROI1() const { return &sROI1; }
			const SJRect*  GetROI2() const { return &sROI2; }
#else
			SInputType  GetInput1() const { return sInput1; }
			SInputType  GetInput2() const { return sInput2; }
			SInputType  GetMask1() const { return sMask1; }
			SInputType  GetMask2() const { return sMask2; }
			SInputType  GetContours1() const { return sContours1; }
			SInputType  GetContours2() const { return sContours2; }
			SInputType  GetROI1_TypeId() const { return sROI1_TypeId; }
			SInputType  GetROI2_TypeId() const { return sROI2_TypeId; }
			SJRect  GetROI1() const { return sROI1; }
			SJRect  GetROI2() const { return sROI2; }
#endif
			string GetErrorMessage() const {return strErrorMessage;}

			bool Check() const
			{
				if (eProcessMode <= PROCESS_NONE || eProcessMode >= PROCESS_FINAL)
				{
					strErrorMessage = "nId_ProcessMode_OutOfRange";
					return false;
				}

				if (nId_Queue < 1)
				{
					strErrorMessage = "nId_Queue_OutOfRange";
					return false;
				}

				return true;
			}

			bool Check(const int nCount_Image, const int nCount_Queue) const
			{
				if (Check() == false)
				{
					return false;
				}

				if (sInput1.Check(nCount_Image, nId_Queue) == false)
				{
					strErrorMessage = "Input1_" + sInput1.GetErrorMessage();
					return false;
				}

				if (sInput2.Check(nCount_Image, nId_Queue) == false)
				{
					strErrorMessage = "Input2_" + sInput2.GetErrorMessage();
					return false;
				}

				if (sMask1.Check(nCount_Image, nId_Queue) == false)
				{
					strErrorMessage = "Mask1_" + sMask1.GetErrorMessage();
					return false;
				}

				if (sMask2.Check(nCount_Image, nId_Queue) == false)
				{
					strErrorMessage = "Mask2_" + sMask2.GetErrorMessage();
					return false;
				}

				if (sROI1.Check() == false)
				{
					strErrorMessage = "ROI1_" + sROI1.GetErrorMessage();
					return false;
				}

				if (sROI2.Check() == false)
				{
					strErrorMessage = "ROI2_" + sROI2.GetErrorMessage();
					return false;
				}

				if (sContours1.Check(nCount_Image, nId_Queue) == false)
				{
					strErrorMessage = "Contours1_" + sInput1.GetErrorMessage();
					return false;
				}

				if (sContours2.Check(nCount_Image, nId_Queue) == false)
				{
					strErrorMessage = "Contours2_" + sInput2.GetErrorMessage();
					return false;
				}

				return true;
			}
		};

		// 色彩轉換參數
		struct SColorTransformParam :BaseParameter
		{
			// 預設值為 0
			// 決定轉換後輸出的分量
			// eMode 為 1~7 時沒有作用
			// eMode 為 8~15 時會依據設定輸出
			// 例: eMode=COLOR_TO_HSV, 若nID為 0, 則最後輸出 H 分量
			//                         若nID為 1, 則最後輸出 S 分量
			//                         若nID為 2, 則最後輸出 V 分量
			//                         若nID為 3, 則最後輸出完整 HSV影像(3 Channel)
			int nID;

			// PCA 相關參數
			SPcaParameter	sPca;

			// Kmean 相關參數
			SKmeanParameter	sKmean;

			// 預設值為 0
			// 色彩轉換模式 : COLOR_TO_NONE = 0, 不處理 
			//              : GRAY_TO_BGR = 1, 灰階轉彩色(BGR)
			//              : COLOR_TO_NEGATIVE = 2, 負片處理
			//              : COLOR_TO_GRAY_AVERAGE = 3, BRG to Gray, 用 (R+G+B)/3 轉灰階	 
			//              : COLOR_TO_GRAY_COEFFICIENT = 4, BRG to Gray, 用 R*0.299 + G*0.587 + B*0.114 轉灰階		 
			//              : COLOR_TO_GRAY_MINIMUM = 5, BRG to Gray, 用 min(R,G,B) 轉灰階
			//              : COLOR_TO_GRAY_MAXIMUM = 6, BRG to Gray, 用 max(R,G,B) 轉灰階
			//              : COLOR_TO_GRAY_STD = 7, BRG to Gray, 計算 R G B 影像各自的標準差, 選標準差最大的分量
			//              : COLOR_TO_GRAY_PCA = 8, BRG To Gray, 單張彩色影像, 使用 PCA轉灰階	nID=[0,3]會輸出最大-第三大主成分的影像, nID=4會取三個主成分在同一pixels的最小值, nID=5會取三個主成分在同一pixels的最大值, 須設定sPca中的nType_Eigenvectors
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
			EColorMode	eMode;
			string      strInfo;
			SColorTransformParam() : nID(0), eMode(COLOR_TO_NONE), strInfo("") {
				eProcessMode = PROCESS_COLOR;
			}

			~SColorTransformParam() = default;

			std::unique_ptr<BaseParameter> Clone() const override {
				return std::make_unique<SColorTransformParam>(*this); // 深拷貝
			}

			bool Check() const
			{
				if (!BaseParameter::Check())
				{
					return false;
				}

				if (eMode < COLOR_TO_NONE || eMode >= COLOR_FINAL)
				{
					strErrorMessage = "模式設定超出範圍";
					return false;
				}

				if (eMode >= COLOR_TO_GRAY_MINCROSSENTROPY && (nID < 0 || nID>3))
				{
					strErrorMessage = "輸出Id設定錯誤";
					return false;
				}
				else if (eMode == COLOR_TO_GRAY_PCA && ((nID < 0 || nID>5) || !sPca.Check()))
				{
					strErrorMessage = "PCA輸出Id或參數設定錯誤";
					return false;
				}
				else if (eMode == COLOR_TO_GRAY_MASKPCA && ((nID < 0 || nID>5) || !sPca.Check()))
				{
					strErrorMessage = "Mask PCA 輸出Id或參數設定錯誤";
					return false;
				}
				else if (eMode == COLOR_TO_GRAY_NEGATIVE_PCA && ((nID < 0 || nID>5) || !sPca.Check()))
				{
					strErrorMessage = "負片PCA輸出Id或參數設定錯誤";
					return false;
				}
				else if (eMode == COLOR_TO_GRAY_NEGATIVE_MASKPCA && ((nID < 0 || nID>5) || !sPca.Check()))
				{
					strErrorMessage = "負片 Mask PCA 輸出Id或參數設定錯誤";
					return false;
				}
				else if (eMode == COLOR_TO_GRAY_KMEANPCA && ((nID < 0 || nID>5) || !sPca.Check() || !sKmean.Check()))
				{
					strErrorMessage = "Kmean PCA 輸出Id或參數設定錯誤";
					return false;
				}
				else if (eMode == COLOR_TO_GRAY_NEGATIVE_KMEANPCA && ((nID < 0 || nID>5) || !sPca.Check() || !sKmean.Check()))
				{
					strErrorMessage = "負片Kmean PCA 輸出Id或參數設定錯誤";
					return false;
				}

				return true;
			}
		};

		// 影像強化參數
		struct SImageEnhanceParam :BaseParameter
		{
			// 預設值為 0
			// 影像增強系數(值越大效果越好)
			// 使用 ENHANCE_EDGE 與 ENHANCE_RETINEX 模式時 必須設定
			float	fEps;

			// 預設值為 0
			// 影像模糊系數(值越大效果越好)
			// 使用 ENHANCE_EDGE 與 ENHANCE_RETINEX 模式時 必須設定
			float	fSigma;

			// 預設值為 0
			// 亮度偏移值
			// 使用 ENHANCE_RETINEX 模式時 必須設定
			// ENHANCE_UNIFORMITY_PCA 模式時, 用來設定 nInterval
			int		nOffset;

			SKmeanParameter  sKmean;

			SDbscanParameter sDbscan;

			// 預設值為 0
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
			//              : ENHANCE_UNIFORMITY_PCA = 15, 使用 pca 進行均勻度校正 (nOffset=nBlockRows, fEps=nBlockCols, fSigma=fOverlapRatio)
			//              : ENHANCE_UNIFORMITY_CLAHE = 16, 使用 CLAHE 進行亮度均衡, (nOffset=nBlockW, fEps=nBlockH, fSigma=fClipLimit)
			//              : ENHANCE_NONLINEAR = 17, 使用非線性轉換 nOffset=模式, fEps = alpha
			//              : ENHANCE_DECENTRALIZATION_PCA = 18, 使用 PCA 對原始影像進行去中心化 (nOffset=nMode, fEps=fCoefficient, fSigma=nEigenvectorsType)
			//				: ENHANCE_DECENTRALIZATION_KMEANPCA = 19, 使用 Kmean + PCA 對原始影像進行去中心化 (nOffset=nMode, fEps=fCoefficient, fSigma=nEigenvectorsType)
			//              : ENHANCE_FINAL = 20,
			EEnhanceMode	eMode;

			SImageEnhanceParam() : fEps(0.0), fSigma(0.0), nOffset(0), eMode(ENHANCE_NONE) {
				eProcessMode = PROCESS_ENHANCE;
			}

			~SImageEnhanceParam() = default;

			std::unique_ptr<BaseParameter> Clone() const override {
				return std::make_unique<SImageEnhanceParam>(*this); // 深拷貝
			}

			bool Check() const
			{
				if (!BaseParameter::Check())
				{
					return false;
				}

				if(eMode < ENHANCE_NONE || eMode >= ENHANCE_FINAL)
				{
					strErrorMessage = "模式設定超出範圍";
					return false;
				}

				if (eMode == ENHANCE_NONLINEAR && (nOffset<1 || nOffset>6))
				{
					strErrorMessage = "";
					return false;
				}

				if ((eMode == ENHANCE_EDGE || eMode == ENHANCE_RETINEX) && fEps <= 0.0)
				{
					strErrorMessage = "Eps參數設定錯誤";
					return false;
				}

				if ((eMode == ENHANCE_EDGE || eMode == ENHANCE_RETINEX) && fSigma <= 0.0)
				{
					strErrorMessage = "Sigma參數設定錯誤";
					return false;
				}

				if (eMode == ENHANCE_CONTRAST_KMEAN && !sKmean.Check())
				{
					strErrorMessage = "Kmean參數設定錯誤";
					return false;
				}
				
				return true;
			}
		};

		// 影像濾波參數
		struct SFilterParam :BaseParameter
		{
			// 預設為 0
			// X方向濾波大小
			int nSizeX;

			// 預設為 0
			// Y方向濾波大小
			int nSizeY;

			// 預設為 1
			// 執行次數
			int nTimes;

			// 預設值為 0
			// 濾波的結構元素 : ELEMENT_NONE = 0, 不處理
			//                : ELEMENT_RECT = 1, 矩形
			//                : ELEMENT_CROSS = 2, 交叉
			//                : ELEMENT_ELLIPSE = 3, 橢圓	
			//                : ELEMENT_RECT_TILTED_45 = 4, 矩形---45度斜線(nSizeX=矩形大小, nSizeY=線寬)
			//                : ELEMENT_RECT_TILTED_135 = 5, 矩形---135度斜線(nSizeX=矩形大小, nSizeY=線寬)
			EElementShape   eElement;

			// 預設為 0
			// 灰階影像濾波模式 : FILTER_NONE = 0, 不處理, 
			//                  : FILTER_AVERAGE = 1, 均值濾波 (不需設定 EElementShape, 須輸入nSizeX 與 nSizeY)
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
			EFilterMode eMode;

			SFilterParam() :  nSizeX(0), nSizeY(0), nTimes(1), eElement(eElement), eMode(FILTER_NONE)  {
				eProcessMode = PROCESS_FILTER;
			}

			~SFilterParam() = default;

			std::unique_ptr<BaseParameter> Clone() const override {
				return std::make_unique<SFilterParam>(*this); // 深拷貝
			}

			bool Check() const
			{
				if (!BaseParameter::Check())
				{
					return false;
				}

				if (eMode < FILTER_NONE || eMode >= FILTER_FINAL)
				{
					strErrorMessage = "模式設定超出範圍";
					return false;
				}

				if (eMode == FILTER_AVERAGE && (nSizeX <= 0 || nSizeY <= 0))
				{
					strErrorMessage = "平均值濾波Size設定錯誤";
					return false;
				}

				if (eMode == FILTER_GAUSSIAN_BLUR && (nSizeX <= 0 || nSizeY <= 0))
				{
					strErrorMessage = "高斯平滑濾波Size設定錯誤";
					return false;
				}

				if (eMode == FILTER_GABOR && (nSizeX <= 0 || nSizeY < 1 || nSizeY>8))
				{
					strErrorMessage = "Gabor濾波的Radius或Model設定錯誤";
					return false;
				}

				if (eMode == FILTER_MEDIAN && nSizeX <= 0)
				{
					strErrorMessage = "中值濾波Size設定錯誤";
					return false;
				}

				if (eMode == FILTER_SOBEL && nSizeX <= 0)
				{
					strErrorMessage = "Sobel濾波Size設定錯誤";
					return false;
				}

				if ((eMode > FILTER_MEDIAN && eMode <= FILTER_EDGE) && (eElement <= ELEMENT_NONE || eElement >= ELEMENT_FINAL))
				{
					strErrorMessage = "元素結構未設定";
					return false;
				}

				if ((eMode > FILTER_MEDIAN && eMode <= FILTER_EDGE) && (nSizeX <= 0 || nSizeY <= 0))
				{
					strErrorMessage = "Size設定錯誤";
					return false;
				}

				if (eMode == FILTER_PCA && (nSizeX < 0 || nSizeX>1 || nSizeY < 1 || nSizeY>10))
				{
					strErrorMessage = "PCA濾波參數設定錯誤";
					return false;
				}

				if (nTimes <= 0)
				{
					strErrorMessage = "執行次數設定錯誤";
					return false;
				}

				return true;
			}
		};

		// 二值化參數
		struct SThresholdParam :BaseParameter
		{
			// 預設為 false
			// 二值化值域選擇, 
			bool bDark;

			// 預設為 0
			// 二值化低閥值, 
			// 使用 THRESHOLD_SINGLE 與 THRESHOLD_DOUBLE 時,必須設定
			// 使用 THRESHOLD_GLCM 為 X 方向
			int nThreshold_Low;

			// 預設為 0
			// 二值化高閥值, 
			// 使用 THRESHOLD_DOUBLE 時,必須設定
			// 使用 THRESHOLD_GLCM 為 Y 方向
			int nThreshold_High;

			// 預設為 1.0
			// 使用 THRESHOLD_RENYIENTROPY 時,必須設定
			// 使用 THRESHOLD_GLCM 為 灰階層數 方向
			// 使用 THRESHOLD_AVERAGE , THRESHOLD_OTSU , THRESHOLD_TRIANGLE , THRESHOLD_MAXENTROPY , THRESHOLD_MINCROSSENTROPY , THRESHOLD_RENYIENTROPY 時, 為係數比
			float fAlpha;

			// 預設為 0
			//   eMode>=THRESHOLD_OTSU && eMode<=THRESHOLD_RENYIENTROPY 時, 算出來的閥值
			// 使用 THRESHOLD_GLCM 為 距離差
			int   nAutoThreshold;

			vector<int> vtnLow;

			vector<int> vtnUpper;

			// 使用 kmean相關 二值化時才須設定
			SKmeanParameter		sKmean;

			// 使用 DBSCAN相關 二值化時才需設定
			SDbscanParameter	sDbscan;

			// 使用自動閥值時, 可設定範圍, 預設關閉
			SLimitModeParameter	sLimit;

			// 預設為 0
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
			//              bDark = true  : 值域 [0 , nAutoThreshold]
			//              bDark = false : 值域 [nAutoThreshold , (256/fAlpha)-1]
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
			EThresholdMode eMode;

			SThresholdParam() : bDark(false), nThreshold_Low(0), nThreshold_High(0), fAlpha(1.0), eMode(THRESHOLD_NONE), nAutoThreshold(0) {
				eProcessMode = PROCESS_THRESHOLD;
			}

			~SThresholdParam() = default;

			std::unique_ptr<BaseParameter> Clone() const override {
				return std::make_unique<SThresholdParam>(*this); // 深拷貝
			}

			bool Check() const
			{
				if (!BaseParameter::Check())
				{
					return false;
				}

				if (eMode < THRESHOLD_NONE || eMode >= THRESHOLD_FINAL)
				{
					strErrorMessage = "模式設定超出範圍";
					return false;
				}

				if (eMode== THRESHOLD_SINGLE && (nThreshold_Low<0 || nThreshold_Low>255))
				{
					strErrorMessage = "單閥值模式,閥值設定錯誤";
					return false;
				}

				if ((eMode == THRESHOLD_DOUBLE) && (nThreshold_Low > nThreshold_High || nThreshold_High > 255 || nThreshold_Low < 0))
				{
					strErrorMessage = "雙閥值模式,閥值設定錯誤";
					return false;
				}

				if (eMode == THRESHOLD_GLCM && (nThreshold_Low<0 || nThreshold_High<0 || nAutoThreshold<0))
				{
					strErrorMessage = "灰階共生矩陣模式,參數設定錯誤";
					return false;
				}

				if (((eMode == THRESHOLD_AVERAGE || eMode == THRESHOLD_OTSU || eMode == THRESHOLD_TRIANGLE || eMode == THRESHOLD_MAXENTROPY ||
						eMode == THRESHOLD_MINCROSSENTROPY)) && fAlpha <= 0.0)
				{
					strErrorMessage = "Alpha值設定錯誤";
					return false;
				}

				if (((eMode == THRESHOLD_COLOR_TARGET) || (eMode == THRESHOLD_HSV_TARGET)) && nThreshold_Low < 0 || nThreshold_Low>255 || nThreshold_High < 0 || nThreshold_High>255 || nAutoThreshold < 0 || nAutoThreshold>255 ||
					fAlpha < 0 || fAlpha>255)
				{
					strErrorMessage = "彩色目標值二值化參數設定錯誤";
					return false;
				}

				return true;
			}
		};

		// 形態學參數
		struct SMorphologParam :BaseParameter
		{
			// 預設為 0
			// X方向濾波大小, 
			int nSizeX;

			// 預設為 0
			// Y方向濾波大小, 
			int nSizeY;

			// 預設值為 0
			// 濾波的結構元素 : ELEMENT_NONE = 0, 不處理
			//                : ELEMENT_RECT = 1, 矩形
			//                : ELEMENT_CROSS = 2, 交叉
			//                : ELEMENT_ELLIPSE = 3, 橢圓
			//                : ELEMENT_RECT_TILTED_45 = 4, 矩形---45度斜線(nSizeX=矩形大小, nSizeY=線寬)
			//                : ELEMENT_RECT_TILTED_135 = 5, 矩形---135度斜線(nSizeX=矩形大小, nSizeY=線寬)
			EElementShape   eElement;

			// 預設為 0
			// 二值化影像形態學模式 : MORPHOLOG_NONE = 0, 不處理, 
			//                      : MORPHOLOG_EROSION = 1, 侵蝕, 
			//						: MORPHOLOG_DILATE = 2, 膨脹
			//						: MORPHOLOG_OPENING = 3, Opening(先侵蝕再膨脹)
			//					    : MORPHOLOG_CLOSING = 4, Closing(先膨脹再侵蝕)
			//						: MORPHOLOG_OPENCLOSE = 5, 先 Opening 再 Closing
			//						: MORPHOLOG_CLOSEOPEN = 6, 先 Closing 再 Opening
			EMorphologMode eMode;

			SMorphologParam() : nSizeX(0), nSizeY(0), eElement(ELEMENT_NONE), eMode(MORPHOLOG_NONE) {
				eProcessMode = PROCESS_MORPHOLOG;
			}

			~SMorphologParam() = default;

			std::unique_ptr<BaseParameter> Clone() const override {
				return std::make_unique<SMorphologParam>(*this); // 深拷貝
			}

			bool Check() const
			{
				if (!BaseParameter::Check())
				{
					return false;
				}

				if (eMode < MORPHOLOG_NONE || eMode >= MORPHOLOG_FINAL )
				{
					strErrorMessage = "模式設定超出範圍";
					return false;
				}

				if (eMode > MORPHOLOG_NONE && (eElement <= ELEMENT_NONE || eElement >= ELEMENT_FINAL))
				{
					strErrorMessage = "元素結構未設定";
					return false;
				}

				if ((eMode > MORPHOLOG_NONE && (nSizeX < 0 || nSizeY < 0)))
				{
					strErrorMessage = "Size設定錯誤";
					return false;
				}

				return true;
			}
		};

#pragma region Contours
		struct SContours_Base 
		{
			bool bOn;

			// 線段數(一個輪廓由nCount條線組成)
			int nCount;

			// 輪廓的週長
			int nPerimeter;

			// 線段形成的面積
			int nArea;

			//輪廓的外接矩形
			SJRect sjROI;

			// 輪廓線段
			vector<vector<POINT>> vt2ptContours;

			// 輪廓夾角
			vector<vector<vector<float>>> vt3fAngle;

			// 以sjROI為範圍, 將輪廓形成遮罩
			vector<BYTE> vtu8Mask;

			SContours_Base() : bOn(true), nCount(0), nPerimeter(0), nArea(0), sjROI()
			{
			}

			virtual ~SContours_Base() = default;

			bool AddLineCount(const int& nAddCount)
			{
				if (nAddCount < 0) return false;
				if (nAddCount == 0) return true;
				nCount += nAddCount;
				vt2ptContours.resize(nCount);
				return true;
			}
		};

		struct SContours_Type1 : SContours_Base
		{
			using SContours_Base::SContours_Base;

			SContours_Type1() = default;
			~SContours_Type1() = default;

			bool AddLine(SContours_Type1& scLine)
			{
				int nSrcCount = nCount;
				if (scLine.nCount == 0) return true;

				if (!AddLineCount(scLine.nCount)) {
					return false;
				}

				for (int k = nSrcCount; k < nCount; ++k)
				{
					vt2ptContours[k].assign(scLine.vt2ptContours[k - nSrcCount].begin(), scLine.vt2ptContours[k - nSrcCount].end());
				}

				return true;
			}

			void Initial()
			{
				nCount = 0;
				nPerimeter = 0;
				nArea = 0;
				sjROI.Initial();
				vt2ptContours.clear();
				vt3fAngle.clear();
				vtu8Mask.clear();
			}
		};

		struct SContours_Type2 : SContours_Type1 
		{
			// 輪廓線段中的交叉點 
			vector<vector<POINT>> vt2ptCross;

			SContours_Type2() : SContours_Type1() {}
			~SContours_Type2() = default;

			bool AddLine(SContours_Type2& scLine)
			{
				int nSrcCount = nCount;
				if (scLine.nCount == 0) return true;

				if (!AddLineCount(scLine.nCount)) {
					return false;
				}

				vt2ptCross.resize(nCount);
				for (int k = nSrcCount; k < nCount; ++k)
				{
					vt2ptContours[k].assign(scLine.vt2ptContours[k - nSrcCount].begin(), scLine.vt2ptContours[k - nSrcCount].end());
					vt2ptCross[k].assign(scLine.vt2ptCross[k - nSrcCount].begin(), scLine.vt2ptCross[k - nSrcCount].end());
				}

				return true;
			}

			void Initial()
			{
				nCount = 0;
				nPerimeter = 0;
				nArea = 0;
				sjROI.Initial();
				vt2ptContours.clear();
				vt3fAngle.clear();
				vtu8Mask.clear();
				vt2ptCross.clear();
			}
		};

		// 輪廓參數
		struct SContoursParam
		{
		public:
			vector<SContours_Type1> vtsContours_Type1;
			vector<SContours_Type2> vtsContours_Type2;

		protected:
			// 預設為 0
			int nSearchType;
			int nImageHeight;
			int nImageWidth;

		public:
			SContoursParam() : nSearchType(0), nImageHeight(0), nImageWidth(0) {}

			~SContoursParam() = default;

			void Initial()
			{
				nSearchType = 0;
				nImageHeight = 0;
				nImageWidth = 0;
				vtsContours_Type1.clear();
				vtsContours_Type2.clear();
			}

			bool Contours_Shift(const int& nShiftX, const int& nShiftY)
			{
				int nCount = GetCount();
				if (nCount == 0 || GetCount_On() == 0) return true;
				for (int k = 0; k < nCount; ++k)
				{
					bool* pOn = GetOnPtr(k);
					if (pOn == nullptr) return false;
					if (*pOn)
					{
						SJRect* psRect = GetSJRectPtr(k);
						if (psRect == nullptr) return false;
						psRect->nX += nShiftX;
						psRect->nY += nShiftY;
						if (!psRect->ChangeSize(0, 0, nImageWidth, nImageHeight)) return false;
					}
				}
				return true;
			}

			// 增加輪廓
			bool Contours_Merge(SContoursParam& sAddContours)
			{
				if (sAddContours.Check() == false)
				{
					return false;
				}

				int nCount_Add = sAddContours.GetCount();
				if (nCount_Add == 0)
				{
					nSearchType = sAddContours.GetSearchType();
					nImageHeight = sAddContours.GetImageHeight();
					nImageWidth = sAddContours.GetImageWidth();
					return true;
				}

				int nSearchType_Add = sAddContours.GetSearchType();
				if (nSearchType_Add < 1 || nSearchType_Add > 3)
				{
					return false;
				}

				if (nSearchType == 0)
				{
					nSearchType = nSearchType_Add;
					nImageHeight = sAddContours.GetImageHeight();
					nImageWidth = sAddContours.GetImageWidth();
				}
				else
				{
					if (nSearchType != nSearchType_Add)
					{
						return false;
					}
				}

				int nOnCount = sAddContours.GetCount_On();
				int nCount_Type1 = 0, nCount_Type2 = 0, nSumCount = 0;
				int nId = 0;
				switch (nSearchType_Add)
				{
				case 1:
				case 3:
					nCount_Type1 = vtsContours_Type1.size();
					nSumCount = nCount_Type1 + nOnCount;
					vtsContours_Type1.resize(nSumCount);
					for (int k = 0; k < nCount_Add; ++k)
					{
						bool *pbOn = sAddContours.GetOnPtr(k);
						if (pbOn == nullptr)	return false;
						if (*pbOn)
						{
							vtsContours_Type1[nCount_Type1 + nId] = sAddContours.vtsContours_Type1[k];
							++nId;
						}
					}
					break;

				case 2:
					nCount_Type2 = vtsContours_Type2.size();
					nSumCount = nCount_Type2 + nOnCount;
					vtsContours_Type2.resize(nSumCount);
					for (int k = 0; k < nCount_Add; ++k)
					{
						bool *pbOn = sAddContours.GetOnPtr(k);
						if (pbOn == nullptr)	return false;
						if (*pbOn)
						{
							vtsContours_Type2[nCount_Type2 + nId] = sAddContours.vtsContours_Type2[k];
							++nId;
						}
					}
					break;
				}
				return true;
			}

			// 改變外接矩形的大小---只會對 On為true的ROI修改
			// nDistX : X方向(單邊)的外擴或內縮距離
			// nDistY : Y方向(單邊)的外擴或內縮距離
			bool ChangeROISize(const int& nDistX, const int& nDistY)
			{
				if (nSearchType <= 0 || nImageHeight <= 0 || nImageWidth <= 0) return false;
				int nCount = GetCount();
				if ((nDistX == 0 && nDistY == 0) || nCount == 0 || GetCount_On()==0) return true;

				for (int k = 0; k < nCount; ++k)
				{
					bool* pOn = GetOnPtr(k);
					if (pOn == nullptr) return false;
					if (*pOn)
					{
						SJRect* psRect = GetSJRectPtr(k);
						if (psRect == nullptr) return false;
						if (!psRect->ChangeSize(nDistX, nDistY, nImageWidth, nImageHeight)) return false;
					}
				}
				return true;
			}

			bool SetSearchType(const int nType) 
			{ 
				if (nType < 1 || nType > 3)
				{
					return false;
				}
				nSearchType = nType; 
				return true;
			}
			bool SetImageSize(const int& nImageH, const int& nImageW)
			{
				if (nImageH <= 0 || nImageW <= 0)
				{
					return false;
				}
				nImageHeight = nImageH;
				nImageWidth = nImageW;
				return true;
			}
			bool SetInfo(const SContoursParam& sContours)
			{
				if (!sContours.Check()) return false;

				nImageHeight = sContours.nImageHeight;
				nImageWidth = sContours.nImageWidth;
				nSearchType = sContours.nSearchType;
				return true;
			}

			// 建立輪廓的數量
			bool CreateContoursCount(const int& nCount)
			{
				if (!Check()) return false;
				switch (nSearchType)
				{
				case 1:
				case 3:
					vtsContours_Type1.resize(nCount);
					break;
				case 2:
					vtsContours_Type2.resize(nCount);
					break;
				default:
					return false;
				}
				return true;
			}

			bool SetOn(const int nId, const bool bOn)
			{
				if (nId >= GetCount()) return false;
				switch (nSearchType)
				{
				case 1:
				case 3:
					vtsContours_Type1[nId].bOn = bOn;
					break;
				case 2:
					vtsContours_Type2[nId].bOn = bOn;
					break;
				default:
					return false;
					break;
				}
				return true;
			}

			bool SetArea(const int& nId, int& nArea)
			{
				if (nId >= GetCount() || nArea < 0) return false;
				switch (nSearchType)
				{
				case 1:
				case 3:
					vtsContours_Type1[nId].nArea = nArea;
					break;
				case 2:
					vtsContours_Type2[nId].nArea = nArea;
					break;
				default:
					return false;
				}
				return true;
			}

			const int GetImageHeight() const { return nImageHeight; }
			const int GetImageWidth() const { return nImageWidth; }
			const int GetSearchType() const { return nSearchType; }
			const int GetCount() const
			{
				int nCount = -1;
				switch (nSearchType)
				{
				case 1:
				case 3:
					nCount = vtsContours_Type1.size();
					break;
				case 2:
					nCount = vtsContours_Type2.size();
					break;
				default:
					break;
				}
				return nCount;
			}

			vector<vector<POINT>>* GetLinePtr(const int& nId) 
			{
				if (nId >= GetCount()) return nullptr;
				switch (nSearchType)
				{
				case 1:
				case 3:
					return &vtsContours_Type1[nId].vt2ptContours;
				case 2:
					return &vtsContours_Type2[nId].vt2ptContours;
				default:
					return nullptr;
				}
			}
			vector<SContours_Type1>* GetType1Ptr() { return &vtsContours_Type1; }
			vector<SContours_Type2>* GetType2Ptr() { return &vtsContours_Type2; }

			const int GetLineCount(const int& nId) const
			{
				switch (nSearchType)
				{
				case 1:
				case 3:
					return vtsContours_Type1[nId].nCount;
				case 2:
					return vtsContours_Type2[nId].nCount;
				}
				return 0;
			}

			bool MoveLine(const int& nSrcId, const int& nDstId)
			{
				if (nSrcId < 0 || nSrcId >= GetCount()) return false;
				if (nDstId < 0 || nDstId >= GetCount()) return false;

				bool bResult = false;
				switch (nSearchType)
				{
				case 1:
				case 3:
					bResult = vtsContours_Type1[nDstId].AddLine(vtsContours_Type1[nSrcId]);	
					break;
				case 2:
					bResult = vtsContours_Type2[nDstId].AddLine(vtsContours_Type2[nSrcId]);
					break;
				default:
					return false;
					break;
				}

				return bResult;
			}

			vector<vector<vector<float>>>* GetAngle(const int& nId)
			{
				if (nId >= GetCount()) return nullptr;
				switch (nSearchType)
				{
				case 1:
				case 3:
					return &vtsContours_Type1[nId].vt3fAngle;
				case 2:
					return &vtsContours_Type2[nId].vt3fAngle;
				default:
					return nullptr;
				}
			}

			int GetPerimeter(const int& nId)
			{
				if (nId >= GetCount()) return -1;
				switch (nSearchType)
				{
				case 1:
				case 3:
					return vtsContours_Type1[nId].nPerimeter;
				case 2:
					return vtsContours_Type2[nId].nPerimeter;
				default:
					return -1;
				}
			}

			int GetArea(const int& nId)
			{
				if (nId >= GetCount()) return -1;
				switch (nSearchType)
				{
				case 1:
				case 3:
					return vtsContours_Type1[nId].nArea;
				case 2:
					return vtsContours_Type2[nId].nArea;
				default:
					return -1;
				}
			}

			bool* GetOnPtr(const int& nId) 
			{
				if (nId >= GetCount()) return nullptr;
				switch (nSearchType)
				{
				case 1:
				case 3:
					return &vtsContours_Type1[nId].bOn;
				case 2:
					return &vtsContours_Type2[nId].bOn;
				default:
					return nullptr;
				}
			}

			const int GetCount_On()
			{
				int nCount_On = 0;
				int nCount = GetCount();
				for (int k = 0; k < nCount; ++k)
				{
					bool* pbOn = GetOnPtr(k);
					if (pbOn == nullptr)
					{
						return false;
					}

					if (*pbOn)
					{
						++nCount_On;
					}
				}

				return nCount_On;
			}

			SJRect* GetSJRectPtr(const int& nId)  
			{
				if (nId >= GetCount()) return nullptr;
				switch (nSearchType)
				{
				case 1:
				case 3:
					return &vtsContours_Type1[nId].sjROI;
				case 2:
					return &vtsContours_Type2[nId].sjROI;
				default:
					return nullptr;
				}
			}

			BYTE* GetMaskBufferPtr(const int& nId)
			{
				if (nId >= GetCount()) return nullptr;
				switch (nSearchType)
				{
				case 1:
				case 3:
					return vtsContours_Type1[nId].vtu8Mask.data();
				case 2:
					return vtsContours_Type2[nId].vtu8Mask.data();
				default:
					return nullptr;
				}
			}

			bool CreateMaskBuffer(const int& nId)
			{
				if (nId >= GetCount()) return false;
				int nArea = 0;
				switch (nSearchType)
				{
				case 1:
				case 3:
					nArea = vtsContours_Type1[nId].sjROI.GetArea();
					break;
				case 2:
					nArea= vtsContours_Type2[nId].sjROI.GetArea();
					break;
				default:
					return false;
				}

				if (nArea == 0) return false;
				switch (nSearchType)
				{
				case 1:
				case 3:
					vtsContours_Type1[nId].vtu8Mask.resize(nArea, 0);
					break;
				case 2:
					vtsContours_Type2[nId].vtu8Mask.resize(nArea, 0);
					break;
				default:
					return false;
				}
				return true;
			}

			bool Check() const
			{
				if (nSearchType < 0 || nSearchType > 3 || nImageHeight < 0 || nImageWidth < 0)
				{
					return false;
				}

				if (GetCount() == -1)
				{
					return false;
				}

				return true;
			}
			void Release()
			{
				vtsContours_Type1.clear();
				vtsContours_Type2.clear();
			}
		};

		// 輪廓形狀參數
		struct SContoursShapeParam :BaseParameter
		{
		public:

			// 預設為 0
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
			// CONTOUR_FLOOD_FILL = 16,             輪廓線填充(使用洪水填充), 需設定SetMask1(), bToEdge為true,代表要完全填滿
			// CONTOUR_CONVEX = 17,                 輪廓線轉凸集合
			// CONTOUR_CONVEX_FILL = 18,            凸集合輪廓填充
			// CONTOUR_DRAW = 19,					畫出輪廓點與ROI
			// CONTOUR_CONVERT_MASK = 20,			將輪廓轉成 Mask,
			// CONTOUR_MERGE = 21,					將2個輪廓結構合併成ㄧ個
			// CONTOUR_CHANGE_SIZE = 22,            改變輪廓外接矩形的大小, 須定 nMinPerimeter(X的方向的改變量) 與 nMaxPerimeter(Y的方向的改變量)
			// CONTOUR_DISPLAY = 23,                顯示輪廓跟矩形(彩色影像), 設定SetInput1()決定背景影像, bToEdge決定是否畫輪廓線, bRectangle決定是否畫矩形, bBoundingToEdge決定是否顯示bLabel
			// CONTOUR_FINAL = 24,
			EContoursMode eMode;

			// 域設為 true;
			// 影像的邊界是否也是邊界影像
			// 目前應用在 CONTOUR_SELECT 模式
			bool bBoundingToEdge;

			// 預設為 false
			// 是否要將二值化影像轉成邊界影像
			bool bToEdge;

			// 預設為 false
			// 是否建立輪廓的外接矩形
			bool bRectangle;

			// 預設為 false
			// 是否計算輪廓的面積
			bool bArea;

			// 預設為 0
			// 輪廓搜尋的方式
			// 1 = vt2ptContours
			// 2 = vt2ptCross
			// 3 = 灰階, vt2ptContours
			int nSearchType;

			// 預設為 0
			// nSearchType為2時才有用
			// 1 = 搜尋到的邊界全部輸出(相鄰的vt2ptContours_Out會被記錄到同一個vt2nObjIndex_Out) ; 
			// 2 = 輸出週長最長的線 , 其餘線的分支都忽略(未完成)
			int nSortType;

			// 預設為 1
			// 輪廓選擇方式
			// 在CONTOUR_SEARCH模式且bToEdge為true時, 可用來選擇作用區域 1,2 => 都是對白色區域取邊; 3=>對黑色區域取邊
			// 1 => 依 On 狀態
			// 2 => On + 白色區域形成的輪廓
			// 3 => On + 黑色區域形成的輪廓	
			int nSelectType;

			// 預設為 0
			// 物件的最小週長
			int nMinPerimeter;

			// 預設為 INT_MAX
			// 物件的最大週長
			int nMaxPerimeter;

		protected:

			// 輸入輪廓參數
			int nId_Queue_In;
			SContoursParam* psContours_In;

			// 輪廓計算結果
			SContoursParam sContours_Out;

		public:
			SContoursShapeParam() : eMode(CONTOUR_NONE), bBoundingToEdge(true), bToEdge(false), bRectangle(false), bArea(false), nSearchType(0), nSortType(0), nSelectType(1),
				nMinPerimeter(0), nMaxPerimeter(INT_MAX), nId_Queue_In(0), psContours_In(nullptr)
			{
				eProcessMode = PROCESS_CONTOUR;
			}

			~SContoursShapeParam() {
				psContours_In = nullptr;
			}

			std::unique_ptr<BaseParameter> Clone() const override {
				return std::make_unique<SContoursShapeParam>(*this); // 深拷貝
			}

			bool SetInfo(const SContoursShapeParam* psContoursShape)
			{
				if (psContoursShape == nullptr) return false;
				bBoundingToEdge = psContoursShape->bBoundingToEdge;
				bToEdge = psContoursShape->bToEdge;
				bRectangle = psContoursShape->bRectangle;
				bArea = psContoursShape->bArea;
				//nMinPerimeter = psContoursShape->nMinPerimeter;
				//nMaxPerimeter = psContoursShape->nMaxPerimeter;
				nSearchType = psContoursShape->nSearchType;
				nSelectType = psContoursShape->nSelectType;
				nSortType = psContoursShape->nSortType;
				return true;
			}

			bool SetContoursParamId_In(const int nQueue)
			{
				if (nQueue < 1 || nQueue >= nId_Queue)
				{
					strErrorMessage = "nId_Queue_In 設定錯誤";
					return false;
				}
				nId_Queue_In = nQueue;

				return true;
			}
			bool SetContoursParam_In(vector<BaseParameter*>& vtpBPqueue)
			{
				int nCount = vtpBPqueue.size();
				if (nCount <= 0 || nId_Queue > nCount)
				{
					strErrorMessage = "指令數量異常";
					return false;
				}

				if (nId_Queue_In < 1 || nId_Queue_In >= nId_Queue)
				{
					strErrorMessage = "nId_Queue_In 未設定";
					return false;
				}

				int nId = nId_Queue_In - 1;
				if (vtpBPqueue[nId] == nullptr)
				{
					strErrorMessage = "nId_Queue_In 設定錯誤";
					return false;
				}
				
				if (vtpBPqueue[nId]->GetProcessMode() != PROCESS_CONTOUR)
				{
					strErrorMessage = "處理指令為空";
					return false;
				}

				SContoursShapeParam* psContours = (SContoursShapeParam*)vtpBPqueue[nId];
				if (vtpBPqueue[nId]->GetProcessMode() == PROCESS_CONTOUR)
				{		
					if (psContours->eMode == CONTOUR_BOUNDING_RECTANGLE)
					{
						psContours_In = psContours->GetContoursParam_In();
					}
					else
					{
						psContours_In = &psContours->GetContoursParam_Out();
					}
				}

				bRectangle = psContours->bRectangle;
				nSearchType = psContours->nSearchType;
				nSortType = psContours->nSortType;

				if (eMode != CONTOUR_ANGLE)
				{
					nMinPerimeter = psContours->nMinPerimeter;
					nMaxPerimeter = psContours->nMaxPerimeter;
				}

				return true;
			}

			SContoursParam* GetContoursParam_In() { return psContours_In; }
			SContoursParam& GetContoursParam_Out() { return sContours_Out; }

			bool Check() const
			{
				if (!BaseParameter::Check())
				{
					return false;
				}

				if (eMode < CONTOUR_NONE || eMode >= CONTOUR_FINAL)
				{
					strErrorMessage = "eMode設定超出範圍";
					return false;
				}

				if (eMode == CONTOUR_SEARCH)
				{
					if (nSearchType < 1 || nSearchType > 3)
					{
						strErrorMessage = "SearchType設定超出範圍";
						return false;
					}

					if (nSearchType==2 && (nSortType < 1 || nSortType > 2))
					{
						strErrorMessage = "SortType設定超出範圍";
						return false;
					}

					if (nSelectType < 1 || nSelectType > 3)
					{
						strErrorMessage = "SelectType設定超出範圍";
						return false;
					}	

					if (nMinPerimeter<0 || nMinPerimeter>= nMaxPerimeter)
					{
						strErrorMessage = "Perimeter設定超出範圍";
						return false;
					}
				}

				if (eMode == CONTOUR_BOUNDING_RECTANGLE || eMode == CONTOUR_UNION_1 ||
					eMode == CONTOUR_INTERSECTION_1 || eMode == CONTOUR_DRAW || eMode == CONTOUR_ANGLE)
				{
					//if (psContours_In->Check() == false)
					//{
					//	strErrorMessage = "psContours_In 值異常";
					//	return false;
					//}

					if (eMode == CONTOUR_ANGLE)
					{
						if (nMinPerimeter < 3 || nMinPerimeter > nMaxPerimeter)
						{
							strErrorMessage = "Perimeter設定超出範圍";
							return false;
						}
					}
				}

				//if (eMode == CONTOUR_AREA && bRectangle==false)
				//{
				//	strErrorMessage = "請先計算輪廓ROI";
				//	return false;
				//}

				return true;
			}
		};
#pragma endregion

		// 物件刪除參數(針對ROI的 寬高 面積 位置 密度 來判斷)
		struct SDeleteObjectParam :BaseParameter
		{
			// 預設為 false
			// 是否對二值化影像取邊
			bool bEdge;

			// 預設為 1
			// 結果輸出模式, 1=> 輸出輪廓影像
			//               2=> 輸出輪廓結構
			//               3=> 輸出ROI
			//               4=>輸出輪廓影像 & ROI
			int nOutputMode;

			// 預設為 0
			// 以物件寬度(X方向)設定刪除條件(pixels)
			int  nWidth_Min;
			int  nWidth_Max;

			// 預設為 0
			// 以物件長度(Y方向)設定刪除條件(pixels)
			int  nHeight_Min;
			int  nHeight_Max;

			string strInfo;

			// 預設為 DELETE_NONE
			// DELETE_NONE = 0 :                        不處理
			// DELETE_SIZE_MORETHAN = 1 :				刪除長度大於 nWidth_Min 的物件
			// DELETE_SIZE_LESSTHAN = 2 :				刪除長度小於 nWidth_Min 的物件
			// DELETE_SIZE_BETWEEN = 3 :				刪除長度在 [nWidth_Min , nWidth_Max] 之間的物件
			// DELETE_SIZE_LESSTHAN_MORETHAN = 4 :		刪除長度小於 nWidth_Min 或 大於 nWidth_Max 的物件
			// DELETE_DENSITY_MORETHAN = 5 :			ROI中的物件數大於 nWidth_Min 即全刪除 , 須用 SetROI1() 設定ROI大小	
			// DELETE_DENSITY_LESSTHAN = 6 :			ROI中的物件數小於 nWidth_Min 即全刪除 , 須用 SetROI1() 設定ROI大小	
			// DELETE_DENSITY_BETWEEN = 7 :				ROI中的物件數在 [nWidth_Min , nWidth_Max] 之間 即全刪除 , 須用 SetROI1() 設定ROI大小
			// DELETE_DENSITY_LESSTHAN_MORETHAN = 8 :	ROI中的物件數小於 nWidth_Min 或 大於 nWidth_Max 即全刪除 , 須用 SetROI1() 設定ROI大小
			// DELETE_DISTANCE_MORETHAN = 9 :			物件與目標影像物件的距離差大於 nWidth_Min, 即刪除 , 須用 SetContours2() 設定目標輪廓
			// DELETE_DISTANCE_LESSTHAN = 10 :			物件與目標影像物件距離差小於 nWidth_Min, 即刪除 , 須用 SetContours2() 設定目標輪廓
			// DELETE_DISTANCE_BETWEEN = 11 :			物件與目標影像物件距離差在 [nWidth_Min , nWidth_Max] 之間, 即刪除 , 須用 SetContours2() 設定目標輪廓
			// DELETE_DISTANCE_LESSTHAN_MORETHAN = 12 : 物件與目標影像物件距離差小於 nWidth_Min 或 大於 nWidth_Max, 即刪除 , 須用 SetContours2() 設定目標輪廓
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
			EDeleteMode eMode_Width;

			// 預設為 DELETE_NONE
			// DELETE_NONE = 0 :                        不處理
			// DELETE_SIZE_MORETHAN = 1 :				刪除長度大於 nHeight_Min 的物件
			// DELETE_SIZE_LESSTHAN = 2 :				刪除長度小於 nHeight_Min 的物件
			// DELETE_SIZE_BETWEEN = 3 :				刪除長度在 [nHeight_Min , nHeight_Max] 之間的物件
			// DELETE_SIZE_LESSTHAN_MORETHAN = 4 :		刪除長度小於 nHeight_Min 或 大於 nHeight_Max 的物件
			// DELETE_DENSITY_MORETHAN = 5 :			ROI中的物件數大於 nHeight_Min 即全刪除 , 須用 SetROI1() 設定ROI大小
			// DELETE_DENSITY_LESSTHAN = 6 :			ROI中的物件數小於 nHeight_Min 即全刪除 , 須用 SetROI1() 設定ROI大小
			// DELETE_DENSITY_BETWEEN = 7 :				ROI中的物件數在 [nHeight_Min , nHeight_Max] 之間 即全刪除 , 須用 SetROI1() 設定ROI大小
			// DELETE_DENSITY_LESSTHAN_MORETHAN = 8 :	ROI中的物件數小於 nHeight_Min 或 大於 nHeight_Max 即全刪除 , 須用 SetROI1() 設定ROI大小
			// DELETE_DISTANCE_MORETHAN = 9 :			物件與目標影像物件的距離差大於 nHeight_Min, 即刪除 , 須用 SetContours2() 設定目標輪廓
			// DELETE_DISTANCE_LESSTHAN = 10 :			物件與目標影像物件距離差小於 nHeight_Min, 即刪除 , 須用 SetContours2() 設定目標輪廓
			// DELETE_DISTANCE_BETWEEN = 11 :			物件與目標影像物件距離差在 [nHeight_Min , nHeight_Max] 之間, 即刪除 , 須用 SetContours2() 設定目標輪廓
			// DELETE_DISTANCE_LESSTHAN_MORETHAN = 12 : 物件與目標影像物件距離差小於 nHeight_Min 或 大於 nHeight_Max, 即刪除 , 須用 SetContours2() 設定目標輪廓
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
			EDeleteMode eMode_Height;

			SDeleteObjectParam() : bEdge(false), nOutputMode(1), eMode_Width(DELETE_NONE), nWidth_Min(0), nWidth_Max(0), eMode_Height(DELETE_NONE), nHeight_Min(0), nHeight_Max(0), strInfo("") {
				eProcessMode = PROCESS_DELETE;
			}

			~SDeleteObjectParam() = default;

			std::unique_ptr<BaseParameter> Clone() const override {
				return std::make_unique<SDeleteObjectParam>(*this); // 深拷貝
			}

			bool Check_Single(const EDeleteMode& eMode, const int& nMin, const int& nMax) const
			{
				if (eMode == DELETE_NONE)	return true;

				if (eMode < DELETE_NONE || eMode >= DELETE_FINAL)
				{
					strErrorMessage = "模式設定超出範圍";
					return false;
				}

				if (nOutputMode < 1 || nOutputMode > 4)
				{
					strErrorMessage = "輸出模式設定超出範圍";
					return false;
				}

				if ((eMode == DELETE_SIZE_MORETHAN || eMode == DELETE_SIZE_LESSTHAN || eMode == DELETE_DENSITY_MORETHAN || eMode == DELETE_DENSITY_LESSTHAN ||
					eMode == DELETE_DISTANCE_MORETHAN || eMode == DELETE_DISTANCE_LESSTHAN || eMode == KEEP_SHAPE_CIRCLE || eMode == KEEP_SHAPE_RECTANGLE) && nMin <= 0)
				{
					strErrorMessage = "值小於0";
					return false;
				}

				if (((eMode == DELETE_SIZE_BETWEEN || eMode == DELETE_SIZE_LESSTHAN_MORETHAN || eMode == DELETE_DENSITY_BETWEEN || eMode == DELETE_DENSITY_LESSTHAN_MORETHAN ||
					eMode == DELETE_DISTANCE_BETWEEN || eMode == DELETE_DISTANCE_LESSTHAN_MORETHAN || eMode == KEEP_DISTANCE_BETWEEN || eMode == KEEP_DISTANCE_LESSTHAN_MORETHAN) && 
					(nMin <= 0 || nMax <= nMin)))
				{
					strErrorMessage = "最小值大於等於最大值";
					return false;
				}

				if ((eMode == KEEP_ROI_NEAR_BORDER || eMode == KEEP_ROI_AWAY_BORDER) && (nMin < 1 || nMin > 5 || nMax <= 0))
				{
					strErrorMessage = "最小值大於等於最大值";
					return false;
				}

				return true;
			}

			bool Check() const
			{ 
				if (!BaseParameter::Check())
				{
					return false;
				}

				if (nOutputMode < 1 || nOutputMode > 4)
				{
					strErrorMessage = "nOutputMode設定錯誤";
					return false;
				}

				if (Check_Single(eMode_Width, nWidth_Min, nWidth_Max)==false)
				{
					strErrorMessage = ("WidthMode_" + strErrorMessage);
					return false;
				}

				if (Check_Single(eMode_Height, nHeight_Min, nHeight_Max) == false)
				{
					strErrorMessage = ("HeightMode_" + strErrorMessage);
					return false;
				}

				return true;
			}
		};

		// 影像計算模式
		struct SImageCalculatorParam :BaseParameter
		{
			// 預設為 0
			// 影像計算處理模式 : CALCULATOR_NONE = 0, 不處理
			//                  : CALCULATOR_ADD = 1, 影像相加
			//                  : CALCULATOR_SUBTRACT = 2, 影像相減
			//                  : CALCULATOR_ABS_SUBTRACT = 3, 影像相減取絕對值
			//                  : CALCULATOR_MULTIPLY = 4, 影像相乘
			//                  : CALCULATOR_DIVIDE = 5, 影像相除	
			//                  : CALCULATOR_MAXIMUM = 6, 兩張影像同一pixels取最大值 (bSingle 須為 false)
			//                  : CALCULATOR_MINIMUM = 7, 兩張影像同一pixels取最小值 (bSingle 須為 false)	
			//                  : CALCULATOR_AVERAGE = 8, 兩張影像取平均 (bSingle 須為 false)
			//                  : CALCULATOR_RESIZE = 9, 影像縮放, nGrayValue=模式 1=>指定倍率 dValue , 2=>指定大小 nDstValue1=Dst_Width ; nDstValue2=Dst_Height
			//                  : CALCULATOR_COPY = 10, 影像Copy, 可設定 Mask 或 ROI, dValue值可當背景色
			//                  : CALCULATOR_AND = 11, 兩張影像取交集
			//                  : CALCULATOR_OR = 12, 兩張影像取聯集
			//                  : CALCULATOR_XOR = 13, 兩張影像取 互斥或
			//                  : CALCULATOR_EXTRACT_ROI = 14, 取出ROI範圍的影像, 須設定ROI1
			//                  : CALCULATOR_FILL = 15, 影像填充, 指定顏色填入影像(須設定Mask1) 或 ROI(須設定ROI1), dValue值為填充的顏色
			//                  : CALCULATOR_FILL_BOUNDARY = 16, 指定顏色填入影像外圍邊界, 須設定 ROI1(上下邊界) 與 ROI2(左右邊界)
			//                  : CALCULATOR_FILL_VALUE = 17, 取代指定的顏色 (須設定 nGrayValue 與 dValue 以及 eQModel)	
			//                  : CALCULATOR_FINAL = 18,
			ECalculatorMode eMode;

			// 模式指定值, 預設為 0
			// 在 eMode為CALCULATOR_FILL_VALUE時, nGrayValue為要被取代的値, 搭配dValue(要填入的値)使用
			// 當 eMode = CALCULATOR_RESIZE 時, 為模式指定1=>倍率(dValue), 2=>大小(nDstValue1=width, nDstValue2=height)
			int nGrayValue;

			// 當 eMode = CALCULATOR_RESIZE 時 才有用到
			int nDstValue1;
			int nDstValue2;

			// 條件模式 預設為 QUALIFICATION_NONE
			// 目前在 eMode為CALCULATOR_FILL_VALUE時, 會被使用
			// QUALIFICATION_NONE = 0, 不處理
			// QUALIFICATION_GREATER = 1, 大於
			// QUALIFICATION_LESS = 2, 小於
			// QUALIFICATION_EQUAL = 3, 等於
			// QUALIFICATION_GREATER_EQUAL = 4, 大於等於
			// QUALIFICATION_LESS_EQUAL = 5, 小於等於		
			EQualification eQModel;

			// 預設為 0.0
			// bSingle 為 true 時(單張影像處理), 才會用到
			// 當 eMode = CALCULATOR_RESIZE 時 為縮放倍率
			double dValue;

			SImageCalculatorParam() : eMode(CALCULATOR_NONE), nGrayValue(0), nDstValue1(0), nDstValue2(0), eQModel(QUALIFICATION_NONE), dValue(0.0) {
				eProcessMode = PROCESS_CALCULATOR;
			}

			~SImageCalculatorParam() = default;

			std::unique_ptr<BaseParameter> Clone() const override {
				return std::make_unique<SImageCalculatorParam>(*this); // 深拷貝
			}

			bool Check() const
			{
				if (!BaseParameter::Check())
				{
					return false;
				}

				if (eMode < CALCULATOR_NONE || eMode >= CALCULATOR_FINAL)
				{
					strErrorMessage = "模式設定超出範圍";
					return false;
				}

				if (eMode == CALCULATOR_RESIZE)
				{
					if (nGrayValue < 1 || nGrayValue > 2)
					{
						strErrorMessage = "縮放模式設定錯誤";
						return false;
					}
					if (nGrayValue == 1 && dValue <= 0.0)
					{
						strErrorMessage = "縮放倍率設定錯誤";
						return false;
					}
					else if (nGrayValue == 2 && (nDstValue1<=0 || nDstValue2<=0))
					{
						strErrorMessage = "目標大小設定超出範圍";
						return false;
					}
				}

				if (eMode == CALCULATOR_FILL_VALUE)
				{
					if (eQModel == QUALIFICATION_NONE)
					{
						strErrorMessage = "CALCULATOR_FILL_VALUE模式的條件模式設定錯誤";
						return false;
					}

					if (nGrayValue < 0 || nGrayValue > 255)
					{
						strErrorMessage = "CALCULATOR_FILL_VALUE模式的指定值範圍錯誤";
						return false;
					}

					if (dValue < 0 || dValue > 255)
					{
						strErrorMessage = "CALCULATOR_FILL_VALUE模式的填入值範圍錯誤";
						return false;
					}
				}

				return true;
			}
		};

#pragma region FeatureAnalyze
		
		struct SFeatureResult_Base
		{
		public:
			// 特徵值
			// 第一維 : nCount_Contours
			// 第二為 : nCount_Feature
			vector<vector<float>> vt2fFeatureValue;

			// 結果
			vector<vector<bool>> vt2bResult;

		public:
			SFeatureResult_Base() : nCount_Contours(0), nCount_FeatureOn(0) {};
			~SFeatureResult_Base() = default;

			bool CreateBuffer(const int nC, const int nF)
			{
				if (nC <= 0 || nF <= 0)
				{
					return false;
				}

				nCount_Contours = nC;
				nCount_FeatureOn = nF;
				vt2fFeatureValue.resize(nCount_Contours, vector<float>(nCount_FeatureOn, 0.0));
				vt2bResult.resize(nCount_Contours, vector<bool>(nCount_FeatureOn, false));

				return true;
			}

		protected:

			// 輪廓數量
			int nCount_Contours;

			// 啟用特徵的數量
			int nCount_FeatureOn;
		};

		// 基礎特徵參數
		struct SFeatureParam_Base
		{
		public:
			// 特徵閥值, size() = nCount_Feature
			vector<float> vtfThreshold;

			// 判斷條件
			vector<EQualification> vteQualification;

		protected:

			// 紀錄特徵方法, size() = nCount_Feature
			vector<int> vtnFeatureMethod;

			// 控制是否啟用 vtnFeatureMethod
			vector<bool> vtbFeatureOn;

		private:

			// 特徵數量
			int nCount_Feature;

			mutable string strErrorMessage;

		public:
			SFeatureParam_Base() : nCount_Feature(0), strErrorMessage("") {}
			~SFeatureParam_Base() = default;

			// 計算實際使用的特徵數
			const int GetMethod_On_Number()
			{
				int nN = 0;
				for (int k = 0; k < 14; ++k)
				{
					if (vtbFeatureOn[k])
					{
						++nN;
					}
				}
				return nN;
			}

			// nId 從0開始
			const bool GetFeatureMethod_State(const int nId) const { return vtbFeatureOn[nId]; }

			// 取出特徵方法數量
			const int GetFeatureMethod_Number() const { return nCount_Feature; }

			// 啟用特徵方法
			// nFeatureId : 從1開始
			bool SetFeatureMethod_On(const int& nFeatureId, const float fThres, const EQualification eQ)
			{
				if (nFeatureId < 1 || nFeatureId > nCount_Feature)
				{
					strErrorMessage = "Id設定異常";
					return false;
				}

				if (eQ <= QUALIFICATION_NONE || eQ >= QUALIFICATION_FINAL)
				{
					strErrorMessage = "判斷條件設定異常";
					return false;
				}

				int nId = nFeatureId - 1;
				vtbFeatureOn[nId] = true;
				vtfThreshold[nId] = fThres;
				vteQualification[nId] = eQ;

				return true;
			}

			// 關閉特徵方法
			// vtnId : 從1開始
			bool SetFeatureMethod_Off(const vector<int>& vtnId)
			{
				int nCount = vtnId.size();
				if (nCount < 1)
				{
					strErrorMessage = "Id數量錯誤";
					return false;
					//throw std::invalid_argument("SetFeatureOn : Error1");
				}

				int nId = 0;
				for (int k = 0; k < nCount; ++k)
				{
					nId = vtnId[k] - 1;
					if (nId < 0 || nId >= nCount_Feature) return false;
					vtbFeatureOn[nId] = false;
				}
				return true;
			}

			// 設定特徵方法的閥值
			// nFeatureId : 從1開始
			bool SetThreshold(const int nFeatureId, const float fT)
			{
				if (nFeatureId < 1 || nFeatureId > nCount_Feature)
				{
					strErrorMessage = "Id設定異常";
					return false;
					//throw std::invalid_argument("SetThreshold_2 : Error1");
				}
				vtfThreshold[nFeatureId - 1] = fT;
				return true;
			}

			// 設定特徵方法的判斷條件
			// nFeatureId : 從1開始
			bool SetQualification(const int nFeatureId, EQualification eQ)
			{
				if (nFeatureId < 1 || nFeatureId > nCount_Feature || eQ <= QUALIFICATION_NONE || eQ >= QUALIFICATION_FINAL)
				{
					strErrorMessage = "判斷條件設定異常";
					return false;
					//throw std::invalid_argument("SetQualification : Error1");
				}
				vteQualification[nFeatureId - 1] = eQ;
				return true;
			}

			bool qualifyFeature(const float& featureValue, const float& threshold, const EQualification& qualification)
			{
				switch (qualification)
				{
				case QUALIFICATION_GREATER:
					return featureValue > threshold;
				case QUALIFICATION_LESS:
					return featureValue < threshold;
				case QUALIFICATION_EQUAL:
					return featureValue == threshold;
				case QUALIFICATION_GREATER_EQUAL:
					return featureValue >= threshold;
				case QUALIFICATION_LESS_EQUAL:
					return featureValue <= threshold;
				default:
					return false;
				}
			}

			bool Check()
			{
				if (nCount_Feature < 0 || nCount_Feature != vtfThreshold.size() || nCount_Feature != vteQualification.size())
				{
					return false;
				}

				return true;
			}

		protected:
			// 設定使用的特徵方法
			bool SetSumFeatureMethod(const vector<int>& vtnMethod)
				{
					if (vtnMethod.empty())
					{
						strErrorMessage = "輸入內容為空";
						return false;
						//throw std::invalid_argument("SetFeatureType_Vector : Feature type list cannot be empty");
					}

					if (vtnMethod.empty() || std::any_of(vtnMethod.begin(), vtnMethod.end(), [](int type) { return type < 0; }))
					{
						strErrorMessage = "數量不可為0";
						return false;
						//throw std::invalid_argument("SetFeatureType_Vector : Feature type must be greater than 0");
					}
					vtnFeatureMethod = vtnMethod;
					nCount_Feature = vtnFeatureMethod.size();
					vtbFeatureOn.assign(nCount_Feature, false);
					vteQualification.resize(nCount_Feature, QUALIFICATION_NONE);
					vtfThreshold.resize(nCount_Feature, 0.0);
					return true;
				}
		};

		// GLCM 參數
		struct SGLCM_Param : SFeatureParam_Base
		{
		public:
			int nInterval_X;
			int nInterval_Y;
			int nGrayLevel;
			vector<bool> vtbKeep;
			vector<vector<vector<int>>> vt3nGLCM;
			vector<vector<vector<float>>> vt3fPdf;

		public:
			SGLCM_Param(const int nShift_X=1, const int nShift_Y=1, const int nGrayLevel=1) : nInterval_X(nShift_X), nInterval_Y(nShift_Y), nGrayLevel(nGrayLevel)
			{
				// GLCM 特徵方法目前有14項, 
				// 0=熵, 
				// 1=一階矩, 
				// 2=二階矩, 
				// 3=三階矩, 
				// 4=四階矩, 
				// 5=逆一階矩, 
				// 6=逆二階矩, 
				// 7=逆三階矩, 
				// 8=逆四階矩
				// 9=對比度, 
				// 10=均勻性/能量, 
				// 11=同質性, 
				// 12=相關性, 
				// 13=方差
				vector<int> vtnMethod(14, 0);
				SetSumFeatureMethod(vtnMethod);
			}
			~SGLCM_Param() = default;

			bool CreateGLCM_Buffer(const int nCount_Contours)
			{
				if (nCount_Contours <= 0) return false;
				vt3nGLCM.resize(nCount_Contours);
				vt3fPdf.resize(nCount_Contours);
				vtbKeep.resize(nCount_Contours, true);
				return true;
			}

			bool Check()
			{
				if (SFeatureParam_Base::Check() == false)
				{
					return false;
				}

				if (nInterval_X < 1 || nInterval_Y < 1 || nGrayLevel < 1 || nGrayLevel > 255)
				{
					return false;
				}

				return true;
			}
		};

		// GLCM 特徵
		struct SFeature_GLCM 
		{

		public :		
			vector<SGLCM_Param>			vtsParam;
			vector<SFeatureResult_Base> vtsResult;
			vector<bool>                vtbResult;

		protected:
			int nCount_Param;

			const SContoursParam* psContours;

		public:
			SFeature_GLCM() : nCount_Param(0), psContours(nullptr) {};
			~SFeature_GLCM() = default;

			const int GetParameterNumber() const {return nCount_Param;}

			bool AddParam(const int nShift_X, const int nShift_Y, const int nGrayLevel)
			{
				if (nShift_X < 0 || nShift_Y < 0 || nGrayLevel < 1 || nGrayLevel >255)
				{
					return false;
				}
				SGLCM_Param sTemp(nShift_X, nShift_Y, nGrayLevel);
				vtsParam.push_back(sTemp);
				nCount_Param = vtsParam.size();
				return true;
			}

			void CleraParam()
			{
				vtsParam.clear();
				vtsResult.clear();
				nCount_Param = vtsParam.size();
			}

			// 取出輪廓指標
			const SContoursParam* GetContoursPtr() const { return psContours; }

			// 設定輪廓指標
			bool SetContoursPtr(SContoursParam& sCP)
			{
				if (sCP.Check() == false)
				{
					return false;
				}
				psContours = &sCP;
				return true;
			}

			// 取出輪廓數量
			const int GetContoursNumber() const 
			{ 
				if (psContours == nullptr) return 0;
				return psContours->GetCount();
			}

			bool CreateResultBuffer()
			{
				int nCount_Contours = psContours->GetCount();
				if (nCount_Param <= 0 || nCount_Param!= vtsParam.size() || nCount_Contours<0)
				{
					return false;
				}

				vtbResult.resize(nCount_Contours, true);
				vtsResult.resize(nCount_Param);
				for (int k = 0; k < nCount_Param; ++k)
				{
					if (vtsParam[k].CreateGLCM_Buffer(nCount_Contours) == false)
					{
						return false;
					}

					int nCount_FeatureOn = vtsParam[k].GetMethod_On_Number();
					if (vtsResult[k].CreateBuffer(nCount_Contours, nCount_FeatureOn) == false)
					{
						return false;
					}
				}

				return true;
			}

			// 進行特徵分析
			bool CalculationResult()
			{
				int nCount_Contours = psContours->GetCount();
				if (nCount_Param <= 0 || nCount_Contours <= 0)
				{
					return false;
				}

				for (int k = 0; k < nCount_Param; ++k)
				{
					int nCount_SumFeature = vtsParam[k].GetFeatureMethod_Number();
					int nCount_FeatureOn = vtsParam[k].GetMethod_On_Number();
					if (nCount_SumFeature <= 0 || nCount_FeatureOn <= 0 || nCount_FeatureOn > nCount_SumFeature)
					{
						return false;
					}

					for (int h = 0; h < nCount_Contours; ++h)
					{
						if (vtsParam[k].vtbKeep[h])
						{
							int nId = 0;
							for (int f = 0; f < nCount_SumFeature; ++f)
							{
								if (vtsParam[k].GetFeatureMethod_State(f))
								{
									vtsResult[k].vt2bResult[h][nId] = vtsParam[k].qualifyFeature(vtsResult[k].vt2fFeatureValue[h][nId], vtsParam[k].vtfThreshold[f], vtsParam[k].vteQualification[f]);
									if(!vtsResult[k].vt2bResult[h][nId]) vtbResult[h] = false;
									++nId;
								}
							}
						}
						else
						{
							vtbResult[h] = false;
							for (int f = 0; f < nCount_FeatureOn; ++f)
							{
								vtsResult[k].vt2bResult[h][f] = false;
							}
						}
					}
				}

				return true;
			}

			bool Check()
			{
				int nCunt = vtsParam.size();
				if (nCunt == 0) return true;

				for (int k = 0; k < nCunt; ++k)
				{
					if (vtsParam[k].Check() == false) return false;
				}
				return true;
			}
		};

		// 特徵分析
		struct SFeatureAnalyzeParam : BaseParameter
		{
		public:

			// FEATURE_NONE = 0, 不處理
			// FEATURE_GLCM = 1, 灰階共生矩陣
			// FEATURE_ANGLE = 2, 計算物件的平面旋轉角度
			// FEATURE_CHAIN_CODE = 3, 計算物件的鍊碼
			// FEATURE_ANALYZE = 4, 特徵分析
			// FEATURE_FINAL = 5,
			EFeatureMode eMode;			

			int nId_Queue_GLCMContours;

			SContoursParam sContours_Out;

			SFeature_GLCM sGLCM;

		public:
			SFeatureAnalyzeParam() : nId_Queue_GLCMContours(0), eMode(FEATURE_NONE) {
				eProcessMode = PROCESS_FEATURE;
			}

			~SFeatureAnalyzeParam() = default;

			std::unique_ptr<BaseParameter> Clone() const override {
				return std::make_unique<SFeatureAnalyzeParam>(*this); // 深拷貝
			}

			SContoursParam& GetContoursParam_Out() { return sContours_Out; }

			bool SetContoursParam_GLCM(vector<BaseParameter*>& vtpBPqueue)
			{
				int nCount = vtpBPqueue.size();
				if (nCount <= 0 || nId_Queue > nCount || nId_Queue_GLCMContours < 1 || nId_Queue_GLCMContours >= nId_Queue)
				{
					return false;
				}

				int nId = nId_Queue_GLCMContours - 1;
				if (vtpBPqueue[nId] == nullptr)
				{
					return false;
				}

				if (vtpBPqueue[nId]->GetProcessMode() != PROCESS_CONTOUR && vtpBPqueue[nId]->GetProcessMode() != PROCESS_FEATURE)
				{
					return false;
				}

				if (vtpBPqueue[nId]->GetProcessMode() == PROCESS_CONTOUR)
				{
					SContoursShapeParam* psContours = (SContoursShapeParam*)vtpBPqueue[nId];
					if (psContours->eMode == CONTOUR_BOUNDING_RECTANGLE)
					{
						sGLCM.SetContoursPtr(*psContours->GetContoursParam_In());
					}
					else
					{
						sGLCM.SetContoursPtr(psContours->GetContoursParam_Out());
					}
				}
				else if (vtpBPqueue[nId]->GetProcessMode() == PROCESS_FEATURE)
				{
					SFeatureAnalyzeParam* psFeature = (SFeatureAnalyzeParam*)vtpBPqueue[nId];
					sGLCM.SetContoursPtr(psFeature->GetContoursParam_Out());
				}

				return true;
			}

			bool Check()
			{

				return true;
			}

		protected:
			
		};
#pragma endregion

#pragma region Qualification

		struct SQualification_Parameter
		{
			// 預設為 0
			// 指令輸入來源, 
			// 1=>影像, 2=>輪廓, 3=>ROI, 4=>使用者輸入
			int nSrcType;

			// 預設為 0
			// nSrcType : 1 => nParameter1 : 1=>影像寬, 2=>影像高, 3=>影像面積
			//          : 2 => nParameter1 : 1=>輪廓總數, 2=>輪廓on的總數, 3=>指定id的個數, 4=>指定id的ROI寬, 5=>指定id的ROI高, 6=>指定id的ROI面積(id由nParameter2設定)
			//          : 3 => nParameter1 : 1=>ROI寬, 2=>ROI高, 3=>ROI面積
			//          : 4 => nParameter1 : 使用者輸入
			int nParameter1;

			// 預設為 0
			int nParameter2;

			SQualification_Parameter() : nSrcType(0), nParameter1(0), nParameter2(0) {}
			~SQualification_Parameter() = default;

			bool SetParameter(const int& nType, const int& nP1, const int& nP2)
			{
				switch (nType)
				{
				case 1:
					if (nP1 < 1 || nP1 > 3) return false;
					break;
				case 2:
					if (nP1 < 1 || nP1 > 6) return false;
					if (nP1 == 3 || nP1 == 4 || nP1 == 5 || nP1 == 6) {
						if (nParameter2 < 0) return false;
					}
					break;
				case 3:
					if (nP1 < 1 || nP1 > 3) return false;
					break;
				default:
					return false;
				}

				nSrcType = nType;
				nParameter1 = nP1;
				nParameter2 = nP2;

				return true;
			}

			bool Check() const
			{
				switch (nSrcType)
				{
				case 1:
					if (nParameter1 < 1 || nParameter1 > 3) return false;
					break;
				case 2:
					if (nParameter1 < 1 || nParameter1 > 6) return false;
					if (nParameter1 == 3 || nParameter1 == 4 || nParameter1 == 5 || nParameter1 == 6) {
						if (nParameter2 < 0) return false;
					}
					break;
				case 3:
					if (nParameter1 < 1 || nParameter1 > 3) return false;
					break;
				default:
					return false;
				}
				return true;
			}
		};

		// 判斷式條件參數設定
		struct SQualification_One
		{
		public:

			// 預設為 0
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
			EQualification				eQualification;

			SQualification_Parameter	sL_Parameter;

			SQualification_Parameter	sR_Parameter;	

			// 預設為 0
			// 1=>bool, 2=>L Parameter, 3=>R Parameter
			int nOutputType;

		public:
			SQualification_One() : eQualification(QUALIFICATION_NONE), sL_Parameter(), sR_Parameter(), nOutputType(0) {}
			~SQualification_One() = default;

			bool Check() const
			{
				if (eQualification <= QUALIFICATION_NONE || eQualification >= QUALIFICATION_FINAL || !sL_Parameter.Check() || !sR_Parameter.Check() ||
					nOutputType < 1 || nOutputType > 3) {
					return false;
				}
				return true;
			}
		};

		// 判斷式條件參數
		struct SQualificationParam :BaseParameter
		{
		public :

			// 預設為 0
			int nCount;
			vector<SQualification_One>	vtsQualification;
			vector<EQualification>		vteBetween;

			// 設定 Replace 指令
			//std::vector<std::unique_ptr<BaseParameter>> vtpsBP;
		private :

			bool  bResult;

		public:
			SQualificationParam() : nCount(0), bResult(false) {
				eProcessMode = PROCESS_QUALIFICATION;
			}

			// 自己撰寫拷貝建構子（deep copy）
			SQualificationParam(const SQualificationParam& other)
				: BaseParameter(other) // 明確調用基底類的拷貝建構子
				, nCount(other.nCount)
				, vtsQualification(other.vtsQualification)
				, vteBetween(other.vteBetween)
				, bResult(other.bResult)
			{
				eProcessMode = PROCESS_QUALIFICATION;
#if 0
				// 注意：other.vtpsBP[i] 是 unique_ptr<BaseParameter>
				//       必須呼叫其 Clone() (或複製建構子) 來新建一個物件
				vtpsBP.reserve(other.vtpsBP.size());
				for (const auto &bp : other.vtpsBP) {
					if (bp) {
						// 假設 bp->Clone() 返回 std::unique_ptr<BaseParameter>
						vtpsBP.push_back(bp->Clone());
					}
					else {
						// 如果原本是 nullptr，就放 nullptr
						vtpsBP.push_back(nullptr);
					}
				}
#endif
			}

			// 自己撰寫拷貝賦值運算子（deep copy）
			SQualificationParam& operator=(const SQualificationParam& other)
			{
				if (this == &other) {
					return *this; // 避免自我賦值
				}

				BaseParameter::operator=(other); // 顯式調用基底類的賦值運算子

				// 複製其它成員
				nCount = other.nCount;
				vtsQualification = other.vtsQualification;
				vteBetween = other.vteBetween;
				bResult = other.bResult;
				eProcessMode = other.eProcessMode;

#if 0
				// 先把舊的釋放掉
				//vtpsBP.clear();

				// 深拷貝 vtpsBP
				vtpsBP.reserve(other.vtpsBP.size());
				for (const auto &bp : other.vtpsBP) {
					if (bp) {
						vtpsBP.push_back(bp->Clone());
					}
					else {
						vtpsBP.push_back(nullptr);
					}
				}
#endif
				return *this;
			}

			~SQualificationParam() = default;

			std::unique_ptr<BaseParameter> Clone() const override {
				return std::make_unique<SQualificationParam>(*this); // 深拷貝
			}

			bool GetResult() const {return bResult;}

			bool Check()
			{
				if(nCount <= 0 || nCount != vtsQualification.size() || vteBetween.size() < nCount-1) {
					return false;
				}
				return true;
			}
		};

#pragma endregion

#pragma region PatternMatch (under construction)
		// 旋轉相關參數
		struct SRotationParametar
		{
		public:
			// 預設為 false
			// 控制是否開啟旋轉偵測
			bool					bEnable;

			// 預設為 5.0, 須為正數
			// 角度偵測範圍, 須為正數(單位:度)
			float					fAngleRange;

			// 預設為 0.1, 須為正數
			// 角度偵測的最小刻度, 須為正數(單位:度)
			float                   fMinAngleScale;

			// nMinShiftX 與 nMinShiftY 皆預設為 1, 須為正數
			// 計算旋轉 Pattern的四個角點 fAngleScale_First度時會有 nMinShiftX 與 nMinShiftY 的偏移量
			int						nMinShiftX;
			int                     nMinShiftY;

		protected:

			// 預設size為1, 值為0.1, 此值不需設定, 呼叫CalculateRotationScale()產生
			// 初步偵測旋轉角度的旋轉刻度
			mutable float   fRotationScale;

		public :
			SRotationParametar() : bEnable(false), fAngleRange(5.0), fMinAngleScale(0.1), nMinShiftX(1), nMinShiftY(1), fRotationScale(0.1){}

			~SRotationParametar() = default;

			bool Check() const
			{
				if (bEnable)
				{
					if (fAngleRange <= 0.0 || fMinAngleScale <= 0.0 || fMinAngleScale <= 0.0 || nMinShiftX <= 0 || nMinShiftY <= 0 || fRotationScale <= 0.0)
					{
						return false;
					}
				}
				return true;
			}

			// 計算旋轉刻度 fRotationScale
			// nPatternWidth : Pattern影像的寬 
			// nPatternHeight : Pattern影像的高
			bool CalculateRotationScale(const int& nPatternWidth, const int& nPatternHeight) const
			{
				fRotationScale = 0.1;
				if (nPatternWidth <= 0 || nPatternHeight <= 0 || !Check()) {
					return false;
				}

				const float fPI = 3.14159265359;
				vector<float> vtfCornerX(4, 0.0);
				vector<float> vtfCornerY(4, 0.0);

				// 右上
				vtfCornerX[1] = nPatternWidth - 1;
				vtfCornerY[1] = 0;

				// 右下
				vtfCornerX[1] = nPatternWidth - 1;
				vtfCornerY[1] = nPatternHeight - 1;

				// 左下
				vtfCornerX[1] = 0;
				vtfCornerY[1] = nPatternHeight - 1;

				const float fMaxAngle = 45.0;
				int nCount_Angle = (fMaxAngle / fMinAngleScale) + 1;
				bool bChange = true;

				float fWidth = nPatternWidth;
				float fHeight = nPatternHeight;
				float fCenterX = fWidth / 2.0;
				float fCenterY = fHeight / 2.0;

				for (int k = 1; k <= nCount_Angle; ++k)
				{
					float fAngleStep = k*fMinAngleScale;
					float fRadian = (fAngleStep*fPI) / 180.0;
					float fCos = cos(fRadian);
					float fSin = sin(fRadian);

					for (int h = 0; h < 4; ++h)
					{
						float fDiffX = vtfCornerX[h] - fCenterX;
						float fDiffY = vtfCornerY[h] - fCenterY;

						float fRotateX = (fDiffX*fCos - fDiffY*fSin) + fCenterX;
						float fRotateY = (fDiffX*fSin + fDiffY*fCos) + fCenterY;

						float fErrorX = fabs(fRotateX - vtfCornerX[h]);
						float fErrorY = fabs(fRotateY - vtfCornerY[h]);

						if (fErrorX >= nMinShiftX || fErrorY >= nMinShiftY) {
							fRotationScale = fAngleStep;
							h = 4;
							k = nCount_Angle + 1;
							bChange = false;
						}
					}
				}

				if (bChange) {
					fRotationScale = fMaxAngle + fMinAngleScale;
				}

				return true;
			}

			float& GetRotationScale() const {return fRotationScale;}
		};

		struct SSubPattern
		{
			// 預設為 0, 子Pattern數量, 0代表不使用, 不可大於4
			// 設定好數量與位置會自動生成
			int		nSubPatternNumber;

			// 預設為 1, 1=> 左上 右上 右下 左下, 2=>上 下 左 右, 3=>手動設定
			// 當 nSubPatternNumber<4時, 選擇1時會依Pattern 左上 右上 右下 左下的順序來設置, 選擇2時會依Pattern的上 下 左 右的順序來設置, 
			// 生成 子Pattern 位置的分佈模式1
			int	    nSubPatternLocationMode1;

			// 預設size為 0, 當 nSubPatternLocationMode1為 3時才需設定
			// 以Pattern為中心, 0=>中心, 1=>左上, 2=>上, 3=>右上, 4=>右, 5=>右下, 6=>下, 7=>左下, 8=>左, 
			// vtnSubPatternLocationMode2 的 size 必須與nSubPatternNumber相同
			vector<int>     vtnSubPatternLocationMode2;

			// 子 pattern 的位置, 預設size為 0
			// 第一層的size對應到 Pattern的數量,
			// 子Pattern的Size 會以 min(PatternWidth, PatternHeight)/2, 來設定
			vector<vector<RECT>> vt2reSubPatternROI;

			SSubPattern() : nSubPatternNumber(0), nSubPatternLocationMode1(1){}
			~SSubPattern() = default;

			bool Check() const
			{
				if (nSubPatternNumber > 0)
				{
					if (nSubPatternLocationMode1 < 1 || nSubPatternLocationMode1>3 || nSubPatternNumber != vtnSubPatternLocationMode2.size() ||
						nSubPatternNumber != vt2reSubPatternROI.size())
					{
						return false;
					}
				}
				return true;
			}
		};

		// Pattern 基底參數
		struct SPatternBase
		{

		protected:

			// 預設為 1, 不可小於1, 不可大於5 
			// 設定 Pattern 的數量
			// 只會用第一個 Pattern 計算旋轉角度
			int    nPatternNumber;

			// 預設 size皆為1, 內容為 0
			// Pattern 的 寬 高 與 channel數 
			vector<int>    vtnPatternWidth;
			vector<int>    vtnPatternHeight;
			vector<int>    vtnPatternChannel;

			// SubPattern
			vector<SSubPattern> vtsSubPattern;

		public:

			SPatternBase() : nPatternNumber(1)
			{
				vtnPatternWidth.resize(1, 0);
				vtnPatternHeight.resize(1, 0);
				vtnPatternChannel.resize(1, 0);
				vtsSubPattern.resize(1);
			}

			~SPatternBase() = default;

			bool Check() const
			{
				if (nPatternNumber < 1 || nPatternNumber > 5 || nPatternNumber != vtnPatternWidth.size() || nPatternNumber != vtnPatternHeight.size() || nPatternNumber != vtnPatternChannel.size() ||
					nPatternNumber != vtsSubPattern.size())
				{
					return false;
				}

				for (int k = 0; k < nPatternNumber; ++k)
				{
					if (vtnPatternWidth[k] < 0 || vtnPatternHeight[k] < 0 || (vtnPatternChannel[k] != 1 && vtnPatternChannel[k] != 3) || !vtsSubPattern[k].Check())
					{
						return false;
					}
				}
				return true;
			}
		};

		// Pattern 相關參數
		struct SPatternParameter : SPatternBase
		{

		protected:

			// 直方圖 pattern 的直徑
			int nHistPatternDiameter;

			// 直方圖 pattern 的半徑
			int nHistPatternRadius;

			// 原始Pattern在HistPattern的位置,
			RECT reHistPatternROI;

		public:

			SPatternParameter() : SPatternBase(), nHistPatternDiameter(1), nHistPatternRadius(1)
			{
				reHistPatternROI.left = 0;
				reHistPatternROI.top = 0;
				reHistPatternROI.right = 0;
				reHistPatternROI.bottom = 0;
			}

			~SPatternParameter() = default;

			bool CalculateHistogramPatternSize()
			{
				if (vtnPatternWidth[0] <= 0 || vtnPatternHeight[0] <= 0) return false;
				nHistPatternDiameter = (vtnPatternWidth[0] > vtnPatternHeight[0]) ? vtnPatternWidth[0] : vtnPatternHeight[0];
				nHistPatternRadius = nHistPatternDiameter / 2;
				if (nHistPatternDiameter <= 0 || nHistPatternRadius <= 0) return false;	
				return true;
			}

			// 取出直方圖Pattern直徑
			int GetHistogramPatternDiameter() const 
			{ 
				return nHistPatternDiameter;
			}

			// 取出直方圖Pattern半徑
			int GetHistogramPatternRadius() const 
			{ 
				return nHistPatternRadius;
			}

			bool Check() const
			{
				if (!SPatternBase::Check() || nHistPatternDiameter <= 0 || nHistPatternRadius <= 0) {
					return false;
				}
				return true;
			}

			bool Check(const int& nImageW, const int& nImageH) const
			{
				if (!SPatternBase::Check() || nHistPatternDiameter <= 0 || nHistPatternRadius <= 0) {
					return false;
				}

				if (reHistPatternROI.left < 0 || reHistPatternROI.top < 0 || reHistPatternROI.right >= nImageW || reHistPatternROI.bottom >= nImageH) {
					return false;
				}
				return true;
			}
		};

		struct SPatternMathParameter :BaseParameter
		{
		public :

			// 預設為 0
			// 圖像比對模式 : PATTERN_NONE = 0,				不處理
			//              : PATTERN_CREATE_PATTERN = 1,	建立 Pattern, 在sInput1中 輸入影像與 ROI; nMethod=1時, pattern 輸出在 sOutput.vtmatImage; 
			//                                              nMethod=2時, Mask輸出在sOutput.vtmatImage[0], pattern輸出在sOutput.vtmatImage[1], Pattern ROI輸出在 sOutput.cvROI
			//              : PATTERN_MATCH = 2,			Match, 在sInput1中輸入Pattern , 在sInput2中輸入搜尋影像
			//              : PATTERN_FINAL = 3,
			EPatternMatch			eMode;

			// 預設為 1
			// nMethod : 1=> 使用標準化共變異數(相關係數)
			//         : 2=> 使用直方圖
			int						nMethod;

			// 預設為 1, 
			// 最多可以偵測多少個物件
			int						nMatchNumber;

			// 預設為 70
			// 如果nMatchNumber為3, 有10個大於70分, 此時只會輸出分數最大的前3個
			// 比對成功的最低分數
			int						nScore;

			// 旋轉相關參數
			// 如果使用多個 pattern, 只會用第一個計算旋轉角度
			SRotationParametar		sRotation;

			// Pattern 相關參數
			SPatternParameter       sPattern;

		public:
			SPatternMathParameter() : eMode(PATTERN_NONE), nMethod(1), nMatchNumber(1), nScore(70), sRotation(), sPattern(){}
			~SPatternMathParameter() = default;

			std::unique_ptr<BaseParameter> Clone() const override {
				return std::make_unique<SPatternMathParameter>(*this); // 深拷貝
			}

			bool Check() const
			{
				if (!BaseParameter::Check())
				{
					return false;
				}

				if (eMode < PATTERN_NONE || eMode >= PATTERN_FINAL)
				{
					strErrorMessage = "模式設定超出範圍";
					return false;
				}

				if (!sRotation.Check())
				{
					strErrorMessage = "相關參數設定錯誤";
					return false;
				}

				if (!sPattern.Check())
				{
					strErrorMessage = "相關參數設定錯誤";
					return false;
				}

				if (nMethod < 1 || nMethod>2 || nMatchNumber < 1 || nScore < 0 || nScore>100 )
				{
					return false;
				}
				return true;
			}
		};

#pragma endregion
	
		// ========== 流程節點結構 ==========
		struct SProcessNode
		{
		public:
 
			std::unique_ptr<BaseParameter> unptrParam;

			// 預設值為 0, 0代表不啟用
			// 正常執行時下一步的 StepId
			int nNextStep;       

			// 預設值為 0, 0代表不啟用
			// 若結果不符合，倒回至哪個步驟
			int nRollbackStep;            

		private:

			// 預設為 0
			// 已執行的倒退次數
			int nTimes;

			// 預設為 0
			// 倒退重來的最大次數
			int nMaxTimes;

			// 預設值為 0, 0代表不啟用
			// 要用哪個步驟的參數倒回, 必須與nRollbackStep對應的參數類型相同, vtnReplaceStep.size()=nMaxTimes;
			vector<int>  vtnReplaceStep;

			mutable string strErrorMessage;

		public:
			SProcessNode() : unptrParam(nullptr), nNextStep(0), nRollbackStep(0), nTimes(0), nMaxTimes(0) {}

			// 接受一個「基底參數的參考」，在此建構時進行 Clone()
			SProcessNode(const BaseParameter& param, int nextStep, int rollbackStep, const vector<int>& vtreplaceStep)
				: nNextStep(nextStep)
				, nRollbackStep(rollbackStep)
				, nTimes(0)
			{
				// 利用多型的Clone()，避免 slicing
				unptrParam = param.Clone();

				int nT = static_cast<int>(vtreplaceStep.size());
				if (nT > 0) {
					nMaxTimes = nT;
					vtnReplaceStep = vtreplaceStep;
				}
				else {
					nMaxTimes = 0;
				}
			}

			// 如果需要拷貝 SProcessNode，也最好實作 copy constructor:
			SProcessNode(const SProcessNode& other)
				: nNextStep(other.nNextStep)
				, nRollbackStep(other.nRollbackStep)
				, nTimes(other.nTimes)
				, nMaxTimes(other.nMaxTimes)
				, vtnReplaceStep(other.vtnReplaceStep)
				, strErrorMessage(other.strErrorMessage)
			{
				// 需要深拷貝 sParam
				if (other.unptrParam)
					unptrParam = other.unptrParam->Clone();
				else
					unptrParam = nullptr;
			}

			// 同理，若需要賦值運算子，也可實作：
			SProcessNode& operator=(const SProcessNode& other)
			{
				if (this == &other) return *this;
				nNextStep = other.nNextStep;
				nRollbackStep = other.nRollbackStep;
				nTimes = other.nTimes;
				nMaxTimes = other.nMaxTimes;
				vtnReplaceStep = other.vtnReplaceStep;
				strErrorMessage = other.strErrorMessage;

				if (other.unptrParam)
					unptrParam = other.unptrParam->Clone();
				else
					unptrParam = nullptr;

				return *this;
			}

			// 判斷當前 nTimes 是否超過 nMaxTimes
			bool IsExceedMaxTimes() const {return (nTimes >= nMaxTimes);}

			// 回傳最大倒退次數
			int GetMaxTimes() const { return nMaxTimes; }

			// 回傳目前的倒退次數
			int GetTimes() const { return nTimes; }

			// 更新倒退次數
			void UpdateTimes() {++nTimes;}

			// 輸入 id 取出 ReplaceStep
			int GetReplaceStep(const int& nId)
			{
				if (nId < 0 || nId >= nMaxTimes) return -1;
				else return vtnReplaceStep[nId];
			}

			string GetErrorMessage() const { return strErrorMessage; }

			bool Check() const
			{
				if (unptrParam == nullptr) {
					strErrorMessage = "指令錯誤" + unptrParam->GetErrorMessage();
					return false;
				}

				if (!unptrParam->Check()) {
					strErrorMessage = "處理指令的" + unptrParam->GetErrorMessage();
					return false;
				}

				if (nNextStep < 0) {
					strErrorMessage = "NextStep 的值小於 0";
					return false;
				}

				if (nRollbackStep < 0) {
					strErrorMessage = "RollbackStep 的值小於 0";
					return false;
				}

				if (vtnReplaceStep.size() > 0) {
					if (nMaxTimes != vtnReplaceStep.size()) {
						strErrorMessage = "ReplaceStep 的數量不等於 MaxTimes";
						return false;
					}
					
					for (int k = 0; k < nMaxTimes; ++k) {
						if (vtnReplaceStep[k] < 0) {
							strErrorMessage = "ReplaceStep 的值小於 0";
							return false;
						}
					}
				}

				return true;
			}
		};

		// 處理模式
		struct SProcessModeParam
		{
		public :
			// 色彩轉換參數
			vector<SColorTransformParam>	vtsColorParam;

			// 影像強化參數
			vector<SImageEnhanceParam>		vtsEnhanceParam;

			// 灰階濾波參數
			vector<SFilterParam>			vtsFilterParam;

			// 二值化參數
			vector<SThresholdParam>			vtsThresholdParam;

			// 形態學參數
			vector<SMorphologParam>			vtsMorphologParam;

			// 輪廓轉換參數
			vector<SContoursShapeParam>		vtsContoursParam;

			// 特徵分析參數
			vector<SFeatureAnalyzeParam>	vtsFeatureParam;

			// 物件刪除參數
			vector<SDeleteObjectParam>		vtsDeleteParam;

			// 影像計算參數
			vector<SImageCalculatorParam>	vtsCalculator;

			// 圖像比對參數
			//vector<SPatternMathParameter>		vtsPatternMatch;

			// 條件判斷參數
			vector<SQualificationParam>		vtsQualification;

		private:
			// 版次
			string                          strVersion;
			vector<pair<int, SProcessNode>> vtsProcessNode; // 按照流程順序儲存
			map<int, int>					StepIndexMap;	// stepId 對應到 vtsProcessNode 的索引

			vector<BaseParameter*>			vtpsBPList;

			int nSumCount;
			mutable string strErrorMessage;

		public:
			SProcessModeParam() : strVersion ("2.0.1") {
				Clear();
			}

			~SProcessModeParam() {
				Clear();
			}

			string GetVersion() const {return strVersion;}

			void SetErrorMessage(string strMessage) { strErrorMessage = strMessage; }

			bool Set(const BaseParameter& sParam)
			{
				if (!sParam.Check())	return false;

				switch (sParam.GetProcessMode())
				{
				case PROCESS_COLOR:
					vtsColorParam.emplace_back(*(SColorTransformParam*)&sParam);
					break;
				case PROCESS_ENHANCE:
					vtsEnhanceParam.emplace_back(*(SImageEnhanceParam*)&sParam);
					break;
				case PROCESS_FILTER:
					vtsFilterParam.emplace_back(*(SFilterParam*)&sParam);
					break;
				case PROCESS_THRESHOLD:
					vtsThresholdParam.emplace_back(*(SThresholdParam*)&sParam);
					break;
				case PROCESS_MORPHOLOG:
					vtsMorphologParam.emplace_back(*(SMorphologParam*)&sParam);
					break;
				case PROCESS_CONTOUR:
					vtsContoursParam.emplace_back(*(SContoursShapeParam*)&sParam);
					break;
				case PROCESS_FEATURE:
					vtsFeatureParam.emplace_back(*(SFeatureAnalyzeParam*)&sParam);
					break;
				case PROCESS_DELETE:
					vtsDeleteParam.emplace_back(*(SDeleteObjectParam*)&sParam);
					break;
				case PROCESS_CALCULATOR:
					vtsCalculator.emplace_back(*(SImageCalculatorParam*)&sParam);
					break;
				//case PROCESS_PATTERN_MATCH:
					//vtsPatternMatch.emplace_back(*(SPatternMathParameter*)&sParam);
					//break;
				case PROCESS_QUALIFICATION:
					vtsQualification.emplace_back(*(SQualificationParam*)&sParam);
					break;
				}
				return true;
			}

			//===================================================
			// (B) 新增一個流程步驟 → 產生對應的 SProcessNode
			//===================================================
			// 這裡改用「傳參考」以避免多餘的拷貝；在 SProcessNode 裡會做 Clone()
			bool AddProcessStep(const BaseParameter& Param,
				const int& nextStep,
				const int& rollbackStep = 0,
				const vector<int>& vtreplaceStep = vector<int>())
			{
				// 先檢查基礎
				if (!Param.Check() || nextStep < 0 || rollbackStep < 0) {
					strErrorMessage = " 輸入值為空或小於 0";
					return false;
				}

				int nQueueId = Param.GetQueueId();
				if (nQueueId < 1) {
					strErrorMessage = " QueueId 不可小於 1";
					return false;
				}

				// 不允許同樣的 QueueId 重複加入
				if (StepIndexMap.find(nQueueId) != StepIndexMap.end()) {
					strErrorMessage = "QueueId 重複";
					return false;
				}

				// 建構一個新的 SProcessNode (內部會把 Param.Clone() 存到 sParam)
				SProcessNode node(Param, nextStep, rollbackStep, vtreplaceStep);

				// 加到容器
				StepIndexMap[nQueueId] = static_cast<int>(vtsProcessNode.size());
				vtsProcessNode.emplace_back(nQueueId, std::move(node));

				// 更新總步數
				nSumCount = static_cast<int>(vtsProcessNode.size());
				return true;
			}

			//===========================================================
			// (C) 將第 nRollbackStep 的節點參數，用 第 nReplaceStep 的節點參數 覆蓋掉
			//===========================================================
			bool ReplaceBaseParameter(SProcessNode* psNode)
			{
				if (!psNode->Check()) {
					return false;
				}

				int nRollbackId = -1;
				{
					auto it = StepIndexMap.find(psNode->nRollbackStep);
					if (it != StepIndexMap.end()) {
						nRollbackId = it->second;  // 找到 vtsProcessNode 中對應的 index
					}
					else {
						strErrorMessage = " RollbackStep步驟有誤";
						return false;
					}
				}

				int nTimes = psNode->GetTimes();
				int nReplaceId = -1;
				{
					auto it = StepIndexMap.find(psNode->GetReplaceStep(nTimes));
					if (it != StepIndexMap.end()) {
						nReplaceId = it->second;
					}
					else {
						strErrorMessage = " ReplaceStep步驟有誤";
						return false;
					}
				}

				if (nRollbackId < 0 || nRollbackId >= nSumCount) {
					strErrorMessage = " 找不到RollbackStep對應的Id";
					return false;
				}
				if (nReplaceId < 0 || nReplaceId >= nSumCount) {
					strErrorMessage = " 找不到ReplaceStep對應的Id";
					return false;
				}

				// 取出目前 rollback 節點、replace 節點 的參數
				// 注意：我們現在是 unique_ptr<BaseParameter>
				SProcessNode& nodeRollback = vtsProcessNode[nRollbackId].second;
				SProcessNode& nodeReplace = vtsProcessNode[nReplaceId].second;
#if 1
				if (nodeRollback.unptrParam && nodeReplace.unptrParam) {
					nodeRollback.unptrParam = nodeReplace.unptrParam->Clone();
				}
#else
				// 先記下 rollback 節點原本的 QueueId
				int nQId = 0;
				if (nodeRollback.unptrParam) {
					nQId = nodeRollback.unptrParam->GetQueueId();
				}

				// ★★ 關鍵：一定要做「深拷貝」(Clone)，否則如果直接 = 會把 ownership 移走
				if (nodeReplace.unptrParam) {
					nodeRollback.unptrParam = nodeReplace.unptrParam->Clone();
				}
				else {
					// 萬一 replace 節點沒有參數？
					nodeRollback.unptrParam.reset();
				}

				// 將 rollback 節點的參數隊列 ID 設回原本的
				if (nodeRollback.unptrParam) {
					nodeRollback.unptrParam->SetQueueId(nQId);
				}
#endif
				// 更新倒退次數
				psNode->UpdateTimes();

				return true;
			}

			//==============================
			// 取得某一步驟的 BaseParameter (以指標形式)
			//==============================
			BaseParameter* GetNodeBaseParameter(const int& nCurrentIndex)
			{
				if (nCurrentIndex > nSumCount) return nullptr;
				auto it = StepIndexMap.find(nCurrentIndex);
				if (it != StepIndexMap.end()) {
					// 改成取 unique_ptr 裡的原生指標
					auto& uptr = vtsProcessNode[it->second].second.unptrParam;
					return (uptr ? uptr.get() : nullptr);
				}
				return nullptr;
			}

			int GetNextStep(const int& nCurrentIndex) const
			{
				if (nCurrentIndex > nSumCount) return -1;
				auto it = StepIndexMap.find(nCurrentIndex);
				if (it != StepIndexMap.end()) {
					return vtsProcessNode[it->second].second.nNextStep;
				}
				else {
					return -1;
				}
			}

			int GetRollbackStep(const int& nCurrentIndex) const
			{
				if (nCurrentIndex > nSumCount) return -1;
				auto it = StepIndexMap.find(nCurrentIndex);
				if (it != StepIndexMap.end()) {
					return vtsProcessNode[it->second].second.nRollbackStep;
				}
				else {
					return -1;
				}
			}

			SProcessNode* GetProcessNode(const int& nCurrentIndex) 
			{
				if (nCurrentIndex < 1 || nCurrentIndex > nSumCount) return nullptr;
				return &vtsProcessNode[nCurrentIndex-1].second;
			}

			bool CreateList(const vector<BaseParameter*>& vtpsBP, const int& nQueueId_Start, const int& nQueueId_End)
			{

				return true;
			}

			bool CreateList()
			{
				nSumCount = 0;
				int nCount_Color = vtsColorParam.size();
				int nCount_Enhance = vtsEnhanceParam.size();
				int nCount_Filter = vtsFilterParam.size();
				int nCount_Threshold = vtsThresholdParam.size();
				int nCount_Morpholog = vtsMorphologParam.size();
				int nCount_Contours = vtsContoursParam.size();
				int nCount_Feature = vtsFeatureParam.size();
				int nCount_DeleteParam = vtsDeleteParam.size();
				int nCount_Calculator = vtsCalculator.size();
				//int nCount_PatternMatch = vtsPatternMatch.size();
				int nCount_Qualification = vtsQualification.size();
				nSumCount = nCount_Color + nCount_Enhance + nCount_Filter + nCount_Threshold + nCount_Morpholog + 
					nCount_Contours + nCount_Feature + nCount_DeleteParam + nCount_Calculator + nCount_Qualification;

				if (nSumCount <= 0)
				{
					strErrorMessage = "沒有任何指令";
					return false;
				}

				int nId = 0;
				vtpsBPList.resize(nSumCount);

				// SColorTransformParam
				for (int k = 0; k < nCount_Color; ++k)
				{
					nId = vtsColorParam[k].GetQueueId() - 1;
					if (nId >= nSumCount)
					{
						return false;
					}
					vtpsBPList[nId] = &vtsColorParam[k];
				}

				// SImageEnhanceParam
				for (int k = 0; k < nCount_Enhance; ++k)
				{
					nId = vtsEnhanceParam[k].GetQueueId() - 1;
					if (nId >= nSumCount)
					{
						return false;
					}
					vtpsBPList[nId] = &vtsEnhanceParam[k];
				}

				// SFilterParam
				for (int k = 0; k < nCount_Filter; ++k)
				{
					nId = vtsFilterParam[k].GetQueueId() - 1;
					if (nId >= nSumCount)
					{
						return false;
					}
					vtpsBPList[nId] = &vtsFilterParam[k];
				}

				// SThresholdParam
				for (int k = 0; k < nCount_Threshold; ++k)
				{
					nId = vtsThresholdParam[k].GetQueueId() - 1;
					if (nId >= nSumCount)
					{
						return false;
					}
					vtpsBPList[nId] = &vtsThresholdParam[k];
				}

				// SMorphologParam
				for (int k = 0; k < nCount_Morpholog; ++k)
				{
					nId = vtsMorphologParam[k].GetQueueId() - 1;
					if (nId >= nSumCount)
					{
						return false;
					}
					vtpsBPList[nId] = &vtsMorphologParam[k];
				}

				// SContoursShapeParam
				for (int k = 0; k < nCount_Contours; ++k)
				{
					nId = vtsContoursParam[k].GetQueueId() - 1;
					if (nId >= nSumCount)
					{
						return false;
					}
					vtpsBPList[nId] = &vtsContoursParam[k];
				}

				// SFeatureAnalyzeParam
				for (int k = 0; k < nCount_Feature; ++k)
				{
					nId = vtsFeatureParam[k].GetQueueId() - 1;
					if (nId >= nSumCount)
					{
						return false;
					}
					vtpsBPList[nId] = &vtsFeatureParam[k];
				}

				// SDeleteObjectParam
				for (int k = 0; k < nCount_DeleteParam; ++k)
				{
					nId = vtsDeleteParam[k].GetQueueId() - 1;
					if (nId >= nSumCount)
					{
						return false;
					}
					vtpsBPList[nId] = &vtsDeleteParam[k];
				}

				// SImageCalculatorParam
				for (int k = 0; k < nCount_Calculator; ++k)
				{
					nId = vtsCalculator[k].GetQueueId() - 1;
					if (nId >= nSumCount)
					{
						return false;
					}
					vtpsBPList[nId] = &vtsCalculator[k];
				}

#if 0
				// SPatternMatchParam
				for (int k = 0; k < nCount_PatternMatch; ++k)
				{
					nId = vtsPatternMatch[k].GetQueueId() - 1;
					if (nId >= nSumCount)
					{
						return false;
					}
					vtpsBPList[nId] = &vtsPatternMatch[k];
				}
#endif

				// SQualificationParam
				for (int k = 0; k < nCount_Qualification; ++k)
				{
					nId = vtsQualification[k].GetQueueId() - 1;
					if (nId >= nSumCount)
					{
						return false;
					}
					vtpsBPList[nId] = &vtsQualification[k];
				}

				return true;
			}

			const vector<BaseParameter*>& GetList() const {return vtpsBPList;}

			// nQueueId 從 1 開始
			BaseParameter* GetBaseParameter(const int nQueueId) const
			{ 
				if (nQueueId < 1 || nQueueId > nSumCount) return nullptr;
				return vtpsBPList[nQueueId-1];
			}

			int GetBaseParameterNumber() const { return nSumCount; }

			string GetErrorMessage() const {return strErrorMessage;}

			void Clear()
			{
				vtsProcessNode.clear();
				StepIndexMap.clear();			
				vtsColorParam.clear();
				vtsEnhanceParam.clear();
				vtsFilterParam.clear();
				vtsThresholdParam.clear();
				vtsMorphologParam.clear();
				vtsContoursParam.clear();
				vtsFeatureParam.clear();
				vtsDeleteParam.clear();
				vtsCalculator.clear();
				//vtsPatternMatch.clear();
				vtsQualification.clear();
				vtpsBPList.clear();
				nSumCount = 0;
				strErrorMessage = "";
			}

			// 取出部分指令建立 sPMP_Part
			bool ExtractList(const int& nQueueId_Start, const int& nQueueId_End, SProcessModeParam& sPMP_Part, vector<pair<SInputType, SInputType>>& vtsReplace = vector<pair<SInputType, SInputType>>())
			{
				if (nSumCount <= 0 || nQueueId_Start < 1 || nQueueId_Start > nQueueId_End || nQueueId_End > nSumCount) {
					return false;
				}

				sPMP_Part.Clear();

				// 定義輔助函數來簡化處理
				auto processParam = [&](auto& param, BaseParameter* pBP) -> bool {
					// 從 param 的型別推斷出 T (例如 SColorTransformParam)
					// 然後把 pBP 轉型為 T*
					using T = typename std::remove_reference<decltype(param)>::type;
					param = *static_cast<T*>(pBP);

					// 下面的呼叫若需要 BaseParameter*，仍然可以用 static_cast
					if (!Offset_BaseParameter(nQueueId_Start, static_cast<BaseParameter*>(&param), vtsReplace)) {
						return false;
					}
					sPMP_Part.Set(param);
					return true;
				};

				for (int k = nQueueId_Start; k <= nQueueId_End; ++k)
				{
					BaseParameter* pBP = GetBaseParameter(k);
					if (pBP == nullptr) return false;

					switch (pBP->GetProcessMode())
					{
					case PROCESS_COLOR: { SColorTransformParam sParam; if (!processParam(sParam, pBP)) return false; break; }
					case PROCESS_ENHANCE: { SImageEnhanceParam sParam; if (!processParam(sParam, pBP)) return false; break; }
					case PROCESS_FILTER: { SFilterParam sParam; if (!processParam(sParam, pBP)) return false; break; }
					case PROCESS_THRESHOLD: { SThresholdParam sParam; if (!processParam(sParam, pBP)) return false; break; }
					case PROCESS_MORPHOLOG: { SMorphologParam sParam; if (!processParam(sParam, pBP)) return false; break; }
					case PROCESS_CONTOUR: { SContoursShapeParam sParam; if (!processParam(sParam, pBP)) return false; break; }
					case PROCESS_DELETE: { SDeleteObjectParam sParam; if (!processParam(sParam, pBP)) return false; break; }
					case PROCESS_CALCULATOR: { SImageCalculatorParam sParam; if (!processParam(sParam, pBP)) return false; break; }
					case PROCESS_FEATURE: { SFeatureAnalyzeParam sParam; if (!processParam(sParam, pBP)) return false; break; }
					case PROCESS_QUALIFICATION: { SQualificationParam sParam; if (!processParam(sParam, pBP)) return false; break; }
					}
				}

				return sPMP_Part.CreateList();
			}

		private :
			bool Offset_BaseParameter(const int& nOffset, BaseParameter* pBP, vector<pair<SInputType, SInputType>>& vtsReplace = vector<pair<SInputType, SInputType>>())
			{
				if (nOffset <= 0 || pBP == nullptr) {
					return false;
				}

				// QueueId
				int nQid = pBP->GetQueueId();
				if (nQid < nOffset) return false;

				// 輔助函數：處理輸入類型的 Offset
				auto processInput = [&](SInputType* pInput) {
					return (pInput && pInput->GetState()) ? Offset_Input(nQid, nOffset, pInput, vtsReplace) : true;
				};

				// 處理所有需要偏移的輸入
				if (!(processInput(pBP->GetInput1()) &&
					processInput(pBP->GetInput2()) &&
					processInput(pBP->GetContours1()) &&
					processInput(pBP->GetContours2()) &&
					processInput(pBP->GetMask1()) &&
					processInput(pBP->GetMask2()) &&
					processInput(pBP->GetROI1_TypeId()) &&
					processInput(pBP->GetROI2_TypeId()))) {
					return false;
				}

				// 更新 QueueId
				pBP->SetQueueId(nQid - nOffset + 1);

				return true;
			}

			bool Offset_Input(const int& QueueId, const int& nOffset, SInputType* pInput, vector<pair<SInputType, SInputType>>& vtsReplace = vector<pair<SInputType, SInputType>>())
			{
				if (QueueId < nOffset || nOffset <= 0 || pInput == nullptr) {
					return false;
				}

				int nCount_Replace = vtsReplace.size();
				int nSrcType = pInput->GetType();
				int nId = pInput->GetId();
				if (QueueId == nOffset) {
					pInput->SetType(1);
					pInput->SetId(1);
				}
				else {
					bool bGo = true;
					for (int k = 0; k < nCount_Replace; ++k) {
						if (nSrcType == vtsReplace[k].first.GetType() && nId == vtsReplace[k].first.GetId()) {
							bGo = false;
							pInput->SetType(vtsReplace[k].second.GetType());
							pInput->SetId(vtsReplace[k].second.GetId());
							k = nCount_Replace;
						}
					}

					if(bGo) {
						switch (nSrcType)
						{
						case 0:
							if (nId == -1) {
								pInput->SetType(0);
								pInput->SetId(-1);
							}
							break;
						case 1:
							pInput->SetType(1);
							pInput->SetId(nId);
							break;
						case 2:
							pInput->SetType(2);
							pInput->SetId(nId - nOffset + 1);
							break;
						default:
							return false;
						}
					}
				}
				return true;
			}
		};
	}
}
