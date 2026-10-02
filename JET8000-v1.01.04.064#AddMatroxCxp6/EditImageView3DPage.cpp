// EditImageView3DPage.cpp : implementation file
//

#include "stdafx.h"
#include "jet8000.h"
#include "EditImageView3DPage.h"
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
// CEditImageView3DPage dialog
//-------------------------------------------------------------------------------------//
CEditImageView3DPage::CEditImageView3DPage(CWnd* pParent /*=NULL*/)
	: CDialog(CEditImageView3DPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditImageView3DPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ImageW = 0;
	m_ImageH = 0;
	m_BitCount = 0;
	m_ImageStep = 0;
	m_MaskPtr = NULL;
	m_ImagePtr = NULL;
	m_SpacePtr = NULL;
	m_ImageTmpPtr = NULL;

	m_WndPtr = NULL;
	m_ProjectPtr = NULL;
	m_TargetHegiht = -1;
	m_TargetRuleMax = -1;
	m_TargetRuleMin = -1;	
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditImageView3DPage)
	DDX_Control(pDX, VIEW3D_OPENGL_WND, m_OpenGLWnd);
	DDX_Control(pDX, VIEW3D_PROFILE_WND, m_ChartWnd);	
	DDX_Control(pDX, VIEW3D_DETAIL_LEVEL_COMBO, m_DetailLeveCombox);		
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditImageView3DPage, CDialog)
	//{{AFX_MSG_MAP(CEditImageView3DPage)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(DRAW3D_OBJECT_COLOR_MENU, OnObjectColorMenu)	
	ON_COMMAND(DRAW3D_OBJECT_LINE_MENU, OnObjectLineMenu)	
	ON_COMMAND(DRAW3D_OBJECT_TEXTURE_MENU, OnObjectTextureMenu)	
	ON_COMMAND(DRAW3D_OBJECT_COLOR_LINE_MENU, OnObjectColorLineMenu)	
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(VIEW3D_TOOLBAR_MODEL_BTN, OnToolbarModelBtn)
	ON_BN_CLICKED(VIEW3D_TOOLBAR_SHOW_BTN, OnToolbarShowBtn)
	ON_COMMAND(DRAW3D_UPPER_PLANE_MENU, OnUpperPlaneMenu)	
	ON_COMMAND(DRAW3D_LOWER_PLANE_MENU, OnLowerPlaneMenu)	
	ON_COMMAND(DRAW3D_DEFAULT_MENU, OnDefaultMenu)	
	ON_COMMAND(DRAW3D_BASE_PLANE_MENU, OnBasePlaneMenu)
	ON_COMMAND(DRAW3D_PLANE_PITCH_MENU, OnPlanePitchMenu)
	ON_COMMAND(DRAW3D_PLANE_HEIGHT_MENU, OnPlaneHeightMenu)	
	ON_BN_CLICKED(VIEW3D_OBJECT_COLOR_RADIO, OnObjectColorRadio)
	ON_BN_CLICKED(VIEW3D_OBJECT_LINE_RADIO, OnObjectLineRadio)
	ON_BN_CLICKED(VIEW3D_OBJECT_TEXTURE_RADIO, OnObjectTextureRadio)
	ON_BN_CLICKED(VIEW3D_OBJECT_COLOR_LINE_RADIO, OnObjectColorLineRadio)
	ON_BN_CLICKED(VIEW3D_PLANE_UPPER_CHK, OnPlaneUpperChk)
	ON_BN_CLICKED(VIEW3D_PLANE_BASE_CHK, OnPlaneBaseChk)
	ON_BN_CLICKED(VIEW3D_PLANE_LOWER_CHK, OnPlaneLowerChk)
	ON_BN_CLICKED(VIEW3D_PLANE_PITCH_BTN, OnPlanePitchBtn)
	ON_BN_CLICKED(VIEW3D_VIEW_DEFAULT_BTN, OnViewDefaultBtn)
	ON_BN_CLICKED(VIEW3D_ANIMATION_PLAY_BTN, OnAnimationPlayBtn)
	ON_BN_CLICKED(VIEW3D_ANIMATION_STOP_BTN, OnAnimationStopBtn)
	ON_BN_CLICKED(VIEW3D_PLANE_CLIP_CHK, OnPlaneClipChk)
	ON_BN_CLICKED(VIEW3D_PLANE_CLIP_HOR_RDO, OnPlaneClipHorRdo)
	ON_BN_CLICKED(VIEW3D_PLANE_CLIP_VER_RDO, OnPlaneClipVerRdo)
	ON_BN_CLICKED(VIEW3D_PLANE_CLIP_ANY_RDO, OnPlaneClipAnyRdo)	
	ON_CBN_SELCHANGE(VIEW3D_DETAIL_LEVEL_COMBO, OnSelchangeDetailLevelCombo)
	ON_BN_CLICKED(VIEW3D_VIEW_WND_BTN, OnViewWndBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditImageView3DPage message handlers
//-------------------------------------------------------------------------------------//
BOOL CEditImageView3DPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//CMenu menu;
	//menu.LoadMenu(IDR_DRAW3D_MENU);CEditImageView3DPage::OnInitDialog
	//CDialog::SetMenu(&menu);
	//menu.Detach();		
	BuildDetailLeveCombox();
	CWnd::CheckDlgButton(VIEW3D_VIEW_WND_BTN, TRUE);
	CWnd::SetDlgItemInt(VIEW3D_COLOR_RANGE_MAX_EDIT, (UINT)(-1));
	CWnd::SetDlgItemInt(VIEW3D_COLOR_RANGE_MIN_EDIT, (UINT)(-1));

	//m_OpenGLWnd.SetClipPlaneOpen(true);	
	m_OpenGLWnd.InitialDisplay();	
	m_OpenGLWnd.SetMainObjectMode(OPENGL_OBJECT_TEXTURE);
	m_OpenGLWnd.CreateMaxDataBuffer();
	m_OpenGLWnd.SetDetailLevel(OPEN_GL_FINENESS_LOW_MOST);//OPEN_GL_FINENESS_LOW_MORE, OPEN_GL_FINENESS_LOW, OPEN_GL_FINENESS_HIGHT
	InitProfileWnd(m_ChartWnd);
	ShowProfileWnd(m_ChartWnd);
	AdjustView3DWnd();
	UpdateCtrlUIEnabled();
	UpdateParamToUI();
	SwitchMultiLanguage();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ClearImageBuffer();
	ReleaseUniFrameList();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( GetSafeHwnd() == NULL ) { return; }
	AdjustView3DWnd();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnOK() 
{
	// TODO: Add extra validation here
	return ;
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnCancel() 
{
	// TODO: Add extra cleanup here
	return ;
	CDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::AdjustView3DWnd()
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }
	if ( this->m_OpenGLWnd.GetSafeHwnd() == NULL )
	{	return; }

	//return ;
	SIZE size={0};
	RECT WndRect={0};
	RECT MainWndRect={0};
	RECT ProfileRect={0};
	RECT SldRectX1={0};
	RECT SldRectY1={0};
	RECT SldRectX2={0};
	RECT SldRectY2={0};
	int  WndGap = 2;

	this->GetClientRect(&MainWndRect);	
	const int MaxOpenGLSizeW = 480;
	const int cx = MainWndRect.right-MainWndRect.left;
	const int cy = MainWndRect.bottom-MainWndRect.top;
	const int WndW = MIN(cx-WndGap-WndGap, MaxOpenGLSizeW);
	const int WndH = cy-WndGap-WndGap;
	const int OpenGLCy=MIN(WndW, WndH);	

	this->m_OpenGLWnd.GetWindowRect(&WndRect);
	this->ScreenToClient(&WndRect);	
	if ( 0 == cx )//20250905
	{
		WndRect.left = MainWndRect.left;
		WndRect.right = WndRect.left+1;	
	}
	else
	{
		WndRect.left = MainWndRect.left+WndGap;
		WndRect.right = MainWndRect.right-WndGap;	
	}
	if ( 0 == cy )
	{	WndRect.bottom = WndRect.top+1;	}
	else
	{	WndRect.bottom = WndRect.top+OpenGLCy;	}	
	m_OpenGLWnd.MoveWindow(&WndRect);	
	m_OpenGLWnd.Invalidate();

	const int OpenGLWndHEnd = WndRect.bottom;
	const int ProfileWndH = 240;
	WndRect.top = OpenGLWndHEnd+4;
	WndRect.bottom = WndRect.top+ProfileWndH;
	if ( m_ChartWnd.GetSafeHwnd() != NULL )
	{	m_ChartWnd.MoveWindow(&WndRect);	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_IMAGE_VIEW3D_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_DRAW3D_WND;
	WndKey = _T("IDD_EDIT_IMAGE_VIEW3D_WND");
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
	WndID = VIEW3D_TOOLBAR_MODEL_BTN;
	WndKey = _T("VIEW3D_TOOLBAR_MODEL_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_TOOLBAR_SHOW_BTN;
	WndKey = _T("VIEW3D_TOOLBAR_SHOW_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = VIEW3D_OBJECT_COLOR_RADIO;
	WndKey = _T("VIEW3D_OBJECT_COLOR_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_OBJECT_LINE_RADIO;
	WndKey = _T("VIEW3D_OBJECT_LINE_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_OBJECT_TEXTURE_RADIO;
	WndKey = _T("VIEW3D_OBJECT_TEXTURE_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_OBJECT_COLOR_LINE_RADIO;
	WndKey = _T("VIEW3D_OBJECT_COLOR_LINE_RADIO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = VIEW3D_PLANE_UPPER_CHK;
	WndKey = _T("VIEW3D_PLANE_UPPER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_PLANE_BASE_CHK;
	WndKey = _T("VIEW3D_PLANE_BASE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_PLANE_LOWER_CHK;
	WndKey = _T("VIEW3D_PLANE_LOWER_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_PLANE_CLIP_CHK;
	WndKey = _T("VIEW3D_PLANE_CLIP_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_PLANE_CLIP_HOR_RDO;
	WndKey = _T("VIEW3D_PLANE_CLIP_HOR_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_PLANE_CLIP_VER_RDO;
	WndKey = _T("VIEW3D_PLANE_CLIP_VER_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_PLANE_CLIP_ANY_RDO;
	WndKey = _T("VIEW3D_PLANE_CLIP_ANY_RDO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_VIEW_DEFAULT_BTN;
	WndKey = _T("VIEW3D_VIEW_DEFAULT_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_VIEW_WND_BTN;
	WndKey = _T("VIEW3D_VIEW_WND_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_PLANE_PITCH_BTN;
	WndKey = _T("VIEW3D_PLANE_PITCH_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
	WndID = VIEW3D_ANIMATION_PLAY_BTN;
	WndKey = _T("VIEW3D_ANIMATION_PLAY_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_ANIMATION_STOP_BTN;
	WndKey = _T("VIEW3D_ANIMATION_STOP_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
	WndID = VIEW3D_COLOR_RANGE_GROUP;
	WndKey = _T("VIEW3D_COLOR_RANGE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_COLOR_RANGE_CHK;
	WndKey = _T("VIEW3D_COLOR_RANGE_CHK");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_COLOR_RANGE_MAX_LABEL;
	WndKey = _T("VIEW3D_COLOR_RANGE_MAX_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = VIEW3D_COLOR_RANGE_MIN_LABEL;
	WndKey = _T("VIEW3D_COLOR_RANGE_MIN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CEditImageView3DPage::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_IMAGE_VIEW3D_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CEditImageView3DPage::BuildDetailLeveCombox()
{
	size_t      i=0;
	int         idx=0;
	CString     str;
	DWORD       Param=0;	
	CComboBox  &Combox = m_DetailLeveCombox;

	idx = 0;
	JetAPI::ClearCombox(Combox);	

	Param = OPEN_GL_FINENESS_HIGHT_MOST;
	str = _T("Most High");
	str = LoadMultiLanguageString(str, str);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;	

	Param = OPEN_GL_FINENESS_HIGHT_MORE;
	str = _T("More High");
	str = LoadMultiLanguageString(str, str);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;	

	Param = OPEN_GL_FINENESS_HIGHT;//1
	str = _T("High");
	str = LoadMultiLanguageString(str, str);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;	

	Param = OPEN_GL_FINENESS_MIDDLE;//2
	str = _T("Middle");
	str = LoadMultiLanguageString(str, str);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;	

	Param = OPEN_GL_FINENESS_LOW;//4
	str = _T("Low");
	str = LoadMultiLanguageString(str, str);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;	

	Param = OPEN_GL_FINENESS_LOW_MORE;//4
	str = _T("More Low");
	str = LoadMultiLanguageString(str, str);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;	

	Param = OPEN_GL_FINENESS_LOW_MOST;//4
	str = _T("Most Low");
	str = LoadMultiLanguageString(str, str);
	Combox.InsertString(-1, str);
	Combox.SetItemData(idx, Param);
	idx ++;	
	
	JetAPI::SetComboxCurSel(Combox, 2);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::UpdateParamToUI()
{
	OPENGL_OBJECT_MODE ObjMode = m_OpenGLWnd.GetMainObjectMode();
	switch ( ObjMode )
	{
	case OPENGL_OBJECT_COLOR: CWnd::CheckDlgButton(VIEW3D_OBJECT_COLOR_RADIO, TRUE); break; 
	case OPENGL_OBJECT_LINE: CWnd::CheckDlgButton(VIEW3D_OBJECT_LINE_RADIO, TRUE); break; 
	case OPENGL_OBJECT_TEXTURE: CWnd::CheckDlgButton(VIEW3D_OBJECT_TEXTURE_RADIO, TRUE); break; 
	case OPENGL_OBJECT_COLOR_LINE: CWnd::CheckDlgButton(VIEW3D_OBJECT_COLOR_LINE_RADIO, TRUE); break; 
	}
	if ( m_OpenGLWnd.GetUpperPlaneOpen() == true ) 
	{	CWnd::CheckDlgButton(VIEW3D_PLANE_UPPER_CHK, TRUE); }
	if ( m_OpenGLWnd.GetBasePlaneOpen() == true ) 
	{	CWnd::CheckDlgButton(VIEW3D_PLANE_BASE_CHK, TRUE); }
	if ( m_OpenGLWnd.GetLowerPlaneOpen() == true )
	{	CWnd::CheckDlgButton(VIEW3D_PLANE_LOWER_CHK, TRUE); }
	if ( m_OpenGLWnd.GetClipPlaneOpen() == true )
	{	CWnd::CheckDlgButton(VIEW3D_PLANE_CLIP_CHK, TRUE); }

	int ClipPlaneDir = m_OpenGLWnd.GetClipPlaneDir();
	if ( OPENGL_CLIP_PLANE_HOR == ClipPlaneDir ) 
	{	CWnd::CheckDlgButton(VIEW3D_PLANE_CLIP_HOR_RDO, TRUE); }
	if ( OPENGL_CLIP_PLANE_VER == ClipPlaneDir ) 
	{	CWnd::CheckDlgButton(VIEW3D_PLANE_CLIP_VER_RDO, TRUE); }	
	if ( OPENGL_CLIP_PLANE_ANY == ClipPlaneDir ) 
	{	CWnd::CheckDlgButton(VIEW3D_PLANE_CLIP_ANY_RDO, TRUE); }	

	JetAPI::SetComboxCurSel(m_DetailLeveCombox, m_OpenGLWnd.GetDetailLevel());
}
//-------------------------------------------------------------------------------------//
BOOL CEditImageView3DPage::PreTranslateMessage(MSG* pMsg)
{		
	return CDialog::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CEditImageView3DPage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWnd *pWnd = NULL;	
	switch ( message )
	{
	case MSG_MAIN_FRAME_MESSAGE:
		switch ( wParam )
		{
		case WPARAM_PROJECT_CLOSE:
			CloseProject();
			break;
		case WPARAM_PROJECT_SWITCH:
			//UpdateWndSelected();
			break;
		case WPARAM_PROJECT_UPDATE:
			//UpdateWndSelected();
			break;		
		case WPARAM_PROJECT_CLOSE_ACTIVE_OBJ:
			break;
		default:			
			break;
		}		
		break;
	case MSG_EDIT_IMAGE_VIEW_WND:
		switch ( wParam )
		{
		case WPARAM_UPDATE_IMAGE_MODEL_SELECTED:
			if ( CWnd::IsWindowVisible() == TRUE )
			{	BuildView3DPoints(true, true);	}
			break;
		case WPARAM_UPDATE_IMAGE_MODEL_NO_PROCESS:
			if ( CWnd::IsWindowVisible() == TRUE )
			{	BuildView3DPoints(true, false);	}
			break;
		case WPARAM_UPDATE_IMAGE_WND_SELECTED:
		case WPARAM_UPDATE_IMAGE_WND_SELECTED_NO_PROCESS:
			if ( CWnd::IsWindowVisible() == TRUE )
			{
				BuildView3DPoints(false, false); UpdateView3DBox();	}
			break;
		case WPARAM_UPDATE_IMAGE_SWITCH_FRAME:
			if ( CWnd::IsWindowVisible() == TRUE )
			{	BuildView3DPoints(true, false); }
			break;
		}
		break;
	case MSG_EDIT_VIEW_3D_WND:
		switch ( wParam )
		{
		case WPARAM_UPDATE_3D_DATA:
			BuildView3DPoints(true, false);
			break;		
		case WPARAM_UPDATE_3D_WND_SELECTED:		
			UpdateView3DBox();
			break;
		default:
			//UpdateWndSelected();
			break;
		}
		break;
	case MSG_OPEN_GL_WND:
		switch( wParam )
		{
		case WPARAM_UPDATE_CLIP_PLANE:		
			BuildProfileWnd(m_ChartWnd);			
			break;
		}
		break;
	}	
	return CDialog::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::ReleaseUniFrameList()
{
	
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::CloseProject()
{
	m_WndPtr = NULL;
	m_ProjectPtr = NULL;	
	ClearImageBuffer();
	ClearView3DPoints();		
}
//-------------------------------------------------------------------------------------//
CAOIProject* CEditImageView3DPage::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::SetImageBuffer(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE Bit, IMAGE_PTR Ptr2D, MASK_PTR PtrMsk, SPACE_PTR Ptr3D)
{
	ClearImageBuffer();
	if ( NULL==Ptr2D || NULL==Ptr3D || NULL==PtrMsk ) 
	{	return; }

	m_ImageW = W;
	m_ImageH = H;
	m_BitCount = Bit;	
	m_ImageStep = Step;
	m_MaskPtr = PtrMsk;
	m_ImagePtr = Ptr2D;	
	m_SpacePtr = Ptr3D;
	return;
}
//-------------------------------------------------------------------------------------//
bool CEditImageView3DPage::GetImageBuffer(IMAGE_SIZE &W, IMAGE_SIZE &H, IMAGE_SIZE &Step, IMAGE_SIZE &Bit, IMAGE_PTR &Ptr2D, MASK_PTR &PtrMsk, SPACE_PTR &Ptr3D)
{
	if ( NULL==m_ImagePtr || NULL==m_SpacePtr || NULL==m_MaskPtr ) 
	{	return false; }
	W = m_ImageW;
	H = m_ImageH;
	Bit = m_BitCount;
	Step = m_ImageStep;
	PtrMsk = m_MaskPtr;
	Ptr2D = m_ImagePtr;
	Ptr3D = m_SpacePtr;
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::ClearImageBuffer()
{	
	if ( NULL != m_MaskPtr ) 
	{	JetMemory.free_func(m_MaskPtr); }
	if ( NULL != m_ImagePtr ) 
	{	JetMemory.free_func(m_ImagePtr); }
	if ( NULL != m_SpacePtr ) 
	{	JetMemory.free_func(m_SpacePtr); }
	if  (NULL != m_ImageTmpPtr)
	{	JetMemory.free_func(m_ImageTmpPtr); }

	m_ImageW = 0;
	m_ImageH = 0;
	m_BitCount = 0;
	m_ImageStep = 0;
	m_ImagePtr = NULL;
	m_SpacePtr = NULL;
	m_ImageTmpPtr = NULL;
	return;
}
//-------------------------------------------------------------------------------------//
bool CEditImageView3DPage::ClearView3DPoints()//清除顯示3D的資料
{
	const char fnName[] = "CEditImageView3DPage::ClearView3DPoints()";
	bool           IsColor = true;
	float         *Buffer3DPtr=NULL;	
	unsigned char *Buffer2DPtr=NULL;
	IMAGE_SIZE ImageW=200;
	IMAGE_SIZE ImageH=200;
	IMAGE_SIZE BitCount = 24;
	IMAGE_SIZE ImageStep=JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, Buffer3DPtr, fnName, "Buffer3DPtr") == false ||
		 JetMemory.alloc_func(BufferSize, Buffer2DPtr, fnName, "Buffer2DPtr") == false )
	{
		JetMemory.free_func(Buffer2DPtr);
		JetMemory.free_func(Buffer3DPtr);
		return false;
	}
	RECT PadRect={0,0,0,0};
	RECT RoiRect={0,0,0,0};
	const double Max = 0;	

	::memset(Buffer3DPtr, 0x00, sizeof(float)*BufferSize);
	::memset(Buffer2DPtr, 0x00, sizeof(unsigned char)*BufferSize);
	
	m_OpenGLWnd.ClearGLBoxList();
	m_OpenGLWnd.UpdateGLBoxList();
	//m_OpenGLWnd.ReleaseAllBuffer();//避免記憶體釋放, 給予相當小的資料
	m_OpenGLWnd.Set3DData(Buffer3DPtr, Buffer2DPtr, ImageW, ImageH, IsColor, -1, -1, -1, -1, PadRect, RoiRect, Max);
	m_OpenGLWnd.SetIsShowMainObject(false);
	m_OpenGLWnd.SetModalCenter(true);

	JetMemory.free_func(Buffer2DPtr);
	JetMemory.free_func(Buffer3DPtr);

	BuildProfileWnd(m_ChartWnd);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageView3DPage::GetRuleColorRange(float &MinH, float &MaxH)
{	
	BOOL bColorRange=CWnd::IsDlgButtonChecked(VIEW3D_COLOR_RANGE_CHK);
	if ( TRUE == bColorRange )
	{
		MinH = (int)(CWnd::GetDlgItemInt(VIEW3D_COLOR_RANGE_MIN_EDIT));
		MaxH = (int)(CWnd::GetDlgItemInt(VIEW3D_COLOR_RANGE_MAX_EDIT));
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::BuildView3DPoints(bool UpdateView, bool ResetZoom)//建立顯示3D的資料
{
	if ( ExecBuildView3DPoints(UpdateView, ResetZoom) == false ) 
	{	ClearView3DPoints(); }
	return;
}
//-------------------------------------------------------------------------------------//
bool CEditImageView3DPage::ExecBuildView3DPoints(bool UpdateView, bool ResetZoom)//建立顯示3D的資料
{	
	m_WndPtr = NULL;
	m_ProjectPtr = NULL;
	ClearImageBuffer();
	ReleaseUniFrameList();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) 
	{	return false;		}
	m_ProjectPtr = ProjectPtr;
	
	CAOIModel *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();
	if ( NULL == ModelPtr ) 
	{	return false;		}		
	
	CAOIWnd *WndPtr = NULL;
	TALG_PARAM_RESIN_HEIGHT Param;
	for (int i = 0; i < ModelPtr->GetModelWndCount(); i++)
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if (WndPtr->GetWndDefectID() == WND_DEFECT_PART_ALIGN)
		{
			Param = WndPtr->GetWndAlgParam().GetAlgParamResinHeight();
			break;
		}
	}

	CString str;	
	const bool bCloned = true;
	TUNI_FRAME  *UniFramePtr = NULL;
	std::vector<TUNI_FRAME> UniFrameList;	
	const bool bUpdate3D = CheckUpdate3DBoxData();

	if ( AOIDataCollect.CopyModelUniFrameList(UniFrameList, bCloned) == false )
	{	return false;	}

	const size_t UniFrameCount = UniFrameList.size();
	if ( 0 == UniFrameCount ) 
	{	return false;	}	

	size_t i=0, j=0;
	size_t BufferSize=0;
	double Value=0, Sum=0, Ave=0, Max=0;
	unsigned int FrameIndex = ProjectPtr->GetProjectMapIndex();
	const double BaseLevel = ProjectPtr->GetProjectSpaceBaseHeight();			
	if ( FrameIndex >= UniFrameCount ) 
	{	FrameIndex = 0;	}	
	for ( i=0; i<UniFrameCount; i++ )
	{
		UniFramePtr = &(UniFrameList[i]);
		if ( NULL == UniFramePtr ) { continue; }
		if ( i == FrameIndex )
		{	AOIDataCollect.ExecEnhanceDisplayImage(UniFramePtr->ImageW, UniFramePtr->ImageH, UniFramePtr->ImageStep, UniFramePtr->BitCount, UniFramePtr->ImagePtr, UniFramePtr->ImagePtr); }

		if ( NULL == UniFramePtr->SpacePtr ) { continue; }		
		BufferSize = UniFramePtr->ImageStep*UniFramePtr->ImageH;
		Max = 0;
		for ( j=0; j<BufferSize; j++ )
		{
			Value = UniFramePtr->SpacePtr[j];
			if ( 0 == j ) 
			{	Max = Value;	}
			else
			{
				if ( Max < Value ) { Max = Value; }
			}
		}
	}

	TUNI_FRAME UniFrame = UniFrameList[FrameIndex];	
	bool   IsColor = false;
	const int ImageW = (int)(UniFrame.ImageW);
	const int ImageH = (int)(UniFrame.ImageH);
	const int BitCount = (int)(UniFrame.BitCount);	
	const int ImageStep = (int)(UniFrame.ImageStep);	
	int Image3DStep = ImageW;
	float *pSrc3D = NULL;
	MASK_PTR pMask = NULL;
	
	unsigned char *pSrc2D = NULL;
	if ( 24 == BitCount ) 
	{	IsColor = true; }
	else
	{	IsColor = false; }

	pSrc3D = NULL;
	for ( i=0; i<UniFrameCount; i++ )
	{
		if ( NULL != UniFrameList[i].SpacePtr )
		{	
			Image3DStep = (int)(UniFrameList[i].ImageStep);	
			pSrc3D = UniFrameList[i].SpacePtr;
			pMask = UniFrameList[i].MaskPtr;
			break;
		}
	}

	if (Param.enabled)	// 按鈕
	{
		BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		if (JetMemory.alloc_func(BufferSize, m_ImageTmpPtr, "EditImageView3D", "m_ImageTmpPtr") == true)
		{
			::memcpy(m_ImageTmpPtr, UniFrame.ImagePtr, sizeof(IMAGE_DATA)*BufferSize);
			size_t srcIdx = 0;
			POINT point;
			for (int k = 0; k< Param.partPoint.size(); k++)
			{
				for (j = 0; j< Param.partPoint[k].size(); j++)
				{
					if (Param.partPoint[k][j].y > 0 || Param.partPoint[k][j].x > 0)
					{
						point.x = Param.partPoint[k][j].x;
						point.y = Param.partPoint[k][j].y;

						srcIdx = point.y * ImageStep;
						//畫粗
						m_ImageTmpPtr[srcIdx + (point.x * 3)-3] = 255;
						m_ImageTmpPtr[srcIdx + (point.x * 3) -2] = 255;
						m_ImageTmpPtr[srcIdx + (point.x * 3) -1] = 255;

						m_ImageTmpPtr[srcIdx + (point.x*3)] = 255;
						m_ImageTmpPtr[srcIdx + (point.x*3)+1] = 255;
						m_ImageTmpPtr[srcIdx + (point.x*3)+2] = 255;

						m_ImageTmpPtr[srcIdx + (point.x * 3)+3] = 255;
						m_ImageTmpPtr[srcIdx + (point.x * 3) + 4] = 255;
						m_ImageTmpPtr[srcIdx + (point.x * 3) + 5] = 255;
					}
				}
			}
			for (int k = 0; k< Param.tinPoint.size(); k++)
			{
				for (j = 0; j< Param.tinPoint[k].size(); j++)
				{
					srcIdx = Param.tinPoint[k][j].y * ImageStep;
					if (Param.tinPoint[k][j].y > 0 || Param.tinPoint[k][j].x > 0)
					{
						point.x = Param.tinPoint[k][j].x;
						point.y = Param.tinPoint[k][j].y;

						srcIdx = point.y * ImageStep;

						m_ImageTmpPtr[srcIdx + (point.x * 3) - 3] = 255;
						m_ImageTmpPtr[srcIdx + (point.x * 3) - 2] = 255;
						m_ImageTmpPtr[srcIdx + (point.x * 3) - 1] = 255;

						m_ImageTmpPtr[srcIdx + (point.x * 3)] = 255;
						m_ImageTmpPtr[srcIdx + (point.x * 3) + 1] = 255;
						m_ImageTmpPtr[srcIdx + (point.x * 3) + 2] = 255;

						m_ImageTmpPtr[srcIdx + (point.x * 3) + 3] = 255;
						m_ImageTmpPtr[srcIdx + (point.x * 3) + 4] = 255;
						m_ImageTmpPtr[srcIdx + (point.x * 3) + 5] = 255;
					}
				}
			}

			pSrc2D = m_ImageTmpPtr;
		}
		else
		{
			JetMemory.free_func(m_ImageTmpPtr);
		}
	}
	else
	{
		pSrc2D = UniFrame.ImagePtr;
	}

	//cv::Size sz((int)(ImageW), (int)(ImageH));
	//cv::Mat M(sz, CV_8UC3, pSrc2D, ImageStep * sizeof(uchar));
	//cv::imshow("1", M);

	if ( NULL == pSrc3D || NULL==pMask )
	{
		if ( true == bCloned )
		{	JetAPI::ClearUniFrameList(UniFrameList);	}
		return false;	
	}
	
	//轉成Real Image Size
	IMAGE_SIZE     BufferW=0;
	IMAGE_SIZE     BufferH=0;
	IMAGE_SIZE     BufferStep=0;
	float         *pBuffer3D = NULL;
	unsigned char *pBuffer2D = NULL;		
	unsigned char *pBufferMsk = NULL;		
	
	BufferW = ImageW;
	BufferH = ImageH;		
	if ( true == IsColor )
	{	BufferStep = BufferW*3; }	
	else
	{	BufferStep = BufferW; }	
	//影像記憶體重新排列
	if ( ImageAPI.AlignImageBuffer(ImageW, ImageH, ImageStep, BitCount, pSrc2D, BufferStep, pBuffer2D, false) == false )
	{
		if ( true == bCloned )
		{	JetAPI::ClearUniFrameList(UniFrameList);	}
		return false; 			
	}		
	if ( ImageAPI.AlignImageBuffer(ImageW, ImageH, Image3DStep, 8, pMask, BufferW, pBufferMsk, false) == false )
	{
		JetMemory.free_func(pBuffer2D);
		if ( true == bCloned )
		{	JetAPI::ClearUniFrameList(UniFrameList);	}
		return false; 			
	}
	if ( ImageAPI.AlignSpaceImageBuffer(ImageW, ImageH, Image3DStep, pSrc3D, BufferW, pBuffer3D, false) == false ) 
	{	
		JetMemory.free_func(pBuffer2D);
		JetMemory.free_func(pBufferMsk);		
		if ( true == bCloned )
		{	JetAPI::ClearUniFrameList(UniFrameList);	}
		return false; 
	}
	

	Max = 0.0;	
	float ShowMinH = -1;
	float ShowMaxH = -1;
	float RuleMinH = -1;
	float RuleMaxH = -1;
	RECT PadRect = {0, 0, 0, 0};
	RECT RoiRect = {0, 0, 0, 0};
	const double BodyHeight = ModelPtr->GetModelBodyHeight();
	if ( BodyHeight > 1.0 ) 
	{
		int nHeight = (int)(BodyHeight/100);
		RuleMaxH = (nHeight+1)*100; 
	}
	m_TargetHegiht = BodyHeight;
	m_TargetRuleMax = RuleMaxH;
	m_OpenGLWnd.ClearGLBoxList();
	m_OpenGLWnd.UpdateGLBoxList();

	GetRuleColorRange(RuleMinH, RuleMaxH);
	if ( false == bUpdate3D )
	{	
		m_OpenGLWnd.Set3DData(pBuffer3D, pBuffer2D, BufferW, BufferH, IsColor, RuleMinH, RuleMaxH, ShowMinH, ShowMaxH, PadRect, RoiRect, Max); 
		if ( true == ResetZoom )
		{	m_OpenGLWnd.SetModalCenter(false);	}
		const double OpenGL3DScale = m_OpenGLWnd.Get3DScale();
		BuildProfileWnd(m_ChartWnd);
	}
	m_TargetRuleMin = m_OpenGLWnd.GetRuleMinH();
	//str.Format(_T("CEditImageView3DPage::ExecBuildView3DPoints::OpenGL3DScale(%.2f)"), OpenGL3DScale);
	//AOIDataCollect.SaveCurrentProcess(str);

	//JetMemory.free_func(pBuffer2D);
	//JetMemory.free_func(pBuffer3D);
	//JetMemory.free_func(pBufferMsk);	
	SetImageBuffer(BufferW, BufferH, BufferStep, BitCount, pBuffer2D, pBufferMsk, pBuffer3D);
	if ( true == bCloned )
	{	JetAPI::ClearUniFrameList(UniFrameList);	}

	if ( true == bUpdate3D )
	{	UpdateView3DBox_Data();	}
	else
	{
		if ( true == UpdateView )	
		{	UpdateView3DBox_Rect();	}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::UpdateView3DBox()//更新顯示3D的資料
{
	const bool bUpdate3D = CheckUpdate3DBoxData();	
	if ( true == bUpdate3D )
	{	UpdateView3DBox_Data();	}
	else
	{
		if ( NULL == m_WndPtr )//前一張為框3D模式
		{	UpdateView3DBox_Rect();	}
		else
		{	BuildView3DPoints(true, true);	 }
	}	
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::UpdateView3DBox_Data()//更新顯示3D的資料
{
	CAOIModel   *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();
	if ( NULL == ModelPtr ) { return; }		
	CAOIWnd     *WndPtr = ModelPtr->GetModelWndActived();	
	if ( NULL == WndPtr ) { return; }		
	CAOILand    *LandPtr = WndPtr->GetWndLandPtr();	
	if ( NULL == LandPtr ) { return; }
	if ( m_WndPtr == WndPtr )
	{	return ;} //相同資料檢測框就不要再度更新
	if ( LandPtr->GetLandLeadTipBox().GetBoxEnabled() == true )//啟用引腳前端
	{	m_TargetHegiht = LandPtr->GetLandLeadTipHeight();	}
	else if ( LandPtr->GetLandLeadShoulderBox().GetBoxEnabled() == true )//啟用引腳根部
	{	m_TargetHegiht = LandPtr->GetLandLeadShoulderHeight();	}
	else if ( LandPtr->GetLandLeadBox().GetBoxEnabled() == true )//啟用引腳
	{	m_TargetHegiht = LandPtr->GetLandLeadHeight();	}
	else//套用本體高度
	{	m_TargetHegiht = ModelPtr->GetModelBodyHeight();	}
	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);

	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	IMAGE_PTR  Ptr2D=NULL;
	IMAGE_PTR  PtrMsk=NULL;
	SPACE_PTR  Ptr3D=NULL;
		
	int    nHeightCnt=0;	
	bool   bShowBox=false;
	bool   bShowRoi=false;
	double fHeight = 0;
	double fSkewAngle =0.0;
	float  fPadHeight=0.0f;
	float  fRoiHeight=0.0f;
	float  fExtraHeight = 1.0f;	
	RECT PadRect = {0, 0, 0, 0};
	RECT RoiRect = {0, 0, 0, 0};
	RECT LocRect = {0, 0, 0, 0};
	RECT WndRect = {0, 0, 0, 0};
	RECT LandRect = {0, 0, 0, 0};
	RECT ModelRect = {0, 0, 0, 0};	
	TREGION4D ModelRgn, LandRgn, WndRgn, BoxRgn;
	TPOINT2D  BoxCornerPts[4];	
	TPOINT2D  PadCornerPts[4];
	TPOINT2D  RgnCp, Scale, ImageCp;	
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	if ( GetImageBuffer(ImageW, ImageH, ImageStep, BitCount, Ptr2D, PtrMsk, Ptr3D) == false )
	{	return; }

	ModelPtr->GetModelTotalRegion(ModelRgn);

	const int Gap = 128;//pixel
	const double RegionW = ModelRgn.GetWidth();
	const double RegionH = ModelRgn.GetHeight();
	Scale.x = ImageW;
	Scale.y = ImageH;
	Scale.x = Scale.x/RegionW;
	Scale.y = Scale.y/RegionH;
	RgnCp.x = ModelRgn.GetCpX();
	RgnCp.y = ModelRgn.GetCpY();
	ImageCp.x = ImageW;
	ImageCp.y = ImageH;
	ImageCp.x = ImageCp.x*0.5;
	ImageCp.y = ImageCp.y*0.5;	
	WndPtr->GetWndUseRegion(WndRgn);
	LandPtr->UpdateLandTotalRegion();
	LandPtr->GetLandTotalRegion(LandRgn);
	JetAPI::SizeToRect(ImageW, ImageH, ModelRect);
	CAOIModel::CalcModelBoxRegionRect(WndRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, WndRect);
	CAOIModel::CalcModelBoxRegionRect(LandRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, LandRect);	
	JetAPI::UnionRect(WndRect, LandRect, LocRect);
	InflateRect(&LocRect, Gap, Gap);
	JetAPI::IntersectRect(LocRect, ModelRect, LocRect);	
	
	TPOINT2D   LocCp;
	size_t     u=0, v=0;
	size_t     i=0, j=0, k=0;
	bool       IsColor=false;
	IMAGE_SIZE LocW=0;
	IMAGE_SIZE LocH=0;
	IMAGE_SIZE LocStep=0;
	IMAGE_PTR  LocPtr2D=NULL;
	SPACE_PTR  LocPtr3D=NULL;
	
	LocW = LocRect.right-LocRect.left;
	LocH = LocRect.bottom-LocRect.top;	
	LocCp.x = (LocRect.left+LocRect.right)*0.5;
	LocCp.y = (LocRect.top+LocRect.bottom)*0.5;
	if ( 24 == BitCount ) 
	{	
		IsColor = true;
		LocStep = LocW*3; 
	}
	else
	{
		IsColor = false;
		LocStep = LocW; 	
	}
	const size_t LocSize3D=LocW*LocH;
	const size_t LocSize2D=LocStep*LocH;
	if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, Ptr2D, LocRect, LocStep, LocPtr2D, false) == false )
	{
		JetMemory.free_func(LocPtr2D);
		JetMemory.free_func(LocPtr3D);
		return ; 
	}
	if ( ImageAPI.ExtractSpaceGrayRoiImage(ImageW, ImageH, ImageW, Ptr3D, LocRect, LocW, LocPtr3D, false) == false )
	{
		JetMemory.free_func(LocPtr2D);
		JetMemory.free_func(LocPtr3D);
		return ; 
	}	

	float Max=-FLT_MAX;
	float RuleMinH=-1.0f;
	float RuleMaxH=-1.0f;
	float ShowMinH=-1.0f;
	float ShowMaxH=-1.0f;		
	float sX=1.0, sY=1.0, sZ=1.0;	
	m_OpenGLWnd.ClearGLBoxList();
	m_OpenGLWnd.UpdateGLBoxList();
	m_OpenGLWnd.GetZoomScale(sX, sY, sZ);
	GetRuleColorRange(RuleMinH, RuleMaxH);
	m_OpenGLWnd.Set3DData(LocPtr3D, LocPtr2D, LocW, LocH, IsColor, RuleMinH, RuleMaxH, ShowMinH, ShowMaxH, PadRect, RoiRect, Max);		
	if ( NULL == m_WndPtr )
	{	m_OpenGLWnd.SetModalCenter(false); }
	else
	{	m_OpenGLWnd.SetModalCenterScale(false, sX, sY, sZ);	}
	JetMemory.free_func(LocPtr2D);
	JetMemory.free_func(LocPtr3D);
	m_TargetRuleMax = m_OpenGLWnd.GetRuleMaxH();
	m_TargetRuleMin = m_OpenGLWnd.GetRuleMinH();

	if ( NULL != WndPtr )
	{
		if (DRAW_MODEL_RESULT == DrawModelMode )
		{				
			WndPtr->GetWndRegionRes(BoxRgn);	
			fSkewAngle = WndPtr->GetWndAngleSkew();
			WndPtr->GetWndCornerPosRes(PadCornerPts);			
		}
		else
		{	
			fSkewAngle = 0.0;
			WndPtr->GetWndRegion(BoxRgn);	
			WndPtr->GetWndCornerPos(PadCornerPts);
		} 
	}
	else
	{
		CAOIBox *BoxPtr = ModelPtr->GetModelBodyBoxPtr();
		if (DRAW_MODEL_RESULT == DrawModelMode )
		{	
			BoxPtr->GetBoxRegionRes(BoxRgn);	
			fSkewAngle = BoxPtr->GetBoxAngleSkew();
			BoxPtr->GetBoxCornerPosRes(PadCornerPts);
		}
		else
		{	
			fSkewAngle = 0.0;
			BoxPtr->GetBoxRegion(BoxRgn); 
			BoxPtr->GetBoxCornerPos(PadCornerPts);
		}
	}	
	bShowRoi = true;
	bShowBox = true;
	TPOINT2D OffsetPt(-LocCp.x, -(ImageH-LocCp.y));	
	OffsetPt.x += (LocW/2);
	OffsetPt.y += (LocH/2);
	JetAPI::SizeToRect(ImageW, ImageH, RoiRect);
	JetAPI::SizeToRect(ImageW, ImageH, PadRect);
	CAOIModel::CalcModelBoxRegionRect(BoxRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, PadRect);
	CAOIModel::CalcModelBoxCornerPts(PadCornerPts, ImageW, ImageH, RgnCp, Scale, ImageCp, PadCornerPts) ;
	const bool IsSkewAngle = JetAPI::CheckIsExceptionAngle(fSkewAngle);
	if ( true == IsSkewAngle )
	{	JetAPI::RotateCornerPos(fSkewAngle, PadCornerPts);	}	

	fPadHeight = 0.0f;
	fRoiHeight = 0.0f;
	for ( i=0; i<ImageW*ImageH; i++ )
	{
		if ( ImageAPI.CheckSpaceMaskValid(PtrMsk[i]) == false ) { continue; }
		if ( fRoiHeight < Ptr3D[i] ) 
		{	fRoiHeight = Ptr3D[i]; }
	}	
	if ( ImageAPI.CheckRoiRect(ImageW, ImageH, PadRect) == true ) 
	{	
		fHeight = 0;
		nHeightCnt=0;	
		for ( v=PadRect.top; v<PadRect.bottom; v++ )
		{
			for ( u=PadRect.left; u<PadRect.right; u++ )
			{
				k = (v*ImageW+u);
				if ( ImageAPI.CheckSpaceMaskValid(PtrMsk[k]) == false ) { continue; }				
				if ( fPadHeight < Ptr3D[k] ) 
				{	fPadHeight = Ptr3D[k]; }
				nHeightCnt ++;
				fHeight += Ptr3D[k];
			}
		}
		//if ( nHeightCnt > 0 ) 
		//{	fPadHeight = fHeight/nHeightCnt;	}
	}

	m_WndPtr = WndPtr;
	JetAPI::MoveRect(PadRect, OffsetPt, PadRect);
	JetAPI::MoveCornerPts(PadCornerPts, OffsetPt, PadCornerPts);
	m_OpenGLWnd.SetShowPadHeight(false);	
	m_OpenGLWnd.SetBoundPadOpen(bShowBox);
	m_OpenGLWnd.SetBoundRoiOpen(bShowRoi);			
	if ( true == IsExceptionAngle ) 
	{	m_OpenGLWnd.SetPadCornerPts(PadCornerPts, fRoiHeight+fExtraHeight);	}
	else
	{	m_OpenGLWnd.SetPadCornerPts(PadCornerPts, fPadHeight+(100*fExtraHeight));	}

	if ( CWnd::IsDlgButtonChecked(VIEW3D_VIEW_WND_BTN)==TRUE ) 
	{	m_OpenGLWnd.RotateToPadAngleByAroundHeight();	}


	TOpenGLBox   glBox;		
	double       WndSkewAngle=0.0f;
	bool         IsWndSkewAngle=false;
	const size_t ModelWndCount = ModelPtr->GetModelWndCount();
	m_OpenGLWnd.ClearGLBoxList();	
	for ( i=0; i<ModelWndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }		
		if ( WndPtr->GetWndVisibled() == false ) { continue; }
		if (DRAW_MODEL_RESULT == DrawModelMode )
		{	
			WndPtr->GetWndRegionRes(BoxRgn);				
			WndPtr->GetWndCornerPosRes(BoxCornerPts);
			WndSkewAngle = WndPtr->GetWndAngleSkew();
		}
		else
		{	
			WndSkewAngle = 0.0;
			WndPtr->GetWndRegion(BoxRgn); 
			WndPtr->GetWndCornerPos(BoxCornerPts);
		}
		if ( CAOIModel::CalcModelBoxRegionRect(BoxRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, RoiRect) == false ) { continue; }
		if ( JetAPI::CheckRectInRect(RoiRect, LocRect, true) == false ) { continue; }
		if ( CAOIModel::CalcModelBoxCornerPts(BoxCornerPts, ImageW, ImageH, RgnCp, Scale, ImageCp, BoxCornerPts) == false ) { continue; }

		IsWndSkewAngle = JetAPI::CheckIsExceptionAngle(WndSkewAngle);
		if ( true == IsWndSkewAngle )
		{	JetAPI::RotateCornerPos(WndSkewAngle, BoxCornerPts);	}		

		fHeight = 0;
		fRoiHeight=0;
		nHeightCnt=0;
		for ( v=RoiRect.top; v<RoiRect.bottom; v++ )
		{
			for ( u=RoiRect.left; u<RoiRect.right; u++ )
			{
				k = (v*ImageW+u);
				if ( ImageAPI.CheckSpaceMaskValid(PtrMsk[k]) == false ) { continue; }
				if ( fRoiHeight < Ptr3D[k] ) 
				{	fRoiHeight = Ptr3D[k];	}
				nHeightCnt ++;
				fHeight += Ptr3D[k];				
			}
		}
		if ( nHeightCnt > 0 ) 
		{	fHeight = fHeight/nHeightCnt; }
		fHeight = fRoiHeight;

		JetAPI::MoveRect(RoiRect, OffsetPt, RoiRect);
		JetAPI::MoveCornerPts(BoxCornerPts, OffsetPt, BoxCornerPts);

		glBox.bShow = true;		
		//glBox.strText.Format(_T("%d"), i+1);
		m_OpenGLWnd.ConvertCornerPtToGLBox(BoxCornerPts, fHeight+fExtraHeight, glBox);
		m_OpenGLWnd.AddGLBoxObj(glBox);
	}
	//m_OpenGLWnd.SetPadRegion(RoiCornerPts, fRoiHeight+fExtraHeight);	
	m_OpenGLWnd.UpdateGLBoxList();
	m_OpenGLWnd.SetGLBoxListOpen(true);		
	m_OpenGLWnd.UpdateWindow();//立即重繪
	BuildProfileWnd(m_ChartWnd);			
	return;
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::UpdateView3DBox_Rect()//更新顯示3D的資料	
{		
	CAOIModel   *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();
	if ( NULL == ModelPtr ) { return; }		
	CAOIWnd     *WndPtr = ModelPtr->GetModelWndActived();	
	CAOIWnd     *ActWndPtr = ModelPtr->GetModelWndActived();	
	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);

	const bool bCloned = false;
	TUNI_FRAME  *UniFramePtr = NULL;
	std::vector<TUNI_FRAME> UniFrameList;	
	if ( AOIDataCollect.CopyModelUniFrameList(UniFrameList, bCloned) == false )
	{	return;	}	

	int    u=0, v=0;
	size_t i=0, j=0, k=0;	
	const size_t UniFrameCount = UniFrameList.size();	
	float *pBuffer3D = NULL;	
	MASK_PTR pMask = NULL;
	int ImageW = 0;
	int ImageH = 0;
	int ImageStep=0;
	int BitCount = 0;
	for ( i=0; i<UniFrameCount; i++ )
	{
		if ( NULL != UniFrameList[i].SpacePtr )
		{	
			ImageW = (int)(UniFrameList[i].ImageW);
			ImageH = (int)(UniFrameList[i].ImageH);
			ImageStep = (int)(UniFrameList[i].ImageStep);			
			BitCount = (int)(UniFrameList[i].BitCount);			
			pBuffer3D = UniFrameList[i].SpacePtr;
			pMask = UniFrameList[i].MaskPtr;
			break;
		}
	}
	if ( NULL == pBuffer3D || NULL==pMask )
	{	return;	}
	
	int    nHeightCnt=0;	
	bool   bShowBox=false;
	bool   bShowRoi=false;
	double fHeight = 0;
	double fSkewAngle =0.0;
	float  fPadHeight=0.0f;
	float  fRoiHeight=0.0f;
	float  fExtraHeight = 1.0f;	
	RECT PadRect = {0, 0, 0, 0};
	RECT RoiRect = {0, 0, 0, 0};
	TREGION4D ModelRgn, BoxRgn;
	TPOINT2D  BoxCornerPts[4];	
	TPOINT2D  PadCornerPts[4];
	TPOINT2D  RgnCp, Scale, ImageCp;	
	DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
	ModelPtr->GetModelTotalRegion(ModelRgn);

	const double RegionW = ModelRgn.GetWidth();
	const double RegionH = ModelRgn.GetHeight();
	Scale.x = ImageW;
	Scale.y = ImageH;
	Scale.x = Scale.x/RegionW;
	Scale.y = Scale.y/RegionH;
	RgnCp.x = ModelRgn.GetCpX();
	RgnCp.y = ModelRgn.GetCpY();
	ImageCp.x = ImageW;
	ImageCp.y = ImageH;
	ImageCp.x = ImageCp.x*0.5;
	ImageCp.y = ImageCp.y*0.5;	
	if ( NULL != WndPtr )
	{
		if (DRAW_MODEL_RESULT == DrawModelMode )
		{				
			WndPtr->GetWndRegionRes(BoxRgn);	
			fSkewAngle = WndPtr->GetWndAngleSkew();
			WndPtr->GetWndCornerPosRes(PadCornerPts);			
		}
		else
		{	
			fSkewAngle = 0.0;
			WndPtr->GetWndRegion(BoxRgn);	
			WndPtr->GetWndCornerPos(PadCornerPts);
		} 
	}
	else
	{
		CAOIBox *BoxPtr = ModelPtr->GetModelBodyBoxPtr();
		if (DRAW_MODEL_RESULT == DrawModelMode )
		{	
			BoxPtr->GetBoxRegionRes(BoxRgn);	
			fSkewAngle = BoxPtr->GetBoxAngleSkew();
			BoxPtr->GetBoxCornerPosRes(PadCornerPts);
		}
		else
		{	
			fSkewAngle = 0.0;
			BoxPtr->GetBoxRegion(BoxRgn); 
			BoxPtr->GetBoxCornerPos(PadCornerPts);
		}
	}	
	bShowRoi = true;
	bShowBox = true;
	JetAPI::SizeToRect(ImageW, ImageH, RoiRect);
	PadRect.left = 0;
	PadRect.top = 0;
	PadRect.right = ImageW;
	PadRect.bottom = ImageH;

	CAOIModel::CalcModelBoxRegionRect(BoxRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, PadRect);
	CAOIModel::CalcModelBoxCornerPts(PadCornerPts, ImageW, ImageH, RgnCp, Scale, ImageCp, PadCornerPts) ;
	const bool IsSkewAngle = JetAPI::CheckIsExceptionAngle(fSkewAngle);
	if ( true == IsSkewAngle )
	{	JetAPI::RotateCornerPos(fSkewAngle, PadCornerPts);	}

	fPadHeight = 0.0f;
	fRoiHeight = 0.0f;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	for ( i=0; i<BufferSize; i++ )
	{
		if ( fRoiHeight < pBuffer3D[i] ) 
		{	fRoiHeight = pBuffer3D[i]; }
	}	
	if ( ImageAPI.CheckRoiRect(ImageW, ImageH, PadRect) == true ) 
	{
		fHeight = 0;
		nHeightCnt=0;	
		for ( v=PadRect.top; v<PadRect.bottom; v++ )
		{
			for ( u=PadRect.left; u<PadRect.right; u++ )
			{
				k = (v*ImageStep+u);
				if ( ImageAPI.CheckSpaceMaskValid(pMask[k]) == false ) { continue; }
				if ( fPadHeight < pBuffer3D[k] ) 
				{	fPadHeight = pBuffer3D[k]; }
				nHeightCnt ++;
				fHeight += pBuffer3D[k];
			}
		}
		//if ( nHeightCnt > 0 ) 
		//{	fPadHeight = fHeight/nHeightCnt;	}
	}

	m_OpenGLWnd.SetShowPadHeight(false);
	m_OpenGLWnd.SetBoundPadOpen(bShowBox);
	m_OpenGLWnd.SetBoundRoiOpen(bShowRoi);	
	if ( true == IsExceptionAngle ) 
	{	m_OpenGLWnd.SetPadCornerPts(PadCornerPts, fRoiHeight+fExtraHeight);	}
	else
	{	m_OpenGLWnd.SetPadCornerPts(PadCornerPts, fPadHeight+(100*fExtraHeight));	}

	if ( CWnd::IsDlgButtonChecked(VIEW3D_VIEW_WND_BTN)==TRUE ) 
	{
		const int PadRectCpX=(PadRect.right+PadRect.left)/2;
		const int PadRectCpY=(PadRect.top+PadRect.bottom)/2;
		const int PadOffsetX = abs(PadRectCpX-(ImageW/2));
		const int PadOffsetY = abs(PadRectCpY-(ImageH/2));
		const int OffsetXRatio = 100*PadOffsetX/ImageW;
		const int OffsetYRatio = 100*PadOffsetY/ImageH;
		if ( OffsetXRatio>10 || OffsetYRatio>10 )
		{	m_OpenGLWnd.RotateToPadAngle();	}
	}
	
	TOpenGLBox   glBox;		
	double       WndSkewAngle=0.0f;
	bool         IsWndSkewAngle=false;
	

	const size_t ModelWndCount = ModelPtr->GetModelWndCount();
	m_OpenGLWnd.ClearGLBoxList();
	//CalcModelBoxCornerPts
	for ( i=0; i<ModelWndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr == ActWndPtr ) { continue; }
		if ( WndPtr->GetWndVisibled() == false ) { continue; }		
		if (DRAW_MODEL_RESULT == DrawModelMode )
		{	
			WndPtr->GetWndRegionRes(BoxRgn);				
			WndPtr->GetWndCornerPosRes(BoxCornerPts);
			WndSkewAngle = WndPtr->GetWndAngleSkew();
		}
		else
		{	
			WndSkewAngle = 0.0;
			WndPtr->GetWndRegion(BoxRgn); 
			WndPtr->GetWndCornerPos(BoxCornerPts);
		}
		if ( CAOIModel::CalcModelBoxRegionRect(BoxRgn, ImageW, ImageH, RgnCp, Scale, ImageCp, RoiRect) == false ) { continue; }			
		if ( CAOIModel::CalcModelBoxCornerPts(BoxCornerPts, ImageW, ImageH, RgnCp, Scale, ImageCp, BoxCornerPts) == false ) { continue; }			
		
		IsWndSkewAngle = JetAPI::CheckIsExceptionAngle(WndSkewAngle);
		if ( true == IsWndSkewAngle )
		{	JetAPI::RotateCornerPos(WndSkewAngle, BoxCornerPts);	}

		fHeight = 0;
		fRoiHeight=0;
		nHeightCnt=0;
		for ( v=RoiRect.top; v<RoiRect.bottom; v++ )
		{
			for ( u=RoiRect.left; u<RoiRect.right; u++ )
			{
				k = (v*ImageStep+u);				
				if ( ImageAPI.CheckSpaceMaskValid(pMask[k]) == false ) { continue; }
				if ( fRoiHeight < pBuffer3D[k] ) 
				{	fRoiHeight = pBuffer3D[k];	}
				nHeightCnt ++;
				fHeight += pBuffer3D[k];				
			}
		}
		if ( nHeightCnt > 0 ) 
		{	fHeight = fHeight/nHeightCnt; }
		fHeight = fRoiHeight;
		glBox.bShow = true;		
		//glBox.strText.Format(_T("%d"), i+1);
		m_OpenGLWnd.ConvertCornerPtToGLBox(BoxCornerPts, fHeight+fExtraHeight, glBox);
		m_OpenGLWnd.AddGLBoxObj(glBox);
	}
	//m_OpenGLWnd.SetPadRegion(RoiCornerPts, fRoiHeight+fExtraHeight);	
	m_OpenGLWnd.UpdateGLBoxList();
	m_OpenGLWnd.SetGLBoxListOpen(true);
	m_OpenGLWnd.UpdateWindow();//立即重繪
	return ;
}
//-------------------------------------------------------------------------------------//
bool CEditImageView3DPage::CheckUpdate3DBoxData()//確認更新顯示3D模式
{	
	CAOIModel   *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();
	if ( NULL == ModelPtr ) { return false; }		
	CAOIWnd     *WndPtr = ModelPtr->GetModelWndActived();		
	if ( NULL == WndPtr ) { return false; }
	CAOILand    *LandPtr = WndPtr->GetWndLandPtr();
	if ( NULL == LandPtr ) { return false; }

	bool       bLocal3D=false;
	MODEL_TYPE ModelType = ModelPtr->GetModelType();

	//bLocal3D = true;
	switch ( ModelType )
	{
	case MODEL_TYPE_CHIP:
	case MODEL_TYPE_CHIP_C:
	case MODEL_TYPE_CHIP_R:
	case MODEL_TYPE_CHIP_L:
	case MODEL_TYPE_CHIP_LED:
	case MODEL_TYPE_MELF:
		break;
	case MODEL_TYPE_ELECTRODE:
	case MODEL_TYPE_CAPACITY_ARRAY:
	case MODEL_TYPE_RESISTOR_ARRAY:
	case MODEL_TYPE_TRANSISTOR:
	case MODEL_TYPE_LEAD_TRANSISTOR:
		break;	
	case MODEL_TYPE_TANTALUM_CONDENSER:
	case MODEL_TYPE_ELECTROLYTIC_CAPACITOR:
		bLocal3D = true;
		break;
	default:
		bLocal3D = true;
		break;
	}
	if ( false == bLocal3D )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::UpdateCommandUI()
{
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }

	bool bOnOff=false;
	int  nValue=0;
	UINT CtrlID = 0;

	CtrlID = VIEW3D_PLANE_UPPER_CHK;
	bOnOff = m_OpenGLWnd.GetUpperPlaneOpen();
	CWnd::CheckDlgButton(CtrlID, bOnOff);

	CtrlID = VIEW3D_PLANE_BASE_CHK;
	bOnOff = m_OpenGLWnd.GetBasePlaneOpen();
	CWnd::CheckDlgButton(CtrlID, bOnOff);

	CtrlID = VIEW3D_PLANE_LOWER_CHK;
	bOnOff = m_OpenGLWnd.GetLowerPlaneOpen();
	CWnd::CheckDlgButton(CtrlID, bOnOff);

	CtrlID = VIEW3D_PLANE_CLIP_CHK;
	bOnOff = m_OpenGLWnd.GetClipPlaneOpen();
	CWnd::CheckDlgButton(CtrlID, bOnOff);

	nValue = m_OpenGLWnd.GetClipPlaneDir();
	CtrlID = VIEW3D_PLANE_CLIP_HOR_RDO;	
	if ( OPENGL_CLIP_PLANE_HOR == nValue ) { bOnOff = true; } 
	else { bOnOff = false; }
	CWnd::CheckDlgButton(CtrlID, bOnOff);

	CtrlID = VIEW3D_PLANE_CLIP_VER_RDO;	
	if ( OPENGL_CLIP_PLANE_VER == nValue ) { bOnOff = true; } 
	else { bOnOff = false; }
	CWnd::CheckDlgButton(CtrlID, bOnOff);

	CtrlID = VIEW3D_PLANE_CLIP_ANY_RDO;	
	if ( OPENGL_CLIP_PLANE_ANY == nValue ) { bOnOff = true; } 
	else { bOnOff = false; }
	CWnd::CheckDlgButton(CtrlID, bOnOff);

	//m_OpenGLWnd.SetFocus();	
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::UpdateCtrlUIEnabled()//更新控制介面是否啟用
{
	return;

	BOOL bEnable=FALSE;
	USER_LEVEL_MODE UserLevel = AOIDataCollect.GetCurrentUserLevel();

	switch ( UserLevel )
	{
	case USER_LEVEL_SIGN_OUT:
		bEnable = FALSE;
		break;
	case USER_LEVEL_OPERATOR:
		bEnable = FALSE;
		break;
	case USER_LEVEL_ENGINEER:
		bEnable = FALSE;
		break;
	case USER_LEVEL_SUPERVISOR:
		bEnable = FALSE;
		break;
	case USER_LEVEL_JET_FAE:		
	case USER_LEVEL_JET_SENIOR:
	case USER_LEVEL_JET_RD:
		bEnable = FALSE;
		break;
	}
	if ( FALSE == bEnable )
	{		
		OnObjectTextureRadio();		
		CWnd::CheckDlgButton(VIEW3D_OBJECT_COLOR_RADIO, FALSE);
		CWnd::CheckDlgButton(VIEW3D_OBJECT_LINE_RADIO, FALSE);
		CWnd::CheckDlgButton(VIEW3D_OBJECT_TEXTURE_RADIO, TRUE);
		CWnd::CheckDlgButton(VIEW3D_OBJECT_COLOR_LINE_RADIO, FALSE);
	}

	const bool bShow = false;
	if ( true == bShow )
	{
		JetAPI::ShowCtrlWnd(this, VIEW3D_OBJECT_COLOR_RADIO, bEnable);
		JetAPI::ShowCtrlWnd(this, VIEW3D_OBJECT_LINE_RADIO, bEnable);
		//JetAPI::ShowCtrlWnd(this, VIEW3D_OBJECT_TEXTURE_RADIO, bEnable);
		JetAPI::ShowCtrlWnd(this, VIEW3D_OBJECT_COLOR_LINE_RADIO, bEnable);
	}
	else
	{
		JetAPI::EnableCtrlWnd(this, VIEW3D_OBJECT_COLOR_RADIO, bEnable);
		JetAPI::EnableCtrlWnd(this, VIEW3D_OBJECT_LINE_RADIO, bEnable);
		//JetAPI::EnableCtrlWnd(this, VIEW3D_OBJECT_TEXTURE_RADIO, bEnable);
		JetAPI::EnableCtrlWnd(this, VIEW3D_OBJECT_COLOR_LINE_RADIO, bEnable);	
	}
}
//-------------------------------------------------------------------------------------//
bool CEditImageView3DPage::ExecPopupMenu(POINT point, UINT menuID, UINT ID)
{
	if ( menuID == 0 ) { return false; }
	//if ( GetLockUIWnd() == true ) { return true; }

	CMenu menu;
	CPoint CtrlPt = point;
	VERIFY(menu.LoadMenu(menuID));
	AOIDataCollect.SwitchMultiLanguageMenu(menu, menuID);	
	CMenu* pPopup = menu.GetSubMenu(ID);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{	pWndPopupOwner = pWndPopupOwner->GetParent();	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	POINT dp=point;
	POINT pt=point;
	CWnd::ScreenToClient(&pt);
	dp.x = m_MousePosFirst.x-pt.x;
	dp.y = m_MousePosFirst.y-pt.y;
	if ( ::abs(dp.x)>2 || ::abs(dp.y)>2 )
	{	return; }

	//ExecPopupMenu(point, IDR_DRAW3D_MENU);
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	POINT pt2={0};		
	//if ( PtInControlWnd(point, VIEW3D_OPENGL_WND, pt2) == false )
	if ( JetAPI::CheckPtInCtrlWnd(this, point, VIEW3D_OPENGL_WND, &pt2) == false )
	{
		CDialog::OnRButtonDown(nFlags, point);
		return;
	}	
	m_MousePosFirst = m_MousePosLast = point;
	//CWnd::MapWindowPoints(&m_ImageWnd, &m_MousePosImageWnd, 1);
	CDialog::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	m_MousePosLast = point;	
	CDialog::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnObjectColorMenu() 
{
	// TODO: Add your command handler code here
	this->m_OpenGLWnd.SetMainObjectMode(OPENGL_OBJECT_COLOR);
	this->m_OpenGLWnd.SetShowColorRuler(true);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnObjectLineMenu() 
{
	// TODO: Add your command handler code here
	this->m_OpenGLWnd.SetMainObjectMode(OPENGL_OBJECT_LINE);
	this->m_OpenGLWnd.SetShowColorRuler(true);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnObjectTextureMenu() 
{
	// TODO: Add your command handler code here
	this->m_OpenGLWnd.SetMainObjectMode(OPENGL_OBJECT_TEXTURE);
	this->m_OpenGLWnd.SetShowColorRuler(false);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnObjectColorLineMenu() 
{
	// TODO: Add your command handler code here
	this->m_OpenGLWnd.SetMainObjectMode(OPENGL_OBJECT_COLOR_LINE);	
	this->m_OpenGLWnd.SetShowColorRuler(true);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{
		UpdateCtrlUIEnabled();		
		AOIDataCollect.SetEditImagePageWndID(WPARAM_SHOW_IMAGE_OPENGL_3D_PAGE);
		CWnd::PostMessage(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_MODEL_SELECTED, NULL);
	}
	else
	{
		ClearView3DPoints();
		ReleaseUniFrameList();	
	}
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnToolbarModelBtn() 
{
	// TODO: Add your control notification handler code here
	POINT point={0,0};
	::GetCursorPos(&point);
	ExecPopupMenu(point, IDR_DRAW3D_MENU, 1);
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnToolbarShowBtn() 
{
	// TODO: Add your control notification handler code here
	POINT point={0,0};
	::GetCursorPos(&point);
	ExecPopupMenu(point, IDR_DRAW3D_MENU, 2);
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnUpperPlaneMenu() 
{
	// TODO: Add your command handler code here
	if ( this->m_OpenGLWnd.GetUpperPlaneOpen() == true )
	{	this->m_OpenGLWnd.SetUpperPlaneOpen(false); }
	else
	{	this->m_OpenGLWnd.SetUpperPlaneOpen(true); }
	this->UpdateCommandUI();
	m_OpenGLWnd.SetFocus();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnLowerPlaneMenu() 
{
	// TODO: Add your command handler code here
	if ( this->m_OpenGLWnd.GetLowerPlaneOpen() == true )
	{	this->m_OpenGLWnd.SetLowerPlaneOpen(false); }
	else
	{	this->m_OpenGLWnd.SetLowerPlaneOpen(true); }
	this->UpdateCommandUI();
	m_OpenGLWnd.SetFocus();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnDefaultMenu() 
{
	// TODO: Add your command handler code here
	this->m_OpenGLWnd.SetModalCenter(true);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnBasePlaneMenu() 
{
	// TODO: Add your command handler code here
	if ( this->m_OpenGLWnd.GetBasePlaneOpen() == true )
	{	this->m_OpenGLWnd.SetBasePlaneOpen(false); }
	else
	{	this->m_OpenGLWnd.SetBasePlaneOpen(true); }
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnPlanePitchMenu() 
{
	// TODO: Add your command handler code here
	CString strValue;
	CString strLabel;
	CString strCaption;
	CInputBoxWnd InputBox;

	strCaption = _T("Move Plane Pitch");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Pitch (um):");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strValue.Format(_T("%.2f"), m_OpenGLWnd.GetBoundPitch());
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return;  }

	float Pitch = (float)(::_tcstod(InputBox.m_DataEdit1, NULL));
	m_OpenGLWnd.SetBountPitch(Pitch);
	m_OpenGLWnd.SetFocus();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnPlaneHeightMenu()
{
	CString strValue;
	CString strLabel;
	CString strCaption;
	CInputBoxWnd InputBox;	

	strCaption = _T("Plane Height");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Height (um):");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strValue.Format(_T("%.2f"), m_OpenGLWnd.GetBoundHeight());
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return;  }

	float Height = (float)(::_tcstod(InputBox.m_DataEdit1, NULL));
	m_OpenGLWnd.SetBountHeight(Height);	
	m_OpenGLWnd.SetFocus();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnObjectColorRadio() 
{
	// TODO: Add your control notification handler code here
	float RuleMinH=m_TargetRuleMin;
	float RuleMaxH=m_TargetRuleMax;
	//float RuleMinH=m_OpenGLWnd.GetRuleMinH();//GetDataMinH
	//float RuleMaxH=m_OpenGLWnd.GetRuleMaxH();//GetDataMaxH	
	GetRuleColorRange(RuleMinH, RuleMaxH);	
	m_OpenGLWnd.SetRuleMinH(RuleMinH);
	m_OpenGLWnd.SetRuleMaxH(RuleMaxH);	

	this->m_OpenGLWnd.SetMainObjectMode(OPENGL_OBJECT_COLOR);
	this->m_OpenGLWnd.SetShowColorRuler(true);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnObjectLineRadio() 
{
	// TODO: Add your control notification handler code here
	this->m_OpenGLWnd.SetMainObjectMode(OPENGL_OBJECT_LINE);
	this->m_OpenGLWnd.SetShowColorRuler(true);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnObjectTextureRadio() 
{
	// TODO: Add your control notification handler code here
	this->m_OpenGLWnd.SetMainObjectMode(OPENGL_OBJECT_TEXTURE);
	this->m_OpenGLWnd.SetShowColorRuler(false);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnObjectColorLineRadio() 
{
	// TODO: Add your control notification handler code here
	this->m_OpenGLWnd.SetMainObjectMode(OPENGL_OBJECT_COLOR_LINE);
	this->m_OpenGLWnd.SetShowColorRuler(true);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnPlaneUpperChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(VIEW3D_PLANE_UPPER_CHK);
	if ( FALSE == bCheck )
	{	this->m_OpenGLWnd.SetUpperPlaneOpen(false); }
	else
	{	this->m_OpenGLWnd.SetUpperPlaneOpen(true); }
	this->UpdateCommandUI();
	m_OpenGLWnd.SetFocus();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnPlaneBaseChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(VIEW3D_PLANE_BASE_CHK);
	if ( FALSE == bCheck )
	{	this->m_OpenGLWnd.SetBasePlaneOpen(false); }
	else
	{	this->m_OpenGLWnd.SetBasePlaneOpen(true); }
	this->UpdateCommandUI();
	m_OpenGLWnd.SetFocus();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnPlaneLowerChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(VIEW3D_PLANE_LOWER_CHK);
	if ( FALSE == bCheck )
	{	this->m_OpenGLWnd.SetLowerPlaneOpen(false); }
	else
	{	this->m_OpenGLWnd.SetLowerPlaneOpen(true); }
	this->UpdateCommandUI();
	m_OpenGLWnd.SetFocus();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnPlanePitchBtn() 
{
	// TODO: Add your control notification handler code here
	CString strValue;
	CString strLabel;
	CString strCaption;
	CInputBoxWnd InputBox;

	strCaption = _T("Move Plane Pitch");	
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Pitch (um):");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strValue.Format(_T("%.f"), m_OpenGLWnd.GetBoundPitch());
	InputBox.SetParam1(strCaption, strLabel, strValue);
	if ( InputBox.DoModal() == IDCANCEL ) 
	{	return;  }

	float Pitch = (float)(::_tcstod(InputBox.m_DataEdit1, NULL));
	m_OpenGLWnd.SetBountPitch(Pitch);
	m_OpenGLWnd.SetFocus();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnViewDefaultBtn() 
{
	// TODO: Add your control notification handler code here	
	this->m_OpenGLWnd.SetModalCenter(true);
	this->UpdateCommandUI();
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnAnimationPlayBtn() 
{
	// TODO: Add your control notification handler code here
	m_OpenGLWnd.SetAnimationPlay(true);
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnAnimationStopBtn() 
{
	// TODO: Add your control notification handler code here
	m_OpenGLWnd.SetAnimationPlay(false);
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnPlaneClipChk() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(VIEW3D_PLANE_CLIP_CHK);
	if ( FALSE == bCheck )
	{	m_OpenGLWnd.SetClipPlaneOpen(false);	}
	else
	{	m_OpenGLWnd.SetClipPlaneOpen(true);	}
	this->UpdateCommandUI();
	m_OpenGLWnd.SetFocus();
	ShowProfileWnd(m_ChartWnd);
	BuildProfileWnd(m_ChartWnd);
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnPlaneClipHorRdo() 
{
	// TODO: Add your control notification handler code here		
	m_OpenGLWnd.SetClipPlaneDir(OPENGL_CLIP_PLANE_HOR);		
	m_OpenGLWnd.SetFocus();	
	BuildProfileWnd(m_ChartWnd);
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnPlaneClipVerRdo() 
{
	// TODO: Add your control notification handler code here	
	m_OpenGLWnd.SetClipPlaneDir(OPENGL_CLIP_PLANE_VER);	
	m_OpenGLWnd.SetFocus();	
	BuildProfileWnd(m_ChartWnd);
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnPlaneClipAnyRdo()
{
	m_OpenGLWnd.SetClipPlaneDir(OPENGL_CLIP_PLANE_ANY);	
	m_OpenGLWnd.SetFocus();	
	BuildProfileWnd(m_ChartWnd);
}
//-------------------------------------------------------------------------------------//
bool CEditImageView3DPage::InitProfileWnd(CThisChartCtrl &ChartWnd)//初始化圖表視窗-剖線圖
{
	Font_ST font;
	COLORREF BKColor = ::GetSysColor(COLOR_BTNFACE);

	font.sFontColor = 0x0000FF;
	font.sFontSize = 16;
	font.sIsBold = true;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetTitleFont(font);	
	
	font.sFontColor = 0x000000;
	font.sFontSize = 14;//12
	font.sIsBold = false;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetFrameFont(font);	
	
	font.sFontColor = 0x0000ff;
	font.sFontSize = 16;
	font.sIsBold = true;
	font.sIsItalic = false;
	font.sIsUnderline = false;
	font.sEscapement = 0;
	_stprintf(font.sFontType, _T("Times New Roman"));
	ChartWnd.SetInfoFont(font);
	
/*
	CString Title = "";
	Title.Format("%d.   %s", HistoryIndex+1, DateTime);
	ChartWnd.SetTitle(Title);
*/
	ChartWnd.SetChartType(CHART_TYPE_LINE);
	ChartWnd.SetXAxisUnit("pxl");
	ChartWnd.SetYAxisUnit("um");
	ChartWnd.SetXAxisLabelMode(JET_CHART_LABEL_MODE_LABEL);
	ChartWnd.SetFrameColor(BKColor);
	ChartWnd.SetBKColor(BKColor);
	ChartWnd.SetTitleColor(BKColor);
	ChartWnd.SetIsShowLegend(false);
//	ChartWnd.SetLegendLocationMode(JET_CHART_LENGEND_LOCATION_MODE_BOTTOM);	
	ChartWnd.SetLegendLocationMode(JET_CHART_LENGEND_LOCATION_MODE_RIGHT);	

	ChartWnd.Set3DThicness(10);
	ChartWnd.SetChartTopSpace(30);
	ChartWnd.SetChartBottomSpace(40);
	ChartWnd.SetChartLeftSpace(40);
	ChartWnd.SetChartRightSpace(20);

	ChartWnd.SetSeriesLeftSpace(5);
	ChartWnd.SetSeriesRightSpace(5);
	ChartWnd.SetSeriesTopSpace(5);
	ChartWnd.SetSeriesBottomSpace(5);
	ChartWnd.SetIsTranspose(false);

	ChartWnd.SetIsIntYValue(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageView3DPage::ShowProfileWnd(CThisChartCtrl &ChartWnd)//顯示圖表視窗-剖線圖
{
	if ( ChartWnd.GetSafeHwnd() == NULL ) { return true; }
	bool bOpen = m_OpenGLWnd.GetClipPlaneOpen();
	if ( true == bOpen ) 
	{	ChartWnd.ShowWindow(SW_SHOW);	}
	else
	{	ChartWnd.ShowWindow(SW_HIDE);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageView3DPage::BuildProfileWnd(CThisChartCtrl &ChartWnd)//建立圖表視窗-剖線圖
{
	ChartWnd.Clear();
	if ( ChartWnd.IsWindowVisible() == FALSE ) { return true; }
	ChartWnd.Set3DThicness(20);		

	CString str;
	CString XLabel = _T("");
	CJetSeries *pSeries1 = new CJetSeries();
	CJetSeries *pSeries2 = new CJetSeries();
	if ( NULL==pSeries1 || NULL==pSeries2 )
	{
		delete pSeries1; pSeries1=NULL;
		delete pSeries2; pSeries2=NULL;
		return false; 
	}
	CAOIModel *ModelPtr = AOIDataCollect.GetModelUniFrameListModelPtr();
	CAOIWnd *WndPtr = NULL;
	TALG_PARAM_RESIN_HEIGHT Param;
	std::vector<int> Pos;
	if (NULL != ModelPtr)
	{
		for (int i = 0; i < ModelPtr->GetModelWndCount(); i++)
		{
			WndPtr = ModelPtr->GetModelWndPtr(i, false);
			if (WndPtr->GetWndDefectID() == WND_DEFECT_PART_ALIGN && WndPtr->GetWndAlgParam().GetAlgParamResinHeight().enabled)
			{
				Param = WndPtr->GetWndAlgParam().GetAlgParamResinHeight();
				break;
			}
		}
	}

	pSeries1->SetSeriesType(SERIES_TYPE_LINE);
	pSeries1->SetIsVisible(true);
	pSeries1->SetDotWidth(0);
	pSeries1->SetLineColor(0x0000FF);	
	//pSeries1->SetDotColor(0x0000FF);
	pSeries1->SetIsShowMarkValue(false);

	pSeries2->SetSeriesType(SERIES_TYPE_BAR);
	pSeries2->SetIsVisible(true);
	pSeries2->SetDotWidth(0);
	pSeries2->SetLineColor(0xD0FFFF);	
	//pSeries2->SetDotColor(0x0000FF);
	pSeries2->SetIsShowMarkValue(true);
	
	
	int   i=0;	
	int   SeriesIndex = 0;
	int   nPosX=0, nPosY=0;
	int   nSizeW=0;
	double PosX=0, PosY=0;	
	const int SmoothSize=5;
	CString strValue;
	std::vector<float>  DataList;
	std::vector<TPiece> PieceList;	
	const float MinH = m_OpenGLWnd.GetShowMinH();
	const float MaxH = m_OpenGLWnd.GetShowMaxH();
	m_OpenGLWnd.BuildProfileValue(DataList);
	SmoothProfile(SmoothSize, DataList);
	const int DataCount = (int)(DataList.size());
	SeriesIndex = 0;
	for ( i=0; i<DataCount; i++ )
	{	pSeries1->AddXYValue(true, i, i, DataList[i]);	}

	int PieceCount = 0;
	double dRatio = 1.25;
	const int  nMinW = 5;
	dRatio = 1.25;
	BuildPieceList(DataList, dRatio, nMinW, PieceList);
	PieceCount = (int)(PieceList.size());
	if ( 0 == PieceCount )
	{
		dRatio = 1.00;
		BuildPieceList(DataList, dRatio, nMinW, PieceList);
		PieceCount = (int)(PieceList.size());
	}
	for ( i=0; i<PieceCount; i++ )
	{	
		nPosX = (PieceList[i].nX1+PieceList[i].nX2)/2;
		nSizeW = abs(PieceList[i].nX2-PieceList[i].nX1);
		strValue.Format(_T("%.0f"), PieceList[i].fValue);		
		pSeries2->AddXYValue(true, nPosX, nPosX, PieceList[i].fValue, NULL, strValue, 0x000000, nSizeW);	
	}	

	ChartWnd.SetYAxisMin(MinH);
	ChartWnd.SetYAxisMax(MaxH);
	ChartWnd.SetYAxisPercentageVisible(true);
	ChartWnd.SetYAxisPercentageTarget(m_TargetHegiht);
	//ChartWnd.SetYAxisFix(true);
	
	if (m_OpenGLWnd.GetClipPlaneOpen() && Param.enabled && NULL != ModelPtr)	// 按鈕
	{
		size_t srcIdx = 0;
		POINT point;
		ChartWnd.SetXAxisGribLineVisible(true);
		
		std::vector<float>planePos = m_OpenGLWnd.GetClipPlanePos();

		for (int k = 0; k< Param.partPoint.size(); k++)
		{
			for (int j = 0; j< Param.partPoint[k].size(); j++)
			{
				if (Param.partPoint[k][j].y > 0 || Param.partPoint[k][j].x > 0)
				{
					point.x = Param.partPoint[k][j].x;
					point.y = Param.partPoint[k][j].y;

					if ((WndPtr->GetWndBox().GetBoxToward() == BOX_TOWARD_UP || WndPtr->GetWndBox().GetBoxToward() == BOX_TOWARD_DOWN) && m_OpenGLWnd.GetClipPlaneDir() == 2)
					{
						if (point.x == (int)planePos[0])
						{
							//pSeries2->AddXYValue(true, m_ImageH - point.y, m_ImageH - point.y, DataList[m_ImageH - point.y], NULL, _T("") , 0x000000, nSizeW);
							Pos.push_back(m_ImageH - point.y);
						}
					}
					if ((WndPtr->GetWndBox().GetBoxToward() == BOX_TOWARD_LEFT || WndPtr->GetWndBox().GetBoxToward() == BOX_TOWARD_RIGHT) && m_OpenGLWnd.GetClipPlaneDir() == 1)
					{
						if (point.y == (int)planePos[1])
						{
							//pSeries2->AddXYValue(true, point.x, point.x, DataList[point.x], NULL, _T(""), 0x000000, nSizeW);
							Pos.push_back(point.x);
						}
					}
				}
			}
		}
		for (int k = 0; k< Param.tinPoint.size(); k++)
		{
			for (int j = 0; j< Param.tinPoint[k].size(); j++)
			{
				if (Param.tinPoint[k][j].y > 0 || Param.tinPoint[k][j].x > 0)
				{
					point.x = Param.tinPoint[k][j].x;
					point.y = Param.tinPoint[k][j].y;

					if ((WndPtr->GetWndBox().GetBoxToward() == BOX_TOWARD_UP || WndPtr->GetWndBox().GetBoxToward() == BOX_TOWARD_DOWN) && m_OpenGLWnd.GetClipPlaneDir() == 2)
					{
						if (point.x == (int)planePos[0])
						{
							//pSeries2->AddXYValue(true, m_ImageH - point.y, m_ImageH - point.y, DataList[m_ImageH - point.y], NULL, _T(""), 0x000000, nSizeW);
							Pos.push_back(m_ImageH - point.y);
						}
					}
					if ((WndPtr->GetWndBox().GetBoxToward() == BOX_TOWARD_LEFT || WndPtr->GetWndBox().GetBoxToward() == BOX_TOWARD_RIGHT) && m_OpenGLWnd.GetClipPlaneDir() == 1)
					{
						if (point.y == (int)planePos[1])
						{
							//pSeries2->AddXYValue(true, point.x, point.x, DataList[point.x], NULL, _T(""), 0x000000, nSizeW);
							Pos.push_back(point.x);
						}
					}
				}
			}
		}
	}
	ChartWnd.SetXAxisGribLineTarget(Pos);

	ChartWnd.AddSeries(pSeries2);
	ChartWnd.AddSeries(pSeries1);	
	delete pSeries1; pSeries1=NULL;	
	delete pSeries2; pSeries2=NULL;	

	ChartWnd.BuildChart();
	ChartWnd.RedrawWindow();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageView3DPage::SmoothProfile(int KenSize, std::vector<float> &DataList)//平滑線段
{
	size_t       i=0, j=0;	
	int          Count=0;
	float        DropVal=0.0f;
	double       Sum=0;
	double       Ave=0;
	const size_t DataCount = DataList.size();	
	if ( KenSize > DataCount ) { return false; }
	const int KenSize2 = KenSize/2;
	std::vector<float> TmpDataList=DataList;
	for ( i=KenSize2; i<DataCount-KenSize2; i++ )
	{
		Sum = 0.0f;
		Count = 0;
		for ( j=i-KenSize2; j<=i+KenSize2; j++ )
		{
			Sum += TmpDataList[j];
			Count ++;
		}
		if ( Count == 0 ) { continue; }
		Ave = Sum/Count;		
		DataList[i] = Ave;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditImageView3DPage::BuildPieceList(const std::vector<float> &Profile, double dRatio, int nMinW, std::vector<TPiece> &PieceList)//取得子線段
{
	size_t       i=0, j=0;	
	int          Count=0;
	float        DropVal=0.0f;
	double       Sum=0;
	double       Ave=0;
	const size_t DataCount = Profile.size();		

	PieceList.clear();
	if ( 0 == DataCount ) { return true; }	
	for ( i=0; i<DataCount; i++ )
	{
		Sum += Profile[i];
		Count ++;		
	}
	if ( Count > 0 ) 
	{	Ave = Sum/Count; }
	const float Threshold = Ave*dRatio;

	Sum=0;
	Count=0;
	TPiece Piece;
	float fValue=0;	
	for ( i=0; i<DataCount; i++ )
	{
		fValue = Profile[i];
		if ( fValue < Threshold ) 
		{
			if ( Piece.nX1 >= 0 )
			{
				if ( Count > nMinW ) 
				{
					Ave = Sum/Count;
					Piece.nX2 = i;
					Piece.fValue = Ave;					
					PieceList.push_back(Piece);				
				}
				Piece = TPiece();
			}
			Sum = 0;
			Count = 0;			
			continue; 
		}		
		Sum += fValue;
		Count ++;
		if ( Piece.nX1 < 0 ) 
		{	Piece.nX1 = i; }
	}
	if ( Piece.nX1 >= 0  )
	{		
		if ( Count > nMinW ) 
		{
			Ave = Sum/Count;
			Piece.nX2 = i;
			Piece.fValue = Ave;
			PieceList.push_back(Piece);
		}
		Piece = TPiece();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnSelchangeDetailLevelCombo() 
{
	// TODO: Add your control notification handler code here	
	const int nLv = (int)(JetAPI::GetComboxCurSelData(m_DetailLeveCombox));
	m_OpenGLWnd.SetDetailLevel(nLv);
	BuildView3DPoints(true, false);
}
//-------------------------------------------------------------------------------------//
void CEditImageView3DPage::OnViewWndBtn() 
{
	// TODO: Add your control notification handler code here
	BOOL bCheck = CWnd::IsDlgButtonChecked(VIEW3D_VIEW_WND_BTN);
	if ( TRUE == bCheck )
	{
		if ( NULL == m_WndPtr )
		{	m_OpenGLWnd.RotateToPadAngle();	 }
		else
		{	m_OpenGLWnd.RotateToPadAngleByAroundHeight();	 }
	}
}
//-------------------------------------------------------------------------------------//