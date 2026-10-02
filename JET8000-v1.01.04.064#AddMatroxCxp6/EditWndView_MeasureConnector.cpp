// EditWndView_MeasurementMeasureConnector.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditWndView.h"
//-------------------------------------------------------------------------------------//
#include "ConnectorPinSettingsWnd.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_MeasureConnector(CJETPropertyGridProperty * pProp, TWND_PARAM_CHANGED_RESULT & Changed)
{
	if (NULL == pProp) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if (NULL == ModelPtr) { return true; }
	CAOIProject *Project = GetActiveProject();
	if (NULL == Project) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if (NULL == WndPtr) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	CString str;
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());
	switch (ParamID)
	{
	case WND_ALG_PROPERTY_MEASURE_CONNECTOR_BEGIN:
	case WND_ALG_PROPERTY_MEASURE_CONNECTOR_END:
		break;
	case WND_ALG_PROPERTY_MEASURE_CONNECTOR_IMPORT:
		{
			CConnectorPinSettingsWnd Wnd;
			Wnd.SetModelInfo(ModelPtr, WndPtr);
			Wnd.DoModal();
		}
		break;
	case WND_ALG_PROPERTY_MEASURE_CONNECTOR_RESULT:
		{
			CConnectorPinSettingsWnd Wnd;
			bool bShowResult = true;
			Wnd.SetModelInfo(ModelPtr, WndPtr, bShowResult);
			Wnd.DoModal();
		}
	break;
	}
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_MeasureConnector(CAOIProject * Project, CAOIWnd * WndPtr)
{
	if (NULL == WndPtr) { return false; }
	if (NULL == Project) { return false; }

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const TALG_PARAM_MEASURE_CONNECTOR &mcParam = AlgParam.GetAlgParamMeasureConnector();

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
	if (NULL == pGroupBasic) { return false; }
	pGroupBasic->SetID(WND_ALG_PROPERTY_MEASURE_CONNECTOR_BEGIN);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);
	if (BuildWndParamList_WndDefectID(pGroupBasic, Project, WndPtr) == false)
	{	return false;	}
	if (BuildWndParamList_AlgFrameIndex(pGroupBasic, Project, WndPtr) == false )
	{	return false;	}
	//增加主參數
	strCaption = _T("Import");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("");
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_CONNECTOR_IMPORT);
	pParamItem->SetHasUserBtn();
	pParamItem->AllowEdit(FALSE);
	pGroupBasic->AddSubItem(pParamItem);

	const bool bShowX=AlgParam.CheckAlgShowOffsetByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowY=AlgParam.CheckAlgShowOffsetByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowA=AlgParam.CheckAlgShowSkewByAlgType(AlgParam.GetAlgType());//false;
	const bool bShowS=AlgParam.CheckAlgShowScaleByAlgType(AlgParam.GetAlgType());//false;
	if ( BuildWndParamList_MatchOffset(pGroupBasic, Project, WndPtr, bShowX, bShowY, bShowA, bShowS) == false )
	{	return false; }	


	//結果文字顯示
	if (BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false)
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
bool CEditWndView::BuildWndParamList_MeasureConnector_Pin(CAOIProject * Project, CAOIWnd * WndPtr)
{
		if (NULL == WndPtr) { return false; }
	if (NULL == Project) { return false; }

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const TALG_PARAM_MEASURE_CONNECTOR &mcParam = AlgParam.GetAlgParamMeasureConnector();

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
	if (NULL == pGroupBasic) { return false; }
	pGroupBasic->SetID(WND_ALG_PROPERTY_MEASURE_CONNECTOR_BEGIN);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);
	if (BuildWndParamList_WndDefectID(pGroupBasic, Project, WndPtr) == false)
	{	return false;	}
	//增加主參數
	strCaption = _T("Result Table");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("");
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_CONNECTOR_RESULT);
	pParamItem->SetHasUserBtn();
	pParamItem->AllowEdit(FALSE);
	pGroupBasic->AddSubItem(pParamItem);


	//結果文字顯示
	if (BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false)
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