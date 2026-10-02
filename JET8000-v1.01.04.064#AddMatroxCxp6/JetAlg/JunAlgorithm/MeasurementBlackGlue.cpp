#include "StdAfx.h"

#include <numeric>  
#include <unordered_map>
#include <unordered_set>

#include "MeasurementBlackGlue.h"
#include "ImageProcessBaseStruct.h"


namespace JET {
	namespace alg {
		
		// 黑膠量測
		// [In] vtInputImage 的 size 為 4
		// [In] vtInputImage[0] = 量測膠邊的彩色影像
		// [In] vtInputImage[1] = 量測板邊的彩色影像
		// [In] vtInputImage[2] = 量測散熱膠的彩色影像
		// [In] vtInputImage[3] = 量測coating的彩色影像
		// [In] sParam : 黑膠量測的相關參數
		// [Out] sResult: 黑膠量測的結果
		bool MeasurementBlackGlue::Measurement(const vector<Mat>& vtInputImage, SMeasurementBlackGlue_Parameter& sParam, SMeasurementBlackGlue_Result& sResult)
		{
			m_nSaveIndex = 0;
			m_strErrorMessage = "OK";
			if (!Check_Input(vtInputImage, sParam)) {
				Save_Parameter();
				return false;
			}

			// 相關變數初始化
			Initial();

#pragma region Find Contours

			// 找膠邊界
			if (m_sParam.sGlue.bEnable) {
				m_sResult.sGlueEdge.bEnable = true;
				if (!RunProcess_FindGlueContours()) {
					m_strErrorMessage = "FindGlueContours Error";
					Save_Parameter();
					return false;
				}
			}

			// 找板邊
			if (m_sParam.sBoard.bEnable_Board) {
				m_sResult.sBoardEdge.bEnable = true;
				if (!RunBoardProcess_FindContours()) {
					m_strErrorMessage = "FindBorderContours Error";
					Save_Parameter();
					return false;
				}
			}

			// 找Coating邊界
			if (m_sParam.sCoating.bEnable) {
				m_sResult.sCoatingEdge.bEnable = true;
				if (!RunSetCoatingProcess_FindContours()) {
					m_strErrorMessage = "FindCoatingContours Error";
					Save_Parameter();
					return false;
				}
			}
			SaveImage(0, m_matEdgeMask, "Measurement Contours", m_nExtensionType);
#pragma endregion

#pragma region Glue StartEnd

			// 要開啟膠邊偵測 才能進行 膠首尾距離量測
			if (m_sResult.sGlueEdge.bEnable && m_sParam.sGlueStartEnd.bEnable) {
				m_sResult.sStartEnd.bEnable = true;
				m_sResult.sStartEnd.nCount = m_sParam.sGlueStartEnd.nCount;
				m_sResult.sStartEnd.vtsStartEnd.resize(m_sResult.sStartEnd.nCount);

				// 計算 StartROI 的位置
				bool bChange = true;
				bool bOk_StartEnd = Calculate_StartROI(bChange);
				if (bOk_StartEnd)  {
					// 計算 EndROI 的位置
					bOk_StartEnd = Calculate_EndROI(bChange);
					if (bOk_StartEnd) {
						// 量測StartROI 與 EndROI的距離
						bOk_StartEnd = Measurement_StartEnd(bChange);
						if (bOk_StartEnd) {
							m_strErrorMessage = "CoordinateTransformation Error";
							Save_Parameter();
						}
					}
					else {
						m_strErrorMessage = "Calculate_EndROI Error";
						Save_Parameter();
					}
				}
				else {
					m_strErrorMessage = "Calculate_StartROI Error";
					Save_Parameter();
				}
			}
#pragma endregion

#pragma region Glue Width

			// 膠邊要開啟偵測 才能測量膠寬
			if (m_sResult.sGlueEdge.bEnable) {

#pragma region GlueWidth_Horizontal
				// 水平膠寬量測
				if (m_sParam.sGlueWidth_Horizontal.bEnable) {
					m_sResult.sWidth_Horizontal.bEnable = true;

					// 計算ROI位置
					bool bChange = true;
					bool bOk_Width_Horizontal = Calculate_RoiPosition(bChange, m_sParam.sGlueWidth_Horizontal.nCount, m_sParam.sGlueWidth_Horizontal.vtrectROI, m_sResult.sWidth_Horizontal);
					if (bOk_Width_Horizontal) {
						// 量測水平膠寬
						bOk_Width_Horizontal = Measurement_GlueWidth_Horizontal(bChange);
						if (!bOk_Width_Horizontal) {
							m_strErrorMessage = "Measurement_GlueWidth_Horizontal Error";
							Save_Parameter();
						}
					}
					else {
						m_strErrorMessage = "Calculate_GlueWidthROI_Horizontal Error";
						Save_Parameter();
					}
				}
#pragma endregion

#pragma region GlueWidth_Vertical

				// 垂直膠寬量測
				if (m_sParam.sGlueWidth_Vertical.bEnable) {
					m_sResult.sWidth_Vertical.bEnable = true;

					// 計算ROI位置
					bool bChange = true;
					bool bOk_Width_Vertical = Calculate_RoiPosition(bChange, m_sParam.sGlueWidth_Vertical.nCount, m_sParam.sGlueWidth_Vertical.vtrectROI, m_sResult.sWidth_Vertical);
					if (bOk_Width_Vertical) {
						bOk_Width_Vertical = Measurement_GlueWidth_Vertical(bChange);
						if (!bOk_Width_Vertical) {
							// 量測垂直膠寬
							m_strErrorMessage = "Measurement_GlueWidth_Vertical Error";
							Save_Parameter();
						}
					}
					else {
						m_strErrorMessage = "Calculate_GlueWidthROI_Vertical Error";
						Save_Parameter();
					}
				}
#pragma endregion

#pragma region Width_Arc

				// 弧線膠寬量測 
				if (m_sParam.sGlueArc.bEnable) {
					m_sResult.sWidth_Arc.bEnable = true;

					// 計算ROI位置
					bool bChange = true;
					bool bOk_Width_Arc = Calculate_RoiPosition(bChange, m_sParam.sGlueArc.nCount, m_sParam.sGlueArc.vtrectROI, m_sResult.sWidth_Arc);
					if (bOk_Width_Arc) {
						bOk_Width_Arc = Measurement_GlueWidth_Arc(bChange);
						if (!bOk_Width_Arc)  {
							m_strErrorMessage = "Measurement_GlueWidth_Arc Error";
							Save_Parameter();
						}
					}
					else {
						m_strErrorMessage = "Calculate_GlueWidthROI_Arc Error";
						Save_Parameter();
					}
				}
#pragma endregion
			}
#pragma endregion

#pragma region BoardToGlue

			// 膠邊 板邊 都要開啟偵測 才能測量
			if (m_sResult.sGlueEdge.bEnable && m_sResult.sBoardEdge.bEnable) {

				// 水平方向板邊到膠邊距離量測
				if (m_sParam.sBoardToGlue_Horizontal.bEnable) {
					m_sResult.sBoardToGlue_Horizontal.bEnable = true;

					// 計算ROI位置
					bool bChange = true;
					bool bOk_BoardToGlue_Horizontal = Calculate_RoiPosition(bChange, m_sParam.sBoardToGlue_Horizontal.nCount, m_sParam.sBoardToGlue_Horizontal.vtrectROI, m_sResult.sBoardToGlue_Horizontal);
					if (bOk_BoardToGlue_Horizontal) {
						// 量測板邊到膠邊的距離
						bOk_BoardToGlue_Horizontal = Measurement_BorderToGlue_Horizontal(bChange);
						if (!bOk_BoardToGlue_Horizontal) {
							m_strErrorMessage = "Measurement_BoardToGlue_Horizontal Error";
							Save_Parameter();
						}
					}
					else {
						m_strErrorMessage = "Calculate_BoardToGlue_ROI_Horizontal Error";
						Save_Parameter();
					}
				}

				// 垂直方向板邊到膠邊距離量測
				if (m_sParam.sBoardToGlue_Vertical.bEnable) {
					m_sResult.sBoardToGlue_Vertical.bEnable = true;

					// 計算ROI位置
					bool bChange = true;
					bool bOk_BoardToGlue_Vertical = Calculate_RoiPosition(bChange, m_sParam.sBoardToGlue_Vertical.nCount, m_sParam.sBoardToGlue_Vertical.vtrectROI, m_sResult.sBoardToGlue_Vertical);
					if (bOk_BoardToGlue_Vertical) {
						// 量測板邊到膠邊的距離
						bOk_BoardToGlue_Vertical = Measurement_BorderToGlue_Vertical(bChange);
						if (!bOk_BoardToGlue_Vertical) {
							m_strErrorMessage = "Measurement_BoardToGlue_Vertical Error";
							Save_Parameter();
						}
					}
					else {
						m_strErrorMessage = "Calculate_BoardToGlue_ROI_Vertical Error";
						Save_Parameter();
					}
				}
			}
#pragma endregion

#pragma region CoatingToGlue

			// 膠邊 Coating 都要開啟偵測 才能測量
			if (m_sResult.sGlueEdge.bEnable && m_sResult.sCoatingEdge.bEnable) {

				// 水平方向Coating到膠邊距離量測
				if (m_sParam.sCoatingToGlue_Horizontal.bEnable) {
					m_sResult.sCoatingToGlue_Horizontal.bEnable = true;

					// 計算ROI位置
					bool bChange = true;
					bool bOk_CoatingToGlue_Horizontal = Calculate_RoiPosition(bChange, m_sParam.sCoatingToGlue_Horizontal.nCount, m_sParam.sCoatingToGlue_Horizontal.vtrectROI, m_sResult.sCoatingToGlue_Horizontal);
					if (bOk_CoatingToGlue_Horizontal) {
						// 量測膠邊到板邊的距離
						bOk_CoatingToGlue_Horizontal = Measurement_CoatingToGlue_Horizontal(bChange);
						if (!bOk_CoatingToGlue_Horizontal) {
							m_strErrorMessage = "Measurement_CoatingToGlue_Horizontal Error";
							Save_Parameter();
						}
					}
					else {
						m_strErrorMessage = "Calculate_CoatingToGlue_Horizontal Error";
						Save_Parameter();
					}
				}

				// 垂直方向板邊到膠邊距離量測
				if (m_sParam.sCoatingToGlue_Vertical.bEnable) {
					m_sResult.sCoatingToGlue_Vertical.bEnable = true;

					// 計算ROI位置
					bool bChange = true;
					bool bOk_CoatingToGlue_Vertical = Calculate_RoiPosition(bChange, m_sParam.sCoatingToGlue_Vertical.nCount, m_sParam.sCoatingToGlue_Vertical.vtrectROI, m_sResult.sCoatingToGlue_Vertical);
					if (bOk_CoatingToGlue_Vertical) {
						// 量測膠邊到板邊的距離
						bOk_CoatingToGlue_Vertical = Measurement_CoatingToGlue_Vertical(bChange);
						if (!bOk_CoatingToGlue_Vertical) {
							m_strErrorMessage = "Measurement_CoatingToGlue_Vertical Error";
							Save_Parameter();
						}
					}
					else {
						m_strErrorMessage = "Calculate_CoatingToGlue_Vertical Error";
						Save_Parameter();
					}
				}
			}
#pragma endregion

#pragma region CoatingToBoard

			// 板邊 Coating 都要開啟偵測 才能測量
			if (m_sResult.sCoatingEdge.bEnable && m_sResult.sBoardEdge.bEnable) 
			{
				// 水平方向Coating到板邊距離量測
				if (m_sParam.sCoatingToBoard_Horizontal.bEnable) {
					m_sResult.sCoatingToBoard_Horizontal.bEnable = true;
					if (!Measurement_CoatingToBoard_Horizontal()) {
						m_strErrorMessage = "Measurement_CoatingToBoard_Horizontal Error";
						return false;
					}
				}

				// 垂直方向Coating到板邊距離量測
				if (m_sParam.sCoatingToBoard_Vertical.bEnable) {
					m_sResult.sCoatingToBoard_Vertical.bEnable = true;
					if (!Measurement_CoatingToBoard_Vertical()) {
						m_strErrorMessage = "Measurement_CoatingToBoard_Vertical Error";
						return false;
					}
				}
			}
#pragma endregion

#pragma region ThermalGlue
			if (m_sParam.sThermal.bEnable) {
				m_sResult.sThermalGlue.bEnable = true;
				m_sResult.sThermalGlue.nGlue_Count = m_sParam.sThermal.nGlue_Count;

				if (!RunProcess_ThermalGlue_Find_Die_Contours()) {
					m_strErrorMessage = "ThermalGlue_Find_Die_Contours Error";
					Save_Parameter();
				}

				if (!Calculate_ThermalGlueMeasurementRange(m_sResult.sThermalGlue.rectDie, m_rectThermalGlueMeasurementRange)) {
					m_strErrorMessage = "Calculate_ThermalGlueMeasurementRange Error";
					Save_Parameter();
				}

				if (!RunProcess_ThermalGlue_Find_Glue_Contours()) {
					m_strErrorMessage = "ThermalGlue_Find_Glue_Contours Error";
					Save_Parameter();
				}	
				

				if (!Measurement_Thermal_GlueWidth()) {
					m_strErrorMessage = "Measurement_Thermal_GlueWidth Error";
					Save_Parameter();
				}

				if (!Measurement_Thermal_GlueToGlue()) {
					m_strErrorMessage = "Measurement_Thermal_GlueToGlue Error";
					Save_Parameter();
				}

				if (!Measurement_Thermal_GlueToDie()) {
					m_strErrorMessage = "Measurement_Thermal_GlueToDie Error";
					Save_Parameter();
				}
			}
#pragma endregion

			// 顯示結果影像
			sResult = m_sResult;
			if (m_bSaveImage) {
				Save_Parameter();
				Mat matDisplay = m_matGlue.clone();
				if (!ShowResult(m_sResult, matDisplay)) {
					m_strErrorMessage = "ShowResult Error";
					return false;
				}
				SaveImage(0, matDisplay, "Result", m_nExtensionType);
			}

			return true;
		}

		// Flux面積量測
		// [In] matInputImage = Flux量測的彩色影像(使用低角度白光)
		// [In] sParam : Flux量測的相關參數
		// [Out] sResult: Flux量測的結果
		bool MeasurementBlackGlue::Measurement_FluxArea(const Mat& matInputImage, SMeasurementFluxArea_Parameter& sParam, SMeasurementFluxArea_Result& sResult)
		{
			m_nSaveIndex = 0;
			m_strErrorMessage = "OK";
			if (!Check_Input_Flux(matInputImage, sParam)) {
				return false;
			}

			// 相關變數初始化
			Initial_FluxArea();

			// 找Flux邊界
			if (!RunProcess_FindFluxArea()) {
				m_strErrorMessage = "RunProcess_FindFluxArea Error";
				return false;
			}

			if (!OutputFluxArea()) {
				m_strErrorMessage = "OutputFluxArea Error";
				Save_Parameter();
				return false;
			}

			sResult = m_sResult_FluxArea;

			if (m_bSaveImage) {
				Mat matResult = m_matGlue.clone();
				vector<string> vtstrResult; 
				bool bResult;
				ShowResult_FluxArea(sResult, matResult, vtstrResult, bResult, 3);
				SaveImage(0, matResult, "_Result", m_nExtensionType);
			}

			return true;
		}

		// 膠高比例量測
		bool MeasurementBlackGlue::Measurement_GlueHeightRatio(const Mat& matInputImage, SMeasurementGlueHeightRatio_Parameter& sParam, SMeasurementGlueHeightRatio_Result& sResult)
		{
			m_nSaveIndex = 0;
			m_strErrorMessage = "OK";
			if (!Check_Input_HeightRatio(matInputImage, sParam)) {
				return false;
			}

			// 相關變數初始化
			Initial_GlueHeight();

			// 找IC邊界
			if (!RunProcess_FindICEdge()) {
				return false;
			}
			
			// 找膠高邊界
			if (!RunProcess_FindGlueHeightEdge()) {
				return false;
			}

			// 計算 IC 到 膠的距離
			if (!Calculate_IcToGlue()) {
				m_strErrorMessage = "Calculate_IcToGlue Error";
				return false;
			}

			// 計算 膠高 與 高度比例
			if (!Calculate_GluetHeightRatio()) {
				m_strErrorMessage = "Calculate_GluetHeightRatio Error";
				return false;
			}
			
			// 計算每個區塊的輸出結果
			if (!OutputHeightRatio()) {
				m_strErrorMessage = "OutputHeightRatio Error";
				return false;
			}

			if (m_bSaveImage) {
				Mat matDisplay;
				jet_imagefunction::LogTransform(m_matGlue, matDisplay);
				jet_imagefunction::DrawEdgePoint(matDisplay, m_sResult_GlueHeight.sGlueEdge.vtptContoursPos, Scalar(0,255,255), 0);
				jet_imagefunction::DrawEdgePoint(matDisplay, m_sResult_GlueHeight.sIcEdge.vtptContoursPos, Scalar(255, 0, 255), 0);
				SaveImage(0, matDisplay, "Edge", m_nExtensionType);
			}

			sResult = m_sResult_GlueHeight;

			return true;
		}

		// 膠面積偵測
		bool MeasurementBlackGlue::Measurement_GlueArea(const Mat& matInputImage, SMeasurementGlueArea_Parameter& sParam, SMeasurementGlueArea_Result& sResult)
		{
			m_nSaveIndex = 0;
			m_strErrorMessage = "OK";

			// 檢查輸入參數
			if (!Check_Input_GlueArea(matInputImage, sParam)) {
				return false;
			}

			// 相關變數初始化
			Initial_GlueArea();

			// 找IC範圍
			if (!RunProcess_FindIcRange()) {
				m_strErrorMessage = "RunProcess_FindIcRange Error";
				return false;
			}

			// 找膠邊界
			if (!RunProcess_FindGlueArea()) {
				m_strErrorMessage = "RunProcess_FindGlueArea Error";
				return false;
			}

			if (!OutputGlueArea()) {
				m_strErrorMessage = "OutputGlueArea Error";
				Save_Parameter();
				return false;
			}

			sResult = m_sResult_GlueArea;

			return true;
		}
#pragma region Display

		// AMD 黑膠---將量測結果畫在 matResult-不顯示量測數據
		// [In]      SMeasurementBlackGlue_Result : 黑膠量測的結果
		// [In][Out] matResult : 顯示最終量測結果的影像, 輸入時需為彩色影像
		// [In]      nContourWidth = 輪廓或ROI線的寬, 0=>1 pixels; 1=>3 pixels ...
		// [In]      nLineWidth = 量測線的寬, 1=>1 pixels; 2=>2 pixels ...
		bool MeasurementBlackGlue::ShowResult(const SMeasurementBlackGlue_Result& sResult, Mat& matResult, const int& nContourWidth, const int& nLineWidth)
		{
			if (matResult.empty() || matResult.channels() != 3) {
				return false;
			}

			// 顯示板邊資訊
			if (!DisplayBoardPosition(sResult.sBoardEdge, m_scContours_Board, m_scLine_Board, matResult, nContourWidth, nLineWidth)) {
				return false;
			}

			// 畫膠輪廓
			if (!DisplayGluePosition(sResult.sGlueEdge, m_scContours_Glue, matResult, nContourWidth)) {
				return false;
			}

			if (sResult.sGlueEdge2.nContoursCount > 0) {
				// 畫膠輪廓
				if (!DisplayGluePosition(sResult.sGlueEdge2, m_scContours_Glue, matResult, nContourWidth)) {
					return false;
				}
			}

			// 畫 Coating 輪廓
			if (!DisplayCoatingPosition(sResult.sCoatingEdge, m_scContours_Coating, matResult, nContourWidth)) {
				return false;
			}

			// 畫ThermalGlue 輪廓
			if (!DisplayThermalGluePosition(m_sResult.sThermalGlue, m_scContours_Glue, matResult, nContourWidth)) {
				return false;
			}

			bool bROI = false;
			bool bLine = false;
			bool bMax = true;
			bool bMin = true;
			string strInfo;

			// 畫膠寬量測結果---水平框
			strInfo = "GWidth H";
			if (!DisplayRoiResult(sResult.sWidth_Horizontal, m_scROI_GlueWidth, m_scDistance_GlueWidth, m_scMaxDist_GlueWidth, m_scMinDist_GlueWidth, strInfo, matResult, bROI, bLine, bMax, bMin, nContourWidth, nLineWidth)) {
				return false;
			}

			// 畫膠寬量測結果---垂直框
			strInfo = "GWidth V";
			if (!DisplayRoiResult(sResult.sWidth_Vertical, m_scROI_GlueWidth, m_scDistance_GlueWidth, m_scMaxDist_GlueWidth, m_scMinDist_GlueWidth, strInfo, matResult, bROI, bLine, bMax, bMin, nContourWidth, nLineWidth)) {
				return false;
			}

			// 畫膠寬量測結果---弧邊
			strInfo = "GWidth Arc";
			if (!DisplayRoiResult(sResult.sWidth_Arc, m_scROI_GlueWidth, m_scDistance_GlueArc, m_scMaxDist_GlueArc, m_scMinDist_GlueArc, strInfo, matResult, bROI, bLine, bMax, bMin, nContourWidth, nLineWidth)) {
				return false;
			}

			// 畫膠首尾距離量測結果
			strInfo = "StartEnd";
			if (!DisplayStartEndResult(sResult.sStartEnd, m_scROI_GlueArc, m_scDistance_GlueArc, m_scMaxDist_GlueArc, strInfo, matResult, bROI, nContourWidth, nLineWidth)) {
				return false;
			}

			// 畫板邊到膠邊的量測結果---水平方向
			strInfo = "BToG H";
			if (!DisplayRoiResult(sResult.sBoardToGlue_Horizontal, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, strInfo, matResult, bROI, bLine, bMax, bMin, nContourWidth, nLineWidth)) {
				return false;
			}

			// 畫板邊到膠邊的量測結果---垂直方向
			strInfo = "BToG V";
			if (!DisplayRoiResult(sResult.sBoardToGlue_Vertical, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, strInfo, matResult, bROI, bLine, bMax, bMin, nContourWidth, nLineWidth)) {
				return false;
			}

			// 畫Coating到膠邊的量測結果---水平方向
			strInfo = "CoToG H";
			if (!DisplayRoiResult(sResult.sCoatingToGlue_Horizontal, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, strInfo, matResult, bROI, bLine, false, true, nContourWidth, nLineWidth)) {
				return false;
			}

			// 畫Coating到膠邊的量測結果---垂直方向
			strInfo = "CoToG V";
			if (!DisplayRoiResult(sResult.sCoatingToGlue_Vertical, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, strInfo, matResult, bROI, bLine, false, true, nContourWidth, nLineWidth)) {
				return false;
			}

			// 畫Coating到板邊的量測結果---水平方向
			strInfo = "CoToB H"; 
			if (!DisplayRoiResult(sResult.sCoatingToBoard_Horizontal, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, strInfo, matResult, bROI, bLine, false, true, nContourWidth, nLineWidth)) {
				return false;
			}

			// 畫Coating到板邊的量測結果---垂直方向
			strInfo = "CoToB V";
			if (!DisplayRoiResult(sResult.sCoatingToBoard_Vertical, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, strInfo, matResult, bROI, bLine, false, true, nContourWidth, nLineWidth)) {
				return false;
			}

			// ThermalGlue---GlueWidth
			strInfo = "Thermal Width";
			if (!DisplayRoiResult(m_sResult.sThermalGlue.sGlueWidth, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, strInfo, matResult, bROI, bLine, bMax, bMin, nContourWidth, nLineWidth)) {
				return false;
			}
			 
			// ThermalGlue---GlueToGlue 
			strInfo = "Thermal GToG";
			if (!DisplayRoiResult(m_sResult.sThermalGlue.sGlueToGlue, m_scROI_GlueWidth, m_scDistance_GlueWidth, m_scMaxDist_GlueWidth, m_scMinDist_GlueWidth, strInfo, matResult, bROI, bLine, bMax, bMin, nContourWidth, nLineWidth)) {
				return false;
			}

			// ThermalGlue---GlueToDie
			strInfo = "Thermal GToD";
			if (!DisplayRoiResult(m_sResult.sThermalGlue.sGlueToDie, m_scROI_GlueArc, m_scDistance_GlueArc, m_scMaxDist_GlueArc, m_scMinDist_GlueArc, strInfo, matResult, bROI, bLine, bMax, bMin, nContourWidth, nLineWidth)) {
				return false;
			}

			return true;
		}

		// AMD 黑膠---將量測結果畫在 matResult-顯示量測數據
		// [In]      SMeasurementBlackGlue_Result : 黑膠量測的結果
		// [In][Out] matResult : 顯示最終量測結果的影像, 輸入時需為彩色影像
		// [In]      nContourWidth = 輪廓或ROI線的寬, 0=>1 pixels; 1=>3 pixels ...
		// [In]      nLineWidth = 量測線的寬, 1=>1 pixels; 2=>2 pixels ...
		bool MeasurementBlackGlue::ShowResult_Txt(const SMeasurementBlackGlue_Result& sResult, Mat& matResult, const int& nContourWidth, const int& nLineWidth)
		{
			if (matResult.empty() || matResult.channels() != 3) {
				return false;
			}

			string strData, strDist;
			Point ptBase;
			ptBase.x = 30;
			ptBase.y = 30;
			int nAdd = 30;
			float fSize = 0.6;

			// 顯示板邊資訊
			if (!DisplayBoardPosition(sResult.sBoardEdge, m_scContours_Board, m_scLine_Board, matResult, nContourWidth, nLineWidth)) {
				return false;
			}

			// 畫膠輪廓
			if (!DisplayGluePosition(sResult.sGlueEdge, m_scContours_Glue, matResult, nContourWidth)) {
				return false;
			}

			// 畫膠輪廓2
			if (!DisplayGluePosition(sResult.sGlueEdge2, m_scContours_Glue, matResult, nContourWidth)) {
				return false;
			}

			// 畫 Coating 輪廓
			if (!DisplayCoatingPosition(sResult.sCoatingEdge, m_scContours_Coating, matResult, nContourWidth)) {
				return false;
			}


			bool bROI = false;
			bool bLine = false;
			bool bMax = true;
			bool bMin = true;
			string strInfo;

			// 畫膠寬量測結果---水平框
			strInfo = "GlueWidth_H";
			if (!DisplayRoiResult(sResult.sWidth_Horizontal, m_scROI_GlueWidth, m_scDistance_GlueWidth, m_scMaxDist_GlueWidth, m_scMinDist_GlueWidth, strInfo, matResult, bROI, bLine, bMax, bMin, nContourWidth, nLineWidth)) {
				return false;
			}

			if (sResult.sWidth_Horizontal.bEnable) {
				for (int k = 0; k < sResult.sWidth_Horizontal.nRoiCount; ++k)
				{
					// Max Dist 
					if (sResult.sWidth_Horizontal.vtsRoiInfo[k].bResult_MaxDist) {
						strData = "GlueROI_Horizontal[" + to_string(k + 1) + "]-MaxDist-Result=OK";
					}
					else {
						strData = "GlueROI_Horizontal[" + to_string(k + 1) + "]-MaxDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueWidth, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sWidth_Horizontal.vtsRoiInfo[k].sMaxDist.fDistance);
					strData = "GlueROI_Horizontal[" + to_string(k + 1) + "]-MaxDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueWidth, 1);
					ptBase.y += nAdd;

					// Min Dist
					if (sResult.sWidth_Horizontal.vtsRoiInfo[k].bResult_MinDist) {
						strData = "GlueROI_Horizontal[" + to_string(k + 1) + "]-MinDist-Result=OK";
					}
					else {
						strData = "GlueROI_Horizontal[" + to_string(k + 1) + "]-MinDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_GlueWidth, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sWidth_Horizontal.vtsRoiInfo[k].sMinDist.fDistance);
					strData = "GlueROI_Horizontal[" + to_string(k + 1) + "]-MinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_GlueWidth, 1);
					ptBase.y += nAdd;

					// Diff_MaxMin
					if (sResult.sWidth_Horizontal.vtsRoiInfo[k].bResult_Diff_MaxMin) {
						strData = "GlueROI_Horizontal[" + to_string(k + 1) + "]-Diff_MaxMin-Result=OK";
					}
					else {
						strData = "GlueROI_Horizontal[" + to_string(k + 1) + "]-Diff_MaxMin-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_GlueWidth, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sWidth_Horizontal.vtsRoiInfo[k].fDiff_Dist_MaxMin);
					strData = "GlueROI_Horizontal[" + to_string(k + 1) + "]-Diff_MaxMinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_GlueWidth, 1);
					ptBase.y += nAdd;
				}
			}


			// 畫膠寬量測結果---垂直框
			strInfo = "GlueWidth_V";
			if (!DisplayRoiResult(sResult.sWidth_Vertical, m_scROI_GlueWidth, m_scDistance_GlueWidth, m_scMaxDist_GlueWidth, m_scMinDist_GlueWidth, strInfo, matResult, bROI, bLine, bMax, bMin, nContourWidth, nLineWidth)) {
				return false;
			}

			if (sResult.sWidth_Vertical.bEnable) {
				for (int k = 0; k < sResult.sWidth_Vertical.nRoiCount; ++k) {
					// Max Dist
					if (sResult.sWidth_Vertical.vtsRoiInfo[k].bResult_MaxDist) {
						strData = "GlueROI_Vertical[" + to_string(k + 1) + "]-MaxDist-Result=OK";
					}
					else {
						strData = "GlueROI_Vertical[" + to_string(k + 1) + "]-MaxDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueWidth, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sWidth_Vertical.vtsRoiInfo[k].sMaxDist.fDistance);
					strData = "GlueROI_Vertical[" + to_string(k + 1) + "]-MaxDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueWidth, 1);
					ptBase.y += nAdd;

					// Min Dist	
					if (sResult.sWidth_Vertical.vtsRoiInfo[k].bResult_MinDist) {
						strData = "GlueROI_Vertical[" + to_string(k + 1) + "]-MinDist-Result=OK";
					}
					else {
						strData = "GlueROI_Vertical[" + to_string(k + 1) + "]-MinDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_GlueWidth, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sWidth_Vertical.vtsRoiInfo[k].sMinDist.fDistance);
					strData = "GlueROI_Vertical[" + to_string(k + 1) + "]-MinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_GlueWidth, 1);
					ptBase.y += nAdd;

					// Diff_MaxMin
					if (sResult.sWidth_Vertical.vtsRoiInfo[k].bResult_Diff_MaxMin) {
						strData = "GlueROI_Vertical[" + to_string(k + 1) + "]-Diff_MaxMin-Result=OK";
					}
					else {
						strData = "GlueROI_Vertical[" + to_string(k + 1) + "]-Diff_MaxMin-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_GlueWidth, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sWidth_Vertical.vtsRoiInfo[k].fDiff_Dist_MaxMin);
					strData = "GlueROI_Vertical[" + to_string(k + 1) + "]-Diff_MaxMinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_GlueWidth, 1);
					ptBase.y += nAdd;
				}
			}


			// 畫膠寬量測結果---弧邊
			strInfo = "GlueWidth_Arc";
			if (!DisplayRoiResult(sResult.sWidth_Arc, m_scROI_GlueWidth, m_scDistance_GlueArc, m_scMaxDist_GlueArc, m_scMinDist_GlueArc, strInfo, matResult, bROI, bLine, bMax, bMin, nContourWidth, nLineWidth)) {
				return false;
			}

			if (sResult.sWidth_Arc.bEnable) {
				for (int k = 0; k < sResult.sWidth_Arc.nRoiCount; ++k) {
					// Max Dist
					if (sResult.sWidth_Arc.vtsRoiInfo[k].bResult_MaxDist) {
						strData = "GlueROI_Arc[" + to_string(k + 1) + "]-MaxDist-Result=OK";
					}
					else {
						strData = "GlueROI_Arc[" + to_string(k + 1) + "]-MaxDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueArc, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sWidth_Arc.vtsRoiInfo[k].sMaxDist.fDistance);
					strData = "GlueROI_Arc[" + to_string(k + 1) + "]-MaxDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueArc, 1);
					ptBase.y += nAdd;

					// Min Dist	
					if (sResult.sWidth_Arc.vtsRoiInfo[k].bResult_MinDist) {
						strData = "GlueROI_Arc[" + to_string(k + 1) + "]-MinDist-Result=OK";
					}
					else {
						strData = "GlueROI_Arc[" + to_string(k + 1) + "]-MinDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_GlueArc, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sWidth_Arc.vtsRoiInfo[k].sMinDist.fDistance);
					strData = "GlueROI_Arc[" + to_string(k + 1) + "]-MinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_GlueArc, 1);
					ptBase.y += nAdd;

					// Diff_MaxMin
					if (sResult.sWidth_Arc.vtsRoiInfo[k].bResult_Diff_MaxMin) {
						strData = "GlueROI_Arc[" + to_string(k + 1) + "]-Diff_MaxMin-Result=OK";
					}
					else {
						strData = "GlueROI_Arc[" + to_string(k + 1) + "]-Diff_MaxMin-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueArc, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sWidth_Arc.vtsRoiInfo[k].fDiff_Dist_MaxMin);
					strData = "GlueROI_Arc[" + to_string(k + 1) + "]-Diff_MaxMinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueArc, 1);
					ptBase.y += nAdd;
				}
			}


			// 畫膠首尾距離量測結果
			strInfo = "GlueWidth_StartEnd";
			if (!DisplayStartEndResult(sResult.sStartEnd, m_scROI_GlueArc, m_scDistance_GlueArc, m_scMaxDist_GlueArc, strInfo, matResult, bROI, nContourWidth, nLineWidth)) {
				return false;
			}

			if (sResult.sStartEnd.bEnable) {
				for (int k = 0; k < sResult.sStartEnd.nCount; ++k) {
					// Max Dist
					if (sResult.sStartEnd.vtsStartEnd[k].bResult_MaxDist) {
						strData = "GlueROI_StartEnd[" + to_string(k + 1) + "]-MaxDist-Result=OK";
					}
					else {
						strData = "GlueROI_StartEnd[" + to_string(k + 1) + "]-MaxDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueArc, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sStartEnd.vtsStartEnd[k].sDist.fDistance);
					strData = "BoardToGlue_Horizontal[" + to_string(k + 1) + "]-MaxDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueArc, 1);
					ptBase.y += nAdd;

				}
			}


			// 畫板邊到膠邊的量測結果---水平方向
			strInfo = "BoardToGlue_H";
			if (!DisplayRoiResult(sResult.sBoardToGlue_Horizontal, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, strInfo, matResult, bROI, bLine, bMax, bMin, nContourWidth, nLineWidth)) {
				return false;
			}

			if (sResult.sBoardToGlue_Horizontal.bEnable) {
				for (int k = 0; k < sResult.sBoardToGlue_Horizontal.nRoiCount; ++k) {
					// Max Dist
					if (sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].bResult_MaxDist) {
						strData = "BoardToGlue_Horizontal[" + to_string(k + 1) + "]-MaxDist-Result=OK";
					}
					else {
						strData = "BoardToGlue_Horizontal[" + to_string(k + 1) + "]-MaxDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].sMaxDist.fDistance);
					strData = "BoardToGlue_Horizontal[" + to_string(k + 1) + "]-MaxDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					// Min Dist	
					if (sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].bResult_MinDist) {
						strData = "BoardToGlue_Horizontal[" + to_string(k + 1) + "]-MinDist-Result=OK";
					}
					else {
						strData = "BoardToGlue_Horizontal[" + to_string(k + 1) + "]-MinDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].sMinDist.fDistance);
					strData = "BoardToGlue_Horizontal[" + to_string(k + 1) + "]-MinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					// Diff_MaxMin
					if (sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].bResult_Diff_MaxMin) {
						strData = "BoardToGlue_Horizontal[" + to_string(k + 1) + "]-Diff_MaxMin-Result=OK";
					}
					else {
						strData = "BoardToGlue_Horizontal[" + to_string(k + 1) + "]-Diff_MaxMin-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].fDiff_Dist_MaxMin);
					strData = "BoardToGlue_Horizontal[" + to_string(k + 1) + "]-Diff_MaxMinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_BoardToGlue, 1);
					ptBase.y += nAdd;
				}
			}


			// 畫板邊到膠邊的量測結果---垂直方向
			strInfo = "BoardToGlue_V";
			if (!DisplayRoiResult(sResult.sBoardToGlue_Vertical, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, strInfo, matResult, bROI, bLine, bMax, bMin, nContourWidth, nLineWidth)) {
				return false;
			}

			if (sResult.sBoardToGlue_Vertical.bEnable) {
				for (int k = 0; k < sResult.sBoardToGlue_Vertical.nRoiCount; ++k) {
					// Max Dist
					if (sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].bResult_MaxDist) {
						strData = "BoardToGlue_Vertical[" + to_string(k + 1) + "]-MaxDist-Result=OK";
					}
					else {
						strData = "BoardToGlue_Vertical[" + to_string(k + 1) + "]-MaxDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].sMaxDist.fDistance);
					strData = "BoardToGlue_Vertical[" + to_string(k + 1) + "]-MaxDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					// Min Dist	
					if (sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].bResult_MinDist) {
						strData = "BoardToGlue_Vertical[" + to_string(k + 1) + "]-MinDist-Result=OK";
					}
					else {
						strData = "BoardToGlue_Vertical[" + to_string(k + 1) + "]-MinDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].sMinDist.fDistance);
					strData = "BoardToGlue_Vertical[" + to_string(k + 1) + "]-MinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					// Diff_MaxMin
					if (sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].bResult_Diff_MaxMin) {
						strData = "BoardToGlue_Vertical[" + to_string(k + 1) + "]-Diff_MaxMin-Result=OK";
					}
					else {
						strData = "BoardToGlue_Vertical[" + to_string(k + 1) + "]-Diff_MaxMin-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].fDiff_Dist_MaxMin);
					strData = "BoardToGlue_Vertical[" + to_string(k + 1) + "]-Diff_MaxMinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_BoardToGlue, 1);
					ptBase.y += nAdd;
				}
			}


			// 畫Coating到膠邊的量測結果---水平方向
			strInfo = "CoatingToGlue_H";
			if (!DisplayRoiResult(sResult.sCoatingToGlue_Horizontal, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, strInfo, matResult, bROI, bLine, false, true, nContourWidth, nLineWidth)) {
				return false;
			}

			if (sResult.sCoatingToGlue_Horizontal.bEnable) {
				for (int k = 0; k < sResult.sCoatingToGlue_Horizontal.nRoiCount; ++k) {
					// Min Dist	
					if (sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].bResult_MinDist) {
						strData = "CoatingToGlue_Horizontal[" + to_string(k + 1) + "]-MinDist-Result=OK";
					}
					else {
						strData = "CoatingToGlue_Horizontal[" + to_string(k + 1) + "]-MinDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].sMinDist.fDistance);
					strData = "CoatingToGlue_Horizontal[" + to_string(k + 1) + "]-MinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_BoardToGlue, 1);
					ptBase.y += nAdd;
				}
			}


			// 畫Coating到膠邊的量測結果---垂直方向
			strInfo = "CoatingToGlue_V";
			if (!DisplayRoiResult(sResult.sCoatingToGlue_Vertical, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, strInfo,  matResult, bROI, bLine, false, true, nContourWidth, nLineWidth)) {
				return false;
			}

			if (sResult.sCoatingToGlue_Vertical.bEnable) {
				for (int k = 0; k < sResult.sCoatingToGlue_Vertical.nRoiCount; ++k) {
					// Min Dist	
					if (sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].bResult_MinDist) {
						strData = "CoatingToGlue_Vertical[" + to_string(k + 1) + "]-MinDist-Result=OK";
					}
					else {
						strData = "CoatingToGlue_Vertical[" + to_string(k + 1) + "]-MinDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].sMinDist.fDistance);
					strData = "CoatingToGlue_Vertical[" + to_string(k + 1) + "]-MinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_BoardToGlue, 1);
					ptBase.y += nAdd;
				}
			}


			// 畫Coating到板邊的量測結果---水平方向
			strInfo = "CoatingToBoard_H";
			if (!DisplayRoiResult(sResult.sCoatingToBoard_Horizontal, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, strInfo, matResult, bROI, bLine, false, true, nContourWidth, nLineWidth)) {
				return false;
			}

			if (sResult.sCoatingToBoard_Horizontal.bEnable) {
				for (int k = 0; k < sResult.sCoatingToBoard_Horizontal.nRoiCount; ++k) {
					// Min Dist	
					if (sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].bResult_MinDist) {
						strData = "CoatingToBoard_Horizontal[" + to_string(k + 1) + "]-MinDist-Result=OK";
					}
					else {
						strData = "CoatingToBoard_Horizontal[" + to_string(k + 1) + "]-MinDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].sMinDist.fDistance);
					strData = "CoatingToBoard_Horizontal[" + to_string(k + 1) + "]-MinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_BoardToGlue, 1);
					ptBase.y += nAdd;
				}
			}


			// 畫Coating到板邊的量測結果---垂直方向
			strInfo = "CoatingToBoard_V";
			if (!DisplayRoiResult(sResult.sCoatingToBoard_Vertical, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, strInfo, matResult, bROI, bLine, false, true, nContourWidth, nLineWidth)) {
				return false;
			}

			if (sResult.sCoatingToBoard_Vertical.bEnable) {
				for (int k = 0; k < sResult.sCoatingToBoard_Vertical.nRoiCount; ++k) {
					// Min Dist	
					if (sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].bResult_MinDist) {
						strData = "CoatingToBoard_Vertical[" + to_string(k + 1) + "]-MinDist-Result=OK";
					}
					else {
						strData = "CoatingToBoard_Vertical[" + to_string(k + 1) + "]-MinDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].sMinDist.fDistance);
					strData = "CoatingToBoard_Vertical[" + to_string(k + 1) + "]-MinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_BoardToGlue, 1);
					ptBase.y += nAdd;
				}
			}


			// ThermalGlue---GlueWidth
			strInfo = "Thermal_GlueWidth";
			if (!DisplayRoiResult(m_sResult.sThermalGlue.sGlueWidth, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, strInfo, matResult, bROI, bLine, bMax, bMin, nContourWidth, nLineWidth)) {
				return false;
			}

			if (sResult.sThermalGlue.sGlueWidth.bEnable) {
				for (int k = 0; k < sResult.sThermalGlue.sGlueWidth.nRoiCount; ++k) {
					// Max Dist
					if (sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MaxDist) {
						strData = "ThermalGlue_GlueWidth[" + to_string(k + 1) + "]-MaxDist-Result=OK";
					}
					else {
						strData = "ThermalGlue_GlueWidth[" + to_string(k + 1) + "]-MaxDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMaxDist.fDistance);
					strData = "ThermalGlue_GlueWidth[" + to_string(k + 1) + "]-MaxDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					// Min Dist	
					if (sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MinDist) {
						strData = "ThermalGlue_GlueWidth[" + to_string(k + 1) + "]-MinDist-Result=OK";
					}
					else {
						strData = "ThermalGlue_GlueWidth[" + to_string(k + 1) + "]-MinDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMinDist.fDistance);
					strData = "ThermalGlue_GlueWidth[" + to_string(k + 1) + "]-MinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					// Diff_MaxMin
					if (sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_Diff_MaxMin) {
						strData = "ThermalGlue_GlueWidth[" + to_string(k + 1) + "]-Diff_MaxMin-Result=OK";
					}
					else {
						strData = "ThermalGlue_GlueWidth[" + to_string(k + 1) + "]-Diff_MaxMin-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_BoardToGlue, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].fDiff_Dist_MaxMin);
					strData = "ThermalGlue_GlueWidth[" + to_string(k + 1) + "]-Diff_MaxMinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_BoardToGlue, 1);
					ptBase.y += nAdd;
				}
			}


			// ThermalGlue---GlueToGlue 
			strInfo = "Thermal_GlueToGlue";
			if (!DisplayRoiResult(m_sResult.sThermalGlue.sGlueToGlue, m_scROI_GlueWidth, m_scDistance_GlueWidth, m_scMaxDist_GlueWidth, m_scMinDist_GlueWidth, strInfo, matResult, bROI, bLine, bMax, bMin, nContourWidth, nLineWidth)) {
				return false;
			}

			if (sResult.sThermalGlue.sGlueToGlue.bEnable) {
				for (int k = 0; k < sResult.sThermalGlue.sGlueToGlue.nRoiCount; ++k) {
					// Max Dist
					if (sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].bResult_MaxDist) {
						strData = "ThermalGlue_GlueToGlue[" + to_string(k + 1) + "]-MaxDist-Result=OK";
					}
					else {
						strData = "ThermalGlue_GlueToGlue[" + to_string(k + 1) + "]-MaxDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueWidth, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].sMaxDist.fDistance);
					strData = "ThermalGlue_GlueToGlue[" + to_string(k + 1) + "]-MaxDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueWidth, 1);
					ptBase.y += nAdd;

					// Min Dist	
					if (sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].bResult_MinDist) {
						strData = "ThermalGlue_GlueToGlue[" + to_string(k + 1) + "]-MinDist-Result=OK";
					}
					else {
						strData = "ThermalGlue_GlueToGlue[" + to_string(k + 1) + "]-MinDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_GlueWidth, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].sMinDist.fDistance);
					strData = "ThermalGlue_GlueToGlue[" + to_string(k + 1) + "]-MinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_GlueWidth, 1);
					ptBase.y += nAdd;

					// Diff_MaxMin
					if (sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].bResult_Diff_MaxMin) {
						strData = "ThermalGlue_GlueToGlue[" + to_string(k + 1) + "]-Diff_MaxMin-Result=OK";
					}
					else {
						strData = "ThermalGlue_GlueToGlue[" + to_string(k + 1) + "]-Diff_MaxMin-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueWidth, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].fDiff_Dist_MaxMin);
					strData = "ThermalGlue_GlueToGlue[" + to_string(k + 1) + "]-Diff_MaxMinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueWidth, 1);
					ptBase.y += nAdd;
				}
			}


			// ThermalGlue---GlueToDie
			strInfo = "Thermal_GlueToDie";
			if (!DisplayRoiResult(m_sResult.sThermalGlue.sGlueToDie, m_scROI_GlueArc, m_scDistance_GlueArc, m_scMaxDist_GlueArc, m_scMinDist_GlueArc, strInfo, matResult, bROI, bLine, bMax, bMin, nContourWidth, nLineWidth)) {
				return false;
			}

			if (sResult.sThermalGlue.sGlueToDie.bEnable) {
				for (int k = 0; k < sResult.sThermalGlue.sGlueToDie.nRoiCount; ++k) {
					// Max Dist
					if (sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].bResult_MaxDist) {
						strData = "ThermalGlue_GlueToDie[" + to_string(k + 1) + "]-MaxDist-Result=OK";
					}
					else {
						strData = "ThermalGlue_GlueToDie[" + to_string(k + 1) + "]-MaxDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueWidth, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].sMaxDist.fDistance);
					strData = "ThermalGlue_GlueToDie[" + to_string(k + 1) + "]-MaxDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueWidth, 1);
					ptBase.y += nAdd;

					// Min Dist	
					if (sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].bResult_MinDist) {
						strData = "ThermalGlue_GlueToDie[" + to_string(k + 1) + "]-MinDist-Result=OK";
					}
					else {
						strData = "ThermalGlue_GlueToDie[" + to_string(k + 1) + "]-MinDist-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_GlueWidth, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].sMinDist.fDistance);
					strData = "ThermalGlue_GlueToDie[" + to_string(k + 1) + "]-MinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMinDist_GlueWidth, 1);
					ptBase.y += nAdd;

					// Diff_MaxMin
					if (sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].bResult_Diff_MaxMin) {
						strData = "ThermalGlue_GlueToDie[" + to_string(k + 1) + "]-Diff_MaxMin-Result=OK";
					}
					else {
						strData = "ThermalGlue_GlueToDie[" + to_string(k + 1) + "]-Diff_MaxMin-Result=NG";
					}
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueWidth, 1);
					ptBase.y += nAdd;

					strDist = jet_imagefunction::FloatingToString("%.1f", sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].fDiff_Dist_MaxMin);
					strData = "ThermalGlue_GlueToDie[" + to_string(k + 1) + "]-Diff_MaxMinDistance=" + strDist;
					cv::putText(matResult, strData, ptBase, FONT_HERSHEY_TRIPLEX, fSize, m_scMaxDist_GlueWidth, 1);
					ptBase.y += nAdd;
				}
			}

			return true;
		}

		// AMD---Flux面積量測結果輸出
		// [In]      SMeasurementFluxArea_Result : Flux量測的結果
		// [In][Out] matResult : 顯示最終量測結果的影像, 輸入時需為彩色影像
		// [Out]     vtstrResult: 量測數據
		// [In]      nContourWidth = 輪廓寬, 0=>1 pixels; 1=>3 pixels ...
		bool MeasurementBlackGlue::ShowResult_FluxArea(const SMeasurementFluxArea_Result& sResult, Mat& matResult, vector<string>& vtstrResult, bool& bResult, const int& nContourWidth)
		{
			if (matResult.empty() || matResult.channels() != 3) {
				return false;
			}

			// 畫Flux輪廓
			if (!DisplayGluePosition(sResult.sFluxEdge, m_scContours_Glue, matResult, nContourWidth)) {
				return false;
			}

			bResult = sResult.bResult;

			vtstrResult.resize(5);
			string strData = jet_imagefunction::FloatingToString("Total Area = %.1f ( μm2 )", m_sParam_FluxArea.fTotalArea);
			vtstrResult[0] = strData;

			strData = "Flux Area = " + to_string(sResult.sFluxEdge.nPixelArea) + "( Pixels )";
			vtstrResult[1] = strData;

			strData = jet_imagefunction::FloatingToString("Flux Area = %.1f ( μm2 )", sResult.fFluxArea);
			vtstrResult[2] = strData;

			if (!m_matBoard.empty()) {
				Scalar scMean, scStd;
				cv::meanStdDev(m_matBoard, scMean, scStd);
				vtstrResult[3] = jet_imagefunction::FloatingToString("Mean = %.1f", scMean.val[0]) + jet_imagefunction::FloatingToString(" ; Std = %.1f", scStd.val[0]);
			}

			strData = jet_imagefunction::FloatingToString("Area Ratio = %.1f%%", sResult.fAreaRatio);
			strData += jet_imagefunction::FloatingToString(", Upper = %.1f%%", m_sParam_FluxArea.sLimit.fUpper);
			strData += jet_imagefunction::FloatingToString(", Lower = %.1f%%", m_sParam_FluxArea.sLimit.fLower);
			if (bResult) {
				vtstrResult[4] = "OK , " + strData;
			}
			else {
				vtstrResult[4] = "NG , " + strData;
			}

			return true;
		}

		// AMD---Flux面積量測結果輸出
		// [In]      SMeasurementFluxArea_Result : Flux量測的結果
		// [In][Out] matResult : 顯示最終量測結果的影像, 輸入時需為彩色影像
		// [In]      nContourWidth = 輪廓寬, 0=>1 pixels; 1=>3 pixels ...
		bool MeasurementBlackGlue::ShowResult_FluxArea_Txt(const SMeasurementFluxArea_Result& sResult, Mat& matResult, const int& nContourWidth)
		{
			if (matResult.empty() || matResult.channels() != 3) {
				return false;
			}

			// 畫Flux輪廓
			if (!DisplayGluePosition(sResult.sFluxEdge, m_scContours_Glue, matResult, nContourWidth)) {
				return false;
			}

			vector<string> vtstrResult(5);
			string strData = jet_imagefunction::FloatingToString("Total Area = %.1f ( um2 )", m_sParam_FluxArea.fTotalArea);
			vtstrResult[0] = strData;

			strData = "Flux Area = " + to_string(sResult.sFluxEdge.nPixelArea) + "( Pixels )";
			vtstrResult[1] = strData;

			strData = jet_imagefunction::FloatingToString("Flux Area = %.1f ( um2 )", sResult.fFluxArea);
			vtstrResult[2] = strData;

			if (!m_matBoard.empty()) {
				Scalar scMean, scStd;
				cv::meanStdDev(m_matBoard, scMean, scStd);
				vtstrResult[3] = jet_imagefunction::FloatingToString("Mean = %.1f", scMean.val[0]) + jet_imagefunction::FloatingToString(" ; Std = %.1f", scStd.val[0]);
			}

			strData = jet_imagefunction::FloatingToString("Area Ratio = %.1f%%", sResult.fAreaRatio);
			strData += jet_imagefunction::FloatingToString(", Upper = %.1f%%", m_sParam_FluxArea.sLimit.fUpper);
			strData += jet_imagefunction::FloatingToString(", Lower = %.1f%%", m_sParam_FluxArea.sLimit.fLower);
			if (sResult.bResult) {
				vtstrResult[4] = "OK , " + strData;
			}
			else {
				vtstrResult[4] = "NG , " + strData;
			}

			Point ptBase;
			ptBase.x = 30;
			ptBase.y = 30;
			int nAdd = 40;
			float fSize = 0.8;
			for (int k = 0; k < 5; ++k) {
				cv::putText(matResult, vtstrResult[k], ptBase, FONT_HERSHEY_TRIPLEX, fSize, cv::Scalar(0, 0, 0), 3);
				if (k == 4) {
					if (sResult.bResult) {
						cv::putText(matResult, vtstrResult[k], ptBase, FONT_HERSHEY_TRIPLEX, fSize, Scalar(0, 255, 0), 1);
					}
					else {
						cv::putText(matResult, vtstrResult[k], ptBase, FONT_HERSHEY_TRIPLEX, fSize, Scalar(0, 0, 255), 1);
					}
				}
				else {
					cv::putText(matResult, vtstrResult[k], ptBase, FONT_HERSHEY_TRIPLEX, fSize, Scalar(0, 255, 255), 1);
				}
				ptBase.y += nAdd;
			}

			return true;
		}

		// 膠高量測結果輸出
		bool MeasurementBlackGlue::ShowResult_GlueHeight(const SMeasurementGlueHeightRatio_Result& sResult, Mat& matResult, bool& bResult, vector<string>& vtstrResult)
		{
			if (matResult.empty() || matResult.channels() != 3) {
				return false;
			}

			// 畫膠輪廓
			if (!DisplayGluePosition(sResult.sGlueEdge, m_scContours_Glue, matResult, 0)) {
				return false;
			}

			// 畫 IC 輪廓
			if (!DisplayGluePosition(sResult.sIcEdge, m_scContours_Board, matResult, 0)) {
				return false;
			}

			// 畫區塊範圍
			string strBlockResult = "";
			Scalar scRange(128, 255, 128);
			for (int k = 0; k < sResult.sHeight.nBlockCount; ++k) {
				const Block_MeasurementInfo& sBlockInfo = sResult.sHeight.vtsBlock[k].sInfo;
				int nCount = sBlockInfo.nPixelsCount;
				Point cvptStart, cvptEnd, cvptCenter;
				switch (m_sParam_GlueHeight.nMeasuringDirection) {
				case 1: case 3:
					cvptCenter.x = sBlockInfo.vtsPixels[0].ptStart.x + (sBlockInfo.vtsPixels[nCount - 1].ptStart.x - sBlockInfo.vtsPixels[0].ptStart.x)*0.3;
					cvptCenter.y = matResult.rows / 2;
					cv::putText(matResult, "[" + to_string(k + 1) + "]", cvptCenter, FONT_HERSHEY_TRIPLEX, 0.7, m_scMaxDist_GlueWidth, 1);

					cvptCenter.y += 30;
					strBlockResult = jet_imagefunction::FloatingToString("%.1f%%", sResult.sHeight.vtsBlock[k].fHeightRatio);
					cv::putText(matResult, strBlockResult, cvptCenter, FONT_HERSHEY_TRIPLEX, 0.7, m_scMaxDist_GlueWidth, 1);

					cvptStart.x = sBlockInfo.vtsPixels[0].ptStart.x;
					cvptStart.y = 0;
					cvptEnd.x = sBlockInfo.vtsPixels[0].ptStart.x;
					cvptEnd.y = matResult.rows - 1;
					cv::line(matResult, cvptStart, cvptEnd, scRange, 1);

					cvptStart.x = sBlockInfo.vtsPixels[nCount - 1].ptStart.x;
					cvptStart.y = 0;
					cvptEnd.x = sBlockInfo.vtsPixels[nCount - 1].ptStart.x;
					cvptEnd.y = matResult.rows - 1;
					cv::line(matResult, cvptStart, cvptEnd, scRange, 1);
					break;

				case 2: case 4:
					cvptCenter.x = matResult.cols*0.4;
					cvptCenter.y = sBlockInfo.vtsPixels[0].ptStart.y + (sBlockInfo.vtsPixels[nCount - 1].ptStart.y - sBlockInfo.vtsPixels[0].ptStart.y)*0.5;
					cv::putText(matResult, "[" + to_string(k + 1) + "]", cvptCenter, FONT_HERSHEY_TRIPLEX, 1.0, m_scMaxDist_GlueWidth, 1);

					cvptCenter.x += 80;
					strBlockResult = jet_imagefunction::FloatingToString("%.1f%%", sResult.sHeight.vtsBlock[k].fHeightRatio);
					cv::putText(matResult, strBlockResult, cvptCenter, FONT_HERSHEY_TRIPLEX, 1.0, m_scMaxDist_GlueWidth, 1);

					cvptStart.x = 0;
					cvptStart.y = sBlockInfo.vtsPixels[0].ptStart.y;
					cvptEnd.x = matResult.cols - 1;
					cvptEnd.y = sBlockInfo.vtsPixels[0].ptStart.y;
					cv::line(matResult, cvptStart, cvptEnd, scRange, 1);

					cvptStart.x = 0;
					cvptStart.y = sBlockInfo.vtsPixels[nCount - 1].ptStart.y;
					cvptEnd.x = matResult.cols - 1;
					cvptEnd.y = sBlockInfo.vtsPixels[nCount - 1].ptStart.y;
					cv::line(matResult, cvptStart, cvptEnd, scRange, 1);
					break;
				}
			}

			// 畫每個區塊的結果
			for (int k = 0; k < sResult.sHeight.nBlockCount; ++k) {
				const SingleBlockGlueHeight_Result& sBlockResult = sResult.sHeight.vtsBlock[k];
				Point cvptStart, cvptEnd;
				cvptStart.x = sBlockResult.sFirst.ptStart.x;
				cvptStart.y = sBlockResult.sFirst.ptStart.y;
				cvptEnd.x = sBlockResult.sFirst.ptEnd.x;
				cvptEnd.y = sBlockResult.sFirst.ptEnd.y;
				if ((cvptStart.x == 0 && cvptStart.y == 0) || (cvptEnd.x == 0 && cvptEnd.y == 0)) continue;

				jet_imagefunction::DrawCrossPoint(sBlockResult.sFirst.ptStart, 5, 1, m_scMaxDist_BoardToGlue, matResult);
				jet_imagefunction::DrawCrossPoint(sBlockResult.sFirst.ptEnd, 5, 1, m_scMaxDist_BoardToGlue, matResult);
				cv::line(matResult, cvptStart, cvptEnd, Scalar(0, 255, 255), 1);

				switch (m_sParam_GlueHeight.vtnHeightMode[k]) {
				case 4: case 5:
					cvptStart.x = sBlockResult.sSecond.ptStart.x;
					cvptStart.y = sBlockResult.sSecond.ptStart.y;
					cvptEnd.x = sBlockResult.sSecond.ptEnd.x;
					cvptEnd.y = sBlockResult.sSecond.ptEnd.y;
					if ((cvptStart.x == 0 && cvptStart.y == 0) || (cvptEnd.x == 0 && cvptEnd.y == 0)) continue;

					jet_imagefunction::DrawCrossPoint(sBlockResult.sSecond.ptStart, 5, 1, m_scMinDist_BoardToGlue, matResult);
					jet_imagefunction::DrawCrossPoint(sBlockResult.sSecond.ptEnd, 5, 1, m_scMinDist_BoardToGlue, matResult);
					cv::line(matResult, cvptStart, cvptEnd, Scalar(255, 0, 255), 1);
					break;
				}
			}

			// 輸出結果
			int nAddCount = 2;
			vtstrResult.resize(sResult.sHeight.nBlockCount + nAddCount, "");
			bResult = sResult.sHeight.bFinalResult;

			string strData;
			if (bResult) {	
				strData = "最終結果 OK, ";
			}
			else {
				strData = "最終結果 NG, ";
			}

			switch (m_sParam_GlueHeight.nFinal_HeightMode) {
			case 1:
				strData += jet_imagefunction::FloatingToString("高度算法 : 最高 , 高度比 = %.1f%%", sResult.sHeight.fFinalHeightRatio);
				break;
			case 2:
				strData += jet_imagefunction::FloatingToString("高度算法 : 平均 , 高度比 = %.1f%%", sResult.sHeight.fFinalHeightRatio);
				break;
			case 3:
				strData += jet_imagefunction::FloatingToString("高度算法 : 最低 , 高度比 = %.1f%%", sResult.sHeight.fFinalHeightRatio);
				break;
			case 4:
				strData += jet_imagefunction::FloatingToString("高度算法 : 最高-最低 , 高度差比 = %.1f%%", sResult.sHeight.fFinalHeightRatio);
				break;
			case 5:
				strData += jet_imagefunction::FloatingToString("高度算法 : 平均-最低 , 高度差比 = %.1f%%", sResult.sHeight.fFinalHeightRatio);
				break;
			}
			strData += jet_imagefunction::FloatingToString(" , IC邊到膠邊的距離 = %.1f ( μm )", sResult.sHeight.sFinalFirst.fDist_ICtoGlue);
			strData += jet_imagefunction::FloatingToString(", 上限 = %.1f%%", m_sParam_GlueHeight.sLimit.fUpper);
			strData += jet_imagefunction::FloatingToString(", 下限 = %.1f%%", m_sParam_GlueHeight.sLimit.fLower);	
			vtstrResult[0] = strData;

			// IC 高
			vtstrResult[1] = jet_imagefunction::FloatingToString("IC高度 = %.1f ( μm )", m_sParam_GlueHeight.fIC_Height);

			// 每個區塊的結果	
			for (int k = 0; k < sResult.sHeight.nBlockCount; ++k) {
				strData = "[" + to_string(k + 1) + "] :";
				const SingleBlockGlueHeight_Result& sBlockResult = sResult.sHeight.vtsBlock[k];
				switch (m_sParam_GlueHeight.vtnHeightMode[k]) {
				case 1:
					strData += jet_imagefunction::FloatingToString("高度算法 : 最高 , 高度比 = %.1f%%", sBlockResult.fHeightRatio);
					break;
				case 2:
					strData += jet_imagefunction::FloatingToString("高度算法 : 平均 , 高度比 = %.1f%%", sBlockResult.fHeightRatio);
					break;
				case 3:
					strData += jet_imagefunction::FloatingToString("高度算法 : 最低 , 高度比 = %.1f%%", sBlockResult.fHeightRatio);
					break;
				case 4:
					strData += jet_imagefunction::FloatingToString("高度算法 : 最高-最低 , 高度差比 = %.1f%%", sBlockResult.fHeightRatio);
					break;
				case 5:
					strData += jet_imagefunction::FloatingToString("高度算法 : 平均-最低 , 高度差比 = %.1f%%", sBlockResult.fHeightRatio);
					break;
				}
				strData += jet_imagefunction::FloatingToString(" , IC邊到膠邊的距離 = %.1f ( μm )", sBlockResult.sFirst.fDist_ICtoGlue);
				vtstrResult[k + 2] = strData;
			}

			return true;
		}

		bool MeasurementBlackGlue::ShowResult_GlueArea(const SMeasurementGlueArea_Result& sResult, Mat& matResult, vector<string>& vtstrResult, bool& bResult)
		{
			if (matResult.empty() || matResult.channels() != 3) {
				return false;
			}

			bResult = sResult.bResult;

			// 畫 IC 輪廓
			for (int k = 0; k < sResult.sIcRange.vt2ptContoursPos.size(); ++k) {
				jet_imagefunction::DrawEdgePoint(matResult, sResult.sIcRange.vt2ptContoursPos[k], Scalar(0, 255, 0), 1);
			}

			vtstrResult.reserve(20);
			string strData = "偵測到的膠數量 = " + to_string(sResult.nGlueCount) + " ; NG數量 = " + to_string(m_sParam_GlueArea.nGlueNG_Count);
			vtstrResult.emplace_back(strData);

			strData = "偵測到的IC寬 = " + to_string(sResult.sIcRange.nIC_Width) + " (μm) ; 目標IC寬 = " + to_string(m_sParam_GlueArea.nIC_Width) + " (μm)";
			vtstrResult.emplace_back(strData);

			strData = "偵測到的IC高 = " + to_string(sResult.sIcRange.nIC_Height) + " (μm) ; 目標IC高 = " + to_string(m_sParam_GlueArea.nIC_Height) + " (μm)";
			vtstrResult.emplace_back(strData);

			int nDiffW = abs(m_sResult_GlueArea.sIcRange.nIC_Width - m_sParam_GlueArea.nIC_Width);
			int nDiffH = abs(m_sResult_GlueArea.sIcRange.nIC_Height - m_sParam_GlueArea.nIC_Height);

			string strResult = "";
			if (m_sResult_GlueArea.nGlueCount >= m_sParam_GlueArea.nGlueNG_Count) {
				strResult = " 偵測到膠的數量 >= 設定值 ; ";
				// 畫膠輪廓
				if (sResult.nGlueCount > 0) {
					if (sResult.nGlueCount != sResult.vtsGlueArea.size()) return false;
					for (int k = 0; k < sResult.nGlueCount; ++k) {
						if (!DisplayGluePosition(sResult.vtsGlueArea[k], m_scContours_Glue, matResult, 1)) {
							return false;
						}
					}
				}
			}

			if (nDiffW > m_sParam_GlueArea.nIC_NG_DiffWidth) {
				strResult += "IC寬度誤差太大 ; ";
			}

			if (nDiffH > m_sParam_GlueArea.nIC_NG_DiffHeight) {
				strResult += "IC高度誤差太大 ; ";
			}

			int nMaxGlueWidth = 0;
			int nMaxGlueHeight = 0;
			for (int k = 0; k < sResult.nGlueCount; ++k) {
				if (nMaxGlueWidth < sResult.vtsGlueArea[k].nWidth) {
					nMaxGlueWidth = sResult.vtsGlueArea[k].nWidth;
				}

				if (nMaxGlueHeight < sResult.vtsGlueArea[k].nHeight) {
					nMaxGlueHeight = sResult.vtsGlueArea[k].nHeight;
				}
			}

			if (nMaxGlueWidth >= m_sParam_GlueArea.nGlueNG_Width) {
				strResult += "膠的寬度 >= 設定值 ; ";
				for (int k = 0; k < sResult.nGlueCount; ++k) {
					if (sResult.vtsGlueArea[k].nWidth >= m_sParam_GlueArea.nGlueNG_Width) {
						if (!DisplayGluePosition(sResult.vtsGlueArea[k], m_scContours_Glue, matResult, 1)) {
							return false;
						}
					}
				}
			}

			if (nMaxGlueHeight >= m_sParam_GlueArea.nGlueNG_Height) {
				strResult += "膠的高度 >= 設定值 ; ";
				for (int k = 0; k < sResult.nGlueCount; ++k) {
					if (sResult.vtsGlueArea[k].nHeight >= m_sParam_GlueArea.nGlueNG_Height) {
						if (!DisplayGluePosition(sResult.vtsGlueArea[k], m_scContours_Glue, matResult, 1)) {
							return false;
						}
					}
				}
			}

			if (bResult) {
				vtstrResult.emplace_back("檢測結果 : OK");
			}
			else {
				vtstrResult.emplace_back("檢測結果 : NG ;" + strResult);
			}	

			return true;
		}

		// 膠面積量測結果輸出(結果會顯示在畫面上)
		bool MeasurementBlackGlue::ShowResult_GlueArea_Txt(const SMeasurementGlueArea_Result& sResult, Mat& matResult)
		{
			if (matResult.empty() || matResult.channels() != 3) {
				return false;
			}

			// 畫 IC 輪廓
			for (int k = 0; k < sResult.sIcRange.vt2ptContoursPos.size(); ++k) {
				jet_imagefunction::DrawEdgePoint(matResult, sResult.sIcRange.vt2ptContoursPos[k], Scalar(0, 255, 0), 1);
			}

			vector<string> vtstrResult;
			vtstrResult.reserve(20);
			string strData = "Glue Count = " + to_string(sResult.nGlueCount) + " ; Minimum Count = " + to_string(m_sParam_GlueArea.nGlueNG_Count);
			vtstrResult.emplace_back(strData);

			strData = "IC Width = " + to_string(sResult.sIcRange.nIC_Width) + " (um) ; Target IC Width = " + to_string(m_sParam_GlueArea.nIC_Width) + " (um)";
			vtstrResult.emplace_back(strData);

			strData = "IC Height = " + to_string(sResult.sIcRange.nIC_Height) + " (um) ; Target IC Height = " + to_string(m_sParam_GlueArea.nIC_Height) + " (um)";
			vtstrResult.emplace_back(strData);

			int nDiffW = abs(m_sResult_GlueArea.sIcRange.nIC_Width - m_sParam_GlueArea.nIC_Width);
			int nDiffH = abs(m_sResult_GlueArea.sIcRange.nIC_Height - m_sParam_GlueArea.nIC_Height);

			if (sResult.bResult) {
				vtstrResult.emplace_back("Result : OK");
			}
			else {
				vtstrResult.emplace_back("Result : NG");
			}

			string strResult = "";
			if (m_sResult_GlueArea.nGlueCount >= m_sParam_GlueArea.nGlueNG_Count) {
				vtstrResult.emplace_back("Glue Count >=  Minimum Glue Count");
				// 畫膠輪廓
				if (sResult.nGlueCount > 0) {
					if (sResult.nGlueCount != sResult.vtsGlueArea.size()) return false;
					for (int k = 0; k < sResult.nGlueCount; ++k) {
						if (!DisplayGluePosition(sResult.vtsGlueArea[k], m_scContours_Glue, matResult, 2)) {
							return false;
						}
					}
				}
			}

			int nMaxGlueWidth = 0;
			int nMaxGlueHeight = 0;
			for (int k = 0; k < sResult.nGlueCount; ++k) {
				if (nMaxGlueWidth < sResult.vtsGlueArea[k].nWidth) {
					nMaxGlueWidth = sResult.vtsGlueArea[k].nWidth;
				}

				if (nMaxGlueHeight < sResult.vtsGlueArea[k].nHeight) {
					nMaxGlueHeight = sResult.vtsGlueArea[k].nHeight;
				}
			}

			if (nMaxGlueWidth >= m_sParam_GlueArea.nGlueNG_Width) {
				vtstrResult.emplace_back("Glue Width >=  Min Glue Width");
				for (int k = 0; k < sResult.nGlueCount; ++k) {
					if (sResult.vtsGlueArea[k].nWidth >= m_sParam_GlueArea.nGlueNG_Width) {
						if (!DisplayGluePosition(sResult.vtsGlueArea[k], m_scContours_Glue, matResult, 2)) {
							return false;
						}
					}
				}
			}

			if (nMaxGlueHeight >= m_sParam_GlueArea.nGlueNG_Height) {
				vtstrResult.emplace_back("Glue Height >= Min Glue Height");
				for (int k = 0; k < sResult.nGlueCount; ++k) {
					if (sResult.vtsGlueArea[k].nHeight >= m_sParam_GlueArea.nGlueNG_Height) {
						if (!DisplayGluePosition(sResult.vtsGlueArea[k], m_scContours_Glue, matResult, 2)) {
							return false;
						}
					}
				}
			}		

			if (nDiffW > m_sParam_GlueArea.nIC_NG_DiffWidth) {
				vtstrResult.emplace_back("IC Width Error");
			}

			if (nDiffH > m_sParam_GlueArea.nIC_NG_DiffHeight) {
				vtstrResult.emplace_back("IC Height Error");
			}
			
			Point ptBase;
			ptBase.x = sResult.sIcRange.vtptCorners[0].x + 30;
			ptBase.y = sResult.sIcRange.vtptCorners[0].y + 50;
			int nAdd = 40;
			float fSize = 1.0;
			for (int k = 0; k < vtstrResult.size(); ++k) {
				if (k < 3) {
					cv::putText(matResult, vtstrResult[k], ptBase, FONT_HERSHEY_TRIPLEX, fSize, Scalar(0, 255, 255), 1);
				}
				else {
					if (sResult.bResult) {
						cv::putText(matResult, vtstrResult[k], ptBase, FONT_HERSHEY_TRIPLEX, fSize, Scalar(0, 255, 0), 1);
					}
					else {
						cv::putText(matResult, vtstrResult[k], ptBase, FONT_HERSHEY_TRIPLEX, fSize, Scalar(0, 0, 255), 1);
					}
				}
				ptBase.y += nAdd;
			}

			return true;
		}

		// 顯示板邊輪廓
		// [In] sResult : 板邊檢測結果
		// [In] scContours : 輪廓的顏色
		// [In] scLine : 線的顏色
		// [In] [Out] matDisplay : 輸入彩色影像, 將檢測結果畫上去
		// [In] nContourWidth = 輪廓或ROI線的寬, 0=>1 pixels; 1=>3 pixels ...
		// [In] nLineWidth = 量測線的寬, 1=>1 pixels; 2=>2 pixels ...
		bool MeasurementBlackGlue::DisplayBoardPosition(const SBoardEdge_Result& sResult, const Scalar& scContours, const Scalar& scLine, Mat& matDisplay, const int& nContourWidth, const int& nLineWidth)
		{
			if (matDisplay.empty() || matDisplay.channels() != 3) {
				return false;
			}

			// 顯示板邊資訊
			if (sResult.bEnable)
			{
				// 畫輪廓
				jet_imagefunction::DrawEdgePoint(matDisplay, sResult.vtptContoursPos, scContours, nContourWidth);

				// 畫板邊直線---水平線
				Point2f ptfStartX, ptfEndX;
				if (sResult.dSlope_Horizontal != 99999.0 || sResult.dIntercept_Horizontal != 0.0)
				{
					ptfStartX.x = sResult.sLine_Horizontal.ptStart.x;
					ptfStartX.y = sResult.sLine_Horizontal.ptStart.y;
					ptfEndX.x = sResult.sLine_Horizontal.ptEnd.x;
					ptfEndX.y = sResult.sLine_Horizontal.ptEnd.y;
					cv::line(matDisplay, ptfStartX, ptfEndX, scLine, nLineWidth);
				}

				if (sResult.dIntercept_Horizontal2 != 0.0) {
					ptfStartX.x = sResult.sLine_Horizontal2.ptStart.x;
					ptfStartX.y = sResult.sLine_Horizontal2.ptStart.y;
					ptfEndX.x = sResult.sLine_Horizontal2.ptEnd.x;
					ptfEndX.y = sResult.sLine_Horizontal2.ptEnd.y;
					cv::line(matDisplay, ptfStartX, ptfEndX, scLine, nLineWidth);
				}

				// 畫板邊直線---垂直線
				if (sResult.dSlope_Vertical != 0.0 || sResult.dIntercept_Vertical != 0.0)
				{
					ptfStartX.x = sResult.sLine_Vertical.ptStart.x;
					ptfStartX.y = sResult.sLine_Vertical.ptStart.y;
					ptfEndX.x = sResult.sLine_Vertical.ptEnd.x;
					ptfEndX.y = sResult.sLine_Vertical.ptEnd.y;
					cv::line(matDisplay, ptfStartX, ptfEndX, scLine, nLineWidth);
				}

				if (sResult.dIntercept_Vertical2 != 0.0) {
					ptfStartX.x = sResult.sLine_Vertical2.ptStart.x;
					ptfStartX.y = sResult.sLine_Vertical2.ptStart.y;
					ptfEndX.x = sResult.sLine_Vertical2.ptEnd.x;
					ptfEndX.y = sResult.sLine_Vertical2.ptEnd.y;
					cv::line(matDisplay, ptfStartX, ptfEndX, scLine, nLineWidth);
				}
			}

			return true;
		}

		// 顯示膠邊位置
		// [In] sResult : 膠輪廓檢測結果
		// [In] scContours : 輪廓的顏色
		// [In] [Out] matDisplay : 輸入彩色影像, 將檢測結果畫上去
		// [In] nLineWidth = 輪廓線寬, 0=>1 pixels; 1=>3 pixels ...
		bool MeasurementBlackGlue::DisplayGluePosition(const SGlueEdge_Result& sResult, const Scalar& scContours, Mat& matDisplay, const int& nContourWidth)
		{
			if (matDisplay.empty() || matDisplay.channels() != 3) {
				return false;
			}

			if (sResult.bEnable) {
				// 畫輪廓
				jet_imagefunction::DrawEdgePoint(matDisplay, sResult.vtptContoursPos, scContours, nContourWidth);
			}

			return true;
		}

		bool MeasurementBlackGlue::DisplayGluePosition(const SFindEdge_Result& sResult, const Scalar& scContours, Mat& matDisplay, const int& nContourWidth)
		{
			if (matDisplay.empty() || matDisplay.channels() != 3) {
				return false;
			}

			// 畫輪廓
			jet_imagefunction::DrawEdgePoint(matDisplay, sResult.vtptContoursPos, scContours, nContourWidth);

			return true;
		}

		// 顯示Coating輪廓
		// [In] sResult : Coating檢測結果
		// [In] scContours : 輪廓的顏色
		// [In] [Out] matDisplay : 輸入彩色影像, 將檢測結果畫上去
		// [In] nContourWidth = 輪廓線寬, 0=>1 pixels; 1=>3 pixels ...
		bool MeasurementBlackGlue::DisplayCoatingPosition(const SCoatingEdge_Result& sResult, const Scalar& scContours, Mat& matDisplay, const int& nContourWidth)
		{
			if (matDisplay.empty() || matDisplay.channels() != 3) {
				return false;
			}

			if (sResult.bEnable) {
				// 畫輪廓
				for (int k = 0; k < sResult.nCoatingCount; ++k) {
					jet_imagefunction::DrawEdgePoint(matDisplay, sResult.vt2ptContoursPos[k], scContours, nContourWidth);
				}
			}

			return true;
		}

		// 顯示散熱膠IC位置的散熱膠輪廓
		// [In] sResult : 散熱膠檢測結果
		// [In] scContours : 輪廓的顏色
		// [In] [Out] matDisplay : 輸入彩色影像, 將檢測結果畫上去
		// [In] nContourWidth = 輪廓或ROI寬, 0=>1 pixels; 1=>3 pixels ...
		bool MeasurementBlackGlue::DisplayThermalGluePosition(const SThermalGlue_Result& sResult, const Scalar& scContours, Mat& matDisplay, const int& nContourWidth)
		{
			if (matDisplay.empty() || matDisplay.channels() != 3) {
				return false;
			}

			if (sResult.bEnable) {

				// 畫輪廓
				if (sResult.nGlue_Count == sResult.vtsGlueEdgeInfo.size()) {
					for (int k = 0; k < sResult.nGlue_Count; ++k) {
						for (int h = 0; h < sResult.vtsGlueEdgeInfo[k].nContoursCount; ++h) {
							jet_imagefunction::DrawEdgePoint(matDisplay, sResult.vtsGlueEdgeInfo[k].vtptContoursPos, scContours, nContourWidth);
						}
					}
				}

				if (sResult.vtptDieCorner.size() == 4) {
					Point cvPoint1, cvPoint2;
					cvPoint1.x = sResult.vtptDieCorner[0].x;
					cvPoint1.y = sResult.vtptDieCorner[0].y;
					cvPoint2.x = sResult.vtptDieCorner[1].x;
					cvPoint2.y = sResult.vtptDieCorner[1].y;
					line(matDisplay, cvPoint1, cvPoint2, Scalar(255, 0, 0), nContourWidth);

					cvPoint1.x = sResult.vtptDieCorner[1].x;
					cvPoint1.y = sResult.vtptDieCorner[1].y;
					cvPoint2.x = sResult.vtptDieCorner[2].x;
					cvPoint2.y = sResult.vtptDieCorner[2].y;
					line(matDisplay, cvPoint1, cvPoint2, Scalar(0, 255, 0), nContourWidth);

					cvPoint1.x = sResult.vtptDieCorner[2].x;
					cvPoint1.y = sResult.vtptDieCorner[2].y;
					cvPoint2.x = sResult.vtptDieCorner[3].x;
					cvPoint2.y = sResult.vtptDieCorner[3].y;
					line(matDisplay, cvPoint1, cvPoint2, Scalar(0, 0, 255), nContourWidth);

					cvPoint1.x = sResult.vtptDieCorner[3].x;
					cvPoint1.y = sResult.vtptDieCorner[3].y;
					cvPoint2.x = sResult.vtptDieCorner[0].x;
					cvPoint2.y = sResult.vtptDieCorner[0].y;
					line(matDisplay, cvPoint1, cvPoint2, Scalar(255, 255, 0), nContourWidth);
				}
				else {
					Rect cvROI;
					jet_imagefunction::RECTToRect(m_sResult.sThermalGlue.rectDie, 0, 0, cvROI);
					cv::rectangle(matDisplay, cvROI, Scalar(0, 0, 255), nContourWidth);
				}
			}

			return true;
		}

		// 顯示單一ROI框的量測結果
		// [In] sResult : ROI框檢測結果
		// [In] scROI : ROI框的顏色
		// [In] scDistance : 顯示距離的顏色
		// [In] scDistance : 顯示距離的顏色
		// [In] scMaxDistance : 顯示最大距離位置記號的顏色
		// [In] scMinDistance : 顯示最小距離位置記號的顏色
		// [In] [Out] matDisplay : 輸入彩色影像, 將檢測結果畫上去
		// [In] nContourWidth = 輪廓或ROI線的寬, 0=>1 pixels; 1=>3 pixels ...
		// [In] nLineWidth = 量測線的寬, 1=>1 pixels; 2=>2 pixels ...
		bool MeasurementBlackGlue::DisplayRoiResult(const SMultipleROI_Result& sResult, const Scalar& scROI, const Scalar& scDistance, const Scalar& scMaxDistance, const Scalar& scMinDistance, const string& strInfo, Mat& matDisplay, const bool& bShowROI, const bool& bShowLine, const bool& bMax, const bool& bMin, const int& nContourWidth, const int& nLineWidth)
		{
			if (matDisplay.empty() || matDisplay.channels() != 3) {
				return false;
			}

			if (sResult.bEnable)
			{
				Point ptBase;
				float fSize = 0.5;

				Rect cvROI;
				Point cvptStart, cvptEnd;
				for (int k = 0; k < sResult.nRoiCount; ++k)
				{
					string strIndex = to_string(k + 1);

					// 畫 ROI
					if (bShowROI) {
						cvROI.x = sResult.vtsRoiInfo[k].rectROI.left;
						cvROI.y = sResult.vtsRoiInfo[k].rectROI.top;
						cvROI.width = sResult.vtsRoiInfo[k].rectROI.right - sResult.vtsRoiInfo[k].rectROI.left + 1;
						cvROI.height = sResult.vtsRoiInfo[k].rectROI.bottom - sResult.vtsRoiInfo[k].rectROI.top + 1;
						cv::rectangle(matDisplay, cvROI, scROI, nContourWidth);
					}

					// 畫寬度量測結果
					if (bShowLine) {
						for (int h = 0; h < sResult.vtsRoiInfo[k].nPositionCount; h += m_nLineInterval)
						{
							Point cvptStart, cvptEnd;
							cvptStart.x = sResult.vtsRoiInfo[k].vtsPositionInfo[h].ptStart.x;
							cvptStart.y = sResult.vtsRoiInfo[k].vtsPositionInfo[h].ptStart.y;
							cvptEnd.x = sResult.vtsRoiInfo[k].vtsPositionInfo[h].ptEnd.x;
							cvptEnd.y = sResult.vtsRoiInfo[k].vtsPositionInfo[h].ptEnd.y;
							if ((cvptStart.x == 0 && cvptStart.y == 0) || (cvptEnd.x == 0 && cvptEnd.y == 0)) continue;
							cv::line(matDisplay, cvptStart, cvptEnd, scDistance, nLineWidth);
						}
					}

					if (bMax) {
						cvptStart.x = sResult.vtsRoiInfo[k].sMaxDist.ptStart.x;
						cvptStart.y = sResult.vtsRoiInfo[k].sMaxDist.ptStart.y;
						cvptEnd.x = sResult.vtsRoiInfo[k].sMaxDist.ptEnd.x;
						cvptEnd.y = sResult.vtsRoiInfo[k].sMaxDist.ptEnd.y;
						cv::line(matDisplay, cvptStart, cvptEnd, scDistance, nLineWidth);

						// 畫寬度最長距離
						jet_imagefunction::DrawCrossPoint(sResult.vtsRoiInfo[k].sMaxDist.ptStart, 5, 2, scMaxDistance, matDisplay);
						jet_imagefunction::DrawCrossPoint(sResult.vtsRoiInfo[k].sMaxDist.ptEnd, 5, 2, scMaxDistance, matDisplay);

						ptBase.x = (cvptStart.x + cvptEnd.x) / 2;
						ptBase.y = (cvptStart.y + cvptEnd.y) / 2;
						cv::putText(matDisplay, strInfo + strIndex + "-Max", ptBase, FONT_HERSHEY_SIMPLEX, fSize, scMaxDistance, 1);
					}

					if (bMin) {
						cvptStart.x = sResult.vtsRoiInfo[k].sMinDist.ptStart.x;
						cvptStart.y = sResult.vtsRoiInfo[k].sMinDist.ptStart.y;
						cvptEnd.x = sResult.vtsRoiInfo[k].sMinDist.ptEnd.x;
						cvptEnd.y = sResult.vtsRoiInfo[k].sMinDist.ptEnd.y;
						cv::line(matDisplay, cvptStart, cvptEnd, scDistance, nLineWidth);

						// 畫寬度最短距離
						jet_imagefunction::DrawCrossPoint(sResult.vtsRoiInfo[k].sMinDist.ptStart, 5, 2, scMinDistance, matDisplay);
						jet_imagefunction::DrawCrossPoint(sResult.vtsRoiInfo[k].sMinDist.ptEnd, 5, 2, scMinDistance, matDisplay);

						ptBase.x = (cvptStart.x + cvptEnd.x) / 2;
						ptBase.y = (cvptStart.y + cvptEnd.y) / 2;
						cv::putText(matDisplay, strInfo + strIndex + "-Min", ptBase, FONT_HERSHEY_SIMPLEX, fSize, scMinDistance, 1);
					}
				}
			}
			return true;
		}

		// 顯示量測首尾ROI距離的結果
		// [In] sResult : StartEnd ROI 檢測結果
		// [In] scROI : ROI框的顏色
		// [In] scDistance : 顯示距離的顏色
		// [In] scMaxDistance : 顯示最大距離位置記號的顏色
		// [In] [Out] matDisplay : 輸入彩色影像, 將檢測結果畫上去
		// [In] nContourWidth = 輪廓或ROI線的寬, 0=>1 pixels; 1=>3 pixels ...
		// [In] nLineWidth = 量測線的寬, 1=>1 pixels; 2=>2 pixels ...
		bool MeasurementBlackGlue::DisplayStartEndResult(const SStartEndROI_Result& sResult, const Scalar& scROI, const Scalar& scDistance, const Scalar& scMaxDistance, const string& strInfo, Mat& matDisplay, const bool& bShowROI, const int& nContourWidth, const int& nLineWidth)
		{
			if (matDisplay.empty() || matDisplay.channels() != 3) {
				return false;
			}

			// 畫膠首尾距離量測結果
			if (sResult.bEnable)
			{
				if (bShowROI) {
					for (int k = 0; k < sResult.nCount; ++k) {
						// 畫 Start ROI
						Rect cvROI;
						cvROI.x = sResult.vtsStartEnd[k].rectStart.left;
						cvROI.y = sResult.vtsStartEnd[k].rectStart.top;
						cvROI.width = sResult.vtsStartEnd[k].rectStart.right - sResult.vtsStartEnd[k].rectStart.left + 1;
						cvROI.height = sResult.vtsStartEnd[k].rectStart.bottom - sResult.vtsStartEnd[k].rectStart.top + 1;
						cv::rectangle(matDisplay, cvROI, scROI, nContourWidth);

						// 畫 End ROI
						cvROI.x = sResult.vtsStartEnd[k].rectEnd.left;
						cvROI.y = sResult.vtsStartEnd[k].rectEnd.top;
						cvROI.width = sResult.vtsStartEnd[k].rectEnd.right - sResult.vtsStartEnd[k].rectEnd.left + 1;
						cvROI.height = sResult.vtsStartEnd[k].rectEnd.bottom - sResult.vtsStartEnd[k].rectEnd.top + 1;
						cv::rectangle(matDisplay, cvROI, scROI, nContourWidth);
					}
				}

				for (int k = 0; k < sResult.nCount; ++k) {
					string strIndex = to_string(k + 1);
					Point cvptStart, cvptEnd;
					cvptStart.x = sResult.vtsStartEnd[k].sDist.ptStart.x;
					cvptStart.y = sResult.vtsStartEnd[k].sDist.ptStart.y;
					cvptEnd.x = sResult.vtsStartEnd[k].sDist.ptEnd.x;
					cvptEnd.y = sResult.vtsStartEnd[k].sDist.ptEnd.y;
					cv::line(matDisplay, cvptStart, cvptEnd, scDistance, nLineWidth);
					jet_imagefunction::DrawCrossPoint(sResult.vtsStartEnd[k].sDist.ptStart, 5, 2, scMaxDistance, matDisplay);
					jet_imagefunction::DrawCrossPoint(sResult.vtsStartEnd[k].sDist.ptEnd, 5, 2, scMaxDistance, matDisplay);

					Point ptBase;
					float fSize = 0.6;
					ptBase.x = (cvptStart.x + cvptEnd.x) / 2;
					ptBase.y = (cvptStart.y + cvptEnd.y) / 2;
					cv::putText(matDisplay, strInfo + strIndex, ptBase, FONT_HERSHEY_SIMPLEX, fSize, scMaxDistance, 1);
				}
			}

			return true;
		}
#pragma endregion

		// 影像儲存相關功能
#pragma region Save Image

		// 儲存參數
		// sParam : 要儲存的參數
		// strPathName : 儲存路徑+檔名(不須要檔名)
		bool MeasurementBlackGlue::Save_Parameter(const SMeasurementBlackGlue_Parameter& sParam, const string& strPathName)
		{
			if (strPathName.empty()) {
				return false;
			}

			vector<ConfigData> vtsConfig;
			if (ParameterToConfig(sParam, vtsConfig) == false) {
				return false;
			}

			ConfigFile config;
			if (config.Save(strPathName, vtsConfig, 2) != 1) {
				return false;
			}

			return true;
		}

		// 讀取參數
		// strPathName : 讀取路徑+檔名(不須要檔名)
		// sParam : 讀取後存放資料的變數
		bool MeasurementBlackGlue::Load_Parameter(const string& strPathName, SMeasurementBlackGlue_Parameter& sParam)
		{
			if (strPathName.empty()) {
				return false;
			}

			ConfigFile config;
			std::vector<ConfigData> vtsConfig;
			if (config.Read(strPathName, vtsConfig, 2) != 1) {
				//if (!Save_Parameter(m_sParam, strPathName)) {
				//	return false;
				//}
				return false;
			}

			// 將讀到的 vtsConfig -> sParam
			if (!ConfigToParameter(vtsConfig, sParam)) {
				return false;
			}

			return true;
		}

		// AMD黑膠讀取參數
		// strPathName : 讀取路徑+檔名(不須要檔名)
		// sParam : 讀取後存放資料的變數
		bool MeasurementBlackGlue::SaveParameter_FluxArea(const SMeasurementFluxArea_Parameter& sParam, const string& strPathName)
		{
			string strInfo;
			if (strPathName.empty()) {
				return false;
			}

			vector<ConfigData> vtsConfig(1);
			vtsConfig[0].AppName = "Info";
			vtsConfig[0].AddKeyValue("ErrorMessage", m_strErrorMessage, DATATYPE_STRING);
			vtsConfig[0].AddKeyValue("TotalArea", jet_imagefunction::FloatingToString("%.5f", sParam.fTotalArea), DATATYPE_FLOAT);
			vtsConfig[0].AddKeyValue("ResolutionX", jet_imagefunction::FloatingToString("%.5f", sParam.fResolutionX), DATATYPE_FLOAT);
			vtsConfig[0].AddKeyValue("ResolutionY", jet_imagefunction::FloatingToString("%.5f", sParam.fResolutionY), DATATYPE_FLOAT);
			vtsConfig[0].AddKeyValue("Upper", jet_imagefunction::FloatingToString("%.5f", sParam.sLimit.fUpper), DATATYPE_FLOAT);
			vtsConfig[0].AddKeyValue("Lower", jet_imagefunction::FloatingToString("%.5f", sParam.sLimit.fLower), DATATYPE_FLOAT);

			// 膠邊
			if (FindEdgeParameterToConfig("FluxEdge", sParam.sFluxEdge, vtsConfig) == false) {
				return false;
			}

			ConfigFile config;
			if (config.Save(strPathName, vtsConfig, 2) != 1) {
				return false;
			}

			return true;
		}

		// 膠面積量測讀取參數
		// strPathName : 讀取路徑+檔名(不須要檔名)
		// sParam : 讀取後存放資料的變數
		bool MeasurementBlackGlue::LoadParameter_FluxArea(const string& strPathName, SMeasurementFluxArea_Parameter& sParam)
		{
			if (strPathName.empty()) {
				return false;
			}

			ConfigFile config;
			std::vector<ConfigData> vtsConfig;
			if (config.Read(strPathName, vtsConfig, 2) != 1) {
				return false;
			}

			string strAppName = "Info";
			string tmp;

			if (config.FindConfigValue(vtsConfig, strAppName, "TotalArea", tmp))
				sParam.fTotalArea = config.parseFloat(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "ResolutionX", tmp))
				sParam.fResolutionX = config.parseFloat(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "ResolutionY", tmp))
				sParam.fResolutionY = config.parseFloat(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "Upper", tmp))
				sParam.sLimit.fUpper = config.parseFloat(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "Lower", tmp))
				sParam.sLimit.fLower = config.parseFloat(tmp);

			if (!FindEdgeConfigToParameter("FluxEdge", vtsConfig, sParam.sFluxEdge)) {
				return false;
			}

			return true;
		}


		// 膠高量測儲存參數
		// sParam : 要儲存的參數
		// strPathName : 儲存路徑+檔名(不須要檔名)
		bool MeasurementBlackGlue::SaveParameter_GlueHeight(const SMeasurementGlueHeightRatio_Parameter& sParam, const string& strPathName)
		{
			string strInfo;
			if (strPathName.empty()) {
				return false;
			}
			
			vector<ConfigData> vtsConfig(1);
			vtsConfig[0].AppName = "Info";
			vtsConfig[0].AddKeyValue("ErrorMessage", m_strErrorMessage, DATATYPE_STRING);
			vtsConfig[0].AddKeyValue("ResolutionX", jet_imagefunction::FloatingToString("%.5f", sParam.fResolutionX), DATATYPE_FLOAT);
			vtsConfig[0].AddKeyValue("ResolutionY", jet_imagefunction::FloatingToString("%.5f", sParam.fResolutionY), DATATYPE_FLOAT);
			vtsConfig[0].AddKeyValue("IC_Height", jet_imagefunction::FloatingToString("%.5f", sParam.fIC_Height), DATATYPE_FLOAT);	
			vtsConfig[0].AddKeyValue("IC_Indent_Left_Top", to_string(sParam.nICEdge_Indent_Left_Top), DATATYPE_INTEGER);
			vtsConfig[0].AddKeyValue("IC_Indent_Right_Bottom", to_string(sParam.nICEdge_Indent_Right_Bottom), DATATYPE_INTEGER);
			vtsConfig[0].AddKeyValue("IC_BaseLine_SearchRange", to_string(sParam.nIC_BaseLine_SearchRange), DATATYPE_INTEGER);
			vtsConfig[0].AddKeyValue("MeasuringDirection", to_string(sParam.nMeasuringDirection), DATATYPE_INTEGER);
			vtsConfig[0].AddKeyValue("MeasuringObject", to_string(sParam.nMeasuringObject), DATATYPE_INTEGER);
			vtsConfig[0].AddKeyValue("BlockCount", to_string(sParam.nBlockCount), DATATYPE_INTEGER);

			if (sParam.nBlockCount == sParam.vtnHeightMode.size()) {
				for (int k = 0; k < sParam.nBlockCount; ++k) {
					vtsConfig[0].AddKeyValue("HeightMode-" + to_string(k + 1), to_string(sParam.vtnHeightMode[k]), DATATYPE_INTEGER);
				}
			}
			else {
				for (int k = 0; k < sParam.nBlockCount; ++k) {
					vtsConfig[0].AddKeyValue("HeightMode-" + to_string(k + 1), to_string(1), DATATYPE_INTEGER);
				}
			}

			vtsConfig[0].AddKeyValue("HeightMode-Final", to_string(sParam.nFinal_HeightMode), DATATYPE_INTEGER);
			vtsConfig[0].AddKeyValue("Limit-Upper", to_string(sParam.sLimit.fUpper), DATATYPE_FLOAT);
			vtsConfig[0].AddKeyValue("Limit-Lower", to_string(sParam.sLimit.fLower), DATATYPE_FLOAT);

			// 膠邊
			if (FindEdgeParameterToConfig("GlueEdge", sParam.sGlueEdge, vtsConfig) == false) {
				return false;
			}

			// IC Edge
			if (FindEdgeParameterToConfig("ICEdge", sParam.sIcEdge, vtsConfig) == false) {
				return false;
			}

			ConfigFile config;
			if (config.Save(strPathName, vtsConfig, 2) != 1) {
				return false;
			}

			return true;
		}

		// 膠高量測讀取參數
		// strPathName : 讀取路徑+檔名(不須要檔名)
		// sParam : 讀取後存放資料的變數
		bool MeasurementBlackGlue::LoadParameter_GlueHeight(const string& strPathName, SMeasurementGlueHeightRatio_Parameter& sParam)
		{
			if (strPathName.empty()) {
				return false;
			}

			ConfigFile config;
			std::vector<ConfigData> vtsConfig;
			if (config.Read(strPathName, vtsConfig, 2) != 1) {
				return false;
			}

			string strAppName = "Info";
			string tmp;

			if (config.FindConfigValue(vtsConfig, strAppName, "ResolutionX", tmp))
				sParam.fResolutionX = config.parseFloat(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "ResolutionY", tmp))
				sParam.fResolutionY = config.parseFloat(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "IC_Height", tmp))
				sParam.fIC_Height = config.parseFloat(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "IC_Indent_Left_Top", tmp))
				sParam.nICEdge_Indent_Left_Top = config.parseInt(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "IC_Indent_Right_Bottom", tmp))
				sParam.nICEdge_Indent_Right_Bottom = config.parseInt(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "IC_BaseLine_SearchRange", tmp))
				sParam.nIC_BaseLine_SearchRange = config.parseInt(tmp);
			
			if (config.FindConfigValue(vtsConfig, strAppName, "MeasuringDirection", tmp))
				sParam.nMeasuringDirection = config.parseInt(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "MeasuringObject", tmp))
				sParam.nMeasuringObject = config.parseInt(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "BlockCount", tmp))
				sParam.nBlockCount = config.parseFloat(tmp);

			if (sParam.nBlockCount < 1) {
				sParam.nBlockCount = 1;
			}

			sParam.vtnHeightMode.resize(sParam.nBlockCount, 1);
			for (int k = 0; k < sParam.nBlockCount; ++k) {
				if (config.FindConfigValue(vtsConfig, strAppName, "HeightMode-" + to_string(k + 1), tmp))
					sParam.vtnHeightMode[k] = config.parseInt(tmp);
			}

			if (config.FindConfigValue(vtsConfig, strAppName, "HeightMode-Final", tmp))
				sParam.nFinal_HeightMode = config.parseInt(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "Limit-Upper", tmp))
				sParam.sLimit.fUpper = config.parseFloat(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "Limit-Lower", tmp))
				sParam.sLimit.fLower = config.parseFloat(tmp);

			if (!FindEdgeConfigToParameter("GlueEdge", vtsConfig, sParam.sGlueEdge)) {
				return false;
			}

			if (!FindEdgeConfigToParameter("ICEdge", vtsConfig, sParam.sIcEdge)) {
				return false;
			}

			return true;
		}

		// 膠面積量測儲存參數
		// sParam : 要儲存的參數
		// strPathName : 儲存路徑+檔名(不須要檔名)
		bool MeasurementBlackGlue::SaveParameter_GlueArea(const SMeasurementGlueArea_Parameter& sParam, const string& strPathName)
		{
			string strInfo;
			if (strPathName.empty()) {
				return false;
			}

			vector<ConfigData> vtsConfig(1);
			vtsConfig[0].AppName = "Info";
			vtsConfig[0].AddKeyValue("ErrorMessage", m_strErrorMessage, DATATYPE_STRING);
			vtsConfig[0].AddKeyValue("ResolutionX", jet_imagefunction::FloatingToString("%.5f", sParam.fResolutionX), DATATYPE_FLOAT);
			vtsConfig[0].AddKeyValue("ResolutionY", jet_imagefunction::FloatingToString("%.5f", sParam.fResolutionY), DATATYPE_FLOAT);
			vtsConfig[0].AddKeyValue("IC_Width", to_string(sParam.nIC_Width), DATATYPE_INTEGER);
			vtsConfig[0].AddKeyValue("IC_Height", to_string(sParam.nIC_Height), DATATYPE_INTEGER);
			vtsConfig[0].AddKeyValue("IC_Indent_Width", to_string(sParam.nIC_Indent_Width), DATATYPE_INTEGER);
			vtsConfig[0].AddKeyValue("IC_Indent_Height", to_string(sParam.nIC_Indent_Height), DATATYPE_INTEGER);
			vtsConfig[0].AddKeyValue("GlueSize_MinWidth", to_string(sParam.nGlueSize_MinWidth), DATATYPE_INTEGER);
			vtsConfig[0].AddKeyValue("GlueSize_MinHeight", to_string(sParam.nGlueSize_MinHeight), DATATYPE_INTEGER);
			vtsConfig[0].AddKeyValue("GlueNG_Count", to_string(sParam.nGlueNG_Count), DATATYPE_INTEGER);
			vtsConfig[0].AddKeyValue("GlueNG_Width", to_string(sParam.nGlueNG_Width), DATATYPE_INTEGER);
			vtsConfig[0].AddKeyValue("GlueNG_Height", to_string(sParam.nGlueNG_Height), DATATYPE_INTEGER);
			vtsConfig[0].AddKeyValue("IcNG_DiffWidth", to_string(sParam.nIC_NG_DiffWidth), DATATYPE_INTEGER);
			vtsConfig[0].AddKeyValue("IcNG_DiffHeight", to_string(sParam.nIC_NG_DiffHeight), DATATYPE_INTEGER);

			// 膠邊
			if (FindEdgeParameterToConfig("GlueEdge", sParam.sGlueEdge, vtsConfig) == false) {
				return false;
			}

			// IC邊
			if (FindEdgeParameterToConfig("IcEdge", sParam.sIcEdge, vtsConfig) == false) {
				return false;
			}

			ConfigFile config;
			if (config.Save(strPathName, vtsConfig, 2) != 1) {
				return false;
			}

			return true;
		}

		// 膠面積量測讀取參數
		// strPathName : 讀取路徑+檔名(不須要檔名)
		// sParam : 讀取後存放資料的變數
		bool MeasurementBlackGlue::LoadParameter_GlueArea(const string& strPathName, SMeasurementGlueArea_Parameter& sParam)
		{
			if (strPathName.empty()) {
				return false;
			}

			ConfigFile config;
			std::vector<ConfigData> vtsConfig;
			if (config.Read(strPathName, vtsConfig, 2) != 1) {
				return false;
			}

			string strAppName = "Info";
			string tmp;

			if (config.FindConfigValue(vtsConfig, strAppName, "ResolutionX", tmp))
				sParam.fResolutionX = config.parseFloat(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "ResolutionY", tmp))
				sParam.fResolutionY = config.parseFloat(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "IC_Width", tmp))
				sParam.nIC_Width = config.parseInt(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "IC_Height", tmp))
				sParam.nIC_Height = config.parseInt(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "IC_Indent_Width", tmp))
				sParam.nIC_Indent_Width = config.parseInt(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "IC_Indent_Height", tmp))
				sParam.nIC_Indent_Height = config.parseInt(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "GlueSize_MinWidth", tmp))
				sParam.nGlueSize_MinWidth = config.parseInt(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "GlueSize_MinHeight", tmp))
				sParam.nGlueSize_MinHeight = config.parseInt(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "GlueNG_Count", tmp))
				sParam.nGlueNG_Count = config.parseInt(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "GlueNG_Width", tmp))
				sParam.nGlueNG_Width = config.parseInt(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "GlueNG_Height", tmp))
				sParam.nGlueNG_Height = config.parseInt(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "IcNG_DiffWidth", tmp))
				sParam.nIC_NG_DiffWidth = config.parseInt(tmp);

			if (config.FindConfigValue(vtsConfig, strAppName, "IcNG_DiffHeight", tmp))
				sParam.nIC_NG_DiffHeight = config.parseInt(tmp);


			if (!FindEdgeConfigToParameter("GlueEdge", vtsConfig, sParam.sGlueEdge)) {
				return false;
			}

			if (!FindEdgeConfigToParameter("IcEdge", vtsConfig, sParam.sIcEdge)) {
				return false;
			}

			return true;
		}

		// spil 膠邊到零件邊偵測儲存參數
		bool MeasurementBlackGlue::SaveParameter_GlueToPart(const SMeasurementGlueToPart_Parameter& sParam, const string& strPathName)
		{
			string strInfo;
			if (strPathName.empty()) {
				return false;
			}

			vector<ConfigData> vtsConfig;

			// 膠邊
			if (FindEdgeParameterToConfig("GlueEdge", sParam.sGlue, vtsConfig) == false) {
				return false;
			}

			// 零件邊
			if (FindEdgeParameterToConfig("PartEdge", sParam.sPart, vtsConfig) == false) {
				return false;
			}

			vtsConfig[1].AddKeyValue("IcROI_L", to_string(sParam.rectIcROI.left), DATATYPE_INTEGER);
			vtsConfig[1].AddKeyValue("IcROI_T", to_string(sParam.rectIcROI.top), DATATYPE_INTEGER);
			vtsConfig[1].AddKeyValue("IcROI_R", to_string(sParam.rectIcROI.right), DATATYPE_INTEGER);
			vtsConfig[1].AddKeyValue("IcROI_B", to_string(sParam.rectIcROI.bottom), DATATYPE_INTEGER);

			vtsConfig[1].AddKeyValue("Part_Gradient_Threshold", to_string(sParam.m_nPart_Gradient_Threshold), DATATYPE_INTEGER);
			vtsConfig[1].AddKeyValue("Part_Types", to_string(sParam.m_nPartTypes), DATATYPE_INTEGER);

			ConfigFile config;
			if (config.Save(strPathName, vtsConfig, 2) != 1) {
				return false;
			}
		}

		
		// spil 膠邊到零件邊偵測讀取參數
		bool MeasurementBlackGlue::LoadParameter_GlueToPart(const string& strPathName, SMeasurementGlueToPart_Parameter& sParam)
		{
			if (strPathName.empty()) {
				return false;
			}

			ConfigFile config;
			std::vector<ConfigData> vtsConfig;
			if (config.Read(strPathName, vtsConfig, 2) != 1) {
				return false;
			}

			if (!FindEdgeConfigToParameter("GlueEdge", vtsConfig, sParam.sGlue)) {
				return false;
			}

			if (!FindEdgeConfigToParameter("PartEdge", vtsConfig, sParam.sPart)) {
				return false;
			}

			ConfigFile sConfigfile;
			string tmp;

			if (sConfigfile.FindConfigValue(vtsConfig, "PartEdge", "IcROI_L", tmp))
				sParam.rectIcROI.left = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "PartEdge", "IcROI_T", tmp))
				sParam.rectIcROI.top = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "PartEdge", "IcROI_R", tmp))
				sParam.rectIcROI.right = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "PartEdge", "IcROI_B", tmp))
				sParam.rectIcROI.bottom = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "PartEdge", "Part_Gradient_Threshold", tmp))
				sParam.m_nPart_Gradient_Threshold = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "PartEdge", "Part_Types", tmp))
				sParam.m_nPartTypes = sConfigfile.parseInt(tmp);

			return true;
		}

		// 儲存量測結果文字檔
		// strPathName : 讀取路徑+檔名(不須要檔名)
		// sResult : 量測結果
		bool MeasurementBlackGlue::Save_Result_Txt(const string& strPathName, const SMeasurementBlackGlue_Result& sResult)
		{
			if (strPathName.empty()) {
				return false;
			}

			fstream SaveFile;
			SaveFile.open(strPathName+".txt", ios::out | ios::trunc); //寫檔, 清除內容

			string strData, strTemp;
			strData = "\n";

			// Board
			strTemp = "[BoardEdge]\n";
			if (sResult.sBoardEdge.bEnable) {
				strTemp += "Enable=true\n";
			}
			else {
				strTemp += "Enable=false\n";
			}
			strData += (strTemp + "\n");

			// Glue
			strTemp = "[GlueEdge]\n";
			if (sResult.sGlueEdge.bEnable) {
				strTemp += "Enable=true\n";
			}
			else {
				strTemp += "Enable=false\n";
			}
			strData += (strTemp + "\n");

			// Coating
			strTemp = "[Coating]\n";
			if (sResult.sCoatingEdge.bEnable) {
				strTemp += "Enable=true\n";
			}
			else {
				strTemp += "Enable=false\n";
			}
			strTemp += ("Count=" + to_string(sResult.sCoatingEdge.nCoatingCount) + "\n");
			strData += (strTemp + "\n");	

			// Width_Horizontal
			strTemp = "[Width_Horizontal]\n";
			if (sResult.sWidth_Horizontal.bEnable) {
				strTemp += "Enable=true\n";
				strTemp += ("Count=" + to_string(sResult.sWidth_Horizontal.nRoiCount) + "\n");
				for (int k = 0; k < sResult.sWidth_Horizontal.nRoiCount; ++k) {
					// Max
					if (sResult.sWidth_Horizontal.vtsRoiInfo[k].bResult_MaxDist) strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=OK\n");
					else                                                        strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=NG\n");
					strTemp += ("[" + to_string(k+1) + "]-MaxDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sWidth_Horizontal.vtsRoiInfo[k].sMaxDist.fDistance)+"\n");
					
					// Min
					if (sResult.sWidth_Horizontal.vtsRoiInfo[k].bResult_MinDist) strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=OK\n");
					else                                                        strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sWidth_Horizontal.vtsRoiInfo[k].sMinDist.fDistance) + "\n");

					// Diff
					if (sResult.sWidth_Horizontal.vtsRoiInfo[k].bResult_Diff_MaxMin) strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMin-Result=OK\n");
					else                                                        strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMin-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sWidth_Horizontal.vtsRoiInfo[k].fDiff_Dist_MaxMin) + "\n");
				}
			}
			else {
				strTemp += "Enable=false\n";
			}
			strData += (strTemp + "\n");

			// Width_Vertical
			strTemp = "[Width_Vertical]\n";
			if (sResult.sWidth_Vertical.bEnable) {
				strTemp += "Enable=true\n";
				strTemp += ("Count=" + to_string(sResult.sWidth_Vertical.nRoiCount) + "\n");
				for (int k = 0; k < sResult.sWidth_Vertical.nRoiCount; ++k) {
					// Max
					if (sResult.sWidth_Vertical.vtsRoiInfo[k].bResult_MaxDist) strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=OK\n");
					else                                                      strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MaxDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sWidth_Vertical.vtsRoiInfo[k].sMaxDist.fDistance) + "\n");
					
					// Min
					if (sResult.sWidth_Vertical.vtsRoiInfo[k].bResult_MinDist) strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=OK\n");
					else                                                      strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sWidth_Vertical.vtsRoiInfo[k].sMinDist.fDistance) + "\n");

					// Diff
					if (sResult.sWidth_Vertical.vtsRoiInfo[k].bResult_Diff_MaxMin) strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMin-Result=OK\n");
					else                                                       strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMin-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sWidth_Vertical.vtsRoiInfo[k].fDiff_Dist_MaxMin) + "\n");
				}
			}
			else {
				strTemp += "Enable=false\n";
			}
			strData += (strTemp + "\n");

			// Width_Arc
			strTemp = "[Width_Arc]\n";
			if (sResult.sWidth_Arc.bEnable) {
				strTemp += "Enable=true\n";
				strTemp += ("Count=" + to_string(sResult.sWidth_Arc.nRoiCount) + "\n");
				for (int k = 0; k < sResult.sWidth_Arc.nRoiCount; ++k) {
					// Max
					if (sResult.sWidth_Arc.vtsRoiInfo[k].bResult_MaxDist) strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=OK\n");
					else                                                 strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MaxDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sWidth_Arc.vtsRoiInfo[k].sMaxDist.fDistance) + "\n");
					
					// Min
					if (sResult.sWidth_Arc.vtsRoiInfo[k].bResult_MinDist) strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=OK\n");
					else                                                 strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sWidth_Arc.vtsRoiInfo[k].sMinDist.fDistance) + "\n");

					// Diff
					if (sResult.sWidth_Arc.vtsRoiInfo[k].bResult_Diff_MaxMin) strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMin-Result=OK\n");
					else                                                  strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMin-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sWidth_Arc.vtsRoiInfo[k].fDiff_Dist_MaxMin) + "\n");
				}
			}
			else {
				strTemp += "Enable=false\n";
			}
			strData += (strTemp + "\n");

			// StartEnd
			strTemp = "[StartEnd]\n";
			if (sResult.sStartEnd.bEnable) {
				strTemp += "Enable=true\n";
				for (int k = 0; k < sResult.sStartEnd.nCount; ++k) {
					if (sResult.sStartEnd.vtsStartEnd[k].bResult_MaxDist)	strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=OK\n");
					else													strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MaxDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sStartEnd.vtsStartEnd[k].sDist.fDistance) + "\n");
				}
			}
			else {
				strTemp += "Enable=false\n";
			}
			strData += (strTemp + "\n");

			// BoardToGlue_Horizontal
			strTemp = "[BoardToGlue_Horizontal]\n";
			if (sResult.sBoardToGlue_Horizontal.bEnable) {
				strTemp += "Enable=true\n";
				strTemp += ("Count=" + to_string(sResult.sBoardToGlue_Horizontal.nRoiCount) + "\n");
				for (int k = 0; k < sResult.sBoardToGlue_Horizontal.nRoiCount; ++k) {
					// Max
					if (sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].bResult_MaxDist) strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=OK\n");
					else                                                               strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MaxDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].sMaxDist.fDistance) + "\n");
					
					// Min
					if (sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].bResult_MinDist) strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=OK\n");
					else                                                               strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].sMinDist.fDistance) + "\n");

					// Diff
					if (sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].bResult_Diff_MaxMin) strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMin-Result=OK\n");
					else                                                               strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMin-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].fDiff_Dist_MaxMin) + "\n");
				}
			}
			else {
				strTemp += "Enable=false\n";
			}
			strData += (strTemp + "\n");

			// BoardToGlue_Vertical
			strTemp = "[BoardToGlue_Vertical]\n";
			if (sResult.sBoardToGlue_Vertical.bEnable) {
				strTemp += "Enable=true\n";
				strTemp += ("Count=" + to_string(sResult.sBoardToGlue_Vertical.nRoiCount) + "\n");
				for (int k = 0; k < sResult.sBoardToGlue_Vertical.nRoiCount; ++k) {
					// Max
					if (sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].bResult_MaxDist) strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=OK\n");
					else                                                            strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MaxDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].sMaxDist.fDistance) + "\n");
					
					// Min
					if (sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].bResult_MinDist) strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=OK\n");
					else                                                            strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].sMinDist.fDistance) + "\n");

					// Diff
					if (sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].bResult_Diff_MaxMin) strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMin-Result=OK\n");
					else                                                             strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMin-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].fDiff_Dist_MaxMin) + "\n");
				}
			}
			else {
				strTemp += "Enable=false\n";
			}
			strData += (strTemp + "\n");

			// CoatingToBoard_Horizontal
			strTemp = "[CoatingToBoard_Horizontal]\n";
			if (sResult.sCoatingToBoard_Horizontal.bEnable) {
				strTemp += "Enable=true\n";
				strTemp += ("Count=" + to_string(sResult.sCoatingToBoard_Horizontal.nRoiCount) + "\n");
				for (int k = 0; k < sResult.sCoatingToBoard_Horizontal.nRoiCount; ++k) {
					// Min
					if (sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].bResult_MinDist) strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=OK\n");
					else                                                            strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].sMinDist.fDistance) + "\n");
				}
			}
			else {
				strTemp += "Enable=false\n";
			}
			strData += (strTemp + "\n");

			// CoatingToBoard_Vertical
			strTemp = "[CoatingToBoard_Vertical]\n";
			if (sResult.sCoatingToBoard_Vertical.bEnable) {
				strTemp += "Enable=true\n";
				strTemp += ("Count=" + to_string(sResult.sCoatingToBoard_Vertical.nRoiCount) + "\n");
				for (int k = 0; k < sResult.sCoatingToBoard_Vertical.nRoiCount; ++k) {
					// Min
					if (sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].bResult_MinDist) strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=OK\n");
					else                                                            strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].sMinDist.fDistance) + "\n");
				}
			}
			else {
				strTemp += "Enable=false\n";
			}
			strData += (strTemp + "\n");

			// CoatingToGlue_Horizontal
			strTemp = "[CoatingToGlue_Horizontal]\n";
			if (sResult.sCoatingToGlue_Horizontal.bEnable) {
				strTemp += "Enable=true\n";
				strTemp += ("Count=" + to_string(sResult.sCoatingToGlue_Horizontal.nRoiCount) + "\n");
				for (int k = 0; k < sResult.sCoatingToGlue_Horizontal.nRoiCount; ++k) {
					// Min
					if (sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].bResult_MinDist) strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=OK\n");
					else                                                            strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].sMinDist.fDistance) + "\n");
				}
			}
			else {
				strTemp += "Enable=false\n";
			}
			strData += (strTemp + "\n");

			// CoatingToGlue_Vertical
			strTemp = "[CoatingToGlue_Vertical]\n";
			if (sResult.sCoatingToGlue_Vertical.bEnable) {
				strTemp += "Enable=true\n";
				strTemp += ("Count=" + to_string(sResult.sCoatingToGlue_Vertical.nRoiCount) + "\n");
				for (int k = 0; k < sResult.sCoatingToGlue_Vertical.nRoiCount; ++k) {
					// Min
					if (sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].bResult_MinDist) strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=OK\n");
					else                                                            strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].sMinDist.fDistance) + "\n");
				}
			}
			else {
				strTemp += "Enable=false\n";
			}
			strData += (strTemp + "\n");

			// ThermalGlue
			strTemp = "[ThermalGlue_GlueWidth]\n";
			if (sResult.sThermalGlue.sGlueWidth.bEnable) {
				strTemp += "Enable=true\n";
				strTemp += ("Count=" + to_string(sResult.sThermalGlue.sGlueWidth.nRoiCount) + "\n");
				for (int k = 0; k < sResult.sThermalGlue.sGlueWidth.nRoiCount; ++k) {
					// Max
					if (sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MaxDist) strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=OK\n");
					else                                                              strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MaxDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMaxDist.fDistance) + "\n");

					// Min
					if (sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MinDist) strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=OK\n");
					else                                                              strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMinDist.fDistance) + "\n");

					// Diff
					if (sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_Diff_MaxMin) strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMin-Result=OK\n");
					else                                                               strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMin-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].fDiff_Dist_MaxMin) + "\n");
				}
			}
			else {
				strTemp += "Enable=false\n";
			}
			strData += (strTemp + "\n");

			strTemp = "[ThermalGlue_GlueToGlue]\n";
			if (sResult.sThermalGlue.sGlueToGlue.bEnable) {
				strTemp += "Enable=true\n";
				strTemp += ("Count=" + to_string(sResult.sThermalGlue.sGlueToGlue.nRoiCount) + "\n");
				for (int k = 0; k < sResult.sThermalGlue.sGlueToGlue.nRoiCount; ++k) {
					// Max
					if (sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].bResult_MaxDist) strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=OK\n");
					else                                                              strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MaxDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].sMaxDist.fDistance) + "\n");

					// Min
					if (sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].bResult_MinDist) strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=OK\n");
					else                                                              strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].sMinDist.fDistance) + "\n");

					// Diff
					if (sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].bResult_Diff_MaxMin) strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMin-Result=OK\n");
					else                                                               strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMin-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].fDiff_Dist_MaxMin) + "\n");
				}
			}
			else {
				strTemp += "Enable=false\n";
			}
			strData += (strTemp + "\n");

			strTemp = "[ThermalGlue_GlueToDie]\n";
			if (sResult.sThermalGlue.sGlueToDie.bEnable) {
				strTemp += "Enable=true\n";
				strTemp += ("Count=" + to_string(sResult.sThermalGlue.sGlueToDie.nRoiCount) + "\n");
				for (int k = 0; k < sResult.sThermalGlue.sGlueToDie.nRoiCount; ++k) {
					// Max
					if (sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].bResult_MaxDist) strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=OK\n");
					else                                                              strTemp += ("[" + to_string(k + 1) + "]-MaxDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MaxDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].sMaxDist.fDistance) + "\n");

					// Min
					if (sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].bResult_MinDist) strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=OK\n");
					else                                                              strTemp += ("[" + to_string(k + 1) + "]-MinDist-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-MinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].sMinDist.fDistance) + "\n");

					// Diff
					if (sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].bResult_Diff_MaxMin) strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMin-Result=OK\n");
					else                                                               strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMin-Result=NG\n");
					strTemp += ("[" + to_string(k + 1) + "]-Diff_MaxMinDist=" + jet_imagefunction::FloatingToString("%.5f", sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].fDiff_Dist_MaxMin) + "\n");
				}
			}
			else {
				strTemp += "Enable=false\n";
			}
			strData += (strTemp + "\n");

			SaveFile.write((char*)strData.c_str(), strData.length());
			SaveFile.close();

			return true;
		}

		// 儲存膠高量測結果
		bool MeasurementBlackGlue::Save_GlueHeightResult_Txt(const string& strPathName, const SMeasurementGlueHeightRatio_Result& sResult)
		{
			//if (strPathName.empty()) {
			//	return false;
			//}

			//fstream SaveFile;
			//SaveFile.open(strPathName + ".txt", ios::out | ios::trunc); //寫檔, 清除內容

			//string strData, strTemp;

			//strData = "[Result]\n";

			//strTemp += ("IC Height=" + jet_imagefunction::FloatingToString("%.5f", sResult..sWidth_Horizontal.vtsRoiInfo[k].sMaxDist.fDistance) + "\n");

			return true;
		}

		// 設定存圖的路徑與檔名(不需要副檔名)
		// [In] strPathName	: 會將 Measurement()過程影像 儲存到 strPathName所指定的路徑與檔名(資料夾須先建立)
		bool MeasurementBlackGlue::SetSavePathName(const string& strPathName)
		{
			if (strPathName.empty()) {
				return false;
			}
			m_strSavePathName = strPathName;
			return true;
		}

		// 設定儲存影像的副檔名
		// [In] nType : 1=>jpg ; 2=>bmp ; 3=>png
		bool MeasurementBlackGlue::SetImageExtension(const int& nType)
		{
			if (nType < 1 || nType>3) return false;
			m_nExtensionType = nType;
			return true;
		}
#pragma endregion

#pragma region Report

		// 輸出黑膠的報表
		// [In] sParam : 黑膠量測的參數, 
		// [In] sResult : 黑膠量測的結果
		// [Out] vtstrReport : 輸出的報表
		// [In] nMode : 1=>有量測的全部輸出, 2=>只輸出OK的, 3=>只輸出NG的
		// [In] nSymbolType : 符號樣式, 1=>" , "(逗號2邊都有空白)  ; 2=", "(逗號右邊有空白) ; 3=>","(逗號2側都沒有空白)	
		bool MeasurementBlackGlue::ReportTxt_BlackGlue(const SMeasurementBlackGlue_Parameter& sParam, const SMeasurementBlackGlue_Result& sResult, std::vector<std::string>& vtstrReport, const int& nMode, const int& nSymbolType)
		{
			vtstrReport.clear();
			if (nMode < 1 || nMode > 3 || nSymbolType < 1 || nSymbolType > 3)
				return false;

			// 報表分隔符號
			static const std::string symbolArr[3] = { " , ", ", ", "," };
			std::string strSymbol = symbolArr[nSymbolType - 1];

			// 項目名稱
			static const std::vector<std::string> vtstrItem = {
				"Glue Width Horizontal", "Glue Width Vertical", "Glue Width Arc", "Start End",
				"BoardToGlue Horizontal", "BoardToGlue Vertical",
				"CoatingToGlue Horizontal", "CoatingToGlue Vertical",
				"CoatingToBoard Horizontal", "CoatingToBoard Vertical",
				"Thermal GlueWidth", "Thermal GlueToGlue", "Thermal GlueToDie"
			};
			static const std::vector<std::string> vtstrSubName = { "Max", "Min" , "Max-Min" };
			const int nDecimalPlaces_Value = 1;
			const int nDecimalPlaces_Limit = 1;
			vtstrReport.reserve(100);

			// --- ROI 類
			if (sResult.sWidth_Horizontal.bEnable)
				AppendROIReport(vtstrReport, vtstrItem[0], vtstrSubName, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sWidth_Horizontal, sParam.sGlueWidth_Horizontal);

			if (sResult.sWidth_Vertical.bEnable)
				AppendROIReport(vtstrReport, vtstrItem[1], vtstrSubName, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sWidth_Vertical, sParam.sGlueWidth_Vertical);

			if (sResult.sWidth_Arc.bEnable)
				AppendROIReport(vtstrReport, vtstrItem[2], vtstrSubName, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sWidth_Arc, sParam.sGlueArc);

			// --- 首尾距離
			if (sResult.sStartEnd.bEnable)
				AppendStartEndROIReport(vtstrReport, vtstrItem[3], strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sStartEnd, sParam.sGlueStartEnd);

			// --- 其他各種 ROI
			if (sResult.sBoardToGlue_Horizontal.bEnable)
				AppendROIReport(vtstrReport, vtstrItem[4], vtstrSubName, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sBoardToGlue_Horizontal, sParam.sBoardToGlue_Horizontal);

			if (sResult.sBoardToGlue_Vertical.bEnable)
				AppendROIReport(vtstrReport, vtstrItem[5], vtstrSubName, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sBoardToGlue_Vertical, sParam.sBoardToGlue_Vertical);

			// coating-to-glue/board 只顯示 Min
			if (sResult.sCoatingToGlue_Horizontal.bEnable)
				AppendROIReport(vtstrReport, vtstrItem[6], { "Min" }, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sCoatingToGlue_Horizontal, sParam.sCoatingToGlue_Horizontal);

			if (sResult.sCoatingToGlue_Vertical.bEnable)
				AppendROIReport(vtstrReport, vtstrItem[7], { "Min" }, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sCoatingToGlue_Vertical, sParam.sCoatingToGlue_Vertical);

			if (sResult.sCoatingToBoard_Horizontal.bEnable)
				AppendROIReport(vtstrReport, vtstrItem[8], { "Min" }, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sCoatingToBoard_Horizontal, sParam.sCoatingToBoard_Horizontal);

			if (sResult.sCoatingToBoard_Vertical.bEnable)
				AppendROIReport(vtstrReport, vtstrItem[9], { "Min" }, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sCoatingToBoard_Vertical, sParam.sCoatingToBoard_Vertical);
			
			// 散熱膠-膠寬
			const SMultipleROI_Result* pResult_ROI = &sResult.sThermalGlue.sGlueWidth;
			const vector<SLimit_ROI>* psLimit = &sParam.sThermal.vtsLimit_Width;
			if (pResult_ROI->bEnable) {
				if (sParam.sThermal.nGlue_Count == psLimit->size() && sParam.sThermal.nGlue_Count == pResult_ROI->vtsRoiInfo.size() && sParam.sThermal.nGlue_Count == pResult_ROI->nRoiCount) {
					AppendThermalReport(vtstrReport, vtstrItem[10], vtstrSubName, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, *pResult_ROI, *psLimit);
				}
				else {
					vtstrReport.emplace_back(vtstrItem[10] + strSymbol + "Data Error");
				}
			}

			// 散熱膠-間距
			pResult_ROI = &sResult.sThermalGlue.sGlueToGlue;
			psLimit = &sParam.sThermal.vtsLimit_GlueToGlue;
			if (pResult_ROI->bEnable) {
				if (sParam.sThermal.nGlue_Count - 1 == psLimit->size() && sParam.sThermal.nGlue_Count - 1 == pResult_ROI->vtsRoiInfo.size() && sParam.sThermal.nGlue_Count - 1 == pResult_ROI->nRoiCount) {
					AppendThermalReport(vtstrReport, vtstrItem[11], vtstrSubName, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, *pResult_ROI, *psLimit);
				}
				else {
					vtstrReport.emplace_back(vtstrItem[11] + strSymbol + "Data Error");
				}
			}

			// 散熱膠到IC邊的距離
			pResult_ROI = &sResult.sThermalGlue.sGlueToDie;
			psLimit = &sParam.sThermal.vtsLimit_GlueToDie;
			if (pResult_ROI->bEnable) {
				if (psLimit->size() == 2 && pResult_ROI->vtsRoiInfo.size() == 2 && pResult_ROI->nRoiCount == 2) {
					AppendThermalReport(vtstrReport, vtstrItem[12], vtstrSubName, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, *pResult_ROI, *psLimit);
				}
				else {
					vtstrReport.emplace_back(vtstrItem[12] + strSymbol + "Data Error");
				}
			}

			return true;
		}

		// ROI報表產生器
		void MeasurementBlackGlue::AppendROIReport(vector<string>& vtstrReport, const string& itemName, const vector<string>& subNames, const string& strSymbol, const int nMode, const int nDecimalPlaces_Value, const int nDecimalPlaces_Limit, const SMultipleROI_Result& result, const SMultipleROI_Parameter& param)
		{
			int nRoiCount = result.nRoiCount;
			// 防呆
			if (param.nCount != nRoiCount || param.nCount != param.vtsLimit.size() || nRoiCount != result.vtsRoiInfo.size())
			{
				vtstrReport.emplace_back(itemName + strSymbol + "Data Error");
				return;
			}

			int nCount = subNames.size();
			for (int k = 0; k < nRoiCount; ++k) {
				for (int h = 0; h < nCount; ++h) {
					const SSingleROI_Result& roiRes = result.vtsRoiInfo[k];
					const SLimit_ROI& limit = param.vtsLimit[k];
					float fDistance = 0.0, fUpper=0.0, fLower=0.0;
					bool bResult = false;
					switch (h) {
					case 0:
						fDistance = roiRes.sMaxDist.fDistance;
						fUpper = limit.sMaxDist.fUpper;
						fLower = limit.sMaxDist.fLower;
						bResult = roiRes.bResult_MaxDist;
						break;
					case 1:
						fDistance = roiRes.sMinDist.fDistance;
						fUpper = limit.sMinDist.fUpper;
						fLower = limit.sMinDist.fLower;
						bResult = roiRes.bResult_MinDist;
						break;
					case 2:
						fDistance = roiRes.fDiff_Dist_MaxMin;
						fUpper = limit.sDiff_MaxMin.fUpper;
						fLower = limit.sDiff_MaxMin.fLower;
						bResult = roiRes.bResult_Diff_MaxMin;
						break;
					}

					switch (nMode) {
					case 1:
						vtstrReport.emplace_back(ReportTxt_Single(itemName, k, subNames[h], strSymbol, fDistance, fUpper, fLower, nDecimalPlaces_Value, nDecimalPlaces_Limit));
						break;
					case 2:
						if (bResult)
							vtstrReport.emplace_back(ReportTxt_Single(itemName, k, subNames[h], strSymbol, fDistance, fUpper, fLower, nDecimalPlaces_Value, nDecimalPlaces_Limit));
						break;
					case 3:
						if (!bResult)
							vtstrReport.emplace_back(ReportTxt_Single(itemName, k, subNames[h], strSymbol, fDistance, fUpper, fLower, nDecimalPlaces_Value, nDecimalPlaces_Limit));
						break;
					}
				}
			}
		}

		// StartEnd報表產生器
		void MeasurementBlackGlue::AppendStartEndROIReport(vector<string>& vtstrReport, const string& itemName, const string& strSymbol, const int nMode, const int nDecimalPlaces_Value, const int nDecimalPlaces_Limit, const SStartEndROI_Result& result, const SStartEndROI_Parameter& param)
		{
			int nCount = result.nCount;
			if (param.nCount != nCount || nCount != result.vtsStartEnd.size() || nCount != param.vtsStartEnd.size()) {
				vtstrReport.emplace_back(itemName + strSymbol + "Data Error");
				return;
			}
			for (int k = 0; k < nCount; ++k) {
				const SSingleStartEndROI_Result& roiRes = result.vtsStartEnd[k];
				const SStartEndROI_Single& limit = param.vtsStartEnd[k];

				// bResult_MaxDist 判斷 OK/NG
				bool bOK = roiRes.bResult_MaxDist;
				if (nMode == 1 || (nMode == 2 && bOK) || (nMode == 3 && !bOK)) {
					vtstrReport.emplace_back(
						ReportTxt_Single(
							itemName, k, "", strSymbol,
							roiRes.sDist.fDistance,
							limit.sLimit.fUpper,
							limit.sLimit.fLower,
							nDecimalPlaces_Value, nDecimalPlaces_Limit
						)
					);
				}
			}
		}

		// Thermal報表產生器
		void MeasurementBlackGlue::AppendThermalReport(std::vector<std::string>& vtstrReport, const std::string& itemName, const vector<string>& subNames, const std::string& strSymbol, const int nMode, const int nDecimalPlaces_Value, const int nDecimalPlaces_Limit, const SMultipleROI_Result& result, const vector<SLimit_ROI>& param)
		{
			for (int k = 0; k < result.nRoiCount; ++k) {
				for (int h = 0; h < (int)subNames.size(); ++h) {
					// 讀取各自欄位
					const SSingleROI_Result& roiRes = result.vtsRoiInfo[k];
					const SLimit_ROI& limit = param[k];

					float fDistance = 0.0, fUpper = 0.0, fLower = 0.0;
					bool bResult = false;
					switch (h) {
					case 0:
						fDistance = roiRes.sMaxDist.fDistance;
						fUpper = limit.sMaxDist.fUpper;
						fLower = limit.sMaxDist.fLower;
						bResult = roiRes.bResult_MaxDist;
						break;
					case 1:
						fDistance = roiRes.sMinDist.fDistance;
						fUpper = limit.sMinDist.fUpper;
						fLower = limit.sMinDist.fLower;
						bResult = roiRes.bResult_MinDist;
						break;
					case 2:
						fDistance = roiRes.fDiff_Dist_MaxMin;
						fUpper = limit.sDiff_MaxMin.fUpper;
						fLower = limit.sDiff_MaxMin.fLower;
						bResult = roiRes.bResult_Diff_MaxMin;
						break;
					}

					switch (nMode) {
					case 1:
						vtstrReport.emplace_back(ReportTxt_Single(itemName, k, subNames[h], strSymbol, fDistance, fUpper, fLower, nDecimalPlaces_Value, nDecimalPlaces_Limit));
						break;
					case 2:
						if (bResult)
							vtstrReport.emplace_back(ReportTxt_Single(itemName, k, subNames[h], strSymbol, fDistance, fUpper, fLower, nDecimalPlaces_Value, nDecimalPlaces_Limit));
						break;
					case 3:
						if (!bResult)
							vtstrReport.emplace_back(ReportTxt_Single(itemName, k, subNames[h], strSymbol, fDistance, fUpper, fLower, nDecimalPlaces_Value, nDecimalPlaces_Limit));
						break;
					}
				}
			}
		}

		// 輸出黑膠的報表與對應的顏色
		// [In] sParam : 黑膠量測的參數, 
		// [In] sResult : 黑膠量測的結果
		// [Out] vtstrReport : 輸出的報表
		// [Out] vtscColor : 報表對應的顏色
		// [In] nMode : 1=>有量測的全部輸出, 2=>只輸出OK的, 3=>只輸出NG的
		// [In] nSymbolType : 符號樣式, 1=>" , "(逗號2邊都有空白)  ; 2=", "(逗號右邊有空白) ; 3=>","(逗號2側都沒有空白)	
		bool MeasurementBlackGlue::ReportTxt_BlackGlue_Color(const SMeasurementBlackGlue_Parameter& sParam, const SMeasurementBlackGlue_Result& sResult, vector<string>& vtstrReport, vector<Scalar>& vtscrReportColor, const int& nMode, const int& nSymbolType)
		{
			vtscrReportColor.clear();
			vtstrReport.clear();
			if (nMode < 1 || nMode > 3 || nSymbolType < 1 || nSymbolType > 3)
				return false;

			// 報表分隔符號
			static const std::string symbolArr[3] = { " , ", ", ", "," };
			std::string strSymbol = symbolArr[nSymbolType - 1];

			// 項目名稱
			static const std::vector<std::string> vtstrItem = {
				"Glue Width Horizontal", "Glue Width Vertical", "Glue Width Arc", "Start End",
				"BoardToGlue Horizontal", "BoardToGlue Vertical",
				"CoatingToGlue Horizontal", "CoatingToGlue Vertical",
				"CoatingToBoard Horizontal", "CoatingToBoard Vertical",
				"Thermal GlueWidth", "Thermal GlueToGlue", "Thermal GlueToDie"
			};
			static const std::vector<std::string> vtstrSubName = { "Max", "Min" , "Diff-MaxMin" };
			const int nDecimalPlaces_Value = 1;
			const int nDecimalPlaces_Limit = 1;
			vtscrReportColor.reserve(100);
			vtstrReport.reserve(100);

			// --- ROI 類
			if (sResult.sWidth_Horizontal.bEnable) {
				vector<Scalar> vtscritemColor(3);
				vtscritemColor[0] = m_scMaxDist_GlueWidth;
				vtscritemColor[1] = m_scMinDist_GlueWidth;
				vtscritemColor[2] = m_scMaxDist_GlueWidth*0.5 + m_scMinDist_GlueWidth*0.5;
				AppendROIReport_Color(vtstrReport, vtscrReportColor, vtstrItem[0], vtscritemColor, vtstrSubName, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sWidth_Horizontal, sParam.sGlueWidth_Horizontal);
			}

			if (sResult.sWidth_Vertical.bEnable) {
				vector<Scalar> vtscritemColor(3);
				vtscritemColor[0] = m_scMaxDist_GlueWidth;
				vtscritemColor[1] = m_scMinDist_GlueWidth;
				vtscritemColor[2] = m_scMaxDist_GlueWidth*0.5 + m_scMinDist_GlueWidth*0.5;
				AppendROIReport_Color(vtstrReport, vtscrReportColor, vtstrItem[1], vtscritemColor, vtstrSubName, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sWidth_Vertical, sParam.sGlueWidth_Vertical);
			}

			if (sResult.sWidth_Arc.bEnable) {
				vector<Scalar> vtscritemColor(3);
				vtscritemColor[0] = m_scMaxDist_GlueArc;
				vtscritemColor[1] = m_scMinDist_GlueArc;
				vtscritemColor[2] = m_scMaxDist_GlueArc*0.5 + m_scMinDist_GlueArc*0.5;
				AppendROIReport_Color(vtstrReport, vtscrReportColor, vtstrItem[2], vtscritemColor, vtstrSubName, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sWidth_Arc, sParam.sGlueArc);
			}

			// --- 首尾距離
			if (sResult.sStartEnd.bEnable) {
				size_t nN1 = vtstrReport.size();
				AppendStartEndROIReport(vtstrReport, vtstrItem[3], strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sStartEnd, sParam.sGlueStartEnd);
				size_t nN2 = vtstrReport.size();
				if (nN2 > nN1) {
					vtscrReportColor.emplace_back(m_scMaxDist_GlueArc);
				}
			}

			// Board To Glue Horizontal
			if (sResult.sBoardToGlue_Horizontal.bEnable) {
				vector<Scalar> vtscritemColor(3);
				vtscritemColor[0] = m_scMaxDist_BoardToGlue;
				vtscritemColor[1] = m_scMinDist_BoardToGlue;
				vtscritemColor[2] = m_scMaxDist_BoardToGlue*0.5 + m_scMinDist_BoardToGlue*0.5;
				AppendROIReport_Color(vtstrReport, vtscrReportColor, vtstrItem[4], vtscritemColor, vtstrSubName, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sBoardToGlue_Horizontal, sParam.sBoardToGlue_Horizontal);
			}

			// Board To Glue Vertical
			if (sResult.sBoardToGlue_Vertical.bEnable) {
				vector<Scalar> vtscritemColor(3);
				vtscritemColor[0] = m_scMaxDist_BoardToGlue;
				vtscritemColor[1] = m_scMinDist_BoardToGlue;
				vtscritemColor[2] = m_scMaxDist_BoardToGlue*0.5 + m_scMinDist_BoardToGlue*0.5;
				AppendROIReport_Color(vtstrReport, vtscrReportColor, vtstrItem[5], vtscritemColor, vtstrSubName, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sBoardToGlue_Vertical, sParam.sBoardToGlue_Vertical);
			}

			// Coating To Glue Horizontal
			if (sResult.sCoatingToGlue_Horizontal.bEnable) {
				size_t nN1 = vtstrReport.size();
				AppendROIReport(vtstrReport, vtstrItem[6], { "Min" }, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sCoatingToGlue_Horizontal, sParam.sCoatingToGlue_Horizontal);
				size_t nN2 = vtstrReport.size();
				if (nN2 > nN1) {
					vtscrReportColor.emplace_back(m_scMinDist_BoardToGlue);
				}
			}

			// Coating To Glue Vertical
			if (sResult.sCoatingToGlue_Vertical.bEnable) {
				size_t nN1 = vtstrReport.size();
				AppendROIReport(vtstrReport, vtstrItem[7], { "Min" }, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sCoatingToGlue_Vertical, sParam.sCoatingToGlue_Vertical);
				size_t nN2 = vtstrReport.size();
				if (nN2 > nN1) {
					vtscrReportColor.emplace_back(m_scMinDist_BoardToGlue);
				}
			}

			if (sResult.sCoatingToBoard_Horizontal.bEnable) {
				size_t nN1 = vtstrReport.size();
				AppendROIReport(vtstrReport, vtstrItem[8], { "Min" }, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sCoatingToBoard_Horizontal, sParam.sCoatingToBoard_Horizontal);
				size_t nN2 = vtstrReport.size();
				if (nN2 > nN1) {
					vtscrReportColor.emplace_back(m_scMinDist_BoardToGlue);
				}
			}

			if (sResult.sCoatingToBoard_Vertical.bEnable) {
				size_t nN1 = vtstrReport.size();
				AppendROIReport(vtstrReport, vtstrItem[9], { "Min" }, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, sResult.sCoatingToBoard_Vertical, sParam.sCoatingToBoard_Vertical);
				size_t nN2 = vtstrReport.size();
				if (nN2 > nN1) {
					vtscrReportColor.emplace_back(m_scMinDist_BoardToGlue);
				}
			}

			// 散熱膠-膠寬
			const SMultipleROI_Result* pResult_ROI = &sResult.sThermalGlue.sGlueWidth;
			const vector<SLimit_ROI>* psLimit = &sParam.sThermal.vtsLimit_Width;
			if (pResult_ROI->bEnable) {
				if (sParam.sThermal.nGlue_Count == psLimit->size() && sParam.sThermal.nGlue_Count == pResult_ROI->vtsRoiInfo.size() && sParam.sThermal.nGlue_Count == pResult_ROI->nRoiCount) {
					vector<Scalar> vtscritemColor(3);
					vtscritemColor[0] = m_scMaxDist_BoardToGlue;
					vtscritemColor[1] = m_scMinDist_BoardToGlue;
					vtscritemColor[2] = m_scMaxDist_BoardToGlue*0.5 + m_scMinDist_BoardToGlue*0.5;
					AppendThermalReport_Color(vtstrReport, vtscrReportColor, vtstrItem[10], vtscritemColor, vtstrSubName, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, *pResult_ROI, *psLimit);
				}
				else {
					vtstrReport.emplace_back(vtstrItem[10] + strSymbol + "Data Error");
				}
			}

			// 散熱膠-間距
			pResult_ROI = &sResult.sThermalGlue.sGlueToGlue;
			psLimit = &sParam.sThermal.vtsLimit_GlueToGlue;
			if (pResult_ROI->bEnable) {
				if (sParam.sThermal.nGlue_Count - 1 == psLimit->size() && sParam.sThermal.nGlue_Count - 1 == pResult_ROI->vtsRoiInfo.size() && sParam.sThermal.nGlue_Count - 1 == pResult_ROI->nRoiCount) {
					vector<Scalar> vtscritemColor(3);
					vtscritemColor[0] = m_scMaxDist_GlueWidth;
					vtscritemColor[1] = m_scMinDist_GlueWidth;
					vtscritemColor[2] = m_scMaxDist_GlueWidth*0.5 + m_scMinDist_GlueWidth*0.5;
					AppendThermalReport_Color(vtstrReport, vtscrReportColor, vtstrItem[11], vtscritemColor, vtstrSubName, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, *pResult_ROI, *psLimit);
				}
				else {
					vtstrReport.emplace_back(vtstrItem[11] + strSymbol + "Data Error");
				}
			}

			// 散熱膠到IC邊的距離
			pResult_ROI = &sResult.sThermalGlue.sGlueToDie;
			psLimit = &sParam.sThermal.vtsLimit_GlueToDie;
			if (pResult_ROI->bEnable) {
				if (psLimit->size() == 2 && pResult_ROI->vtsRoiInfo.size() == 2 && pResult_ROI->nRoiCount == 2) {
					vector<Scalar> vtscritemColor(3);
					vtscritemColor[0] = m_scMaxDist_GlueArc;
					vtscritemColor[1] = m_scMinDist_GlueArc;
					vtscritemColor[2] = m_scMaxDist_GlueArc*0.5 + m_scMinDist_GlueArc*0.5;
					AppendThermalReport_Color(vtstrReport, vtscrReportColor, vtstrItem[12], vtscritemColor, vtstrSubName, strSymbol, nMode, nDecimalPlaces_Value, nDecimalPlaces_Limit, *pResult_ROI, *psLimit);
				}
				else {
					vtstrReport.emplace_back(vtstrItem[12] + strSymbol + "Data Error");
				}
			}

			return true;
		}

		void MeasurementBlackGlue::AppendROIReport_Color(vector<string>& vtstrReport, vector<Scalar>& vtscrReportColor, const string& itemName, const vector<Scalar>& vtscritemColor, const vector<string>& subNames, const string& strSymbol, const int nMode, const int nDecimalPlaces_Value, const int nDecimalPlaces_Limit, const SMultipleROI_Result& result, const SMultipleROI_Parameter& param)
		{
			int nRoiCount = result.nRoiCount;
			// 防呆
			if (param.nCount != nRoiCount || param.nCount != param.vtsLimit.size() || nRoiCount != result.vtsRoiInfo.size())
			{
				vtstrReport.emplace_back(itemName + strSymbol + "Data Error");
				return;
			}

			int nColorIndex = -1;
			int nCount = subNames.size();
			for (int k = 0; k < nRoiCount; ++k) {
				for (int h = 0; h < nCount; ++h) {
					const SSingleROI_Result& roiRes = result.vtsRoiInfo[k];
					const SLimit_ROI& limit = param.vtsLimit[k];
					float fDistance = 0.0, fUpper = 0.0, fLower = 0.0;
					bool bResult = false;
					switch (h) {
					case 0:
						fDistance = roiRes.sMaxDist.fDistance;
						fUpper = limit.sMaxDist.fUpper;
						fLower = limit.sMaxDist.fLower;
						bResult = roiRes.bResult_MaxDist;
						nColorIndex = 0;
						break;
					case 1:
						fDistance = roiRes.sMinDist.fDistance;
						fUpper = limit.sMinDist.fUpper;
						fLower = limit.sMinDist.fLower;
						bResult = roiRes.bResult_MinDist;
						nColorIndex = 1;
						break;
					case 2:
						fDistance = roiRes.fDiff_Dist_MaxMin;
						fUpper = limit.sDiff_MaxMin.fUpper;
						fLower = limit.sDiff_MaxMin.fLower;
						bResult = roiRes.bResult_Diff_MaxMin;
						nColorIndex = 2;
						break;
					}
					if (nColorIndex == -1) return;

					switch (nMode) {
					case 1:
						vtstrReport.emplace_back(ReportTxt_Single(itemName, k, subNames[h], strSymbol, fDistance, fUpper, fLower, nDecimalPlaces_Value, nDecimalPlaces_Limit));
						vtscrReportColor.emplace_back(vtscritemColor[nColorIndex]);
						break;
					case 2:
						if (bResult) {
							vtstrReport.emplace_back(ReportTxt_Single(itemName, k, subNames[h], strSymbol, fDistance, fUpper, fLower, nDecimalPlaces_Value, nDecimalPlaces_Limit));
							vtscrReportColor.emplace_back(vtscritemColor[nColorIndex]);
						}
						break;
					case 3:
						if (!bResult) {
							vtstrReport.emplace_back(ReportTxt_Single(itemName, k, subNames[h], strSymbol, fDistance, fUpper, fLower, nDecimalPlaces_Value, nDecimalPlaces_Limit));
							vtscrReportColor.emplace_back(vtscritemColor[nColorIndex]);
						}
						break;
					}
				}
			}
		}
		void MeasurementBlackGlue::AppendThermalReport_Color(std::vector<std::string>& vtstrReport, vector<Scalar>& vtscrReportColor, const std::string& itemName, const vector<Scalar>& vtscritemColor, const vector<string>& subNames, const std::string& strSymbol, const int nMode, const int nDecimalPlaces_Value, const int nDecimalPlaces_Limit, const SMultipleROI_Result& result, const vector<SLimit_ROI>& param)
		{
			for (int k = 0; k < result.nRoiCount; ++k) {
				for (int h = 0; h < (int)subNames.size(); ++h) {
					// 讀取各自欄位
					const SSingleROI_Result& roiRes = result.vtsRoiInfo[k];
					const SLimit_ROI& limit = param[k];

					int nColorIndex = -1;
					float fDistance = 0.0, fUpper = 0.0, fLower = 0.0;
					bool bResult = false;
					switch (h) {
					case 0:
						fDistance = roiRes.sMaxDist.fDistance;
						fUpper = limit.sMaxDist.fUpper;
						fLower = limit.sMaxDist.fLower;
						bResult = roiRes.bResult_MaxDist;
						nColorIndex = 0;
						break;
					case 1:
						fDistance = roiRes.sMinDist.fDistance;
						fUpper = limit.sMinDist.fUpper;
						fLower = limit.sMinDist.fLower;
						bResult = roiRes.bResult_MinDist;
						nColorIndex = 1;
						break;
					case 2:
						fDistance = roiRes.fDiff_Dist_MaxMin;
						fUpper = limit.sDiff_MaxMin.fUpper;
						fLower = limit.sDiff_MaxMin.fLower;
						bResult = roiRes.bResult_Diff_MaxMin;
						nColorIndex = 2;
						break;
					}
					if (nColorIndex == -1) return;

					switch (nMode) {
					case 1:
						vtstrReport.emplace_back(ReportTxt_Single(itemName, k, subNames[h], strSymbol, fDistance, fUpper, fLower, nDecimalPlaces_Value, nDecimalPlaces_Limit));
						vtscrReportColor.emplace_back(vtscritemColor[nColorIndex]);
						break;
					case 2:
						if (bResult) {
							vtstrReport.emplace_back(ReportTxt_Single(itemName, k, subNames[h], strSymbol, fDistance, fUpper, fLower, nDecimalPlaces_Value, nDecimalPlaces_Limit));
							vtscrReportColor.emplace_back(vtscritemColor[nColorIndex]);
						}
						break;
					case 3:
						if (!bResult) {
							vtstrReport.emplace_back(ReportTxt_Single(itemName, k, subNames[h], strSymbol, fDistance, fUpper, fLower, nDecimalPlaces_Value, nDecimalPlaces_Limit));
							vtscrReportColor.emplace_back(vtscritemColor[nColorIndex]);
						}
						break;
					}
				}
			}
		}

		// 輸出一行
		// nDecimalPlaces_Value : fValue 顯示的小數位數
		// nDecimalPlaces_Limit : fUpper與fLower 顯示的小數位數
		string MeasurementBlackGlue::ReportTxt_Single(const string& strItemName, const int& nIndex, const string& strSubName, const string& strSymbol, const float& fValue, const float& fUpper, const float& fLower, const int& nDecimalPlaces_Value, const int& nDecimalPlaces_Limit)
		{
			string strData = "";
			string strIndex = to_string(nIndex + 1);
			string strValue = jet_imagefunction::FloatingToString(1, nDecimalPlaces_Value, fValue);
			string strUpper = jet_imagefunction::FloatingToString(1, nDecimalPlaces_Limit, fUpper);
			string strLower = jet_imagefunction::FloatingToString(1, nDecimalPlaces_Limit, fLower);
			if (strSubName.empty()) {
				strData = strItemName + strIndex + strSymbol + strValue + strSymbol + strLower + strSymbol + strUpper;
			}
			else {
				strData = strItemName + strIndex + "-" + strSubName + strSymbol + strValue + strSymbol + strLower + strSymbol + strUpper;
			}
			return strData;
		}
#pragma endregion

		// 調整膠參數 相關功能
#pragma region Adjustment Glue Parameter

		// Method1 : 找膠本體(抽色)
#pragma region Method1

		// Method1-Step1 輸入膠的彩色影像進行抽色, 輸出膠的二值化影像
		// [In] matColor : 膠的彩色影像(彩色)
		// [In] nGlueValue_R, nGlueValue_G, nGlueValue_R : 黑膠的 R G B 值
		// [In] nGlueValueTolerance : 黑膠的 R G B 值的誤差範圍
		// [Out] matThreshold : 輸出 膠的二值化影像(灰階)
		bool MeasurementBlackGlue::AdjustmentGlueParameter_Method1_Step1_ColorToThreshold(const Mat& matColor, const int& nGlueValue_R, const int& nGlueValue_G, const int& nGlueValue_B, const int& nGlueValueTolerance, Mat& matThreshold)
		{
			m_strErrorMessage = "OK";
			if ( matColor.empty() || matColor.channels() != 3) {
				m_strErrorMessage = "請輸入有效的彩色影像";
				return false;
			}

			if (nGlueValue_R < 0 || nGlueValue_R>255 || nGlueValue_G < 0 || nGlueValue_G>255 || nGlueValue_B < 0 || nGlueValue_B>255) {
				m_strErrorMessage = "膠的 RGB 值輸入錯誤, 請輸入0-255";
				return false;
			}

			if (nGlueValueTolerance < 0 || nGlueValueTolerance>255) {
				m_strErrorMessage = "膠 RGB的寬放值輸入錯誤, 請輸入0-255";
				return false;
			}

			SGlueEdge_Parameter sParam;
			sParam.nFindEdgeMethod = 1;
			sParam.nGlueValue_B = nGlueValue_B;
			sParam.nGlueValue_G = nGlueValue_G;
			sParam.nGlueValue_R = nGlueValue_R;
			sParam.nGlueValueTolerance = nGlueValueTolerance;

			SProcessModeParam sPM_All;
			if (!SetGlueProcess_FindContours(sParam, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Threshold;
			if (!sPM_All.ExtractList(1, 1, sPM_Threshold)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matColor;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Threshold, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentGlueParameter", "Method1_Step1", sPM_Threshold, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Threshold.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Threshold.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確";
				return false;
			}

			matThreshold = vtsResult[nCount - 1].matImage.clone();
			if (matThreshold.empty() || matThreshold.channels() != 1 || matColor.size() != matThreshold.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確";
				return false;
			}

			return true;
		}

		// Method1-Step2 輸入膠的二值化影像與濾波size, 輸出連接二值化影像邊緣的影像
		// [In] matThreshold : 膠的二值化影像(灰階)
		// [In] nOpenX : X方向濾波尺寸,不可小於1,最好是奇數(單位 pixels)
		// [In] nOpenY : Y方向濾波尺寸,不可小於1,最好是奇數(單位 pixels)
		// [Out] matMedian : 雜訊過濾後的二值化影像(灰階)
		bool MeasurementBlackGlue::AdjustmentGlueParameter_Method1_Step2_Open(const Mat& matThreshold, const int& nOpenX, const int& nOpenY, Mat& matMedian)
		{
			m_strErrorMessage = "OK";
			if (matThreshold.empty() || matThreshold.channels() != 1) {
				m_strErrorMessage = "請輸入有效的灰階影像";
				return false;
			}

			if (nOpenX < 1 || nOpenY < 1) {
				m_strErrorMessage = "Median濾波大小不可小於1";
				return false;
			}

			SGlueEdge_Parameter sParam;
			sParam.nFindEdgeMethod = 1;
			sParam.nFilter_OpenX = nOpenX;
			sParam.nFilter_OpenY = nOpenY;

			SProcessModeParam sPM_All;
			if (!SetGlueProcess_FindContours(sParam, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Filter;
			if (!sPM_All.ExtractList(2, 2, sPM_Filter)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matThreshold;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Filter, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentGlueParameter", "Method1_Step2", sPM_Filter, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Filter.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Filter.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matMedian = vtsResult[nCount - 1].matImage.clone();
			if (matMedian.empty() || matMedian.channels() != 1 || matMedian.size() != matThreshold.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		// Method1-Step3 輸入雜訊過濾後的二值化影像像與濾波size, 輸出連接二值化影像邊緣的影像
		// [In] matMedian : 膠的二值化影像(灰階)
		// [In] nConnectX : X方向濾波尺寸,不可小於1,最好是奇數(單位 pixels)
		// [In] nConnectY : Y方向濾波尺寸,不可小於1,最好是奇數(單位 pixels)
		// [Out] matConnect : 膠的二值化影像的連接結果(灰階)
		bool MeasurementBlackGlue::AdjustmentGlueParameter_Method1_Step3_Connect(const Mat& matMedian, const int& nConnectX, const int& nConnectY, Mat& matConnect)
		{
			m_strErrorMessage = "OK";
			if (matMedian.empty() || matMedian.channels() != 1) {
				m_strErrorMessage = "請輸入有效的灰階影像";
				return false;
			}

			if (nConnectX < 1 || nConnectY < 1) {
				m_strErrorMessage = "Connect濾波大小不可小於1";
				return false;
			}

			SGlueEdge_Parameter sParam;
			sParam.nFindEdgeMethod = 1;
			sParam.nFilter_ConnectX = nConnectX;
			sParam.nFilter_ConnectY = nConnectY;

			SProcessModeParam sPM_All;
			if (!SetGlueProcess_FindContours(sParam, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Filter;
			if (!sPM_All.ExtractList(3, 3, sPM_Filter)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matMedian;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Filter, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentGlueParameter", "Method1_Step3", sPM_Filter, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Filter.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Filter.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matConnect = vtsResult[nCount - 1].matImage.clone();
			if (matConnect.empty() || matConnect.channels() != 1 || matConnect.size() != matMedian.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		// Method1-Step4 輸入連接邊緣後的二值化影像與板邊位置以及膠ROI, 輸出黑膠本體的二值化影像
		// [In] matConnect : 連接邊緣後的二值化(灰階)
		// [In] nLocation : 板邊在 IC的位置
		// [In] rectGlueROI : 黑膠在影像中的ROI座標
		// [Out] matGlue : 膠本體的二值化影像(灰階)
		bool MeasurementBlackGlue::AdjustmentGlueParameter_Method1_Step4_Position(const Mat& matConnect, const int& nLocation, const int& nOpenX2, const int& nOpenY2, const float& fGlueROI_Tolerance, const RECT& rectGlueROI, Mat& matGlue)
		{
			m_strErrorMessage = "OK";
			if (matConnect.empty() || matConnect.channels() != 1) {
				m_strErrorMessage = "請輸入有效的灰階影像";
				return false;
			}

			if (nLocation < 1 || nLocation > 10) {
				m_strErrorMessage = "Location輸入錯誤, 請輸入1-10";
				return false;
			}

			if (nOpenX2 < 1 || nOpenY2 < 1) {
				m_strErrorMessage = "Open濾波大小不可小於1";
				return false;
			}

			if (fGlueROI_Tolerance < 0.1 || fGlueROI_Tolerance >= 1.0) {
				m_strErrorMessage = "GlueROI_Tolerance輸入錯誤, 請輸入 0.1 - 1.0";
				return false;
			}

			if (!jet_imagefunction::Check_RECT(matConnect.rows, matConnect.cols, rectGlueROI)) {
				m_strErrorMessage = "GlueROI座標輸入錯誤";
				return false;
			}

			SGlueEdge_Parameter sParam;
			sParam.nFindEdgeMethod = 1;
			sParam.nLocation = nLocation;
			sParam.fGlueROI_Tolerance = fGlueROI_Tolerance;
			sParam.rectGlueROI = rectGlueROI;
			sParam.nFilter_OpenX2 = nOpenX2;
			sParam.nFilter_OpenY2 = nOpenY2;

			m_matGlue = matConnect;
			SProcessModeParam sPM_All;
			if (!SetGlueProcess_FindContours(sParam, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Result;
			if (!sPM_All.ExtractList(4, 10, sPM_Result)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			// 進行轉灰階流程
			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matConnect;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Result, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentGlueParameter", "Method1_Step4", sPM_Result, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Result.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK", "Method1_Step4", sPM_Result, vtmatImage, vtsResult);
			}

			int nCount = sPM_Result.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matGlue = vtsResult[nCount - 1].matImage.clone();
			if (matGlue.empty() || matGlue.channels() != 1 || matGlue.size() != matConnect.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		// Method1-Step5-End 膠本體的二值化影像與 膨脹 侵蝕濾波的大小, 膠輪廓影像
		// [In] matColor : 膠的彩色影像(彩色)
		// [In] matGlue : 膠本體的二值化影像(灰階)
		// [In] nDilateX nDilateY : 將膠本體的二值化影像進行膨脹
		// [In] nErosionX nErosionY : 將膠本體的二值化影像進行侵蝕
		// [Out] matEnd : 膠輪廓影像(彩色)
		bool MeasurementBlackGlue::AdjustmentGlueParameter_Method1_Step5_End(const Mat& matColor, const Mat& matGlue, const int& nLocation, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, Mat& matEnd)
		{
			m_strErrorMessage = "OK";
			if (matColor.empty() || matColor.channels() != 3) {
				m_strErrorMessage = "請輸入有效的彩色影像";
				return false;
			}

			if (matGlue.empty() || matGlue.channels() != 1) {
				m_strErrorMessage = "請輸入有效的灰階影像";
				return false;
			}

			if (nLocation < 1 || nLocation > 10) {
				m_strErrorMessage = "Location輸入錯誤, 請輸入1-10";
				return false;
			}

			if (nDilateX < 1 || nDilateY < 1) {
				m_strErrorMessage = "膨脹濾波大小不可小於1";
				return false;
			}

			if (nErosionX < 1 || nErosionY < 1) {
				m_strErrorMessage = "侵蝕濾波大小不可小於1";
				return false;
			}

			SGlueEdge_Parameter sParam;
			sParam.nFindEdgeMethod = 1;
			sParam.nFilter_DilateX = nDilateX;
			sParam.nFilter_DilateY = nDilateY;
			sParam.nFilter_ErosionX = nErosionX;
			sParam.nFilter_ErosionY = nErosionY;
			sParam.nLocation = nLocation;
			sParam.rectGlueROI.left = 1;
			sParam.rectGlueROI.right = 5;
			sParam.rectGlueROI.top = 1;
			sParam.rectGlueROI.bottom = 5;

			m_matGlue = matColor;
			SProcessModeParam sPM_All;
			if (!SetGlueProcess_FindContours(sParam, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Result;
			if (!sPM_All.ExtractList(11, sPM_All.GetBaseParameterNumber(), sPM_Result)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matGlue;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Result, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentGlueParameter", "Method1_Step5", sPM_Result, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Result.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK", "Method1_Step5", sPM_Result, vtmatImage, vtsResult);
			}

			int nCount = sPM_Result.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			SContoursParam* psContours = &vtsResult[nCount - 1].sContours;
			if (psContours == nullptr) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			vector<POINT> vtptContoursPos;
			if (!ContoursConvertVector(psContours, vtptContoursPos)) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matEnd = matColor.clone();
			if (matGlue.empty() || matGlue.channels() != 1 || matGlue.size() != matEnd.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			jet_imagefunction::DrawEdgePoint(matEnd, vtptContoursPos, m_scContours_Glue, 0);
			
			return true;
		}
#pragma endregion

		// Method2 : 找膠輪廓(轉灰階+二值化)
#pragma region Method2

		// Method2-Step1 輸入膠的彩色影像, 與中值濾波Size, 輸出過濾雜訊後的灰階影像
		// [In] matColor : 膠彩色影像(彩色)
		// [In] nMedianX nMedianY : 中值濾波大小
		// [Out] matGray : 輸出 膠灰階影像(灰階)
		bool MeasurementBlackGlue::AdjustmentGlueParameter_Method2_Step1_ColorToGray(const Mat& matColor, const int& nMedianX, const int& nMedianY, Mat& matGray)
		{
			m_strErrorMessage = "OK";
			if (matColor.empty() || matColor.channels() != 3) {
				m_strErrorMessage = "請輸入有效的彩色影像";
				return false;
			}

			if (nMedianX < 1 || nMedianY < 1) {
				m_strErrorMessage = "濾波大小不可小於1";
				return false;
			}

			SGlueEdge_Parameter sParam;
			sParam.nFindEdgeMethod = 2;
			sParam.nFilter_MedianX = nMedianX;
			sParam.nFilter_MedianY = nMedianY;
			SProcessModeParam sPM_All;
			if (!SetGlueProcess_FindContours(sParam, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_ToGray;
			if (!sPM_All.ExtractList(1, 3, sPM_ToGray)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}
			
			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matColor;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_ToGray, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentGlueParameter", "Method2_Step1", sPM_ToGray, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_ToGray.GetErrorMessage();
				return false;
			}

			int nCount = sPM_ToGray.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確";
				return false;
			}

			matGray = vtsResult[nCount - 1].matImage.clone();
			if (matGray.empty() || matGray.channels() != 1 || matColor.size() != matGray.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確";
				return false;
			}

			return true;
		}

		// Method2-Step2 膠的灰階影像二值化
		// [In] matGray : 膠的灰階影像(灰階)
		// [In] bDark : 為true=>選取[0 , nThreshold] ; 為false=>選取[nThreshold , 255]
		// [In] nThreshold : 二值化閥值, 值域 [0 , 255] 	
		// [Out] matThreshold : 膠的二值化影像(灰階)
		bool MeasurementBlackGlue::AdjustmentGlueParameter_Method2_Step2_Threshold(const Mat& matGray, const bool& bDark, const int& nThreshold, const int& nOpenX, const int& nOpenY, Mat& matThreshold)
		{
			m_strErrorMessage = "OK";
			if (matGray.empty() || matGray.channels() != 1) {
				m_strErrorMessage = "請輸入有效的灰階影像";
				return false;
			}

			if (nThreshold < 0 || nThreshold>255) {
				m_strErrorMessage = "二值化閥值輸入錯誤, 請輸入 0-255";
				return false;
			}

			if (nOpenX < 1 || nOpenY < 1) {
				m_strErrorMessage = "濾波大小不可小於1";
				return false;
			}

			SGlueEdge_Parameter sParam;
			sParam.nFindEdgeMethod = 2;
			sParam.bDark = bDark;
			sParam.nThreshold = nThreshold;
			sParam.nFilter_OpenX = nOpenX;
			sParam.nFilter_OpenY = nOpenY;

			SProcessModeParam sPM_All;
			if (!SetGlueProcess_FindContours(sParam, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Threshold;
			if (!sPM_All.ExtractList(4, 5, sPM_Threshold)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matGray;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Threshold, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentGlueParameter", "Method2_Step2", sPM_Threshold, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Threshold.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Threshold.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matThreshold = vtsResult[nCount - 1].matImage.clone();
			if (matThreshold.empty() || matThreshold.channels() != 1 || matThreshold.size() != matGray.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		// Method-Step3 膠的二值化影像連接
		// [In] matThreshold : 膠的二值化影像(灰階)
		// [Out] matConnect : 連接後的二值化影像(灰階)
		// [In] nConnectX : X方向連接尺寸,不可小於1,最好是奇數(單位 pixels)
		// [In] nConnectY : Y方向連接尺寸,不可小於1,最好是奇數(單位 pixels)
		bool MeasurementBlackGlue::AdjustmentGlueParameter_Method2_Step3_Connect(const Mat& matThreshold, const int& nConnectX, const int& nConnectY, Mat& matConnect)
		{
			m_strErrorMessage = "OK";
			if (matThreshold.empty() || matThreshold.channels() != 1) {
				m_strErrorMessage = "請輸入有效的灰階影像";
				return false;
			}

			if (nConnectX < 1 || nConnectY < 1) {
				m_strErrorMessage = "濾波大小不可小於1";
				return false;
			}

			SGlueEdge_Parameter sParam;
			sParam.nFindEdgeMethod = 2;
			sParam.nFilter_ConnectX = nConnectX;
			sParam.nFilter_ConnectY = nConnectY;

			SProcessModeParam sPM_All;
			if (!SetGlueProcess_FindContours(sParam, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Dilate;
			if (!sPM_All.ExtractList(6, 6, sPM_Dilate)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matThreshold;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Dilate, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentGlueParameter", "Method2_Step3", sPM_Dilate, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Dilate.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Dilate.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matConnect = vtsResult[nCount - 1].matImage.clone();
			if (matConnect.empty() || matConnect.channels() != 1 || matConnect.size() != matThreshold.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		// Method2-Step4 輸入連接邊緣後的二值化影像與板邊位置以及膠ROI的大小, 輸出黑膠本體的二值化影像
		// [In] matConnect : 連接邊緣後的二值化(灰階)
		// [In] nLocation : 板邊在 IC的位置
		// [In] fGlueROI_Tolerance : 膠ROI大小寬放比例
		// [In] rectGlueROI : 黑膠在影像中的ROI座標
		// [Out] matGlue : 膠本體的二值化影像(灰階)
		bool MeasurementBlackGlue::AdjustmentGlueParameter_Method2_Step4_Position(const Mat& matConnect, const int& nLocation, const int& nOpenX2, const int& nOpenY2, const float& fGlueROI_Tolerance, const RECT& rectGlueROI, Mat& matGlue)
		{
			m_strErrorMessage = "OK";
			if (matConnect.empty() || matConnect.channels() != 1) {
				m_strErrorMessage = "請輸入有效的灰階影像";
				return false;
			}

			if (nLocation < 1 || nLocation > 10) {
				m_strErrorMessage = "Location輸入錯誤, 請輸入1-10";
				return false;
			}

			if (nOpenX2 < 1 || nOpenY2 < 1) {
				m_strErrorMessage = "濾波大小不可小於1";
				return false;
			}

			if (fGlueROI_Tolerance < 0.1 || fGlueROI_Tolerance >= 1.0) {
				m_strErrorMessage = "GlueROI_Tolerance輸入錯誤, 請輸入 0.1 - 1.0";
				return false;
			}

			if (!jet_imagefunction::Check_RECT(matConnect.rows, matConnect.cols, rectGlueROI)) {
				m_strErrorMessage = "GlueROI座標輸入錯誤";
				return false;
			}

			SGlueEdge_Parameter sParam;
			sParam.nFindEdgeMethod = 2;
			sParam.nLocation = nLocation;
			sParam.fGlueROI_Tolerance = fGlueROI_Tolerance;
			sParam.rectGlueROI = rectGlueROI;
			sParam.nFilter_OpenX2 = nOpenX2;
			sParam.nFilter_OpenY2 = nOpenY2;

			m_matGlue = matConnect;
			SProcessModeParam sPM_All;
			if (!SetGlueProcess_FindContours(sParam, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Result;
			if (!sPM_All.ExtractList(7, 13, sPM_Result)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			// 進行轉灰階流程
			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matConnect;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Result, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentGlueParameter", "Method2_Step4", sPM_Result, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Result.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Result.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matGlue = vtsResult[nCount - 1].matImage.clone();
			if (matGlue.empty() || matGlue.channels() != 1 || matGlue.size() != matConnect.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		// Method2-Step5-End 膠本體的二值化影像與 膨脹 侵蝕濾波的大小, 膠輪廓影像
		// [In] matColor : 膠的彩色影像(彩色)
		// [In] matGlue : 膠本體的二值化影像(灰階)
		// [In] nDilateX nDilateY : 將膠本體的二值化影像進行膨脹
		// [In] nErosionX nErosionY : 將膠本體的二值化影像進行侵蝕
		// [Out] matEnd : 膠輪廓影像(彩色)
		bool MeasurementBlackGlue::AdjustmentGlueParameter_Method2_Step5_End(const Mat& matColor, const Mat& matGlue, const int& nLocation, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, Mat& matEnd)
		{
			m_strErrorMessage = "OK";
			if (matColor.empty() || matColor.channels() != 3) {
				m_strErrorMessage = "請輸入有效的彩色影像";
				return false;
			}

			if (matGlue.empty() || matGlue.channels() != 1) {
				m_strErrorMessage = "請輸入有效的灰階影像";
				return false;
			}

			if (nLocation < 1 || nLocation > 10) {
				m_strErrorMessage = "Location輸入錯誤, 請輸入1-10";
				return false;
			}

			if (nDilateX < 1 || nDilateY < 1) {
				m_strErrorMessage = "膨脹濾波大小不可小於1";
				return false;
			}

			if (nErosionX < 1 || nErosionY < 1) {
				m_strErrorMessage = "侵蝕濾波大小不可小於1";
				return false;
			}

			SGlueEdge_Parameter sParam;
			sParam.nFindEdgeMethod = 2;
			sParam.nFilter_DilateX = nDilateX;
			sParam.nFilter_DilateY = nDilateY;
			sParam.nFilter_ErosionX = nErosionX;
			sParam.nFilter_ErosionY = nErosionY;
			sParam.nLocation = nLocation;
			m_matGlue = matColor;
			sParam.rectGlueROI.left = 1;
			sParam.rectGlueROI.right = 5;
			sParam.rectGlueROI.top = 1;
			sParam.rectGlueROI.bottom = 5;

			SProcessModeParam sPM_All;
			if (!SetGlueProcess_FindContours(sParam, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Result;
			if (!sPM_All.ExtractList(14, sPM_All.GetBaseParameterNumber(), sPM_Result)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matGlue;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Result, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentGlueParameter", "Method2_Step5", sPM_Result, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Result.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK", "Method2_Step5", sPM_Result, vtmatImage, vtsResult);
			}

			int nCount = sPM_Result.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			SContoursParam* psContours = &vtsResult[nCount - 1].sContours;
			if (psContours == nullptr) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			vector<POINT> vtptContoursPos;
			if (!ContoursConvertVector(psContours, vtptContoursPos)) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matEnd = matColor.clone();
			if (matGlue.empty() || matGlue.channels() != 1 || matGlue.size() != matEnd.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			jet_imagefunction::DrawEdgePoint(matEnd, vtptContoursPos, m_scContours_Glue, 0);

			return true;
		}	
#pragma endregion
#pragma endregion

		// 調整板邊參數 相關功能
#pragma region Adjustment Board Parameter

		bool MeasurementBlackGlue::AdjustmentBoardParameter_Method1_Step1_Median(const Mat& matColor, const int& nMedianX, const int& nMedianY, Mat& matMedian)
		{
			if (matColor.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matColor.empty() || matColor.channels() != 3) {
				m_strErrorMessage = "輸入影像不是彩色";
				return false;
			}

			if (nMedianX < 1) {
				m_strErrorMessage = "MedianX 不可小於1";
				return false;
			}

			if (nMedianY < 1) {
				m_strErrorMessage = "MedianY 不可小於1";
				return false;
			}

			SBoardEdge_Parameter sBoard;
			sBoard.nFindEdgeMethod = 1;
			sBoard.nMedianX = nMedianX;
			sBoard.nMedianY = nMedianY;

			SProcessModeParam sPM_All;
			if (!SetBoardProcess_FindContours(sBoard, 1, matColor.cols, matColor.rows, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_BoardThreshold;
			if (!sPM_All.ExtractList(1, 1, sPM_BoardThreshold)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matColor;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_BoardThreshold, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentBoardParameter", "Method1_Step1", sPM_BoardThreshold, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_BoardThreshold.GetErrorMessage();
				return false;
			}

			int nCount = sPM_BoardThreshold.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確";
				return false;
			}

			matMedian = vtsResult[nCount - 1].matImage.clone();
			if (matMedian.empty() || matMedian.channels() != 3 || matColor.size() != matMedian.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentBoardParameter_Method1_Step2_ColorToThreshold(const Mat& matMedian, const vector<bool>& vtbEnable, const vector<int>& vtnBoardValue_R, const vector<int>& vtnBoardValue_G, const vector<int>& vtnBoardValue_B, const vector<int>& vtnBoardValueTolerance, Mat& matThreshold)
		{
			if (matMedian.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matMedian.empty() || matMedian.channels() != 3) {
				m_strErrorMessage = "輸入影像不是彩色";
				return false;
			}

			int nCount = 4;
			if (vtnBoardValue_R.size() != vtnBoardValue_G.size() || vtnBoardValue_G.size() != vtnBoardValue_B.size() ||
				vtnBoardValue_B.size() != vtnBoardValueTolerance.size() || vtnBoardValueTolerance.size() != nCount || vtbEnable.size() != nCount-1) {
				m_strErrorMessage = "抽色參數不是4組";
				return false;
			}

			for (int k = 0; k < nCount; ++k) {
				if (vtnBoardValue_R[k] < 0 || vtnBoardValue_R[k] > 255 || vtnBoardValue_G[k] < 0 || vtnBoardValue_G[k] > 255 || vtnBoardValue_B[k] < 0 || vtnBoardValue_B[k] > 255) {
					m_strErrorMessage = "第" + to_string(k+1) +"組抽色值 RGB 範圍設定錯誤";
					return false;
				}

				if (vtnBoardValueTolerance[k] < 0 || vtnBoardValueTolerance[k] > 255) {
					m_strErrorMessage = "第" + to_string(k + 1) + "組抽色值 誤差 範圍設定錯誤";
					return false;
				}
			}

			SBoardEdge_Parameter sBoard;
			sBoard.nFindEdgeMethod = 1;
			sBoard.nBoardValue_R = vtnBoardValue_R[0];
			sBoard.nBoardValue_G = vtnBoardValue_G[0];
			sBoard.nBoardValue_B = vtnBoardValue_B[0];
			sBoard.nBoardValueTolerance = vtnBoardValueTolerance[0];

			sBoard.bEnable_Color2 = vtbEnable[0];
			sBoard.nBoardValue_R2 = vtnBoardValue_R[1];
			sBoard.nBoardValue_G2 = vtnBoardValue_G[1];
			sBoard.nBoardValue_B2 = vtnBoardValue_B[1];
			sBoard.nBoardValueTolerance2 = vtnBoardValueTolerance[1];

			sBoard.bEnable_Color3 = vtbEnable[1];
			sBoard.nBoardValue_R3 = vtnBoardValue_R[2];
			sBoard.nBoardValue_G3 = vtnBoardValue_G[2];
			sBoard.nBoardValue_B3 = vtnBoardValue_B[2];
			sBoard.nBoardValueTolerance3 = vtnBoardValueTolerance[2];

			sBoard.bEnable_Color4 = vtbEnable[2];
			sBoard.nBoardValue_R4 = vtnBoardValue_R[3];
			sBoard.nBoardValue_G4 = vtnBoardValue_G[3];
			sBoard.nBoardValue_B4 = vtnBoardValue_B[3];
			sBoard.nBoardValueTolerance4 = vtnBoardValueTolerance[3];

			int nEndStep = -1;
			if (sBoard.bEnable_Color2 == false && sBoard.bEnable_Color3 == false && sBoard.bEnable_Color4 == false) {
				nEndStep = 2;
			}
			else if (sBoard.bEnable_Color2 == true && sBoard.bEnable_Color3 == false && sBoard.bEnable_Color4 == false) {
				nEndStep = 4;
			}
			else if (sBoard.bEnable_Color2 == false && sBoard.bEnable_Color3 == true && sBoard.bEnable_Color4 == false) {
				nEndStep = 4;
			}
			else if (sBoard.bEnable_Color2 == false && sBoard.bEnable_Color3 == false && sBoard.bEnable_Color4 == true) {
				nEndStep = 4;
			}
			else if (sBoard.bEnable_Color2 == true && sBoard.bEnable_Color3 == true && sBoard.bEnable_Color4 == false) {
				nEndStep = 6;
			}
			else if (sBoard.bEnable_Color2 == true && sBoard.bEnable_Color3 == false && sBoard.bEnable_Color4 == true) {
				nEndStep = 6;
			}
			else if (sBoard.bEnable_Color2 == false && sBoard.bEnable_Color3 == true && sBoard.bEnable_Color4 == true) {
				nEndStep = 6;
			}
			else if (sBoard.bEnable_Color2 == true && sBoard.bEnable_Color3 == true && sBoard.bEnable_Color4 == true) {
				nEndStep = 8;
			}

			if (nEndStep == -1) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_All;
			if (!SetBoardProcess_FindContours(sBoard, 1, matMedian.cols, matMedian.rows, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			SProcessModeParam sPM_BoardThreshold;
			vector<pair<SInputType, SInputType>> vtsReplace(1);
			vtsReplace[0].first.SetType(2);
			vtsReplace[0].first.SetId(1);
			vtsReplace[0].second.SetType(1);
			vtsReplace[0].second.SetId(1);
			if (!sPM_All.ExtractList(2, nEndStep, sPM_BoardThreshold, vtsReplace)) {
				m_strErrorMessage = "流程設定錯誤3";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matMedian;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_BoardThreshold, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentBoardParameter", "Method1_Step2", sPM_BoardThreshold, vtmatImage, vtsResult, 2);
				}
				m_strErrorMessage = "執行失敗-" + sPM_BoardThreshold.GetErrorMessage();
				return false;
			}

			

			int nCount2 = sPM_BoardThreshold.GetBaseParameterNumber();
			if (nCount2 != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matThreshold = vtsResult[nCount2 - 1].matImage.clone();
			if (matThreshold.empty() || matThreshold.channels() != 1 || matMedian.size() != matThreshold.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentBoardParameter_Method1_Step3_Open(const Mat& matThreshold, const int& nOpenX, const int& nOpenY, Mat& matOpen)
		{
			if (matThreshold.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matThreshold.channels() != 1) {
				m_strErrorMessage = "輸入影像不是灰階";
				return false;
			}

			if (nOpenX < 1) {
				m_strErrorMessage = "OpenX 不可小於1";
				return false;
			}

			if (nOpenY < 1) {
				m_strErrorMessage = "OpenY 不可小於1";
				return false;
			}

			// 帶入參數
			SBoardEdge_Parameter sBoard;
			sBoard.nOpenX = nOpenX;
			sBoard.nOpenY = nOpenY;
			sBoard.bEnable_Color2 = false;
			sBoard.bEnable_Color3 = false;
			sBoard.bEnable_Color4 = false;

			SProcessModeParam sPM_All;
			if (!SetBoardProcess_FindContours(sBoard, 1, matThreshold.cols, matThreshold.rows, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_BoardFilter;
			if (!sPM_All.ExtractList(3, 3, sPM_BoardFilter)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matThreshold;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_BoardFilter, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentBoardParameter", "Method1_Step3", sPM_BoardFilter, vtmatImage, vtsResult, 2);
				}
				m_strErrorMessage = "執行失敗-" + sPM_BoardFilter.GetErrorMessage();
				return false;
			}

			int nCount = sPM_BoardFilter.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matOpen = vtsResult[0].matImage.clone();
			if (matOpen.empty() || matOpen.channels() != 1 || matOpen.size() != matThreshold.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentBoardParameter_Method1_Step4_Connect(const Mat& matOpen, const int& nConnectX, const int& nConnectY, Mat& matConnect)
		{
			if (matOpen.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matOpen.channels() != 1) {
				m_strErrorMessage = "輸入影像不是灰階";
				return false;
			}

			if (nConnectX < 1) {
				m_strErrorMessage = "ConnectX 不可小於1";
				return false;
			}

			if (nConnectY < 1) {
				m_strErrorMessage = "ConnectY 不可小於1";
				return false;
			}

			// 帶入參數
			SBoardEdge_Parameter sBoard;
			sBoard.nConnectX = nConnectX;
			sBoard.nConnectY = nConnectY;
			sBoard.bEnable_Color2 = false;
			sBoard.bEnable_Color3 = false;
			sBoard.bEnable_Color4 = false;

			SProcessModeParam sPM_All;
			if (!SetBoardProcess_FindContours(sBoard, 1, matOpen.cols, matOpen.rows, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_BoardFilter;
			if (!sPM_All.ExtractList(4, 4, sPM_BoardFilter)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matOpen;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_BoardFilter, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentBoardParameter", "Method1_Step4", sPM_BoardFilter, vtmatImage, vtsResult, 2);
				}
				m_strErrorMessage = "執行失敗-" + sPM_BoardFilter.GetErrorMessage();
				return false;
			}

			int nCount = sPM_BoardFilter.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matConnect = vtsResult[0].matImage.clone();
			if (matConnect.empty() || matConnect.channels() != 1 || matConnect.size() != matOpen.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentBoardParameter_Method1_Step5_End(const Mat& matColor, const Mat& matBoard, bool& bSlope, const int& nLocation, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, Mat& matEnd)
		{
			if (matColor.empty()) {
				m_strErrorMessage = "第一張影像是空的";
				return false;
			}

			if (matColor.channels() != 3) {
				m_strErrorMessage = "第一張影像不是彩色影像";
				return false;
			}

			if (matBoard.empty()) {
				m_strErrorMessage = "第二張影像是空的";
				return false;
			}

			if (matBoard.channels() != 1) {
				m_strErrorMessage = "第一張影像不是灰階影像";
				return false;
			}

			if (matColor.size() != matBoard.size()) {
				m_strErrorMessage = "第一張影像與第二張影像大小不一";
				return false;
			}

			if (nLocation < 1 || nLocation > 10) {
				m_strErrorMessage = "Location 請輸入 1-10";
				return false;
			}

			if (nDilateX < 1) {
				m_strErrorMessage = "DilateX 不可小於1";
				return false;
			}

			if (nDilateY < 1) {
				m_strErrorMessage = "DilateY 不可小於1";
				return false;
			}

			if (nErosionX < 1) {
				m_strErrorMessage = "ErosionX 不可小於1";
				return false;
			}

			if (nErosionY < 1) {
				m_strErrorMessage = "ErosionY 不可小於1";
				return false;
			}

			SBoardEdge_Parameter sBoard;
			sBoard.bEnable_Color2 = false;
			sBoard.bEnable_Color3 = false;
			sBoard.bEnable_Color4 = false;
			sBoard.nDilateX = nDilateX;
			sBoard.nDilateY = nDilateY;
			sBoard.nErosionX = nErosionX;
			sBoard.nErosionY = nErosionY;

			SProcessModeParam sPM_All;
			if (!SetBoardProcess_FindContours(sBoard, nLocation, matColor.cols, matColor.rows, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_BorderLine;
			if (!sPM_All.ExtractList(5, sPM_All.GetBaseParameterNumber(), sPM_BorderLine)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matBoard;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_BorderLine, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentBoardParameter", "Method1_Step5", sPM_BorderLine, vtmatImage, vtsResult, 2);
				}
				m_strErrorMessage = "執行失敗-" + sPM_BorderLine.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK_Board", "Method1_Step5", sPM_BorderLine, vtmatImage, vtsResult, 2);
			}

			SContoursParam* psContours = &vtsResult[sPM_BorderLine.GetBaseParameterNumber() - 1].sContours;
			if (psContours == nullptr) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			SBoardEdge_Result sBoardEdge;
			sBoardEdge.bEnable = true;
			if (!ContoursConvertVector(psContours, sBoardEdge.vtptContoursPos)) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}
			sBoardEdge.nContoursCount = sBoardEdge.vtptContoursPos.size();

			if (bSlope) {
				m_matBoard = matColor;
				if (!Calculate_LinearEquation(nLocation, *psContours, sBoardEdge, false)) {
					m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
					return false;
				}

				// Display
				matEnd = matColor.clone();
				if (!DisplayBoardPosition(sBoardEdge, m_scContours_Board, m_scLine_Board, matEnd)) {
					m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
					return false;
				}
			}
			else {
				// 畫輪廓
				matEnd = matColor.clone();
				jet_imagefunction::DrawEdgePoint(matEnd, sBoardEdge.vtptContoursPos, m_scContours_Board, 1);
			}

			return true;
		}

#pragma endregion

#pragma region Adjustment ThermalGlue Parameter

		bool MeasurementBlackGlue::AdjustmentThermalGlueParameter_Die_Method1_Step1_ROI(const Mat& matColor, const RECT& rectDie, const int& nOutsetDistanceX, const int& nOutsetDistanceY, Mat& matROI_Color)
		{
			if (matColor.empty()) {
				m_strErrorMessage = "影像是空的";
				return false;
			}

			if (matColor.channels() != 3) {
				m_strErrorMessage = "不是彩色影像";
				return false;
			}

			if (!jet_imagefunction::Check_RECT(matColor.rows, matColor.cols, rectDie)) {
				m_strErrorMessage = "Die ROI座標輸入錯誤";
				return false;
			}

			if (nOutsetDistanceX < 0) {
				m_strErrorMessage = "OutsetDistanceX 不可小於0";
				return false;
			}

			if (nOutsetDistanceY < 0) {
				m_strErrorMessage = "OutsetDistanceY 不可小於0";
				return false;
			}

			// 帶入參數
			SThermalGlue_Parameter sThermal;
			sThermal.rectDie = rectDie;
			sThermal.nDie_OutsetDistanceX = nOutsetDistanceX;
			sThermal.nDie_OutsetDistanceY = nOutsetDistanceY;
			m_matThermal = matColor;

			SProcessModeParam sPM_All;
			if (!SetThermalGlueProcess_Find_Die_Contours(sThermal, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Die_ROI;
			if (!sPM_All.ExtractList(1, 1, sPM_Die_ROI)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matColor;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Die_ROI, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentThermalGlueParameter_Die", "Method1_Step1", sPM_Die_ROI, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Die_ROI.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Die_ROI.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確";
				return false;
			}

			matROI_Color = vtsResult[nCount-1].matImage.clone();
			if (matROI_Color.empty() || matROI_Color.channels() != 3) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentThermalGlueParameter_Die_Method1_Step2_ToGray(const Mat& matROI_Color, const int& nMedianX, const int& nMedianY, Mat& matROI_Gray)
		{
			if (matROI_Color.empty()) {
				m_strErrorMessage = "影像是空的";
				return false;
			}

			if (matROI_Color.channels() != 3) {
				m_strErrorMessage = "不是彩色影像";
				return false;
			}

			if (nMedianX < 1) {
				m_strErrorMessage = "MedianX 不可小於1";
				return false;
			}

			if (nMedianY < 1) {
				m_strErrorMessage = "MedianY 不可小於1";
				return false;
			}

			// 帶入參數
			SThermalGlue_Parameter sThermal;
			sThermal.nDie_MedianX = nMedianX;
			sThermal.nDie_MedianY = nMedianY;

			SProcessModeParam sPM_All;
			if (!SetThermalGlueProcess_Find_Die_Contours(sThermal, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Die_ToGray;
			if (!sPM_All.ExtractList(2, 3, sPM_Die_ToGray)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matROI_Color;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Die_ToGray, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentThermalGlueParameter_Die", "Method1_Step2", sPM_Die_ToGray, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Die_ToGray.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Die_ToGray.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matROI_Gray = vtsResult[nCount - 1].matImage.clone();
			if (matROI_Gray.empty() || matROI_Gray.channels() != 1 || matROI_Gray.size() != matROI_Color.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentThermalGlueParameter_Die_Method1_Step3_Threshold(const Mat& matROI_Gray, const int& nThreshold_Low, const int& nThreshold_High, const bool& bBetween, Mat& matROI_Threshold)
		{
			if (matROI_Gray.empty()) {
				m_strErrorMessage = "影像是空的";
				return false;
			}

			if (matROI_Gray.channels() != 1) {
				m_strErrorMessage = "不是灰階影像";
				return false;
			}

			if (nThreshold_Low < 0 || nThreshold_Low > 255) {
				m_strErrorMessage = "Threshold_Low值的範圍是[0-255]";
				return false;
			}

			if (nThreshold_High < 0 || nThreshold_High > 255) {
				m_strErrorMessage = "Threshold_High值的範圍是[0-255]";
				return false;
			}

			if (nThreshold_Low >= nThreshold_High) {
				m_strErrorMessage = "Threshold_Low 必須小於 Threshold_High";
				return false;
			}

			// 帶入參數
			SThermalGlue_Parameter sThermal;
			sThermal.bDie_Between = bBetween;
			sThermal.nDie_Threshold_Low = nThreshold_Low;
			sThermal.nDie_Threshold_High = nThreshold_High;

			SProcessModeParam sPM_All;
			if (!SetThermalGlueProcess_Find_Die_Contours(sThermal, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Die_Threshold;
			vector<pair<SInputType, SInputType>> vtsReplace(1);
			vtsReplace[0].first.SetType(2);
			vtsReplace[0].first.SetId(3);
			vtsReplace[0].second.SetType(1);
			vtsReplace[0].second.SetId(1);
			if (!sPM_All.ExtractList(4, 4, sPM_Die_Threshold, vtsReplace)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matROI_Gray;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Die_Threshold, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentThermalGlueParameter_Die", "Method1_Step3", sPM_Die_Threshold, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Die_Threshold.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 1) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentThermalGlueParameter_Die", "Method1_Step3", sPM_Die_Threshold, vtmatImage, vtsResult);
			}

			int nCount = sPM_Die_Threshold.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matROI_Threshold = vtsResult[nCount - 1].matImage.clone();
			if (matROI_Threshold.empty() || matROI_Threshold.channels() != 1 || matROI_Threshold.size() != matROI_Gray.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentThermalGlueParameter_Die_Method1_Step4_Position(const Mat& matROI_Threshold, const int& nOpenX, const int& nOpenY, const int& nConnectX, const int& nConnectY, Mat& matROI_Open)
		{
			if (matROI_Threshold.empty()) {
				m_strErrorMessage = "影像是空的";
				return false;
			}

			if (matROI_Threshold.channels() != 1) {
				m_strErrorMessage = "不是灰階影像";
				return false;
			}

			if (nOpenX < 1) {
				m_strErrorMessage = "OpenX 不可小於1";
				return false;
			}

			if (nOpenY < 1) {
				m_strErrorMessage = "OpenY 不可小於1";
				return false;
			}

			if (nConnectX < 1) {
				m_strErrorMessage = "ConnectX 不可小於1";
				return false;
			}

			if (nConnectY < 1) {
				m_strErrorMessage = "ConnectY 不可小於1";
				return false;
			}

			// 帶入參數
			SThermalGlue_Parameter sThermal;
			sThermal.nDie_OpenX = nOpenX;
			sThermal.nDie_OpenY = nOpenY;
			sThermal.nDie_ConnectX = nConnectX;
			sThermal.nDie_ConnectY = nConnectY;

			SProcessModeParam sPM_All;
			if (!SetThermalGlueProcess_Find_Die_Contours(sThermal, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Die_Open;
			if (!sPM_All.ExtractList(5, 6, sPM_Die_Open)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matROI_Threshold;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Die_Open, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentThermalGlueParameter_Die", "Method1_Step4", sPM_Die_Open, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Die_Open.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Die_Open.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matROI_Open = vtsResult[nCount - 1].matImage.clone();
			if (matROI_Open.empty() || matROI_Open.channels() != 1 || matROI_Open.size() != matROI_Threshold.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentThermalGlueParameter_Die_Method1_Step5_End(const Mat& matROI_Color, const Mat& matROI_Open, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, const bool& bRebuildDie, RECT& rectDie, Mat& matEnd)
		{
			rectDie.left = 0;
			rectDie.right = 0;
			rectDie.top = 0;
			rectDie.bottom = 0;

			m_strErrorMessage = "OK";
			if (matROI_Color.empty() || matROI_Color.channels() != 3) {
				m_strErrorMessage = "請輸入有效的彩色影像";
				return false;
			}

			if (matROI_Open.empty() || matROI_Open.channels() != 1) {
				m_strErrorMessage = "請輸入有效的灰階影像";
				return false;
			}

			if (nDilateX < 1) {
				m_strErrorMessage = "DilateX 不可小於1";
				return false;
			}

			if (nDilateY < 1) {
				m_strErrorMessage = "DilateY 不可小於1";
				return false;
			}

			if (nErosionX < 1) {
				m_strErrorMessage = "ErosionX 不可小於1";
				return false;
			}

			if (nErosionY < 1) {
				m_strErrorMessage = "ErosionY 不可小於1";
				return false;
			}

			// 帶入參數
			SThermalGlue_Parameter sThermal;
			sThermal.nDie_DilateX = nDilateX;
			sThermal.nDie_DilateY = nDilateY;
			sThermal.nDie_ErosionX = nErosionX;
			sThermal.nDie_ErosionY = nErosionY;
		
			SProcessModeParam sPM_All;
			if (!SetThermalGlueProcess_Find_Die_Contours(sThermal, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Result;
			if (!sPM_All.ExtractList(7, sPM_All.GetBaseParameterNumber(), sPM_Result)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matROI_Open;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Result, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentThermalGlueParameter_Die", "Method1_Step5", sPM_Result, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Result.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Result.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			int nMode = 0;
			SContoursParam* psContours = &vtsResult[nCount - 1].sContours;
			if (psContours == nullptr) return false;

			if (bRebuildDie) {
				nMode = 2;
			}
			else {
				nMode = 1;
			}

			vector<vector<POINT>> vt2ptICEdgePoint;

			int nSearchRangeX = 20;
			int nSearchRangeY = 20;
			if (!ROI_ContoursToLine(nSearchRangeX, nSearchRangeY, psContours, vt2ptICEdgePoint)) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 0) {
				Mat matDisplay = matROI_Color.clone();
				jet_imagefunction::LabelLine(vt2ptICEdgePoint, Scalar(), 1, matDisplay.cols, matDisplay.rows, matDisplay);
				SaveImage(0, matDisplay, "Die_Line", 2);
			}

			if (!ReCreateDie(nMode, vt2ptICEdgePoint, rectDie, m_sResult.sThermalGlue.vtptDieCorner, m_vtdDieSlope, m_vtdDieIntercept)) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matEnd = matROI_Color.clone();
			if (bRebuildDie) {
				Point cvPoint1, cvPoint2;
				cvPoint1.x = m_sResult.sThermalGlue.vtptDieCorner[0].x;
				cvPoint1.y = m_sResult.sThermalGlue.vtptDieCorner[0].y;
				cvPoint2.x = m_sResult.sThermalGlue.vtptDieCorner[1].x;
				cvPoint2.y = m_sResult.sThermalGlue.vtptDieCorner[1].y;
				line(matEnd, cvPoint1, cvPoint2, Scalar(255, 0, 0), 1);

				cvPoint1.x = m_sResult.sThermalGlue.vtptDieCorner[1].x;
				cvPoint1.y = m_sResult.sThermalGlue.vtptDieCorner[1].y;
				cvPoint2.x = m_sResult.sThermalGlue.vtptDieCorner[2].x;
				cvPoint2.y = m_sResult.sThermalGlue.vtptDieCorner[2].y;
				line(matEnd, cvPoint1, cvPoint2, Scalar(0, 255, 0), 1);

				cvPoint1.x = m_sResult.sThermalGlue.vtptDieCorner[2].x;
				cvPoint1.y = m_sResult.sThermalGlue.vtptDieCorner[2].y;
				cvPoint2.x = m_sResult.sThermalGlue.vtptDieCorner[3].x;
				cvPoint2.y = m_sResult.sThermalGlue.vtptDieCorner[3].y;
				line(matEnd, cvPoint1, cvPoint2, Scalar(0, 0, 255), 1);

				cvPoint1.x = m_sResult.sThermalGlue.vtptDieCorner[3].x;
				cvPoint1.y = m_sResult.sThermalGlue.vtptDieCorner[3].y;
				cvPoint2.x = m_sResult.sThermalGlue.vtptDieCorner[0].x;
				cvPoint2.y = m_sResult.sThermalGlue.vtptDieCorner[0].y;
				line(matEnd, cvPoint1, cvPoint2, Scalar(255, 255, 0), 1);
			}
			else {
				// 輸出 Die ROI
				Rect cvROI;
				jet_imagefunction::RECTToRect(rectDie, 0, 0, cvROI);
				cv::rectangle(matEnd, cvROI, Scalar(0, 0, 255), 1);
			}

			return true;
		}

		// Glue
		bool MeasurementBlackGlue::AdjustmentThermalGlueParameter_Glue_Method1_Step1_ROI(const Mat& matColor, const RECT& rectDie, const int& nInsetDistanceX, const int& nInsetDistanceY, Mat& matROI_Color)
		{
			if (matColor.empty()) {
				m_strErrorMessage = "影像是空的";
				return false;
			}

			if (matColor.channels() != 3) {
				m_strErrorMessage = "不是彩色影像";
				return false;
			}

			if (!jet_imagefunction::Check_RECT(matColor.rows, matColor.cols, rectDie)) {
				m_strErrorMessage = "Die ROI座標輸入錯誤";
				return false;
			}

			if (nInsetDistanceX < 0) {
				m_strErrorMessage = "InsetDistanceX 不可小於0";
				return false;
			}

			if (nInsetDistanceY < 0) {
				m_strErrorMessage = "InsetDistanceY 不可小於0";
				return false;
			}

			// 帶入參數
			SThermalGlue_Parameter sThermal;
			sThermal.nDie_InsetDistanceX = nInsetDistanceX;
			sThermal.nDie_InsetDistanceY = nInsetDistanceY;
			m_matThermal = matColor;

			SProcessModeParam sPM_All;
			if (!SetThermalGlueProcess_Find_Glue_Contours(sThermal, rectDie, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Glue_ROI;
			if (!sPM_All.ExtractList(1, 1, sPM_Glue_ROI)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matColor;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Glue_ROI, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentThermalGlueParameter_Glue", "Method1_Step1", sPM_Glue_ROI, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Glue_ROI.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Glue_ROI.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確";
				return false;
			}

			matROI_Color = vtsResult[nCount - 1].matImage.clone();
			if (matROI_Color.empty() || matROI_Color.channels() != 3) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentThermalGlueParameter_Glue_Method1_Step2_ToGray(const Mat& matROI_Color, const int& nMedianX, const int& nMedianY, Mat& matROI_Gray)
		{
			if (matROI_Color.empty()) {
				m_strErrorMessage = "影像是空的";
				return false;
			}

			if (matROI_Color.channels() != 3) {
				m_strErrorMessage = "不是彩色影像";
				return false;
			}

			if (nMedianX < 1) {
				m_strErrorMessage = "MedianX 不可小於1";
				return false;
			}

			if (nMedianY < 1) {
				m_strErrorMessage = "MedianY 不可小於1";
				return false;
			}

			// 帶入參數
			SThermalGlue_Parameter sThermal;
			sThermal.nGlue_MedianX = nMedianX;
			sThermal.nGlue_MedianY = nMedianY;

			RECT rectTemp;
			rectTemp.left = 1;
			rectTemp.top = 1;
			rectTemp.right = 2;
			rectTemp.bottom = 2;
			SProcessModeParam sPM_All;
			if (!SetThermalGlueProcess_Find_Glue_Contours(sThermal, rectTemp, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Glue_ToGray;
			if (!sPM_All.ExtractList(2, 3, sPM_Glue_ToGray)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matROI_Color;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Glue_ToGray, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentThermalGlueParameter_Glue", "Method1_Step2", sPM_Glue_ToGray, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Glue_ToGray.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Glue_ToGray.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matROI_Gray = vtsResult[nCount - 1].matImage.clone();
			if (matROI_Gray.empty() || matROI_Gray.channels() != 1 || matROI_Gray.size() != matROI_Color.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentThermalGlueParameter_Glue_Method1_Step3_Threshold(const Mat& matROI_Gray, const bool& bDark, const int& nThreshold, Mat& matROI_Threshold)
		{
			if (matROI_Gray.empty()) {
				m_strErrorMessage = "影像是空的";
				return false;
			}

			if (matROI_Gray.channels() != 1) {
				m_strErrorMessage = "不是灰階影像";
				return false;
			}

			if (nThreshold < 0 || nThreshold > 255) {
				m_strErrorMessage = "Threshold的範圍是[0-255]";
				return false;
			}

			// 帶入參數
			SThermalGlue_Parameter sThermal;
			sThermal.bGlue_Dark = bDark;
			sThermal.nGlue_Threshold = nThreshold;

			RECT rectTemp;
			rectTemp.left = 1;
			rectTemp.top = 1;
			rectTemp.right = 2;
			rectTemp.bottom = 2;

			SProcessModeParam sPM_All;
			if (!SetThermalGlueProcess_Find_Glue_Contours(sThermal, rectTemp, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Glue_Threshold;
			if (!sPM_All.ExtractList(4, 4, sPM_Glue_Threshold)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matROI_Gray;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Glue_Threshold, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentThermalGlueParameter_Glue", "Method1_Step3", sPM_Glue_Threshold, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Glue_Threshold.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Glue_Threshold.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matROI_Threshold = vtsResult[nCount - 1].matImage.clone();
			if (matROI_Threshold.empty() || matROI_Threshold.channels() != 1 || matROI_Threshold.size() != matROI_Gray.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentThermalGlueParameter_Glue_Method1_Step4_End(const Mat& matROI_Color, const Mat& matROI_Threshold, const int& nGlueDirection, const int& nOpenX, const int& nOpenY, int& nGlueCount, Mat& matEnd)
		{
			nGlueCount = 0;
			if (matROI_Color.empty() || matROI_Color.channels() != 3) {
				m_strErrorMessage = "請輸入有效的彩色影像";
				return false;
			}

			if (matROI_Threshold.empty() || matROI_Threshold.channels() != 1) {
				m_strErrorMessage = "請輸入有效的灰階影像";
				return false;
			}

			if (nGlueDirection < 1 || nGlueDirection > 2) {
				m_strErrorMessage = "請設定膠排列方向,水平或垂直";
				return false;
			}

			if (nOpenX < 1) {
				m_strErrorMessage = "OpenX 不可小於1";
				return false;
			}

			if (nOpenY < 1) {
				m_strErrorMessage = "OpenY 不可小於1";
				return false;
			}

			// 帶入參數
			SThermalGlue_Parameter sThermal;
			sThermal.nGlue_OpenX = nOpenX;
			sThermal.nGlue_OpenY = nOpenY;
			sThermal.nGlue_Direction = nGlueDirection;
			m_matThermal = matROI_Color;

			RECT rectTemp;
			rectTemp.left = 1;
			rectTemp.top = 1;
			rectTemp.right = 2;
			rectTemp.bottom = 2;

			SProcessModeParam sPM_All;
			if (!SetThermalGlueProcess_Find_Glue_Contours(sThermal, rectTemp, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Result;
			if (!sPM_All.ExtractList(6, sPM_All.GetBaseParameterNumber(), sPM_Result)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matROI_Threshold;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Result, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentThermalGlueParameter_Glue", "Method1_Step4", sPM_Result, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Result.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Result.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			SContoursParam* psContours = &vtsResult[nCount - 1].sContours;
			if (psContours == nullptr) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			nGlueCount = psContours->GetCount_On();

			vector<POINT> vtptContoursPos;
			if (!ContoursConvertVector(psContours, vtptContoursPos)) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matEnd = matROI_Color.clone();
			if (matEnd.empty() || matEnd.channels() != 3 || matEnd.size() != matROI_Color.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			int nId = 0;
			vector<SJRect*> vtpjRect(nGlueCount);
			for (int k = 0; k < psContours->GetCount(); ++k) {
				bool* pbOn = psContours->GetOnPtr(k);
				if (pbOn == nullptr) {
					m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
					return false;
				}

				if (*pbOn) {
					SJRect* pjRect = psContours->GetSJRectPtr(k);
					if (pjRect == nullptr) {
						m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
						return false;
					}
					vtpjRect[nId] = pjRect;
					++nId;
					if (nId >= nGlueCount) {
						k = psContours->GetCount();
					}
				}
			}

			vector<RECT> vtrectSort;
			Sort_ROI(nGlueDirection, vtpjRect, vtrectSort);

			jet_imagefunction::DrawEdgePoint(matEnd, vtptContoursPos, Scalar(0, 255, 255), 1);
			for (int k = 0; k < nGlueCount; ++k) {
				Rect cvROI;
				jet_imagefunction::RECTToRect(vtrectSort[k], 0, 0, cvROI, matEnd.rows, matEnd.cols);

				if (nGlueDirection == 1) {
					Point ptCenter = Point(cvROI.x + cvROI.width / 3, (cvROI.y + cvROI.br().y) / 2);
					cv::putText(matEnd, "Therml Glue " + to_string(k+1), ptCenter, FONT_HERSHEY_TRIPLEX, 1.0, Scalar(0, 128, 0), 2);
				}
				else {
					int nY = cvROI.height / 5;
					Point ptCenter = Point(cvROI.x , nY*(k + 1));
					cv::putText(matEnd, "Therml Glue " + to_string(k+1), ptCenter, FONT_HERSHEY_TRIPLEX, 1.0, Scalar(0, 128, 0), 2);
				}
				cv::rectangle(matEnd, cvROI, Scalar(0, 0, 255), 2);
			}

			return true;
		}
#pragma endregion

		// 調整 Coating參數
#pragma region Adjustment Coating Parameter

#pragma region Method1

		bool MeasurementBlackGlue::AdjustmentCoatingParameter_Method1_Step1_Median(const Mat& matColor, const int& nMedianX, const int& nMedianY, Mat& matMedian)
		{
			m_strErrorMessage = "OK";
			if (matColor.empty()) {
				m_strErrorMessage = "影像是空的";
				return false;
			}

			if (matColor.channels() != 3) {
				m_strErrorMessage = "不是彩色影像";
				return false;
			}

			if (nMedianX < 1) {
				m_strErrorMessage = "MedianX 不可小於1";
				return false;
			}

			if (nMedianY < 1) {
				m_strErrorMessage = "MedianY 不可小於1";
				return false;
			}

			// 帶入參數
			SCoatingEdge_Parameter sCoating;
			sCoating.nFindEdgeMethod = 1;
			sCoating.nFilter_MedianX = nMedianX;
			sCoating.nFilter_MedianY = nMedianY;

			SProcessModeParam sPM_All;
			if (!SetCoatingProcess_FindContours(sCoating, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Median;
			if (!sPM_All.ExtractList(1, 1, sPM_Median)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matColor;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Median, vtsResult)) {
				m_strErrorMessage = "流程執行錯誤1";
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentCoatingParameter", "Method1_Step1", sPM_Median, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Median.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Median.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確";
				return false;
			}

			matMedian = vtsResult[nCount - 1].matImage.clone();
			if (matMedian.empty() || matMedian.channels() != 3 || matColor.size() != matMedian.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentCoatingParameter_Method1_Step2_ColorToThreshold(const Mat& matMedian, const int& nCoatingValue_R, const int& nCoatingValue_G, const int& nCoatingValue_B, const int& nCoatingValueTolerance, Mat& matThreshold)
		{
			if (matMedian.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matMedian.channels() != 3) {
				m_strErrorMessage = "輸入影像不是彩色";
				return false;
			}

			if (nCoatingValue_R < 0 || nCoatingValue_R > 255 || nCoatingValue_G < 0 || nCoatingValue_G > 255 || nCoatingValue_B < 0 || nCoatingValue_B > 255) {
				m_strErrorMessage = "抽色 RGB值 範圍設定錯誤";
				return false;
			}

			if (nCoatingValueTolerance < 0 || nCoatingValueTolerance > 255) {
				m_strErrorMessage = "抽色值 誤差 範圍設定錯誤";
				return false;
			}

			// 帶入參數
			SCoatingEdge_Parameter sCoating;
			sCoating.nFindEdgeMethod = 1;
			sCoating.nCoatingValue_R = nCoatingValue_R;
			sCoating.nCoatingValue_G = nCoatingValue_G;
			sCoating.nCoatingValue_B = nCoatingValue_B;
			sCoating.nCoatingValueTolerance = nCoatingValueTolerance;

			SProcessModeParam sPM_All;
			if (!SetCoatingProcess_FindContours(sCoating, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Threshold;
			if (!sPM_All.ExtractList(2, 2, sPM_Threshold)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matMedian;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Threshold, vtsResult)) {
				m_strErrorMessage = "流程執行錯誤1";
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentCoatingParameter", "Method1_Step2", sPM_Threshold, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Threshold.GetErrorMessage();
				return false;
			}

			int nCount2 = sPM_Threshold.GetBaseParameterNumber();
			if (nCount2 != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matThreshold = vtsResult[nCount2 - 1].matImage.clone();
			if (matThreshold.empty() || matThreshold.channels() != 1 || matMedian.size() != matThreshold.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentCoatingParameter_Method1_Step3_Open(const Mat& matThreshold, const int& nOpenX, const int& nOpenY, Mat& matOpen)
		{
			if (matThreshold.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matThreshold.empty() || matThreshold.channels() != 1) {
				m_strErrorMessage = "輸入影像不是灰階";
				return false;
			}

			if (nOpenX < 1) {
				m_strErrorMessage = "OpenX 不可小於1";
				return false;
			}

			if (nOpenY < 1) {
				m_strErrorMessage = "OpenY 不可小於1";
				return false;
			}

			// 帶入參數
			SCoatingEdge_Parameter sCoating;
			sCoating.nFindEdgeMethod = 1;
			sCoating.nFilter_OpenX = nOpenX;
			sCoating.nFilter_OpenY = nOpenY;

			SProcessModeParam sPM_All;
			if (!SetCoatingProcess_FindContours(sCoating, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Open;
			if (!sPM_All.ExtractList(3, 3, sPM_Open)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matThreshold;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Open, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentCoatingParameter", "Method1_Step3", sPM_Open, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Open.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Open.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matOpen = vtsResult[nCount - 1].matImage.clone();
			if (matOpen.empty() || matOpen.channels() != 1 || matOpen.size() != matThreshold.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentCoatingParameter_Method1_Step4_Connect(const Mat& matOpen, const int& nConnectX, const int& nConnectY, const int& nSmoothX, const int& nSmoothY, Mat& matConnect)
		{
			if (matOpen.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matOpen.channels() != 1) {
				m_strErrorMessage = "輸入影像不是灰階";
				return false;
			}

			if (nConnectX < 1) {
				m_strErrorMessage = "ConnectX 不可小於1";
				return false;
			}

			if (nConnectY < 1) {
				m_strErrorMessage = "ConnectY 不可小於1";
				return false;
			}

			if (nSmoothX < 1) {
				m_strErrorMessage = "SmoothX 不可小於1";
				return false;
			}

			if (nSmoothY < 1) {
				m_strErrorMessage = "SmoothY 不可小於1";
				return false;
			}

			// 帶入參數
			SCoatingEdge_Parameter sCoating;
			sCoating.nFindEdgeMethod = 1;
			sCoating.nFilter_ConnectX = nConnectX;
			sCoating.nFilter_ConnectY = nConnectY;
			sCoating.nFilter_SmoothX = nSmoothX;
			sCoating.nFilter_SmoothY = nSmoothY;

			SProcessModeParam sPM_All;
			if (!SetCoatingProcess_FindContours(sCoating, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Connect;
			if (!sPM_All.ExtractList(4, 5, sPM_Connect)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matOpen;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Connect, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentCoatingParameter", "Method1_Step4", sPM_Connect, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Connect.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Connect.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matConnect = vtsResult[nCount - 1].matImage.clone();
			if (matConnect.empty() || matConnect.channels() != 1 || matConnect.size() != matOpen.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentCoatingParameter_Method1_Step5_End(const Mat& matColor, const Mat& matConnect, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, Mat& matEnd)
		{
			if (matColor.empty()) {
				m_strErrorMessage = "第一張輸入影像是空的";
				return false;
			}

			if (matColor.channels() != 3) {
				m_strErrorMessage = "第一張輸入影像不是彩色";
				return false;
			}

			if (matConnect.empty()) {
				m_strErrorMessage = "第二張輸入影像是空的";
				return false;
			}

			if (matConnect.channels() != 1) {
				m_strErrorMessage = "第二張輸入影像不是灰階";
				return false;
			}

			if (nDilateX < 1) {
				m_strErrorMessage = "DilateX 不可小於1";
				return false;
			}

			if (nDilateY < 1) {
				m_strErrorMessage = "DilateY 不可小於1";
				return false;
			}

			if (nErosionX < 1) {
				m_strErrorMessage = "ErosionX 不可小於1";
				return false;
			}

			if (nErosionY < 1) {
				m_strErrorMessage = "ErosionY 不可小於1";
				return false;
			}

			// 帶入參數
			SCoatingEdge_Parameter sCoating;
			sCoating.nFindEdgeMethod = 1;
			sCoating.nFilter_DilateX = nDilateX;
			sCoating.nFilter_DilateY = nDilateY;
			sCoating.nFilter_ErosionX = nErosionX;
			sCoating.nFilter_ErosionY = nErosionY;

			SProcessModeParam sPM_All;
			if (!SetCoatingProcess_FindContours(sCoating, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_End;
			if (!sPM_All.ExtractList(6, sPM_All.GetBaseParameterNumber(), sPM_End)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matConnect;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_End, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentCoatingParameter", "Method1_Step5", sPM_End, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_End.GetErrorMessage();
				return false;
			}

			int nCount = sPM_End.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			SContoursParam* psContours = &vtsResult[nCount - 1].sContours;
			if (psContours == nullptr) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			vector<POINT> vtptContours;
			if (!ContoursConvertVector(psContours, vtptContours)) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			// 畫輪廓
			matEnd = matColor.clone();
			jet_imagefunction::DrawEdgePoint(matEnd, vtptContours, m_scContours_Coating, 1);
			//GetMaxROI(SContoursParam* psContours, const int& nCount, vector<RECT>& vtrectROI, vector<vector<POINT>>& vt2ptContoursPos)
			return true;
		}
		bool MeasurementBlackGlue::AdjustmentCoatingParameter_Method1_Step5_End(const Mat& matColor, const Mat& matConnect, const int& nGlueCount, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, Mat& matEnd)
		{
			if (matColor.empty()) {
				m_strErrorMessage = "第一張輸入影像是空的";
				return false;
			}

			if (matColor.empty() || matColor.channels() != 3) {
				m_strErrorMessage = "第一張輸入影像不是彩色";
				return false;
			}

			if (matConnect.empty()) {
				m_strErrorMessage = "第二張輸入影像是空的";
				return false;
			}

			if (matConnect.empty() || matConnect.channels() != 1) {
				m_strErrorMessage = "第二張輸入影像不是灰階";
				return false;
			}

			if (nGlueCount <= 0) {
				m_strErrorMessage = "GlueCount 要大於0";
				return false;
			}

			if (nDilateX < 1) {
				m_strErrorMessage = "DilateX 不可小於1";
				return false;
			}

			if (nDilateY < 1) {
				m_strErrorMessage = "DilateY 不可小於1";
				return false;
			}

			if (nErosionX < 1) {
				m_strErrorMessage = "ErosionX 不可小於1";
				return false;
			}

			if (nErosionY < 1) {
				m_strErrorMessage = "ErosionY 不可小於1";
				return false;
			}

			// 帶入參數
			SCoatingEdge_Parameter sCoating;
			sCoating.nFindEdgeMethod = 1;
			sCoating.nCount = nGlueCount;
			sCoating.nFilter_DilateX = nDilateX;
			sCoating.nFilter_DilateY = nDilateY;
			sCoating.nFilter_ErosionX = nErosionX;
			sCoating.nFilter_ErosionY = nErosionY;

			SProcessModeParam sPM_All;
			if (!SetCoatingProcess_FindContours(sCoating, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_End;
			if (!sPM_All.ExtractList(6, sPM_All.GetBaseParameterNumber(), sPM_End)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matConnect;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_End, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentCoatingParameter", "Method1_Step5", sPM_End, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_End.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 0) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentCoatingParameter", "Method1_Step5", sPM_End, vtmatImage, vtsResult);
			}

			int nCount = sPM_End.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			SContoursParam* psContours = &vtsResult[nCount - 1].sContours;
			if (psContours == nullptr) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			vector<RECT> vtrectROI;
			vector<vector<POINT>> vt2ptContoursPos;
			if (!GetSortROI(psContours, nGlueCount, vtrectROI, vt2ptContoursPos)) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			// 畫輪廓
			matEnd = matColor.clone();
			for (int k = 0; k < nGlueCount; ++k) {
				jet_imagefunction::DrawEdgePoint(matEnd, vt2ptContoursPos[k], m_scContours_Coating, 1);
			}

			return true;
		}
#pragma endregion

#pragma region Method2

		bool MeasurementBlackGlue::AdjustmentCoatingParameter_Method2_Step1_ColorMedianToGray(const Mat& matColor, const int& nMedianX, const int& nMedianY, Mat& matGray)
		{
			m_strErrorMessage = "OK";
			if (matColor.empty()) {
				m_strErrorMessage = "輸入影像為空";
				return false;
			}

			if (matColor.channels() != 3) {
				m_strErrorMessage = "請輸入彩色影像";
				return false;
			}

			if (nMedianX < 1) {
				m_strErrorMessage = "MedianX 不可小於1";
				return false;
			}

			if (nMedianY < 1) {
				m_strErrorMessage = "MedianY 不可小於1";
				return false;
			}

			// 帶入參數
			SCoatingEdge_Parameter sCoating;
			sCoating.nFindEdgeMethod = 2;
			sCoating.nFilter_MedianX = nMedianX;
			sCoating.nFilter_MedianY = nMedianY;

			SProcessModeParam sPM_All;
			if (!SetCoatingProcess_FindContours(sCoating, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_MedianToGray;
			if (!sPM_All.ExtractList(1, 2, sPM_MedianToGray)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matColor;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_MedianToGray, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentCoatingParameter", "Method2_Step1", sPM_MedianToGray, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_MedianToGray.GetErrorMessage();
				return false;
			}

			int nCount = sPM_MedianToGray.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確";
				return false;
			}

			matGray = vtsResult[nCount - 1].matImage.clone();
			if (matGray.empty() || matGray.channels() != 1 || matColor.size() != matGray.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentCoatingParameter_Method2_Step2_Threshold(const Mat& matGray, const bool& bDark, const int& nThreshold, Mat& matThreshold)
		{
			m_strErrorMessage = "OK";
			if (matGray.empty() || matGray.channels() != 1) {
				m_strErrorMessage = "請輸入有效的灰階影像";
				return false;
			}

			if (nThreshold < 0 || nThreshold > 255) {
				m_strErrorMessage = "Threshold的範圍是[0-255]";
				return false;
			}

			// 帶入參數
			SCoatingEdge_Parameter sCoating;
			sCoating.nFindEdgeMethod = 2;
			sCoating.bDark = bDark;
			sCoating.nThreshold = nThreshold;

			SProcessModeParam sPM_All;
			if (!SetCoatingProcess_FindContours(sCoating, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Threshold;
			if (!sPM_All.ExtractList(3, 3, sPM_Threshold)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matGray;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Threshold, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentCoatingParameter", "Method2_Step2", sPM_Threshold, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Threshold.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Threshold.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matThreshold = vtsResult[nCount - 1].matImage.clone();
			if (matThreshold.empty() || matThreshold.channels() != 1 || matThreshold.size() != matGray.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentCoatingParameter_Method2_Step3_Open(const Mat& matThreshold, const int& nOpenX, const int& nOpenY, Mat& matOpen)
		{
			if (matThreshold.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matThreshold.empty() || matThreshold.channels() != 1) {
				m_strErrorMessage = "輸入影像不是灰階";
				return false;
			}

			if (nOpenX < 1) {
				m_strErrorMessage = "OpenX 不可小於1";
				return false;
			}

			if (nOpenY < 1) {
				m_strErrorMessage = "OpenY 不可小於1";
				return false;
			}

			// 帶入參數
			SCoatingEdge_Parameter sCoating;
			sCoating.nFindEdgeMethod = 2;
			sCoating.nFilter_OpenX = nOpenX;
			sCoating.nFilter_OpenY = nOpenY;

			SProcessModeParam sPM_All;
			if (!SetCoatingProcess_FindContours(sCoating, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Open;
			if (!sPM_All.ExtractList(4, 4, sPM_Open)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matThreshold;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Open, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentCoatingParameter", "Method2_Step3", sPM_Open, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Open.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Open.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matOpen = vtsResult[nCount - 1].matImage.clone();
			if (matOpen.empty() || matOpen.channels() != 1 || matOpen.size() != matThreshold.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentCoatingParameter_Method2_Step4_Connect(const Mat& matOpen, const int& nConnectX, const int& nConnectY, const int& nSmoothX, const int& nSmoothY, Mat& matConnect)
		{
			if (matOpen.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matOpen.empty() || matOpen.channels() != 1) {
				m_strErrorMessage = "輸入影像不是灰階";
				return false;
			}

			if (nConnectX < 1) {
				m_strErrorMessage = "ConnectX 不可小於1";
				return false;
			}

			if (nConnectY < 1) {
				m_strErrorMessage = "ConnectY 不可小於1";
				return false;
			}

			if (nSmoothX < 1 ) {
				m_strErrorMessage = "SmoothX 不可小於1";
				return false;
			}

			if (nSmoothY < 1) {
				m_strErrorMessage = "SmoothY 不可小於1";
				return false;
			}

			// 帶入參數
			SCoatingEdge_Parameter sCoating;
			sCoating.nFindEdgeMethod = 2;
			sCoating.nFilter_ConnectX = nConnectX;
			sCoating.nFilter_ConnectY = nConnectY;
			sCoating.nFilter_SmoothX = nSmoothX;
			sCoating.nFilter_SmoothY = nSmoothY;

			SProcessModeParam sPM_All;
			if (!SetCoatingProcess_FindContours(sCoating, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Connect;
			if (!sPM_All.ExtractList(5, 6, sPM_Connect)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matOpen;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Connect, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentCoatingParameter", "Method2_Step4", sPM_Connect, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Connect.GetErrorMessage();
				return false;
			}

			int nCount = sPM_Connect.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matConnect = vtsResult[nCount - 1].matImage.clone();
			if (matConnect.empty() || matConnect.channels() != 1 || matConnect.size() != matOpen.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentCoatingParameter_Method2_Step5_End(const Mat& matColor, const Mat& matConnect, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, Mat& matEnd)
		{
			if (matColor.empty()) {
				m_strErrorMessage = "第一張輸入影像是空的";
				return false;
			}

			if (matColor.empty() || matColor.channels() != 3) {
				m_strErrorMessage = "第一張輸入影像不是彩色";
				return false;
			}

			if (matConnect.empty()) {
				m_strErrorMessage = "第二張輸入影像是空的";
				return false;
			}

			if (matConnect.empty() || matConnect.channels() != 1) {
				m_strErrorMessage = "第二張輸入影像不是灰階";
				return false;
			}

			if (nDilateX < 1) {
				m_strErrorMessage = "DilateX 不可小於1";
				return false;
			}

			if (nDilateY < 1) {
				m_strErrorMessage = "DilateY 不可小於1";
				return false;
			}

			if (nErosionX < 1) {
				m_strErrorMessage = "ErosionX 不可小於1";
				return false;
			}

			if (nErosionY < 1) {
				m_strErrorMessage = "ErosionY 不可小於1";
				return false;
			}

			// 帶入參數
			SCoatingEdge_Parameter sCoating;
			sCoating.nFindEdgeMethod = 2;
			sCoating.nFilter_DilateX = nDilateX;
			sCoating.nFilter_DilateY = nDilateY;
			sCoating.nFilter_ErosionX = nErosionX;
			sCoating.nFilter_ErosionY = nErosionY;

			SProcessModeParam sPM_All;
			if (!SetCoatingProcess_FindContours(sCoating, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_End;
			if (!sPM_All.ExtractList(7, sPM_All.GetBaseParameterNumber(), sPM_End)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matConnect;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_End, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentCoatingParameter", "Method2_Step5", sPM_End, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_End.GetErrorMessage();
				return false;
			}

			int nCount = sPM_End.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			SContoursParam* psContours = &vtsResult[nCount - 1].sContours;
			if (psContours == nullptr) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			vector<POINT> vtptContours;
			if (!ContoursConvertVector(psContours, vtptContours)) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			// 畫輪廓
			matEnd = matColor.clone();
			jet_imagefunction::DrawEdgePoint(matEnd, vtptContours, m_scContours_Coating, 1);

			return true;
		}
		bool MeasurementBlackGlue::AdjustmentCoatingParameter_Method2_Step5_End(const Mat& matColor, const Mat& matConnect, const int& nGlueCount, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, Mat& matEnd)
		{
			if (matColor.empty()) {
				m_strErrorMessage = "第一張輸入影像是空的";
				return false;
			}

			if (matColor.empty() || matColor.channels() != 3) {
				m_strErrorMessage = "第一張輸入影像不是彩色";
				return false;
			}

			if (matConnect.empty()) {
				m_strErrorMessage = "第二張輸入影像是空的";
				return false;
			}

			if (matConnect.empty() || matConnect.channels() != 1) {
				m_strErrorMessage = "第二張輸入影像不是灰階";
				return false;
			}

			if (nGlueCount <= 0) {
				m_strErrorMessage = "GlueCount 要大於0";
				return false;
			}

			if (nDilateX < 1) {
				m_strErrorMessage = "DilateX 不可小於1";
				return false;
			}

			if (nDilateY < 1) {
				m_strErrorMessage = "DilateY 不可小於1";
				return false;
			}

			if (nErosionX < 1) {
				m_strErrorMessage = "ErosionX 不可小於1";
				return false;
			}

			if (nErosionY < 1) {
				m_strErrorMessage = "ErosionY 不可小於1";
				return false;
			}

			// 帶入參數
			SCoatingEdge_Parameter sCoating;
			sCoating.nFindEdgeMethod = 2;
			sCoating.nCount = nGlueCount;
			sCoating.nFilter_DilateX = nDilateX;
			sCoating.nFilter_DilateY = nDilateY;
			sCoating.nFilter_ErosionX = nErosionX;
			sCoating.nFilter_ErosionY = nErosionY;

			SProcessModeParam sPM_All;
			if (!SetCoatingProcess_FindContours(sCoating, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_End;
			if (!sPM_All.ExtractList(7, sPM_All.GetBaseParameterNumber(), sPM_End)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matConnect;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_End, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_AdjustmentCoatingParameter", "Method2_Step5", sPM_End, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_End.GetErrorMessage();
				return false;
			}

			int nCount = sPM_End.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			SContoursParam* psContours = &vtsResult[nCount - 1].sContours;
			if (psContours == nullptr) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			vector<RECT> vtrectROI;
			vector<vector<POINT>> vt2ptContoursPos;
			if (!GetSortROI(psContours, nGlueCount, vtrectROI, vt2ptContoursPos)) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			// 畫輪廓
			matEnd = matColor.clone();
			for (int k = 0; k < nGlueCount; ++k) {
				jet_imagefunction::DrawEdgePoint(matEnd, vt2ptContoursPos[k], m_scContours_Coating, 1);
			}

			return true;
		}
#pragma endregion

#pragma endregion

#pragma region Adjustment FindEdge Parameter

		// Step 1:色彩轉換
		bool MeasurementBlackGlue::AdjustmentFindEdgeParameter_Step1_ColorTransform(const Mat& matColor, const int& nColorMode, Mat& matTransform)
		{
			if (matColor.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (nColorMode < 1 || nColorMode > 11) {
				m_strErrorMessage = "轉換模式 輸入錯誤";
				return false;
			}

			// 帶入參數
			SFindEdge_Parameter sFindEdge;
			sFindEdge.nColorTransformMode = nColorMode;

			SProcessModeParam sPM_All;
			if (!SetFindEdgeProcess(sFindEdge, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_ColorTransform;
			if (!sPM_All.ExtractList(1, 1, sPM_ColorTransform)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matColor;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_ColorTransform, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_NG_AdjustmentFindEdgeParameter", "Step1", sPM_ColorTransform, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_ColorTransform.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK_AdjustmentFindEdgeParameter", "Step1", sPM_ColorTransform, vtmatImage, vtsResult);
			}

			int nCount = sPM_ColorTransform.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matTransform = vtsResult[nCount - 1].matImage.clone();

			return true;
		}
		bool MeasurementBlackGlue::AdjustmentFindEdgeParameter_Step1_ColorTransform_GlueHeight(const Mat& matColor, const int& nColorMode, Mat& matTransform)
		{
			if (matColor.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (nColorMode < 1 || nColorMode > 11) {
				m_strErrorMessage = "轉換模式 輸入錯誤";
				return false;
			}

			// 帶入參數
			SFindEdge_Parameter sFindEdge;
			sFindEdge.nColorTransformMode = nColorMode;

			SProcessModeParam sPM_All;
			if (!SetFindEdgeProcess_GlueHeight(sFindEdge, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_ColorTransform;
			if (!sPM_All.ExtractList(1, 1, sPM_ColorTransform)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matColor;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_ColorTransform, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_NG_AdjustmentFindEdgeParameter", "Step1", sPM_ColorTransform, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_ColorTransform.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK_AdjustmentFindEdgeParameter", "Step1", sPM_ColorTransform, vtmatImage, vtsResult);
			}

			int nCount = sPM_ColorTransform.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matTransform = vtsResult[nCount - 1].matImage.clone();

			return true;
		}

		// Step 2:中值濾波
		bool MeasurementBlackGlue::AdjustmentFindEdgeParameter_Step2_Median(const Mat& matTransform, const int& nFilterMode, const int& nMedianSize, Mat& matMedian)
		{
			if (matTransform.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (nFilterMode < 1 || nFilterMode > 6) {
				m_strErrorMessage = "濾波模式設定錯誤";
				return false;
			}

			if (nMedianSize < 1) {
				m_strErrorMessage = "MedianSize 不可小於1";
				return false;
			}

			// 帶入參數
			SFindEdge_Parameter sFindEdge;
			sFindEdge.nFilterMode = nFilterMode;
			sFindEdge.nMedianSize = nMedianSize;

			SProcessModeParam sPM_All;
			if (!SetFindEdgeProcess(sFindEdge, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Median;
			if (!sPM_All.ExtractList(2, 2, sPM_Median)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matTransform;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Median, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_NG_AdjustmentFindEdgeParameter", "Step2", sPM_Median, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Median.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK_AdjustmentFindEdgeParameter", "Step2", sPM_Median, vtmatImage, vtsResult);
			}

			int nCount = sPM_Median.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matMedian = vtsResult[nCount - 1].matImage.clone();

			return true;

		}

		// Step 3: 影像抽色 
		bool MeasurementBlackGlue::AdjustmentFindEdgeParameter_Step3_Threshold(const Mat& matMedian, const SFindEdge_Parameter& sParam, Mat& matThreshold)
		{
			switch (sParam.nColorTransformMode) {
			case 1:
				if (!AdjustmentFindEdgeParameter_Step3_Color(matMedian, sParam.nColorTransformMode, sParam.nColorThresholdMode, sParam.nCount_ColorExtraction, sParam.vtsColorExtraction, sParam.nCount_HsvExtraction, sParam.vtsHsvExtraction, matThreshold)) {
					return false;
				}
				break;
			case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:
				if (!AdjustmentFindEdgeParameter_Step3_Gray(matMedian, sParam.nColorTransformMode, sParam.nCount_GrayExtraction, sParam.vtsGrayExtraction, matThreshold)) {
					return false;
				}
				break;
			}
			return true;
		}
		bool MeasurementBlackGlue::AdjustmentFindEdgeParameter_Step3_Threshold_GlueHeight(const Mat& matMedian, const SFindEdge_Parameter& sParam, Mat& matThreshold)
		{
			switch (sParam.nColorTransformMode) {
			case 1: case 6:
				if (!AdjustmentFindEdgeParameter_Step3_Color_GlueHeight(matMedian, sParam.nColorTransformMode, sParam.nColorThresholdMode, sParam.nCount_ColorExtraction, sParam.vtsColorExtraction, sParam.nCount_HsvExtraction, sParam.vtsHsvExtraction, matThreshold)) {
					return false;
				}
				break;
			case 2:case 3:case 4:case 5:
				if (!AdjustmentFindEdgeParameter_Step3_Gray(matMedian, sParam.nColorTransformMode, sParam.nCount_GrayExtraction, sParam.vtsGrayExtraction, matThreshold)) {
					return false;
				}
				break;
			}
			return true;
		}

		// Step 3-1: 彩色影像抽色 
		bool MeasurementBlackGlue::AdjustmentFindEdgeParameter_Step3_Color(const Mat& matMedian, const int& nColorMode, const int& nColorThresholdMode, const int& nRgbCount, const vector<SColorExtraction>& vtsRgbExtraction, const int& nHsvCount, const vector<SHsvExtraction>& vtsHsvExtraction, Mat& matThreshold)
		{
			if (matMedian.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matMedian.channels() != 3) {
				m_strErrorMessage = "輸入影像不是彩色";
				return false;
			}

			if (nColorMode < 1 || nColorMode > 5) {
				m_strErrorMessage = "轉換模式 輸入錯誤";
				return false;
			}

			if (nRgbCount != vtsRgbExtraction.size()) {
				m_strErrorMessage = "RGB抽色組數錯誤";
				return false;
			}

			if (nHsvCount != vtsHsvExtraction.size()) {
				m_strErrorMessage = "HSV抽色組數錯誤";
				return false;
			}

			// 帶入參數
			SFindEdge_Parameter sFindEdge;
			sFindEdge.nColorTransformMode = nColorMode;
			sFindEdge.nCount_ColorExtraction = nRgbCount;
			sFindEdge.vtsColorExtraction = vtsRgbExtraction;
			sFindEdge.nColorThresholdMode = nColorThresholdMode;
			sFindEdge.nCount_HsvExtraction = nHsvCount;
			sFindEdge.vtsHsvExtraction = vtsHsvExtraction;

			SProcessModeParam sPM_All;
			if (!SetFindEdgeProcess(sFindEdge, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			int nCount = 0;
			if (sFindEdge.nColorThresholdMode == 1) nCount = nRgbCount;
			else if (sFindEdge.nColorThresholdMode == 2) nCount = nHsvCount;
			int nEndStep = 3 + (nCount - 1) * 2;
			vector<pair<SInputType, SInputType>> vtsReplace(1);
			vtsReplace[0].first.SetType(2);
			vtsReplace[0].first.SetId(2);
			vtsReplace[0].second.SetType(1);
			vtsReplace[0].second.SetId(1);
			SProcessModeParam sPM_Threshold;
			if (!sPM_All.ExtractList(3, nEndStep, sPM_Threshold, vtsReplace)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matMedian;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Threshold, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_NG_AdjustmentFindEdgeParameter", "Step3_Color", sPM_Threshold, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Threshold.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK_AdjustmentFindEdgeParameter", "Step3_Color", sPM_Threshold, vtmatImage, vtsResult);
			}

			int nNumber = sPM_Threshold.GetBaseParameterNumber();
			if (nNumber != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matThreshold = vtsResult[nNumber - 1].matImage.clone();

			return true;
		}
		bool MeasurementBlackGlue::AdjustmentFindEdgeParameter_Step3_Color_GlueHeight(const Mat& matMedian, const int& nColorMode, const int& nColorThresholdMode, const int& nRgbCount, const vector<SColorExtraction>& vtsRgbExtraction, const int& nHsvCount, const vector<SHsvExtraction>& vtsHsvExtraction, Mat& matThreshold)
		{
			if (matMedian.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matMedian.channels() != 3) {
				m_strErrorMessage = "輸入影像不是彩色";
				return false;
			}

			if (nColorMode < 1 || nColorMode > 6) {
				m_strErrorMessage = "轉換模式 輸入錯誤";
				return false;
			}

			if (nRgbCount != vtsRgbExtraction.size()) {
				m_strErrorMessage = "RGB抽色組數錯誤";
				return false;
			}

			if (nHsvCount != vtsHsvExtraction.size()) {
				m_strErrorMessage = "HSV抽色組數錯誤";
				return false;
			}

			// 帶入參數
			SFindEdge_Parameter sFindEdge;
			sFindEdge.nColorTransformMode = nColorMode;
			sFindEdge.nCount_ColorExtraction = nRgbCount;
			sFindEdge.vtsColorExtraction = vtsRgbExtraction;
			sFindEdge.nColorThresholdMode = nColorThresholdMode;
			sFindEdge.nCount_HsvExtraction = nHsvCount;
			sFindEdge.vtsHsvExtraction = vtsHsvExtraction;

			SProcessModeParam sPM_All;
			if (!SetFindEdgeProcess_GlueHeight(sFindEdge, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			int nCount = 0;
			if (sFindEdge.nColorThresholdMode == 1) nCount = nRgbCount;
			else if (sFindEdge.nColorThresholdMode == 2) nCount = nHsvCount;
			int nEndStep = 3 + (nCount - 1) * 2;
			vector<pair<SInputType, SInputType>> vtsReplace(1);
			vtsReplace[0].first.SetType(2);
			vtsReplace[0].first.SetId(2);
			vtsReplace[0].second.SetType(1);
			vtsReplace[0].second.SetId(1);
			SProcessModeParam sPM_Threshold;
			if (!sPM_All.ExtractList(3, nEndStep, sPM_Threshold, vtsReplace)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matMedian;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Threshold, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_NG_AdjustmentFindEdgeParameter", "Step3_Color", sPM_Threshold, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Threshold.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK_AdjustmentFindEdgeParameter", "Step3_Color", sPM_Threshold, vtmatImage, vtsResult);
			}

			int nNumber = sPM_Threshold.GetBaseParameterNumber();
			if (nNumber != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matThreshold = vtsResult[nNumber - 1].matImage.clone();

			return true;
		}

		// Step 3-2: 灰階影像雙閥值二值化 
		bool MeasurementBlackGlue::AdjustmentFindEdgeParameter_Step3_Gray(const Mat& matMedian, const int& nColorMode, const int& nCount, const vector<SGrayExtraction>& vtsGrayExtraction, Mat& matThreshold)
		{
			if (matMedian.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matMedian.channels() != 1) {
				m_strErrorMessage = "輸入影像不是灰階";
				return false;
			}

			if (nColorMode < 1 || nColorMode > 9) {
				m_strErrorMessage = "轉換模式 輸入錯誤";
				return false;
			}

			if (nCount != vtsGrayExtraction.size()) {
				m_strErrorMessage = "灰階抽色組數錯誤";
				return false;
			}

			// 帶入參數
			SFindEdge_Parameter sFindEdge;
			sFindEdge.nColorTransformMode = nColorMode;
			sFindEdge.nCount_GrayExtraction = nCount;
			sFindEdge.vtsGrayExtraction = vtsGrayExtraction;

			SProcessModeParam sPM_All;
			if (!SetFindEdgeProcess(sFindEdge, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			int nEndStep = 3 + (nCount - 1) * 2;
			vector<pair<SInputType, SInputType>> vtsReplace(1);
			vtsReplace[0].first.SetType(2);
			vtsReplace[0].first.SetId(2);
			vtsReplace[0].second.SetType(1);
			vtsReplace[0].second.SetId(1);
			SProcessModeParam sPM_Threshold;
			if (!sPM_All.ExtractList(3, nEndStep, sPM_Threshold, vtsReplace)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matMedian;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Threshold, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_NG_AdjustmentFindEdgeParameter", "Step3_Gray", sPM_Threshold, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Threshold.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK_AdjustmentFindEdgeParameter", "Step3_Gray", sPM_Threshold, vtmatImage, vtsResult);
			}

			int nNumber = sPM_Threshold.GetBaseParameterNumber();
			if (nNumber != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matThreshold = vtsResult[nNumber - 1].matImage.clone();

			return true;
		}

		// Step 4 : 二值化影像過濾雜訊
		bool MeasurementBlackGlue::AdjustmentFindEdgeParameter_Step4_Open(const Mat& matThreshold, const int& nOpenX, const int& nOpenY, Mat& matOpen)
		{
			if (matThreshold.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matThreshold.channels() != 1) {
				m_strErrorMessage = "輸入影像不是灰階";
				return false;
			}

			if (nOpenX < 1) {
				m_strErrorMessage = "OpenX 小於1";
				return false;
			}

			if (nOpenY < 1) {
				m_strErrorMessage = "OpenY 小於1";
				return false;
			}

			// 帶入參數
			SFindEdge_Parameter sFindEdge;
			sFindEdge.nOpenX = nOpenX;
			sFindEdge.nOpenY = nOpenY;

			SProcessModeParam sPM_All;
			if (!SetFindEdgeProcess(sFindEdge, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Open;
			if (!sPM_All.ExtractList(4, 4, sPM_Open)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matThreshold;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Open, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_NG_AdjustmentFindEdgeParameter", "Step4", sPM_Open, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Open.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK_AdjustmentFindEdgeParameter", "Step4", sPM_Open, vtmatImage, vtsResult);
			}

			int nNumber = sPM_Open.GetBaseParameterNumber();
			if (nNumber != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matOpen = vtsResult[nNumber - 1].matImage.clone();

			return true;
		}

		// Step 5 : 二值化影像連接
		bool MeasurementBlackGlue::AdjustmentFindEdgeParameter_Step5_Connect(const Mat& matOpen, const int& nConnectX, const int& nConnectY, Mat& matConnect)
		{
			if (matOpen.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matOpen.channels() != 1) {
				m_strErrorMessage = "輸入影像不是灰階";
				return false;
			}

			if (nConnectX < 1) {
				m_strErrorMessage = "ConnectX 小於1";
				return false;
			}

			if (nConnectY < 1) {
				m_strErrorMessage = "ConnectY 小於1";
				return false;
			}

			// 帶入參數
			SFindEdge_Parameter sFindEdge;
			sFindEdge.nConnectX = nConnectX;
			sFindEdge.nConnectY = nConnectY;

			SProcessModeParam sPM_All;
			if (!SetFindEdgeProcess(sFindEdge, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Connect;
			if (!sPM_All.ExtractList(5, 5, sPM_Connect)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matOpen;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Connect, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_NG_AdjustmentFindEdgeParameter", "Step5", sPM_Connect, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Connect.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK_AdjustmentFindEdgeParameter", "Step5", sPM_Connect, vtmatImage, vtsResult);
			}

			int nNumber = sPM_Connect.GetBaseParameterNumber();
			if (nNumber != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matConnect = vtsResult[nNumber - 1].matImage.clone();

			return true;
		}

		// Step 6 : End
		bool MeasurementBlackGlue::SetFindEdgeProcess_Adj_Step6_BaseObject(const SFindEdge_Parameter& sParam, SProcessModeParam& sPM)
		{
			int nQueueId = 1;
			sPM.Clear();

			// 1
			JET::mod::SColorTransformParam sColorTransform;
			sColorTransform.SetInput1(1, 1);
			switch (sParam.nColorTransformMode)
			{
			case 1:// 彩色
				sColorTransform.eMode = GRAY_TO_BGR;
				break;
			case 2:// R
			case 3:// G
			case 4:// B
				sColorTransform.eMode = COLOR_TO_GRAY_BGR;
				sColorTransform.nID = 4 - sParam.nColorTransformMode;
				break;
			case 5:
				sColorTransform.eMode = COLOR_TO_GRAY_AVERAGE;
				break;
			}
			sColorTransform.SetQueueId(nQueueId);
			sPM.Set(sColorTransform);
			++nQueueId;

			// 2
			int nMedianSize = (sParam.nMedianSize > 0) ? sParam.nMedianSize : 1;
			JET::mod::SFilterParam sFilter_Median;
			sFilter_Median.SetInput1(0, -1);
			sFilter_Median.eMode = FILTER_MEDIAN;
			sFilter_Median.nSizeX = nMedianSize;
			sFilter_Median.nSizeY = nMedianSize;
			sFilter_Median.SetQueueId(nQueueId);
			sPM.Set(sFilter_Median);
			++nQueueId;

			// 3
			if (sParam.nColorTransformMode == 1) {
				for (int k = 0; k < sParam.nCount_ColorExtraction; ++k) {
					JET::mod::SThresholdParam sThreshold_Color;
					sThreshold_Color.SetInput1(2, 2);
					sThreshold_Color.eMode = THRESHOLD_COLOR_TARGET;
					int nB = sParam.vtsColorExtraction[k].nValue_B;
					if (nB < 0) nB = 0;
					if (nB > 255) nB = 255;
					sThreshold_Color.nThreshold_Low = nB;
					int nG = sParam.vtsColorExtraction[k].nValue_G;
					if (nG < 0) nB = 0;
					if (nG > 255) nB = 255;
					sThreshold_Color.nThreshold_High = nG;
					int nR = sParam.vtsColorExtraction[k].nValue_R;
					if (nR < 0) nB = 0;
					if (nR > 255) nB = 255;
					sThreshold_Color.nAutoThreshold = nR;
					int nRange = sParam.vtsColorExtraction[k].nRange;
					if (nRange < 0) nB = 0;
					if (nRange > 255) nB = 255;
					sThreshold_Color.fAlpha = nRange;
					sThreshold_Color.bDark = true;
					sThreshold_Color.SetQueueId(nQueueId);
					sPM.Set(sThreshold_Color);
					++nQueueId;
				}

				if (sParam.nCount_ColorExtraction >= 2) {
					SImageCalculatorParam sCalculator_OR;
					sCalculator_OR.SetInput1(2, 3);
					sCalculator_OR.SetInput2(2, 4);
					sCalculator_OR.eMode = CALCULATOR_OR;
					sCalculator_OR.SetQueueId(nQueueId);
					sPM.Set(sCalculator_OR);
					++nQueueId;

					for (int k = 0; k < sParam.nCount_ColorExtraction - 2; ++k) {
						SImageCalculatorParam sCalculator_OR;
						sCalculator_OR.SetInput1(0, -1);
						sCalculator_OR.SetInput2(2, 5 + k);
						sCalculator_OR.eMode = CALCULATOR_OR;
						sCalculator_OR.SetQueueId(nQueueId);
						sPM.Set(sCalculator_OR);
						++nQueueId;
					}
				}
			}
			else {
				for (int k = 0; k < sParam.nCount_GrayExtraction; ++k) {
					int nRange = sParam.vtsGrayExtraction[k].nRange;
					int nLow = sParam.vtsGrayExtraction[k].nValue_Gray - nRange;
					if (nLow < 0) nLow = 0;
					int nHigh = sParam.vtsGrayExtraction[k].nValue_Gray + nRange;
					if (nHigh > 255) nHigh = 255;
					JET::mod::SThresholdParam sThreshold_Double;
					sThreshold_Double.SetInput1(2, 2);
					sThreshold_Double.eMode = THRESHOLD_DOUBLE;
					sThreshold_Double.bDark = true;
					sThreshold_Double.nThreshold_Low = nLow;
					sThreshold_Double.nThreshold_High = nHigh;
					sThreshold_Double.SetQueueId(nQueueId);
					sPM.Set(sThreshold_Double);
					++nQueueId;
				}

				if (sParam.nCount_GrayExtraction >= 2) {
					SImageCalculatorParam sCalculator_OR;
					sCalculator_OR.SetInput1(2, 3);
					sCalculator_OR.SetInput2(2, 4);
					sCalculator_OR.eMode = CALCULATOR_OR;
					sCalculator_OR.SetQueueId(nQueueId);
					sPM.Set(sCalculator_OR);
					++nQueueId;

					for (int k = 0; k < sParam.nCount_GrayExtraction - 2; ++k) {
						SImageCalculatorParam sCalculator_OR;
						sCalculator_OR.SetInput1(0, -1);
						sCalculator_OR.SetInput2(2, 5);
						sCalculator_OR.eMode = CALCULATOR_OR;
						sCalculator_OR.SetQueueId(nQueueId);
						sPM.Set(sCalculator_OR);
						++nQueueId;
					}
				}
			}

			// 4
			JET::mod::SMorphologParam sMorpholog_CloseOpen;
			int nOpenX = (sParam.nOpenX > 0) ? sParam.nOpenX : 1;
			int nOpenY = (sParam.nOpenY > 0) ? sParam.nOpenY : 1;
			sMorpholog_CloseOpen.SetInput1(0, -1);
			sMorpholog_CloseOpen.eMode = MORPHOLOG_CLOSEOPEN;
			sMorpholog_CloseOpen.eElement = ELEMENT_ELLIPSE;
			sMorpholog_CloseOpen.nSizeX = nOpenX;
			sMorpholog_CloseOpen.nSizeY = nOpenY;
			sMorpholog_CloseOpen.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_CloseOpen);
			++nQueueId;

			// 5
			JET::mod::SMorphologParam sMorpholog_Connect;
			int nConnectX = (sParam.nConnectX > 0) ? sParam.nConnectX : 1;
			int nConnectY = (sParam.nConnectY > 0) ? sParam.nConnectY : 1;
			sMorpholog_Connect.SetInput1(0, -1);
			sMorpholog_Connect.eMode = MORPHOLOG_DILATE;
			sMorpholog_Connect.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Connect.nSizeX = nConnectX;
			sMorpholog_Connect.nSizeY = nConnectY;
			sMorpholog_Connect.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Connect);
			++nQueueId;

			// 6
			JET::mod::SContoursShapeParam sContours_Search;
			sContours_Search.SetInput1(0, -1);
			sContours_Search.eMode = CONTOUR_SEARCH;
			sContours_Search.bBoundingToEdge = true;
			sContours_Search.bRectangle = true;
			sContours_Search.bToEdge = true;
			sContours_Search.nSearchType = 1;
			sContours_Search.nSortType = 1;
			sContours_Search.SetQueueId(nQueueId);
			sPM.Set(sContours_Search);
			++nQueueId;

			// 7
			JET::mod::SContoursShapeParam sContours_Fill;
			sContours_Fill.SetContours1(0, -1);
			sContours_Fill.eMode = CONTOUR_FILL;
			sContours_Fill.SetQueueId(nQueueId);
			sPM.Set(sContours_Fill);
			++nQueueId;

			// 8
			JET::mod::SMorphologParam sMorpholog_Dilate;
			int nDilate_X = sParam.nDilateX;
			if (nDilate_X <= 0) nDilate_X = 1;
			int nDilate_Y = sParam.nDilateY;
			if (nDilate_Y <= 0) nDilate_Y = 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Dilate.eMode = MORPHOLOG_DILATE;
			sMorpholog_Dilate.nSizeX = nDilate_X;
			sMorpholog_Dilate.nSizeY = nDilate_Y;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 9
			JET::mod::SMorphologParam sMorpholog_Erosion;
			int nErosionX = (sParam.nErosionX > 0) ? sParam.nErosionX : 1;
			int nErosionY = (sParam.nErosionY > 0) ? sParam.nErosionY : 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Dilate.eMode = MORPHOLOG_EROSION;
			sMorpholog_Dilate.nSizeX = nErosionX;
			sMorpholog_Dilate.nSizeY = nErosionY;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 10
			JET::mod::SContoursShapeParam sContours_Search2;
			sContours_Search2.SetInput1(0, -1);
			sContours_Search2.eMode = CONTOUR_SEARCH;
			sContours_Search2.bBoundingToEdge = true;
			sContours_Search2.bRectangle = true;
			sContours_Search2.bToEdge = true;
			sContours_Search2.nSearchType = 1;
			sContours_Search2.nSortType = 1;
			sContours_Search2.SetQueueId(nQueueId);
			sPM.Set(sContours_Search2);
			++nQueueId;

			if (!sPM.CreateList()) {
				return false;
			}

			return true;
		}
		bool MeasurementBlackGlue::AdjustmentFindEdgeParameter_Step6_End(const Mat& matConnect, const Mat& matColor, const int& nDeleteMode, const int& nDilateX, const int& nDilateY, const int& nErosionX, const int& nErosionY, Mat& matEnd)
		{
			if (matConnect.empty()) {
				m_strErrorMessage = "第一張影像是空的";
				return false;
			}

			if (matConnect.channels() != 1) {
				m_strErrorMessage = "第一張影像不是灰階影像";
				return false;
			}

			if (matColor.empty()) {
				m_strErrorMessage = "第二張影像是空的";
				return false;
			}

			if (matColor.channels() != 3) {
				m_strErrorMessage = "第二張影像不是彩色影像";
				return false;
			}

			if (matColor.size() != matConnect.size()) {
				m_strErrorMessage = "第一張影像與第二張影像大小不一";
				return false;
			}

			if (nDeleteMode < 1 || nDeleteMode > 3) {
				m_strErrorMessage = "刪除模式設定失敗";
				return false;
			}

			if (nDilateX < 1) {
				m_strErrorMessage = "DilateX 不可小於1";
				return false;
			}

			if (nDilateY < 1) {
				m_strErrorMessage = "DilateY 不可小於1";
				return false;
			}

			if (nErosionX < 1) {
				m_strErrorMessage = "ErosionX 不可小於1";
				return false;
			}

			if (nErosionY < 1) {
				m_strErrorMessage = "ErosionY 不可小於1";
				return false;
			}

			// 白點(255)數量
			int nWhiteCount = countNonZero(matConnect);
			if (nWhiteCount == 0) {
				matEnd = matColor.clone();
				return true;
			}

			// 帶入參數
			SFindEdge_Parameter sFindEdge;
			sFindEdge.nDilateX = nDilateX;
			sFindEdge.nDilateY = nDilateY;
			sFindEdge.nErosionX = nErosionX;
			sFindEdge.nErosionY = nErosionY;	
			sFindEdge.sDeleteMode.nMode = nDeleteMode;

			SProcessModeParam sPM_All;
			SProcessModeParam sPM_End;
			switch(sFindEdge.sDeleteMode.nMode) {
				case 1:
					{
						if (!SetFindEdgeProcess(sFindEdge, sPM_All)) {
							m_strErrorMessage = "流程設定錯誤1";
							return false;
						}

						vector<pair<SInputType, SInputType>> vtsReplace(1);
						vtsReplace[0].first.SetType(2);
						vtsReplace[0].first.SetId(5);
						vtsReplace[0].second.SetType(1);
						vtsReplace[0].second.SetId(1);
						if (!sPM_All.ExtractList(6, sPM_All.GetBaseParameterNumber(), sPM_End, vtsReplace)) {
							m_strErrorMessage = "流程設定錯誤2";
							return false;
						}
					}
				break;
				case 2:
				case 3:
					if (!SetFindEdgeProcess_Adj_Step6_BaseObject(sFindEdge, sPM_All)) {
						m_strErrorMessage = "流程設定錯誤1";
						return false;
					}
					if (!sPM_All.ExtractList(6, sPM_All.GetBaseParameterNumber(), sPM_End)) {
						m_strErrorMessage = "流程設定錯誤2";
						return false;
					}
				break;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matConnect;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_End, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_NG_AdjustmentFindEdgeParameter", "Step6", sPM_End, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_End.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK_AdjustmentFindEdgeParameter", "Step6", sPM_End, vtmatImage, vtsResult);
			}

			int nNumber = sPM_End.GetBaseParameterNumber();
			if (nNumber != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			SContoursParam* psContours = &vtsResult[nNumber - 1].sContours;
			if (psContours == nullptr) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matEnd = matColor.clone();
			switch (sFindEdge.sDeleteMode.nMode) {
			case 1:
				{
					vector<POINT> vtptContoursPos;
					if (!ContoursConvertVector(psContours, vtptContoursPos)) {
						m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
						return false;
					}	
					jet_imagefunction::DrawEdgePoint(matEnd, vtptContoursPos, m_scContours_Glue, 0);
				}
				break;
			case 2:
			case 3:
					jet_imagefunction::DisplayContours(*psContours, true, false, matEnd, false);
				break;
			}

			return true;
		}

		bool MeasurementBlackGlue::AdjustmentFindEdgeParameter_Step6_IC_End(const Mat& matConnect, const Mat& matColor, const int& nMeasuringDirection, const int& nDeleteMode, Mat& matEnd)
		{
			if (matConnect.empty()) {
				m_strErrorMessage = "第一張影像是空的";
				return false;
			}

			if (matConnect.channels() != 1) {
				m_strErrorMessage = "第一張影像不是灰階影像";
				return false;
			}

			if (matColor.empty()) {
				m_strErrorMessage = "第二張影像是空的";
				return false;
			}

			if (matColor.channels() != 3) {
				m_strErrorMessage = "第二張影像不是彩色影像";
				return false;
			}

			if (matColor.size() != matConnect.size()) {
				m_strErrorMessage = "第一張影像與第二張影像大小不一";
				return false;
			}

			if (nMeasuringDirection < 1 || nMeasuringDirection > 4) {
				m_strErrorMessage = "量測方向設定錯誤";
				return false;
			}

			if (nDeleteMode < 1 || nDeleteMode > 3) {
				m_strErrorMessage = "刪除模式設定失敗";
				return false;
			}

			// 白點(255)數量
			int nWhiteCount = countNonZero(matConnect);
			if (nWhiteCount == 0) {
				matEnd = matColor.clone();
				return true;
			}

			// 帶入參數
			SFindEdge_Parameter sFindEdge;
			sFindEdge.sDeleteMode.nMode = nDeleteMode;
			m_sParam_GlueHeight.nMeasuringDirection = nMeasuringDirection;

			SProcessModeParam sPM_All;
			SProcessModeParam sPM_End;
			if (!SetFindEdgeProcess_GlueHeight_IC(sFindEdge, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			if (!sPM_All.ExtractList(6, sPM_All.GetBaseParameterNumber(), sPM_End)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matConnect;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_End, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_NG_AdjustmentFindEdgeParameter_IC", "Step6", sPM_End, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_End.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK_AdjustmentFindEdgeParameter_IC", "Step6", sPM_End, vtmatImage, vtsResult);
			}

			int nNumber = sPM_End.GetBaseParameterNumber();
			if (nNumber != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			SContoursParam* psContours = &vtsResult[nNumber - 1].sContours;
			if (psContours == nullptr) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

#if 1
			vector<POINT> vtptContoursPos;
			if (!ContoursConvertVector(psContours, vtptContoursPos)) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}
			matEnd = matColor.clone();
			jet_imagefunction::DrawEdgePoint(matEnd, vtptContoursPos, m_scContours_Glue, 1);
#else
			matEnd = matColor.clone();
			jet_imagefunction::DisplayContours(*psContours, true, false, matEnd, false);
#endif

			return true;
		}
#pragma endregion

#pragma region SPIL FindGlueEdge Adjustment Parameter

		// Step 1:轉灰階
		bool MeasurementBlackGlue::AdjustmentSpilGluePartParameter_Step1_Gray(const Mat& matColor, const int& nMedianSize, const int& nMode, Mat& matGray)
		{
			if (matColor.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matColor.channels() != 3) {
				m_strErrorMessage = "輸入影像不是彩色格式";
				return false;
			}

			if (nMedianSize < 1) {
				m_strErrorMessage = "MedianSize 不可小於1";
				return false;
			}

			if (nMode < 1 || nMode > 4) {
				m_strErrorMessage = "轉換模式 輸入錯誤";
				return false;
			}

			// 帶入參數
			SFindEdge_Parameter sFindEdge;
			sFindEdge.nMedianSize = nMedianSize;
			sFindEdge.nColorTransformMode = nMode;

			SProcessModeParam sPM_All;
			if (!SetSpilGluePartProcess(sFindEdge, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Gray;
			if (!sPM_All.ExtractList(1, 2, sPM_Gray)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matColor;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Gray, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_NG_AdjustmentSpilGluePartParameter", "Step1", sPM_Gray, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Gray.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK_AdjustmentSpilGluePartParameter", "Step1", sPM_Gray, vtmatImage, vtsResult);
			}

			int nCount = sPM_Gray.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matGray = vtsResult[nCount - 1].matImage.clone();

			return true;
		}

		// Step 2: 彩色影像抽色 
		bool MeasurementBlackGlue::AdjustmentSpilGluePartParameter_Step2_Threshold(const Mat& matGray, const int& nCount, const vector<SGrayExtraction>& vtsGrayExtraction, Mat& matThreshold)
		{
			if (matGray.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matGray.channels() != 1) {
				m_strErrorMessage = "輸入影像不是灰階";
				return false;
			}

			if (nCount != vtsGrayExtraction.size()) {
				m_strErrorMessage = "灰階抽色組數錯誤";
				return false;
			}

			// 帶入參數
			SFindEdge_Parameter sFindEdge;
			sFindEdge.nCount_GrayExtraction = nCount;
			sFindEdge.vtsGrayExtraction = vtsGrayExtraction;

			SProcessModeParam sPM_All;
			if (!SetSpilGluePartProcess(sFindEdge, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			int nEndStep = 3 + (nCount - 1) * 2;
			vector<pair<SInputType, SInputType>> vtsReplace(1);
			vtsReplace[0].first.SetType(2);
			vtsReplace[0].first.SetId(2);
			vtsReplace[0].second.SetType(1);
			vtsReplace[0].second.SetId(1);
			SProcessModeParam sPM_Threshold;
			if (!sPM_All.ExtractList(3, nEndStep, sPM_Threshold, vtsReplace)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matGray;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Threshold, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_NG_AdjustmentSpilGluePartParameter_Step2", "Step2_Threshold", sPM_Threshold, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Threshold.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK_AdjustmentSpilGluePartParameter_Step2", "Step2_Threshold", sPM_Threshold, vtmatImage, vtsResult);
			}

			int nNumber = sPM_Threshold.GetBaseParameterNumber();
			if (nNumber != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matThreshold = vtsResult[nNumber - 1].matImage.clone();

			return true;
		}

		// Step 3: 取出 IC區域 
		bool MeasurementBlackGlue::AdjustmentSpilGluePartParameter_Step3_IC(const Mat& matThreshold, const RECT& rectIC, Mat& matIC)
		{
			if (matThreshold.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matThreshold.channels() != 1) {
				m_strErrorMessage = "輸入影像不是灰階";
				return false;
			}

			// 帶入參數
			SFindEdge_Parameter sFindEdge;

			SProcessModeParam sPM_All;
			if (!SetSpilGluePartProcess(sFindEdge, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}


			return true;
		}

		// Step 3: 取出膠區域
		bool MeasurementBlackGlue::AdjustmentSpilGlueParameter_Step3_Area(const Mat& matThreshold, const int& nCount, const int& nDilateX, const int& nDilateY, Mat& matGlueThreshold)
		{
			if (matThreshold.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matThreshold.channels() != 1) {
				m_strErrorMessage = "輸入影像不是灰階";
				return false;
			}

			if (nCount < 1) {
				m_strErrorMessage = "區塊數不能小於1";
				return false;
			}

			if (nDilateX < 1) {
				m_strErrorMessage = "OpenX 小於1";
				return false;
			}

			if (nDilateY < 1) {
				m_strErrorMessage = "OpenY 小於1";
				return false;
			}

			// 帶入參數
			SFindEdge_Parameter sFindEdge;
			sFindEdge.nConnectX = nDilateX;
			sFindEdge.nConnectY = nDilateY;
			sFindEdge.sDeleteMode.nMode = 1;
			sFindEdge.sDeleteMode.nMaxCount = nCount;
/*
			SProcessModeParam sPM_All;
			if (!SetSpilGlueProcess(sFindEdge, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Grab;
			if (!sPM_All.ExtractList(3, 6, sPM_Grab)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matThreshold;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Grab, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_NG_AdjustmentSPIL_GlueEdgeParameter", "Step3_Grab", sPM_Grab, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Grab.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK_AdjustmentSPIL_GlueEdgeParameter", "Step3_Grab", sPM_Grab, vtmatImage, vtsResult);
			}

			int nNumber = sPM_Grab.GetBaseParameterNumber();
			if (nNumber != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matGlueThreshold = vtsResult[nNumber - 1].matImage.clone();
*/
			return true;
		}

		// Step 4: 影像強化
		bool MeasurementBlackGlue::AdjustmentSpilGlueParameter_Step4_Enhance(const Mat& matColor, const Mat& matGlueThreshold, const SEnhanceMode& sEnhance, Mat& matEnhance)
		{
			if (matColor.empty()) {
				m_strErrorMessage = "原始影像是空的";
				return false;
			}

			if (matColor.channels() != 3) {
				m_strErrorMessage = "原始影像不是彩色影格式";
				return false;
			}

			if (matGlueThreshold.empty()) {
				m_strErrorMessage = "膠區域影像是空的";
				return false;
			}

			if (matGlueThreshold.channels() != 1) {
				m_strErrorMessage = "膠區域影像不是灰階";
				return false;
			}

			string strData;
			if (!sEnhance.Check(strData)) {
				m_strErrorMessage = "強化參數 " + strData;
				return false;
			}

			// 帶入參數
			SFindEdge_Parameter sFindEdge;
			sFindEdge.sEnhanceMode.sKmean.nGroupNumber = sEnhance.sKmean.nGroupNumber;
			sFindEdge.sEnhanceMode.sKmean.nDarkId= sEnhance.sKmean.nDarkId;
			sFindEdge.sEnhanceMode.sKmean.nLightId = sEnhance.sKmean.nLightId;
/*
			SProcessModeParam sPM_All;
			if (!SetSpilGlueProcess(sFindEdge, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			vector<pair<SInputType, SInputType>> vtsReplace(1);
			vtsReplace[0].first.SetType(2);
			vtsReplace[0].first.SetId(3);
			vtsReplace[0].second.SetType(1);
			vtsReplace[0].second.SetId(2);

			SProcessModeParam sPM_Enhance;
			if (!sPM_All.ExtractList(7, 12, sPM_Enhance, vtsReplace)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(2);
			vtmatImage[0] = matColor;
			vtmatImage[1] = matGlueThreshold;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Enhance, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_NG_AdjustmentSPIL_GlueEdgeParameter", "Step3_Grab", sPM_Enhance, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Enhance.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK_AdjustmentSPIL_GlueEdgeParameter", "Step3_Grab", sPM_Enhance, vtmatImage, vtsResult);
			}

			int nNumber = sPM_Enhance.GetBaseParameterNumber();
			if (nNumber != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matEnhance = vtsResult[nNumber - 1].matImage.clone();
*/
			return true;
		}
#pragma endregion

#pragma region SPIL Part Adjustment Parameter

		// Step 2:二值化
		bool MeasurementBlackGlue::AdjustmentSpilPartParameter_Step2_Threshold(const Mat& matGray, const RECT& rectIC, const bool& bDark, const int& nThreshold, Mat& matThreshold, Mat& matIC)
		{
			if (matGray.empty()) {
				m_strErrorMessage = "輸入影像是空的";
				return false;
			}

			if (matGray.channels() != 1) {
				m_strErrorMessage = "輸入影像不是灰階格式";
				return false;
			}

			if (nThreshold < 1 || nThreshold > 255) {
				m_strErrorMessage = "閥值輸入錯誤";
				return false;
			}

			// 帶入參數
			SFindEdge_Parameter sFindEdge;
			sFindEdge.nCount_GrayExtraction = 1;
			sFindEdge.vtsGrayExtraction.resize(sFindEdge.nCount_GrayExtraction);
			sFindEdge.vtsGrayExtraction[0].nValue_Gray = nThreshold;
			if (bDark) {
				sFindEdge.vtsGrayExtraction[0].nRange = 0;
			}
			else {
				sFindEdge.vtsGrayExtraction[0].nRange = 255;
			}

			SProcessModeParam sPM_All;
			if (!SetSpilPartProcess(sFindEdge, rectIC, sPM_All)) {
				m_strErrorMessage = "流程設定錯誤1";
				return false;
			}

			SProcessModeParam sPM_Threshold;
			if (!sPM_All.ExtractList(2, 5, sPM_Threshold)) {
				m_strErrorMessage = "流程設定錯誤2";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vtmatImage[0] = matGray;
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, sPM_Threshold, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 1) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName + "_NG_AdjustmentSpilPartParameter", "Step2", sPM_Threshold, vtmatImage, vtsResult);
				}
				m_strErrorMessage = "執行失敗-" + sPM_Threshold.GetErrorMessage();
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName + "_OK_AdjustmentSpilPartParameter", "Step2", sPM_Threshold, vtmatImage, vtsResult);
			}

			int nCount = sPM_Threshold.GetBaseParameterNumber();
			if (nCount != vtsResult.size()) {
				m_strErrorMessage = "執行失敗, 請確認參數是否正確, 以及上一步結果是否正確";
				return false;
			}

			matThreshold = vtsResult[0].matImage.clone();
			matIC = vtsResult[nCount - 1].matImage.clone();

			return true;
		}
#pragma endregion

		// 檢測輸入參數
		bool MeasurementBlackGlue::Check_Input(const vector<Mat>& vtInputImage, SMeasurementBlackGlue_Parameter& sParam)
		{
			if (vtInputImage.size() != 4) {
				m_strErrorMessage = "輸入影像張數錯誤, 請輸入4張影像";
				return false;
			}

			for (int k = 0; k < 4; ++k) {
				if (vtInputImage[k].empty()) {
					m_strErrorMessage = "輸入的第" + to_string(k+1) + "張影像是空的";
					return false;
				}

				if (vtInputImage[k].channels() != 3) {
					m_strErrorMessage = "第" + to_string(k + 1) + "張影像不是彩色影像";
					return false;
				}
			}

			for (int k = 0; k < 4; ++k) {
				for (int h = 0; h < 4; ++h) {
					if (k == h) continue;
					if (vtInputImage[k].size() != vtInputImage[h].size()) {
						m_strErrorMessage = "第" + to_string(k + 1) + "張影像與第" + to_string(h + 1) + "張影像的大小不同";
						return false;
					}
				}
			}

			if (!sParam.Check(vtInputImage[0].rows, vtInputImage[0].cols, m_strErrorMessage)) {
				return false;
			}

			m_matGlue = vtInputImage[0].clone();
			m_matBoard = vtInputImage[1].clone();
			m_matThermal = vtInputImage[2].clone();
			m_matCoating = vtInputImage[3].clone();
			SaveImage(0, m_matGlue, "Glue_Src", m_nExtensionType);
			SaveImage(0, m_matBoard, "Board_Src", m_nExtensionType);
			SaveImage(0, m_matThermal, "Thermal_Src", m_nExtensionType);
			SaveImage(0, m_matCoating, "Coating_Src", m_nExtensionType);

			m_sParam = sParam;

			return true;
		}

		bool MeasurementBlackGlue::Check_Input_Flux(const Mat& matImage, SMeasurementFluxArea_Parameter& sParam)
		{
			if (matImage.empty()) {
				m_strErrorMessage = "輸入影像為空";
				return false;
			}

			if (!sParam.Check(m_strErrorMessage)) {
				return false;
			}

			m_matGlue = matImage.clone();
			SaveImage(0, m_matGlue, "Src", m_nExtensionType);
			m_sParam_FluxArea = sParam;

			return true;
		}

		bool MeasurementBlackGlue::Check_Input_HeightRatio(const Mat& matImage, SMeasurementGlueHeightRatio_Parameter& sParam)
		{
			if (matImage.empty()) {
				m_strErrorMessage = "輸入影像為空";
				return false;
			}

			if (!sParam.Check(m_strErrorMessage)) {
				return false;
			}

			m_matGlue = matImage.clone();
			SaveImage(0, m_matGlue, "GlueHeight_Src", m_nExtensionType);
			m_sParam_GlueHeight = sParam;

			return true;
		}

		bool MeasurementBlackGlue::Check_Input_GlueArea(const Mat& matImage, SMeasurementGlueArea_Parameter& sParam)
		{
			if (matImage.empty()) {
				m_strErrorMessage = "輸入影像為空";
				return false;
			}

			if (!sParam.Check(m_strErrorMessage)) {
				return false;
			}

			m_matGlue = matImage.clone();
			SaveImage(0, m_matGlue, "Src", m_nExtensionType);
			m_sParam_GlueArea = sParam;

			return true;
		}

		// 變數初始化
		void MeasurementBlackGlue::Initial()
		{
			m_nShiftIndex = -1;
			m_nContoursIndex = -1;
			m_nGlueROIIndex = -1;

			if (!m_matEdgeMask.empty()) {
				m_matEdgeMask.release();
			}

			if (!m_matBoardEdge.empty()) {
				m_matBoardEdge.release();
			}
			
			if (!m_matThermalGlueEdge.empty()) {
				m_matThermalGlueEdge.release();
			}

			m_vtdCoefficient.clear();
			m_vtdDieSlope.clear();
			m_vtdDieIntercept.clear();
			m_vtptBoardEdgePoint_H.clear();
			m_vtptBoardEdgePoint_V.clear();
			m_vt2ptICEdgePoint.clear();
			m_sPM_FindGlue.Clear();
			m_sPM_FindBoard.Clear();
			m_sPM_FindCoating.Clear();
			m_sPM_FindThermal_IC.Clear();

			m_sResult.Initial();
		}

		void MeasurementBlackGlue::Initial_FluxArea()
		{
			m_nGlueROIIndex = -1;
			m_nContoursIndex = -1;
			if (!m_matEdgeMask.empty()) {
				m_matEdgeMask.release();
			}

			// 記錄投影+濾波 影像
			if (!m_matBoard.empty()) {
				m_matBoard.release();
			}

			m_vt2ptICEdgePoint.clear();

			// 找膠面積邊界用
			m_sPM_FindGlue.Clear();

			m_sResult_FluxArea.Initial();
		}

		void MeasurementBlackGlue::Initial_GlueHeight()
		{
			m_nGlueROIIndex = -1;
			m_nContoursIndex = -1;
			if (!m_matEdgeMask.empty()) {
				m_matEdgeMask.release();
			}

			m_vt2ptICEdgePoint.clear();

			// 找膠邊界用
			m_sPM_FindThermal_Glue.Clear();

			// 找IC邊界用
			m_sPM_FindThermal_IC.Clear();

			m_sResult_GlueHeight.Initial();

			//m_sResult_GlueHeight.sHeight.nBlockCount = m_sParam_GlueHeight.nBlockCount;
			//m_sResult_GlueHeight.sHeight.vtsBlock.resize(m_sParam_GlueHeight.nBlockCount);
			//for (int k = 0; k < m_sResult_GlueHeight.sHeight.nBlockCount; ++k) {
			//	m_sResult_GlueHeight.sHeight.vtsBlock[k].sFirst.fDist_ICtoGlue = m_sParam_GlueHeight.fIC_Height;
			//	m_sResult_GlueHeight.sHeight.vtsBlock[k].sFirst.fGlueHeight = 0.0;
			//	m_sResult_GlueHeight.sHeight.vtsBlock[k].sFirst.fHeightRatio = 0.0;
			//}
		}

		void MeasurementBlackGlue::Initial_GlueArea()
		{
			m_nGlueROIIndex = -1;
			m_nContoursIndex = -1;
			if (!m_matEdgeMask.empty()) {
				m_matEdgeMask.release();
			}

			// 記錄投影+濾波 影像
			if (!m_matBoard.empty()) {
				m_matBoard.release();
			}		

			m_vt2ptICEdgePoint.clear();

			// 找膠面積邊界用
			m_sPM_FindGlue.Clear();

			// 找 IC範圍用
			m_sPM_FindBoard.Clear();

			m_sResult_GlueArea.Initial();
		}
#pragma region GlueEdge

		// 設定找黑膠邊緣的處理流程
		// Method1=>找膠本體(抽色), Method2=>找膠輪廓(轉灰階+二值化)
		bool MeasurementBlackGlue::SetGlueProcess_FindContours(const SGlueEdge_Parameter& sParam, SProcessModeParam& sPM)
		{
			bool bResult = false;
			switch (sParam.nFindEdgeMethod)
			{
			case 1:
				bResult = SetGlueProcess_FindContours_Method1(sParam, sPM);
				if (!bResult) {
					m_strErrorMessage = "AdjustmentGlueParameter_Method1-Error";
				}
				break;
			case 2:
				bResult = SetGlueProcess_FindContours_Method2(sParam, sPM);
				if (!bResult) {
					m_strErrorMessage = "AdjustmentGlueParameter_Method2-Error";
				}
				break;
			default:
				return false;
			}

			return bResult;
		}
		bool MeasurementBlackGlue::SetGlueProcess_FindContours_Method1(const SGlueEdge_Parameter& sParam, SProcessModeParam& sPM)
		{
			int nQueueId = 1;
			sPM.Clear();

			// 1
			JET::mod::SThresholdParam sThreshold_Color;
			sThreshold_Color.SetInput1(1, 1);
			sThreshold_Color.eMode = THRESHOLD_COLOR_TARGET;
			sThreshold_Color.nThreshold_Low = sParam.nGlueValue_B;
			sThreshold_Color.nThreshold_High = sParam.nGlueValue_G;
			sThreshold_Color.nAutoThreshold = sParam.nGlueValue_R;
			sThreshold_Color.fAlpha = sParam.nGlueValueTolerance;
			sThreshold_Color.bDark = true;
			sThreshold_Color.SetQueueId(nQueueId);
			sPM.Set(sThreshold_Color);
			++nQueueId;

			// 2
			JET::mod::SMorphologParam sMorpholog_Open;
			int nOpenX = sParam.nFilter_OpenX;
			if (nOpenX <= 0) nOpenX = 1;
			int nOpenY = sParam.nFilter_OpenY;
			if (nOpenY <= 0) nOpenY = 1;
			sMorpholog_Open.SetInput1(0, -1);
			sMorpholog_Open.eMode = MORPHOLOG_OPENING;
			sMorpholog_Open.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Open.nSizeX = nOpenX;
			sMorpholog_Open.nSizeY = nOpenY;
			sMorpholog_Open.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Open);
			++nQueueId;

			// 3
			JET::mod::SMorphologParam sMorpholog_Close;
			int nConnectX = sParam.nFilter_ConnectX;
			if (nConnectX <= 0) nConnectX = 1;
			int nConnectY = sParam.nFilter_ConnectY;
			if (nConnectY <= 0) nConnectY = 1;
			sMorpholog_Close.SetInput1(0, -1);
			sMorpholog_Close.eMode = MORPHOLOG_CLOSEOPEN;
			sMorpholog_Close.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Close.nSizeX = nConnectX;
			sMorpholog_Close.nSizeY = nConnectY;
			sMorpholog_Close.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Close);
			++nQueueId;

			// 4
			JET::mod::SContoursShapeParam sContours_Search;
			sContours_Search.SetInput1(0, -1);
			sContours_Search.eMode = CONTOUR_SEARCH;
			sContours_Search.bBoundingToEdge = false;
			sContours_Search.bRectangle = true;
			sContours_Search.bToEdge = true;
			sContours_Search.nSearchType = 1;
			sContours_Search.nSortType = 1;
			sContours_Search.SetQueueId(nQueueId);
			sPM.Set(sContours_Search);
			++nQueueId;

			// 5
			int nMinDist = 3;
			JET::mod::SDeleteObjectParam sDelete_KeepROI_AwayBorder;
			sDelete_KeepROI_AwayBorder.SetContours1(0, -1);
			sDelete_KeepROI_AwayBorder.bEdge = false;
			switch (sParam.nLocation)
			{
			case 1: // 左上
				sDelete_KeepROI_AwayBorder.eMode_Height = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nHeight_Min = 2;// 上
				sDelete_KeepROI_AwayBorder.nHeight_Max = nMinDist;
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 4;// 左
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			case 2: // 上
				sDelete_KeepROI_AwayBorder.eMode_Height = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nHeight_Min = 2;// 上
				sDelete_KeepROI_AwayBorder.nHeight_Max = nMinDist;
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 4;// 左
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			case 3:// 右上
				sDelete_KeepROI_AwayBorder.eMode_Height = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nHeight_Min = 2;// 上
				sDelete_KeepROI_AwayBorder.nHeight_Max = nMinDist;
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 3;// 右
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			case 4:// 右
				sDelete_KeepROI_AwayBorder.eMode_Height = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nHeight_Min = 2;// 上
				sDelete_KeepROI_AwayBorder.nHeight_Max = nMinDist;
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 3;// 右
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			case 5:// 右下
				sDelete_KeepROI_AwayBorder.eMode_Height = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nHeight_Min = 1;// 下
				sDelete_KeepROI_AwayBorder.nHeight_Max = nMinDist;
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 3;// 右
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			case 6:// 下
				sDelete_KeepROI_AwayBorder.eMode_Height = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nHeight_Min = 1;// 下
				sDelete_KeepROI_AwayBorder.nHeight_Max = nMinDist;
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 3;// 右
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			case 7:// 左下
				sDelete_KeepROI_AwayBorder.eMode_Height = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nHeight_Min = 1;// 下
				sDelete_KeepROI_AwayBorder.nHeight_Max = nMinDist;
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 4;// 左
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			case 8:// 左
				sDelete_KeepROI_AwayBorder.eMode_Height = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nHeight_Min = 1;// 下
				sDelete_KeepROI_AwayBorder.nHeight_Max = nMinDist;
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 4;// 左
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			case 9:
			case 10:
			case 11:
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 5;// 左
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			}
			sDelete_KeepROI_AwayBorder.nOutputMode = 2;
			sDelete_KeepROI_AwayBorder.SetQueueId(nQueueId);
			sPM.Set(sDelete_KeepROI_AwayBorder);
			++nQueueId;

			// 6
			JET::mod::SDeleteObjectParam sDelete_Size_LesstMore;
			int nGlue_ROI_Width = (sParam.rectGlueROI.right - sParam.rectGlueROI.left + 1);
			if (nGlue_ROI_Width <= 0) nGlue_ROI_Width = 1;
			int nGlue_ROI_Height = (sParam.rectGlueROI.bottom - sParam.rectGlueROI.top + 1);
			if (nGlue_ROI_Height <= 0) nGlue_ROI_Height = 1;
			sDelete_Size_LesstMore.SetContours1(0, -1);
			sDelete_Size_LesstMore.eMode_Width = DELETE_SIZE_LESSTHAN_MORETHAN;
			sDelete_Size_LesstMore.eMode_Height = DELETE_SIZE_LESSTHAN_MORETHAN;
			sDelete_Size_LesstMore.nWidth_Min = nGlue_ROI_Width * (1 - sParam.fGlueROI_Tolerance);
			if (sDelete_Size_LesstMore.nWidth_Min <= 0) sDelete_Size_LesstMore.nWidth_Min = 1;
			sDelete_Size_LesstMore.nWidth_Max = nGlue_ROI_Width * (1 + sParam.fGlueROI_Tolerance);
			sDelete_Size_LesstMore.nHeight_Min = nGlue_ROI_Height * (1 - sParam.fGlueROI_Tolerance);
			if (sDelete_Size_LesstMore.nHeight_Min <= 0) sDelete_Size_LesstMore.nHeight_Min = 1;
			sDelete_Size_LesstMore.nHeight_Max = nGlue_ROI_Height * (1 + sParam.fGlueROI_Tolerance);
			sDelete_Size_LesstMore.nOutputMode = 2;
			sDelete_Size_LesstMore.SetQueueId(nQueueId);
			sPM.Set(sDelete_Size_LesstMore);
			++nQueueId;

			// 7
			JET::mod::SDeleteObjectParam sDelete_Dist;
			float fMaxRatio = (1.0 + sParam.fGlueROI_Tolerance);
			if (fMaxRatio < 1.0) return false;
			if (fMaxRatio >= 2.0) fMaxRatio = 1.9;
			switch (sParam.nLocation)
			{
			case 1: // 左上
				sDelete_Dist.eMode_Width = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nWidth_Min = 3;
				sDelete_Dist.nWidth_Max = sParam.rectGlueROI.right*fMaxRatio;
				if (sDelete_Dist.nWidth_Max <= 0) return false;

				sDelete_Dist.eMode_Height = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nHeight_Min = 1;
				sDelete_Dist.nHeight_Max = sParam.rectGlueROI.bottom*fMaxRatio;
				if (sDelete_Dist.nHeight_Max <= 0) return false;
				break;
			case 2: // 上
				sDelete_Dist.eMode_Height = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nHeight_Min = 1;
				sDelete_Dist.nHeight_Max = sParam.rectGlueROI.bottom*fMaxRatio;
				if (sDelete_Dist.nHeight_Max <= 0) return false;
				break;
			case 3:// 右上
				sDelete_Dist.eMode_Width = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nWidth_Min = 4;
				sDelete_Dist.nWidth_Max = (m_matGlue.cols - sParam.rectGlueROI.left)*fMaxRatio;
				if (sDelete_Dist.nWidth_Max <= 0) return false;

				sDelete_Dist.eMode_Height = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nHeight_Min = 1;
				sDelete_Dist.nHeight_Max = sParam.rectGlueROI.bottom*fMaxRatio;
				if (sDelete_Dist.nHeight_Max <= 0) return false;
				break;
			case 4:// 右
				sDelete_Dist.eMode_Width = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nWidth_Min = 4;
				sDelete_Dist.nWidth_Max = (m_matGlue.cols - sParam.rectGlueROI.left)*fMaxRatio;
				if (sDelete_Dist.nWidth_Max <= 0) return false;
				break;
			case 5:// 右下
				sDelete_Dist.eMode_Width = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nWidth_Min = 4;
				sDelete_Dist.nWidth_Max = (m_matGlue.cols - sParam.rectGlueROI.left)*fMaxRatio;
				if (sDelete_Dist.nWidth_Max <= 0) return false;

				sDelete_Dist.eMode_Height = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nHeight_Min = 2;
				sDelete_Dist.nHeight_Max = (m_matGlue.rows - sParam.rectGlueROI.top)*fMaxRatio;
				if (sDelete_Dist.nHeight_Max <= 0) return false;
				break;
			case 6:// 下
				sDelete_Dist.eMode_Height = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nHeight_Min = 2;
				sDelete_Dist.nHeight_Max = (m_matGlue.rows - sParam.rectGlueROI.top)*fMaxRatio;
				if (sDelete_Dist.nHeight_Max <= 0) return false;
				break;
			case 7:// 左下
				sDelete_Dist.eMode_Width = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nWidth_Min = 3;
				sDelete_Dist.nWidth_Max = sParam.rectGlueROI.right*fMaxRatio;
				if (sDelete_Dist.nWidth_Max <= 0) return false;

				sDelete_Dist.eMode_Height = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nHeight_Min = 2;
				sDelete_Dist.nHeight_Max = (m_matGlue.rows - sParam.rectGlueROI.top)*fMaxRatio;
				if (sDelete_Dist.nHeight_Max <= 0) return false;
				break;
			case 8:// 左
				sDelete_Dist.eMode_Width = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nWidth_Min = 3;
				sDelete_Dist.nWidth_Max = sParam.rectGlueROI.right*fMaxRatio;
				if (sDelete_Dist.nWidth_Max <= 0) return false;
				break;
			case 9:
			case 10:
			case 11:
			{
				sDelete_Dist.eMode_Width = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nWidth_Min = 5;
				int nDist_L = sParam.rectGlueROI.left;
				int nDist_R = m_matGlue.cols - sParam.rectGlueROI.right;
				int nDist_T = sParam.rectGlueROI.top;
				int nDist_B = m_matGlue.rows - sParam.rectGlueROI.bottom;
				int nMinW = (nDist_L > nDist_R) ? nDist_L : nDist_R;
				int nMinH = (nDist_T > nDist_B) ? nDist_T : nDist_B;
				sDelete_Dist.nWidth_Max = (nMinW > nMinH) ? nMinW*fMaxRatio : nMinH*fMaxRatio;
				if (sDelete_Dist.nWidth_Max <= 0) return false;
			}
				break;
			}
			sDelete_Dist.SetContours1(0, -1);
			sDelete_Dist.nOutputMode = 2;
			sDelete_Dist.SetQueueId(nQueueId);
			sPM.Set(sDelete_Dist);
			++nQueueId;

			// 8
			JET::mod::SDeleteObjectParam sDelete_MaxArea;
			sDelete_MaxArea.SetContours1(0, -1);
			if (sParam.nLocation <= 9) {
				sDelete_MaxArea.eMode_Width = KEEP_ROI_MAXAREA;
				sDelete_MaxArea.nHeight_Max = 1;
				sDelete_MaxArea.nOutputMode = 4;
			}
			else {
				//nWidth_Min=目標ROI寬; nHeight_Min=目標ROI高; nWidth_Max=模式,1=>不限制,2=>長寬都不能大於目標值,3=>長寬都不能小於目標值, nHeight_Max=輸出的數量
				sDelete_MaxArea.eMode_Width = KEEP_ROI_THE_SIMILAR_SIZE;
				int nROI_W = sParam.rectGlueROI.right - sParam.rectGlueROI.left;
				if (nROI_W < 0) nROI_W = 1;
				int nROI_H = sParam.rectGlueROI.bottom - sParam.rectGlueROI.top;
				if (nROI_H < 0) nROI_H = 1;
				sDelete_MaxArea.nWidth_Min = nROI_W;
				sDelete_MaxArea.nHeight_Min = nROI_H;
				sDelete_MaxArea.nWidth_Max = 1;
				sDelete_MaxArea.nHeight_Max = 2;
			}
			sDelete_MaxArea.SetQueueId(nQueueId);
			sPM.Set(sDelete_MaxArea);
			++nQueueId;

			// 9
			JET::mod::SContoursShapeParam sContours_Fill;
			sContours_Fill.SetContours1(0, -1);
			if (sParam.nLocation == 9) {
				sContours_Fill.eMode = CONTOUR_CONVEX_FILL;
			}
			else {
				sContours_Fill.eMode = CONTOUR_FILL;
			}
			sContours_Fill.SetQueueId(nQueueId);
			sPM.Set(sContours_Fill);
			++nQueueId;

			// 10
			JET::mod::SMorphologParam sMorpholog_Open2;
			int nOpenX2 = sParam.nFilter_OpenX2;
			if (nOpenX2 <= 0) nOpenX2 = 1;
			int nOpenY2 = sParam.nFilter_OpenY2;
			if (nOpenY2 <= 0) nOpenY2 = 1;
			sMorpholog_Open2.SetInput1(0, -1);
			sMorpholog_Open2.eMode = MORPHOLOG_OPENING;
			sMorpholog_Open2.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Open2.nSizeX = nOpenX2;
			sMorpholog_Open2.nSizeY = nOpenY2;
			sMorpholog_Open2.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Open2);
			++nQueueId;

			// 11
			JET::mod::SMorphologParam sMorpholog_Dilate;
			int nDilate_X = sParam.nFilter_DilateX;
			if (nDilate_X <= 0) nDilate_X = 1;
			int nDilate_Y = sParam.nFilter_DilateY;
			if (nDilate_Y <= 0) nDilate_Y = 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Dilate.eMode = MORPHOLOG_DILATE;
			sMorpholog_Dilate.nSizeX = nDilate_X;
			sMorpholog_Dilate.nSizeY = nDilate_Y;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 12
			JET::mod::SMorphologParam sMorpholog_Erosion;
			int nErosionX = sParam.nFilter_ErosionX;
			if (nErosionX <= 0) nErosionX = 1;
			int nErosionY = sParam.nFilter_ErosionY;
			if (nErosionY <= 0) nErosionY = 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Dilate.eMode = MORPHOLOG_EROSION;
			sMorpholog_Dilate.nSizeX = nErosionX;
			sMorpholog_Dilate.nSizeY = nErosionY;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 13
			JET::mod::SContoursShapeParam sContours_Search2;
			sContours_Search2.SetInput1(0, -1);
			sContours_Search2.eMode = CONTOUR_SEARCH;
			sContours_Search2.bBoundingToEdge = false;
			sContours_Search2.bRectangle = true;
			sContours_Search2.bToEdge = true;
			sContours_Search2.nSearchType = 1;
			sContours_Search2.nSortType = 1;
			sContours_Search2.SetQueueId(nQueueId);
			sPM.Set(sContours_Search2);
			++nQueueId;

			// 14
			JET::mod::SDeleteObjectParam sDelete_MaxArea2;
			sDelete_MaxArea2.SetContours1(0, -1);
			sDelete_MaxArea2.eMode_Width = KEEP_ROI_MAXAREA;
			sDelete_MaxArea2.nOutputMode = 3;
			if (sParam.nLocation <= 9) {
				sDelete_MaxArea2.nHeight_Max = 1;
			}
			else {
				sDelete_MaxArea2.nHeight_Max = 2;
			}
			sDelete_MaxArea2.SetQueueId(nQueueId);
			sPM.Set(sDelete_MaxArea2);
			++nQueueId;

			if (!sPM.CreateList()) {
				return false;
			}

			return true;
		}
		bool MeasurementBlackGlue::SetGlueProcess_FindContours_Method2(const SGlueEdge_Parameter& sParam, SProcessModeParam& sPM)
		{
			int nQueueId = 1;
			sPM.Clear();

			// 1
			JET::mod::SColorTransformParam sColor_ToGray_R;
			sColor_ToGray_R.SetInput1(1, 1);
			sColor_ToGray_R.eMode = COLOR_TO_GRAY_BGR;
			sColor_ToGray_R.nID = 2;
			sColor_ToGray_R.SetQueueId(nQueueId);
			sPM.Set(sColor_ToGray_R);
			++nQueueId;

			// 2
			JET::mod::SFilterParam sFilter_Median;
			sFilter_Median.SetInput1(0, -1);
			sFilter_Median.eMode = FILTER_MEDIAN;
			sFilter_Median.nSizeX = sParam.nFilter_MedianX;
			sFilter_Median.nSizeY = sParam.nFilter_MedianY;
			sFilter_Median.SetQueueId(nQueueId);
			sPM.Set(sFilter_Median);
			++nQueueId;

			// 3
			JET::mod::SFilterParam sFilter_Edge;
			sFilter_Edge.SetInput1(0, -1);
			sFilter_Edge.eMode = FILTER_EDGE;
			sFilter_Edge.eElement = ELEMENT_RECT;
			sFilter_Edge.nSizeX = 7;
			sFilter_Edge.nSizeY = 7;
			sFilter_Edge.nTimes = 1;
			sFilter_Edge.SetQueueId(nQueueId);
			sPM.Set(sFilter_Edge);
			++nQueueId;

			// 4
			JET::mod::SThresholdParam sThreshold_MaxEntropy;
			sThreshold_MaxEntropy.SetInput1(0, -1);
			sThreshold_MaxEntropy.bDark = sParam.bDark;
			sThreshold_MaxEntropy.eMode = THRESHOLD_SINGLE;
			sThreshold_MaxEntropy.nThreshold_Low = sParam.nThreshold;
			sThreshold_MaxEntropy.SetQueueId(nQueueId);
			sPM.Set(sThreshold_MaxEntropy);
			++nQueueId;

			// 5
			JET::mod::SMorphologParam sMorpholog_Open;
			int nOpenX = sParam.nFilter_OpenX;
			if (nOpenX <= 0) nOpenX = 1;
			int nOpenY = sParam.nFilter_OpenY;
			if (nOpenY <= 0) nOpenY = 1;
			sMorpholog_Open.SetInput1(0, -1);
			sMorpholog_Open.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Open.eMode = MORPHOLOG_OPENING;
			sMorpholog_Open.nSizeX = nOpenX;
			sMorpholog_Open.nSizeY = nOpenY;
			sMorpholog_Open.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Open);
			++nQueueId;

			// 6
			JET::mod::SMorphologParam sMorpholog_Connect;
			int nConnect_X = sParam.nFilter_ConnectX;
			if (nConnect_X <= 0) nConnect_X = 1;
			int nConnect_Y = sParam.nFilter_ConnectY;
			if (nConnect_Y <= 0) nConnect_Y = 1;
			sMorpholog_Connect.SetInput1(0, -1);
			sMorpholog_Connect.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Connect.eMode = MORPHOLOG_DILATE;
			sMorpholog_Connect.nSizeX = nConnect_X;
			sMorpholog_Connect.nSizeY = nConnect_Y;
			sMorpholog_Connect.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Connect);
			++nQueueId;

			// 7
			JET::mod::SContoursShapeParam sContours_Search;
			sContours_Search.SetInput1(0, -1);
			sContours_Search.eMode = CONTOUR_SEARCH;
			sContours_Search.bBoundingToEdge = false;
			sContours_Search.bRectangle = true;
			sContours_Search.bToEdge = true;
			sContours_Search.nSearchType = 1;
			sContours_Search.nSortType = 1;
			sContours_Search.SetQueueId(nQueueId);
			sPM.Set(sContours_Search);
			++nQueueId;

			// 8
			int nMinDist = 3;
			JET::mod::SDeleteObjectParam sDelete_KeepROI_AwayBorder;
			sDelete_KeepROI_AwayBorder.SetContours1(0, -1);
			sDelete_KeepROI_AwayBorder.bEdge = false;
			switch (sParam.nLocation)
			{
			case 1: // 左上
				sDelete_KeepROI_AwayBorder.eMode_Height = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nHeight_Min = 2;// 上
				sDelete_KeepROI_AwayBorder.nHeight_Max = nMinDist;
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 4;// 左
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			case 2: // 上
				sDelete_KeepROI_AwayBorder.eMode_Height = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nHeight_Min = 2;// 上
				sDelete_KeepROI_AwayBorder.nHeight_Max = nMinDist;
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 4;// 左
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			case 3:// 右上
				sDelete_KeepROI_AwayBorder.eMode_Height = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nHeight_Min = 2;// 上
				sDelete_KeepROI_AwayBorder.nHeight_Max = nMinDist;
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 3;// 右
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			case 4:// 右
				sDelete_KeepROI_AwayBorder.eMode_Height = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nHeight_Min = 2;// 上
				sDelete_KeepROI_AwayBorder.nHeight_Max = nMinDist;
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 3;// 右
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			case 5:// 右下
				sDelete_KeepROI_AwayBorder.eMode_Height = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nHeight_Min = 1;// 下
				sDelete_KeepROI_AwayBorder.nHeight_Max = nMinDist;
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 3;// 右
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			case 6:// 下
				sDelete_KeepROI_AwayBorder.eMode_Height = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nHeight_Min = 1;// 下
				sDelete_KeepROI_AwayBorder.nHeight_Max = nMinDist;
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 3;// 右
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			case 7:// 左下
				sDelete_KeepROI_AwayBorder.eMode_Height = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nHeight_Min = 1;// 下
				sDelete_KeepROI_AwayBorder.nHeight_Max = nMinDist;
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 4;// 左
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			case 8:// 左
				sDelete_KeepROI_AwayBorder.eMode_Height = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nHeight_Min = 1;// 下
				sDelete_KeepROI_AwayBorder.nHeight_Max = nMinDist;
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 4;// 左
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			case 9:
				sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
				sDelete_KeepROI_AwayBorder.nWidth_Min = 5;// 左
				sDelete_KeepROI_AwayBorder.nWidth_Max = nMinDist;
				break;
			}
			sDelete_KeepROI_AwayBorder.nOutputMode = 2;
			sDelete_KeepROI_AwayBorder.SetQueueId(nQueueId);
			sPM.Set(sDelete_KeepROI_AwayBorder);
			++nQueueId;

			// 9
			JET::mod::SDeleteObjectParam sDelete_Size_LesstMore;
			int nGlue_ROI_Width = (sParam.rectGlueROI.right - sParam.rectGlueROI.left + 1);
			if (nGlue_ROI_Width <= 0) nGlue_ROI_Width = 1;
			int nGlue_ROI_Height = (sParam.rectGlueROI.bottom - sParam.rectGlueROI.top + 1);
			if (nGlue_ROI_Height <= 0) nGlue_ROI_Height = 1;
			sDelete_Size_LesstMore.SetContours1(0, -1);
			sDelete_Size_LesstMore.eMode_Width = DELETE_SIZE_LESSTHAN_MORETHAN;
			sDelete_Size_LesstMore.eMode_Height = DELETE_SIZE_LESSTHAN_MORETHAN;
			sDelete_Size_LesstMore.nWidth_Min = nGlue_ROI_Width * (1 - sParam.fGlueROI_Tolerance);
			if (sDelete_Size_LesstMore.nWidth_Min <= 0) sDelete_Size_LesstMore.nWidth_Min = 1;
			sDelete_Size_LesstMore.nWidth_Max = nGlue_ROI_Width * (1 + sParam.fGlueROI_Tolerance);
			sDelete_Size_LesstMore.nHeight_Min = nGlue_ROI_Height * (1 - sParam.fGlueROI_Tolerance);
			if (sDelete_Size_LesstMore.nHeight_Min <= 0) sDelete_Size_LesstMore.nHeight_Min = 1;
			sDelete_Size_LesstMore.nHeight_Max = nGlue_ROI_Height * (1 + sParam.fGlueROI_Tolerance);
			sDelete_Size_LesstMore.nOutputMode = 2;
			sDelete_Size_LesstMore.SetQueueId(nQueueId);
			sPM.Set(sDelete_Size_LesstMore);
			++nQueueId;

			// 10
			JET::mod::SDeleteObjectParam sDelete_Dist;
			float fMaxRatio = (1.0 + sParam.fGlueROI_Tolerance);
			if (fMaxRatio < 1.0) return false;
			if (fMaxRatio >= 2.0) fMaxRatio = 1.9;
			switch (sParam.nLocation)
			{
			case 1: // 左上
				sDelete_Dist.eMode_Width = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nWidth_Min = 3;
				sDelete_Dist.nWidth_Max = sParam.rectGlueROI.right*fMaxRatio;
				if (sDelete_Dist.nWidth_Max <= 0) return false;

				sDelete_Dist.eMode_Height = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nHeight_Min = 1;
				sDelete_Dist.nHeight_Max = sParam.rectGlueROI.bottom*fMaxRatio;
				if (sDelete_Dist.nHeight_Max <= 0) return false;
				break;
			case 2: // 上
				sDelete_Dist.eMode_Height = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nHeight_Min = 1;
				sDelete_Dist.nHeight_Max = sParam.rectGlueROI.bottom*fMaxRatio;
				if (sDelete_Dist.nHeight_Max <= 0) return false;
				break;
			case 3:// 右上
				sDelete_Dist.eMode_Width = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nWidth_Min = 4;
				sDelete_Dist.nWidth_Max = (m_matGlue.cols - sParam.rectGlueROI.left)*fMaxRatio;
				if (sDelete_Dist.nWidth_Max <= 0) return false;

				sDelete_Dist.eMode_Height = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nHeight_Min = 1;
				sDelete_Dist.nHeight_Max = sParam.rectGlueROI.bottom*fMaxRatio;
				if (sDelete_Dist.nHeight_Max <= 0) return false;
				break;
			case 4:// 右
				sDelete_Dist.eMode_Width = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nWidth_Min = 4;
				sDelete_Dist.nWidth_Max = (m_matGlue.cols - sParam.rectGlueROI.left)*fMaxRatio;
				if (sDelete_Dist.nWidth_Max <= 0) return false;
				break;
			case 5:// 右下
				sDelete_Dist.eMode_Width = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nWidth_Min = 4;
				sDelete_Dist.nWidth_Max = (m_matGlue.cols - sParam.rectGlueROI.left)*fMaxRatio;
				if (sDelete_Dist.nWidth_Max <= 0) return false;

				sDelete_Dist.eMode_Height = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nHeight_Min = 2;
				sDelete_Dist.nHeight_Max = (m_matGlue.rows - sParam.rectGlueROI.top)*fMaxRatio;
				if (sDelete_Dist.nHeight_Max <= 0) return false;
				break;
			case 6:// 下
				sDelete_Dist.eMode_Height = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nHeight_Min = 2;
				sDelete_Dist.nHeight_Max = (m_matGlue.rows - sParam.rectGlueROI.top)*fMaxRatio;
				if (sDelete_Dist.nHeight_Max <= 0) return false;
				break;
			case 7:// 左下
				sDelete_Dist.eMode_Width = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nWidth_Min = 3;
				sDelete_Dist.nWidth_Max = sParam.rectGlueROI.right*fMaxRatio;
				if (sDelete_Dist.nWidth_Max <= 0) return false;

				sDelete_Dist.eMode_Height = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nHeight_Min = 2;
				sDelete_Dist.nHeight_Max = (m_matGlue.rows - sParam.rectGlueROI.top)*fMaxRatio;
				if (sDelete_Dist.nHeight_Max <= 0) return false;
				break;
			case 8:// 左
				sDelete_Dist.eMode_Width = KEEP_ROI_NEAR_BORDER;
				sDelete_Dist.nWidth_Min = 3;
				sDelete_Dist.nWidth_Max = sParam.rectGlueROI.right*fMaxRatio;
				if (sDelete_Dist.nWidth_Max <= 0) return false;
				break;
			case 9:
				{
					sDelete_Dist.eMode_Width = KEEP_ROI_NEAR_BORDER;
					sDelete_Dist.nWidth_Min = 5;
					int nDist_L = sParam.rectGlueROI.left;
					int nDist_R = m_matGlue.cols - sParam.rectGlueROI.right;
					int nDist_T = sParam.rectGlueROI.top;
					int nDist_B = m_matGlue.rows - sParam.rectGlueROI.bottom;
					int nMinW = (nDist_L > nDist_R) ? nDist_L : nDist_R;
					int nMinH = (nDist_T > nDist_B) ? nDist_T : nDist_B;
					sDelete_Dist.nWidth_Max = (nMinW > nMinH) ? nMinW*fMaxRatio : nMinH*fMaxRatio;
					if (sDelete_Dist.nWidth_Max <= 0) return false;
				}
				break;
			}
			sDelete_Dist.SetContours1(0, -1);
			sDelete_Dist.nOutputMode = 2;
			sDelete_Dist.SetQueueId(nQueueId);
			sPM.Set(sDelete_Dist);
			++nQueueId;

			// 11
			JET::mod::SDeleteObjectParam sDelete_MaxArea;
			sDelete_MaxArea.SetContours1(0, -1);

			if (sParam.nLocation <= 9) {
				sDelete_MaxArea.eMode_Width = KEEP_ROI_MAXAREA;
				sDelete_MaxArea.nHeight_Max = 1;
				sDelete_MaxArea.nOutputMode = 3;
			}
			else {
				//nWidth_Min=目標ROI寬; nHeight_Min=目標ROI高; nWidth_Max=模式,1=>不限制,2=>長寬都不能大於目標值,3=>長寬都不能小於目標值, nHeight_Max=輸出的數量
				sDelete_MaxArea.eMode_Width = KEEP_ROI_THE_SIMILAR_SIZE;
				int nROI_W = sParam.rectGlueROI.right - sParam.rectGlueROI.left;
				if (nROI_W < 0) nROI_W = 1;
				int nROI_H = sParam.rectGlueROI.bottom - sParam.rectGlueROI.top;
				if (nROI_H < 0) nROI_H = 1;
				sDelete_MaxArea.nWidth_Min = nROI_W;
				sDelete_MaxArea.nHeight_Min = nROI_H;
				sDelete_MaxArea.nWidth_Max = 1;
				sDelete_MaxArea.nHeight_Max = 2;
			}
			sDelete_MaxArea.SetQueueId(nQueueId);
			sPM.Set(sDelete_MaxArea);
			++nQueueId;

			// 12
			JET::mod::SContoursShapeParam sContours_Fill;
			sContours_Fill.SetContours1(0, -1);
			if (sParam.nLocation == 9) {
				sContours_Fill.eMode = CONTOUR_CONVEX_FILL;
			}
			else {
				sContours_Fill.eMode = CONTOUR_FILL;
			}
			sContours_Fill.SetQueueId(nQueueId);
			sPM.Set(sContours_Fill);
			++nQueueId;

			// 13
			JET::mod::SMorphologParam sMorpholog_Open2;
			int nOpenX2 = sParam.nFilter_OpenX2;
			if (nOpenX2 <= 0) nOpenX2 = 1;
			int nOpenY2 = sParam.nFilter_OpenY2;
			if (nOpenY2 <= 0) nOpenY2 = 1;
			sMorpholog_Open2.SetInput1(0, -1);
			sMorpholog_Open2.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Open2.eMode = MORPHOLOG_OPENING;
			sMorpholog_Open2.nSizeX = nOpenX2;
			sMorpholog_Open2.nSizeY = nOpenY2;
			sMorpholog_Open2.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Open2);
			++nQueueId;

			// 14
			JET::mod::SMorphologParam sMorpholog_Dilate;
			int nDilate_X = sParam.nFilter_DilateX;
			if (nDilate_X <= 0) nDilate_X = 1;
			int nDilate_Y = sParam.nFilter_DilateY;
			if (nDilate_Y <= 0) nDilate_Y = 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Dilate.eMode = MORPHOLOG_DILATE;
			sMorpholog_Dilate.nSizeX = nDilate_X;
			sMorpholog_Dilate.nSizeY = nDilate_Y;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 15
			JET::mod::SMorphologParam sMorpholog_Erosion;
			int nErosionX = sParam.nFilter_ErosionX;
			if (nErosionX <= 0) nErosionX = 1;
			int nErosionY = sParam.nFilter_ErosionY;
			if (nErosionY <= 0) nErosionY = 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Dilate.eMode = MORPHOLOG_EROSION;
			sMorpholog_Dilate.nSizeX = nErosionX;
			sMorpholog_Dilate.nSizeY = nErosionY;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 16
			JET::mod::SContoursShapeParam sContours_Search2;
			sContours_Search2.SetInput1(0, -1);
			sContours_Search2.eMode = CONTOUR_SEARCH;
			sContours_Search2.bBoundingToEdge = false;
			sContours_Search2.bRectangle = true;
			sContours_Search2.bToEdge = true;
			sContours_Search2.nSearchType = 1;
			sContours_Search2.nSortType = 1;
			sContours_Search2.SetQueueId(nQueueId);
			sPM.Set(sContours_Search2);
			++nQueueId;

			// 17
			JET::mod::SDeleteObjectParam sDelete_MaxArea2;
			sDelete_MaxArea2.SetContours1(0, -1);
			sDelete_MaxArea2.eMode_Width = KEEP_ROI_MAXAREA;
			if (sParam.nLocation <= 9) {
				sDelete_MaxArea2.nHeight_Max = 1;
			}
			else {
				sDelete_MaxArea2.nHeight_Max = 2;
			}
			sDelete_MaxArea2.nOutputMode = 3;
			sDelete_MaxArea2.SetQueueId(nQueueId);
			sPM.Set(sDelete_MaxArea2);
			++nQueueId;

			if (!sPM.CreateList()) {
				return false;
			}

			return true;
		}
		bool MeasurementBlackGlue::RunProcess_FindGlueContours()
		{
			if (!SetGlueProcess_FindContours(m_sParam.sGlue, m_sPM_FindGlue) || m_matGlue.empty() || m_matGlue.channels() != 3) {
				return false;
			}

			vector<Mat> vtmatImage(1);
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			vtmatImage[0] = m_matGlue;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, m_sPM_FindGlue, vtsResult))
			{
				if (m_bSaveImage && m_nSaveLevel > 0) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName, "Error_FindGlueContours_"+ m_sPM_FindGlue.GetErrorMessage(), m_sPM_FindGlue, vtmatImage, vtsResult, 2);
				}
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 1) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName, "OK_FindGlueContours", m_sPM_FindGlue, vtmatImage, vtsResult, 2);
			}
			
			int nCount = m_sPM_FindGlue.GetBaseParameterNumber();
			if (vtsResult.size() != nCount) {
				return false;
			}

			if (m_sParam.sGlue.nLocation <= 9) {
				// 輸出 ROI
				int nId = nCount - 1;
				m_sResult.sGlueEdge.rectROI.left = vtsResult[nId].cvROI.x;
				m_sResult.sGlueEdge.rectROI.right = vtsResult[nId].cvROI.br().x;
				m_sResult.sGlueEdge.rectROI.top = vtsResult[nId].cvROI.y;
				m_sResult.sGlueEdge.rectROI.bottom = vtsResult[nId].cvROI.br().y;
				if (!jet_imagefunction::Check_RECT(m_matGlue.rows, m_matGlue.cols, m_sResult.sGlueEdge.rectROI)) {
					return false;
				}

				// 輸出輪廓
				SContoursParam* psContours = &vtsResult[nId].sContours;
				if (psContours == nullptr) return false;
				if (!ContoursConvertVector(psContours, m_sResult.sGlueEdge.vtptContoursPos)) {
					return false;
				}
				m_sResult.sGlueEdge.nContoursCount = m_sResult.sGlueEdge.vtptContoursPos.size();

				if (m_bSaveImage) {
					Mat matDisplay;
					if (!DisplayGlueContours(1, 1, matDisplay)) {
						return false;
					}
					SaveImage(0, matDisplay, "GlueContours", m_nExtensionType);
				}

				// 畫輪廓遮罩
				if (!DrawMask(1, 255, m_matEdgeMask)) {
					m_strErrorMessage = "Draw GlueMask Error";
					Save_Parameter();
					return false;
				}
			}
			else {

				int nId = nCount - 1;
				m_sResult.sGlueEdge.rectROI.left = vtsResult[nId].vtcvROI[0].x;
				m_sResult.sGlueEdge.rectROI.right = vtsResult[nId].vtcvROI[0].br().x;
				m_sResult.sGlueEdge.rectROI.top = vtsResult[nId].vtcvROI[0].y;
				m_sResult.sGlueEdge.rectROI.bottom = vtsResult[nId].vtcvROI[0].br().y;

				m_sResult.sGlueEdge2.rectROI.left = vtsResult[nId].vtcvROI[1].x;
				m_sResult.sGlueEdge2.rectROI.right = vtsResult[nId].vtcvROI[1].br().x;
				m_sResult.sGlueEdge2.rectROI.top = vtsResult[nId].vtcvROI[1].y;
				m_sResult.sGlueEdge2.rectROI.bottom = vtsResult[nId].vtcvROI[1].br().y;

				// 輸出輪廓
				SContoursParam* psContours = &vtsResult[nId].sContours;
				if (psContours == nullptr) return false;
				if (!ContoursConvertVector(psContours, m_sResult.sGlueEdge.vtptContoursPos, m_sResult.sGlueEdge2.vtptContoursPos)) {
					return false;
				}
				m_sResult.sGlueEdge.nContoursCount = m_sResult.sGlueEdge.vtptContoursPos.size();
				m_sResult.sGlueEdge2.nContoursCount = m_sResult.sGlueEdge2.vtptContoursPos.size();
				m_sResult.sGlueEdge2.bEnable = true;

				if (m_bSaveImage) {
					Mat matDisplay;
					if (!DisplayGlueContours(1, 1, matDisplay)) {
						return false;
					}
					SaveImage(0, matDisplay, "GlueContours", m_nExtensionType);
				}

				// 畫輪廓遮罩
				if (!DrawMask(1, 255, m_matEdgeMask)) {
					m_strErrorMessage = "Draw GlueMask Error";
					Save_Parameter();
					return false;
				}

				if (!DrawMask(2, 255, m_matEdgeMask)) {
					m_strErrorMessage = "Draw GlueMask Error";
					Save_Parameter();
					return false;
				}
			}

			return true;
		}
#pragma endregion

#pragma region BoardEdge

		// 設定找板邊直線的處理流程
		bool MeasurementBlackGlue::SetBoardProcess_FindContours(const SBoardEdge_Parameter& sBoard, const int& nLocation, const int& nImageW, const int& nImageH, SProcessModeParam& sPM)
		{
			bool bResult = false;
			switch (sBoard.nFindEdgeMethod)
			{
			case 1:
				bResult = SetBoardProcess_FindContours_Method1(sBoard, nLocation, nImageW, nImageH, sPM);
				if (!bResult) {
					m_strErrorMessage = "AdjustmentBoardParameter_Method1-Error";
				}
				break;
			default:
				return false;
			}

			return bResult;
		}

		bool MeasurementBlackGlue::SetBoardProcess_FindContours_Method1(const SBoardEdge_Parameter& sBoard, const int& nLocation, const int& nImageW, const int& nImageH, SProcessModeParam& sPM)
		{
			int nQueueId = 1;
			sPM.Clear();

			// 1
			JET::mod::SFilterParam sFilter_Median;
			int nMedian_X = sBoard.nMedianX;
			if (nMedian_X <= 0) nMedian_X = 1;
			int nMedian_Y = sBoard.nMedianY;
			if (nMedian_Y <= 0) nMedian_Y = 1;
			sFilter_Median.SetInput1(1, 1);
			sFilter_Median.eMode = FILTER_MEDIAN;
			sFilter_Median.nSizeX = nMedian_X;
			sFilter_Median.nSizeY = nMedian_Y;
			sFilter_Median.SetQueueId(nQueueId);
			sPM.Set(sFilter_Median);
			++nQueueId;

			// 2
			JET::mod::SThresholdParam sThreshold_Color1;
			sThreshold_Color1.SetInput1(2, 1);
			sThreshold_Color1.eMode = THRESHOLD_COLOR_TARGET;
			sThreshold_Color1.nThreshold_Low = sBoard.nBoardValue_B;
			sThreshold_Color1.nThreshold_High = sBoard.nBoardValue_G;
			sThreshold_Color1.nAutoThreshold = sBoard.nBoardValue_R;
			sThreshold_Color1.fAlpha = sBoard.nBoardValueTolerance;
			sThreshold_Color1.bDark = true;
			sThreshold_Color1.SetQueueId(nQueueId);
			sPM.Set(sThreshold_Color1);
			++nQueueId;

			// 第2組抽色
			JET::mod::SThresholdParam sThreshold_Color2;
			SImageCalculatorParam sCalculator_OR;
			if (sBoard.bEnable_Color2) {
				// 3			
				sThreshold_Color2.SetInput1(2, 1);
				sThreshold_Color2.eMode = THRESHOLD_COLOR_TARGET;
				sThreshold_Color2.nThreshold_Low = sBoard.nBoardValue_B2;
				sThreshold_Color2.nThreshold_High = sBoard.nBoardValue_G2;
				sThreshold_Color2.nAutoThreshold = sBoard.nBoardValue_R2;
				sThreshold_Color2.fAlpha = sBoard.nBoardValueTolerance2;
				sThreshold_Color2.bDark = true;
				sThreshold_Color2.SetQueueId(nQueueId);
				sPM.Set(sThreshold_Color2);
				++nQueueId;

				// 4	
				sCalculator_OR.SetInput1(sThreshold_Color1);
				sCalculator_OR.SetInput2(0, -1);
				sCalculator_OR.eMode = CALCULATOR_OR;
				sCalculator_OR.SetQueueId(nQueueId);
				sPM.Set(sCalculator_OR);
				++nQueueId;
			}

			// 第三組抽色
			JET::mod::SThresholdParam sThreshold_Color3;
			SImageCalculatorParam sCalculator_OR2;
			if (sBoard.bEnable_Color3) {
				// 5	
				sThreshold_Color3.SetInput1(2, 1);
				sThreshold_Color3.eMode = THRESHOLD_COLOR_TARGET;
				sThreshold_Color3.nThreshold_Low = sBoard.nBoardValue_B3;
				sThreshold_Color3.nThreshold_High = sBoard.nBoardValue_G3;
				sThreshold_Color3.nAutoThreshold = sBoard.nBoardValue_R3;
				sThreshold_Color3.fAlpha = sBoard.nBoardValueTolerance3;
				sThreshold_Color3.bDark = true;
				sThreshold_Color3.SetQueueId(nQueueId);
				sPM.Set(sThreshold_Color3);
				++nQueueId;

				// 6
				if (sBoard.bEnable_Color2) {
					sCalculator_OR2.SetInput1(sCalculator_OR);
					sCalculator_OR2.SetInput2(0, -1);
				}
				else {
					sCalculator_OR2.SetInput1(sThreshold_Color1);
					sCalculator_OR2.SetInput2(0, -1);
				}
				sCalculator_OR2.eMode = CALCULATOR_OR;
				sCalculator_OR2.SetQueueId(nQueueId);
				sPM.Set(sCalculator_OR2);
				++nQueueId;
			}

			// 第四組抽色
			if (sBoard.bEnable_Color4) {
				// 7
				JET::mod::SThresholdParam sThreshold_Color4;
				sThreshold_Color4.SetInput1(2, 1);
				sThreshold_Color4.eMode = THRESHOLD_COLOR_TARGET;
				sThreshold_Color4.nThreshold_Low = sBoard.nBoardValue_B4;
				sThreshold_Color4.nThreshold_High = sBoard.nBoardValue_G4;
				sThreshold_Color4.nAutoThreshold = sBoard.nBoardValue_R4;
				sThreshold_Color4.fAlpha = sBoard.nBoardValueTolerance4;
				sThreshold_Color4.bDark = true;
				sThreshold_Color4.SetQueueId(nQueueId);
				sPM.Set(sThreshold_Color4);
				++nQueueId;

				// 8
				SImageCalculatorParam sCalculator_OR3;
				if (sBoard.bEnable_Color3) {
					sCalculator_OR3.SetInput1(sCalculator_OR2);
					sCalculator_OR3.SetInput2(0, -1);
				}
				else if (sBoard.bEnable_Color2) {
					sCalculator_OR3.SetInput1(sCalculator_OR);
					sCalculator_OR3.SetInput2(0, -1);
				}
				else {
					sCalculator_OR3.SetInput1(sThreshold_Color1);
					sCalculator_OR3.SetInput2(0, -1);
				}
				sCalculator_OR3.eMode = CALCULATOR_OR;
				sCalculator_OR3.SetQueueId(nQueueId);
				sPM.Set(sCalculator_OR3);
				++nQueueId;
			}

			// 9
			JET::mod::SMorphologParam sMorpholog_Open;
			int nOpen_X = sBoard.nOpenX;
			if (nOpen_X <= 0) nOpen_X = 1;
			int nOpen_Y = sBoard.nOpenY;
			if (nOpen_Y <= 0) nOpen_Y = 1;
			sMorpholog_Open.SetInput1(0, -1);
			sMorpholog_Open.eMode = MORPHOLOG_OPENING;
			sMorpholog_Open.eElement = ELEMENT_RECT;
			sMorpholog_Open.nSizeX = nOpen_X;
			sMorpholog_Open.nSizeY = nOpen_Y;
			sMorpholog_Open.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Open);
			++nQueueId;

			// 10
			JET::mod::SMorphologParam sMorpholog_Connect;
			int nConnect_X = sBoard.nConnectX;
			if (nConnect_X <= 0) nConnect_X = 1;
			int nConnect_Y = sBoard.nConnectY;
			if (nConnect_Y <= 0) nConnect_Y = 1;
			sMorpholog_Connect.SetInput1(0, -1);
			sMorpholog_Connect.eMode = MORPHOLOG_DILATE;
			sMorpholog_Connect.eElement = ELEMENT_RECT;
			sMorpholog_Connect.nSizeX = nConnect_X;
			sMorpholog_Connect.nSizeY = nConnect_Y;
			sMorpholog_Connect.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Connect);
			++nQueueId;

			// 11
			JET::mod::SMorphologParam sMorpholog_Dilate;
			int nDilate_X = sBoard.nDilateX;
			if (nDilate_X <= 0) nDilate_X = 1;
			int nDilate_Y = sBoard.nDilateY;
			if (nDilate_Y <= 0) nDilate_Y = 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Dilate.eMode = MORPHOLOG_DILATE;
			sMorpholog_Dilate.nSizeX = nDilate_X;
			sMorpholog_Dilate.nSizeY = nDilate_Y;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 12
			JET::mod::SMorphologParam sMorpholog_Erosion;
			int nErosionX = sBoard.nErosionX;
			if (nErosionX <= 0) nErosionX = 1;
			int nErosionY = sBoard.nErosionY;
			if (nErosionY <= 0) nErosionY = 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Dilate.eMode = MORPHOLOG_EROSION;
			sMorpholog_Dilate.nSizeX = nErosionX;
			sMorpholog_Dilate.nSizeY = nErosionY;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 13
			JET::mod::SContoursShapeParam sContours_Search;
			sContours_Search.SetInput1(0, -1);
			sContours_Search.eMode = CONTOUR_SEARCH;
			sContours_Search.bBoundingToEdge = false;
			sContours_Search.bRectangle = true;
			sContours_Search.bToEdge = true;
			sContours_Search.nSearchType = 1;
			sContours_Search.nSortType = 1;
			sContours_Search.SetQueueId(nQueueId);
			sPM.Set(sContours_Search);
			++nQueueId;

			// 14
			int nMinWidth = static_cast<int>(nImageW * 0.5);
			if (nMinWidth < 1) nMinWidth = 1;
			int nMinHeight = static_cast<int>(nImageH * 0.5);
			if (nMinHeight < 1) nMinHeight = 1;
			JET::mod::SDeleteObjectParam sDelete_Size_LessThan;
			sDelete_Size_LessThan.SetContours1(0, -1);
			switch (nLocation)
			{
			case 1:
			case 3:
			case 5:
			case 7:
			case 9:
			case 10:
				sDelete_Size_LessThan.eMode_Width = DELETE_SIZE_LESSTHAN;
				sDelete_Size_LessThan.eMode_Height = DELETE_SIZE_LESSTHAN;
				sDelete_Size_LessThan.nWidth_Min = nMinWidth;
				sDelete_Size_LessThan.nHeight_Min = nMinHeight;
				break;

			case 2:
			case 6:
				sDelete_Size_LessThan.eMode_Width = DELETE_SIZE_LESSTHAN;
				sDelete_Size_LessThan.nWidth_Min = nMinWidth;
				break;

			case 4:
			case 8:
				sDelete_Size_LessThan.eMode_Height = DELETE_SIZE_LESSTHAN;
				sDelete_Size_LessThan.nHeight_Min = nMinHeight;
				break;
			}
			sDelete_Size_LessThan.nHeight_Max = 1;
			sDelete_Size_LessThan.nOutputMode = 2;
			sDelete_Size_LessThan.SetQueueId(nQueueId);
			sPM.Set(sDelete_Size_LessThan);
			++nQueueId;

			// 15
			JET::mod::SDeleteObjectParam sDelete_KeepROI_Nearest;
			sDelete_KeepROI_Nearest.SetContours1(0, -1);
			switch (nLocation)
			{
			case 1:
			case 2:
			case 3:
				sDelete_KeepROI_Nearest.eMode_Height = KEEP_ROI_NEAREST_BORDER;
				sDelete_KeepROI_Nearest.nHeight_Min = 1;
				break;

			case 4:
				sDelete_KeepROI_Nearest.eMode_Width = KEEP_ROI_NEAREST_BORDER;
				sDelete_KeepROI_Nearest.nWidth_Min = 4;
				break;

			case 5:
			case 6:
			case 7:
				sDelete_KeepROI_Nearest.eMode_Height = KEEP_ROI_NEAREST_BORDER;
				sDelete_KeepROI_Nearest.nHeight_Min = 2;
				break;

			case 8:
				sDelete_KeepROI_Nearest.eMode_Width = KEEP_ROI_NEAREST_BORDER;
				sDelete_KeepROI_Nearest.nWidth_Min = 3;
				break;

			case 9:
			case 10:
				sDelete_KeepROI_Nearest.eMode_Width = KEEP_ROI_NEAREST_BORDER;
				sDelete_KeepROI_Nearest.nWidth_Min = 5;
				break;
			}

			sDelete_KeepROI_Nearest.nHeight_Max = 1;
			sDelete_KeepROI_Nearest.nOutputMode = 3;
			sDelete_KeepROI_Nearest.SetQueueId(nQueueId);
			sPM.Set(sDelete_KeepROI_Nearest);
			++nQueueId;

			if (!sPM.CreateList()) {
				return false;
			}

			return true;
		}
		bool MeasurementBlackGlue::SetBoardProcess_FindContours_Method2(const SBoardEdge_Parameter& sBoard, const int& nLocation, const int& nImageW, const int& nImageH, SProcessModeParam& sPM)
		{
			int nQueueId = 1;
			sPM.Clear();

			// 1
			JET::mod::SThresholdParam sThreshold_Color;
			sThreshold_Color.SetInput1(1, 1);
			sThreshold_Color.eMode = THRESHOLD_COLOR_TARGET;
			sThreshold_Color.nThreshold_Low = sBoard.nBoardValue_B;
			sThreshold_Color.nThreshold_High = sBoard.nBoardValue_G;
			sThreshold_Color.nAutoThreshold = sBoard.nBoardValue_R;
			sThreshold_Color.fAlpha = sBoard.nBoardValueTolerance;
			sThreshold_Color.bDark = true;
			sThreshold_Color.SetQueueId(nQueueId);
			sPM.Set(sThreshold_Color);
			++nQueueId;

			// 2
			JET::mod::SMorphologParam sMorpholog_Close;
			sMorpholog_Close.SetInput1(0, -1);
			sMorpholog_Close.eMode = MORPHOLOG_CLOSEOPEN;
			sMorpholog_Close.eElement = ELEMENT_RECT;
			sMorpholog_Close.nSizeX = sBoard.nConnectX;
			sMorpholog_Close.nSizeY = sBoard.nConnectY;
			sMorpholog_Close.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Close);
			++nQueueId;

			// 3
			JET::mod::SContoursShapeParam sContours_Search;
			sContours_Search.SetInput1(0, -1);
			sContours_Search.eMode = CONTOUR_SEARCH;
			sContours_Search.bBoundingToEdge = false;
			sContours_Search.bRectangle = true;
			sContours_Search.bToEdge = true;
			sContours_Search.nSearchType = 1;
			sContours_Search.nSortType = 1;
			sContours_Search.SetQueueId(nQueueId);
			sPM.Set(sContours_Search);
			++nQueueId;

			// 4
			int nMinWidth = static_cast<int>(nImageW * 0.5);
			if (nMinWidth < 1) nMinWidth = 1;
			int nMinHeight = static_cast<int>(nImageH * 0.5);
			if (nMinHeight < 1) nMinHeight = 1;
			JET::mod::SDeleteObjectParam sDelete_Size_LessThan;
			sDelete_Size_LessThan.SetContours1(0, -1);
			switch (nLocation)
			{
			case 1:
			case 3:
			case 5:
			case 7:
				sDelete_Size_LessThan.eMode_Width = DELETE_SIZE_LESSTHAN;
				sDelete_Size_LessThan.eMode_Height = DELETE_SIZE_LESSTHAN;
				sDelete_Size_LessThan.nWidth_Min = nMinWidth;
				sDelete_Size_LessThan.nHeight_Min = nMinHeight;
				break;

			case 2:
			case 6:
				sDelete_Size_LessThan.eMode_Width = DELETE_SIZE_LESSTHAN;
				sDelete_Size_LessThan.nWidth_Min = nMinWidth;
				break;

			case 4:
			case 8:
				sDelete_Size_LessThan.eMode_Height = DELETE_SIZE_LESSTHAN;
				sDelete_Size_LessThan.nHeight_Min = nMinHeight;
				break;
			}
			sDelete_Size_LessThan.nOutputMode = 2;
			sDelete_Size_LessThan.SetQueueId(nQueueId);
			sPM.Set(sDelete_Size_LessThan);
			++nQueueId;

			// 5
			JET::mod::SDeleteObjectParam sDelete_KeepROI_Nearest;
			sDelete_KeepROI_Nearest.SetContours1(0, -1);
			switch (nLocation)
			{
			case 1:
			case 2:
			case 3:
				sDelete_KeepROI_Nearest.eMode_Height = KEEP_ROI_NEAREST_BORDER;
				sDelete_KeepROI_Nearest.nHeight_Min = 1;
				break;

			case 4:
				sDelete_KeepROI_Nearest.eMode_Width = KEEP_ROI_NEAREST_BORDER;
				sDelete_KeepROI_Nearest.nWidth_Min = 4;
				break;

			case 5:
			case 6:
			case 7:
				sDelete_KeepROI_Nearest.eMode_Height = KEEP_ROI_NEAREST_BORDER;
				sDelete_KeepROI_Nearest.nHeight_Min = 2;
				break;

			case 8:
				sDelete_KeepROI_Nearest.eMode_Width = KEEP_ROI_NEAREST_BORDER;
				sDelete_KeepROI_Nearest.nWidth_Min = 3;
				break;
			}
			sDelete_KeepROI_Nearest.nHeight_Max = 1;
			sDelete_KeepROI_Nearest.nOutputMode = 3;
			sDelete_KeepROI_Nearest.SetQueueId(nQueueId);
			sPM.Set(sDelete_KeepROI_Nearest);
			++nQueueId;

			if (!sPM.CreateList()) {
				return false;
			}

			return true;
		}
		bool MeasurementBlackGlue::RunBoardProcess_FindContours()
		{
			if (!SetBoardProcess_FindContours(m_sParam.sBoard, m_sParam.sGlue.nLocation, m_matBoard.cols, m_matBoard.rows, m_sPM_FindBoard) || m_matBoard.empty() || m_matBoard.channels() != 3) {
				return false;
			}

			vector<Mat> vtmatImage(1);
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			vtmatImage[0] = m_matBoard;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, m_sPM_FindBoard, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 0) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName, "Error_FindBorderContours", m_sPM_FindBoard, vtmatImage, vtsResult, 2);
				}
				return false;
			}

			// 取出輪廓
			SContoursParam* psContours = &vtsResult[m_sPM_FindBoard.GetBaseParameterNumber() - 1].sContours;
			if (psContours == nullptr) return false;

			// 輸出輪廓
			if (!ContoursConvertVector(psContours, m_sResult.sBoardEdge.vtptContoursPos)) {
				return false;
			}
			m_sResult.sBoardEdge.nContoursCount = m_sResult.sBoardEdge.vtptContoursPos.size();

			if (m_bSaveImage)
			{
				if (m_nSaveLevel > 0) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName, "FindBorderContours", m_sPM_FindBoard, vtmatImage, vtsResult, 2);
				}

				Mat matDisplay;
				if (!DisplayBoardContours(2, 1, matDisplay)) {
					return false;
				}
				SaveImage(0, matDisplay, "BorderContours", m_nExtensionType);
			}

			// 計算板邊的線性方程式
			if (m_sParam.sBoard.bEnable_Slope) {
				if (!Calculate_LinearEquation(m_sParam.sGlue.nLocation, *psContours, m_sResult.sBoardEdge, m_bSaveImage)) {
					return false;
				}
			}
			else {
				if (!DrawEdge(m_sParam.sGlue.nLocation, 2, m_matBoardEdge)) {
					return false;
				}	
				SaveImage(0, m_matBoardEdge, "BorderContours_Defual", m_nExtensionType);
			}

			return true;
		}

		bool MeasurementBlackGlue::Calculate_LinearEquation(const int& nLocation, SContoursParam& sBorderContours, SBoardEdge_Result& sBoardEdge, const bool& bSave)
		{
			switch (nLocation)
			{
			case 1:
			case 3:
			case 5:
			case 7:
				if (!Calculate_LinearEquation_Corner(sBorderContours, sBoardEdge)) return false;
				break;

			case 2:
			case 6:
				if (!Calculate_LinearEquation_Horizontal(sBorderContours, sBoardEdge)) return false;
				break;

			case 4:
			case 8:
				if (!Calculate_LinearEquation_Vertical(sBorderContours, sBoardEdge)) return false;
				break;
			case 9:
			case 10:
				if(!Calculate_LinearEquation_All(sBorderContours, sBoardEdge)) return false;
				break;
			default:
				return false;
			}

			if (bSave)
			{
				Mat matDisplay = m_matBoard.clone();
				if (!DisplayBoardPosition(sBoardEdge, m_scContours_Board, m_scLine_Board, matDisplay)) {
					return false;
				}

				SaveImage(0, matDisplay, "BorderContours_Line", m_nExtensionType);
			}

			return true;
		}
		bool MeasurementBlackGlue::Calculate_LinearEquation_All(SContoursParam& sBorderContours, SBoardEdge_Result& sBoardEdge)
		{
			int nSearchRangeX = 20;
			int nSearchRangeY = 20;
			vector<vector<POINT>> vt2ptLine;
			if (!ContoursToLine(nSearchRangeX, nSearchRangeY, sBoardEdge.vtptContoursPos, vt2ptLine)) {
				return false;
			}

			int nCount = vt2ptLine.size();
			if (nCount != 4) return false;

			for (int k = 0; k < nCount; ++k) {
				Mat matTemp = Mat(m_matBoard.size(), CV_8U, Scalar(0));
				int nCount_Point = vt2ptLine[k].size();
				for (int h = 0; h < nCount_Point; ++h) {
					uchar* pu8Edge = matTemp.ptr<uchar>(vt2ptLine[k][h].y);
					pu8Edge[vt2ptLine[k][h].x] = 255;
				}

				int nId = 0;
				switch (k) {
				case 0:
				{
					for (int x = 0; x < matTemp.cols; ++x) {
						for (int y = 0; y < matTemp.rows / 2; ++y) {
							uchar* pu8Temp = matTemp.ptr<uchar>(y);
							if (pu8Temp[x] == 255) {
								vt2ptLine[0][nId].x = x;
								vt2ptLine[0][nId].y = y;
								++nId;
								y = matTemp.rows;
							}
						}
					}
				}
				break;
				case 1:
				{
					for (int x = 0; x < matTemp.cols; ++x) {
						for (int y = matTemp.rows - 1; y >= matTemp.rows / 2; --y) {
							uchar* pu8Temp = matTemp.ptr<uchar>(y);
							if (pu8Temp[x] == 255) {
								vt2ptLine[1][nId].x = x;
								vt2ptLine[1][nId].y = y;
								++nId;
								y = 0;
							}
						}
					}
				}
				break;
				case 2:
				{
					for (int y = 0; y < matTemp.rows; ++y) {
						uchar* pu8Temp = matTemp.ptr<uchar>(y);
						for (int x = 0; x < matTemp.cols / 2; ++x) {
							if (pu8Temp[x] == 255) {
								vt2ptLine[2][nId].x = x;
								vt2ptLine[2][nId].y = y;
								++nId;
								x = matTemp.cols;
							}
						}
					}
				}
				break;
				case 3:
				{
					for (int y = 0; y < matTemp.rows; ++y) {
						uchar* pu8Temp = matTemp.ptr<uchar>(y);
						for (int x = matTemp.cols - 1; x >= matTemp.cols / 2; --x) {
							if (pu8Temp[x] == 255) {
								vt2ptLine[3][nId].x = x;
								vt2ptLine[3][nId].y = y;
								++nId;
								x = 0;
							}
						}
					}
				}
				break;
				}
				vt2ptLine[k].erase(vt2ptLine[k].begin() + nId, vt2ptLine[k].end());
			}

			if (m_bSaveImage) {
				Mat matTemp = m_matBoard.clone();
				jet_imagefunction::DrawEdgePoint(matTemp, vt2ptLine[0], Scalar(0, 0, 255), 1);
				jet_imagefunction::DrawEdgePoint(matTemp, vt2ptLine[1], Scalar(0, 255, 0), 1);
				jet_imagefunction::DrawEdgePoint(matTemp, vt2ptLine[2], Scalar(255, 0, 0), 1);
				jet_imagefunction::DrawEdgePoint(matTemp, vt2ptLine[3], Scalar(255, 255, 0), 1);
				SaveImage(0, matTemp, "BoardEdgePoint_All", 2);
			}

			vector<double> vtdSlope(4, 0.0);
			vector<double> vtdIntercept(4, 0.0);
			for (int k = 0; k < nCount; ++k) {
				if (k == 0 || k == 1) {
					if (!jet_imagefunction::SimpleLinearRegr(vt2ptLine[k], 5, 0.00001, vtdSlope[k], vtdIntercept[k])) {
						return false;
					}
				}
				else {
					if (!jet_imagefunction::SimpleLinearRegr_Vertical(vt2ptLine[k], 5, 0.00001, vtdSlope[k], vtdIntercept[k])) {
						return false;
					}
				}
			}

			sBoardEdge.dSlope_Horizontal = (vtdSlope[0] + vtdSlope[1]) / 2.0;
			vtdSlope[0] = vtdSlope[1] = sBoardEdge.dSlope_Horizontal;
			sBoardEdge.dSlope_Vertical = (vtdSlope[2] + vtdSlope[3]) / 2.0;
			vtdSlope[2] = vtdSlope[3] = sBoardEdge.dSlope_Vertical;

			for (int k = 0; k < nCount; ++k) {
				vtdIntercept[k] = 0.0;
				int nCount_Point = vt2ptLine[k].size();
				for (int h = 0; h < nCount_Point; ++h) {
					vtdIntercept[k] += (vt2ptLine[k][h].y - vtdSlope[k] * vt2ptLine[k][h].x);
				}
				vtdIntercept[k] /= nCount_Point;
			}

			sBoardEdge.dIntercept_Horizontal = vtdIntercept[0];
			sBoardEdge.dIntercept_Horizontal2 = vtdIntercept[1];
			sBoardEdge.dIntercept_Vertical = vtdIntercept[2];
			sBoardEdge.dIntercept_Vertical2 = vtdIntercept[3];

			Point2f ptfStartX, ptfEndX;
			ptfStartX.x = 0;
			ptfStartX.y = sBoardEdge.dIntercept_Horizontal;
			ptfEndX.x = m_matBoard.cols - 1;
			ptfEndX.y = sBoardEdge.dSlope_Horizontal*ptfEndX.x + sBoardEdge.dIntercept_Horizontal;
			sBoardEdge.sLine_Horizontal.ptStart.x = static_cast<LONG>(ptfStartX.x + 0.5);
			sBoardEdge.sLine_Horizontal.ptStart.y = static_cast<LONG>(ptfStartX.y + 0.5);
			sBoardEdge.sLine_Horizontal.ptEnd.x = static_cast<LONG>(ptfEndX.x + 0.5);
			sBoardEdge.sLine_Horizontal.ptEnd.y = static_cast<LONG>(ptfEndX.y + 0.5);
			Calculate_Distance(sBoardEdge.sLine_Horizontal);

			ptfStartX.y = sBoardEdge.dIntercept_Horizontal2;
			ptfEndX.y = sBoardEdge.dSlope_Horizontal*ptfEndX.x + sBoardEdge.dIntercept_Horizontal2;
			sBoardEdge.sLine_Horizontal2.ptStart.x = static_cast<LONG>(ptfStartX.x + 0.5);
			sBoardEdge.sLine_Horizontal2.ptStart.y = static_cast<LONG>(ptfStartX.y + 0.5);
			sBoardEdge.sLine_Horizontal2.ptEnd.x = static_cast<LONG>(ptfEndX.x + 0.5);
			sBoardEdge.sLine_Horizontal2.ptEnd.y = static_cast<LONG>(ptfEndX.y + 0.5);
			Calculate_Distance(sBoardEdge.sLine_Horizontal2);


			ptfStartX.y = 0;
			ptfStartX.x = (ptfStartX.y - sBoardEdge.dIntercept_Vertical) / sBoardEdge.dSlope_Vertical;
			ptfEndX.y = m_matBoard.rows - 1;
			ptfEndX.x = (ptfEndX.y - sBoardEdge.dIntercept_Vertical) / sBoardEdge.dSlope_Vertical;
			sBoardEdge.sLine_Vertical.ptStart.x = static_cast<LONG>(ptfStartX.x + 0.5);
			sBoardEdge.sLine_Vertical.ptStart.y = static_cast<LONG>(ptfStartX.y + 0.5);
			sBoardEdge.sLine_Vertical.ptEnd.x = static_cast<LONG>(ptfEndX.x + 0.5);
			sBoardEdge.sLine_Vertical.ptEnd.y = static_cast<LONG>(ptfEndX.y + 0.5);
			Calculate_Distance(sBoardEdge.sLine_Vertical);

			ptfStartX.x = (ptfStartX.y - sBoardEdge.dIntercept_Vertical2) / sBoardEdge.dSlope_Vertical;
			ptfEndX.x = (ptfEndX.y - sBoardEdge.dIntercept_Vertical2) / sBoardEdge.dSlope_Vertical;
			sBoardEdge.sLine_Vertical2.ptStart.x = static_cast<LONG>(ptfStartX.x + 0.5);
			sBoardEdge.sLine_Vertical2.ptStart.y = static_cast<LONG>(ptfStartX.y + 0.5);
			sBoardEdge.sLine_Vertical2.ptEnd.x = static_cast<LONG>(ptfEndX.x + 0.5);
			sBoardEdge.sLine_Vertical2.ptEnd.y = static_cast<LONG>(ptfEndX.y + 0.5);
			Calculate_Distance(sBoardEdge.sLine_Vertical2);

			return true;
		}
		bool MeasurementBlackGlue::Calculate_LinearEquation_Corner(SContoursParam& sBorderContours, SBoardEdge_Result& sBoardEdge)
		{
			if (!Calculate_LinearEquation_Vertical(sBorderContours, sBoardEdge)) return false;
			if (!Calculate_LinearEquation_Horizontal(sBorderContours, sBoardEdge)) return false;
			return true;
		}
		bool MeasurementBlackGlue::Calculate_LinearEquation_Vertical(SContoursParam& sBorderContours, SBoardEdge_Result& sBoardEdge)
		{
			vector<vector<POINT>> vt2ptPointY(m_matBoard.cols, vector<POINT>(m_matBoard.rows));
			vector<int> vtnCountY(m_matBoard.cols, 0);
			m_vtptBoardEdgePoint_V.clear();
			int nCount_Contours = sBorderContours.GetCount();

			for (int k = 0; k < nCount_Contours; ++k)
			{
				bool* pbOn = sBorderContours.GetOnPtr(k);
				if (pbOn == nullptr)	return false;

				if (*pbOn)
				{
					vector<vector<POINT>>* pvt2ptLine = sBorderContours.GetLinePtr(k);
					if (pvt2ptLine == nullptr)	return false;

					POINT* pPt = nullptr;
					int nCount_Line = (*pvt2ptLine).size();
					for (int h = 0; h < nCount_Line; ++h)
					{
						pPt = &(*pvt2ptLine)[h][0];
						int nCount_Point = (*pvt2ptLine)[h].size();
						for (int p = 0; p < nCount_Point; ++p) {
							int nX = pPt[p].x;
							vt2ptPointY[nX][vtnCountY[nX]] = pPt[p];
							++vtnCountY[nX];
						}
					}
				}
			}

			int nRange = 5;
			int nMaxSum = 0, nSum = 0, nMaxIndex = -1;
			for (int k = nRange; k < m_matBoard.cols- nRange; ++k) {
				int nL = k - nRange;
				int nR = k + nRange;
				if (nL < 0) nL = 0;
				if (nR >= m_matBoard.cols) nR = m_matBoard.cols - 1;

				nSum = 0;
				for (int h = nL; h <= nR; ++h) {
					nSum += vtnCountY[h];
					if (nMaxSum < nSum) {
						nMaxSum = nSum;
						nMaxIndex = k;
					}
				}
			}
			if (nMaxIndex == -1) {
				return false;
			}

			m_vtptBoardEdgePoint_V.resize(m_matBoard.total());
			int nIndexY = 0;
			int nL = nMaxIndex - nRange;
			int nR = nMaxIndex + nRange;
			if (nL < 0) nL = 0;
			if (nR >= m_matBoard.cols) nR = m_matBoard.cols - 1;
			for (int k = nL; k <= nR; ++k) {
				int nN = vtnCountY[k];
				for (int n = 0; n < nN; ++n)
				{
					m_vtptBoardEdgePoint_V[nIndexY] = vt2ptPointY[k][n];
					++nIndexY;
				}
			}

			m_vtptBoardEdgePoint_V.resize(nIndexY);
			if (m_bSaveImage) {
				Mat matTemp = m_matBoard.clone();
				jet_imagefunction::DrawEdgePoint(matTemp, m_vtptBoardEdgePoint_V, Scalar(0, 0, 255), 1);
				SaveImage(0, matTemp, "BoardEdgePoint_V", 2);
			}

			if (!jet_imagefunction::SimpleLinearRegr_Vertical(m_vtptBoardEdgePoint_V, 5, 0.00001, sBoardEdge.dSlope_Vertical, sBoardEdge.dIntercept_Vertical)) {
				return false;
			}

			if (sBoardEdge.dSlope_Vertical == 0.0) {
				return false;
			}

			Point2f ptfStartX, ptfEndX;
			ptfStartX.y = 0;
			ptfStartX.x = (ptfStartX.y - sBoardEdge.dIntercept_Vertical) / sBoardEdge.dSlope_Vertical;
			ptfEndX.y = m_matBoard.rows - 1;
			ptfEndX.x = (ptfEndX.y - sBoardEdge.dIntercept_Vertical) / sBoardEdge.dSlope_Vertical;

			sBoardEdge.sLine_Vertical.ptStart.x = static_cast<LONG>(ptfStartX.x + 0.5);
			sBoardEdge.sLine_Vertical.ptStart.y = static_cast<LONG>(ptfStartX.y + 0.5);
			sBoardEdge.sLine_Vertical.ptEnd.x = static_cast<LONG>(ptfEndX.x + 0.5);
			sBoardEdge.sLine_Vertical.ptEnd.y = static_cast<LONG>(ptfEndX.y + 0.5);
			Calculate_Distance(sBoardEdge.sLine_Vertical);

			return true;
		}
		bool MeasurementBlackGlue::Calculate_LinearEquation_Horizontal(SContoursParam& sBorderContours, SBoardEdge_Result& sBoardEdge)
		{
			vector<vector<POINT>> vt2ptPointX(m_matBoard.rows, vector<POINT>(m_matBoard.cols));
			vector<int> vtnCountX(m_matBoard.rows, 0);
			m_vtptBoardEdgePoint_H.clear();
			int nCount_Contours = sBorderContours.GetCount();

			for (int k = 0; k < nCount_Contours; ++k)
			{
				bool* pbOn = sBorderContours.GetOnPtr(k);
				if (pbOn == nullptr)	return false;

				if (*pbOn)
				{
					vector<vector<POINT>>* pvt2ptLine = sBorderContours.GetLinePtr(k);
					if (pvt2ptLine == nullptr)	return false;

					POINT* pPt = nullptr;
					int nCount_Line = (*pvt2ptLine).size();
					for (int h = 0; h < nCount_Line; ++h)
					{
						pPt = &(*pvt2ptLine)[h][0];
						int nCount_Point = (*pvt2ptLine)[h].size();
						for (int p = 0; p < nCount_Point; ++p)
						{
							int nY = pPt[p].y;
							vt2ptPointX[nY][vtnCountX[nY]] = pPt[p];
							++vtnCountX[nY];
						}
					}
				}
			}

			int nRange = 5;
			int nMaxSum = 0, nSum = 0, nMaxIndex = -1;
			for (int k = nRange; k < m_matBoard.rows- nRange; ++k) {
				int nT = k - nRange;
				int nB = k + nRange;
				if (nT < 0) nT = 0;
				if (nB >= m_matBoard.rows) nB = m_matBoard.rows - 1;

				nSum = 0;
				for (int h = nT; h <= nB; ++h) {
					nSum += vtnCountX[h];
				}
				if (nMaxSum < nSum) {
					nMaxSum = nSum;
					nMaxIndex = k;
				}
			}
			if (nMaxIndex == -1) {
				return false;
			}

			m_vtptBoardEdgePoint_H.resize(m_matBoard.total());
			int nIndexX = 0;
			int nT = nMaxIndex - nRange;
			int nB = nMaxIndex + nRange;
			if (nT < 0) nT = 0;
			if (nB >= m_matBoard.rows) nB = m_matBoard.rows - 1;
			for (int k = nT; k <= nB; ++k) {
				int nN = vtnCountX[k];
				for (int n = 0; n < nN; ++n)
				{
					m_vtptBoardEdgePoint_H[nIndexX] = vt2ptPointX[k][n];
					++nIndexX;
				}
			}

			m_vtptBoardEdgePoint_H.resize(nIndexX);
			if (m_bSaveImage) {
				Mat matTemp = m_matBoard.clone();
				jet_imagefunction::DrawEdgePoint(matTemp, m_vtptBoardEdgePoint_H, Scalar(0, 0, 255), 1);
				SaveImage(0, matTemp, "BoardEdgePoint_H", 2);
			}

			if (!jet_imagefunction::SimpleLinearRegr(m_vtptBoardEdgePoint_H, 5, 0.00001, sBoardEdge.dSlope_Horizontal, sBoardEdge.dIntercept_Horizontal)) {
				return false;
			}

			Point2f ptfStartX, ptfEndX;
			ptfStartX.x = 0;
			ptfStartX.y = sBoardEdge.dIntercept_Horizontal;
			ptfEndX.x = m_matBoard.cols - 1;
			ptfEndX.y = sBoardEdge.dSlope_Horizontal*ptfEndX.x + sBoardEdge.dIntercept_Horizontal;

			sBoardEdge.sLine_Horizontal.ptStart.x = static_cast<LONG>(ptfStartX.x + 0.5);
			sBoardEdge.sLine_Horizontal.ptStart.y = static_cast<LONG>(ptfStartX.y + 0.5);
			sBoardEdge.sLine_Horizontal.ptEnd.x = static_cast<LONG>(ptfEndX.x + 0.5);
			sBoardEdge.sLine_Horizontal.ptEnd.y = static_cast<LONG>(ptfEndX.y + 0.5);
			Calculate_Distance(sBoardEdge.sLine_Horizontal);

			return true;
		}

		// vt2ptLine[0] = top, vt2ptLine[1] = bottom, vt2ptLine[2] = left, vt2ptLine[3] = right
		bool MeasurementBlackGlue::ContoursToLine(const int& nSearchRangeX, const int& nSearchRangeY, const vector<POINT>& vtptContours, vector<vector<POINT>>& vt2ptLine)
		{
			int nSumPoint = vtptContours.size();
			if (nSumPoint <= 0) {
				return false;
			}

			vt2ptLine.resize(4, vector<POINT>(nSumPoint));
			vector<int> vtnId(4, 0);
			vector<int> vtnSearch1(4, 0);
			vector<int> vtnSearch2(4, 0);

			int nImageH = m_matBoard.rows;
			int nImageW = m_matBoard.cols;
			int nTop = nImageH, nBottom = 0, nLeft = nImageW, nRight = 0;

			vector<int> vtnTimes_Y(nImageH, 0);
			vector<int> vtnTimes_X(nImageW, 0);
			int nCountPoint = 0, nCount_Line = 0;
			for (int k = 0; k < nSumPoint; ++k) {
				++vtnTimes_Y[vtptContours[k].y];
				++vtnTimes_X[vtptContours[k].x];
				if (nTop > vtptContours[k].y)		nTop = vtptContours[k].y;
				if (nBottom < vtptContours[k].y)	nBottom = vtptContours[k].y;
				if (nLeft > vtptContours[k].x)		nLeft = vtptContours[k].x;
				if (nRight < vtptContours[k].x)		nRight = vtptContours[k].x;
			}

			int nCenterY = (nTop + nBottom) / 2;
			int nCenterX = (nLeft + nRight) / 2;
			int nRange = 10;
			int nMaxY = 0, nIndexY1 = 0, nIndexY2 = 0;
			vector<int> vtnSumY(nImageH, 0);
			for (int y = nRange; y < nImageH - nRange; ++y) {
				int nT = y - nRange;
				int nB = y + nRange;
				for (int r = nT; r < nB; ++r) {
					vtnSumY[y] += vtnTimes_Y[r];
				}
				if (vtnSumY[y] > nMaxY) {
					nMaxY = vtnSumY[y];
					nIndexY1 = y;
				}
			}

			nMaxY = 0;
			for (int y = nRange; y < nImageH - nRange; ++y) {
				if (abs(y - nIndexY1) > nCenterY) {
					if (vtnSumY[y] > nMaxY) {
						nMaxY = vtnSumY[y];
						nIndexY2 = y;
					}
				}
			}

			vector<int> vtnSumX(nImageW, 0);
			int nMaxX = 0, nIndexX1 = 0, nIndexX2 = 0;
			for (int x = nRange; x < nImageW - nRange; ++x) {
				int nL = x - nRange;
				int nR = x + nRange;
				for (int r = nL; r < nR; ++r) {
					vtnSumX[x] += vtnTimes_X[r];
				}
				if (vtnSumX[x] > nMaxX) {
					nMaxX = vtnSumX[x];
					nIndexX1 = x;
				}
			}

			nMaxX = 0;
			for (int x = nRange; x < nImageW - nRange; ++x) {
				if (abs(x - nIndexX1) > nCenterX) {
					if (vtnSumX[x] > nMaxX) {
						nMaxX = vtnSumX[x];
						nIndexX2 = x;
					}
				}
			}

			// Y
			if (nIndexY1 < nIndexY2) {
				// top
				vtnSearch1[0] = nIndexY1 - nSearchRangeY;
				if (vtnSearch1[0] < 0) vtnSearch1[0] = 0;
				vtnSearch2[0] = nIndexY1 + nSearchRangeY;
				if (vtnSearch2[0] >= nImageH) vtnSearch2[0] = nImageH - 1;

				// bottom
				vtnSearch1[1] = nIndexY2 - nSearchRangeY;
				if (vtnSearch1[1] < 0) vtnSearch1[1] = 0;
				vtnSearch2[1] = nIndexY2 + nSearchRangeY;
				if (vtnSearch2[1] >= nImageH) vtnSearch2[1] = nImageH - 1;
			}
			else {
				// top
				vtnSearch1[0] = nIndexY2 - nSearchRangeY;
				if (vtnSearch1[0] < 0) vtnSearch1[0] = 0;
				vtnSearch2[0] = nIndexY2 + nSearchRangeY;
				if (vtnSearch2[0] >= nImageH) vtnSearch2[0] = nImageH - 1;

				// bottom
				vtnSearch1[1] = nIndexY1 - nSearchRangeY;
				if (vtnSearch1[1] < 0) vtnSearch1[1] = 0;
				vtnSearch2[1] = nIndexY1 + nSearchRangeY;
				if (vtnSearch2[1] >= nImageH) vtnSearch2[1] = nImageH - 1;
			}

			// X
			if (nIndexX1 < nIndexX2) {
				// left
				vtnSearch1[2] = nIndexX1 - nSearchRangeX;
				if (vtnSearch1[2] < 0) vtnSearch1[2] = 0;
				vtnSearch2[2] = nIndexX1 + nSearchRangeX;
				if (vtnSearch2[2] >= nImageW) vtnSearch2[2] = nImageW - 1;

				// right
				vtnSearch1[3] = nIndexX2 - nSearchRangeX;
				if (vtnSearch1[3] < 0) vtnSearch1[3] = 0;
				vtnSearch2[3] = nIndexX2 + nSearchRangeX;
			}
			else {
				// left
				vtnSearch1[2] = nIndexX2 - nSearchRangeX;
				if (vtnSearch1[2] < 0) vtnSearch1[2] = 0;
				vtnSearch2[2] = nIndexX2 + nSearchRangeX;
				if (vtnSearch2[2] >= nImageW) vtnSearch2[2] = nImageW - 1;

				// right
				vtnSearch1[3] = nIndexX1 - nSearchRangeX;
				if (vtnSearch1[3] < 0) vtnSearch1[3] = 0;
				vtnSearch2[3] = nIndexX1 + nSearchRangeX;
			}

			for (int k = 0; k < nSumPoint; ++k) {
				int nY = vtptContours[k].y;
				int nX = vtptContours[k].x;
				for (int s = 0; s < 4; ++s) {
					if (vtnId[s] >= nSumPoint) continue;
					switch (s) {
					case 0:
					case 1:
						if (vtnSearch1[s] < nY && nY < vtnSearch2[s]) {
							vt2ptLine[s][vtnId[s]] = vtptContours[k];
							++vtnId[s];
						}
						break;

					case 2:
					case 3:
						if (vtnSearch1[s] < nX && nX < vtnSearch2[s]) {
							vt2ptLine[s][vtnId[s]] = vtptContours[k];
							++vtnId[s];
						}
						break;
					}
				}
			}

			for (int s = 0; s < 4; ++s) {
				vt2ptLine[s].resize(vtnId[s]);
			}

			return true;
		}
#if 0
		bool MeasurementBlackGlue::Calculate_LinearEquation_Vertical(SContoursParam& sBorderContours, SBoardEdge_Result& sBoardEdge)
		{
			vector<vector<POINT>> vt2ptPointY(m_matBoard.rows, vector<POINT>(m_matBoard.cols));
			vector<int> vtnCountY(m_matBoard.rows, 0);
			m_vtptBoardEdgePoint_V.clear();
			int nCount_Contours = sBorderContours.GetCount();

			for (int k = 0; k < nCount_Contours; ++k)
			{
				bool* pbOn = sBorderContours.GetOnPtr(k);
				if (pbOn == nullptr)	return false;

				if (*pbOn)
				{
					vector<vector<POINT>>* pvt2ptLine = sBorderContours.GetLinePtr(k);
					if (pvt2ptLine == nullptr)	return false;

					POINT* pPt = nullptr;
					int nCount_Line = (*pvt2ptLine).size();
					for (int h = 0; h < nCount_Line; ++h)
					{
						pPt = &(*pvt2ptLine)[h][0];
						int nCount_Point = (*pvt2ptLine)[h].size();
						for (int p = 0; p < nCount_Point; ++p)
						{
							int nY = pPt[p].y;
							vt2ptPointY[nY][vtnCountY[nY]] = pPt[p];
							++vtnCountY[nY];
						}
					}
				}
			}

			m_vtptBoardEdgePoint_V.resize(m_matBoard.total());
			int nIndexY = 0;
			for (int y = 0; y < m_matBoard.rows; ++y)
			{
				int nN = vtnCountY[y];
				if (nN > 0 && nN < 5)
				{
					for (int n = 0; n < nN; ++n)
					{
						m_vtptBoardEdgePoint_V[nIndexY] = vt2ptPointY[y][n];
						++nIndexY;
					}
				}
			}

			m_vtptBoardEdgePoint_V.resize(nIndexY);
			if (m_bSaveImage) {
				Mat matTemp = m_matBoard.clone();
				jet_imagefunction::DrawEdgePoint(matTemp, m_vtptBoardEdgePoint_V, Scalar(0, 0, 255), 1);
				SaveImage(0, matTemp, "BoardEdgePoint_V", 2);
			}

			if (!jet_imagefunction::SimpleLinearRegr_Vertical(m_vtptBoardEdgePoint_V, 10, 0.00001, sBoardEdge.dSlope_Vertical, sBoardEdge.dIntercept_Vertical)) {
				return false;
			}

			if (sBoardEdge.dSlope_Vertical == 0.0) {
				return false;
			}

			Point2f ptfStartX, ptfEndX;
			ptfStartX.y = 0;
			ptfStartX.x = (ptfStartX.y - sBoardEdge.dIntercept_Vertical) / sBoardEdge.dSlope_Vertical;
			ptfEndX.y = m_matBoard.rows - 1;
			ptfEndX.x = (ptfEndX.y - sBoardEdge.dIntercept_Vertical) / sBoardEdge.dSlope_Vertical;

			sBoardEdge.sLine_Vertical.ptStart.x = static_cast<LONG>(ptfStartX.x + 0.5);
			sBoardEdge.sLine_Vertical.ptStart.y = static_cast<LONG>(ptfStartX.y + 0.5);
			sBoardEdge.sLine_Vertical.ptEnd.x = static_cast<LONG>(ptfEndX.x + 0.5);
			sBoardEdge.sLine_Vertical.ptEnd.y = static_cast<LONG>(ptfEndX.y + 0.5);
			Calculate_Distance(sBoardEdge.sLine_Vertical);

			return true;
		}

		bool MeasurementBlackGlue::Calculate_LinearEquation_Horizontal(SContoursParam& sBorderContours, SBoardEdge_Result& sBoardEdge)
		{
			vector<vector<POINT>> vt2ptPointX(m_matBoard.rows, vector<POINT>(m_matBoard.cols));
			vector<int> vtnCountX(m_matBoard.cols, 0);
			m_vtptBoardEdgePoint_H.clear();
			int nCount_Contours = sBorderContours.GetCount();

			for (int k = 0; k < nCount_Contours; ++k)
			{
				bool* pbOn = sBorderContours.GetOnPtr(k);
				if (pbOn == nullptr)	return false;

				if (*pbOn)
				{
					vector<vector<POINT>>* pvt2ptLine = sBorderContours.GetLinePtr(k);
					if (pvt2ptLine == nullptr)	return false;

					POINT* pPt = nullptr;
					int nCount_Line = (*pvt2ptLine).size();
					for (int h = 0; h < nCount_Line; ++h)
					{
						pPt = &(*pvt2ptLine)[h][0];
						int nCount_Point = (*pvt2ptLine)[h].size();
						for (int p = 0; p < nCount_Point; ++p)
						{
							int nX = pPt[p].x;
							vt2ptPointX[vtnCountX[nX]][nX] = pPt[p];
							++vtnCountX[nX];
						}
					}
				}
			}

			// 畫X軸
			m_vtptBoardEdgePoint_H.resize(m_matBoard.total());
			int nIndexX = 0;
			for (int x = 0; x < m_matBoard.cols; ++x)
			{
				int nN = vtnCountX[x];
				if (nN > 0 && nN < 5)
				{
					for (int n = 0; n < nN; ++n)
					{
						m_vtptBoardEdgePoint_H[nIndexX] = vt2ptPointX[n][x];
						++nIndexX;
					}
				}
			}

			m_vtptBoardEdgePoint_H.resize(nIndexX);
			if (m_bSaveImage) {
				Mat matTemp = m_matBoard.clone();
				jet_imagefunction::DrawEdgePoint(matTemp, m_vtptBoardEdgePoint_H, Scalar(0, 0, 255), 1);
				SaveImage(0, matTemp, "BoardEdgePoint_H", 2);
			}

			if (!jet_imagefunction::SimpleLinearRegr(m_vtptBoardEdgePoint_H, 10, 0.00001, sBoardEdge.dSlope_Horizontal, sBoardEdge.dIntercept_Horizontal)) {
				return false;
			}

			Point2f ptfStartX, ptfEndX;
			ptfStartX.x = 0;
			ptfStartX.y = sBoardEdge.dIntercept_Horizontal;
			ptfEndX.x = m_matBoard.cols - 1;
			ptfEndX.y = sBoardEdge.dSlope_Horizontal*ptfEndX.x + sBoardEdge.dIntercept_Horizontal;

			sBoardEdge.sLine_Horizontal.ptStart.x = static_cast<LONG>(ptfStartX.x + 0.5);
			sBoardEdge.sLine_Horizontal.ptStart.y = static_cast<LONG>(ptfStartX.y + 0.5);
			sBoardEdge.sLine_Horizontal.ptEnd.x = static_cast<LONG>(ptfEndX.x + 0.5);
			sBoardEdge.sLine_Horizontal.ptEnd.y = static_cast<LONG>(ptfEndX.y + 0.5);
			Calculate_Distance(sBoardEdge.sLine_Horizontal);

			return true;
		}
#endif
#pragma endregion


#pragma region CoatingEdge

		// 找 Coating邊界的處理流程
		// Method1=>找Coating本體(抽色), Method2=>找Coating輪廓(轉灰階+二值化)
		bool MeasurementBlackGlue::SetCoatingProcess_FindContours(const SCoatingEdge_Parameter& sCoating, SProcessModeParam& sPM)
		{
			bool bResult = false;
			switch (sCoating.nFindEdgeMethod)
			{
			case 1:
				bResult = SetCoatingProcess_FindContours_Method1(sCoating, sPM);
				break;
			case 2:
				bResult = SetCoatingProcess_FindContours_Method2(sCoating, sPM);
				break;
			default:
				return false;
			}

			return bResult;
		}
		bool MeasurementBlackGlue::SetCoatingProcess_FindContours_Method1(const SCoatingEdge_Parameter& sCoating, SProcessModeParam& sPM)
		{
			int nQueueId = 1;
			sPM.Clear();

			// 1
			JET::mod::SFilterParam sFilter_Median;
			int nMedian_X = sCoating.nFilter_MedianX;
			if (nMedian_X <= 0) nMedian_X = 1;
			int nMedian_Y = sCoating.nFilter_MedianY;
			if (nMedian_Y <= 0) nMedian_Y = 1;
			sFilter_Median.SetInput1(1, 1);
			sFilter_Median.eMode = FILTER_MEDIAN;
			sFilter_Median.nSizeX = nMedian_X;
			sFilter_Median.nSizeY = nMedian_Y;
			sFilter_Median.SetQueueId(nQueueId);
			sPM.Set(sFilter_Median);
			++nQueueId;

			// 2
			JET::mod::SThresholdParam sThreshold_Color;
			sThreshold_Color.SetInput1(0, -1);
			sThreshold_Color.eMode = THRESHOLD_COLOR_TARGET;
			sThreshold_Color.nThreshold_Low = sCoating.nCoatingValue_B;
			sThreshold_Color.nThreshold_High = sCoating.nCoatingValue_G;
			sThreshold_Color.nAutoThreshold = sCoating.nCoatingValue_R;
			sThreshold_Color.fAlpha = sCoating.nCoatingValueTolerance;
			sThreshold_Color.bDark = true;
			sThreshold_Color.SetQueueId(nQueueId);
			sPM.Set(sThreshold_Color);
			++nQueueId;

			// 3
			JET::mod::SMorphologParam sMorpholog_Open;
			int nOpen_X = sCoating.nFilter_OpenX;
			if (nOpen_X <= 0) nOpen_X = 1;
			int nOpen_Y = sCoating.nFilter_OpenY;
			if (nOpen_Y <= 0) nOpen_Y = 1;
			sMorpholog_Open.SetInput1(0, -1);
			sMorpholog_Open.eMode = MORPHOLOG_OPENING;
			sMorpholog_Open.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Open.nSizeX = nOpen_X;
			sMorpholog_Open.nSizeY = nOpen_Y;
			sMorpholog_Open.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Open);
			++nQueueId;

			// 4
			JET::mod::SMorphologParam sMorpholog_Connect;
			int nConnect_X = sCoating.nFilter_ConnectX;
			if (nConnect_X <= 0) nConnect_X = 1;
			int nConnect_Y = sCoating.nFilter_ConnectY;
			if (nConnect_Y <= 0) nConnect_Y = 1;
			sMorpholog_Connect.SetInput1(0, -1);
			sMorpholog_Connect.eMode = MORPHOLOG_DILATE;
			sMorpholog_Connect.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Connect.nSizeX = nConnect_X;
			sMorpholog_Connect.nSizeY = nConnect_Y;
			sMorpholog_Connect.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Connect);
			++nQueueId;

			// 5
			JET::mod::SMorphologParam sMorpholog_CloseOpen;
			int nCloseOpen_X = sCoating.nFilter_SmoothX;
			if (nCloseOpen_X <= 0) nCloseOpen_X = 1;
			int nCloseOpen_Y = sCoating.nFilter_SmoothY;
			if (nCloseOpen_Y <= 0) nCloseOpen_Y = 1;
			sMorpholog_CloseOpen.SetInput1(0, -1);
			sMorpholog_CloseOpen.eMode = MORPHOLOG_CLOSEOPEN;
			sMorpholog_CloseOpen.eElement = ELEMENT_ELLIPSE;
			sMorpholog_CloseOpen.nSizeX = nCloseOpen_X;
			sMorpholog_CloseOpen.nSizeY = nCloseOpen_Y;
			sMorpholog_CloseOpen.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_CloseOpen);
			++nQueueId;

			// 6
			JET::mod::SContoursShapeParam sContours_Search;
			sContours_Search.SetInput1(0, -1);
			sContours_Search.eMode = CONTOUR_SEARCH;
			sContours_Search.bBoundingToEdge = true;
			sContours_Search.bRectangle = true;
			sContours_Search.bToEdge = true;
			sContours_Search.nSearchType = 1;
			sContours_Search.nSortType = 1;
			sContours_Search.SetQueueId(nQueueId);
			sPM.Set(sContours_Search);
			++nQueueId;

			// 7
			JET::mod::SContoursShapeParam sContours_Fill;
			sContours_Fill.SetContours1(0, -1);
			sContours_Fill.eMode = CONTOUR_CONVEX_FILL;
			sContours_Fill.SetQueueId(nQueueId);
			sPM.Set(sContours_Fill);
			++nQueueId;

			// 8
			JET::mod::SMorphologParam sMorpholog_Dilate;
			int nDilate_X = sCoating.nFilter_DilateX;
			if (nDilate_X <= 0) nDilate_X = 1;
			int nDilate_Y = sCoating.nFilter_DilateY;
			if (nDilate_Y <= 0) nDilate_Y = 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Dilate.eMode = MORPHOLOG_DILATE;
			sMorpholog_Dilate.nSizeX = nDilate_X;
			sMorpholog_Dilate.nSizeY = nDilate_Y;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 9
			JET::mod::SMorphologParam sMorpholog_Erosion;
			int nErosionX = sCoating.nFilter_ErosionX;
			if (nErosionX <= 0) nErosionX = 1;
			int nErosionY = sCoating.nFilter_ErosionY;
			if (nErosionY <= 0) nErosionY = 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Dilate.eMode = MORPHOLOG_EROSION;
			sMorpholog_Dilate.nSizeX = nErosionX;
			sMorpholog_Dilate.nSizeY = nErosionY;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 10
			JET::mod::SContoursShapeParam sContours_Search2;
			sContours_Search2.SetInput1(0, -1);
			sContours_Search2.eMode = CONTOUR_SEARCH;
			sContours_Search2.bBoundingToEdge = false;
			sContours_Search2.bRectangle = true;
			sContours_Search2.bToEdge = true;
			sContours_Search2.nSearchType = 1;
			sContours_Search2.nSortType = 1;
			sContours_Search2.SetQueueId(nQueueId);
			sPM.Set(sContours_Search2);
			++nQueueId;

			if (!sPM.CreateList()) {
				return false;
			}

			return true;
		}
		bool MeasurementBlackGlue::SetCoatingProcess_FindContours_Method2(const SCoatingEdge_Parameter& sCoating, SProcessModeParam& sPM)
		{
			int nQueueId = 1;
			sPM.Clear();

			// 1
			JET::mod::SColorTransformParam sColor_ToGray_Average;
			sColor_ToGray_Average.SetInput1(1, 1);
			sColor_ToGray_Average.eMode = COLOR_TO_GRAY_AVERAGE;
			sColor_ToGray_Average.SetQueueId(nQueueId);
			sPM.Set(sColor_ToGray_Average);
			++nQueueId;

			// 2
			JET::mod::SFilterParam sFilter_Median;
			int nMedian_X = sCoating.nFilter_MedianX;
			if (nMedian_X <= 0) nMedian_X = 1;
			int nMedian_Y = sCoating.nFilter_MedianY;
			if (nMedian_Y <= 0) nMedian_Y = 1;
			sFilter_Median.SetInput1(0, -1);
			sFilter_Median.eMode = FILTER_MEDIAN;
			sFilter_Median.nSizeX = nMedian_X;
			sFilter_Median.nSizeY = nMedian_Y;
			sFilter_Median.SetQueueId(nQueueId);
			sPM.Set(sFilter_Median);
			++nQueueId;

			// 3
			JET::mod::SThresholdParam sThreshold_Single;
			sThreshold_Single.SetInput1(0, -1);
			sThreshold_Single.bDark = sCoating.bDark;
			sThreshold_Single.eMode = THRESHOLD_SINGLE;
			sThreshold_Single.nThreshold_Low = sCoating.nThreshold;
			sThreshold_Single.SetQueueId(nQueueId);
			sPM.Set(sThreshold_Single);
			++nQueueId;

			// 4
			JET::mod::SMorphologParam sMorpholog_Open;
			int nOpen_X = sCoating.nFilter_OpenX;
			if (nOpen_X <= 0) nOpen_X = 1;
			int nOpen_Y = sCoating.nFilter_OpenY;
			if (nOpen_Y <= 0) nOpen_Y = 1;
			sMorpholog_Open.SetInput1(0, -1);
			sMorpholog_Open.eMode = MORPHOLOG_OPENING;
			sMorpholog_Open.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Open.nSizeX = nOpen_X;
			sMorpholog_Open.nSizeY = nOpen_Y;
			sMorpholog_Open.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Open);
			++nQueueId;

			// 5
			JET::mod::SMorphologParam sMorpholog_Connect;
			int nConnect_X = sCoating.nFilter_ConnectX;
			if (nConnect_X <= 0) nConnect_X = 1;
			int nConnect_Y = sCoating.nFilter_ConnectY;
			if (nConnect_Y <= 0) nConnect_Y = 1;
			sMorpholog_Connect.SetInput1(0, -1);
			sMorpholog_Connect.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Connect.eMode = MORPHOLOG_DILATE;
			sMorpholog_Connect.nSizeX = nConnect_X;
			sMorpholog_Connect.nSizeY = nConnect_Y;
			sMorpholog_Connect.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Connect);
			++nQueueId;

			// 6
			JET::mod::SMorphologParam sMorpholog_CloseOpen;
			int nCloseOpen_X = sCoating.nFilter_SmoothX;
			if (nCloseOpen_X <= 0) nCloseOpen_X = 1;
			int nCloseOpen_Y = sCoating.nFilter_SmoothY;
			if (nCloseOpen_Y <= 0) nCloseOpen_Y = 1;
			sMorpholog_CloseOpen.SetInput1(0, -1);
			sMorpholog_CloseOpen.eMode = MORPHOLOG_CLOSEOPEN;
			sMorpholog_CloseOpen.eElement = ELEMENT_ELLIPSE;
			sMorpholog_CloseOpen.nSizeX = nCloseOpen_X;
			sMorpholog_CloseOpen.nSizeY = nCloseOpen_Y;
			sMorpholog_CloseOpen.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_CloseOpen);
			++nQueueId;

			// 7
			JET::mod::SContoursShapeParam sContours_Search;
			sContours_Search.SetInput1(0, -1);
			sContours_Search.eMode = CONTOUR_SEARCH;
			sContours_Search.bBoundingToEdge = true;
			sContours_Search.bRectangle = true;
			sContours_Search.bToEdge = true;
			sContours_Search.nSearchType = 1;
			sContours_Search.nSortType = 1;
			sContours_Search.SetQueueId(nQueueId);
			sPM.Set(sContours_Search);
			++nQueueId;

			// 8
			JET::mod::SDeleteObjectParam sDelete_KeepROI_AwayBorder;
			sDelete_KeepROI_AwayBorder.SetContours1(0, -1);
			sDelete_KeepROI_AwayBorder.bEdge = false;
			sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
			sDelete_KeepROI_AwayBorder.nWidth_Min = 5;// 左
			sDelete_KeepROI_AwayBorder.nWidth_Max = 3;
			sDelete_KeepROI_AwayBorder.nOutputMode = 2;
			sDelete_KeepROI_AwayBorder.SetQueueId(nQueueId);
			sPM.Set(sDelete_KeepROI_AwayBorder);
			++nQueueId;

			if (!sPM.CreateList()) {
				return false;
			}
		}
#if 0
		bool MeasurementBlackGlue::SetCoatingProcess_FindContours_Method1(const SCoatingEdge_Parameter& sCoating, SProcessModeParam& sPM)
		{
			int nQueueId = 1;
			sPM.Clear();

			// 1
			JET::mod::SFilterParam sFilter_Median;
			int nMedian_X = sCoating.nFilter_MedianX;
			if (nMedian_X <= 0) nMedian_X = 1;
			int nMedian_Y = sCoating.nFilter_MedianY;
			if (nMedian_Y <= 0) nMedian_Y = 1;
			sFilter_Median.SetInput1(1, 1);
			sFilter_Median.eMode = FILTER_MEDIAN;
			sFilter_Median.nSizeX = nMedian_X;
			sFilter_Median.nSizeY = nMedian_Y;
			sFilter_Median.SetQueueId(nQueueId);
			sPM.Set(sFilter_Median);
			++nQueueId;

			// 2
			JET::mod::SThresholdParam sThreshold_Color;
			sThreshold_Color.SetInput1(0, -1);
			sThreshold_Color.eMode = THRESHOLD_COLOR_TARGET;
			sThreshold_Color.nThreshold_Low = sCoating.nCoatingValue_B;
			sThreshold_Color.nThreshold_High = sCoating.nCoatingValue_G;
			sThreshold_Color.nAutoThreshold = sCoating.nCoatingValue_R;
			sThreshold_Color.fAlpha = sCoating.nCoatingValueTolerance;
			sThreshold_Color.bDark = true;
			sThreshold_Color.SetQueueId(nQueueId);
			sPM.Set(sThreshold_Color);
			++nQueueId;

			// 3
			JET::mod::SMorphologParam sMorpholog_Open;
			int nOpen_X = sCoating.nFilter_OpenX;
			if (nOpen_X <= 0) nOpen_X = 1;
			int nOpen_Y = sCoating.nFilter_OpenY;
			if (nOpen_Y <= 0) nOpen_Y = 1;
			sMorpholog_Open.SetInput1(0, -1);
			sMorpholog_Open.eMode = MORPHOLOG_OPENING;
			sMorpholog_Open.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Open.nSizeX = nOpen_X;
			sMorpholog_Open.nSizeY = nOpen_Y;
			sMorpholog_Open.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Open);
			++nQueueId;

			// 4
			JET::mod::SMorphologParam sMorpholog_Connect;
			int nConnect_X = sCoating.nFilter_ConnectX;
			if (nConnect_X <= 0) nConnect_X = 1;
			int nConnect_Y = sCoating.nFilter_ConnectY;
			if (nConnect_Y <= 0) nConnect_Y = 1;
			sMorpholog_Connect.SetInput1(0, -1);
			sMorpholog_Connect.eMode = MORPHOLOG_DILATE;
			sMorpholog_Connect.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Connect.nSizeX = nConnect_X;
			sMorpholog_Connect.nSizeY = nConnect_Y;
			sMorpholog_Connect.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Connect);
			++nQueueId;

			// 5
			JET::mod::SMorphologParam sMorpholog_CloseOpen;
			int nCloseOpen_X = sCoating.nFilter_SmoothX;
			if (nCloseOpen_X <= 0) nCloseOpen_X = 1;
			int nCloseOpen_Y = sCoating.nFilter_SmoothY;
			if (nCloseOpen_Y <= 0) nCloseOpen_Y = 1;
			sMorpholog_CloseOpen.SetInput1(0, -1);
			sMorpholog_CloseOpen.eMode = MORPHOLOG_CLOSEOPEN;
			sMorpholog_CloseOpen.eElement = ELEMENT_ELLIPSE;
			sMorpholog_CloseOpen.nSizeX = nCloseOpen_X;
			sMorpholog_CloseOpen.nSizeY = nCloseOpen_Y;
			sMorpholog_CloseOpen.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_CloseOpen);
			++nQueueId;

			// 6
			JET::mod::SContoursShapeParam sContours_Search;
			sContours_Search.SetInput1(0, -1);
			sContours_Search.eMode = CONTOUR_SEARCH;
			sContours_Search.bBoundingToEdge = true;
			sContours_Search.bRectangle = true;
			sContours_Search.bToEdge = true;
			sContours_Search.nSearchType = 1;
			sContours_Search.nSortType = 1;
			sContours_Search.SetQueueId(nQueueId);
			sPM.Set(sContours_Search);
			++nQueueId;

			// 7
			JET::mod::SDeleteObjectParam sDelete_MaxArea;
			sDelete_MaxArea.SetContours1(0, -1);
			sDelete_MaxArea.eMode_Width = KEEP_ROI_MAXAREA;
			sDelete_MaxArea.nOutputMode = 2;
			sDelete_MaxArea.SetQueueId(nQueueId);
			sPM.Set(sDelete_MaxArea);
			++nQueueId;

			// 8
			JET::mod::SContoursShapeParam sContours_Fill;
			sContours_Fill.SetContours1(0, -1);
			sContours_Fill.eMode = CONTOUR_FILL;
			sContours_Fill.SetQueueId(nQueueId);
			sPM.Set(sContours_Fill);
			++nQueueId;

			// 9
			JET::mod::SMorphologParam sMorpholog_Dilate;
			int nDilate_X = sCoating.nFilter_DilateX;
			if (nDilate_X <= 0) nDilate_X = 1;
			int nDilate_Y = sCoating.nFilter_DilateY;
			if (nDilate_Y <= 0) nDilate_Y = 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Dilate.eMode = MORPHOLOG_DILATE;
			sMorpholog_Dilate.nSizeX = nDilate_X;
			sMorpholog_Dilate.nSizeY = nDilate_Y;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 10
			JET::mod::SMorphologParam sMorpholog_Erosion;
			int nErosionX = sCoating.nFilter_ErosionX;
			if (nErosionX <= 0) nErosionX = 1;
			int nErosionY = sCoating.nFilter_ErosionY;
			if (nErosionY <= 0) nErosionY = 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Dilate.eMode = MORPHOLOG_EROSION;
			sMorpholog_Dilate.nSizeX = nErosionX;
			sMorpholog_Dilate.nSizeY = nErosionY;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 11
			JET::mod::SImageCalculatorParam sCalculator_Boundary_Fill;
			sCalculator_Boundary_Fill.SetInput1(0, -1);
			sCalculator_Boundary_Fill.SetROI1(1, 1, 1, 1);
			sCalculator_Boundary_Fill.dValue = 0;
			sCalculator_Boundary_Fill.eMode = CALCULATOR_FILL_BOUNDARY;
			sCalculator_Boundary_Fill.SetQueueId(nQueueId);
			sPM.Set(sCalculator_Boundary_Fill);
			++nQueueId;

			// 12
			JET::mod::SContoursShapeParam sContours_Search2;
			sContours_Search2.SetInput1(0, -1);
			sContours_Search2.eMode = CONTOUR_SEARCH;
			sContours_Search2.bBoundingToEdge = true;
			sContours_Search2.bRectangle = true;
			sContours_Search2.bToEdge = true;
			sContours_Search2.nSearchType = 1;
			sContours_Search2.nSortType = 1;
			sContours_Search2.SetQueueId(nQueueId);
			sPM.Set(sContours_Search2);
			++nQueueId;

			// 13
			JET::mod::SDeleteObjectParam sDelete_MaxArea2;
			sDelete_MaxArea2.SetContours1(sContours_Search2);
			sDelete_MaxArea2.eMode_Width = KEEP_ROI_MAXAREA;
			sDelete_MaxArea2.nOutputMode = 3;
			sDelete_MaxArea2.SetQueueId(nQueueId);
			sPM.Set(sDelete_MaxArea2);
			++nQueueId;

			m_nContoursIndex = sContours_Search2.GetQueueId();
			if (!sPM.CreateList()) {
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::SetCoatingProcess_FindContours_Method2(const SCoatingEdge_Parameter& sCoating, SProcessModeParam& sPM)
		{
			int nQueueId = 1;
			sPM.Clear();

			// 1
			JET::mod::SColorTransformParam sColor_ToGray_Average;
			sColor_ToGray_Average.SetInput1(1, 1);
			sColor_ToGray_Average.eMode = COLOR_TO_GRAY_AVERAGE;
			sColor_ToGray_Average.SetQueueId(nQueueId);
			sPM.Set(sColor_ToGray_Average);
			++nQueueId;

			// 2
			JET::mod::SFilterParam sFilter_Median;
			int nMedian_X = sCoating.nFilter_MedianX;
			if (nMedian_X <= 0) nMedian_X = 1;
			int nMedian_Y = sCoating.nFilter_MedianY;
			if (nMedian_Y <= 0) nMedian_Y = 1;
			sFilter_Median.SetInput1(0, -1);
			sFilter_Median.eMode = FILTER_MEDIAN;
			sFilter_Median.nSizeX = nMedian_X;
			sFilter_Median.nSizeY = nMedian_Y;
			sFilter_Median.SetQueueId(nQueueId);
			sPM.Set(sFilter_Median);
			++nQueueId;

			// 3
			JET::mod::SThresholdParam sThreshold_Single;
			sThreshold_Single.SetInput1(0, -1);
			sThreshold_Single.bDark = sCoating.bDark;
			sThreshold_Single.eMode = THRESHOLD_SINGLE;
			sThreshold_Single.nThreshold_Low = sCoating.nThreshold;
			sThreshold_Single.SetQueueId(nQueueId);
			sPM.Set(sThreshold_Single);
			++nQueueId;

			// 4
			JET::mod::SMorphologParam sMorpholog_Open;
			int nOpen_X = sCoating.nFilter_OpenX;
			if (nOpen_X <= 0) nOpen_X = 1;
			int nOpen_Y = sCoating.nFilter_OpenY;
			if (nOpen_Y <= 0) nOpen_Y = 1;
			sMorpholog_Open.SetInput1(0, -1);
			sMorpholog_Open.eMode = MORPHOLOG_OPENING;
			sMorpholog_Open.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Open.nSizeX = nOpen_X;
			sMorpholog_Open.nSizeY = nOpen_Y;
			sMorpholog_Open.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Open);
			++nQueueId;

			// 5
			JET::mod::SMorphologParam sMorpholog_Connect;
			int nConnect_X = sCoating.nFilter_ConnectX;
			if (nConnect_X <= 0) nConnect_X = 1;
			int nConnect_Y = sCoating.nFilter_ConnectY;
			if (nConnect_Y <= 0) nConnect_Y = 1;
			sMorpholog_Connect.SetInput1(0, -1);
			sMorpholog_Connect.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Connect.eMode = MORPHOLOG_DILATE;
			sMorpholog_Connect.nSizeX = nConnect_X;
			sMorpholog_Connect.nSizeY = nConnect_Y;
			sMorpholog_Connect.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Connect);
			++nQueueId;

			// 6
			JET::mod::SMorphologParam sMorpholog_CloseOpen;
			int nCloseOpen_X = sCoating.nFilter_SmoothX;
			if (nCloseOpen_X <= 0) nCloseOpen_X = 1;
			int nCloseOpen_Y = sCoating.nFilter_SmoothY;
			if (nCloseOpen_Y <= 0) nCloseOpen_Y = 1;
			sMorpholog_CloseOpen.SetInput1(0, -1);
			sMorpholog_CloseOpen.eMode = MORPHOLOG_CLOSEOPEN;
			sMorpholog_CloseOpen.eElement = ELEMENT_ELLIPSE;
			sMorpholog_CloseOpen.nSizeX = nCloseOpen_X;
			sMorpholog_CloseOpen.nSizeY = nCloseOpen_Y;
			sMorpholog_CloseOpen.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_CloseOpen);
			++nQueueId;

			// 7
			JET::mod::SContoursShapeParam sContours_Search;
			sContours_Search.SetInput1(0, -1);
			sContours_Search.eMode = CONTOUR_SEARCH;
			sContours_Search.bBoundingToEdge = true;
			sContours_Search.bRectangle = true;
			sContours_Search.bToEdge = true;
			sContours_Search.nSearchType = 1;
			sContours_Search.nSortType = 1;
			sContours_Search.SetQueueId(nQueueId);
			sPM.Set(sContours_Search);
			++nQueueId;

			// 8
			JET::mod::SDeleteObjectParam sDelete_KeepROI_AwayBorder;
			sDelete_KeepROI_AwayBorder.SetContours1(0, -1);
			sDelete_KeepROI_AwayBorder.bEdge = false;
			sDelete_KeepROI_AwayBorder.eMode_Width = KEEP_ROI_AWAY_BORDER;
			sDelete_KeepROI_AwayBorder.nWidth_Min = 5;// 左
			sDelete_KeepROI_AwayBorder.nWidth_Max = 3;
			sDelete_KeepROI_AwayBorder.nOutputMode = 2;
			sDelete_KeepROI_AwayBorder.SetQueueId(nQueueId);
			sPM.Set(sDelete_KeepROI_AwayBorder);
			++nQueueId;

			// 9
			JET::mod::SDeleteObjectParam sDelete_MaxArea;
			sDelete_MaxArea.SetContours1(0, -1);
			sDelete_MaxArea.eMode_Width = KEEP_ROI_MAXAREA;
			sDelete_MaxArea.nOutputMode = 2;
			sDelete_MaxArea.SetQueueId(nQueueId);
			sPM.Set(sDelete_MaxArea);
			++nQueueId;

			// 10
			JET::mod::SContoursShapeParam sContours_Fill;
			sContours_Fill.SetContours1(0, -1);
			sContours_Fill.eMode = CONTOUR_FILL;
			sContours_Fill.SetQueueId(nQueueId);
			sPM.Set(sContours_Fill);
			++nQueueId;

			// 11
			JET::mod::SMorphologParam sMorpholog_Dilate;
			int nDilate_X = sCoating.nFilter_DilateX;
			if (nDilate_X <= 0) nDilate_X = 1;
			int nDilate_Y = sCoating.nFilter_DilateY;
			if (nDilate_Y <= 0) nDilate_Y = 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Dilate.eMode = MORPHOLOG_DILATE;
			sMorpholog_Dilate.nSizeX = nDilate_X;
			sMorpholog_Dilate.nSizeY = nDilate_Y;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 12
			JET::mod::SMorphologParam sMorpholog_Erosion;
			int nErosionX = sCoating.nFilter_ErosionX;
			if (nErosionX <= 0) nErosionX = 1;
			int nErosionY = sCoating.nFilter_ErosionY;
			if (nErosionY <= 0) nErosionY = 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Dilate.eMode = MORPHOLOG_EROSION;
			sMorpholog_Dilate.nSizeX = nErosionX;
			sMorpholog_Dilate.nSizeY = nErosionY;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 13
			JET::mod::SImageCalculatorParam sCalculator_Boundary_Fill;
			sCalculator_Boundary_Fill.SetInput1(0, -1);
			sCalculator_Boundary_Fill.SetROI1(1, 1, 1, 1);
			sCalculator_Boundary_Fill.dValue = 0;
			sCalculator_Boundary_Fill.eMode = CALCULATOR_FILL_BOUNDARY;
			sCalculator_Boundary_Fill.SetQueueId(nQueueId);
			sPM.Set(sCalculator_Boundary_Fill);
			++nQueueId;

			// 14
			JET::mod::SContoursShapeParam sContours_Search2;
			sContours_Search2.SetInput1(0, -1);
			sContours_Search2.eMode = CONTOUR_SEARCH;
			sContours_Search2.bBoundingToEdge = true;
			sContours_Search2.bRectangle = true;
			sContours_Search2.bToEdge = true;
			sContours_Search2.nSearchType = 1;
			sContours_Search2.nSortType = 1;
			sContours_Search2.SetQueueId(nQueueId);
			sPM.Set(sContours_Search2);
			++nQueueId;

			// 15
			JET::mod::SDeleteObjectParam sDelete_MaxArea2;
			sDelete_MaxArea2.SetContours1(0, -1);
			sDelete_MaxArea2.eMode_Width = KEEP_ROI_MAXAREA;
			sDelete_MaxArea2.nOutputMode = 3;
			sDelete_MaxArea2.SetQueueId(nQueueId);
			sPM.Set(sDelete_MaxArea2);
			++nQueueId;

			if (!sPM.CreateList()) {
				return false;
			}
		}
#endif
		
		bool MeasurementBlackGlue::RunSetCoatingProcess_FindContours()
		{
			if (!SetCoatingProcess_FindContours(m_sParam.sCoating, m_sPM_FindCoating) || m_matCoating.empty() || m_matCoating.channels() != 3) {
				return false;
			}

			vector<Mat> vtmatImage(1);
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			vtmatImage[0] = m_matCoating;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, m_sPM_FindCoating, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 0) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName, "Error_FindCoatingContours", m_sPM_FindCoating, vtmatImage, vtsResult, 2);
				}
				return false;
			}

			// 取出 ROI 位置
			//m_sResult.sCoatingEdge.rectROI.left = vtsResult[m_sPM_FindCoating.GetBaseParameterNumber() - 1].cvROI.x;
			//m_sResult.sCoatingEdge.rectROI.top = vtsResult[m_sPM_FindCoating.GetBaseParameterNumber() - 1].cvROI.y;
			//m_sResult.sCoatingEdge.rectROI.right = vtsResult[m_sPM_FindCoating.GetBaseParameterNumber() - 1].cvROI.br().x;
			//m_sResult.sCoatingEdge.rectROI.bottom = vtsResult[m_sPM_FindCoating.GetBaseParameterNumber() - 1].cvROI.br().y;
			//if (!jet_imagefunction::Fix_RECT(m_matCoating.rows, m_matCoating.cols, m_sResult.sCoatingEdge.rectROI)) {
			//	return false;
			//}

			// 取出輪廓
			SContoursParam* psContours = &vtsResult[m_sPM_FindCoating.GetBaseParameterNumber() - 1].sContours;
			if (psContours == nullptr) return false;

			// 取出 ROI 位置
			if (!GetSortROI(psContours, m_sParam.sCoating.nCount, m_sResult.sCoatingEdge.vtrectCoatingROI, m_sResult.sCoatingEdge.vt2ptContoursPos)) {
				m_strErrorMessage = "流程執行錯誤4";
				return false;
			}
			 
			m_sResult.sCoatingEdge.nCoatingCount = m_sResult.sCoatingEdge.vtrectCoatingROI.size();
			if (m_sResult.sCoatingEdge.nCoatingCount != m_sParam.sCoating.nCount || m_sResult.sCoatingEdge.nCoatingCount != m_sResult.sCoatingEdge.vt2ptContoursPos.size()) {
				return false;
			}

			m_sResult.sCoatingEdge.vtnContoursCount.resize(m_sResult.sCoatingEdge.nCoatingCount);
			for (int k = 0; k < m_sResult.sCoatingEdge.nCoatingCount; ++k) {
				m_sResult.sCoatingEdge.vtnContoursCount[k] = m_sResult.sCoatingEdge.vt2ptContoursPos[k].size();
			}

			if (m_bSaveImage)
			{
				if (m_nSaveLevel > 0) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName, "FindCoatingContours", m_sPM_FindCoating, vtmatImage, vtsResult, 2);
				}

				Mat matDisplay;
				if (!DisplayCoatingContours(3, 1, matDisplay)) {
					return false;
				}
				SaveImage(0, matDisplay, "CoatingContours", m_nExtensionType);
			}

			// 畫輪廓遮罩
			if (!DrawMask(3, m_nCoatingValue, m_matEdgeMask)) {
				m_strErrorMessage = "Draw CoatingMask Error";
				Save_Parameter();
				return false;
			}

			return true;
		}
#pragma endregion

#pragma region ThermalGlue
		bool MeasurementBlackGlue::SetThermalGlueProcess_Find_Die_Contours(const SThermalGlue_Parameter& sThermal, SProcessModeParam& sPM)
		{
			bool bResult = false;
			int nMethod = 1;
			switch (nMethod)
			{
			case 1:
				bResult = SetThermalGlueProcess_Find_Die_Contours_Method1(sThermal, sPM);
				if (!bResult) {
					m_strErrorMessage = "AdjustmentThermalGlue_Method1-Error";
				}
				break;
			default:
				return false;
			}

			return bResult;
		}

		bool MeasurementBlackGlue::SetThermalGlueProcess_Find_Die_Contours_Method1(const SThermalGlue_Parameter& sThermal, SProcessModeParam& sPM)
		{
			int nQueueId = 1;
			sPM.Clear();

			// 1
			JET::mod::SImageCalculatorParam sCalculator_ROI;
			int nDie_Left = sThermal.rectDie.left - sThermal.nDie_OutsetDistanceX;
			if (nDie_Left < 0) nDie_Left = 0;
			int nDie_Top = sThermal.rectDie.top - sThermal.nDie_OutsetDistanceY;
			if (nDie_Top < 0) nDie_Top = 0;
			int nIC_Right = sThermal.rectDie.right + sThermal.nDie_OutsetDistanceX;
			if (nIC_Right >= m_matThermal.cols) nIC_Right = m_matThermal.cols-1;
			int nIC_Bottom = sThermal.rectDie.bottom + sThermal.nDie_OutsetDistanceY;
			if (nIC_Bottom >= m_matThermal.rows) nIC_Bottom = m_matThermal.rows - 1;
			int nDie_SearchRangeX = nIC_Right - nDie_Left;
			int nDie_SearchRangeY = nIC_Bottom - nDie_Top;
			sCalculator_ROI.SetInput1(1, 1);
			sCalculator_ROI.SetROI1(nDie_Left, nDie_Top, nDie_SearchRangeX, nDie_SearchRangeY);
			sCalculator_ROI.eMode = CALCULATOR_EXTRACT_ROI;
			sCalculator_ROI.SetQueueId(nQueueId);
			sPM.Set(sCalculator_ROI);
			++nQueueId;

			// 2
			JET::mod::SColorTransformParam sColor_ToGray_Average;
			sColor_ToGray_Average.SetInput1(0, -1);
			sColor_ToGray_Average.eMode = COLOR_TO_GRAY_AVERAGE;
			sColor_ToGray_Average.nID = 2;
			sColor_ToGray_Average.SetQueueId(nQueueId);
			sPM.Set(sColor_ToGray_Average);
			++nQueueId;

			// 3
			JET::mod::SFilterParam sFilter_Median;
			int nMedianX = sThermal.nDie_MedianX;
			if (nMedianX < 1) nMedianX = 1;
			int nMedianY = sThermal.nDie_MedianY;
			if (nMedianY < 1) nMedianY = 1;
			sFilter_Median.SetInput1(0, -1);
			sFilter_Median.eMode = FILTER_MEDIAN;
			sFilter_Median.nSizeX = nMedianX;
			sFilter_Median.nSizeY = nMedianY;
			sFilter_Median.SetQueueId(nQueueId);
			sPM.Set(sFilter_Median);
			++nQueueId;

			// 4
			JET::mod::SThresholdParam sThreshold_Double;
			sThreshold_Double.SetInput1(0, -1);
			sThreshold_Double.eMode = THRESHOLD_DOUBLE;
			sThreshold_Double.bDark = sThermal.bDie_Between;
			sThreshold_Double.nThreshold_Low = sThermal.nDie_Threshold_Low;
			sThreshold_Double.nThreshold_High = sThermal.nDie_Threshold_High;
			sThreshold_Double.SetQueueId(nQueueId);
			sPM.Set(sThreshold_Double);
			++nQueueId;

			// 5
			JET::mod::SMorphologParam sMorpholog_Open;
			int nOpenX = sThermal.nDie_OpenX;
			if (nOpenX <= 0) nOpenX = 1;
			int nOpenY = sThermal.nDie_OpenY;
			if (nOpenY <= 0) nOpenY = 1;
			sMorpholog_Open.SetInput1(0, -1);
			sMorpholog_Open.eElement = ELEMENT_RECT;
			sMorpholog_Open.eMode = MORPHOLOG_OPENING;
			sMorpholog_Open.nSizeX = nOpenX;
			sMorpholog_Open.nSizeY = nOpenY;
			sMorpholog_Open.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Open);
			++nQueueId;

			// 6
			JET::mod::SMorphologParam sMorpholog_Connect;
			int nConnect_X = sThermal.nDie_ConnectX;
			if (nConnect_X <= 0) nConnect_X = 1;
			int nConnect_Y = sThermal.nDie_ConnectY;
			if (nConnect_Y <= 0) nConnect_Y = 1;
			sMorpholog_Connect.SetInput1(0, -1);
			sMorpholog_Connect.eMode = MORPHOLOG_DILATE;
			sMorpholog_Connect.eElement = ELEMENT_RECT;
			sMorpholog_Connect.nSizeX = nConnect_X;
			sMorpholog_Connect.nSizeY = nConnect_Y;
			sMorpholog_Connect.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Connect);
			++nQueueId;

			// 7
			JET::mod::SMorphologParam sMorpholog_Dilate;
			int nDilate_X = sThermal.nDie_DilateX;
			if (nDilate_X <= 0) nDilate_X = 1;
			int nDilate_Y = sThermal.nDie_DilateY;
			if (nDilate_Y <= 0) nDilate_Y = 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eElement = ELEMENT_RECT;
			sMorpholog_Dilate.eMode = MORPHOLOG_DILATE;
			sMorpholog_Dilate.nSizeX = nDilate_X;
			sMorpholog_Dilate.nSizeY = nDilate_Y;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 8
			JET::mod::SMorphologParam sMorpholog_Erosion;
			int nErosionX = sThermal.nDie_ErosionX;
			if (nErosionX <= 0) nErosionX = 1;
			int nErosionY = sThermal.nDie_ErosionY;
			if (nErosionY <= 0) nErosionY = 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eElement = ELEMENT_RECT;
			sMorpholog_Dilate.eMode = MORPHOLOG_EROSION;
			sMorpholog_Dilate.nSizeX = nErosionX;
			sMorpholog_Dilate.nSizeY = nErosionY;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 9
			JET::mod::SContoursShapeParam sContours_Search;
			sContours_Search.SetInput1(0, -1);
			sContours_Search.eMode = CONTOUR_SEARCH;
			sContours_Search.bBoundingToEdge = true;
			sContours_Search.bRectangle = true;
			sContours_Search.bToEdge = true;
			sContours_Search.nSearchType = 1;
			sContours_Search.nSortType = 1;
			sContours_Search.SetQueueId(nQueueId);
			sPM.Set(sContours_Search);
			++nQueueId;

			// 10
			JET::mod::SDeleteObjectParam sDelete_Size_Width;
			sDelete_Size_Width.SetContours1(0, -1);
			sDelete_Size_Width.nOutputMode = 4;
			sDelete_Size_Width.eMode_Width = DELETE_SIZE_LESSTHAN;
			sDelete_Size_Width.nWidth_Min = nDie_SearchRangeX / 10;
			if (sDelete_Size_Width.nWidth_Min <= 0)	sDelete_Size_Width.nWidth_Min = m_matThermal.cols / 3;
			sDelete_Size_Width.SetQueueId(nQueueId);
			sPM.Set(sDelete_Size_Width);
			++nQueueId;

			// 11
			JET::mod::SDeleteObjectParam sDelete_Size_Height;
			sDelete_Size_Height.SetContours1(0, -1);
			sDelete_Size_Height.nOutputMode = 4;
			sDelete_Size_Height.eMode_Height = DELETE_SIZE_LESSTHAN;
			sDelete_Size_Height.nHeight_Min = nDie_SearchRangeY / 3;
			if (sDelete_Size_Height.nHeight_Min <= 0)	sDelete_Size_Height.nHeight_Min = m_matThermal.rows / 3;
			sDelete_Size_Height.SetQueueId(nQueueId);
			sPM.Set(sDelete_Size_Height);
			++nQueueId;

			// 12
			JET::mod::SDeleteObjectParam sDelete_MaxArea;
			sDelete_MaxArea.SetContours1(0, -1);
			sDelete_MaxArea.eMode_Width = KEEP_ROI_MAXAREA;
			sDelete_MaxArea.nHeight_Max = 1;
			sDelete_MaxArea.nOutputMode = 4;
			sDelete_MaxArea.SetQueueId(nQueueId);
			sPM.Set(sDelete_MaxArea);
			++nQueueId;

			if (!sPM.CreateList()) {
				return false;
			}

			return true;
		}
		bool MeasurementBlackGlue::RunProcess_ThermalGlue_Find_Die_Contours()
		{
			if (!SetThermalGlueProcess_Find_Die_Contours(m_sParam.sThermal, m_sPM_FindThermal_IC) || m_matThermal.empty() || m_matThermal.channels() != 3) {
				return false;
			}

			vector<Mat> vtmatImage(1);
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			vtmatImage[0] = m_matThermal;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, m_sPM_FindThermal_IC, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 0) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName, "Error_FindThermalGlue_Die_Contours", m_sPM_FindThermal_IC, vtmatImage, vtsResult, 2);
				}
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 0) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName, "OK_FindThermalGlue_Die_Contours", m_sPM_FindThermal_IC, vtmatImage, vtsResult, 2);
			}

			int nCount = m_sPM_FindThermal_IC.GetBaseParameterNumber();
			if (vtsResult.size() != nCount) {
				return false;
			}

			int nMode = 0;
			SContoursParam* psContours = &vtsResult[nCount - 1].sContours;
			if (psContours == nullptr) return false;

			if (m_sParam.sThermal.bRebuildDie) {
				nMode = 2;
			}
			else {
				nMode = 1;
			}

			int nSearchRangeX = 20;
			int nSearchRangeY = 20;
			if (!ROI_ContoursToLine(nSearchRangeX, nSearchRangeY, psContours, m_vt2ptICEdgePoint)) {
				return false;
			}

			// m_vt2ptICEdgePoint[0] = top, m_vt2ptICEdgePoint[1] = bottom, m_vt2ptICEdgePoint[2] = left, m_vt2ptICEdgePoint[3] = right
			if (!ReCreateDie(nMode, m_vt2ptICEdgePoint, m_sResult.sThermalGlue.rectDie, m_sResult.sThermalGlue.vtptDieCorner, m_vtdDieSlope, m_vtdDieIntercept)) {
				return false;
			}

			int nDie_Left = m_sParam.sThermal.rectDie.left - m_sParam.sThermal.nDie_OutsetDistanceX;
			if (nDie_Left < 0) nDie_Left = 0;
			int nDie_Top = m_sParam.sThermal.rectDie.top - m_sParam.sThermal.nDie_OutsetDistanceY;
			if (nDie_Top < 0) nDie_Top = 0;
			m_sResult.sThermalGlue.rectDie.left += nDie_Left;
			m_sResult.sThermalGlue.rectDie.right += nDie_Left;
			m_sResult.sThermalGlue.rectDie.top += nDie_Top;
			m_sResult.sThermalGlue.rectDie.bottom += nDie_Top;

			if (nMode == 2) {
				if (m_sResult.sThermalGlue.vtptDieCorner.size() != 4) {
					return false;
				}

				for (int k = 0; k < 4; ++k) {
					m_sResult.sThermalGlue.vtptDieCorner[k].x += nDie_Left;
					m_sResult.sThermalGlue.vtptDieCorner[k].y += nDie_Top;
				}
			}
			if (!jet_imagefunction::Check_RECT(m_matThermal.rows, m_matThermal.cols, m_sResult.sThermalGlue.rectDie)) {
				return false;
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matThermal.clone();

				if (nMode == 2) {
					Point cvPoint1, cvPoint2;
					cvPoint1.x = m_sResult.sThermalGlue.vtptDieCorner[0].x;
					cvPoint1.y = m_sResult.sThermalGlue.vtptDieCorner[0].y;
					cvPoint2.x = m_sResult.sThermalGlue.vtptDieCorner[1].x;
					cvPoint2.y = m_sResult.sThermalGlue.vtptDieCorner[1].y;
					line(matDisplay, cvPoint1, cvPoint2, Scalar(255, 0, 0), 1);

					cvPoint1.x = m_sResult.sThermalGlue.vtptDieCorner[1].x;
					cvPoint1.y = m_sResult.sThermalGlue.vtptDieCorner[1].y;
					cvPoint2.x = m_sResult.sThermalGlue.vtptDieCorner[2].x;
					cvPoint2.y = m_sResult.sThermalGlue.vtptDieCorner[2].y;
					line(matDisplay, cvPoint1, cvPoint2, Scalar(0, 255, 0), 1);

					cvPoint1.x = m_sResult.sThermalGlue.vtptDieCorner[2].x;
					cvPoint1.y = m_sResult.sThermalGlue.vtptDieCorner[2].y;
					cvPoint2.x = m_sResult.sThermalGlue.vtptDieCorner[3].x;
					cvPoint2.y = m_sResult.sThermalGlue.vtptDieCorner[3].y;
					line(matDisplay, cvPoint1, cvPoint2, Scalar(0, 0, 255), 1);

					cvPoint1.x = m_sResult.sThermalGlue.vtptDieCorner[3].x;
					cvPoint1.y = m_sResult.sThermalGlue.vtptDieCorner[3].y;
					cvPoint2.x = m_sResult.sThermalGlue.vtptDieCorner[0].x;
					cvPoint2.y = m_sResult.sThermalGlue.vtptDieCorner[0].y;
					line(matDisplay, cvPoint1, cvPoint2, Scalar(255, 255, 0), 1);
					SaveImage(0, matDisplay, "Die_ROI_Slope", 1);
				}
				else {
					Rect cvROI;
					jet_imagefunction::RECTToRect(m_sResult.sThermalGlue.rectDie, 0, 0, cvROI);
					cv::rectangle(matDisplay, cvROI, Scalar(0, 255, 255), 2);
					SaveImage(0, matDisplay, "Die_ROI", 1);
				}
			}

			return true;
		}

		bool MeasurementBlackGlue::SetThermalGlueProcess_Find_Glue_Contours(const SThermalGlue_Parameter& sThermal, const RECT& rectIC_ROI, SProcessModeParam& sPM)
		{
			bool bResult = false;
			int nMethod = 1;
			switch (nMethod)
			{
			case 1:
				bResult = SetThermalGlueProcess_Find_Glue_Contours_Method1(sThermal, rectIC_ROI, sPM);
				if (!bResult) {
					m_strErrorMessage = "AdjustmentThermalGlue_Method1-Error";
				}
				break;
			default:
				return false;
			}

			return bResult;
		}
		bool MeasurementBlackGlue::SetThermalGlueProcess_Find_Glue_Contours_Method1(const SThermalGlue_Parameter& sThermal, const RECT& rectDie, SProcessModeParam& sPM)
		{
			int nQueueId = 1;
			sPM.Clear();

			// 1
			JET::mod::SImageCalculatorParam sCalculator_ROI;
			int nIC_Left = rectDie.left + sThermal.nDie_InsetDistanceX;
			if (nIC_Left < 0) nIC_Left = 0;
			int nIC_Top = rectDie.top + sThermal.nDie_InsetDistanceY;
			if (nIC_Top < 0) nIC_Top = 0;
			int nIC_Right = rectDie.right - sThermal.nDie_InsetDistanceX;
			if (nIC_Right >= m_matThermal.cols) nIC_Right = m_matThermal.cols - 1;
			int nIC_Bottom = rectDie.bottom - sThermal.nDie_InsetDistanceY;
			if (nIC_Bottom >= m_matThermal.rows) nIC_Bottom = m_matThermal.rows - 1;
			int nIC_RangeX = nIC_Right - nIC_Left;
			int nIC_RangeY = nIC_Bottom - nIC_Top;
			sCalculator_ROI.SetInput1(1, 1);
			sCalculator_ROI.SetROI1(nIC_Left, nIC_Top, nIC_RangeX, nIC_RangeY);
			sCalculator_ROI.eMode = CALCULATOR_EXTRACT_ROI;
			sCalculator_ROI.SetQueueId(nQueueId);
			sPM.Set(sCalculator_ROI);
			++nQueueId;

			// 2
			JET::mod::SColorTransformParam sColor_ToGray_Average;
			sColor_ToGray_Average.SetInput1(0, -1);
			sColor_ToGray_Average.eMode = COLOR_TO_GRAY_AVERAGE;
			sColor_ToGray_Average.nID = 2;
			sColor_ToGray_Average.SetQueueId(nQueueId);
			sPM.Set(sColor_ToGray_Average);
			++nQueueId;

			// 3
			JET::mod::SFilterParam sFilter_Median;
			int nMedianX = sThermal.nGlue_MedianX;
			if (nMedianX < 1) nMedianX = 1;
			int nMedianY = sThermal.nGlue_MedianY;
			if (nMedianY < 1) nMedianY = 1;
			sFilter_Median.SetInput1(0, -1);
			sFilter_Median.eMode = FILTER_MEDIAN;
			sFilter_Median.nSizeX = nMedianX;
			sFilter_Median.nSizeY = nMedianY;
			sFilter_Median.SetQueueId(nQueueId);
			sPM.Set(sFilter_Median);
			++nQueueId;

			// 4
			JET::mod::SThresholdParam sThreshold_Single;
			sThreshold_Single.SetInput1(0, -1);
			sThreshold_Single.bDark = sThermal.bGlue_Dark;
			sThreshold_Single.eMode = THRESHOLD_SINGLE;
			sThreshold_Single.nThreshold_Low = sThermal.nGlue_Threshold;
			sThreshold_Single.SetQueueId(nQueueId);
			sPM.Set(sThreshold_Single);
			++nQueueId;

			// 5
			JET::mod::SMorphologParam sMorpholog_Open;
			int nOpenX = sThermal.nGlue_OpenX;
			if (nOpenX <= 0) nOpenX = 1;
			int nOpenY = sThermal.nGlue_OpenY;
			if (nOpenY <= 0) nOpenY = 1;
			sMorpholog_Open.SetInput1(0, -1);
			sMorpholog_Open.eElement = ELEMENT_RECT;
			sMorpholog_Open.eMode = MORPHOLOG_OPENCLOSE;
			sMorpholog_Open.nSizeX = nOpenX;
			sMorpholog_Open.nSizeY = nOpenY;
			sMorpholog_Open.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Open);
			++nQueueId;

			// 6
			JET::mod::SContoursShapeParam sContours_Search;
			sContours_Search.SetInput1(0, -1);
			sContours_Search.eMode = CONTOUR_SEARCH;
			sContours_Search.bBoundingToEdge = true;
			sContours_Search.bRectangle = true;
			sContours_Search.bToEdge = true;
			sContours_Search.nSearchType = 1;
			sContours_Search.nSortType = 1;
			sContours_Search.SetQueueId(nQueueId);
			sPM.Set(sContours_Search);
			++nQueueId;

			// 7
			JET::mod::SDeleteObjectParam sDelete_Size;
			sDelete_Size.SetContours1(0, -1);
			sDelete_Size.nOutputMode = 4;
			switch (sThermal.nGlue_Direction) {
			case 1:			
				sDelete_Size.eMode_Width = DELETE_SIZE_LESSTHAN;
				sDelete_Size.nWidth_Min = nIC_RangeX / 3;
				if (sDelete_Size.nWidth_Min <= 0)	sDelete_Size.nWidth_Min = m_matThermal.cols/3;
				break;
			case 2:
				sDelete_Size.eMode_Height = DELETE_SIZE_LESSTHAN;
				sDelete_Size.nHeight_Min = nIC_RangeY / 3;
				if (sDelete_Size.nHeight_Min <= 0)	sDelete_Size.nHeight_Min = m_matThermal.rows / 3;
				break;
			}
			sDelete_Size.SetQueueId(nQueueId);
			sPM.Set(sDelete_Size);
			++nQueueId;

			if (!sPM.CreateList()) {
				return false;
			}

			return true;
		}
		bool MeasurementBlackGlue::RunProcess_ThermalGlue_Find_Glue_Contours()
		{
			if (!SetThermalGlueProcess_Find_Glue_Contours(m_sParam.sThermal, m_sResult.sThermalGlue.rectDie, m_sPM_FindThermal_Glue) || m_matThermal.empty() || m_matThermal.channels() != 3) {
				return false;
			}

			vector<Mat> vtmatImage(1);
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			vtmatImage[0] = m_matThermal;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, m_sPM_FindThermal_Glue, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 0) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName, "Error_FindThermalGlue_Glue_Contours", m_sPM_FindThermal_Glue, vtmatImage, vtsResult, 2);
				}
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 0) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName, "OK_FindThermalGlue_Glue_Contours", m_sPM_FindThermal_Glue, vtmatImage, vtsResult, 2);
			}
			
			int nCount = m_sPM_FindThermal_Glue.GetBaseParameterNumber();
			if (vtsResult.size() != nCount) {
				return false;
			}

			SContoursParam* psContours = &vtsResult[nCount - 1].sContours;
			if (psContours == nullptr) return false;

			vector<SJRect*> vtpjRect;
			vector<vector<POINT>> vt2ptContoursPos;
			if (!ContoursConvertMultipleVector(psContours, vtpjRect, vt2ptContoursPos)) {
				return false;
			}	

			// 實際偵測到的 Glue ROI
			int nGlueCount = psContours->GetCount_On();
			if (nGlueCount <= 0) {
				return false;
			}

			// 建立 Measurement ROI 
			if (!m_sResult.sThermalGlue.SetGlue_Count(nGlueCount)) {
				return false;
			}

			if (nGlueCount != m_sResult.sThermalGlue.sGlueWidth.nRoiCount || m_sResult.sThermalGlue.sGlueWidth.nRoiCount != m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo.size()) {
				return false;
			}

			if (nGlueCount > 1) {
				if (nGlueCount-1 != m_sResult.sThermalGlue.sGlueToGlue.nRoiCount || m_sResult.sThermalGlue.sGlueToGlue.nRoiCount != m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo.size()) {
					return false;
				}
			}

			if (2 != m_sResult.sThermalGlue.sGlueToDie.nRoiCount || m_sResult.sThermalGlue.sGlueToDie.nRoiCount != m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo.size()) {
				return false;
			}

			vector<RECT> vtrectGlueROI;
			vector<SJRect*> vtpjRect_Temp = vtpjRect;
			if (!Sort_ROI(m_sParam.sThermal.nGlue_Direction, vtpjRect, vtrectGlueROI)) {
				return false;
			}
			
			int nIC_Left = m_sResult.sThermalGlue.rectDie.left + m_sParam.sThermal.nDie_InsetDistanceX;
			if (nIC_Left < 0) nIC_Left = 0;
			int nIC_Top = m_sResult.sThermalGlue.rectDie.top + m_sParam.sThermal.nDie_InsetDistanceY;
			if (nIC_Top < 0) nIC_Top = 0;	
			
			// Glue Contours	
			for (int k = 0; k < nGlueCount; ++k) {
				vtrectGlueROI[k].left += nIC_Left;
				vtrectGlueROI[k].top += nIC_Top;
				vtrectGlueROI[k].right += nIC_Left;
				vtrectGlueROI[k].bottom += nIC_Top;
				for (int h = 0; h < nGlueCount; ++h) {
					if (vtpjRect[k]->nX == vtpjRect_Temp[h]->nX && vtpjRect[k]->nY == vtpjRect_Temp[h]->nY) {
						int nPointNumber = vt2ptContoursPos[h].size();
						m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].nContoursCount = nPointNumber;
						m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].vtptContoursPos.resize(nPointNumber);
						for (int p = 0; p < nPointNumber; ++p) {
							m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].vtptContoursPos[p].x = vt2ptContoursPos[h][p].x + nIC_Left;
							m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].vtptContoursPos[p].y = vt2ptContoursPos[h][p].y + nIC_Top;
						}
					}	
				}
			}

			// Set Measurement ROI 位置
			// sGlueWidth
			for (int k = 0; k < m_sResult.sThermalGlue.sGlueWidth.nRoiCount; ++k) {
				m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI = vtrectGlueROI[k];

				switch (m_sParam.sThermal.nGlue_Direction) {
				case 1:	// 水平
					if (m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.left < m_rectThermalGlueMeasurementRange.left) {
						m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.left = m_rectThermalGlueMeasurementRange.left;
					}

					if (m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.right > m_rectThermalGlueMeasurementRange.right) {
						m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.right = m_rectThermalGlueMeasurementRange.right;
					}
					break;
				case 2:// 垂直
					if (m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.top < m_rectThermalGlueMeasurementRange.top) {
						m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.top = m_rectThermalGlueMeasurementRange.top;
					}

					if (m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.bottom > m_rectThermalGlueMeasurementRange.bottom) {
						m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.bottom = m_rectThermalGlueMeasurementRange.bottom;
					}
					break;
				}
			}

			// sGlueToGlue
			RECT* prectROI1 = nullptr;
			RECT* prectROI2 = nullptr;
			for (int k = 0; k < m_sResult.sThermalGlue.sGlueToGlue.nRoiCount; ++k) {
				prectROI1 = &m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI;
				prectROI2 = &m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k+1].rectROI;
				switch (m_sParam.sThermal.nGlue_Direction) {
				case 1:	// 水平
					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.top = (prectROI1->top + prectROI1->bottom) / 2;
					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.bottom = (prectROI2->top + prectROI2->bottom) / 2;
					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.left = (prectROI1->left > prectROI2->left) ? prectROI1->left : prectROI2->left;
					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.right = (prectROI1->right < prectROI2->right) ? prectROI1->right : prectROI2->right;

					if (m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.left < m_rectThermalGlueMeasurementRange.left) {
						m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.left = m_rectThermalGlueMeasurementRange.left;
					}

					if (m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.right > m_rectThermalGlueMeasurementRange.right) {
						m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.right = m_rectThermalGlueMeasurementRange.right;
					}
					break;

				case 2:// 垂直
					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.top = (prectROI1->top > prectROI2->top) ? prectROI1->top : prectROI2->top;
					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.bottom = (prectROI1->bottom < prectROI2->bottom) ? prectROI1->bottom : prectROI2->bottom;
					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.left = (prectROI1->left + prectROI1->right) / 2;
					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.right = (prectROI2->left + prectROI2->right) / 2;

					if (m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.top < m_rectThermalGlueMeasurementRange.top) {
						m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.top = m_rectThermalGlueMeasurementRange.top;
					}

					if (m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.bottom > m_rectThermalGlueMeasurementRange.bottom) {
						m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.bottom = m_rectThermalGlueMeasurementRange.bottom;
					}
					break;
				}
			}

			// sGlueToDie
			int nDist = 10;
			prectROI1 = &m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[0].rectROI;
			prectROI2 = &m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[m_sResult.sThermalGlue.sGlueWidth.nRoiCount-1].rectROI;
			switch (m_sParam.sThermal.nGlue_Direction) {
			case 1:	// 水平
				if (m_sResult.sThermalGlue.rectDie.top > nDist) {
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.top = m_sResult.sThermalGlue.rectDie.top - nDist;
				}
				else {
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.top = m_sResult.sThermalGlue.rectDie.top;
				}
				m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.bottom = (prectROI1->top + prectROI1->bottom) / 2;
				m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.left = prectROI1->left;
				m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.right = prectROI1->right;
				if (m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.left < m_rectThermalGlueMeasurementRange.left) {
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.left = m_rectThermalGlueMeasurementRange.left;
				}
				if (m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.right > m_rectThermalGlueMeasurementRange.right) {
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.right = m_rectThermalGlueMeasurementRange.right;
				}

				m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.top = (prectROI2->top + prectROI2->bottom) / 2;
				if (m_sResult.sThermalGlue.rectDie.bottom < m_matThermal.rows - nDist) {
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.bottom = m_sResult.sThermalGlue.rectDie.bottom + nDist;
				}
				else {
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.bottom = m_sResult.sThermalGlue.rectDie.bottom;
				}
				m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.left = prectROI2->left;
				m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.right = prectROI2->right;
				if (m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.left < m_rectThermalGlueMeasurementRange.left) {
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.left = m_rectThermalGlueMeasurementRange.left;
				}
				if (m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.right > m_rectThermalGlueMeasurementRange.right) {
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.right = m_rectThermalGlueMeasurementRange.right;
				}
				break;

			case 2:// 垂直
				m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.top = prectROI1->top;
				m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.bottom = prectROI1->bottom;
				if (m_sResult.sThermalGlue.rectDie.left > nDist) {
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.left = m_sResult.sThermalGlue.rectDie.left - nDist;
				}
				else {
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.left = m_sResult.sThermalGlue.rectDie.left;
				}
				m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.right = (prectROI1->left + prectROI1->right) / 2;
				if (m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.top < m_rectThermalGlueMeasurementRange.top) {
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.top = m_rectThermalGlueMeasurementRange.top;
				}
				if (m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.bottom > m_rectThermalGlueMeasurementRange.bottom) {
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[0].rectROI.bottom = m_rectThermalGlueMeasurementRange.bottom;
				}

				m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.top = prectROI2->top;
				m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.bottom = prectROI2->bottom;
				m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.left = (prectROI2->left + prectROI2->right) / 2;
				if (m_sResult.sThermalGlue.rectDie.bottom < m_matThermal.cols - nDist) {
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.right = m_sResult.sThermalGlue.rectDie.right + nDist;
				}
				else {
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.right = m_sResult.sThermalGlue.rectDie.right;
				}
				if (m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.top < m_rectThermalGlueMeasurementRange.top) {
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.top = m_rectThermalGlueMeasurementRange.top;
				}
				if (m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.bottom > m_rectThermalGlueMeasurementRange.bottom) {
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[1].rectROI.bottom = m_rectThermalGlueMeasurementRange.bottom;
				}
				break;
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matThermal.clone();

				Rect cvROI;
				jet_imagefunction::RECTToRect(m_sResult.sThermalGlue.rectDie, 0, 0, cvROI);
				cv::rectangle(matDisplay, cvROI, m_scDistance_GlueWidth, 2);

				// sGlueWidth
				for (int k = 0; k < m_sResult.sThermalGlue.sGlueWidth.nRoiCount; ++k) {
					jet_imagefunction::RECTToRect(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI, 0, 0, cvROI);
					if (m_sParam.sThermal.nGlue_Direction == 1) {
						Point ptCenter = Point(cvROI.x + cvROI.width / 3, (cvROI.y + cvROI.br().y) / 2);
						cv::putText(matDisplay, "Therml GlueWidth " + to_string(k + 1), ptCenter, FONT_HERSHEY_TRIPLEX, 1.0, Scalar(0, 128, 0), 2);
					}
					else {
						int nY = cvROI.height / 5;
						Point ptCenter = Point(cvROI.x, nY*(k + 1));
						cv::putText(matDisplay, "Therml GlueWidth " + to_string(k + 1), ptCenter, FONT_HERSHEY_TRIPLEX, 1.0, Scalar(0, 128, 0), 2);
					}
					jet_imagefunction::DrawEdgePoint(matDisplay, m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].vtptContoursPos, m_scContours_Glue, 1);
				}

				// sGlueToGlue
				for (int k = 0; k < m_sResult.sThermalGlue.sGlueToGlue.nRoiCount; ++k) {
					jet_imagefunction::RECTToRect(m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI, 0, 0, cvROI);
					cv::rectangle(matDisplay, cvROI, Scalar(0, 0, 128), 2);
				}

				// sGlueToDie
				for (int k = 0; k < m_sResult.sThermalGlue.sGlueToDie.nRoiCount; ++k) {
					jet_imagefunction::RECTToRect(m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].rectROI, 0, 0, cvROI);
					cv::rectangle(matDisplay, cvROI, Scalar(128, 0, 0), 2);
				}

				SaveImage(0, matDisplay, "ThermalGlue", 1);	
			}

			return true;
		}

		// 將ROI的輪廓的分成4條邊界線
		// vt2ptLine[0] = top, vt2ptLine[1] = bottom, vt2ptLine[2] = left, vt2ptLine[3] = right
		bool MeasurementBlackGlue::ROI_ContoursToLine(const int& nSearchRangeX, const int& nSearchRangeY, SContoursParam* psContours, vector<vector<POINT>>& vt2ptLine)
		{
			if (nSearchRangeX < 0 || nSearchRangeY < 0 || psContours == nullptr) return false;
			int nCount_Contours = psContours->GetCount();
			int nCount = psContours->GetCount_On();
			if (nCount_Contours == 0 || nCount == 0) return true;

			int nImageH = psContours->GetImageHeight();
			int nImageW = psContours->GetImageWidth();
			if (nImageH <= 0 || nImageW <= 0) return false;

			vector<int> vtnTimes_Y(nImageH, 0);
			vector<int> vtnTimes_X(nImageW, 0);

			SJRect* psjRect = nullptr;
			vector<vector<POINT>>* pvt2ptLine = nullptr;
			int nSumPoint = 0, nCountPoint = 0, nCount_Line = 0;
			for (int k = 0; k < nCount_Contours; ++k) {
				bool* pbOn = psContours->GetOnPtr(k);
				if (pbOn == nullptr)	return false;

				if (*pbOn) {
					pvt2ptLine = psContours->GetLinePtr(k);
					psjRect = psContours->GetSJRectPtr(k);
					if (pvt2ptLine == nullptr || psjRect == nullptr) return false;

					nCount_Line = pvt2ptLine->size();
					for (int h = 0; h < nCount_Line; ++h) {
						nCountPoint = (*pvt2ptLine)[h].size();
						nSumPoint += nCountPoint;
						POINT* ptPoint = &(*pvt2ptLine)[h][0];
						for (int p = 0; p < nCountPoint; ++p) {
							++vtnTimes_Y[ptPoint[p].y];
							++vtnTimes_X[ptPoint[p].x];
						}
					}
					k = nCount_Contours;
				}
			}

			vt2ptLine.resize(4, vector<POINT>(nSumPoint));
			vector<int> vtnId(4, 0);
			vector<int> vtnSearch1(4, 0);
			vector<int> vtnSearch2(4, 0);
			POINT ptCenter = psjRect->GetCenter();

			int nRange = 10;
			int nMaxY = 0, nIndexY1 = 0, nIndexY2 = 0;
			vector<int> vtnSumY(nImageH, 0);
			for (int y = nRange; y < nImageH- nRange; ++y) {
				int nT = y - nRange;
				int nB = y + nRange;
				for (int r = nT; r < nB; ++r) {
					vtnSumY[y] += vtnTimes_Y[r];
				}
				if (vtnSumY[y] > nMaxY) {
					nMaxY = vtnSumY[y];
					nIndexY1 = y;
				}
			}

			nMaxY = 0;
			for (int y = nRange; y < nImageH - nRange; ++y) {
				if (abs(y - nIndexY1) > psjRect->nHeight / 2) {
					if (vtnSumY[y] > nMaxY) {
						nMaxY = vtnSumY[y];
						nIndexY2 = y;
					}
				}
			}

			vector<int> vtnSumX(nImageW, 0);
			int nMaxX = 0, nIndexX1 = 0, nIndexX2 = 0;
			for (int x = nRange; x < nImageW - nRange; ++x) {
				int nL = x - nRange;
				int nR = x + nRange;
				for (int r = nL; r < nR; ++r) {
					vtnSumX[x] += vtnTimes_X[r];
				}
				if (vtnSumX[x] > nMaxX) {
					nMaxX = vtnSumX[x];
					nIndexX1 = x;
				}
			}

			nMaxX = 0;
			for (int x = nRange; x < nImageW - nRange; ++x) {
				if (abs(x - nIndexX1) > psjRect->nWidth / 2) {
					if (vtnSumX[x] > nMaxX) {
						nMaxX = vtnSumX[x];
						nIndexX2 = x;
					}
				}		
			}

			// Y
			if (nIndexY1 < nIndexY2) {
				// top
				vtnSearch1[0] = nIndexY1 - nSearchRangeY;
				if (vtnSearch1[0] < 0) vtnSearch1[0] = 0;
				vtnSearch2[0] = nIndexY1 + nSearchRangeY;
				if (vtnSearch2[0] >= nImageH) vtnSearch2[0] = nImageH - 1;

				// bottom
				vtnSearch1[1] = nIndexY2 - nSearchRangeY;
				if (vtnSearch1[1] < 0) vtnSearch1[1] = 0;
				vtnSearch2[1] = nIndexY2 + nSearchRangeY;
				if (vtnSearch2[1] >= nImageH) vtnSearch2[1] = nImageH - 1;
			}
			else {
				// top
				vtnSearch1[0] = nIndexY2 - nSearchRangeY;
				if (vtnSearch1[0] < 0) vtnSearch1[0] = 0;
				vtnSearch2[0] = nIndexY2 + nSearchRangeY;
				if (vtnSearch2[0] >= nImageH) vtnSearch2[0] = nImageH - 1;

				// bottom
				vtnSearch1[1] = nIndexY1 - nSearchRangeY;
				if (vtnSearch1[1] < 0) vtnSearch1[1] = 0;
				vtnSearch2[1] = nIndexY1 + nSearchRangeY;
				if (vtnSearch2[1] >= nImageH) vtnSearch2[1] = nImageH - 1;
			}

			// X
			if (nIndexX1 < nIndexX2) {
				// left
				vtnSearch1[2] = nIndexX1 - nSearchRangeX;
				if (vtnSearch1[2] < 0) vtnSearch1[2] = 0;
				vtnSearch2[2] = nIndexX1 + nSearchRangeX;
				if (vtnSearch2[2] >= nImageW) vtnSearch2[2] = nImageW - 1;

				// right
				vtnSearch1[3] = nIndexX2 - nSearchRangeX;
				if (vtnSearch1[3] < 0) vtnSearch1[3] = 0;
				vtnSearch2[3] = nIndexX2 + nSearchRangeX;
			}
			else {
				// left
				vtnSearch1[2] = nIndexX2 - nSearchRangeX;
				if (vtnSearch1[2] < 0) vtnSearch1[2] = 0;
				vtnSearch2[2] = nIndexX2 + nSearchRangeX;
				if (vtnSearch2[2] >= nImageW) vtnSearch2[2] = nImageW - 1;

				// right
				vtnSearch1[3] = nIndexX1 - nSearchRangeX;
				if (vtnSearch1[3] < 0) vtnSearch1[3] = 0;
				vtnSearch2[3] = nIndexX1 + nSearchRangeX;
			}

			for (int h = 0; h < nCount_Line; ++h) {
				int nCount_Point = (*pvt2ptLine)[h].size();
				POINT* ptPoint = &(*pvt2ptLine)[h][0];
				for (int p = 0; p < nCount_Point; ++p) {
					int nY = ptPoint[p].y;
					int nX = ptPoint[p].x;
					for (int s = 0; s < 4; ++s) {
						if (vtnId[s] >= nSumPoint) continue;
						switch (s) {
						case 0:
						case 1:
							if (vtnSearch1[s] <= nY && nY <= vtnSearch2[s]) {
								vt2ptLine[s][vtnId[s]] = ptPoint[p];
								++vtnId[s];
							}
							break;

						case 2:
						case 3:
							if (vtnSearch1[s] <= nX && nX <= vtnSearch2[s]) {
								vt2ptLine[s][vtnId[s]] = ptPoint[p];
								++vtnId[s];
							}
							break;
						}
					}
				}
			}

			for (int s = 0; s < 4; ++s) {
				vt2ptLine[s].resize(vtnId[s]);
			}

			return true;
		}

		// vtdDieSlope[0]=top, vtdDieSlope[1]=bottom, vtdDieSlope[2]=left, vtdDieSlope[3]=right
		bool MeasurementBlackGlue::ReCreateDie(const int& nType, vector<vector<POINT>>& vt2ptLine, RECT& rectDie, vector<POINT>& vtptPoint, vector<double>& vtdDieSlope, vector<double>& vtdDieIntercept)
		{
			int nCount = vt2ptLine.size();
			if (nCount != 4) {
				return false;
			}

			vtdDieSlope.resize(nCount, 0.0);
			vtdDieIntercept.resize(nCount, 0.0);
			if (nType == 1) {
				for (int k = 0; k < nCount; ++k) {
					int nIndex_Start = vt2ptLine[k].size() * 0.25;
					int nIndex_End = vt2ptLine[k].size() - nIndex_Start;
					int nNumber = nIndex_End - nIndex_Start + 1;
					switch (k)
					{
					case 0:
					case 1:
					{
						std::sort(vt2ptLine[k].begin(), vt2ptLine[k].end(), [](const POINT& a, const POINT& b) {return a.y > b.y; });
						float fMeanY = std::accumulate(vt2ptLine[k].begin() + nIndex_Start, vt2ptLine[k].begin() + nIndex_End, 0.0,
							[](float fSum, const POINT& p) { return fSum + p.y; }) / nNumber;
						if (k == 0) {
							rectDie.top = (int)(fMeanY + 0.5);
						}
						else {
							rectDie.bottom = (int)(fMeanY + 0.5);
						}
						break;
					}
					case 2:
					case 3:
					{
						std::sort(vt2ptLine[k].begin(), vt2ptLine[k].end(), [](const POINT& a, const POINT& b) {return a.x > b.x; });
						float fMeanX = std::accumulate(vt2ptLine[k].begin() + nIndex_Start, vt2ptLine[k].begin() + nIndex_End, 0.0,
							[](float fSum, const POINT& p) { return fSum + p.x; }) / nNumber;
						if (k == 2) {
							rectDie.left = (int)(fMeanX + 0.5);
						}
						else {
							rectDie.right = (int)(fMeanX + 0.5);
						}
						break;
					}
					}
				}
			}
			else if (nType == 2) {	
				for (int k = 0; k < nCount; ++k) {
					if (k == 0 || k == 1) {
						if (!jet_imagefunction::SimpleLinearRegr(vt2ptLine[k], 5, 0.00001, vtdDieSlope[k], vtdDieIntercept[k])) {
							return false;
						}
					}
					else {
						if (!jet_imagefunction::SimpleLinearRegr_Vertical(vt2ptLine[k], 5, 0.00001, vtdDieSlope[k], vtdDieIntercept[k])) {
							return false;
						}
					}
				}

				double dSlope_H = (vtdDieSlope[0] + vtdDieSlope[1]) / 2.0;
				vtdDieSlope[0] = vtdDieSlope[1] = dSlope_H;
				double dSlope_V = (vtdDieSlope[2] + vtdDieSlope[3]) / 2.0;
				vtdDieSlope[2] = vtdDieSlope[3] = dSlope_V;

				for (int k = 0; k < nCount; ++k) {
					vtdDieIntercept[k] = 0.0;
					int nCount_Point = vt2ptLine[k].size();
					for (int h = 0; h < nCount_Point; ++h) {
						vtdDieIntercept[k] += (vt2ptLine[k][h].y - vtdDieSlope[k]* vt2ptLine[k][h].x);
					}	
					vtdDieIntercept[k] /= nCount_Point;
				}

				vtptPoint.resize(nCount);
				vector<Point2f> vtptfCrossPoint(nCount);
				// LT
				jet_imagefunction::GetCrossPoint(dSlope_V, vtdDieIntercept[2], dSlope_H, vtdDieIntercept[0], vtptfCrossPoint[0]);
				vtptPoint[0].x = vtptfCrossPoint[0].x + 0.5;
				vtptPoint[0].y = vtptfCrossPoint[0].y + 0.5;

				// RT
				jet_imagefunction::GetCrossPoint(dSlope_V, vtdDieIntercept[3], dSlope_H, vtdDieIntercept[0], vtptfCrossPoint[1]);
				vtptPoint[1].x = vtptfCrossPoint[1].x + 0.5;
				vtptPoint[1].y = vtptfCrossPoint[1].y + 0.5;

				// RB
				jet_imagefunction::GetCrossPoint(dSlope_V, vtdDieIntercept[3], dSlope_H, vtdDieIntercept[1], vtptfCrossPoint[2]);
				vtptPoint[2].x = vtptfCrossPoint[2].x + 0.5;
				vtptPoint[2].y = vtptfCrossPoint[2].y + 0.5;

				// LB
				jet_imagefunction::GetCrossPoint(dSlope_V, vtdDieIntercept[2], dSlope_H, vtdDieIntercept[1], vtptfCrossPoint[3]);
				vtptPoint[3].x = vtptfCrossPoint[3].x + 0.5;
				vtptPoint[3].y = vtptfCrossPoint[3].y + 0.5;

				rectDie.left = (vtptPoint[0].x < vtptPoint[2].x) ? vtptPoint[0].x : vtptPoint[2].x;
				rectDie.top = (vtptPoint[0].y < vtptPoint[1].y) ? vtptPoint[0].y : vtptPoint[1].y;
				rectDie.right = (vtptPoint[2].x > vtptPoint[1].x) ? vtptPoint[2].x : vtptPoint[1].x;
				rectDie.bottom = (vtptPoint[2].y > vtptPoint[3].y) ? vtptPoint[2].y : vtptPoint[3].y;
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matThermal.clone();
				int nDie_Left = m_sParam.sThermal.rectDie.left - m_sParam.sThermal.nDie_OutsetDistanceX;
				if (nDie_Left < 0) nDie_Left = 0;
				int nDie_Top = m_sParam.sThermal.rectDie.top - m_sParam.sThermal.nDie_OutsetDistanceY;
				if (nDie_Top < 0) nDie_Top = 0;
				int nDieW = m_sParam.sThermal.rectDie.right - m_sParam.sThermal.rectDie.left + 2 * m_sParam.sThermal.nDie_OutsetDistanceX;
				int nDieH = m_sParam.sThermal.rectDie.bottom - m_sParam.sThermal.rectDie.top + 2 * m_sParam.sThermal.nDie_OutsetDistanceY;
				Rect cvROI(nDie_Left, nDie_Top, nDieW, nDieH);

				Point cvPoint1, cvPoint2;
				cvPoint1.x = vtptPoint[0].x;
				cvPoint1.y = vtptPoint[0].y;
				cvPoint2.x = vtptPoint[1].x;
				cvPoint2.y = vtptPoint[1].y;
				line(matDisplay(cvROI), cvPoint1, cvPoint2, Scalar(255, 0, 0), 1);

				cvPoint1.x = vtptPoint[1].x;
				cvPoint1.y = vtptPoint[1].y;
				cvPoint2.x = vtptPoint[2].x;
				cvPoint2.y = vtptPoint[2].y;
				line(matDisplay(cvROI), cvPoint1, cvPoint2, Scalar(0, 255, 0), 1);

				cvPoint1.x = vtptPoint[2].x;
				cvPoint1.y = vtptPoint[2].y;
				cvPoint2.x = vtptPoint[3].x;
				cvPoint2.y = vtptPoint[3].y;
				line(matDisplay(cvROI), cvPoint1, cvPoint2, Scalar(0, 0, 255), 1);

				cvPoint1.x = vtptPoint[3].x;
				cvPoint1.y = vtptPoint[3].y;
				cvPoint2.x = vtptPoint[0].x;
				cvPoint2.y = vtptPoint[0].y;
				line(matDisplay(cvROI), cvPoint1, cvPoint2, Scalar(255, 255, 0), 1);
				SaveImage(0, matDisplay(cvROI), "Die_ROI_Slope", 1);
			}

			return true;
		}

		bool MeasurementBlackGlue::Sort_ROI(const int& nGlueDirection, vector<SJRect*>& vtpjRect, vector<RECT>& vtrectSort)
		{
			int nCount = vtpjRect.size();
			if (nCount <= 0) return false;

			if (nGlueDirection == 1) {
				std::sort(vtpjRect.begin(), vtpjRect.end(), [](const SJRect* a, const SJRect* b) {return a->GetCenter().y < b->GetCenter().y; });
			}
			else if (nGlueDirection == 2) {
				std::sort(vtpjRect.begin(), vtpjRect.end(), [](const SJRect* a, const SJRect* b) {return a->GetCenter().x < b->GetCenter().x; });
			}

			vtrectSort.resize(nCount);
			for (int k = 0; k < nCount; ++k) {
				vtrectSort[k].left = vtpjRect[k]->nX;
				vtrectSort[k].right = vtpjRect[k]->GetR();
				vtrectSort[k].top = vtpjRect[k]->nY;
				vtrectSort[k].bottom = vtpjRect[k]->GetB();
			}

			return true;
		}
#pragma endregion

#pragma region ROI Positioning

		// 計算水平膠寬的位置
		bool MeasurementBlackGlue::Calculate_RoiPosition(const bool& bChange, const int& nCount, const vector<RECT>& vtrectROI, SMultipleROI_Result& sResult)
		{
			bool bResult = false;
			switch (m_nPositioningMode_ROI) {
			case 1:
				bResult = Calculate_RoiPosition_Type1(nCount, vtrectROI, sResult);
				break;
			case 2:
				bResult = Calculate_RoiPosition_Type2(nCount, vtrectROI, sResult);
				break;
			default:
				return false;
			}

			if (bChange && bResult == false) {
				bResult = Calculate_RoiPosition_Change(nCount, vtrectROI, sResult);
			}

			return bResult;
		}

		// 依設定的位置
		bool MeasurementBlackGlue::Calculate_RoiPosition_Type1(const int& nCount, const vector<RECT>& vtrectROI, SMultipleROI_Result& sResult)
		{
			if (nCount <= 0 || nCount != vtrectROI.size()) {
				return false;
			}

			sResult.nRoiCount = nCount;
			sResult.vtsRoiInfo.resize(nCount);
			for (int k = 0; k < nCount; ++k) {
				if (!jet_imagefunction::Check_RECT(m_matGlue.rows, m_matGlue.cols, vtrectROI[k])) {
					return false;
				}
				sResult.vtsRoiInfo[k].rectROI = vtrectROI[k];
			}

			return true;
		}

		// 自動計算
		bool MeasurementBlackGlue::Calculate_RoiPosition_Type2(const int& nCount, const vector<RECT>& vtrectROI, SMultipleROI_Result& sResult)
		{
			if (nCount <= 0 || nCount != vtrectROI.size() || m_vtdCoefficient.size() != 6) {
				return false;
			}

			sResult.nRoiCount = nCount;
			sResult.vtsRoiInfo.resize(nCount);
			for (int k = 0; k < nCount; ++k) {
				if (!jet_imagefunction::Check_RECT(m_matGlue.rows, m_matGlue.cols, vtrectROI[k])) {
					return false;
				}

				if (!AffineTransform_RECT(m_vtdCoefficient, vtrectROI[k], sResult.vtsRoiInfo[k].rectROI)) {
					return false;
				}

				if (!jet_imagefunction::Check_RECT(m_matGlue.rows, m_matGlue.cols, sResult.vtsRoiInfo[k].rectROI)) {
					return false;
				}
			}

			return true;
		}

		// 自動切換模式
		bool MeasurementBlackGlue::Calculate_RoiPosition_Change(const int& nCount, const vector<RECT>& vtrectROI, SMultipleROI_Result& sResult)
		{
			bool bResult = false;
			switch (m_nPositioningMode_ROI) {
			case 1:
				bResult = CoordinateTransformation_Affine();
				if (bResult)
				{
					bResult = Calculate_RoiPosition_Type2(nCount, vtrectROI, sResult);
				}
				break;

			case 2:
				bResult = Calculate_RoiPosition_Type1(nCount, vtrectROI, sResult);
				break;

			default:
				return false;
			}

			return bResult;
		}

		// 計算散熱膠的量測範圍
		bool MeasurementBlackGlue::Calculate_ThermalGlueMeasurementRange(const RECT& rectDieROI, RECT& rectRange)
		{
			rectRange.left = m_sParam.sThermal.rectGlue_MeasurementRange.left + rectDieROI.left;
			rectRange.right = m_sParam.sThermal.rectGlue_MeasurementRange.right + rectDieROI.left;
			rectRange.top = m_sParam.sThermal.rectGlue_MeasurementRange.top + rectDieROI.top;
			rectRange.bottom = m_sParam.sThermal.rectGlue_MeasurementRange.bottom + rectDieROI.top;
			return true;
		}

		// Get Sort ROI
		bool MeasurementBlackGlue::GetSortROI(SContoursParam* psContours, const int& nCount, vector<RECT>& vtrectROI, vector<vector<POINT>>& vt2ptContoursPos)
		{
			if (psContours == nullptr || nCount <= 0) {
				return false;
			}

			int nCount_Contours = psContours->GetCount();
			if (nCount_Contours <= 0 || psContours->GetCount_On() <= 0) return true;

			vtrectROI.resize(nCount);
			vt2ptContoursPos.resize(nCount);

			POINT ptTemp;
			ptTemp.x = -1;
			ptTemp.y = 0;
			vector<POINT> vtptArea(nCount_Contours, ptTemp);
			for (int k = 0; k < nCount_Contours; ++k) {
				bool* pbOn = psContours->GetOnPtr(k);
				if (pbOn == nullptr)	return false;

				if (*pbOn) {
					SJRect* psjRect = psContours->GetSJRectPtr(k);
					if (psjRect == nullptr) return false;

					vtptArea[k].x = k;
					vtptArea[k].y = psjRect->GetArea();
				}
			}

			std::sort(vtptArea.begin(), vtptArea.end(), [](const POINT& a, const POINT& b) {return a.y > b.y; });
			for (int k = 0; k < nCount_Contours; ++k) {
				int nId = vtptArea[k].x;
				if (nId >= 0) {
					SJRect* psjRect = psContours->GetSJRectPtr(nId);
					for (int h = k + 1; h < nCount_Contours; ++h) {
						int nId2 = vtptArea[h].x;
						if (nId2 >= 0) {
							SJRect* psjRect2 = psContours->GetSJRectPtr(nId2);
							if (psjRect == nullptr || psjRect2 == nullptr) return false;
							if (psjRect->nX <= psjRect2->nX && psjRect->GetR() >= psjRect2->GetR() &&
								psjRect->nY <= psjRect2->nY && psjRect->GetB() >= psjRect2->GetB())
							{
								vtptArea[h].x = -1;
							}
						}
					}
				}
			}
			int nNumber = 0;
			for (int k = 0; k < nCount_Contours; ++k) {
				int nId = vtptArea[k].x;
				if (nId >= 0) {
					SJRect* psjRect = psContours->GetSJRectPtr(nId);
					vector<vector<POINT>>* pvt2ptLine = psContours->GetLinePtr(nId);
					if (psjRect == nullptr || pvt2ptLine == nullptr) return false;

					vtrectROI[nNumber].left = psjRect->nX;
					vtrectROI[nNumber].top = psjRect->nY;
					vtrectROI[nNumber].right = psjRect->GetR();
					vtrectROI[nNumber].bottom = psjRect->GetB();

					int nSumPoint = 0;
					int nCount_Line = pvt2ptLine->size();
					for (int h = 0; h < nCount_Line; ++h) {
						nSumPoint += (*pvt2ptLine)[h].size();
					}

					int nId2 = 0;
					vt2ptContoursPos[nNumber].resize(nSumPoint);
					for (int h = 0; h < nCount_Line; ++h) {
						int nCount_Point = (*pvt2ptLine)[h].size();
						for (int p = 0; p < nCount_Point; ++p) {
							vt2ptContoursPos[nNumber][nId2] = (*pvt2ptLine)[h][p];
							++nId2;
							if (nId2 >= nSumPoint) {
								p = nCount_Point;
								h = nCount_Line;
							}
						}
					}
					++nNumber;
					if (nNumber == nCount) {
						k = nCount_Contours;
					}
				}
			}

			return true;
		}

#pragma region StartEnd

		// 計算 StartROI 位置
		bool MeasurementBlackGlue::Calculate_StartROI(const bool& bChange)
		{
			if (m_sParam.sGlueStartEnd.nCount <= 0 || m_sParam.sGlueStartEnd.nCount != m_sParam.sGlueStartEnd.vtsStartEnd.size()) {
				return false;
			}

			bool bResult = false;
			for (int k = 0; k < m_sParam.sGlueStartEnd.nCount; ++k) {
				if (!jet_imagefunction::Check_RECT(m_matGlue.rows, m_matGlue.cols, m_sParam.sGlueStartEnd.vtsStartEnd[k].rectStartROI)) {
					return false;
				}

				switch (m_nPositioningMode_ROI)
				{
				case 1:
					bResult = Calculate_StartROI_Type1(k);
					break;

				case 2:
					bResult = Calculate_StartROI_Type2(k);
					break;

				default:
					return false;
				}

				if (bChange && bResult == false)
				{
					bResult = Calculate_StartROI_Change(k);
				}

				if (bResult)
				{
					if (!jet_imagefunction::Check_RECT(m_matGlue.rows, m_matGlue.cols, m_sResult.sStartEnd.vtsStartEnd[k].rectStart)) {
						return false;
					}
				}
			}

			return bResult;
		}

		// StartROI-依輸入位置
		bool MeasurementBlackGlue::Calculate_StartROI_Type1(const int nId)
		{
			// StartROI量測範圍
			m_sResult.sStartEnd.vtsStartEnd[nId].rectStart = m_sParam.sGlueStartEnd.vtsStartEnd[nId].rectStartROI;

			return true;
		}

		// StartROI-自動計算
		bool MeasurementBlackGlue::Calculate_StartROI_Type2(const int nId)
		{
			if (m_vtdCoefficient.size() != 6) {
				return false;
			}

			// 計算膠首ROI量測範圍
			if (!AffineTransform_RECT(m_vtdCoefficient, m_sParam.sGlueStartEnd.vtsStartEnd[nId].rectStartROI, m_sResult.sStartEnd.vtsStartEnd[nId].rectStart)) {
				return false;
			}

			return true;
		}

		// StartROI-自動切換模式
		bool MeasurementBlackGlue::Calculate_StartROI_Change(const int nId)
		{
			bool bResult = false;
			switch (m_nPositioningMode_ROI)
			{
			case 1:
				bResult = CoordinateTransformation_Affine();
				if (bResult) {
					bResult = Calculate_StartROI_Type2(nId);
				}
				break;

			case 2:
				bResult = Calculate_StartROI_Type1(nId);
				break;

			default:
				return false;
			}
			return bResult;
		}

		// 計算 EndROI 位置
		bool MeasurementBlackGlue::Calculate_EndROI(const bool& bChange)
		{
			if (m_sParam.sGlueStartEnd.nCount <= 0 || m_sParam.sGlueStartEnd.nCount != m_sParam.sGlueStartEnd.vtsStartEnd.size()) {
				return false;
			}

			bool bResult = false;
			for (int k = 0; k < m_sParam.sGlueStartEnd.nCount; ++k) {
				if (!jet_imagefunction::Check_RECT(m_matGlue.rows, m_matGlue.cols, m_sParam.sGlueStartEnd.vtsStartEnd[k].rectEndROI)) {
					return false;
				}

				switch (m_nPositioningMode_ROI)
				{
				case 1:
					bResult = Calculate_EndROI_Type1(k);
					break;

				case 2:
					bResult = Calculate_EndROI_Type2(k);
					break;

				default:
					return false;
				}

				if (bChange && bResult == false)
				{
					bResult = Calculate_EndROI_Change(k);
				}

				if (bResult)
				{
					if (!jet_imagefunction::Check_RECT(m_matGlue.rows, m_matGlue.cols, m_sResult.sStartEnd.vtsStartEnd[k].rectEnd)) {
						return false;
					}
				}
			}

			return bResult;
		}

		// EndROI-依輸入位置
		bool MeasurementBlackGlue::Calculate_EndROI_Type1(const int nId)
		{
			// EndROI量測範圍
			m_sResult.sStartEnd.vtsStartEnd[nId].rectEnd = m_sParam.sGlueStartEnd.vtsStartEnd[nId].rectEndROI;

			return true;
		}

		// EndROI-自動計算
		bool MeasurementBlackGlue::Calculate_EndROI_Type2(const int nId)
		{
			if (m_vtdCoefficient.size() != 6) {
				return false;
			}

			// 計算膠尾ROI量測範圍
			if (!AffineTransform_RECT(m_vtdCoefficient, m_sParam.sGlueStartEnd.vtsStartEnd[nId].rectEndROI, m_sResult.sStartEnd.vtsStartEnd[nId].rectEnd)) {
				return false;
			}

			return true;
		}

		// EndROI-自動切換模式
		bool MeasurementBlackGlue::Calculate_EndROI_Change(const int nId)
		{
			bool bResult = false;
			switch (m_nPositioningMode_ROI)
			{
			case 1:
				bResult = CoordinateTransformation_Affine();
				if (bResult) {
					bResult = Calculate_EndROI_Type2(nId);
				}
				break;

			case 2:
				bResult = Calculate_EndROI_Type1(nId);
				break;

			default:
				return false;
			}
			return bResult;
		}

		// 計算 StartROI EndROI 的弧線端點
		bool MeasurementBlackGlue::GetLimitPoint(const Mat& matGlueEdge, const RECT& rectROI, const int& nLocation, POINT& ptPos)
		{
			if (matGlueEdge.empty() || matGlueEdge.channels() != 1 || nLocation < 1 || nLocation > 8) {
				return false;
			}

			// 計算ROI的邊界
			int nT = std::max((int)rectROI.top, 0);
			int nB = std::min((int)rectROI.bottom, m_matGlue.rows - 1);
			int nL = std::max((int)rectROI.left, 0);
			int nR = std::min((int)rectROI.right, m_matGlue.cols - 1);

			// Lambda 函數表
			using GetPointFunction = std::function<bool(const int&, const int&, const int&, const int&)>;
			unordered_map<int, GetPointFunction> processMap = {
				{ 1, [&](const int& nT, const int& nB, const int& nL, const int& nR) {
					return GetLimitPoint_Location1(matGlueEdge, nT, nB, nL, nR, ptPos); }},
				{ 2, [&](const int& nT, const int& nB, const int& nL, const int& nR) {
					return GetLimitPoint_Location2(matGlueEdge, nT, nB, nL, nR, ptPos); } },
				{ 3, [&](const int& nT, const int& nB, const int& nL, const int& nR) {
					return GetLimitPoint_Location3(matGlueEdge, nT, nB, nL, nR, ptPos); } },
				{ 4, [&](const int& nT, const int& nB, const int& nL, const int& nR) {
					return GetLimitPoint_Location4(matGlueEdge, nT, nB, nL, nR, ptPos); } },
				{ 5, [&](const int& nT, const int& nB, const int& nL, const int& nR) {
					return GetLimitPoint_Location5(matGlueEdge, nT, nB, nL, nR, ptPos); } },
				{ 6, [&](const int& nT, const int& nB, const int& nL, const int& nR) {
					return GetLimitPoint_Location6(matGlueEdge, nT, nB, nL, nR, ptPos); } },
				{ 7, [&](const int& nT, const int& nB, const int& nL, const int& nR) {
					return GetLimitPoint_Location7(matGlueEdge, nT, nB, nL, nR, ptPos); } },
				{ 8, [&](const int& nT, const int& nB, const int& nL, const int& nR) {
					return GetLimitPoint_Location8(matGlueEdge, nT, nB, nL, nR, ptPos); } },
			};

			return processMap[nLocation](nT, nB, nL, nR);
		}

		bool MeasurementBlackGlue::GetLimitPoint_Location1(const Mat& matGlueEdge, const int& nT, const int& nB, const int& nL, const int& nR, POINT& ptPoint)
		{
			/*
			int nValue = 0;
			switch (nLocation)
			{
			case 2: // 最上
				nValue = 99999;
				for (int y = nT; y <= nB; ++y)
				{
					const uchar* pu8Edge = matGlueEdge.ptr<uchar>(y);
					for (int x = nL; x <= nR; ++x)
					{
						if (pu8Edge[x] == 255 && nValue > y)
						{
							nValue = y;
							ptPos.x = x;
							ptPos.y = y;
						}
					}
				}
				break;

			case 4: // 最右
				nValue = 0;
				for (int y = nT; y <= nB; ++y)
				{
					const uchar* pu8Edge = matGlueEdge.ptr<uchar>(y);
					for (int x = nL; x <= nR; ++x)
					{
						if (pu8Edge[x] == 255 && x > nValue)
						{
							nValue = x;
							ptPos.x = x;
							ptPos.y = y;
						}
					}
				}
				break;

			case 6: // 最下
				nValue = 0;
				for (int y = nT; y <= nB; ++y)
				{
					const uchar* pu8Edge = matGlueEdge.ptr<uchar>(y);
					for (int x = nL; x <= nR; ++x)
					{
						if (pu8Edge[x] == 255 && y > nValue)
						{
							nValue = y;
							ptPos.x = x;
							ptPos.y = y;
						}
					}
				}
				break;

			case 8: // 最左
				nValue = 9999;
				for (int y = nT; y <= nB; ++y)
				{
					const uchar* pu8Edge = matGlueEdge.ptr<uchar>(y);
					for (int x = nL; x <= nR; ++x)
					{
						if (pu8Edge[x] == 255 && x < nValue)
						{
							nValue = x;
							ptPos.x = x;
							ptPos.y = y;
						}
					}
				}
				break;
			}
			*/
			return true;
		}

		bool MeasurementBlackGlue::GetLimitPoint_Location2(const Mat& matGlueEdge, const int& nT, const int& nB, const int& nL, const int& nR, POINT& ptPoint)
		{
			int nY = 99999;
			int nX = 0;
			int nNum = 0;
			for (int y = nT; y <= nB; ++y)
			{
				const uchar* pu8Edge = matGlueEdge.ptr<uchar>(y);
				for (int x = nL; x <= nR; ++x)
				{
					if (pu8Edge[x] == 255 && nY > y)
					{
						nY = y;
						nX = x;
						nNum = 1;
					}
					else if (pu8Edge[x] == 255 && nY == y)
					{
						nX += x;
						++nNum;
					}
				}
			}

			if (nNum <= 0) return false;
			ptPoint.x = nX / nNum;
			ptPoint.y = nY;

			return true;
		}

		bool MeasurementBlackGlue::GetLimitPoint_Location3(const Mat& matGlueEdge, const int& nT, const int& nB, const int& nL, const int& nR, POINT& ptPoint)
		{

			return true;
		}

		bool MeasurementBlackGlue::GetLimitPoint_Location4(const Mat& matGlueEdge, const int& nT, const int& nB, const int& nL, const int& nR, POINT& ptPoint)
		{
			int nX = 0;
			int nY = 0;
			int nNum = 0;
			for (int y = nT; y <= nB; ++y)
			{
				const uchar* pu8Edge = matGlueEdge.ptr<uchar>(y);
				for (int x = nL; x <= nR; ++x)
				{
					if (pu8Edge[x] == 255 && x > nX)
					{
						nX = x;
						nY = y;
						nNum = 1;
					}
					else if (pu8Edge[x] == 255 && x == nX)
					{
						nY += y;
						++nNum;
					}
				}
			}

			if (nNum <= 0) return false;
			ptPoint.x = nX;
			ptPoint.y = nY / nNum;

			return true;
		}

		bool MeasurementBlackGlue::GetLimitPoint_Location5(const Mat& matGlueEdge, const int& nT, const int& nB, const int& nL, const int& nR, POINT& ptPoint)
		{

			return true;
		}

		bool MeasurementBlackGlue::GetLimitPoint_Location6(const Mat& matGlueEdge, const int& nT, const int& nB, const int& nL, const int& nR, POINT& ptPoint)
		{
			int nY = 0;
			int nX = 0;
			int nNum = 0;
			for (int y = nT; y <= nB; ++y)
			{
				const uchar* pu8Edge = matGlueEdge.ptr<uchar>(y);
				for (int x = nL; x <= nR; ++x) {
					if (pu8Edge[x] == 255 && y > nY) {
						nY = y;
						nX = x;
						nNum = 1;
					}
					else if (pu8Edge[x] == 255 && y == nY) {
						nX += x;
						++nNum;
					}
				}
			}

			if (nNum <= 0) return false;
			ptPoint.x = nX / nNum;
			ptPoint.y = nY;

			return true;
		}

		bool MeasurementBlackGlue::GetLimitPoint_Location7(const Mat& matGlueEdge, const int& nT, const int& nB, const int& nL, const int& nR, POINT& ptPoint)
		{

			return true;
		}

		bool MeasurementBlackGlue::GetLimitPoint_Location8(const Mat& matGlueEdge, const int& nT, const int& nB, const int& nL, const int& nR, POINT& ptPoint)
		{
			int nX = 9999;
			int nY = 0;
			int nNum = 0;
			for (int y = nT; y <= nB; ++y)
			{
				const uchar* pu8Edge = matGlueEdge.ptr<uchar>(y);
				for (int x = nL; x <= nR; ++x) {
					if (pu8Edge[x] == 255 && x < nX) {
						nX = x;
						nY = y;
						nNum = 1;
					}
					else if (pu8Edge[x] == 255 && x == nX) {
						nY += y;
						++nNum;
					}
				}
			}

			if (nNum <= 0) return false;
			ptPoint.x = nX;
			ptPoint.y = nY / nNum;

			return true;
		}
#pragma endregion


		// 使用仿射轉換進行座標轉換
		bool MeasurementBlackGlue::CoordinateTransformation_Affine()
		{
			// LT
			vector<pair<POINT, POINT>> vtInputPoint(4);
			vtInputPoint[0].first.x = m_sParam.sGlue.rectGlueROI.left;
			vtInputPoint[0].first.y = m_sParam.sGlue.rectGlueROI.top;
			vtInputPoint[0].second.x = m_sResult.sGlueEdge.rectROI.left;
			vtInputPoint[0].second.y = m_sResult.sGlueEdge.rectROI.top;

			// RT
			vtInputPoint[1].first.x = m_sParam.sGlue.rectGlueROI.right;
			vtInputPoint[1].first.y = m_sParam.sGlue.rectGlueROI.top;
			vtInputPoint[1].second.x = m_sResult.sGlueEdge.rectROI.right;
			vtInputPoint[1].second.y = m_sResult.sGlueEdge.rectROI.top;

			// RB
			vtInputPoint[2].first.x = m_sParam.sGlue.rectGlueROI.right;
			vtInputPoint[2].first.y = m_sParam.sGlue.rectGlueROI.bottom;
			vtInputPoint[2].second.x = m_sResult.sGlueEdge.rectROI.right;
			vtInputPoint[2].second.y = m_sResult.sGlueEdge.rectROI.bottom;

			// LB
			vtInputPoint[3].first.x = m_sParam.sGlue.rectGlueROI.left;
			vtInputPoint[3].first.y = m_sParam.sGlue.rectGlueROI.bottom;
			vtInputPoint[3].second.x = m_sResult.sGlueEdge.rectROI.left;
			vtInputPoint[3].second.y = m_sResult.sGlueEdge.rectROI.bottom;

			double dRate = 100;
			if (jet_imagefunction::GetAffine(vtInputPoint, dRate, m_vtdCoefficient) != 1) {
				return false;
			}

			if (m_vtdCoefficient.size() != 6) {
				return false;
			}

			return true;
		}

		// 進行仿射轉換
		// U = a11*x + a12*y + tx
		// V = a21*x + a22*y + ty
		// a11=vtdCoefficient[0], a12=vtdCoefficient[1], tx=vtdCoefficient[2]
		// a21=vtdCoefficient[3], a22=vtdCoefficient[4], ty=vtdCoefficient[5]
		bool MeasurementBlackGlue::AffineTransform_RECT(const std::vector<double> &vtdCoefficient, const RECT& rectROI, RECT& rectAffineROI)
		{
			// 1. 準備原始 ROI 的四個角點
			POINT pts[4] = {
				{rectROI.left,  rectROI.top},
				{rectROI.right, rectROI.top},
				{rectROI.right, rectROI.bottom},
				{rectROI.left,  rectROI.bottom}
			};

			// 2. 使用給定的變換係數，依序轉換每個角點
			POINT transformed[4];
			for (int i = 0; i < 4; ++i) {
				if (jet_imagefunction::AffineTransform(vtdCoefficient, pts[i], transformed[i]) != 1) {
					return false; // 若任何角點轉換失敗，就直接回傳 false
				}
			}

			// 3. 計算轉換後所有角點的包圍盒 (最小/最大 x 與 y)
			int minX = transformed[0].x;
			int maxX = transformed[0].x;
			int minY = transformed[0].y;
			int maxY = transformed[0].y;

			for (int i = 1; i < 4; ++i) {
				if (transformed[i].x < minX) minX = transformed[i].x;
				if (transformed[i].x > maxX) maxX = transformed[i].x;
				if (transformed[i].y < minY) minY = transformed[i].y;
				if (transformed[i].y > maxY) maxY = transformed[i].y;
			}

			// 4. 將包圍盒範圍限制(clamp)在影像 (m_matGlue) 的有效區域內
			//    m_matGlue.cols 代表影像寬度，m_matGlue.rows 代表影像高度
			if (minX < 0)                 minX = 0;
			if (minX >= m_matGlue.cols)   minX = m_matGlue.cols - 1;
			if (maxX < 0)                 maxX = 0;
			if (maxX >= m_matGlue.cols)   maxX = m_matGlue.cols - 1;
			if (minX >= maxX)             return false;

			if (minY < 0)                 minY = 0;
			if (minY >= m_matGlue.rows)   minY = m_matGlue.rows - 1;
			if (maxY < 0)                 maxY = 0;
			if (maxY >= m_matGlue.rows)   maxY = m_matGlue.rows - 1;
			if (minY >= maxY)             return false;

			// 5. 將最終結果寫入輸出 RECT
			rectAffineROI.left = minX;
			rectAffineROI.right = maxX;
			rectAffineROI.top = minY;
			rectAffineROI.bottom = maxY;

			return true;
		}

		// 輸入邊界影像與ROI, 取出ROI內的 線段
		bool MeasurementBlackGlue::CalculatePosition_Arc(const Mat& matEdge, const RECT& rectROI, SContoursParam& sContours)
		{
			if (matEdge.empty() || matEdge.channels() != 1 || !jet_imagefunction::Check_RECT(m_matGlue.rows, m_matGlue.cols, rectROI)) {
				return false;
			}

			Rect cvROI;
			if (!jet_imagefunction::RECTToRect(rectROI, 0, 0, cvROI, matEdge.rows, matEdge.cols)) {
				return false;
			}

			Mat matROI = matEdge(cvROI);
			if (!jet_imagefunction::SearchContours_Type1(matROI, true, false, true, 1, 1, matEdge.total(), sContours)) {
				return false;
			}

			int nCount = sContours.GetCount();
			if (nCount <= 0) return false;

			// 判斷方向性
			vector<bool> vtbDirection(nCount, true);
			for (int k = 0; k < nCount; ++k) {
				vector<vector<POINT>>* pvt2ptLine = sContours.GetLinePtr(k);
				if (pvt2ptLine == nullptr) return false;

				int nCount_Line = pvt2ptLine->size();
				if (nCount_Line > 0)
				{
					int nCount_Point = (*pvt2ptLine)[0].size();
					((*pvt2ptLine)[0][0].x > (*pvt2ptLine)[0][nCount_Point - 1].x) ? vtbDirection[k] = true : vtbDirection[k] = false;
				}
			}

			// 座標偏移
			sContours.SetImageSize(matEdge.rows, matEdge.cols);
			
			for (int k = 0; k < nCount; ++k) {
				vector<vector<POINT>>* pvt2ptLine = sContours.GetLinePtr(k);
				if (pvt2ptLine == nullptr) return false;

				int nCount_Line = pvt2ptLine->size();
				if (vtbDirection[k]) {
					for (int h = 0; h < nCount_Line; ++h) {
						int nCount_Point = (*pvt2ptLine)[h].size();
						for (int p = 0; p < nCount_Point; ++p) {
							(*pvt2ptLine)[h][p].x += rectROI.left;
							(*pvt2ptLine)[h][p].y += rectROI.top;
						}
					}
				}
				else {
					for (int h = 0; h < nCount_Line; ++h) {
						int nCount_Point = (*pvt2ptLine)[h].size();
						vector<POINT> vtptTemp(nCount_Point);
						std::reverse_copy((*pvt2ptLine)[h].begin(), (*pvt2ptLine)[h].end(), vtptTemp.begin());

						for (int p = 0; p < nCount_Point; ++p) {
							(*pvt2ptLine)[h][p].x = vtptTemp[p].x + rectROI.left;
							(*pvt2ptLine)[h][p].y = vtptTemp[p].y + rectROI.top;
						}
					}
				}
			}

			return true;
		}
#pragma endregion

#pragma region 量測

		// 水平膠寬量測
		bool MeasurementBlackGlue::Measurement_GlueWidth_Horizontal(const bool& bChange)
		{
			bool bResult = Measurement_GlueWidth_Horizontal();
			if (bChange && bResult == false)
			{
				bResult = Calculate_RoiPosition_Change(m_sParam.sGlueWidth_Horizontal.nCount, m_sParam.sGlueWidth_Horizontal.vtrectROI, m_sResult.sWidth_Horizontal);
				if (bResult) {
					bResult = Measurement_GlueWidth_Horizontal();
				}
			}

			if (m_bSaveImage && bResult)
			{
				Mat matDisplay = m_matGlue.clone();
				if (!DisplayRoiResult(m_sResult.sWidth_Horizontal, m_scROI_GlueWidth, m_scDistance_GlueWidth, m_scMaxDist_GlueWidth, m_scMinDist_GlueWidth, "GlueWidth_Horizontal", matDisplay)) {
					return false;
				}
				SaveImage(0, matDisplay, "Width_Horizontal", m_nExtensionType);
			}

			return bResult;
		}
		bool MeasurementBlackGlue::Measurement_GlueWidth_Horizontal()
		{
			if (!m_sResult.sWidth_Horizontal.bEnable || m_sParam.sGlueWidth_Horizontal.nMeanCount < 1) return true;
			int nCount_Horizontal = m_sParam.sGlueWidth_Horizontal.nCount;
			if (m_matEdgeMask.empty() || nCount_Horizontal <= 0 || nCount_Horizontal != m_sResult.sWidth_Horizontal.nRoiCount || m_sResult.sBoardEdge.dSlope_Horizontal == 99999.0) {
				return false;
			}

			double dSlope = 0.0;
			if (m_sResult.sBoardEdge.dSlope_Horizontal == 0.0) {
				dSlope = 999999.0;
			}
			else {
				dSlope = -1.0 / m_sResult.sBoardEdge.dSlope_Horizontal;
			}

			POINT ptStart, ptEnd;
			for (int k = 0; k < nCount_Horizontal; ++k) {
				int nCount = m_sResult.sWidth_Horizontal.vtsRoiInfo[k].rectROI.right - m_sResult.sWidth_Horizontal.vtsRoiInfo[k].rectROI.left;
				if (nCount <= 0) {
					continue;
				}

				if (m_sParam.sGlueWidth_Horizontal.nMeanCount > nCount) {
					return false;
				}

				m_sResult.sWidth_Horizontal.vtsRoiInfo[k].nPositionCount = nCount;
				m_sResult.sWidth_Horizontal.vtsRoiInfo[k].vtsPositionInfo.resize(nCount);
				for (int x = 0; x < nCount; ++x) {
					ptStart.x = m_sResult.sWidth_Horizontal.vtsRoiInfo[k].rectROI.left + x;
					ptStart.y = m_sResult.sWidth_Horizontal.vtsRoiInfo[k].rectROI.top;
					ptEnd.y = m_sResult.sWidth_Horizontal.vtsRoiInfo[k].rectROI.bottom;
					if (!jet_imagefunction::CalculatePointX(dSlope, ptStart, ptEnd)) {
						continue;
					}

					vector<POINT> vtptLine;
					if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, ptStart, ptEnd, vtptLine) != 1) {
						continue;
					}

					int nCount_Line = vtptLine.size();
					if (nCount_Line <= 0) continue;

					// find first
					for (int v = 0; v < nCount_Line; ++v) {
						if (vtptLine[v].x < 0 || vtptLine[v].y < 0 || vtptLine[v].x >= m_matEdgeMask.cols || vtptLine[v].y >= m_matEdgeMask.rows) {
							continue;
						}
						uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(vtptLine[v].y);
						if (pu8Edge[vtptLine[v].x] == 255)
						{
							m_sResult.sWidth_Horizontal.vtsRoiInfo[k].vtsPositionInfo[x].ptStart = vtptLine[v];
							m_sResult.sWidth_Horizontal.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd = vtptLine[v];
							v = nCount_Line;
						}
					}

					// find second
					for (int v = nCount_Line - 1; v >= 0; --v) {
						if (vtptLine[v].x < 0 || vtptLine[v].y < 0 || vtptLine[v].x >= m_matEdgeMask.cols || vtptLine[v].y >= m_matEdgeMask.rows) {
							continue;
						}
						uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(vtptLine[v].y);
						if (pu8Edge[vtptLine[v].x] == 255)
						{
							m_sResult.sWidth_Horizontal.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd = vtptLine[v];
							v = -1;
						}
					}

					// 兩點距離
					Calculate_Distance(m_sResult.sWidth_Horizontal.vtsRoiInfo[k].vtsPositionInfo[x]);
				}

				// 計算多點平均
				if (!CalcMeanDist(m_sParam.sGlueWidth_Horizontal.nMeanCount, m_sResult.sWidth_Horizontal.vtsRoiInfo[k])) {
					return false;
				}

				// 輸出最大 最小值 與 (最大-最小)
				if (!OutputMaxMinDistance(m_sResult.sWidth_Horizontal.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sWidth_Horizontal.vtsRoiInfo[k])) {
					continue;
				}

				// 判斷最大值範圍
				m_sResult.sWidth_Horizontal.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sWidth_Horizontal.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sGlueWidth_Horizontal.vtsLimit[k].sMaxDist);

				// 判斷最小值範圍
				m_sResult.sWidth_Horizontal.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sWidth_Horizontal.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sGlueWidth_Horizontal.vtsLimit[k].sMinDist);

				// 判斷(最大-最小)範圍
				m_sResult.sWidth_Horizontal.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sWidth_Horizontal.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sGlueWidth_Horizontal.vtsLimit[k].sDiff_MaxMin);
			}

			return true;
		}

		// 垂直膠寬量測
		bool MeasurementBlackGlue::Measurement_GlueWidth_Vertical(const bool& bChange)
		{
			bool bResult = Measurement_GlueWidth_Vertical();
			if (bChange && bResult == false)
			{
				bResult = Calculate_RoiPosition_Change(m_sParam.sGlueWidth_Vertical.nCount, m_sParam.sGlueWidth_Vertical.vtrectROI, m_sResult.sWidth_Vertical);
				if (bResult) {
					bResult = Measurement_GlueWidth_Vertical();
				}
			}

			if (m_bSaveImage && bResult)
			{
				Mat matDisplay = m_matGlue.clone();
				if (!DisplayRoiResult(m_sResult.sWidth_Vertical, m_scROI_GlueWidth, m_scDistance_GlueWidth, m_scMaxDist_GlueWidth, m_scMinDist_GlueWidth, "GlueWidth_Vertical", matDisplay)) {
					return false;
				}
				SaveImage(0, matDisplay, "Width_Vertical", m_nExtensionType);
			}

			return bResult;
		}
		bool MeasurementBlackGlue::Measurement_GlueWidth_Vertical()
		{
			if (!m_sResult.sWidth_Vertical.bEnable) return true;
			int nCount_Vertical = m_sParam.sGlueWidth_Vertical.nCount;
			if (m_matEdgeMask.empty() || nCount_Vertical <= 0 || nCount_Vertical != m_sResult.sWidth_Vertical.nRoiCount || m_sResult.sBoardEdge.dSlope_Vertical == 0.0) {
				return false;
			}

			// 計算每一點的座標與距離
			double dSlope = -1.0 / m_sResult.sBoardEdge.dSlope_Vertical;
			POINT ptStart, ptEnd;
			for (int k = 0; k < nCount_Vertical; ++k) {
				int nCount = m_sResult.sWidth_Vertical.vtsRoiInfo[k].rectROI.bottom - m_sResult.sWidth_Vertical.vtsRoiInfo[k].rectROI.top;
				if (nCount <= 0) {
					continue;
				}

				m_sResult.sWidth_Vertical.vtsRoiInfo[k].nPositionCount = nCount;
				m_sResult.sWidth_Vertical.vtsRoiInfo[k].vtsPositionInfo.resize(nCount);
				for (int y = 0; y < nCount; ++y) {
					ptStart.x = m_sResult.sWidth_Vertical.vtsRoiInfo[k].rectROI.left;
					ptStart.y = m_sResult.sWidth_Vertical.vtsRoiInfo[k].rectROI.top + y;
					ptEnd.x = m_sResult.sWidth_Vertical.vtsRoiInfo[k].rectROI.right;
					if (!jet_imagefunction::CalculatePointY(dSlope, ptStart, ptEnd)) {
						continue;
					}

					// L -> R
					vector<POINT> vtptLine;
					if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, ptStart, ptEnd, vtptLine) != 1) {
						continue;
					}

					int nCount_Line = vtptLine.size();
					if (nCount_Line <= 0) continue;

					// find first
					for (int x = 0; x < nCount_Line; ++x) {
						if (vtptLine[x].x < 0 || vtptLine[x].y < 0 || vtptLine[x].x >= m_matEdgeMask.cols || vtptLine[x].y >= m_matEdgeMask.rows) {
							continue;
						}
						uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(vtptLine[x].y);
						if (pu8Edge[vtptLine[x].x] == 255) {
							m_sResult.sWidth_Vertical.vtsRoiInfo[k].vtsPositionInfo[y].ptStart = vtptLine[x];
							m_sResult.sWidth_Vertical.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd = vtptLine[x];
							x = nCount_Line;
						}
					}

					// find second
					for (int x = nCount_Line - 1; x >= 0; --x) {
						if (vtptLine[x].x < 0 || vtptLine[x].y < 0 || vtptLine[x].x >= m_matEdgeMask.cols || vtptLine[x].y >= m_matEdgeMask.rows) {
							continue;
						}
						uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(vtptLine[x].y);
						if (pu8Edge[vtptLine[x].x] == 255) {
							m_sResult.sWidth_Vertical.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd = vtptLine[x];
							x = -1;
						}
					}

					// 兩點距離
					Calculate_Distance(m_sResult.sWidth_Vertical.vtsRoiInfo[k].vtsPositionInfo[y]);
				}

				// 計算多點平均
				if (!CalcMeanDist(m_sParam.sGlueWidth_Vertical.nMeanCount, m_sResult.sWidth_Vertical.vtsRoiInfo[k])) {
					return false;
				}

				// 輸出最大 最小值 與 (最大-最小)
				if (!OutputMaxMinDistance(m_sResult.sWidth_Vertical.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sWidth_Vertical.vtsRoiInfo[k])) {
					continue;
				}

				// 判斷最大值範圍
				m_sResult.sWidth_Vertical.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sWidth_Vertical.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sGlueWidth_Vertical.vtsLimit[k].sMaxDist);

				// 判斷最小值範圍
				m_sResult.sWidth_Vertical.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sWidth_Vertical.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sGlueWidth_Vertical.vtsLimit[k].sMinDist);

				// 判斷(最大-最小)範圍
				m_sResult.sWidth_Vertical.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sWidth_Vertical.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sGlueWidth_Vertical.vtsLimit[k].sDiff_MaxMin);
			}

			return true;
		}

		// 弧邊膠寬量測
		bool MeasurementBlackGlue::Measurement_GlueWidth_Arc(const bool& bChange)
		{
			bool bResult = Measurement_GlueWidth_Arc();
			if (bChange && bResult == false)
			{
				bResult = Calculate_RoiPosition_Change(m_sParam.sGlueArc.nCount, m_sParam.sGlueArc.vtrectROI, m_sResult.sWidth_Arc);
				if (bResult) {
					bResult = Measurement_GlueWidth_Arc();
				}
			}

			if (m_bSaveImage && bResult)
			{
				Mat matDisplay = m_matGlue.clone();
				if (!DisplayRoiResult(m_sResult.sWidth_Arc, m_scROI_GlueWidth, m_scDistance_GlueWidth, m_scMaxDist_GlueWidth, m_scMinDist_GlueWidth, "GlueWidth_Arc", matDisplay)) {
					return false;
				}
				SaveImage(0, matDisplay, "Width_Arc", m_nExtensionType);
			}

			return bResult;
		}
		bool MeasurementBlackGlue::Measurement_GlueWidth_Arc()
		{
			if (!m_sResult.sWidth_Arc.bEnable) return true;
			int nCount_Arc = m_sParam.sGlueArc.nCount;
			if (m_matEdgeMask.empty() || nCount_Arc <= 0 || nCount_Arc != m_sResult.sWidth_Arc.nRoiCount) {
				return false;
			}

			for (int k = 0; k < nCount_Arc; ++k) {
				// 從 邊界影像的 ROI中 找出2條 弧線
				SContoursParam sContours;
				if (!CalculatePosition_Arc(m_matEdgeMask, m_sResult.sWidth_Arc.vtsRoiInfo[k].rectROI, sContours)) {
					continue;
				}

				// 畫出弧線
				Mat matDisplay;
				if (!jet_imagefunction::DisplayContours(sContours, true, false, matDisplay, false)) {
					continue;
				}
				SaveImage(1, matDisplay, "Width_Arc", m_nExtensionType);

				int nCount_Contours = sContours.GetCount();
				if (nCount_Contours != 2) {
					continue;
				}

				// 依外接矩形面積判斷弧線大小
				int nMaxArea=0, nMaxId = -1;
				for (int h = 0; h < nCount_Contours; ++h) {
					SJRect* psjROI = sContours.GetSJRectPtr(h);
					if (psjROI == nullptr)	continue;
					int nArea = psjROI->GetArea();
					if (nArea > nMaxArea) {
						nMaxArea = nArea;
						nMaxId = h;
					}
				}
				if (nMaxId == -1) continue;

				int nSecondId = -1;
				nMaxArea = 0;
				for (int h = 0; h < nCount_Contours; ++h) {
					SJRect* psjROI = sContours.GetSJRectPtr(h);
					if (h == nMaxId || psjROI == nullptr) continue;
					int nArea = psjROI->GetArea();
					if (nArea > nMaxArea) {
						nMaxArea = nArea;
						nSecondId = h;
					}
				}
				if (nSecondId == -1) continue;

				// 計算最大弧線的總點數 
				vector<vector<POINT>>* p2ptLineMax = sContours.GetLinePtr(nMaxId);
				if (p2ptLineMax == nullptr) continue;
				int nCount_Line = p2ptLineMax->size();
				int nMaxCount = (*p2ptLineMax)[0].size();

				// 計算第2大弧線的總點數 
				vector<vector<POINT>>* p2ptLineSecond = sContours.GetLinePtr(nSecondId);
				if (p2ptLineSecond == nullptr) continue;
				int nSecondCount = (*p2ptLineSecond)[0].size();

				// 依點數比例進行連線
				float fRatio = ((float)nSecondCount)/((float)nMaxCount);
				m_sResult.sWidth_Arc.vtsRoiInfo[k].nPositionCount = nMaxCount;
				m_sResult.sWidth_Arc.vtsRoiInfo[k].vtsPositionInfo.resize(nMaxCount);
				for (int h = 0; h < nMaxCount; ++h) {
					int nId = static_cast<int>(h*fRatio + 0.5);
					if (nId < 0) nId = 0;
					if (nId >= nSecondCount) nId = nSecondCount - 1;
					m_sResult.sWidth_Arc.vtsRoiInfo[k].vtsPositionInfo[h].ptStart = (*p2ptLineMax)[0][h];
					m_sResult.sWidth_Arc.vtsRoiInfo[k].vtsPositionInfo[h].ptEnd = (*p2ptLineSecond)[0][nId];

					// 兩點距離
					Calculate_Distance(m_sResult.sWidth_Arc.vtsRoiInfo[k].vtsPositionInfo[h]);
				}

				// 計算多點平均
				if (!CalcMeanDist(m_sParam.sGlueArc.nMeanCount, m_sResult.sWidth_Arc.vtsRoiInfo[k])) {
					return false;
				}

				// 輸出最大 最小值 與 (最大-最小)
				if (!OutputMaxMinDistance(m_sResult.sWidth_Arc.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sWidth_Arc.vtsRoiInfo[k])) {
					continue;
				}

				// 判斷最大值範圍
				m_sResult.sWidth_Arc.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sWidth_Arc.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sGlueArc.vtsLimit[k].sMaxDist);

				// 判斷最小值範圍
				m_sResult.sWidth_Arc.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sWidth_Arc.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sGlueArc.vtsLimit[k].sMinDist);

				// 判斷(最大-最小)範圍
				m_sResult.sWidth_Arc.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sWidth_Arc.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sGlueArc.vtsLimit[k].sDiff_MaxMin);
			}

			return true;
		}
		
		// 水平方向板邊到膠邊的距離
		bool MeasurementBlackGlue::Measurement_BorderToGlue_Horizontal(const bool& bChange)
		{
			bool bResult = false;
			if (m_sParam.sBoard.bEnable_Slope) {
				bResult = Measurement_BorderToGlue_Horizontal_Slope();
				if (bChange && bResult == false)
				{
					bResult = Calculate_RoiPosition_Change(m_sParam.sBoardToGlue_Horizontal.nCount, m_sParam.sBoardToGlue_Horizontal.vtrectROI, m_sResult.sBoardToGlue_Horizontal);
					if (bResult) {
						bResult = Measurement_BorderToGlue_Horizontal_Slope();
					}
				}
			}
			else {
				bResult = Measurement_BorderToGlue_Horizontal_Defulat();
				if (bChange && bResult == false)
				{
					bResult = Calculate_RoiPosition_Change(m_sParam.sBoardToGlue_Horizontal.nCount, m_sParam.sBoardToGlue_Horizontal.vtrectROI, m_sResult.sBoardToGlue_Horizontal);
					if (bResult) {
						bResult = Measurement_BorderToGlue_Horizontal_Defulat();
					}
				}
			}

			if (m_bSaveImage && bResult)
			{
				Mat matDisplay = m_matGlue.clone();
				if (!DisplayRoiResult(m_sResult.sBoardToGlue_Horizontal, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, "BorderToGlue_Horizontal", matDisplay)) {
					return false;
				}
				SaveImage(0, matDisplay, "BoardToGlue_Horizontal", m_nExtensionType);
			}
			return bResult;
		}

		// 使用板邊計算斜率
		bool MeasurementBlackGlue::Measurement_BorderToGlue_Horizontal_Slope()
		{
			if (!m_sResult.sBoardToGlue_Horizontal.bEnable) return true;
			int nCount_Horizontal = m_sParam.sBoardToGlue_Horizontal.nCount;
			if (m_matEdgeMask.empty() || nCount_Horizontal <= 0 || nCount_Horizontal != m_sResult.sBoardToGlue_Horizontal.nRoiCount || m_sResult.sBoardEdge.dSlope_Vertical == 0.0) {
				return false;
			}

			// 計算每一點的座標與距離
			double dSlope = -1.0 / m_sResult.sBoardEdge.dSlope_Vertical;
			POINT ptStart, ptEnd;
			for (int k = 0; k < nCount_Horizontal; ++k) {
				int nCount = m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.bottom - m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.top;
				if (nCount <= 0) {
					continue;
				}

				int nLocation = m_sParam.sGlue.nLocation;
				if (nLocation == 9) {
					nLocation = (m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.left < m_matBoard.cols- m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.right)? 1:9;
				}

				m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].nPositionCount = nCount;
				m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo.resize(nCount);
				for (int y = 0; y < nCount; ++y) {
					ptStart.x = m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.left;
					ptStart.y = m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.top + y;
					ptEnd.x = m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.right;
					if (!jet_imagefunction::CalculatePointY(dSlope, ptStart, ptEnd)) {
						continue;
					}

					// L -> R
					vector<POINT> vtptLine;
					if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, ptStart, ptEnd, vtptLine) != 1) {
						continue;
					}

					int nCount_Line = vtptLine.size();
					if (nCount_Line <= 0) continue;

					// find first
					switch (nLocation)
					{
					case 1: // L
					case 7:
					case 8:
						for (int p = 0; p < nCount_Line; ++p) {
							if (vtptLine[p].x < 0 || vtptLine[p].y < 0 || vtptLine[p].x >= m_matEdgeMask.cols || vtptLine[p].y >= m_matEdgeMask.rows) {
								continue;
							}
							uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(vtptLine[p].y);
							if (pu8Edge[vtptLine[p].x] == 255) {
								m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptStart = vtptLine[p];
								p = nCount_Line;
							}
						}
						break;
					case 3: // R
					case 4:
					case 5:
					case 9:
						for (int p = nCount_Line - 1; p >= 0; --p) {
							if (vtptLine[p].x < 0 || vtptLine[p].y < 0 || vtptLine[p].x >= m_matEdgeMask.cols || vtptLine[p].y >= m_matEdgeMask.rows) {
								continue;
							}
							uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(vtptLine[p].y);
							if (pu8Edge[vtptLine[p].x] == 255) {
								m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptStart = vtptLine[p];
								p = -1;
							}
						}
						break;

					default:
						return false;
					}

					// find second
					if (nLocation == 9) {
						if (!jet_imagefunction::CalculateIntersection(m_sResult.sBoardEdge.dSlope_Vertical, m_sResult.sBoardEdge.dIntercept_Vertical2, m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptStart, m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd)) {
							continue;
						}
					}
					else {
						if (!jet_imagefunction::CalculateIntersection(m_sResult.sBoardEdge.dSlope_Vertical, m_sResult.sBoardEdge.dIntercept_Vertical, m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptStart, m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd)) {
							continue;
						}
					}

					// 兩點距離
					Calculate_Distance(m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y]);
				}

				// 計算多點平均
				if (!CalcMeanDist(m_sParam.sBoardToGlue_Horizontal.nMeanCount, m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k])) {
					return false;
				}

				// 輸出最大 最小值 與 (最大-最小)
				if (!OutputMaxMinDistance(m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k])) {
					continue;
				}

				// 判斷最大值範圍
				m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sBoardToGlue_Horizontal.vtsLimit[k].sMaxDist);

				// 判斷最小值範圍
				m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sBoardToGlue_Horizontal.vtsLimit[k].sMinDist);

				// 判斷(最大-最小)範圍
				m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sBoardToGlue_Horizontal.vtsLimit[k].sDiff_MaxMin);
			}

			return true;
		}

		// 使用水平斜率
		bool MeasurementBlackGlue::Measurement_BorderToGlue_Horizontal_Defulat()
		{
			if (!m_sResult.sBoardToGlue_Horizontal.bEnable) return true;
			int nCount_Horizontal = m_sParam.sBoardToGlue_Horizontal.nCount;
			if (m_matEdgeMask.empty() || m_matBoardEdge.empty() || nCount_Horizontal <= 0 || nCount_Horizontal != m_sResult.sBoardToGlue_Horizontal.nRoiCount) {
				return false;
			}

			POINT ptStart, ptEnd, ptCenter;
			for (int k = 0; k < nCount_Horizontal; ++k) {
				int nCount = m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.bottom - m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.top;	
				if (nCount <= 0) {
					continue;
				}
				
				int nRoiWidth = 0;
				m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].nPositionCount = nCount;
				m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo.resize(nCount);
				for (int y = 0; y < nCount; ++y) {
					ptStart.x = m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.left;
					ptStart.y = m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.top + y;
					ptEnd.x = m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.right;

					POINT ptTemp;
					ptTemp.y = ptStart.y;
					uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(ptTemp.y);
					uchar* pu8Board = m_matBoardEdge.ptr<uchar>(ptTemp.y);
					switch (m_sParam.sGlue.nLocation)
					{
					case 1: 
					case 7:
					case 8:
						// L
						nRoiWidth = m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.right;
						for (int p = 0; p < nRoiWidth; ++p) {
							ptTemp.x = p;
							if (pu8Board[ptTemp.x] == 255) {
								m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptStart = ptTemp;
								p = nRoiWidth;
							}
						}

						// R
						nRoiWidth = m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.right - m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.left;
						for (int p = 0; p < nRoiWidth; ++p) {
							ptTemp.x = ptEnd.x - p;
							if (ptTemp.x < 1 || ptTemp.x >= m_matEdgeMask.cols - 1) continue;
							if (pu8Edge[ptTemp.x-1] == 0 && pu8Edge[ptTemp.x] == 255) {
								m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd = ptTemp;
								p = nRoiWidth;
							}
						}
						break;
					case 3: 
					case 4:
					case 5:
						// R
						nRoiWidth = m_matBoardEdge.cols - m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.left;
						for (int p = 0; p < nRoiWidth; ++p) {
							ptTemp.x = m_matBoardEdge.cols - p;
							if (pu8Board[ptTemp.x] == 255) {
								m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptStart = ptTemp;
								p = nRoiWidth;
							}
						}

						// L
						nRoiWidth = m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.right - m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.left;
						for (int p = 0; p < nRoiWidth; ++p) {
							ptTemp.x = ptStart.x + p;
							if (ptTemp.x < 1 || ptTemp.x >= m_matEdgeMask.cols - 1) continue;
							if (pu8Edge[ptTemp.x] == 255 && pu8Edge[ptTemp.x+1] == 0) {
								m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd = ptTemp;
								p = nRoiWidth;
							}
						}
						break;
					case 9:
						ptCenter.x = m_matEdgeMask.cols / 2;
						ptCenter.y = m_matEdgeMask.rows / 2;
						if (m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.right < ptCenter.x) {
							// L
							nRoiWidth = m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.right;
							for (int p = 0; p < nRoiWidth; ++p) {
								ptTemp.x = p;
								if (pu8Board[ptTemp.x] == 255) {
									m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptStart = ptTemp;
									p = nRoiWidth;
								}
							}

							// R
							nRoiWidth = m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.right - m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.left;
							for (int p = 0; p < nRoiWidth; ++p) {
								ptTemp.x = ptEnd.x - p;
								if (ptTemp.x < 1 || ptTemp.x >= m_matEdgeMask.cols - 1) continue;
								if (pu8Edge[ptTemp.x - 1] == 0 && pu8Edge[ptTemp.x] == 255) {
									m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd = ptTemp;
									p = nRoiWidth;
								}
							}
						}
						else {
							// R
							nRoiWidth = m_matBoardEdge.cols - m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.left;
							for (int p = 0; p < nRoiWidth; ++p) {
								ptTemp.x = m_matBoardEdge.cols - p;
								if (pu8Board[ptTemp.x] == 255) {
									m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptStart = ptTemp;
									p = nRoiWidth;
								}
							}

							// L
							nRoiWidth = m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.right - m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].rectROI.left;
							for (int p = 0; p < nRoiWidth; ++p) {
								ptTemp.x = ptStart.x + p;
								if (ptTemp.x < 1 || ptTemp.x >= m_matEdgeMask.cols - 1) continue;
								if (pu8Edge[ptTemp.x] == 255 && pu8Edge[ptTemp.x + 1] == 0) {
									m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd = ptTemp;
									p = nRoiWidth;
								}
							}
						}
						break;
					default:
						return false;
					}

					// 兩點距離
					Calculate_Distance(m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y]);
				}

				// 計算多點平均
				if (!CalcMeanDist(m_sParam.sBoardToGlue_Horizontal.nMeanCount, m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k])) {
					return false;
				}

				// 輸出最大 最小值 與 (最大-最小)
				if (!OutputMaxMinDistance(m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k])) {
					continue;
				}

				// 判斷最大值範圍
				m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sBoardToGlue_Horizontal.vtsLimit[k].sMaxDist);

				// 判斷最小值範圍
				m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sBoardToGlue_Horizontal.vtsLimit[k].sMinDist);

				// 判斷(最大-最小)範圍
				m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sBoardToGlue_Horizontal.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sBoardToGlue_Horizontal.vtsLimit[k].sDiff_MaxMin);
			}

			return true;
		}

		// 垂直方向板邊到膠邊的距離
		bool MeasurementBlackGlue::Measurement_BorderToGlue_Vertical(const bool& bChange)
		{
			bool bResult = false;
			if (m_sParam.sBoard.bEnable_Slope) {
				bResult = Measurement_BorderToGlue_Vertical_Slope();
				if (bChange && bResult == false)
				{
					bResult = Calculate_RoiPosition_Change(m_sParam.sBoardToGlue_Vertical.nCount, m_sParam.sBoardToGlue_Vertical.vtrectROI, m_sResult.sBoardToGlue_Vertical);
					if (bResult) {
						bResult = Measurement_BorderToGlue_Vertical_Slope();
					}
				}
			}
			else {
				bResult = Measurement_BorderToGlue_Vertical_Defulat();
				if (bChange && bResult == false)
				{
					bResult = Calculate_RoiPosition_Change(m_sParam.sBoardToGlue_Vertical.nCount, m_sParam.sBoardToGlue_Vertical.vtrectROI, m_sResult.sBoardToGlue_Vertical);
					if (bResult) {
						bResult = Measurement_BorderToGlue_Vertical_Defulat();
					}
				}
			}

			if (m_bSaveImage && bResult)
			{
				Mat matDisplay = m_matGlue.clone();
				if (!DisplayRoiResult(m_sResult.sBoardToGlue_Vertical, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, "BorderToGlue_Vertical", matDisplay)) {
					return false;
				}
				SaveImage(0, matDisplay, "BoardToGlue_Vertical", m_nExtensionType);
			}
			return bResult;
		}

		// 使用板邊計算斜率
		bool MeasurementBlackGlue::Measurement_BorderToGlue_Vertical_Slope()
		{
			if (!m_sResult.sBoardToGlue_Vertical.bEnable) return true;
			int nCount_Vertical = m_sParam.sBoardToGlue_Vertical.nCount;
			if (m_matEdgeMask.empty() || nCount_Vertical <= 0 || nCount_Vertical != m_sResult.sBoardToGlue_Vertical.nRoiCount) {
				return false;
			}

			// 計算每一點的座標與距離
			double dSlope = 0.0;
			if (m_sResult.sBoardEdge.dSlope_Horizontal == 0.0) {
				dSlope = 999999.0;
			}
			else {
				dSlope = -1.0 / m_sResult.sBoardEdge.dSlope_Horizontal;
			}

			POINT ptStart, ptEnd;
			for (int k = 0; k < nCount_Vertical; ++k) {
				int nCount = m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.right - m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.left;
				if (nCount <= 0) {
					continue;
				}

				int nLocation = m_sParam.sGlue.nLocation;
				if (nLocation == 9) {
					nLocation = (m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.top < m_matBoard.rows - m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.bottom) ? 1 : 9;
				}

				m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].nPositionCount = nCount;
				m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo.resize(nCount);
				for (int x = 0; x < nCount; ++x) {
					ptStart.x = m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.left + x;
					ptStart.y = m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.top;
					ptEnd.y = m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.bottom;
					if (!jet_imagefunction::CalculatePointX(dSlope, ptStart, ptEnd)) {
						continue;
					}

					// T -> B
					vector<POINT> vtptLine;
					if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, ptStart, ptEnd, vtptLine) != 1) {
						continue;
					}

					int nCount_Line = vtptLine.size();
					if (nCount_Line <= 0) continue;

					// find first
					switch (nLocation)
					{
					case 1: // T
					case 2:
					case 3:
						for (int p = 0; p < nCount_Line; ++p) {
							if (vtptLine[p].x < 0 || vtptLine[p].y < 0 || vtptLine[p].x >= m_matEdgeMask.cols || vtptLine[p].y >= m_matEdgeMask.rows) {
								continue;
							}
							uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(vtptLine[p].y);
							if (pu8Edge[vtptLine[p].x] == 255) {
								m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptStart = vtptLine[p];
								p = nCount_Line;
							}
						}
						break;
					case 5: // B
					case 6:
					case 7:
					case 9:
						for (int p = nCount_Line - 1; p >= 0; --p) {
							if (vtptLine[p].x < 0 || vtptLine[p].y < 0 || vtptLine[p].x >= m_matEdgeMask.cols || vtptLine[p].y >= m_matEdgeMask.rows) {
								continue;
							}
							uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(vtptLine[p].y);
							if (pu8Edge[vtptLine[p].x] == 255) {
								m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptStart = vtptLine[p];
								p = -1;
							}
						}
						break;
					default:
						return false;
					}

					// find second
					if (nLocation == 9) {
						if (!jet_imagefunction::CalculateIntersection(m_sResult.sBoardEdge.dSlope_Horizontal, m_sResult.sBoardEdge.dIntercept_Horizontal2, m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptStart, m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd)) {
							continue;
						}
					}
					else {
						if (!jet_imagefunction::CalculateIntersection(m_sResult.sBoardEdge.dSlope_Horizontal, m_sResult.sBoardEdge.dIntercept_Horizontal, m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptStart, m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd)) {
							continue;
						}
					}

					// 兩點距離
					Calculate_Distance(m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x]);
				}

				// 計算多點平均
				if (!CalcMeanDist(m_sParam.sBoardToGlue_Vertical.nMeanCount, m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k])) {
					return false;
				}

				// 輸出最大 最小值 與 (最大-最小)
				if (!OutputMaxMinDistance(m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k])) {
					continue;
				}

				// 判斷最大值範圍
				m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sBoardToGlue_Vertical.vtsLimit[k].sMaxDist);

				// 判斷最小值範圍
				m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sBoardToGlue_Vertical.vtsLimit[k].sMinDist);

				// 判斷(最大-最小)範圍
				m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sBoardToGlue_Vertical.vtsLimit[k].sDiff_MaxMin);
			}
			return true;
		}

		// 使用垂直斜率
		bool MeasurementBlackGlue::Measurement_BorderToGlue_Vertical_Defulat()
		{
			if (!m_sResult.sBoardToGlue_Vertical.bEnable) return true;
			int nCount_Vertical = m_sParam.sBoardToGlue_Vertical.nCount;
			if (m_matEdgeMask.empty() || m_matBoardEdge.empty() || nCount_Vertical <= 0 || nCount_Vertical != m_sResult.sBoardToGlue_Vertical.nRoiCount) {
				return false;
			}

			POINT ptStart, ptEnd, ptCenter;
			for (int k = 0; k < nCount_Vertical; ++k) {
				int nCount = m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.right - m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.left;
				if (nCount <= 0) {
					continue;
				}

				int nRoi_Height = 0;
				m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].nPositionCount = nCount;
				m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo.resize(nCount);
				for (int x = 0; x < nCount; ++x) {
					ptStart.x = m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.left + x;
					ptStart.y = m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.top;
					ptEnd.y = m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.bottom;

					POINT ptTemp;
					ptTemp.x = ptStart.x;	
					switch (m_sParam.sGlue.nLocation)
					{
					case 1: 
					case 2:
					case 3:
						// T
						nRoi_Height = m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.bottom;
						for (int p = 0; p < nRoi_Height; ++p) {
							ptTemp.y = p;
							uchar* pu8Board = m_matBoardEdge.ptr<uchar>(ptTemp.y);
							if (pu8Board[ptTemp.x] == 255) {
								m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptStart = ptTemp;
								p = nRoi_Height;
							}
						}

						// B
						nRoi_Height = m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.bottom - m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.top;
						for (int p = 0; p < nRoi_Height; ++p) {
							ptTemp.y = ptEnd.y - p;
							if (ptTemp.y < 1 || ptTemp.y >= m_matEdgeMask.rows - 1) continue;
							uchar* pu8Edge_U = m_matEdgeMask.ptr<uchar>(ptTemp.y-1);
							uchar* pu8Edge_D = m_matEdgeMask.ptr<uchar>(ptTemp.y);
							if (pu8Edge_U[ptTemp.x] == 0 && pu8Edge_D[ptTemp.x] == 255) {
								m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd = ptTemp;
								p = nRoi_Height;
							}
						}
						break;
					case 5: 
					case 6:
					case 7:
						// B
						nRoi_Height = m_matBoardEdge.rows - m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.top;
						for (int p = 0; p < nRoi_Height; ++p) {
							ptTemp.y = m_matBoardEdge.rows - p - 1;
							uchar* pu8Board = m_matBoardEdge.ptr<uchar>(ptTemp.y);
							if (pu8Board[ptTemp.x] == 255) {
								m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptStart = ptTemp;
								p = nRoi_Height;
							}
						}

						// T
						nRoi_Height = m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.bottom - m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.top;
						for (int p = 0; p < nRoi_Height; ++p) {
							ptTemp.y = ptStart.y + p;
							if (ptTemp.y < 1 || ptTemp.y >= m_matEdgeMask.rows - 1) continue;
							uchar* pu8Edge_U = m_matEdgeMask.ptr<uchar>(ptTemp.y);
							uchar* pu8Edge_D = m_matEdgeMask.ptr<uchar>(ptTemp.y + 1);
							if (pu8Edge_U[ptTemp.x] == 255 && pu8Edge_D[ptTemp.x] == 0) {
								m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd = ptTemp;
								p = nRoi_Height;
							}
						}
						break;
					case 9:
						ptCenter.x = m_matEdgeMask.cols / 2;
						ptCenter.y = m_matEdgeMask.rows / 2;
						if (m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.bottom < ptCenter.y) {
							nRoi_Height = m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.bottom;
							for (int p = 0; p < nRoi_Height; ++p) {
								ptTemp.y = p;
								uchar* pu8Board = m_matBoardEdge.ptr<uchar>(ptTemp.y);
								if (pu8Board[ptTemp.x] == 255) {
									m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptStart = ptTemp;
									p = nRoi_Height;
								}
							}

							// B
							nRoi_Height = m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.bottom - m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.top;
							for (int p = 0; p < nRoi_Height; ++p) {
								ptTemp.y = ptEnd.y - p;
								if (ptTemp.y < 1 || ptTemp.y >= m_matEdgeMask.rows - 1) continue;
								uchar* pu8Edge_U = m_matEdgeMask.ptr<uchar>(ptTemp.y - 1);
								uchar* pu8Edge_D = m_matEdgeMask.ptr<uchar>(ptTemp.y);
								if (pu8Edge_U[ptTemp.x] == 0 && pu8Edge_D[ptTemp.x] == 255) {
									m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd = ptTemp;
									p = nRoi_Height;
								}
							}
						}
						else {
							// B
							nRoi_Height = m_matBoardEdge.rows - m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.top;
							for (int p = 0; p < nRoi_Height; ++p) {
								ptTemp.y = m_matBoardEdge.rows - p - 1;
								uchar* pu8Board = m_matBoardEdge.ptr<uchar>(ptTemp.y);
								if (pu8Board[ptTemp.x] == 255) {
									m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptStart = ptTemp;
									p = nRoi_Height;
								}
							}

							// T
							nRoi_Height = m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.bottom - m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].rectROI.top;
							for (int p = 0; p < nRoi_Height; ++p) {
								ptTemp.y = ptStart.y + p;
								if (ptTemp.y < 1 || ptTemp.y >= m_matEdgeMask.rows - 1) continue;
								uchar* pu8Edge_U = m_matEdgeMask.ptr<uchar>(ptTemp.y);
								uchar* pu8Edge_D = m_matEdgeMask.ptr<uchar>(ptTemp.y + 1);
								if (pu8Edge_U[ptTemp.x] == 255 && pu8Edge_D[ptTemp.x] == 0) {
									m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd = ptTemp;
									p = nRoi_Height;
								}
							}
						}
						break;
					default:
						return false;
					}

					// 兩點距離
					Calculate_Distance(m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x]);
				}

				// 計算多點平均
				if (!CalcMeanDist(m_sParam.sBoardToGlue_Vertical.nMeanCount, m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k])) {
					return false;
				}

				// 輸出最大 最小值 與 (最大-最小)
				if (!OutputMaxMinDistance(m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k])) {
					continue;
				}

				// 判斷最大值範圍
				m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sBoardToGlue_Vertical.vtsLimit[k].sMaxDist);

				// 判斷最小值範圍
				m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sBoardToGlue_Vertical.vtsLimit[k].sMinDist);

				// 判斷(最大-最小)範圍
				m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sBoardToGlue_Vertical.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sBoardToGlue_Vertical.vtsLimit[k].sDiff_MaxMin);
			}
			return true;
		}

		// 水平方向Coating邊到膠邊的距離
		bool MeasurementBlackGlue::Measurement_CoatingToGlue_Horizontal(const bool& bChange)
		{
			bool bResult = Measurement_CoatingToGlue_Horizontal();
			if (bChange && bResult == false)
			{
				bResult = Calculate_RoiPosition_Change(m_sParam.sCoatingToGlue_Horizontal.nCount, m_sParam.sCoatingToGlue_Horizontal.vtrectROI, m_sResult.sCoatingToGlue_Horizontal);
				if (bResult) {
					bResult = Measurement_CoatingToGlue_Horizontal();
				}
			}

			if (m_bSaveImage && bResult)
			{
				Mat matDisplay = m_matCoating.clone();
				// 畫 Coating 輪廓
				if (!DisplayCoatingPosition(m_sResult.sCoatingEdge, m_scContours_Coating, matDisplay)) {
					return false;
				}

				if (!DisplayRoiResult(m_sResult.sCoatingToGlue_Horizontal, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, "CoatingToGlue_Horizontal", matDisplay, true, true, false, true)) {
					return false;
				}
				SaveImage(0, matDisplay, "CoatingToGlue_Horizontal", m_nExtensionType);
			}
			return bResult;
		}
		bool MeasurementBlackGlue::Measurement_CoatingToGlue_Horizontal()
		{
			if (!m_sResult.sCoatingToGlue_Horizontal.bEnable) return true;
			int nCount_Horizontal = m_sParam.sCoatingToGlue_Horizontal.nCount;
			if (m_matEdgeMask.empty() || nCount_Horizontal <= 0 || nCount_Horizontal != m_sResult.sCoatingToGlue_Horizontal.nRoiCount || m_sResult.sBoardEdge.dSlope_Vertical == 0.0) {
				return false;
			}

			// 計算每一點的座標與距離
			double dSlope = -1.0 / m_sResult.sBoardEdge.dSlope_Vertical;
			POINT ptStart, ptEnd;
			for (int k = 0; k < nCount_Horizontal; ++k) {
				int nCount = m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].rectROI.bottom - m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].rectROI.top;
				if (nCount <= 0) {
					return false;
				}

				int nCrnterX = m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].rectROI.left;
				int nId_Coating = -1;
				for (int h = 0; h < m_sResult.sCoatingEdge.nCoatingCount; ++h) {	
					if (!(m_sResult.sCoatingEdge.vtrectCoatingROI[h].left > m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].rectROI.right || 
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].right < m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].rectROI.left || 
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].top > m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].rectROI.bottom || 
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].bottom < m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].rectROI.top))
					{
						nId_Coating = h;
						h = m_sResult.sCoatingEdge.nCoatingCount;
					}
				}
				if (nId_Coating == -1) continue;

				int nDirection = -1;
				if (m_sResult.sCoatingEdge.vtrectCoatingROI[nId_Coating].left > nCrnterX) {
					nDirection = 1;
				}
				else {
					nDirection = 2;
				}

				m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].nPositionCount = nCount;
				m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo.resize(nCount);
				for (int y = 0; y < nCount; ++y) {
					ptStart.x = m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].rectROI.left;
					ptStart.y = m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].rectROI.top + y;
					ptEnd.x = m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].rectROI.right;
					if (!jet_imagefunction::CalculatePointY(dSlope, ptStart, ptEnd)) {
						return false;
					}

					// L -> R
					vector<POINT> vtptLine;
					if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, ptStart, ptEnd, vtptLine) != 1) {
						return false;
					}

					int nCount_Line = vtptLine.size();
					if (nCount_Line <= 0) return false;
					switch (nDirection)
					{
					case 1: // L->R
						for (int p = 0; p < nCount_Line; ++p) {
							if (vtptLine[p].x < 0 || vtptLine[p].y < 0 || vtptLine[p].x >= m_matEdgeMask.cols || vtptLine[p].y >= m_matEdgeMask.rows) {
								return false;
							}
							uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(vtptLine[p].y);
							if (pu8Edge[vtptLine[p].x] == 255 && pu8Edge[vtptLine[p].x + 1] == 0) {
								m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptStart = vtptLine[p];
								//p = nCount_Line;
							}
						}
						for (int p = nCount_Line - 1; p >= 0; --p) {
							if (vtptLine[p].x < 0 || vtptLine[p].y < 0 || vtptLine[p].x >= m_matEdgeMask.cols || vtptLine[p].y >= m_matEdgeMask.rows) {
								return false;
							}
							uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(vtptLine[p].y);
							if (pu8Edge[vtptLine[p].x] == m_nCoatingValue && pu8Edge[vtptLine[p].x-1] == 0) {
								m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd = vtptLine[p];
							}
						}
						break;
					case 2: // R->L
						for (int p = nCount_Line - 1; p >= 0; --p) {
							if (vtptLine[p].x < 0 || vtptLine[p].y < 0 || vtptLine[p].x >= m_matEdgeMask.cols || vtptLine[p].y >= m_matEdgeMask.rows) {
								return false;
							}
							uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(vtptLine[p].y);
							if (pu8Edge[vtptLine[p].x] == 255 && pu8Edge[vtptLine[p].x-1] == 0) {
								m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptStart = vtptLine[p];
								//p = -1;
							}
						}
						for (int p = 0; p < nCount_Line; ++p) {
							if (vtptLine[p].x < 0 || vtptLine[p].y < 0 || vtptLine[p].x >= m_matEdgeMask.cols || vtptLine[p].y >= m_matEdgeMask.rows) {
								return false;
							}
							uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(vtptLine[p].y);
							if (pu8Edge[vtptLine[p].x] == m_nCoatingValue && pu8Edge[vtptLine[p].x+1] == 0) {
								m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd = vtptLine[p];
							}
						}
						break;
					default:
						return false;
					}

					// 兩點距離
					Calculate_Distance(m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y]);
				}

				// 計算多點平均
				if (!CalcMeanDist(m_sParam.sCoatingToGlue_Horizontal.nMeanCount, m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k])) {
					return false;
				}

				// 輸出最大 最小值 與 (最大-最小)
				if (!OutputMaxMinDistance(m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k])) {
					return false;
				}

				// 判斷最大值範圍
				m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sCoatingToGlue_Horizontal.vtsLimit[k].sMaxDist);
				m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].bResult_MaxDist = true;

				// 判斷最小值範圍
				m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sCoatingToGlue_Horizontal.vtsLimit[k].sMinDist);

				// 判斷(最大-最小)範圍
				m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sCoatingToGlue_Horizontal.vtsLimit[k].sDiff_MaxMin);
				m_sResult.sCoatingToGlue_Horizontal.vtsRoiInfo[k].bResult_Diff_MaxMin = true;
			}

			return true;
		}

		// 垂直方向Coating邊到膠邊的距離
		bool MeasurementBlackGlue::Measurement_CoatingToGlue_Vertical(const bool& bChange)
		{
			bool bResult = Measurement_CoatingToGlue_Vertical();
			if (bChange && bResult == false)
			{
				bResult = Calculate_RoiPosition_Change(m_sParam.sCoatingToGlue_Vertical.nCount, m_sParam.sCoatingToGlue_Vertical.vtrectROI, m_sResult.sCoatingToGlue_Vertical);
				if (bResult) {
					bResult = Measurement_CoatingToGlue_Vertical();
				}
			}

			if (m_bSaveImage && bResult)
			{
				Mat matDisplay = m_matGlue.clone();
				// 畫 Coating 輪廓
				if (!DisplayCoatingPosition(m_sResult.sCoatingEdge, m_scContours_Coating, matDisplay)) {
					return false;
				}

				if (!DisplayRoiResult(m_sResult.sCoatingToGlue_Vertical, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, "CoatingToGlue_Vertical", matDisplay, true, true, false, true)) {
					return false;
				}
				SaveImage(0, matDisplay, "CoatingToGlue_Vertical", m_nExtensionType);
			}
			return bResult;
		}
		bool MeasurementBlackGlue::Measurement_CoatingToGlue_Vertical()
		{
			if (!m_sResult.sCoatingToGlue_Vertical.bEnable) return true;
			int nCount_Vertical = m_sParam.sCoatingToGlue_Vertical.nCount;
			if (m_matEdgeMask.empty() || nCount_Vertical <= 0 || nCount_Vertical != m_sResult.sCoatingToGlue_Vertical.nRoiCount) {
				return false;
			}

			// 計算每一點的座標與距離
			double dSlope = 0.0;
			if (m_sResult.sBoardEdge.dSlope_Horizontal == 0.0) {
				dSlope = 999999.0;
			}
			else {
				dSlope = -1.0 / m_sResult.sBoardEdge.dSlope_Horizontal;
			}

			POINT ptStart, ptEnd;
			for (int k = 0; k < nCount_Vertical; ++k) {
				int nCount = m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].rectROI.right - m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].rectROI.left;
				if (nCount <= 0) {
					continue;
				}

				int nId_Coating = -1;
				int nCrnterY = m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].rectROI.top;
				for (int h = 0; h < m_sResult.sCoatingEdge.nCoatingCount; ++h) {
					if (!(m_sResult.sCoatingEdge.vtrectCoatingROI[h].left > m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].rectROI.right ||
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].right < m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].rectROI.left ||
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].top > m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].rectROI.bottom ||
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].bottom < m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].rectROI.top))
					{
						nId_Coating = h;
						h = m_sResult.sCoatingEdge.nCoatingCount;
					}
				}
				if (nId_Coating == -1) continue;

				int nDirection = -1;
				if (m_sResult.sCoatingEdge.vtrectCoatingROI[nId_Coating].top > nCrnterY) {
					nDirection = 1;
				}
				else {
					nDirection = 2;
				}

				m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].nPositionCount = nCount;
				m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo.resize(nCount);
				for (int x = 0; x < nCount; ++x) {
					ptStart.x = m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].rectROI.left + x;
					ptStart.y = m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].rectROI.top;
					ptEnd.y = m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].rectROI.bottom;
					if (!jet_imagefunction::CalculatePointX(dSlope, ptStart, ptEnd)) continue;

					// T -> B
					vector<POINT> vtptLine;
					if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, ptStart, ptEnd, vtptLine) != 1) {
						continue;
					}

					int nCount_Line = vtptLine.size();
					if (nCount_Line <= 0) continue;
					switch (nDirection)
					{
					case 1: // T
						for (int p = 0; p < nCount_Line; ++p) {
							if (vtptLine[p].x < 0 || vtptLine[p].y < 1 || vtptLine[p].x >= m_matEdgeMask.cols || vtptLine[p].y >= m_matEdgeMask.rows-1) {
								continue;
							}
							uchar* pu8Edge_U = m_matEdgeMask.ptr<uchar>(vtptLine[p].y);
							uchar* pu8Edge_D = m_matEdgeMask.ptr<uchar>(vtptLine[p].y + 1);
							if (pu8Edge_U[vtptLine[p].x] == 255 && pu8Edge_D[vtptLine[p].x] == 0) {
								m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptStart = vtptLine[p];
								//p = nCount_Line;
							}
						}
						for (int p = nCount_Line - 1; p >= 1; --p) {
							if (vtptLine[p].x < 0 || vtptLine[p].y < 1 || vtptLine[p].x >= m_matEdgeMask.cols || vtptLine[p].y >= m_matEdgeMask.rows-1) {
								continue;
							}
							uchar* pu8Edge_D = m_matEdgeMask.ptr<uchar>(vtptLine[p].y);
							uchar* pu8Edge_U = m_matEdgeMask.ptr<uchar>(vtptLine[p].y - 1);
							if (pu8Edge_D[vtptLine[p].x] == m_nCoatingValue && pu8Edge_U[vtptLine[p].x] == 0) {
								m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd = vtptLine[p];
								//p = -1;
							}
						}
						break;
					case 2: // B
						for (int p = nCount_Line - 1; p >= 0; --p) {
							if (vtptLine[p].x < 0 || vtptLine[p].y < 1 || vtptLine[p].x >= m_matEdgeMask.cols || vtptLine[p].y >= m_matEdgeMask.rows-1) {
								continue;
							}
							uchar* pu8Edge_D = m_matEdgeMask.ptr<uchar>(vtptLine[p].y);
							uchar* pu8Edge_U = m_matEdgeMask.ptr<uchar>(vtptLine[p].y - 1);
							if (pu8Edge_D[vtptLine[p].x] == 255 && pu8Edge_U[vtptLine[p].x] == 0) {
								m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptStart = vtptLine[p];
								//p = -1;
							}
						}
						for (int p = 0; p < nCount_Line; ++p) {
							if (vtptLine[p].x < 0 || vtptLine[p].y < 1 || vtptLine[p].x >= m_matEdgeMask.cols || vtptLine[p].y >= m_matEdgeMask.rows-1) {
								continue;
							}
							uchar* pu8Edge_U = m_matEdgeMask.ptr<uchar>(vtptLine[p].y);
							uchar* pu8Edge_D = m_matEdgeMask.ptr<uchar>(vtptLine[p].y + 1);
							if (pu8Edge_U[vtptLine[p].x] == m_nCoatingValue && pu8Edge_D[vtptLine[p].x] == 0) {
								m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd = vtptLine[p];
								//p = nCount_Line;
							}
						}
						break;
					default:
						return false;
					}

					// 兩點距離
					Calculate_Distance(m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo[x]);
				}

				// 計算多點平均
				if (!CalcMeanDist(m_sParam.sCoatingToGlue_Vertical.nMeanCount, m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k])) {
					return false;
				}

				// 輸出最大 最小值 與 (最大-最小)
				if (!OutputMaxMinDistance(m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k])) {
					continue;
				}

				// 判斷最大值範圍
				m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sCoatingToGlue_Vertical.vtsLimit[k].sMaxDist);
				m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].bResult_MaxDist = true;

				// 判斷最小值範圍
				m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sCoatingToGlue_Vertical.vtsLimit[k].sMinDist);

				// 判斷(最大-最小)範圍
				m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sCoatingToGlue_Vertical.vtsLimit[k].sDiff_MaxMin);
				m_sResult.sCoatingToGlue_Vertical.vtsRoiInfo[k].bResult_Diff_MaxMin = true;
			}
			return true;
		}

		// 水平方向Coating邊到板邊的距離
		bool MeasurementBlackGlue::Measurement_CoatingToBoard_Horizontal()
		{
			bool bResult = Calculate_RoiPosition_Type1(m_sParam.sCoatingToBoard_Horizontal.nCount, m_sParam.sCoatingToBoard_Horizontal.vtrectROI, m_sResult.sCoatingToBoard_Horizontal);
			if (bResult) {
				if (m_sParam.sBoard.bEnable_Slope) {
					bResult = Measurement_CoatingToBoard_Horizontal_Slope();
				}
				else {
					bResult = Measurement_CoatingToBoard_Horizontal_Defulat();
				}
			}

			if (m_bSaveImage && bResult)
			{
				Mat matDisplay = m_matCoating.clone();
				// 畫 Coating 輪廓
				if (!DisplayCoatingPosition(m_sResult.sCoatingEdge, m_scContours_Coating, matDisplay)) {
					return false;
				}

				if (!DisplayRoiResult(m_sResult.sCoatingToBoard_Horizontal, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, "CoatingToBoard_Horizontal", matDisplay, true, true, false, true)) {
					return false;
				}
				SaveImage(0, matDisplay, "CoatingToBoard_Horizontal", m_nExtensionType);
			}
			return bResult;
		}

		bool MeasurementBlackGlue::Measurement_CoatingToBoard_Horizontal_Slope()
		{
			if (!m_sResult.sCoatingToBoard_Horizontal.bEnable) return true;
			int nCount_Horizontal = m_sParam.sCoatingToBoard_Horizontal.nCount;
			if (m_matEdgeMask.empty() || nCount_Horizontal <= 0 || nCount_Horizontal != m_sResult.sCoatingToBoard_Horizontal.nRoiCount || m_sResult.sBoardEdge.dSlope_Vertical == 0.0) {
				return false;
			}

			// 計算每一點的座標與距離
			double dSlope = -1.0 / m_sResult.sBoardEdge.dSlope_Vertical;
			POINT ptStart, ptEnd;
			for (int k = 0; k < nCount_Horizontal; ++k) {
				int nCount = m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.bottom - m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.top;
				if (nCount <= 0) {
					continue;
				}

				int nId_Coating = -1;
				int nCrnterX = m_matEdgeMask.cols / 2;
				for (int h = 0; h < m_sResult.sCoatingEdge.nCoatingCount; ++h) {
					if (!(m_sResult.sCoatingEdge.vtrectCoatingROI[h].left > m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.right ||
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].right < m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.left ||
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].top > m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.bottom ||
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].bottom < m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.top))
					{
						nId_Coating = h;
						h = m_sResult.sCoatingEdge.nCoatingCount;
					}
				}
				if (nId_Coating == -1) continue;

				int nDirection = -1;
				if (m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.left < nCrnterX) {
					nDirection = 1;
				}
				else {
					nDirection = 2;
				}

				m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].nPositionCount = nCount;
				m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].vtsPositionInfo.resize(nCount);
				for (int y = 0; y < nCount; ++y) {
					ptStart.x = m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.left;
					ptStart.y = m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.top + y;
					ptEnd.x = m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.right;
					if (!jet_imagefunction::CalculatePointY(dSlope, ptStart, ptEnd)) {
						continue;
					}

					// L -> R
					vector<POINT> vtptLine;
					if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, ptStart, ptEnd, vtptLine) != 1) {
						continue;
					}

					int nCount_Line = vtptLine.size();
					if (nCount_Line <= 0) continue;

					// find first
					switch (nDirection)
					{
					case 1: // L
						for (int p = 0; p < nCount_Line; ++p) {
							if (vtptLine[p].x < 1 || vtptLine[p].y < 0 || vtptLine[p].x >= m_matEdgeMask.cols-1 || vtptLine[p].y >= m_matEdgeMask.rows) {
								continue;
							}
							uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(vtptLine[p].y);
							if (pu8Edge[vtptLine[p].x-1] == 0 && pu8Edge[vtptLine[p].x] == m_nCoatingValue) {
								m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptStart = vtptLine[p];
								//p = nCount_Line;
							}
						}
						break;
					case 2: // R
						for (int p = 0; p < nCount_Line; ++p) {
							if (vtptLine[p].x < 1 || vtptLine[p].y < 0 || vtptLine[p].x >= m_matEdgeMask.cols-1 || vtptLine[p].y >= m_matEdgeMask.rows) {
								continue;
							}
							uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(vtptLine[p].y);
							if (pu8Edge[vtptLine[p].x] == m_nCoatingValue && pu8Edge[vtptLine[p].x + 1] == 0) {
								m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptStart = vtptLine[p];
								//p = nCount_Line;
							}
						}
						break;
					default:
						return false;
					}

					// find second
					if (nDirection == 2 && m_sResult.sBoardEdge.dIntercept_Vertical2 != 0.0) {
						if (!jet_imagefunction::CalculateIntersection(m_sResult.sBoardEdge.dSlope_Vertical, m_sResult.sBoardEdge.dIntercept_Vertical2, m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptStart, m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd)) {
							continue;
						}
					}
					else {
						if (!jet_imagefunction::CalculateIntersection(m_sResult.sBoardEdge.dSlope_Vertical, m_sResult.sBoardEdge.dIntercept_Vertical, m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptStart, m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd)) {
							continue;
						}
					}

					// 兩點距離
					Calculate_Distance(m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y]);
				}

				// 計算多點平均
				if (!CalcMeanDist(m_sParam.sCoatingToBoard_Horizontal.nMeanCount, m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k])) {
					return false;
				}

				// 輸出最大 最小值 與 (最大-最小)
				if (!OutputMaxMinDistance(m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k])) {
					continue;
				}

				// 判斷最大值範圍
				m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sMaxDist);
				m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].bResult_MaxDist = true;

				// 判斷最小值範圍
				m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sMinDist);

				// 判斷(最大-最小)範圍
				m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sDiff_MaxMin);
				m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].bResult_Diff_MaxMin = true;
			}

			return true;
		}
		bool MeasurementBlackGlue::Measurement_CoatingToBoard_Horizontal_Defulat()
		{
			if (!m_sResult.sCoatingToBoard_Horizontal.bEnable) return true;
			int nCount_Horizontal = m_sParam.sCoatingToBoard_Horizontal.nCount;
			if (m_matEdgeMask.empty() || m_matBoardEdge.empty() || nCount_Horizontal <= 0 || nCount_Horizontal != m_sResult.sCoatingToBoard_Horizontal.nRoiCount) {
				return false;
			}

			POINT ptStart, ptEnd, ptCenter;
			for (int k = 0; k < nCount_Horizontal; ++k) {
				int nCount = m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.bottom - m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.top;
				if (nCount <= 0) continue;

				int nId_Coating = -1;
				int nCrnterX = m_matEdgeMask.cols / 2;
				for (int h = 0; h < m_sResult.sCoatingEdge.nCoatingCount; ++h) {
					if (!(m_sResult.sCoatingEdge.vtrectCoatingROI[h].left > m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.right ||
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].right < m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.left ||
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].top > m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.bottom ||
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].bottom < m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.top))
					{
						nId_Coating = h;
						h = m_sResult.sCoatingEdge.nCoatingCount;
					}
				}

				int nDirection = -1;
				if (m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.left < nCrnterX) {
					nDirection = 1;
				}
				else {
					nDirection = 2;
				}

				int nRoiWidth = 0;
				m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].nPositionCount = nCount;
				m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].vtsPositionInfo.resize(nCount);
				for (int y = 0; y < nCount; ++y) {
					ptStart.x = m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.left;
					ptStart.y = m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.top + y;
					ptEnd.x = m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.right;

					POINT ptTemp;
					ptTemp.y = ptStart.y;
					uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(ptTemp.y);
					uchar* pu8Board = m_matBoardEdge.ptr<uchar>(ptTemp.y);
					switch (nDirection)
					{
					case 1:
						// L
						nRoiWidth = m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.right;
						for (int p = 0; p < nRoiWidth; ++p) {
							ptTemp.x = p;
							if (pu8Board[ptTemp.x] == 255) {
								m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptStart = ptTemp;
								p = nRoiWidth;
							}
						}

						// R
						for (int p = m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.left; p < m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.right; ++p) {
							ptTemp.x = p;
							if (pu8Edge[ptTemp.x] == m_nCoatingValue) {
								m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd = ptTemp;
								p = m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.right;
							}
						}
						break;
					case 2:
						// R
						nRoiWidth = m_matBoardEdge.cols - m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.left;
						for (int p = 0; p < nRoiWidth; ++p) {
							ptTemp.x = m_matBoardEdge.cols - p;
							if (pu8Board[ptTemp.x] == 255) {
								m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptStart = ptTemp;
								p = nRoiWidth;
							}
						}

						// L
						for (int p = m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.right-1; p > m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].rectROI.left; --p) {
							ptTemp.x = p;
							if (pu8Edge[ptTemp.x] == m_nCoatingValue) {
								m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd = ptTemp;
								p = -1;
							}
						}
						break;
					default:
						return false;
					}

					// 兩點距離
					Calculate_Distance(m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].vtsPositionInfo[y]);
				}

				// 計算多點平均
				if (!CalcMeanDist(m_sParam.sCoatingToBoard_Horizontal.nMeanCount, m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k])) {
					return false;
				}

				// 輸出最大 最小值 與 (最大-最小)
				if (!OutputMaxMinDistance(m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k])) {
					continue;
				}

				// 判斷最大值範圍
				m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sMaxDist);
				m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].bResult_MaxDist = true;

				// 判斷最小值範圍
				m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sMinDist);

				// 判斷(最大-最小)範圍
				m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sDiff_MaxMin);
				m_sResult.sCoatingToBoard_Horizontal.vtsRoiInfo[k].bResult_Diff_MaxMin = true;
			}
			return true;
		}

		// 垂直方向Coating邊到板邊的距離
		bool MeasurementBlackGlue::Measurement_CoatingToBoard_Vertical()
		{
			bool bResult = Calculate_RoiPosition_Type1(m_sParam.sCoatingToBoard_Vertical.nCount, m_sParam.sCoatingToBoard_Vertical.vtrectROI, m_sResult.sCoatingToBoard_Vertical);
			if (bResult) {
				if (m_sParam.sBoard.bEnable_Slope) {
					bResult = Measurement_CoatingToBoard_Vertical_Slope();
				}
				else {
					bResult = Measurement_CoatingToBoard_Vertical_Defulat();
				}
			}

			if (m_bSaveImage && bResult)
			{
				Mat matDisplay = m_matCoating.clone();
				// 畫 Coating 輪廓
				if (!DisplayCoatingPosition(m_sResult.sCoatingEdge, m_scContours_Coating, matDisplay)) {
					return false;
				}

				if (!DisplayRoiResult(m_sResult.sCoatingToBoard_Vertical, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, "CoatingToBoard_Vertical", matDisplay, true, true, false, true)) {
					return false;
				}
				if (m_sParam.sBoard.bEnable_Slope) {
					SaveImage(0, matDisplay, "CoatingToBoard_Vertical_Slope", m_nExtensionType);
				}
				else {
					SaveImage(0, matDisplay, "CoatingToBoard_Vertical_Defulat", m_nExtensionType);
				}
			}
			return bResult;
		}

		bool MeasurementBlackGlue::Measurement_CoatingToBoard_Vertical_Slope()
		{
			if (!m_sResult.sCoatingToBoard_Vertical.bEnable) return true;
			int nCount_Vertical = m_sParam.sCoatingToBoard_Vertical.nCount;
			if (m_matEdgeMask.empty() || nCount_Vertical <= 0 || nCount_Vertical != m_sResult.sCoatingToBoard_Vertical.nRoiCount) {
				return false;
			}

			// 計算每一點的座標與距離
			double dSlope = 0.0;
			if (m_sResult.sBoardEdge.dSlope_Horizontal == 0.0) {
				dSlope = 999999.0;
			}
			else {
				dSlope = -1.0 / m_sResult.sBoardEdge.dSlope_Horizontal;
			}

			POINT ptStart, ptEnd;
			for (int k = 0; k < nCount_Vertical; ++k) {
				int nCount = m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.right - m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.left;
				if (nCount <= 0) {
					continue;
				}

				int nId_Coating = -1;
				int nCrnterY = m_matEdgeMask.rows / 2;
				for (int h = 0; h < m_sResult.sCoatingEdge.nCoatingCount; ++h) {
					if (!(m_sResult.sCoatingEdge.vtrectCoatingROI[h].left > m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.right ||
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].right < m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.left ||
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].top > m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.bottom ||
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].bottom < m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.top))
					{
						nId_Coating = h;
						h = m_sResult.sCoatingEdge.nCoatingCount;
					}
				}

				int nDirection = -1;
				if (m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.top < nCrnterY) {
					nDirection = 1;
				}
				else {
					nDirection = 2;
				}

				m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].nPositionCount = nCount;
				m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].vtsPositionInfo.resize(nCount);
				for (int x = 0; x < nCount; ++x) {
					ptStart.x = m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.left + x;
					ptStart.y = m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.top;
					ptEnd.y = m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.bottom;
					if (!jet_imagefunction::CalculatePointX(dSlope, ptStart, ptEnd)) {
						continue;
					}

					// T -> B
					vector<POINT> vtptLine;
					if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, ptStart, ptEnd, vtptLine) != 1) {
						continue;
					}

					int nCount_Line = vtptLine.size();
					if (nCount_Line <= 0) continue;

					// find first
					switch (nDirection)
					{
					case 1: // T
						for (int p = 0; p < nCount_Line; ++p) {
							if (vtptLine[p].x < 0 || vtptLine[p].y < 1 || vtptLine[p].x >= m_matEdgeMask.cols || vtptLine[p].y >= m_matEdgeMask.rows-1) {
								continue;
							}
							uchar* pu8Edge_U = m_matEdgeMask.ptr<uchar>(vtptLine[p].y-1);
							uchar* pu8Edge_D = m_matEdgeMask.ptr<uchar>(vtptLine[p].y);
							if (pu8Edge_U[vtptLine[p].x]==0 && pu8Edge_D[vtptLine[p].x] == m_nCoatingValue) {
								m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptStart = vtptLine[p];
								//p = nCount_Line;
							}
						}
						break;
					case 2: // B
						for (int p = nCount_Line - 1; p >= 0; --p) {
							if (vtptLine[p].x < 0 || vtptLine[p].y < 1 || vtptLine[p].x >= m_matEdgeMask.cols || vtptLine[p].y >= m_matEdgeMask.rows-1) {
								continue;
							}
							uchar* pu8Edge_U = m_matEdgeMask.ptr<uchar>(vtptLine[p].y);
							uchar* pu8Edge_D = m_matEdgeMask.ptr<uchar>(vtptLine[p].y + 1);

							if (pu8Edge_U[vtptLine[p].x] == m_nCoatingValue && pu8Edge_D[vtptLine[p].x] == 0) {
								m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptStart = vtptLine[p];
								//p = -1;
							}
						}
						break;
					default:
						return false;
					}

					// find second
					if (nDirection == 2 && m_sResult.sBoardEdge.dIntercept_Horizontal2 != 0.0) {
						if (!jet_imagefunction::CalculateIntersection(m_sResult.sBoardEdge.dSlope_Horizontal, m_sResult.sBoardEdge.dIntercept_Horizontal2, m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptStart, m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd)) {
							continue;
						}
					}
					else {
						if (!jet_imagefunction::CalculateIntersection(m_sResult.sBoardEdge.dSlope_Horizontal, m_sResult.sBoardEdge.dIntercept_Horizontal, m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptStart, m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd)) {
							continue;
						}
					}

					// 兩點距離
					Calculate_Distance(m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].vtsPositionInfo[x]);
				}

				// 計算多點平均
				if (!CalcMeanDist(m_sParam.sCoatingToBoard_Vertical.nMeanCount, m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k])) {
					return false;
				}

				// 輸出最大 最小值 與 (最大-最小)
				if (!OutputMaxMinDistance(m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k])) {
					continue;
				}

				// 判斷最大值範圍
				m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sCoatingToBoard_Vertical.vtsLimit[k].sMaxDist);
				m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].bResult_MaxDist = true;

				// 判斷最小值範圍
				m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sCoatingToBoard_Vertical.vtsLimit[k].sMinDist);

				// 判斷(最大-最小)範圍
				m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sCoatingToBoard_Vertical.vtsLimit[k].sDiff_MaxMin);
				m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].bResult_Diff_MaxMin = true;
			}
			return true;
		}

		bool MeasurementBlackGlue::Measurement_CoatingToBoard_Vertical_Defulat()
		{
			if (!m_sResult.sCoatingToBoard_Vertical.bEnable) return true;
			int nCount_Vertical = m_sParam.sCoatingToBoard_Vertical.nCount;
			if (m_matEdgeMask.empty() || m_matBoardEdge.empty() || nCount_Vertical <= 0 || nCount_Vertical != m_sResult.sCoatingToBoard_Vertical.nRoiCount) {
				return false;
			}

			POINT ptStart, ptEnd, ptCenter;
			for (int k = 0; k < nCount_Vertical; ++k) {
				int nCount = m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.right - m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.left;
				if (nCount <= 0) {
					continue;
				}

				int nId_Coating = -1;
				int nCrnterY = m_matEdgeMask.rows / 2;
				for (int h = 0; h < m_sResult.sCoatingEdge.nCoatingCount; ++h) {
					if (!(m_sResult.sCoatingEdge.vtrectCoatingROI[h].left > m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.right ||
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].right < m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.left ||
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].top > m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.bottom ||
						m_sResult.sCoatingEdge.vtrectCoatingROI[h].bottom < m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.top))
					{
						nId_Coating = h;
						h = m_sResult.sCoatingEdge.nCoatingCount;
					}
				}

				int nDirection = -1;
				if (m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.top < nCrnterY) {
					nDirection = 1;
				}
				else {
					nDirection = 2;
				}

				int nRoi_Height = 0;
				m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].nPositionCount = nCount;
				m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].vtsPositionInfo.resize(nCount);
				for (int x = 0; x < nCount; ++x) {
					ptStart.x = m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.left + x;
					ptStart.y = m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.top;
					ptEnd.y = m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.bottom;

					POINT ptTemp;
					ptTemp.x = ptStart.x;
					switch (nDirection)
					{
					case 1:
						// T
						nRoi_Height = m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.bottom;
						for (int p = 0; p < nRoi_Height; ++p) {
							ptTemp.y = p;
							uchar* pu8Board = m_matBoardEdge.ptr<uchar>(ptTemp.y);
							if (pu8Board[ptTemp.x] == 255) {
								m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptStart = ptTemp;
								p = nRoi_Height;
							}
						}

						// B
						for (int p = m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.top; p < m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.bottom; ++p) {
							ptTemp.y = p;
							uchar* pu8Board = m_matEdgeMask.ptr<uchar>(ptTemp.y);
							if (pu8Board[ptTemp.x] == m_nCoatingValue) {
								m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd = ptTemp;
								p = nRoi_Height;
							}
						}
						break;
					case 2:
						// B
						nRoi_Height = m_matBoardEdge.rows - m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.top;
						for (int p = 0; p < nRoi_Height; ++p) {
							ptTemp.y = m_matBoardEdge.rows - p - 1;
							uchar* pu8Board = m_matBoardEdge.ptr<uchar>(ptTemp.y);
							if (pu8Board[ptTemp.x] == 255) {
								m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptStart = ptTemp;
								p = nRoi_Height;
							}
						}

						// T
						for (int p = m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.bottom-1; p > m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].rectROI.top; --p) {
							ptTemp.y = p;
							uchar* pu8Board = m_matEdgeMask.ptr<uchar>(ptTemp.y);
							if (pu8Board[ptTemp.x] == m_nCoatingValue) {
								m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd = ptTemp;
								p = nRoi_Height;
							}
						}
						break;
					default:
						return false;
					}

					// 兩點距離
					Calculate_Distance(m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].vtsPositionInfo[x]);
				}

				// 計算多點平均
				if (!CalcMeanDist(m_sParam.sCoatingToBoard_Vertical.nMeanCount, m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k])) {
					return false;
				}

				// 輸出最大 最小值 與 (最大-最小)
				if (!OutputMaxMinDistance(m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k])) {
					continue;
				}

				// 判斷最大值範圍
				m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sCoatingToBoard_Vertical.vtsLimit[k].sMaxDist);
				m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].bResult_MaxDist = true;

				// 判斷最小值範圍
				m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sCoatingToBoard_Vertical.vtsLimit[k].sMinDist);

				// 判斷(最大-最小)範圍
				m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sCoatingToBoard_Vertical.vtsLimit[k].sDiff_MaxMin);
				m_sResult.sCoatingToBoard_Vertical.vtsRoiInfo[k].bResult_Diff_MaxMin = true;
			}
			return true;
		}

		// Thermal Glue---GlueWidth
		bool MeasurementBlackGlue::Measurement_Thermal_GlueWidth()
		{
			if (!m_sResult.sThermalGlue.bEnable) return true;

			int nCount = m_sResult.sThermalGlue.sGlueWidth.nRoiCount;
			if (nCount <= 0 || nCount != m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo.size()) return false;

			m_sResult.sThermalGlue.sGlueWidth.bEnable = true;
			if (m_matThermalGlueEdge.empty()) {
				m_matThermalGlueEdge = Mat(m_matThermal.size(), CV_8U, Scalar(0));
				for (int k = 0; k < nCount; ++k) {
					int nNumber = m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].nContoursCount;
					POINT* ptPoint = &m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].vtptContoursPos[0];
					for (int h = 0; h < nNumber; ++h) {
						uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(ptPoint[h].y);
						pu8Edge[ptPoint[h].x] = 255;
					}
				}
			}	
			SaveImage(0, m_matThermalGlueEdge, "Thermal_GlueWidth_Edge", m_nExtensionType);

			int nCount_Limit = m_sParam.sThermal.vtsLimit_Width.size();
			for (int k = 0; k < nCount; ++k) {
				int nL = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.left;
				int nR = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.right;
				int nT = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.top;
				int nB = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.bottom;

				switch (m_sParam.sThermal.nGlue_Direction) {
				case 1: // 水平
					{	
						int nSumPoint = nR - nL;
						if (nSumPoint <= 0) continue;

						m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].nPositionCount = nSumPoint;
						m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo.resize(nSumPoint);	
						for (int x = 0; x < nSumPoint; ++x) {
							for (int y = nT; y < nB; ++y) { // start
								uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(y);
								if (pu8Edge[x+nL] == 255) {
									m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[x].ptStart.x = x + nL;
									m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[x].ptStart.y = y;
									y = nB;
								}
							}

							for (int y = nB; y >= nT; --y) { // End
								uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(y);
								if (pu8Edge[x + nL] == 255) {
									m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd.x = x + nL;
									m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd.y = y;
									y = -1;
								}
							}
							// 兩點距離
							Calculate_Distance(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[x]);
						}
					}
					break;
				case 2: // 垂直
					{
						int nSumPoint = nB - nT;
						if (nSumPoint <= 0) continue;

						m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].nPositionCount = nSumPoint;
						m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo.resize(nSumPoint);
						for (int y = 0; y < nSumPoint; ++y) {
							for (int x = nL; x < nR; ++x) { // start
								uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(y + nT);
								if (pu8Edge[x] == 255) {
									m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[y].ptStart.x = x;
									m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[y].ptStart.y = y + nT;
									x = nR;
								}
							}

							for (int x = nR; x >= nL; --x) { // End
								uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(y + nT);
								if (pu8Edge[x] == 255) {
									m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd.x = x;
									m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd.y = y + nT;
									x = -1;
								}
							}
							// 兩點距離
							Calculate_Distance(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[y]);
						}

					}
					break;
				}
				
				if (k >= nCount_Limit) 
				{
					// 輸出最大 最小值 與 (最大-最小)
					if (!OutputMaxMinDistance(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k])) {
						continue;
					}

					// 判斷最大值範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sThermal.vtsLimit_Width[nCount_Limit-1].sMaxDist);

					// 判斷最小值範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sThermal.vtsLimit_Width[nCount_Limit-1].sMinDist);

					// 判斷(最大-最小)範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sThermal.vtsLimit_Width[nCount_Limit-1].sDiff_MaxMin);
				}
				else {
					// 判斷最大值範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sThermal.vtsLimit_Width[k].sMaxDist);

					// 判斷最小值範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sThermal.vtsLimit_Width[k].sMinDist);

					// 判斷(最大-最小)範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sThermal.vtsLimit_Width[k].sDiff_MaxMin);
				}
			}

			if (m_bSaveImage)
			{
				Mat matDisplay = m_matThermal.clone();
				if (!DisplayRoiResult(m_sResult.sThermalGlue.sGlueWidth, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, "Thermal_GlueWidth", matDisplay)) {
					return false;
				}
				SaveImage(0, matDisplay, "ThermalGlue_GlueWidth", m_nExtensionType);
			}

			return true;
		}
		bool MeasurementBlackGlue::Measurement_Thermal_GlueWidth_New()
		{
			if (!m_sResult.sThermalGlue.bEnable) return true;

			bool bResult = false;
			if (m_sParam.sThermal.bRebuildDie) {
				if (m_sParam.sThermal.nGlue_Direction == 1) {
					bResult = Measurement_Thermal_GlueWidth_Slope_V();
				}
				else {
					bResult = Measurement_Thermal_GlueWidth_Slope_H();
				}
			}
			else {
				bResult = Measurement_Thermal_GlueWidth_Defulat();
			}

			if (m_bSaveImage)
			{
				Mat matDisplay = m_matThermal.clone();
				if (!DisplayRoiResult(m_sResult.sThermalGlue.sGlueWidth, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, "Thermal_GlueWidth", matDisplay)) {
					return false;
				}
				if (m_sParam.sThermal.bRebuildDie) {
					SaveImage(0, matDisplay, "ThermalGlue_GlueWidth_Slope", m_nExtensionType);
				}
				else {
					SaveImage(0, matDisplay, "ThermalGlue_GlueWidth_Defulat", m_nExtensionType);
				}
			}
		}

		// 膠水平排列, 垂直量測
		bool MeasurementBlackGlue::Measurement_Thermal_GlueWidth_Slope_V()
		{
			int nCount = m_sResult.sThermalGlue.sGlueWidth.nRoiCount;
			if (nCount <= 0 || nCount != m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo.size() || m_vtdDieSlope.size() != 4 || m_vtdDieIntercept.size() != 4) return false;

			m_sResult.sThermalGlue.sGlueWidth.bEnable = true;
			if (m_matThermalGlueEdge.empty()) {
				m_matThermalGlueEdge = Mat(m_matThermal.size(), CV_8U, Scalar(0));
				for (int k = 0; k < nCount; ++k) {
					int nNumber = m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].nContoursCount;
					POINT* ptPoint = &m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].vtptContoursPos[0];
					for (int h = 0; h < nNumber; ++h) {
						uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(ptPoint[h].y);
						pu8Edge[ptPoint[h].x] = 255;
					}
				}
			}
			SaveImage(0, m_matThermalGlueEdge, "Thermal_GlueWidth_Edge", m_nExtensionType);

			int nCount_Limit = m_sParam.sThermal.vtsLimit_Width.size();
			for (int k = 0; k < nCount; ++k) {
				int nL = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.left;
				int nR = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.right;
				int nT = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.top;
				int nB = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.bottom;

				int nSumPoint = nR - nL;
				if (nSumPoint <= 0) continue;

				m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].nPositionCount = nSumPoint;
				m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo.resize(nSumPoint);

				POINT ptStart, ptEnd;
				for (int x = 0; x < nSumPoint; ++x) {
					ptStart.x = nL + x;
					ptStart.y = nT;
					ptEnd.y = nB;
					if (!jet_imagefunction::CalculatePointX(m_vtdDieSlope[2], ptStart, ptEnd)) {
						continue;
					}

					// T -> B
					vector<POINT> vtptLine;
					if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, ptStart, ptEnd, vtptLine) != 1) {
						continue;
					}

					int nCount_Point = vtptLine.size();
					if (nCount_Point <= 0) continue;
					for (int y = 0; y < nCount_Point; ++y) { // start
						uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(vtptLine[y].y);
						if (pu8Edge[vtptLine[y].x] == 255) {
							m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[x].ptStart = vtptLine[y];
							y = nCount_Point;
						}
					}

					for (int y = nCount_Point -1; y >= 0; --y) { // End
						uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(vtptLine[y].y);
						if (pu8Edge[vtptLine[y].x] == 255) {
							m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[x].ptStart = vtptLine[y];
							y = -1;
						}
					}
					// 兩點距離
					Calculate_Distance(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[x]);
				}

				if (k >= nCount_Limit) 
				{
					// 輸出最大 最小值 與 (最大-最小)
					if (!OutputMaxMinDistance(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k])) {
						continue;
					}

					// 判斷最大值範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sThermal.vtsLimit_Width[nCount_Limit - 1].sMaxDist);

					// 判斷最小值範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sThermal.vtsLimit_Width[nCount_Limit - 1].sMinDist);

					// 判斷(最大-最小)範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sThermal.vtsLimit_Width[nCount_Limit - 1].sDiff_MaxMin);
				}
				else {
					// 判斷最大值範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sThermal.vtsLimit_Width[k].sMaxDist);

					// 判斷最小值範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sThermal.vtsLimit_Width[k].sMinDist);

					// 判斷(最大-最小)範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sThermal.vtsLimit_Width[k].sDiff_MaxMin);
				}
			}

			return true;
		}

		// 膠垂直排列, 水平量測
		bool MeasurementBlackGlue::Measurement_Thermal_GlueWidth_Slope_H()
		{
			int nCount = m_sResult.sThermalGlue.sGlueWidth.nRoiCount;
			if (nCount <= 0 || nCount != m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo.size() || m_vtdDieSlope.size() != 4 || m_vtdDieIntercept.size() != 4) return false;

			m_sResult.sThermalGlue.sGlueWidth.bEnable = true;
			if (m_matThermalGlueEdge.empty()) {
				m_matThermalGlueEdge = Mat(m_matThermal.size(), CV_8U, Scalar(0));
				for (int k = 0; k < nCount; ++k) {
					int nNumber = m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].nContoursCount;
					POINT* ptPoint = &m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].vtptContoursPos[0];
					for (int h = 0; h < nNumber; ++h) {
						uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(ptPoint[h].y);
						pu8Edge[ptPoint[h].x] = 255;
					}
				}
			}
			SaveImage(0, m_matThermalGlueEdge, "Thermal_GlueWidth_Edge", m_nExtensionType);

			int nCount_Limit = m_sParam.sThermal.vtsLimit_Width.size();
			for (int k = 0; k < nCount; ++k) {
				int nL = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.left;
				int nR = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.right;
				int nT = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.top;
				int nB = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.bottom;

				int nSumPoint = nB - nT;
				if (nSumPoint <= 0) continue;

				m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].nPositionCount = nSumPoint;
				m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo.resize(nSumPoint);

				POINT ptStart, ptEnd;
				for (int y = 0; y < nSumPoint; ++y) {
					ptStart.x = nL;
					ptStart.y = nT + y;
					ptEnd.x = nR;
					if (!jet_imagefunction::CalculatePointY(m_vtdDieSlope[0], ptStart, ptEnd)) {
						continue;
					}

					// L -> R
					vector<POINT> vtptLine;
					if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, ptStart, ptEnd, vtptLine) != 1) {
						continue;
					}

					int nCount_Point = vtptLine.size();
					if (nCount_Point <= 0) continue;
					for (int x = 0; x < nCount_Point; ++x) { // start
						uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(vtptLine[x].y);
						if (pu8Edge[vtptLine[x].x] == 255) {
							m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[y].ptStart = vtptLine[x];
							x = nCount_Point;
						}
					}

					for (int x = nCount_Point - 1; x >= 0; --x) { // End
						uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(vtptLine[x].y);
						if (pu8Edge[vtptLine[x].x] == 255) {
							m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[y].ptStart = vtptLine[x];
							x = -1;
						}
					}
					// 兩點距離
					Calculate_Distance(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[y]);
				}

				if (k >= nCount_Limit) 
				{
					// 輸出最大 最小值 與 (最大-最小)
					if (!OutputMaxMinDistance(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k])) {
						continue;
					}

					// 判斷最大值範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sThermal.vtsLimit_Width[nCount_Limit - 1].sMaxDist);

					// 判斷最小值範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sThermal.vtsLimit_Width[nCount_Limit - 1].sMinDist);

					// 判斷(最大-最小)範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sThermal.vtsLimit_Width[nCount_Limit - 1].sDiff_MaxMin);
				}
				else {
					// 判斷最大值範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sThermal.vtsLimit_Width[k].sMaxDist);

					// 判斷最小值範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sThermal.vtsLimit_Width[k].sMinDist);

					// 判斷(最大-最小)範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sThermal.vtsLimit_Width[k].sDiff_MaxMin);
				}
			}

			return true;
		}
		bool MeasurementBlackGlue::Measurement_Thermal_GlueWidth_Defulat()
		{
			int nCount = m_sResult.sThermalGlue.sGlueWidth.nRoiCount;
			if (nCount <= 0 || nCount != m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo.size()) return false;

			m_sResult.sThermalGlue.sGlueWidth.bEnable = true;
			if (m_matThermalGlueEdge.empty()) {
				m_matThermalGlueEdge = Mat(m_matThermal.size(), CV_8U, Scalar(0));
				for (int k = 0; k < nCount; ++k) {
					int nNumber = m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].nContoursCount;
					POINT* ptPoint = &m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].vtptContoursPos[0];
					for (int h = 0; h < nNumber; ++h) {
						uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(ptPoint[h].y);
						pu8Edge[ptPoint[h].x] = 255;
					}
				}
			}
			SaveImage(0, m_matThermalGlueEdge, "Thermal_GlueWidth_Edge", m_nExtensionType);

			int nCount_Limit = m_sParam.sThermal.vtsLimit_Width.size();
			for (int k = 0; k < nCount; ++k) {
				int nL = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.left;
				int nR = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.right;
				int nT = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.top;
				int nB = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.bottom;

				switch (m_sParam.sThermal.nGlue_Direction) {
				case 1: // 水平
				{
					int nSumPoint = nR - nL;
					if (nSumPoint <= 0) continue;

					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].nPositionCount = nSumPoint;
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo.resize(nSumPoint);
					for (int x = 0; x < nSumPoint; ++x) {
						for (int y = nT; y < nB; ++y) { // start
							uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(y);
							if (pu8Edge[x + nL] == 255) {
								m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[x].ptStart.x = x + nL;
								m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[x].ptStart.y = y;
								y = nB;
							}
						}

						for (int y = nB; y >= nT; --y) { // End
							uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(y);
							if (pu8Edge[x + nL] == 255) {
								m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd.x = x + nL;
								m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd.y = y;
								y = -1;
							}
						}
						// 兩點距離
						Calculate_Distance(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[x]);
					}
				}
				break;
				case 2: // 垂直
				{
					int nSumPoint = nB - nT;
					if (nSumPoint <= 0) continue;

					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].nPositionCount = nSumPoint;
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo.resize(nSumPoint);
					for (int y = 0; y < nSumPoint; ++y) {
						for (int x = nL; x < nR; ++x) { // start
							uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(y + nT);
							if (pu8Edge[x] == 255) {
								m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[y].ptStart.x = x;
								m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[y].ptStart.y = y + nT;
								x = nR;
							}
						}

						for (int x = nR; x >= nL; --x) { // End
							uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(y + nT);
							if (pu8Edge[x] == 255) {
								m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd.x = x;
								m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd.y = y + nT;
								x = -1;
							}
						}
						// 兩點距離
						Calculate_Distance(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo[y]);
					}

				}
				break;
				}

				if (k >= nCount_Limit) 
				{
					// 輸出最大 最小值 與 (最大-最小)
					if (!OutputMaxMinDistance(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k])) {
						continue;
					}

					// 判斷最大值範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sThermal.vtsLimit_Width[nCount_Limit - 1].sMaxDist);

					// 判斷最小值範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sThermal.vtsLimit_Width[nCount_Limit - 1].sMinDist);

					// 判斷(最大-最小)範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sThermal.vtsLimit_Width[nCount_Limit - 1].sDiff_MaxMin);
				}
				else {
					// 判斷最大值範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sThermal.vtsLimit_Width[k].sMaxDist);

					// 判斷最小值範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sThermal.vtsLimit_Width[k].sMinDist);

					// 判斷(最大-最小)範圍
					m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sThermal.vtsLimit_Width[k].sDiff_MaxMin);
				}
			}

			if (m_bSaveImage)
			{
				Mat matDisplay = m_matThermal.clone();
				if (!DisplayRoiResult(m_sResult.sThermalGlue.sGlueWidth, m_scROI_BoardToGlue, m_scDistance_BoardToGlue, m_scMaxDist_BoardToGlue, m_scMinDist_BoardToGlue, "Thermal_GlueWidth", matDisplay)) {
					return false;
				}
				SaveImage(0, matDisplay, "ThermalGlue_GlueWidth", m_nExtensionType);
			}

			return true;
		}

		// Thermal Glue---GlueToGlue
		bool MeasurementBlackGlue::Measurement_Thermal_GlueToGlue()
		{
			if (!m_sResult.sThermalGlue.bEnable) return true;

			int nCount = m_sResult.sThermalGlue.sGlueToGlue.nRoiCount;
			if (nCount <= 0 || nCount != m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo.size()) return false;

			m_sResult.sThermalGlue.sGlueToGlue.bEnable = true;
			if (m_matThermalGlueEdge.empty()) {
				m_matThermalGlueEdge = Mat(m_matThermal.size(), CV_8U, Scalar(0));
				for (int k = 0; k < nCount; ++k) {
					int nNumber = m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].nContoursCount;
					POINT* ptPoint = &m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].vtptContoursPos[0];
					for (int h = 0; h < nNumber; ++h) {
						uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(ptPoint[h].y);
						pu8Edge[ptPoint[h].x] = 255;
					}
				}
				SaveImage(0, m_matThermalGlueEdge, "Thermal_GlueWidth_Edge", m_nExtensionType);
			}
		
			int nCount_Limit = m_sParam.sThermal.vtsLimit_GlueToGlue.size();
			for (int k = 0; k < nCount; ++k) {
				int nL = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.left;
				int nR = m_sResult.sThermalGlue.sGlueWidth.vtsRoiInfo[k].rectROI.right;
				int nT = m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.top;
				int nB = m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].rectROI.bottom;

				switch (m_sParam.sThermal.nGlue_Direction) {
				case 1: // 水平
				{
					int nSumPoint = nR - nL;
					if (nSumPoint <= 0) continue;

					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].nPositionCount = nSumPoint;
					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].vtsPositionInfo.resize(nSumPoint);
					for (int x = 0; x < nSumPoint; ++x) {
						for (int y = nT; y < nB; ++y) { // start
							uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(y);
							if (pu8Edge[x + nL] == 255) {
								m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].vtsPositionInfo[x].ptStart.x = x + nL;
								m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].vtsPositionInfo[x].ptStart.y = y;
								y = nB;
							}
						}

						for (int y = nB; y >= nT; --y) { // End
							uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(y);
							if (pu8Edge[x + nL] == 255) {
								m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd.x = x + nL;
								m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd.y = y;
								y = -1;
							}
						}
						// 兩點距離
						Calculate_Distance(m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].vtsPositionInfo[x]);
					}
				}
				break;
				case 2: // 垂直
				{
					int nSumPoint = nB - nT;
					if (nSumPoint <= 0) continue;

					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].nPositionCount = nSumPoint;
					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].vtsPositionInfo.resize(nSumPoint);
					for (int y = 0; y < nSumPoint; ++y) {
						for (int x = nL; x < nR; ++x) { // start
							uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(y + nT);
							if (pu8Edge[x] == 255) {
								m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].vtsPositionInfo[y].ptStart.x = x;
								m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].vtsPositionInfo[y].ptStart.y = y + nT;
								x = nR;
							}
						}

						for (int x = nR; x >= nL; --x) { // End
							uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(y + nT);
							if (pu8Edge[x] == 255) {
								m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd.x = x;
								m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd.y = y + nT;
								x = -1;
							}
						}
						// 兩點距離
						Calculate_Distance(m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].vtsPositionInfo[y]);
					}

				}
				break;
				}

				if (k >= nCount_Limit) 
				{
					// 輸出最大 最小值 與 (最大-最小)
					if (!OutputMaxMinDistance(m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k])) {
						continue;
					}

					// 判斷最大值範圍
					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sThermal.vtsLimit_GlueToGlue[nCount_Limit - 1].sMaxDist);

					// 判斷最小值範圍
					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sThermal.vtsLimit_GlueToGlue[nCount_Limit - 1].sMinDist);

					// 判斷(最大-最小)範圍
					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sThermal.vtsLimit_GlueToGlue[nCount_Limit - 1].sDiff_MaxMin);
				}
				else {
					// 判斷最大值範圍
					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sThermal.vtsLimit_GlueToGlue[k].sMaxDist);

					// 判斷最小值範圍
					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sThermal.vtsLimit_GlueToGlue[k].sMinDist);

					// 判斷(最大-最小)範圍
					m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sThermal.vtsLimit_GlueToGlue[k].sDiff_MaxMin);
				}
			}

			if (m_bSaveImage)
			{
				Mat matDisplay = m_matThermal.clone();
				if (!DisplayRoiResult(m_sResult.sThermalGlue.sGlueToGlue, m_scROI_GlueWidth, m_scDistance_GlueWidth, m_scMaxDist_GlueWidth, m_scMinDist_GlueWidth, "Thermal_GlueToGlue", matDisplay)) {
					return false;
				}
				SaveImage(0, matDisplay, "ThermalGlue_GlueToGlue", m_nExtensionType);
			}

			return true;
		}

		// Thermal Glue---GlueToDie
		bool MeasurementBlackGlue::Measurement_Thermal_GlueToDie()
		{
			if (!m_sResult.sThermalGlue.bEnable) return true;

			int nCount = m_sResult.sThermalGlue.sGlueToDie.nRoiCount;
			if (nCount != 2) return false;

			m_sResult.sThermalGlue.sGlueToDie.bEnable = true;
			if (m_matThermalGlueEdge.empty()) {
				m_matThermalGlueEdge = Mat(m_matThermal.size(), CV_8U, Scalar(0));
				for (int k = 0; k < nCount; ++k) {
					int nNumber = m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].nContoursCount;
					POINT* ptPoint = &m_sResult.sThermalGlue.vtsGlueEdgeInfo[k].vtptContoursPos[0];
					for (int h = 0; h < nNumber; ++h) {
						uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(ptPoint[h].y);
						pu8Edge[ptPoint[h].x] = 255;
					}
				}
				SaveImage(0, m_matThermalGlueEdge, "Thermal_GlueWidth_Edge", m_nExtensionType);
			}

			Point ptLT, ptRT, ptRB, ptLB;
			if (m_sParam.sThermal.bRebuildDie) {
				if (m_sResult.sThermalGlue.vtptDieCorner.size() != 4) return false;
				ptLT = Point(m_sResult.sThermalGlue.vtptDieCorner[0].x, m_sResult.sThermalGlue.vtptDieCorner[0].y);
				ptRT = Point(m_sResult.sThermalGlue.vtptDieCorner[1].x, m_sResult.sThermalGlue.vtptDieCorner[1].y);
				ptRB = Point(m_sResult.sThermalGlue.vtptDieCorner[2].x, m_sResult.sThermalGlue.vtptDieCorner[2].y);
				ptLB = Point(m_sResult.sThermalGlue.vtptDieCorner[3].x, m_sResult.sThermalGlue.vtptDieCorner[3].y);
			}
			else {
				ptLT = Point(m_sResult.sThermalGlue.rectDie.left, m_sResult.sThermalGlue.rectDie.top);
				ptRT = Point(m_sResult.sThermalGlue.rectDie.right, m_sResult.sThermalGlue.rectDie.top);
				ptLB = Point(m_sResult.sThermalGlue.rectDie.left, m_sResult.sThermalGlue.rectDie.bottom);
				ptRB = Point(m_sResult.sThermalGlue.rectDie.right, m_sResult.sThermalGlue.rectDie.bottom);
			}

			// T
			cv::line(m_matThermalGlueEdge, ptLT, ptRT, Scalar(255), 1);
			// B
			cv::line(m_matThermalGlueEdge, ptLB, ptRB, Scalar(255), 1);
			// L
			cv::line(m_matThermalGlueEdge, ptLT, ptLB, Scalar(255), 1);
			// R
			cv::line(m_matThermalGlueEdge, ptRT, ptRB, Scalar(255), 1);
			SaveImage(0, m_matThermalGlueEdge, "Thermal_GlueToDie_Edge", m_nExtensionType);

			int nCount_Limit = m_sParam.sThermal.vtsLimit_GlueToDie.size();
			for (int k = 0; k < nCount; ++k) {
				int nL = m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].rectROI.left;
				int nR = m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].rectROI.right;
				int nT = m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].rectROI.top;
				int nB = m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].rectROI.bottom;

				switch (m_sParam.sThermal.nGlue_Direction) {
				case 1: // 水平
				{
					int nSumPoint = nR - nL;
					if (nSumPoint <= 0) continue;

					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].nPositionCount = nSumPoint;
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].vtsPositionInfo.resize(nSumPoint);
					for (int x = 0; x < nSumPoint; ++x) {
						for (int y = nT; y < nB; ++y) { // start
							uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(y);
							if (pu8Edge[x + nL] == 255) {
								m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].vtsPositionInfo[x].ptStart.x = x + nL;
								m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].vtsPositionInfo[x].ptStart.y = y;
								y = nB;
							}
						}

						for (int y = nB; y >= nT; --y) { // End
							uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(y);
							if (pu8Edge[x + nL] == 255) {
								m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd.x = x + nL;
								m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].vtsPositionInfo[x].ptEnd.y = y;
								y = -1;
							}
						}
						// 兩點距離
						Calculate_Distance(m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].vtsPositionInfo[x]);
					}
				}
				break;
				case 2: // 垂直
				{
					int nSumPoint = nB - nT;
					if (nSumPoint <= 0) continue;

					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].nPositionCount = nSumPoint;
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].vtsPositionInfo.resize(nSumPoint);
					for (int y = 0; y < nSumPoint; ++y) {
						for (int x = nL; x < nR; ++x) { // start
							uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(y + nT);
							if (pu8Edge[x] == 255) {
								m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].vtsPositionInfo[y].ptStart.x = x;
								m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].vtsPositionInfo[y].ptStart.y = y + nT;
								x = nR;
							}
						}

						for (int x = nR; x >= nL; --x) { // End
							uchar* pu8Edge = m_matThermalGlueEdge.ptr<uchar>(y + nT);
							if (pu8Edge[x] == 255) {
								m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd.x = x;
								m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].vtsPositionInfo[y].ptEnd.y = y + nT;
								x = -1;
							}
						}
						// 兩點距離
						Calculate_Distance(m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].vtsPositionInfo[y]);
					}

				}
				break;
				}

				if (k >= nCount_Limit) 
				{
					// 輸出最大 最小值 與 (最大-最小)
					if (!OutputMaxMinDistance(m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].vtsPositionInfo, m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k])) {
						continue;
					}

					// 判斷最大值範圍
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sThermal.vtsLimit_GlueToGlue[nCount_Limit - 1].sMaxDist);

					// 判斷最小值範圍
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sThermal.vtsLimit_GlueToGlue[nCount_Limit - 1].sMinDist);

					// 判斷(最大-最小)範圍
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sThermal.vtsLimit_GlueToGlue[nCount_Limit - 1].sDiff_MaxMin);
				}
				else {
					// 判斷最大值範圍
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].sMaxDist.fDistance, m_sParam.sThermal.vtsLimit_GlueToGlue[k].sMaxDist);

					// 判斷最小值範圍
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].bResult_MinDist = JudgmentResultRange(m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].sMinDist.fDistance, m_sParam.sThermal.vtsLimit_GlueToGlue[k].sMinDist);

					// 判斷(最大-最小)範圍
					m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].bResult_Diff_MaxMin = JudgmentResultRange(m_sResult.sThermalGlue.sGlueToDie.vtsRoiInfo[k].fDiff_Dist_MaxMin, m_sParam.sThermal.vtsLimit_GlueToGlue[k].sDiff_MaxMin);
				}
			}

			if (m_bSaveImage)
			{
				Mat matDisplay = m_matThermal.clone();
				if (!DisplayRoiResult(m_sResult.sThermalGlue.sGlueToDie, m_scROI_GlueArc, m_scDistance_GlueArc, m_scMaxDist_GlueArc, m_scMinDist_GlueArc, "Thermal_GlueToDie", matDisplay)) {
					return false;
				}
				SaveImage(0, matDisplay, "ThermalGlue_GlueToDie", m_nExtensionType);
			}

			return true;
		}

		// 計算多點平均（固定視窗大小 nMeanCount；邊界時向另一側補齊）
		bool MeasurementBlackGlue::CalcMeanDist(const int nMeanCount, SSingleROI_Result& sResult)
		{
			if (nMeanCount <= 1) return true;
			const int nCount = sResult.nPositionCount;
			if (nCount <= 0) return false;
			if (nCount != static_cast<int>(sResult.vtsPositionInfo.size())) return false;
			if (nMeanCount < 1 || nMeanCount >= nCount) return false;

			const bool bEven = ((nMeanCount & 1) == 0);

			// 以你的定義：奇數 nMeanCount=2nRadius+1；偶數 nMeanCount=2r（視窗是 [k-nRadius, k+nRadius-1]）
			const int nRadius = nMeanCount / 2;

			// 三段分界（避免每個 k 都做分支與補齊）
			// 左段 k in [0, kLeftEnd] 使用視窗 [0, nMeanCount-1]
			// 中段 k in [kMidStart, kMidEnd] 視窗滑動
			// 右段 k in [kRightStart, nCount-1] 使用視窗 [nCount-nMeanCount, nCount-1]
			const int nLeftStart = 0;
			const int nLeftEnd = nRadius - 1;       // 若 nRadius==0 不會進左段（但 nMeanCount>1 時 nRadius>=1）
			const int nMidStart = nRadius;
			const int nMidEnd = bEven ? (nCount - nRadius) : (nCount - nRadius - 1);
			const int nRightStart = nMidEnd + 1;
			const int nRightEnd = nCount;

			// 讀原始資料：避免邊算邊改造成遞迴平滑
			std::vector<float> vtfDist(nCount);
			for (int i = 0; i < nCount; ++i) vtfDist[i] = sResult.vtsPositionInfo[i].fDistance;

			const double dInvM = 1.0 / static_cast<double>(nMeanCount);

			// 初始化左段/起始視窗 [0, nMeanCount-1]
			double dSum = 0.0;
			for (int i = 0; i < nMeanCount; ++i) dSum += static_cast<double>(vtfDist[i]);

			// 左段：視窗固定 [0, nMeanCount-1]
			for (int k = nLeftStart; k <= nLeftEnd && k < nCount; ++k) {
				sResult.vtsPositionInfo[k].fDistance = static_cast<float>(dSum * dInvM);
			}

			// 中段：滑動視窗
			int nStart = 0;
			int nEnd = nMeanCount - 1;
			for (int k = nMidStart; k <= nMidEnd; ++k) {
				// nMidStart 時 dSum 已是對應視窗，先寫結果
				sResult.vtsPositionInfo[k].fDistance = static_cast<float>(dSum * dInvM);

				// 下一步：視窗往右滑一格
				if (k < nMidEnd) {
					dSum -= static_cast<double>(vtfDist[nStart]);
					++nStart;
					++nEnd;
					dSum += static_cast<double>(vtfDist[nEnd]);
				}
			}

			// 右段：視窗固定 [nCount-nMeanCount, nCount-1]
			if (nRightStart < nRightEnd) {
				for (int k = nRightStart; k < nRightEnd; ++k) {
					sResult.vtsPositionInfo[k].fDistance = static_cast<float>(dSum * dInvM);
				}
			}

			return true;
		}


		// 計算首尾的多點平均距離
		bool MeasurementBlackGlue::CalcMeanDist_StartEnd(const int nMeanCount, const Mat& matGlueEdge, SSingleStartEndROI_Result& sResult)
		{
			if (nMeanCount < 1) return false;
			if (nMeanCount == 1) return true;

			int nRadius = static_cast<int>(((float)nMeanCount) / 2.0 + 0.5);
			if (nRadius <= 0) nRadius = 1;

			auto CalcMeanPos = [&](const POINT& ptPos, const RECT& rectROI, double& dMeanPosX, double& dMeanPosY)->bool {
				dMeanPosX = 0.0; dMeanPosY = 0.0;

				int nStartY = ptPos.y - nRadius;
				if (nStartY < rectROI.top) nStartY = rectROI.top;

				int nEndY = ptPos.y + nRadius;
				if (nEndY > rectROI.bottom) nEndY = rectROI.bottom;

				int nStartX = ptPos.x - nRadius;
				if (nStartX < rectROI.left) nStartX = rectROI.left;

				int nEndX = ptPos.x + nRadius;
				if (nEndX > rectROI.right) nEndX = rectROI.right;

				int nCount = 0;
				double dSumPosX = 0.0, dSumPosY = 0.0;
				for (int y = nStartY; y <= nEndY; ++y) {
					const uchar* pu8Row = matGlueEdge.ptr<uchar>(y);
					const uchar* pu8Edge = pu8Row + nStartX;
					for (int x = nStartX; x <= nEndX; ++x, ++pu8Edge) {
						if (*pu8Edge == 255) {
							dSumPosX += x;
							dSumPosY += y;
							++nCount;
						}
					}
				}

				if (nCount <= 0) return false;

				dMeanPosX = dSumPosX / nCount;
				dMeanPosY = dSumPosY / nCount;
				return true;
			};

			// Start ROI
			double dMeanPosX_Start, dMeanPosY_Start;
			if (!CalcMeanPos(sResult.sDist.ptStart, sResult.rectStart, dMeanPosX_Start, dMeanPosY_Start)) {
				return false;
			}

			// End ROI
			double dMeanPosX_End, dMeanPosY_End;
			if (!CalcMeanPos(sResult.sDist.ptEnd, sResult.rectEnd, dMeanPosX_End, dMeanPosY_End)) {
				return false;
			}

			double dDiffX = (dMeanPosX_Start - dMeanPosX_End)*m_sParam.fResolutionX;
			double dDiffY = (dMeanPosY_Start - dMeanPosY_End)*m_sParam.fResolutionY;
			sResult.sDist.fDistance = static_cast<float>(sqrt(pow(dDiffX, 2.0) + pow(dDiffY, 2.0)));

			return true;
		}

		// 量測膠首尾距離
		bool MeasurementBlackGlue::Measurement_StartEnd(const bool& bChange)
		{
			if (m_matEdgeMask.empty()) {
				return false;
			}

			int nCount = m_sResult.sStartEnd.nCount;
			if (nCount <= 0 || nCount != m_sResult.sStartEnd.vtsStartEnd.size()) return false;

			bool bResult = false;
			for (int k = 0; k < nCount; ++k) {
				// StartROI
				bResult = GetLimitPoint(m_matEdgeMask, m_sResult.sStartEnd.vtsStartEnd[k].rectStart, m_sParam.sGlueStartEnd.vtsStartEnd[k].nStartROI_ArcLocation, m_sResult.sStartEnd.vtsStartEnd[k].sDist.ptStart);
				if (bChange && bResult == false) {
					bResult = Calculate_StartROI_Change(k);
					if (bResult) {
						bResult = GetLimitPoint(m_matEdgeMask, m_sResult.sStartEnd.vtsStartEnd[k].rectStart, m_sParam.sGlueStartEnd.vtsStartEnd[k].nStartROI_ArcLocation, m_sResult.sStartEnd.vtsStartEnd[k].sDist.ptStart);
					}
				}

				if (bResult)
				{
					// EndROI
					bResult = GetLimitPoint(m_matEdgeMask, m_sResult.sStartEnd.vtsStartEnd[k].rectEnd, m_sParam.sGlueStartEnd.vtsStartEnd[k].nEndROI_ArcLocation, m_sResult.sStartEnd.vtsStartEnd[k].sDist.ptEnd);
					if (bChange && bResult == false) {
						bResult = Calculate_EndROI_Change(k);
						if (bResult) {
							bResult = GetLimitPoint(m_matEdgeMask, m_sResult.sStartEnd.vtsStartEnd[k].rectEnd, m_sParam.sGlueStartEnd.vtsStartEnd[k].nEndROI_ArcLocation, m_sResult.sStartEnd.vtsStartEnd[k].sDist.ptEnd);
						}
					}

					// 兩點距離
					if (bResult) {
						if (m_sParam.sGlueStartEnd.nMeanCount == 1) {
							Calculate_Distance(m_sResult.sStartEnd.vtsStartEnd[k].sDist);
						}
						else {
							// 計算出端點後再計算平均
							if (!CalcMeanDist_StartEnd(m_sParam.sGlueStartEnd.nMeanCount, m_matEdgeMask, m_sResult.sStartEnd.vtsStartEnd[k])) {
								return false;
							}
						}
						
						// 判斷最大值範圍
						m_sResult.sStartEnd.vtsStartEnd[k].bResult_MaxDist = JudgmentResultRange(m_sResult.sStartEnd.vtsStartEnd[k].sDist.fDistance, m_sParam.sGlueStartEnd.vtsStartEnd[k].sLimit);
					}
				}
			}

			if (m_bSaveImage && bResult)
			{
				Mat matDisplay = m_matGlue.clone();
				if (!DisplayStartEndResult(m_sResult.sStartEnd, m_scROI_GlueArc, m_scDistance_GlueArc, m_scMaxDist_GlueArc, "GlueWidth_StartEnd", matDisplay)) {
					return false;
				}
				SaveImage(0, matDisplay, "StartEnd", m_nExtensionType);
			}

			return bResult;
		}
#pragma endregion

#pragma region 輸出

		// 將輪廓點位移
		bool MeasurementBlackGlue::ContoursShift(const int& nShiftX, const int& nShiftY, SContoursParam* psContours)
		{
			if (psContours == nullptr) return false;
			int nCount_Contours = psContours->GetCount();
			int nCount = psContours->GetCount_On();
			if (nCount_Contours == 0 || nCount == 0) return true;

			int nSumPoint = 0;
			for (int k = 0; k < nCount_Contours; ++k) {
				bool* pbOn = psContours->GetOnPtr(k);
				if (pbOn == nullptr)	return false;

				if (*pbOn) {
					SJRect* psjROI = psContours->GetSJRectPtr(k);
					if (psjROI == nullptr) return false;
					psjROI->nX += nShiftX;
					psjROI->nY += nShiftY;

					vector<vector<POINT>>* pvt2ptLine = psContours->GetLinePtr(k);
					if (pvt2ptLine == nullptr) return false;

					int nCount_Line = pvt2ptLine->size();
					for (int h = 0; h < nCount_Line; ++h) {
						int nCount_Point = (*pvt2ptLine)[h].size();
						nSumPoint += nCount_Point;
						if (nCount_Point > 0) {
							POINT* pPt = &(*pvt2ptLine)[h][0];
							for (int p = 0; p < nCount_Point; ++p) {
								pPt[p].x += nShiftX;
								pPt[p].y += nShiftY;
							}
						}
					}
				}
			}
			if (nSumPoint <= 0) return false;

			return true;
		}

		// 將輪廓點從 psContours 轉成 vtptContoursPos
		bool MeasurementBlackGlue::ContoursConvertVector(SContoursParam* psContours, vector<POINT>& vtptContoursPos)
		{
			if (psContours == nullptr) return false;
			int nCount = psContours->GetCount_On();
			if (nCount == 0) return true;

			int nSumPoint = 0;
			int nCount_Contours = psContours->GetCount();
			for (int k = 0; k < nCount_Contours; ++k) {
				bool* pbOn = psContours->GetOnPtr(k);
				if (pbOn == nullptr)	return false;

				if (*pbOn) {
					vector<vector<POINT>>* pvt2ptLine = psContours->GetLinePtr(k);
					if (pvt2ptLine == nullptr) return false;

					int nCount_Line = pvt2ptLine->size();
					for (int h = 0; h < nCount_Line; ++h) {
						nSumPoint += (*pvt2ptLine)[h].size();
					}
				}
			}
			if (nSumPoint <= 0) return false;

			vtptContoursPos.resize(nSumPoint);
			int nId = 0;
			for (int k = 0; k < nCount_Contours; ++k)
			{
				bool* pbOn = psContours->GetOnPtr(k);
				if (pbOn == nullptr)	return false;

				if (*pbOn) {
					vector<vector<POINT>>* pvt2ptLine = psContours->GetLinePtr(k);
					if (pvt2ptLine == nullptr) return false;

					int nCount_Line = pvt2ptLine->size();
					for (int h = 0; h < nCount_Line; ++h) {
						int nCount_Point = (*pvt2ptLine)[h].size();
						if (nCount_Point > 0) {
							POINT* pPt = &(*pvt2ptLine)[h][0];
							for (int p = 0; p < nCount_Point; ++p) {
								vtptContoursPos[nId] = pPt[p];
								++nId;
								if (nId == nSumPoint) {
									p = nCount_Point;
									h = nCount_Line;
									k = nCount_Contours;
								}
							}
						}
					}
				}
			}

			return true;
		}

		bool MeasurementBlackGlue::ContoursConvertVector(SContoursParam* psContours, vector<POINT>& vtptContoursPos1, vector<POINT>& vtptContoursPos2)
		{
			if (psContours == nullptr) return false;
			int nCount = psContours->GetCount_On();
			if (nCount == 0) return true;

			int nNumber = 0;
			int nCount_Contours = psContours->GetCount();
			for (int k = 0; k < nCount_Contours; ++k) {
				bool* pbOn = psContours->GetOnPtr(k);
				if (pbOn == nullptr)	return false;

				if (*pbOn) {
					vector<vector<POINT>>* pvt2ptLine = psContours->GetLinePtr(k);
					if (pvt2ptLine == nullptr) return false;

					int nSumPoint = 0;
					int nCount_Line = pvt2ptLine->size();
					for (int h = 0; h < nCount_Line; ++h) {
						nSumPoint += (*pvt2ptLine)[h].size();
					}

					if (nSumPoint <= 0) return false;

					if (nNumber == 0) {
						vtptContoursPos1.resize(nSumPoint);
						int nId = 0;
						for (int h = 0; h < nCount_Line; ++h) {
							int nCount_Point = (*pvt2ptLine)[h].size();
							if (nCount_Point > 0) {
								POINT* pPt = &(*pvt2ptLine)[h][0];
								for (int p = 0; p < nCount_Point; ++p) {
									vtptContoursPos1[nId] = pPt[p];
									++nId;
								}
							}
						}
						++nNumber;
					}
					else if (nNumber == 1) {
						vtptContoursPos2.resize(nSumPoint);
						int nId = 0;
						for (int h = 0; h < nCount_Line; ++h) {
							int nCount_Point = (*pvt2ptLine)[h].size();
							if (nCount_Point > 0) {
								POINT* pPt = &(*pvt2ptLine)[h][0];
								for (int p = 0; p < nCount_Point; ++p) {
									vtptContoursPos2[nId] = pPt[p];
									++nId;
									if (nId == nSumPoint) {
										p = nCount_Point;
										h = nCount_Line;
										k = nCount_Contours;
									}
								}
							}
						}
						++nNumber;
					}
				}
			}

			return true;
		}

		// 將輪廓點從 psContours 轉成 vtptContoursPos
		bool MeasurementBlackGlue::ContoursConvertMultipleVector(SContoursParam* psContours, vector<SJRect*>& vtpjRect, vector<vector<POINT>>& vt2ptContoursPos)
		{
			if (psContours == nullptr) return false;
			int nCount_Contours = psContours->GetCount();
			int nCount = psContours->GetCount_On();
			if(nCount_Contours == 0 || nCount == 0) return true;
			
			vt2ptContoursPos.resize(nCount);
			vtpjRect.resize(nCount);
			vector<int> vtnSumPoint(nCount, 0);

			int nId = 0;	
			for (int k = 0; k < nCount_Contours; ++k) {
				bool* pbOn = psContours->GetOnPtr(k);
				if (pbOn == nullptr)	return false;

				if (*pbOn) {
					vector<vector<POINT>>* pvt2ptLine = psContours->GetLinePtr(k);
					if (pvt2ptLine == nullptr) return false;

					int nCount_Line = pvt2ptLine->size();
					for (int h = 0; h < nCount_Line; ++h) {
						vtnSumPoint[nId] += (*pvt2ptLine)[h].size();
					}
					++nId;
					if (nId >= nCount) {
						k = nCount_Contours;
					}
				}
			}

			nId = 0;
			for (int k = 0; k < nCount_Contours; ++k)
			{
				bool* pbOn = psContours->GetOnPtr(k);
				if (pbOn == nullptr)	return false;

				if (*pbOn) {
					SJRect* pjRect = psContours->GetSJRectPtr(k);
					vector<vector<POINT>>* pvt2ptLine = psContours->GetLinePtr(k);
					if (pvt2ptLine == nullptr || pjRect == nullptr) return false;

					vtpjRect[nId] = pjRect;
					vt2ptContoursPos[nId].resize(vtnSumPoint[nId]);
					int nNumber = 0;
					int nCount_Line = pvt2ptLine->size();
					for (int h = 0; h < nCount_Line; ++h) {
						int nCount_Point = (*pvt2ptLine)[h].size();
						if (nCount_Point > 0) {
							POINT* pPt = &(*pvt2ptLine)[h][0];
							for (int p = 0; p < nCount_Point; ++p) {
								vt2ptContoursPos[nId][nNumber] = pPt[p];
								++nNumber;
								if (nNumber == vtnSumPoint[nId]) {
									p = nCount_Point;
									h = nCount_Line;
								}
							}
						}
					}
					++nId;
					if (nId >= nCount) {
						k = nCount_Contours;
					}
				}
			}

			return true;
		}

		// 輸出距離
		// nMode: 1=> 輸出最大值, 2=> 輸出最小值, 3=>平均值
		// nIndex : 暫無作用
		bool MeasurementBlackGlue::OutputDistance(const int& nMode, const int& nSortIndex, const vector<SMeasurementResult>& vtsMeasurementInfo, int& nId)
		{
			nId = -1;
			int nCount = vtsMeasurementInfo.size();
			if (nMode < 1 || nMode > 3 || (nMode == 3 && (nSortIndex < 1 || nSortIndex > 100)) || nCount <= 0) {
				return false;
			}

			float fValue = 0.0;
			switch (nMode)
			{
			case 1:// 最大值
				fValue = 0.0;
				for (int k = 0; k < nCount; ++k) {
					if (vtsMeasurementInfo[k].ptStart.x == 0.0 && vtsMeasurementInfo[k].ptStart.y == 0.0 ||
						vtsMeasurementInfo[k].ptEnd.x == 0.0 && vtsMeasurementInfo[k].ptEnd.y == 0.0) continue;
					if (fValue < vtsMeasurementInfo[k].fDistance) {
						fValue = vtsMeasurementInfo[k].fDistance;
						nId = k;
					}
				}
				break;

			case 2:// 最小值
				fValue = 99999.0;
				for (int k = 0; k < nCount; ++k) {
					if (vtsMeasurementInfo[k].ptStart.x == 0.0 && vtsMeasurementInfo[k].ptStart.y == 0.0 ||
						vtsMeasurementInfo[k].ptEnd.x == 0.0 && vtsMeasurementInfo[k].ptEnd.y == 0.0) continue;
					if (fValue > vtsMeasurementInfo[k].fDistance) {
						fValue = vtsMeasurementInfo[k].fDistance;
						nId = k;
					}
				}
				break;

			case 3:// 平均值
				{
					float fSum = 0.0;
					int nValidCount = 0;
					for (int k = 0; k < nCount; ++k) {
						if ((vtsMeasurementInfo[k].ptStart.x == 0.0 && vtsMeasurementInfo[k].ptStart.y == 0.0) ||
							(vtsMeasurementInfo[k].ptEnd.x == 0.0 && vtsMeasurementInfo[k].ptEnd.y == 0.0)) continue;
						fSum += vtsMeasurementInfo[k].fDistance;
						++nValidCount;
					}
					if (nValidCount == 0) {
						return false;
					}
					fValue = fSum / nValidCount;
					nId = -2; // 特別標記，因為平均值沒有對應到特定一個元素
				}
				break;
			}

			if (nId == -1) {
				return false;
			}

			return true;
		}

		// 輸出最大 最小距離
		bool MeasurementBlackGlue::OutputMaxMinDistance(const vector<SMeasurementResult>& vtsMeasurementInfo, SMeasurementResult& sMaxDist, SMeasurementResult& sMinDist)
		{
			int nCount = vtsMeasurementInfo.size();
			if (nCount <= 0) return false;

			// 輸出最大值
			int nMaxId = -1;
			if (!OutputDistance(1, 0, vtsMeasurementInfo, nMaxId)) {
				return false;
			}

			if (nMaxId < 0 || nMaxId >= nCount) {
				return false;
			}
			sMaxDist = vtsMeasurementInfo[nMaxId];

			// 輸出最小值
			int nMinId = -1;
			if (!OutputDistance(2, 0, vtsMeasurementInfo, nMinId)) {
				return false;
			}

			if (nMinId < 0 || nMinId >= nCount) {
				return false;
			}
			sMinDist = vtsMeasurementInfo[nMinId];

			return true;
		}

		// 輸出最大 最小距離 與 最大-最小
		bool MeasurementBlackGlue::OutputMaxMinDistance(const vector<SMeasurementResult>& vtsMeasurementInfo, SSingleROI_Result& sResult)
		{
			int nCount = vtsMeasurementInfo.size();
			if (nCount <= 0) return false;

			// 輸出最大值
			int nMaxId = -1;
			if (!OutputDistance(1, 0, vtsMeasurementInfo, nMaxId)) {
				return false;
			}

			if (nMaxId < 0 || nMaxId >= nCount) {
				return false;
			}
			sResult.sMaxDist = vtsMeasurementInfo[nMaxId];

			// 輸出最小值
			int nMinId = -1;
			if (!OutputDistance(2, 0, vtsMeasurementInfo, nMinId)) {
				return false;
			}

			if (nMinId < 0 || nMinId >= nCount) {
				return false;
			}
			sResult.sMinDist = vtsMeasurementInfo[nMinId];

			// 輸出最大距離-最小距離
			sResult.fDiff_Dist_MaxMin = sResult.sMaxDist.fDistance - sResult.sMinDist.fDistance;

			return true;
		}

		// 判斷量測結果的範圍
		bool MeasurementBlackGlue::JudgmentResultRange(const float& fDistance, SLimit_Single& sLimit)
		{
			return (sLimit.fLower < fDistance && fDistance < sLimit.fUpper);
		}

		// 計算2點距離
		void MeasurementBlackGlue::Calculate_Distance(SMeasurementResult& sDist)
		{
			sDist.fDistance = 0.0;
			if (sDist.ptStart.x == sDist.ptEnd.x && sDist.ptStart.y == sDist.ptEnd.y)
			{
				sDist.fDistance = 1.0;
			}
			else
			{
				double dX1 = sDist.ptStart.x;
				double dY1 = sDist.ptStart.y;
				double dX2 = sDist.ptEnd.x;
				double dY2 = sDist.ptEnd.y;
				double dDiffX = (dX1 - dX2)*m_sParam.fResolutionX;
				double dDiffY = (dY1 - dY2)*m_sParam.fResolutionY;
				sDist.fDistance = sqrt(pow(dDiffX, 2) + pow(dDiffY, 2));
			}
		}
#pragma endregion

#pragma region Flux Area

		// 找Flux面積邊界
		bool MeasurementBlackGlue::RunProcess_FindFluxArea()
		{
			if (!SetFindEdgeProcess(m_sParam_FluxArea.sFluxEdge, m_sPM_FindGlue)) {
				return false;
			}

			vector<Mat> vtmatImage(1);
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;

			switch (m_sParam_FluxArea.sFluxEdge.nColorTransformMode) {
			case 6:
				jet_imagefunction::LogTransform_STD(m_matGlue, vtmatImage[0]);
				break;
			case 7:
				jet_imagefunction::BrightnessUniformityCorrection_CLAHE(m_matGlue, 16, 16, 3.0, vtmatImage[0]);
				break;
			default:
				vtmatImage[0] = m_matGlue;
				break;
			}

			SaveImage(0, vtmatImage[0], "Pre", m_nExtensionType);

			if (!jet_imagefunction::RunProcessMode(vtmatImage, m_sPM_FindGlue, vtsResult)) {
				if (!vtsResult[1].matImage.empty()) {
					m_matBoard = vtsResult[1].matImage.clone();
				}
				int nEndStep = 0;
				if (m_sParam_FluxArea.sFluxEdge.nColorTransformMode == 1) {
					nEndStep = 3 + (m_sParam_FluxArea.sFluxEdge.nCount_ColorExtraction - 1) * 2 - 1;
				}
				else {
					nEndStep = 3 + (m_sParam_FluxArea.sFluxEdge.nCount_GrayExtraction - 1) * 2 - 1;
				}

				// 抽色結果為空, IC上面沒有膠
				if (nEndStep >= 2 && nEndStep < vtsResult.size() && !vtsResult[nEndStep].matImage.empty()) {
					int nWhiteCount = countNonZero(vtsResult[nEndStep].matImage);
					if (nWhiteCount == 0) {
						m_sResult_FluxArea.sFluxEdge.nContoursCount = 0;
						m_matEdgeMask = Mat(m_matGlue.size(), CV_8U, Scalar(0));
						return true;
					}
					else {
						if (m_bSaveImage && m_nSaveLevel > 0) {
							jet_imagefunction::SaveProcessResult(m_strSavePathName, "Error_FindFluxArea", m_sPM_FindGlue, vtmatImage, vtsResult, 2);
						}
						return false;
					}
				}
				else {
					if (m_bSaveImage && m_nSaveLevel > 0) {
						jet_imagefunction::SaveProcessResult(m_strSavePathName, "Error_FindFluxArea", m_sPM_FindGlue, vtmatImage, vtsResult, 2);
					}
					return false;
				}
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName, "OK_FindFluxArea", m_sPM_FindGlue, vtmatImage, vtsResult, 2);
			}

			int nNumber = m_sPM_FindGlue.GetBaseParameterNumber();
			if (nNumber != vtsResult.size()) {
				m_strErrorMessage = "RunProcess_FindFluxArea is error";
				return false;
			}

			m_matBoard = vtsResult[1].matImage.clone();
			SContoursParam* psContours = &vtsResult[nNumber - 1].sContours;
			if (psContours == nullptr) {
				m_strErrorMessage = "RunProcess_FindFluxArea is error";
				return false;
			}

			// 輸出輪廓點
			if (!ContoursConvertVector(psContours, m_sResult_FluxArea.sFluxEdge.vtptContoursPos)) {
				m_strErrorMessage = "RunProcess_FindFluxArea is error";
				return false;
			}
			m_sResult_FluxArea.sFluxEdge.nContoursCount = m_sResult_FluxArea.sFluxEdge.vtptContoursPos.size();

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				jet_imagefunction::DrawEdgePoint(matDisplay, m_sResult_FluxArea.sFluxEdge.vtptContoursPos, m_scContours_Glue, 1);
				SaveImage(0, matDisplay, "GlueContours", m_nExtensionType);
			}

			// 輪廓填滿
			if (!jet_imagefunction::FillContours_All(*psContours, m_matEdgeMask)) {
				m_strErrorMessage = "FillContours_All is error";
				return false;
			}

			m_sResult_FluxArea.sFluxEdge.nPixelArea = countNonZero(m_matEdgeMask);

			SaveImage(0, m_matEdgeMask, "FluxArea", m_nExtensionType);

			return true;
		}

		// 輸出Flux面積比例
		bool MeasurementBlackGlue::OutputFluxArea()
		{
			if (m_matEdgeMask.empty() || m_matEdgeMask.channels() != 1 || m_sResult_FluxArea.sFluxEdge.nPixelArea < 0) {
				return false;
			}

			m_sResult_FluxArea.fFluxArea = m_sResult_FluxArea.sFluxEdge.nPixelArea*m_sParam_GlueArea.fResolutionX*m_sParam_GlueArea.fResolutionY;
			m_sResult_FluxArea.fAreaRatio = (m_sResult_FluxArea.fFluxArea / m_sParam_FluxArea.fTotalArea) * 100.0f;

			m_sResult_FluxArea.bResult = false;
			if (m_sParam_FluxArea.sLimit.fLower <= m_sResult_FluxArea.fAreaRatio && m_sResult_FluxArea.fAreaRatio <= m_sParam_FluxArea.sLimit.fUpper) {
				m_sResult_FluxArea.bResult = true;
			}

			return true;
		}

#pragma endregion

#pragma region 畫圖

		// 顯示膠的輪廓
		// nMode : 選擇哪一張影像當底圖 ; 1=>膠影像 ; 2=>板邊影像 ; 3=>Coating影像 ; 4=>matDisplay
		// nLineWidth : 線寬, 0=>1 pixels, 1=>3 pixels ...
		// matDisplay : 輸入時可為空, 輸出時為彩色格式
		bool MeasurementBlackGlue::DisplayGlueContours(const int nMode, const int& nLineWidth, Mat& matDisplay)
		{
			if (!m_sResult.sGlueEdge.bEnable) return true;

			int nCount = m_sResult.sGlueEdge.nContoursCount;
			if (nCount <= 0) return false;

			if (!AssignImage(nMode, matDisplay)) {
				return false;
			}

			int nR = nLineWidth;
			if (nR < 0) nR = 0;
			jet_imagefunction::DrawEdgePoint(matDisplay, m_sResult.sGlueEdge.vtptContoursPos, m_scContours_Glue, nR);

			if (m_sResult.sGlueEdge2.nContoursCount > 0) {
				jet_imagefunction::DrawEdgePoint(matDisplay, m_sResult.sGlueEdge2.vtptContoursPos, m_scContours_Glue, nR);
			}

			return true;
		}

		// 顯示板邊的輪廓
		// nMode : 選擇哪一張影像當底圖 ; 1=>膠影像 ; 2=>板邊影像 ; 3=>Coating影像 ; 4=>matDisplay
		// matDisplay : 輸入時可為空, 輸出時為彩色格式 
		bool MeasurementBlackGlue::DisplayBoardContours(const int nMode, const int& nLineWidth, Mat& matDisplay)
		{
			if (!m_sResult.sBoardEdge.bEnable) return true;

			int nCount = m_sResult.sBoardEdge.nContoursCount;
			if (nCount <= 0) return false;

			if (!AssignImage(nMode, matDisplay)) {
				return false;
			}

			int nR = nLineWidth;
			if (nR < 0) nR = 0;
			jet_imagefunction::DrawEdgePoint(matDisplay, m_sResult.sBoardEdge.vtptContoursPos, m_scContours_Board, nR);

			return true;
		}

		// 顯示Coating的輪廓
		// nMode : 選擇哪一張影像當底圖 ; 1=>膠影像 ; 2=>板邊影像 ; 3=>Coating影像 ; 4=>matDisplay
		// matDisplay : 輸入時可為空, 輸出時為彩色格式
		bool MeasurementBlackGlue::DisplayCoatingContours(const int nMode, const int& nLineWidth, Mat& matDisplay)
		{
			if (!m_sResult.sCoatingEdge.bEnable) return true;

			int nCount = m_sResult.sCoatingEdge.nCoatingCount;
			if (nCount <= 0) return false;

			if (!AssignImage(nMode, matDisplay)) {
				return false;
			}

			int nR = nLineWidth;
			if (nR < 0) nR = 0;
			for (int k = 0; k < nCount; ++k) {
				jet_imagefunction::DrawEdgePoint(matDisplay, m_sResult.sCoatingEdge.vt2ptContoursPos[k], m_scContours_Board, nR);
			}	

			return true;
		}

		// nMode : 選擇哪一張影像當底圖 ; 1=>膠影像 ; 2=>板邊影像 ; 3=>Coating影像 ; 4=>matDisplay
		bool MeasurementBlackGlue::AssignImage(const int nMode, Mat& matShow)
		{
			if (nMode < 1 || nMode > 4) return false;

			Mat matDisplay;
			if (matShow.empty())
			{
				switch (nMode)
				{
				case 1:
					matDisplay = m_matGlue.clone();
					break;
				case 2:
					matDisplay = m_matBoard.clone();
					break;
				case 3:
					matDisplay = m_matCoating.clone();
					break;
				case 4:
					matDisplay = matShow.clone();
					break;
				}
			}
			else
			{
				if (matShow.size() != m_matGlue.size())
				{
					switch (nMode)
					{
					case 1:
						matDisplay = m_matGlue.clone();
						break;
					case 2:
						matDisplay = m_matBoard.clone();
						break;
					case 3:
						matDisplay = m_matCoating.clone();
						break;
					case 4:
						matDisplay = matShow.clone();
						break;
					}
				}
				else
				{
					if (matShow.channels() == 1)
					{
						cvtColor(matShow, matDisplay, COLOR_GRAY2BGR);
					}
					else if (matShow.channels() == 3)
					{
						switch (nMode)
						{
						case 1:
							matDisplay = m_matGlue.clone();
							break;
						case 2:
							matDisplay = m_matBoard.clone();
							break;
						case 3:
							matDisplay = m_matCoating.clone();
							break;
						case 4:
							matDisplay = matShow.clone();
							break;
						}
					}
					else
					{
						return false;
					}
				}
			}

			matShow = matDisplay.clone();

			return true;
		}

		// 畫出膠邊或板邊
		// nMode : 1=> 膠邊; 2=>板邊; 3=>Coating
		// matEdge = 輸出的影像(灰階影像, 邊為255, 背景為0)
		bool MeasurementBlackGlue::DrawEdge(const int& nLocation, const int& nMode, Mat& matEdge)
		{
			POINT* pPoint = nullptr;
			RECT rectROI;
			int nCoatingCount = 0;
			int nCount = 0;
			uchar u8Value = 255;

			switch (nMode) {
			case 1: // 膠邊
				if (m_sResult.sGlueEdge.bEnable) {
					nCount = m_sResult.sGlueEdge.nContoursCount;
					pPoint = &m_sResult.sGlueEdge.vtptContoursPos[0];
					rectROI = m_sResult.sGlueEdge.rectROI;
				}
				break;
			case 2: // 板邊
				if (m_sResult.sBoardEdge.bEnable) {
					nCount = m_sResult.sBoardEdge.nContoursCount;
					pPoint = &m_sResult.sBoardEdge.vtptContoursPos[0];
				}
				break;
			case 3: // Coating
				if (m_sResult.sCoatingEdge.bEnable) {
					nCoatingCount = m_sResult.sCoatingEdge.nCoatingCount;
					if (nCoatingCount > 0) {
						nCount = m_sResult.sCoatingEdge.vtnContoursCount[0];
						pPoint = &m_sResult.sCoatingEdge.vt2ptContoursPos[0][0];
						rectROI = m_sResult.sCoatingEdge.vtrectCoatingROI[0];
						u8Value = m_nCoatingValue;
					}
				}
				break;
			default:
				return false;
			}

			if (matEdge.empty()) {
				matEdge = Mat(m_matGlue.size(), CV_8U, Scalar(0));
			}
			else {
				if (matEdge.channels() != 1 && matEdge.size() != m_matGlue.size()) {
					matEdge.release();
					matEdge = Mat(m_matGlue.size(), CV_8U, Scalar(0));
				}
			}

			for (int k = 0; k < nCount; ++k) {
				uchar* pu8Edge = matEdge.ptr<uchar>(pPoint[k].y);
				pu8Edge[pPoint[k].x] = u8Value;
			}

			if (nCoatingCount > 1) {
				for (int k = 1; k < nCoatingCount; ++k) {
					nCount = m_sResult.sCoatingEdge.vtnContoursCount[k];
					pPoint = &m_sResult.sCoatingEdge.vt2ptContoursPos[k][0];
					//rectROI = m_sResult.sCoatingEdge.vtrectCoatingROI[k];
					for (int h = 0; h < nCount; ++h) {
						uchar* pu8Edge = matEdge.ptr<uchar>(pPoint[k].y);
						pu8Edge[pPoint[k].x] = u8Value;
					}
				}
			}

			return true;
		}

		// nMode : 1=> 膠邊; 3=>Coating
		bool MeasurementBlackGlue::DrawMask(const int& nMode, const int& nValue, Mat& matMask)
		{
			if (matMask.empty()) {
				matMask = Mat(m_matGlue.size(), CV_8U, Scalar(0));
			}
			else {
				if (matMask.channels() != 1 && matMask.size() != m_matGlue.size()) {
					matMask.release();
					matMask = Mat(m_matGlue.size(), CV_8U, Scalar(0));
				}
			}

			RECT rectROI;
			int nCoatingCount = 0;
			int nCount = 0;
			POINT* pptContoursPos = nullptr;
			switch (nMode) {
			case 1: // 膠邊
				if (m_sResult.sGlueEdge.bEnable) {
					rectROI = m_sResult.sGlueEdge.rectROI;
					nCount = m_sResult.sGlueEdge.nContoursCount;
					pptContoursPos = &m_sResult.sGlueEdge.vtptContoursPos[0];
				}
				break;
			case 2: // 膠邊2
				if (m_sResult.sGlueEdge.bEnable) {
					rectROI = m_sResult.sGlueEdge2.rectROI;
					nCount = m_sResult.sGlueEdge2.nContoursCount;
					pptContoursPos = &m_sResult.sGlueEdge2.vtptContoursPos[0];
				}
				break;
			case 3: // Coating
				if (m_sResult.sCoatingEdge.bEnable) {
					nCoatingCount = m_sResult.sCoatingEdge.nCoatingCount;
					if (nCoatingCount > 0) {
						nCount = m_sResult.sCoatingEdge.vtnContoursCount[0];
						rectROI = m_sResult.sCoatingEdge.vtrectCoatingROI[0];
						pptContoursPos = &m_sResult.sCoatingEdge.vt2ptContoursPos[0][0];
					}
				}
				break;
			default:
				return false;
			}

			Mat matTemp(m_matGlue.size(), CV_8U, Scalar(0));
			if (!Convex_Fill(pptContoursPos, nCount, nValue, matTemp) || matTemp.empty()) {
				return false;
			}

			for (int y = rectROI.top; y < rectROI.bottom; ++y) {
				if (y >= m_matGlue.rows) continue;
				uchar* pu8Temp = matTemp.ptr<uchar>(y);
				uchar* pu8Mask = matMask.ptr<uchar>(y);
				for (int x = rectROI.left; x < rectROI.right; ++x) {
					if (x >= m_matGlue.cols) continue;
					if (pu8Temp[x] == nValue) {
						pu8Mask[x] = nValue;
					}
				}
			}

			if (nCoatingCount > 1) {
				for (int k = 1; k < nCoatingCount; ++k) {
					Mat matTemp2(m_matGlue.size(), CV_8U, Scalar(0));
					nCount = m_sResult.sCoatingEdge.vtnContoursCount[k];
					rectROI = m_sResult.sCoatingEdge.vtrectCoatingROI[k];
					pptContoursPos = &m_sResult.sCoatingEdge.vt2ptContoursPos[k][0];
					if (!Convex_Fill(pptContoursPos, nCount, nValue, matTemp) || matTemp.empty()) {
						return false;
					}

					for (int y = rectROI.top; y < rectROI.bottom; ++y) {
						if (y >= m_matGlue.rows) continue;
						uchar* pu8Temp = matTemp.ptr<uchar>(y);
						uchar* pu8Mask = matMask.ptr<uchar>(y);
						for (int x = rectROI.left; x < rectROI.right; ++x) {
							if (x >= m_matGlue.cols) continue;
							if (pu8Temp[x] == nValue) {
								pu8Mask[x] = nValue;
							}
						}
					}
				}
			}

			return true;
		}

		// 將邊界內的範圍填滿
		bool MeasurementBlackGlue::Convex_Fill(const POINT* pContoursPos, const int& nCount, const int& nValue, Mat& matDisplay)
		{
			if (pContoursPos == nullptr || nCount == 0 || nValue<0 || nValue>255 || matDisplay.empty() || matDisplay.channels() != 1) {
				return false;
			}

			vector<vector<Point>> vt2cvptHull(1, vector<Point>(nCount));
			for (int p = 0; p < nCount; ++p) {
				vt2cvptHull[0][p].x = pContoursPos[p].x;
				vt2cvptHull[0][p].y = pContoursPos[p].y;
			}

			cv::fillPoly(matDisplay, vt2cvptHull, Scalar(nValue));

			return true;
		}
#pragma endregion

#pragma region 2D GlueHeight

		bool MeasurementBlackGlue::SetFindEdgeProcess(const SFindEdge_Parameter& sParam, SProcessModeParam& sPM)
		{
			int nQueueId = 1;
			sPM.Clear();

			// 1
			JET::mod::SColorTransformParam sColorTransform;
			sColorTransform.SetInput1(1, 1);
			switch (sParam.nColorTransformMode)
			{
			case 1:// 彩色
				sColorTransform.eMode = GRAY_TO_BGR;
				break;
			case 2:// R
			case 3:// G
			case 4:// B
				sColorTransform.eMode = COLOR_TO_GRAY_BGR;
				sColorTransform.nID = 4 - sParam.nColorTransformMode;
				break;
			case 5:
				sColorTransform.eMode = COLOR_TO_GRAY_AVERAGE;
				break;
			case 6: // Log_STD
			case 7: // CLAHE
			case 8:	// Sigmoid
				sColorTransform.eMode = COLOR_TO_GRAY_KMEANPCA;
				sColorTransform.sPca.nType_Eigenvectors = 1;
				sColorTransform.sKmean.nGroupMode = 1;
				sColorTransform.sKmean.nGroupNumber = 3;
				sColorTransform.sKmean.sOutputParams.nMinId = 0;
				sColorTransform.sKmean.sOutputParams.nMaxId = 2;
				sColorTransform.nID = 5;
				break;
			}
			sColorTransform.SetQueueId(nQueueId);
			sPM.Set(sColorTransform);
			++nQueueId;

			// 2
			int nMedianSize = (sParam.nMedianSize > 0) ? sParam.nMedianSize : 1;
			JET::mod::SFilterParam sFilter_Median;
			sFilter_Median.SetInput1(0, -1);
			switch (sParam.nFilterMode) {
			case 1:
				sFilter_Median.eMode = FILTER_MEDIAN;
				break;
			case 2:
				sFilter_Median.eMode = FILTER_AVERAGE;
				break;
			case 3:
				sFilter_Median.eMode = FILTER_MAX;
				sFilter_Median.eElement = ELEMENT_RECT;
				break;
			case 4:
				sFilter_Median.eMode = FILTER_MIN;
				sFilter_Median.eElement = ELEMENT_RECT;
				break;
			case 5:
				sFilter_Median.eMode = FILTER_MAXMIN;
				sFilter_Median.eElement = ELEMENT_RECT;
				break;
			case 6:
				sFilter_Median.eMode = FILTER_MINMAX;
				sFilter_Median.eElement = ELEMENT_RECT;
				break;
			}	
			sFilter_Median.nSizeX = nMedianSize;
			sFilter_Median.nSizeY = nMedianSize;
			sFilter_Median.SetQueueId(nQueueId);
			sPM.Set(sFilter_Median);
			++nQueueId;

			// 3
#if 1
			if (sParam.nColorTransformMode == 1 || sParam.nColorTransformMode == 6) {
				int nCount_Color = 0;
				if (sParam.nColorThresholdMode == 1) {
					nCount_Color = sParam.nCount_ColorExtraction;
				}
				else if (sParam.nColorThresholdMode == 2) {
					nCount_Color = sParam.nCount_HsvExtraction;
				}
				for (int k = 0; k < nCount_Color; ++k) {
					JET::mod::SThresholdParam sThreshold_Color;
					sThreshold_Color.SetInput1(2, 2);
					if (sParam.nColorThresholdMode == 1) {
						sThreshold_Color.eMode = THRESHOLD_COLOR_TARGET;
						int nB = sParam.vtsColorExtraction[k].nValue_B;
						if (nB < 0) nB = 0;
						if (nB > 255) nB = 255;
						sThreshold_Color.nThreshold_Low = nB;
						int nG = sParam.vtsColorExtraction[k].nValue_G;
						if (nG < 0) nG = 0;
						if (nG > 255) nG = 255;
						sThreshold_Color.nThreshold_High = nG;
						int nR = sParam.vtsColorExtraction[k].nValue_R;
						if (nR < 0) nR = 0;
						if (nR > 255) nR = 255;
						sThreshold_Color.nAutoThreshold = nR;
						int nRange = sParam.vtsColorExtraction[k].nRange;
						if (nRange < 0) nRange = 0;
						if (nRange > 255) nRange = 255;
						sThreshold_Color.fAlpha = nRange;
						sThreshold_Color.bDark = true;
						sThreshold_Color.SetQueueId(nQueueId);
						sPM.Set(sThreshold_Color);
						++nQueueId;
					}
					else if (sParam.nColorThresholdMode == 2) {
						sThreshold_Color.eMode = THRESHOLD_HSV_TARGET;
						sThreshold_Color.vtnLow.resize(3, 0);
						sThreshold_Color.vtnUpper.resize(3, 0);

						sThreshold_Color.vtnLow[0] = sParam.vtsHsvExtraction[k].nMean_H;
						sThreshold_Color.vtnLow[1] = sParam.vtsHsvExtraction[k].nMin_S;
						sThreshold_Color.vtnLow[2] = sParam.vtsHsvExtraction[k].nMin_V;

						sThreshold_Color.vtnUpper[0] = sParam.vtsHsvExtraction[k].nRange_H;
						sThreshold_Color.vtnUpper[1] = sParam.vtsHsvExtraction[k].nMax_S;
						sThreshold_Color.vtnUpper[2] = sParam.vtsHsvExtraction[k].nMax_V;

						sThreshold_Color.bDark = true;
						sThreshold_Color.SetQueueId(nQueueId);
						sPM.Set(sThreshold_Color);
						++nQueueId;
					}
				}

				if (nCount_Color >= 2) {
					SImageCalculatorParam sCalculator_OR;
					sCalculator_OR.SetInput1(2, 3);
					sCalculator_OR.SetInput2(2, 4);
					sCalculator_OR.eMode = CALCULATOR_OR;
					sCalculator_OR.SetQueueId(nQueueId);
					sPM.Set(sCalculator_OR);
					++nQueueId;

					for (int k = 0; k < nCount_Color - 2; ++k) {
						SImageCalculatorParam sCalculator_OR;
						sCalculator_OR.SetInput1(0, -1);
						sCalculator_OR.SetInput2(2, 5 + k);
						sCalculator_OR.eMode = CALCULATOR_OR;
						sCalculator_OR.SetQueueId(nQueueId);
						sPM.Set(sCalculator_OR);
						++nQueueId;
					}
				}
			}
			else {
				for (int k = 0; k < sParam.nCount_GrayExtraction; ++k) {
					int nRange = sParam.vtsGrayExtraction[k].nRange;
					int nLow = sParam.vtsGrayExtraction[k].nValue_Gray - nRange;
					if (nLow < 0) nLow = 0;
					int nHigh = sParam.vtsGrayExtraction[k].nValue_Gray + nRange;
					if (nHigh > 255) nHigh = 255;
					JET::mod::SThresholdParam sThreshold_Double;
					sThreshold_Double.SetInput1(2, 2);
					sThreshold_Double.eMode = THRESHOLD_DOUBLE;
					sThreshold_Double.bDark = true;
					sThreshold_Double.nThreshold_Low = nLow;
					sThreshold_Double.nThreshold_High = nHigh;
					sThreshold_Double.SetQueueId(nQueueId);
					sPM.Set(sThreshold_Double);
					++nQueueId;
				}

				if (sParam.nCount_GrayExtraction >= 2) {
					SImageCalculatorParam sCalculator_OR;
					sCalculator_OR.SetInput1(2, 3);
					sCalculator_OR.SetInput2(2, 4);
					sCalculator_OR.eMode = CALCULATOR_OR;
					sCalculator_OR.SetQueueId(nQueueId);
					sPM.Set(sCalculator_OR);
					++nQueueId;

					for (int k = 0; k < sParam.nCount_GrayExtraction - 2; ++k) {
						SImageCalculatorParam sCalculator_OR;
						sCalculator_OR.SetInput1(0, -1);
						sCalculator_OR.SetInput2(2, 5 + k);
						sCalculator_OR.eMode = CALCULATOR_OR;
						sCalculator_OR.SetQueueId(nQueueId);
						sPM.Set(sCalculator_OR);
						++nQueueId;
					}
				}
			}
#else
			if (sParam.nColorTransformMode == 1) {
				for (int k = 0; k < sParam.nCount_ColorExtraction; ++k) {
					JET::mod::SThresholdParam sThreshold_Color;
					sThreshold_Color.SetInput1(2, 2);
					sThreshold_Color.eMode = THRESHOLD_COLOR_TARGET;
					int nB = sParam.vtsColorExtraction[k].nValue_B;
					if (nB < 0) nB = 0;
					if (nB > 255) nB = 255;
					sThreshold_Color.nThreshold_Low = nB;
					int nG = sParam.vtsColorExtraction[k].nValue_G;
					if (nG < 0) nG = 0;
					if (nG > 255) nG = 255;
					sThreshold_Color.nThreshold_High = nG;
					int nR = sParam.vtsColorExtraction[k].nValue_R;
					if (nR < 0) nR = 0;
					if (nR > 255) nR = 255;
					sThreshold_Color.nAutoThreshold = nR;
					int nRange = sParam.vtsColorExtraction[k].nRange;
					if (nRange < 0) nRange = 0;
					if (nRange > 255) nRange = 255;
					sThreshold_Color.fAlpha = nRange;
					sThreshold_Color.bDark = true;
					sThreshold_Color.SetQueueId(nQueueId);
					sPM.Set(sThreshold_Color);
					++nQueueId;
				}

				if (sParam.nCount_ColorExtraction >= 2) {
					SImageCalculatorParam sCalculator_OR;
					sCalculator_OR.SetInput1(2, 3);
					sCalculator_OR.SetInput2(2, 4);
					sCalculator_OR.eMode = CALCULATOR_OR;
					sCalculator_OR.SetQueueId(nQueueId);
					sPM.Set(sCalculator_OR);
					++nQueueId;

					for (int k = 0; k < sParam.nCount_ColorExtraction - 2; ++k) {
						SImageCalculatorParam sCalculator_OR;
						sCalculator_OR.SetInput1(0, -1);
						sCalculator_OR.SetInput2(2, 5 + k);
						sCalculator_OR.eMode = CALCULATOR_OR;
						sCalculator_OR.SetQueueId(nQueueId);
						sPM.Set(sCalculator_OR);
						++nQueueId;
					}
				}
			}
			else {
				for (int k = 0; k < sParam.nCount_GrayExtraction; ++k) {
					int nRange = sParam.vtsGrayExtraction[k].nRange;
					int nLow = sParam.vtsGrayExtraction[k].nValue_Gray - nRange;
					if (nLow < 0) nLow = 0;
					int nHigh = sParam.vtsGrayExtraction[k].nValue_Gray + nRange;
					if (nHigh > 255) nHigh = 255;
					JET::mod::SThresholdParam sThreshold_Double;
					sThreshold_Double.SetInput1(2, 2);
					sThreshold_Double.eMode = THRESHOLD_DOUBLE;
					sThreshold_Double.bDark = true;
					sThreshold_Double.nThreshold_Low = nLow;
					sThreshold_Double.nThreshold_High = nHigh;
					sThreshold_Double.SetQueueId(nQueueId);
					sPM.Set(sThreshold_Double);
					++nQueueId;
				}

				if (sParam.nCount_GrayExtraction >= 2) {
					SImageCalculatorParam sCalculator_OR;
					sCalculator_OR.SetInput1(2, 3);
					sCalculator_OR.SetInput2(2, 4);
					sCalculator_OR.eMode = CALCULATOR_OR;
					sCalculator_OR.SetQueueId(nQueueId);
					sPM.Set(sCalculator_OR);
					++nQueueId;

					for (int k = 0; k < sParam.nCount_GrayExtraction - 2; ++k) {
						SImageCalculatorParam sCalculator_OR;
						sCalculator_OR.SetInput1(0, -1);
						sCalculator_OR.SetInput2(2, 5 + k);
						sCalculator_OR.eMode = CALCULATOR_OR;
						sCalculator_OR.SetQueueId(nQueueId);
						sPM.Set(sCalculator_OR);
						++nQueueId;
					}
				}
			}
#endif

			// 4
			JET::mod::SMorphologParam sMorpholog_CloseOpen;
			int nOpenX = (sParam.nOpenX > 0) ? sParam.nOpenX : 1;
			int nOpenY = (sParam.nOpenY > 0) ? sParam.nOpenY : 1;
			sMorpholog_CloseOpen.SetInput1(0, -1);
			sMorpholog_CloseOpen.eMode = MORPHOLOG_CLOSEOPEN;
			sMorpholog_CloseOpen.eElement = ELEMENT_ELLIPSE;
			sMorpholog_CloseOpen.nSizeX = nOpenX;
			sMorpholog_CloseOpen.nSizeY = nOpenY;
			sMorpholog_CloseOpen.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_CloseOpen);
			++nQueueId;

			
			// 二值化+型態學完成
			m_nGlueROIIndex = nQueueId - 1;

			// 5
			JET::mod::SMorphologParam sMorpholog_Connect;
			int nConnectX = (sParam.nConnectX > 0) ? sParam.nConnectX : 1;
			int nConnectY = (sParam.nConnectY > 0) ? sParam.nConnectY : 1;
			sMorpholog_Connect.SetInput1(0, -1);
			sMorpholog_Connect.eMode = MORPHOLOG_DILATE;
			sMorpholog_Connect.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Connect.nSizeX = nConnectX;
			sMorpholog_Connect.nSizeY = nConnectY;
			sMorpholog_Connect.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Connect);
			++nQueueId;

			switch (sParam.sDeleteMode.nMode) {
			case 1: // 取最大面積
#pragma region Max Area
			{
				// 6
				JET::mod::SContoursShapeParam sContours_Search;
				sContours_Search.SetInput1(0, -1);
				sContours_Search.eMode = CONTOUR_SEARCH;
				sContours_Search.bBoundingToEdge = true;
				sContours_Search.bRectangle = true;
				sContours_Search.bToEdge = true;
				sContours_Search.nSearchType = 1;
				sContours_Search.nSortType = 1;
				sContours_Search.SetQueueId(nQueueId);
				sPM.Set(sContours_Search);
				++nQueueId;

				// 輪廓Id
				m_nContoursIndex = nQueueId - 1;

				// 7
				JET::mod::SDeleteObjectParam sDelete_MaxArea;
				sDelete_MaxArea.SetContours1(0, -1);
				sDelete_MaxArea.eMode_Width = KEEP_ROI_MAXAREA;
				sDelete_MaxArea.nHeight_Max = 1;
				sDelete_MaxArea.nOutputMode = 2;
				sDelete_MaxArea.SetQueueId(nQueueId);
				sPM.Set(sDelete_MaxArea);
				++nQueueId;

				// 8
				JET::mod::SContoursShapeParam sContours_Fill;
				sContours_Fill.SetContours1(0, -1);
#if 0
				sContours_Fill.eMode = CONTOUR_FILL;
#else
				sContours_Fill.bToEdge = true;
				sContours_Fill.eMode = CONTOUR_FLOOD_FILL;
				sContours_Fill.SetMask1(sMorpholog_Connect);
#endif		
				sContours_Fill.SetQueueId(nQueueId);
				sPM.Set(sContours_Fill);
				++nQueueId;

				// 9
				JET::mod::SMorphologParam sMorpholog_Dilate;
				int nDilate_X = (sParam.nDilateX > 0) ? sParam.nDilateX : 1;
				int nDilate_Y = (sParam.nDilateY > 0) ? sParam.nDilateY : 1;
				sMorpholog_Dilate.SetInput1(0, -1);
				sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
				sMorpholog_Dilate.eMode = MORPHOLOG_DILATE;
				sMorpholog_Dilate.nSizeX = nDilate_X;
				sMorpholog_Dilate.nSizeY = nDilate_Y;
				sMorpholog_Dilate.SetQueueId(nQueueId);
				sPM.Set(sMorpholog_Dilate);
				++nQueueId;

				// 10
				JET::mod::SMorphologParam sMorpholog_Erosion;
				int nErosionX = (sParam.nErosionX > 0) ? sParam.nErosionX : 1;
				int nErosionY = (sParam.nErosionY > 0) ? sParam.nErosionY : 1;
				sMorpholog_Erosion.SetInput1(0, -1);
				sMorpholog_Erosion.eElement = ELEMENT_ELLIPSE;
				sMorpholog_Erosion.eMode = MORPHOLOG_EROSION;
				sMorpholog_Erosion.nSizeX = nErosionX;
				sMorpholog_Erosion.nSizeY = nErosionY;
				sMorpholog_Erosion.SetQueueId(nQueueId);
				sPM.Set(sMorpholog_Erosion);
				++nQueueId;

				// 11
				JET::mod::SContoursShapeParam sContours_Search2;
				sContours_Search2.SetInput1(0, -1);
				sContours_Search2.eMode = CONTOUR_SEARCH;
				sContours_Search2.bBoundingToEdge = true;
				sContours_Search2.bRectangle = true;
				sContours_Search2.bToEdge = true;
				sContours_Search2.nSearchType = 1;
				sContours_Search2.nSortType = 1;
				sContours_Search2.SetQueueId(nQueueId);
				sPM.Set(sContours_Search2);
				++nQueueId;

				// 12
				JET::mod::SDeleteObjectParam sDelete_MaxArea2;
				sDelete_MaxArea2.SetContours1(0, -1);
				sDelete_MaxArea2.eMode_Width = KEEP_ROI_MAXAREA;
				sDelete_MaxArea2.nHeight_Max = 1;
				sDelete_MaxArea2.nOutputMode = 3;
				sDelete_MaxArea2.SetQueueId(nQueueId);
				sPM.Set(sDelete_MaxArea2);
				++nQueueId;
			}
#pragma endregion
				break;
			case 2: // 基於指定物件的距離
#pragma region Base the object
			{
				// 6---膠邊界
				int nGlue_QueueId = nQueueId;
				JET::mod::SContoursShapeParam sContours_Search;
				sContours_Search.SetInput1(0, -1);
				sContours_Search.eMode = CONTOUR_SEARCH;
				sContours_Search.bBoundingToEdge = true;
				sContours_Search.bRectangle = true;
				sContours_Search.bToEdge = true;
				sContours_Search.nSearchType = 1;
				sContours_Search.nSortType = 1;
				sContours_Search.SetQueueId(nQueueId);
				sPM.Set(sContours_Search);
				++nQueueId;

				// 輪廓Id
				m_nContoursIndex = nQueueId - 1;

				// 7---IC邊界
				int nIC_QueueId = nQueueId;
				JET::mod::SContoursShapeParam sContours_Search_IC;
				sContours_Search_IC.SetInput1(1, 2);
				sContours_Search_IC.eMode = CONTOUR_SEARCH;
				sContours_Search_IC.bBoundingToEdge = true;
				sContours_Search_IC.bRectangle = true;
				sContours_Search_IC.bToEdge = true;
				sContours_Search_IC.nSearchType = 1;
				sContours_Search_IC.nSortType = 1;
				sContours_Search_IC.SetQueueId(nQueueId);
				sPM.Set(sContours_Search_IC);
				++nQueueId;

				// 8
				JET::mod::SDeleteObjectParam sDelete_Dist;
				sDelete_Dist.SetContours1(2, nGlue_QueueId);
				sDelete_Dist.SetContours2(2, nIC_QueueId);
				sDelete_Dist.bEdge = true;
				switch (m_sParam_GlueHeight.nMeasuringDirection) {
				case 1:
				case 3:
					sDelete_Dist.eMode_Height = KEEP_DISTANCE_LESSTHAN;
					sDelete_Dist.eMode_Width = DELETE_NONE;
					sDelete_Dist.nHeight_Min = m_sParam_GlueHeight.fIC_Height / m_sParam_GlueHeight.fResolutionY;
					break;

				case 2:
				case 4:
					sDelete_Dist.eMode_Height = DELETE_NONE;
					sDelete_Dist.eMode_Width = KEEP_DISTANCE_LESSTHAN;
					sDelete_Dist.nWidth_Min = m_sParam_GlueHeight.fIC_Height / m_sParam_GlueHeight.fResolutionX;
					break;
				}
				sDelete_Dist.SetQueueId(nQueueId);
				sPM.Set(sDelete_Dist);
				++nQueueId;

				// 9
				JET::mod::SContoursShapeParam sContours_Fill;
				sContours_Fill.SetContours1(0, -1);
#if 0
				sContours_Fill.eMode = CONTOUR_FILL;
#else
				sContours_Fill.eMode = CONTOUR_FLOOD_FILL;
				sContours_Fill.SetMask1(sMorpholog_Connect);
				sContours_Fill.bToEdge = true;
#endif
				sContours_Fill.SetQueueId(nQueueId);
				sPM.Set(sContours_Fill);
				++nQueueId;

				// 10
				JET::mod::SMorphologParam sMorpholog_Dilate;
				int nDilate_X = (sParam.nDilateX > 0) ? sParam.nDilateX : 1;
				int nDilate_Y = (sParam.nDilateY > 0) ? sParam.nDilateY : 1;
				sMorpholog_Dilate.SetInput1(0, -1);
				sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
				sMorpholog_Dilate.eMode = MORPHOLOG_DILATE;
				sMorpholog_Dilate.nSizeX = nDilate_X;
				sMorpholog_Dilate.nSizeY = nDilate_Y;
				sMorpholog_Dilate.SetQueueId(nQueueId);
				sPM.Set(sMorpholog_Dilate);
				++nQueueId;

				// 11
				JET::mod::SMorphologParam sMorpholog_Erosion;
				int nErosionX = (sParam.nErosionX > 0) ? sParam.nErosionX : 1;
				int nErosionY = (sParam.nErosionY > 0) ? sParam.nErosionY : 1;
				sMorpholog_Erosion.SetInput1(0, -1);
				sMorpholog_Erosion.eElement = ELEMENT_ELLIPSE;
				sMorpholog_Erosion.eMode = MORPHOLOG_EROSION;
				sMorpholog_Erosion.nSizeX = nErosionX;
				sMorpholog_Erosion.nSizeY = nErosionY;
				sMorpholog_Erosion.SetQueueId(nQueueId);
				sPM.Set(sMorpholog_Erosion);
				++nQueueId;

				// 12
				JET::mod::SContoursShapeParam sContours_Search2;
				sContours_Search2.SetInput1(0, -1);
				sContours_Search2.eMode = CONTOUR_SEARCH;
				sContours_Search2.bBoundingToEdge = true;
				sContours_Search2.bRectangle = true;
				sContours_Search2.bToEdge = true;
				sContours_Search2.nSearchType = 1;
				sContours_Search2.nSortType = 1;
				sContours_Search2.SetQueueId(nQueueId);
				sPM.Set(sContours_Search2);
				++nQueueId;
			}
#pragma endregion
				break;
			case 3:
#pragma region All Object
				JET::mod::SContoursShapeParam sContours_Search;
				sContours_Search.SetInput1(0, -1);
				sContours_Search.eMode = CONTOUR_SEARCH;
				sContours_Search.bBoundingToEdge = false;
				sContours_Search.bRectangle = true;
				sContours_Search.bToEdge = true;
				sContours_Search.nSearchType = 1;
				sContours_Search.SetQueueId(nQueueId);
				sPM.Set(sContours_Search);
				++nQueueId;
#pragma endregion
				break;
			}
			
			if (!sPM.CreateList()) {
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::SetFindEdgeProcess_GlueHeight(const SFindEdge_Parameter& sParam, SProcessModeParam& sPM)
		{
			int nQueueId = 1;
			sPM.Clear();

			// 1
			JET::mod::SColorTransformParam sColorTransform;
			sColorTransform.SetInput1(1, 1);
			switch (sParam.nColorTransformMode)
			{
			case 1:// 彩色
				sColorTransform.eMode = GRAY_TO_BGR;
				break;
			case 2:// R
			case 3:// G
			case 4:// B
				sColorTransform.eMode = COLOR_TO_GRAY_BGR;
				sColorTransform.nID = 4 - sParam.nColorTransformMode;
				break;
			case 5:
				sColorTransform.eMode = COLOR_TO_GRAY_AVERAGE;
				break;
			case 6: // Log_STD
			case 7: // CLAHE
			case 8:	// Sigmoid
				sColorTransform.eMode = COLOR_TO_NONE;
				break;
			}
			sColorTransform.SetQueueId(nQueueId);
			sPM.Set(sColorTransform);
			++nQueueId;

			// 2
			int nMedianSize = (sParam.nMedianSize > 0) ? sParam.nMedianSize : 1;
			JET::mod::SFilterParam sFilter_Median;
			sFilter_Median.SetInput1(0, -1);
			switch (sParam.nFilterMode) {
			case 1:
				sFilter_Median.eMode = FILTER_MEDIAN;
				break;
			case 2:
				sFilter_Median.eMode = FILTER_AVERAGE;
				break;
			case 3:
				sFilter_Median.eMode = FILTER_MAX;
				sFilter_Median.eElement = ELEMENT_RECT;
				break;
			case 4:
				sFilter_Median.eMode = FILTER_MIN;
				sFilter_Median.eElement = ELEMENT_RECT;
				break;
			case 5:
				sFilter_Median.eMode = FILTER_MAXMIN;
				sFilter_Median.eElement = ELEMENT_RECT;
				break;
			case 6:
				sFilter_Median.eMode = FILTER_MINMAX;
				sFilter_Median.eElement = ELEMENT_RECT;
				break;
			}
			sFilter_Median.nSizeX = nMedianSize;
			sFilter_Median.nSizeY = nMedianSize;
			sFilter_Median.SetQueueId(nQueueId);
			sPM.Set(sFilter_Median);
			++nQueueId;

			// 3
			if (sParam.nColorTransformMode == 1 || sParam.nColorTransformMode == 6) {
				int nCount_Color = 0;
				if (sParam.nColorThresholdMode == 1) {
					nCount_Color = sParam.nCount_ColorExtraction;
				}
				else if (sParam.nColorThresholdMode == 2) {
					nCount_Color = sParam.nCount_HsvExtraction;
				}
				for (int k = 0; k < nCount_Color; ++k) {
					JET::mod::SThresholdParam sThreshold_Color;
					sThreshold_Color.SetInput1(2, 2);
					if (sParam.nColorThresholdMode == 1) {
						sThreshold_Color.eMode = THRESHOLD_COLOR_TARGET;
						int nB = sParam.vtsColorExtraction[k].nValue_B;
						if (nB < 0) nB = 0;
						if (nB > 255) nB = 255;
						sThreshold_Color.nThreshold_Low = nB;
						int nG = sParam.vtsColorExtraction[k].nValue_G;
						if (nG < 0) nG = 0;
						if (nG > 255) nG = 255;
						sThreshold_Color.nThreshold_High = nG;
						int nR = sParam.vtsColorExtraction[k].nValue_R;
						if (nR < 0) nR = 0;
						if (nR > 255) nR = 255;
						sThreshold_Color.nAutoThreshold = nR;
						int nRange = sParam.vtsColorExtraction[k].nRange;
						if (nRange < 0) nRange = 0;
						if (nRange > 255) nRange = 255;
						sThreshold_Color.fAlpha = nRange;
						sThreshold_Color.bDark = true;
						sThreshold_Color.SetQueueId(nQueueId);
						sPM.Set(sThreshold_Color);
						++nQueueId;
					}
					else if (sParam.nColorThresholdMode == 2) {
						sThreshold_Color.eMode = THRESHOLD_HSV_TARGET;
						sThreshold_Color.vtnLow.resize(3, 0);
						sThreshold_Color.vtnUpper.resize(3, 0);

						sThreshold_Color.vtnLow[0] = sParam.vtsHsvExtraction[k].nMean_H;
						sThreshold_Color.vtnLow[1] = sParam.vtsHsvExtraction[k].nMin_S;
						sThreshold_Color.vtnLow[2] = sParam.vtsHsvExtraction[k].nMin_V;

						sThreshold_Color.vtnUpper[0] = sParam.vtsHsvExtraction[k].nRange_H;
						sThreshold_Color.vtnUpper[1] = sParam.vtsHsvExtraction[k].nMax_S;
						sThreshold_Color.vtnUpper[2] = sParam.vtsHsvExtraction[k].nMax_V;

						sThreshold_Color.bDark = true;
						sThreshold_Color.SetQueueId(nQueueId);
						sPM.Set(sThreshold_Color);
						++nQueueId;
					}
				}

				if (nCount_Color >= 2) {
					SImageCalculatorParam sCalculator_OR;
					sCalculator_OR.SetInput1(2, 3);
					sCalculator_OR.SetInput2(2, 4);
					sCalculator_OR.eMode = CALCULATOR_OR;
					sCalculator_OR.SetQueueId(nQueueId);
					sPM.Set(sCalculator_OR);
					++nQueueId;

					for (int k = 0; k < nCount_Color - 2; ++k) {
						SImageCalculatorParam sCalculator_OR;
						sCalculator_OR.SetInput1(0, -1);
						sCalculator_OR.SetInput2(2, 5 + k);
						sCalculator_OR.eMode = CALCULATOR_OR;
						sCalculator_OR.SetQueueId(nQueueId);
						sPM.Set(sCalculator_OR);
						++nQueueId;
					}
				}
			}
			else {
				for (int k = 0; k < sParam.nCount_GrayExtraction; ++k) {
					int nRange = sParam.vtsGrayExtraction[k].nRange;
					int nLow = sParam.vtsGrayExtraction[k].nValue_Gray - nRange;
					if (nLow < 0) nLow = 0;
					int nHigh = sParam.vtsGrayExtraction[k].nValue_Gray + nRange;
					if (nHigh > 255) nHigh = 255;
					JET::mod::SThresholdParam sThreshold_Double;
					sThreshold_Double.SetInput1(2, 2);
					sThreshold_Double.eMode = THRESHOLD_DOUBLE;
					sThreshold_Double.bDark = true;
					sThreshold_Double.nThreshold_Low = nLow;
					sThreshold_Double.nThreshold_High = nHigh;
					sThreshold_Double.SetQueueId(nQueueId);
					sPM.Set(sThreshold_Double);
					++nQueueId;
				}

				if (sParam.nCount_GrayExtraction >= 2) {
					SImageCalculatorParam sCalculator_OR;
					sCalculator_OR.SetInput1(2, 3);
					sCalculator_OR.SetInput2(2, 4);
					sCalculator_OR.eMode = CALCULATOR_OR;
					sCalculator_OR.SetQueueId(nQueueId);
					sPM.Set(sCalculator_OR);
					++nQueueId;

					for (int k = 0; k < sParam.nCount_GrayExtraction - 2; ++k) {
						SImageCalculatorParam sCalculator_OR;
						sCalculator_OR.SetInput1(0, -1);
						sCalculator_OR.SetInput2(2, 5 + k);
						sCalculator_OR.eMode = CALCULATOR_OR;
						sCalculator_OR.SetQueueId(nQueueId);
						sPM.Set(sCalculator_OR);
						++nQueueId;
					}
				}
			}

			// 4
			JET::mod::SMorphologParam sMorpholog_CloseOpen;
			int nOpenX = (sParam.nOpenX > 0) ? sParam.nOpenX : 1;
			int nOpenY = (sParam.nOpenY > 0) ? sParam.nOpenY : 1;
			sMorpholog_CloseOpen.SetInput1(0, -1);
			sMorpholog_CloseOpen.eMode = MORPHOLOG_CLOSEOPEN;
			sMorpholog_CloseOpen.eElement = ELEMENT_ELLIPSE;
			sMorpholog_CloseOpen.nSizeX = nOpenX;
			sMorpholog_CloseOpen.nSizeY = nOpenY;
			sMorpholog_CloseOpen.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_CloseOpen);
			++nQueueId;


			// 二值化+型態學完成
			m_nGlueROIIndex = nQueueId - 1;

			// 5
			JET::mod::SMorphologParam sMorpholog_Connect;
			int nConnectX = (sParam.nConnectX > 0) ? sParam.nConnectX : 1;
			int nConnectY = (sParam.nConnectY > 0) ? sParam.nConnectY : 1;
			sMorpholog_Connect.SetInput1(0, -1);
			sMorpholog_Connect.eMode = MORPHOLOG_DILATE;
			sMorpholog_Connect.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Connect.nSizeX = nConnectX;
			sMorpholog_Connect.nSizeY = nConnectY;
			sMorpholog_Connect.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Connect);
			++nQueueId;

			switch (sParam.sDeleteMode.nMode) {
			case 1: // 取最大面積
#pragma region Max Area
			{
				// 6
				JET::mod::SContoursShapeParam sContours_Search;
				sContours_Search.SetInput1(0, -1);
				sContours_Search.eMode = CONTOUR_SEARCH;
				sContours_Search.bBoundingToEdge = true;
				sContours_Search.bRectangle = true;
				sContours_Search.bToEdge = true;
				sContours_Search.nSearchType = 1;
				sContours_Search.nSortType = 1;
				sContours_Search.SetQueueId(nQueueId);
				sPM.Set(sContours_Search);
				++nQueueId;

				// 輪廓Id
				m_nContoursIndex = nQueueId - 1;

				// 7
				JET::mod::SDeleteObjectParam sDelete_MaxArea;
				sDelete_MaxArea.SetContours1(0, -1);
				sDelete_MaxArea.eMode_Width = KEEP_ROI_MAXAREA;
				sDelete_MaxArea.nHeight_Max = 1;
				sDelete_MaxArea.nOutputMode = 2;
				sDelete_MaxArea.SetQueueId(nQueueId);
				sPM.Set(sDelete_MaxArea);
				++nQueueId;

				// 8
				JET::mod::SContoursShapeParam sContours_Fill;
				sContours_Fill.SetContours1(0, -1);
				sContours_Fill.eMode = CONTOUR_FILL;
				sContours_Fill.SetQueueId(nQueueId);
				sPM.Set(sContours_Fill);
				++nQueueId;

				// 9
				JET::mod::SMorphologParam sMorpholog_Dilate;
				int nDilate_X = (sParam.nDilateX > 0) ? sParam.nDilateX : 1;
				int nDilate_Y = (sParam.nDilateY > 0) ? sParam.nDilateY : 1;
				sMorpholog_Dilate.SetInput1(0, -1);
				sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
				sMorpholog_Dilate.eMode = MORPHOLOG_DILATE;
				sMorpholog_Dilate.nSizeX = nDilate_X;
				sMorpholog_Dilate.nSizeY = nDilate_Y;
				sMorpholog_Dilate.SetQueueId(nQueueId);
				sPM.Set(sMorpholog_Dilate);
				++nQueueId;

				// 10
				JET::mod::SMorphologParam sMorpholog_Erosion;
				int nErosionX = (sParam.nErosionX > 0) ? sParam.nErosionX : 1;
				int nErosionY = (sParam.nErosionY > 0) ? sParam.nErosionY : 1;
				sMorpholog_Erosion.SetInput1(0, -1);
				sMorpholog_Erosion.eElement = ELEMENT_ELLIPSE;
				sMorpholog_Erosion.eMode = MORPHOLOG_EROSION;
				sMorpholog_Erosion.nSizeX = nErosionX;
				sMorpholog_Erosion.nSizeY = nErosionY;
				sMorpholog_Erosion.SetQueueId(nQueueId);
				sPM.Set(sMorpholog_Erosion);
				++nQueueId;

				// 11
				JET::mod::SContoursShapeParam sContours_Search2;
				sContours_Search2.SetInput1(0, -1);
				sContours_Search2.eMode = CONTOUR_SEARCH;
				sContours_Search2.bBoundingToEdge = true;
				sContours_Search2.bRectangle = true;
				sContours_Search2.bToEdge = true;
				sContours_Search2.nSearchType = 1;
				sContours_Search2.nSortType = 1;
				sContours_Search2.SetQueueId(nQueueId);
				sPM.Set(sContours_Search2);
				++nQueueId;

				// 12
				JET::mod::SDeleteObjectParam sDelete_MaxArea2;
				sDelete_MaxArea2.SetContours1(0, -1);
				sDelete_MaxArea2.eMode_Width = KEEP_ROI_MAXAREA;
				sDelete_MaxArea2.nHeight_Max = 1;
				sDelete_MaxArea2.nOutputMode = 3;
				sDelete_MaxArea2.SetQueueId(nQueueId);
				sPM.Set(sDelete_MaxArea2);
				++nQueueId;
			}
#pragma endregion
			break;
			case 2: // 基於指定物件的距離
#pragma region Base the object
			{
				// 6---膠邊界
				int nGlue_QueueId = nQueueId;
				JET::mod::SContoursShapeParam sContours_Search;
				sContours_Search.SetInput1(0, -1);
				sContours_Search.eMode = CONTOUR_SEARCH;
				sContours_Search.bBoundingToEdge = true;
				sContours_Search.bRectangle = true;
				sContours_Search.bToEdge = true;
				sContours_Search.nSearchType = 1;
				sContours_Search.nSortType = 1;
				sContours_Search.SetQueueId(nQueueId);
				sPM.Set(sContours_Search);
				++nQueueId;

				// 輪廓Id
				m_nContoursIndex = nQueueId - 1;

				// 7---IC邊界
				int nIC_QueueId = nQueueId;
				JET::mod::SContoursShapeParam sContours_Search_IC;
				sContours_Search_IC.SetInput1(1, 2);
				sContours_Search_IC.eMode = CONTOUR_SEARCH;
				//sContours_Search_IC.bBoundingToEdge = true;
				sContours_Search_IC.bRectangle = true;
				sContours_Search_IC.bToEdge = true;
				sContours_Search_IC.nSearchType = 1;
				sContours_Search_IC.nSortType = 1;
				sContours_Search_IC.SetQueueId(nQueueId);
				sPM.Set(sContours_Search_IC);
				++nQueueId;

				// 8
				JET::mod::SDeleteObjectParam sDelete_Dist;
				sDelete_Dist.SetContours1(2, nGlue_QueueId);
				sDelete_Dist.SetContours2(2, nIC_QueueId);
				sDelete_Dist.bEdge = true;
				switch (m_sParam_GlueHeight.nMeasuringDirection) {
				case 1:
				case 3:
					sDelete_Dist.eMode_Height = KEEP_DISTANCE_LESSTHAN;
					sDelete_Dist.eMode_Width = DELETE_NONE;
					sDelete_Dist.nHeight_Min = m_sParam_GlueHeight.fIC_Height / m_sParam_GlueHeight.fResolutionY;
					break;

				case 2:
				case 4:
					sDelete_Dist.eMode_Height = DELETE_NONE;
					sDelete_Dist.eMode_Width = KEEP_DISTANCE_LESSTHAN;
					sDelete_Dist.nWidth_Min = m_sParam_GlueHeight.fIC_Height / m_sParam_GlueHeight.fResolutionX;
					break;
				}
				sDelete_Dist.SetQueueId(nQueueId);
				sPM.Set(sDelete_Dist);
				++nQueueId;

				// 9
				JET::mod::SContoursShapeParam sContours_Fill;
				sContours_Fill.SetContours1(0, -1);
#if 0
				sContours_Fill.eMode = CONTOUR_FILL;
#else
				sContours_Fill.eMode = CONTOUR_FLOOD_FILL;
				sContours_Fill.SetMask1(sMorpholog_Connect);
				sContours_Fill.bToEdge = true;
#endif
				sContours_Fill.SetQueueId(nQueueId);
				sPM.Set(sContours_Fill);
				++nQueueId;

				// 10
				JET::mod::SMorphologParam sMorpholog_Dilate;
				int nDilate_X = (sParam.nDilateX > 0) ? sParam.nDilateX : 1;
				int nDilate_Y = (sParam.nDilateY > 0) ? sParam.nDilateY : 1;
				sMorpholog_Dilate.SetInput1(0, -1);
				sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
				sMorpholog_Dilate.eMode = MORPHOLOG_DILATE;
				sMorpholog_Dilate.nSizeX = nDilate_X;
				sMorpholog_Dilate.nSizeY = nDilate_Y;
				sMorpholog_Dilate.SetQueueId(nQueueId);
				sPM.Set(sMorpholog_Dilate);
				++nQueueId;

				// 11
				JET::mod::SMorphologParam sMorpholog_Erosion;
				int nErosionX = (sParam.nErosionX > 0) ? sParam.nErosionX : 1;
				int nErosionY = (sParam.nErosionY > 0) ? sParam.nErosionY : 1;
				sMorpholog_Erosion.SetInput1(0, -1);
				sMorpholog_Erosion.eElement = ELEMENT_ELLIPSE;
				sMorpholog_Erosion.eMode = MORPHOLOG_EROSION;
				sMorpholog_Erosion.nSizeX = nErosionX;
				sMorpholog_Erosion.nSizeY = nErosionY;
				sMorpholog_Erosion.SetQueueId(nQueueId);
				sPM.Set(sMorpholog_Erosion);
				++nQueueId;

				// 12
				JET::mod::SContoursShapeParam sContours_Search2;
				sContours_Search2.SetInput1(0, -1);
				sContours_Search2.eMode = CONTOUR_SEARCH;
				sContours_Search2.bBoundingToEdge = true;
				sContours_Search2.bRectangle = true;
				sContours_Search2.bToEdge = true;
				sContours_Search2.nSearchType = 1;
				sContours_Search2.nSortType = 1;
				sContours_Search2.SetQueueId(nQueueId);
				sPM.Set(sContours_Search2);
				++nQueueId;
			}
#pragma endregion
			break;
			case 3:
#pragma region All Object
				JET::mod::SContoursShapeParam sContours_Search;
				sContours_Search.SetInput1(0, -1);
				sContours_Search.eMode = CONTOUR_SEARCH;
				sContours_Search.bBoundingToEdge = false;
				sContours_Search.bRectangle = true;
				sContours_Search.bToEdge = true;
				sContours_Search.nSearchType = 1;
				sContours_Search.SetQueueId(nQueueId);
				sPM.Set(sContours_Search);
				++nQueueId;
#pragma endregion
				break;
			}

			if (!sPM.CreateList()) {
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::SetFindEdgeProcess_GlueHeight_IC(const SFindEdge_Parameter& sParam, SProcessModeParam& sPM)
		{
			int nQueueId = 1;
			sPM.Clear();

			// 1
			JET::mod::SColorTransformParam sColorTransform;
			sColorTransform.SetInput1(1, 1);
			switch (sParam.nColorTransformMode)
			{
			case 1:// 彩色
				sColorTransform.eMode = GRAY_TO_BGR;
				break;
			case 2:// R
			case 3:// G
			case 4:// B
				sColorTransform.eMode = COLOR_TO_GRAY_BGR;
				sColorTransform.nID = 4 - sParam.nColorTransformMode;
				break;
			case 5:
				sColorTransform.eMode = COLOR_TO_GRAY_AVERAGE;
				break;
			case 6: // Log_STD
			case 7: // CLAHE
			case 8:	// Sigmoid
				sColorTransform.eMode = COLOR_TO_NONE;
				break;
			}
			sColorTransform.SetQueueId(nQueueId);
			sPM.Set(sColorTransform);
			++nQueueId;

			// 2
			int nMedianSize = (sParam.nMedianSize > 0) ? sParam.nMedianSize : 1;
			JET::mod::SFilterParam sFilter_Median;
			sFilter_Median.SetInput1(0, -1);
			switch (sParam.nFilterMode) {
			case 1:
				sFilter_Median.eMode = FILTER_MEDIAN;
				break;
			case 2:
				sFilter_Median.eMode = FILTER_AVERAGE;
				break;
			case 3:
				sFilter_Median.eMode = FILTER_MAX;
				sFilter_Median.eElement = ELEMENT_RECT;
				break;
			case 4:
				sFilter_Median.eMode = FILTER_MIN;
				sFilter_Median.eElement = ELEMENT_RECT;
				break;
			case 5:
				sFilter_Median.eMode = FILTER_MAXMIN;
				sFilter_Median.eElement = ELEMENT_RECT;
				break;
			case 6:
				sFilter_Median.eMode = FILTER_MINMAX;
				sFilter_Median.eElement = ELEMENT_RECT;
				break;
			}
			sFilter_Median.nSizeX = nMedianSize;
			sFilter_Median.nSizeY = nMedianSize;
			sFilter_Median.SetQueueId(nQueueId);
			sPM.Set(sFilter_Median);
			++nQueueId;

			// 3
			if (sParam.nColorTransformMode == 1 || sParam.nColorTransformMode == 6) {
				int nCount_Color = 0;
				if (sParam.nColorThresholdMode == 1) {
					nCount_Color = sParam.nCount_ColorExtraction;
				}
				else if (sParam.nColorThresholdMode == 2) {
					nCount_Color = sParam.nCount_HsvExtraction;
				}
				for (int k = 0; k < nCount_Color; ++k) {
					JET::mod::SThresholdParam sThreshold_Color;
					sThreshold_Color.SetInput1(2, 2);
					if (sParam.nColorThresholdMode == 1) {
						sThreshold_Color.eMode = THRESHOLD_COLOR_TARGET;
						int nB = sParam.vtsColorExtraction[k].nValue_B;
						if (nB < 0) nB = 0;
						if (nB > 255) nB = 255;
						sThreshold_Color.nThreshold_Low = nB;
						int nG = sParam.vtsColorExtraction[k].nValue_G;
						if (nG < 0) nG = 0;
						if (nG > 255) nG = 255;
						sThreshold_Color.nThreshold_High = nG;
						int nR = sParam.vtsColorExtraction[k].nValue_R;
						if (nR < 0) nR = 0;
						if (nR > 255) nR = 255;
						sThreshold_Color.nAutoThreshold = nR;
						int nRange = sParam.vtsColorExtraction[k].nRange;
						if (nRange < 0) nRange = 0;
						if (nRange > 255) nRange = 255;
						sThreshold_Color.fAlpha = nRange;
						sThreshold_Color.bDark = true;
						sThreshold_Color.SetQueueId(nQueueId);
						sPM.Set(sThreshold_Color);
						++nQueueId;
					}
					else if (sParam.nColorThresholdMode == 2) {
						sThreshold_Color.eMode = THRESHOLD_HSV_TARGET;
						sThreshold_Color.vtnLow.resize(3, 0);
						sThreshold_Color.vtnUpper.resize(3, 0);

						sThreshold_Color.vtnLow[0] = sParam.vtsHsvExtraction[k].nMean_H;
						sThreshold_Color.vtnLow[1] = sParam.vtsHsvExtraction[k].nMin_S;
						sThreshold_Color.vtnLow[2] = sParam.vtsHsvExtraction[k].nMin_V;

						sThreshold_Color.vtnUpper[0] = sParam.vtsHsvExtraction[k].nRange_H;
						sThreshold_Color.vtnUpper[1] = sParam.vtsHsvExtraction[k].nMax_S;
						sThreshold_Color.vtnUpper[2] = sParam.vtsHsvExtraction[k].nMax_V;

						sThreshold_Color.bDark = true;
						sThreshold_Color.SetQueueId(nQueueId);
						sPM.Set(sThreshold_Color);
						++nQueueId;
					}
				}

				if (nCount_Color >= 2) {
					SImageCalculatorParam sCalculator_OR;
					sCalculator_OR.SetInput1(2, 3);
					sCalculator_OR.SetInput2(2, 4);
					sCalculator_OR.eMode = CALCULATOR_OR;
					sCalculator_OR.SetQueueId(nQueueId);
					sPM.Set(sCalculator_OR);
					++nQueueId;

					for (int k = 0; k < nCount_Color - 2; ++k) {
						SImageCalculatorParam sCalculator_OR;
						sCalculator_OR.SetInput1(0, -1);
						sCalculator_OR.SetInput2(2, 5 + k);
						sCalculator_OR.eMode = CALCULATOR_OR;
						sCalculator_OR.SetQueueId(nQueueId);
						sPM.Set(sCalculator_OR);
						++nQueueId;
					}
				}
			}
			else {
				for (int k = 0; k < sParam.nCount_GrayExtraction; ++k) {
					int nRange = sParam.vtsGrayExtraction[k].nRange;
					int nLow = sParam.vtsGrayExtraction[k].nValue_Gray - nRange;
					if (nLow < 0) nLow = 0;
					int nHigh = sParam.vtsGrayExtraction[k].nValue_Gray + nRange;
					if (nHigh > 255) nHigh = 255;
					JET::mod::SThresholdParam sThreshold_Double;
					sThreshold_Double.SetInput1(2, 2);
					sThreshold_Double.eMode = THRESHOLD_DOUBLE;
					sThreshold_Double.bDark = true;
					sThreshold_Double.nThreshold_Low = nLow;
					sThreshold_Double.nThreshold_High = nHigh;
					sThreshold_Double.SetQueueId(nQueueId);
					sPM.Set(sThreshold_Double);
					++nQueueId;
				}

				if (sParam.nCount_GrayExtraction >= 2) {
					SImageCalculatorParam sCalculator_OR;
					sCalculator_OR.SetInput1(2, 3);
					sCalculator_OR.SetInput2(2, 4);
					sCalculator_OR.eMode = CALCULATOR_OR;
					sCalculator_OR.SetQueueId(nQueueId);
					sPM.Set(sCalculator_OR);
					++nQueueId;

					for (int k = 0; k < sParam.nCount_GrayExtraction - 2; ++k) {
						SImageCalculatorParam sCalculator_OR;
						sCalculator_OR.SetInput1(0, -1);
						sCalculator_OR.SetInput2(2, 5 + k);
						sCalculator_OR.eMode = CALCULATOR_OR;
						sCalculator_OR.SetQueueId(nQueueId);
						sPM.Set(sCalculator_OR);
						++nQueueId;
					}
				}
			}

			// 4
			JET::mod::SMorphologParam sMorpholog_CloseOpen;
			int nOpenX = (sParam.nOpenX > 0) ? sParam.nOpenX : 1;
			int nOpenY = (sParam.nOpenY > 0) ? sParam.nOpenY : 1;
			sMorpholog_CloseOpen.SetInput1(0, -1);
			sMorpholog_CloseOpen.eMode = MORPHOLOG_CLOSEOPEN;
			sMorpholog_CloseOpen.eElement = ELEMENT_ELLIPSE;
			sMorpholog_CloseOpen.nSizeX = nOpenX;
			sMorpholog_CloseOpen.nSizeY = nOpenY;
			sMorpholog_CloseOpen.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_CloseOpen);
			++nQueueId;


			// 二值化+型態學完成
			m_nGlueROIIndex = nQueueId - 1;

			// 5
			JET::mod::SMorphologParam sMorpholog_Connect;
			int nConnectX = (sParam.nConnectX > 0) ? sParam.nConnectX : 1;
			int nConnectY = (sParam.nConnectY > 0) ? sParam.nConnectY : 1;
			sMorpholog_Connect.SetInput1(0, -1);
			sMorpholog_Connect.eMode = MORPHOLOG_DILATE;
			sMorpholog_Connect.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Connect.nSizeX = nConnectX;
			sMorpholog_Connect.nSizeY = nConnectY;
			sMorpholog_Connect.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Connect);
			++nQueueId;

			switch (sParam.sDeleteMode.nMode) {
			case 1: // 取最大面積
#pragma region Max Area
			{
				// 6
				JET::mod::SContoursShapeParam sContours_Search;
				sContours_Search.SetInput1(0, -1);
				sContours_Search.eMode = CONTOUR_SEARCH;
				sContours_Search.bBoundingToEdge = true;
				sContours_Search.bRectangle = true;
				sContours_Search.bToEdge = true;
				sContours_Search.nSearchType = 1;
				sContours_Search.nSortType = 1;
				sContours_Search.SetQueueId(nQueueId);
				sPM.Set(sContours_Search);
				++nQueueId;

				// 輪廓Id
				m_nContoursIndex = nQueueId - 1;

				// 7
				JET::mod::SDeleteObjectParam sDelete_MaxArea;
				sDelete_MaxArea.SetContours1(0, -1);
				sDelete_MaxArea.eMode_Width = KEEP_ROI_MAXAREA;
				sDelete_MaxArea.nHeight_Max = 1;
				sDelete_MaxArea.nOutputMode = 2;
				sDelete_MaxArea.SetQueueId(nQueueId);
				sPM.Set(sDelete_MaxArea);
				++nQueueId;

				// 8
				JET::mod::SContoursShapeParam sContours_Fill;
				sContours_Fill.SetContours1(0, -1);
				sContours_Fill.eMode = CONTOUR_FILL;
				sContours_Fill.SetQueueId(nQueueId);
				sPM.Set(sContours_Fill);
				++nQueueId;

				// 9
				JET::mod::SMorphologParam sMorpholog_Dilate;
				int nDilate_X = (sParam.nDilateX > 0) ? sParam.nDilateX : 1;
				int nDilate_Y = (sParam.nDilateY > 0) ? sParam.nDilateY : 1;
				sMorpholog_Dilate.SetInput1(0, -1);
				sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
				sMorpholog_Dilate.eMode = MORPHOLOG_DILATE;
				sMorpholog_Dilate.nSizeX = nDilate_X;
				sMorpholog_Dilate.nSizeY = nDilate_Y;
				sMorpholog_Dilate.SetQueueId(nQueueId);
				sPM.Set(sMorpholog_Dilate);
				++nQueueId;

				// 10
				JET::mod::SMorphologParam sMorpholog_Erosion;
				int nErosionX = (sParam.nErosionX > 0) ? sParam.nErosionX : 1;
				int nErosionY = (sParam.nErosionY > 0) ? sParam.nErosionY : 1;
				sMorpholog_Erosion.SetInput1(0, -1);
				sMorpholog_Erosion.eElement = ELEMENT_ELLIPSE;
				sMorpholog_Erosion.eMode = MORPHOLOG_EROSION;
				sMorpholog_Erosion.nSizeX = nErosionX;
				sMorpholog_Erosion.nSizeY = nErosionY;
				sMorpholog_Erosion.SetQueueId(nQueueId);
				sPM.Set(sMorpholog_Erosion);
				++nQueueId;

				// 11
				JET::mod::SContoursShapeParam sContours_Search2;
				sContours_Search2.SetInput1(0, -1);
				sContours_Search2.eMode = CONTOUR_SEARCH;
				sContours_Search2.bBoundingToEdge = true;
				sContours_Search2.bRectangle = true;
				sContours_Search2.bToEdge = true;
				sContours_Search2.nSearchType = 1;
				sContours_Search2.nSortType = 1;
				sContours_Search2.SetQueueId(nQueueId);
				sPM.Set(sContours_Search2);
				++nQueueId;

				// 12
				JET::mod::SDeleteObjectParam sDelete_MaxArea2;
				sDelete_MaxArea2.SetContours1(0, -1);
				sDelete_MaxArea2.eMode_Width = KEEP_ROI_MAXAREA;
				sDelete_MaxArea2.nHeight_Max = 1;
				sDelete_MaxArea2.nOutputMode = 3;
				sDelete_MaxArea2.SetQueueId(nQueueId);
				sPM.Set(sDelete_MaxArea2);
				++nQueueId;
			}
#pragma endregion
			break;
			case 2: // 刪除邊界物件
#pragma region Delete Border
			{
				// 6
				JET::mod::SContoursShapeParam sContours_Search;
				sContours_Search.SetInput1(0, -1);
				sContours_Search.eMode = CONTOUR_SEARCH;
				sContours_Search.bBoundingToEdge = true;
				sContours_Search.bRectangle = true;
				sContours_Search.bToEdge = true;
				sContours_Search.nSearchType = 1;
				sContours_Search.nSortType = 1;
				sContours_Search.SetQueueId(nQueueId);
				sPM.Set(sContours_Search);
				++nQueueId;

				// 7 保留遠離影像邊界的 ROI,	nWidth_Min=邊的位置(1=上, 2=下, 3=左, 4=右, 5=全部), nWidth_Max=距離
				JET::mod::SDeleteObjectParam sDelete_Keep;
				sDelete_Keep.SetContours1(0, -1);
				sDelete_Keep.eMode_Height = DELETE_NONE;
				sDelete_Keep.eMode_Width = KEEP_ROI_AWAY_BORDER;
				switch (m_sParam_GlueHeight.nMeasuringDirection) {
				case 1:
					sDelete_Keep.nWidth_Min = 2;
					break;
				case 2:
					sDelete_Keep.nWidth_Min = 3;
					break;
				case 3:
					sDelete_Keep.nWidth_Min = 1;
					break;
				case 4:
					sDelete_Keep.nWidth_Min = 4;
					break;
				}
				sDelete_Keep.nWidth_Max = 30;
				sDelete_Keep.nOutputMode = 2;
				sDelete_Keep.SetQueueId(nQueueId);
				sPM.Set(sDelete_Keep);
				++nQueueId;

				// 8
				JET::mod::SDeleteObjectParam sDelete_MaxArea;
				sDelete_MaxArea.SetContours1(0, -1);
				sDelete_MaxArea.eMode_Width = KEEP_ROI_MAXAREA;
				sDelete_MaxArea.nHeight_Max = 1;
				sDelete_MaxArea.nOutputMode = 2;
				sDelete_MaxArea.SetQueueId(nQueueId);
				sPM.Set(sDelete_MaxArea);
				++nQueueId;

				// 9
				JET::mod::SContoursShapeParam sContours_Fill;
				sContours_Fill.SetContours1(0, -1);
#if 1
				sContours_Fill.eMode = CONTOUR_FILL;
#else
				sContours_Fill.bToEdge = true;
				sContours_Fill.eMode = CONTOUR_FLOOD_FILL;
				sContours_Fill.SetMask1(sMorpholog_Connect);
#endif		
				sContours_Fill.SetQueueId(nQueueId);
				sPM.Set(sContours_Fill);
				++nQueueId;

				// 10
				JET::mod::SMorphologParam sMorpholog_Dilate;
				int nDilate_X = (sParam.nDilateX > 0) ? sParam.nDilateX : 1;
				int nDilate_Y = (sParam.nDilateY > 0) ? sParam.nDilateY : 1;
				sMorpholog_Dilate.SetInput1(0, -1);
				sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
				sMorpholog_Dilate.eMode = MORPHOLOG_DILATE;
				sMorpholog_Dilate.nSizeX = nDilate_X;
				sMorpholog_Dilate.nSizeY = nDilate_Y;
				sMorpholog_Dilate.SetQueueId(nQueueId);
				sPM.Set(sMorpholog_Dilate);
				++nQueueId;

				// 11
				JET::mod::SMorphologParam sMorpholog_Erosion;
				int nErosionX = (sParam.nErosionX > 0) ? sParam.nErosionX : 1;
				int nErosionY = (sParam.nErosionY > 0) ? sParam.nErosionY : 1;
				sMorpholog_Erosion.SetInput1(0, -1);
				sMorpholog_Erosion.eElement = ELEMENT_ELLIPSE;
				sMorpholog_Erosion.eMode = MORPHOLOG_EROSION;
				sMorpholog_Erosion.nSizeX = nErosionX;
				sMorpholog_Erosion.nSizeY = nErosionY;
				sMorpholog_Erosion.SetQueueId(nQueueId);
				sPM.Set(sMorpholog_Erosion);
				++nQueueId;

				// 12
				JET::mod::SContoursShapeParam sContours_Search2;
				sContours_Search2.SetInput1(0, -1);
				sContours_Search2.eMode = CONTOUR_SEARCH;
				sContours_Search2.bBoundingToEdge = true;
				sContours_Search2.bRectangle = true;
				sContours_Search2.bToEdge = true;
				sContours_Search2.nSearchType = 1;
				sContours_Search2.nSortType = 1;
				sContours_Search2.SetQueueId(nQueueId);
				sPM.Set(sContours_Search2);
				++nQueueId;

				// 13
				JET::mod::SDeleteObjectParam sDelete_MaxArea2;
				sDelete_MaxArea2.SetContours1(0, -1);
				sDelete_MaxArea2.eMode_Width = KEEP_ROI_MAXAREA;
				sDelete_MaxArea2.nHeight_Max = 1;
				sDelete_MaxArea2.nOutputMode = 3;
				sDelete_MaxArea2.SetQueueId(nQueueId);
				sPM.Set(sDelete_MaxArea2);
				++nQueueId;
			}
#pragma endregion
			break;
			}

			if (!sPM.CreateList()) {
				return false;
			}

			return true;
		}


		// 將ROI的輪廓的分成4條邊界線
		// vt2ptLine[0] = top, vt2ptLine[1] = bottom, vt2ptLine[2] = left, vt2ptLine[3] = right
		bool MeasurementBlackGlue::ROI_ContoursToLine(const int& nSearchRangeX, const int& nSearchRangeY, const int& nRange, SContoursParam* psContours, vector<vector<POINT>>& vt2ptLine, Rect& cvROI)
		{
			cvROI.x = -1;
			cvROI.y = -1;
			if (nSearchRangeX < 0 || nSearchRangeY < 0 || psContours == nullptr) return false;
			int nCount_Contours = psContours->GetCount();
			int nCount = psContours->GetCount_On();
			if (nCount_Contours == 0 || nCount == 0) return true;

			int nImageH = psContours->GetImageHeight();
			int nImageW = psContours->GetImageWidth();
			if (nImageH <= 0 || nImageW <= 0) return false;

			vector<int> vtnTimes_Y(nImageH, 0);
			vector<int> vtnTimes_X(nImageW, 0);

			SJRect* psjRect = nullptr;
			vector<vector<POINT>>* pvt2ptLine = nullptr;
			int nSumPoint = 0, nCountPoint = 0, nCount_Line = 0;
			for (int k = 0; k < nCount_Contours; ++k) {
				bool* pbOn = psContours->GetOnPtr(k);
				if (pbOn == nullptr)	return false;

				if (*pbOn) {
					pvt2ptLine = psContours->GetLinePtr(k);
					psjRect = psContours->GetSJRectPtr(k);
					if (pvt2ptLine == nullptr || psjRect == nullptr) return false;

					nCount_Line = pvt2ptLine->size();
					for (int h = 0; h < nCount_Line; ++h) {
						nCountPoint = (*pvt2ptLine)[h].size();
						nSumPoint += nCountPoint;
						POINT* ptPoint = &(*pvt2ptLine)[h][0];
						for (int p = 0; p < nCountPoint; ++p) {
							++vtnTimes_Y[ptPoint[p].y];
							++vtnTimes_X[ptPoint[p].x];
						}
					}
					k = nCount_Contours;
				}
			}

			vt2ptLine.resize(4, vector<POINT>(nSumPoint));
			vector<int> vtnId(4, 0);
			vector<int> vtnSearch1(4, 0);
			vector<int> vtnSearch2(4, 0);
			POINT ptCenter = psjRect->GetCenter();

			int nMaxY = 0, nIndexY1 = 0, nIndexY2 = 0;
			vector<int> vtnSumY(nImageH, 0);
			for (int y = 0; y < nImageH; ++y) {
				int nT = y - nRange;
				if (nT < 0) nT = 0;
				int nB = y + nRange;
				if (nB >= nImageH - 1) nB = nImageH - 1;
				for (int r = nT; r <= nB; ++r) {
					vtnSumY[y] += vtnTimes_Y[r];
				}
				if (vtnSumY[y] > nMaxY) {
					nMaxY = vtnSumY[y];
					nIndexY1 = y;
				}
			}

			nMaxY = 0;
			for (int y = 0; y < nImageH; ++y) {
				if (abs(y - nIndexY1) > psjRect->nHeight / 2) {
					if (vtnSumY[y] >= nMaxY) {
						nMaxY = vtnSumY[y];
						nIndexY2 = y;
					}
				}
			}

			vector<int> vtnSumX(nImageW, 0);
			int nMaxX = 0, nIndexX1 = 0, nIndexX2 = 0;
			for (int x = 0; x < nImageW; ++x) {
				int nL = x - nRange;
				if (nL < 0) nL = 0;
				int nR = x + nRange;
				if (nR >= nImageW-1) nR = nImageW - 1;
				for (int r = nL; r <= nR; ++r) {
					vtnSumX[x] += vtnTimes_X[r];
				}
				if (vtnSumX[x] > nMaxX) {
					nMaxX = vtnSumX[x];
					nIndexX1 = x;
				}
			}

			nMaxX = 0;
			for (int x = 0; x < nImageW; ++x) {
				if (abs(x - nIndexX1) > psjRect->nWidth / 2) {
					if (vtnSumX[x] >= nMaxX) {
						nMaxX = vtnSumX[x];
						nIndexX2 = x;
					}
				}
			}

			// Y
			if (nIndexY1 < nIndexY2) {
				// top
				vtnSearch1[0] = nIndexY1 - nSearchRangeY;
				if (vtnSearch1[0] < 0) vtnSearch1[0] = 0;
				vtnSearch2[0] = nIndexY1 + nSearchRangeY;
				if (vtnSearch2[0] >= nImageH) vtnSearch2[0] = nImageH - 1;

				// bottom
				vtnSearch1[1] = nIndexY2 - nSearchRangeY;
				if (vtnSearch1[1] < 0) vtnSearch1[1] = 0;
				vtnSearch2[1] = nIndexY2 + nSearchRangeY;
				if (vtnSearch2[1] >= nImageH) vtnSearch2[1] = nImageH - 1;
			}
			else {
				// top
				vtnSearch1[0] = nIndexY2 - nSearchRangeY;
				if (vtnSearch1[0] < 0) vtnSearch1[0] = 0;
				vtnSearch2[0] = nIndexY2 + nSearchRangeY;
				if (vtnSearch2[0] >= nImageH) vtnSearch2[0] = nImageH - 1;

				// bottom
				vtnSearch1[1] = nIndexY1 - nSearchRangeY;
				if (vtnSearch1[1] < 0) vtnSearch1[1] = 0;
				vtnSearch2[1] = nIndexY1 + nSearchRangeY;
				if (vtnSearch2[1] >= nImageH) vtnSearch2[1] = nImageH - 1;
			}

			// X
			if (nIndexX1 < nIndexX2) {
				// left
				vtnSearch1[2] = nIndexX1 - nSearchRangeX;
				if (vtnSearch1[2] < 0) vtnSearch1[2] = 0;
				vtnSearch2[2] = nIndexX1 + nSearchRangeX;
				if (vtnSearch2[2] >= nImageW) vtnSearch2[2] = nImageW - 1;

				// right
				vtnSearch1[3] = nIndexX2 - nSearchRangeX;
				if (vtnSearch1[3] < 0) vtnSearch1[3] = 0;
				vtnSearch2[3] = nIndexX2 + nSearchRangeX;
			}
			else {
				// left
				vtnSearch1[2] = nIndexX2 - nSearchRangeX;
				if (vtnSearch1[2] < 0) vtnSearch1[2] = 0;
				vtnSearch2[2] = nIndexX2 + nSearchRangeX;
				if (vtnSearch2[2] >= nImageW) vtnSearch2[2] = nImageW - 1;

				// right
				vtnSearch1[3] = nIndexX1 - nSearchRangeX;
				if (vtnSearch1[3] < 0) vtnSearch1[3] = 0;
				vtnSearch2[3] = nIndexX1 + nSearchRangeX;
			}

			for (int h = 0; h < nCount_Line; ++h) {
				int nCount_Point = (*pvt2ptLine)[h].size();
				POINT* ptPoint = &(*pvt2ptLine)[h][0];
				for (int p = 0; p < nCount_Point; ++p) {
					int nY = ptPoint[p].y;
					int nX = ptPoint[p].x;
					for (int s = 0; s < 4; ++s) {
						if (vtnId[s] >= nSumPoint) continue;
						switch (s) {
						case 0:
						case 1:
							if (vtnSearch1[s] <= nY && nY <= vtnSearch2[s]) {
								vt2ptLine[s][vtnId[s]] = ptPoint[p];
								++vtnId[s];
							}
							break;

						case 2:
						case 3:
							if (vtnSearch1[s] <= nX && nX <= vtnSearch2[s]) {
								vt2ptLine[s][vtnId[s]] = ptPoint[p];
								++vtnId[s];
							}
							break;
						}
					}
				}
			}
			
			int nL = (vtnSearch1[2] + vtnSearch2[2]) / 2;
			int nT = (vtnSearch1[0] + vtnSearch2[0]) / 2;
			int nR = (vtnSearch1[3] + vtnSearch2[3]) / 2;
			int nB = (vtnSearch1[1] + vtnSearch2[1]) / 2;

			cvROI.x = nL;
			cvROI.y = nT;
			cvROI.width = nR - nL - 1;
			cvROI.height = nB - nT - 1;
			for (int s = 0; s < 4; ++s) {
				vt2ptLine[s].resize(vtnId[s]);
			}

			return true;
		}

		// 將ROI的輪廓的分成4條邊界線
		// vt2ptLine[0] = top, vt2ptLine[1] = bottom, vt2ptLine[2] = left, vt2ptLine[3] = right
		bool MeasurementBlackGlue::ROI_ContoursToLine_Connector(const int& nSearchRangeX, const int& nSearchRangeY, const int& nRange, SContoursParam* psContours, vector<vector<POINT>>& vt2ptLine, Rect& cvROI)
		{
			if (nSearchRangeX < 0 || nSearchRangeY < 0 || psContours == nullptr) return false;
			int nCount_Contours = psContours->GetCount();
			int nCount = psContours->GetCount_On();
			if (nCount_Contours == 0 || nCount == 0) return true;

			int nImageH = psContours->GetImageHeight();
			int nImageW = psContours->GetImageWidth();
			if (nImageH <= 0 || nImageW <= 0) return false;

			vector<int> vtnTimes_Y(nImageH, 0);
			vector<int> vtnTimes_X(nImageW, 0);

			SJRect* psjRect = nullptr;
			vector<vector<POINT>>* pvt2ptLine = nullptr;
			int nSumPoint = 0, nCountPoint = 0, nCount_Line = 0;
			for (int k = 0; k < nCount_Contours; ++k) {
				bool* pbOn = psContours->GetOnPtr(k);
				if (pbOn == nullptr)	return false;

				if (*pbOn) {
					pvt2ptLine = psContours->GetLinePtr(k);
					psjRect = psContours->GetSJRectPtr(k);
					if (pvt2ptLine == nullptr || psjRect == nullptr) return false;

					if (!jet_imagefunction::SJRectToRect(*psjRect, 0, 0, cvROI, nImageH, nImageW)) {
						return false;
					}
					
					nCount_Line = pvt2ptLine->size();
					for (int h = 0; h < nCount_Line; ++h) {
						nCountPoint = (*pvt2ptLine)[h].size();
						nSumPoint += nCountPoint;
						POINT* ptPoint = &(*pvt2ptLine)[h][0];
						for (int p = 0; p < nCountPoint; ++p) {
							++vtnTimes_Y[ptPoint[p].y];
							++vtnTimes_X[ptPoint[p].x];
						}
					}
					k = nCount_Contours;
				}
			}

			vt2ptLine.resize(4, vector<POINT>(nSumPoint));
			vector<int> vtnId(4, 0);
			vector<int> vtnSearch1(4, 0);
			vector<int> vtnSearch2(4, 0);
			vector<int> vtnSumX(nImageW, 0);
			vector<int> vtnSumY(nImageH, 0);
			int nStart = 0, nEnd = 0;

#if 0
			// Top
			nStart = cvROI.y;
			nEnd = cvROI.y + nImageH / 4;
			if (nEnd >= nImageH) nEnd = nImageH - 1;
			int nMaxTop = 0, nTop = -1;
			for (int y = nStart; y <= nEnd; ++y) {
				if (y < 0 || y >= nImageH) continue;
				int nT = y - nRange;
				if (nT < 0) nT = 0;
				int nB = y + nRange;
				if (nB >= nImageH - 1) nB = nImageH - 1;
				for (int r = nT; r <= nB; ++r) {
					vtnSumY[y] += vtnTimes_Y[r];
				}
				if (vtnSumY[y] > nMaxTop) {
					nMaxTop = vtnSumY[y];
					nTop = y;
				}
			}

			if (nTop < 0) return false;
			vtnSearch1[0] = nTop - nSearchRangeY;
			if (vtnSearch1[0] < 0) vtnSearch1[0] = 0;
			vtnSearch2[0] = nTop + nSearchRangeY;
			if (vtnSearch2[0] >= nImageH) vtnSearch2[0] = nImageH - 1;

			// Bottom
			nStart = cvROI.br().y - nImageH / 4 - 1;
			if (nStart < 0) nStart = 0;
			nEnd = cvROI.br().y - 1;
			int nMaxBottom = 0, nBottom = -1;
			for (int y = nStart; y <= nEnd; ++y) {
				if (y < 0 || y >= nImageH) continue;
				int nT = y - nRange;
				if (nT < 0) nT = 0;
				int nB = y + nRange;
				if (nB >= nImageH - 1) nB = nImageH - 1;
				for (int r = nT; r <= nB; ++r) {
					vtnSumY[y] += vtnTimes_Y[r];
				}
				if (vtnSumY[y] > nMaxBottom) {
					nMaxBottom = vtnSumY[y];
					nBottom = y;
				}
			}

			if (nBottom < 0) return false;
			vtnSearch1[1] = nBottom - nSearchRangeY;
			if (vtnSearch1[1] < 0) vtnSearch1[1] = 0;
			vtnSearch2[1] = nBottom + nSearchRangeY;
			if (vtnSearch2[1] >= nImageH) vtnSearch2[1] = nImageH - 1;

			// Left
			nStart = cvROI.x;
			nEnd = cvROI.x + nImageW / 4;
			if (nEnd >= nImageW) nEnd = nImageW - 1;
			int nMaxLeft = 0, nLeft = -1;
			for (int x = nStart; x <= nEnd; ++x) {
				int nL = x - nRange;
				if (nL < 0) nL = 0;
				int nR = x + nRange;
				if (nR >= nImageW - 1) nR = nImageW - 1;
				for (int r = nL; r <= nR; ++r) {
					vtnSumX[x] += vtnTimes_X[r];
				}
				if (vtnSumX[x] > nMaxLeft) {
					nMaxLeft = vtnSumX[x];
					nLeft = x;
				}
			}

			if (nLeft < 0) return false;
			vtnSearch1[2] = nLeft - nSearchRangeX;
			if (vtnSearch1[2] < 0) vtnSearch1[2] = 0;
			vtnSearch2[2] = nLeft + nSearchRangeX;
			if (vtnSearch2[2] >= nImageW) vtnSearch2[2] = nImageW - 1;

			// Right
			nStart = cvROI.br().x - nImageW/4 - 1;
			if (nStart < 0) nStart = 0;
			nEnd = cvROI.br().x - 1;
			int nMaxRight = 0, nRight = -1;
			for (int x = nStart; x <= nEnd; ++x) {
				int nL = x - nRange;
				if (nL < 0) nL = 0;
				int nR = x + nRange;
				if (nR >= nImageW - 1) nR = nImageW - 1;
				for (int r = nL; r <= nR; ++r) {
					vtnSumX[x] += vtnTimes_X[r];
				}
				if (vtnSumX[x] > nMaxRight) {
					nMaxRight = vtnSumX[x];
					nRight = x;
				}
			}

			if (nRight < 0) return false;
			vtnSearch1[3] = nRight - nSearchRangeX;
			if (vtnSearch1[3] < 0) vtnSearch1[3] = 0;
			vtnSearch2[3] = nRight + nSearchRangeX;
#else
			vtnSearch1[0] = cvROI.y - nSearchRangeY;
			if (vtnSearch1[0] < 0) vtnSearch1[0] = 0;
			vtnSearch2[0] = cvROI.y + nSearchRangeY;
			if (vtnSearch2[0] >= nImageH) vtnSearch2[0] = nImageH - 1;

			vtnSearch1[1] = cvROI.br().y - nSearchRangeY - 1;
			if (vtnSearch1[1] < 0) vtnSearch1[1] = 0;
			vtnSearch2[1] = cvROI.br().y + nSearchRangeY - 1;
			if (vtnSearch2[1] >= nImageH) vtnSearch2[1] = nImageH - 1;

			vtnSearch1[2] = cvROI.x - nSearchRangeX;
			if (vtnSearch1[2] < 0) vtnSearch1[2] = 0;
			vtnSearch2[2] = cvROI.x + nSearchRangeX;
			if (vtnSearch2[2] >= nImageW) vtnSearch2[2] = nImageW - 1;

			vtnSearch1[3] = cvROI.br().x - nSearchRangeX - 1;
			if (vtnSearch1[3] < 0) vtnSearch1[3] = 0;
			vtnSearch2[3] = cvROI.br().x + nSearchRangeX - 1;
#endif

			
			
			
			for (int h = 0; h < nCount_Line; ++h) {
				int nCount_Point = (*pvt2ptLine)[h].size();
				POINT* ptPoint = &(*pvt2ptLine)[h][0];
				for (int p = 0; p < nCount_Point; ++p) {
					int nY = ptPoint[p].y;
					int nX = ptPoint[p].x;
					for (int s = 0; s < 4; ++s) {
						if (vtnId[s] >= nSumPoint) continue;
						switch (s) {
						case 0:
						case 1:
							if (vtnSearch1[s] <= nY && nY <= vtnSearch2[s]) {
								vt2ptLine[s][vtnId[s]] = ptPoint[p];
								++vtnId[s];
							}
							break;
						case 2:
						case 3:
							if (vtnSearch1[s] <= nX && nX <= vtnSearch2[s]) {
								vt2ptLine[s][vtnId[s]] = ptPoint[p];
								++vtnId[s];
							}
							break;
						}
					}
				}
			}

			for (int s = 0; s < 4; ++s) {
				vt2ptLine[s].resize(vtnId[s]);
			}

			return true;
		}

		// 計算IC邊到膠邊的距離
		void MeasurementBlackGlue::Calculate_Distance(const float& fResolutionX, const float& fResolutionY, Pixels_MeasurementInfo& sDist)
		{
			sDist.fDist_ICtoGlue = 0.0;
			if (sDist.ptStart.x == sDist.ptEnd.x && sDist.ptStart.y == sDist.ptEnd.y) {
				sDist.fDist_ICtoGlue = 0.0;
			}
			else {
				double dX1 = sDist.ptStart.x;
				double dY1 = sDist.ptStart.y;
				double dX2 = sDist.ptEnd.x;
				double dY2 = sDist.ptEnd.y;
				double dDiffX = (dX1 - dX2)*fResolutionX;
				double dDiffY = (dY1 - dY2)*fResolutionY;
				sDist.fDist_ICtoGlue = sqrt(pow(dDiffX, 2) + pow(dDiffY, 2));
			}
		}

		// 輸出最大距離
		bool MeasurementBlackGlue::OutputDistance_Max(SingleBlockGlueHeight_Result& sSingleResult)
		{
			int nCount = sSingleResult.sInfo.nPixelsCount;
			if (nCount < 1) return false;

			int nId = -1;
			float fMaxValue = 0.0;
			vector<Pixels_MeasurementInfo>& sPixelsInfo = sSingleResult.sInfo.vtsPixels;
			for (int p = 0; p < nCount; ++p) {
				if (fMaxValue < sPixelsInfo[p].fHeightRatio) {
					fMaxValue = sPixelsInfo[p].fHeightRatio;
					nId = p;
				}
			}

			if (nId == -1) return false;
			sSingleResult.Assign(1, sPixelsInfo[nId]);
			sSingleResult.fHeightRatio = sSingleResult.sFirst.fHeightRatio;

			return true;
		}
		bool MeasurementBlackGlue::OutputDistance_Max(MultipleBlockGlueHeight_Result& sMultipleResult)
		{
			if (sMultipleResult.nBlockCount < 1) {
				return false;
			}

			int nId = -1;
			float fMaxValue = 0.0;
			for (int k = 0; k < sMultipleResult.nBlockCount; ++k) {
				if (fMaxValue < sMultipleResult.vtsBlock[k].fHeightRatio) {
					fMaxValue = sMultipleResult.vtsBlock[k].fHeightRatio;
					nId = k;
				}
			}

			if (nId == -1) return false;
			sMultipleResult.fFinalHeightRatio = fMaxValue;
			sMultipleResult.sFinalFirst = sMultipleResult.vtsBlock[nId].sFirst;
			
			return true;
		}

		// 輸出最小距離
		bool MeasurementBlackGlue::OutputDistance_Min(SingleBlockGlueHeight_Result& sSingleResult)
		{
			int nCount = sSingleResult.sInfo.nPixelsCount;
			if (nCount < 1) return false;

			int nId = -1;
			float fMinValue = std::numeric_limits<float>::max();
			vector<Pixels_MeasurementInfo>& sPixelsInfo = sSingleResult.sInfo.vtsPixels;
			for (int p = 0; p < nCount; ++p) {
				if (fMinValue > sPixelsInfo[p].fHeightRatio) {
					fMinValue = sPixelsInfo[p].fHeightRatio;
					nId = p;
				}
			}
			if (nId == -1) return false;

			sSingleResult.Assign(1, sPixelsInfo[nId]);
			sSingleResult.fHeightRatio = sSingleResult.sFirst.fHeightRatio;

			return true;
		}
		bool MeasurementBlackGlue::OutputDistance_Min(MultipleBlockGlueHeight_Result& sMultipleResult)
		{
			if (sMultipleResult.nBlockCount < 1) {
				return false;
			}

			int nId = -1;
			float fMinValue = std::numeric_limits<float>::max();
			for (int k = 0; k < sMultipleResult.nBlockCount; ++k) {
				if (fMinValue > sMultipleResult.vtsBlock[k].fHeightRatio) {
					fMinValue = sMultipleResult.vtsBlock[k].fHeightRatio;
					nId = k;
				}
			}

			if (nId == -1) return false;
			sMultipleResult.fFinalHeightRatio = fMinValue;
			sMultipleResult.sFinalFirst = sMultipleResult.vtsBlock[nId].sFirst;

			return true;
		}

		// 輸出平均距離
		bool MeasurementBlackGlue::OutputDistance_Mean(SingleBlockGlueHeight_Result& sSingleResult)
		{
			int nCount = sSingleResult.sInfo.nPixelsCount;
			if (nCount < 1) return false;

			int nValidCount = 0;
			sSingleResult.fHeightRatio = 0.0;
			vector<Pixels_MeasurementInfo>& sPixelsInfo = sSingleResult.sInfo.vtsPixels;
			for (int p = 0; p < nCount; ++p) {
				sSingleResult.fHeightRatio += sPixelsInfo[p].fHeightRatio;
				++nValidCount;
			}

			if (nValidCount == 0) return false;
			sSingleResult.fHeightRatio /= nValidCount;
			sSingleResult.sFirst.fHeightRatio = sSingleResult.fHeightRatio;
			sSingleResult.sFirst.fGlueHeight = m_sParam_GlueHeight.fIC_Height *sSingleResult.sFirst.fHeightRatio / 100.0f;
			sSingleResult.sFirst.fDist_ICtoGlue = m_sParam_GlueHeight.fIC_Height - sSingleResult.sFirst.fGlueHeight;

			switch (m_sParam_GlueHeight.nMeasuringDirection) {
			case 1: case 3:
				sSingleResult.sFirst.ptStart = sPixelsInfo[sSingleResult.sInfo.nPixelsCount / 2].ptStart;
				sSingleResult.sFirst.ptEnd.x = sSingleResult.sFirst.ptStart.x;
				if (m_sParam_GlueHeight.nMeasuringDirection == 1) {
					sSingleResult.sFirst.ptEnd.y = sSingleResult.sFirst.ptStart.y + (sSingleResult.sFirst.fDist_ICtoGlue / m_sParam_GlueHeight.fResolutionY);
				}
				else {
					sSingleResult.sFirst.ptEnd.y = sSingleResult.sFirst.ptStart.y - (sSingleResult.sFirst.fDist_ICtoGlue / m_sParam_GlueHeight.fResolutionY);
				}
				break;
			case 2: case 4:
				sSingleResult.sFirst.ptStart = sPixelsInfo[sSingleResult.sInfo.nPixelsCount / 2].ptStart;
				sSingleResult.sFirst.ptEnd.y = sSingleResult.sFirst.ptStart.y;
				if (m_sParam_GlueHeight.nMeasuringDirection == 2) {
					sSingleResult.sFirst.ptEnd.x = sSingleResult.sFirst.ptStart.x - (sSingleResult.sFirst.fDist_ICtoGlue / m_sParam_GlueHeight.fResolutionX);
				}
				else {
					sSingleResult.sFirst.ptEnd.x = sSingleResult.sFirst.ptStart.x + (sSingleResult.sFirst.fDist_ICtoGlue / m_sParam_GlueHeight.fResolutionX);
				}
				break;
			}

			return true;
		}
		bool MeasurementBlackGlue::OutputDistance_Mean(MultipleBlockGlueHeight_Result& sMultipleResult)
		{
			if (sMultipleResult.nBlockCount < 1) {
				return false;
			}

			sMultipleResult.fFinalHeightRatio = 0.0;
			for (int k = 0; k < sMultipleResult.nBlockCount; ++k) {
				sMultipleResult.fFinalHeightRatio += sMultipleResult.vtsBlock[k].fHeightRatio;
			}
			sMultipleResult.fFinalHeightRatio /= sMultipleResult.nBlockCount;

			sMultipleResult.sFinalFirst.fHeightRatio = sMultipleResult.fFinalHeightRatio;
			sMultipleResult.sFinalFirst.fGlueHeight = m_sParam_GlueHeight.fIC_Height *sMultipleResult.fFinalHeightRatio / 100.0f;
			sMultipleResult.sFinalFirst.fDist_ICtoGlue = m_sParam_GlueHeight.fIC_Height - sMultipleResult.sFinalFirst.fGlueHeight;

			switch (m_sParam_GlueHeight.nMeasuringDirection) {
			case 1: case 3:
				sMultipleResult.sFinalFirst.ptStart = sMultipleResult.vtsBlock[sMultipleResult.nBlockCount / 2].sFirst.ptStart;
				sMultipleResult.sFinalFirst.ptEnd.x = sMultipleResult.sFinalFirst.ptStart.x;
				if (m_sParam_GlueHeight.nMeasuringDirection == 1) {
					sMultipleResult.sFinalFirst.ptEnd.y = sMultipleResult.sFinalFirst.ptStart.y + (sMultipleResult.sFinalFirst.fDist_ICtoGlue / m_sParam_GlueHeight.fResolutionY);
				}
				else {
					sMultipleResult.sFinalFirst.ptEnd.y = sMultipleResult.sFinalFirst.ptStart.y - (sMultipleResult.sFinalFirst.fDist_ICtoGlue / m_sParam_GlueHeight.fResolutionY);
				}
				break;
			case 2: case 4:
				sMultipleResult.sFinalFirst.ptStart = sMultipleResult.vtsBlock[sMultipleResult.nBlockCount / 2].sFirst.ptStart;
				sMultipleResult.sFinalFirst.ptEnd.y = sMultipleResult.sFinalFirst.ptStart.y;
				if (m_sParam_GlueHeight.nMeasuringDirection == 2) {
					sMultipleResult.sFinalFirst.ptEnd.x = sMultipleResult.sFinalFirst.ptStart.x - (sMultipleResult.sFinalFirst.fDist_ICtoGlue / m_sParam_GlueHeight.fResolutionX);
				}
				else {
					sMultipleResult.sFinalFirst.ptEnd.x = sMultipleResult.sFinalFirst.ptStart.x + (sMultipleResult.sFinalFirst.fDist_ICtoGlue / m_sParam_GlueHeight.fResolutionX);
				}
				break;
			}

			return true;
		}

		// 輸出(最大-最小)距離
		bool MeasurementBlackGlue::OutputDistance_MaxMin(SingleBlockGlueHeight_Result& sSingleResult)
		{
			int nCount = sSingleResult.sInfo.nPixelsCount;
			if (nCount < 1) return false;

			float fMaxValue = 0.0;
			float fMinValue = std::numeric_limits<float>::max();
			int nMaxId = -1, nMinId = -1;
			vector<Pixels_MeasurementInfo>& sPixelsInfo = sSingleResult.sInfo.vtsPixels;
			for (int p = 0; p < nCount; ++p) {
				if (fMaxValue < sPixelsInfo[p].fHeightRatio) {
					fMaxValue = sPixelsInfo[p].fHeightRatio;
					nMaxId = p;
				}

				if (fMinValue > sPixelsInfo[p].fHeightRatio) {
					fMinValue = sPixelsInfo[p].fHeightRatio;
					nMinId = p;
				}
			}
			if (nMaxId == -1 || nMinId == -1) return false;

			// 設定 sFirst 為最大
			sSingleResult.Assign(1, sPixelsInfo[nMaxId]);

			// 設定 sSecond 為最小
			sSingleResult.Assign(2, sPixelsInfo[nMinId]);

			// 計算高度差
			sSingleResult.fHeightRatio = fMaxValue - fMinValue;

			return true;
		}
		bool MeasurementBlackGlue::OutputDistance_MaxMin(MultipleBlockGlueHeight_Result& sMultipleResult)
		{
			if (sMultipleResult.nBlockCount < 1) {
				return false;
			}

			// Max
			int nMaxId = -1, nMinId = -1;
			float fMaxValue = 0.0;
			float fMinValue = std::numeric_limits<float>::max();
			for (int k = 0; k < sMultipleResult.nBlockCount; ++k) {
				if (fMaxValue < sMultipleResult.vtsBlock[k].fHeightRatio) {
					fMaxValue = sMultipleResult.vtsBlock[k].fHeightRatio;
					nMaxId = k;
				}

				if (fMinValue > sMultipleResult.vtsBlock[k].fHeightRatio) {
					fMinValue = sMultipleResult.vtsBlock[k].fHeightRatio;
					nMinId = k;
				}
			}

			if (nMaxId == -1 || nMinId == -1) return false;

			// 設定 sFirst 為最大
			sMultipleResult.sFinalFirst = sMultipleResult.vtsBlock[nMaxId].sFirst;

			// 設定 sSecond 為最小
			sMultipleResult.sFinalSecond = sMultipleResult.vtsBlock[nMinId].sSecond;

			// 計算高度差
			sMultipleResult.fFinalHeightRatio = fMaxValue - fMinValue;

			return true;
		}

		// 輸出(平均-最小)距離
		bool MeasurementBlackGlue::OutputDistance_MeanMin(SingleBlockGlueHeight_Result& sSingleResult)
		{
			int nCount = sSingleResult.sInfo.nPixelsCount;
			if (nCount < 1) return false;

			float fSum = 0.0, fMinValue = std::numeric_limits<float>::max();
			int nValidCount = 0, nMinId = -1;
			vector<Pixels_MeasurementInfo>& sPixelsInfo = sSingleResult.sInfo.vtsPixels;
			for (int p = 0; p < nCount; ++p) {
				if (fMinValue > sPixelsInfo[p].fHeightRatio) {
					fMinValue = sPixelsInfo[p].fHeightRatio;
					nMinId = p;
				}

				fSum += sPixelsInfo[p].fHeightRatio;
				++nValidCount;
			}
			if (nMinId == -1 || nValidCount == 0) {
				return false;
			}

			// 計算高度差
			sSingleResult.fHeightRatio = (fSum / nValidCount) - fMinValue;

			// 設定 sSecond 為最小
			sSingleResult.Assign(2, sPixelsInfo[nMinId]);

			// 設定 sFirst 為平均
			sSingleResult.sFirst.fHeightRatio = (fSum / nValidCount);
			sSingleResult.sFirst.fGlueHeight = m_sParam_GlueHeight.fIC_Height * sSingleResult.sFirst.fHeightRatio / 100.0f;
			sSingleResult.sFirst.fDist_ICtoGlue = m_sParam_GlueHeight.fIC_Height - sSingleResult.sFirst.fGlueHeight;
			switch (m_sParam_GlueHeight.nMeasuringDirection) {
			case 1: case 3:
				sSingleResult.sFirst.ptStart = sPixelsInfo[sSingleResult.sInfo.nPixelsCount / 2].ptStart;
				sSingleResult.sFirst.ptEnd.x = sSingleResult.sFirst.ptStart.x;
				if (m_sParam_GlueHeight.nMeasuringDirection == 1) {
					sSingleResult.sFirst.ptEnd.y = sSingleResult.sFirst.ptStart.y + (sSingleResult.sFirst.fDist_ICtoGlue / m_sParam_GlueHeight.fResolutionY);
				}
				else {
					sSingleResult.sFirst.ptEnd.y = sSingleResult.sFirst.ptStart.y - (sSingleResult.sFirst.fDist_ICtoGlue / m_sParam_GlueHeight.fResolutionY);
				}
				break;
			case 2: case 4:
				sSingleResult.sFirst.ptStart = sPixelsInfo[sSingleResult.sInfo.nPixelsCount / 2].ptStart;
				sSingleResult.sFirst.ptEnd.y = sSingleResult.sFirst.ptStart.y;
				if (m_sParam_GlueHeight.nMeasuringDirection == 2) {
					sSingleResult.sFirst.ptEnd.x = sSingleResult.sFirst.ptStart.x - (sSingleResult.sFirst.fDist_ICtoGlue / m_sParam_GlueHeight.fResolutionX);
				}
				else {
					sSingleResult.sFirst.ptEnd.x = sSingleResult.sFirst.ptStart.x + (sSingleResult.sFirst.fDist_ICtoGlue / m_sParam_GlueHeight.fResolutionX);
				}
				break;
			}

			return true;
		}
		bool MeasurementBlackGlue::OutputDistance_MeanMin(MultipleBlockGlueHeight_Result& sMultipleResult)
		{
			if (sMultipleResult.nBlockCount < 1) {
				return false;
			}

			int nMinId = -1;
			float fMean = 0.0;
			float fMinValue = std::numeric_limits<float>::max();
			for (int k = 0; k < sMultipleResult.nBlockCount; ++k) {
				if (fMinValue > sMultipleResult.vtsBlock[k].fHeightRatio) {
					fMinValue = sMultipleResult.vtsBlock[k].fHeightRatio;
					nMinId = k;
				}
				fMean += sMultipleResult.vtsBlock[k].fHeightRatio;
			}

			if (nMinId == -1) return false;
			fMean /= sMultipleResult.nBlockCount;

			// 計算高度差
			sMultipleResult.fFinalHeightRatio = fMean - fMinValue;

			// 設定 sSecond 為最小
			sMultipleResult.sFinalSecond = sMultipleResult.vtsBlock[nMinId].sFirst;

			// 設定 sFirst 為平均
			sMultipleResult.sFinalFirst.fHeightRatio = fMean;
			sMultipleResult.sFinalFirst.fGlueHeight = m_sParam_GlueHeight.fIC_Height * sMultipleResult.fFinalHeightRatio / 100.0f;
			sMultipleResult.sFinalFirst.fDist_ICtoGlue = m_sParam_GlueHeight.fIC_Height - sMultipleResult.sFinalFirst.fGlueHeight;
			switch (m_sParam_GlueHeight.nMeasuringDirection) {
			case 1: case 3:
				sMultipleResult.sFinalSecond.ptStart = sMultipleResult.vtsBlock[sMultipleResult.nBlockCount / 2].sFirst.ptStart;
				sMultipleResult.sFinalSecond.ptEnd.x = sMultipleResult.sFinalFirst.ptStart.x;
				if (m_sParam_GlueHeight.nMeasuringDirection == 1) {
					sMultipleResult.sFinalFirst.ptEnd.y = sMultipleResult.sFinalFirst.ptStart.y + (sMultipleResult.sFinalFirst.fDist_ICtoGlue / m_sParam_GlueHeight.fResolutionY);
				}
				else {
					sMultipleResult.sFinalFirst.ptEnd.y = sMultipleResult.sFinalFirst.ptStart.y - (sMultipleResult.sFinalFirst.fDist_ICtoGlue / m_sParam_GlueHeight.fResolutionY);
				}
				break;
			case 2: case 4:
				sMultipleResult.sFinalFirst.ptStart = sMultipleResult.vtsBlock[sMultipleResult.nBlockCount / 2].sFirst.ptStart;
				sMultipleResult.sFinalFirst.ptEnd.y = sMultipleResult.sFinalFirst.ptStart.y;
				if (m_sParam_GlueHeight.nMeasuringDirection == 2) {
					sMultipleResult.sFinalFirst.ptEnd.x = sMultipleResult.sFinalFirst.ptStart.x - (sMultipleResult.sFinalFirst.fDist_ICtoGlue / m_sParam_GlueHeight.fResolutionX);
				}
				else {
					sMultipleResult.sFinalFirst.ptEnd.x = sMultipleResult.sFinalFirst.ptStart.x + (sMultipleResult.sFinalFirst.fDist_ICtoGlue / m_sParam_GlueHeight.fResolutionX);
				}
				break;
			}
			sMultipleResult.sFinalFirst = sMultipleResult.vtsBlock[nMinId].sFirst;

			return true;
		}
#pragma endregion

#pragma region Glue Height Ratio

		// 找膠高邊界
		bool MeasurementBlackGlue::RunProcess_FindGlueHeightEdge()
		{
			if (!SetFindEdgeProcess_GlueHeight(m_sParam_GlueHeight.sGlueEdge, m_sPM_FindThermal_Glue)) {
				m_strErrorMessage = "找膠邊界流程參數設定失敗";
				return false;
			}

			vector<Mat> vtmatImage;
			switch (m_sParam_GlueHeight.sGlueEdge.sDeleteMode.nMode) {
			case 1:
				vtmatImage.resize(1);
				//vtmatImage[0] = m_matGlue;
				break;
			case 2:
				// 畫IC邊界
				Mat matIC_Edge(m_matGlue.size(), CV_8U, Scalar(0));
				int nSumCount = m_sResult_GlueHeight.sIcEdge.nContoursCount;
				for (int k = 0; k < nSumCount; ++k) {
					uchar* pu8Edge = matIC_Edge.ptr<uchar>(m_sResult_GlueHeight.sIcEdge.vtptContoursPos[k].y);
					pu8Edge[m_sResult_GlueHeight.sIcEdge.vtptContoursPos[k].x] = 255;
				}
				vtmatImage.resize(2);
				//vtmatImage[0] = m_matGlue;
				vtmatImage[1] = matIC_Edge;
				break;
			}

			switch (m_sParam_GlueHeight.sGlueEdge.nColorTransformMode) {
			case 6:
				jet_imagefunction::LogTransform_STD(m_matGlue, vtmatImage[0]);
				if (m_bSaveImage) {
					SaveImage(0, vtmatImage[0], "Glue_Log", m_nExtensionType);
				}
				break;
				//case 7:
				//	jet_imagefunction::BrightnessUniformityCorrection_CLAHE(m_matGlue, 16, 16, 3.0, vtmatImage[0]);
				//	break;
			default:
				vtmatImage[0] = m_matGlue;
				break;
			}

			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, m_sPM_FindThermal_Glue, vtsResult)) {
				m_strErrorMessage = "找膠邊界流程執行失敗-1";
				if (m_bSaveImage && m_nSaveLevel > 0) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName, "Error_FindGlueHeightEdge", m_sPM_FindThermal_Glue, vtmatImage, vtsResult, 2);
				}
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 1) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName, "OK_FindGlueHeightEdge", m_sPM_FindThermal_Glue, vtmatImage, vtsResult, 2);
			}

			int nNumber = m_sPM_FindThermal_Glue.GetBaseParameterNumber();
			if (nNumber != vtsResult.size()) {
				m_strErrorMessage = "找膠邊界流程執行失敗-2";
				return false;
			}

			SContoursParam* psContours = &vtsResult[nNumber - 1].sContours;
			if (psContours == nullptr) {
				m_strErrorMessage = "找膠邊界流程執行失敗-3";
				return false;
			}

			// 輸出輪廓點
			if (!ContoursConvertVector(psContours, m_sResult_GlueHeight.sGlueEdge.vtptContoursPos)) {
				m_strErrorMessage = "輸出膠邊界失敗";
				return false;
			}
			m_sResult_GlueHeight.sGlueEdge.nContoursCount = m_sResult_GlueHeight.sGlueEdge.vtptContoursPos.size();

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				jet_imagefunction::DrawEdgePoint(matDisplay, m_sResult_GlueHeight.sGlueEdge.vtptContoursPos, m_scContours_Glue, 1);
				SaveImage(0, matDisplay, "GlueContours", m_nExtensionType);
			}

			// 畫出邊界
			m_matEdgeMask = Mat(m_matGlue.size(), CV_8U, Scalar(0));

			if (m_sParam_GlueHeight.sGlueEdge.sDeleteMode.nMode == 2) {
				m_matEdgeMask = vtsResult[nNumber - 2].matImage.clone();
			}
			else {
				bool bFill = true;
				if (bFill) {
#if 1
					m_matEdgeMask = vtsResult[nNumber - 3].matImage.clone();
					//if (!jet_imagefunction::FillContours_All(*psContours, m_matEdgeMask)) {
					//	return false;
					//}
#else
					if (!jet_imagefunction::FillContours_Flood(*psContours, vtsResult[nNumber - 3].matImage, false, m_matEdgeMask)) {
						return false;
					}
#endif
				}
				else {
					for (int k = 0; k < m_sResult_GlueHeight.sGlueEdge.nContoursCount; ++k) {
						POINT ptPoint = m_sResult_GlueHeight.sGlueEdge.vtptContoursPos[k];
						uchar* pu8Glue = m_matEdgeMask.ptr<uchar>(ptPoint.y);
						pu8Glue[ptPoint.x] = 255;
					}
				}
			}

			if (m_bSaveImage) {
				SaveImage(0, m_matEdgeMask, "GlueEdgeMask", m_nExtensionType);
			}

			return true;
		}

		// 找IC邊界
		bool MeasurementBlackGlue::RunProcess_FindICEdge()
		{
			if (!SetFindEdgeProcess_GlueHeight_IC(m_sParam_GlueHeight.sIcEdge, m_sPM_FindThermal_IC)) {
				m_strErrorMessage = "IC找邊流程參數設定失敗";
				return false;
			}

			vector<Mat> vtmatImage(1);
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			switch (m_sParam_GlueHeight.sIcEdge.nColorTransformMode) {
			case 6:
				jet_imagefunction::LogTransform_STD(m_matGlue, vtmatImage[0]);
				if (m_bSaveImage) {
					SaveImage(0, vtmatImage[0], "IC_Log", m_nExtensionType);
				}
				break;
			//case 7:
			//	jet_imagefunction::BrightnessUniformityCorrection_CLAHE(m_matGlue, 16, 16, 3.0, vtmatImage[0]);
			//	break;
			default:
				vtmatImage[0] = m_matGlue;
				break;
			}

			if (!jet_imagefunction::RunProcessMode(vtmatImage, m_sPM_FindThermal_IC, vtsResult)) {
				m_strErrorMessage = "執行IC找邊流程失敗-1";
				if (m_bSaveImage && m_nSaveLevel > 0) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName, "Error_FindICEdge", m_sPM_FindThermal_IC, vtmatImage, vtsResult, 2);
				}
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 1) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName, "OK_FindICEdge", m_sPM_FindThermal_IC, vtmatImage, vtsResult, 2);
			}

			int nNumber = m_sPM_FindThermal_IC.GetBaseParameterNumber();
			if (nNumber != vtsResult.size()) {
				m_strErrorMessage = "執行IC找邊流程失敗-2";
				return false;
			}

			SContoursParam* psContours = &vtsResult[nNumber - 1].sContours;
			if (psContours == nullptr) {
				m_strErrorMessage = "執行IC找邊流程失敗-3";
				return false;
			}

			if (m_sParam_GlueHeight.bRebuilding_ICEdge) {
				if (m_bSaveImage) {
					vector<POINT> vtTemp;
					if (!ContoursConvertVector(psContours, vtTemp)) {
						m_strErrorMessage = "執行IC找邊流程失敗-4";
						return false;
					}
					Mat matDisplay = m_matGlue.clone();
					jet_imagefunction::DrawEdgePoint(matDisplay, vtTemp, m_scContours_Glue, 1);
					SaveImage(0, matDisplay, "IC_Contours_Src", m_nExtensionType);
				}

				// 依不同零件來定位
				switch(m_sParam_GlueHeight.nMeasuringObject){
				case 1:// IC
					if (!Rebuilding_ICEdge_Apple(psContours, m_sResult_GlueHeight.sIcEdge.vtptContoursPos)) {
						m_strErrorMessage = "IC邊界重建失敗";
						return false;
					}
					break;
				case 2:// Connector
					if (!Rebuilding_Connector_BaseEdge(psContours, m_sResult_GlueHeight.sIcEdge.vtptContoursPos)) {
						m_strErrorMessage = "Connector基線重建失敗";
						return false;
					}
					break;
				case 3:// 不計算斜率
					if (!Rebuilding_BaseEdge_ROI(psContours, m_sResult_GlueHeight.sIcEdge.vtptContoursPos)) {
						m_strErrorMessage = "ROI基線重建失敗";
						return false;
					}
					break;
				default:
					return false;
					m_strErrorMessage = "零件定位模式錯誤";
					break;
				}
				m_sResult_GlueHeight.sIcEdge.nContoursCount = m_sResult_GlueHeight.sIcEdge.vtptContoursPos.size();
			}
			else {
				// 輸出輪廓點
				if (!ContoursConvertVector(psContours, m_sResult_GlueHeight.sIcEdge.vtptContoursPos)) {
					m_strErrorMessage = "輸出原始IC邊界失敗";
					return false;
				}

				int nCount_Point = m_sResult_GlueHeight.sIcEdge.vtptContoursPos.size();
				if (nCount_Point <= 0) {
					m_strErrorMessage = "輸出原始IC邊界失敗";
					return false;
				}
				m_sResult_GlueHeight.sIcEdge.nContoursCount = nCount_Point;

				if (m_bSaveImage) {
					Mat matDisplay = m_matGlue.clone();
					jet_imagefunction::DrawEdgePoint(matDisplay, m_sResult_GlueHeight.sIcEdge.vtptContoursPos, m_scContours_Glue, 1);
					SaveImage(0, matDisplay, "IC_Contours_Src", m_nExtensionType);
				}
			}

			return true;
		}

		// 重建 IC邊界
		bool MeasurementBlackGlue::Rebuilding_ICEdge_Apple(SContoursParam* psContours, vector<POINT>& vtptContoursPos)
		{
			vtptContoursPos.clear();
			if (psContours == nullptr) {
				return false;
			}

			// 將輪廓拆成4條邊
			int nSearchRangeX = m_sParam_GlueHeight.nIC_BaseLine_SearchRange;
			int nSearchRangeY = m_sParam_GlueHeight.nIC_BaseLine_SearchRange;
			m_vt2ptICEdgePoint.clear();
			Rect cvROI;
			if (!ROI_ContoursToLine(nSearchRangeX, nSearchRangeY, 1, psContours, m_vt2ptICEdgePoint, cvROI)) {
				return false;
			}

			if (m_vt2ptICEdgePoint.size() != 4 || cvROI.x<0 || cvROI.y<0 || cvROI.width<=0 || cvROI.height<=0) {
				return false;
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				jet_imagefunction::DrawRectangle(matDisplay, cvROI, Scalar(255,255,255), 2, matDisplay);
				for (int k = 0; k < m_vt2ptICEdgePoint.size(); ++k) {
					Scalar csColor;
					switch (k) {
					case 0:
						csColor = Scalar(0, 0, 255);
						break;
					case 1:
						csColor = Scalar(0, 255, 0);
						break;
					case 2:
						csColor = Scalar(255, 0, 0);
						break;
					case 3:
						csColor = Scalar(0, 255, 255);
						break;
					}
					jet_imagefunction::DrawEdgePoint(matDisplay, m_vt2ptICEdgePoint[k], csColor, 1);
				}
				SaveImage(0, matDisplay, "IC_Edge", m_nExtensionType);
			}

			// 計算4條邊的斜率
			vector<double> vtdSlope(4, 0.0);
			vector<double> vtdIntercept(4, 0.0);
			for (int k = 0; k < 4; ++k) {
				if (m_vt2ptICEdgePoint[k].size() <= 0) {
					return false;
				}
				switch (k) {
				case 0:
				case 1:
					if (!jet_imagefunction::SimpleLinearRegr(m_vt2ptICEdgePoint[k], 10, 0.00001, vtdSlope[k], vtdIntercept[k])) {
						return false;
					}
					break;
				case 2:
				case 3:
					if (!jet_imagefunction::SimpleLinearRegr_Vertical(m_vt2ptICEdgePoint[k], 10, 0.00001, vtdSlope[k], vtdIntercept[k])) {
						return false;
					}
					break;
				}
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				Point2f ptfStart, ptfEnd;
				Scalar scColor;
				for (int k = 0; k < 4; ++k) {
					switch (m_sParam_GlueHeight.nMeasuringDirection) {
					case 1:
					case 3:
						ptfStart.x = 0;
						ptfStart.y = vtdSlope[k] * ptfStart.x + vtdIntercept[k];
						ptfEnd.x = m_matGlue.cols - 1;
						ptfEnd.y = vtdSlope[k] * ptfEnd.x + vtdIntercept[k];
						scColor = Scalar(0, 0, 255);
						break;
					case 2:
					case 4:
						ptfStart.y = 0;
						ptfStart.x = (ptfStart.y - vtdIntercept[k]) / vtdSlope[k];
						ptfEnd.y = m_matGlue.rows - 1;
						ptfEnd.x = (ptfEnd.y - vtdIntercept[k]) / vtdSlope[k];
						scColor = Scalar(0, 255, 0);
						break;
					}
					cv::line(matDisplay, ptfStart, ptfEnd, scColor, 1);
				}
				SaveImage(0, matDisplay, "IC_Line", m_nExtensionType);
			}

			// 計算目前邊的範圍
			Point2f cvCrossPoint1(-1.0f, -1.0f);
			Point2f cvCrossPoint2(-1.0f, -1.0f);
			switch (m_sParam_GlueHeight.nMeasuringDirection) {
			case 1: // 下
				// L
				if (jet_imagefunction::GetCrossPoint(vtdSlope[1], vtdIntercept[1], vtdSlope[2], vtdIntercept[2], cvCrossPoint1) != 1) {
					return false;
				}
				if (cvCrossPoint1.x < 0 || cvCrossPoint1.x >= m_matGlue.cols || std::isinf(cvCrossPoint1.x) || std::isnan(cvCrossPoint1.x) ||
					cvCrossPoint1.y < 0 || cvCrossPoint1.y >= m_matGlue.rows || std::isinf(cvCrossPoint1.y) || std::isnan(cvCrossPoint1.y)) {
					cvCrossPoint1.x = cvROI.x;
					cvCrossPoint1.y = vtdSlope[1] * cvCrossPoint1.x + vtdIntercept[1];
				}

				// R
				if (jet_imagefunction::GetCrossPoint(vtdSlope[1], vtdIntercept[1], vtdSlope[3], vtdIntercept[3], cvCrossPoint2) != 1) {
					return false;
				}
				if (cvCrossPoint2.x < 0 || cvCrossPoint2.x >= m_matGlue.cols || std::isinf(cvCrossPoint2.x) || std::isnan(cvCrossPoint2.x) ||
					cvCrossPoint2.y < 0 || cvCrossPoint2.y >= m_matGlue.rows || std::isinf(cvCrossPoint2.y) || std::isnan(cvCrossPoint2.y)) {
					cvCrossPoint2.x = cvROI.br().x - 1;
					cvCrossPoint2.y = vtdSlope[1] * cvCrossPoint1.x + vtdIntercept[1];
				}
				break;

			case 2: // 左
				// T
				if (jet_imagefunction::GetCrossPoint(vtdSlope[2], vtdIntercept[2], vtdSlope[0], vtdIntercept[0], cvCrossPoint1) != 1) {
					return false;
				}
				if (cvCrossPoint1.x < 0 || cvCrossPoint1.x >= m_matGlue.cols || std::isinf(cvCrossPoint1.x) || std::isnan(cvCrossPoint1.x) ||
					cvCrossPoint1.y < 0 || cvCrossPoint1.y >= m_matGlue.rows || std::isinf(cvCrossPoint1.y) || std::isnan(cvCrossPoint1.y)) {
					cvCrossPoint1.y = cvROI.y;
					cvCrossPoint1.x = (cvCrossPoint1.y - vtdIntercept[2])/ vtdSlope[2];
				}

				// B
				if (jet_imagefunction::GetCrossPoint(vtdSlope[2], vtdIntercept[2], vtdSlope[1], vtdIntercept[1], cvCrossPoint2) != 1) {
					return false;
				}
				if (cvCrossPoint2.x < 0 || cvCrossPoint2.x >= m_matGlue.cols || std::isinf(cvCrossPoint2.x) || std::isnan(cvCrossPoint2.x) ||
					cvCrossPoint2.y < 0 || cvCrossPoint2.y >= m_matGlue.rows || std::isinf(cvCrossPoint2.y) || std::isnan(cvCrossPoint2.y)) {
					cvCrossPoint1.y = cvROI.br().y - 1;
					cvCrossPoint1.x = (cvCrossPoint1.y - vtdIntercept[2]) / vtdSlope[2];
				}
				break;

			case 3: // 上
				// L
				if (jet_imagefunction::GetCrossPoint(vtdSlope[0], vtdIntercept[0], vtdSlope[2], vtdIntercept[2], cvCrossPoint1) != 1) {
					return false;
				}
				if (cvCrossPoint1.x < 0 || cvCrossPoint1.x >= m_matGlue.cols || std::isinf(cvCrossPoint1.x) || std::isnan(cvCrossPoint1.x) ||
					cvCrossPoint1.y < 0 || cvCrossPoint1.y >= m_matGlue.rows || std::isinf(cvCrossPoint1.y) || std::isnan(cvCrossPoint1.y)) {
					cvCrossPoint1.x = cvROI.x;
					cvCrossPoint1.y = vtdSlope[0] * cvCrossPoint1.x + vtdIntercept[0];
				}

				// R
				if (jet_imagefunction::GetCrossPoint(vtdSlope[0], vtdIntercept[0], vtdSlope[3], vtdIntercept[3], cvCrossPoint2) != 1) {
					return false;
				}
				if (cvCrossPoint2.x < 0 || cvCrossPoint2.x >= m_matGlue.cols || std::isinf(cvCrossPoint2.x) || std::isnan(cvCrossPoint2.x) ||
					cvCrossPoint2.y < 0 || cvCrossPoint2.y >= m_matGlue.rows || std::isinf(cvCrossPoint2.y) || std::isnan(cvCrossPoint2.y)) {
					cvCrossPoint2.x = cvROI.br().x - 1;
					cvCrossPoint2.y = vtdSlope[0] * cvCrossPoint1.x + vtdIntercept[0];
				}
				break;

			case 4: // 右
				// T
				if (jet_imagefunction::GetCrossPoint(vtdSlope[3], vtdIntercept[3], vtdSlope[0], vtdIntercept[0], cvCrossPoint1) != 1) {
					return false;
				}
				if (cvCrossPoint1.x < 0 || cvCrossPoint1.x >= m_matGlue.cols || std::isinf(cvCrossPoint1.x) || std::isnan(cvCrossPoint1.x) ||
					cvCrossPoint1.y < 0 || cvCrossPoint1.y >= m_matGlue.rows || std::isinf(cvCrossPoint1.y) || std::isnan(cvCrossPoint1.y)) {
					cvCrossPoint1.y = cvROI.y;
					cvCrossPoint1.x = (cvCrossPoint1.y - vtdIntercept[3]) / vtdSlope[3];
				}

				// B
				if (jet_imagefunction::GetCrossPoint(vtdSlope[3], vtdIntercept[3], vtdSlope[1], vtdIntercept[1], cvCrossPoint2) != 1) {
					return false;
				}
				if (cvCrossPoint2.x < 0 || cvCrossPoint2.x >= m_matGlue.cols || std::isinf(cvCrossPoint2.x) || std::isnan(cvCrossPoint2.x) ||
					cvCrossPoint2.y < 0 || cvCrossPoint2.y >= m_matGlue.rows || std::isinf(cvCrossPoint2.y) || std::isnan(cvCrossPoint2.y)) {
					cvCrossPoint1.y = cvROI.br().y - 1;
					cvCrossPoint1.x = (cvCrossPoint1.y - vtdIntercept[3]) / vtdSlope[3];
				}
				break;
			}

			if (cvCrossPoint1.x < 0 || cvCrossPoint1.x >= m_matGlue.cols || std::isinf(cvCrossPoint1.x) || std::isnan(cvCrossPoint1.x) ||
				cvCrossPoint1.y < 0 || cvCrossPoint1.y >= m_matGlue.rows || std::isinf(cvCrossPoint1.y) || std::isnan(cvCrossPoint1.y) ||
				cvCrossPoint2.x < 0 || cvCrossPoint2.x >= m_matGlue.cols || std::isinf(cvCrossPoint2.x) || std::isnan(cvCrossPoint2.x) ||
				cvCrossPoint2.y < 0 || cvCrossPoint2.y >= m_matGlue.rows || std::isinf(cvCrossPoint2.y) || std::isnan(cvCrossPoint2.y)) {
				return false;
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				Point2f ptfStart, ptfEnd;
				Scalar scColor;
				for (int k = 0; k < 4; ++k) {
					switch (m_sParam_GlueHeight.nMeasuringDirection) {
					case 1:
					case 3:
						ptfStart.x = 0;
						ptfStart.y = vtdSlope[k] * ptfStart.x + vtdIntercept[k];
						ptfEnd.x = m_matGlue.cols - 1;
						ptfEnd.y = vtdSlope[k] * ptfEnd.x + vtdIntercept[k];
						scColor = Scalar(0, 0, 255);
						break;
					case 2:
					case 4:
						ptfStart.y = 0;
						ptfStart.x = (ptfStart.y - vtdIntercept[k]) / vtdSlope[k];
						ptfEnd.y = m_matGlue.rows - 1;
						ptfEnd.x = (ptfEnd.y - vtdIntercept[k]) / vtdSlope[k];
						scColor = Scalar(0, 255, 0);
						break;
					}
					cv::line(matDisplay, ptfStart, ptfEnd, scColor, 1);
				}

				jet_imagefunction::DrawCrossLine(cvCrossPoint1, 10, 2, Scalar(0, 255, 0), matDisplay);
				jet_imagefunction::DrawCrossLine(cvCrossPoint2, 10, 2, Scalar(0, 0, 255), matDisplay);
				SaveImage(0, matDisplay, "IC_Line_Range", m_nExtensionType);
			}

			// 輸出目標邊界的每一點
			POINT ptStart, ptEnd;
			ptStart.x = cvCrossPoint1.x + 0.5;
			ptStart.y = cvCrossPoint1.y + 0.5;
			ptEnd.x = cvCrossPoint2.x + 0.5;
			ptEnd.y = cvCrossPoint2.y + 0.5;
			vector<POINT> vtptTempPos;
			if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, ptStart, ptEnd, vtptTempPos) != 1) {
				return false;
			}

			int nSumPoint = vtptTempPos.size();
			int nShift_Left_Top = m_sParam_GlueHeight.nICEdge_Indent_Left_Top;
			if (nShift_Left_Top < 0) nShift_Left_Top = 0;
			int nShift_Right_Bottom = m_sParam_GlueHeight.nICEdge_Indent_Right_Bottom;
			if (nShift_Right_Bottom < 0) nShift_Right_Bottom = 0;
			int nNumber = nSumPoint - (nShift_Left_Top + nShift_Right_Bottom);
			if (nNumber <= 0) return false;

			vtptContoursPos.resize(nNumber);
			std::copy(vtptTempPos.begin() + nShift_Left_Top, vtptTempPos.end() - nShift_Right_Bottom, vtptContoursPos.begin());

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				jet_imagefunction::DrawEdgePoint(matDisplay, vtptContoursPos, m_scContours_Glue, 1);
				SaveImage(0, matDisplay, "IC_Contours_Final", m_nExtensionType);
			}

			return true;
		}

		// 20251015 重建 Connector 基線邊界
		bool MeasurementBlackGlue::Rebuilding_Connector_BaseEdge(SContoursParam* psContours, vector<POINT>& vtptContoursPos)
		{
			vtptContoursPos.clear();
			if (psContours == nullptr) {
				return false;
			}

			// 將輪廓拆成4條邊
			Rect cvROI;
			int nSearchRangeX = m_sParam_GlueHeight.nIC_BaseLine_SearchRange;
			int nSearchRangeY = m_sParam_GlueHeight.nIC_BaseLine_SearchRange;
			m_vt2ptICEdgePoint.clear();
			if (!ROI_ContoursToLine_Connector(nSearchRangeX, nSearchRangeY, 3, psContours, m_vt2ptICEdgePoint, cvROI)) {
				return false;
			}

			if (m_vt2ptICEdgePoint.size() != 4) {
				return false;
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				jet_imagefunction::DrawRectangle(matDisplay, cvROI, Scalar(255, 255, 255), 2, matDisplay);
				for (int k = 0; k < m_vt2ptICEdgePoint.size(); ++k) {
					Scalar csColor;
					switch (k) {
					case 0:
						csColor = Scalar(0, 0, 255);
						break;
					case 1:
						csColor = Scalar(0, 255, 0);
						break;
					case 2:
						csColor = Scalar(255, 0, 0);
						break;
					case 3:
						csColor = Scalar(0, 255, 255);
						break;
					}
					jet_imagefunction::DrawEdgePoint(matDisplay, m_vt2ptICEdgePoint[k], csColor, 1);
				}
				SaveImage(0, matDisplay, "Connector_Edge", m_nExtensionType);
			}

			// 計算基線斜率
			int nIndex = -1;
			switch (m_sParam_GlueHeight.nMeasuringDirection) {
			case 1: // 下
				nIndex = 1;
				break;
			case 2: // 左
				nIndex = 2;
				break;
			case 3:// 右
				nIndex = 0;
				break;
			case 4:// 上
				nIndex = 3;
				break;
			}

			double dSlope = 0.0;
			double dIntercept = 0.0;
			if (!jet_imagefunction::SimpleLinearRegr(m_vt2ptICEdgePoint[nIndex], 10, 0.00001, dSlope, dIntercept)) {
				return false;
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				Point2f ptfStart, ptfEnd;
				Scalar scColor;
				switch (m_sParam_GlueHeight.nMeasuringDirection) {
				case 1:
				case 3:
					ptfStart.x = 0;
					ptfStart.y = dSlope * ptfStart.x + dIntercept;
					ptfEnd.x = m_matGlue.cols - 1;
					ptfEnd.y = dSlope * ptfEnd.x + dIntercept;
					scColor = Scalar(0, 0, 255);
					break;
				case 2:
				case 4:
					ptfStart.y = 0;
					ptfStart.x = (ptfStart.y - dIntercept) / dSlope;
					ptfEnd.y = m_matGlue.rows - 1;
					ptfEnd.x = (ptfEnd.y - dIntercept) / dSlope;
					scColor = Scalar(0, 255, 0);
					break;
				}
				jet_imagefunction::DrawRectangle(matDisplay, cvROI, Scalar(255, 255, 255), 2, matDisplay);
				cv::line(matDisplay, ptfStart, ptfEnd, scColor, 1);
				jet_imagefunction::DrawCrossLine(ptfStart, 10, 2, Scalar(0, 255, 0), matDisplay);
				jet_imagefunction::DrawCrossLine(ptfEnd, 10, 2, Scalar(0, 0, 255), matDisplay);
				SaveImage(0, matDisplay, "Connector_BaseLine", m_nExtensionType);
			}

			int nShift_Left_Top = m_sParam_GlueHeight.nICEdge_Indent_Left_Top;
			if (nShift_Left_Top < 0) nShift_Left_Top = 0;
			int nShift_Right_Bottom = m_sParam_GlueHeight.nICEdge_Indent_Right_Bottom;
			if (nShift_Right_Bottom < 0) nShift_Right_Bottom = 0;
			int nSumShift = nShift_Left_Top + nShift_Right_Bottom;
			// 計算目前邊的範圍
			Point2f ptfStart, ptfEnd;
			switch (m_sParam_GlueHeight.nMeasuringDirection) {
			case 1: // 下
			case 3: // 上
				if (cvROI.width < 2 * nSumShift) {
					nShift_Left_Top = 0;
					nShift_Right_Bottom = 0;
				}
				ptfStart.x = cvROI.x + nShift_Left_Top;
				ptfStart.y = dSlope*ptfStart.x + dIntercept;
				ptfEnd.x = cvROI.br().x - nShift_Right_Bottom;
				ptfEnd.y = dSlope*ptfEnd.x + dIntercept;
				
				break;
			case 2: // 左
			case 4: // 右
				if (cvROI.height < 2 * nSumShift) {
					nShift_Left_Top = 0;
					nShift_Right_Bottom = 0;
				}
				ptfStart.y = cvROI.y + nShift_Left_Top;
				ptfStart.x = (ptfStart.y - dIntercept) / dSlope;
				ptfEnd.y = cvROI.br().y - nShift_Right_Bottom;
				ptfEnd.x = (ptfEnd.y - dIntercept) / dSlope;
				break;
			}
	
			POINT ptStart;
			ptStart.x = ptfStart.x + 0.5;
			ptStart.y = ptfStart.y + 0.5;
			if (ptStart.x < 0) {
				ptStart.x = 0;
			}

			if (ptStart.y < 0) {
				ptStart.y = 0;
			}

			POINT ptEnd;
			ptEnd.x = ptfEnd.x + 0.5;
			ptEnd.y = ptfEnd.y + 0.5;
			if (ptEnd.x >= m_matGlue.cols) {
				ptEnd.x = m_matGlue.cols - 1;
			}

			if (ptEnd.y >= m_matGlue.rows) {
				ptEnd.y = m_matGlue.rows - 1;
			}

			if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, ptStart, ptEnd, vtptContoursPos) != 1) {
				return false;
			}
			
			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				jet_imagefunction::DrawEdgePoint(matDisplay, vtptContoursPos, m_scContours_Glue, 1);
				SaveImage(0, matDisplay, "Connector_BaseLine_Final", m_nExtensionType);
			}

			return true;
		}

		// 20251015 重建 基線邊界---直接用外接矩形, 不使用線性回歸
		bool MeasurementBlackGlue::Rebuilding_BaseEdge_ROI(SContoursParam* psContours, vector<POINT>& vtptContoursPos)
		{
			vtptContoursPos.clear();
			if (psContours == nullptr) {
				return false;
			}

			// 將輪廓拆成4條邊
			Rect cvROI;
			int nSearchRangeX = m_sParam_GlueHeight.nIC_BaseLine_SearchRange;
			int nSearchRangeY = m_sParam_GlueHeight.nIC_BaseLine_SearchRange;
			m_vt2ptICEdgePoint.clear();
			if (!ROI_ContoursToLine_Connector(nSearchRangeX, nSearchRangeY, 3, psContours, m_vt2ptICEdgePoint, cvROI)) {
				return false;
			}

			if (m_vt2ptICEdgePoint.size() != 4) {
				return false;
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				jet_imagefunction::DrawRectangle(matDisplay, cvROI, Scalar(255, 255, 255), 2, matDisplay);
				for (int k = 0; k < m_vt2ptICEdgePoint.size(); ++k) {
					Scalar csColor;
					switch (k) {
					case 0:
						csColor = Scalar(0, 0, 255);
						break;
					case 1:
						csColor = Scalar(0, 255, 0);
						break;
					case 2:
						csColor = Scalar(255, 0, 0);
						break;
					case 3:
						csColor = Scalar(0, 255, 255);
						break;
					}
					jet_imagefunction::DrawEdgePoint(matDisplay, m_vt2ptICEdgePoint[k], csColor, 1);
				}
				SaveImage(0, matDisplay, "ROI_Edge", m_nExtensionType);
			}


			int nShift_Left_Top = m_sParam_GlueHeight.nICEdge_Indent_Left_Top;
			if (nShift_Left_Top < 0) nShift_Left_Top = 0;
			int nShift_Right_Bottom = m_sParam_GlueHeight.nICEdge_Indent_Right_Bottom;
			if (nShift_Right_Bottom < 0) nShift_Right_Bottom = 0;
			int nSumShift = nShift_Left_Top + nShift_Right_Bottom;

			// 計算目前邊的範圍
			POINT ptStart, ptEnd;
			switch (m_sParam_GlueHeight.nMeasuringDirection) {
			case 1: // 下
			case 3: // 上
				if (cvROI.width < 2 * nSumShift) {
					nShift_Left_Top = 0;
					nShift_Right_Bottom = 0;
				}
				ptStart.x = cvROI.x + nShift_Left_Top;
				ptEnd.x = cvROI.br().x - nShift_Right_Bottom - 1;
				if (m_sParam_GlueHeight.nMeasuringDirection == 1) {
					ptStart.y = cvROI.br().y - 1;
					ptEnd.y = cvROI.br().y - 1;
				}
				else {
					ptStart.y = cvROI.y;
					ptEnd.y = cvROI.y;
				}

				break;
			case 2: // 左
			case 4: // 右
				if (cvROI.height < 2 * nSumShift) {
					nShift_Left_Top = 0;
					nShift_Right_Bottom = 0;
				}
				ptStart.y = cvROI.y + nShift_Left_Top;
				ptEnd.y = cvROI.br().y - nShift_Right_Bottom - 1;
				if (m_sParam_GlueHeight.nMeasuringDirection == 2) {
					ptStart.x = cvROI.x;
					ptEnd.x = cvROI.x;
				}
				else {
					ptStart.x = cvROI.br().x - 1;
					ptEnd.x = cvROI.br().x - 1;
				}
				break;
			}

			if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, ptStart, ptEnd, vtptContoursPos) != 1) {
				return false;
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				jet_imagefunction::DrawEdgePoint(matDisplay, vtptContoursPos, m_scContours_Glue, 1);
				SaveImage(0, matDisplay, "ROI_BaseLine_Final", m_nExtensionType);
			}

			return true;
		}

		// 計算 IC邊 到 膠邊的距離
		bool MeasurementBlackGlue::Calculate_IcToGlue()
		{
			int nSumCount = m_sResult_GlueHeight.sIcEdge.nContoursCount;
			if (m_matEdgeMask.empty() || nSumCount <= 0 || m_sParam_GlueHeight.nBlockCount < 1) {
				m_strErrorMessage = "Calculate_IcToGlue 1";
				return false;
			}

			if (m_bSaveImage) {
				Mat matEdge = m_matEdgeMask.clone();
				for (int k = 0; k < nSumCount; ++k) {
					uchar* pu8Edge = matEdge.ptr<uchar>(m_sResult_GlueHeight.sIcEdge.vtptContoursPos[k].y);
					pu8Edge[m_sResult_GlueHeight.sIcEdge.vtptContoursPos[k].x] = 128;
				}
				SaveImage(0, matEdge, "Measurement_Edge", 2);
			}

			m_sResult_GlueHeight.sHeight.nBlockCount = m_sParam_GlueHeight.nBlockCount;
			m_sResult_GlueHeight.sHeight.vtsBlock.resize(m_sParam_GlueHeight.nBlockCount);
			int nInterval = nSumCount / m_sResult_GlueHeight.sHeight.nBlockCount;
			if (nInterval < 1) {
				m_strErrorMessage = "Calculate_IcToGlue 2";
				return false;
			}

			int nShift = 0;
			int nId = 0;
			for (int n = 0; n < m_sResult_GlueHeight.sHeight.nBlockCount; ++n) {
				Block_MeasurementInfo& sBlockInfo = m_sResult_GlueHeight.sHeight.vtsBlock[n].sInfo;

				// 建置每個區塊大小
				if (n == m_sResult_GlueHeight.sHeight.nBlockCount - 1) {
					sBlockInfo.nPixelsCount = nSumCount - (n*nInterval);
				}
				else {
					sBlockInfo.nPixelsCount = nInterval;
				}

				if (sBlockInfo.nPixelsCount < 1) {
					m_strErrorMessage = "Calculate_IcToGlue 3";
					return false;
				}
				sBlockInfo.vtsPixels.resize(sBlockInfo.nPixelsCount);

				// 計算每個區塊 IC邊 到膠邊的距離
				if (nId >= nSumCount) {
					n = m_sResult_GlueHeight.sHeight.nBlockCount;
					continue;
				}

				for (int p = 0; p < sBlockInfo.nPixelsCount; ++p) {
					POINT ptIC = m_sResult_GlueHeight.sIcEdge.vtptContoursPos[nId];
					if (ptIC.x < 0 || ptIC.y < 0 ) {
						m_strErrorMessage = "Calculate_IcToGlue 4";
						return false;
					}

					if (ptIC.x >= m_matEdgeMask.cols || ptIC.y >= m_matEdgeMask.rows) {
						m_strErrorMessage = "Calculate_IcToGlue 5";
						return false;
					}
					sBlockInfo.vtsPixels[p].ptStart = ptIC;
					bool bEdge = false;
					switch (m_sParam_GlueHeight.nMeasuringDirection)
					{
					case 1: // 下
						for (int y = ptIC.y- nShift; y < m_matEdgeMask.rows; ++y) {
							if (y < 0) continue;
							uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(y);
							if (pu8Edge[ptIC.x] == 255) {
								bEdge = true;
								sBlockInfo.vtsPixels[p].ptEnd.x = ptIC.x;
								sBlockInfo.vtsPixels[p].ptEnd.y = y;
								Calculate_Distance(m_sParam_GlueHeight.fResolutionX, m_sParam_GlueHeight.fResolutionY, sBlockInfo.vtsPixels[p]);
								y = m_matEdgeMask.rows;
							}
						}
						if (!bEdge) {
							sBlockInfo.vtsPixels[p].ptEnd.x = ptIC.x;
							sBlockInfo.vtsPixels[p].ptEnd.y = ptIC.y + m_sParam_GlueHeight.fIC_Height/ m_sParam_GlueHeight.fResolutionY;
							Calculate_Distance(m_sParam_GlueHeight.fResolutionX, m_sParam_GlueHeight.fResolutionY, sBlockInfo.vtsPixels[p]);
						}
						break;
					case 2: // 左
						{
							uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(ptIC.y);
							for (int x = ptIC.x+ nShift; x >= 0; --x) {
								if (x >= m_matEdgeMask.cols) continue;
								if (pu8Edge[x] == 255) {
									bEdge = true;
									sBlockInfo.vtsPixels[p].ptEnd.y = ptIC.y;
									sBlockInfo.vtsPixels[p].ptEnd.x = x;
									Calculate_Distance(m_sParam_GlueHeight.fResolutionX, m_sParam_GlueHeight.fResolutionY, sBlockInfo.vtsPixels[p]);
									x = -1;
								}
							}
							if (!bEdge) {
								sBlockInfo.vtsPixels[p].ptEnd.x = ptIC.x - m_sParam_GlueHeight.fIC_Height / m_sParam_GlueHeight.fResolutionX;
								sBlockInfo.vtsPixels[p].ptEnd.y = ptIC.y;
								Calculate_Distance(m_sParam_GlueHeight.fResolutionX, m_sParam_GlueHeight.fResolutionY, sBlockInfo.vtsPixels[p]);
							}
						}
						break;
					case 3: // 上
						for (int y = ptIC.y+ nShift; y >= 0; --y) {
							if (y >= m_matEdgeMask.rows) continue;
							uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(y);
							if (pu8Edge[ptIC.x] == 255) {
								bEdge = true;
								sBlockInfo.vtsPixels[p].ptEnd.x = ptIC.x;
								sBlockInfo.vtsPixels[p].ptEnd.y = y;
								Calculate_Distance(m_sParam_GlueHeight.fResolutionX, m_sParam_GlueHeight.fResolutionY, sBlockInfo.vtsPixels[p]);
								y = -1;
							}
						}
						if (!bEdge) {
							sBlockInfo.vtsPixels[p].ptEnd.x = ptIC.x;
							sBlockInfo.vtsPixels[p].ptEnd.y = ptIC.y - m_sParam_GlueHeight.fIC_Height / m_sParam_GlueHeight.fResolutionY;
							Calculate_Distance(m_sParam_GlueHeight.fResolutionX, m_sParam_GlueHeight.fResolutionY, sBlockInfo.vtsPixels[p]);
						}
						break;
					case 4: // 右
						{
							uchar* pu8Edge = m_matEdgeMask.ptr<uchar>(ptIC.y);
							for (int x = ptIC.x- nShift; x < m_matEdgeMask.cols; ++x) {
								if (x < 0) continue;
								if (pu8Edge[x] == 255) {
									bEdge = true;
									sBlockInfo.vtsPixels[p].ptEnd.y = ptIC.y;
									sBlockInfo.vtsPixels[p].ptEnd.x = x;
									Calculate_Distance(m_sParam_GlueHeight.fResolutionX, m_sParam_GlueHeight.fResolutionY, sBlockInfo.vtsPixels[p]);
									x = m_matEdgeMask.cols;
								}
							}
							if (!bEdge) {
								sBlockInfo.vtsPixels[p].ptEnd.x = ptIC.x + m_sParam_GlueHeight.fIC_Height / m_sParam_GlueHeight.fResolutionX;
								sBlockInfo.vtsPixels[p].ptEnd.y = ptIC.y;
								Calculate_Distance(m_sParam_GlueHeight.fResolutionX, m_sParam_GlueHeight.fResolutionY, sBlockInfo.vtsPixels[p]);
							}
						}
						break;
					}
					++nId;
				}
			}

			return true;
		}

		// 計算 膠高 與 高度比例
		bool MeasurementBlackGlue::Calculate_GluetHeightRatio()
		{
			int nBlockCount = m_sResult_GlueHeight.sHeight.nBlockCount;
			if (nBlockCount < 1 || m_sParam_GlueHeight.fIC_Height <= 0.0) {
				return false;
			}

			for (int k = 0; k < nBlockCount; ++k) {
				Block_MeasurementInfo& sBlockInfo = m_sResult_GlueHeight.sHeight.vtsBlock[k].sInfo;
				for (int p = 0; p < sBlockInfo.nPixelsCount; ++p) {
					float fGlueHeight = m_sParam_GlueHeight.fIC_Height - sBlockInfo.vtsPixels[p].fDist_ICtoGlue;
					sBlockInfo.vtsPixels[p].fGlueHeight = (fGlueHeight >= 0.0) ? fGlueHeight : 0.0;
					sBlockInfo.vtsPixels[p].fHeightRatio = sBlockInfo.vtsPixels[p].fGlueHeight*100.0f / m_sParam_GlueHeight.fIC_Height;
				}
			}

			return true;
		}

		// nMode : 高度模式, 1=>最高, 2=>平均, 3=>最低, 4=>最高-最低, 5=>平均-最低
		// 計算每個區塊的輸出結果
		bool MeasurementBlackGlue::OutputHeightRatio()
		{
			int nBlockCount = m_sResult_GlueHeight.sHeight.nBlockCount;
			if (nBlockCount < 1) {
				return false;
			}

			// 計算各個區塊的結果
			bool bOk = false;
			for (int k = 0; k < nBlockCount; ++k) {
				switch (m_sParam_GlueHeight.vtnHeightMode[k]) {
				case 1:
					bOk = OutputDistance_Max(m_sResult_GlueHeight.sHeight.vtsBlock[k]);
					break;
				case 2:
					bOk = OutputDistance_Mean(m_sResult_GlueHeight.sHeight.vtsBlock[k]);
					break;
				case 3:
					bOk = OutputDistance_Min(m_sResult_GlueHeight.sHeight.vtsBlock[k]);
					break;
				case 4:
					bOk = OutputDistance_MaxMin(m_sResult_GlueHeight.sHeight.vtsBlock[k]);
					break;
				case 5:
					bOk = OutputDistance_MeanMin(m_sResult_GlueHeight.sHeight.vtsBlock[k]);
					break;
				}
			}

			// 統計多區塊的結果
			if (bOk) {
				switch (m_sParam_GlueHeight.nFinal_HeightMode) {
				case 1:
					bOk = OutputDistance_Max(m_sResult_GlueHeight.sHeight);
					break;
				case 2:
					bOk = OutputDistance_Mean(m_sResult_GlueHeight.sHeight);
					break;
				case 3:
					bOk = OutputDistance_Min(m_sResult_GlueHeight.sHeight);
					break;
				case 4:
					bOk = OutputDistance_MaxMin(m_sResult_GlueHeight.sHeight);
					break;
				case 5:
					bOk = OutputDistance_MeanMin(m_sResult_GlueHeight.sHeight);
					break;
				}
			}

			if (bOk) {
				if (m_sParam_GlueHeight.sLimit.fLower <= m_sResult_GlueHeight.sHeight.fFinalHeightRatio &&
					m_sResult_GlueHeight.sHeight.fFinalHeightRatio <= m_sParam_GlueHeight.sLimit.fUpper) {
					m_sResult_GlueHeight.sHeight.bFinalResult = true;
				}
				else {
					m_sResult_GlueHeight.sHeight.bFinalResult = false;
				}
			}

			return bOk;
		}
#pragma endregion

#pragma region Glue Area Ratio

		// 找IC範圍
		bool MeasurementBlackGlue::RunProcess_FindIcRange()
		{
			if (!SetFindEdgeProcess(m_sParam_GlueArea.sIcEdge, m_sPM_FindBoard)) {
				return false;
			}

			vector<Mat> vtmatImage(1);
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			vtmatImage[0] = m_matGlue;
			if (!jet_imagefunction::RunProcessMode(vtmatImage, m_sPM_FindBoard, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 0) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName, "Error_FindIcRange", m_sPM_FindBoard, vtmatImage, vtsResult, 2);
				}
				return false;
			}

			int nNumber = m_sPM_FindBoard.GetBaseParameterNumber();
			if (nNumber != vtsResult.size()) {
				m_strErrorMessage = "RunProcess_FindIcRange is error";
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName, "OK_FindIcRange", m_sPM_FindBoard, vtmatImage, vtsResult, 2);
			}

			SContoursParam* psContours = &vtsResult[nNumber - 1].sContours;
			if (psContours == nullptr) {
				m_strErrorMessage = "RunProcess_FindIcRange is error";
				return false;
			}

			// 重建 IC的 4個邊
			int nIC_Indent_Width_Pixels = m_sParam_GlueArea.nIC_Indent_Width / m_sParam_GlueArea.fResolutionX;
			int nIC_Indent_Height_Pixels = m_sParam_GlueArea.nIC_Indent_Height / m_sParam_GlueArea.fResolutionY;
			if (!Rebuilding_IC_Range(nIC_Indent_Width_Pixels, nIC_Indent_Height_Pixels, psContours, m_sResult_GlueArea.sIcRange.vt2ptContoursPos, m_sResult_GlueArea.sIcRange.vtptCorners)) {
				m_strErrorMessage = "Rebuilding_IC_Range is error";
				return false;
			}

			if (m_sResult_GlueArea.sIcRange.vt2ptContoursPos.size() != 4 || m_sResult_GlueArea.sIcRange.vtptCorners.size() != 4) {
				m_strErrorMessage = "Rebuilding_IC_Range is error";
				return false;
			}

			m_sResult_GlueArea.sIcRange.vtnContoursCount.resize(4, 0);
			for (int k = 0; k < 4; ++k) {
				m_sResult_GlueArea.sIcRange.vtnContoursCount[k] = m_sResult_GlueArea.sIcRange.vt2ptContoursPos[k].size();
				if (m_sResult_GlueArea.sIcRange.vtnContoursCount[k] <= 0) {
					m_strErrorMessage = "Rebuilding_IC_Range is error";
					return false;
				}
			}

			// 建立 IC範圍遮罩 m_matEdgeMask
			if (!jet_imagefunction::DisplayEdge(m_sResult_GlueArea.sIcRange.vt2ptContoursPos, m_matGlue.cols, m_matGlue.rows, m_matEdgeMask)) {
				m_strErrorMessage = "Rebuilding_IC_Range is error";
				return false;
			}

			SaveImage(0, m_matEdgeMask, "IC_Mask-1", m_nExtensionType);

			if (!jet_imagefunction::FillEdge(m_matEdgeMask)) {
				m_strErrorMessage = "Rebuilding_IC_Range is error";
				return false;
			}

			SaveImage(0, m_matEdgeMask, "IC_Mask-2", m_nExtensionType);

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				int nC = matDisplay.channels();			
				for (int y = 0; y < matDisplay.rows; ++y) {
					uchar* pu8Display = matDisplay.ptr<uchar>(y);
					uchar* pu8Mask = m_matEdgeMask.ptr<uchar>(y);
					for (int x = 0; x < matDisplay.cols*nC; x+= nC) {
						if (pu8Mask[x/ nC] == 0) {
							for (int c = 0; c < nC; ++c) {
								pu8Display[x + c] = 0;
							}
						}
					}
				}
				SaveImage(0, matDisplay, "IC_Mask-3", m_nExtensionType);
			}

			return true;
		}

		// 重建 IC範圍 (4個邊都要重建)
		// vt2ptContoursPos : [0]=上, [1]=下, [2]=左, [3]=右
		bool MeasurementBlackGlue::Rebuilding_IC_Range(const int nIndent_W, const int nIndent_H, SContoursParam* psContours, vector<vector<POINT>>& vt2ptContoursPos, vector<POINT>& vtptCorners)
		{
			vt2ptContoursPos.clear();
			vtptCorners.clear();
			if (nIndent_W<0 || nIndent_H<0 || psContours == nullptr) {
				return false;
			}

			// 將輪廓拆成4條邊
			int nSearchRangeX = 20;
			int nSearchRangeY = 20;
			m_vt2ptICEdgePoint.clear();
			Rect cvROI;
			if (!ROI_ContoursToLine(nSearchRangeX, nSearchRangeY, 5, psContours, m_vt2ptICEdgePoint, cvROI)) {
				return false;
			}

			if (m_vt2ptICEdgePoint.size() != 4) {
				return false;
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				for (int k = 0; k < m_vt2ptICEdgePoint.size(); ++k) {
					Scalar csColor;
					switch (k) {
					case 0:
						csColor = Scalar(0, 0, 255);
						break;
					case 1:
						csColor = Scalar(0, 255, 0);
						break;
					case 2:
						csColor = Scalar(255, 0, 0);
						break;
					case 3:
						csColor = Scalar(0, 255, 255);
						break;
					}
					jet_imagefunction::DrawEdgePoint(matDisplay, m_vt2ptICEdgePoint[k], csColor, 1);
				}
				SaveImage(0, matDisplay, "IC_Edge", m_nExtensionType);
			}

			// 計算4條邊的斜率
			vector<double> vtdSlope(4, 0.0);
			vector<double> vtdIntercept(4, 0.0);
			for (int k = 0; k < 4; ++k) {
				if (m_vt2ptICEdgePoint[k].size() <= 0) {
					return false;
				}
				switch (k) {
				case 0:
				case 1:
					if (!jet_imagefunction::SimpleLinearRegr(m_vt2ptICEdgePoint[k], 10, 0.00001, vtdSlope[k], vtdIntercept[k])) {
						return false;
					}
					break;
				case 2:
				case 3:
					if (!jet_imagefunction::SimpleLinearRegr_Vertical(m_vt2ptICEdgePoint[k], 10, 0.00001, vtdSlope[k], vtdIntercept[k])) {
						return false;
					}
					break;
				}
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				Point2f ptfStart, ptfEnd;
				Scalar scColor;
				for (int k = 0; k < 4; ++k) {
					switch (k) {
					case 0:
					case 2:
						ptfStart.x = 0;
						ptfStart.y = vtdSlope[k] * ptfStart.x + vtdIntercept[k];
						ptfEnd.x = m_matGlue.cols - 1;
						ptfEnd.y = vtdSlope[k] * ptfEnd.x + vtdIntercept[k];
						if (k == 0) scColor = Scalar(0, 0, 255);
						else		scColor = Scalar(255, 0, 0);
						break;
					case 1:
					case 3:
						ptfStart.y = 0;
						ptfStart.x = (ptfStart.y - vtdIntercept[k]) / vtdSlope[k];
						ptfEnd.y = m_matGlue.rows - 1;
						ptfEnd.x = (ptfEnd.y - vtdIntercept[k]) / vtdSlope[k];
						if (k == 1) scColor = Scalar(0, 255, 0);
						else		scColor = Scalar(0, 255, 255);
						break;
					}
					cv::line(matDisplay, ptfStart, ptfEnd, scColor, 1);
				}
				SaveImage(0, matDisplay, "IC_Line", m_nExtensionType);
			}

			
			Point2f cvCrossPoint_LT(-1.0f, -1.0f);
			if (jet_imagefunction::GetCrossPoint(vtdSlope[0], vtdIntercept[0], vtdSlope[2], vtdIntercept[2], cvCrossPoint_LT) != 1) {
				return false;
			}

			if (cvCrossPoint_LT.x < 0 || cvCrossPoint_LT.y < 0 || cvCrossPoint_LT.x >= m_matGlue.cols || cvCrossPoint_LT.y >= m_matGlue.rows) {
				return false;
			}

			Point2f cvCrossPoint_RT(-1.0f, -1.0f);
			if (jet_imagefunction::GetCrossPoint(vtdSlope[0], vtdIntercept[0], vtdSlope[3], vtdIntercept[3], cvCrossPoint_RT) != 1) {
				return false;
			}

			if (cvCrossPoint_RT.x < 0 || cvCrossPoint_RT.y < 0 || cvCrossPoint_RT.x >= m_matGlue.cols || cvCrossPoint_RT.y >= m_matGlue.rows) {
				return false;
			}

			Point2f cvCrossPoint_RB(-1.0f, -1.0f);
			if (jet_imagefunction::GetCrossPoint(vtdSlope[1], vtdIntercept[1], vtdSlope[3], vtdIntercept[3], cvCrossPoint_RB) != 1) {
				return false;
			}

			if (cvCrossPoint_RB.x < 0 || cvCrossPoint_RB.y < 0 || cvCrossPoint_RB.x >= m_matGlue.cols || cvCrossPoint_RB.y >= m_matGlue.rows) {
				return false;
			}

			Point2f cvCrossPoint_LB(-1.0f, -1.0f);
			if (jet_imagefunction::GetCrossPoint(vtdSlope[1], vtdIntercept[1], vtdSlope[2], vtdIntercept[2], cvCrossPoint_LB) != 1) {
				return false;
			}

			if (cvCrossPoint_LB.x < 0 || cvCrossPoint_LB.y < 0 || cvCrossPoint_LB.x >= m_matGlue.cols || cvCrossPoint_LB.y >= m_matGlue.rows) {
				return false;
			}

			vtptCorners.resize(4);
			vtptCorners[0].x = cvCrossPoint_LT.x + nIndent_W + 0.5;
			vtptCorners[0].y = cvCrossPoint_LT.y + nIndent_H + 0.5;
			vtptCorners[1].x = cvCrossPoint_RT.x - nIndent_W + 0.5;
			vtptCorners[1].y = cvCrossPoint_RT.y + nIndent_H + 0.5;
			vtptCorners[2].x = cvCrossPoint_RB.x - nIndent_W + 0.5;
			vtptCorners[2].y = cvCrossPoint_RB.y - nIndent_H + 0.5;
			vtptCorners[3].x = cvCrossPoint_LB.x + nIndent_W + 0.5;
			vtptCorners[3].y = cvCrossPoint_LB.y - nIndent_H + 0.5;
			

			// 0=T
			vt2ptContoursPos.resize(4);
			if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, vtptCorners[0], vtptCorners[1], vt2ptContoursPos[0]) != 1) {
				return false;
			}

			// 1=B
			if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, vtptCorners[3], vtptCorners[2], vt2ptContoursPos[1]) != 1) {
				return false;
			}

			// 2=L
			if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, vtptCorners[0], vtptCorners[3], vt2ptContoursPos[2]) != 1) {
				return false;
			}

			// 3=R
			if (jet_imagefunction::ScanningLine(m_matGlue.cols, m_matGlue.rows, vtptCorners[1], vtptCorners[2], vt2ptContoursPos[3]) != 1) {
				return false;
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				jet_imagefunction::DrawEdgePoint(matDisplay, vt2ptContoursPos[0], Scalar(0, 0, 255), 1);
				jet_imagefunction::DrawEdgePoint(matDisplay, vt2ptContoursPos[1], Scalar(0, 255, 0), 1);
				jet_imagefunction::DrawEdgePoint(matDisplay, vt2ptContoursPos[2], Scalar(255, 0, 0), 1);
				jet_imagefunction::DrawEdgePoint(matDisplay, vt2ptContoursPos[3], Scalar(0, 255, 255), 1);
				SaveImage(0, matDisplay, "IC_Range", m_nExtensionType);
			}

			return true;
		}

		// 找膠面積邊界
		bool MeasurementBlackGlue::RunProcess_FindGlueArea()
		{
			if (!SetFindEdgeProcess(m_sParam_GlueArea.sGlueEdge, m_sPM_FindGlue)) {
				return false;
			}

			vector<Mat> vtmatImage(1);
			vector<jet_imagefunction::SProcessModeOutput> vtsResult;
			vtmatImage[0] = Mat(m_matGlue.size(), m_matGlue.type(), Scalar(0, 0, 0));
			m_matGlue.copyTo(vtmatImage[0], m_matEdgeMask);

			if (!jet_imagefunction::RunProcessMode(vtmatImage, m_sPM_FindGlue, vtsResult)) {
				if (m_bSaveImage && m_nSaveLevel > 0) {
					jet_imagefunction::SaveProcessResult(m_strSavePathName, "Error_FindGlueArea", m_sPM_FindGlue, vtmatImage, vtsResult, 2);
				}
				return false;
			}

			if (m_bSaveImage && m_nSaveLevel > 2) {
				jet_imagefunction::SaveProcessResult(m_strSavePathName, "OK_FindGlueArea", m_sPM_FindGlue, vtmatImage, vtsResult, 2);
			}

			int nNumber = m_sPM_FindGlue.GetBaseParameterNumber();
			if (nNumber != vtsResult.size()) {
				m_strErrorMessage = "RunProcess_FindGlueArea is error";
				return false;
			}

			SContoursParam* psContours = &vtsResult[nNumber - 1].sContours;
			if (psContours == nullptr) {
				m_strErrorMessage = "RunProcess_FindGlueArea is error";
				return false;
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				jet_imagefunction::DisplayContours(*psContours, true, false, matDisplay);
				SaveImage(0, matDisplay, "GlueContours_Src", m_nExtensionType);
			}

			EDeleteMode eDeleteMode = DELETE_SIZE_LESSTHAN;
			int nGlueSize_MinWidth_Pixels = m_sParam_GlueArea.nGlueSize_MinWidth / m_sParam_GlueArea.fResolutionX;
			int nGlueSize_MinHeight_Pixels = m_sParam_GlueArea.nGlueSize_MinHeight / m_sParam_GlueArea.fResolutionY;
			if (!jet_imagefunction::DeleteROI_Size(*psContours, eDeleteMode, eDeleteMode, nGlueSize_MinWidth_Pixels, nGlueSize_MinWidth_Pixels, nGlueSize_MinHeight_Pixels, nGlueSize_MinHeight_Pixels)) {
				m_strErrorMessage = "DeleteROI_Size is error";
				return false;
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				jet_imagefunction::DisplayContours(*psContours, true, false, matDisplay);
				SaveImage(0, matDisplay, "GlueContours_Delete", m_nExtensionType);
			}

			int nMode = 1;
			if (!jet_imagefunction::UnionEdge(nMode, *psContours)) {
				m_strErrorMessage = "UnionEdge is error";
				return false;
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				jet_imagefunction::DisplayContours(*psContours, true, false, matDisplay);
				SaveImage(0, matDisplay, "GlueContours_Union", m_nExtensionType);
			}

			// 輸出輪廓點
			m_sResult_GlueArea.nGlueCount = psContours->GetCount_On();
			if (m_sResult_GlueArea.nGlueCount > 0) {
				m_sResult_GlueArea.vtsGlueArea.resize(m_sResult_GlueArea.nGlueCount);
				int nId = 0;
				int nCount = psContours->GetCount();
				for (int k = 0; k < nCount; ++k) {
					bool* pbOn = psContours->GetOnPtr(k);
					if (pbOn != nullptr && (*pbOn)) {
						vector<vector<POINT>>* pvt2ptLine = psContours->GetLinePtr(k);
						SJRect* psjROI = psContours->GetSJRectPtr(k);
						if (pvt2ptLine != nullptr && psjROI != nullptr) {
							m_sResult_GlueArea.vtsGlueArea[nId].nWidth = (int)(psjROI->nWidth * m_sParam_GlueArea.fResolutionX + 0.5);
							m_sResult_GlueArea.vtsGlueArea[nId].nHeight = (int)(psjROI->nHeight * m_sParam_GlueArea.fResolutionY + 0.5);
							m_sResult_GlueArea.vtsGlueArea[nId].nContoursCount = 0;
							int nCount_Line = (*pvt2ptLine).size();
							for (int h = 0; h < nCount_Line; ++h) {
								m_sResult_GlueArea.vtsGlueArea[nId].nContoursCount += (*pvt2ptLine)[h].size();
							}
							m_sResult_GlueArea.vtsGlueArea[nId].vtptContoursPos.reserve(m_sResult_GlueArea.vtsGlueArea[nId].nContoursCount);
							for (const auto& innerVec : (*pvt2ptLine)) {
								m_sResult_GlueArea.vtsGlueArea[nId].vtptContoursPos.insert(m_sResult_GlueArea.vtsGlueArea[nId].vtptContoursPos.end(), innerVec.begin(), innerVec.end());
							}
							++nId;
						}
					}
				}
			}

			if (m_bSaveImage) {
				Mat matDisplay = m_matGlue.clone();
				for (int k = 0; k < m_sResult_GlueArea.nGlueCount; ++k) {
					jet_imagefunction::DrawEdgePoint(matDisplay, m_sResult_GlueArea.vtsGlueArea[k].vtptContoursPos, m_scContours_Glue, 1);
				}
				SaveImage(0, matDisplay, "GlueContours_Count=" + to_string(m_sResult_GlueArea.nGlueCount), m_nExtensionType);
			}


			return true;
		}

		// 輸出膠面積比例
		bool MeasurementBlackGlue::OutputGlueArea()
		{
			if (m_matEdgeMask.empty() || m_matEdgeMask.channels() != 1 || m_sResult_GlueArea.nGlueCount < 0) {
				return false;
			}

			float fIC_W_T = m_sResult_GlueArea.sIcRange.vtptCorners[1].x - m_sResult_GlueArea.sIcRange.vtptCorners[0].x;
			float fIC_W_B = m_sResult_GlueArea.sIcRange.vtptCorners[2].x - m_sResult_GlueArea.sIcRange.vtptCorners[3].x;
			float fIC_H_L = m_sResult_GlueArea.sIcRange.vtptCorners[3].y - m_sResult_GlueArea.sIcRange.vtptCorners[0].y;
			float fIC_H_R = m_sResult_GlueArea.sIcRange.vtptCorners[2].y - m_sResult_GlueArea.sIcRange.vtptCorners[1].y;
			float fIC_Width_Pixels = (fIC_W_T + fIC_W_B) / 2.0;
			float fIC_Height_Pixels = (fIC_H_L + fIC_H_R) / 2.0;

			// Pixels 轉 um
			m_sResult_GlueArea.sIcRange.nIC_Width = (int)(fIC_Width_Pixels*m_sParam_GlueArea.fResolutionX + 0.5);
			m_sResult_GlueArea.sIcRange.nIC_Height = (int)(fIC_Height_Pixels*m_sParam_GlueArea.fResolutionY + 0.5);

			int nDiffW = abs(m_sResult_GlueArea.sIcRange.nIC_Width - m_sParam_GlueArea.nIC_Width);
			int nDiffH = abs(m_sResult_GlueArea.sIcRange.nIC_Height - m_sParam_GlueArea.nIC_Height);

			if (m_sResult_GlueArea.nGlueCount >= m_sParam_GlueArea.nGlueNG_Count || nDiffW > m_sParam_GlueArea.nIC_NG_DiffWidth || nDiffH > m_sParam_GlueArea.nIC_NG_DiffHeight) {
				m_sResult_GlueArea.bResult = false;
			}
			else {
				m_sResult_GlueArea.bResult = true;
				for (int k = 0; k < m_sResult_GlueArea.nGlueCount; ++k) {
					if (m_sResult_GlueArea.vtsGlueArea[k].nWidth >= m_sParam_GlueArea.nGlueNG_Width || m_sResult_GlueArea.vtsGlueArea[k].nHeight >= m_sParam_GlueArea.nGlueNG_Height) {
						m_sResult_GlueArea.bResult = false;
						k = m_sResult_GlueArea.nGlueCount;
					}
				}
			}

			return true;
		}

#pragma endregion

#pragma region SPIL GlueToPart
		bool MeasurementBlackGlue::SetSpilGluePartProcess(const SFindEdge_Parameter& sParam, SProcessModeParam& sPM)
		{
			int nQueueId = 1;
			sPM.Clear();

			// 1
			JET::mod::SColorTransformParam sColorTransform_ToGray;
			sColorTransform_ToGray.SetInput1(1, 1);
			switch (sParam.nColorTransformMode)
			{
			case 1:// 彩色
				sColorTransform_ToGray.eMode = GRAY_TO_BGR;
				break;
			case 2:// R
			case 3:// G
			case 4:// B
				sColorTransform_ToGray.eMode = COLOR_TO_GRAY_BGR;
				sColorTransform_ToGray.nID = 4 - sParam.nColorTransformMode;
				break;
			case 5:
				sColorTransform_ToGray.eMode = COLOR_TO_GRAY_AVERAGE;
				break;
			}
			sColorTransform_ToGray.SetQueueId(nQueueId);
			sPM.Set(sColorTransform_ToGray);
			++nQueueId;

			// 2
			int nMedianSize = (sParam.nMedianSize > 0) ? sParam.nMedianSize : 1;
			JET::mod::SFilterParam sFilter_Median;
			sFilter_Median.SetInput1(0, -1);
			sFilter_Median.eMode = FILTER_MEDIAN;
			sFilter_Median.nSizeX = nMedianSize;
			sFilter_Median.nSizeY = nMedianSize;
			sFilter_Median.SetQueueId(nQueueId);
			sPM.Set(sFilter_Median);
			++nQueueId;

			// 3
			for (int k = 0; k < sParam.nCount_GrayExtraction; ++k) {
				int nRange = sParam.vtsGrayExtraction[k].nRange;
				int nLow = sParam.vtsGrayExtraction[k].nValue_Gray - nRange;
				if (nLow < 0) nLow = 0;
				int nHigh = sParam.vtsGrayExtraction[k].nValue_Gray + nRange;
				if (nHigh > 255) nHigh = 255;
				JET::mod::SThresholdParam sThreshold_Double;
				sThreshold_Double.SetInput1(2, 2);
				sThreshold_Double.eMode = THRESHOLD_DOUBLE;
				sThreshold_Double.bDark = true;
				sThreshold_Double.nThreshold_Low = nLow;
				sThreshold_Double.nThreshold_High = nHigh;
				sThreshold_Double.SetQueueId(nQueueId);
				sPM.Set(sThreshold_Double);
				++nQueueId;
			}

			if (sParam.nCount_GrayExtraction >= 2) {
				SImageCalculatorParam sCalculator_OR;
				sCalculator_OR.SetInput1(2, 3);
				sCalculator_OR.SetInput2(2, 4);
				sCalculator_OR.eMode = CALCULATOR_OR;
				sCalculator_OR.SetQueueId(nQueueId);
				sPM.Set(sCalculator_OR);
				++nQueueId;

				for (int k = 0; k < sParam.nCount_GrayExtraction - 2; ++k) {
					SImageCalculatorParam sCalculator_OR;
					sCalculator_OR.SetInput1(0, -1);
					sCalculator_OR.SetInput2(2, 5 + k);
					sCalculator_OR.eMode = CALCULATOR_OR;
					sCalculator_OR.SetQueueId(nQueueId);
					sPM.Set(sCalculator_OR);
					++nQueueId;
				}
			}

			// 4
			JET::mod::SContoursShapeParam sContours_Search;
			sContours_Search.SetInput1(0, -1);
			sContours_Search.eMode = CONTOUR_SEARCH;
			sContours_Search.bBoundingToEdge = true;
			sContours_Search.bRectangle = true;
			sContours_Search.bToEdge = true;
			sContours_Search.nSearchType = 1;
			sContours_Search.nSortType = 1;
			sContours_Search.SetQueueId(nQueueId);
			sPM.Set(sContours_Search);
			++nQueueId;

			// 5
			if (sParam.sDeleteMode.sROI.nCount >= 2 && sParam.sDeleteMode.sROI.nCount != sParam.sDeleteMode.sROI.vtrectROI.size()) {
				JET::mod::SDeleteObjectParam sDelete_MaxArea;
				sDelete_MaxArea.SetContours1(0, -1);
				sDelete_MaxArea.eMode_Width = KEEP_ROI_THE_SIMILAR_SIZE;
				sDelete_MaxArea.nHeight_Min = sParam.sDeleteMode.sROI.vtrectROI[0].bottom - sParam.sDeleteMode.sROI.vtrectROI[0].top;
				sDelete_MaxArea.nWidth_Min = sParam.sDeleteMode.sROI.vtrectROI[0].right - sParam.sDeleteMode.sROI.vtrectROI[0].left;
				sDelete_MaxArea.nWidth_Max = 1;
				sDelete_MaxArea.nHeight_Max = 1;
				sDelete_MaxArea.nOutputMode = 2;
				sDelete_MaxArea.SetQueueId(nQueueId);
				sPM.Set(sDelete_MaxArea);
				++nQueueId;
			}




/*
			// 3
			JET::mod::SMorphologParam sMorpholog_Dilate;
			int nDilateX = (sParam.nConnectX > 0) ? sParam.nConnectX : 1;
			int nDilateY = (sParam.nConnectY > 0) ? sParam.nConnectY : 1;
			sMorpholog_Dilate.SetInput1(0, -1);
			sMorpholog_Dilate.eMode = MORPHOLOG_DILATE;
			sMorpholog_Dilate.eElement = ELEMENT_ELLIPSE;
			sMorpholog_Dilate.nSizeX = nDilateX;
			sMorpholog_Dilate.nSizeY = nDilateY;
			sMorpholog_Dilate.SetQueueId(nQueueId);
			sPM.Set(sMorpholog_Dilate);
			++nQueueId;

			// 4
			JET::mod::SContoursShapeParam sContours_Search;
			sContours_Search.SetInput1(0, -1);
			sContours_Search.eMode = CONTOUR_SEARCH;
			sContours_Search.bToEdge = true;
			sContours_Search.bRectangle = true;
			sContours_Search.nSearchType = 1;
			sContours_Search.SetQueueId(nQueueId);
			sPM.Set(sContours_Search);
			++nQueueId;

			// 5
			JET::mod::SDeleteObjectParam sDelete_Keep_Max;
			sDelete_Keep_Max.SetContours1(0, -1);
			sDelete_Keep_Max.eMode_Width = KEEP_ROI_MAXAREA;
			sDelete_Keep_Max.nHeight_Max = sParam.sDeleteMode.nMaxCount;
			sDelete_Keep_Max.SetQueueId(nQueueId);
			sPM.Set(sDelete_Keep_Max);
			++nQueueId;

			// 6
			JET::mod::SContoursShapeParam sContours_Fill;
			sContours_Fill.SetContours1(0, -1);
			sContours_Fill.bToEdge = true;
			sContours_Fill.SetMask1(sMorpholog_Dilate);
			sContours_Fill.eMode = CONTOUR_FLOOD_FILL;
			sContours_Fill.SetQueueId(nQueueId);
			sPM.Set(sContours_Fill);
			++nQueueId;

			// 7
			JET::mod::SColorTransformParam sColorTransform;
			sColorTransform.SetInput1(2, 1);
			sColorTransform.eMode = COLOR_TO_GRAY_BGR;
			sColorTransform.nID = 2;
			sColorTransform.SetQueueId(nQueueId);
			sPM.Set(sColorTransform);
			++nQueueId;

			// 8
			int nGroupNumber = (sParam.sEnhanceMode.sKmean.nGroupNumber > 1) ? sParam.sEnhanceMode.sKmean.nGroupNumber : 2;
			int nLightId = (sParam.sEnhanceMode.sKmean.nLightId > sParam.sEnhanceMode.sKmean.nDarkId) ? sParam.sEnhanceMode.sKmean.nLightId : sParam.sEnhanceMode.sKmean.nDarkId;
			int nDarkId = (sParam.sEnhanceMode.sKmean.nLightId > sParam.sEnhanceMode.sKmean.nDarkId) ? sParam.sEnhanceMode.sKmean.nDarkId : sParam.sEnhanceMode.sKmean.nLightId;
			JET::mod::SImageEnhanceParam sImageEnhance_MaskKmean;
			sImageEnhance_MaskKmean.eMode = ENHANCE_CONTRAST_KMEAN_MASK;
			sImageEnhance_MaskKmean.SetInput1(0, -1);
			sImageEnhance_MaskKmean.SetMask1(2, 3);
			sImageEnhance_MaskKmean.sKmean.nGroupMode = 1;
			sImageEnhance_MaskKmean.sKmean.nGroupNumber = nGroupNumber;
			sImageEnhance_MaskKmean.sKmean.sOutputParams.nMaxId = nLightId;
			sImageEnhance_MaskKmean.sKmean.sOutputParams.nMinId = nDarkId;
			sImageEnhance_MaskKmean.SetQueueId(nQueueId);
			sPM.Set(sImageEnhance_MaskKmean);
			++nQueueId;

			// 9
			JET::mod::SColorTransformParam sColorTransform_Inv;
			sColorTransform_Inv.SetInput1(2, 3);
			sColorTransform_Inv.eMode = COLOR_TO_NEGATIVE;
			sColorTransform_Inv.SetQueueId(nQueueId);
			sPM.Set(sColorTransform_Inv);
			++nQueueId;

			// 10
			JET::mod::SImageCalculatorParam sCalculator_Fill;
			sCalculator_Fill.SetInput1(sImageEnhance_MaskKmean);
			sCalculator_Fill.SetMask1(sColorTransform_Inv);
			sCalculator_Fill.eMode = CALCULATOR_FILL;
			sCalculator_Fill.dValue = 0;
			sCalculator_Fill.SetQueueId(nQueueId);
			sPM.Set(sCalculator_Fill);
			++nQueueId;

			// 11
			JET::mod::SImageEnhanceParam sEnhanceParam;
			sEnhanceParam.SetInput1(0, -1);
			sEnhanceParam.eMode = ENHANCE_EDGE;
			sEnhanceParam.fEps = 5.0;
			sEnhanceParam.fSigma = 5.0;
			sEnhanceParam.SetQueueId(nQueueId);
			sPM.Set(sEnhanceParam);
			++nQueueId;

			// 12
			JET::mod::SImageEnhanceParam sEnhance_Log;
			sEnhance_Log.SetInput1(0, -1);
			sEnhance_Log.eMode = ENHANCE_LOG;
			sEnhance_Log.SetQueueId(nQueueId);
			sPM.Set(sEnhance_Log);
			++nQueueId;
*/
			if (!sPM.CreateList()) {
				return false;
			}

			return true;
		}
		bool MeasurementBlackGlue::SetSpilPartProcess(const SFindEdge_Parameter& sParam, const RECT& rectIC, SProcessModeParam& sPM)
		{
			int nQueueId = 1;
			sPM.Clear();

			// 1
			JET::mod::SColorTransformParam sColorTransform_ToGray;
			sColorTransform_ToGray.SetInput1(1, 1);
			switch (sParam.nColorTransformMode)
			{
			case 1:// R
			case 2:// G
			case 3:// B
				sColorTransform_ToGray.eMode = COLOR_TO_GRAY_BGR;
				sColorTransform_ToGray.nID = 3 - sParam.nColorTransformMode;
				break;
			case 4:
				sColorTransform_ToGray.eMode = COLOR_TO_GRAY_AVERAGE;
				break;
			}
			sColorTransform_ToGray.SetQueueId(nQueueId);
			sPM.Set(sColorTransform_ToGray);
			++nQueueId;

			// 2
			if (sParam.nCount_GrayExtraction != 1 || sParam.vtsGrayExtraction.size() != 1) return false;
			int nThreshold = max(min(255, sParam.vtsGrayExtraction[0].nValue_Gray), 0);
			JET::mod::SThresholdParam sThreshold_Single;
			sThreshold_Single.SetInput1(0, -1);
			sThreshold_Single.eMode = THRESHOLD_SINGLE;
			sThreshold_Single.nThreshold_Low = nThreshold;
			if (sParam.vtsGrayExtraction[0].nRange == 0) {
				sThreshold_Single.bDark = true;
			}
			else {
				sThreshold_Single.bDark = false;;
			}
			sThreshold_Single.SetQueueId(nQueueId);
			sPM.Set(sThreshold_Single);
			++nQueueId;

			// 3
			JET::mod::SContoursShapeParam sContours_Search;
			sContours_Search.SetInput1(0, -1);
			sContours_Search.eMode = CONTOUR_SEARCH;
			sContours_Search.bBoundingToEdge = true;
			sContours_Search.bRectangle = true;
			sContours_Search.bToEdge = true;
			sContours_Search.nSearchType = 1;
			sContours_Search.nSortType = 1;
			sContours_Search.SetQueueId(nQueueId);
			sPM.Set(sContours_Search);
			++nQueueId;

			// 4
			JET::mod::SDeleteObjectParam sDelete_MaxArea;
			sDelete_MaxArea.SetContours1(0, -1);
			sDelete_MaxArea.eMode_Width = KEEP_ROI_THE_SIMILAR_SIZE;
			sDelete_MaxArea.nHeight_Min = rectIC.bottom - rectIC.top;
			sDelete_MaxArea.nWidth_Min = rectIC.right - rectIC.left;
			sDelete_MaxArea.nWidth_Max = 1;
			sDelete_MaxArea.nHeight_Max = 1;
			sDelete_MaxArea.nOutputMode = 2;
			sDelete_MaxArea.SetQueueId(nQueueId);
			sPM.Set(sDelete_MaxArea);
			++nQueueId;

			// 5
			JET::mod::SContoursShapeParam sContours_Fill;
			sContours_Fill.SetContours1(0, -1);
			sContours_Fill.eMode = CONTOUR_FILL;
			sContours_Fill.SetQueueId(nQueueId);
			sPM.Set(sContours_Fill);
			++nQueueId;

			if (!sPM.CreateList()) {
				return false;
			}

			return true;
		}

		bool MeasurementBlackGlue::RunProcess_SpilGlue(const SFindEdge_Parameter& sParam, SProcessModeParam& sPM)
		{
			return true;
		}
#pragma endregion

		// nExtensionType = 1, jpg
		//                = 2, bmp
		//                = 3, png
		bool MeasurementBlackGlue::SaveImage(const int nSaveLevel, const Mat& matImage, const string strTxt, int nExtensionType)
		{
			if (!m_bSaveImage) {
				return true;
			}

			if (nSaveLevel > 0 && nSaveLevel != m_nSaveLevel) {
				return true;
			}

			if (matImage.empty() || m_strSavePathName.empty() || nExtensionType < 1 || nExtensionType > 3) {
				return false;
			}

			string strExtension = "";
			switch (nExtensionType) {
			case 1:
				strExtension = ".jpg";
				break;
			case 2:
				strExtension = ".bmp";
				break;
			case 3:
				strExtension = ".png";
				break;
			}

			cv::imwrite(m_strSavePathName + "_" + to_string(m_nSaveIndex) + "_" + strTxt + strExtension, matImage);
			++m_nSaveIndex;

			return true;
		}

		bool MeasurementBlackGlue::ParameterToConfig(const SMeasurementBlackGlue_Parameter& sParam, vector<ConfigData>& vtsConfig)
		{
			vtsConfig.resize(17);

			int nId = 0;
			vtsConfig[nId].AppName = "Info";
			vtsConfig[nId].AddKeyValue("Version", m_strVersion, DATATYPE_STRING);
			vtsConfig[nId].AddKeyValue("ErrorMessage", m_strErrorMessage, DATATYPE_STRING);
			vtsConfig[nId].AddKeyValue("ResolutionX", jet_imagefunction::FloatingToString("%.5f", sParam.fResolutionX), DATATYPE_FLOAT);
			vtsConfig[nId].AddKeyValue("ResolutionY", jet_imagefunction::FloatingToString("%.5f", sParam.fResolutionY), DATATYPE_FLOAT);
			++nId;

			
#pragma region BoardEdge

			vtsConfig[nId].AppName = "BoardEdge";
			vtsConfig[nId].AddKeyValue("Enable_Board", to_string(sParam.sBoard.bEnable_Board), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("Enable_Slope", to_string(sParam.sBoard.bEnable_Slope), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("FindEdgeMethod", to_string(sParam.sBoard.nFindEdgeMethod), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("BoardValue_R", to_string(sParam.sBoard.nBoardValue_R), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("BoardValue_G", to_string(sParam.sBoard.nBoardValue_G), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("BoardValue_B", to_string(sParam.sBoard.nBoardValue_B), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("BoardValueTolerance", to_string(sParam.sBoard.nBoardValueTolerance), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("ColorThres2_Enable", to_string(sParam.sBoard.bEnable_Color2), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("BoardValue_R2", to_string(sParam.sBoard.nBoardValue_R2), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("BoardValue_G2", to_string(sParam.sBoard.nBoardValue_G2), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("BoardValue_B2", to_string(sParam.sBoard.nBoardValue_B2), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("BoardValueTolerance2", to_string(sParam.sBoard.nBoardValueTolerance2), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("ColorThres3_Enable", to_string(sParam.sBoard.bEnable_Color3), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("BoardValue_R3", to_string(sParam.sBoard.nBoardValue_R3), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("BoardValue_G3", to_string(sParam.sBoard.nBoardValue_G3), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("BoardValue_B3", to_string(sParam.sBoard.nBoardValue_B3), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("BoardValueTolerance3", to_string(sParam.sBoard.nBoardValueTolerance3), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("ColorThres4_Enable", to_string(sParam.sBoard.bEnable_Color4), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("BoardValue_R4", to_string(sParam.sBoard.nBoardValue_R4), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("BoardValue_G4", to_string(sParam.sBoard.nBoardValue_G4), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("BoardValue_B4", to_string(sParam.sBoard.nBoardValue_B4), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("BoardValueTolerance4", to_string(sParam.sBoard.nBoardValueTolerance4), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("ConnectX", to_string(sParam.sBoard.nConnectX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("ConnectY", to_string(sParam.sBoard.nConnectY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("MedianX", to_string(sParam.sBoard.nMedianX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("MedianY", to_string(sParam.sBoard.nMedianY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("OpenX", to_string(sParam.sBoard.nOpenX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("OpenY", to_string(sParam.sBoard.nOpenY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("DilateX", to_string(sParam.sBoard.nDilateX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("DilateY", to_string(sParam.sBoard.nDilateY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("ErosionX", to_string(sParam.sBoard.nErosionX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("ErosionY", to_string(sParam.sBoard.nErosionY), DATATYPE_INTEGER);
			++nId;
#pragma endregion

#pragma region GlueEdge

			vtsConfig[nId].AppName = "GlueEdge";
			vtsConfig[nId].AddKeyValue("Enable", to_string(sParam.sGlue.bEnable), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("FindEdgeMethod", to_string(sParam.sGlue.nFindEdgeMethod), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Location", to_string(sParam.sGlue.nLocation), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("GlueValue_R", to_string(sParam.sGlue.nGlueValue_R), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("GlueValue_G", to_string(sParam.sGlue.nGlueValue_G), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("GlueValue_B", to_string(sParam.sGlue.nGlueValue_B), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("GlueValueTolerance", to_string(sParam.sGlue.nGlueValueTolerance), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Threshold", to_string(sParam.sGlue.nThreshold), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Dark", to_string(sParam.sGlue.bDark), DATATYPE_BOOLEAN);
		
			vtsConfig[nId].AddKeyValue("Filter_MedianX", to_string(sParam.sGlue.nFilter_MedianX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_MedianY", to_string(sParam.sGlue.nFilter_MedianY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_OpenX", to_string(sParam.sGlue.nFilter_OpenX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_OpenY", to_string(sParam.sGlue.nFilter_OpenY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_ConnectX", to_string(sParam.sGlue.nFilter_ConnectX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_ConnectY", to_string(sParam.sGlue.nFilter_ConnectY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_OpenX2", to_string(sParam.sGlue.nFilter_OpenX2), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_OpenY2", to_string(sParam.sGlue.nFilter_OpenY2), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_DilateX", to_string(sParam.sGlue.nFilter_DilateX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_DilateY", to_string(sParam.sGlue.nFilter_DilateY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_ErosionX", to_string(sParam.sGlue.nFilter_ErosionX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_ErosionY", to_string(sParam.sGlue.nFilter_ErosionY), DATATYPE_INTEGER);

			vtsConfig[nId].AddKeyValue("GlueROI_L", to_string(sParam.sGlue.rectGlueROI.left), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("GlueROI_T", to_string(sParam.sGlue.rectGlueROI.top), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("GlueROI_R", to_string(sParam.sGlue.rectGlueROI.right), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("GlueROI_B", to_string(sParam.sGlue.rectGlueROI.bottom), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("GlueROI_Tolerance", to_string(sParam.sGlue.fGlueROI_Tolerance), DATATYPE_FLOAT);
			++nId;
#pragma endregion

#pragma region Coating

			vtsConfig[nId].AppName = "CoatingEdge";
			vtsConfig[nId].AddKeyValue("Enable", to_string(sParam.sCoating.bEnable), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("FindEdgeMethod", to_string(sParam.sCoating.nFindEdgeMethod), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Count", to_string(sParam.sCoating.nCount), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("CoatingValue_R", to_string(sParam.sCoating.nCoatingValue_R), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("CoatingValue_G", to_string(sParam.sCoating.nCoatingValue_G), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("CoatingValue_B", to_string(sParam.sCoating.nCoatingValue_B), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("CoatingValueTolerance", to_string(sParam.sCoating.nCoatingValueTolerance), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_MedianX", to_string(sParam.sCoating.nFilter_MedianX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_MedianY", to_string(sParam.sCoating.nFilter_MedianY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Threshold", to_string(sParam.sCoating.nThreshold), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Dark", to_string(sParam.sCoating.bDark), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("Filter_OpenX", to_string(sParam.sCoating.nFilter_OpenX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_OpenY", to_string(sParam.sCoating.nFilter_OpenY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_ConnectX", to_string(sParam.sCoating.nFilter_ConnectX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_ConnectY", to_string(sParam.sCoating.nFilter_ConnectY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_SmoothX", to_string(sParam.sCoating.nFilter_SmoothX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_SmoothY", to_string(sParam.sCoating.nFilter_SmoothY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_DilateX", to_string(sParam.sCoating.nFilter_DilateX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_DilateY", to_string(sParam.sCoating.nFilter_DilateY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_ErosionX", to_string(sParam.sCoating.nFilter_ErosionX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Filter_ErosionY", to_string(sParam.sCoating.nFilter_ErosionY), DATATYPE_INTEGER);
			++nId;
#pragma endregion

#pragma region GlueWidth_Vertical

			vtsConfig[nId].AppName = "Width_Vertical";
			vtsConfig[nId].AddKeyValue("Enable", to_string(sParam.sGlueWidth_Vertical.bEnable), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("Count", to_string(sParam.sGlueWidth_Vertical.nCount), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("MeanCount", to_string(sParam.sGlueWidth_Vertical.nMeanCount), DATATYPE_INTEGER);
			if (sParam.sGlueWidth_Vertical.nCount > 0) {
				if (sParam.sGlueWidth_Vertical.nCount == sParam.sGlueWidth_Vertical.vtrectROI.size()) {
					for (int k = 0; k < sParam.sGlueWidth_Vertical.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(sParam.sGlueWidth_Vertical.vtrectROI[k].left), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(sParam.sGlueWidth_Vertical.vtrectROI[k].top), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(sParam.sGlueWidth_Vertical.vtrectROI[k].right), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(sParam.sGlueWidth_Vertical.vtrectROI[k].bottom), DATATYPE_INTEGER);
					}
				}
				else {
					for (int k = 0; k < sParam.sGlueWidth_Vertical.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(200), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(200), DATATYPE_INTEGER);
					}
				}

				if (sParam.sGlueWidth_Vertical.nCount == sParam.sGlueWidth_Vertical.vtsLimit.size()) {
					for (int k = 0; k < sParam.sGlueWidth_Vertical.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sGlueWidth_Vertical.vtsLimit[k].sMaxDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sGlueWidth_Vertical.vtsLimit[k].sMaxDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sGlueWidth_Vertical.vtsLimit[k].sMinDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sGlueWidth_Vertical.vtsLimit[k].sMinDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(sParam.sGlueWidth_Vertical.vtsLimit[k].sDiff_MaxMin.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(sParam.sGlueWidth_Vertical.vtsLimit[k].sDiff_MaxMin.fLower), DATATYPE_FLOAT);
					}
				}
				else {
					for (int k = 0; k < sParam.sGlueWidth_Vertical.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
					}
				}
			}
			++nId;
#pragma endregion

#pragma region GlueWidth_Horizontal

			vtsConfig[nId].AppName = "Width_Horizontal";
			vtsConfig[nId].AddKeyValue("Enable", to_string(sParam.sGlueWidth_Horizontal.bEnable), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("Count", to_string(sParam.sGlueWidth_Horizontal.nCount), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("MeanCount", to_string(sParam.sGlueWidth_Horizontal.nMeanCount), DATATYPE_INTEGER);
			if (sParam.sGlueWidth_Horizontal.nCount > 0) {
				if (sParam.sGlueWidth_Horizontal.nCount == sParam.sGlueWidth_Horizontal.vtrectROI.size()) {
					for (int k = 0; k < sParam.sGlueWidth_Horizontal.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(sParam.sGlueWidth_Horizontal.vtrectROI[k].left), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(sParam.sGlueWidth_Horizontal.vtrectROI[k].top), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(sParam.sGlueWidth_Horizontal.vtrectROI[k].right), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(sParam.sGlueWidth_Horizontal.vtrectROI[k].bottom), DATATYPE_INTEGER);
					}
				}
				else {
					for (int k = 0; k < sParam.sGlueWidth_Horizontal.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(200), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(200), DATATYPE_INTEGER);
					}
				}

				if (sParam.sGlueWidth_Horizontal.nCount == sParam.sGlueWidth_Horizontal.vtsLimit.size()) {
					for (int k = 0; k < sParam.sGlueWidth_Horizontal.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sGlueWidth_Horizontal.vtsLimit[k].sMaxDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sGlueWidth_Horizontal.vtsLimit[k].sMaxDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sGlueWidth_Horizontal.vtsLimit[k].sMinDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sGlueWidth_Horizontal.vtsLimit[k].sMinDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(sParam.sGlueWidth_Horizontal.vtsLimit[k].sDiff_MaxMin.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(sParam.sGlueWidth_Horizontal.vtsLimit[k].sDiff_MaxMin.fLower), DATATYPE_FLOAT);
					}
				}
				else {
					for (int k = 0; k < sParam.sGlueWidth_Horizontal.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
					}
				}
			}
			++nId;
#pragma endregion

#pragma region GlueWidth_Arc

			vtsConfig[nId].AppName = "Width_Arc";
			vtsConfig[nId].AddKeyValue("Enable", to_string(sParam.sGlueArc.bEnable), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("Count", to_string(sParam.sGlueArc.nCount), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("MeanCount", to_string(sParam.sGlueArc.nMeanCount), DATATYPE_INTEGER);
			if (sParam.sGlueArc.nCount > 0) {
				if (sParam.sGlueArc.nCount == sParam.sGlueArc.vtrectROI.size()) {
					for (int k = 0; k < sParam.sGlueArc.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(sParam.sGlueArc.vtrectROI[k].left), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(sParam.sGlueArc.vtrectROI[k].top), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(sParam.sGlueArc.vtrectROI[k].right), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(sParam.sGlueArc.vtrectROI[k].bottom), DATATYPE_INTEGER);
					}
				}
				else {
					for (int k = 0; k < sParam.sGlueArc.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(200), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(200), DATATYPE_INTEGER);
					}
				}

				if (sParam.sGlueArc.nCount == sParam.sGlueArc.vtsLimit.size()) {
					for (int k = 0; k < sParam.sGlueArc.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sGlueArc.vtsLimit[k].sMaxDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sGlueArc.vtsLimit[k].sMaxDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sGlueArc.vtsLimit[k].sMinDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sGlueArc.vtsLimit[k].sMinDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(sParam.sGlueArc.vtsLimit[k].sDiff_MaxMin.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(sParam.sGlueArc.vtsLimit[k].sDiff_MaxMin.fLower), DATATYPE_FLOAT);
					}
				}
				else {
					for (int k = 0; k < sParam.sGlueArc.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
					}
				}
			}
			++nId;
#pragma endregion

#pragma region StartEndROI

			vtsConfig[nId].AppName = "StartEndROI";
			vtsConfig[nId].AddKeyValue("Enable", to_string(sParam.sGlueStartEnd.bEnable), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("Count", to_string(sParam.sGlueStartEnd.nCount), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("MeanCount", to_string(sParam.sGlueStartEnd.nMeanCount), DATATYPE_INTEGER);
			if (sParam.sGlueStartEnd.nCount > 0) {
				if (sParam.sGlueStartEnd.nCount == sParam.sGlueStartEnd.vtsStartEnd.size()) {
					for (int k = 0; k < sParam.sGlueStartEnd.nCount; ++k) {
						if (k == 0) {
							vtsConfig[nId].AddKeyValue("StartROI_L", to_string(sParam.sGlueStartEnd.vtsStartEnd[0].rectStartROI.left), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("StartROI_T", to_string(sParam.sGlueStartEnd.vtsStartEnd[0].rectStartROI.top), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("StartROI_R", to_string(sParam.sGlueStartEnd.vtsStartEnd[0].rectStartROI.right), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("StartROI_B", to_string(sParam.sGlueStartEnd.vtsStartEnd[0].rectStartROI.bottom), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("StartROI_ArcLocation", to_string(sParam.sGlueStartEnd.vtsStartEnd[0].nStartROI_ArcLocation), DATATYPE_INTEGER);

							vtsConfig[nId].AddKeyValue("EndROI_L", to_string(sParam.sGlueStartEnd.vtsStartEnd[0].rectEndROI.left), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("EndROI_T", to_string(sParam.sGlueStartEnd.vtsStartEnd[0].rectEndROI.top), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("EndROI_R", to_string(sParam.sGlueStartEnd.vtsStartEnd[0].rectEndROI.right), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("EndROI_B", to_string(sParam.sGlueStartEnd.vtsStartEnd[0].rectEndROI.bottom), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("EndROI_ArcLocation", to_string(sParam.sGlueStartEnd.vtsStartEnd[0].nEndROI_ArcLocation), DATATYPE_INTEGER);

							vtsConfig[nId].AddKeyValue("MaxLimit_Upper", to_string(sParam.sGlueStartEnd.vtsStartEnd[0].sLimit.fUpper), DATATYPE_FLOAT);
							vtsConfig[nId].AddKeyValue("MaxLimit_Lower", to_string(sParam.sGlueStartEnd.vtsStartEnd[0].sLimit.fLower), DATATYPE_FLOAT);
						}
						else {
							vtsConfig[nId].AddKeyValue("StartROI-" + to_string(k + 1) + "_L", to_string(sParam.sGlueStartEnd.vtsStartEnd[k].rectStartROI.left), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("StartROI-" + to_string(k + 1) + "_T", to_string(sParam.sGlueStartEnd.vtsStartEnd[k].rectStartROI.top), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("StartROI-" + to_string(k + 1) + "_R", to_string(sParam.sGlueStartEnd.vtsStartEnd[k].rectStartROI.right), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("StartROI-" + to_string(k + 1) + "_B", to_string(sParam.sGlueStartEnd.vtsStartEnd[k].rectStartROI.bottom), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("StartROI-" + to_string(k + 1) + "_ArcLocation", to_string(sParam.sGlueStartEnd.vtsStartEnd[k].nStartROI_ArcLocation), DATATYPE_INTEGER);

							vtsConfig[nId].AddKeyValue("EndROI-" + to_string(k + 1) + "_L", to_string(sParam.sGlueStartEnd.vtsStartEnd[k].rectEndROI.left), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("EndROI-" + to_string(k + 1) + "_T", to_string(sParam.sGlueStartEnd.vtsStartEnd[k].rectEndROI.top), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("EndROI-" + to_string(k + 1) + "_R", to_string(sParam.sGlueStartEnd.vtsStartEnd[k].rectEndROI.right), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("EndROI-" + to_string(k + 1) + "_B", to_string(sParam.sGlueStartEnd.vtsStartEnd[k].rectEndROI.bottom), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("EndROI-" + to_string(k + 1) + "_ArcLocation", to_string(sParam.sGlueStartEnd.vtsStartEnd[k].nEndROI_ArcLocation), DATATYPE_INTEGER);

							vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sGlueStartEnd.vtsStartEnd[k].sLimit.fUpper), DATATYPE_FLOAT);
							vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sGlueStartEnd.vtsStartEnd[k].sLimit.fLower), DATATYPE_FLOAT);
						}
					}
				}
				else {
					for (int k = 0; k < sParam.sGlueStartEnd.nCount; ++k) {
						if (k == 0) {
							vtsConfig[nId].AddKeyValue("StartROI_L", to_string(0), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("StartROI_T", to_string(0), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("StartROI_R", to_string(100), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("StartROI_B", to_string(100), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("StartROI_ArcLocation", to_string(1), DATATYPE_INTEGER);

							vtsConfig[nId].AddKeyValue("EndROI_L", to_string(0), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("EndROI_T", to_string(0), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("EndROI_R", to_string(100), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("EndROI_B", to_string(100), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("EndROI_ArcLocation", to_string(1), DATATYPE_INTEGER);

							vtsConfig[nId].AddKeyValue("MaxLimit_Upper", to_string(2.0), DATATYPE_FLOAT);
							vtsConfig[nId].AddKeyValue("MaxLimit_Lower", to_string(1.0), DATATYPE_FLOAT);
						}
						else {
							vtsConfig[nId].AddKeyValue("StartROI-" + to_string(k + 1) + "_L", to_string(0), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("StartROI-" + to_string(k + 1) + "_T", to_string(0), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("StartROI-" + to_string(k + 1) + "_R", to_string(100), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("StartROI-" + to_string(k + 1) + "_B", to_string(100), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("StartROI-" + to_string(k + 1) + "_ArcLocation", to_string(1), DATATYPE_INTEGER);

							vtsConfig[nId].AddKeyValue("EndROI-" + to_string(k + 1) + "_L", to_string(0), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("EndROI-" + to_string(k + 1) + "_T", to_string(0), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("EndROI-" + to_string(k + 1) + "_R", to_string(100), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("EndROI-" + to_string(k + 1) + "_B", to_string(100), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("EndROI-" + to_string(k + 1) + "_ArcLocation", to_string(1), DATATYPE_INTEGER);

							vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
							vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						}
					}
				}
			}
			++nId;
#pragma endregion

#pragma region BoardToGlue_Vertical

			vtsConfig[nId].AppName = "BoardToGlue_Vertical";
			vtsConfig[nId].AddKeyValue("Enable", to_string(sParam.sBoardToGlue_Vertical.bEnable), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("Count", to_string(sParam.sBoardToGlue_Vertical.nCount), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("MeanCount", to_string(sParam.sBoardToGlue_Vertical.nMeanCount), DATATYPE_INTEGER);
			if (sParam.sBoardToGlue_Vertical.nCount > 0) {
				if (sParam.sBoardToGlue_Vertical.nCount == sParam.sBoardToGlue_Vertical.vtrectROI.size()) {
					for (int k = 0; k < sParam.sBoardToGlue_Vertical.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(sParam.sBoardToGlue_Vertical.vtrectROI[k].left), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(sParam.sBoardToGlue_Vertical.vtrectROI[k].top), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(sParam.sBoardToGlue_Vertical.vtrectROI[k].right), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(sParam.sBoardToGlue_Vertical.vtrectROI[k].bottom), DATATYPE_INTEGER);
					}
				}
				else {
					for (int k = 0; k < sParam.sBoardToGlue_Vertical.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(200), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(200), DATATYPE_INTEGER);
					}
				}

				if (sParam.sBoardToGlue_Vertical.nCount == sParam.sBoardToGlue_Vertical.vtsLimit.size()) {
					for (int k = 0; k < sParam.sBoardToGlue_Vertical.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sBoardToGlue_Vertical.vtsLimit[k].sMaxDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sBoardToGlue_Vertical.vtsLimit[k].sMaxDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sBoardToGlue_Vertical.vtsLimit[k].sMinDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sBoardToGlue_Vertical.vtsLimit[k].sMinDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(sParam.sBoardToGlue_Vertical.vtsLimit[k].sDiff_MaxMin.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(sParam.sBoardToGlue_Vertical.vtsLimit[k].sDiff_MaxMin.fLower), DATATYPE_FLOAT);
					}
				}
				else {
					for (int k = 0; k < sParam.sBoardToGlue_Vertical.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("DiffLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("DiffLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
					}
				}
			}
			++nId;
#pragma endregion

#pragma region BoardToGlue_Horizontal

			vtsConfig[nId].AppName = "BoardToGlue_Horizontal";
			vtsConfig[nId].AddKeyValue("Enable", to_string(sParam.sBoardToGlue_Horizontal.bEnable), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("Count", to_string(sParam.sBoardToGlue_Horizontal.nCount), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("MeanCount", to_string(sParam.sBoardToGlue_Horizontal.nMeanCount), DATATYPE_INTEGER);
			if (sParam.sBoardToGlue_Horizontal.nCount > 0) {
				if (sParam.sBoardToGlue_Horizontal.nCount == sParam.sBoardToGlue_Horizontal.vtrectROI.size()) {
					for (int k = 0; k < sParam.sBoardToGlue_Horizontal.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(sParam.sBoardToGlue_Horizontal.vtrectROI[k].left), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(sParam.sBoardToGlue_Horizontal.vtrectROI[k].top), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(sParam.sBoardToGlue_Horizontal.vtrectROI[k].right), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(sParam.sBoardToGlue_Horizontal.vtrectROI[k].bottom), DATATYPE_INTEGER);
					}
				}
				else {
					for (int k = 0; k < sParam.sBoardToGlue_Horizontal.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(200), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(200), DATATYPE_INTEGER);
					}
				}

				if (sParam.sBoardToGlue_Horizontal.nCount == sParam.sBoardToGlue_Horizontal.vtsLimit.size()) {
					for (int k = 0; k < sParam.sBoardToGlue_Horizontal.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sBoardToGlue_Horizontal.vtsLimit[k].sMaxDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sBoardToGlue_Horizontal.vtsLimit[k].sMaxDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sBoardToGlue_Horizontal.vtsLimit[k].sMinDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sBoardToGlue_Horizontal.vtsLimit[k].sMinDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(sParam.sBoardToGlue_Horizontal.vtsLimit[k].sDiff_MaxMin.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(sParam.sBoardToGlue_Horizontal.vtsLimit[k].sDiff_MaxMin.fLower), DATATYPE_FLOAT);
					}
				}
				else {
					for (int k = 0; k < sParam.sBoardToGlue_Horizontal.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
					}
				}
			}
			++nId;
#pragma endregion

#pragma region CoatingToGlue_Vertical

			vtsConfig[nId].AppName = "CoatingToGlue_Vertical";
			vtsConfig[nId].AddKeyValue("Enable", to_string(sParam.sCoatingToGlue_Vertical.bEnable), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("Count", to_string(sParam.sCoatingToGlue_Vertical.nCount), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("MeanCount", to_string(sParam.sCoatingToGlue_Vertical.nMeanCount), DATATYPE_INTEGER);
			if (sParam.sCoatingToGlue_Vertical.nCount > 0) {
				if (sParam.sCoatingToGlue_Vertical.nCount == sParam.sCoatingToGlue_Vertical.vtrectROI.size()) {
					for (int k = 0; k < sParam.sCoatingToGlue_Vertical.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(sParam.sCoatingToGlue_Vertical.vtrectROI[k].left), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(sParam.sCoatingToGlue_Vertical.vtrectROI[k].top), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(sParam.sCoatingToGlue_Vertical.vtrectROI[k].right), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(sParam.sCoatingToGlue_Vertical.vtrectROI[k].bottom), DATATYPE_INTEGER);
					}
				}
				else {
					for (int k = 0; k < sParam.sCoatingToGlue_Vertical.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(200), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(200), DATATYPE_INTEGER);
					}
				}

				if (sParam.sCoatingToGlue_Vertical.nCount == sParam.sCoatingToGlue_Vertical.vtsLimit.size()) {
					for (int k = 0; k < sParam.sCoatingToGlue_Vertical.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sCoatingToGlue_Vertical.vtsLimit[k].sMaxDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sCoatingToGlue_Vertical.vtsLimit[k].sMaxDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sCoatingToGlue_Vertical.vtsLimit[k].sMinDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sCoatingToGlue_Vertical.vtsLimit[k].sMinDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(sParam.sCoatingToGlue_Vertical.vtsLimit[k].sDiff_MaxMin.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(sParam.sCoatingToGlue_Vertical.vtsLimit[k].sDiff_MaxMin.fLower), DATATYPE_FLOAT);
					}
				}
				else {
					for (int k = 0; k < sParam.sCoatingToGlue_Vertical.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
					}
				}
			}
			++nId;
#pragma endregion

#pragma region CoatingToGlue_Horizontal

			vtsConfig[nId].AppName = "CoatingToGlue_Horizontal";
			vtsConfig[nId].AddKeyValue("Enable", to_string(sParam.sCoatingToGlue_Horizontal.bEnable), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("Count", to_string(sParam.sCoatingToGlue_Horizontal.nCount), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("MeanCount", to_string(sParam.sCoatingToGlue_Horizontal.nMeanCount), DATATYPE_INTEGER);
			if (sParam.sCoatingToGlue_Horizontal.nCount > 0) {
				if (sParam.sCoatingToGlue_Horizontal.nCount == sParam.sCoatingToGlue_Horizontal.vtrectROI.size()) {
					for (int k = 0; k < sParam.sCoatingToGlue_Horizontal.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(sParam.sCoatingToGlue_Horizontal.vtrectROI[k].left), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(sParam.sCoatingToGlue_Horizontal.vtrectROI[k].top), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(sParam.sCoatingToGlue_Horizontal.vtrectROI[k].right), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(sParam.sCoatingToGlue_Horizontal.vtrectROI[k].bottom), DATATYPE_INTEGER);
					}
				}
				else {
					for (int k = 0; k < sParam.sCoatingToGlue_Horizontal.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(200), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(200), DATATYPE_INTEGER);
					}
				}

				if (sParam.sCoatingToGlue_Vertical.nCount == sParam.sCoatingToGlue_Vertical.vtsLimit.size()) {
					for (int k = 0; k < sParam.sCoatingToGlue_Horizontal.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sCoatingToGlue_Horizontal.vtsLimit[k].sMaxDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sCoatingToGlue_Horizontal.vtsLimit[k].sMaxDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sCoatingToGlue_Horizontal.vtsLimit[k].sMinDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sCoatingToGlue_Horizontal.vtsLimit[k].sMinDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(sParam.sCoatingToGlue_Horizontal.vtsLimit[k].sDiff_MaxMin.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(sParam.sCoatingToGlue_Horizontal.vtsLimit[k].sDiff_MaxMin.fLower), DATATYPE_FLOAT);
					}
				}
				else {
					for (int k = 0; k < sParam.sCoatingToGlue_Horizontal.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
					}
				}
			}
			++nId;
#pragma endregion

#pragma region CoatingToBoard_Vertical

			vtsConfig[nId].AppName = "CoatingToBoard_Vertical";
			vtsConfig[nId].AddKeyValue("Enable", to_string(sParam.sCoatingToBoard_Vertical.bEnable), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("Count", to_string(sParam.sCoatingToBoard_Vertical.nCount), DATATYPE_INTEGER);
			if (sParam.sCoatingToBoard_Vertical.nCount > 0) {
				if (sParam.sCoatingToBoard_Vertical.nCount == sParam.sCoatingToBoard_Vertical.vtrectROI.size()) {
					for (int k = 0; k < sParam.sCoatingToBoard_Vertical.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(sParam.sCoatingToBoard_Vertical.vtrectROI[k].left), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(sParam.sCoatingToBoard_Vertical.vtrectROI[k].top), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(sParam.sCoatingToBoard_Vertical.vtrectROI[k].right), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(sParam.sCoatingToBoard_Vertical.vtrectROI[k].bottom), DATATYPE_INTEGER);
					}
				}
				else {
					for (int k = 0; k < sParam.sCoatingToBoard_Vertical.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(200), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(200), DATATYPE_INTEGER);
					}
				}

				if (sParam.sCoatingToBoard_Vertical.nCount == sParam.sCoatingToBoard_Vertical.vtsLimit.size()) {
					for (int k = 0; k < sParam.sCoatingToBoard_Vertical.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sCoatingToBoard_Vertical.vtsLimit[k].sMaxDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sCoatingToBoard_Vertical.vtsLimit[k].sMaxDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sCoatingToBoard_Vertical.vtsLimit[k].sMinDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sCoatingToBoard_Vertical.vtsLimit[k].sMinDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(sParam.sCoatingToBoard_Vertical.vtsLimit[k].sDiff_MaxMin.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(sParam.sCoatingToBoard_Vertical.vtsLimit[k].sDiff_MaxMin.fLower), DATATYPE_FLOAT);
					}
				}
				else {
					for (int k = 0; k < sParam.sCoatingToBoard_Vertical.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
					}
				}
			}
			++nId;
#pragma endregion

#pragma region CoatingToBoard_Horizontal

			vtsConfig[nId].AppName = "CoatingToBoard_Horizontal";
			vtsConfig[nId].AddKeyValue("Enable", to_string(sParam.sCoatingToBoard_Horizontal.bEnable), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("Count", to_string(sParam.sCoatingToBoard_Horizontal.nCount), DATATYPE_INTEGER);
			if (sParam.sCoatingToBoard_Horizontal.nCount > 0) {
				if (sParam.sCoatingToBoard_Horizontal.nCount == sParam.sCoatingToBoard_Horizontal.vtrectROI.size()) {
					for (int k = 0; k < sParam.sCoatingToBoard_Horizontal.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(sParam.sCoatingToBoard_Horizontal.vtrectROI[k].left), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(sParam.sCoatingToBoard_Horizontal.vtrectROI[k].top), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(sParam.sCoatingToBoard_Horizontal.vtrectROI[k].right), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(sParam.sCoatingToBoard_Horizontal.vtrectROI[k].bottom), DATATYPE_INTEGER);
					}
				}
				else {
					for (int k = 0; k < sParam.sCoatingToBoard_Horizontal.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_L", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_T", to_string(100), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_R", to_string(200), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ROI-" + to_string(k + 1) + "_B", to_string(200), DATATYPE_INTEGER);
					}
				}

				if (sParam.sCoatingToBoard_Horizontal.nCount == sParam.sCoatingToBoard_Horizontal.vtsLimit.size()) {
					for (int k = 0; k < sParam.sCoatingToBoard_Horizontal.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sMaxDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sMaxDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sMinDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sMinDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sDiff_MaxMin.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sDiff_MaxMin.fLower), DATATYPE_FLOAT);
					}
				}
				else {
					for (int k = 0; k < sParam.sCoatingToBoard_Horizontal.nCount; ++k) {
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MaxLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("MinLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
					}
				}
			}
			++nId;
#pragma endregion

#pragma region ThermalGlue

			vtsConfig[nId].AppName = "ThermalGlue";
			vtsConfig[nId].AddKeyValue("Enable", to_string(sParam.sThermal.bEnable), DATATYPE_BOOLEAN);

			// Die
			vtsConfig[nId].AddKeyValue("Die_ROI_L", to_string(sParam.sThermal.rectDie.left), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_ROI_T", to_string(sParam.sThermal.rectDie.top), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_ROI_R", to_string(sParam.sThermal.rectDie.right), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_ROI_B", to_string(sParam.sThermal.rectDie.bottom), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_OutsetDistanceX", to_string(sParam.sThermal.nDie_OutsetDistanceX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_OutsetDistanceY", to_string(sParam.sThermal.nDie_OutsetDistanceY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_Filter_MedianX", to_string(sParam.sThermal.nDie_MedianX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_Filter_MedianY", to_string(sParam.sThermal.nDie_MedianY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_Threshold_Low", to_string(sParam.sThermal.nDie_Threshold_Low), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_Threshold_High", to_string(sParam.sThermal.nDie_Threshold_High), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_Between", to_string(sParam.sThermal.bDie_Between), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("Die_Filter_OpenX", to_string(sParam.sThermal.nDie_OpenX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_Filter_OpenY", to_string(sParam.sThermal.nDie_OpenY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_Filter_ConnectX", to_string(sParam.sThermal.nDie_ConnectX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_Filter_ConnectY", to_string(sParam.sThermal.nDie_ConnectY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_Filter_DilateX", to_string(sParam.sThermal.nDie_DilateX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_Filter_DilateY", to_string(sParam.sThermal.nDie_DilateY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_Filter_ErosionX", to_string(sParam.sThermal.nDie_ErosionX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_Filter_ErosionY", to_string(sParam.sThermal.nDie_ErosionY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_Rebuild", to_string(sParam.sThermal.bRebuildDie), DATATYPE_BOOLEAN);

			// Glue
			vtsConfig[nId].AddKeyValue("Glue_Count", to_string(sParam.sThermal.nGlue_Count), DATATYPE_INTEGER);
			if (sParam.sThermal.nGlue_Count > 0) 
			{
				// GlueWidth
				if (sParam.sThermal.nGlue_Count == sParam.sThermal.vtsLimit_Width.size()) {
					for (int k = 0; k < sParam.sThermal.vtsLimit_Width.size(); ++k) {
						vtsConfig[nId].AddKeyValue("GlueWidth_MaxLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sThermal.vtsLimit_Width[k].sMaxDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueWidth_MaxLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sThermal.vtsLimit_Width[k].sMaxDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueWidth_MinLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sThermal.vtsLimit_Width[k].sMinDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueWidth_MinLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sThermal.vtsLimit_Width[k].sMinDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueWidth_Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(sParam.sThermal.vtsLimit_Width[k].sDiff_MaxMin.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueWidth_Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(sParam.sThermal.vtsLimit_Width[k].sDiff_MaxMin.fLower), DATATYPE_FLOAT);
					}
				}
				else {
					for (int k = 0; k < sParam.sThermal.vtsLimit_Width.size(); ++k) {
						vtsConfig[nId].AddKeyValue("GlueWidth_MaxLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueWidth_MaxLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueWidth_MinLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueWidth_MinLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueWidth_Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueWidth_Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
					}
				}

				// GlueToGlue
				if (sParam.sThermal.nGlue_Count > 1) {
					if (sParam.sThermal.nGlue_Count - 1 == sParam.sThermal.vtsLimit_GlueToGlue.size()) {
						for (int k = 0; k < sParam.sThermal.vtsLimit_GlueToGlue.size(); ++k) {
							vtsConfig[nId].AddKeyValue("GlueToGlue_MaxLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sThermal.vtsLimit_GlueToGlue[k].sMaxDist.fUpper), DATATYPE_FLOAT);
							vtsConfig[nId].AddKeyValue("GlueToGlue_MaxLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sThermal.vtsLimit_GlueToGlue[k].sMaxDist.fLower), DATATYPE_FLOAT);
							vtsConfig[nId].AddKeyValue("GlueToGlue_MinLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sThermal.vtsLimit_GlueToGlue[k].sMinDist.fUpper), DATATYPE_FLOAT);
							vtsConfig[nId].AddKeyValue("GlueToGlue_MinLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sThermal.vtsLimit_GlueToGlue[k].sMinDist.fLower), DATATYPE_FLOAT);
							vtsConfig[nId].AddKeyValue("GlueToGlue_Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(sParam.sThermal.vtsLimit_GlueToGlue[k].sDiff_MaxMin.fUpper), DATATYPE_FLOAT);
							vtsConfig[nId].AddKeyValue("GlueToGlue_Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(sParam.sThermal.vtsLimit_GlueToGlue[k].sDiff_MaxMin.fLower), DATATYPE_FLOAT);
						}
					}
					else {
						for (int k = 0; k < sParam.sThermal.vtsLimit_GlueToGlue.size(); ++k) {
							vtsConfig[nId].AddKeyValue("GlueToGlue_MaxLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
							vtsConfig[nId].AddKeyValue("GlueToGlue_MaxLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
							vtsConfig[nId].AddKeyValue("GlueToGlue_MinLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
							vtsConfig[nId].AddKeyValue("GlueToGlue_MinLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
							vtsConfig[nId].AddKeyValue("GlueToGlue_Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
							vtsConfig[nId].AddKeyValue("GlueToGlue_Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						}
					}
				}

				// GlueToDie
				if (2 == sParam.sThermal.vtsLimit_GlueToDie.size()) {
					for (int k = 0; k < sParam.sThermal.vtsLimit_GlueToDie.size(); ++k) {
						vtsConfig[nId].AddKeyValue("GlueToDie_MaxLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sThermal.vtsLimit_GlueToDie[k].sMaxDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueToDie_MaxLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sThermal.vtsLimit_GlueToDie[k].sMaxDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueToDie_MinLimit-" + to_string(k + 1) + "_Upper", to_string(sParam.sThermal.vtsLimit_GlueToDie[k].sMinDist.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueToDie_MinLimit-" + to_string(k + 1) + "_Lower", to_string(sParam.sThermal.vtsLimit_GlueToDie[k].sMinDist.fLower), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueToDie_Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(sParam.sThermal.vtsLimit_GlueToDie[k].sDiff_MaxMin.fUpper), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueToDie_Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(sParam.sThermal.vtsLimit_GlueToDie[k].sDiff_MaxMin.fLower), DATATYPE_FLOAT);
					}
				}
				else {
					for (int k = 0; k < sParam.sThermal.vtsLimit_GlueToGlue.size(); ++k) {
						vtsConfig[nId].AddKeyValue("GlueToDie_MaxLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueToDie_MaxLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueToDie_MinLimit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueToDie_MinLimit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueToDie_Diff_MaxMin_Limit-" + to_string(k + 1) + "_Upper", to_string(2.0), DATATYPE_FLOAT);
						vtsConfig[nId].AddKeyValue("GlueToDie_Diff_MaxMin_Limit-" + to_string(k + 1) + "_Lower", to_string(1.0), DATATYPE_FLOAT);
					}
				}

			}
			vtsConfig[nId].AddKeyValue("Glue_Direction", to_string(sParam.sThermal.nGlue_Direction), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_InsetDistanceX", to_string(sParam.sThermal.nDie_InsetDistanceX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Die_InsetDistanceY", to_string(sParam.sThermal.nDie_InsetDistanceY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Glue_Filter_MedianX", to_string(sParam.sThermal.nGlue_MedianX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Glue_Filter_MedianY", to_string(sParam.sThermal.nGlue_MedianY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Glue_Threshold", to_string(sParam.sThermal.nGlue_Threshold), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Glue_Dark", to_string(sParam.sThermal.bGlue_Dark), DATATYPE_BOOLEAN);
			vtsConfig[nId].AddKeyValue("Glue_Filter_OpenX", to_string(sParam.sThermal.nGlue_OpenX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Glue_Filter_OpenY", to_string(sParam.sThermal.nGlue_OpenY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Glue_Range_L", to_string(sParam.sThermal.rectGlue_MeasurementRange.left), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Glue_Range_T", to_string(sParam.sThermal.rectGlue_MeasurementRange.top), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Glue_Range_R", to_string(sParam.sThermal.rectGlue_MeasurementRange.right), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("Glue_Range_B", to_string(sParam.sThermal.rectGlue_MeasurementRange.bottom), DATATYPE_INTEGER);

			//
			++nId;
#pragma endregion

			vtsConfig.resize(nId);
			for (int k = 0; k < nId; ++k) {
				if (!vtsConfig[k].Check()){
					return false;
				}
			}

			return true;
		}
		bool MeasurementBlackGlue::ConfigToParameter(const std::vector<ConfigData>& vtsConfig, SMeasurementBlackGlue_Parameter& sParam)
		{
			ConfigFile sConfigfile;
			string tmp;

#pragma region Info

			if (sConfigfile.FindConfigValue(vtsConfig, "Info", "Version", tmp)) {
				m_strVersion = tmp;
			}
			if (sConfigfile.FindConfigValue(vtsConfig, "Info", "ErrorMessage", tmp)) {
				m_strErrorMessage = tmp;
			}

			if (sConfigfile.FindConfigValue(vtsConfig, "Info", "ResolutionX", tmp)) {
				sParam.fResolutionX = sConfigfile.parseFloat(tmp);
			}

			if (sConfigfile.FindConfigValue(vtsConfig, "Info", "ResolutionY", tmp)) {
				sParam.fResolutionY = sConfigfile.parseFloat(tmp);
			}

#pragma endregion

#pragma region BoardEdge

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "Enable_Board", tmp))
				sParam.sBoard.bEnable_Board = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "Enable_Slope", tmp))
				sParam.sBoard.bEnable_Slope = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "FindEdgeMethod", tmp))
				sParam.sBoard.nFindEdgeMethod = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "BoardValue_R", tmp))
				sParam.sBoard.nBoardValue_R = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "BoardValue_G", tmp))
				sParam.sBoard.nBoardValue_G = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "BoardValue_B", tmp))
				sParam.sBoard.nBoardValue_B = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "BoardValueTolerance", tmp))
				sParam.sBoard.nBoardValueTolerance = sConfigfile.parseInt(tmp);

			// 第二組
			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "ColorThres2_Enable", tmp))
				sParam.sBoard.bEnable_Color2 = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "BoardValue_R2", tmp))
				sParam.sBoard.nBoardValue_R2 = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "BoardValue_G2", tmp))
				sParam.sBoard.nBoardValue_G2 = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "BoardValue_B2", tmp))
				sParam.sBoard.nBoardValue_B2 = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "BoardValueTolerance2", tmp))
				sParam.sBoard.nBoardValueTolerance2 = sConfigfile.parseInt(tmp);

			// 第三組
			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "ColorThres3_Enable", tmp))
				sParam.sBoard.bEnable_Color3 = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "BoardValue_R3", tmp))
				sParam.sBoard.nBoardValue_R3 = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "BoardValue_G3", tmp))
				sParam.sBoard.nBoardValue_G3 = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "BoardValue_B3", tmp))
				sParam.sBoard.nBoardValue_B3 = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "BoardValueTolerance3", tmp))
				sParam.sBoard.nBoardValueTolerance3 = sConfigfile.parseInt(tmp);

			// 第四組
			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "ColorThres4_Enable", tmp))
				sParam.sBoard.bEnable_Color4 = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "BoardValue_R4", tmp))
				sParam.sBoard.nBoardValue_R4 = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "BoardValue_G4", tmp))
				sParam.sBoard.nBoardValue_G4 = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "BoardValue_B4", tmp))
				sParam.sBoard.nBoardValue_B4 = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "BoardValueTolerance4", tmp))
				sParam.sBoard.nBoardValueTolerance4 = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "MedianX", tmp))
				sParam.sBoard.nMedianX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "MedianY", tmp))
				sParam.sBoard.nMedianY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "OpenX", tmp))
				sParam.sBoard.nOpenX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "OpenY", tmp))
				sParam.sBoard.nOpenY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "ConnectX", tmp))
				sParam.sBoard.nConnectX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "ConnectY", tmp))
				sParam.sBoard.nConnectY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "DilateX", tmp))
				sParam.sBoard.nDilateX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "DilateY", tmp))
				sParam.sBoard.nDilateY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "ErosionX", tmp))
				sParam.sBoard.nErosionX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardEdge", "ErosionY", tmp))
				sParam.sBoard.nErosionY = sConfigfile.parseInt(tmp);

#pragma endregion

#pragma region GlueEdge

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "Enable", tmp))
				sParam.sGlue.bEnable = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "FindEdgeMethod", tmp))
				sParam.sGlue.nFindEdgeMethod = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "Location", tmp))
				sParam.sGlue.nLocation = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "GlueValue_R", tmp))
				sParam.sGlue.nGlueValue_R = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "GlueValue_G", tmp))
				sParam.sGlue.nGlueValue_G = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "GlueValue_B", tmp))
				sParam.sGlue.nGlueValue_B = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "GlueValueTolerance", tmp))
				sParam.sGlue.nGlueValueTolerance = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "Threshold", tmp))
				sParam.sGlue.nThreshold = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "Dark", tmp))
				sParam.sGlue.bDark = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "Filter_MedianX", tmp))
				sParam.sGlue.nFilter_MedianX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "Filter_MedianY", tmp))
				sParam.sGlue.nFilter_MedianY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "Filter_OpenX", tmp))
				sParam.sGlue.nFilter_OpenX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "Filter_OpenY", tmp))
				sParam.sGlue.nFilter_OpenY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "Filter_ConnectX", tmp))
				sParam.sGlue.nFilter_ConnectX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "Filter_ConnectY", tmp))
				sParam.sGlue.nFilter_ConnectY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "Filter_OpenX2", tmp))
				sParam.sGlue.nFilter_OpenX2 = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "Filter_OpenY2", tmp))
				sParam.sGlue.nFilter_OpenY2 = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "Filter_DilateX", tmp))
				sParam.sGlue.nFilter_DilateX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "Filter_DilateY", tmp))
				sParam.sGlue.nFilter_DilateY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "Filter_ErosionX", tmp))
				sParam.sGlue.nFilter_ErosionX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "Filter_ErosionY", tmp))
				sParam.sGlue.nFilter_ErosionY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "GlueROI_L", tmp))
				sParam.sGlue.rectGlueROI.left = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "GlueROI_T", tmp))
				sParam.sGlue.rectGlueROI.top = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "GlueROI_R", tmp))
				sParam.sGlue.rectGlueROI.right = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "GlueROI_B", tmp))
				sParam.sGlue.rectGlueROI.bottom = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "GlueEdge", "GlueROI_Tolerance", tmp))
				sParam.sGlue.fGlueROI_Tolerance = sConfigfile.parseFloat(tmp);
#pragma endregion

#pragma region CoatingEdge

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "Enable", tmp))
				sParam.sCoating.bEnable = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "FindEdgeMethod", tmp))
				sParam.sCoating.nFindEdgeMethod = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "Count", tmp))
				sParam.sCoating.nCount = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "CoatingValue_R", tmp))
				sParam.sCoating.nCoatingValue_R = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "CoatingValue_G", tmp))
				sParam.sCoating.nCoatingValue_G = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "CoatingValue_B", tmp))
				sParam.sCoating.nCoatingValue_B = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "CoatingValueTolerance", tmp))
				sParam.sCoating.nCoatingValueTolerance = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "Filter_MedianX", tmp))
				sParam.sCoating.nFilter_MedianX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "Filter_MedianY", tmp))
				sParam.sCoating.nFilter_MedianY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "Threshold", tmp))
				sParam.sCoating.nThreshold = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "Dark", tmp))
				sParam.sCoating.bDark = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "Filter_OpenX", tmp))
				sParam.sCoating.nFilter_OpenX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "Filter_OpenY", tmp))
				sParam.sCoating.nFilter_OpenY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "Filter_ConnectX", tmp))
				sParam.sCoating.nFilter_ConnectX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "Filter_ConnectY", tmp))
				sParam.sCoating.nFilter_ConnectY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "Filter_SmoothX", tmp))
				sParam.sCoating.nFilter_SmoothX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "Filter_SmoothY", tmp))
				sParam.sCoating.nFilter_SmoothY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "Filter_DilateX", tmp))
				sParam.sCoating.nFilter_DilateX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "Filter_DilateY", tmp))
				sParam.sCoating.nFilter_DilateY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "Filter_ErosionX", tmp))
				sParam.sCoating.nFilter_ErosionX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingEdge", "Filter_ErosionY", tmp))
				sParam.sCoating.nFilter_ErosionY = sConfigfile.parseInt(tmp);
#pragma endregion

			int nCount = 0;
#pragma region Width_Vertical

			if (sConfigfile.FindConfigValue(vtsConfig, "Width_Vertical", "Enable", tmp))
				sParam.sGlueWidth_Vertical.bEnable = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "Width_Vertical", "Count", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sGlueWidth_Vertical.nCount = nCount;

				sParam.sGlueWidth_Vertical.vtrectROI.resize(sParam.sGlueWidth_Vertical.nCount);
				sParam.sGlueWidth_Vertical.vtsLimit.resize(sParam.sGlueWidth_Vertical.nCount);
				for (int k = 0; k < sParam.sGlueWidth_Vertical.nCount; ++k)
				{
					string strROI = "ROI-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Vertical", strROI + "L", tmp))
						sParam.sGlueWidth_Vertical.vtrectROI[k].left = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Vertical", strROI + "T", tmp))
						sParam.sGlueWidth_Vertical.vtrectROI[k].top = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Vertical", strROI + "R", tmp))
						sParam.sGlueWidth_Vertical.vtrectROI[k].right = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Vertical", strROI + "B", tmp))
						sParam.sGlueWidth_Vertical.vtrectROI[k].bottom = sConfigfile.parseInt(tmp);

					string strLimit_Max = "MaxLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Vertical", strLimit_Max + "Upper", tmp))
						sParam.sGlueWidth_Vertical.vtsLimit[k].sMaxDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Vertical", strLimit_Max + "Lower", tmp))
						sParam.sGlueWidth_Vertical.vtsLimit[k].sMaxDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Min = "MinLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Vertical", strLimit_Min + "Upper", tmp))
						sParam.sGlueWidth_Vertical.vtsLimit[k].sMinDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Vertical", strLimit_Min + "Lower", tmp))
						sParam.sGlueWidth_Vertical.vtsLimit[k].sMinDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Diff = "Diff_MaxMin_Limit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Vertical", strLimit_Diff + "Upper", tmp))
						sParam.sGlueWidth_Vertical.vtsLimit[k].sDiff_MaxMin.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Vertical", strLimit_Diff + "Lower", tmp))
						sParam.sGlueWidth_Vertical.vtsLimit[k].sDiff_MaxMin.fLower = sConfigfile.parseFloat(tmp);
				}
			}

			if (sConfigfile.FindConfigValue(vtsConfig, "Width_Vertical", "MeanCount", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sGlueWidth_Vertical.nMeanCount = nCount;
			}
			else {
				sParam.sGlueWidth_Vertical.nMeanCount = 1;
			}
#pragma endregion

#pragma region Width_Horizontal

			nCount = 0;
			if (sConfigfile.FindConfigValue(vtsConfig, "Width_Horizontal", "Enable", tmp))
				sParam.sGlueWidth_Horizontal.bEnable = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "Width_Horizontal", "Count", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sGlueWidth_Horizontal.nCount = nCount;
				sParam.sGlueWidth_Horizontal.vtrectROI.resize(sParam.sGlueWidth_Horizontal.nCount);
				sParam.sGlueWidth_Horizontal.vtsLimit.resize(sParam.sGlueWidth_Horizontal.nCount);
				for (int k = 0; k < sParam.sGlueWidth_Horizontal.nCount; ++k)
				{
					string strROI = "ROI-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Horizontal", strROI + "L", tmp))
						sParam.sGlueWidth_Horizontal.vtrectROI[k].left = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Horizontal", strROI + "T", tmp))
						sParam.sGlueWidth_Horizontal.vtrectROI[k].top = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Horizontal", strROI + "R", tmp))
						sParam.sGlueWidth_Horizontal.vtrectROI[k].right = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Horizontal", strROI + "B", tmp))
						sParam.sGlueWidth_Horizontal.vtrectROI[k].bottom = sConfigfile.parseInt(tmp);

					string strLimit_Max = "MaxLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Horizontal", strLimit_Max + "Upper", tmp))
						sParam.sGlueWidth_Horizontal.vtsLimit[k].sMaxDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Horizontal", strLimit_Max + "Lower", tmp))
						sParam.sGlueWidth_Horizontal.vtsLimit[k].sMaxDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Min = "MinLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Horizontal", strLimit_Min + "Upper", tmp))
						sParam.sGlueWidth_Horizontal.vtsLimit[k].sMinDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Horizontal", strLimit_Min + "Lower", tmp))
						sParam.sGlueWidth_Horizontal.vtsLimit[k].sMinDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Diff = "Diff_MaxMin_Limit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Horizontal", strLimit_Diff + "Upper", tmp))
						sParam.sGlueWidth_Horizontal.vtsLimit[k].sDiff_MaxMin.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Horizontal", strLimit_Diff + "Lower", tmp))
						sParam.sGlueWidth_Horizontal.vtsLimit[k].sDiff_MaxMin.fLower = sConfigfile.parseFloat(tmp);
				}
			}

			if (sConfigfile.FindConfigValue(vtsConfig, "Width_Horizontal", "MeanCount", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sGlueWidth_Horizontal.nMeanCount = nCount;
			}
			else {
				sParam.sGlueWidth_Horizontal.nMeanCount = 1;
			}
#pragma endregion

#pragma region Width_Arc

			nCount = 0;
			if (sConfigfile.FindConfigValue(vtsConfig, "Width_Arc", "Enable", tmp))
				sParam.sGlueArc.bEnable = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "Width_Arc", "Count", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sGlueArc.nCount = nCount;
				sParam.sGlueArc.vtrectROI.resize(sParam.sGlueArc.nCount);
				sParam.sGlueArc.vtsLimit.resize(sParam.sGlueArc.nCount);
				for (int k = 0; k < sParam.sGlueArc.nCount; ++k)
				{
					string strROI = "ROI-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Arc", strROI + "L", tmp))
						sParam.sGlueArc.vtrectROI[k].left = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Arc", strROI + "T", tmp))
						sParam.sGlueArc.vtrectROI[k].top = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Arc", strROI + "R", tmp))
						sParam.sGlueArc.vtrectROI[k].right = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Arc", strROI + "B", tmp))
						sParam.sGlueArc.vtrectROI[k].bottom = sConfigfile.parseInt(tmp);

					string strLimit_Max = "MaxLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Arc", strLimit_Max + "Upper", tmp))
						sParam.sGlueArc.vtsLimit[k].sMaxDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Arc", strLimit_Max + "Lower", tmp))
						sParam.sGlueArc.vtsLimit[k].sMaxDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Min = "MinLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Arc", strLimit_Min + "Upper", tmp))
						sParam.sGlueArc.vtsLimit[k].sMinDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Arc", strLimit_Min + "Lower", tmp))
						sParam.sGlueArc.vtsLimit[k].sMinDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Diff = "Diff_MaxMin_Limit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Arc", strLimit_Diff + "Upper", tmp))
						sParam.sGlueArc.vtsLimit[k].sDiff_MaxMin.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "Width_Arc", strLimit_Diff + "Lower", tmp))
						sParam.sGlueArc.vtsLimit[k].sDiff_MaxMin.fLower = sConfigfile.parseFloat(tmp);
				}
			}

			if (sConfigfile.FindConfigValue(vtsConfig, "Width_Arc", "MeanCount", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sGlueArc.nMeanCount = nCount;
			}
			else {
				sParam.sGlueArc.nMeanCount = 1;
			}
#pragma endregion

#pragma region StartEndROI

			nCount = 0;
			if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "Enable", tmp))
				sParam.sGlueStartEnd.bEnable = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "Count", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount <= 0) {
				nCount = 1;
			}

			sParam.sGlueStartEnd.nCount = nCount;
			sParam.sGlueStartEnd.vtsStartEnd.resize(sParam.sGlueStartEnd.nCount);
			for (int k = 0; k < sParam.sGlueStartEnd.nCount; ++k) {
				if (k == 0) {
					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "StartROI_L", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[0].rectStartROI.left = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "StartROI_T", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[0].rectStartROI.top = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "StartROI_R", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[0].rectStartROI.right = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "StartROI_B", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[0].rectStartROI.bottom = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "StartROI_ArcLocation", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[0].nStartROI_ArcLocation = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "EndROI_L", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[0].rectEndROI.left = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "EndROI_T", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[0].rectEndROI.top = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "EndROI_R", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[0].rectEndROI.right = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "EndROI_B", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[0].rectEndROI.bottom = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "EndROI_ArcLocation", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[0].nEndROI_ArcLocation = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "MaxLimit_Upper", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[0].sLimit.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "MaxLimit_Lower", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[0].sLimit.fLower = sConfigfile.parseFloat(tmp);
				}
				else {
					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "StartROI-" + to_string(k + 1) + "_L", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[k].rectStartROI.left = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "StartROI-" + to_string(k + 1) + "_T", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[k].rectStartROI.top = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "StartROI-" + to_string(k + 1) + "_R", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[k].rectStartROI.right = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "StartROI-" + to_string(k + 1) + "_B", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[k].rectStartROI.bottom = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "StartROI-" + to_string(k + 1) + "_ArcLocation", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[k].nStartROI_ArcLocation = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "EndROI-" + to_string(k + 1) + "_L", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[k].rectEndROI.left = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "EndROI-" + to_string(k + 1) + "_T", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[k].rectEndROI.top = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "EndROI-" + to_string(k + 1) + "_R", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[k].rectEndROI.right = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "EndROI-" + to_string(k + 1) + "_B", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[k].rectEndROI.bottom = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "EndROI-" + to_string(k + 1) + "_ArcLocation", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[k].nEndROI_ArcLocation = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "MaxLimit-" + to_string(k + 1) + "_Upper", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[k].sLimit.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "MaxLimit-" + to_string(k + 1) + "_Lower", tmp))
						sParam.sGlueStartEnd.vtsStartEnd[k].sLimit.fLower = sConfigfile.parseFloat(tmp);
				}
			}

			if (sConfigfile.FindConfigValue(vtsConfig, "StartEndROI", "MeanCount", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sGlueStartEnd.nMeanCount = nCount;
			}
			else {
				sParam.sGlueStartEnd.nMeanCount = 1;
			}
#pragma endregion	

#pragma region BoardToGlue_Horizontal

			nCount = 0;
			if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Horizontal", "Enable", tmp))
				sParam.sBoardToGlue_Horizontal.bEnable = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Horizontal", "Count", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sBoardToGlue_Horizontal.nCount = nCount;
				sParam.sBoardToGlue_Horizontal.vtrectROI.resize(sParam.sBoardToGlue_Horizontal.nCount);
				sParam.sBoardToGlue_Horizontal.vtsLimit.resize(sParam.sBoardToGlue_Horizontal.nCount);
				for (int k = 0; k < sParam.sBoardToGlue_Horizontal.nCount; ++k)
				{
					string strROI = "ROI-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Horizontal", strROI + "L", tmp))
						sParam.sBoardToGlue_Horizontal.vtrectROI[k].left = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Horizontal", strROI + "T", tmp))
						sParam.sBoardToGlue_Horizontal.vtrectROI[k].top = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Horizontal", strROI + "R", tmp))
						sParam.sBoardToGlue_Horizontal.vtrectROI[k].right = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Horizontal", strROI + "B", tmp))
						sParam.sBoardToGlue_Horizontal.vtrectROI[k].bottom = sConfigfile.parseInt(tmp);

					string strLimit_Max = "MaxLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Horizontal", strLimit_Max + "Upper", tmp))
						sParam.sBoardToGlue_Horizontal.vtsLimit[k].sMaxDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Horizontal", strLimit_Max + "Lower", tmp))
						sParam.sBoardToGlue_Horizontal.vtsLimit[k].sMaxDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Min = "MinLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Horizontal", strLimit_Min + "Upper", tmp))
						sParam.sBoardToGlue_Horizontal.vtsLimit[k].sMinDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Horizontal", strLimit_Min + "Lower", tmp))
						sParam.sBoardToGlue_Horizontal.vtsLimit[k].sMinDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Diff = "Diff_MaxMin_Limit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Horizontal", strLimit_Diff + "Upper", tmp))
						sParam.sBoardToGlue_Horizontal.vtsLimit[k].sDiff_MaxMin.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Horizontal", strLimit_Diff + "Lower", tmp))
						sParam.sBoardToGlue_Horizontal.vtsLimit[k].sDiff_MaxMin.fLower = sConfigfile.parseFloat(tmp);
				}
			}

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Horizontal", "MeanCount", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sBoardToGlue_Horizontal.nMeanCount = nCount;
			}
			else {
				sParam.sBoardToGlue_Horizontal.nMeanCount = 1;
			}
#pragma endregion

#pragma region BoardToGlue_Vertical

			nCount = 0;
			if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Vertical", "Enable", tmp))
				sParam.sBoardToGlue_Vertical.bEnable = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Vertical", "Count", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sBoardToGlue_Vertical.nCount = nCount;
				sParam.sBoardToGlue_Vertical.vtrectROI.resize(sParam.sBoardToGlue_Vertical.nCount);
				sParam.sBoardToGlue_Vertical.vtsLimit.resize(sParam.sBoardToGlue_Vertical.nCount);
				for (int k = 0; k < sParam.sBoardToGlue_Vertical.nCount; ++k)
				{
					string strROI = "ROI-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Vertical", strROI + "L", tmp))
						sParam.sBoardToGlue_Vertical.vtrectROI[k].left = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Vertical", strROI + "T", tmp))
						sParam.sBoardToGlue_Vertical.vtrectROI[k].top = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Vertical", strROI + "R", tmp))
						sParam.sBoardToGlue_Vertical.vtrectROI[k].right = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Vertical", strROI + "B", tmp))
						sParam.sBoardToGlue_Vertical.vtrectROI[k].bottom = sConfigfile.parseInt(tmp);

					string strLimit_Max = "MaxLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Vertical", strLimit_Max + "Upper", tmp))
						sParam.sBoardToGlue_Vertical.vtsLimit[k].sMaxDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Vertical", strLimit_Max + "Lower", tmp))
						sParam.sBoardToGlue_Vertical.vtsLimit[k].sMaxDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Min = "MinLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Vertical", strLimit_Min + "Upper", tmp))
						sParam.sBoardToGlue_Vertical.vtsLimit[k].sMinDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Vertical", strLimit_Min + "Lower", tmp))
						sParam.sBoardToGlue_Vertical.vtsLimit[k].sMinDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Diff = "Diff_MaxMin_Limit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Vertical", strLimit_Diff + "Upper", tmp))
						sParam.sBoardToGlue_Vertical.vtsLimit[k].sDiff_MaxMin.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Vertical", strLimit_Diff + "Lower", tmp))
						sParam.sBoardToGlue_Vertical.vtsLimit[k].sDiff_MaxMin.fLower = sConfigfile.parseFloat(tmp);
				}
			}

			if (sConfigfile.FindConfigValue(vtsConfig, "BoardToGlue_Vertical", "MeanCount", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sBoardToGlue_Vertical.nMeanCount = nCount;
			}
			else {
				sParam.sBoardToGlue_Vertical.nMeanCount = 1;
			}
#pragma endregion

#pragma region CoatingToGlue_Horizontal

			nCount = 0;
			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Horizontal", "Enable", tmp))
				sParam.sCoatingToGlue_Horizontal.bEnable = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Horizontal", "Count", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sCoatingToGlue_Horizontal.nCount = nCount;
				sParam.sCoatingToGlue_Horizontal.vtrectROI.resize(sParam.sCoatingToGlue_Horizontal.nCount);
				sParam.sCoatingToGlue_Horizontal.vtsLimit.resize(sParam.sCoatingToGlue_Horizontal.nCount);
				for (int k = 0; k < sParam.sCoatingToGlue_Horizontal.nCount; ++k)
				{
					string strROI = "ROI-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Horizontal", strROI + "L", tmp))
						sParam.sCoatingToGlue_Horizontal.vtrectROI[k].left = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Horizontal", strROI + "T", tmp))
						sParam.sCoatingToGlue_Horizontal.vtrectROI[k].top = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Horizontal", strROI + "R", tmp))
						sParam.sCoatingToGlue_Horizontal.vtrectROI[k].right = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Horizontal", strROI + "B", tmp))
						sParam.sCoatingToGlue_Horizontal.vtrectROI[k].bottom = sConfigfile.parseInt(tmp);

					string strLimit_Max = "MaxLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Horizontal", strLimit_Max + "Upper", tmp))
						sParam.sCoatingToGlue_Horizontal.vtsLimit[k].sMaxDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Horizontal", strLimit_Max + "Lower", tmp))
						sParam.sCoatingToGlue_Horizontal.vtsLimit[k].sMaxDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Min = "MinLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Horizontal", strLimit_Min + "Upper", tmp))
						sParam.sCoatingToGlue_Horizontal.vtsLimit[k].sMinDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Horizontal", strLimit_Min + "Lower", tmp))
						sParam.sCoatingToGlue_Horizontal.vtsLimit[k].sMinDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Diff = "Diff_MaxMin_Limit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Horizontal", strLimit_Diff + "Upper", tmp))
						sParam.sCoatingToGlue_Horizontal.vtsLimit[k].sDiff_MaxMin.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Horizontal", strLimit_Diff + "Lower", tmp))
						sParam.sCoatingToGlue_Horizontal.vtsLimit[k].sDiff_MaxMin.fLower = sConfigfile.parseFloat(tmp);
				}
			}

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Horizontal", "MeanCount", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sCoatingToGlue_Horizontal.nMeanCount = nCount;
			}
			else {
				sParam.sCoatingToGlue_Horizontal.nMeanCount = 1;
			}
#pragma endregion

#pragma region CoatingToGlue_Vertical

			nCount = 0;
			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Vertical", "Enable", tmp))
				sParam.sCoatingToGlue_Vertical.bEnable = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Vertical", "Count", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sCoatingToGlue_Vertical.nCount = nCount;
				sParam.sCoatingToGlue_Vertical.vtrectROI.resize(sParam.sCoatingToGlue_Vertical.nCount);
				sParam.sCoatingToGlue_Vertical.vtsLimit.resize(sParam.sCoatingToGlue_Vertical.nCount);
				for (int k = 0; k < sParam.sCoatingToGlue_Vertical.nCount; ++k)
				{
					string strROI = "ROI-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Vertical", strROI + "L", tmp))
						sParam.sCoatingToGlue_Vertical.vtrectROI[k].left = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Vertical", strROI + "T", tmp))
						sParam.sCoatingToGlue_Vertical.vtrectROI[k].top = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Vertical", strROI + "R", tmp))
						sParam.sCoatingToGlue_Vertical.vtrectROI[k].right = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Vertical", strROI + "B", tmp))
						sParam.sCoatingToGlue_Vertical.vtrectROI[k].bottom = sConfigfile.parseInt(tmp);

					string strLimit_Max = "MaxLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Vertical", strLimit_Max + "Upper", tmp))
						sParam.sCoatingToGlue_Vertical.vtsLimit[k].sMaxDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Vertical", strLimit_Max + "Lower", tmp))
						sParam.sCoatingToGlue_Vertical.vtsLimit[k].sMaxDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Min = "MinLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Vertical", strLimit_Min + "Upper", tmp))
						sParam.sCoatingToGlue_Vertical.vtsLimit[k].sMinDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Vertical", strLimit_Min + "Lower", tmp))
						sParam.sCoatingToGlue_Vertical.vtsLimit[k].sMinDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Diff = "Diff_MaxMin_Limit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Vertical", strLimit_Diff + "Upper", tmp))
						sParam.sCoatingToGlue_Vertical.vtsLimit[k].sDiff_MaxMin.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Vertical", strLimit_Diff + "Lower", tmp))
						sParam.sCoatingToGlue_Vertical.vtsLimit[k].sDiff_MaxMin.fLower = sConfigfile.parseFloat(tmp);
				}
			}

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToGlue_Vertical", "MeanCount", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sCoatingToGlue_Vertical.nMeanCount = nCount;
			}
			else {
				sParam.sCoatingToGlue_Vertical.nMeanCount = 1;
			}
#pragma endregion		

#pragma region CoatingToBoard_Horizontal

			nCount = 0;
			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Horizontal", "Enable", tmp))
				sParam.sCoatingToBoard_Horizontal.bEnable = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Horizontal", "Count", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sCoatingToBoard_Horizontal.nCount = nCount;
				sParam.sCoatingToBoard_Horizontal.vtrectROI.resize(sParam.sCoatingToBoard_Horizontal.nCount);
				sParam.sCoatingToBoard_Horizontal.vtsLimit.resize(sParam.sCoatingToBoard_Horizontal.nCount);
				for (int k = 0; k < sParam.sCoatingToBoard_Horizontal.nCount; ++k)
				{
					string strROI = "ROI-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Horizontal", strROI + "L", tmp))
						sParam.sCoatingToBoard_Horizontal.vtrectROI[k].left = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Horizontal", strROI + "T", tmp))
						sParam.sCoatingToBoard_Horizontal.vtrectROI[k].top = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Horizontal", strROI + "R", tmp))
						sParam.sCoatingToBoard_Horizontal.vtrectROI[k].right = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Horizontal", strROI + "B", tmp))
						sParam.sCoatingToBoard_Horizontal.vtrectROI[k].bottom = sConfigfile.parseInt(tmp);

					string strLimit_Max = "MaxLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Horizontal", strLimit_Max + "Upper", tmp))
						sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sMaxDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Horizontal", strLimit_Max + "Lower", tmp))
						sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sMaxDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Min = "MinLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Horizontal", strLimit_Min + "Upper", tmp))
						sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sMinDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Horizontal", strLimit_Min + "Lower", tmp))
						sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sMinDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Diff = "Diff_MaxMin_Limit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Horizontal", strLimit_Diff + "Upper", tmp))
						sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sDiff_MaxMin.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Horizontal", strLimit_Diff + "Lower", tmp))
						sParam.sCoatingToBoard_Horizontal.vtsLimit[k].sDiff_MaxMin.fLower = sConfigfile.parseFloat(tmp);
				}
			}
#pragma endregion

#pragma region CoatingToBoard_Vertical

			nCount = 0;
			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Vertical", "Enable", tmp))
				sParam.sCoatingToBoard_Vertical.bEnable = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Vertical", "Count", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sCoatingToBoard_Vertical.nCount = nCount;
				sParam.sCoatingToBoard_Vertical.vtrectROI.resize(sParam.sCoatingToBoard_Vertical.nCount);
				sParam.sCoatingToBoard_Vertical.vtsLimit.resize(sParam.sCoatingToBoard_Vertical.nCount);
				for (int k = 0; k < sParam.sCoatingToBoard_Vertical.nCount; ++k)
				{
					string strROI = "ROI-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Vertical", strROI + "L", tmp))
						sParam.sCoatingToBoard_Vertical.vtrectROI[k].left = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Vertical", strROI + "T", tmp))
						sParam.sCoatingToBoard_Vertical.vtrectROI[k].top = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Vertical", strROI + "R", tmp))
						sParam.sCoatingToBoard_Vertical.vtrectROI[k].right = sConfigfile.parseInt(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Vertical", strROI + "B", tmp))
						sParam.sCoatingToBoard_Vertical.vtrectROI[k].bottom = sConfigfile.parseInt(tmp);

					string strLimit_Max = "MaxLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Vertical", strLimit_Max + "Upper", tmp))
						sParam.sCoatingToBoard_Vertical.vtsLimit[k].sMaxDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Vertical", strLimit_Max + "Lower", tmp))
						sParam.sCoatingToBoard_Vertical.vtsLimit[k].sMaxDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Min = "MinLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Vertical", strLimit_Min + "Upper", tmp))
						sParam.sCoatingToBoard_Vertical.vtsLimit[k].sMinDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Vertical", strLimit_Min + "Lower", tmp))
						sParam.sCoatingToBoard_Vertical.vtsLimit[k].sMinDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Diff = "Diff_MaxMin_Limit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Vertical", strLimit_Diff + "Upper", tmp))
						sParam.sCoatingToBoard_Vertical.vtsLimit[k].sDiff_MaxMin.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "CoatingToBoard_Vertical", strLimit_Diff + "Lower", tmp))
						sParam.sCoatingToBoard_Vertical.vtsLimit[k].sDiff_MaxMin.fLower = sConfigfile.parseFloat(tmp);
				}
			}
#pragma endregion	

#pragma region ThermalGlue

			nCount = 0;

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Enable", tmp))
				sParam.sThermal.bEnable = sConfigfile.parseBoolean(tmp);

			// Die
			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_ROI_L", tmp))
				sParam.sThermal.rectDie.left = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_ROI_T", tmp))
				sParam.sThermal.rectDie.top = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_ROI_R", tmp))
				sParam.sThermal.rectDie.right = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_ROI_B", tmp))
				sParam.sThermal.rectDie.bottom = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_OutsetDistanceX", tmp))
				sParam.sThermal.nDie_OutsetDistanceX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_OutsetDistanceY", tmp))
				sParam.sThermal.nDie_OutsetDistanceY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_Filter_MedianX", tmp))
				sParam.sThermal.nDie_MedianX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_Filter_MedianY", tmp))
				sParam.sThermal.nDie_MedianY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_Threshold_Low", tmp))
				sParam.sThermal.nDie_Threshold_Low = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_Threshold_High", tmp))
				sParam.sThermal.nDie_Threshold_High = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_Between", tmp))
				sParam.sThermal.bDie_Between = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_Filter_ConnectX", tmp))
				sParam.sThermal.nDie_ConnectX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_Filter_ConnectY", tmp))
				sParam.sThermal.nDie_ConnectY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_Filter_OpenX", tmp))
				sParam.sThermal.nDie_OpenX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_Filter_OpenY", tmp))
				sParam.sThermal.nDie_OpenY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_Filter_DilateX", tmp))
				sParam.sThermal.nDie_DilateX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_Filter_DilateY", tmp))
				sParam.sThermal.nDie_DilateY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_Filter_ErosionX", tmp))
				sParam.sThermal.nDie_ErosionX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_Filter_ErosionY", tmp))
				sParam.sThermal.nDie_ErosionY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_Rebuild", tmp))
				sParam.sThermal.bRebuildDie = sConfigfile.parseBoolean(tmp);


			// Glue
			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Glue_Count", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.sThermal.nGlue_Count = nCount;

				// GlueWidth
				sParam.sThermal.vtsLimit_Width.resize(sParam.sThermal.nGlue_Count);
				for (int k = 0; k < sParam.sThermal.nGlue_Count; ++k)
				{
					string strLimit_Max = "GlueWidth_MaxLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Max + "Upper", tmp))
						sParam.sThermal.vtsLimit_Width[k].sMaxDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Max + "Lower", tmp))
						sParam.sThermal.vtsLimit_Width[k].sMaxDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Min = "GlueWidth_MinLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Min + "Upper", tmp))
						sParam.sThermal.vtsLimit_Width[k].sMinDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Min + "Lower", tmp))
						sParam.sThermal.vtsLimit_Width[k].sMinDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Diff = "GlueWidth_Diff_MaxMin_Limit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Diff + "Upper", tmp))
						sParam.sThermal.vtsLimit_Width[k].sDiff_MaxMin.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Diff + "Lower", tmp))
						sParam.sThermal.vtsLimit_Width[k].sDiff_MaxMin.fLower = sConfigfile.parseFloat(tmp);
				}

				// GlueToGlue
				if (sParam.sThermal.nGlue_Count > 1) {
					sParam.sThermal.vtsLimit_GlueToGlue.resize(sParam.sThermal.nGlue_Count - 1);
					for (int k = 0; k < sParam.sThermal.nGlue_Count - 1; ++k)
					{
						string strLimit_Max = "GlueToGlue_MaxLimit-" + std::to_string(k + 1) + "_";
						if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Max + "Upper", tmp))
							sParam.sThermal.vtsLimit_GlueToGlue[k].sMaxDist.fUpper = sConfigfile.parseFloat(tmp);

						if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Max + "Lower", tmp))
							sParam.sThermal.vtsLimit_GlueToGlue[k].sMaxDist.fLower = sConfigfile.parseFloat(tmp);

						string strLimit_Min = "GlueToGlue_MinLimit-" + std::to_string(k + 1) + "_";
						if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Min + "Upper", tmp))
							sParam.sThermal.vtsLimit_GlueToGlue[k].sMinDist.fUpper = sConfigfile.parseFloat(tmp);

						if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Min + "Lower", tmp))
							sParam.sThermal.vtsLimit_GlueToGlue[k].sMinDist.fLower = sConfigfile.parseFloat(tmp);

						string strLimit_Diff = "GlueToGlue_Diff_MaxMin_Limit-" + std::to_string(k + 1) + "_";
						if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Diff + "Upper", tmp))
							sParam.sThermal.vtsLimit_GlueToGlue[k].sDiff_MaxMin.fUpper = sConfigfile.parseFloat(tmp);

						if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Diff + "Lower", tmp))
							sParam.sThermal.vtsLimit_GlueToGlue[k].sDiff_MaxMin.fLower = sConfigfile.parseFloat(tmp);
					}
				}

				// GlueToDie
				sParam.sThermal.vtsLimit_GlueToDie.resize(2);
				for (int k = 0; k < 2; ++k)
				{
					string strLimit_Max = "GlueToDie_MaxLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Max + "Upper", tmp))
						sParam.sThermal.vtsLimit_GlueToDie[k].sMaxDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Max + "Lower", tmp))
						sParam.sThermal.vtsLimit_GlueToDie[k].sMaxDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Min = "GlueToDie_MinLimit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Min + "Upper", tmp))
						sParam.sThermal.vtsLimit_GlueToDie[k].sMinDist.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Min + "Lower", tmp))
						sParam.sThermal.vtsLimit_GlueToDie[k].sMinDist.fLower = sConfigfile.parseFloat(tmp);

					string strLimit_Diff = "GlueToDie_Diff_MaxMin_Limit-" + std::to_string(k + 1) + "_";
					if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Diff + "Upper", tmp))
						sParam.sThermal.vtsLimit_GlueToDie[k].sDiff_MaxMin.fUpper = sConfigfile.parseFloat(tmp);

					if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", strLimit_Diff + "Lower", tmp))
						sParam.sThermal.vtsLimit_GlueToDie[k].sDiff_MaxMin.fLower = sConfigfile.parseFloat(tmp);
				}
			}
			else {
				sParam.sThermal.nGlue_Count = 0;
			}

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Glue_Direction", tmp))
				sParam.sThermal.nGlue_Direction = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_InsetDistanceX", tmp))
				sParam.sThermal.nDie_InsetDistanceX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Die_InsetDistanceY", tmp))
				sParam.sThermal.nDie_InsetDistanceY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Glue_Filter_MedianX", tmp))
				sParam.sThermal.nGlue_MedianX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Glue_Filter_MedianY", tmp))
				sParam.sThermal.nGlue_MedianY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Glue_Threshold", tmp))
				sParam.sThermal.nGlue_Threshold = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Glue_Dark", tmp))
				sParam.sThermal.bGlue_Dark = sConfigfile.parseBoolean(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Glue_Filter_OpenX", tmp))
				sParam.sThermal.nGlue_OpenX = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Glue_Filter_OpenY", tmp))
				sParam.sThermal.nGlue_OpenY = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Glue_Range_L", tmp))
				sParam.sThermal.rectGlue_MeasurementRange.left = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Glue_Range_T", tmp))
				sParam.sThermal.rectGlue_MeasurementRange.top = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Glue_Range_R", tmp))
				sParam.sThermal.rectGlue_MeasurementRange.right = sConfigfile.parseInt(tmp);

			if (sConfigfile.FindConfigValue(vtsConfig, "ThermalGlue", "Glue_Range_B", tmp))
				sParam.sThermal.rectGlue_MeasurementRange.bottom = sConfigfile.parseInt(tmp);

#pragma endregion

			return true;
		}

		// 儲存參數
		bool MeasurementBlackGlue::Save_Parameter()
		{
			if (!m_bSaveImage) return true;
			if (!Save_Parameter(m_sParam, m_strSavePathName)) {
				return false;
			}
			return true;
		}
	}
}