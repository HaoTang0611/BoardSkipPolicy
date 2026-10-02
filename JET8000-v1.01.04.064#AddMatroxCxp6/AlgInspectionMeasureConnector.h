#pragma once

#include <vector>
#include <utility>
#include <unordered_map>
//-------------------------------------------------------------------------------------//
class CAlgInspectionMeasureConnector
{
public:
	explicit CAlgInspectionMeasureConnector(CAlgParam& Owner);
	//-------------------------------------------------------------------------------------//
public:
	bool SearchFeaturePointAndApplyRoughAlignment(CAOIModel* ModelPtr, const TREGION4D& ModelRgn, const RECT& RoiRect, const RECT& WndRect, const std::vector<TUNI_FRAME>& UniFrameList);
	// 搜尋 Pin/Connector 特徵點，與 mc_PinPosTable 參考答案做粗略角度/位移匹配，
	// 並把 Land / LandWnd / ExtendBox 移動到推估位置。
	// Deprecated
	bool SearchEightPointAndMeasureCenter(CAOIModel* ModelPtr, const TREGION4D& ModelRgn, const RECT& RoiRect, const RECT& WndRect, const std::vector<TUNI_FRAME>& UniFrameList, const std::vector<TUNI_FRAME>& WndUniFrameList);
	// 搜尋 8 個 WndRoi 點位/線段，並計算中心點 Reading。
	bool CalcPinRelativePos(CAOIModel* ModelPtr);
	// 使用找到的 Land 位置與中心點 Reading 計算出相對位置
	// 再用 mc_PinPosTable 與 mc_PinPosTableRes 計算 affine
	// 將標準 pin 表轉成 affine-fit 後的結果 pin 表
	//-------------------------------------------------------------------------------------//
private:
	CAlgParam& m_Owner;
	TPOINT2D m_RelativePt;
	//-------------------------------------------------------------------------------------//
private:
	struct TInputGridInfo2D
	{
		int nIndex;
		double x;
		double y;
		TInputGridInfo2D()
		{
			nIndex = -1;
			x = 0.0;
			y = 0.0;
		}
	};
	struct TOffsetVoteInfo2D
	{
		int nCount;
		double dSumOffsetX;
		double dSumOffsetY;

		TOffsetVoteInfo2D()
		{
			nCount = 0;
			dSumOffsetX = 0.0;
			dSumOffsetY = 0.0;
		}
	};
	//-------------------------------------------------------------------------------------//
private:
	static long long           MakeGridKey2D(int x, int y);
	//-------------------------------------------------------------------------------------//
	static bool		GetAffine(const std::vector<std::pair<TPOINT2D, TPOINT2D> >& vtInputPoint2d, int nType, double dRate, int nRecursiveCount, double dAllowError, std::vector<double>& vtdCoefficient);
	// nType : 1 => first  轉 second
	// nType : 2 => second 轉 first
	// nRecursiveCount : 最多剃除異常值次數
	// dAllowError     : 誤差容許值，單位與輸入點相同
	static void		           BuildInputGrid2D(const std::vector<TPOINT2D>& vtInput, double dCellSizeX, double dCellSizeY, std::unordered_map<long long, std::vector<TInputGridInfo2D> >& mapInputGrid);
	static int		           MatchByShiftAndAngle_EndPoints(const std::vector<TPOINT2D>& vtSTD, const std::vector<TPOINT2D>& vtInput, const std::vector<TPOINT2D>& vtRotateSTD, const std::unordered_map<long long, std::vector<TInputGridInfo2D> >& mapInputGrid, double dOffsetX, double dOffsetY, double dMatchErrorX, double dMatchErrorY, std::vector<int>& vtnIndex);
	static bool		           RoughMatchAngleByPCAEndPoints(TPOINT2D ImageCp, const std::vector<TPOINT2D>& vtSTD_EndPoint, const std::vector<TPOINT2D>& vtInput_EndPoint, double dAngleRange, double dAngleStep, double dOffsetBinX, double dOffsetBinY, double dMatchErrorX, double dMatchErrorY, int& nBestMatchCount, double& dBestAngle, double& dBestOffsetX, double& dBestOffsetY, std::vector<int>& vtnBestIndex);
	bool                       ParseRowColName(const CString& strName, int& nRow, int& nCol);
	TPOINT2D                   ApplyAffinePoint(const TPOINT2D& Pt, const std::vector<double>& vtdCoefficient);
	bool                       CalcAffinePinTableRes(TALG_PARAM_MEASURE_CONNECTOR& mcParam, TPOINT2D RefPoint, bool bUseRefPt, std::vector<std::vector<TPOINT2D> >& vtStdPinInResCoord, TPOINT2D& OriginInResCoord);
};
