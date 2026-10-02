// AlgParam_BlobCount.cpp: implementation of the CAlgParam class.
//
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
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

bool					findFirst_rowcol(const cv::Mat mat, int direct, bool inv_direct, std::vector<TPOINT2D> &dataPoint, bool inv_dataXY);
std::vector<double>		Least_square(std::vector<TPOINT2D> Pointdata, bool rotate90);
//bool ExecAlgInspection_PartAlign(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);
//bool ExecAlgInspection_ICLeadAlign(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList);

//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_IPC(const TALG_PARAM_IPC_PRODUCT &Param, CAOIFileIO &FileIO)//儲存亮度比例參數
{
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IPC_START, 0) == false) { return false; }

	if (FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_IPC_ENABLE, Param.ipcEnabled) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IPC_X, Param.ipcX) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IPC_Y, Param.ipcY) == false) { return false; }

	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IPC_CONTINUOUS_SET, Param.ipcContinuousPixel) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IPC_WIDTH_GAP, Param.ipcGapPixel) == false) { return false; }
	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IPC_WIDTH_RATIO, Param.ipcWidthRatio) == false) { return false; }

	if (FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_IPC_END, 0) == false) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_IPC(TALG_PARAM_IPC_PRODUCT &Param, CAOIFileIO &FileIO)//載入
{
	int       index = 0;
	int       nValue = 0;
	while (FileIO.CheckFileEnd() == false)
	{
		if (FileIO.LoadChunk(index) == false)
		{
			continue;
		}
		switch ( index )
		{
		case FILE_IO_ALG_PARAM_IPC_END:
			return true;
			break;
		case FILE_IO_ALG_PARAM_IPC_ENABLE:
			Param.ipcEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_IPC_X:
			Param.ipcX = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_IPC_Y:
			Param.ipcY = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_IPC_CONTINUOUS_SET://
			Param.ipcContinuousPixel = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_IPC_WIDTH_GAP://
			Param.ipcGapPixel = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_IPC_WIDTH_RATIO://
			Param.ipcWidthRatio = FileIO.GetData_INT();
			break;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_IPC Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_IPC(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	switch (ModelPtr->GetModelType())
	{
		case 100:
		case 101:
		case 102:
		case 103:
		case 104:
		case 105:
		case 106:
		case 200:
		case 201:
		case 202:
		case 203:
		case 204:
		case 205:
		case 206:
		case 900:
		{
			return ExecAlgInspection_PartAlign(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);
			break;
		}
		case 400:
		case 401:
		case 402:
		case 450:
		case 451:
		case 452:
		case 500:
		case 501:
		case 511:
		{
			return ExecAlgInspection_ICLeadAlign(ModelPtr, ModelRgn, RoiRect, WndRect, UniFrameList);
			break;
		}
		default:
		{
			break;
		}
	}
	return true;
}

bool CAlgParam::ExecAlgInspection_PartAlign(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	if (NULL == ModelPtr) { return false; }

	CAOIWnd *WndPtr = GetAlgWndPtr();
	if (NULL == WndPtr) { return false; }

	CAOIBox *body = ModelPtr->GetModelBodyBoxPtr();
	CAOIBox *pad = NULL;
	CAOIBox *lead = NULL;
	const size_t LandCount = ModelPtr->GetModelLandCount();
	CAOILand    *LandPtr = NULL;

	TPOINT2D bodyPos;

	TPOINT2D leadLeft;
	TPOINT2D leadRight;
	TPOINT2D leadPos;
	TPOINT2D leadSize;
	TPOINT2D padPos;
	TPOINT2D padSize;
	BOX_TOWARD toward;
	BOX_TOWARD padToward;
	body->GetBoxPosRes(bodyPos);

	TALG_PARAM_IPC_PRODUCT &Param = CAlgParam::GetAlgParamIPC();
	if (!Param.ipcEnabled) { return true; }

	int xAlign, yAlign;
	int Xd, yd, xSize, ySize;

	LandPtr = WndPtr->GetWndLandPtr();
	pad = LandPtr->GetLandPadBoxPtr();
	lead = LandPtr->GetLandLeadBoxPtr();

	lead->GetBoxPosRes(leadPos);
	lead->GetBoxSize(leadSize.x, leadSize.y);
	padToward = pad->GetBoxToward();
	toward = lead->GetBoxToward();
	pad->GetBoxPosRes(padPos);
	pad->GetBoxSize(padSize.x, padSize.y);


	if (toward == BOX_TOWARD_LEFT || toward == BOX_TOWARD_RIGHT)
	{
		if (leadSize.y <= padSize.y)	// 取最小當標準
		{	// Y方向
			yd = leadSize.y * ((double)Param.ipcY * 0.01);
			ySize = leadSize.y;
		}
		else
		{
			yd = padSize.y * ((double)Param.ipcY * 0.01);
			ySize = padSize.y;
		}

		if (leadSize.x <= padSize.x)
		{
			Xd = leadSize.x * (1 - (double)Param.ipcX * 0.01);
			xSize = leadSize.x;
		}
		else
		{
			Xd = padSize.x * (1 - (double)Param.ipcX * 0.01);	// 用pad當size
			xSize = padSize.x;
		}
		if (toward == BOX_TOWARD_LEFT)
		{
			leadLeft.x = leadPos.x - leadSize.x / 2;
			leadLeft.y = leadPos.y;

			yAlign = abs(leadLeft.y - padPos.y) - (abs(leadSize.y - padSize.y) / 2);
			//pad1XAlign = bodyPos.x - bodySize.x / 2;
			//pad1XAlign = abs(leadLeft.x - (padPos.x + padSize.x / 2));

			xAlign = (padPos.x + padSize.x / 2) - leadLeft.x;
		}
		else if (toward == BOX_TOWARD_RIGHT)
		{
			leadRight.x = leadPos.x + leadSize.x / 2;
			leadRight.y = leadPos.y;

			yAlign = abs(leadRight.y - padPos.y) - (abs(leadSize.y - padSize.y) / 2);
			//pad2XAlign = abs((padPos.x - padSize.x / 2) - leadRight.x);

			xAlign = leadRight.x - (padPos.x - padSize.x / 2);
		}
	}
	else
	{	// UP DOWN
		if (leadSize.x <= padSize.x)	// 取最小當標準
		{
			yd = leadSize.x * ((double)Param.ipcY * 0.01);
			ySize = leadSize.x;
		}
		else
		{
			yd = padSize.x * ((double)Param.ipcY * 0.01);
			ySize = padSize.x;
		}

		if (leadSize.y <= padSize.y)
		{
			Xd = leadSize.y * (1 - (double)Param.ipcX * 0.01);
			xSize = leadSize.y;
		}
		else
		{
			Xd = padSize.y * (1 - (double)Param.ipcX * 0.01);
			xSize = padSize.y;
		}
		if (toward == BOX_TOWARD_UP)
		{
			leadLeft.x = leadPos.x;
			leadLeft.y = leadPos.y + leadSize.y / 2;

			yAlign = abs(leadLeft.x - padPos.x) - (abs(leadSize.x - padSize.x) / 2);
			xAlign = leadLeft.y - (padPos.y - padSize.y / 2);
			//a = (padPos.y - padSize.y / 2) - leadLeft.y;	// 測試用
		}
		else if (toward == BOX_TOWARD_DOWN)
		{
			leadRight.x = leadPos.x;
			leadRight.y = leadPos.y - leadSize.y / 2;

			yAlign = abs(leadRight.x - padPos.x) - (abs(leadSize.x - padSize.x) / 2);
			xAlign = padPos.y + padSize.y / 2 - leadRight.y;
			//pad2YAlign = abs(bodyPos.x - padPos.x) - (abs(padSize.x - bodySize.x) / 2);
			//a = leadRight.y - (padPos.y + padSize.y / 2);	// 測試用
		}
	}

	//Result
	CString strResult = WndPtr->GetWndResultText();
	double valuePercent, valuePercent2;
	m_AlgResultID = WndPtr->GetWndResultID();
	if (xAlign <= Xd || yAlign >= yd)
	{
		m_AlgResultID = RESULT_ID_NG;
	}

	valuePercent = double(yAlign) / ySize;
	valuePercent2 = double(xAlign) / xSize;
	if (valuePercent < 0) { valuePercent = 0; }
	
	if (valuePercent2 < 0) { valuePercent2 = 1 + abs(valuePercent2); }
	else if (valuePercent2 > 1) { valuePercent2 = 0; }
	else { valuePercent2 = 1 - valuePercent2;}
	strResult.Format(_T("Hor=%d, Ver=%d"), (int)std::round(valuePercent2 * 100), (int)std::round(valuePercent * 100));

	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(strResult);

	return true;
}

bool CAlgParam::ExecAlgInspection_ICLeadAlign(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	if (NULL == ModelPtr) { return false; }

	CAOIWnd *WndPtr = GetAlgWndPtr();
	if (NULL == WndPtr) { return false; }

	CAOIBox *body = ModelPtr->GetModelBodyBoxPtr();
	CAOIBox *pad = NULL;
	CAOIBox *lead = NULL;
	CAOIBox *leadTip = NULL;
	const size_t LandCount = ModelPtr->GetModelLandCount();
	CAOILand    *LandPtr = NULL;

	//TPOINT2D bodyPos;

	TPOINT2D leadResultPos;
	TPOINT2D leadTipPos;
	TPOINT2D leadTipSize;
	TPOINT2D leadSize;
	TPOINT2D padPos;
	TPOINT2D padSize;
	BOX_TOWARD leadTipToward;

	TALG_PARAM_IPC_PRODUCT &Param = CAlgParam::GetAlgParamIPC();
	if (!Param.ipcEnabled) { return true; }

	int xAlign, yAlign;
	int Xd, yd, xSize, ySize;

	LandPtr = WndPtr->GetWndLandPtr();
	pad = LandPtr->GetLandPadBoxPtr();
	lead = LandPtr->GetLandLeadBoxPtr();
	leadTip = LandPtr->GetLandLeadTipBoxPtr();// GetLandLeadBoxPtr();

	leadTip->GetBoxPosRes(leadTipPos);
	leadTip->GetBoxSize(leadTipSize.x, leadTipSize.y);
	leadTipToward = leadTip->GetBoxToward();
	lead->GetBoxSize(leadSize.x, leadSize.y);
	pad->GetBoxPosRes(padPos);
	pad->GetBoxSize(padSize.x, padSize.y);


	if (leadTipToward == BOX_TOWARD_LEFT || leadTipToward == BOX_TOWARD_RIGHT)
	{
		if (leadTipSize.y <= padSize.y)	// 取最小當標準
		{	// Y方向
			yd = leadTipSize.y * ((double)Param.ipcY * 0.01);
			ySize = leadTipSize.y;
		}
		else
		{
			yd = padSize.y * ((double)Param.ipcY * 0.01);
			ySize = padSize.y;
		}
		Xd = padSize.x * ((double)Param.ipcX * 0.01);
		xSize = padSize.x;


		if (leadTipToward == BOX_TOWARD_LEFT)
		{
			leadResultPos.x = leadTipPos.x - leadTipSize.x / 2;
			leadResultPos.y = leadTipPos.y;
			padPos.x = padPos.x - padSize.x / 2;
			//pad1YAlign = abs(leadLeft.y - padPos.y) - (abs(leadSize.y - padSize.y) / 2);
			//pad1XAlign = abs(leadLeft.x - (padPos.x + padSize.x / 2));

			//a = (padPos.x + padSize.x / 2) - leadLeft.x;
		}
		else if (leadTipToward == BOX_TOWARD_RIGHT)
		{
			leadResultPos.x = leadTipPos.x + leadTipSize.x / 2;
			leadResultPos.y = leadTipPos.y;
			padPos.x = padPos.x + padSize.x / 2;
			//pad2YAlign = abs(leadRight.y - padPos.y) - (abs(leadSize.y - padSize.y) / 2);
			//pad2XAlign = abs((padPos.x - padSize.x / 2) - leadRight.x);

			//a = leadRight.x - (padPos.x - padSize.x / 2);
			//pad2XAlign = abs( (padPos.x - padSize.x / 2) - (bodyPos.x + bodySize.x / 2) );
		}
		xAlign = abs(leadResultPos.x - padPos.x);
		yAlign = abs(leadResultPos.y - padPos.y) - abs(leadTipSize.y - padSize.y) / 2;
	}
	else
	{	// UP DOWM
		if (leadTipSize.x <= padSize.x)	// 取最小當標準
		{	// Y方向(取X)
			yd = leadTipSize.x * ((double)Param.ipcY * 0.01);
			ySize = leadTipSize.x;
		}
		else
		{
			yd = padSize.x * ((double)Param.ipcY * 0.01);
			ySize = padSize.x;
		}
		Xd = padSize.y * ((double)Param.ipcX * 0.01);
		xSize = padSize.y;


		if (leadTipToward == BOX_TOWARD_UP)
		{
			leadResultPos.x = leadTipPos.x;
			leadResultPos.y = leadTipPos.y + leadTipSize.y / 2;
			padPos.y = padPos.y + padSize.y / 2;
			//pad1YAlign = abs(leadLeft.x - padPos.x) - (abs(leadSize.x - padSize.x) / 2);
			//pad1XAlign = abs(leadLeft.y - (padPos.y - padSize.y / 2));
		}
		else if (leadTipToward == BOX_TOWARD_DOWN)
		{
			leadResultPos.x = leadTipPos.x;
			leadResultPos.y = leadTipPos.y - leadTipSize.y / 2;
			padPos.y = padPos.y - padSize.y / 2;
			//pad2YAlign = abs(leadRight.x - padPos.x) - (abs(leadSize.x - padSize.x) / 2);
			//pad2XAlign = abs(padPos.y + padSize.y / 2 - leadRight.y);
		}
		yAlign = abs(leadResultPos.x - padPos.x) - abs(leadTipSize.x - padSize.x) / 2;
		xAlign = abs(leadResultPos.y - padPos.y);
	}

	//Result
	CString strResult;
	double valuePercent, valuePercent2;
	m_AlgResultID = RESULT_ID_OK;
	if (xAlign >= Xd || yAlign >= yd)
	{
		m_AlgResultID = RESULT_ID_NG;
	}

	valuePercent = double(yAlign) / ySize;
	valuePercent2 = double(xAlign) / xSize;
	if (valuePercent < 0) { valuePercent = 0; }
	strResult.Format(_T("Hor=%d, Ver=%d"), (int)(std::round(valuePercent2 * 100)), (int)std::round(valuePercent * 100));

	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(strResult);

	return true;
}

bool CAlgParam::ExecAlgInspection_WidthRatio(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	if (NULL == ModelPtr) { return false; }
	
	CAOIWnd *WndPtr = GetAlgWndPtr();
	
	if (NULL == WndPtr) { return false; }

	CAOIBox *body = ModelPtr->GetModelBodyBoxPtr();

	TALG_PARAM_IPC_PRODUCT &Param = CAlgParam::GetAlgParamIPC();
	const double ratio = Param.ipcWidthRatio;
	//const int minSize = blobParam.bcCountLSL; // 預備沒再用
	int continuousPixel;

	double sizeX = ModelRgn.minX;
	double sizey = ModelRgn.minY;
	CAOIBox *pad = NULL;
	CAOIBox *lead = NULL;
	CAOILand    *LandPtr = NULL;

	TPOINT2D scale;
	BOX_TOWARD toward;

	RECT ROIWnd;
	TPOINT2D ImageCp;
	TPOINT2D RCp;
	double StartV = 0;

	ImageCp.x = UniFrameList[0].ImageW / 2;
	ImageCp.y = UniFrameList[0].ImageH / 2;
	RCp.x = ModelRgn.GetCpX();
	RCp.y = ModelRgn.GetCpY();

	ModelPtr->GetModelImageScale(scale);
	LandPtr = WndPtr->GetWndLandPtr();

	pad = LandPtr->GetLandPadBoxPtr();
	toward = pad->GetBoxToward();

	double dL, dT, dR, dB;
	int nL, nT, nR, nB;
	TREGION4D   padRgn;
	TREGION4D   leadRgn;
	TPOINT2D	leadSize;
	double		leadSizeBox;
	pad->GetBoxRegionRes(padRgn);
	
	// 計算pad lead的影像RECT，將um座標位置轉成影像位置
	RECT padRect;
	dL = (padRgn.minX - RCp.x)* scale.x + ImageCp.x;
	dT = (padRgn.minY - RCp.y)* scale.y + ImageCp.y;
	dR = (padRgn.maxX - RCp.x)* scale.x + ImageCp.x;
	dB = (padRgn.maxY - RCp.y)* scale.y + ImageCp.y;
	nL = (int)(dL + 0.5);
	nT = (int)(dT + 0.5);
	nR = (int)(dR + 0.5);
	nB = (int)(dB + 0.5);

	padRect.left = nL;
	padRect.top = nT;
	padRect.right = nR;
	padRect.bottom = nB;
	padRect.top = UniFrameList[0].ImageH - nB;
	padRect.bottom = UniFrameList[0].ImageH - nT;

	RECT leadRect;
	lead = LandPtr->GetLandLeadBoxPtr();
	lead->GetBoxRegionRes(leadRgn);
	lead->GetBoxSize(leadSize.x, leadSize.y);

	dL = (leadRgn.minX - RCp.x)* scale.x + ImageCp.x;
	dT = (leadRgn.minY - RCp.y)* scale.y + ImageCp.y;
	dR = (leadRgn.maxX - RCp.x)* scale.x + ImageCp.x;
	dB = (leadRgn.maxY - RCp.y)* scale.y + ImageCp.y;
	nL = (int)(dL + 0.5);
	nT = (int)(dT + 0.5);
	nR = (int)(dR + 0.5);
	nB = (int)(dB + 0.5);

	leadRect.left = nL;
	leadRect.top = nT;
	leadRect.right = nR;
	leadRect.bottom = nB;
	leadRect.top = UniFrameList[0].ImageH - nB;
	leadRect.bottom = UniFrameList[0].ImageH - nT;
	//dL = (WndRgn.minX - RgnCp.x)*Scale.x + ImageCp.x;
	//dT = (WndRgn.minY - RgnCp.y)*Scale.y + ImageCp.y;
	//dR = (WndRgn.maxX - RgnCp.x)*Scale.x + ImageCp.x;
	//dB = (WndRgn.maxY - RgnCp.y)*Scale.y + ImageCp.y;
	//nL = (int)(dL + 0.5);
	//nT = (int)(dT + 0.5);
	//nR = (int)(dR + 0.5);
	//nB = (int)(dB + 0.5);
	//WndRect.left = nL;
	//WndRect.top = nT;
	//WndRect.right = nR;
	//WndRect.bottom = nB;
	//WndRect.top = ImageH - nB;
	//WndRect.bottom = ImageH - nT;
	//return true;
	const int Gap = Param.ipcGapPixel;
	switch (toward)
	{
	case BOX_TOWARD_UP:
		StartV = leadRect.top;

		ROIWnd.bottom = StartV - Gap;
		ROIWnd.top = padRect.top;
		ROIWnd.right = leadRect.right;// MIN(padRect.right, leadRect.right);
		ROIWnd.left = leadRect.left;// MAX(padRect.left, leadRect.left);
		break;
	case BOX_TOWARD_LEFT:
		StartV = leadRect.left;
		//StartV = MIN(StartV, RgnBody.minX);

		ROIWnd.right = StartV - Gap;
		ROIWnd.left = padRect.left;
		ROIWnd.top = leadRect.top;// MAX(padRect.top, leadRect.top);
		ROIWnd.bottom = leadRect.bottom;// MIN(padRect.bottom, leadRect.bottom);
		break;
	case BOX_TOWARD_DOWN:
		StartV = leadRect.bottom;

		ROIWnd.top = StartV + Gap;
		ROIWnd.bottom = padRect.bottom;
		ROIWnd.right = leadRect.right;// MIN(padRect.right, leadRect.right);
		ROIWnd.left = leadRect.left;// MAX(padRect.left, leadRect.left);
		break;
	case BOX_TOWARD_RIGHT:
		StartV = leadRect.right;

		ROIWnd.left = StartV + Gap;
		ROIWnd.right = padRect.right;
		ROIWnd.top = leadRect.top;// MAX(padRect.top, leadRect.top);
		ROIWnd.bottom = leadRect.bottom;// MIN(padRect.bottom, leadRect.bottom);
		break;
	}
	

	IMAGE_SIZE		 MaskBitCount = 8;

	IMAGE_SIZE		 MaskW = 0;
	IMAGE_SIZE		 MaskH = 0;
	IMAGE_SIZE		 MaskStep = 0;

	MASK_PTR		 MaskPtr = NULL;
	IMAGE_PTR		 GrayPtr = NULL;
	const bool		 bTestWnd = false;

	//BOX_TOWARD toward = body->GetBoxToward();
	//TALG_PARAM_BRIGHT_RATIO &brParam = CAlgParam::GetAlgParamBrightRatio();

	//RECT Wnd = { 0,0,UniFrameList[0].ImageW, UniFrameList[0].ImageH };
	if (ExecAlgUniFrameBinary(m_AlgImageBinParam, ROIWnd, ROIWnd, UniFrameList, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, GrayPtr, bTestWnd) == false)
	{ //ExecAlgUniFrameBinary(m_AlgImageBinParam, WndRoiRect, WndRoiRect, WndUniFrameList, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, GrayPtr, bTestWnd)
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return false;
	}
	
	//Start Image Process.
	cv::Rect roiRect = cv::Rect(ROIWnd.left, ROIWnd.top, ROIWnd.right - ROIWnd.left, ROIWnd.bottom - ROIWnd.top);

	if (m_AlgImageBinParam.GetBinaryMode() == BINARY_DISABLE)
	{	// 沒設定使用全黑影像
		const size_t     BufferSize = ImageAPI.CalcBufferSize(MaskStep, MaskH);
		::memset(MaskPtr, 0x00, sizeof(MASK_DATA)*BufferSize);
	}

	cv::Mat maskImg = ImageAPI.CreateMat(MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr);
	cv::Mat mat(maskImg, roiRect);

	//cv::imwrite("roiRect.jpg", maskImg);
	//cv::imwrite("mat.jpg", mat);

	int Start;
	int row;
	int col;
	uchar *value = NULL;
	int numberOfNullPixel = 0;
	int widthInSigleRaw;
	int bodyWidth;
	int flag = 0; // 紀錄col中第一個像素
	std::vector<uchar*> scatteredPixels;
	std::vector<int> totalWidthInRaw;
	std::vector<int> widthInRaw;
	if (toward == BOX_TOWARD_LEFT || toward == BOX_TOWARD_RIGHT)
	{	//左右
		row = mat.rows;
		col = mat.cols;
		bodyWidth = leadSize.y * scale.y * (ratio * 0.01);
		leadSizeBox = row;
		continuousPixel = leadSizeBox * Param.ipcContinuousPixel * 0.01;
	}
	else
	{	//上下
		row = mat.cols;
		col = mat.rows;
		bodyWidth = leadSize.x * scale.x * (ratio * 0.01);
		leadSizeBox = row;
		continuousPixel = leadSizeBox * Param.ipcContinuousPixel * 0.01;
	}

	for (int j = 0; j < col; j++)
	{//左至右
		widthInSigleRaw = 0;
		numberOfNullPixel = 0;
		flag = 0;
		for (int i = 0; i < row; i++)
		{//上至下
			if (toward == BOX_TOWARD_UP || toward == BOX_TOWARD_DOWN)
			{
				value = &mat.at<uchar>(j, i);
			}
			else
			{
				value = &mat.at<uchar>(i, j);
			}
			if (*value > 1)
			{	// pixel>1 消除scatteredPixels內資訊
				numberOfNullPixel = 0;
				for (int z = 0; z < scatteredPixels.size(); z++) {
					*scatteredPixels[z] = 255;
				}
				scatteredPixels.clear();
				flag = 1;
			}
			else
			{	// 紀錄pixel=0，加資料，資料>n不算scattered
				numberOfNullPixel++;
				if (numberOfNullPixel > continuousPixel)
					scatteredPixels.clear();
				else if (flag == 1/*有第一個pixel*/)
				{
					scatteredPixels.push_back(value);
				}
			}
		}//end for i

		scatteredPixels.clear();
		//if (numberOfNullPixel < continuousPixel)
		//{	// 該行末端，檢查&消除scatteredPixels內資訊
		//	for (int z = 0; z < scatteredPixels.size(); z++) {
		//		*scatteredPixels[z] = 0;
		//	}
		//	scatteredPixels.clear();
		//}

		for (int i = 0; i < row; i++)
		{	// 統計該行幾個
			if (toward == BOX_TOWARD_UP || toward == BOX_TOWARD_DOWN)
			{
				value = &mat.at<uchar>(j, i);
			}
			else
			{
				value = &mat.at<uchar>(i, j);
			}
			if (*value > 0)
			{
				widthInSigleRaw++;
			}

		}//end for i
		if (widthInSigleRaw >= bodyWidth)
			widthInRaw.push_back(widthInSigleRaw);

		//if (widthInSigleRaw > 0)
		totalWidthInRaw.push_back(widthInSigleRaw);
	}//end for j
	//cv::imwrite("mat2.jpg", mat);

	//Result
	if (toward == BOX_TOWARD_UP || toward == BOX_TOWARD_LEFT)
	{
		std::reverse(widthInRaw.begin(), widthInRaw.end());
		std::reverse(totalWidthInRaw.begin(), totalWidthInRaw.end());
	}

	int sum = 0;
	double avg = 0;
	double avg2 = 0;
	int count = 0;
	std::vector<int> count2;

	double ga = 0;
	
	double std = 0;
	if (totalWidthInRaw.size() > 0)
	{
		for (int z = 0; z < 1; z++)
		{
			if (totalWidthInRaw.at(z) == 0)
				break;
			sum += totalWidthInRaw.at(z);
			count++;

			if (totalWidthInRaw.at(z) >= bodyWidth)
			{
				count2.push_back(totalWidthInRaw.at(z));
				avg2 += totalWidthInRaw.at(z);
			}
		}
		if (count != 0)
		{
			avg = (double)sum / count;
			//avg2 = avg2 / count2.size();

			/*avg2 = 0;
			std = 0;
			count = 0;
			for (int z = 0; z < 1; z++)
			{
				if (totalWidthInRaw.at(z) == 0)
					break;
				count++;
				std += std::pow(totalWidthInRaw.at(z) - avg, 2);

			}
			std = std::sqrt(std / count);

			ga = avg - 1.5 * std;
			sum = 0;
			count = 0;
			for (int z = 0; z < 1; z++)
			{
				if (totalWidthInRaw.at(z) == 0)
					break;
				if (totalWidthInRaw.at(z) < ga)
					continue;
				sum += totalWidthInRaw.at(z);
				count++;
			}
			avg2 = (double)sum / count;*/
		}
		
	}
	/*std::sort(widthInRaw.begin(), widthInRaw.end(), std::greater<int>());
	std::map<int, int> map;*/
	
	//for (int i = 0; i < widthInRaw.size(); i++) {
	//	map[widthInRaw.at(i)]++;
	//}


	CString strResult = _T("OK");
	Param.ipcResult = (int)std::round((avg / leadSizeBox) * 100);
	m_AlgResultID = RESULT_ID_OK;
	
	if ((avg / leadSizeBox) < (ratio * 0.01)/*widthInRaw.size() < minSize*/)
	{
		m_AlgResultID = RESULT_ID_NG;
	}
	strResult.Format(_T("Value= %d"), (int)std::round( (avg / leadSizeBox) * 100));
	WndPtr->SetWndResultID(m_AlgResultID);
	WndPtr->SetWndResultText(strResult);

	JetMemory.free_func(GrayPtr);
	JetMemory.free_func(MaskPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
//bool CAlgParam::ExecAlgInspection_IPC(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
//{
//	if (NULL == ModelPtr) { return false; }
//	CAOIComponent	*ComponentPtr = ModelPtr->GetModelComponentPtr();
//	//const size_t MaskFrameIndex = m_AlgMaskBinParam.GetBinaryFrameIndex();
//	const size_t	 ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
//
//	//Start
//	TUNI_FRAME      *UniFramePtr = &(UniFrameList[ImageFrameIndex]);
//	const IMAGE_SIZE FrameImageW = UniFramePtr->ImageW;
//	const IMAGE_SIZE FrameImageH = UniFramePtr->ImageH;
//	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
//	const IMAGE_SIZE FrameBitCount = UniFramePtr->BitCount;
//	const int        nAlign = 4;
//	/*
//	const unsigned int WndIndex = WndPtr->GetWndIndex();
//	BOX_TOWARD       WndToward = WndPtr->GetWndToward();
//	TUNI_FRAME      *UniFramePtr = &(UniFrameList[ImageFrameIndex]);
//	const IMAGE_SIZE FrameImageW = UniFramePtr->ImageW;
//	const IMAGE_SIZE FrameImageH = UniFramePtr->ImageH;
//	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
//	const IMAGE_SIZE FrameBitCount = UniFramePtr->BitCount;
//	IMAGE_PTR        FrameImagePtr = UniFramePtr->ImagePtr;
//	MASK_PTR         FrameMaskPtr = UniFramePtr->MaskPtr;
//	SPACE_PTR        FrameSpacePtr = UniFramePtr->SpacePtr;
//	*/
//	CAOIWnd          *WndPtr = GetAlgWndPtr();
//	if (NULL == WndPtr) { return false; }
//
//	const size_t	 FrameCount = UniFrameList.size();
//	const size_t     WndRoiCount = WndPtr->GetWndRoiWndCount();
//
//	const IMAGE_SIZE RoiW = RoiRect.right - RoiRect.left;
//	const IMAGE_SIZE RoiH = RoiRect.bottom - RoiRect.top;
//
//	CAOIWndRoi      *WndRoiPtr = NULL;
//	TREGION4D        WndRegion;
//	TREGION4D        WndRoiRegion;
//	CAlgBinaryParam *BinaryParamPtr = NULL;
//
//	IMAGE_SIZE		 MaskW = 0;
//	IMAGE_SIZE		 MaskH = 0;
//	IMAGE_SIZE		 MaskStep = 0;
//	RECT			 WndRoiRect;
//	TUNI_FRAME		 WndUniFrame;
//	std::vector<TUNI_FRAME> WndUniFrameList;
//	IMAGE_SIZE		 MaskBitCount = 8;
//	MASK_PTR		 MaskPtr = NULL;
//	IMAGE_PTR		 GrayPtr = NULL;
//	const bool		 bTestWnd = false;
//
//	for (int i = 0; i < FrameCount; i++)
//	{
//		IMAGE_SIZE    WndImageW = 0;
//		IMAGE_SIZE    WndImageH = 0;
//		IMAGE_SIZE    WndImageStep = 0;
//		IMAGE_SIZE    WndImageBitCount = FrameBitCount;
//		MASK_PTR      WndMaskPtr = NULL;
//		SPACE_PTR     WndSpacePtr = NULL;
//		IMAGE_PTR     WndImagePtr = NULL;
//
//		TUNI_FRAME  TempUniFrame = UniFrameList[i];
//		IMAGE_SIZE  TempFrameW = TempUniFrame.ImageW;
//		IMAGE_SIZE  TempFrameH = TempUniFrame.ImageH;
//		IMAGE_SIZE  TempFrameStep = TempUniFrame.ImageStep;
//		IMAGE_SIZE  TempBitCount = TempUniFrame.BitCount;
//		IMAGE_PTR   TempImagePtr = TempUniFrame.ImagePtr;
//		MASK_PTR    TempMaskPtr = TempUniFrame.MaskPtr;
//		SPACE_PTR   TempSpacePtr = TempUniFrame.SpacePtr;
//
//		WndImageW = RoiW;
//		WndImageH = RoiH;
//		MaskStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, MaskBitCount, nAlign);
//		WndImageStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, TempBitCount, nAlign);
//		if (NULL != TempMaskPtr)
//		{
//			if (ImageAPI.ExtractGrayRoiImage(TempFrameW, TempFrameH, TempFrameStep, TempMaskPtr, RoiRect, MaskStep, WndMaskPtr, false) == false)
//			{
//				JetMemory.free_func(WndMaskPtr);
//				JetMemory.free_func(WndSpacePtr);
//				JetMemory.free_func(WndImagePtr);
//				JetAPI::ClearUniFrameList(WndUniFrameList);
//				return false;
//			}
//		}
//		if (NULL != TempSpacePtr)
//		{
//			if (ImageAPI.ExtractSpaceGrayRoiImage(TempFrameW, TempFrameH, TempFrameStep, TempSpacePtr, RoiRect, MaskStep, WndSpacePtr, false) == false)
//			{
//				JetMemory.free_func(WndMaskPtr);
//				JetMemory.free_func(WndSpacePtr);
//				JetMemory.free_func(WndImagePtr);
//				JetAPI::ClearUniFrameList(WndUniFrameList);
//				return false;
//			}
//		}
//		if (NULL != TempImagePtr)
//		{
//			if (ImageAPI.ExtractRoiImage(TempFrameW, TempFrameH, TempFrameStep, TempBitCount, TempImagePtr, RoiRect, WndImageStep, WndImagePtr, false) == false)
//			{
//				JetMemory.free_func(WndMaskPtr);
//				JetMemory.free_func(WndSpacePtr);
//				JetMemory.free_func(WndImagePtr);
//				JetAPI::ClearUniFrameList(WndUniFrameList);
//				return false;
//			}
//#ifdef _DEBUG
//			if (TRUE == bSave)
//			{
//				str.Format(_T("%s\\%s_ModelWndAlgColorCode#%d_Image#%d.PNG"), AOIDataCollect.GetAOITempDirectory(), ComponentName, WndIndex + 1, i + 1);
//				ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, FrameBitCount, WndImagePtr, true);
//			}
//#endif//_DEBUG
//		}
//		WndUniFrame.ImageW = RoiRect.right - RoiRect.left;
//		WndUniFrame.ImageH = RoiRect.bottom - RoiRect.top;
//		WndUniFrame.ImageStep = JetAPI::GetBMPImagePixelsPerLine(WndUniFrame.ImageW, TempBitCount, nAlign);
//		WndUniFrame.BitCount = TempBitCount;
//		WndUniFrame.MaskPtr = WndMaskPtr;
//		WndUniFrame.ImagePtr = WndImagePtr;
//		WndUniFrame.SpacePtr = WndSpacePtr;
//		WndUniFrameList.push_back(WndUniFrame);
//
//		WndMaskPtr = NULL;
//		WndImagePtr = NULL;
//		WndSpacePtr = NULL;
//	}
//
//	RECT RoiWndRect = { 0,0,0,0 };
//	WndPtr->GetWndRegion(WndRegion);
//	JetAPI::SizeToRect(RoiW, RoiH, RoiWndRect);
//
//	std::vector<TPOINT2D> grabFirstLine, grabSecLine;
//	std::vector<double> grabFirstData, grabSecData;
//	TALG_PARAM_BRIGHT_RATIO &brParam = CAlgParam::GetAlgParamBrightRatio();
//	int padMid1, padMid2, bodyMid1, bodyMid2;
//	int padW, bodyW, padL; //todo padW是否該分1 2
//	int bodyRect;
//	cv::Point topLeftPoint;
//	cv::Point bottomLeftPoint;
//	cv::Point topRightPoint;
//	cv::Point bottomRightPoint;
//	int rightPoint; // 計算橫向偏移用
//	int leftPoint;
//	int leftAlignValue, rightAlignValue;
//
//	for (int i = 0; i < 3; i++)
//	{
//		WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
//
//		if (NULL == WndRoiPtr) { continue; }
//		WndRoiPtr->GetWndRoiRegion(WndRoiRegion);
//		if (JetAPI::CalcRegionRect(WndRegion, RoiWndRect, WndRoiRegion, WndRoiRect, true) == false) //Cad和Image的Y是顛倒的
//		{
//			continue;
//		}
//
//		//BinaryParamPtr = m_AlgImageBinParam;
//		//BinaryParamPtr->SetBinaryMode(BINARY_FIXED_THRESHOLD);
//		if (ExecAlgUniFrameBinary(m_AlgImageBinParam, WndRoiRect, WndRoiRect, WndUniFrameList, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, GrayPtr, bTestWnd) == false)
//		{
//			JetMemory.free_func(MaskPtr);
//			JetMemory.free_func(GrayPtr);
//			JetAPI::ClearUniFrameList(WndUniFrameList);
//			return false;
//		}
//
//		//Start Image Process.
//		cv::Rect roiRect = cv::Rect(WndRoiRect.left + 1, WndRoiRect.top + 1, WndRoiRect.right - WndRoiRect.left - 2, WndRoiRect.bottom - WndRoiRect.top - 2);
//		grabFirstLine.clear(), grabSecLine.clear(), grabFirstData.clear(), grabSecData.clear();
//
//		cv::Mat grayImg = ImageAPI.CreateMat(MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr);
//		cv::Mat roi(grayImg, roiRect);
//
//		cv::imwrite("roi.jpg", roi);
//
//		BOX_TOWARD toward = WndRoiPtr->GetWndRoiToward();
//		int sobeldxToward = 0, sobeldyToward = 0;
//		if (toward == BOX_TOWARD_UP || toward == BOX_TOWARD_DOWN)
//		{	// 垂直
//			sobeldxToward = 1;
//			sobeldyToward = 0;
//		}
//		else
//		{
//			sobeldxToward = 0;
//			sobeldyToward = 1;
//		}
//
//		if (i == 1)	// For Body
//		{
//			cv::threshold(roi, roi, 5, 255, cv::THRESH_BINARY);	// todo
//			cv::Mat element = cv::getStructuringElement(0, cv::Size(3, 3));
//
//			cv::erode(roi, roi, element);
//			cv::imwrite("roiBlob.jpg", roi);
//
//			CJetBlob BlobDetector;
//
//			MASK_PTR roiPtr = NULL;
//			roiPtr = roi.ptr<unsigned char>(0);
//
//			BlobDetector.InitialBlob();
//			BlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
//			BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);
//			if (BlobDetector.GrayImageBlobDetect(roi.cols, roi.rows, MaskStep, roiPtr, 5, 255) == false)
//			{
//				JetMemory.free_func(MaskPtr);
//				JetMemory.free_func(GrayPtr);
//				return false;
//			}
//
//			const size_t ObjBlobCount = BlobDetector.GetBlobCount();
//			TBlobResult *BlobPtrTmp = NULL;
//			TBlobResult *BlobPtr = NULL;
//			int maxPixels = 0;
//			for (int k = 0; k < ObjBlobCount; k++)
//			{
//				BlobPtrTmp = BlobDetector.GetBlobPtr(k, false);
//				if (NULL == BlobPtrTmp) { continue; }
//				if (BlobPtrTmp->m_BlobPixels > maxPixels)
//				{
//					maxPixels = BlobPtrTmp->m_BlobPixels;
//					BlobPtr = BlobPtrTmp;
//				}
//			}
//
//			//取出最大blob
//			std::vector<cv::Point> point;
//			BlobDetector.GetBlobPixelList(BlobPtr, point);
//			cv::Mat reBuildRoi = cv::Mat::zeros(roi.rows, roi.cols, CV_8UC1);
//			size_t count = point.size();
//			for (int j = 0; j < count; j++)
//			{
//				reBuildRoi.at<char>(point[j].y, point[j].x) = 255;
//			}
//			cv::imwrite("MaxRoiBlob.jpg", reBuildRoi);
//
//			findFirst_rowcol(reBuildRoi, sobeldxToward, false, grabFirstLine, sobeldxToward);
//			grabFirstData = Least_square(grabFirstLine, false);
//
//			findFirst_rowcol(reBuildRoi, sobeldxToward, true, grabSecLine, sobeldxToward);
//			grabSecData = Least_square(grabSecLine, false);
//		}
//		else
//		{	// Pad
//			//cv::Sobel(roi, roi, CV_16S, sobeldxToward, sobeldyToward);
//			//convertScaleAbs(roi, roi);
//
//			//cv::imwrite("Pad sobel.jpg", roi);
//
//			//cv::threshold(roi, roi, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
//			cv::imwrite("Pad thres.jpg", roi);
//			findFirst_rowcol(roi, sobeldxToward, false, grabFirstLine, sobeldxToward);
//			grabFirstData = Least_square(grabFirstLine, false);
//
//			findFirst_rowcol(roi, sobeldxToward, true, grabSecLine, sobeldxToward);
//			grabSecData = Least_square(grabSecLine, false);
//		} // end if
//
//
//		if (i == 0) // 計算位置
//		{	// Pad1
//			if (grabFirstLine.size() == 0 || grabSecLine.size() == 0) { continue; }
//			padW = abs(grabSecData[1] - grabFirstData[1]);	// 目前忽略斜率
//			padL = (grabFirstLine.size() + grabSecLine.size()) / 2; // 點數平均當作長度
//			if (toward == BOX_TOWARD_LEFT || toward == BOX_TOWARD_RIGHT)
//			{
//				padMid1 = roiRect.y + (grabSecData[1] + grabFirstData[1]) / 2;	// 縱向
//				leftPoint = roiRect.x + (grabFirstLine[grabFirstLine.size() - 1].x + grabSecLine[grabSecLine.size() - 1].x) / 2;
//			}
//			else if (toward == BOX_TOWARD_UP || toward == BOX_TOWARD_DOWN)
//			{
//				padMid1 = roiRect.x + (grabSecData[1] + grabFirstData[1]) / 2;
//				leftPoint = roiRect.y + (grabFirstLine[grabFirstLine.size() - 1].x + grabSecLine[grabSecLine.size() - 1].x) / 2;
//			}
//		}
//		else if (i == 1)
//		{	// Body用側邊端點算中心線
//			if (grabFirstLine.size() == 0 || grabSecLine.size() == 0) { continue; }
//			bodyW = abs(grabSecData[1] - grabFirstData[1]);// 目前忽略斜率
//														   // 算body左右&上下的點
//			topLeftPoint = cv::Point(grabFirstLine[0].x, grabFirstData[0] * grabFirstLine[0].x + grabFirstData[1]);
//			bottomLeftPoint = cv::Point(grabSecLine[0].x, grabSecData[0] * grabSecLine[0].x + grabSecData[1]);
//			topRightPoint = cv::Point(grabFirstLine[grabFirstLine.size() - 1].x, grabFirstData[0] * grabFirstLine[grabFirstLine.size() - 1].x + grabFirstData[1]);
//			bottomRightPoint = cv::Point(grabSecLine[grabSecLine.size() - 1].x, grabSecData[0] * grabSecLine[grabSecLine.size() - 1].x + grabSecData[1]);
//
//			if (toward == BOX_TOWARD_LEFT || toward == BOX_TOWARD_RIGHT)
//			{
//				bodyMid1 = roiRect.y + (topLeftPoint.y + bottomLeftPoint.y) / 2;
//				bodyMid2 = roiRect.y + (topRightPoint.y + bottomRightPoint.y) / 2;
//				bodyRect = roiRect.x;
//			}
//			else if (toward == BOX_TOWARD_UP || toward == BOX_TOWARD_DOWN)
//			{
//				bodyMid1 = roiRect.x + (topLeftPoint.y + bottomLeftPoint.y) / 2;
//				bodyMid2 = roiRect.x + (topRightPoint.y + bottomRightPoint.y) / 2;
//				bodyRect = roiRect.y;
//			}
//		}
//		else if (i == 2)
//		{	// Pad2
//			if (grabFirstLine.size() == 0 || grabSecLine.size() == 0) { continue; }
//			if (toward == BOX_TOWARD_LEFT || toward == BOX_TOWARD_RIGHT)
//			{
//				padMid2 = roiRect.y + (grabSecData[1] + grabFirstData[1]) / 2;
//				rightPoint = roiRect.x + (grabFirstLine[0].x + grabSecLine[0].x) / 2;
//
//				leftAlignValue = topLeftPoint.x + bodyRect - leftPoint; // 橫向偏移 左
//				rightAlignValue = topRightPoint.x + bodyRect - rightPoint; // 橫向偏移 右
//			}
//			else if (toward == BOX_TOWARD_UP || toward == BOX_TOWARD_DOWN)
//			{
//				padMid2 = roiRect.x + (grabSecData[1] + grabFirstData[1]) / 2;
//				rightPoint = roiRect.y + (grabFirstLine[0].x + grabSecLine[0].x) / 2;
//
//				leftAlignValue = topLeftPoint.x + bodyRect - leftPoint;// 橫向偏移 上
//				rightAlignValue = topRightPoint.x + bodyRect - rightPoint; // 橫向偏移 下
//			}
//		}
//
//		GrayPtr = NULL;
//	} // end For
//
//	  //Result
//	int width = 0, a = 0, d = 0;
//	// 中心線位置bodyMid1、padMid1，bodyMid2、padMid2
//	d = padL * brParam.brYIPC; // 先使用
//	if (padW < bodyW)
//	{
//		width = padW * brParam.brXIPC;
//		a = (bodyW - padW) / 2;
//	}
//	else
//	{
//		width = bodyW * brParam.brXIPC;
//		a = (padW - bodyW) / 2;
//	}
//
//	m_AlgResultID = RESULT_ID_OK;
//	CString strResult;
//	// 判斷橫向: 中心線-中心線-a
//	// 看看能不能整合m_AlgImageOffsetX
//	if (abs(bodyMid1 - padMid1) - a >= width || abs(bodyMid2 - padMid2) - a >= width || abs(leftAlignValue + rightAlignValue) > d)
//	{
//		m_AlgResultID = RESULT_ID_NG;
//	}
//
//	strResult.Format(_T("A1=%d, A2=%d, w=%d, L=%d, R=%d, d=%d"), abs(bodyMid1 - padMid1) - a, abs(bodyMid2 - padMid2) - a, width, leftAlignValue, rightAlignValue, d);
//	//m_AlgResultText = _T("offest T");
//
//	WndPtr->SetWndResultID(m_AlgResultID);
//	WndPtr->SetWndResultText(strResult);
//
//	return true;
//}
//-------------------------------------------------------------------------------------//
bool findFirst_rowcol(const cv::Mat mat, int direct, bool inv_direct, std::vector<TPOINT2D> &dataPoint, bool inv_dataXY)
{
	//cv::Mat img = mat.clone();
	int row = mat.rows;
	int col = mat.cols;
	int i_;
	TPOINT2D data;
	cv::Mat result = cv::Mat::zeros(row, col, CV_8UC1);
	uchar pix;
	if (direct == 1) {
		for (int j = 0; j < row - 1; j++) {//上至下
			for (int i = 0; i < col / 2; i++) {//左至右
				if (inv_direct) {//反向
					i_ = col - i - 1;
				}
				else {
					i_ = i;
				}
				pix = mat.at<uchar>(j, i_);
				if (pix > 0) {
					result.at<uchar>(j, i_) = 255;
					if (inv_dataXY) {//xy顛倒存
						data.x = j;
						data.y = i_;
					}
					else {
						data.x = i_;
						data.y = j;
					}
					dataPoint.push_back(data);
					break;
				}// end if
			}//end for i
		}//end for j
	}
	else {
		for (int i = 0; i < col - 1; i++) {
			for (int j = 0; j < row / 2; j++) {
				if (inv_direct) {
					i_ = row - j - 1;
				}
				else {
					i_ = j;
				}
				pix = mat.at<uchar>(i_, i);
				if (pix > 0) {
					result.at<uchar>(i_, i) = 255;
					if (inv_dataXY) {
						data.x = i_;
						data.y = i;
					}
					else {
						data.x = i;
						data.y = i_;
					}
					dataPoint.push_back(data);
					break;
				}// end if
			}//end for i
		}//end for j
	}
	return true;
}
//-------------------------------------------------------------------------------------//
std::vector<double> Least_square(std::vector<TPOINT2D> Pointdata, bool rotate90)
{

	double _x = 0, _y = 0, _xy = 0, _x2 = 0;
	int n = Pointdata.size();
	int maxLoc = 0, minLoc = 0;
	double a, b, error;
	double e = 0;
	double maxerror = 0, minerror = 0;
	std::vector<double> resultVec;
	resultVec.reserve(5);

	for (int i = 0; i < n; i++) {
		_x += Pointdata[i].x;
		_y += Pointdata[i].y;
		_xy += Pointdata[i].x * Pointdata[i].y;
		_x2 += Pointdata[i].x * Pointdata[i].x;
	}
	a = (_xy - (_x * _y / n)) / (_x2 - (_x * _x / n));
	b = (_y - a * _x) / n;

	for (int i = 0; i < n; i++) {
		e = Pointdata[i].y - (a * Pointdata[i].x + b);
		if (e > maxerror) {
			maxerror = e;
			maxLoc = i;
		}
		if (e < minerror) {
			minerror = e;
			minLoc = i;
		}
	}
	error = maxerror - minerror;

	if (rotate90) {
		a = atan(a);
		a = a + (CV_PI / 2);
		a = tan(a);
		b = 0 - a * b;
	}
	//todo TLineEquation2D
	double result[] = { a, b, error, maxLoc, minLoc, maxerror, minerror };
	resultVec.assign(result, result + 7);
	return resultVec;
}