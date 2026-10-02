// EditWndView_ShapeVerify.cpp : implementation file
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
bool CEditWndView::BuildWndParamList_ShapeVerify(CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }	

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const TALG_PARAM_SHAPE_VERIFY &svParam = AlgParam.GetAlgParamShapeVerify();

	CString str;
	CString strCaption, strValue, strDescr, strUnit, strOption;
	CString strReadingX, strReadingY, strReadingS, strResultID;
	CJETPropertyGridProperty* pParamItem = NULL;
	CJETPropertyGridProperty* pGroupBasic = NULL;
	CJETPropertyGridProperty* pGroupEdge = NULL;
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
	//if ( BuildWndParamList_RegionLink(pGroupBasic, WndPtr, false) == false )
	//{	return false;	}

	//const bool bShowX=AlgParam.CheckAlgShowOffsetByAlgType(AlgParam.GetAlgType());//true;
	//const bool bShowY=AlgParam.CheckAlgShowOffsetByAlgType(AlgParam.GetAlgType());//true;
	//const bool bShowA=AlgParam.CheckAlgShowSkewByAlgType(AlgParam.GetAlgType());//false;
	//const bool bShowS=AlgParam.CheckAlgShowScaleByAlgType(AlgParam.GetAlgType());//false;
	//if ( BuildWndParamList_MatchOffset(pGroupBasic, Project, WndPtr, bShowX, bShowY, bShowA, bShowS) == false )
	//{	return false; }
	strUnit = AOIDataDefine.GetRatioText();
	strCaption = _T("Skip Inner");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), svParam.svSkipRatioInner);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }		
	pParamItem->SetID(WND_ALG_PROPERTY_SHAPE_VERIFY_SKIP_RATIO_INNER);
	pParamItem->SetReading(strUnit);
	pGroupBasic->AddSubItem(pParamItem);

	strUnit = AOIDataDefine.GetRatioText();
	strCaption = _T("Skip Outer");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), svParam.svSkipRatioOuter);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }		
	pParamItem->SetID(WND_ALG_PROPERTY_SHAPE_VERIFY_SKIP_RATIO_OUTER);
	pParamItem->SetReading(strUnit);
	pGroupBasic->AddSubItem(pParamItem);

	strUnit = _T("um");
	strCaption = _T("Radius");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), svParam.svCircleR);
	strReadingS.Format(_T("%.0f"), svParam.svReadingRAverage);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }		
	pParamItem->SetID(WND_ALG_PROPERTY_SHAPE_VERIFY_RESULT);	
	pParamItem->AllowEdit(FALSE);
	pParamItem->SetReading(strReadingS);	
	pGroupBasic->AddSubItem(pParamItem);

	strUnit = _T("um");
	strCaption = _T("Outer Tol.");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), svParam.svOuterTol);
	strReadingS.Format(_T("%.0f"), svParam.svReadingROuter-svParam.svCircleR);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_SHAPE_VERIFY_OUTER_TOL);	
	pParamItem->SetReading(strReadingS);
	if ( CAlgParam::CheckOK_ShapeVerifyOuterTol(svParam) )
	{	pParamItem->SetReadingTextColor(clrOK); }
	else
	{	pParamItem->SetReadingTextColor(clrNG); }
	pGroupBasic->AddSubItem(pParamItem);

	strUnit = _T("um");
	strCaption = _T("Inner Tol.");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), svParam.svInnerTol);
	strReadingS.Format(_T("%.0f"), svParam.svReadingRInner-svParam.svCircleR);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_SHAPE_VERIFY_INNER_TOL);	
	pParamItem->SetReading(strReadingS);
	if ( CAlgParam::CheckOK_ShapeVerifyInnerTol(svParam) )
	{	pParamItem->SetReadingTextColor(clrOK); }
	else
	{	pParamItem->SetReadingTextColor(clrNG); }
	pGroupBasic->AddSubItem(pParamItem);

	strUnit = _T("um");
	strCaption = _T("Range Tol.");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), svParam.svRangeTol);
	strReadingS.Format(_T("%.0f"), svParam.svReadingROuter-svParam.svReadingRInner);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_SHAPE_VERIFY_RANGE_TOL);	
	pParamItem->SetReading(strReadingS);
	if ( CAlgParam::CheckOK_ShapeVerifyRangeTol(svParam) )
	{	pParamItem->SetReadingTextColor(clrOK); }
	else
	{	pParamItem->SetReadingTextColor(clrNG); }
	pGroupBasic->AddSubItem(pParamItem);

	strUnit = _T("um");
	strCaption = _T("Error Tol.");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), svParam.svErrorTol);
	strReadingS.Format(_T("%.0f"), svParam.svReadingRAverage-svParam.svCircleR);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_SHAPE_VERIFY_ERROR_TOL);	
	pParamItem->SetReading(strReadingS);
	if ( CAlgParam::CheckOK_ShapeVerifyErrorTol(svParam) )
	{	pParamItem->SetReadingTextColor(clrOK); }
	else
	{	pParamItem->SetReadingTextColor(clrNG); }
	pGroupBasic->AddSubItem(pParamItem);

	//結果文字顯示
	if ( BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false )
	{	return false; }

	if ( BuildWndParamList_ExtendRange(wndPropList, Project, WndPtr) == false )
	{	return false;	}	

	//邏輯參數
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
bool CEditWndView::ExecWndParamListChanged_ShapeVerify(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();
	TALG_PARAM_SHAPE_VERIFY   &svParam  = AlgParam.GetAlgParamShapeVerify();

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
	case WND_ALG_PROPERTY_SHAPE_VERIFY_SKIP_RATIO_INNER:
		strValue = pProp->GetValue();		
		dValue = ::_ttof(strValue);
		if ( dValue < 0 ) 
		{ 
			dValue = 0; 
			strValue.Format(_T("%.2f"), dValue);
			pProp->SetValue(strValue);
		}
		svParam.svSkipRatioInner = dValue;
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_SHAPE_VERIFY_SKIP_RATIO_OUTER:
		strValue = pProp->GetValue();		
		dValue = ::_ttof(strValue);
		if ( dValue < 0 ) 
		{ 
			dValue = 0; 
			strValue.Format(_T("%.2f"), dValue);
			pProp->SetValue(strValue);
		}
		svParam.svSkipRatioOuter = dValue;
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_SHAPE_VERIFY_RESULT:
		break;
	case WND_ALG_PROPERTY_SHAPE_VERIFY_OUTER_TOL:
		strValue = pProp->GetValue();		
		dValue = ::_ttof(strValue);
		if ( dValue < 0 )
		{
			dValue = 0;
			strValue.Format(_T("%.0f"), dValue);
			pProp->SetValue(strValue);
		}
		svParam.svOuterTol = dValue;
		if ( CAlgParam::CheckOK_ShapeVerifyOuterTol(svParam) )
		{	pProp->SetReadingTextColor(clrOK); }
		else
		{	pProp->SetReadingTextColor(clrNG); }
		bChanged = true;
		break;			
	case WND_ALG_PROPERTY_SHAPE_VERIFY_INNER_TOL:
		strValue = pProp->GetValue();		
		dValue = ::_ttof(strValue);
		if ( dValue < 0 )
		{
			dValue = 0;
			strValue.Format(_T("%.0f"), dValue);
			pProp->SetValue(strValue);
		}
		svParam.svInnerTol = dValue;
		if ( CAlgParam::CheckOK_ShapeVerifyInnerTol(svParam) )
		{	pProp->SetReadingTextColor(clrOK); }
		else
		{	pProp->SetReadingTextColor(clrNG); }
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_SHAPE_VERIFY_RANGE_TOL:
		strValue = pProp->GetValue();		
		dValue = ::_ttof(strValue);
		if ( dValue < 0 )
		{
			dValue = 0;
			strValue.Format(_T("%.0f"), dValue);
			pProp->SetValue(strValue);
		}
		svParam.svRangeTol = dValue;
		if ( CAlgParam::CheckOK_ShapeVerifyRangeTol(svParam) )
		{	pProp->SetReadingTextColor(clrOK); }
		else
		{	pProp->SetReadingTextColor(clrNG); }
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_SHAPE_VERIFY_ERROR_TOL:
		strValue = pProp->GetValue();		
		dValue = ::_ttof(strValue);
		if ( dValue < 0 )
		{
			dValue = 0;
			strValue.Format(_T("%.0f"), dValue);
			pProp->SetValue(strValue);
		}
		svParam.svErrorTol = dValue;
		if ( CAlgParam::CheckOK_ShapeVerifyErrorTol(svParam) )
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