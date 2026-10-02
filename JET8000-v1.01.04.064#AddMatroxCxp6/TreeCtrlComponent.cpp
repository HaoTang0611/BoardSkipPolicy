// TreeCtrlComponent.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "TreeCtrlComponent.h"
//-------------------------------------------------------------------------------------//
#include "TreeCtrlDef.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlComponent
//-------------------------------------------------------------------------------------//
CTreeCtrlComponent::CTreeCtrlComponent()
{
	m_ProjectPtr = NULL;
	m_ClickComponentTreeNode = FALSE;
	m_StopComponentTreeBeClick = FALSE;
}
//-------------------------------------------------------------------------------------//
CTreeCtrlComponent::~CTreeCtrlComponent()
{
	CTreeCtrlComponent::ClearTreeWnd();
	m_ProjectPtr = NULL;
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CTreeCtrlComponent, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeCtrlComponent)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)		
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlComponent message handlers
//-------------------------------------------------------------------------------------//
void CTreeCtrlComponent::SetProjectPtr(CAOIProject *ProjectPtr)
{
	CTreeCtrlComponent::ClearTreeWnd();
	m_ProjectPtr = ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CTreeCtrlComponent::CloseProject()
{
	m_ProjectPtr = NULL;
}
//-------------------------------------------------------------------------------------//
CAOIProject* CTreeCtrlComponent::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::ClearTreeWnd()
{
	m_StopComponentTreeBeClick = FALSE;	
	if ( this->GetSafeHwnd() == NULL ) { return TRUE; }
	m_StopComponentTreeBeClick = TRUE; 	
	if ( CTreeCtrl::DeleteAllItems() == FALSE )
	{ 
		m_StopComponentTreeBeClick = FALSE;	
		return FALSE; 
	}
	m_StopComponentTreeBeClick = FALSE;	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::RemoveTreeItem(HTREEITEM hItem)
{
	if ( hItem == NULL ) { return FALSE; }
#ifdef _DEBUG
	CString str;
#endif
	HTREEITEM SubItem = CTreeCtrl::GetChildItem(hItem);
	HTREEITEM NextSubItem = SubItem;
	while ( SubItem != NULL )
	{
	#ifdef _DEBUG
		str = CTreeCtrl::GetItemText(SubItem);
	#endif
		NextSubItem = CTreeCtrl::GetNextSiblingItem(SubItem);
		CTreeCtrl::DeleteItem(SubItem);
		SubItem = NextSubItem;
	};
	return TRUE;	
}
//-------------------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::BuildTreeImageList()
{
	m_TreeImageList.DeleteImageList();
	SIZE ImageSize;
	ImageSize.cx = 16;
	ImageSize.cy = 16;
	if ( m_TreeImageList.Create(ImageSize.cx, ImageSize.cy, ILC_COLOR24|ILC_MASK, 0, 10) == false ) 
	{
		TRACE0("Failed to create Object Toolbar Image List\n");
		return false;      // fail to create
	}

	CBitmap bm;
	bm.LoadBitmap(IDB_PROJECT_TREE_ICON);					m_TreeImageList.Add(&bm, RGB(255,255,255));	bm.DeleteObject();//00
	/*
	bm.LoadBitmap(IDB_TREE_PROGRAM);					m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//00
	bm.LoadBitmap(IDB_TREE_FD);							m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//01
	
	bm.LoadBitmap(IDB_TREE_PANEL);						m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//02
	bm.LoadBitmap(IDB_TREE_PANEL_BYPASS);				m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//03

	bm.LoadBitmap(IDB_TREE_BOARD);						m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//04
	bm.LoadBitmap(IDB_TREE_BOARD_SELECTED);				m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//05
	bm.LoadBitmap(IDB_TREE_BOARD_BYPASS);				m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//06
	bm.LoadBitmap(IDB_TREE_BOARD_BYPASS_SELECTED);		m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//07
		
	bm.LoadBitmap(IDB_TREE_COMPONENT);					m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//08
	bm.LoadBitmap(IDB_TREE_COMPONENT_SELECTED);			m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//09
	bm.LoadBitmap(IDB_TREE_COMPONENT_BYPASS);			m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//10
	bm.LoadBitmap(IDB_TREE_COMPONENT_BYPASS_SELECTED);	m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//11
	bm.LoadBitmap(IDB_TREE_COMPONENT_SKIP);				m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//12
	bm.LoadBitmap(IDB_TREE_COMPONENT_SKIP_SELECTED);	m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//13
	bm.LoadBitmap(IDB_TREE_MODEL);						m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//14
	bm.LoadBitmap(IDB_TREE_MODEL_CHECK);				m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//15
	bm.LoadBitmap(IDB_TREE_WINDOW);						m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//16
	bm.LoadBitmap(IDB_TREE_WINDOW_ENABLE);				m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//17
	bm.LoadBitmap(IDB_TREE_WINDOW_DISABLE);				m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//18
	bm.LoadBitmap(IDB_TREE_LOGIC_NODE);					m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//19 //成, Logic	
	bm.LoadBitmap(IDB_TREE_LOGIC_NODE_SELECTED);		m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//20 //成, Logic	
	bm.LoadBitmap(IDB_TREE_LOGIC_NODE_CHILD);			m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//21 //成, Logic	
	bm.LoadBitmap(IDB_TREE_LOGIC_NODE_SELECTED_CHILD);	m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//22 //成, Logic	
	
	bm.LoadBitmap(IDB_TREE_SUB_MODEL);					m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//23
	bm.LoadBitmap(IDB_TREE_SUB_CLUSTER);				m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//24
	bm.LoadBitmap(IDB_TREE_TYPE_NODE);					m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//25	
	
	bm.LoadBitmap(IDB_TREE_CLUSTER);					m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//26
//	bm.LoadBitmap(IDB_TREE_CLUSTER_CHECK);				m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();	

	bm.LoadBitmap(IDB_TREE_OTHERS);						m_TreeImageList.Add(&bm, RGB(212,208,200));	bm.DeleteObject();//27		
	*/
	CTreeCtrl::SetImageList(&m_TreeImageList, TVSIL_NORMAL);	
	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::BuildTreeWnd()
{
	CTreeCtrlComponent::ClearTreeWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return FALSE; }

	LPARAM aa = MakeItemlParam(254, 16777215);//WORD-246bit, 0 ~ 16777216
	unsigned int ItemType=0, ItemIndex=0;
	DecodeItemlParam(aa, ItemType, ItemIndex);
	//LPARAM  MakeItemlParam(WORD ItemType, WORD ItemIndex)//WORD-16bit, 0 ~ 65535
	//void DecodeItemlParam(LPARAM lParam, size_t &ItemType, size_t &ItemIndex)

	//Project
	// +Panel
	//  +FD	
	//  +Board
	//   +FD
	//   +Component
	//    +Window
	CAOIPanel *pPanel = NULL;
	
	size_t i=0;
	const size_t TextLen = 256;
	TCHAR text[TextLen]=_T("");
	TCHAR text2[TextLen]=_T("");
	TVINSERTSTRUCT tvInsert;
	tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	HTREEITEM TreeItem1=NULL, TreeItem2=NULL, TreeItem3=NULL;	
	
	CString    str;	
	
	CTreeCtrl::SetRedraw(FALSE);
	//---------------------------------------------------------------------------//	
	//專案參數
	str = ProjectPtr->GetProjectModuleName();
	::_stprintf(text, _T("Model: %s"), (LPCTSTR)str);
	tvInsert.hParent = TVI_ROOT;
	tvInsert.item.pszText = text;	
	tvInsert.item.iImage = TREE_IMAGE_LIST_PROGRAM;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_PROGRAM;
	tvInsert.item.lParam  = MakeItemlParam(TREE_ITEM_TYPE_PROJECT, 0);
	TreeItem1 = CTreeCtrl::InsertItem(&tvInsert);
	CTreeCtrlComponent::InsertProjectNodeToTreeCtrl(TreeItem1);
	//---------------------------------------------------------------------------//
	const size_t NPanels = ProjectPtr->GetProjectPanelCount();
	for ( i=0; i<NPanels; i++ )
	{
		pPanel = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == pPanel ) { continue; }
		if ( pPanel->GetPanelDeleted() == true ) { continue; }
		
		//Panel的基本節點
		::_stprintf(text, _T("Panel-%d"), i+1);
		tvInsert.hParent = TVI_ROOT;
		tvInsert.item.pszText = text;
		if ( pPanel->GetPanelBypassed() == false )
		{
			tvInsert.item.iImage = TREE_IMAGE_LIST_PANEL;
			tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_PANEL_SELECTED;
		}
		else
		{
			tvInsert.item.iImage = TREE_IMAGE_LIST_PANEL_BYPASS;
			tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_PANEL_BYPASS_SELECTED;
		}
		tvInsert.item.lParam  = MakeItemlParam(TREE_ITEM_TYPE_PANEL, i);
		TreeItem1 = CTreeCtrl::InsertItem(&tvInsert);

		if ( CTreeCtrlComponent::InsertPanelNodeToTreeCtrl(TreeItem1, pPanel) == FALSE )
		{
			CTreeCtrl::SetRedraw(TRUE);
			CTreeCtrl::Invalidate();
			CTreeCtrl::UpdateWindow();
			CTreeCtrlComponent::ClearTreeWnd();
			return FALSE;
		}
	}
	CTreeCtrl::SetRedraw(TRUE);
	CTreeCtrl::Invalidate();
	CTreeCtrl::UpdateWindow();	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::InsertProjectNodeToTreeCtrl(HTREEITEM hProjectItem)//增加專案資訊
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==hProjectItem || NULL==ProjectPtr ) { return FALSE; }
	CString str;
	size_t i=0, j=0, k=0, s=0, t=0;
	const size_t TextLen = 256;
	TCHAR text[TextLen]=_T("");
	TCHAR text2[TextLen]=_T("");

	TVINSERTSTRUCT tvInsert;
	tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	HTREEITEM TreeItem1=NULL;	

	tvInsert.hParent = hProjectItem;
	str = _T("FileName");
	::_stprintf(text, _T("File: %s"), (LPCTSTR)str);
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_PROJECT_FILENAME, 0);
	TreeItem1 = CTreeCtrl::InsertItem(&tvInsert);

	::_stprintf(text, _T("Region Width= %.0f um"), 100.0);
	tvInsert.hParent = hProjectItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_PROJECT_REGION_W, 0);
	TreeItem1 = CTreeCtrl::InsertItem(&tvInsert);

	::_stprintf(text, _T("Region Height= %.0f um"), 100.0);
	tvInsert.hParent = hProjectItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_PROJECT_REGION_H, 0);
	TreeItem1 = CTreeCtrl::InsertItem(&tvInsert);

	::_stprintf(text, _T("FD Range X= %.2f mm"), 0.0);
	tvInsert.hParent = hProjectItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_PROJECT_FD_RANGE_W, 0);
	TreeItem1 = CTreeCtrl::InsertItem(&tvInsert);

	::_stprintf(text, _T("FD Range Y= %.2f mm"), 0.0);
	tvInsert.hParent = hProjectItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_PROJECT_FD_RANGE_H, 0);
	TreeItem1 = CTreeCtrl::InsertItem(&tvInsert);

	CString BarcodeName;
//	BarcodeName = Project->GetBarcodeStats(Project->m_ProjectParameter.m_BarcodeStates);
	::_stprintf(text, _T("Barcode Mode: %s"), (LPCTSTR)BarcodeName);

	tvInsert.hParent = hProjectItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_PROJECT_BARCODE_MODE, 0);;
	TreeItem1 = CTreeCtrl::InsertItem(&tvInsert);

//	if ( Project->m_ProjectParameter.m_BarcodeStates!=PROJECT_BARCODE_HARDWARE_STOP )
//	{
//		::_stprintf(text, _T("Barcode: %s"), BarcodeName);
//	}
//	else	
//	{
//		if ( Project->m_ProjectParameter.m_BarcodeAutoExtendMode != BARCODE_AUTO_EXTEND_MODE_DISABLE ) 
//		{	::_stprintf(text, _T("Barcode: %s_Extend"), BarcodeName);	}
//		else
//		{	::_stprintf(text, _T("Barcode: %s"), BarcodeName);	}
//	}

	tvInsert.hParent = hProjectItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_PROJECT_BARCODE_MODE, 0);;
	TreeItem1 = CTreeCtrl::InsertItem(&tvInsert);	
	return TRUE;
}
//-----------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::InsertPanelNodeToTreeCtrl(HTREEITEM hPanelItem, CAOIPanel* pPanel)//增加一個零件的節點
{	
	if ( NULL==hPanelItem || NULL==pPanel ) { return FALSE; }
	
	const size_t NFiducials = pPanel->GetPanelFdCount();
	const size_t NBoards    = pPanel->GetPanelBoardCount();
	
	size_t i=0, j=0;
	size_t NComponents=0;		
	CAOIFd    *pFd = NULL;		
	CAOIBoard *pBoard = NULL;
	CAOIComponent *pComponent = NULL;	
	//CModelObj  *ModelPtr = NULL;
	CString   strBarcode;

	TVINSERTSTRUCT tvInsert;
	tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;	
	HTREEITEM hFDGroupItem=NULL, hFDItem=NULL, hBoardItem=NULL, SubTreeItem2=NULL, hComponentItem=NULL, hGroupItem=NULL;
	const size_t textlen = 256;
	TCHAR text[textlen]=_T("");	
	int ComponentType = 0;
	//---------------------------------------------------------------------------//		
	//定位點
	::_tcscpy(text, _T("Fiducial"));
	tvInsert.hParent = hPanelItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_FD;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_FD_SELECTED;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_FD_GROUP, 0);
	hFDGroupItem = CTreeCtrl::InsertItem(&tvInsert);
	for ( i=0; i<NFiducials; i++ )
	{
		pFd = pPanel->GetPanelFdPtr(i, false);
		if ( NULL == pFd ) { continue; }
		
		//::strcpy(text, pfd->GetFiducialName());
		::_stprintf(text, _T("FD %d"), i+1);
		tvInsert.hParent = hFDGroupItem;
		tvInsert.item.pszText = text;
		tvInsert.item.iImage = TREE_IMAGE_LIST_FD;
		tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_FD_SELECTED;
		tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_FD, pFd->GetFdIndex_Project());//Fiducial 引數
		hFDItem = CTreeCtrl::InsertItem(&tvInsert);
		CTreeCtrlComponent::InsertFdNodeToTreeCtrl(hFDItem, pFd, i);
	}
	//---------------------------------------------------------------------------//	
	//單板
	for ( i=0; i<NBoards; i++ )
	{
		pBoard = pPanel->GetPanelBoardPtr(i, false);
		if ( NULL == pBoard ) { continue; }
		//if ( pBoard->GetIsDeleted_B() == true ) { continue; }
		//----------------------------------------------------------------------//
		strBarcode = pBoard->GetBoardBarcode();
		::_stprintf(text, _T("Board %d (%s)"), pBoard->GetBoardIndex_Panel()+1, (LPCTSTR)strBarcode);

		tvInsert.hParent = hPanelItem;
		tvInsert.item.pszText = text;		
		if ( pBoard->GetBoardBypassed() == false )
		{
			tvInsert.item.iImage = TREE_IMAGE_LIST_BOARD;
			tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_BOARD_SELECTED;
		}
		else
		{
			tvInsert.item.iImage = TREE_IMAGE_LIST_BOARD_BYPASS;
			tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_BOARD_BYPASS_SELECTED;			
		}
		tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_BOARD, pBoard->GetBoardIndex_Project());//Board Index
		hBoardItem = CTreeCtrl::InsertItem(&tvInsert);

		NComponents = pBoard->GetBoardComponentCount();
		for ( j=0; j<NComponents; j++ )
		{
			pComponent = pBoard->GetBoardComponentPtr(j, false);
			if ( NULL == pComponent ) { continue; }
			//if ( pComponent->GetIsDeleted_C() == true ) { continue; }				
					
			::_tcscpy(text, CString(pComponent->GetComponentName()));
			tvInsert.hParent = hBoardItem;
			tvInsert.item.pszText = text;
			if ( pComponent->GetComponentBypassed() == true )
			{
				tvInsert.item.iImage = TREE_IMAGE_LIST_COMPONENT_BYPASS;
				tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_COMPONENT_BYPASS_SELECTED;
			}
			else if ( pComponent->GetComponentXBoardUnit() == true )
			{
				tvInsert.item.iImage = TREE_IMAGE_LIST_COMPONENT_SKIP;
				tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_COMPONENT_SKIP_SELECTED;
			}
			else
			{
				tvInsert.item.iImage = TREE_IMAGE_LIST_COMPONENT;
				tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_COMPONENT_SELECTED;		
			}
			tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_COMPONENT, pComponent->GetComponentIndex_Project());//Component Index
			hComponentItem = CTreeCtrl::InsertItem(&tvInsert);
			//----------------------------------------------------------------------//
			if( CTreeCtrlComponent::InsertComponentWindowToTreeCtrl(hComponentItem, pComponent) == false ) 
			{	return FALSE; }
		}		
	}	
	return TRUE;
}
//--------------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::InsertFdNodeToTreeCtrl(HTREEITEM hFDItem, CAOIFd *pFd, size_t FDID)//增加一個定位點的節點	
{
	if ( NULL==hFDItem || NULL==pFd ) { return FALSE; }

	TVINSERTSTRUCT tvInsert;
	tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	
	CString str;
	HTREEITEM SubTreeItem=NULL;
	const size_t textlen = 256;
	TCHAR text[textlen]=_T("");	
	
	::_tcscpy(text, _T("Image Type= Normal"));
	tvInsert.hParent = hFDItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = FDID;//Fiducial 引數
	SubTreeItem = CTreeCtrl::InsertItem(&tvInsert);

	//::_stprintf(text, "Golden Pos= (%.3f, %.3f)mm", pfd->GetFDStagePositionX()/1000.0, pfd->GetFDStagePositionY()/1000.0);
	::_tcscpy(text, _T("Golden Position"));
	tvInsert.hParent = hFDItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = FDID;//Fiducial 引數
	SubTreeItem = CTreeCtrl::InsertItem(&tvInsert);
	
	//::_stprintf(text, "Golden Score= %.2f%%", pfd->GetFDScore() );		
	::_tcscpy(text, _T("Golden Score"));
	tvInsert.hParent = hFDItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = FDID;//Fiducial 引數
	SubTreeItem = CTreeCtrl::InsertItem(&tvInsert);		

	//::_stprintf(text, "Current Pos= (%.3f, %.3f)mm", pfd->GetCalibrateStagePosX()/1000.0, pfd->GetCalibrateStagePosY()/1000.0);		
	::_tcscpy(text, _T("Current Position"));
	tvInsert.hParent = hFDItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = FDID;//Fiducial 引數
	SubTreeItem = CTreeCtrl::InsertItem(&tvInsert);		

	//::_stprintf(text, "Exposure Time= %d us", pfd->GetFDExposureTime());		
	::_tcscpy(text, _T("Exposure Time"));
	tvInsert.hParent = hFDItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = FDID;//Fiducial 引數
	SubTreeItem = CTreeCtrl::InsertItem(&tvInsert);		

	//str = CAlgImg::GetAlgImageSourceText(pfd->GetFDImageSource());
	//::_stprintf(text, "Binary Mode= %s", str);		
	::_tcscpy(text, _T("Binary Mode"));
	tvInsert.hParent = hFDItem;
	tvInsert.item.pszText = text;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = FDID;//Fiducial 引數
	SubTreeItem = CTreeCtrl::InsertItem(&tvInsert);
	return TRUE;
}
//--------------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::InsertComponentWindowToTreeCtrl(HTREEITEM HComponentItem, CAOIComponent *pComponent)//增加一個零件的節點	
{
	if ( NULL==HComponentItem || NULL==pComponent ) { return FALSE; }	
	//---------------------------------------------------------------------//		
	CString str;
	TVINSERTSTRUCT tvInsert;
	tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;	
	HTREEITEM NewComponentItem = NULL;
	HTREEITEM hModelItem = NULL;
	HTREEITEM hWindowItem = NULL;
	HTREEITEM hWndGroupltem = NULL;	
	
	int            i=0, j=0;
	int            WndGrupID = 0;	
	CString        WndItemText;	
//	CAOIWindow    *pWindow = NULL;	
//	CWndObj       *WndPtr = NULL;
//	CLandObj      *LandPtr = NULL;

	const size_t TextLen = 128;
	TCHAR ItemName[TextLen]=_T("");	
//	const int NWindows = pComponent->GetNWindows_C();
//	CModelObj  *ModelPtr = pComponent->GetComponentModelPtrActived();
	
	//Part Number Name
	str = pComponent->GetComponentPartNumber();
	::_tcscpy(ItemName, str);
	tvInsert.hParent = HComponentItem;
	tvInsert.item.pszText = ItemName;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_COMPONENT_PART_NUMBER, 0);	
	hModelItem = CTreeCtrl::InsertItem(&tvInsert);	
	//----------------------------------------------------------------------//		
	//Position
	pComponent->GetComponentLocationText(ItemName);		
	tvInsert.hParent = HComponentItem;
	tvInsert.item.pszText = ItemName;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_COMPONENT_POSITION, 0);
	hModelItem = CTreeCtrl::InsertItem(&tvInsert);
	//----------------------------------------------------------------------//
	pComponent->GetComponentOffsetText(ItemName);	
	tvInsert.hParent = HComponentItem;
	tvInsert.item.pszText = ItemName;
	tvInsert.item.iImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_OTHERS;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_COMPONENT_OFFSET, 0);
	hModelItem = CTreeCtrl::InsertItem(&tvInsert);
	//----------------------------------------------------------------------//	
	
	//if ( ModelPtr != NULL )
	//{	::_stprintf(ItemName, _T("%s"), ModelPtr->GetModelName());	}
	//else
	//{	::_stprintf(ItemName, _T("%s"), "NULL"); }
	str = pComponent->GetComponentModelName();
	::_stprintf(ItemName, _T("%s"), (LPCTSTR)str);
	tvInsert.hParent = HComponentItem;
	tvInsert.item.pszText = ItemName;
	tvInsert.item.iImage = TREE_IMAGE_LIST_MODEL;
	tvInsert.item.iSelectedImage = TREE_IMAGE_LIST_MODEL;
	tvInsert.item.lParam  = ::MakeItemlParam(TREE_ITEM_TYPE_COMPONENT_MODE, 0);;//Com Tree:-1, Model Tree:Component Cluster Index, pComponent->GetModelIndex_C();
	hModelItem = CTreeCtrl::InsertItem(&tvInsert);	
	return TRUE;
}
//--------------------------------------------------------------------------------//
void CTreeCtrlComponent::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CTreeCtrlComponent::DoUpdateComponentTreeClick();
	*pResult = 0;
}
//--------------------------------------------------------------------------------//
void CTreeCtrlComponent::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here	
	if ( m_StopComponentTreeBeClick == TRUE ) { return ; }	
	if ( m_ClickComponentTreeNode == TRUE )
	{	
		m_ClickComponentTreeNode = FALSE;
		return;
	}	
	HTREEITEM hItem = ((NM_TREEVIEW*) pNMTreeView)->itemNew.hItem;
	if ( hItem == NULL ) { return; }
	CTreeCtrlComponent::DoSelectComponentTreeWndItem(hItem);	
	
	*pResult = 0;
}
//--------------------------------------------------------------------------------//
void CTreeCtrlComponent::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	// TODO: Add your control notification handler code here
//	if ( AOIDataCollect.GetIsGrabingImage() == true ) { return ; }
//	AOIDataCollect.SetIsGrabingImage(true);
//	this->DoSendMessage(EDIT_VIEW_WINDOW_TO_CURRENT_POS, NULL, NULL);	
//	AOIDataCollect.UpdateStatsticsResult();	
	HWND hWnd = CWnd::GetSafeHwnd();	
	AOIDataCollect.PostParentWndMessage(hWnd, MSG_TREE_WND_MOVE_TO_ACTIVE_OBJ, NULL, NULL);
	*pResult = 0;
}
//--------------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::UpdateComponentTreeWnd(BOOL SelectedOnly)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || NULL==this->GetSafeHwnd() ) { return FALSE; }

	HTREEITEM hPanelItem = CTreeCtrl::GetRootItem();
	if ( hPanelItem == NULL ) { return FALSE; }//沒有建立任何結點			

	HTREEITEM hBoardItem = NULL;
	HTREEITEM hComponentItem=NULL;		
	CAOIPanel     *pPanel = NULL;
	CAOIBoard     *pBoard = NULL;
	CAOIComponent *pComponent = NULL;

	CString str;	
	DWORD_PTR    ItemData = 0;
	unsigned int ItemType = 0;
	unsigned int ItemIndex = 0;	
		
	this->m_StopComponentTreeBeClick = TRUE;
	while ( hPanelItem != NULL )
	{		
		ItemData = CTreeCtrl::GetItemData(hPanelItem);		
		::DecodeItemlParam(ItemData, ItemType, ItemIndex);
		if ( TREE_ITEM_TYPE_PANEL != ItemType ) 
		{
			hPanelItem = CTreeCtrl::GetNextSiblingItem(hPanelItem);
			continue; 
		}
		pPanel = ProjectPtr->GetProjectPanelPtr(ItemIndex, true);
		if ( pPanel == NULL )//可能是Panel或者是Fiducial
		{
			hPanelItem = CTreeCtrl::GetNextSiblingItem(hPanelItem);
			continue; 
		}

		hBoardItem = CTreeCtrl::GetChildItem(hPanelItem);
		while ( hBoardItem != NULL )
		{
			ItemData = CTreeCtrl::GetItemData(hBoardItem);
			::DecodeItemlParam(ItemData, ItemType, ItemIndex);
			if ( TREE_ITEM_TYPE_BOARD != ItemType ) 
			{
				hBoardItem = CTreeCtrl::GetNextSiblingItem(hBoardItem);
				continue; 
			}			
			pBoard = ProjectPtr->GetProjectBoardPtr(ItemIndex, true);
			if ( pBoard == NULL ) //可能是Panel或者是Fiducial
			{
				hBoardItem = CTreeCtrl::GetNextSiblingItem(hBoardItem);
				continue; 
			}

			hComponentItem = CTreeCtrl::GetChildItem(hBoardItem);
			while ( hComponentItem != NULL )
			{	
				ItemData = CTreeCtrl::GetItemData(hComponentItem);
				::DecodeItemlParam(ItemData, ItemType, ItemIndex);
				if ( TREE_ITEM_TYPE_COMPONENT != ItemType ) 
				{
					hComponentItem = CTreeCtrl::GetNextSiblingItem(hComponentItem);
					continue;
				}				
				pComponent = ProjectPtr->GetProjectComponentPtr(ItemIndex, true);
				if ( pComponent == NULL )		
				{
					hComponentItem = CTreeCtrl::GetNextSiblingItem(hComponentItem);
					continue;
				}
				
				if ( SelectedOnly == TRUE )//是否僅修改被選擇到的零件
				{
					if ( pComponent->GetComponentSelected() == false ) //若沒有選到則進行下依筆
					{ 
						hComponentItem = CTreeCtrl::GetNextSiblingItem(hComponentItem);
						continue; 
					}
				}

				CTreeCtrlComponent::RemoveTreeItem(hComponentItem);
				if( CTreeCtrlComponent::InsertComponentWindowToTreeCtrl(hComponentItem, pComponent) == FALSE ) 
				{
					this->m_StopComponentTreeBeClick = FALSE;
					return FALSE; 
				}

				hComponentItem = CTreeCtrl::GetNextSiblingItem(hComponentItem);
			};
			hBoardItem = CTreeCtrl::GetNextSiblingItem(hBoardItem);
		}
		hPanelItem = CTreeCtrl::GetNextSiblingItem(hPanelItem);		
	};
	this->m_StopComponentTreeBeClick = FALSE;
	return TRUE;
}
//--------------------------------------------------------------------------------//
HTREEITEM  CTreeCtrlComponent::GetTreeSelectedItem()//取得現在滑鼠下的節點
{	
	if ( CTreeCtrl::GetSafeHwnd() == NULL ) { return NULL; }

	UINT flag=0;
	POINT point;
	::GetCursorPos(&point);	
	CTreeCtrl::ScreenToClient(&point);
	HTREEITEM hItem = CTreeCtrl::HitTest(point, &flag);	
	if ( hItem == NULL ) { return NULL; }
	int Item = TVHT_ONITEM;
	int ItemLabel = TVHT_ONITEMLABEL;
	int ItemIcon = TVHT_ONITEMICON;
	if ( (ItemLabel & flag)!=0 || (ItemIcon & flag)!=0  )
	{	return hItem;	}
	return NULL; 
}
//--------------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::DoUpdateComponentTreeClick()
{	
	//注意, 當刪除Tree的節點時會呼叫到Select Change	
//	this->m_IsClickComponentTree = true;	//chia8
	HTREEITEM hItem = CTreeCtrlComponent::GetTreeSelectedItem();
	if ( hItem == NULL ) 
	{	return TRUE;	}
	
	this->m_ClickComponentTreeNode = TRUE;	//chia8
	//-----------------------------------------------------------------------//	
	this->DoSelectComponentTreeWndItem(hItem);	
	return TRUE;
}
//-----------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::DoSelectComponentTreeWndItem(HTREEITEM hItem)//執行選取到零件樹狀圖的Item
{	
	if ( this->m_StopComponentTreeBeClick == TRUE ) { return TRUE; }	
//	AOIDataCollect.SetCallBackHWND(AOIDataCollect.GetViewHWND());
//	this->ResetClickStatus();	
	if ( hItem == NULL ) { return TRUE; }
	//-----------------------------------------------------------------------//	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return TRUE; }		
	
	//初始化回傳直	
	unsigned int FdIdx = -1;
	unsigned int PanelIdx = -1;
	unsigned int BoardIdx = -1;
	unsigned int ComponentIdx = -1;
	unsigned int WindowIdx = -1;
		
	DWORD_PTR    ItemData = 0;
	unsigned int ItemType = 0;
	unsigned int ItemIndex = 0;	
	HTREEITEM hItemParent = hItem;
	HTREEITEM hItemChild  = hItem;
	HTREEITEM hItemTemp   = hItem;

	CString str;
#ifdef _DEBUG
	str = CTreeCtrl::GetItemText(hItem);
#endif
	ItemData = CTreeCtrl::GetItemData(hItem);
	::DecodeItemlParam(ItemData, ItemType, ItemIndex);
	
	CAOIFd        *pFd=NULL;
	CAOIPanel     *pPanel = NULL;
	CAOIBoard     *pBoard = NULL;
	CAOIComponent *pComponent = NULL;
	CAOIWindow    *pWindow = NULL;
	
	bool AutoExpand_C = false;
	//取得點的的結點是第幾層
	const int NodeLevel = CTreeCtrlComponent::GetTreeNodeLevel(hItem);	
	switch ( ItemType )
	{
	case TREE_ITEM_TYPE_PROJECT:
		CTreeCtrl::Expand(hItem, TVE_EXPAND);
		break;
	case TREE_ITEM_TYPE_PANEL:
		pPanel = ProjectPtr->GetProjectPanelPtr(ItemIndex, true);
		if ( NULL == pPanel ) { return FALSE; }
		PanelIdx = ItemIndex;
		//AutoExpand_C = true;
		break;
	case TREE_ITEM_TYPE_FD:
		pFd = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
		if ( NULL == pFd ) { return FALSE; }
		FdIdx = ItemIndex;
		pBoard = pFd->GetFdBoardPtr();
		if ( NULL != pBoard )
		{	BoardIdx = pBoard->GetBoardIndex_Project(); }
		pPanel = pFd->GetFdPanelPtr();
		if ( NULL != pPanel )
		{	PanelIdx = pPanel->GetPanelIndex_Project(); }		
		break;
	case TREE_ITEM_TYPE_BOARD:
		pBoard = ProjectPtr->GetProjectBoardPtr(ItemIndex, true);
		if ( NULL == pBoard ) { return FALSE; }
		pPanel = pBoard->GetBoardPanelPtr();
		if ( NULL != pPanel )
		{	PanelIdx = pPanel->GetPanelIndex_Project(); }
		BoardIdx = ItemIndex;
		//AutoExpand_C = true;
		break;
	case TREE_ITEM_TYPE_COMPONENT:
	case TREE_ITEM_TYPE_COMPONENT_PACKAGE:
	case TREE_ITEM_TYPE_COMPONENT_PART_NUMBER:
	case TREE_ITEM_TYPE_COMPONENT_POSITION:
	case TREE_ITEM_TYPE_COMPONENT_OFFSET:
	case TREE_ITEM_TYPE_COMPONENT_MODE:
		pComponent = ProjectPtr->GetProjectComponentPtr(ItemIndex, true);
		if ( NULL == pComponent ) { return FALSE; }
		ComponentIdx = ItemIndex;
		pBoard = pComponent->GetComponentBoardPtr();
		if ( NULL != pBoard )
		{	BoardIdx = pBoard->GetBoardIndex_Project(); }
		pPanel = pComponent->GetComponentPanelPtr();
		if ( NULL != pPanel )
		{	PanelIdx = pPanel->GetPanelIndex_Project(); }
		//AutoExpand_C = true;
		break;	
	case TREE_ITEM_TYPE_MODEL:
		break;
	case TREE_ITEM_TYPE_WINDOW:
		break;
	}

	ProjectPtr->ResetProjectActiveIndex();
	ProjectPtr->SetProjectActiveFdIndex(FdIdx);
	ProjectPtr->SetProjectActivePanelIndex(PanelIdx);
	ProjectPtr->SetProjectActiveBoardIndex(BoardIdx);
	//ProjectPtr->SetProjectActiveBarcodeIndex(BoardIdx);
	ProjectPtr->SetProjectActiveComponentIndex(ComponentIdx);
	ProjectPtr->SetProjectActiveComponentWindowIndex(WindowIdx);
	
	/*
	pPanel = ProjectPtr->GetProjectActivePanel();
	pBoard = ProjectPtr->GetProjectActiveBoard();
	pComponent = ProjectPtr->GetProjectActiveComponent();
	pWindow = AOIDataCollect.GetActiveWindow();
	
	//檢測框不支援多選
	Project->SetAllWindowsIsSelected(false);	
	if ( AOIDataCollect.GetIsPressVRKey(VK_CONTROL) == false )
	{	
		Project->SetAllComponentsIsSelected(false); 
	}	

	int i=0, j=0, s=0;	

	if ( FDID >= 0 )
	{
	}
	else
	{
		ModelPtr = NULL;
		WndPtr = NULL;
		if ( pWindow != NULL )
		{	
			pWindow->SetIsSelected_W(true);	
			ModelPtr = pWindow->GetModelPtr_W();
			WndPtr = pWindow->GetWndObjPtr_W();
		}
		
		if ( pComponent != NULL )
		{	
			pComponent->SetIsSelected_C(true);	
			AOIDataCollect.m_ModelIndex = pComponent->GetModelIndex_C();
			if ( ModelPtr == NULL )
			{	ModelPtr = pComponent->GetComponentModelPtrActived(); }

			if ( ModelPtr != NULL )
			{
				ModelPtr->UnSelectModel();					
				if ( WndPtr == NULL )
				{	ModelPtr->GetComponentBox()->SetActived(TRUE); }
				else
				{						
					WndPtr->SetWndSelected(TRUE); 
					ModelPtr->InvisibleModelWnd();
					ModelPtr->SetModelWndVisibledByWndGroupID(WndPtr->GetWndGroupIDModel(), -1, true);
				}
				this->DoSendMessage(MAIN_FRAME_BUILD_MODEL_PARAM, (WPARAM)ModelPtr, TRUE);
			}


			if ( AOIDataCollect.GetIsMultiBoardControlMode() == true )
			{			
				str = pComponent->GetComponentName_C();			
				const int NPanels = Project->GetNPanels();
				for ( s=0; s<NPanels; s++ )
				{
					pPanel = Project->GetPanelPtr(s);
					int NBoards = pPanel->GetNBoards();
					for ( i=0; i<NBoards; i++ )
					{
						if ( i==AOIDataCollect.m_ActBoardIndex && s==AOIDataCollect.m_ActPanelIndex ) { continue; }
						pBoard = pPanel->GetBoardPtr(i);

						pComponent = pBoard->FindComponent(str);
						if ( pComponent == NULL ) { continue; }
						pComponent->SetIsSelected_C(true);
					}
				}
			}
		}
		else
		{
			if ( pBoard != NULL )//僅點選到單板上去
			{
				pBoard->SetIsSelected_B(true);
				pBoard->SetAllComponentsIsSelected_B(true);
			}
			else
			{
				if ( pPanel != NULL )
				{
					pPanel->SetIsSelected_P(true);
					pPanel->SetAllComponentsIsSelected_P(true);
				}
			}
		}		
	}

	if ( (AutoExpand_C==true) && (AOIDataCollect.GetIsPressVRKey(VK_CONTROL)==false) )
	{	TreeCtrl.Expand(hItem, TVE_EXPAND );	}
	
	if ( WindowID < 0 ) 
	{	this->DoUpdateComponentTreeWndItemState(false);	 }
	else
	{	this->DoUpdateComponentTreeWndItemState(true);	 }

	this->DoSendMessage(EDIT_VIEW_UPDATE_ACTIVE_OBJECT, TRUE, NULL);
	*/
	return TRUE;
}
//--------------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::DoUpdateComponentTreeWndItemState(const bool IsWithWindow)//更新零件樹狀圖的結點
{		
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || NULL==this->GetSafeHwnd() ) { return FALSE; }
	
	HTREEITEM hPanelItem = CTreeCtrl::GetRootItem();
	if ( hPanelItem == NULL ) { return TRUE; }//沒有建立任何結點	

	HTREEITEM hBoardItem = NULL;	
	HTREEITEM hComponentItem=NULL;
	HTREEITEM hWindowItem=NULL;
	HTREEITEM hComponentModeltem=NULL;
	HTREEITEM hGroupltem=NULL;
	HTREEITEM hWndGroupltem=NULL;

	DWORD_PTR    ItemData = 0;
	unsigned int ItemType = 0;
	unsigned int ItemIndex = 0;	
	CAOIPanel *pPanel = NULL;
	CAOIBoard *pBoard = NULL;
	CAOIComponent *pComponent = NULL;
//	CAOIComponent *pComponentSelected = ProjectPtr->GetProjectActiveComponent();
	CAOIWindow *pWindow=NULL;
	
	CString str;
#ifdef _DEBUG
	CString PanelName, BoardName, ComName, GroupName, WndGroupName, WindowName;
#endif

	this->m_StopComponentTreeBeClick = TRUE;
	//CTreeCtrl::SelectItem(NULL);//pComponentSelected
	while ( NULL != hPanelItem )
	{
		ItemData = CTreeCtrl::GetItemData(hPanelItem);
		::DecodeItemlParam(ItemData, ItemType, ItemIndex);
		pPanel = ProjectPtr->GetProjectPanelPtr(ItemIndex, true);		
		if ( TREE_ITEM_TYPE_PANEL!=ItemType || NULL==pPanel )
		{
			hPanelItem = CTreeCtrl::GetNextSiblingItem(hPanelItem);
			continue; 
		}				
	#ifdef _DEBUG
		PanelName = CTreeCtrl::GetItemText(hPanelItem);
	#endif

		if ( pPanel->GetPanelBypassed() == false )
		{	CTreeCtrl::SetItemImage(hPanelItem, TREE_IMAGE_LIST_PANEL, TREE_IMAGE_LIST_PANEL_SELECTED);	}
		else
		{	CTreeCtrl::SetItemImage(hPanelItem, TREE_IMAGE_LIST_PANEL_BYPASS, TREE_IMAGE_LIST_PANEL_BYPASS_SELECTED);	}

		hBoardItem = CTreeCtrl::GetChildItem(hPanelItem);	
		while ( NULL != hBoardItem )
		{
			ItemData = CTreeCtrl::GetItemData(hBoardItem);
			::DecodeItemlParam(ItemData, ItemType, ItemIndex);
			pBoard = ProjectPtr->GetProjectBoardPtr(ItemIndex, true);
			if ( TREE_ITEM_TYPE_BOARD!=ItemType || NULL==pBoard ) //可能是Panel或者是Fiducial
			{
				hBoardItem = CTreeCtrl::GetNextSiblingItem(hBoardItem);
				continue; 
			}
			
		#ifdef _DEBUG
			BoardName = CTreeCtrl::GetItemText(hBoardItem);
		#endif

			if ( pBoard->GetBoardBypassed() == true )
			{	CTreeCtrl::SetItemImage(hBoardItem, TREE_IMAGE_LIST_BOARD_BYPASS, TREE_IMAGE_LIST_BOARD_BYPASS_SELECTED);	}
			else
			{	CTreeCtrl::SetItemImage(hBoardItem, TREE_IMAGE_LIST_BOARD, TREE_IMAGE_LIST_BOARD_SELECTED);	}

			if ( pBoard->GetBoardSelected() == true )
			{	CTreeCtrl::SetItemState(hBoardItem, TVIS_SELECTED, TVIS_SELECTED);	}
			else
			{	CTreeCtrl::SetItemState(hBoardItem, 0, TVIS_SELECTED);	}			
			//-------------------------------------------------------//
			hComponentItem = CTreeCtrl::GetChildItem(hBoardItem);
			while ( NULL != hComponentItem )
			{				
				ItemData = CTreeCtrl::GetItemData(hComponentItem);
				::DecodeItemlParam(ItemData, ItemType, ItemIndex);				
				pComponent = ProjectPtr->GetProjectComponentPtr(ItemIndex, true);
				if ( TREE_ITEM_TYPE_COMPONENT!=ItemType || NULL==pComponent )		
				{
					hComponentItem = CTreeCtrl::GetNextSiblingItem(hComponentItem);
					continue;
				}
			#ifdef _DEBUG
				ComName = CTreeCtrl::GetItemText(hComponentItem);
			#endif
				
				if ( pComponent->GetComponentBypassed() == true )
				{	CTreeCtrl::SetItemImage(hComponentItem, TREE_IMAGE_LIST_COMPONENT_BYPASS, TREE_IMAGE_LIST_COMPONENT_BYPASS_SELECTED);	}
				else if ( pComponent->GetComponentXBoardUnit() == true )
				{	CTreeCtrl::SetItemImage(hComponentItem, TREE_IMAGE_LIST_COMPONENT_SKIP, TREE_IMAGE_LIST_COMPONENT_SKIP_SELECTED);		}
				else
				{	CTreeCtrl::SetItemImage(hComponentItem, TREE_IMAGE_LIST_COMPONENT, TREE_IMAGE_LIST_COMPONENT_SELECTED);	}

				if ( pComponent->GetComponentSelected() == true )
				{	CTreeCtrl::SetItemState(hComponentItem, TVIS_SELECTED, TVIS_SELECTED);	}
				else
				{	CTreeCtrl::SetItemState(hComponentItem, 0, TVIS_SELECTED);	}

				if ( IsWithWindow == true )
				{
					//零件下的狀態
					hComponentModeltem = CTreeCtrl::GetChildItem(hComponentItem);//Package
					while ( hComponentModeltem != NULL )
					{
					#ifdef _DEBUG
						GroupName = CTreeCtrl::GetItemText(hComponentModeltem);
					#endif
						ItemData = CTreeCtrl::GetItemData(hComponentModeltem);
						::DecodeItemlParam(ItemData, ItemType, ItemIndex);							
						if ( ItemType==TREE_ITEM_TYPE_COMPONENT_PACKAGE || 
							 ItemType==TREE_ITEM_TYPE_COMPONENT_PART_NUMBER ||
							 ItemType==TREE_ITEM_TYPE_COMPONENT_POSITION ||
							 ItemType==TREE_ITEM_TYPE_COMPONENT_OFFSET 
							)
						{
							hComponentModeltem = CTreeCtrl::GetNextSiblingItem(hComponentModeltem);
							continue;
						}

						hWndGroupltem = CTreeCtrl::GetChildItem(hComponentModeltem);
						while ( hWndGroupltem != NULL )
						{
						#ifdef _DEBUG
							WndGroupName = CTreeCtrl::GetItemText(hWndGroupltem);
						#endif
							
							hWindowItem = CTreeCtrl::GetChildItem(hWndGroupltem);
							while ( hWindowItem != NULL )
							{
							#ifdef _DEBUG
								WindowName = CTreeCtrl::GetItemText(hWindowItem);
							#endif								
								ItemData = CTreeCtrl::GetItemData(hWindowItem);
								::DecodeItemlParam(ItemData, ItemType, ItemIndex);	
								pWindow = pComponent->GetComponentWindowPtr(ItemIndex, true);
								if ( TREE_ITEM_TYPE_WINDOW!=ItemType || NULL==pWindow ) 
								{	
									hWindowItem = CTreeCtrl::GetNextSiblingItem(hWindowItem);
									continue; 
								}
								//if ( pWindow->GetIsDisabled() == true )
								//{	CTreeCtrl::SetItemImage(hWindowItem, TREE_IMAGE_LIST_WINDOW_DISABLE, TREE_IMAGE_LIST_WINDOW_DISABLE);	}
								//else
								//{	CTreeCtrl::SetItemImage(hWindowItem, TREE_IMAGE_LIST_WINDOW_ENABLE, TREE_IMAGE_LIST_WINDOW_ENABLE);	}
								hWindowItem = CTreeCtrl::GetNextSiblingItem(hWindowItem);
							};
							hWndGroupltem = CTreeCtrl::GetNextSiblingItem(hWndGroupltem);
						};
						
						hComponentModeltem = CTreeCtrl::GetNextSiblingItem(hComponentModeltem);
					}
				}
				hComponentItem = CTreeCtrl::GetNextSiblingItem(hComponentItem);
			};
			hBoardItem = CTreeCtrl::GetNextSiblingItem(hBoardItem);
		};
		hPanelItem = CTreeCtrl::GetNextSiblingItem(hPanelItem);
	};
	this->m_StopComponentTreeBeClick = FALSE;
	return TRUE;
}
//--------------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::MakeSureComponentTreeNodeVisible(int ToLevel)//確定Window可以看得到
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || NULL==this->GetSafeHwnd() ) { return FALSE; }

	int i=0;
	CString str;
	DWORD_PTR    ItemData = 0;
	unsigned int ItemType = 0;
	unsigned int ItemIndex = 0;
	HTREEITEM  hPanelItem = NULL;
	HTREEITEM  hBoardItem = NULL;
	HTREEITEM  hComponentItem = NULL;
	HTREEITEM  hGroupItem = NULL;
	HTREEITEM  hWindowItem = NULL;
	HTREEITEM  hWndGroupItem = NULL;
	
	bool IsFind = false;
	const unsigned int PanelIndex = ProjectPtr->GetProjectActivePanelIndex();
	const unsigned int BoardIndex = ProjectPtr->GetProjectActiveBoardIndex();
	const unsigned int ComponentIndex = ProjectPtr->GetProjectActiveComponentIndex();
	const unsigned int WindowIndex = ProjectPtr->GetProjectActiveComponentWindowIndex();

#ifdef _DEBUG
	CString PanelName, BoardName, ComName, GroupName, WndGroupName, WindowName;
#endif

	hPanelItem = CTreeCtrl::GetRootItem();	
	this->m_StopComponentTreeBeClick = TRUE;	
	//CTreeCtrl::SelectItem(NULL);	
	while ( NULL != hPanelItem )
	{
	#ifdef _DEBUG
		PanelName = CTreeCtrl::GetItemText(hPanelItem);
	#endif
		ItemData = CTreeCtrl::GetItemData(hPanelItem);		
		::DecodeItemlParam(ItemData, ItemType, ItemIndex);		
		if ( TREE_ITEM_TYPE_PANEL!=ItemType || ItemIndex!=PanelIndex )		
		{
			hPanelItem = CTreeCtrl::GetNextSiblingItem(hPanelItem);
			continue;
		}			
		
		if ( ToLevel == TREE_NODE_PANEL_ID )
		{
			CTreeCtrl::SelectItem(hPanelItem);
			CTreeCtrl::SetItemState(hPanelItem, TVIS_SELECTED, TVIS_SELECTED);
			CTreeCtrl::Expand(hPanelItem, TVE_EXPAND);
			CTreeCtrl::EnsureVisible(hPanelItem);
			this->m_StopComponentTreeBeClick = FALSE;
			IsFind = true;
			return TRUE;
		}

		hBoardItem = CTreeCtrl::GetChildItem(hPanelItem);
		while ( NULL!=hBoardItem  )
		{
		#ifdef _DEBUG
			BoardName = CTreeCtrl::GetItemText(hBoardItem);
		#endif
			ItemData = CTreeCtrl::GetItemData(hBoardItem);		
			::DecodeItemlParam(ItemData, ItemType, ItemIndex);			
			if ( TREE_ITEM_TYPE_BOARD!=ItemType || ItemIndex!=BoardIndex ) 
			{
				hBoardItem = CTreeCtrl::GetNextSiblingItem(hBoardItem);
				continue;
			}
			
			if ( ToLevel == TREE_NODE_BOARD_ID )//只選到單板
			{
				if ( ItemData == BoardIndex )
				{
					CTreeCtrl::SelectItem(hBoardItem);
					CTreeCtrl::SetItemState(hBoardItem, TVIS_SELECTED, TVIS_SELECTED);
					//CTreeCtrl::Expand(hBoardItem, TVE_EXPAND);
					CTreeCtrl::EnsureVisible(hBoardItem);
					this->m_StopComponentTreeBeClick = FALSE;
					IsFind = true;
					return TRUE;
				}
			}

			hComponentItem = CTreeCtrl::GetChildItem(hBoardItem);
			while ( hComponentItem!=NULL )
			{
			#ifdef _DEBUG
				ComName = CTreeCtrl::GetItemText(hComponentItem);
			#endif				
				ItemData = CTreeCtrl::GetItemData(hComponentItem);		
				::DecodeItemlParam(ItemData, ItemType, ItemIndex);
				if ( TREE_ITEM_TYPE_COMPONENT!=ItemType || ItemIndex!=ComponentIndex ) 
				{	
					CTreeCtrl::Expand(hComponentItem, TVE_COLLAPSE);
					hComponentItem = CTreeCtrl::GetNextSiblingItem(hComponentItem);
					continue; 
				}
				
				if ( ToLevel == TREE_NODE_COMPONENT_ID )//只選到零件
				{
					if ( ItemIndex == ComponentIndex )
					{
						CTreeCtrl::SelectItem(hComponentItem);
						CTreeCtrl::SetItemState(hComponentItem, TVIS_SELECTED, TVIS_SELECTED);
						//CTreeCtrl::Expand(hComponentItem, TVE_EXPAND);
						CTreeCtrl::EnsureVisible(hComponentItem);
						this->m_StopComponentTreeBeClick = FALSE;
						IsFind = true;
						return TRUE;						
					}
				}

				if ( ToLevel == TREE_NODE_WINDOW_ID )//選到檢測框
				{
					hGroupItem = CTreeCtrl::GetChildItem(hComponentItem);
					while ( hGroupItem != NULL )
					{					
					#ifdef _DEBUG
						GroupName = CTreeCtrl::GetItemText(hGroupItem);
					#endif						
						ItemData = CTreeCtrl::GetItemData(hGroupItem);		
						::DecodeItemlParam(ItemData, ItemType, ItemIndex);
						if ( ItemType==TREE_ITEM_TYPE_COMPONENT_PACKAGE || 
							 ItemType==TREE_ITEM_TYPE_COMPONENT_PART_NUMBER || 
							 ItemType==TREE_ITEM_TYPE_COMPONENT_POSITION ||
							 ItemType==TREE_ITEM_TYPE_COMPONENT_OFFSET
							)
						{
							hGroupItem = CTreeCtrl::GetNextSiblingItem(hGroupItem);
							continue;
						}

						//Collapse Other Group
						hWndGroupItem = CTreeCtrl::GetChildItem(hGroupItem);
						while ( hWndGroupItem != NULL )
						{					
							CTreeCtrl::Expand(hWndGroupItem, TVE_COLLAPSE);
							hWndGroupItem = CTreeCtrl::GetNextSiblingItem(hWndGroupItem);
						};

						hWndGroupItem = CTreeCtrl::GetChildItem(hGroupItem);//Wnd Group
						while ( hWndGroupItem != NULL )
						{
						#ifdef _DEBUG
							WndGroupName = CTreeCtrl::GetItemText(hWndGroupItem);
						#endif
							hWindowItem = CTreeCtrl::GetChildItem(hWndGroupItem);//First Window Item
							while ( hWindowItem!= NULL)
							{
							#ifdef _DEBUG
								WindowName = CTreeCtrl::GetItemText(hWindowItem);
							#endif								
								ItemData = CTreeCtrl::GetItemData(hWindowItem);		
								::DecodeItemlParam(ItemData, ItemType, ItemIndex);
								if ( TREE_ITEM_TYPE_WINDOW!=ItemType || ItemIndex!=WindowIndex )
								{
									hWindowItem = CTreeCtrl::GetNextSiblingItem(hWindowItem);
									continue;
								}

								if ( ItemIndex == WindowIndex )
								{
									CTreeCtrl::SelectItem(hWindowItem);
									CTreeCtrl::SetItemState(hWindowItem, TVIS_SELECTED, TVIS_SELECTED);								
									CTreeCtrl::EnsureVisible(hWindowItem);
									this->m_StopComponentTreeBeClick = FALSE;
									IsFind = true;
									return TRUE;
								}		
								if ( IsFind == true ) { break; }	
								hWindowItem = CTreeCtrl::GetNextSiblingItem(hWindowItem);
							};							
							if ( IsFind == true ) { break; }
							hWndGroupItem = CTreeCtrl::GetNextSiblingItem(hWndGroupItem);
						};
						if ( IsFind == true ) { break; }
						hGroupItem = CTreeCtrl::GetNextSiblingItem(hGroupItem);
					};
				}
				if ( IsFind == true ) { break; }
				hComponentItem = CTreeCtrl::GetNextSiblingItem(hComponentItem);
			};
			if ( IsFind == true ) { break; }
			hBoardItem = CTreeCtrl::GetNextSiblingItem(hBoardItem);	
		};
		if ( IsFind == true ) { break; }
		hPanelItem = CTreeCtrl::GetNextSiblingItem(hPanelItem);	
	};	
	this->m_StopComponentTreeBeClick = FALSE;
	return TRUE;
}
//--------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::DoUpdateComponentTreeItemText(BOOL IsOnlySelected)//更新零件樹狀圖中零件被選取到的結點-僅更改文字
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || NULL==this->GetSafeHwnd() ) { return FALSE; }
	
	HTREEITEM hPanelItem = CTreeCtrl::GetRootItem();
	if ( hPanelItem == NULL ) { return FALSE; }//沒有建立任何結點	

	HTREEITEM hFiducialItem = NULL;
	HTREEITEM hBoardItem = NULL;
	HTREEITEM hComponentItem=NULL;	

	CAOIFd    *pFd = NULL;
	CAOIPanel *pPanel = NULL;
	CAOIBoard *pBoard = NULL;
	CAOIComponent *pComponent = NULL;

	CString  str;
	DWORD_PTR    ItemData = 0;
	unsigned int ItemType = 0;
	unsigned int ItemIndex = 0;

#ifdef _DEBUG
	CString PanelName, BoardName, ComName;
#endif
	
	this->m_StopComponentTreeBeClick = TRUE;
	while ( hPanelItem != NULL )
	{
	#ifdef _DEBUG
		PanelName = CTreeCtrl::GetItemText(hPanelItem);
	#endif
		ItemData = CTreeCtrl::GetItemData(hPanelItem);
		::DecodeItemlParam(ItemData, ItemType, ItemIndex);
		pPanel = ProjectPtr->GetProjectPanelPtr(ItemIndex, true);		
		if ( TREE_ITEM_TYPE_PANEL!=ItemType || NULL==pPanel )//可能是Panel或者是FiducialPanelID = (int)CTreeCtrl::GetItemData(hPanelItem);
		{
			//str.Format(_T("Model: %s"), Project->GetProjectModuleName());			
			//CTreeCtrl::SetItemText(hPanelItem, str);

			CTreeCtrlComponent::RemoveTreeItem(hPanelItem);			
			CTreeCtrlComponent::InsertProjectNodeToTreeCtrl(hPanelItem);
			hPanelItem = CTreeCtrl::GetNextSiblingItem(hPanelItem);
			continue; 			
		}		
		str.Format(_T("Panel-%d"), ItemIndex+1);
		CTreeCtrl::SetItemText(hPanelItem, str);

		hBoardItem = CTreeCtrl::GetChildItem(hPanelItem);
		while ( NULL != hBoardItem )
		{
		#ifdef _DEBUG
			BoardName = CTreeCtrl::GetItemText(hBoardItem);
		#endif			
			ItemData = CTreeCtrl::GetItemData(hBoardItem);
			::DecodeItemlParam(ItemData, ItemType, ItemIndex);
			if ( TREE_ITEM_TYPE_FD_GROUP == ItemType )
			{
				hFiducialItem = CTreeCtrl::GetChildItem(hBoardItem);
				while ( NULL != hFiducialItem )
				{					
					ItemData = CTreeCtrl::GetItemData(hFiducialItem);
					::DecodeItemlParam(ItemData, ItemType, ItemIndex);
					pFd = ProjectPtr->GetProjectFdPtr(ItemIndex, true);
					if ( TREE_ITEM_TYPE_FD!=ItemType || NULL == pFd ) //可能是Panel或者是Fiducial
					{
						hFiducialItem = CTreeCtrl::GetNextSiblingItem(hFiducialItem);
						continue; 
					}			
					CTreeCtrlComponent::RemoveTreeItem(hFiducialItem);
					CTreeCtrlComponent::InsertFdNodeToTreeCtrl(hFiducialItem, pFd, ItemIndex);				
					hFiducialItem = CTreeCtrl::GetNextSiblingItem(hFiducialItem);
				};
				hBoardItem = CTreeCtrl::GetNextSiblingItem(hBoardItem);
				continue; 			
			}
			
			pBoard = ProjectPtr->GetProjectBoardPtr(ItemIndex, true);
			if ( TREE_ITEM_TYPE_BOARD!=ItemType || NULL==pBoard ) //可能是Panel或者是Fiducial
			{
				hBoardItem = CTreeCtrl::GetNextSiblingItem(hBoardItem);
				continue; 
			}			
			str.Format(_T("Board %d (%s)"), ItemIndex+1, pBoard->GetBoardBarcode());
			CTreeCtrl::SetItemText(hBoardItem, str);

			hComponentItem = CTreeCtrl::GetChildItem(hBoardItem);
			while ( NULL!=hComponentItem )
			{			
			#ifdef _DEBUG
				ComName = CTreeCtrl::GetItemText(hComponentItem);
			#endif				
				ItemData = CTreeCtrl::GetItemData(hComponentItem);
				::DecodeItemlParam(ItemData, ItemType, ItemIndex);
				pComponent = ProjectPtr->GetProjectComponentPtr(ItemIndex, true);
				if ( TREE_ITEM_TYPE_COMPONENT!=ItemType ||  NULL==pComponent )
				{
					hComponentItem = CTreeCtrl::GetNextSiblingItem(hComponentItem);
					continue;
				}
				
				if ( IsOnlySelected == TRUE )//是否僅修改被選擇到的零件
				{
					if ( pComponent->GetComponentSelected() == false ) //若沒有選到則進行下依筆
					{ 
						hComponentItem = CTreeCtrl::GetNextSiblingItem(hComponentItem);
						continue; 
					}
				}				
				str = pComponent->GetComponentName();
				CTreeCtrl::SetItemText(hComponentItem, str);				
				if ( CTreeCtrlComponent::DoUpdateComponentNodeItemText(hComponentItem, pComponent) == FALSE )
				{
					JetAPI::ShowMessageBox(_T("Error, ModifyComponentNodeTToTreeCtrl Fault in Component Tree"));
					this->m_StopComponentTreeBeClick = FALSE;					
					return FALSE;
				}
				hComponentItem = CTreeCtrl::GetNextSiblingItem(hComponentItem);
			};
			hBoardItem = CTreeCtrl::GetNextSiblingItem(hBoardItem);
		};
		hPanelItem = CTreeCtrl::GetNextSiblingItem(hPanelItem);		
	};
	this->m_StopComponentTreeBeClick = FALSE;
	return TRUE;
}
//--------------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::DoUpdateComponentNodeItemText(HTREEITEM HComponentItem, CAOIComponent *pComponent)//修正零件的文字, 
{
	if ( HComponentItem == NULL ) { return FALSE; }	
	if ( pComponent == NULL ) { return FALSE; }		
	CString str;
	//---------------------------------------------------------------------//		
	int        i = 0;	
	DWORD_PTR    ItemData = 0;
	unsigned int ItemType = 0;
	unsigned int ItemIndex = 0;
	CAOIWindow *pWindow=NULL;	
	HTREEITEM hSubItem=NULL;	
	HTREEITEM hGroupItem = NULL;
	HTREEITEM hWindowItem = NULL;
	HTREEITEM hWndGroupItem = NULL;
	CString   ItemText;	

#ifdef _DEBUG
	CString ComName, GroupName, WndGroupName, WindowName;
#endif

#ifdef _DEBUG
	ComName = CTreeCtrl::GetItemText(HComponentItem);//Component Index;
#endif

	hGroupItem = CTreeCtrl::GetChildItem(HComponentItem);
	if ( hGroupItem == NULL ) { return FALSE; }	

	//Part Number	
#ifdef _DEBUG
	GroupName = CTreeCtrl::GetItemText(hGroupItem);
#endif
	ItemData = CTreeCtrl::GetItemData(hGroupItem);
	::DecodeItemlParam(ItemData, ItemType, ItemIndex);
	if ( TREE_ITEM_TYPE_COMPONENT_PART_NUMBER == ItemType )
	{	
		ItemText.Format(_T("%s"), pComponent->GetComponentPartNumber());
		CTreeCtrl::SetItemText(hGroupItem, ItemText); 
	}

	//Position
	hGroupItem = CTreeCtrl::GetNextSiblingItem(hGroupItem);
	if ( hGroupItem == NULL ) { return FALSE; }	
#ifdef _DEBUG
	GroupName = CTreeCtrl::GetItemText(hGroupItem);
#endif
	ItemData = CTreeCtrl::GetItemData(hGroupItem);
	::DecodeItemlParam(ItemData, ItemType, ItemIndex);
	if ( TREE_ITEM_TYPE_COMPONENT_POSITION == ItemType )	
	{		
		pComponent->GetComponentLocationText(ItemText);		
		CTreeCtrl::SetItemText(hGroupItem, ItemText);
	}

	//Offset
	hGroupItem = CTreeCtrl::GetNextSiblingItem(hGroupItem);
	if ( hGroupItem == NULL ) { return FALSE; }	
#ifdef _DEBUG
	GroupName = CTreeCtrl::GetItemText(hGroupItem);
#endif
	ItemData = CTreeCtrl::GetItemData(hGroupItem);
	::DecodeItemlParam(ItemData, ItemType, ItemIndex);
	if ( TREE_ITEM_TYPE_COMPONENT_OFFSET == ItemType )	
	{
		pComponent->GetComponentOffsetText(ItemText);		
		CTreeCtrl::SetItemText(hGroupItem, ItemText);
	}

	//Model
	/*
	hGroupItem = CTreeCtrl::GetNextSiblingItem(hGroupItem);
	if ( hGroupItem == NULL ) { return FALSE; }	
#ifdef _DEBUG
	GroupName = CTreeCtrl::GetItemText(hGroupItem);
#endif
	ItemData = CTreeCtrl::GetItemData(hGroupItem);
	if ( ItemData == INSPECT_COMPONENT_MODEL_GROUP_NODE )
	{
		ModelPtr = pComponent->GetComponentModelPtrActived();
		if ( ModelPtr != NULL )
		{
			ItemText.Format(_T("%s"), ModelPtr->GetModelName());
			CTreeCtrl::SetItemText(hGroupItem, ItemText); 
		}

		hWndGroupItem = CTreeCtrl::GetChildItem(hGroupItem);
		while ( hWndGroupItem != NULL )
		{
		#ifdef _DEBUG
			WndGroupName = CTreeCtrl::GetItemText(hWndGroupItem);
		#endif
			WndGroupID = (int)(CTreeCtrl::GetItemData(hWndGroupItem));
			if ( ModelPtr != NULL )
			{
				WndPtr = ModelPtr->GetModelWndPtrByGroupID(WndGroupID, -1, false);
				if ( WndPtr != NULL )
				{	
					this->FormatWndGroupItemText(WndPtr, ItemText);
					CTreeCtrl::SetItemText(hWndGroupItem, ItemText); 
				}
			}
			
			hWindowItem = CTreeCtrl::GetChildItem(hWndGroupItem);
			while ( hWindowItem!=NULL  )
			{
			#ifdef _DEBUG
				WindowName = CTreeCtrl::GetItemText(hWindowItem);
			#endif
				WindowIndex = (int)(CTreeCtrl::GetItemData(hWindowItem));
				pWindow = pComponent->GetWindowPtr_C(WindowIndex, true);
				if ( pWindow == NULL )
				{
					hWindowItem = CTreeCtrl::GetNextSiblingItem(hWindowItem);
					continue;
				}
				this->FormatWindowItemText(WindowIndex, pWindow, ItemText);
				CTreeCtrl::SetItemText(hSubItem, ItemText);

				hWindowItem = CTreeCtrl::GetNextSiblingItem(hWindowItem);
			};			
			hWndGroupItem = CTreeCtrl::GetNextSiblingItem(hWndGroupItem);
		};
	}*/
	return TRUE;	
}
//--------------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::DoDeleteComponentTreeWndItem()//刪除零件樹狀圖選取到零件的Item
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || NULL==this->GetSafeHwnd() ) { return FALSE; }
	
	HTREEITEM hPanelItem = CTreeCtrl::GetRootItem();
	if ( hPanelItem == NULL ) { return FALSE; }//沒有建立任何結點	

	HTREEITEM hBoardItem = NULL;	
	HTREEITEM hComponentItem=NULL;
	HTREEITEM hDeleteItem=NULL;	

	CAOIPanel *pPanel = NULL;
	CAOIBoard *pBoard = NULL;
	CAOIComponent *pComponent = NULL;
	
	CString str;
	DWORD_PTR    ItemData = 0;
	unsigned int ItemType = 0;
	unsigned int ItemIndex = 0;

	this->m_StopComponentTreeBeClick = TRUE;	
	while ( hPanelItem != NULL )
	{
		ItemData = CTreeCtrl::GetItemData(hPanelItem);
		::DecodeItemlParam(ItemData, ItemType, ItemIndex);
		pPanel = ProjectPtr->GetProjectPanelPtr(ItemIndex, true);		
		if ( TREE_ITEM_TYPE_PANEL!=ItemType || NULL==pPanel )//可能是Panel或者是FiducialPanelID = (int)CTreeCtrl::GetItemData(hPanelItem);
		{
			hPanelItem = CTreeCtrl::GetNextSiblingItem(hPanelItem);
			continue; 
		}		
	#ifdef _DEBUG
		str = CTreeCtrl::GetItemText(hPanelItem);
	#endif

		hBoardItem = CTreeCtrl::GetChildItem(hPanelItem);	
		while ( NULL != hBoardItem )
		{
			ItemData = CTreeCtrl::GetItemData(hBoardItem);
			::DecodeItemlParam(ItemData, ItemType, ItemIndex);
			pBoard = ProjectPtr->GetProjectBoardPtr(ItemIndex, true);
			if ( TREE_ITEM_TYPE_BOARD!=ItemType || pBoard==NULL ) //可能是Panel或者是Fiducial
			{
				hBoardItem = CTreeCtrl::GetNextSiblingItem(hBoardItem);
				continue; 
			}
		#ifdef _DEBUG
			str = CTreeCtrl::GetItemText(hBoardItem);
		#endif
			
			hComponentItem = CTreeCtrl::GetChildItem(hBoardItem);
			while ( NULL != hComponentItem )
			{	
				ItemData = CTreeCtrl::GetItemData(hComponentItem);
				::DecodeItemlParam(ItemData, ItemType, ItemIndex);
				pComponent = ProjectPtr->GetProjectComponentPtr(ItemIndex, true);
				if ( TREE_ITEM_TYPE_COMPONENT!=ItemType ||pComponent==NULL )		
				{
					hComponentItem = CTreeCtrl::GetNextSiblingItem(hComponentItem);
					continue;
				}
			#ifdef _DEBUG
				str = CTreeCtrl::GetItemText(hComponentItem);
			#endif
				if ( pComponent->GetComponentSelected() == false )
				{
					hComponentItem = CTreeCtrl::GetNextSiblingItem(hComponentItem);
					continue;
				}				
				hDeleteItem = hComponentItem;
				hComponentItem = CTreeCtrl::GetNextSiblingItem(hComponentItem);
				CTreeCtrl::DeleteItem(hDeleteItem);
			};
			hBoardItem = CTreeCtrl::GetNextSiblingItem(hBoardItem);
		};
		hPanelItem = CTreeCtrl::GetNextSiblingItem(hPanelItem);
	};
	this->m_StopComponentTreeBeClick = FALSE;
	return TRUE;
}
//--------------------------------------------------------------------------//
BOOL CTreeCtrlComponent::DoCollapseComponentTreeWnd()
{
	if ( CTreeCtrl::GetSafeHwnd() == NULL ) { return FALSE; }	

	CString   str, str1, str2, str3, str4, str5;	
	HTREEITEM hItem = CTreeCtrl::GetRootItem();
	HTREEITEM hSubItem1 = NULL;
	HTREEITEM hSubItem2 = NULL;
	HTREEITEM hSubItem3 = NULL;
	HTREEITEM hSubItem4 = NULL;
	HTREEITEM hSubItem5 = NULL;
	this->m_StopComponentTreeBeClick = TRUE;
	while ( hItem != NULL )
	{
	#ifdef _DEBUG
		str = CTreeCtrl::GetItemText(hItem);
	#endif
		hSubItem1 = CTreeCtrl::GetChildItem(hItem);
		while ( hSubItem1 != NULL )
		{
		#ifdef _DEBUG
			str1 = CTreeCtrl::GetItemText(hSubItem1);
		#endif
			hSubItem2 = CTreeCtrl::GetChildItem(hSubItem1);
			while ( hSubItem2 != NULL )
			{
			#ifdef _DEBUG
				str2 = CTreeCtrl::GetItemText(hSubItem2);
			#endif
				hSubItem3 = CTreeCtrl::GetChildItem(hSubItem2);
				while ( hSubItem3 != NULL )
				{	
				#ifdef _DEBUG
					str3 = CTreeCtrl::GetItemText(hSubItem3);
				#endif
					hSubItem4 = CTreeCtrl::GetChildItem(hSubItem3);
					while ( hSubItem4 != NULL )
					{
					#ifdef _DEBUG
						str4 = CTreeCtrl::GetItemText(hSubItem4);
					#endif						
						hSubItem5 = CTreeCtrl::GetChildItem(hSubItem4);
						while ( hSubItem5 != NULL )
						{
						#ifdef _DEBUG
							str5 = CTreeCtrl::GetItemText(hSubItem5);
						#endif
							CTreeCtrl::Expand(hSubItem5, TVE_COLLAPSE);
							hSubItem5 = CTreeCtrl::GetNextSiblingItem(hSubItem5);
						};
						CTreeCtrl::Expand(hSubItem4, TVE_COLLAPSE);
						hSubItem4 = CTreeCtrl::GetNextSiblingItem(hSubItem4);
					};
					CTreeCtrl::Expand(hSubItem3, TVE_COLLAPSE);
					hSubItem3 = CTreeCtrl::GetNextSiblingItem(hSubItem3);
				};
				CTreeCtrl::Expand(hSubItem2, TVE_COLLAPSE);
				hSubItem2 = CTreeCtrl::GetNextSiblingItem(hSubItem2);
			};
			CTreeCtrl::Expand(hSubItem1, TVE_COLLAPSE);
			hSubItem1 = CTreeCtrl::GetNextSiblingItem(hSubItem1);
		};
		CTreeCtrl::Expand(hItem, TVE_COLLAPSE);
		hItem = CTreeCtrl::GetNextSiblingItem(hItem);		
	};
	this->m_StopComponentTreeBeClick = FALSE;	
	return TRUE;
}
//--------------------------------------------------------------------------------//
UINT CTreeCtrlComponent::GetPopupMenuID(CPoint Point)
{
	UINT MenuID = 0;
	UINT uFlags = 0;
	HTREEITEM hItem = CTreeCtrl::HitTest(Point, &uFlags);
	if ( NULL == hItem ) { return MenuID; }

#ifdef _DEBUG
	CString ItemText = CTreeCtrl::GetItemText(hItem);
#endif
	
	unsigned int ItemType=0, ItemIndex=0;
	DWORD_PTR    ItemData = CTreeCtrl::GetItemData(hItem);
	::DecodeItemlParam(ItemData, ItemType, ItemIndex);	
	switch ( ItemType )
	{
	case TREE_ITEM_TYPE_PROJECT:
	case TREE_ITEM_TYPE_PROJECT_FILENAME:
	case TREE_ITEM_TYPE_PROJECT_REGION_W:
	case TREE_ITEM_TYPE_PROJECT_REGION_H:
	case TREE_ITEM_TYPE_PROJECT_FD_RANGE_W:
	case TREE_ITEM_TYPE_PROJECT_FD_RANGE_H:
	case TREE_ITEM_TYPE_PROJECT_BARCODE_MODE:	
		MenuID = IDR_MENU_TREE_PROJECT;
		break;
	case TREE_ITEM_TYPE_PANEL:
		MenuID = IDR_MENU_TREE_PANEL;
		break;		
	case TREE_ITEM_TYPE_FD:
		MenuID = IDR_MENU_TREE_FD;
		break;		
	case TREE_ITEM_TYPE_MARK:
		MenuID = IDR_MENU_TREE_MARK;
		break;
	case TREE_ITEM_TYPE_BOARD:
		MenuID = IDR_MENU_TREE_BOARD;
		break;		
	case TREE_ITEM_TYPE_COMPONENT:
	case TREE_ITEM_TYPE_COMPONENT_PACKAGE:
	case TREE_ITEM_TYPE_COMPONENT_PART_NUMBER:
	case TREE_ITEM_TYPE_COMPONENT_POSITION:
	case TREE_ITEM_TYPE_COMPONENT_OFFSET:
	case TREE_ITEM_TYPE_COMPONENT_MODE:
		MenuID = IDR_MENU_TREE_COMPONENT;
		break;
	case TREE_ITEM_TYPE_MODEL:
		//MenuID = IDR_MENU_TREE_COMPONENT;
		break;
	case TREE_ITEM_TYPE_WINDOW:
		//MenuID = IDR_MENU_TREE_COMPONENT;
		break;
	}

	return MenuID;
}
//--------------------------------------------------------------------------------//
int CTreeCtrlComponent::GetTreeNodeLevel(HTREEITEM hItem)//取得節點所存在的階層, 0為最上層, 1為第二層, 2為第三層
{
	//由目前往上找, 找到NULL 表示找完
	HTREEITEM hItemParent=CTreeCtrl::GetParentItem(hItem);
	int Level = 0;
	while ( hItemParent != NULL )
	{
		hItemParent = CTreeCtrl::GetParentItem(hItemParent);
		Level ++;
	};	
	return Level;
}
//--------------------------------------------------------------------------------//