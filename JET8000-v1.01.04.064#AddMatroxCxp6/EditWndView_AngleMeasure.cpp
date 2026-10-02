// EditWndView_AngleMeasure.cpp : implementation file
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
bool CEditWndView::BuildWndParamList_AngleMeasure(CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }	

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const TALG_PARAM_ANGLE_MEASURE &amParam = AlgParam.GetAlgParamAngleMeasure();

	CString str;
	CString strCaption, strValue, strDescr, strUnit, strOption;
	CString strReadingX, strReadingY, strReadingS, strResultID;
	CJETPropertyGridProperty* pParamItem = NULL;
	CJETPropertyGridProperty* pGroupBasic = NULL;
	CJETPropertyGridProperty* pGroupLength = NULL;
	CJETPropertyGridProperty* pGroupAdvanced = NULL;
	CJETPropertyGridProperty* pGroupResult = NULL;
	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;	

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

	//糤Angle Measure把计
	double dUSL = 0;
	double dLSL = 0;
	double dReading = 0;

	//à家Α
	strCaption = _T("Angle Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgAngleMeasureAngleModeText(amParam.amAngleMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_ANGLE_MEASURE_ANGLE_MODE);	
	strOption = AOIDataDefine.GetAlgAngleMeasureAngleModeText(ANGLE_MEASURE_SKEW);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgAngleMeasureAngleModeText(ANGLE_MEASURE_TILT);	pParamItem->AddOption(strOption);		
	pGroupBasic->AddSubItem(pParamItem);

	//膀非絬家Α
	strCaption = _T("Base Line");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgAngleMeasureLineEqnModeText(amParam.amBaseLineMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_ANGLE_MEASURE_BASE_LINE_MODE);	
	strOption = AOIDataDefine.GetAlgAngleMeasureLineEqnModeText(LINE_EQUATION_CALC);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgAngleMeasureLineEqnModeText(LINE_EQUATION_HOR);	pParamItem->AddOption(strOption);	
	strOption = AOIDataDefine.GetAlgAngleMeasureLineEqnModeText(LINE_EQUATION_VER);	pParamItem->AddOption(strOption);		      
	pGroupBasic->AddSubItem(pParamItem);
	pParamItem->Show(CheckShowAlgAngleMeasureBaseLineMode(amParam.amAngleMode));

	//à夹非-砏絛	
	strUnit = _T("Deg");
	strCaption = _T("Angle Spec.");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.2f"), amParam.amAngleSpec);
	strReadingS.Format(_T("%.2f"), amParam.amAngleReading);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_ANGLE_MEASURE_ANGLE_SPEC);	
	pParamItem->SetReading(strReadingS);
	pGroupBasic->AddSubItem(pParamItem);

	strUnit = _T("Deg");
	strCaption = _T("USL(Deg)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), amParam.amAngleTolUSL);
	strReadingS.Format(_T("%.2f"), amParam.amAngleSpec+amParam.amAngleTolUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_ANGLE_MEASURE_TOLERANCE_USL);		
	pParamItem->SetReading(strReadingS);	
	if ( CAlgParam::CheckOK_AngleMeasureTolUSL(amParam) )
	{	pParamItem->SetReadingTextColor(clrOK); }
	else
	{	pParamItem->SetReadingTextColor(clrNG); }
	pGroupBasic->AddSubItem(pParamItem);

	strUnit = _T("Deg");
	strCaption = _T("LSL(Deg)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), amParam.amAngleTolLSL);
	strReadingS.Format(_T("%.2f"), amParam.amAngleSpec+amParam.amAngleTolLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_ANGLE_MEASURE_TOLERANCE_LSL);	
	pParamItem->SetReading(strReadingS);	
	if ( CAlgParam::CheckOK_AngleMeasureTolLSL(amParam) )
	{	pParamItem->SetReadingTextColor(clrOK); }
	else
	{	pParamItem->SetReadingTextColor(clrNG); }
	pGroupBasic->AddSubItem(pParamItem);

	//挡狦ゅ陪ボ
	if ( BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false )
	{	return false; }
	
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
bool CEditWndView::ExecWndParamListChanged_AngleMeasure(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();
	TALG_PARAM_ANGLE_MEASURE   &amParam  = AlgParam.GetAlgParamAngleMeasure();

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	bool          bChanged = false;	
	bool          bBoolParam = false;
	int           nValue=0, nReading=0;
	bool          bValue=false, bReading=false, bEnabled=true;
	BOOL          BValue=FALSE, BReading=FALSE;
	double        dValue=0.0, dReading=0.0;	
	CString       strValue, strReading;
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
	case WND_ALG_PROPERTY_ANGLE_MEASURE_ANGLE_MODE:
		strValue = pProp->GetValue();
		amParam.amAngleMode = AOIDataDefine.FindAlgAngleMeasureAngleModeByText(strValue);		
		bChanged = true;

		pProp2 =  m_wndGroupList.FindItemByID(WND_ALG_PROPERTY_ANGLE_MEASURE_BASE_LINE_MODE);
		if ( NULL != pProp2 )
		{	pProp2->Show(CheckShowAlgAngleMeasureBaseLineMode(amParam.amAngleMode));	}
		break;		
	case WND_ALG_PROPERTY_ANGLE_MEASURE_BASE_LINE_MODE:
		strValue = pProp->GetValue();
		amParam.amBaseLineMode = AOIDataDefine.FindAlgAngleMeasureLineEqnModeByText(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_ANGLE_MEASURE_ANGLE_SPEC:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )	{	dValue = 0.0; }		
		amParam.amAngleSpec = dValue;
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_ANGLE_MEASURE_TOLERANCE_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);
		dValue = MAX(dValue, 0);		
		amParam.amAngleTolUSL = dValue;
		strValue.Format(_T("%.2f"), dValue);
		strReading.Format(_T("%.2f"), amParam.amAngleSpec+dValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		pProp->SetReading(strReading);
		if ( CAlgParam::CheckOK_AngleMeasureTolUSL(amParam) )
		{	pProp->SetReadingTextColor(clrOK); }
		else
		{	pProp->SetReadingTextColor(clrNG); }
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_ANGLE_MEASURE_TOLERANCE_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);
		dValue = MIN(dValue, 0);		
		amParam.amAngleTolLSL = dValue;
		strValue.Format(_T("%.2f"), dValue);
		strReading.Format(_T("%.2f"), amParam.amAngleSpec+dValue);
		pProp->SetValue(strValue);		
		pProp->SetOriginalValue(strValue);
		pProp->SetReading(strReading);
		if ( CAlgParam::CheckOK_AngleMeasureTolLSL(amParam) )
		{	pProp->SetReadingTextColor(clrOK); }
		else
		{	pProp->SetReadingTextColor(clrNG); }			
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