// EditWndView_BlobCount.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditWndView.h"
//-------------------------------------------------------------------------------------//
#include "JetBlob.h"
#include "AlgBlobCountWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
bool CEditWndView::BuildWndParamList_BlobCount(CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }

	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const TALG_PARAM_BLOB_COUNT &blobParam = AlgParam.GetAlgParamBlobCount();

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	bool  bIsPass = true;
	CString strCaption, strValue, strBlobCount, strDescr, strUnit;
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
	if ( BuildWndParamList_MaskFrameIndex(pGroupBasic, Project, WndPtr) == false )
	{	return false;	}
	
	//依照各種演算法外增加			
	double dValue=0;		
	BOOL   bEnabled = TRUE;
	const int BlobCountMax = blobParam.bcCountUSL;//區塊數量最大值
	const int BlobCountMin = blobParam.bcCountLSL;//區塊數量最小值
	const int BlobCount = blobParam.bcCountNum;//區塊數量
	strUnit = _T("um");
	strBlobCount.Format(_T("%d"), BlobCount);
	
	dValue = blobParam.bcXSizeMax;
	bEnabled = blobParam.bcXSizeMaxEnabled;	
	strCaption = _T("X Size USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_X_SIZE_MAX);		
	pParamItem->SetReading(strUnit);	
	pParamItem->SetCheckValue(bEnabled);
	pGroupBasic->AddSubItem(pParamItem);	

	dValue = blobParam.bcXSizeMin;
	bEnabled = blobParam.bcXSizeMinEnabled;
	strCaption = _T("X Size LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_X_SIZE_MIN);
	pParamItem->SetReading(strUnit);	
	pParamItem->SetCheckValue(bEnabled);
	pGroupBasic->AddSubItem(pParamItem);

	dValue = blobParam.bcYSizeMax;
	bEnabled = blobParam.bcYSizeMaxEnabled;
	strCaption = _T("Y Size USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_Y_SIZE_MAX);	
	pParamItem->SetReading(strUnit);	
	pParamItem->SetCheckValue(bEnabled);
	pGroupBasic->AddSubItem(pParamItem);	

	dValue = blobParam.bcYSizeMin;
	bEnabled = blobParam.bcYSizeMinEnabled;
	strCaption = _T("Y Size LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_Y_SIZE_MIN);	
	pParamItem->SetReading(strUnit);	
	pParamItem->SetCheckValue(bEnabled);
	pGroupBasic->AddSubItem(pParamItem);

	dValue = blobParam.bcLSizeMax;
	bEnabled = blobParam.bcLSizeMaxEnabled;
	strCaption = _T("L Size USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_L_SIZE_MAX);	
	pParamItem->SetReading(strUnit);	
	pParamItem->SetCheckValue(bEnabled);
	pGroupBasic->AddSubItem(pParamItem);	

	dValue = blobParam.bcLSizeMin;
	bEnabled = blobParam.bcLSizeMinEnabled;
	strCaption = _T("L Size LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_L_SIZE_MIN);	
	pParamItem->SetReading(strUnit);	
	pParamItem->SetCheckValue(bEnabled);
	pGroupBasic->AddSubItem(pParamItem);	

	dValue = blobParam.bcAreaSizeMax;
	bEnabled = blobParam.bcAreaSizeMaxEnabled;
	strUnit = _T("um^2");
	strCaption = _T("Area USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_AREA_SIZE_MAX);	
	pParamItem->SetReading(strUnit);
	pParamItem->SetCheckValue(bEnabled);
	pGroupBasic->AddSubItem(pParamItem);	

	dValue = blobParam.bcAreaSizeMin;
	bEnabled = blobParam.bcAreaSizeMinEnabled;
	strCaption = _T("Area LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), dValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_AREA_SIZE_MIN);	
	pParamItem->SetReading(strUnit);
	pParamItem->SetCheckValue(bEnabled);
	pGroupBasic->AddSubItem(pParamItem);	
	
	//Aspect
	dValue = blobParam.bcAspectRatioMax;
	bEnabled = blobParam.bcAspectRatioMaxEnabled;
	strUnit = _T("%");
	strCaption = _T("Aspect USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_ASPECT_RATIO_MAX);	
	pParamItem->SetReading(strUnit);
	pParamItem->SetCheckValue(bEnabled);
	pGroupBasic->AddSubItem(pParamItem);	

	dValue = blobParam.bcAspectRatioMin;
	bEnabled = blobParam.bcAspectRatioMinEnabled;
	strCaption = _T("Aspect LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_ASPECT_RATIO_MIN);	
	pParamItem->SetReading(strUnit);
	pParamItem->SetCheckValue(bEnabled);
	pGroupBasic->AddSubItem(pParamItem);	
	
	//Fill
	dValue = blobParam.bcFillRatioMax;
	bEnabled = blobParam.bcFillRatioMaxEnabled;
	strUnit = _T("%");
	strCaption = _T("Fill USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_FILL_RATIO_MAX);	
	pParamItem->SetReading(strUnit);
	pParamItem->SetCheckValue(bEnabled);
	pGroupBasic->AddSubItem(pParamItem);	

	dValue = blobParam.bcFillRatioMin;
	bEnabled = blobParam.bcFillRatioMinEnabled;
	strCaption = _T("Fill LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_FILL_RATIO_MIN);	
	pParamItem->SetReading(strUnit);
	pParamItem->SetCheckValue(bEnabled);
	pGroupBasic->AddSubItem(pParamItem);

	//Long Short Ratio	
	dValue = blobParam.bcLongShortRatioMax;
	bEnabled = blobParam.bcLongShortRatioMaxEnabled;
	strUnit = _T("%");
	strCaption = _T("L/S USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_LONG_SHORT_RATIO_MAX);	
	pParamItem->SetReading(strUnit);
	pParamItem->SetCheckValue(bEnabled);
	pGroupBasic->AddSubItem(pParamItem);	

	dValue = blobParam.bcLongShortRatioMin;
	bEnabled = blobParam.bcLongShortRatioMinEnabled;
	strCaption = _T("L/S LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_LONG_SHORT_RATIO_MIN);	
	pParamItem->SetReading(strUnit);
	pParamItem->SetCheckValue(bEnabled);
	pGroupBasic->AddSubItem(pParamItem);	

	//Connectivity	
	strCaption = _T("Connectivity");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), blobParam.bcConnectivity);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	strValue.Format(_T("%d"), BLOB_CONNECTIVITY_4);	pParamItem->AddOption(strValue);
	strValue.Format(_T("%d"), BLOB_CONNECTIVITY_8);	pParamItem->AddOption(strValue);
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_CONNECTIVITY);
	pGroupBasic->AddSubItem(pParamItem);

	strUnit = _T("um");
	strCaption = _T("Box Extend X");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), blobParam.bcRoiBoxExtendX);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_ROI_SHRINK_X);
	pParamItem->SetReading(strUnit);		
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Box Extend Y");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.0f"), blobParam.bcRoiBoxExtendY);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_ROI_SHRINK_Y);
	pParamItem->SetReading(strUnit);		
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Count USL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), BlobCountMax);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_COUNT_USL);
	pParamItem->SetReading(strBlobCount);	
	bIsPass = CAlgParam::CheckOK_BlobCountUSL(blobParam);
	if ( false == bIsPass)
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Count LSL");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%d"), BlobCountMin);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_BLOB_COUNT_LSL);
	pParamItem->SetReading(strBlobCount);	
	bIsPass = CAlgParam::CheckOK_BlobCountLSL(blobParam);
	if ( false == bIsPass)
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	pGroupBasic->AddSubItem(pParamItem);	
	
	//結果文字顯示
	if ( BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false )
	{	return false; }

	//strCaption = _T("Detail Set");
	//strCaption = LoadMultiLanguageString(strCaption, strCaption);
	//strValue = _T("Exec");
	//strValue = LoadMultiLanguageString(strValue, strValue);	
	//pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	//if ( NULL == pParamItem ) { return false; }
	//pParamItem->SetID(WND_ALG_PROPERTY_BLOB_DETAIL_SET);	
	//pParamItem->SetHasUserBtn();
	//pGroupBasic->AddSubItem(pParamItem);

	if ( BuildWndParamList_ModelMaskFlag(wndPropList, Project, WndPtr) == false )
	{	return false;	}	

	//子框參數
	if ( BuildWndParamList_WndRoi(wndPropList, Project, WndPtr) == false )
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
	pGroupAdvanced->Expand(FALSE);

	if ( FALSE == bAdjustLayou )
	{	wndPropList.AdjustLayout(); }
	wndPropList.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamDetailSetting_BlobCount(CAOIWnd *WndPtr)
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

	CAlgBlobCountWnd Wnd;
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
bool CEditWndView::ExecWndParamListChanged_BlobCount(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();		
	TALG_PARAM_BLOB_COUNT  &blobParam = AlgParam.GetAlgParamBlobCount();
	
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
	case WND_ALG_PROPERTY_BLOB_X_SIZE_MAX:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);
		if ( bEnabled != blobParam.bcXSizeMaxEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		if ( true == blobParam.bcXSizeMinEnabled )
		{		
			if ( dValue < blobParam.bcXSizeMin ) 
			{	dValue = blobParam.bcXSizeMin;	}			
		}
		if ( dValue < 0 ) { dValue = 0; }
		blobParam.bcXSizeMax = dValue;
		blobParam.bcXSizeMaxEnabled = bEnabled;
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_X_SIZE_MIN:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( bEnabled != blobParam.bcXSizeMinEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		if ( true == blobParam.bcXSizeMaxEnabled )
		{
			if ( dValue > blobParam.bcXSizeMax ) 
			{	dValue = blobParam.bcXSizeMax;	}			
		}
		if ( dValue < 0 ) { dValue = 0; }
		blobParam.bcXSizeMin = dValue;
		blobParam.bcXSizeMinEnabled = bEnabled;
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_Y_SIZE_MAX:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( bEnabled != blobParam.bcYSizeMaxEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		if ( true == blobParam.bcYSizeMinEnabled )
		{
			if ( dValue < blobParam.bcYSizeMin ) 
			{	dValue = blobParam.bcYSizeMin;	}		
		}
		if ( dValue < 0 ) { dValue = 0; }
		blobParam.bcYSizeMax = dValue;
		blobParam.bcYSizeMaxEnabled = bEnabled;
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_Y_SIZE_MIN:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( bEnabled != blobParam.bcYSizeMinEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		if ( true == blobParam.bcYSizeMaxEnabled )
		{
			if ( dValue > blobParam.bcYSizeMax ) 
			{	dValue = blobParam.bcYSizeMax;	 }			
		}
		if ( dValue < 0 ) { dValue = 0; }
		blobParam.bcYSizeMin = dValue;
		blobParam.bcYSizeMinEnabled = bEnabled;
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_L_SIZE_MAX:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( bEnabled != blobParam.bcLSizeMaxEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		if ( true == blobParam.bcLSizeMinEnabled )
		{
			if ( dValue < blobParam.bcLSizeMin ) 
			{	dValue = blobParam.bcLSizeMin;	}		
		}
		if ( dValue < 0 ) { dValue = 0; }
		blobParam.bcLSizeMax = dValue;
		blobParam.bcLSizeMaxEnabled = bEnabled;
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_L_SIZE_MIN:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( bEnabled != blobParam.bcLSizeMinEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		if ( true == blobParam.bcLSizeMaxEnabled )
		{
			if ( dValue > blobParam.bcLSizeMax ) 
			{	dValue = blobParam.bcLSizeMax;	 }			
		}
		if ( dValue < 0 ) { dValue = 0; }
		blobParam.bcLSizeMin = dValue;
		blobParam.bcLSizeMinEnabled = bEnabled;
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_AREA_SIZE_MAX:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( bEnabled != blobParam.bcAreaSizeMaxEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		if ( true == blobParam.bcAreaSizeMinEnabled )
		{
			if ( dValue < blobParam.bcAreaSizeMin ) 
			{	dValue = blobParam.bcAreaSizeMin;	}			
		}
		if ( dValue < 0 ) { dValue = 0; }
		blobParam.bcAreaSizeMax = dValue;
		blobParam.bcAreaSizeMaxEnabled = bEnabled;
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_AREA_SIZE_MIN:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( bEnabled != blobParam.bcAreaSizeMinEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		if ( true == blobParam.bcAreaSizeMaxEnabled )
		{
			if ( dValue > blobParam.bcAreaSizeMax ) 
			{	dValue = blobParam.bcAreaSizeMax;	}		
		}
		if ( dValue < 0 ) { dValue = 0; }
		blobParam.bcAreaSizeMin = dValue;
		blobParam.bcAreaSizeMinEnabled = bEnabled;
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_ASPECT_RATIO_MAX:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);
		if ( bEnabled != blobParam.bcAspectRatioMaxEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		if ( true == blobParam.bcAspectRatioMinEnabled )
		{
			if ( dValue < blobParam.bcAspectRatioMin ) 
			{	dValue = blobParam.bcAspectRatioMin;	}			
		}
		if ( dValue < 0 ) { dValue = 0; }
		blobParam.bcAspectRatioMax = dValue;
		blobParam.bcAspectRatioMaxEnabled = bEnabled;
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_ASPECT_RATIO_MIN:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);	
		if ( bEnabled != blobParam.bcAspectRatioMinEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		if ( true == blobParam.bcAspectRatioMaxEnabled )
		{
			if ( dValue > blobParam.bcAspectRatioMax ) 
			{	dValue = blobParam.bcAspectRatioMax;	}		
		}
		if ( dValue < 0 ) { dValue = 0; }
		blobParam.bcAspectRatioMin = dValue;
		blobParam.bcAspectRatioMinEnabled = bEnabled;
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_FILL_RATIO_MAX:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);
		if ( bEnabled != blobParam.bcFillRatioMaxEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		if ( true == blobParam.bcFillRatioMinEnabled )
		{
			if ( dValue < blobParam.bcFillRatioMin ) 
			{	dValue = blobParam.bcFillRatioMin;	}			
		}
		if ( dValue < 0 ) { dValue = 0; }
		blobParam.bcFillRatioMax = dValue;
		blobParam.bcFillRatioMaxEnabled = bEnabled;
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);		
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_FILL_RATIO_MIN:
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);
		if ( bEnabled != blobParam.bcFillRatioMinEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		if ( true == blobParam.bcFillRatioMaxEnabled )
		{
			if ( dValue > blobParam.bcFillRatioMax ) 
			{	dValue = blobParam.bcFillRatioMax;	}		
		}
		if ( dValue < 0 ) { dValue = 0; }
		blobParam.bcFillRatioMin = dValue;
		blobParam.bcFillRatioMinEnabled = bEnabled;
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_LONG_SHORT_RATIO_MAX:	
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( bEnabled != blobParam.bcLongShortRatioMaxEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		if ( true == blobParam.bcLongShortRatioMinEnabled )
		{
			if ( dValue < blobParam.bcLongShortRatioMin ) 
			{	dValue = blobParam.bcLongShortRatioMin;	}			
		}
		if ( dValue < 0 ) { dValue = 0; }
		blobParam.bcLongShortRatioMax = dValue;
		blobParam.bcLongShortRatioMaxEnabled = bEnabled;
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_LONG_SHORT_RATIO_MIN:	
		strValue = pProp->GetValue();
		bEnabled = pProp->GetCheckValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( bEnabled != blobParam.bcLongShortRatioMinEnabled )
		{
			bBoolParam = true;
			strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
			strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		}
		if ( true == blobParam.bcLongShortRatioMaxEnabled )
		{
			if ( dValue > blobParam.bcLongShortRatioMax ) 
			{	dValue = blobParam.bcLongShortRatioMax;	}		
		}
		if ( dValue < 0 ) { dValue = 0; }
		blobParam.bcLongShortRatioMin = dValue;
		blobParam.bcLongShortRatioMinEnabled = bEnabled;
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_CONNECTIVITY:	
		strValue = pProp->GetValue();		
		nValue = ::_ttoi(strValue);
		blobParam.bcConnectivity = nValue;
		strValue.Format(_T("%d"), nValue);
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_ROI_SHRINK_X:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);				
		blobParam.bcRoiBoxExtendX = nValue;		
		strValue.Format(_T("%d"), nValue);		
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_ROI_SHRINK_Y:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);				
		blobParam.bcRoiBoxExtendY = nValue;
		strValue.Format(_T("%d"), nValue);		
		pProp->SetValue(strValue);
		pProp->SetOriginalValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_COUNT_USL:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);		
		if ( nValue < 0 )
		{	nValue = 0; }
		else if ( nValue < blobParam.bcCountLSL ) 
		{	nValue = blobParam.bcCountLSL;	}
		blobParam.bcCountUSL = nValue;						
		bIsPass = CAlgParam::CheckOK_BlobCountUSL(blobParam);
		if ( false == bIsPass)
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%d"), nValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_COUNT_LSL:
		strValue = pProp->GetValue();
		nValue = ::_ttoi(strValue);		
		if ( nValue < 0 )
		{	nValue = 0; }
		else if ( nValue > blobParam.bcCountUSL ) 
		{	nValue = blobParam.bcCountUSL;	}
		blobParam.bcCountLSL = nValue;				
		bIsPass = CAlgParam::CheckOK_BlobCountLSL(blobParam);
		if ( false == bIsPass)
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%d"), nValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_BLOB_DETAIL_SET:
		if ( TRUE == bClickBtn )
		{	bChanged = ExecWndParamDetailSetting_BlobCount(WndPtr);	}
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