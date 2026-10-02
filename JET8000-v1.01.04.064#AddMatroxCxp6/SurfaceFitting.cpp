#include "StdAfx.h"

#include "SurfaceFitting.h"
#include "OpenCV_Def.h"
//#include <opencv2/opencv.hpp>
#include <vector>
#include <functional>
#include <numeric>

using namespace cv;
using namespace std;

const bool bUseJetMemory=true;
float *CreateFn(int size)
{
	float *Ptr=nullptr;
	if ( false == bUseJetMemory )
	{	Ptr = new float[size]; }
	else
	{
		if ( JetMemory.alloc_func(size, Ptr, "SurfaceFitting::CreateFn", "Ptr") == false )
		{	return nullptr; }
	}	
	return Ptr;
}
void   DestoryFn(float *&Ptr)
{
	if ( nullptr == Ptr ) { return; }
	if ( false == bUseJetMemory )
	{	delete[] Ptr; Ptr=nullptr;	}
	else
	{	JetMemory.free_func(Ptr); }
	return ;
}

CSurfaceFitting::CSurfaceFitting()
{
	m_strVersion = "1.06";
	m_nCount_Coefficient = 0;
	m_vtptCorner.resize(4);
	m_vtdCoefficient_Finally.clear();
	m_vtdCoefficient_Process.clear();
	m_pfX = nullptr;
	m_pfY = nullptr;
	m_pfZ = nullptr;
}

CSurfaceFitting::~CSurfaceFitting()
{
	m_vtptCorner.clear();
	m_vtdCoefficient_Finally.clear();
	m_vtdCoefficient_Process.clear();

	// 釋放 m_pfX, m_pfY, m_pfZ
	ReleaseData();
}

// 執行結果判斷
bool CSurfaceFitting::CheckIsExecption(const int& nReturnValue) const
{
	if (nReturnValue == -1)
	{
		return false;
	}
	
	return true;
}

// 平面擬合  
// ImageH : pfSrc 的高
// ImageW : pfSrc 的寬
// ROI : 平面範圍
// pfSrc : 原始3D高度
// pfDst : 擬合後的高度
// sParam : 輸入參數
// sResult : 輸出結果
// return值 : -1代表執行函式失敗 , 1代表成功 
int CSurfaceFitting::SurfaceFitting(const int& ImageH, const int& ImageW, const RECT& ROI, const float* pfSrc, float* pfDst, const SSurfaceParam& sParam, SSurfaceResult& sResult)
{	
	ReleaseData();
	sResult.Initial();
	int nROI_H = ROI.bottom - ROI.top;
	int nROI_W = ROI.right - ROI.left;
	if (ImageH <= 0 || ImageW <= 0 || nROI_H <= 0 || nROI_W <= 0 || ROI.left<0 || ROI.top<0 || ROI.right>ImageW || ROI.bottom>ImageH ||
		pfSrc == nullptr || pfDst == nullptr || CheckParam(sParam)==-1)
	{
		ReleaseData();
		return -1;
	}

	// 計算取值範圍
	int nRange_Indent = m_sParam.nIndent * 2;
	int nRange_Height = nROI_H - nRange_Indent;
	int nRange_Width = nROI_W - nRange_Indent;
	if (nRange_Height <= 0 || nRange_Width <= 0)
	{
		ReleaseData();
		return -1;
	}

	// 計算總點數
	int nSumCount = 0;
	if (m_sParam.bDataInside == true)
	{
		// 取內縮邊界內的Data
		nSumCount = nRange_Height*nRange_Width;
	}
	else
	{
		// 取零件邊界到內縮邊界內的Data
		nSumCount = (nRange_Height + nRange_Width)*m_sParam.nIndent * 2;
		nSumCount += (m_sParam.nIndent*m_sParam.nIndent * 4);
	}

	if (nSumCount <= 0)
	{
		ReleaseData();
		return -1;
	}

	m_pfX = CreateFn(nSumCount);
	m_pfY = CreateFn(nSumCount);
	m_pfZ = CreateFn(nSumCount);
	if (m_pfX == nullptr || m_pfY == nullptr || m_pfZ == nullptr)
	{
		ReleaseData();
		return -1;
	}

	// 設置輸入資料
	if (SetData_Line(ImageH, ImageW, ROI, nSumCount, pfSrc, m_pfX, m_pfY, m_pfZ) == -1)
	{
		ReleaseData();
		return -1;
	}

	// 計算平面方程式
	switch (m_sParam.nSurfaceType)
	{
	case SURFACE_LINE:
		// 線性平面方程式
		if (SurfaceFitting_Line(m_pfX, m_pfY, m_pfZ, nSumCount, m_sParam.nSumTimes, sResult.nTimes) == -1)
		{
			ReleaseData();
			return -1;
		}
		break;

	case SURFACE_GAUSS:

		break;

	case SURFACE_CURVE:

		break;
	}

	// 計算結果
	if (GetResult(sResult) == -1)
	{
		ReleaseData();
		return -1;
	}

	// 計算擬合後的平面
	if (m_sParam.bCreateSurface = true)
	{
		if (CalculateSurface(m_vtdCoefficient_Finally, ImageH, ImageW, ROI, pfDst) == -1)
		{
			ReleaseData();
			return -1;
		}
	}

	return 1;
}

// 取得擬合平面的最大高度差 傾斜角度 傾斜斜率 最高與最低點的座標
// sResult : 輸出結果
// return值 : -1代表執行函式失敗 , 1代表成功 
void CSurfaceFitting::GetCoefficient(vector<double>& vtdCoefficient)
{
	vtdCoefficient = m_vtdCoefficient_Finally;
}

// 取得平面方程式係數
// vtdCoefficient : 方程式係數 ; 
// 若 SSurfaceParam.nSurfaceType = TYPE_LINE => Z = a1*x + a2*y + a3
//                                 a1 = vtdCoefficient[0] , a2 = vtdCoefficient[1] , a3=vtdCoefficient[2]
int CSurfaceFitting::GetResult(SSurfaceResult& sResult)
{
	if (m_vtptCorner.size() != 4 || m_vtdCoefficient_Finally.size() != m_nCount_Coefficient)
	{
		return -1;
	}

	float fZ = 0.0;
	sResult.fZ_Highest = 0.0;
	sResult.fZ_Lowest = 999999.0;
	switch (m_sParam.nSurfaceType)
	{
	case SURFACE_LINE:
		for (int k = 0; k < 4; ++k)
		{
			fZ = m_vtdCoefficient_Finally[0] * m_vtptCorner[k].x + m_vtdCoefficient_Finally[1] * m_vtptCorner[k].y + m_vtdCoefficient_Finally[2];

			if (fZ > sResult.fZ_Highest)
			{
				sResult.fZ_Highest = fZ;
				sResult.ptHighest = m_vtptCorner[k];
			}

			if (fZ < sResult.fZ_Lowest)
			{
				sResult.fZ_Lowest = fZ;
				sResult.ptLowest = m_vtptCorner[k];
			}
		}
		break;

	case SURFACE_GAUSS:

		break;

	case SURFACE_CURVE:

		break;
	}

	sResult.fDifference_Height = sResult.fZ_Highest - sResult.fZ_Lowest;
	float fDiff_X = sResult.ptHighest.x - sResult.ptLowest.x;
	float fDiff_Y = sResult.ptHighest.y - sResult.ptLowest.y;
	float fDist = sqrt(fDiff_X*fDiff_X + fDiff_Y * fDiff_Y);
	sResult.fSlope = sResult.fDifference_Height / fDist;
	sResult.fAngle = atan(sResult.fSlope)*180.0 / CV_PI;

	return 1;
}

// 計算擬合面
// ImageW : 影像寬度
// ROI : 要進行平面擬合的範圍
// pfDst : 擬合的結果
// return值 : -1代表執行函式失敗 , 1代表成功 
int CSurfaceFitting::CalculateSurface(const vector<double>& vtdCoefficient, const int& ImageH, const int& ImageW, const RECT& ROI, float* pfDst)
{
	int nCount = vtdCoefficient.size();
	int nROI_H = ROI.bottom - ROI.top;
	int nROI_W = ROI.right - ROI.left;
	if (ImageH <= 0 || ImageW <= 0 || nROI_H <= 0 || nROI_W <= 0 || ROI.left<0 || ROI.top<0 || ROI.right>ImageW || ROI.bottom>ImageH ||
		nCount <= 0 || pfDst == nullptr )
	{
		return -1;
	}

	float* pfTemp_Dst = nullptr;
	int nIndexY = 0, nIndex = 0;

	switch (nCount)
	{
	case 3:
		for (int y = ROI.top; y < ROI.bottom; ++y)
		{
			nIndexY = y * ImageW;
			pfTemp_Dst = &pfDst[nIndexY];
			for (int x = ROI.left; x < ROI.right; ++x)
			{
				pfTemp_Dst[x] = vtdCoefficient[0] * x + vtdCoefficient[1] * y + vtdCoefficient[2];
			}
		}
		break;

	default:
		return -1;
		break;
	}

	return 1;
}

// 傾斜校正
// vtdCoefficient : 平面方程式的係數
// ImageH : pfSrc 的高
// ImageW : pfSrc 的寬
// ROI : 平面範圍
// pfSrc : 原始3D高度
// pfDst : 校正後的高度
// return值 : -1代表執行函式失敗 , 1代表成功 
int CSurfaceFitting::TiltCorrection(const vector<double>& vtdCoefficient, const int& ImageH, const int& ImageW, const RECT& ROI, const float* pfSrc, float* pfDst)
{
	int nCount_Coefficient = vtdCoefficient.size();
	int nROI_H = ROI.bottom - ROI.top;
	int nROI_W = ROI.right - ROI.left;
	if (nCount_Coefficient<=0 || ImageH <= 0 || ImageW <= 0 || nROI_H <= 0 || nROI_W <= 0 || ROI.left<0 || ROI.top<0 || 
		ROI.right>ImageW || ROI.bottom>ImageH || pfSrc == nullptr || pfDst == nullptr)
	{
		return -1;
	}

	vector<double> vtdCoefficient_Tilt(nCount_Coefficient-1, 0.0);
	for (int k = 0; k < nCount_Coefficient - 1; ++k)
	{
		vtdCoefficient_Tilt[k] = vtdCoefficient[k] * (-1);
	}

	int nIndexY = 0, nIndex = 0;
	float* pfTemp_Dst = nullptr;

	switch (nCount_Coefficient)
	{
	case 3:// 線性       Z = a1*x + a2*y + a3
		for (int y = 0; y < nROI_H; ++y)
		{
			nIndexY = (y+ ROI.top) * ImageW;
			const float* pfTemp_Src = &pfSrc[nIndexY];
			pfTemp_Dst = &pfDst[nIndexY];
			for (int x = 0; x < nROI_W; ++x)
			{
				nIndex = x + ROI.left;
				pfTemp_Dst[nIndex] = vtdCoefficient_Tilt[0] * x + vtdCoefficient_Tilt[1] * y + pfTemp_Src[nIndex];
			}
		}
		break;

	case 6:// 高斯		  Z = a1*x + a2*y + a3*xy + a4*x^2 + a5*y^2 + a6

		break;

	case 10:// 曲面	      Z = a1*x + a2*y + a3*xy + a4*x^2 + a5*y^2 + a6*x^2y + a7*xy^2 + a8*x^3 + a9*y^3 + a10

		break;

	default:
		return -1;
		break;
	}

	return 1;
}

// 將平面傾斜
// fAngle_X : X方向傾斜角度 ; 在影像坐標系中(原點在左上), 若fAngle_X為正值=>越往右高度會越高
// fAngle_Y : Y方向傾斜角度 ; 在影像坐標系中(原點在左上), 若fAngle_Y為正值=>越往下高度會越高
// ImageH : pfSrc 的高
// ImageW : pfSrc 的寬
// ROI : 平面範圍
// pfSrc : 原始3D高度
// pfDst : 傾斜後的高度
// return值 : -1代表執行函式失敗 , 1代表成功 
int CSurfaceFitting::TiltSurface(const float& fAngle_X, const float& fAngle_Y, const int& ImageH, const int& ImageW, const RECT& ROI, const float* pfSrc, float* pfDst)
{
	int nROI_H = ROI.bottom - ROI.top;
	int nROI_W = ROI.right - ROI.left;
	if (ImageH <= 0 || ImageW <= 0 || nROI_H <= 0 || nROI_W <= 0 || ROI.left<0 || ROI.top<0 || 
		ROI.right>ImageW || ROI.bottom>ImageH || pfSrc == nullptr || pfDst == nullptr)
	{
		return -1;
	}

	float fSlope_X = tan(fAngle_X * CV_PI / 180.0);
	float fSlope_Y = tan(fAngle_Y * CV_PI / 180.0);

	float* pfTemp_Dst = nullptr;
	int nIndexY = 0, nIndex = 0;
	
	for (int y = 0; y < nROI_H; ++y)
	{
		nIndexY = (y + ROI.top) * ImageW;
		const float* pfTemp_Src = &pfSrc[nIndexY];
		pfTemp_Dst = &pfDst[nIndexY];

		for (int x = 0; x < nROI_W; ++x)
		{
			nIndex = x + ROI.left;
			pfTemp_Dst[nIndex] = fSlope_X * x + fSlope_Y * y + pfTemp_Src[nIndex];
		}
	}

	return 1;
}

// 取得遞迴過程的資料
void CSurfaceFitting::GetRecursionData(vector<vector<double>>& vtdCoefficient, vector<float>& vtfMeanError, vector<float>& vtfDiff_Height)
{
	vtdCoefficient = m_vtdCoefficient_Process;
	vtfMeanError = m_fMeanError;
	vtfDiff_Height = m_fDiffHeight;

	int nCount = vtdCoefficient.size();
	int nIndex_Min = 0, nIndex_Max = 0;

	switch (m_sParam.nLossesMode)
	{
	case LOSSES_HEIGHT_MIN:
		
		break;

	case LOSSES_MEANERROR_MIN:
	case LOSSES_ERRORTHRES:
		for (int k = 0; k < nCount; ++k)
		{
			Calculate_DifferenceHeight(vtdCoefficient[k], nIndex_Min, nIndex_Max, vtfDiff_Height[k]);
		}
		break;
	}
}

// 釋放 m_pfX, m_pfY, m_pfZ
void CSurfaceFitting::ReleaseData()
{
	DestoryFn(m_pfX);
	DestoryFn(m_pfY);
	DestoryFn(m_pfZ);
}

// 檢查輸入參數
int CSurfaceFitting::CheckParam(const SSurfaceParam& sParam)
{
	if (sParam.nSurfaceType < 0 || sParam.nSurfaceType >= SURFACE_MAX || sParam.fMaxErrorPercent < 0.0 || sParam.fMaxErrorPercent >= 0.5 ||
		sParam.nIndent < 0 || sParam.nSumTimes <= 0 || sParam.fRatio<=0.0 || sParam.nLossesMode<0 || sParam.nLossesMode>= LOSSES_MAX)
	{
		return -1;
	}

	m_sParam = sParam;

	switch (m_sParam.nSurfaceType)
	{
	case SURFACE_LINE:
		m_nCount_Coefficient = 3;
		break;
	case SURFACE_GAUSS:
		m_nCount_Coefficient = 6;
		break;
	case SURFACE_CURVE:
		m_nCount_Coefficient = 10;
		break;
	}

	m_vtdCoefficient_Finally.resize(m_nCount_Coefficient, 0.0);
	m_vtdCoefficient_Process.resize(m_sParam.nSumTimes, vector<double>(m_nCount_Coefficient, 0.0));
	m_fMeanError.resize(m_sParam.nSumTimes, 0.0);
	m_fDiffHeight.resize(m_sParam.nSumTimes, 0.0);

	return 1;
}

// 設置計算線性平面的Data
// ImageW : pfSrc的寬度
// ROI :要計算的範圍
// pfSrc : 原始3D高度
// pfX : 輸出 ROI範圍的 X座標
// pfY : 輸出 ROI範圍的 Y座標
// pfZ : 輸出 ROI範圍的 高度
int CSurfaceFitting::SetData_Line(const int& ImageH, const int& ImageW, const RECT& ROI, const int& nCount, const float* pfSrc, float* pfX, float* pfY, float* pfZ)
{
	if (nCount<=0 || pfX == nullptr || pfY == nullptr || pfZ == nullptr)
	{
		return -1;
	}

	// 紀錄 ROI 的4個角點
	// LT
	m_vtptCorner[0].x = ROI.left;
	m_vtptCorner[0].y = ROI.top;

	// RT
	m_vtptCorner[1].x = ROI.right-1;
	m_vtptCorner[1].y = ROI.top;

	// RB
	m_vtptCorner[2].x = ROI.right-1;
	m_vtptCorner[2].y = ROI.bottom-1;

	// LB
	m_vtptCorner[3].x = ROI.left;
	m_vtptCorner[3].y = ROI.bottom-1;

	int nStartY = ROI.top + m_sParam.nIndent;
	int nEndY = ROI.bottom - m_sParam.nIndent;
	if (nStartY > ImageH || nEndY < 0 || nStartY>nEndY)
	{
		return -1;
	}

	int nIndex = 0, nId = 0;
	int nStartX = ROI.left + m_sParam.nIndent;
	int nEndX = ROI.right - m_sParam.nIndent;
	if (nStartX > ImageW || nEndX < 0 || nStartX>nEndX)
	{
		return -1;
	}

	
	if (m_sParam.bDataInside == true)
	{
		// 取內縮邊界內的Data
		for (int y = nStartY; y < nEndY; ++y)
		{
			nIndex = y * ImageW;
			const float* pfTemp_Src = &pfSrc[nIndex];
			for (int x = nStartX; x < nEndX; ++x)
			{
				if (nId < nCount)
				{
					pfZ[nId] = pfTemp_Src[x];
					pfX[nId] = x;
					pfY[nId] = y;
					++nId;
				}
			}
		}
	}
	else
	{
		// 取零件邊界到內縮邊界內的Data
		for (int y = ROI.top; y < ROI.bottom; ++y)
		{
			nIndex = y * ImageW;
			const float* pfTemp_Src = &pfSrc[nIndex];
			for (int x = ROI.left; x < ROI.right; ++x)
			{
				if (!(y >= nStartY && y < nEndY && x >= nStartX && x < nEndX) && nId < nCount)
				{
					pfZ[nId] = pfTemp_Src[x];
					pfX[nId] = x;
					pfY[nId] = y;
					++nId;
				}
			}
		}
	}

	return 1;
}

// 線性平面擬合  Z = a1*x + a2*y + a3(遞迴)
// pfX : X座標
// pfY : Y座標
// pfZ : 3D高度
// nCount : pfCoordinateX, pfCoordinateY, pf3D 的數量
// nSumTimes : 遞迴次數上限
// nTimes : 實際遞迴次數
int CSurfaceFitting::SurfaceFitting_Line(float* pfX, const float* pfY, const float* pfZ, const int& nCount, const int& nSumTimes, int& nTimes)
{
	nTimes = 0;
	if (nCount<=0 || pfX == nullptr || pfY == nullptr || pfZ == nullptr || nCount <= 0 || nSumTimes<=0)
	{
		return -1;
	}

	if (nSumTimes == 1)
	{
		if (SurfaceFitting_Line(pfX, pfY, pfZ, nCount, m_vtdCoefficient_Finally) == -1)
		{
			return -1;
		}
		nTimes = 1;
	}
	else
	{
		int nDataSize = sizeof(float)*nCount;
		int nThres_Id = (int)(nCount * m_sParam.fMaxErrorPercent);
		vector<float> vtdError(nCount, 0.0);

		float* pfZ_New = CreateFn(nCount);
		if (pfZ_New == nullptr)
		{
			return -1;
		}
		::memset(pfZ_New, 0.0, nDataSize);

		float* pfError = CreateFn(nCount);
		if (pfError == nullptr)
		{
			DestoryFn(pfZ_New);			
			return -1;
		}
		::memset(pfError, 0.0, nDataSize);

		int nNum = 0, nIndex_Min = 0, nIndex_Max = 0;
		float fErrorThres = 0.0, fErrorThres_Last = 999999.0;
		float fMeanError = 0.0, fMeanError_Last = 999999.0;
		float fMinHeight = 999999.0, fDiffHeight = 0.0;
		bool bGo = true;

		vector<double> vtdCoefficient_Temp(3, 0.0);

		do
		{
			// 計算平面方程式
			if (SurfaceFitting_Line(pfX, pfY, pfZ, nCount, vtdCoefficient_Temp) == -1)
			{				
				DestoryFn(pfZ_New);
				DestoryFn(pfError);
				return -1;
			}

			// 紀錄方程式係數
			m_vtdCoefficient_Process[nTimes] = vtdCoefficient_Temp;

			// 計算擬合平面與實際高度的誤差
			nNum = 0;
			for (int k = 0; k < nCount; ++k)
			{
				if (pfX[k] != -1)
				{
					// 計算擬合平面高度
					pfZ_New[k] = vtdCoefficient_Temp[0] * pfX[k] + vtdCoefficient_Temp[1] * pfY[k] + vtdCoefficient_Temp[2];

					// 計算高度誤差
					pfError[k] = fabs(pfZ_New[k] - pfZ[k]);

					// 排序用
					vtdError[k] = pfError[k];

					// 計算點數
					++nNum;
				}
			}

			if (nNum <= 0)
			{
				DestoryFn(pfZ_New);
				DestoryFn(pfError);
				return -1;
			}

			switch (m_sParam.nLossesMode)
			{
			case LOSSES_HEIGHT_MIN:
#pragma region 取擬合平面高度差異最小的
				if (Calculate_DifferenceHeight(vtdCoefficient_Temp, nIndex_Min, nIndex_Max, fDiffHeight) == -1)
				{
					DestoryFn(pfZ_New);
					DestoryFn(pfError);
					return -1;
				}

				m_fDiffHeight[nTimes] = fDiffHeight; 
				if (fMinHeight > fDiffHeight)
				{
					fMinHeight = fDiffHeight;
					m_vtdCoefficient_Finally = vtdCoefficient_Temp;
				}

				// 剃除極端值
				if (nTimes < nSumTimes - 1)
				{
					// 誤差排序 大到小
					std::sort(vtdError.begin(), vtdError.begin() + nNum, std::greater<float>());

					// 取誤差最大的前 m_sParam.fMaxErrorPercent% 當閥值
					fErrorThres = vtdError[nThres_Id];

					for (int k = 0; k < nCount; ++k)
					{
						if (pfX[k] != -1)
						{
							if (pfError[k] >= fErrorThres)
							{
								pfX[k] = -1;
							}
						}
					}
				}
#pragma endregion
				break;

			case LOSSES_MEANERROR_MIN:
#pragma region 取擬合平面高度與實際高度的平均誤差增加時就停止
				// 誤差排序 大到小
				std::sort(vtdError.begin(), vtdError.begin() + nNum, std::greater<float>());

				// 計算平均誤差
				fMeanError = std::accumulate(vtdError.begin(), vtdError.begin() + nNum, 0.0) / nNum;
				m_fMeanError[nTimes] = fMeanError;

				// 這次平均誤差小於上次的平均誤差才會繼續做
				if (fMeanError < fMeanError_Last)
				{
					m_vtdCoefficient_Finally = vtdCoefficient_Temp;

					// 剔除極端值
					if (nTimes < nSumTimes - 1)
					{
						fMeanError_Last = fMeanError;

						// 取誤差最大的前 m_sParam.fMaxErrorPercent% 當閥值
						fErrorThres = vtdError[nThres_Id];
						for (int k = 0; k < nCount; ++k)
						{
							if (pfX[k] != -1)
							{
								if (pfError[k] >= fErrorThres)
								{
									pfX[k] = -1;
								}
							}
						}
					}
				}
				else
				{
					bGo = false;
				}
#pragma endregion
				break;

			case LOSSES_ERRORTHRES:
#pragma region 誤差閥值前後次差異小於1就停止
				// 誤差排序 大到小
				std::sort(vtdError.begin(), vtdError.begin() + nNum, std::greater<float>());

				// 取誤差最大的前 m_sParam.fMaxErrorPercent% 當閥值
				fErrorThres = vtdError[nThres_Id];
				m_fMeanError[nTimes] = fErrorThres;

				// 這次的誤差閥值比上次的小1.0以上才會繼續做
				if ((fErrorThres_Last- fErrorThres) > 1.0)
				{
					m_vtdCoefficient_Finally = vtdCoefficient_Temp;

					// 剔除極端值
					if (nTimes < nSumTimes - 1)
					{
						fErrorThres_Last = fErrorThres;
						for (int k = 0; k < nCount; ++k)
						{
							if (pfX[k] != -1)
							{
								if (pfError[k] >= fErrorThres)
								{
									pfX[k] = -1;
								}
							}
						}
					}
				}
				else
				{
					if (fErrorThres_Last > fErrorThres)
					{
						m_vtdCoefficient_Finally = vtdCoefficient_Temp;
					}
					bGo = false;
				}
#pragma endregion
				break;
			}

			++nTimes;
			if (nTimes >= nSumTimes)
			{
				bGo = false;
			}

		} while (bGo);

		DestoryFn(pfZ_New);
		DestoryFn(pfError);
	}

	return 1;
}

// 線性平面擬合  Z = a1*x + a2*y + a3(單次)
// pfCoordinateX : X座標
// pfCoordinateY : Y座標
// pf3D : 3D高度
// nCount = pfCoordinateX, pfCoordinateY, pf3D 的數量
// vtdCoefficient : 方程式係數
// vtdCoefficient[0] = a1
// vtdCoefficient[1] = a2
// vtdCoefficient[2] = a3
int CSurfaceFitting::SurfaceFitting_Line(const float* pfCoordinateX, const float* pfCoordinateY, const float* pf3D, const int& nCount, vector<double>& vtdCoefficient)
{
	if (nCount <= 0 || pfCoordinateX == nullptr || pfCoordinateY == nullptr || pf3D == nullptr)
	{
		return -1;
	}

	vtdCoefficient.resize(3, 0.0);

	double dX = 0.0, dY = 0.0, dSumX = 0.0, dSumY = 0.0, dSumXY = 0.0, dSumX2 = 0.0, dSumY2 = 0.0;
	double dZ = 0.0, dSumZ = 0.0, dSumXZ = 0.0, dSumYZ = 0.0;

	int nNum = 0;
	for (int k = 0; k < nCount; ++k)
	{
		if (pfCoordinateX[k] != -1 && nNum<nCount)
		{
			dX = pfCoordinateX[k] / m_sParam.fRatio;
			dY = pfCoordinateY[k] / m_sParam.fRatio;
			dZ = pf3D[k] / m_sParam.fRatio;

			dSumX += dX;
			dSumY += dY;
			dSumZ += dZ;
			dSumXY += (dX*dY);
			dSumX2 += (dX*dX);
			dSumY2 += (dY*dY);
			dSumXZ += (dX*dZ);
			dSumYZ += (dY*dZ);
			++nNum;
		}
	}

	Mat matL(3, 3, CV_64F, Scalar(0.0));
	double* pfPtr = matL.ptr<double>(0);
	pfPtr[0] = dSumX2;
	pfPtr[1] = dSumXY;
	pfPtr[2] = dSumX;

	pfPtr = matL.ptr<double>(1);
	pfPtr[0] = dSumXY;
	pfPtr[1] = dSumY2;
	pfPtr[2] = dSumY;

	pfPtr = matL.ptr<double>(2);
	pfPtr[0] = dSumX;
	pfPtr[1] = dSumY;
	pfPtr[2] = nNum;

	Mat matL_Inv;
	cv::invert(matL, matL_Inv);

	Mat matR(3, 1, CV_64F, Scalar(0.0));
	matR.at<double>(0, 0) = dSumXZ;
	matR.at<double>(1, 0) = dSumYZ;
	matR.at<double>(2, 0) = dSumZ;

	Mat matM;
	matM = matL_Inv * matR;

	vtdCoefficient[0] = matM.at<double>(0, 0);
	vtdCoefficient[1] = matM.at<double>(1, 0);
	vtdCoefficient[2] = matM.at<double>(2, 0) * m_sParam.fRatio;

	return 1;
}

// 計算擬合平面4個角點的高度差
// vtdCoefficient : 方程式係數
// nIndex_Min : 高度最低角點的 index
// nIndex_Max : 高度最高角點的 index
// fDiffHeight : 高度差
int CSurfaceFitting::Calculate_DifferenceHeight(const vector<double>& vtdCoefficient, int& nIndex_Min, int& nIndex_Max, float& fDiffHeight)
{
	fDiffHeight = 0.0;
	int nCount_Coefficient = vtdCoefficient.size();
	if (nCount_Coefficient <= 0 || m_vtptCorner.size() != 4)
	{
		return -1;
	}

	float fZ=0.0, fZ_Min = 999999.0, fZ_Max = 0.0;
	for (int k = 0; k < 4; ++k)
	{
		fZ = vtdCoefficient[0] * m_vtptCorner[k].x + vtdCoefficient[1] * m_vtptCorner[k].y + vtdCoefficient[2];
		if (fZ_Min > fZ)
		{
			fZ_Min = fZ;
			nIndex_Min = k;
		}

		if (fZ_Max < fZ)
		{
			fZ_Max = fZ;
			nIndex_Max = k;
		}
	}

	fDiffHeight = fZ_Max - fZ_Min;

	return 1;
}

// 計算平面傾斜角度
// fSlope : 傾斜斜率
// fAngle : 傾斜角度
int CSurfaceFitting::Calculate_3DSlope(const vector<double>& vtdCoefficient, float& fSlope, float& fAngle)
{
	fSlope = 0.0;
	fAngle = 999.0;
	if (vtdCoefficient.size() != m_nCount_Coefficient || m_vtptCorner.size() != 4)
	{
		return -1;
	}

	float fDiff_X = 0.0, fDiff_Y = 0.0, fDiff_Z = 0.0, fDist = 0.0;
	int nIndex_Min = 0, nIndex_Max = 0;
	switch (m_sParam.nSurfaceType)
	{
	case SURFACE_LINE:
		if (Calculate_DifferenceHeight(vtdCoefficient, nIndex_Min, nIndex_Max, fDiff_Z) == -1)
		{
			return -1;
		}

		fDiff_X = m_vtptCorner[nIndex_Max].x - m_vtptCorner[nIndex_Min].x;
		fDiff_Y = m_vtptCorner[nIndex_Max].y - m_vtptCorner[nIndex_Min].y;
		fDist = sqrt(fDiff_X*fDiff_X + fDiff_Y*fDiff_Y);
		if (fDist < 0)
		{
			return -1;
		}
		else if (fDist == 0)
		{
			fDist = 1;
		}

		fSlope = fDiff_Z / fDist;
		fAngle = atan(fSlope)*180.0 / CV_PI;
		break;

	case SURFACE_GAUSS:

		break;

	case SURFACE_CURVE:

		break;
	}

	return 1;
}