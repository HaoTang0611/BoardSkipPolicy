// AlgInspectionMeasureConnector.cpp: implementation of the CAlgInspectionMeasureConnector class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AlgParam.h"
#include "AlgInspectionMeasureConnector.h"
//-------------------------------------------------------------------------------------//
#include "JetBlob.h"

CAlgInspectionMeasureConnector::CAlgInspectionMeasureConnector(CAlgParam& Owner)
	: m_Owner(Owner)
{
}
//-------------------------------------------------------------------------------------//
long long CAlgInspectionMeasureConnector::MakeGridKey2D(int x, int y)
{
	return (((long long)(unsigned int)x) << 32) | (unsigned int)y;
}
//-------------------------------------------------------------------------------------//
bool CAlgInspectionMeasureConnector::GetAffine(const std::vector<std::pair<TPOINT2D, TPOINT2D> >& vtInputPoint2d, int nType, double dRate, int nRecursiveCount, double dAllowError, std::vector<double>& vtdCoefficient)
{
	// nType : 1 => first  轉 second
	// nType : 2 => second 轉 first
	//
	// nRecursiveCount : 最多剃除異常值次數
	// dAllowError     : 誤差容許值，單位與輸入點相同
	vtdCoefficient.clear();
	if (vtInputPoint2d.size() < 3) { return false; }
	if (nType != 1 && nType != 2) { return false; }
	if (dRate <= 0.0) { return false; }
	if (nRecursiveCount < 0) { return false; }
	if (dAllowError < 0.0) { return false; }

	const double dInvRate = 1.0 / dRate;
	const double dDetEps = 1e-12;
	const double dAllowError2 = dAllowError * dAllowError;
	std::vector<int> vtUseIndex;
	vtUseIndex.reserve(vtInputPoint2d.size());

	for (int i = 0; i < (int)vtInputPoint2d.size(); ++i)
	{
		const TPOINT2D& ptSrc = (nType == 1) ? vtInputPoint2d[i].first : vtInputPoint2d[i].second;
		const TPOINT2D& ptDst = (nType == 1) ? vtInputPoint2d[i].second : vtInputPoint2d[i].first;
		if (ptSrc.x == -1 || ptSrc.y == -1 || ptDst.x == -1 || ptDst.y == -1) { continue; }
		vtUseIndex.push_back(i);
	}
	if (vtUseIndex.size() < 3) { return false; }

	auto SolveAffine = [&](const std::vector<int>& vtIndex, std::vector<double>& vtCoef) -> bool
	{
		if (vtIndex.size() < 3) { return false; }
		double dSumX = 0.0;
		double dSumY = 0.0;
		double dSumX2 = 0.0;
		double dSumY2 = 0.0;
		double dSumXY = 0.0;

		double dSumU = 0.0;
		double dSumV = 0.0;

		double dSumXU = 0.0;
		double dSumYU = 0.0;
		double dSumXV = 0.0;
		double dSumYV = 0.0;

		const int nCount = (int)vtIndex.size();

		for (int i = 0; i < nCount; ++i)
		{
			const int nIndex = vtIndex[i];

			const TPOINT2D& ptSrc = (nType == 1) ? vtInputPoint2d[nIndex].first : vtInputPoint2d[nIndex].second;
			const TPOINT2D& ptDst = (nType == 1) ? vtInputPoint2d[nIndex].second : vtInputPoint2d[nIndex].first;

			const double x = ptSrc.x * dInvRate;
			const double y = ptSrc.y * dInvRate;
			const double u = ptDst.x * dInvRate;
			const double v = ptDst.y * dInvRate;

			dSumX += x;
			dSumY += y;
			dSumX2 += x * x;
			dSumY2 += y * y;
			dSumXY += x * y;

			dSumU += u;
			dSumV += v;

			dSumXU += x * u;
			dSumYU += y * u;
			dSumXV += x * v;
			dSumYV += y * v;
		}

		const double a11 = dSumX2;
		const double a12 = dSumXY;
		const double a13 = dSumX;

		const double a21 = dSumXY;
		const double a22 = dSumY2;
		const double a23 = dSumY;

		const double a31 = dSumX;
		const double a32 = dSumY;
		const double a33 = (double)nCount;

		//const double det = Det3x3(a11, a12, a13, a21, a22, a23, a31, a32, a33);
		const double det =
			a11 * (a22 * a33 - a23 * a32) -
			a12 * (a21 * a33 - a23 * a31) +
			a13 * (a21 * a32 - a22 * a31);

		if (std::fabs(det) < dDetEps)
			return false;

		const double invDet = 1.0 / det;

		const double invA11 = (a22 * a33 - a23 * a32) * invDet;
		const double invA12 = (a13 * a32 - a12 * a33) * invDet;
		const double invA13 = (a12 * a23 - a13 * a22) * invDet;

		const double invA21 = (a23 * a31 - a21 * a33) * invDet;
		const double invA22 = (a11 * a33 - a13 * a31) * invDet;
		const double invA23 = (a13 * a21 - a11 * a23) * invDet;

		const double invA31 = (a21 * a32 - a22 * a31) * invDet;
		const double invA32 = (a12 * a31 - a11 * a32) * invDet;
		const double invA33 = (a11 * a22 - a12 * a21) * invDet;

		vtCoef.resize(6);

		// X' = coef[0] * X + coef[1] * Y + coef[2]
		vtCoef[0] = invA11 * dSumXU + invA12 * dSumYU + invA13 * dSumU;
		vtCoef[1] = invA21 * dSumXU + invA22 * dSumYU + invA23 * dSumU;
		vtCoef[2] = (invA31 * dSumXU + invA32 * dSumYU + invA33 * dSumU) * dRate;

		// Y' = coef[3] * X + coef[4] * Y + coef[5]
		vtCoef[3] = invA11 * dSumXV + invA12 * dSumYV + invA13 * dSumV;
		vtCoef[4] = invA21 * dSumXV + invA22 * dSumYV + invA23 * dSumV;
		vtCoef[5] = (invA31 * dSumXV + invA32 * dSumYV + invA33 * dSumV) * dRate;

		return true;
	};

	auto CalcPointError2 = [&](int nIndex, const std::vector<double>& vtCoef) -> double
	{
		const TPOINT2D& ptSrc = (nType == 1) ? vtInputPoint2d[nIndex].first : vtInputPoint2d[nIndex].second;
		const TPOINT2D& ptDst = (nType == 1) ? vtInputPoint2d[nIndex].second : vtInputPoint2d[nIndex].first;

		const double dPredX = vtCoef[0] * ptSrc.x + vtCoef[1] * ptSrc.y + vtCoef[2];
		const double dPredY = vtCoef[3] * ptSrc.x + vtCoef[4] * ptSrc.y + vtCoef[5];

		const double dx = dPredX - ptDst.x;
		const double dy = dPredY - ptDst.y;

		return dx * dx + dy * dy;
	};

	std::vector<double> vtCoef;

	for (int nIter = 0; nIter <= nRecursiveCount; ++nIter)
	{
		if (vtUseIndex.size() < 3) { return false; }
		if (false == SolveAffine(vtUseIndex, vtCoef)) { return false; }
		double dMaxError2 = 0.0;
		int nMaxErrorPos = -1;

		for (int i = 0; i < (int)vtUseIndex.size(); ++i)
		{
			const int nIndex = vtUseIndex[i];
			const double dError2 = CalcPointError2(nIndex, vtCoef);

			if (dError2 > dMaxError2)
			{
				dMaxError2 = dError2;
				nMaxErrorPos = i;
			}
		}

		if (dMaxError2 <= dAllowError2)
		{
			vtdCoefficient = vtCoef;
			return true;
		}

		if (nIter >= nRecursiveCount)
		{
			vtdCoefficient = vtCoef;
			return true;
		}

		if (vtUseIndex.size() <= 3)
		{
			vtdCoefficient = vtCoef;
			return true;
		}

		if (nMaxErrorPos >= 0)
		{
			vtUseIndex[nMaxErrorPos] = vtUseIndex.back();
			vtUseIndex.pop_back();
		}
		else
		{
			vtdCoefficient = vtCoef;
			return true;
		}
	}

	vtdCoefficient = vtCoef;
	return !vtdCoefficient.empty();
}

//-------------------------------------------------------------------------------------//
void CAlgInspectionMeasureConnector::BuildInputGrid2D(const std::vector<TPOINT2D>& vtInput, double dCellSizeX, double dCellSizeY, std::unordered_map<long long, std::vector<TInputGridInfo2D> >& mapInputGrid)
{
	mapInputGrid.clear();
	if (dCellSizeX <= 0.0) { dCellSizeX = 1.0; }
	if (dCellSizeY <= 0.0) { dCellSizeY = 1.0; }

	mapInputGrid.reserve(vtInput.size() * 2);
	for (int i = 0; i < (int)vtInput.size(); ++i)
	{
		const double x = vtInput[i].x;
		const double y = vtInput[i].y;

		const int nGridX = std::floor(x / dCellSizeX);
		const int nGridY = std::floor(y / dCellSizeY);

		const long long key = MakeGridKey2D(nGridX, nGridY);

		TInputGridInfo2D info;
		info.nIndex = i;
		info.x = x;
		info.y = y;

		mapInputGrid[key].push_back(info);
	}
}
//-------------------------------------------------------------------------------------//
int CAlgInspectionMeasureConnector::MatchByShiftAndAngle_EndPoints(const std::vector<TPOINT2D>& vtSTD, const std::vector<TPOINT2D>& vtInput, const std::vector<TPOINT2D>& vtRotateSTD, const std::unordered_map<long long, std::vector<TInputGridInfo2D> >& mapInputGrid, double dOffsetX, double dOffsetY, double dMatchErrorX, double dMatchErrorY, std::vector<int>& vtnIndex)
{
	const int nCountSTD = (int)vtSTD.size();
	const int nCountInput = (int)vtInput.size();

	vtnIndex.assign(nCountSTD, -1);

	if (nCountSTD <= 0 || nCountInput <= 0) { return 0; }
	if (dMatchErrorX <= 0.0) { dMatchErrorX = 1.0; }
	if (dMatchErrorY <= 0.0) { dMatchErrorY = 1.0; }

	std::vector<unsigned char> vtUsedInput(nCountInput, 0);
	int nMatchCount = 0;

	for (int i = 0; i < nCountSTD; ++i)
	{
		const double dPredictX = vtRotateSTD[i].x - dOffsetX;
		const double dPredictY = vtRotateSTD[i].y - dOffsetY;

		const int nGridX = std::floor(dPredictX / dMatchErrorX);
		const int nGridY = std::floor(dPredictY / dMatchErrorY);

		int nBestInputIndex = -1;
		double dBestDist2 = DBL_MAX;

		for (int gy = nGridY - 1; gy <= nGridY + 1; ++gy)
		{
			for (int gx = nGridX - 1; gx <= nGridX + 1; ++gx)
			{
				const long long key = MakeGridKey2D(gx, gy);

				std::unordered_map<long long, std::vector<TInputGridInfo2D> >::const_iterator it = mapInputGrid.find(key);
				if (it == mapInputGrid.end()) { continue; }
				const std::vector<TInputGridInfo2D>& vtCandidate = it->second;
				for (int k = 0; k < (int)vtCandidate.size(); ++k)
				{
					const int nInputIndex = vtCandidate[k].nIndex;
					if (nInputIndex < 0 || nInputIndex >= nCountInput) { continue; }
					if (vtUsedInput[nInputIndex] != 0) { continue; }
					const double dx = dPredictX - vtCandidate[k].x;
					const double dy = dPredictY - vtCandidate[k].y;
					if (std::fabs(dx) > dMatchErrorX) { continue; }
					if (std::fabs(dy) > dMatchErrorY) { continue; }
					const double dDist2 = dx * dx + dy * dy;
					if (dDist2 < dBestDist2)
					{
						dBestDist2 = dDist2;
						nBestInputIndex = nInputIndex;
					}
				}
			}
		}
		if (nBestInputIndex >= 0)
		{
			vtnIndex[i] = nBestInputIndex;
			vtUsedInput[nBestInputIndex] = 1;
			++nMatchCount;
		}
	}
	return nMatchCount;
}
//-------------------------------------------------------------------------------------//
bool CAlgInspectionMeasureConnector::RoughMatchAngleByPCAEndPoints(TPOINT2D ImageCp, const std::vector<TPOINT2D>& vtSTD_EndPoint, const std::vector<TPOINT2D>& vtInput_EndPoint, double dAngleRange, double dAngleStep, double dOffsetBinX, double dOffsetBinY, double dMatchErrorX, double dMatchErrorY, int& nBestMatchCount, double& dBestAngle, double& dBestOffsetX, double& dBestOffsetY, std::vector<int>& vtnBestIndex)
{
	nBestMatchCount = 0;
	dBestAngle = 0.0;
	dBestOffsetX = 0.0;
	dBestOffsetY = 0.0;
	vtnBestIndex.clear();

	const int nCountSTD = (int)vtSTD_EndPoint.size();
	const int nCountInput = (int)vtInput_EndPoint.size();

	if (nCountSTD <= 0 || nCountInput <= 0) { return false; }
	if (dAngleRange < 0.0) { return false; }
	if (dAngleStep <= 0.0) { return false; }
	if (dOffsetBinX <= 0.0) { dOffsetBinX = 1.0; }
	if (dOffsetBinY <= 0.0) { dOffsetBinY = 1.0; }
	if (dMatchErrorX <= 0.0) { dMatchErrorX = 1.0; }
	if (dMatchErrorY <= 0.0) { dMatchErrorY = 1.0; }
	const double dRotateCenterX = ImageCp.x;
	const double dRotateCenterY = ImageCp.y;
	std::unordered_map<long long, std::vector<TInputGridInfo2D> > mapInputGrid;
	BuildInputGrid2D(vtInput_EndPoint, dMatchErrorX, dMatchErrorY, mapInputGrid);

	std::vector<TPOINT2D> vtRotateSTD;
	vtRotateSTD.resize(nCountSTD);

	std::vector<int> vtTempIndex;
	std::vector<int> vtCurrentBestIndex;

	int nCurrentBestMatchCount = -1;
	int nCurrentBestVoteCount = -1;

	double dCurrentBestAngle = 0.0;
	double dCurrentBestOffsetX = 0.0;
	double dCurrentBestOffsetY = 0.0;

	const int nAngleCount = (int)std::floor((dAngleRange * 2.0) / dAngleStep + 0.5) + 1;
	for (int nAngleIndex = 0; nAngleIndex < nAngleCount; ++nAngleIndex)
	{
		const double dAngle = -dAngleRange + (double)nAngleIndex * dAngleStep;
		if (dAngle > dAngleRange + 1e-9) { break; }
		const double dRad = dAngle * CV_PI / 180.0;
		const double dCosA = std::cos(dRad);
		const double dSinA = std::sin(dRad);

		for (int i = 0; i < nCountSTD; ++i)
		{
			const double dx = vtSTD_EndPoint[i].x - dRotateCenterX;
			const double dy = vtSTD_EndPoint[i].y - dRotateCenterY;

			vtRotateSTD[i].x = dx * dCosA - dy * dSinA + dRotateCenterX;
			vtRotateSTD[i].y = dx * dSinA + dy * dCosA + dRotateCenterY;
		}

		std::unordered_map<long long, TOffsetVoteInfo2D> mapOffsetVote;
		mapOffsetVote.reserve((size_t)nCountSTD * (size_t)nCountInput * 2);

		int nAngleBestVoteCount = 0;
		double dAngleBestOffsetX = 0.0;
		double dAngleBestOffsetY = 0.0;

		for (int i = 0; i < nCountSTD; ++i)
		{
			const double dRotateX = vtRotateSTD[i].x;
			const double dRotateY = vtRotateSTD[i].y;

			for (int j = 0; j < nCountInput; ++j)
			{
				const double dOffsetX = dRotateX - vtInput_EndPoint[j].x;
				const double dOffsetY = dRotateY - vtInput_EndPoint[j].y;

				const int nBinX = std::round(dOffsetX / dOffsetBinX);
				const int nBinY = std::round(dOffsetY / dOffsetBinY);

				const long long key = MakeGridKey2D(nBinX, nBinY);

				TOffsetVoteInfo2D& vote = mapOffsetVote[key];

				++vote.nCount;
				vote.dSumOffsetX += dOffsetX;
				vote.dSumOffsetY += dOffsetY;

				if (vote.nCount > nAngleBestVoteCount)
				{
					nAngleBestVoteCount = vote.nCount;
					dAngleBestOffsetX = vote.dSumOffsetX / (double)vote.nCount;
					dAngleBestOffsetY = vote.dSumOffsetY / (double)vote.nCount;
				}
			}
		}

		const int nMatchCount = MatchByShiftAndAngle_EndPoints(
			vtSTD_EndPoint,
			vtInput_EndPoint,
			vtRotateSTD,
			mapInputGrid,
			dAngleBestOffsetX,
			dAngleBestOffsetY,
			dMatchErrorX,
			dMatchErrorY,
			vtTempIndex);

		bool bBetter = false;

		if (nMatchCount > nCurrentBestMatchCount) { bBetter = true; }
		else if (nMatchCount == nCurrentBestMatchCount && nAngleBestVoteCount > nCurrentBestVoteCount) {
			bBetter = true;
		}
		if (bBetter)
		{
			nCurrentBestMatchCount = nMatchCount;
			nCurrentBestVoteCount = nAngleBestVoteCount;

			dCurrentBestAngle = dAngle;
			dCurrentBestOffsetX = dAngleBestOffsetX;
			dCurrentBestOffsetY = dAngleBestOffsetY;

			vtCurrentBestIndex = vtTempIndex;

			if (nCurrentBestMatchCount >= nCountSTD) { break; }
		}
	}

	if (nCurrentBestMatchCount <= 0)
	{
		nBestMatchCount = 0;
		dBestAngle = 0.0;
		dBestOffsetX = 0.0;
		dBestOffsetY = 0.0;
		vtnBestIndex.assign(nCountSTD, -1);
		return false;
	}

	nBestMatchCount = nCurrentBestMatchCount;
	dBestAngle = dCurrentBestAngle;
	dBestOffsetX = dCurrentBestOffsetX;
	dBestOffsetY = dCurrentBestOffsetY;
	vtnBestIndex = vtCurrentBestIndex;
	return true;

}
//-------------------------------------------------------------------------------------//
bool CAlgInspectionMeasureConnector::ParseRowColName(const CString & strName, int & nRow, int & nCol)
{
	nRow = -1;
	nCol = -1;

	int nRPos = strName.Find(_T('R'));
	int nCPos = strName.Find(_T('C'));

	if (nRPos < 0 || nCPos < 0 || nCPos <= nRPos) { return false; }

	CString strRow = strName.Mid(nRPos + 1, nCPos - nRPos - 1);
	CString strCol = strName.Mid(nCPos + 1);

	if (strRow.IsEmpty() || strCol.IsEmpty()) { return false; }

	nRow = _ttoi(strRow);
	nCol = _ttoi(strCol);

	if (nRow <= 0 || nCol <= 0)
	{
		nRow = -1;
		nCol = -1;
		return false;
	}

	return true;
}
//-------------------------------------------------------------------------------------//
TPOINT2D CAlgInspectionMeasureConnector::ApplyAffinePoint(const TPOINT2D& Pt, const std::vector<double>& vtdCoefficient)
{
	TPOINT2D OutPt;
	if (vtdCoefficient.size() < 6)
	{
		OutPt = Pt;
		return OutPt;
	}
	OutPt.x = vtdCoefficient[0] * Pt.x + vtdCoefficient[1] * Pt.y + vtdCoefficient[2];
	OutPt.y = vtdCoefficient[3] * Pt.x + vtdCoefficient[4] * Pt.y + vtdCoefficient[5];
	return OutPt;
}
//-------------------------------------------------------------------------------------//
bool CAlgInspectionMeasureConnector::CalcAffinePinTableRes(TALG_PARAM_MEASURE_CONNECTOR & mcParam, TPOINT2D RefPoint, bool bUseRefPt, std::vector<std::vector<TPOINT2D>>& vtStdPinInResCoord, TPOINT2D & OriginInResCoord)
{
	// 1. mc_PinPosTableRes 會被更新成「量測 pin 在私有座標系下的位置」。
	// 2. vtStdPinInResCoord 會輸出「標準 pin 在 result 座標系下的位置」，用來畫正確 pin。
	// 3. OriginInResCoord 會輸出「私有座標 O 點 (0,0) 在 result 座標系下的位置」。
	vtStdPinInResCoord.clear();
	OriginInResCoord.x = 0.0;
	OriginInResCoord.y = 0.0;

	if (mcParam.mc_PinPosTable.empty()) { return false; }
	if (mcParam.mc_PinPosTableRes.empty()) { return false; }
	if (mcParam.mc_PinPosTable.size() != mcParam.mc_PinPosTableRes.size()) { return false; }
	std::vector<std::pair<TPOINT2D, TPOINT2D> > vtInputPoint2d;
	vtInputPoint2d.reserve(128);

	for (size_t r = 0; r < mcParam.mc_PinPosTable.size(); ++r)
	{
		if (mcParam.mc_PinPosTable[r].size() != mcParam.mc_PinPosTableRes[r].size()) { return false; }

		for (size_t c = 0; c < mcParam.mc_PinPosTable[r].size(); ++c)
		{
			const TPOINT2D& StdPt = mcParam.mc_PinPosTable[r][c];
			const TPOINT2D& ResPt = mcParam.mc_PinPosTableRes[r][c];

			TPOINT2D ptStd(StdPt.x, StdPt.y);
			TPOINT2D ptRes(ResPt.x, ResPt.y);
			// first  = StdPt：標準 pin table 座標，也就是零件私有座標。
			// second = ResPt：原始量測 / result 座標。
			vtInputPoint2d.push_back(std::make_pair(ptStd, ptRes));
		}
	}
	// affine 至少需要 3 組有效點。
	if (vtInputPoint2d.size() < 3) { return false; }
	std::vector<double> vtdCoeffResToStd;
	std::vector<double> vtdCoeffStdToRes;

	const double dRate = 1.0;
	const int    nRecursiveCount = 5;
	const double dAllowError = 0.05;

	// nType = 2：second -> first
	// result / raw measure coordinate -> standard private coordinate
	// 把實際量到的 pin 從 result 座標轉成私有座標，之後拿來跟 mc_PinPosTable 比 dRTP。
	if (GetAffine(vtInputPoint2d, 2, dRate, nRecursiveCount, dAllowError, vtdCoeffResToStd) == false)
	{
		return false;
	}

	// nType = 1：first -> second
	// standard private coordinate -> result / raw measure coordinate
	// 把標準 pin table 與 O 點轉回 result 座標，用來畫圖。
	if (GetAffine(vtInputPoint2d, 1, dRate, nRecursiveCount, dAllowError, vtdCoeffStdToRes) == false)
	{
		return false;
	}

	// ApplyAffinePoint 需要 6 個 affine 係數。
	if (vtdCoeffResToStd.size() < 6) { return false; }
	if (vtdCoeffStdToRes.size() < 6) { return false; }
	// ------------------------------------------------------------
	// 1. 計算 RefPoint 經過 Res -> Std 後的位置。
	// RefPoint 是外部邊緣找出的 O 點，原始座標系應該跟 ResPt 相同。
	// 將它套用 Res -> Std，是為了讓它吃到跟 pin 一樣的旋轉 / 縮放。
	// ------------------------------------------------------------
	TPOINT2D RefAffinePoint;
	RefAffinePoint.x = 0.0;
	RefAffinePoint.y = 0.0;
	if (bUseRefPt) { RefAffinePoint = ApplyAffinePoint(RefPoint, vtdCoeffResToStd); }
	// ------------------------------------------------------------
	// 2. 更新 mc_PinPosTableRes。
	// 進入此函式前：  mc_PinPosTableRes = 原始量測 pin 座標，result coordinate。
	// 離開此函式後：  mc_PinPosTableRes = 量測 pin 在零件私有座標系下的位置。
	// 如果 bUseRefPt == true： PinPrivate = AffineResToStd(ResPin) - AffineResToStd(RefPoint)
	// 如果 bUseRefPt == false：PinPrivate = AffineResToStd(ResPin)
	// ------------------------------------------------------------
	for (size_t r = 0; r < mcParam.mc_PinPosTableRes.size(); ++r)
	{
		for (size_t c = 0; c < mcParam.mc_PinPosTableRes[r].size(); ++c)
		{
			const TPOINT2D ResPt = mcParam.mc_PinPosTableRes[r][c];
			TPOINT2D AffineResPt = ApplyAffinePoint(ResPt, vtdCoeffResToStd);
			if (bUseRefPt)
			{
				AffineResPt.x -= RefAffinePoint.x;
				AffineResPt.y -= RefAffinePoint.y;
			}
			mcParam.mc_PinPosTableRes[r][c] = AffineResPt;
		}
	}

	// ------------------------------------------------------------
	// 3. 計算畫圖用 O 點。
	//     私有座標系的 (0,0) -> result 座標系
	// ------------------------------------------------------------
	TPOINT2D OriginStd = { 0.0, 0.0 };
	OriginInResCoord = ApplyAffinePoint(OriginStd, vtdCoeffStdToRes);

	// ------------------------------------------------------------
	// 4. 計算畫圖用標準 pin 位置。
	// mc_PinPosTable 私有座標系轉成圖上的坐標系：
	//     O = (0,0)
	//     Pin = (x,y)
	// ------------------------------------------------------------
	vtStdPinInResCoord.resize(mcParam.mc_PinPosTable.size());
	for (size_t r = 0; r < mcParam.mc_PinPosTable.size(); ++r)
	{
		vtStdPinInResCoord[r].resize(mcParam.mc_PinPosTable[r].size());

		for (size_t c = 0; c < mcParam.mc_PinPosTable[r].size(); ++c)
		{
			const TPOINT2D& StdPrivatePt = mcParam.mc_PinPosTable[r][c];
			vtStdPinInResCoord[r][c] = ApplyAffinePoint(StdPrivatePt, vtdCoeffStdToRes);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgInspectionMeasureConnector::SearchFeaturePointAndApplyRoughAlignment(CAOIModel* ModelPtr, const TREGION4D& ModelRgn, const RECT& RoiRect, const RECT& WndRect, const std::vector<TUNI_FRAME>& UniFrameList)
{
	if (NULL == ModelPtr) { return false; }
	CAlgParam& AlgParam = m_Owner;
	CAlgBinaryParam AlgImageBinParam = AlgParam.GetAlgImageBinParam();

	const size_t FrameCount = UniFrameList.size();
	const size_t ImageFrameIndex = AlgImageBinParam.GetBinaryFrameIndex();
	if (ImageFrameIndex >= FrameCount) { return false; }

	TALG_PARAM_MEASURE_CONNECTOR& mcParam = AlgParam.GetAlgParamMeasureConnector();

	CAOIWnd* WndPtr = AlgParam.GetAlgWndPtr();
	if (NULL == WndPtr) { return false; }
	CString str;
	size_t i = 0, j = 0;

	const int nAlign = 4;
	const unsigned int WndIndex = WndPtr->GetWndIndex();

	const TUNI_FRAME* UniFramePtr = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount = UniFramePtr->BitCount;
	IMAGE_PTR FrameImagePtr = UniFramePtr->ImagePtr;
	TPOINT2D ImageScale = ModelPtr->GetModelImageScale();

	IMAGE_SIZE MaskW = 0;
	IMAGE_SIZE MaskH = 0;
	IMAGE_SIZE MaskStep = 0;
	IMAGE_SIZE MaskBitCount = 8;
	MASK_PTR MaskPtr = NULL;
	IMAGE_PTR GrayPtr = NULL;
	const bool bTestWnd = false;
	//#define _DEBUG
#ifdef _DEBUG
	bool bSave = true;
	CString ComponentName;
	CString DebugFolder = AlgParam.GetAlgDebugFolder();
	CAOIComponent* ComponentPtr = ModelPtr->GetModelComponentPtr();
	if (NULL != ComponentPtr)
	{
		ComponentName = ComponentPtr->GetComponentFullName();
	}

	if (true == bSave)
	{
		str.Format(_T("%s\\%s_ModelWndAlgMeasureConnector#%d_Model.PNG"), DebugFolder, ComponentName, WndIndex + 1);
		ImageAPI.SaveImage(str, FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameImagePtr, true);
	}

#endif // _DEBUG

	if (AlgParam.ExecAlgUniFrameBinary(AlgImageBinParam, WndRect, RoiRect, UniFrameList,
		MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, GrayPtr, bTestWnd) == false)
	{
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return false;
	}

	CJetBlob BlobDetector;
	BlobDetector.InitialBlob();
	BlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
	BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);

	if (BlobDetector.GrayImageRoiBlobDetect(MaskW, MaskH, MaskStep, MaskPtr, RoiRect, 128, 255) == false)
	{
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return false;
	}

	RECT BlobRect = { 0, 0, 0, 0 };
	CAOIBox ResBox;
	CAOIBox* BoxPtr = NULL;
	TBlobResult* BlobPtr = NULL;
	const size_t BlobResCount = BlobDetector.GetBlobCount();

	TPOINT2D RgnCp;
	TPOINT2D ImageCp;
	TREGION4D BoxRgn;

	ImageCp.x = MaskW * 0.5;
	ImageCp.y = MaskH * 0.5;
	RgnCp.x = ModelRgn.GetCpX();
	RgnCp.y = ModelRgn.GetCpY();

	const double ScaleX = 1.0 / ImageScale.x;
	const double ScaleY = 1.0 / ImageScale.y;

	const double BlobMaxW = mcParam.mc_nBlobWidthMax;
	const double BlobMinW = mcParam.mc_nBlobWidthMin;
	const double BlobMaxH = mcParam.mc_nBlobHeightMax;
	const double BlobMinH = mcParam.mc_nBlobHeightMin;

	bool bCalcPCA = false;
	double dSlope = 0.0;
	double dAngle = 0.0;
	TPOINT2D PCACenterPt;
	TPOINT2D TempPt1;
	TPOINT2D TempPt2;
	std::vector<TPOINT2D> PCAEndPtList;
	std::vector<TPOINT2D> RefPtList;

	int count = 0;
	for (i = 0; i < BlobResCount; ++i)
	{
		BlobPtr = BlobDetector.GetBlobPtr(i, false);
		if (NULL == BlobPtr) { continue; }

		BlobRect = BlobPtr->m_BlobRect; // 20230628-Blob
		double BlobW = BlobRect.right - BlobRect.left;
		double BlobH = BlobRect.bottom - BlobRect.top;

		const IMAGE_SIZE BlobImgW = (IMAGE_SIZE)BlobW;
		const IMAGE_SIZE BlobImgH = (IMAGE_SIZE)BlobH;

		BlobW *= ScaleX;
		BlobH *= ScaleY;
		if (BlobW > BlobMaxW) { continue; }
		if (BlobW < BlobMinW) { continue; }
		if (BlobH > BlobMaxH) { continue; }
		if (BlobH < BlobMinH) { continue; }

		const IMAGE_SIZE BlobRoiStep = JetAPI::GetBMPImagePixelsPerLine(BlobImgW, MaskBitCount, nAlign);
		IMAGE_PTR RoiBlob = NULL;
		ImageAPI.ExtractGrayRoiImage(MaskW, MaskH, MaskStep, MaskPtr, BlobRect, BlobRoiStep, RoiBlob, false);

#ifdef _DEBUG
		str.Format(_T("%s\\%s_ModelWndAlgMeasureConnect#%d_Blob.PNG"), DebugFolder, ComponentName, WndIndex + 1);
		ImageAPI.SaveImage(str, BlobImgW, BlobImgH, BlobRoiStep, MaskBitCount, RoiBlob, true);
#endif // _DEBUG

		if (bCalcPCA == false)
		{
			if (false == ImageAPI.ExecAnglePCA_Binary(BlobImgW, BlobImgH, BlobRoiStep, MaskBitCount,
				RoiBlob, dSlope, dAngle, PCACenterPt))
			{
				JetMemory.free_func(RoiBlob);
				JetMemory.free_func(MaskPtr);
				JetMemory.free_func(GrayPtr);
				return false;
			}
			dAngle = -dAngle;
			count++;
			if (count > 5) { bCalcPCA = true; }
		}

		//ImageAPI.ExecGetPCAAxisEndPoint_Binary(BlobImgW, BlobImgH, BlobRoiStep, MaskBitCount,
		//	RoiBlob, PCACenterPt, dAngle, TempPt1, TempPt2);

		CAOIModel::CalcModelBoxRectRegion(BlobRect, MaskW, MaskH, RgnCp, ImageScale, ImageCp, BoxRgn);
		//ResBox.SetBoxRegion(BoxRgn);
		const double dRad = dAngle * CV_PI / 180.0;
		const double dVecX = std::cos(dRad);
		const double dVecY = std::sin(dRad);
		double RegionW = BoxRgn.GetSizeX() / 4;
		double RegionH = BoxRgn.GetSizeY() / 4;

		double tX = DBL_MAX;
		double tY = DBL_MAX;

		if (std::abs(dVecX) > 1e-10) { tX = RegionW / std::abs(dVecX); }
		if (std::abs(dVecY) > 1e-10) { tY = RegionH / std::abs(dVecY); }
		const double t = std::min(tX, tY);
		TempPt1.x = BoxRgn.GetCpX() + dVecX * t;
		TempPt1.y = BoxRgn.GetCpY() + dVecY * t;
		TempPt2.x = BoxRgn.GetCpX() - dVecX * t;
		TempPt2.y = BoxRgn.GetCpY() - dVecY * t;

#ifdef _DEBUG
		ResBox.SetBoxRegion(BoxRgn);
		WndPtr->AddWndResultBox(ResBox);

		TREGION4D BoxTempRgn;
		BoxTempRgn.minX = MIN(TempPt1.x, BoxRgn.GetCpX());
		BoxTempRgn.maxX = MAX(TempPt1.x, BoxRgn.GetCpX());
		BoxTempRgn.minY = MIN(TempPt1.y, BoxRgn.GetCpY());
		BoxTempRgn.maxY = MAX(TempPt1.y, BoxRgn.GetCpY());
		ResBox.SetBoxRegion(BoxTempRgn);
		WndPtr->AddWndResultBox(ResBox);

		BoxTempRgn.minX = MIN(TempPt2.x, BoxRgn.GetCpX());
		BoxTempRgn.maxX = MAX(TempPt2.x, BoxRgn.GetCpX());
		BoxTempRgn.minY = MIN(TempPt2.y, BoxRgn.GetCpY());
		BoxTempRgn.maxY = MAX(TempPt2.y, BoxRgn.GetCpY());
		ResBox.SetBoxRegion(BoxTempRgn);
		WndPtr->AddWndResultBox(ResBox);
#endif // _DEBUG
		PCAEndPtList.push_back(TempPt1);
		PCAEndPtList.push_back(TempPt2);

		JetMemory.free_func(RoiBlob);
	}

	if (true == PCAEndPtList.empty()) {
		AlgParam.SetAlgResultID(RESULT_ID_NG);
		AlgParam.SetAlgResultText(_T("Unset binary Param"));
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return true;
	}
	if (mcParam.mc_nPinPosTableRow != mcParam.mc_PinPosTable.size())
	{
		AlgParam.SetAlgResultID(RESULT_ID_NG);
		AlgParam.SetAlgResultText(_T("Unset PinTable"));
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return true;
	}
	const double ModelAngle = ModelPtr->GetModelAttachedAngle();

	for (i = 0; i < mcParam.mc_nPinPosTableRow; ++i)
	{
		for (j = 0; j < mcParam.mc_nPinPosTableCol; ++j)
		{
			TPOINT2D pt;
			if (fabs(ModelAngle - 0) < 0.001) {
				pt = mcParam.mc_PinPosTable[i][j];
			}
			else if (fabs(ModelAngle - 90) < 0.001) {
				pt.x = -mcParam.mc_PinPosTable[i][j].y;
				pt.y = mcParam.mc_PinPosTable[i][j].x;
			}
			else if (fabs(ModelAngle - 180) < 0.001) {
				pt.x = -mcParam.mc_PinPosTable[i][j].x;
				pt.y = -mcParam.mc_PinPosTable[i][j].y;
			}
			else if (fabs(ModelAngle - 270) < 0.001) {
				pt.x = mcParam.mc_PinPosTable[i][j].y;
				pt.y = -mcParam.mc_PinPosTable[i][j].x;
			}
			pt.x = JetAPI::Unit_MMtoUM(pt.x);
			pt.y = JetAPI::Unit_MMtoUM(pt.y);
			RefPtList.push_back(pt);
		}
	}

	if (RefPtList.empty())
	{
		AlgParam.SetAlgResultID(RESULT_ID_NG);
		AlgParam.SetAlgResultText(_T("Unset PinTable"));
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return true;
	}

	int nBestMatchCount = 0;
	double dBestAngle = 0.0;
	double dBestOffsetX = 0.0;
	double dBestOffsetY = 0.0;
	std::vector<int> vtnBestIndex;

	// 粗略計算旋轉角度與偏移。
	TPOINT2D ImageCP = { 0,0 };
	double dAngleRange = 5.0;
	double dAngleStep = 0.1;
	double dOffsetBinX = ScaleX * 25;
	double dOffsetBinY = ScaleY * 25;
	double dMatchErrorX = ScaleX * 200;
	double dMatchErrorY = ScaleY * 200;
	const bool bRoughOK = RoughMatchAngleByPCAEndPoints(
		ImageCP, RefPtList, PCAEndPtList,
		dAngleRange, dAngleStep, dOffsetBinX, dOffsetBinY, dMatchErrorX, dMatchErrorY,
		nBestMatchCount, dBestAngle, dBestOffsetX, dBestOffsetY,
		vtnBestIndex);

	if (bRoughOK == false)
	{
		AlgParam.SetAlgResultID(RESULT_ID_NG);
		AlgParam.SetAlgResultText(_T("Rough Match Failed"));
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return true;
	}

	std::vector<std::pair<TPOINT2D, TPOINT2D> > vtInputPoint2d;
	for (i = 0; i < nBestMatchCount; i++) {
		TPOINT2D RfPoint = { RefPtList[i].x, RefPtList[i].y };
		TPOINT2D TgPoint = { PCAEndPtList[vtnBestIndex[i]].x, PCAEndPtList[vtnBestIndex[i]].y };
		vtInputPoint2d.push_back({ RfPoint ,TgPoint });
	}

	std::vector<double> vtdCoefficient;
	// nType = 1：first 轉 second，
	const int    nType = 1;
	const double dRate = 1.0;
	const int    nRecursiveCount = 5;
	const double dAllowError = 0.05;

	if (GetAffine(vtInputPoint2d, nType, dRate, nRecursiveCount, dAllowError, vtdCoefficient) == false)
	{
		AlgParam.SetAlgResultID(RESULT_ID_NG);
		AlgParam.SetAlgResultText(_T("GetAffine Failed"));
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return true;
	}

	double CadSkew_Self = 0;
	double CadOffsetX_Self = 0;
	double CadOffsetY_Self = 0;
	double CadSkew_Others = 0;
	double CadOffsetX_Others = 0;
	double CadOffsetY_Others = 0;
	const bool bChkDefect = false;
	AlgParam.SetAlgOffsetXReading(vtdCoefficient[2]);
	AlgParam.SetAlgOffsetYReading(vtdCoefficient[5]);
	AlgParam.CalcAlgOffsetL();
	AlgParam.CheckAlgOffset(CadOffsetX_Self, CadOffsetY_Self, CadSkew_Self, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, bChkDefect);
	vtdCoefficient[2] = 0;
	vtdCoefficient[5] = 0;

	BoxPtr = WndPtr->GetWndBoxPtr();
	BoxPtr->MoveBoxRes(CadOffsetX_Self, CadOffsetY_Self);
	WND_DEFECT_ID    WndDefectID = WndPtr->GetWndDefectID();
	WND_LOGIC_TYPE   WndLogicType = WndPtr->GetWndLogicType();
	if (AOIDataDefine.CheckWndDefectIDCanToAlign(WndDefectID) == true)
	{
		if (WND_LOGIC_NONE == WndLogicType)
		{
			if (ModelPtr->UpdateModelInspectionPosRes(WndPtr, CadOffsetX_Others, CadOffsetY_Others, CadSkew_Others, true) == false)
			{
				return false;
			}
		}
	}

	// 使用旋轉角度與偏移，從標準答案中選出候選區域。
#ifdef _DEBUG
	const int Width = mcParam.mc_nPinWidth;
	const int Height = mcParam.mc_nPinHeight;
	const int ExtendW = 200;
	const int ExtendH = 200;
#endif // _DEBUG

	CAOILand* LandPtr = NULL;
	CAOIWnd* LandWndPtr = NULL;
	CString LandName;
	const int LandCount = ModelPtr->GetModelLandCount();
	for (i = 0; i < RefPtList.size(); ++i)
	{
		const int nRow = (int)i / mcParam.mc_nPinPosTableCol;
		const int nCol = (int)i % mcParam.mc_nPinPosTableCol;
		str.Format(_T("R%dC%d"), nRow + 1, nCol + 1);

		LandPtr = NULL;
		for (j = 0; j < (size_t)LandCount; ++j)
		{
			LandPtr = ModelPtr->GetModelLandPtr(j, false);
			if (NULL == LandPtr) { continue; }
			LandName = LandPtr->GetLandName();
			if (LandName == str) { break; }
		}

		if (NULL == LandPtr) { continue; }

		TPOINT2D& TempPt = RefPtList[i];

		TPOINT2D AffinePoint = ApplyAffinePoint(TempPt, vtdCoefficient);

		//const double dx = TempPt.x - dCenterX;
		//const double dy = TempPt.y - dCenterY;

		//const double dRotX = dx * dCosA - dy * dSinA + dCenterX;
		//const double dRotY = dx * dSinA + dy * dCosA + dCenterY;

		//TempPt.x = dRotX - dBestOffsetX;
		//TempPt.y = dRotY - dBestOffsetY;

		//TempPt.x = JetAPI::Unit_MMtoUM(AffinePoint.x);
		//TempPt.y = JetAPI::Unit_MMtoUM(AffinePoint.y);
		TempPt = AffinePoint;


		BoxPtr = LandPtr->GetLandBoxPtr();
		if (NULL == BoxPtr) { continue; }

		BoxPtr->GetBoxPos(TempPt1);
		TempPt2.x = TempPt.x - TempPt1.x;
		TempPt2.y = TempPt.y - TempPt1.y;

		LandPtr->MoveLandLeadResult(TempPt2.x, TempPt2.y);
		LandPtr->MoveLandPadResult(TempPt2.x, TempPt2.y);
		BoxPtr = LandPtr->GetLandPadBoxPtr();
		/*if (NULL == BoxPtr) { continue; }
		ResBox = *BoxPtr;
		ResBox.SetBoxResultText(LandName);
		ResBox.SetBoxResultTextVisibled(true);
		WndPtr->AddWndResultBox(ResBox);*/

		const size_t LandWndCount = LandPtr->GetLandWndCount();
		for (j = 0; j < LandWndCount; ++j)
		{
			LandWndPtr = LandPtr->GetLandWndPtr(j, false);
			if (NULL == LandWndPtr) { continue; }

			BoxPtr = LandWndPtr->GetWndBoxPtr();
			if (NULL == BoxPtr) { continue; }
			BoxPtr->MoveBoxRes(TempPt2.x, TempPt2.y);

			if (true == LandWndPtr->GetWndExtendBoxUsed())
			{
				BoxPtr = LandWndPtr->GetWndExtendBoxPtr();
				if (NULL == BoxPtr) { continue; }
				BoxPtr->MoveBoxRes(TempPt2.x, TempPt2.y);
			}
		}

#ifdef _DEBUG
		TREGION4D BoxTempRgn;
		BoxTempRgn.minX = TempPt.x - Width / 2;
		BoxTempRgn.maxX = TempPt.x + Width / 2;
		BoxTempRgn.minY = TempPt.y - Height / 2;
		BoxTempRgn.maxY = TempPt.y + Height / 2;
		ResBox.SetBoxRegion(BoxTempRgn);
		ResBox.SetBoxResultID(RESULT_ID_OK);
		WndPtr->AddWndResultBox(ResBox);

		RECT SubWndRect;
		RECT SubRoiRect;
		CAOIModel::CalcModelBoxRegionRect(BoxTempRgn, MaskW, MaskH, RgnCp, ImageScale, ImageCp, SubWndRect);

		BoxTempRgn.SetSize(BoxTempRgn.GetSizeX() + ExtendW * 2, BoxTempRgn.GetSizeY() + ExtendH * 2);
		ResBox.SetBoxRegion(BoxTempRgn);
		ResBox.SetBoxResultID(RESULT_ID_NONE);
		WndPtr->AddWndResultBox(ResBox);

#endif // _DEBUG
	}
	AlgParam.SetAlgResultID(RESULT_ID_OK);
	AlgParam.SetAlgResultText(_T("OK"));
	JetMemory.free_func(MaskPtr);
	JetMemory.free_func(GrayPtr);

	return true;
}

//-------------------------------------------------------------------------------------//
bool CAlgInspectionMeasureConnector::SearchEightPointAndMeasureCenter(CAOIModel* ModelPtr, const TREGION4D& ModelRgn, const RECT& RoiRect, const RECT& WndRect, const std::vector<TUNI_FRAME>& UniFrameList, const std::vector<TUNI_FRAME>& WndUniFrameList)
{
	if (NULL == ModelPtr) { return false; }

	CAlgParam& AlgParam = m_Owner;
	CAlgBinaryParam AlgImageBinParam = AlgParam.GetAlgImageBinParam();

	const size_t FrameCount = UniFrameList.size();
	const size_t ImageFrameIndex = AlgImageBinParam.GetBinaryFrameIndex();
	if (ImageFrameIndex >= FrameCount) { return false; }

	TALG_PARAM_MEASURE_CONNECTOR& mcParam = AlgParam.GetAlgParamMeasureConnector();

	CAOIWnd* WndPtr = AlgParam.GetAlgWndPtr();
	if (NULL == WndPtr) { return false; }

	CString str;
	size_t i = 0;
	size_t j = 0;

	RECT WndRoiRect;
	RECT RoiWndRect = { 0, 0, 0, 0 };
	TREGION4D WndRegion;
	TREGION4D WndRoiRegion;

	CAOIWndRoi* WndRoiPtr = NULL;
	CAlgBinaryParam* BinaryParamPtr = NULL;

	const unsigned int WndIndex = WndPtr->GetWndIndex();
	const size_t WndRoiCount = WndPtr->GetWndRoiWndCount();

	const TUNI_FRAME* UniFramePtr = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH = UniFramePtr->ImageH;

	TPOINT2D ImageScale = ModelPtr->GetModelImageScale();

	const IMAGE_SIZE RoiW = RoiRect.right - RoiRect.left;

	IMAGE_SIZE MaskW = 0;
	IMAGE_SIZE MaskH = 0;
	IMAGE_SIZE MaskStep = 0;
	IMAGE_SIZE MaskBitCount = 8;
	MASK_PTR MaskPtr = NULL;
	IMAGE_PTR GrayPtr = NULL;
	const bool bTestWnd = false;

	CAOIBox ResBox;
	CAOIBox* WndBoxPtr = NULL;
	CAOIBox* WndRoiBoxPtr = NULL;

	TPOINT2D RgnCp;
	TPOINT2D ImageCp;
	ImageCp.x = FrameImageW * 0.5;
	ImageCp.y = FrameImageH * 0.5;
	RgnCp.x = ModelRgn.GetCpX();
	RgnCp.y = ModelRgn.GetCpY();

	const int BoxStartX = RoiRect.left;
	const int BoxStartY = RoiRect.top;

	std::vector<TPOINT2D> BoxPtList1(WndRoiCount);
	std::vector<TPOINT2D> BoxPtList2(WndRoiCount);
	std::vector<TPOINT2D> BoxCTPtByWndIndex(WndRoiCount);
	std::vector<unsigned char> vtWndRoiValid(WndRoiCount, 0);
	std::vector<std::pair<int, TPOINT2D> > BoxCTPtList;
	std::vector<std::pair<int, TPOINT2D> > BoxCTPtTempList;

#ifdef _DEBUG
	bool bSave = true;
	CString ComponentName;
	CString DebugFolder = AlgParam.GetAlgDebugFolder();
	CAOIComponent* ComponentPtr = ModelPtr->GetModelComponentPtr();
	if (NULL != ComponentPtr)
	{
		ComponentName = ComponentPtr->GetComponentFullName();
	}
#endif // _DEBUG

	WndPtr->GetWndExtendRegionRes(WndRegion);
	//WndPtr->GetWndRegionRes(WndRegion);
	JetAPI::SizeToRect(RoiW, RoiRect.bottom - RoiRect.top, RoiWndRect);

	for (i = 0; i < WndRoiCount; ++i)
	{
		WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
		if (NULL == WndRoiPtr) { continue; }

		WndRoiBoxPtr = WndRoiPtr->GetWndRoiBoxPtr();
		if (NULL == WndRoiBoxPtr) { continue; }

		WndRoiBoxPtr->GetBoxRegionRes(WndRoiRegion);
		if (JetAPI::CalcRegionRect(WndRegion, RoiWndRect, WndRoiRegion, WndRoiRect, true) == false)
		{
			continue;
		}

		BinaryParamPtr = WndRoiPtr->GetWndRoiBinaryParamPtr();
		if (NULL == BinaryParamPtr) { continue; }

		if (AlgParam.ExecAlgUniFrameBinary(*BinaryParamPtr, WndRoiRect, WndRoiRect, WndUniFrameList,
			MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, GrayPtr, bTestWnd) == false)
		{
			JetMemory.free_func(MaskPtr);
			JetMemory.free_func(GrayPtr);
			return false;
		}

#ifdef _DEBUG
		//if (true == bSave)
		//{
		//	str.Format(_T("%s\\%s_ModelWndAlgAngleMeasure#%d_Gray#%d.PNG"), DebugFolder, ComponentName, WndIndex + 1, i + 1);
		//	ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, true);
		//	str.Format(_T("%s\\%s_ModelWndAlgAngleMeasure#%d_Mask#%d.PNG"), DebugFolder, ComponentName, WndIndex + 1, i + 1);
		//	ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);
		//}
#endif // _DEBUG

		TLineEquation2D ImgLine;
		if (BINARY_DISABLE == BinaryParamPtr->GetBinaryMode())
		{
			if (mcParam.mc_EdgeFinder == LINE_FINDER_HOUGH) {
				ImageAPI.CalcGrayImageLineEquation2D_v2(MaskW, MaskH, MaskStep, GrayPtr, WndRoiRect, ImgLine);
			}
			else {
				ImageAPI.CalcGrayImageLineEquation2D(MaskW, MaskH, MaskStep, GrayPtr, WndRoiRect, ImgLine);
			}
		}
		else
		{
			if (mcParam.mc_EdgeFinder == LINE_FINDER_HOUGH) {
				ImageAPI.CalcGrayImageLineEquation2D_v2(MaskW, MaskH, MaskStep, GrayPtr, WndRoiRect, ImgLine);
			}
			else {
				ImageAPI.CalcGrayImageLineEquation2D(MaskW, MaskH, MaskStep, GrayPtr, WndRoiRect, ImgLine);
			}
		}

		TPOINT2D ImgPt1, ImgPt2;
		TPOINT2D BoxPt1, BoxPt2;

		if (fabs(ImgLine.a + 1) < 0.00001) // x = b*y + c
		{
			ImgPt1.y = WndRoiRect.top;
			ImgPt1.x = (ImgPt1.y * ImgLine.b) + ImgLine.c;

			ImgPt2.y = WndRoiRect.bottom;
			ImgPt2.x = (ImgPt2.y * ImgLine.b) + ImgLine.c;

			ImgPt1.x += BoxStartX;
			ImgPt1.y += BoxStartY;
			ImgPt2.x += BoxStartX;
			ImgPt2.y += BoxStartY;

			ModelPtr->CalcModelBoxPtPoint(ImgPt1, FrameImageW, FrameImageH, RgnCp, ImageScale, ImageCp, BoxPt2);
			ModelPtr->CalcModelBoxPtPoint(ImgPt2, FrameImageW, FrameImageH, RgnCp, ImageScale, ImageCp, BoxPt1);
		}
		else // y = a*x + c
		{
			ImgPt1.x = WndRoiRect.left;
			ImgPt1.y = (ImgPt1.x * ImgLine.a) + ImgLine.c;

			ImgPt2.x = WndRoiRect.right;
			ImgPt2.y = (ImgPt2.x * ImgLine.a) + ImgLine.c;

			ImgPt1.x += BoxStartX;
			ImgPt1.y += BoxStartY;
			ImgPt2.x += BoxStartX;
			ImgPt2.y += BoxStartY;

			ModelPtr->CalcModelBoxPtPoint(ImgPt1, FrameImageW, FrameImageH, RgnCp, ImageScale, ImageCp, BoxPt1);
			ModelPtr->CalcModelBoxPtPoint(ImgPt2, FrameImageW, FrameImageH, RgnCp, ImageScale, ImageCp, BoxPt2);
		}

		TPOINT2D CenterPt;
		CenterPt.x = (BoxPt1.x + BoxPt2.x) * 0.5;
		CenterPt.y = (BoxPt1.y + BoxPt2.y) * 0.5;

		BoxPtList1[i] = BoxPt1;
		BoxPtList2[i] = BoxPt2;
		BoxCTPtByWndIndex[i] = CenterPt;
		vtWndRoiValid[i] = 1;

		BoxCTPtList.push_back(std::make_pair((int)i, CenterPt));

		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
	}

	if (BoxCTPtList.size() < 4)
	{
		AlgParam.SetAlgResultID(RESULT_ID_NG);
		AlgParam.SetAlgResultText(_T("Measure point count NG"));
		WndPtr->SetWndResultID(AlgParam.GetAlgResultID());
		WndPtr->SetWndResultText(AlgParam.GetAlgResultText());
		return true;
	}

	BoxCTPtTempList = BoxCTPtList;

	std::vector<std::pair<int, int> > PairIndexList;
	std::sort(BoxCTPtList.begin(), BoxCTPtList.end(),
		[](const std::pair<int, TPOINT2D>& a, const std::pair<int, TPOINT2D>& b) { return a.second.x < b.second.x; });

	// left
	PairIndexList.push_back(std::make_pair(BoxCTPtList[0].first, BoxCTPtList[1].first));
	// right
	PairIndexList.push_back(std::make_pair(BoxCTPtList[BoxCTPtList.size() - 1].first, BoxCTPtList[BoxCTPtList.size() - 2].first));

	std::sort(BoxCTPtList.begin(), BoxCTPtList.end(),
		[](const std::pair<int, TPOINT2D>& a, const std::pair<int, TPOINT2D>& b) { return a.second.y < b.second.y; });

	// bottom
	PairIndexList.push_back(std::make_pair(BoxCTPtList[0].first, BoxCTPtList[1].first));
	// top
	PairIndexList.push_back(std::make_pair(BoxCTPtList[BoxCTPtList.size() - 1].first, BoxCTPtList[BoxCTPtList.size() - 2].first));

	BoxCTPtList = BoxCTPtTempList;
	BoxCTPtTempList.clear();

	RESULT_ID ResultID = RESULT_ID_OK;
	for (i = 0; i < WndRoiCount; ++i)
	{
		if (vtWndRoiValid[i] == 0) { continue; }

		RESULT_ID WndResultID = RESULT_ID_OK;
		WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
		if (NULL == WndRoiPtr) { continue; }

		WndRoiBoxPtr = WndRoiPtr->GetWndRoiBoxPtr();
		if (NULL == WndRoiBoxPtr) { continue; }

		ResBox = *WndRoiBoxPtr;
		ResBox.GetBoxRegion(WndRoiRegion);
		WndRoiRegion.SetSize(WndRoiRegion.GetSizeX() + 10, WndRoiRegion.GetSizeY() + 10);

		const bool bPtInRegion =
			true == WndRoiRegion.CheckPtInside(BoxPtList1[i]) ||
			true == WndRoiRegion.CheckPtInside(BoxPtList2[i]);

		if (false == bPtInRegion)
		{
			WndResultID = RESULT_ID_NG;
			ResultID = RESULT_ID_NG;
		}

		ResBox.AddBoxPolygonPt(BoxPtList1[i]);
		ResBox.AddBoxPolygonPt(BoxPtList2[i]);
		ResBox.AddBoxPolygonPt(BoxCTPtByWndIndex[i]);

		for (j = 0; j < PairIndexList.size(); ++j)
		{
			if ((int)i != PairIndexList[j].first) { continue; }
			ResBox.AddBoxPolygonPt(BoxCTPtByWndIndex[PairIndexList[j].second]);
			break;
		}

		ResBox.SetBoxResultID(WndResultID);
		ResBox.SetBoxPolygonVisibled(true);
		ResBox.SetBoxVisibled(true);
		WndPtr->AddWndResultBox(ResBox);
		ResBox.ClearBoxPolygon();
		ResBox.SetBoxPolygonVisibled(false);
	}

	WndBoxPtr = WndPtr->GetWndBoxPtr();
	if (NULL != WndBoxPtr)
	{
		ResBox = *WndBoxPtr;

		const double SumX =
			BoxCTPtByWndIndex[PairIndexList[0].first].x +
			BoxCTPtByWndIndex[PairIndexList[0].second].x +
			BoxCTPtByWndIndex[PairIndexList[1].first].x +
			BoxCTPtByWndIndex[PairIndexList[1].second].x;

		const double SumY =
			BoxCTPtByWndIndex[PairIndexList[2].first].y +
			BoxCTPtByWndIndex[PairIndexList[2].second].y +
			BoxCTPtByWndIndex[PairIndexList[3].first].y +
			BoxCTPtByWndIndex[PairIndexList[3].second].y;

		const double AvgX = SumX / 4.0;
		const double AvgY = SumY / 4.0;

		mcParam.mcPtXReading = AvgX;
		mcParam.mcPtYReading = AvgY;
		AlgParam.SetAlgOffsetXReading(AvgX);
		AlgParam.SetAlgOffsetYReading(AvgY);

		ResBox.AddBoxPolygonPt(TPOINT2D(AvgX, AvgY));
		ResBox.AddBoxPolygonPt(TPOINT2D(AvgX + 100, AvgY));
		ResBox.AddBoxPolygonPt(TPOINT2D(AvgX - 100, AvgY));
		ResBox.AddBoxPolygonPt(TPOINT2D(AvgX, AvgY));
		ResBox.AddBoxPolygonPt(TPOINT2D(AvgX, AvgY + 100));
		ResBox.AddBoxPolygonPt(TPOINT2D(AvgX, AvgY - 100));

		ResBox.SetBoxPolygonVisibled(true);
		ResBox.SetBoxVisibled(true);
		WndPtr->AddWndResultBox(ResBox);
		ResBox.ClearBoxPolygon();
		ResBox.SetBoxPolygonVisibled(false);
	}

	AlgParam.SetAlgResultID(ResultID);
	if (ResultID == RESULT_ID_OK) { AlgParam.SetAlgResultText(_T("OK")); }
	else { AlgParam.SetAlgResultText(_T("NG")); }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgInspectionMeasureConnector::CalcPinRelativePos(CAOIModel * ModelPtr)
{
	if (ModelPtr == NULL) { return false; }
	CAlgParam& AlgParam = m_Owner;
	CAOIWnd *WndPtr = AlgParam.GetAlgWndPtr();
	if (WndPtr == NULL) { return false; }
	int WndGroupID = WndPtr->GetWndGroupID();
	TALG_PARAM_MEASURE_CONNECTOR& mcParam = AlgParam.GetAlgParamMeasureConnector();
	size_t i = 0, j = 0;
	mcParam.InitResultTable();

	for (size_t r = 0; r < mcParam.mc_PinPosTableRes.size(); ++r)
	{
		for (size_t c = 0; c < mcParam.mc_PinPosTableRes[r].size(); ++c)
		{
			mcParam.mc_PinPosTableRes[r][c].x = -DBL_MAX;
			mcParam.mc_PinPosTableRes[r][c].y = -DBL_MAX;
		}
	}

	CString LandName, str;
	int nCol = 0;
	int nRow = 0;
	int nColIndex = -1;
	int nRowIndex = -1;

	CAOILand *LandPtr = NULL;
	CAOIBox  *BoxPtr = NULL;

	TPOINT2D PinPos;
	TPOINT2D RelatePos;
	TPOINT2D SpecPos;
	const size_t LandCount = ModelPtr->GetModelLandCount();
	RESULT_ID ResultID = RESULT_ID_OK;
	RESULT_ID BoxResultID = RESULT_ID_OK;
	const double DUSL = mcParam.mc_PtDUSL;
	double dRTP, dXDiff, dYDiff;
	int n_NGPIN = 0;
	for (i = 0; i < LandCount; ++i)
	{
		LandPtr = ModelPtr->GetModelLandPtr(i, false);
		if (LandPtr == NULL) { continue; }
		LandName = LandPtr->GetLandName();
		if (false == ParseRowColName(LandName, nRow, nCol)) { continue; }
		nRowIndex = nRow - 1;
		nColIndex = nCol - 1;
		if (nRowIndex < 0 || nColIndex < 0) { continue; }
		if (nRowIndex >= (int)mcParam.mc_PinPosTableRes.size()) { continue; }
		if (nRowIndex >= (int)mcParam.mc_PinPosTable.size()) { continue; }
		if (nColIndex >= (int)mcParam.mc_PinPosTableRes[nRowIndex].size()) { continue; }
		if (nColIndex >= (int)mcParam.mc_PinPosTable[nRowIndex].size()) { continue; }

		BoxPtr = LandPtr->GetLandLeadBoxPtr();
		if (BoxPtr == NULL) { continue; }
		BoxPtr->GetBoxPosRes(PinPos);
		// PinPos / m_RelativePt 為 um，mc_PinPosTable / mc_PinPosTableRes 使用 mm。
		RelatePos.x = JetAPI::Unit_UmtoMM(PinPos.x);
		RelatePos.y = JetAPI::Unit_UmtoMM(PinPos.y);

		// 先寫入原始量測相對座標。
		mcParam.mc_PinPosTableRes[nRowIndex][nColIndex] = RelatePos;
	}
	TPOINT2D RefPoint;
	RefPoint.x = JetAPI::Unit_UmtoMM(mcParam.mcPtXReading);
	RefPoint.y = JetAPI::Unit_UmtoMM(mcParam.mcPtYReading);
	// mc_bUseEdge == true 時，使用邊緣量測出的 RefPoint 做補償
	bool bUseRefPoint = mcParam.mc_bUseEdge;

	std::vector<std::vector<TPOINT2D> > vtStdPinInResCoord;
	TPOINT2D OriginInResCoord;

	if (CalcAffinePinTableRes(mcParam, RefPoint, bUseRefPoint, vtStdPinInResCoord, OriginInResCoord) == false)
	{
		// 函式內部會使用Affine修改mc_PinPosTableRes
		AlgParam.SetAlgResultID(RESULT_ID_NG);
		AlgParam.SetAlgResultText(_T("Affine Failed"));
		WndPtr->SetWndResultID(AlgParam.GetAlgResultID());
		WndPtr->SetWndResultText(AlgParam.GetAlgResultText());
		return true;
	}
	OriginInResCoord.x = JetAPI::Unit_MMtoUM(OriginInResCoord.x);
	OriginInResCoord.y = JetAPI::Unit_MMtoUM(OriginInResCoord.y);
	CAOIWnd *LandWndPtr = NULL;
	TPOINT2D SpecPosDraw;
	CAOIBox Resbox;
	for (i = 0; i < LandCount; ++i)
	{
		LandPtr = ModelPtr->GetModelLandPtr(i, false);
		if (LandPtr == NULL) { continue; }
		LandName = LandPtr->GetLandName();
		if (false == ParseRowColName(LandName, nRow, nCol)) { continue; }
		nRowIndex = nRow - 1;
		nColIndex = nCol - 1;
		if (nRowIndex < 0 || nColIndex < 0) { continue; }
		if (nRowIndex >= (int)mcParam.mc_PinPosTableRes.size()) { continue; }
		if (nColIndex >= (int)mcParam.mc_PinPosTableRes[nRowIndex].size()) { continue; }
		SpecPos = mcParam.mc_PinPosTable[nRowIndex][nColIndex];
		RelatePos = mcParam.mc_PinPosTableRes[nRowIndex][nColIndex];
		SpecPosDraw = vtStdPinInResCoord[nRowIndex][nColIndex];
		SpecPosDraw.x = JetAPI::Unit_MMtoUM(SpecPosDraw.x);
		SpecPosDraw.y = JetAPI::Unit_MMtoUM(SpecPosDraw.y);

		dXDiff = SpecPos.x - RelatePos.x;
		dYDiff = SpecPos.y - RelatePos.y;
		dRTP = sqrt(dXDiff * dXDiff + dYDiff * dYDiff);

		BoxResultID = (dRTP <= DUSL) ? RESULT_ID_OK : RESULT_ID_NG;
		if (dRTP <= DUSL) { str.Format(L"OK"); }
		else {
			str.Format(L"NG [dRTP: %.3f>%.3f]", dRTP, DUSL);
		}
		n_NGPIN += (BoxResultID == RESULT_ID_NG) ? 1 : 0;

		BoxPtr = LandPtr->GetLandPadBoxPtr();
		Resbox = *BoxPtr;
		if (BoxPtr == NULL) { continue; }
		BoxPtr->GetBoxPosRes(PinPos);
		SpecPosDraw.x -= PinPos.x;	SpecPosDraw.y -= PinPos.y;
		LandPtr->MoveLandPadResult(SpecPosDraw.x, SpecPosDraw.y);
		Resbox.MoveBoxRes(SpecPosDraw.x, SpecPosDraw.y);
		Resbox.SetBoxResultText(LandName);
		Resbox.SetBoxResultTextVisibled(true);

		const size_t LandWndCount = LandPtr->GetLandWndCount();
		int FrameIndex = 0;
		for (j = 0; j < LandWndCount; j++) {
			LandWndPtr = LandPtr->GetLandWndPtr(j, false);
			if (LandWndPtr == NULL) { continue; }
			if (LandWndPtr->GetWndDefectID() == WND_DEFECT_LEAD_ADJUST) {
				const CAlgParam &LandWndAlgParam = LandWndPtr->GetWndAlgParam();
				const CAlgBinaryParam LandWndBinaryParam = LandWndAlgParam.GetAlgImageBinParam();
				FrameIndex = LandWndBinaryParam.GetBinaryFrameIndex();
			}
			if (LandWndPtr->GetWndAlgType() != ALG_MEASURE_CONNECTOR) { continue; }
			LandWndPtr->SetWndResultID(BoxResultID);
			LandWndPtr->SetWndResultText(str);
			LandWndPtr->SetWndAlgImageFrameIndex(FrameIndex);
			LandWndPtr->MoveWndResult(SpecPosDraw.x, SpecPosDraw.y);
			Resbox.SetBoxResultID(BoxResultID);
			LandWndPtr->AddWndResultBox(Resbox);
		}
	}

	// 畫出使用仿射運算所計算出的中心點。
	TREGION4D ResRegion;
	BoxPtr = WndPtr->GetWndBoxPtr();
	BoxPtr->GetBoxRegionRes(ResRegion);
	Resbox = *BoxPtr;

	Resbox.SetBoxRegion(ResRegion);
	Resbox.AddBoxPolygonPt(OriginInResCoord);
	Resbox.AddBoxPolygonPt(TPOINT2D(OriginInResCoord.x + 100, OriginInResCoord.y + 100));
	Resbox.AddBoxPolygonPt(TPOINT2D(OriginInResCoord.x - 100, OriginInResCoord.y - 100));
	Resbox.AddBoxPolygonPt(OriginInResCoord);
	Resbox.AddBoxPolygonPt(TPOINT2D(OriginInResCoord.x - 100, OriginInResCoord.y + 100));
	Resbox.AddBoxPolygonPt(TPOINT2D(OriginInResCoord.x + 100, OriginInResCoord.y - 100));
	Resbox.SetBoxPolygonVisibled(true);
	Resbox.SetBoxVisibled(true);
	WndPtr->AddWndResultBox(Resbox);
	Resbox.ClearBoxPolygon();
	Resbox.SetBoxPolygonVisibled(false);


	if (n_NGPIN > 0) { ResultID = RESULT_ID_NG; }
	const int PinCount = mcParam.mc_nPinPosTableRow*mcParam.mc_nPinPosTableCol;
	if (ResultID == RESULT_ID_OK) { str = _T("OK"); }
	else { str.Format(_T("dRTP NG [%d/%d]"), n_NGPIN, PinCount); }
	// ResultID 強制設為OK 讓NG顯示在個別PIN上，送到RSM的NG零件就只會顯示 NG PIN。。
	ResultID = RESULT_ID_OK;
	AlgParam.SetAlgResultID(ResultID);
	AlgParam.SetAlgResultText(str);
	if (WndPtr->GetWndResultID() == RESULT_ID_OK) {
		WndPtr->SetWndResultID(AlgParam.GetAlgResultID());
		WndPtr->SetWndResultText(AlgParam.GetAlgResultText());
	}
	/*if (MainWnd->GetWndResultID() == RESULT_ID_OK) {
	MainWnd->SetWndResultID(AlgParam.GetAlgResultID());
	MainWnd->SetWndResultText(AlgParam.GetAlgResultText());
	}*/
	return true;
}
//-------------------------------------------------------------------------------------//
