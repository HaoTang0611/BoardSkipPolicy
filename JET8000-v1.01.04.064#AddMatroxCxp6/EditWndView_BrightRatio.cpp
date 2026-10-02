// EditWndView_BrightRatio.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditWndView.h"
//-------------------------------------------------------------------------------------//
#include "InputListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_BrightRatio(CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }	
	
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const WND_LOGIC_TYPE    WndLogicType = WndPtr->GetWndLogicType();
	const TALG_PARAM_BRIGHT_RATIO &brParam = AlgParam.GetAlgParamBrightRatio();
	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;	
	const bool bShowScaleParam=FN_DISABLE==AOIDataCollect.GetSystemParameter().m_ShowAlgBrightRatioScaleParam ? false:true;

	CString str;
	bool    bIsPass = true;
	BOOL    bEnabled=TRUE;	
	CString strCaption, strValue, strReading, strDescr, strOption;
	CJETPropertyGridProperty* pParamItem = NULL;
	CJETPropertyGridProperty* pGroupBasic = NULL;
	CJETPropertyGridProperty* pGroupAdvanced = NULL;
	CJETPropertyGridProperty* pGroupResult = NULL;

	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;	
	wndPropList.SetRedraw(FALSE);

	strCaption = FormWndParamListCategoryName(WndPtr);	
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_BASIC_BEGIN);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);	
	if ( BuildWndParamList_WndDefectID(pGroupBasic, Project, WndPtr) == false )
	{	return false;	}	
	if ( BuildWndParamList_AlgFrameIndex(pGroupBasic, Project, WndPtr) == false )
	{	return false;	}
	
	//增加Birght Ratio 參數
	double dUSL = 0;
	double dLSL = 0;
	double dReading = 0;
	double dRatio = 0.0;

	dReading = brParam.brTargetValue;		
	strCaption = _T("Target");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dReading);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_TARGET_VALUE);	
	strReading.Format(_T("%.0f"), brParam.brAverageReading);
	//pParamItem->SetReading(strReading);
	pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);

	//啟用子框		
	strValue = _T("");
	strCaption = _T("ROI Box");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetEnableDisableText(brParam.brRoiBoxEnabled);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_ROI_BOX_ENABLED);			
	pParamItem->AddOption(AOIDataDefine.GetEnableDisableText(FN_ENABLE));
	pParamItem->AddOption(AOIDataDefine.GetEnableDisableText(FN_DISABLE));
	pParamItem->AllowEdit(FALSE);
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Average Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgBrightAverageModeText(brParam.brAverageMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_AVERAGE_MODE);	
	strOption = AOIDataDefine.GetAlgBrightAverageModeText(ALG_BRIGHT_AVERAGE_FULL);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgBrightAverageModeText(ALG_BRIGHT_AVERAGE_PARTIAL);	pParamItem->AddOption(strOption);		
	strReading.Format(_T("%.0f"), brParam.brAverageReading);
	pParamItem->SetReading(strReading);
	pGroupBasic->AddSubItem(pParamItem);
	
	dUSL = brParam.brAveragePartialH;
	dLSL = brParam.brAveragePartialL;
	strReading = _T("%");
	strCaption = _T("Parital USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_AVERAGE_PARTIAL_H);	
	pParamItem->SetReading(strReading);
	if ( ALG_BRIGHT_AVERAGE_FULL == brParam.brAverageMode ) 
	{	pParamItem->Show(FALSE); }
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Parital LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_AVERAGE_PARTIAL_L);	
	pParamItem->SetReading(strReading);
	if ( ALG_BRIGHT_AVERAGE_PARTIAL != brParam.brAverageMode ) 
	{	pParamItem->Show(FALSE); }
	pGroupBasic->AddSubItem(pParamItem);
	
	if ( true == bShowScaleParam )
	{
		strCaption = _T("Ave-Scale");	
		strCaption = LoadMultiLanguageString(strCaption, strCaption);	
		bEnabled = brParam.brAverageScaleEnabled;	
		strValue.Format(_T("%.2f"), brParam.brAverageScale);		
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_AVERAGE_SCALE_ENB);
		pParamItem->SetReading(_T("%"));
		pParamItem->SetCheckValue(bEnabled);
		pGroupBasic->AddSubItem(pParamItem);
	}

	//結果文字顯示
	if ( BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false )
	{	return false; }

	//Tolerance
	str = _T("Tolerance");
	str = LoadMultiLanguageString(str, str);	
	if ( false == brParam.brToleranceEnabled )
	{	strCaption = str;  }
	else
	{	strCaption.Format(_T("%s  [%.0f]"), str, brParam.brToleranceReading); }
	bEnabled = brParam.brToleranceEnabled;	
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_TOLERANCE_ENABLED);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);		
	pGroupBasic->SetCheckValue(bEnabled);		
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);	

	dUSL = brParam.brToleranceUSL;
	dLSL = brParam.brToleranceLSL;	
	dReading = brParam.brToleranceReading;	
	strReading.Format(_T("%.0f"), dReading);
	strCaption = _T("Tol. USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_TOLERANCE_USL);
	strReading.Format(_T("%.0f"), dUSL+brParam.brTargetValue);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_BrightRatioToleranceUSL(brParam);		
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	//m_clrUnTest = 0x808080;
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("Tol. LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_TOLERANCE_LSL);
	strReading.Format(_T("%.0f"), dLSL+brParam.brTargetValue);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_BrightRatioToleranceLSL(brParam);		
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	pGroupBasic->AddSubItem(pParamItem);
	//m_clrUnTest = 0x808080;
	pGroupBasic->Expand(bEnabled);	

	//Limit	
	str = _T("Limit");
	strCaption = LoadMultiLanguageString(str, str);	
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_LIMIT_GROUP);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);	

	dUSL = brParam.brLimitTolUSL;
	dLSL = brParam.brLimitTolLSL;
	strCaption = _T("Tol. USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_LIMIT_TOL_USL);
	strReading.Format(_T("%.0f"), dUSL+brParam.brTargetValue);
	pParamItem->SetReading(strReading);		
	pGroupBasic->AddSubItem(pParamItem);	
	
	strCaption = _T("Tol. LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_LIMIT_TOL_LSL);
	strReading.Format(_T("%.0f"), dLSL+brParam.brTargetValue);
	pParamItem->SetReading(strReading);		
	pGroupBasic->AddSubItem(pParamItem);

	//Limit Max	
	strCaption = _T("Max");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	bEnabled = brParam.brLimitMaxEnabled;	
	strValue.Format(_T("%.0f"), brParam.brLimitReadingMax);	
	strReading.Format(_T("%.0f"), brParam.brAverageReadingMax);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_LIMIT_MAX_ENABLED);
	pParamItem->AllowEdit(FALSE);
	pParamItem->SetReading(strReading);
	pParamItem->SetCheckValue(bEnabled);	
	bIsPass = CAlgParam::CheckOK_BrightRatioLimitMax(brParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);

	if ( true == bShowScaleParam )
	{
		strCaption = _T("Max-Scale");	
		strCaption = LoadMultiLanguageString(strCaption, strCaption);	
		bEnabled = brParam.brLimitMaxScaleEnabled;	
		strValue.Format(_T("%.2f"), brParam.brLimitMaxScale);		
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_LIMIT_MAX_SCALE_ENB);
		pParamItem->SetReading(_T("%"));
		pParamItem->SetCheckValue(bEnabled);
		pGroupBasic->AddSubItem(pParamItem);	
	}

	//Limit Min	
	strCaption = _T("Min");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	bEnabled = brParam.brLimitMinEnabled;	
	strValue.Format(_T("%.0f"), brParam.brLimitReadingMin);	
	strReading.Format(_T("%.0f"), brParam.brAverageReadingMin);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_LIMIT_MIN_ENABLED);
	pParamItem->AllowEdit(FALSE);
	pParamItem->SetReading(strReading);
	pParamItem->SetCheckValue(bEnabled);	
	bIsPass = CAlgParam::CheckOK_BrightRatioLimitMin(brParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);	

	if ( true == bShowScaleParam )
	{
		strCaption = _T("Min-Scale");	
		strCaption = LoadMultiLanguageString(strCaption, strCaption);	
		bEnabled = brParam.brLimitMinScaleEnabled;	
		strValue.Format(_T("%.2f"), brParam.brLimitMinScale);		
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_LIMIT_MIN_SCALE_ENB);
		pParamItem->SetReading(_T("%"));
		pParamItem->SetCheckValue(bEnabled);
		pGroupBasic->AddSubItem(pParamItem);	
	}

	//m_clrUnTest = 0x808080;	
	if ( true==brParam.brLimitMinEnabled || true==brParam.brLimitMaxEnabled )
	{	pGroupBasic->Expand(TRUE);	}	
	else
	{	pGroupBasic->Expand(FALSE);	}

	//Ratio
	strCaption = _T("Ratio");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	bEnabled = brParam.brRatioEnabled;	
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_RATIO_ENABLED);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);		
	pGroupBasic->SetCheckValue(bEnabled);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);	

	dUSL = brParam.brRatioUSL;
	dLSL = brParam.brRatioLSL;	
	dReading = brParam.brRatioReading;	
	strReading.Format(_T("%.2f"), dReading);

	strCaption = _T("Area");
	//strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strCaption = AOIDataDefine.GetAreaText();
	strValue.Format(_T("%.0f"), brParam.brRatioArea);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_RATIO_AREA);
	pParamItem->SetReading(_T("um^2"));		
	pParamItem->AllowEdit(FALSE);
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Ratio USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_RATIO_USL);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_BrightRatioRatioUSL(brParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	//m_clrUnTest = 0x808080;
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("Ratio LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_RATIO_LSL);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_BrightRatioRatioLSL(brParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	pGroupBasic->AddSubItem(pParamItem);
	//m_clrUnTest = 0x808080;
	pGroupBasic->Expand(bEnabled);		
	
	//Range
	strCaption = _T("Range");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	bEnabled = brParam.brRangeEnabled;	
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_RANGE_ENABLED);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);		
	pGroupBasic->SetCheckValue(bEnabled);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);	

	dUSL = brParam.brRangeUSL;
	dLSL = brParam.brRangeLSL;
	dReading = brParam.brRangeReading;	
	strReading.Format(_T("%.2f"), dReading);	
	strCaption = _T("USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_RANGE_USL);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_BrightRatioRangeUSL(brParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_RANGE_LSL);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_BrightRatioRangeLSL(brParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	pGroupBasic->AddSubItem(pParamItem);
	pGroupBasic->Expand(bEnabled);	
	
	//Contrast
	strCaption = _T("Contrast");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	bEnabled = brParam.brContrastEnabled;	
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_CONTRAST_ENABLED);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);		
	pGroupBasic->SetCheckValue(bEnabled);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);	

	dUSL = brParam.brContrastUSL;
	dLSL = brParam.brContrastLSL;
	dReading = brParam.brContrastReading;	
	strReading.Format(_T("%.2f"), dReading);	
	strCaption = _T("USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_CONTRAST_USL);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_BrightRatioContrastUSL(brParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_CONTRAST_LSL);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_BrightRatioContrastLSL(brParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	pGroupBasic->AddSubItem(pParamItem);			
	pGroupBasic->Expand(bEnabled);
	
	//X Through
	strCaption = _T("X Through");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	bEnabled = brParam.brXLineEnabled;	
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_XLINE_ENABLED);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);		
	pGroupBasic->SetCheckValue(bEnabled);		
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);

	strCaption = _T("Continue Range");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), brParam.brXLineRange);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_XLINE_RANGE);
	pParamItem->SetReading(_T("um"));		
	pGroupBasic->AddSubItem(pParamItem);	
	
	strCaption = _T("Line Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgBrightLineModeText(brParam.brXLineMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_XLINE_MODE);	
	strOption = AOIDataDefine.GetAlgBrightLineModeText(LINE_MODE_BRIGHT);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgBrightLineModeText(LINE_MODE_DARK);	pParamItem->AddOption(strOption);	
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("Unit Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgCalcUnitModeText(brParam.brXLineUnitMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_XLINE_UNIT_MODE);	
	strOption = AOIDataDefine.GetAlgCalcUnitModeText(ALG_CALC_UNIT_ABS);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgCalcUnitModeText(ALG_CALC_UNIT_RATIO);	pParamItem->AddOption(strOption);	
	pGroupBasic->AddSubItem(pParamItem);	

	dUSL = brParam.brXLineUSL;
	dLSL = brParam.brXLineLSL;
	dReading = brParam.brXLineReading;	
	strReading.Format(_T("%.2f"), dReading);	
	strCaption = _T("Ratio USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_XLINE_USL);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_BrightRatioLineXUSL(brParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);		

	strCaption = _T("Ratio LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_XLINE_LSL);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_BrightRatioLineXLSL(brParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	pGroupBasic->AddSubItem(pParamItem);	
	pGroupBasic->Expand(bEnabled);	
	
	//Y Through
	strCaption = _T("Y Through");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	bEnabled = brParam.brYLineEnabled;	
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }
	pGroupBasic->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_YLINE_ENABLED);	
	pGroupBasic->SetData((DWORD_PTR)WndPtr);
	pGroupBasic->SetCheckValue(bEnabled);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);

	strCaption = _T("Continue Range");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), brParam.brYLineRange);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_YLINE_RANGE);	
	pParamItem->SetReading(_T("um"));		
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("Line Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgBrightLineModeText(brParam.brYLineMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_YLINE_MODE);	
	strOption = AOIDataDefine.GetAlgBrightLineModeText(LINE_MODE_BRIGHT);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgBrightLineModeText(LINE_MODE_DARK);	pParamItem->AddOption(strOption);	
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Unit Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgCalcUnitModeText(brParam.brYLineUnitMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_YLINE_UNIT_MODE);	
	strOption = AOIDataDefine.GetAlgCalcUnitModeText(ALG_CALC_UNIT_ABS);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgCalcUnitModeText(ALG_CALC_UNIT_RATIO);	pParamItem->AddOption(strOption);	
	pGroupBasic->AddSubItem(pParamItem);	

	dUSL = brParam.brYLineUSL;
	dLSL = brParam.brYLineLSL;
	dReading = brParam.brYLineReading;	
	strReading.Format(_T("%.2f"), dReading);	
	strCaption = _T("Ratio USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_YLINE_USL);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_BrightRatioLineYUSL(brParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("Ratio LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BRIGHT_RATIO_YLINE_LSL);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_BrightRatioLineYLSL(brParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	pGroupBasic->AddSubItem(pParamItem);
	pGroupBasic->Expand(bEnabled);	

	if ( BuildWndParamList_ModelMaskFlag(wndPropList, Project, WndPtr) == false )
	{	return false;	}	

	if ( BuildWndParamList_GroupCompare(wndPropList, Project, WndPtr) == false )
	{	return false; }

	//基準數值
	if ( BuildWndParamList_BaseValue(wndPropList, Project, WndPtr) == false )
	{	return false; }

	//邏輯設定
	if ( BuildWndParamList_LogicParam(wndPropList, Project, WndPtr) == false )
	{	return false; }

	//遮罩框
	if ( BuildWndParamList_MaskBox(wndPropList, Project, WndPtr) == false )
	{	return false;	}

	//進階設定
	strCaption = _T("Advance");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pGroupAdvanced = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupAdvanced ) { return false; }	
	pGroupAdvanced->SetID(WND_ALG_PROPERTY_ADVANCE_BEGIN);
	pGroupAdvanced->SetData((DWORD_PTR)WndPtr);
	wndPropList.AddProperty(pGroupAdvanced, bRedraw, bAdjustLayou);
	
	if ( BuildWndParamList_AdvanceGeneral(pGroupAdvanced, Project, WndPtr) == false )	
	{	return false;	}		
	pGroupAdvanced->Expand(FALSE);

	if ( FALSE == bAdjustLayou )
	{	wndPropList.AdjustLayout(); }
	wndPropList.SetRedraw(TRUE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_BrightRatio(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	
	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	CAOILand        *LandPtr = WndPtr->GetWndLandPtr();
	CAlgParam       &AlgParam = WndPtr->GetWndAlgParam();	
	CAlgBinaryParam *AlgBinParamPtr = AlgParam.GetAlgImageBinParamPtr();
	TALG_PARAM_BRIGHT_RATIO  &brParam = AlgParam.GetAlgParamBrightRatio();
	
	bool          bIsPass = true;
	bool          bChanged = false;	
	bool          bBoolParam = false;
	bool          bWndRgnChanged = false;
	int           nValue=0, nReading=0;
	bool          bValue=false, bReading=false, bEnabled=true;
	BOOL          BValue=FALSE, BReading=FALSE, bClickBtn=FALSE, BShow=TRUE;
	double        dValue=0.0, dValue2=0.0, dReading=0.0;	
	CString       strValue, strValue2;
	CString       strName = pProp->GetName();
	CString       strDescr = pProp->GetDescription();
	COleVariant   vtValue = pProp->GetValue();	
	CString       strNewValue(pProp->GetValue());
	CString       strOldValue(pProp->GetOriginalValue());
	DWORD_PTR     dwData = pProp->GetData();	
	CJETPropertyGridProperty *pProp2 = NULL;	
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());	
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;
	const unsigned int FrameUniqueID = AlgBinParamPtr->GetBinaryFrameUniqueID();
	const int ShowModelPropertyParam = AOIDataCollect.GetSystemParameter().m_ShowModelPropertyParam;
	bClickBtn = pProp->GetClickUserBtn();

	switch ( ParamID )
	{	
	case WND_ALG_PROPERTY_BRIGHT_RATIO_TARGET_VALUE:
		if ( TRUE == bClickBtn )
		{			
			if (FN_ENABLE==ShowModelPropertyParam && FRAME_UNIQUE_ID_DLP==FrameUniqueID )
			{	
				TListNode     Node;				
				CInputListWnd EnumWnd;				
				std::vector<TListNode> NodelList;
				const DWORD_PTR SelMode_Test=0;
				const DWORD_PTR SelMode_Body=1;
				const DWORD_PTR SelMode_Lead=2;
				const DWORD_PTR SelMode_LeadTip=3;
				const DWORD_PTR SelMode_LeadShoulder=4;
	
				CString strCaption = _T("Set Height Value Wnd");	
				CString strLabel = _T("Select Height Value");	
				CString strAve = AOIDataDefine.GetAveText();				
				CString strBody = AOIDataDefine.GetModelBodyText();
				CString strLead = AOIDataDefine.GetModelLeadText();
				CString strHeight = AOIDataDefine.GetThicknessText();
				CString strLeadTip = AOIDataDefine.GetModelLeadTipText();
				CString strLeadShoulder = AOIDataDefine.GetModelLeadShoulderText();

				Node.Data = SelMode_Test;	Node.Text.Format(_T("%s %s[%.0f um]"), strAve, strHeight, brParam.brAverageReading);	NodelList.push_back(Node);	
				if ( NULL == LandPtr ) 
				{	Node.Data = SelMode_Body;	Node.Text.Format(_T("%s %s[%.0f um]"), strBody, strHeight, ModelPtr->GetModelBodyHeight());	NodelList.push_back(Node);	}
				else
				{
					LAND_TYPE LandType=LandPtr->GetLandType();
					if ( LAND_TYPE_ELECTRODE == LandType )
					{	Node.Data = SelMode_Lead;	Node.Text.Format(_T("%s %s[%.0f um]"), strLead, strHeight, LandPtr->GetLandLeadHeight());	NodelList.push_back(Node);	}
					if ( LAND_TYPE_IC_LEAD==LandType ||LAND_TYPE_CON_LEAD==LandType)
					{
						Node.Data = SelMode_LeadTip;	Node.Text.Format(_T("%s %s[%.0f um]"), strLeadTip, strHeight, LandPtr->GetLandLeadTipHeight());	NodelList.push_back(Node);						
						Node.Data = SelMode_LeadShoulder;	Node.Text.Format(_T("%s %s[%.0f um]"), strLeadShoulder, strHeight, LandPtr->GetLandLeadShoulderHeight());	NodelList.push_back(Node);
					}
					if ( LAND_TYPE_DIP_LEAD == LandType )
					{	Node.Data = SelMode_Lead;	Node.Text.Format(_T("%s %s[%.0f um]"), strLead, strHeight, LandPtr->GetLandLeadHeight());	NodelList.push_back(Node);	}
				}
				EnumWnd.SetParam1(strCaption, strLabel, SelMode_Test, NodelList);
				if ( EnumWnd.GetSelIndex1() < 0 ) 
				{	EnumWnd.SetSelIndex1(0); }	
				if ( EnumWnd.DoModal() == IDOK )
				{
					dValue = brParam.brAverageReading;
					const DWORD_PTR SelMode = (int)(EnumWnd.GetSelData());						
					if ( NULL == LandPtr ) 
					{
						if ( SelMode_Body != SelMode )
						{	dValue = brParam.brAverageReading;	}
						else
						{	dValue = ModelPtr->GetModelBodyHeight();	}
					}
					else
					{
						switch ( SelMode ) 
						{
						case SelMode_Body:	dValue = ModelPtr->GetModelBodyHeight(); break;
						case SelMode_Lead:	dValue = LandPtr->GetLandLeadHeight(); break;
						case SelMode_LeadTip: dValue = LandPtr->GetLandLeadTipHeight(); break;
						case SelMode_LeadShoulder: dValue = LandPtr->GetLandLeadShoulderHeight(); break;							
						case SelMode_Test:
						default:
							dValue = brParam.brAverageReading;
							break;
						}
					}
					strValue.Format(_T("%.0f"), dValue);
				}
			}
			else
			{
				dValue = brParam.brAverageReading;
				strValue.Format(_T("%.0f"), dValue);
			}			
		}
		else
		{
			strValue = pProp->GetValue();
			dValue = ::_tcstod(strValue, NULL);		
			//if ( dValue < 1 ) {	dValue = 1;	}
			strValue.Format(_T("%.0f"), dValue);
		}
		brParam.brTargetValue = dValue;
		pProp->SetValue(strValue);			
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_ROI_BOX_ENABLED:			
		bChanged = true;
		bBoolParam = true;		
		strValue = pProp->GetValue();
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		bEnabled = (bool)(AOIDataDefine.FindEnableDisableIDByText(strValue));
		if ( true == bEnabled )
		{	
			if ( 0 != WndPtr->GetWndRoiWndCount() )
			{	brParam.brRoiBoxEnabled = true;  }
			else
			{				
				CAOIWndRoi *WndRoiPtr = AOIObjManager.CreateWndRoiObj();				;
				if ( NULL == WndRoiPtr )
				{	brParam.brRoiBoxEnabled = false; }
				else
				{
					TREGION4D     WndRgn;
					TREGION4D     WndRoiRgn;
					bWndRgnChanged = true;
					brParam.brRoiBoxEnabled = true; 					
					WndPtr->GetWndRegion(WndRgn);
					const double WndCpX = WndRgn.GetCpX();
					const double WndCpY = WndRgn.GetCpY();
					const double WndSizeW = WndRgn.GetWidth();
					const double WndSizeH = WndRgn.GetHeight();
					const double WndRoiSizeW = WndSizeW/2;
					const double WndRoiSizeH = WndSizeH/2;
					WndRoiRgn.minX = WndCpX-(WndRoiSizeW/2);
					WndRoiRgn.minY = WndCpY-(WndRoiSizeH/2);
					WndRoiRgn.maxX = WndCpX+(WndRoiSizeW/2);
					WndRoiRgn.maxY = WndCpY+(WndRoiSizeH/2);
					WndRoiPtr->SetWndRoiRegion(WndRoiRgn);
					WndRoiPtr->SetWndRoiToward(WndPtr->GetWndToward());
					WndPtr->AddWndRoiWndPtr(WndRoiPtr, false);					
					WndPtr->SetWndModified(true);						
				}
			}
		}
		else
		{	
			bWndRgnChanged = true;
			brParam.brRoiBoxEnabled = false; 
			WndPtr->ClearWndRoiWndList();
			WndPtr->SetWndModified(true);
		}
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_AVERAGE_MODE:
		BShow = TRUE;
		strValue = pProp->GetValue();
		brParam.brAverageMode = AOIDataDefine.FindAlgBrightAverageModeByText(strValue);
		if ( ALG_BRIGHT_AVERAGE_PARTIAL == brParam.brAverageMode )
		{	BShow = TRUE; }
		else
		{	BShow = FALSE; }
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BRIGHT_RATIO_AVERAGE_PARTIAL_H);
		if ( NULL != pProp2 )
		{	pProp2->Show(BShow); }
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_BRIGHT_RATIO_AVERAGE_PARTIAL_L);
		if ( NULL != pProp2 )
		{	pProp2->Show(BShow); }
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_AVERAGE_PARTIAL_H:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0.0 ) 
		{	dValue = 0.0;	}		
		else if ( dValue > 100.0 )
		{	dValue = 100.0; }
		if ( dValue < brParam.brAveragePartialL ) 
		{	dValue = brParam.brAveragePartialL;	}		
		brParam.brAveragePartialH = dValue;
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_AVERAGE_PARTIAL_L:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0.0 ) 
		{	dValue = 0.0;	}		
		else if ( dValue > 100.0 )
		{	dValue = 100.0; }
		if ( dValue > brParam.brAveragePartialH ) 
		{	dValue = brParam.brAveragePartialH;	}		
		brParam.brAveragePartialL = dValue;
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_AVERAGE_SCALE_ENB:
		bEnabled = pProp->GetCheckValue();	
		if ( brParam.brAverageScaleEnabled != bEnabled )
		{
			bBoolParam = true;
			brParam.brAverageScaleEnabled = bEnabled;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		else
		{
			strValue = pProp->GetValue();
			brParam.brAverageScale = ::_tcstod(strValue, NULL);
		}
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_TOLERANCE_ENABLED:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		brParam.brToleranceEnabled = bEnabled;
		pProp->Expand(bEnabled);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_TOLERANCE_USL:		
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < brParam.brToleranceLSL ) 
		{	dValue = brParam.brToleranceLSL;	}		
		brParam.brToleranceUSL = dValue;
		bIsPass = CAlgParam::CheckOK_BrightRatioToleranceUSL(brParam);		
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;			
	case WND_ALG_PROPERTY_BRIGHT_RATIO_TOLERANCE_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > brParam.brToleranceUSL ) 
		{	dValue = brParam.brToleranceUSL;	}
		brParam.brToleranceLSL = dValue;				
		bIsPass = CAlgParam::CheckOK_BrightRatioToleranceLSL(brParam);		
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);		
		bChanged = true;
		break;

	case WND_ALG_PROPERTY_BRIGHT_RATIO_LIMIT_GROUP:
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_LIMIT_MAX_ENABLED:
		bEnabled = pProp->GetCheckValue();		
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		brParam.brLimitMaxEnabled = bEnabled;
		pProp->Expand(bEnabled);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_LIMIT_MIN_ENABLED:
		bEnabled = pProp->GetCheckValue();		
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		brParam.brLimitMinEnabled = bEnabled;
		pProp->Expand(bEnabled);
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_BRIGHT_RATIO_LIMIT_MAX_SCALE_ENB:
		bEnabled = pProp->GetCheckValue();	
		if ( brParam.brLimitMaxScaleEnabled != bEnabled )
		{
			bBoolParam = true;
			brParam.brLimitMaxScaleEnabled = bEnabled;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		else
		{
			strValue = pProp->GetValue();
			brParam.brLimitMaxScale = ::_tcstod(strValue, NULL);
		}
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_LIMIT_MIN_SCALE_ENB:		
		bEnabled = pProp->GetCheckValue();	
		if ( brParam.brLimitMinScaleEnabled != bEnabled )
		{
			bBoolParam = true;
			brParam.brLimitMinScaleEnabled = bEnabled;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		else
		{
			strValue = pProp->GetValue();
			brParam.brLimitMinScale = ::_tcstod(strValue, NULL);
		}
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_LIMIT_TOL_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < brParam.brLimitTolLSL ) 
		{	dValue = brParam.brLimitTolLSL;	}		
		brParam.brLimitTolUSL = dValue;		
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_LIMIT_TOL_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > brParam.brLimitTolUSL ) 
		{	dValue = brParam.brLimitTolUSL;	}
		brParam.brLimitTolLSL = dValue;
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;

	case WND_ALG_PROPERTY_BRIGHT_RATIO_RATIO_ENABLED:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		brParam.brRatioEnabled = bEnabled;
		pProp->Expand(bEnabled);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_RATIO_AREA:
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_RATIO_USL:		
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < brParam.brRatioLSL ) 
		{	dValue = brParam.brRatioLSL;	}		
		brParam.brRatioUSL = dValue;		
		bIsPass = CAlgParam::CheckOK_BrightRatioRatioUSL(brParam);		
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_RATIO_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > brParam.brRatioUSL ) 
		{	dValue = brParam.brRatioUSL;	}
		brParam.brRatioLSL = dValue;				
		bIsPass = CAlgParam::CheckOK_BrightRatioRatioLSL(brParam);		
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);		
		bChanged = true;
		break;	

	case WND_ALG_PROPERTY_BRIGHT_RATIO_RANGE_ENABLED:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		brParam.brRangeEnabled = bEnabled;
		pProp->Expand(bEnabled);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_RANGE_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < brParam.brRangeLSL ) 
		{	dValue = brParam.brRangeLSL;	}		
		brParam.brRangeUSL = dValue;				
		bIsPass = CAlgParam::CheckOK_BrightRatioRangeUSL(brParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.0f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_RANGE_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > brParam.brRangeUSL ) 
		{	dValue = brParam.brRangeUSL;	}
		brParam.brRangeLSL = dValue;				
		bIsPass = CAlgParam::CheckOK_BrightRatioRangeLSL(brParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.0f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_CONTRAST_ENABLED:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		brParam.brContrastEnabled = bEnabled;
		pProp->Expand(bEnabled);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_CONTRAST_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < brParam.brContrastLSL ) 
		{	dValue = brParam.brContrastLSL;	}		
		brParam.brContrastUSL = dValue;				
		bIsPass = CAlgParam::CheckOK_BrightRatioContrastUSL(brParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_CONTRAST_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > brParam.brContrastUSL ) 
		{	dValue = brParam.brContrastUSL;	}
		brParam.brContrastLSL = dValue;				
		bIsPass = CAlgParam::CheckOK_BrightRatioContrastLSL(brParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_BRIGHT_RATIO_XLINE_ENABLED:		
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		brParam.brXLineEnabled = bEnabled;
		pProp->Expand(bEnabled);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_XLINE_RANGE:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 1 ) 
		{	dValue = 1;	}		
		brParam.brXLineRange = dValue;		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_XLINE_MODE:
		strValue = pProp->GetValue();
		nValue = AOIDataDefine.FindAlgBrightLineModeByText(strValue);
		switch ( nValue ) 
		{
		case LINE_MODE_DARK:
		case LINE_MODE_BRIGHT:
			brParam.brXLineMode = nValue;
			bChanged = true;
			break;
		}		
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_XLINE_UNIT_MODE:
		strValue = pProp->GetValue();
		nValue = AOIDataDefine.FindAlgCalcUnitModeByText(strValue);
		switch ( nValue ) 
		{
		case ALG_CALC_UNIT_ABS:
		case ALG_CALC_UNIT_RATIO:
			brParam.brXLineUnitMode = (ALG_CALC_UNIT_MODE)(nValue);
			bChanged = true;
			break;
		}	
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_XLINE_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < brParam.brXLineLSL ) 
		{	dValue = brParam.brXLineLSL;	}		
		brParam.brXLineUSL = dValue;				
		bIsPass = CAlgParam::CheckOK_BrightRatioLineXUSL(brParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_XLINE_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > brParam.brXLineUSL ) 
		{	dValue = brParam.brXLineUSL;	}
		brParam.brXLineLSL = dValue;				
		bIsPass = CAlgParam::CheckOK_BrightRatioLineXLSL(brParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_YLINE_ENABLED:
		bEnabled = pProp->GetCheckValue();	
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		brParam.brYLineEnabled = bEnabled;
		pProp->Expand(bEnabled);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_YLINE_RANGE:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 1 ) 
		{	dValue = 1;	}		
		brParam.brYLineRange = dValue;		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_YLINE_MODE:	
		strValue = pProp->GetValue();
		nValue = AOIDataDefine.FindAlgBrightLineModeByText(strValue);
		switch ( nValue ) 
		{
		case LINE_MODE_DARK:
		case LINE_MODE_BRIGHT:
			brParam.brYLineMode = nValue;
			bChanged = true;
			break;
		}		
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_YLINE_UNIT_MODE:
		strValue = pProp->GetValue();
		nValue = AOIDataDefine.FindAlgCalcUnitModeByText(strValue);
		switch ( nValue ) 
		{
		case ALG_CALC_UNIT_ABS:
		case ALG_CALC_UNIT_RATIO:
			brParam.brYLineUnitMode = (ALG_CALC_UNIT_MODE)(nValue);
			bChanged = true;
			break;
		}	
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_YLINE_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < brParam.brYLineLSL ) 
		{	dValue = brParam.brYLineLSL;	}		
		brParam.brYLineUSL = dValue;				
		bIsPass = CAlgParam::CheckOK_BrightRatioLineYUSL(brParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BRIGHT_RATIO_YLINE_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > brParam.brYLineUSL ) 
		{	dValue = brParam.brYLineUSL;	}
		brParam.brYLineLSL = dValue;				
		bIsPass = CAlgParam::CheckOK_BrightRatioLineYLSL(brParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	}

	if ( false == bBoolParam )
	{	strNewValue = pProp->GetValue(); }
	Changed.sValueName = strName;
	Changed.sValueOld = strOldValue;
	Changed.sValueNew = strNewValue;	

	Changed.bParamChanged = bChanged;
	Changed.bWndRgnChanged = bWndRgnChanged;
	return true;
}
//-------------------------------------------------------------------------------------//