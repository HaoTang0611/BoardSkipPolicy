// EditWndView_MeasurementBlackGlue.cpp : implementation file
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
bool CEditWndView::BuildWndParamList_MeasureBlackGlue(CAOIProject *Project, CAOIWnd *WndPtr)
{
#ifdef ALG_MEASURE_BLACK_GLUE_USE
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }	
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();	
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const WND_LOGIC_TYPE    WndLogicType = WndPtr->GetWndLogicType();		
	const TALG_PARAM_MEASURE_BLACK_GLUE &bgParam = AlgParam.GetAlgParamMeasureBlackGlue();	

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;	
	const TPropGridParam &PropGridParam=GetPropGridParam();

	CString str;
	bool    bIsPass = true;
	BOOL    bEnabled=TRUE;	
	CString strMax, strMin, strDif, strUSL, strLSL;
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
	//if ( BuildWndParamList_AlgFrameIndex(pGroupBasic, Project, WndPtr) == false )
	//{	return false;	}	
	//if ( BuildWndParamList_MaskFrameIndex(pGroupBasic, Project, WndPtr) == false )
	//{	return false;	}

	//畫面列表		
	strCaption = _T("Glue Frame");//Glue
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyWndFrameList(strCaption, Project, WndPtr, bgParam.bgFrameUniqueID1, strDescr);
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_FRAME_ID_01);
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Board Frame");//Board
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyWndFrameList(strCaption, Project, WndPtr, bgParam.bgFrameUniqueID2, strDescr);
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_FRAME_ID_02);
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Thermal Frame");//Thermal
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyWndFrameList(strCaption, Project, WndPtr, bgParam.bgFrameUniqueID3, strDescr);
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_FRAME_ID_03);
	pGroupBasic->AddSubItem(pParamItem);

	strCaption = _T("Coating Frame");//Coating
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyWndFrameList(strCaption, Project, WndPtr, bgParam.bgFrameUniqueID4, strDescr);
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_FRAME_ID_04);
	pGroupBasic->AddSubItem(pParamItem);

	//增加Birght Ratio 參數
	double dUSL = 0;
	double dLSL = 0;
	double dReading = 0;
	double dRatio = 0.0;			
	
	strMax = _T("Max"); strMax = LoadMultiLanguageString(strMax, strMax);
	strMin = _T("Min"); strMin = LoadMultiLanguageString(strMin, strMin);
	strDif = _T("Dif"); strDif = LoadMultiLanguageString(strDif, strDif);	
	strUSL = _T("USL"); strUSL = LoadMultiLanguageString(strUSL, strUSL);
	strLSL = _T("LSL"); strLSL = LoadMultiLanguageString(strLSL, strLSL);

	//結果文字顯示
	if ( BuildWndParamList_ResultText(pGroupBasic, Project, WndPtr) == false )
	{	return false; }
	
	const auto &sParam = bgParam.bgParam;
	const auto &sResult = bgParam.bgResult;

	const auto &sGlue = sParam.sGlue;	
	const auto &sBoard = sParam.sBoard;	
	const auto &sCoating = sParam.sCoating;	
	const auto &sGlueArc = sParam.sGlueArc;	
	const auto &sGlueStartEnd = sParam.sGlueStartEnd;
	const auto &sGlueWidth_Vertical = sParam.sGlueWidth_Vertical;	
	const auto &sGlueWidth_Horizontal = sParam.sGlueWidth_Horizontal;	
	const auto &sBoardToGlue_Vertical = sParam.sBoardToGlue_Vertical;		
	const auto &sBoardToGlue_Horizontal = sParam.sBoardToGlue_Horizontal;
	const auto &sCoatingToGlue_Vertical = sParam.sCoatingToGlue_Vertical;		
	const auto &sCoatingToGlue_Horizontal = sParam.sCoatingToGlue_Horizontal;	
	const auto &sThermal = sParam.sThermal;

	if ( sBoard.bEnable_Board  )
	{
		const auto &sBoardEdge = sResult.sBoardEdge;// 板邊偵測結果	
		const auto &sLine_Vertical=sBoardEdge.sLine_Vertical;
		const auto &sLine_Horizontal=sBoardEdge.sLine_Horizontal;
		//WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_BOARD_EDGE,//板邊偵測結果
	}

	if ( sGlue.bEnable )
	{
		const auto &sGlueEdge = sResult.sGlueEdge;// 膠邊偵測結果
		//WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_GLUE_EDGE,//膠邊偵測結果
	}

	if ( sCoating.bEnable )
	{	
		const auto &sCoatingEdge = sResult.sCoatingEdge;// Coating偵測結果	
		//WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_COATING_EDGE,//Coating偵測結果

	}
	
	if ( sGlueStartEnd.bEnable )
	{
		CJETPropertyGridProperty* pGroupItem = NULL;		
		JET::alg::SSingleStartEndROI_Result sDummy_Result;
		const auto &vtsStartEndParam = sGlueStartEnd.vtsStartEnd;
		const auto &vtsStartEndResult = sResult.sStartEnd.vtsStartEnd;// 膠首尾距離量測結果		
		
		for ( size_t i=0; i<vtsStartEndParam.size(); i++ )
		{
			auto sStartEndResult = sDummy_Result;
			const auto &sStartEndParam=vtsStartEndParam[i];
			const auto &sLimit = sStartEndParam.sLimit;
			if ( i < vtsStartEndResult.size() )
			{	sStartEndResult = vtsStartEndResult[i];	}
		
			str = _T("Start-End");
			//strCaption = LoadMultiLanguageString(str, str);				
			strCaption.Format(_T("%s[%d]"), str, i+1);
			pGroupItem = new CJETPropertyGridProperty(strCaption);
			if ( NULL == pGroupItem ) { return false; }	
			pGroupItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_START_END);
			pGroupItem->SetData((DWORD_PTR)WndPtr);
			wndPropList.AddProperty(pGroupItem, bRedraw, bAdjustLayou);	
			
			strCaption = _T("USL");	
			strCaption.Format(_T("%s"), strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.fUpper);	
			strReading.Format(_T("%.0f"), sStartEndResult.sDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_START_DIST_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sStartEndResult.bResult_MaxDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("LSL");	
			strCaption.Format(_T("%s"), strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);
			strValue.Format(_T("%.0f"), sLimit.fLower);	
			strReading.Format(_T("%.0f"), sStartEndResult.sDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_START_DIST_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sStartEndResult.bResult_MaxDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);
		}		
	}

	if ( sGlueWidth_Horizontal.bEnable && sGlueWidth_Horizontal.nCount>0 )// 水平膠寬量測結果
	{	
		JET::alg::SSingleROI_Result sDummy_Result;
		CJETPropertyGridProperty* pGroupItem = NULL;
		const auto &vtsLimit = sGlueWidth_Horizontal.vtsLimit;		
		const auto &vtsRoiInfo = sResult.sWidth_Horizontal.vtsRoiInfo;
		
		for ( size_t i=0; i<vtsLimit.size(); i++ )
		{	
			auto sRoiInfo = sDummy_Result;
			const auto &sLimit = vtsLimit[i];				
			if ( i < vtsRoiInfo.size() )
			{	sRoiInfo = vtsRoiInfo[i];	}
			const auto &sMaxDist = sRoiInfo.sMaxDist;
			const auto &sMinDist = sRoiInfo.sMinDist;
			const auto fDist_MaxMin=sRoiInfo.fDiff_Dist_MaxMin;			

			str = _T("Width Hor");
			//strCaption = LoadMultiLanguageString(str, str);	
			strCaption.Format(_T("%s[%d]"), str, i+1);
			pGroupItem = new CJETPropertyGridProperty(strCaption);
			if ( NULL == pGroupItem ) { return false; }	
			pGroupItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_WIDTH_HOR);
			pGroupItem->SetData((DWORD_PTR)WndPtr);
			wndPropList.AddProperty(pGroupItem, bRedraw, bAdjustLayou);	

			strCaption = _T("Max-USL");	
			strCaption.Format(_T("%s-%s"), strMax, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMaxDist.fUpper);	
			strReading.Format(_T("%.0f"), sMaxDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_WIDTH_HOR_DIST_MAX_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MaxDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Max-LSL");	
			strCaption.Format(_T("%s-%s"), strMax, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMaxDist.fLower);	
			strReading.Format(_T("%.0f"), sMaxDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_WIDTH_HOR_DIST_MAX_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MaxDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);				

			strCaption = _T("Min-USL");	
			strCaption.Format(_T("%s-%s"), strMin, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMinDist.fUpper);	
			strReading.Format(_T("%.0f"), sMinDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_WIDTH_HOR_DIST_MIN_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MinDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Min-LSL");	
			strCaption.Format(_T("%s-%s"), strMin, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMinDist.fLower);	
			strReading.Format(_T("%.0f"), sMinDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_WIDTH_HOR_DIST_MIN_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MinDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);
			
			strCaption = _T("Dif-USL");	
			strCaption.Format(_T("%s-%s"), strDif, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fUpper);	
			strReading.Format(_T("%.0f"), fDist_MaxMin);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_WIDTH_HOR_DIST_DIF_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_Diff_MaxMin )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Dif-LSL");	
			strCaption.Format(_T("%s-%s"), strDif, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);
			strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fLower);	
			strReading.Format(_T("%.0f"), fDist_MaxMin);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_WIDTH_HOR_DIST_DIF_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_Diff_MaxMin )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);
		}		
	}

	if ( sGlueWidth_Vertical.bEnable && sGlueWidth_Vertical.nCount>0 )// 垂直膠寬量測結果
	{	
		JET::alg::SSingleROI_Result sDummy_Result;
		CJETPropertyGridProperty* pGroupItem = NULL;
		const auto &vtsLimit = sGlueWidth_Vertical.vtsLimit;		
		const auto &vtsRoiInfo = sResult.sWidth_Vertical.vtsRoiInfo;
		
		for ( size_t i=0; i<vtsLimit.size(); i++ )
		{	
			auto sRoiInfo = sDummy_Result;
			const auto &sLimit = vtsLimit[i];				
			if ( i < vtsRoiInfo.size() )
			{	sRoiInfo = vtsRoiInfo[i];	}
			const auto &sMaxDist = sRoiInfo.sMaxDist;
			const auto &sMinDist = sRoiInfo.sMinDist;
			const auto fDist_MaxMin=sRoiInfo.fDiff_Dist_MaxMin;			

			str = _T("Width Ver");
			//strCaption = LoadMultiLanguageString(str, str);	
			strCaption.Format(_T("%s[%d]"), str, i+1);
			pGroupItem = new CJETPropertyGridProperty(strCaption);
			if ( NULL == pGroupItem ) { return false; }	
			pGroupItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_WIDTH_VER);
			pGroupItem->SetData((DWORD_PTR)WndPtr);
			wndPropList.AddProperty(pGroupItem, bRedraw, bAdjustLayou);	

			strCaption = _T("Max-USL");	
			strCaption.Format(_T("%s-%s"), strMax, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMaxDist.fUpper);	
			strReading.Format(_T("%.0f"), sMaxDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_WIDTH_VER_DIST_MAX_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MaxDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Max-LSL");	
			strCaption.Format(_T("%s-%s"), strMax, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMaxDist.fLower);	
			strReading.Format(_T("%.0f"), sMaxDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_WIDTH_VER_DIST_MAX_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MaxDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);				

			strCaption = _T("Min-USL");	
			strCaption.Format(_T("%s-%s"), strMin, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMinDist.fUpper);	
			strReading.Format(_T("%.0f"), sMinDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_WIDTH_VER_DIST_MIN_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MinDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Min-LSL");	
			strCaption.Format(_T("%s-%s"), strMin, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMinDist.fLower);	
			strReading.Format(_T("%.0f"), sMinDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_WIDTH_VER_DIST_MIN_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MinDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);
			
			strCaption = _T("Dif-USL");	
			strCaption.Format(_T("%s-%s"), strDif, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fUpper);	
			strReading.Format(_T("%.0f"), fDist_MaxMin);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_WIDTH_VER_DIST_DIF_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_Diff_MaxMin )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Dif-LSL");	
			strCaption.Format(_T("%s-%s"), strDif, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);
			strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fLower);	
			strReading.Format(_T("%.0f"), fDist_MaxMin);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_WIDTH_VER_DIST_DIF_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_Diff_MaxMin )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);
		}		
	}

	if ( sBoardToGlue_Horizontal.bEnable && sBoardToGlue_Horizontal.nCount>0 )// 水平方向板邊到膠邊參數
	{	
		JET::alg::SSingleROI_Result sDummy_Result;
		CJETPropertyGridProperty* pGroupItem = NULL;
		const auto &vtsLimit = sBoardToGlue_Horizontal.vtsLimit;		
		const auto &vtsRoiInfo = sResult.sBoardToGlue_Horizontal.vtsRoiInfo;

		for ( size_t i=0; i<vtsLimit.size(); i++ )
		{	
			auto sRoiInfo = sDummy_Result;
			const auto &sLimit = vtsLimit[i];				
			if ( i < vtsRoiInfo.size() )
			{	sRoiInfo = vtsRoiInfo[i];	}
			const auto &sMaxDist = sRoiInfo.sMaxDist;
			const auto &sMinDist = sRoiInfo.sMinDist;
			const auto fDist_MaxMin=sRoiInfo.fDiff_Dist_MaxMin;			

			str = _T("Board-Glue Hor");
			//strCaption = LoadMultiLanguageString(str, str);	
			strCaption.Format(_T("%s[%d]"), str, i+1);
			pGroupItem = new CJETPropertyGridProperty(strCaption);
			if ( NULL == pGroupItem ) { return false; }	
			pGroupItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_BOARD_TO_GLUE_HOR);
			pGroupItem->SetData((DWORD_PTR)WndPtr);
			wndPropList.AddProperty(pGroupItem, bRedraw, bAdjustLayou);	

			strCaption = _T("Max-USL");	
			strCaption.Format(_T("%s-%s"), strMax, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMaxDist.fUpper);	
			strReading.Format(_T("%.0f"), sMaxDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_BOARD_TO_GLUE_HOR_DIST_MAX_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MaxDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Max-LSL");	
			strCaption.Format(_T("%s-%s"), strMax, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMaxDist.fLower);	
			strReading.Format(_T("%.0f"), sMaxDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_BOARD_TO_GLUE_HOR_DIST_MAX_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MaxDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);				

			strCaption = _T("Min-USL");	
			strCaption.Format(_T("%s-%s"), strMin, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMinDist.fUpper);	
			strReading.Format(_T("%.0f"), sMinDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_BOARD_TO_GLUE_HOR_DIST_MIN_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MinDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Min-LSL");	
			strCaption.Format(_T("%s-%s"), strMin, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMinDist.fLower);	
			strReading.Format(_T("%.0f"), sMinDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_BOARD_TO_GLUE_HOR_DIST_MIN_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MinDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);
			
			strCaption = _T("Dif-USL");	
			strCaption.Format(_T("%s-%s"), strDif, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fUpper);	
			strReading.Format(_T("%.0f"), fDist_MaxMin);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_BOARD_TO_GLUE_HOR_DIST_DIF_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_Diff_MaxMin )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Dif-LSL");	
			strCaption.Format(_T("%s-%s"), strDif, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);
			strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fLower);	
			strReading.Format(_T("%.0f"), fDist_MaxMin);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_BOARD_TO_GLUE_HOR_DIST_DIF_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_Diff_MaxMin )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);
		}		
	}	
	
	if ( sBoardToGlue_Vertical.bEnable && sBoardToGlue_Vertical.nCount>0 )// 垂直方向板邊到膠邊參數
	{	
		JET::alg::SSingleROI_Result sDummy_Result;
		CJETPropertyGridProperty* pGroupItem = NULL;
		const auto &vtsLimit = sBoardToGlue_Vertical.vtsLimit;		
		const auto &vtsRoiInfo = sResult.sBoardToGlue_Vertical.vtsRoiInfo;

		for ( size_t i=0; i<vtsLimit.size(); i++ )
		{	
			auto sRoiInfo = sDummy_Result;
			const auto &sLimit = vtsLimit[i];				
			if ( i < vtsRoiInfo.size() )
			{	sRoiInfo = vtsRoiInfo[i];	}
			const auto &sMaxDist = sRoiInfo.sMaxDist;
			const auto &sMinDist = sRoiInfo.sMinDist;
			const auto fDist_MaxMin=sRoiInfo.fDiff_Dist_MaxMin;			

			str = _T("Board-Glue Ver");
			//strCaption = LoadMultiLanguageString(str, str);	
			strCaption.Format(_T("%s[%d]"), str, i+1);
			pGroupItem = new CJETPropertyGridProperty(strCaption);
			if ( NULL == pGroupItem ) { return false; }	
			pGroupItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_BOARD_TO_GLUE_VER);
			pGroupItem->SetData((DWORD_PTR)WndPtr);
			wndPropList.AddProperty(pGroupItem, bRedraw, bAdjustLayou);	

			strCaption = _T("Max-USL");	
			strCaption.Format(_T("%s-%s"), strMax, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMaxDist.fUpper);	
			strReading.Format(_T("%.0f"), sMaxDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_BOARD_TO_GLUE_VER_DIST_MAX_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MaxDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Max-LSL");	
			strCaption.Format(_T("%s-%s"), strMax, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMaxDist.fLower);	
			strReading.Format(_T("%.0f"), sMaxDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_BOARD_TO_GLUE_VER_DIST_MAX_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MaxDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);				

			strCaption = _T("Min-USL");	
			strCaption.Format(_T("%s-%s"), strMin, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMinDist.fUpper);	
			strReading.Format(_T("%.0f"), sMinDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_BOARD_TO_GLUE_VER_DIST_MIN_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MinDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Min-LSL");	
			strCaption.Format(_T("%s-%s"), strMin, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMinDist.fLower);	
			strReading.Format(_T("%.0f"), sMinDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_BOARD_TO_GLUE_VER_DIST_MIN_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MinDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);
			
			strCaption = _T("Dif-USL");	
			strCaption.Format(_T("%s-%s"), strDif, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fUpper);	
			strReading.Format(_T("%.0f"), fDist_MaxMin);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_BOARD_TO_GLUE_VER_DIST_DIF_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_Diff_MaxMin )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Dif-LSL");	
			strCaption.Format(_T("%s-%s"), strDif, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);
			strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fLower);	
			strReading.Format(_T("%.0f"), fDist_MaxMin);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_BOARD_TO_GLUE_VER_DIST_DIF_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_Diff_MaxMin )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);
		}		
	}	
	
	if ( sCoatingToGlue_Horizontal.bEnable && sCoatingToGlue_Horizontal.nCount>0 )// 水平方向Coating到膠邊參數
	{	
		JET::alg::SSingleROI_Result sDummy_Result;
		CJETPropertyGridProperty* pGroupItem = NULL;
		const auto &vtsLimit = sCoatingToGlue_Horizontal.vtsLimit;		
		const auto &vtsRoiInfo = sResult.sCoatingToGlue_Horizontal.vtsRoiInfo;

		for ( size_t i=0; i<vtsLimit.size(); i++ )
		{	
			auto sRoiInfo = sDummy_Result;
			const auto &sLimit = vtsLimit[i];				
			if ( i < vtsRoiInfo.size() )
			{	sRoiInfo = vtsRoiInfo[i];	}
			const auto &sMaxDist = sRoiInfo.sMaxDist;
			const auto &sMinDist = sRoiInfo.sMinDist;
			const auto fDist_MaxMin=sRoiInfo.fDiff_Dist_MaxMin;			

			str = _T("Coating-Glue Hor");
			//strCaption = LoadMultiLanguageString(str, str);	
			strCaption.Format(_T("%s[%d]"), str, i+1);
			pGroupItem = new CJETPropertyGridProperty(strCaption);
			if ( NULL == pGroupItem ) { return false; }	
			pGroupItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_COATING_TO_GLUE_HOR);
			pGroupItem->SetData((DWORD_PTR)WndPtr);
			wndPropList.AddProperty(pGroupItem, bRedraw, bAdjustLayou);	

			strCaption = _T("Max-USL");	
			strCaption.Format(_T("%s-%s"), strMax, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMaxDist.fUpper);	
			strReading.Format(_T("%.0f"), sMaxDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_COATING_TO_GLUE_HOR_DIST_MAX_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MaxDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Max-LSL");	
			strCaption.Format(_T("%s-%s"), strMax, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMaxDist.fLower);	
			strReading.Format(_T("%.0f"), sMaxDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_COATING_TO_GLUE_HOR_DIST_MAX_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MaxDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);				

			strCaption = _T("Min-USL");	
			strCaption.Format(_T("%s-%s"), strMin, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMinDist.fUpper);	
			strReading.Format(_T("%.0f"), sMinDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_COATING_TO_GLUE_HOR_DIST_MIN_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MinDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Min-LSL");	
			strCaption.Format(_T("%s-%s"), strMin, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMinDist.fLower);	
			strReading.Format(_T("%.0f"), sMinDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_COATING_TO_GLUE_HOR_DIST_MIN_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MinDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);
			
			strCaption = _T("Dif-USL");	
			strCaption.Format(_T("%s-%s"), strDif, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fUpper);	
			strReading.Format(_T("%.0f"), fDist_MaxMin);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_COATING_TO_GLUE_HOR_DIST_DIF_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_Diff_MaxMin )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Dif-LSL");	
			strCaption.Format(_T("%s-%s"), strDif, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);
			strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fLower);	
			strReading.Format(_T("%.0f"), fDist_MaxMin);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_COATING_TO_GLUE_HOR_DIST_DIF_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_Diff_MaxMin )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);
		}		
	}
	
	if ( sCoatingToGlue_Vertical.bEnable && sCoatingToGlue_Vertical.nCount>0 )// 垂直方向Coating到膠邊參數
	{	
		JET::alg::SSingleROI_Result sDummy_Result;
		CJETPropertyGridProperty* pGroupItem = NULL;
		const auto &vtsLimit = sCoatingToGlue_Vertical.vtsLimit;		
		const auto &vtsRoiInfo = sResult.sCoatingToGlue_Vertical.vtsRoiInfo;

		for ( size_t i=0; i<vtsLimit.size(); i++ )
		{	
			auto sRoiInfo = sDummy_Result;
			const auto &sLimit = vtsLimit[i];				
			if ( i < vtsRoiInfo.size() )
			{	sRoiInfo = vtsRoiInfo[i];	}
			const auto &sMaxDist = sRoiInfo.sMaxDist;
			const auto &sMinDist = sRoiInfo.sMinDist;
			const auto fDist_MaxMin=sRoiInfo.fDiff_Dist_MaxMin;			

			str = _T("Coating-Glue Hor");
			//strCaption = LoadMultiLanguageString(str, str);	
			strCaption.Format(_T("%s[%d]"), str, i+1);
			pGroupItem = new CJETPropertyGridProperty(strCaption);
			if ( NULL == pGroupItem ) { return false; }	
			pGroupItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_COATING_TO_GLUE_VER);
			pGroupItem->SetData((DWORD_PTR)WndPtr);
			wndPropList.AddProperty(pGroupItem, bRedraw, bAdjustLayou);	

			strCaption = _T("Max-USL");	
			strCaption.Format(_T("%s-%s"), strMax, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMaxDist.fUpper);	
			strReading.Format(_T("%.0f"), sMaxDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_COATING_TO_GLUE_VER_DIST_MAX_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MaxDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Max-LSL");	
			strCaption.Format(_T("%s-%s"), strMax, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMaxDist.fLower);	
			strReading.Format(_T("%.0f"), sMaxDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_COATING_TO_GLUE_VER_DIST_MAX_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MaxDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);				

			strCaption = _T("Min-USL");	
			strCaption.Format(_T("%s-%s"), strMin, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMinDist.fUpper);	
			strReading.Format(_T("%.0f"), sMinDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_COATING_TO_GLUE_VER_DIST_MIN_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MinDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Min-LSL");	
			strCaption.Format(_T("%s-%s"), strMin, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMinDist.fLower);	
			strReading.Format(_T("%.0f"), sMinDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_COATING_TO_GLUE_VER_DIST_MIN_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MinDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);
			
			strCaption = _T("Dif-USL");	
			strCaption.Format(_T("%s-%s"), strDif, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fUpper);	
			strReading.Format(_T("%.0f"), fDist_MaxMin);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_COATING_TO_GLUE_VER_DIST_DIF_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_Diff_MaxMin )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Dif-LSL");	
			strCaption.Format(_T("%s-%s"), strDif, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);
			strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fLower);	
			strReading.Format(_T("%.0f"), fDist_MaxMin);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_COATING_TO_GLUE_VER_DIST_DIF_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_Diff_MaxMin )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);
		}		
	}
	
	if ( sGlueArc.bEnable && sGlueArc.nCount>0 )// 弧線膠寬參數
	{	
		JET::alg::SSingleROI_Result sDummy_Result;
		CJETPropertyGridProperty* pGroupItem = NULL;
		const auto &vtsLimit = sGlueArc.vtsLimit;		
		const auto &vtsRoiInfo = sResult.sWidth_Arc.vtsRoiInfo;

		for ( size_t i=0; i<vtsLimit.size(); i++ )
		{	
			auto sRoiInfo = sDummy_Result;
			const auto &sLimit = vtsLimit[i];				
			if ( i < vtsRoiInfo.size() )
			{	sRoiInfo = vtsRoiInfo[i];	}
			const auto &sMaxDist = sRoiInfo.sMaxDist;
			const auto &sMinDist = sRoiInfo.sMinDist;
			const auto fDist_MaxMin=sRoiInfo.fDiff_Dist_MaxMin;			

			str = _T("Width Arc");
			//strCaption = LoadMultiLanguageString(str, str);	
			strCaption.Format(_T("%s[%d]"), str, i+1);
			pGroupItem = new CJETPropertyGridProperty(strCaption);
			if ( NULL == pGroupItem ) { return false; }	
			pGroupItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_ARC);
			pGroupItem->SetData((DWORD_PTR)WndPtr);
			wndPropList.AddProperty(pGroupItem, bRedraw, bAdjustLayou);	

			strCaption = _T("Max-USL");	
			strCaption.Format(_T("%s-%s"), strMax, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMaxDist.fUpper);	
			strReading.Format(_T("%.0f"), sMaxDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_ARC_DIST_MAX_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MaxDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Max-LSL");	
			strCaption.Format(_T("%s-%s"), strMax, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMaxDist.fLower);	
			strReading.Format(_T("%.0f"), sMaxDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_ARC_DIST_MAX_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MaxDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);				

			strCaption = _T("Min-USL");	
			strCaption.Format(_T("%s-%s"), strMin, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMinDist.fUpper);	
			strReading.Format(_T("%.0f"), sMinDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_ARC_DIST_MIN_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MinDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Min-LSL");	
			strCaption.Format(_T("%s-%s"), strMin, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sMinDist.fLower);	
			strReading.Format(_T("%.0f"), sMinDist.fDistance);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_ARC_DIST_MIN_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_MinDist )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);
			
			strCaption = _T("Dif-USL");	
			strCaption.Format(_T("%s-%s"), strDif, strUSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
			strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fUpper);	
			strReading.Format(_T("%.0f"), fDist_MaxMin);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_ARC_DIST_DIF_USL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_Diff_MaxMin )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);

			strCaption = _T("Dif-LSL");	
			strCaption.Format(_T("%s-%s"), strDif, strLSL);
			//strCaption = LoadMultiLanguageString(strCaption, strCaption);
			strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fLower);	
			strReading.Format(_T("%.0f"), fDist_MaxMin);
			pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
			if ( NULL == pParamItem ) { return false; }	
			pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_ARC_DIST_DIF_LSL);
			pParamItem->AllowEdit(FALSE);
			pParamItem->SetReading(strReading);
			if ( true == sRoiInfo.bResult_Diff_MaxMin )
			{	pParamItem->SetReadingTextColor(clrOK); }
			else
			{	pParamItem->SetReadingTextColor(clrNG); }
			pGroupItem->AddSubItem(pParamItem);
		}		
	}	

	if ( sThermal.bEnable && sThermal.nGlue_Count>0 )//散熱膠參數
	{		
		const bool bUseWidth = true;
		const bool bUseToGlue = true;
		const bool bUseToDie = true;
		JET::alg::SSingleROI_Result sDummy_Result;
		CJETPropertyGridProperty* pGroupItem = NULL;		
		if ( bUseWidth )
		{
			const auto &vtsLimit = sThermal.vtsLimit_Width;
			const auto &vtsRoiInfo = sResult.sThermalGlue.sGlueWidth.vtsRoiInfo;
			for ( size_t i=0; i<vtsLimit.size(); i++ )
			{	
				auto sRoiInfo = sDummy_Result;
				const auto &sLimit = vtsLimit[i];				
				if ( i < vtsRoiInfo.size() )
				{	sRoiInfo = vtsRoiInfo[i];	}
				const auto &sMaxDist = sRoiInfo.sMaxDist;
				const auto &sMinDist = sRoiInfo.sMinDist;
				const auto fDist_MaxMin=sRoiInfo.fDiff_Dist_MaxMin;			

				str = _T("Thermal-Glue Width");
				//strCaption = LoadMultiLanguageString(str, str);	
				strCaption.Format(_T("%s[%d]"), str, i+1);
				pGroupItem = new CJETPropertyGridProperty(strCaption);
				if ( NULL == pGroupItem ) { return false; }	
				pGroupItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_WIDTH);
				pGroupItem->SetData((DWORD_PTR)WndPtr);
				wndPropList.AddProperty(pGroupItem, bRedraw, bAdjustLayou);	

				strCaption = _T("Max-USL");	
				strCaption.Format(_T("%s-%s"), strMax, strUSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
				strValue.Format(_T("%.0f"), sLimit.sMaxDist.fUpper);	
				strReading.Format(_T("%.0f"), sMaxDist.fDistance);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_WIDTH_DIST_MAX_USL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_MaxDist )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);

				strCaption = _T("Max-LSL");	
				strCaption.Format(_T("%s-%s"), strMax, strLSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
				strValue.Format(_T("%.0f"), sLimit.sMaxDist.fLower);	
				strReading.Format(_T("%.0f"), sMaxDist.fDistance);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_WIDTH_DIST_MAX_LSL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_MaxDist )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);				

				strCaption = _T("Min-USL");	
				strCaption.Format(_T("%s-%s"), strMin, strUSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
				strValue.Format(_T("%.0f"), sLimit.sMinDist.fUpper);	
				strReading.Format(_T("%.0f"), sMinDist.fDistance);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_WIDTH_DIST_MIN_USL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_MinDist )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);

				strCaption = _T("Min-LSL");	
				strCaption.Format(_T("%s-%s"), strMin, strLSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
				strValue.Format(_T("%.0f"), sLimit.sMinDist.fLower);	
				strReading.Format(_T("%.0f"), sMinDist.fDistance);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_WIDTH_DIST_MIN_LSL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_MinDist )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);
			
				strCaption = _T("Dif-USL");	
				strCaption.Format(_T("%s-%s"), strDif, strUSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
				strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fUpper);	
				strReading.Format(_T("%.0f"), fDist_MaxMin);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_WIDTH_DIST_DIF_USL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_Diff_MaxMin )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);

				strCaption = _T("Dif-LSL");	
				strCaption.Format(_T("%s-%s"), strDif, strLSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);
				strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fLower);	
				strReading.Format(_T("%.0f"), fDist_MaxMin);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_WIDTH_DIST_DIF_LSL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_Diff_MaxMin )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);
			}
		}

		if ( bUseToGlue )
		{
			const auto &vtsLimit = sThermal.vtsLimit_GlueToGlue;
			const auto &vtsRoiInfo = sResult.sThermalGlue.sGlueToGlue.vtsRoiInfo;
			for ( size_t i=0; i<vtsLimit.size(); i++ )
			{	
				auto sRoiInfo = sDummy_Result;
				const auto &sLimit = vtsLimit[i];				
				if ( i < vtsRoiInfo.size() )
				{	sRoiInfo = vtsRoiInfo[i];	}
				const auto &sMaxDist = sRoiInfo.sMaxDist;
				const auto &sMinDist = sRoiInfo.sMinDist;
				const auto fDist_MaxMin=sRoiInfo.fDiff_Dist_MaxMin;			

				str = _T("Thermal-Glue to Glue");
				//strCaption = LoadMultiLanguageString(str, str);	
				strCaption.Format(_T("%s[%d]"), str, i+1);
				pGroupItem = new CJETPropertyGridProperty(strCaption);
				if ( NULL == pGroupItem ) { return false; }	
				pGroupItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_TO_GLUE);
				pGroupItem->SetData((DWORD_PTR)WndPtr);
				wndPropList.AddProperty(pGroupItem, bRedraw, bAdjustLayou);	

				strCaption = _T("Max-USL");	
				strCaption.Format(_T("%s-%s"), strMax, strUSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
				strValue.Format(_T("%.0f"), sLimit.sMaxDist.fUpper);	
				strReading.Format(_T("%.0f"), sMaxDist.fDistance);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_TO_GLUE_DIST_MAX_USL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_MaxDist )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);

				strCaption = _T("Max-LSL");	
				strCaption.Format(_T("%s-%s"), strMax, strLSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
				strValue.Format(_T("%.0f"), sLimit.sMaxDist.fLower);	
				strReading.Format(_T("%.0f"), sMaxDist.fDistance);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_TO_GLUE_DIST_MAX_LSL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_MaxDist )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);

				strCaption = _T("Min-USL");	
				strCaption.Format(_T("%s-%s"), strMin, strUSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
				strValue.Format(_T("%.0f"), sLimit.sMinDist.fUpper);	
				strReading.Format(_T("%.0f"), sMinDist.fDistance);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_TO_GLUE_DIST_MIN_USL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_MinDist )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);

				strCaption = _T("Min-LSL");	
				strCaption.Format(_T("%s-%s"), strMin, strLSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
				strValue.Format(_T("%.0f"), sLimit.sMinDist.fLower);	
				strReading.Format(_T("%.0f"), sMinDist.fDistance);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_TO_GLUE_DIST_MIN_LSL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_MinDist )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);
			
				strCaption = _T("Dif-USL");	
				strCaption.Format(_T("%s-%s"), strDif, strUSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
				strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fUpper);	
				strReading.Format(_T("%.0f"), fDist_MaxMin);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_TO_GLUE_DIST_DIF_USL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_Diff_MaxMin )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);

				strCaption = _T("Dif-LSL");	
				strCaption.Format(_T("%s-%s"), strDif, strLSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);
				strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fLower);	
				strReading.Format(_T("%.0f"), fDist_MaxMin);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_TO_GLUE_DIST_DIF_LSL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_Diff_MaxMin )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);
			}
		}

		if ( bUseToDie )
		{
			const auto &vtsLimit = sThermal.vtsLimit_GlueToDie;
			const auto &vtsRoiInfo = sResult.sThermalGlue.sGlueToDie.vtsRoiInfo;
			for ( size_t i=0; i<vtsLimit.size(); i++ )
			{	
				auto sRoiInfo = sDummy_Result;
				const auto &sLimit = vtsLimit[i];				
				if ( i < vtsRoiInfo.size() )
				{	sRoiInfo = vtsRoiInfo[i];	}
				const auto &sMaxDist = sRoiInfo.sMaxDist;
				const auto &sMinDist = sRoiInfo.sMinDist;
				const auto fDist_MaxMin=sRoiInfo.fDiff_Dist_MaxMin;			

				str = _T("Thermal-Glue to Die");
				//strCaption = LoadMultiLanguageString(str, str);	
				strCaption.Format(_T("%s[%d]"), str, i+1);
				pGroupItem = new CJETPropertyGridProperty(strCaption);
				if ( NULL == pGroupItem ) { return false; }	
				pGroupItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_TO_DIE);
				pGroupItem->SetData((DWORD_PTR)WndPtr);
				wndPropList.AddProperty(pGroupItem, bRedraw, bAdjustLayou);	

				strCaption = _T("Max-USL");	
				strCaption.Format(_T("%s-%s"), strMax, strUSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
				strValue.Format(_T("%.0f"), sLimit.sMaxDist.fUpper);	
				strReading.Format(_T("%.0f"), sMaxDist.fDistance);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_TO_DIE_DIST_MAX_USL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_MaxDist )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);

				strCaption = _T("Max-LSL");	
				strCaption.Format(_T("%s-%s"), strMax, strLSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
				strValue.Format(_T("%.0f"), sLimit.sMaxDist.fLower);	
				strReading.Format(_T("%.0f"), sMaxDist.fDistance);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_TO_DIE_DIST_MAX_LSL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_MaxDist )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);

				strCaption = _T("Min-USL");	
				strCaption.Format(_T("%s-%s"), strMin, strUSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
				strValue.Format(_T("%.0f"), sLimit.sMinDist.fUpper);	
				strReading.Format(_T("%.0f"), sMinDist.fDistance);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_TO_DIE_DIST_MIN_USL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_MinDist )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);

				strCaption = _T("Min-LSL");	
				strCaption.Format(_T("%s-%s"), strMin, strLSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
				strValue.Format(_T("%.0f"), sLimit.sMinDist.fLower);	
				strReading.Format(_T("%.0f"), sMinDist.fDistance);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_TO_DIE_DIST_MIN_LSL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_MinDist )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);
			
				strCaption = _T("Dif-USL");	
				strCaption.Format(_T("%s-%s"), strDif, strUSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);				
				strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fUpper);	
				strReading.Format(_T("%.0f"), fDist_MaxMin);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_TO_DIE_DIST_DIF_USL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_Diff_MaxMin )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);

				strCaption = _T("Dif-LSL");	
				strCaption.Format(_T("%s-%s"), strDif, strLSL);
				//strCaption = LoadMultiLanguageString(strCaption, strCaption);
				strValue.Format(_T("%.0f"), sLimit.sDiff_MaxMin.fLower);	
				strReading.Format(_T("%.0f"), fDist_MaxMin);
				pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
				if ( NULL == pParamItem ) { return false; }	
				pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_RESULT_THERMAL_TO_DIE_DIST_DIF_LSL);
				pParamItem->AllowEdit(FALSE);
				pParamItem->SetReading(strReading);
				if ( true == sRoiInfo.bResult_Diff_MaxMin )
				{	pParamItem->SetReadingTextColor(clrOK); }
				else
				{	pParamItem->SetReadingTextColor(clrNG); }
				pGroupItem->AddSubItem(pParamItem);
			}
		}
	}
	//進階設定
	strCaption = _T("Setup");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("");
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_SETUP_BTN);		
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
#endif//ALG_MEASURE_BLACK_GLUE_USE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamList_MeasureBlackGlueWnd(CAOIWnd *WndPtr)
{
#ifdef ALG_MEASURE_BLACK_GLUE_USE
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	if ( NULL == WndPtr ) { return false; }	

	const bool bExtend = false;
	std::vector<TUNI_FRAME> WndUniFrameList;
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();	
	TALG_PARAM_MEASURE_BLACK_GLUE &bgParam= AlgParam.GetAlgParamMeasureBlackGlue();
	const unsigned int FrameIndex = bgParam.bgFrameIndex1;
	const unsigned int FrameIndex2= bgParam.bgFrameIndex2;
	const unsigned int FrameIndex3= bgParam.bgFrameIndex3;
	const unsigned int FrameIndex4= bgParam.bgFrameIndex4;
	const double ResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(CameraID);
	//if ( AOIDataCollect.CreateWndUniFrameListByField(bExtend, WndPtr, WndUniFrameList) == false ) 
	if ( AOIDataCollect.CreateWndUniFrameListByModel(bExtend, WndPtr, WndUniFrameList) == false )
	{	return false;	}
	const size_t WndUniFrameCount = WndUniFrameList.size();
	if ( FrameIndex>=WndUniFrameCount || FrameIndex2>=WndUniFrameCount || FrameIndex3>=WndUniFrameCount || FrameIndex4>=WndUniFrameCount )
	{
		JetAPI::ClearUniFrameList(WndUniFrameList);
		return false; 
	}	

	CString str;
	CString AppName;
	CString TempFolder;
	CString strGlue;
	CString strBoard;
	CString strThermal;
	CString strCoating;
	CString strParam;
	CString strOutput;
	CString strFolder;
	CString KeyName = _T("MeasurementBlackGlue");
	std::string sPath, sParam, sParamOut;
	const bool bUseAOIFolder = false;
	JET::alg::MeasurementBlackGlue sBG;
	TUNI_FRAME UniFrameGlue=WndUniFrameList[FrameIndex];
	TUNI_FRAME UniFrameBoard=WndUniFrameList[FrameIndex2];
	TUNI_FRAME UniFrameThermal=WndUniFrameList[FrameIndex3];	
	TUNI_FRAME UniFrameCoating=WndUniFrameList[FrameIndex4];	

	bgParam.bgParam.fResolutionX = (float)(ResX);
	bgParam.bgParam.fResolutionY = (float)(ResY);
	strFolder = AOIDataCollect.GetAOITempDirectory();
	if ( true == bUseAOIFolder )
	{	strFolder = AOIDataCollect.GetAOIDirectory();	}
	TempFolder.Format(_T("%s\\%s"), strFolder, KeyName);
	strGlue.Format(_T("%s\\%s"), TempFolder, _T("Glue.PNG"));
	strBoard.Format(_T("%s\\%s"), TempFolder, _T("Board.PNG"));	
	strThermal.Format(_T("%s\\%s"), TempFolder, _T("Thermal.PNG"));		
	strCoating.Format(_T("%s\\%s"), TempFolder, _T("Coating.PNG"));		
	AppName.Format(_T("%s\\%s\\%s"), AOIDataCollect.GetAOIDirectory(), KeyName, _T("MeasurementBlackGlue.exe"));	

	if ( false == bUseAOIFolder )
	{
		sParam = "Param";
		sParamOut = "ParamOutput";
		::CreateDirectory(TempFolder, NULL);	
		JetAPI::ClearFolder(TempFolder);
	}
	else
	{
		sParam = "Parameter";
		sParamOut = "Parameter";
		AppName.Format(_T("%s\\%s\\%s"), AOIDataCollect.GetAOIDirectory(), KeyName, _T("JET-ShowImageTool.exe"));
	}
	JetAPI::TCHAR2string(TempFolder, sPath);
	strParam.Format(_T("%s\\%s.TXT"), TempFolder, CString(sParam.c_str()));		
	strOutput.Format(_T("%s\\%s.TXT"), TempFolder, CString(sParamOut.c_str()));	

	if ( sBG.Save_Parameter(bgParam.bgParam, sPath+"\\"+sParam) == false ||
		 ImageAPI.SaveImage(strGlue, UniFrameGlue, true) == false ||
		 ImageAPI.SaveImage(strBoard, UniFrameBoard, true) == false ||
		 ImageAPI.SaveImage(strThermal, UniFrameThermal, true) == false ||
		 ImageAPI.SaveImage(strCoating, UniFrameCoating, true) == false )
	{
		JetAPI::ClearUniFrameList(WndUniFrameList);
		return false;
	}	

	CString AppParam;
	time_t TimeModified=0;	
	JetAPI::GetFileModifedTime(strParam, TimeModified);//呼叫前的參數檔修改時間

	AppParam.Format(_T("%s %d"), TempFolder, 0);//0為量測黑膠
	bool bSucc = JetAPI::CallExecApp(NULL, NULL, AppName, AppParam, NULL, SW_SHOW, true);
	JetAPI::ClearUniFrameList(WndUniFrameList);	
	if ( false == bSucc )
	{	return false; }

	time_t TimeModified2=0;	
	JetAPI::GetFileModifedTime(strOutput, TimeModified2);//呼叫後的參數檔修改時間
	if ( TimeModified2 <= (TimeModified+1) )
	{	return false; }

	if ( sBG.Load_Parameter(sPath+"\\"+sParamOut, bgParam.bgParam) == false )
	{	
		CString Err=sBG.GetErrorMessage().c_str();
		JetAPI::ShowMessageBox(Err);
		return false;	
	}
	bgParam.bgParam.fResolutionX = (float)(ResX);
	bgParam.bgParam.fResolutionY = (float)(ResY);
	WndPtr->SetWndUIUpated_Param(false);	

	TWND_PARAM_CHANGED_RESULT Changed;
	Changed.bParamChanged = true;
	ExecWndParamChangedUpdate(ModelPtr, WndPtr, Changed);	
	CWnd::PostMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_REBUILD_WND_PARAM_LIST, NULL);
#endif//ALG_MEASURE_BLACK_GLUE_USE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_MeasureBlackGlue(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
#ifdef ALG_MEASURE_BLACK_GLUE_USE
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam &ImageBinParam = AlgParam.GetAlgImageBinParam();
	TALG_PARAM_MEASURE_BLACK_GLUE   &bgParam  = AlgParam.GetAlgParamMeasureBlackGlue();

	const COLORREF  clrOK = m_clrOK;
	const COLORREF  clrNG = m_clrNG;
	const COLORREF  clrUnTest = m_clrUnTest;

	bool          bIsPass = true;
	bool          bChanged = false;	
	unsigned int  FrameIndex = 0;
	unsigned int  FrameUniqueID = 0;
	TFrameParam  *FrameParamPtr = NULL;
	bool          bBoolParam = false;
	bool          AlgFrameIndexChanged=false;	
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
	case WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_FRAME_ID_01:
		strValue = pProp->GetValue();		
		FrameUniqueID = DecodeFrameUniqueID(strValue);				
		FrameIndex = Project->GetProjectFrameIndexByUniqueID(FrameUniqueID);
		FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);		
		if ( -1!= FrameIndex && NULL!=FrameParamPtr )
		{	
			bChanged = true;
			AlgFrameIndexChanged = true;
			bgParam.bgFrameIndex1 = FrameIndex;
			bgParam.bgFrameUniqueID1 = FrameUniqueID;
			Project->SetProjectMapIndex(FrameIndex);
			ImageBinParam.SetBinaryFrameIndex(FrameIndex);
			ImageBinParam.SetBinaryFrameUniqueID(FrameUniqueID);
		}		
		break;
	case WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_FRAME_ID_02:		
		strValue = pProp->GetValue();		
		FrameUniqueID = DecodeFrameUniqueID(strValue);				
		FrameIndex = Project->GetProjectFrameIndexByUniqueID(FrameUniqueID);
		FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);		
		if ( -1!= FrameIndex && NULL!=FrameParamPtr )
		{	
			bChanged = true;
			AlgFrameIndexChanged = true;
			bgParam.bgFrameIndex2 = FrameIndex;
			bgParam.bgFrameUniqueID2 = FrameUniqueID;
			Project->SetProjectMapIndex(FrameIndex);
			ImageBinParam.SetBinaryFrameIndex(FrameIndex);
			ImageBinParam.SetBinaryFrameUniqueID(FrameUniqueID);
		}		
		break;
	case WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_FRAME_ID_03:
		strValue = pProp->GetValue();		
		FrameUniqueID = DecodeFrameUniqueID(strValue);				
		FrameIndex = Project->GetProjectFrameIndexByUniqueID(FrameUniqueID);
		FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);		
		if ( -1!= FrameIndex && NULL!=FrameParamPtr )
		{	
			bChanged = true;
			AlgFrameIndexChanged = true;
			bgParam.bgFrameIndex3 = FrameIndex;
			bgParam.bgFrameUniqueID3 = FrameUniqueID;
			Project->SetProjectMapIndex(FrameIndex);
			ImageBinParam.SetBinaryFrameIndex(FrameIndex);
			ImageBinParam.SetBinaryFrameUniqueID(FrameUniqueID);
		}
		break;
	case WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_FRAME_ID_04:
		strValue = pProp->GetValue();		
		FrameUniqueID = DecodeFrameUniqueID(strValue);				
		FrameIndex = Project->GetProjectFrameIndexByUniqueID(FrameUniqueID);
		FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);		
		if ( -1!= FrameIndex && NULL!=FrameParamPtr )
		{	
			bChanged = true;
			AlgFrameIndexChanged = true;
			bgParam.bgFrameIndex4 = FrameIndex;
			bgParam.bgFrameUniqueID4 = FrameUniqueID;
			Project->SetProjectMapIndex(FrameIndex);
			ImageBinParam.SetBinaryFrameIndex(FrameIndex);
			ImageBinParam.SetBinaryFrameUniqueID(FrameUniqueID);
		}
		break;
	case WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_SETUP_BTN:
		if ( ExecWndParamList_MeasureBlackGlueWnd(WndPtr) == true ) 
		{
			bChanged = true;
			//bReBuildWndUI = true;
		}
		break;
	case WND_ALG_PROPERTY_MEASURE_BLACK_GLUE_END:
		break;
	}
	
	if ( false == bBoolParam  )
	{	strNewValue = pProp->GetValue();	}
	Changed.sValueName = strName;
	Changed.sValueOld = strOldValue;
	Changed.sValueNew = strNewValue;	

	Changed.bParamChanged = bChanged;		
	Changed.bFrameIndexChagned = AlgFrameIndexChanged;
#endif//ALG_MEASURE_BLACK_GLUE_USE
	return true;
}
//-------------------------------------------------------------------------------------//