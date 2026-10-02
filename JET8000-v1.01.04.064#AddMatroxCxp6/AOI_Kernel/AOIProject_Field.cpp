// AOIProject_Field.cpp: implementation of the CAOIProject class.
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
#define      SERVER_MODEL_ADD               1//新增模組
#define      SERVER_MODEL_MODIFY            2//修改模組
#define      SERVER_MODEL_DELETE            3//刪除模組
#define      SERVER_MODEL_UNCHANGE          4//不變更模組
//-------------------------------------------------------------------------------------//
inline size_t CAOIProject::GetProjectFieldCount_Inline() const//取得專案區域數量
{
	return m_ProjectFieldPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline CAOIField* CAOIProject::GetProjectFieldPtr_Inline(size_t index) const//取得專案區域指標
{	
	return m_ProjectFieldPtrList[index];
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::ClearProjectProgramField_Inline()//清除專案編程區域列表
{
	m_ProjectProgramFieldPtrList.clear();
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIProject::GetProjectProgramFieldCount_Inline() const//取得專案編程區域數量
{
	return CAOIProject::m_ProjectProgramFieldPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::AddProjectProgramFieldPtr_Inline(CAOIField *FieldPtr)//增加專案編程區域
{
	CAOIProject::m_ProjectProgramFieldPtrList.push_back(FieldPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIField* CAOIProject::GetProjectProgramFieldPtr_Inline(size_t index) const//取得專案編程區域指標
{	
	return m_ProjectProgramFieldPtrList[index];
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::ClearProjectInspectionField_Inline()//清除專案檢測區域列表
{
	m_ProjectInspectionFieldPtrList.clear();
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIProject::GetProjectInspectionFieldCount_Inline() const//取得專案檢測區域數量
{
	return m_ProjectInspectionFieldPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::AddProjectInspectionFieldPtr_Inline(CAOIField *FieldPtr)//增加專案檢測區域
{
	m_ProjectInspectionFieldPtrList.push_back(FieldPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIField* CAOIProject::GetProjectInspectionFieldPtr_Inline(size_t index) const//取得專案檢測區域指標
{	
	return m_ProjectInspectionFieldPtrList[index];
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::ClearProjectInspectionPartField_Inline()//清除專案檢測局部區域列表	
{
	m_ProjectInspectionPartFieldPtrList.clear();
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIProject::GetProjectInspectionPartFieldCount_Inline() const//取得專案檢測局部區域數量
{
	return m_ProjectInspectionPartFieldPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::AddProjectInspectionPartFieldPtr_Inline(CAOIField *FieldPtr)//增加專案檢測局部區域
{
	m_ProjectInspectionPartFieldPtrList.push_back(FieldPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIField* CAOIProject::GetProjectInspectionPartFieldPtr_Inline(size_t index) const//取得專案檢測局部區域指標
{
	return m_ProjectInspectionPartFieldPtrList[index];
}
//-------------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectFieldCount() const//取得專案區域數量
{
	return GetProjectFieldCount_Inline();	
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectFieldCount(DISTRICT_ID DistrictID) const//取得專案區域數量
{
	size_t       i=0;
	size_t       Count=0;
	CAOIField   *FielddPtr = NULL;
	const size_t FlelddCount = GetProjectFieldCount_Inline();
	for ( i=0; i<FlelddCount; i++ )
	{
		FielddPtr = GetProjectFieldPtr_Inline(i);
		if ( NULL == FielddPtr ) { continue; }
		if ( DistrictID != FielddPtr->GetFieldDistrictID() ) { continue; }
		Count ++;
	}
	return Count;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::CalcProjectFieldCountGrab() const//計算專案區域數量要取像
{
	size_t       i=0;
	size_t       Count=0;
	CAOIField   *FielddPtr = NULL;
	const size_t FlelddCount = GetProjectFieldCount_Inline();
	for ( i=0; i<FlelddCount; i++ )
	{
		FielddPtr = GetProjectFieldPtr_Inline(i);
		if ( NULL == FielddPtr ) { continue; }		
		if ( FielddPtr->GetFieldFramePtrCount() == 0 ) { continue; }
		Count ++;
	}
	return Count;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::CalcProjectFieldCountGrab(DISTRICT_ID DistrictID) const//計算專案區域數量要取像
{
	size_t       i=0;
	size_t       Count=0;
	CAOIField   *FielddPtr = NULL;
	const size_t FlelddCount = GetProjectFieldCount_Inline();
	for ( i=0; i<FlelddCount; i++ )
	{
		FielddPtr = GetProjectFieldPtr_Inline(i);
		if ( NULL == FielddPtr ) { continue; }
		if ( DistrictID != FielddPtr->GetFieldDistrictID() ) { continue; }
		if ( FielddPtr->GetFieldFramePtrCount() == 0 ) { continue; }
		Count ++;
	}
	return Count;
}
//---------------------------------------------------------------------------------//
CAOIField* CAOIProject::GetProjectFieldPtr(size_t index, bool check) const//取得專案區域指標
{
	if ( check )
	{
		size_t count = GetProjectFieldCount_Inline();
		if ( index >= count )
		{	return NULL; }
	}
	return GetProjectFieldPtr_Inline(index);	
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ResetProjectObjectFieldPtr()//復歸專案內物件的區域指標
{
	size_t       i=0, j=0, k=0;
	size_t       SubRgnCount = 0;
	CAOIRgn     *SubRgnPtr = NULL;

	CAOIBoard   *BoardPtr = NULL;
	const size_t BoardCount = GetProjectBoardCount();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->SetBoardPanelFieldPtr(NULL, DISTRICT_ID_A);
		BoardPtr->SetBoardPanelFieldPtr(NULL, DISTRICT_ID_B);
	}

	CAOIFd      *FdPtr = NULL;
	const size_t FdCount = GetProjectFdCount();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		FdPtr->SetFdFieldPtr(NULL);		

		SubRgnCount = FdPtr->GetFdSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = FdPtr->GetFdSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->SetRgnFieldPtr(NULL);			
		}		
	}

	CAOIMark    *MarkPtr = NULL;
	const size_t MarkCount = GetProjectMarkCount();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		MarkPtr->SetMarkFieldPtr(NULL);		

		SubRgnCount = MarkPtr->GetMarkSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = MarkPtr->GetMarkSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->SetRgnFieldPtr(NULL);			
		}		
	}

	CAOIBarcode *BarcodePtr = NULL;
	const size_t BarcodeCount = GetProjectBarcodeCount();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		BarcodePtr->SetBarcodeFieldPtr(NULL);		

		SubRgnCount = BarcodePtr->GetBarcodeSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = BarcodePtr->GetBarcodeSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->SetRgnFieldPtr(NULL);			
		}		
	}

	size_t         WindowCount = 0;
	CAOIWindow    *WindowPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ComponentCount = GetProjectComponentCount();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentFieldPtr(NULL);	
		SubRgnCount = ComponentPtr->GetComponentSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = ComponentPtr->GetComponentSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->SetRgnFieldPtr(NULL);			
		}

		WindowCount = ComponentPtr->GetComponentWindowCount();
		for ( j=0; j<WindowCount; j++ )
		{
			WindowPtr = ComponentPtr->GetComponentWindowPtr(j, false);
			if ( NULL == WindowPtr ) { continue; }
			WindowPtr->SetWindowFieldPtr(NULL);
			SubRgnCount = WindowPtr->GetWindowSubRgnCount();
			for ( k=0; k<SubRgnCount; k++ )
			{
				SubRgnPtr = WindowPtr->GetWindowSubRgnPtr(k, false);
				if ( NULL == SubRgnPtr ) { continue; }
				SubRgnPtr->SetRgnFieldPtr(NULL);			
			}
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ResetProjectObjectFieldPtr(DISTRICT_ID DistrictID)//復歸專案內物件的區域指標	
{
	size_t       i=0, j=0, k=0;
	size_t       SubRgnCount = 0;
	CAOIRgn     *SubRgnPtr = NULL;

	CAOIBoard   *BoardPtr = NULL;
	const size_t BoardCount = GetProjectBoardCount();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->SetBoardPanelFieldPtr(NULL, DistrictID);		
	}

	CAOIFd      *FdPtr = NULL;
	const size_t FdCount = GetProjectFdCount();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		FdPtr->SetFdFieldPtr(NULL);		

		SubRgnCount = FdPtr->GetFdSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = FdPtr->GetFdSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->SetRgnFieldPtr(NULL);			
		}		
	}

	CAOIMark    *MarkPtr = NULL;
	const size_t MarkCount = GetProjectMarkCount();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		MarkPtr->SetMarkFieldPtr(NULL);		

		SubRgnCount = MarkPtr->GetMarkSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = MarkPtr->GetMarkSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->SetRgnFieldPtr(NULL);			
		}		
	}

	CAOIBarcode *BarcodePtr = NULL;
	const size_t BarcodeCount = GetProjectBarcodeCount();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		BarcodePtr->SetBarcodeFieldPtr(NULL);		

		SubRgnCount = BarcodePtr->GetBarcodeSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = BarcodePtr->GetBarcodeSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->SetRgnFieldPtr(NULL);			
		}		
	}

	size_t         WindowCount = 0;
	CAOIWindow    *WindowPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ComponentCount = GetProjectComponentCount();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }

		ComponentPtr->SetComponentFieldPtr(NULL);	
		SubRgnCount = ComponentPtr->GetComponentSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = ComponentPtr->GetComponentSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->SetRgnFieldPtr(NULL);			
		}

		WindowCount = ComponentPtr->GetComponentWindowCount();
		for ( j=0; j<WindowCount; j++ )
		{
			WindowPtr = ComponentPtr->GetComponentWindowPtr(j, false);
			if ( NULL == WindowPtr ) { continue; }
			WindowPtr->SetWindowFieldPtr(NULL);
			SubRgnCount = WindowPtr->GetWindowSubRgnCount();
			for ( k=0; k<SubRgnCount; k++ )
			{
				SubRgnPtr = WindowPtr->GetWindowSubRgnPtr(k, false);
				if ( NULL == SubRgnPtr ) { continue; }
				SubRgnPtr->SetRgnFieldPtr(NULL);			
			}
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::BackupProjectObjectFieldIndex()//備份專案內物件的區域引數
{
	size_t       i=0, j=0, k=0;
	size_t       SubRgnCount = 0;
	CAOIRgn     *SubRgnPtr = NULL;

	CAOIFd      *FdPtr = NULL;
	const size_t FdCount = GetProjectFdCount();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		FdPtr->BackupRgnFieldIndex();		

		SubRgnCount = FdPtr->GetFdSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = FdPtr->GetFdSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->BackupRgnFieldIndex(); 
		}		
	}

	CAOIMark    *MarkPtr = NULL;
	const size_t MarkCount = GetProjectMarkCount();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		MarkPtr->BackupRgnFieldIndex();

		SubRgnCount = MarkPtr->GetMarkSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = MarkPtr->GetMarkSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->BackupRgnFieldIndex(); 
		}		
	}

	CAOIBarcode *BarcodePtr = NULL;
	const size_t BarcodeCount = GetProjectBarcodeCount();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		BarcodePtr->BackupRgnFieldIndex();
		
		SubRgnCount = BarcodePtr->GetBarcodeSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = BarcodePtr->GetBarcodeSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->BackupRgnFieldIndex(); 
		}		
	}

	size_t         WindowCount = 0;
	CAOIWindow    *WindowPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ComponentCount = GetProjectComponentCount();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->BackupRgnFieldIndex();
		
		SubRgnCount = ComponentPtr->GetComponentSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = ComponentPtr->GetComponentSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->BackupRgnFieldIndex(); 
		}

		WindowCount = ComponentPtr->GetComponentWindowCount();
		for ( j=0; j<WindowCount; j++ )
		{
			WindowPtr = ComponentPtr->GetComponentWindowPtr(j, false);
			if ( NULL == WindowPtr ) { continue; }
			WindowPtr->BackupRgnFieldIndex();
			
			SubRgnCount = WindowPtr->GetWindowSubRgnCount();
			for ( k=0; k<SubRgnCount; k++ )
			{
				SubRgnPtr = WindowPtr->GetWindowSubRgnPtr(k, false);
				if ( NULL == SubRgnPtr ) { continue; }
				SubRgnPtr->BackupRgnFieldIndex(); 
			}
		}
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RestoreProjectObjectFieldIndex()//還原專案內物件的區域引數
{
	size_t       i=0, j=0, k=0;
	size_t       SubRgnCount = 0;
	CAOIRgn     *SubRgnPtr = NULL;

	CAOIFd      *FdPtr = NULL;
	const size_t FdCount = GetProjectFdCount();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		FdPtr->RestoreRgnFieldIndex();		

		SubRgnCount = FdPtr->GetFdSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = FdPtr->GetFdSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->RestoreRgnFieldIndex(); 
		}		
	}

	CAOIMark    *MarkPtr = NULL;
	const size_t MarkCount = GetProjectMarkCount();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		MarkPtr->RestoreRgnFieldIndex();

		SubRgnCount = MarkPtr->GetMarkSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = MarkPtr->GetMarkSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->RestoreRgnFieldIndex(); 
		}		
	}

	CAOIBarcode *BarcodePtr = NULL;
	const size_t BarcodeCount = GetProjectBarcodeCount();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		BarcodePtr->RestoreRgnFieldIndex();
		
		SubRgnCount = BarcodePtr->GetBarcodeSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = BarcodePtr->GetBarcodeSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->RestoreRgnFieldIndex(); 
		}		
	}

	size_t         WindowCount = 0;
	CAOIWindow    *WindowPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ComponentCount = GetProjectComponentCount();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->RestoreRgnFieldIndex();
		
		SubRgnCount = ComponentPtr->GetComponentSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = ComponentPtr->GetComponentSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->RestoreRgnFieldIndex(); 
		}

		WindowCount = ComponentPtr->GetComponentWindowCount();
		for ( j=0; j<WindowCount; j++ )
		{
			WindowPtr = ComponentPtr->GetComponentWindowPtr(j, false);
			if ( NULL == WindowPtr ) { continue; }
			WindowPtr->RestoreRgnFieldIndex();
			
			SubRgnCount = WindowPtr->GetWindowSubRgnCount();
			for ( k=0; k<SubRgnCount; k++ )
			{
				SubRgnPtr = WindowPtr->GetWindowSubRgnPtr(k, false);
				if ( NULL == SubRgnPtr ) { continue; }
				SubRgnPtr->RestoreRgnFieldIndex(); 
			}
		}
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectObjectFieldPtrByIndex()//設定專案內物件的區域指標以引數為主
{
	CAOIField   *FieldPtr=NULL;
	unsigned int FieldIndex=-1;
	size_t       i=0, j=0, k=0;
	size_t       SubRgnCount = 0;
	CAOIRgn     *SubRgnPtr = NULL;

	CAOIFd      *FdPtr = NULL;
	const size_t FdCount = GetProjectFdCount();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		FieldIndex = FdPtr->GetRgnFieldIndex();
		FieldPtr = GetProjectInspectionFieldPtr(FieldIndex, true);
		if ( NULL!=FieldPtr && FieldPtr->GetFieldDistrictID()==FdPtr->GetFdDistrictID() )
		{	FdPtr->SetFdFieldPtr(FieldPtr);	}		

		SubRgnCount = FdPtr->GetFdSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = FdPtr->GetFdSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			FieldIndex = SubRgnPtr->GetRgnFieldIndex();
			FieldPtr = GetProjectInspectionFieldPtr(FieldIndex, true);
			if ( NULL!=FieldPtr && FieldPtr->GetFieldDistrictID()==SubRgnPtr->GetRgnDistrictID() )
			{	SubRgnPtr->SetRgnFieldPtr(FieldPtr);	}
		}		
	}

	CAOIMark    *MarkPtr = NULL;
	const size_t MarkCount = GetProjectMarkCount();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		FieldIndex = MarkPtr->GetRgnFieldIndex();
		FieldPtr = GetProjectInspectionFieldPtr(FieldIndex, true);
		if ( NULL!=FieldPtr && FieldPtr->GetFieldDistrictID()==MarkPtr->GetMarkDistrictID() )
		{	MarkPtr->SetMarkFieldPtr(FieldPtr);	}	

		SubRgnCount = MarkPtr->GetMarkSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = MarkPtr->GetMarkSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			FieldIndex = SubRgnPtr->GetRgnFieldIndex();
			FieldPtr = GetProjectInspectionFieldPtr(FieldIndex, true);
			if ( NULL!=FieldPtr && FieldPtr->GetFieldDistrictID()==SubRgnPtr->GetRgnDistrictID() )
			{	SubRgnPtr->SetRgnFieldPtr(FieldPtr);	}
		}		
	}

	CAOIBarcode *BarcodePtr = NULL;
	const size_t BarcodeCount = GetProjectBarcodeCount();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		FieldIndex = BarcodePtr->GetRgnFieldIndex();
		FieldPtr = GetProjectInspectionFieldPtr(FieldIndex, true);
		if ( NULL!=FieldPtr && FieldPtr->GetFieldDistrictID()==BarcodePtr->GetBarcodeDistrictID() )
		{	BarcodePtr->SetBarcodeFieldPtr(FieldPtr);	}	

		SubRgnCount = BarcodePtr->GetBarcodeSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = BarcodePtr->GetBarcodeSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			FieldIndex = SubRgnPtr->GetRgnFieldIndex();
			FieldPtr = GetProjectInspectionFieldPtr(FieldIndex, true);
			if ( NULL!=FieldPtr && FieldPtr->GetFieldDistrictID()==SubRgnPtr->GetRgnDistrictID() )
			{	SubRgnPtr->SetRgnFieldPtr(FieldPtr);	}
		}		
	}

	size_t         WindowCount = 0;
	CAOIWindow    *WindowPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ComponentCount = GetProjectComponentCount();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		FieldIndex = ComponentPtr->GetRgnFieldIndex();
		FieldPtr = GetProjectInspectionFieldPtr(FieldIndex, true);
		if ( NULL!=FieldPtr && FieldPtr->GetFieldDistrictID()==ComponentPtr->GetComponentDistrictID() )
		{	ComponentPtr->SetComponentFieldPtr(FieldPtr);	}		

		SubRgnCount = ComponentPtr->GetComponentSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = ComponentPtr->GetComponentSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			FieldIndex = SubRgnPtr->GetRgnFieldIndex();
			FieldPtr = GetProjectInspectionFieldPtr(FieldIndex, true);
			if ( NULL!=FieldPtr && FieldPtr->GetFieldDistrictID()==SubRgnPtr->GetRgnDistrictID() )
			{	SubRgnPtr->SetRgnFieldPtr(FieldPtr);	}
		}

		WindowCount = ComponentPtr->GetComponentWindowCount();
		for ( j=0; j<WindowCount; j++ )
		{
			WindowPtr = ComponentPtr->GetComponentWindowPtr(j, false);
			if ( NULL == WindowPtr ) { continue; }
			FieldIndex = WindowPtr->GetRgnFieldIndex();
			FieldPtr = GetProjectInspectionFieldPtr(FieldIndex, true);
			if ( NULL!=FieldPtr && FieldPtr->GetFieldDistrictID()==ComponentPtr->GetComponentDistrictID() )
			{	WindowPtr->SetWindowFieldPtr(FieldPtr);	}		

			SubRgnCount = WindowPtr->GetWindowSubRgnCount();
			for ( k=0; k<SubRgnCount; k++ )
			{
				SubRgnPtr = WindowPtr->GetWindowSubRgnPtr(k, false);
				if ( NULL == SubRgnPtr ) { continue; }
				FieldIndex = SubRgnPtr->GetRgnFieldIndex();
				FieldPtr = GetProjectInspectionFieldPtr(FieldIndex, true);
				if ( NULL!=FieldPtr && FieldPtr->GetFieldDistrictID()==SubRgnPtr->GetRgnDistrictID() )
				{	SubRgnPtr->SetRgnFieldPtr(FieldPtr);	}
			}
		}
	}	
	return true;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectProgramFieldCount() const//取得專案編程區域數量
{
	return GetProjectProgramFieldCount_Inline();
}
//---------------------------------------------------------------------------------//
CAOIField* CAOIProject::GetProjectProgramFieldPtr(size_t index, bool check) const//取得專案編程區域指標
{
	if ( check )
	{
		const size_t count = GetProjectProgramFieldCount_Inline();
		if ( index >= count )
		{	return NULL; }
	}
	return GetProjectProgramFieldPtr_Inline(index);	
}
//---------------------------------------------------------------------------------//
CAOIField* CAOIProject::AddProjectProgramFieldPtr(CAOIField *FieldPtr, bool clone)//增加專案編程區域
{
	if ( NULL == FieldPtr )
	{
		this->m_ErrorString.Format(_T("Error, AddProjectProgramFieldPtr Fault"));
		return NULL;
	}

	CAOIField *NewFieldPtr = FieldPtr;
	const unsigned int FieldIndex = (unsigned int)(GetProjectProgramFieldCount_Inline());
	if ( true == clone )
	{
		NewFieldPtr = FieldPtr->CloneFieldObj();
		if ( NewFieldPtr == NULL ) { return NULL; }		
		NewFieldPtr->SetFieldIndex(FieldIndex);			
	}	
	else
	{	NewFieldPtr->SetFieldIndex(FieldIndex);	 }
	NewFieldPtr->SetFieldListMode(FIELD_LIST_PROGRAM);
	CAOIProject::AddProjectProgramFieldPtr_Inline(NewFieldPtr);		
	return NewFieldPtr;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ClearProjectAllProgramField()//刪除專案編程區域
{
	size_t     i=0, j=0;	
	size_t     FrameCount=0;
	CAOIField *FieldPtr = NULL;	
	CAOIFrame *FramePtr = NULL;
	const size_t FieldCount = GetProjectProgramFieldCount_Inline();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectProgramFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }		
		FieldPtr->ClearFieldAllFrames();

		AOIObjManager.DestroyFieldObj(m_ProjectProgramFieldPtrList[i]);
		FieldPtr = NULL;		
	}			
	ClearProjectProgramField_Inline();
	SetProjectProgramFieldBufferReleased(true);
	ResetProjectObjectFieldPtr();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::LayoutProjectProgramFieldList()//重整專案編程區域列表
{
	size_t       i=0, j=0;
	size_t       FrameCount=0;
	unsigned int index = 0;
	CAOIField *FieldPtr = NULL;
	CAOIFrame *FramePtr = NULL;
	std::vector<CAOIField*> ProjectFieldPtrList = this->m_ProjectProgramFieldPtrList;
	
	index = 0;
	ClearProjectProgramField_Inline();
	const size_t FieldCount = ProjectFieldPtrList.size();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = ProjectFieldPtrList[i];
		if ( NULL == FieldPtr ) { continue; }
		FieldPtr->SetFieldIndex(index);

		FrameCount = FieldPtr->GetFieldFramePtrCount();
		for ( j=0; j<FrameCount; j++ )
		{
			FramePtr = FieldPtr->GetFieldFramePtr(j, false);
			if ( NULL == FramePtr ) { continue; }
			FramePtr->SetFrameFieldIndex(index);
		}

		CAOIProject::AddProjectProgramFieldPtr_Inline(FieldPtr);
		index ++;
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectProgramFieldValid(const CAOIField *RefFieldPtr)//確認專案編程區域指標有效
{
	if ( NULL == RefFieldPtr ) { return false; }
	size_t     i=0;		
	CAOIField *FieldPtr = NULL;		
	const size_t FieldCount = GetProjectProgramFieldCount_Inline();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectProgramFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }		
		if ( RefFieldPtr == FieldPtr ) 
		{	return true; }
	}			
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ReleaseProjectProgramFieldFrameImageBuffer()//釋放專案編程區域影像記憶體
{
	size_t     i=0, j=0;		
	size_t     FrameCount=0;
	CAOIFrame *FramePtr = NULL;
	CAOIField *FieldPtr = NULL;		
	const size_t FieldCount = GetProjectProgramFieldCount_Inline();
	AOIDataCollect.SaveMovingTimeMsg(_T("CAOIProject::ReleaseProjectProgramFieldFrameImageBuffer Start"));
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectProgramFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }		
		FrameCount = FieldPtr->GetFieldFramePtrCount();
		for ( j=0; j<FrameCount; j++ )
		{
			FramePtr = FieldPtr->GetFieldFramePtr(j, false);
			if ( NULL == FramePtr ) { continue; }
			FramePtr->ClearFrameBuffer();
			FramePtr->SetFrameCalcState(FRAME_CALC_NONE);
			FramePtr->SetFrameMergeState(FRAME_MERGE_NONE);
		}
	}		
	SetProjectProgramFieldBufferReleased(true);
	AOIDataCollect.SaveMovingTimeMsg(_T("CAOIProject::ReleaseProjectProgramFieldFrameImageBuffer End"));
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::GetProjectProgramFieldDoNotSave() const//取得專案編程區域是否不要存檔
{
	return m_ProjectProgramFieldDoNotSave;
}
//---------------------------------------------------------------------------------//
void CAOIProject::SetProjectProgramFieldDoNotSave(bool NoSave)//設定專案編程區域是否強迫存檔
{
	m_ProjectProgramFieldDoNotSave = NoSave;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectProgramFieldMustToLoad(bool ToLoad)//設定專案編程區域是否強迫載入圖檔
{
	size_t i=0;	
	CAOIField *FieldPtr = NULL;	
	const size_t ProgramFieldCount = GetProjectProgramFieldCount_Inline();	
	for ( i=0; i<ProgramFieldCount; i++ )
	{
		FieldPtr = GetProjectProgramFieldPtr_Inline(i);		
		if ( NULL == FieldPtr ) { continue; }
		FieldPtr->SetFieldMustToLoad(ToLoad);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectInspectionFieldMustToLoad(bool ToLoad)//設定專案檢測區域是否強迫載入圖檔
{
	size_t i=0;	
	CAOIField *FieldPtr = NULL;		
	const size_t InspectionFieldCount = GetProjectInspectionFieldCount_Inline();	
	for ( i=0; i<InspectionFieldCount; i++ )
	{	
		FieldPtr = GetProjectInspectionFieldPtr_Inline(i);		
		if ( NULL == FieldPtr ) { continue; }
		FieldPtr->SetFieldMustToLoad(ToLoad);
	}
	return true;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectInspectionFieldCount() const//取得專案檢測區域數量
{
	return GetProjectInspectionFieldCount_Inline();
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectInspectionFieldCount(DISTRICT_ID DistrictID) const//取得專案檢測區域數量
{
	size_t       i=0;
	size_t       Count=0;
	CAOIField   *FieldPtr = NULL;
	const size_t FieldCount = GetProjectInspectionFieldCount_Inline();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectInspectionFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		if ( DistrictID != FieldPtr->GetFieldDistrictID() ) { continue; }
		Count ++;
	}
	return Count;
}
//---------------------------------------------------------------------------------//
CAOIField* CAOIProject::GetProjectInspectionFieldPtr(size_t index, bool check) const//取得專案檢測區域指標
{
	if ( check )
	{
		const size_t count = GetProjectInspectionFieldCount_Inline();
		if ( index >= count )
		{	return NULL; }
	}
	return GetProjectInspectionFieldPtr_Inline(index);	
}
//---------------------------------------------------------------------------------//
CAOIField* CAOIProject::AddProjectInspectionFieldPtr(CAOIField *FieldPtr, bool clone)//增加專案檢測區域
{
	if ( NULL == FieldPtr )
	{
		this->m_ErrorString.Format(_T("Error, AddProjectInspectionFieldPtr Fault"));
		return NULL;
	}

	CAOIField *NewFieldPtr = FieldPtr;
	const unsigned int FieldIndex = (unsigned int)(GetProjectInspectionFieldCount_Inline());
	if ( true == clone )
	{
		NewFieldPtr = FieldPtr->CloneFieldObj();
		if ( NewFieldPtr == NULL ) { return NULL; }		
		NewFieldPtr->SetFieldIndex(FieldIndex);			
	}	
	else
	{	NewFieldPtr->SetFieldIndex(FieldIndex);	 }
	NewFieldPtr->SetFieldListMode(FIELD_LIST_INSPECTION);
	AddProjectInspectionFieldPtr_Inline(NewFieldPtr);		
	return NewFieldPtr;
}
//---------------------------------------------------------------------------------//
void CAOIProject::GetProjectInspectionFileList(std::vector<CAOIField*> &FieldList)//取得專案檢測區域列表
{
	FieldList = m_ProjectInspectionFieldPtrList;		
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectInspectionFileList(std::vector<CAOIField*> &FieldList)//設定專案檢測區域列表
{	
	CAOIProject   *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t         i=0, j=0;
	unsigned int   PanelIndex=0;
	unsigned int   BoardIndex=0;
	unsigned int   ComponentIndex=0;	
	CAOIPanel     *PanelPtr=NULL;
	CAOIBoard     *BoardPtr=NULL;
	CAOIComponent *ComponentPtr=NULL;
	CAOIField     *SrcFieldPtr = NULL;
	CAOIField     *DstFieldPtr = NULL;
	const size_t   FieldCount = FieldList.size();

	ProjectPtr->ClearProjectAllInspectionField();
	for ( i=0; i<FieldCount; i++ )
	{
		SrcFieldPtr = FieldList[i];
		if ( NULL == SrcFieldPtr ) { continue; }		
		SrcFieldPtr->ClearFieldAllFrames();//記得清楚影像列表, 否則會共用
		DstFieldPtr = SrcFieldPtr->CloneFieldObj();
		if ( NULL == DstFieldPtr ) { continue; }

		PanelIndex = DstFieldPtr->GetFieldPanelIndex();
		BoardIndex = DstFieldPtr->GetFieldBoardIndex();
		ComponentIndex = DstFieldPtr->GetFieldComponentIndex();
		if ( -1 != ComponentIndex ) 
		{	//零件的專屬檢測區域
			ComponentPtr = ProjectPtr->GetProjectComponentPtr(ComponentIndex, true);
			if ( NULL == ComponentPtr ) 
			{
				DstFieldPtr->ClearFieldAllFrames();
				AOIObjManager.DestroyFieldObj(DstFieldPtr);
				continue;
			}
			ComponentPtr->SetComponentSelfFieldPtr(DstFieldPtr);
		}
		if ( -1 != BoardIndex ) 
		{	//單板的檢測區域
			BoardPtr = ProjectPtr->GetProjectBoardPtr(BoardIndex, true);
			if ( NULL == BoardPtr ) 
			{
				DstFieldPtr->ClearFieldAllFrames();
				AOIObjManager.DestroyFieldObj(DstFieldPtr);
				continue;
			}
			BoardPtr->AddBoardFieldPtr(DstFieldPtr);
		}
		if ( -1 != PanelIndex ) 
		{	//整板的檢測區域
			PanelPtr = ProjectPtr->GetProjectPanelPtr(PanelIndex, true);
			if ( NULL == PanelPtr ) 
			{
				DstFieldPtr->ClearFieldAllFrames();
				AOIObjManager.DestroyFieldObj(DstFieldPtr);
				continue;
			}
			PanelPtr->AddPanelFieldPtr(DstFieldPtr);
		}
		//專案等間距的檢測區域
		ProjectPtr->AddProjectInspectionFieldPtr_Inline(DstFieldPtr);	
	}
	SetProjectOnlineTuningFolder(_T(""));
	SetProjectInspectionFieldBufferReleased(true);
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ModifyProjectInspectionFileList(const std::vector<CAOIField*> &FieldList)//修改專案檢測區域列表
{
	CAOIProject   *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t         i=0;	
	size_t         GrabIndex=0;	
	CAOIField     *SrcFieldPtr = NULL;
	CAOIField     *DstFieldPtr = NULL;
	const size_t   FieldCount = FieldList.size();
	const size_t   ProgFieldCount = ProjectPtr->GetProjectInspectionFieldCount();
	if ( FieldCount != ProgFieldCount )
	{	return false; }

	for ( i=0; i<ProgFieldCount; i++ )
	{
		SrcFieldPtr = FieldList[i];
		DstFieldPtr = ProjectPtr->GetProjectInspectionFieldPtr_Inline(i);
		if ( NULL == SrcFieldPtr ) { continue; }
		if ( NULL == DstFieldPtr ) { continue; }
		GrabIndex = SrcFieldPtr->GetFieldGrabIndexByUser();
		DstFieldPtr->SetFieldGrabIndexByUser(GrabIndex);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ClearProjectAllInspectionField()//刪除專案檢測區域
{
	size_t     i=0, j=0, k=0;	
	size_t     SubRgnCount = 0;
	size_t     FrameCount=0;
	CAOIRgn   *SubRgnPtr = NULL;
	CAOIField *FieldPtr = NULL;	
	CAOIFrame *FramePtr = NULL;	

	//注意, 檢測的Field是與整板的Field相連的
	CAOIPanel *PanelPtr = NULL;
	const size_t PanelCount = GetProjectPanelCount();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		PanelPtr->RemovePanelAllFields();
	}

	CAOIBoard *BoardPtr = NULL;
	const size_t BoardCount = GetProjectBoardCount();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->RemoveBoardAllFields();
	}

	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = GetProjectComponentCount();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentSelfFieldPtr(NULL);
	}

	const size_t FieldCount = GetProjectInspectionFieldCount_Inline();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectInspectionFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }

		FieldPtr->ClearFieldAllFrames();
		AOIObjManager.DestroyFieldObj(m_ProjectInspectionFieldPtrList[i]);
		FieldPtr = NULL;		
	}		
	ClearProjectInspectionField_Inline();	

	ResetProjectObjectFieldPtr();
	SetProjectInspectionFieldBufferReleased(true);
	m_ProjectFieldPtrList.clear();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ClearProjectAllInspectionField(DISTRICT_ID DistrictID)//刪除專案檢測區域	
{
	size_t     i=0, j=0, k=0;	
	size_t     SubRgnCount = 0;
	size_t     FrameCount=0;
	CAOIRgn   *SubRgnPtr = NULL;
	CAOIField *FieldPtr = NULL;	
	CAOIFrame *FramePtr = NULL;	

	//注意, 檢測的Field是與整板的Field相連的
	CAOIPanel *PanelPtr = NULL;
	const size_t PanelCount = GetProjectPanelCount();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		PanelPtr->RemovePanelAllFields(DistrictID);
	}

	CAOIBoard *BoardPtr = NULL;
	const size_t BoardCount = GetProjectBoardCount();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }		
		BoardPtr->RemoveBoardAllFields(DistrictID);
	}

	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = GetProjectComponentCount();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		ComponentPtr->SetComponentSelfFieldPtr(NULL);
	}

	std::vector<CAOIField*> FieldPtrList;
	const size_t FieldCount = GetProjectInspectionFieldCount_Inline();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectInspectionFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		if ( DistrictID != FieldPtr->GetFieldDistrictID() )
		{
			FieldPtrList.push_back(FieldPtr);
			continue; 
		}
		FieldPtr->ClearFieldAllFrames();
		AOIObjManager.DestroyFieldObj(m_ProjectInspectionFieldPtrList[i]);
		FieldPtr = NULL;		
	}	
	m_ProjectInspectionFieldPtrList = FieldPtrList;	
	ResetProjectObjectFieldPtr(DistrictID);
	SetProjectInspectionFieldBufferReleased(true);
	m_ProjectFieldPtrList = FieldPtrList;
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ResetProjectAllInspectionField()//復歸專案檢測區域
{
	size_t     i=0, j=0, k=0;
	CAOIField *FieldPtr = NULL;
	const size_t FieldCount = GetProjectInspectionFieldCount_Inline();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectInspectionFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		FieldPtr->RemoveFieldAllRgns();
		FieldPtr->RemoveFieldAllBoards();
	}			
	ResetProjectObjectFieldPtr();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ResetProjectAllInspectionField(DISTRICT_ID DistrictID)//復歸專案檢測區域
{
	size_t     i=0, j=0, k=0;
	CAOIField *FieldPtr = NULL;
	const size_t FieldCount = GetProjectInspectionFieldCount_Inline();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectInspectionFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		if ( DistrictID != FieldPtr->GetFieldDistrictID() ) { continue; }
		FieldPtr->RemoveFieldAllRgns();
		FieldPtr->RemoveFieldAllBoards();
	}			
	ResetProjectObjectFieldPtr(DistrictID);
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::LayoutProjectInspectionFieldList()//重整專案檢測區域列表
{
	size_t       i=0, j=0;
	size_t       FrameCount=0;
	unsigned int index = 0;
	CAOIField *FieldPtr = NULL;
	CAOIFrame *FramePtr = NULL;
	std::vector<CAOIField*> ProjectFieldPtrList = this->m_ProjectInspectionFieldPtrList;
	
	index = 0;
	ClearProjectInspectionField_Inline();
	const size_t FieldCount = ProjectFieldPtrList.size();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = ProjectFieldPtrList[i];
		if ( NULL == FieldPtr ) { continue; }

		FieldPtr->SetFieldIndex(index);
		FrameCount = FieldPtr->GetFieldFramePtrCount();
		for ( j=0; j<FrameCount; j++ )
		{
			FramePtr = FieldPtr->GetFieldFramePtr(j, false);
			if ( NULL == FramePtr ) { continue; }
			FramePtr->SetFrameFieldIndex(index);
		}
		AddProjectInspectionFieldPtr_Inline(FieldPtr);
		index ++;
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DestroyProjectInspectionFieldSelected()//刪除專案選取的檢測區域
{
	size_t       i=0, j=0;
	size_t       FrameCount=0;
	unsigned int index = 0;
	CAOIField *FieldPtr = NULL;
	CAOIFrame *FramePtr = NULL;
	std::vector<CAOIField*> ProjectFieldPtrList = m_ProjectInspectionFieldPtrList;
	
	index = 0;
	ClearProjectInspectionField_Inline();
	const size_t FieldCount = ProjectFieldPtrList.size();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = ProjectFieldPtrList[i];
		if ( NULL == FieldPtr ) { continue; }
		if ( FieldPtr->GetFieldSelected() == true ) 
		{
			FieldPtr->RemoveFieldAllRgns();
			FieldPtr->RemoveFieldAllBoards();
			FieldPtr->ClearFieldAllFrames();
			AOIObjManager.DestroyFieldObj(ProjectFieldPtrList[i]);
			FieldPtr = NULL;		
			continue;
		}

		FieldPtr->SetFieldIndex(index);
		FrameCount = FieldPtr->GetFieldFramePtrCount();
		for ( j=0; j<FrameCount; j++ )
		{
			FramePtr = FieldPtr->GetFieldFramePtr(j, false);
			if ( NULL == FramePtr ) { continue; }
			FramePtr->SetFrameFieldIndex(index);
		}
		AddProjectInspectionFieldPtr_Inline(FieldPtr);
		index ++;
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectAllInspectionField(bool bSel)//選取專案整個檢測區域
{
	size_t     i=0, j=0, k=0;
	CAOIField *FieldPtr = NULL;		
	const size_t FieldCount = GetProjectInspectionFieldCount_Inline();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectInspectionFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		FieldPtr->SetFieldSelected(bSel);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectInspectionFieldValid(const CAOIField *RefFieldPtr)//確認專案檢測區域指標有效
{
	if ( NULL == RefFieldPtr ) { return false; }
	size_t       i=0;	
	CAOIField *FieldPtr = NULL;	
	const size_t FieldCount = GetProjectInspectionFieldCount_Inline();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectInspectionFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		if ( RefFieldPtr == FieldPtr )
		{	return true; }
	}		
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ReleaseProjectInspectionFieldFrameImageBuffer()//釋放專案檢測區域影像記憶體
{	
	size_t       i=0, j=0;	
	size_t     FrameCount=0;
	CAOIFrame *FramePtr = NULL;
	CAOIField *FieldPtr = NULL;	
	OFFLINE_IMAGE_SCOPE OfflineImageScope=GetProjectOfflineImageScope();
	const size_t FieldCount = GetProjectInspectionFieldUsedCount(OfflineImageScope);
	AOIDataCollect.SaveMovingTimeMsg(_T("CAOIProject::ReleaseProjectInspectionFieldFrameImageBuffer Start"));
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectInspectionFieldUsedPtr(i, false, OfflineImageScope);
		if ( NULL == FieldPtr ) { continue; }
		FrameCount = FieldPtr->GetFieldFramePtrCount();
		for ( j=0; j<FrameCount; j++ )
		{
			FramePtr = FieldPtr->GetFieldFramePtr(j, false);
			if ( NULL == FramePtr ) { continue; }
			FramePtr->ClearFrameBuffer();
			FramePtr->SetFrameCalcState(FRAME_CALC_NONE);
			FramePtr->SetFrameMergeState(FRAME_MERGE_NONE);
		}
	}		
	SetProjectInspectionFieldBufferReleased(true);
	AOIDataCollect.SaveMovingTimeMsg(_T("CAOIProject::ReleaseProjectInspectionFieldFrameImageBuffer End"));
	return true;
}
//---------------------------------------------------------------------------------//
void CAOIProject::ClearProjectAllInspectionPartField()//清除專案檢測局部區域列表		
{
	CAOIField *FieldPtr=NULL;
	const size_t FieldCount = GetProjectInspectionPartFieldCount_Inline();
	for ( size_t i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectInspectionPartFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		FieldPtr->ClearFieldAllFrames();
		AOIObjManager.DestroyFieldObj(m_ProjectInspectionPartFieldPtrList[i]);
		FieldPtr = NULL;		
	}		
	ClearProjectInspectionPartField_Inline();		
	ResetProjectObjectFieldPtr();
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectInspectionPartFieldCount() const//取得專案檢測局部區域數量	
{
	return GetProjectInspectionPartFieldCount_Inline();
}
//---------------------------------------------------------------------------------//
void CAOIProject::AddProjectInspectionPartFieldPtr(CAOIField *FieldPtr, bool SetIdx)//增加專案檢測局部區域
{
	if ( NULL == FieldPtr ) { return; }
	size_t Idx=FieldPtr->GetFieldIndex();
	if ( false == SetIdx )
	{	AddProjectInspectionPartFieldPtr_Inline(FieldPtr);	}
	else
	{
		LockProject();
		Idx=GetProjectInspectionPartFieldCount();
		AddProjectInspectionPartFieldPtr_Inline(FieldPtr);
		UnlockProject();
	}

	FieldPtr->SetFieldIndex(Idx);
	FieldPtr->SetFieldProjectPtr(this);
	FieldPtr->SetFieldPartImageMode(true);
	CString OfflineFolder=GetProjectInspectionOfflineFolder();
	const size_t FrameCount=FieldPtr->GetFieldFramePtrCount();
	for ( size_t i=0; i<FrameCount; i++ )
	{
		CAOIFrame *FramePtr=FieldPtr->GetFieldFramePtr(i, false);
		if ( NULL == FramePtr ) { continue; }
		FramePtr->SetFrameFieldIndex(Idx);
		FramePtr->SetFrameFileFolder(OfflineFolder);
		FramePtr->SetFrameFileNameOffline(OfflineFolder);
		FramePtr->SetFrameUniqueID(GetProjectFrameUniqueID(i, true));
	}	
	return;
}
//---------------------------------------------------------------------------------//
CAOIField* CAOIProject::GetProjectInspectionPartFieldPtr(size_t index, bool check) const//取得專案檢測局部區域指標
{
	if ( check )
	{
		const size_t count = GetProjectInspectionPartFieldCount_Inline();
		if ( index >= count )
		{	return NULL; }
	}
	return GetProjectInspectionPartFieldPtr_Inline(index);	
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SaveProjectInspectionPartOfflineFile(LPCTSTR pfilename)//儲存檢測局部離線檔案
{
	if ( SaveProjectInspectionOfflineFileFn(pfilename, OFFLINE_IMAGE_PART) == false )
	{
		SetProjectExceptionCode_FileWrite();
		return false; 
	}
	return true;	
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectInspectionFieldUsedCount(OFFLINE_IMAGE_SCOPE Scope) const//取得專案檢測區域使用數量	
{
	size_t Count=0;
	if ( OFFLINE_IMAGE_PART == Scope )
	{	Count = GetProjectInspectionPartFieldCount_Inline();	}
	else
	{	Count = GetProjectInspectionFieldCount_Inline();	}
	return Count;
}
//---------------------------------------------------------------------------------//
CAOIField* CAOIProject::GetProjectInspectionFieldUsedPtr(size_t index, bool check, OFFLINE_IMAGE_SCOPE Scope) const//取得專案檢測區域使用指標
{
	CAOIField *FieldPtr = NULL;
	if ( OFFLINE_IMAGE_PART == Scope )
	{	FieldPtr = GetProjectInspectionPartFieldPtr(index, check);	}
	else
	{	FieldPtr = GetProjectInspectionFieldPtr(index, check);	}
	return FieldPtr;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectMatrixField(bool ByCadRegion, std::vector<CAOIField*> &FieldPtrList)//建立等間距區域位置
{
	CString         str;
	size_t          i=0, j=0;		
	size_t          WindowCount = 0;
	bool            bCheckRegion=true;	
	TREGION4D       ProjectMapCadRgn;
	TREGION4D       ProjectMapStageRgn;	
	TREGION4D       Region, FullRegion;		
	CAOIPanel      *PanelPtr = NULL;	
	CMapCoordinate *MapCTSPtr = NULL;
	DISTRICT_ID     DistrictID = GetProjectActDistrictID();

	//GetProjectMapCadRgn(ProjectMapCadRgn);
	//GetProjectMapStageRgn(ProjectMapStageRgn);
	GetProjectMapLocCadRgn(ProjectMapCadRgn);
	GetProjectMapLocStageRgn(ProjectMapStageRgn);
	if ( true == ByCadRegion )
	{	FullRegion = ProjectMapCadRgn;	}
	else
	{	FullRegion = ProjectMapStageRgn; }

	FieldPtrList.clear();
	if ( CheckProjectObjectRegion(ByCadRegion) == false )
	{	return false; }	

	//根據FullRegion來配置Field	
	TPOINT2D   Pos;
	TPOINT3D   FieldPos;
	TPOINT3D   FieldStagePos;
	TPOINT3D   StartPos;
	TSIZE2D    RegionSize;
	RECT       FieldRect={0};
	CAOIField* FieldPtr=NULL;
	double FovWReal = 0.0, FovHReal=0.0;
	double FovWInner = 0.0, FovHInner=0.0;
	double FovWOuter = 0.0, FovHOuter=0.0;
	AOIDataCollect.GetFovSizeReal(FovWReal, FovHReal);
	AOIDataCollect.GetFovSizeInner(FovWInner, FovHInner);
	AOIDataCollect.GetFovSizeOuter(FovWOuter, FovHOuter);
	ModifyProjectFieldSize(FovWInner, FovHInner);

	RegionSize.cx = FullRegion.maxX-FullRegion.minX;
	RegionSize.cy = FullRegion.maxY-FullRegion.minY;
	const size_t FieldRows = JetAPI::Ceil(RegionSize.cy/FovHInner);
	const size_t FieldCols = JetAPI::Ceil(RegionSize.cx/FovWInner);	
	const double DummyW = 50;//FOV_REGION_MARGIN_OUTER/2;//50;
	const double DummyH = 50;//FOV_REGION_MARGIN_OUTER/2;//50;
	const bool   CheckInner=false;
	//從最內側往外長
	PanelPtr = GetProjectPanelPtr(0, true);
	if ( NULL != PanelPtr )
	{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);	}
	StartPos.x = FullRegion.minX+(FovWInner*0.5);
	StartPos.y = FullRegion.minY+(FovHInner*0.5);;
	FieldPos.z = FieldStagePos.z = StartPos.z = GetProjectFocusPos();	
	for (i=0; i<FieldRows; i++ )
	{		
		for ( j=0; j<FieldCols; j++ )
		{
			FieldPos.y = StartPos.y+(i*FovHInner);
			FieldPos.x = StartPos.x+(j*FovWInner);						
			//機台區分的區塊
			FieldPtr = AOIObjManager.CreateFieldObj();
			if ( NULL == FieldPtr )
			{
				this->m_ErrorString = _T("Error, Create AOI Field Obj Fault");
				return false;
			}
			//this->AddProjectTempFieldPtr(FieldPtr, false);
			FieldPtr->SetFieldIndex(FieldPtrList.size());
			FieldPtr->SetFieldDistrictID(DistrictID);
			FieldPtr->SetFieldSize_Real(FovWReal, FovHReal);
			FieldPtr->SetFieldSize_Inner(FovWInner, FovHInner);
			FieldPtr->SetFieldSize_Outer(FovWOuter, FovHOuter);
			FieldPtrList.push_back(FieldPtr);
			if ( true == ByCadRegion )
			{
				FieldPtr->SetFieldCadPosX(FieldPos.x);
				FieldPtr->SetFieldCadPosY(FieldPos.y);
				if ( NULL != MapCTSPtr )
				{
					MapCTSPtr->Map2D(FieldPos.x, FieldPos.y, FieldStagePos.x, FieldStagePos.y);
					FieldPtr->SetFieldStagePos(FieldStagePos);
				}				
			}
			else
			{	FieldPtr->SetFieldStagePos(FieldPos);	}			
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectMatrixField_v2(bool ByCadRegion, std::vector<CAOIField*> &FieldPtrList)//建立等間距區域位置
{
	CString         str;
	size_t          i=0, j=0;		
	size_t          WindowCount = 0;
	bool            bCheckRegion=true;	
	TREGION4D       ProjectMapCadRgn;
	TREGION4D       ProjectMapStageRgn;	
	TREGION4D       Region, FullRegion;		
	CAOIPanel      *PanelPtr = NULL;	
	CMapCoordinate *MapCTSPtr = NULL;	
	DISTRICT_ID     DistrictID = GetProjectActDistrictID();

	//GetProjectMapCadRgn(ProjectMapCadRgn);
	//GetProjectMapStageRgn(ProjectMapStageRgn);
	GetProjectMapLocCadRgn(ProjectMapCadRgn);
	GetProjectMapLocStageRgn(ProjectMapStageRgn);
	if ( true == ByCadRegion )
	{	FullRegion = ProjectMapCadRgn;	}
	else
	{	FullRegion = ProjectMapStageRgn; }

	FieldPtrList.clear();
	if ( CheckProjectObjectRegion(ByCadRegion) == false )
	{	return false; }	

	//根據FullRegion來配置Field	
	TPOINT2D   Pos;
	TPOINT3D   FieldPos;
	TPOINT3D   FieldStagePos;
	TPOINT3D   StartPos;
	TSIZE2D    RegionSize;
	RECT       FieldRect={0};
	CAOIField* FieldPtr=NULL;
	double FovWReal = 0.0, FovHReal=0.0;
	double FovWInner = 0.0, FovHInner=0.0;
	double FovWOuter = 0.0, FovHOuter=0.0;
	AOIDataCollect.GetFovSizeReal(FovWReal, FovHReal);
	AOIDataCollect.GetFovSizeInner(FovWInner, FovHInner);
	AOIDataCollect.GetFovSizeOuter(FovWOuter, FovHOuter);
	ModifyProjectFieldSize(FovWInner, FovHInner);

	RegionSize.cx = FullRegion.maxX-FullRegion.minX;
	RegionSize.cy = FullRegion.maxY-FullRegion.minY;
	size_t FieldRows = JetAPI::Ceil(RegionSize.cy/FovHInner);
	size_t FieldCols = JetAPI::Ceil(RegionSize.cx/FovWInner);	
	const double DummyW = 50;//FOV_REGION_MARGIN_OUTER/2;//50;
	const double DummyH = 50;//FOV_REGION_MARGIN_OUTER/2;//50;
	const bool   CheckInner=false;
	//從最內側往外長
	PanelPtr = GetProjectPanelPtr(0, true);
	if ( NULL != PanelPtr )
	{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);	}
	StartPos.x = FullRegion.minX+(FovWInner*0.5);
	StartPos.y = FullRegion.minY+(FovHInner*0.5);

	const double MarginW = 0;
	const double MarginH = 0;
	const double ComponentSizeW = FovWInner*0.8;
	const double ComponentSizeH = FovHInner*0.8;
	const double MapSizeW = FullRegion.GetWidth()-MarginW-MarginW;
	const double MapSizeH = FullRegion.GetHeight()-MarginH-MarginH;	
	double MapSizeInnerW = MapSizeW-ComponentSizeW;
	double MapSizeInnerH = MapSizeH-ComponentSizeH;
	StartPos.x = FullRegion.minX+(ComponentSizeW/2)+MarginW;//左下腳第1個位置
	StartPos.y = FullRegion.minY+(ComponentSizeH/2)+MarginH;
	FieldPos.z = FieldStagePos.z = StartPos.z = GetProjectFocusPos();	

	if ( MapSizeInnerW < 0 )
	{		
		MapSizeInnerW = 0; 
		StartPos.x = FullRegion.GetCpX();
	}
	if ( MapSizeInnerH < 0 ) 
	{ 
		MapSizeInnerH = 0; 
		StartPos.y = FullRegion.GetCpY();
	}
	FieldCols = (int)(MapSizeInnerW/FovWInner)+1;
	FieldRows = (int)(MapSizeInnerH/FovHInner)+1;
	const double PitchX = MapSizeInnerW/FieldCols;
	const double PitchY = MapSizeInnerH/FieldRows;
	FieldCols += 1;
	FieldRows += 1;
	for (i=0; i<FieldRows; i++ )
	{		
		for ( j=0; j<FieldCols; j++ )
		{
			FieldPos.y = StartPos.y+(i*PitchY);
			FieldPos.x = StartPos.x+(j*PitchX);						
			//機台區分的區塊
			FieldPtr = AOIObjManager.CreateFieldObj();
			if ( NULL == FieldPtr )
			{
				this->m_ErrorString = _T("Error, Create AOI Field Obj Fault");
				return false;
			}
			//this->AddProjectTempFieldPtr(FieldPtr, false);
			FieldPtr->SetFieldIndex(FieldPtrList.size());
			FieldPtr->SetFieldDistrictID(DistrictID);
			FieldPtr->SetFieldSize_Real(FovWReal, FovHReal);
			FieldPtr->SetFieldSize_Inner(FovWInner, FovHInner);
			FieldPtr->SetFieldSize_Outer(FovWOuter, FovHOuter);
			FieldPtrList.push_back(FieldPtr);
			if ( true == ByCadRegion )
			{
				FieldPtr->SetFieldCadPosX(FieldPos.x);
				FieldPtr->SetFieldCadPosY(FieldPos.y);
				if ( NULL != MapCTSPtr )
				{
					MapCTSPtr->Map2D(FieldPos.x, FieldPos.y, FieldStagePos.x, FieldStagePos.y);
					FieldPtr->SetFieldStagePos(FieldStagePos);
				}				
			}
			else
			{	FieldPtr->SetFieldStagePos(FieldPos);	}			
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectRandomField_Panel(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, std::vector<CAOIField*> &FieldPtrList, bool ByCadRegion, FIELD_BUILD_AREA_MODE AreaMode)//建立任意位置區域位置-整板
{	
	size_t       i=0, j=0;
	size_t       FieldCount=0;	
	CAOIField   *FieldPtr=NULL;
	CAOIPanel   *PanelPtr=NULL;
	std::vector<CAOIField*>  PanelFieldPtrList;
	const double StagePosZ = GetProjectFocusPos();
	const size_t PanelCount = GetProjectPanelCount();
	FIELD_DIVISION_MODE DivisionMode=GetProjectParameter().m_FieldDivisionMode;

	FieldPtrList.clear();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }

		PanelFieldPtrList.clear();
		if ( PanelPtr->CreatePanelFieldList(BuildMode, DistrictID, PanelFieldPtrList, DivisionMode, ByCadRegion, AreaMode) == false )
		{
			this->m_ErrorString.Format(_T("Error, CreatePanelFieldList Fault [Panel:%d]"), i+1);
			return false;
		}		

		FieldCount = PanelFieldPtrList.size();
		for ( j=0; j<FieldCount; j++ )
		{
			FieldPtr = PanelFieldPtrList[j];
			if ( NULL == FieldPtr ) { continue; }
			FieldPtr->SetFieldPanelIndex(i);
			FieldPtr->SetFieldPanelPtr(PanelPtr);
			FieldPtr->SetFieldStagePosZ(StagePosZ);
			FieldPtrList.push_back(FieldPtr);			
		}
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectRandomField_Board(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, std::vector<CAOIField*> &FieldPtrList, bool ByCadRegion)//建立任意位置區域位置-單板
{
	size_t       i=0, j=0;
	size_t       FieldCount=0;	
	CAOIField   *FieldPtr=NULL;
	CAOIBoard   *BoardPtr=NULL;	
	std::vector<CAOIField*>  BoardFieldPtrList;	
	const double StagePosZ = GetProjectFocusPos();
	const size_t BoardCount = GetProjectBoardCount();
	FIELD_DIVISION_MODE DivisionMode=GetProjectParameter().m_FieldDivisionMode;
	
	FieldPtrList.clear();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }

		BoardFieldPtrList.clear();
		if ( BoardPtr->CreateBoardFieldList(BuildMode, DistrictID, BoardFieldPtrList, DivisionMode, ByCadRegion) == false )
		{
			this->m_ErrorString.Format(_T("Error, CreateBoardFieldList Fault [Board:%d]"), i+1);
			return false;
		}		

		FieldCount = BoardFieldPtrList.size();
		for ( j=0; j<FieldCount; j++ )
		{
			FieldPtr = BoardFieldPtrList[j];
			if ( NULL == FieldPtr ) { continue; }
			FieldPtr->SetFieldBoardIndex(i);
			FieldPtr->SetFieldBoardPtr(BoardPtr);			
			FieldPtr->SetFieldStagePosZ(StagePosZ);
			FieldPtr->SetFieldDistrictID(DistrictID);
			FieldPtrList.push_back(FieldPtr);			
		}
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectRandomField_Project(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, std::vector<CAOIField*> &FieldPtrList, bool ByCadRegion)//建立任意位置區域位置-專案
{
	size_t       j=0;
	size_t       FieldCount=0;	
	CAOIField   *FieldPtr=NULL;	
	std::vector<CAOIField*>  ProjectFieldPtrList;	
	const double StagePosZ = GetProjectFocusPos();	
	FIELD_DIVISION_MODE DivisionMode=GetProjectParameter().m_FieldDivisionMode;
	
	FieldPtrList.clear();	
	if ( CreateProjectFieldList(BuildMode, DistrictID, ProjectFieldPtrList, DivisionMode, ByCadRegion) == false )
	{
		m_ErrorString = _T("Error, CreateProjectFieldList Fault");
		return false;
	}		

	FieldCount = ProjectFieldPtrList.size();
	for ( j=0; j<FieldCount; j++ )
	{
		FieldPtr = ProjectFieldPtrList[j];
		if ( NULL == FieldPtr ) { continue; }		
		FieldPtr->SetFieldStagePosZ(StagePosZ);
		FieldPtr->SetFieldDistrictID(DistrictID);
		FieldPtrList.push_back(FieldPtr);			
	}	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectRandomField_Component(std::vector<CAOIField*> &FieldPtrList)//建立任意位置區域位置-零件
{	
	size_t         i=0, j=0;
	size_t         FieldCount=0;		
	bool           bCreate=false;
	double         CadPosX=0, CadPosY=0;
	double         StagePosX=0, StagePosY=0;
	DISTRICT_ID    DistrictID;
	CAOIField     *FieldPtr=NULL;		
	CAOIComponent *ComponentPtr = NULL;	
	const double  StagePosZ = GetProjectFocusPos();
	const size_t  ComponentCount = GetProjectComponentCount();
	const bool    OfflineMode = AOIDataCollect.GetOfflineMode();
	
	FieldPtrList.clear();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentNeedToCalculate() == false ) { continue; }		
		if ( ComponentPtr->GetComponentSelfFieldEnabled() == false ) { continue; }		
		FieldPtr = ComponentPtr->GetComponentSelfFieldPtr();		
		if ( NULL == FieldPtr )
		{
			if ( ComponentPtr->CreateComponentSelfFieldPtr() == false ) { continue; }
			bCreate = true;
		}
		else
		{
			bCreate = false;
			if ( true == OfflineMode ) 
			{
				CadPosX = ComponentPtr->GetComponentCadPosX();
				CadPosY = ComponentPtr->GetComponentCadPosY();
				StagePosX = ComponentPtr->GetComponentStagePosX();
				StagePosY = ComponentPtr->GetComponentStagePosY();	
				FieldPtr->SetFieldCadPosX(CadPosX);
				FieldPtr->SetFieldCadPosY(CadPosY);
				FieldPtr->SetFieldStagePosX(StagePosX);
				FieldPtr->SetFieldStagePosY(StagePosY);
			}
			FieldPtr->AddFieldRgnPtr(ComponentPtr);			
		}
		FieldPtr = ComponentPtr->GetComponentSelfFieldPtr();		
		DistrictID = ComponentPtr->GetComponentDistrictID();
		if ( NULL == FieldPtr )	{	continue; }
		
		FieldPtr->SetFieldComponentIndex(i);
		FieldPtr->SetFieldDistrictID(DistrictID);
		FieldPtr->SetFieldComponentPtr(ComponentPtr);				
		FieldPtr->SetFieldStagePosZ(StagePosZ);
		if ( true == bCreate )
		{	FieldPtrList.push_back(FieldPtr); }
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectProgramObject()//建立編程區域物件
{
	bool IsOK = false;	
	const bool ByCadRegion = false;
	FIELD_BUILD_MODE BuildMode = FIELD_BUILD_MATRIX;
	IsOK = CreateProjectProgramField();
	if ( true == IsOK )
	{	IsOK = CreateProjectSubRgn(BuildMode, ByCadRegion);	}
	if ( true == IsOK )
	{	m_ProjectFieldPtrList = m_ProjectProgramFieldPtrList; }
	SetProjectFieldBuildMode(BuildMode);
	SetProjectFieldBuildAreaMode(FIELD_BUILD_AREA_COMPONENT);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectInspectionObject(FIELD_BUILD_MODE BuildMode, FIELD_BUILD_AREA_MODE AreaMode)//建立檢測區域物件
{
	bool IsOK = false;	
	bool ByCadRegion = true;
	OFFLINE_IMAGE_SCOPE OfflineImageScope=GetProjectOfflineImageScope();
	SetProjectInspectionFieldBuildMode(BuildMode);	
	if ( OFFLINE_IMAGE_PART == OfflineImageScope )
	{	IsOK = true;	}
	else
	{
		switch ( BuildMode )
		{	
		case FIELD_BUILD_RANDOM_PANEL:
			ByCadRegion = true;
			if ( FIELD_BUILD_AREA_COMPONENT == AreaMode )
			{	ResetProjectBoardPanelFieldParam();	}
			IsOK = CreateProjectSubRgn(BuildMode, ByCadRegion);
			if ( true == IsOK )
			{	IsOK = CreateProjectInspectionField_RandomPanel(BuildMode, ByCadRegion, AreaMode); }
			if ( true == IsOK )
			{	m_ProjectFieldPtrList = m_ProjectInspectionFieldPtrList; }
			if ( true == IsOK )
			{				
				if ( CreateProjectInspectionField_RandomComponent() )
				{	m_ProjectFieldPtrList = m_ProjectInspectionFieldPtrList; }
			}
			break;
		case FIELD_BUILD_RANDOM_BOARD:
			ByCadRegion = true;
			AreaMode = FIELD_BUILD_AREA_COMPONENT;
			if ( FIELD_BUILD_AREA_COMPONENT == AreaMode )
			{	ResetProjectBoardPanelFieldParam();	}
			IsOK = CreateProjectSubRgn(BuildMode, ByCadRegion);
			if ( true == IsOK )
			{	IsOK = CreateProjectInspectionField_RandomBoard(BuildMode, ByCadRegion); }
			if ( true == IsOK )//其他不屬於單板上的物件
			{	IsOK = CreateProjectInspectionField_RandomPanel(BuildMode, ByCadRegion, AreaMode); }		
			if ( true == IsOK )
			{	m_ProjectFieldPtrList = m_ProjectInspectionFieldPtrList; }
			if ( true == IsOK )
			{				
				if ( CreateProjectInspectionField_RandomComponent() )
				{	m_ProjectFieldPtrList = m_ProjectInspectionFieldPtrList; }
			}
			break;		
		case FIELD_BUILD_RANDOM_PROJECT:
			ByCadRegion = true;
			if ( FIELD_BUILD_AREA_COMPONENT == AreaMode )
			{	ResetProjectBoardPanelFieldParam();	}
			IsOK = CreateProjectSubRgn(BuildMode, ByCadRegion);
			if ( true == IsOK )
			{	IsOK = CreateProjectInspectionField_RandomProject(BuildMode, ByCadRegion); }
			if ( true == IsOK )
			{	m_ProjectFieldPtrList = m_ProjectInspectionFieldPtrList; }
			if ( true == IsOK )
			{				
				if ( CreateProjectInspectionField_RandomComponent() )
				{	m_ProjectFieldPtrList = m_ProjectInspectionFieldPtrList; }
			}
			break;
		default:		
			BuildMode = FIELD_BUILD_MATRIX;
			AreaMode = FIELD_BUILD_AREA_COMPONENT;	
			if ( FIELD_BUILD_AREA_COMPONENT == AreaMode )
			{	ResetProjectBoardPanelFieldParam();	}
			ByCadRegion = GetProjectInspectionFieldCadMode_Matrix();
			IsOK = CreateProjectInspectionField_Matrix();
			if ( true == IsOK )
			{	IsOK = CreateProjectSubRgn(BuildMode, ByCadRegion);	}
			if ( true == IsOK )
			{	m_ProjectFieldPtrList = m_ProjectInspectionFieldPtrList; }			
			break;
		}
		SetProjectFieldBuildMode(BuildMode);
		SetProjectFieldBuildAreaMode(AreaMode);
		SetProjectInspectionFieldBuildAreaMode(AreaMode);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectProgramField()//建立編程Field位置
{	
	CString        str;	
	size_t         i=0, j=0;		
	size_t         FieldCount=0;
	size_t         WindowCount = 0;
	unsigned int   FieldIdx=0;
	bool           bCheckRegion=true;
	const bool     ByCadRegion = false;
	TREGION4D      Region, FullRegion;	
	CAOIMark      *MarkPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIPanel     *PanelPtr = NULL;
	CAOIWindow    *WindowPtr = NULL;	
	CAOIComponent *ComponentPtr = NULL;	
	CAOIField     *FieldPtr = NULL;
	std::vector<CAOIField*> FieldPtrList;
	const size_t   MarkCount = GetProjectMarkCount();
	const size_t   BarcodeCount = GetProjectBarcodeCount();
	const size_t   PanelCount = GetProjectPanelCount();
	const size_t   ComponentCount = GetProjectComponentCount();

	ClearProjectAllProgramField();	
	if ( CreateProjectMatrixField(ByCadRegion, FieldPtrList) == false ) 
	{	
		FieldCount = FieldPtrList.size();
		for ( i=0; i<FieldCount; i++ )
		{
			FieldPtr = FieldPtrList[i];
			if ( NULL == FieldPtr ) { continue; }
			AOIObjManager.DestroyFieldObj(FieldPtrList[i]);
			FieldPtr = NULL;
		}
		FieldPtrList.clear();
		return false; 
	}

	FieldIdx=0;
	FieldCount = FieldPtrList.size();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = FieldPtrList[i];
		if ( NULL == FieldPtr ) { continue; }
		FieldPtr->SetFieldIndex(FieldIdx);
		FieldPtr->SetFieldListMode(FIELD_LIST_PROGRAM);
		FieldIdx ++;
	}
	m_ProjectProgramFieldPtrList = FieldPtrList;
	if ( this->CheckProjectProgramField(false) == false )
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectInspectionField_Matrix()//建立檢測區域d位置	
{
	CString        str;
	size_t         i=0, j=0;			
	size_t         WindowCount = 0;
	unsigned int   FieldIdx = 0;
	bool           bCheckRegion=true;	
	TREGION4D      Region, FullRegion;	
	CAOIMark      *MarkPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIPanel     *PanelPtr = NULL;
	CAOIWindow    *WindowPtr = NULL;	
	CAOIComponent *ComponentPtr = NULL;	
	CAOIField     *FieldPtr = NULL;
	std::vector<CAOIField*> FieldPtrList;	
	size_t         InspectionFieldCountDistrictID=0;
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	const size_t   MarkCount = GetProjectMarkCount();
	const size_t   BarcodeCount = GetProjectBarcodeCount();
	const size_t   PanelCount = GetProjectPanelCount();
	const size_t   ComponentCount = GetProjectComponentCount();
	const size_t   InspectionFieldCount = GetProjectInspectionFieldCount();
	const bool     bMultiDistrictMode = GetProjectMultiDistrictMode();
	const bool     ByCadRegion = GetProjectInspectionFieldCadMode_Matrix();
	
	if ( false == bMultiDistrictMode )
	{	InspectionFieldCountDistrictID = InspectionFieldCount;	}
	else
	{	InspectionFieldCountDistrictID = GetProjectInspectionFieldCount(DistrictID);	}
	//if ( CheckProjectObjectRegion(ByCadRegion) == false )
	//{	return false; }	
	if ( InspectionFieldCountDistrictID > 0  )
	{	
		if ( false == bMultiDistrictMode )
		{	ResetProjectAllInspectionField();	}
		else
		{	ResetProjectAllInspectionField(DistrictID);	}
	}
	else
	{
		if ( false == bMultiDistrictMode )
		{	ClearProjectAllInspectionField(); }
		else
		{	ClearProjectAllInspectionField(DistrictID); }

		bool bSucc = true;
		const int CreateFullMapComponentMode = GetCreateProjectComponentForFullProjectMapMode();		
		if ( PROJECT_CREATE_FULL_MAP_COMPONENT_V2 == CreateFullMapComponentMode )
		{	bSucc = CreateProjectMatrixField_v2(ByCadRegion, FieldPtrList); }
		else
		{	bSucc = CreateProjectMatrixField(ByCadRegion, FieldPtrList); }		
		if ( false == bSucc )
		{	return false; }

		const size_t FieldCount = FieldPtrList.size();		
		if ( false == bMultiDistrictMode )
		{
			FieldIdx=0;
			for ( i=0; i<FieldCount; i++ )
			{
				FieldPtr = FieldPtrList[i];
				if ( NULL == FieldPtr ) { continue; }
				FieldPtr->SetFieldIndex(FieldIdx);				
				FieldPtr->SetFieldListMode(FIELD_LIST_INSPECTION);
				FieldIdx ++;
			}
			m_ProjectInspectionFieldPtrList = FieldPtrList;
		}
		else
		{
			FieldIdx=(unsigned int)(m_ProjectInspectionFieldPtrList.size());
			for ( i=0; i<FieldCount; i++ )
			{
				FieldPtr = FieldPtrList[i];
				if ( NULL == FieldPtr ) { continue; }
				FieldPtr->SetFieldIndex(FieldIdx);				
				FieldPtr->SetFieldListMode(FIELD_LIST_INSPECTION);				
				m_ProjectInspectionFieldPtrList.push_back(FieldPtr);
				FieldIdx ++;
			}			
		}
	}
	if ( CheckProjectInspectionField_Matrix(false) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectInspectionField_RandomPanel(FIELD_BUILD_MODE BuildMode, bool ByCadRegion, FIELD_BUILD_AREA_MODE AreaMode)//建立檢測區域d位置	
{	
	size_t       i=0, j=0;		
	CAOIField   *FieldPtr=NULL;
	CAOIPanel   *PanelPtr=NULL;
	std::vector<CAOIField*>  FieldPtrList;	
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	const bool     bMultiDistrictMode = GetProjectMultiDistrictMode();
	
	if ( FIELD_BUILD_RANDOM_PANEL == BuildMode )
	{
		//ClearProjectAllInspectionField();//注意這部分會解構Field指標, 因此只用clear
		if ( false == bMultiDistrictMode ) 
		{	ResetProjectAllInspectionField();	}
		else
		{	ResetProjectAllInspectionField(DistrictID);	}
	}

	if ( CreateProjectRandomField_Panel(BuildMode, DistrictID, FieldPtrList, ByCadRegion, AreaMode) == false )
	{	return false; }
	
	const size_t FieldCount = FieldPtrList.size();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = FieldPtrList[i];
		if ( NULL == FieldPtr ) { continue; }
		AddProjectInspectionFieldPtr(FieldPtr, false);

		PanelPtr = FieldPtr->GetFieldPanelPtr();
		if ( NULL != PanelPtr )
		{	PanelPtr->AddPanelFieldPtr(FieldPtr); }		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectInspectionField_RandomBoard(FIELD_BUILD_MODE BuildMode, bool ByCadRegion)//建立檢測區域位置-任意位置-單板
{
	size_t       i=0, j=0;		
	CAOIField   *FieldPtr=NULL;
	CAOIBoard   *BoardPtr=NULL;	
	std::vector<CAOIField*>  FieldPtrList;	
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	const bool     bMultiDistrictMode = GetProjectMultiDistrictMode();
	
	if ( FIELD_BUILD_RANDOM_BOARD == BuildMode )
	{
		//ClearProjectAllInspectionField();//注意這部分會解構Field指標, 因此只用clear
		if ( false == bMultiDistrictMode )
		{	ResetProjectAllInspectionField(); }
		else
		{	ResetProjectAllInspectionField(DistrictID); }
	}

	if ( CreateProjectRandomField_Board(BuildMode, DistrictID, FieldPtrList, ByCadRegion) == false )
	{	return false; }
	
	const size_t FieldCount = FieldPtrList.size();	
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = FieldPtrList[i];
		if ( NULL == FieldPtr ) { continue; }
		AddProjectInspectionFieldPtr(FieldPtr, false);

		BoardPtr = FieldPtr->GetFieldBoardPtr();
		if ( NULL != BoardPtr )
		{	BoardPtr->AddBoardFieldPtr(FieldPtr); }
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectInspectionField_RandomProject(FIELD_BUILD_MODE BuildMode, bool ByCadRegion)//建立檢測區域位置-任意位置-專案
{
	size_t       i=0, j=0;		
	CAOIField   *FieldPtr=NULL;		
	std::vector<CAOIField*>  FieldPtrList;	
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	const bool     bMultiDistrictMode = GetProjectMultiDistrictMode();
	
	if ( FIELD_BUILD_RANDOM_PROJECT == BuildMode )
	{
		//ClearProjectAllInspectionField();//注意這部分會解構Field指標, 因此只用clear
		if ( false == bMultiDistrictMode )
		{	ResetProjectAllInspectionField(); }
		else
		{	ResetProjectAllInspectionField(DistrictID); }
	}

	if ( CreateProjectRandomField_Project(BuildMode, DistrictID, FieldPtrList, ByCadRegion) == false )
	{	return false; }
	
	const size_t FieldCount = FieldPtrList.size();	
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = FieldPtrList[i];
		if ( NULL == FieldPtr ) { continue; }		
		AddProjectInspectionFieldPtr(FieldPtr, false);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectInspectionField_RandomComponent()//建立檢測區域位置-任意位置-零件
{
	size_t       i=0, j=0;	
	CAOIField   *FieldPtr=NULL;	
	std::vector<CAOIField*>  FieldPtrList;	

	//ClearProjectAllInspectionField();//注意這部分會解構Field指標, 因此只用clear
	if ( CreateProjectRandomField_Component(FieldPtrList) == false )
	{	return false; }
	
	const size_t FieldCount = FieldPtrList.size();	
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = FieldPtrList[i];
		if ( NULL == FieldPtr ) { continue; }		
		AddProjectInspectionFieldPtr(FieldPtr, false);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::AdjustProjectField(OFFLINE_FILE_MODE OfflineFileMode, FIELD_BUILD_MODE BuildMode, FIELD_BUILD_AREA_MODE AreaMode)//調整Field位置
{
	if ( AdjustProjectFieldFn(OfflineFileMode, BuildMode, AreaMode) == false )
	{
		SetProjectExceptionCode(AOI_EXCEPTION_PROJECT_FIELD_ADJUST);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::AdjustProjectFieldFn(OFFLINE_FILE_MODE OfflineFileMode, FIELD_BUILD_MODE BuildMode, FIELD_BUILD_AREA_MODE AreaMode)//調整區域位置	
{
	bool IsOK = false;	
	switch ( OfflineFileMode )
	{
	case OFFLINE_FILE_PROGRAM:
		IsOK = AdjustProjectProgramField();
		break;
	case OFFLINE_FILE_INSPECTION:
		IsOK = AdjustProjectInspectionField(BuildMode, AreaMode);
		break;
	}	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::AdjustProjectProgramField()//調整編程區域位置
{
	bool IsOK = false;	
	const bool ByCadRegion = false;
	FIELD_BUILD_MODE BuildMode = FIELD_BUILD_MATRIX;
	IsOK = CheckProjectProgramField(true);
	if ( true == IsOK )
	{	IsOK = CreateProjectSubRgn(BuildMode, ByCadRegion);	}
	if ( true == IsOK )
	{
		SetProjectFieldBuildMode(BuildMode);
		m_ProjectFieldPtrList = m_ProjectProgramFieldPtrList; 
		SetProjectFieldBuildAreaMode(FIELD_BUILD_AREA_COMPONENT);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::AdjustProjectInspectionField(FIELD_BUILD_MODE BuildMode, FIELD_BUILD_AREA_MODE AreaMode)//調整檢測區域位置
{
	bool IsOK = false;
	bool ByCadRegion = true;
	OFFLINE_IMAGE_SCOPE OfflineImageScope=GetProjectOfflineImageScope();
	if ( OFFLINE_IMAGE_PART == OfflineImageScope )
	{	IsOK = true;	}
	else
	{
		switch ( BuildMode )
		{	
		case FIELD_BUILD_RANDOM_PANEL:
			ByCadRegion = true;			
			IsOK = CreateProjectSubRgn(BuildMode, ByCadRegion);
			if ( true == IsOK )
			{	IsOK = CreateProjectInspectionField_RandomPanel(BuildMode, ByCadRegion, AreaMode); }
			if ( true == IsOK )
			{	m_ProjectFieldPtrList = m_ProjectInspectionFieldPtrList;	}
			if ( true == IsOK )
			{				
				if ( CreateProjectInspectionField_RandomComponent() )
				{	m_ProjectFieldPtrList = m_ProjectInspectionFieldPtrList; }
			}
			break;
		case FIELD_BUILD_RANDOM_BOARD:
			ByCadRegion = true;			
			AreaMode = FIELD_BUILD_AREA_COMPONENT;
			IsOK = CreateProjectSubRgn(BuildMode, ByCadRegion);
			if ( true == IsOK )
			{	IsOK = CreateProjectInspectionField_RandomBoard(BuildMode, ByCadRegion); }
			if ( true == IsOK )//其他不屬於單板上的物件
			{	IsOK = CreateProjectInspectionField_RandomPanel(BuildMode, ByCadRegion, AreaMode); }
			if ( true == IsOK )
			{	m_ProjectFieldPtrList = m_ProjectInspectionFieldPtrList;	}
			if ( true == IsOK )
			{				
				if ( CreateProjectInspectionField_RandomComponent() )
				{	m_ProjectFieldPtrList = m_ProjectInspectionFieldPtrList; }
			}
			break;
		case FIELD_BUILD_RANDOM_PROJECT:
			ByCadRegion = true;						
			IsOK = CreateProjectSubRgn(BuildMode, ByCadRegion);
			if ( true == IsOK )
			{	IsOK = CreateProjectInspectionField_RandomProject(BuildMode, ByCadRegion); }
			if ( true == IsOK )
			{	m_ProjectFieldPtrList = m_ProjectInspectionFieldPtrList;	}
			if ( true == IsOK )
			{				
				if ( CreateProjectInspectionField_RandomComponent() )
				{	m_ProjectFieldPtrList = m_ProjectInspectionFieldPtrList; }
			}
			break;
		default:		
			BuildMode = FIELD_BUILD_MATRIX;
			AreaMode = FIELD_BUILD_AREA_COMPONENT;
			ByCadRegion = GetProjectInspectionFieldCadMode_Matrix();			
			IsOK = CreateProjectInspectionField_Matrix();		
			if ( true == IsOK )
			{	IsOK = CreateProjectSubRgn(BuildMode, ByCadRegion);	}
			if ( true == IsOK )
			{	m_ProjectFieldPtrList = m_ProjectInspectionFieldPtrList;	}
			break;
		}
		if ( true == IsOK )
		{	
			SetProjectFieldBuildMode(BuildMode);	
			SetProjectFieldBuildAreaMode(AreaMode);
		}
		SetProjectInspectionFieldBuildMode(BuildMode);
		SetProjectInspectionFieldBuildAreaMode(AreaMode);
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectProgramField(bool CheckObjRgn)//調整編程Field位置
{
	CString        str;
	size_t         i=0, j=0;		
	size_t         WindowCount = 0;
	bool           bCheckRegion=true;
	const bool     ByCadRegion = false;//編程模式僅以機台座標為主
	TREGION4D      Region, FullRegion;		
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIPanel     *PanelPtr = NULL;
	CAOIBoard     *BoardPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIWindow    *WindowPtr = NULL;	
	CAOIComponent *ComponentPtr = NULL;		
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	const size_t   FdCount = GetProjectFdCount();
	const size_t   MarkCount = GetProjectMarkCount();
	const size_t   PanelCount = GetProjectPanelCount();
	const size_t   BarcodeCount = GetProjectBarcodeCount();	
	const size_t   ComponentCount = GetProjectComponentCount();	

	if ( true == CheckObjRgn )
	{
		if ( this->CheckProjectObjectRegion(ByCadRegion) == false )
		{	return false; }	
	}

	//根據FullRegion來配置Field	
	TPOINT2D   Pos, Pos2;	
	RECT       FieldRect={0};
	CAOIField* FieldPtr=NULL;	
	const double DummyW = 50;//FOV_REGION_MARGIN_OUTER/2;//50;
	const double DummyH = 50;//FOV_REGION_MARGIN_OUTER/2;//50;
	const bool   CheckInner=false;	

	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		BoardPtr = FdPtr->GetFdBoardPtr();
		if ( true == ByCadRegion )
		{	FdPtr->GetFdExtendCadRegion(Region);	}
		else
		{	FdPtr->GetFdExtendStageRegion(Region);	}
		Pos.x = (Region.minX+Region.maxX)*0.5;
		Pos.y = (Region.minY+Region.maxY)*0.5;
		Pos2.x = FdPtr->GetFdStagePos().x;
		Pos2.y = FdPtr->GetFdStagePos().y;
		Region.minX = Pos.x - DummyW;
		Region.maxX = Pos.x + DummyW;
		Region.minY = Pos.y - DummyH;
		Region.maxY = Pos.y + DummyH;		
		FieldPtr = MatchProjectProgramFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
		if ( NULL == FieldPtr )
		{					
			if ( NULL == BoardPtr ) { continue; }
			this->m_ErrorString.Format(_T("Error, MatchProjectProgramFieldPtr Fault (Fd:%d)"), i+1);
			return false; 
		}
		FdPtr->SetFdFieldPtr(FieldPtr);
	}

	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		if ( true == ByCadRegion )
		{	MarkPtr->GetMarkRoiCadRegion(Region);	}
		else
		{	MarkPtr->GetMarkRoiStageRegion(Region);	}
		Pos.x = (Region.minX+Region.maxX)*0.5;
		Pos.y = (Region.minY+Region.maxY)*0.5;
		Region.minX = Pos.x - DummyW;
		Region.maxX = Pos.x + DummyW;
		Region.minY = Pos.y - DummyH;
		Region.maxY = Pos.y + DummyH;		
		FieldPtr = MatchProjectProgramFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
		if ( NULL == FieldPtr )
		{			
			this->m_ErrorString.Format(_T("Error, MatchProjectProgramFieldPtr Fault (Mark:%d)"), i+1);
			return false; 
		}
		MarkPtr->SetMarkFieldPtr(FieldPtr);
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		if ( true == ByCadRegion )
		{	BarcodePtr->GetBarcodeRoiCadRegion(Region);	}
		else
		{	BarcodePtr->GetBarcodeRoiStageRegion(Region);	}
		Pos.x = (Region.minX+Region.maxX)*0.5;
		Pos.y = (Region.minY+Region.maxY)*0.5;
		Region.minX = Pos.x - DummyW;
		Region.maxX = Pos.x + DummyW;
		Region.minY = Pos.y - DummyH;
		Region.maxY = Pos.y + DummyH;		
		FieldPtr = MatchProjectProgramFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
		if ( NULL == FieldPtr )
		{			
			this->m_ErrorString.Format(_T("Error, MatchProjectProgramFieldPtr Fault (Barcode:%d)"), i+1);
			return false; 
		}
		BarcodePtr->SetBarcodeFieldPtr(FieldPtr);
	}
	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		if ( true == ByCadRegion )
		{	ComponentPtr->GetComponentRoiCadRegion(Region);	}
		else
		{	ComponentPtr->GetComponentRoiStageRegion(Region);	}

		Pos.x = (Region.minX+Region.maxX)*0.5;
		Pos.y = (Region.minY+Region.maxY)*0.5;
		Region.minX = Pos.x - DummyW;
		Region.maxX = Pos.x + DummyW;
		Region.minY = Pos.y - DummyH;
		Region.maxY = Pos.y + DummyH;		
		FieldPtr = MatchProjectProgramFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
		if ( NULL == FieldPtr )
		{
			str = ComponentPtr->GetComponentName();
			this->m_ErrorString.Format(_T("Error, MatchProjectProgramFieldPtr Fault (C:%s)"), str);
			return false; 
		}
		ComponentPtr->SetComponentFieldPtr(FieldPtr);

		WindowCount = ComponentPtr->GetComponentWindowCount();
		for ( j=0; j<WindowCount; j++ )
		{
			WindowPtr = ComponentPtr->GetComponentWindowPtr(j, false);
			if ( NULL == WindowPtr ) { continue; }

			if ( true == ByCadRegion )
			{	WindowPtr->GetWindowRoiCadRegion(Region);	}
			else
			{	WindowPtr->GetWindowRoiStageRegion(Region);	}

			Pos.x = (Region.minX+Region.maxX)*0.5;
			Pos.y = (Region.minY+Region.maxY)*0.5;
			Region.minX = Pos.x - DummyW;
			Region.maxX = Pos.x + DummyW;
			Region.minY = Pos.y - DummyH;
			Region.maxY = Pos.y + DummyH;		
			FieldPtr = MatchProjectProgramFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
			if ( NULL == FieldPtr )
			{
				str = ComponentPtr->GetComponentName();
				this->m_ErrorString.Format(_T("Error, MatchProjectProgramFieldPtr Fault (C:%s, W:%d)"), str, j+1);
				return false; 
			}
			WindowPtr->SetWindowFieldPtr(FieldPtr);	
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::GetProjectInspectionFieldCadMode_Matrix()//取得檢測區域是否使用Cad座標-等間距
{
	return true;
	//return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectInspectionField_Matrix(bool CheckObjRgn)//確認檢測區域位置
{
	CString        str;
	size_t         i=0, j=0;		
	size_t         WindowCount = 0;
	bool           bCheckRegion=true;	
	TREGION4D      Region, FullRegion;		
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIPanel     *PanelPtr = NULL;
	CAOIBoard     *BoardPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIWindow    *WindowPtr = NULL;	
	CAOIComponent *ComponentPtr = NULL;		
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	const size_t   FdCount = GetProjectFdCount();
	const size_t   MarkCount = GetProjectMarkCount();
	const size_t   PanelCount = GetProjectPanelCount();
	const size_t   BarcodeCount = GetProjectBarcodeCount();	
	const size_t   ComponentCount = GetProjectComponentCount();	
	const bool     ByCadRegion = GetProjectInspectionFieldCadMode_Matrix();

	if ( true == CheckObjRgn )
	{
		if ( this->CheckProjectObjectRegion(ByCadRegion) == false )
		{	return false; }
	}	

	//根據FullRegion來配置Field	
	TPOINT2D   Pos;
	RECT       FieldRect={0};
	CAOIField* FieldPtr=NULL;	
	const double DummyW = 50;//FOV_REGION_MARGIN_OUTER/2;//50;
	const double DummyH = 50;//FOV_REGION_MARGIN_OUTER/2;//50;
	const bool   CheckInner=false;	

	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		BoardPtr = FdPtr->GetFdBoardPtr();
		if ( NULL == BoardPtr ) { continue; }
		if ( true == ByCadRegion )
		{	FdPtr->GetFdExtendCadRegion(Region);	}
		else
		{	FdPtr->GetFdExtendStageRegion(Region);	}		
		Pos.x = (Region.minX+Region.maxX)*0.5;
		Pos.y = (Region.minY+Region.maxY)*0.5;
		Region.minX = Pos.x - DummyW;
		Region.maxX = Pos.x + DummyW;
		Region.minY = Pos.y - DummyH;
		Region.maxY = Pos.y + DummyH;
		FieldPtr = MatchProjectInspectionFieldPtr_Matrix(Region, ByCadRegion, CheckInner, DistrictID);
		if ( NULL == FieldPtr )
		{			
			this->m_ErrorString.Format(_T("Error, CheckProjectInspectionField_Matrix Fault (Fd:%d)"), i+1);
			return false; 
		}
		FdPtr->SetFdFieldPtr(FieldPtr);
	}

	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		if ( true == ByCadRegion )
		{	MarkPtr->GetMarkRoiCadRegion(Region);	}
		else
		{	MarkPtr->GetMarkRoiStageRegion(Region);	}		
		Pos.x = (Region.minX+Region.maxX)*0.5;
		Pos.y = (Region.minY+Region.maxY)*0.5;
		Region.minX = Pos.x - DummyW;
		Region.maxX = Pos.x + DummyW;
		Region.minY = Pos.y - DummyH;
		Region.maxY = Pos.y + DummyH;
		FieldPtr = MatchProjectInspectionFieldPtr_Matrix(Region, ByCadRegion, CheckInner, DistrictID);
		if ( NULL == FieldPtr )
		{			
			this->m_ErrorString.Format(_T("Error, CheckProjectInspectionField_Matrix Fault (Mark:%d)"), i+1);
			return false; 
		}
		MarkPtr->SetMarkFieldPtr(FieldPtr);
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		if ( true == ByCadRegion )
		{	BarcodePtr->GetBarcodeRoiCadRegion(Region);	}
		else
		{	BarcodePtr->GetBarcodeRoiStageRegion(Region);	}		
		Pos.x = (Region.minX+Region.maxX)*0.5;
		Pos.y = (Region.minY+Region.maxY)*0.5;
		Region.minX = Pos.x - DummyW;
		Region.maxX = Pos.x + DummyW;
		Region.minY = Pos.y - DummyH;
		Region.maxY = Pos.y + DummyH;
		FieldPtr = MatchProjectInspectionFieldPtr_Matrix(Region, ByCadRegion, CheckInner, DistrictID);
		if ( NULL == FieldPtr )
		{			
			this->m_ErrorString.Format(_T("Error, CheckProjectInspectionField_Matrix Fault (Barcode:%d)"), i+1);
			return false; 
		}
		BarcodePtr->SetBarcodeFieldPtr(FieldPtr);
	}
	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		if ( true == ByCadRegion )
		{	ComponentPtr->GetComponentRoiCadRegion(Region);	}
		else
		{	ComponentPtr->GetComponentRoiStageRegion(Region);	}

		Pos.x = (Region.minX+Region.maxX)*0.5;
		Pos.y = (Region.minY+Region.maxY)*0.5;
		Region.minX = Pos.x - DummyW;
		Region.maxX = Pos.x + DummyW;
		Region.minY = Pos.y - DummyH;
		Region.maxY = Pos.y + DummyH;		
		FieldPtr = MatchProjectInspectionFieldPtr_Matrix(Region, ByCadRegion, CheckInner, DistrictID);
		if ( NULL == FieldPtr )
		{
			str = ComponentPtr->GetComponentName();
			this->m_ErrorString.Format(_T("Error, CheckProjectInspectionField_Matrix Fault (C:%s)"), str);
			return false; 
		}
		ComponentPtr->SetComponentFieldPtr(FieldPtr);

		WindowCount = ComponentPtr->GetComponentWindowCount();
		for ( j=0; j<WindowCount; j++ )
		{
			WindowPtr = ComponentPtr->GetComponentWindowPtr(j, false);
			if ( NULL == WindowPtr ) { continue; }

			if ( true == ByCadRegion )
			{	WindowPtr->GetWindowRoiCadRegion(Region);	}
			else
			{	WindowPtr->GetWindowRoiStageRegion(Region);	}

			Pos.x = (Region.minX+Region.maxX)*0.5;
			Pos.y = (Region.minY+Region.maxY)*0.5;
			Region.minX = Pos.x - DummyW;
			Region.maxX = Pos.x + DummyW;
			Region.minY = Pos.y - DummyH;
			Region.maxY = Pos.y + DummyH;		
			FieldPtr = MatchProjectInspectionFieldPtr_Matrix(Region, ByCadRegion, CheckInner, DistrictID);
			if ( NULL == FieldPtr )
			{
				str = ComponentPtr->GetComponentName();
				this->m_ErrorString.Format(_T("Error, CheckProjectInspectionField_Matrix Fault (C:%s, W:%d)"), str, j+1);
				return false; 
			}
			WindowPtr->SetWindowFieldPtr(FieldPtr);	
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectSubRgn(FIELD_BUILD_MODE BuildMode, bool ByCadRegion)//建立專案的子檢測框
{
	CString        str;
	size_t         i=0, j=0, k=0;
	size_t         SubRgnCount = 0;
	size_t         WindowCount = 0;	
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIRgn       *SubRgnPtr = NULL;	
	CAOIWindow    *WindowPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	const size_t   FdCount = GetProjectFdCount();
	const size_t   MarkCount = GetProjectMarkCount();
	const size_t   BarcodeCount = GetProjectBarcodeCount();		
	const size_t   ComponentCount = GetProjectComponentCount();	

	//Fd 
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		if ( NULL == FdPtr->GetFdBoardPtr() ) { continue; }
		//continue;//20201201拿掉, 避免跨FOV的臭蟲
		if ( FdPtr->CreateFdSubRgnList(BuildMode, ByCadRegion) == false )
		{
			this->m_ErrorString.Format(_T("Error, Create Fd Sub Rgn Fault (Fd:%d)"), i+1);
			return false;
		}
	}

	//Mark
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		if ( MarkPtr->CreateMarkSubRgnList(BuildMode, ByCadRegion) == false )
		{
			this->m_ErrorString.Format(_T("Error, Create Mark Sub Rgn Fault (Mark:%d)"), i+1);
			return false;
		}
	}

	//Barcode 
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		if ( BarcodePtr->CreateBarcodeSubRgnList(BuildMode, ByCadRegion) == false )
		{
			this->m_ErrorString.Format(_T("Error, Create Barcode Sub Rgn Fault (Barcode:%d)"), i+1);
			return false;
		}
	}

	//Component 
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }		
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		//if ( ComponentPtr->GetComponentNeedToUpdate() == false ) { continue; }
		//if ( ComponentPtr->GetComponentNeedToCalculate() == false ) { continue; }		
		if ( ComponentPtr->CreateComponentSubRgnList(BuildMode, ByCadRegion) == false )
		{
			str = ComponentPtr->GetComponentName();
			this->m_ErrorString.Format(_T("Error, Create Component Sub Rgn Fault (C:%s)"), str);
			return false;
		}

		WindowCount = ComponentPtr->GetComponentWindowCount();
		for ( j=0; j<WindowCount; j++ )
		{
			WindowPtr = ComponentPtr->GetComponentWindowPtr(j, false);
			if ( NULL == WindowPtr ) { continue; }			
			if ( WindowPtr->CreateWindowSubRgnList(BuildMode, ByCadRegion) == false )
			{
				str = ComponentPtr->GetComponentName();
				this->m_ErrorString.Format(_T("Error, Create Window Sub Rgn Fault (C:%s Window:%d)"), str, j+1);
				return false;
			}
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::AssignProjectField(OFFLINE_FILE_MODE OfflineFileMode)//配置Field位置
{
	bool IsOK = true;
	switch ( OfflineFileMode )
	{
	case OFFLINE_FILE_PROGRAM:
		IsOK = CAOIProject::AssignProjectProgramField();
		break;
	case OFFLINE_FILE_INSPECTION:
		IsOK = CAOIProject::AssignProjectInspectionField();
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::AssignProjectProgramField()//配置編程Field位置
{
	CString        str;
	size_t         i=0, j=0, k=0;	
	size_t         RgnIdx = 0;
	TREGION4D      Region, CameraRgn;	
	IMAGE_SIZE     ImageW=0, ImageH;
	size_t         SubRgnCount = 0;
	size_t         WindowCount = 0;
	RECT           FrameRect={0};	
	CAMERA_ID      CameraID;	
	CAOIRgn*       RgnPtr = NULL;
	CAOIRgn*       SubRgnPtr = NULL;
	CAOIField*     FieldPtr = NULL;	
	CAOIFd*        FdPtr = NULL;
	CAOIMark*      MarkPtr = NULL;
	CAOIPanel*     PanelPtr = NULL;
	CAOIBoard*     BoardPtr = NULL;
	CAOIBarcode*   BarcodePtr = NULL;
	CAOIWindow*    WindowPtr = NULL;
	CAOIComponent* ComponentPtr = NULL;
	TSIZE2D        FrameSizeUm;
	TPOINT2D       StagePos2D;
	TPOINT3D       StagePos3D;
	const int      nAlign = 4;
	const bool     CheckInner = false;
	const bool     ByCadRegion = false;	
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	BOARD_FD_GRAB_MODE BoardFdGrabMode = GetProjectBoardFdGrabMode();
	const size_t   FieldCount = GetProjectProgramFieldCount();
	const size_t   FdCount = GetProjectFdCount();	
	const size_t   MarkCount = GetProjectMarkCount();	
	const size_t   BarcodeCount = GetProjectBarcodeCount();	
	const size_t   ComponentCount = GetProjectComponentCount();

	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectProgramFieldPtr(i, false);
		if ( NULL == FieldPtr ) { continue; }
		if ( DistrictID != FieldPtr->GetFieldDistrictID() ) { continue; }
		FieldPtr->RemoveFieldAllRgns();
		FieldPtr->RemoveFieldAllBoards();
	}	
	
	CameraID = PRIMARY_CAMERA_ID;
	ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	ImageH = AOIDataCollect.GetCameraImageH(CameraID);
	const int nImageW = (int)(ImageW);
	const int nImageH = (int)(ImageH);

	if ( BOARD_FD_GRAB_INSPECTING == BoardFdGrabMode )
	{
		//Assign Field To Fiducial
		for ( i=0; i<FdCount; i++ )
		{
			FdPtr = GetProjectFdPtr(i, false);
			if ( NULL == FdPtr ) { continue; }
			if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
			if ( FdPtr->GetFdNeedToCalculate() == false ) { continue; }
			BoardPtr = FdPtr->GetFdBoardPtr();
			if ( NULL == BoardPtr ) { continue; }			

			SubRgnCount = FdPtr->GetFdSubRgnCount();
			for ( k=0; k<SubRgnCount; k++ )
			{
				SubRgnPtr = FdPtr->GetFdSubRgnPtr(k, false);
				if ( NULL == SubRgnPtr ) { continue; }
				FieldPtr = SubRgnPtr->GetRgnFieldPtr();
				if ( NULL == FieldPtr )
				{	
					if ( true == ByCadRegion )
					{	SubRgnPtr->GetRgnRoiCadRegion(Region); }
					else
					{	SubRgnPtr->GetRgnRoiStageRegion(Region); }
					FieldPtr = MatchProjectProgramFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
					if ( NULL == FieldPtr )
					{
						this->m_ErrorString.Format(_T("Error, Assign Fd Field Ptr Fault (Fd:%d-%d)"), i+1, k+1);
						return false;	
					}
				}
				FieldPtr->AddFieldRgnPtr(SubRgnPtr);	
			
				//分配影像區域
				SubRgnPtr->GetRgnRoiStageRegion(Region);
				StagePos3D = FieldPtr->GetFieldStagePos();
				StagePos2D.x = StagePos3D.x;
				StagePos2D.y = StagePos3D.y;
				AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
				JetAPI::Region4DToRect(CameraRgn, FrameRect, true);
				JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬
				if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
				{
					this->m_ErrorString.Format(_T("Error, Calc Fd Image Rect Fault (Fd:%d-%d)"), i+1, k+1);
					return false; 
				}
				//是否計算四個端點呢!?
				AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
				SubRgnPtr->SetRgnFrameImageRect(FrameRect);
				SubRgnPtr->SetRgnFrameImageSize_um(FrameSizeUm);
			}

			FieldPtr = FdPtr->GetFdFieldPtr();
			if ( NULL == FieldPtr )
			{	
				if ( true == ByCadRegion )
				{	FdPtr->GetFdExtendCadRegion(Region); }
				else
				{	FdPtr->GetFdExtendStageRegion(Region); }
				FieldPtr = MatchProjectProgramFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{
					this->m_ErrorString.Format(_T("Error, Assign Fd Field Ptr Fault (Fd:%d)"), i+1);
					return false;	
				}
			}
			FieldPtr->AddFieldRgnPtr(FdPtr);

			FdPtr->GetFdRoiStageRegion(Region);		
			StagePos3D = FieldPtr->GetFieldStagePos();
			StagePos2D.x = StagePos3D.x;
			StagePos2D.y = StagePos3D.y;
			AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
			JetAPI::Region4DToRect(CameraRgn, FrameRect, true);			
			if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
			{
				if ( 0 == SubRgnCount )
				{
					this->m_ErrorString.Format(_T("Error, Calc Fd Image Rect Fault (Fd:%d)"), i+1);
					return false; 
				}
				if ( FrameRect.left < 0 ) { FrameRect.left = 0; }
				if ( FrameRect.top  < 0 ) { FrameRect.top  = 0; }
				if ( FrameRect.right > nImageW ) { FrameRect.right = nImageW; }
				if ( FrameRect.bottom > nImageH ) { FrameRect.bottom = nImageH; }
			}
		
			//是否計算四個端點呢!?
			AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
			FdPtr->SetFdFrameImageRect(FrameRect);
			FdPtr->SetFdFrameImageSize_um(FrameSizeUm);

			TRECT4D FrameRegion;
			TPOINT2D FrameOffsetUm;
			if ( 0 == SubRgnCount )
			{	
				AOIDataCollect.MapCameraRectToStage(CameraID, FrameRect, StagePos2D, FrameRegion);
				FrameOffsetUm.x=FrameRegion.GetCpX()-Region.GetCpX();
				FrameOffsetUm.y=FrameRegion.GetCpY()-Region.GetCpY();
			}
			FdPtr->SetFdFrameImageStageOffset_um(FrameOffsetUm);
		}
	}
	
	//Assign Field To Mark
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }		
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		if ( MarkPtr->GetMarkNeedToCalculate() == false ) { continue; }

		SubRgnCount = MarkPtr->GetMarkSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = MarkPtr->GetMarkSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			FieldPtr = SubRgnPtr->GetRgnFieldPtr();
			if ( NULL == FieldPtr )
			{	
				if ( true == ByCadRegion )
				{	SubRgnPtr->GetRgnRoiCadRegion(Region); }
				else
				{	SubRgnPtr->GetRgnRoiStageRegion(Region); }
				FieldPtr = MatchProjectProgramFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{
					this->m_ErrorString.Format(_T("Error, Assign Mark Field Ptr Fault (Mark:%d-%d)"), i+1, k+1);
					return false;	
				}
			}
			FieldPtr->AddFieldRgnPtr(SubRgnPtr);	
			
			//分配影像區域
			SubRgnPtr->GetRgnRoiStageRegion(Region);
			StagePos3D = FieldPtr->GetFieldStagePos();
			StagePos2D.x = StagePos3D.x;
			StagePos2D.y = StagePos3D.y;
			AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
			JetAPI::Region4DToRect(CameraRgn, FrameRect, true);
			JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬
			if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
			{
				this->m_ErrorString.Format(_T("Error, Calc Mark Image Rect Fault (Mark:%d-%d)"), i+1, k+1);
				return false; 
			}
			//是否計算四個端點呢!?
			AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
			SubRgnPtr->SetRgnFrameImageRect(FrameRect);
			SubRgnPtr->SetRgnFrameImageSize_um(FrameSizeUm);
		}

		FieldPtr = MarkPtr->GetMarkFieldPtr();
		if ( NULL == FieldPtr )
		{	
			if ( true == ByCadRegion )
			{	MarkPtr->GetMarkRoiCadRegion(Region); }
			else
			{	MarkPtr->GetMarkRoiStageRegion(Region); }
			FieldPtr = MatchProjectProgramFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
			if ( NULL == FieldPtr )
			{
				this->m_ErrorString.Format(_T("Error, Assign Mark Field Ptr Fault (Mark:%d)"), i+1);
				return false;	
			}
		}
		FieldPtr->AddFieldRgnPtr(MarkPtr);

		MarkPtr->GetMarkRoiStageRegion(Region);
		StagePos3D = FieldPtr->GetFieldStagePos();
		StagePos2D.x = StagePos3D.x;
		StagePos2D.y = StagePos3D.y;
		AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
		JetAPI::Region4DToRect(CameraRgn, FrameRect, true);			
		if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
		{
			if ( 0 == SubRgnCount )
			{
				this->m_ErrorString.Format(_T("Error, Calc Mark Image Rect Fault (Mark:%d)"), i+1);
				return false; 
			}
			if ( FrameRect.left < 0 ) { FrameRect.left = 0; }
			if ( FrameRect.top  < 0 ) { FrameRect.top  = 0; }
			if ( FrameRect.right > nImageW ) { FrameRect.right = nImageW; }
			if ( FrameRect.bottom > nImageH ) { FrameRect.bottom = nImageH; }
		}		
		//是否計算四個端點呢!?
		AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
		MarkPtr->SetMarkFrameImageRect(FrameRect);
		MarkPtr->SetMarkFrameImageSize_um(FrameSizeUm);

		TRECT4D FrameRegion;
		TPOINT2D FrameOffsetUm;
		if ( 0 == SubRgnCount )
		{	
			AOIDataCollect.MapCameraRectToStage(CameraID, FrameRect, StagePos2D, FrameRegion);
			FrameOffsetUm.x=FrameRegion.GetCpX()-Region.GetCpX();
			FrameOffsetUm.y=FrameRegion.GetCpY()-Region.GetCpY();
		}
		MarkPtr->SetMarkFrameImageStageOffset_um(FrameOffsetUm);
	}

	//Assign Field To Barcode
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }		
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		if ( BarcodePtr->GetBarcodeNeedToCalculate() == false ) { continue; }

		SubRgnCount = BarcodePtr->GetBarcodeSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = BarcodePtr->GetBarcodeSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			FieldPtr = SubRgnPtr->GetRgnFieldPtr();
			if ( NULL == FieldPtr )
			{	
				if ( true == ByCadRegion )
				{	SubRgnPtr->GetRgnRoiCadRegion(Region); }
				else
				{	SubRgnPtr->GetRgnRoiStageRegion(Region); }
				FieldPtr = MatchProjectProgramFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{
					this->m_ErrorString.Format(_T("Error, Assign Barcode Field Ptr Fault (Barcode:%d-%d)"), i+1, k+1);
					return false;	
				}
			}
			FieldPtr->AddFieldRgnPtr(SubRgnPtr);	
			
			//分配影像區域
			SubRgnPtr->GetRgnRoiStageRegion(Region);
			StagePos3D = FieldPtr->GetFieldStagePos();
			StagePos2D.x = StagePos3D.x;
			StagePos2D.y = StagePos3D.y;
			AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
			JetAPI::Region4DToRect(CameraRgn, FrameRect, true);
			JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬
			if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
			{
				this->m_ErrorString.Format(_T("Error, Calc Barcode Image Rect Fault (Barcode:%d-%d)"), i+1, k+1);
				return false; 
			}
			//是否計算四個端點呢!?
			AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
			SubRgnPtr->SetRgnFrameImageRect(FrameRect);
			SubRgnPtr->SetRgnFrameImageSize_um(FrameSizeUm);
		}

		FieldPtr = BarcodePtr->GetBarcodeFieldPtr();
		if ( NULL == FieldPtr )
		{	
			if ( true == ByCadRegion )
			{	BarcodePtr->GetBarcodeRoiCadRegion(Region); }
			else
			{	BarcodePtr->GetBarcodeRoiStageRegion(Region); }
			FieldPtr = MatchProjectProgramFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
			if ( NULL == FieldPtr )
			{
				this->m_ErrorString.Format(_T("Error, Assign Barcode Field Ptr Fault (Barcode:%d)"), i+1);
				return false;	
			}
		}
		FieldPtr->AddFieldRgnPtr(BarcodePtr);

		BarcodePtr->GetBarcodeRoiStageRegion(Region);
		StagePos3D = FieldPtr->GetFieldStagePos();
		StagePos2D.x = StagePos3D.x;
		StagePos2D.y = StagePos3D.y;
		AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
		JetAPI::Region4DToRect(CameraRgn, FrameRect, true);			
		if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
		{
			if ( 0 == SubRgnCount )
			{
				this->m_ErrorString.Format(_T("Error, Calc Barcode Image Rect Fault (Barcode:%d)"), i+1);
				return false; 
			}
			if ( FrameRect.left < 0 ) { FrameRect.left = 0; }
			if ( FrameRect.top  < 0 ) { FrameRect.top  = 0; }
			if ( FrameRect.right > nImageW ) { FrameRect.right = nImageW; }
			if ( FrameRect.bottom > nImageH ) { FrameRect.bottom = nImageH; }
		}		
		//是否計算四個端點呢!?
		AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
		BarcodePtr->SetBarcodeFrameImageRect(FrameRect);
		BarcodePtr->SetBarcodeFrameImageSize_um(FrameSizeUm);

		TRECT4D FrameRegion;
		TPOINT2D FrameOffsetUm;
		if ( 0 == SubRgnCount )
		{	
			AOIDataCollect.MapCameraRectToStage(CameraID, FrameRect, StagePos2D, FrameRegion);
			FrameOffsetUm.x=FrameRegion.GetCpX()-Region.GetCpX();
			FrameOffsetUm.y=FrameRegion.GetCpY()-Region.GetCpY();
		}
		BarcodePtr->SetBarcodeFrameImageStageOffset_um(FrameOffsetUm);
	}
	
	//Assign Field To Component Window, window first then component
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }	
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		if ( ComponentPtr->GetComponentNeedToCalculate() == false ) { continue; }

		//to Window
		WindowCount = ComponentPtr->GetComponentWindowCount();
		for ( j=0; j<WindowCount; j++ )
		{
			WindowPtr = ComponentPtr->GetComponentWindowPtr(j, false);
			if ( NULL == WindowPtr ) { continue; }
			
			SubRgnCount = WindowPtr->GetWindowSubRgnCount();
			for ( k=0; k<SubRgnCount; k++ )
			{
				SubRgnPtr = WindowPtr->GetWindowSubRgnPtr(k, false);
				if ( NULL == SubRgnPtr ) { continue; }
				FieldPtr = SubRgnPtr->GetRgnFieldPtr();
				if ( NULL == FieldPtr )
				{	
					if ( true == ByCadRegion )
					{	SubRgnPtr->GetRgnRoiCadRegion(Region); }
					else
					{	SubRgnPtr->GetRgnRoiStageRegion(Region); }
					FieldPtr = MatchProjectProgramFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
					if ( NULL == FieldPtr )
					{
						str = ComponentPtr->GetComponentName();
						this->m_ErrorString.Format(_T("Error, Assign Window Field Ptr Fault (C:%s, Window:%d-%d)"), str, j+1, k+1);
						return false;	
					}
				}
				FieldPtr->AddFieldRgnPtr(SubRgnPtr);	
			
				//分配影像區域
				SubRgnPtr->GetRgnRoiStageRegion(Region);
				StagePos3D = FieldPtr->GetFieldStagePos();
				StagePos2D.x = StagePos3D.x;
				StagePos2D.y = StagePos3D.y;
				AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
				JetAPI::Region4DToRect(CameraRgn, FrameRect, true);
				JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬
				if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
				{
					str = ComponentPtr->GetComponentName();
					this->m_ErrorString.Format(_T("Error, Calc Window Image Rect Fault (C:%s, Window:%d-%d)"), str, j+1, k+1);
					return false; 
				}
				//是否計算四個端點呢!?
				AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
				SubRgnPtr->SetRgnFrameImageRect(FrameRect);
				SubRgnPtr->SetRgnFrameImageSize_um(FrameSizeUm);
			}

			FieldPtr = WindowPtr->GetWindowFieldPtr();
			if ( NULL == FieldPtr )
			{	
				if ( true == ByCadRegion )
				{	WindowPtr->GetWindowRoiCadRegion(Region); }
				else
				{	WindowPtr->GetWindowRoiStageRegion(Region); }
				FieldPtr = MatchProjectProgramFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{	
					str = ComponentPtr->GetComponentName();
					this->m_ErrorString.Format(_T("Error, Assign Window Field Ptr Fault (C:%s, Window:%d)"), str, j+1);
					return false;	
				}
			}
			FieldPtr->AddFieldRgnPtr(WindowPtr);			
			//分配影像區域
			WindowPtr->GetWindowRoiStageRegion(Region);
			StagePos3D = FieldPtr->GetFieldStagePos();
			StagePos2D.x = StagePos3D.x;
			StagePos2D.y = StagePos3D.y;
			AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
			JetAPI::Region4DToRect(CameraRgn, FrameRect, true);	
			if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
			{
				if ( 0 == SubRgnCount )
				{
					str = ComponentPtr->GetComponentName();
					this->m_ErrorString.Format(_T("Error, Calc Window Image Rect Fault (C:%s, Window:%d)"), str, j+1);
					return false; 
				}
				if ( FrameRect.left < 0 ) { FrameRect.left = 0; }
				if ( FrameRect.top  < 0 ) { FrameRect.top  = 0; }
				if ( FrameRect.right > nImageW ) { FrameRect.right = nImageW; }
				if ( FrameRect.bottom > nImageH ) { FrameRect.bottom = nImageH; }
			}
			//是否計算四個端點呢!?
			AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
			WindowPtr->SetWindowFrameImageRect(FrameRect);
			WindowPtr->SetWindowFrameImageSize_um(FrameSizeUm);

			TRECT4D FrameRegion;
			TPOINT2D FrameOffsetUm;
			if ( 0 == SubRgnCount )
			{	
				AOIDataCollect.MapCameraRectToStage(CameraID, FrameRect, StagePos2D, FrameRegion);
				FrameOffsetUm.x=FrameRegion.GetCpX()-Region.GetCpX();
				FrameOffsetUm.y=FrameRegion.GetCpY()-Region.GetCpY();
			}
			WindowPtr->SetWindowFrameImageStageOffset_um(FrameOffsetUm);
		}

		SubRgnCount = ComponentPtr->GetComponentSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = ComponentPtr->GetComponentSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }			
			FieldPtr = SubRgnPtr->GetRgnFieldPtr();
			if ( NULL == FieldPtr )
			{	
				if ( true == ByCadRegion )
				{	SubRgnPtr->GetRgnRoiCadRegion(Region); }
				else
				{	SubRgnPtr->GetRgnRoiStageRegion(Region); }
				FieldPtr = MatchProjectProgramFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{	
					str = ComponentPtr->GetComponentName();
					this->m_ErrorString.Format(_T("Error, Assign Component Field Ptr Fault (C:%s-%d)"), str, k+1);
					return false;	
				}
			}
			FieldPtr->AddFieldRgnPtr(SubRgnPtr);

			//分配影像區域
			SubRgnPtr->GetRgnRoiStageRegion(Region);
			StagePos3D = FieldPtr->GetFieldStagePos();
			StagePos2D.x = StagePos3D.x;
			StagePos2D.y = StagePos3D.y;
			AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
			JetAPI::Region4DToRect(CameraRgn, FrameRect, true);
			JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬
			if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
			{
				str = ComponentPtr->GetComponentFullName();
				this->m_ErrorString.Format(_T("Error, Calc Component Image Rect Fault (C:%s-%d)"), str, k+1);
				return false; 
			}
			//是否計算四個端點呢!?
			AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
			SubRgnPtr->SetRgnFrameImageRect(FrameRect);
			SubRgnPtr->SetRgnFrameImageSize_um(FrameSizeUm);
		}

		FieldPtr = ComponentPtr->GetComponentFieldPtr();
		if ( NULL == FieldPtr )
		{	
			if ( true == ByCadRegion )
			{	ComponentPtr->GetComponentRoiCadRegion(Region); }
			else
			{	ComponentPtr->GetComponentRoiStageRegion(Region); }
			FieldPtr = MatchProjectProgramFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
			if ( NULL == FieldPtr )
			{
				str = ComponentPtr->GetComponentName();
				this->m_ErrorString.Format(_T("Error, Assign Component Field Ptr Fault (C:%s)"), str);
				return false;	
			}
		}
		FieldPtr->AddFieldRgnPtr(ComponentPtr);		
		//分配影像區域
		ComponentPtr->GetComponentRoiStageRegion(Region);
		StagePos3D = FieldPtr->GetFieldStagePos();
		StagePos2D.x = StagePos3D.x;
		StagePos2D.y = StagePos3D.y;
		AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
		JetAPI::Region4DToRect(CameraRgn, FrameRect, true);
		JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬
		if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
		{
			if ( 0 == SubRgnCount )
			{
				str = ComponentPtr->GetComponentFullName();
				this->m_ErrorString.Format(_T("Error, Calc Component Image Rect Fault (C:%s)"), str);
				return false; 
			}
			if ( FrameRect.left < 0 ) { FrameRect.left = 0; }
			if ( FrameRect.top  < 0 ) { FrameRect.top  = 0; }
			if ( FrameRect.right > nImageW ) { FrameRect.right = nImageW; }
			if ( FrameRect.bottom > nImageH ) { FrameRect.bottom = nImageH; }
		}
		//是否計算四個端點呢!?
		AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
		ComponentPtr->SetComponentFrameImageRect(FrameRect);	
		ComponentPtr->SetComponentFrameImageSize_um(FrameSizeUm);

		TRECT4D FrameRegion;
		TPOINT2D FrameOffsetUm;
		if ( 0 == SubRgnCount )
		{	
			AOIDataCollect.MapCameraRectToStage(CameraID, FrameRect, StagePos2D, FrameRegion);
			FrameOffsetUm.x=FrameRegion.GetCpX()-Region.GetCpX();
			FrameOffsetUm.y=FrameRegion.GetCpY()-Region.GetCpY();
		}
		ComponentPtr->SetComponentFrameImageStageOffset_um(FrameOffsetUm);
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::AssignProjectInspectionField()//配置Field位置
{
	bool IsOK = true;
	OFFLINE_IMAGE_SCOPE OfflineImageScope=GetProjectOfflineImageScope();
	FIELD_BUILD_MODE FieldBuildMode = GetProjectInspectionFieldBuildMode();
	FIELD_BUILD_AREA_MODE AreaMode= GetProjectInspectionFieldBuildAreaMode();
	if ( OFFLINE_IMAGE_PART == OfflineImageScope )
	{	IsOK = AssignProjectInspectionPartField();	}
	else
	{
		switch ( FieldBuildMode )
		{	
		case FIELD_BUILD_RANDOM_PANEL:
			IsOK = AssignProjectInspectionField_RandomPanel(FieldBuildMode, AreaMode);		
			break;
		case FIELD_BUILD_RANDOM_BOARD:
			IsOK = AssignProjectInspectionField_RandomBoard(FieldBuildMode);
			if ( true == IsOK )
			{	IsOK = AssignProjectInspectionField_RandomPanel(FieldBuildMode, AreaMode);	}
			break;	
		case FIELD_BUILD_RANDOM_PROJECT:
			IsOK = AssignProjectInspectionField_RandomProject(FieldBuildMode);		
			break;
		default:		
			IsOK = AssignProjectInspectionField_Matrix(FieldBuildMode);		
			break;
		}
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::AssignProjectInspectionPartField()//配置檢測區域位置-部分視野
{
	CString        str;
	size_t         i=0, j=0, k=0;	
	size_t         RgnIdx = 0;
	TREGION4D      Region, CameraRgn;	
	IMAGE_SIZE     ImageW=0, ImageH;
	size_t         SubRgnCount = 0;
	size_t         WindowCount = 0;
	RECT           FrameRect={0};	
	CAMERA_ID      CameraID;
	CAOIRgn*       RgnPtr = NULL;
	CAOIRgn*       SubRgnPtr = NULL;
	CAOIFd*        FdPtr = NULL;
	CAOIMark*      MarkPtr = NULL;
	CAOIPanel*     PanelPtr = NULL;
	CAOIBoard*     BoardPtr = NULL;
	CAOIField*     FieldPtr = NULL;	
	CAOIBarcode*   BarcodePtr = NULL;
	CAOIWindow*    WindowPtr = NULL;
	CAOIComponent* ComponentPtr = NULL;
	TSIZE2D        FrameSizeUm;
	TPOINT2D       ImageRes;
	TPOINT2D       StagePos2D;
	TPOINT3D       StagePos3D;		
	const int      nAlign = 4;
	const bool     CheckInner = false;
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();	
	BOARD_FD_GRAB_MODE BoardFdGrabMode = GetProjectBoardFdGrabMode();	
	const size_t   FdCount = GetProjectFdCount();	
	const size_t   MarkCount = GetProjectMarkCount();	
	const size_t   BarcodeCount = GetProjectBarcodeCount();		
	const size_t   ComponentCount = GetProjectComponentCount();
	const size_t   FieldCount = GetProjectInspectionPartFieldCount();
	const bool     ByCadRegion = GetProjectInspectionFieldCadMode_Matrix();

	CameraID = PRIMARY_CAMERA_ID;
	ImageRes.x = AOIDataCollect.GetCameraResolutionX(CameraID);
	ImageRes.y = AOIDataCollect.GetCameraResolutionY(CameraID);
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectInspectionPartFieldPtr(i, false);
		if ( NULL == FieldPtr ) { continue; }
		if ( DistrictID != FieldPtr->GetFieldDistrictID() ) { continue; }
		FieldPtr->RemoveFieldAllRgns();
		FieldPtr->RemoveFieldAllBoards();
	}		
	
	if ( BOARD_FD_GRAB_INSPECTING == BoardFdGrabMode )
	{
		//Assign Field To Fd
		for ( i=0; i<FdCount; i++ )
		{
			FdPtr = GetProjectFdPtr(i, false);
			if ( NULL == FdPtr ) { continue; }
			if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
			if ( FdPtr->GetFdNeedToCalculate() == false ) { continue; }
			BoardPtr = FdPtr->GetFdBoardPtr();
			if ( NULL == BoardPtr ) { continue; }
			
			FieldPtr = FdPtr->GetFdFieldPtr();
			if ( true == ByCadRegion )
			{	FdPtr->GetFdExtendCadRegion(Region); }
			else
			{	FdPtr->GetFdExtendStageRegion(Region); }
			if ( NULL != FieldPtr )
			{	
				if ( FieldPtr->CheckRegionPartInField(Region, ByCadRegion, CheckInner) == false )
				{	FieldPtr = NULL;	}
			}
			if ( NULL == FieldPtr )
			{					
				FieldPtr = MatchProjectInspectionPartFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);				
				if ( NULL == FieldPtr )
				{
					FdPtr->SetFdNeedToCalculate(false);
					this->m_ErrorString.Format(_T("Error, Assign Fd Field Ptr Fault (Fd:%d)"), i+1);
					continue;
				}
			}
			FieldPtr->AddFieldRgnPtr(FdPtr);

			FdPtr->GetFdRoiStageRegion(Region);		
			StagePos3D = FieldPtr->GetFieldStagePos();
			StagePos2D.x = StagePos3D.x;
			StagePos2D.y = StagePos3D.y;
			FieldPtr->CalcFieldImageSize(ImageRes, ImageW, ImageH);		
			AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, Region, StagePos2D, CameraRgn);
			JetAPI::Region4DToRect(CameraRgn, FrameRect, true);		

			//是否計算四個端點呢!?
			AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
			FdPtr->SetRgnDynamicFrameRectMode(true);
			FdPtr->SetFdFrameImageRect(FrameRect);
			FdPtr->SetFdFrameImageSize_um(FrameSizeUm);

			TRECT4D FrameRegion;
			TPOINT2D FrameOffsetUm;
			if ( 0 == SubRgnCount )
			{	
				//AOIDataCollect.MapCameraRectToStage(CameraID, FrameRect, StagePos2D, FrameRegion);
				AOIDataCollect.MapCameraRectToStage(ImageW, ImageH, ImageRes, FrameRect, StagePos2D, FrameRegion);
				FrameOffsetUm.x=FrameRegion.GetCpX()-Region.GetCpX();
				FrameOffsetUm.y=FrameRegion.GetCpY()-Region.GetCpY();
			}
			FdPtr->SetFdFrameImageStageOffset_um(FrameOffsetUm);
		}
	}

	//Assign Field To Mark
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }		
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		if ( MarkPtr->GetMarkNeedToCalculate() == false ) { continue; }		

		FieldPtr = MarkPtr->GetMarkFieldPtr();
		if ( true == ByCadRegion )
		{	MarkPtr->GetMarkRoiCadRegion(Region); }
		else
		{	MarkPtr->GetMarkRoiStageRegion(Region); }
		if ( NULL != FieldPtr )
		{	
			if ( FieldPtr->CheckRegionPartInField(Region, ByCadRegion, CheckInner) == false )
			{	FieldPtr = NULL;	}
		}
		if ( NULL == FieldPtr )
		{	
			FieldPtr = MatchProjectInspectionPartFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);			
			if ( NULL == FieldPtr )
			{
				MarkPtr->SetMarkNeedToCalculate(false);
				this->m_ErrorString.Format(_T("Error, Assign Mark Field Ptr Fault (Mark:%d)"), i+1);
				continue;
			}
		}
		FieldPtr->AddFieldRgnPtr(MarkPtr);

		MarkPtr->GetMarkRoiStageRegion(Region);
		StagePos3D = FieldPtr->GetFieldStagePos();
		StagePos2D.x = StagePos3D.x;
		StagePos2D.y = StagePos3D.y;
		FieldPtr->CalcFieldImageSize(ImageRes, ImageW, ImageH);		
		AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, Region, StagePos2D, CameraRgn);
		JetAPI::Region4DToRect(CameraRgn, FrameRect, true);

		//是否計算四個端點呢!?
		AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
		MarkPtr->SetRgnDynamicFrameRectMode(true);
		MarkPtr->SetMarkFrameImageRect(FrameRect);
		MarkPtr->SetMarkFrameImageSize_um(FrameSizeUm);

		TRECT4D FrameRegion;
		TPOINT2D FrameOffsetUm;
		if ( 0 == SubRgnCount )
		{	
			//AOIDataCollect.MapCameraRectToStage(CameraID, FrameRect, StagePos2D, FrameRegion);
			AOIDataCollect.MapCameraRectToStage(ImageW, ImageH, ImageRes, FrameRect, StagePos2D, FrameRegion);
			FrameOffsetUm.x=FrameRegion.GetCpX()-Region.GetCpX();
			FrameOffsetUm.y=FrameRegion.GetCpY()-Region.GetCpY();
		}
		MarkPtr->SetMarkFrameImageStageOffset_um(FrameOffsetUm);
	}
	
	//Assign Field To Barcode
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }		
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		if ( BarcodePtr->GetBarcodeNeedToCalculate() == false ) { continue; }

		FieldPtr = BarcodePtr->GetBarcodeFieldPtr();
		if ( true == ByCadRegion )
		{	BarcodePtr->GetBarcodeRoiCadRegion(Region); }
		else
		{	BarcodePtr->GetBarcodeRoiStageRegion(Region); }
		if ( NULL != FieldPtr )
		{	
			if ( FieldPtr->CheckRegionPartInField(Region, ByCadRegion, CheckInner) == false )
			{	FieldPtr = NULL;	}
		}
		if ( NULL == FieldPtr )
		{	
			FieldPtr = MatchProjectInspectionPartFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);			
			if ( NULL == FieldPtr )
			{
				BarcodePtr->SetBarcodeNeedToCalculate(false);
				this->m_ErrorString.Format(_T("Error, Assign Barcode Field Ptr Fault (Barcode:%d)"), i+1);
				continue;
			}
		}
		FieldPtr->AddFieldRgnPtr(BarcodePtr);

		BarcodePtr->GetBarcodeRoiStageRegion(Region);
		StagePos3D = FieldPtr->GetFieldStagePos();
		StagePos2D.x = StagePos3D.x;
		StagePos2D.y = StagePos3D.y;
		FieldPtr->CalcFieldImageSize(ImageRes, ImageW, ImageH);		
		AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, Region, StagePos2D, CameraRgn);
		JetAPI::Region4DToRect(CameraRgn, FrameRect, true);		

		//是否計算四個端點呢!?
		AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
		BarcodePtr->SetRgnDynamicFrameRectMode(true);
		BarcodePtr->SetBarcodeFrameImageRect(FrameRect);
		BarcodePtr->SetBarcodeFrameImageSize_um(FrameSizeUm);

		TRECT4D FrameRegion;
		TPOINT2D FrameOffsetUm;
		if ( 0 == SubRgnCount )
		{	
			//AOIDataCollect.MapCameraRectToStage(CameraID, FrameRect, StagePos2D, FrameRegion);
			AOIDataCollect.MapCameraRectToStage(ImageW, ImageH, ImageRes, FrameRect, StagePos2D, FrameRegion);
			FrameOffsetUm.x=FrameRegion.GetCpX()-Region.GetCpX();
			FrameOffsetUm.y=FrameRegion.GetCpY()-Region.GetCpY();
		}
		BarcodePtr->SetBarcodeFrameImageStageOffset_um(FrameOffsetUm);
	}
	
	//Assign Field To Component Window, window first then component
	for ( i=0; i<ComponentCount; i++ )
	{	
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		if ( ComponentPtr->GetComponentNeedToCalculate() == false ) { continue; }

		//to Window		
		WindowCount = ComponentPtr->GetComponentWindowCount();
		for ( j=0; j<WindowCount; j++ )
		{
			WindowPtr = ComponentPtr->GetComponentWindowPtr(j, false);
			if ( NULL == WindowPtr ) { continue; }			
			
			FieldPtr = WindowPtr->GetWindowFieldPtr();
			if ( true == ByCadRegion )
			{	WindowPtr->GetWindowRoiCadRegion(Region); }
			else
			{	WindowPtr->GetWindowRoiStageRegion(Region); }
			if ( NULL != FieldPtr )
			{	
				if ( FieldPtr->CheckRegionPartInField(Region, ByCadRegion, CheckInner) == false )
				{	FieldPtr = NULL;	}
			}
			if ( NULL == FieldPtr )
			{	
				FieldPtr = MatchProjectInspectionPartFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{
					WindowPtr->SetRgnNeedToCalculate(false);
					str = ComponentPtr->GetComponentFullName();					
					this->m_ErrorString.Format(_T("Error, Assign Window Field Ptr Fault (C:%s, Window:%d)"), str, j+1);
					continue;
				}
			}
			
			FieldPtr->AddFieldRgnPtr(WindowPtr);			
			//分配影像區域
			WindowPtr->GetWindowRoiStageRegion(Region);
			StagePos3D = FieldPtr->GetFieldStagePos();
			StagePos2D.x = StagePos3D.x;
			StagePos2D.y = StagePos3D.y;
			FieldPtr->CalcFieldImageSize(ImageRes, ImageW, ImageH);		
			AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, Region, StagePos2D, CameraRgn);
			JetAPI::Region4DToRect(CameraRgn, FrameRect, true);				
			
			//是否計算四個端點呢!?
			AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
			WindowPtr->SetRgnDynamicFrameRectMode(true);
			WindowPtr->SetWindowFrameImageRect(FrameRect);
			WindowPtr->SetWindowFrameImageSize_um(FrameSizeUm);

			TRECT4D FrameRegion;
			TPOINT2D FrameOffsetUm;
			if ( 0 == SubRgnCount )
			{	
				//AOIDataCollect.MapCameraRectToStage(CameraID, FrameRect, StagePos2D, FrameRegion);
				AOIDataCollect.MapCameraRectToStage(ImageW, ImageH, ImageRes, FrameRect, StagePos2D, FrameRegion);
				FrameOffsetUm.x=FrameRegion.GetCpX()-Region.GetCpX();
				FrameOffsetUm.y=FrameRegion.GetCpY()-Region.GetCpY();
			}
			WindowPtr->SetWindowFrameImageStageOffset_um(FrameOffsetUm);
		}		
		
		FieldPtr = ComponentPtr->GetComponentFieldPtr();
		if ( true == ByCadRegion )
		{	ComponentPtr->GetComponentRoiCadRegion(Region); }
		else
		{	ComponentPtr->GetComponentRoiStageRegion(Region); }
		if ( NULL != FieldPtr )
		{	
			if ( FieldPtr->CheckRegionPartInField(Region, ByCadRegion, CheckInner) == false )
			{	FieldPtr = NULL;	}
		}
		if ( NULL == FieldPtr )
		{	
			FieldPtr = MatchProjectInspectionPartFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
			if ( NULL == FieldPtr )
			{				
				str = ComponentPtr->GetComponentFullName();
				ComponentPtr->SetComponentNeedToCalculate(false);
				this->m_ErrorString.Format(_T("Error, Assign Component Field Ptr Fault (C:%s)"), str);
				continue;
			}
		}
		FieldPtr->AddFieldRgnPtr(ComponentPtr);

		//分配影像區域
		ComponentPtr->GetComponentRoiStageRegion(Region);
		StagePos3D = FieldPtr->GetFieldStagePos();
		StagePos2D.x = StagePos3D.x;
		StagePos2D.y = StagePos3D.y;
		FieldPtr->CalcFieldImageSize(ImageRes, ImageW, ImageH);		
		AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, Region, StagePos2D, CameraRgn);
		JetAPI::Region4DToRect(CameraRgn, FrameRect, true);
		JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬		

		//是否計算四個端點呢!?
		AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
		ComponentPtr->SetRgnDynamicFrameRectMode(true);
		ComponentPtr->SetComponentFrameImageRect(FrameRect);
		ComponentPtr->SetComponentFrameImageSize_um(FrameSizeUm);

		TRECT4D FrameRegion;
		TPOINT2D FrameOffsetUm;
		if ( 0 == SubRgnCount )
		{	
			//AOIDataCollect.MapCameraRectToStage(CameraID, FrameRect, StagePos2D, FrameRegion);
			AOIDataCollect.MapCameraRectToStage(ImageW, ImageH, ImageRes, FrameRect, StagePos2D, FrameRegion);
			FrameOffsetUm.x=FrameRegion.GetCpX()-Region.GetCpX();
			FrameOffsetUm.y=FrameRegion.GetCpY()-Region.GetCpY();
		}
		ComponentPtr->SetComponentFrameImageStageOffset_um(FrameOffsetUm);
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::AssignProjectInspectionField_Matrix(FIELD_BUILD_MODE BuildMode)//配置檢測區域位置	
{
	CString        str;
	size_t         i=0, j=0, k=0;	
	size_t         RgnIdx = 0;
	TREGION4D      Region, CameraRgn;	
	IMAGE_SIZE     ImageW=0, ImageH;
	size_t         SubRgnCount = 0;
	size_t         WindowCount = 0;
	RECT           FrameRect={0};	
	CAMERA_ID      CameraID;
	CAOIRgn*       RgnPtr = NULL;
	CAOIRgn*       SubRgnPtr = NULL;
	CAOIFd*        FdPtr = NULL;
	CAOIMark*      MarkPtr = NULL;
	CAOIPanel*     PanelPtr = NULL;
	CAOIBoard*     BoardPtr = NULL;
	CAOIField*     FieldPtr = NULL;	
	CAOIBarcode*   BarcodePtr = NULL;
	CAOIWindow*    WindowPtr = NULL;
	CAOIComponent* ComponentPtr = NULL;
	TSIZE2D        FrameSizeUm;
	TPOINT2D       StagePos2D;
	TPOINT3D       StagePos3D;		
	const int      nAlign = 4;
	const bool     CheckInner = false;		
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	BOARD_FD_GRAB_MODE BoardFdGrabMode = GetProjectBoardFdGrabMode();	
	const size_t   FdCount = GetProjectFdCount();	
	const size_t   MarkCount = GetProjectMarkCount();	
	const size_t   BarcodeCount = GetProjectBarcodeCount();		
	const size_t   ComponentCount = GetProjectComponentCount();
	const size_t   FieldCount = GetProjectInspectionFieldCount();
	const bool     ByCadRegion = GetProjectInspectionFieldCadMode_Matrix();

	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectInspectionFieldPtr(i, false);
		if ( NULL == FieldPtr ) { continue; }
		if ( DistrictID != FieldPtr->GetFieldDistrictID() ) { continue; }
		FieldPtr->RemoveFieldAllRgns();
		FieldPtr->RemoveFieldAllBoards();
	}	
	
	CameraID = PRIMARY_CAMERA_ID;
	ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	ImageH = AOIDataCollect.GetCameraImageH(CameraID);
	const int nImageW = (int)(ImageW);
	const int nImageH = (int)(ImageH);

	if ( BOARD_FD_GRAB_INSPECTING == BoardFdGrabMode )
	{
		//Assign Field To Fd
		for ( i=0; i<FdCount; i++ )
		{
			FdPtr = GetProjectFdPtr(i, false);
			if ( NULL == FdPtr ) { continue; }
			if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
			if ( FdPtr->GetFdNeedToCalculate() == false ) { continue; }
			BoardPtr = FdPtr->GetFdBoardPtr();
			if ( NULL == BoardPtr ) { continue; }			

			SubRgnCount = FdPtr->GetFdSubRgnCount();
			for ( k=0; k<SubRgnCount; k++ )
			{
				SubRgnPtr = FdPtr->GetFdSubRgnPtr(k, false);
				if ( NULL == SubRgnPtr ) { continue; }
				FieldPtr = SubRgnPtr->GetRgnFieldPtr();
				if ( NULL == FieldPtr )
				{	
					if ( true == ByCadRegion )
					{	SubRgnPtr->GetRgnRoiCadRegion(Region); }
					else
					{	SubRgnPtr->GetRgnRoiStageRegion(Region); }
					FieldPtr = MatchProjectInspectionFieldPtr_Matrix(Region, ByCadRegion, CheckInner, DistrictID);
					if ( NULL == FieldPtr )
					{
						this->m_ErrorString.Format(_T("Error, Assign Fd Field Ptr Fault (Fd:%d-%d)"), i+1, k+1);
						return false;	
					}
				}
				FieldPtr->AddFieldRgnPtr(SubRgnPtr);	
			
				//分配影像區域
				SubRgnPtr->GetRgnRoiStageRegion(Region);
				StagePos3D = FieldPtr->GetFieldStagePos();
				StagePos2D.x = StagePos3D.x;
				StagePos2D.y = StagePos3D.y;
				AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
				JetAPI::Region4DToRect(CameraRgn, FrameRect, true);
				JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬
				if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
				{
					this->m_ErrorString.Format(_T("Error, Calc Fd Image Rect Fault (Fd:%d-%d)"), i+1, k+1);
					return false; 
				}
				//是否計算四個端點呢!?
				AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
				SubRgnPtr->SetRgnFrameImageRect(FrameRect);
				SubRgnPtr->SetRgnFrameImageSize_um(FrameSizeUm);
			}

			FieldPtr = FdPtr->GetFdFieldPtr();
			if ( NULL == FieldPtr )
			{	
				if ( true == ByCadRegion )
				{	FdPtr->GetFdExtendCadRegion(Region); }
				else
				{	FdPtr->GetFdExtendStageRegion(Region); }
				FieldPtr = MatchProjectInspectionFieldPtr_Matrix(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{
					this->m_ErrorString.Format(_T("Error, Assign Fd Field Ptr Fault (Fd:%d)"), i+1);
					return false;	
				}
			}
			FieldPtr->AddFieldRgnPtr(FdPtr);

			FdPtr->GetFdRoiStageRegion(Region);		
			StagePos3D = FieldPtr->GetFieldStagePos();
			StagePos2D.x = StagePos3D.x;
			StagePos2D.y = StagePos3D.y;
			AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
			JetAPI::Region4DToRect(CameraRgn, FrameRect, true);			
			if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
			{
				if ( 0 == SubRgnCount )
				{
					this->m_ErrorString.Format(_T("Error, Calc Fd Image Rect Fault (Fd:%d)"), i+1);
					return false; 
				}
				if ( FrameRect.left < 0 ) { FrameRect.left = 0; }
				if ( FrameRect.top  < 0 ) { FrameRect.top  = 0; }
				if ( FrameRect.right > nImageW ) { FrameRect.right = nImageW; }
				if ( FrameRect.bottom > nImageH ) { FrameRect.bottom = nImageH; }
			}
		
			//是否計算四個端點呢!?
			AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
			FdPtr->SetFdFrameImageRect(FrameRect);
			FdPtr->SetFdFrameImageSize_um(FrameSizeUm);

			TRECT4D FrameRegion;
			TPOINT2D FrameOffsetUm;
			if ( 0 == SubRgnCount )
			{	
				AOIDataCollect.MapCameraRectToStage(CameraID, FrameRect, StagePos2D, FrameRegion);
				FrameOffsetUm.x=FrameRegion.GetCpX()-Region.GetCpX();
				FrameOffsetUm.y=FrameRegion.GetCpY()-Region.GetCpY();
			}
			FdPtr->SetFdFrameImageStageOffset_um(FrameOffsetUm);
		}
	}

	//Assign Field To Mark
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }		
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		if ( MarkPtr->GetMarkNeedToCalculate() == false ) { continue; }

		SubRgnCount = MarkPtr->GetMarkSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = MarkPtr->GetMarkSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			FieldPtr = SubRgnPtr->GetRgnFieldPtr();
			if ( NULL == FieldPtr )
			{	
				if ( true == ByCadRegion )
				{	SubRgnPtr->GetRgnRoiCadRegion(Region); }
				else
				{	SubRgnPtr->GetRgnRoiStageRegion(Region); }
				FieldPtr = MatchProjectInspectionFieldPtr_Matrix(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{
					this->m_ErrorString.Format(_T("Error, Assign Mark Field Ptr Fault (Mark:%d-%d)"), i+1, k+1);
					return false;	
				}
			}
			FieldPtr->AddFieldRgnPtr(SubRgnPtr);	
			
			//分配影像區域
			SubRgnPtr->GetRgnRoiStageRegion(Region);
			StagePos3D = FieldPtr->GetFieldStagePos();
			StagePos2D.x = StagePos3D.x;
			StagePos2D.y = StagePos3D.y;
			AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
			JetAPI::Region4DToRect(CameraRgn, FrameRect, true);
			JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬
			if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
			{
				this->m_ErrorString.Format(_T("Error, Calc Mark Image Rect Fault (Mark:%d-%d)"), i+1, k+1);
				return false; 
			}
			//是否計算四個端點呢!?
			AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
			SubRgnPtr->SetRgnFrameImageRect(FrameRect);
			SubRgnPtr->SetRgnFrameImageSize_um(FrameSizeUm);
		}

		FieldPtr = MarkPtr->GetMarkFieldPtr();
		if ( NULL == FieldPtr )
		{	
			if ( true == ByCadRegion )
			{	MarkPtr->GetMarkRoiCadRegion(Region); }
			else
			{	MarkPtr->GetMarkRoiStageRegion(Region); }
			FieldPtr = MatchProjectInspectionFieldPtr_Matrix(Region, ByCadRegion, CheckInner, DistrictID);
			if ( NULL == FieldPtr )
			{
				this->m_ErrorString.Format(_T("Error, Assign Mark Field Ptr Fault (Mark:%d)"), i+1);
				return false;	
			}
		}
		FieldPtr->AddFieldRgnPtr(MarkPtr);

		MarkPtr->GetMarkRoiStageRegion(Region);
		StagePos3D = FieldPtr->GetFieldStagePos();
		StagePos2D.x = StagePos3D.x;
		StagePos2D.y = StagePos3D.y;
		AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
		JetAPI::Region4DToRect(CameraRgn, FrameRect, true);			
		if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
		{
			if ( 0 == SubRgnCount )
			{
				this->m_ErrorString.Format(_T("Error, Calc Mark Image Rect Fault (Mark:%d)"), i+1);
				return false; 
			}
			if ( FrameRect.left < 0 ) { FrameRect.left = 0; }
			if ( FrameRect.top  < 0 ) { FrameRect.top  = 0; }
			if ( FrameRect.right > nImageW ) { FrameRect.right = nImageW; }
			if ( FrameRect.bottom > nImageH ) { FrameRect.bottom = nImageH; }
		}
		
		//是否計算四個端點呢!?
		AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
		MarkPtr->SetMarkFrameImageRect(FrameRect);
		MarkPtr->SetMarkFrameImageSize_um(FrameSizeUm);

		TRECT4D FrameRegion;
		TPOINT2D FrameOffsetUm;
		if ( 0 == SubRgnCount )
		{	
			AOIDataCollect.MapCameraRectToStage(CameraID, FrameRect, StagePos2D, FrameRegion);
			FrameOffsetUm.x=FrameRegion.GetCpX()-Region.GetCpX();
			FrameOffsetUm.y=FrameRegion.GetCpY()-Region.GetCpY();
		}
		MarkPtr->SetMarkFrameImageStageOffset_um(FrameOffsetUm);
	}

	//Assign Field To Barcode
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }		
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		if ( BarcodePtr->GetBarcodeNeedToCalculate() == false ) { continue; }

		SubRgnCount = BarcodePtr->GetBarcodeSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = BarcodePtr->GetBarcodeSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			FieldPtr = SubRgnPtr->GetRgnFieldPtr();
			if ( NULL == FieldPtr )
			{	
				if ( true == ByCadRegion )
				{	SubRgnPtr->GetRgnRoiCadRegion(Region); }
				else
				{	SubRgnPtr->GetRgnRoiStageRegion(Region); }
				FieldPtr = MatchProjectInspectionFieldPtr_Matrix(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{
					this->m_ErrorString.Format(_T("Error, Assign Barcode Field Ptr Fault (Barcode:%d-%d)"), i+1, k+1);
					return false;	
				}
			}
			FieldPtr->AddFieldRgnPtr(SubRgnPtr);	
			
			//分配影像區域
			SubRgnPtr->GetRgnRoiStageRegion(Region);
			StagePos3D = FieldPtr->GetFieldStagePos();
			StagePos2D.x = StagePos3D.x;
			StagePos2D.y = StagePos3D.y;
			AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
			JetAPI::Region4DToRect(CameraRgn, FrameRect, true);
			JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬
			if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
			{
				this->m_ErrorString.Format(_T("Error, Calc Barcode Image Rect Fault (Barcode:%d-%d)"), i+1, k+1);
				return false; 
			}
			//是否計算四個端點呢!?
			AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
			SubRgnPtr->SetRgnFrameImageRect(FrameRect);
			SubRgnPtr->SetRgnFrameImageSize_um(FrameSizeUm);
		}

		FieldPtr = BarcodePtr->GetBarcodeFieldPtr();
		if ( NULL == FieldPtr )
		{	
			if ( true == ByCadRegion )
			{	BarcodePtr->GetBarcodeRoiCadRegion(Region); }
			else
			{	BarcodePtr->GetBarcodeRoiStageRegion(Region); }
			FieldPtr = MatchProjectInspectionFieldPtr_Matrix(Region, ByCadRegion, CheckInner, DistrictID);
			if ( NULL == FieldPtr )
			{
				this->m_ErrorString.Format(_T("Error, Assign Barcode Field Ptr Fault (Barcode:%d)"), i+1);
				return false;	
			}
		}
		FieldPtr->AddFieldRgnPtr(BarcodePtr);

		BarcodePtr->GetBarcodeRoiStageRegion(Region);
		StagePos3D = FieldPtr->GetFieldStagePos();
		StagePos2D.x = StagePos3D.x;
		StagePos2D.y = StagePos3D.y;
		AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
		JetAPI::Region4DToRect(CameraRgn, FrameRect, true);			
		if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
		{
			if ( 0 == SubRgnCount )
			{
				this->m_ErrorString.Format(_T("Error, Calc Barcode Image Rect Fault (Barcode:%d)"), i+1);
				return false; 
			}
			if ( FrameRect.left < 0 ) { FrameRect.left = 0; }
			if ( FrameRect.top  < 0 ) { FrameRect.top  = 0; }
			if ( FrameRect.right > nImageW ) { FrameRect.right = nImageW; }
			if ( FrameRect.bottom > nImageH ) { FrameRect.bottom = nImageH; }
		}
		
		//是否計算四個端點呢!?
		AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
		BarcodePtr->SetBarcodeFrameImageRect(FrameRect);
		BarcodePtr->SetBarcodeFrameImageSize_um(FrameSizeUm);

		TRECT4D FrameRegion;
		TPOINT2D FrameOffsetUm;
		if ( 0 == SubRgnCount )
		{	
			AOIDataCollect.MapCameraRectToStage(CameraID, FrameRect, StagePos2D, FrameRegion);
			FrameOffsetUm.x=FrameRegion.GetCpX()-Region.GetCpX();
			FrameOffsetUm.y=FrameRegion.GetCpY()-Region.GetCpY();
		}
		BarcodePtr->SetBarcodeFrameImageStageOffset_um(FrameOffsetUm);
	}
	
	//Assign Field To Component Window, window first then component
	for ( i=0; i<ComponentCount; i++ )
	{	
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		if ( ComponentPtr->GetComponentNeedToCalculate() == false ) { continue; }

		//to Window
		WindowCount = ComponentPtr->GetComponentWindowCount();
		for ( j=0; j<WindowCount; j++ )
		{
			WindowPtr = ComponentPtr->GetComponentWindowPtr(j, false);
			if ( NULL == WindowPtr ) { continue; }
			
			SubRgnCount = WindowPtr->GetWindowSubRgnCount();
			for ( k=0; k<SubRgnCount; k++ )
			{
				SubRgnPtr = WindowPtr->GetWindowSubRgnPtr(k, false);
				if ( NULL == SubRgnPtr ) { continue; }
				FieldPtr = SubRgnPtr->GetRgnFieldPtr();
				if ( NULL == FieldPtr )
				{	
					if ( true == ByCadRegion )
					{	SubRgnPtr->GetRgnRoiCadRegion(Region); }
					else
					{	SubRgnPtr->GetRgnRoiStageRegion(Region); }
					FieldPtr = MatchProjectInspectionFieldPtr_Matrix(Region, ByCadRegion, CheckInner, DistrictID);
					if ( NULL == FieldPtr )
					{
						str = ComponentPtr->GetComponentFullName();
						this->m_ErrorString.Format(_T("Error, Assign Window Field Ptr Fault (C:%s, Window:%d-%d)"), str, j+1, k+1);
						return false;	
					}
				}
				FieldPtr->AddFieldRgnPtr(SubRgnPtr);	
			
				//分配影像區域
				SubRgnPtr->GetRgnRoiStageRegion(Region);
				StagePos3D = FieldPtr->GetFieldStagePos();
				StagePos2D.x = StagePos3D.x;
				StagePos2D.y = StagePos3D.y;
				AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
				JetAPI::Region4DToRect(CameraRgn, FrameRect, true);
				JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬
				if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
				{
					str = ComponentPtr->GetComponentFullName();
					this->m_ErrorString.Format(_T("Error, Calc Window Image Rect Fault (C:%s, Window:%d-%d)"), str, j+1, k+1);
					return false; 
				}
				//是否計算四個端點呢!?
				AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
				SubRgnPtr->SetRgnFrameImageRect(FrameRect);
				SubRgnPtr->SetRgnFrameImageSize_um(FrameSizeUm);
			}

			FieldPtr = WindowPtr->GetWindowFieldPtr();
			if ( NULL == FieldPtr )
			{	
				if ( true == ByCadRegion )
				{	WindowPtr->GetWindowRoiCadRegion(Region); }
				else
				{	WindowPtr->GetWindowRoiStageRegion(Region); }
				FieldPtr = MatchProjectInspectionFieldPtr_Matrix(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{	
					str = ComponentPtr->GetComponentFullName();
					this->m_ErrorString.Format(_T("Error, Assign Window Field Ptr Fault (C:%s, Window:%d)"), str, j+1);
					return false;	
				}
			}
			FieldPtr->AddFieldRgnPtr(WindowPtr);			
			//分配影像區域
			WindowPtr->GetWindowRoiStageRegion(Region);
			StagePos3D = FieldPtr->GetFieldStagePos();
			StagePos2D.x = StagePos3D.x;
			StagePos2D.y = StagePos3D.y;
			AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
			JetAPI::Region4DToRect(CameraRgn, FrameRect, true);	
			if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
			{
				if ( 0 == SubRgnCount )
				{
					str = ComponentPtr->GetComponentFullName();
					this->m_ErrorString.Format(_T("Error, Calc Window Image Rect Fault (C:%s, Window:%d)"), str, j+1);
					return false; 
				}
				if ( FrameRect.left < 0 ) { FrameRect.left = 0; }
				if ( FrameRect.top  < 0 ) { FrameRect.top  = 0; }
				if ( FrameRect.right > nImageW ) { FrameRect.right = nImageW; }
				if ( FrameRect.bottom > nImageH ) { FrameRect.bottom = nImageH; }
			}
			//是否計算四個端點呢!?
			AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
			WindowPtr->SetWindowFrameImageRect(FrameRect);
			WindowPtr->SetWindowFrameImageSize_um(FrameSizeUm);

			TRECT4D FrameRegion;
			TPOINT2D FrameOffsetUm;
			if ( 0 == SubRgnCount )
			{	
				AOIDataCollect.MapCameraRectToStage(CameraID, FrameRect, StagePos2D, FrameRegion);
				FrameOffsetUm.x=FrameRegion.GetCpX()-Region.GetCpX();
				FrameOffsetUm.y=FrameRegion.GetCpY()-Region.GetCpY();
			}
			WindowPtr->SetWindowFrameImageStageOffset_um(FrameOffsetUm);
		}

		SubRgnCount = ComponentPtr->GetComponentSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = ComponentPtr->GetComponentSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }			
			FieldPtr = SubRgnPtr->GetRgnFieldPtr();
			if ( NULL == FieldPtr )
			{	
				if ( true == ByCadRegion )
				{	SubRgnPtr->GetRgnRoiCadRegion(Region); }
				else
				{	SubRgnPtr->GetRgnRoiStageRegion(Region); }
				FieldPtr = MatchProjectInspectionFieldPtr_Matrix(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{
					str = ComponentPtr->GetComponentFullName();
					this->m_ErrorString.Format(_T("Error, Assign Component Field Ptr Fault (C:%s-%d)"), str, k+1);
					return false;	
				}
			}
			FieldPtr->AddFieldRgnPtr(SubRgnPtr);

			//分配影像區域
			SubRgnPtr->GetRgnRoiStageRegion(Region);
			StagePos3D = FieldPtr->GetFieldStagePos();
			StagePos2D.x = StagePos3D.x;
			StagePos2D.y = StagePos3D.y;
			AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
			JetAPI::Region4DToRect(CameraRgn, FrameRect, true);
			JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬
			if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
			{
				str = ComponentPtr->GetComponentFullName();
				this->m_ErrorString.Format(_T("Error, Calc Component Image Rect Fault (C:%s-%d)"), str, k+1);
				return false; 
			}
			//是否計算四個端點呢!?
			AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
			SubRgnPtr->SetRgnFrameImageRect(FrameRect);
			SubRgnPtr->SetRgnFrameImageSize_um(FrameSizeUm);
		}

		FieldPtr = ComponentPtr->GetComponentFieldPtr();
		if ( NULL == FieldPtr )
		{	
			if ( true == ByCadRegion )
			{	ComponentPtr->GetComponentRoiCadRegion(Region); }
			else
			{	ComponentPtr->GetComponentRoiStageRegion(Region); }
			FieldPtr = MatchProjectInspectionFieldPtr_Matrix(Region, ByCadRegion, CheckInner, DistrictID);
			if ( NULL == FieldPtr )
			{
				str = ComponentPtr->GetComponentFullName();
				this->m_ErrorString.Format(_T("Error, Assign Component Field Ptr Fault (C:%s)"), str);
				return false;	
			}
		}
		FieldPtr->AddFieldRgnPtr(ComponentPtr);		
		//分配影像區域
		ComponentPtr->GetComponentRoiStageRegion(Region);
		StagePos3D = FieldPtr->GetFieldStagePos();
		StagePos2D.x = StagePos3D.x;
		StagePos2D.y = StagePos3D.y;
		AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);		
		JetAPI::Region4DToRect(CameraRgn, FrameRect, true);
		JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬
		if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
		{
			if ( 0 == SubRgnCount )
			{
				str = ComponentPtr->GetComponentFullName();
				this->m_ErrorString.Format(_T("Error, Calc Component Image Rect Fault (C:%s)"), str);
				return false; 
			}
			if ( FrameRect.left < 0 ) { FrameRect.left = 0; }
			if ( FrameRect.top  < 0 ) { FrameRect.top  = 0; }
			if ( FrameRect.right > nImageW ) { FrameRect.right = nImageW; }
			if ( FrameRect.bottom > nImageH ) { FrameRect.bottom = nImageH; }
		}
		//是否計算四個端點呢!?
		AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
		ComponentPtr->SetComponentFrameImageRect(FrameRect);
		ComponentPtr->SetComponentFrameImageSize_um(FrameSizeUm);

		TRECT4D FrameRegion;
		TPOINT2D FrameOffsetUm;
		if ( 0 == SubRgnCount )
		{	
			AOIDataCollect.MapCameraRectToStage(CameraID, FrameRect, StagePos2D, FrameRegion);
			FrameOffsetUm.x=FrameRegion.GetCpX()-Region.GetCpX();
			FrameOffsetUm.y=FrameRegion.GetCpY()-Region.GetCpY();
		}
		ComponentPtr->SetComponentFrameImageStageOffset_um(FrameOffsetUm);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::AssignProjectInspectionField_RandomPanel(FIELD_BUILD_MODE BuildMode, FIELD_BUILD_AREA_MODE AreaMode)//配置檢測區域位置	
{
	CString      str;	
	CString      strError;	
	size_t       i=0, j=0;	
	CAOIPanel   *PanelPtr=NULL;
	DISTRICT_ID  DistrictID = GetProjectActDistrictID();
	CString      strPanel = AOIDataDefine.GetPanelText();
	const size_t PanelCount = GetProjectPanelCount();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }		
		if ( PanelPtr->AssignPanelFieldList(BuildMode, DistrictID, AreaMode) == false )
		{
			str = _T("Error, AssignPanelFieldList Fault");
			str = LoadMultiLanguageString(str, str);
			strError = PanelPtr->GetPanelErrorString();
			m_ErrorString.Format(_T("%s [%s:%d] [%s]"), str, strPanel, i+1, strError);
			return false;
		}	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::AssignProjectInspectionField_RandomBoard(FIELD_BUILD_MODE BuildMode)//配置檢測區域位置	
{
	CString      str;	
	CString      strError;	
	size_t       i=0, j=0;
	CAOIBoard   *BoardPtr=NULL;
	DISTRICT_ID  DistrictID = GetProjectActDistrictID();
	CString      strBoard = AOIDataDefine.GetBoardText();
	const size_t BoardCount = GetProjectBoardCount();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }		
		if ( BoardPtr->AssignBoardFieldList(BuildMode, DistrictID) == false )
		{
			str = _T("Error, AssignBoardFieldList Fault");
			str = LoadMultiLanguageString(str, str);
			strError = BoardPtr->GetBoardErrorString();
			m_ErrorString.Format(_T("%s [%s:%d] [%s]"), str, strBoard, i+1, strError);
			return false;
		}	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::AssignProjectInspectionField_RandomProject(FIELD_BUILD_MODE BuildMode)//配置檢測區域位置-任意位置-專案
{
	CString      str;	
	CString      strError;
	DISTRICT_ID  DistrictID = GetProjectActDistrictID();		
	if ( AssignProjectFieldList(DistrictID) == false )
	{
		strError = GetErrorString();
		str = _T("Error, AssignProjectFieldList Fault");
		str = LoadMultiLanguageString(str, str);		
		m_ErrorString.Format(_T("%s [%s]"), str, strError);
		return false;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIField* CAOIProject::MatchProjectProgramFieldPtr(const TREGION4D &Region, bool CadMode, bool Inner, DISTRICT_ID DistrictID)
{
	size_t i = 0;
	double      Dist = 0;
	double      DistX=0, DistY=0;
	double      FieldCpX=0, FieldCpY=0;
	double      BestDist = 0;
	CAOIField*  FieldPtr = NULL;
	CAOIField*  BestFieldPtr = NULL;
	const double RegionCpX = Region.GetCpX();
	const double RegionCpY = Region.GetCpY();
	const size_t FieldCount = GetProjectProgramFieldCount();

	BestFieldPtr = NULL;
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectProgramFieldPtr(i, false);
		if ( NULL == FieldPtr ) { continue; }
		if ( DistrictID != FieldPtr->GetFieldDistrictID() ) { continue; }
		if ( FieldPtr->CheckRegionInField(Region, CadMode, Inner) == false ) { continue; }

		if ( true == CadMode )
		{
			FieldCpX = FieldPtr->GetFieldCadPosX();
			FieldCpY = FieldPtr->GetFieldCadPosY();
		}
		else
		{
			FieldCpX = FieldPtr->GetFieldStagePosX();
			FieldCpY = FieldPtr->GetFieldStagePosY();			
		}
		DistX = RegionCpX-FieldCpX;
		DistY = RegionCpY-FieldCpY;
		Dist = sqrt((DistX*DistX)+(DistY*DistY));//找最近的
		if ( (NULL==BestFieldPtr) || (Dist<BestDist) ) 
		{
			BestDist = Dist;
			BestFieldPtr = FieldPtr; 
		}		
	}
	return BestFieldPtr;
}
//-------------------------------------------------------------------------------------//
CAOIField* CAOIProject::MatchProjectInspectionPartFieldPtr(const TREGION4D &Region, bool CadMode, bool Inner, DISTRICT_ID DistrictID)
{
	size_t      i = 0;
	double      Dist = 0;
	double      DistX=0, DistY=0;
	double      FieldCpX=0, FieldCpY=0;
	double      BestDist = 0;
	CAOIField*  FieldPtr = NULL;
	CAOIField*  BestFieldPtr = NULL;
	const double RegionCpX = Region.GetCpX();
	const double RegionCpY = Region.GetCpY();
	const size_t FieldCount = GetProjectInspectionPartFieldCount();

	BestFieldPtr = NULL;
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectInspectionPartFieldPtr(i, false);
		if ( NULL == FieldPtr ) { continue; }
		if ( DistrictID != FieldPtr->GetFieldDistrictID() ) { continue; }
		//if ( FieldPtr->CheckRegionInField(Region, CadMode, Inner) == false ) { continue; }
		if ( FieldPtr->CheckRegionPartInField(Region, CadMode, Inner) == false ) { continue; }		

		if ( true == CadMode )
		{
			FieldCpX = FieldPtr->GetFieldCadPosX();
			FieldCpY = FieldPtr->GetFieldCadPosY();
		}
		else
		{
			FieldCpX = FieldPtr->GetFieldStagePosX();
			FieldCpY = FieldPtr->GetFieldStagePosY();			
		}
		DistX = RegionCpX-FieldCpX;
		DistY = RegionCpY-FieldCpY;
		Dist = sqrt((DistX*DistX)+(DistY*DistY));//找最近的
		if ( (NULL==BestFieldPtr) || (Dist<BestDist) ) 
		{
			BestDist = Dist;
			BestFieldPtr = FieldPtr; 
		}		
	}
	return BestFieldPtr;
}
//-------------------------------------------------------------------------------------//
CAOIField* CAOIProject::MatchProjectInspectionFieldPtr_Matrix(const TREGION4D &Region, bool CadMode, bool Inner, DISTRICT_ID DistrictID)
{
	size_t      i = 0;
	double      Dist = 0;
	double      DistX=0, DistY=0;
	double      FieldCpX=0, FieldCpY=0;
	double      BestDist = 0;
	CAOIField*  FieldPtr = NULL;
	CAOIField*  BestFieldPtr = NULL;
	const double RegionCpX = Region.GetCpX();
	const double RegionCpY = Region.GetCpY();
	const size_t FieldCount = GetProjectInspectionFieldCount();

	BestFieldPtr = NULL;
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetProjectInspectionFieldPtr(i, false);
		if ( NULL == FieldPtr ) { continue; }
		if ( DistrictID != FieldPtr->GetFieldDistrictID() ) { continue; }
		if ( FieldPtr->CheckRegionInField(Region, CadMode, Inner) == false ) { continue; }

		if ( true == CadMode )
		{
			FieldCpX = FieldPtr->GetFieldCadPosX();
			FieldCpY = FieldPtr->GetFieldCadPosY();
		}
		else
		{
			FieldCpX = FieldPtr->GetFieldStagePosX();
			FieldCpY = FieldPtr->GetFieldStagePosY();			
		}
		DistX = RegionCpX-FieldCpX;
		DistY = RegionCpY-FieldCpY;
		Dist = sqrt((DistX*DistX)+(DistY*DistY));//找最近的
		if ( (NULL==BestFieldPtr) || (Dist<BestDist) ) 
		{
			BestDist = Dist;
			BestFieldPtr = FieldPtr; 
		}		
	}
	return BestFieldPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::AddFieldFrameToCurrentFrame(LPCTSTR OfflineFolder, CAOIField *FieldPtr, const TPOINT3D &Pos, const TSIZE2D &res, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TUNI_FRAME UniFrameList[], size_t Count, bool bUseInner)
{
	if ( NULL == FieldPtr ) { return true; }	
	double fnTime=0.0;
	LARGE_INTEGER fnStart, fnEnd;	
	CString    str;
	CString    strError[32];	
	bool       IsOK[32]={true}; 
	const int  FrameCount = (int)(FieldPtr->GetFieldFramePtrCount());
	const int  OpenMPCount = AOIDataCollect.GetSystemParameter().m_OpenMPCount_General;	
	int  OpenMPCountUsed = MIN(FrameCount, OpenMPCount);	
#ifndef OPEN_MP_USE	
	OpenMPCountUsed = 0;
#endif//OPEN_MP_USE
	JetAPI::SetFuncTimeStart(fnStart);
	if ( OpenMPCountUsed > 1 )
	{	
#ifdef OPEN_MP_USE	
	#pragma omp parallel for num_threads(OpenMPCountUsed) 			
		for ( int i=0; i<FrameCount; i++ )
		{	IsOK[i] = AddFieldFrameToCurrentFrameKernel(i, OfflineFolder, FieldPtr, Pos, res, ImageW, ImageH, UniFrameList[i], Count, bUseInner, strError[i]);	}
#endif//OPEN_MP_USE	
	}
	else
	{
		for ( int i=0; i<FrameCount; i++ )
		{	IsOK[i] = AddFieldFrameToCurrentFrameKernel(i, OfflineFolder, FieldPtr, Pos, res, ImageW, ImageH, UniFrameList[i], Count, bUseInner, strError[i]);	}
	}
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);//計算函式經過時間
	//str.Format(_T("AddFieldFrameToCurrentFrame Time=%.3f ms"), fnTime);
	//AOIDataCollect.SaveMovingTimeMsg(str);

	for ( int i=0; i<FrameCount; i++ )
	{
		if ( true == IsOK[i] ) { continue; }		
		m_ErrorString = strError[i];
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::AddFieldFrameToCurrentFrameKernel(int FrameIndex, LPCTSTR OfflineFolder, CAOIField *FieldPtr, const TPOINT3D &Pos, const TSIZE2D &res, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TUNI_FRAME &UniFrame, size_t Count, bool bUseInner, CString &strError)
{
	if ( NULL == FieldPtr ) { return true; }
	char vaName[256] = "";
	const char fnName[] = "CAOIProject::AddFieldFrameToCurrentFrameKernel";		
	int          s=0, t=0;
	int          BigIndex=0;
	int          SmallIndex=0;	
	TSIZE2D      sz;	
	POINT        FieldRoiCp;	
	TPOINT2D     FieldPos2D;
	TPOINT2D     ImagePos2D;	
	TPOINT2D     Pos2D(Pos.x, Pos.y);
	TPOINT2D     Res2D(res.cx, res.cy);
	TPOINT2D     FieldRoiCp2D;	
	TREGION4D    FovRgn;
	TREGION4D    FieldRgn;
	CString      Folder = OfflineFolder;	
	CString      FrameFileName;		
	CString      FrameFileFolder;
	CString      ImageMaskName;
	CString      ImageFileName;
	CString      DebugFileName;
	IMAGE_SIZE   NewBitCount=0;
	IMAGE_SIZE   NewImageStep=0;
	IMAGE_SIZE   NewImageSize=0;
	IMAGE_PTR    NewImaegPtr = NULL;
	MASK_PTR     NewMaskPtr = NULL;
	SPACE_PTR    NewSpacePtr = NULL;
	RECT         FieldRoi={0};
	RECT         ImageRoi={0};
	int          nImageChannels = 0;
	const int    ImageW2 = (int)(ImageW/2);
	const int    ImageH2 = (int)(ImageH/2);
	BOOL         bSave = FALSE;

	RECT         FrameRoi={0};

	bool         bFrameRawLoaded = false;
	bool         bFrameImageLoaded = false;	
	TSIZE2D      FrameRes;
	IMAGE_SIZE   FrameImageW=0;
	IMAGE_SIZE   FrameImageH=0;	
	IMAGE_SIZE   FrameBitCount=0;	
	IMAGE_SIZE   FrameImageStep=0;	
	IMAGE_SIZE   FrameImageSize=0;
	IMAGE_SIZE   RawBitCount=0;	
	IMAGE_SIZE   FrameRawStep=0;	
	IMAGE_PTR    FrameRawPtr = NULL;
	IMAGE_PTR    FrameImagePtr = NULL;
	MASK_PTR     FrameMaskPtr = NULL;
	SPACE_PTR    FrameSpacePtr = NULL;
	unsigned int FrameUniqueID = 0;	
	CString      DebugFolder;

	bool         IsOK=true;
	const int    nAlign = 4;
	bool         PtInFieldRgn = false;	
	TRECT4D      FieldRoiRect;
	RECT         FOVRect   = {0};
	RECT         FieldRect = {0};
	RECT         FrameRect = {0};
	const bool   SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool   SignY = AOIDataCollect.GetStageSignPositiveY();
	const bool   MemKeep = AOIDataCollect.GetMemoryKeepMode();	
	const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();	
	const bool   ShowFrameLine = false;	
	const int    FrameLineSize = 4;
	const unsigned char usRed = 16;
	const unsigned char usGrn = 16;
	const unsigned char usBlu = 32;		
	//const bool   MemKeep = true;		
	FRAME_TYPE   FrameType = FRAME_NULL;

	sz.cx = ImageW*res.cx;
	sz.cy = ImageH*res.cy;
	FovRgn.minX = Pos.x - (sz.cx/2.0);
	FovRgn.minY = Pos.y - (sz.cy/2.0);
	FovRgn.maxX = Pos.x + (sz.cx/2.0);
	FovRgn.maxY = Pos.y + (sz.cy/2.0);
	FOVRect.left   = (int)(FovRgn.minX);
	FOVRect.right  = (int)(FovRgn.maxX);
	FOVRect.top    = (int)(FovRgn.minY);
	FOVRect.bottom = (int)(FovRgn.maxY);

#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		DebugFileName.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FillCurrentInspectionFrame.TXT"));
		DebugFolder.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FillFrame"));
	}
#endif//_DEBUG

	const TPOINT3D FieldPos = FieldPtr->GetFieldStagePos();
	FieldPos2D.x = FieldPos.x;
	FieldPos2D.y = FieldPos.y;

	const double FieldSizeW_Real = FieldPtr->GetFieldSizeW_Real();
	const double FieldSizeH_Real = FieldPtr->GetFieldSizeH_Real();	
	const double FieldSizeW_Inner = FieldPtr->GetFieldSizeW_Inner();
	const double FieldSizeH_Inner = FieldPtr->GetFieldSizeH_Inner();	
	const double FieldSizeW = FieldSizeW_Real;
	const double FieldSizeH = FieldSizeH_Real;	
	double SysFieldSizeW = AOIDataCollect.GetFovSizeRealW();//先行使用相同的視野尺寸
	double SysFieldSizeH = AOIDataCollect.GetFovSizeRealH();//先行使用相同的視野尺寸

	//FieldSizeW = AOIDataCollect.GetFovSizeRealW();//先行使用相同的視野尺寸
	//FieldSizeH = AOIDataCollect.GetFovSizeRealH();//先行使用相同的視野尺寸		

	FieldRgn.minX = FieldPos.x - (FieldSizeW/2.0);
	FieldRgn.minY = FieldPos.y - (FieldSizeH/2.0);
	FieldRgn.maxX = FieldPos.x + (FieldSizeW/2.0);
	FieldRgn.maxY = FieldPos.y + (FieldSizeH/2.0);

	FieldRect.left   = (int)(FieldRgn.minX);
	FieldRect.right  = (int)(FieldRgn.maxX);
	FieldRect.top    = (int)(FieldRgn.minY);
	FieldRect.bottom = (int)(FieldRgn.maxY);

	if ( FieldRect.right  < FOVRect.left ) { return true; }
	if ( FieldRect.bottom < FOVRect.top  ) { return true; }
	if ( FieldRect.left   > FOVRect.right) { return true; }
	if ( FieldRect.top    > FOVRect.bottom  ) { return true; }

	PtInFieldRgn = JetAPI::CheckPtInRegion(Pos.x, Pos.y, FieldRgn);		
	//if ( false == PtInFieldRgn ) { continue; }
	if ( true == PtInFieldRgn )
	{	AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, Res2D, Pos2D, FieldPos2D, ImagePos2D);	}

	//計算該Field中心位置在填滿圖像的位置
	if ( false == SignX )
	{	FieldRoiCp2D.x = (ImageW2-((FieldPos2D.x-Pos.x)/res.cx));	}
	else
	{	FieldRoiCp2D.x   = (ImageW2+((FieldPos2D.x-Pos.x)/res.cx));	}
	if ( false == SignY )
	{	FieldRoiCp2D.y    = (ImageH2+((FieldPos2D.y-Pos.y)/res.cy));	}
	else
	{	FieldRoiCp2D.y    = (ImageH2-((FieldPos2D.y-Pos.y)/res.cy));	}			

	FieldRoiCp.x = (int)(FieldRoiCp2D.x+0.5);//避免太大的浮點數誤差, 使用4捨五入
	FieldRoiCp.y = (int)(FieldRoiCp2D.y+0.5);//避免太大的浮點數誤差, 使用4捨五入

	const int i = 0;
	const int j = FrameIndex;
	const int  FrameCount = (int)(FieldPtr->GetFieldFramePtrCount());
	if ( FrameIndex>=FrameCount || FrameIndex>=Count ) { return true; }	
	CAOIFrame *FramePtr = FieldPtr->GetFieldFramePtr(FrameIndex, false);
	if ( NULL == FramePtr ) { return true; }		
	TUNI_FRAME  *UniFramePtr = &(UniFrame);
	if ( NULL == UniFramePtr ) { return true; }

	bFrameRawLoaded = false;			
	FrameType = FramePtr->GetFrameType();
	FrameIndex = FramePtr->GetFrameIndex();		
	FrameFileName = FramePtr->GetFrameFileName();
	FrameFileFolder = FramePtr->GetFrameFileFolder();		
	UniFramePtr->FrameUniqueID = GetProjectFrameUniqueID(j, true);
	ImageFileName.Format(_T("%s\\%s"), FrameFileFolder, FrameFileName);
	switch ( FrameType )
	{	
	case FRAME_BAYER:
		nImageChannels =  3;		
		FrameBitCount  =  8;
		NewBitCount    = 24;
		break;
	case FRAME_COLOR:
		nImageChannels = 3;
		FrameBitCount = NewBitCount = 24;
		break;
	case FRAME_SPACE:
		nImageChannels = 1;
		FrameBitCount = NewBitCount = 8;
		break;
	default:
	case FRAME_GRAY:
		nImageChannels = 1;
		FrameBitCount = NewBitCount = 8;
		break;
	}
	if ( FRAME_SPACE == FrameType )
	{				
		ImageMaskName = AOIDataDefine.GetFrameMaskName(ImageFileName);				
		FramePtr->GetFrameImagePtr(FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameImagePtr);				
		FramePtr->GetFrameSpacePtr(FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameMaskPtr, FrameSpacePtr);
		if ( NULL==FrameImagePtr || NULL==FrameMaskPtr || NULL==FrameSpacePtr )
		{
			if ( NULL==FrameMaskPtr || NULL==FrameSpacePtr )//尚未載入
			{
				//以下函式會清除指標，因此要將指標先復歸，或者加上使用Frame來清除指標, 因為外部的指標是指向Frame內部的指標
				FramePtr->ClearFrameBuffer();
				FrameMaskPtr = NULL;
				FrameImagePtr = NULL;
				FrameSpacePtr = NULL;

				if ( ImageAPI.LoadSpaceGrayImage(ImageFileName, FrameImageW, FrameImageH, FrameImageStep, FrameSpacePtr, true) == false )
				{	return true;	}
				if ( ImageAPI.LoadMaskGrayImage(ImageMaskName, FrameImageW, FrameImageH, FrameImageStep, FrameMaskPtr, true) == false )
				{	
					JetMemory.free_func(FrameMaskPtr);
					JetMemory.free_func(FrameSpacePtr);
					return true;	
				}
				FrameBitCount = 8; 
				bFrameRawLoaded = true;
			}
			else
			{	
				bFrameRawLoaded = false; 
				if ( FramePtr->GetFrameImageValid() == false )
				{	return true;	}
			}

			JetAPI::SizeToRect(FrameImageW, FrameImageH, FrameRect);			
			FrameImageSize = ImageAPI.CalcBufferSize(FrameImageStep, FrameImageH);
			if ( JetMemory.alloc_func(FrameImageSize, FrameImagePtr, fnName, "FrameImagePtr") == false )
			{
				JetMemory.free_func(FrameMaskPtr);
				JetMemory.free_func(FrameSpacePtr);
				return true;
			}
			if ( ImageAPI.SpaceGrayImageConvertToGray3(FrameImageW, FrameImageH, FrameImageStep, FrameSpacePtr, FrameMaskPtr, FrameRect, FrameImageStep, FrameImagePtr, SpaceRatio, false) == false )
			{
				JetMemory.free_func(FrameMaskPtr);
				JetMemory.free_func(FrameSpacePtr);
				JetMemory.free_func(FrameImagePtr);
				return true;
			}					
			if ( false == bFrameRawLoaded )
			{	FramePtr->SetFrameImagePtr(FrameImagePtr);	}
		}
		else
		{	
			bFrameRawLoaded = false; 
			if ( FramePtr->GetFrameImageValid() == false )
			{	return true;	}
		}

		RawBitCount = FrameBitCount;	
		FrameRawStep = FrameImageStep;	
		FrameRawPtr = FrameImagePtr;
	}
	else if ( FRAME_BAYER == FrameType )
	{
		if( FramePtr->GetFrameImagePtr(FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameImagePtr) == false )
		{
			strError.Format(_T("Error, Get Frame Ptr Fault"));
			return false;
		}	
		if ( NULL == FrameImagePtr )
		{					
			if ( ImageAPI.LoadImage(ImageFileName, FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameImagePtr, 4, true) == false )
			{	return true;	}
			if ( true == ShowFrameLine ) 
			{	ImageAPI.DrawImageBoundary3(FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameImagePtr, FrameLineSize, usRed, usGrn, usBlu, FrameImagePtr);	}
			bFrameRawLoaded = true;
		}
		else
		{	
			bFrameRawLoaded = false;	
			if ( FramePtr->GetFrameImageValid() == false )
			{	return true;	}
		}

		RawBitCount = FrameBitCount;	
		FrameRawStep = FrameImageStep;	
		FrameRawPtr = FrameImagePtr;

		IMAGE_SIZE   DeBayerBit=0;
		IMAGE_SIZE   DeBayerStep=0;
		IMAGE_PTR    DeBayerPtr=NULL;
		BAYER_PATTERN_MODE BayerPattern = FramePtr->GetFrameBayerPattern();		
		if ( AOIDataCollect.ExecDebayerImage(fnName, FrameImageW, FrameImageH, FrameImageStep, FrameImagePtr, BayerPattern, DeBayerStep, DeBayerBit, DeBayerPtr) == false)		
		{
			if ( true == bFrameRawLoaded )
			{	JetMemory.free_func(FrameImagePtr);	}
			return true;
		}
		FrameImagePtr = DeBayerPtr;
		FrameImageStep = DeBayerStep;
		FrameBitCount = NewBitCount = DeBayerBit;
		bFrameImageLoaded = true;
	}
	else
	{				
		if( FramePtr->GetFrameImagePtr(FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameImagePtr) == false )
		{
			strError.Format(_T("Error, Get Frame Ptr Fault"));
			return false;
		}	
		if ( NULL == FrameImagePtr )
		{					
			if ( ImageAPI.LoadImage(ImageFileName, FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameImagePtr, 4, true) == false )
			{	return true;	}
			if ( true == ShowFrameLine ) 
			{	ImageAPI.DrawImageBoundary3(FrameImageW, FrameImageH, FrameImageStep, FrameBitCount, FrameImagePtr, FrameLineSize, usRed, usGrn, usBlu, FrameImagePtr);	}
			bFrameRawLoaded = true;
		}
		else
		{	
			bFrameRawLoaded = false;	
			if ( FramePtr->GetFrameImageValid() == false )
			{	return true;	}
		}

		RawBitCount = FrameBitCount;	
		FrameRawStep = FrameImageStep;	
		FrameRawPtr = FrameImagePtr;
	}
	
	const int nFrameImageW = (int)(FrameImageW);
	const int nFrameImageH = (int)(FrameImageH);
	const int nFrameImageW2 = (int)(nFrameImageW/2);
	const int nFrameImageH2 = (int)(nFrameImageH/2);
	
	int nUsedImageW  = nFrameImageW;
	int nUsedImageH  = nFrameImageH;
	int PaddingSize = AOIDataCollect.GetSystemParameter().m_StitchImagePaddingSize;
	if ( true == bUseInner )//使用部分範圍
	{		
		if ( PaddingSize < 0 ) { PaddingSize = 0; }
		nUsedImageW  = (int)(nFrameImageW*FieldSizeW_Inner/FieldSizeW_Real);
		nUsedImageH  = (int)(nFrameImageH*FieldSizeH_Inner/FieldSizeH_Real);
		
		//外擴一些
		nUsedImageW += PaddingSize;
		nUsedImageH += PaddingSize;
		//變成2的倍數
		if ( 0!=(nUsedImageW&0x01) ) { nUsedImageW += 1; } 
		if ( 0!=(nUsedImageH&0x01) ) { nUsedImageH += 1; } 
		//確認沒超過相機影像大小
		if ( nUsedImageW > nFrameImageW ) { nUsedImageW = nFrameImageW; }
		if ( nUsedImageH > nFrameImageH ) { nUsedImageH = nFrameImageH; }
	}
	const int nUsedImageW2 = nUsedImageW/2;
	const int nUsedImageH2 = nUsedImageH/2;
	
	FieldRoi.left   = FieldRoiCp.x - nUsedImageW2;
	FieldRoi.right  = FieldRoiCp.x + nUsedImageW2;
	FieldRoi.top    = FieldRoiCp.y - nUsedImageH2;
	FieldRoi.bottom = FieldRoiCp.y + nUsedImageH2;	

	ImageRoi = FieldRoi;//後面會變更, 因此需要重新取得

	FrameRoi.left   = nFrameImageW2 - nUsedImageW2;
	FrameRoi.top    = nFrameImageH2 - nUsedImageH2;
	FrameRoi.right  = nFrameImageW2 + nUsedImageW2;
	FrameRoi.bottom = nFrameImageH2 + nUsedImageH2;
	
	OFFLINE_IMAGE_SCOPE OfflineImageScope=GetProjectOfflineImageScope();//20221223
	if ( OFFLINE_IMAGE_PART == OfflineImageScope )
	{
		FieldRoi.right  = FieldRoi.left+ nUsedImageW;
		FieldRoi.bottom = FieldRoi.top + nUsedImageH;
		FrameRoi.right  = FrameRoi.left+ nUsedImageW;
		FrameRoi.bottom = FrameRoi.top + nUsedImageH;
	}

	//避免進位誤差, 所以重新設定
	ImageRoi.right  = (int)(ImageRoi.left+nUsedImageW);
	ImageRoi.bottom = (int)(ImageRoi.top +nUsedImageH);
	
	//合併圖域範圍
	if ( ImageRoi.left < 0 ) 
	{	
		FrameRoi.left = FrameRoi.left-ImageRoi.left;	
		ImageRoi.left = 0;
	}
	if ( ImageRoi.top < 0 ) 
	{	
		FrameRoi.top = FrameRoi.top-ImageRoi.top;	
		ImageRoi.top = 0;
	}
	if ( ImageRoi.right > ImageW )
	{	
		FrameRoi.right = FrameRoi.right-(ImageRoi.right-ImageW);	
		ImageRoi.right = ImageW;
	}
	if ( ImageRoi.bottom > ImageH )
	{	
		FrameRoi.bottom = FrameRoi.bottom-(ImageRoi.bottom-ImageH);	
		ImageRoi.bottom = ImageH;
	}

	//FOV 範圍錯誤
	if ( FrameRoi.left>=FrameImageW || FrameRoi.top>=FrameImageH || FrameRoi.right>FrameImageW || FrameRoi.bottom>FrameImageH )
	{
		if ( true == bFrameRawLoaded )
		{	
			JetMemory.free_func(FrameRawPtr);
			JetMemory.free_func(FrameMaskPtr);
			JetMemory.free_func(FrameSpacePtr);
		}
		if ( true == bFrameImageLoaded )
		{	JetMemory.free_func(FrameImagePtr); }
		return true;
	}
	if ( FrameRoi.left>=FrameRoi.right || FrameRoi.top>=FrameRoi.bottom )
	{
		if ( true == bFrameRawLoaded )
		{	
			JetMemory.free_func(FrameRawPtr);
			JetMemory.free_func(FrameMaskPtr);
			JetMemory.free_func(FrameSpacePtr);
		}
		if ( true == bFrameImageLoaded )
		{	JetMemory.free_func(FrameImagePtr); }
		return true;
	}
	//尺寸不相等
	if ( (FrameRoi.right-FrameRoi.left)!=(ImageRoi.right-ImageRoi.left) || (FrameRoi.bottom-FrameRoi.top)!=(ImageRoi.bottom-ImageRoi.top) )
	{
		if ( true == bFrameRawLoaded )
		{	
			JetMemory.free_func(FrameRawPtr);
			JetMemory.free_func(FrameMaskPtr);
			JetMemory.free_func(FrameSpacePtr);
		}
		if ( true == bFrameImageLoaded )
		{	JetMemory.free_func(FrameImagePtr); }
		return true;
	}
	NewImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, NewBitCount, 4);
	NewImageSize = ImageAPI.CalcBufferSize(NewImageStep, ImageH);
	if ( FRAME_SPACE == FrameType )
	{	
		if ( NULL == UniFramePtr->SpacePtr )
		{	
			::sprintf(vaName, "Field:%d_Space:%d", i+1, j+1);
			if ( JetMemory.alloc_func(NewImageSize, UniFramePtr->SpacePtr, fnName, vaName) == false )
			{
				if ( true == bFrameRawLoaded )
				{
					JetMemory.free_func(FrameRawPtr);
					JetMemory.free_func(FrameMaskPtr);					
					JetMemory.free_func(FrameSpacePtr);
				}
				if ( true == bFrameImageLoaded )
				{	JetMemory.free_func(FrameImagePtr); }
				strError = JetMemory.GetErrorString();
				return false;	
			}										
			::memset(UniFramePtr->SpacePtr, 0x00, sizeof(SPACE_DATA)*NewImageSize);
		}
		if ( NULL == UniFramePtr->MaskPtr )
		{	
			::sprintf(vaName, "Field:%d_Mask:%d", i+1, j+1);
			if ( JetMemory.alloc_func(NewImageSize, UniFramePtr->MaskPtr, fnName, vaName) == false )
			{
				if ( true == bFrameRawLoaded )
				{
					JetMemory.free_func(FrameRawPtr);
					JetMemory.free_func(FrameMaskPtr);					
					JetMemory.free_func(FrameSpacePtr);
				}
				if ( true == bFrameImageLoaded )
				{	JetMemory.free_func(FrameImagePtr); }
				JetMemory.free_func(UniFramePtr->SpacePtr);
				strError = JetMemory.GetErrorString();						
				return false;	
			}										
			::memset(UniFramePtr->MaskPtr, PHASE_MASK_NOISE_ONLY, sizeof(MASK_DATA)*NewImageSize);
		}
	}
	if ( NULL == UniFramePtr->ImagePtr )
	{	
		::sprintf(vaName, "Field:%d_Frame:%d", i+1, j+1);
		if ( JetMemory.alloc_func(NewImageSize, UniFramePtr->ImagePtr, fnName, vaName) == false )
		{
			if ( true == bFrameRawLoaded )
			{
				JetMemory.free_func(FrameRawPtr);
				JetMemory.free_func(FrameMaskPtr);				
				JetMemory.free_func(FrameSpacePtr);
			}
			if ( true == bFrameImageLoaded )
			{	JetMemory.free_func(FrameImagePtr); }
			JetMemory.free_func(UniFramePtr->MaskPtr);
			JetMemory.free_func(UniFramePtr->SpacePtr);
			strError = JetMemory.GetErrorString();
			return false;	
		}
		UniFramePtr->ImageW = ImageW;
		UniFramePtr->ImageH = ImageH;
		UniFramePtr->ImageStep = NewImageStep;
		UniFramePtr->BitCount = NewBitCount;					
		::memset(UniFramePtr->ImagePtr, 0x00, sizeof(IMAGE_DATA)*NewImageSize);				
	}
	else
	{
		if ( (NewImageStep!=UniFramePtr->ImageStep) || (NewBitCount!=UniFramePtr->BitCount) )
		{	
			if ( true == bFrameRawLoaded )
			{	
				JetMemory.free_func(FrameRawPtr);
				JetMemory.free_func(FrameMaskPtr);				
				JetMemory.free_func(FrameSpacePtr);
			}
			if ( true == bFrameImageLoaded )
			{	JetMemory.free_func(FrameImagePtr); }
			JetMemory.free_func(UniFramePtr->MaskPtr);
			JetMemory.free_func(UniFramePtr->ImagePtr);
			JetMemory.free_func(UniFramePtr->SpacePtr);
			strError.Format(_T("Error, Frame Image Size Exception (Filed:%d, Frame:%d)"), i+1, j+1);
			return false;	
		}
	}
	int          nImageLenW = 0;
	int          nImageLenH = 0;
	size_t       uYIndex=0;
	size_t       uCopyLen = 0;
	size_t       uBigIndex=0, uSmallIndex=0;

	NewImaegPtr = UniFramePtr->ImagePtr;
	nImageLenW = ImageRoi.right-ImageRoi.left;
	nImageLenH = ImageRoi.bottom-ImageRoi.top;
	nImageLenW = nImageLenW*nImageChannels;
	uCopyLen = (size_t)(sizeof(IMAGE_DATA)*nImageLenW);
	//const size_t BufferSize_Sml=FrameImageH*FrameImageStep;
	//const size_t BufferSize_Big=UniFramePtr->ImageStep*UniFramePtr->ImageH;	
	for ( s=0; s<nImageLenH; s++ )
	{
		//BigIndex = ((s+ImageRoi.top)*NewImageStep)+(ImageRoi.left*nImageChannels);
		//SmallIndex = ((s+FrameRoi.top)*FrameImageStep)+(FrameRoi.left*nImageChannels);
		//::memcpy(&(NewImaegPtr[BigIndex]), &(FrameImagePtr[SmallIndex]), uCopyLen);		

		//避免引數過大, 導致負號
		uYIndex = s;
		uBigIndex = ((uYIndex+ImageRoi.top)*NewImageStep);
		uBigIndex += (ImageRoi.left*nImageChannels);
		uSmallIndex = ((uYIndex+FrameRoi.top)*FrameImageStep);
		uSmallIndex += (FrameRoi.left*nImageChannels);
		::memcpy(&(NewImaegPtr[uBigIndex]), &(FrameImagePtr[uSmallIndex]), uCopyLen);		
	}

	if ( FRAME_SPACE == FrameType )
	{
		NewMaskPtr = UniFramePtr->MaskPtr;
		NewSpacePtr = UniFramePtr->SpacePtr;
		uCopyLen = (size_t)(sizeof(MASK_DATA)*nImageLenW);
		for ( s=0; s<nImageLenH; s++ )
		{
			//BigIndex = ((s+ImageRoi.top)*NewImageStep)+(ImageRoi.left*nImageChannels);
			//SmallIndex = ((s+FrameRoi.top)*FrameImageStep)+(FrameRoi.left*nImageChannels);
			//::memcpy(&(NewMaskPtr[BigIndex]), &(FrameMaskPtr[SmallIndex]), uCopyLen);

			//避免引數過大, 導致負號
			uYIndex = s;
			uBigIndex = ((uYIndex+ImageRoi.top)*NewImageStep);
			uBigIndex += (ImageRoi.left*nImageChannels);
			uSmallIndex = ((uYIndex+FrameRoi.top)*FrameImageStep);
			uSmallIndex += (FrameRoi.left*nImageChannels);
			::memcpy(&(NewMaskPtr[uBigIndex]), &(FrameMaskPtr[uSmallIndex]), uCopyLen);
		}
		uCopyLen = (size_t)(sizeof(SPACE_DATA)*nImageLenW);
		for ( s=0; s<nImageLenH; s++ )
		{
			//BigIndex = ((s+ImageRoi.top)*NewImageStep)+(ImageRoi.left*nImageChannels);
			//SmallIndex = ((s+FrameRoi.top)*FrameImageStep)+(FrameRoi.left*nImageChannels);
			//::memcpy(&(NewSpacePtr[BigIndex]), &(FrameSpacePtr[SmallIndex]), uCopyLen);

			//避免引數過大, 導致負號
			uYIndex = s;
			uBigIndex = ((uYIndex+ImageRoi.top)*NewImageStep);
			uBigIndex += (ImageRoi.left*nImageChannels);
			uSmallIndex = ((uYIndex+FrameRoi.top)*FrameImageStep);
			uSmallIndex += (FrameRoi.left*nImageChannels);
			::memcpy(&(NewSpacePtr[uBigIndex]), &(FrameSpacePtr[uSmallIndex]), uCopyLen);
		}
	}
	
	if ( false == MemKeep )
	{	
		JetMemory.free_func(FrameRawPtr);
		JetMemory.free_func(FrameMaskPtr);		
		JetMemory.free_func(FrameSpacePtr);		
	}
	else
	{
		if ( true == bFrameRawLoaded )
		{
			if ( FRAME_SPACE == FrameType )
			{	IsOK = FramePtr->SetFrameSpacePtr(FrameImageW, FrameImageH, FrameRawStep, RawBitCount, FrameRawPtr, FrameMaskPtr, FrameSpacePtr, false);	}
			else
			{	IsOK = FramePtr->SetFrameImagePtr(FrameImageW, FrameImageH, FrameRawStep, RawBitCount, FrameRawPtr, false);	}						
			FramePtr->SetFrameImageValid(IsOK);
			if ( false==IsOK )
			{				
				JetMemory.free_func(FrameRawPtr);
				JetMemory.free_func(FrameMaskPtr);				
				JetMemory.free_func(FrameSpacePtr);
				if ( true == bFrameImageLoaded )
				{	JetMemory.free_func(FrameImagePtr); }
				strError.Format(_T("Error, Set Frame Ptr Fault"));
				return false;
			}
		}
	}
	if ( true == bFrameImageLoaded )
	{	JetMemory.free_func(FrameImagePtr); }

	FrameImageW = 0;
	FrameImageH = 0;
	FrameImageStep = 0;
	FrameBitCount = 0;
	FrameImagePtr = NULL;
	FrameMaskPtr = NULL;
	FrameSpacePtr = NULL;			

	RawBitCount = 0;	
	FrameRawStep = 0;	
	FrameRawPtr = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::FillCurrentFrame(OFFLINE_FILE_MODE OfflineFileMode, const TPOINT3D &Pos, const TSIZE2D &res, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TUNI_FRAME UniFrameList[], size_t Count)
{
	if ( FillCurrentFrameFn(OfflineFileMode, Pos, res, ImageW, ImageH, UniFrameList, Count) == false )
	{
		SetProjectExceptionCode(AOI_EXCEPTION_PROJECT_FIELD_FILL);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::FillCurrentFrameFn(OFFLINE_FILE_MODE OfflineFileMode, const TPOINT3D &Pos, const TSIZE2D &res, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TUNI_FRAME UniFrameList[], size_t Count)
{
	if ( OFFLINE_FILE_PROGRAM == OfflineFileMode )
	{	return FillCurrentProgramFrame(Pos, res, ImageW, ImageH, UniFrameList, Count);	}
	else if ( OFFLINE_FILE_INSPECTION == OfflineFileMode )
	{	return FillCurrentInspectionFrame(Pos, res, ImageW, ImageH, UniFrameList, Count); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::FillCurrentProgramFrame(const TPOINT3D &Pos, const TSIZE2D &res, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TUNI_FRAME UniFrameList[], size_t Count)
{	
	size_t       i=0, j=0;	
	CString      DebugFolder;
	CString      DebugFileName;
	BOOL         bSave = FALSE;

	bool         IsOK=true;
	CAOIField   *FieldPtr = NULL;	
	DISTRICT_ID  DistrictID = GetProjectActDistrictID();
	CString      Folder = GetProjectProgramOfflineFolder();	
	const size_t FieldCount = GetProjectProgramFieldCount();		

#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		DebugFileName.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FillCurrentProgramFrame.TXT"));
		::DeleteFile(DebugFileName);

		DebugFolder.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FillFrame"));
		::CreateDirectory(DebugFolder, NULL);
		::Sleep(0);
		//JetAPI::ClearFolder(DebugFolder);
	}
#endif//_DEBUG
	bool   bUseInner = true;
	size_t szTempBuffer = 0;	
	const bool bResetField=false;	
	if ( false == bResetField ) 
	{
		for ( i=0; i<FieldCount; i++ )
		{
			FieldPtr = GetProjectProgramFieldPtr(i, false);
			if ( NULL == FieldPtr ) { continue; }
			if ( FieldPtr->GetFieldDistrictID() != DistrictID )
			{	continue; }
			IsOK = AddFieldFrameToCurrentFrame(Folder, FieldPtr, Pos, res, ImageW, ImageH, UniFrameList, Count, bUseInner);
			if ( false == IsOK )
			{	continue; }
		}	
	}
	else
	{
		for ( i=0; i<Count; i++ )
		{
			szTempBuffer = UniFrameList[i].ImageStep*UniFrameList[i].ImageH;
			if ( NULL != UniFrameList[i].ImagePtr )
			{	::memset(UniFrameList[i].ImagePtr, 0x00, sizeof(IMAGE_DATA)*szTempBuffer);	}

			if ( NULL != UniFrameList[i].MaskPtr )
			{	::memset(UniFrameList[i].MaskPtr, 0x00, sizeof(MASK_DATA)*szTempBuffer);	}

			if ( NULL != UniFrameList[i].SpacePtr )
			{	::memset(UniFrameList[i].SpacePtr, 0x00, sizeof(SPACE_DATA)*szTempBuffer);	}

			if ( NULL != UniFrameList[i].PhasePtr )
			{	::memset(UniFrameList[i].PhasePtr, 0x00, sizeof(PHASE_DATA)*szTempBuffer);	}
		}
	}

	CAOIComponent  *ComponentPtr = GetProjectActiveComponent();
	if ( NULL != ComponentPtr )
	{		
		FieldPtr = ComponentPtr->GetComponentFieldPtr();
		const size_t SubRngCount = ComponentPtr->GetComponentSubRgnCount();
		if ( NULL != FieldPtr  )		
		{	
			FIELD_LIST_MODE FieldListMode = FieldPtr->GetFieldListMode();
			if ( FIELD_LIST_PROGRAM == FieldListMode )
			{
				if ( 0 == SubRngCount ) 
				{	bUseInner = false; }
				else
				{	bUseInner = true; }
				AddFieldFrameToCurrentFrame(Folder, FieldPtr, Pos, res, ImageW, ImageH, UniFrameList, Count, bUseInner);	
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::FillCurrentInspectionFrame(const TPOINT3D &Pos, const TSIZE2D &res, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, TUNI_FRAME UniFrameList[], size_t Count)
{
	size_t       i=0, j=0;	
	CString      DebugFolder;
	CString      DebugFileName;
	BOOL         bSave = FALSE;
	OFFLINE_IMAGE_SCOPE OfflineImageScope=GetProjectOfflineImageScope();	

	bool         IsOK=true;
	CAOIField   *FieldPtr = NULL;	
	DISTRICT_ID  DistrictID = GetProjectActDistrictID();
	CString      Folder = GetProjectInspectionOfflineFolder();	
	const size_t FieldCount = GetProjectInspectionFieldUsedCount(OfflineImageScope);		

#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		DebugFileName.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FillCurrentInspectionFrame.TXT"));
		::DeleteFile(DebugFileName);

		DebugFolder.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FillFrame"));
		::CreateDirectory(DebugFolder, NULL);
		::Sleep(0);
		//JetAPI::ClearFolder(DebugFolder);
	}
#endif//_DEBUG
	bool   bUseInner = true;
	size_t szTempBuffer = 0;
	const bool bResetField=false;	
	if ( false == bResetField ) 
	{
		for ( i=0; i<FieldCount; i++ )
		{
			FieldPtr = GetProjectInspectionFieldUsedPtr(i, false, OfflineImageScope);
			if ( NULL == FieldPtr ) { continue; }
			if ( FieldPtr->GetFieldDistrictID() != DistrictID )
			{	continue; }
			IsOK = AddFieldFrameToCurrentFrame(Folder, FieldPtr, Pos, res, ImageW, ImageH, UniFrameList, Count, bUseInner);
			if ( false == IsOK )
			{	continue; }
		}	
	}
	else
	{
		for ( i=0; i<Count; i++ )
		{
			szTempBuffer = UniFrameList[i].ImageStep*UniFrameList[i].ImageH;
			if ( NULL != UniFrameList[i].ImagePtr )
			{	::memset(UniFrameList[i].ImagePtr, 0x00, sizeof(IMAGE_DATA)*szTempBuffer);	}

			if ( NULL != UniFrameList[i].MaskPtr )
			{	::memset(UniFrameList[i].MaskPtr, 0x00, sizeof(MASK_DATA)*szTempBuffer);	}

			if ( NULL != UniFrameList[i].SpacePtr )
			{	::memset(UniFrameList[i].SpacePtr, 0x00, sizeof(SPACE_DATA)*szTempBuffer);	}

			if ( NULL != UniFrameList[i].PhasePtr )
			{	::memset(UniFrameList[i].PhasePtr, 0x00, sizeof(PHASE_DATA)*szTempBuffer);	}
		}
	}
	
	CAOIComponent  *ComponentPtr = GetProjectActiveComponent();
	if ( NULL != ComponentPtr )
	{		
		FieldPtr = ComponentPtr->GetComponentFieldPtr();
		const size_t SubRngCount = ComponentPtr->GetComponentSubRgnCount();
		if ( NULL != FieldPtr )
		{			
			FIELD_LIST_MODE FieldListMode = FieldPtr->GetFieldListMode();			
			if ( FIELD_LIST_INSPECTION == FieldListMode )
			{
				if ( 0 == SubRngCount ) 
				{	bUseInner = false; }
				else
				{	bUseInner = true; }		
				AddFieldFrameToCurrentFrame(Folder, FieldPtr, Pos, res, ImageW, ImageH, UniFrameList, Count, bUseInner);	
			}			
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//