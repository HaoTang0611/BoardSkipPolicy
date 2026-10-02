// EditWndView_BodyTilt.cpp : implementation file
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
bool CEditWndView::BuildWndParamList_BodyTilt(CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const TALG_PARAM_BODY_TILT &btParam = AlgParam.GetAlgParamBodyTilt();

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	CString strCaption, strValue, strReading, strDescr, strUnit;
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
	
	//依照各種演算法外增加
	double USL=0, LSL=0, Reading=0.0;
	const double SizeRatioX = btParam.btSizeRatioX;//Cell尺寸比例-X
	const double SizeRatioY = btParam.btSizeRatioY;//Cell尺寸比例-Y
	const ALG_TILE_CELL_MODE CellMode = btParam.btCellMode;//傾斜模式(水平, 垂直, 對腳, 四角, 四邊, 八點)
	const double TitlGapUSL = btParam.btTiltGapUSL;//差距上限
	const double TitlGapLSL = btParam.btTiltGapLSL;//差距下限
	const double TitlAngleUSL = btParam.btTiltAngleUSL;//角度上限
	const double TitlAngleLSL = btParam.btTiltAngleLSL;//角度上限
	
	strUnit = _T("%");	
	strCaption = _T("X Size Ratio");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), SizeRatioX);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_TILT_SIZE_RATIO_X);	
	pParamItem->SetReading(strUnit);	
	pGroupBasic->AddSubItem(pParamItem);	

	strUnit = _T("%");
	strCaption = _T("Y Size Ratio");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), SizeRatioY);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_TILT_SIZE_RATIO_Y);	
	pParamItem->SetReading(strUnit);	
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("Cell Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), CellMode);
	//pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	pParamItem = CreateGridPropertyTiltCellModeList(strCaption, CellMode, strDescr, WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_TILT_CELL_MODE);		
	pGroupBasic->AddSubItem(pParamItem);	

	USL = TitlGapUSL;
	LSL = TitlGapLSL;
	Reading = btParam.btTiltGapReading;
	strReading.Format(_T("%.0f"), Reading);
	strCaption = _T("Dif. USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), USL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_TILT_GAP_USL);
	pParamItem->SetReading(strReading);	
	if ( Reading > USL )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Dif. LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), LSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_TILT_GAP_LSL);
	pParamItem->SetReading(strReading);	
	if ( Reading < LSL )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	pGroupBasic->AddSubItem(pParamItem);

	USL = TitlAngleUSL;
	LSL = TitlAngleLSL;
	Reading = btParam.btTiltAngleReading;
	strReading.Format(_T("%.2f"), Reading);
	strCaption = _T("Angle USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), USL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_TILT_ANGLE_USL);
	pParamItem->SetReading(strReading);	
	if ( Reading > USL )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	pGroupBasic->AddSubItem(pParamItem);
	/*
	strCaption = _T("Angle LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), LSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_TILT_ANGLE_LSL);
	pParamItem->SetReading(strReading);	
	if ( Reading < LSL )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	pGroupBasic->AddSubItem(pParamItem);
	*/
	
	//結果文字顯示
	if ( BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false )
	{	return false; }

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
bool CEditWndView::ExecWndParamListChanged_BodyTilt(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();		
	TALG_PARAM_BODY_TILT  &btParam = AlgParam.GetAlgParamBodyTilt();	
	
	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

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
	CJETPropertyGridProperty *pProp2 = NULL;		
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());	
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;

	switch ( ParamID )
	{		
	case WND_ALG_PROPERTY_TILT_SIZE_RATIO_X:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 1 )
		{	dValue = 1; }		
		btParam.btSizeRatioX = dValue;		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_TILT_SIZE_RATIO_Y:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 1 )
		{	dValue = 1; }		
		btParam.btSizeRatioY = dValue;		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;		
		break;
	case WND_ALG_PROPERTY_TILT_CELL_MODE:
		strValue = pProp->GetValue();		
		btParam.btCellMode = CAlgParam::GetAlgTiltCellModeByText(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_TILT_GAP_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )
		{	dValue = 0; }
		else if ( dValue < btParam.btTiltGapLSL ) 
		{	dValue = btParam.btTiltGapLSL;	}
		btParam.btTiltGapUSL = dValue;				
		dReading = btParam.btTiltGapReading;
		if ( dReading > dValue )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.0f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_TILT_GAP_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )
		{	dValue = 0; }
		else if ( dValue > btParam.btTiltGapUSL ) 
		{	dValue = btParam.btTiltGapUSL;	}
		btParam.btTiltGapLSL = dValue;				
		dReading = btParam.btTiltGapReading;
		if ( dReading < dValue )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.0f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	
	case WND_ALG_PROPERTY_TILT_ANGLE_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )
		{	dValue = 0; }
		else if ( dValue < btParam.btTiltAngleLSL ) 
		{	dValue = btParam.btTiltAngleLSL;	}
		btParam.btTiltAngleUSL = dValue;				
		dReading = btParam.btTiltAngleReading;
		if ( dReading > dValue )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_TILT_ANGLE_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 0 )
		{	dValue = 0; }
		else if ( dValue > btParam.btTiltAngleUSL ) 
		{	dValue = btParam.btTiltAngleUSL;	}
		btParam.btTiltAngleLSL = dValue;				
		dReading = btParam.btTiltAngleReading;
		if ( dReading < dValue )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
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
