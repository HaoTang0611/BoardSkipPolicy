// EditWndView_ModelMatch.cpp : implementation file
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
bool CEditWndView::BuildWndParamList_ModelMatch(CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }	
	
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const TALG_PARAM_MODEL_MATCH &mmParam = AlgParam.GetAlgParamModelMatch();

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;
	
	CString strCaption, strValue, strDescr, strUnit;	
	CJETPropertyGridProperty* pParamItem = NULL;
	CJETPropertyGridProperty* pGroupBasic = NULL;
	CJETPropertyGridProperty* pGroupScale = NULL;
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
	if ( BuildWndParamList_RegionLink(pGroupBasic, WndPtr, true) == false )
	{	return false;	}

	//增加Model Match 參數	
	const ALG_MATCH_DOCK_MODE DockMode = mmParam.mmDockMode;
	const int    nRecheckBox = mmParam.mmReCheckBox;

	strUnit = _T("um");
	strCaption = _T("Dock Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyAlgMatchDockModeList(strCaption, DockMode, WndPtr, strDescr);
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_MODEL_MATCH_DOCK_MODE);	
	pGroupBasic->AddSubItem(pParamItem);
	
	const bool bRecheckBox = (bool)(nRecheckBox);
	strCaption = _T("Recheck Box");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = new CJETPropertyGridProperty(strCaption, (_variant_t)bRecheckBox, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }		
	pParamItem->SetID(WND_ALG_PROPERTY_MODEL_MATCH_RECHECK_BOX);	
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

	if (WndPtr->GetWndDefectID() == WND_DEFECT_LEAD_ADJUST) {
		if (BuildWndParamList_ROICompare(wndPropList, Project, WndPtr) == false)
		{
			return false;
		}
	}

	if ( BuildWndParamList_ExtendRange(wndPropList, Project, WndPtr) == false )
	{	return false;	}	
	
	if ( BuildWndParamList_ModelMaskFlag(wndPropList, Project, WndPtr) == false )
	{	return false;	}	
	
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
	if ( BuildWndParamList_AdvancePatMatch(pGroupAdvanced, Project, WndPtr) == false )	
	{	return false;	}	
	pGroupAdvanced->Expand(FALSE);	

	if (WndPtr->GetWndDefectID() == WND_DEFECT_PART_ALIGN)
	{	BuildWndParamList_ResinHight(Project, WndPtr); }
	else
	{
		if ( FALSE == bAdjustLayou )
		{	wndPropList.AdjustLayout(); }
		wndPropList.SetRedraw(TRUE);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_ModelMatch(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();		
	TALG_PARAM_MODEL_MATCH   &mmParam = AlgParam.GetAlgParamModelMatch();
	
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
	ALG_MATCH_DOCK_MODE DockMode;
	CJETPropertyGridProperty *pProp2 = NULL;		
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());	
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;

	switch ( ParamID )
	{
	case WND_ALG_PROPERTY_MODEL_MATCH_BEGIN:
		break;	
	case WND_ALG_PROPERTY_MODEL_MATCH_DOCK_MODE:
		strValue = pProp->GetValue();		
		DockMode = AOIDataDefine.FindAlgMatchDockModeByText(strValue);
		mmParam.mmDockMode = DockMode;
		bChanged = true;		
		break;	
	case WND_ALG_PROPERTY_MODEL_MATCH_RECHECK_BOX:
		bEnabled = (bool)(vtValue.boolVal);
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		if ( FALSE == vtValue.boolVal )
		{	mmParam.mmReCheckBox = FN_DISABLE; }
		else
		{	mmParam.mmReCheckBox = FN_ENABLE; }
		bChanged = true;
		break;		
	case WND_ALG_PROPERTY_MODEL_MATCH_END:
		break;
	default:
		break;	
	}

	if ( false == bBoolParam )
	{	strNewValue = pProp->GetValue();	}
	Changed.sValueName = strName;
	Changed.sValueOld = strOldValue;
	Changed.sValueNew = strNewValue;	

	Changed.bParamChanged = bChanged;	
	return true;
}
//-------------------------------------------------------------------------------------//