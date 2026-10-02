// EditWndView_EdgeSearch.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditWndView.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_EdgeSearch(CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }	
	
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const TALG_PARAM_EDGE_SEARCH &esParam = AlgParam.GetAlgParamEdgeSearch();

	CString str;
	bool    bEnabled=false;
	CString strCaption, strValue, strDescr, strUnit, strOption;
	CString strReadingX, strReadingY, strReadingS, strResultID;
	CJETPropertyGridProperty* pParamItem = NULL;
	CJETPropertyGridProperty* pGroupBasic = NULL;
	CJETPropertyGridProperty* pGroupEdge = NULL;
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
	if ( BuildWndParamList_RegionLink(pGroupBasic, WndPtr, false) == false )
	{	return false;	}

	//General Setting	
	strCaption = _T("Cut Line");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue = _T("");
	bEnabled = esParam.esCutLineEnabled;
	pParamItem = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_CUT_LINE_ENB);		
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Invert Align");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue = _T("");
	bEnabled = esParam.esInvertAlign;
	pParamItem = new CJETPropertyGridProperty(strCaption, (_variant_t)bEnabled, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_INVERT_ALIGN);		
	pGroupBasic->AddSubItem(pParamItem);

	const bool bShowX=AlgParam.CheckAlgShowOffsetByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowY=AlgParam.CheckAlgShowOffsetByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowA=AlgParam.CheckAlgShowSkewByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowS=AlgParam.CheckAlgShowScaleByAlgType(AlgParam.GetAlgType());//false;
	if ( BuildWndParamList_MatchOffset(pGroupBasic, Project, WndPtr, bShowX, bShowY, bShowA, bShowS) == false )
	{	return false; }	

	//結果文字顯示
	if ( BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false )
	{	return false; }

	//增加Edge Search參數
	double dUSL = 0;
	double dLSL = 0;
	double dReading = 0;	
	
	//Front Search
	str = _T("Front Search");	
	str = LoadMultiLanguageString(str, str);
	strResultID = AOIDataDefine.GetResultIDText(esParam.esResultID_F);
	strCaption.Format(_T("%s [%s]"), str, strResultID);
	pGroupEdge = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupEdge ) 
	{	return false;	}	
	pGroupEdge->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_ENABLE_F);
	pGroupEdge->SetCheckValue(esParam.esEnabled_F);
	pGroupEdge->SetData((DWORD_PTR)WndPtr);
	//pGroupBasic->AddSubItem(pGroupEdge);
	wndPropList.AddProperty(pGroupEdge, bRedraw, bAdjustLayou);
	
	//尺寸計算模式
	strCaption = _T("Size Calc Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgObjectSizeCalcModeText(esParam.esSizeCalcMode_F);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_SIZE_CALC_MODE_F);	
	strOption = AOIDataDefine.GetAlgObjectSizeCalcModeText(ALG_OBJECT_SIZE_CALC_BOUNDARY);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgObjectSizeCalcModeText(ALG_OBJECT_SIZE_CALC_AVERAGE);	pParamItem->AddOption(strOption);	
	pGroupEdge->AddSubItem(pParamItem);

	//搜尋方向
	strCaption = _T("Search Direction");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyAlgSearchDirectionList(strCaption, esParam.esSearchDir_F, strDescr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_SEARCH_DIRECTION_F);	
	pParamItem->SetData((DWORD_PTR)WndPtr);	
	pGroupEdge->AddSubItem(pParamItem);
	
	//
	strUnit = _T("%");
	strCaption = _T("Min Ratio U");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), esParam.esMinRatioU_F);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_MIN_RATIO_U_F);	
	pParamItem->SetReading(strUnit);
	pGroupEdge->AddSubItem(pParamItem);	
	
	strUnit = _T("%");
	strCaption = _T("Max Ratio U");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), esParam.esMaxRatioU_F);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_MAX_RATIO_U_F);	
	pParamItem->SetReading(strUnit);
	pGroupEdge->AddSubItem(pParamItem);	

	strUnit = _T("um");
	strCaption = _T("Min Range V");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), esParam.esMinRangeV_F);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_MIN_RANGE_V_F);	
	pParamItem->SetReading(strUnit);
	pGroupEdge->AddSubItem(pParamItem);

	strUnit = _T("um");
	strCaption = _T("Max Range V");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), esParam.esMaxRangeV_F);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_MAX_RANGE_V_F);	
	pParamItem->SetReading(strUnit);
	pGroupEdge->AddSubItem(pParamItem);
	
	strUnit = _T("%");
	strCaption = _T("Size Ratio V");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), esParam.esSizeRatioV_F);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_SIZE_RATIO_V_F);	
	pParamItem->SetReading(strUnit);
	pGroupEdge->AddSubItem(pParamItem);
	
	strUnit = _T("%");
	strCaption = _T("Search Ratio V");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), esParam.esSearchRatioV_F);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_FIND_RATIO_V_F);	
	pParamItem->SetReading(strUnit);
	pGroupEdge->AddSubItem(pParamItem);

	if ( false == esParam.esEnabled_F )
	{	pGroupEdge->Expand(FALSE);	}
	
	//Back Search	
	str = _T("Back Search");	
	str = LoadMultiLanguageString(str, str);
	strResultID = AOIDataDefine.GetResultIDText(esParam.esResultID_B);
	strCaption.Format(_T("%s [%s]"), str, strResultID);
	pGroupEdge = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupEdge ) 
	{	return false;	}	
	pGroupEdge->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_ENABLE_B);
	pGroupEdge->SetCheckValue(esParam.esEnabled_B);
	pGroupEdge->SetData((DWORD_PTR)WndPtr);
	//pGroupBasic->AddSubItem(pGroupEdge);
	wndPropList.AddProperty(pGroupEdge, bRedraw, bAdjustLayou);	

	//尺寸計算模式
	strCaption = _T("Size Calc Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgObjectSizeCalcModeText(esParam.esSizeCalcMode_B);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_SIZE_CALC_MODE_B);	
	strOption = AOIDataDefine.GetAlgObjectSizeCalcModeText(ALG_OBJECT_SIZE_CALC_BOUNDARY);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgObjectSizeCalcModeText(ALG_OBJECT_SIZE_CALC_AVERAGE);	pParamItem->AddOption(strOption);	
	pGroupEdge->AddSubItem(pParamItem);
	
	//搜尋方向
	strCaption = _T("Search Direction");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyAlgSearchDirectionList(strCaption, esParam.esSearchDir_B, strDescr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_SEARCH_DIRECTION_B);	
	pParamItem->SetData((DWORD_PTR)WndPtr);	
	pGroupEdge->AddSubItem(pParamItem);

	strUnit = _T("%");
	strCaption = _T("Min Ratio U");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), esParam.esMinRatioU_B);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }		
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_MIN_RATIO_U_B);	
	pParamItem->SetReading(strUnit);
	pGroupEdge->AddSubItem(pParamItem);	

	//
	strUnit = _T("%");
	strCaption = _T("Max Ratio U");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), esParam.esMaxRatioU_B);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }		
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_MAX_RATIO_U_B);	
	pParamItem->SetReading(strUnit);
	pGroupEdge->AddSubItem(pParamItem);	

	strUnit = _T("um");
	strCaption = _T("Min Range V");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), esParam.esMinRangeV_B);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_MIN_RANGE_V_B);	
	pParamItem->SetReading(strUnit);
	pGroupEdge->AddSubItem(pParamItem);

	strUnit = _T("um");
	strCaption = _T("Max Range V");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), esParam.esMaxRangeV_B);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_MAX_RANGE_V_B);	
	pParamItem->SetReading(strUnit);
	pGroupEdge->AddSubItem(pParamItem);
	
	strUnit = _T("%");
	strCaption = _T("Size Ratio V");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), esParam.esSizeRatioV_B);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_SIZE_RATIO_V_B);	
	pParamItem->SetReading(strUnit);
	pGroupEdge->AddSubItem(pParamItem);
	
	strUnit = _T("%");
	strCaption = _T("Search Ratio V");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), esParam.esSearchRatioV_B);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_EDGE_SEARCH_FIND_RATIO_V_B);	
	pParamItem->SetReading(strUnit);
	pGroupEdge->AddSubItem(pParamItem);

	if ( false == esParam.esEnabled_B )
	{	pGroupEdge->Expand(FALSE);	}
	
	
	if ( BuildWndParamList_ExtendRange(wndPropList, Project, WndPtr) == false )
	{	return false;	}	

	//邏輯參數
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
bool CEditWndView::ExecWndParamListChanged_EdgeSearch(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();
	TALG_PARAM_EDGE_SEARCH   &esParam  = AlgParam.GetAlgParamEdgeSearch();

	bool          bChanged = false;	
	bool          bBoolParam = false;
	int           nValue=0, nReading=0;
	bool          bValue=false, bReading=false, bEnabled=true;
	BOOL          BValue=FALSE, BReading=FALSE;
	double        dValue=0.0, dReading=0.0;	
	CString       strValue;
	CString       strName = pProp->GetName();
	CString       strDescr = pProp->GetDescription();
	COleVariant   vtValue = pProp->GetValue();	
	CString       strNewValue(pProp->GetValue());
	CString       strOldValue(pProp->GetOriginalValue());
	DWORD_PTR     dwData = pProp->GetData();	
	BOOL          bClickBtn = pProp->GetClickUserBtn();	
	CJETPropertyGridProperty *pProp2 = NULL;	
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());	
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;

	switch ( ParamID )
	{
	case WND_ALG_PROPERTY_EDGE_SEARCH_CUT_LINE_ENB:		
		bBoolParam = true;
		if ( FALSE == vtValue.boolVal )
		{	bEnabled = false; }
		else
		{	bEnabled = true; }
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		esParam.esCutLineEnabled = bEnabled;
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_EDGE_SEARCH_INVERT_ALIGN:		
		bBoolParam = true;
		if ( FALSE == vtValue.boolVal )
		{	bEnabled = false; }
		else
		{	bEnabled = true; }
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		esParam.esInvertAlign = bEnabled;
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_EDGE_SEARCH_ENABLE_F:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		esParam.esEnabled_F = bEnabled;
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_EDGE_SEARCH_SIZE_CALC_MODE_F:
		strValue = pProp->GetValue();
		esParam.esSizeCalcMode_F = AOIDataDefine.FindAlgObjectSizeCalcModeByText(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_EDGE_SEARCH_SEARCH_DIRECTION_F:		
		strValue = pProp->GetValue();
		esParam.esSearchDir_F = AOIDataDefine.FindAlgSearchDirectionByText(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_EDGE_SEARCH_MIN_RATIO_U_F:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )	{	dValue = 0.0; }		
		esParam.esMinRatioU_F = MIN(dValue, esParam.esMaxRatioU_F);
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_EDGE_SEARCH_MAX_RATIO_U_F:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )	{	dValue = 0.0; }		
		esParam.esMaxRatioU_F = MAX(dValue, esParam.esMinRatioU_F);;		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_EDGE_SEARCH_MIN_RANGE_V_F:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )	{	dValue = 0.0; }		
		esParam.esMinRangeV_F = MIN(dValue, esParam.esMaxRangeV_F);		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_EDGE_SEARCH_MAX_RANGE_V_F:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )	{	dValue = 0.0; }		
		esParam.esMaxRangeV_F = MAX(dValue, esParam.esMinRangeV_F);		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_EDGE_SEARCH_SIZE_RATIO_V_F:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )	{	dValue = 0.0; }
		if ( dValue > 100 )	{	dValue = 100.0; }
		esParam.esSizeRatioV_F = dValue;
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_EDGE_SEARCH_FIND_RATIO_V_F:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 1 )	{	dValue = 1.0; }
		if ( dValue > 100 )	{	dValue = 100.0; }
		esParam.esSearchRatioV_F = dValue;
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_EDGE_SEARCH_ENABLE_B:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		esParam.esEnabled_B = bEnabled;
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_EDGE_SEARCH_SIZE_CALC_MODE_B:
		strValue = pProp->GetValue();
		esParam.esSizeCalcMode_B = AOIDataDefine.FindAlgObjectSizeCalcModeByText(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_EDGE_SEARCH_SEARCH_DIRECTION_B:
		strValue = pProp->GetValue();
		esParam.esSearchDir_B = AOIDataDefine.FindAlgSearchDirectionByText(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_EDGE_SEARCH_MIN_RATIO_U_B:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )	{	dValue = 0.0; }		
		esParam.esMinRatioU_B = MIN(dValue, esParam.esMaxRatioU_B);		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_EDGE_SEARCH_MAX_RATIO_U_B:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )	{	dValue = 0.0; }		
		esParam.esMaxRatioU_B = MAX(dValue, esParam.esMinRatioU_B);
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;	
		//
	case WND_ALG_PROPERTY_EDGE_SEARCH_MIN_RANGE_V_B:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )	{	dValue = 0.0; }		
		esParam.esMinRangeV_B = MIN(dValue, esParam.esMaxRangeV_B);
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;		
	case WND_ALG_PROPERTY_EDGE_SEARCH_MAX_RANGE_V_B:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )	{	dValue = 0.0; }		
		esParam.esMaxRangeV_B = MAX(dValue, esParam.esMinRangeV_B);
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_EDGE_SEARCH_SIZE_RATIO_V_B:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )	{	dValue = 0.0; }
		if ( dValue > 100 )	{	dValue = 100.0; }
		esParam.esSizeRatioV_B = dValue;
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;		
	case WND_ALG_PROPERTY_EDGE_SEARCH_FIND_RATIO_V_B:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 1 )	{	dValue = 1.0; }
		if ( dValue > 100 )	{	dValue = 100.0; }
		esParam.esSearchRatioV_B = dValue;
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	}

	if ( false == bBoolParam  )
	{	strNewValue = pProp->GetValue();	}
	Changed.sValueName = strName;
	Changed.sValueOld = strOldValue;
	Changed.sValueNew = strNewValue;

	Changed.bParamChanged = bChanged;	
	return true;
}
//-------------------------------------------------------------------------------------//