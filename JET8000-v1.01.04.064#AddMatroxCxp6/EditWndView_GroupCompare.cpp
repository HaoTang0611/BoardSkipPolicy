// EditWndView_GroupCompare.cpp : implementation file
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
bool CEditWndView::BuildWndParamList_GroupCompare(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }	
	
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	const TALG_PARAM_GROUP_COMPARE &gcParam = AlgParam.GetAlgParamGroupCompare();

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;
	
	CString str;
	bool    bIsPass = true;
	const BOOL bRedraw = FALSE;
	const BOOL bAdjustLayou = FALSE;
	CString strUnit, strReading;
	CString strCaption, strValue, strDescr;	
	CJETPropertyGridProperty* pParentItem = NULL;
	CJETPropertyGridProperty* pParamItem = NULL;
	CJETPropertyGridProperty* pGroupBasic = NULL;	
	WND_DEFECT_ID WndDefectID = WndPtr->GetWndDefectID();
	
	strCaption = AlgTypeText;
	strCaption = _T("Group Compare");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_GROUP_COMPARE_BEGIN);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);
	
	//增加Group Compare 參數
	bool bEnabled = false;	
	double dUSL = 0.0;
	double dLSL = 0.0;
	double dValue=0.0;
	double dReading=0.0;	
	ALG_GROUP_CMP_DIR_MODE   eCmpDirMode;
	ALG_3D_BASE_HEIGHT_MODE  e3DBaseMode;
	
	eCmpDirMode = gcParam.gcDirectionMode;
	strCaption = _T("Dir Mode");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue = AOIDataDefine.GetAlgGroupCompareDirText(eCmpDirMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_GROUP_COMPARE_DIR_MODE);	
	eCmpDirMode = ALG_GROUP_CMP_DIR_ANY;
	strValue = AOIDataDefine.GetAlgGroupCompareDirText(eCmpDirMode);	
	pParamItem->AddOption(strValue);
	eCmpDirMode = ALG_GROUP_CMP_DIR_ONE;
	strValue = AOIDataDefine.GetAlgGroupCompareDirText(eCmpDirMode);	
	pParamItem->AddOption(strValue);
	pParamItem->AllowEdit(FALSE);	
	pGroupBasic->AddSubItem(pParamItem);

	//2D Gray	
	/*
	bEnabled = AlgParamPtr->gc2DGrayEnabled;
	dUSL = AlgParamPtr->gc2DGrayUSL;
	dLSL = AlgParamPtr->gc2DGrayLSL;
	dValue = AlgParamPtr->gc2DGrayValue;
	dReading = AlgParamPtr->gc2DGrayReading;

	strCaption = _T("灰階比較");	
	pParentItem = new CJETPropertyGridProperty(strCaption);	
	if ( NULL == pParentItem ) { return false; }
	pParentItem->SetCheckValue(bEnabled);
	pParentItem->SetID(WND_ALG_PROPERTY_GROUP_COMPARE_2D_GRAY_ENABLED);	
	pParentItem->SetData((DWORD_PTR)WndPtr);		
	pGroupBasic->AddSubItem(pParentItem);

	strCaption = _T("灰階上限");
	strValue.Format(_T("%.0f"), dUSL);
	strReading.Format(_T("%.0f"), dReading);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_GROUP_COMPARE_2D_GRAY_USL);
	pParamItem->SetReading(strReading);		
	bIsPass = CAlgParam::CheckOK_GroupCompare2DGrayUSL(gcParam);		
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	//pGroupBasic->AddSubItem(pParamItem);
	pParentItem->AddSubItem(pParamItem);

	strCaption = _T("灰階下限");
	strValue.Format(_T("%.0f"), dLSL);
	strReading.Format(_T("%.0f"), dReading);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_GROUP_COMPARE_2D_GRAY_LSL);
	pParamItem->SetReading(strReading);		
	bIsPass = CAlgParam::CheckOK_GroupCompare2DGrayLSL(gcParam);		
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	//pGroupBasic->AddSubItem(pParamItem);
	pParentItem->AddSubItem(pParamItem);	
	pParentItem->Expand(FALSE);
	*/
	//*/
	//3D Hegiht
	bEnabled = gcParam.gc3DHeightEnabled;
	dUSL = gcParam.gc3DHeightUSL;
	dLSL = gcParam.gc3DHeightLSL;
	e3DBaseMode = gcParam.gc3DHeightBaseMode;	
	dValue = gcParam.gc3DHeightValue;	
	dReading = gcParam.gc3DHeightReading;
	
	strCaption = _T("Height Compare");	
	str = LoadMultiLanguageString(strCaption, strCaption);	
	strCaption.Format(_T("%s [%.0f]"), str, gcParam.gc3DHeightBase);
	strValue = AOIDataDefine.GetAlg3DHeightBaseText(e3DBaseMode);	
	pParentItem = new CJETPropertyGridProperty(strCaption);	
	if ( NULL == pParentItem ) { return false; }
	pParentItem->SetCheckValue(bEnabled);
	pParentItem->SetID(WND_ALG_PROPERTY_GROUP_COMPARE_3D_HEIGHT_ENABLED);		
	pParentItem->SetData((DWORD_PTR)WndPtr);	
	pGroupBasic->AddSubItem(pParentItem);	
	
	strCaption = _T("Base Mode");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);		
	strValue = AOIDataDefine.GetAlg3DHeightBaseText(e3DBaseMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_GROUP_COMPARE_3D_HEIGHT_BASE_MODE);	
	e3DBaseMode = ALG_3D_BASE_HEIGHT_MIN;//ALG_3D_BASE_HEIGHT_MAX
	strValue = AOIDataDefine.GetAlg3DHeightBaseText(e3DBaseMode);	
	pParamItem->AddOption(strValue);
	e3DBaseMode = ALG_3D_BASE_HEIGHT_AVE;//ALG_3D_BASE_HEIGHT_MID
	strValue = AOIDataDefine.GetAlg3DHeightBaseText(e3DBaseMode);	
	pParamItem->AddOption(strValue);
	e3DBaseMode = ALG_3D_BASE_HEIGHT_SQR;//ALG_3D_BASE_HEIGHT_SQR
	strValue = AOIDataDefine.GetAlg3DHeightBaseText(e3DBaseMode);
	pParamItem->AddOption(strValue);
	pParamItem->AllowEdit(FALSE);	
	pParentItem->AddSubItem(pParamItem);
	
	strCaption = _T("Height USL");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), dUSL);
	strReading.Format(_T("%.0f / %.0f"), dReading, dValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_GROUP_COMPARE_3D_HEIGHT_USL);
	pParamItem->SetReading(strReading);		
	bIsPass = CAlgParam::CheckOK_GroupCompare3DHeightUSL(gcParam);		
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	//pGroupBasic->AddSubItem(pParamItem);
	pParentItem->AddSubItem(pParamItem);

	strCaption = _T("Height LSL");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.0f"), dLSL);
	strReading.Format(_T("%.0f / %.0f"), dReading, dValue);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_GROUP_COMPARE_3D_HEIGHT_LSL);
	pParamItem->SetReading(strReading);		
	bIsPass = CAlgParam::CheckOK_GroupCompare3DHeightLSL(gcParam);		
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	//pGroupBasic->AddSubItem(pParamItem);
	pParentItem->AddSubItem(pParamItem);
	if ( false == bEnabled )
	{	pParentItem->Expand(FALSE); }

	//3D Angle
	bool bOpen3DAngle = true;
if ( true == bOpen3DAngle )
{
	strUnit.Format(_T("%c"), TCHAR(176));
	bEnabled = gcParam.gcTiltAngleEnabled;
	dUSL = gcParam.gcTiltAngleUSL;
	dLSL = gcParam.gcTiltAngleLSL;
	dValue = 0.0;
	dReading = gcParam.gcTiltAngleReading;
	
	strCaption = _T("Tilt Angle");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	pParentItem = new CJETPropertyGridProperty(strCaption);	
	if ( NULL == pParentItem ) { return false; }
	pParentItem->SetCheckValue(bEnabled);
	pParentItem->SetID(WND_ALG_PROPERTY_GROUP_COMPARE_TILT_ANGLE_ENABLED);	
	pParentItem->SetData((DWORD_PTR)WndPtr);	
	pGroupBasic->AddSubItem(pParentItem);

	strCaption = _T("Angle USL");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.2f"), dUSL);
	strReading.Format(_T("%.2f%s"), dReading, strUnit);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_GROUP_COMPARE_TILT_ANGLE_USL);
	pParamItem->SetReading(strReading);		
	bIsPass = CAlgParam::CheckOK_GroupCompareTiltAngleUSL(gcParam);		
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	//pGroupBasic->AddSubItem(pParamItem);
	pParentItem->AddSubItem(pParamItem);
	
	strCaption = _T("Angle LSL");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);	
	strValue.Format(_T("%.2f"), dLSL);
	strReading.Format(_T("%.2f%s"), dReading, strUnit);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_GROUP_COMPARE_TILT_ANGLE_LSL);
	pParamItem->SetReading(strReading);		
	bIsPass = CAlgParam::CheckOK_GroupCompareTiltAngleLSL(gcParam);		
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }	
	//pGroupBasic->AddSubItem(pParamItem);
	pParentItem->AddSubItem(pParamItem);
	
	if ( false == bEnabled ) 
	{	pParentItem->Expand(FALSE); }
}

	GridCtrl.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);
	bool bShow = CAlgParam::CheckAlgGroupCompareSupported(WndDefectID);
	pGroupBasic->Show(bShow);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_GroupCompare(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();		
	TALG_PARAM_GROUP_COMPARE &gcParam = AlgParam.GetAlgParamGroupCompare();

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
	CJETPropertyGridProperty *pProp2 = NULL;	
	WND_ALG_PROPERTY_ID ParamID = (WND_ALG_PROPERTY_ID)(pProp->GetID());	
	CJETPropertyGridCtrl &wndPropList = m_wndWndParam;

	switch ( ParamID )
	{
	case WND_ALG_PROPERTY_GROUP_COMPARE_DIR_MODE:
		strValue = pProp->GetValue();
		gcParam.gcDirectionMode = AOIDataDefine.FindAlgGroupCompareDirByText(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_GROUP_COMPARE_2D_GRAY_ENABLED:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		gcParam.gc2DGrayEnabled = bEnabled;
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_GROUP_COMPARE_2D_GRAY_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 255 ) { dValue = 255; }
		else if ( dValue < -255 )	{	dValue = -255; }
		if ( dValue < gcParam.gc2DGrayLSL ) 
		{	dValue = gcParam.gc2DGrayLSL;	}
		gcParam.gc2DGrayUSL = dValue;				
		bIsPass = CAlgParam::CheckOK_GroupCompare2DGrayUSL(gcParam);		
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;		
		break;
	case WND_ALG_PROPERTY_GROUP_COMPARE_2D_GRAY_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 255 ) { dValue = 255; }
		else if ( dValue < -255 )	{	dValue = -255; }
		if ( dValue > gcParam.gc2DGrayUSL ) 
		{	dValue = gcParam.gc2DGrayUSL;	}
		gcParam.gc2DGrayLSL = dValue;				
		bIsPass = CAlgParam::CheckOK_GroupCompare2DGrayLSL(gcParam);		
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_GROUP_COMPARE_3D_HEIGHT_ENABLED:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		gcParam.gc3DHeightEnabled = bEnabled;
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_GROUP_COMPARE_3D_HEIGHT_BASE_MODE:
		strValue = pProp->GetValue();
		gcParam.gc3DHeightBaseMode = AOIDataDefine.FindAlg3DHeightBaseModeByText(strValue);
		bChanged = true;
		break;	
	case WND_ALG_PROPERTY_GROUP_COMPARE_3D_HEIGHT_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 20000 ) { dValue = 20000; }
		else if ( dValue < -20000 )	{	dValue = -20000; }
		if ( dValue < gcParam.gc3DHeightLSL ) 
		{	dValue = gcParam.gc3DHeightLSL;	}
		gcParam.gc3DHeightUSL = dValue;				
		bIsPass = CAlgParam::CheckOK_GroupCompare3DHeightUSL(gcParam);		
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_GROUP_COMPARE_3D_HEIGHT_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 20000 ) { dValue = 20000; }
		else if ( dValue < -20000 )	{	dValue = -20000; }
		if ( dValue > gcParam.gc3DHeightUSL ) 
		{	dValue = gcParam.gc3DHeightUSL;	}
		gcParam.gc3DHeightLSL = dValue;				
		bIsPass = CAlgParam::CheckOK_GroupCompare3DHeightLSL(gcParam);		
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_GROUP_COMPARE_TILT_ANGLE_ENABLED:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		gcParam.gcTiltAngleEnabled = bEnabled;
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_GROUP_COMPARE_TILT_ANGLE_USL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 90 ) { dValue = 90; }
		else if ( dValue < -90 )	{	dValue = -90; }
		if ( dValue < gcParam.gcTiltAngleLSL ) 
		{	dValue = gcParam.gcTiltAngleLSL;	}
		gcParam.gcTiltAngleUSL = dValue;
		bIsPass = CAlgParam::CheckOK_GroupCompareTiltAngleUSL(gcParam);		
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_GROUP_COMPARE_TILT_ANGLE_LSL:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > 90 ) { dValue = 90; }
		else if ( dValue < -90 )	{	dValue = -90; }
		if ( dValue > gcParam.gcTiltAngleUSL ) 
		{	dValue = gcParam.gcTiltAngleUSL;	}
		gcParam.gcTiltAngleLSL = dValue;				
		bIsPass = CAlgParam::CheckOK_GroupCompareTiltAngleLSL(gcParam);		
		if ( false == bIsPass )
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

	Changed.sValueName.Format(_T("%s-%s"), _T("GroupCompaore"), strName);
	Changed.sValueOld = strOldValue;
	Changed.sValueNew = strNewValue;	

	Changed.bParamChanged = bChanged;	
	return true;
}
//-------------------------------------------------------------------------------------//