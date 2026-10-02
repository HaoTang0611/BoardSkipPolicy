// EditWndView_MeasurementMeasureSIP.cpp : implementation file
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
bool CEditWndView::ExecWndParamListChanged_MeasureSIP(CJETPropertyGridProperty * pProp, TWND_PARAM_CHANGED_RESULT & Changed)
{
	if (NULL == pProp) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if (NULL == ModelPtr) { return true; }
	CAOIProject *Project = GetActiveProject();
	if (NULL == Project) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if (NULL == WndPtr) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();
	TALG_PARAM_MEASURE_SIP   &msParam = AlgParam.GetAlgParamMeasureSIP();

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	bool          bChanged = false;
	bool          bBoolParam = false;
	bool          bReBuildWndUI = false;
	int           nValue = 0, nReading = 0, nTemp;
	bool          bValue = false, bReading = false, bEnabled = true;
	BOOL          BValue = FALSE, BReading = FALSE;
	double        dValue = 0.0, dReading = 0.0;
	CString       strValue, strReading;
	CString       strName = pProp->GetName();
	CString       strDescr = pProp->GetDescription();
	COleVariant   vtValue = pProp->GetValue();
	CString       strNewValue(pProp->GetValue());
	CString       strOldValue(pProp->GetOriginalValue());
	DWORD_PTR     dwData = pProp->GetData();
	CJETPropertyGridProperty *pProp2 = NULL;
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;

	switch (ParamID)
	{
	case WND_ALG_PROPERTY_MEASURE_SIP_BEGIN:
		break;
	case WND_ALG_PROPERTY_MEASURE_SIP_DIRECTION:
		strValue = pProp->GetValue();
		//msParam.msDirection = AOIDataDefine.FindAlgAngleMeasureLineEqnModeByText(strValue);
		msParam.msDirection = AOIDataDefine.FindAlgDirectionByXYText(strValue);
		bChanged = true;
		break;

	case WND_ALG_PROPERTY_MEASURE_SIP_INSPEC_EDGE_COUNT:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		nTemp = msParam.msInspecEdgeCount;
		msParam.msInspecEdgeCount = nValue;
		if (false == ExecWndParamListChanged_WndRoi_Modify_Auto(WndPtr, ParamID)) {
			msParam.msInspecEdgeCount = nTemp;
		}
		msParam.ClearReading();
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_MEASURE_SIP_REF_EDGE_COUNT:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		nTemp = msParam.msRefEdgeCount;
		msParam.msRefEdgeCount = nValue;		
		if (false == ExecWndParamListChanged_WndRoi_Modify_Auto(WndPtr, ParamID)) {
			msParam.msRefEdgeCount = nTemp;
		}
		msParam.ClearReading();
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_MEASURE_SIP_ANGLE_SPEC:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);
		if (dValue < 0) { dValue = 0.0; }
		msParam.msAngleSpec = dValue;
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_MEASURE_SIP_ANGLE_TOLERANCE_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);
		dValue = MAX(dValue, 0);		
		msParam.msAngleTolUSL = dValue;
		strValue.Format(_T("%.2f"), dValue);
		strReading.Format(_T("%.2f"), msParam.msAngleSpec+dValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		pProp->SetReading(strReading);
		if ( CAlgParam::CheckOK_AngleMeasureTolUSL(msParam) )
		{	pProp->SetReadingTextColor(clrOK); }
		else
		{	pProp->SetReadingTextColor(clrNG); }
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_MEASURE_SIP_ANGLE_TOLERANCE_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);
		dValue = MIN(dValue, 0);		
		msParam.msAngleTolLSL = dValue;
		strValue.Format(_T("%.2f"), dValue);
		strReading.Format(_T("%.2f"), msParam.msAngleSpec+dValue);
		pProp->SetValue(strValue);		
		pProp->SetOriginalValue(strValue);
		pProp->SetReading(strReading);
		if ( CAlgParam::CheckOK_AngleMeasureTolLSL(msParam) )
		{	pProp->SetReadingTextColor(clrOK); }
		else
		{	pProp->SetReadingTextColor(clrNG); }			
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_MEASURE_SIP_DISTANCE_TO_A_SPEC:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);
		if (dValue < 0)
		{	dValue = 0.0;	}
		msParam.msPtASpec = dValue;
		strValue.Format(_T("%d"), int(dValue));
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_MEASURE_SIP_DISTANCE_TO_A_TOLERANCE_USL:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);
		if (dValue < 0) { dValue = 0.0; }

		msParam.msPtATolUSL = dValue;
		//dReading = (msParam.msPtASpec + msParam.msPtATolUSL) - msParam.msPtAReading;
		dReading = msParam.msPtASpec + msParam.msPtATolUSL;
		strValue.Format(_T("%d"), int(dValue));
		strReading.Format(_T("%.2f"), dReading);
		//if (dReading > 0) { pProp->SetReadingTextColor(clrOK); }
		if (dReading - msParam.msPtAReading > 0) { pProp->SetReadingTextColor(clrOK); }
		else { pProp->SetReadingTextColor(clrNG); }
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		pProp->SetReading(strReading);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_MEASURE_SIP_DISTANCE_TO_A_TOLERANCE_LSL:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);
		if (dValue < 0) { dValue = 0.0; }
		msParam.msPtATolLSL = dValue;
		//dReading = msParam.msPtAReading - (msParam.msPtASpec - msParam.msPtATolLSL);
		dReading = msParam.msPtASpec - msParam.msPtATolLSL;
		strValue.Format(_T("%d"), int(dValue));
		strReading.Format(_T("%.2f"), dReading);
		//if (dReading > 0) { pProp->SetReadingTextColor(clrOK); }
		if (msParam.msPtAReading - dReading > 0) { pProp->SetReadingTextColor(clrOK); }
		else { pProp->SetReadingTextColor(clrNG); }
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		pProp->SetReading(strReading);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_MEASURE_SIP_DISTANCE_TO_B_SPEC:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);
		if (dValue < 0)
		{	dValue = 0.0;	}
		msParam.msPtBSpec = dValue;
		strValue.Format(_T("%d"), int(dValue));
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_MEASURE_SIP_DISTANCE_TO_B_TOLERANCE_USL:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);
		if (dValue < 0) { dValue = 0.0; }
		msParam.msPtBTolUSL = dValue;
		//dReading = (msParam.msPtBSpec + msParam.msPtBTolUSL) - msParam.msPtBReading;
		dReading = (msParam.msPtBSpec + msParam.msPtBTolUSL);
		strValue.Format(_T("%d"), int(dValue));
		strReading.Format(_T("%.2f"), dReading);
		//if (dReading >0) { pProp->SetReadingTextColor(clrOK); }
		if (dReading - msParam.msPtBReading > 0) { pProp->SetReadingTextColor(clrOK); }
		else { pProp->SetReadingTextColor(clrNG); }
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		pProp->SetReading(strReading);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_MEASURE_SIP_DISTANCE_TO_B_TOLERANCE_LSL:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);
		if (dValue < 0)	{	dValue = 0.0;	}

		msParam.msPtBTolLSL = dValue;
		//dReading = msParam.msPtBReading - (msParam.msPtBSpec - msParam.msPtBTolLSL);
		dReading = msParam.msPtBSpec - msParam.msPtBTolLSL;
		strValue.Format(_T("%d"), int(dValue));
		strReading.Format(_T("%.2f"), dReading);
		if (msParam.msPtBReading - dReading > 0) { pProp->SetReadingTextColor(clrOK); }
		else {	pProp->SetReadingTextColor(clrNG);	}

		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		pProp->SetReading(strReading);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_MEASURE_SIP_END:
		break;
	default:
		break;
	}
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_MeasureSIP(CAOIProject * Project, CAOIWnd * WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }	

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	//const TALG_PARAM_ANGLE_MEASURE &amParam = AlgParam.GetAlgParamAngleMeasure();
	//const TALG_PARAM_IMAGE_MATCH &imParam = AlgParam.GetAlgParamImageMatch();
	const TALG_PARAM_MEASURE_SIP &msParam = AlgParam.GetAlgParamMeasureSIP();
	const TALG_PARAM_ANGLE_MEASURE &msParam_am = msParam.msPart_am;

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	CString str;
	CString strCaption, strValue, strDescr, strUnit, strOption;
	CString strReadingX, strReadingY, strReadingS, strResultID;
	CJETPropertyGridProperty* pParamItem = NULL;
	CJETPropertyGridProperty* pGroupBasic = NULL;
	CJETPropertyGridProperty* pGroupLength = NULL;
	CJETPropertyGridProperty* pGroupAdvanced = NULL;
	CJETPropertyGridProperty* pGroupResult = NULL;

	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;
	wndPropList.SetRedraw(FALSE);
	
	strCaption = FormWndParamListCategoryName(WndPtr);	
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_MEASURE_SIP_BEGIN);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);	
	if ( BuildWndParamList_WndDefectID(pGroupBasic, Project, WndPtr) == false )
	{	return false;	}	

	//糤Angle Measure把计
	double dUSL = 0;
	double dLSL = 0;
	double dReading = 0;

	
	strCaption = _T("Distance Direction");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgDirectionXYText(msParam.msDirection);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_SIP_DIRECTION);      
	strOption = AOIDataDefine.GetAlgDirectionXYText(ALG_HORIZONTAL);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgDirectionXYText(ALG_VERTICAL);	pParamItem->AddOption(strOption);
	//strOption = AOIDataDefine.GetAlgDirectionText(ALG_HORIZONTAL);	pParamItem->AddOption(strOption);
	//strOption = AOIDataDefine.GetAlgDirectionText(ALG_VERTICAL);	pParamItem->AddOption(strOption);
	pGroupBasic->AddSubItem(pParamItem);
	//浪代娩计秖
	strCaption = _T("Inspection Edge");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), msParam.msInspecEdgeCount);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_SIP_INSPEC_EDGE_COUNT);
	strOption = _T('1');	pParamItem->AddOption(strOption);
	strOption = _T('2');	pParamItem->AddOption(strOption);
	pGroupBasic->AddSubItem(pParamItem);
	//把σ娩计秖
	strCaption = _T("Reference Edge");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), msParam.msRefEdgeCount);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_SIP_REF_EDGE_COUNT);
	strOption = _T('0');	pParamItem->AddOption(strOption);
	strOption = _T('1');	pParamItem->AddOption(strOption);
	strOption = _T('2');	pParamItem->AddOption(strOption);
	pGroupBasic->AddSubItem(pParamItem);

	if (msParam.msRefEdgeCount > 0) {
		//à夹非-砏絛	
		strUnit = _T("Deg");
		strCaption = _T("Angle Spec.");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);	
		strValue.Format(_T("%.2f"), msParam.msAngleSpec);
		strReadingS.Format(_T("%.2f"), msParam.msAngleReading);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_SIP_ANGLE_SPEC);
		pParamItem->SetReading(strReadingS);
		pGroupBasic->AddSubItem(pParamItem);

		strUnit = _T("Deg");
		strCaption = _T("USL(Deg)");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue.Format(_T("%.2f"), msParam.msAngleTolUSL);
		strReadingS.Format(_T("%.2f"), msParam.msAngleSpec+ msParam.msAngleTolUSL);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_SIP_ANGLE_TOLERANCE_USL);
		pParamItem->SetReading(strReadingS);	
		if ( CAlgParam::CheckOK_AngleMeasureTolUSL(msParam) )
		{	pParamItem->SetReadingTextColor(clrOK); }
		else
		{	pParamItem->SetReadingTextColor(clrNG); }
		pGroupBasic->AddSubItem(pParamItem);

		strUnit = _T("Deg");
		strCaption = _T("LSL(Deg)");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue.Format(_T("%.2f"), msParam.msAngleTolLSL);
		strReadingS.Format(_T("%.2f"), msParam.msAngleSpec+ msParam.msAngleTolLSL);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_SIP_ANGLE_TOLERANCE_LSL);
		pParamItem->SetReading(strReadingS);	
		if ( CAlgParam::CheckOK_AngleMeasureTolLSL(msParam) )
		{	pParamItem->SetReadingTextColor(clrOK); }
		else
		{	pParamItem->SetReadingTextColor(clrNG); }
		pGroupBasic->AddSubItem(pParamItem);
	}

	//弄A
	strUnit = _T("um");
	strCaption = _T("Distance A Spec");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), msParam.msPtASpec);
	strReadingS.Format(_T("%.2f"), msParam.msPtAReading);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_SIP_DISTANCE_TO_A_SPEC);
	pParamItem->SetReading(strReadingS);
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Distance A USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), msParam.msPtATolUSL);
	//strReadingS.Format(_T("%.2f"), (msParam.msPtASpec + msParam.msPtATolUSL) - msParam.msPtAReading);
	strReadingS.Format(_T("%.2f"), (msParam.msPtASpec + msParam.msPtATolUSL));
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_SIP_DISTANCE_TO_A_TOLERANCE_USL);
	pParamItem->SetReading(strReadingS);
	if (msParam.msPtAReading - msParam.msPtASpec < msParam.msPtATolUSL )
	{	pParamItem->SetReadingTextColor(clrOK);	}
	else
	{	pParamItem->SetReadingTextColor(clrNG);	}
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Distance A LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), msParam.msPtATolLSL);
	//strReadingS.Format(_T("%.2f"), msParam.msPtAReading - (msParam.msPtASpec - msParam.msPtATolLSL));
	strReadingS.Format(_T("%.2f"), (msParam.msPtASpec - msParam.msPtATolLSL));
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_SIP_DISTANCE_TO_A_TOLERANCE_LSL);
	pParamItem->SetReading(strReadingS);
	if (msParam.msPtASpec - msParam.msPtAReading  < msParam.msPtATolLSL)
	{	pParamItem->SetReadingTextColor(clrOK);	}
	else
	{	pParamItem->SetReadingTextColor(clrNG);	}
	pGroupBasic->AddSubItem(pParamItem);

	if (msParam.msInspecEdgeCount > 1) {
		//弄B
		strCaption = _T("Distance B Spec");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue.Format(_T("%.2f"), msParam.msPtBSpec);
		strReadingS.Format(_T("%.2f"), msParam.msPtBReading);
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
		if ( NULL == pParamItem ) { return false; }	
		pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_SIP_DISTANCE_TO_B_SPEC);
		pParamItem->SetReading(strReadingS);
		pGroupBasic->AddSubItem(pParamItem);

		strCaption = _T("Distance B USL");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue.Format(_T("%.2f"), msParam.msPtBTolUSL);
		//strReadingS.Format(_T("%.2f"), (msParam.msPtBSpec + msParam.msPtBTolUSL) - msParam.msPtBReading);
		strReadingS.Format(_T("%.2f"), (msParam.msPtBSpec + msParam.msPtBTolUSL));
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
		if (NULL == pParamItem) { return false; }
		pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_SIP_DISTANCE_TO_B_TOLERANCE_USL);
		pParamItem->SetReading(strReadingS);
		if (msParam.msPtBReading - msParam.msPtBSpec  < msParam.msPtBTolUSL)
		{	pParamItem->SetReadingTextColor(clrOK);	}
		else
		{	pParamItem->SetReadingTextColor(clrNG);	}
		pGroupBasic->AddSubItem(pParamItem);

		strCaption = _T("Distance B LSL");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strValue.Format(_T("%.2f"), msParam.msPtBTolLSL);
		//strReadingS.Format(_T("%.2f"), msParam.msPtBReading - (msParam.msPtBSpec - msParam.msPtBTolLSL));
		strReadingS.Format(_T("%.2f"), (msParam.msPtBSpec - msParam.msPtBTolLSL));
		pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
		if (NULL == pParamItem) { return false; }
		pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_SIP_DISTANCE_TO_B_TOLERANCE_LSL);
		pParamItem->SetReading(strReadingS);
		if (msParam.msPtBSpec - msParam.msPtBReading < msParam.msPtBTolLSL)
		{	pParamItem->SetReadingTextColor(clrOK);	}
		else
		{	pParamItem->SetReadingTextColor(clrNG);	}
		pGroupBasic->AddSubItem(pParamItem);
	}
	//////////////////////////////////
	strCaption = _T("Mark Match");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if (NULL == pGroupBasic) { return false; }
	pGroupBasic->SetID(WND_ALG_PROPERTY_PATTERN_MATCH_SCALE_NODE);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);
	if ( BuildWndParamList_AlgFrameIndex(pGroupBasic, Project, WndPtr) == false )
	{	return false;	}
	const bool bShowX = AlgParam.CheckAlgShowOffsetByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowY = AlgParam.CheckAlgShowOffsetByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowA = AlgParam.CheckAlgShowSkewByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowS = AlgParam.CheckAlgShowScaleByAlgType(AlgParam.GetAlgType());//true;
	if (BuildWndParamList_MatchOffset(pGroupBasic, Project, WndPtr, bShowX, bShowY, bShowA, bShowS) == false)
	{	return false;	}

	const int    PatternPolarity = AlgParam.GetAlgPatternPolarity();
	strUnit = _T("um");
	strCaption = _T("Direction");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), PatternPolarity);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_IMAGE_MATCH_POLARITY);
	strValue = _T("1");	pParamItem->AddOption(strValue);
	strValue = _T("2");	pParamItem->AddOption(strValue);
	pGroupBasic->AddSubItem(pParamItem);

	//挡狦ゅ陪ボ
	if (BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false)
	{	return false;	}

	if ( BuildWndParamList_ScaleRatio(wndPropList, Project, WndPtr) == false )
	{	return false;	}

	if ( BuildWndParamList_ExtendRange(wndPropList, Project, WndPtr) == false )
	{	return false;	}

	////把计
	//if ( BuildWndParamList_WndRoi(wndPropList, Project, WndPtr) == false )
	//{	return false;	}
	
	//呸胯把计
	if ( BuildWndParamList_LogicParam(wndPropList, Project, WndPtr) == false )
	{	return false; }	

	//秈顶砞﹚
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
bool CEditWndView::ExecWndParamListChanged_WndRoi_Modify_Auto(CAOIWnd * WndPtr, WND_ALG_PROPERTY_ID ParamID)
{
	if (NULL == WndPtr) { return false; }
	ALG_TYPE AlgType = WndPtr->GetWndAlgType();
	if (ALG_MEASURE_SIP_DISTANCE != AlgType) { return true; }
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	const TALG_PARAM_MEASURE_SIP &msParam = AlgParam.GetAlgParamMeasureSIP();
	size_t i;
	const size_t MarkRoiCount  = 1;
	const size_t InspectionEdgeCount = msParam.msInspecEdgeCount;
	const size_t RefEdgeCount = msParam.msRefEdgeCount;
	const size_t TotalCount = MarkRoiCount + InspectionEdgeCount + RefEdgeCount;
	const size_t WndRoiCount = WndPtr->GetWndRoiWndCount();
	CAOIModel *ModelPtr = GetModelPtr();
	CAOIWndRoi *WndRoiPtr = NULL, *WndRoiTempPtr = NULL;
	size_t RoiIndex = 0, RoiTempCount;
	WndPtr->SetWndAllRoiWndSelected(false);
	if (WndRoiCount < TotalCount) { //add
		std::vector<CAOIWndRoi*> RefRoiPtrList;
		if (ParamID == WND_ALG_PROPERTY_MEASURE_SIP_INSPEC_EDGE_COUNT) {
			RoiIndex = WndRoiCount - RefEdgeCount;
			for (i = RoiIndex; i < WndRoiCount; i++) {
				WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
				if (NULL == WndRoiPtr) { continue; }
				WndRoiTempPtr = WndRoiPtr->CloneWndRoiObj();
				RefRoiPtrList.push_back(WndRoiTempPtr);
				WndRoiPtr->SetWndRoiSelected(true);
			}
			ModelPtr->DeleteModelWndRoiWnd(WndPtr);

			ExecWndParamListChanged_WndRoi_Add(WndPtr);
			WndRoiPtr = WndPtr->GetWndRoiWndSelected();
			WndRoiPtr->SetWndRoiSelfFrameEnabled(true);

			for (i = 0; i < RefRoiPtrList.size() ; i++) {
				WndRoiPtr = RefRoiPtrList[i];
				if (NULL == WndRoiPtr) { continue; }
				WndPtr->AddWndRoiWndPtr(WndRoiPtr, false);
				WndRoiPtr->SetWndRoiSelected(false);
			}
		}
		else {
			for (i = WndRoiCount; i < TotalCount; i++) {
				ExecWndParamListChanged_WndRoi_Add(WndPtr);
			}
		}
	}
	else { // Delete
		if (ParamID == WND_ALG_PROPERTY_MEASURE_SIP_REF_EDGE_COUNT) {
			RoiIndex = WndRoiCount - 1;
			RoiTempCount = TotalCount;
		}
		else {
			RoiIndex = MarkRoiCount+ InspectionEdgeCount;
			RoiTempCount = InspectionEdgeCount + 1;
		}
		for (i = RoiIndex; i >= RoiTempCount; i--) {
			WndRoiPtr = WndPtr->GetWndRoiWndPtr(i, false);
			WndRoiPtr->SetWndRoiSelected(true);
		}
		if (false == ExecWndParamListChanged_WndRoi_Modify_Auto_Delete(WndPtr)) {
			return false;	
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_WndRoi_Modify_Auto_Delete(CAOIWnd * WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }

	CString str;
	str = _T("Do you want to delete roi wnd?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return false; }	

	LogOperCtrl.SaveLogWndRoiSelectedDelete(WndPtr);
	ModelPtr->DeleteModelWndRoiWnd(WndPtr);

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
	PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);			
	return true;
}