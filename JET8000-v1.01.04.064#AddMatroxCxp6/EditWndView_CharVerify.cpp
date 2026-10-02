// EditWndView_CharVerify.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditWndView.h"
//-------------------------------------------------------------------------------------//
#include "AlgCharVerifyWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_CharVerify(CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }	
	
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const int    PatternPolarity = AlgParam.GetAlgPatternPolarity();
	const TALG_PARAM_CHAR_VERIFY &cvParam = AlgParam.GetAlgParamCharVerify();

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

	//糤Char Verify 把计	
	const int nCellGridCnt = cvParam.cvCellGridCnt;
	const int nCellExtSize = cvParam.cvCellExtSize;
	const double dCellSMax = cvParam.cvCellScoreMax;
	const double dCellSMin = cvParam.cvCellScoreMin;
	const double dSUSL = cvParam.cvPassRatioUSL;
	const double dSLSL = cvParam.cvPassRatioLSL;	
	const double dPassReading = cvParam.cvPassRatioReading;	

	const bool bShowX=AlgParam.CheckAlgShowOffsetByAlgType(AlgParam.GetAlgType());//false;
	const bool bShowY=AlgParam.CheckAlgShowOffsetByAlgType(AlgParam.GetAlgType());//false;
	const bool bShowA=AlgParam.CheckAlgShowSkewByAlgType(AlgParam.GetAlgType());//true;
	const bool bShowS=AlgParam.CheckAlgShowScaleByAlgType(AlgParam.GetAlgType());//false;
	if ( BuildWndParamList_MatchOffset(pGroupBasic, Project, WndPtr, bShowX, bShowY, bShowA, bShowS) == false )
	{	return false; }

	strUnit = _T("um");
	strReading.Format(_T("%.0f"), dPassReading);

	strCaption = _T("Direction");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), PatternPolarity);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_CHAR_VERIFY_POLARITY);
	strValue = _T("1");	pParamItem->AddOption(strValue);
	strValue = _T("2");	pParamItem->AddOption(strValue);
	pGroupBasic->AddSubItem(pParamItem);

	//
	strCaption = _T("Cell Level");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), nCellGridCnt);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_CHAR_VERIFY_CELL_LEVEL);
	strValue = _T("1");	pParamItem->AddOption(strValue);
	strValue = _T("2");	pParamItem->AddOption(strValue);
	strValue = _T("3");	pParamItem->AddOption(strValue);
	strValue = _T("4");	pParamItem->AddOption(strValue);
	strValue = _T("5");	pParamItem->AddOption(strValue);
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Cell Ext Size");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%d"), nCellExtSize);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_CHAR_VERIFY_CELL_EXT_SIZE);
	pParamItem->SetReading(AOIDataDefine.GetPixelText());		
	pGroupBasic->AddSubItem(pParamItem);	

	strUnit = _T("%");	
	strCaption = _T("Cell LSL");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), dCellSMin);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_CHAR_VERIFY_CELL_SCORE_MIN);
	pParamItem->SetReading(strUnit);		
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Pass LSL");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), dSLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_CHAR_VERIFY_PASS_RATIO_LSL);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_CharVerifyLSL(cvParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	pGroupBasic->AddSubItem(pParamItem);

	//挡狦ゅ陪ボ
	if ( BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false )
	{	return false; }
	/*		
	WND_ALG_PROPERTY_CHAR_VERIFY_CELL_SCORE_MAX,
	WND_ALG_PROPERTY_CHAR_VERIFY_PASS_RATIO_USL,
	*/	
	if ( BuildWndParamList_ExtendRange(wndPropList, Project, WndPtr) == false )
	{	return false;	}

	if ( BuildWndParamList_AIModel(wndPropList, Project, WndPtr) == false )
	{	return false;	}

	//把计
	if ( BuildWndParamList_WndRoi(wndPropList, Project, WndPtr) == false )
	{	return false;	}

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

	if ( BuildWndParamList_AdvancePatMatch(pGroupAdvanced, Project, WndPtr) == false )	
	{	return false;	}
	pGroupAdvanced->Expand(FALSE);

	if ( FALSE == bAdjustLayou )
	{	wndPropList.AdjustLayout(); }
	wndPropList.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_CharVerify(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();		
	TALG_PARAM_CHAR_VERIFY   &cvParam = AlgParam.GetAlgParamCharVerify();
	
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
	case WND_ALG_PROPERTY_CHAR_VERIFY_POLARITY:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);	
		AlgParam.SetAlgPatternPolarity(nValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_CHAR_VERIFY_CELL_LEVEL:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);	
		if ( nValue > 0 )
		{
			cvParam.cvCellGridCnt = nValue;
			bChanged = true;
		}
		break;
	case WND_ALG_PROPERTY_CHAR_VERIFY_CELL_EXT_SIZE:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);	
		if ( nValue < 0 ) 
		{	nValue = 0; }
		cvParam.cvCellExtSize = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;		
	case WND_ALG_PROPERTY_CHAR_VERIFY_CELL_SCORE_MAX:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 100 ) { dValue = 100.0; }
		else if ( dValue < 0 )	{	dValue = 0.0; }
		if ( dValue < cvParam.cvCellScoreMin ) 
		{	dValue = cvParam.cvCellScoreMin;	}
		cvParam.cvCellScoreMax = dValue;		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_CHAR_VERIFY_CELL_SCORE_MIN:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 100 ) { dValue = 100.0; }
		else if ( dValue < 0 )	{	dValue = 0.0; }
		if ( dValue > cvParam.cvCellScoreMax ) 
		{	dValue = cvParam.cvCellScoreMax;	}
		cvParam.cvCellScoreMin = dValue;		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_CHAR_VERIFY_PASS_RATIO_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 100 ) { dValue = 100.0; }
		else if ( dValue < 0 )	{	dValue = 0.0; }
		if ( dValue < cvParam.cvPassRatioLSL ) 
		{	dValue = cvParam.cvPassRatioLSL;	}
		cvParam.cvPassRatioUSL = dValue;
		bIsPass = CAlgParam::CheckOK_CharVerifyUSL(cvParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_CHAR_VERIFY_PASS_RATIO_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 100 ) { dValue = 100.0; }
		else if ( dValue < 0 )	{	dValue = 0.0; }
		if ( dValue > cvParam.cvPassRatioUSL ) 
		{	dValue = cvParam.cvPassRatioUSL;	}
		cvParam.cvPassRatioLSL = dValue;				
		bIsPass = CAlgParam::CheckOK_CharVerifyLSL(cvParam);
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
bool CEditWndView::ExecWndParamDetailSetting_CharVerify(CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	bool bExtend = false;
	std::vector<TUNI_FRAME> UniFrameList;	
	if ( AOIDataCollect.CreateWndUniFrameListByField(bExtend, WndPtr, UniFrameList) == false )	
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return false; 
	}	

	size_t                  i=0;
	TUNI_FRAME              UniFrame;
	CAlgParam              &AlgParam = WndPtr->GetWndAlgParam();		
	TALG_PARAM_BLOB_COUNT  &blobParam = AlgParam.GetAlgParamBlobCount();
	unsigned int FrameIndex = AlgParam.GetAlgImageBinParamPtr()->GetBinaryFrameIndex();
	const size_t UniFrameCount = UniFrameList.size();	
	if ( FrameIndex<0 || FrameIndex>=UniFrameCount ) { return false; }
	UniFrame = UniFrameList[FrameIndex];

	CAlgCharVerifyWnd Wnd;
	Wnd.SetWndPtr(WndPtr);
	Wnd.SetWndUniFrameList(UniFrameList);
	if ( Wnd.DoModal() == IDCANCEL ) 
	{	
		JetAPI::ClearUniFrameList(UniFrameList);
		return false; 
	}
	JetAPI::ClearUniFrameList(UniFrameList);
	return true;
}
//-------------------------------------------------------------------------------------//