//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AlgParam.h"
//-------------------------------------------------------------------------------------//
#include "AOIWnd.h"
#include "AOILand.h"
#include "AOIModel.h"
#include "AOIFileIO.h"
#include "JetMatch.h"
#include "JetBlob.h"
#include "JetBarcode.h"
#include "JetAlg\JETAlg_Inc.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_Height(const TALG_PARAM_RESIN_HEIGHT &Param, CAOIFileIO &FileIO)
{
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_HEIGHT_START, 0) == false) { return false; }

	if (FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_HEIGHT_ENABLED, Param.enabled) == false) { return false; }
	if (FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_HEIGHT_NG_CHECK, Param.enableDoubleCheck) == false) { return false; }

	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_HEIGHT_RESIN_TIN_TYPE, Param.nInspectionType) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_HEIGHT_DIRECTION, Param.nDirection) == false) { return false; }

	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_HEIGHT_RANGE, Param.nMeasureRange) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_HEIGHT_MEASURE_MODE, Param.nPartHeightMeasureMode) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_HEIGHT_VALUE, Param.nPartHeightThreshold) == false) { return false; }

	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_HEIGHT_OUTPUT_TYPE, Param.nOutputType) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_HEIGHT_STEPZ, Param.nStepZ) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_HEIGHT_POSITION_SHIFT, Param.nShift_Tin) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_HEIGHT_THRESHOLDZ_WIDTH, Param.nThresholdZ_Width) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_HEIGHT_THRESHOLDZ_HEIGHT, Param.nThresholdZ_Height) == false) { return false; }

	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_HEIGHT_END, 0) == false) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_Wire(const TALG_PARAM_WIRE_WIDTH &Param, CAOIFileIO &FileIO)
{
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_WIRE_START, 0) == false) { return false; }

	if (FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_HEIGHT_ENABLED, Param.bFilter) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_WIRE_WIDTH_FILTER_SIZE, Param.nFilterSize) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_WIRE_WIDTH_EDGE_LOW_THRESHOLD, Param.nLowThres) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_WIRE_WIDTH_EDGE_HIGHT_THRESHOLD, Param.nHeightThres) == false) { return false; }
	if (FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_WIRE_WIDTH_VALUE_USL, Param.widthUSL) == false) { return false; }
	if (FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_WIRE_WIDTH_VALUE_LSL, Param.widthLSL) == false) { return false; }

	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_WIRE_END, 0) == false) { return false; }

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_Height(TALG_PARAM_RESIN_HEIGHT &Param, CAOIFileIO &FileIO)//載入
{
	int       index = 0;
	int       nValue = 0;
	while (FileIO.CheckFileEnd() == false)
	{
		if (FileIO.LoadChunk(index) == false)
		{
			continue;
		}
		switch (index)
		{
		case FILE_IO_ALG_PARAM_HEIGHT_END:
			return true;
			break;
		case FILE_IO_ALG_PARAM_HEIGHT_ENABLED:
			Param.enabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_HEIGHT_NG_CHECK:
			Param.enableDoubleCheck = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_HEIGHT_RESIN_TIN_TYPE:
			Param.nInspectionType = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_HEIGHT_DIRECTION:
			Param.nDirection = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_HEIGHT_RANGE://
			Param.nMeasureRange = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_HEIGHT_MEASURE_MODE:
			Param.nPartHeightMeasureMode = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_HEIGHT_OUTPUT_TYPE:
			Param.nOutputType = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_HEIGHT_VALUE://
			Param.nPartHeightThreshold = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_HEIGHT_STEPZ:
			Param.nStepZ = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_HEIGHT_POSITION_SHIFT:
			Param.nShift_Tin = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_HEIGHT_THRESHOLDZ_WIDTH:
			Param.nThresholdZ_Width = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_HEIGHT_THRESHOLDZ_HEIGHT:
			Param.nThresholdZ_Height = FileIO.GetData_INT();
			break;
		default:
#ifdef _DEBUG
			index = index;
#endif//_DEBUG
			break;
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_Height Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_Wire(TALG_PARAM_WIRE_WIDTH &Param, CAOIFileIO &FileIO)//載入
{
	int       index = 0;
	int       nValue = 0;
	while (FileIO.CheckFileEnd() == false)
	{
		if (FileIO.LoadChunk(index) == false)
		{
			continue;
		}
		switch (index)
		{
		case FILE_IO_ALG_PARAM_WIRE_END:
			return true;
			break;
		case FILE_IO_ALG_PARAM_WIRE_WIDTH_FILTER_ENABLED:
			Param.bFilter = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_WIRE_WIDTH_FILTER_SIZE:
			Param.nFilterSize = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_WIRE_WIDTH_EDGE_LOW_THRESHOLD:
			Param.nLowThres = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_WIRE_WIDTH_EDGE_HIGHT_THRESHOLD:
			Param.nHeightThres = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_WIRE_WIDTH_VALUE_USL:
			Param.widthUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_WIRE_WIDTH_VALUE_LSL:
			Param.widthLSL = FileIO.GetData_DBL();
			break;
		default:
#ifdef _DEBUG
			index = index;
#endif//_DEBUG
			break;
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_Wire Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_Height(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	if (NULL == ModelPtr) { return false; }

	CAOIWnd *WndPtr = GetAlgWndPtr();
	
	if (NULL == WndPtr) { return false; }
	
	const size_t FrameCount = UniFrameList.size();
	TFrameParam *FrameParamPtr = NULL;
	size_t  i = 0, j = 0;
	for (i = 0; i<FrameCount; i++)
	{
		if ( FRAME_UNIQUE_ID_TOP == UniFrameList[i].FrameUniqueID ) { break; }
		/*
		FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(UniFrameList[i].FrameUniqueID);
		if (NULL == FrameParamPtr) { continue; }
		if (FrameParamPtr->FrameName == _T("高角度燈")) { break; }		
		*/
	}
	for (j = 0; j < FrameCount; j++)
	{
		if (UniFrameList[j].SpacePtr != NULL) { break; }
	}
	if (j== FrameCount || i == FrameCount) { return false; }
	const size_t Index_2D = i;
	const size_t Index_3D = j;	

	TALG_PARAM_RESIN_HEIGHT &Param = CAlgParam::GetAlgParamResinHeight();
	Param.resultID = 0;
	CAOIBox *body = ModelPtr->GetModelBodyBoxPtr();

	TPOINT2D scale;
	const BOX_TOWARD bodyToward = body->GetBoxToward();
	const BOX_TOWARD padToward = WndPtr->GetWndToward();
	RECT wndROI;
	WndPtr->GetWndImageRect(wndROI);

	ModelPtr->GetModelImageScale(scale);

	std::vector<float> boxHeight = body->GetBoxHeight();
	std::vector<float> EdgeHeight = body->GetBoxEdgePadHeight();

	if (WndPtr->GetWndDefectID() == WND_DEFECT_PART_ALIGN || boxHeight.size() == 0) // 分辨零件定位(其他瑕疵只顯示結果，沒高度就重算)
	{
		if (WndPtr->GetWndDefectID() == WND_DEFECT_PART_ALIGN && !Param.enabled) { return true; }
		IMAGE_SIZE		 MaskBitCount = UniFrameList[Index_3D].BitCount;

		IMAGE_SIZE		 MaskW = UniFrameList[Index_3D].ImageW;
		IMAGE_SIZE		 MaskH = UniFrameList[Index_3D].ImageH;
		IMAGE_SIZE		 MaskStep = UniFrameList[Index_3D].ImageStep;

		const bool		 bTestWnd = false;

		double patternH, patternW;
		body->GetBoxSize(patternW, patternH);
		patternH = patternH * scale.y;
		patternW = patternW * scale.x;

		//cv::Mat maskImg = ImageAPI.CreateMat(MaskW, MaskH, MaskStep, MaskBitCount, UniFrameList[1].ImagePtr);
		//cv::imwrite("mat.jpg", maskImg);

		// const char fnName[] = "ExecAlgUniFrameBinary";
		// const size_t     BufferSize = ImageAPI.CalcBufferSize(MaskStep, MaskH);

		// JetMemory.alloc_func(BufferSize, GrayPtr, fnName, "GrayPtr");

		// ImageAPI.SpaceGrayImageConvertToGray3(MaskW, MaskH, UniFrameList[3].ImageStep, UniFrameList[3].SpacePtr, GrayPtr);

		// cv::Mat maskImg2 = ImageAPI.CreateMat(MaskW, MaskH, UniFrameList[3].ImageStep, 8, GrayPtr);
		// cv::imwrite("mat2.jpg", maskImg2);

		// cv::Size sz((int)(MaskW), (int)(MaskH));
		// cv::Mat M(sz, CV_32FC1, UniFrameList[3].SpacePtr, UniFrameList[3].ImageStep *sizeof(float));


		// float value = 0;
		// std::ofstream myfile;
		// myfile.open("example.csv");
		// //for (int i = 0; i < 2; i++) {
		//	 for (int j = 0; j < MaskH; j++)
		//	 {
		//		 value = M.at<float>(j, MaskW/2);
		//		 myfile << value;
		//		 myfile << "\n";
		//	 }
		// //}

		// myfile.close();

		JET::alg::CHeightDetection hd;
		JET::alg::SHeightDetectionParam hightDetectParm;
		JET::alg::SHeightDetectionResult result;

		hightDetectParm.nInspectionType = Param.nInspectionType;
		hightDetectParm.sTinParam.nDirection = Param.nDirection;
		if (bodyToward == BOX_TOWARD_UP || bodyToward == BOX_TOWARD_DOWN)
		{
			if (Param.nDirection == 1)
				hightDetectParm.sTinParam.nDirection = 2;
			else if (Param.nDirection == 2)
				hightDetectParm.sTinParam.nDirection = 1;
		}
		hightDetectParm.sTinParam.nMeasureRange = Param.nMeasureRange;
		hightDetectParm.sTinParam.nOutputType = Param.nOutputType;
		hightDetectParm.sTinParam.nStepZ = Param.nStepZ;
		hightDetectParm.sTinParam.nShift_Tin = Param.nShift_Tin;
		hightDetectParm.sTinParam.nThresholdZ_Width = Param.nThresholdZ_Width;
		hightDetectParm.sTinParam.nThresholdZ_Height = Param.nThresholdZ_Height;
		hightDetectParm.sTinParam.nPatternHeight = patternH;
		hightDetectParm.sTinParam.nPatternWidth = patternW;

		TASK_STATE_MODE status = AOIDataCollect.GetOnlineTaskState();
		
		//if (status != TASK_STATE_RUNNING)
		//{//todo
		//	bool is = true;
		//	hd.SetSaveImage(is);
		//	string s = "E:\\Jet\\";
		//	bool isSave = hd.SetSavePathName(s);
		//}
		bool isOK = hd.MeasureHeight(MaskH, MaskStep, UniFrameList[Index_3D].SpacePtr, UniFrameList[Index_2D].ImagePtr, wndROI, hightDetectParm, result);
		
		//if (status != TASK_STATE_RUNNING)
		//{//todo
		//	cv::Rect roi1;
		//	cv::Mat M = cv::Mat(MaskH, MaskW, CV_8UC3, UniFrameList[1].ImagePtr);
		//	hd.GetAlignmentROI(roi1);
		//	string s = "E:\\Jet\\result.bmp";
		//	hd.DrawRotateRectangle(M, result.sTinResult.fRadian, roi1, cv::Scalar(255, 0, 0), 1);
		//	cv::imwrite(s, M);
		//}
		//Result
		if (!isOK)
		{
			WndPtr->SetWndResultID(RESULT_ID_NG);
			WndPtr->SetWndResultText(_T("NG"));
			Param.partPoint = hd.GetPartCoordinate();
			Param.tinPoint = hd.GetTinCoordinate();
			Param.resultID = -1;
			return true;
		}

		cv::Rect roi;
		double sx, sy;
		double wx, wy;
		
		hd.GetAlignmentROI(roi);
		//TPOINT2D WndRectCalValue = WndPtr->GetWndRectCalValue();
		WndPtr->GetWndImageRect(wndROI);
		
		POINT x1, x2, y1, y2, center;
		hd.CalPartROI_3D(roi, result.sTinResult.fRadian, x1, x2, y1, y2);
		
		center.x = (x1.x + y1.x) / 2;
		center.y = (x1.y + y1.y) / 2;
		//y.x = (x2.x + y2.x) / 2;
		//y.y = (x2.y + y2.y) / 2;

		//計算零件中心位置
		wx = wndROI.left + (wndROI.right - wndROI.left) / 2;
		wy = wndROI.top + (wndROI.bottom - wndROI.top) / 2;
		sx = roi.x + roi.width / 2;//patternW //roi.width
		sy = roi.y + roi.height / 2;//patternH //roi.height

		sx = center.x - wx;
		sy = center.y - wy;

		//計算scale補償
		sx = sx / scale.x;
		sy = sy / scale.y;
		//double ssx = WndRectCalValue.x;
		//double ssy = WndRectCalValue.y;

		if (fabs(sx) < 0.001) { sx = 0; }
		if (fabs(sy) < 0.001) { sy = 0; }
		if (fabs(result.sTinResult.fAngle) < 0.001) { result.sTinResult.fAngle = 0; }
		
		//cv::Mat mask = ImageAPI.CreateMat(UniFrameList[3].ImageW, UniFrameList[3].ImageH, UniFrameList[3].ImageStep, UniFrameList[3].BitCount, UniFrameList[3].MaskPtr);

		//cv::Mat maskImg = ImageAPI.CreateMat(MaskW, MaskH, UniFrameList[1].ImageStep, UniFrameList[1].BitCount, UniFrameList[1].ImagePtr);
		//cv::imwrite("or.jpg", mask);
		//hd.DrawRotateRectangle(maskImg, result.sTinResult.fRadian, roi, cv::Scalar(0, 0, 255), 1);
		//cv::imwrite("mat.jpg", maskImg);

		Param.partPoint = hd.GetPartCoordinate();
		Param.tinPoint = hd.GetTinCoordinate();

		/*const size_t     BufferSize = ImageAPI.CalcBufferSize(MaskStep, MaskH);
		if (JetMemory.alloc_func(BufferSize, UniFrameList[3].SpaceMaskPtr, "CAlgParam::ExecAlgInspection_Height", "SpaceMaskPtr") == false)
		{
			JetMemory.free_func(UniFrameList[3].SpaceMaskPtr);
			return false;
		}*/
		//::memcpy(UniFrameList[3].SpaceMaskPtr, UniFrameList[3].SpacePtr, sizeof(IMAGE_DATA)*BufferSize);
		
		//for (int i = RoiRect.top; i<RoiRect.bottom; i++)
		//{
		//	srcIdx = (SpaceH - i - 1)*SpaceStep;
		//	destIdx = i*ImageStep;
		//	for (int j = RoiRect.left; j<RoiRect.right; j++)
		//	{
		//		if ( (pMask[srcIdx+j]&PHASE_MASK_NOISE) != NULL )//會造成太多雜訊點導至無法影像匹配
		//		{
		//			pImage[destIdx+j] = 0;
		//			continue;
		//		}
		//		Space = pSpace[srcIdx + j];
		//		Space -= SpaceMin;
		//		Space *= SpaceScale;
		//		nGray = static_cast<int>(Space);
		//		pChar[destIdx+j] = (Space) >> BitShift;	
		//		if (nGray > 255)
		//		{
		//			pImage[destIdx + j] = 255;
		//		}
		//		else if (nGray < 0)
		//		{
		//			pImage[destIdx + j] = 0;
		//		}
		//		else
		//		{
		//			pImage[destIdx + j] = static_cast<unsigned char>(nGray);
		//		}
		//	}
		//}


		body->ResetBoxHeight();
		switch (hightDetectParm.sTinParam.nDirection)
		{
		case 1:
			body->SetBoxHeight(result.sTinResult.vtfPartHeight[0]);
			body->SetBoxHeight(result.sTinResult.vtfPartHeight[1]);
			body->SetBoxEdgePadHeight(result.sTinResult.vtfTinHeight[0]);
			body->SetBoxEdgePadHeight(result.sTinResult.vtfTinHeight[1]);
			break;
		case 2:
			body->SetBoxHeight(result.sTinResult.vtfPartHeight[2]);
			body->SetBoxHeight(result.sTinResult.vtfPartHeight[3]);
			body->SetBoxEdgePadHeight(result.sTinResult.vtfTinHeight[2]);
			body->SetBoxEdgePadHeight(result.sTinResult.vtfTinHeight[3]);
			break;
		case 3:
			body->SetBoxHeight(result.sTinResult.vtfPartHeight[0]);
			body->SetBoxHeight(result.sTinResult.vtfPartHeight[1]);
			body->SetBoxHeight(result.sTinResult.vtfPartHeight[2]);
			body->SetBoxHeight(result.sTinResult.vtfPartHeight[3]);
			body->SetBoxEdgePadHeight(result.sTinResult.vtfTinHeight[0]);
			body->SetBoxEdgePadHeight(result.sTinResult.vtfTinHeight[1]);
			body->SetBoxEdgePadHeight(result.sTinResult.vtfTinHeight[2]);
			body->SetBoxEdgePadHeight(result.sTinResult.vtfTinHeight[3]);
			break;
		default:
			body->SetBoxHeight(result.sTinResult.vtfPartHeight[0]);
			body->SetBoxHeight(result.sTinResult.vtfPartHeight[1]);
			body->SetBoxHeight(result.sTinResult.vtfPartHeight[2]);
			body->SetBoxHeight(result.sTinResult.vtfPartHeight[3]);
			body->SetBoxEdgePadHeight(result.sTinResult.vtfTinHeight[0]);
			body->SetBoxEdgePadHeight(result.sTinResult.vtfTinHeight[1]);
			body->SetBoxEdgePadHeight(result.sTinResult.vtfTinHeight[2]);
			body->SetBoxEdgePadHeight(result.sTinResult.vtfTinHeight[3]);
			break;
		}

		CString strResult = _T("OK");
		m_AlgResultID = RESULT_ID_OK;
		Param.resultID = 1;
		
		if (true == GetAlgOffsetXEnabled() && fabs(sx) > GetAlgOffsetXUSL())
		{
			m_AlgResultID = RESULT_ID_NG;
			strResult = _T("NG");
			Param.resultID = -2;
		}
		if (true == GetAlgOffsetYEnabled() && fabs(sy) > GetAlgOffsetYUSL())
		{
			m_AlgResultID = RESULT_ID_NG;
			strResult = _T("NG");
			Param.resultID = -2;
		}
		if (true == GetAlgSkewEnabled() && fabs(result.sTinResult.fAngle) > GetAlgSkewUSL())
		{
			m_AlgResultID = RESULT_ID_NG;
			strResult = _T("NG");
			Param.resultID = -2;
		}
		if (m_AlgResultID == RESULT_ID_NG && Param.enableDoubleCheck)
		{
			// 給復判更新
		}
		else
		{
			double scalx, scaly;

			SetAlgSkewReading(-result.sTinResult.fAngle);
			SetAlgOffsetXReading(sx);
			SetAlgOffsetYReading(-sy);
			
			CAOIBox *BoxPtr = WndPtr->GetWndBoxPtr();
			scalx = roi.size().width / scale.x;
			scaly = roi.size().height / scale.y;

			if (WND_LOGIC_NONE == WndPtr->GetWndLogicType())
			{
				if (ModelPtr->UpdateModelInspectionPosRes(WndPtr, sx, -sy, -result.sTinResult.fAngle, true) == false)
				{
					return true;
				}
			}
			//body->SkewBoxAngle(-result.sTinResult.fAngle);
			//body->MoveBoxRes(sx, -sy);

			BoxPtr->SetBoxSizeRes(scalx, scaly, false);
			BoxPtr->SkewBoxAngle(-result.sTinResult.fAngle);
			BoxPtr->MoveBoxRes(sx, -sy);
		}

		//strResult.Format(_T("dx, dy, angle= %d, %d, %.2f"), (int)sx, (int)sy, result.sTinResult.fAngle);
		WndPtr->SetWndResultID(m_AlgResultID);
		WndPtr->SetWndResultText(strResult);
		return true;
	}
	else
	{
		//Result
		// 只拿出結果
		CString strResult = _T("OK");

		m_AlgResultID = RESULT_ID_OK;
		if (Param.nPartHeightMeasureMode == 0)
		{
			if (padToward == BOX_TOWARD_UP || padToward == BOX_TOWARD_LEFT)
			{
				Param.resultH = (int)std::round((EdgeHeight[0] / boxHeight[0]) * 100);
			}
			else if (padToward == BOX_TOWARD_DOWN || padToward == BOX_TOWARD_RIGHT)
			{
				Param.resultH = (int)std::round((EdgeHeight[1] / boxHeight[1]) * 100);
			}

			if (Param.nPartHeightThreshold > Param.resultH)
			{
				m_AlgResultID = RESULT_ID_NG;
			}
			strResult.Format(_T("Ratio= %d"), Param.resultH);
		}
		else
		{
			if (padToward == BOX_TOWARD_UP || padToward == BOX_TOWARD_LEFT)
			{
				Param.resultH = (int)std::round(EdgeHeight[0]);
			}
			else if (padToward == BOX_TOWARD_DOWN || padToward == BOX_TOWARD_RIGHT)
			{
				Param.resultH = (int)std::round(EdgeHeight[1]);
			}

			if (Param.nPartHeightThreshold > Param.resultH)
			{
				m_AlgResultID = RESULT_ID_NG;
			}
			strResult.Format(_T("Height(um)= %d"), Param.resultH);
		}
		WndPtr->SetWndResultID(m_AlgResultID);
		WndPtr->SetWndResultText(strResult);
		return true;
	}
}

bool CAlgParam::ExecAlgInspection_WireWidth(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	if (NULL == ModelPtr) { return false; }

	CAOIWnd *WndPtr = GetAlgWndPtr();

	if (NULL == WndPtr) { return false; }

	TALG_PARAM_WIRE_WIDTH &wireWidthParm = CAlgParam::GetAlgParamWireWidth();

	int frameIndex = WndPtr->GetWndAlgParam().GetAlgImageBinParamPtr()->GetBinaryFrameIndex();
	if (UniFrameList[frameIndex].ImagePtr == NULL) { return false; }

	JET::alg::CHeightDetection hd;
	JET::alg::SDeltaMeasureParam wireParm;
	JET::alg::SDeltaMeasureResult result;

	RECT wndROI, roi;
	TPOINT2D scale;
	WndPtr->GetWndImageRect(wndROI);
	ModelPtr->GetModelImageScale(scale);

	//轉角度會變成step會變，ExtractRoiImage修正回去
	IMAGE_PTR imagePtr = NULL;

	roi.top = 0;
	roi.bottom = UniFrameList[frameIndex].ImageH;
	roi.left = 0;
	roi.right = UniFrameList[frameIndex].ImageW;
	if (ImageAPI.ExtractRoiImage(UniFrameList[frameIndex].ImageW, UniFrameList[frameIndex].ImageH, UniFrameList[frameIndex].ImageStep, UniFrameList[frameIndex].BitCount, UniFrameList[frameIndex].ImagePtr, roi, UniFrameList[frameIndex].ImageW * 3, imagePtr, false) == false)
	{
		JetMemory.free_func(imagePtr);
		return false;
	}


	wireParm.bFilter = wireWidthParm.bFilter;
	wireParm.nFilterSize = wireWidthParm.nFilterSize;
	wireParm.nLowThres = wireWidthParm.nLowThres;
	wireParm.nHeightThres = wireWidthParm.nHeightThres;
	wireParm.nImageH = UniFrameList[frameIndex].ImageH;
	wireParm.nImageW = UniFrameList[frameIndex].ImageW;
	wireParm.pu8Image = imagePtr;
	wireParm.rectPartRange = wndROI;

	//cv::Size sz((int)(UniFrameList[2].ImageW), (int)(UniFrameList[2].ImageH));
	//cv::Mat M(sz, CV_8UC3, imagePtr, UniFrameList[2].ImageW * 3 * sizeof(uchar));
	//cv::imwrite("M.jpg", M);

	bool is = true;
	hd.SetSaveImage(is);
	CString s0 = "D:\\Jet";
	string s = "D:\\Jet\\";
	JetAPI::ClearFolder(s0);
	bool isSave = hd.SetSavePathName(s);

	bool isOK = hd.DeltaMeasure(wireParm, result);
		
	//Result
	CString strResult = _T("OK");
	m_AlgResultID = RESULT_ID_OK;
	if (!isOK)
	{
		JetMemory.free_func(imagePtr);
		return false;
	}

	wireWidthParm.fAngle = result.fAngle;
	wireWidthParm.fWidth = result.fWidth / scale.x;//旋轉之後，
	wireWidthParm.ptCenterLine_Start = result.ptCenterLine_Start;
	wireWidthParm.ptCenterLine_End = result.ptCenterLine_End;
	wireWidthParm.ptLimit1 = result.ptLimit1;
	wireWidthParm.ptLimit2 = result.ptLimit2;

	if (wireWidthParm.fWidth > wireWidthParm.widthUSL || wireWidthParm.fWidth < wireWidthParm.widthLSL)
	{
		m_AlgResultID = RESULT_ID_NG;
	}
	strResult.Format(_T("Width(um) = %.1f"), wireWidthParm.fWidth);

	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(strResult);

	JetMemory.free_func(imagePtr);
	return true;
}