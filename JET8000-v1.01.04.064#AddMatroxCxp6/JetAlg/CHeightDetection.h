//Author : Jun
#pragma once
#include "JETAlg_Std.h"
#include <windows.h>
#include <string>
#include <vector>
#include <memory>

using namespace std;

namespace JET {
	namespace alg {

		// 錫高檢測的輸入參數
		struct STinInputParam
		{
			// 錫高輸出方式: (預設為 1)
			// 當 nMeasureLength > 1  時, 會計算最大的斜率差
			// 當 nMeasureLength == 1 時, 會計算 nShift_Tin 位置的平均高度
			// nMeasureLength 不可小於 1
			int nOutputType;

			// 錫(膠)的方向, 1=垂直, 2=水平, 3=垂直水平都有, 4=零件的四個角, 5=垂直水平+四個角
			int nDirection;

			// 預設為 15
			// 錫高量測的計算長度(pixels), 不可小於 1
			int	nMeasureLength;

			// 預設為 2
			// 錫高度量測的面積範圍(pixels), 不可小於 1
			int nMeasureRange;

			// 預設為 1
			// 錫高量測起始位置 = 3D定位後再往外偏移 nShift_Tin (pixels), 不可小於 1
			int nShift_Tin;

			// Pattern的寬度(pixels)
			int nPatternWidth;

			// Pattern的高度(pixels)
			int nPatternHeight;

			// 高度偵測的移動距離, 預設為 50(um)
			int nStepZ;

			// 零件寬度的高度差允收值, 預設為 2
			int nThresholdZ_Width;

			// 零件高度的高度差允收值, 預設為 2
			int nThresholdZ_Height;

			STinInputParam()
			{
				nOutputType = 1;

				// 1=垂直, 2=水平, 3=垂直水平都有, 4=零件的四個角, 5=垂直水平+四個角
				nDirection = 1;

				// 錫高量測的長度
				nMeasureLength = 15;

				// 計算高度的範圍(pixels)
				nMeasureRange = 2;

				// 3D定位後再偏移
				nShift_Tin = 1;

				nPatternWidth = 0;

				nPatternHeight = 0;

				//  以 nStepZ 當作高度間隔
				nStepZ = 50;

				nThresholdZ_Width = 2;

				nThresholdZ_Height = 2;
			}

			~STinInputParam()
			{

			}
		};

		// 灰階濾波參數
		struct SGrayFilterParam
		{
			// 預設為 0
			// 灰階影像濾波模式 : 0, 不處理, 
			//                  : 1, 中值濾波, 
			//					: 2, 最小值濾波
			//					: 3, 最大值濾波
			//				    : 4, 先最小值再最大值
			//					: 5, 先最大值再最小值
			//					: 6, 4 + 5
			//					: 7, 5 + 4
			//					: 8, 梯度
			int nMode;

			// X方向濾波大小, 預設為 0
			int nSizeX;

			// Y方向濾波大小, 預設為 0
			int nSizeY;

			SGrayFilterParam()
			{
				nMode = 0;
				nSizeX = 0;
				nSizeY = 0;
			}
		};

		// 處理模式
		struct SProcessMode
		{
			// 預設為 0
			// 影像Index : 1, RGB影像
			//           : 2, 低角度白光
			//           : 3, 高角度白光
			int nImage_Index;

			// 預設為 0
			// 彩色轉灰階的模式 : 1, 以B分量
			//                  : 2, 以G分量
			//                  : 3, 以R分量
			//                  : 4, (R+G+B)/3
			int nGrayMode;

			// 預設為 0
			// 影像的增強模式 : 1, 用Log 進行影像增強
			//				  : 2, 用直方圖均化進行影像增強
			int nEnhanMode;

			// 預設為 false
			// 二值化值域選擇, 須與 nThresholdMode 搭配
			bool bThreshold_Dark;

			// 預設為 0
			// 二值化模式 : 1, 單閥值, bThreshold_Dark = true  ; 值域 [0 , nThreshold_Low]
			//                         bThreshold_Dark = false ; 值域 [nThreshold_Low, 255]
			//            : 2, 雙閥值, bThreshold_Dark = true  ; 值域 [nThreshold_Low , nThreshold_High]
			//            :            bThreshold_Dark = false ; 值域 [0 , nThreshold_Low] + [nThreshold_High , 255]
			//            : 3, 自動閥值, Otsu,            bThreshold_Dark = true  ; 值域 [0 , Otsu]
			//											  bThreshold_Dark = false ; 值域 [Otsu , 255]
			//            : 4, 自動閥值, Triangle,        bThreshold_Dark = true  ; 值域 [0 , Triangle]
			//                                            bThreshold_Dark = false ; 值域 [Triangle , 255]
			//            : 5, 自動閥值, MaxEntropy,      bThreshold_Dark = true  ; 值域 [0 , MaxEntropy]
			//                                            bThreshold_Dark = false ; 值域 [MaxEntropy , 255]
			//            : 6, 自動閥值, MinCrossEntropy, bThreshold_Dark = true  ; 值域 [0 , MinCrossEntropy]
			//            :                               bThreshold_Dark = false ; 值域 [MaxEntropy , 255]
			int nThresholdMode;

			// 預設為 0
			// 二值化低閥值, 
			int nThreshold_Low;

			// 預設為 0
			// 二值化高閥值, 當 nThresholdMode為2時 才會用到
			int nThreshold_High;

			// 預設為 0
			// 型態學模式 : 1, 侵蝕, 
			//            : 2, 膨脹
			//            : 3, Opening(先侵蝕再膨脹)
			//            : 4, Closing(先膨脹再侵蝕)
			//            : 5, 先 Opening 再 Closing
			//            : 6, 先 Closing 再 Opening
			int nMorphologyMode;

			// 預設為 0
			// 型態學濾波大小(須為奇數), 
			int nMorphologySize;

			SProcessMode()
			{
				nImage_Index = 0;
				nGrayMode = 0;
				nEnhanMode = 0;
				nThresholdMode = 0;
				bThreshold_Dark = false;
				nThreshold_Low = 0;
				nThreshold_High = 0;
				nMorphologyMode = 0;
				nMorphologySize = 0;
			}

			~SProcessMode()
			{

			}
		};

		// 膠高檢測的輸入參數
		struct SGlueInputParam
		{
			// IC的寬度--水平方向(pixels)
			int nIC_Width;

			// IC的長度--垂直方向(pixels)
			int nIC_Length;

			// 溢膠的最小寬度--水平方向(pixels)
			int nMinSpill_Width;

			// 溢膠最小長度--垂直方向(pixels)
			int nMinSpill_Length;

			// 膠的量測範圍(IC邊界的外擴距離)(pixels)
			int nMeasureRange;

			// 高度偵測模式, 0=>不偵測高度, 
			//             , 1=>用2D影像計算膠的範圍,再用最大梯度計算膠高的區域 
			//             , 2=>用2D RGB光影像中的藍色膠位置當做計算膠高的區域 
			//             , 3=>用3D影像定位計算膠高的區域 
			int nHeightMode;

			// 膠邊界偵測模式, 0=>不偵測高度, 
			//               , 1=>用2值化偵測
			int nGlueMode;

			// 溢膠檢測模式, 0=>不檢測溢膠, 
			//             , 1=>用2值化檢測
			int nSpillMode;

			// IC定位的處理模式
			SProcessMode sProcess_IC;

			// 膠長量測的處理模式
			SProcessMode sProcess_Glue;

			// 溢膠檢測的處理模式
			SProcessMode sProcess_Spill;

			SGlueInputParam()
			{
				// 以力成模式初始化
				Initial(1);
			}

			~SGlueInputParam()
			{
				
			}

			// 
			//  nType : 0 => 無
		    //        : 1 => 力成
			//        : 2 => 矽品
			void Initial(const int& nType)
			{
				switch (nType)
				{
				case 1:
#pragma region 力成
					nIC_Length = 732;
					nIC_Width = 758;
					nMinSpill_Length = 10;
					nMinSpill_Width = 10;
					nMeasureRange = 120;

					nHeightMode = 1;
					nGlueMode = 1;
					nSpillMode = 1;

					// IC
					sProcess_IC.nImage_Index = 2;
					sProcess_IC.nGrayMode = 4;
					sProcess_IC.nEnhanMode = 1;
					sProcess_IC.nThresholdMode = 1;
					sProcess_IC.bThreshold_Dark = true;
					sProcess_IC.nThreshold_Low = 70;
					sProcess_IC.nMorphologyMode = 0;
					sProcess_IC.nMorphologySize = 0;

					// Glue
					sProcess_Glue.nImage_Index = 3;
					sProcess_Glue.nGrayMode = 4;
					sProcess_Glue.nEnhanMode = 1;
					sProcess_Glue.nThresholdMode = 2;
					sProcess_Glue.bThreshold_Dark = true;
					sProcess_Glue.nThreshold_Low = 1;
					sProcess_Glue.nThreshold_High = 100;
					sProcess_Glue.nMorphologyMode = 6;
					sProcess_Glue.nMorphologySize = 3;

					// Spill
					sProcess_Spill.nImage_Index = 2;
					sProcess_Spill.nGrayMode = 4;
					sProcess_Spill.nEnhanMode = 1;
					sProcess_Spill.nThresholdMode = 1;
					sProcess_Spill.bThreshold_Dark = false;
					sProcess_Spill.nThreshold_Low = 70;
					sProcess_Spill.nThreshold_High = 0;
					sProcess_Spill.nMorphologyMode = 0;
					sProcess_Spill.nMorphologySize = 0;
#pragma endregion
					break;

				case 2:
#pragma region 矽品
					nIC_Length = 3664;
					nIC_Width = 2562;
					nMinSpill_Length = 30;
					nMinSpill_Width = 30;
					nMeasureRange = 120;

					nHeightMode = 1;
					nGlueMode = 1;
					nSpillMode = 1;

					// IC
					sProcess_IC.nImage_Index = 2;
					sProcess_IC.nGrayMode = 4;
					sProcess_IC.nEnhanMode = 2;
					sProcess_IC.nThresholdMode = 1;
					sProcess_IC.bThreshold_Dark = true;
					sProcess_IC.nThreshold_Low = 170;
					sProcess_IC.nMorphologyMode = 0;
					sProcess_IC.nMorphologySize = 0;

					// Glue
					sProcess_Glue.nImage_Index = 3;
					sProcess_Glue.nGrayMode = 4;
					sProcess_Glue.nEnhanMode = 1;
					sProcess_Glue.nThresholdMode = 2;
					sProcess_Glue.bThreshold_Dark = true;
					sProcess_Glue.nThreshold_Low = 1;
					sProcess_Glue.nThreshold_High = 100;
					sProcess_Glue.nMorphologyMode = 6;
					sProcess_Glue.nMorphologySize = 7;

					// Spill
					sProcess_Spill.nImage_Index = 2;
					sProcess_Spill.nGrayMode = 4;
					sProcess_Spill.nEnhanMode = 1;
					sProcess_Spill.nThresholdMode = 1;
					sProcess_Spill.bThreshold_Dark = false;
					sProcess_Spill.nThreshold_Low = 110;
					sProcess_Spill.nThreshold_High = 0;
					sProcess_Spill.nMorphologyMode = 0;
					sProcess_Spill.nMorphologySize = 0;
#pragma endregion
					break;

				default:
#pragma region 其餘
					nIC_Width = 0;
					nIC_Length = 0;
					nMeasureRange = 0;
					nHeightMode = 0;
					nGlueMode = 0;
					nSpillMode = 0;
					nMinSpill_Width = 0;
					nMinSpill_Length = 0;

					sProcess_IC.nImage_Index = 0;
					sProcess_IC.nGrayMode = 0;
					sProcess_IC.nEnhanMode = 0;
					sProcess_IC.nThresholdMode = 0;
					sProcess_IC.bThreshold_Dark = false;
					sProcess_IC.nThreshold_Low = 0;
					sProcess_IC.nThreshold_High = 0;

					sProcess_Glue.nImage_Index = 0;
					sProcess_Glue.nGrayMode = 0;
					sProcess_Glue.nEnhanMode = 0;
					sProcess_Glue.nThresholdMode = 0;
					sProcess_Glue.bThreshold_Dark = false;
					sProcess_Glue.nThreshold_Low = 0;
					sProcess_Glue.nThreshold_High = 0;

					sProcess_Spill.nImage_Index = 0;
					sProcess_Spill.nGrayMode = 0;
					sProcess_Spill.nEnhanMode = 0;
					sProcess_Spill.nThresholdMode = 0;
					sProcess_Spill.bThreshold_Dark = false;
					sProcess_Spill.nThreshold_Low = 0;
					sProcess_Spill.nThreshold_High = 0;
#pragma endregion
					break;
				}
			}
		};

		// 錫高檢測輸出結果
		struct STinOutputResult
		{
			// 零件的旋轉角度
			float fAngle;

			// 零件的旋轉弧度
			float fRadian;

			// 零件跟錫的高度差(um) 
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<float> vtfHeightDifference;

			// 零件跟錫的高度差百分比 = 錫高 / 零件高
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<float> vtfHeightPercent;

			// 零件高度(um)
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<float> vtfPartHeight;

			// 錫高度(um)
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<float> vtfTinHeight;

			STinOutputResult()
			{
				Initial();
			}

			~STinOutputResult()
			{
				Release();
			}

			void Initial()
			{
				fAngle = 0.0;
				fRadian = 0.0;
				
				Release();
				vtfHeightDifference.resize(4, 0.0);		
				vtfHeightPercent.resize(4, 0.0);
				vtfPartHeight.resize(4, 0.0);
				vtfTinHeight.resize(4, 0.0);
			}

			void Release()
			{
				vtfHeightDifference.clear();
				vtfHeightPercent.clear();
				vtfPartHeight.clear();
				vtfTinHeight.clear();
			}
		};

		// 膠高檢測輸出結果
		class SGlueOutputResult
		{
		public:
			// 膠的長度(pixels)
			vector<int> vtnGlue_MaxLength;
			vector<int> vtnGlue_MinLength;
			vector<pair<POINT,POINT>> vtptMinLength;
			vector<pair<POINT,POINT>> vtptMaxLength;

			// 膠的高度(um)
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<float> vtfHeight_Glue;

			// IC高度(um)
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<float> vtfHeight_IC;

			// IC跟膠的高度差(um) 
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<float> vtfHeight_Diff;

			// IC跟膠的高度差百分比 = 膠高 / IC高
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<float> vtfHeight_Percent;

			// 溢膠數量
			int nCount_GlueSpill;

			// 溢膠的區域
			vector<cv::Rect> vtGlueSpillROI;

			std::shared_ptr<std::vector<cv::Rect>> val;

			SGlueOutputResult()
			{
				Initial();
			}

			~SGlueOutputResult()
			{
				Release();
			}

			void Initial()
			{
				pair<POINT,POINT> ptDefual;
				ptDefual.first.x = -1;
				ptDefual.first.y = -1;
				ptDefual.second.x = -1;
				ptDefual.second.y = -1;

				Release();
				vtnGlue_MaxLength.resize(4, 0);
				vtnGlue_MinLength.resize(4, 0);
				vtptMinLength.resize(4, ptDefual);
				vtptMaxLength.resize(4, ptDefual);

				vtfHeight_Glue.resize(4, 0.0);
				vtfHeight_IC.resize(4, 0.0);
				vtfHeight_Diff.resize(4, 0.0);
				vtfHeight_Percent.resize(4, 0.0);

				nCount_GlueSpill = 0;
				vtGlueSpillROI.resize(30);
				val = std::make_shared<std::vector<cv::Rect>>();
			}

			void Release()
			{
				vtnGlue_MaxLength.clear();
				vtnGlue_MinLength.clear();
				vtptMinLength.clear();
				vtptMaxLength.clear();
				vtfHeight_Glue.clear();
				vtfHeight_IC.clear();
				vtfHeight_Diff.clear();
				vtfHeight_Percent.clear();
				vtGlueSpillROI.clear();
			}
		};

		struct SHeightDetectionParam
		{
			// 檢測類型, 1=錫高, 2=膠高
			int nInspectionType;

			// 錫高檢測輸入變數
			STinInputParam sTinParam;

			// 膠高檢測輸入變數
			SGlueInputParam sGlueParam;

			SHeightDetectionParam()
			{
				nInspectionType = 1;
			}

			~SHeightDetectionParam()
			{

			}
		};

		// 高度量測輸出結果
		struct SHeightDetectionResult
		{
			// 錫高檢測輸出結果
			STinOutputResult sTinResult;

			// 膠高檢測輸出結果
			SGlueOutputResult sGlueResult;

			SHeightDetectionResult()
			{

			}

			~SHeightDetectionResult()
			{

			}
		};

		// 自動生成零件框的輸入參數
		struct SAutoCreateROIParam
		{
			// 預設為 1
			// 零件類型, 1=IC類(目前只有 1)
			int nType;

			// 預設為 0
			// 引腳方向, 1=垂直, 2=水平, 3=垂直水平都有
			int nDirection;

			// 預設為 0
			// 影像高 = vtpu8Image的高度
			int nImageH;

			// 預設為 0
			// 影像寬 = vtpu8Image的寬度
			int nImageW;

			// 20230830 零件本體範圍
			RECT rectPartRange;

			// 3D高度(目前無作用, 不用給值)
			float* pf3D;

			// vtpu8Image size 預設為 3
			// vtpu8Image[0] = RGB光彩色影像(檢測框影像)
			// vtpu8Image[1] = 低角度白光彩色影像(檢測框影像)
			// vtpu8Image[2] = 高角度白光彩色影像(檢測框影像)
			vector<BYTE*> vtpu8Image;

			SAutoCreateROIParam()
			{
				nType = 1;
				nDirection = 0;
				nImageH = 0;
				nImageW = 0;
				rectPartRange.top = 0;
				rectPartRange.bottom = 0;
				rectPartRange.left = 0;
				rectPartRange.right = 0;
				pf3D = nullptr;
				vtpu8Image.resize(3);
				vtpu8Image[0] = nullptr;
				vtpu8Image[1] = nullptr;
				vtpu8Image[2] = nullptr;
			}
		};

		// 自動生成零件框的結果
		struct SAutoCreateROIResult
		{
			// 輸出的座標以 SAutoCreateROIParam.rectPartROI 左上角為原點
			// 零件本體框
			RECT rtPart;

			// 紀錄焊盤框的數量, size 預設為 4, 定義如下:
			// vtnCount_Pad[0] = 焊盤在本體上方的數量
			// vtnCount_Pad[1] = 焊盤在本體下方的數量
			// vtnCount_Pad[2] = 焊盤在本體左邊的數量
			// vtnCount_Pad[3] = 焊盤在本體右邊的數量
			vector<int> vtnCount_Pad;

			// 紀錄焊盤框位置, size 預設為4個方向 每個方向預設100個, 每個方向的實際數量請參照 "vtnCount_Pad"
			// vt2rtPad[0] = 焊盤在本體上方的位置
			// vt2rtPad[1] = 焊盤在本體下方的位置
			// vt2rtPad[2] = 焊盤在本體左邊的位置
			// vt2rtPad[3] = 焊盤在本體右邊的位置
			vector<vector<RECT>> vt2rtPad;

			// 紀錄引腳框數量, size 預設為 4, 定義如下:
			// vtnCount_Lead[0] = 引腳在本體上方的數量
			// vtnCount_Lead[1] = 引腳在本體下方的數量
			// vtnCount_Lead[2] = 引腳在本體左邊的數量
			// vtnCount_Lead[3] = 引腳在本體右邊的數量
			vector<int> vtnCount_Lead;

			// 紀錄引腳框位置, size 預設為4個方向 每個方向預設100個, 每個方向的實際數量請參照 "vtnCount_Lead"
			// vt2rtLead[0] = 引腳在本體上方的位置
			// vt2rtLead[1] = 引腳在本體下方的位置
			// vt2rtLead[2] = 引腳在本體左邊的位置
			// vt2rtLead[3] = 引腳在本體右邊的位置
			vector<vector<RECT>> vt2rtLead;

			SAutoCreateROIResult()
			{
				rtPart.top = -1;
				rtPart.bottom = -1;
				rtPart.left = -1;
				rtPart.right = -1;

				vtnCount_Lead.resize(4, 0);
				vt2rtLead.resize(4, vector<RECT>(100));

				vtnCount_Pad.resize(4, 0);
				vt2rtPad.resize(4, vector<RECT>(100));
			}

			void Clear()
			{
				rtPart.top = -1;
				rtPart.bottom = -1;
				rtPart.left = -1;
				rtPart.right = -1;
				std::fill(vtnCount_Lead.begin(), vtnCount_Lead.end(), 0);
				std::fill(vtnCount_Pad.begin(), vtnCount_Pad.end(), 0);
			}
		};

		// 台達電金線線寬量測 輸入參數
		struct SDeltaMeasureParam
		{
			// 預設為 false(不開啟)
			// 是否進行影像濾波,背景有嚴重干擾再開啟
			bool bFilter;

			// 預設為 31, "bFilter" 必須為true才會執行
			// 濾波大小
			int nFilterSize;

			// 預設為 1
			// 1 = Canny找邊
			// 2 = 二值化找邊
			int nEdgeMode;		

			// 預設為 500
			// Canny低閥值
			int nLowThres;

			// 預設為 1500
			// Canny高閥值
			int nHeightThres;

			// 預設為 0
			// 影像高 = pu8Image的高度
			int nImageH;

			// 預設為 0
			// 影像寬 = pu8Image的寬度
			int nImageW;

			// 預設為 0
			// 零件量測範圍
			RECT rectPartRange;

			// 預設為 nullptr
			// pu8Image = 高角度白光彩色影像(檢測框影像)
			BYTE* pu8Image;

			SDeltaMeasureParam()
			{
				bFilter = false;
				nEdgeMode = 1;
				nFilterSize = 31;
				nImageH = 0;
				nImageW = 0;
				nLowThres = 500;
				nHeightThres = 1500;
				rectPartRange.left = 0;
				rectPartRange.right = 0;
				rectPartRange.top = 0;
				rectPartRange.bottom = 0;
				pu8Image = nullptr;
			}

			bool Check()
			{
				if (nLowThres <= 0 || nHeightThres <= 0 || nLowThres > nHeightThres || pu8Image==nullptr || 
					rectPartRange.left<0 || rectPartRange.left>= rectPartRange.right || rectPartRange.top<0 || 
					rectPartRange.top>= rectPartRange.bottom || nImageH<=0 || nImageW<=0 || nFilterSize<1 ||
					nEdgeMode<=0 || nEdgeMode>2)
				{
					return false;
				}

				return true;
			}
		};

		// 台達電金線線寬量測 輸出結果
		struct SDeltaMeasureResult
		{
			// 旋轉角度
			float fAngle;

			// 線寬(pixel)
			float fWidth;

			// 計算線寬的兩個端點(顯示用)
			POINT ptLimit1;
			POINT ptLimit2;

			// 物體中心線(顯示用)
			POINT ptCenterLine_Start;
			POINT ptCenterLine_End;

			SDeltaMeasureResult()
			{
				Init();
			}

			void Init()
			{
				fAngle = 0.0;
				fWidth = 0.0;
				ptLimit1.x = 0;
				ptLimit1.y = 0;
				ptLimit2.x = 0;
				ptLimit2.y = 0;
				ptCenterLine_Start.x = 0;
				ptCenterLine_Start.y = 0;
				ptCenterLine_End.x = 0;
				ptCenterLine_End.y = 0;
			}
		};

		class JETALG_API CHeightDetection
		{

		private:
			// 控制是否要儲存影像
			bool						m_SaveImage;

			// 零件邊界搜尋範圍 X 方向
			int							m_nSearchRangeX;

			// 零件邊界搜尋範圍 Y 方向
			int							m_nSearchRangeY;

			// 影像存檔順序
			int							m_nSaveIndex;

			// 
			int							m_nInspectionType;

			// 自動計算錫高位置時, 最大的偏移範圍, 預設為15(pixels)
			int							m_nTin_MaxShiftRange;

			// 計算錫高時取最高的百分比來平均, 預設為 0.3
			// m_fTinHeight_Percentage = 0.3 => 取最高的30%來平均
			float						m_fTinHeight_Percentage;

			// 零件本體的斜率
			float						m_fPartSlope;

			// 儲存影像的路徑與檔名
			string						m_strSavePathName;

			// 紀錄版次
			string						m_strVersion;

			// 輸入參數
			const STinInputParam*		m_psParam;

			const SGlueInputParam*		m_psParam_Glue;

			const SAutoCreateROIParam*	m_psParam_CreateROI;

			// 輸出結果
			STinOutputResult*			m_psResult;

			// 輸出結果
			SGlueOutputResult*			m_psResult_Glue;

			// 自動建框
			SAutoCreateROIResult*		m_psResult_CreateROI;

			// 用3D高度影像計算出來的零件ROI
			cv::Rect					m_cv3DROI;

			// m_cv3DROI 旋轉後的4個角點
			//vector<POINT>				m_vtpt3DROI_Rotate;
			vector<cv::Point2f>			m_vtpt3DROI_Rotate;

			// IC的ROI位置
			cv::Rect					m_cvICROI;

			// 零件的ROI位置
			cv::Rect					m_cvPartROI;

			// 計算零件高度的位置
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<vector<POINT>>		m_vtptPartArea;

			// 計算錫高度的位置
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<vector<POINT>>		m_vtptTinArea;

			// 紀錄不同偏移距離的錫高
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<vector<float>>		m_vt2fShiftTin_Height;

			// 紀錄最佳偏移距離
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<int>					m_vtnShiftTin_Dist;

			// 紀錄膠的範圍
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<vector<pair<POINT,POINT>>>	m_vtptGlueRange;

			// 紀錄膠的長度
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<vector<int>>			m_vtnGlueLength;

			// 紀錄計算膠高的位置
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<vector<POINT>>		m_vtptGlueHeightPos;

			// 紀錄膠的高度
			vector<vector<float>>		m_vtfGlueHeight;

			// 計算IC高度的位置
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<vector<POINT>>		m_vtptICHeightPos;

			vector<float>				m_vtfTinHeight;

			// 3D高度影像 CV_32FC1
			cv::Mat						m_mat3D;

			// 用m_mat3D做 Sobel計算 CV_8UC1
			cv::Mat						m_matSobel;

			// 2D彩色影像 CV_8UC3
			cv::Mat						m_matColor;

			cv::Mat						m_matLow;

			cv::Mat						m_matHeight;

		public:
			CHeightDetection();
			~CHeightDetection();

			// 取得版次
			string GetVersion()const { return m_strVersion; }

			// 高度量測
			// rectPartROI = 零件的位置
			bool MeasureHeight(const int& nImageH, const int& nImageW, const float* pfInput, const BYTE* pu8Image, const RECT& rect2DROI, const SHeightDetectionParam& sParam, SHeightDetectionResult& sResult);

			// 膠量(檢)測(膠長 膠高 溢膠)
			// nImageH = 影像高
			// nImageW = 影像寬
			// rectPartROI = 零件的位置
			// pfInput = 3D高度
			// vtpu8Image[0] = RGB光彩色影像
			// vtpu8Image[1] = 低角度白光彩色影像
			// vtpu8Image[2] = 高角度白光彩色影像
			// sParam = 輸入參數
			// sResult = 輸出結果
			bool Underfill(const int& nImageH, const int& nImageW, const RECT& rectPartROI, const float* pfInput, const vector<BYTE*>& vtpu8Image, const SGlueInputParam& sParam, SGlueOutputResult& sResult);

			// 自動建立零件框---使用2D影像
			bool AutoCreateROI_Old(const SAutoCreateROIParam& sParam, SAutoCreateROIResult& sResult);
			bool AutoCreateROI(const SAutoCreateROIParam& sParam, SAutoCreateROIResult& sResult);

			// 台達電金線量測
			bool DeltaMeasure(const SDeltaMeasureParam& sParam, SDeltaMeasureResult& sResult);

			// 量測電容錫高
			// nLocation : 錫的位置, 0=L; 1=T, 2=R, 3=B
			// nPartHeight : 零件高
			// nPartWidth : 零件寬
			// nSearchH : 錫高搜尋範圍---高
			// nSearchW : 錫高搜尋範圍---寬
			// fAngle : 零件旋轉角度
			bool Measure_Capacitance_Tin(const int& nLocation, const int& nPartHeight, const int& nPartWidth, const int& nSearchH, const int& nSearchW, const float& fAngle, const float* pf3D, float& fTinHeight, int& nDist);

			// 顯示台達電金線量測的結果
			// sResult = 量測結果
			// matImage = 顯示影像, 須為彩色影像
			bool ShowDeltaMeasureResult(const SDeltaMeasureResult& sResult, cv::Mat& matImage);

			// 彩色影像轉灰階
			// nMode = 0 ; 假如 matColor是灰階影像, 則 matGray = matColor
			//             假如 matColor是彩色影像, 則 return false
			// nMode = 1 ; 以B分量當灰階
			// nMode = 2 ; 以G分量當灰階
			// nMode = 3 ; 以R分量當灰階
			// nMode = 4 ; 以(R+G+B)/3當灰階
			// nMode = 5 ; HLS取S當灰階
			// nMode = 6 ; HSV取S當灰階
			// nMode = 7 ; PCA轉灰階
			bool ColorToGray(const cv::Mat& matColor, const int& nMode, const cv::Rect& cvPartROI, cv::Mat& matGray);

			// PCA轉灰階可輸入額外影像
			bool PcaToGray(const cv::Mat& matInput, const vector<cv::Mat>& vtmatAdd, cv::Mat& matOutput);

			// 影像強化
			// matSrc : 彩色或灰階皆可
			// nMode = 0 => Log
			//       = 1 => Equalize
			// bMedian : true => 對 matSrc 先進行中值濾波再做後續增強
			// nFilterSize : 中值濾波的 size , 須為奇數
			bool ImageEnhan(const cv::Mat &matSrc, cv::Mat &matDst, const int& nMode, const bool& bMedian, const int& nFilterSize);

			// 灰階影像濾波處理
			// matGray : 須為灰階影像
			//   nMode : 0, 不處理 (matGaryFilter = matGray) 
			//   nMode : 1, 中值濾波, 
			//		   : 2, 最小值濾波
			//		   : 3, 最大值濾波
			//		   : 4, 先最小值再最大值(Opening)
			//		   : 5, 先最大值再最小值(Closing)
			//		   : 6, 4+5
			//		   : 7, 5+4
			//         : 8, 梯度
			bool GrayFilter(const cv::Mat& matGray, const SGrayFilterParam& sParam, cv::Mat& matGaryFilter);

			// 二值化
			// matGray : 須為灰階影像
			// nMode : 1, 單閥值,					 bDark = true  ; 值域 [0 , nThreshold_Low]
			//										 bDark = false ; 值域 [nThreshold_Low, 255]
			//       : 2, 雙閥值,					 bDark = true  ; 值域 [nThreshold_Low , nThreshold_High]
			//										 bDark = false ; 值域 [0 , nThreshold_Low] + [nThreshold_High , 255]
			//       : 3, 自動閥值, Otsu,            bDark = true  ; 值域 [0 , Otsu]
			//										 bDark = false ; 值域 [Otsu , 255]
			//       : 4, 自動閥值, Triangle,        bDark = true  ; 值域 [0 , Triangle]
			//                                       bDark = false ; 值域 [Triangle , 255]
			//       : 5, 自動閥值, MaxEntropy,      bDark = true  ; 值域 [0 , MaxEntropy]
			//                                       bDark = false ; 值域 [MaxEntropy , 255]
			//       : 6, 自動閥值, MinCrossEntropy, bDark = true  ; 值域 [0 , MinCrossEntropy]
			//                                       bDark = false ; 值域 [MaxEntropy , 255]
			// return : 閥值
			int Threshold(const cv::Mat& matGray, const int& nMode, const bool& bDark, const int& nLow, const int& nHeigh, cv::Mat& matThres, double dA=1.0, double dB=0.0);

			// 型態學處理 
			// matInput : 須為灰階格式
			// nMode : 1, 侵蝕
			//       : 2, 膨脹
			//       : 3, Opening(先侵蝕再膨脹)
			//       : 4, Closing(先膨脹再侵蝕)
			//       : 5, 先 Opening 再 Closing
			//       : 6, 先 Closing 再 Opening
			// nSize : 濾波大小,須為奇數
			bool Morphology(const cv::Mat matInput, const int& nMode, const int& nSize);

			// 輸出處理模式執行的結果
			// nImageH = 影像高
			// nImageW = 影像寬
			// rectPartROI = 零件的位置
			// vtpu8Image[0] = RGB光彩色影像
			// vtpu8Image[1] = 低角度白光彩色影像
			// vtpu8Image[2] = 高角度白光彩色影像
			// sPM : 處理模式的參數
			// strFolderPath : 檔案輸出的資料夾路徑
			bool ShowProcessMode(const int& nImageH, const int& nImageW, const RECT& rectPartROI, const vector<BYTE*>& vtpu8Image, const SProcessMode& sPM, const string& strFolderPath);

			// 設定存圖的路徑與檔名(不需要副檔名)
			bool SetSavePathName(const string& strPathName);

			// 設定是否要輸出計算過程的影像
			void SetSaveImage(const bool& bSave);

			// 取出3D高度計算出來的ROI範圍
			void GetAlignmentROI(cv::Rect& cv3DROI);

			// 取出 IC的ROI範圍
			void GetICROI(cv::Rect& cvICROI);

			// 取出計算零件高度的座標
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<vector<POINT>>& GetPartCoordinate();		

			// 取出計算錫高度的座標
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<vector<POINT>>& GetTinCoordinate();

			// 取出偏移距離的錫高
			vector<vector<float>>& GetShiftTin_Height();

			vector<int>& GetShiftTin_Dist();
			
			// 取出膠的範圍
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			// pair<POINT, POINT> : first  => 膠範圍的起始點
			//                    : second => 膠範圍的終止點
			vector<vector<pair<POINT, POINT>>>& GetGlueRange();

			// 取出計算膠高的位置
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<vector<POINT>>& GetGlueHeightPos();

			// 取出計算IC高的位置
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<vector<POINT>>& GetICHeightPos();

			// 取出膠的長度
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<vector<int>>& GetGlueLength();

			//Capacitance
			vector<float>& GetCapacitanceTinHeight();

			// 輸入ROI的外接矩形與旋轉弧度, 計算旋轉後ROI的4個角點
			// // MBR = minimum bounding rectangle
			bool CalPartROI_3D(const cv::Rect& cvMBR, const float& fRadian, POINT& ptLT, POINT& ptRT, POINT& ptRB, POINT& ptLB);
			
			// 計算矩形旋轉後的4個角點
			bool CalRect_Corner(const cv::Rect2f& cvfROI, const float& fRadian, cv::Point2f& ptcvfLT, cv::Point2f& ptcvfRT, cv::Point2f& ptcvfRB, cv::Point2f& ptcvfLB);

			// 畫旋轉後的矩形
			// matShow : 要顯示的影像
			// fRadian : 旋轉的弧度
			// cvROI : 要旋轉的 ROI
			// scColor : 顏色
			void DrawRotateRectangle(cv::Mat& matShow, const float& fRadian, const cv::Rect& cvROI, const cv::Scalar& scColor, int nLineWidth);

			// cvCorner[0]
			void DrawRotateRectangle(cv::Mat& matShow, vector<POINT>& cvCorner, const cv::Scalar& scColor, int nLineWidth);

			bool Show_Part_3D(const int& nImageH, const int& nImageW, const float* pfInput, cv::Mat& matPart3D);
			bool Show_Tin_3D(const int& nImageH, const int& nImageW, const float* pfInput, cv::Mat& matTin3D);
			bool Show_Tin_MeasureROI(const int& nImageH, const int& nImageW, const float* pfInput, cv::Mat& matMeasureROI);

			// 顯示錫高量測結果
			// 會在 matImage 影像上顯示零件本體位置與錫高量測位置
			bool ShowTinResult(const bool& bShowText, cv::Mat& matImage);

			// 顯示ShowUnderfill的檢測結果	;   Vec3b.val[0] = 藍色	
			//									Vec3b.val[1] = 綠色
			//									Vec3b.val[2] = 紅色
			// matImage : 會在matImage上畫出膠的範圍(v3bGlueRange) 膠的最長距離(v3bLong) 膠的最短距離(v3bShort)
			// v3bGlueRange : 膠範圍的顏色
			// v3bLong : 膠最長距離的顏色
			// v3bShort : 膠最短距離的顏色
			bool ShowUnderfillResult(const bool& bShowText, const POINT& ptShift, cv::Mat& matImage, cv::Vec3b& v3bGlueRange, cv::Vec3b& v3bLong, cv::Vec3b& v3bShort, cv::Vec3b& v3bIC);

			// 顯示自動建立框的結果
			// matDisplay : 輸入空的 Mat 即可
			bool ShowAutoCreateROIResult_2D(cv::Mat& matDisplay);


		private:
			void ReleaseData();
			void Initial();

			// 檢查輸入參數
			bool CheckParam(const int& nImageH, const int& nImageW, const SHeightDetectionParam& sParam);

			// // 3D零件定位與錫高量測
			// 20221101 rect2DROI = 零件的位置
			bool MeasureHeight_Tin_StepSlope(const RECT& rect2DROI);

			bool MeasureHeight_Tin_IC_Pin(const RECT& rect2DROI);

			// 計算零件旋轉角度
			bool CalPartAngle(const cv::Mat& matThres, const cv::Rect& cvROI, double& dSlope, double& dAngle, cv::Point2d& ptcvdCenter);
			bool CalPartAngle(const cv::Mat matPartEdge, float& fRadian, float& fAngle);
			bool CalPartAngle(const cv::Mat matMask, const cv::Rect& cvROI_PartRange, float& fSlope, float& fRadian, float& fAngle);
			bool CalPartAngle(const cv::Mat matPartEdge, const cv::Rect& cvROI_PartRange, const cv::Rect& cvROI_PartSurface, float& fRadian, float& fAngle);

			bool Sobel_3D(const cv::Mat& mat3D, const int &nFilterSize, cv::Mat& matSobel);
			bool Sobel_2D(const cv::Mat& mat3D, const int &nFilterSize, cv::Mat& matSobel);
			bool EdgeImage(const cv::Mat& matThres, cv::Mat& matEdge);
			bool GetEdgeImage(const cv::Mat& matThres, const bool& bBoundary, cv::Mat& matEdge);

			// matImage 須為二值化影像或邊界影像
			// 若bEdge為true => 會進行取邊界
			// 若bClose為true => 只保留封閉邊界的物件
			// 若bSort為true => 會重新進行位置排序
			bool SearchROI(cv::Mat &matImage, const int &nMinLength, const int &nMaxLength, vector<vector<POINT>>& cvptEdge, cv::Rect& cvROI);
			bool SearchEdge(cv::Mat& matEdge, std::vector<cv::Rect>& vtcvGlueROI, std::vector<std::vector<std::vector<POINT>>>& vtEdge);
			bool SearchROI_MaxROI(cv::Mat &matImage, const int &nMinLength, const int &nMaxLength, cv::Rect& cvROI);
			// 搜尋指定大小的 ROI
			bool SearchROI(cv::Mat &matImage, const int &nROIW, const int &nROIH, cv::Rect& cvROI);

			// 取出最靠近影像中心的 ROI Index
			bool GetCenterROI(const int& nImageWidth, const int& nImageHeight, const std::vector<cv::Rect>& vtcvROI, int& nIndex);

			// 用3D高度找出零件範圍
			// 20221028
			bool Alignment_Z(const cv::Mat& mat3D, const int& nPatternW, const int& nPatternH, cv::Rect& cv3DROI);

			// 輸入不同高度切出來的ROI 計算3D ROI
			bool Find_3DROI_Step(const vector<cv::Rect>& vtcvROI, const int& nThresDiffZ_Width, const int& nThresDiffZ_Height, const int& nNumber, cv::Rect& cv3DROI);
			
			// 所有邊都會做2次搜尋
			bool Find_3DROI_Step_Suface(const vector<cv::Rect>& vtcvROI, const int& nThresDiffZ_Width, const int& nThresDiffZ_Height, cv::Rect& cv3DROI);
			
			// 20230317 Jun+ 非量測邊不做2次搜尋
			// 輸入高度切割 ROI與 3D斜率ROI, 來計算最終 ROI
			bool Find_3DROI_Step_Suface(const vector<cv::Rect>& vtcvROI, const int& nThresDiffZ_Width, const int& nThresDiffZ_Height, const cv::Rect& cv3DROI_Slope, cv::Rect& cv3DROI);

			// 20230228
			// 計算3D零件的範圍與旋轉角度
			bool Alignment_Z_StepSlope(const cv::Mat& mat3D, cv::Rect& cv3DROI);
			bool Alignment_Z_IC_Pin(const cv::Mat& mat3D, cv::Rect& cv3DROI);

			// nMode = 1 大到小
			// nMode = 2 小到大
			bool Threshold_StepZ(const int& nMode, const int& nStartZ, const int& nEndZ, const int& nRangeZ, const cv::Rect& cvROI_Part, const cv::Mat& mat3D, vector<cv::Rect>& vtROI);
			bool Threshold_DiffSlope(const cv::Mat& mat3D, cv::Rect& vtROI_Part, cv::Mat& mat3DThres_Gradient);
			bool Threshold_DiffSlope(const cv::Mat& mat3D, const cv::Rect& cvROI_PartRange, const cv::Rect& cvROI_SurfaceRange, cv::Rect& cvROI_PartSurface, cv::Mat& matEdge_3D, cv::Mat& matMask, cv::Mat& matNoise);

			// 計算3D零件本體範圍跟每一根Pin的範圍
			bool Threshold_IC_Pin(const cv::Mat& mat3D, cv::Rect& vcROI_IC);

			// 計算實際Pin的ROI範圍
			// vt2cvrtPin => 0=Top; 1=Bottom; 2=Left; 3=Right
			bool PinFit(vector<vector<cv::Rect>>& vt2cvrtPin);
			bool FittingPin(vector<cv::Rect>& vtnTop, vector<cv::Rect>& vtnBottom, vector<cv::Rect>& vtnLeft, vector<cv::Rect>& vtnRight);

			// 20230320 計算 IC 邊界範圍
			// nDirection : 1(上) 2(下) 3(左) 4(右)
			bool Cal_IC_Boundary(const cv::Mat& mat3D, const cv::Rect& cvROI_PartSurface, const int& nDirection, const int& nExpand, int& nPosition);

			// 20230321 計算 Pin ROI
			// nDirection : 1(上) 2(下) 3(左) 4(右)
			bool Cal_PinROI(const cv::Mat& mat3D, const cv::Rect& cvROI_IC, const int& nDirection, vector<cv::Rect>& vtcvPin);

			// 計算零件範圍
			bool GrabMaxROI(const cv::Mat& matGray, const int& nThresMode, int& nThres, cv::Rect& cvROI);

			// 初步計算 零件範圍 cvPartROI
			bool Cal_PartROI_2D(cv::Mat& matGray, int& nThres, cv::Rect& cvInitialROI, cv::Rect& cvPartROI);

			// 20230621 檢察 ICROI 範圍是否正確
			bool Check_ICROI_Range_2D(const cv::Rect& cvInitialROI, const cv::Rect& cvPartRange, const std::vector<cv::Rect>& vtcvPadROI, cv::Rect& cvICROI);

			// 初步計算 PadROI
			bool Cal_PadROI_2D(const cv::Mat& matThres, const cv::Mat& matThres2, const cv::Rect& cvInitialROI, cv::Rect& cvPartROI, cv::Rect& cvICROI, std::vector<cv::Rect>& vtcvPadROI);

			// 利用初步 ICROI 與 PadROI 重新計算 PartROI與 PadROI
			bool ReCal_PartROI_PadROI(cv::Rect& cvPartROI, cv::Rect& cvICROI, std::vector<cv::Rect>& vtcvPadROI);

			// 初步計算 ICROI
			bool Cal_ICROI_2D(const cv::Mat& matPartGray, cv::Rect& cvICROI, cv::Mat& matThres, cv::Mat& matThres2);

			// 初步計算 IC ROI 與 PadROI
			bool Cal_ICROI_PadROI_2D(const cv::Mat& matGray, const cv::Rect& cvPartRange, cv::Rect& cvPartROI, cv::Rect& cvICROI, std::vector<cv::Rect>& vtcvPadROI);

			// 輸入初始 PartROI ICROI PadROI, 再重新計算
			bool ReCal_PartROI_ICROI_PadROI_2D(cv::Mat& matGray, const cv::Rect& cvPartRange, cv::Rect& cvPartROI, cv::Rect& cvICROI, std::vector<cv::Rect>& vtcvPadROI);

			// 20230613 計算 引腳ROI
			bool Cal_Lead_Object(const cv::Mat& matThres, const cv::Rect& cvPadROI, const int& nDirection, cv::Rect& cvLeadROI);
			bool Cal_InitialLeadROI(const cv::Mat& matThres, const cv::Rect& cvICROI, vector<vector<cv::Rect>>& vt2cvPadROI, vector<vector<cv::Rect>>& vt2cvInitialLeadROI, cv::Point& cvptLead_Std);
			
			// 重新計算 ICROI 與 PadROI
			bool ReCal_ICROI_PadROI_2D(const vector<vector<cv::Rect>>& vt2cvTempLead, const cv::Point& cvptLead_Std, cv::Rect& cvICROI, vector<vector<cv::Rect>>& vt2cvPadROI);
			
			// 計算 Lead ROI, 並確認 PartROI 與 PadROI
			bool Cal_LeadROI_2D(const cv::Rect& cvPartRange, const cv::Rect& cvPartROI, cv::Rect& cvICROI, vector<vector<cv::Rect>>& vt2cvPadROI, vector<vector<cv::Rect>>& vt2cvLeadROI);

			// 輸出對齊後的 ROI
			bool OutputAlignROI_2D(const int& nShiftX, const int& nShiftY, const vector<vector<cv::Rect>>& vtcvPadROI, vector<int>& vtnCount, vector<vector<RECT>>& vt2rtPadROI);
			bool AlignROI_2D(const vector<vector<cv::Rect>>& vtcvROI, vector<vector<cv::Rect>>& vt2rtAlignROI);

			// 刪除不是Pin的異物
			bool DeleteErrorPad(const cv::Mat& matPartGray, const cv::Rect& cvICROI, vector<cv::Rect>& vtcvPinROI);
			bool KMean(const float& fThres_Ratio, vector<cv::Rect>& vtcvTopROI, vector<cv::Rect>& vtcvBottomROI, vector<cv::Rect>& vtcvLeftROI, vector<cv::Rect>& vtcvRightROI);
			bool KMean(const float& fThres_Ratio, vector<cv::Rect>& vtcvPinROI);

			// 輸入所有 ROI 計算標準大小
			bool KMean_STD(const vector<cv::Rect>& vtcvPinROI, cv::Point& cvptPin);

			// PadROI 依 ICROI 定位 長度對齊 大小一致
			bool PadROI_Position_Align(const vector<vector<cv::Rect>>& vtcvROI, const cv::Rect& cvICROI, vector<vector<cv::Rect>>& vt2cvROI);

			// LeadROI 依 ICROI 定位 長度對齊 大小一致
			bool LeadROI_Position_Align(const vector<vector<cv::Rect>>& vtcvLeadROI, const vector<vector<cv::Rect>>& vtcvPadROI, const cv::Rect& cvICROI, vector<vector<cv::Rect>>& vt2cvROI);

			// 初步計算本體框
			// cvRange = 計算範圍
			// cvPartROI = 零件ROI
			// cvICROI = IC本體ROI
			bool Cal_Initial_ICRange_New(cv::Rect& cvRange, cv::Rect& cvPartROI, cv::Rect& cvICROI, cv::Mat& matMask, cv::Mat& matPCA, cv::Mat& matLow_HSV_S);

			// 初步計算Pad Lead範圍
			bool Cal_Initial_PadLeadROI_New(const cv::Mat& matMask, const cv::Mat& matPCA, const cv::Mat& matLow_HSV_S, const cv::Rect& cvPartROI, const cv::Rect& cvICROI, vector<vector<cv::Rect>>& vt2PadROI);

			// 精確計算Pad範圍
			bool Cal_Real_PadLeadROI_New(const cv::Mat& matMask, const cv::Rect& cvICROI, const cv::Rect& cvPartROI, const vector<vector<cv::Rect>>& vt2PadROI_Init, vector<vector<cv::Rect>>& vt2PadROI, vector<vector<cv::Rect>>& vt2LeadROI);

			// 調整 Pin ROI 大小
			bool FixPadROI(const cv::Mat& matPartGray, const cv::Rect& cvICROI, vector<cv::Rect>& vtcvPinROI);

			// 直線掃描轉換
			// nImageW : 影像寬
			// nImageH : 影像高
			// ptStart : 直線起始點
			// ptEnd : 直線終點
			// vtptLine : 起始點到終點所經過的點位
			bool ScanningLine(const int &nImageW, const int &nImageH, const POINT &ptStart, const POINT &ptEnd, std::vector<POINT> &vtptLine);

			// 計算線性方程式 y = dSlope * x + dIntercept(單次)
			// vtdCoordinate_X = 每一點的X座標
			// vtdCoordinate_Y = 每一點的X座標
			// dSlope = 輸出方程式的斜率
			// dIntercept = 輸出方程式的截距
			bool SimpleLinearRegr(const vector<double>& vtdCoordinate_X, const vector<double>& vtdCoordinate_Y, double &dSlope, double &dIntercept);

			// 計算線性方程式(不包含垂直線) y = dSlope * x + dIntercept(遞迴)
			// vtdCoordinate_X = 點的X座標
			// vtdCoordinate_Y = 點的Y座標
			// nSumTimes = 遞迴的最大次數
			// dDiff = 前後2次的斜率差若小於 dDiff, 則不再計算 
			// dSlope = 輸出方程式的斜率
			// dIntercept = 輸出方程式的截距
			bool SimpleLinearRegr(const vector<POINT>& vtptPoint, const int& nSumTimes, const double& dDiff, double& dSlope, double& dIntercept);

			bool SimpleLinearRegr_Vertical(const vector<POINT>& vtptPoint, const int& nSumTimes, const double& dDiff, double& dSlope, double& dIntercept);

			// 20231026 計算零件高
			bool CalPartHeight();
			// 20231026 計算零件高---垂直方向
			bool CalPartHeight_Vertical();
			// 20231026 計算零件高---水平方向
			bool CalPartHeight_Horizontal();

			// 20231026 計算錫高
			bool CalTinHeight();
			bool CalTinHeight_Vertical();
			bool CalTinHeight_Horizontal();

			// 計算錫的平均高度
			// nDirection = 錫在零件方向, 0=上, 1=下, 2=左, 3=右
			// nSpace = 單邊忽略不計算的數量
			// vtptTinPos = 錫的座標
			bool CalTinMeanZ(const int& nDirection, const int& nSpace, const vector<POINT>& vtptTinPos, float& fMeanZ);

			// 計算錫高
			// nType=1 : 自動找
			// nType=2 : 先大到小排序, 輸出前 n% 的平均 (n目前預設為 30)
			// nType=3 : 平均值	
			bool OutputTinResult(const int& nType, vector<float>& vtfTin, float& fTin, int& nDistIndex);
			bool OutputTinResult(const int& nType, const bool bDataStd, const int& nLength, const int& nShift, vector<float>& vtfTin, float& fTin, int& nDistIndex);
			bool OutputTinResult(const int& Model, const int& nLocation, const bool& bDataStd, const int& nLength, const int& nShift, vector<float>& vtfTin, float& fTin, int& nDistIndex);
			bool CalTurningPoint_Model1(const int& nLocation, const int& nLength, const int& nShift, vector<float>& vtfTin, float& fTin, int& nDistIndex);
			bool CalTurningPoint_Model2(const int& nLocation, const bool& bDataStd, const int& nLength, const int& nShift, vector<float>& vtfTin, float& fTin, int& nDistIndex);

			// 點旋轉
			void PointRotate(const int& nImageW, const int& nImageH, const cv::Point2f& cvfCenter,  const POINT& ptInput, const float& fRadian, POINT& ptOutput);
			void PointRotate(const int& nImageW, const int& nImageH, const cv::Point2f& cvfCenter, const POINT& ptInput, const float& fRadian, const float& fScaleX, const float& fScaleY, POINT& ptOutput);
			void PointRotate(const int& nImageW, const int& nImageH, const cv::Point2f& cvfCenter, const cv::Point2f& cvfInput, const float& fRadian, cv::Point2f& cvfOutput);
			void PointRotate(const int& nImageW, const int& nImageH, const cv::Point2f& cvfCenter, const cv::Point2f& cvfInput, const float& fRadian, const float& fScaleX, const float& fScaleY, cv::Point2f& cvfOutput);

			// 影像旋轉
			bool ImageRotate(const cv::Mat &matInput, const float &fDegree, cv::Mat &matRotate);

			// 最大熵二值化
			// return : 閥值
			int Threshold_MaxEntropy(const cv::Mat& matSrc);

			// 最小交叉熵二值化
			// return : 閥值
			int Threshold_MinCrossEntropy(const cv::Mat& matSrc);

			// 區間二值化
			// matInput : 輸入影像
			// matOutput : 輸出影像
			// nThres_Low : 低閥值
			// nThres_Height : 高閥值
			// nTargetValue : 目標物的值
			// bMoreThan = true 代表抓取 [nThres_Low,nThres_Height]之間的區域 (包含nThres_Low與nThres_Height); 
			//             false代表抓取 [0,nThres_Low) 與 (nThres_Height,255]的區域 (不含nThres_Low與nThres_Height)
			bool Threshold_Tow(const cv::Mat& matInput, cv::Mat& matOutput, const int& nThres_Low, const int& nThres_Height, const int& nTargetValue, const bool& bMoreThan);

			bool ContrastEnhan(const cv::Mat &matGray, cv::Mat &matDst);

			// 計算IC範圍
			bool CalculateIC_ROI(cv::Mat &matSrc);

			// 計算膠的位置
			bool CalculateGlue_Mask(cv::Mat &matSrc, const cv::Rect& IC_ROI, const int& nExpand, cv::Mat& matGlueThres);

			// 膠ROI區域的型態學
			bool Morphology_GlueMask(cv::Mat& matImage, const int& nSize);

			// 計算膠的長度
			bool CalculateGlue_Length(const cv::Rect& IC_ROI, const cv::Rect& ROI2D, const cv::Mat& matGlueThres);

			// 偵測 IC 的高度
			// mat3D : 高度影像
			// IC_ROI : IC的位置
			// nRange : IC 的量測範圍
			// nShift : 從IC邊界往內偏移的距離 (pixels)
			// fScale : IC邊長的縮放比例
			bool DetectHeight_IC(cv::Mat &mat3D, const cv::Rect& IC_ROI, const int& nRange, const int& nShift, const float& fScale);

			// 偵測膠的高度--2D--最大梯度
			// mat3D : 高度影像
			// IC_ROI : IC的位置
			bool DetectHeight_Glue_2D_MaxGradient(cv::Mat &mat3D, const cv::Rect& IC_ROI);

			// 偵測膠的高度--2D--RGBLight--Blue
			// mat2D : RGB光影像的藍色分量
			// mat3D : 高度影像
			bool DetectHeight_Glue_2D_Blue(cv::Mat &mat2D, cv::Mat &mat3D);

			// 偵測膠的高度--3D定位
			// mat3D : 高度影像
			// nRange : 膠 的量測範圍
			// IC_ROI : IC的位置
			bool DetectHeight_Glue_3D(cv::Mat& mat3D, const int& nRange, const cv::Rect& IC_ROI);

			// 溢膠偵測
			bool DetectionGlueSpill(cv::Mat &matSrc, const cv::Rect& IC_ROI);

			// 搜尋溢膠邊界
			bool SearchGlueSpill(cv::Mat &matEdge, const int &nMinPerimeter, const int &nMaxPerimeter, vector<vector<POINT>>& vtEdgePoint, vector<cv::Rect>& vtcvRECT);

			// 灰階垂直投影 : matSrc須為灰階格式, bBright為true=> 找大於nThres的部分做投影, vtnProjection_V=垂直投影向量
			bool Projection_Gray_V(const cv::Mat& matGray, const int& nThres, const bool& bDark, vector<int>& vtnProjection_V);
			bool Projection_GrayROI_V(const cv::Mat& matGray, const vector<cv::Rect>& vtcvrtROI, const int& nThres, const bool& bDark, vector<int>& vtnProjection_V);

			// 灰階水平投影 : matSrc須為灰階格式, bBright為true=> 找大於nThres的部分做投影, vtnProjection_H=水平投影向量
			bool Projection_Gray_H(const cv::Mat& matGray, const int& nThres, const bool& bDark, vector<int>& vtnProjection_H);
			bool Projection_GrayROI_H(const cv::Mat& matGray, const vector<cv::Rect>& vtcvrtROI, const int& nThres, const bool& bDark, vector<int>& vtnProjection_H);

			// 檢察 GrayFilterParam
			bool Check_GrayFilterParam(const SGrayFilterParam& sParam);

			// 檢查 SProcessMode 
			bool Check_ProcessMode(const SProcessMode& sPM);

			// 檢查 RECT
			bool Check_RECT(const int& nImageH, const int& nImageW, const RECT& ROI);

			// 檢查 Rect
			bool Check_Rect(const int& nImageH, const int& nImageW, const cv::Rect& cvROI);

			// 如果 ROI 範圍異常, 則會修改 ROI
			void Fix_Rect(const int& nImageH, const int& nImageW, cv::Rect& ROI);

			// nExpand : 擴大的範圍
			void RECTToRect(const RECT& ROI, const int& nExpandX, const int& nExpandY, cv::Rect& cvROI);

			bool FillEdge(cv::Mat& matEdge, cv::Mat& matFill);

			bool Save3D_FImg(const string &strPathName, cv::Mat &matImage);

			// 畫出投影Data
			void Draw_Projection(const bool& bVertical, const vector<int>& vtnProjection, cv::Mat& matImage);

			// 將 vtfData 進行標準化
			bool Data_Standard(const vector<float>& vtfData, vector<float>& vtfStd);

			// 檢察 SAutoCreateROIParam 的參數是否正確
			bool Check_AutoCreateROIParam(const SAutoCreateROIParam& sParam);

			void InitialAutoCreateROI(const SAutoCreateROIParam& sParam, const SAutoCreateROIResult& sResult);

			// ROI外擴 聯集
			bool Union_ROI(const vector<cv::Rect>& vtcvROI, const int& nDistX, const int& nDistY, vector<cv::Rect>& vtcvUnionROI);

			// strFolderPath : 存檔的資料夾路徑
			// strName : 存檔檔名(不需副檔名)
			void OutputData(const string& strFolderPath, const string& strName, vector<float>& vtfData);
			void OutputData(const string& strFolderPath, const string& strName, vector<vector<float>>& vt2fData);
			void OutputHeightData(const string& strFolderPath, const string& strName, const int& nLocation, vector<vector<float>>& vt2fData);
		};
	}
}