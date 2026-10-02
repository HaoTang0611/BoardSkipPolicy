// AlgParam_BarcodeRecognize.cpp: implementation of the CAlgParam class.
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
//-------------------------------------------------------------------------------------//
bool CAlgParam::DefaultAlgBarcodeStepParam(TALG_BARCODE_STEP_PARAM &Param)
{	
	switch ( Param.BarcodeStep )
	{
	case ALG_BARCODE_STEP_NONE:
		Param.Param1 = 1.0;
		Param.Param2 = 1.0;
		break;
	case ALG_BARCODE_STEP_SCALE:
		Param.Param1 = 0.5;
		Param.Param2 = 1.0;
		break;
	case ALG_BARCODE_STEP_GAIN_OFFSET:
		Param.Param1 = 1.0;
		Param.Param2 = 0.0;
		break;
	case ALG_BARCODE_STEP_SMOOTH:
		Param.Param1 = 3.0;
		Param.Param2 = 3.0;
		break;
	case ALG_BARCODE_STEP_OPEN:
		Param.Param1 = 3.0;
		Param.Param2 = 1.0;
		break;
	case ALG_BARCODE_STEP_CLOSE:
		Param.Param1 = 3.0;
		Param.Param2 = 1.0;
		break;
	case ALG_BARCODE_STEP_MEDIAN:
		Param.Param1 = 3.0;
		Param.Param2 = 1.0;
		break;
	case ALG_BARCODE_STEP_INVERT:		
		Param.Param1 = 1.0;
		Param.Param2 = 1.0;
		break;
	case ALG_BARCODE_STEP_FLIP:		
		Param.Param1 = 1.0;
		Param.Param2 = 1.0;
		break;
	case ALG_BARCODE_STEP_FILL:
		Param.Param1 = 1.0;
		Param.Param2 = 1.0;
		break;
	case ALG_BARCODE_STEP_ERODE:
		Param.Param1 = 3.0;
		Param.Param2 = 1.0;
		break;
	case ALG_BARCODE_STEP_DILATE:
		Param.Param1 = 3.0;
		Param.Param2 = 1.0;
		break;
	case ALG_BARCODE_STEP_FILL_2D:
		Param.Param1 = 1.0;
		Param.Param2 = 1.0;
		break;
	case ALG_BARCODE_STEP_SHARP:
		Param.Param1 = 3.0;
		Param.Param2 = 5.0;
		break;
	case ALG_BARCODE_STEP_GRAY_RANGE:
		Param.Param1 = 0.0;
		Param.Param2 = 255.0;
		break;
	default:	
		Param.Param1 = 1.0;
		Param.Param2 = 1.0;
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BarcodeVerify(const TALG_PARAM_BARCODE_RECOGNIZE &Param)
{
	if ( CheckOK_BarcodeVerifyUSL(Param) == false )
	{	return false; }
	if ( CheckOK_BarcodeVerifyLSL(Param) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BarcodeVerifyUSL(const TALG_PARAM_BARCODE_RECOGNIZE &Param)
{
	if ( Param.brVerifyReading > Param.brVerifyUSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::CheckOK_BarcodeVerifyLSL(const TALG_PARAM_BARCODE_RECOGNIZE &Param)
{
	if ( Param.brVerifyReading < Param.brVerifyLSL )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::BuildBarcodeDecodeStepList(std::vector<ALG_BARCODE_STEP_MODE> &List)
{
	List.clear();
	List.push_back(ALG_BARCODE_STEP_NONE);
	List.push_back(ALG_BARCODE_STEP_SCALE);
	List.push_back(ALG_BARCODE_STEP_GAIN_OFFSET);
	List.push_back(ALG_BARCODE_STEP_SMOOTH);
	List.push_back(ALG_BARCODE_STEP_OPEN);
	List.push_back(ALG_BARCODE_STEP_CLOSE);
	List.push_back(ALG_BARCODE_STEP_MEDIAN);
	List.push_back(ALG_BARCODE_STEP_INVERT);
	List.push_back(ALG_BARCODE_STEP_FLIP);
	List.push_back(ALG_BARCODE_STEP_FILL);
	List.push_back(ALG_BARCODE_STEP_ERODE);
	List.push_back(ALG_BARCODE_STEP_DILATE);
	List.push_back(ALG_BARCODE_STEP_FILL_2D);
	//List.push_back(ALG_BARCODE_STEP_SHARP);	
	List.push_back(ALG_BARCODE_STEP_GRAY_RANGE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::WriteAlgParamFile_BarcodeRecognize(const TALG_PARAM_BARCODE_RECOGNIZE &barParam, CAOIFileIO &FileIO)//儲存條碼辨識參數
{	
	size_t     i=0;	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_START, 0) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CODE_COUNT, barParam.brCodeCount) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CODE_COUNT_USED, barParam.brCodeCountEnabled) == false ) { return false; }

	if ( FileIO.SaveChunk_STR(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CODE_CONTENT, barParam.brBarcodeContent.c_str()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CODE_CONTENT_USED, barParam.brCodeContentEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CODE_JSON_CHK_USED, barParam.brCodeJSONCheckEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CHECK_SUM_USED, barParam.brCheckSumEnabled) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_VERIFY_USL, barParam.brVerifyUSL) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_VERIFY_LSL, barParam.brVerifyLSL) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_DECODER_TIMOUT, barParam.brDecodeTimeout) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_DIRECTION_MODE, barParam.brCodeDirectionMode) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_DECODE_START, barParam.brDecodeStartStep) == false ) { return false; }		

	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_1D_USED, barParam.br1DCodeEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_QRCODE_USED, barParam.brQRCodeEnabled) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_DATA_MATRIX_USED, barParam.brDataMatrixEnabled) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_1D_DECODER, barParam.br1DCodeDecoderType) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_QRCODE_DECODER, barParam.brQRCodeDecoderType) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_DATA_MATRIX_DECODER, barParam.brDataMatrixDecoderType) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_STEP_NODE, 0) == false ) { return false; }
	for ( i=0; i<MAX_BARCODE_DECODE_COUNT; i++ )
	{
		if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_STEP_START, 0) == false ) { return false; }
		if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_STEP_STEP_INDEX, i) == false ) { return false; }			
		if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_STEP_STEP_MODE, barParam.brDecodeStep[i].BarcodeStep) == false ) { return false; }
		if ( FileIO.SaveChunk_DBL(FILE_IO_BARCODE_STEP_PARAM_1, barParam.brDecodeStep[i].Param1) == false ) { return false; }
		if ( FileIO.SaveChunk_DBL(FILE_IO_BARCODE_STEP_PARAM_2, barParam.brDecodeStep[i].Param2) == false ) { return false; }
		if ( FileIO.SaveChunk_BOL(FILE_IO_BARCODE_STEP_ENABLED, barParam.brDecodeStep[i].Enabled) == false ) { return false; }		
		if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_STEP_END, 0) == false ) { return false; }
	}

	if ( FileIO.SaveChunk_INT(FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ReadAlgParamFile_BarcodeRecognize(TALG_PARAM_BARCODE_RECOGNIZE &barParam, CAOIFileIO &FileIO)//載入條碼辨識參數
{
	int       index = 0;
	size_t    szBarcodeIndex=0;
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		switch ( index )
		{
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_END://條碼辨識參數-終點
			return true;
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CODE_COUNT://字數
			barParam.brCodeCount = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CODE_COUNT_USED://字數啟用
			barParam.brCodeCountEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CODE_CONTENT://內容
			if ( FileIO.GetLoadWStr()==true )
			{	barParam.brBarcodeContent = FileIO.GetData_WSTR();	}
			else
			{	JetAPI::char2wstring(FileIO.GetData_STR(), barParam.brBarcodeContent);	}
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CODE_CONTENT_USED://內容啟用
			barParam.brCodeContentEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CODE_JSON_CHK_USED://內容JSON確認啟用	
			barParam.brCodeJSONCheckEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CHECK_SUM_USED://條碼的CheckSum啟用
			barParam.brCheckSumEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_VERIFY_USL://驗證上限
			barParam.brVerifyUSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_VERIFY_LSL://驗證下限
			barParam.brVerifyLSL = FileIO.GetData_DBL();
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_DECODER_TIMOUT://解碼逾時
			barParam.brDecodeTimeout = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_DIRECTION_MODE://方向模式
			barParam.brCodeDirectionMode = (ALG_BARCODE_DIR_MODE)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_DECODE_START://解碼開始步驟
			barParam.brDecodeStartStep = FileIO.GetData_INT();
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_1D_USED://1維條碼
			barParam.br1DCodeEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_QRCODE_USED://QR-Code
			barParam.brQRCodeEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_DATA_MATRIX_USED://Data Matrix
			barParam.brDataMatrixEnabled = FileIO.GetData_BOL();
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_1D_DECODER://1維條碼-解碼器
			barParam.br1DCodeDecoderType = (BARCODE_DECODER_TYPE)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_QRCODE_DECODER://QR-Code-解碼器
			barParam.brQRCodeDecoderType = (BARCODE_DECODER_TYPE)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_DATA_MATRIX_DECODER://Data Matrix-解碼器
			barParam.brDataMatrixDecoderType = (BARCODE_DECODER_TYPE)(FileIO.GetData_INT());
			break;
		case FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_STEP_NODE://解碼步驟
			szBarcodeIndex=0;
			break;
		case FILE_IO_BARCODE_STEP_START://解碼步驟參數-起點
			break;
		case FILE_IO_BARCODE_STEP_END://解碼步驟參數-終點
			szBarcodeIndex ++;
			break;
		case FILE_IO_BARCODE_STEP_STEP_INDEX://解碼步驟參數-引數
			index = FileIO.GetData_INT();
			break;
		case FILE_IO_BARCODE_STEP_STEP_MODE://解碼步驟參數-步驟
			if ( szBarcodeIndex<MAX_BARCODE_DECODE_COUNT )
			{	barParam.brDecodeStep[szBarcodeIndex].BarcodeStep = (ALG_BARCODE_STEP_MODE)(FileIO.GetData_INT());	}
			break;
		case FILE_IO_BARCODE_STEP_PARAM_1://解碼步驟參數-參數1
			if ( szBarcodeIndex<MAX_BARCODE_DECODE_COUNT )
			{	barParam.brDecodeStep[szBarcodeIndex].Param1 = (FileIO.GetData_DBL());	}
			break;
		case FILE_IO_BARCODE_STEP_PARAM_2://解碼步驟參數-參數2
			if ( szBarcodeIndex<MAX_BARCODE_DECODE_COUNT )
			{	barParam.brDecodeStep[szBarcodeIndex].Param2 = (FileIO.GetData_DBL());	}
			break;	
		case FILE_IO_BARCODE_STEP_ENABLED://解碼步驟參數-啟用
			if ( szBarcodeIndex<MAX_BARCODE_DECODE_COUNT )
			{	barParam.brDecodeStep[szBarcodeIndex].Enabled = (FileIO.GetData_BOL());	}
			break;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	}
	FileIO.SetErrorString(_T("Error, ReadAlgParamFile_BarcodeRecognize Fault"));
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgParam::ExecAlgInspection_BarcodeRecognize(CAOIModel *ModelPtr, const TREGION4D &ModelRgn, const RECT &RoiRect, const RECT &WndRect, std::vector<TUNI_FRAME> &UniFrameList)
{
	if ( NULL == ModelPtr ) { return false; }
	const size_t FrameCount = UniFrameList.size();
	const size_t ImageFrameIndex = m_AlgImageBinParam.GetBinaryFrameIndex();
	if ( ImageFrameIndex >= FrameCount ) 
	{	return false; }	
	CAOIWnd         *WndPtr = CAlgParam::GetAlgWndPtr();
	if ( NULL == WndPtr ) { return false; }

	size_t           i=0, j=0, k=0;
	const int        MaxBarcodeDecodeStep = MAX_BARCODE_DECODE_COUNT;
	TUNI_FRAME      *UniFramePtr  = &(UniFrameList[ImageFrameIndex]);
	const IMAGE_SIZE FrameImageW    = UniFramePtr->ImageW;
	const IMAGE_SIZE FrameImageH    = UniFramePtr->ImageH;
	const IMAGE_SIZE FrameImageStep = UniFramePtr->ImageStep;
	const IMAGE_SIZE FrameBitCount  = UniFramePtr->BitCount;		
	IMAGE_PTR        FrameImagePtr  = UniFramePtr->ImagePtr;
	MASK_PTR         FrameMaskPtr   = UniFramePtr->MaskPtr;
	SPACE_PTR        FrameSpacePtr  = UniFramePtr->SpacePtr;	
	TALG_BARCODE_STEP_PARAM       BarcodeStepParam;
	TALG_PARAM_BARCODE_RECOGNIZE &barParam=GetAlgParamBarcodeRecognize();
	const int RoiW = RoiRect.right-RoiRect.left;
	const int RoiH = RoiRect.bottom-RoiRect.top;
	const int RoiRectSize = (int)((RoiRect.right-RoiRect.left)*(RoiRect.bottom-RoiRect.top));
	ALG_BARCODE_DIR_MODE BarcodeCodeDirMode = barParam.brCodeDirectionMode;
	const int DecodeStartStep = barParam.brDecodeStartStep;
	
	IMAGE_SIZE    MaskW=0;
	IMAGE_SIZE    MaskH=0;	
	IMAGE_SIZE    MaskStep=0;
	IMAGE_SIZE    MaskBitCount=8;
	MASK_PTR      MaskPtr  = NULL;
	IMAGE_PTR     GrayPtr = NULL;	
	const bool    bTestWnd = false;

	if ( ExecAlgUniFrameBinary(m_AlgImageBinParam, WndRect, RoiRect, UniFrameList, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, GrayPtr, bTestWnd) == false )
	{		
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(GrayPtr);
		return false; 
	}

	CJetBarcode   JetBarcode;
	IMAGE_PTR     CodePtr = NULL;
	IMAGE_SIZE    CodeW=(RoiRect.right-RoiRect.left);
	IMAGE_SIZE    CodeH=(RoiRect.bottom-RoiRect.top);	
	IMAGE_SIZE    CodeBitCount = 8;	
	IMAGE_SIZE    CodeStep=JetAPI::GetBMPImagePixelsPerLine(CodeW, CodeBitCount, 4);	

	IMAGE_PTR     CodePtr2 = NULL;
	IMAGE_SIZE    CodeW2=0;
	IMAGE_SIZE    CodeH2=0;	
	IMAGE_SIZE    CodeStep2=0;
	IMAGE_SIZE    CodeBitCount2=8;		

	if ( true == barParam.brQRCodeEnabled )
	{	JetBarcode.SetQRCodeDecoder(barParam.brQRCodeDecoderType); }
	if ( true == barParam.br1DCodeEnabled )
	{	JetBarcode.SetBarcode1DDecoder(barParam.br1DCodeDecoderType);	}
	if ( true == barParam.brDataMatrixEnabled )
	{	JetBarcode.SetDataMatrixDecoder(barParam.brDataMatrixDecoderType);	}

	if ( BINARY_DISABLE == m_AlgImageBinParam.GetBinaryMode() )
	{	ImageAPI.ExtractRoiImage(MaskW, MaskH, MaskStep, 8, GrayPtr, RoiRect, CodeStep, CodePtr, false);	}
	else
	{	ImageAPI.ExtractRoiImage(MaskW, MaskH, MaskStep, 8, MaskPtr, RoiRect, CodeStep, CodePtr, false);	}	
	if ( NULL == CodePtr ) 
	{	
		JetMemory.free_func(GrayPtr);
		JetMemory.free_func(MaskPtr);
		return false;	
	}	
	
	bool IsOK = true;
	bool Enabled = true;
	bool Decoded = false;
	int  MatchCount = 0;	
	size_t CodeSize=0;
	size_t CodeSize1=0;
	size_t CodeSize2=0;
	double Gain=0;
	double Scale=0;
	double Offset=0;
	int  nKernelSize=0;
	int  nIterCount =0;
	char BarcodeContent[BARCODE_CONTENT_SIZE]="";
	const size_t BarcodeContentLen = barParam.brBarcodeContent.size();
	double USL=0.0, LSL=0.0, Reading=0.0;
	CString str;	
#ifdef _DEBUG
	bool   bSaved = false;
	CString      DebugFolder=GetAlgDebugFolder();
	if ( true == bSaved )
	{
		str.Format(_T("%s\\Barcode_%s"), DebugFolder, _T("Img.PNG"));
		ImageAPI.SaveImage(str, CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, true);
	}
#endif//_DEBUG
	for ( i=0; i<MaxBarcodeDecodeStep; i++ )
	{
		BarcodeStepParam = barParam.brDecodeStep[i];
		if ( ALG_BARCODE_STEP_NONE == BarcodeStepParam.BarcodeStep ) { continue; }

		IsOK = true;
		Decoded = false;
		CodeW2 = CodeW;
		CodeH2 = CodeH;
		CodeStep2 = CodeStep;
		Enabled = BarcodeStepParam.Enabled;
		::memset(BarcodeContent, 0x00, sizeof(BarcodeContent));
		switch ( BarcodeStepParam.BarcodeStep )
		{
		case ALG_BARCODE_STEP_SCALE:
			Scale = BarcodeStepParam.Param1;
			IsOK = ImageAPI.ScaleImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, Scale, CodeW2, CodeH2, CodeStep2, CodePtr2);			
			break;
		case ALG_BARCODE_STEP_GAIN_OFFSET:
			Gain = BarcodeStepParam.Param1;
			Offset = -BarcodeStepParam.Param2;
			IsOK = ImageAPI.ImageOffsetGain(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, CodePtr2, Offset, Gain);
			break;
		case ALG_BARCODE_STEP_SMOOTH:
			nKernelSize = (int)(BarcodeStepParam.Param1);
			if ( (nKernelSize%2) == 0 ) { nKernelSize += 1; }
			IsOK = ImageAPI.SmoothImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, nKernelSize, CodePtr2);
			break;
		case ALG_BARCODE_STEP_OPEN:
			nKernelSize = (int)(BarcodeStepParam.Param1);
			nIterCount  = (int)(BarcodeStepParam.Param2);
			if ( (nKernelSize%2) == 0 ) { nKernelSize += 1; }
			if ( nIterCount <= 0 ) { nIterCount = 1; }
			IsOK = ImageAPI.MorphImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, MORPH_OPEN, MORPH_SHAPE_RECT, nKernelSize, nIterCount, CodePtr2);
			break;
		case ALG_BARCODE_STEP_CLOSE:
			nKernelSize = (int)(BarcodeStepParam.Param1);
			nIterCount  = (int)(BarcodeStepParam.Param2);
			if ( (nKernelSize%2) == 0 ) { nKernelSize += 1; }
			if ( nIterCount <= 0 ) { nIterCount = 1; }
			IsOK = ImageAPI.MorphImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, MORPH_CLOSE, MORPH_SHAPE_RECT, nKernelSize, nIterCount, CodePtr2);
			break;
		case ALG_BARCODE_STEP_MEDIAN:
			nKernelSize = (int)(BarcodeStepParam.Param1);
			if ( (nKernelSize%2) == 0 ) { nKernelSize += 1; }
			IsOK = ImageAPI.MedianImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, nKernelSize, CodePtr2);			
			break;
		case ALG_BARCODE_STEP_INVERT:
			IsOK = ImageAPI.InvertImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, CodePtr2);
			break;
		case ALG_BARCODE_STEP_FLIP:
			IsOK = ImageAPI.ReverseImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, CodePtr2);
			break;
		case ALG_BARCODE_STEP_FILL:
			nKernelSize = (int)(BarcodeStepParam.Param1);
			if ( (nKernelSize%2) == 0 ) { nKernelSize += 1; }
			IsOK = ImageAPI.FillBarcodeImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, nKernelSize, CodePtr2);
			break;
		case ALG_BARCODE_STEP_ERODE:
			nKernelSize = (int)(BarcodeStepParam.Param1);
			nIterCount  = (int)(BarcodeStepParam.Param2);
			if ( (nKernelSize%2) == 0 ) { nKernelSize += 1; }
			if ( nIterCount <= 0 ) { nIterCount = 1; }
			IsOK = ImageAPI.ErodeImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, nKernelSize, nIterCount, CodePtr2);			
			break;
		case ALG_BARCODE_STEP_DILATE:
			nKernelSize = (int)(BarcodeStepParam.Param1);
			nIterCount  = (int)(BarcodeStepParam.Param2);
			if ( (nKernelSize%2) == 0 ) { nKernelSize += 1; }
			if ( nIterCount <= 0 ) { nIterCount = 1; }
			IsOK = ImageAPI.DilateImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, nKernelSize, nIterCount, CodePtr2);			
			break;
		case ALG_BARCODE_STEP_FILL_2D:
			nKernelSize = (int)(BarcodeStepParam.Param1);
			if ( (nKernelSize%2) == 0 ) { nKernelSize += 1; }
			IsOK = ImageAPI.FillBarcode2DImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, nKernelSize, CodePtr2);
			break;
		case ALG_BARCODE_STEP_SHARP:			
			IsOK = ImageAPI.SharpnessGausImage(CodeW, CodeH, CodeStep, CodeBitCount, CodePtr, 5, 5, 200, CodePtr2);
			break;
		case ALG_BARCODE_STEP_GRAY_RANGE:
			IsOK = ImageAPI.RangeGrayImage(CodeW, CodeH, CodeStep, CodePtr, CodePtr2, BarcodeStepParam.Param1, BarcodeStepParam.Param2);
			break;
		}	
		if ( false == IsOK )
		{	break;	}

	#ifdef _DEBUG
		if ( true == bSaved )
		{
			str.Format(_T("%s\\Barcode_Step%d_%s"), DebugFolder, i+1, _T("Img.PNG"));
			ImageAPI.SaveImage(str, CodeW2, CodeH2, CodeStep2, CodeBitCount, CodePtr2, true);
		}
	#endif//_DEBUG

		if ( i >= DecodeStartStep && true==Enabled )
		{
			if ( false==Decoded && true==barParam.br1DCodeEnabled )
			{				
				switch ( BarcodeCodeDirMode ) 
				{
				case ALG_BARCODE_DIR_AUTO:
					if ( CodeW > CodeH ) 
					{	JetBarcode.SetBarcodeDirection(BARCODE_DIRECTION_HOR);	}
					else
					{	JetBarcode.SetBarcodeDirection(BARCODE_DIRECTION_VER);	}
					break;
				case ALG_BARCODE_DIR_HOR:
					JetBarcode.SetBarcodeDirection(BARCODE_DIRECTION_HOR);
					break;
				case ALG_BARCODE_DIR_VER:
					JetBarcode.SetBarcodeDirection(BARCODE_DIRECTION_VER);
					break;
				default://ALG_BARCODE_DIR_ALL
					JetBarcode.SetBarcodeDirection(BARCODE_DIRECTION_ALL);
					break;
				}				
				JetBarcode.SetDecodeTimeout(barParam.brDecodeTimeout);
				JetBarcode.SetEnableCheckSum(barParam.brCheckSumEnabled);
				IsOK = JetBarcode.DecodeBarcode1D(CodeW2, CodeH2, CodeStep2, CodePtr2, BarcodeContent, BARCODE_CONTENT_SIZE);
				if ( true == IsOK )
				{
					CodeSize1 = ::strlen(BarcodeContent);
					if ( CodeSize1 > 0 )
					{	Decoded = true;	}
				}
			}
		
			if ( false==Decoded && true==barParam.brQRCodeEnabled )
			{				
				JetBarcode.SetBarcodeDirection(BARCODE_DIRECTION_ALL);
				JetBarcode.SetDecodeTimeout(barParam.brDecodeTimeout);
				JetBarcode.SetEnableCheckSum(barParam.brCheckSumEnabled);
				IsOK = JetBarcode.DecodeBarcodeQRCode(CodeW2, CodeH2, CodeStep2, CodePtr2, BarcodeContent, BARCODE_CONTENT_SIZE);
				if ( true == IsOK )
				{
					CodeSize1 = ::strlen(BarcodeContent);
					if ( CodeSize1 > 0 )
					{	Decoded = true;	}
				}
			}

			if ( false==Decoded && true==barParam.brDataMatrixEnabled )		
			{				
				JetBarcode.SetBarcodeDirection(BARCODE_DIRECTION_ALL);
				JetBarcode.SetDecodeTimeout(barParam.brDecodeTimeout);		
				JetBarcode.SetEnableCheckSum(barParam.brCheckSumEnabled);
				IsOK = JetBarcode.DecodeBarcodeDataMatrix(CodeW2, CodeH2, CodeStep2, CodePtr2, BarcodeContent, BARCODE_CONTENT_SIZE);
				if ( true == IsOK )
				{
					CodeSize1 = ::strlen(BarcodeContent);
					if ( CodeSize1 > 0 )
					{	Decoded = true;	}
				}
			}

			if ( true == Decoded )
			{
				if ( true == barParam.brCodeJSONCheckEnabled )
				{
					if ( JetAPI::CheckJSONStringA(BarcodeContent) == false )
					{	Decoded = false;	}
				}
			}

			if ( true == Decoded )
			{				
				barParam.brDecodeStep[i].Decoded = true;
				barParam.brCodeCountReading = ::strlen(BarcodeContent);
				JetAPI::char2wstring(BarcodeContent, barParam.brBarcodeResult);			
				if ( false==barParam.brCodeContentEnabled || 0==BarcodeContentLen )
				{	barParam.brVerifyReading = 100.0;	}
				else
				{				
					CodeSize2 = barParam.brBarcodeContent.size();
					CodeSize = MIN(CodeSize1, CodeSize2);
					MatchCount = 0;
					for ( j=0; j<CodeSize; j++ )
					{
						if ( BarcodeContent[j] != barParam.brBarcodeContent[j] )
						{	continue;	}
						MatchCount ++;
					}
					if ( CodeSize > 0 ) 
					{	barParam.brVerifyReading = 100.0*MatchCount/CodeSize;	}
					else
					{	barParam.brVerifyReading = 100.0; }
				}			
				//重新確認條碼內容是否正確
				if ( true == barParam.brCodeCountEnabled )
				{
					if ( barParam.brCodeCountReading != barParam.brCodeCount )
					{	Decoded = false;	}
				}

				if ( true==barParam.brCodeContentEnabled && 0!=BarcodeContentLen )
				{
					USL = barParam.brVerifyUSL;
					LSL = barParam.brVerifyLSL;
					Reading = barParam.brVerifyReading;
					if ( Reading>USL || Reading<LSL )
					{	Decoded = false;	}
				}
				if ( true == Decoded )
				{	break; }
			}
		}
		JetMemory.free_func(CodePtr);

		CodePtr = CodePtr2;
		CodeW = CodeW2;
		CodeH = CodeH2;
		CodeStep = CodeStep2;
		CodeBitCount = CodeBitCount2;

		CodePtr2 = NULL;
	}
	JetMemory.free_func(CodePtr);
	JetMemory.free_func(CodePtr2);

	//Judge OK/NG
	CString strResult;	
	SetAlgResultReading1(Reading);	
	SetAlgResultText(_T(""));	
	if ( false == Decoded )
	{	SetAlgResultID(RESULT_ID_NG);	 }
	else
	{		
		SetAlgResultID(RESULT_ID_OK);	
		strResult = barParam.brBarcodeResult.c_str();
		SetAlgResultText(strResult);
		if ( true == barParam.brCodeCountEnabled )
		{
			if ( barParam.brCodeCountReading != barParam.brCodeCount )
			{	SetAlgResultID(RESULT_ID_NG);;	}
		}

		if ( true==barParam.brCodeContentEnabled && 0!=BarcodeContentLen )
		{
			USL = barParam.brVerifyUSL;
			LSL = barParam.brVerifyLSL;
			Reading = barParam.brVerifyReading;
			if ( Reading>USL || Reading<LSL )
			{	SetAlgResultID(RESULT_ID_NG);;	}
		}
	}
	WndPtr->SetWndResultID(m_AlgResultID);
	SaveAlgDefectImage(MaskW, MaskH, MaskStep, MaskBitCount, GrayPtr, MaskPtr, WndRect);
	JetMemory.free_func(GrayPtr);
	JetMemory.free_func(MaskPtr);
	return true;
}
//-------------------------------------------------------------------------------------//