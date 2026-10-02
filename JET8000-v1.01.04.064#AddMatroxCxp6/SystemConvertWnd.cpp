// SystemConvertWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "SystemConvertWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CSystemConvertWnd dialog
//-------------------------------------------------------------------------------------//
CSystemConvertWnd::CSystemConvertWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CSystemConvertWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemConvertWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CSystemConvertWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemConvertWnd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CSystemConvertWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CSystemConvertWnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(SYSCVT_SRC_FOLDER_BTN, OnSrcFolderBtn)
	ON_BN_CLICKED(SYSCVT_DST_FOLDER_BTN, OnDstFolderBtn)	
	ON_BN_CLICKED(SYSCVT_CONVERT_XYCALI_BTN, OnConvertXYCaliBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemConvertWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CSystemConvertWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	CString SrcFolder = AOIDataCollect.GetAOIDirectory();
	CWnd::SetDlgItemText(SYSCVT_SRC_FOLDER_EDIT, SrcFolder);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CSystemConvertWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CSystemConvertWnd::OnOK() 
{
	// TODO: Add extra validation here
	return;
	CBaseDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CSystemConvertWnd::OnCancel() 
{
	// TODO: Add extra cleanup here
	//return;
	CBaseDialog::OnCancel();
}
//-------------------------------------------------------------------------------------//
void CSystemConvertWnd::SwitchMultiLanguage()
{
	int     i = 0;
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_SYSTEM_CONVERT_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_SYSTEM_CONVERT_WND;
	WndKey = _T("IDD_SYSTEM_CONVERT_WND");
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
	WndID = SYSCVT_SRC_FOLDER_LABEL;
	WndKey = _T("SYSCVT_SRC_FOLDER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SYSCVT_DST_FOLDER_LABEL;
	WndKey = _T("SYSCVT_DST_FOLDER_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = SYSCVT_CONVERT_XYCALI_BTN;
	WndKey = _T("SYSCVT_CONVERT_XYCALI_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CSystemConvertWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_SYSTEM_CONVERT_WND");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CSystemConvertWnd::OnSrcFolderBtn() 
{
	// TODO: Add your control notification handler code here	
	ExecFolderBnt(SYSCVT_SRC_FOLDER_EDIT);
}
//-------------------------------------------------------------------------------------//
void CSystemConvertWnd::OnDstFolderBtn() 
{
	// TODO: Add your control notification handler code here
	ExecFolderBnt(SYSCVT_DST_FOLDER_EDIT);
}
//-------------------------------------------------------------------------------------//
bool CSystemConvertWnd::ExecFolderBnt(UINT CtrlID)
{
	CString Folder;
	CWnd::GetDlgItemText(CtrlID, Folder);
	if ( JetAPI::OpenFolderDialog(this, Folder) == false )
	{	return false; }
	CWnd::SetDlgItemText(CtrlID, Folder);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSystemConvertWnd::ExecConvertXYCaliBtn()
{
	CString str;
	CString SrcFolder;
	CString DstFolder;
	CString ShortName = JET8000_INI_FILE;
	CString SysFolder = AOIDataCollect.GetAOIDirectory();
	CWnd::GetDlgItemText(SYSCVT_SRC_FOLDER_EDIT, SrcFolder);
	CWnd::GetDlgItemText(SYSCVT_DST_FOLDER_EDIT, DstFolder);
	if ( DstFolder.CompareNoCase(SrcFolder) == 0 )
	{	
		str = _T("Error, the Folder is the same");
		JetAPI::ShowMessageBox(str);
		return false;	
	}
	if ( DstFolder.CompareNoCase(SysFolder) == 0 )
	{	
		str = _T("Error, the Folder can not be system folder");
		JetAPI::ShowMessageBox(str);
		return false;	
	}

	CString SysFilename;
	CString SrcFilename;
	CString DstFilename;
	TMotionParameter SrcMotionParam;
	TMotionParameter DstMotionParam;
	SysFilename.Format(_T("%s\\%s"), SysFolder, ShortName);
	SrcFilename.Format(_T("%s\\%s"), SrcFolder, ShortName);
	DstFilename.Format(_T("%s\\%s"), DstFolder, ShortName);
	if ( JetAPI::IsFileExist(DstFilename) == false )
	{
		str.Format(_T("Error, No System File(%s)"), DstFilename);
		JetAPI::ShowMessageBox(str);
		return false;
	}
	if ( JetAPI::IsFileExist(SrcFilename) == false )
	{
		str.Format(_T("Error, No System File(%s)"), SrcFilename);
		JetAPI::ShowMessageBox(str);
		return false;
	}
	if ( MotionCtrlPtr->LoadMotionParameter(DstFilename, DstMotionParam) == false )
	{
		str = MotionCtrlPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}
	if ( MotionCtrlPtr->LoadMotionParameter(SrcFilename, SrcMotionParam) == false )
	{
		str = MotionCtrlPtr->GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false;
	}

	//找出原點的偏差值
	LANE_ID LaneID;
	double OrgOffsetX=SrcMotionParam.m_HomeOrgOffsetX-DstMotionParam.m_HomeOrgOffsetX;
	double OrgOffsetY=SrcMotionParam.m_HomeOrgOffsetY-DstMotionParam.m_HomeOrgOffsetY;
	double OrgOffsetZ=SrcMotionParam.m_HomeOrgOffsetZ-DstMotionParam.m_HomeOrgOffsetZ;

	int CountX = 0;
	int CountY = 0;
	std::vector<TDotNode> DotList;		
	//For Lane A	
	DotList.clear();
	LaneID = LANE_ID_A;
	SrcFilename = AOIDataCollect.GetDotNodeCaliFilename(SrcFolder, LaneID);	
	if ( AOIDataCollect.LoadXYDotNodeCaliFile(SrcFilename, CountX, CountY, DotList) == true )
	{
		AOIDataCollect.MoveXYDotNodeList(OrgOffsetX, OrgOffsetY, DotList);
		DstFilename = AOIDataCollect.GetDotNodeCaliFilename(DstFolder, LaneID);
		AOIDataCollect.SaveXYDotNodeCaliFile(DstFilename, CountX, CountY, DotList);
	}
	//For Lane B
	DotList.clear();
	LaneID = LANE_ID_B;
	SrcFilename = AOIDataCollect.GetDotNodeCaliFilename(SrcFolder, LaneID);	
	if ( AOIDataCollect.LoadXYDotNodeCaliFile(SrcFilename, CountX, CountY, DotList) == true )
	{
		AOIDataCollect.MoveXYDotNodeList(OrgOffsetX, OrgOffsetY, DotList);
		DstFilename = AOIDataCollect.GetDotNodeCaliFilename(DstFolder, LaneID);
		AOIDataCollect.SaveXYDotNodeCaliFile(DstFilename, CountX, CountY, DotList);
	}

	//For Motion XYCali	
	std::vector<TXYCali> XYCaliList;
	CString XYCaliName = MOTION_XY_CALI_FILE;
	SrcFilename.Format(_T("%s\\%s"), SrcFolder, XYCaliName);
	MotionCtrlPtr->LoadMotionXYCali(SrcFilename, XYCaliList);	
	MotionCtrlPtr->MoveMotionXYCali(OrgOffsetX, OrgOffsetY, XYCaliList);//移動運動系統的XY校正表
	DstFilename.Format(_T("%s\\%s"), DstFolder, XYCaliName);
	MotionCtrlPtr->SaveMotionXYCali(DstFilename, XYCaliList);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CSystemConvertWnd::OnConvertXYCaliBtn() 
{
	// TODO: Add your control notification handler code here
	ExecConvertXYCaliBtn();
}
//-------------------------------------------------------------------------------------//