// DefectListWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "DefectListWnd.h"
//-------------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CDefectListWnd dialog
//-------------------------------------------------------------------------------------//

CDefectListWnd::CDefectListWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDefectListWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDefectListWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_StopDefectListBeSelected = false;
	m_StopAlgorithmListBeSelected = false;
	
	m_LandPtr = NULL;
	m_Algorithm = ALG_BRIGHT_RATIO;
	m_DefectID = WND_DEFECT_BODY_MISSING;
}
//-------------------------------------------------------------------------------------//
void CDefectListWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefectListWnd)
	DDX_Control(pDX, DEFECT_ALGORITHM_LIST_WND, m_AlgorithmListWnd);
	DDX_Control(pDX, DEFECT_DEFECT_LIST_WND, m_DefectListWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CDefectListWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CDefectListWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(DEFECT_ALGORITHM_FILTER_CHK, OnAlgorithmFilterChk)
	ON_NOTIFY(NM_CLICK, DEFECT_DEFECT_LIST_WND, OnClickDefectListWnd)
	ON_NOTIFY(LVN_ITEMCHANGED, DEFECT_DEFECT_LIST_WND, OnItemchangedDefectListWnd)
	ON_NOTIFY(NM_DBLCLK, DEFECT_ALGORITHM_LIST_WND, OnDblclkAlgorithmListWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CDefectListWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CDefectListWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd::CheckDlgButton(DEFECT_ALGORITHM_FILTER_CHK, TRUE);
	BuildDefectListWnd();
	BuildAlgorithmListWnd();
	SwitchMultiLanguage();
	//AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CDefectListWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CDefectListWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CDefectListWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CBaseDialog::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//
void CDefectListWnd::OnAlgorithmFilterChk() 
{
	// TODO: Add your control notification handler code here
	BuildAlgorithmListWnd();
}
//-------------------------------------------------------------------------------------//
void CDefectListWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_DEFECT_LIST_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_DEFECT_LIST_WND;
	WndKey = _T("IDD_DEFECT_LIST_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = IDOK;
	NewLabelText = AOIDataDefine.GetWndOKText();
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IDCANCEL;
	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = DEFECT_DEFECT_LIST_LABEL;
	WndKey = _T("DEFECT_DEFECT_LIST_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = DEFECT_ALGORITHM_LIST_LABEL;
	WndKey = _T("DEFECT_ALGORITHM_LIST_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = DEFECT_ALGORITHM_FILTER_CHK;
	WndKey = _T("DEFECT_ALGORITHM_FILTER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
WND_DEFECT_ID CDefectListWnd::GetDefectID()
{
	return m_DefectID;
}
//-------------------------------------------------------------------------------------//
ALG_TYPE CDefectListWnd::GetAlgorithm()
{	
	return m_Algorithm;
}
//-------------------------------------------------------------------------------------//
void CDefectListWnd::SetLandPtr(CAOILand *LandPtr)
{
	m_LandPtr = LandPtr;
}
//-------------------------------------------------------------------------------------//
bool CDefectListWnd::FilterDefectID(WND_DEFECT_ID DefectID)
{
	if ( CAOIWnd::FilterWndDefectID(DefectID, m_LandPtr) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CDefectListWnd::FilterAlgorithm(ALG_TYPE AlgType)
{
	BOOL bCheck = CWnd::IsDlgButtonChecked(DEFECT_ALGORITHM_FILTER_CHK);
	if ( FALSE == bCheck ) { return true; }

	//const int nItem = m_DefectListWnd.GetNextItem(-1, LVNI_SELECTED);
	//if ( nItem < 0 ) { return true; }	
	//WND_DEFECT_ID DefectID = (WND_DEFECT_ID)(m_DefectListWnd.GetItemData(nItem));

	WND_DEFECT_ID DefectID = m_DefectID;	
	if ( CAOIWnd::FilterWndAlgType(AlgType, DefectID, m_LandPtr) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
UINT CDefectListWnd::GetDefectListIcon(WND_DEFECT_ID DefectID, bool Small)
{
	return 0;
}
//-------------------------------------------------------------------------------------//
UINT CDefectListWnd::GetAlgorithmListIcon(ALG_TYPE AlgType, bool Small)
{
	return 0;
}
//-------------------------------------------------------------------------------------//
BOOL CDefectListWnd::ClearDefectListWnd()
{
	m_StopDefectListBeSelected = true;	
	m_DefectListWnd.DeleteAllItems();
	m_StopDefectListBeSelected = false;	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CDefectListWnd::BuildDefectListWnd()
{
	int i=0;
	UINT       TypeIcon=0;
	CString    TypeName;	
	const int  IconW = 40;//80
	const int  IconH = 40;//80
	CBitmap    bmp;
	CSize      SpaceSize;
	COLORREF   clrMask=0x00000000;
	WND_DEFECT_ID DefectID = WND_DEFECT_NONE;

	CListCtrl  &ListCtrl  = m_DefectListWnd;
	CImageList &ImageList = m_DefectImageList;
	
	ClearDefectListWnd();
	ImageList.DeleteImageList();	
	//ImageList.Create(IconW, IconH, ILC_COLOR24, 0, 4);	//建立Image列表
	//ListCtrl.SetImageList(&ImageList, LVSIL_NORMAL);	

	SpaceSize.cx = IconW+8;
	SpaceSize.cy = IconH+32;
	SpaceSize = ListCtrl.SetIconSpacing(SpaceSize);	
	
	i = 0;
	ListCtrl.SetRedraw(FALSE);
	m_StopDefectListBeSelected = true;	

	const std::vector<WND_DEFECT_ID> &List=AOIDataCollect.GetWndDefectIDList();
	const size_t Count=List.size();
	for ( size_t j=0; j<Count; j++ )
	{
		DefectID=List[j];
		if ( FilterDefectID(DefectID) == true )
		{
			TypeIcon = GetDefectListIcon(DefectID, true);
			TypeName = AOIDataDefine.GetWndDefectIDText(DefectID);		
			ListCtrl.InsertItem(i, TypeName, i);		
			ListCtrl.SetItemText(i, 0, TypeName);
			ListCtrl.SetItemData(i, (DWORD_PTR)DefectID);
			//bmp.LoadBitmap(TypeIcon);
			//ImageList.Add(&bmp, clrMask);	
			bmp.DeleteObject();
			i ++;
		}
	}

	if ( i > 0 ) 
	{
		const int nItem = 0;
		ListCtrl.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
	}	

	m_StopDefectListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	ListCtrl.Invalidate();
	ListCtrl.UpdateWindow();
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CDefectListWnd::ClearAlgorithmListWnd()
{
	m_StopAlgorithmListBeSelected = true;	
	m_AlgorithmListWnd.DeleteAllItems();
	m_StopAlgorithmListBeSelected = false;	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CDefectListWnd::BuildAlgorithmListWnd()
{
	int i=0;
	UINT       TypeIcon=0;
	CString    TypeName;	
	const int  IconW = 40;//80
	const int  IconH = 40;//80
	CBitmap    bmp;
	CSize      SpaceSize;
	COLORREF   clrMask=0x00000000;
	ALG_TYPE   AlgType = ALG_EMPTY;

	CListCtrl  &ListCtrl  = m_AlgorithmListWnd;
	CImageList &ImageList = m_AlgorithmImageList;
	
	ClearAlgorithmListWnd();
	ImageList.DeleteImageList();
	//ImageList.Create(IconW, IconH, ILC_COLOR24, 0, 4);	//建立Image列表
	//ListCtrl.SetImageList(&ImageList, LVSIL_NORMAL);

	SpaceSize.cx = IconW+8;
	SpaceSize.cy = IconH+32;
	SpaceSize = ListCtrl.SetIconSpacing(SpaceSize);	
	
	const int nItem = m_DefectListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( nItem < 0 ) { return TRUE; }
	m_DefectID = (WND_DEFECT_ID)(m_DefectListWnd.GetItemData(nItem));

	i = 0;
	ListCtrl.SetRedraw(FALSE);
	m_StopAlgorithmListBeSelected = true;	

	std::vector<ALG_TYPE> AlgList;
	CAlgParam::BuildAlgTypeList(AlgList);
	const size_t AlgCount=AlgList.size();
	for ( size_t ii=0; ii<AlgCount; ii++ )
	{
		AlgType = AlgList[ii];
		if ( FilterAlgorithm(AlgType) == false )
		{	continue; }

		TypeIcon = GetAlgorithmListIcon(AlgType, true);
		TypeName = AOIDataDefine.GetAlgTypeText(AlgType);		
		ListCtrl.InsertItem(i, TypeName, i);		
		ListCtrl.SetItemText(i, 0, TypeName);
		ListCtrl.SetItemData(i, (DWORD_PTR)AlgType);
		//bmp.LoadBitmap(TypeIcon);
		//ImageList.Add(&bmp, clrMask);	
		bmp.DeleteObject();
		i ++;			
	}

	if ( i > 0 ) 
	{
		const int nItem = 0;
		ListCtrl.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
	}

	m_StopAlgorithmListBeSelected = false;
	ListCtrl.SetRedraw(TRUE);
	ListCtrl.Invalidate();
	ListCtrl.UpdateWindow();
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CDefectListWnd::OnClickDefectListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CDefectListWnd::OnItemchangedDefectListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( true == m_StopDefectListBeSelected ) { return; }
	const int nItem = pNMListView->iItem;	
	if ( nItem < 0 ) { return; }
	DWORD Res = pNMListView->uNewState&LVIS_SELECTED;
	if ( Res == 0 ) { return; }	
	Res = pNMListView->uNewState&LVIS_FOCUSED;
	if ( Res == 0 ) { return; }

	BOOL bCheck = CWnd::IsDlgButtonChecked(DEFECT_ALGORITHM_FILTER_CHK);
	if ( TRUE == bCheck )
	{	BuildAlgorithmListWnd();	}

	*pResult = 0;
}
//-------------------------------------------------------------------------------------//
void CDefectListWnd::OnOK() 
{
	// TODO: Add extra validation here	
	if ( ExecOnOK() == false ) { return; }
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CDefectListWnd::OnDblclkAlgorithmListWnd(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CDefectListWnd::OnOK();
	*pResult = 0;
}
//---------------------------------------------------------------------------------//	
bool CDefectListWnd::ExecOnOK()
{
	const int nDefectItem = m_DefectListWnd.GetNextItem(-1, LVNI_SELECTED);
	const int nAlgorithmItem = m_AlgorithmListWnd.GetNextItem(-1, LVNI_SELECTED);
	if ( nDefectItem<0 || nAlgorithmItem<0 ) 
	{ return false; }	
	m_DefectID = (WND_DEFECT_ID)(m_DefectListWnd.GetItemData(nDefectItem));
	m_Algorithm = (ALG_TYPE)(m_AlgorithmListWnd.GetItemData(nAlgorithmItem));
	return true;
}
//---------------------------------------------------------------------------------//	