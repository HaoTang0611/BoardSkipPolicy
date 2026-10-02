// EditWndView_ColorCode.cpp : implementation file
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
bool CEditWndView::BuildWndParamList_ColorCode(CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }	
	
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const TALG_PARAM_COLOR_CODE &ccParam = AlgParam.GetAlgParamColorCode();

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	bool    bIsPass = true;
	CString strCaption, strValue, strDescr;
	CString strUnit, strReading;
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
	
	//增加Char Verify 參數	
	const double dCellSMax = ccParam.ccCellScoreMax;
	const double dCellSMin = ccParam.ccCellScoreMin;
	const double dSUSL = ccParam.ccPassRatioUSL;
	const double dSLSL = ccParam.ccPassRatioLSL;	
	const double dPassReading = ccParam.ccPassRatioReading;	
	strUnit = _T("um");
	strReading.Format(_T("%.0f"), dPassReading);

	strCaption = _T("Direction");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), ccParam.ccPolarityNum);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_COLOR_CODE_POLARITY);
	strValue = _T("1");
	pParamItem->AddOption(strValue);
	strValue = _T("2");
	pParamItem->AddOption(strValue);
	pGroupBasic->AddSubItem(pParamItem);

	strUnit = _T("%");	
	strCaption = _T("Cell LSL");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), dCellSMin);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_COLOR_CODE_CELL_SCORE_MIN);
	pParamItem->SetReading(strUnit);		
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Pass LSL");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), dSLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_COLOR_CODE_PASS_RATIO_LSL);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_ColorCodeLSL(ccParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);	

	//結果文字顯示
	if ( BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false )
	{	return false; }
	//
	/*
	WND_ALG_PROPERTY_COLOR_CODE_CELL_SCORE_MAX,		
	WND_ALG_PROPERTY_COLOR_CODE_PASS_RATIO_USL,	
	*/

	//子框參數
	if ( BuildWndParamList_WndRoi(wndPropList, Project, WndPtr) == false )
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
bool CEditWndView::ExecWndParamListChanged_ColorCode(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();		
	TALG_PARAM_COLOR_CODE   &ccParam = AlgParam.GetAlgParamColorCode();
	
	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	bool          bIsPass = true;
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
	case WND_ALG_PROPERTY_COLOR_CODE_POLARITY:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);	
		ccParam.ccPolarityNum = nValue;
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_COLOR_CODE_CELL_SCORE_MAX:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 100 ) { dValue = 100.0; }
		else if ( dValue < 0 )	{	dValue = 0.0; }
		if ( dValue < ccParam.ccCellScoreMin ) 
		{	dValue = ccParam.ccCellScoreMin;	}
		ccParam.ccCellScoreMax = dValue;		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_COLOR_CODE_CELL_SCORE_MIN:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 100 ) { dValue = 100.0; }
		else if ( dValue < 0 )	{	dValue = 0.0; }
		if ( dValue > ccParam.ccCellScoreMax ) 
		{	dValue = ccParam.ccCellScoreMax;	}
		ccParam.ccCellScoreMin = dValue;		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_COLOR_CODE_PASS_RATIO_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 100 ) { dValue = 100.0; }
		else if ( dValue < 0 )	{	dValue = 0.0; }
		if ( dValue < ccParam.ccPassRatioLSL ) 
		{	dValue = ccParam.ccPassRatioLSL;	}
		ccParam.ccPassRatioUSL = dValue;
		bIsPass = CAlgParam::CheckOK_ColorCodeUSL(ccParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_COLOR_CODE_PASS_RATIO_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 100 ) { dValue = 100.0; }
		else if ( dValue < 0 )	{	dValue = 0.0; }
		if ( dValue > ccParam.ccPassRatioUSL ) 
		{	dValue = ccParam.ccPassRatioUSL;	}
		ccParam.ccPassRatioLSL = dValue;				
		bIsPass = CAlgParam::CheckOK_ColorCodeLSL(ccParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
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