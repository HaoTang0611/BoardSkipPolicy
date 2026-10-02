#pragma once

#include <vector>
#include <string>
#include "ConfigFile.h"

namespace JET {
	namespace alg {
		using namespace std;

#pragma region Base Struct

		// HSV抽色
		struct SHsvExtraction
		{
			// 預設是 10
			int nMean_H;

			// 預設是 10
			int nRange_H;

			// 預設都是 10
			int nMin_S;
			int nMin_V;

			// 預設都是 200
			int nMax_S;
			int nMax_V;

			SHsvExtraction() : nMean_H(10), nMin_S(10), nMin_V(10), nRange_H(10), nMax_S(200), nMax_V(200) {}
			~SHsvExtraction() = default;

			bool Check(string& strInfo) const
			{
				if (nMean_H < 0 || nMean_H > 180) {
					strInfo = "Min H值設定錯誤";
					return false;
				}

				if (nRange_H < 0 || nRange_H > 180) {
					strInfo = "Max H值設定錯誤";
					return false;
				}

				if (nMin_S < 0 || nMin_S > 255) {
					strInfo = "Min S值設定錯誤";
					return false;
				}

				if (nMax_S < 0 || nMax_S > 255) {
					strInfo = "Max S值設定錯誤";
					return false;
				}

				if (nMin_V < 0 || nMin_V > 255) {
					strInfo = "Min V值設定錯誤";
					return false;
				}

				if (nMax_V < 0 || nMax_V > 255) {
					strInfo = "Max V值範圍設定錯誤";
					return false;
				}

				return true;
			}
		};

		// RGB抽色
		struct SColorExtraction
		{
			// 預設都是 10
			int nValue_R;
			int nValue_G;
			int nValue_B;

			// 預設都是 20
			int nRange;

			SColorExtraction() : nValue_R(10), nValue_G(10), nValue_B(10), nRange(20) {}

			~SColorExtraction() = default;

			bool Check(string& strInfo) const
			{
				if (nValue_R < 0 || nValue_R > 255) {
					strInfo = "R值設定錯誤";
					return false;
				}

				if (nValue_G < 0 || nValue_G > 255) {
					strInfo = "G值設定錯誤";
					return false;
				}

				if (nValue_B < 0 || nValue_B > 255) {
					strInfo = "B值設定錯誤";
					return false;
				}

				if (nRange < 0 || nRange > 255) {
					strInfo = "值範圍設定錯誤";
					return false;
				}

				return true;
			}
		};

		// 灰階抽色
		struct SGrayExtraction
		{
			int nValue_Gray;
			int nRange;

			SGrayExtraction() : nValue_Gray(200), nRange(20) {}
			~SGrayExtraction() = default;

			bool Check(string& strInfo) const
			{
				if (nValue_Gray < 0 || nValue_Gray > 255) {
					strInfo = "灰階值設定錯誤";
					return false;
				}

				if (nRange < 0 || nRange > 255) {
					strInfo = "值範圍設定錯誤";
					return false;
				}

				return true;
			}
		};

		struct SKmean {

			// 組數, 預設為2, 不可小於2 
			int nGroupNumber;

			// 預設為 0
			int nDarkId;

			// 預設為 1
			int nLightId;

			SKmean() : nGroupNumber(2), nDarkId(0), nLightId(1) {}
			~SKmean() = default;

			bool Check(string& strInfo) const
			{
				if (nGroupNumber < 2) {
					strInfo = "GroupNumber less than 2";
					return false;
				}

				if (nDarkId < 0) {
					strInfo = "DarkId less than 0";
					return false;
				}

				if (nLightId >= nGroupNumber) {
					strInfo = "LightId greater than or equal GroupNumber";
					return false;
				}

				if (nDarkId >= nLightId) {
					strInfo = "DarkId greater than or equal nLightId";
					return false;
				}
				return true;
			}
		};

		// 強化模式
		struct SEnhanceMode {

			// 預設為 0(不處理)
			// 影像強化模式, 0=>不處理, 1=>Log, 2=>Kmean
			int nMode;

			SKmean sKmean;

			SEnhanceMode() : nMode(0), sKmean() {}
			~SEnhanceMode() = default;

			bool Check(string& strInfo) const
			{
				if (nMode < 0 || nMode > 3) {
					strInfo = "Mode is error";
					return false;
				}

				string strData;
				if (!sKmean.Check(strData)) {
					strInfo = "KMean " + strData;
					return false;
				}
				return true;
			}
		};

		// 刪除模式用 ROI
		struct SDeleteROI {

			// 預設為 0.3 值域 (0 , 1.0]
			// 膠本體大小的寬放係數
			// 膠本體大小的判斷範圍 = [GlueWidth*(1-fTolerance) , GlueWidth*(1+fTolerance))] ; [GlueHeight*(1-fTolerance) , GlueHeight*(1+fTolerance))]
			float fROIW_Tolerance;

			float fROIH_Tolerance;

			// ROI 數量
			int nCount;

			// 每個 ROI 的座標, vtrectROI.size() 必須等於 nCount
			vector<RECT> vtrectROI;

			SDeleteROI() : fROIW_Tolerance(0.3), fROIH_Tolerance(0.3), nCount(0) {}
			~SDeleteROI() = default;

			bool SetCount(const int nC) {
				if (nC <= 0) return false;
				nCount = nC;
				vtrectROI.resize(nC);
				return true;
			}

			// nId : 從0開始
			bool SetROI(const int nId, const int nL, const int nT, const int nR, const int nB) {
				if (nId < 0 || nId >= nCount || nCount != vtrectROI.size()) return false;
				vtrectROI[nId].left = nL;
				vtrectROI[nId].top = nT;
				vtrectROI[nId].right = nR;
				vtrectROI[nId].bottom = nB;
				return true;
			}

			bool Check(string& strInfo) const
			{
				if (fROIW_Tolerance <= 0.0 || fROIW_Tolerance >= 1.0) {
					strInfo = "ROI寬度的寬放比例設定錯誤";
					return false;
				}

				if (fROIH_Tolerance <= 0.0 || fROIH_Tolerance >= 1.0) {
					strInfo = "ROI高度的寬放比例設定錯誤";
					return false;
				}

				if (nCount > 0) {
					if (nCount != vtrectROI.size()) {
						strInfo = "ROI數量設定錯誤";
						return false;
					}

					for (int k = 0; k < nCount; ++k) {
						if (vtrectROI[k].left < 0 || vtrectROI[k].left >= vtrectROI[k].right || vtrectROI[k].top < 0 || vtrectROI[k].top >= vtrectROI[k].bottom) {
							strInfo = "No." + to_string(k + 1) + " ROI coordinate is error";
							return false;
						}
					}
				}
				return true;
			}

			bool Check(const int& nImageH, const int& nImageW, string& strInfo) const
			{
				if (nImageH <= 0 || nImageW <= 0) {
					strInfo = "Image Width or Height is error";
					return false;
				}

				if (fROIW_Tolerance <= 0.0 || fROIW_Tolerance >= 1.0) {
					strInfo = "ROI寬度的寬放比例設定錯誤";
					return false;
				}

				if (fROIH_Tolerance <= 0.0 || fROIH_Tolerance >= 1.0) {
					strInfo = "ROI高度的寬放比例設定錯誤";
					return false;
				}

				if (nCount > 0) {
					if (nCount != vtrectROI.size()) {
						strInfo = "ROI數量設定錯誤";
						return false;
					}

					for (int k = 0; k < nCount; ++k) {
						if (vtrectROI[k].left < 0 || vtrectROI[k].left >= vtrectROI[k].right || vtrectROI[k].top < 0 || vtrectROI[k].top >= vtrectROI[k].bottom ||
							vtrectROI[k].right >= nImageW || vtrectROI[k].bottom >= nImageH) {
							strInfo = "No." + to_string(k + 1) + " ROI coordinate is error";
							return false;
						}
					}
				}

				return true;
			}
		};

		// 刪除模式
		struct SDeleteMode
		{
			// 設為 1 (0代表不使用)
			// 物件刪除模式, 1=>取最大面積, 2=>任一ROI與指定物件的距離大於設定值,即刪除, 3=>保留全部物件, 4=>保留指定大小物件
			int nMode;

			// 預設為 1, Mode為1時要設定
			int nMaxCount;

			// 基準方向, 預設是 1, nMode=2時要設定
			//     1
			// 3       4
			//     2
			int nDirection;

			// 預設 0, Mode為4時要設定
			// ROI 目標 寬度
			int nROI_Width_Std;
			int nROI_Width_Range;

			// 預設 0, Mode為4時要設定
			// ROI 目標 高度
			int nROI_Height_Std;
			int nROI_Height_Range;

			// Mode為3 4 時要設定
			SDeleteROI sROI;

			SDeleteMode() : nMode(1), nMaxCount(1), nDirection(1), nROI_Width_Std(0), nROI_Width_Range(0), nROI_Height_Std(0), nROI_Height_Range(0), sROI() {}
			~SDeleteMode() = default;

			bool Check(string& strInfo) const
			{
				if (nMode < 0 || nMode > 4) {
					strInfo = "Mode is error";
					return false;
				}

				if (nMode == 1 && nMaxCount < 1) {
					strInfo = "MaxCount is error";
					return false;
				}

				if (nMode == 2 && (nDirection < 1 || nDirection > 4)) {
					strInfo = "Direction is error";
					return false;
				}

				if (nMode == 3 && !sROI.Check(strInfo)) {
					return false;
				}

				if (nMode == 4) {
					if (nROI_Width_Std <= 0) {
						strInfo = "ROI目標寬度設定錯誤";
						return false;
					}

					if (nROI_Width_Range < 0) {
						strInfo = "ROI目標寬度範圍設定錯誤";
						return false;
					}

					if (nROI_Height_Std <= 0) {
						strInfo = "ROI目標高度設定錯誤";
						return false;
					}

					if (nROI_Height_Range < 0) {
						strInfo = "ROI目標高度範圍設定錯誤";
						return false;
					}
				}

				return true;
			}
		};

		// 找邊界參數
		struct SFindEdge_Parameter
		{
			// 預設為 1
			// 1=>彩色, 2=>取R, 3=>取G, 4=>取B, 5=>(R+G+B)/3, 6=>正投影, 7=>負投影, 8=>雙邊正投影, 9=>雙邊負投影
			int nColorTransformMode;

			// 預設為 1
			// 濾波模式, 0=>不處理, 1=>中值, 2=>均值, 3=>最大值, 4=>最小值, 5=>最大最小, 6=>最小最大
			int nFilterMode;

			// 預設為 1 pixels
			// 過濾影像雜訊
			int nMedianSize;

			// 預設為 1 (RGB)
			// 彩色影像二值化模式, 1=>RGB, 2=>HSV
			int nColorThresholdMode;

			// 預設為 0(不處理)
			// 影像強化模式, 0=>不處理, 1=>Log, 2=>Kmean
			SEnhanceMode sEnhanceMode;

			// 刪除模式, 預設為 0
			// 物件刪除模式, 0=>不處理, 1=>取最大面積, 2=>任一ROI與指定物件的距離大於設定值,即刪除, 3=>保留指定大小物件
			SDeleteMode sDeleteMode;

			// HSV抽色組數, 預設為 1
			int nCount_HsvExtraction;
			vector<SHsvExtraction> vtsHsvExtraction;

			// 彩色抽色組數, 預設為 1
			int nCount_ColorExtraction;
			vector<SColorExtraction> vtsColorExtraction;

			// 灰階抽色組數, 預設為 1
			int nCount_GrayExtraction;
			vector<SGrayExtraction> vtsGrayExtraction;

			// 預設都為 1 pixels
			// 二值化影像過濾雜訊
			int nOpenX;
			int nOpenY;

			// 預設都為 1 pixels
			// 二值化影像連接
			int nConnectX;
			int nConnectY;

			// 預設都為 1 pixels (1代表不執行), 不可小於1, 最好為奇數
			// 膠邊界的膨脹濾波, 將膠邊界外擴, 單位為 pixels
			int nDilateX;
			int nDilateY;

			// 預設都為 21 pixels (1代表不執行), 不可小於1, 最好為奇數
			// 膠邊界的侵蝕濾波, 將膠邊界內縮, 單位為 pixels
			int nErosionX;
			int nErosionY;

			SFindEdge_Parameter() : nColorTransformMode(1), nFilterMode(1), nMedianSize(1), sEnhanceMode(), sDeleteMode(),
				nCount_HsvExtraction(1), nCount_ColorExtraction(1), nCount_GrayExtraction(1), nOpenX(1), nOpenY(1), 
				nColorThresholdMode(1), nConnectX(1), nConnectY(1), nDilateX(1), nDilateY(1), nErosionX(1), nErosionY(1)
			{
				vtsHsvExtraction.resize(nCount_HsvExtraction);
				vtsColorExtraction.resize(nCount_ColorExtraction);
				vtsGrayExtraction.resize(nCount_GrayExtraction);
			}

			bool Check(string& strInfo) const
			{
				if (nColorTransformMode < 1 || nColorTransformMode > 11) {
					strInfo = "色彩轉換模式設定錯誤";
					return false;
				}

				if (nFilterMode < 1 || nFilterMode > 6) {
					strInfo = "濾波模式設定錯誤";
					return false;
				}

				string strData;
				if (!sEnhanceMode.Check(strData)) {
					strInfo = "EnhanceMode " + strData;
					return false;
				}

				if (!sDeleteMode.Check(strData)) {
					strInfo = "DeleteMode " + strData;
					return false;
				}

				if (nMedianSize < 1) {
					strInfo = "MedianSize 設定錯誤";
					return false;
				}

				if (nColorTransformMode == 1) {
					if (nCount_ColorExtraction < 1 || nCount_ColorExtraction != vtsColorExtraction.size()) {
						strInfo = "彩色抽色組數設定錯誤";
						return false;
					}
					else {
						string strData;
						for (int k = 0; k < nCount_ColorExtraction; ++k) {
							if (!vtsColorExtraction[k].Check(strData)) {
								strInfo = "第" + to_string(k + 1) + "組彩色抽色參數_" + strData;
								return false;
							}
						}
					}
				}
				else {
					if (nCount_GrayExtraction < 1 || nCount_GrayExtraction != vtsGrayExtraction.size()) {
						strInfo = "灰階抽色組數設定錯誤";
						return false;
					}
					else {
						string strData;
						for (int k = 0; k < nCount_GrayExtraction; ++k) {
							if (!vtsGrayExtraction[k].Check(strData)) {
								strInfo = "第" + to_string(k + 1) + "組灰階抽色參數_" + strData;
								return false;
							}
						}
					}
				}

				if (nOpenX < 1) {
					strInfo = "二值化過濾雜訊濾波 OpenX 設定錯誤";
					return false;
				}

				if (nOpenY < 1) {
					strInfo = "二值化過濾雜訊濾波 OpenY 設定錯誤";
					return false;
				}

				if (nConnectX < 1) {
					strInfo = "二值化連接濾波 ConnectX 設定錯誤";
					return false;
				}

				if (nConnectY < 1) {
					strInfo = "二值化連接濾波 ConnectY 設定錯誤";
					return false;
				}

				if (nDilateX < 1) {
					strInfo = "邊界調整濾波 DilateX 設定錯誤";
					return false;
				}

				if (nDilateY < 1) {
					strInfo = "邊界調整濾波 DilateY 設定錯誤";
					return false;
				}

				if (nErosionX < 1) {
					strInfo = "邊界調整濾波 ErosionX 設定錯誤";
					return false;
				}

				if (nErosionY < 1) {
					strInfo = "邊界調整濾波 ErosionY 設定錯誤";
					return false;
				}

				return true;
			}
		};

		// 找膠邊結果
		struct SFindEdge_Result
		{
			// 預設為 0
			// 膠本體 ROI
			RECT  rectROI;

			// 預設為 0
			// 輪廓形成的面積(Pixels數量)
			int nPixelArea;

			// 預設為 0
			// 輪廓的寬度(Pixels)
			int nWidth;

			// 預設為 0
			// 輪廓的高度(Pixels)
			int nHeight;

			// 預設為 0
			// 輪廓點數
			int nContoursCount;

			// 膠的輪廓點
			// vtptContoursPos.size()必須與 nContoursCount 相同
			vector<POINT>  vtptContoursPos;

			SFindEdge_Result() : nPixelArea(0), nWidth(0), nHeight(0), nContoursCount(0) {
				rectROI.left = 0;
				rectROI.top = 0;
				rectROI.right = 0;
				rectROI.bottom = 0;
			}

			void Initial()
			{
				rectROI.left = 0;
				rectROI.top = 0;
				rectROI.right = 0;
				rectROI.bottom = 0;
				nPixelArea = 0;
				nWidth = 0;
				nHeight = 0;
				nContoursCount = 0;
				vtptContoursPos.clear();
			}
		};
#pragma endregion

#pragma region Save Load

		bool FindEdgeParameterToConfig(const string& strAppName, const SFindEdge_Parameter& sParam, vector<ConfigData>& vtsConfig);

		bool FindEdgeConfigToParameter(const string& strAppName, const std::vector<ConfigData>& vtsConfig, SFindEdge_Parameter& sParam);
	
#pragma endregion
	}
}