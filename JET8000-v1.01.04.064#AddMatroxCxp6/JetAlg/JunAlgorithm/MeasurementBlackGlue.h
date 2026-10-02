#pragma once
//==============================================================
// 20250220 Version 1.2.1 修改 Save_Parameter() 與 Load_Parameter(), 漏存與少讀的bug, 修改後 Version 1.2.2
// 20250224 Version 1.2.2 修改 1. 膠調參流程 Location=7時會出現例外的 bug
//                             2. 增加 影像解析度, 將量測結果轉成 um, 修改後 Version 1.2.3
// 20250225 Version 1.2.3 修改 1. 每組量測ROI除了最大距離 與 最小距離外, 再增加 (最大-最小距離)
//                             2. 對 最大距離 最小距離, (最大-最小距離都) 增加 上下限值, 並增加相關的code, 修改後 Version 1.2.4
// 20250304 Version 1.2.4 修改 1. Glue_Method1 抽色後增加過濾雜訊的功能, 調參流程從原本4步, 變成5步
//                             2. 膠參數結構 SGlueEdge_Parameter 中的 nLocation , 原本是1-8, 增加到 9代表整版, 修改後 Version 1.2.5
// 20250305 Version 1.2.5 修改 1. 單一量測框計算失敗時不直接 return false 讓程式結束, 改成可繼續計算其他框, 修改後 Version 1.2.6
// 20250310 Version 1.2.6 修改 1. 膠參數增加 nFilter_Open濾波 與 nFilter_Open2濾波, 
//                             2. 板邊參數增加Median濾波 nOpenX濾波 Dilate濾波 Erosion濾波 與多加3組抽色參數                
//                             3. 修正板邊有凹槽時會算錯的例外, 修改後 Version 1.2.9
// 20250318 Version 1.2.9 修改 1. 增加散熱膠(SThermalGlue)的量測, 參數定義:SThermalGlue_Parameter, 量測結果定義:SThermalGlue_Result
//                             2. Measurement()須輸入3張影像, // [In] vtInputImage 的 size 為 3
			                                                  // [In] vtInputImage[0] = 量測膠邊的彩色影像
															  // [In] vtInputImage[1] = 量測板邊的彩色影像
															  // [In] vtInputImage[2] = 量測散熱膠的彩色影像, 修改後 Version 1.3.0
// 20250324 Version 1.3.0 修改 1. 增加Coating邊界偵測, 參數定義:SCoatingEdge_Parameter, 偵測結果定義:SCoatingEdge_Result
//                             2. 增加 sCoatingToGlue_Horizontal, sCoatingToGlue_Vertical, sCoatingToBoard_Horizontal, sCoatingToBoard_Vertical, 量測方式
//                             3. Measurement()須輸入4張影像, // [In] vtInputImage 的 size 為 4
			                                                  // [In] vtInputImage[0] = 量測膠邊的彩色影像
															  // [In] vtInputImage[1] = 量測板邊的彩色影像
															  // [In] vtInputImage[2] = 量測散熱膠的彩色影像, 
															  // [In] vtInputImage[3] = Coating的彩色影像, 修改後 Version 1.4.0
// 20250401 Version 1.4.0 修改 1. 修改 ParameterToConfig() 與 ConfigToParameter() 中變數存取錯誤的 bug
//                             2. 增加 板邊在 nLocation=9時, 可以計算板邊斜率的功能
//                             3. 增加 板邊在 nLocation=9且有開啟斜率計算時, 相關的量測功能
//                             4. 增加 SThermalGlue的 Die 開啟 "Rebuild"功能時, 會精密計算Die邊界的功能
//                             5. 增加 SThermalGlue的 Die開啟 "Rebuild"時的相關量測功能
//                             6. 修改 sBoardToGlue_Horizontal, sBoardToGlue_Vertical, sCoatingToGlue_Horizontal, sCoatingToGlue_Vertical, 
//                                     sCoatingToBoard_Horizontal, sCoatingToBoard_Vertical 的量測方向性, 修改後 Version 1.4.6
// 20250401 Version 1.4.6 修改 1. 修改 sCoatingToGlue_Vertical 量測方向錯誤的 bug, 修改後 Version 1.4.7
// 20250401 Version 1.4.7 修改 1. 將 OpencCV 2.4.13.6 換成 OpenCV 3.4.16, 並進行相關修改, 修改後 Version 1.5.0
// 20250402 Version 1.5.0 修改 1. 修改 sCoatingToGlue_Horizontal, sCoatingToGlue_Vertical 量測方向的判斷方式, 修改後 Version 1.5.1
// 20250407 Version 1.5.1 修改 1. 修改 調參錯誤時, 執行 量測功能時會閃退的 bug
//                             2. 增加調參錯誤時的錯誤輸出, 修改後 Version 1.5.2
// 20250418 Version 1.5.2 增加 1. "FindEdge"處理流程的相關結構與操作函數, "FindEdge"為標準化找邊流程, 修改後 Version 1.6.0
// 20250429 Version 1.6.0 增加 1. "Glue HeightRatio algorithm"的相關參數與對應函數, 客戶為成都富士康, 使用opencv 2.4.13.6, 修改後 Version 1.7.0
// 20250505 Version 1.7.0 修改 1. "Glue HeightRatio algorithm"的 "高度算法" 與 "高度上下限" 都改為以 "區塊" 為單位來設定
//                             2. 重建IC邊界時, 會將重建結果內縮再輸出
//                             3. 設定上下限的預設值為 SLimit_Single(90.0f, 70.0f)
//                             4. 增加IC邊界範圍內縮功能, 修改後 Version 1.7.4
// 20250512 Version 1.7.4 增加 1. "FindEdge"結構增加 物件刪除模式 "nDeleteMode", 1=>保留最大面積, 2=>保留指定物件附近 
//                             2. "SetFindEdgeProcess()" 增加對應不同 nDeleteMode 的流程
//                             3. "RunProcess_FindICEdge()" 與 "RunProcess_FindGlueAreaEdge" 無論 nDeleteMode 是多少都強制為 1
//                             4. "RunProcess_FindGlueHeightEdge()"在nDeleteMode為2時, 會以IC邊界為基準來找膠邊的流程
//                        修改 5. 膠高結果判斷改為先依各區塊的高度算法輸出膠高比例, 再以各區的膠高比例為輸入值再計算第2次高度算法當作最終輸出
//                             6. 將原本每區都可調上下限制, 改為只對最終輸出調上下限值
//                             7. 上下限的預設值改為 SLimit_Single(100.0f, 25.0f), 修改後 Version 1.7.11
// 20250519 Version 1.7.11修改 1. 輪廓填充改回原始版本, 修改後 Version 1.7.12
// 20250520 Version 1.7.12修改 1. IC輪廓異常時會閃退的 bug, 修改後 Version 1.7.13
// 20250521 Version 1.7.13修改 1. 找膠邊流程誤用Mask導致UI程式Step6出現異常的bug, 修改後 Version 1.7.14
// 20250527 Version 1.7.14修改 1. 修改 ShowResult_GlueHeight(), 將各區高度比例顯示在圖上
//                             2. 量測前先將膠輪廓填滿, 避免輪廓上緣出現在IC表面(IC表面有膠且與側邊膠連在一起)導致量測高度為0的情況, 修改後 Version 1.7.16
// 20250708 Version 1.8.0 --- 1. 增加 檢測 AMD Flux 有無的演算法
//                            2. 黑膠增加 新量測模式 nLocation=10, 並修改SMeasurementBlackGlue_Parameter 與 SMeasurementBlackGlue_Result 中的相關結構與 Save_Parameter()跟 Load_Parameter()的內容
// 20250709 Version 1.8.1 --- 1. "JET_ImageFunction.h" 增加 "SplitChannel()" 與 "MergeChannel()" , 替換原本 OpenCV的寫法
// 20250710 Version 1.8.5 --- 1. 黑膠算法增加報表輸出功能
//                            2. 黑膠算法結果影像增加標註量測項目
//                            3. 修改 Flux 設定參數 "總面積" 無法修改的 bug
//                            4. Flux UI 的"量測"功能增加輸出結果
// 20250711 Version 1.8.6 --- 1. Flux 報表 Upper 與 Lower 欄位對調
// 20250711 Version 1.8.7 --- 1. 修改 Flux 報表 StartEnd 項目參數輸入錯誤的bug
// 20250724 Version 1.9.0 --- 1. 修改 鴻佰膠面積偵測 演算法
// 20250801 Version 2.0.0 --- 1. 將 SFindEdge_Parameter相關結構移出
// 20250902 Version 2.0.1 --- 1. 修改 報表不會輸出 Diff相關結果的 bug
// 20251015 Version 2.1.0 --- 1. 膠高量測演算法, IC參數增加定位方式; 1=>IC定位, 2=>Connector定位
// 20251016 Version 2.1.1 --- 1. 修改 膠高量測演算法, 抽色範圍的算法
// 20251021 Version 2.1.4 --- 1. 膠高量測演算法中的IC內縮範圍,改成2個變數來控制
//                            2. 修改膠高量測演算法的基準邊算法, 並將搜尋範圍開放出來設定
//                            3. 修改 Connector定位 流程
// 20251023 Version 2.1.6 --- 1. 增加用IC(Connector)外接矩形直接量測, 不做線性回歸
//                            2. 修改輪廓搜尋的邏輯, 避免物件不連接
// 20251023 Version 2.1.7 --- 1. 彩色抽色增加HSV模式
// 20251205 Version 2.1.8 --- 1. 修改膠高量測在輸出區塊膠高比例時, 解析度使用錯誤的bug
// 20251231 Version 2.2.0 --- 1. 增加黑膠相關量測可以用取N點平均再輸出的功能
//                            2. 修改 ReportTxt_BlackGlue_Color(), 報表文字與顯示顏色長度不一致的 bug
//==============================================================

#include <vector>
#include <string>

#include "JET_ImageFunction.h"
#include "ImageProcessBaseStruct.h"
#include "ConfigFile.h"


namespace JET {
	namespace alg {
		using namespace JET::mod;
		using namespace std;
		using namespace cv;



#pragma region 3D AOI AMD

#pragma region Black Glue Parameter

		// 板邊參數
		struct SBoardEdge_Parameter
		{
			// 預設為 false
			// 是否啟用板邊偵測
			bool bEnable_Board;

			// 預設為 false
			// 是計算板邊斜率
			bool bEnable_Slope;

			// 預設為 1 (目前只有一種)
			// 找板邊的方法
			int nFindEdgeMethod;
			
			// 預設為 R=20, G=110, B=160 值域[0,255]
			// 板邊的RGB值
			int nBoardValue_R;
			int nBoardValue_G;
			int nBoardValue_B;

			// 預設 50 (對BoardValue取正負50的範圍), 值域[0,255] 
			// 板邊RGB值的誤差範圍範圍
			int nBoardValueTolerance;

			// 第二組抽色參數, 預設 false, R2=G2=B2=10, Tolerance2=30
			bool bEnable_Color2;
			int nBoardValue_R2;
			int nBoardValue_G2;
			int nBoardValue_B2;
			int nBoardValueTolerance2;

			// 第三組抽色參數, 預設 false, R3=G3=B3=10, Tolerance3=30
			bool bEnable_Color3;
			int nBoardValue_R3;
			int nBoardValue_G3;
			int nBoardValue_B3;
			int nBoardValueTolerance3;

			// 第四組抽色參數, 預設 false, R4=G4=B4=10, Tolerance4=30
			bool bEnable_Color4;
			int nBoardValue_R4;
			int nBoardValue_G4;
			int nBoardValue_B4;
			int nBoardValueTolerance4;

			// 預設都是 1 pixels, 不可小於1
			// 彩色影像濾雜訊
			int nMedianX;
			int nMedianY;

			// 預設都是 1 pixels, 不可小於1
			// 抽色後濾雜訊
			int nOpenX;
			int nOpenY;

			// 預設都是 15 pixels, 不可小於1, 最好是奇數
			// 將版邊二值化影像的連接濾波, 將孔洞連接起來
			int nConnectX;
			int nConnectY;

			// 預設都為 1 pixels (1代表不執行), 不可小於1, 最好為奇數
			// 膠邊界的膨脹濾波, 將膠邊界外擴, 單位為 pixels
			int nDilateX;
			int nDilateY;

			// 預設都為 1 pixels (1代表不執行), 不可小於1, 最好為奇數
			// 膠邊界的侵蝕濾波, 將膠邊界內縮, 單位為 pixels
			int nErosionX;
			int nErosionY;

			SBoardEdge_Parameter() : bEnable_Board(false), bEnable_Slope(false), nFindEdgeMethod(1), 
				nBoardValue_R(20), nBoardValue_G(110), nBoardValue_B(160), nBoardValueTolerance(50),
				bEnable_Color2(false), nBoardValue_R2(10), nBoardValue_G2(10), nBoardValue_B2(10), nBoardValueTolerance2(30),
				bEnable_Color3(false), nBoardValue_R3(10), nBoardValue_G3(10), nBoardValue_B3(10), nBoardValueTolerance3(30),
				bEnable_Color4(false), nBoardValue_R4(10), nBoardValue_G4(10), nBoardValue_B4(10), nBoardValueTolerance4(30),
				nMedianX(1), nMedianY(1), nOpenX(1), nOpenY(1), nConnectX(15), nConnectY(15), nDilateX(1), nDilateY(1), nErosionX(1), nErosionY(1) {}

			~SBoardEdge_Parameter() = default;

			bool Check(string& strInfo) const
			{
				if (!bEnable_Board) return true;
				if (nFindEdgeMethod != 1) {
					strInfo = "BoardEdge FindEdgeMethod is error";
					return false;
				}

				if (nBoardValue_R < 0 || nBoardValue_R > 255) {
					strInfo = "BoardEdge BoardValue_R is error";
					return false;
				}

				if (nBoardValue_G < 0 || nBoardValue_G > 255) {
					strInfo = "BoardEdge BoardValue_G is error";
					return false;
				}

				if (nBoardValue_B < 0 || nBoardValue_B > 255) {
					strInfo = "BoardEdge BoardValue_B is error";
					return false;
				}

				if (nBoardValueTolerance < 0 || nBoardValueTolerance > 255) {
					strInfo = "BoardEdge BoardValueTolerance is error";
					return false;
				}

				if (bEnable_Color2) {
					if (nBoardValue_R2 < 0 || nBoardValue_R2 > 255) {
						strInfo = "BoardEdge BoardValue_R2 is error";
						return false;
					}

					if (nBoardValue_G2 < 0 || nBoardValue_G2 > 255) {
						strInfo = "BoardEdge BoardValue_G2 is error";
						return false;
					}

					if (nBoardValue_B2 < 0 || nBoardValue_B2 > 255) {
						strInfo = "BoardEdge BoardValue_B2 is error";
						return false;
					}

					if (nBoardValueTolerance2 < 0 || nBoardValueTolerance2 > 255) {
						strInfo = "BoardEdge BoardValueTolerance2 is error";
						return false;
					}
				}

				if (bEnable_Color3) {
					if (nBoardValue_R3 < 0 || nBoardValue_R3 > 255) {
						strInfo = "BoardEdge BoardValue_R3 is error";
						return false;
					}

					if (nBoardValue_G3 < 0 || nBoardValue_G3 > 255) {
						strInfo = "BoardEdge BoardValue_G3 is error";
						return false;
					}

					if (nBoardValue_B3 < 0 || nBoardValue_B3 > 255) {
						strInfo = "BoardEdge BoardValue_B3 is error";
						return false;
					}

					if (nBoardValueTolerance3 < 0 || nBoardValueTolerance3 > 255) {
						strInfo = "BoardEdge BoardValueTolerance3 is error";
						return false;
					}
				}

				if (bEnable_Color4) {
					if (nBoardValue_R4 < 0 || nBoardValue_R4 > 255) {
						strInfo = "BoardEdge BoardValue_R4 is error";
						return false;
					}

					if (nBoardValue_G4 < 0 || nBoardValue_G4 > 255) {
						strInfo = "BoardEdge BoardValue_G4 is error";
						return false;
					}

					if (nBoardValue_B4 < 0 || nBoardValue_B4 > 255) {
						strInfo = "BoardEdge BoardValue_B4 is error";
						return false;
					}

					if (nBoardValueTolerance4 < 0 || nBoardValueTolerance4 > 255) {
						strInfo = "BoardEdge BoardValueTolerance4 is error";
						return false;
					}
				}

				// 彩色影像雜訊濾波
				if (nMedianX < 1) {
					strInfo = "BoardEdge MedianX is error";
					return false;
				}

				if (nMedianX < 1) {
					strInfo = "BoardEdge MedianY is error";
					return false;
				}

				// 二值化影像雜訊濾波
				if (nOpenX < 1) {
					strInfo = "BoardEdge OpenX is error";
					return false;
				}

				if (nOpenY < 1) {
					strInfo = "BoardEdge OpenY is error";
					return false;
				}

				// 二值化影像連接濾波
				if (nConnectX < 1) {
					strInfo = "BoardEdge ConnectX is error";
					return false;
				}

				if (nConnectY < 1) {
					strInfo = "BoardEdge ConnectY is error";
					return false;
				}

				// 膨脹濾波Size
				if (nDilateX < 1) {
					strInfo = "BoardEdge DilateX is error";
					return false;
				}

				if (nDilateY < 1) {
					strInfo = "BoardEdge DilateY is error";
					return false;
				}

				// 侵蝕濾波Size
				if (nErosionX < 1) {
					strInfo = "BoardEdge ErosionX is error";
					return false;
				}

				if (nErosionY < 1) {
					strInfo = "BoardEdge ErosionY is error";
					return false;
				}
				return true;
			}
		};

		// 膠邊界參數
		struct SGlueEdge_Parameter
		{
			// 預設為 false
			// 是否啟用膠邊偵測
			bool bEnable;

			// 預設為 1
			// 找黑膠邊界的方法, // 1=>找膠本體(抽色), 2=>找膠輪廓(轉灰階+二值化)
			int nFindEdgeMethod;

			// 1=>左上, 2=>上, 3=>右上, 4=>右, 5=>右下, 6=>下, 7=>左下, 8=>左
			// 9=>整版, 10=>左右各一半 or 上下各一半
			/////////////////////////////
			// 1          2          3 //
			//                         //
			//                         //
			// 8                     4 //
			//                         //
			//                         //
			// 7           6         5 //
			/////////////////////////////
			// 預設為 0
			// 板邊在整個IC的位置(參照上圖), 值域[1,10]
			int nLocation;

			// 預設都為 20 值域[0,255]
			// 膠本體的顏色, 膠本體的RGB值
			int nGlueValue_R;
			int nGlueValue_G;
			int nGlueValue_B;

			// 預設 20, 值域[0,255]
			// 膠本體目標值的誤差範圍範圍
			int nGlueValueTolerance;

			// 預設為 10(自動計算)
			// 膠的閥值, 值域[0,255]
			int nThreshold;

			// 預設為 false, 範圍 : [nThreshold,255](抓亮區)
			// 閥值的方向, 若為true : [0,nThreshold](抓暗區) 
			bool bDark;

			// 預設為 0
			// 膠本體在影像中的位置
			RECT rectGlueROI;

			// 預設為 0.3 值域 (0 , 1.0]
			// 膠本體大小的寬放係數
			// 膠本體大小的判斷範圍 = [GlueWidth*(1-fTolerance) , GlueWidth*(1+fTolerance))] ; [GlueHeight*(1-fTolerance) , GlueHeight*(1+fTolerance))]
			float fGlueROI_Tolerance;

			// 預設都是 1 pixels (1代表不執行), 不可小於1, 最好是奇數
			// 膠灰階影像的雜訊過濾濾波
			int nFilter_MedianX;
			int nFilter_MedianY;

			// 預設都是 1 pixels (1代表不執行), 不可小於1, 最好是奇數
			// 二值化影像過濾雜訊
			int nFilter_OpenX;
			int nFilter_OpenY;

			// 預設都為 21 pixels, 不可小於1, 最好為奇數
			// 膠二值化影像的連接濾波, 膠的邊界並不連續, 需要設定濾波大小來連接, 單位為 pixels
			int nFilter_ConnectX;
			int nFilter_ConnectY;

			// 預設都是 1 pixels (1代表不執行), 不可小於1, 最好是奇數
			// 二值化影像過濾雜訊
			int nFilter_OpenX2;
			int nFilter_OpenY2;

			// 預設都為 1 pixels (1代表不執行), 不可小於1, 最好為奇數
			// 膠邊界的膨脹濾波, 將膠邊界外擴, 單位為 pixels
			int nFilter_DilateX;
			int nFilter_DilateY;

			// 預設都為 21 pixels (1代表不執行), 不可小於1, 最好為奇數
			// 膠邊界的侵蝕濾波, 將膠邊界內縮, 單位為 pixels
			int nFilter_ErosionX;
			int nFilter_ErosionY;

			SGlueEdge_Parameter() :bEnable(false), nFindEdgeMethod(1), nLocation(0), nGlueValue_R(20), nGlueValue_G(20), nGlueValue_B(20), nGlueValueTolerance(20), 
				nThreshold(10), bDark(false), fGlueROI_Tolerance(0.3) , nFilter_MedianX(1), nFilter_MedianY(1), nFilter_ConnectX(21), nFilter_ConnectY(21),	
				nFilter_DilateX(1), nFilter_DilateY(1), nFilter_ErosionX(21), nFilter_ErosionY(21), nFilter_OpenX(1), nFilter_OpenY(1), nFilter_OpenX2(1), nFilter_OpenY2(1)
			{
				rectGlueROI.left = 0;
				rectGlueROI.right = 0;
				rectGlueROI.top = 0;
				rectGlueROI.bottom = 0;
			}

			~SGlueEdge_Parameter() = default;

			bool Check(const int& nImageH, const int& nImageW, string& strInfo) const
			{
				if (!bEnable) return true;
				if (nImageH <= 0 || nImageW <= 0) {
					strInfo = "Image Width or Height is error";
					return false;
				}

				if (nFindEdgeMethod < 1 || nFindEdgeMethod > 2) {
					strInfo = "Glue FindEdgeMethod is error";
					return false;
				}

				// 板邊的位置
				if (nLocation < 1 || nLocation > 10) {
					strInfo = "Glue Location is error";
					return false;
				}

				switch (nFindEdgeMethod)
				{
				case 1: // 找膠本體
					if (nGlueValue_R < 0 || nGlueValue_R > 255) {
						strInfo = "Glue Value_R is error";
						return false;
					}

					if (nGlueValue_G < 0 || nGlueValue_G > 255) {
						strInfo = "Glue Value_G is error";
						return false;
					}

					if (nGlueValue_B < 0 || nGlueValue_B > 255) {
						strInfo = "Glue Value_B is error";
						return false;
					}

					if (nGlueValueTolerance < 0 || nGlueValueTolerance > 255) {
						strInfo = "Glue ValueTolerance is error";
						return false;
					}
					break;

				case 2: // 找膠邊緣
					if (nThreshold < 0 || nThreshold > 255) {
						strInfo = "Glue Threshold is error";
						return false;
					}

					// 中值濾波Size
					if (nFilter_MedianX < 1) {
						strInfo = "Glue MedianX is error";
						return false;
					}

					if (nFilter_MedianY < 1) {
						strInfo = "Glue MedianY is error";
						return false;
					}

					// Open濾波Size
					if (nFilter_OpenX < 1) {
						strInfo = "Glue OpenX is error";
						return false;
					}

					if (nFilter_OpenY < 1) {
						strInfo = "Glue OpenY is error";
						return false;
					}
					
					break;

				default:
					return false;
				}
				
				// 連接濾波Size
				if (nFilter_ConnectX < 1) {
					strInfo = "Glue ConnectX is error";
					return false;
				}

				if (nFilter_ConnectY < 1) {
					strInfo = "Glue ConnectY is error";
					return false;
				}

				// Open2濾波Size
				if (nFilter_OpenX2 < 1) {
					strInfo = "Glue OpenX2 is error";
					return false;
				}

				if (nFilter_OpenY2 < 1) {
					strInfo = "Glue OpenY2 is error";
					return false;
				}

				// 膨脹濾波Size
				if (nFilter_DilateX < 1) {
					strInfo = "Glue DilateX is error";
					return false;
				}

				if (nFilter_DilateY < 1) {
					strInfo = "Glue DilateY is error";
					return false;
				}

				// 侵蝕濾波Size
				if (nFilter_ErosionX < 1) {
					strInfo = "Glue ErosionX is error";
					return false;
				}

				if (nFilter_ErosionY < 1) {
					strInfo = "Glue ErosionY is error";
					return false;
				}

				// 膠ROI Size的寬放係數
				if (fGlueROI_Tolerance <= 0.0 || fGlueROI_Tolerance > 1.0) {
					strInfo = "Glue GlueROI_Tolerance is error";
					return false;
				}

				// 膠的位置
				if (rectGlueROI.left < 0 || rectGlueROI.left >= rectGlueROI.right || rectGlueROI.top < 0 || rectGlueROI.top >= rectGlueROI.bottom ||
					rectGlueROI.right >= nImageW || rectGlueROI.bottom >= nImageH) {
					strInfo = "GlueROI coordinate is error";
					return false;
				}

				return true;
			}
		};

		// Coating邊界參數
		struct SCoatingEdge_Parameter
		{
			// 預設為 false
			// 是否啟用Coating邊界偵測
			bool bEnable;

			// 預設為 1
			// 找Coating邊界的方法, 1=>找膠本體(抽色), 2=>找膠輪廓(轉灰階+二值化)
			int nFindEdgeMethod;

			// 預設為 0
			// 影像中有幾個Coating區域
			int nCount;

			// 預設都為 1 pixels, 不可小於1, 最好為奇數
			// Coating彩色影像的過濾雜訊濾波, 單位為 pixels
			int nFilter_MedianX;
			int nFilter_MedianY;

			// 預設為 R=200, G=200, B=150 值域[0,255]
			// Coating本體的顏色, Coating本體的RGB值
			int nCoatingValue_R;
			int nCoatingValue_G;
			int nCoatingValue_B;

			// 預設 50, 值域[0,255]
			// Coating本體目標值的誤差範圍範圍
			int nCoatingValueTolerance;

			// 預設為 100(自動計算)
			// Coating的閥值, 值域[0,255]
			int nThreshold;

			// 預設為 false, 範圍 : [nThreshold,255](抓亮區)
			// 閥值的方向, 若為true : [0,nThreshold](抓暗區) 
			bool bDark;

			// 預設都是 1 pixels, 不可小於1, 最好為奇數
			// Coating二值化影像過濾邊緣雜訊
			int nFilter_OpenX;
			int nFilter_OpenY;

			// 預設都為 1 pixels, 不可小於1, 最好為奇數
			// Coating二值化影像的連接濾波, Coating本體需要設定濾波大小來連接, 單位為 pixels
			int nFilter_ConnectX;
			int nFilter_ConnectY;

			// 預設都為 1 pixels, 不可小於1, 最好為奇數
			// Coating二值化影像的邊緣平滑濾波, 單位為 pixels
			int nFilter_SmoothX;
			int nFilter_SmoothY;

			// 預設都為 1 pixels (1代表不執行), 不可小於1, 最好為奇數
			// Coating邊界的膨脹濾波, 將膠邊界外擴, 單位為 pixels
			int nFilter_DilateX;
			int nFilter_DilateY;

			// 預設都為 1 pixels (1代表不執行), 不可小於1, 最好為奇數
			// Coating邊界的侵蝕濾波, 將膠邊界內縮, 單位為 pixels
			int nFilter_ErosionX;
			int nFilter_ErosionY;

			SCoatingEdge_Parameter() : bEnable(false), nFindEdgeMethod(1), nCoatingValue_R(200), nCoatingValue_G(200), nCoatingValue_B(150), nCoatingValueTolerance(50),
				nFilter_MedianX(1), nFilter_MedianY(1), nThreshold(100), bDark(false), nFilter_OpenX(1), nFilter_OpenY(1), nFilter_ConnectX(1), nFilter_ConnectY(1), 
				nFilter_SmoothX(1), nFilter_SmoothY(1), nFilter_DilateX(1), nFilter_DilateY(1), nFilter_ErosionX(1), nFilter_ErosionY(1), nCount(0) {}

			~SCoatingEdge_Parameter() = default;

			bool Check(const int& nImageH, const int& nImageW, string& strInfo) const
			{
				if (!bEnable) return true;

				if (nFindEdgeMethod < 1 || nFindEdgeMethod > 2) {
					strInfo = "Coating FindEdgeMethod is error";
					return false;
				}

				if (nCount <= 0) {
					strInfo = "Coating Count is error";
					return false;
				}

				switch (nFindEdgeMethod)
				{
				case 1: // 找Coating本體
					if (nCoatingValue_R < 0 || nCoatingValue_R > 255) {
						strInfo = "Coating Value_R is error";
						return false;
					}

					if (nCoatingValue_G < 0 || nCoatingValue_G > 255) {
						strInfo = "Coating Value_G is error";
						return false;
					}

					if (nCoatingValue_B < 0 || nCoatingValue_B > 255) {
						strInfo = "Coating Value_B is error";
						return false;
					}

					if (nCoatingValueTolerance < 0 || nCoatingValueTolerance > 255) {
						strInfo = "Coating ValueTolerance is error";
						return false;
					}
					break;

				case 2: // 找Coating邊緣
					if (nThreshold < 0 || nThreshold > 255) {
						strInfo = "Coating Threshold is error";
						return false;
					}
					break;

				default:
					return false;
				}

				// 中值濾波Size
				if (nFilter_MedianX < 1) {
					strInfo = "Coating MedianX is error";
					return false;
				}

				if (nFilter_MedianY < 1) {
					strInfo = "Coating MedianY is error";
					return false;
				}

				// Open濾波Size
				if (nFilter_OpenX < 1) {
					strInfo = "Coating OpenX is error";
					return false;
				}

				if (nFilter_OpenY < 1) {
					strInfo = "Coating OpenY is error";
					return false;
				}

				// 連接濾波Size
				if (nFilter_ConnectX < 1) {
					strInfo = "Coating ConnectX is error";
					return false;
				}

				if (nFilter_ConnectY < 1) {
					strInfo = "Coating ConnectY is error";
					return false;
				}

				// Smooth濾波Size
				if (nFilter_SmoothX < 1) {
					strInfo = "Coating SmoothX is error";
					return false;
				}

				if (nFilter_SmoothY < 1) {
					strInfo = "Coating SmoothY is error";
					return false;
				}

				// 膨脹濾波Size
				if (nFilter_DilateX < 1) {
					strInfo = "Coating DilateX is error";
					return false;
				}

				if (nFilter_DilateY < 1) {
					strInfo = "Coating DilateY is error";
					return false;
				}

				// 侵蝕濾波Size
				if (nFilter_ErosionX < 1) {
					strInfo = "Coating ErosionX is error";
					return false;
				}

				if (nFilter_ErosionY < 1) {
					strInfo = "Coating ErosionY is error";
					return false;
				}

				return true;
			}
		};
		
		// 上下限範圍---單一組
		struct SLimit_Single
		{
			// 上限, 預設值 2.0, 單位 um
			float fUpper;

			// 下限, 預設值 1.0, 單位 um
			float fLower;

			SLimit_Single() : fUpper(2.0), fLower(1.0) {}
			SLimit_Single(const float fU, const float& fL) : fUpper(fU), fLower(fL) {}
			~SLimit_Single() = default;

			bool Check(string& strInfo) const {
				if (fUpper < 0.0) {
					strInfo = "Upper less than 0";
					return false;
				}

				if (fLower < 0.0) {
					strInfo = "Lower less than 0";
					return false;
				}

				if (fLower == fUpper) {
					strInfo = "Upper equals Lower";
					return false;
				}

				if (fLower > fUpper) {
					strInfo = "Lower more than Upper";
					return false;
				}

				return true;
			}
		};

		// 單一個 ROI框的 上下限範圍
		struct SLimit_ROI
		{
			// 最大距離的上下限範圍
			SLimit_Single sMaxDist;

			// 最小距離的上下限範圍
			SLimit_Single sMinDist;

			// (最大值-最小值)的上下限範圍
			SLimit_Single sDiff_MaxMin;

			SLimit_ROI() : sMaxDist(), sMinDist(), sDiff_MaxMin() {}
			~SLimit_ROI() = default;

			bool Check(string& strInfo) const
			{
				string strData;
				if (!sMaxDist.Check(strData)) {
					strInfo = "MaxDist Limit-" + strData;
					return false;
				}

				if (!sMinDist.Check(strData)) {
					strInfo = "MinDist Limit-" + strData;
					return false;
				}

				if (!sDiff_MaxMin.Check(strData)) {
					strInfo = "Diff_MaxMin Limit-" + strData;
					return false;
				}

				return true;
			}
		};

		// 多個同類型ROI的參數
		struct SMultipleROI_Parameter
		{
			// 預設為 false
			// 是否啟用膠寬量測
			bool bEnable;

			// 預設為 0
			// 總共有幾個 膠寬ROI
			int nCount;

			// 預設為 1
			// 量測結果取多點平均
			int nMeanCount;

			// 每一個量測ROI在影像中的位置, vtrectROI.size()必須與 nCount相同 
			vector<RECT> vtrectROI;

			// 每一個量測ROI的上下限值, vtsLimit.size()必須與 nCount相同 
			vector<SLimit_ROI> vtsLimit;

			SMultipleROI_Parameter() : bEnable(false), nCount(0), nMeanCount(1) {}
			~SMultipleROI_Parameter() = default;

			bool Check(const int& nImageH, const int& nImageW, string& strInfo) const
			{
				if (bEnable)
				{
					if (nCount <= 0) {
						strInfo += "Count is error";
						return false;
					}

					if (nCount != vtrectROI.size()) {
						strInfo += "vtrectROI is error";
						return false;
					}

					if (nCount != vtsLimit.size()) {
						strInfo += "vtsLimit is error";
						return false;
					}

					if (nMeanCount < 1) {
						strInfo += "MeanCount is error";
						return false;
					}

					// ROI位置
					for (int k = 0; k < nCount; ++k)
					{
						if (vtrectROI[k].left < 0 || vtrectROI[k].left >= vtrectROI[k].right || vtrectROI[k].top < 0 || vtrectROI[k].top >= vtrectROI[k].bottom ||
							vtrectROI[k].right >= nImageW || vtrectROI[k].bottom >= nImageH) {
							strInfo += "ROI" + to_string(k + 1) + " - " + "coordinate is error";
							return false;
						}

						string strData;
						if (!vtsLimit[k].Check(strData))
						{
							strInfo = "ROI" + to_string(k + 1) + "-" + strData;
							return false;
						}
					}
				}

				return true;
			}
		};

		// 單一組 首尾ROI框
		struct SStartEndROI_Single
		{
			// 預設為 0
			// StartROI在影像中的位置
			RECT rectStartROI;

			// 預設為 0
			// EndROI在影像中的位置
			RECT rectEndROI;

			// 1=>左上, 2=>上, 3=>右上, 4=>右, 5=>右下, 6=>下, 7=>左下, 8=>左
			/////////////////////////////
			// 1          2          3 //
			//                         //
			//                         //
			// 8                     4 //
			//                         //
			//                         //
			// 7           6         5 //
			/////////////////////////////

			// 預設為 0, 值域[1,8]
			// StartROI 弧凸起的方向, 方向參照上圖
			int nStartROI_ArcLocation;

			// 預設為 0, 值域[1,8]
			// EndROI 弧凸起的方向, 方向參照上圖
			int nEndROI_ArcLocation;

			// 頭尾距離的上下限值
			SLimit_Single	sLimit;

			SStartEndROI_Single() : nStartROI_ArcLocation(0), nEndROI_ArcLocation(0), sLimit()
			{
				rectStartROI.left = 0;
				rectStartROI.top = 0;
				rectStartROI.right = 0;
				rectStartROI.bottom = 0;

				rectEndROI.left = 0;
				rectEndROI.top = 0;
				rectEndROI.right = 0;
				rectEndROI.bottom = 0;
			}

			~SStartEndROI_Single() = default;

			bool Check(const int& nImageH, const int& nImageW, string& strInfo) const
			{
				// StartROI位置
				if (rectStartROI.left < 0 || rectStartROI.left >= rectStartROI.right || rectStartROI.top < 0 || rectStartROI.top >= rectStartROI.bottom ||
					rectStartROI.right >= nImageW || rectStartROI.bottom >= nImageH) {
					strInfo = "StratROI coordinate is error";
					return false;
				}

				// EndROI位置
				if (rectEndROI.left < 0 || rectEndROI.left >= rectEndROI.right || rectEndROI.top < 0 || rectEndROI.top >= rectEndROI.bottom ||
					rectEndROI.right >= nImageW || rectEndROI.bottom >= nImageH) {
					strInfo = "EndROI coordinate is error";
					return false;
				}

				// StartROI 弧方向
				if (nStartROI_ArcLocation < 1 || nStartROI_ArcLocation > 8) {
					strInfo = "StartROI_ArcLocation is error";
					return false;
				}

				// EndROI 弧方向
				if (nEndROI_ArcLocation < 1 || nEndROI_ArcLocation > 8) {
					strInfo = "EndROI_ArcLocation is error";
					return false;
				}

				string strData;
				if (!sLimit.Check(strData)) {
					strInfo = "StartEndROI-Limit-" + strData;
					return false;
				}

				return true;
			}
		};

		// 首尾ROI的參數
		struct SStartEndROI_Parameter
		{
			// 預設為 false
			// 是否啟用
			bool bEnable;

			// 預設為 1
			// 總共有幾組 膠首尾ROI
			int nCount;

			// 預設為 1
			// 量測結果取多點平均
			int nMeanCount;

			// size 預設為 1
			// vtsStartEnd.size()必須與 nCount相同 
			vector<SStartEndROI_Single> vtsStartEnd;

			SStartEndROI_Parameter() : bEnable(false), nMeanCount(1), nCount(1)
			{
				vtsStartEnd.resize(nCount);
			}

			~SStartEndROI_Parameter() = default;

			bool Check(const int& nImageH, const int& nImageW, string& strInfo) const
			{
				if (bEnable)
				{
					if (nImageH <= 0) {
						strInfo = "ImageH is error";
						return false;
					}

					if (nImageW <= 0) {
						strInfo = "ImageW is error";
						return false;
					}

					if (nCount < 1) {
						strInfo = "StratEndROI Count is error";
						return false;
					}

					if (nMeanCount < 1) {
						strInfo = "MeanCount is error";
						return false;
					}

					if (vtsStartEnd.size() != nCount) {
						strInfo = "StratEndROI Size is error";
						return false;
					}

					string strData;
					for (int k = 0; k < nCount; ++k) {
						if (!vtsStartEnd[k].Check(nImageH, nImageW, strData)) {
							strInfo = "No_" + to_string(k + 1) + "_" + strData;
							return false;
						}
					}
				}

				return true;
			}
		};
#if 0
		struct SStartEndROI_Parameter
		{
			// 預設為 false
			// 是否啟用
			bool bEnable;

			// 預設為 0
			// StartROI在影像中的位置
			RECT rectStartROI;

			// 預設為 0
			// EndROI在影像中的位置
			RECT rectEndROI;

			// 1=>左上, 2=>上, 3=>右上, 4=>右, 5=>右下, 6=>下, 7=>左下, 8=>左
			/////////////////////////////
			// 1          2          3 //
			//                         //
			//                         //
			// 8                     4 //
			//                         //
			//                         //
			// 7           6         5 //
			/////////////////////////////

			// 預設為 0, 值域[1,8]
			// StartROI 弧凸起的方向, 方向參照上圖
			int nStartROI_ArcLocation;

			// 預設為 0, 值域[1,8]
			// EndROI 弧凸起的方向, 方向參照上圖
			int nEndROI_ArcLocation;

			// 頭尾距離的上下限值
			SLimit_Single	sLimit;

			SStartEndROI_Parameter() : bEnable(false), nStartROI_ArcLocation(0), nEndROI_ArcLocation(0), sLimit()
			{
				rectStartROI.left = 0;
				rectStartROI.top = 0;
				rectStartROI.right = 0;
				rectStartROI.bottom = 0;

				rectEndROI.left = 0;
				rectEndROI.top = 0;
				rectEndROI.right = 0;
				rectEndROI.bottom = 0;
			}

			~SStartEndROI_Parameter() = default;

			bool Check(const int& nImageH, const int& nImageW, string& strInfo) const
			{
				if (bEnable)
				{
					// StartROI位置
					if (rectStartROI.left < 0 || rectStartROI.left >= rectStartROI.right || rectStartROI.top < 0 || rectStartROI.top >= rectStartROI.bottom ||
						rectStartROI.right >= nImageW || rectStartROI.bottom >= nImageH) {
						strInfo = "StratROI coordinate is error";
						return false;
					}

					// EndROI位置
					if (rectEndROI.left < 0 || rectEndROI.left >= rectEndROI.right || rectEndROI.top < 0 || rectEndROI.top >= rectEndROI.bottom ||
						rectEndROI.right >= nImageW || rectEndROI.bottom >= nImageH) {
						strInfo = "EndROI coordinate is error";
						return false;
					}

					// StartROI 弧方向
					if (nStartROI_ArcLocation < 1 || nStartROI_ArcLocation > 8) {
						strInfo = "StartROI_ArcLocation is error";
						return false;
					}

					// EndROI 弧方向
					if (nEndROI_ArcLocation < 1 || nEndROI_ArcLocation > 8) {
						strInfo = "EndROI_ArcLocation is error";
						return false;
					}

					string strData;
					if (!sLimit.Check(strData)) {
						strInfo = "StartEndROI-Limit-" + strData;
						return false;
					}
				}

				return true;
			}
		};
#endif

		// 散熱膠量測參數
		// 量測時會先計算 Die的邊界, 再計算膠的邊界
		struct SThermalGlue_Parameter
		{
			// 預設為 false
			// 是否啟用ThermalGlue量測
			bool bEnable;

#pragma region Die

			// 預設為 false
			// 是否使用Die的輪廓重建Die的邊界
			bool bRebuildDie;

			// 預設為 0
			// Die在影像中的位置
			RECT rectDie;

			// 預設為 30 pixels, 不可為負
			// rectDie水平方向再外擴一段距離當做搜尋範圍
			int nDie_OutsetDistanceX;

			// 預設為 30 pixels, 不可為負
			// rectDie垂直方向再外擴一段距離當做搜尋範圍
			int nDie_OutsetDistanceY;

			// 預設都是 1 pixels (1代表不執行), 不可小於1, 最好是奇數
			// Die灰階影像的雜訊過濾濾波
			int nDie_MedianX;
			int nDie_MedianY;

			// 預設為 10,11
			// Die的雙閥值, 值域[0,255]
			int nDie_Threshold_Low;
			int nDie_Threshold_High;

			// 預設為 false
			// Die雙閥值閥值的區間, 若為true : [nDie_Threshold_Low , nDie_Threshold_High] ()
			//                         false : [0,nDie_Threshold_Low] + [nDie_Threshold_High,255]
			bool bDie_Between;

			// 預設都是 1 pixels (1代表不執行), 不可小於1, 最好是奇數
			// Die二值化影像過濾雜訊
			int nDie_OpenX;
			int nDie_OpenY;

			// 預設都是 1 pixels, 不可小於1, 最好是奇數
			// 將Die二值化影像連接起來
			int nDie_ConnectX;
			int nDie_ConnectY;

			// 預設都為 1 pixels (1代表不執行), 不可小於1, 最好為奇數
			// Die邊界的膨脹濾波, 將Die邊界外擴, 單位為 pixels
			int nDie_DilateX;
			int nDie_DilateY;

			// 預設都為 1 pixels (1代表不執行), 不可小於1, 最好為奇數
			// Die邊界的侵蝕濾波, 將Die邊界內縮, 單位為 pixels
			int nDie_ErosionX;
			int nDie_ErosionY;
#pragma endregion

#pragma region Glue

			// 預設為 0
			// Die上有幾條膠, 可呼叫SetGlue_Count()來設定, 
			// 使用 SetGlue_Count()設定時, 會將vtsLimit_Width, vtsLimit_GlueToGlue, vtsLimit_sGlueToDie 進行 resize() 
			int nGlue_Count;

			// 膠寬的上下限值, vtsLimit_Width.size()必須與 nGlue_Count 相同 
			// 建構子會自動 resize()
			vector<SLimit_ROI> vtsLimit_Width;

			// 膠間距的上下限值, vtsLimit_Width.size()必須與 nGlue_Count-1 相同 
			// 建構子會自動 resize()
			vector<SLimit_ROI> vtsLimit_GlueToGlue;

			// 外側膠到Die的上下限值, vtsLimit_Width.size()必須為 2 
			// 建構子會自動 resize()
			vector<SLimit_ROI> vtsLimit_GlueToDie;

			// 預設為 30 (目前只支援2個方向)
			// 膠在Die上的排列方向
			// 1=>水平, 2=>垂直, 
			int nGlue_Direction;

			// 預設為 30 pixels, 不可為負
			// 將計算出來的 Die ROI 水平方向再內縮一段距離當搜尋膠的範圍
			int nDie_InsetDistanceX;

			// 預設為 0 pixels, 不可為負
			// 將計算出來的 Die ROI 垂直方向再內縮一段距離當搜尋膠的範圍
			int nDie_InsetDistanceY;

			// 預設都是 1 pixels (1代表不執行), 不可小於1, 最好是奇數
			// 膠灰階影像的雜訊過濾濾波
			int nGlue_MedianX;
			int nGlue_MedianY;

			// 預設為 10
			// 膠的閥值, 值域[0,255]
			int nGlue_Threshold;

			// 預設為 false, 範圍 : [nThreshold,255](抓亮區)
			// 膠閥值的方向, 若為true : [0,nThreshold](抓暗區) 
			bool bGlue_Dark;

			// 預設都是 1 pixels (1代表不執行), 不可小於1, 最好是奇數
			// 膠二值化影像過濾雜訊
			int nGlue_OpenX;
			int nGlue_OpenY;

			// 預設為 0
			// 量測範圍, 以Die ROI左上角為原點
			RECT rectGlue_MeasurementRange;
#pragma endregion

			SThermalGlue_Parameter() : bEnable(false), bRebuildDie(false), nDie_OutsetDistanceX(30), nDie_OutsetDistanceY(30),
				nDie_MedianX(1), nDie_MedianY(1), nDie_Threshold_Low(10), nDie_Threshold_High(nDie_Threshold_Low+1), bDie_Between(false),
				nDie_OpenX(1), nDie_OpenY(1), nDie_ConnectX(1), nDie_ConnectY(1), nDie_DilateX(1), nDie_DilateY(1), 
				nDie_ErosionX(1), nDie_ErosionY(1), nGlue_Count(0), nGlue_Direction(30), nDie_InsetDistanceX(30), nDie_InsetDistanceY(0),
				nGlue_MedianX(1), nGlue_MedianY(1), nGlue_Threshold(10), bGlue_Dark(false), nGlue_OpenX(1), nGlue_OpenY(1)
			{
				rectDie.left = 0;
				rectDie.top = 0;
				rectDie.right = 0;
				rectDie.bottom = 0;

				rectGlue_MeasurementRange.left = 0;
				rectGlue_MeasurementRange.top = 0;
				rectGlue_MeasurementRange.right = 0;
				rectGlue_MeasurementRange.bottom = 0;
			}

			~SThermalGlue_Parameter() = default;

			// 設定散熱膠的數量
			bool SetGlue_Count(const int& nCount) {
				if (nCount <= 0) return false;
				nGlue_Count = nCount;
				vtsLimit_Width.resize(nGlue_Count);
				if (nCount > 1) {
					vtsLimit_GlueToGlue.resize(nGlue_Count - 1);
				}
				vtsLimit_GlueToDie.resize(2);
				return true;
			}

			bool Check(const int& nImageH, const int& nImageW, string& strInfo) const
			{
				if (!bEnable) return true;

#pragma region Die

				if (rectDie.left < 0 || rectDie.left >= rectDie.right || rectDie.top < 0 || rectDie.top >= rectDie.bottom ||
					rectDie.right >= nImageW || rectDie.bottom >= nImageH) {
					strInfo = "Die ROI coordinate is error";
					return false;
				}

				if (nDie_OutsetDistanceX < 0) {
					strInfo = "Die_OutsetDistanceX is error";
					return false;
				}

				if (nDie_OutsetDistanceY < 0) {
					strInfo = "Die_OutsetDistanceY is error";
					return false;
				}

				if (nDie_MedianX < 1) {
					strInfo = "Die_MedianX is error";
					return false;
				}

				if (nDie_MedianY < 1) {
					strInfo = "Die_MedianY is error";
					return false;
				}

				if (nDie_Threshold_Low < 0 || nDie_Threshold_Low > 255) {
					strInfo = "Die_Threshold_Low is error";
					return false;
				}

				if (nDie_Threshold_High < 0 || nDie_Threshold_High > 255) {
					strInfo = "Die_Threshold_High is error";
					return false;
				}

				if (nDie_OpenX < 1) {
					strInfo = "Die_OpenX is error";
					return false;
				}

				if (nDie_OpenY < 1) {
					strInfo = "Die_OpenY is error";
					return false;
				}

				if (nDie_ConnectX < 1) {
					strInfo = "Die_ConnectX is error";
					return false;
				}

				if (nDie_ConnectY < 1) {
					strInfo = "Die_ConnectY is error";
					return false;
				}	

				if (nDie_DilateX < 1) {
					strInfo = "Die_DilateX is error";
					return false;
				}

				if (nDie_DilateY < 1) {
					strInfo = "Die_DilateY is error";
					return false;
				}

				if (nDie_ErosionX < 1) {
					strInfo = "Die_ErosionX is error";
					return false;
				}

				if (nDie_ErosionY < 1) {
					strInfo = "Die_ErosionY is error";
					return false;
				}

#pragma endregion

#pragma region Glue

				if (nGlue_Count <= 0) {
					strInfo = "Glue_Count is error";
					return false;
				}

				if (vtsLimit_Width.size() != nGlue_Count) {
					strInfo = "GlueWidth_Limit size is error";
					return false;
				}

				if (nGlue_Count > 1) {
					if (vtsLimit_GlueToGlue.size() != nGlue_Count - 1) {
						strInfo = "GlueToGlue_Limit size is error";
						return false;
					}
				}

				if (vtsLimit_GlueToDie.size() != 2) {
					strInfo = "GlueToDie_Limit size is error";
					return false;
				}

				if (nGlue_Direction < 1 || nGlue_Direction > 2) {
					strInfo = "Glue_Direction is error";
					return false;
				}

				if (nDie_InsetDistanceX < 0) {
					strInfo = "Die_InsetDistanceX is error";
					return false;
				}

				if (nDie_InsetDistanceY < 0) {
					strInfo = "Die_InsetDistanceY is error";
					return false;
				}

				if (nGlue_Threshold < 0 || nGlue_Threshold > 255) {
					strInfo = "Die_Threshold is error";
					return false;
				}

				if (nGlue_MedianX < 1) {
					strInfo = "Glue_MedianX is error";
					return false;
				}

				if (nGlue_MedianY < 1) {
					strInfo = "Glue_MedianY is error";
					return false;
				}

				if (nGlue_OpenX < 1) {
					strInfo = "Glue_OpenX is error";
					return false;
				}

				if (nGlue_OpenY < 1) {
					strInfo = "Glue_OpenY is error";
					return false;
				}

				if (rectGlue_MeasurementRange.left < 0 || rectGlue_MeasurementRange.left >= rectGlue_MeasurementRange.right || 
					rectGlue_MeasurementRange.top < 0 || rectGlue_MeasurementRange.top >= rectGlue_MeasurementRange.bottom || 
					rectGlue_MeasurementRange.right >= nImageW || rectGlue_MeasurementRange.bottom >= nImageH) {
					strInfo = "Glue_MeasurementRange coordinate is error";
					return false;
				}
#pragma endregion

				return true;
			}
		};

		// 黑膠量測輸入參數
		struct SMeasurementBlackGlue_Parameter
		{
			// 預設 1.0
			// 水平方向 1 pixels 對應的實際距離, 單位 um
			float fResolutionX;

			// 預設 1.0
			// 垂直方向 1 pixels 對應的實際距離, 單位 um
			float fResolutionY;

			// 板邊參數
			SBoardEdge_Parameter		sBoard;

			// 膠參數
			SGlueEdge_Parameter			sGlue;

			// Coating參數
			SCoatingEdge_Parameter		sCoating;

			// 散熱膠參數
			SThermalGlue_Parameter		sThermal;

			// 垂直膠寬參數
			SMultipleROI_Parameter		sGlueWidth_Vertical;

			// 水平膠寬參數
			SMultipleROI_Parameter		sGlueWidth_Horizontal;

			// 弧線膠寬參數
			SMultipleROI_Parameter		sGlueArc;

			// 垂直方向板邊到膠邊參數
			SMultipleROI_Parameter		sBoardToGlue_Vertical;

			// 水平方向板邊到膠邊參數
			SMultipleROI_Parameter		sBoardToGlue_Horizontal;

			// 垂直方向Coating到膠邊參數
			SMultipleROI_Parameter		sCoatingToGlue_Vertical;

			// 水平方向Coating到膠邊參數
			SMultipleROI_Parameter		sCoatingToGlue_Horizontal;

			// 垂直方向Coating到板邊參數
			SMultipleROI_Parameter		sCoatingToBoard_Vertical;

			// 水平方向Coating到板邊參數
			SMultipleROI_Parameter		sCoatingToBoard_Horizontal;
	
			// 首尾ROI參數
			SStartEndROI_Parameter		sGlueStartEnd;

			SMeasurementBlackGlue_Parameter() : fResolutionX(1.0), fResolutionY(1.0), sBoard(), sGlue(), sGlueWidth_Vertical(), sGlueWidth_Horizontal(), 
				sGlueArc(), sGlueStartEnd(), sBoardToGlue_Vertical(), sBoardToGlue_Horizontal(), sCoatingToGlue_Vertical(), sCoatingToGlue_Horizontal() {}

			~SMeasurementBlackGlue_Parameter() = default;

			bool Check(const int& nImageH, const int& nImageW, string& strInfo) const
			{
				strInfo = "OK";
				if (fResolutionX <= 0.0) {
					strInfo = "ResolutionX is error";
					return false;
				}

				if (fResolutionY <= 0.0) {
					strInfo = "ResolutionY is error";
					return false;
				}

				if (nImageH <= 0) {
					strInfo = "Image Height is error";
					return false;
				}

				if (nImageW <= 0) {
					strInfo = "Image Width is error";
					return false;
				}

				if (!sBoard.Check(strInfo)) {
					return false;
				}

				if (!sGlue.Check(nImageH, nImageW, strInfo)) {
					return false;
				}

				if (!sCoating.Check(nImageH, nImageW, strInfo)) {
					return false;
				}

				if (!sThermal.Check(nImageH, nImageW, strInfo)) {
					return false;
				}
				
				string strData = "GlueWidth_Vertical ";
				if (!sGlueWidth_Vertical.Check(nImageH, nImageW, strData)) {
					strInfo = strData;
					return false;
				}

				strData = "GlueWidth_Horizontal ";
				if (!sGlueWidth_Horizontal.Check(nImageH, nImageW, strData)) {
					strInfo = strData;
					return false;
				}

				strData = "BoardToGlue_Vertical ";
				if (!sBoardToGlue_Vertical.Check(nImageH, nImageW, strData)) {
					strInfo = strData;
					return false;
				}

				strData = "BoardToGlue_Horizontal ";
				if (!sBoardToGlue_Horizontal.Check(nImageH, nImageW, strData)) {
					strInfo = strData;
					return false;
				}

				strData = "CoatingToGlue_Vertical ";
				if (!sCoatingToGlue_Vertical.Check(nImageH, nImageW, strData)) {
					strInfo = strData;
					return false;
				}

				strData = "CoatingToGlue_Horizontal ";
				if (!sCoatingToGlue_Horizontal.Check(nImageH, nImageW, strData)) {
					strInfo = strData;
					return false;
				}

				strData = "GlueArc ";
				if (!sGlueArc.Check(nImageH, nImageW, strData)) {
					strInfo = strData;
					return false;
				}
				
				if (!sGlueStartEnd.Check(nImageH, nImageW, strInfo)) {
					return false;
				}

				return true;
			}
		};
#pragma endregion

#pragma region Black Glue Result

	// 單點量測結果
	struct SMeasurementResult
	{
		// 預設為 0
		// 量測起始點位(pixels)
		POINT ptStart;

		// 預設為 0
		// 量測終止點位(pixels)
		POINT ptEnd;

		// 預設為 0
		// 兩點的直線距離(um)
		float fDistance;

		SMeasurementResult() : fDistance(0.0) {
			ptStart.x = 0;
			ptStart.y = 0;
			ptEnd.x = 0;
			ptEnd.y = 0;
		}

		~SMeasurementResult() = default;

		void Initial() {
			ptStart.x = 0;
			ptStart.y = 0;
			ptEnd.x = 0;
			ptEnd.y = 0;
			fDistance = 0.0;
		}
	};

	// 板邊偵測結果
	struct SBoardEdge_Result
	{
		// 預設為 false
		// 是否有量測板邊
		bool bEnable;

		// 預設為 0.0
		// 板邊水平線斜率
		double	dSlope_Horizontal;

		// 預設為 99999.0 
		// 板邊水平線捷距
		double	dIntercept_Horizontal;
		double	dIntercept_Horizontal2;

		// 預設為 0.0 
		// 板邊垂直線斜率
		double  dSlope_Vertical;

		// 預設為 0.0 
		// 板邊垂直線捷距
		double  dIntercept_Vertical;
		double  dIntercept_Vertical2;

		// 預設為 0
		// 輪廓點數
		int nContoursCount;

		// 板邊的輪廓點
		// vtptBorderContoursPos.size()必須與nContoursCount相同
		vector<POINT> vtptContoursPos;

		// 垂直線點位
		SMeasurementResult sLine_Vertical;
		SMeasurementResult sLine_Vertical2;

		// 水平線點位
		SMeasurementResult sLine_Horizontal;
		SMeasurementResult sLine_Horizontal2;

		SBoardEdge_Result() : bEnable(false), dSlope_Horizontal(0.0), dIntercept_Horizontal(0.0), dIntercept_Horizontal2(0.0), 
			dSlope_Vertical(99999.0), dIntercept_Vertical(0.0), dIntercept_Vertical2(0.0), nContoursCount(0),
			sLine_Vertical(), sLine_Vertical2(), sLine_Horizontal(), sLine_Horizontal2() {}
		~SBoardEdge_Result() = default;

		void Initial()
		{
			bEnable = false;
			dSlope_Horizontal = 0.0;
			dIntercept_Horizontal = 0.0;
			dIntercept_Horizontal2 = 0.0;
			dSlope_Vertical = 99999.0;
			dIntercept_Vertical = 0.0;
			dIntercept_Vertical2 = 0.0;
			nContoursCount = 0;
			vtptContoursPos.clear();
			sLine_Vertical.Initial();
			sLine_Vertical2.Initial();
			sLine_Horizontal.Initial();
			sLine_Horizontal2.Initial();
		}
	};

	// 膠邊偵測結果
	struct SGlueEdge_Result
	{
		// 預設為 false
		// 是否有量測膠邊
		bool bEnable;

		// 預設為 0
		// 膠本體 ROI
		RECT  rectROI;

		// 預設為 0
		// 輪廓點數
		int nContoursCount;

		// 膠的輪廓點
		// vtptContoursPos.size()必須與 nContoursCount 相同
		vector<POINT>  vtptContoursPos;

		SGlueEdge_Result() : bEnable(false), nContoursCount(0) {
			rectROI.left = 0;
			rectROI.top = 0;
			rectROI.right = 0;
			rectROI.bottom = 0;
		}

		~SGlueEdge_Result() = default;

		void Initial()
		{
			bEnable = false;
			rectROI.left = 0;
			rectROI.top = 0;
			rectROI.right = 0;
			rectROI.bottom = 0;
			nContoursCount = 0;
			vtptContoursPos.clear();
		}
	};

	// Coating邊偵測結果
	struct SCoatingEdge_Result
	{
		// 預設為 false
		// 是否有量測膠邊
		bool bEnable;

		// 預設為 0
		// 有多少個 Coating區域
		int nCoatingCount;

		// 預設為 0
		// 每一個 Coating的ROI位置, nCoatingCount = vtrectCoatingROI.size()
		vector<RECT>  vtrectCoatingROI;

		// 預設為 0
		// 每一個 Coating的輪廓點數, nCoatingCount = vtnContoursCount.size()
		vector<int> vtnContoursCount;

		// 每一個 Coating的輪廓點位
		// vtptContoursPos.size()必須與 nCoatingCount 相同
		// vtptContoursPos[].size()必須與 nContoursCount 相同
		vector<vector<POINT>>  vt2ptContoursPos;

		SCoatingEdge_Result() : bEnable(false), nCoatingCount(0) {

		}

		~SCoatingEdge_Result() = default;

		void Initial()
		{
			bEnable = false;
			nCoatingCount = 0;
			vtrectCoatingROI.clear();
			vtnContoursCount.clear();
			vt2ptContoursPos.clear();
		}
	};

	// 單一個ROI量測結果
	struct SSingleROI_Result
	{
		// 預設為 0
		// 量測範圍
		RECT rectROI;

		// 預設為 0
		// 量測數
		int nPositionCount;

		// 預設為空
		// 所有點位的量測資訊, vtsPositionInfo.size()必須與 nPositionCount相同
		vector<SMeasurementResult> vtsPositionInfo;

		// 預設為 false
		// 最長距離 量測結果, true=OK, false=NG
		bool bResult_MaxDist;
		SMeasurementResult sMaxDist;

		// 預設為 false
		// 最短距離 量測結果, true=OK, false=NG
		bool bResult_MinDist;
		SMeasurementResult sMinDist;

		// 預設為 false
		// (最長距離-最短距離) 量測結果, true=OK, false=NG
		bool bResult_Diff_MaxMin;

		// 預設值 0.0
		// (最長距離-最短距離) 單位 um 
		float			   fDiff_Dist_MaxMin;

		SSingleROI_Result() : nPositionCount(0), bResult_MaxDist(false), sMaxDist(), bResult_MinDist(false), sMinDist(), bResult_Diff_MaxMin(false), fDiff_Dist_MaxMin(0.0) {
			rectROI.left = 0;
			rectROI.top = 0;
			rectROI.right = 0;
			rectROI.bottom = 0;
		}

		~SSingleROI_Result() = default;

		void Initial() {
			rectROI.left = 0;
			rectROI.top = 0;
			rectROI.right = 0;
			rectROI.bottom = 0;
			nPositionCount = 0;
			vtsPositionInfo.clear();		
			bResult_MaxDist = false;
			sMaxDist.Initial();
			bResult_MinDist = false;
			sMinDist.Initial();
			bResult_Diff_MaxMin = false;
			fDiff_Dist_MaxMin = 0.0;
		}
	};

	// 多個同類型ROI的量測結果
	struct SMultipleROI_Result
	{
		// 預設為 false
		// 是否量測
		bool bEnable;

		// 預設為 0
		// ROI數量
		int nRoiCount;

		// 所有ROI的量測結果, nRoiCount = vtsRoiInfo.size()
		vector<SSingleROI_Result>	vtsRoiInfo;

		SMultipleROI_Result() : bEnable(false), nRoiCount(0) {}
		~SMultipleROI_Result() = default;

		void Initial()
		{
			bEnable = false;
			nRoiCount = 0;
			vtsRoiInfo.clear();
		}
	};

	// 單一個ROI量測結果
	struct SSingleStartEndROI_Result
	{
		// Start ROI的位置
		RECT rectStart;

		// End ROI 的位置
		RECT rectEnd;

		// 預設為 false
		// 最長距離 量測結果, true=OK, false=NG
		bool bResult_MaxDist;

		SMeasurementResult sDist;

		SSingleStartEndROI_Result() : bResult_MaxDist(false), sDist()
		{
			rectStart.left = 0;
			rectStart.top = 0;
			rectStart.right = 0;
			rectStart.bottom = 0;

			rectEnd.left = 0;
			rectEnd.top = 0;
			rectEnd.right = 0;
			rectEnd.bottom = 0;
		}

		~SSingleStartEndROI_Result() = default;

		void Initial()
		{
			rectStart.left = 0;
			rectStart.top = 0;
			rectStart.right = 0;
			rectStart.bottom = 0;
			rectEnd.left = 0;
			rectEnd.top = 0;
			rectEnd.right = 0;
			rectEnd.bottom = 0;
			bResult_MaxDist = false;
		}
	};

	// 首尾ROI距離量測結果
	struct SStartEndROI_Result
	{
		// 預設為 false
		// 是否有進行量測
		bool bEnable;

		// 預設為 0
		// StartEndROI組數
		int nCount;

		// 所有ROI的量測結果, nCount = vtsStartEnd.size()
		vector<SSingleStartEndROI_Result> vtsStartEnd;

		SStartEndROI_Result() : bEnable(false), nCount(0)
		{
			vtsStartEnd.clear();
		}

		~SStartEndROI_Result() = default;

		void Initial()
		{
			bEnable = false;
			nCount = 0;
			vtsStartEnd.clear();
		}
	};

	// 散熱膠量測結果
	struct SThermalGlue_Result
	{
		// 預設為 false
		// 是否有進行量測
		bool bEnable;

		// Die 的位置
		RECT rectDie;

		// Die 的 四個角點
		vector<POINT> vtptDieCorner;

		// 預設為 0
		// 散熱膠的數量, nGlueCount = SThermalGlue_Parameter.nGlue_Count
		int nGlue_Count;

		// 散熱膠邊緣偵測結果, vtsGlueEdgeInfo.size() = nGlueCount
		vector<SGlueEdge_Result> vtsGlueEdgeInfo;

		// 膠寬量測結果
		SMultipleROI_Result sGlueWidth;

		// 膠到膠的距寬量測結果
		SMultipleROI_Result sGlueToGlue;

		// 膠到Die的距離量測結果
		SMultipleROI_Result sGlueToDie;

		SThermalGlue_Result() : bEnable(false), nGlue_Count(0), sGlueWidth(), sGlueToGlue(), sGlueToDie() {
			rectDie.left = 0;
			rectDie.top = 0;
			rectDie.right = 0;
			rectDie.bottom = 0;
		}

		~SThermalGlue_Result() = default;

		void Initial()
		{
			bEnable = false;
			rectDie.left = 0;
			rectDie.top = 0;
			rectDie.right = 0;
			rectDie.bottom = 0;
			nGlue_Count = 0;
			vtsGlueEdgeInfo.clear();
			vtptDieCorner.clear();
			sGlueWidth.Initial();
			sGlueToGlue.Initial();
			sGlueToDie.Initial();
		}

		// 設定散熱膠的數量
		bool SetGlue_Count(const int& nCount) {
			if (nCount <= 0) return false;
			vtsGlueEdgeInfo.resize(nCount);
			sGlueWidth.nRoiCount = nCount;
			sGlueWidth.vtsRoiInfo.resize(nCount);
			if (nCount > 1) {
				sGlueToGlue.nRoiCount = nCount - 1;
				sGlueToGlue.vtsRoiInfo.resize(nCount - 1);
			}
			sGlueToDie.nRoiCount = 2;
			sGlueToDie.vtsRoiInfo.resize(2);
			return true;
		}
	};

	// 黑膠量測輸出結果
	struct SMeasurementBlackGlue_Result
	{
		// 板邊偵測結果
		SBoardEdge_Result				sBoardEdge;

		// 膠邊偵測結果
		SGlueEdge_Result				sGlueEdge;

		SGlueEdge_Result				sGlueEdge2;

		// Coating偵測結果
		SCoatingEdge_Result				sCoatingEdge;

		// 膠首尾距離量測結果
		SStartEndROI_Result				sStartEnd;

		// 水平膠寬量測結果
		SMultipleROI_Result				sWidth_Horizontal;

		// 垂直膠寬量測結果
		SMultipleROI_Result				sWidth_Vertical;

		// 弧線膠寬量測結果
		SMultipleROI_Result				sWidth_Arc;

		// 水平方向板邊到膠邊量測結果
		SMultipleROI_Result				sBoardToGlue_Horizontal;

		// 垂直方向板邊到膠邊量測結果
		SMultipleROI_Result				sBoardToGlue_Vertical;

		// 水平方向Coating到膠邊量測結果(bResult_Diff_MaxMin 與 bResult_MaxDist演算法會強制判 OK)
		SMultipleROI_Result				sCoatingToGlue_Horizontal;

		// 垂直方向Coating到膠邊量測結果(bResult_Diff_MaxMin 與 bResult_MaxDist演算法會強制判 OK)
		SMultipleROI_Result				sCoatingToGlue_Vertical;

		// 水平方向Coating邊到板邊量測結果(bResult_Diff_MaxMin 與 bResult_MaxDist演算法會強制判 OK)
		SMultipleROI_Result				sCoatingToBoard_Horizontal;

		// 垂直方向Coating邊到板邊量測結果(bResult_Diff_MaxMin 與 bResult_MaxDist演算法會強制判 OK)
		SMultipleROI_Result				sCoatingToBoard_Vertical;

		// 散熱膠量測結果
		SThermalGlue_Result				sThermalGlue;

		SMeasurementBlackGlue_Result() : sBoardEdge(), sGlueEdge(), sStartEnd(), sWidth_Horizontal(), sWidth_Vertical(), sWidth_Arc(),
			sBoardToGlue_Horizontal(), sBoardToGlue_Vertical(), sCoatingToGlue_Horizontal(), sCoatingToGlue_Vertical() {}

		~SMeasurementBlackGlue_Result() = default;

		void Initial() {
			sBoardEdge.Initial();
			sGlueEdge.Initial();
			sCoatingEdge.Initial();
			sStartEnd.Initial();
			sWidth_Horizontal.Initial();
			sWidth_Vertical.Initial();
			sBoardToGlue_Horizontal.Initial();
			sBoardToGlue_Vertical.Initial();
			sWidth_Arc.Initial();
			sCoatingToGlue_Horizontal.Initial();
			sCoatingToGlue_Vertical.Initial();
			sThermalGlue.Initial();
		}
	};

#pragma endregion

#pragma region Flux Area algorithm

	// 量測 Flux面積的參數
	struct SMeasurementFluxArea_Parameter
	{
		// 預設 10.0
		// 水平方向 1 pixels 對應的實際距離, 單位:um
		float fResolutionX;

		// 預設 10.0
		// 垂直方向 1 pixels 對應的實際距離, 單位:um
		float fResolutionY;

		// 預設 為0.0
		// 量測總面積(單位:um^2)
		float fTotalArea;

		// 面積上下限值
		SLimit_Single sLimit;

		// Flux邊界參數
		SFindEdge_Parameter		sFluxEdge;

		SMeasurementFluxArea_Parameter() : fResolutionX(10.0), fResolutionY(10.0), fTotalArea(0.0), sLimit(1.0f, 0.0f), sFluxEdge() {}
		~SMeasurementFluxArea_Parameter() = default;

		bool Check(string& strInfo) const
		{
			if (fResolutionX <= 0.0) {
				strInfo = "ResolutionX is error";
				return false;
			}

			if (fResolutionY <= 0.0) {
				strInfo = "ResolutionY is error";
				return false;
			}

			if (fTotalArea <= 0.0) {
				strInfo = "量測面積設定錯誤";
				return false;
			}

			string strData;
			if (!sLimit.Check(strData)) {
				strInfo = "上下限參數_" + strData;
				return false;
			}

			if (!sFluxEdge.Check(strData)) {
				strInfo = "Flux邊界參數_" + strData;
				return false;
			}

			return true;
		}
	};

	// Flux面積的量測結果
	struct SMeasurementFluxArea_Result
	{
		// 預設為 false
		// 量測結果, true=OK, false=NG
		bool bResult;

		// Flux面積(單位:um^2)
		float fFluxArea;

		// 面積比
		float fAreaRatio;

		// Flux邊界相關資訊
		SFindEdge_Result sFluxEdge;

		SMeasurementFluxArea_Result() : bResult(false), fFluxArea(0.0), fAreaRatio(0.0), sFluxEdge() {}
		~SMeasurementFluxArea_Result() = default;

		void Initial()
		{
			bResult = false;
			fFluxArea = 0.0;
			fAreaRatio = 0.0;
			sFluxEdge.Initial();
		}
	};

#pragma endregion

#pragma endregion

#pragma region 2D Algorithm

#pragma region Glue HeightRatio algorithm

		// 量測膠高比例參數
		struct SMeasurementGlueHeightRatio_Parameter
		{
			// 預設為 true
			// 是否重建 IC 邊界
			bool bRebuilding_ICEdge;

			// 預設是 0
			//     3
			// 2       4
			//     1
			// 量測方向, 以IC邊界為出發點
			int nMeasuringDirection;

			// 預設是 1
			// 量測方式, 1=>IC(輪廓搜尋+線性回歸), 2=>Connector(外接矩形+線性回歸), 3=>Rectangular(外接矩形)
			int nMeasuringObject;

			// 預設 10.0
			// 水平方向 1 pixels 對應的實際距離, 單位:um
			float fResolutionX;

			// 預設 10.0
			// 垂直方向 1 pixels 對應的實際距離, 單位:um
			float fResolutionY;

			// 預設  0.0
			// IC 實際高度(單位:um)
			float fIC_Height;

			// 預設 10 pixels
			// IC邊界的搜尋範圍
			int nIC_BaseLine_SearchRange;

			// 預設為 0
			// IC邊界左上_頭尾內縮的距離, 單位 pixels
			int  nICEdge_Indent_Left_Top;

			// 預設為 0
			// IC邊界右下_頭尾內縮的距離, 單位 pixels
			int  nICEdge_Indent_Right_Bottom;

			// 預設為 1
			// 區塊數
			int nBlockCount;

			// 高度算法模式, 預設 size() = 1, 值為1
			// vtnHeightMode.size() 必須等於 nBlockCount
			// 1=>最高, 2=>平均, 3=>最低, 4=>最高-最低, 5=>平均-最低
			vector<int>				vtnHeightMode;

			// 預設值為 1
			// 統計各個區塊的 高度算法模式
			int nFinal_HeightMode;

			// 預設上限為100, 下限為25
			// 設定最終結果的上下限值
			SLimit_Single   sLimit;

			// 膠邊界參數
			SFindEdge_Parameter		sGlueEdge;

			// IC邊界參數
			SFindEdge_Parameter		sIcEdge;

			SMeasurementGlueHeightRatio_Parameter() : nMeasuringDirection(0), bRebuilding_ICEdge(true), nICEdge_Indent_Left_Top(0), 
				nICEdge_Indent_Right_Bottom(0), nBlockCount(1), nFinal_HeightMode(1), fIC_Height(0.0), nIC_BaseLine_SearchRange(10), 
				sLimit(100.0f, 25.0f), sGlueEdge(), sIcEdge(), fResolutionX(10.0), fResolutionY(10.0), nMeasuringObject(1)
			{
				vtnHeightMode.resize(nBlockCount, 1);
			}

			~SMeasurementGlueHeightRatio_Parameter() = default;

			bool Check(string& strInfo) const
			{
				strInfo = "OK";

				if (nMeasuringDirection < 1 || nMeasuringDirection > 4) {
					strInfo = "量測方向設定錯誤";
					return false;
				}

				if (nMeasuringObject < 1 || nMeasuringObject > 3) {
					strInfo = "量測方式設定錯誤";
					return false;
				}

				if (fIC_Height <= 0.0) {
					strInfo = "IC高度未設定";
					return false;
				}

				if (fResolutionX <= 0.0) {
					strInfo = "ResolutionX is error";
					return false;
				}

				if (fResolutionY <= 0.0) {
					strInfo = "ResolutionY is error";
					return false;
				}

				if (nBlockCount < 1) {
					strInfo = "區塊數量不可小於1";
					return false;
				}

				if (nIC_BaseLine_SearchRange < 0) {
					strInfo = "IC_BaseLine 搜尋範圍不可小於0";
					return false;
				}

				if (nICEdge_Indent_Left_Top < 0) {
					strInfo = "ICEdge_Indent IC邊界內左(上)縮距離不可小於0";
					return false;
				}

				if (nICEdge_Indent_Right_Bottom < 0) {
					strInfo = "ICEdge_Indent IC邊界內右(下)縮距離不可小於0";
					return false;
				}

				if (nFinal_HeightMode < 1 || nFinal_HeightMode > 5) {
					strInfo = "最終高度算法設定值超出範圍";
					return false;
				}

				if (vtnHeightMode.size() != nBlockCount) {
					strInfo = "OutputMode 數量與 nBlockCount 不相等";
					return false;
				}
				
				if (!sLimit.Check(strInfo)) {
					return false;
				}

				string strData;
				for (int k = 0; k < nBlockCount; ++k) {
					if (vtnHeightMode[k] < 1 || vtnHeightMode[k] > 5) {
						strInfo = "第" + to_string(k + 1) + "區塊_高度算法模式設定超出範圍";
						return false;
					}
				}
				
				if (!sGlueEdge.Check(strData)) {
					strInfo = "膠邊界參數_" + strData;
					return false;
				}

				if (!sIcEdge.Check(strData)) {
					strInfo = "IC邊界參數_" + strData;
					return false;
				}

				return true;
			}
		};

		// 單一pixels量測資訊
		struct Pixels_MeasurementInfo
		{
			// 預設為 0
			// 量測起始點位(pixels)
			POINT ptStart;

			// 預設為 0
			// 量測終止點位(pixels)
			POINT ptEnd;

			float fDist_ICtoGlue;	// IC邊到膠邊的距離
			float fGlueHeight;		// 膠高
			float fHeightRatio;		// 膠的高度比例

			Pixels_MeasurementInfo() : fDist_ICtoGlue(0.0), fGlueHeight(0.0), fHeightRatio(0.0)
			{
				ptStart.x = 0;
				ptStart.y = 0;
				ptEnd.x = 0;
				ptEnd.y = 0;
			}

			~Pixels_MeasurementInfo() = default;

			void Initial() {
				ptStart.x = 0;
				ptStart.y = 0;
				ptEnd.x = 0;
				ptEnd.y = 0;
				fDist_ICtoGlue = 0.0;
				fGlueHeight = 0.0;
				fHeightRatio = 0.0;
			}
		};

		// 單一區塊量測資訊
		struct Block_MeasurementInfo
		{
			// 單一區塊中有多少pixels
			int nPixelsCount;

			// 每一pixels的量測結果
			vector<Pixels_MeasurementInfo> vtsPixels;

			Block_MeasurementInfo() : nPixelsCount(0) {}

			~Block_MeasurementInfo() = default;

			void Initial() {
				nPixelsCount = 0;
				vtsPixels.clear();
			}
		};

		// 單一區塊 膠高量測結果
		struct SingleBlockGlueHeight_Result
		{
			float fHeightRatio;

			// 第一筆統計資訊
			Pixels_MeasurementInfo sFirst;

			//第二筆統計資訊
			Pixels_MeasurementInfo sSecond;

			// 區塊中每一pixels的量測資訊
			Block_MeasurementInfo sInfo;

			SingleBlockGlueHeight_Result() : fHeightRatio(0.0), sFirst(), sSecond(), sInfo() {}
			~SingleBlockGlueHeight_Result() = default;

			void Initial() {
				fHeightRatio = 0.0;
				sFirst.Initial();
				sSecond.Initial();
				sInfo.Initial();
			}

			void Assign(const int nType, const Pixels_MeasurementInfo& sPixels)
			{
				switch (nType) {
				case 1:
					sFirst = sPixels;
					break;
				case 2:
					sSecond = sPixels;
					break;
				}
			}
		};

		// 多區塊 膠高量測結果
		struct MultipleBlockGlueHeight_Result 
		{
			// 預設為 false
			// 最終量測結果
			bool bFinalResult;

			// 多區塊的統計結果
			float fFinalHeightRatio;

			// 最終第一筆統計資訊
			Pixels_MeasurementInfo sFinalFirst;

			// 最終第二筆統計資訊
			Pixels_MeasurementInfo sFinalSecond;

			// 總區塊數
			int nBlockCount;	

			// 各區塊每一pixels的量測結果
			vector<SingleBlockGlueHeight_Result> vtsBlock;	
			
			MultipleBlockGlueHeight_Result() : bFinalResult(false), fFinalHeightRatio(0.0), sFinalFirst(), sFinalSecond(), nBlockCount(0) {}
			~MultipleBlockGlueHeight_Result() = default;

			void Initial() {
				bFinalResult = false;
				fFinalHeightRatio = 0.0;
				sFinalFirst.Initial();
				sFinalSecond.Initial();
				nBlockCount = 0;
				vtsBlock.clear();
			}
		};

		// 膠高比例量測結果
		struct SMeasurementGlueHeightRatio_Result
		{
			// 膠邊界相關資訊
			SFindEdge_Result sGlueEdge;

			// IC邊界相關資訊
			SFindEdge_Result sIcEdge;

			// 各區塊膠高量測資訊
			MultipleBlockGlueHeight_Result sHeight;

			void Initial()
			{
				sGlueEdge.Initial();
				sIcEdge.Initial();
				sHeight.Initial();
			}
		};
#pragma endregion

#pragma region Glue Area algorithm

		// 膠面積偵測參數
		struct SMeasurementGlueArea_Parameter
		{
			// 預設 10.0
			// 水平方向 1 pixels 對應的實際距離, 單位:um
			float fResolutionX;

			// 預設 10.0
			// 垂直方向 1 pixels 對應的實際距離, 單位:um
			float fResolutionY;

			// 預設為 0
			// IC的標準寬度 (um)
			int nIC_Width;

			// 預設為 0
			// IC的標準高度 (um)
			int nIC_Height;

			// 預設為 0
			// IC寬度 內縮距離(um)
			// 計算出IC範圍後, 再依設定值內縮
			int nIC_Indent_Width;

			// 預設為 0
			// IC高度 內縮距離(um)
			// 計算出IC範圍後, 再依設定值內縮
			int nIC_Indent_Height;

			// 預設為 50 (um)
			// 膠的最小寬度, 單位:um
			int nGlueSize_MinWidth;

			// 預設為 50 (um)
			// 膠的最小高度, 單位:um
			int nGlueSize_MinHeight;	

			// 預設為 1
			// 膠的寬跟高都要大於最小設定值, 才會被計數
			// 膠的數量 >= 此值會報NG
			int nGlueNG_Count;

			// 預設為 100 (um)
			// 任一膠寬大於等於此設定值則判NG, 單位:um
			int nGlueNG_Width;

			// 預設為 100 (um)
			// 任一膠高大於等於此設定值則判NG, 單位:um
			int nGlueNG_Height;

			// 預設為 0 (um)
			// IC 寬度差多少 um 要判 NG
			int nIC_NG_DiffWidth;

			// 預設為 0 (um)
			// IC 高差多少 um 要判 NG
			int nIC_NG_DiffHeight;

			// IC界參數
			SFindEdge_Parameter		sIcEdge;

			// 膠邊界參數
			SFindEdge_Parameter		sGlueEdge;

			SMeasurementGlueArea_Parameter() : fResolutionX(10.0), fResolutionY(10.0), nIC_Width(0), nIC_Height(0), nIC_Indent_Width(0), nIC_Indent_Height(0),
				nGlueSize_MinWidth(50), nGlueSize_MinHeight(50), nGlueNG_Count(1), sIcEdge(), sGlueEdge(), nGlueNG_Width(100), nGlueNG_Height(100),
				nIC_NG_DiffWidth(0), nIC_NG_DiffHeight(0) {}
			~SMeasurementGlueArea_Parameter() = default;

			bool Check(string& strInfo) const
			{
				if (fResolutionX <= 0.0) {
					strInfo = "ResolutionX is error";
					return false;
				}

				if (fResolutionY <= 0.0) {
					strInfo = "ResolutionY is error";
					return false;
				}

				if (nIC_Width <= 0) {
					strInfo = "IC面寬度定錯誤";
					return false;
				}

				if (nIC_Height <= 0) {
					strInfo = "IC面高度設定錯誤";
					return false;
				}

				if (nIC_Indent_Width < 0) {
					strInfo = "IC寬度內縮值不可小於0";
					return false;
				}

				if (nIC_Indent_Width >= nIC_Width/5) {
					strInfo = "IC寬度內縮值過大";
					return false;
				}

				if (nIC_Indent_Height < 0) {
					strInfo = "IC高度內縮值不可小於0";
					return false;
				}

				if (nIC_Indent_Height >= nIC_Height / 5) {
					strInfo = "IC高度內縮值過大";
					return false;
				}

				if (nGlueSize_MinWidth < 1) {
					strInfo = "膠的最小寬度不可小於1";
					return false;
				}

				if (nGlueSize_MinHeight < 1) {
					strInfo = "膠的最小高度不可小於1";
					return false;
				}

				if (nGlueNG_Count < 1) {
					strInfo = "膠 NG數量不可小於1";
					return false;
				}	

				if (nGlueNG_Width < 1) {
					strInfo = "膠 NG寬度不可小於1";
					return false;
				}

				if (nGlueNG_Height < 1) {
					strInfo = "膠 NG高度不可小於1";
					return false;
				}

				if (nIC_NG_DiffWidth < 0) {
					strInfo = "IC寬度差異值不可小於0";
					return false;
				}

				if (nIC_NG_DiffHeight < 0) {
					strInfo = "IC高度差異值不可小於0";
					return false;
				}

				string strData;
				if (!sIcEdge.Check(strData)) {
					strInfo = "IC邊界參數_" + strData;
					return false;
				}

				if (!sGlueEdge.Check(strData)) {
					strInfo = "膠邊界參數_" + strData;
					return false;
				}

				return true;
			}
		};

		// 找範圍結果
		struct SFindRange_Result
		{
			// 預設 size=4, 空陣列
			vector<POINT> vtptCorners;

			// 預設為 0 (um)
			int nIC_Width;

			// 預設為 0 (um)
			int nIC_Height;

			// 預設size=4, 值為0 ; 0=上, 1=下, 2=左, 3=右
			// 輪廓 Pixels數
			vector<int> vtnContoursCount;

			// 預設size=4, 空陣列 ; 0=上, 1=下, 2=左, 3=右
			// 膠輪廓的點位座標
			// vt2ptContoursPos[k].size()必須與 vtnContoursCount[k] 相同
			vector<vector<POINT>>  vt2ptContoursPos;

			SFindRange_Result() : nIC_Width(0), nIC_Height(0){
				vtptCorners.resize(4);
				vtnContoursCount.resize(4, 0);
				vt2ptContoursPos.resize(4);
			}

			void Initial()
			{
				nIC_Width = 0;
				nIC_Height = 0;
				vtptCorners.resize(4);
				vtnContoursCount.resize(4, 0);
				vt2ptContoursPos.clear();
				vt2ptContoursPos.resize(4);
			}
		};

		// 膠面積偵測結果
		struct SMeasurementGlueArea_Result
		{
			// 預設為 false
			// 區塊量測結果, true=OK, false=NG
			bool bResult;

			// 預設為 0
			// 偵測到的膠數量
			int nGlueCount;

			// IC 範圍相關資訊
			SFindRange_Result			sIcRange;

			// 膠面積相關資訊, vtsGlueArea.size() 必須等於 nGlueCount
			vector<SFindEdge_Result>	vtsGlueArea;

			SMeasurementGlueArea_Result() : bResult(false), nGlueCount(0) {}
			~SMeasurementGlueArea_Result() = default;

			void Initial()
			{
				bResult = false;
				nGlueCount = 0;
				sIcRange.Initial();
				vtsGlueArea.clear();
			}
		};
#pragma endregion

#pragma region SPIL Glue Edge To Part algorithm

		// 量測膠邊界到零件距離演算法參數
		struct SMeasurementGlueToPart_Parameter
		{
			// 預設 30
			// 零件梯度影像閥值
			int		m_nPart_Gradient_Threshold;

			// 預設為 1
			// 零件種類
			int		m_nPartTypes;

			// 預設為 0
			// IC在影像中的位置
			RECT rectIcROI;

			SFindEdge_Parameter sGlue;

			SFindEdge_Parameter sPart;

			SMeasurementGlueToPart_Parameter() : m_nPart_Gradient_Threshold(30), m_nPartTypes(1), sGlue(), sPart() {
				rectIcROI.left = 0;
				rectIcROI.top = 0;
				rectIcROI.right = 0;
				rectIcROI.bottom = 0;
			}

			~SMeasurementGlueToPart_Parameter() = default;

			bool Check(string& strInfo) const
			{
				string strData;
				if (!sGlue.Check(strData)) {
					strInfo = "膠邊_" + strData;
					return false;
				}

				if (!sPart.Check(strData)) {
					strInfo = "零件_" + strData;
					return false;
				}
				return true;
			}
		};
#pragma endregion

#pragma endregion

		class MeasurementBlackGlue
		{
		public:

			//										錯誤訊息 : 預設為 空字串
			string									m_strErrorMessage;

		private:

			//										版次
			string									m_strVersion;

			//										黑膠輸入參數
			SMeasurementBlackGlue_Parameter			m_sParam;

			//										黑膠量測結果
			SMeasurementBlackGlue_Result			m_sResult;

			//                                      Flux面積偵測輸入參數
			SMeasurementFluxArea_Parameter			m_sParam_FluxArea;

			//                                      Flux面積偵測結果
			SMeasurementFluxArea_Result				m_sResult_FluxArea;

			//										膠高量測參數
			SMeasurementGlueHeightRatio_Parameter	m_sParam_GlueHeight;

			//										膠高量測結果
			SMeasurementGlueHeightRatio_Result		m_sResult_GlueHeight;

			//										膠面積量測參數
			SMeasurementGlueArea_Parameter			m_sParam_GlueArea;

			//										膠面積量測結果
			SMeasurementGlueArea_Result				m_sResult_GlueArea;

			//										膠邊到零件邊參數
			SMeasurementGlueToPart_Parameter		m_sParam_GlueToPart;

			//                                  影像存檔的開關 : 預設為 false
			bool								m_bSaveImage;

			//                                  影像存檔的路徑+檔名(不包含附檔名) : 預設為 空字串
			string								m_strSavePathName;

			//                                  影像存檔的副檔名 : 預設為 1 ; 1=>jpg ; 2=>bmp ; 3=>png
			int									m_nExtensionType;

			//                                  影像存檔的權限等級 : 預設為 0
			int									m_nSaveLevel;

			//                                  影像存檔的順序Id : 預設為 0
			int									m_nSaveIndex;

			//                                  顯示線寬的間隔 : 預設為 5
			int                                 m_nLineInterval;

			//                                  膠ROI的定位模式, 1=>依使用者框的位置, 2=>程式自動計算, 預設為 1, 值域[1,2]
			int                                 m_nPositioningMode_ROI;

			int									m_nShiftIndex;
			int									m_nContoursIndex;
			int									m_nGlueROIIndex;
			int									m_nCoatingValue;

			RECT								m_rectThermalGlueMeasurementRange;

			// 計算膠邊的彩色影像
			Mat									m_matGlue;

			// 計算板邊的彩色影像
			Mat									m_matBoard;

			// 計算Coating的彩色影像
			Mat									m_matCoating;

			// 計算ThermalGlue的彩色影像
			Mat									m_matThermal;

			Mat                                 m_matEdgeMask;

			Mat									m_matBoardEdge;

			Mat                                 m_matThermalGlueEdge;

			// 仿射轉換係數      
			vector<double>						m_vtdCoefficient;

			// 散熱膠 Die 四條邊的斜率
			vector<double>						m_vtdDieSlope;

			// 散熱膠 Die 四條邊的截距
			vector<double>						m_vtdDieIntercept;

			// 水平板邊的每一個座標點
			vector<POINT>						m_vtptBoardEdgePoint_H;

			// 垂直板邊的每一個座標點
			vector<POINT>						m_vtptBoardEdgePoint_V;

			// 散熱膠IC---四條邊的每一個座標點, 0=上, 1=下, 2=左, 3=右
			vector<vector<POINT>>				m_vt2ptICEdgePoint;

			// 彩色影像找黑膠邊緣的流程
			SProcessModeParam					m_sPM_FindGlue;

			// 彩色影像找板邊的流程
			SProcessModeParam					m_sPM_FindBoard;

			// 彩色影像找Coating的流程
			SProcessModeParam					m_sPM_FindCoating;

			// 彩色影像找ThermalGlue IC的流程
			SProcessModeParam					m_sPM_FindThermal_IC;

			// 彩色影像找ThermalGlue Glue的流程
			SProcessModeParam					m_sPM_FindThermal_Glue;

			// 顯示 膠輪廓 的顏色, 
			Scalar                              m_scContours_Glue;

			// 顯示 Coating輪廓 的顏色, 
			Scalar                              m_scContours_Coating;

			// 顯示板邊輪廓的顏色, 
			Scalar                              m_scContours_Board;

			// 顯示板邊直線的顏色, 
			Scalar                              m_scLine_Board;

			// 顯示膠寬ROI(垂直框 水平框)的顏色, 以及sThermalGlue.sGlueWidth
			Scalar                              m_scROI_GlueWidth;

			// 顯示膠寬(垂直框 水平框)距離線的顏色, 以及sThermalGlue.sGlueWidth 
			Scalar                              m_scDistance_GlueWidth;

			// 顯示膠寬(垂直框 水平框)最長距離記號的顏色, 以及sThermalGlue.sGlueWidth 
			Scalar                              m_scMaxDist_GlueWidth;

			// 顯示膠寬(垂直框 水平框)最短距離記號的顏色, 以及sThermalGlue.sGlueWidth 
			Scalar                              m_scMinDist_GlueWidth;

			// 顯示膠Arc ROI 與 StartEnd ROI的顏色, 以及 sThermalGlue.sGlueToDie
			Scalar                              m_scROI_GlueArc;

			// 顯示膠Arc 與 StartEnd 的ROI距離線的顏色, 以及 sThermalGlue.sGlueToDie 
			Scalar                              m_scDistance_GlueArc;

			// 顯示膠Arc 與 StartEndROI 的最長距離記號的顏色, 以及 sThermalGlue.sGlueToDie
			Scalar                              m_scMaxDist_GlueArc;

			// 顯示膠Arc最短距離記號的顏色, 以及 sThermalGlue.sGlueToDie 
			Scalar                              m_scMinDist_GlueArc;

			// 顯示板邊到膠邊 與 Coating到膠邊 的ROI的顏色, 以及sThermalGlue.sGlueToGlue
			Scalar                              m_scROI_BoardToGlue;

			// 顯示板邊到膠邊 與 Coating到膠邊 的距離線的顏色, 以及sThermalGlue.sGlueToGlue
			Scalar                              m_scDistance_BoardToGlue;

			// 顯示板邊到膠邊 與 Coating到膠邊 的最長距離記號的顏色, 以及sThermalGlue.sGlueToGlue
			Scalar                              m_scMaxDist_BoardToGlue;

			// 顯示板邊到膠邊 與 Coating到膠邊 的最短距離記號的顏色, 以及sThermalGlue.sGlueToGlue
			Scalar                              m_scMinDist_BoardToGlue;

			vector<ConfigData>					m_vtsConfig;
		public:

			MeasurementBlackGlue() : m_strErrorMessage(""), m_strVersion("2.2.0"), m_bSaveImage(false), m_strSavePathName(""), m_nExtensionType(1), 
				m_nSaveLevel(0), m_nSaveIndex(0), m_nLineInterval(5), m_nPositioningMode_ROI(1), m_nShiftIndex(-1), m_nContoursIndex(-1), m_nGlueROIIndex(-1),
				m_nCoatingValue(254),
				m_scContours_Glue(190, 235, 128), m_scContours_Coating(46, 110, 188), m_scContours_Board(30, 131, 240), m_scLine_Board(30, 131, 120),
				m_scROI_GlueWidth(134, 25, 96), m_scDistance_GlueWidth(0, 255, 255), m_scMaxDist_GlueWidth(0, 141, 241), m_scMinDist_GlueWidth(0, 206, 194),
				m_scROI_GlueArc(218, 225, 189), m_scDistance_GlueArc(123, 230, 221), m_scMaxDist_GlueArc(237, 128, 252), m_scMinDist_GlueArc(200, 0, 255),
				m_scROI_BoardToGlue(9, 58, 219), m_scDistance_BoardToGlue(230, 120, 175), m_scMaxDist_BoardToGlue(87, 134, 71), m_scMinDist_BoardToGlue(217, 167, 121) {};

			~MeasurementBlackGlue() = default;

			// 黑膠量測
			// [In] vtInputImage 的 size 為 4
			// [In] vtInputImage[0] = 量測膠邊的彩色影像
			// [In] vtInputImage[1] = 量測板邊的彩色影像
			// [In] vtInputImage[2] = 量測散熱膠的彩色影像
			// [In] vtInputImage[3] = 量測coating的彩色影像
			// [In] sParam : 黑膠量測的相關參數
			// [Out] sResult: 黑膠量測的結果
			bool Measurement(const vector<Mat>& vtInputImage, SMeasurementBlackGlue_Parameter& sParam, SMeasurementBlackGlue_Result& sResult);

			// Flux面積量測
			// [In] matInputImage = Flux量測的彩色影像(使用低角度白光)
			// [In] sParam : Flux量測的相關參數
			// [Out] sResult: Flux量測的結果
			bool Measurement_FluxArea(const Mat& matInputImage, SMeasurementFluxArea_Parameter& sParam, SMeasurementFluxArea_Result& sResult);

			// 膠高比例量測
			bool Measurement_GlueHeightRatio(const Mat& matInputImage, SMeasurementGlueHeightRatio_Parameter& sParam, SMeasurementGlueHeightRatio_Result& sResult);

			// 膠面積偵測
			bool Measurement_GlueArea(const Mat& matInputImage, SMeasurementGlueArea_Parameter& sParam, SMeasurementGlueArea_Result& sResult);

			// 取得版次
			string GetVersion() const {return m_strVersion;}

			// 取得錯誤訊息
			string GetErrorMessage() const {return m_strErrorMessage;}

			// 儲存相關功能
#pragma region Save

			// AMD黑膠儲存參數
			// sParam : 要儲存的參數
			// strPathName : 儲存路徑+檔名(不須要檔名)
			bool Save_Parameter(const SMeasurementBlackGlue_Parameter& sParam, const string& strPathName);

			// AMD黑膠讀取參數
			// strPathName : 讀取路徑+檔名(不須要檔名)
			// sParam : 讀取後存放資料的變數
			bool Load_Parameter(const string& strPathName, SMeasurementBlackGlue_Parameter& sParam);

			// AMD Flux面積量測儲存參數
			// sParam : 要儲存的參數
			// strPathName : 儲存路徑+檔名(不須要檔名)
			bool SaveParameter_FluxArea(const SMeasurementFluxArea_Parameter& sParam, const string& strPathName);

			// AMD Flux面積量測讀取參數
			// strPathName : 讀取路徑+檔名(不須要檔名)
			// sParam : 讀取後存放資料的變數
			bool LoadParameter_FluxArea(const string& strPathName, SMeasurementFluxArea_Parameter& sParam);

			// AMD黑膠 儲存量測結果文字檔
			// strPathName : 讀取路徑+檔名(不須要檔名)
			// sResult : 量測結果
			bool Save_Result_Txt(const string& strPathName, const SMeasurementBlackGlue_Result& sResult);

			// apple膠高量測儲存參數
			// sParam : 要儲存的參數
			// strPathName : 儲存路徑+檔名(不須要檔名)
			bool SaveParameter_GlueHeight(const SMeasurementGlueHeightRatio_Parameter& sParam, const string& strPathName);

			// apple膠高量測讀取參數
			// strPathName : 讀取路徑+檔名(不須要檔名)
			// sParam : 讀取後存放資料的變數
			bool LoadParameter_GlueHeight(const string& strPathName, SMeasurementGlueHeightRatio_Parameter& sParam);

			// apple 儲存膠高量測結果
			bool Save_GlueHeightResult_Txt(const string& strPathName, const SMeasurementGlueHeightRatio_Result& sResult);

			// apple膠面積量測儲存參數
			// sParam : 要儲存的參數
			// strPathName : 儲存路徑+檔名(不須要檔名)
			bool SaveParameter_GlueArea(const SMeasurementGlueArea_Parameter& sParam, const string& strPathName);

			// apple膠面積量測讀取參數
			// strPathName : 讀取路徑+檔名(不須要檔名)
			// sParam : 讀取後存放資料的變數
			bool LoadParameter_GlueArea(const string& strPathName, SMeasurementGlueArea_Parameter& sParam);

			// spil 膠邊到零件邊偵測儲存參數
			bool SaveParameter_GlueToPart(const SMeasurementGlueToPart_Parameter& sParam, const string& strPathName);

			// spil 膠邊到零件邊偵測讀取參數
			bool LoadParameter_GlueToPart(const string& strPathName, SMeasurementGlueToPart_Parameter& sParam);

			// 設定是否要輸出計算過程的影像
			// [In] bSave	: 為true 會將 Inspection()過程影像紀錄下來
			void SetSaveImage(const bool& bSave) { m_bSaveImage = bSave; }

			// 控制輸出前處理過程
			void SetSaveLevel(const int nLevel) { m_nSaveLevel = nLevel; };

			// 設定存圖的路徑與檔名(不需要副檔名)
			// [In] strPathName	: 會將 Measurement()過程影像 儲存到 strPathName所指定的路徑與檔名(資料夾須先建立)
			bool SetSavePathName(const string& strPathName);

			// 設定儲存影像的副檔名
			// [In] nType : 1=>jpg ; 2=>bmp ; 3=>png
			bool SetImageExtension(const int& nType);

#pragma endregion

#pragma region Report

			// 輸出黑膠的報表
			// [In] sParam : 黑膠量測的參數, 
			// [In] sResult : 黑膠量測的結果
			// [Out] vtstrReport : 輸出的報表
			// [In] nMode : 1=>有量測的全部輸出, 2=>只輸出OK的, 3=>只輸出NG的
			// [In] nSymbolType : 符號樣式, 1=>" , "(逗號2邊都有空白)  ; 2=", "(逗號右邊有空白) ; 3=>","(逗號2側都沒有空白)	
			bool ReportTxt_BlackGlue(const SMeasurementBlackGlue_Parameter& sParam, const SMeasurementBlackGlue_Result& sResult, vector<string>& vtstrReport, const int& nMode=1, const int& nSymbolType=1);

			// ROI報表產生器
			void AppendROIReport(vector<string>& vtstrReport, const string& itemName, const vector<string>& subNames, const string& strSymbol, const int nMode, const int nDecimalPlaces_Value, const int nDecimalPlaces_Limit, const SMultipleROI_Result& result, const SMultipleROI_Parameter& param);

			// StartEnd報表產生器
			void AppendStartEndROIReport(std::vector<std::string>& vtstrReport, const std::string& itemName, const std::string& strSymbol, const int nMode, const int nDecimalPlaces_Value, const int nDecimalPlaces_Limit, const SStartEndROI_Result& result, const SStartEndROI_Parameter& param);

			// Thermal報表產生器
			void AppendThermalReport(std::vector<std::string>& vtstrReport, const std::string& itemName, const vector<string>& subNames, const std::string& strSymbol, const int nMode, const int nDecimalPlaces_Value, const int nDecimalPlaces_Limit, const SMultipleROI_Result& result, const vector<SLimit_ROI>& param);

			// 輸出黑膠的報表與對應的顏色
			// [In] sParam : 黑膠量測的參數, 
			// [In] sResult : 黑膠量測的結果
			// [Out] vtstrReport : 輸出的報表
			// [Out] vtscColor : 報表對應的顏色
			// [In] nMode : 1=>有量測的全部輸出, 2=>只輸出OK的, 3=>只輸出NG的
			// [In] nSymbolType : 符號樣式, 1=>" , "(逗號2邊都有空白)  ; 2=", "(逗號右邊有空白) ; 3=>","(逗號2側都沒有空白)	
			bool ReportTxt_BlackGlue_Color(const SMeasurementBlackGlue_Parameter& sParam, const SMeasurementBlackGlue_Result& sResult, vector<string>& vtstrReport, vector<Scalar>& vtscrReportColor, const int& nMode = 1, const int& nSymbolType = 1);		
			void AppendROIReport_Color(vector<string>& vtstrReport, vector<Scalar>& vtscrReportColor, const string& itemName, const vector<Scalar>& vtscritemColor, const vector<string>& subNames, const string& strSymbol, const int nMode, const int nDecimalPlaces_Value, const int nDecimalPlaces_Limit, const SMultipleROI_Result& result, const SMultipleROI_Parameter& param);
			void AppendThermalReport_Color(std::vector<std::string>& vtstrReport, vector<Scalar>& vtscrReportColor, const std::string& itemName, const vector<Scalar>& vtscritemColor, const vector<string>& subNames, const std::string& strSymbol, const int nMode, const int nDecimalPlaces_Value, const int nDecimalPlaces_Limit, const SMultipleROI_Result& result, const vector<SLimit_ROI>& param);

			// 輸出一行
			// nDecimalPlaces_Value : fValue 顯示的小數位數
			// nDecimalPlaces_Limit : fUpper與fLower 顯示的小數位數
			string ReportTxt_Single(const string& strItemName, const int& nIndex, const string& strSubName, const string& strSymbol, const float& fValue, const float& fUpper, const float& fLower, const int& nDecimalPlaces_Value=1, const int& nDecimalPlaces_Limit=1);

#pragma endregion

			// 顯示相關功能
#pragma region Display

			// 設置顯示線寬的間隔
			void SetLineInterval(const int& nInterval) {
				if (nInterval <= 0)m_nLineInterval = 1;
				else m_nLineInterval = nInterval;
			}

			// AMD 黑膠---將量測結果畫在 matResult-不顯示量測數據
			// [In]      SMeasurementBlackGlue_Result : 黑膠量測的結果
			// [In][Out] matResult : 顯示最終量測結果的影像, 輸入時需為彩色影像
			// [In]      nContourWidth = 輪廓或ROI線的寬, 0=>1 pixels; 1=>3 pixels ...
			// [In]      nLineWidth = 量測線的寬, 1=>1 pixels; 2=>2 pixels ...
			bool ShowResult(const SMeasurementBlackGlue_Result& sResult, Mat& matResult, const int& nContourWidth = 1, const int& nLineWidth = 1);

			// AMD 黑膠---將量測結果畫在 matResult-顯示量測數據
			// [In]      SMeasurementBlackGlue_Result : 黑膠量測的結果
			// [In][Out] matResult : 顯示最終量測結果的影像, 輸入時需為彩色影像
			// [In]      nContourWidth = 輪廓或ROI線的寬, 0=>1 pixels; 1=>3 pixels ...
			// [In]      nLineWidth = 量測線的寬, 1=>1 pixels; 2=>2 pixels ...
			bool ShowResult_Txt(const SMeasurementBlackGlue_Result& sResult, Mat& matResult, const int& nContourWidth = 1, const int& nLineWidth = 1);


			// AMD---Flux面積量測結果輸出
			// [In]      SMeasurementFluxArea_Result : Flux量測的結果
			// [In][Out] matResult : 顯示最終量測結果的影像, 輸入時需為彩色影像
			// [Out]     vtstrResult: 量測數據
			// [In]      nContourWidth = 輪廓寬, 0=>1 pixels; 1=>3 pixels ...
			bool ShowResult_FluxArea(const SMeasurementFluxArea_Result& sResult, Mat& matResult, vector<string>& vtstrResult, bool& bResult, const int& nContourWidth = 1);

			// AMD---Flux面積量測結果輸出(結果寫在影像上)
			// [In]      SMeasurementFluxArea_Result : Flux量測的結果
			// [In][Out] matResult : 顯示最終量測結果的影像, 輸入時需為彩色影像
			// [In]      nContourWidth = 輪廓寬, 0=>1 pixels; 1=>3 pixels ...
			bool ShowResult_FluxArea_Txt(const SMeasurementFluxArea_Result& sResult, Mat& matResult, const int& nContourWidth = 1);

			// 膠高量測結果輸出
			bool ShowResult_GlueHeight(const SMeasurementGlueHeightRatio_Result& sResult, Mat& matResult, bool& bResult, vector<string>& vtstrResult);

			// 膠面積量測結果輸出
			bool ShowResult_GlueArea(const SMeasurementGlueArea_Result& sResult, Mat& matResult, vector<string>& vtstrResult, bool& bResult);

			// 膠面積量測結果輸出(結果會顯示在畫面上)
			bool ShowResult_GlueArea_Txt(const SMeasurementGlueArea_Result& sResult, Mat& matResult);

			// 顯示板邊輪廓
			// [In] sResult : 板邊檢測結果
			// [In] scContours : 輪廓的顏色
			// [In] scLine : 線的顏色
			// [In] [Out] matDisplay : 輸入彩色影像, 將檢測結果畫上去
			// [In] nContourWidth = 輪廓或ROI線的寬, 0=>1 pixels; 1=>3 pixels ...
			// [In] nLineWidth = 量測線的寬, 1=>1 pixels; 2=>2 pixels ...
			bool DisplayBoardPosition(const SBoardEdge_Result& sResult, const Scalar& scContours, const Scalar& scLine, Mat& matDisplay, const int& nContourWidth = 1, const int& nLineWidth = 1);

			// 顯示膠邊輪廓
			// [In] sResult : 膠輪廓檢測結果
			// [In] scContours : 輪廓的顏色
			// [In] [Out] matDisplay : 輸入彩色影像, 將檢測結果畫上去
			// [In] nContourWidth = 輪廓線寬, 0=>1 pixels; 1=>3 pixels ...
			bool DisplayGluePosition(const SGlueEdge_Result& sResult, const Scalar& scContours, Mat& matDisplay, const int& nContourWidth = 1);
			bool DisplayGluePosition(const SFindEdge_Result& sResult, const Scalar& scContours, Mat& matDisplay, const int& nContourWidth = 1);

			// 顯示Coating輪廓
			// [In] sResult : Coating檢測結果
			// [In] scContours : 輪廓的顏色
			// [In] [Out] matDisplay : 輸入彩色影像, 將檢測結果畫上去
			// [In] nContourWidth = 輪廓或ROI線寬, 0=>1 pixels; 1=>3 pixels ...
			bool DisplayCoatingPosition(const SCoatingEdge_Result& sResult, const Scalar& scContours, Mat& matDisplay, const int& nContourWidth = 1);

			// 顯示散熱膠IC位置的散熱膠輪廓
			// [In] sResult : 散熱膠檢測結果
			// [In] scContours : 輪廓的顏色
			// [In] [Out] matDisplay : 輸入彩色影像, 將檢測結果畫上去
			// [In] nContourWidth = 輪廓或ROI寬, 0=>1 pixels; 1=>3 pixels ...
			bool DisplayThermalGluePosition(const SThermalGlue_Result& sResult, const Scalar& scContours, Mat& matDisplay, const int& nContourWidth = 1);

			// 顯示單一ROI框的量測結果
			// [In] sResult : ROI框檢測結果
			// [In] scROI : ROI框的顏色
			// [In] scDistance : 顯示距離的顏色
			// [In] scMaxDistance : 顯示最大距離位置記號的顏色
			// [In] scMinDistance : 顯示最小距離位置記號的顏色
			// [In] [Out] matDisplay : 輸入彩色影像, 將檢測結果畫上去
			// [In] nContourWidth = 輪廓或ROI線的寬, 0=>1 pixels; 1=>3 pixels ...
			// [In] nLineWidth = 量測線的寬, 1=>1 pixels; 2=>2 pixels ...
			bool DisplayRoiResult(const SMultipleROI_Result& sResult, const Scalar& scROI, const Scalar& scDistance, const Scalar& scMaxDistance, const Scalar& scMinDistance, const string& strInfo,  Mat& matDisplay, const bool& bShowROI = true, const bool& bShowLine = true, const bool& bMax = true, const bool& bMin = true, const int& nContourWidth = 1, const int& nLineWidth = 1);

			// 顯示量測首尾ROI距離的結果
			// [In] sResult : StartEnd ROI 檢測結果
			// [In] scROI : ROI框的顏色
			// [In] scDistance : 顯示距離的顏色
			// [In] scMaxDistance : 顯示最大距離位置記號的顏色
			// [In] [Out] matDisplay : 輸入彩色影像, 將檢測結果畫上去
			// [In] nContourWidth = 輪廓或ROI線的寬, 0=>1 pixels; 1=>3 pixels ...
			// [In] nLineWidth = 量測線的寬, 1=>1 pixels; 2=>2 pixels ...
			bool DisplayStartEndResult(const SStartEndROI_Result& sResult, const Scalar& scROI, const Scalar& scDistance, const Scalar& scMaxDistance, const string& strInfo, Mat& matDisplay, const bool& bShowROI = true, const int& nContourWidth = 1, const int& nLineWidth = 1);
#pragma endregion

			// 設定顯示顏色
#pragma region SetColor

			// 設定顯示膠輪廓的顏色
			void SetColor_GlueContours(const uchar& unR, const uchar& unG, const uchar& unB) { m_scContours_Glue = Scalar(unB, unG, unR); }

			// 設定顯示Coating輪廓的顏色
			void SetColor_Coating(const uchar& unR, const uchar& unG, const uchar& unB) { m_scContours_Coating = Scalar(unB, unG, unR); }

			// 設定顯示板邊輪廓的顏色
			void SetColor_BorderContours(const uchar& unR, const uchar& unG, const uchar& unB) { m_scContours_Board = Scalar(unB, unG, unR); }

			// 設定顯示板邊直線的顏色
			void SetColor_BorderLine(const uchar& unR, const uchar& unG, const uchar& unB) { m_scLine_Board = Scalar(unB, unG, unR); }

			// 設定顯示膠寬ROI(垂直框 水平框)的顏色
			void SetColor_GlueWidthROI(const uchar& unR, const uchar& unG, const uchar& unB) { m_scROI_GlueWidth = Scalar(unB, unG, unR); }

			// 設定膠寬ROI(垂直框 水平框)距離線的顏色
			void SetColor_Distance_GlueWidth(const uchar& unR, const uchar& unG, const uchar& unB) { m_scDistance_GlueWidth = Scalar(unB, unG, unR); }

			// 設定膠寬ROI(垂直框 水平框)最長距離記號的顏色
			void SetColor_MaxDist_GlueWidth(const uchar& unR, const uchar& unG, const uchar& unB) { m_scMaxDist_GlueWidth = Scalar(unB, unG, unR); }

			// 設定膠寬ROI(垂直框 水平框)最短距離記號的顏色
			void SetColor_MinDist_GlueWidth(const uchar& unR, const uchar& unG, const uchar& unB) { m_scMinDist_GlueWidth = Scalar(unB, unG, unR); }

			// 設定顯示膠Arc ROI 與 StartEnd ROI的顏色
			void SetColor_GlueArcROI(const uchar& unR, const uchar& unG, const uchar& unB) { m_scROI_GlueArc = Scalar(unB, unG, unR); }

			// 設定顯示膠Arc 與 StartEnd 的ROI距離線的顏色
			void SetColor_Distance_GlueArc(const uchar& unR, const uchar& unG, const uchar& unB) { m_scDistance_GlueArc = Scalar(unB, unG, unR); }

			// 設定膠Arc 與 StartEnd 的最長距離記號的顏色
			void SetColor_MaxDist_GlueArc(const uchar& unR, const uchar& unG, const uchar& unB) { m_scMaxDist_GlueArc = Scalar(unB, unG, unR); }

			// 設定顯示膠Arc最短距離記號的顏色的顏色
			void SetColor_MinDist_GlueArc(const uchar& unR, const uchar& unG, const uchar& unB) { m_scMinDist_GlueArc = Scalar(unB, unG, unR); }

			// 設定顯示板邊到膠邊 與 Coating到膠邊 的ROI的顏色, 
			void SetColor_BoardToGlueROI(const uchar& unR, const uchar& unG, const uchar& unB) { m_scROI_BoardToGlue = Scalar(unB, unG, unR); }

			// 設定顯示板邊到膠邊 與 Coating到膠邊ROI 的距離線的顏色
			void SetColor_Distance_BoardToGlue(const uchar& unR, const uchar& unG, const uchar& unB) { m_scDistance_BoardToGlue = Scalar(unB, unG, unR); }

			// 設定顯示板邊到膠邊 與 Coating到膠邊的最長距離記號的顏色
			void SetColor_MaxDist_BoardToGlue(const uchar& unR, const uchar& unG, const uchar& unB) { m_scMaxDist_BoardToGlue = Scalar(unB, unG, unR); }

			// 設定顯示板邊到膠邊 與 Coating到膠邊的最短距離記號的顏色
			void SetColor_MinDist_BoardToGlue(const uchar& unR, const uchar& unG, const uchar& unB) { m_scMinDist_BoardToGlue = Scalar(unB, unG, unR); }

#pragma endregion

#pragma region AMD Adjustment Parameter

			// 調整膠參數
#pragma region Adjustment Glue Parameter

			// Method1 : 找膠本體(抽色)
#pragma region Method1

			// Method1-Step1 輸入膠的彩色影像進行抽色, 輸出膠的二值化影像
			// [In] matColor : 膠的彩色影像(彩色)
			// [In] nGlueValue_R, nGlueValue_G, nGlueValue_R : 黑膠的 R G B 值
			// [In] nGlueValueTolerance : 黑膠的 R G B 值的誤差範圍
			// [Out] matThreshold : 輸出 膠的二值化影像(灰階)
			bool AdjustmentGlueParameter_Method1_Step1_ColorToThreshold(const Mat& matColor, const int& nGlueValue_R, const int& nGlueValue_G, const int& nGlueValue_B, const int& nGlueValueTolerance, Mat& matThreshold);

			// Method1-Step2 輸入膠的二值化影像與濾波size, 輸出雜訊過濾後的二值化影像
			// [In] matThreshold : 膠的二值化影像(灰階)
			// [In] nOpenX : X方向濾波尺寸,不可小於1,最好是奇數(單位 pixels)
			// [In] nOpenY : Y方向濾波尺寸,不可小於1,最好是奇數(單位 pixels)
			// [Out] matMedian : 雜訊過濾後的二值化影像(灰階)
			bool AdjustmentGlueParameter_Method1_Step2_Open(const Mat& matThreshold, const int& nOpenX, const int& nOpenY, Mat& matMedian);

			// Method1-Step3 輸入雜訊過濾後的二值化影像像與濾波size, 輸出連接二值化影像邊緣的影像
			// [In] matMedian : 雜訊過濾後的二值化影像(灰階)
			// [In] nConnectX : X方向濾波尺寸,不可小於1,最好是奇數(單位 pixels)
			// [In] nConnectY : Y方向濾波尺寸,不可小於1,最好是奇數(單位 pixels)
			// [Out] matConnect : 膠的二值化影像的連接結果(灰階)
			bool AdjustmentGlueParameter_Method1_Step3_Connect(const Mat& matMedian, const int& nConnectX, const int& nConnectY, Mat& matConnect);

			// Method1-Step4 輸入連接邊緣後的二值化影像與板邊位置以及膠ROI的大小, 輸出黑膠本體的二值化影像
			// [In] matConnect : 連接邊緣後的二值化(灰階)
			// [In] nLocation : 板邊在 IC的位置
			// [In] fGlueROI_Tolerance : 膠本體的寬放係數, 值域 (0 , 1.0]
			// [In] rectGlueROI : 黑膠在影像中的ROI座標
			// [Out] matGlue : 膠本體的二值化影像(灰階)
			bool AdjustmentGlueParameter_Method1_Step4_Position(const Mat& matConnect, const int& nLocation, const int& nOpenX2, const int& nOpenY2, const float& fGlueROI_Tolerance, const RECT& rectGlueROI, Mat& matGlue);

			// Method1-Step5-End 膠本體的二值化影像與 膨脹 侵蝕濾波的大小, 膠輪廓影像
			// [In] matColor : 膠的彩色影像(彩色)
			// [In] matGlue : 膠本體的二值化影像(灰階)
			// [In] nDilateX nDilateY : 將膠本體的二值化影像進行膨脹
			// [In] nErosionX nErosionY : 將膠本體的二值化影像進行侵蝕
			// [Out] matEnd : 膠輪廓影像(彩色)
			bool AdjustmentGlueParameter_Method1_Step5_End(const Mat& matColor, const Mat& matGlue, const int& nLocation, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, Mat& matEnd);
#pragma endregion

			// Method2 : 找膠輪廓(轉灰階+二值化)
#pragma region Method2

			// Method2-Step1 輸入膠的彩色影像, 與中值濾波Size, 輸出過濾雜訊後的灰階影像
			// [In] matColor : 膠彩色影像(彩色)
			// [In] nMedianX nMedianY : 中值濾波大小
			// [Out] matGray : 輸出 膠灰階影像(灰階)
			bool AdjustmentGlueParameter_Method2_Step1_ColorToGray(const Mat& matColor, const int& nMedianX, const int& nMedianY, Mat& matGray);

			// Method2-Step2 膠的灰階影像二值化取出邊界
			// [In] matGray : 膠的過濾後的灰階影像(灰階)
			// [In] bDark : 為true=>選取[0 , nThreshold] ; 為false=>選取[nThreshold , 255]
			// [In] nThreshold : 二值化閥值, 值域 [0 , 255]
			// [Out] matThreshold : 膠的二值化影像(灰階)
			bool AdjustmentGlueParameter_Method2_Step2_Threshold(const Mat& matGray, const bool& bDark, const int& nThreshold, const int& nOpenX, const int& nOpenY, Mat& matThreshold);

			// Method2-Step3 膠的二值化影像連接
			// [In] matThreshold : 膠的二值化影像(灰階)
			// [In] nConnectX : X方向連接尺寸,不可小於1,最好是奇數(單位 pixels)
			// [In] nConnectY : Y方向連接尺寸,不可小於1,最好是奇數(單位 pixels)
			// [Out] matConnect : 連接後的二值化影像(灰階)
			bool AdjustmentGlueParameter_Method2_Step3_Connect(const Mat& matThreshold, const int& nConnectX, const int& nConnectY, Mat& matConnect);

			// Method2-Step4 輸入連接邊緣後的二值化影像與板邊位置以及膠ROI的大小, 輸出黑膠本體的二值化影像
			// [In] matConnect : 連接邊緣後的二值化(灰階)
			// [In] nLocation : 板邊在 IC的位置
			// [In] fGlueROI_Tolerance : 膠本體的寬放係數, 值域 (0 , 1.0]
			// [In] rectGlueROI : 黑膠在影像中的ROI座標
			// [Out] matGlue : 膠本體的二值化影像(灰階)
			bool AdjustmentGlueParameter_Method2_Step4_Position(const Mat& matConnect, const int& nLocation, const int& nOpenX2, const int& nOpenY2, const float& fGlueROI_Tolerance, const RECT& rectGlueROI, Mat& matGlue);

			// Method2-Step5-End 膠本體的二值化影像與 膨脹 侵蝕濾波的大小, 膠輪廓影像
			// [In] matColor : 膠的彩色影像(彩色)
			// [In] matGlue : 膠本體的二值化影像(灰階)
			// [In] nDilateX nDilateY : 將膠本體的二值化影像進行膨脹
			// [In] nErosionX nErosionY : 將膠本體的二值化影像進行侵蝕
			// [Out] matEnd : 膠輪廓影像(彩色)
			bool AdjustmentGlueParameter_Method2_Step5_End(const Mat& matColor, const Mat& matGlue, const int& nLocation, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, Mat& matEnd);
#pragma endregion
#pragma endregion

			// 調整板邊參數 
#pragma region Adjustment Board Parameter

			bool AdjustmentBoardParameter_Method1_Step1_Median(const Mat& matColor, const int& nMedianX, const int& nMedianY, Mat& matThreshold);

			bool AdjustmentBoardParameter_Method1_Step2_ColorToThreshold(const Mat& matColor, const vector<bool>& vtbEnable, const vector<int>& vtnBoardValue_R, const vector<int>& vtnBoardValue_G, const vector<int>& vtnBoardValue_B, const vector<int>& vtnBoardValueTolerance, Mat& matThreshold);

			bool AdjustmentBoardParameter_Method1_Step3_Open(const Mat& matThreshold, const int& nOpenX, const int& nOpenY, Mat& matOpen);

			bool AdjustmentBoardParameter_Method1_Step4_Connect(const Mat& matOpen, const int& nConnectX, const int& nConnectY, Mat& matConnect);

			bool AdjustmentBoardParameter_Method1_Step5_End(const Mat& matColor, const Mat& matBoard, bool& bSlope, const int& nLocation, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, Mat& matEnd);

#pragma endregion

			// 調整散熱膠參數
#pragma region Adjustment ThermalGlue Parameter

			// Die
			// rectDie : Die在影像上的位置
			// nOutsetDistanceX : Die 水平方向外擴距離
			// nOutsetDistanceY : Die 垂直方向外擴距離
			bool AdjustmentThermalGlueParameter_Die_Method1_Step1_ROI(const Mat& matColor, const RECT& rectDie, const int& nOutsetDistanceX, const int& nOutsetDistanceY, Mat& matROI_Color);

			bool AdjustmentThermalGlueParameter_Die_Method1_Step2_ToGray(const Mat& matROI_Color, const int& nMedianX, const int& nMedianY, Mat& matROI_Gray);

			bool AdjustmentThermalGlueParameter_Die_Method1_Step3_Threshold(const Mat& matROI_Gray, const int& nThreshold_Low, const int& nThreshold_High, const bool& bBetween, Mat& matROI_Threshold);

			bool AdjustmentThermalGlueParameter_Die_Method1_Step4_Position(const Mat& matROI_Threshold, const int& nOpenX, const int& nOpenY, const int& nConnectX, const int& nConnectY, Mat& matROI_Open);

			bool AdjustmentThermalGlueParameter_Die_Method1_Step5_End(const Mat& matROI_Color, const Mat& matROI_Open, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, const bool& bRebuildDie, RECT& rectDie, Mat& matEnd);

			// Glue
			// rectDie : 實際計算得到的 Die座標 
			// nInsetDistanceX : rectDie 水平方向內縮距離
			// nInsetDistanceY : rectDie 垂直方內縮擴距離
			bool AdjustmentThermalGlueParameter_Glue_Method1_Step1_ROI(const Mat& matColor, const RECT& rectDie, const int& nInsetDistanceX, const int& nInsetDistanceY, Mat& matROI_Color);

			bool AdjustmentThermalGlueParameter_Glue_Method1_Step2_ToGray(const Mat& matROI_Color, const int& nMedianX, const int& nMedianY, Mat& matROI_Gray);

			bool AdjustmentThermalGlueParameter_Glue_Method1_Step3_Threshold(const Mat& matROI_Gray, const bool& bDark, const int& nThreshold, Mat& matROI_Threshold);

			bool AdjustmentThermalGlueParameter_Glue_Method1_Step4_End(const Mat& matROI_Color, const Mat& matROI_Threshold, const int& nGlueDirection, const int& nOpenX, const int& nOpenY, int& nGlueCount,  Mat& matEnd);
#pragma endregion

			// 調整 Coating參數
#pragma region Adjustment Coating Parameter

#pragma region Method1

			bool AdjustmentCoatingParameter_Method1_Step1_Median(const Mat& matColor, const int& nMedianX, const int& nMedianY, Mat& matMedian);

			bool AdjustmentCoatingParameter_Method1_Step2_ColorToThreshold(const Mat& matMedian, const int& nCoatingValue_R, const int& nCoatingValue_G, const int& nCoatingValue_B, const int& nCoatingValueTolerance, Mat& matThreshold);

			bool AdjustmentCoatingParameter_Method1_Step3_Open(const Mat& matThreshold, const int& nOpenX, const int& nOpenY, Mat& matOpen);

			bool AdjustmentCoatingParameter_Method1_Step4_Connect(const Mat& matOpen, const int& nConnectX, const int& nConnectY, const int& nSmoothX, const int& nSmoothY, Mat& matConnect);

			bool AdjustmentCoatingParameter_Method1_Step5_End(const Mat& matColor, const Mat& matConnect, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, Mat& matEnd);
			bool AdjustmentCoatingParameter_Method1_Step5_End(const Mat& matColor, const Mat& matConnect, const int& nCount, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, Mat& matEnd);
#pragma endregion

#pragma region Method2

			bool AdjustmentCoatingParameter_Method2_Step1_ColorMedianToGray(const Mat& matColor, const int& nMedianX, const int& nMedianY, Mat& matGray);

			bool AdjustmentCoatingParameter_Method2_Step2_Threshold(const Mat& matGray, const bool& bDark, const int& nThreshold, Mat& matThreshold);

			bool AdjustmentCoatingParameter_Method2_Step3_Open(const Mat& matThreshold, const int& nOpenX, const int& nOpenY, Mat& matOpen);

			bool AdjustmentCoatingParameter_Method2_Step4_Connect(const Mat& matOpen, const int& nConnectX, const int& nConnectY, const int& nSmoothX, const int& nSmoothY, Mat& matConnect);

			bool AdjustmentCoatingParameter_Method2_Step5_End(const Mat& matColor, const Mat& matConnect, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, Mat& matEnd);
			bool AdjustmentCoatingParameter_Method2_Step5_End(const Mat& matColor, const Mat& matConnect, const int& nCount, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, Mat& matEnd);

#pragma endregion

#pragma endregion

#pragma endregion

			// 調整找邊參數
#pragma region FindEdge Adjustment Parameter

			// Step 1:色彩轉換
			bool AdjustmentFindEdgeParameter_Step1_ColorTransform(const Mat& matColor, const int& nColorMode, Mat& matTransform);
			bool AdjustmentFindEdgeParameter_Step1_ColorTransform_GlueHeight(const Mat& matColor, const int& nColorMode, Mat& matTransform);

			// Step 2:中值濾波
			bool AdjustmentFindEdgeParameter_Step2_Median(const Mat& matTransform, const int& nFilterMode, const int& nMedianSize, Mat& matMedian);

			// Step 3: 影像抽色 
			bool AdjustmentFindEdgeParameter_Step3_Threshold(const Mat& matMedian, const SFindEdge_Parameter& sParam, Mat& matThreshold);
			bool AdjustmentFindEdgeParameter_Step3_Threshold_GlueHeight(const Mat& matMedian, const SFindEdge_Parameter& sParam, Mat& matThreshold);

			// Step 3-1: 彩色影像抽色 
			bool AdjustmentFindEdgeParameter_Step3_Color(const Mat& matMedian, const int& nColorMode, const int& nColorThresholdMode, const int& nColorCount, const vector<SColorExtraction>& vtsColorExtraction, const int& nHsvCount, const vector<SHsvExtraction>& vtsHsvExtraction, Mat& matThreshold);
			bool AdjustmentFindEdgeParameter_Step3_Color_GlueHeight(const Mat& matMedian, const int& nColorMode, const int& nColorThresholdMode, const int& nColorCount, const vector<SColorExtraction>& vtsColorExtraction, const int& nHsvCount, const vector<SHsvExtraction>& vtsHsvExtraction, Mat& matThreshold);

			// Step 3-2: 灰階影像雙閥值二值化 
			bool AdjustmentFindEdgeParameter_Step3_Gray(const Mat& matMedian, const int& nColorMode, const int& nCount, const vector<SGrayExtraction>& vtsGrayExtraction, Mat& matThreshold);

			// Step 4 : 二值化影像過濾雜訊
			bool AdjustmentFindEdgeParameter_Step4_Open(const Mat& matThreshold, const int& nOpenX, const int& nOpenY, Mat& matOpen);

			// Step 5 : 二值化影像連接
			bool AdjustmentFindEdgeParameter_Step5_Connect(const Mat& matOpen, const int& nConnectX, const int& nConnectY, Mat& matConnect);

			// Step 6 : End
			bool SetFindEdgeProcess_Adj_Step6_BaseObject(const SFindEdge_Parameter& sParam, SProcessModeParam& sPM);
			bool AdjustmentFindEdgeParameter_Step6_End(const Mat& matConnect, const Mat& matColor, const int& nDeleteMode, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, Mat& matEnd);
			bool AdjustmentFindEdgeParameter_Step6_IC_End(const Mat& matConnect, const Mat& matColor, const int& nMeasuringDirection, const int& nDeleteMode, Mat& matEnd);

#pragma endregion

#pragma region SPIL Glue Adjustment Parameter

			// Step 1:轉灰階 + 中值濾波
			bool AdjustmentSpilGluePartParameter_Step1_Gray(const Mat& matColor, const int& nMedianSize, const int& nMode, Mat& matGray);

			// Step 2: 彩色影像抽色 
			bool AdjustmentSpilGluePartParameter_Step2_Threshold(const Mat& matGray, const int& nCount, const vector<SGrayExtraction>& vtsGrayExtraction, Mat& matThreshold);

			// Step 3: 取出 IC區域 
			bool AdjustmentSpilGluePartParameter_Step3_IC(const Mat& matThreshold, const RECT& rectIC, Mat& matIC);

			// Step 3: 取出膠區域
			bool AdjustmentSpilGlueParameter_Step3_Area(const Mat& matThreshold, const int& nCount, const int& nOpenX, const int& nOpenY, Mat& matGlueThreshold);

			// Step 4: 影像強化
			bool AdjustmentSpilGlueParameter_Step4_Enhance(const Mat& matColor, const Mat& matGlueThreshold, const SEnhanceMode& sEnhance, Mat& matEnhance);
#pragma endregion

#pragma region SPIL Part Adjustment Parameter

			

			// Step 2:二值化
			bool AdjustmentSpilPartParameter_Step2_Threshold(const Mat& matGray, const RECT& rectIC, const bool& bDark, const int& nThreshold, Mat& matThreshold, Mat& matIC);

#pragma endregion

		private:

			// 檢測輸入參數
			bool Check_Input(const vector<Mat>& vtInputImage, SMeasurementBlackGlue_Parameter& sParam);
			bool Check_Input_Flux(const Mat& matImage, SMeasurementFluxArea_Parameter& sParam);
			bool Check_Input_HeightRatio(const Mat& matImage, SMeasurementGlueHeightRatio_Parameter& sParam);
			bool Check_Input_GlueArea(const Mat& matImage, SMeasurementGlueArea_Parameter& sParam);

			// 變數初始化
			void Initial();
			void Initial_FluxArea();
			void Initial_GlueHeight();
			void Initial_GlueArea();

#pragma region AMD Function

#pragma region GlueEdge

			// 設定找黑膠邊緣的處理流程
			// Method1=>找膠本體(抽色), Method2=>找膠輪廓(轉灰階+二值化)
			bool SetGlueProcess_FindContours(const SGlueEdge_Parameter& sParam, SProcessModeParam& sPM);
			bool SetGlueProcess_FindContours_Method1(const SGlueEdge_Parameter& sParam, SProcessModeParam& sPM);
			bool SetGlueProcess_FindContours_Method2(const SGlueEdge_Parameter& sParam, SProcessModeParam& sPM);
			bool RunProcess_FindGlueContours();
#pragma endregion

#pragma region BoardEdge

			// 設定找板邊直線的處理流程
			// Method1=>找板邊本體(抽色),
			bool SetBoardProcess_FindContours(const SBoardEdge_Parameter& sBoard, const int& nLocation, const int& nImageW, const int& nImageH, SProcessModeParam& sPM);
			bool SetBoardProcess_FindContours_Method1(const SBoardEdge_Parameter& sBoard, const int& nLocation, const int& nImageW, const int& nImageH, SProcessModeParam& sPM);
			bool SetBoardProcess_FindContours_Method2(const SBoardEdge_Parameter& sBoard, const int& nLocation, const int& nImageW, const int& nImageH, SProcessModeParam& sPM);
			bool RunBoardProcess_FindContours();

			bool Calculate_LinearEquation(const int& nLocation, SContoursParam& sBorderContours, SBoardEdge_Result& sBoardEdge, const bool& bSave = false);
			bool Calculate_LinearEquation_All(SContoursParam& sBorderContours, SBoardEdge_Result& sBoardEdge);
			bool Calculate_LinearEquation_Corner(SContoursParam& sBorderContours, SBoardEdge_Result& sBoardEdge);
			bool Calculate_LinearEquation_Vertical(SContoursParam& sBorderContours, SBoardEdge_Result& sBoardEdge);
			bool Calculate_LinearEquation_Horizontal(SContoursParam& sBorderContours, SBoardEdge_Result& sBoardEdge);

			// vt2ptLine[0] = top, vt2ptLine[1] = bottom, vt2ptLine[2] = left, vt2ptLine[3] = right
			bool ContoursToLine(const int& nSearchRangeX, const int& nSearchRangeY, const vector<POINT>& vtptContours, vector<vector<POINT>>& vt2ptLine);
#pragma endregion

#pragma region CoatingEdge

			// 找 Coating邊界的處理流程
			// Method1=>找Coating本體(抽色), Method2=>找Coating輪廓(轉灰階+二值化)
			bool SetCoatingProcess_FindContours(const SCoatingEdge_Parameter& sCoating, SProcessModeParam& sPM);
			bool SetCoatingProcess_FindContours_Method1(const SCoatingEdge_Parameter& sCoating, SProcessModeParam& sPM);
			bool SetCoatingProcess_FindContours_Method2(const SCoatingEdge_Parameter& sCoating, SProcessModeParam& sPM);
			bool RunSetCoatingProcess_FindContours();
#pragma endregion

#pragma region ThermalGlue
			bool SetThermalGlueProcess_Find_Die_Contours(const SThermalGlue_Parameter& sThermal, SProcessModeParam& sPM);
			bool SetThermalGlueProcess_Find_Die_Contours_Method1(const SThermalGlue_Parameter& sThermal, SProcessModeParam& sPM);
			bool RunProcess_ThermalGlue_Find_Die_Contours();

			bool SetThermalGlueProcess_Find_Glue_Contours(const SThermalGlue_Parameter& sThermal, const RECT& rectDie, SProcessModeParam& sPM);
			bool SetThermalGlueProcess_Find_Glue_Contours_Method1(const SThermalGlue_Parameter& sThermal, const RECT& rectIC_ROI, SProcessModeParam& sPM);
			bool RunProcess_ThermalGlue_Find_Glue_Contours();

			// 將ROI的輪廓的分成4條邊界線
			// vt2ptLine[0] = top, vt2ptLine[1] = bottom, vt2ptLine[2] = left, vt2ptLine[3] = right
			bool ROI_ContoursToLine(const int& nSearchRangeX, const int& nSearchRangeY, SContoursParam* psContours, vector<vector<POINT>>& vt2ptLine);

			// vtdDieSlope[0]=top, vtdDieSlope[1]=bottom, vtdDieSlope[2]=left, vtdDieSlope[3]=right
			bool ReCreateDie(const int& nType, vector<vector<POINT>>& vt2ptLine, RECT& rectDie, vector<POINT>& vtptPoint, vector<double>& vtdDieSlope, vector<double>& vtdDieIntercept);

			bool Sort_ROI(const int& nGlueDirection, vector<SJRect*>& vtpjRect, vector<RECT>& vtrectSort);
#pragma endregion

#pragma region ROI Position

			// 計算ROI的位置
			bool Calculate_RoiPosition(const bool& bChange, const int& nCount, const vector<RECT>& vtrectROI, SMultipleROI_Result& sResult);

			// 計算ROI的位置-依輸入位置
			bool Calculate_RoiPosition_Type1(const int& nCount, const vector<RECT>& vtrectROI, SMultipleROI_Result& sResult);

			// 計算ROI的位置-自動計算
			bool Calculate_RoiPosition_Type2(const int& nCount, const vector<RECT>& vtrectROI, SMultipleROI_Result& sResult);

			// 計算ROI的位置-自動切換模式
			bool Calculate_RoiPosition_Change(const int& nCount, const vector<RECT>& vtrectROI, SMultipleROI_Result& sResult);

			// 計算散熱膠的量測範圍
			bool Calculate_ThermalGlueMeasurementRange(const RECT& rectDieROI, RECT& rectRange);

			// Get ROI
			bool GetSortROI(SContoursParam* psContours, const int& nCount, vector<RECT>& vtrectROI, vector<vector<POINT>>& vt2ptContoursPos);

#pragma region StartEnd

			// 計算 StartROI 位置
			bool Calculate_StartROI(const bool& bChange);

			// StartROI-依輸入位置
			bool Calculate_StartROI_Type1(const int nId);

			// StartROI-自動計算
			bool Calculate_StartROI_Type2(const int nId);

			// StartROI-自動切換模式
			bool Calculate_StartROI_Change(const int nId);

			// 計算 EndROI 位置
			bool Calculate_EndROI(const bool& bChange);

			// EndROI-依輸入位置
			bool Calculate_EndROI_Type1(const int nId);

			// EndROI-自動計算
			bool Calculate_EndROI_Type2(const int nId);

			// EndROI-自動切換模式
			bool Calculate_EndROI_Change(const int nId);

			// 計算 StartROI EndROI 的弧線端點
			bool GetLimitPoint(const Mat& matGlueEdge, const RECT& rectROI, const int& nLocation, POINT& ptPos);
			bool GetLimitPoint_Location1(const Mat& matGlueEdge, const int& nT, const int& nB, const int& nL, const int& nR, POINT& ptPoint);
			bool GetLimitPoint_Location2(const Mat& matGlueEdge, const int& nT, const int& nB, const int& nL, const int& nR, POINT& ptPoint);
			bool GetLimitPoint_Location3(const Mat& matGlueEdge, const int& nT, const int& nB, const int& nL, const int& nR, POINT& ptPoint);
			bool GetLimitPoint_Location4(const Mat& matGlueEdge, const int& nT, const int& nB, const int& nL, const int& nR, POINT& ptPoint);
			bool GetLimitPoint_Location5(const Mat& matGlueEdge, const int& nT, const int& nB, const int& nL, const int& nR, POINT& ptPoint);
			bool GetLimitPoint_Location6(const Mat& matGlueEdge, const int& nT, const int& nB, const int& nL, const int& nR, POINT& ptPoint);
			bool GetLimitPoint_Location7(const Mat& matGlueEdge, const int& nT, const int& nB, const int& nL, const int& nR, POINT& ptPoint);
			bool GetLimitPoint_Location8(const Mat& matGlueEdge, const int& nT, const int& nB, const int& nL, const int& nR, POINT& ptPoint);
#pragma endregion

			// 使用仿射轉換進行座標轉換
			bool CoordinateTransformation_Affine();

			// 進行仿射轉換
			// U = a11*x + a12*y + tx
			// V = a21*x + a22*y + ty
			// a11=vtdCoefficient[0], a12=vtdCoefficient[1], tx=vtdCoefficient[2]
			// a21=vtdCoefficient[3], a22=vtdCoefficient[4], ty=vtdCoefficient[5]
			bool AffineTransform_RECT(const vector<double> &vtdCoefficient, const RECT& rectROI, RECT& rectAffineROI);

			// 輸入邊界影像與ROI, 取出ROI內的 線段
			bool CalculatePosition_Arc(const Mat& matEdge, const RECT& rectROI, SContoursParam& sContours);
#pragma endregion

#pragma region Measurement

			// 水平膠寬量測
			bool Measurement_GlueWidth_Horizontal(const bool& bChange);
			bool Measurement_GlueWidth_Horizontal();

			// 垂直膠寬量測
			bool Measurement_GlueWidth_Vertical(const bool& bChange);	
			bool Measurement_GlueWidth_Vertical();

			// 弧邊膠寬量測
			bool Measurement_GlueWidth_Arc(const bool& bChange);
			bool Measurement_GlueWidth_Arc();

			// 量測膠首尾距離
			bool Measurement_StartEnd(const bool& bChange);

			// 水平方向板邊到膠邊的距離
			bool Measurement_BorderToGlue_Horizontal(const bool& bChange);
			bool Measurement_BorderToGlue_Horizontal_Slope();	// 使用板邊計算斜率
			bool Measurement_BorderToGlue_Horizontal_Defulat();	// 使用水平斜率

			// 垂直方向板邊到膠邊的距離
			bool Measurement_BorderToGlue_Vertical(const bool& bChange);
			bool Measurement_BorderToGlue_Vertical_Slope();		// 使用板邊計算斜率
			bool Measurement_BorderToGlue_Vertical_Defulat();	// 使用垂直斜率

			// 水平方向Coating邊到膠邊的距離
			bool Measurement_CoatingToGlue_Horizontal(const bool& bChange);
			bool Measurement_CoatingToGlue_Horizontal();

			// 垂直方向Coating邊到膠邊的距離
			bool Measurement_CoatingToGlue_Vertical(const bool& bChange);
			bool Measurement_CoatingToGlue_Vertical();

			// 水平方向Coating邊到板邊的距離
			bool Measurement_CoatingToBoard_Horizontal();
			bool Measurement_CoatingToBoard_Horizontal_Slope(); 
			bool Measurement_CoatingToBoard_Horizontal_Defulat();

			// 垂直方向Coating邊到板邊的距離
			bool Measurement_CoatingToBoard_Vertical();
			bool Measurement_CoatingToBoard_Vertical_Slope();
			bool Measurement_CoatingToBoard_Vertical_Defulat();

			// Thermal Glue---GlueWidth
			bool Measurement_Thermal_GlueWidth();
			bool Measurement_Thermal_GlueWidth_New();

			// 膠水平排列, 垂直量測
			bool Measurement_Thermal_GlueWidth_Slope_V();

			// 膠垂直排列, 水平量測
			bool Measurement_Thermal_GlueWidth_Slope_H();

			bool Measurement_Thermal_GlueWidth_Defulat();

			// Thermal Glue---GlueToGlue
			bool Measurement_Thermal_GlueToGlue();

			// Thermal Glue---GlueToDie
			bool Measurement_Thermal_GlueToDie();

			// 計算多點平均 20251229
			bool CalcMeanDist(const int nMeanCount, SSingleROI_Result& sResult);
	
			// 計算首尾的多點平均距離
			bool CalcMeanDist_StartEnd(const int nMeanCount, const Mat& matGlueEdge, SSingleStartEndROI_Result& sResult);

#pragma endregion

#pragma region OutputResult

			// 將輪廓點位移
			bool ContoursShift(const int& nShiftX, const int& nShiftY, SContoursParam* psContours);

			// 將輪廓點從 psContours 轉成 vtptContoursPos
			bool ContoursConvertVector(SContoursParam* psContours, vector<POINT>& vtptContoursPos);
			bool ContoursConvertVector(SContoursParam* psContours, vector<POINT>& vtptContoursPos1, vector<POINT>& vtptContoursPos2);
			bool ContoursConvertMultipleVector(SContoursParam* psContours, vector<SJRect*>& vtpjRect, vector<vector<POINT>>& vt2ptContoursPos);

			// 輸出距離
			// nMode: 1=> 輸出最大值, 2=> 輸出最小值, 3=>平均值
			// nSortIndex : 當nMode為3時, 1=輸出最大值, 100=輸出最小值(值域1-100)
			bool OutputDistance(const int& nMode, const int& nSortIndex, const vector<SMeasurementResult>& vtsMeasurementInfo, int& nId);

			// 輸出最大 最小距離
			bool OutputMaxMinDistance(const vector<SMeasurementResult>& vtsMeasurementInfo, SMeasurementResult& sMaxDist, SMeasurementResult& sMinDist);

			// 輸出最大 最小距離 與 最大-最小
			bool OutputMaxMinDistance(const vector<SMeasurementResult>& vtsMeasurementInfo, SSingleROI_Result& sResult);

			// 判斷量測結果的範圍
			bool JudgmentResultRange(const float& fDistance, SLimit_Single& sLimit);

			// 計算2點距離
			void Calculate_Distance(SMeasurementResult& sDist);
			
#pragma endregion

#pragma region Flux Area

			// 找Flux面積邊界
			bool RunProcess_FindFluxArea();

			// 輸出Flux面積比例
			bool OutputFluxArea();

#pragma endregion

#pragma endregion

#pragma region Draw

			// 顯示膠的輪廓
			// nMode : 選擇哪一張影像當底圖 ; 1=>膠影像 ; 2=>板邊影像 ; 3=>Coating影像 ; 4=>matDisplay
			// nLineWidth : 線寬, 0=>1 pixels, 1=>3 pixels ...
			// matDisplay : 輸入時可為空, 輸出時為彩色格式
			bool DisplayGlueContours(const int nMode, const int& nLineWidth, Mat& matDisplay);

			// 顯示板邊的輪廓
			// nMode : 選擇哪一張影像當底圖 ; 1=>膠影像 ; 2=>板邊影像 ; 3=>Coating影像 ; 4=>matDisplay
			// matDisplay : 輸入時可為空, 輸出時為彩色格式
			bool DisplayBoardContours(const int nMode, const int& nLineWidth, Mat& matDisplay);

			// 顯示Coating的輪廓
			// nMode : 選擇哪一張影像當底圖 ; 1=>膠影像 ; 2=>板邊影像 ; 3=>Coating影像 ; 4=>matDisplay
			// matDisplay : 輸入時可為空, 輸出時為彩色格式
			bool DisplayCoatingContours(const int nMode, const int& nLineWidth, Mat& matDisplay);

			// nMode : 選擇哪一張影像當底圖 ; 1=>膠影像 ; 2=>板邊影像 ; 3=>Coating影像 ; 4=>matDisplay
			bool AssignImage(const int nMode, Mat& matShow);

			// 畫出膠邊或板邊
			// nMode : 1=> 膠邊; 2=>板邊; 3=>Coating
			// matEdge = 輸出的影像(灰階影像, 邊為255, 背景為0)
			bool DrawEdge(const int& nLocation, const int& nMode, Mat& matEdge);

			// nMode : 1=> 膠邊; 3=>Coating
			bool DrawMask(const int& nMode, const int& nValue, Mat& matMask);

			// 將邊界內的範圍填滿
			bool Convex_Fill(const POINT* pContoursPos, const int& nCount, const int& nValue, Mat& matDisplay);
#pragma endregion

#pragma region Find Edge

			bool SetFindEdgeProcess(const SFindEdge_Parameter& sParam, SProcessModeParam& sPM);
			bool SetFindEdgeProcess_GlueHeight(const SFindEdge_Parameter& sParam, SProcessModeParam& sPM);
			bool SetFindEdgeProcess_GlueHeight_IC(const SFindEdge_Parameter& sParam, SProcessModeParam& sPM);

			bool ROI_ContoursToLine(const int& nSearchRangeX, const int& nSearchRangeY, const int& nRange, SContoursParam* psContours, vector<vector<POINT>>& vt2ptLine, Rect& cvROI);
			bool ROI_ContoursToLine_Connector(const int& nSearchRangeX, const int& nSearchRangeY, const int& nRange, SContoursParam* psContours, vector<vector<POINT>>& vt2ptLine, Rect& cvROI);

			// 計算IC邊到膠邊的距離
			void Calculate_Distance(const float& fResolutionX, const float& fResolutionY, Pixels_MeasurementInfo& sDist);

			// 輸出最大距離 
			bool OutputDistance_Max(SingleBlockGlueHeight_Result& sSingleResult);
			bool OutputDistance_Max(MultipleBlockGlueHeight_Result& sMultipleResult);

			// 輸出最小距離
			bool OutputDistance_Min(SingleBlockGlueHeight_Result& sSingleResult);
			bool OutputDistance_Min(MultipleBlockGlueHeight_Result& sMultipleResult);

			// 輸出平均距離
			bool OutputDistance_Mean(SingleBlockGlueHeight_Result& sSingleResult);
			bool OutputDistance_Mean(MultipleBlockGlueHeight_Result& sMultipleResult);

			// 輸出(最大-最小)距離
			bool OutputDistance_MaxMin(SingleBlockGlueHeight_Result& sSingleResult);
			bool OutputDistance_MaxMin(MultipleBlockGlueHeight_Result& sMultipleResult);

			// 輸出(平均-最小)距離
			bool OutputDistance_MeanMin(SingleBlockGlueHeight_Result& sSingleResult);
			bool OutputDistance_MeanMin(MultipleBlockGlueHeight_Result& sMultipleResult);
#pragma endregion

#pragma region Glue Height Ratio

			// 找膠高邊界
			bool RunProcess_FindGlueHeightEdge();

			// 找IC邊界
			bool RunProcess_FindICEdge();

			// 重建 IC邊界
			bool Rebuilding_ICEdge_Apple(SContoursParam* psContours, vector<POINT>& vtptContoursPos);

			// 20251015 重建 Connector 基線邊界
			bool Rebuilding_Connector_BaseEdge(SContoursParam* psContours, vector<POINT>& vtptContoursPos);

			// 20251015 重建 基線邊界---直接用外接矩形, 不使用線性回歸
			bool Rebuilding_BaseEdge_ROI(SContoursParam* psContours, vector<POINT>& vtptContoursPos);

			// 計算 IC邊 到 膠邊的距離
			bool Calculate_IcToGlue();

			// 計算 膠高 與 高度比例
			bool Calculate_GluetHeightRatio();

			// nMode : 高度模式, 1=>最高, 2=>平均, 3=>最低, 4=>最高-最低, 5=>平均-最低
			// 計算每個區塊的輸出結果
			bool OutputHeightRatio();
#pragma endregion

#pragma region Glue Area Ratio

			// 找IC範圍
			bool RunProcess_FindIcRange();

			// 重建 IC範圍 (4個邊都要重建)
			// vt2ptContoursPos : [0]=上, [1]=下, [2]=左, [3]=右
			// vtptCorners : [0]=LT, [1]=RT, [2]=RB, [3]=LB
			bool Rebuilding_IC_Range(const int nIndent_W, const int nIndent_H, SContoursParam* psContours, vector<vector<POINT>>& vt2ptContoursPos, vector<POINT>& vtptCorners);

			// 找膠面積邊界
			bool RunProcess_FindGlueArea();

			// 輸出膠面積比例
			bool OutputGlueArea();

#pragma endregion

#pragma region SPIL GlueToPart
			bool SetSpilGluePartProcess(const SFindEdge_Parameter& sParam, SProcessModeParam& sPM);
			bool SetSpilPartProcess(const SFindEdge_Parameter& sParam, const RECT& rectIC, SProcessModeParam& sPM);

			bool RunProcess_SpilGlue(const SFindEdge_Parameter& sParam, SProcessModeParam& sPM);

#pragma endregion

			// nExtensionType = 1, jpg
			//                = 2, bmp
			//                = 3, png
			bool SaveImage(const int nSaveLevel, const Mat& matImage, const string strTxt, int nExtensionType = 1);

			bool ParameterToConfig(const SMeasurementBlackGlue_Parameter& sParam, vector<ConfigData>& vtsConfig);
			bool ConfigToParameter(const std::vector<ConfigData>& vtsConfig, SMeasurementBlackGlue_Parameter& sParam);

			// 儲存參數
			bool Save_Parameter();
		};
	}
}

