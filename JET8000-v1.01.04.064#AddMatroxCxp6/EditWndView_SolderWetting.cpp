// EditWndView_SolderWetting.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditWndView.h"
//-------------------------------------------------------------------------------------//
#include "AlgSolderWettingWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_SolderWetting(CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }	
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();	
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const WND_LOGIC_TYPE    WndLogicType = WndPtr->GetWndLogicType();	
	const TALG_PARAM_SOLDER_WETTING &swParam = AlgParam.GetAlgParamSolderWetting();	
	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;	
	const TPropGridParam &PropGridParam=GetPropGridParam();

	CString str;
	bool    bIsPass = true;
	BOOL    bEnabled=TRUE;	
	CString strCaption, strValue, strReading, strDescr, strOption;
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

	//增加Birght Ratio 參數
	double dUSL = 0;
	double dLSL = 0;
	double dReading = 0;
	double dRatio = 0.0;		
	
	//結果文字顯示
	if ( BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false )
	{	return false; }

	//Sector Angle
	str = _T("Circle Angle");
	strCaption = LoadMultiLanguageString(str, str);		
	bEnabled = swParam.swCircleAngleEnabled;	
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_ENABLED);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);		
	pGroupBasic->SetCheckValue(bEnabled);		
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);		

	dUSL = swParam.swCircleAngleUSL;
	dLSL = swParam.swCircleAngleLSL;	
	dReading = swParam.swReadingCircleAngle;	
	strReading.Format(_T("%.2f"), dReading);
	strCaption = _T("Angle USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_USL);	
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_SolderWettingSectorAngleUSL(swParam);		
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	//m_clrUnTest = 0x808080;
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("Angle LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_LSL);	
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_SolderWettingSectorAngleLSL(swParam);		
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	pGroupBasic->AddSubItem(pParamItem);
	
	strCaption = _T("Radius Start Ratio");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.1f"), swParam.swCircleAngleLineStartRatio);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_LINE_START_RATIO);
	strReading = AOIDataDefine.GetRatioText();	
	pParamItem->SetReading(strReading);		
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Radius Pass Ratio");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.1f"), swParam.swCircleAngleLinePassRatio);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_LINE_PASS_RATIO);
	strReading = AOIDataDefine.GetRatioText();	
	pParamItem->SetReading(strReading);		
	pGroupBasic->AddSubItem(pParamItem);

	//進階設定
	strCaption = _T("Setup");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("");
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_SOLDER_WETTING_SETUP_BTN);		
	pParamItem->SetHasUserBtn();
	pParamItem->AllowEdit(FALSE);
	pGroupBasic->AddSubItem(pParamItem);

	//m_clrUnTest = 0x808080;
	pGroupBasic->Expand(bEnabled);	

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
	pGroupAdvanced->Expand(FALSE);

	if ( FALSE == bAdjustLayou )
	{	wndPropList.AdjustLayout(); }
	wndPropList.SetRedraw(TRUE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamList_SolderWettingWnd(CAOIWnd *WndPtr)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	if ( NULL == WndPtr ) { return false; }	

	INT_PTR Ret=0;
	const bool bExtend = false;
	CAlgSolderWettingWnd SolderWettingWnd;
	std::vector<TUNI_FRAME> WndUniFrameList;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam& AlgBinParam = AlgParam.GetAlgImageBinParam();	
	TALG_PARAM_SOLDER_WETTING swParam= AlgParam.GetAlgParamSolderWetting();
	const unsigned int FrameIndex = AlgBinParam.GetBinaryFrameIndex();
	//if ( AOIDataCollect.CreateWndUniFrameListByField(bExtend, WndPtr, WndUniFrameList) == false ) 
	if ( AOIDataCollect.CreateWndUniFrameListByModel(bExtend, WndPtr, WndUniFrameList) == false )
	{	return false;	}

	SolderWettingWnd.SetWndPtr(WndPtr);	
	SolderWettingWnd.SetUniFrameList(FrameIndex, WndUniFrameList);
	Ret = SolderWettingWnd.DoModal();
	JetAPI::ClearUniFrameList(WndUniFrameList);
	if ( IDCANCEL == Ret ) 
	{	return true; }
	WndPtr->SetWndUIUpated_Param(false);
	SolderWettingWnd.GetSolderWettingParam(swParam);
	AlgParam.SetAlgParamSolderWetting(swParam);		

	TWND_PARAM_CHANGED_RESULT Changed;
	Changed.bParamChanged = true;
	ExecWndParamChangedUpdate(ModelPtr, WndPtr, Changed);	
	CWnd::PostMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_REBUILD_WND_PARAM_LIST, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_SolderWetting(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();
	TALG_PARAM_SOLDER_WETTING   &swParam  = AlgParam.GetAlgParamSolderWetting();

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
	case WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_ENABLED:
		bEnabled = pProp->GetCheckValue();		
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		swParam.swCircleAngleEnabled = bEnabled;
		pProp->Expand(bEnabled);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);
		dValue = MAX(0, dValue);
		dValue = MAX(dValue, swParam.swCircleAngleLSL);;
		swParam.swCircleAngleUSL = dValue;
		bIsPass = CAlgParam::CheckOK_SolderWettingSectorAngleUSL(swParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_LSL:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);
		dValue = MAX(0, dValue);
		dValue = MIN(dValue, swParam.swCircleAngleUSL);		
		swParam.swCircleAngleLSL = dValue;
		bIsPass = CAlgParam::CheckOK_SolderWettingSectorAngleLSL(swParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_LINE_START_RATIO:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);		
		dValue = MAX(0, dValue);		
		dValue = MIN(100, dValue);
		swParam.swCircleAngleLineStartRatio = dValue;		
		strValue.Format(_T("%.1f"), dValue);		
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_LINE_PASS_RATIO:
		strValue = pProp->GetValue();		
		dValue = ::_tcstod(strValue, NULL);		
		dValue = MAX(0, dValue);		
		dValue = MIN(100, dValue);
		swParam.swCircleAngleLinePassRatio = dValue;		
		strValue.Format(_T("%.1f"), dValue);		
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_SOLDER_WETTING_CIRCLE_ANGLE_READING:
		break;
	case WND_ALG_PROPERTY_SOLDER_WETTING_SETUP_BTN:
		if ( ExecWndParamList_SolderWettingWnd(WndPtr) == true ) 
		{
			bChanged = true;
			//bReBuildWndUI = true;
		}
		break;
	case WND_ALG_PROPERTY_SOLDER_WETTING_END:
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