// EditWndView_ImageMatch.cpp : implementation file
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
bool CEditWndView::BuildWndParamList_ImageMatch(CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }	
	
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	TALG_PARAM_IMAGE_MATCH &imParam = AlgParam.GetAlgParamImageMatch();

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	CString strReading;
	CString strCaption, strValue, strDescr, strUnit;	
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
	
	//增加Image Match 參數
	const int    PatternPolarity = AlgParam.GetAlgPatternPolarity();

	strUnit = _T("um");

	strCaption = _T("Direction");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), PatternPolarity);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_IMAGE_MATCH_POLARITY);
	strValue = _T("1");	pParamItem->AddOption(strValue);
	strValue = _T("2");	pParamItem->AddOption(strValue);
	pGroupBasic->AddSubItem(pParamItem);	
	
	const bool bShowX=AlgParam.CheckAlgShowOffsetByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowY=AlgParam.CheckAlgShowOffsetByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowA=AlgParam.CheckAlgShowSkewByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowS=AlgParam.CheckAlgShowScaleByAlgType(AlgParam.GetAlgType());//true;
	if ( BuildWndParamList_MatchOffset(pGroupBasic, Project, WndPtr, bShowX, bShowY, bShowA, bShowS) == false )
	{	return false; }

	//結果文字顯示
	if ( BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false )
	{	return false; }

	if ( BuildWndParamList_ScaleRatio(wndPropList, Project, WndPtr) == false )
	{	return false;	}

	if ( BuildWndParamList_ExtendRange(wndPropList, Project, WndPtr) == false )
	{	return false;	}
	
	//像素比較	
	if ( BuildWndParamList_PixelCompare(wndPropList, Project, WndPtr) == false )
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

	if ( BuildWndParamList_AdvancePatMatch(pGroupAdvanced, Project, WndPtr) == false )	
	{	return false;	}
	pGroupAdvanced->Expand(FALSE);	

	if ( FALSE == bAdjustLayou )
	{	wndPropList.AdjustLayout(); }
	wndPropList.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_ImageMatch(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();		
	TALG_PARAM_IMAGE_MATCH   &imParam = AlgParam.GetAlgParamImageMatch();
	
	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	bool          bChanged = false;	
	bool          bBoolParam = false;
	bool          bReBuildWndUI = false;
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
	CJETPropertyGridProperty *pProp2 = NULL;	
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());	
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;

	switch ( ParamID )
	{	
	case WND_ALG_PROPERTY_IMAGE_MATCH_BEGIN:
		break;
	case WND_ALG_PROPERTY_IMAGE_MATCH_POLARITY:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);	
		AlgParam.SetAlgPatternPolarity(nValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_ENABLED:		
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		imParam.imPxlCmpEnabled = bEnabled;		
		nReading = imParam.imPxlCmpCountNum;
		if ( true == imParam.imPxlCmpEnabled )
		{
			if ( nReading>imParam.imPxlCmpCountUSL || nReading<imParam.imPxlCmpCountLSL )
			{	pProp->SetReadingTextColor(clrNG); }
			else
			{	pProp->SetReadingTextColor(clrOK); }
		}
		else
		{	pProp->SetReadingTextColor(clrUnTest);	}
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_DARK_LEVEL:
		strValue = pProp->GetValue();		
		nValue = ::_ttoi(strValue);
		if ( nValue < 0 ) { nValue = 0; }		
		else if ( nValue > 255 ) { nValue = 255; }
		imParam.imPxlCmpDarkLevel = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_TOLERANCE:
		strValue = pProp->GetValue();		
		nValue = ::_ttoi(strValue);
		if ( nValue < 0 ) { nValue = 0; }		
		else if ( nValue > 255 ) { nValue = 255; }
		imParam.imPxlCmpTolerance = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;		
	case WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_GAUSSIAN_SIZE:
		strValue = pProp->GetValue();		
		nValue = ::_ttoi(strValue);
		if ( nValue <= 0 ) { nValue = 0; }
		else { nValue = (nValue/2)*2+1; }
		imParam.imPxlCmpGaussianSize = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_OPEN_SIZE:
		strValue = pProp->GetValue();		
		nValue = ::_ttoi(strValue);
		if ( nValue <= 0 ) { nValue = 0; }
		else { nValue = (nValue/2)*2+1; }
		imParam.imPxlCmpOpenSize = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_CLOSE_SIZE:
		strValue = pProp->GetValue();		
		nValue = ::_ttoi(strValue);
		if ( nValue <= 0 ) { nValue = 0; }
		else { nValue = (nValue/2)*2+1; }
		imParam.imPxlCmpCloseSize = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_X_SIZE_MIN:
		strValue = pProp->GetValue();		
		dValue = ::_ttof(strValue);
		if ( dValue <= 0 ) { dValue = 0; }		
		imParam.imPxlCmpXSizeMin = dValue;
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_Y_SIZE_MIN:
		strValue = pProp->GetValue();		
		dValue = ::_ttof(strValue);
		if ( dValue <= 0 ) { dValue = 0; }		
		imParam.imPxlCmpYSizeMin = dValue;
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_AREA_MIN:
		strValue = pProp->GetValue();		
		dValue = ::_ttof(strValue);
		if ( dValue <= 0 ) { dValue = 0; }		
		imParam.imPxlCmpAreaMin = dValue;
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;		
	case WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_COUNT_USL:
		strValue = pProp->GetValue();		
		nValue = ::_ttoi(strValue);
		imParam.imPxlCmpCountUSL = nValue;		
		nReading = imParam.imPxlCmpCountNum;
		if ( true == imParam.imPxlCmpEnabled )
		{
			if ( nReading>imParam.imPxlCmpCountUSL || nReading<imParam.imPxlCmpCountLSL )
			{	pProp->SetReadingTextColor(clrNG); }
			else
			{	pProp->SetReadingTextColor(clrOK); }
		}
		else
		{	pProp->SetReadingTextColor(clrUnTest);	}		
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_IMAGE_MATCH_PXL_CMP_COUNT_LSL:
		strValue = pProp->GetValue();		
		nValue = ::_ttoi(strValue);
		imParam.imPxlCmpCountLSL = nValue;		
		nReading = imParam.imPxlCmpCountNum;
		if ( true == imParam.imPxlCmpEnabled )
		{
			if ( nReading>imParam.imPxlCmpCountUSL || nReading<imParam.imPxlCmpCountLSL )
			{	pProp->SetReadingTextColor(clrNG); }
			else
			{	pProp->SetReadingTextColor(clrOK); }
		}
		else
		{	pProp->SetReadingTextColor(clrUnTest);	}		
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_IMAGE_MATCH_END:
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