// AOIProject.cpp: implementation of the CAOIProject class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIProject.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
inline size_t CAOIProject::GetProjectMarkCount_Inline() const//取得專案特徵點數量
{
	return m_ProjectMarkPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::AddProjectMarkPtr_Inline(CAOIMark *MarkPtr)//增加專案特徵點
{
	m_ProjectMarkPtrList.push_back(MarkPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIMark* CAOIProject::GetProjectMarkPtr_Inline(size_t index) const//取得專案特徵點指標	
{
	return m_ProjectMarkPtrList[index];
}
//-------------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectMarkCount() const//取得專案特徵點數量
{
	return GetProjectMarkCount_Inline();
}
//---------------------------------------------------------------------------------//
CAOIMark* CAOIProject::GetProjectMarkPtr(size_t index, bool check) const//取得專案特徵點指標
{
	if ( check )
	{
		const size_t count = GetProjectMarkCount_Inline();
		if ( index >= count )
		{	return NULL; }
	}
	return GetProjectMarkPtr_Inline(index);
}
//---------------------------------------------------------------------------------//
CAOIMark* CAOIProject::AddProjectMarkPtr(CAOIMark *MarkPtr, bool clone)//增加專案特徵點
{
	if ( NULL == MarkPtr )
	{
		this->m_ErrorString.Format(_T("Error, AddProjectMarkPtr Fault"));
		return NULL;
	}

	CAOIMark *NewMarkPtr = MarkPtr;
	unsigned int MarkIndex = (unsigned int)(GetProjectMarkCount_Inline());
	if ( true == clone )
	{
		CAOIPanel        *PanelPtr = MarkPtr->GetMarkPanelPtr();
		CAOIBoard        *BoardPtr = MarkPtr->GetMarkBoardPtr();

		NewMarkPtr = MarkPtr->CloneMarkObj();
		if ( NULL == NewMarkPtr ) { return NULL; }

		NewMarkPtr->SetMarkIndex_Project(MarkIndex);	
		if ( NULL != BoardPtr )
		{	BoardPtr->AddBoardMarkPtr(NewMarkPtr); }

		if ( NULL != PanelPtr )
		{	PanelPtr->AddPanelMarkPtr(NewMarkPtr); }
	}	
	else
	{	NewMarkPtr->SetMarkIndex_Project(MarkIndex); }

	int MarkUniqueID = NewMarkPtr->GetMarkUniqueID();
	if ( MarkUniqueID < 0 )
	{
		NewMarkPtr->SetMarkUniqueID(m_ProjectMarkMaxUniqueID);
		m_ProjectMarkMaxUniqueID ++;
	}
	else
	{
		if ( m_ProjectMarkMaxUniqueID <= MarkUniqueID )
		{	m_ProjectMarkMaxUniqueID = MarkUniqueID+1; }
	}
	NewMarkPtr->SetMarkProjectPtr(this);	
	AddProjectMarkPtr_Inline(NewMarkPtr);	
	return NewMarkPtr;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectMarkUseDrawRoughLine() const//確認專案特徵點使用粗糙線段
{
	const size_t MarkCount=GetProjectMarkCount_Inline();
	if ( MarkCount > 5000 ) { return true; }
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DestroyProjectMarkSelected()//摧毀選取到的專案特徵點
{
	size_t i = 0;
	int    index = 0;
	CAOIMark *MarkPtr = NULL;
	std::vector<CAOIMark*> ProjectMarkPtrList = m_ProjectMarkPtrList;
	
	index = 0;
	m_ProjectMarkPtrList.clear();
	const size_t MarkCount = ProjectMarkPtrList.size();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = ProjectMarkPtrList[i];
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkSelected() == true )
		{
			AOIObjManager.DestroyMarkObj(ProjectMarkPtrList[i]);			
			MarkPtr = NULL;
			continue;
		}

		MarkPtr->SetMarkIndex_Project(index);
		AddProjectMarkPtr_Inline(MarkPtr);
		index ++;
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ClearProjectAllMarks()//刪除專案特徵點
{
	size_t i = 0;
	CAOIMark *MarkPtr = NULL;
	const size_t MarkCount = GetProjectMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }		
		AOIObjManager.DestroyMarkObj(m_ProjectMarkPtrList[i]);			
		MarkPtr = NULL;		
	}		
	m_ProjectMarkPtrList.clear();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::LayoutProjectMarkList()//重整專案的特徵點列表	
{
	size_t i = 0;
	int    index = 0;
	CAOIMark *MarkPtr = NULL;
	std::vector<CAOIMark*> ProjectMarkPtrList = m_ProjectMarkPtrList;
	
	index = 0;
	m_ProjectMarkPtrList.clear();
	const size_t MarkCount = ProjectMarkPtrList.size();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = ProjectMarkPtrList[i];
		if ( NULL == MarkPtr ) { continue; }

		MarkPtr->SetMarkIndex_Project(index);
		AddProjectMarkPtr_Inline(MarkPtr);
		index ++;
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectAllMarks(bool value)//選取專案的所有特徵點
{
	size_t i = 0;	
	CAOIMark *MarkPtr = NULL;	
	const size_t MarkCount = GetProjectMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		MarkPtr->SetMarkSelected(value);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
CAOIMark* CAOIProject::GetProjectMarkPtrBySelected()//取得專案特徵點指標-依據選取到	
{
	size_t i = 0;	
	CAOIMark *MarkPtr = NULL;	
	const size_t MarkCount = GetProjectMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkDeleted() == true ) { continue; }
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }
		return MarkPtr;		
	}		
	return NULL;
}
//---------------------------------------------------------------------------------//
CAOIMark* CAOIProject::GetProjectMarkPtrByMarkUUID(const UUID &uuid)//取得專案特徵點指標-依據萬用字碼
{
	size_t    i = 0;	
	UUID      MarkUUID;
	CAOIMark *MarkPtr = NULL;	
	const size_t MarkCount = GetProjectMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkDeleted() == true ) { continue; }
		MarkUUID = MarkPtr->GetObjUuid();
		if ( MarkUUID != uuid ) { continue; }
		return MarkPtr;		
	}		
	return NULL;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RotateProjectMarkSelected(double Angle)//旋轉專案選取到特徵點
{
	size_t          i = 0;
	DISTRICT_ID     DistrictID;
	CAOIMark       *MarkPtr = NULL;	
	CAOIBoard      *BoardPtr = NULL;
	CAOIPanel      *PanelPtr = NULL;	
	CMapCoordinate *MapCTSPtr = NULL;	
	const size_t    MarkCount = GetProjectMarkCount_Inline();		
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkDeleted() == true ) { continue; }
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }
		MapCTSPtr = NULL;
		PanelPtr = MarkPtr->GetMarkPanelPtr();
		BoardPtr = MarkPtr->GetMarkBoardPtr();
		DistrictID = MarkPtr->GetMarkDistrictID();
		if ( NULL==MapCTSPtr && NULL!=BoardPtr ) 
		{	MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);	}
		if ( NULL==MapCTSPtr && NULL!=PanelPtr ) 
		{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);	}
		MarkPtr->SpinMark(Angle, MapCTSPtr);		
	}	
	
	const size_t BoardCount = GetProjectBoardCount();
	const size_t PanelCount = GetProjectPanelCount();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		PanelPtr->CalcPanelStagePosition();		
	}
	
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->CalcBoardStagePosition();		
	}
	LayoutProjectRegion();
	return true;	
}
//---------------------------------------------------------------------------------//
bool CAOIProject::GetProjectMarkSelected(std::vector<CAOIMark*> &SelMarkList)//取得專案特徵點選取到
{
	size_t i = 0;	
	CAOIMark *MarkPtr = NULL;	
	const size_t MarkCount = GetProjectMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkDeleted() == true ) { continue; }
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }
		SelMarkList.push_back(MarkPtr);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectMarkSelectedCount() const//計算專案特徵點選取到數量	
{
	size_t  i = 0;	
	size_t  Count=0;
	CAOIMark *MarkPtr = NULL;	
	const size_t MarkCount = GetProjectMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkDeleted() == true ) { continue; }
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }
		Count ++;
	}		
	return Count;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::BypassProjectMarkSelected()//不檢測選取到的專案特徵點
{
	size_t      i=0, j=0, k=0;
	bool         Bypass=false;
	CAOIMark    *MarkPtr = GetProjectActiveMark();	
	const size_t MarkCount = GetProjectMarkCount_Inline();
	if ( NULL != MarkPtr )
	{
		if ( MarkPtr->GetMarkBypassed() == true ) { Bypass = false; }
		else { Bypass = true; }
	}
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }
		MarkPtr->SetMarkBypassed(Bypass);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectMarkGroup()//刪除同群組的專案特徵點
{	
	int         MaxGroupID=0;	
	size_t      i=0, j=0, k=0;
	CAOIMark    *MarkPtr = NULL;
	CAOIPanel   *PanelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;	
	const size_t MarkCount = GetProjectMarkCount_Inline();
	const size_t PanelCount = GetProjectPanelCount();
	const size_t BoardCount = GetProjectBoardCount();
	
	ClearProjectAllTempObjects();//刪除所有暫時用的物件
	SelectProjectAllFds(false);
	SelectProjectAllPanels(false);
	SelectProjectAllBoards(false);
	SelectProjectAllBarcodes(false);
	SelectProjectAllComponents(false);
	MaxGroupID = 0;
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }
		MarkPtr->SetMarkSelected(true);

		if ( MaxGroupID < MarkPtr->GetMarkGroupID() )
		{	MaxGroupID = MarkPtr->GetMarkGroupID(); }		
	}

	int         GroupID=0;	
	const size_t GroupCount=MaxGroupID+1;	
	std::vector<int> SelGroupID(GroupCount);
	std::fill(SelGroupID.begin(), SelGroupID.end(), 0x00);
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }
		GroupID = MarkPtr->GetMarkGroupID();
		SelGroupID[GroupID] = FN_ENABLE;
	}

	for ( i=0; i<GroupCount; i++ )
	{		
		if ( FN_ENABLE != SelGroupID[i] ) { continue; }
		GroupID = (int)(i);
		for ( i=0; i<MarkCount; i++ )
		{
			MarkPtr = GetProjectMarkPtr_Inline(i);
			if ( NULL == MarkPtr ) { continue; }			
			if ( GroupID != MarkPtr->GetMarkGroupID() ) { continue; }
			MarkPtr->SetMarkSelected(true);
		}
	}
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		
		const size_t NBoards = PanelPtr->GetPanelBoardCount();
		for ( j=0; j<NBoards; j++ )
		{
			BoardPtr = PanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }

			MarkPtr = BoardPtr->GetBoardMarkPtrBySelected();			
			if ( NULL == MarkPtr ) { continue; }
			BoardPtr->RemoveBoardMarkSelected();
			BoardPtr->LayoutBoardRegion();
		}

		MarkPtr = PanelPtr->GetPanelMarkPtrBySelected();
		if ( NULL == MarkPtr ) { continue; }
		PanelPtr->RemovePanelMarkSelected();
		PanelPtr->LayoutPanelRegion();
	}	

	DestroyProjectObjectSelected();
	LayoutProjectAllObjectsIndex();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectMarkSelected()//刪除選取到的專案特徵點
{
	size_t      i=0, j=0, k=0;
	CAOIMark    *MarkPtr = NULL;
	CAOIPanel   *PanelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;		
	const size_t MarkCount = GetProjectMarkCount_Inline();
	const size_t PanelCount = GetProjectPanelCount();
	const size_t BoardCount = GetProjectBoardCount();
	
	ClearProjectAllTempObjects();//刪除所有暫時用的物件
	SelectProjectAllFds(false);
	SelectProjectAllPanels(false);
	SelectProjectAllBoards(false);
	SelectProjectAllBarcodes(false);
	SelectProjectAllComponents(false);
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }
		MarkPtr->SetMarkSelected(true);
	}

	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		
		const size_t NBoards = PanelPtr->GetPanelBoardCount();
		for ( j=0; j<NBoards; j++ )
		{
			BoardPtr = PanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }

			MarkPtr = BoardPtr->GetBoardMarkPtrBySelected();			
			if ( NULL == MarkPtr ) { continue; }
			BoardPtr->RemoveBoardMarkSelected();
			BoardPtr->LayoutBoardRegion();
		}

		MarkPtr = PanelPtr->GetPanelMarkPtrBySelected();
		if ( NULL == MarkPtr ) { continue; }
		PanelPtr->RemovePanelMarkSelected();
		PanelPtr->LayoutPanelRegion();
	}	

	DestroyProjectObjectSelected();
	LayoutProjectAllObjectsIndex();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectMarkUnselected()//刪除未選取到的專案特徵點
{
	size_t      i=0, j=0, k=0;		
	CAOIMark    *MarkPtr = NULL;
	CAOIPanel   *PanelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;		
	std::vector<CAOIMark*> SelMarkList;
	const size_t MarkCount = GetProjectMarkCount_Inline();
	const size_t PanelCount = GetProjectPanelCount();
	const size_t BoardCount = GetProjectBoardCount();		
	
	ClearProjectAllTempObjects();//刪除所有暫時用的物件
	SelectProjectAllFds(false);	
	SelectProjectAllPanels(false);
	SelectProjectAllBoards(false);	
	SelectProjectAllBarcodes(false);
	SelectProjectAllComponents(false);
	GetProjectMarkSelected(SelMarkList);
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkSelected() == true ) 
		{ 
			MarkPtr->SetMarkSelected(false);
			continue; 
		}
		MarkPtr->SetMarkSelected(true);
	}

	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		
		const size_t NBoards = PanelPtr->GetPanelBoardCount();
		for ( j=0; j<NBoards; j++ )
		{
			BoardPtr = PanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }

			MarkPtr = BoardPtr->GetBoardMarkPtrBySelected();			
			if ( NULL == MarkPtr ) { continue; }
			BoardPtr->RemoveBoardMarkSelected();
			BoardPtr->LayoutBoardRegion();
		}

		MarkPtr = PanelPtr->GetPanelMarkPtrBySelected();
		if ( NULL == MarkPtr ) { continue; }
		PanelPtr->RemovePanelMarkSelected();
		PanelPtr->LayoutPanelRegion();
	}	
	DestroyProjectObjectSelected();

	const size_t SelMarkCount = SelMarkList.size();
	for ( i=0; i<SelMarkCount; i++ )
	{
		MarkPtr = SelMarkList[i];
		if ( NULL == MarkPtr ) { continue; }
		MarkPtr->SetMarkSelected(true);
	}	

	LayoutProjectAllObjectsIndex();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectMarkGroupOthers()//刪除同群組其他的專案特徵點
{
	int         MaxGroupID=0;	
	size_t      i=0, j=0, k=0;
	CAOIMark    *MarkPtr = NULL;
	CAOIPanel   *PanelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;	
	const size_t MarkCount = GetProjectMarkCount_Inline();
	const size_t PanelCount = GetProjectPanelCount();
	const size_t BoardCount = GetProjectBoardCount();
	
	ClearProjectAllTempObjects();//刪除所有暫時用的物件
	SelectProjectAllFds(false);
	SelectProjectAllPanels(false);
	SelectProjectAllBoards(false);
	SelectProjectAllBarcodes(false);
	SelectProjectAllComponents(false);
	MaxGroupID = 0;
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }
		MarkPtr->SetMarkSelected(true);

		if ( MaxGroupID < MarkPtr->GetMarkGroupID() )
		{	MaxGroupID = MarkPtr->GetMarkGroupID(); }		
	}

	int         GroupID=0;	
	const size_t GroupCount=MaxGroupID+1;	
	std::vector<int> SelGroupID(GroupCount);
	std::fill(SelGroupID.begin(), SelGroupID.end(), 0x00);
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }
		GroupID = MarkPtr->GetMarkGroupID();
		SelGroupID[GroupID] = FN_ENABLE;
	}

	for ( i=0; i<GroupCount; i++ )
	{		
		if ( FN_ENABLE != SelGroupID[i] ) { continue; }
		GroupID = (int)(i);
		for ( i=0; i<MarkCount; i++ )
		{
			MarkPtr = GetProjectMarkPtr_Inline(i);
			if ( NULL == MarkPtr ) { continue; }			
			if ( GroupID != MarkPtr->GetMarkGroupID() ) { continue; }
			if ( MarkPtr->GetMarkSelected() == true ) 
			{
				MarkPtr->SetMarkSelected(false);
				continue;
			}
			MarkPtr->SetMarkSelected(true);
		}
	}
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		
		const size_t NBoards = PanelPtr->GetPanelBoardCount();
		for ( j=0; j<NBoards; j++ )
		{
			BoardPtr = PanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }

			MarkPtr = BoardPtr->GetBoardMarkPtrBySelected();			
			if ( NULL == MarkPtr ) { continue; }
			BoardPtr->RemoveBoardMarkSelected();
			BoardPtr->LayoutBoardRegion();
		}

		MarkPtr = PanelPtr->GetPanelMarkPtrBySelected();
		if ( NULL == MarkPtr ) { continue; }
		PanelPtr->RemovePanelMarkSelected();
		PanelPtr->LayoutPanelRegion();
	}	

	DestroyProjectObjectSelected();
	LayoutProjectAllObjectsIndex();
	return true;	
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectMarkValid(const CAOIMark *RefMarkPtr)//確認專案的特徵點指標有效
{
	if ( NULL == RefMarkPtr ) { return false; }
	size_t i = 0;	
	CAOIMark *MarkPtr = NULL;	
	const size_t MarkCount = GetProjectMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( RefMarkPtr == MarkPtr )
		{	return true; }
	}		
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectMarksByStage(const TREGION4D &Rgn, bool ResultMode, std::vector<CAOIMark*> &MarkList)//選取特徵點
{
	size_t       i = 0;	
	CAOIMark    *MarkPtr = NULL;	
	const bool   PickSelMode = JetAPI::CheckPickSelectMode(Rgn, 25);
	const size_t MarkCount = GetProjectMarkCount_Inline();

	MarkList.clear();
	if ( true == PickSelMode )
	{
		TPOINT2D Pt;
		Pt.x = Rgn.GetCpX();
		Pt.y = Rgn.GetCpY();
		for ( i=0; i<MarkCount; i++ )
		{
			MarkPtr = GetProjectMarkPtr_Inline(i);
			if ( NULL == MarkPtr ) { continue; }
			if ( MarkPtr->GetMarkDeleted() == true ) { continue; }			
			if ( MarkPtr->CheckMarkBePickByStage(Pt) == false ) { continue; }
			MarkList.push_back(MarkPtr);
		}
	}
	else
	{
		for ( i=0; i<MarkCount; i++ )
		{
			MarkPtr = GetProjectMarkPtr_Inline(i);
			if ( NULL == MarkPtr ) { continue; }
			if ( MarkPtr->GetMarkDeleted() == true ) { continue; }
			if ( MarkPtr->CheckMarkInRegionByStage(Rgn, true) == false ) { continue; }
			MarkList.push_back(MarkPtr);
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::PasteProjectMarkToOtherBoards(std::vector<CAOIMark*> &CloneMarkList)//將特徵點貼上其餘整板單板上	
{	
	CString        ComponentName;	
	CString        OrgComponentName;
	size_t         i=0, j=0, k=0;
	size_t         u=0, v=0, w=0;
	size_t         PanelBoardCount=0;
	size_t         BoardComponentCount=0;
	size_t         RefBoardComponentCount=0;
	double         CadPosX=0;
	double         CadPosY=0;	
	double         NewCadPosX=0;
	double         NewCadPosY=0;
	double         CadOffsetX=0;
	double         CadOffsetY=0;
	double         OrgCadPosX=0;
	double         OrgCadPosY=0;
	double         OrgOffsetX=0;
	double         OrgOffsetY=0;
	double         CadOffsetX2=0;
	double         CadOffsetY2=0;
	double         RefOrgCadPosX=0;
	double         RefOrgCadPosY=0;
	double         StagePosX=0;
	double         StagePosY=0;
	double         NewStagePosX=0;
	double         NewStagePosY=0;	
	double         NewStagePosX2=0;
	double         NewStagePosY2=0;	
	double         BoardAngle = 0;
	bool           BoardMirrorXAxis=false;
	bool           BoardMirrorYAxis=false;
	CAOIProject   *ProjectPtr = this;
	CAOIPanel     *PanelPtr = NULL;
	CAOIBoard     *BoardPtr = NULL;	
	CAOIPanel     *RefPanelPtr = NULL;
	CAOIBoard     *RefBoardPtr = NULL;
	CMapCoordinate *MapCTSPtr = NULL;
	CMapCoordinate *PanelMapCTSPtr = NULL;
	CAOIMark      *MarkPtr = NULL;	
	CAOIMark      *RefMarkPtr = NULL;	
	CAOIComponent *OrgComponentPtr = NULL;			
	CAOIComponent *RefOrgComponentPtr = NULL;	
	BOARD_ORIENTATION_MODE BoardOrientMode;
	BOARD_ORIENTATION_MODE RefBoardOrientMode;
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	const size_t   SelMarkCount = CloneMarkList.size();
	const size_t   PanelCount = GetProjectPanelCount();
	const size_t   BoardCount = GetProjectBoardCount();
	if ( 0 == SelMarkCount ) { return true; }

	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		PanelPtr->SetPanelModified(false);
	}
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->SetBoardModified(false);
	}

	for ( i=0; i<SelMarkCount; i++ )
	{
		RefMarkPtr = CloneMarkList[i];
		if ( NULL == RefMarkPtr ) { continue; }
		RefPanelPtr = RefMarkPtr->GetMarkPanelPtr();
		RefBoardPtr = RefMarkPtr->GetMarkBoardPtr();
		if ( NULL==RefPanelPtr || NULL==RefBoardPtr ) { continue; }

		PanelMapCTSPtr = RefPanelPtr->GetPanelMapCTSPtr(DistrictID);
		PanelBoardCount = RefPanelPtr->GetPanelBoardCount();		
		RefBoardOrientMode = RefBoardPtr->GetBoardOrientationMode();
		RefBoardComponentCount = RefBoardPtr->GetBoardComponentCount();
		for ( j=0; j<PanelBoardCount; j++ )
		{
			BoardPtr = RefPanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }
			if ( RefBoardPtr == BoardPtr ) { continue; }

			MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);
			if ( NULL == MapCTSPtr ) 
			{	MapCTSPtr = RefPanelPtr->GetPanelMapCTSPtr(DistrictID);	}
			if ( NULL == MapCTSPtr ) { continue; }

			BoardOrientMode = BoardPtr->GetBoardOrientationMode();
			BoardComponentCount = BoardPtr->GetBoardComponentCount();

			//尋找參考零件-作為原點
			OrgComponentPtr = NULL;
			RefOrgComponentPtr = NULL;
			for ( u=0; u<RefBoardComponentCount; u++ )
			{
				RefOrgComponentPtr = RefBoardPtr->GetBoardComponentPtr(u, false);
				if ( NULL == RefOrgComponentPtr ) { continue; }
				if ( RefOrgComponentPtr->GetComponentDeleted() == true ) { continue; }				
				OrgComponentName = RefOrgComponentPtr->GetComponentName();
				OrgComponentPtr = BoardPtr->GetBoardComponentPtrByName(OrgComponentName);
				if ( NULL != OrgComponentPtr )
				{	break; }
			}
			if ( RefBoardComponentCount == u ) { continue; }			
			MarkPtr = RefMarkPtr->CloneMarkObj();
			if ( NULL == MarkPtr ) { continue; }

			CadPosX = MarkPtr->GetMarkCadPosX();
			CadPosY = MarkPtr->GetMarkCadPosY();
			StagePosX = MarkPtr->GetMarkStagePosX();
			StagePosY = MarkPtr->GetMarkStagePosY();
			OrgCadPosX=OrgComponentPtr->GetComponentCadPosX();
			OrgCadPosY=OrgComponentPtr->GetComponentCadPosY();
			RefOrgCadPosX=RefOrgComponentPtr->GetComponentCadPosX();
			RefOrgCadPosY=RefOrgComponentPtr->GetComponentCadPosY();			
			CadOffsetX2 = CadOffsetX = CadPosX-RefOrgCadPosX;
			CadOffsetY2 = CadOffsetY = CadPosY-RefOrgCadPosY;

			ProjectPtr->AddProjectMarkPtr(MarkPtr, false);
			RefPanelPtr->AddPanelMarkPtr(MarkPtr);
			RefPanelPtr->SetPanelModified(true);
			BoardPtr->AddBoardMarkPtr(MarkPtr);			
			BoardPtr->SetBoardModified(true);
			if ( BoardOrientMode!=RefBoardOrientMode )
			{
				JetAPI::CalcBoardOrientationOffset(RefBoardOrientMode, BoardOrientMode, BoardAngle, BoardMirrorXAxis, BoardMirrorYAxis);
				MarkPtr->RotateMark(BoardAngle, CadPosX, CadPosY, NULL);
				if ( true == BoardMirrorXAxis )//對X軸鏡射, 變更Y值
				{	MarkPtr->MirrorYMark(CadPosY, NULL);	}
				if ( true == BoardMirrorYAxis )//對Y軸鏡射, 變更X值
				{	MarkPtr->MirrorXMark(CadPosX, NULL);	}

				JetAPI::RotatePos(BoardAngle, 0, 0, CadOffsetX2, CadOffsetY2);		
				if ( true == BoardMirrorXAxis )
				{	JetAPI::MirrorXAxisPos(0, CadOffsetY2);	}
				if ( true == BoardMirrorYAxis )
				{	JetAPI::MirrorYAxisPos(0, CadOffsetX2);	}	
			}
			NewCadPosX = OrgCadPosX+CadOffsetX2;
			NewCadPosY = OrgCadPosY+CadOffsetY2;
			MapCTSPtr->Map2D(NewCadPosX, NewCadPosY, NewStagePosX, NewStagePosY);
			//PanelMapCTSPtr->Map2D(NewCadPosX, NewCadPosY, NewStagePosX2, NewStagePosY2);
			MarkPtr->SetMarkCadPosX(NewCadPosX);
			MarkPtr->SetMarkCadPosY(NewCadPosY);			
			MarkPtr->SetMarkStagePosX(NewStagePosX);
			MarkPtr->SetMarkStagePosY(NewStagePosY);
			MarkPtr->CalcMarkCadCornerPos();
			MarkPtr->LayoutMarkStageCornerPos();
			MarkPtr->UpdateMarkParamToModel();
		}
	}

	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardModified() == false ) { continue; }		
		BoardPtr->LayoutBoardRegion();
		BoardPtr->SetBoardModified(true);
	}
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelModified() == false ) { continue; }
		PanelPtr->LayoutPanelRegion();
		PanelPtr->SetPanelModified(true);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectMarkNeedToCalculate(bool val)//設定專案特徵點需要去檢測	
{
	size_t       i = 0;	
	bool        bNeedToCalc = val;
	CAOIMark    *MarkPtr = NULL;	
	const size_t MarkCount = GetProjectMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkBypassed() == true ) 
		{	
			MarkPtr->SetMarkNeedToCalculate(false);	
			continue;
		}
		MarkPtr->SetMarkNeedToCalculate(bNeedToCalc);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RestoreProjectMarkNeedToCalculate()//還原專案特徵點需要去檢測	
{
	size_t       i = 0;	
	bool        bNeedToCalc = true;
	CAOIMark    *MarkPtr = NULL;	
	const size_t MarkCount = GetProjectMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		bNeedToCalc = MarkPtr->GetMarkNeedToCalculateBackup();
		MarkPtr->SetMarkNeedToCalculate(bNeedToCalc);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::BackupProjectMarkNeedToCalculate()//備份專案特徵點需要去檢測	
{
	size_t       i = 0;	
	bool        bNeedToCalc = true;
	CAOIMark    *MarkPtr = NULL;	
	const size_t MarkCount = GetProjectMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		bNeedToCalc = MarkPtr->GetMarkNeedToCalculate();
		MarkPtr->SetMarkNeedToCalculateBackup(bNeedToCalc);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::UpdateProjectMarkToOtherByGroupID(CAOIMark *RefMarkPtr)//更新專案特徵點至其他特徵點-依據群組編號
{
	if ( NULL == RefMarkPtr ) { return false; }
	if ( RefMarkPtr->CheckMarkGroupIDValid() == false ) { return true; }

	size_t     i=0;
	double     Angle=0;
	int        GroupID=0;
	int        AngleLabel=0;
	CString    ModelName;
	CString    RefModelName;
	CString    SrcModelName;
	CString    ModelFolder;
	CString    RefModelFolder;
	CString    SrcModelFolder;	
	CAOIMark    *MarkPtr = NULL;	
	CAOIModel    LibraryModel;
	CAOIModel   *ModelPtr = NULL;
	CAOIModel   *SrcModelPtr = NULL;		
	CAOIModel   *RefModelPtr = RefMarkPtr->GetMarkModelPtr();	
	const double RefAngle = RefMarkPtr->GetMarkAngle();	
	const int    RefAngleLabel = JetAPI::GetAngleLabel(RefAngle);
	const int    RefGroupID = RefMarkPtr->GetMarkGroupID();	
	const bool   RefBypassed = RefMarkPtr->GetMarkBypassed();
	const int    LocalBasePlaneID = RefMarkPtr->GetMarkLocalBasePlaneID();
	const size_t MarkCount = GetProjectMarkCount_Inline();		
	
	RefModelName = RefModelPtr->GetModelName();
	RefModelFolder = RefModelPtr->GetModelFolder();	
	switch ( RefAngleLabel )
	{	
	case 90:
		LibraryModel = *RefModelPtr;		
		LibraryModel.RotateModel(270, 0, 0);		
		break;
	case 180:
		LibraryModel = *RefModelPtr;		
		LibraryModel.RotateModel(180, 0, 0);		
		break;
	case 270:
		LibraryModel = *RefModelPtr;		
		LibraryModel.RotateModel(90, 0, 0);		
		break;
	default:
		LibraryModel = *RefModelPtr;		
		break;
	}	
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr == RefMarkPtr ) { continue; }
		if ( MarkPtr->CheckMarkGroupIDValid() == false ) { continue; }

		GroupID = MarkPtr->GetMarkGroupID();		
		if ( GroupID != RefGroupID ) { continue; }
		MarkPtr->SetMarkBypassed(RefBypassed);
		MarkPtr->SetMarkLocalBasePlaneID(LocalBasePlaneID);
		MarkPtr->UpdateMarkModelFromLibrary(&LibraryModel);		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectMarkSelectedSpaceBasePlaneParam(const TBasePlaneParam& Param)//設定專案特徵點空間基準面參數
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t        i = 0;		
	CAOIMark     *MarkPtr = NULL;	
	const size_t  MarkCount = GetProjectMarkCount_Inline();		
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkDeleted() == true ) { continue; }	
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }		
		MarkPtr->SetMarkSpaceBasePlaneParam(Param);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectMarkSelectedSpaceNoiseFilterParam(const TNoiseFilterParam& Param)//設定專案特徵點空間雜訊過濾參數
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t        i = 0;		
	CAOIMark     *MarkPtr = NULL;	
	const size_t  MarkCount = GetProjectMarkCount_Inline();		
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkDeleted() == true ) { continue; }	
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }		
		MarkPtr->SetMarkSpaceNoiseFilterParam(Param);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::EnableProjectMarkSelectedMaskFunc_Base(bool bEnable)//啟用專案特徵點遮罩函式
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t        i = 0;		
	CAOIMark     *MarkPtr = NULL;	
	const size_t  MarkCount = GetProjectMarkCount_Inline();		
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkDeleted() == true ) { continue; }	
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }		
		MarkPtr->SetMarkMaskEnable_Base(bEnable);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectMarkSelectedMaskColorIndex_Base(int ColorIndex)//設定專案特徵點遮罩畫面
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }
	CColorGroup    *ColorGroupPtr = ProjectPtr->GetProjectColorGroupPtr(ColorIndex, true);
	if ( NULL == ColorGroupPtr ) { return false; }

	size_t             i = 0;		
	CAOIMark          *MarkPtr = NULL;	
	const size_t       MarkCount = GetProjectMarkCount_Inline();	
	const unsigned int FrameIndex = ColorGroupPtr->GetColorGroupFrameIndex();
	const unsigned int FrameUniqueID = ColorGroupPtr->GetColorGroupFrameUniqueID();

	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkDeleted() == true ) { continue; }	
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }				
		MarkPtr->SetMarkMaskColorGroupLinkIndex(ColorIndex);
		MarkPtr->SetMarkMaskFrameIndex_Base(FrameIndex);
		MarkPtr->SetMarkMaskFrameUniqueID_Base(FrameUniqueID);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectMarkSelectedLocalBasePlaneID(int BasePlaneID)//設定專案特徵點局部基準面編號
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIMark     *MarkPtr = NULL;	
	const size_t  MarkCount = GetProjectMarkCount_Inline();		
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkDeleted() == true ) { continue; }	
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }		
		MarkPtr->SetMarkLocalBasePlaneID(BasePlaneID);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
