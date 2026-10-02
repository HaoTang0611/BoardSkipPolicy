// EditWndView_BarcodeRecognize.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditWndView.h"
//-------------------------------------------------------------------------------------//
#include "AlgBarcodeRecognizeWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_BarcodeRecognize(CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }	
	
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const TALG_PARAM_BARCODE_RECOGNIZE &barParam = AlgParam.GetAlgParamBarcodeRecognize();

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	bool    bIsPass = true;
	CString strReadingS;
	CString strCaption, strValue, strDescr, strUnit;	
	CJETPropertyGridProperty* pParamItem = NULL;	
	CJETPropertyGridProperty* pGroupBasic = NULL;
	CJETPropertyGridProperty* pGroupType = NULL;
	CJETPropertyGridProperty* pGroupAdvanced = NULL;
	CJETPropertyGridProperty* pGroupResult = NULL;

	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;
	wndPropList.SetRedraw(FALSE);
	
	const bool   Code1DEnabled = barParam.br1DCodeEnabled;//1D啟用
	const bool   QRCodeEnabled = barParam.brQRCodeEnabled;//QRCode啟用		
	const bool   DataMatrixEnabled = barParam.brDataMatrixEnabled;//DataMatrix啟用	
	BARCODE_DECODER_TYPE Code1D_Decoder=barParam.br1DCodeDecoderType;
	BARCODE_DECODER_TYPE QRCode_Decoder=barParam.brQRCodeDecoderType;
	BARCODE_DECODER_TYPE DataMatrix_Decoder=barParam.brDataMatrixDecoderType;

	//條碼樣式
	strCaption = _T("Barcode Type");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pGroupType = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupType ) { return false; }	
	pGroupType->SetID(WND_ALG_PROPERTY_BARCODE_ENABLE_BEGIN);
	pGroupType->SetData((DWORD_PTR)WndPtr);	
	wndPropList.AddProperty(pGroupType, bRedraw, bAdjustLayou);

	strCaption = _T("1D Barcode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	if ( true == Code1DEnabled )
	{	strValue = _T("Enable"); }
	else
	{	strValue = _T("Disable"); }	
	strValue = LoadMultiLanguageString(strValue, strValue);
	pParamItem = CreateGridPropertyBarcodeDecoderList(strCaption, Code1D_Decoder, WndPtr, strDescr);
	if ( NULL == pParamItem ) 
	{	return false;	}	
	pParamItem->SetID(WND_ALG_PROPERTY_BARCODE_1D_ENABLED);	
	pParamItem->SetCheckValue(Code1DEnabled);	
	pGroupType->AddSubItem(pParamItem);

	strCaption = _T("QR Code");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	if ( true == QRCodeEnabled )
	{	strValue = _T("Enable"); }
	else
	{	strValue = _T("Disable"); }	
	strValue = LoadMultiLanguageString(strValue, strValue);
	pParamItem = CreateGridPropertyBarcodeDecoderList(strCaption, QRCode_Decoder, WndPtr, strDescr);
	if ( NULL == pParamItem ) 
	{	return false;	}	
	pParamItem->SetID(WND_ALG_PROPERTY_BARCODE_QRCODE_ENABLED);	
	pParamItem->SetCheckValue(QRCodeEnabled);	
	pGroupType->AddSubItem(pParamItem);

	strCaption = _T("Data Matrix");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	if ( true == DataMatrixEnabled )
	{	strValue = _T("Enable"); }
	else
	{	strValue = _T("Disable"); }	
	strValue = LoadMultiLanguageString(strValue, strValue);
	pParamItem = CreateGridPropertyBarcodeDecoderList(strCaption, DataMatrix_Decoder, WndPtr, strDescr);
	if ( NULL == pParamItem ) 
	{	return false;	}	
	pParamItem->SetID(WND_ALG_PROPERTY_BARCODE_DATA_MATRIX_ENABLED);	
	pParamItem->SetCheckValue(DataMatrixEnabled);
	pGroupType->AddSubItem(pParamItem);
	pGroupType->Expand(FALSE);	

	strCaption = FormWndParamListCategoryName(WndPtr);	
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_BARCODE_BEGIN);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);
	if ( BuildWndParamList_WndDefectID(pGroupBasic, Project, WndPtr) == false )
	{	return false;	}	
	if ( BuildWndParamList_AlgFrameIndex(pGroupBasic, Project, WndPtr) == false )
	{	return false;	}

	//依照各種演算法外增加	
	int    i=0;
	double USL=0, LSL=0, Reading=0.0;	
	const int    DecodeTimeout = barParam.brDecodeTimeout;
	const int    DecodeStartStep = barParam.brDecodeStartStep;
	const int    CodeCount = barParam.brCodeCount;//條碼長度
	const int    CodeCountReading = barParam.brCodeCountReading;
	const bool   CodeCountEnabled = barParam.brCodeCountEnabled;//條碼長度啟用
	CString      CodeContent = barParam.brBarcodeContent.c_str();//條碼內容
	const bool   CodeContentEnabled = barParam.brCodeContentEnabled;//條碼內容啟用
	const bool   CodeJSONChkEnabled = barParam.brCodeJSONCheckEnabled;//條碼內容JSON確認啟用
	const bool   CheckSumEnabled = barParam.brCheckSumEnabled;
	const double VerifyUSL = barParam.brVerifyUSL;//驗證上限
	const double VerifyLSL = barParam.brVerifyLSL;//驗證下限	
	const double VerifyReading = barParam.brVerifyReading;//讀值
	ALG_BARCODE_DIR_MODE BarcodeDirMode = barParam.brCodeDirectionMode;
	CString      CodeResult = barParam.brBarcodeResult.c_str();		

	Reading = 0;	
	strCaption = _T("Char Count");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), CodeCount);
	strReadingS.Format(_T("%d"), CodeCountReading);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BARCODE_CODE_COUNT);
	pParamItem->SetReading(strReadingS);
	pParamItem->SetCheckValue(CodeCountEnabled);
	if ( true == CodeCountEnabled )
	{
		if ( (0==CodeCount) || CodeCount==CodeCountReading )
		{	pParamItem->SetReadingTextColor(clrOK);		}
		else
		{	pParamItem->SetReadingTextColor(clrNG);		}
	}
	else
	{	pParamItem->SetReadingTextColor(clrOK);	}	
	pGroupBasic->AddSubItem(pParamItem);

	//CodeContentEnabled
	Reading = 0;
	strCaption = _T("Char Compare");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%s"), CodeContent);
	strReadingS.Format(_T("%s"), CodeResult);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BARCODE_CODE_CONTENT);	
	pParamItem->SetCheckValue(CodeContentEnabled);
	if ( true == CodeContentEnabled )
	{
		if ( (0==CodeCount) || CodeCount==CodeCountReading )
		{	pParamItem->SetReadingTextColor(clrOK);		}
		else
		{	pParamItem->SetReadingTextColor(clrNG);		}
	}
	else
	{	pParamItem->SetReadingTextColor(clrOK);	}	
	pGroupBasic->AddSubItem(pParamItem);

	//CodeJSONChkEnabled
	Reading = 0;
	strCaption = _T("JSON Check");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	if ( true == CodeJSONChkEnabled )
	{	strValue = _T("Enable"); }
	else
	{	strValue = _T("Disable"); }	
	strValue = LoadMultiLanguageString(strValue, strValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BARCODE_CODE_JSON_CHECK_ENABLE);		
	pParamItem->SetCheckValue(CodeJSONChkEnabled);	
	pGroupBasic->AddSubItem(pParamItem);

	//CheckSumEnabled
	Reading = 0;
	strCaption = _T("Check Sum");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	if ( true == CheckSumEnabled )
	{	strValue = _T("Enable"); }
	else
	{	strValue = _T("Disable"); }	
	strValue = LoadMultiLanguageString(strValue, strValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BARCODE_CHECK_SUM_ENABLE);		
	pParamItem->SetCheckValue(CheckSumEnabled);	
	pGroupBasic->AddSubItem(pParamItem);		

	strCaption = _T("Char Result");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%s"), CodeResult);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BARCODE_CODE_RESULT);	
	//pParamItem->Enable(FALSE);	
	pGroupBasic->AddSubItem(pParamItem);

	USL = VerifyUSL;
	LSL = VerifyLSL;
	Reading = VerifyReading;
	strReadingS.Format(_T("%.0f"), Reading);
	
	strCaption = _T("Verify USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), USL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BARCODE_VERIFY_USL);
	pParamItem->SetReading(strReadingS);
	if ( true == CodeContentEnabled )
	{
		bIsPass = CAlgParam::CheckOK_BarcodeVerifyUSL(barParam);
		if ( false == bIsPass )
		{	pParamItem->SetReadingTextColor(clrNG); }
		else
		{	pParamItem->SetReadingTextColor(clrOK); }
	}
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("Verify LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), LSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BARCODE_VERIFY_LSL);
	pParamItem->SetReading(strReadingS);	
	if ( true == CodeContentEnabled )
	{
		bIsPass = CAlgParam::CheckOK_BarcodeVerifyLSL(barParam);
		if ( false == bIsPass )
		{	pParamItem->SetReadingTextColor(clrNG); }
		else
		{	pParamItem->SetReadingTextColor(clrOK); }
	}
	pGroupBasic->AddSubItem(pParamItem);	
	
	//結果文字顯示
	if ( BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false )
	{	return false; }

	//進階設定
	strCaption = _T("Setup");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("");
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BARCODE_SETUP_BTN);		
	pParamItem->SetHasUserBtn();
	pParamItem->AllowEdit(FALSE);
	pGroupBasic->AddSubItem(pParamItem);	

	//邏輯設定
	if ( BuildWndParamList_LogicParam(wndPropList, Project, WndPtr) == false )
	{	return false; }

	CString str;
	WND_ALG_PROPERTY_ID ParamID;
	TALG_BARCODE_STEP_PARAM    BarcodeDecodeParam;//解碼步驟
	CJETPropertyGridProperty* pGroupStep = NULL;
	for ( i=0; i<MAX_BARCODE_DECODE_COUNT; i++ )
	{
		switch ( i )
		{
		case 0: ParamID=WND_ALG_PROPERTY_BARCODE_DECODE_STEP_1;	break;
		case 1: ParamID=WND_ALG_PROPERTY_BARCODE_DECODE_STEP_2;	break;
		case 2: ParamID=WND_ALG_PROPERTY_BARCODE_DECODE_STEP_3;	break;
		case 3: ParamID=WND_ALG_PROPERTY_BARCODE_DECODE_STEP_4;	break;
		case 4: ParamID=WND_ALG_PROPERTY_BARCODE_DECODE_STEP_5;	break;
		case 5: ParamID=WND_ALG_PROPERTY_BARCODE_DECODE_STEP_6;	break;
		case 6: ParamID=WND_ALG_PROPERTY_BARCODE_DECODE_STEP_7;	break;
		case 7: ParamID=WND_ALG_PROPERTY_BARCODE_DECODE_STEP_8;	break;
		}

		str = _T("Step");
		str = LoadMultiLanguageString(str, str);
		strCaption.Format(_T("%s %d"), str, i+1);		
		BarcodeDecodeParam = barParam.brDecodeStep[i];
		pGroupStep = CreateGridPropertyBarcodeStepParam(strCaption, BarcodeDecodeParam, ParamID, strDescr, WndPtr);
		if ( NULL == pGroupStep ) 
		{	return false;	}
		//pGroupBasic->AddSubItem(pGroupStep);
		wndPropList.AddProperty(pGroupStep, bRedraw, bAdjustLayou);
	}	

	//進階設定
	strCaption = _T("Advance");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pGroupAdvanced = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupAdvanced ) { return false; }	
	pGroupAdvanced->SetID(WND_ALG_PROPERTY_ADVANCE_BEGIN);
	pGroupAdvanced->SetData((DWORD_PTR)WndPtr);	
	wndPropList.AddProperty(pGroupAdvanced, bRedraw, bAdjustLayou);

	//解碼逾時	
	strCaption = _T("Decode Timeout");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), DecodeTimeout);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BARCODE_DECODE_TIMEOUT);		
	pGroupAdvanced->AddSubItem(pParamItem);

	//解碼開始步驟
	strCaption = _T("Decode Start Step");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), DecodeStartStep+1);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BARCODE_DECODE_START_STEP);		
	for ( i=0; i<MAX_BARCODE_DECODE_COUNT; i++ )
	{	
		strValue.Format(_T("%d"), i+1);
		pParamItem->AddOption(strValue);
	}
	pGroupAdvanced->AddSubItem(pParamItem);

	//條碼方向
	strCaption = _T("Direction");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	pParamItem = CreateGridPropertyBarcodeDirModeList(strCaption, BarcodeDirMode, strDescr, WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BARCODE_DIRECTION_MODE);		
	pGroupAdvanced->AddSubItem(pParamItem);

	if ( BuildWndParamList_AdvanceGeneral(pGroupAdvanced, Project, WndPtr) == false )	
	{	return false;	}
	pGroupAdvanced->Expand(FALSE);	

	if ( FALSE == bAdjustLayou )
	{	wndPropList.AdjustLayout(); }
	wndPropList.SetRedraw(TRUE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamList_BarcodeRecognizeWnd(CAOIWnd *WndPtr, bool bUpdateModel)
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	if ( NULL == WndPtr ) { return false; }	

	INT_PTR Ret=0;
	const bool bExtend = false;
	CAlgBarcodeRecognizeWnd BarcodeWnd;
	std::vector<TUNI_FRAME> WndUniFrameList;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam& AlgBinParam = AlgParam.GetAlgImageBinParam();
	TALG_PARAM_BARCODE_RECOGNIZE barParam= AlgParam.GetAlgParamBarcodeRecognize();	
	const unsigned int FrameIndex = AlgBinParam.GetBinaryFrameIndex();

	if ( AOIDataCollect.CreateWndUniFrameListByField(bExtend, WndPtr, WndUniFrameList) == false ) 	
	{	return false;	}

	BarcodeWnd.SetBinaryParam(AlgBinParam);
	BarcodeWnd.SetBarcodeRecognizeParam(barParam);
	BarcodeWnd.SetUniFrameList(FrameIndex, WndUniFrameList);
	Ret = BarcodeWnd.DoModal();
	JetAPI::ClearUniFrameList(WndUniFrameList);
	if ( IDCANCEL == Ret ) 
	{	return true; }
	WndPtr->SetWndUIUpated_Param(false);
	BarcodeWnd.GetBarcodeRecognizeParam(barParam);
	AlgParam.SetAlgParamBarcodeRecognize(barParam);		

	if ( true == bUpdateModel )
	{
		TWND_PARAM_CHANGED_RESULT Changed;
		Changed.bParamChanged = true;
		ExecWndParamChangedUpdate(ModelPtr, WndPtr, Changed);	
	}
	CWnd::PostMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_REBUILD_WND_PARAM_LIST, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_BarcodeRecognize(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();		
	TALG_PARAM_BARCODE_RECOGNIZE   &barParam  = AlgParam.GetAlgParamBarcodeRecognize();

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	bool          bIsPass = true;
	bool          bChanged = false;	
	bool          bBoolParam = false;
	bool          bReBuildWndUI = false;
	int           strLen=0;
	int           nBarcodeParamIdx=0;
	int           nValue=0, nReading=0;
	bool          bValue=false, bReading=false, bEnabled=true;
	BOOL          bShow=TRUE;
	BOOL          BValue=FALSE, BReading=FALSE;
	double        dValue=0.0, dReading=0.0;	
	ALG_BARCODE_STEP_MODE  BarcodeStep;	
	TALG_BARCODE_STEP_PARAM  StepParam;
	CString       strStep;
	CString       strValue;
	CString       strValue2;
	CString       strDisable;
	CString       strResult;
	CString       strName2;	
	CString       strName = pProp->GetName();
	CString       strDescr = pProp->GetDescription();
	COleVariant   vtValue = pProp->GetValue();	
	CString       strNewValue(pProp->GetValue());
	CString       strOldValue(pProp->GetOriginalValue());
	DWORD_PTR     dwData = pProp->GetData();	
	BARCODE_DECODER_TYPE BarcodeDecoder=BARCODE_DECODER_OFF;
	ALG_BARCODE_DIR_MODE BarcodeDirMode=ALG_BARCODE_DIR_ALL;
	CJETPropertyGridProperty *pProp2 = NULL;		
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());	
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;
	const int MaxBarcodeDecodeCount = MAX_BARCODE_DECODE_COUNT;

	switch ( ParamID )
	{
	case WND_ALG_PROPERTY_BARCODE_ENABLE_BEGIN:
		break;
	case WND_ALG_PROPERTY_BARCODE_1D_ENABLED:
	case WND_ALG_PROPERTY_BARCODE_1D_DECODER:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		if ( bEnabled != barParam.br1DCodeEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
			barParam.br1DCodeEnabled = bEnabled;
		}
		BarcodeDecoder = AOIDataDefine.FindBarcodeDecoderTypeByText(strValue);
		if ( BarcodeDecoder != barParam.br1DCodeDecoderType )
		{
			pProp->SetOriginalValue(strValue);
			barParam.br1DCodeDecoderType = BarcodeDecoder;	
		}		
		bChanged = true;	
		break;
	case WND_ALG_PROPERTY_BARCODE_QRCODE_ENABLED:
	case WND_ALG_PROPERTY_BARCODE_QRCODE_DECODER:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();		
		if ( bEnabled != barParam.brQRCodeEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
			barParam.brQRCodeEnabled = bEnabled;		
		}
		BarcodeDecoder = AOIDataDefine.FindBarcodeDecoderTypeByText(strValue);
		if ( BarcodeDecoder != barParam.brQRCodeDecoderType )
		{
			pProp->SetOriginalValue(strValue);
			barParam.brQRCodeDecoderType = BarcodeDecoder;	
		}		
		bChanged = true;
		break;		
	case WND_ALG_PROPERTY_BARCODE_DATA_MATRIX_ENABLED:
	case WND_ALG_PROPERTY_BARCODE_DATA_MATRIX_DECODER:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		if ( bEnabled != barParam.brDataMatrixEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
			barParam.brDataMatrixEnabled = bEnabled;	
		}
		BarcodeDecoder = AOIDataDefine.FindBarcodeDecoderTypeByText(strValue);
		if ( BarcodeDecoder != barParam.brDataMatrixDecoderType )
		{
			pProp->SetOriginalValue(strValue);
			barParam.brDataMatrixDecoderType = BarcodeDecoder;	
		}
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_TIMEOUT:
		strValue = pProp->GetValue();		
		nValue = ::_ttoi(strValue);
		if ( nValue >= 0 ) 
		{
			barParam.brDecodeTimeout = nValue;
			bChanged = true;
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_START_STEP:
		strValue = pProp->GetValue();		
		nValue = ::_ttoi(strValue)-1;
		if ( nValue>=0 && nValue<MaxBarcodeDecodeCount ) 
		{
			barParam.brDecodeStartStep = nValue;
			bChanged = true;
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DIRECTION_MODE:
		strValue = pProp->GetValue();
		BarcodeDirMode = AOIDataDefine.FindBarcodeDirectionModeByText(strValue);
		//if ( TRUE == bClickBtn )
		{
			barParam.brCodeDirectionMode = BarcodeDirMode;
			bChanged = true;
		}		
		break;
	case WND_ALG_PROPERTY_BARCODE_CODE_COUNT:
		strValue = pProp->GetValue();		
		nValue = ::_ttoi(strValue);
		bEnabled = pProp->GetCheckValue();
		if ( nValue >= 0 )
		{
			barParam.brCodeCount = nValue;
			barParam.brCodeCountEnabled = bEnabled;			
			bChanged = true;
		}
		else
		{
			strValue = _T("0");
			pProp->SetValue(strValue);
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_CODE_CONTENT:
		strValue = pProp->GetValue();		
		strLen = strValue.GetLength();
		bEnabled = pProp->GetCheckValue();
		barParam.brCodeContentEnabled = bEnabled;
		JetAPI::TCHAR2wstring(strValue, barParam.brBarcodeContent);
		bChanged = true;		
		break;	
	case WND_ALG_PROPERTY_BARCODE_CODE_JSON_CHECK_ENABLE:
		bEnabled = pProp->GetCheckValue();		
		barParam.brCodeJSONCheckEnabled = bEnabled;		
		if ( true == bEnabled )
		{	strValue = _T("Enable"); }
		else
		{	strValue = _T("Disable"); }
		strValue = LoadMultiLanguageString(strValue, strValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BARCODE_CHECK_SUM_ENABLE:
		bEnabled = pProp->GetCheckValue();		
		barParam.brCheckSumEnabled = bEnabled;		
		if ( true == bEnabled )
		{	strValue = _T("Enable"); }
		else
		{	strValue = _T("Disable"); }
		strValue = LoadMultiLanguageString(strValue, strValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BARCODE_CODE_RESULT:
		strValue = barParam.brBarcodeResult.c_str();
		pProp->SetValue(strValue);
		break;
	case WND_ALG_PROPERTY_BARCODE_VERIFY_USL:
		strValue = pProp->GetValue();	
		dValue = ::_tcstod(strValue, NULL);
		barParam.brVerifyUSL = dValue;
		bIsPass = CAlgParam::CheckOK_BarcodeVerifyUSL(barParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG);	}
		else
		{	pProp->SetReadingTextColor(clrOK);	}
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_BARCODE_VERIFY_LSL:
		strValue = pProp->GetValue();	
		dValue = ::_tcstod(strValue, NULL);
		barParam.brVerifyLSL = dValue;
		bIsPass = CAlgParam::CheckOK_BarcodeVerifyLSL(barParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG);	}
		else
		{	pProp->SetReadingTextColor(clrOK);	}
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BARCODE_SETUP_BTN:
		if ( ExecWndParamList_BarcodeRecognizeWnd(WndPtr, false) == true ) 
		{
			bChanged = true;	
			bReBuildWndUI = true;
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_STEP_1:
		nBarcodeParamIdx = 0;		
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_1:
		nBarcodeParamIdx = 0;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{
			BarcodeStep = barParam.brDecodeStep[nBarcodeParamIdx].BarcodeStep;
			strValue = AOIDataDefine.GetAlgBarcodeDecodeStepText(BarcodeStep);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			BarcodeStep = AOIDataDefine.FindAlgBarcodeStepModeByText(strValue);
			barParam.brDecodeStep[nBarcodeParamIdx].BarcodeStep = BarcodeStep;
			bChanged = true;			
			
			if ( true == barParam.brDecodeStep[nBarcodeParamIdx].Decoded )
			{	strResult = _T("OK");	}
			else
			{	strResult = _T("NG");	}
			strStep = _T("Step");
			strStep = LoadMultiLanguageString(strStep, strStep);
			strName2.Format(_T("%s 1 [%s] [%s]"), strStep, strValue, strResult);				
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_STEP_1);
			if ( NULL != pProp2 )
			{	pProp2->SetName(strName2);	}

			StepParam.BarcodeStep = BarcodeStep;
			CAlgParam::DefaultAlgBarcodeStepParam(StepParam);
			strDisable = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(ALG_BARCODE_STEP_NONE, 0);			

			strValue2.Format(_T("%.2f"), StepParam.Param1);
			strName2 = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(BarcodeStep, 0);
			if ( strName2 == strDisable ) { bShow = FALSE; }
			else { bShow = TRUE; }
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_1);
			if ( NULL != pProp2 )
			{					
				pProp2->Show(bShow);	
				pProp2->SetName(strName2);
				pProp2->SetValue(strValue2);
			}

			strValue2.Format(_T("%.2f"), StepParam.Param2);
			strName2 = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(BarcodeStep, 1);
			if ( strName2 == strDisable ) { bShow = FALSE; }
			else { bShow = TRUE; }
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_1);
			if ( NULL != pProp2 )
			{				
				pProp2->Show(bShow);	
				pProp2->SetName(strName2);
				pProp2->SetValue(strValue2);
			}
			barParam.brDecodeStep[nBarcodeParamIdx] = StepParam;
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_1:
		nBarcodeParamIdx = 0;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{			
			strValue.Format(_T("%.2f"),  barParam.brDecodeStep[nBarcodeParamIdx].Param1);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);
			barParam.brDecodeStep[nBarcodeParamIdx].Param1 = dValue;
			bChanged = true;
			
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_1:
		nBarcodeParamIdx = 0;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{			
			strValue.Format(_T("%.2f"),  barParam.brDecodeStep[nBarcodeParamIdx].Param2);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);
			barParam.brDecodeStep[nBarcodeParamIdx].Param2 = dValue;
			bChanged = true;
			
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_1:
		nBarcodeParamIdx = 0;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{	
			bEnabled = barParam.brDecodeStep[nBarcodeParamIdx].Enabled;			
			pProp->SetCheckValue(bEnabled);
		}
		else
		{
			bEnabled = pProp->GetCheckValue();
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
			strValue = AOIDataDefine.GetEnableDisableText(bEnabled);			
			barParam.brDecodeStep[nBarcodeParamIdx].Enabled = bEnabled;			
			pProp->SetValue(strValue);
			bChanged = true;
		}		
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_1:
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_STEP_2:
		nBarcodeParamIdx = 1;		
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_2:
		nBarcodeParamIdx = 1;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{
			BarcodeStep = barParam.brDecodeStep[nBarcodeParamIdx].BarcodeStep;
			strValue = AOIDataDefine.GetAlgBarcodeDecodeStepText(BarcodeStep);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			BarcodeStep = AOIDataDefine.FindAlgBarcodeStepModeByText(strValue);
			barParam.brDecodeStep[nBarcodeParamIdx].BarcodeStep = BarcodeStep;
			bChanged = true;			
			
			if ( true == barParam.brDecodeStep[nBarcodeParamIdx].Decoded )
			{	strResult = _T("OK");	}
			else
			{	strResult = _T("NG");	}
			strStep = _T("Step");
			strStep = LoadMultiLanguageString(strStep, strStep);
			strName2.Format(_T("%s 2 [%s] [%s]"), strStep, strValue, strResult);				
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_STEP_2);
			if ( NULL != pProp2 )
			{	pProp2->SetName(strName2);	}

			StepParam.BarcodeStep = BarcodeStep;
			CAlgParam::DefaultAlgBarcodeStepParam(StepParam);
			strDisable = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(ALG_BARCODE_STEP_NONE, 0);

			strValue2.Format(_T("%.2f"), StepParam.Param1);
			strName2 = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(BarcodeStep, 0);
			if ( strName2 == strDisable ) { bShow = FALSE; }
			else { bShow = TRUE; }
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_2);
			if ( NULL != pProp2 )
			{	
				pProp2->Show(bShow);	
				pProp2->SetName(strName2);
				pProp2->SetValue(strValue2);
			}

			strValue2.Format(_T("%.2f"), StepParam.Param2);
			strName2 = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(BarcodeStep, 1);
			if ( strName2 == strDisable ) { bShow = FALSE; }
			else { bShow = TRUE; }
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_2);
			if ( NULL != pProp2 )
			{	
				pProp2->Show(bShow);	
				pProp2->SetName(strName2);
				pProp2->SetValue(strValue2);
			}
			barParam.brDecodeStep[nBarcodeParamIdx] = StepParam;
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_2:
		nBarcodeParamIdx = 1;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{			
			strValue.Format(_T("%.2f"),  barParam.brDecodeStep[nBarcodeParamIdx].Param1);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);
			barParam.brDecodeStep[nBarcodeParamIdx].Param1 = dValue;
			bChanged = true;
			
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_2:
		nBarcodeParamIdx = 1;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{			
			strValue.Format(_T("%.2f"),  barParam.brDecodeStep[nBarcodeParamIdx].Param2);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);
			barParam.brDecodeStep[nBarcodeParamIdx].Param2 = dValue;
			bChanged = true;			
		}
		break;	
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_2:
		nBarcodeParamIdx = 1;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{	
			bEnabled = barParam.brDecodeStep[nBarcodeParamIdx].Enabled;			
			pProp->SetCheckValue(bEnabled);
		}
		else
		{
			bEnabled = pProp->GetCheckValue();
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
			strValue = AOIDataDefine.GetEnableDisableText(bEnabled);			
			barParam.brDecodeStep[nBarcodeParamIdx].Enabled = bEnabled;			
			pProp->SetValue(strValue);
			bChanged = true;
		}		
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_2:
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_STEP_3:
		nBarcodeParamIdx = 2;		
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_3:
		nBarcodeParamIdx = 2;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{
			BarcodeStep = barParam.brDecodeStep[nBarcodeParamIdx].BarcodeStep;
			strValue = AOIDataDefine.GetAlgBarcodeDecodeStepText(BarcodeStep);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			BarcodeStep = AOIDataDefine.FindAlgBarcodeStepModeByText(strValue);
			barParam.brDecodeStep[nBarcodeParamIdx].BarcodeStep = BarcodeStep;
			bChanged = true;			
			
			if ( true == barParam.brDecodeStep[nBarcodeParamIdx].Decoded )
			{	strResult = _T("OK");	}
			else
			{	strResult = _T("NG");	}
			strStep = _T("Step");
			strStep = LoadMultiLanguageString(strStep, strStep);
			strName2.Format(_T("%s 3 [%s] [%s]"), strStep, strValue, strResult);
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_STEP_3);
			if ( NULL != pProp2 )
			{	pProp2->SetName(strName2);	}

			StepParam.BarcodeStep = BarcodeStep;
			CAlgParam::DefaultAlgBarcodeStepParam(StepParam);
			strDisable = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(ALG_BARCODE_STEP_NONE, 0);			

			strValue2.Format(_T("%.2f"), StepParam.Param1);
			strName2 = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(BarcodeStep, 0);
			if ( strName2 == strDisable ) { bShow = FALSE; }
			else { bShow = TRUE; }
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_3);
			if ( NULL != pProp2 )
			{					
				pProp2->Show(bShow);	
				pProp2->SetName(strName2);
				pProp2->SetValue(strValue2);
			}

			strValue2.Format(_T("%.2f"), StepParam.Param2);
			strName2 = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(BarcodeStep, 1);
			if ( strName2 == strDisable ) { bShow = FALSE; }
			else { bShow = TRUE; }
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_3);
			if ( NULL != pProp2 )
			{				
				pProp2->Show(bShow);	
				pProp2->SetName(strName2);
				pProp2->SetValue(strValue2);
			}
			barParam.brDecodeStep[nBarcodeParamIdx] = StepParam;
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_3:
		nBarcodeParamIdx = 2;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{			
			strValue.Format(_T("%.2f"),  barParam.brDecodeStep[nBarcodeParamIdx].Param1);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);
			barParam.brDecodeStep[nBarcodeParamIdx].Param1 = dValue;
			bChanged = true;
			
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_3:
		nBarcodeParamIdx = 2;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{			
			strValue.Format(_T("%.2f"),  barParam.brDecodeStep[nBarcodeParamIdx].Param2);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);
			barParam.brDecodeStep[nBarcodeParamIdx].Param2 = dValue;
			bChanged = true;
			
		}
		break;	
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_3:
		nBarcodeParamIdx = 2;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{	
			bEnabled = barParam.brDecodeStep[nBarcodeParamIdx].Enabled;			
			pProp->SetCheckValue(bEnabled);
		}
		else
		{
			bEnabled = pProp->GetCheckValue();
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
			strValue = AOIDataDefine.GetEnableDisableText(bEnabled);			
			barParam.brDecodeStep[nBarcodeParamIdx].Enabled = bEnabled;			
			pProp->SetValue(strValue);
			bChanged = true;
		}		
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_3:
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_STEP_4:
		nBarcodeParamIdx = 3;		
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_4:
		nBarcodeParamIdx = 3;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{
			BarcodeStep = barParam.brDecodeStep[nBarcodeParamIdx].BarcodeStep;
			strValue = AOIDataDefine.GetAlgBarcodeDecodeStepText(BarcodeStep);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			BarcodeStep = AOIDataDefine.FindAlgBarcodeStepModeByText(strValue);
			barParam.brDecodeStep[nBarcodeParamIdx].BarcodeStep = BarcodeStep;
			bChanged = true;			
			
			if ( true == barParam.brDecodeStep[nBarcodeParamIdx].Decoded )
			{	strResult = _T("OK");	}
			else
			{	strResult = _T("NG");	}
			strStep = _T("Step");
			strStep = LoadMultiLanguageString(strStep, strStep);
			strName2.Format(_T("%s 4 [%s] [%s]"), strStep, strValue, strResult);
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_STEP_4);
			if ( NULL != pProp2 )
			{	pProp2->SetName(strName2);	}

			StepParam.BarcodeStep = BarcodeStep;
			CAlgParam::DefaultAlgBarcodeStepParam(StepParam);
			strDisable = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(ALG_BARCODE_STEP_NONE, 0);			

			strValue2.Format(_T("%.2f"), StepParam.Param1);
			strName2 = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(BarcodeStep, 0);
			if ( strName2 == strDisable ) { bShow = FALSE; }
			else { bShow = TRUE; }
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_4);
			if ( NULL != pProp2 )
			{					
				pProp2->Show(bShow);	
				pProp2->SetName(strName2);
				pProp2->SetValue(strValue2);
			}

			strValue2.Format(_T("%.2f"), StepParam.Param2);
			strName2 = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(BarcodeStep, 1);
			if ( strName2 == strDisable ) { bShow = FALSE; }
			else { bShow = TRUE; }
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_4);
			if ( NULL != pProp2 )
			{				
				pProp2->Show(bShow);	
				pProp2->SetName(strName2);
				pProp2->SetValue(strValue2);
			}
			barParam.brDecodeStep[nBarcodeParamIdx] = StepParam;
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_4:
		nBarcodeParamIdx = 3;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{			
			strValue.Format(_T("%.2f"),  barParam.brDecodeStep[nBarcodeParamIdx].Param1);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);
			barParam.brDecodeStep[nBarcodeParamIdx].Param1 = dValue;
			bChanged = true;
			
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_4:
		nBarcodeParamIdx = 3;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{			
			strValue.Format(_T("%.2f"),  barParam.brDecodeStep[nBarcodeParamIdx].Param2);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);
			barParam.brDecodeStep[nBarcodeParamIdx].Param2 = dValue;
			bChanged = true;
			
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_4:
		nBarcodeParamIdx = 3;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{	
			bEnabled = barParam.brDecodeStep[nBarcodeParamIdx].Enabled;			
			pProp->SetCheckValue(bEnabled);
		}
		else
		{
			bEnabled = pProp->GetCheckValue();
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
			strValue = AOIDataDefine.GetEnableDisableText(bEnabled);			
			barParam.brDecodeStep[nBarcodeParamIdx].Enabled = bEnabled;			
			pProp->SetValue(strValue);
			bChanged = true;
		}		
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_4:
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_STEP_5:
		nBarcodeParamIdx = 4;
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_5:
		nBarcodeParamIdx = 4;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{
			BarcodeStep = barParam.brDecodeStep[nBarcodeParamIdx].BarcodeStep;
			strValue = AOIDataDefine.GetAlgBarcodeDecodeStepText(BarcodeStep);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			BarcodeStep = AOIDataDefine.FindAlgBarcodeStepModeByText(strValue);
			barParam.brDecodeStep[nBarcodeParamIdx].BarcodeStep = BarcodeStep;
			bChanged = true;			
			
			if ( true == barParam.brDecodeStep[nBarcodeParamIdx].Decoded )
			{	strResult = _T("OK");	}
			else
			{	strResult = _T("NG");	}
			strStep = _T("Step");
			strStep = LoadMultiLanguageString(strStep, strStep);
			strName2.Format(_T("%s 5 [%s] [%s]"), strStep, strValue, strResult);
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_STEP_5);
			if ( NULL != pProp2 )
			{	pProp2->SetName(strName2);	}

			StepParam.BarcodeStep = BarcodeStep;
			CAlgParam::DefaultAlgBarcodeStepParam(StepParam);
			strDisable = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(ALG_BARCODE_STEP_NONE, 0);			

			strValue2.Format(_T("%.2f"), StepParam.Param1);
			strName2 = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(BarcodeStep, 0);
			if ( strName2 == strDisable ) { bShow = FALSE; }
			else { bShow = TRUE; }
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_5);
			if ( NULL != pProp2 )
			{					
				pProp2->Show(bShow);	
				pProp2->SetName(strName2);
				pProp2->SetValue(strValue2);
			}

			strValue2.Format(_T("%.2f"), StepParam.Param2);
			strName2 = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(BarcodeStep, 1);
			if ( strName2 == strDisable ) { bShow = FALSE; }
			else { bShow = TRUE; }
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_5);
			if ( NULL != pProp2 )
			{				
				pProp2->Show(bShow);	
				pProp2->SetName(strName2);
				pProp2->SetValue(strValue2);
			}
			barParam.brDecodeStep[nBarcodeParamIdx] = StepParam;
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_5:
		nBarcodeParamIdx = 4;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{			
			strValue.Format(_T("%.2f"),  barParam.brDecodeStep[nBarcodeParamIdx].Param1);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);
			barParam.brDecodeStep[nBarcodeParamIdx].Param1 = dValue;
			bChanged = true;
			
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_5:
		nBarcodeParamIdx = 4;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{			
			strValue.Format(_T("%.2f"),  barParam.brDecodeStep[nBarcodeParamIdx].Param2);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);
			barParam.brDecodeStep[nBarcodeParamIdx].Param2 = dValue;
			bChanged = true;
			
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_5:
		nBarcodeParamIdx = 4;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{	
			bEnabled = barParam.brDecodeStep[nBarcodeParamIdx].Enabled;			
			pProp->SetCheckValue(bEnabled);
		}
		else
		{
			bEnabled = pProp->GetCheckValue();
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
			strValue = AOIDataDefine.GetEnableDisableText(bEnabled);			
			barParam.brDecodeStep[nBarcodeParamIdx].Enabled = bEnabled;			
			pProp->SetValue(strValue);
			bChanged = true;
		}		
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_5:
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_STEP_6:
		nBarcodeParamIdx = 5;		
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_6:
		nBarcodeParamIdx = 5;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{
			BarcodeStep = barParam.brDecodeStep[nBarcodeParamIdx].BarcodeStep;
			strValue = AOIDataDefine.GetAlgBarcodeDecodeStepText(BarcodeStep);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			BarcodeStep = AOIDataDefine.FindAlgBarcodeStepModeByText(strValue);
			barParam.brDecodeStep[nBarcodeParamIdx].BarcodeStep = BarcodeStep;
			bChanged = true;			
			
			if ( true == barParam.brDecodeStep[nBarcodeParamIdx].Decoded )
			{	strResult = _T("OK");	}
			else
			{	strResult = _T("NG");	}
			strStep = _T("Step");
			strStep = LoadMultiLanguageString(strStep, strStep);
			strName2.Format(_T("%s 6 [%s] [%s]"), strStep, strValue, strResult);
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_STEP_6);
			if ( NULL != pProp2 )
			{	pProp2->SetName(strName2);	}

			StepParam.BarcodeStep = BarcodeStep;
			CAlgParam::DefaultAlgBarcodeStepParam(StepParam);
			strDisable = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(ALG_BARCODE_STEP_NONE, 0);			

			strValue2.Format(_T("%.2f"), StepParam.Param1);
			strName2 = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(BarcodeStep, 0);
			if ( strName2 == strDisable ) { bShow = FALSE; }
			else { bShow = TRUE; }
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_6);
			if ( NULL != pProp2 )
			{					
				pProp2->Show(bShow);	
				pProp2->SetName(strName2);
				pProp2->SetValue(strValue2);
			}

			strValue2.Format(_T("%.2f"), StepParam.Param2);
			strName2 = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(BarcodeStep, 1);
			if ( strName2 == strDisable ) { bShow = FALSE; }
			else { bShow = TRUE; }
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_6);
			if ( NULL != pProp2 )
			{				
				pProp2->Show(bShow);	
				pProp2->SetName(strName2);
				pProp2->SetValue(strValue2);
			}
			barParam.brDecodeStep[nBarcodeParamIdx] = StepParam;
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_6:
		nBarcodeParamIdx = 5;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{			
			strValue.Format(_T("%.2f"),  barParam.brDecodeStep[nBarcodeParamIdx].Param1);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);
			barParam.brDecodeStep[nBarcodeParamIdx].Param1 = dValue;
			bChanged = true;
			
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_6:
		nBarcodeParamIdx = 5;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{			
			strValue.Format(_T("%.2f"),  barParam.brDecodeStep[nBarcodeParamIdx].Param2);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);
			barParam.brDecodeStep[nBarcodeParamIdx].Param2 = dValue;
			bChanged = true;
			
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_6:
		nBarcodeParamIdx = 5;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{	
			bEnabled = barParam.brDecodeStep[nBarcodeParamIdx].Enabled;			
			pProp->SetCheckValue(bEnabled);
		}
		else
		{
			bEnabled = pProp->GetCheckValue();
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
			strValue = AOIDataDefine.GetEnableDisableText(bEnabled);			
			barParam.brDecodeStep[nBarcodeParamIdx].Enabled = bEnabled;			
			pProp->SetValue(strValue);
			bChanged = true;
		}		
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_6:
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_STEP_7:
		nBarcodeParamIdx = 6;		
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_7:
		nBarcodeParamIdx = 6;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{
			BarcodeStep = barParam.brDecodeStep[nBarcodeParamIdx].BarcodeStep;
			strValue = AOIDataDefine.GetAlgBarcodeDecodeStepText(BarcodeStep);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			BarcodeStep = AOIDataDefine.FindAlgBarcodeStepModeByText(strValue);
			barParam.brDecodeStep[nBarcodeParamIdx].BarcodeStep = BarcodeStep;
			bChanged = true;			
			
			if ( true == barParam.brDecodeStep[nBarcodeParamIdx].Decoded )
			{	strResult = _T("OK");	}
			else
			{	strResult = _T("NG");	}
			strStep = _T("Step");
			strStep = LoadMultiLanguageString(strStep, strStep);
			strName2.Format(_T("%s 7 [%s] [%s]"), strStep, strValue, strResult);
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_STEP_7);
			if ( NULL != pProp2 )
			{	pProp2->SetName(strName2);	}

			StepParam.BarcodeStep = BarcodeStep;
			CAlgParam::DefaultAlgBarcodeStepParam(StepParam);
			strDisable = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(ALG_BARCODE_STEP_NONE, 0);			

			strValue2.Format(_T("%.2f"), StepParam.Param1);
			strName2 = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(BarcodeStep, 0);
			if ( strName2 == strDisable ) { bShow = FALSE; }
			else { bShow = TRUE; }
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_7);
			if ( NULL != pProp2 )
			{					
				pProp2->Show(bShow);	
				pProp2->SetName(strName2);
				pProp2->SetValue(strValue2);
			}

			strValue2.Format(_T("%.2f"), StepParam.Param2);
			strName2 = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(BarcodeStep, 1);
			if ( strName2 == strDisable ) { bShow = FALSE; }
			else { bShow = TRUE; }
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_7);
			if ( NULL != pProp2 )
			{				
				pProp2->Show(bShow);	
				pProp2->SetName(strName2);
				pProp2->SetValue(strValue2);
			}
			barParam.brDecodeStep[nBarcodeParamIdx] = StepParam;
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_7:
		nBarcodeParamIdx = 6;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{			
			strValue.Format(_T("%.2f"),  barParam.brDecodeStep[nBarcodeParamIdx].Param1);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);
			barParam.brDecodeStep[nBarcodeParamIdx].Param1 = dValue;
			bChanged = true;
			
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_7:
		nBarcodeParamIdx = 6;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{			
			strValue.Format(_T("%.2f"),  barParam.brDecodeStep[nBarcodeParamIdx].Param2);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);
			barParam.brDecodeStep[nBarcodeParamIdx].Param2 = dValue;
			bChanged = true;
			
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_7:
		nBarcodeParamIdx = 6;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{	
			bEnabled = barParam.brDecodeStep[nBarcodeParamIdx].Enabled;			
			pProp->SetCheckValue(bEnabled);
		}
		else
		{
			bEnabled = pProp->GetCheckValue();
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
			strValue = AOIDataDefine.GetEnableDisableText(bEnabled);			
			barParam.brDecodeStep[nBarcodeParamIdx].Enabled = bEnabled;			
			pProp->SetValue(strValue);
			bChanged = true;
		}		
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_7:
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_STEP_8:
		nBarcodeParamIdx = 7;		
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_NAME_8:
		nBarcodeParamIdx = 7;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{
			BarcodeStep = barParam.brDecodeStep[nBarcodeParamIdx].BarcodeStep;
			strValue = AOIDataDefine.GetAlgBarcodeDecodeStepText(BarcodeStep);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			BarcodeStep = AOIDataDefine.FindAlgBarcodeStepModeByText(strValue);
			barParam.brDecodeStep[nBarcodeParamIdx].BarcodeStep = BarcodeStep;
			bChanged = true;			
			
			if ( true == barParam.brDecodeStep[nBarcodeParamIdx].Decoded )
			{	strResult = _T("OK");	}
			else
			{	strResult = _T("NG");	}
			strStep = _T("Step");
			strStep = LoadMultiLanguageString(strStep, strStep);
			strName2.Format(_T("%s 8 [%s] [%s]"), strStep, strValue, strResult);
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_STEP_8);
			if ( NULL != pProp2 )
			{	pProp2->SetName(strName2);	}

			StepParam.BarcodeStep = BarcodeStep;
			CAlgParam::DefaultAlgBarcodeStepParam(StepParam);
			strDisable = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(ALG_BARCODE_STEP_NONE, 0);			

			strValue2.Format(_T("%.2f"), StepParam.Param1);
			strName2 = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(BarcodeStep, 0);
			if ( strName2 == strDisable ) { bShow = FALSE; }
			else { bShow = TRUE; }
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_8);
			if ( NULL != pProp2 )
			{					
				pProp2->Show(bShow);	
				pProp2->SetName(strName2);
				pProp2->SetValue(strValue2);
			}

			strValue2.Format(_T("%.2f"), StepParam.Param2);
			strName2 = AOIDataDefine.GetAlgBarcodeDecodeStepParamText(BarcodeStep, 1);
			if ( strName2 == strDisable ) { bShow = FALSE; }
			else { bShow = TRUE; }
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_8);
			if ( NULL != pProp2 )
			{				
				pProp2->Show(bShow);	
				pProp2->SetName(strName2);
				pProp2->SetValue(strValue2);
			}
			barParam.brDecodeStep[nBarcodeParamIdx] = StepParam;
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM1_8:
		nBarcodeParamIdx = 7;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{			
			strValue.Format(_T("%.2f"),  barParam.brDecodeStep[nBarcodeParamIdx].Param1);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);
			barParam.brDecodeStep[nBarcodeParamIdx].Param1 = dValue;
			bChanged = true;
			
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_PARAM2_8:
		nBarcodeParamIdx = 7;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MAX_BARCODE_DECODE_COUNT )
		{			
			strValue.Format(_T("%.2f"),  barParam.brDecodeStep[nBarcodeParamIdx].Param2);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);
			barParam.brDecodeStep[nBarcodeParamIdx].Param2 = dValue;
			bChanged = true;
			
		}
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_ENABLE_8:		
		nBarcodeParamIdx = 7;
		if ( nBarcodeParamIdx<0 || nBarcodeParamIdx>=MaxBarcodeDecodeCount )
		{	
			bEnabled = barParam.brDecodeStep[nBarcodeParamIdx].Enabled;			
			pProp->SetCheckValue(bEnabled);
		}
		else
		{
			bEnabled = pProp->GetCheckValue();
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
			strValue = AOIDataDefine.GetEnableDisableText(bEnabled);			
			barParam.brDecodeStep[nBarcodeParamIdx].Enabled = bEnabled;			
			pProp->SetValue(strValue);
			bChanged = true;
		}		
		break;
	case WND_ALG_PROPERTY_BARCODE_DECODE_DECODED_8:
		break;
	}

	if ( false == bBoolParam )
	{	strNewValue = pProp->GetValue();	}
	Changed.sValueName = strName;
	Changed.sValueOld = strOldValue;
	Changed.sValueNew = strNewValue;	

	Changed.bParamChanged = bChanged;	
	Changed.bReBuildWndUI = bReBuildWndUI;
	return true;
}
//-------------------------------------------------------------------------------------//