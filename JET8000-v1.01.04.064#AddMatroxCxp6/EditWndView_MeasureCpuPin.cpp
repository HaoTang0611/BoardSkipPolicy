// EditWndView_MeasurementCpuPin.cpp : implementation file
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
bool CEditWndView::BuildWndParamList_MeasureCpuPin(CAOIProject *Project, CAOIWnd *WndPtr)
{
#ifdef ALG_MEASURE_CPU_PIN_USE
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == Project ) { return false; }	
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	CString    AlgTypeText = WndPtr->GetWndAlgTypeText();
	unsigned int WndIndex = WndPtr->GetWndIndex();
	const int  WndGroupID = WndPtr->GetWndGroupID();	
	const double WndInspectedTime = WndPtr->GetWndInspectedTime();
	const WND_LOGIC_TYPE    WndLogicType = WndPtr->GetWndLogicType();		
	const TALG_PARAM_MEASURE_CPU_PIN &cpParam = AlgParam.GetAlgParamMeasureCpuPin();
	
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
	strCaption = _T("CPU Frame");//Glue
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	pParamItem = CreateGridPropertyWndFrameList(strCaption, Project, WndPtr, cpParam.cpFrameUniqueID1, strDescr);
	if ( NULL == pParamItem ) { return false; }
	pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_CPU_PIN_FRAME_ID_01);
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
	
	const auto &sParam = cpParam.cpParam;
	const auto &sResult = cpParam.cpResult;
	
	//進階設定
	strCaption = _T("Setup");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strValue = _T("");
	pParamItem = new CJETPropertyGridProperty(strCaption, strValue, strDescr, (DWORD_PTR)WndPtr);	
	if ( NULL == pParamItem ) { return false; }	
	pParamItem->SetID(WND_ALG_PROPERTY_MEASURE_CPU_PIN_SETUP_BTN);		
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
#endif//ALG_MEASURE_CPU_PIN_USE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamList_MeasureCpuPinWnd(CAOIWnd *WndPtr)
{
#ifdef ALG_MEASURE_CPU_PIN_USE
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	if ( NULL == WndPtr ) { return false; }	

	const bool bExtend = false;	
	std::vector<TUNI_FRAME> WndUniFrameList;
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();		
	TALG_PARAM_MEASURE_CPU_PIN &cpParam= AlgParam.GetAlgParamMeasureCpuPin();
	const unsigned int FrameIndex = cpParam.cpFrameIndex1;	
	const double ResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(CameraID);	
	if ( AOIDataCollect.CreateWndUniFrameListByModel(bExtend, WndPtr, WndUniFrameList) == false )
	{	return false;	}
	const size_t WndUniFrameCount = WndUniFrameList.size();
	if ( FrameIndex >= WndUniFrameCount )
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
	CString KeyName = _T("MeasurementCpuPin");
	std::string sPath, sParam, sParamOut;	
	CPU_Alignment CpuAlignment;
	TUNI_FRAME UniFrameGlue=WndUniFrameList[FrameIndex];	
	
	cpParam.cpParam.fResolutionX = (float)(ResX);
	cpParam.cpParam.fResolutionY = (float)(ResY);
	strFolder = AOIDataCollect.GetAOITempDirectory();	
	TempFolder.Format(_T("%s\\%s"), strFolder, KeyName);
	strGlue.Format(_T("%s\\%s"), TempFolder, _T("Glue.PNG"));	
	//AppName.Format(_T("%s\\%s\\%s"), AOIDataCollect.GetAOIDirectory(), KeyName, _T("MeasurementBlackGlue.exe"));	
	AppName.Format(_T("%s\\%s\\%s"), AOIDataCollect.GetAOIDirectory(), _T("MeasurementBlackGlue"), _T("MeasurementBlackGlue.exe"));		
	
	sParam = "Param";
	sParamOut = "Param";
	//sParamOut = "ParamOutput";
	::CreateDirectory(TempFolder, NULL);	
	JetAPI::ClearFolder(TempFolder);
	
	JetAPI::TCHAR2string(TempFolder, sPath);
	strParam.Format(_T("%s\\%s.TXT"), TempFolder, CString(sParam.c_str()));		
	strOutput.Format(_T("%s\\%s.TXT"), TempFolder, CString(sParamOut.c_str()));	
	
	if ( CpuAlignment.Save_Parameter(cpParam.cpParam, sPath+"\\"+sParam) == false ||
		 ImageAPI.SaveImage(strGlue, UniFrameGlue, true) == false )
	{
		JetAPI::ClearUniFrameList(WndUniFrameList);
		return false;
	}	

	CString AppParam;
	time_t TimeModified=0;	
	JetAPI::GetFileModifedTime(strParam, TimeModified);//呼叫前的參數檔修改時間

	AppParam.Format(_T("%s %d"), TempFolder, 5);//4為量測CpuPin//ALGORITHM_PIN_DETECT
	bool bSucc = JetAPI::CallExecApp(NULL, NULL, AppName, AppParam, NULL, SW_SHOW, true);
	JetAPI::ClearUniFrameList(WndUniFrameList);	
	if ( false == bSucc )
	{	return false; }

	time_t TimeModified2=0;	
	JetAPI::GetFileModifedTime(strOutput, TimeModified2);//呼叫後的參數檔修改時間
	if ( TimeModified2 <= (TimeModified+1) )
	{	return false; }

	if ( CpuAlignment.Load_Parameter(sPath+"\\"+sParamOut, cpParam.cpParam) == false )
	{	
		CString Err;
		CpuAlignment.GetErrorMessage(Err);
		JetAPI::ShowMessageBox(Err);
		return false;	
	}
	cpParam.cpParam.fResolutionX = (float)(ResX);
	cpParam.cpParam.fResolutionY = (float)(ResY);
	WndPtr->SetWndUIUpated_Param(false);	

	TWND_PARAM_CHANGED_RESULT Changed;
	Changed.bParamChanged = true;
	ExecWndParamChangedUpdate(ModelPtr, WndPtr, Changed);	
	CWnd::PostMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_REBUILD_WND_PARAM_LIST, NULL);
#endif//ALG_MEASURE_CPU_PIN_USE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditWndView::ExecWndParamListChanged_MeasureCpuPin(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed)
{
#ifdef ALG_MEASURE_CPU_PIN_USE
	if ( NULL == pProp ) { return false; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIWnd      *WndPtr = (CAOIWnd*)(pProp->GetData());
	if ( NULL == WndPtr ) { return false; }
	CAMERA_ID    CameraID = PRIMARY_CAMERA_ID;
	CAlgParam   &AlgParam = WndPtr->GetWndAlgParam();
	CAlgBinaryParam &ImageBinParam = AlgParam.GetAlgImageBinParam();
	TALG_PARAM_MEASURE_CPU_PIN   &cpParam  = AlgParam.GetAlgParamMeasureCpuPin();

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
	case WND_ALG_PROPERTY_MEASURE_CPU_PIN_FRAME_ID_01:
		strValue = pProp->GetValue();		
		FrameUniqueID = DecodeFrameUniqueID(strValue);				
		FrameIndex = Project->GetProjectFrameIndexByUniqueID(FrameUniqueID);
		FrameParamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);	
		if ( -1!= FrameIndex && NULL!=FrameParamPtr )
		{	
			bChanged = true;
			AlgFrameIndexChanged = true;
			cpParam.cpFrameIndex1 = FrameIndex;
			cpParam.cpFrameUniqueID1 = FrameUniqueID;			
			Project->SetProjectMapIndex(FrameIndex);
			ImageBinParam.SetBinaryFrameIndex(FrameIndex);
			ImageBinParam.SetBinaryFrameUniqueID(FrameUniqueID);
		}			
		break;	
	case WND_ALG_PROPERTY_MEASURE_CPU_PIN_SETUP_BTN:
		if ( ExecWndParamList_MeasureCpuPinWnd(WndPtr) == true ) 
		{
			bChanged = true;
			//bReBuildWndUI = true;
		}
		break;
	case WND_ALG_PROPERTY_MEASURE_CPU_PIN_END:
		break;
	}
	
	if ( false == bBoolParam  )
	{	strNewValue = pProp->GetValue();	}
	Changed.sValueName = strName;
	Changed.sValueOld = strOldValue;
	Changed.sValueNew = strNewValue;	

	Changed.bParamChanged = bChanged;		
	Changed.bFrameIndexChagned = AlgFrameIndexChanged;
#endif//ALG_MEASURE_CPU_PIN_USE
	return true;
}
//-------------------------------------------------------------------------------------//