// EditWndView_ObjectMeasure.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditWndView.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
#include "InputListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_ObjectMeasure(CAOIProject *Project, CAOIWnd *WndPtr)
{	
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }		
	
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const TALG_PARAM_OBJECT_MEASURE &omParam = AlgParam.GetAlgParamObjectMeasure();
	
	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	CString str;
	CString strReadingS;
	bool    bIsPass=true;
	bool    Enable = true;
	double  USL=0, LSL=0;
	double  DiffUSL=0, DiffLSL=0;
	double  RatioUSL=0, RatioLSL=0, Spec=0, Reading=0.0, Ratio=0.0, PartialH=0.0, PartialL=0.0;
	CString strCaption, strValue, strDescr, strUnit, strReading, strOption, strDiff;	
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
	pGroupBasic->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_BEGIN);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);	

	if ( BuildWndParamList_WndDefectID(pGroupBasic, Project, WndPtr) == false )
	{	return false;	}	
	if ( BuildWndParamList_AlgFrameIndex(pGroupBasic, Project, WndPtr) == false )
	{	return false;	}	
	if ( BuildWndParamList_MaskFrameIndex(pGroupBasic, Project, WndPtr) == false )
	{	return false;	}
	if ( BuildWndParamList_RegionLink(pGroupBasic, WndPtr, false) == false )
	{	return false;	}

	const bool bShowX=AlgParam.CheckAlgShowOffsetByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowY=AlgParam.CheckAlgShowOffsetByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowA=AlgParam.CheckAlgShowSkewByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowS=AlgParam.CheckAlgShowScaleByAlgType(AlgParam.GetAlgType());//false;
	if ( BuildWndParamList_MatchOffset(pGroupBasic, Project, WndPtr, bShowX, bShowY, bShowA, bShowS) == false )
	{	return false; }	

	//標準設定	
	strValue = _T("");
	strCaption = _T("Set Standard");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SPEC_SETTING);
	pParamItem->SetHasUserBtn();
	pParamItem->AllowEdit(FALSE);
	pGroupBasic->AddSubItem(pParamItem);	

	//多個物件模式	
	strCaption = _T("Multi Blob");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	pParamItem = new CJETPropertyGridProperty(strCaption, (_variant_t)omParam.omUseMultiBlob, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_USE_MULTI_BLOB);	
	//pParamItem->SetCheckValue(bEnabled);	
	pGroupBasic->AddSubItem(pParamItem);	

	//尺寸計算模式
	strCaption = _T("Size Calc Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgObjectSizeCalcModeText(omParam.omSizeCalcMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_CALC_MODE);	
	strOption = AOIDataDefine.GetAlgObjectSizeCalcModeText(ALG_OBJECT_SIZE_CALC_BOUNDARY);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgObjectSizeCalcModeText(ALG_OBJECT_SIZE_CALC_AVERAGE);	pParamItem->AddOption(strOption);	
	strOption = AOIDataDefine.GetAlgObjectSizeCalcModeText(ALG_OBJECT_SIZE_CALC_AVE_RECT);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgObjectSizeCalcModeText(ALG_OBJECT_SIZE_CALC_BLUR_RECT);	pParamItem->AddOption(strOption);
	pGroupBasic->AddSubItem(pParamItem);	
	
	//尺寸計算單位
	strCaption = _T("Size Calc Unit");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgCalcUnitModeText(omParam.omSizeCalcUnitMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_CALC_UNIT_MODE);	
	strOption = AOIDataDefine.GetAlgCalcUnitModeText(ALG_CALC_UNIT_DIFF);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgCalcUnitModeText(ALG_CALC_UNIT_RATIO);	pParamItem->AddOption(strOption);
	pGroupBasic->AddSubItem(pParamItem);	

	//尺寸平滑參數
	strCaption = _T("Size Blue Size");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%d"), omParam.omSizeBlurSize);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }		
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_BLUR_PARAM);	
	pParamItem->SetReading(_T("pxl"));
	pGroupBasic->AddSubItem(pParamItem);

	//結果文字顯示
	if ( BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false )
	{	return false; }

	//寬度參數	
	Spec = omParam.omSizeXSpec;
	DiffUSL = omParam.omSizeXDiffUSL;
	DiffLSL = omParam.omSizeXDiffLSL;
	RatioUSL = omParam.omSizeXRatioUSL;
	RatioLSL = omParam.omSizeXRatioLSL;
	Enable = omParam.omSizeXEnabled;
	Reading = omParam.omSizeXReading;	
	if ( fabs(Spec-0.001) > 0 )
	{	Ratio = 100.0*Reading/Spec; }
	else
	{	Ratio = 0.0; }
	if ( ALG_CALC_UNIT_DIFF == omParam.omSizeCalcUnitMode )
	{	
		USL = Spec+DiffUSL;
		LSL = Spec+DiffLSL;
	}
	else
	{
		USL = Spec*RatioUSL/100;
		LSL = Spec*RatioLSL/100;
	}
	strReading.Format(_T("%.1f"), Ratio);
	strDiff.Format(_T("%.0f"), Reading-Spec);
	//strReading.Format(_T("%.1f%%"), Ratio);
	strReadingS.Format(_T("%.0f"), Reading);	

	str = _T("Size X");
	str = LoadMultiLanguageString(str, str);	
	strCaption.Format(_T("%s [%.0f um]"), str, Reading);
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_ENB);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);		
	pGroupBasic->SetCheckValue(Enable);		
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);
	
	strCaption = _T("Standard");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), Spec);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_SPEC);
	pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);	
	
	strCaption = _T("Result");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);			
	strValue.Format(_T("%.0f"), Reading);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_READING);	
	bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeX(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetValueTextColor(clrNG); }
	else
	{	pParamItem->SetValueTextColor(clrOK); }	
	pParamItem->AllowEdit(FALSE);
	pGroupBasic->AddSubItem(pParamItem);		
	
	strCaption = _T("USL(um)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.0f"), DiffUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_DIFF_USL);
	pParamItem->SetReading(strDiff);
	bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeXDiffUSL(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);
	if ( ALG_CALC_UNIT_DIFF != omParam.omSizeCalcUnitMode )
	{	pParamItem->Show(FALSE);	}

	strCaption = _T("LSL(um)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.0f"), DiffLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_DIFF_LSL);
	pParamItem->SetReading(strDiff);
	bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeXDiffLSL(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);	
	if ( ALG_CALC_UNIT_DIFF != omParam.omSizeCalcUnitMode )
	{	pParamItem->Show(FALSE);	}
	
	strCaption = _T("USL(%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.2f"), RatioUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_RATIO_USL);
	pParamItem->SetReading(strReading);
	bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeXRatioUSL(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);
	if ( ALG_CALC_UNIT_RATIO != omParam.omSizeCalcUnitMode )
	{	pParamItem->Show(FALSE);	}

	strCaption = _T("LSL(%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.2f"), RatioLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_RATIO_LSL);
	pParamItem->SetReading(strReading);
	bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeXRatioLSL(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);		
	if ( ALG_CALC_UNIT_RATIO != omParam.omSizeCalcUnitMode )
	{	pParamItem->Show(FALSE);	}
	pGroupBasic->Expand(Enable);//要在加入項目後

	//長度參數	
	Spec = omParam.omSizeYSpec;
	DiffUSL = omParam.omSizeYDiffUSL;
	DiffLSL = omParam.omSizeYDiffLSL;
	RatioUSL = omParam.omSizeYRatioUSL;
	RatioLSL = omParam.omSizeYRatioLSL;
	Enable = omParam.omSizeYEnabled;
	Reading = omParam.omSizeYReading;	
	if ( fabs(Spec-0.001) > 0 )
	{	Ratio = 100.0*Reading/Spec; }
	else
	{	Ratio = 0.0; }
	if ( ALG_CALC_UNIT_DIFF == omParam.omSizeCalcUnitMode )
	{	
		USL = Spec+DiffUSL;
		LSL = Spec+DiffLSL;
	}
	else
	{
		USL = Spec*RatioUSL/100;
		LSL = Spec*RatioLSL/100;
	}
	strReading.Format(_T("%.1f"), Ratio);
	strDiff.Format(_T("%.0f"), Reading-Spec);
	//strReading.Format(_T("%.1f%%"), Ratio);
	strReadingS.Format(_T("%.0f"), Reading);

	str = _T("Size Y");
	str = LoadMultiLanguageString(str, str);	
	strCaption.Format(_T("%s [%.0f um]"), str, Reading);
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_ENB);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);		
	pGroupBasic->SetCheckValue(Enable);	
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);	

	strCaption = _T("Standard");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), Spec);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_SPEC);
	pParamItem->SetHasUserBtn();	
	pGroupBasic->AddSubItem(pParamItem);
	
	strCaption = _T("Result");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.0f"), Reading);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_READING);	
	bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeY(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetValueTextColor(clrNG); }
	else
	{	pParamItem->SetValueTextColor(clrOK); }	
	pParamItem->AllowEdit(FALSE);
	pGroupBasic->AddSubItem(pParamItem);
	
	strCaption = _T("USL(um)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.0f"), DiffUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_DIFF_USL);
	pParamItem->SetReading(strDiff);
	bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeYDiffUSL(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);
	if ( ALG_CALC_UNIT_DIFF != omParam.omSizeCalcUnitMode )
	{	pParamItem->Show(FALSE);	}

	strCaption = _T("LSL(um)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.0f"), DiffLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_DIFF_LSL);
	pParamItem->SetReading(strDiff);
	bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeYDiffLSL(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);	
	if ( ALG_CALC_UNIT_DIFF != omParam.omSizeCalcUnitMode )
	{	pParamItem->Show(FALSE);	}	

	strCaption = _T("USL(%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.2f"), RatioUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_RATIO_USL);
	pParamItem->SetReading(strReading);
	bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeYRatioUSL(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);
	if ( ALG_CALC_UNIT_RATIO != omParam.omSizeCalcUnitMode )
	{	pParamItem->Show(FALSE);	}

	strCaption = _T("LSL(%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), RatioLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_RATIO_LSL);
	pParamItem->SetReading(strReading);
	bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeYRatioLSL(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);
	if ( ALG_CALC_UNIT_RATIO != omParam.omSizeCalcUnitMode )
	{	pParamItem->Show(FALSE);	}
	pGroupBasic->Expand(Enable);//要在加入項目後	

	//高度參數	
	Spec = omParam.omHeightSpec;
	DiffUSL = omParam.omHeightDiffUSL;
	DiffLSL = omParam.omHeightDiffLSL;
	RatioUSL = omParam.omHeightRatioUSL;
	RatioLSL = omParam.omHeightRatioLSL;
	Enable = omParam.omHeightEnabled;
	Reading = omParam.omHeightReading;
	PartialH = omParam.omHeightAveragePartialH;
	PartialL = omParam.omHeightAveragePartialL;
	if ( fabs(Spec-0.001) > 0 )
	{	Ratio = 100.0*Reading/Spec; }
	else
	{	Ratio = 0.0; }
	strReading.Format(_T("%.1f%%"), Ratio);
	strDiff.Format(_T("%.0f"), Reading-Spec);
	strReadingS.Format(_T("%.0f"), Reading);

	str = _T("Height");
	str = LoadMultiLanguageString(str, str);	
	strCaption.Format(_T("%s [%.0f um]"), str, Reading);
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_ENB);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);		
	pGroupBasic->SetCheckValue(Enable);	
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);	

	strCaption = _T("Average Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgObjectHeightAverageModeText(omParam.omHeightAverageMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_AVE_MODE);	
	strOption = AOIDataDefine.GetAlgObjectHeightAverageModeText(ALG_OBJECT_HEIGHT_AVERAGE_FULL);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgObjectHeightAverageModeText(ALG_OBJECT_HEIGHT_AVERAGE_PARTIAL);	pParamItem->AddOption(strOption);				
	pGroupBasic->AddSubItem(pParamItem);	
	
	strReading = _T("%");
	strCaption = _T("Parital USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), PartialH);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_AVE_PARTIAL_USL);	
	pParamItem->SetReading(strReading);
	if ( ALG_OBJECT_HEIGHT_AVERAGE_PARTIAL != omParam.omHeightAverageMode ) 
	{	pParamItem->Show(FALSE); }
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Parital LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), PartialL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_AVE_PARTIAL_LSL);	
	pParamItem->SetReading(strReading);
	if ( ALG_OBJECT_HEIGHT_AVERAGE_PARTIAL != omParam.omHeightAverageMode ) 
	{	pParamItem->Show(FALSE); }
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Standard");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), Spec);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_SPEC);
	pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Result");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), Reading);
	strReading.Format(_T("%.1f"), Ratio);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_READING);	
	bIsPass = CAlgParam::CheckOK_ObjectMeasureHeight(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetValueTextColor(clrNG); }
	else
	{	pParamItem->SetValueTextColor(clrOK); }	
	pParamItem->AllowEdit(FALSE);
	pGroupBasic->AddSubItem(pParamItem);
	
	strCaption = _T("Calc Unit");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgCalcUnitModeText(omParam.omHeightCalcUnitMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_CALC_UNIT_MODE);	
	strOption = AOIDataDefine.GetAlgCalcUnitModeText(ALG_CALC_UNIT_DIFF);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgCalcUnitModeText(ALG_CALC_UNIT_RATIO);	pParamItem->AddOption(strOption);
	pGroupBasic->AddSubItem(pParamItem);
	
	strCaption = _T("USL(um)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.0f"), DiffUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_DIFF_USL);
	pParamItem->SetReading(strDiff);
	bIsPass = CAlgParam::CheckOK_ObjectMeasureHeightDiffUSL(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);
	if ( ALG_CALC_UNIT_DIFF != omParam.omHeightCalcUnitMode )
	{	pParamItem->Show(FALSE);	}	

	strCaption = _T("LSL(um)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.0f"), DiffLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_DIFF_LSL);
	pParamItem->SetReading(strDiff);
	bIsPass = CAlgParam::CheckOK_ObjectMeasureHeightDiffLSL(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);
	if ( ALG_CALC_UNIT_DIFF != omParam.omHeightCalcUnitMode )
	{	pParamItem->Show(FALSE);	}	
	
	strCaption = _T("USL(%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.2f"), RatioUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_RATIO_USL);
	pParamItem->SetReading(strReading);
	bIsPass = CAlgParam::CheckOK_ObjectMeasureHeightRatioUSL(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);
	if ( ALG_CALC_UNIT_RATIO != omParam.omHeightCalcUnitMode )
	{	pParamItem->Show(FALSE);	}	

	strCaption = _T("LSL(%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.2f"), RatioLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_RATIO_LSL);
	pParamItem->SetReading(strReading);
	bIsPass = CAlgParam::CheckOK_ObjectMeasureHeightRatioLSL(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);
	if ( ALG_CALC_UNIT_RATIO != omParam.omHeightCalcUnitMode )
	{	pParamItem->Show(FALSE);	}	
	pGroupBasic->Expand(Enable);//要在加入項目後
	
	//面積參數	
	Spec = omParam.omAreaSpec;
	RatioUSL = omParam.omAreaUSL;
	RatioLSL = omParam.omAreaLSL;
	Enable = omParam.omAreaEnabled;
	Reading = omParam.omAreaReading;	
	if ( fabs(Spec-0.001) > 0 )
	{	Ratio = 100.0*Reading/Spec; }
	else
	{	Ratio = 0.0; }
	strReading.Format(_T("%.1f"), Ratio);
	strDiff.Format(_T("%.0f"), Reading-Spec);
	//strReading.Format(_T("%.1f%%"), Ratio);	
	strReadingS.Format(_T("%.0f"), Reading);
	
	str = _T("Area");
	str = LoadMultiLanguageString(str, str);	
	strCaption.Format(_T("%s [%.0f um^2]"), str, Reading);
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_AREA_ENB);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);		
	pGroupBasic->SetCheckValue(Enable);	
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);	

	strCaption = _T("Standard");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), Spec);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_AREA_SPEC);
	pParamItem->SetHasUserBtn();	
	pGroupBasic->AddSubItem(pParamItem);
	
	strCaption = _T("Result");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.0f"), Reading);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_AREA_READING);	
	bIsPass = CAlgParam::CheckOK_ObjectMeasureArea(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetValueTextColor(clrNG); }
	else
	{	pParamItem->SetValueTextColor(clrOK); }	
	pParamItem->AllowEdit(FALSE);
	pGroupBasic->AddSubItem(pParamItem);
	
	strCaption = _T("USL(%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.2f"), RatioUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_AREA_RATIO_USL);
	pParamItem->SetReading(strReading);
	bIsPass = CAlgParam::CheckOK_ObjectMeasureAreaRatioUSL(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("LSL(%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.2f"), RatioLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_AREA_RATIO_LSL);
	pParamItem->SetReading(strReading);
	bIsPass = CAlgParam::CheckOK_ObjectMeasureAreaRatioLSL(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);
	pGroupBasic->Expand(Enable);//要在加入項目後	

	//體積參數	
	Spec = omParam.omVolumeSpec;
	RatioUSL = omParam.omVolumeUSL;
	RatioLSL = omParam.omVolumeLSL;
	Enable = omParam.omVolumeEnabled;
	Reading = omParam.omVolumeReading;	
	if ( fabs(Spec-0.001) > 0 )
	{	Ratio = 100.0*Reading/Spec; }
	else
	{	Ratio = 0.0; }
	strReading.Format(_T("%.1f"), Ratio);
	strDiff.Format(_T("%.0f"), Reading-Spec);
	//strReading.Format(_T("%.1f%%"), Ratio);
	strReadingS.Format(_T("%.0f"), Reading);	

	str = _T("Volume");
	str = LoadMultiLanguageString(str, str);	
	strCaption.Format(_T("%s [%.0f um^3]"), str, Reading);
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_VOLUME_ENB);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);		
	pGroupBasic->SetCheckValue(Enable);	
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);	

	strCaption = _T("Standard");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), Spec);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_VOLUME_SPEC);
	pParamItem->SetHasUserBtn();	
	pGroupBasic->AddSubItem(pParamItem);
	
	strCaption = _T("Result");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.0f"), Reading);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_VOLUME_READING);	
	bIsPass = CAlgParam::CheckOK_ObjectMeasureVolume(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetValueTextColor(clrNG); }
	else
	{	pParamItem->SetValueTextColor(clrOK); }	
	pParamItem->AllowEdit(FALSE);
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("USL(%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.2f"), RatioUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_VOLUME_RATIO_USL);
	pParamItem->SetReading(strReading);
	bIsPass = CAlgParam::CheckOK_ObjectMeasureVolumeRatioUSL(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);
	
	strCaption = _T("LSL(%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue.Format(_T("%.2f"), RatioLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OBJECT_MEASURE_VOLUME_RATIO_LSL);
	pParamItem->SetReading(strReading);
	bIsPass = CAlgParam::CheckOK_ObjectMeasureVolumeRatioLSL(omParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);
	pGroupBasic->Expand(Enable);//要在加入項目後	

	//外擴範圍設定
	if ( BuildWndParamList_ExtendRange(wndPropList, Project, WndPtr) == false )
	{	return false;	}

	//邏輯設定
	if ( BuildWndParamList_LogicParam(wndPropList, Project, WndPtr) == false )
	{	return false; }

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
bool CEditWndView::ExecWndParamListChanged_ObjectMeasure(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel   *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd     *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();		
	BOX_SHAPE_MODE WndShapeMode = WndPtr->GetWndShapeMode();
	TALG_PARAM_OBJECT_MEASURE  &omParam = AlgParam.GetAlgParamObjectMeasure();
	
	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	bool          bIsPass=true;
	bool          bBoolParam = false;
	int           nValue=0, nReading=0;
	bool          bChanged = false, bReBuildWndUI=false;		
	bool          bShowDiff=true, bShowRatio=true;
	bool          bValue=false, bReading=false, bEnabled=true;
	BOOL          BValue=FALSE, BReading=FALSE, bClickBtn=FALSE, BShow=TRUE;
	double        dValue=0.0, dReading=0.0, dRatio=0.0;	
	TREGION4D     WndRegion;
	CString       str;
	CString       strCaption, strLabel;
	CString       strValue, strRatio, strDiff;
	CString       strName = pProp->GetName();
	CString       strDescr = pProp->GetDescription();
	COleVariant   vtValue = pProp->GetValue();	
	CString       strNewValue(pProp->GetValue());
	CString       strOldValue(pProp->GetOriginalValue());
	DWORD_PTR     dwData = pProp->GetData();
	TListNode       Node;		
	CInputBoxWnd    InputBox;
	CInputListWnd   EnumWnd;		
	std::vector<TListNode> NodelList;		
	CJETPropertyGridProperty *pProp2 = NULL;		
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());	
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;
	
	bClickBtn = pProp->GetClickUserBtn();
	WndPtr->GetWndRegion(WndRegion);
	const double WndSize = WndRegion.GetWidth()*WndRegion.GetHeight();
	
	switch ( ParamID )
	{
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SPEC_SETTING:
		if ( TRUE == bClickBtn )
		{
			Node.Data = 1;//By Wnd Spec
			Node.Text = AOIDataDefine.GetWndText();
			NodelList.push_back(Node);

			Node.Data = 2;//By Reading			
			Node.Text = AOIDataDefine.GetResultText();
			NodelList.push_back(Node);
			
			str = _T("Standard");
			str = LoadMultiLanguageString(str, str);
			strCaption = strLabel = str;			
			EnumWnd.SetParam1(strCaption, strLabel, 1, NodelList);
			if ( EnumWnd.DoModal() == IDCANCEL )
			{	return false; }

			nValue = (int)(EnumWnd.GetSelData());	
			if ( 2 == nValue )
			{	
				omParam.omSizeXSpec = omParam.omSizeXReading;
				omParam.omSizeYSpec = omParam.omSizeYReading;
				omParam.omHeightSpec = omParam.omHeightReading;
				omParam.omAreaSpec = omParam.omAreaReading;
				omParam.omVolumeSpec = omParam.omVolumeReading;
			}
			else
			{	
				strCaption = _T("Input Height");				
				strName = AOIDataDefine.GetThicknessText();		
				strValue.Format(_T("%.0f"), omParam.omHeightReading);
				InputBox.SetParam1(strCaption, strName, strValue);
				if ( InputBox.DoModal() == IDCANCEL ) { return false; }
				dValue = ::_ttof(InputBox.m_DataEdit1);

				omParam.omSizeXSpec = WndRegion.GetWidth();
				omParam.omSizeYSpec = WndRegion.GetHeight();
				omParam.omHeightSpec = dValue;
				if ( BOX_SHAPE_ELLIPSE == WndShapeMode )
				{	omParam.omAreaSpec = WndRegion.GetArea()*PI_RAD/4.0;	}
				else
				{	omParam.omAreaSpec = WndRegion.GetArea(); }
				omParam.omVolumeSpec = omParam.omAreaSpec*dValue;
			}			
			bChanged = true;
			bReBuildWndUI = true;
		}
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_USE_MULTI_BLOB:
		bEnabled = (bool)(vtValue.boolVal);

		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		omParam.omUseMultiBlob = bEnabled;
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_CALC_MODE:		
		strValue = pProp->GetValue();
		omParam.omSizeCalcMode = AOIDataDefine.FindAlgObjectSizeCalcModeByText(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_CALC_UNIT_MODE:
		strValue = pProp->GetValue();
		omParam.omSizeCalcUnitMode = AOIDataDefine.FindAlgCalcUnitModeByText(strValue);		
		bChanged = true;

		if ( ALG_CALC_UNIT_DIFF == omParam.omSizeCalcUnitMode )
		{
			bShowDiff = true;
			bShowRatio = false;			
		}
		else
		{
			bShowDiff = false;
			bShowRatio = true;	
		}
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_DIFF_USL);
		if ( NULL != pProp2 )
		{	pProp2->Show(bShowDiff); }
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_DIFF_LSL);
		if ( NULL != pProp2 )
		{	pProp2->Show(bShowDiff); }
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_RATIO_USL);
		if ( NULL != pProp2 )
		{	pProp2->Show(bShowRatio); }
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_RATIO_LSL);
		if ( NULL != pProp2 )
		{	pProp2->Show(bShowRatio); }

		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_DIFF_USL);
		if ( NULL != pProp2 )
		{	pProp2->Show(bShowDiff); }
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_DIFF_LSL);
		if ( NULL != pProp2 )
		{	pProp2->Show(bShowDiff); }
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_RATIO_USL);
		if ( NULL != pProp2 )
		{	pProp2->Show(bShowRatio); }
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_RATIO_LSL);
		if ( NULL != pProp2 )
		{	pProp2->Show(bShowRatio); }
		
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_BLUR_PARAM:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		nValue = MAX(1, nValue);
		omParam.omSizeBlurSize = nValue;
		bChanged = true;		
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_ENB:
		bEnabled = pProp->GetCheckValue();		
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		omParam.omSizeXEnabled = bEnabled;
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_SPEC:
		if ( TRUE == bClickBtn )
		{
			Node.Data = 1;
			str = AOIDataDefine.GetWndText();
			Node.Text.Format(_T("%s:%.0f um"), str, WndRegion.GetWidth());
			NodelList.push_back(Node);

			Node.Data = 2;
			str = AOIDataDefine.GetResultText();
			Node.Text.Format(_T("%s:%.0f um"), str, omParam.omSizeXReading);
			NodelList.push_back(Node);
			
			strCaption = strLabel = AOIDataDefine.GetSizeXText();
			EnumWnd.SetParam1(strCaption, strLabel, 1, NodelList);
			if ( EnumWnd.DoModal() == IDCANCEL )
			{	return false; }

			nValue = (int)(EnumWnd.GetSelData());	
			if ( 2 == nValue )
			{	dValue = omParam.omSizeXReading; }
			else
			{	dValue = WndRegion.GetWidth(); }
			strValue.Format(_T("%.0f"), dValue);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);				
		}		
		bEnabled = pProp->GetCheckValue();		
		omParam.omSizeXSpec = dValue;
		bEnabled = true;
		bChanged = true;

		if ( true == bEnabled )
		{
			if ( fabs(dValue) > 0.0001 )
			{	dRatio = 100.0*omParam.omSizeXReading/dValue; }
			else
			{	dRatio = 0.0; }			
			strRatio.Format(_T("%.1f"), dRatio);			
			strDiff.Format(_T("%.0f"), omParam.omSizeXReading-dValue); 
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_READING);
			if ( NULL != pProp2 )
			{	
				//pProp2->Show(bEnabled);
				//pProp2->SetReading(strRatio);	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeX(omParam);
				if ( false == bIsPass )
				{	pProp2->SetValueTextColor(clrNG); }
				else
				{	pProp2->SetValueTextColor(clrOK); }
				pProp2->Redraw();
			}

			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_DIFF_USL);
			if ( NULL != pProp2 )
			{	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeXDiffUSL(omParam);
				if ( false == bIsPass )
				{	pProp2->SetReadingTextColor(clrNG); }
				else
				{	pProp2->SetReadingTextColor(clrOK); }
				pProp2->SetReading(strDiff);	
				pProp2->Redraw();
			}
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_DIFF_LSL);
			if ( NULL != pProp2 )
			{	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeXDiffLSL(omParam);
				if ( false == bIsPass )
				{	pProp2->SetReadingTextColor(clrNG); }
				else
				{	pProp2->SetReadingTextColor(clrOK); }
				pProp2->SetReading(strDiff);
				pProp2->Redraw();
			}
			
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_RATIO_USL);
			if ( NULL != pProp2 )
			{	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeXRatioUSL(omParam);
				if ( false == bIsPass )
				{	pProp2->SetReadingTextColor(clrNG); }
				else
				{	pProp2->SetReadingTextColor(clrOK); }
				pProp2->SetReading(strRatio);	
				pProp2->Redraw();
			}
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_RATIO_LSL);
			if ( NULL != pProp2 )
			{	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeXRatioLSL(omParam);
				if ( false == bIsPass )
				{	pProp2->SetReadingTextColor(clrNG); }
				else
				{	pProp2->SetReadingTextColor(clrOK); }
				pProp2->SetReading(strRatio);	
				pProp2->Redraw();
			}
		}	
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_READING:
		strValue.Format(_T("%.0f"), omParam.omSizeXReading);
		pProp->SetValue(strValue);
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_DIFF_USL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);		
		omParam.omSizeXDiffUSL = MAX(dValue, omParam.omSizeXDiffLSL);
		bChanged = true;

		strValue.Format(_T("%.0f"), omParam.omSizeXDiffUSL);
		pProp->SetValue(strValue);	
		bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeXDiffUSL(omParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }

		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_DIFF_LSL);
		if ( NULL != pProp2 )
		{
			omParam.omSizeXDiffLSL = -omParam.omSizeXDiffUSL;
			strValue.Format(_T("%.0f"), omParam.omSizeXDiffLSL);
			pProp2->SetValue(strValue);	
			bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeXDiffLSL(omParam);
			if ( false == bIsPass )
			{	pProp2->SetReadingTextColor(clrNG); }
			else
			{	pProp2->SetReadingTextColor(clrOK); }
			pProp2->Redraw();
		}
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_DIFF_LSL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);		
		omParam.omSizeXDiffLSL = MIN(dValue, omParam.omSizeXDiffUSL);
		bChanged = true;
		
		strValue.Format(_T("%.0f"), omParam.omSizeXDiffLSL);
		pProp->SetValue(strValue);	
		bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeXDiffLSL(omParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_RATIO_USL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);		
		omParam.omSizeXRatioUSL = MAX(dValue, omParam.omSizeXRatioLSL);
		bChanged = true;

		strValue.Format(_T("%.2f"), omParam.omSizeXRatioUSL);
		pProp->SetValue(strValue);	
		bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeXRatioUSL(omParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_X_RATIO_LSL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);		
		omParam.omSizeXRatioLSL = MIN(dValue, omParam.omSizeXRatioUSL);
		bChanged = true;

		strValue.Format(_T("%.2f"), omParam.omSizeXRatioLSL);
		pProp->SetValue(strValue);	
		bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeXRatioLSL(omParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		break;

	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_ENB:
		bEnabled = pProp->GetCheckValue();		
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		omParam.omSizeYEnabled = bEnabled;
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_SPEC:
		if ( TRUE == bClickBtn )
		{
			Node.Data = 1;
			str = AOIDataDefine.GetWndText();
			Node.Text.Format(_T("%s:%.0f um"), str, WndRegion.GetHeight());
			NodelList.push_back(Node);

			Node.Data = 2;
			str = AOIDataDefine.GetResultText();
			Node.Text.Format(_T("%s:%.0f um"), str, omParam.omSizeYReading);
			NodelList.push_back(Node);

			strCaption = strLabel = AOIDataDefine.GetSizeYText();
			EnumWnd.SetParam1(strCaption, strLabel, 1, NodelList);
			if ( EnumWnd.DoModal() == IDCANCEL )
			{	return false; }

			nValue = (int)(EnumWnd.GetSelData());	
			if ( 2 == nValue )
			{	dValue = omParam.omSizeYReading; }
			else
			{	dValue = WndRegion.GetHeight(); }
			strValue.Format(_T("%.0f"), dValue);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);				
		}		
		bEnabled = pProp->GetCheckValue();		
		omParam.omSizeYSpec = dValue;
		bEnabled = true;
		bChanged = true;
	
		if ( true == bEnabled )
		{
			if ( fabs(dValue) > 0.0001 )
			{	dRatio = 100.0*omParam.omSizeYReading/dValue; }
			else
			{	dRatio = 0.0; }
			strRatio.Format(_T("%.1f"), dRatio);
			strDiff.Format(_T("%.0f"), omParam.omSizeYReading-dValue); 
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_READING);
			if ( NULL != pProp2 )
			{	
				//pProp2->Show(bEnabled);
				//pProp2->SetReading(strRatio);	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeY(omParam);
				if ( false == bIsPass )
				{	pProp2->SetValueTextColor(clrNG); }
				else
				{	pProp2->SetValueTextColor(clrOK); }
				pProp2->Redraw();
			}

			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_DIFF_USL);
			if ( NULL != pProp2 )
			{	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeYDiffUSL(omParam);
				if ( false == bIsPass )
				{	pProp2->SetReadingTextColor(clrNG); }
				else
				{	pProp2->SetReadingTextColor(clrOK); }
				pProp2->SetReading(strDiff);	
				pProp2->Redraw();
			}	
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_DIFF_LSL);
			if ( NULL != pProp2 )
			{	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeYDiffLSL(omParam);
				if ( false == bIsPass )
				{	pProp2->SetReadingTextColor(clrNG); }
				else
				{	pProp2->SetReadingTextColor(clrOK); }
				pProp2->SetReading(strDiff);	
				pProp2->Redraw();
			}
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_RATIO_USL);
			if ( NULL != pProp2 )
			{	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeYRatioUSL(omParam);
				if ( false == bIsPass )
				{	pProp2->SetReadingTextColor(clrNG); }
				else
				{	pProp2->SetReadingTextColor(clrOK); }
				pProp2->SetReading(strRatio);	
				pProp2->Redraw();
			}
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_RATIO_LSL);
			if ( NULL != pProp2 )
			{	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeYRatioLSL(omParam);
				if ( false == bIsPass )
				{	pProp2->SetReadingTextColor(clrNG); }
				else
				{	pProp2->SetReadingTextColor(clrOK); }
				pProp2->SetReading(strRatio);	
				pProp2->Redraw();
			}
		}	
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_READING:
		strValue.Format(_T("%.0f"), omParam.omSizeYReading);
		pProp->SetValue(strValue);
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_DIFF_USL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);		
		omParam.omSizeYDiffUSL = MAX(dValue, omParam.omSizeYDiffLSL);
		bChanged = true;

		strValue.Format(_T("%.0f"), omParam.omSizeYDiffUSL);
		pProp->SetValue(strValue);	
		bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeYDiffUSL(omParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }

		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_DIFF_LSL);
		if ( NULL != pProp2 )
		{
			omParam.omSizeYDiffLSL = -omParam.omSizeYDiffUSL;
			strValue.Format(_T("%.0f"), omParam.omSizeYDiffLSL);
			pProp2->SetValue(strValue);	
			bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeYDiffLSL(omParam);
			if ( false == bIsPass )
			{	pProp2->SetReadingTextColor(clrNG); }
			else
			{	pProp2->SetReadingTextColor(clrOK); }
			pProp2->Redraw();
		}
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_DIFF_LSL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);		
		omParam.omSizeYDiffLSL = MIN(dValue, omParam.omSizeYDiffUSL);
		bChanged = true;

		strValue.Format(_T("%.0f"), omParam.omSizeYDiffLSL);
		pProp->SetValue(strValue);		
		bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeYDiffLSL(omParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_RATIO_USL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);		
		omParam.omSizeYRatioUSL = MAX(dValue, omParam.omSizeYRatioLSL);
		bChanged = true;

		strValue.Format(_T("%.2f"), omParam.omSizeYRatioUSL);
		pProp->SetValue(strValue);	
		bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeYRatioUSL(omParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_SIZE_Y_RATIO_LSL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);		
		omParam.omSizeYRatioLSL = MIN(dValue, omParam.omSizeYRatioUSL);
		bChanged = true;

		strValue.Format(_T("%.2f"), omParam.omSizeYRatioLSL);
		pProp->SetValue(strValue);	
		bIsPass = CAlgParam::CheckOK_ObjectMeasureSizeYRatioLSL(omParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		break;

	case WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_ENB:
		bEnabled = pProp->GetCheckValue();		
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		omParam.omHeightEnabled = bEnabled;
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_SPEC:
		if ( TRUE == bClickBtn )
		{	
			dValue = omParam.omHeightReading;
			strValue.Format(_T("%.0f"), dValue);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);				
		}
		bEnabled = pProp->GetCheckValue();		
		omParam.omHeightSpec = dValue;
		bEnabled = true;
		bChanged = true;

		if ( true == bEnabled )
		{
			if ( fabs(dValue) > 0.0001 )
			{	dRatio = 100.0*omParam.omHeightReading/dValue; }
			else
			{	dRatio = 0.0; }
			strRatio.Format(_T("%.1f"), dRatio);
			strDiff.Format(_T("%.0f"), omParam.omHeightReading-dValue); 
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_READING);
			if ( NULL != pProp2 )
			{	
				//pProp2->Show(bEnabled);
				//pProp2->SetReading(strRatio);
				bIsPass = CAlgParam::CheckOK_ObjectMeasureHeight(omParam);
				if ( false == bIsPass ) 
				{	pProp2->SetValueTextColor(clrNG);	}
				else
				{	pProp2->SetValueTextColor(clrOK);	}				
				pProp2->Redraw();
			}
			
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_DIFF_USL);
			if ( NULL != pProp2 )
			{	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureHeightDiffUSL(omParam);
				if ( false == bIsPass )
				{	pProp2->SetReadingTextColor(clrNG); }
				else
				{	pProp2->SetReadingTextColor(clrOK); }
				pProp2->SetReading(strDiff);
				pProp2->Redraw();
			}

			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_DIFF_LSL);
			if ( NULL != pProp2 )
			{	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureHeightDiffLSL(omParam);
				if ( false == bIsPass )
				{	pProp2->SetReadingTextColor(clrNG); }
				else
				{	pProp2->SetReadingTextColor(clrOK); }
				pProp2->SetReading(strDiff);
				pProp2->Redraw();
			}

			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_RATIO_USL);
			if ( NULL != pProp2 )
			{	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureHeightRatioUSL(omParam);
				if ( false == bIsPass )
				{	pProp2->SetReadingTextColor(clrNG); }
				else
				{	pProp2->SetReadingTextColor(clrOK); }
				pProp2->SetReading(strRatio);		
				pProp2->Redraw();
			}
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_RATIO_LSL);
			if ( NULL != pProp2 )
			{	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureHeightRatioLSL(omParam);
				if ( false == bIsPass )
				{	pProp2->SetReadingTextColor(clrNG); }
				else
				{	pProp2->SetReadingTextColor(clrOK); }
				pProp2->SetReading(strRatio);
				pProp2->Redraw();
			}
		}		
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_AVE_MODE:
		BShow = TRUE;
		strValue = pProp->GetValue();
		omParam.omHeightAverageMode = AOIDataDefine.FindAlgObjectHeightAverageModeByText(strValue);
		if ( ALG_OBJECT_HEIGHT_AVERAGE_PARTIAL == omParam.omHeightAverageMode )
		{	BShow = TRUE; }
		else
		{	BShow = FALSE; }
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_AVE_PARTIAL_USL);
		if ( NULL != pProp2 )
		{	pProp2->Show(BShow); }
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_AVE_PARTIAL_LSL);
		if ( NULL != pProp2 )
		{	pProp2->Show(BShow); }			
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_AVE_PARTIAL_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0.0 ) 
		{	dValue = 0.0;	}		
		else if ( dValue > 100.0 )
		{	dValue = 100.0; }
		if ( dValue < omParam.omHeightAveragePartialL ) 
		{	dValue = omParam.omHeightAveragePartialL;	}		
		omParam.omHeightAveragePartialH = dValue;
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_AVE_PARTIAL_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0.0 ) 
		{	dValue = 0.0;	}		
		else if ( dValue > 100.0 )
		{	dValue = 100.0; }
		if ( dValue > omParam.omHeightAveragePartialH ) 
		{	dValue = omParam.omHeightAveragePartialH;	}		
		omParam.omHeightAveragePartialL = dValue;
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;		
	case WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_READING:
		strValue.Format(_T("%.0f"), omParam.omHeightReading);
		pProp->SetValue(strValue);
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_CALC_UNIT_MODE:
		strValue = pProp->GetValue();
		omParam.omHeightCalcUnitMode = AOIDataDefine.FindAlgCalcUnitModeByText(strValue);		
		bChanged = true;

		if ( ALG_CALC_UNIT_DIFF == omParam.omHeightCalcUnitMode )
		{
			bShowDiff = true;
			bShowRatio = false;			
		}
		else
		{
			bShowDiff = false;
			bShowRatio = true;	
		}
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_DIFF_USL);
		if ( NULL != pProp2 )
		{	pProp2->Show(bShowDiff); }
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_DIFF_LSL);
		if ( NULL != pProp2 )
		{	pProp2->Show(bShowDiff); }
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_RATIO_USL);
		if ( NULL != pProp2 )
		{	pProp2->Show(bShowRatio); }
		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_RATIO_LSL);
		if ( NULL != pProp2 )
		{	pProp2->Show(bShowRatio); }		
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_DIFF_USL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);				
		omParam.omHeightDiffUSL = MAX(dValue, omParam.omHeightDiffLSL);
		bChanged = true;
		
		strValue.Format(_T("%.0f"), omParam.omHeightDiffUSL);
		pProp->SetValue(strValue);	
		bIsPass = CAlgParam::CheckOK_ObjectMeasureHeightDiffUSL(omParam);
		if ( false == bIsPass )		
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }

		pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_DIFF_LSL);
		if ( NULL != pProp2 )
		{
			omParam.omHeightDiffLSL = -omParam.omHeightDiffUSL;
			strValue.Format(_T("%.0f"), omParam.omHeightDiffLSL);
			pProp2->SetValue(strValue);	
			bIsPass = CAlgParam::CheckOK_ObjectMeasureHeightDiffLSL(omParam);
			if ( false == bIsPass )
			{	pProp2->SetReadingTextColor(clrNG); }
			else
			{	pProp2->SetReadingTextColor(clrOK); }
			pProp2->Redraw();
		}
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_DIFF_LSL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);				
		omParam.omHeightDiffLSL = MIN(dValue, omParam.omHeightDiffUSL);
		bChanged = true;

		strValue.Format(_T("%.0f"), omParam.omHeightDiffLSL);
		pProp->SetValue(strValue);	
		bIsPass = CAlgParam::CheckOK_ObjectMeasureHeightDiffLSL(omParam);
		if ( false == bIsPass )		
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_RATIO_USL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);				
		omParam.omHeightRatioUSL = MAX(dValue, omParam.omHeightRatioLSL);
		bChanged = true;

		strValue.Format(_T("%.2f"), omParam.omHeightRatioUSL);
		pProp->SetValue(strValue);	
		bIsPass = CAlgParam::CheckOK_ObjectMeasureHeightRatioUSL(omParam);
		if ( false == bIsPass )		
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_HEIGHT_RATIO_LSL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);				
		omParam.omHeightRatioLSL = MIN(dValue, omParam.omHeightRatioUSL);
		bChanged = true;

		strValue.Format(_T("%.2f"), omParam.omHeightRatioLSL);
		pProp->SetValue(strValue);	
		bIsPass = CAlgParam::CheckOK_ObjectMeasureHeightRatioLSL(omParam);
		if ( false == bIsPass )		
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		break;

	case WND_ALG_PROPERTY_OBJECT_MEASURE_AREA_ENB:
		bEnabled = pProp->GetCheckValue();	
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		omParam.omAreaEnabled = bEnabled;
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_AREA_SPEC:
		if ( TRUE == bClickBtn )
		{
			Node.Data = 1;
			str = AOIDataDefine.GetWndText();
			if ( BOX_SHAPE_ELLIPSE == WndShapeMode )
			{	Node.Text.Format(_T("%s:%.0f um^2"), str, WndRegion.GetArea()*PI_RAD/4.0);	}
			else
			{	Node.Text.Format(_T("%s:%.0f um^2"), str, WndRegion.GetArea()); }
			NodelList.push_back(Node);

			Node.Data = 2;
			str = AOIDataDefine.GetResultText();
			Node.Text.Format(_T("%s:%.0f um^2"), str, omParam.omAreaReading);
			NodelList.push_back(Node);
			
			strCaption = strLabel = AOIDataDefine.GetAreaText();
			EnumWnd.SetParam1(strCaption, strLabel, 1, NodelList);
			if ( EnumWnd.DoModal() == IDCANCEL )
			{	return false; }

			nValue = (int)(EnumWnd.GetSelData());	
			if ( 2 == nValue )
			{	dValue = omParam.omAreaReading; }
			else
			{
				if ( BOX_SHAPE_ELLIPSE == WndShapeMode )
				{	dValue = WndRegion.GetArea()*PI_RAD/4.0;	}
				else
				{	dValue = WndRegion.GetArea();  }
			}
			strValue.Format(_T("%.0f"), dValue);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);				
		}		
		bEnabled = pProp->GetCheckValue();		
		omParam.omAreaSpec = dValue;
		bEnabled = true;
		bChanged = true;

		if ( true == bEnabled )
		{
			if ( fabs(dValue) > 0.0001 )
			{	dRatio = 100.0*omParam.omAreaReading/dValue; }
			else
			{	dRatio = 0.0; }
			strRatio.Format(_T("%.1f"), dRatio);
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_AREA_READING);
			if ( NULL != pProp2 )
			{	
				//pProp2->Show(bEnabled);
				//pProp2->SetReading(strRatio);	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureArea(omParam);
				if ( false == bIsPass )		
				{	pProp2->SetValueTextColor(clrNG); }
				else
				{	pProp2->SetValueTextColor(clrOK); }
				pProp2->Redraw();
			}

			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_AREA_RATIO_USL);
			if ( NULL != pProp2 )
			{	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureAreaRatioUSL(omParam);
				if ( false == bIsPass )		
				{	pProp2->SetReadingTextColor(clrNG); }
				else
				{	pProp2->SetReadingTextColor(clrOK); }
				pProp2->SetReading(strRatio);	
				pProp2->Redraw();
			}
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_AREA_RATIO_LSL);
			if ( NULL != pProp2 )
			{	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureAreaRatioLSL(omParam);
				if ( false == bIsPass )		
				{	pProp2->SetReadingTextColor(clrNG); }
				else
				{	pProp2->SetReadingTextColor(clrOK); }								
				pProp2->SetReading(strRatio);	
				pProp2->Redraw();
			}
		}	
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_AREA_READING:
		strValue.Format(_T("%.0f"), omParam.omAreaReading);
		pProp->SetValue(strValue);
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_AREA_RATIO_USL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);		
		omParam.omAreaUSL = MAX(dValue, omParam.omAreaLSL);
		bChanged = true;

		strValue.Format(_T("%.2f"), omParam.omAreaUSL);
		pProp->SetValue(strValue);	
		bIsPass = CAlgParam::CheckOK_ObjectMeasureAreaRatioUSL(omParam);
		if ( false == bIsPass )		
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_AREA_RATIO_LSL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);		
		omParam.omAreaLSL = MIN(dValue, omParam.omAreaUSL);
		bChanged = true;

		strValue.Format(_T("%.2f"), omParam.omAreaLSL);
		pProp->SetValue(strValue);	
		bIsPass = CAlgParam::CheckOK_ObjectMeasureAreaRatioLSL(omParam);
		if ( false == bIsPass )		
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		break;

	case WND_ALG_PROPERTY_OBJECT_MEASURE_VOLUME_ENB:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		omParam.omVolumeEnabled = bEnabled;
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_VOLUME_SPEC:
		if ( TRUE == bClickBtn )
		{
			dValue = omParam.omVolumeReading;
			strValue.Format(_T("%.0f"), dValue);
			pProp->SetValue(strValue);
		}
		else
		{
			strValue = pProp->GetValue();		
			dValue = ::_tcstod(strValue, NULL);				
		}		
		bEnabled = pProp->GetCheckValue();		
		omParam.omVolumeSpec = dValue;
		bEnabled = true;
		bChanged = true;

		if ( true == bEnabled )
		{
			if ( fabs(dValue) > 0.0001 )
			{	dRatio = 100.0*omParam.omVolumeReading/dValue; }
			else
			{	dRatio = 0.0; }
			strRatio.Format(_T("%.1f"), dRatio);
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_VOLUME_READING);
			if ( NULL != pProp2 )
			{
				//pProp2->Show(bEnabled);
				//pProp2->SetReading(strRatio);	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureVolume(omParam);
				if ( false == bIsPass )		
				{	pProp2->SetValueTextColor(clrNG); }
				else
				{	pProp2->SetValueTextColor(clrOK); }
				pProp2->Redraw();
			}

			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_VOLUME_RATIO_USL);
			if ( NULL != pProp2 )
			{	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureVolumeRatioUSL(omParam);
				if ( false == bIsPass )		
				{	pProp2->SetReadingTextColor(clrNG); }
				else
				{	pProp2->SetReadingTextColor(clrOK); }				
				pProp2->SetReading(strRatio);	
				pProp2->Redraw();
			}
			pProp2 = wndPropList.FindItemByID(WND_ALG_PROPERTY_OBJECT_MEASURE_VOLUME_RATIO_LSL);
			if ( NULL != pProp2 )
			{	
				bIsPass = CAlgParam::CheckOK_ObjectMeasureVolumeRatioLSL(omParam);
				if ( false == bIsPass )		
				{	pProp2->SetReadingTextColor(clrNG); }
				else
				{	pProp2->SetReadingTextColor(clrOK); }				
				pProp2->SetReading(strRatio);	
				pProp2->Redraw();
			}
		}	
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_VOLUME_READING:
		strValue.Format(_T("%.0f"), omParam.omVolumeReading);
		pProp->SetValue(strValue);
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_VOLUME_RATIO_USL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);		
		omParam.omVolumeUSL = MAX(dValue, omParam.omVolumeLSL);
		bChanged = true;

		strValue.Format(_T("%.2f"), omParam.omVolumeUSL);
		pProp->SetValue(strValue);	
		bIsPass = CAlgParam::CheckOK_ObjectMeasureVolumeRatioUSL(omParam);
		if ( false == bIsPass )		
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		break;
	case WND_ALG_PROPERTY_OBJECT_MEASURE_VOLUME_RATIO_LSL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);		
		omParam.omVolumeLSL = MIN(dValue, omParam.omVolumeUSL);
		bChanged = true;

		strValue.Format(_T("%.2f"), omParam.omVolumeLSL);
		pProp->SetValue(strValue);	
		bIsPass = CAlgParam::CheckOK_ObjectMeasureVolumeRatioLSL(omParam);
		if ( false == bIsPass )		
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		break;
	}

	if ( false == bBoolParam )
	{	strNewValue = pProp->GetValue();	}
	Changed.sValueName = strName;
	Changed.sValueOld = strOldValue;
	Changed.sValueNew = strNewValue;	

	Changed.bReBuildWndUI = bReBuildWndUI;
	Changed.bParamChanged = bChanged;		
	return true;
}
//-------------------------------------------------------------------------------------//