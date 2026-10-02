// AOIProject_Board.cpp: implementation of the CAOIProject class.
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
inline size_t CAOIProject::GetProjectBoardCount_Inline() const//取得專案單板數量
{
	return CAOIProject::m_ProjectBoardPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::AddProjectBoardPtr_Inline(CAOIBoard *BoardPtr)//增加專案單板
{
	CAOIProject::m_ProjectBoardPtrList.push_back(BoardPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIBoard* CAOIProject::GetProjectBoardPtr_Inline(size_t index) const//取得專案單板指標	
{	
	return m_ProjectBoardPtrList[index];
}
//-------------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectBoardCount() const//取得專案單板數量
{
	return GetProjectBoardCount_Inline();
}
//---------------------------------------------------------------------------------//
CAOIBoard* CAOIProject::GetProjectBoardPtr(size_t index, bool check) const//取得專案單板指標
{
	if ( check )
	{
		const size_t count = GetProjectBoardCount_Inline();
		if ( index >= count )
		{	return NULL; }
	}
	return GetProjectBoardPtr_Inline(index);	
}
//---------------------------------------------------------------------------------//
CAOIBoard* CAOIProject::AddProjectBoardPtr(CAOIBoard *BoardPtr, bool clone)//增加專案單板
{
	if ( NULL == BoardPtr )
	{
		this->m_ErrorString.Format(_T("Error, AddProjectBoardPtr Fault"));
		return NULL;
	}

	CAOIBoard *NewBoardPtr = BoardPtr;
	const int BoardIndex = (int)(GetProjectBoardCount_Inline());
	if ( true == clone )
	{
		CAOIPanel        *PanelPtr = BoardPtr->GetBoardPanelPtr();
		if ( NULL == PanelPtr ) 
		{ 
			this->m_ErrorString.Format(_T("Error, AddProjectBoardPtr Fault"));
			return NULL; 
		}
		NewBoardPtr = BoardPtr->CloneBoardObj();
		if ( NewBoardPtr == NULL ) { return NULL; }

		size_t            i=0, j=0, k=0;
		int               FdUniqueID=0;
		size_t            SubRgns = 0;
		size_t            BoardFds = 0;
		size_t            BoardSBs = 0;
		size_t            BoardComponents = 0;
		size_t            ComponentWindows = 0;
		size_t            OldIndex = 0;
		size_t            NewIndex = 0;		
		size_t            OldBoardIndexProject = 0;
		size_t            NewBoardIndexProject = 0;				
		CString           OldFolder;
		CString           NewFolder;
		CString           NewModelName;
		CString           FiducialFolder;
		CAOIRgn          *SubRgnPtr = NULL;
		CAOIFd           *OldFdPtr = NULL;
		CAOIFd           *NewFdPtr = NULL;
		CAOIMark         *OldMarkPtr = NULL;
		CAOIMark         *NewMarkPtr = NULL;
		CAOIModel        *OldModelPtr = NULL;
		CAOIModel        *NewModelPtr = NULL;
		CAOIBarcode      *OldBarcodePtr = NULL;
		CAOIBarcode      *NewBarcodePtr = NULL;
		CAOIComponent    *OldComponentPtr = NULL;
		CAOIComponent    *NewComponentPtr = NULL;				
		CAOIWindow       *NewWindowPtr = NULL;

		const int         BoardIndex_Panel = (int)(PanelPtr->GetPanelBoardCount());
		const size_t      FdCount = BoardPtr->GetBoardFdCount();
		const size_t      MarkCount = BoardPtr->GetBoardMarkCount();
		const size_t      BarcodeCount = BoardPtr->GetBoardBarcodeCount();
		const size_t      ComponentCount = BoardPtr->GetBoardComponentCount();		

		const size_t      ProjectFdCount = GetProjectFdCount();
		const size_t      ProjectMarkCount = GetProjectMarkCount();
		const size_t      ProjectBarcodeCount = GetProjectBarcodeCount();
		const size_t      ProjectComponentCount = GetProjectComponentCount();
		
		std::vector<CAOIComponent*> ComponentModelIsolatedList;
		std::vector<CAOIFd*> ProjectFdPtrMap=m_ProjectFdPtrList;
		std::vector<CAOIMark*> ProjectMarkPtrMap=m_ProjectMarkPtrList;
		std::vector<CAOIBarcode*> ProjectBarcodePtrMap=m_ProjectBarcodePtrList;
		std::vector<CAOIComponent*> ProjectComponentPtrMap=m_ProjectComponentPtrList;
		
		NewBoardPtr->RemoveBoardAllFds();
		NewBoardPtr->RemoveBoardAllMarks();
		NewBoardPtr->RemoveBoardAllBarcodes();
		NewBoardPtr->RemoveBoardAllComponents();
		NewBoardPtr->RemoveBoardAllFields();
		NewBoardPtr->SetBoardIndex_Project(BoardIndex);
		NewBoardPtr->SetBoardIndex_Panel(BoardIndex_Panel);
		NewBoardPtr->SetBoardResultID(RESULT_ID_NONE);
		NewBoardPtr->SetBoardResultID_AOI_LA(RESULT_ID_NONE);
		NewBoardPtr->SetBoardResultID_AOI_LB(RESULT_ID_NONE);
		NewBoardPtr->SetBoardResultID_Alarm(RESULT_ID_NONE);

		FiducialFolder = GetProjectFdFolder();
		for ( i=0; i<FdCount; i++ )		
		{
			OldFdPtr = BoardPtr->GetBoardFdPtr(i, false);
			if ( NULL == OldFdPtr ) { continue; }

			NewFdPtr = OldFdPtr->CloneFdObj();
			if ( NULL == NewFdPtr ) { continue; }
			NewFdPtr->SetFdUniqueID(-1);
			this->AddProjectFdPtr(NewFdPtr, false);

			OldIndex = OldFdPtr->GetFdIndex_Project();
			NewIndex = NewFdPtr->GetFdIndex_Project();
			ProjectFdPtrMap[OldIndex] = NewFdPtr;

			FdUniqueID = NewFdPtr->GetFdUniqueID();
			OldModelPtr = OldFdPtr->GetFdModelPtr();
			NewModelPtr = NewFdPtr->GetFdModelPtr();
			OldFolder = OldModelPtr->GetModelFolder();			
			NewFolder = AOIDataDefine.GetFdModelFolder(FiducialFolder, FdUniqueID);			
			JetAPI::CreateFolder(NewFolder);
			JetAPI::ExtractTopFolder(NewFolder, NewModelName);
			NewModelPtr->SetModelName(NewModelName);
			NewModelPtr->SetModelFolderModel(NewFolder);
			NewModelPtr->AssignModelFolder();
			if ( OldFolder.GetLength() > 0 )  
			{	JetAPI::CopyFolderAToFolderB(OldFolder, NewFolder, false, true, _T(""), -1, -1); }

			NewBoardPtr->AddBoardFdPtr(NewFdPtr);
			PanelPtr->AddPanelFdPtr(NewFdPtr);

			//清除Field指標
			NewFdPtr->SetFdFieldPtr(NULL);			
			SubRgns = NewFdPtr->GetFdSubRgnCount();
			for ( k=0; k<SubRgns; k++ )
			{
				SubRgnPtr = NewFdPtr->GetFdSubRgnPtr(k, false);
				if ( NULL == SubRgnPtr ) { continue; }
				SubRgnPtr->SetRgnFieldPtr(NULL);
			}
		}

		for ( i=0; i<MarkCount; i++ )		
		{
			OldMarkPtr = BoardPtr->GetBoardMarkPtr(i, false);
			if ( NULL == OldMarkPtr ) { continue; }

			NewMarkPtr = OldMarkPtr->CloneMarkObj();
			if ( NULL == NewMarkPtr ) { continue; }
			NewMarkPtr->SetMarkUniqueID(-1);
			AddProjectMarkPtr(NewMarkPtr, false);

			OldIndex = OldMarkPtr->GetMarkIndex_Project();
			NewIndex = NewMarkPtr->GetMarkIndex_Project();
			ProjectMarkPtrMap[OldIndex] = NewMarkPtr;
			
			NewBoardPtr->AddBoardMarkPtr(NewMarkPtr);
			PanelPtr->AddPanelMarkPtr(NewMarkPtr);

			//清除Field指標			
			NewMarkPtr->SetMarkFieldPtr(NULL);
			NewMarkPtr->InitMarkInspection();			
			SubRgns = NewMarkPtr->GetMarkSubRgnCount();
			for ( k=0; k<SubRgns; k++ )
			{
				SubRgnPtr = NewMarkPtr->GetMarkSubRgnPtr(k, false);
				if ( NULL == SubRgnPtr ) { continue; }
				SubRgnPtr->SetRgnFieldPtr(NULL);
			}			
		}

		for ( i=0; i<BarcodeCount; i++ )		
		{
			OldBarcodePtr = BoardPtr->GetBoardBarcodePtr(i, false);
			if ( NULL == OldBarcodePtr ) { continue; }

			NewBarcodePtr = OldBarcodePtr->CloneBarcodeObj();
			if ( NULL == NewBarcodePtr ) { continue; }
			NewBarcodePtr->SetBarcodeUniqueID(-1);
			this->AddProjectBarcodePtr(NewBarcodePtr, false);

			OldIndex = OldBarcodePtr->GetBarcodeIndex_Project();
			NewIndex = NewBarcodePtr->GetBarcodeIndex_Project();
			ProjectBarcodePtrMap[OldIndex] = NewBarcodePtr;
			
			NewBoardPtr->AddBoardBarcodePtr(NewBarcodePtr);
			PanelPtr->AddPanelBarcodePtr(NewBarcodePtr);

			//清除Field指標
			NewBarcodePtr->SetBarcodeFieldPtr(NULL);
			NewBarcodePtr->InitBarcodeInspection();			
			SubRgns = NewBarcodePtr->GetBarcodeSubRgnCount();
			for ( k=0; k<SubRgns; k++ )
			{
				SubRgnPtr = NewBarcodePtr->GetBarcodeSubRgnPtr(k, false);
				if ( NULL == SubRgnPtr ) { continue; }
				SubRgnPtr->SetRgnFieldPtr(NULL);
			}
		}

		for ( i=0; i<ComponentCount; i++ )		
		{
			OldComponentPtr = BoardPtr->GetBoardComponentPtr(i, false);
			if ( NULL == OldComponentPtr ) { continue; }

			NewComponentPtr = OldComponentPtr->CloneComponentObj();
			if ( NULL == NewComponentPtr ) { continue; }
			NewComponentPtr->SetComponentUniqueID(-1);
			this->AddProjectComponentPtr(NewComponentPtr, false);
			if ( NewComponentPtr->GetComponentModelIsolated() == true )
			{	ComponentModelIsolatedList.push_back(NewComponentPtr); }

			OldIndex = OldComponentPtr->GetComponentIndex_Project();
			NewIndex = NewComponentPtr->GetComponentIndex_Project();
			ProjectComponentPtrMap[OldIndex] = NewComponentPtr;
			
			NewBoardPtr->AddBoardComponentPtr(NewComponentPtr);
			PanelPtr->AddPanelComponentPtr(NewComponentPtr);

			//清除Field指標
			NewComponentPtr->SetComponentFieldPtr(NULL);
			NewComponentPtr->InitComponentInspection();
			NewComponentPtr->ResetComponentResultStatistic();	
			SubRgns = NewComponentPtr->GetComponentSubRgnCount();
			for ( k=0; k<SubRgns; k++ )
			{
				SubRgnPtr = NewComponentPtr->GetComponentSubRgnPtr(k, false);
				if ( NULL == SubRgnPtr ) { continue; }
				SubRgnPtr->SetRgnFieldPtr(NULL);
			}
			ComponentWindows = NewComponentPtr->GetComponentWindowCount();
			for ( j=0; j<ComponentWindows; j++ )
			{
				NewWindowPtr = NewComponentPtr->GetComponentWindowPtr(j, false);
				if ( NULL == NewWindowPtr ) { continue; }
				NewWindowPtr->SetWindowFieldPtr(NULL);
				SubRgns = NewWindowPtr->GetWindowSubRgnCount();
				for ( k=0; k<SubRgns; k++ )
				{
					SubRgnPtr = NewWindowPtr->GetWindowSubRgnPtr(k, false);
					if ( NULL == SubRgnPtr ) { continue; }
					SubRgnPtr->SetRgnFieldPtr(NULL);
				}
			}
		}

		for ( i=0; i<ComponentCount; i++ )		
		{
			OldComponentPtr = BoardPtr->GetBoardComponentPtr(i, false);
			if ( NULL == OldComponentPtr ) { continue; }
			if ( OldComponentPtr->CheckComponentIsMaster() == false ) { continue; }
			OldIndex = OldComponentPtr->GetComponentIndex_Project();
			NewComponentPtr = ProjectComponentPtrMap[OldIndex];
			if ( NULL == NewComponentPtr ) { continue; }
			if ( NewComponentPtr->CheckComponentIsMaster() == false ) { continue; }

			CAOIComponent *OldMasterPtr=OldComponentPtr;
			CAOIComponent *NewMasterPtr=NewComponentPtr;
			const size_t AgentCount=OldMasterPtr->GetComponentAgentCount();
			NewMasterPtr->ClearComponentAgentList();
			for ( j=0; j<AgentCount; j++ )
			{
				OldComponentPtr = OldMasterPtr->GetComponentAgentPtr(j, false);
				if ( NULL == OldComponentPtr ) { continue; }
				OldIndex = OldComponentPtr->GetComponentIndex_Project();
				NewComponentPtr = ProjectComponentPtrMap[OldIndex];
				if ( NULL == NewComponentPtr ) { continue; }
				NewMasterPtr->AddComponentAgentPtr(NewComponentPtr);
			}
		}
		PanelPtr->AddPanelBoardPtr(NewBoardPtr);

		CString FullComponentName;
		CString PartLibraryFolder = GetProjectPartLibraryFolder();
		const size_t ComponentModelIsolatedCount=ComponentModelIsolatedList.size();
		for ( i=0; i<ComponentModelIsolatedCount; i++ )
		{
			NewComponentPtr = ComponentModelIsolatedList[i];
			if ( NULL == NewComponentPtr ) { continue; }
			NewModelPtr = NewComponentPtr->GetComponentModelPtr();
			if ( NULL == NewModelPtr ) { continue; }
			FullComponentName = NewComponentPtr->GetComponentFullName();

			OldFolder = NewModelPtr->GetModelFolder();
			NewFolder.Format(_T("%s\\%s"), PartLibraryFolder, FullComponentName);
			if ( JetAPI::CopyFolderAToFolderB(OldFolder, NewFolder, false, false, _T(""), -1, -1) == true )
			{
				NewModelPtr->SetupkModelModifiedDateTime();
				NewModelPtr->SetModelNeedSaveFiles(true);
				NewModelPtr->SetModelBKImageNeedToGrab(true);
				NewModelPtr->SetModelFolderComponent(NewFolder);
				NewModelPtr->AssignModelFolder();
			}
		}
	}	
	else
	{	NewBoardPtr->SetBoardIndex_Project(BoardIndex); }	
	NewBoardPtr->SetBoardProjectPtr(this);	
	NewBoardPtr->SetBoardMultiDistrictMode(GetProjectMultiDistrictMode());
	AddProjectBoardPtr_Inline(NewBoardPtr);
	return NewBoardPtr;	
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DestroyProjectBoardSelected()//摧毀選取到的專案單板
{
	size_t i = 0;
	int    index = 0;
	CAOIBoard *BoardPtr = NULL;
	std::vector<CAOIBoard*> ProjectBoardPtrList = this->m_ProjectBoardPtrList;
	
	index = 0;
	this->m_ProjectBoardPtrList.clear();
	const size_t BoardCount = ProjectBoardPtrList.size();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = ProjectBoardPtrList[i];
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardSelected() == true )
		{
			AOIObjManager.DestroyBoardObj(ProjectBoardPtrList[i]);			
			BoardPtr = NULL;
			continue;
		}

		BoardPtr->SetBoardIndex_Project(index);
		CAOIProject::AddProjectBoardPtr_Inline(BoardPtr);
		index ++;
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ClearProjectAllBoards()//刪除專案單板
{
	size_t i = 0;	
	CAOIBoard *BoardPtr = NULL;
	const size_t BoardCount = GetProjectBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }		
		AOIObjManager.DestroyBoardObj(m_ProjectBoardPtrList[i]);			
		BoardPtr = NULL;		
	}	
	this->m_ProjectBoardPtrList.clear();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::LayoutProjectBoardList()//重整專案的單板列表
{
	size_t       i = 0;
	unsigned int index = 0;
	CAOIBoard *BoardPtr = NULL;
	std::vector<CAOIBoard*> ProjectBoardPtrList = this->m_ProjectBoardPtrList;
	
	index = 0;
	this->m_ProjectBoardPtrList.clear();
	const size_t BoardCount = ProjectBoardPtrList.size();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = ProjectBoardPtrList[i];
		if ( NULL == BoardPtr ) { continue; }

		BoardPtr->SetBoardIndex_Project(index);
		CAOIProject::AddProjectBoardPtr_Inline(BoardPtr);
		index ++;
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectAllBoards(bool value)//選取專案的所有單板
{
	size_t i = 0;	
	CAOIBoard *BoardPtr = NULL;	
	const size_t BoardCount = GetProjectBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->SetBoardSelected(value);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectBoardsByStage(const TREGION4D &Rgn, bool ResultMode, std::vector<CAOIBoard*> &BoardList)//選取單板
{
	size_t         i = 0;	
	CAOIBoard     *BoardPtr = NULL;
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	const bool     PickSelMode = JetAPI::CheckPickSelectMode(Rgn, 25);
	const size_t   BoardCount = GetProjectBoardCount_Inline();

	BoardList.clear();
	if ( true == PickSelMode )
	{
		TPOINT2D Pt;
		Pt.x = Rgn.GetCpX();
		Pt.y = Rgn.GetCpY();
		for ( i=0; i<BoardCount; i++ )
		{
			BoardPtr = GetProjectBoardPtr_Inline(i);
			if ( NULL == BoardPtr ) { continue; }
			if ( BoardPtr->GetBoardDeleted() == true ) { continue; }			
			if ( BoardPtr->CheckBoardBePickByStage(DistrictID, Pt) == false ) { continue; }
			BoardList.push_back(BoardPtr);
		}
	}
	else
	{
		for ( i=0; i<BoardCount; i++ )
		{
			BoardPtr = GetProjectBoardPtr_Inline(i);
			if ( NULL == BoardPtr ) { continue; }
			if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
			if ( BoardPtr->CheckBoardInRegionByStage(DistrictID, Rgn, true) == false ) { continue; }
			BoardList.push_back(BoardPtr);
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
CAOIBoard* CAOIProject::GetProjectBoardPtrByLastOne()//取得專案單板指標-依據最末個
{
	size_t i=0, j=0;			
	CAOIBoard *BoardPtr = NULL;	
	const size_t BoardCount = GetProjectBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(BoardCount-i-1);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }		
		return BoardPtr;
	}
	return NULL;	
}
//---------------------------------------------------------------------------------//
CAOIBoard* CAOIProject::GetProjectBoardPtrByFirstOne()//取得專案單板指標-依據最前個
{
	size_t i=0, j=0;			
	CAOIBoard *BoardPtr = NULL;	
	const size_t BoardCount = GetProjectBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }		
		return BoardPtr;
	}
	return NULL;	
}
//---------------------------------------------------------------------------------//
CAOIBoard* CAOIProject::GetProjectBoardPtrBySelected()//取得專案單板指標-依據選取到	
{
	size_t       i=0;			
	CAOIBoard   *BoardPtr = NULL;	
	const size_t BoardCount = GetProjectBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		if ( BoardPtr->GetBoardSelected() == false )
		{	continue; }
		return BoardPtr;
	}
	return NULL;
}
//---------------------------------------------------------------------------------//
CAOIBoard* CAOIProject::GetProjectBoardPtrByBoardUUID(const UUID &uuid)//取得專案單板指標-依據萬用字碼
{
	size_t       i=0;
	UUID         BoardUUID;
	CAOIBoard   *BoardPtr = NULL;	
	const size_t BoardCount = GetProjectBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		BoardUUID = BoardPtr->GetObjUuid();
		if ( BoardUUID != uuid ) { continue; }		
		return BoardPtr;
	}
	return NULL;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectBoardSelectedCount() const//取得專案單板選取到的數量
{
	size_t i=0;
	size_t Count=0;
	CAOIBoard *BoardPtr = NULL;	
	const size_t BoardCount = GetProjectBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		if ( BoardPtr->GetBoardSelected() == false )
		{	continue; }
		Count ++;
	}
	return Count;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::GetProjectBoardSelected(std::vector<CAOIBoard*> &SelBoardList)//取得專案單板選取到
{
	size_t i=0, j=0;			
	CAOIBoard *BoardPtr = NULL;	
	const size_t BoardCount = GetProjectBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		if ( BoardPtr->GetBoardSelected() == false )
		{	continue; }
		SelBoardList.push_back(BoardPtr);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RemoveProjectBoardSelected()//移除選取到的專案單板
{
	size_t i = 0;	
	CAOIBoard*  BoardPtr = NULL;
	std::vector<CAOIBoard*> BoardPtrList = m_ProjectBoardPtrList;
	const size_t BoardCount = BoardPtrList.size();

	m_ProjectBoardPtrList.clear();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = BoardPtrList[i];
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardSelected() == true ) { continue; }
		m_ProjectBoardPtrList.push_back(BoardPtr);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectBoardSelected()//刪除選取到的專案單板
{
	size_t i=0, j=0;		
	CAOIPanel *PanelPtr = NULL;
	CAOIBoard *BoardPtr = NULL;	
	const size_t PanelCount = GetProjectPanelCount();
	const size_t BoardCount = GetProjectBoardCount_Inline();
	
	ClearProjectAllTempObjects();//刪除所有暫時用的物件

	SelectProjectAllFds(false);
	SelectProjectAllMarks(false);
	SelectProjectAllBarcodes(false);
	SelectProjectAllPanels(false);	
	SelectProjectAllComponents(false);
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardSelected() == false )
		{	continue; }
		BoardPtr->SelectBoardAllObjects(true);
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
			if ( BoardPtr->GetBoardSelected() == true )
			{	break; }			
		}
		if ( j == NBoards ) { continue; }
		PanelPtr->RemovePanelObjectSelected();
	}	

	RemoveProjectPartGroupComponentSelected();
	RemoveProjectDefectComponentSelected();
	RemoveProjectDefectComponentSelected_LA();
	RemoveProjectDefectComponentSelected_LB();
	RemoveProjectDefectComponentSelected_All();
	DestroyProjectObjectSelected();
	LayoutProjectAllObjectsIndex();
	UpdateProjectComponentModelIsolatedFolder();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RotateProjectBoardSelected(double Angle)//選轉單板角度
{
	size_t      i=0;
	CAOIPanel   *PanelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;
	const size_t BoardCount = GetProjectBoardCount_Inline();
	const size_t PanelCount = GetProjectPanelCount();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		if ( BoardPtr->GetBoardSelected() == false ) { continue; }		
		BoardPtr->SpinBoard(Angle);
	}
	
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		PanelPtr->CalcPanelStagePosition();		
	}
	
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->CalcBoardStagePosition();		
	}
	LayoutProjectRegion();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::InvertSelectProjectBoard(bool bUpdate)//反向選取單板
{
	size_t      i=0;
	bool        Selected = false;
	CAOIBoard  *BoardPtr = NULL;
	const size_t BoardCount = GetProjectBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		if ( BoardPtr->GetBoardSelected() == true )
		{	Selected = false; }
		else 
		{	Selected = true; }
		BoardPtr->SetBoardSelected(Selected);		
		if ( true == bUpdate )
		{	BoardPtr->SelectBoardAllObjects(Selected);	}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::MoveProjectBoardSelected(double dX, double dY)//移動專案選取到的單板
{
	size_t       i=0;	
	CAOIBoard   *BoardPtr = NULL;	
	const size_t BoardCount = GetProjectBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		if ( BoardPtr->GetBoardSelected() == false ) { continue; }
		BoardPtr->MoveBoardPos(dX, dY);		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::MirrorXProjectBoardSelected()//鏡射專案選取到單板的X座標
{
	size_t       i=0;	
	double       CadCpX=0;	
	TREGION4D    RgnCad;	
	CAOIBoard   *BoardPtr = NULL;	
	const size_t BoardCount = GetProjectBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		if ( BoardPtr->GetBoardSelected() == false ) { continue; }

		RgnCad = BoardPtr->GetBoardRgnCad();		
		CadCpX = (RgnCad.minX+RgnCad.maxX)*0.5;		
		BoardPtr->MirrorXBoard(CadCpX);		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::MirrorYProjectBoardSelected()//鏡射專案選取到單板的Y座標
{
	size_t       i=0;	
	double       CadCpY=0;
	double       StageCpY=0;
	TREGION4D    RgnCad;
	TREGION4D    RgnStage;
	CAOIBoard   *BoardPtr = NULL;	
	const size_t BoardCount = GetProjectBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		if ( BoardPtr->GetBoardSelected() == false ) { continue; }

		RgnCad = BoardPtr->GetBoardRgnCad();		
		CadCpY = (RgnCad.minY+RgnCad.maxY)*0.5;		
		BoardPtr->MirrorYBoard(CadCpY);		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SwitchProjectBoardBypassed()//切換專案單板不檢測模式
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return true; }
	std::vector<CAOIBoard*> SelBoardList;	

	ProjectPtr->GetProjectBoardSelected(SelBoardList);
	const size_t SelCount =  SelBoardList.size();	
	if ( 0 == SelCount ) { return true; }	

	size_t i=0;
	CAOIBoard  *BoardPtr = NULL;	
	BoardPtr =  SelBoardList[0];
	if ( NULL == BoardPtr ) { return true; }
	bool Bypass = BoardPtr->GetBoardBypassed();
	if ( true == Bypass ) { Bypass = false; }
	else { Bypass = true; }

	for ( i=0; i<SelCount; i++ )
	{
		BoardPtr = (SelBoardList[i]);
		if ( NULL == BoardPtr ) { continue; }		
		BoardPtr->SetBoardBypassed(Bypass);
		BoardPtr->UpdateBoardBypassed();
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ChangeProjectBoardSelectedPanel(CAOIPanel *RefPanelPtr)//切換專案選取單板的整板指標
{
	CAOIProject *ProjectPtr = this;	
	if ( NULL == ProjectPtr ) { return false; }
	if ( NULL == RefPanelPtr ) { return false; }

	size_t         i=0;	
	CAOIPanel     *PanelPtr=NULL;
	CAOIBoard     *BoardPtr=NULL;	
	std::vector<CAOIBoard*> SelBoardList;	
	ProjectPtr->GetProjectBoardSelected(SelBoardList);
	const size_t SelBoardCount = SelBoardList.size();

	for ( i=0; i<SelBoardCount; i++ )
	{
		BoardPtr = SelBoardList[i];
		if ( NULL == BoardPtr ) { continue; }		
		PanelPtr = BoardPtr->GetBoardPanelPtr();
		if ( PanelPtr == RefPanelPtr ) { continue; }
		BoardPtr->ChangeBoardPanel(RefPanelPtr);		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectBoardValid(const CAOIBoard *RefBoardPtr)//確認專案的單板指標有效
{
	if ( NULL == RefBoardPtr ) { return false; }

	size_t       i=0;	
	CAOIBoard   *BoardPtr = NULL;
	const size_t BoardCount = GetProjectBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		if ( RefBoardPtr == BoardPtr )
		{	return true; }
	}
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::PasteProjectBoardList(std::vector<CAOIBoard*> &CloneBoardList, double StageOffsetX, double StageOffsetY)//貼上專案單板
{
	size_t         i=0, j=0;
	size_t         FdCount = 0;	
	size_t         BarcodeCount = 0;	
	size_t         ComponentCount = 0;			
	TPOINT2D       CadPosOld;
	TPOINT2D       CadPosNew;
	TPOINT2D       StagePosOld;	
	TPOINT2D       StagePosNew;
	TPOINT2D       CadOffset;
	TPOINT2D       StageOffset(StageOffsetX, StageOffsetY);
	CAOIFd        *NewFdPtr = NULL;		
	CAOIPanel     *PanelPtr = NULL;
	CAOIBoard     *pBoard = NULL;		
	CAOIBoard     *NewBoardPtr = NULL;		
	CAOIBarcode   *NewBarcodePtr = NULL;
	CAOIComponent *NewComponentPtr = NULL;
	CMapCoordinate *MapCTSPtr = NULL;		
	CMapCoordinate *MapSTCPtr = NULL;				
	DISTRICT_ID     DistrictID = GetProjectActDistrictID();
	const size_t    CloneCount = CloneBoardList.size();
	if ( 0 == CloneCount ) { return true; }

	//this->SelectProjectAllBoards(false);
	for ( i=0; i<CloneCount; i++ )
	{
		pBoard = CloneBoardList[i];
		if ( NULL == pBoard ) { continue; }		
		pBoard->SetBoardSelected(false);
		pBoard->SelectBoardAllObjects(false);
		PanelPtr = pBoard->GetBoardPanelPtr();		
		if ( NULL == PanelPtr ) { continue; }
		CadPosOld.x = pBoard->GetBoardRgnCad().GetCpX();
		CadPosOld.y = pBoard->GetBoardRgnCad().GetCpY();
		MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);
		MapSTCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID);		
		StagePosOld.x = pBoard->GetBoardRgnStage(DistrictID).GetCpX();
		StagePosOld.y = pBoard->GetBoardRgnStage(DistrictID).GetCpY();		
		NewBoardPtr = AddProjectBoardPtr(pBoard, true);
		if ( NULL == NewBoardPtr ) { continue; }

		if ( NULL != MapSTCPtr )
		{
			StagePosNew.x = StagePosOld.x+StageOffset.x;
			StagePosNew.y = StagePosOld.y+StageOffset.y;
			MapSTCPtr->Map2D(StagePosNew.x, StagePosNew.y, CadPosNew.x, CadPosNew.y);	
			CadOffset.x = CadPosNew.x-CadPosOld.x;
			CadOffset.y = CadPosNew.y-CadPosOld.y;
		}
		else
		{	AOIDataCollect.MapStageOffsetPtToCad(StageOffset, CadOffset);	}
		NewBoardPtr->SetBoardSelected(true);
		NewBoardPtr->SelectBoardAllObjects(true);
		NewBoardPtr->MoveBoardPos(CadOffset.x, CadOffset.y);
		NewBoardPtr->CalcBoardMapParam(DistrictID);
		NewBoardPtr->LayoutBoardRegion();
		PanelPtr->SetPanelModified(true);		
	}

	const size_t PanelCount = this->GetProjectPanelCount();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelModified() == false ) { continue; }
		PanelPtr->LayoutPanelRegion();
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ArrayPasteProjectBoardList(std::vector<CAOIBoard*> &CloneBoardList, const std::vector<TPOINT2D> &PosList)//陣列貼上專案單板
{
	size_t     i=0, j=0, k=0;
	size_t     FdCount = 0;	
	size_t     BarcodeCount = 0;	
	size_t     ComponentCount = 0;		
	double     OffsetX=0, OffsetY=0;
	CAOIFd    *FdPtr = NULL;
	CAOIFd    *NewFdPtr = NULL;
	CAOIBarcode    *BarcodePtr = NULL;
	CAOIBarcode    *NewBarcodePtr = NULL;
	CAOIPanel *PanelPtr = NULL;
	CAOIBoard *pBoard = NULL;		
	CAOIBoard *NewBoardPtr = NULL;		
	CMapCoordinate *PanelMapCTSPtr=NULL;
	CMapCoordinate *PanelMapSTCPtr=NULL;
	CAOIComponent *ComponentPtr = NULL;
	CAOIComponent *NewComponentPtr = NULL;
	RESULT_ID ResultID = RESULT_ID_NONE;
	DISTRICT_ID  DistrictID = GetProjectActDistrictID();
	const size_t PosCount = PosList.size();
	const size_t CloneCount = CloneBoardList.size();
	if ( 0 == CloneCount ) { return true; }

	//this->SelectProjectAllBoards(false);
	for ( k=0; k<PosCount; k++ )
	{
		OffsetX = PosList[k].x;
		OffsetY = PosList[k].y;
		for ( i=0; i<CloneCount; i++ )
		{
			pBoard = CloneBoardList[i];
			if ( NULL == pBoard ) { continue; }		
			PanelPtr = pBoard->GetBoardPanelPtr();		
			if ( NULL == PanelPtr ) { continue; }
			pBoard->SetBoardSelected(false);
			pBoard->SelectBoardAllObjects(false);

			NewBoardPtr = this->AddProjectBoardPtr(pBoard, true);
			if ( NULL == NewBoardPtr ) { continue; }
			PanelMapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);
			PanelMapSTCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID);
			NewBoardPtr->SetBoardMapCTS(DistrictID, PanelMapCTSPtr);
			NewBoardPtr->SetBoardMapSTC(DistrictID, PanelMapSTCPtr);
			NewBoardPtr->SetBoardSelected(true);
			NewBoardPtr->SelectBoardAllObjects(true);
			NewBoardPtr->MoveBoardPos(OffsetX, OffsetY);
			NewBoardPtr->CalcBoardMapParam(DistrictID);

			ComponentCount = NewBoardPtr->GetBoardComponentCount();
			for ( j=0; j<ComponentCount; j++ )
			{
				NewComponentPtr = NewBoardPtr->GetBoardComponentPtr(j, false);
				if ( NULL == NewComponentPtr ) { continue; }
				NewComponentPtr->SetComponentResultID_AOI(ResultID);
				NewComponentPtr->SetComponentResultID_AOI_LA(ResultID);
				NewComponentPtr->SetComponentResultID_AOI_LB(ResultID);
				NewComponentPtr->SetComponentResultID_ARS(ResultID);
				NewComponentPtr->SetComponentResultID_ARS_LA(ResultID);
				NewComponentPtr->SetComponentResultID_ARS_LB(ResultID);
				NewComponentPtr->SetComponentResultID_Alarm(ResultID);				
				NewComponentPtr->GetComponentModelPtr()->InitModelInspection();
			}		
			NewBoardPtr->LayoutBoardRegion();
			PanelPtr->SetPanelModified(true);		
		}
	}

	const size_t PanelCount = this->GetProjectPanelCount();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelModified() == false ) { continue; }
		PanelPtr->LayoutPanelRegion();
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectBoardSelectedBarcodeCodeIndex(int BarcodeCodeIndex)//設定專案單板條碼序號
{
	size_t       i=0;	
	CAOIBoard   *BoardPtr = NULL;
	const size_t BoardCount = GetProjectBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		if ( BoardPtr->GetBoardSelected() == false ) { continue; }
		BoardPtr->SetBoardBarcodeDeviceCodeIndex(BarcodeCodeIndex);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectBoardSelectedBarcodeDeviceIndex(int BarcodeDeviceIndex)//設定專案單板條碼機序號
{
	size_t       i=0;	
	CAOIBoard   *BoardPtr = NULL;
	const size_t BoardCount = GetProjectBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		if ( BoardPtr->GetBoardSelected() == false ) { continue; }
		BoardPtr->SetBoardBarcodeDeviceIndex(BarcodeDeviceIndex);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectBoardAllObjectInOneField() const
{
	size_t       i=0;	
	std::vector<size_t> BadBoardIdList;
	CAOIBoard   *BoardPtr = NULL;
	DISTRICT_ID  DistrictID = GetProjectActDistrictID();
	const size_t BoardCount = GetProjectBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }		
		if ( BoardPtr->CheckBoardAllObjectInOneField(DistrictID) == true ) { continue; }
		BadBoardIdList.push_back(i);
	}
	if ( BadBoardIdList.size() > 0 )
	{	return false; }
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ResetProjectBoardPanelFieldParam()//清除專案單板整板區域參數
{	
	size_t       i=0;
	CAOIBoard   *BoardPtr = NULL;	
	DISTRICT_ID  DistrictID = GetProjectActDistrictID();
	const size_t BoardCount = GetProjectBoardCount_Inline();	
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->ResetBoardPanelFieldParam(DistrictID);		
	}	
	return true;
}
//---------------------------------------------------------------------------------//
