// EditWndView_AIModel.cpp : implementation file
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
bool CEditWndView::BuildWndParamList_AIModel(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr)
{
	//if ( NULL == pCtrl ) { return false; }
	if ( NULL == WndPtr ) { return false; }	
	if ( NULL == Project ) { return false; }	
	//if ( NULL == pGroup ) { return false; }
	if ( WndPtr->CheckWndAlgAISupported() == false ) { return true; }

	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CString strCaption, strValue, strDescr, strUnit;
	CJETPropertyGridProperty *pGroup=NULL;
	CJETPropertyGridProperty *pParamItem = NULL;	
	const TPropGridParam &PropGridParam=GetPropGridParam();
	const TALG_PARAM_AI_MODEL &aiParam=WndPtr->GetWndAlgParam().GetAlgParamAiModel();

	strCaption = _T("AI Model");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pGroup = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroup ) { return false; }	
	pGroup->SetID(WND_ALG_PROPERTY_AIMODEL_BEGIN);	
	pGroup->SetData((DWORD_PTR)WndPtr);
	
	strCaption = _T("AI Model ID");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyAIModelIDList(strCaption, WndPtr, strDescr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_AIMODEL_AI_MODEL_ID);		
	pGroup->AddSubItem(pParamItem);

	strCaption = _T("Pattern Angle");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), aiParam.aiPatternAngle);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);
	if (NULL == pParamItem) { return false; }
	strValue.Format(_T("%.0f"), 000.0f);	pParamItem->AddOption(strValue);
	strValue.Format(_T("%.0f"), 090.0f);	pParamItem->AddOption(strValue);
	strValue.Format(_T("%.0f"), 180.0f);	pParamItem->AddOption(strValue);
	strValue.Format(_T("%.0f"), 270.0f);	pParamItem->AddOption(strValue);
	pParamItem->SetID(WND_ALG_PROPERTY_AIMODEL_PATTERN_ANGLE);
	pGroup->AddSubItem(pParamItem);

	strCaption = _T("Confidence Threshold");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), aiParam.aiConfidenceThreshold);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_AIMODEL_CONFIDENCE_THRESHOLD);	
	pGroup->AddSubItem(pParamItem);

	//字元相符數量閥值
	strCaption = _T("Char Match Num Threshold");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), aiParam.aiCharMatchNumThreshold);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_AIMODEL_CHAR_MATCH_NUM_THRESHOLD);	
	pGroup->AddSubItem(pParamItem);

	//字元數量上限閥值	
	strCaption = _T("Char Num Upper Threshold");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), aiParam.aiCharNumUpperThreshold);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_AIMODEL_CHAR_NUM_UPPER_THRESHOLD);	
	pGroup->AddSubItem(pParamItem);

	pGroup->Expand(PropGridParam.bGeneral_AIModelExpand);	
	GridCtrl.AddProperty(pGroup, bRedraw, bAdjustLayou);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_AIModel(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if (NULL == pProp) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if (NULL == ModelPtr) { return true; }
	CAOIProject *Project = GetActiveProject();
	if (NULL == Project) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if (NULL == WndPtr) { return false; }

	CAOILand        *LandPtr = WndPtr->GetWndLandPtr();
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();
	TALG_PARAM_AI_MODEL  &aiParam = AlgParam.GetAlgParamAiModel();

	bool          bIsPass = true;
	bool          bChanged = false;
	bool          bBoolParam = false;
	int           nValue = 0, nReading = 0;
	bool          bValue = false, bReading = false, bEnabled = true;
	BOOL          BValue = FALSE, BReading = FALSE, bClickBtn = FALSE, BShow = TRUE;
	double        dValue = 0.0, dValue2 = 0.0, dReading = 0.0;
	CString       strValue, strValue2;
	CString       strName = pProp->GetName();
	CString       strDescr = pProp->GetDescription();
	COleVariant   vtValue = pProp->GetValue();
	CString       strNewValue(pProp->GetValue());
	CString       strOldValue(pProp->GetOriginalValue());
	DWORD_PTR     dwData = pProp->GetData();
	CJETPropertyGridProperty *pProp2 = NULL;
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;	
	bClickBtn = pProp->GetClickUserBtn();

	switch (ParamID)
	{
	case WND_ALG_PROPERTY_AIMODEL_BEGIN:
		break;
	case WND_ALG_PROPERTY_AIMODEL_AI_MODEL_ID:
		strValue = pProp->GetValue();
		aiParam.aiModelID = AOIDataDefine.FindAIModelIDByText(strValue);		
		bChanged = true;		
		break;
	case WND_ALG_PROPERTY_AIMODEL_PATTERN_ANGLE:
		strValue = pProp->GetValue();
		dValue = JetAPI::StrToDbl(strValue);
		dValue = JetAPI::AdjustRotationAngle(dValue);
		aiParam.aiPatternAngle = (float)(dValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_AIMODEL_CONFIDENCE_THRESHOLD:
		strValue = pProp->GetValue();
		dValue = JetAPI::StrToDbl(strValue);
		dValue = MIN(dValue, 100);
		dValue = MAX(dValue, 0.0);
		aiParam.aiConfidenceThreshold = (float)(dValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_AIMODEL_CHAR_MATCH_NUM_THRESHOLD:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if ( nValue >= 0 )
		{
			aiParam.aiCharMatchNumThreshold = nValue;
			bChanged = true;
		}
		break;		
	case WND_ALG_PROPERTY_AIMODEL_CHAR_NUM_UPPER_THRESHOLD:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);
		if ( nValue >= 0 )
		{
			aiParam.aiCharNumUpperThreshold = MAX(nValue, aiParam.aiCharMatchNumThreshold);
			if ( aiParam.aiCharNumUpperThreshold != nValue )
			{
				strValue.Format(_T("%d"), aiParam.aiCharNumUpperThreshold);
				pProp->SetValue(strValue);
			}
			bChanged = true;
		}
		break;
	case WND_ALG_PROPERTY_AIMODEL_END:
		break;
	}

	if (false == bBoolParam)
	{	strNewValue = pProp->GetValue();	}
	Changed.sValueName = strName;
	Changed.sValueOld = strOldValue;
	Changed.sValueNew = strNewValue;

	Changed.bParamChanged = bChanged;
	return true;
}
//-------------------------------------------------------------------------------------//