// AlgParam_MeasureConnector.cpp: implementation of the CAlgParam class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AlgParam.h"
#include "AlgInspectionMeasureConnector.h"
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_MeasureConnector(CAOIModel* ModelPtr, const TREGION4D& ModelRgn, const RECT& RoiRect, const RECT& WndRect, const std::vector<TUNI_FRAME>& UniFrameList)
{
	CAOIWnd* WndPtr = GetAlgWndPtr();
	if (NULL == WndPtr) { return false; }
	CAOILand *LandPtr = WndPtr->GetWndLandPtr();
	if (NULL != LandPtr) { return true; }
	CAlgInspectionMeasureConnector MeasureConnector(*this);
	ALG_TYPE AlgType = GetAlgType();
	const WND_DEFECT_ID WndDefectID = WndPtr->GetWndDefectID();
	TALG_PARAM_MEASURE_CONNECTOR& mcParam = GetAlgParamMeasureConnector();
	bool bResult;

	if (WndDefectID == WND_DEFECT_PART_ALIGN) {
		// 第 1 段：搜尋特徵點，計算粗略角度/偏移，並移動 Land/Window 結果。
		// 不使用，用原本的零件定位更穩定。
		bResult = MeasureConnector.SearchFeaturePointAndApplyRoughAlignment(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);
	}
	else {
		std::vector<TUNI_FRAME> WndUniFrameList;
		if (BuildWndUniFrameList(UniFrameList, RoiRect, WndUniFrameList) == false) { return false; }
		// 第 2 段：使用已經建立好的 WndUniFrameList 搜尋 8 個點位並計算中心點。
		bResult = MeasureConnector.SearchEightPointAndMeasureCenter(
			ModelPtr,
			ModelRgn,
			RoiRect,
			WndRect,
			UniFrameList,
			WndUniFrameList);
		JetAPI::ClearUniFrameList(WndUniFrameList);
		// 第 3 段：計算針腳位置
		bResult = MeasureConnector.CalcPinRelativePos(ModelPtr);
	}
	WndPtr->SetWndResultID(GetAlgResultID());
	WndPtr->SetWndResultText(GetAlgResultText());
	return bResult;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_MeasureConnector(const TALG_PARAM_MEASURE_CONNECTOR & Param, CAOIFileIO & FileIO)
{
	size_t nRow, nCol;
	bool bChild = Param.mc_bChild;
	// 儲存開始標記
	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_START, 0)) return false;
	if (bChild == true) {
		if (false == FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_CHILD, Param.mc_bChild)) return false;
		if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_END, 0)) return false;// 結束標記
		return true;
	}

	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_ROW_COUNT, Param.mc_nPinPosTableRow)) return false;
	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_COL_COUNT, Param.mc_nPinPosTableCol)) return false;

	// TABLE Start
	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_START, 0)) return false;
	for (nRow = 0; nRow < Param.mc_nPinPosTableRow; nRow++) {
		for (nCol = 0; nCol < Param.mc_nPinPosTableCol; nCol++) {
			if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_ROW_INDEX, nRow)) return false;
			if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_COL_INDEX, nCol)) return false;
			if (false == FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_PIN_X, Param.mc_PinPosTable[nRow][nCol].x)) return false;
			if (false == FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_PIN_Y, Param.mc_PinPosTable[nRow][nCol].y)) return false;
		}
	}
	// TABLE End
	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_END, 0)) return false;
	// 其他設定

	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_PIN_WIDTH, Param.mc_nPinWidth)) return false;
	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_PIN_HEIGHT, Param.mc_nPinHeight)) return false;
	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_BLOB_W_MIN, Param.mc_nBlobWidthMin)) return false;
	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_BLOB_H_MIN, Param.mc_nBlobHeightMin)) return false;
	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_BLOB_W_MAX, Param.mc_nBlobWidthMax)) return false;
	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_BLOB_H_MAX, Param.mc_nBlobHeightMax)) return false;
	if (false == FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_PIN_D_USL, Param.mc_PtDUSL)) return false;
	if (false == FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_USE_EDGE, Param.mc_bUseEdge)) return false;
	if (false == FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_AUTO_EDGE, Param.mc_EdgeFinder)) return false;
	if (false == FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_CHILD, Param.mc_bChild)) return false;

	// 結束標記
	if (false == FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_END, 0)) return false;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_MeasureConnector(TALG_PARAM_MEASURE_CONNECTOR & Param, CAOIFileIO & FileIO)
{

	int index = 0;
	int RowIdx = 0, ColIdx = 0;
	while (false == FileIO.CheckFileEnd())
	{
		if (false == FileIO.LoadChunk(index))
		{
			continue;
		}
		switch (index)
		{
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_ROW_COUNT:
			Param.mc_nPinPosTableRow = (FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_COL_COUNT:
			Param.mc_nPinPosTableCol = (FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_START:
			Param.InitPinTable();
			Param.InitResultTable();
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_END:
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_COL_INDEX:
			ColIdx = (FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_ROW_INDEX:
			RowIdx = (FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_PIN_X:
			Param.mc_PinPosTable[RowIdx][ColIdx].x = (FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_PIN_Y:
			Param.mc_PinPosTable[RowIdx][ColIdx].y = (FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_PIN_WIDTH:
			Param.mc_nPinWidth = (FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_PIN_HEIGHT:
			Param.mc_nPinHeight = (FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_BLOB_W_MIN:
			Param.mc_nBlobWidthMin = (FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_BLOB_H_MIN:
			Param.mc_nBlobHeightMin = (FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_BLOB_W_MAX:
			Param.mc_nBlobWidthMax = (FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_BLOB_H_MAX:
			Param.mc_nBlobHeightMax = (FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_PIN_D_USL:
			Param.mc_PtDUSL = (FileIO.GetData_DBL());
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_USE_EDGE:
			Param.mc_bUseEdge = (FileIO.GetData_BOL());
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_AUTO_EDGE:
			Param.mc_EdgeFinder = (LINE_FINDER)(FileIO.GetData_BOL());
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_CHILD:
			Param.mc_bChild = (FileIO.GetData_BOL());
			break;
		case FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_END: // 結束標記
			return true;


		default://v1.01.04.061
#ifdef _DEBUG
			index = index;
#endif//_DEBUG
			break;
		}
	}

	return true;
}