// AOIProject_Fd.cpp: implementation of the CAOIProject class.
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
inline size_t CAOIProject::GetProjectFdCount_Inline() const//取得專案定位點數量
{
	return CAOIProject::m_ProjectFdPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::AddProjectFdPtr_Inline(CAOIFd *FdPtr)//增加專案定位點
{
	CAOIProject::m_ProjectFdPtrList.push_back(FdPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIFd* CAOIProject::GetProjectFdPtr_Inline(size_t index) const//取得專案定位點指標	
{
	return m_ProjectFdPtrList[index];
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIProject::GetProjectFdSortedCount_Inline() const//取得專案定位點排序數量
{
	return m_ProjectFdSortedList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::AddProjectFdSorted_Inline(CAOIFd *FdPtr)//增加專案定位點排序
{
	m_ProjectFdSortedList.push_back(FdPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIFd* CAOIProject::GetProjectFdSorted_Inline(size_t index) const//取得專案定位點排序指標	
{
	return m_ProjectFdSortedList[index];
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::ClearProjectFdSortedList_Inline()//清除專案定位點排序列表
{
	m_ProjectFdSortedList.clear();
}
//-------------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectFdCount() const//取得專案定位點數量
{
	return GetProjectFdCount_Inline();
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::CalcProjectPanelFdCount(DISTRICT_ID DistrictID) const//計算專案整板定位點數量
{
	size_t       i=0;	
	size_t       Count=0;		
	CAOIFd      *FdPtr = NULL;	
	const size_t FdCount = GetProjectFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->CheckFdIsPanelFd() == false ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }		
		Count ++;
	}
	return Count;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::CalcProjectBoardFdCount(DISTRICT_ID DistrictID) const//計算專案單板定位點數量
{
	size_t       i=0;	
	size_t       Count=0;		
	CAOIFd      *FdPtr = NULL;	
	const size_t FdCount = GetProjectFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->CheckFdIsBoardFd() == false ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }		
		Count ++;
	}
	return Count;
}
//---------------------------------------------------------------------------------//
size_t  CAOIProject::GetProjectFdCount(DISTRICT_ID DistrictID, bool PanelFdOnly) const//取得專案定位點數量	
{
	size_t       i=0;	
	size_t       Count=0;		
	CAOIFd      *FdPtr = NULL;	
	const size_t FdCount = GetProjectFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		if ( true == PanelFdOnly ) 
		{
			if ( NULL != FdPtr->GetFdBoardPtr() ) 
			{	continue; }
		}
		Count ++;
	}
	return Count;
}
//---------------------------------------------------------------------------------//
CAOIFd* CAOIProject::GetProjectFdPtr(size_t index, bool check) const//取得專案定位點指標
{
	if ( check )
	{
		const size_t count = GetProjectFdCount_Inline();
		if ( index >= count )
		{	return NULL; }
	}
	return GetProjectFdPtr_Inline(index);	
}
//---------------------------------------------------------------------------------//
CAOIFd* CAOIProject::AddProjectFdPtr(CAOIFd *FdPtr, bool clone)//增加專案定位點
{
	if ( NULL == FdPtr )
	{
		this->m_ErrorString.Format(_T("Error, AddProjectFdPtr Fault"));
		return NULL;
	}

	CAOIFd *NewFdPtr = FdPtr;
	const int FdIndex = (int)(GetProjectFdCount_Inline());
	if ( true == clone )
	{
		CAOIPanel        *PanelPtr = FdPtr->GetFdPanelPtr();
		CAOIBoard        *BoardPtr = FdPtr->GetFdBoardPtr();
		if ( NULL==PanelPtr ) 
		{ 
			this->m_ErrorString.Format(_T("Error, AddProjectFdPtr Fault"));
			return NULL; 
		}

		NewFdPtr = FdPtr->CloneFdObj();
		if ( NewFdPtr == NULL ) { return NULL; }

		NewFdPtr->SetFdIndex_Project(FdIndex);	
		if ( NULL != BoardPtr )
		{	BoardPtr->AddBoardFdPtr(NewFdPtr); }
		PanelPtr->AddPanelFdPtr(NewFdPtr);
	}	
	else
	{	NewFdPtr->SetFdIndex_Project(FdIndex); }

	int FdUniqueID = NewFdPtr->GetFdUniqueID();
	if ( FdUniqueID < 0 )
	{
		NewFdPtr->SetFdUniqueID(m_ProjectFdMaxUniqueID);
		m_ProjectFdMaxUniqueID ++;
	}
	else
	{
		if ( m_ProjectFdMaxUniqueID <= FdUniqueID )
		{	m_ProjectFdMaxUniqueID = FdUniqueID+1; }
	}
	SetProjectFdModified(true);
	NewFdPtr->SetFdProjectPtr(this);
	CAOIProject::AddProjectFdPtr_Inline(NewFdPtr);	
	return NewFdPtr;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DestroyProjectFdSelected()//摧毀選取到的專案定位點
{
	size_t     i = 0;
	int        index = 0;
	CAOIFd    *FdPtr = NULL;
	CAOIModel *ModelPtr = NULL;
	CString    FdModelFolder;
	std::vector<CAOIFd*> ProjectFdPtrList = this->m_ProjectFdPtrList;
	
	index = 0;
	this->m_ProjectFdPtrList.clear();
	const size_t FdCount = ProjectFdPtrList.size();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = ProjectFdPtrList[i];
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdSelected() == TRUE )
		{
			ModelPtr = FdPtr->GetFdModelPtr();
			FdModelFolder = ModelPtr->GetModelFolder();			
			AOIObjManager.DestroyFdObj(ProjectFdPtrList[i]);			
			FdPtr = NULL;
			ModelPtr = NULL;
			JetAPI::RemoveFolder(FdModelFolder);
			continue;
		}

		FdPtr->SetFdIndex_Project(index);
		CAOIProject::AddProjectFdPtr_Inline(FdPtr);
		index ++;
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ClearProjectAllFds()//刪除專案定位點
{
	size_t i = 0;
	CAOIFd *FdPtr = NULL;
	const size_t FdCount = GetProjectFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }		
		AOIObjManager.DestroyFdObj(m_ProjectFdPtrList[i]);			
		FdPtr = NULL;		
	}		
	this->m_ProjectFdPtrList.clear();

	ClearProjectFdSortedList_Inline();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::LayoutProjectFdList()//重整專案的定位點列表	
{
	size_t       i = 0;
	unsigned int index = 0;
	CAOIFd *FdPtr = NULL;
	std::vector<CAOIFd*> ProjectFdPtrList = m_ProjectFdPtrList;
	
	index = 0;
	m_ProjectFdPtrList.clear();
	const size_t FdCount = ProjectFdPtrList.size();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = ProjectFdPtrList[i];
		if ( NULL == FdPtr ) { continue; }

		FdPtr->SetFdIndex_Project(index);
		AddProjectFdPtr_Inline(FdPtr);
		index ++;
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SortProjectFdListFull()//排序專案定位點列表
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t  i=0;
	void *Ptr = NULL;
	unsigned int FdSortID=0;
	CAOIFd *FdPtr = NULL;
	CAOIPanel *PanelPtr = NULL;
	CAOIBoard *BoardPtr = NULL;
	std::vector<CAOIFd*> PanelFdList;
	std::vector<CAOIFd*> BoardFdList;
	const size_t FdCount = ProjectPtr->GetProjectFdCount_Inline();
	ProjectPtr->ClearProjectFdSortedList_Inline();

	CSortObj SortObj;
	std::vector<CSortObj> SortList;

	SortObj.SetSortMode(SORT_BY_INT);

	//Panel Fd List
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = ProjectPtr->GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }

		PanelPtr = FdPtr->GetFdPanelPtr();
		BoardPtr = FdPtr->GetFdBoardPtr();
		FdSortID = FdPtr->GetFdSortID();

		if ( NULL == PanelPtr ) { continue; }
		if ( NULL != BoardPtr ) { continue; }

		SortObj.SetID(i);
		SortObj.SetPtr(FdPtr);
		SortObj.SetValueInt(FdSortID);
		SortList.push_back(SortObj);
	}	
	std::sort(SortList.begin(), SortList.end());

	
	const size_t PanelSortCount = SortList.size();
	for ( i=0; i<PanelSortCount; i++ )
	{
		SortObj = SortList[i];
		Ptr = SortObj.GetPtr();
		if ( NULL == Ptr ) { continue; }		
		FdPtr = (CAOIFd*)(Ptr);		
		PanelFdList.push_back(FdPtr);
	}
	
	const size_t PanelFdCount = PanelFdList.size();
	if ( 0==PanelFdCount || PanelFdCount != PanelSortCount )//確認是否有誤
	{
		PanelFdList.clear();
		for ( i=0; i<FdCount; i++ )
		{
			FdPtr = ProjectPtr->GetProjectFdPtr_Inline(i);
			if ( NULL == FdPtr ) { continue; }

			PanelPtr = FdPtr->GetFdPanelPtr();
			BoardPtr = FdPtr->GetFdBoardPtr();
			FdSortID = FdPtr->GetFdSortID();
			
			if ( NULL == PanelPtr ) { continue; }
			if ( NULL != BoardPtr ) { continue; }
			PanelFdList.push_back(FdPtr);
		}	
	}

	//Board Fd List
	SortList.clear();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = ProjectPtr->GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }

		PanelPtr = FdPtr->GetFdPanelPtr();
		BoardPtr = FdPtr->GetFdBoardPtr();
		FdSortID = FdPtr->GetFdSortID();

		if ( NULL == PanelPtr ) { continue; }
		if ( NULL == BoardPtr ) { continue; }

		SortObj.SetID(i);
		SortObj.SetPtr(FdPtr);
		SortObj.SetValueInt(FdSortID);
		SortList.push_back(SortObj);
	}	
	std::sort(SortList.begin(), SortList.end());
	
	const size_t BoardSortCount = SortList.size();
	for ( i=0; i<BoardSortCount; i++ )
	{
		SortObj = SortList[i];
		Ptr = SortObj.GetPtr();
		if ( NULL == Ptr ) { continue; }		
		FdPtr = (CAOIFd*)(Ptr);		
		BoardFdList.push_back(FdPtr);
	}
	
	const size_t BoardFdCount = BoardFdList.size();
	if ( 0==BoardFdCount || BoardFdCount != BoardSortCount )//確認是否有誤
	{
		BoardFdList.clear();
		for ( i=0; i<FdCount; i++ )
		{
			FdPtr = ProjectPtr->GetProjectFdPtr_Inline(i);
			if ( NULL == FdPtr ) { continue; }

			PanelPtr = FdPtr->GetFdPanelPtr();
			BoardPtr = FdPtr->GetFdBoardPtr();
			FdSortID = FdPtr->GetFdSortID();
			
			if ( NULL == PanelPtr ) { continue; }
			if ( NULL == BoardPtr ) { continue; }
			BoardFdList.push_back(FdPtr);
		}	
	}

	const size_t PanelFdCount2 = PanelFdList.size();
	const size_t BoardFdCount2 = BoardFdList.size();
	for ( i=0; i<PanelFdCount2; i++  )
	{
		FdPtr = PanelFdList[i];
		AddProjectFdSorted_Inline(FdPtr);
	}
	for ( i=0; i<BoardFdCount2; i++  )
	{
		FdPtr = BoardFdList[i];
		AddProjectFdSorted_Inline(FdPtr);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectAllFds(bool value)//選取專案的所有定位點
{
	size_t i = 0;	
	CAOIFd *FdPtr = NULL;	
	const size_t FdCount = GetProjectFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		FdPtr->SetFdSelected(value);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ClearProjectFdImageBuffer()//清除專案定位點影像資料
{
	size_t i = 0;	
	CAOIFd *FdPtr = NULL;	
	const size_t FdCount = GetProjectFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdKeepImage() == false ) { continue; }
		FdPtr->ClearRgnImageBuffer();
		FdPtr->SetFdKeepImage(false);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectFdByObjectSelected(bool bPanelFd, bool bBoardFd)//依據專案選取道的零件選取專案的定位點	
{
	size_t         i = 0;	
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIBoard     *BoardPtr = NULL;
	CAOIPanel     *PanelPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const size_t FdCount = GetProjectFdCount_Inline();
	const size_t MarkCount = GetProjectMarkCount();
	const size_t BoardCount = GetProjectBoardCount();
	const size_t PanelCount = GetProjectPanelCount();
	const size_t BarcodeCount = GetProjectBarcodeCount();
	const size_t ComponentCount = GetProjectComponentCount();

	SelectProjectAllFds(false);
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		PanelPtr->SetPanelTempInt(FN_DISABLE);
	}
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->SetBoardTempInt(FN_DISABLE);
	}

	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }
		PanelPtr = MarkPtr->GetMarkPanelPtr();
		if ( NULL != PanelPtr )
		{	PanelPtr->SetPanelTempInt(FN_ENABLE); }

		BoardPtr = MarkPtr->GetMarkBoardPtr();
		if ( NULL != BoardPtr )
		{	BoardPtr->SetBoardTempInt(FN_ENABLE); }
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeSelected() == false ) { continue; }
		PanelPtr = BarcodePtr->GetBarcodePanelPtr();
		if ( NULL != PanelPtr )
		{	PanelPtr->SetPanelTempInt(FN_ENABLE); }

		BoardPtr = BarcodePtr->GetBarcodeBoardPtr();
		if ( NULL != BoardPtr )
		{	BoardPtr->SetBoardTempInt(FN_ENABLE); }
	}

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		PanelPtr = ComponentPtr->GetComponentPanelPtr();
		if ( NULL != PanelPtr )
		{	PanelPtr->SetPanelTempInt(FN_ENABLE); }

		BoardPtr = ComponentPtr->GetComponentBoardPtr();
		if ( NULL != BoardPtr )
		{	BoardPtr->SetBoardTempInt(FN_ENABLE); }
	}

	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		PanelPtr = FdPtr->GetFdPanelPtr();
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelBypassed() == true ) { continue; }

		BoardPtr = FdPtr->GetFdBoardPtr();
		if ( NULL == BoardPtr )
		{ 
			if ( false == bPanelFd ) { continue; }
			if ( PanelPtr->GetPanelTempInt() == FN_DISABLE ) { continue; }
			FdPtr->SetFdSelected(true);
			continue; 
		}
		if ( false == bBoardFd ) { continue; }
		if ( BoardPtr->GetBoardBypassed() == true ) { continue; }
		if ( BoardPtr->GetBoardMapEnable() == false ) { continue; }
		if ( BoardPtr->GetBoardTempInt() == FN_DISABLE ) { continue; }
		FdPtr->SetFdSelected(true);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectFdForTuning(bool bOffline, bool bNeedGrabFd)//選取專案定位點來調適專案
{
	bool bGrabFdBoard=true;
	bool bGrabFdPanel=true;
	bool bPanelFdXBoard=false;
	bool bBoardFdXBoard=false;	
	const DISTRICT_ID DistrictID = GetProjectActDistrictID();
	const TProjectParameter &ProgParam = GetProjectParameter();
	const size_t PanelFdCount = CalcProjectPanelFdCount(DistrictID);
	const size_t BoardFdCount = CalcProjectBoardFdCount(DistrictID);

	BOARD_FD_GRAB_MODE BoardFdGrabMode = ProgParam.m_BoardFdGrabMode;
	FD_NG_HANDLE_MODE PanelFdNGHandleMode = ProgParam.m_PanelFdNGHandleMode;
	FD_NG_HANDLE_MODE BoardFdNGHandleMode = ProgParam.m_BoardFdNGHandleMode;
	
	if ( 0 == PanelFdCount )
	{	bGrabFdPanel = false;	}
	else if ( FD_NG_HANDLE_XBOARD == PanelFdNGHandleMode )
	{	bPanelFdXBoard = true; }

	if ( 0 == BoardFdCount )
	{	bGrabFdBoard = false;	}
	else if ( FD_NG_HANDLE_XBOARD == BoardFdNGHandleMode )
	{	bBoardFdXBoard = true; }
 
	if ( true == bOffline )
	{	SelectProjectAllFds(false);	}
	else
	{	
		if ( true == bNeedGrabFd )
		{	SelectProjectAllFds(true);	}
		else
		{			
			bGrabFdPanel = bPanelFdXBoard;
			const bool bUseGrabFdPanel=bPanelFdXBoard;
			const bool bUseGrabFdBoard=bBoardFdXBoard;
			SelectProjectFdByObjectSelected(bUseGrabFdPanel, bUseGrabFdBoard);			
			if ( true == bUseGrabFdPanel )
			{	bNeedGrabFd = true;	}
			else if ( true==bUseGrabFdBoard && BOARD_FD_GRAB_AFTER_PANEL==BoardFdGrabMode )
			{	bNeedGrabFd = true;	}
		}
	}	
	SetProjectNeedGrabFdForTuning(bNeedGrabFd);
	return true;
}
//---------------------------------------------------------------------------------//
CAOIFd* CAOIProject::GetProjectFdPtrBySelected()//取得專案定位點指標-依據選取到	
{
	size_t i = 0;	
	CAOIFd *FdPtr = NULL;	
	const size_t FdCount = GetProjectFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdDeleted() == true ) { continue; }
		if ( FdPtr->GetFdSelected() == false ) { continue; }
		return FdPtr;		
	}		
	return NULL;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::GetProjectFdSelected(std::vector<CAOIFd*> &SelFdList)//取得專案定位點選取到
{
	size_t i=0, j=0;			
	CAOIFd *FdPtr = NULL;	
	const size_t FdCount = GetProjectFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdDeleted() == true ) { continue; }
		if ( FdPtr->GetFdSelected() == false )
		{	continue; }
		SelFdList.push_back(FdPtr);
	}
	return true;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectFdSelectedCount() const//計算專案定位點選取到數量
{
	size_t  i=0;			
	size_t  Count=0;
	CAOIFd *FdPtr = NULL;	
	const size_t FdCount = GetProjectFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdDeleted() == true ) { continue; }
		if ( FdPtr->GetFdSelected() == false )
		{	continue; }
		Count ++;
	}
	return Count;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::GetProjectBoardFdSelected(std::vector<CAOIFd*> &SelFdList)//取得專案單板定位點選取到
{
	size_t i=0, j=0;			
	CAOIFd *FdPtr = NULL;	
	const size_t FdCount = GetProjectFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdDeleted() == true ) { continue; }
		if ( FdPtr->GetFdSelected() == false )
		{	continue; }
		if ( NULL == FdPtr->GetFdBoardPtr() )
		{	continue; }
		SelFdList.push_back(FdPtr);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectFdSelected()//刪除選取到的專案定位點
{
	size_t     FdSelCount=0;
	size_t     i=0, j=0, k=0;	
	CAOIFd    *FdPtr = NULL;	
	CAOIPanel *PanelPtr = NULL;
	CAOIBoard *BoardPtr = NULL;	
	const size_t FdCount = GetProjectFdCount_Inline();
	const size_t PanelCount = GetProjectPanelCount();
	const size_t BoardCount = GetProjectBoardCount();	
	
	ClearProjectAllTempObjects();//刪除所有暫時用的物件

	SelectProjectAllMarks(false);	
	SelectProjectAllBarcodes(false);
	SelectProjectAllPanels(false);
	SelectProjectAllBoards(false);
	SelectProjectAllComponents(false);

	FdSelCount = 0;
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdSelected() == FALSE ) { continue; }
		FdPtr->SetFdSelected(TRUE);
		FdSelCount ++;
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

			const size_t NFds = BoardPtr->GetBoardFdCount();
			for ( k=0; k<NFds; k++ )
			{
				FdPtr = BoardPtr->GetBoardFdPtr(k, false);
				if ( FdPtr == NULL ) { continue; }
				if ( FdPtr->GetFdSelected() == TRUE )
				{	break; }
			}
			if ( k == NFds ) { continue; }
			BoardPtr->RemoveBoardFdSelected();
			BoardPtr->LayoutBoardRegion();
		}

		const size_t NFds = PanelPtr->GetPanelFdCount();
		for ( k=0; k<NFds; k++ )
		{
			FdPtr = PanelPtr->GetPanelFdPtr(k, false);
			if ( FdPtr == NULL ) { continue; }
			if ( FdPtr->GetFdSelected() == TRUE )
			{	break; }
		}
		if ( k == NFds ) { continue; }
		PanelPtr->RemovePanelFdSelected();
		PanelPtr->LayoutPanelRegion();
	}	

	if ( FdSelCount > 0 ) 
	{	SetProjectFdModified(true); }
	DestroyProjectObjectSelected();
	LayoutProjectAllObjectsIndex();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectFdInsideStageLimit()//確認專案定位點落於機台內
{
	size_t i = 0;	
	TPOINT3D FdPos;
	CAOIFd *FdPtr = NULL;		
	std::vector<CAOIFd*> BadFdList;
	const size_t FdCount = GetProjectFdCount_Inline();
	const double StageMinX = MotionCtrlPtr->GetMotionParameter().m_LimitMinX;
	const double StageMinY = MotionCtrlPtr->GetMotionParameter().m_LimitMinY;
	const double StageMaxX = MotionCtrlPtr->GetMotionParameter().m_LimitMaxX;
	const double StageMaxY = MotionCtrlPtr->GetMotionParameter().m_LimitMaxY;
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		FdPtr->GetFdTeachStagePos(FdPos);		
		if ( FdPos.x<StageMinX || FdPos.y<StageMinY || FdPos.x>StageMaxX || FdPos.y>StageMaxY )
		{	BadFdList.push_back(FdPtr);	}
	}

	const size_t BadFdCount = BadFdList.size();
	if ( BadFdCount > 0 )
	{	
		CString str;
		CString filename;	
		unsigned int FdIndex=0;
		unsigned int PanelIndex=0;
		unsigned int BoardIndex=0;
		CString Folder = AOIDataCollect.GetAOITempDirectory();
		TCHAR TMode[32] = _T("");
		_tcscpy(TMode, _T("w+"));
		JetAPI::ModifyOpenFileMode_Write(TMode);		
		filename.Format(_T("%s\\%s"), Folder, _T("BadFdList.TXT"));
		FILE *pfile = ::_tfopen(filename, TMode);
		if ( NULL != pfile ) 
		{
			CString strFd = AOIDataDefine.GetFdText();
			CString strPanel = AOIDataDefine.GetPanelText();
			CString strBoard = AOIDataDefine.GetBoardText();
			::_ftprintf(pfile, _T("Fd postion is out of stage Limit\n"));
			::_ftprintf(pfile, _T("%s, %s, %s\n"), strPanel, strBoard, strFd);
			for ( i=0; i<BadFdCount; i++ )
			{
				FdPtr = BadFdList[i];
				if ( NULL == FdPtr ) { continue; }				
				PanelIndex = FdPtr->GetFdPanelIndex_Project();
				BoardIndex = FdPtr->GetFdBoardIndex_Panel();
				if ( -1 == BoardIndex )
				{	FdIndex = FdPtr->GetFdIndex_Panel();	}
				else
				{	FdIndex = FdPtr->GetFdIndex_Board();	}
				::_ftprintf(pfile, _T("%u, %u, %u\n"), PanelIndex+1, BoardIndex+1, FdIndex+1);
			}
			::fclose(pfile); pfile = NULL;
			::ShellExecute(NULL, _T("open"), filename, NULL, NULL, SW_SHOW);
		}
		str = _T("Error, Fd Position is out of Stage Limit");
		m_ErrorString = LoadMultiLanguageString(str, str);
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectFdValid(const CAOIFd *RefFdPtr)//確認專案的定位點指標有效
{
	if ( NULL == RefFdPtr ) { return false; }
	size_t i = 0;	
	CAOIFd *FdPtr = NULL;	
	const size_t FdCount = GetProjectFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( RefFdPtr == FdPtr )
		{	return true; }
	}		
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectFdsByStage(const TREGION4D &Rgn, bool ResultMode, std::vector<CAOIFd*> &FdList)//選取定位點
{
	size_t         i = 0;	
	CAOIFd        *FdPtr = NULL;
	const bool     PickSelMode = JetAPI::CheckPickSelectMode(Rgn, 25);
	const size_t   FdCount = GetProjectFdCount_Inline();

	FdList.clear();
	if ( true == PickSelMode )
	{
		TPOINT2D Pt;
		Pt.x = Rgn.GetCpX();
		Pt.y = Rgn.GetCpY();
		for ( i=0; i<FdCount; i++ )
		{
			FdPtr = GetProjectFdPtr_Inline(i);
			if ( NULL == FdPtr ) { continue; }
			if ( FdPtr->GetFdDeleted() == true ) { continue; }			
			if ( FdPtr->CheckFdBePickByStage(Pt) == false ) { continue; }
			FdList.push_back(FdPtr);
		}
	}
	else
	{
		for ( i=0; i<FdCount; i++ )
		{
			FdPtr = GetProjectFdPtr_Inline(i);
			if ( NULL == FdPtr ) { continue; }
			if ( FdPtr->GetFdDeleted() == true ) { continue; }
			if ( FdPtr->CheckFdInRegionByStage(Rgn, true) == false ) { continue; }
			FdList.push_back(FdPtr);
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectFdNeedToCalculate(bool val)//設定專案定位點需要去檢測	
{
	size_t  i = 0;	
	bool    bNeedToCalc = val;
	CAOIFd *FdPtr = NULL;	
	const size_t FdCount = GetProjectFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }		
		FdPtr->SetFdNeedToCalculate(bNeedToCalc);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectBoardFdNeedToCalculateByBarcode()//設定專案單板定位點需要去檢測-依據條碼
{
	size_t  i = 0;		
	CAOIFd *FdPtr = NULL;	
	CAOIBoard *BoardPtr = NULL;
	CAOIBarcode *BarcodePtr = NULL;
	const bool   bNeedToCalc=true;
	const size_t FdCount = GetProjectFdCount();	
	const size_t BoardCount = GetProjectBoardCount();	
	const size_t BarcodeCount = GetProjectBarcodeCount();
	for ( size_t i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->SetBoardTempInt(FN_DISABLE);
	}
	for ( size_t i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeNeedToCalculate() == false ) { continue; }
		BoardPtr = BarcodePtr->GetBarcodeBoardPtr();
		if ( NULL == BoardPtr ) { continue; }		
		if ( BoardPtr->CheckBoardBypassedSkipped() == true ) { continue; }
		BoardPtr->SetBoardTempInt(FN_ENABLE);
	}
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }		
		CAOIBoard *BoardPtr = FdPtr->GetFdBoardPtr();
		if ( NULL == BoardPtr ) { continue; }
		if ( FN_DISABLE == BoardPtr->GetBoardTempInt() )
		{	continue; }

		FdPtr->SetFdNeedCalcMap(bNeedToCalc);
		FdPtr->SetFdNeedToCalculate(bNeedToCalc);
		FdPtr->SetFdNeedToCalculateBackup(false);
		if ( true == bNeedToCalc )
		{	BoardPtr->SetBoardCalcMapFinish(false);	}
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RestoreProjectFdNeedToCalculate()//還原專案定位點需要去檢測
{
	size_t  i = 0;	
	bool    bNeedToCalc = true;
	CAOIFd *FdPtr = NULL;	
	const size_t FdCount = GetProjectFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		bNeedToCalc = FdPtr->GetFdNeedToCalculateBackup();
		FdPtr->SetFdNeedToCalculate(bNeedToCalc);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::BackupProjectFdNeedToCalculate()//備份專案定位點需要去檢測
{
	size_t  i = 0;	
	bool    bNeedToCalc = true;
	CAOIFd *FdPtr = NULL;	
	const size_t FdCount = GetProjectFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		bNeedToCalc = FdPtr->GetFdNeedToCalculate();
		FdPtr->SetFdNeedToCalculateBackup(bNeedToCalc);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectFdUse3DLight_Panel() const//確認專案定位點使用3D燈源-整板
{	
	size_t     i = 0;
	CAOIFd    *FdPtr = NULL;
	CAOIWnd   *WndPtr = NULL;
	CAOIBoard *BoardPtr = NULL;
	CAOIPanel *PanelPtr = NULL;
	unsigned int FrameUniqueID = 0;
	const size_t FdCount = GetProjectFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		PanelPtr = FdPtr->GetFdPanelPtr();
		if ( NULL == PanelPtr ) { continue; }
		BoardPtr = FdPtr->GetFdBoardPtr();
		if ( NULL != BoardPtr ) { continue; }
		WndPtr = FdPtr->GetFdWndPtr();
		if ( NULL == WndPtr ) { continue; }		
		FrameUniqueID = WndPtr->GetWndAlgParam().GetAlgImageBinParam().GetBinaryFrameUniqueID();
		if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID )
		{	return true;	}
	}
	return false;	
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectFdUse3DLight_Board() const//確認專案定位點使用3D燈源-單板
{
	size_t     i = 0;
	CAOIFd    *FdPtr = NULL;
	CAOIWnd   *WndPtr = NULL;
	CAOIBoard *BoardPtr = NULL;
	CAOIPanel *PanelPtr = NULL;
	unsigned int FrameUniqueID = 0;
	const size_t FdCount = GetProjectFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		//PanelPtr = FdPtr->GetFdPanelPtr();
		//if ( NULL == PanelPtr ) { continue; }
		BoardPtr = FdPtr->GetFdBoardPtr();
		if ( NULL == BoardPtr ) { continue; }
		WndPtr = FdPtr->GetFdWndPtr();
		if ( NULL == WndPtr ) { continue; }		
		FrameUniqueID = WndPtr->GetWndAlgParam().GetAlgImageBinParam().GetBinaryFrameUniqueID();
		if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID )
		{	return true;	}
	}
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::UpdateProjectFdToOtherByGroupID(CAOIFd *RefFdPtr)//更新專案定位點至其他定位點-依據群組編號
{	
	if ( NULL == RefFdPtr ) { return false; }
	if ( RefFdPtr->CheckFdGroupIDValid() == false ) { return true; }

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
	CAOIFd    *FdPtr = NULL;	
	CAOIModel  LibraryModel;
	CAOIModel *ModelPtr = NULL;
	CAOIModel *SrcModelPtr = NULL;	
	CAOIModel *RefModelPtr = RefFdPtr->GetFdModelPtr();
	const double RefAngle = RefFdPtr->GetFdAngle();
	const int    RefAngleLabel = JetAPI::GetAngleLabel(RefAngle);
	const int    RefGroupID = RefFdPtr->GetFdGroupID();
	const size_t FdCount = GetProjectFdCount_Inline();	
	
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
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr == RefFdPtr ) { continue; }
		if ( FdPtr->CheckFdGroupIDValid() == false ) { continue; }

		GroupID = FdPtr->GetFdGroupID();		
		if ( GroupID != RefGroupID ) { continue; }		
		FdPtr->UpdateFdModelFromLibrary(&LibraryModel);		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectFdReadyToInspection()//確認定位點準備好檢測	
{
	CAOIProject *ProjectPtr = this;
	if ( AOIDataCollect.CheckProjectPtr(ProjectPtr) == false ) { return false; }

	size_t         i=0,j=0;		
	size_t         WndCount=0;
	size_t         ModelCount=0;
	MODEL_TYPE     ModelType;	
	CString        ModelName;	
	CAOIFd        *FdPtr = NULL;
	CAOIWnd       *WndPtr = NULL;
	CAOIModel     *ModelPtr = NULL;
	CAOIPanel     *PanelPtr = NULL;
	CAOIBoard     *BoardPtr = NULL;		
	std::vector<CString> BadModelNameList;
	std::vector<CAOIFd*> BadFdList;
	const size_t   MaxFdNgCount = 10;
	const size_t   FdCount = ProjectPtr->GetProjectFdCount();

	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = ProjectPtr->GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdDeleted() == true ) { continue; }
		//if ( FdPtr->GetFdBypassed() == true ) { continue; }

		PanelPtr = FdPtr->GetFdPanelPtr();
		if ( NULL != PanelPtr )
		{
			if ( PanelPtr->GetPanelDeleted() == true ) { continue; }
			if ( PanelPtr->GetPanelBypassed() == true ) { continue; }
		}
		BoardPtr = FdPtr->GetFdBoardPtr();
		if ( NULL != BoardPtr )
		{
			if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
			if ( BoardPtr->GetBoardBypassed() == true ) { continue; }
		}

		ModelPtr = FdPtr->GetFdModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		WndCount = ModelPtr->GetModelWndCount();
		if ( 0 == WndCount )
		{ 
			BadFdList.push_back(FdPtr);
			continue; 
		}

		bool bFdOk=true;		
		CString PatternName;
		CString PatternFolder;
		CString PatternFullName;
		unsigned int PatternCount=0;
		for ( j=0; j<WndCount; j++ )
		{
			WndPtr = ModelPtr->GetModelWndPtr(j, false);
			if ( NULL == WndPtr ) { continue; }

			bool bCheckImage=false;
			ALG_TYPE AlgType = WndPtr->GetWndAlgType();
			CAlgParam &AlgParam=WndPtr->GetWndAlgParam();
			if ( ALG_FD_MATCH == AlgType )
			{
				TALG_PARAM_FD_MATCH &fmParam=AlgParam.GetAlgParamFdMatch();
				if ( FD_MATCH_IMAGE == fmParam.fmMatchMode )
				{	bCheckImage = true;	}
			}
			if ( ALG_IMAGE_MATCH == AlgType )
			{	bCheckImage = true;	}

			if ( true == bCheckImage )
			{
				PatternCount = AlgParam.GetAlgPatternCount();
				PatternFolder = AlgParam.GetAlgPatternFolder();
				if ( 0 == PatternCount )
				{	bFdOk = false;	}
				else
				{
					for ( unsigned int k=0; k<PatternCount; k++ )
					{
						PatternName = AOIDataDefine.GetAlgPatternName(k, BOX_TOWARD_RIGHT);	
						PatternFullName.Format(_T("%s\\%s"), PatternFolder, PatternName);
						if ( JetAPI::IsFileExist(PatternFullName) == false )
						{
							bFdOk = false;
							break;
						}
					}
					
				}
			}

			if ( false == bFdOk )
			{	break; }
		}

		if ( false == bFdOk )
		{	BadFdList.push_back(FdPtr);	}
	}

	const size_t BadFdCount = BadFdList.size();
	if ( BadFdCount > 0 ) 
	{
		CString      str;
		CString      strTemp;		
		CString      strName;		
		const size_t ShowCount = MIN(BadFdCount, MaxFdNgCount);
		for ( i=0; i<ShowCount; i++ )
		{
			FdPtr = BadFdList[i];
			if ( NULL == FdPtr ) { continue; }
			strName = FdPtr->GetFdFullName();
			if ( str.GetLength() == 0 ) 
			{	str = strName; }
			else
			{
				strTemp = str;
				str.Format(_T("%s\n%s"), strTemp, strName);
			}
		}
		strTemp = str;
		str = _T("Warning, Fd's wnd not be done");
		str = LoadMultiLanguageString(str, str);
		m_ErrorString.Format(_T("%s\n%s"), str, strTemp);
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::PasteProjectFdToOtherBoards(std::vector<CAOIFd*> &CloneFdList)//將定位點貼上其餘整板單板上	
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
	CAOIFd        *FdPtr = NULL;	
	CAOIFd        *RefFdPtr = NULL;	
	CAOIProject   *ProjectPtr = this;
	CAOIPanel     *PanelPtr = NULL;
	CAOIBoard     *BoardPtr = NULL;	
	CAOIPanel     *RefPanelPtr = NULL;
	CAOIBoard     *RefBoardPtr = NULL;
	CMapCoordinate *MapCTSPtr = NULL;
	CMapCoordinate *PanelMapCTSPtr = NULL;	
	CAOIComponent *OrgComponentPtr = NULL;			
	CAOIComponent *RefOrgComponentPtr = NULL;	
	BOARD_ORIENTATION_MODE BoardOrientMode;
	BOARD_ORIENTATION_MODE RefBoardOrientMode;
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	const size_t   SelFdCount = CloneFdList.size();
	const size_t   PanelCount = GetProjectPanelCount();
	const size_t   BoardCount = GetProjectBoardCount();
	if ( 0 == SelFdCount ) { return true; }
	CString FdFolder = ProjectPtr->GetProjectFdFolder();
	JetAPI::CreateFolder(FdFolder);

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

	for ( i=0; i<SelFdCount; i++ )
	{
		RefFdPtr = CloneFdList[i];
		if ( NULL == RefFdPtr ) { continue; }
		RefPanelPtr = RefFdPtr->GetFdPanelPtr();
		RefBoardPtr = RefFdPtr->GetFdBoardPtr();
		if ( NULL==RefPanelPtr || NULL==RefBoardPtr ) { continue; }
		CAOIModel *RefModelPtr = RefFdPtr->GetFdModelPtr();
		if ( NULL==RefModelPtr ) { continue; }
		CString RefFdModelFolder  = RefModelPtr->GetModelFolder();

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
			FdPtr = RefFdPtr->CloneFdObj();
			if ( NULL == FdPtr ) { continue; }
			
			CadPosX = FdPtr->GetFdCadPosX();
			CadPosY = FdPtr->GetFdCadPosY();
			StagePosX = FdPtr->GetFdStagePosX();
			StagePosY = FdPtr->GetFdStagePosY();
			OrgCadPosX=OrgComponentPtr->GetComponentCadPosX();
			OrgCadPosY=OrgComponentPtr->GetComponentCadPosY();
			RefOrgCadPosX=RefOrgComponentPtr->GetComponentCadPosX();
			RefOrgCadPosY=RefOrgComponentPtr->GetComponentCadPosY();			
			CadOffsetX2 = CadOffsetX = CadPosX-RefOrgCadPosX;
			CadOffsetY2 = CadOffsetY = CadPosY-RefOrgCadPosY;

			FdPtr->SetFdUniqueID(-1);
			ProjectPtr->AddProjectFdPtr(FdPtr, false);
			RefPanelPtr->AddPanelFdPtr(FdPtr);
			RefPanelPtr->SetPanelModified(true);
			BoardPtr->AddBoardFdPtr(FdPtr);			
			BoardPtr->SetBoardModified(true);

			CString FdModelName;
			CAOIModel *ModelPtr = FdPtr->GetFdModelPtr();
			if ( NULL != ModelPtr )
			{
				const int FdUniqueID = FdPtr->GetFdUniqueID();
				CString FdModelFolder = AOIDataDefine.GetFdModelFolder(FdFolder, FdUniqueID);
				JetAPI::CreateFolder(FdModelFolder);	
				JetAPI::ExtractTopFolder(FdModelFolder, FdModelName);	
				ModelPtr->SetModelName(FdModelName);
				ModelPtr->SetModelFolderModel(FdModelFolder);
				ModelPtr->AssignModelFolder();
				ModelPtr->SetupkModelModifiedDateTime();
				JetAPI::CopyFolderAToFolderB(RefFdModelFolder, FdModelFolder, false, true, _T(""), -1, -1);				
			}

			if ( BoardOrientMode!=RefBoardOrientMode )
			{
				JetAPI::CalcBoardOrientationOffset(RefBoardOrientMode, BoardOrientMode, BoardAngle, BoardMirrorXAxis, BoardMirrorYAxis);
				FdPtr->RotateFd(BoardAngle, CadPosX, CadPosY, NULL);
				if ( true == BoardMirrorXAxis )//對X軸鏡射, 變更Y值
				{	FdPtr->MirrorYFd(CadPosY, NULL);	}
				if ( true == BoardMirrorYAxis )//對Y軸鏡射, 變更X值
				{	FdPtr->MirrorXFd(CadPosX, NULL);	}

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
			FdPtr->SetFdCadPosX(NewCadPosX);
			FdPtr->SetFdCadPosY(NewCadPosY);			
			FdPtr->SetFdStagePosX(NewStagePosX);
			FdPtr->SetFdStagePosY(NewStagePosY);
			FdPtr->SetFdTeachStagePosX(NewStagePosX);
			FdPtr->SetFdTeachStagePosY(NewStagePosY);			
			FdPtr->CalcFdCadCornerPos();
			FdPtr->LayoutFdStageCornerPos();
			FdPtr->UpdateFdParamToModel();
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
bool CAOIProject::FilterProjectFdAdded(const std::vector<CAOIFd*> &SelList, const std::vector<CAOIFd*> &SelList2, std::vector<CAOIFd*> &AddList)//建立專案單板定位點新增加
{
	AddList.clear();
	for ( size_t i=0; i<SelList2.size(); i++ )
	{
		CAOIFd *FdPtr=SelList2[i];
		if ( NULL == FdPtr ) { continue; }
		FdPtr->SetFdTempInt(FN_ENABLE);
	}
	for ( size_t i=0; i<SelList.size(); i++ )
	{
		CAOIFd *FdPtr=SelList[i];
		if ( NULL == FdPtr ) { continue; }
		FdPtr->SetFdTempInt(FN_DISABLE);
	}
	for ( size_t i=0; i<SelList2.size(); i++ )
	{
		CAOIFd *FdPtr=SelList2[i];
		if ( NULL == FdPtr ) { continue; }
		if ( FN_DISABLE == FdPtr->GetFdTempInt() ) { continue; }
		AddList.push_back(FdPtr);
	}
	return true;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectFdSortedCount() const//取得專案排序好的專案定位點數量
{
	return GetProjectFdSortedCount_Inline();
}
//---------------------------------------------------------------------------------//
CAOIFd* CAOIProject::GetProjectFdSortedPtr(size_t index, bool check)//取得專案定排序好的位點指標
{
	if ( true == check ) 
	{
		const size_t Count = GetProjectFdSortedCount_Inline();
		if ( index >= Count ) { return NULL; }
	}
	return GetProjectFdSorted_Inline(index);
}
//---------------------------------------------------------------------------------//
CAOIFd* CAOIProject::GetProjectFdLastSortedPtr(DISTRICT_ID DistrictID)//取得專案定排序好的最末定位點指標
{
	size_t       i=0;
	size_t       idx=0;
	CAOIFd      *FdPtr=NULL;
	const size_t FdCount = GetProjectFdSortedCount_Inline();
	BOARD_FD_GRAB_MODE BoardFdGrabMode = GetProjectBoardFdGrabMode(); 
	for ( i=0; i<FdCount; i++ )
	{
		idx = FdCount-i-1;
		FdPtr = GetProjectFdSorted_Inline(idx);
		if ( NULL == FdPtr ) { continue; }
		if ( BOARD_FD_GRAB_INSPECTING == BoardFdGrabMode )
		{
			if ( NULL != FdPtr->GetFdBoardPtr() )
			{	continue; }
		}
		if ( DistrictID == FdPtr->GetFdDistrictID() ) 
		{	return FdPtr; }
	}
	return NULL;
}
//---------------------------------------------------------------------------------//
