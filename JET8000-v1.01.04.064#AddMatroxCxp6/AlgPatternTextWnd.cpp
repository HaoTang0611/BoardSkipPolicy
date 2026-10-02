// AlgPatternTextWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "AlgPatternTextWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgPatternTextWnd dialog
//-------------------------------------------------------------------------------------//
CAlgPatternTextWnd::CAlgPatternTextWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CAlgPatternTextWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlgPatternTextWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_RawPtr = NULL;
	m_ImagePtr = NULL;
	m_ImageW = 0;
	m_ImageH = 0;
	m_ImageStep = 0;
	m_BitCount = 0;	

	m_OffsetPt.x = 0;
	m_OffsetPt.y = 0;
	m_ZoomScale = 1.0;
	m_BkColor = 0xDFC3AB;

	m_Modified = false;
	m_FirstShow = false;
	m_ActiveRoiIdx = -1;
	m_PatternAngle = 0.0f;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlgPatternTextWnd)
	DDX_Control(pDX, PATTEXT_IMAGE_WND, m_ImageWnd);	
	DDX_Control(pDX, PATTEXT_SIMILAR_TEXT_LIST_BOX, m_SimilarTextListBox);	
	DDX_Control(pDX, PATTEXT_PATTERN_ANGLE_COMBO, m_PatternAngleCombox);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CAlgPatternTextWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CAlgPatternTextWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()	
	ON_BN_CLICKED(PATTEXT_SET_TEXT_CHK, OnSetTextChk)
	ON_BN_CLICKED(PATTEXT_SHOW_INDEX_CHK, OnShowIndexChk)
	ON_BN_CLICKED(PATTEXT_SIMILAR_TEXT_ADD_BTN, OnSimilarTextAddBtn)
	ON_BN_CLICKED(PATTEXT_SIMILAR_TEXT_DEL_BTN, OnSimilarTextDelBtn)
	ON_BN_CLICKED(PATTEXT_SIMILAR_TEXT_CLEAR_BTN, OnSimilarTextClearBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CAlgPatternTextWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CAlgPatternTextWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();	
	// TODO: Add extra initialization here	
	RECT WndRect={0,0,0,0};
	m_FirstShow = true;
	m_ImageWnd.GetClientRect(&WndRect);
	m_ImageMemDC.CreateMemDC(&m_ImageWnd, m_BkColor);
	ImageAPI.CalcImageWndFitZoom(m_ImageW, m_ImageH, WndRect, 1.2, m_ZoomScale);
	AOIDataCollect.LimitImageZoomScale(m_ZoomScale, true);

	CreateBKImageWnd();
	BuildPatternAngleList();
	BuildSimilarTextListBox();
	BuildPatRoiListShowRect();

	SwitchMultiLanguage();			
	//CWnd::CheckDlgButton(PATTEXT_SHOW_INDEX_CHK, TRUE);
	CWnd::CheckDlgButton(PATTEXT_UPPER_CASE_CHK, TRUE);
	CWnd::SetDlgItemText(PATTEXT_TEXT_EDIT, m_PatternText);		
	JetAPI::SetComboxCurSel(m_PatternAngleCombox, (int)(m_PatternAngle));
	//AskSetPatternText();//此時會造成對話盒無法鎖住上層, 參考[m_FirstShow]
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ReleaseBuffer();
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	RedrawWnd();	
	// Do not call CBaseDialog::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( true == m_FirstShow )
	{
		AskSetPatternText();		
		m_FirstShow = false;
	}
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CWnd::MapWindowPoints(&m_ImageWnd, &point, 1);
	const bool bInputStringMode=GetInputStringMode();
	if ( true == bInputStringMode )
	{	InputPatRoiListTextString();	}
	else
	{	SetPatRoiListText(point); }
	//this->SetCapture();	
	CBaseDialog::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	//::ReleaseCapture();
	CBaseDialog::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
CPatternParam& CAlgPatternTextWnd::GetPatternParam()
{
	return m_PatternParam;
}
//-------------------------------------------------------------------------------------//
float CAlgPatternTextWnd::GetPatternAngle() const
{
	return m_PatternAngle;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::SetPatternParam(const CPatternParam &PatParam, BOX_TOWARD Toward, float Angle)
{
	m_WndToward = Toward;
	m_PatternAngle = Angle;
	m_PatternParam = PatParam;	
	m_PatternText = PatParam.GetPatText();
	m_InputStringMode = PatParam.GetPatInputStringMode();

	m_SimilarTextList.clear();
	const size_t SimilarTextCount=PatParam.GetPatSimilarTextCount();
	for ( size_t i=0; i<SimilarTextCount; i++ )
	{	m_SimilarTextList.push_back(CString(PatParam.GetPatSimilarText(i, false)));	}	

	const int PatID = 0;	
	const std::vector<TPATTERN_ROI> &PatRoiList=m_PatternParam.GetPatRoiList(Toward, PatID);	
	const size_t PatRoiCount=PatRoiList.size();
	for ( size_t i=0; i<PatRoiCount; i++ )
	{
		TPatRoiInfo PatRoiInfo;
		const TPATTERN_ROI &PatRoi=PatRoiList[i];

		PatRoiInfo.index = i;		
		PatRoiInfo.RoiRect = PatRoi.RoiRect;
		m_PatRoiInfoList.push_back(PatRoiInfo);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::SetImagePtr(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR Ptr)
{
	const char fnName[]="CAlgPatternTextWnd::SetImagePtr";
	ReleaseBuffer();
	const size_t BufferSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( NULL==Ptr || 0==BufferSize ) { return; }
	IMAGE_PTR RawPtr=NULL;
	IMAGE_PTR ImagePtr=NULL;
	if ( JetMemory.alloc_func(BufferSize, RawPtr, fnName, "RawPtr") == false ||
		 JetMemory.alloc_func(BufferSize, ImagePtr, fnName, "ImagePtr") == false )
	{
		JetMemory.free_func(RawPtr);
		JetMemory.free_func(ImagePtr);
		return; 
	}
	::memcpy(RawPtr, Ptr, sizeof(IMAGE_DATA)*BufferSize);
	if ( AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ImageStep, BitCount, Ptr, ImagePtr) == false )
	{
		JetMemory.free_func(RawPtr);
		JetMemory.free_func(ImagePtr);
		return;
	}
	m_RawPtr = RawPtr;
	m_ImagePtr = ImagePtr;
	m_ImageW = ImageW;
	m_ImageH = ImageH;
	m_ImageStep = ImageStep;
	m_BitCount = BitCount;	
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::ReleaseBuffer()
{
	if ( NULL != m_RawPtr )
	{	JetMemory.free_func(m_RawPtr);	}	
	if ( NULL != m_ImagePtr )
	{	JetMemory.free_func(m_ImagePtr);	}	
	m_ImageW = 0;
	m_ImageH = 0;
	m_ImageStep = 0;
	m_BitCount = 0;	
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::BuildPatternAngleList()
{
	CComboBox &Combox=m_PatternAngleCombox;
	if ( Combox.GetSafeHwnd() == NULL ) { return ; }

	size_t       i=0;	
	int          idx=0;	
	int          Angle=0;
	CString      String;
	
	
	idx = 0;
	Angle = 0;
	JetAPI::ClearCombox(Combox);			
	for ( int i=0; i<4; i++ )
	{
		Angle = i*90;
		String.Format(_T("%03d"), Angle);
		Combox.InsertString(-1, String);
		Combox.SetItemData(idx, Angle);
		idx ++;
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::BuildSimilarTextListBox()
{
	CListBox &ListBox=m_SimilarTextListBox;	
	if ( ListBox.GetSafeHwnd() == NULL ) { return ; }
	const size_t Count=m_SimilarTextList.size();

	ListBox.ResetContent();
	for ( size_t i=0; i<Count; i++ )
	{	ListBox.AddString(m_SimilarTextList[i]);	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::RedrawWnd()
{
	//DrawImageWnd();
	if ( m_ImageWnd.GetSafeHwnd() == NULL ) { return; }
	CClientDC dc(&m_ImageWnd);
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ImageMemDC.GetSafeHdc();
	if ( NULL == hDC ) { return; }
	if ( NULL == hMemDC ) { return; }		

	RECT WndRect={0,0,0,0};	
	const POINT &OffsetPt=m_OffsetPt;
	const IMAGE_SIZE ImageW=m_ImageW;
	const IMAGE_SIZE ImageH=m_ImageH;
	const double ZoomScale = m_ZoomScale;	
	const size_t ActiveRoiIdx = GetActiveRoiIdx();
	const BOOL bShowIndex=CWnd::IsDlgButtonChecked(PATTEXT_SHOW_INDEX_CHK);
	m_ImageWnd.GetClientRect(&WndRect);		
	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC, 0, 0, SRCCOPY );	

	const std::vector<TPatRoiInfo>&PatRoiInfoList=GetPatRoiInfoList();//包含兩個極性
	const size_t PatRoiInfoCount=PatRoiInfoList.size();
	
	CString str;
	const int TextGap=4;
	COLORREF clr=0xFFFFFF;
	COLORREF clrOk=0x00FF00;
	COLORREF clrFocus=0x0000FF;
	COLORREF clrUnSet=0xFFFFFF;
	//const int OldBkMode=::SetBkMode(hDC, TRANSPARENT);
	for ( size_t i=0; i<PatRoiInfoCount; i++ )
	{
		const TPatRoiInfo &PatRoiInfo=PatRoiInfoList[i];
		const RECT &Rect=PatRoiInfo.ShowRect;

		if ( true == PatRoiInfo.bSet )
		{	clr = clrOk;	}
		else if ( ActiveRoiIdx == i )
		{	clr = clrFocus;	}
		else
		{	clr = clrUnSet;	}

		HPEN hPen = ::CreatePen(PS_SOLID, 1, clr);
		HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));
		ImageAPI.DrawRectLine(hDC, Rect);
		if ( TRUE == bShowIndex )
		{
			str.Format(_T("%d"), i+1);
			::TextOut(hDC, Rect.left+TextGap, Rect.top+TextGap, str, str.GetLength());		
		}
		::SelectObject(hDC, hOldPen);
		::DeleteObject(hPen); hPen=NULL;
	}
	//::SetBkMode(hDC, OldBkMode);
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::CreateBKImageWnd()
{
	HDC hDC = m_ImageMemDC.GetSafeHdc();
	if ( NULL == hDC ) { return; }

	RECT Rect={0,0,0,0};
	m_ImageWnd.GetClientRect(&Rect);

	IMAGE_SIZE ImageW=m_ImageW;
	IMAGE_SIZE ImageH=m_ImageH;
	IMAGE_SIZE BitCount=m_BitCount;
	IMAGE_SIZE ImageStep=m_ImageStep;
	IMAGE_PTR  ImagePtr=m_ImagePtr;
	if ( NULL == ImagePtr )
	{	return; }
	TPOINT2D Offset;
	double ZoomScale = m_ZoomScale;	
	ImageAPI.DrawImageToDC(hDC, ImageW, ImageH, ImageStep, BitCount, ImagePtr, Rect, Offset, ZoomScale, m_BkColor);	
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::SwitchMultiLanguage()
{
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDD_ALG_PATTERN_TEXT_WND), false);
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(IDOK));
	SetMultiLanauage(LoadIDAndName(IDCANCEL));
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(PATTEXT_SET_TEXT_CHK));	
	SetMultiLanauage(LoadIDAndName(PATTEXT_SHOW_INDEX_CHK));	
	SetMultiLanauage(LoadIDAndName(PATTEXT_UPPER_CASE_CHK));	
	SetMultiLanauage(LoadIDAndName(PATTEXT_PATTERN_ANGLE_LABEL));	
	//---------------------------------------------------------------------------------//	
	SetMultiLanauage(LoadIDAndName(PATTEXT_SIMILAR_TEXT_GROUP));
	SetMultiLanauage(LoadIDAndName(PATTEXT_SIMILAR_TEXT_ADD_BTN));
	SetMultiLanauage(LoadIDAndName(PATTEXT_SIMILAR_TEXT_DEL_BTN));
	SetMultiLanauage(LoadIDAndName(PATTEXT_SIMILAR_TEXT_CLEAR_BTN));
	//SetMultiLanauage(LoadIDAndName(PATLST_WND_INFO_LABEL));
	
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID)
{
	LPCTSTR Section=_T("IDD_ALG_PATTERN_TEXT_WND");		
	if ( AOIDataCollect.SwitchMultiLanguageWnd(this, Section, ID, Text, bCtrlID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAlgPatternTextWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_ALG_PATTERN_TEXT_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
std::vector<TPatRoiInfo>& CAlgPatternTextWnd::GetPatRoiInfoList()
{	
	return m_PatRoiInfoList;
}
//-------------------------------------------------------------------------------------//
size_t CAlgPatternTextWnd::GetActiveRoiIdx() const
{
	return m_ActiveRoiIdx;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::SetActiveRoiIdx(size_t idx)
{
	m_ActiveRoiIdx = idx;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::GetInputStringMode() const
{
	return m_InputStringMode;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::InputString()//輸入字串
{
	CString      strValue;
	CString      strLabel;
	CString      strCaption;	
	CInputBoxWnd InputBox;
	const int InputBoxWndH = 96;
	const BOOL bUpperCase=CWnd::IsDlgButtonChecked(PATTEXT_UPPER_CASE_CHK);
	strLabel=_T("String");
	strLabel=LoadMultiLanguageString(strLabel, strLabel);
	strCaption=_T("Input String Wnd");
	strCaption=LoadMultiLanguageString(strCaption, strCaption);

	CWnd::GetDlgItemText(PATTEXT_TEXT_EDIT, strValue);
	if ( strValue.GetLength() == 0 )
	{
		std::wstring AiOcrText;
		if ( DetectAIModelOcrText(AiOcrText) == true )
		{	strValue = CString(AiOcrText.c_str());	}
	}

	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return false;  }
	m_Modified = true;
	if ( TRUE == bUpperCase )
	{	InputBox.m_DataEdit1.MakeUpper();	}
	m_PatternText=InputBox.m_DataEdit1;	
	BuildPatternText();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::InputRoiChar()//輸入Roi字元
{
	POINT WndPt={0};
	RECT WndRect={0};	
	CString      strLabel;
	CString      strCaption;	
	CInputBoxWnd InputBox;
	const int InputBoxWndH = 96;
	const size_t ActiveRoiIdx=GetActiveRoiIdx();	
	std::vector<TPatRoiInfo> &PatRoiInfoList=GetPatRoiInfoList();	
	const size_t PatRoiInfoCount=PatRoiInfoList.size();
	const BOOL bUpperCase=CWnd::IsDlgButtonChecked(PATTEXT_UPPER_CASE_CHK);
	if ( ActiveRoiIdx >= PatRoiInfoCount ) { return false; }
	WndRect = m_RoiRectRgn;
	m_ImageWnd.ClientToScreen(&WndRect);
	WndPt.x = (WndRect.right+WndRect.left)/2;
	//WndPt.y = (WndRect.top+WndRect.bottom)/2;
	WndPt.y = WndRect.bottom+InputBoxWndH;

	strLabel=_T("Char");
	strLabel=LoadMultiLanguageString(strLabel, strLabel);
	strCaption=_T("Input Char Wnd");
	strCaption=LoadMultiLanguageString(strCaption, strCaption);

	InputBox.SetWndPos(WndPt);
	InputBox.SetParam1(strCaption, strLabel, _T(""));
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return false;  }
	m_Modified = true;
	if ( TRUE == bUpperCase )
	{	InputBox.m_DataEdit1.MakeUpper();	}
	PatRoiInfoList[ActiveRoiIdx].sText=InputBox.m_DataEdit1;	
	BuildPatternText();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::BuildPatternText()
{
	const bool bInputStringMode=GetInputStringMode();
	if ( false == bInputStringMode )
	{
		const std::vector<TPatRoiInfo>&PatRoiInfoList=GetPatRoiInfoList();
		const size_t PatRoiInfoCount=PatRoiInfoList.size();
	
		m_PatternText = _T("");
		for ( size_t i=0; i<PatRoiInfoCount; i++ )
		{
			const TPatRoiInfo &PatRoiInfo=PatRoiInfoList[i];
			m_PatternText += PatRoiInfo.sText;
		}
	}
	CWnd::SetDlgItemText(PATTEXT_TEXT_EDIT, m_PatternText);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::AskSetPatternText()//詢問設定樣板文字
{
	CString str=m_PatternParam.GetPatText();
	if ( str.GetLength() > 0 ) { return false; }
	//str = _T("Do you want to set the pattern text ?");
	//str = LoadMultiLanguageString(str, str);
	//if ( IDNO == JetAPI::ShowMessageBox(str, MB_YESNO) )
	//{	return false; }
	CWnd::CheckDlgButton(PATTEXT_SET_TEXT_CHK, TRUE);
	CWnd::PostMessage(WM_COMMAND, PATTEXT_SET_TEXT_CHK, 0);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::BuildPatRoiListShowRect()
{
	RECT WndRect={0,0,0,0};	
	const POINT &OffsetPt=m_OffsetPt;
	const IMAGE_SIZE ImageW=m_ImageW;
	const IMAGE_SIZE ImageH=m_ImageH;
	const double ZoomScale = m_ZoomScale;		
	m_ImageWnd.GetClientRect(&WndRect);		
	std::vector<TPatRoiInfo>&PatRoiInfoList=GetPatRoiInfoList();
	const size_t PatRoiInfoCount=PatRoiInfoList.size();
	::memset(&m_RoiRectRgn, 0x00, sizeof(m_RoiRectRgn));

	for ( size_t i=0; i<PatRoiInfoCount; i++ )
	{		
		TPatRoiInfo &PatRoiInfo=PatRoiInfoList[i];
		ImageAPI.MapImageRectToWndRect_INT(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, PatRoiInfo.RoiRect, PatRoiInfo.ShowRect);
		if ( 0 == i )
		{	m_RoiRectRgn = PatRoiInfo.ShowRect; }
		else
		{	::UnionRect(&m_RoiRectRgn, &m_RoiRectRgn, &PatRoiInfo.ShowRect);	}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::InitialPatRoiListSetting()
{
	m_Modified = false;
	std::vector<TPatRoiInfo>&PatRoiInfoList=GetPatRoiInfoList();
	const size_t PatRoiInfoCount=PatRoiInfoList.size();
	for ( size_t i=0; i<PatRoiInfoCount; i++ )
	{	
		TPatRoiInfo &PatRoiInfo=PatRoiInfoList[i];
		PatRoiInfo.bSet = false;	
		PatRoiInfo.sText = _T("");
	}
	BuildPatternText();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::CheckPatRoiListSetFinish()
{
	const std::vector<TPatRoiInfo>&PatRoiInfoList=GetPatRoiInfoList();
	const size_t PatRoiInfoCount=PatRoiInfoList.size();
	for ( size_t i=0; i<PatRoiInfoCount; i++ )
	{
		if ( false == PatRoiInfoList[i].bSet )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAlgPatternTextWnd::GetPatRoiListSetFinishCount()
{
	size_t Count=0;
	const std::vector<TPatRoiInfo>&PatRoiInfoList=GetPatRoiInfoList();
	const size_t PatRoiInfoCount=PatRoiInfoList.size();
	for ( size_t i=0; i<PatRoiInfoCount; i++ )
	{
		if ( false == PatRoiInfoList[i].bSet ) { continue; }
		Count ++;
	}
	return Count;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::InputPatRoiListTextString()
{
	//BOOL bChk=CWnd::IsDlgButtonChecked(PATTEXT_SET_TEXT_CHK);
	//if ( FALSE == bChk ) { return true; }
	if ( InputString() == false ) { return false; }

	std::vector<TPatRoiInfo>&PatRoiInfoList=GetPatRoiInfoList();//包含兩個極性
	const size_t PatRoiInfoCount=PatRoiInfoList.size();
	for ( size_t i=0; i<PatRoiInfoCount; i++ )
	{
		TPatRoiInfo &PatRoiInfo=PatRoiInfoList[i];				
		PatRoiInfo.bSet = true;			
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::SetPatRoiListText(const POINT &pt)
{
	BOOL bChk=CWnd::IsDlgButtonChecked(PATTEXT_SET_TEXT_CHK);
	if ( FALSE == bChk ) { return true; }

	std::vector<TPatRoiInfo>&PatRoiInfoList=GetPatRoiInfoList();//包含兩個極性
	const size_t PatRoiInfoCount=PatRoiInfoList.size();
	for ( size_t i=0; i<PatRoiInfoCount; i++ )
	{
		TPatRoiInfo &PatRoiInfo=PatRoiInfoList[i];
		if ( ::PtInRect(&PatRoiInfo.ShowRect, pt) == FALSE ) { continue; }
		SetActiveRoiIdx(i);
		RedrawWnd();
		if ( InputRoiChar() == false ) { return false; }
		PatRoiInfo.bSet = false;
		const size_t idx=GetPatRoiListSetFinishCount();
		PatRoiInfo.bSet = true;
		SwapPatRoi(i, idx);
		if ( CheckPatRoiListSetFinish() == true )
		{	
			CWnd::CheckDlgButton(PATTEXT_SET_TEXT_CHK, FALSE);
			JetAPI::ShowMessageBox(AOIDataDefine.GetFinishText());			
			RedrawWnd();
		}
		return true;
	}	
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::SwapPatRoi(size_t idx1, size_t idx2)
{		
	std::vector<TPatRoiInfo> &PatRoiInfoList=GetPatRoiInfoList();
	const size_t PatRoiInfoCount=PatRoiInfoList.size();
	if ( idx1>=PatRoiInfoCount || idx2>=PatRoiInfoCount || idx1==idx2 )
	{	return false;	}
	TPatRoiInfo Tmp=PatRoiInfoList[idx1];
	PatRoiInfoList[idx1] = PatRoiInfoList[idx2];
	PatRoiInfoList[idx2] = Tmp;		
	SetActiveRoiIdx(idx2);
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::SwapPatRoiList(const std::vector<TPATTERN_ROI> &SrcList, std::vector<TPATTERN_ROI> &DstList)
{
	const size_t SrcRoiCount=SrcList.size();
	const std::vector<TPatRoiInfo>& PatRoiInfoList=GetPatRoiInfoList();
	const size_t PatRoiInfoCount=PatRoiInfoList.size();

	DstList.clear();
	for ( size_t i=0; i<PatRoiInfoCount; i++ )
	{
		const TPatRoiInfo &PatRoiInfo=PatRoiInfoList[i];
		const size_t RoiIdx=PatRoiInfo.index;
		if ( RoiIdx >= SrcRoiCount )
		{	return false;	}
		DstList.push_back(SrcList[RoiIdx]);
	}
	if ( DstList.size() != SrcRoiCount )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::OnOK() 
{
	// TODO: Add extra validation here			
	DWORD Res = IDYES;
	CString str, strErr;
	bool bSucc = true;
	const bool bAsk=false;
	if ( true == bAsk )
	{
		str = _T("Do you want to update the Roi Text?");
		str = LoadMultiLanguageString(str, str);
		Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
	}
	if ( IDCANCEL == Res )
	{	return; }
	if ( IDNO == Res )
	{
		CBaseDialog::OnCancel();
		return; 
	}
	if ( IDYES == Res )
	{
		std::wstring ws;
		const size_t MaxPatternRoiCount=MAX_PATTERN_ROI_COUNT;
		std::vector<TPATTERN_ROI> PatRoiListL[MaxPatternRoiCount];
		std::vector<TPATTERN_ROI> PatRoiListT[MaxPatternRoiCount];
		std::vector<TPATTERN_ROI> PatRoiListR[MaxPatternRoiCount];
		std::vector<TPATTERN_ROI> PatRoiListB[MaxPatternRoiCount];

		for ( size_t i=0; i<MaxPatternRoiCount; i++ )
		{	
			if ( SwapPatRoiList(m_PatternParam.GetPatRoiListL(i), PatRoiListL[i]) == false )
			{
				bSucc = false;
				strErr.Format(_T("Error, Change Pattern Roi List Fault [Left]"));
				break;
			}
			if ( SwapPatRoiList(m_PatternParam.GetPatRoiListT(i), PatRoiListT[i]) == false )
			{
				bSucc = false;
				strErr.Format(_T("Error, Change Pattern Roi List Fault [Top]"));
				break;
			}
			if ( SwapPatRoiList(m_PatternParam.GetPatRoiListR(i), PatRoiListR[i]) == false )
			{
				bSucc = false;
				strErr.Format(_T("Error, Change Pattern Roi List Fault [Right]"));
				break;
			}
			if ( SwapPatRoiList(m_PatternParam.GetPatRoiListB(i), PatRoiListB[i]) == false )
			{
				bSucc = false;
				strErr.Format(_T("Error, Change Pattern Roi List Fault [Bottom]"));
				break;
			}				
		}

		if ( false == bSucc )
		{
			JetAPI::ShowMessageBox(strErr);
			return ;
		}

		CloneSimilarListBoxContent();
		JetAPI::TCHAR2wstring(m_PatternText, ws);
		m_PatternParam.SetPatText(ws.c_str());

		const size_t SimilarTextCount=m_SimilarTextList.size();
		m_PatternParam.ClearPatSimilarTextList();
		for ( size_t i=0; i<SimilarTextCount; i++ )
		{
			ws = L"";
			JetAPI::TCHAR2wstring(m_SimilarTextList[i], ws);
			if ( ws.length() == 0 ) { continue; }
			m_PatternParam.AddPatSimilarText(ws.c_str());
		}

		for ( size_t i=0; i<MaxPatternRoiCount; i++ )
		{
			m_PatternParam.SetPatRoiListL(i, PatRoiListL[i]);
			m_PatternParam.SetPatRoiListT(i, PatRoiListT[i]);
			m_PatternParam.SetPatRoiListR(i, PatRoiListR[i]);
			m_PatternParam.SetPatRoiListB(i, PatRoiListB[i]);
		}
		m_PatternAngle = (float)(JetAPI::GetComboxCurSelData(m_PatternAngleCombox));
	}	
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::OnSetTextChk()
{
	// TODO: Add your message handler code here and/or call default	
	BOOL bChk=CWnd::IsDlgButtonChecked(PATTEXT_SET_TEXT_CHK);
	if ( FALSE == bChk ) { return ; }
	InitialPatRoiListSetting();
	RedrawWnd();
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::OnShowIndexChk()
{
	// TODO: Add your message handler code here and/or call default	
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::DetectAIModelOcrText(std::wstring &OcrText)
{
	if ( CAOIModel::GetAIServerIsReady() == false )
	{	return false; }

	TALG_PARAM_AI_MODEL aiModelParam;
	if ( SaveAIModelFile()==false )
	{	return false; }
	if ( LoadAIModelFile(aiModelParam)==false )//失敗
	{	return false;	}
	//CString sResult=CString(aiModelParam.aiResultText.c_str());
	if ( BuildAIModelOcrText(aiModelParam, OcrText) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::SaveAIModelFile()
{
	CString SyncFile;
	CString Filename;	
	CString MainName;
	CString ExtName = _T("PNG");	
	CString FolderSend=CAOIModel::GetAIServerFolderSend();	
	const IMAGE_PTR  ImagePtr = m_RawPtr;	
	const IMAGE_SIZE ImageW = m_ImageW;
	const IMAGE_SIZE ImageH = m_ImageH;	
	const IMAGE_SIZE BitCount=m_BitCount;	
	const IMAGE_SIZE ImageStep=m_ImageStep;	
	if ( NULL == ImagePtr ) { return false; }	
	const bool bInputStringMode=GetInputStringMode();
	MainName = _T("PatternText");
	Filename.Format(_T("%s\\%s#1.%s"), FolderSend, MainName, ExtName);
	if ( ImageAPI.SaveImage(Filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true) == false )
	{	return false;	}
	
	CString        strText;	
	RECT           WndRect;
	RECT           ComponentRect;
	BOX_TOWARD     WndToward;	
	TREGION4D      Region;
	const size_t   szBuffer = 256;
	wchar_t        strTag[szBuffer] = L"";
	wchar_t        strBuffer[szBuffer] = L"";			
	const float    PatternAngle = (float)(JetAPI::GetComboxCurSelData(m_PatternAngleCombox));
	
	Filename.Format(_T("%s\\%s"), FolderSend, _T("PatternText.JSON"));	
	JetAPI::GetSyncFilename(Filename, SyncFile);
	DeleteFile(SyncFile);

	int Sub=2;
	//JetAPI::ExtractMainFileNameNoPath(Filename, MainName);
	JetAPI::SizeToRect(ImageW-Sub, ImageH-Sub, WndRect);
	JetAPI::SizeToRect(ImageW-Sub, ImageH-Sub, ComponentRect);

	FILE *pfile = ::_tfopen(Filename, _T("w+"));
	if ( NULL == pfile )
	{	return false;	}

	::fwprintf(pfile, L"{\n");

	::fwprintf(pfile, L"  \"HeaderData\":\n");//Header_cs
	::fwprintf(pfile, L"  {\n");//Header_cs Begin
	//"Version_s": "1.0.2",
	::wcscpy(strTag, L"Version_s");	
	::wcscpy(strBuffer, AI_MODEL_VERSION);	
	::fwprintf(pfile, L"    \"%s\": \"%s\",\n", strTag, strBuffer);

	::wcscpy(strTag, L"Sender_s");	
	::wcscpy(strBuffer, AOI3D_APP_NAME_W);//L"JET8000"	
	::fwprintf(pfile, L"    \"%s\": \"%s\"\n", strTag, strBuffer);
	::fwprintf(pfile, L"  },\n");//Header_cs End

	::fwprintf(pfile, L"  \"%s\":\n", L"AIInferenceDataW");//AIInferenceDataW
	::fwprintf(pfile, L"  [\n");//AIInferenceDataW Begin
	const size_t AIWndCount = 1;
	for ( size_t i=0; i<AIWndCount; i++ )
	{			
		WndToward = m_WndToward;
		if ( 0 != i )
		{	::fwprintf(pfile, L",\n");	}
		
		::fwprintf(pfile, L"    {\n");//Wnd Begin
		
		//"ComponentImageFileName": "PanelID_BoardID_ComponentName_ImgID.jpg"				
		::wcscpy(strTag, L"ComponentImageFileName");
		strText.Format(_T("%s#1.%s"), MainName, ExtName);		
		JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
		::fwprintf(pfile, L"      \"%s\": \"%s\",\n", strTag, strBuffer);

		//"ROI":
		::fwprintf(pfile, L"      \"%s\":\n", L"Rect");
		::fwprintf(pfile, L"      [\n");//Rect Begin
		//"window rect": [0,0,342,282],		
		::fwprintf(pfile, L"        [%d, %d, %d, %d],\n", WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);

		//"Component rect": [0,0,342,282],		
		::fwprintf(pfile, L"        [%d, %d, %d, %d]\n", ComponentRect.left, ComponentRect.top, ComponentRect.right, ComponentRect.bottom);
		::fwprintf(pfile, L"      ],\n");//Rect End

		//"AImodelID": 0
		::wcscpy(strTag, L"AImodelID");
		::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, ALG_AI_MODEL_OCR_01);

		//"WindowDirection": 1,
		::wcscpy(strTag, L"WindowDirection");
		::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, 1);

		//"ConfidenceThreshold": 1,
		::wcscpy(strTag, L"ConfidenceThreshold");
		::fwprintf(pfile, L"      \"%s\": %.2f,\n", strTag, 0.5f);

		//"OCRImageRotateAngle": 0,
		::wcscpy(strTag, L"OCRImageRotateAngle");
		const int nPatternAngle=JetAPI::AdjustRotationAngle(PatternAngle);
		::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, nPatternAngle);

		std::vector<RECT> CharRectList;
		std::vector<std::wstring> OcrTextList;
		
		const size_t OcrTextCount=OcrTextList.size();
		//"OCRGroundTruthStr": []
		::wcscpy(strTag, L"OCRGroundTruthStr");
		::fwprintf(pfile, L"      \"%s\":\n", strTag);
		::fwprintf(pfile, L"      [\n");//OCRGroundTruthStr Begin
		for ( size_t j=0; j<OcrTextCount; j++ )
		{
			if ( 0 != j )
			{	::fwprintf(pfile, L",\n");	}
			::fwprintf(pfile, L"        \"%s\"", OcrTextList[j].c_str());				
		}
		::fwprintf(pfile, L"\n");		
		::fwprintf(pfile, L"      ],\n");//OCRGroundTruthStr End
		
		size_t CharRectCount=0;
		if ( false == bInputStringMode )
		{	CharRectCount=CharRectList.size(); }
		::wcscpy(strTag, L"OCRGroundTruthBox");//"OCRGroundTruthBox": []
		::fwprintf(pfile, L"      \"%s\":\n", strTag);
		::fwprintf(pfile, L"      [\n");//OCRGroundTruthBox Begin
		for ( size_t j=0; j<CharRectCount; j++ )
		{
			if ( 0 != j )
			{	::fwprintf(pfile, L",\n");	}
			const RECT &rc=CharRectList[j];
			::fwprintf(pfile, L"        [%d, %d, %d, %d]", rc.left, rc.top, rc.right, rc.bottom);
		}
		::fwprintf(pfile, L"\n");		
		::fwprintf(pfile, L"      ],\n");//OCRGroundTruthBox End		

		//"WindowID": 0,
		::wcscpy(strTag, L"WindowID");
		::fwprintf(pfile, L"      \"%s\": %d\n", strTag, 0);

		::fwprintf(pfile, L"    }");//Wnd End
	}
	::fwprintf(pfile, L"\n");
	::fwprintf(pfile, L"  ]\n");//AIInferenceDataW End		
	
	::fwprintf(pfile, L"}\n");
	::fclose(pfile); pfile = NULL;

	JetAPI::CreateSyncFile(Filename);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::LoadAIModelFile(TALG_PARAM_AI_MODEL &aiModelParam)
{
	CString Filename, SyncFile;
	CString FolderRecv=CAOIModel::GetAIServerFolderRecv();	

	bool bVal=true;
	bool bRet=false;
	int  nVal=1;
	float fVal=1.f;	
	double dVal=1.0;	
	RECT   rcVal;
	std::wstring wsVal;	
	std::wstring wsSender;
	std::wstring wsVersion;
	std::vector<int> nList;
	std::vector<float> fList;
	std::vector<double> dList;
	std::vector<std::wstring> wsList;	
	rapidjson::CGMItr itr;
	std::wstring strfilename;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;		
	const int AiExceptionCode = -1;

	Filename.Format(_T("%s\\%s"), FolderRecv, _T("PatternText.JSON"));	
	JetAPI::GetSyncFilename(Filename, SyncFile);
	if ( CAOIModel::WaitForAIServerSyncFile(SyncFile) == false )
	{	return false; }
	
	JetAPI::TCHAR2wstring(Filename, strfilename);
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);	
	if ( JSonCtrl.OpenFile(strfilename.c_str(), Doc) == false )
	{	return false; }

	if ( JSonCtrl.ReadDocItr(Doc, L"HeaderData", itr) == false )		
	{	return false;	}
	if ( JSonCtrl.ReadObjString(itr, L"Version_s", wsVersion) == false )
	{	return false; }
	if ( JSonCtrl.ReadObjString(itr, L"Sender_s", wsSender) == false )
	{	return false; }

	if ( JSonCtrl.ReadDocItr(Doc, L"AIResultW", itr) == false )		
	{	return false;	}	
	auto pName = itr->name.GetString();	
	auto arrayV = itr->value.GetArray();
	for (auto m = arrayV.Begin(); arrayV.End() != m; ++m)
	{	
		if ( m->IsObject() == false )
		{	continue; }
		
		rapidjson::CGMItr itrLv2;
		if ( JSonCtrl.ReadValInt(*m, L"WindowID", nVal) == false )//"WindowID": 0,
		{	continue;	}
		if ( AiExceptionCode == nVal )
		{	return false; }

		const int WndIdx=nVal;
		if ( JSonCtrl.ReadValInt(*m, L"IsOK", nVal) == true )//"IsOK": true,
		{
			switch ( nVal )
			{
			case AI_RESULT_ID_NONE:
			case AI_RESULT_ID_OK:
			case AI_RESULT_ID_NG:
			case AI_RESULT_ID_BYPASS:
				aiModelParam.aiResultID=(AI_RESULT_ID)(nVal);
				break;
			default:
			case AI_RESULT_ID_EXCEPTION:
				aiModelParam.aiResultID=AI_RESULT_ID_EXCEPTION;
				break;
			}			
		}

		if ( JSonCtrl.ReadValString(*m, L"ErrorMessage", wsVal) == true )//"ErrorMessage": "None",
		{	aiModelParam.aiResultText = wsVal;	}

		if ( JSonCtrl.ReadValFloat(*m, L"AIConfidenceW", fVal) == true )//"AIConfidenceW": 0.9999,
		{	aiModelParam.aiConfidence = fVal;	}		
		
		if ( JSonCtrl.ReadValItr(*m, L"AIResultB", itrLv2) == true )//AIResultB": []			
		{
			TALG_PARAM_AI_CHAR aiChar;							
			if ( itrLv2->value.IsArray() == true )
			{					
				auto arrayLv2 = itrLv2->value.GetArray();					
				for (auto m2=arrayLv2.Begin(); arrayLv2.End()!=m2; ++m2)
				{
					if ( m2->IsObject() == false ) { continue; }

					if ( JSonCtrl.ReadValString(*m2, L"AIBoxClassName", wsVal) == true )//"AIBoxClassName": "P",
					{	aiChar.acChar = wsVal;	}

					if ( JSonCtrl.ReadValRect(*m2, L"AIBoxRect", rcVal) == true )//"AIBoxRect": [93,99,147,200],
					{	aiChar.acCharRect = rcVal;	}

					if ( JSonCtrl.ReadValFloat(*m2, L"AIBoxConfidence", fVal) == true )//"AIBoxConfidence": 0.999999
					{	aiChar.acCharConfidence = fVal;	}

					aiModelParam.aiOcrCharList.push_back(aiChar);
				}
			}			
		}	
		
		bRet = bRet;
	}
	bRet = bRet;
	::DeleteFile(SyncFile);
	::DeleteFile(Filename);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAlgPatternTextWnd::BuildAIModelOcrText(const TALG_PARAM_AI_MODEL &aiModelParam, std::wstring &string)
{
	string = L"";
	const std::vector<TALG_PARAM_AI_CHAR> &aiOcrCharList=aiModelParam.aiOcrCharList;
	const size_t OcrCharCount=aiOcrCharList.size();
	for ( size_t i=0; i<OcrCharCount; i++ )
	{
		const TALG_PARAM_AI_CHAR &AiOcrChar=aiOcrCharList[i];
		const std::wstring &acChar=AiOcrChar.acChar;		
		if ( acChar.length() == 2 )
		{
			const wchar_t *p=&acChar[1];
			string.append(p);
		}
		else
		{	string.append(acChar);	}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::CloneSimilarListBoxContent()
{	
	CString str;
	CListBox &ListBox=m_SimilarTextListBox;	
	std::vector<CString> &List=m_SimilarTextList;
	if ( ListBox.GetSafeHwnd() == NULL ) { return ; }
	
	List.clear();
	
	const size_t Count=ListBox.GetCount();
	for ( size_t i=0; i<Count; i++ )
	{
		ListBox.GetText(i, str);
		List.push_back(str);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::OnSimilarTextAddBtn()
{
	// TODO: Add your message handler code here and/or call default	
	CListBox &ListBox=m_SimilarTextListBox;	
	if ( ListBox.GetSafeHwnd() == NULL ) { return ; }
	
	CString      strLabel;
	CString      strCaption;	
	CInputBoxWnd InputBox;
	const BOOL bUpperCase=CWnd::IsDlgButtonChecked(PATTEXT_UPPER_CASE_CHK);

	strLabel=_T("Similar Text");
	strLabel=LoadMultiLanguageString(strLabel, strLabel);
	strCaption=_T("Input Text Wnd");
	strCaption=LoadMultiLanguageString(strCaption, strCaption);
	//InputBox.SetWndPos(WndPt);
	InputBox.SetParam1(strCaption, strLabel, _T(""));
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return ;  }
	m_Modified = true;
	if ( TRUE == bUpperCase )
	{	InputBox.m_DataEdit1.MakeUpper();	}
	ListBox.AddString(InputBox.m_DataEdit1);
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::OnSimilarTextDelBtn()
{
	// TODO: Add your message handler code here and/or call default	
	CListBox &ListBox=m_SimilarTextListBox;	
	if ( ListBox.GetSafeHwnd() == NULL ) { return ; }
	const int nSel=ListBox.GetCurSel();
	if ( nSel < 0 ) { return ; }
	ListBox.DeleteString(nSel);
	return;
}
//-------------------------------------------------------------------------------------//
void CAlgPatternTextWnd::OnSimilarTextClearBtn()	
{
	// TODO: Add your message handler code here and/or call default	
	CString str;
	CListBox &ListBox=m_SimilarTextListBox;	
	if ( ListBox.GetSafeHwnd() == NULL ) { return ; }
	str = _T("Do you want to clear all similar text?");
	str=LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }
	ListBox.ResetContent();
	return;
}
//-------------------------------------------------------------------------------------//