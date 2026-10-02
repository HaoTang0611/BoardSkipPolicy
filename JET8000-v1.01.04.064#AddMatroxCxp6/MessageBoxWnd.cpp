// MessageBoxWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "MessageBoxWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define MSG_WND_RESIZE_WND   WM_USER+100
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CMessageBoxWnd dialog
//-------------------------------------------------------------------------------------//
CMessageBoxWnd::CMessageBoxWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CMessageBoxWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMessageBoxWnd)
	m_MessageText = _T("");
	//}}AFX_DATA_INIT	
	int BtnID=0;
	const int BtnCount=3;
	for ( int i=0; i<BtnCount; i++ )
	{		
		switch ( i )
		{
		case 0: BtnID=MSG_DEFBTN1; break;
		case 1: BtnID=MSG_DEFBTN2; break;
		case 2: BtnID=MSG_DEFBTN3; break;
		default:
			BtnID=0;
			break;
		}		
		m_BtnUsdMap[BtnID]=false;
		m_BtnIDList.push_back(BtnID);
	}

	m_MsgIcon = NULL;
	m_BoxType=MB_OK;//視窗樣式
	m_TextColor = CLR_DEFAULT;//文字顏色
	m_TextBkColor = CLR_DEFAULT;//背景顏色	
	m_FontSize = 0;
	m_FontName = _T("Cambria");//字型名稱	
	m_WndTitleText=CString(AfxGetAppName());	
}
//-------------------------------------------------------------------------------------//
void CMessageBoxWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMessageBoxWnd)
	DDX_Text(pDX, MSG_MESSAGE_LABEL, m_MessageText);
	DDX_Control(pDX, MSG_MESSAGE_ICON, m_MessageIcon);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CMessageBoxWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CMessageBoxWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(MSG_DEFBTN1, OnDefBtn1)
	ON_BN_CLICKED(MSG_DEFBTN2, OnDefBtn2)
	ON_BN_CLICKED(MSG_DEFBTN3, OnDefBtn3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CMessageBoxWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CMessageBoxWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here	
	HWND hWnd=GetSafeHwnd();
	CreateWndFont();
	SwitchMultiLanguage();	
	CWnd::SetWindowText(m_WndTitleText);
	if ( CheckEnableCloseBtn() == false )//啟用視窗右上方關閉按鈕(系統選單)
	{	JetAPI::EnableCloseButton(hWnd, FALSE); }
	const int BtnCount=m_BtnIDList.size();
	for ( int i=0; i<BtnCount; i++ )
	{	InitalBtnWnd(m_BtnIDList[i]);	}		
	if ( CheckUseTextBkColor() )
	{	m_TextBkBrush.CreateSolidBrush(m_TextBkColor); }
	LoadMessageIcon();	
	CWnd::PostMessage(MSG_WND_RESIZE_WND, 0, 0);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CMessageBoxWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}
//-------------------------------------------------------------------------------------//
void CMessageBoxWnd::OnSize(UINT nType, int cx, int cy) 
{
	CBaseDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
}
//-------------------------------------------------------------------------------------//	
HBRUSH CMessageBoxWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	if ( MSG_MESSAGE_LABEL == pWnd->GetDlgCtrlID() )
	{
		bool bSetColor=false;
		if ( CheckUseTextColor() )
		{
			bSetColor = true;
			pDC->SetTextColor(m_TextColor);
		}
		if ( CheckUseTextBkColor() )
		{
			bSetColor = true;			
			pDC->SetBkColor(m_TextBkColor);
			hbr = (HBRUSH)(m_TextBkBrush.GetSafeHandle());
		}
		if ( true == bSetColor )
		{	pDC->SetBkMode(TRANSPARENT);	}		

		if ( CheckUIUseTextFont() == false )
		{
			if ( CheckUseTextFont() )
			{	pDC->SelectObject(&m_TextFont);	}
		}
	}
	return hbr;
}
//-------------------------------------------------------------------------------------//
LRESULT CMessageBoxWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class	
	switch ( message )
	{
	case MSG_WND_RESIZE_WND:
		ReSizeMsgWnd();	
		break;
	}	
	return CBaseDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CMessageBoxWnd::SwitchMultiLanguage()
{
	SetMultiLanauage(LoadIDAndName(MSG_DEFBTN1));
	SetMultiLanauage(LoadIDAndName(MSG_DEFBTN2));
	SetMultiLanauage(LoadIDAndName(MSG_DEFBTN3));
}
//-------------------------------------------------------------------------------------//
bool CMessageBoxWnd::SetMultiLanauage(UINT ID, LPCTSTR Text, bool bCtrlID)
{
	UINT BtnID=ID;
	CString str=Text;
	LPCTSTR Section=_T("IDD_MESSAGE_BOX_WND");		
	if ( MSG_DEFBTN1==ID || MSG_DEFBTN2==ID || MSG_DEFBTN3==ID )
	{	BtnID = GetMessageBtnID(ID);	}
	if ( 0 == BtnID ) { return true; }
	switch ( BtnID )
	{
	case IDOK:		str=_T("IDOK"); break; 
	case IDCANCEL:	str=_T("IDCANCEL"); break; 
	case IDABORT:	str=_T("IDABORT"); break; 
	case IDRETRY:	str=_T("IDRETRY"); break; 
	case IDIGNORE:	str=_T("IDIGNORE"); break; 
	case IDYES:		str=_T("IDYES"); break; 
	case IDNO:		str=_T("IDNO"); break; 
	case IDCLOSE:	str=_T("IDCLOSE"); break; 
	case IDHELP:	str=_T("IDHELP"); break; 
	case IDTRYAGAIN: str=_T("IDTRYAGAIN"); break; 
	case IDCONTINUE: str=_T("IDCONTINUE"); break; 
	}	
	if ( AOIDataCollect.SwitchMultiLanguageWnd(this, Section, ID, str, bCtrlID) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMessageBoxWnd::SetBoxType(UINT Type)
{
	m_BoxType = Type;
	return true;
}
//-------------------------------------------------------------------------------------//
void CMessageBoxWnd::SetTextColor(COLORREF clr)//設定文字顏色
{
	m_TextColor = clr;
}
//-------------------------------------------------------------------------------------//
void CMessageBoxWnd::SetTextBkColor(COLORREF clr)//設定背景顏色
{
	m_TextBkColor = clr;
}
//-------------------------------------------------------------------------------------//
void CMessageBoxWnd::SetWndTitleText(LPCTSTR str)//設定視窗標題
{
	if ( NULL == str ) { return; }
	const size_t len=::_tcslen(str);
	if ( 0 == len ) { return; }	
	m_WndTitleText = str;
}
//-------------------------------------------------------------------------------------//
void CMessageBoxWnd::SetMessageText(LPCTSTR str)//設定訊息文字
{
	m_MessageText = str;
}
//-------------------------------------------------------------------------------------//
void CMessageBoxWnd::SetFontInfo(int nSize, LPCWSTR sName)//設定字型參數
{
	m_FontSize = nSize;
	if ( NULL != sName )
	{	m_FontName = sName;	}
	return;	
}
//-------------------------------------------------------------------------------------//
int CMessageBoxWnd::ShowMessageWnd(LPCTSTR lpszText, UINT nType, UINT nIDHelp)//顯示訊息視窗
{	
	//nType=MB_OKCANCEL;//MB_OK, MB_OKCANCEL, MB_ABORTRETRYIGNORE, MB_YESNOCANCEL, MB_YESNO, MB_RETRYCANCEL, MB_CANCELTRYCONTINUE		
	//SetFontInfo(24);
	//SetTextColor(0x0000FF);
	//SetTextBkColor(0x000000);
	SetBoxType(nType);
	SetMessageText(lpszText);	
	return DoModal();
}
//-------------------------------------------------------------------------------------//
UINT CMessageBoxWnd::GetMsgBtnType() const//取得訊息按鈕樣式
{
	UINT Type=m_BoxType;
	return (Type&0x0000000FL);
}
//-------------------------------------------------------------------------------------//
UINT CMessageBoxWnd::GetDefBtnType() const//取得預設按鈕樣式
{
	UINT Type=m_BoxType;
	return (m_BoxType&0x00000F00L);
}
//-------------------------------------------------------------------------------------//
UINT CMessageBoxWnd::GetMsgIconType() const//取得訊息圖示樣式
{
	UINT Type=m_BoxType;		
	return (Type&0x000000F0L);
}
//-------------------------------------------------------------------------------------//
bool CMessageBoxWnd::CreateWndFont()//建立視窗字型
{
	if ( CheckUseTextFont() == false ) { return true; }

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = m_FontSize;
	::_tcscpy(lf.lfFaceName, m_FontName);
	m_TextFont.CreateFontIndirect(&lf);	
	CWnd::SetFont(&m_TextFont);
	if ( CheckUIUseTextFont() == true )
	{	SendMessageToDescendants(WM_SETFONT, (WPARAM)m_TextFont.GetSafeHandle(), MAKELPARAM(FALSE, 0), TRUE); }
	else
	{
		CWnd *WndPtr=CWnd::GetDlgItem(MSG_MESSAGE_LABEL);
		if ( NULL != WndPtr )
		{		
			if ( CheckUseTextFont() )
			{	WndPtr->SetFont(&m_TextFont, FALSE);	}
		}
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMessageBoxWnd::ReSizeMsgWnd()//調整訊息視窗尺寸
{
	int szX=0, szY=0;
	RECT TextRect;	
	RECT ClientRect, WndRect, TmpRect;		
	const bool bUserFont=true;
	UINT IconID=GetMessageIconID();	
	CWnd *WndPtr=CWnd::GetDlgItem(MSG_MESSAGE_LABEL);	
	CalcMsgStringSize(WndPtr, m_MessageText, bUserFont, szX, szY);	
	CWnd::GetWindowRect(&WndRect);
	CWnd::GetClientRect(&ClientRect);
	CWnd::ClientToScreen(&ClientRect);	
	szX += 16;//外擴16
	szY += 16;//外擴16
	int   NewClientSizeX=szX;
	int   NewClientSizeY=szY;		
	const SIZE BtnSize=GetMessageBtnSize();
	const SIZE IconSize=GetMessageIconSize();
	const int TextSizeX=szX;
	const int TextSizeY=szY;
	const int GapTop=ClientRect.top-WndRect.top;
	const int GapBot=WndRect.bottom-ClientRect.bottom;	
	const int GapLef=ClientRect.left-WndRect.left;
	const int GapRig=WndRect.right-ClientRect.right;	
	const int WndCpX=(WndRect.left+WndRect.right)/2;
	const int WndCpY=(WndRect.top+WndRect.bottom)/2;
	const int BtnGapX=8;
	const int BtnGapY=8;
	const int BtnSizeX=BtnSize.cx;//84;
	const int BtnSizeY=BtnSize.cy;//48;
	const int BtnRectPitchX=BtnSizeX+BtnGapX;
	const int BtnUsedCount=GetMessageBtnCountUsed();
	const int TextGapX=8;
	const int TextGapY=32;
	const int IconGapX=24;
	const int IconSizeX=IconSize.cx;
	const int IconSizeY=IconSize.cy;
	const int MinClientSizeX=64;
	const int MinClientSizeY=64;
	const int MaxClientSizeX=1920;
	const int MaxClientSizeY=1080;	
	const int NewClientSizeXText=IconGapX+IconSizeX+TextGapX+TextSizeX+TextGapX;//上方圖示+文字	
	const int NewClientSizeXBtn =BtnGapX+(BtnUsedCount*BtnRectPitchX)+BtnGapX;//下方按鈕
	NewClientSizeX=MAX(NewClientSizeXText, NewClientSizeXBtn);
	NewClientSizeY=TextGapY+TextSizeY+TextGapY+BtnGapY+BtnSizeY+BtnGapY;
	NewClientSizeX=MAX(MinClientSizeX, NewClientSizeX);
	NewClientSizeY=MAX(MinClientSizeY, NewClientSizeY);
	NewClientSizeX=MIN(MaxClientSizeX, NewClientSizeX);
	NewClientSizeY=MIN(MaxClientSizeY, NewClientSizeY);

	const int NewWndSizeX=GapLef+NewClientSizeX+GapRig;
	const int NewWndSizeY=GapTop+NewClientSizeY+GapBot;
	WndRect.left = WndCpX-(NewWndSizeX/2);
	WndRect.top  = WndCpY-(NewWndSizeY/2);
	if ( WndRect.left < 0 ) { WndRect.left = 0; }
	if ( WndRect.top < 0 ) { WndRect.top = 0; }
	WndRect.right = WndRect.left+NewWndSizeX;
	WndRect.bottom = WndRect.top+NewWndSizeY;	
	const int WndCpX2=(WndRect.left+WndRect.right)/2;
	const int WndCpY2=(WndRect.top+WndRect.bottom)/2;
	ClientRect.left = WndRect.left+GapLef;
	ClientRect.top  = WndRect.top+GapTop;
	ClientRect.right = WndRect.right-GapRig;
	ClientRect.bottom = WndRect.bottom-GapBot;

	TextRect.left = ClientRect.left+IconGapX+IconSizeX+TextGapX;
	TextRect.top  = ClientRect.top+TextGapY;
	TextRect.right = ClientRect.right-TextGapX;
	TextRect.bottom = ClientRect.bottom-BtnGapY-BtnSizeY-BtnGapY-TextGapY;
	CWnd::MoveWindow(&WndRect, FALSE);

	RECT BtnRect;
	CWnd *pWnd=NULL;		
	if ( NULL == m_MsgIcon )
	{	m_MessageIcon.ShowWindow(SW_HIDE); }
	else
	{
		m_MessageIcon.GetWindowRect(&TmpRect);
		const int TempSizeX=TmpRect.right-TmpRect.left;
		const int TempSizeY=TmpRect.bottom-TmpRect.top;		
		BtnRect.top=TextRect.top;
		BtnRect.left=WndRect.left+GapLef+IconGapX;
		BtnRect.right=BtnRect.left+TempSizeX;
		BtnRect.bottom=BtnRect.top+TempSizeY;
		CWnd::ScreenToClient(&BtnRect);
		m_MessageIcon.MoveWindow(&BtnRect, FALSE);
	}

	pWnd=CWnd::GetDlgItem(MSG_MESSAGE_LABEL);
	if ( NULL!=pWnd && NULL!=pWnd->GetSafeHwnd() )
	{		
		BtnRect=TextRect;
		CWnd::ScreenToClient(&BtnRect);
		pWnd->MoveWindow(&BtnRect, FALSE);
	}

	int nBtnPitch=0;
	const size_t BtnCount=m_BtnIDList.size();
	for ( size_t i=0; i<BtnCount; i++ )
	{
		//int idx=(int)(i);
		int idx=(int)(BtnCount-i-1);
		UINT BtnID=m_BtnIDList[idx];
		if ( false==m_BtnUsdMap[BtnID] ) { continue; }
		pWnd=CWnd::GetDlgItem(BtnID);
		if ( NULL==pWnd || NULL==pWnd->GetSafeHwnd() )
		{	continue; }

		BtnRect.bottom=WndRect.bottom-GapBot-BtnGapY;
		BtnRect.top = BtnRect.bottom-BtnSizeY;		
		//BtnRect.left = WndRect.left+BtnGapX+(nBtnPitch*BtnRectPitchX);
		//BtnRect.right = BtnRect.left+BtnSizeX;
		BtnRect.right=ClientRect.right-BtnGapX-(nBtnPitch*BtnRectPitchX);
		BtnRect.left=BtnRect.right-BtnSizeX;
		CWnd::ScreenToClient(&BtnRect);
		pWnd->MoveWindow(&BtnRect, FALSE);		
		nBtnPitch ++;
	}	

	UINT DefBtnID=MSG_DEFBTN1;
	UINT DefFlag=GetDefBtnType();
	switch ( DefFlag )
	{
	case MB_DEFBUTTON2:	DefBtnID=MSG_DEFBTN2;	break;
	case MB_DEFBUTTON3:	DefBtnID=MSG_DEFBTN3;	break;
	default:
	case MB_DEFBUTTON1:	DefBtnID=MSG_DEFBTN1;	break;		
	}	
	if ( false == m_BtnUsdMap[DefBtnID] )
	{	DefBtnID=MSG_DEFBTN1; }
	CWnd::SendMessage(DM_SETDEFID, DefBtnID, 0);//切換預設按鈕
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMessageBoxWnd::InitalBtnWnd(UINT BtnID)
{	
	UINT RetD=GetMessageBtnID(BtnID);	
	if ( 0 == RetD ) 
	{ 
		m_BtnUsdMap[BtnID]=false;
		ShowCtrlWnd(BtnID, FALSE); 
		return true;
	}
	m_BtnUsdMap[BtnID]=true;
	//CString str;
	//str.LoadString(RetD); 
	//CWnd::SetDlgItemText(BtnID, str);
	return true;
}
//-------------------------------------------------------------------------------------//
UINT CMessageBoxWnd::GetMessageIconID() const//取得訊息圖示編號
{	
	UINT BtnID=0;	
	UINT IconID=0;	
	UINT Flag=GetMsgBtnType();
	switch ( Flag )
	{
	case MB_OK:				IconID=MB_ICONWARNING;	break;
	case MB_OKCANCEL:		IconID=MB_ICONWARNING;	break;
	case MB_YESNO:			IconID=MB_ICONQUESTION;	break;
	case MB_YESNOCANCEL:	IconID=MB_ICONQUESTION;	break;
	default:
		IconID = 0;
		break;
	}
	if ( 0 != IconID ) { return IconID; }

	Flag=GetMsgIconType();
	switch ( Flag )
	{
	case MB_ICONASTERISK:		IconID=MB_ICONASTERISK;	break;
	case MB_ICONQUESTION:		IconID=MB_ICONQUESTION;	break;
	case MB_ICONEXCLAMATION:	IconID=MB_ICONEXCLAMATION;	break;
	//case MB_ICONWARNING:		IconID=MB_ICONWARNING;	break;
	case MB_ICONERROR:			IconID=MB_ICONERROR;	break;
	//case MB_ICONINFORMATION:	IconID=MB_ICONINFORMATION;	break;
	//case MB_ICONSTOP:			conID=MB_ICONSTOP;	break;
	default:
		IconID = 0;
		break;
	}	
	return IconID;
}
//-------------------------------------------------------------------------------------//
UINT  CMessageBoxWnd::GetMessageBtnID(UINT ID) const//取得訊息按鈕編號
{	
	UINT BtnID=0;	
	UINT Flag=GetMsgBtnType();
	switch ( ID )
	{
	case MSG_DEFBTN1:
		switch ( Flag )
		{
		case MB_OK:					BtnID = IDOK;	break;
		case MB_OKCANCEL:			BtnID = IDOK;	break;
		case MB_ABORTRETRYIGNORE:	BtnID = IDABORT;	break;
		case MB_YESNOCANCEL:		BtnID = IDYES;	break;
		case MB_YESNO:				BtnID = IDYES;	break;
		case MB_RETRYCANCEL:		BtnID = IDRETRY;	break;
		case MB_CANCELTRYCONTINUE:	BtnID = IDCANCEL;	break;		
		default:
			BtnID = 0;
			break;
		}
		break;
	case MSG_DEFBTN2:
		switch ( Flag )
		{
		case MB_OK:					BtnID = 0;	break;
		case MB_OKCANCEL:			BtnID = IDCANCEL;	break;
		case MB_ABORTRETRYIGNORE:	BtnID = IDRETRY;	break;
		case MB_YESNOCANCEL:		BtnID = IDNO;	break;
		case MB_YESNO:				BtnID = IDNO;	break;
		case MB_RETRYCANCEL:		BtnID = IDCANCEL;	break;
		case MB_CANCELTRYCONTINUE:	BtnID = IDTRYAGAIN;	break;		
		default:
			BtnID = 0;
			break;
		}		
		break;
	case MSG_DEFBTN3:
		switch ( Flag )
		{
		case MB_OK:					BtnID = 0;	break;
		case MB_OKCANCEL:			BtnID = 0;	break;
		case MB_ABORTRETRYIGNORE:	BtnID = IDIGNORE;	break;
		case MB_YESNOCANCEL:		BtnID = IDCANCEL;	break;
		case MB_YESNO:				BtnID = 0;	break;
		case MB_RETRYCANCEL:		BtnID = 0;	break;
		case MB_CANCELTRYCONTINUE:	BtnID = IDCONTINUE;	break;		
		default:
			BtnID = 0;
			break;
		}		
		break;
	}
	return BtnID;
}
//-------------------------------------------------------------------------------------//
int CMessageBoxWnd::GetMessageBtnCountUsed()//取得訊息按鈕使用數量
{
	int Count=0;
	const std::vector<UINT> &BtnIDList=m_BtnIDList;
	const size_t BtnCnt=BtnIDList.size();
	for ( size_t i=0; i<BtnCnt; i++ )
	{
		UINT BtnID=BtnIDList[i];
		if ( false==m_BtnUsdMap[BtnID] ) { continue; }
		Count ++;
	}
	return Count;
}
//-------------------------------------------------------------------------------------//
SIZE CMessageBoxWnd::GetMessageBtnSize()//取得訊息按鈕尺寸
{	
	size_t i=0;		
	CString str;
	UINT BtnID=0;	
	CWnd *pWnd=NULL;		
	int szX=0, szY=0;
	int szMaxX=0, szMaxY=0;
	const bool bUserFont=CheckUIUseTextFont();
	const size_t BtnCount=m_BtnIDList.size();	
	for ( i=0; i<BtnCount; i++ )
	{	
		BtnID=m_BtnIDList[i];
		if ( false==m_BtnUsdMap[BtnID] ) { continue; }

		CWnd::GetDlgItemText(BtnID, str);		
		CWnd *WndPtr=CWnd::GetDlgItem(BtnID);
		CalcMsgStringSize(WndPtr, str, bUserFont, szX, szY);
		if ( szMaxX < szX ) { szMaxX=szX; }
		if ( szMaxY < szY ) { szMaxY=szY; }
	}

	SIZE sz{0, 0};
	sz.cx = szMaxX+16;
	sz.cy = szMaxY+8;
	sz.cx = MAX(sz.cx, 88);
	sz.cy = MAX(sz.cy, 30);
	return sz;
}
//-------------------------------------------------------------------------------------//
bool CMessageBoxWnd::LoadMessageIcon()//載入訊息圖示
{
	HICON hIcon=NULL;	
	CWinApp *pWinApp=AfxGetApp();
	UINT IconID=GetMessageIconID();
	if ( NULL == pWinApp ) { return false; }	
	switch ( IconID )
	{		
	case MB_ICONHAND:		hIcon=pWinApp->LoadStandardIcon((IDI_HAND)); break;
	case MB_ICONASTERISK:	hIcon=pWinApp->LoadStandardIcon((IDI_ASTERISK)); break;
	case MB_ICONQUESTION:	hIcon=pWinApp->LoadStandardIcon((IDI_QUESTION)); break;
	case MB_ICONEXCLAMATION:hIcon=pWinApp->LoadStandardIcon((IDI_EXCLAMATION)); break;
	//case MB_ICONWARNING:	hIcon=pWinApp->LoadStandardIcon((IDI_WARNING)); break;
	//case MB_ICONERROR:		hIcon=pWinApp->LoadStandardIcon((IDI_ERROR)); break;
	//case MB_ICONINFORMATION:hIcon=pWinApp->LoadStandardIcon((IDI_INFORMATION)); break;
	//case MB_ICONSTOP:		hIcon=pWinApp->LoadStandardIcon((IDI_HAND)); break;
	default:
		hIcon=NULL;
		break;
	}
	m_MsgIcon=hIcon;
	if ( NULL != hIcon )
	{	m_MessageIcon.SetIcon(hIcon); }
	return true;
}
//-------------------------------------------------------------------------------------//
SIZE CMessageBoxWnd::GetMessageIconSize() const//取得訊息圖示尺寸
{
	SIZE sz{0, 0};
	ICONINFO IconInf;
	HICON hIcon=m_MsgIcon;
	if ( NULL == hIcon ) { return sz; }	
	::memset(&IconInf, 0x00, sizeof(IconInf));
	if ( ::GetIconInfo(hIcon, &IconInf) == FALSE ) { return sz; }	
	
	BITMAP bm;	
	BOOL fResult = GetObject(IconInf.hbmMask, sizeof(bm), &bm) == sizeof(bm);
	if (fResult)
	{
      sz.cx = bm.bmWidth;
      sz.cy = IconInf.hbmColor ? bm.bmHeight : bm.bmHeight / 2;
    }
    if (IconInf.hbmMask)  DeleteObject(IconInf.hbmMask);
    if (IconInf.hbmColor) DeleteObject(IconInf.hbmColor);
	return sz;
	sz.cx += 16;
	sz.cy += 16;	
	return sz;
}
//-------------------------------------------------------------------------------------//
bool CMessageBoxWnd::ShowCtrlWnd(UINT ID, BOOL bShow)//顯示控制視窗
{
	if ( NULL == this ) { return false; }
	CWnd *pWnd=GetDlgItem(ID);
	if ( NULL == pWnd ) { return false; }
	if ( NULL == pWnd->GetSafeHwnd() ) { return false; }
	if ( TRUE == bShow )
	{	pWnd->ShowWindow(SW_SHOW); }
	else
	{	pWnd->ShowWindow(SW_HIDE); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMessageBoxWnd::CheckUseTextFont() const//確認使用文字字型
{
	if ( 0 == m_FontSize ) { return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CMessageBoxWnd::CheckUIUseTextFont() const//確認介面使用字型
{
	return false;
}
//-------------------------------------------------------------------------------------//
bool CMessageBoxWnd::CheckUseTextColor() const//確認使用文字顏色
{
	if ( CLR_DEFAULT == m_TextColor ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMessageBoxWnd::CheckUseTextBkColor() const//確認使用背景顏色	
{
	if ( CLR_DEFAULT == m_TextBkColor ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMessageBoxWnd::CheckEnableCloseBtn() const//確認啟用[關閉]按鈕
{
	bool bEnable=false;	
	UINT Flag=GetMsgBtnType();
	switch ( Flag )
	{
	case MB_OK:					bEnable = true;		break;
	case MB_OKCANCEL:			bEnable = true;		break;
	case MB_ABORTRETRYIGNORE:	bEnable = false;	break;
	case MB_YESNOCANCEL:		bEnable = true;		break;
	case MB_YESNO:				bEnable = false;	break;
	case MB_RETRYCANCEL:		bEnable = true;		break;
	case MB_CANCELTRYCONTINUE:	bEnable = true;		break;		
	default:
		bEnable=false;
		break;
	}
	return bEnable;
}
//-------------------------------------------------------------------------------------//
bool CMessageBoxWnd::CalcMsgStringSize(CWnd *WndPtr, LPCTSTR str, bool bUserFont, int &szX, int &szY)//計算訊息字串
{		
	if ( CWnd::GetSafeHwnd() == NULL ) { return false; }

	bool bEndLoop=false;
	int Rows=0, MaxCols=0;		
	int CurPos=0, LastPos=0;
	CSize sz(0, 0), szMax(0, 0);
	CString Str=str;
	CString SubStr;	
	CClientDC dc(this);
	//Str = _T("ABCD\n");		
	const int Len=Str.GetLength();
	int MaxColStart=0, MaxColEnd=Len;
	if ( true == bUserFont )
	{
		if ( CheckUseTextFont() == true )
		{	dc.SelectObject(m_TextFont); }
	}	
	for ( int i=0; i<Len; i++ )
	{
		CurPos=Str.Find(_T("\n"), CurPos+1);
		if ( -1==CurPos )
		{
			CurPos = Len;
			bEndLoop = true;			
		}
		Rows ++;
		//字元數多少不代表實際顯示尺寸大小
		SubStr = Str.Mid(LastPos, CurPos-LastPos);
		sz = dc.GetTextExtent(SubStr);
		if ( szMax.cx < sz.cx ) { szMax.cx = sz.cx; }
		if ( szMax.cy < sz.cy ) { szMax.cy = sz.cy; }

		if ( (CurPos-LastPos) > MaxCols )
		{
			MaxCols = CurPos-LastPos;
			MaxColStart = LastPos;
			MaxColEnd = CurPos;
		}
		if ( false==bEndLoop )
		{	CurPos ++; }
		LastPos=CurPos;
		if ( true == bEndLoop )
		{	break; }
	}
	
	Rows = MIN(Rows, 16);//最多十列	
	szX=szMax.cx;
	szY=szMax.cy*Rows;
	return true;
}
//-------------------------------------------------------------------------------------//
void CMessageBoxWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	bool bEnableCloseBtn=CheckEnableCloseBtn();
	if ( false == bEnableCloseBtn ) 
	{	return; }
	UINT BtnID=GetMsgBtnType();
	if ( MB_OK == BtnID )
	{
		CBaseDialog::OnOK();
		return;
	}
	CBaseDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CMessageBoxWnd::OnDefBtn1() 
{
	// TODO: Add your control notification handler code here
	UINT BtnID=0;	
	BtnID = GetMessageBtnID(MSG_DEFBTN1);
	if ( 0 != BtnID ) 
	{	EndDialog(BtnID);	}
}
//-------------------------------------------------------------------------------------//
void CMessageBoxWnd::OnDefBtn2() 
{
	// TODO: Add your control notification handler code here
	UINT BtnID=0;	
	BtnID = GetMessageBtnID(MSG_DEFBTN2);
	if ( 0 != BtnID ) 
	{	EndDialog(BtnID);	}
}
//-------------------------------------------------------------------------------------//
void CMessageBoxWnd::OnDefBtn3() 
{
	// TODO: Add your control notification handler code here
	UINT BtnID=0;	
	BtnID = GetMessageBtnID(MSG_DEFBTN3);
	if ( 0 != BtnID ) 
	{	EndDialog(BtnID);	}
}
//-------------------------------------------------------------------------------------//