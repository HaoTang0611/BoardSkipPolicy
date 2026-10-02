// AOIProject_Panel.cpp: implementation of the CAOIProject class.
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
inline size_t CAOIProject::GetProjectPanelCount_Inline() const//取得專案整板數量	
{
	return CAOIProject::m_ProjectPanelPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void  CAOIProject::AddProjectPanelPtr_Inline(CAOIPanel *PanelPtr)//增加專案整板	
{
	CAOIProject::m_ProjectPanelPtrList.push_back(PanelPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIPanel* CAOIProject::GetProjectPanelPtr_Inline(size_t index) const//取得專案整板指標	
{	
	return m_ProjectPanelPtrList[index];
}
//-------------------------------------------------------------------------------------//
size_t  CAOIProject::GetProjectPanelCount() const
{
	return GetProjectPanelCount_Inline();	
}
//-------------------------------------------------------------------------------------//
CAOIPanel* CAOIProject::GetProjectPanelPtr(size_t index, bool check) const
{
	if ( check )
	{
		const size_t count = GetProjectPanelCount_Inline();
		if ( index >= count )
		{	return NULL; }
	}
	return GetProjectPanelPtr_Inline(index);	
}
//-------------------------------------------------------------------------------------//
CAOIPanel* CAOIProject::AddProjectPanelPtr(CAOIPanel *PanelPtr, bool clone)
{
	if ( NULL == PanelPtr )
	{
		this->m_ErrorString.Format(_T("Error, AddProjectPanelPtr Fault"));
		return NULL;
	}

	CAOIPanel *NewPanelPtr = PanelPtr;	
	const int PanelIndex = (int)(GetProjectPanelCount_Inline());
	if ( true == clone )
	{
		NewPanelPtr = NewPanelPtr->ClonePanelObj();
		if ( NewPanelPtr == NULL ) { return NULL; }

		size_t            i=0, j=0, k=0;		
		size_t            SubRgns = 0;
		size_t            BoardFds = 0;
		size_t            BoardSBs = 0;
		size_t            BoardMarks = 0;
		size_t            BoardComponents = 0;
		size_t            ComponentWindows = 0;
		int               OldIndex = 0;
		int               NewIndex = 0;
		int               FdUniqueID = 0;		
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
		CAOIBoard        *OldBoardPtr = NULL;
		CAOIBoard        *NewBoardPtr = NULL;
		CAOIWindow       *NewWindowPtr = NULL;
		CAOIComponent    *OldComponentPtr = NULL;
		CAOIComponent    *NewComponentPtr = NULL;		

		const size_t      FdCount = PanelPtr->GetPanelFdCount();
		const size_t      MarkCount = PanelPtr->GetPanelMarkCount();
		const size_t      BoardCount = PanelPtr->GetPanelBoardCount();
		const size_t      BarcodeCount = PanelPtr->GetPanelBarcodeCount();
		const size_t      ComponentCount = PanelPtr->GetPanelComponentCount();

		const size_t      ProjectFdCount = GetProjectFdCount();
		const size_t      ProjectMarkCount = GetProjectMarkCount();
		const size_t      ProjectBarcodeCount = GetProjectBarcodeCount();
		const size_t      ProjectBoardCount = GetProjectBoardCount();
		const size_t      ProjectComponentCount = GetProjectComponentCount();

		std::vector<CAOIComponent*> ComponentModelIsolatedList;
		std::vector<CAOIFd*>        ProjectFdPtrMap=m_ProjectFdPtrList;
		std::vector<CAOIMark*>      ProjectMarkPtrMap=m_ProjectMarkPtrList;		
		std::vector<CAOIBoard*>     ProjectBoardPtrMap=m_ProjectBoardPtrList;
		std::vector<CAOIBarcode*>   ProjectBarcodePtrMap=m_ProjectBarcodePtrList;
		std::vector<CAOIComponent*> ProjectComponentPtrMap=m_ProjectComponentPtrList;		

		NewPanelPtr->RemovePanelAllFds();
		NewPanelPtr->RemovePanelAllMarks();
		NewPanelPtr->RemovePanelAllBarcodes();
		NewPanelPtr->RemovePanelAllBoards();
		NewPanelPtr->RemovePanelAllComponents();
		NewPanelPtr->RemovePanelAllFields();
		NewPanelPtr->SetPanelIndex_Project(PanelIndex);
		NewPanelPtr->SetPanelResultID(RESULT_ID_NONE);
		NewPanelPtr->SetPanelResultID_AOI_LA(RESULT_ID_NONE);
		NewPanelPtr->SetPanelResultID_AOI_LB(RESULT_ID_NONE);		
		NewPanelPtr->SetPanelResultID_Alarm(RESULT_ID_NONE);				

		FiducialFolder = GetProjectFdFolder();
		for ( i=0; i<FdCount; i++ )		
		{
			OldFdPtr = PanelPtr->GetPanelFdPtr(i, false);
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
			
			NewPanelPtr->AddPanelFdPtr(NewFdPtr);

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
			OldMarkPtr = PanelPtr->GetPanelMarkPtr(i, false);
			if ( NULL == OldMarkPtr ) { continue; }

			NewMarkPtr = OldMarkPtr->CloneMarkObj();
			if ( NULL == NewMarkPtr ) { continue; }
			NewMarkPtr->SetMarkUniqueID(-1);
			AddProjectMarkPtr(NewMarkPtr, false);

			OldIndex = OldMarkPtr->GetMarkIndex_Project();
			NewIndex = NewMarkPtr->GetMarkIndex_Project();
			ProjectMarkPtrMap[OldIndex] = NewMarkPtr;
			NewPanelPtr->AddPanelMarkPtr(NewMarkPtr);

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
			OldBarcodePtr = PanelPtr->GetPanelBarcodePtr(i, false);
			if ( NULL == OldBarcodePtr ) { continue; }

			NewBarcodePtr = OldBarcodePtr->CloneBarcodeObj();
			if ( NULL == NewBarcodePtr ) { continue; }
			NewBarcodePtr->SetBarcodeUniqueID(-1);
			this->AddProjectBarcodePtr(NewBarcodePtr, false);

			OldIndex = OldBarcodePtr->GetBarcodeIndex_Project();
			NewIndex = NewBarcodePtr->GetBarcodeIndex_Project();
			ProjectBarcodePtrMap[OldIndex] = NewBarcodePtr;
			NewPanelPtr->AddPanelBarcodePtr(NewBarcodePtr);

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
			OldComponentPtr = PanelPtr->GetPanelComponentPtr(i, false);
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
			NewPanelPtr->AddPanelComponentPtr(NewComponentPtr);			

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
			OldComponentPtr = PanelPtr->GetPanelComponentPtr(i, false);
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

		for ( i=0; i<BoardCount; i++ )		
		{
			OldBoardPtr = PanelPtr->GetPanelBoardPtr(i, false);
			if ( NULL == OldBoardPtr ) { continue; }

			NewBoardPtr = OldBoardPtr->CloneBoardObj();
			if ( NULL == NewBoardPtr ) { continue; }
			this->AddProjectBoardPtr(NewBoardPtr, false);

			OldIndex = OldBoardPtr->GetBoardIndex_Project();
			NewIndex = NewBoardPtr->GetBoardIndex_Project();
			ProjectBoardPtrMap[OldIndex] = NewBoardPtr;
			
			NewBoardPtr->RemoveBoardAllFds();
			NewBoardPtr->RemoveBoardAllMarks();
			NewBoardPtr->RemoveBoardAllBarcodes();
			NewBoardPtr->RemoveBoardAllComponents();
			NewBoardPtr->RemoveBoardAllFields();
			NewPanelPtr->AddPanelBoardPtr(NewBoardPtr);

			NewBoardPtr->SetBoardResultID(RESULT_ID_NONE);
			NewBoardPtr->SetBoardResultID_AOI_LA(RESULT_ID_NONE);
			NewBoardPtr->SetBoardResultID_AOI_LB(RESULT_ID_NONE);
			NewBoardPtr->SetBoardResultID_Alarm(RESULT_ID_NONE);

			BoardFds = OldBoardPtr->GetBoardFdCount();
			for ( j=0; j<BoardFds; j++ )
			{
				OldFdPtr = OldBoardPtr->GetBoardFdPtr(j, false);
				if ( NULL == OldFdPtr ) { continue; }
				
				OldIndex = OldFdPtr->GetFdIndex_Project();
				NewFdPtr = ProjectFdPtrMap[OldIndex];
				NewBoardPtr->AddBoardFdPtr(NewFdPtr);
			}

			BoardMarks = OldBoardPtr->GetBoardMarkCount();
			for ( j=0; j<BoardMarks; j++ )
			{
				OldMarkPtr = OldBoardPtr->GetBoardMarkPtr(j, false);
				if ( NULL == OldMarkPtr ) { continue; }
				
				OldIndex = OldMarkPtr->GetMarkIndex_Project();
				NewMarkPtr = ProjectMarkPtrMap[OldIndex];
				NewBoardPtr->AddBoardMarkPtr(NewMarkPtr);
			}

			BoardSBs = OldBoardPtr->GetBoardBarcodeCount();
			for ( j=0; j<BoardSBs; j++ )
			{
				OldBarcodePtr = OldBoardPtr->GetBoardBarcodePtr(j, false);
				if ( NULL == OldBarcodePtr ) { continue; }
				
				OldIndex = OldBarcodePtr->GetBarcodeIndex_Project();
				NewBarcodePtr = ProjectBarcodePtrMap[OldIndex];
				NewBoardPtr->AddBoardBarcodePtr(NewBarcodePtr);
			}

			BoardComponents = OldBoardPtr->GetBoardComponentCount();
			for ( j=0; j<BoardComponents; j++ )
			{
				OldComponentPtr = OldBoardPtr->GetBoardComponentPtr(j, false);
				if ( NULL == OldComponentPtr ) { continue; }
				
				OldIndex = OldComponentPtr->GetComponentIndex_Project();
				NewComponentPtr = ProjectComponentPtrMap[OldIndex];
				NewBoardPtr->AddBoardComponentPtr(NewComponentPtr);
			}
		}

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
	{	NewPanelPtr->SetPanelIndex_Project(PanelIndex); }
	NewPanelPtr->SetPanelProjectPtr(this);
	NewPanelPtr->SetPanelMultiDistrictMode(GetProjectMultiDistrictMode());
	AddProjectPanelPtr_Inline(NewPanelPtr);	
	return NewPanelPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::DestroyProjectPanelSelected()
{
	size_t i = 0;
	int    index = 0;
	CAOIPanel *PanelPtr = NULL;
	std::vector<CAOIPanel*> ProjectPanelPtrList = this->m_ProjectPanelPtrList;
	
	index = 0;
	this->m_ProjectPanelPtrList.clear();
	const size_t PanelCount = ProjectPanelPtrList.size();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPanelPtrList[i];
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelSelected() == TRUE )
		{
			AOIObjManager.DestroyPanelObj(ProjectPanelPtrList[i]);			
			PanelPtr = NULL;
			continue;
		}

		PanelPtr->SetPanelIndex_Project(index);
		CAOIProject::AddProjectPanelPtr_Inline(PanelPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::ClearProjectAllPanels()//刪除專案整板
{
	size_t i = 0;	
	CAOIPanel *PanelPtr = NULL;	
	const size_t PanelCount = GetProjectPanelCount_Inline();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr_Inline(i);
		if ( NULL == PanelPtr ) { continue; }		
		AOIObjManager.DestroyPanelObj(m_ProjectPanelPtrList[i]);			
		PanelPtr = NULL;				
	}
	this->m_ProjectPanelPtrList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::LayoutProjectPanelList()//重整專案的整板列表
{
	size_t       i = 0;
	unsigned int index = 0;
	CAOIPanel *PanelPtr = NULL;
	std::vector<CAOIPanel*> ProjectPanelPtrList = this->m_ProjectPanelPtrList;
	
	index = 0;
	this->m_ProjectPanelPtrList.clear();
	const size_t PanelCount = ProjectPanelPtrList.size();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPanelPtrList[i];
		if ( NULL == PanelPtr ) { continue; }
		
		PanelPtr->SetPanelIndex_Project(index);
		CAOIProject::AddProjectPanelPtr_Inline(PanelPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectAllPanels(bool value)//選取專案的所有整板
{
	size_t i = 0;	
	CAOIPanel *PanelPtr = NULL;	
	const size_t PanelCount = GetProjectPanelCount_Inline();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr_Inline(i);
		if ( NULL == PanelPtr ) { continue; }		
		PanelPtr->SetPanelSelected(value);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectPanelsByStage(const TREGION4D &Rgn, bool ResultMode, std::vector<CAOIPanel*> &PanelList)//選取零件	
{
	size_t         i = 0;	
	CAOIPanel     *PanelPtr = NULL;
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	const bool     PickSelMode = JetAPI::CheckPickSelectMode(Rgn, 25);
	const size_t   PanelCount = GetProjectPanelCount_Inline();

	PanelList.clear();
	if ( true == PickSelMode )
	{
		TPOINT2D Pt;
		Pt.x = Rgn.GetCpX();
		Pt.y = Rgn.GetCpY();
		for ( i=0; i<PanelCount; i++ )
		{
			PanelPtr = GetProjectPanelPtr_Inline(i);
			if ( NULL == PanelPtr ) { continue; }
			if ( PanelPtr->GetPanelDeleted() == true ) { continue; }			
			if ( PanelPtr->CheckPanelBePickByStage(DistrictID, Pt) == false ) { continue; }
			PanelList.push_back(PanelPtr);
		}
	}
	else
	{
		for ( i=0; i<PanelCount; i++ )
		{
			PanelPtr = GetProjectPanelPtr_Inline(i);
			if ( NULL == PanelPtr ) { continue; }
			if ( PanelPtr->GetPanelDeleted() == true ) { continue; }
			if ( PanelPtr->CheckPanelInRegionByStage(DistrictID, Rgn, true) == false ) { continue; }
			PanelList.push_back(PanelPtr);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIPanel* CAOIProject::GetProjectPanelPtrBySelected()//取得專案整板指標-依據選取到	
{
	size_t i = 0;	
	CAOIPanel *PanelPtr = NULL;	
	const size_t PanelCount = GetProjectPanelCount_Inline();	
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr_Inline(i);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelDeleted() == true ) { continue; }
		if ( PanelPtr->GetPanelSelected() == false )
		{	continue; }
		return PanelPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
CAOIPanel* CAOIProject::GetProjectPanelPtrByPanelUUID(const UUID &uuid)//取得專案整板指標-依據萬用字碼
{
	size_t       i = 0;	
	UUID         PanelUUID;
	CAOIPanel   *PanelPtr = NULL;	
	const size_t PanelCount = GetProjectPanelCount_Inline();	
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr_Inline(i);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelDeleted() == true ) { continue; }
		PanelUUID = PanelPtr->GetObjUuid();
		if ( PanelUUID != uuid ) { continue; }		
		return PanelPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectPanelSelectedCount() const//取得專案整板選取到的數量
{
	size_t i = 0;	
	size_t Count=0;
	CAOIPanel *PanelPtr = NULL;	
	const size_t PanelCount = GetProjectPanelCount_Inline();	
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr_Inline(i);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelDeleted() == true ) { continue; }
		if ( PanelPtr->GetPanelSelected() == false )
		{	continue; }
		Count ++;
	}
	return Count;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::GetProjectPanelSelected(std::vector<CAOIPanel*> &SelPanelList)//取得專案整板選取到
{
	size_t i = 0;	
	CAOIPanel *PanelPtr = NULL;	
	const size_t PanelCount = GetProjectPanelCount_Inline();	
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr_Inline(i);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelDeleted() == true ) { continue; }
		if ( PanelPtr->GetPanelSelected() == false )
		{	continue; }
		SelPanelList.push_back(PanelPtr);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectPanelSelected()//刪除選取到的專案整板
{
	size_t i = 0;	
	CAOIPanel *PanelPtr = NULL;	
	const size_t PanelCount = GetProjectPanelCount_Inline();
	
	ClearProjectAllTempObjects();//刪除所有暫時用的物件

	SelectProjectAllFds(false);
	SelectProjectAllMarks(false);
	SelectProjectAllBarcodes(false);
	SelectProjectAllBoards(false);	
	SelectProjectAllComponents(false);
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr_Inline(i);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelSelected() == FALSE )
		{	continue; }
		PanelPtr->SelectPanelAllObjects(true);
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
//-------------------------------------------------------------------------------------//
bool CAOIProject::RotateProjectPanelSelected(double Angle)//選轉整板角度
{	
	size_t      i=0;
	CAOIPanel   *PanelPtr = NULL;	
	const size_t PanelCount = GetProjectPanelCount_Inline();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr_Inline(i);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelSelected() == false ) { continue; }		
		PanelPtr->SpinPanel(Angle);		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::InvertSelectProjectPanel(bool bUpdate)//反向選取整板
{
	size_t       i=0;
	bool         Selected = false;
	CAOIPanel   *PanelPtr = NULL;
	const size_t PanelCount = GetProjectPanelCount_Inline();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr_Inline(i);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelSelected() == true )
		{	Selected = false; }
		else
		{	Selected = true; }
		PanelPtr->SetPanelSelected(Selected);
		if ( true == bUpdate )
		{	PanelPtr->SelectPanelAllObjects(Selected); }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::MoveProjectPanelSelected(double dX, double dY)//移動專案選取到的整板
{
	size_t       i=0;	
	CAOIPanel   *PanelPtr = NULL;	
	const size_t PanelCount = GetProjectPanelCount_Inline();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr_Inline(i);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelSelected() == false ) { continue; }		
		PanelPtr->MovePanelPos(dX, dY);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::MirrorXProjectPanelSelected()//鏡射專案選取到整板的X座標
{
	size_t       i=0;	
	double       CadCpXD=0;
	TREGION4D    RgnCadD;	
	CAOIPanel   *PanelPtr = NULL;
	const size_t PanelCount = GetProjectPanelCount_Inline();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr_Inline(i);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelSelected() == false ) { continue; }		

		RgnCadD = PanelPtr->GetPanelRgnCad();				
		CadCpXD = (RgnCadD.minX+RgnCadD.maxX)*0.5;
		PanelPtr->MirrorXPanel(CadCpXD);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::MirrorYProjectPanelSelected()//鏡射專案選取到整板的Y座標
{
	size_t       i=0;	
	double       CadCpY=0;	
	TREGION4D    RgnCad;
	CAOIPanel   *PanelPtr = NULL;	
	const size_t PanelCount = GetProjectPanelCount_Inline();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr_Inline(i);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelSelected() == false ) { continue; }		

		RgnCad = PanelPtr->GetPanelRgnCad();		
		CadCpY = (RgnCad.minY+RgnCad.maxY)*0.5;		
		PanelPtr->MirrorYPanel(CadCpY);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::SwitchProjectPanelBypassed()//切換專案整板不檢測模式
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return true; }
	std::vector<CAOIPanel*> SelPanelList;	

	ProjectPtr->GetProjectPanelSelected(SelPanelList);
	const size_t SelCount =  SelPanelList.size();	
	if ( 0 == SelCount ) { return true; }	

	size_t i=0;
	CAOIPanel  *PanelPtr = NULL;	
	PanelPtr =  SelPanelList[0];
	if ( NULL == PanelPtr ) { return true; }
	bool Bypass = PanelPtr->GetPanelBypassed();
	if ( true == Bypass ) { Bypass = false; }
	else { Bypass = true; }

	for ( i=0; i<SelCount; i++ )
	{
		PanelPtr = (SelPanelList[i]);
		if ( NULL == PanelPtr ) { continue; }		
		PanelPtr->SetPanelBypassed(Bypass);
		PanelPtr->UpdatePanelBypassed();
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectPanelValid(const CAOIPanel *RefPanelPtr)//確認專案的整板指標有效
{
	if ( NULL == RefPanelPtr ) { return false; }
	size_t       i=0;		
	CAOIPanel   *PanelPtr = NULL;
	const size_t PanelCount = GetProjectPanelCount_Inline();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr_Inline(i);
		if ( NULL == PanelPtr ) { continue; }
		if ( RefPanelPtr == PanelPtr )
		{	return true; }
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::PasteProjectPanelList(std::vector<CAOIPanel*> &ClonePanelList, double StageOffsetX, double StageOffsetY)//貼上專案整板
{
	size_t         i=0, j=0;
	size_t         FdCount = 0;
	size_t         BoardCount = 0;
	size_t         BarcodeCount = 0;
	size_t         ComponentCount = 0;
	TPOINT2D       CadPosOld;
	TPOINT2D       CadPosNew;
	TPOINT2D       StagePosOld;
	TPOINT2D       StagePosNew;
	TPOINT2D       CadOffset;
	TPOINT2D       StageOffset(StageOffsetX, StageOffsetY);
	CAOIFd        *NewFdPtr = NULL;
	CAOIPanel     *pPanel = NULL;
	CAOIPanel     *NewPanelPtr = NULL;
	CAOIBoard     *NewBoardPtr = NULL;
	CAOIBarcode   *NewBarcodePtr = NULL;
	CAOIComponent *NewComponentPtr = NULL;
	CMapCoordinate *MapCTSPtr = NULL;		
	CMapCoordinate *MapSTCPtr = NULL;		
	DISTRICT_ID     DistrictID = GetProjectActDistrictID();
	const size_t    CloneCount = ClonePanelList.size();
	if ( 0 == CloneCount ) { return true; }

	//this->SelectProjectAllPanels(false);
	for ( i=0; i<CloneCount; i++ )
	{
		pPanel = ClonePanelList[i];
		if ( NULL == pPanel ) { continue; }				
		pPanel->SetPanelSelected(false);
		pPanel->SelectPanelAllObjects(false);
		CadPosOld.x = pPanel->GetPanelRgnCad().GetCpX();
		CadPosOld.y = pPanel->GetPanelRgnCad().GetCpY();
		MapCTSPtr = pPanel->GetPanelMapCTSPtr(DistrictID);
		MapSTCPtr = pPanel->GetPanelMapSTCPtr(DistrictID);		
		StagePosOld.x = pPanel->GetPanelRgnStage(DistrictID).GetCpX();
		StagePosOld.y = pPanel->GetPanelRgnStage(DistrictID).GetCpY();
		NewPanelPtr = AddProjectPanelPtr(pPanel, true);
		if ( NULL == NewPanelPtr ) { continue; }		

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
		NewPanelPtr->SetPanelSelected(true);
		NewPanelPtr->SelectPanelAllObjects(true);
		NewPanelPtr->MovePanelPos(CadOffset.x, CadOffset.y);
		NewPanelPtr->CalcPanelMapParam();		
		NewPanelPtr->AssignPanelMapParamToBoards();		

		BoardCount = NewPanelPtr->GetPanelBoardCount();
		for ( j=0; j<BoardCount; j++ )
		{
			NewBoardPtr = NewPanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == NewBoardPtr ) { continue; }
			NewBoardPtr->CalcBoardMapParam(DistrictID);			
		}
		NewPanelPtr->LayoutPanelRegion();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::ArrayPasteProjectPanelList(std::vector<CAOIPanel*> &ClonePanelList, const std::vector<TPOINT2D> &PosList)//陣列貼上專案整板
{
	size_t          i=0, j=0, k=0;
	size_t          BoardCount = 0;
	size_t          ComponentCount = 0;	
	double          OffsetX=0, OffsetY=0;
	CAOIPanel      *pPanel = NULL;
	CAOIPanel      *NewPanelPtr = NULL;
	CAOIBoard      *NewBoardPtr = NULL;
	CAOIComponent  *NewComponentPtr = NULL;
	const size_t    PosCount = PosList.size();
	DISTRICT_ID     DistrictID = GetProjectActDistrictID();
	const size_t    CloneCount = ClonePanelList.size();
	if ( 0 == CloneCount ) { return true; }

	//this->SelectProjectAllPanels(false);
	for ( k=0; k<PosCount; k++ )
	{
		OffsetX = PosList[k].x;
		OffsetY = PosList[k].y;
		for ( i=0; i<CloneCount; i++ )
		{
			pPanel = ClonePanelList[i];
			if ( NULL == pPanel ) { continue; }		
			pPanel->SetPanelSelected(false);
			pPanel->SelectPanelAllObjects(false);

			NewPanelPtr = this->AddProjectPanelPtr(pPanel, true);
			if ( NULL == NewPanelPtr ) { continue; }		
			NewPanelPtr->SetPanelSelected(true);
			NewPanelPtr->SelectPanelAllObjects(true);
			NewPanelPtr->MovePanelPos(OffsetX, OffsetY);
			NewPanelPtr->CalcPanelMapParam();		
			NewPanelPtr->AssignPanelMapParamToBoards();
			NewPanelPtr->SetPanelResultID(RESULT_ID_NONE);
			NewPanelPtr->SetPanelResultID_AOI_LA(RESULT_ID_NONE);
			NewPanelPtr->SetPanelResultID_AOI_LB(RESULT_ID_NONE);		
			NewPanelPtr->SetPanelResultID_Alarm(RESULT_ID_NONE);

			BoardCount = NewPanelPtr->GetPanelBoardCount();
			for ( j=0; j<BoardCount; j++ )
			{
				NewBoardPtr = NewPanelPtr->GetPanelBoardPtr(j, false);
				if ( NULL == NewBoardPtr ) { continue; }
				NewBoardPtr->CalcBoardMapParam();
				NewBoardPtr->SetBoardResultID(RESULT_ID_NONE);
				NewBoardPtr->SetBoardResultID_AOI_LA(RESULT_ID_NONE);
				NewBoardPtr->SetBoardResultID_AOI_LB(RESULT_ID_NONE);
				NewBoardPtr->SetBoardResultID_Alarm(RESULT_ID_NONE);
			}

			ComponentCount = NewPanelPtr->GetPanelComponentCount();
			for ( j=0; j<ComponentCount; j++ )
			{
				NewComponentPtr = NewPanelPtr->GetPanelComponentPtr(j, false);
				if ( NULL == NewComponentPtr ) { continue; }
				NewComponentPtr->InitComponentInspection();
				NewComponentPtr->ResetComponentResultStatistic();
			}
			NewPanelPtr->LayoutPanelRegion();
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::SetProjectPanelSelectedBarcodeCodeIndex(unsigned int BarcodeCodeIndex)//設定專案整板條碼序號
{
	size_t       i=0;		
	CAOIPanel   *PanelPtr = NULL;
	const size_t PanelCount = GetProjectPanelCount_Inline();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr_Inline(i);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelDeleted() == true ) { continue; }
		if ( PanelPtr->GetPanelSelected() == false ) { continue; }
		PanelPtr->SetPanelBarcodeDeviceCodeIndex(BarcodeCodeIndex);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::SetProjectPanelSelectedBarcodeDeviceIndex(unsigned int BarcodeDeviceIndex)//設定專案整板條碼機序號
{
	size_t       i=0;		
	CAOIPanel   *PanelPtr = NULL;
	const size_t PanelCount = GetProjectPanelCount_Inline();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr_Inline(i);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelDeleted() == true ) { continue; }
		if ( PanelPtr->GetPanelSelected() == false ) { continue; }
		PanelPtr->SetPanelBarcodeDeviceIndex(BarcodeDeviceIndex);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
