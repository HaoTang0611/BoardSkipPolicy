// PropertiesWnd.cpp : 實作檔
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "JET8000.h"
#include "PropertiesWnd.h"
//-------------------------------------------------------------------------------------//
// CPropertiesWnd
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CPropertiesWnd, CDockablePane)
//-------------------------------------------------------------------------------------//
CPropertiesWnd::CPropertiesWnd()
{	
}
//-------------------------------------------------------------------------------------//
CPropertiesWnd::~CPropertiesWnd()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_LCLICKED,OnPropertyLClicked)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_RCLICKED,OnPropertyRClicked)
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED,OnPropertyChanged)
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
// CPropertiesWnd 訊息處理常式
//-------------------------------------------------------------------------------------//
int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此加入特別建立的程式碼
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 建立下拉式方塊:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	{
		TRACE0("無法建立 [屬性] 下拉式方塊\n");
		return -1;      // 無法建立
	}

	m_wndObjectCombo.AddString(_T("應用程式"));
	m_wndObjectCombo.AddString(_T("屬性視窗"));
	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("無法建立 [屬性] 方格\n");
		return -1;      // 無法建立
	}

	InitPropList();
	
	//m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	//m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 已鎖定 */);
	//m_wndToolBar.CleanUpLockedImages();
	//m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 鎖定 */);

	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	//m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	//m_wndToolBar.SetOwner(this);

	// 所有命令都將經由此控制項傳送，而不是經由父框架:
	//m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}
//-------------------------------------------------------------------------------------//
void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此加入您的訊息處理常式程式碼
	AdjustLayout();
}
//-------------------------------------------------------------------------------------//
void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	// TODO: 在此加入您的訊息處理常式程式碼
	m_wndPropList.SetFocus();
}
//-------------------------------------------------------------------------------------//
void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);

	// TODO: 在此加入您的訊息處理常式程式碼
	SetPropListFont();
}
//-------------------------------------------------------------------------------------//
LRESULT CPropertiesWnd::OnPropertyLClicked(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty *pProp = (CMFCPropertyGridProperty*)lParam;
	if ( NULL == pProp ) { return 0; }
	return 0;
}
//-------------------------------------------------------------------------------------//
LRESULT CPropertiesWnd::OnPropertyRClicked(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty *pProp = (CMFCPropertyGridProperty*)lParam;
	if ( NULL == pProp ) { return 0; }
	return 0;
}
//-------------------------------------------------------------------------------------//
LRESULT CPropertiesWnd::OnPropertyChanged(WPARAM wParam, LPARAM lParam)
{
	CMFCPropertyGridProperty *pProp = (CMFCPropertyGridProperty*)lParam;
	if ( NULL == pProp ) { return 0; }
	return 0;
}
//-------------------------------------------------------------------------------------//
void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = 0;

	if ( m_wndObjectCombo.GetSafeHwnd() != NULL )
	{	m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER); }
	if ( m_wndToolBar.GetSafeHwnd() != NULL )
	{
		cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
		m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER); 
	}
	if ( m_wndPropList.GetSafeHwnd() != NULL )
	{	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER); }
}
//-------------------------------------------------------------------------------------//
void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();
	
	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("外觀"));
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("3D 外觀"), (_variant_t) false, _T("指定視窗的字型為非粗體，且控制項有 3D 框線")));

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("框線"), _T("Dialog Frame"), _T("下列其中一項: None、Thin、Resizable 或 Dialog Frame"));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("Thin"));
	pProp->AddOption(_T("Resizable"));
	pProp->AddOption(_T("Dialog Frame"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridProperty(_T("標題"), (_variant_t) _T("關於"), _T("指定文字將顯示在視窗的標題列")));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("視窗大小"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("高度"), (_variant_t) 250l, _T("指定視窗的高度"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("寬度"), (_variant_t) 150l, _T("指定視窗的寬度"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("字型"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	lstrcpy(lf.lfFaceName, _T("Arial, 新細明體"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("字型"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("指定視窗的預設字型")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("使用系統字型"), (_variant_t) true, _T("指定視窗使用 MS Shell Dlg 字型")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("其他"));
	pProp = new CMFCPropertyGridProperty(_T("(名稱)"), _T("應用程式"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("視窗色彩"), RGB(210, 192, 254), NULL, _T("指定預設的視窗色彩"));
	pColorProp->EnableOtherButton(_T("其他..."));
	pColorProp->EnableAutomaticButton(_T("預設"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("圖示檔(*.ico)|*.ico|所有檔案(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("圖示"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("指定視窗圖示")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("資料夾"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);

	CMFCPropertyGridProperty* pGroup4 = new CMFCPropertyGridProperty(_T("階層"));

	CMFCPropertyGridProperty* pGroup41 = new CMFCPropertyGridProperty(_T("第一子層級"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridProperty* pGroup411 = new CMFCPropertyGridProperty(_T("第二子層級"));
	pGroup41->AddSubItem(pGroup411);


	CJETPropertyGridProperty* pProp2 = NULL;
	pProp2 = new CJETPropertyGridProperty(_T("項目 1"), (_variant_t) 200L, _T("這是描述"));
	//pProp2->SetReading((_variant_t) _T("讀值1234567890"));
	pProp2->SetReading((_variant_t) 198L);
	pProp2->SetReadingMode(FALSE);
	pProp2->SetReadingTextColor(0x0000FF);
	pProp2->SetReadingOnRight(TRUE);
	pProp2->EnableSpinControl(TRUE, 0, 500);
	pGroup411->AddSubItem(pProp2);
	//pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("項目 1"), (_variant_t) _T("值 1"), _T("這是描述")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("項目 2"), (_variant_t) _T("值 2"), _T("這是描述")));
	pGroup411->AddSubItem(new CMFCPropertyGridProperty(_T("項目 3"), (_variant_t) _T("值 3"), _T("這是描述")));

	pGroup4->Expand(FALSE);
	m_wndPropList.AddProperty(pGroup4);
	
}
//-------------------------------------------------------------------------------------//
void CPropertiesWnd::SetPropListFont()
{
	const TSystemParameter &SysParam=AOIDataCollect.GetSystemParameter();
	const int nAdd=SysParam.m_UIWndFontAddSize;
	if ( 0 != nAdd ) { return ; }

	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	m_wndObjectCombo.SetFont(&m_fntPropList);
}
//-------------------------------------------------------------------------------------//