// AOIProject_Barcode.cpp: implementation of the CAOIProject class.
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
inline size_t CAOIProject::GetProjectBarcodeCount_Inline() const//取得專案軟體條碼數量
{
	return m_ProjectBarcodePtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::AddProjectBarcodePtr_Inline(CAOIBarcode *BarcodePtr)//增加專案軟體條碼
{
	m_ProjectBarcodePtrList.push_back(BarcodePtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIBarcode* CAOIProject::GetProjectBarcodePtr_Inline(size_t index) const//取得專案軟體條碼指標	
{	
	return m_ProjectBarcodePtrList[index];
}
//-------------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectBarcodeCount() const//取得專案軟體條碼數量
{
	return GetProjectBarcodeCount_Inline();
}
//---------------------------------------------------------------------------------//
CAOIBarcode* CAOIProject::GetProjectBarcodePtr(size_t index, bool check) const//取得專案軟體條碼指標
{
	if ( check )
	{
		const size_t count = GetProjectBarcodeCount_Inline();
		if ( index >= count )
		{	return NULL; }
	}
	return GetProjectBarcodePtr_Inline(index);	
}
//---------------------------------------------------------------------------------//
CAOIBarcode* CAOIProject::AddProjectBarcodePtr(CAOIBarcode *BarcodePtr, bool clone)//增加專案軟體條碼
{
	if ( NULL == BarcodePtr )
	{
		this->m_ErrorString.Format(_T("Error, AddProjectBarcodePtr Fault"));
		return NULL;
	}

	CAOIBarcode *NewBarcodePtr = BarcodePtr;
	const int SBIndex = (int)(GetProjectBarcodeCount_Inline());
	if ( true == clone )
	{
		CAOIPanel        *PanelPtr = BarcodePtr->GetBarcodePanelPtr();
		CAOIBoard        *BoardPtr = BarcodePtr->GetBarcodeBoardPtr();

		NewBarcodePtr = BarcodePtr->CloneBarcodeObj();
		if ( NULL == NewBarcodePtr ) { return NULL; }

		NewBarcodePtr->SetBarcodeIndex_Project(SBIndex);	
		if ( NULL != BoardPtr )
		{	BoardPtr->AddBoardBarcodePtr(NewBarcodePtr); }

		if ( NULL != PanelPtr )
		{	PanelPtr->AddPanelBarcodePtr(NewBarcodePtr); }
	}	
	else
	{	NewBarcodePtr->SetBarcodeIndex_Project(SBIndex); }

	int BarcodeUniqueID = NewBarcodePtr->GetBarcodeUniqueID();
	if ( BarcodeUniqueID < 0 )
	{
		NewBarcodePtr->SetBarcodeUniqueID(m_ProjectBarcodeMaxUniqueID);
		m_ProjectBarcodeMaxUniqueID ++;
	}
	else
	{
		if ( m_ProjectBarcodeMaxUniqueID <= BarcodeUniqueID )
		{	m_ProjectBarcodeMaxUniqueID = BarcodeUniqueID+1; }
	}
	NewBarcodePtr->SetBarcodeProjectPtr(this);
	AddProjectBarcodePtr_Inline(NewBarcodePtr);	
	return NewBarcodePtr;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DestroyProjectBarcodeSelected()//摧毀選取到的專案軟體條碼
{
	size_t i = 0;
	int    index = 0;
	CAOIBarcode *BarcodePtr = NULL;
	std::vector<CAOIBarcode*> ProjectSBPtrList = this->m_ProjectBarcodePtrList;
	
	index = 0;
	this->m_ProjectBarcodePtrList.clear();
	const size_t BarcodeCount = ProjectSBPtrList.size();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = ProjectSBPtrList[i];
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeSelected() == true )
		{
			AOIObjManager.DestroyBarcodeObj(ProjectSBPtrList[i]);			
			BarcodePtr = NULL;
			continue;
		}

		BarcodePtr->SetBarcodeIndex_Project(index);
		CAOIProject::AddProjectBarcodePtr_Inline(BarcodePtr);
		index ++;
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ClearProjectAllBarcodes()//刪除專案軟體條碼
{
	size_t i = 0;
	CAOIBarcode *BarcodePtr = NULL;
	const size_t BarcodeCount = GetProjectBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		AOIObjManager.DestroyBarcodeObj(m_ProjectBarcodePtrList[i]);			
		BarcodePtr = NULL;		
	}		
	this->m_ProjectBarcodePtrList.clear();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::LayoutProjectBarcodeList()//重整專案的軟體條碼列表	
{
	size_t       i = 0;
	unsigned int index = 0;
	CAOIBarcode *BarcodePtr = NULL;
	std::vector<CAOIBarcode*> ProjectSBPtrList = this->m_ProjectBarcodePtrList;
	
	index = 0;
	this->m_ProjectBarcodePtrList.clear();
	const size_t BarcodeCount = ProjectSBPtrList.size();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = ProjectSBPtrList[i];
		if ( NULL == BarcodePtr ) { continue; }

		BarcodePtr->SetBarcodeIndex_Project(index);
		CAOIProject::AddProjectBarcodePtr_Inline(BarcodePtr);
		index ++;
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectAllBarcodes(bool value)//選取專案的所有軟體條碼
{
	size_t i = 0;	
	CAOIBarcode *BarcodePtr = NULL;	
	const size_t BarcodeCount = GetProjectBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		BarcodePtr->SetBarcodeSelected(value);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
CAOIBarcode* CAOIProject::GetProjectBarcodePtrBySelected()//取得專案軟體條碼指標-依據選取到	
{
	size_t i = 0;	
	CAOIBarcode *BarcodePtr = NULL;	
	const size_t BarcodeCount = GetProjectBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeDeleted() == true ) { continue; }
		if ( BarcodePtr->GetBarcodeSelected() == false ) { continue; }
		return BarcodePtr;		
	}		
	return NULL;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectBarcodeEnabledCount() const//取得專案軟體條碼啟用數量
{
	size_t i = 0;	
	size_t Count = 0;
	CAOIBarcode *BarcodePtr = NULL;	
	const size_t BarcodeCount = GetProjectBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeDeleted() == true ) { continue; }
		if ( BarcodePtr->GetBarcodeBypassed() == true ) { continue; }
		Count ++;
	}		
	return Count;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RotateProjectBarcodeSelected(double Angle)//旋轉專案選取到條碼
{
	size_t          i = 0;
	DISTRICT_ID     DistrictID;
	CAOIBoard      *BoardPtr = NULL;
	CAOIPanel      *PanelPtr = NULL;
	CAOIBarcode    *BarcodePtr = NULL;	
	CMapCoordinate *MapCTSPtr = NULL;	
	const size_t    BarcodeCount = GetProjectBarcodeCount_Inline();		
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeDeleted() == true ) { continue; }
		if ( BarcodePtr->GetBarcodeSelected() == false ) { continue; }
		MapCTSPtr = NULL;
		PanelPtr = BarcodePtr->GetBarcodePanelPtr();
		BoardPtr = BarcodePtr->GetBarcodeBoardPtr();
		DistrictID = BarcodePtr->GetBarcodeDistrictID();
		if ( NULL==MapCTSPtr && NULL!=BoardPtr ) 
		{	MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);	}
		if ( NULL==MapCTSPtr && NULL!=PanelPtr ) 
		{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);	}
		BarcodePtr->SpinBarcode(Angle, MapCTSPtr);		
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
bool CAOIProject::GetProjectBarcodeSelected(std::vector<CAOIBarcode*> &SelBarcodeList)//取得專案軟體條碼選取到
{
	size_t i = 0;	
	CAOIBarcode *BarcodePtr = NULL;	
	const size_t BarcodeCount = GetProjectBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeDeleted() == true ) { continue; }
		if ( BarcodePtr->GetBarcodeSelected() == false ) { continue; }
		SelBarcodeList.push_back(BarcodePtr);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectBarcodeSelectedCount() const//計算專案軟體條碼選取到數量
{
	size_t  i = 0;	
	size_t  Count=0;
	CAOIBarcode *BarcodePtr = NULL;	
	const size_t BarcodeCount = GetProjectBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeDeleted() == true ) { continue; }
		if ( BarcodePtr->GetBarcodeSelected() == false ) { continue; }
		Count ++;
	}		
	return Count;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectBarcodeSelected()//刪除選取到的專案軟體條碼
{
	size_t      i=0, j=0, k=0;		
	CAOIPanel   *PanelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;	
	CAOIBarcode *BarcodePtr = NULL;	
	const size_t PanelCount = GetProjectPanelCount();
	const size_t BoardCount = GetProjectBoardCount();	
	const size_t BarcodeCount = GetProjectBarcodeCount_Inline();
	
	ClearProjectAllTempObjects();//刪除所有暫時用的物件
	SelectProjectAllFds(false);
	SelectProjectAllMarks(false);
	SelectProjectAllPanels(false);
	SelectProjectAllBoards(false);
	SelectProjectAllComponents(false);
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeSelected() == false ) { continue; }
		BarcodePtr->SetBarcodeSelected(true);
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

			const size_t NBarcodes = BoardPtr->GetBoardBarcodeCount();
			for ( k=0; k<NBarcodes; k++ )
			{
				BarcodePtr = BoardPtr->GetBoardBarcodePtr(k, false);
				if ( BarcodePtr == NULL ) { continue; }
				if ( BarcodePtr->GetBarcodeSelected() == true )
				{	break; }
			}
			if ( k == NBarcodes ) { continue; }
			BoardPtr->RemoveBoardBarcodeSelected();
			BoardPtr->LayoutBoardRegion();
		}

		const size_t NBarcodes = PanelPtr->GetPanelBarcodeCount();
		for ( k=0; k<NBarcodes; k++ )
		{
			BarcodePtr = PanelPtr->GetPanelBarcodePtr(k, false);
			if ( BarcodePtr == NULL ) { continue; }
			if ( BarcodePtr->GetBarcodeSelected() == true )
			{	break; }
		}
		if ( k == NBarcodes ) { continue; }
		PanelPtr->RemovePanelBarcodeSelected();
		PanelPtr->LayoutPanelRegion();
	}	

	DestroyProjectObjectSelected();
	LayoutProjectAllObjectsIndex();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectBarcodeUnselected()//刪除未選取到的專案軟體條碼
{
	size_t      i=0, j=0, k=0;		
	CAOIPanel   *PanelPtr = NULL;
	CAOIBoard   *BoardPtr = NULL;	
	CAOIBarcode *BarcodePtr = NULL;		
	std::vector<CAOIBarcode*> SelBarcodeList;
	const size_t PanelCount = GetProjectPanelCount();
	const size_t BoardCount = GetProjectBoardCount();	
	const size_t BarcodeCount = GetProjectBarcodeCount_Inline();
	
	ClearProjectAllTempObjects();//刪除所有暫時用的物件
	SelectProjectAllFds(false);
	SelectProjectAllMarks(false);
	SelectProjectAllPanels(false);
	SelectProjectAllBoards(false);
	SelectProjectAllComponents(false);
	GetProjectBarcodeSelected(SelBarcodeList);
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeSelected() == true ) 
		{ 
			BarcodePtr->SetBarcodeSelected(false);
			continue; 
		}
		BarcodePtr->SetBarcodeSelected(true);
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

			const size_t NBarcodes = BoardPtr->GetBoardBarcodeCount();
			for ( k=0; k<NBarcodes; k++ )
			{
				BarcodePtr = BoardPtr->GetBoardBarcodePtr(k, false);
				if ( BarcodePtr == NULL ) { continue; }
				if ( BarcodePtr->GetBarcodeSelected() == true )
				{	break; }
			}
			if ( k == NBarcodes ) { continue; }
			BoardPtr->RemoveBoardBarcodeSelected();
			BoardPtr->LayoutBoardRegion();
		}

		const size_t NBarcodes = PanelPtr->GetPanelBarcodeCount();
		for ( k=0; k<NBarcodes; k++ )
		{
			BarcodePtr = PanelPtr->GetPanelBarcodePtr(k, false);
			if ( BarcodePtr == NULL ) { continue; }
			if ( BarcodePtr->GetBarcodeSelected() == true )
			{	break; }
		}
		if ( k == NBarcodes ) { continue; }
		PanelPtr->RemovePanelBarcodeSelected();
		PanelPtr->LayoutPanelRegion();
	}	
	DestroyProjectObjectSelected();

	const size_t SelBarcodeCount = SelBarcodeList.size();
	for ( i=0; i<SelBarcodeCount; i++ )
	{
		BarcodePtr = SelBarcodeList[i];
		if ( NULL == BarcodePtr ) { continue; }
		BarcodePtr->SetBarcodeSelected(true);
	}	

	LayoutProjectAllObjectsIndex();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectBarcodeValid(const CAOIBarcode *RefBarcodePtr)//確認專案的軟體條碼指標有效
{
	if ( NULL == RefBarcodePtr ) { return false; }
	size_t i = 0;	
	CAOIBarcode *BarcodePtr = NULL;	
	const size_t BarcodeCount = GetProjectBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( RefBarcodePtr == BarcodePtr )
		{	return true; }
	}		
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectBarcodesByStage(const TREGION4D &Rgn, bool ResultMode, std::vector<CAOIBarcode*> &BarcodeList)//選取軟體條碼
{
	size_t         i = 0;	
	CAOIBarcode   *BarcodePtr = NULL;	
	const bool     PickSelMode = JetAPI::CheckPickSelectMode(Rgn, 25);
	const size_t   BarcodeCount = GetProjectBarcodeCount_Inline();

	BarcodeList.clear();
	if ( true == PickSelMode )
	{
		TPOINT2D Pt;
		Pt.x = Rgn.GetCpX();
		Pt.y = Rgn.GetCpY();
		for ( i=0; i<BarcodeCount; i++ )
		{
			BarcodePtr = GetProjectBarcodePtr_Inline(i);
			if ( NULL == BarcodePtr ) { continue; }
			if ( BarcodePtr->GetBarcodeDeleted() == true ) { continue; }			
			if ( BarcodePtr->CheckBarcodeBePickByStage(Pt) == false ) { continue; }
			BarcodeList.push_back(BarcodePtr);
		}
	}
	else
	{
		for ( i=0; i<BarcodeCount; i++ )
		{
			BarcodePtr = GetProjectBarcodePtr_Inline(i);
			if ( NULL == BarcodePtr ) { continue; }
			if ( BarcodePtr->GetBarcodeDeleted() == true ) { continue; }
			if ( BarcodePtr->CheckBarcodeInRegionByStage(Rgn, true) == false ) { continue; }
			BarcodeList.push_back(BarcodePtr);
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::PasteProjectBarcodeToOtherPanels(std::vector<CAOIBarcode*> &CloneBarcodeList)//將軟體條碼貼上其餘整板整板上	
{
	CString        ComponentName;	
	CString        OrgComponentName;
	size_t         i=0, j=0, k=0;
	size_t         u=0, v=0, w=0;	
	size_t         PanelComponentCount=0;	
	size_t         BoardComponentCount=0;
	size_t         RefPanelComponentCount=0;
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
	CAOIBarcode   *BarcodePtr = NULL;	
	CAOIBarcode   *RefBarcodePtr = NULL;	
	CAOIComponent *OrgComponentPtr = NULL;			
	CAOIComponent *RefOrgComponentPtr = NULL;	
	BOARD_ORIENTATION_MODE BoardOrientMode;
	BOARD_ORIENTATION_MODE RefBoardOrientMode;
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	const size_t   SelBarcodeCount = CloneBarcodeList.size();
	const size_t   PanelCount = GetProjectPanelCount();
	const size_t   BoardCount = GetProjectBoardCount();
	if ( 0 == SelBarcodeCount ) { return true; }

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

	
	//貼上其餘整板
	for ( i=0; i<SelBarcodeCount; i++ )
	{
		RefBarcodePtr = CloneBarcodeList[i];
		if ( NULL == RefBarcodePtr ) { continue; }
		RefPanelPtr = RefBarcodePtr->GetBarcodePanelPtr();
		RefBoardPtr = RefBarcodePtr->GetBarcodeBoardPtr();
		if ( NULL==RefPanelPtr || NULL!=RefBoardPtr ) { continue; }

		RefBoardPtr = RefPanelPtr->GetPanelBoardPtr(0, true);		
		PanelMapCTSPtr = RefPanelPtr->GetPanelMapCTSPtr(DistrictID);
		if ( NULL == RefBoardPtr ) 
		{	RefBoardOrientMode = BOARD_ORIENTATION_030; }
		else
		{	RefBoardOrientMode = RefBoardPtr->GetBoardOrientationMode();	}
		RefPanelComponentCount = RefPanelPtr->GetPanelComponentCount();
		for ( j=0; j<PanelCount; j++ )
		{
			PanelPtr = GetProjectPanelPtr(j, false);
			if ( NULL == PanelPtr ) { continue; }
			if ( RefPanelPtr == PanelPtr ) { continue; }			
			MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);
			if ( NULL == MapCTSPtr ) { continue; }

			BoardPtr = PanelPtr->GetPanelBoardPtr(0, true);
			if ( NULL == RefBoardPtr ) 
			{	BoardOrientMode = BOARD_ORIENTATION_030; }
			else
			{	BoardOrientMode = BoardPtr->GetBoardOrientationMode();	}			
			PanelComponentCount = PanelPtr->GetPanelComponentCount();

			//尋找參考零件-作為原點
			OrgComponentPtr = NULL;
			RefOrgComponentPtr = NULL;
			for ( u=0; u<RefPanelComponentCount; u++ )
			{
				RefOrgComponentPtr = RefPanelPtr->GetPanelComponentPtr(u, false);
				if ( NULL == RefOrgComponentPtr ) { continue; }
				if ( RefOrgComponentPtr->GetComponentDeleted() == true ) { continue; }				
				OrgComponentName = RefOrgComponentPtr->GetComponentName();
				OrgComponentPtr = PanelPtr->GetPanelComponentPtrByName(OrgComponentName);
				if ( NULL != OrgComponentPtr )
				{	break; }
			}
			if ( RefPanelComponentCount == u ) { continue; }			
			BarcodePtr = RefBarcodePtr->CloneBarcodeObj();
			if ( NULL == BarcodePtr ) { continue; }

			CadPosX = BarcodePtr->GetBarcodeCadPosX();
			CadPosY = BarcodePtr->GetBarcodeCadPosY();
			StagePosX = BarcodePtr->GetBarcodeStagePosX();
			StagePosY = BarcodePtr->GetBarcodeStagePosY();
			OrgCadPosX=OrgComponentPtr->GetComponentCadPosX();
			OrgCadPosY=OrgComponentPtr->GetComponentCadPosY();
			RefOrgCadPosX=RefOrgComponentPtr->GetComponentCadPosX();
			RefOrgCadPosY=RefOrgComponentPtr->GetComponentCadPosY();			
			CadOffsetX2 = CadOffsetX = CadPosX-RefOrgCadPosX;
			CadOffsetY2 = CadOffsetY = CadPosY-RefOrgCadPosY;

			ProjectPtr->AddProjectBarcodePtr(BarcodePtr, false);
			PanelPtr->AddPanelBarcodePtr(BarcodePtr);
			PanelPtr->SetPanelModified(true);
			if ( BoardOrientMode!=RefBoardOrientMode )
			{
				JetAPI::CalcBoardOrientationOffset(RefBoardOrientMode, BoardOrientMode, BoardAngle, BoardMirrorXAxis, BoardMirrorYAxis);
				BarcodePtr->RotateBarcode(BoardAngle, CadPosX, CadPosY, NULL);
				if ( true == BoardMirrorXAxis )//對X軸鏡射, 變更Y值
				{	BarcodePtr->MirrorYBarcode(CadPosY, NULL);	}
				if ( true == BoardMirrorYAxis )//對Y軸鏡射, 變更X值
				{	BarcodePtr->MirrorXBarcode(CadPosX, NULL);	}

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
			BarcodePtr->SetBarcodeCadPosX(NewCadPosX);
			BarcodePtr->SetBarcodeCadPosY(NewCadPosY);			
			BarcodePtr->SetBarcodeStagePosX(NewStagePosX);
			BarcodePtr->SetBarcodeStagePosY(NewStagePosY);
			BarcodePtr->CalcBarcodeCadCornerPos();
			BarcodePtr->LayoutBarcodeStageCornerPos();
			BarcodePtr->UpdateBarcodeParamToModel();
		}
	}

	//貼上跨整板的單板下
	for ( i=0; i<SelBarcodeCount; i++ )
	{
		RefBarcodePtr = CloneBarcodeList[i];
		if ( NULL == RefBarcodePtr ) { continue; }
		RefPanelPtr = RefBarcodePtr->GetBarcodePanelPtr();
		RefBoardPtr = RefBarcodePtr->GetBarcodeBoardPtr();
		if ( NULL==RefPanelPtr || NULL==RefBoardPtr ) { continue; }

		PanelMapCTSPtr = RefPanelPtr->GetPanelMapCTSPtr(DistrictID);		
		RefBoardOrientMode = RefBoardPtr->GetBoardOrientationMode();
		RefBoardComponentCount = RefBoardPtr->GetBoardComponentCount();
		for ( j=0; j<BoardCount; j++ )
		{
			BoardPtr = GetProjectBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }
			PanelPtr = BoardPtr->GetBoardPanelPtr();
			if ( NULL == PanelPtr ) { continue; }
			if ( RefBoardPtr == BoardPtr ) { continue; }

			MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);
			if ( NULL == MapCTSPtr ) 
			{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);	}
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
			BarcodePtr = RefBarcodePtr->CloneBarcodeObj();
			if ( NULL == BarcodePtr ) { continue; }

			CadPosX = BarcodePtr->GetBarcodeCadPosX();
			CadPosY = BarcodePtr->GetBarcodeCadPosY();
			StagePosX = BarcodePtr->GetBarcodeStagePosX();
			StagePosY = BarcodePtr->GetBarcodeStagePosY();
			OrgCadPosX=OrgComponentPtr->GetComponentCadPosX();
			OrgCadPosY=OrgComponentPtr->GetComponentCadPosY();
			RefOrgCadPosX=RefOrgComponentPtr->GetComponentCadPosX();
			RefOrgCadPosY=RefOrgComponentPtr->GetComponentCadPosY();			
			CadOffsetX2 = CadOffsetX = CadPosX-RefOrgCadPosX;
			CadOffsetY2 = CadOffsetY = CadPosY-RefOrgCadPosY;

			ProjectPtr->AddProjectBarcodePtr(BarcodePtr, false);
			PanelPtr->AddPanelBarcodePtr(BarcodePtr);
			PanelPtr->SetPanelModified(true);
			BoardPtr->AddBoardBarcodePtr(BarcodePtr);			
			BoardPtr->SetBoardModified(true);			

			if ( BoardOrientMode!=RefBoardOrientMode )
			{
				JetAPI::CalcBoardOrientationOffset(RefBoardOrientMode, BoardOrientMode, BoardAngle, BoardMirrorXAxis, BoardMirrorYAxis);
				BarcodePtr->RotateBarcode(BoardAngle, CadPosX, CadPosY, NULL);
				if ( true == BoardMirrorXAxis )//對X軸鏡射, 變更Y值
				{	BarcodePtr->MirrorYBarcode(CadPosY, NULL);	}
				if ( true == BoardMirrorYAxis )//對Y軸鏡射, 變更X值
				{	BarcodePtr->MirrorXBarcode(CadPosX, NULL);	}

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
			BarcodePtr->SetBarcodeCadPosX(NewCadPosX);
			BarcodePtr->SetBarcodeCadPosY(NewCadPosY);			
			BarcodePtr->SetBarcodeStagePosX(NewStagePosX);
			BarcodePtr->SetBarcodeStagePosY(NewStagePosY);
			BarcodePtr->CalcBarcodeCadCornerPos();
			BarcodePtr->LayoutBarcodeStageCornerPos();
			BarcodePtr->UpdateBarcodeParamToModel();
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
bool CAOIProject::PasteProjectBarcodeToOtherBoards(std::vector<CAOIBarcode*> &CloneBarcodeList)//將軟體條碼貼上其餘整板單板上	
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
	CAOIBarcode   *BarcodePtr = NULL;	
	CAOIBarcode   *RefBarcodePtr = NULL;	
	CAOIComponent *OrgComponentPtr = NULL;			
	CAOIComponent *RefOrgComponentPtr = NULL;	
	BOARD_ORIENTATION_MODE BoardOrientMode;
	BOARD_ORIENTATION_MODE RefBoardOrientMode;
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	const size_t   SelBarcodeCount = CloneBarcodeList.size();
	const size_t   PanelCount = GetProjectPanelCount();
	const size_t   BoardCount = GetProjectBoardCount();
	if ( 0 == SelBarcodeCount ) { return true; }

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

	for ( i=0; i<SelBarcodeCount; i++ )
	{
		RefBarcodePtr = CloneBarcodeList[i];
		if ( NULL == RefBarcodePtr ) { continue; }
		RefPanelPtr = RefBarcodePtr->GetBarcodePanelPtr();
		RefBoardPtr = RefBarcodePtr->GetBarcodeBoardPtr();
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
			BarcodePtr = RefBarcodePtr->CloneBarcodeObj();
			if ( NULL == BarcodePtr ) { continue; }

			CadPosX = BarcodePtr->GetBarcodeCadPosX();
			CadPosY = BarcodePtr->GetBarcodeCadPosY();
			StagePosX = BarcodePtr->GetBarcodeStagePosX();
			StagePosY = BarcodePtr->GetBarcodeStagePosY();
			OrgCadPosX=OrgComponentPtr->GetComponentCadPosX();
			OrgCadPosY=OrgComponentPtr->GetComponentCadPosY();
			RefOrgCadPosX=RefOrgComponentPtr->GetComponentCadPosX();
			RefOrgCadPosY=RefOrgComponentPtr->GetComponentCadPosY();			
			CadOffsetX2 = CadOffsetX = CadPosX-RefOrgCadPosX;
			CadOffsetY2 = CadOffsetY = CadPosY-RefOrgCadPosY;

			ProjectPtr->AddProjectBarcodePtr(BarcodePtr, false);
			RefPanelPtr->AddPanelBarcodePtr(BarcodePtr);
			RefPanelPtr->SetPanelModified(true);
			BoardPtr->AddBoardBarcodePtr(BarcodePtr);			
			BoardPtr->SetBoardModified(true);
			if ( BoardOrientMode!=RefBoardOrientMode )
			{
				JetAPI::CalcBoardOrientationOffset(RefBoardOrientMode, BoardOrientMode, BoardAngle, BoardMirrorXAxis, BoardMirrorYAxis);
				BarcodePtr->RotateBarcode(BoardAngle, CadPosX, CadPosY, NULL);
				if ( true == BoardMirrorXAxis )//對X軸鏡射, 變更Y值
				{	BarcodePtr->MirrorYBarcode(CadPosY, NULL);	}
				if ( true == BoardMirrorYAxis )//對Y軸鏡射, 變更X值
				{	BarcodePtr->MirrorXBarcode(CadPosX, NULL);	}

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
			BarcodePtr->SetBarcodeCadPosX(NewCadPosX);
			BarcodePtr->SetBarcodeCadPosY(NewCadPosY);			
			BarcodePtr->SetBarcodeStagePosX(NewStagePosX);
			BarcodePtr->SetBarcodeStagePosY(NewStagePosY);
			BarcodePtr->CalcBarcodeCadCornerPos();
			BarcodePtr->LayoutBarcodeStageCornerPos();
			BarcodePtr->UpdateBarcodeParamToModel();
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
bool CAOIProject::AnalysisProjectBarcodes()//分析專案條碼是否正常
{
	CAOIProject   *ProjectPtr = this;	
	if ( NULL == ProjectPtr ) { return true; }

	CString        str;
	int            BarcodeLen=0;
	CString        strBarcode;	
	RESULT_ID      ResultID;	
	RESULT_ID      ResultID2;
	RESULT_ID      ResultIDGroup;
	RESULT_ID      BoardResultID;
	RESULT_ID      PanelResultID;
	UINT           UIResultID=0;	
	size_t         i=0, j=0, k=0;
	TPOINT3D       BarcodePos;
	std::wstring   Barcode;	
	std::wstring   Barcode2;	
	WND_LOGIC_TYPE WndLogicType;
	WND_LOGIC_TYPE WndLogicType2;
	int            WndLogicGroupID=0;
	int            WndLogicGroupID2=0;
	CAOIWnd       *WndPtr = NULL;
	CAOIPanel     *PanelPtr = NULL;
	CAOIPanel     *PanelPtr2 = NULL;
	CAOIBoard     *BoardPtr = NULL;
	CAOIBoard     *BoardPtr2 = NULL;
	CAOIBarcode   *BarcodePtr = NULL;	
	CAOIBarcode   *BarcodePtr2 = NULL;
	BARCODE_NG_HANDLE_MODE BarcodeNGHandleMode = ProjectPtr->GetProjectParameter().m_BarcodeNGHandleMode;
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	const size_t BoardCount = ProjectPtr->GetProjectBoardCount();
	const size_t BarcodeCount = ProjectPtr->GetProjectBarcodeCount_Inline();
	
	//確認條碼是否正常		
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = ProjectPtr->GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		ResultID = BarcodePtr->GetBarcodeResultID_AOI();
		if ( RESULT_ID_NONE==ResultID || RESULT_ID_OK==ResultID || RESULT_ID_BYPASS==ResultID ||RESULT_ID_SKIP==ResultID) 
		{	
			BarcodePtr->ClearRgnImageBuffer();
			continue; 
		}

		strBarcode = _T("");
		PanelPtr = BarcodePtr->GetBarcodePanelPtr();		
		BoardPtr = BarcodePtr->GetBarcodeBoardPtr();
		if ( NULL != BoardPtr )
		{
			BoardResultID = BoardPtr->GetBoardResultID();
			if ( BoardPtr->GetBoardBypassed()==true || RESULT_ID_BYPASS==BoardResultID ||RESULT_ID_SKIP==BoardResultID ) 
			{
				BarcodePtr->ClearRgnImageBuffer();
				BarcodePtr->SetBarcodeResultID_AOI(RESULT_ID_BYPASS);
				BarcodePtr->SetBarcodeResultID_Alarm(RESULT_ID_BYPASS);				
				continue; 
			}
		}
		if ( NULL != PanelPtr )
		{
			PanelResultID = PanelPtr->GetPanelResultID();
			if ( PanelPtr->GetPanelBypassed()==true || RESULT_ID_BYPASS==PanelResultID ||RESULT_ID_SKIP==PanelResultID) 
			{
				BarcodePtr->ClearRgnImageBuffer();
				BarcodePtr->SetBarcodeResultID_AOI(RESULT_ID_BYPASS);
				BarcodePtr->SetBarcodeResultID_Alarm(RESULT_ID_BYPASS);
				continue; 
			}
		}
		Barcode = BarcodePtr->GetBarcodeResultText();
		if ( BARCODE_NG_HANDLE_PASS == BarcodeNGHandleMode )
		{
			BarcodePtr->ClearRgnImageBuffer();
			continue;
		}

		WndLogicType = BarcodePtr->GetBarcodeLogicType();
		WndLogicGroupID = BarcodePtr->GetBarcodeLogicGroupID();
		if ( WND_LOGIC_NONE != WndLogicType )
		{	
			ResultIDGroup = ResultID;
			for ( j=0; j<BarcodeCount; j++ )
			{	
				BarcodePtr2 = ProjectPtr->GetProjectBarcodePtr_Inline(j);
				if ( NULL == BarcodePtr2 ) { continue; }		
				if ( BarcodePtr2 == BarcodePtr ) { continue; }		
				PanelPtr2 = BarcodePtr2->GetBarcodePanelPtr();		
				BoardPtr2 = BarcodePtr2->GetBarcodeBoardPtr();
				if ( PanelPtr2 != PanelPtr ) { continue; }
				if ( BoardPtr2 != BoardPtr ) { continue; }
				WndLogicType2 = BarcodePtr2->GetBarcodeLogicType();
				if ( WND_LOGIC_NONE == WndLogicType2 ) { continue; }
				WndLogicGroupID2 = BarcodePtr2->GetBarcodeLogicGroupID();
				if ( WndLogicGroupID2 != WndLogicGroupID ) { continue; }
				ResultID2 = BarcodePtr2->GetBarcodeResultID_AOI();			
				if ( RESULT_ID_NONE==ResultID2 || RESULT_ID_BYPASS==ResultID2 || RESULT_ID_SKIP==ResultID2) 
				{	continue;	}
				if ( RESULT_ID_OK==ResultID2 )
				{
					Barcode2 = BarcodePtr2->GetBarcodeResultText();
					ResultIDGroup = RESULT_ID_OK;
					break;	
				}
			}
			if ( RESULT_ID_OK == ResultIDGroup )
			{				
				BarcodePtr->SetBarcodeResultID_AOI(ResultIDGroup);		
				BarcodePtr->SetBarcodeResultID_Alarm(ResultIDGroup);
				BarcodePtr->SetBarcodeResultText(Barcode2.c_str());
				BarcodePtr->ClearRgnImageBuffer();
				continue;
			}
		}

		//如果軌道自動沒有運轉, 才移動XYZ位置, 避免影響進出板
		if ( ProjectPtr->GetProjectConveyerPreRunEnabled() == false )
		{
			BarcodePos = BarcodePtr->GetBarcodeStagePos();
			MotionCtrlPtr->XYMoveTo(BarcodePos.x, BarcodePos.y);
		}
		BarcodePtr->SetBarcodeConfirmUIResultID(0);
		//避免執行緒啟用沒有最上層, 因此改由CMainFrame來創建
		AOIDataCollect.LockUserInput();
		if ( AOIDataCollect.GetIsStopUserInput() == true )
		{ 
			AOIDataCollect.UnlockUserInput();
			return false; 
		}
		AOIDataCollect.SetIsWaitUserInput(true);
		AOIDataCollect.SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_EXEC_BARCODE_CONFIRM_WND , (LPARAM)(BarcodePtr));
		UIResultID = BarcodePtr->GetBarcodeConfirmUIResultID();			
		if ( IDCANCEL == UIResultID )
		{
			m_ErrorString = AOIDataDefine.GetBarcodeText_NG();
			AOIDataCollect.SetIsStopUserInput(true);
			AOIDataCollect.SetIsWaitUserInput(false);
			AOIDataCollect.UnlockUserInput();
			return false;
		}

		AOIDataCollect.SetIsWaitUserInput(false);		
		AOIDataCollect.UnlockUserInput();
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::AssignProjectBarcodeToBoards()//分配專案條碼
{
	CAOIProject   *ProjectPtr = this;		
	RESULT_ID      ResultID;
	std::wstring   Barcode=L"";
	std::wstring   ResultBarcode=L"";	
	size_t         i=0, j=0, k=0;
	size_t         PanelBoardCount = 0;	
	CAOIPanel     *PanelPtr = NULL;
	CAOIBoard     *BoardPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;	
	BARCODE_BELONG_MODE BarcodeBelongMode;
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	const size_t BoardCount = ProjectPtr->GetProjectBoardCount();
	const size_t BarcodeCount = ProjectPtr->GetProjectBarcodeCount_Inline();	

	//將所有專案條碼更新至專案上//目前沒有此流程
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = ProjectPtr->GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		BoardPtr = BarcodePtr->GetBarcodeBoardPtr();
		if ( NULL != BoardPtr ) { continue; }
		PanelPtr = BarcodePtr->GetBarcodePanelPtr();
		if ( NULL != PanelPtr ) { continue; }
		ResultID = BarcodePtr->GetBarcodeResultID_AOI();
		if ( RESULT_ID_OK != ResultID ) { continue; }
		Barcode = BarcodePtr->GetBarcodeResultText();
		ResultBarcode=RemoveProjectBarcodeCharCount(Barcode.c_str());
		ProjectPtr->SetProjectIsGetBarcode(true);
		ProjectPtr->SetProjectBarcode(ResultBarcode.c_str());
		ProjectPtr->SetProjectBarcodeBelongMode(BARCODE_BELONG_PROJECT);
		break;
	}
	
	//將所有專案條碼更新至載具上
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = ProjectPtr->GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		BarcodeBelongMode = BarcodePtr->GetBarcodeBelongMode();
		if ( BARCODE_BELONG_TRAY != BarcodeBelongMode ) { continue; }
		ResultID = BarcodePtr->GetBarcodeResultID_AOI();
		if ( RESULT_ID_OK != ResultID ) { continue; }
		Barcode = BarcodePtr->GetBarcodeResultText();
		ResultBarcode=RemoveProjectBarcodeCharCount(Barcode.c_str());		
		ProjectPtr->SetProjectTrayBarcode(ResultBarcode.c_str());		
		break;
	}
	
	//將所有專案條碼更新至蓋板上
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = ProjectPtr->GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		BarcodeBelongMode = BarcodePtr->GetBarcodeBelongMode();
		if ( BARCODE_BELONG_COVER != BarcodeBelongMode ) { continue; }
		ResultID = BarcodePtr->GetBarcodeResultID_AOI();
		if ( RESULT_ID_OK != ResultID ) { continue; }
		Barcode = BarcodePtr->GetBarcodeResultText();
		ResultBarcode=RemoveProjectBarcodeCharCount(Barcode.c_str());		
		ProjectPtr->SetProjectCoverBarcode(ResultBarcode.c_str());		
		break;
	}

	//將所有整板條碼更新至整板上
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = ProjectPtr->GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }				
		BoardPtr = BarcodePtr->GetBarcodeBoardPtr();
		if ( NULL != BoardPtr ) { continue; }
		ResultID = BarcodePtr->GetBarcodeResultID_AOI();

		if ( RESULT_ID_OK != ResultID ) { continue; }		
		PanelPtr = BarcodePtr->GetBarcodePanelPtr();
		if ( NULL == PanelPtr ) { continue; }
		Barcode = BarcodePtr->GetBarcodeResultText();
		BarcodeBelongMode = BarcodePtr->GetBarcodeBelongMode();
		if ( BARCODE_BELONG_TRAY == BarcodeBelongMode )
		{	continue; }
		if ( BARCODE_BELONG_COVER == BarcodeBelongMode )
		{	continue; }
		ResultBarcode=RemoveProjectBarcodeCharCount(Barcode.c_str());
		if ( BARCODE_BELONG_PROJECT == BarcodeBelongMode )
		{
			if ( ProjectPtr->GetProjectIsGetBarcode() == false )
			{
				ProjectPtr->SetProjectIsGetBarcode(true);
				ProjectPtr->SetProjectBarcode(ResultBarcode.c_str());
				ProjectPtr->SetProjectBarcodeBelongMode(BARCODE_BELONG_PROJECT);
			}
		}
		if ( PanelPtr->GetPanelBarcodeEnabled() == false ) { continue; }		
		if ( BARCODE_BELONG_PROJECT == BarcodeBelongMode )
		{
			if ( PanelPtr->GetPanelIsGetBarcode() == true )
			{	continue; }
		}
		PanelPtr->SetPanelIsGetBarcode(true);
		PanelPtr->SetPanelBarcode(ResultBarcode.c_str());	
		PanelPtr->SetPanelBarcodeBelongMode(BARCODE_BELONG_PANEL);
	}

	
	//將所有整板條碼更新至整板上-其他整板上
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = ProjectPtr->GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		BoardPtr = BarcodePtr->GetBarcodeBoardPtr();
		if ( NULL != BoardPtr ) { continue; }

		if ( BarcodePtr->GetBarcodeSpreadMode() == BARCODE_SPREAD_OFF ) { continue; }
		ResultID = BarcodePtr->GetBarcodeResultID_AOI();
		if ( RESULT_ID_OK != ResultID ) { continue; }		
		PanelPtr = BarcodePtr->GetBarcodePanelPtr();
		if ( NULL == PanelPtr ) { continue; }
		Barcode = BarcodePtr->GetBarcodeResultText();		
		ResultBarcode=RemoveProjectBarcodeCharCount(Barcode.c_str());
		for ( j=0; j<PanelCount; j++ )
		{
			PanelPtr=ProjectPtr->GetProjectPanelPtr(j, false);
			if ( NULL == PanelPtr ) { continue; }			
			if ( PanelPtr->GetPanelBarcodeEnabled() == false ) { continue; }		
			if ( PanelPtr->GetPanelIsGetBarcode() == true ) { continue; }
			PanelPtr->SetPanelIsGetBarcode(true);
			PanelPtr->SetPanelBarcode(ResultBarcode.c_str());	
			PanelPtr->SetPanelBarcodeBelongMode(BARCODE_BELONG_PANEL);
		}
	}

	//將所有單板條碼更新至單板上
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = ProjectPtr->GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		BoardPtr = BarcodePtr->GetBarcodeBoardPtr();
		if ( NULL == BoardPtr ) { continue; }		
		ResultID = BarcodePtr->GetBarcodeResultID_AOI();
		if ( RESULT_ID_OK != ResultID ) { continue; }
		Barcode = BarcodePtr->GetBarcodeResultText();
		BarcodeBelongMode = BarcodePtr->GetBarcodeBelongMode();
		if ( BARCODE_BELONG_TRAY == BarcodeBelongMode )
		{	continue; }
		if ( BARCODE_BELONG_COVER == BarcodeBelongMode )
		{	continue; }
		ResultBarcode=RemoveProjectBarcodeCharCount(Barcode.c_str());
		if ( BARCODE_BELONG_PROJECT == BarcodeBelongMode )
		{
			if ( ProjectPtr->GetProjectIsGetBarcode() == false )
			{
				ProjectPtr->SetProjectIsGetBarcode(true);
				ProjectPtr->SetProjectBarcode(ResultBarcode.c_str());
				ProjectPtr->SetProjectBarcodeBelongMode(BARCODE_BELONG_PROJECT);
			}
		}
		if ( BoardPtr->GetBoardBarcodeEnabled() == false ) { continue; }
		if ( BARCODE_BELONG_PROJECT == BarcodeBelongMode )
		{
			if ( BoardPtr->GetBoardIsGetBarcode() == true )
			{	continue; }
		}
		BoardPtr->SetBoardIsGetBarcode(true);
		BoardPtr->SetBoardBarcode(ResultBarcode.c_str());	
		BoardPtr->SetBoardBarcodeBelongMode(BARCODE_BELONG_BOARD);
	}

	//將所有單板條碼更新至單板上-其他單板上
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = ProjectPtr->GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		BoardPtr = BarcodePtr->GetBarcodeBoardPtr();
		if ( NULL == BoardPtr ) { continue; }
		if ( BarcodePtr->GetBarcodeSpreadMode() == BARCODE_SPREAD_OFF ) { continue; }

		ResultID = BarcodePtr->GetBarcodeResultID_AOI();
		if ( RESULT_ID_OK != ResultID ) { continue; }
		Barcode = BarcodePtr->GetBarcodeResultText();		
		ResultBarcode=RemoveProjectBarcodeCharCount(Barcode.c_str());
		
		PanelPtr = BoardPtr->GetBoardPanelPtr();
		if ( NULL == PanelPtr ) { continue; }
		const size_t PanelBoardCount=PanelPtr->GetPanelBoardCount();
		for ( j=0; j<PanelBoardCount; j++ )
		{
			BoardPtr = PanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }
			if ( BoardPtr->GetBoardBarcodeEnabled() == false ) { continue; }
			if ( BoardPtr->GetBoardIsGetBarcode() == true ) { continue; }
			BoardPtr->SetBoardIsGetBarcode(true);
			BoardPtr->SetBoardBarcode(ResultBarcode.c_str());	
			BoardPtr->SetBoardBarcodeBelongMode(BARCODE_BELONG_BOARD);
		}
	}

	//將所有單板條碼更新至單板上-所有單板上
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = ProjectPtr->GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		BoardPtr = BarcodePtr->GetBarcodeBoardPtr();
		if ( NULL == BoardPtr ) { continue; }
		if ( BarcodePtr->GetBarcodeSpreadMode() != BARCODE_SPREAD_ALL ) { continue; }

		ResultID = BarcodePtr->GetBarcodeResultID_AOI();
		if ( RESULT_ID_OK != ResultID ) { continue; }
		Barcode = BarcodePtr->GetBarcodeResultText();		
		ResultBarcode=RemoveProjectBarcodeCharCount(Barcode.c_str());		
		
		for ( j=0; j<BoardCount; j++ )
		{
			BoardPtr = ProjectPtr->GetProjectBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }
			if ( BoardPtr->GetBoardBarcodeEnabled() == false ) { continue; }
			if ( BoardPtr->GetBoardIsGetBarcode() == true ) { continue; }
			BoardPtr->SetBoardIsGetBarcode(true);
			BoardPtr->SetBoardBarcode(ResultBarcode.c_str());	
			BoardPtr->SetBoardBarcodeBelongMode(BARCODE_BELONG_BOARD);
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ExpandProjectBarcodeToPanels()//擴展專案條碼
{
	CAOIProject   *ProjectPtr = this;
	BARCODE_AUTO_EXPAND_MODE BarcodeAutoExpandMode=ProjectPtr->GetProjectParameter().m_BarcodeAutoExpandMode_Panel;
	if ( BARCODE_AUTO_EXPAND_DISABLE == BarcodeAutoExpandMode ) { return true; }
	
	size_t         i=0;		
	std::wstring   Barcode=L"";
	std::wstring   ResultBarcode=L"";
	CAOIPanel     *PanelPtr = NULL;
	const size_t PanelCount = ProjectPtr->GetProjectPanelCount();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelIsGetBarcode() == false ) { continue; }

		bool bSucc=true;
		Barcode = PanelPtr->GetPanelBarcode();
		switch ( BarcodeAutoExpandMode )
		{
		case BARCODE_AUTO_EXPAND_INCREMENT:
			bSucc = JetAPI::ExpandString_Increment(Barcode, i, ResultBarcode, 10);			
			break;
		case BARCODE_AUTO_EXPAND_ADD_CHAR_1:
			bSucc = JetAPI::ExpandString_AddChar(Barcode, 1, (i+1), ResultBarcode);
			break;
		case BARCODE_AUTO_EXPAND_ADD_CHAR_2:
			bSucc = JetAPI::ExpandString_AddChar(Barcode, 2, (i+1), ResultBarcode);
			break;
		case BARCODE_AUTO_EXPAND_REPLACE_01:
			bSucc = JetAPI::ExpandString_Replace(Barcode, 1, (i+1), ResultBarcode);			
			break;
		case BARCODE_AUTO_EXPAND_REPLACE_02:
			bSucc = JetAPI::ExpandString_Replace(Barcode, 2, (i+1), ResultBarcode);
			break;
		case BARCODE_AUTO_EXPAND_INCREMENT_BASE36:
			bSucc = JetAPI::ExpandString_Increment(Barcode, i, ResultBarcode, 36);
			break;
		}
		if ( false == bSucc )
		{	continue; }
		PanelPtr->SetPanelBarcode(ResultBarcode.c_str());
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ExpandProjectBarcodeToBoards()//擴展專案條碼
{
	CAOIProject   *ProjectPtr = this;
	BARCODE_AUTO_EXPAND_MODE BarcodeAutoExpandMode=ProjectPtr->GetProjectParameter().m_BarcodeAutoExpandMode_Board;
	if ( BARCODE_AUTO_EXPAND_DISABLE == BarcodeAutoExpandMode ) { return true; }
	
	size_t         i=0;		
	std::wstring   Barcode=L"";
	std::wstring   ResultBarcode=L"";
	CAOIBoard     *BoardPtr = NULL;
	const size_t BoardCount = ProjectPtr->GetProjectBoardCount();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = ProjectPtr->GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardIsGetBarcode() == false ) { continue; }

		bool bSucc=true;
		Barcode = BoardPtr->GetBoardBarcode();
		switch ( BarcodeAutoExpandMode )
		{
		case BARCODE_AUTO_EXPAND_INCREMENT:
			bSucc = JetAPI::ExpandString_Increment(Barcode, i, ResultBarcode, 10);			
			break;
		case BARCODE_AUTO_EXPAND_ADD_CHAR_1:
			bSucc = JetAPI::ExpandString_AddChar(Barcode, 1, (i+1), ResultBarcode);
			break;
		case BARCODE_AUTO_EXPAND_ADD_CHAR_2:
			bSucc = JetAPI::ExpandString_AddChar(Barcode, 2, (i+1), ResultBarcode);
			break;
		case BARCODE_AUTO_EXPAND_REPLACE_01:
			bSucc = JetAPI::ExpandString_Replace(Barcode, 1, (i+1), ResultBarcode);			
			break;
		case BARCODE_AUTO_EXPAND_REPLACE_02:
			bSucc = JetAPI::ExpandString_Replace(Barcode, 2, (i+1), ResultBarcode);
			break;
		case BARCODE_AUTO_EXPAND_INCREMENT_BASE36:
			bSucc = JetAPI::ExpandString_Increment(Barcode, i, ResultBarcode, 36);
			break;
		}
		if ( false == bSucc )
		{	continue; }
		BoardPtr->SetBoardBarcode(ResultBarcode.c_str());
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ToggleProjectBarcodeNeedToCalculate()//切換專案條碼需要去檢測
{
	size_t       i = 0;		
	bool        bNeedToCalc = true;
	CAOIBarcode *BarcodePtr = NULL;	
	const size_t BarcodeCount = GetProjectBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeBypassed() == true ) 
		{	
			BarcodePtr->SetBarcodeNeedToCalculate(false);	
			continue;
		}
		if ( BarcodePtr->GetBarcodeNeedToCalculate() == false )
		{	bNeedToCalc = true; }
		else
		{	bNeedToCalc = false; }
		BarcodePtr->SetBarcodeNeedToCalculate(bNeedToCalc);		
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectBarcodeNeedToCalculate(bool val)//設定專案條碼需要去檢測	
{
	size_t       i = 0;	
	bool        bNeedToCalc = val;
	CAOIBarcode *BarcodePtr = NULL;	
	const size_t BarcodeCount = GetProjectBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeBypassed() == true ) 
		{	
			BarcodePtr->SetBarcodeNeedToCalculate(false);	
			continue;
		}
		BarcodePtr->SetBarcodeNeedToCalculate(bNeedToCalc);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RestoreProjectBarcodeNeedToCalculate()//還原專案條碼需要去檢測	
{
	size_t       i = 0;	
	bool        bNeedToCalc = true;
	CAOIBarcode *BarcodePtr = NULL;	
	const size_t BarcodeCount = GetProjectBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		bNeedToCalc = BarcodePtr->GetBarcodeNeedToCalculateBackup();
		BarcodePtr->SetBarcodeNeedToCalculate(bNeedToCalc);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::BackupProjectBarcodeNeedToCalculate()//備份專案條碼需要去檢測	
{
	size_t       i = 0;	
	bool        bNeedToCalc = true;
	CAOIBarcode *BarcodePtr = NULL;	
	const size_t BarcodeCount = GetProjectBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		bNeedToCalc = BarcodePtr->GetBarcodeNeedToCalculate();
		BarcodePtr->SetBarcodeNeedToCalculateBackup(bNeedToCalc);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::UpdateProjectBarcodeToOtherByGroupID(CAOIBarcode *RefBarcodePtr)//更新專案條碼至其他條碼-依據群組編號
{
	if ( NULL == RefBarcodePtr ) { return false; }
	if ( RefBarcodePtr->CheckBarcodeGroupIDValid() == false ) { return true; }

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
	CAOIModel    LibraryModel;
	CAOIModel   *ModelPtr = NULL;
	CAOIModel   *SrcModelPtr = NULL;	
	CAOIBarcode *BarcodePtr = NULL;	
	CAOIModel   *RefModelPtr = RefBarcodePtr->GetBarcodeModelPtr();
	const double RefAngle = RefBarcodePtr->GetBarcodeAngle();
	const int    RefAngleLabel = JetAPI::GetAngleLabel(RefAngle);
	const int    RefGroupID = RefBarcodePtr->GetBarcodeGroupID();
	const int    LocalBasePlaneID = RefBarcodePtr->GetBarcodeLocalBasePlaneID();
	const size_t BarcodeCount = GetProjectBarcodeCount_Inline();	
	
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
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr == RefBarcodePtr ) { continue; }
		if ( BarcodePtr->CheckBarcodeGroupIDValid() == false ) { continue; }

		GroupID = BarcodePtr->GetBarcodeGroupID();		
		if ( GroupID != RefGroupID ) { continue; }
		BarcodePtr->SetBarcodeLocalBasePlaneID(LocalBasePlaneID);
		BarcodePtr->UpdateBarcodeModelFromLibrary(&LibraryModel);		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectBarcodeSelectedLocalBasePlaneID(int BasePlaneID)//設定專案條碼局部基準面編號
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIBarcode *BarcodePtr = NULL;	
	const size_t BarcodeCount = GetProjectBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeDeleted() == true ) { continue; }	
		if ( BarcodePtr->GetBarcodeSelected() == false ) { continue; }		
		BarcodePtr->SetBarcodeLocalBasePlaneID(BasePlaneID);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
