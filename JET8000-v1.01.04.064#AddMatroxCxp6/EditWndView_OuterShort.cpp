// EditWndView_OuterShort.cpp : implementation file
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
bool CEditWndView::BuildWndParamList_OuterShort(CAOIProject *Project, CAOIWnd *WndPtr)
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }	
	
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const TALG_PARAM_OUTER_SHORT &osParam = AlgParam.GetAlgParamOuterShort();	

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	bool   bIsPass = true;
	bool    bEnabled = false;
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
	if ( BuildWndParamList_RegionLink(pGroupBasic, WndPtr, true) == false )
	{	return false;	}

	//結果文字顯示
	if ( BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false )
	{	return false; }
	if ( BuildWndParamList_ExtendRange(wndPropList, Project, WndPtr) == false )
	{	return false;	}	
	
	//增加Outer Short 參數
	double dUSL = 0;
	double dLSL = 0;	
	double dRange = 0;
	int    nMode = 0;
	double dReading = 0;

	//朝右側
	dUSL = osParam.osLine_R.olUSL;
	dLSL = osParam.osLine_R.olLSL;	
	dReading = osParam.osLine_R.olReading;
	strReading.Format(_T("%.2f"), dReading);		
	
	strCaption = AOIDataDefine.GetBoxTowardText(BOX_TOWARD_RIGHT);
	strCaption = _T("Right Side");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	bEnabled = osParam.osLine_R.olEnabled;	
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_OUTER_SHORT_ENABLED_R);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);		
	pGroupBasic->SetCheckValue(bEnabled);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);

	strCaption = _T("Continue Range");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), osParam.osLine_R.olRange);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_RANGE_R);
	pParamItem->SetReading(_T("um"));		
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("Line Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgBrightLineModeText(osParam.osLine_R.olMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_MODE_R);	
	strOption = AOIDataDefine.GetAlgBrightLineModeText(LINE_MODE_BRIGHT);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgBrightLineModeText(LINE_MODE_DARK);	pParamItem->AddOption(strOption);	
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("Extend Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgOuterShortExtendModeText(osParam.osLine_R.olExtMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_EXT_MODE_R);	
	strOption = AOIDataDefine.GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_NONE);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_LEFT);	pParamItem->AddOption(strOption);	
	strOption = AOIDataDefine.GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_RIGHT);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_BOTH);	pParamItem->AddOption(strOption);	
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("USL (%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_USL_R);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_OuterShortUSL_R(osParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	//m_clrUnTest = 0x808080;
	pGroupBasic->AddSubItem(pParamItem);	
	
	strCaption = _T("LSL (%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_LSL_R);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_OuterShortLSL_R(osParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	//m_clrUnTest = 0x808080;
	pGroupBasic->AddSubItem(pParamItem);
	
	strCaption = _T("Apply To Others");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("Exec");
	strValue = LoadMultiLanguageString(strValue, strValue);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_APPLY_R);	
	pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);	
	if ( true == bEnabled )
	{	pGroupBasic->Expand(TRUE);	}
	else
	{	pGroupBasic->Expand(FALSE);	}	
	
	//朝上側
	dUSL = osParam.osLine_T.olUSL;
	dLSL = osParam.osLine_T.olLSL;	
	dReading = osParam.osLine_T.olReading;
	strReading.Format(_T("%.2f"), dReading);		
	
	strCaption = AOIDataDefine.GetBoxTowardText(BOX_TOWARD_UP);
	strCaption = _T("Top Side");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	bEnabled = osParam.osLine_T.olEnabled;
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_OUTER_SHORT_ENABLED_T);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);		
	pGroupBasic->SetCheckValue(bEnabled);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);

	strCaption = _T("Continue Range");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), osParam.osLine_T.olRange);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_RANGE_T);
	pParamItem->SetReading(_T("um"));		
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("Line Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgBrightLineModeText(osParam.osLine_T.olMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_MODE_T);	
	strOption = AOIDataDefine.GetAlgBrightLineModeText(LINE_MODE_BRIGHT);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgBrightLineModeText(LINE_MODE_DARK);	pParamItem->AddOption(strOption);	
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("Extend Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgOuterShortExtendModeText(osParam.osLine_T.olExtMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_EXT_MODE_T);	
	strOption = AOIDataDefine.GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_NONE);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_LEFT);	pParamItem->AddOption(strOption);	
	strOption = AOIDataDefine.GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_RIGHT);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_BOTH);	pParamItem->AddOption(strOption);	
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("USL (%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_USL_T);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_OuterShortUSL_T(osParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	//m_clrUnTest = 0x808080;
	pGroupBasic->AddSubItem(pParamItem);	
	
	strCaption = _T("LSL (%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_LSL_T);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_OuterShortLSL_T(osParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	//m_clrUnTest = 0x808080;
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Apply To Others");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("Exec");
	strValue = LoadMultiLanguageString(strValue, strValue);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_APPLY_T);	
	pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);
	if ( true == bEnabled )
	{	pGroupBasic->Expand(TRUE);	}
	else
	{	pGroupBasic->Expand(FALSE);	}

	//朝左側
	dUSL = osParam.osLine_L.olUSL;
	dLSL = osParam.osLine_L.olLSL;	
	dReading = osParam.osLine_L.olReading;
	strReading.Format(_T("%.2f"), dReading);		
	
	strCaption = AOIDataDefine.GetBoxTowardText(BOX_TOWARD_LEFT);
	strCaption = _T("Left Side");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	bEnabled = osParam.osLine_L.olEnabled;
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_OUTER_SHORT_ENABLED_L);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);		
	pGroupBasic->SetCheckValue(bEnabled);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);

	strCaption = _T("Continue Range");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), osParam.osLine_L.olRange);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_RANGE_L);
	pParamItem->SetReading(_T("um"));		
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("Line Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgBrightLineModeText(osParam.osLine_L.olMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_MODE_L);	
	strOption = AOIDataDefine.GetAlgBrightLineModeText(LINE_MODE_BRIGHT);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgBrightLineModeText(LINE_MODE_DARK);	pParamItem->AddOption(strOption);	
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("Extend Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgOuterShortExtendModeText(osParam.osLine_L.olExtMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_EXT_MODE_L);	
	strOption = AOIDataDefine.GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_NONE);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_LEFT);	pParamItem->AddOption(strOption);	
	strOption = AOIDataDefine.GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_RIGHT);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_BOTH);	pParamItem->AddOption(strOption);	
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("USL (%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_USL_L);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_OuterShortUSL_L(osParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	//m_clrUnTest = 0x808080;
	pGroupBasic->AddSubItem(pParamItem);	
	
	strCaption = _T("LSL (%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_LSL_L);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_OuterShortLSL_L(osParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	//m_clrUnTest = 0x808080;
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Apply To Others");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("Exec");
	strValue = LoadMultiLanguageString(strValue, strValue);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_APPLY_L);
	pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);
	if ( true == bEnabled )
	{	pGroupBasic->Expand(TRUE);	}
	else
	{	pGroupBasic->Expand(FALSE);	}
	
	//朝下側
	dUSL = osParam.osLine_B.olUSL;
	dLSL = osParam.osLine_B.olLSL;
	dReading = osParam.osLine_B.olReading;
	strReading.Format(_T("%.2f"), dReading);		
	
	strCaption = AOIDataDefine.GetBoxTowardText(BOX_TOWARD_DOWN);
	strCaption = _T("Bottom Side");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	bEnabled = osParam.osLine_B.olEnabled;
	pGroupBasic = new CJETPropertyGridProperty(strCaption);
	if ( NULL == pGroupBasic ) { return false; }	
	pGroupBasic->SetID(WND_ALG_PROPERTY_OUTER_SHORT_ENABLED_B);
	pGroupBasic->SetData((DWORD_PTR)WndPtr);		
	pGroupBasic->SetCheckValue(bEnabled);
	wndPropList.AddProperty(pGroupBasic, bRedraw, bAdjustLayou);

	strCaption = _T("Continue Range");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), osParam.osLine_B.olRange);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_RANGE_B);
	pParamItem->SetReading(_T("um"));		
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("Line Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgBrightLineModeText(osParam.osLine_B.olMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_MODE_B);	
	strOption = AOIDataDefine.GetAlgBrightLineModeText(LINE_MODE_BRIGHT);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgBrightLineModeText(LINE_MODE_DARK);	pParamItem->AddOption(strOption);	
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("Extend Mode");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = AOIDataDefine.GetAlgOuterShortExtendModeText(osParam.osLine_B.olExtMode);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);		
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_EXT_MODE_B);	
	strOption = AOIDataDefine.GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_NONE);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_LEFT);	pParamItem->AddOption(strOption);	
	strOption = AOIDataDefine.GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_RIGHT);	pParamItem->AddOption(strOption);
	strOption = AOIDataDefine.GetAlgOuterShortExtendModeText(ALG_OUTER_SHORT_EXT_BOTH);	pParamItem->AddOption(strOption);	
	pGroupBasic->AddSubItem(pParamItem);	

	strCaption = _T("USL (%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dUSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_USL_B);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_OuterShortUSL_B(osParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	//m_clrUnTest = 0x808080;
	pGroupBasic->AddSubItem(pParamItem);	
	
	strCaption = _T("LSL (%)");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue.Format(_T("%.2f"), dLSL);
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_LSL_B);
	pParamItem->SetReading(strReading);	
	bIsPass = CAlgParam::CheckOK_OuterShortLSL_B(osParam);
	if ( false == bIsPass )
	{	pParamItem->SetReadingTextColor(clrNG); }
	else
	{	pParamItem->SetReadingTextColor(clrOK); }
	//m_clrUnTest = 0x808080;
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Apply To Others");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("Exec");
	strValue = LoadMultiLanguageString(strValue, strValue);	
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_OUTER_SHORT_APPLY_B);	
	pParamItem->SetHasUserBtn();
	pGroupBasic->AddSubItem(pParamItem);
	if ( true == bEnabled )
	{	pGroupBasic->Expand(TRUE);	}
	else
	{	pGroupBasic->Expand(FALSE);	}

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
bool CEditWndView::ExecWndParamListChanged_OuterShort(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();		
	TALG_PARAM_OUTER_SHORT &osParam = AlgParam.GetAlgParamOuterShort();
	
	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	bool          bIsPass = true;
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
	case WND_ALG_PROPERTY_OUTER_SHORT_BEGIN:
		break;	
	case WND_ALG_PROPERTY_OUTER_SHORT_ENABLED_R:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		osParam.osLine_R.olEnabled = bEnabled;
		pProp->Expand(bEnabled);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_RANGE_R:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 1 ) 
		{	dValue = 1;	}		
		osParam.osLine_R.olRange = dValue;		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_MODE_R:
		strValue = pProp->GetValue();
		nValue = AOIDataDefine.FindAlgBrightLineModeByText(strValue);
		switch ( nValue ) 
		{
		case LINE_MODE_DARK:
		case LINE_MODE_BRIGHT:
			osParam.osLine_R.olMode = nValue;
			bChanged = true;
			break;
		}	
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_EXT_MODE_R:
		strValue = pProp->GetValue();
		nValue = AOIDataDefine.FindAlgOuterShortExtendModeByText(strValue);
		switch ( nValue ) 
		{
		case ALG_OUTER_SHORT_EXT_NONE:
		case ALG_OUTER_SHORT_EXT_LEFT:
		case ALG_OUTER_SHORT_EXT_RIGHT:
		case ALG_OUTER_SHORT_EXT_BOTH:
			osParam.osLine_R.olExtMode = (ALG_OUTER_SHORT_EXT_MODE)(nValue);
			bChanged = true;
			break;
		}
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_USL_R:		
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < osParam.osLine_R.olLSL ) 
		{	dValue = osParam.osLine_R.olLSL;	}		
		osParam.osLine_R.olUSL = dValue;
		bIsPass = CAlgParam::CheckOK_OuterShortUSL_R(osParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_LSL_R:		
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > osParam.osLine_R.olUSL ) 
		{	dValue = osParam.osLine_R.olUSL;	}
		osParam.osLine_R.olLSL = dValue;				
		bIsPass = CAlgParam::CheckOK_OuterShortLSL_R(osParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_APPLY_R:
		osParam.SetAll(osParam.osLine_R);
		bChanged = true;
		bReBuildWndUI = true;
		break;

	case WND_ALG_PROPERTY_OUTER_SHORT_ENABLED_T:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		osParam.osLine_T.olEnabled = bEnabled;
		pProp->Expand(bEnabled);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_RANGE_T:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 1 ) 
		{	dValue = 1;	}		
		osParam.osLine_T.olRange = dValue;		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_MODE_T:
		strValue = pProp->GetValue();
		nValue = AOIDataDefine.FindAlgBrightLineModeByText(strValue);
		switch ( nValue ) 
		{
		case LINE_MODE_DARK:
		case LINE_MODE_BRIGHT:
			osParam.osLine_T.olMode = nValue;
			bChanged = true;
			break;
		}	
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_EXT_MODE_T:
		strValue = pProp->GetValue();
		nValue = AOIDataDefine.FindAlgOuterShortExtendModeByText(strValue);
		switch ( nValue ) 
		{
		case ALG_OUTER_SHORT_EXT_NONE:
		case ALG_OUTER_SHORT_EXT_LEFT:
		case ALG_OUTER_SHORT_EXT_RIGHT:
		case ALG_OUTER_SHORT_EXT_BOTH:
			osParam.osLine_T.olExtMode = (ALG_OUTER_SHORT_EXT_MODE)(nValue);
			bChanged = true;
			break;
		}
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_USL_T:		
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < osParam.osLine_T.olLSL ) 
		{	dValue = osParam.osLine_T.olLSL;	}		
		osParam.osLine_T.olUSL = dValue;				
		bIsPass = CAlgParam::CheckOK_OuterShortUSL_T(osParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_LSL_T:		
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > osParam.osLine_T.olUSL ) 
		{	dValue = osParam.osLine_T.olUSL;	}
		osParam.osLine_T.olLSL = dValue;				
		bIsPass = CAlgParam::CheckOK_OuterShortLSL_T(osParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_APPLY_T:
		osParam.SetAll(osParam.osLine_T);
		bChanged = true;
		bReBuildWndUI = true;
		break;

	case WND_ALG_PROPERTY_OUTER_SHORT_ENABLED_L:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		osParam.osLine_L.olEnabled = bEnabled;
		pProp->Expand(bEnabled);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_RANGE_L:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 1 ) 
		{	dValue = 1;	}		
		osParam.osLine_L.olRange = dValue;		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_MODE_L:
		strValue = pProp->GetValue();
		nValue = AOIDataDefine.FindAlgBrightLineModeByText(strValue);
		switch ( nValue ) 
		{
		case LINE_MODE_DARK:
		case LINE_MODE_BRIGHT:
			osParam.osLine_L.olMode = nValue;
			bChanged = true;
			break;
		}	
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_EXT_MODE_L:
		strValue = pProp->GetValue();
		nValue = AOIDataDefine.FindAlgOuterShortExtendModeByText(strValue);
		switch ( nValue ) 
		{
		case ALG_OUTER_SHORT_EXT_NONE:
		case ALG_OUTER_SHORT_EXT_LEFT:
		case ALG_OUTER_SHORT_EXT_RIGHT:
		case ALG_OUTER_SHORT_EXT_BOTH:
			osParam.osLine_L.olExtMode = (ALG_OUTER_SHORT_EXT_MODE)(nValue);
			bChanged = true;
			break;
		}
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_USL_L:		
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < osParam.osLine_L.olLSL ) 
		{	dValue = osParam.osLine_L.olLSL;	}		
		osParam.osLine_L.olUSL = dValue;				
		bIsPass = CAlgParam::CheckOK_OuterShortUSL_L(osParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_LSL_L:		
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > osParam.osLine_L.olUSL ) 
		{	dValue = osParam.osLine_L.olUSL;	}
		osParam.osLine_L.olLSL = dValue;				
		bIsPass = CAlgParam::CheckOK_OuterShortLSL_L(osParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_APPLY_L:
		osParam.SetAll(osParam.osLine_L);
		bChanged = true;
		bReBuildWndUI = true;
		break;

	case WND_ALG_PROPERTY_OUTER_SHORT_ENABLED_B:
		bEnabled = pProp->GetCheckValue();
		bBoolParam = true;
		strNewValue = AOIDataDefine.GetEnableDisableText(bEnabled);
		strOldValue = AOIDataDefine.GetEnableDisableText(!bEnabled);
		osParam.osLine_B.olEnabled = bEnabled;
		pProp->Expand(bEnabled);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_RANGE_B:
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < 1 ) 
		{	dValue = 1;	}		
		osParam.osLine_B.olRange = dValue;		
		strValue.Format(_T("%.0f"), dValue);
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_MODE_B:
		strValue = pProp->GetValue();
		nValue = AOIDataDefine.FindAlgBrightLineModeByText(strValue);
		switch ( nValue ) 
		{
		case LINE_MODE_DARK:
		case LINE_MODE_BRIGHT:
			osParam.osLine_B.olMode = nValue;
			bChanged = true;
			break;
		}	
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_EXT_MODE_B:
		strValue = pProp->GetValue();
		nValue = AOIDataDefine.FindAlgOuterShortExtendModeByText(strValue);
		switch ( nValue ) 
		{
		case ALG_OUTER_SHORT_EXT_NONE:
		case ALG_OUTER_SHORT_EXT_LEFT:
		case ALG_OUTER_SHORT_EXT_RIGHT:
		case ALG_OUTER_SHORT_EXT_BOTH:
			osParam.osLine_B.olExtMode = (ALG_OUTER_SHORT_EXT_MODE)(nValue);
			bChanged = true;
			break;
		}
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_USL_B:		
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue < osParam.osLine_B.olLSL ) 
		{	dValue = osParam.osLine_B.olLSL;	}		
		osParam.osLine_B.olUSL = dValue;				
		bIsPass = CAlgParam::CheckOK_OuterShortUSL_B(osParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_LSL_B:		
		strValue = pProp->GetValue();
		dValue = ::_tcstod(strValue, NULL);		
		if ( dValue > osParam.osLine_B.olUSL ) 
		{	dValue = osParam.osLine_B.olUSL;	}
		osParam.osLine_B.olLSL = dValue;				
		bIsPass = CAlgParam::CheckOK_OuterShortLSL_B(osParam);
		if ( false == bIsPass )
		{	pProp->SetReadingTextColor(clrNG); }
		else
		{	pProp->SetReadingTextColor(clrOK); }
		strValue.Format(_T("%.2f"), dValue);		
		pProp->SetValue(strValue);
		bChanged = true;
		break;
	case WND_ALG_PROPERTY_OUTER_SHORT_APPLY_B:
		osParam.SetAll(osParam.osLine_B);
		bChanged = true;
		bReBuildWndUI = true;
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