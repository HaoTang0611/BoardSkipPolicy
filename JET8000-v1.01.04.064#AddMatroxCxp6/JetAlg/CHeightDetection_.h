//Author : Jun
#pragma once
#include "JETAlg_Std.h"
#include <windows.h>
#include <string>
#include <vector>

using namespace std;

namespace JET {
	namespace alg {

		// 錫高檢測的輸入參數
		struct STinInputParam
		{
			// 錫(膠)的方向, 1=垂直, 2=水平, 3=垂直水平都有, 4=零件的四個角, 5=垂直水平+四個角
			int nDirection;

			// 計算高度的範圍(pixels)
			int nMeasureRange;

			// Pattern的寬度(pixels)
			int nPatternWidth;

			// Pattern的高度(pixels)
			int nPatternHeight;

			STinInputParam()
			{
				// 1=垂直, 2=水平, 3=垂直水平都有, 4=零件的四個角, 5=垂直水平+四個角
				nDirection = 1;

				// 計算高度的範圍(pixels)
				nMeasureRange = 3;

				nPatternWidth = 0;

				nPatternHeight = 0;
			}
		};

		// 膠高檢測的輸入參數
		struct SGlueInputParam
		{
			int nThres;

			SGlueInputParam()
			{
				nThres = 0;
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

			void Initial()
			{
				fAngle = 0.0;
				fRadian = 0.0;
				vtfHeightDifference.resize(4, 0.0);
				vtfHeightPercent.resize(4, 0.0);
				vtfPartHeight.resize(4, 0.0);
				vtfTinHeight.resize(4, 0.0);
			}
		};

		// 膠高檢測輸出結果
		struct SGlueOutputResult
		{
			// 膠的長度
			vector<int> vtnGlue_Length;

			// 膠的寬度
			vector<int> vtnGlue_Width;

			// 膠的高度
			vector<int> vtnGlue_Height;

			SGlueOutputResult()
			{
				Initial();
			}

			void Initial()
			{
				vtnGlue_Length.resize(4, 0.0);
				vtnGlue_Width.resize(4, 0.0);
				vtnGlue_Height.resize(4, 0.0);
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
		};

		// 高度量測輸出結果
		struct SHeightDetectionResult
		{
			// 錫高檢測輸出結果
			STinOutputResult sTinResult;

			// 膠高檢測輸出結果
			SGlueOutputResult sGlueResult;
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

			// 儲存影像的路徑與檔名
			string						m_strSavePathName;

			// 紀錄版次
			string						m_strVersion;

			// 輸入參數
			const STinInputParam*		m_psParam;

			// 輸出結果
			STinOutputResult*			m_psResult;

			// 用3D高度影像計算出來的零件ROI
			cv::Rect					m_cv3DROI;

			// 計算零件高度的位置
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<vector<POINT>>		m_vtptPartArea;

			// 計算錫高度的位置
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<vector<POINT>>		m_vtptTinArea;

			// 3D高度影像 CV_32FC1
			cv::Mat						m_mat3D;

			// 用m_mat3D做 Sobel計算 CV_8UC1
			cv::Mat						m_matSobel;

			// 2D彩色影像 CV_8UC3
			cv::Mat						m_matColor;

		public:
			CHeightDetection();
			~CHeightDetection();

			// 取得版次
			string GetVersion()const { return m_strVersion; }

			// rectPartROI = 零件的位置
			bool MeasureHeight(const int& nImageH, const int& nImageW, const float* pfInput, const BYTE* pu8Image, const RECT& rect2DROI, const SHeightDetectionParam& sParam, SHeightDetectionResult& sResult);

			// 設定存圖的路徑與檔名(不需要副檔名)
			bool SetSavePathName(const string& strPathName);

			// 設定是否要輸出計算過程的影像
			void SetSaveImage(const bool& bSave);

			// 取出3D高度計算出來的ROI範圍
			void GetAlignmentROI(cv::Rect& cv3DROI);

			// 取出計算零件高度的座標
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<vector<POINT>>& GetPartCoordinate();		

			// 取出計算錫高度的座標
			// 0=Top, 1=Bottom, 2=Left, 3=Right
			vector<vector<POINT>>& GetTinCoordinate();

			// 畫旋轉後的矩形
			// matShow : 要顯示的影像
			// fRadian : 旋轉的弧度
			// cvROI : 要旋轉的 ROI
			// scColor : 顏色
			void DrawRotateRectangle(cv::Mat& matShow, const float& fRadian, const cv::Rect& cvROI, const cv::Scalar& scColor, int nLineWidth);

		private:
			void ReleaseData();

			// 檢查輸入參數
			bool CheckParam(const int& nImageH, const int& nImageW, const SHeightDetectionParam& sParam);

			// 錫高量測
			// rectPartROI = 零件的位置
			bool MeasureHeight_Tin(const RECT& rect2DROI);

			bool Sobel(const cv::Mat& mat3D, const int &nFilterSize, cv::Mat& matSobel);
			bool EdgeImage(const cv::Mat& matThres, cv::Mat& matEdge);

			// matImage 須為二值化影像或邊界影像---ok
			// 若bEdge為true => 會進行取邊界
			// 若bClose為true => 只保留封閉邊界的物件
			// 若bSort為true => 會重新進行位置排序
			bool SearchROI(cv::Mat &matImage, const int &nMinLength, const int &nMaxLength, cv::Rect& cvROI);

			// 用3D高度找出零件範圍
			bool Alignment_Z(const cv::Mat& mat3D, cv::Rect& cv3DROI);

			// 計算線性方程式 y = dSlope * x + dIntercept(單次)
			// vtdCoordinate_X = 每一點的X座標
			// vtdCoordinate_Y = 每一點的X座標
			// dSlope = 輸出方程式的斜率
			// dIntercept = 輸出方程式的截距
			bool SimpleLinearRegr(const vector<double>& vtdCoordinate_X, const vector<double>& vtdCoordinate_Y, double &dSlope, double &dIntercept);

			// 計算線性方程式(不包含垂直線) y = dSlope * x + dIntercept(遞迴)
			// vtdCoordinate_X = 每一點的X座標
			// vtdCoordinate_Y = 每一點的X座標
			// nSumTimes = 遞迴的最大次數
			// dDiff = 前後2次的斜率差若小於 dDiff, 則不再計算 
			// dSlope = 輸出方程式的斜率
			// dIntercept = 輸出方程式的截距
			bool SimpleLinearRegr(const vector<POINT>& vtptPoint, const int& nSumTimes, const double& dDiff, double& dSlope, double& dIntercept);

			bool SimpleLinearRegr_Vertical(const vector<POINT>& vtptPoint, const int& nSumTimes, const double& dDiff, double& dSlope, double& dIntercept);

			// 錫高量測--垂直方向
			bool TinHeightDifference_Vertical();

			// 錫高量測--水平方向
			bool TinHeightDifference_Horizontal();

			// 點旋轉
			void PointRotate(const int& nImageW, const int& nImageH, const POINT& ptInput, const float& fRadian, POINT& ptOutput);

			// 最大熵二值化
			// return : 閥值
			int Threshold_MaxEntropy(const cv::Mat& matSrc);

			// 最小交叉熵二值化
			// return : 閥值
			int Threshold_MinCrossEntropy(const cv::Mat& matSrc);
		};
	}
}