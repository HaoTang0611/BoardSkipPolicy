// AlgBarcodeRecognizeWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "SmartChartStaticWnd.h"
#include <thread>
//-------------------------------------------------------------------------------------//
#include "WndAlgPropertyDef.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgBarcodeRecognizeWnd dialog
//-------------------------------------------------------------------------------------//
CStaticChartWnd::CStaticChartWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CStaticChartWnd::IDD, pParent)
{
	smartChartType = 0;
	algType = 0;
	s = NULL;
	m_StopParamListBeSelected = false;
}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlgBarcodeRecognizeWnd)				
	DDX_Control(pDX, ALGBAR_PARAM_COMBO, m_ComboxCtrl);	
	DDX_Control(pDX, ALGBAR_PARAM_LIST_WND, m_ParamListCtrl);
	DDX_Control(pDX, ALGBAR_IMAGE_WND, m_ImageWnd);
	DDX_Control(pDX, ALGBAR_BARCODE_RECOGNIZE_BTN, m_BtnCtrl);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CStaticChartWnd, CDialog)
	//{{AFX_MSG_MAP(CAlgBarcodeRecognizeWnd)
	ON_WM_DESTROY()
 	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
//	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
//	ON_BN_CLICKED(ALGBAR_LOAD_IMAGE_BTN, OnLoadImageBtn)
	ON_BN_CLICKED(ALGBAR_BARCODE_RECOGNIZE_BTN, OnClearBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, ALGBAR_PARAM_LIST_WND, OnItemchangedParamListWnd)
//	ON_NOTIFY(NM_DBLCLK, ALGBAR_PARAM_LIST_WND, OnDblclkParamListWnd)
	ON_CBN_SELCHANGE(ALGBAR_PARAM_COMBO, OnSelchangeParamCombo)
//	ON_CBN_KILLFOCUS(ALGBAR_PARAM_COMBO, OnKillfocusParamCombo)
//	ON_BN_CLICKED(ALGBAR_SAVE_IMAGE_BTN, OnSaveImageBtn)
	//}}AFX_MSG_MAP
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
//	ON_WM_SETFOCUS()
	
ON_STN_DBLCLK(ALGBAR_IMAGE_WND, &CStaticChartWnd::OnStnDblclickImageWnd)
ON_NOTIFY(NM_DBLCLK, ALGBAR_PARAM_LIST_WND, &CStaticChartWnd::OnNMDblclkParamListWnd)
ON_NOTIFY(NM_DBLCLK, ALGBAR_PARAM_LIST_WND, &CStaticChartWnd::OnNMDblclkParamListWnd)
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgBarcodeRecognizeWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CStaticChartWnd::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	//CWnd::ShowWindow(SW_SHOWMAXIMIZED);
	//m_BkColor = 0x000000;
	//m_ImageMemDC.CreateMemDC(&m_ImageWnd, m_BkColor);

	JetAPI::InitialListCtrl(m_ParamListCtrl);	
	SwitchMultiLanguage();
	BuildParamListWndHeader();
	//BuildParamListWnd();
	JetAPI::ClearCombox(m_ComboxCtrl);
	
	obj = NULL;
	RECT WndRect = { 0,0,0,0 };
	m_ImageWnd.GetClientRect(&WndRect);
	s = CSmartChart_Agent::CreateEmbeddedSmartChart(m_ImageWnd, WndRect);
	obj = new CSmartChart_Agent(s, this->GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
int CStaticChartWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;



	return 0;
}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::OnDestroy()
{
	//::SendMessage(s, WM_CLOSE, NULL, NULL);
	ClearChart();
	if (obj) {
		delete obj;
		obj = nullptr;
	}
	CDialog::OnDestroy();
}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	BuildCombox();
	if (smartChartType != 2)//讀檔不需要
		ExecDataStatic();
	BuildParamListWnd();
	CDialog::OnShowWindow(bShow, nStatus);
}
//-------------------------------------------------------------------------------------//
//LRESULT CAlgHeightRecognizeWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	// TODO: Add your specialized code here and/or call the base class
//	
//	return CDialog::WindowProc(message, wParam, lParam);
//}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::SwitchMultiLanguage()
{
	//int     i = 0;
	int     WndID = 0;
	CString WndKey;
	//CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_SMART_CHART_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_SMART_CHART_WND;
	WndKey = _T("IDD_SMART_CHART_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetWindowText(NewLabelText);
	m_WndTitle = NewLabelText;
	//---------------------------------------------------------------------------------//
	WndID = IDOK;
	NewLabelText = AOIDataDefine.GetWndOKText();
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IDCANCEL;
	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = ALGBAR_BARCODE_RECOGNIZE_BTN;
	WndKey = _T("SMART_CHART_CLEAR_BTN");
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
}
//-------------------------------------------------------------------------------------//
CString CStaticChartWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default, int viewID)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_WND_VIEW");
	if (viewID == 0)
		Section = _T("IDD_EDIT_WND_VIEW");
	else
		Section = _T("IDD_ALG_BARCODE_RECOGNIZE_WND");
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::SetProject(CAOIProject* model)
{
	m_ProjectPtr = model;
	return;
}
//-------------------------------------------------------------------------------------//
bool CStaticChartWnd::SetWndPtr(vector<CAOIWnd*> WndPtr, CAOIWnd* aWndPtr)
{
	m_WndPtr = WndPtr;
	m_ActiveWndPtr = aWndPtr;
	return true;
}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::SetComponent(vector<CAOIComponent*> component)
{
	m_Component = component;
	return;
}
//-------------------------------------------------------------------------------------//
//CParamUni*  CStaticChartWnd::GetActParamUni()
//{
//	return m_ParamActPtr;
//}
////-------------------------------------------------------------------------------------//
//void CStaticChartWnd::SetActParamUni(CParamUni *Ptr)
//{
//	m_ParamActPtr = Ptr;
//}
//-------------------------------------------------------------------------------------//
bool CStaticChartWnd::BuildCombox()
{
	int       i=0;
	CString   str;
	CString   strCaption;	
	int       nItem=0;
	WND_ALG_PROPERTY_ID ParamID;	

	const CAlgParam &AlgParam = m_ActiveWndPtr->GetWndAlgParam();
	const TALG_PARAM_BRIGHT_RATIO &brParam = AlgParam.GetAlgParamBrightRatio();
	const TALG_PARAM_OUTER_SHORT &osParam = AlgParam.GetAlgParamOuterShort();
	const TALG_PARAM_OBJECT_MEASURE &omParam = AlgParam.GetAlgParamObjectMeasure();
	const TALG_PARAM_GROUP_COMPARE &gcParam = AlgParam.GetAlgParamGroupCompare();
	const TALG_PARAM_IPC_PRODUCT &ipcParam = AlgParam.GetAlgParamIPC();
	const TALG_PARAM_WIRE_WIDTH  &wireWidthParm = AlgParam.GetAlgParamWireWidth();

	switch (m_ActiveWndPtr->GetWndAlgType()) {
	case ALG_BRIGHT_RATIO:
		if (brParam.brToleranceEnabled)
		{
			str = _T("Tolerance");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 3);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 3);
		}
		if (brParam.brRatioEnabled)
		{
			str = _T("Ratio");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 0);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 0);
		}

		if (gcParam.gc3DHeightEnabled)
		{
			str = _T("Height Compare");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 1);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 1);
		}
		if (gcParam.gcTiltAngleEnabled)
		{
			str = _T("Tilt Angle");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 2);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 2);
		}
		if (brParam.brRangeEnabled)
		{
			str = _T("Range");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 4);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 4);
		}
		if (brParam.brContrastEnabled)
		{
			str = _T("Contrast");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 5);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 5);
		}
		if (brParam.brXLineEnabled)
		{
			str = _T("X Through");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 6);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 6);
		}
		if (brParam.brYLineEnabled)
		{
			str = _T("Y Through");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 7);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 7);
		}
		break; 
	case ALG_OUTER_SHORT:

		if (osParam.osLine_R.olEnabled)
		{
			str = _T("Right Side");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 10);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 10);
		}
		if (osParam.osLine_T.olEnabled)
		{
			str = _T("Top Side");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 11);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 11);
		}
		if (osParam.osLine_L.olEnabled)
		{
			str = _T("Left Side");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 12);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 12);
		}
		if (osParam.osLine_B.olEnabled)
		{
			str = _T("Bottom Side");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 13);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 13);
		}
		break;
	case ALG_OBJECT_MEASURE:
		if (AlgParam.GetAlgOffsetXEnabled())
		{
			str = _T("X USL");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 100);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 100);
		}
		if (AlgParam.GetAlgOffsetYEnabled())
		{
			str = _T("Y USL");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 101);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 101);
		}
		if (AlgParam.GetAlgSkewEnabled())
		{
			str = _T("Angle USL");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 102);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 102);
		}
		if (omParam.omHeightEnabled)
		{
			str = _T("Height");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 103);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 103);
		}
		if (omParam.omAreaEnabled)
		{
			str = _T("Area");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 104);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 104);
		}
		if (omParam.omVolumeEnabled)
		{
			str = _T("Volume");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 105);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 105);
		}
		break;
	case ALG_MODEL_MATCH:
	case ALG_IMAGE_MATCH:
		if (AlgParam.GetAlgOffsetXEnabled())
		{//GetAlgOffsetXReading
			str = _T("X USL");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 100);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 100);
		}
		if (AlgParam.GetAlgOffsetYEnabled())
		{
			str = _T("Y USL");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 101);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 101);
		}
		if (AlgParam.GetAlgSkewEnabled())
		{
			str = _T("Angle USL");
			strCaption = LoadMultiLanguageString(str, str, 0);
			m_ComboxCtrl.InsertString(-1, strCaption);
			m_ComboxCtrl.SetItemData(i++, 102);
			if (nItem++ == 0)
				JetAPI::SetComboxCurSel(m_ComboxCtrl, 102);
		}
		break;
	case ALG_WIDTH_RATIO:
		strCaption = m_WndPtr[0]->GetWndAlgTypeText();
		m_ComboxCtrl.InsertString(-1, strCaption);
		m_ComboxCtrl.SetItemData(0, 110);
		JetAPI::SetComboxCurSel(m_ComboxCtrl, 110);
		break;
	case ALG_HEIGHT:
		str = _T("3D Detect");
		strCaption = LoadMultiLanguageString(str, str, 0);
		m_ComboxCtrl.InsertString(-1, strCaption);
		m_ComboxCtrl.SetItemData(0, 200);
		JetAPI::SetComboxCurSel(m_ComboxCtrl, 200);
		break;
	case ALG_WIRE_WIDTH:
		str = _T("Wire Detect");
		strCaption = LoadMultiLanguageString(str, str, 0);
		m_ComboxCtrl.InsertString(-1, strCaption);
		m_ComboxCtrl.SetItemData(0, 201);
		JetAPI::SetComboxCurSel(m_ComboxCtrl, 201);
		break;
	case ALG_MEASURE_BLACK_GLUE:
		break;
	case ALG_MEASURE_FLUX_AREA:
		break;
	case ALG_MEASURE_CPU_PIN:
		break;
	}

	return true;
}
//-------------------------------------------------------------------------------------//
bool CStaticChartWnd::BuildParamListWnd()
{	
	CThisListCtrl_01 &ListCtrl = m_ParamListCtrl;	
	m_StopParamListBeSelected = true;
	JetAPI::ClearListCtrl(ListCtrl, FALSE);	
	m_StopParamListBeSelected = false;

	int           i=0;
	CString       str;
	CString       strValue;	
	CString       strCaption;	
	int           nItem=0;
	CParamUni    *ParamPtr=NULL;
	const int     nSubItem = 1;		
	const COLORREF rgbRed = 0x000000FF;
	const int ParamCount = (int)(m_Index.size()) - 1;
	m_ParamListIndex = -1;
	if (smartChartType == 2) { m_BtnCtrl.ShowWindow(SW_SHOW); }
	if (smartChartType != 0) { m_ParamListCtrl.ShowWindow(SW_HIDE); return true; }
	

	nItem=0;
	ListCtrl.SetRedraw(FALSE);
	m_StopParamListBeSelected = true;
	for ( i = 1; i < ParamCount; i++ )
	{
/*		ParamPtr = &(m_ParamList[i]);
		if ( NULL == ParamPtr ) { continue; }		
		
		ParamPtr->SetListCtrl(&ListCtrl);
		ParamPtr->SetItemIndex(nItem);
		ParamPtr->SetSubItemIndex(nSubItem);*/		

		strCaption.Format(_T("%.0f"), m_Index[i]);
		ListCtrl.InsertItem(nItem, strCaption);
		ListCtrl.SetItemData(nItem, i);
		ListCtrl.SetItemText(nItem, nSubItem, m_componenetName[i - 1].c_str());

		for (int j = 0; j < m_XIndexNG.size(); j++)
		{
			if (m_XIndexNG[j] == i)//NG Show Red
				ListCtrl.SetItemTextColor(nItem, rgbRed);
		}
		nItem ++;
	}
	
	m_StopParamListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CStaticChartWnd::BuildParamListWndHeader()
{
	CString str;
	int   nCol = 0;
	int width  = 64;
	int width2 = 64;
	RECT      Rect;	
	const int Align = LVCFMT_LEFT;//LVCFMT_CENTER;LVCFMT_RIGHT	
	CThisListCtrl_01 &ListCtrl = m_ParamListCtrl;
	ListCtrl.GetClientRect(&Rect);
	width = (Rect.right - Rect.left - 8) / 4;
	width2 = width*2;

	str = _T("");
	ListCtrl.InsertColumn(nCol, str, Align, width);
	nCol++;

	str = _T("Name");
	width2 = width * 8;
	ListCtrl.InsertColumn(nCol, str, Align, width2);
	nCol++;
	return true;	
}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::ClearChart()
{
	obj->ResetChart();
	//obj->ClearAllSeries();
	//if (s!=NULL)
	//	s = NULL;
	std::this_thread::sleep_for(std::chrono::milliseconds(15));
}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::DrawSmartChart()
{
	if (m_ImageWnd.GetSafeHwnd() == NULL) { return; }

	bool isVer = true;
	int count = m_Index.size();
	m_roundLength = 0;

	SSendData_Chart chart;
	auto& axisY = chart.m_stAxis[0];
	auto& axisX = chart.m_stAxis[1];
	::wcscpy(chart.m_stTitle.m_sTitleName, m_titleName.c_str());
	chart.m_bPanEnabled = true;
	chart.m_bZoomEnabled = true;

	axisY.m_bVisibled = true;//Y
	axisX.m_bVisibled = true;//X

	if (smartChartType == 0)
	{
		axisY.m_dMax = m_Max;
		axisY.m_dMin = m_Min;
		::wcscpy(axisY.m_sLabel_Name, m_labelame.c_str());
		axisY.m_bAutomatic = false;

		axisX.m_dMax = count;
		axisX.m_dMin = 0;
	}
	else if (smartChartType == 1)
	{
		SSendData_Serie_Bar serie(SC_T("test"));
		serie.color_SerieColor = RGB(100, 100, 100);
		serie.SetData(&m_readingData[0], m_readingData.size());

		if (algType == 0)
		{	//比例範圍
			axisX.m_dMin = 0;
			axisX.m_dMax = 150;
			axisX.m_bAutomatic = false;
		}
		else
		{
			axisX.m_dMax = m_Max;
			axisX.m_dMin = m_Min;
			axisX.m_bAutomatic = false;
		}
		axisY.m_dMin = 0;
		double minmax = *std::max_element(serie.data_PtrY, serie.data_PtrY + serie.data_Count);
		axisY.m_dMax = minmax + minmax*0.1;
		axisY.m_bAutomatic = false;
	}
	obj->SendData(&chart);

	int wid = 9;
	//obj->SendData互相穿插 畫圖會怪(click才有反應)
	if (smartChartType == 0)
	{
		if (m_readingDataGO.size() != 0)
		{
			SSendData_Serie_Point serie(SC_T("OK"));
			*serie.getPointSize() = wid;
			serie.color_SerieColor = RGB(0, 100, 0);
			serie.SetData(&m_XIndexOK[0], &m_readingDataGO[0], m_readingDataGO.size());
			obj->SendData(&serie);
		}

		if (m_readingDataNG.size() != 0)
		{
			SSendData_Serie_Point serie2(SC_T("NG"));
			*serie2.getPointSize() = wid;
			serie2.color_SerieColor = RGB(255, 0, 0);
			serie2.SetData(&m_XIndexNG[0], &m_readingDataNG[0], m_readingDataNG.size());
			obj->SendData(&serie2);
		}

		SSendData_DragLine DragAvg(SC_T("Avg"), true, false); //畫Avg
		DragAvg.color = RGB(0, 0, 0);
		DragAvg.m_dPosition = m_readingDataAVG;
		DragAvg.m_bEnableDrag = false;
		DragAvg.m_iNDotNum = 0;
		DragAvg.m_iLineWidth = 1;
		obj->SendData(&DragAvg);
	}
	else if (smartChartType == 1)
	{
		SSendData_Serie_Bar serie(SC_T("OK"));
		serie.color_SerieColor = RGB(100, 100, 100);
		serie.SetData(&m_readingData[0], m_readingData.size());
		obj->SendData(&serie);

		isVer = false;
	}
	else if (smartChartType == 2)
	{
		//製作檔案路徑
		std::wstring alg = JetAPI::GetComboxCurSelText(m_ComboxCtrl);
		//CString ModelFolder;
		std::wstring LibraryFolder;
		//JetAPI::ExtractMainFileName(m_ProjectPtr->GetProjectShowName(), ModelFolder);
		LibraryFolder = AOIDataCollect.GetAOIStaticDataDirectory();
		LibraryFolder.append(L"\\");//存project
		LibraryFolder.append(m_ProjectPtr->GetProjectFileMainName());
		LibraryFolder.append(L"\\");
		LibraryFolder.append(m_Component[0]->GetComponentModelName());
		LibraryFolder.append(L"\\");
		LibraryFolder.append(std::to_wstring(m_ActiveWndPtr->GetWndIndex()));
		LibraryFolder.append(L"_");
		LibraryFolder.append(alg);

		//if (!JetAPI::IsFileExist(LibraryFolder.c_str()))
		//{
		//	this->OnClose();
		//	this->OnDestroy();
		//	return;
		//}
		
		SSendData_Serie_Bar serie(alg.c_str());
		serie.LoadFile(LibraryFolder.c_str());
		
		obj->SendData(&serie);
		isVer = false;

		::ShowWindow(s, 5);
		return;
	}
	
	SSendData_DragLine Drag0(SC_T("0"), true, false); //畫0
	Drag0.color = RGB(0, 0, 0);
	Drag0.m_dPosition = 0;
	//Drag0.m_bDragDirection_Ver = true;
	Drag0.m_bEnableDrag = false;
	Drag0.m_iNDotNum = 0;
	Drag0.m_iLineWidth = 1;
	obj->SendData(&Drag0);

	if (m_LSL != 99999)
	{
		SSendData_DragLine DragLSL(SC_T("LSL"), isVer, false);
		DragLSL.color = RGB(255, 0, 0);
		DragLSL.m_dPosition = m_LSL;
		//DragLSL.m_bDragDirection_Ver = isVer;
		obj->SendData(&DragLSL);
	}

	if (m_USL != -99999)
	{
		SSendData_DragLine DragUSL(SC_T("USL"), isVer, false);
		DragUSL.color = RGB(0, 255, 0);
		DragUSL.m_dPosition = m_USL;
		switch (algType)
		{ 
			case 1:
			case 3:
			case 4:
			{
				DragUSL.m_iNDotNum = m_roundLength = 0;
				break;
			}
			case 100:
			case 101:
			{
				//DragUSL.m_dPosition_paired = -m_USL;
				DragUSL.PairLine_Inv(SC_T("USL2"), RGB(0, 255, 0), -m_USL);
				break;
			}
			case 102:
			{
				//DragUSL.m_dPosition_paired = -m_USL;
				DragUSL.PairLine_Inv(SC_T("USL2"), RGB(0, 255, 0), -m_USL);
				//DragUSL.m_DragLineType = EDragLineType::Pair_Inv;
				DragUSL.m_iNDotNum = m_roundLength = 1;
				break;
			}
			default:
			{
				DragUSL.m_iNDotNum = m_roundLength = 1;
				break;
			}
		}
		obj->SendData(&DragUSL);
		
	}	
	
	::ShowWindow(s, 5);
	return;
}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	DrawSmartChart();
	
	// Do not call CDialog::OnPaint() for painting messages
	//EndPaint();
}
//-------------------------------------------------------------------------------------//
bool CStaticChartWnd::ExecDataStatic()
{
	algType = JetAPI::GetComboxCurSelData(m_ComboxCtrl);

	CString   str;
	CString   strCaption;

	ALG_TYPE AlgType = m_WndPtr[0]->GetWndAlgType();
	CAlgParam *pAlgParam;
	TALG_PARAM_GROUP_COMPARE gcParam;
	TALG_PARAM_BRIGHT_RATIO brParam;
	TALG_PARAM_OUTER_SHORT osParam;
	TALG_PARAM_OBJECT_MEASURE omParam;
	TALG_PARAM_IPC_PRODUCT ipcParam;
	TALG_PARAM_RESIN_HEIGHT hightDetectParm;
	TALG_PARAM_WIRE_WIDTH  wireWidthParm;
	CAOIWnd *pWndPtr;
	double resultValue;
	m_readingDataAVG = 0;
	RESULT_ID resultID;

	m_Max = 0;
	m_Min = DBL_MAX;
	m_Sec = 0;

	m_readingData.clear();
	m_readingDataGO.clear();
	m_readingDataNG.clear();
	m_componenetName.clear();
	m_Index.clear();
	m_XIndexOK.clear();
	m_XIndexNG.clear();
	m_XBase.clear();
	for (int i = 0; i < m_Component.size(); i++)
	{
		//找上下限跑一次
		if (m_WndPtr[i] == m_ActiveWndPtr)
		{
			pAlgParam = &m_ActiveWndPtr->GetWndAlgParam();
			brActiveParam = &pAlgParam->GetAlgParamBrightRatio();
			osActiveParam = &pAlgParam->GetAlgParamOuterShort();
			omActiveParam = &pAlgParam->GetAlgParamObjectMeasure();
			ipcActiveParam = &pAlgParam->GetAlgParamIPC();
			gcActiveParam = &pAlgParam->GetAlgParamGroupCompare();
			hightDetectActiveParm = &pAlgParam->GetAlgParamResinHeight();
			wireWidthActiveParm = &pAlgParam->GetAlgParamWireWidth();
			switch (algType)
			{
			case 0:
				str = _T("Ratio USL");
				strCaption = LoadMultiLanguageString(str, str, 0);
				strCaption += _T(" & ");
				str = _T("Ratio LSL");
				strCaption += LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T(" ");
				m_LSL = brActiveParam->brRatioLSL;
				m_USL = brActiveParam->brRatioUSL;
				m_LSLPtr = &brActiveParam->brRatioLSL;
				m_USLPtr = &brActiveParam->brRatioUSL;
				break;
			case 1:
				str = _T("Height USL");
				strCaption = LoadMultiLanguageString(str, str, 0);
				strCaption += _T(" & ");
				str = _T("Height LSL");
				strCaption += LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T(" ");
				m_LSL = gcActiveParam->gc3DHeightLSL;
				m_USL = gcActiveParam->gc3DHeightUSL;
				m_LSLPtr = &gcActiveParam->gc3DHeightLSL;
				m_USLPtr = &gcActiveParam->gc3DHeightUSL;
				break;
			case 2:
				str = _T("Angle USL");
				strCaption = LoadMultiLanguageString(str, str, 0);
				strCaption += _T(" & ");
				str = _T("Angle LSL");
				strCaption += LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T("°");
				m_LSL = gcActiveParam->gcTiltAngleLSL;
				m_USL = gcActiveParam->gcTiltAngleUSL;
				m_LSLPtr = &gcActiveParam->gcTiltAngleLSL;
				m_USLPtr = &gcActiveParam->gcTiltAngleUSL;
				break;
			case 3:
				str = _T("Tol. USL");
				strCaption = LoadMultiLanguageString(str, str, 0);
				strCaption += _T(" & ");
				str = _T("Tol. LSL");
				strCaption += LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T(" ");
				m_LSL = brActiveParam->brToleranceLSL + brActiveParam->brTargetValue;
				m_USL = brActiveParam->brToleranceUSL + brActiveParam->brTargetValue;
				m_Sec = brActiveParam->brTargetValue;
				m_LSLPtr = &brActiveParam->brToleranceLSL;
				m_USLPtr = &brActiveParam->brToleranceUSL;
				break;
			case 4:
				str = _T("USL");
				strCaption = LoadMultiLanguageString(str, str, 0);
				strCaption += _T(" & ");
				str = _T("LSL");
				strCaption += LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T(" ");
				m_LSL = brActiveParam->brRangeLSL;
				m_USL = brActiveParam->brRangeUSL;
				m_LSLPtr = &brActiveParam->brRangeLSL;
				m_USLPtr = &brActiveParam->brRangeUSL;
				break;
			case 5:
				str = _T("USL");
				strCaption = LoadMultiLanguageString(str, str, 0);
				strCaption += _T(" & ");
				str = _T("LSL");
				strCaption += LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T(" ");
				m_LSL = brActiveParam->brContrastLSL;
				m_USL = brActiveParam->brContrastUSL;
				m_LSLPtr = &brActiveParam->brContrastLSL;
				m_USLPtr = &brActiveParam->brContrastUSL;
				break;
			case 6:
				str = _T("Ratio USL");
				strCaption = LoadMultiLanguageString(str, str, 0);
				strCaption += _T(" & ");
				str = _T("Ratio LSL");
				strCaption += LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T(" ");
				m_LSL = brActiveParam->brXLineLSL;
				m_USL = brActiveParam->brXLineUSL;
				m_LSLPtr = &brActiveParam->brXLineLSL;
				m_USLPtr = &brActiveParam->brXLineUSL;
				break;
			case 7:
				str = _T("Ratio USL");
				strCaption = LoadMultiLanguageString(str, str, 0);
				strCaption += _T(" & ");
				str = _T("Ratio LSL");
				strCaption += LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T(" ");
				m_LSL = brActiveParam->brYLineLSL;
				m_USL = brActiveParam->brYLineUSL;
				m_LSLPtr = &brActiveParam->brYLineLSL;
				m_USLPtr = &brActiveParam->brYLineUSL;
				break;
			case 10:
				str = _T("USL (%)");
				strCaption = LoadMultiLanguageString(str, str, 0);
				strCaption += _T(" & ");
				str = _T("LSL (%)");
				strCaption += LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T(" ");
				m_LSL = osActiveParam->osLine_R.olLSL;
				m_USL = osActiveParam->osLine_R.olUSL;
				m_LSLPtr = &osActiveParam->osLine_R.olLSL;
				m_USLPtr = &osActiveParam->osLine_R.olUSL;
				break;
			case 11:
				str = _T("USL (%)");
				strCaption = LoadMultiLanguageString(str, str, 0);
				strCaption += _T(" & ");
				str = _T("LSL (%)");
				strCaption += LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T(" ");
				m_LSL = osActiveParam->osLine_T.olLSL;
				m_USL = osActiveParam->osLine_T.olUSL;
				m_LSLPtr = &osActiveParam->osLine_T.olLSL;
				m_USLPtr = &osActiveParam->osLine_T.olUSL;
				break;
			case 12:
				str = _T("USL (%)");
				strCaption = LoadMultiLanguageString(str, str, 0);
				strCaption += _T(" & ");
				str = _T("LSL (%)");
				strCaption += LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T(" ");
				m_LSL = osActiveParam->osLine_L.olLSL;
				m_USL = osActiveParam->osLine_L.olUSL;
				m_LSLPtr = &osActiveParam->osLine_L.olLSL;
				m_USLPtr = &osActiveParam->osLine_L.olUSL;
				break;
			case 13:
				str = _T("USL (%)");
				strCaption = LoadMultiLanguageString(str, str, 0);
				strCaption += _T(" & ");
				str = _T("LSL (%)");
				strCaption += LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T(" ");
				m_LSL = osActiveParam->osLine_B.olLSL;
				m_USL = osActiveParam->osLine_B.olUSL;
				m_LSLPtr = &osActiveParam->osLine_B.olLSL;
				m_USLPtr = &osActiveParam->osLine_B.olUSL;
				break;
			case 100:
				str = _T("X USL");
				strCaption = LoadMultiLanguageString(str, str, 0);
				m_labelame = _T(" ");
				m_LSL = 99999;
				m_USL = pAlgParam->GetAlgOffsetXUSL();
				m_LSLPtr = NULL;
				m_USLPtr = NULL;
				break;
			case 101:
				str = _T("Y USL");
				strCaption = LoadMultiLanguageString(str, str, 0);
				m_labelame = _T(" ");
				m_LSL = 99999;
				m_USL = pAlgParam->GetAlgOffsetYUSL();
				m_LSLPtr = NULL;
				m_USLPtr = NULL;
				break;
			case 102:
				str = _T("Angle USL");
				strCaption = LoadMultiLanguageString(str, str, 0);
				m_labelame = _T(" ");
				m_LSL = 99999;
				m_USL = pAlgParam->GetAlgSkewUSL();
				m_LSLPtr = NULL;
				m_USLPtr = NULL;
				break;
			case 103:
				if (omActiveParam->omHeightCalcUnitMode == ALG_CALC_UNIT_DIFF)
				{
					str = _T("USL(um)");
					strCaption = LoadMultiLanguageString(str, str, 0);
					strCaption += _T(" & ");
					str = _T("LSL(um)");
					strCaption += LoadMultiLanguageString(str, str, 0);
					m_titleName = strCaption;
					m_labelame = _T("um");
					m_LSL = omActiveParam->omHeightSpec + omActiveParam->omHeightDiffLSL;
					m_USL = omActiveParam->omHeightSpec + omActiveParam->omHeightDiffUSL;
					m_Sec = omActiveParam->omHeightSpec;
					m_LSLPtr = &omActiveParam->omHeightDiffLSL;
					m_USLPtr = &omActiveParam->omHeightDiffUSL;
				}
				else
				{
					str = _T("USL(%)");
					strCaption = LoadMultiLanguageString(str, str, 0);
					strCaption += _T(" & ");
					str = _T("LSL(%)");
					strCaption += LoadMultiLanguageString(str, str, 0);
					m_titleName = strCaption;
					m_labelame = _T("%");
					m_LSL = omActiveParam->omHeightRatioLSL;
					m_USL = omActiveParam->omHeightRatioUSL;
					m_LSLPtr = &omActiveParam->omHeightRatioLSL;
					m_USLPtr = &omActiveParam->omHeightRatioUSL;
				}
				//str = _T("Parital USL");
				//strCaption = LoadMultiLanguageString(str, str, 0);
				//strCaption += _T(" & ");
				//str = _T("Parital LSL");
				//strCaption += LoadMultiLanguageString(str, str, 0);
				//m_titleName = strCaption;
				//m_labelame = _T(" ");
				//m_LSL = omParam.omHeightAveragePartialL;
				//m_USL = omParam.omHeightAveragePartialH;
				break;
			case 104:
				str = _T("USL(%)");
				strCaption = LoadMultiLanguageString(str, str, 0);
				strCaption += _T(" & ");
				str = _T("LSL(%)");
				strCaption += LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T("%");
				m_LSL = omActiveParam->omAreaLSL;
				m_USL = omActiveParam->omAreaUSL;
				m_LSLPtr = &omActiveParam->omAreaLSL;
				m_USLPtr = &omActiveParam->omAreaUSL;
				break;
			case 105:
				str = _T("USL(%)");
				strCaption = LoadMultiLanguageString(str, str, 0);
				strCaption += _T(" & ");
				str = _T("LSL(%)");
				strCaption += LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T("%");
				m_LSL = omActiveParam->omVolumeLSL;
				m_USL = omActiveParam->omVolumeUSL;
				m_LSLPtr = &omActiveParam->omVolumeLSL;
				m_USLPtr = &omActiveParam->omVolumeUSL;
				break;
			case 110:
				str = _T("Width Ratio");
				strCaption = LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T("%");
				m_LSL = ipcActiveParam->ipcWidthRatio;//顯示1，實際是0
				m_USL = -99999;
				m_LSLPtr = NULL;
				m_USLPtr = NULL;
				break;
			case 200:
				str = _T("Height Ratio");
				strCaption = LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T("um");
				m_LSL = hightDetectActiveParm->nPartHeightThreshold;
				m_USL = -99999;
				m_LSLPtr = NULL;
				m_USLPtr = NULL;
				break;
			case 201://wireWidthParm
				str = _T("Width USL");
				strCaption = LoadMultiLanguageString(str, str, 0);
				strCaption += _T(" & ");
				str = _T("Width LSL");
				strCaption += LoadMultiLanguageString(str, str, 0);
				m_titleName = strCaption;
				m_labelame = _T("");
				m_LSL = wireWidthActiveParm->widthLSL;
				m_USL = wireWidthActiveParm->widthUSL;
				m_LSLPtr = &wireWidthActiveParm->widthLSL;
				m_USLPtr = &wireWidthActiveParm->widthUSL;
				break;
			}
			break;
		}
		
		if (i == m_Component.size()) {
			return false; 
		}
	}

	m_Index.push_back(0);
	m_XBase.push_back(0);
	for (int i = 0; i < m_Component.size(); i++)
	{
		pAlgParam = &m_WndPtr[i]->GetWndAlgParam();
		brParam = pAlgParam->GetAlgParamBrightRatio();
		osParam = pAlgParam->GetAlgParamOuterShort();
		omParam = pAlgParam->GetAlgParamObjectMeasure();
		ipcParam = pAlgParam->GetAlgParamIPC();
		gcParam = pAlgParam->GetAlgParamGroupCompare();
		hightDetectParm = pAlgParam->GetAlgParamResinHeight();
		wireWidthParm = pAlgParam->GetAlgParamWireWidth();

		//分別取結果
		switch (algType)
		{
		case 0:
			resultValue = pAlgParam->GetAlgResultReading1();
			if (resultValue < m_LSL || resultValue > m_USL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 1:
			resultValue = gcParam.gc3DHeightReading;
			resultID = gcParam.gcResultID;
			break;
		case 2:
			resultValue = gcParam.gcTiltAngleReading;
			resultID = gcParam.gcResultID;
			break;
		case 3:
			resultValue = brParam.brTargetValue + brParam.brToleranceReading;
			if (resultValue < m_LSL || resultValue > m_USL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 4:
			resultValue = brParam.brRangeReading;
			if (resultValue < m_LSL || resultValue > m_USL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 5:
			resultValue = brParam.brContrastReading;
			if (resultValue < m_LSL || resultValue > m_USL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 6:
			resultValue = brParam.brXLineReading;
			if (resultValue < m_LSL || resultValue > m_USL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 7:
			resultValue = brParam.brYLineReading;
			if (resultValue < m_LSL || resultValue > m_USL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 10:
			resultValue = osParam.osLine_R.olReading;
			if (resultValue < m_LSL || resultValue > m_USL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 11:
			resultValue = osParam.osLine_T.olReading;
			if (resultValue < m_LSL || resultValue > m_USL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 12:
			resultValue = osParam.osLine_L.olReading;
			if (resultValue < m_LSL || resultValue > m_USL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 13:
			resultValue = osParam.osLine_B.olReading;
			if (resultValue < m_LSL || resultValue > m_USL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 100:
			resultValue = pAlgParam->GetAlgOffsetXReading();
			if (resultValue > m_USL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 101:
			resultValue = pAlgParam->GetAlgOffsetYReading();
			if (resultValue > m_USL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 102:
			resultValue = pAlgParam->GetAlgSkewReading();
			if (resultValue > m_USL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 103:
			if (omParam.omHeightCalcUnitMode == ALG_CALC_UNIT_DIFF)
			{
				resultValue = omParam.omHeightReading;
				if (resultValue < m_LSL || resultValue > m_USL)
					resultID = RESULT_ID_NG;
				else
					resultID = RESULT_ID_OK;
			}
			else
			{
				resultValue = (omParam.omHeightReading / omParam.omHeightSpec) * 100;
				if (resultValue < m_LSL || resultValue > m_USL)
					resultID = RESULT_ID_NG;
				else
					resultID = RESULT_ID_OK;
			}
			break;
		case 104:
			resultValue = (omParam.omAreaReading / omParam.omAreaSpec) * 100;
			if (resultValue < m_LSL || resultValue > m_USL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 105:
			resultValue = (omParam.omVolumeReading / omParam.omVolumeSpec) * 100;
			if (resultValue < m_LSL || resultValue > m_USL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 110:
			resultValue = ipcParam.ipcResult;
			if (resultValue < m_LSL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 200:
			resultValue = hightDetectParm.resultH;
			if (resultValue < m_LSL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		case 201:
			resultValue = wireWidthParm.fWidth;
			if (resultValue < m_LSL || resultValue > m_USL)
				resultID = RESULT_ID_NG;
			else
				resultID = RESULT_ID_OK;
			break;
		}

		//畫成兩條線
		m_readingData.push_back(resultValue);
		m_readingDataAVG += resultValue;
		if (resultID == RESULT_ID_OK)
		{
			m_readingDataGO.push_back(resultValue);
			m_XBase.push_back(0);
			m_componenetName.push_back(m_Component[i]->GetComponentName());
			m_XIndexOK.push_back(i + 1);
			m_Index.push_back(i + 1);
		}
		else if (resultID == RESULT_ID_NG)
		{
			m_XBase.push_back(0);
			m_readingDataNG.push_back(resultValue);
			m_componenetName.push_back(m_Component[i]->GetComponentName());
			m_XIndexNG.push_back(i + 1);
			m_Index.push_back(i + 1);
		}
		else
		{
			continue;
		}

		if (resultValue > m_Max)
			m_Max = resultValue;
		if (resultValue < m_Min)
			m_Min = resultValue;
	}
	m_readingDataAVG = m_readingDataAVG / m_readingData.size();
	m_Index.push_back(m_Component.size() + 1);
	m_XBase.push_back(0);
	
	if (m_USL > m_Max)
		m_Max = m_USL;
	if (m_LSL < m_Min)
		m_Min = m_LSL;
	if (m_USL == -99999)
		m_Max = std::max({ m_LSL, m_Max, m_Min });


	m_Max = m_Max*1.1;
	if (m_Min < 0)
		m_Min -= 10;
	else
		m_Min = m_Min*0.9;

	switch (algType)
	{
		case 100:
		case 101:
		case 102:
		{
			if (m_Max > std::abs(m_Min))
				m_Min = -m_Max;
			else
				m_Max = -m_Min;
			break;
		}
		default:
		{
			break;
		}

	}

	return true;
}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::UpdateWndParm()
{	//切換指標 等同ExecDataStatic前段
	ALG_TYPE AlgType = m_ActiveWndPtr->GetWndAlgType();
	CAlgParam *pAlgParam;
	TALG_PARAM_GROUP_COMPARE gcParam;
	TALG_PARAM_BRIGHT_RATIO brParam;
	TALG_PARAM_OUTER_SHORT osParam;
	TALG_PARAM_OBJECT_MEASURE omParam;
	TALG_PARAM_IPC_PRODUCT ipcParam;
	TALG_PARAM_RESIN_HEIGHT hightDetectParm;
	TALG_PARAM_WIRE_WIDTH  wireWidthParm;

	pAlgParam = &m_ActiveWndPtr->GetWndAlgParam();
	brActiveParam = &pAlgParam->GetAlgParamBrightRatio();
	osActiveParam = &pAlgParam->GetAlgParamOuterShort();
	omActiveParam = &pAlgParam->GetAlgParamObjectMeasure();
	ipcActiveParam = &pAlgParam->GetAlgParamIPC();
	gcActiveParam = &pAlgParam->GetAlgParamGroupCompare();
	hightDetectActiveParm = &pAlgParam->GetAlgParamResinHeight();
	wireWidthActiveParm = &pAlgParam->GetAlgParamWireWidth();
	switch (algType)
	{
	case 0:
		m_LSL = brActiveParam->brRatioLSL;
		m_USL = brActiveParam->brRatioUSL;
		m_LSLPtr = &brActiveParam->brRatioLSL;
		m_USLPtr = &brActiveParam->brRatioUSL;
		break;
	case 1:
		m_LSL = gcActiveParam->gc3DHeightLSL;
		m_USL = gcActiveParam->gc3DHeightUSL;
		m_LSLPtr = &gcActiveParam->gc3DHeightLSL;
		m_USLPtr = &gcActiveParam->gc3DHeightUSL;
		break;
	case 2:
		m_LSL = gcActiveParam->gcTiltAngleLSL;
		m_USL = gcActiveParam->gcTiltAngleUSL;
		m_LSLPtr = &gcActiveParam->gcTiltAngleLSL;
		m_USLPtr = &gcActiveParam->gcTiltAngleUSL;
		break;
	case 3:
		m_LSL = brActiveParam->brToleranceLSL + brActiveParam->brTargetValue;
		m_USL = brActiveParam->brToleranceUSL + brActiveParam->brTargetValue;
		m_Sec = brActiveParam->brTargetValue;
		m_LSLPtr = &brActiveParam->brToleranceLSL;
		m_USLPtr = &brActiveParam->brToleranceUSL;
		break;
	case 4:
		m_LSL = brActiveParam->brRangeLSL;
		m_USL = brActiveParam->brRangeUSL;
		m_LSLPtr = &brActiveParam->brRangeLSL;
		m_USLPtr = &brActiveParam->brRangeUSL;
		break;
	case 5:
		m_LSL = brActiveParam->brContrastLSL;
		m_USL = brActiveParam->brContrastUSL;
		m_LSLPtr = &brActiveParam->brContrastLSL;
		m_USLPtr = &brActiveParam->brContrastUSL;
		break;
	case 6:
		m_LSL = brActiveParam->brXLineLSL;
		m_USL = brActiveParam->brXLineUSL;
		m_LSLPtr = &brActiveParam->brXLineLSL;
		m_USLPtr = &brActiveParam->brXLineUSL;
		break;
	case 7:
		m_LSL = brActiveParam->brYLineLSL;
		m_USL = brActiveParam->brYLineUSL;
		m_LSLPtr = &brActiveParam->brYLineLSL;
		m_USLPtr = &brActiveParam->brYLineUSL;
		break;
	case 10:
		m_LSL = osActiveParam->osLine_R.olLSL;
		m_USL = osActiveParam->osLine_R.olUSL;
		m_LSLPtr = &osActiveParam->osLine_R.olLSL;
		m_USLPtr = &osActiveParam->osLine_R.olUSL;
		break;
	case 11:
		m_LSL = osActiveParam->osLine_T.olLSL;
		m_USL = osActiveParam->osLine_T.olUSL;
		m_LSLPtr = &osActiveParam->osLine_T.olLSL;
		m_USLPtr = &osActiveParam->osLine_T.olUSL;
		break;
	case 12:
		m_LSL = osActiveParam->osLine_L.olLSL;
		m_USL = osActiveParam->osLine_L.olUSL;
		m_LSLPtr = &osActiveParam->osLine_L.olLSL;
		m_USLPtr = &osActiveParam->osLine_L.olUSL;
		break;
	case 13:
		m_LSL = osActiveParam->osLine_B.olLSL;
		m_USL = osActiveParam->osLine_B.olUSL;
		m_LSLPtr = &osActiveParam->osLine_B.olLSL;
		m_USLPtr = &osActiveParam->osLine_B.olUSL;
		break;
	case 100:
		m_LSL = 99999;
		m_USL = pAlgParam->GetAlgOffsetXUSL();
		m_LSLPtr = NULL;
		m_USLPtr = NULL;
		break;
	case 101:
		m_LSL = 99999;
		m_USL = pAlgParam->GetAlgOffsetYUSL();
		m_LSLPtr = NULL;
		m_USLPtr = NULL;
		break;
	case 102:
		m_LSL = 99999;
		m_USL = pAlgParam->GetAlgSkewUSL();
		m_LSLPtr = NULL;
		m_USLPtr = NULL;
		break;
	case 103:
		if (omActiveParam->omHeightCalcUnitMode == ALG_CALC_UNIT_DIFF)
		{
			m_LSL = omActiveParam->omHeightSpec + omActiveParam->omHeightDiffLSL;
			m_USL = omActiveParam->omHeightSpec + omActiveParam->omHeightDiffUSL;
			m_Sec = omActiveParam->omHeightSpec;
			m_LSLPtr = &omActiveParam->omHeightDiffLSL;
			m_USLPtr = &omActiveParam->omHeightDiffUSL;
		}
		else
		{
			m_LSL = omActiveParam->omHeightRatioLSL;
			m_USL = omActiveParam->omHeightRatioUSL;
			m_LSLPtr = &omActiveParam->omHeightRatioLSL;
			m_USLPtr = &omActiveParam->omHeightRatioUSL;
		}
		break;
	case 104:
		m_LSL = omActiveParam->omAreaLSL;
		m_USL = omActiveParam->omAreaUSL;
		m_LSLPtr = &omActiveParam->omAreaLSL;
		m_USLPtr = &omActiveParam->omAreaUSL;
		break;
	case 105:
		m_LSL = omActiveParam->omVolumeLSL;
		m_USL = omActiveParam->omVolumeUSL;
		m_LSLPtr = &omActiveParam->omVolumeLSL;
		m_USLPtr = &omActiveParam->omVolumeUSL;
		break;
	case 110:
		m_LSL = ipcActiveParam->ipcWidthRatio;
		m_USL = -99999;
		m_LSLPtr = NULL;
		m_USLPtr = NULL;
		break;
	case 200:
		m_LSL = hightDetectActiveParm->nPartHeightThreshold;
		m_USL = -99999;
		m_LSLPtr = NULL;
		m_USLPtr = NULL;
		break;
	case 201:
		m_LSL = wireWidthActiveParm->widthLSL;
		m_USL = wireWidthActiveParm->widthUSL;
		m_LSLPtr = &wireWidthActiveParm->widthLSL;
		m_USLPtr = &wireWidthActiveParm->widthUSL;
		break;
	}	
}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::OnClearBtn()
{//Library底下jsc全刪
	std::wstring LibraryFolder;
	//::CreateDirectory(DesFileName, NULL);
	//JetAPI::IsFolderExist(CurrentFolder)
	CString str = _T("Clear All the Static Data(.jsc)?");
	str = LoadMultiLanguageString(str, str, 0);
	if (IDYES == JetAPI::ShowMessageBox(str, MB_YESNO))
	{
		LibraryFolder = AOIDataCollect.GetAOIStaticDataDirectory();
		LibraryFolder.append(L"\\");
		LibraryFolder.append(m_ProjectPtr->GetProjectFileMainName());
		clear(LibraryFolder.c_str());
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::clear(LPCTSTR Folder)
{
	CString FileFolder;
	JetAPI::ExtractFolder(Folder, FileFolder);

	if (NULL == Folder)
	{
		return;
	}
	const size_t FolderLen = ::_tcslen(Folder);
	if (FolderLen == 0)
	{
		return;
	}

	DWORD   Res = 0;
	CString Dir = _T("");
	CString SearchName = _T("");
	CString FileName;
	CString extName;
	bool IsFolder = false;
	Dir.Format(_T("%s\\"), Folder);
	FileName.Format(_T("%s%s"), Dir, _T("*.*"));

	HANDLE handle;
	WIN32_FIND_DATA FindFileData;
	handle = FindFirstFile(FileName, &FindFileData);
	if (handle == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		SearchName.Format(_T("%s"), FindFileData.cFileName);
		if (SearchName == _T('.') || SearchName == _T("..")) { continue; }
		FileName.Format(_T("%s%s"), Dir, FindFileData.cFileName);

		Res = FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY;
		if (0 != Res)
		{
			IsFolder = true;
		}
		else
		{
			IsFolder = false;
		}

		JetAPI::ExtractExtendFileName(FileName, extName);
		if (true == IsFolder)
		{//展開資料夾，去刪檔案
			clear(FileName);//遞迴
		}
		else if (extName == _T("jsc"))
		{
			::DeleteFile(FileName);
		}

	} while (::FindNextFile(handle, &FindFileData));

	FindClose(handle);
}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::OnItemchangedParamListWnd(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopParamListBeSelected ) { return; }	
	const int nItem = pNMListView->iItem;	
	const int nSubItem = pNMListView->iSubItem;
	if ( nItem < 0 ) 
	{
		//SetDescriptionText(NULL);
		return; 
	}
	DWORD Res=0;
	DWORD ResOld = pNMListView->uOldState&LVIS_FOCUSED;
	DWORD ResNew = pNMListView->uNewState&LVIS_FOCUSED;	
	if ( 0!=ResOld && 0==ResNew )
	{
		
		return;
	}
	Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }		
	ExecItemchangedParamListWnd(m_ParamListCtrl, nItem);	
	//BuildBarcodeImage(m_ParamListCtrl, nItem);
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::OnStnDblclickImageWnd()
{
	obj->ClearAllSeriesLabel();
}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::OnNMDblclkParamListWnd(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMITEMACTIVATE *pNMListView = (NMITEMACTIVATE*)pNMHDR;
	const int nItem = pNMListView->iItem;
	const int    ItemCount = m_ParamListCtrl.GetItemCount();
	
	if (nItem < 0 || nItem >= ItemCount) { return; }
	if (m_ProjectPtr == NULL) { return; }
	if (m_ParamListIndex == nItem) { return; }
	const size_t ParamIndex = m_ParamListCtrl.GetItemData(nItem);

	CAOIWindow    *pWindow = NULL;
	CAOIComponent *pComponent = m_ProjectPtr->GetProjectActiveComponent();

	AOIDataCollect.CloseActiveComponent(pComponent);

	m_ProjectPtr->SelectProjectAllComponents(false);

	m_ProjectPtr->ResetProjectActiveIndex();
	
	m_ProjectPtr->SetProjectActiveComponent(m_Component[nItem]);//

	pWindow = m_Component[nItem]->GetComponentWindowPtr(0, true);

	if (NULL == pWindow)
	{
		m_ProjectPtr->SetProjectActiveComponentWindowIndex(-1);
	}
	else
	{
		m_ProjectPtr->SetProjectActiveComponentWindowIndex(0);
	}

	CAOIModel *ModelPtr = m_Component[nItem]->GetComponentModelPtr();
	
	m_ActiveWndPtr = ModelPtr->GetModelWndActived();
	m_ActiveWndPtr->SetWndModified(true);
	m_ActiveWndPtr->SetWndUIUpated_Param(false);
	ModelPtr->ApplyModelWnd(m_ActiveWndPtr);
	UpdateWndParm();

	AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, NULL);//

	AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);
	//AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
	//if (true == m_MoveToComponent)
	{
		AOIDataCollect.MoveStageToComponentOrField(m_Component[nItem], false);
	}
	m_ParamListIndex = nItem;
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::OnSelchangeParamCombo()
{
	if (algType == JetAPI::GetComboxCurSelData(m_ComboxCtrl)) { return; }
	ClearChart();
	if (smartChartType != 2)//讀檔不需要
		ExecDataStatic();
	DrawSmartChart();
	//ExecUpdateParamByCombox();
}
//-------------------------------------------------------------------------------------//
//void CAlgHeightRecognizeWnd::OnKillfocusParamCombo()
//{
//	// TODO: Add your control notification handler code here
//	//if ( m_ComboxCtrl.IsWindowVisible() == FALSE ) { return; }
//	ExecUpdateParamByCombox();
//}
//-------------------------------------------------------------------------------------//
bool CStaticChartWnd::ExecItemchangedParamListWnd(CThisListCtrl_01 &ListCtrl, int nItem)
{	
	const int    ItemCount = ListCtrl.GetItemCount();
	if ( nItem<0 || nItem>=ItemCount ) { return false; }

	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
	// 找這筆數據在哪，看index內數值相同
	for (int i = 0; i < m_XIndexOK.size(); i++)
	{
		if (m_XIndexOK[i] == ParamIndex)
		{
			obj->ClearAllSeriesLabel();
			SSendData_ShowSerieLabel label(SC_T("OK"), i, SC_T(""));
			obj->SendData(&label);
			return true;
		}
	}

	for (int i = 0; i < m_XIndexNG.size(); i++)
	{
		if (m_XIndexNG[i] == ParamIndex)
		{
			obj->ClearAllSeriesLabel();
			SSendData_ShowSerieLabel label(SC_T("NG"), i, SC_T(""));
			obj->SendData(&label);
			return true;
		}
	}
	return false;
	//vector<double>::iterator it = std::find(m_XIndexNG.begin(), m_XIndexNG.end(), ParamIndex);

	//m_XIndexOK
	//if (it == m_XIndexNG.end())
	//{
	//	SSendData_ShowSerieLabel label(SC_T("OK"), ParamIndex, SC_T(""));
	//	obj->SendData(&label);
	//}
	//else
	//{
	//	SSendData_ShowSerieLabel label(SC_T("NG"), ParamIndex, SC_T(""));
	//	obj->SendData(&label);
	//}
	
	return true;
}
//-------------------------------------------------------------------------------------//
//bool CStaticChartWnd::ExecDblclkParamListWnd(CThisListCtrl_01 &ListCtrl, int nItem, int nSubItem)
//{	
//	const int    ItemCount = ListCtrl.GetItemCount();
//	if ( nItem<0 || nItem>=ItemCount ) { return false; }
//	if ( nSubItem < 1 ) { return true; }
//
//	const size_t ParamIndex = ListCtrl.GetItemData(nItem);
//	const size_t ParamCount = m_ParamList.size();
//	if ( ParamIndex >= ParamCount ) { return false; }		
//	
//	size_t          i=0;
//	int             nSelIdx=0;
//	int             nValue=0;
//	CRect           ItemRect;
//	RECT            CtrlRect={0};	
//	CString         ItemText;	
//	const int       Offset = 2;
//	CParamUni      *ParamPtr=&(m_ParamList[ParamIndex]);		
//	const bool      ReadOnly = ParamPtr->GetReadOnly();	
//	if ( true == ReadOnly ) { return true; }	
//	PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
//
//	const size_t    SelItemCount = ParamPtr->GetSelItemCount();
//	if ( ListCtrl.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, ItemRect) == FALSE )
//	{	return false; }
//	ItemText = ListCtrl.GetItemText(nItem, nSubItem);
//
//	CtrlRect = ItemRect;
//	ListCtrl.ClientToScreen(&CtrlRect);
//	this->ScreenToClient(&CtrlRect);
//	::OffsetRect(&CtrlRect, 0, -2);
//	//m_BtnCtrl.ShowWindow(SW_HIDE);
//	SetActParamUni(ParamPtr);
//	if ( PARAM_DATA_SEL == DataType )
//	{
//		if ( m_ComboxCtrl.GetSafeHwnd() != NULL )
//		{
//			//nSelIdx = 0;
//			//JetAPI::ClearCombox(m_ComboxCtrl);
//			//for ( i=0; i<SelItemCount; i++ )
//			//{
//			//	if ( ParamPtr->GetSelItem(i, true, nValue, ItemText) == false ) { continue; }
//			//	m_ComboxCtrl.InsertString(nSelIdx, ItemText);
//			//	m_ComboxCtrl.SetItemData(nSelIdx, nValue);
//			//	nSelIdx ++;
//			//}			
//			//JetAPI::SetComboxCurSel(m_ComboxCtrl, ParamPtr->GetSelParam());
//			//m_ComboxCtrl.MoveWindow(&CtrlRect, FALSE);			
//			//m_ComboxCtrl.SetFocus();
//			//m_ComboxCtrl.ShowDropDown();
//			//m_ComboxCtrl.ShowWindow(SW_SHOW);
//			//m_ComboxCtrl.BringWindowToTop();			
//			//ListCtrl.UpdateWindow();
//			//m_ComboxCtrl.Invalidate();
//		}	
//	}
//	else
//	{
//	}
//	return true;
//}
//-------------------------------------------------------------------------------------//
void CStaticChartWnd::OnBnClickedOk()
{
	if (smartChartType != 2)
		obj->DataCallBack(EMsg_SmartChart_Ask::DragLine);
	else
		CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
BOOL CStaticChartWnd::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	CAlgParam &pAlgParam = m_ActiveWndPtr->GetWndAlgParam();
	TALG_PARAM_IPC_PRODUCT &ipcParam = pAlgParam.GetAlgParamIPC();
	TALG_PARAM_RESIN_HEIGHT &hightDetectParm = pAlgParam.GetAlgParamResinHeight();

	switch ((EReplyData_CallBack)pCopyDataStruct->dwData)
	{
	case EReplyData_CallBack::DragLine:
		SReplyData_DragLine *pdata = reinterpret_cast<SReplyData_DragLine*>(pCopyDataStruct->lpData);
		int i = std::pow(10, m_roundLength);
		m_newLSL = pdata->GetValue(L"LSL");
		m_newUSL = pdata->GetValue(L"USL");

		switch (algType)
		{
		default:
			if (m_LSLPtr != NULL)
				*m_LSLPtr = std::floor((m_newLSL - m_Sec) * i) / i;
			if (m_USLPtr != NULL)
				*m_USLPtr = std::floor((m_newUSL - m_Sec) * i) / i;

			break;
		case 100:
			pAlgParam.SetAlgOffsetXUSL(std::floor(std::abs(m_newLSL)));
			break;
		case 101:
			pAlgParam.SetAlgOffsetYUSL(std::floor(std::abs(m_newLSL)));
			break;
		case 102:
			pAlgParam.SetAlgSkewUSL(std::floor(std::abs(m_newLSL) * i) / i);
			break;
		case 110:
			ipcParam.ipcWidthRatio = m_newLSL;
			break;
		case 200:
			hightDetectParm.nPartHeightThreshold = m_newLSL;
			break;
		}
		break;
	}

	if (m_ParamListIndex >= 0)
	{
		CAOIModel *ModelPtr = m_Component[m_ParamListIndex]->GetComponentModelPtr();
		m_ActiveWndPtr = ModelPtr->GetModelWndActived();
		m_ActiveWndPtr->SetWndModified(true);
		m_ActiveWndPtr->SetWndUIUpated_Param(false);
		ModelPtr->ApplyModelWnd(m_ActiveWndPtr);
		AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, NULL);
		AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
	}
	if ((EReplyData_CallBack)pCopyDataStruct->dwData == EReplyData_CallBack::DragLine)
	{
		CDialog::OnOK();
	}
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}
//-------------------------------------------------------------------------------------//
bool CStaticChartWnd::isSuccess()
{//檢查開檔路徑用(失敗)
	if (smartChartType != 2) { return true; }
	std::wstring alg = JetAPI::GetComboxCurSelText(m_ComboxCtrl);//null
	CString ModelFolder;
	std::wstring LibraryFolder;
	JetAPI::ExtractMainFileName(m_ProjectPtr->GetProjectShowName(), ModelFolder);
	LibraryFolder = AOIDataDefine.GetProjectLibraryFolderName(ModelFolder);
	LibraryFolder.append(L"\\");
	LibraryFolder.append(m_Component[0]->GetComponentModelName());
	LibraryFolder.append(L"\\");
	LibraryFolder.append(std::to_wstring(m_ActiveWndPtr->GetWndIndex()));
	LibraryFolder.append(L"_");
	LibraryFolder.append(alg);

	if (JetAPI::IsFileExist(LibraryFolder.c_str()))
		return true;
	else
		return false;
}
//-------------------------------------------------------------------------------------//
bool CStaticChartWnd::ExecUpdateParamByEdit()
{
	//CParamUni   *ParamPtr = GetActParamUni();
	//if ( NULL == ParamPtr ) { return true; }	
	//PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();	
	//if ( PARAM_DATA_SEL == DataType ) { return true; }
	//SetActParamUni(NULL);	

	//CString ItemText;	
	//WND_ALG_PROPERTY_ID SysParam = (WND_ALG_PROPERTY_ID)(ParamPtr->GetParamID());	
	//m_EditCtrl.GetWindowText(ItemText);
	//if ( ParamPtr->SetNewValue(ItemText) == false )
	//{		
	//	ItemText = ParamPtr->GetParamText();
	//	m_EditCtrl.SetWindowText(ItemText);
	//	return false;
	//}
	////if (SetGlueParameterStringByID(SysParam, m_GlueParam, ItemText, 0) == false )
	////{	return false; }
	//
	//CThisListCtrl_01 *pListCtrl = (CThisListCtrl_01*)(ParamPtr->GetListCtrl());
	//const int nItem = ParamPtr->GetItemIndex();
	//const int nSubItem = ParamPtr->GetSubItemIndex();	
	//if ( NULL != pListCtrl )
	//{
	//	const int ItemCount = pListCtrl->GetItemCount();
	//	if ( nItem>=0 && nItem<ItemCount )
	//	{	
	//		pListCtrl->SetItemText(nItem, nSubItem, ItemText);	
	//		pListCtrl->SetFocus();
	//	}
	//	//BuildBarcodeImage(*pListCtrl, nItem);
	//}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CStaticChartWnd::ExecUpdateParamByCombox()
{
	//CParamUni   *ParamPtr = GetActParamUni();
	//if ( NULL == ParamPtr ) { return true; }	
	//PARAM_DATA_TYPE  DataType = ParamPtr->GetDataType();
	//if ( PARAM_DATA_SEL != DataType ) { return true; }
	//SetActParamUni(NULL);

	//CString ItemText;	
	//WND_ALG_PROPERTY_ID SysParam = (WND_ALG_PROPERTY_ID)(ParamPtr->GetParamID());
	//const int nCurSel = m_ComboxCtrl.GetCurSel();
	//if ( nCurSel < 0 ) { return true; }

	//const int Param = (int)(m_ComboxCtrl.GetItemData(nCurSel));
	//if ( Param == ParamPtr->GetSelParam() ) { return false; } //比較-數字沒變return
	//ItemText.Format(_T("%d"),Param);	
	//if ( ParamPtr->SetNewValue_SEL(Param) == false )
	//{	return false; }
	//
	//ItemText = ParamPtr->GetParamText();
	////if (SetGlueParameterStringByID(SysParam, m_GlueParam, ItemText, Param) == false)
	////{
	////	return false;
	////}
	//
	//CThisListCtrl_01 *pListCtrl = (CThisListCtrl_01*)(ParamPtr->GetListCtrl());
	//const int nItem = ParamPtr->GetItemIndex();
	//const int nSubItem = ParamPtr->GetSubItemIndex();	
	//if ( NULL != pListCtrl )
	//{
	//	const int ItemCount = pListCtrl->GetItemCount();
	//	if ( nItem<0 || nItem>=ItemCount ) { return true; }	
	//	pListCtrl->SetItemText(nItem, nSubItem, ItemText);	
	//	pListCtrl->SetFocus();
	//	//BuildBarcodeImage(*pListCtrl, nItem);
	//}	
	return true;
}
//-------------------------------------------------------------------------------------//
