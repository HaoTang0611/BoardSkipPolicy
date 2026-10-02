// AOIProject_Component.cpp: implementation of the CAOIProject class.
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
inline size_t CAOIProject::GetProjectComponentCount_Inline() const//取得專案零件數量
{
	return CAOIProject::m_ProjectComponentPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::AddProjectComponentPtr_Inline(CAOIComponent *ComponentPtr)//增加專案零件
{
	CAOIProject::m_ProjectComponentPtrList.push_back(ComponentPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIComponent* CAOIProject::GetProjectComponentPtr_Inline(size_t index) const//取得專案零件指標	
{	
	return m_ProjectComponentPtrList[index];
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIProject::GetProjectDropOutPartCount_Inline() const//取得專案拋件數量
{
	return m_ProjectDropOutPartList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::AddProjectDropOutPartPtr_Inline(CAOIComponent *ComponentPtr)//增加專案拋件
{
	m_ProjectDropOutPartList.push_back(ComponentPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIComponent* CAOIProject::GetProjectDropOutPartPtr_Inline(size_t index) const//取得專案拋件指標	
{
	return m_ProjectDropOutPartList[index];
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIProject::GetProjectScratchPartCount_Inline() const//取得專案刮傷數量
{
	return m_ProjectScratchPartList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::AddProjectScratchPartPtr_Inline(CAOIComponent *ComponentPtr)//增加專案刮傷
{
	m_ProjectScratchPartList.push_back(ComponentPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIComponent* CAOIProject::GetProjectScratchPartPtr_Inline(size_t index) const//取得專案刮傷指標	
{
	return m_ProjectScratchPartList[index];
}
//-------------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectPartDimensionCount_Inline() const//取得專案尺寸數量
{
	return m_ProjectPartDimensionList.size();
}
//-------------------------------------------------------------------------------------//
void CAOIProject::AddProjectPartDimensionPtr_Inline(CAOIComponent *ComponentPtr)//增加專案尺寸
{
	m_ProjectPartDimensionList.push_back(ComponentPtr);
}
//-------------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectPartDimensionPtr_Inline(size_t index) const//取得專案尺寸指標	
{
	return m_ProjectPartDimensionList[index];
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIProject::GetProjectDefectComponentCount_Inline() const//取得專案瑕疵零件數量
{
	return m_ProjectDefectComponentPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::AddProjectDefectComponentPtr_Inline(CAOIComponent *ComponentPtr)//增加專案瑕疵零件
{
	m_ProjectDefectComponentPtrList.push_back(ComponentPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIComponent* CAOIProject::GetProjectDefectComponentPtr_Inline(size_t index) const//取得專案瑕疵零件指標	
{
	return m_ProjectDefectComponentPtrList[index];
}
//-------------------------------------------------------------------------------------//	
inline void CAOIProject::ClearProjectDefectComponentPtrList_Inline()//清除專案瑕疵列表
{
	m_ProjectDefectComponentPtrList.clear();
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIProject::GetProjectDefectComponentCount_LA_Inline() const//取得專案瑕疵零件數量
{
	return m_ProjectDefectComponentPtrList_LA.size();
}
//-------------------------------------------------------------------------------------//
inline CAOIComponent* CAOIProject::GetProjectDefectComponentPtr_LA_Inline(size_t index) const//取得專案瑕疵零件指標	
{
	return m_ProjectDefectComponentPtrList_LA[index];
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::ClearProjectDefectComponentPtrList_LA_Inline()//清除專案瑕疵列表
{
	m_ProjectDefectComponentPtrList_LA.clear();
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIProject::GetProjectDefectComponentCount_LB_Inline() const//取得專案瑕疵零件數量
{
	return m_ProjectDefectComponentPtrList_LB.size();
}
//-------------------------------------------------------------------------------------//
inline CAOIComponent* CAOIProject::GetProjectDefectComponentPtr_LB_Inline(size_t index) const//取得專案瑕疵零件指標	
{
	return m_ProjectDefectComponentPtrList_LB[index];
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::ClearProjectDefectComponentPtrList_LB_Inline()//清除專案瑕疵列表
{
	m_ProjectDefectComponentPtrList_LB.clear();
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::AddProjectDefectComponentPtr_All_Inline(CAOIComponent *ComponentPtr)//增加專案瑕疵零件全
{
	m_ProjectDefectComponentPtrList_All.push_back(ComponentPtr);
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::ClearProjectDefectComponentPtrList_All_Inline()//清除專案全瑕疵列表
{
	m_ProjectDefectComponentPtrList_All.clear();
}
//-------------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectComponentCount() const//取得專案零件數量
{
	return GetProjectComponentCount_Inline();
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectComponentCount(DISTRICT_ID DistrictID) const//取得專案零件數量
{
	size_t         i=0;
	size_t         Count=0;
	CAOIComponent *ComponentPtr=NULL;
	const size_t ComonentCount = GetProjectComponentCount_Inline();
	for ( i=0; i<ComonentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		Count ++;
	}
	return Count;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectComponentBypassCount() const//取得專案不檢測零件數量
{
	size_t         i=0;
	size_t         Count=0;
	CAOIComponent *ComponentPtr=NULL;
	const size_t ComonentCount = GetProjectComponentCount_Inline();
	for ( i=0; i<ComonentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentBypassed() == false ) { continue; }
		Count ++;
	}
	return Count;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectComponentBypassCount(DISTRICT_ID DistrictID) const//取得專案不檢測零件數量
{
	size_t         i=0;
	size_t         Count=0;
	CAOIComponent *ComponentPtr=NULL;
	const size_t ComonentCount = GetProjectComponentCount_Inline();
	for ( i=0; i<ComonentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentBypassed() == false ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }		
		Count ++;
	}
	return Count;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectComponentNotAgentCount() const//取得專案非代理零件數量	
{
	size_t         i=0;
	size_t         Count=0;
	CAOIComponent *ComponentPtr=NULL;
	const size_t ComonentCount = GetProjectComponentCount();
	for ( i=0; i<ComonentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }		
		Count ++;
	}
	return Count;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectComponentNotAgentCount(DISTRICT_ID DistrictID) const//取得專案非代理零件數量	
{
	size_t         i=0;
	size_t         Count=0;
	CAOIComponent *ComponentPtr=NULL;
	const size_t ComonentCount = GetProjectComponentCount();
	for ( i=0; i<ComonentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }		
		if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }		
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }		
		Count ++;
	}
	return Count;
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectComponentPtr(size_t index, bool check) const//取得專案零件指標
{
	if ( check )
	{
		const size_t count = GetProjectComponentCount_Inline();
		if ( index >= count )
		{	return NULL; }
	}
	return GetProjectComponentPtr_Inline(index);	
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::AddProjectComponentPtr(CAOIComponent *ComponentPtr, bool clone)//增加專案零件
{
	if ( NULL == ComponentPtr )
	{
		this->m_ErrorString.Format(_T("Error, AddProjectComponentPtr Fault"));
		return NULL;
	}

	CAOIComponent *NewComponentPtr = ComponentPtr;
	const unsigned int ComponentIndex = (unsigned int)(GetProjectComponentCount_Inline());
	if ( true == clone )
	{
		CAOIPanel        *PanelPtr = ComponentPtr->GetComponentPanelPtr();
		CAOIBoard        *BoardPtr = ComponentPtr->GetComponentBoardPtr();
		if ( (NULL==PanelPtr) || (NULL==BoardPtr) ) 
		{ 
			this->m_ErrorString.Format(_T("Error, AddProjectComponentPtr Fault"));
			return NULL; 
		}

		NewComponentPtr = ComponentPtr->CloneComponentObj();
		if ( NewComponentPtr == NULL ) { return NULL; }
		
		NewComponentPtr->SetComponentIndex_Project(ComponentIndex);	
		BoardPtr->AddBoardComponentPtr(NewComponentPtr);
		PanelPtr->AddPanelComponentPtr(NewComponentPtr);
	}	
	else
	{	NewComponentPtr->SetComponentIndex_Project(ComponentIndex);	 }

	int ComponentUniqueID = NewComponentPtr->GetComponentUniqueID();
	if ( ComponentUniqueID < 0 )
	{
		NewComponentPtr->SetComponentUniqueID(m_ProjectComponentMaxUniqueID);
		m_ProjectComponentMaxUniqueID ++;
	}
	else
	{
		if ( m_ProjectComponentMaxUniqueID <= ComponentUniqueID )
		{	m_ProjectComponentMaxUniqueID = ComponentUniqueID+1; }
	}

	NewComponentPtr->SetComponentProjectPtr(this);
	AddProjectComponentPtr_Inline(NewComponentPtr);		
	return NewComponentPtr;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::AddProjectComponentList(std::vector<CAOIComponent*> &List)//增加專案零件列表
{
	size_t i=0;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	AddProjectComponentPtr(List[i], false);	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectComponentBarcode()//確認專案零件條碼
{	
	UINT         UIResultID=0;	
	TPOINT3D     ComponentPos;
	bool         Confirmed=false;
	CAOIProject *ProjectPtr=this;	
	const size_t ComponentCount=GetProjectComponentCount();
	for ( size_t i=0; i<ComponentCount; i++ )
	{
		Confirmed=false;
		CAOIComponent *ComponentPtr=GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr = ComponentPtr->GetComponentResultPtr();
		if ( NULL == ComponentPtr ) { continue; }
		RESULT_ID PartResultID = ComponentPtr->GetComponentResultID_AOI();
		if ( RESULT_ID_NONE==PartResultID || RESULT_ID_OK==PartResultID || RESULT_ID_BYPASS==PartResultID || RESULT_ID_SKIP==PartResultID )//RESULT_ID_NG, RESULT_ID_EXCEPTION
		{	continue; }	
		CAOIModel *ModelPtr=ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }		
		const size_t WndCount=ModelPtr->GetModelWndCount();
		for ( size_t j=0; j<WndCount; j++ )
		{
			CAOIWnd *WndPtr=ModelPtr->GetModelWndPtr(j, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndEnabled() == false ) { continue; }
			ALG_TYPE AlgType = WndPtr->GetWndAlgType();
			if ( ALG_BARCODE_RECOGNIZE != AlgType )	{	continue; }
			RESULT_ID WndResultID = WndPtr->GetWndResultID();
			RESULT_ID LogicResultID = WndPtr->GetWndLogicResultID();
			if ( RESULT_ID_NONE==WndResultID || RESULT_ID_OK==WndResultID || RESULT_ID_BYPASS==WndResultID || RESULT_ID_SKIP==WndResultID )//RESULT_ID_NG, RESULT_ID_EXCEPTION
			{	continue; }	
			if ( RESULT_ID_NONE==LogicResultID || RESULT_ID_OK==LogicResultID || RESULT_ID_BYPASS==LogicResultID || RESULT_ID_SKIP==LogicResultID )//RESULT_ID_NG, RESULT_ID_EXCEPTION
			{	continue; }	
			ModelPtr->SetModelWndActived(WndPtr);
			//如果軌道自動沒有運轉, 才移動XYZ位置, 避免影響進出板
			if ( ProjectPtr->GetProjectConveyerPreRunRunning() == false )		
			{
				ComponentPos = ComponentPtr->GetComponentStagePos();
				MotionCtrlPtr->XYMoveTo(ComponentPos.x, ComponentPos.y); 
			}
			ComponentPtr->SetComponentConfirmUIResultID(0);
			//避免執行緒啟用沒有最上層, 因此改由CMainFrame來創建
			AOIDataCollect.LockUserInput();
			if ( AOIDataCollect.GetIsStopUserInput() == true )
			{ 
				AOIDataCollect.UnlockUserInput();
				return false; 
			}
			AOIDataCollect.SetIsWaitUserInput(true);
			AOIDataCollect.SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_EXEC_COMPONENT_BARCODE_CONFIRM_WND , (LPARAM)(ComponentPtr));
			UIResultID = ComponentPtr->GetComponentConfirmUIResultID();			
			if ( IDCANCEL == UIResultID )
			{
				m_ErrorString = AOIDataDefine.GetBarcodeText_NG();
				AOIDataCollect.SetIsStopUserInput(true);
				AOIDataCollect.SetIsWaitUserInput(false);
				AOIDataCollect.UnlockUserInput();
				return false;
			}
			Confirmed = true;
			AOIDataCollect.SetIsWaitUserInput(false);		
			AOIDataCollect.UnlockUserInput();

			const int WndLogicGroupID = WndPtr->GetWndLogicGroupID();
			const WND_DEFECT_ID WndDefectID = WndPtr->GetWndDefectID();
			const WND_LOGIC_TYPE WndLogicType=WndPtr->GetWndLogicType();
			if ( WND_LOGIC_DEFECT_ID == WndLogicType )
			{
				for ( size_t k=0; k<WndCount; k++ )
				{
					CAOIWnd *OtherWndPtr=ModelPtr->GetModelWndPtr(k, false);
					if ( NULL == OtherWndPtr ) { continue; }
					if ( WndPtr == OtherWndPtr ) { continue; }
					if ( WndDefectID != OtherWndPtr->GetWndDefectID() )	{ continue; }
					if ( WndLogicType != OtherWndPtr->GetWndLogicType() ) { continue; }
					if ( WndLogicGroupID != OtherWndPtr->GetWndLogicGroupID() ) { continue; }
					OtherWndPtr->SetWndLogicResultID(RESULT_ID_OK);
				}
			}		
		}
		if ( true == Confirmed )
		{
			ModelPtr->SetModelResultID(RESULT_ID_OK);
			ModelPtr->SetModelResultID_Alarm(RESULT_ID_OK);
			ModelPtr->BuildModelDefectWndGroupIDList();		
			ComponentPtr->UpdateComponentResultID();			
		}		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectComponentUseDrawRoughLine() const//確認專案零件使用粗糙線段
{
	const size_t ComponentCount=GetProjectComponentCount_Inline();
	if ( ComponentCount > 5000 ) { return true; }
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DestroyProjectComponentSelected()//摧毀選取到的專案零件
{
	size_t i = 0;
	unsigned int  index = 0;
	bool ComponentModelIsolated=false;
	CString        ModelFolder;
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	std::vector<CAOIComponent*> ProjectComponentPtrList = this->m_ProjectComponentPtrList;
	
	index = 0;
	this->m_ProjectComponentPtrList.clear();
	const size_t ComponentCount = ProjectComponentPtrList.size();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectComponentPtrList[i];
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == true )
		{
			ComponentModelIsolated = ComponentPtr->GetComponentModelIsolated();
			if ( true == ComponentModelIsolated )
			{
				ModelPtr = ComponentPtr->GetComponentModelPtr();
				ModelFolder = ModelPtr->GetModelFolderComponent();
				JetAPI::RemoveFolder(ModelFolder);				
			}
			AOIObjManager.DestroyComponentObj(ProjectComponentPtrList[i]);
			ComponentPtr = NULL;
			continue;
		}

		ComponentPtr->SetComponentIndex_Project(index);
		AddProjectComponentPtr_Inline(ComponentPtr);
		index ++;
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectComponentType(COMPONENT_TYPE Type)//摧毀特定的專案零件樣式
{	
	CAOIComponent *ActComponentPtr = GetProjectActiveComponent();
	if ( NULL != ActComponentPtr )
	{
		if ( ActComponentPtr->GetComponentType() == Type ) 
		{	
			ActComponentPtr = NULL;
			ResetProjectActiveIndex(); 
		}
	}
	SelectProjectAllComponents(false);
	SelectProjectComponentsByComponentType(Type);
	DeleteProjectComponentSelected();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ClearProjectAllComponents()//刪除專案零件
{
	size_t i = 0;	
	CAOIComponent *ComponentPtr = NULL;	
	const size_t ComponentCount = GetProjectComponentCount_Inline();
	m_ProjectComponentSelectedList.clear();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		AOIObjManager.DestroyComponentObj(m_ProjectComponentPtrList[i]);
		ComponentPtr = NULL;		
	}		
	this->m_ProjectComponentPtrList.clear();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::LayoutProjectComponentList()//重整專案的零件列表
{
	size_t       i = 0;
	unsigned int index = 0;
	CAOIComponent *ComponentPtr = NULL;
	std::vector<CAOIComponent*> ProjectComponentPtrList = this->m_ProjectComponentPtrList;
	
	index = 0;
	this->m_ProjectComponentPtrList.clear();
	const size_t ComponentCount = ProjectComponentPtrList.size();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectComponentPtrList[i];
		if ( NULL == ComponentPtr ) { continue; }

		ComponentPtr->SetComponentIndex_Project(index);
		AddProjectComponentPtr_Inline(ComponentPtr);
		index ++;
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectAllComponents(bool value)//選取專案的所有零件
{
	size_t i = 0;	
	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = GetProjectComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentSelected(value);		
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectComponentSelected()//刪除選取到的專案零件
{
	size_t     i=0, j=0, k=0;
	CAOIPanel *PanelPtr = NULL;
	CAOIBoard *BoardPtr = NULL;	
	CAOIComponent *ComponentPtr = NULL;
	const size_t PanelCount = GetProjectPanelCount();
	const size_t BoardCount = GetProjectBoardCount();		
	const size_t ComponentCount = GetProjectComponentCount_Inline();

	ClearProjectAllTempObjects();//刪除所有暫時用的物件

	SelectProjectAllFds(false);
	SelectProjectAllMarks(false);
	SelectProjectAllBarcodes(false);
	SelectProjectAllPanels(false);
	SelectProjectAllBoards(false);
	SelectProjectComponentsMaster();//如果本尊選到, 代理人也要選到	

	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		
		const size_t NBoards = PanelPtr->GetPanelBoardCount();
		for ( j=0; j<NBoards; j++ )
		{
			BoardPtr = PanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }

			const size_t NComponents = BoardPtr->GetBoardComponentCount();
			for ( k=0; k<NComponents; k++ )
			{
				ComponentPtr = BoardPtr->GetBoardComponentPtr(k, false);
				if ( ComponentPtr == NULL ) { continue; }
				if ( ComponentPtr->GetComponentSelected() == true )
				{	break; }
			}
			if ( k == NComponents ) { continue; }
			BoardPtr->RemoveBoardComponentSelected();
			BoardPtr->LayoutBoardRegion();
		}

		const size_t NComponents = PanelPtr->GetPanelComponentCount();
		for ( k=0; k<NComponents; k++ )
		{
			ComponentPtr = PanelPtr->GetPanelComponentPtr(k, false);
			if ( ComponentPtr == NULL ) { continue; }
			if ( ComponentPtr->GetComponentSelected() == true )
			{	break; }
		}
		if ( k == NComponents ) { continue; }
		PanelPtr->RemovePanelComponentSelected();
		PanelPtr->LayoutPanelRegion();
	}	

	RemoveProjectComponentAgnetSelected();
	RemoveProjectPartGroupComponentSelected();
	RemoveProjectDefectComponentSelected();
	RemoveProjectDefectComponentSelected_LA();
	RemoveProjectDefectComponentSelected_LB();
	RemoveProjectDefectComponentSelected_All();
	DestroyProjectObjectSelected();
	LayoutProjectAllObjectsIndex();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectComponentAgentInvalid(const std::vector<CAOIComponent*> &AgentList)//刪除無效的代理零件
{
	//移除沒有連結到的Agent	
	CAOIComponent *AgentPtr=NULL;
	std::vector<CAOIComponent*> DeleteAgentList;
	const size_t AgentCount=AgentList.size();
	for ( size_t i=0; i<AgentCount; i++ )
	{
		AgentPtr = AgentList[i];
		if ( NULL == AgentPtr ) { continue; }
		if ( AgentPtr->CheckComponentIsAgent() == true ) { continue; }
		DeleteAgentList.push_back(AgentPtr);
	}
	const size_t DeleteAgentCount=DeleteAgentList.size();
	if ( 0 == DeleteAgentCount ) { return true; }
	
	SelectProjectAllComponents(false);
	for ( size_t i=0; i<DeleteAgentCount; i++ )
	{
		AgentPtr = DeleteAgentList[i];
		if ( NULL == AgentPtr ) { continue; }
		AgentPtr->SetComponentSelected(true);
	}
	if ( DeleteProjectComponentSelected() == false )
	{	return false; }
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ReLinkProjectComponentMasterAgent(CAOIComponent *Master, const std::vector<CAOIComponent*> &ComponentMapList) //重新連結專案零件本尊與代理人
{
	if ( NULL == Master ) { return false; }
	const size_t ComponentCount = ComponentMapList.size();
	const size_t MasterIndex = Master->GetComponentIndex_Project();
	if ( MasterIndex >= ComponentCount )
	{	return false; }
	CAOIComponent *NewMaster = ComponentMapList[MasterIndex];
	if ( NewMaster == Master ) { return true; }

	NewMaster->ClearComponentAgentList();
	const size_t AgentCount=Master->GetComponentAgentCount();
	for ( size_t j=0; j<AgentCount; j++ )
	{
		CAOIComponent *OldAgent=Master->GetComponentAgentPtr(j, false);
		if ( NULL == OldAgent ) { continue; }
		const size_t AgentIndex=OldAgent->GetComponentIndex_Project();
		if ( AgentIndex >= ComponentCount ) { continue; }
		CAOIComponent *NewAgent = ComponentMapList[AgentIndex];
		if ( NewAgent == OldAgent ) { continue; }
		NewMaster->AddComponentAgentPtr(NewAgent);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ReLinkProjectComponentMasterAgentList(const std::vector<CAOIComponent*> &MasterList, const std::vector<CAOIComponent*> &ComponentMapList) //重新連結專案零件本尊與代理人
{	
	const size_t MasterCount=MasterList.size();	
	for ( size_t i=0; i<MasterCount; i++ )
	{	ReLinkProjectComponentMasterAgent(MasterList[i], ComponentMapList);	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::BypassProjectComponentSelected(bool bBypassed)//不檢測選取到的專案零件
{
	size_t i = 0;	
	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = GetProjectComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		ComponentPtr->SetComponentBypassed(bBypassed);
	}
	return true;	
}
//---------------------------------------------------------------------------------//
bool CAOIProject::BackupProjectComponentSelected()//備份選取到的零件
{	
	size_t         i=0;
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ComponentCount = GetProjectComponentCount_Inline();

	m_ProjectComponentSelectedList.clear();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		m_ProjectComponentSelectedList.push_back(ComponentPtr);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RestoreProjectComponentSelected()//還原選取到的零件
{
	size_t         i=0;
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ComponentCount = m_ProjectComponentSelectedList.size();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = m_ProjectComponentSelectedList[i];
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentSelected(true);
	}
	m_ProjectComponentSelectedList.clear();	
	return true;
	
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectComponentsByColRowIndex(int ColMode, int RowMode)//選取零件
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }

	size_t NSelected = 0;
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount();
	for ( size_t i=0; i<ComponentCount; i++ )
	{
		CAOIComponent *ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		const int XIndex = ComponentPtr->GetComponentColIndex();
		const int YIndex = ComponentPtr->GetComponentRowIndex();
		if ( -1 == XIndex || -1 == YIndex ) { continue; }

		int Index = 0;
		bool bSelected=false;		
		if ( SEL_COL_ROW_IDX_NONE != ColMode )
		{
			Index = XIndex+1;
			bool bIsEven = JetAPI::IsEventInt(Index);
			if ( SEL_COL_ROW_IDX_EVEN == ColMode )//偶數
			{	bSelected = bIsEven;	}
			else//SEL_COL_ROW_IDX_ODD
			{	//奇數	
				if ( bIsEven == false )
				{	bSelected = true; }				
			}
			if ( bSelected )
			{
				NSelected ++;
				ComponentPtr->SetComponentSelected(true);
				continue;
			}
		}

		if ( SEL_COL_ROW_IDX_NONE != RowMode )
		{
			Index = YIndex+1;
			bool bIsEven = JetAPI::IsEventInt(Index);
			if ( SEL_COL_ROW_IDX_EVEN == RowMode )//偶數
			{	bSelected = bIsEven;	}
			else//SEL_COL_ROW_IDX_ODD
			{	//奇數	
				if ( bIsEven == false )
				{	bSelected = true; }				
			}
			if ( bSelected )
			{
				NSelected ++;
				ComponentPtr->SetComponentSelected(true);
				continue;
			}
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectComponentsBySelectedCadCp(const std::vector<CAOIComponent*> &SelComponentList, bool bXPos, bool bYPos)//選取零件
{
	CAOIProject *ProjectPtr = this;	
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }	
	const size_t SelComponentCount = SelComponentList.size();
	if ( 0 == SelComponentCount ) { return true; }
	
	for ( size_t i=0; i<SelComponentCount; i++ )
	{
		CAOIComponent *SelComponentPtr = SelComponentList[i];
		if ( NULL == SelComponentPtr ) { continue; }
		CAOIBoard *SelBoardPtr = SelComponentPtr->GetComponentBoardPtr();
		if ( NULL == SelBoardPtr ) { continue; }
		const double SelComponentPosX=SelComponentPtr->GetComponentCadPosX();
		const double SelComponentPosY=SelComponentPtr->GetComponentCadPosY();
		const size_t BoardComponentCount = SelBoardPtr->GetBoardComponentCount();
		for ( size_t j=0; j<BoardComponentCount; j++ )
		{
			CAOIComponent *ComponentPtr = SelBoardPtr->GetBoardComponentPtr(j, false);
			if ( NULL == ComponentPtr ) { continue; }
			if ( ComponentPtr == SelComponentPtr ) { continue; }
			if ( ComponentPtr->GetComponentSelected() == true ) { continue; }

			TREGION4D CadRgn;
			bool bSelXPos=false;
			bool bSelYPos=false;
			ComponentPtr->GetComponentBodyCadRegion(CadRgn);
			if ( true == bXPos )
			{
				bSelXPos = true;
				if ( CadRgn.minX > SelComponentPosX ) { bSelXPos = false; }
				else if ( CadRgn.maxX < SelComponentPosX ) { bSelXPos = false; }
			}
			if ( true == bYPos )
			{
				bSelYPos = true;
				if ( CadRgn.minY > SelComponentPosY ) { bSelYPos = false; }
				else if ( CadRgn.maxY < SelComponentPosY ) { bSelYPos = false; }
			}
			if ( true==bSelXPos || true==bSelYPos )
			{	ComponentPtr->SetComponentSelected(true);	}
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectComponentsByStage(const TREGION4D &Rgn, bool ResultMode, std::vector<CAOIComponent*> &ComponentList)//選取零件
{
	size_t         i = 0;	
	CAOIComponent *ComponentPtr = NULL;	
	const bool     PickSelMode = JetAPI::CheckPickSelectMode(Rgn, 25);
	const size_t   ComponentCount = GetProjectComponentCount_Inline();

	ComponentList.clear();
	if ( true == PickSelMode )
	{
		TPOINT2D Pt;
		Pt.x = Rgn.GetCpX();
		Pt.y = Rgn.GetCpY();
		for ( i=0; i<ComponentCount; i++ )
		{
			ComponentPtr = GetProjectComponentPtr_Inline(i);
			if ( NULL == ComponentPtr ) { continue; }		
			if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
			if ( ComponentPtr->GetComponentVisibled() == false ) { continue; }
			if ( ComponentPtr->CheckComponentTypeBeClicked() == false ) { continue; }
			if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }
			ComponentPtr = ComponentPtr->GetComponentResultPtr();
			if ( ComponentPtr->CheckComponentBePickByStage(Pt, ResultMode) == false ) { continue; }
			ComponentList.push_back(ComponentPtr);
		}
	}
	else
	{
		const bool     bEntireSel = true;
		for ( i=0; i<ComponentCount; i++ )
		{
			ComponentPtr = GetProjectComponentPtr_Inline(i);
			if ( NULL == ComponentPtr ) { continue; }
			if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
			if ( ComponentPtr->GetComponentVisibled() == false ) { continue; }
			if ( ComponentPtr->CheckComponentTypeBeClicked() == false ) { continue; }
			if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }
			ComponentPtr = ComponentPtr->GetComponentResultPtr();
			if ( ComponentPtr->CheckComponentInRegionByStage(Rgn, ResultMode, bEntireSel) == false ) { continue; }
			ComponentList.push_back(ComponentPtr);
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectComponentByMultiBoardCtrlMode(std::vector<CAOIComponent*> &SelComponentList, MULTI_BOARD_CTRL_MODE CtrlMode)//選取零件
{
	if ( MULTI_BOARD_CTRL_DISABLE == CtrlMode ) { return true; }

	size_t         i=0, j=0;
	unsigned int   PanelIndex=0;
	unsigned int   PanelIndexSel=0;
	CString        ComponentName;
	CAOIComponent *ComponentPtr = NULL;		
	CAOIComponent *ComponentPtrSel = NULL;		
	const size_t   SelComponentCount = SelComponentList.size();
	const size_t   ComponentCount = GetProjectComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		ComponentName = ComponentPtr->GetComponentName();
		PanelIndex = ComponentPtr->GetComponentPanelIndex_Project();
		for ( j=0; j<SelComponentCount; j++ )
		{
			ComponentPtrSel = SelComponentList[j];
			if ( NULL == ComponentPtrSel ) { continue; }
			if ( ComponentPtr == ComponentPtrSel ) { continue; }//同顆零件
			PanelIndexSel = ComponentPtrSel->GetComponentPanelIndex_Project();
			//if ( MULTI_BOARD_CTRL_BOARD == CtrlMode )
			//{
			//	if ( PanelIndexSel != PanelIndex ) { continue; }//不同整板
			//}
			if ( ComponentName.CompareNoCase(ComponentPtrSel->GetComponentName()) != 0 ) { continue; }
			ComponentPtr->SetComponentSelected(true);
			break;
		}		
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectComponentsByComponentType(COMPONENT_TYPE ComponentType)
{
	size_t         i = 0;		
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentType() != ComponentType ) { continue; }		
		ComponentPtr->SetComponentSelected(true);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::CheckProjectComponentCountByComponentType(COMPONENT_TYPE ComponentType)
{
	size_t         i = 0;		
	size_t         Count=0;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		if ( ComponentPtr->GetComponentType() != ComponentType ) { continue; }		
		Count ++;
	}	
	return Count;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectComponentsByModelName(LPCTSTR ModelName, bool ModelUnSet)
{
	size_t         i = 0;		
	CString        ModelNameC, ModelNameM;
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		

	ModelNameM = ModelName;
	ModelNameM.MakeUpper();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( true == ModelUnSet )
		{	
			if ( ComponentPtr->CheckComponentModelEnabled() == true )
			{	continue; }			
		}
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		ModelNameC = ModelPtr->GetModelName();
		ModelNameC.MakeUpper();
		if ( ModelNameC != ModelNameM ) { continue; }
		ComponentPtr->SetComponentSelected(true);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectComponentsByPartNumber(LPCTSTR PartNumber, bool ModelUnSet)
{
	size_t         i = 0;	
	MODEL_TYPE     ModelType;
	CString        PartNumberC, PartNumberM;	
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		

	PartNumberM = PartNumber;
	PartNumberM.MakeUpper();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }		
		PartNumberC = ComponentPtr->GetComponentPartNumber();
		PartNumberC.MakeUpper();
		if ( PartNumberC != PartNumberM ) { continue; }

		if ( true == ModelUnSet )
		{
			ModelPtr = ComponentPtr->GetComponentModelPtr();
			ModelType = ModelPtr->GetModelType();
			if ( MODEL_TYPE_NULL != ModelType )
			{	continue; }
		}		
		ComponentPtr->SetComponentSelected(true);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectComponentsByModelGroup(LPCTSTR ModelGroupName, bool ModelUnSet)
{
	size_t         i = 0;	
	MODEL_TYPE     ModelType;
	CString        ModelGroupNameC, ModelGroupNameM;	
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		

	ModelGroupNameM = ModelGroupName;
	ModelGroupNameM.MakeUpper();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }

		ModelGroupNameC = ModelPtr->GetModelGroupName();
		ModelGroupNameC.MakeUpper();
		if ( ModelGroupNameC != ModelGroupNameM ) { continue; }

		if ( true == ModelUnSet )
		{	
			ModelType = ModelPtr->GetModelType();
			if ( MODEL_TYPE_NULL != ModelType )
			{	continue; }
		}
		ComponentPtr->SetComponentSelected(true);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectComponentsForModelBkImage()//選取專案零件-為了模組底圖
{
	size_t         i=0, j=0;	
	size_t         ModelComponentCount=0;
	double         ComponentAngle=0;
	bool           IsExceptionAngle=false;
	MODEL_TYPE     ModelType;
	CString        ModelFolder;
	CString        LibraryFolder;
	CString        ModelNameC, ModelNameM;	
	CAOIModel     *ModelPtr = NULL;
	CAOIModel     *ModelPtr_C = NULL;
	CAOIComponent *ComponentPtr = NULL;
	std::vector<CAOIComponent*> ModelComponentList;
	const size_t   ModelCount = GetProjectModelCount();
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		
	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentSelected(false);
		ComponentPtr->SetComponentTempIndex(-1);
	}

	LibraryFolder = GetProjectLibraryFolder();
	for ( j=0; j<ModelCount; j++ )
	{
		ModelPtr = GetProjectModelPtr(j, false);
		if ( NULL == ModelPtr ) { continue; }
		ModelType = ModelPtr->GetModelType();
		//if ( MODEL_TYPE_NULL == ModelType ) { continue; }
		if ( CAOIModel::CheckModelTypeEnabled(ModelType) == false )
		{	continue;	}
		ModelNameM = ModelPtr->GetModelName();		
		ModelNameM.MakeUpper();

		ModelComponentList.clear();
		ModelFolder.Format(_T("%s\\%s"), LibraryFolder, ModelNameM);
		for ( i=0; i<ComponentCount; i++ )
		{
			ComponentPtr = GetProjectComponentPtr_Inline(i);
			if ( NULL == ComponentPtr ) { continue; }
			if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
			if ( ComponentPtr->GetComponentBypassed() == true ) { continue; }
			if ( ComponentPtr->GetComponentTempIndex() != -1 ) { continue; }
			if ( ComponentPtr->GetComponentModelIsolated() == true ) { continue; }
			ModelPtr_C = ComponentPtr->GetComponentModelPtr();
			if ( NULL == ModelPtr_C ) { continue; }

			ModelNameC = ComponentPtr->GetComponentModelName();
			ModelNameC.MakeUpper();
			if ( ModelNameC != ModelNameM ) { continue; }			
			ComponentPtr->SetComponentTempIndex(j);
			ModelPtr_C->SetModelFolderModel(ModelFolder);
			ModelComponentList.push_back(ComponentPtr);
		}

		ModelComponentCount = ModelComponentList.size();
		if ( 0 == ModelComponentCount ) { continue; }		
		
		JetAPI::CreateFolder(ModelFolder);		
		ModelPtr->SetModelBKImageNeedToGrab(true);
		for ( i=0; i<ModelComponentCount; i++ )
		{
			ComponentPtr = ModelComponentList[i];
			if ( NULL == ComponentPtr ) { continue; }
			ComponentAngle = ComponentPtr->GetComponentAngle();
			IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
			if ( true == IsExceptionAngle ) { continue; }			
			ComponentPtr->SetComponentSelected(true);
			break;
		}		
		if ( i == ModelComponentCount ) { continue; }
		ModelPtr->SetModelNeedSaveFiles(true);//該模組要存圖		
		ModelPtr->SetupkModelModifiedDateTime();
		ModelPtr->SetModelBKImageNeedToGrab(false);
		continue;
		ComponentPtr = ModelComponentList[0];
		if ( NULL != ComponentPtr )
		{	ComponentPtr->SetComponentSelected(true); }		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RemoveProjectComponentAgnetSelected()//移除專案零件選取到的代理人
{
	size_t         i = 0;
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }		
		if ( ComponentPtr->CheckComponentIsMaster() == false ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == true ) { continue; }
		ComponentPtr->RemoveComponentAgentSelected();
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectComponentsMaster()//選取專案零件-為了本尊與代理人
{
	size_t         i = 0;
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }		
		if ( ComponentPtr->CheckComponentIsMaster() == false ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		ComponentPtr->SetComponentAllAgentSelected(true);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectComponentsMasterAgent()//選取專案零件-為了本尊與代理人
{
	size_t         i = 0;
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }		
		if ( ComponentPtr->CheckComponentIsMaster() == false ) { continue; }
		if ( ComponentPtr->GetComponentSelected() || ComponentPtr->CheckComponentAgentSelected() )
		{
			ComponentPtr->SetComponentSelected(true);
			ComponentPtr->SetComponentAllAgentSelected(true);
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectComponentPtrByHighest()//取得專案零件指標-依據本體最高
{	
	double MaxHeight=INVALID_DOUBLE;
	CAOIComponent *MaxComponentPtr=NULL;	
	const size_t ComponentCount=GetProjectComponentCount();
	for ( size_t i=0; i<ComponentCount; i++ )
	{
		CAOIComponent *ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		double ComponentHeight=ComponentPtr->GetComponentResultHeight();
		if ( INVALID_DOUBLE==MaxHeight || MaxHeight<ComponentHeight )
		{
			MaxHeight = ComponentHeight;
			MaxComponentPtr = ComponentPtr;
		}
	}
	return MaxComponentPtr; 
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectComponentPtrBySelected()//取得專案零件指標-依據選取到
{
	size_t         i = 0;		
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		return ComponentPtr;
	}	
	return NULL;
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectComponentPtrByModelName(LPCTSTR ModelName)//取得專案零件指標-依據料號
{
	size_t         i = 0;	
	CString        ModelNameC, ModelNameM;
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		

	ModelNameM = ModelName;
	ModelNameM.MakeUpper();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		ModelNameC = ModelPtr->GetModelName();
		ModelNameC.MakeUpper();
		if ( ModelNameC != ModelNameM ) { continue; }
		return ComponentPtr;
	}	
	return NULL;
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectComponentPtrByPartNumber(LPCTSTR PartNumber)//取得專案零件指標-依據料號
{
	size_t         i = 0;	
	CString        PartNumberC, PartNumberM;	
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		

	PartNumberM = PartNumber;
	PartNumberM.MakeUpper();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }		
		PartNumberC = ComponentPtr->GetComponentPartNumber();
		PartNumberC.MakeUpper();
		if ( PartNumberC != PartNumberM ) { continue; }
		return ComponentPtr;
	}	
	return NULL;
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectComponentPtrByComponentUUID(const UUID &uuid)//取得專案零件指標-依據萬用字碼
{
	size_t         i = 0;	
	UUID           ComponentUUID;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		
	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }		
		ComponentUUID = ComponentPtr->GetObjUuid();
		if ( ComponentUUID != uuid ) { continue; }		
		return ComponentPtr;
	}	
	return NULL;
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectComponentPtrByComponentName(LPCTSTR ComponentName)//取得專案零件指標-依據名稱
{
	size_t         i = 0;	
	CString        ComponentNameC, ComponentNameM;	
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		

	ComponentNameM = ComponentName;
	ComponentNameM.MakeUpper();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }		
		ComponentNameC = ComponentPtr->GetComponentName();
		ComponentNameC.MakeUpper();
		if ( ComponentNameC != ComponentNameM ) { continue; }
		return ComponentPtr;
	}	
	return NULL;
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectComponentPtrByComponentFullName(LPCTSTR ComponentName, bool bModelIsolated)//取得專案零件指標-依據全名
{
	size_t         i = 0;	
	CString        ComponentNameC, ComponentNameM;	
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		

	ComponentNameM = ComponentName;
	ComponentNameM.MakeUpper();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }		
		if ( true == bModelIsolated )
		{
			if ( ComponentPtr->GetComponentModelIsolated() == false )
			{	continue; }
		}
		ComponentNameC = ComponentPtr->GetComponentFullName();
		ComponentNameC.MakeUpper();
		if ( ComponentNameC != ComponentNameM ) { continue; }
		return ComponentPtr;
	}	
	return NULL;
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectComponentPtrByComponentFullName(int PanelIndex, int BoardIndex, LPCSTR ComponentName)//取得專案零件指標-依據全名
{
	size_t         i = 0;		
	CAOIComponent *ComponentPtr = NULL;	
	CString        ComponentNameM(ComponentName);
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		

	ComponentNameM.MakeUpper();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentPanelIndex_Project() != PanelIndex ) { continue; }
		if ( ComponentPtr->GetComponentBoardIndex_Panel() != BoardIndex ) { continue; }		
		if ( ComponentNameM.CompareNoCase(ComponentPtr->GetComponentName()) != 0 ) { continue; }
		return ComponentPtr;
	}	
	return NULL;
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectComponentPtrByComponentFullName(int PanelIndex, int BoardIndex, LPCWSTR ComponentName)//取得專案零件指標-依據全名
{
	size_t         i = 0;		
	CAOIComponent *ComponentPtr = NULL;	
	CString        ComponentNameM(ComponentName);
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		

	ComponentNameM.MakeUpper();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentPanelIndex_Project() != PanelIndex ) { continue; }
		if ( ComponentPtr->GetComponentBoardIndex_Panel() != BoardIndex ) { continue; }		
		if ( ComponentNameM.CompareNoCase(ComponentPtr->GetComponentName()) != 0 ) { continue; }
		return ComponentPtr;
	}	
	return NULL;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectComponentSelectedCount() const//計算專案零件選取到數量
{
	size_t         i = 0;
	size_t         Count = 0;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }		
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		Count ++;
	}	
	return Count;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::GetProjectComponentSelected(std::vector<CAOIComponent*> &SelComponentList)//取得專案零件選取到
{
	size_t         i = 0;	
	CString        ComponentNameC, ComponentNameM;	
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }		
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		SelComponentList.push_back(ComponentPtr);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::GetProjectComponentModelIsolated(std::vector<CAOIComponent*> &SelComponentList)//取得專案零件模組隔離的
{
	size_t         i = 0;	
	CString        ComponentNameC, ComponentNameM;	
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();
	SelComponentList.clear();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }		
		if ( ComponentPtr->GetComponentModelIsolated() == false ) { continue; }		
		SelComponentList.push_back(ComponentPtr);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ApplyProjectCompnentModelToLibraryModel(CAOIComponent *ComponentPtr, bool UpdateToeOthers)//更新零件模組至資料庫
{
	if ( NULL == ComponentPtr ) { return false; }
	const bool ModelIsolated = ComponentPtr->GetComponentModelIsolated();
	if ( true == ModelIsolated ) { return true; }
	if ( ComponentPtr->CheckComponentType_ModelTest() == false ) { return true; }
	
	bool         bSearchModel = false;
	CAOIModel   *ModelPtr = NULL;	
	unsigned int ModelIndex = 0;
	CString ModelNameM;		
	CString ModelNameC = ComponentPtr->GetComponentModelName();
	const size_t ModelCount = GetProjectModelCount();

	ModelNameC.MakeUpper();
	bSearchModel = false;
	ModelIndex = ComponentPtr->GetComponentModelIndex();
	ModelPtr = GetProjectModelPtr(ModelIndex, true);
	if ( NULL == ModelPtr )
	{	bSearchModel = true;	}
	else
	{	
		ModelNameM = ModelPtr->GetModelName();		
		ModelNameM.MakeUpper();
		if ( ModelNameM != ModelNameC )
		{	bSearchModel = true;	}
	}
	if ( true == bSearchModel )
	{	ModelPtr = GetProjectModelPtrByModelName(ModelNameC);	}
	if ( NULL == ModelPtr ) { return false; }
	
	TPOINT2D     psCad, psStage;	
	CAOIModel   *ModelPtrC = ComponentPtr->GetComponentModelPtr();	
	const bool   ModelModified = ModelPtrC->GetModelModifiedCount();
	const double ComponentAngle = ComponentPtr->GetComponentAngle();	
	time_t       ModelModifiedTime_M = ModelPtr->GetModelModifiedDateTime();
	time_t       ModelModifiedTime_C = ModelPtrC->GetModelModifiedDateTime();	

	ModelPtrC->ResetModelWndAlgParam();
	ModelIndex = ModelPtr->GetModelIndex();	
	*ModelPtr = *ModelPtrC;	
	
	ModelPtr->SetModelIndex(ModelIndex);
	ModelPtr->RotateModel(-ComponentAngle, 0, 0);
	ModelPtr->SetModelComponentPtr(NULL);
	ModelPtr->SetModelAttachedAngle(0);	
	ModelPtr->SetModelAttachedPosCad(psCad);
	ModelPtr->SetModelAttachedPosStage(psStage);
	ModelPtr->InitModelInspection();	
	if ( ModelModifiedTime_M > ModelModifiedTime_C )
	{	ModelPtr->SetModelModifiedDateTime(ModelModifiedTime_M); }

	ModelPtr->UnSelectModel();
	ModelPtr->InvisibleModelWnd();
	ModelPtr->SetModelIsolated(false);
	ModelPtrC->SetModelIndex(ModelIndex);		
	if ( true == UpdateToeOthers )
	{
		ModelPtrC->SetModelWndModified(false);
		ModelPtrC->SetModelLogicModified(false);	
		ModelPtrC->SetModelLandModified(false);
		ModelPtrC->SetModelModifiedCount(false);	
	}
	ModelPtrC->SetModelNeedSaveFiles(true);
	ModelPtr->GetModelBodyBox().SetBoxSelected(false);
	ComponentPtr->SetComponentModelIndex(ModelIndex);	
	
	bool IsOK = true;
	if ( true == UpdateToeOthers )
	{	
		BackupProjectComponentSelected();
		SelectProjectAllComponents(false);
		SelectProjectComponentsByModelName(ModelNameC, false);
		ComponentPtr->SetComponentSelected(false);
		IsOK = ApplyProjectModelToComponentsSelected(ModelPtr);
		SelectProjectAllComponents(false);
		RestoreProjectComponentSelected();
		ComponentPtr->SetComponentSelected(true);
	}	
	
	ModelPtr->SetModelWndModified(false);
	ModelPtr->SetModelLogicModified(false);	
	ModelPtr->SetModelLandModified(false);
	ModelPtr->SetModelModifiedCount(false);		
	ModelPtr->SetModelNeedSaveFiles(true);	
	return IsOK;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ApplyProjectCompnentModelToLibraryGroup(TModelUpdateToGroupParam Param, CAOIModel *RefModelPtr)//更新零件模組至資料庫群組
{
	CAOIProject *ProjectPtr = this;	
	if ( NULL == ProjectPtr ) { return false; }		
	if ( NULL == RefModelPtr ) { return false; }	

	std::vector<CAOIModel*> ModelList;
	if ( ListProjectModelByGroupName(RefModelPtr, ModelList) == false ) 
	{	return false; }

	bool         bModified=false;
	LAND_TYPE    LandType;
	LAND_TYPE    RefLandType;
	int          LandGroupID=0;
	int          RefLandGroupID=0;
	size_t       i=0, j=0, k=0;
	size_t       WndCount;	
	CString      ModelName;
	CString      GroupName;
	CString      ModelFolder;	
	CString      ModelFolderTmp;	
	CAOIWnd     *WndPtr = NULL;
	CAOIWnd     *RefWndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	CAOILand    *RefLandPtr = NULL;
	CAOIModel   *ModelPtr = NULL;	
	CAOIModel   *NewModelPtr = NULL;		
	ALG_TYPE      AlgType;
	ALG_TYPE      RefAlgType;
	BOX_TOWARD    WndToward;
	BOX_TOWARD    RefWndToward;
	WND_DEFECT_ID     WndDefectID;
	WND_DEFECT_ID     RefWndDefectID;		
	BOX_SHAPE_MODE    RefWndShapeMode;
	double            RefWndShapeParam1=0;
	double            RefWndShapeParam2=0;
	WND_LOGIC_TYPE    RefWndLogicType;	
	WND_RGN_LINK_MODE WndRgnLinkMode;
	WND_RGN_LINK_MODE RefWndRgnLinkMode;
	bool          RefWndEnabled=false;
	double        ExtendRangeX=0.0, ExtendRangeY=0.0;
	double        RefExtendRangeX=0.0, RefExtendRangeY=0.0;
	double        WndRgnLinkRatioX=1.0, WndRgnLinkRatioY=1.0;
	double        RefWndRgnLinkRatioX=1.0, RefWndRgnLinkRatioY=1.0;	
	int           RefModelMaskFlag=0;
	int           DefectGroupID=0;
	int           RefDefectGroupID=0;
	int           WndLogicGroupID=0;
	int           RefWndLogicGroupID=0;	
	std::vector<unsigned int> SelWndIndexList;
	const size_t ModelCount = ModelList.size();	
	CString      ModelNameRef = RefModelPtr->GetModelName();
	CString      ModelFolderRef = RefModelPtr->GetModelFolderModel();	
	CString      TempFolder = AOIDataCollect.GetAOITempDirectory();
	const double AttachedAngle = RefModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);

	std::vector<CAOIWnd*> SelWndList;
	RefModelPtr->GetModelWndSelectedList(SelWndList);	
	const size_t RefWndCount = SelWndList.size();

	ProjectPtr->SelectProjectAllComponents(false);	
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = ModelList[i];
		if ( NULL == ModelPtr ) { continue; }
		if ( ModelPtr == RefModelPtr )
		{	continue; }				

		bModified=false;
		SelWndIndexList.clear();
		ModelName = ModelPtr->GetModelName();
		GroupName = ModelPtr->GetModelGroupName();
		ModelFolder = ModelPtr->GetModelFolderModel();
		if ( ModelFolder.CompareNoCase(ModelFolderRef) == 0 ) { continue; }

		if ( true == Param.bUpdateAll )
		{
			NewModelPtr = RefModelPtr->CloneModelObj();
			if ( NULL == NewModelPtr ) { continue; }

			//先確認資料夾複製正常
			ModelFolderTmp.Format(_T("%s\\%s"), TempFolder, ModelName);//建立暫存資料夾
			::CreateDirectory(ModelFolderTmp, NULL);	::Sleep(0);		
			//將來源資料夾複製到暫存資料夾
			if ( JetAPI::CopyFolderAToFolderB(ModelFolderRef, ModelFolderTmp, false, true, _T(""), -1, -1) == false )
			{	
				JetAPI::RemoveFolder(ModelFolderTmp);				
				continue; 
			}
			//將原始檔案複製到暫存資料夾
			if ( JetAPI::CopyFolderAToFolderB(ModelFolder, ModelFolderTmp, false, false, _T(""), 0, 0) == false )
			{
				JetAPI::RemoveFolder(ModelFolderTmp);				
				continue;
			}

			NewModelPtr->SetModelName(ModelName);
			NewModelPtr->SetModelGroupName(GroupName);
			NewModelPtr->SetModelFolderModel(ModelFolder);
			NewModelPtr->AssignModelFolder();
			NewModelPtr->SetModelNeedSaveFiles(true);
			NewModelPtr->SetModelBKImageNeedToGrab(true);
			ModelPtr = ProjectPtr->ReplaceProjectModel(NewModelPtr);
			if ( NULL == ModelPtr ) 
			{
				JetAPI::RemoveFolder(ModelFolderTmp);				
				AOIObjManager.DestroyModelObj(NewModelPtr);
				NewModelPtr = NULL;
				continue;
			}
			//將來源資料夾複製到暫存資料夾
			ModelPtr->SetModelNeedSaveFiles(true);
			JetAPI::CopyFolderAToFolderB(ModelFolderTmp, ModelFolder, true, true, _T(""), -1, -1);		
			bModified = true;
		}
		else
		{				
			TREGION4D WndRgn;
			TREGION4D RefWndRgn;
			TREGION4D WndRgnEx;
			TREGION4D RefWndRgnEx;
			TSIZE2D   RefWndSize;
			TSIZE2D   RefWndSizeEx;			
			CString   ModelFolder;
			CString   PatternFolder;
			CString   AlgPatternFolder;
			int       AlgGroupID=0;
			int       PatternFolderIndex=0;
			bool      bFound=false;
			bool      bChangeExtend=false;
			bool      bChangeRgnLink=false;
			bool      bAlgPatternFolder=false;
			bool      bRefAlgPatternFolder=false;			
			CAOIWnd   Wnd_T, Wnd_L, Wnd_B, Wnd_R;
			for ( j=0; j<RefWndCount; j++ )
			{
				RefWndPtr = SelWndList[j];
				if ( NULL == RefWndPtr ) { continue; }	
				Wnd_T=Wnd_L=Wnd_B=Wnd_R=*RefWndPtr;
				RefAlgType = RefWndPtr->GetWndAlgType();
				RefLandPtr = RefWndPtr->GetWndLandPtr();
				RefWndToward = RefWndPtr->GetWndToward();
				RefWndDefectID = RefWndPtr->GetWndDefectID();
				RefDefectGroupID = RefWndPtr->GetWndDefectGroupID();
				switch ( RefWndToward )
				{
				case BOX_TOWARD_LEFT:					
					Wnd_B.SpinWnd(90);
					Wnd_R.SpinWnd(180);
					Wnd_T.SpinWnd(270);
					break;
				case BOX_TOWARD_UP:
					Wnd_L.SpinWnd(90);
					Wnd_B.SpinWnd(180);
					Wnd_R.SpinWnd(270);					
					break;
				case BOX_TOWARD_RIGHT:
					Wnd_T.SpinWnd(90);
					Wnd_L.SpinWnd(180);
					Wnd_B.SpinWnd(270);
					break;
				case BOX_TOWARD_DOWN:
					Wnd_R.SpinWnd(90);
					Wnd_T.SpinWnd(180);
					Wnd_L.SpinWnd(270);
					break;
				}

				LandPtr = NULL;
				bChangeRgnLink=false;				
				WndCount = ModelPtr->GetModelWndCount();
				ModelFolder = ModelPtr->GetModelFolder();
				for ( k=0; k<WndCount; k++ )
				{
					WndPtr = ModelPtr->GetModelWndPtr(k, false);
					if ( NULL == WndPtr ) { continue; }
					AlgType = WndPtr->GetWndAlgType();					
					WndToward = WndPtr->GetWndToward();
					WndDefectID = WndPtr->GetWndDefectID();
					DefectGroupID = WndPtr->GetWndDefectGroupID();
					//if ( WndToward != RefWndToward ) { continue; }
					if ( WndDefectID != RefWndDefectID ) { continue; }
					if ( DefectGroupID != RefDefectGroupID ) { continue; }
					switch ( WndToward )
					{
					case BOX_TOWARD_LEFT:	RefWndPtr=&(Wnd_L);	break;
					case BOX_TOWARD_UP:		RefWndPtr=&(Wnd_T);	break;
					case BOX_TOWARD_RIGHT:	RefWndPtr=&(Wnd_R);	break;
					case BOX_TOWARD_DOWN:	RefWndPtr=&(Wnd_B);	break;
					default:
						RefWndPtr = NULL;
						break;
					}
					if ( NULL == RefWndPtr ) { continue; }
					
					RefWndPtr->GetWndRegion(RefWndRgn);
					RefWndPtr->GetWndExtendBox().GetBoxRegion(RefWndRgnEx);

					RefWndSize.cx = RefWndRgn.GetWidth();
					RefWndSize.cy = RefWndRgn.GetHeight();
					RefWndSizeEx.cx = RefWndRgnEx.GetWidth();
					RefWndSizeEx.cy = RefWndRgnEx.GetHeight();

					RefWndEnabled=RefWndPtr->GetWndEnabled();				
					RefWndLogicType = RefWndPtr->GetWndLogicType();				
					RefWndLogicGroupID=RefWndPtr->GetWndLogicGroupID();
					RefModelMaskFlag = RefWndPtr->GetWndModelMaskFlag();
					RefExtendRangeX = RefWndPtr->GetWndExtendRangeX();
					RefExtendRangeY = RefWndPtr->GetWndExtendRangeY();
					RefWndRgnLinkMode = RefWndPtr->GetWndRgnLinkMode();
					RefWndRgnLinkRatioX = RefWndPtr->GetWndRgnLinkRatioX();
					RefWndRgnLinkRatioY = RefWndPtr->GetWndRgnLinkRatioY();
					//Aboud Shpae Param
					RefWndShapeMode = RefWndPtr->GetWndShapeMode();
					RefWndShapeParam1 = RefWndPtr->GetWndShapeParam();
					RefWndShapeParam2 = RefWndPtr->GetWndShapeParam2();	
				
					CAlgParam &RefAlgParam = RefWndPtr->GetWndAlgParam();
					bRefAlgPatternFolder = RefAlgParam.CheckAlgPatternFileUsed(RefAlgType);
					
					ExtendRangeX = WndPtr->GetWndExtendRangeX();
					ExtendRangeY = WndPtr->GetWndExtendRangeY();
					WndRgnLinkMode = WndPtr->GetWndRgnLinkMode();
					WndRgnLinkRatioX = WndPtr->GetWndRgnLinkRatioX();
					WndRgnLinkRatioY = WndPtr->GetWndRgnLinkRatioY();							
					//Re-check Algorithm
					//if ( AlgType != RefAlgType ) { continue; }
					
					bFound = true;
					bModified = true;
					bChangeExtend=false;
					CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
					AlgGroupID = AlgParam.GetAlgGroupID();
					bAlgPatternFolder = AlgParam.CheckAlgPatternFileUsed(AlgType);	
					if ( true == Param.bUpdateParam )
					{							
						AlgParam.CopyAlgParam_GenParam(RefAlgParam, Param.bKeepSpec);	
						if ( WndRgnLinkMode != RefWndRgnLinkMode )
						{
							bChangeRgnLink = true;
							WndPtr->SetWndRgnLinkMode(RefWndRgnLinkMode);	
						}
						if ( WndRgnLinkRatioX != RefWndRgnLinkRatioX )
						{
							bChangeRgnLink = true;
							WndPtr->SetWndRgnLinkRatioX(RefWndRgnLinkRatioX);	
						}
						if ( WndRgnLinkRatioY != RefWndRgnLinkRatioY )
						{
							bChangeRgnLink = true;
							WndPtr->SetWndRgnLinkRatioY(RefWndRgnLinkRatioY);	
						}

						//Enable
						WndPtr->SetWndEnabled(RefWndEnabled);
						WndPtr->SetWndModelMaskFlag(RefModelMaskFlag);

						//Logic Param
						WndPtr->SetWndLogicType(RefWndLogicType);
						WndPtr->SetWndLogicGroupID(RefWndLogicGroupID);	

						//Shape Mode
						WndPtr->SetWndShapeMode(RefWndShapeMode);
						WndPtr->SetWndShapeParam(RefWndShapeParam1);
						WndPtr->SetWndShapeParam2(RefWndShapeParam2);

						if ( ExtendRangeX != RefExtendRangeX )
						{
							bChangeExtend = true;
							WndPtr->SetWndExtendRangeX(RefExtendRangeX);	
						}
						if ( ExtendRangeY != RefExtendRangeY )
						{
							bChangeExtend = true;
							WndPtr->SetWndExtendRangeY(RefExtendRangeY);	
						}
						if ( true == bChangeExtend )
						{	WndPtr->UpdateWndExtendBox();	}

						if ( bRefAlgPatternFolder != bAlgPatternFolder )
						{
							if ( false == bRefAlgPatternFolder )
							{	AlgParam.ClearAlgPatternFiles();	}
							else
							{	
								PatternFolderIndex = AlgGroupID;	
								PatternFolder = AOIDataDefine.GetAlgPatternFolder(PatternFolderIndex);
								AlgPatternFolder.Format(_T("%s\\%s"), ModelFolder, PatternFolder);
								AlgParam.SetAlgPatternFolder(AlgPatternFolder);
								::CreateDirectory(AlgPatternFolder, NULL);
							}
						}
					}
					if ( true == Param.bUpdateBinary )
					{	AlgParam.CopyAlgParam_BinyParam(RefAlgParam);	}

					if ( true == Param.bUpdateWndSize )
					{
						WndPtr->GetWndRegion(WndRgn);
						WndPtr->SetWndExtendRangeX(RefExtendRangeX);
						WndPtr->SetWndExtendRangeX(RefExtendRangeY);
						WndPtr->GetWndExtendBox().GetBoxRegion(WndRgnEx);
						WndRgn.SetSize(RefWndSize.cx, RefWndSize.cy);
						WndRgnEx.SetSize(RefWndSizeEx.cx, RefWndSizeEx.cy);
						WndPtr->SetWndRegion(WndRgn);
						WndPtr->GetWndExtendBox().SetBoxRegion(WndRgnEx);
					}
					SelWndIndexList.push_back(k);
				}

				if ( true==Param.bUpdateAddOne && false==bFound )
				{//Add New One		
					TPOINT2D AttachedCadPos;
					TPOINT2D AttachedStagePos;
					int NewWndBandID=0;
					int NewWndGroupID=ModelPtr->GetModelWndFreeGroupID();	
					double AttachedAngle = ModelPtr->GetModelAttachedAngle();

					ModelPtr->GetModelAttachedPosCad(AttachedCadPos);
					ModelPtr->GetModelAttachedPosStage(AttachedStagePos);					
					WndPtr = RefWndPtr->CloneWndObj();
					if ( NULL != WndPtr )
					{	
						bModified = true;
						//LandPtr = NULL;						
						WndPtr->SetWndBandID(NewWndBandID);
						WndPtr->SetWndGroupID(NewWndGroupID);						
						WndPtr->SetWndAttachedAngle(AttachedAngle);
						WndPtr->SetWndAttachedPosCad(AttachedCadPos);
						WndPtr->SetWndAttachedPosStage(AttachedStagePos);

						if ( NULL == RefLandPtr ) 
						{	ModelPtr->AddModelWndPtr(WndPtr, false); }
						else
						{							
							ModelPtr->AddModelWndToOtherLand(RefLandPtr, WndPtr, NewWndBandID); 
							AOIObjManager.DestroyWndObj(WndPtr);
							WndPtr = NULL;
						}
					}
				}
				if ( true==Param.bUpdateDelete && true==bFound )
				{	ModelPtr->DeleteModelWndIndexList(SelWndIndexList);	}

				if ( true == bChangeRgnLink )
				{	ModelPtr->UpdateModelWndRgnByLinkMode();	}
				ModelPtr->CalcModelTotalRegionAll();
				ModelPtr->SetModelNeedSaveFiles(true);
			}
		}

		if ( false == bModified ) { continue; }

		//複製模組出來		
		ProjectPtr->SelectProjectComponentsByModelName(ModelName, false);
		ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtr);
		ProjectPtr->SelectProjectAllComponents(false);
	}
	return true;	
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SyncProjectCompnentModelToLibraryModel(CAOIComponent *ComponentPtr, bool bPartial, bool UpdateToeOthers)//同步化零件模組至資料庫
{
	if ( NULL == ComponentPtr ) { return false; }
	const bool ModelIsolated = ComponentPtr->GetComponentModelIsolated();
	if ( true == ModelIsolated ) { return true; }
	if ( ComponentPtr->CheckComponentType_ModelTest() == false ) { return true; }
	
	bool         bSearchModel = false;
	CAOIModel   *ModelPtr = NULL;	
	unsigned int ModelIndex = 0;
	CString ModelNameM;		
	CString ModelNameC = ComponentPtr->GetComponentModelName();
	const size_t ModelCount = CAOIProject::GetProjectModelCount();

	ModelNameC.MakeUpper();
	bSearchModel = false;
	ModelIndex = ComponentPtr->GetComponentModelIndex();
	ModelPtr = GetProjectModelPtr(ModelIndex, true);
	if ( NULL == ModelPtr )
	{	bSearchModel = true;	}
	else
	{	
		ModelNameM = ModelPtr->GetModelName();		
		ModelNameM.MakeUpper();
		if ( ModelNameM != ModelNameC )
		{	bSearchModel = true;	}
	}
	if ( true == bSearchModel )
	{	ModelPtr = GetProjectModelPtrByModelName(ModelNameC);	}
	if ( NULL == ModelPtr ) { return false; }
	
	TPOINT2D     psCad, psStage;
	CAOIModel   *ModelPtrC = ComponentPtr->GetComponentModelPtr();
	const bool   ModelModified = ModelPtrC->GetModelModifiedCount();
	const double ComponentAngle = ComponentPtr->GetComponentAngle();
	time_t       ModelModifiedTime_M = ModelPtr->GetModelModifiedDateTime();
	time_t       ModelModifiedTime_C = ModelPtrC->GetModelModifiedDateTime();

	ModelPtrC->ResetModelWndAlgParam();
	ModelIndex = ModelPtr->GetModelIndex();	
	*ModelPtr = *ModelPtrC;		
	
	ModelPtr->SetModelIndex(ModelIndex);
	ModelPtr->RotateModel(-ComponentAngle, 0, 0);
	ModelPtr->SetModelComponentPtr(NULL);	
	ModelPtr->SetModelAttachedAngle(0);	
	ModelPtr->SetModelAttachedPosCad(psCad);
	ModelPtr->SetModelAttachedPosStage(psStage);		
	ModelPtr->InitModelInspection();		
	if ( ModelModifiedTime_M > ModelModifiedTime_C )
	{	ModelPtr->SetModelModifiedDateTime(ModelModifiedTime_M); }	

	ModelPtrC->SetModelIndex(ModelIndex);
	if ( true == UpdateToeOthers )
	{
		ModelPtrC->SetModelWndModified(false);
		ModelPtrC->SetModelLogicModified(false);
		ModelPtrC->SetModelLandModified(false);
		ModelPtrC->SetModelModifiedCount(false);
	}
	ModelPtrC->SetModelNeedSaveFiles(true);
	ModelPtr->GetModelBodyBox().SetBoxSelected(false);
	ComponentPtr->SetComponentModelIndex(ModelIndex);	

	bool IsOK = true;
	if ( true == UpdateToeOthers )
	{	
		BackupProjectComponentSelected();
		SelectProjectAllComponents(false);
		SelectProjectComponentsByModelName(ModelNameC, false);
		ComponentPtr->SetComponentSelected(false);
		IsOK = SyncProjectModelToComponentsSelected(ModelPtr, bPartial);		
		SelectProjectAllComponents(false);
		RestoreProjectComponentSelected();
		ComponentPtr->SetComponentSelected(true);
	}
	ModelPtr->UnSelectModel();
	ModelPtr->InvisibleModelWnd();
	ModelPtr->SetModelIsolated(false);	
	ModelPtr->SetModelLandModified(false);
	ModelPtr->SetModelWndModified(false);
	ModelPtr->SetModelLogicModified(false);
	ModelPtr->SetModelModifiedCount(false);	
	ModelPtr->SetModelNeedSaveFiles(true);	
	return IsOK;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RotateProjectComponentSelected(double Angle)//選轉零件角度
{
	size_t          i = 0;
	DISTRICT_ID     DistrictID;
	CAOIBoard      *BoardPtr = NULL;
	CAOIPanel      *PanelPtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;	
	CMapCoordinate *MapCTSPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		

	SelectProjectComponentsMasterAgent();

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }		
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		MapCTSPtr = NULL;
		PanelPtr = ComponentPtr->GetComponentPanelPtr();
		BoardPtr = ComponentPtr->GetComponentBoardPtr();
		DistrictID = ComponentPtr->GetComponentDistrictID();
		if ( NULL==MapCTSPtr && NULL!=BoardPtr ) 
		{	MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);	}
		if ( NULL==MapCTSPtr && NULL!=PanelPtr ) 
		{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);	}		
		ComponentPtr->SpinComponent(Angle, MapCTSPtr);
	}
	
	const size_t PanelCount = GetProjectPanelCount();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		PanelPtr->CalcPanelStagePosition();		
	}
	
	const size_t BoardCount = GetProjectBoardCount();
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
bool CAOIProject::ReverseProjectComponentSelected()//反轉零件角度
{
	size_t          i = 0;
	DISTRICT_ID     DistrictID;
	double          RotAngle = 0;
	double          OrgAngle = 0;
	double          NewAngle = 0;	
	CAOIBoard      *BoardPtr = NULL;
	CAOIPanel      *PanelPtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;	
	CMapCoordinate *MapCTSPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		

	SelectProjectComponentsMasterAgent();

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }		
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		OrgAngle = ComponentPtr->GetComponentAngle();
		NewAngle = 360-OrgAngle;
		RotAngle = NewAngle-OrgAngle;
		if ( RotAngle < 0 ) { RotAngle += 360; }
		if ( RotAngle >= 360 ) { RotAngle -= 360; }

		MapCTSPtr = NULL;
		PanelPtr = ComponentPtr->GetComponentPanelPtr();
		BoardPtr = ComponentPtr->GetComponentBoardPtr();
		DistrictID = ComponentPtr->GetComponentDistrictID();
		if ( NULL==MapCTSPtr && NULL!=BoardPtr ) 
		{	MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);	}
		if ( NULL==MapCTSPtr && NULL!=PanelPtr ) 
		{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);	}
		ComponentPtr->SpinComponent(RotAngle, MapCTSPtr);
	}		
	
	const size_t PanelCount = GetProjectPanelCount();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		PanelPtr->CalcPanelStagePosition();		
	}
	
	const size_t BoardCount = GetProjectBoardCount();
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
bool CAOIProject::InvertSelectProjectComponent()//反向選取零件
{
	size_t         i = 0;
	bool           Selected = false;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		

	SelectProjectComponentsMasterAgent();

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == true ) 
		{	Selected = false; }
		else 
		{	Selected = true; }
		ComponentPtr->SetComponentSelected(Selected);		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedSaveLeadReport(bool bSaved)//設定專案選取到零件的儲存引腳報告
{
	size_t         i = 0;		
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->GetComponentModelPtr()->SetModelSaveLeadReport(bSaved);		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedNozzleName(LPCTSTR NozzleName)//設定專案選取到零件的吸嘴名稱
{
	if ( NULL == NozzleName ) { return false; }

	size_t         i = 0;	
	std::wstring   wsNozzleName;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();

	JetAPI::TCHAR2wstring(NozzleName, wsNozzleName);
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentNozzleName(wsNozzleName.c_str());		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedPartNumber(LPCTSTR PartNumber)//設定專案選取到零件的料號
{
	if ( NULL == PartNumber ) { return false; }

	size_t         i = 0;	
	std::wstring   wsPartNumber;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();

	JetAPI::TCHAR2wstring(PartNumber, wsPartNumber);
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentPartNumber(wsPartNumber.c_str());		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentName(LPCTSTR RefName, LPCTSTR NewName)//設定專案零件的名稱
{
	if ( NULL==RefName || NULL==NewName ) { return false; }
	size_t         i = 0;		
	CString        ComponentNameNew;
	CString        ComponentNameC, ComponentNameM;	
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		

	ComponentNameM = RefName;
	ComponentNameNew = NewName;
	ComponentNameM.MakeUpper();
	ComponentNameNew.MakeUpper();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }		
		ComponentNameC = ComponentPtr->GetComponentName();
		ComponentNameC.MakeUpper();
		if ( ComponentNameC != ComponentNameM ) { continue; }
		ComponentPtr->ChangeComponentName(ComponentNameNew);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentPartNumber(LPCTSTR RefPartNumber, LPCTSTR NewPartNumber)//設定專案零件的料號
{
	if ( NULL==RefPartNumber || NULL==NewPartNumber ) { return false; }
	size_t         i = 0;		
	CString        PartNumberNew;
	CString        PartNumberC, PartNumberM;	
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		

	PartNumberM = RefPartNumber;
	PartNumberNew = NewPartNumber;
	PartNumberM.MakeUpper();
	PartNumberNew.MakeUpper();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }		
		PartNumberC = ComponentPtr->GetComponentPartNumber();
		PartNumberC.MakeUpper();
		if ( PartNumberC != PartNumberM ) { continue; }
		ComponentPtr->SetComponentPartNumber(PartNumberNew);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedComponentName(LPCTSTR RefName, LPCTSTR NewName)//設定專案選取到零件的名稱
{	
	if ( NULL == RefName ) { return false; }
	if ( NULL == NewName ) { return false; }

	size_t         i = 0;	
	CString        ComponentNameNew;
	CString        ComponentNameC, ComponentNameM;	
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   BoardCount = GetProjectBoardCount();
	const size_t   ComponentCount = GetProjectComponentCount_Inline();
	const int      CheckComponentName_None=0;//未確認
	const int      CheckComponentName_Done=1;//確認過

	ComponentNameM = RefName;
	ComponentNameNew = NewName;
	ComponentNameM.MakeUpper();
	ComponentNameNew.MakeUpper();	
	for ( i=0; i<BoardCount; i++ )
	{
		CAOIBoard *BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->SetBoardTempInt(CheckComponentName_None);
	}
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentSelected(false);
		ComponentNameC = ComponentPtr->GetComponentName();
		ComponentNameC.MakeUpper();
		if ( ComponentNameC != ComponentNameM )
		{	continue;	}
		CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();
		if ( NULL == BoardPtr )
		{	continue; }
		const int nTempValue = BoardPtr->GetBoardTempInt();
		if ( CheckComponentName_Done == nTempValue )
		{	continue; }
		BoardPtr->SetBoardTempInt(CheckComponentName_Done);		
		if ( BoardPtr->ChceckBoardComponentNameExist(NewName) == true )
		{	continue;	}
		ComponentPtr->SetComponentSelected(true);
		ComponentPtr->ChangeComponentName(ComponentNameNew);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedDefectItemEssential(const CWndDefectItem &DefectItem)//設定專案選取到零件瑕疵必要項目
{
	size_t         i = 0;		
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->GetComponentModelPtr()->SetModelDefectItemEssential(DefectItem);		
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedDefectItemRecheck_ARS(const CWndDefectItem &DefectItem)//設定專案選取到零件瑕疵重複確認-ARS
{
	size_t         i = 0;		
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->GetComponentModelPtr()->SetModelDefectItemRecheck_ARS(DefectItem);		
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::MoveProjectComponentSelected(double dX, double dY)//移動專案選取到的零件
{
	size_t          i = 0;
	DISTRICT_ID     DistrictID;
	CAOIPanel      *PanelPtr = NULL;
	CAOIBoard      *BoardPtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;	
	CMapCoordinate *MapCTSPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		

	SelectProjectComponentsMasterAgent();

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		MapCTSPtr = NULL;		
		PanelPtr = ComponentPtr->GetComponentPanelPtr();
		BoardPtr = ComponentPtr->GetComponentBoardPtr();
		DistrictID = ComponentPtr->GetComponentDistrictID();
		if ( NULL==MapCTSPtr && NULL!=BoardPtr ) 
		{	MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);	}
		if ( NULL==MapCTSPtr && NULL!=PanelPtr ) 
		{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);	}
		ComponentPtr->MoveComponentPos(dX, dY, MapCTSPtr);
	}
	LayoutProjectRegion();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::MirrorXProjectComponentSelected()//鏡射專案選取到零件的X座標
{
	size_t          i = 0;	
	double          CadCpX=0;
	double          StageCpX=0;	
	DISTRICT_ID     DistrictID;
	CAOIPanel      *PanelPtr = NULL;
	CAOIBoard      *BoardPtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;	
	CMapCoordinate *MapCTSPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		

	SelectProjectComponentsMasterAgent();

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		
		MapCTSPtr = NULL;
		CadCpX = ComponentPtr->GetComponentCadPosX();
		StageCpX = ComponentPtr->GetComponentStagePosX();		
		PanelPtr = ComponentPtr->GetComponentPanelPtr();
		BoardPtr = ComponentPtr->GetComponentBoardPtr();
		DistrictID = ComponentPtr->GetComponentDistrictID();
		if ( NULL==MapCTSPtr && NULL!=BoardPtr ) 
		{	MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);	}
		if ( NULL==MapCTSPtr && NULL!=PanelPtr ) 
		{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);	}
		ComponentPtr->MirrorXComponent(CadCpX, MapCTSPtr);		
	}
	LayoutProjectRegion();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::MirrorYProjectComponentSelected()//鏡射專案選取到零件的Y座標
{
	size_t          i = 0;	
	double          CadCpY=0;
	double          StageCpY=0;	
	DISTRICT_ID     DistrictID;
	CAOIPanel      *PanelPtr = NULL;
	CAOIBoard      *BoardPtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;	
	CMapCoordinate *MapCTSPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		

	SelectProjectComponentsMasterAgent();

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		
		MapCTSPtr = NULL;
		CadCpY = ComponentPtr->GetComponentCadPosY();
		StageCpY = ComponentPtr->GetComponentStagePosY();		
		PanelPtr = ComponentPtr->GetComponentPanelPtr();
		BoardPtr = ComponentPtr->GetComponentBoardPtr();
		DistrictID = ComponentPtr->GetComponentDistrictID();
		if ( NULL==MapCTSPtr && NULL!=BoardPtr ) 
		{	MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);	}
		if ( NULL==MapCTSPtr && NULL!=PanelPtr ) 
		{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);	}
		ComponentPtr->MirrorYComponent(CadCpY, MapCTSPtr);		
	}
	LayoutProjectRegion();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ResetProjectComponentModelSelected()//復歸選取到的專案零件模組
{
	size_t         i = 0;	
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		ModelPtr->BuildModelType(MODEL_TYPE_NULL);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ChangeProjectComponentSelectedBoard(CAOIBoard *RefBoardPtr)//切換專案選取單板的整板指標
{
	CAOIProject *ProjectPtr = this;	
	if ( NULL == ProjectPtr ) { return false; }
	if ( NULL == RefBoardPtr ) { return false; }

	size_t         i=0;		
	CAOIPanel     *PanelPtr=NULL;
	CAOIBoard     *BoardPtr=NULL;	
	CAOIComponent *ComponentPtr=NULL;
	std::vector<CAOIComponent*> SelComponentList;	
	ProjectPtr->SelectProjectComponentsMasterAgent();
	ProjectPtr->GetProjectComponentSelected(SelComponentList);
	const size_t SelComponentCount = SelComponentList.size();

	for ( i=0; i<SelComponentCount; i++ )
	{
		ComponentPtr = SelComponentList[i];
		if ( NULL == ComponentPtr ) { continue; }
		BoardPtr = ComponentPtr->GetComponentBoardPtr();		
		if ( BoardPtr == RefBoardPtr ) { continue; }

		if ( ComponentPtr->CheckComponentIsAgent() == false )
		{
			const wchar_t *ComName=ComponentPtr->GetComponentName();
			if ( RefBoardPtr->ChceckBoardComponentNameExist(ComName) == true ) { continue; }
		}
		ComponentPtr->ChangeComponentBoard(RefBoardPtr);
	}		
	LayoutProjectRegion();
	SetProjectActiveComponent(ComponentPtr);
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectComponentValid(const CAOIComponent *RefComponentPtr)//確認專案的零件指標有效
{
	if ( NULL == RefComponentPtr ) 
	{
		m_ErrorString = _T("Error, NULL == RefComponentPtr");
		return false; 
	}
	size_t         i = 0;	
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount_Inline();			
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( RefComponentPtr == ComponentPtr ) 
		{	return true; }
	}

	CString ComponentName = RefComponentPtr->GetComponentName();
	m_ErrorString.Format(_T("Error, Check Project Component[%s] Vaild Fault"), ComponentName);
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::GetProjectComponentModelInfoList(bool bModelIsolated, std::vector<TModelInfo> &ModelInfoList)//取得專案零件模組資訊列表
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPtr(ProjectPtr) == false )
	{	return false; }
	
	size_t         i = 0;		
	TModelInfo     ModelInfo;
	CAOIModel     *ModelPtr=NULL;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount_Inline();			
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		if ( true == bModelIsolated )
		{
			if ( ComponentPtr->GetComponentModelIsolated() == false )
			{	continue; }
		}
		ModelInfo.sModelPtr = ModelPtr;
		ModelInfo.sModelName = ComponentPtr->GetComponentFullName();
		ModelInfo.sModifiedTime = ModelPtr->GetModelModifiedDateTime();
		ModelInfoList.push_back(ModelInfo);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::PasteProjectComponentList(std::vector<CAOIComponent*> &CloneComponentList, double StageOffsetX, double StageOffsetY, double RotateAngle)//貼上專案零件
{
	size_t          i=0, j=0;
	size_t          BoardIndex=0;
	size_t          ComponentIndex=0;
	TPOINT2D        CadPosOld;
	TPOINT2D        CadPosNew;
	TPOINT2D        StagePosOld;
	TPOINT2D        StagePosNew;
	TPOINT2D        CadOffset;
	TPOINT2D        StageOffset(StageOffsetX, StageOffsetY);
	CString         strComponentName;
	CString         strComponentNameOrg;
	CString         strComponentNameNew;	
	CAOIPanel      *PanelPtr = NULL;
	CAOIBoard      *BoardPtr = NULL;	
	CAOIComponent  *pComponent = NULL;
	CAOIComponent  *NewComponentPtr = NULL;
	CMapCoordinate *MapCTSPtr = NULL;		
	CMapCoordinate *MapSTCPtr = NULL;			
	std::vector<CAOIComponent*> NewAgentList;
	std::vector<CAOIComponent*> OldMasterList;
	std::vector<CAOIComponent*> ProjectComponentPtrMap=m_ProjectComponentPtrList;
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	const size_t CloneCount = CloneComponentList.size();
	if ( 0 == CloneCount ) { return true; }
	const size_t BoardCount=GetProjectBoardCount();	
	std::vector<std::map<CString, size_t>> BoardExistComponentNameMapList(BoardCount);
	std::vector<std::map<CString, size_t>> BoardExistComponentNameMaxIdxMapList(BoardCount);
	const size_t BoardExistComponentNameMapCount=BoardExistComponentNameMapList.size();	
	const size_t BoardExistComponentNameMaxIdxMapCount=BoardExistComponentNameMaxIdxMapList.size();	

	//this->SelectProjectAllComponents(false);	
	for ( i=0; i<CloneCount; i++ )
	{
		pComponent = CloneComponentList[i];
		if ( NULL == pComponent ) { continue; }
		pComponent->SetComponentSelected(false);
		if ( pComponent->CheckComponentIsMaster() == true )
		{	OldMasterList.push_back(pComponent);	}	
		MapCTSPtr = NULL;
		MapSTCPtr = NULL;
		PanelPtr = pComponent->GetComponentPanelPtr();
		BoardPtr = pComponent->GetComponentBoardPtr();		
		ComponentIndex = pComponent->GetComponentIndex_Project();
		if ( NULL == PanelPtr ) { continue; }
		if ( NULL == BoardPtr ) { continue; }
		if ( NULL == MapCTSPtr )
		{	MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);	}
		if ( NULL == MapSTCPtr )
		{	MapSTCPtr = BoardPtr->GetBoardMapSTCPtr(DistrictID);	}		
		if ( NULL == MapCTSPtr )
		{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);	}
		if ( NULL == MapSTCPtr )
		{	MapSTCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID);	}
		BoardIndex = BoardPtr->GetBoardIndex_Project();
		std::map<CString, size_t> &ExistComponentNameMap=BoardExistComponentNameMapList[BoardIndex];
		std::map<CString, size_t> &ExistComponentNameMaxIdxMap=BoardExistComponentNameMaxIdxMapList[BoardIndex];

		CadPosOld = pComponent->GetComponentCadPos();
		StagePosOld.x = pComponent->GetComponentStagePosX();
		StagePosOld.y = pComponent->GetComponentStagePosY();
		if ( pComponent->CheckComponentIsAgent() == true )
		{
			strComponentNameOrg = pComponent->GetComponentName();
			strComponentNameNew = pComponent->GetComponentName();			
		}
		else
		{
			strComponentNameOrg = pComponent->GetComponentName();
			if ( JetAPI::ExtractComponentBaseName(strComponentNameOrg, strComponentName) == false ) 
			{	strComponentName = strComponentNameOrg; }
			
			size_t MaxIdx=0;
			size_t FreeIdx=0;			
			std::map<CString, size_t>::iterator iter=ExistComponentNameMap.find(strComponentName);
			std::map<CString, size_t>::iterator MaxIdxiter=ExistComponentNameMaxIdxMap.find(strComponentName);
			if ( iter!=ExistComponentNameMap.end() )			
			{	FreeIdx = iter->second+1;	}

			if ( MaxIdxiter==ExistComponentNameMaxIdxMap.end() )
			{	BoardPtr->GetBoardComponentMaxFreeNameIdx(strComponentName, MaxIdx, FreeIdx);	}
			else
			{	
				MaxIdx=MaxIdxiter->second;	
				if ( MaxIdx >= FreeIdx )
				{	FreeIdx = BoardPtr->GetBoardComponentFreeNameIdx(strComponentName);	}
			}

			if ( 0 == FreeIdx )
			{	strComponentNameNew = strComponentName;	}
			else
			{	strComponentNameNew.Format(_T("%s_%d"), strComponentName, FreeIdx); }							

			if ( iter!=ExistComponentNameMap.end() )
			{	iter->second = FreeIdx;	}
			else
			{	ExistComponentNameMap[strComponentName]=FreeIdx;	}			

			if ( MaxIdx < FreeIdx )
			{	MaxIdx = FreeIdx;	}
			if ( MaxIdxiter!=ExistComponentNameMaxIdxMap.end() )
			{	MaxIdxiter->second = MaxIdx;	}
			else
			{	ExistComponentNameMaxIdxMap[strComponentName]=MaxIdx;	}
		}
		strComponentNameNew.MakeUpper();
		NewComponentPtr = AddProjectComponentPtr(pComponent, true);
		if ( NULL == NewComponentPtr ) { continue; }
		if ( NewComponentPtr->CheckComponentIsAgent() == true )
		{	NewAgentList.push_back(NewComponentPtr);	}
		NewComponentPtr->ResetComponentMaster();
		ProjectComponentPtrMap[ComponentIndex] = NewComponentPtr;

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
		NewComponentPtr->SetComponentName(strComponentNameNew);
		NewComponentPtr->SetComponentSelected(true);
		NewComponentPtr->MoveComponentPos(CadOffset.x, CadOffset.y, MapCTSPtr);
		NewComponentPtr->ResetComponentResultStatistic();
		NewComponentPtr->InitComponentInspection();		
		NewComponentPtr->RotateComponent(RotateAngle, CadPosNew.x, CadPosNew.y, MapCTSPtr);

		BoardPtr->SetBoardModified(true);
		PanelPtr->SetPanelModified(true);		
	}	
	
	//重新連結新的本尊與代理零件
	ReLinkProjectComponentMasterAgentList(OldMasterList, ProjectComponentPtrMap);	

	//移除沒有連結到的Agent	
	DeleteProjectComponentAgentInvalid(NewAgentList);

	const size_t PanelCount = GetProjectPanelCount();	
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelModified() == false ) { continue; }
		PanelPtr->LayoutPanelBoardListRegion();
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::PasteProjectComponentListToOtherBoards(std::vector<CAOIComponent*> &CloneComponentList)//貼上專案零件至其餘單板上
{	
	CString        ComponentName;	
	CString        OrgComponentName;
	size_t         i=0, j=0, k=0;
	size_t         u=0, v=0, w=0;
	size_t         ComponentIndex=0;
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
	CAOIComponent *ComponentPtr = NULL;	
	CAOIComponent *OrgComponentPtr = NULL;		
	CAOIComponent *RefComponentPtr = NULL;	
	CAOIComponent *RefOrgComponentPtr = NULL;	
	BOARD_ORIENTATION_MODE BoardOrientMode;
	BOARD_ORIENTATION_MODE RefBoardOrientMode;
	std::vector<CAOIComponent*> NewAgentList;
	std::vector<CAOIComponent*> OldMasterList;
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	const size_t   SelComponentCount = CloneComponentList.size();
	const size_t   PanelCount = GetProjectPanelCount();
	const size_t   BoardCount = GetProjectBoardCount();
	if ( 0 == SelComponentCount ) { return true; }

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
		BoardPtr->SetBoardComponentPtrTempList(m_ProjectComponentPtrList);
	}

	for ( i=0; i<SelComponentCount; i++ )
	{
		RefComponentPtr = CloneComponentList[i];
		if ( NULL == RefComponentPtr ) { continue; }
		RefPanelPtr = RefComponentPtr->GetComponentPanelPtr();
		RefBoardPtr = RefComponentPtr->GetComponentBoardPtr();
		if ( NULL==RefPanelPtr || NULL==RefBoardPtr ) { continue; }

		ComponentName = RefComponentPtr->GetComponentName();
		ComponentIndex = RefComponentPtr->GetComponentIndex_Project();
		PanelMapCTSPtr = RefPanelPtr->GetPanelMapCTSPtr(DistrictID);
		PanelBoardCount = RefPanelPtr->GetPanelBoardCount();		
		RefBoardOrientMode = RefBoardPtr->GetBoardOrientationMode();
		RefBoardComponentCount = RefBoardPtr->GetBoardComponentCount();
		if ( RefComponentPtr->CheckComponentIsMaster() == true )
		{	OldMasterList.push_back(RefComponentPtr);	}
		for ( j=0; j<PanelBoardCount; j++ )
		{
			BoardPtr = RefPanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }
			if ( RefBoardPtr == BoardPtr ) { continue; }

			MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);
			if ( NULL == MapCTSPtr ) 
			{	MapCTSPtr = RefPanelPtr->GetPanelMapCTSPtr(DistrictID);	}
			if ( NULL == MapCTSPtr ) { continue; }
			if ( RefComponentPtr->CheckComponentIsAgent() == false )
			{
				ComponentPtr = BoardPtr->GetBoardComponentPtrByName(ComponentName);
				if ( NULL != ComponentPtr )
				{
					BoardPtr->SetBoardComponentPtrTempPtr(ComponentIndex, ComponentPtr);
					continue;
				}
			}
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
				for ( w=0; w<SelComponentCount; w++ )
				{
					if ( RefOrgComponentPtr == CloneComponentList[w] ) //是否在選取中的零件
					{	break; }
				}
				if ( SelComponentCount != w ) { continue; }
				OrgComponentName = RefOrgComponentPtr->GetComponentName();
				OrgComponentPtr = BoardPtr->GetBoardComponentPtrByName(OrgComponentName);
				if ( NULL != OrgComponentPtr )
				{	break; }
			}
			if ( RefBoardComponentCount == u ) { continue; }			
			ComponentPtr = RefComponentPtr->CloneComponentObj();
			if ( NULL == ComponentPtr ) { continue; }
			if ( ComponentPtr->CheckComponentIsAgent() == true )
			{	NewAgentList.push_back(ComponentPtr);	}
			ComponentPtr->ResetComponentMaster();
			BoardPtr->SetBoardComponentPtrTempPtr(ComponentIndex, ComponentPtr);

			CadPosX = ComponentPtr->GetComponentCadPosX();
			CadPosY = ComponentPtr->GetComponentCadPosY();
			StagePosX = ComponentPtr->GetComponentStagePosX();
			StagePosY = ComponentPtr->GetComponentStagePosY();
			OrgCadPosX=OrgComponentPtr->GetComponentCadPosX();
			OrgCadPosY=OrgComponentPtr->GetComponentCadPosY();
			RefOrgCadPosX=RefOrgComponentPtr->GetComponentCadPosX();
			RefOrgCadPosY=RefOrgComponentPtr->GetComponentCadPosY();			
			CadOffsetX2 = CadOffsetX = CadPosX-RefOrgCadPosX;
			CadOffsetY2 = CadOffsetY = CadPosY-RefOrgCadPosY;

			ProjectPtr->AddProjectComponentPtr(ComponentPtr, false);
			RefPanelPtr->AddPanelComponentPtr(ComponentPtr);
			RefPanelPtr->SetPanelModified(true);
			BoardPtr->AddBoardComponentPtr(ComponentPtr);			
			BoardPtr->SetBoardModified(true);
			if ( BoardOrientMode!=RefBoardOrientMode )
			{
				JetAPI::CalcBoardOrientationOffset(RefBoardOrientMode, BoardOrientMode, BoardAngle, BoardMirrorXAxis, BoardMirrorYAxis);
				ComponentPtr->RotateComponent(BoardAngle, CadPosX, CadPosY, NULL);
				if ( true == BoardMirrorXAxis )//對X軸鏡射, 變更Y值
				{	ComponentPtr->MirrorYComponent(CadPosY, NULL);	}
				if ( true == BoardMirrorYAxis )//對Y軸鏡射, 變更X值
				{	ComponentPtr->MirrorXComponent(CadPosX, NULL);	}

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
			ComponentPtr->SetComponentCadPosX(NewCadPosX);
			ComponentPtr->SetComponentCadPosY(NewCadPosY);
			ComponentPtr->SetComponentOrgCadPosX(NewCadPosX);
			ComponentPtr->SetComponentOrgCadPosY(NewCadPosY);
			ComponentPtr->SetComponentStagePosX(NewStagePosX);
			ComponentPtr->SetComponentStagePosY(NewStagePosY);
			ComponentPtr->CalcComponentCadCornerPos();
			ComponentPtr->LayoutComponentStageCornerPos();
			ComponentPtr->UpdateComponentParamToModel(false);
		}
	}

	//重新連結新的本尊與代理零件	
	const size_t OldMasterCount=OldMasterList.size();
	for ( i=0; i<OldMasterCount; i++ )
	{
		RefComponentPtr=OldMasterList[i];
		if ( NULL == RefComponentPtr ) { continue; }
		RefPanelPtr = RefComponentPtr->GetComponentPanelPtr();
		RefBoardPtr = RefComponentPtr->GetComponentBoardPtr();
		if ( NULL==RefPanelPtr || NULL==RefBoardPtr ) { continue; }		
		PanelBoardCount = RefPanelPtr->GetPanelBoardCount();		
		for ( j=0; j<PanelBoardCount; j++ )
		{
			BoardPtr = RefPanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }
			if ( RefBoardPtr == BoardPtr ) { continue; }		
			ReLinkProjectComponentMasterAgent(RefComponentPtr, BoardPtr->GetBoardComponentPtrTempList());
		}
	}

	//移除沒有連結到的Agent	
	DeleteProjectComponentAgentInvalid(NewAgentList);

	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->ClearBoardComponentPtrTempList();
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
bool CAOIProject::PasteProjectComponentListToOtherPanels(std::vector<CAOIComponent*> &CloneComponentList)//貼上專案零件至其餘整板上
{
	CString        ComponentName;	
	CString        OrgComponentName;
	size_t         i=0, j=0, k=0;
	size_t         u=0, v=0, w=0;
	size_t         ComponentIndex=0;
	size_t         PanelBoardCount=0;
	size_t         BoardComponentCount=0;
	size_t         RefBoardComponentCount=0;
	unsigned int   RefBoardIndex_Panel=0;
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
	CAOIComponent *ComponentPtr = NULL;	
	CAOIComponent *OrgComponentPtr = NULL;		
	CAOIComponent *RefComponentPtr = NULL;	
	CAOIComponent *RefOrgComponentPtr = NULL;	
	BOARD_ORIENTATION_MODE BoardOrientMode;
	BOARD_ORIENTATION_MODE RefBoardOrientMode;
	std::vector<CAOIComponent*> NewAgentList;
	std::vector<CAOIComponent*> OldMasterList;
	DISTRICT_ID    DistrictID = GetProjectActDistrictID();
	const size_t   SelComponentCount = CloneComponentList.size();
	const size_t   PanelCount = GetProjectPanelCount();
	const size_t   BoardCount = GetProjectBoardCount();
	if ( 0 == SelComponentCount ) { return true; }

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
		BoardPtr->SetBoardComponentPtrTempList(m_ProjectComponentPtrList);
	}

	for ( i=0; i<SelComponentCount; i++ )
	{
		RefComponentPtr = CloneComponentList[i];
		if ( NULL == RefComponentPtr ) { continue; }
		RefPanelPtr = RefComponentPtr->GetComponentPanelPtr();
		RefBoardPtr = RefComponentPtr->GetComponentBoardPtr();
		if ( NULL==RefPanelPtr || NULL==RefBoardPtr ) { continue; }

		ComponentName = RefComponentPtr->GetComponentName();
		ComponentIndex = RefComponentPtr->GetComponentIndex_Project();
		PanelMapCTSPtr = RefPanelPtr->GetPanelMapCTSPtr(DistrictID);
		PanelBoardCount = RefPanelPtr->GetPanelBoardCount();		
		RefBoardOrientMode = RefBoardPtr->GetBoardOrientationMode();
		RefBoardIndex_Panel = RefBoardPtr->GetBoardIndex_Panel();
		RefBoardComponentCount = RefBoardPtr->GetBoardComponentCount();
		if ( RefComponentPtr->CheckComponentIsMaster() == true )
		{	OldMasterList.push_back(RefComponentPtr); }
		for ( j=0; j<PanelCount; j++ )
		{
			PanelPtr = GetProjectPanelPtr(j, false);
			if ( NULL == PanelPtr ) { continue; }
			if ( PanelPtr == RefPanelPtr ) { continue; }

			BoardPtr = PanelPtr->GetPanelBoardPtr(RefBoardIndex_Panel, true);
			if ( NULL == BoardPtr ) { continue; }
			if ( RefBoardPtr == BoardPtr ) { continue; }

			MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);
			if ( NULL == MapCTSPtr ) 
			{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);	}
			if ( NULL == MapCTSPtr ) { continue; }

			if ( RefComponentPtr->CheckComponentIsAgent() == false )
			{
				ComponentPtr = BoardPtr->GetBoardComponentPtrByName(ComponentName);
				if ( NULL != ComponentPtr )
				{
					BoardPtr->SetBoardComponentPtrTempPtr(ComponentIndex, ComponentPtr);
					continue;
				}
			}
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
				for ( w=0; w<SelComponentCount; w++ )
				{
					if ( RefOrgComponentPtr == CloneComponentList[w] ) //是否在選取中的零件
					{	break; }
				}
				if ( SelComponentCount != w ) { continue; }
				OrgComponentName = RefOrgComponentPtr->GetComponentName();
				OrgComponentPtr = BoardPtr->GetBoardComponentPtrByName(OrgComponentName);
				if ( NULL != OrgComponentPtr )
				{	break; }
			}
			if ( RefBoardComponentCount == u ) { continue; }			
			ComponentPtr = RefComponentPtr->CloneComponentObj();
			if ( NULL == ComponentPtr ) { continue; }
			if ( ComponentPtr->CheckComponentIsAgent() == true )
			{	NewAgentList.push_back(ComponentPtr);	}
			ComponentPtr->ResetComponentMaster();
			BoardPtr->SetBoardComponentPtrTempPtr(ComponentIndex, ComponentPtr);

			CadPosX = ComponentPtr->GetComponentCadPosX();
			CadPosY = ComponentPtr->GetComponentCadPosY();
			StagePosX = ComponentPtr->GetComponentStagePosX();
			StagePosY = ComponentPtr->GetComponentStagePosY();
			OrgCadPosX=OrgComponentPtr->GetComponentCadPosX();
			OrgCadPosY=OrgComponentPtr->GetComponentCadPosY();
			RefOrgCadPosX=RefOrgComponentPtr->GetComponentCadPosX();
			RefOrgCadPosY=RefOrgComponentPtr->GetComponentCadPosY();			
			CadOffsetX2 = CadOffsetX = CadPosX-RefOrgCadPosX;
			CadOffsetY2 = CadOffsetY = CadPosY-RefOrgCadPosY;

			ProjectPtr->AddProjectComponentPtr(ComponentPtr, false);
			PanelPtr->AddPanelComponentPtr(ComponentPtr);
			PanelPtr->SetPanelModified(true);
			BoardPtr->AddBoardComponentPtr(ComponentPtr);
			BoardPtr->SetBoardModified(true);
			
			if ( BoardOrientMode!=RefBoardOrientMode )
			{
				JetAPI::CalcBoardOrientationOffset(RefBoardOrientMode, BoardOrientMode, BoardAngle, BoardMirrorXAxis, BoardMirrorYAxis);
				ComponentPtr->RotateComponent(BoardAngle, CadPosX, CadPosY, NULL);
				if ( true == BoardMirrorXAxis )//對X軸鏡射, 變更Y值
				{	ComponentPtr->MirrorYComponent(CadPosY, NULL);	}
				if ( true == BoardMirrorYAxis )//對Y軸鏡射, 變更X值
				{	ComponentPtr->MirrorXComponent(CadPosX, NULL);	}

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
			ComponentPtr->SetComponentCadPosX(NewCadPosX);
			ComponentPtr->SetComponentCadPosY(NewCadPosY);
			ComponentPtr->SetComponentOrgCadPosX(NewCadPosX);
			ComponentPtr->SetComponentOrgCadPosY(NewCadPosY);
			ComponentPtr->SetComponentStagePosX(NewStagePosX);
			ComponentPtr->SetComponentStagePosY(NewStagePosY);
			ComponentPtr->CalcComponentCadCornerPos();
			ComponentPtr->LayoutComponentStageCornerPos();
			ComponentPtr->UpdateComponentParamToModel(false);
		}
	}

	//重新連結新的本尊與代理零件	
	const size_t OldMasterCount=OldMasterList.size();
	for ( i=0; i<OldMasterCount; i++ )
	{
		RefComponentPtr=OldMasterList[i];		
		if ( NULL == RefComponentPtr ) { continue; }
		RefPanelPtr = RefComponentPtr->GetComponentPanelPtr();
		RefBoardPtr = RefComponentPtr->GetComponentBoardPtr();
		if ( NULL==RefPanelPtr || NULL==RefBoardPtr ) { continue; }
		RefBoardIndex_Panel = RefBoardPtr->GetBoardIndex_Panel();		
		for ( j=0; j<PanelCount; j++ )
		{
			PanelPtr = GetProjectPanelPtr(j, false);
			if ( NULL == PanelPtr ) { continue; }
			if ( PanelPtr == RefPanelPtr ) { continue; }

			BoardPtr = PanelPtr->GetPanelBoardPtr(RefBoardIndex_Panel, true);
			if ( NULL == BoardPtr ) { continue; }
			if ( RefBoardPtr == BoardPtr ) { continue; }
			ReLinkProjectComponentMasterAgent(RefComponentPtr, BoardPtr->GetBoardComponentPtrTempList());
		}
	}

	//移除沒有連結到的Agent	
	DeleteProjectComponentAgentInvalid(NewAgentList);	

	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->ClearBoardComponentPtrTempList();
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
bool CAOIProject::ArrayPasteProjectComponentList(std::vector<CAOIComponent*> &CloneComponentList, const std::vector<TPOINT2D> &PosList, const std::vector<POINT> &IdxList, int NameMode, bool ChangeSelName)//陣列貼上專案零件
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }

	size_t         i=0, j=0, k=0;
	size_t         ComponentIndex=0;
	int            IndexX=0, IndexY=0;
	double         OffsetX=0, OffsetY=0;
	CString        strComponentName;
	CString        strComponentNameOld;
	CString        strComponentNameNew;	
	CAOIPanel      *PanelPtr = NULL;
	CAOIBoard      *BoardPtr = NULL;	
	CAOIComponent  *pComponent = NULL;
	CAOIComponent  *NewComponentPtr = NULL;
	CMapCoordinate *MapCTSPtr = NULL;	
	std::vector<int>  NamePaddingList;
	std::vector<CAOIComponent*> NewAgentList;
	std::vector<CAOIComponent*> OldMasterList;
	std::vector<CAOIComponent*> ProjectComponentPtrMap=m_ProjectComponentPtrList;
	DISTRICT_ID       DistrictID = GetProjectActDistrictID();
	const size_t IdxCount = IdxList.size();
	const size_t PosCount = PosList.size();
	const size_t CloneCount = CloneComponentList.size();	
	const size_t BoardCount = ProjectPtr->GetProjectBoardCount();
	const int ComponentName_RowFirst = ARRAY_PASTE_NAME_ROW_FIRST;//先列數再行數
	const int ComponentName_ColFirst = ARRAY_PASTE_NAME_COL_FIRST;//先行數再列數	
	if ( 0 == CloneCount ) { return true; }

	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr =ProjectPtr->GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->SetBoardTempInt(BoardPtr->GetBoardComponentCount());
	}

	//this->SelectProjectAllComponents(false);
	for ( i=0; i<CloneCount; i++ )
	{	NamePaddingList.push_back(0);	}

	for ( k=0; k<PosCount; k++ )
	{
		IndexX = -1;
		IndexY = -1;
		OldMasterList.clear();
		OffsetX = PosList[k].x;
		OffsetY = PosList[k].y;
		if ( IdxCount == PosCount )
		{
			IndexX = IdxList[k].x;
			IndexY = IdxList[k].y;
		}
		for ( i=0; i<CloneCount; i++ )
		{
			pComponent = CloneComponentList[i];
			if ( NULL == pComponent ) { continue; }
			MapCTSPtr = NULL;
			PanelPtr = pComponent->GetComponentPanelPtr();
			BoardPtr = pComponent->GetComponentBoardPtr();
			ComponentIndex = pComponent->GetComponentIndex_Project();
			if ( pComponent->CheckComponentIsMaster() == true )
			{	OldMasterList.push_back(pComponent); }
			ProjectComponentPtrMap[ComponentIndex] = pComponent;

			if ( NULL == PanelPtr ) { continue; }
			if ( NULL == BoardPtr ) { continue; }
			if ( NULL == MapCTSPtr )
			{	MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID); }
			if ( NULL == MapCTSPtr )
			{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID); }
			if ( NULL == MapCTSPtr ) { continue; }
			if ( pComponent->CheckComponentIsAgent() == true )
			{
				strComponentName = pComponent->GetComponentName();
				strComponentNameNew = pComponent->GetComponentName();	
			}
			else
			{
				size_t BoardComponentCount_Before=BoardPtr->GetBoardTempInt();
				if ( ComponentName_RowFirst==NameMode || ComponentName_ColFirst==NameMode )
				{					
					strComponentName = pComponent->GetComponentName();
					if ( ComponentName_RowFirst == NameMode )
					{	strComponentNameNew.Format(_T("%s-%d-%d"), strComponentName, (IndexY+1), (IndexX+1));	}
					if ( ComponentName_ColFirst == NameMode )
					{	strComponentNameNew.Format(_T("%s-%d-%d"), strComponentName, (IndexX+1), (IndexY+1));	}
					j = 0;
					while ( true )
					{						
						if ( BoardPtr->ChceckBoardComponentNameExist(strComponentNameNew, BoardComponentCount_Before) == false )
						{	break; }
						CString strTempName = strComponentNameNew;
						strComponentNameNew.Format(_T("%s_%d"), strTempName, j+1);
						j ++;
					};
				}				
				else
				{	
					strComponentNameOld = pComponent->GetComponentName();
					if ( JetAPI::ExtractComponentBaseName(strComponentNameOld, strComponentName) == false )
					{	strComponentName = strComponentNameOld; }			
					j = NamePaddingList[i];
					while ( true )
					{
						if ( j == 0 ) 
						{	strComponentNameNew = strComponentName;	}
						else
						{	strComponentNameNew.Format(_T("%s_%d"), strComponentName, j);	}
						if ( BoardPtr->ChceckBoardComponentNameExist(strComponentNameNew, BoardComponentCount_Before) == false )
						{	break; }
						j ++;
					};
					NamePaddingList[i] = j+1;
				}
			}
			strComponentNameNew.MakeUpper();
			NewComponentPtr = this->AddProjectComponentPtr(pComponent, true);
			if ( NULL == NewComponentPtr ) { continue; }
			if ( NewComponentPtr->CheckComponentIsAgent() == true )
			{	NewAgentList.push_back(NewComponentPtr); }
			NewComponentPtr->ResetComponentMaster();
			ProjectComponentPtrMap[ComponentIndex] = NewComponentPtr;

			NewComponentPtr->SetComponentName(strComponentNameNew);
			NewComponentPtr->SetComponentSelected(true);
			NewComponentPtr->SetComponentColIndex(IndexX);
			NewComponentPtr->SetComponentRowIndex(IndexY);
			NewComponentPtr->MoveComponentPos(OffsetX, OffsetY, MapCTSPtr);
			NewComponentPtr->ResetComponentResultStatistic();
			NewComponentPtr->InitComponentInspection();
			BoardPtr->SetBoardModified(true);
			PanelPtr->SetPanelModified(true);		
		}	

		//重新連結新的本尊與代理零件
		ReLinkProjectComponentMasterAgentList(OldMasterList, ProjectComponentPtrMap);		
	}

	//移除沒有連結到的Agent	
	DeleteProjectComponentAgentInvalid(NewAgentList);

	//變更原本選到零件的名稱
	IndexX = 0;
	IndexY = 0;
	if ( ComponentName_RowFirst==NameMode || ComponentName_ColFirst==NameMode )
	{	ChangeSelName = ChangeSelName;	}
	else
	{	ChangeSelName = false; }				
	for ( i=0; i<CloneCount; i++ )
	{
		pComponent = CloneComponentList[i];
		if ( NULL == pComponent ) { continue; }
		pComponent->SetComponentColIndex(IndexX);
		pComponent->SetComponentRowIndex(IndexY);
		if ( false == ChangeSelName ) { continue; }
		if ( pComponent->CheckComponentIsAgent() == true )
		{	continue; }
		BoardPtr = pComponent->GetComponentBoardPtr();
		if ( NULL == BoardPtr ) { continue; }
		j = 0;
		strComponentName = pComponent->GetComponentName();
		size_t BoardComponentCount_Before=BoardPtr->GetBoardTempInt();
		strComponentNameNew.Format(_T("%s-%d-%d"), strComponentName, 1, 1);				
		while ( true )
		{						
			if ( BoardPtr->ChceckBoardComponentNameExist(strComponentNameNew, BoardComponentCount_Before) == false )
			{	break; }
			CString strTempName = strComponentNameNew;
			strComponentNameNew.Format(_T("%s_%d"), strTempName, j+1);
			j ++;
		};
		pComponent->ChangeComponentName(strComponentNameNew);				
	}	

	const size_t PanelCount = GetProjectPanelCount();	
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelModified() == false ) { continue; }
		PanelPtr->LayoutPanelBoardListRegion();
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SwitchProjectComponentBypassed()//切換專案零件不檢測模式
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return true; }
	CAOIComponent *ActComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ActComponentPtr )
	{	return true; }

	std::vector<CAOIComponent*> SelComponentList;
	ProjectPtr->SelectProjectComponentsMasterAgent();
	ProjectPtr->GetProjectComponentSelected(SelComponentList);
	const size_t SelCount =  SelComponentList.size();		
	if ( 0 == SelCount ) { return true; }	

	size_t i=0;
	CAOIComponent *ComponentPtr = NULL;
	bool Bypass =  ActComponentPtr->GetComponentBypassed();
	if ( true == Bypass ) { Bypass = false; }
	else { Bypass = true; }
	for ( i=0; i<SelCount; i++ )
	{
		ComponentPtr = (SelComponentList[i]);
		if ( NULL == ComponentPtr ) { continue; }		
		ComponentPtr->SetComponentBypassed(Bypass);
		ComponentPtr->UpdateComponentBypassed();
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SwitchProjectComponentBypass3D()//切換專案零件3D不檢測模式
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return true; }
	CAOIComponent *ActComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ActComponentPtr )
	{	return true; }

	std::vector<CAOIComponent*> SelComponentList;

	ProjectPtr->GetProjectComponentSelected(SelComponentList);
	const size_t SelCount =  SelComponentList.size();	
	if ( 0 == SelCount ) { return true; }	

	size_t i=0;
	CAOIComponent      *ComponentPtr = NULL;	
	bool Bypass3D = ActComponentPtr->GetComponentBypass3D();
	if ( true == Bypass3D ) { Bypass3D = false; }
	else { Bypass3D = true; }
	for ( i=0; i<SelCount; i++ )
	{
		ComponentPtr = (SelComponentList[i]);
		if ( NULL == ComponentPtr ) { continue; }		
		ComponentPtr->SetComponentBypass3D(Bypass3D);		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SwitchProjectComponentXBoardUnit()//切換專案零件報廢件模式 
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return true; }
	CAOIComponent *ActComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ActComponentPtr )
	{	return true; }
	std::vector<CAOIComponent*> SelComponentList;
	ProjectPtr->SelectProjectComponentsMasterAgent();
	ProjectPtr->GetProjectComponentSelected(SelComponentList);
	const size_t SelCount =  SelComponentList.size();	
	if ( 0 == SelCount ) { return true; }	

	size_t i=0;
	CAOIComponent      *ComponentPtr = NULL;		
	bool XBoardUnit = ActComponentPtr->GetComponentXBoardUnit();
	if ( true == XBoardUnit ) { XBoardUnit = false; }
	else { XBoardUnit = true; }
	for ( i=0; i<SelCount; i++ )
	{
		ComponentPtr = (SelComponentList[i]);
		if ( NULL == ComponentPtr ) { continue; }		
		ComponentPtr->SetComponentXBoardUnit(XBoardUnit);		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SwitchProjectComponentModelIsolated()//切換專案零件模組隔離
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return true; }
	CAOIComponent *ActComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ActComponentPtr )
	{	return true; }
	std::vector<CAOIComponent*> SelComponentList;

	ProjectPtr->GetProjectComponentSelected(SelComponentList);
	const size_t SelCount =  SelComponentList.size();	
	if ( 0 == SelCount ) { return true; }	

	size_t i=0;	
	CString             ModelName_C;	
	CString             ModelName_M;	
	CString             ModelFolder;
	CString             ComponentName;
	CString             ComponentFolder;	
	CString             FullComponentName;
	CString             LibraryFolder;
	CString             PartLibraryFolder;
	MODEL_TYPE          ModelType;
	CAOIModel          *ModelPtr_C = NULL;
	CAOIModel          *ModelPtr_M = NULL;
	CAOIComponent      *ComponentPtr = NULL;		

	bool BeforIsolated = false;
	bool ModelIsolated = ActComponentPtr->GetComponentModelIsolated();
	if ( true == ModelIsolated ) { ModelIsolated = false; }
	else { ModelIsolated = true; }

	LibraryFolder = GetProjectLibraryFolder();
	PartLibraryFolder = GetProjectPartLibraryFolder();
	::CreateDirectory(PartLibraryFolder, NULL);
	for ( i=0; i<SelCount; i++ )
	{
		ComponentPtr = (SelComponentList[i]);
		if ( NULL == ComponentPtr ) { continue; }		

		BeforIsolated = ComponentPtr->GetComponentModelIsolated();
		if ( BeforIsolated == ModelIsolated ) { continue; }
		if ( true == ModelIsolated )
		{
			if ( ComponentPtr->CheckComponentIsAgent() == true )
			{	continue; }
		}

		ModelPtr_C = ComponentPtr->GetComponentModelPtr();
		ModelType = ModelPtr_C->GetModelType();
		if ( MODEL_TYPE_NULL == ModelType ) { continue; }
	#ifdef _DEBUG
		ComponentName = ComponentPtr->GetComponentName();
	#endif//_DEBUG		
		
		ModelName_C = ComponentPtr->GetComponentModelName();
		FullComponentName = ComponentPtr->GetComponentFullName();
		ComponentFolder.Format(_T("%s\\%s"), PartLibraryFolder, FullComponentName);
		if ( true == ModelIsolated )
		{	
			//Model -> Component			
			ModelFolder = ModelPtr_C->GetModelFolderModel();

			::CreateDirectory(ComponentFolder, NULL);
			if ( JetAPI::CopyFolderAToFolderB(ModelFolder, ComponentFolder, false, false, _T(""), -1, -1) == false )
			{
				JetAPI::RemoveFolder(ComponentFolder);
				continue; 
			}
			ModelPtr_C->SetupkModelModifiedDateTime();
			ModelPtr_C->SetModelNeedSaveFiles(true);
			ModelPtr_C->SetModelBKImageNeedToGrab(true);
			ModelPtr_C->SetModelFolderComponent(ComponentFolder);			
			ComponentPtr->SetComponentModelIsolated(ModelIsolated);
			ModelPtr_C->AssignModelFolder();			
		}
		else
		{	
			//Component -> Model			
			if ( ComponentFolder.GetLength() != 0 )
			{	JetAPI::RemoveFolder(ComponentFolder);	}
			ModelPtr_C->SetModelFolderComponent(_T(""));	

			if ( ModelName_M != ModelName_C )
			{
				ModelName_M = ModelName_C;
				ModelPtr_M = GetProjectModelPtrByModelName(ModelName_C);				
			}
			ComponentPtr->SetComponentModelIsolated(ModelIsolated);
			if ( NULL == ModelPtr_M )
			{	ModelPtr_C->ClearModelAllObjList();		}
			else
			{	ComponentPtr->UpdateComponentModelFromLibrary(ModelPtr_M);	}
		}		
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SyncProjectCompoenntSelectedToModel()//同步化零件選取至模組內
{	
	size_t         i=0;
	CAOIModel     *ModelPtr;
	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = GetProjectComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		if ( ComponentPtr->GetComponentSelected() == true )
		{	continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }		
		ModelPtr->GetModelBodyBox().SetBoxActived(false);
		ModelPtr->GetModelBodyBox().SetBoxSelected(false);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::UpdateProjectComponentModelIsolatedFolder()//更新專案零件模組隔離資料夾
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }
	
	size_t         i=0;	
	LPCTSTR        ComponentName;	
	unsigned int   PanelIndex=0;
	unsigned int   BoardIndex=0;
	CString        TempName;	
	CString        ModelNameNew;
	CString        ModelNameOld;
	CString        ModelFolderOld;
	CString        ModelFolderTmp;
	CString        ModelFolderNew;	
	CString        ModelFolderName;
	CString        PartLibraryFolder;
	bool           bModelIsolated=false;
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	std::vector<CString> RemoveFolderList;
	const size_t ComponentCount = ProjectPtr->GetProjectComponentCount_Inline();

	PartLibraryFolder = ProjectPtr->GetProjectPartLibraryFolder();

	//先全部轉成暫存資料夾
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentTempIndex(FN_DISABLE);
		bModelIsolated = ComponentPtr->GetComponentModelIsolated();
		if ( false == bModelIsolated ) { continue; }

		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		
		ModelNameNew = ComponentPtr->GetComponentFullName();
		ModelFolderOld = ModelPtr->GetModelFolderComponent();
		ModelFolderNew.Format(_T("%s\\%s"), PartLibraryFolder, ModelNameNew);
		if ( ModelFolderNew.CompareNoCase(ModelFolderOld) == 0 )
		{	continue; }		
		
		TempName.Format(_T("%s#Temp"), ModelNameNew);				
		ModelFolderTmp.Format(_T("%s\\%s"), PartLibraryFolder, TempName);
		::MoveFile(ModelFolderOld, ModelFolderTmp);		
		ModelPtr->SetModelFolderComponent(ModelFolderTmp);
		ComponentPtr->SetComponentTempIndex(FN_ENABLE);
		RemoveFolderList.push_back(ModelFolderOld);				
	}

	//刪除所有舊的零件模組資料夾
	const size_t RemoveFolderCount = RemoveFolderList.size();
	for ( i=0; i<RemoveFolderCount; i++ )
	{
		ModelFolderOld = RemoveFolderList[i];		
		JetAPI::ClearFolder(ModelFolderOld);
	}

	//再全部轉成零件模組資料夾
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentTempIndex() != FN_ENABLE ) { continue; }		
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }

		ModelFolderTmp = ModelPtr->GetModelFolderComponent();
		ModelNameNew = ComponentPtr->GetComponentFullName();
		ModelFolderNew.Format(_T("%s\\%s"), PartLibraryFolder, ModelNameNew);
		::MoveFile(ModelFolderTmp, ModelFolderNew);
		ModelPtr->SetModelFolderComponent(ModelFolderNew);
		ModelPtr->AssignModelFolder();		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentNeedToCalculate(bool val)//設定專案零件需要去檢測	
{
	size_t         i=0;
	bool           bNeedToCalc=val;	
	CAOIComponent *ComponentPtr = NULL;
	COMPONENT_TYPE ComponentType=COMPONENT_TYPE_NORMAL;
	const size_t ComponentCount = GetProjectComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentBypassed() == true ) 
		{	
			ComponentPtr->SetComponentNeedToCalculate(false);	
			continue;
		}
		ComponentType = ComponentPtr->GetComponentType();
		if ( COMPONENT_TYPE_TEMPORARY == ComponentType )
		{	ComponentPtr->SetComponentNeedToCalculate(false);	}		
		else if ( COMPONENT_TYPE_SCRATCH == ComponentType )
		{	ComponentPtr->SetComponentNeedToCalculate(false);	}
		else if ( COMPONENT_TYPE_DROP_OUT == ComponentType )
		{	ComponentPtr->SetComponentNeedToCalculate(false);	}
		else if ( COMPONENT_TYPE_FULL_MAP == ComponentType )
		{	ComponentPtr->SetComponentNeedToCalculate(true);	}	
		else if ( COMPONENT_TYPE_SPECIAL == ComponentType )
		{	ComponentPtr->SetComponentNeedToCalculate(false);	}
		else
		{
			if ( ComponentPtr->CheckComponentModelEnabled() == false )
			{
				ComponentPtr->SetComponentNeedToCalculate(false);	
				continue;
			}
		}
		ComponentPtr->SetComponentNeedToCalculate(bNeedToCalc);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RestoreProjectComponentNeedToCalculate()//還原專案零件需要去檢測	
{
	size_t         i=0;
	bool           bNeedToCalc=true;
	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = GetProjectComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		bNeedToCalc = ComponentPtr->GetComponentNeedToCalculateBackup();
		ComponentPtr->SetComponentNeedToCalculate(bNeedToCalc);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::BackupProjectComponentNeedToCalculate()//備份專案零件需要去檢測
{
	size_t         i=0;
	bool           bNeedToCalc=true;
	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = GetProjectComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		bNeedToCalc = ComponentPtr->GetComponentNeedToCalculate();
		ComponentPtr->SetComponentNeedToCalculateBackup(bNeedToCalc);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SortProjectComponentList()//排序專案零件列表
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t         i=0;
	unsigned int   index=0;
	CString        str;
	CSortObj       SortObj;
	CSortObj      *SortPtr=NULL;
	CAOIPanel     *PanelPtr = NULL;
	CAOIBoard     *BoardPtr = NULL;
	std::vector<CSortObj> SortList;
	CAOIComponent *ComponentPtr = NULL;
	const size_t   PanelCount = ProjectPtr->GetProjectPanelCount();
	const size_t   BoardCount = ProjectPtr->GetProjectBoardCount();
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount_Inline();

	SortObj.SetSortMode(SORT_BY_TXT);
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }

		str = ComponentPtr->GetComponentName();

		SortObj.SetID(i);
		SortObj.SetPtr(ComponentPtr);
		SortObj.SetValueStr(str);	
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());

	index=0;
	const size_t SortCount = SortList.size();
	ProjectPtr->m_ProjectComponentPtrList.clear();
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = ProjectPtr->GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		PanelPtr->RemovePanelAllComponents();
	}
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = ProjectPtr->GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->RemoveBoardAllComponents();
	}
	for ( i=0; i<SortCount; i++ )
	{
		SortPtr = &(SortList[i]);
		ComponentPtr = (CAOIComponent*)(SortPtr->GetPtr());
		ComponentPtr->SetComponentIndex_Project(index);
		ProjectPtr->AddProjectComponentPtr_Inline(ComponentPtr);

		BoardPtr = ComponentPtr->GetComponentBoardPtr();
		if ( NULL != BoardPtr ) 
		{	BoardPtr->AddBoardComponentPtr(ComponentPtr); }	

		PanelPtr = ComponentPtr->GetComponentPanelPtr();
		if ( NULL != PanelPtr ) 
		{	PanelPtr->AddPanelComponentPtr(ComponentPtr); }

		index ++;
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RestoreProjectComponentCadPosBySelected()//恢復專案零件選取到的Cad座標
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;
	DISTRICT_ID     DistrictID;
	CAOIPanel      *PanelPtr = NULL;
	CAOIBoard      *BoardPtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;	
	CMapCoordinate *MapCTSPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		MapCTSPtr = NULL;		
		PanelPtr = ComponentPtr->GetComponentPanelPtr();
		BoardPtr = ComponentPtr->GetComponentBoardPtr();
		DistrictID = ComponentPtr->GetComponentDistrictID();
		if ( NULL==MapCTSPtr && NULL!=BoardPtr ) 
		{	MapCTSPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);	}
		if ( NULL==MapCTSPtr && NULL!=PanelPtr ) 
		{	MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);	}
		ComponentPtr->RestoreComponentCadPos(MapCTSPtr);
	}

	LayoutProjectRegion();	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedCadPosByResult(bool UsePadResult)//設定專案零件選取到的Cad座標-結果座標
{
	const int OpenMPCnt = GetProjectEditOpenMpCount();
	const int PanelCount = (int)(GetProjectPanelCount());
	const int BoardCount = (int)(GetProjectBoardCount());
	const int ComponentCount = (int)(GetProjectComponentCount());	
	for ( int i=0; i<PanelCount; i++ )
	{
		CAOIPanel *PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		PanelPtr->SetPanelTempInt(FN_DISABLE);		
	}
	for ( int i=0; i<BoardCount; i++ )
	{
		CAOIBoard *BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->SetBoardTempInt(FN_DISABLE);		
	}

#ifdef OPEN_MP_USE
	#pragma omp parallel for num_threads(OpenMPCnt)
#endif//OPEN_MP_USE	
	for ( int i=0; i<ComponentCount; i++ )
	{
		if ( i >= ComponentCount ) { continue; }
		CAOIComponent *ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentTempInt(FN_DISABLE);
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }

		CAOIPanel *PanelPtr = ComponentPtr->GetComponentPanelPtr();		
		CAOIBoard *BoardPtr = ComponentPtr->GetComponentBoardPtr();
		if ( NULL == PanelPtr ) { continue; }
		if ( NULL == BoardPtr ) { continue; }

		double CadOffsetX = 0;
		double CadOffsetY = 0;
		double AfterCadPosX=0;
		double AfterCadPosY=0;
		CMapCoordinate MapCTS, MapSTC;		
		DISTRICT_ID DistrictID=ComponentPtr->GetComponentDistrictID();		
		const double StagePosX = ComponentPtr->GetComponentStagePosX();
		const double StagePosY = ComponentPtr->GetComponentStagePosY();		
		const double BeforeCadPosX = ComponentPtr->GetComponentCadPosX();
		const double BeforeCadPosY = ComponentPtr->GetComponentCadPosY();		
		const double StageResultX = ComponentPtr->GetComponentStageResultX();
		const double StageResultY = ComponentPtr->GetComponentStageResultY();		

		PanelPtr->SetPanelTempInt(FN_ENABLE);
		BoardPtr->SetBoardTempInt(FN_ENABLE);
		ComponentPtr->SetComponentTempInt(FN_ENABLE);		
		if ( false==BoardPtr->GetBoardMapEnable() || 0==BoardPtr->GetBoardFdCount() )
		{
			PanelPtr->GetPanelMapSTC(DistrictID, MapSTC);
			PanelPtr->GetPanelMapCTS(DistrictID, MapCTS);
		}
		else
		{
			BoardPtr->GetBoardMapSTC(DistrictID, MapSTC);
			BoardPtr->GetBoardMapCTS(DistrictID, MapCTS);
		}
		if ( true == UsePadResult )
		{
			CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
			if ( NULL != ModelPtr )
			{	ModelPtr->CalcModelPadResultOffsetCad(CadOffsetX, CadOffsetY);	}			
		}
		else
		{
			MapSTC.Map2D(StageResultX, StageResultY, AfterCadPosX, AfterCadPosY);
			CadOffsetX = AfterCadPosX-BeforeCadPosX;
			CadOffsetY = AfterCadPosY-BeforeCadPosY;
		}
		ComponentPtr->MoveComponentPos(CadOffsetX, CadOffsetY, &MapCTS);
		ComponentPtr->SetComponentOrgCadPos(ComponentPtr->GetComponentCadPos());
		ComponentPtr->InitComponentInspection();		
	}

	for ( int i=0; i<BoardCount; i++ )
	{
		CAOIBoard *BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		if ( FN_DISABLE == BoardPtr->GetBoardTempInt() ) { continue; }
		BoardPtr->LayoutBoardRegion();
	}

	for ( int i=0; i<PanelCount; i++ )
	{
		CAOIPanel *PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		if ( FN_DISABLE == PanelPtr->GetPanelTempInt() ) { continue; }
		PanelPtr->LayoutPanelRegion();
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedSaveWndList(bool bSave)//設定專案零件是否儲存檢測框列表
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentSaveWndList(bSave);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedEnableAlarm(bool bAlarm)//設定專案零件是否停機警報
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentEnableAlarm(bAlarm);
		ComponentPtr->SetComponentTotalNGCountEnable(bAlarm);
		ComponentPtr->SetComponentContinueNGCountEnable(bAlarm);	
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedEnableAlarmOnAOI(bool bAlarm)//設定專案零件是否機台警報	
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentDefectAlarmEnableOnAOI(bAlarm);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedEnableAlarmOnARS(bool bAlarm)//設定專案零件是否維修站顯示
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentDefectAlarmEnableOnARS(bAlarm);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedEnableDefectCountOnARS(bool bAlarm)//設定專案零件是否計數維修站顯示
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentDefectCountEnableOnARS(bAlarm);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedDefectAlarmAOI(DEFECT_PARAM_FROM_MODE From, const CWndDefectItem &Item)//設定專案零件停機警報參數
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentDefectItemAlarmAOI(Item);
		ComponentPtr->SetComponentDefectAlarmFromModeAOI(From);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedDefectAlarmARS(DEFECT_PARAM_FROM_MODE From, const CWndDefectItem &Item)//設定專案零件停機警報參數
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentDefectItemAlarmARS(Item);
		ComponentPtr->SetComponentDefectAlarmFromModeARS(From);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedSaveReportARS(bool bSave)//設定專案零件是否儲存報告-維修站
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentSaveReport_ARS(bSave);		
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedEnableSelfField(bool bEnabled)//設定專案零件是否啟用專屬區域
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIField      *FieldPtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		

	SelectProjectAllInspectionField(false);
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		ComponentPtr->SetComponentSelfFieldEnabled(bEnabled);
		if ( false == bEnabled ) 
		{
			FieldPtr = ComponentPtr->GetComponentSelfFieldPtr();
			if ( NULL != FieldPtr )
			{
				FieldPtr->SetFieldSelected(true);
				ComponentPtr->SetComponentFieldPtr(NULL);
				ComponentPtr->SetComponentFieldIndex(-1);
				ComponentPtr->SetComponentSelfFieldPtr(NULL);
			}			
		}		
	}
	DestroyProjectInspectionFieldSelected();//刪除選取到的區域	
	return true;
}
//---------------------------------------------------------------------------------//	
bool CAOIProject::SetProjectComponentSelectedSpaceBasePlaneParam(const TBasePlaneParam& Param, double RefAngle)//設定專案零件空間基準面參數
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;
	double          Angle=0.0;
	int             AngleLabel=0;
	CAOIComponent  *ComponentPtr = NULL;	
	TBasePlaneParam Param_000, Param_090, Param_180, Param_270;
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		

	Param_000=Param;
	RotateProjectBasePlaneParam(-RefAngle, Param_000);
	Param_090 = Param_180 = Param_270 = Param_000;
	RotateProjectBasePlaneParam(90, Param_090);
	RotateProjectBasePlaneParam(180, Param_180);
	RotateProjectBasePlaneParam(270, Param_270);

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		Angle = ComponentPtr->GetComponentAngle();
		AngleLabel = JetAPI::GetAngleLabel(Angle);
		switch ( AngleLabel )
		{
		case 0:	  ComponentPtr->SetComponentSpaceBasePlaneParam(Param_000);	break;
		case 90:  ComponentPtr->SetComponentSpaceBasePlaneParam(Param_090);	break;
		case 180: ComponentPtr->SetComponentSpaceBasePlaneParam(Param_180);	break;
		case 270: ComponentPtr->SetComponentSpaceBasePlaneParam(Param_270);	break;
		default:
			ComponentPtr->SetComponentSpaceBasePlaneParam(Param);
			break;
		}
	}		
	return true;
}
//---------------------------------------------------------------------------------//	
bool CAOIProject::SetProjectComponentSelectedSpaceNoiseFilterParam(const TNoiseFilterParam& Param)//設定專案零件空間雜訊過濾參數
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentSpaceNoiseFilterParam(Param);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::EnableProjectComponentSelectedMaskFunc_Base(bool bEnable)//啟用專案零件遮罩函式
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentMaskEnable_Base(bEnable);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedMaskColorIndex_Base(int ColorIndex)//設定專案零件遮罩畫面
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }
	CColorGroup    *ColorGroupPtr = ProjectPtr->GetProjectColorGroupPtr(ColorIndex, true);
	if ( NULL == ColorGroupPtr ) { return false; }

	size_t          i = 0;		
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();
	const unsigned int FrameIndex = ColorGroupPtr->GetColorGroupFrameIndex();
	const unsigned int FrameUniqueID = ColorGroupPtr->GetColorGroupFrameUniqueID();

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentMaskColorGroupLinkIndex(ColorIndex);
		ComponentPtr->SetComponentMaskFrameIndex_Base(FrameIndex);
		ComponentPtr->SetComponentMaskFrameUniqueID_Base(FrameUniqueID);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedMaskFrameUniqueID_Base(unsigned int FrameIndex, unsigned int FrameUniqueID)//設定專案零件遮罩畫面
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentMaskFrameIndex_Base(FrameIndex);
		ComponentPtr->SetComponentMaskFrameUniqueID_Base(FrameUniqueID);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedMaskExtendSize_Body(double ExtendW, double ExtendH)//設定專案零件遮罩外擴尺寸-本體
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentMaskExtendW_Body(ExtendW);
		ComponentPtr->SetComponentMaskExtendH_Body(ExtendH);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedMaskExtendSize_Land(double ExtendW, double ExtendH)//設定專案零件遮罩外擴尺寸-焊盤
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentMaskExtendW_Land(ExtendW);
		ComponentPtr->SetComponentMaskExtendH_Land(ExtendH);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedGroupID(int GroupID)//設定專案零件群組編號
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i = 0;		
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentGroupID(GroupID);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedGroupOrg(bool bOrg)//設定專案零件群組圓點
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t          i=0, j=0;
	int             GroupID=0;
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();
	if ( true == bOrg )
	{
		//清除原來群組的ORG設定
		size_t GroupIDCount=0;
		std::vector<int> GroupIDList;
		for ( i=0; i<ComponentCount; i++ )
		{
			ComponentPtr = GetProjectComponentPtr_Inline(i);
			if ( NULL == ComponentPtr ) { continue; }
			if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
			if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
			GroupID = ComponentPtr->GetComponentGroupID();
			GroupIDCount = GroupIDList.size();
			for ( j=0; j<GroupIDCount; j++ )
			{
				if ( GroupID != GroupIDList[j] ) { continue; }
				break;
			}
			if ( j != GroupIDCount ) { continue; }
			GroupIDList.push_back(GroupID);
		}
		GroupIDCount = GroupIDList.size();
		for ( j=0; j<GroupIDCount; j++ )
		{
			GroupID = GroupIDList[j];			
			for ( i=0; i<ComponentCount; i++ )
			{
				ComponentPtr = GetProjectComponentPtr_Inline(i);
				if ( NULL == ComponentPtr ) { continue; }				
				if ( ComponentPtr->GetComponentGroupID() != GroupID ) { continue; }				
				ComponentPtr->SetComponentGroupOrg(false);
			}
		}
	}
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentGroupOrg(bOrg);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedLocalBasePlaneID(int BasePlaneID)//設定專案零件局部基準面編號
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }
	if ( BasePlaneID < 0 ) { return false; }

	size_t          i = 0;		
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }		
		ComponentPtr->SetComponentLocalBasePlaneID(BasePlaneID);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentSelectedDataModelParam(int Param)//設定專案零件資料物件參數
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }	

	size_t          i = 0;		
	bool            bEnabled=false;
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	if ( Param > FN_DISABLE )
	{	bEnabled = true; }
	else
	{	bEnabled = false; }
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }	
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		ComponentPtr->SetComponentDataModelEnabled(bEnabled);
		if ( true == bEnabled )
		{	ComponentPtr->SetComponentDataModelLevelID(Param);	}
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::AddProjectComponentAgentByBomNodeList(const std::vector<TComponentNode> &BomNodeList)//加入專案零件代理人-載入BOM列表
{
	CAOIProject *ProjectPtr=this;
	if ( NULL == ProjectPtr ) { return false; }	
	
	CAOIComponent *ComponentPtr=NULL;	
	std::vector<CString> ChangeList;
	const size_t BomNodeCount=BomNodeList.size();
	const size_t ComponentCount2=ProjectPtr->GetProjectComponentCount();		
	
	CString str;
	CString ModelName;
	CString PartNumber;	
	CString PartNumber_C;	
	CString ModelNameM;
	CString ModelFolder;	
	CString ComponentName;
	CString ComponentName_C;
	CAOIPanel *PanelPtr=NULL;
	CAOIBoard *BoardPtr=NULL;
	CAOIModel *ModelPtr=NULL;	
	const int FnEnable=FN_ENABLE;
	const int FnDisable=FN_DISABLE;
	std::set<CString> AgentModelList;
	std::vector<CAOIModel*> DeleteModelList;	
	CString LibraryFolder=ProjectPtr->GetProjectLibraryFolder();	
	const size_t ComponentCount=ProjectPtr->GetProjectComponentCount();
	
	for ( size_t i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentTempInt(FnDisable);
	}

	//Check Primary PartNumber
	for ( size_t i=0; i<BomNodeCount; i++ )
	{
		const TComponentNode &BomNodeRef=BomNodeList[i];
		if ( false == BomNodeRef.bMainPartNumber ) { continue; }		
		PartNumber = BomNodeRef.sPartNumber;
		ComponentName = BomNodeRef.sComponentName;
		for ( size_t j=0; j<ComponentCount; j++ )
		{
			ComponentPtr = ProjectPtr->GetProjectComponentPtr(j, false);
			if ( NULL == ComponentPtr ) { continue; }
			if ( FnEnable == ComponentPtr->GetComponentTempInt() ) { continue; }
			if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }

			ComponentName_C = ComponentPtr->GetComponentName();
			if ( ComponentName.CompareNoCase(ComponentName_C) != 0 ) { continue; }
			PartNumber_C = ComponentPtr->GetComponentPartNumber();
			CAOIComponent *AgentPtr=ComponentPtr->GetComponentAgentPtrByPartNumber(PartNumber);
			if ( AgentPtr == ComponentPtr )
			{
				AgentPtr->SetComponentTempInt(FnEnable);
				continue;
			}

			if ( NULL != AgentPtr )
			{
				AgentPtr->SetComponentTempInt(FnEnable);
				ComponentPtr->ChangeComponentMaster(AgentPtr);
			}
			else
			{	
				if ( ComponentPtr->GetComponentModelIsolated() == true )
				{
					ModelFolder = ModelPtr->GetModelFolderComponent();
					JetAPI::ClearFolder(ModelFolder);
				}
				ModelName = ComponentPtr->GetComponentModelName();

				ComponentPtr->SetComponentTempInt(FnEnable);
				ComponentPtr->SetComponentModelIsolated(false);
				ComponentPtr->SetComponentModelName(PartNumber);				

				ModelPtr = ComponentPtr->GetComponentModelPtr();				
				ModelPtr->ClearModelAllObjList();
				ModelPtr->SetModelIsolated(false);
				ModelPtr->ReleaseModelUniFrameList();
				ModelPtr->SetModelType(MODEL_TYPE_NULL);
				ModelPtr->SetModelName(PartNumber);
				ModelFolder.Format(_T("%s\\%s"), LibraryFolder, PartNumber);
				ModelPtr->SetModelFolderModel(ModelFolder);
				ModelPtr->AssignModelFolder();

				AgentModelList.insert(PartNumber);
				
				LogOperCtrl.SaveLogComponentOperate(ComponentPtr, _T("Change"), _T("PartNumber"), PartNumber);
				str.Format(_T("Change Component[%s] PartNumber [%s] to [%s]"), ComponentPtr->GetComponentFullName(), PartNumber_C, PartNumber);
				ChangeList.push_back(str);
			}						
		}
	}

	//Add Agent PartNumber By Component Name
	for ( size_t i=0; i<BomNodeCount; i++ )
	{		
		const TComponentNode &BomNodeRef=BomNodeList[i];
		if ( true == BomNodeRef.bMainPartNumber ) { continue; }		
		PartNumber = BomNodeRef.sPartNumber;
		AgentModelList.insert(PartNumber);
		for ( size_t j=0; j<ComponentCount; j++ )
		{
			ComponentPtr = ProjectPtr->GetProjectComponentPtr(j, false);
			if ( NULL == ComponentPtr ) { continue; }
			if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }

			ComponentName = ComponentPtr->GetComponentName();
			if ( BomNodeRef.sComponentName.CompareNoCase(ComponentName) != 0 ) { continue; }			
			CAOIComponent *AgentPtr = ComponentPtr->GetComponentAgentPtrByPartNumber(PartNumber);
			if ( NULL != AgentPtr )
			{
				AgentPtr->SetComponentTempInt(FnEnable);
				continue;
			}			

			AgentPtr=ComponentPtr->CloneComponentObj();
			if ( NULL == AgentPtr ) { continue; }
			AgentPtr->SetComponentBeAgent(PartNumber, LibraryFolder);

			PanelPtr = ComponentPtr->GetComponentPanelPtr();
			BoardPtr = ComponentPtr->GetComponentBoardPtr();			
			ComponentPtr->AddComponentAgentPtr(AgentPtr);
			ProjectPtr->AddProjectComponentPtr(AgentPtr, false);
			if ( NULL != BoardPtr )
			{	BoardPtr->AddBoardComponentPtr(AgentPtr); }
			if ( NULL != PanelPtr )
			{	PanelPtr->AddPanelComponentPtr(AgentPtr); }

			LogOperCtrl.SaveLogComponentOperate(AgentPtr, _T("Add"), _T("PartNumber"), PartNumber);
			str.Format(_T("Add Component[%s] PartNumber[%s]"), AgentPtr->GetComponentFullName(), PartNumber);
			ChangeList.push_back(str);
		}
	}

	//Remove Component Not Set
	ProjectPtr->SelectProjectAllComponents(false);
	for ( size_t j=0; j<ComponentCount; j++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(j, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( FnEnable == ComponentPtr->GetComponentTempInt() ) { continue; }
		ComponentPtr->SetComponentSelected(true);
		PartNumber_C = ComponentPtr->GetComponentPartNumber();

		LogOperCtrl.SaveLogComponentOperate(ComponentPtr, _T("Deleted"), _T("PartNumber"), PartNumber_C);
		str.Format(_T("Delete Component[%s] PartNumber[%s]"), ComponentPtr->GetComponentFullName(), PartNumber_C);
		ChangeList.push_back(str);
	}
	ProjectPtr->DeleteProjectComponentSelected();

	//Apply Component To Agent
	const size_t AgentModelCount=AgentModelList.size();
	for ( auto iter=AgentModelList.begin(); iter!=AgentModelList.end(); ++iter )
	{
		ModelName=*iter;
		ModelPtr = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
		if ( NULL == ModelPtr ) { continue; }
		ProjectPtr->SelectProjectAllComponents(false);
		ProjectPtr->SelectProjectComponentsByModelName(ModelName, true);		
		ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtr);
	}

	ProjectPtr->ResetProjectActiveIndex();
	ProjectPtr->SelectProjectAllComponents(false);	

	const size_t ChangeCount=ChangeList.size();
	if ( ChangeCount > 0 )
	{
		FILE *pfile=NULL;
		CString filename;
		TCHAR TMode[32] = _T("");
		_tcscpy(TMode, _T("w+"));
		JetAPI::ModifyOpenFileMode_Write(TMode);
		filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("LoadBomResult.TXT"));		
		pfile = ::_tfopen(filename, TMode);
		if ( NULL != pfile )
		{
			for ( size_t i=0; i<ChangeCount; i++ )
			{	::_ftprintf(pfile, _T("%s\n"), ChangeList[i]);	}
			::fclose(pfile); pfile = NULL;
			::Sleep(0);
			::ShellExecute(NULL, _T("open"), filename, NULL, NULL, SW_SHOW);
		}	
	}
	return true;
}
//---------------------------------------------------------------------------------//
int CAOIProject::GetCreateProjectComponentForFullProjectMapMode() const//建立專案零件-模式
{
	//return PROJECT_CREATE_FULL_MAP_COMPONENT_V1;
	return PROJECT_CREATE_FULL_MAP_COMPONENT_V2;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectComponentForFullProjectMap()//建立專案零件-全底圖
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }
	CAOIPanel *PanelPtr = ProjectPtr->GetProjectPanelPtr(0, true);
	if ( NULL == PanelPtr ) { return false; }
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
	CMapCoordinate *MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);
	if ( NULL == MapCTSPtr ) { return false; }

	int       i=0, j=0;
	TPOINT2D  MapRes;
	TREGION4D MapCadRgn;
	TREGION4D MapStageRgn;	
	double    FovSizeW=0;
	double    FovSizeH=0;
	int       nComponentX=0;
	int       nComponentY=0;
	
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	AOIDataCollect.GetFovSizeInner(FovSizeW, FovSizeH);
	ProjectPtr->ModifyProjectFieldSize(FovSizeW, FovSizeH);
	ProjectPtr->GetProjectMapInfo(MapRes, MapCadRgn, MapStageRgn);
	double MarginW = 1000;//um
	double MarginH = 1000;//um
	double ComponentSizeW = FovSizeW*0.8;
	double ComponentSizeH = FovSizeH*0.8;
	double MapCadSizeW = MapCadRgn.GetWidth()-MarginW-MarginW;
	double MapCadSizeH = MapCadRgn.GetHeight()-MarginH-MarginH;
	double StartX = MapCadRgn.minX+(FovSizeW/2)+MarginW;//左下腳第1個位置
	double StartY = MapCadRgn.minY+(FovSizeH/2)+MarginH;
	double MapCanSizeInnerW = MapCadSizeW-FovSizeW;//扣掉雙邊Fov的範圍
	double MapCanSizeInnerH = MapCadSizeH-FovSizeH;
	const int CreateFullMapComponentMode = GetCreateProjectComponentForFullProjectMapMode();

	if ( PROJECT_CREATE_FULL_MAP_COMPONENT_V2 == CreateFullMapComponentMode ) 
	{
		MarginW = 0;//um
		MarginH = 0;//um		
		MapCadSizeW = MapCadRgn.GetWidth()-MarginW-MarginW;
		MapCadSizeH = MapCadRgn.GetHeight()-MarginH-MarginH;
		StartX = MapCadRgn.minX+(ComponentSizeW/2)+MarginW;//左下腳第1個位置
		StartY = MapCadRgn.minY+(ComponentSizeH/2)+MarginH;
		MapCanSizeInnerW = MapCadSizeW-ComponentSizeW;
		MapCanSizeInnerH = MapCadSizeH-ComponentSizeH;
	}

	if ( MapCanSizeInnerW < 0 )
	{		
		MapCanSizeInnerW = 0; 
		StartX = MapCadRgn.GetCpX();
	}
	if ( MapCanSizeInnerH < 0 ) 
	{ 
		MapCanSizeInnerH = 0; 
		StartY = MapCadRgn.GetCpY();
	}
	nComponentX = (int)(MapCanSizeInnerW/FovSizeW)+1;
	nComponentY = (int)(MapCanSizeInnerH/FovSizeH)+1;
	const double PitchX = MapCanSizeInnerW/nComponentX;
	const double PitchY = MapCanSizeInnerH/nComponentY;

	int    Cnt = 0;		
	double CadPosX = 0;
	double CadPosY = 0;
	double StagePosX=0;
	double StagePosY=0;
	wchar_t ComponentName[128]=L"";
	wchar_t PartNumber[128]=L"FullMap";
	wchar_t ModelName[128]=L"FullMap";	
	
	if ( ProjectPtr->DeleteProjectComponentType(COMPONENT_TYPE_FULL_MAP) == false )
	{	return false; }

	Cnt = 1;
	_wcsupr(ModelName);
	for ( i=0; i<nComponentY+1; i++ )
	{
		for ( j=0; j<nComponentX+1; j++ )
		{
			CadPosX = (j*PitchX)+StartX;
			CadPosY = (i*PitchY)+StartY;
			MapCTSPtr->Map2D(CadPosX, CadPosY, StagePosX, StagePosY);

			ComponentPtr = AOIObjManager.CreateComponentObj();
			if ( NULL ==ComponentPtr )
			{
				m_ErrorString = _T("Error, CreateComponentObj Fault");
				return false;
			}

			if ( Cnt < 10 )
			{	::swprintf(ComponentName, L"%s_0000%d", PartNumber, Cnt); }
			else if ( Cnt < 100 )
			{	::swprintf(ComponentName, L"%s_000%d", PartNumber, Cnt); }
			else if ( Cnt < 100 )
			{	::swprintf(ComponentName, L"%s_00%d", PartNumber, Cnt); }
			else if ( Cnt < 100 )
			{	::swprintf(ComponentName, L"%s_0%d", PartNumber, Cnt); }
			else
			{	::swprintf(ComponentName, L"%s_%d", PartNumber, Cnt); }
			
			ComponentPtr->SetComponentName(ComponentName);
			ComponentPtr->SetComponentModelName(ModelName);
			ComponentPtr->SetComponentPartNumber(PartNumber);
			ComponentPtr->SetComponentType(COMPONENT_TYPE_FULL_MAP);
			ComponentPtr->SetComponentBodySizeW(ComponentSizeW);
			ComponentPtr->SetComponentBodySizeH(ComponentSizeH);
			ComponentPtr->SetComponentRoiSizeW(ComponentSizeW);
			ComponentPtr->SetComponentRoiSizeH(ComponentSizeH);

			ComponentPtr->SetComponentCadPosX(CadPosX);
			ComponentPtr->SetComponentCadPosY(CadPosY);
			
			ComponentPtr->SetComponentStagePosX(StagePosX);
			ComponentPtr->SetComponentStagePosY(StagePosY);
			ComponentPtr->CalcComponentCadCornerPos();
			ComponentPtr->LayoutComponentStageCornerPos();

			ModelPtr = ComponentPtr->GetComponentModelPtr();
			ModelPtr->SetModelType(MODEL_TYPE_OTHERS);
			ComponentPtr->UpdateComponentParamToModel(false);

			ProjectPtr->AddProjectComponentPtr(ComponentPtr, false);
			PanelPtr->AddPanelComponentPtr(ComponentPtr);
			Cnt ++;
		}
	}
	return true;
}
bool CAOIProject::CreateProjectComponentForSpecRegion(TREGION4D SpecRegion)
{
	CAOIProject *ProjectPtr = this;
	if (NULL == ProjectPtr) { return false; }
	CAOIPanel *PanelPtr = ProjectPtr->GetProjectPanelPtr(0, true);
	if (NULL == PanelPtr) { return false; }
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
	CMapCoordinate *MapCTSPtr = PanelPtr->GetPanelMapCTSPtr(DistrictID);
	if (NULL == MapCTSPtr) { return false; }

	int       i = 0, j = 0;
	TPOINT2D  MapRes;
	TPOINT2D StgCornerPos[4];
	TREGION4D MapCadRgn;
	TREGION4D MapStageRgn;
	double    FovSizeW = 0;
	double    FovSizeH = 0;
	int       nComponentX = 0;
	int       nComponentY = 0;

	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	AOIDataCollect.GetFovSizeInner(FovSizeW, FovSizeH);
	ProjectPtr->ModifyProjectFieldSize(FovSizeW, FovSizeH);
	ProjectPtr->GetProjectMapInfo(MapRes, MapCadRgn, MapStageRgn);
	double MarginW = 1000;//um
	double MarginH = 1000;//um
	double ComponentSizeW = FovSizeW*0.8;
	double ComponentSizeH = FovSizeH*0.8;
	double MapCadSizeW = MapCadRgn.GetWidth() - MarginW - MarginW;
	double MapCadSizeH = MapCadRgn.GetHeight() - MarginH - MarginH;
	double StartX = MapCadRgn.minX + (FovSizeW / 2) + MarginW;//左下腳第1個位置
	double StartY = MapCadRgn.minY + (FovSizeH / 2) + MarginH;
	double MapCanSizeInnerW = MapCadSizeW - FovSizeW;//扣掉雙邊Fov的範圍
	double MapCanSizeInnerH = MapCadSizeH - FovSizeH;
	const int CreateFullMapComponentMode = GetCreateProjectComponentForFullProjectMapMode();

	if (PROJECT_CREATE_FULL_MAP_COMPONENT_V2 == CreateFullMapComponentMode)
	{
		MarginW = 0;//um
		MarginH = 0;//um		
		MapCadSizeW = MapCadRgn.GetWidth() - MarginW - MarginW;
		MapCadSizeH = MapCadRgn.GetHeight() - MarginH - MarginH;
		StartX = MapCadRgn.minX + (ComponentSizeW / 2) + MarginW;//左下腳第1個位置
		StartY = MapCadRgn.minY + (ComponentSizeH / 2) + MarginH;
		MapCanSizeInnerW = MapCadSizeW - ComponentSizeW;
		MapCanSizeInnerH = MapCadSizeH - ComponentSizeH;
	}

	if (MapCanSizeInnerW < 0)
	{
		MapCanSizeInnerW = 0;
		StartX = MapCadRgn.GetCpX();
	}
	if (MapCanSizeInnerH < 0)
	{
		MapCanSizeInnerH = 0;
		StartY = MapCadRgn.GetCpY();
	}
	nComponentX = (int)(MapCanSizeInnerW / FovSizeW) + 1;
	nComponentY = (int)(MapCanSizeInnerH / FovSizeH) + 1;
	const double PitchX = MapCanSizeInnerW / nComponentX;
	const double PitchY = MapCanSizeInnerH / nComponentY;

	int    Cnt = 0;
	double CadPosX = 0;
	double CadPosY = 0;
	double StagePosX = 0;
	double StagePosY = 0;
	wchar_t ComponentName[128] = L"";
	wchar_t PartNumber[128] = L"FullMap";
	wchar_t ModelName[128] = L"FullMap";

	if (ProjectPtr->DeleteProjectComponentType(COMPONENT_TYPE_FULL_MAP) == false)
	{
		return false;
	}

	Cnt = 1;
	_wcsupr(ModelName);
	for (i = 0; i<nComponentY + 1; i++)
	{
		for (j = 0; j<nComponentX + 1; j++)
		{
			CadPosX = (j*PitchX) + StartX;
			CadPosY = (i*PitchY) + StartY;
			MapCTSPtr->Map2D(CadPosX, CadPosY, StagePosX, StagePosY);

			ComponentPtr = AOIObjManager.CreateComponentObj();
			if (NULL == ComponentPtr)
			{
				m_ErrorString = _T("Error, CreateComponentObj Fault");
				return false;
			}

			if (Cnt < 10)
			{
				::swprintf(ComponentName, L"%s_0000%d", PartNumber, Cnt);
			}
			else if (Cnt < 100)
			{
				::swprintf(ComponentName, L"%s_000%d", PartNumber, Cnt);
			}
			else if (Cnt < 100)
			{
				::swprintf(ComponentName, L"%s_00%d", PartNumber, Cnt);
			}
			else if (Cnt < 100)
			{
				::swprintf(ComponentName, L"%s_0%d", PartNumber, Cnt);
			}
			else
			{
				::swprintf(ComponentName, L"%s_%d", PartNumber, Cnt);
			}

			ComponentPtr->SetComponentName(ComponentName);
			ComponentPtr->SetComponentModelName(ModelName);
			ComponentPtr->SetComponentPartNumber(PartNumber);
			ComponentPtr->SetComponentType(COMPONENT_TYPE_FULL_MAP);
			ComponentPtr->SetComponentBodySizeW(ComponentSizeW);
			ComponentPtr->SetComponentBodySizeH(ComponentSizeH);
			ComponentPtr->SetComponentRoiSizeW(ComponentSizeW);
			ComponentPtr->SetComponentRoiSizeH(ComponentSizeH);

			ComponentPtr->SetComponentCadPosX(CadPosX);
			ComponentPtr->SetComponentCadPosY(CadPosY);

			ComponentPtr->SetComponentStagePosX(StagePosX);
			ComponentPtr->SetComponentStagePosY(StagePosY);
			ComponentPtr->CalcComponentCadCornerPos();
			ComponentPtr->LayoutComponentStageCornerPos();
			ComponentPtr->GetComponentBodyStageCornerPos(StgCornerPos);
			if (false == JetAPI::CheckPtInRegion(StgCornerPos[0].x, StgCornerPos[0].y, SpecRegion)&&
				false == JetAPI::CheckPtInRegion(StgCornerPos[1].x, StgCornerPos[1].y, SpecRegion)&&
				false == JetAPI::CheckPtInRegion(StgCornerPos[2].x, StgCornerPos[2].y, SpecRegion)&&
				false == JetAPI::CheckPtInRegion(StgCornerPos[3].x, StgCornerPos[3].y, SpecRegion)
				) 
			{
				AOIObjManager.DestroyComponentObj(ComponentPtr);
				continue;
			}
			

			ModelPtr = ComponentPtr->GetComponentModelPtr();
			ModelPtr->SetModelType(MODEL_TYPE_OTHERS);
			ComponentPtr->UpdateComponentParamToModel(false);

			ProjectPtr->AddProjectComponentPtr(ComponentPtr, false);
			PanelPtr->AddPanelComponentPtr(ComponentPtr);
			Cnt++;
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectComponentReadyToInspection()//確認零件準備好檢測
{
	CAOIProject *ProjectPtr = this;
	if ( AOIDataCollect.CheckProjectPtr(ProjectPtr) == false ) { return false; }

	size_t         i=0,j=0;		
	size_t         WndCount=0;
	size_t         ModelCount=0;
	MODEL_TYPE     ModelType;	
	CString        ModelName;	
	CAOIWnd       *WndPtr = NULL;
	CAOIModel     *ModelPtr = NULL;
	CAOIPanel     *PanelPtr = NULL;
	CAOIBoard     *BoardPtr = NULL;	
	CAOIComponent *ComponentPtr = NULL;
	std::vector<CAOIModel*>  CheckModelList; 	
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount_Inline();

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentBypassed() == true ) { continue; }
		if ( ComponentPtr->CheckComponentType_ModelTest() == false ) { continue; }
		
		PanelPtr = ComponentPtr->GetComponentPanelPtr();
		if ( NULL != PanelPtr )
		{
			if ( PanelPtr->GetPanelDeleted() == true ) { continue; }
			if ( PanelPtr->GetPanelBypassed() == true ) { continue; }
		}
		BoardPtr = ComponentPtr->GetComponentBoardPtr();
		if ( NULL != BoardPtr )
		{
			if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
			if ( BoardPtr->GetBoardBypassed() == true ) { continue; }
		}

		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		ModelType = ModelPtr->GetModelType();
		if ( MODEL_TYPE_NULL == ModelType ) { continue; }
		
		if ( ComponentPtr->GetComponentModelIsolated() == true )
		{	CheckModelList.push_back(ModelPtr);	}
		else
		{
			ModelName = ModelPtr->GetModelName();
			ModelCount = CheckModelList.size();
			for ( j=0; j<ModelCount; j++ )
			{
				CAOIModel *CheckModelPtr = CheckModelList[j];
				if ( NULL == CheckModelPtr ) { continue; }
				if ( CheckModelPtr->GetModelIsolated() == true ) { continue; }
				CString CheckModelName=CheckModelPtr->GetModelName();
				if ( ModelName.CompareNoCase(CheckModelName) == 0 ) 
				{	break; }
			}
			if ( j == ModelCount )
			{	CheckModelList.push_back(ModelPtr);	}
		}
	}

	std::vector<CAOIModel*>  BadModelList;		
	std::vector<WND_DEFECT_ID> UnDoneList;
	const size_t CheckModelCount = CheckModelList.size();
	for ( i=0; i<CheckModelCount; i++ )
	{
		ModelPtr = CheckModelList[i];
		if ( NULL == ModelPtr ) { continue; }
		bool bBad = false;
		ModelPtr->SetModelTempInt(0);
		WndCount = ModelPtr->GetModelWndCount();		
		if ( 0 == WndCount )
		{	bBad = true; }
		if ( ModelPtr->CheckModelDefectItemEssential(UnDoneList) == false )
		{	
			bBad = true; 
			if ( UnDoneList.size() > 0 )
			{	ModelPtr->SetModelTempInt(UnDoneList[0]);	}
		}
		if ( false == bBad ) { continue; }
		BadModelList.push_back(ModelPtr);		
	}

	const size_t BadModelCount=BadModelList.size();
	if ( BadModelCount > 0 ) 
	{
		CString      str;
		CString      str2;
		CString      strTemp;		
		CString      strName;		
		const size_t ShowCount = MIN(BadModelCount, 10);
		for ( i=0; i<ShowCount; i++ )
		{				
			ModelPtr = BadModelList[i];			
			if ( NULL == ModelPtr ) { continue; }
			if ( ModelPtr->GetModelIsolated() == false )
			{	ComponentPtr = NULL;	}
			else
			{	ComponentPtr = ModelPtr->GetModelComponentPtr();	}			

			if ( NULL == ComponentPtr )
			{	strName = ModelPtr->GetModelName();	}
			else
			{	strName = ComponentPtr->GetComponentFullName();	}			
			if ( 0 != ModelPtr->GetModelTempInt() )
			{
				strTemp = strName;
				WND_DEFECT_ID WndDefectID=(WND_DEFECT_ID)(ModelPtr->GetModelTempInt());					 
				strName.Format(_T("%s[%s]"), strTemp, AOIDataDefine.GetWndDefectIDText(WndDefectID));
			}
			if ( NULL == ComponentPtr )
			{
				if ( str.GetLength() == 0 ) 
				{	str = strName; }
				else
				{
					strTemp = str;
					str.Format(_T("%s\n%s"), strTemp, strName);
				}
			}
			else
			{
				if ( str2.GetLength() == 0 ) 
				{	str2 = strName; }
				else
				{
					strTemp = str2;
					str2.Format(_T("%s\n%s"), strTemp, strName);
				}
			}
		}
		if ( str.GetLength() > 0 )
		{
			strTemp = str;
			str = _T("Warning, Model's wnd not be done");		
			str = LoadMultiLanguageString(str, str);
			m_ErrorString.Format(_T("%s\n%s"), str, strTemp);
			return false;
		}
		if ( str2.GetLength() > 0 )
		{
			str = str2;
			strTemp = str;			
			str = _T("Warning, Component's wnd not be done");
			str = LoadMultiLanguageString(str, str);
			m_ErrorString.Format(_T("%s\n%s"), str, strTemp);
			return false;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectComponentBarcodeReadyToInspection()//確認零件條碼準備好檢測	
{
	CAOIProject *ProjectPtr = this;	
	if ( AOIDataCollect.CheckProjectPtr(ProjectPtr) == false ) { return false; }

	size_t         i=0,j=0;		
	size_t         WndCount=0;	
	CString        ModelName;	
	CAOIWnd       *WndPtr = NULL;
	CAOIModel     *ModelPtr = NULL;
	CAOIPanel     *PanelPtr = NULL;
	CAOIBoard     *BoardPtr = NULL;	
	CAOIComponent *ComponentPtr = NULL;
	std::vector<CAOIComponent*> BadComponentList;	
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount_Inline();

	BadComponentList.clear();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentBypassed() == true ) { continue; }
		if ( ComponentPtr->CheckComponentType_ModelTest() == false ) { continue; }
		if ( ComponentPtr->GetComponentXBoardUnit() == false ) { continue; }
		CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		const size_t ModelWndCount=ModelPtr->GetModelWndCount();
		for ( j=0; j<ModelWndCount; j++ )
		{
			WndPtr = ModelPtr->GetModelWndPtr(j, false);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndEnabled() == false ) { continue; }
			ALG_TYPE AlgType = WndPtr->GetWndAlgType();
			if ( ALG_BARCODE_RECOGNIZE != AlgType ) { continue; }
			BadComponentList.push_back(ComponentPtr);
			break;
		}
	}
	const size_t BadComponentCount = BadComponentList.size();
	if ( BadComponentCount > 0 ) 
	{
		CString      str;
		CString      strTemp;		
		CString      strName;		
		const size_t ShowCount = MIN(BadComponentCount, 10);
		for ( i=0; i<ShowCount; i++ )
		{
			ComponentPtr = BadComponentList[i];
			if ( NULL == ComponentPtr ) { continue; }
			strName = ComponentPtr->GetComponentFullName();
			if ( str.GetLength() == 0 ) 
			{	str = strName; }
			else
			{
				strTemp = str;
				str.Format(_T("%s\n%s"), strTemp, strName);
			}
		}
		strTemp = str;
		str = _T("Error, Components can not be XBoard-Unit with barcode-confirm");
		str = LoadMultiLanguageString(str, str);
		m_ErrorString.Format(_T("%s\n%s"), str, strTemp);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::ChangeProjectComponentParam(const std::vector<std::wstring> &NameList, CHANGE_COMPARE_PARAM_MODE ChangeMode, bool bResetAll)//變更零件參數 
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }

	size_t         i=0, j=0;
	std::wstring   Name;
	std::wstring   ComponentName;
	CAOIComponent *ComponentPtr=NULL;
	std::vector<std::wstring> BadNameList;
	const size_t   NameCount=NameList.size();
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount_Inline();

	if ( true == bResetAll )
	{
		ProjectPtr->SelectProjectAllComponents(true);
		if ( CHANGE_COMPARE_PARAM_TEST == ChangeMode )//名單上的才要檢測
		{	ProjectPtr->BypassProjectComponentSelected(true);	}
		if ( CHANGE_COMPARE_PARAM_BYPASS == ChangeMode )//名單上的不要檢測
		{	ProjectPtr->BypassProjectComponentSelected(false);	}
	}
	ProjectPtr->SelectProjectAllComponents(false);	

	int   nRes=0;	
	bool  bMatched=false;
	bool  bBypassed=true;
	if ( CHANGE_COMPARE_PARAM_TEST == ChangeMode )//名單上的才要檢測
	{	bBypassed = false;	}
	if ( CHANGE_COMPARE_PARAM_BYPASS == ChangeMode )//名單上的不要檢測
	{	bBypassed = true;	}
	for ( i=0; i<NameCount; i++ )
	{
		bMatched=false;
		Name = NameList[i];
		for ( j=0; j<ComponentCount; j++ )
		{
			ComponentPtr = ProjectPtr->GetProjectComponentPtr_Inline(j);
			if ( NULL == ComponentPtr ) { continue; }
			if ( ComponentPtr->GetComponentSelected() == true ) { continue; }
			ComponentName = ComponentPtr->GetComponentName();
			nRes = ::wcscmp(ComponentName.c_str(), Name.c_str());
			if ( 0 != nRes ) { continue; }
			bMatched = true;
			ComponentPtr->SetComponentSelected(true);
			ComponentPtr->SetComponentBypassed(bBypassed);			
		}
		if ( false == bMatched )
		{	BadNameList.push_back(Name); } 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CAOIProject::ResetProjectComponentNPM_APC_FF1()//設定專案零件的NPM-APC-FF1參數
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }

	size_t         i=0;		
	CAOIComponent *ComponentPtr=NULL;
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount_Inline();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		ComponentPtr->ResetComponentNPM_APC_FF1();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CAOIProject::ResetProjectComponentNPM_APC_MFB()//設定專案零件的NPM-APC-MFB參數
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }

	size_t         i=0;		
	CAOIComponent *ComponentPtr=NULL;
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount_Inline();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		ComponentPtr->ResetComponentNPM_APC_MFB();				
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CAOIProject::ResetProjectComponentNPM_APC_Param()//復歸專案零件的NPM_APC參數
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }

	size_t         i=0;		
	CAOIComponent *ComponentPtr=NULL;
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount_Inline();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		ComponentPtr->ResetComponentNPM_APC_Param();		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::BuildProjectComponentNPM_APC_List(std::vector<CSortObj> &SortList)//建立專案零件的APC列表
{
	CAOIProject *ProjectPtr = this;

	//Sort By IDNUM
	CSortObj SortObj;
	unsigned int IDNUM=0;
	const size_t ComponentCount=ProjectPtr->GetProjectComponentCount();	
	SortList.resize(ComponentCount);
	SortList.clear();

	SortObj.SetSortMode(SORT_BY_ID);
	for ( size_t i=0; i<ComponentCount; i++ )
	{
		CAOIComponent *ComponentPtr=ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		IDNUM = -1;
		if ( ComponentPtr->CheckComponentNPM_APC_FF1_Done() == true )
		{	IDNUM = ComponentPtr->GetComponentNPM_APC_FF1_IDNUM();	}
		else if ( ComponentPtr->CheckComponentNPM_APC_MFB_Done() == true )
		{	IDNUM = ComponentPtr->GetComponentNPM_APC_MFB_IDNUM();	}
		if ( -1 == IDNUM ) { continue; }
		SortObj.SetID(IDNUM);
		SortObj.SetPtr(ComponentPtr);
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentNPM_APC_FF1(unsigned int IDNUM, int PNUM, LPCTSTR CName, double MffX, double MffY, double MffA)//設定專案零件的NPM-APC-FF1參數
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }

	size_t         i=0;		
	CAOIComponent *ComponentPtr=NULL;
	CString        ComponentName=CName;
	unsigned int   RefBoardIndex=PNUM-1;	
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount_Inline();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		if ( ComponentPtr->CheckComponentNPM_APC_FF1_Done() == true ) { continue; }

		unsigned int BoardIndex=ComponentPtr->GetComponentBoardIndex_Panel();
		if ( RefBoardIndex != BoardIndex ) { continue; }
		if ( ComponentName.CollateNoCase(ComponentPtr->GetComponentName()) != 0 ) { continue; }		
		ComponentPtr->SetComponentNPM_APC_MffX(MffX);
		ComponentPtr->SetComponentNPM_APC_MffY(MffY);
		ComponentPtr->SetComponentNPM_APC_MffA(MffA);
		ComponentPtr->SetComponentNPM_APC_FF1_IDNUM(IDNUM);
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::SetProjectComponentNPM_APC_MFB(unsigned int IDNUM, int PNUM, LPCTSTR CName, double EPosX, double EPosY, double EPosA)//設定專案零件的NPM-APC-FF3參數
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }

	size_t         i=0;		
	CAOIComponent *ComponentPtr=NULL;
	CString        ComponentName=CName;
	unsigned int   RefBoardIndex=PNUM-1;	
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount_Inline();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		if ( ComponentPtr->CheckComponentNPM_APC_MFB_Done() == true ) { continue; }

		unsigned int BoardIndex=ComponentPtr->GetComponentBoardIndex_Panel();
		if ( RefBoardIndex != BoardIndex ) { continue; }
		if ( ComponentName.CollateNoCase(ComponentPtr->GetComponentName()) != 0 ) { continue; }		
		ComponentPtr->SetComponentNPM_APC_EPosX(EPosX);
		ComponentPtr->SetComponentNPM_APC_EPosY(EPosY);
		ComponentPtr->SetComponentNPM_APC_EPosA(EPosA);
		ComponentPtr->SetComponentNPM_APC_MFB_IDNUM(IDNUM);
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectDropOutPartCount() const//取得專案拋件數量
{
	return GetProjectDropOutPartCount_Inline();
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectDropOutPartPtr(size_t index, bool check) const//取得專案拋件指標
{
	if ( check )
	{
		const size_t count = GetProjectDropOutPartCount_Inline();
		if ( index >= count )
		{	return NULL; }
	}
	return CAOIProject::GetProjectDropOutPartPtr_Inline(index);	
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::AddProjectDropOutPartPtr(CAOIComponent *ComponentPtr, bool clone)//增加專案拋件
{
	if ( NULL == ComponentPtr )
	{
		this->m_ErrorString.Format(_T("Error, AddProjectDropOutPartPtr Fault"));
		return NULL;
	}

	CAOIComponent *NewComponentPtr = ComponentPtr;
	const unsigned int ComponentIndex = (unsigned int)(GetProjectDropOutPartCount_Inline());
	if ( true == clone )
	{	
		NewComponentPtr = ComponentPtr->CloneComponentObj();
		if ( NewComponentPtr == NULL ) { return NULL; }		
		NewComponentPtr->SetComponentIndex_Project(ComponentIndex);			
	}	
	else
	{	NewComponentPtr->SetComponentIndex_Project(ComponentIndex);	 }

	NewComponentPtr->SetComponentUniqueID(ComponentIndex);	
	NewComponentPtr->SetComponentProjectPtr(this);	
	AddProjectDropOutPartPtr_Inline(NewComponentPtr);		
	return NewComponentPtr;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ClearProjectAllDropOutParts()//刪除專案多件
{
	size_t i = 0;	
	CAOIComponent *ComponentPtr = NULL;	
	const size_t ComponentCount = GetProjectDropOutPartCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectDropOutPartPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		AOIObjManager.DestroyComponentObj(m_ProjectDropOutPartList[i]);
		ComponentPtr = NULL;		
	}		
	this->m_ProjectDropOutPartList.clear();	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ClearXBoardDropOutParts()
{
	// Alan
	const bool bDropOutEnabled = GetProjectParameter().m_DropOutPartEnable;
	if (false == bDropOutEnabled) { return true; }
	const bool bXboardIgnored = GetProjectParameter().m_DropOutPartXBoardExcluded;
	if (false == bXboardIgnored) { return true; }

	size_t i,j;
	unsigned int BoardID;
	CAOIBoard     *BoardPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const size_t BoardCount = GetProjectBoardCount();
	const size_t ComponentCount = GetProjectDropOutPartCount();
	const size_t FrameUniqueIDCount = GetProjectFrameUniqueIDCount();
	TREGION4D BoardRgnCad = TREGION4D();
	TPOINT2D ComponentCad = TPOINT2D();
	if (0 == ComponentCount) { return true; }
	CString         ComponentFullName;
	CString         ComponentImageName;

	bool bInXboard = false;

	std::vector<CAOIComponent*>    ProjectDropOutPartList = m_ProjectDropOutPartList;//專案拋件指標列表	
	m_ProjectDropOutPartList.clear();

	CString   SpcImageFolder = GetProjectSpcImageFolder();
	if (GetProjectUseLocalFolder()){
		SpcImageFolder = GetProjectSpcImageFolderLocal();
	}

	for (i = 0; i<ProjectDropOutPartList.size(); i++)
	{
		ComponentPtr = ProjectDropOutPartList[i];
		if (NULL == ComponentPtr) { continue; }
		bInXboard = false;
		for (j = 0; j < BoardCount; j++) {
			BoardPtr = GetProjectBoardPtr(j, false);
			if (NULL == BoardPtr) { continue; }
			BoardRgnCad = BoardPtr->GetBoardRgnCad();
			ComponentCad = ComponentPtr->GetComponentCadPos();
			if (false == BoardRgnCad.CheckPtInside(ComponentCad)) { continue; }
#ifdef DROP_DEBUG
			BoardID = BoardPtr->GetBoardIndex_Project();
			if (BoardID == 0) {
				BoardPtr->SetBoardResultID(RESULT_ID_SKIP);
				BoardPtr->SetBoardResultID_Alarm(RESULT_ID_SKIP);
			}
#endif // DROP_DEBUG
			if (RESULT_ID_SKIP != BoardPtr->GetBoardResultID() ||
				RESULT_ID_SKIP != BoardPtr->GetBoardResultID_Alarm()) {
				//BoardPtr->AddBoardComponentPtr(ComponentPtr);
				continue;
			}
			bInXboard = true;
			break;
		}
		if (false == bInXboard) { 
			AddProjectDropOutPartPtr_Inline(ComponentPtr);
			continue; 
		}
		ComponentFullName = ComponentPtr->GetComponentFullName();
		for (j = 0; j<FrameUniqueIDCount; j++)
		{
			ComponentImageName.Format(_T("%s\\%s#%d.JPG"), SpcImageFolder, ComponentFullName, j + 1);
			::DeleteFile(ComponentImageName);
			ComponentImageName.Format(_T("%s\\%s#%d.Z3D"), SpcImageFolder, ComponentFullName, j + 1);
			::DeleteFile(ComponentImageName);
		}
		AOIObjManager.DestroyComponentObj(ComponentPtr);
		ProjectDropOutPartList[i] = NULL;
	}
	return true;
}
//---------------------------------------------------------------------------------//
int CAOIProject::GetProjectTestDropOutPart() const
{
	return m_ProjectParameter.m_DropOutPartEnable; 
}
//-------------------------------------------------------------------------------------//
void CAOIProject::SetProjectTestDropOutPart(int val)
{ 
	m_ProjectParameter.m_DropOutPartEnable = val; 
}
//-------------------------------------------------------------------------------------//
int CAOIProject::GetProjectDropOutPartSaveImage() const//檢測拋件-存圖
{
	return m_ProjectParameter.m_DropOutPartSaveImage;
}
//-------------------------------------------------------------------------------------//
void CAOIProject::SetProjectDropOutPartSaveImage(int val)//檢測拋件-存圖	
{
	m_ProjectParameter.m_DropOutPartSaveImage = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::BuildProjectDropOutComponentList()//合併程專案拋件列表
{	
	size_t          i=0, j=0, k=0;
	size_t          ModelWndCount=0;
	bool            bModifyRgn=false;
	double          BodySizeW=0;
	double          BodySizeH=0;		
	CString         ComponentFullName;
	CString         ComponentImageName;
	double          ComponentCadPosX=0;
	double          ComponentCadPosY=0;
	double          ComponentStagePosX=0;
	double          ComponentStagePosY=0;	
	TREGION4D       ComponentRgn1;
	TREGION4D       ComponentRgn2;
	CAOIWnd        *WndPtr = NULL;
	CAOIModel      *ModelPtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;
	CAOIComponent  *ComponentPtr2 = NULL;		
	DISTRICT_ID     DistrictID = GetProjectActDistrictID();
	CAOIPanel *PanelPtr = GetProjectPanelPtr(0, true);
	if ( NULL == PanelPtr ) { return true; }
	CAOIBoard *BoardPtr = PanelPtr->GetPanelBoardPtr(0, true);
	if ( NULL == BoardPtr ) { return true;	}	
	
	CMapCoordinate *STCMapPtr = BoardPtr->GetBoardMapSTCPtr(DistrictID);
	CMapCoordinate *CTSMapPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);
	std::vector<CAOIComponent*>    ProjectDropOutPartList=m_ProjectDropOutPartList;//專案拋件指標列表	
	
	m_ProjectDropOutPartList.clear();		
	CString   SpcImageFolder = GetProjectSpcImageFolder();	
	const size_t    DropOutCount = ProjectDropOutPartList.size();		
	const size_t    FrameUniqueIDCount = GetProjectFrameUniqueIDCount();

	if ( GetProjectUseLocalFolder() )
	{	SpcImageFolder = GetProjectSpcImageFolderLocal();	}

	for ( i=0; i<DropOutCount; i++ )
	{
		ComponentPtr = ProjectDropOutPartList[i];
		if ( NULL == ComponentPtr ) { continue; }		
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		bModifyRgn=false;
		ComponentPtr->GetComponentBodyStageRegion(ComponentRgn1);
		for ( j=i+1; j<DropOutCount; j++ )
		{
			ComponentPtr2 = ProjectDropOutPartList[j];
			if ( NULL == ComponentPtr2 ) { continue; }
			ComponentPtr2->GetComponentBodyStageRegion(ComponentRgn2);
			if ( ComponentRgn2.minX > ComponentRgn1.maxX ) { continue; }
			if ( ComponentRgn2.minY > ComponentRgn1.maxY ) { continue; }
			if ( ComponentRgn2.maxX < ComponentRgn1.minX ) { continue; }
			if ( ComponentRgn2.maxY < ComponentRgn1.minY ) { continue; }
			bModifyRgn = true;
			//剔除重複的零件
			ComponentFullName = ComponentPtr2->GetComponentFullName();
			for ( k=0; k<FrameUniqueIDCount; k++ )
			{
				ComponentImageName.Format(_T("%s\\%s#%d.JPG"), SpcImageFolder, ComponentFullName, k+1);
				::DeleteFile(ComponentImageName);
				ComponentImageName.Format(_T("%s\\%s#%d.Z3D"), SpcImageFolder, ComponentFullName, k+1);
				::DeleteFile(ComponentImageName);
			}
			AOIObjManager.DestroyComponentObj(ProjectDropOutPartList[j]);
			ProjectDropOutPartList[j] = NULL;
			JetAPI::UnionRegion(ComponentRgn1, ComponentRgn2, ComponentRgn1);
		}	
		if ( true == bModifyRgn )
		{
			BodySizeW = ComponentRgn1.GetWidth();
			BodySizeH = ComponentRgn1.GetHeight();
			ComponentStagePosX = ComponentRgn1.GetCpX();
			ComponentStagePosY = ComponentRgn1.GetCpY();
			
			ComponentCadPosX = ComponentPtr->GetComponentCadPosX();
			ComponentCadPosY = ComponentPtr->GetComponentCadPosY();
			STCMapPtr->Map2D(ComponentStagePosX, ComponentStagePosY, ComponentCadPosX, ComponentCadPosY);

			ComponentPtr->SetComponentBodySizeW(BodySizeW);
			ComponentPtr->SetComponentBodySizeH(BodySizeH);
			ComponentPtr->SetComponentCadPosX(ComponentCadPosX);
			ComponentPtr->SetComponentCadPosY(ComponentCadPosY);
			ComponentPtr->SetComponentStagePosX(ComponentStagePosX);
			ComponentPtr->SetComponentStagePosY(ComponentStagePosY);		
			ComponentPtr->SetComponentRoiSizeW(BodySizeW);
			ComponentPtr->SetComponentRoiSizeH(BodySizeH);
			ComponentPtr->CalcComponentCadCornerPos();
			ComponentPtr->LayoutComponentStageCornerPos();

			ModelPtr = ComponentPtr->GetComponentModelPtr();
			ModelWndCount = ModelPtr->GetModelWndCount();
			for ( j=0; j<ModelWndCount; j++ )
			{
				WndPtr = ModelPtr->GetModelWndPtr(j, false);
				if ( NULL == WndPtr ) { continue; }
				WndPtr->GetWndBox().SetBoxSize(BodySizeW, BodySizeH);				
			}
			ModelPtr->GetModelBodyBox().SetBoxSize(BodySizeW, BodySizeH);
			ModelPtr->SetModelAttachedPosCad(ComponentCadPosX, ComponentCadPosY);
			ModelPtr->SetModelAttachedPosStage(ComponentStagePosX, ComponentStagePosY);
		}	
		AddProjectDropOutPartPtr_Inline(ComponentPtr);		
	}	
	const size_t    DropOutCount2 = m_ProjectDropOutPartList.size();	
	if ( DropOutCount2 != DropOutCount )
	{	i = 3;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::SaveProjectDropOutPartLog(LPCTSTR str)
{	
	CString msg;
	msg.Format(_T("DROP_OUT::%s"), str);
	AOIDataCollect.SaveMovingTimeMsg(msg);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::TestProjectDropOutPart(CAOIField *FieldPtr)
{
	if ( TestProjectDropOutPartFn(FieldPtr) == false )
	{
		SetProjectExceptionCode(AOI_EXCEPTION_PROJECT_TEST_DROPOUT);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::TestProjectDropOutPartFn(CAOIField *FieldPtr)
{
	bool IsOK = true;
	const unsigned int DropOutFrameUniqueID = GetProjectParameter().m_DropOutPartFrameUniqueID;
	if ( FRAME_UNIQUE_ID_DLP == DropOutFrameUniqueID )
	{	IsOK = TestProjectDropOutPartBy3DHeight(FieldPtr);	}
	else
	{	IsOK = TestProjectDropOutPartByMapCompare(FieldPtr);	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::ExecProjectDropOutPartMaskFilter(CAOIField *FieldPtr, const TPOINT2D &MapRes, IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, MASK_PTR MaskPtr, MASK_PTR DstPtr, std::vector<CString> &SaveFileList)//執行拋件遮罩過濾
{
	const char fnName[] = "CAOIProject::ExecProjectDropOutPartMaskFilter";
	if ( NULL == FieldPtr ) { return false; }
	if ( NULL == MaskPtr || NULL == DstPtr )
	{	return false; }

	CString      str;
	MASK_PTR     RawMaskPtr = NULL;
	MASK_PTR     TestMaskPtr = NULL;
	MASK_PTR     TestMaskPtr1 = NULL;
	MASK_PTR     TestMaskPtr2 = NULL;
	IMAGE_SIZE   MaskBitCount = 8;
#ifdef _DEBUG
	bool         bSave=true;	
#endif _DEBUG	
	CString      MainName;
	CString      KeyName=_T("DropOut");	
	CString      Folder = GetProjectDebugFolderDropOut();
	const size_t MaskBufferSize = ImageAPI.CalcBufferSize(MaskStep, MaskH);
	if ( JetMemory.alloc_func(MaskBufferSize, RawMaskPtr, fnName, "RawMaskPtr") == false ||
		 JetMemory.alloc_func(MaskBufferSize, TestMaskPtr, fnName, "TestMaskPtr") == false ||
		 JetMemory.alloc_func(MaskBufferSize, TestMaskPtr1, fnName, "TestMaskPtr1") == false ||
		 JetMemory.alloc_func(MaskBufferSize, TestMaskPtr2, fnName, "TestMaskPtr2") == false  )
	{
		JetMemory.free_func(RawMaskPtr);
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		return false;
	}		
	//影像處理
	
	//膨脹-Dilate, In::MaskPtr, Out:RawMaskPtrL;
	const int DilateSize = GetProjectParameter().m_DropOutPartDilateSize;		
	if ( 0 == DilateSize ) 
	{	::memcpy(RawMaskPtr, MaskPtr, sizeof(MASK_DATA)*MaskBufferSize);	}
	else
	{			
		const int nDilateSize=ImageAPI.GetKernelSize(DilateSize);
		//::memcpy(RawMaskPtr, MaskPtr, sizeof(MASK_DATA)*MaskBufferSize);
		if ( ImageAPI.DilateGrayImage3(MaskW, MaskH, MaskStep, MaskPtr, nDilateSize, 1, RawMaskPtr) == false )
		{	
			JetMemory.free_func(RawMaskPtr);
			JetMemory.free_func(TestMaskPtr);
			JetMemory.free_func(TestMaskPtr1);
			JetMemory.free_func(TestMaskPtr2);
			return false;
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MaskCompareDilateL.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, RawMaskPtr, true);
			SaveFileList.push_back(str);			
		}
	#endif//_DEBUG
	}

	//Open, In::RawMaskPtrL, Out:TestMaskPtr;
	int   MorphMode = 0;
	int   ShapeMode = MORPH_SHAPE_ELLIPSE;//MORPH_SHAPE_RECT, MORPH_SHAPE_CROSS, MORPH_SHAPE_ELLIPSE
	const int NoiseFilterOpen = GetProjectParameter().m_DropOutPartFilterOpenSize;	
	if ( 0 == NoiseFilterOpen ) 
	{	::memcpy(TestMaskPtr, RawMaskPtr, sizeof(MASK_DATA)*MaskBufferSize);	}
	else
	{		
		MorphMode = MORPH_OPEN;		
		const int OpenSizeL=ImageAPI.GetKernelSize(NoiseFilterOpen);
		if ( ImageAPI.MorphGrayImage3(MaskW, MaskH, MaskStep, RawMaskPtr, MorphMode, ShapeMode, OpenSizeL, 1, TestMaskPtr) == false )
		{	
			JetMemory.free_func(RawMaskPtr);			
			JetMemory.free_func(TestMaskPtr);
			JetMemory.free_func(TestMaskPtr1);
			JetMemory.free_func(TestMaskPtr2);
			return false;
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MaskCompareOpenL.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr, true);
			SaveFileList.push_back(str);			
		}
	#endif//_DEBUG
	}

	//Close, In::TestMaskPtr, Out:TestMaskPtr2;
	const int NoiseFilterClose = GetProjectParameter().m_DropOutPartFilterCloseSize;		
	if ( 0 == NoiseFilterClose ) 
	{	::memcpy(TestMaskPtr2, TestMaskPtr, sizeof(MASK_DATA)*MaskBufferSize);	}
	else
	{
		MorphMode = MORPH_CLOSE;
		const int CloseSize = ImageAPI.GetKernelSize(NoiseFilterClose);
		if ( ImageAPI.MorphGrayImage3(MaskW, MaskH, MaskStep, TestMaskPtr, MorphMode, ShapeMode, CloseSize, 1, TestMaskPtr2) == false )	
		{	
			JetMemory.free_func(RawMaskPtr);		
			JetMemory.free_func(TestMaskPtr);
			JetMemory.free_func(TestMaskPtr1);
			JetMemory.free_func(TestMaskPtr2);
			return false;
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MaskCompareClose.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
	}
	
	//剔除外圍的部分, In::TestMaskPtr2, Out:TestMaskPtr2;
	size_t       i=0;
	size_t       MaskIdx=0;
	const bool   ExcludeOutside = false;
	const double FieldStageSizeW_Real = FieldPtr->GetFieldSizeW_Real();
	const double FieldStageSizeH_Real = FieldPtr->GetFieldSizeH_Real();
	const double FieldStageSizeW_Inner = FieldPtr->GetFieldSizeW_Inner();
	const double FieldStageSizeH_Inner = FieldPtr->GetFieldSizeH_Inner();
	if ( true == ExcludeOutside )
	{
		double dMarginW = (FieldStageSizeW_Real-FieldStageSizeW_Inner)/MapRes.x;
		double dMarginH = (FieldStageSizeH_Real-FieldStageSizeH_Inner)/MapRes.y;		
		int MarginW = (int)(dMarginW+0.5);
		int MarginH = (int)(dMarginH+0.5);
		MarginW = MarginW/2;
		MarginH = MarginH/2;
		if ( MarginW>=0 && MarginH>=0 )
		{
			//Top && Bottom
			for ( i=0; i<MarginH; i++ )
			{
				MaskIdx = i*MaskStep;
				::memset(&TestMaskPtr2[MaskIdx], 0x00, sizeof(MASK_DATA)*MaskStep);

				MaskIdx = (MaskH-i-1)*MaskStep;
				::memset(&TestMaskPtr2[MaskIdx], 0x00, sizeof(MASK_DATA)*MaskStep);
			}
			
			//Left & Right
			for ( i=MarginH; i<MaskH-MarginH; i++ )
			{
				//Left
				MaskIdx = i*MaskStep;
				::memset(&TestMaskPtr2[MaskIdx], 0x00, sizeof(MASK_DATA)*MarginW);

				MaskIdx = (i*MaskStep)+MaskW-MarginW;
				::memset(&TestMaskPtr2[MaskIdx], 0x00, sizeof(MASK_DATA)*MarginW);
			}
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MaskExcludeOutside.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
	}

	//Output
	::memcpy(DstPtr, TestMaskPtr2, sizeof(MASK_DATA)*MaskBufferSize);

	JetMemory.free_func(RawMaskPtr);		
	JetMemory.free_func(TestMaskPtr);
	JetMemory.free_func(TestMaskPtr1);
	JetMemory.free_func(TestMaskPtr2);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectDropOutPartByComponentList(CAOIField *FieldPtr, std::vector<CAOIComponent*> &ComponentList)//依照零件列表建立拋件列表
{
	const char fnName[] = "CAOIProject::CreateProjectDropOutPartByComponentList";
	if ( NULL == FieldPtr ) { return false; }
	
	CString        str;
	size_t         i=0, j=0;	
	unsigned int UniqueID=0;
	wchar_t ComponentName[MAX_JET_PATH]=L"";
	const wchar_t  PartNumber[32]=L"DROP_OUT";
	const wchar_t  ModelName[32]=L"DROP_OUT";
	const wchar_t  NozzleName[32]=L"DROP_OUT";	
	CString        SpcImageFolder;
	CString        ComponentFullName;
	double         BodySizeW=0;
	double         BodySizeH=0;		
	TPOINT2D       FrameRes;
	TPOINT2D       FieldStageCp;	
	TREGION4D      FieldStageRegion;
	TREGION4D      ModelStageRegion;
	RECT           ComponentImageRect={0,0,0,0};
	double         ComponentCadPosX=0;
	double         ComponentCadPosY=0;
	double         ComponentStagePosX=0;
	double         ComponentStagePosY=0;	
	TREGION4D      ComponentStageRegion;
	TREGION4D      ComponentImageRegion;
	CAOIWnd       *WndPtr = NULL;
	CAOIModel     *ModelPtr = NULL;	
	CAOIComponent *NewComponentPtr=NULL;
	const bool     bAppend=false;
	const bool     bEnhance=true;
	const bool     bSave3D=true;
	const bool     bSaveDefectImage = AOIDataCollect.GetSaveDefectImage();
	TUNI_FRAME     FrameUniFrame;
	TUNI_FRAME     ComponentUniFrame;
	std::vector<TUNI_FRAME> ComponentUniFrameList;	
	const size_t   NewComponentCount = ComponentList.size();
	const double   PanelBasePlane = FieldPtr->GetFieldPanelBasePlane();
	const size_t   OldDropOutCount = GetProjectDropOutPartCount_Inline();	
	const unsigned int DropOutFrameIndex = GetProjectParameter().m_DropOutPartFrameIndex;
	const unsigned int DropOutFrameUniqueID = GetProjectParameter().m_DropOutPartFrameUniqueID;
	TNoiseFilterParam  NoiseFilterParam = GetProjectParameter().m_DropOutPartSpaceNoiseFilter;	
	
	FieldStageCp.x = FieldPtr->GetFieldStagePosX();
	FieldStageCp.y = FieldPtr->GetFieldStagePosY();	
	SpcImageFolder = GetProjectSpcImageFolder();	
	NoiseFilterParam.BasePlaneParam.PanelBasePlane = PanelBasePlane;

	if ( GetProjectUseLocalFolder() )
	{	SpcImageFolder = GetProjectSpcImageFolderLocal();	}

	LockProject();	
	for ( i=0; i<NewComponentCount; i++ )
	{
		NewComponentPtr = ComponentList[i];
		if ( NULL == NewComponentPtr ) { continue; }

		WndPtr = AOIObjManager.CreateWndObj();		
		if ( NULL==WndPtr ) 
		{	continue;	}

		UniqueID = OldDropOutCount+i;
		::swprintf(ComponentName, L"%s_%d", ModelName, UniqueID+1);
		NewComponentPtr->SetComponentFieldPtr(FieldPtr);
		NewComponentPtr->SetComponentName(ComponentName);	
		NewComponentPtr->SetComponentModelName(ModelName);
		NewComponentPtr->SetComponentPartNumber(PartNumber);
		NewComponentPtr->SetComponentNozzleName(NozzleName);
		NewComponentPtr->SetComponentType(COMPONENT_TYPE_DROP_OUT);
		NewComponentPtr->SetComponentResultID_AOI(RESULT_ID_NG);
		NewComponentPtr->SetComponentPanelBasePlane(PanelBasePlane);

		const TBasePlaneParam &PartBasePlaneParam=NewComponentPtr->GetComponentSpaceBasePlaneParam();
		NoiseFilterParam.BasePlaneParam.BasePlaneEqMode = BASE_PLANE_EQUATION_PLANE;
		NoiseFilterParam.BasePlaneParam.GroundEquation=PartBasePlaneParam.GroundEquation;
		NoiseFilterParam.BasePlaneParam.LocalGroundEquation=PartBasePlaneParam.LocalGroundEquation;		
		NoiseFilterParam.BasePlaneParam.CalcBasePlaneMode = PartBasePlaneParam.CalcBasePlaneMode;
		NewComponentPtr->SetComponentSpaceBasePlaneParam(NoiseFilterParam.BasePlaneParam);
		NewComponentPtr->SetComponentSpaceNoiseFilterParam(NoiseFilterParam);

		BodySizeW = NewComponentPtr->GetComponentBodySizeW();
		BodySizeH = NewComponentPtr->GetComponentBodySizeH();
		ComponentCadPosX = NewComponentPtr->GetComponentCadPosX();
		ComponentCadPosY = NewComponentPtr->GetComponentCadPosY();
		ComponentStagePosX = NewComponentPtr->GetComponentStagePosX();
		ComponentStagePosY = NewComponentPtr->GetComponentStagePosY();		
		NewComponentPtr->SetComponentResultWidth(BodySizeW);
		NewComponentPtr->SetComponentResultLength(BodySizeH);
		NewComponentPtr->GetComponentRoiStageRegion(ComponentStageRegion);

		WndPtr->SetWndResultID(RESULT_ID_NG);
		WndPtr->SetWndDefectID(WND_DEFECT_FOREIGN_BODY);		
		WndPtr->GetWndBox().SetBoxSize(BodySizeW, BodySizeH);				
		WndPtr->GetWndAlgParam().SetAlgResultID(RESULT_ID_NG);
		WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameIndex(DropOutFrameIndex);
		WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameUniqueID(DropOutFrameUniqueID);		

		ModelPtr = NewComponentPtr->GetComponentModelPtr();
		ModelPtr->SetModelName(ModelName);
		ModelPtr->SetModelType(MODEL_TYPE_OTHERS);		
		ModelPtr->GetModelBodyBox().SetBoxSize(BodySizeW, BodySizeH);
		ModelPtr->AddModelWndPtr(WndPtr, false);
		ModelPtr->SetModelResultID(RESULT_ID_NG);		
		ModelPtr->SetModelAttachedPosCad(ComponentCadPosX, ComponentCadPosY);
		ModelPtr->SetModelAttachedPosStage(ComponentStagePosX, ComponentStagePosY);		
		ModelPtr->SetModelSpaceBasePlaneParam(NoiseFilterParam.BasePlaneParam);
		ModelPtr->SetModelSpaceNoiseFilterParam(NoiseFilterParam);
		ModelPtr->CalcModelTotalRegionAll();
		ModelPtr->GetModelTotalRegionStage(ModelStageRegion);		
		AddProjectDropOutPartPtr(NewComponentPtr, false);//加入拋件列表內-暫存
		
		//存出圖片
		if ( true == bSaveDefectImage )
		{
			CAOIFrame   *FieldFramePtr = NULL;
			const size_t FiledFrameCount = FieldPtr->GetFieldFramePtrCount();
			ComponentStageRegion = ModelStageRegion;
			ComponentFullName = NewComponentPtr->GetComponentFullName();			
			str.Format(_T("%s\\%s.%s"), SpcImageFolder, ComponentFullName, _T("JPG"));			
			for ( j=0; j<FiledFrameCount; j++ )
			{
				FieldFramePtr = FieldPtr->GetFieldFramePtr(j, false);
				if ( NULL == FieldFramePtr ) { continue; }				
				FrameRes.x = FieldFramePtr->GetFrameResolutionX();
				FrameRes.y = FieldFramePtr->GetFrameResolutionY();
				FieldFramePtr->GetFrameUniFrame(FrameUniFrame);
				if ( 0 == FrameUniFrame.BitCount ) 
				{	continue; }

				AOIDataCollect.MapStageRegionToCamera(FrameUniFrame.ImageW, FrameUniFrame.ImageH, FrameRes, ComponentStageRegion, FieldStageCp, ComponentImageRegion);
				JetAPI::Region4DToRect(ComponentImageRegion, ComponentImageRect, true);
				JetAPI::BoundaryRect(FrameUniFrame.ImageW, FrameUniFrame.ImageH, ComponentImageRect);//調整尺寸, 確保小於相機影像內

				ComponentUniFrame = TUNI_FRAME();
				ComponentUniFrame.ImageW = (ComponentImageRect.right-ComponentImageRect.left);
				ComponentUniFrame.ImageH = (ComponentImageRect.bottom-ComponentImageRect.top);
				ComponentUniFrame.BitCount = FrameUniFrame.BitCount;
				ComponentUniFrame.ImageStep = JetAPI::GetBMPImagePixelsPerLine(ComponentUniFrame.ImageW, FrameUniFrame.BitCount, 4);
				if ( NULL==FrameUniFrame.SpacePtr || NULL==FrameUniFrame.MaskPtr )
				{
					if ( ImageAPI.ExtractRoiImage(FrameUniFrame.ImageW, FrameUniFrame.ImageH, FrameUniFrame.ImageStep, FrameUniFrame.BitCount, FrameUniFrame.ImagePtr, ComponentImageRect, ComponentUniFrame.ImageStep, ComponentUniFrame.ImagePtr, false) == false )
					{
						JetAPI::ClearUniFrame(ComponentUniFrame);
						continue; 
					}
				}
				else
				{
					if ( ImageAPI.ExtractRoiImage(FrameUniFrame.ImageW, FrameUniFrame.ImageH, FrameUniFrame.ImageStep, FrameUniFrame.BitCount, FrameUniFrame.MaskPtr, ComponentImageRect, ComponentUniFrame.ImageStep, ComponentUniFrame.MaskPtr, false) == false )
					{	
						JetAPI::ClearUniFrame(ComponentUniFrame);
						continue; 
					}

					if ( ImageAPI.ExtractSpaceRoiImage(FrameUniFrame.ImageW, FrameUniFrame.ImageH, FrameUniFrame.ImageStep, FrameUniFrame.BitCount, FrameUniFrame.SpacePtr, ComponentImageRect, ComponentUniFrame.ImageStep, ComponentUniFrame.SpacePtr, false) == false )
					{	
						JetAPI::ClearUniFrame(ComponentUniFrame);
						continue; 
					}
					
					MASK_PTR     Mask2DPtr=NULL;
					MASK_PTR     DstMaskPtr=NULL;
					SPACE_PTR    DstSpacePtr=NULL;										
					IMAGE_SIZE   SrcW=ComponentUniFrame.ImageW;
					IMAGE_SIZE   SrcH=ComponentUniFrame.ImageH;
					IMAGE_SIZE   SrcStep=ComponentUniFrame.ImageStep;
					MASK_PTR     SrcMaskPtr=ComponentUniFrame.MaskPtr;
					SPACE_PTR    SrcSpacePtr=ComponentUniFrame.SpacePtr;
					const size_t SrcSize=ImageAPI.CalcBufferSize(SrcStep, SrcH);					
					if ( JetMemory.alloc_func(SrcSize, DstMaskPtr, fnName, "DstMaskPtr") == false || 
						 JetMemory.alloc_func(SrcSize, DstSpacePtr, fnName, "DstSpacePtr") == false )
					{
						JetMemory.free_func(DstMaskPtr);
						JetMemory.free_func(DstSpacePtr);
					}
					else
					{
						const int    OpenMPCnt = AOIDataCollect.GetOpenMPCount_Inspection();
						if ( ImageAPI.BuildSpaceData3(SrcW, SrcH, SrcStep, SrcSpacePtr, SrcMaskPtr, Mask2DPtr, OpenMPCnt, NoiseFilterParam, DstSpacePtr, DstMaskPtr) == false)
						{
							JetMemory.free_func(DstMaskPtr);
							JetMemory.free_func(DstSpacePtr);
						}
						else
						{
							JetMemory.free_func(SrcMaskPtr);
							JetMemory.free_func(SrcSpacePtr);
							ComponentUniFrame.MaskPtr = DstMaskPtr;
							ComponentUniFrame.SpacePtr = DstSpacePtr;
						}
					}
				}
				ComponentUniFrameList.push_back(ComponentUniFrame);				
			}
			double   SpaceRatio = GetProjectSpaceToGrayRatio();
			ImageAPI.SaveUniFrameImage(str, ComponentUniFrameList, true, bEnhance, bSave3D, bAppend, SpaceRatio);
			JetAPI::ClearUniFrameList(ComponentUniFrameList);
		}	
	}
	UnlockProject();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::TestProjectDropOutPartBy3DHeight(CAOIField *FieldPtr)
{
	const char fnName[] = "CAOIProject::TestProjectDropOutPartBy3DHeight";
	const int TestDropOutPart = GetProjectTestDropOutPart();
	if ( FN_DISABLE == TestDropOutPart ) { return true; }

	if ( NULL == FieldPtr ) { return false; }
	if ( FieldPtr->CheckFieldMergeFinish() == false ) { return true; }	
	const int TestFrameIndex = GetProjectParameter().m_DropOutPartFrameIndex;	
	CAOIFrame *FramePtr = FieldPtr->GetFieldFramePtr(TestFrameIndex, true);
	if ( NULL == FramePtr )
	{
		m_ErrorString.Format(_T("Error, Project Test Drop Out Part Frame index Execption [%d]"), TestFrameIndex+1);
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}	
	
	IMAGE_SIZE   ImageW2D = 0;
	IMAGE_SIZE   ImageH2D = 0;
	IMAGE_SIZE   ImageStep2D = 0;
	IMAGE_SIZE   BitCount2D = 0;
	size_t       BufferSize2D=0;
	IMAGE_PTR    ImagePtr2D = NULL;	

	const int    MapIndex = TestFrameIndex;
	FRAME_TYPE   FrameType = FramePtr->GetFrameType();
	IMAGE_SIZE   ImageW = FramePtr->GetFrameImageW();
	IMAGE_SIZE   ImageH = FramePtr->GetFrameImageH();
	IMAGE_SIZE   ImageStep = FramePtr->GetFrameImageStep();
	IMAGE_SIZE   BitCount = FramePtr->GetFrameImageBitCount();
	MASK_PTR     MaskPtr = FramePtr->GetFrameMaskPtr();	
	SPACE_PTR    SpacePtr = FramePtr->GetFrameSpacePtr();	
	IMAGE_PTR    ImagePtr = FramePtr->GetFrameImagePtr();	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const IMAGE_SIZE MaskBitCount = 8;
	const int SaveImage = GetProjectDropOutPartSaveImage();	
	TNoiseFilterParam NoiseParam = GetProjectParameter().m_DropOutPartSpaceNoiseFilter;	
	TBasePlaneParam  &BasePlaneParam=NoiseParam.BasePlaneParam;
	if ( true == BasePlaneParam.BasePlane2DMaskEnabled )
	{	
		const unsigned int TestFrame2DIndex=BasePlaneParam.BasePlane2DMaskFrameIndex;
		CAOIFrame *FramePtr2D = FieldPtr->GetFieldFramePtr(TestFrame2DIndex, true);
		if ( NULL == FramePtr2D )
		{
			m_ErrorString.Format(_T("Error, Project Test Drop Out Part 2D Frame index Execption [%d]"), TestFrame2DIndex+1);
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
		ImageW2D = FramePtr2D->GetFrameImageW();
		ImageH2D = FramePtr2D->GetFrameImageH();
		ImageStep2D = FramePtr2D->GetFrameImageStep();;
		BitCount2D = FramePtr2D->GetFrameImageBitCount();
		ImagePtr2D = FramePtr2D->GetFrameImagePtr();
		BufferSize2D = ImageAPI.CalcBufferSize(ImageStep2D, ImageH2D);
		if ( ImageW!=ImageW2D || ImageH!=ImageH2D || NULL==ImagePtr2D )
		{
			m_ErrorString.Format(_T("Error, Project Test Drop Out Part 2D Frame index Execption [%d]"), TestFrame2DIndex+1);
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
	}

	const int nAlign = 4;
	const int nTeachMapW = (int)(m_ProjectMapW[MapIndex]);//專案底圖寬度
	const int nTeachMapH = (int)(m_ProjectMapH[MapIndex]);//專案底圖長度
	const int nTeachMapStep = (int)(m_ProjectMapStep[MapIndex]);//專案底圖步長
	const int nTeachMapBitCount = (int)(m_ProjectBitCount[MapIndex]);//專案底圖位元數
	const int nTeachMapW2 = nTeachMapW/2;
	const int nTeachMapH2 = nTeachMapH/2;
	IMAGE_PTR TeachMapPtr = m_ProjectMapPtr[MapIndex];//專案底圖指標

	const int nTeachMaskW = (int)(m_ProjectMapMaskW);
	const int nTeachMaskH = (int)(m_ProjectMapMaskH);
	const int nTeachMaskStep = (int)(m_ProjectMapMaskStep);
	const int nTeachMaskBitCount = (int)(m_ProjectMapMaskBitCount);
	IMAGE_PTR TeachMaskPtr = m_ProjectMapMaskPtr;

	if ( 8 != BitCount ) { return false; }
	if ( BitCount != nTeachMapBitCount )
	{	
		m_ErrorString.Format(_T("Error, Project Test Drop Out Part BitCount != nTeachMapBitCount [%d]"), MapIndex+1);
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}

	MASK_PTR     DstMskPtr=NULL;
	SPACE_PTR    DstSpcPtr=NULL;
	MASK_PTR     CellMskPtr=NULL;
	SPACE_PTR    CellSpcPtr=NULL;
	MASK_PTR     CellMsk2DPtr=NULL;	
	IMAGE_SIZE   uCellW = 0;
	IMAGE_SIZE   uCellH = 0;
	IMAGE_SIZE   uCellStep = 0;	
	const int    NPixels = GetProjectMapScaleMode();

#ifdef _DEBUG
	bool         bSave=true;	
#endif _DEBUG
	CString      str;
	CString      Folder;
	CString      MainName;
	CString      KeyName=_T("DropOut");	
	CString      FrameFileName = FramePtr->GetFrameFileName();
	std::vector<CString> SaveFileList;
	Folder = GetProjectDebugFolderDropOut();
	JetAPI::ExtractMainFileName(FrameFileName, MainName);	

	if ( JetMemory.alloc_func(BufferSize, DstMskPtr, fnName, "DstMskPtr")==false ||
		 JetMemory.alloc_func(BufferSize, DstSpcPtr, fnName, "DstSpcPtr")==false ||
		 JetMemory.alloc_func(BufferSize, CellMskPtr, fnName, "CellMskPtr")==false ||
		 JetMemory.alloc_func(BufferSize, CellSpcPtr, fnName, "CellSpcPtr")==false  )
	{
		m_ErrorString = JetMemory.GetErrorString();		
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(CellSpcPtr);
		JetMemory.free_func(CellMskPtr);
		JetMemory.free_func(CellMsk2DPtr);
		SaveProjectDropOutPartLog(m_ErrorString);
		return false; 
	}	
	const double ScaleVal = 1.00/NPixels;
	ImageAPI.CalcScaleSize(ImageW, ImageH, ImageStep, BitCount, ScaleVal, uCellW, uCellH, uCellStep);
	if ( ImageAPI.ScaleMask3(ImageW, ImageH, ImageStep, MaskPtr, ScaleVal, uCellW, uCellH, uCellStep, CellMskPtr) == false ||
		 ImageAPI.ScaleSpace3(ImageW, ImageH, ImageStep, SpacePtr, ScaleVal, uCellW, uCellH, uCellStep, CellSpcPtr) == false )	
	{
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(CellSpcPtr);
		JetMemory.free_func(CellMskPtr);
		JetMemory.free_func(CellMsk2DPtr);
		m_ErrorString = ImageAPI.GetImageApiErrorString();
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}

#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_CellMask.PNG"), Folder, KeyName, MainName);
		//ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellMskPtr, true);
		//SaveFileList.push_back(str);
	}
#endif//_DEBUG
	
	if ( NULL!=ImagePtr2D && 24==BitCount2D )
	{
		IMAGE_PTR  Cell2DPtr=NULL;
		IMAGE_SIZE uCellW2D = 0;
		IMAGE_SIZE uCellH2D = 0;
		IMAGE_SIZE uCellStep2D = 0;		
		const int  nMaskColorGroupLinkIndex = BasePlaneParam.BasePlane2DMaskGroupLinkIndex;		
		CColorGroup *ColorGroupPtr = GetProjectColorGroupPtr(nMaskColorGroupLinkIndex, true);
		if ( NULL != ColorGroupPtr )
		{
			if ( JetMemory.alloc_func(BufferSize2D, Cell2DPtr, fnName, "Cell2DPtr")==false ||
				 JetMemory.alloc_func(BufferSize, CellMsk2DPtr, fnName, "CellMsk2DPtr")==false )
			{
				m_ErrorString = JetMemory.GetErrorString();		
				JetMemory.free_func(Cell2DPtr);
				JetMemory.free_func(DstSpcPtr);
				JetMemory.free_func(DstMskPtr);
				JetMemory.free_func(CellSpcPtr);
				JetMemory.free_func(CellMskPtr);
				JetMemory.free_func(CellMsk2DPtr);
				SaveProjectDropOutPartLog(m_ErrorString);
				return false; 
			}

			ImageAPI.CalcScaleSize(ImageW2D, ImageH2D, ImageStep2D, BitCount2D, ScaleVal, uCellW2D, uCellH2D, uCellStep2D);	
			if ( ImageAPI.ScaleImage3(ImageW2D, ImageH2D, ImageStep2D, BitCount2D, ImagePtr2D, ScaleVal, uCellW2D, uCellH2D, uCellStep2D, Cell2DPtr) == false )
			{
				JetMemory.free_func(Cell2DPtr);
				JetMemory.free_func(DstSpcPtr);
				JetMemory.free_func(DstMskPtr);
				JetMemory.free_func(CellSpcPtr);
				JetMemory.free_func(CellMskPtr);
				JetMemory.free_func(CellMsk2DPtr);
				m_ErrorString = ImageAPI.GetImageApiErrorString();		
				SaveProjectDropOutPartLog(m_ErrorString);
				return false;
			}					
		#ifdef _DEBUG
			if ( true == bSave )
			{
				str.Format(_T("%s\\%s_%s_Cell2D.PNG"), Folder, KeyName, MainName);
				ImageAPI.SaveImage(str, uCellW2D, uCellH2D, uCellStep2D, BitCount2D, Cell2DPtr, true);
				SaveFileList.push_back(str);
			}			
		#endif//_DEBUG

			RECT ClrRect;
			const bool bOpenMP = false;
			JetAPI::SizeToRect(uCellW2D, uCellH2D, ClrRect);
			if ( ImageAPI.ColorImageColorFilter3(uCellW2D, uCellH2D, uCellStep2D, Cell2DPtr, *ColorGroupPtr, ClrRect, uCellStep, CellMsk2DPtr, false, bOpenMP) == false )
			{
				JetMemory.free_func(Cell2DPtr);
				JetMemory.free_func(DstSpcPtr);
				JetMemory.free_func(DstMskPtr);
				JetMemory.free_func(CellSpcPtr);
				JetMemory.free_func(CellMskPtr);
				JetMemory.free_func(CellMsk2DPtr);
				m_ErrorString = ImageAPI.GetImageApiErrorString();		
				SaveProjectDropOutPartLog(m_ErrorString);
				return false;
			}			
		#ifdef _DEBUG
			if ( true == bSave )
			{
				str.Format(_T("%s\\%s_%s_CellMask2D.PNG"), Folder, KeyName, MainName);
				ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellMsk2DPtr, true);
				SaveFileList.push_back(str);
			}			
		#endif//_DEBUG		
			JetMemory.free_func(Cell2DPtr);
		}
	}

	/*
	if ( AOIDataCollect.ExecEnhanceDisplayImage(uCellW, uCellH, uCellStep, BitCount, CellPtr, CellPtr) == false )
	{
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(CellSpcPtr);
		JetMemory.free_func(CellMskPtr);
		JetMemory.free_func(CellMsk2DPtr);
		m_ErrorString = AOIDataCollect.GetErrorString();
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;	
	}
	*/
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_CellMask2.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellMskPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG	
	
	const bool bUseMapMaskFirst=true;//20240119
	if ( true == bUseMapMaskFirst )
	{
		TPOINT2D MapRes;
		TREGION4D FieldRgn;		
		MASK_PTR LocMaskTempPtr=NULL;		
		FieldPtr->GetFieldStageRgn_Real(FieldRgn);
		GetProjectMapResolution(MapRes.x, MapRes.y);
		const size_t LocMaskBufSize=ImageAPI.CalcBufferSize(uCellStep, uCellH);
		if ( JetMemory.alloc_func(LocMaskBufSize, LocMaskTempPtr, fnName, "LocMaskTempPtr") == true )	
		{
			const int    MaskMode=PROJECT_PART_MASK_ALL;			
			::memset(LocMaskTempPtr, 0xFF, sizeof(MASK_DATA)*LocMaskBufSize);			
			if (CreateProjectPartMaskImage_v2(uCellW, uCellH, uCellStep, LocMaskTempPtr, FieldRgn, MapRes, MaskMode, 0x00) == true)
			{	
				for ( size_t i=0; i<LocMaskBufSize; i++ )
				{	//Part In Fov
					if ( 0 != LocMaskTempPtr[i] ) { continue; }
					CellMskPtr[i] = PHASE_MASK_LOW_CONTRAST;
				}
			#ifdef _DEBUG
				if ( true == bSave )
				{
					str.Format(_T("%s\\%s_%s_CellMask3.PNG"), Folder, KeyName, MainName);
					ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellMskPtr, true);
					SaveFileList.push_back(str);
				}
			#endif//_DEBUG
			}
			if ( NULL!=TeachMaskPtr )
			{
				if ( CreateProjectFieldMapMaskImage(uCellW, uCellH, uCellStep, LocMaskTempPtr, FieldRgn) == true )			
				{
					for ( size_t i=0; i<LocMaskBufSize; i++ )
					{
						if ( 0xFF == LocMaskTempPtr[i] ) { continue; }
						CellMskPtr[i] = PHASE_MASK_LOW_CONTRAST;
					}
				#ifdef _DEBUG
					if ( true == bSave )
					{
						str.Format(_T("%s\\%s_%s_CellMask4.PNG"), Folder, KeyName, MainName);
						ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellMskPtr, true);
						SaveFileList.push_back(str);
					}
				#endif//_DEBUG		
				}
			}
			JetMemory.free_func(LocMaskTempPtr);			
		}
	}

	const size_t CellBufSize=ImageAPI.CalcBufferSize(uCellStep, uCellH);	
	const int    OpenMPCnt = AOIDataCollect.GetOpenMPCount_Inspection();	
	const double PanelBasePlane = FieldPtr->GetFieldPanelBasePlane();
	BasePlaneParam.BasePlaneXYPitch = 4;
	NoiseParam.BasePlaneParam.PanelBasePlane = PanelBasePlane;	
	BasePlaneParam.BasePlaneEqMode = BASE_PLANE_EQUATION_CURVE;
	if ( ImageAPI.BuildSpaceData3(uCellW, uCellH, uCellStep, CellSpcPtr, CellMskPtr, CellMsk2DPtr, OpenMPCnt, NoiseParam, DstSpcPtr, DstMskPtr) == false )
	{	
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(CellSpcPtr);
		JetMemory.free_func(CellMskPtr);
		JetMemory.free_func(CellMsk2DPtr);
		m_ErrorString = ImageAPI.GetImageApiErrorString();	
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}

	double nX=0, nY=0, nZ=0;
	BasePlaneParam.GroundEquation.GetGroundNormal(nX, nY, nZ);
	str.Format(_T("%s Panel Normal(%.6f, %.6f, %.2f)"), KeyName, nX, nY, nZ);
	SaveProjectDropOutPartLog(str);
#ifdef _DEBUG
	if ( true == bSave )
	{
		RECT LocRect={0};
		IMAGE_PTR DstImage=CellMskPtr;
		JetAPI::SizeToRect(uCellW, uCellH, LocRect);
		const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
		ImageAPI.SpaceGrayImageConvertToGray3(uCellW, uCellH, uCellStep, DstSpcPtr, DstMskPtr, LocRect, uCellStep, DstImage, SpaceRatio, false);
		str.Format(_T("%s\\%s_%s_Space.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, DstImage, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG
	JetMemory.free_func(CellSpcPtr);
	JetMemory.free_func(CellMskPtr);
	JetMemory.free_func(CellMsk2DPtr);

	//尋找在專案底圖的基本方位		
	TPOINT2D MapRes;	
	TREGION4D CadRgn;
	TREGION4D CellRgn;
	TREGION4D StageRgn;	
	TREGION4D TeachRgn;
	const double FieldStagePosX = FieldPtr->GetFieldStagePosX();
	const double FieldStagePosY = FieldPtr->GetFieldStagePosY();
	const double FieldStageSizeW = FieldPtr->GetFieldSizeW_Real();
	const double FieldStageSizeH = FieldPtr->GetFieldSizeH_Real();	

	GetProjectMapInfo(MapRes, CadRgn, StageRgn);
	GetProjectMapCalcRgn(TeachRgn);
	TeachRgn = StageRgn;

	CellRgn.minX = FieldStagePosX-(FieldStageSizeW*0.5);
	CellRgn.minY = FieldStagePosY-(FieldStageSizeH*0.5);
	CellRgn.maxX = FieldStagePosX+(FieldStageSizeW*0.5);
	CellRgn.maxY = FieldStagePosY+(FieldStageSizeH*0.5);

	int MapRoiPosX = 0;
	int MapRoiPosY = 0;	
	const double MapTeachCpX = TeachRgn.GetCpX();
	const double MapTeachCpY = TeachRgn.GetCpY();
	const double RgnOffsetX = (FieldStagePosX-MapTeachCpX);
	const double RgnOffsetY = (FieldStagePosY-MapTeachCpY);
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	if ( true == SignX )
	{	MapRoiPosX = (int)(nTeachMapW2+(RgnOffsetX/MapRes.x));	}
	else
	{	MapRoiPosX = (int)(nTeachMapW2-(RgnOffsetX/MapRes.x)); }	
	if ( true == SignY )
	{	MapRoiPosY = (int)(nTeachMapH2+(RgnOffsetY/MapRes.y));	}
	else
	{	MapRoiPosY = (int)(nTeachMapH2-(RgnOffsetY/MapRes.y));	}
	MapRoiPosY = nTeachMapH-MapRoiPosY;	

	RECT  MapRoiRect={0};	
	RECT  CellRoiRect={0};
	bool  ModifyCellRoi=false;
	IMAGE_SIZE RoiExt = 0;//沒有在重新對位
	IMAGE_SIZE MapRoiW = uCellW+RoiExt;
	IMAGE_SIZE MapRoiH = uCellH+RoiExt;
	IMAGE_SIZE MapRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, BitCount, nAlign);
	MapRoiRect.left   = MapRoiPosX-(MapRoiW/2);
	MapRoiRect.top    = MapRoiPosY-(MapRoiH/2);
	MapRoiRect.right  = MapRoiPosX+(MapRoiW/2);
	MapRoiRect.bottom = MapRoiPosY+(MapRoiH/2);		
	ModifyCellRoi = false;
	JetAPI::SizeToRect(uCellW, uCellH, CellRoiRect);	
	if ( MapRoiRect.left < 0 )
	{	
		ModifyCellRoi = true;
		CellRoiRect.left = CellRoiRect.left+(0-MapRoiRect.left);
		MapRoiRect.left = 0;
	}
	if ( MapRoiRect.right > nTeachMapW )
	{
		ModifyCellRoi = true;
		CellRoiRect.right = CellRoiRect.right-(MapRoiRect.right-nTeachMapW);
		MapRoiRect.right = nTeachMapW;		
	}
	if ( MapRoiRect.top < 0 )
	{	
		ModifyCellRoi = true;
		CellRoiRect.top = CellRoiRect.top+(0-MapRoiRect.top);
		MapRoiRect.top = 0;
	}
	if ( MapRoiRect.bottom > nTeachMapH )
	{
		ModifyCellRoi = true;
		CellRoiRect.bottom = CellRoiRect.bottom-(MapRoiRect.bottom-nTeachMapH);
		MapRoiRect.bottom = nTeachMapH;		
	}
	if ( CellRoiRect.right<=CellRoiRect.left || CellRoiRect.bottom<=CellRoiRect.top )
	{
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
	#ifdef _DEBUG
		JetAPI::DeleteFileList(SaveFileList);
	#endif//_DEBUG
		SaveProjectDropOutPartLog(_T("CellRoiRect.right<=CellRoiRect.left || CellRoiRect.bottom<=CellRoiRect.top"));		
		return true; 			
	}
	if ( true == ModifyCellRoi )
	{
		if ( true == SignX )
		{
			CellRgn.minX += (CellRoiRect.left-0)*MapRes.x;
			CellRgn.maxX -= (uCellW-CellRoiRect.right)*MapRes.x;
		}
		else
		{	
			CellRgn.maxX -= (CellRoiRect.left-0)*MapRes.x;
			CellRgn.minX += (uCellW-CellRoiRect.right)*MapRes.x;		
		}	
		if ( true == SignY )
		{	
			CellRgn.maxY -= (CellRoiRect.top-0)*MapRes.y;
			CellRgn.minY += (uCellH-CellRoiRect.bottom)*MapRes.y;
		}
		else
		{
			CellRgn.minY += (CellRoiRect.top-0)*MapRes.y;
			CellRgn.maxY -= (uCellH-CellRoiRect.bottom)*MapRes.y;
		}

		MASK_PTR   CellRoiMskPtr=NULL;
		SPACE_PTR  CellRoiSpcPtr=NULL;
		IMAGE_SIZE CellRoiW=CellRoiRect.right-CellRoiRect.left;
		IMAGE_SIZE CellRoiH=CellRoiRect.bottom-CellRoiRect.top;
		IMAGE_SIZE CellRoiStep=JetAPI::GetBMPImagePixelsPerLine(CellRoiW, BitCount, nAlign);;
		const size_t CellRoiBuffserSize = ImageAPI.CalcBufferSize(CellRoiStep, CellRoiH);
		if ( JetMemory.alloc_func(CellRoiBuffserSize, CellRoiMskPtr, fnName, "CellRoiMskPtr") == false ||
			 JetMemory.alloc_func(CellRoiBuffserSize, CellRoiSpcPtr, fnName, "CellRoiSpcPtr") == false  )
		{
			JetMemory.free_func(DstSpcPtr);
			JetMemory.free_func(DstMskPtr);		
			JetMemory.free_func(CellRoiSpcPtr);
			JetMemory.free_func(CellRoiMskPtr);		
			m_ErrorString = JetMemory.GetErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
		if ( ImageAPI.ExtractRoiImage3(uCellW, uCellH, uCellStep, BitCount, DstMskPtr, CellRoiRect, CellRoiStep, CellRoiMskPtr, false) == false )
		{
			JetMemory.free_func(DstSpcPtr);
			JetMemory.free_func(DstMskPtr);		
			JetMemory.free_func(CellRoiSpcPtr);
			JetMemory.free_func(CellRoiMskPtr);		
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
		if ( ImageAPI.ExtractSpaceRoiImage3(uCellW, uCellH, uCellStep, BitCount, DstSpcPtr, CellRoiRect, CellRoiStep, CellRoiSpcPtr, false) == false )
		{
			JetMemory.free_func(DstSpcPtr);
			JetMemory.free_func(DstMskPtr);		
			JetMemory.free_func(CellRoiSpcPtr);
			JetMemory.free_func(CellRoiMskPtr);		
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}		
	#ifdef _DEBUG
		if ( true == bSave )
		{
			RECT LocRect={0};
			IMAGE_PTR DstImage=DstMskPtr;
			JetAPI::SizeToRect(CellRoiW, CellRoiH, LocRect);
			const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
			ImageAPI.SpaceGrayImageConvertToGray3(CellRoiW, CellRoiH, CellRoiStep, CellRoiSpcPtr, CellRoiMskPtr, LocRect, CellRoiStep, DstImage, SpaceRatio, false);
			str.Format(_T("%s\\%s_%s_Space3.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, CellRoiW, CellRoiH, CellRoiStep, BitCount, DstImage, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		uCellW = CellRoiW;
		uCellH = CellRoiH;
		uCellStep = CellRoiStep;
		DstSpcPtr = CellRoiSpcPtr;
		DstMskPtr = CellRoiMskPtr;
		MapRoiW = MapRoiRect.right-MapRoiRect.left;
		MapRoiH = MapRoiRect.bottom-MapRoiRect.top;
		MapRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, BitCount, nAlign);
	}
	
	IMAGE_PTR    MapMaskRoiPtr = NULL;
	const IMAGE_SIZE MapMaskRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, MaskBitCount, nAlign);
	const size_t MapMaskRoiBufferSize = ImageAPI.CalcBufferSize(MapMaskRoiStep, MapRoiH);
	if ( NULL!=TeachMaskPtr && nTeachMaskW==nTeachMapW && nTeachMaskH==nTeachMapH )
	{
		if ( JetMemory.alloc_func(MapMaskRoiBufferSize, MapMaskRoiPtr, fnName, "MapMaskRoiPtr") == false )
		{
			JetMemory.free_func(DstSpcPtr);
			JetMemory.free_func(DstMskPtr);
			JetMemory.free_func(MapMaskRoiPtr);			
			m_ErrorString = JetMemory.GetErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false; 
		}			
		if ( ImageAPI.ExtractRoiImage3(nTeachMaskW, nTeachMaskH, nTeachMaskStep, MaskBitCount, TeachMaskPtr, MapRoiRect, MapMaskRoiStep, MapMaskRoiPtr, false) == false )
		{
			JetMemory.free_func(DstSpcPtr);
			JetMemory.free_func(DstMskPtr);
			JetMemory.free_func(MapMaskRoiPtr);			
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MapMaskRoi.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiPtr, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
		if (false == CreateProjectColorMaskImage(MapRoiW, MapRoiH, MapMaskRoiStep, MapMaskRoiPtr, MapRoiRect, 0xff, fnName, SaveFileList)) {
			JetMemory.free_func(DstSpcPtr);
			JetMemory.free_func(DstMskPtr);
			JetMemory.free_func(MapMaskRoiPtr);
			//m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
	#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_MapMaskRoi2.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiPtr, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
		if (false == CreateProjectSkewMaskImage(FieldPtr,MapRoiW, MapRoiH, MapMaskRoiStep, MapMaskRoiPtr, MapRoiRect, 0xff, fnName, SaveFileList)) {
			JetMemory.free_func(DstSpcPtr);
			JetMemory.free_func(DstMskPtr);
			JetMemory.free_func(MapMaskRoiPtr);
			//m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_MapMaskRoi3.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiPtr, true);
			SaveFileList.push_back(str);
		}
#endif//_DEBUG
	}

	MASK_PTR     RawMaskPtr = NULL;	
	MASK_PTR     TmpMaskPtr = NULL;	
	MASK_PTR     PartMaskPtr = NULL;	
	const IMAGE_SIZE MaskW  = uCellW;
	const IMAGE_SIZE MaskH  = uCellH;	
	const IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(MaskW, MaskBitCount, nAlign);
	const size_t MaskBufferSize = ImageAPI.CalcBufferSize(MaskStep, MaskH);
	if ( JetMemory.alloc_func(MaskBufferSize, RawMaskPtr, fnName, "RawMaskPtr") == false || 		 
		 JetMemory.alloc_func(MaskBufferSize, TmpMaskPtr, fnName, "TmpMaskPtr") == false ||
		 JetMemory.alloc_func(MaskBufferSize, PartMaskPtr, fnName, "PartMaskPtr") == false )
	{
		m_ErrorString = JetMemory.GetErrorString();		
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtr);		
		JetMemory.free_func(TmpMaskPtr);
		JetMemory.free_func(PartMaskPtr);
		SaveProjectDropOutPartLog(m_ErrorString);
		return false; 
	}

	//剔除此FOV內的零件
	size_t       i=0, j=0;	
	TREGION4D    FieldRgn=CellRgn;
	const int    MaskMode=PROJECT_PART_MASK_ALL;
	::memset(RawMaskPtr, 0xFF, sizeof(MASK_DATA)*MaskBufferSize);
	if ( CreateProjectPartMaskImage_v2(MaskW, MaskH, MaskStep, RawMaskPtr, FieldRgn, MapRes, MaskMode, 0x00) == false )
	{
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtr);		
		JetMemory.free_func(TmpMaskPtr);
		JetMemory.free_func(PartMaskPtr);		
		SaveProjectDropOutPartLog(m_ErrorString);
		return false; 
	}		
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskNoPart.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, RawMaskPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG	
	if ( ImageAPI.ErodeGrayImage3(MaskW, MaskH, MaskStep, RawMaskPtr, 9, 1, PartMaskPtr) == false )
	{		
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtr);		
		JetMemory.free_func(TmpMaskPtr);
		JetMemory.free_func(PartMaskPtr);
		m_ErrorString = ImageAPI.GetImageApiErrorString();
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskNoPartErode.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, PartMaskPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG
	
	const SPACE_DATA PartOverLow  = GetProjectParameter().m_DropOutPartOverLow;
	const SPACE_DATA PartOverHigh = GetProjectParameter().m_DropOutPartOverHigh;
	for ( i=0; i<MaskBufferSize; i++ )
	{
		RawMaskPtr[i] = 0;
		//Space Noise
		if ( ImageAPI.CheckSpaceMaskValid(DstMskPtr[i]) == false ) { continue; }
		//Part In Fov
		if ( 0 == PartMaskPtr[i] ) { continue; }
		if ( DstSpcPtr[i] < PartOverLow ) { continue; }
		if ( DstSpcPtr[i] > PartOverHigh ) { continue; }
		RawMaskPtr[i] = 255;
	}
	if ( NULL != MapMaskRoiPtr )
	{
		for ( i=0; i<MaskBufferSize; i++ )
		{
			if ( 0xFF == MapMaskRoiPtr[i] ) { continue; }
			RawMaskPtr[i] = 0;			
		}
	}

#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskRaw.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, RawMaskPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG	

	//JetMemory.free_func(DstSpcPtr);
	//JetMemory.free_func(DstMskPtr);
	JetMemory.free_func(PartMaskPtr);
	JetMemory.free_func(MapMaskRoiPtr);	

	if ( ExecProjectDropOutPartMaskFilter(FieldPtr, MapRes, MaskW, MaskH, MaskStep, RawMaskPtr, TmpMaskPtr, SaveFileList) == false )
	{
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(RawMaskPtr);		
		JetMemory.free_func(TmpMaskPtr);
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskAfter.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TmpMaskPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG	

	::memcpy(RawMaskPtr, TmpMaskPtr, sizeof(MASK_DATA)*MaskBufferSize);
	JetMemory.free_func(TmpMaskPtr);		
	
	const bool bMeasure = false;
	const double MinThickness=PartOverLow*2/4;
	std::vector<CAOIComponent*> NewComponentList;
	const double PartW = GetProjectParameter().m_DropOutPartMinSizeW;
	const double PartH = GetProjectParameter().m_DropOutPartMinSizeH;
	const double PartWHRatio = GetProjectParameter().m_DropOutPartMaxSizeR;	
	if ( CreateProjectComponentByBlob(MaskW, MaskH, MaskStep, RawMaskPtr, DstMskPtr, DstSpcPtr, FieldRgn, MapRes, BasePlaneParam.GroundEquation, PartW, PartH, PartWHRatio, MinThickness, bMeasure, NewComponentList) == false )
	{
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(RawMaskPtr);		
		JetMemory.free_func(TmpMaskPtr);
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}	
	JetMemory.free_func(DstSpcPtr);
	JetMemory.free_func(DstMskPtr);
	JetMemory.free_func(RawMaskPtr);
	JetMemory.free_func(TmpMaskPtr);

	const size_t MaxComponentInFov = 64;
	const size_t NewComponentCount = NewComponentList.size();
	if ( 0 == NewComponentCount )//|| NewComponentCount>MaxComponentInFov )
	{
	#ifdef _DEBUG
		JetAPI::DeleteFileList(SaveFileList);
	#endif//_DEBUG
		SaveProjectDropOutPartLog(_T("0 == NewComponentCount"));
		return true;
	}	
	
	if ( CreateProjectDropOutPartByComponentList(FieldPtr, NewComponentList) == false )
	{
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;	
	}	
	if ( SaveProjectSpecTestField(FieldPtr) == false )
	{
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::TestProjectDropOutPartByMapCompare(CAOIField *FieldPtr)
{
	const char fnName[] = "CAOIProject::TestProjectDropOutPartByMapCompare";
	const int TestDropOutPart = GetProjectTestDropOutPart();
	if ( FN_DISABLE == TestDropOutPart ) { return true; }
	
	if ( NULL == FieldPtr ) { return false; }
	if ( FieldPtr->CheckFieldMergeFinish() == false ) { return true; }	
	const int TestFrameIndex = GetProjectParameter().m_DropOutPartFrameIndex;	
	CAOIFrame *FramePtr = FieldPtr->GetFieldFramePtr(TestFrameIndex, true);
	if ( NULL == FramePtr )
	{
		m_ErrorString.Format(_T("Error, Project Test Drop Out Part Frame index Execption [%d]"), TestFrameIndex+1);
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}	

	CString      str;
	bool         bAllocated=false;	
	const int    MapIndex = TestFrameIndex;
	FRAME_TYPE   FrameType = FramePtr->GetFrameType();
	IMAGE_SIZE   ImageW = FramePtr->GetFrameImageW();
	IMAGE_SIZE   ImageH = FramePtr->GetFrameImageH();
	IMAGE_SIZE   ImageStep = FramePtr->GetFrameImageStep();
	IMAGE_SIZE   BitCount = FramePtr->GetFrameImageBitCount();
	IMAGE_PTR    ImagePtr = FramePtr->GetFrameImagePtr();	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const IMAGE_SIZE MaskBitCount = 8;
	const int SaveImage = GetProjectDropOutPartSaveImage();

	const int nAlign = 4;
	const int nTeachMapW = (int)(m_ProjectMapW[MapIndex]);//專案底圖寬度
	const int nTeachMapH = (int)(m_ProjectMapH[MapIndex]);//專案底圖長度
	const int nTeachMapStep = (int)(m_ProjectMapStep[MapIndex]);//專案底圖步長
	const int nTeachMapBitCount = (int)(m_ProjectBitCount[MapIndex]);//專案底圖位元數
	const int nTeachMapW2 = nTeachMapW/2;
	const int nTeachMapH2 = nTeachMapH/2;
	IMAGE_PTR TeachMapPtr = m_ProjectMapPtr[MapIndex];//專案底圖指標

	const int nTeachMaskW = (int)(m_ProjectMapMaskW);
	const int nTeachMaskH = (int)(m_ProjectMapMaskH);
	const int nTeachMaskStep = (int)(m_ProjectMapMaskStep);
	const int nTeachMaskBitCount = (int)(m_ProjectMapMaskBitCount);
	IMAGE_PTR TeachMaskPtr = m_ProjectMapMaskPtr;
	const unsigned int DropOutFrameIndex = GetProjectParameter().m_DropOutPartFrameIndex;
	const unsigned int DropOutFrameUniqueID = GetProjectParameter().m_DropOutPartFrameUniqueID;

	if ( NULL == TeachMapPtr )
	{
		m_ErrorString.Format(_T("Error, Project Test Drop Out Part NULL == TeachMapPtr [%d]"), MapIndex+1);
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}
	if ( FRAME_BAYER == FrameType )
	{		
		IMAGE_SIZE   DeBayerBit=0;
		IMAGE_SIZE   DeBayerStep=0;
		IMAGE_PTR    DeBayerPtr=NULL;	
		BAYER_PATTERN_MODE BayerPattern = FramePtr->GetFrameBayerPattern();		
		if ( AOIDataCollect.ExecDebayerImage(fnName, ImageW, ImageH, ImageStep, ImagePtr, BayerPattern, DeBayerStep, DeBayerBit, DeBayerPtr) == false)		
		{
			m_ErrorString = AOIDataCollect.GetErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
		ImagePtr = DeBayerPtr;
		BitCount = DeBayerBit;
		ImageStep = DeBayerStep;				
		bAllocated = true;
		DeBayerPtr = NULL;
	}
	if ( BitCount != nTeachMapBitCount )
	{
		if ( true == bAllocated )
		{	JetMemory.free_func(ImagePtr); }
		m_ErrorString.Format(_T("Error, Project Test Drop Out Part BitCount != nTeachMapBitCount [%d]"), MapIndex+1);
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}

	const int    NPixels = GetProjectMapScaleMode();
	IMAGE_PTR    CellPtr = NULL;
	IMAGE_SIZE   uCellW = 0;
	IMAGE_SIZE   uCellH = 0;
	IMAGE_SIZE   uCellStep = 0;	

#ifdef _DEBUG
	bool         bSave=true;	
#endif _DEBUG
	CString      Folder;
	CString      MainName;
	CString      KeyName=_T("DropOut");	
	CString      FrameFileName = FramePtr->GetFrameFileName();
	std::vector<CString> SaveFileList;
	Folder = GetProjectDebugFolderDropOut();
	JetAPI::ExtractMainFileName(FrameFileName, MainName);	

	if ( JetMemory.alloc_func(BufferSize, CellPtr, fnName, "CellPtr") == false )
	{
		m_ErrorString = JetMemory.GetErrorString();		
		JetMemory.free_func(CellPtr);		
		if ( true == bAllocated )
		{	JetMemory.free_func(ImagePtr); }
		SaveProjectDropOutPartLog(m_ErrorString);
		return false; 
	}	
	const double ScaleVal = 1.00/NPixels;
	ImageAPI.CalcScaleSize(ImageW, ImageH, ImageStep, BitCount, ScaleVal, uCellW, uCellH, uCellStep);
	if ( ImageAPI.ScaleImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ScaleVal, uCellW, uCellH, uCellStep, CellPtr) == false )
	//if ( ImageAPI.FastScaleImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, NPixels, uCellW, uCellH, uCellStep, CellPtr) == false )	
	{
		JetMemory.free_func(CellPtr);
		if ( true == bAllocated )
		{	JetMemory.free_func(ImagePtr); }
		m_ErrorString = ImageAPI.GetImageApiErrorString();		
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}

#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_Cell.PNG"), Folder, KeyName, MainName);
		//ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtr, true);
		//SaveFileList.push_back(str);
	}
#endif//_DEBUG

	/*
	if ( AOIDataCollect.ExecEnhanceDisplayImage(uCellW, uCellH, uCellStep, BitCount, CellPtr, CellPtr) == false )
	{
		JetMemory.free_func(CellPtr);
		if ( true == bAllocated )
		{	JetMemory.free_func(ImagePtr); }
		m_ErrorString = ImageAPI.GetImageApiErrorString();		
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;	
	}
	*/
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_Cell2.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG
	if ( true == bAllocated )
	{	JetMemory.free_func(ImagePtr); }

	//尋找在專案底圖的基本方位	
	TPOINT2D MapRes;	
	TREGION4D CadRgn;
	TREGION4D CellRgn;
	TREGION4D StageRgn;	
	TREGION4D TeachRgn;
	const double FieldStagePosX = FieldPtr->GetFieldStagePosX();
	const double FieldStagePosY = FieldPtr->GetFieldStagePosY();	
	const double FieldStageSizeW = FieldPtr->GetFieldSizeW_Real();
	const double FieldStageSizeH = FieldPtr->GetFieldSizeH_Real();	
	
	GetProjectMapInfo(MapRes, CadRgn, StageRgn);
	GetProjectMapCalcRgn(TeachRgn);
	TeachRgn = StageRgn;

	CellRgn.minX = FieldStagePosX-(FieldStageSizeW*0.5);
	CellRgn.minY = FieldStagePosY-(FieldStageSizeH*0.5);
	CellRgn.maxX = FieldStagePosX+(FieldStageSizeW*0.5);
	CellRgn.maxY = FieldStagePosY+(FieldStageSizeH*0.5);

	int MapRoiPosX = 0;
	int MapRoiPosY = 0;	
	const double MapTeachCpX = TeachRgn.GetCpX();
	const double MapTeachCpY = TeachRgn.GetCpY();
	const double RgnOffsetX = (FieldStagePosX-MapTeachCpX);
	const double RgnOffsetY = (FieldStagePosY-MapTeachCpY);
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	if ( true == SignX )
	{	MapRoiPosX = (int)(nTeachMapW2+(RgnOffsetX/MapRes.x));	}
	else
	{	MapRoiPosX = (int)(nTeachMapW2-(RgnOffsetX/MapRes.x)); }	
	if ( true == SignY )
	{	MapRoiPosY = (int)(nTeachMapH2+(RgnOffsetY/MapRes.y));	}
	else
	{	MapRoiPosY = (int)(nTeachMapH2-(RgnOffsetY/MapRes.y));	}
	MapRoiPosY = nTeachMapH-MapRoiPosY;	

	bool  ModifyCellRoi=false;
	RECT  MapRoiRect={0};	
	RECT  CellRoiRect={0};
	IMAGE_SIZE RoiExt = 64;
	IMAGE_SIZE MapRoiW = uCellW+RoiExt;
	IMAGE_SIZE MapRoiH = uCellH+RoiExt;
	IMAGE_SIZE MapRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, BitCount, nAlign);
	MapRoiRect.left   = MapRoiPosX-(MapRoiW/2);
	MapRoiRect.top    = MapRoiPosY-(MapRoiH/2);
	MapRoiRect.right  = MapRoiPosX+(MapRoiW/2);
	MapRoiRect.bottom = MapRoiPosY+(MapRoiH/2);	
	ModifyCellRoi = false;
	JetAPI::SizeToRect(uCellW, uCellH, CellRoiRect);	
	if ( MapRoiRect.left < 0 )
	{	
		ModifyCellRoi = true;
		CellRoiRect.left = CellRoiRect.left+(0-MapRoiRect.left);
		MapRoiRect.left = 0;
	}
	if ( MapRoiRect.right > nTeachMapW )
	{
		ModifyCellRoi = true;
		CellRoiRect.right = CellRoiRect.right-(MapRoiRect.right-nTeachMapW);
		MapRoiRect.right = nTeachMapW;		
	}
	if ( MapRoiRect.top < 0 )
	{	
		ModifyCellRoi = true;
		CellRoiRect.top = CellRoiRect.top+(0-MapRoiRect.top);
		MapRoiRect.top = 0;
	}
	if ( MapRoiRect.bottom > nTeachMapH )
	{
		ModifyCellRoi = true;
		CellRoiRect.bottom = CellRoiRect.bottom-(MapRoiRect.bottom-nTeachMapH);
		MapRoiRect.bottom = nTeachMapH;		
	}
	if ( CellRoiRect.right<=CellRoiRect.left || CellRoiRect.bottom<=CellRoiRect.top )
	{
		JetMemory.free_func(CellPtr);
	#ifdef _DEBUG
		JetAPI::DeleteFileList(SaveFileList);
	#endif//_DEBUG
		SaveProjectDropOutPartLog(_T("CellRoiRect.right<=CellRoiRect.left || CellRoiRect.bottom<=CellRoiRect.top"));		
		return true; 			
	}
	if ( true == ModifyCellRoi )
	{
		if ( true == SignX )
		{
			CellRgn.minX += (CellRoiRect.left-0)*MapRes.x;
			CellRgn.maxX -= (uCellW-CellRoiRect.right)*MapRes.x;
		}
		else
		{	
			CellRgn.maxX -= (CellRoiRect.left-0)*MapRes.x;
			CellRgn.minX += (uCellW-CellRoiRect.right)*MapRes.x;		
		}	
		if ( true == SignY )
		{	
			CellRgn.maxY -= (CellRoiRect.top-0)*MapRes.y;
			CellRgn.minY += (uCellH-CellRoiRect.bottom)*MapRes.y;
		}
		else
		{
			CellRgn.minY += (CellRoiRect.top-0)*MapRes.y;
			CellRgn.maxY -= (uCellH-CellRoiRect.bottom)*MapRes.y;
		}

		IMAGE_PTR  CellRoiPtr=NULL;
		IMAGE_SIZE CellRoiW=CellRoiRect.right-CellRoiRect.left;
		IMAGE_SIZE CellRoiH=CellRoiRect.bottom-CellRoiRect.top;
		IMAGE_SIZE CellRoiStep=JetAPI::GetBMPImagePixelsPerLine(CellRoiW, BitCount, nAlign);;
		const size_t CellRoiBuffserSize = ImageAPI.CalcBufferSize(CellRoiStep, CellRoiH);
		if ( JetMemory.alloc_func(CellRoiBuffserSize, CellRoiPtr, fnName, "CellRoiPtr") == false )
		{
			JetMemory.free_func(CellPtr);			
			m_ErrorString = JetMemory.GetErrorString();		
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
		if ( ImageAPI.ExtractRoiImage3(uCellW, uCellH, uCellStep, BitCount, CellPtr, CellRoiRect, CellRoiStep, CellRoiPtr, false) == false )
		{
			JetMemory.free_func(CellPtr);
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
		JetMemory.free_func(CellPtr);
		uCellW = CellRoiW;
		uCellH = CellRoiH;
		uCellStep = CellRoiStep;
		CellPtr = CellRoiPtr;
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_Cell3.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtr, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
		MapRoiW = MapRoiRect.right-MapRoiRect.left;
		MapRoiH = MapRoiRect.bottom-MapRoiRect.top;
		MapRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, BitCount, nAlign);
	}
	
	IMAGE_PTR    MapRoiPtr = NULL;
	const size_t MapRoiBufferSize = ImageAPI.CalcBufferSize(MapRoiStep, MapRoiH);
	if ( JetMemory.alloc_func(MapRoiBufferSize, MapRoiPtr, fnName, "MapRoiPtr") == false )
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);		
		m_ErrorString = JetMemory.GetErrorString();		
		SaveProjectDropOutPartLog(m_ErrorString);
		return false; 
	}		

	if ( ImageAPI.ExtractRoiImage3(nTeachMapW, nTeachMapH, nTeachMapStep, BitCount, TeachMapPtr, MapRoiRect, MapRoiStep, MapRoiPtr, false) == false )
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);		
		m_ErrorString = ImageAPI.GetImageApiErrorString();		
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MapRoi.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG

	IMAGE_PTR    MapMaskRoiPtr = NULL;
	const IMAGE_SIZE MapMaskRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, MaskBitCount, nAlign);
	const size_t MapMaskRoiBufferSize = ImageAPI.CalcBufferSize(MapMaskRoiStep, MapRoiH);
	if ( NULL!=TeachMaskPtr && nTeachMaskW==nTeachMapW && nTeachMaskH==nTeachMapH )
	{
		if ( JetMemory.alloc_func(MapMaskRoiBufferSize, MapMaskRoiPtr, fnName, "MapMaskRoiPtr") == false )
		{
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(MapRoiPtr);
			JetMemory.free_func(MapMaskRoiPtr);			
			m_ErrorString = JetMemory.GetErrorString();	
			SaveProjectDropOutPartLog(m_ErrorString);
			return false; 
		}			
		if ( ImageAPI.ExtractRoiImage3(nTeachMaskW, nTeachMaskH, nTeachMaskStep, MaskBitCount, TeachMaskPtr, MapRoiRect, MapMaskRoiStep, MapMaskRoiPtr, false) == false )
		{
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(MapRoiPtr);
			JetMemory.free_func(MapMaskRoiPtr);			
			m_ErrorString = ImageAPI.GetImageApiErrorString();		
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MapMaskRoi.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiPtr, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
		if (false == CreateProjectColorMaskImage(MapRoiW, MapRoiH, MapMaskRoiStep, MapMaskRoiPtr, MapRoiRect, 0xff, fnName, SaveFileList)) {
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(MapRoiPtr);
			JetMemory.free_func(MapMaskRoiPtr);
			//m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_MapMaskRoi2.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiPtr, true);
			SaveFileList.push_back(str);
		}
#endif//_DEBUG
		if (false == CreateProjectSkewMaskImage(FieldPtr,MapRoiW, MapRoiH, MapMaskRoiStep, MapMaskRoiPtr, MapRoiRect, 0xff, fnName, SaveFileList)) {
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(MapRoiPtr);
			JetMemory.free_func(MapMaskRoiPtr);
			//m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_MapMaskRoi3.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiPtr, true);
			SaveFileList.push_back(str);
		}
#endif//_DEBUG
	}
	else { JetMemory.free_func(MapMaskRoiPtr); }

	//影像對位
	CJetMatch  Match;	
	const bool bRobustness = true;
	const int  nMinReduceArea = 4096;	
	const int  nFinalReduction = 1;//加速用
	const int  nEnableScaleMatch = GetProjectParameter().m_DropOutPartMatchUseScale;	
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();
	if ( Match.SetMatchLibType(MatchLibType)==false )
	{	
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);		
		m_ErrorString = Match.GetErrorString();	
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}

	//initial eMatch
	Match.SetMatchDefaultParam();	
	Match.SetRobustness(bRobustness);
	Match.SetMinReducedArea(nMinReduceArea);
	Match.SetFinalReduction(nFinalReduction);	
	if ( Match.LearnPattern(uCellW, uCellH, uCellStep, BitCount, CellPtr, true) == false )	
	{	
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);		
		m_ErrorString = Match.GetErrorString();
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}
	
	Match.SetInterpolate(true);
	Match.SetMinScore(-1);
	Match.SetMaxPositions(1);
	Match.SetMaxInitialPositions(4);
	if ( Match.Match(MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, true) == false )
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);		
		m_ErrorString = Match.GetErrorString();
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;	
	}
	const int NResults = Match.GetNumPositions();
	if ( 0 == NResults )
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);		
		m_ErrorString = _T("Error, Map Match Fault");
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;	
	}
	const int ResultIdx = 0;
	double ResultX = Match.GetResultPosX(ResultIdx);
	double ResultY = Match.GetResultPosY(ResultIdx);
	double ResultA = Match.GetResultAngle(ResultIdx);
	double ResultS = Match.GetResultScore(ResultIdx)*100.0;
	double ResultSX = Match.GetResultScaleX(ResultIdx);
	double ResultSY = Match.GetResultScaleY(ResultIdx);	
	if ( FN_ENABLE == nEnableScaleMatch )
	{
		const double ResultX_N = Match.GetResultPosX(ResultIdx);
		const double ResultY_N = Match.GetResultPosY(ResultIdx);
		const double ResultA_N = Match.GetResultAngle(ResultIdx);
		const double ResultS_N = Match.GetResultScore(ResultIdx)*100.0;
		const double ResultSX_N = Match.GetResultScaleX(ResultIdx);
		const double ResultSY_N = Match.GetResultScaleY(ResultIdx);

		//再以Scale計算一次
		const float ScaleRange=0.1f;
		const float ScaleMin=1.0f-ScaleRange;
		const float ScaleMax=1.0f+ScaleRange;

		Match.SetMinScale(ScaleMin);
		Match.SetMinScaleX(ScaleMin);
		Match.SetMinScaleY(ScaleMin);
		Match.SetMaxScale(ScaleMax);
		Match.SetMaxScaleX(ScaleMax);
		Match.SetMaxScaleY(ScaleMax);
		Match.SetUseScale(true);
		if ( Match.Match(MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, true) == false )
		{
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(MapRoiPtr);
			JetMemory.free_func(MapMaskRoiPtr);		
			m_ErrorString = Match.GetErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;	
		}
		const int NResults_F = Match.GetNumPositions();			
		if ( NResults_F > 0 )
		{
			double ResultX_F = Match.GetResultPosX(ResultIdx);
			double ResultY_F = Match.GetResultPosY(ResultIdx);
			double ResultA_F = Match.GetResultAngle(ResultIdx);
			double ResultS_F = Match.GetResultScore(ResultIdx)*100.0;
			double ResultSX_F = Match.GetResultScaleX(ResultIdx);
			double ResultSY_F = Match.GetResultScaleY(ResultIdx);		
			//差太多使用沒有內插的, 以及找最相似度最高的
			if ( fabs(ResultX_F-ResultX_N)>10 || fabs(ResultY_F-ResultY_N)>10 || ResultS_F<ResultS_N )
			{
				ResultX = ResultX_N;
				ResultY = ResultY_N;
				ResultA = ResultA_N;
				ResultS = ResultS_N;
				ResultSX = ResultSX_N;
				ResultSY = ResultSY_N;
			}
			else
			{				
				ResultX = ResultX_F;
				ResultY = ResultY_F;
				ResultA = ResultA_F;
				ResultS = ResultS_F;
				ResultSX = ResultSX_F;
				ResultSY = ResultSY_F;	
			}
		}				
	}
	const double MinScore = 50.0;
	const int    nResultX = (int)(ResultX+0.5);
	const int    nResultY = (int)(ResultY+0.5);	
	const double ResultX_Start = nResultX-(uCellW/2);
	const double ResultY_Start = nResultY-(uCellH/2);
	//const double ResultX_Start = ResultX-(uCellW*0.5);
	//const double ResultY_Start = ResultY-(uCellH*0.5);
	//const int    nCellPosX = (int)(ResultX_Start+0.5);
	//const int    nCellPosY = (int)(ResultY_Start+0.5);	
	const int    nCellPosX = JetAPI::Floor(ResultX_Start);
	const int    nCellPosY = JetAPI::Floor(ResultY_Start);
	const int    nCellPosXEnd = nCellPosX+uCellW;
	const int    nCellPosYEnd = nCellPosY+uCellH;	
	if ( nCellPosX<0 || nCellPosY<0 || ResultS<MinScore || nCellPosXEnd>MapRoiW || nCellPosYEnd>MapRoiH )
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);		
		m_ErrorString = _T("Error, Map Match Result Fault");
	#ifdef _DEBUG
		JetAPI::DeleteFileList(SaveFileList);
	#endif//_DEBUG
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;	
	}	

	MASK_PTR     RawMaskPtrL = NULL;	
	MASK_PTR     TestMaskPtr = NULL;
	MASK_PTR     TestMaskPtr1 = NULL;
	MASK_PTR     TestMaskPtr2 = NULL;
	const IMAGE_SIZE MaskW  = uCellW;
	const IMAGE_SIZE MaskH  = uCellH;	
	const IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(MaskW, MaskBitCount, 4);
	const size_t MaskBufferSize = ImageAPI.CalcBufferSize(MaskStep, MaskH);
	if ( JetMemory.alloc_func(MaskBufferSize, RawMaskPtrL, fnName, "RawMaskPtrL") == false || 		 
		 JetMemory.alloc_func(MaskBufferSize, TestMaskPtr, fnName, "TestMaskPtr") == false || 
		 JetMemory.alloc_func(MaskBufferSize, TestMaskPtr1, fnName, "TestMaskPtr1") == false || 
		 JetMemory.alloc_func(MaskBufferSize, TestMaskPtr2, fnName, "TestMaskPtr2") == false )
	{
		m_ErrorString = JetMemory.GetErrorString();		
		JetMemory.free_func(CellPtr);		
		JetMemory.free_func(MapRoiPtr);		
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);		
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);		
		SaveProjectDropOutPartLog(m_ErrorString);
		return false; 
	}

	//剔除此FOV內的零件
	size_t       i=0, j=0;
	TREGION4D    FieldRgn=CellRgn;
	const int    MaskMode=PROJECT_PART_MASK_BODY|PROJECT_CODE_MASK_BODY;
	::memset(TestMaskPtr1, 0xFF, sizeof(MASK_DATA)*MaskBufferSize);
	if ( CreateProjectPartMaskImage_v2(MaskW, MaskH, MaskStep, TestMaskPtr1, FieldRgn, MapRes, MaskMode, 0x00) == false )
	{
		JetMemory.free_func(CellPtr);		
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);		
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);		
		SaveProjectDropOutPartLog(m_ErrorString);
		return false; 
	}		
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskNoPart.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr1, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG
	if ( ImageAPI.MorphGrayImage3(MaskW, MaskH, MaskStep, TestMaskPtr1, MORPH_OPEN, MORPH_SHAPE_RECT, 9, 1, TestMaskPtr2) == false )
	{		
		JetMemory.free_func(CellPtr);	
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);		
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		m_ErrorString = ImageAPI.GetImageApiErrorString();
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskNoPartOpen.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG	
	
	//::memcpy(TestMaskPtr, TestMaskPtr2, sizeof(MASK_DATA)*MaskBufferSize);
	//::memset(TestMaskPtr, 0xFF, sizeof(MASK_DATA)*MaskBufferSize);

	//依據遮罩來比較影像是否有問題
	int    DR=0, DG=0, DB=0, DV=0, Dif=0, DColor=0;
	int    nR1=0, nG1=0, nB1=0, nV1=0, nSum1=0;
	int    nR2=0, nG2=0, nB2=0, nV2=0, nSum2=0;	
	size_t MaskIdx=0;
	size_t MapMaskIdx=0;
	size_t CellIdx=0, RoiIdx=0;
	int    Threshold  =  0;
	double BaseRatio=1.0;
	double DarkRatio=1.0;
	double ColorRatio = 2.0/BaseRatio;
	const int  nDarkLevel = GetProjectParameter().m_DropOutPartDarkLevel*3;//過暗
	const int  nLightLevel = GetProjectParameter().m_DropOutPartLightLevel*3;//過亮
	const int  nTolerance = GetProjectParameter().m_DropOutPartTolerance;	
	IMAGE_PTR  CellPtrRaw=NULL;
	IMAGE_PTR  MapRoiPtrRaw=NULL;
	const size_t CellSize=ImageAPI.CalcBufferSize(uCellStep, uCellH);
	const size_t MapRoiSize=ImageAPI.CalcBufferSize(MapRoiStep, MapRoiH);
	if ( JetMemory.alloc_func(CellSize, CellPtrRaw, fnName, "CellPtrRaw") == false ||
		JetMemory.alloc_func(MapRoiSize, MapRoiPtrRaw, fnName, "MapRoiPtrRaw") == false )
	{
		m_ErrorString = JetMemory.GetErrorString();
		JetMemory.free_func(CellPtr);	
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(CellPtrRaw);	
		JetMemory.free_func(MapRoiPtrRaw);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);		
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}

	Threshold = nTolerance;	
	::memset(RawMaskPtrL, 0x00, sizeof(MASK_DATA)*MaskBufferSize);			
	
	const int SmoothSize = GetProjectParameter().m_DropOutPartSmoothSize;;
	if ( SmoothSize > 0 )
	{
		IMAGE_PTR SwapPtr=NULL;
		const int nSmoothSize = ImageAPI.GetKernelSize(SmoothSize);							
		if ( ImageAPI.SmoothImage3(uCellW, uCellH, uCellStep, BitCount, CellPtr, nSmoothSize, CellPtrRaw) == true )
		{	
			SwapPtr = CellPtrRaw;
			CellPtrRaw = CellPtr;
			CellPtr = SwapPtr;			
		}
		else
		{	::memcpy(CellPtrRaw, CellPtr, sizeof(IMAGE_DATA)*CellSize); }
		
		if ( ImageAPI.SmoothImage3(MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, nSmoothSize, MapRoiPtrRaw) == true )
		{	
			SwapPtr = MapRoiPtrRaw;
			MapRoiPtrRaw = MapRoiPtr;
			MapRoiPtr = SwapPtr;			
		}
		else
		{	::memcpy(MapRoiPtrRaw, MapRoiPtr, sizeof(IMAGE_DATA)*MapRoiSize); }
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_CellSmooth.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtr, true);
			SaveFileList.push_back(str);

			str.Format(_T("%s\\%s_%s_MapRoiSmooth.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
	}
	else
	{
		::memcpy(CellPtrRaw, CellPtr, sizeof(IMAGE_DATA)*CellSize);
		::memcpy(MapRoiPtrRaw, MapRoiPtr, sizeof(IMAGE_DATA)*MapRoiSize);		
	}
	
	const bool bUseCoRelation = false;
	if ( true == bUseCoRelation )
	{
		size_t s=0, t=0;
		double xx=0, yy=0, xy=0, n=0;	
		double sum_x=0, sum_y=0;
		double sum_xy=0, sum_xx=0, sum_yy=0;
		const double  CalcSizeWum = GetProjectParameter().m_DropOutPartCalcSizeW;
		const double  CalcSizeHum = GetProjectParameter().m_DropOutPartCalcSizeH;	
		size_t WndSizeW = (size_t)((CalcSizeWum/MapRes.x)+0.5);
		size_t WndSizeH = (size_t)((CalcSizeHum/MapRes.y)+0.5);
		if ( WndSizeW < 8 ) { WndSizeW = 8; }
		if ( WndSizeH < 8 ) { WndSizeH = 8; }
		const size_t WndSizeW2 = WndSizeW/2;
		const size_t WndSizeH2 = WndSizeH/2;

		int          DummyCount=0;
		const int    DummyGray=8;
		const int    DummyGray2=2*DummyGray;
		const bool   AddDummyGray=true;	
		const int    MaxDummyCount = (int)((WndSizeW*WndSizeH)/8);

		//Using Co-relation Method
		::memset(RawMaskPtrL, 0x00, sizeof(MASK_DATA)*MaskBufferSize);
		if ( 24 == BitCount )
		{
			for ( i=WndSizeH2; i<MaskH-WndSizeH2; i++ )
			{
				for ( j=WndSizeW2; j<MaskW-WndSizeW2; j++ )
				{
					MaskIdx = i*MaskStep+j;
					if ( 0 == TestMaskPtr2[MaskIdx] )
					{	continue;	}
					if ( NULL != MapMaskRoiPtr )
					{				
						MapMaskIdx = ((i+nCellPosY)*MapMaskRoiStep)+(j+nCellPosX);				
						if ( 0 == MapMaskRoiPtr[MapMaskIdx] )
						{	continue;	}
					}

					n = 0;
					sum_x = sum_y = 0;
					sum_xy = sum_xx = sum_yy = 0;
					if ( true == AddDummyGray )				
					{	DummyCount = 0; }
					else
					{	DummyCount = MaxDummyCount+100; }

					CellIdx = (i*uCellStep)+(j*3);
					RoiIdx = ((i+nCellPosY)*MapRoiStep)+(j+nCellPosX)*3;
					nB1 = CellPtr[CellIdx];
					nG1 = CellPtr[CellIdx+1];
					nR1 = CellPtr[CellIdx+2];						

					nB2 = MapRoiPtr[RoiIdx];
					nG2 = MapRoiPtr[RoiIdx+1];
					nR2 = MapRoiPtr[RoiIdx+2];	
					nSum1 = nR1+nG1+nB1;
					nSum2 = nR2+nG2+nB2;
					if ( nSum1<nDarkLevel && nSum2<nDarkLevel )
					{	
						Threshold = nDarkLevel/2;
						DR  = ::abs(nR1-nR2);
						DG  = ::abs(nG1-nG2);
						DB  = ::abs(nB1-nB2);
						if ( DR>Threshold || DG>Threshold || DB>Threshold || DV>Threshold )
						{	RawMaskPtrL[MaskIdx] = 255; }					
						continue;
					}				

					for ( t=i-WndSizeH2; t<i+WndSizeH2; t++ )
					{
						for ( s=j-WndSizeW2; s<j+WndSizeW2; s++ )
						{
							CellIdx = (t*uCellStep)+(s*3);
							RoiIdx = ((t+nCellPosY)*MapRoiStep)+(s+nCellPosX)*3;

							nB1 = CellPtr[CellIdx];
							nG1 = CellPtr[CellIdx+1];
							nR1 = CellPtr[CellIdx+2];						

							nB2 = MapRoiPtr[RoiIdx];
							nG2 = MapRoiPtr[RoiIdx+1];
							nR2 = MapRoiPtr[RoiIdx+2];	

							if ( DummyCount < MaxDummyCount )
							{
								nB1 += DummyGray;
								if ( nB1 > 255 ) { nB1 -= DummyGray2; }

								nG1 += DummyGray;
								if ( nG1 > 255 ) { nG1 -= DummyGray2; }

								nR1 += DummyGray;
								if ( nR1 > 255 ) { nR1 -= DummyGray2; }

								nB2 += DummyGray;
								if ( nB2 > 255 ) { nB2 -= DummyGray2; }

								nG2 += DummyGray;
								if ( nG2 > 255 ) { nG2 -= DummyGray2; }

								nR2 += DummyGray;
								if ( nR2 > 255 ) { nR2 -= DummyGray2; }					 
								DummyCount ++; 
							}
						
							xx = nB1*nB1;
							yy = nB2*nB2;
							xy = nB1*nB2;
							sum_x  = sum_x+nB1;
							sum_y  = sum_y+nB2;
							sum_xy = sum_xy+xy;
							sum_xx = sum_xx+xx;
							sum_yy = sum_yy+yy;
							n ++;

							xx = nG1*nG1;
							yy = nG2*nG2;
							xy = nG1*nG2;
							sum_x  = sum_x+nG1;
							sum_y  = sum_y+nG2;
							sum_xy = sum_xy+xy;
							sum_xx = sum_xx+xx;
							sum_yy = sum_yy+yy;
							n ++;

							xx = nR1*nR1;
							yy = nR2*nR2;
							xy = nR1*nR2;
							sum_x  = sum_x+nR1;
							sum_y  = sum_y+nR2;
							sum_xy = sum_xy+xy;
							sum_xx = sum_xx+xx;
							sum_yy = sum_yy+yy;
							n ++;

						}
					}

					double r = 1.0;
					if ( n > 0 ) 
					{
						double r1 = (n*sum_xy)-(sum_x*sum_y);
						double r2a = ::sqrt((n*sum_xx)-(sum_x*sum_x));
						double r2b = ::sqrt((n*sum_yy)-(sum_y*sum_y));					
						double r2ab = r2a*r2b;
						if ( fabs(r2ab) > 0.0001 )
						{	r = r1/(r2ab);	}
						if ( r > 1.0 ) { r = 1.0; }
						if ( r < 0.0 ) { r = 0.0; }
					}
					else 
					{	r = 0.0; }				
					RawMaskPtrL[MaskIdx] = (unsigned char)((1.0-r)*100.0);
					if ( RawMaskPtrL[MaskIdx] < nTolerance )
					{	RawMaskPtrL[MaskIdx] = 0;	}
					else
					{	RawMaskPtrL[MaskIdx] = 255; }
				}
			}
		}
		else
		{
			for ( i=WndSizeH2; i<MaskH-WndSizeH2; i++ )
			{
				for ( j=WndSizeW2; j<MaskW-WndSizeW2; j++ )
				{
					MaskIdx = i*MaskStep+j;
					if ( 0 == TestMaskPtr2[MaskIdx] )
					{	continue;	}
					if ( NULL != MapMaskRoiPtr )
					{				
						MapMaskIdx = ((i+nCellPosY)*MapMaskRoiStep)+(j+nCellPosX);				
						if ( 0 == MapMaskRoiPtr[MapMaskIdx] )
						{	continue;	}
					}

					n = 0;
					sum_x = sum_y = 0;
					sum_xy = sum_xx = sum_yy = 0;
					if ( true == AddDummyGray )				
					{	DummyCount = 0; }
					else
					{	DummyCount = MaxDummyCount+100; }
					for ( t=i-WndSizeH2; t<i+WndSizeH2; t++ )
					{
						for ( s=j-WndSizeW2; s<j+WndSizeW2; s++ )
						{
							CellIdx = (t*uCellStep)+(s);
							RoiIdx = ((t+nCellPosY)*MapRoiStep)+(s+nCellPosX);

							nR1 = CellPtr[CellIdx];
							nR2 = MapRoiPtr[RoiIdx];
							if ( DummyCount < MaxDummyCount )
							{							
								nR1 += DummyGray;
								if ( nR1 > 255 ) { nR1 -= DummyGray2; }
								nR2 += DummyGray;
								if ( nR2 > 255 ) { nR2 -= DummyGray2; }					 
								DummyCount ++; 
							}

							xx = nR1*nR1;
							yy = nR2*nR2;
							xy = nR1*nR2;
							sum_x  = sum_x+nR1;
							sum_y  = sum_y+nR2;
							sum_xy = sum_xy+xy;
							sum_xx = sum_xx+xx;
							sum_yy = sum_yy+yy;
							n ++;

						}
					}

					double r = 1.0;
					if ( n > 0 ) 
					{
						double r1 = (n*sum_xy)-(sum_x*sum_y);
						double r2a = ::sqrt((n*sum_xx)-(sum_x*sum_x));
						double r2b = ::sqrt((n*sum_yy)-(sum_y*sum_y));					
						double r2ab = r2a*r2b;
						if ( fabs(r2ab) > 0.0001 )
						{	r = r1/(r2ab);	}
						if ( r > 1.0 ) { r = 1.0; }
						if ( r < 0.0 ) { r = 0.0; }
					}
					else 
					{	r = 0.0; }
				
					RawMaskPtrL[MaskIdx] = (unsigned char)((1.0-r)*100.0);
					if ( RawMaskPtrL[MaskIdx] < nTolerance )
					{	RawMaskPtrL[MaskIdx] = 0;	}
					else
					{	RawMaskPtrL[MaskIdx] = 255; }
				}
			}
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
		//	str.Format(_T("%s\\%s_%s_MaskCompareRaw.PNG"), Folder, KeyName, MainName);
		//	ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, RawMaskPtrL, true);
		//	SaveFileList.push_back(str);
		}
	#endif//_DEBUG

		//遮罩濾除
		/*
		for ( i=WndSizeH2; i<MaskH-WndSizeH2; i++ )
		{
			for ( j=WndSizeW2; j<MaskW-WndSizeW2; j++ )
			{
				MaskIdx = i*MaskStep+j;
				if ( 0 == TestMaskPtr2[MaskIdx] )
				{
					RawMaskPtrL[MaskIdx] = 0;
					continue; 
				}
				if ( NULL != MapMaskRoiPtr )
				{				
					MapMaskIdx = ((i+nCellPosY)*MapMaskRoiStep)+(j+nCellPosX);				
					if ( 0 == MapMaskRoiPtr[MapMaskIdx] )
					{
						RawMaskPtrL[MaskIdx] = 0;
						continue;
					}
				}
				if ( RawMaskPtrL[MaskIdx] < ThresholdL )
				{	RawMaskPtrL[MaskIdx] = 0;	}
				else
				{	RawMaskPtrL[MaskIdx] = 255; }
			}
		}
		*/
	}	
	else//No-UseCoRelation
	{	
		//RGBV Compare		
		int nDMax = 0;
		const int MaskMargin=4;
		::memset(RawMaskPtrL, 0x00, sizeof(MASK_DATA)*MaskBufferSize);	
		for ( i=MaskMargin; i<MaskH-MaskMargin; i++ )
		{
			for ( j=MaskMargin; j<MaskW-MaskMargin; j++ )
			{
				MaskIdx = i*MaskStep+j;
				if ( 0 == TestMaskPtr2[MaskIdx] ) { continue; }
				if ( NULL != MapMaskRoiPtr )
				{				
					MapMaskIdx = ((i+nCellPosY)*MapMaskRoiStep)+(j+nCellPosX);				
					if ( 0 == MapMaskRoiPtr[MapMaskIdx] ) { continue; }
				}

				if ( 24 == BitCount )
				{
					CellIdx = (i*uCellStep)+(j*3);
					RoiIdx = ((i+nCellPosY)*MapRoiStep)+(j+nCellPosX)*3;

					nB1 = CellPtr[CellIdx];
					nG1 = CellPtr[CellIdx+1];
					nR1 = CellPtr[CellIdx+2];
					nV1 = 0;

					nB2 = MapRoiPtr[RoiIdx];
					nG2 = MapRoiPtr[RoiIdx+1];
					nR2 = MapRoiPtr[RoiIdx+2];	
					nV2 = 0;					

					nSum1 = nR1+nG1+nB1;
					nSum2 = nR2+nG2+nB2;
					if ( nSum1 > 0 ) 
					{	
						nV1 = nSum1/3;//取平均
						//nV1 = MAX(nR1, nG1);//取最亮
						//nV1 = MAX(nV1, nB1);
						nR1=(int)(nR1*255/nSum1);
						nG1=(int)(nG1*255/nSum1);
						nB1=(int)(nB1*255/nSum1);						
					}
					if ( nSum2 > 0 ) 
					{	
						nV2 = nSum2/3;//取平均
						//nV2 = MAX(nR2, nG2);//取最亮
						//nV2 = MAX(nV2, nB2);
						nR2=(int)(nR2*255/nSum2);
						nG2=(int)(nG2*255/nSum2);
						nB2=(int)(nB2*255/nSum2);						
					}

				#ifdef _DEBUG
					if ( nSum2<20 && nSum1>150 )
					//if ( (nSum1-nSum2) > 100 ) 
					{
						nV2 = nV2;
					}
				#endif//_DEBUG

					if ( nSum1<nDarkLevel && nSum2<nDarkLevel )						
					{	
						DR  = 0;
						DG  = 0;
						DB  = 0;
						DV  = (int)(::abs(nV1-nV2)*DarkRatio);		
						nDMax = DV;
						Threshold = nDarkLevel/2;	
					}
					else if ( nSum1>nLightLevel && nSum2>nLightLevel )
					{	
						DR  = 0;
						DG  = 0;
						DB  = 0;
						DV  = (int)(::abs(nV1-nV2)*DarkRatio);		
						nDMax = DV;
						Threshold = nDarkLevel/2;	
					}
					else
					{	
						DR  = (int)(::abs(nR1-nR2)*ColorRatio);
						DG  = (int)(::abs(nG1-nG2)*ColorRatio);
						DB  = (int)(::abs(nB1-nB2)*ColorRatio);
						DV  = (int)(::abs(nV1-nV2));
						DColor=(DR+DG+DB)/3;
						nDMax = MAX(DR, DG);
						nDMax = MAX(DB, nDMax);
						nDMax = MAX(DV, nDMax);		

						//nDMax = MAX(DV, DColor);
						Threshold = nTolerance; 
					}
					if ( nDMax > 255 )
					{	nDMax = 255; }

					RawMaskPtrL[MaskIdx] = (unsigned char)(nDMax);
					//if ( DR>Threshold || DG>Threshold || DB>Threshold || DV>Threshold )
					//{	RawMaskPtrL[MaskIdx] = 255; }

					//Dif = MAX(DR, DG);
					//Dif = MAX(Dif, DB);
					//TestMaskPtr[MaskIdx] = Dif;
				}
				else
				{
					CellIdx = (i*uCellStep)+(j);
					RoiIdx = ((i+nCellPosY)*MapRoiStep)+(j+nCellPosX);

					nV1 = CellPtr[CellIdx];
					nV2 = MapRoiPtr[RoiIdx];

					Dif = ::abs(nV1-nV2);
					if ( Dif > 255 ) { Dif = 255; }
					RawMaskPtrL[MaskIdx] = Dif;
					//if ( Dif > nTolerance )
					//{	RawMaskPtrL[MaskIdx] = 255; }
				
					//TestMaskPtr[MaskIdx] = Dif;
				}
			}
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MaskCompareRaw.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, RawMaskPtrL, true);
			SaveFileList.push_back(str);		
		}
	#endif//_DEBUG	

		//Remove Edge Line		
		const int nRemoveEdge=GetProjectParameter().m_DropOutPartEdgeRemove;
		if ( FN_ENABLE == nRemoveEdge )
		{	
			const int EdgeErode=3;//侵蝕-Erode
			if ( ImageAPI.ErodeGrayImage3(MaskW, MaskH, MaskStep, RawMaskPtrL, EdgeErode, 1, TestMaskPtr2) == false )
			{
				JetMemory.free_func(CellPtr);	
				JetMemory.free_func(MapRoiPtr);
				JetMemory.free_func(CellPtrRaw);	
				JetMemory.free_func(MapRoiPtrRaw);
				JetMemory.free_func(RawMaskPtrL);			
				JetMemory.free_func(TestMaskPtr);
				JetMemory.free_func(TestMaskPtr1);
				JetMemory.free_func(TestMaskPtr2);
				m_ErrorString = ImageAPI.GetImageApiErrorString();
				SaveProjectDropOutPartLog(m_ErrorString);
				return false;	
			}		
			for ( i=0; i<MaskBufferSize; i++ )
			{
				if ( RawMaskPtrL[i] < TestMaskPtr2[i] ) 
				{
					TestMaskPtr2[i] = 0;
					continue;
				}
				TestMaskPtr2[i] = RawMaskPtrL[i]-TestMaskPtr2[i];

				if ( RawMaskPtrL[i] < TestMaskPtr2[i] )
				{	TestMaskPtr1[i] = 0;}
				else
				{	TestMaskPtr1[i] = RawMaskPtrL[i]-TestMaskPtr2[i]; }
			}
			::memcpy(RawMaskPtrL, TestMaskPtr1, sizeof(MASK_DATA)*MaskBufferSize);
		#ifdef _DEBUG
			if ( true == bSave )
			{
				str.Format(_T("%s\\%s_%s_MaskCompareEdge.PNG"), Folder, KeyName, MainName);
				ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
				SaveFileList.push_back(str);		

				str.Format(_T("%s\\%s_%s_MaskCompareEdgeRmoved.PNG"), Folder, KeyName, MainName);
				ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr1, true);
				SaveFileList.push_back(str);		
			}
		#endif//_DEBUG
		}	

		const int GaussianSize = GetProjectParameter().m_DropOutPartGaussian;	
		if ( GaussianSize > 0 ) 
		{			
			const int nGaussianSize = ImageAPI.GetKernelSize(GaussianSize);
			if ( ImageAPI.GaussianGrayImage3(MaskW, MaskH, MaskStep, RawMaskPtrL, nGaussianSize, TestMaskPtr1) == false )
			{
				JetMemory.free_func(CellPtr);	
				JetMemory.free_func(MapRoiPtr);
				JetMemory.free_func(CellPtrRaw);	
				JetMemory.free_func(MapRoiPtrRaw);
				JetMemory.free_func(RawMaskPtrL);
				JetMemory.free_func(TestMaskPtr);
				JetMemory.free_func(TestMaskPtr1);
				JetMemory.free_func(TestMaskPtr2);
				m_ErrorString = ImageAPI.GetImageApiErrorString();
				SaveProjectDropOutPartLog(m_ErrorString);
				return false;	
			}
			::memcpy(RawMaskPtrL, TestMaskPtr1, sizeof(MASK_DATA)*MaskBufferSize);
		#ifdef _DEBUG
			if ( true == bSave )
			{
				str.Format(_T("%s\\%s_%s_MaskCompareGaussian.PNG"), Folder, KeyName, MainName);
				ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, RawMaskPtrL, true);
				SaveFileList.push_back(str);		
			}
		#endif//_DEBUG
		}
		else
		{	::memcpy(TestMaskPtr1, RawMaskPtrL, sizeof(MASK_DATA)*MaskBufferSize); }

		//Binary		
		for ( i=0; i<MaskBufferSize; i++ )
		{
			if ( TestMaskPtr1[i] < nTolerance ) { RawMaskPtrL[i] = 0; }
			else { RawMaskPtrL[i] = 255; }
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MaskCompareBinary.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, RawMaskPtrL, true);
			SaveFileList.push_back(str);		
		}
	#endif//_DEBUG
	}

	bool KeepImage=false;
#ifndef _DEBUG
	KeepImage = SaveImage;
#endif//_DEBUG
	JetMemory.free_func(CellPtr);	
	JetMemory.free_func(MapRoiPtr);
	if ( false == KeepImage )
	{		
		JetMemory.free_func(CellPtrRaw);	
		JetMemory.free_func(MapRoiPtrRaw);
		JetMemory.free_func(MapMaskRoiPtr);	
	}
	
	//影像處理
	//膨脹-Dilate
	const int DilateSize = GetProjectParameter().m_DropOutPartDilateSize;	
	if ( DilateSize > 0 )
	{	
		const int nDilateSize=ImageAPI.GetKernelSize(DilateSize);
		if ( ImageAPI.DilateGrayImage3(MaskW, MaskH, MaskStep, RawMaskPtrL, nDilateSize, 1, TestMaskPtr1) == false )
		{	
			JetMemory.free_func(CellPtrRaw);	
			JetMemory.free_func(MapRoiPtrRaw);
			JetMemory.free_func(MapMaskRoiPtr);	
			JetMemory.free_func(RawMaskPtrL);			
			JetMemory.free_func(TestMaskPtr);
			JetMemory.free_func(TestMaskPtr1);
			JetMemory.free_func(TestMaskPtr2);
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
		::memcpy(RawMaskPtrL, TestMaskPtr1, sizeof(MASK_DATA)*MaskBufferSize);		
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MaskCompareDilateL.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr1, true);
			SaveFileList.push_back(str);			
		}
	#endif//_DEBUG
	}

	int   MorphMode = 0;
	int   ShapeMode = MORPH_SHAPE_ELLIPSE;//MORPH_SHAPE_RECT, MORPH_SHAPE_CROSS, MORPH_SHAPE_ELLIPSE
	const int NoiseFilterOpen = GetProjectParameter().m_DropOutPartFilterOpenSize;	
	if ( 0 == NoiseFilterOpen ) 
	{	::memcpy(TestMaskPtr, RawMaskPtrL, sizeof(MASK_DATA)*MaskBufferSize);	}
	else
	{		
		MorphMode = MORPH_OPEN;		
		const int OpenSizeL=ImageAPI.GetKernelSize(NoiseFilterOpen);
		if ( ImageAPI.MorphGrayImage3(MaskW, MaskH, MaskStep, RawMaskPtrL, MorphMode, ShapeMode, OpenSizeL, 1, TestMaskPtr) == false )
		{	
			JetMemory.free_func(CellPtrRaw);	
			JetMemory.free_func(MapRoiPtrRaw);
			JetMemory.free_func(MapMaskRoiPtr);	
			JetMemory.free_func(RawMaskPtrL);			
			JetMemory.free_func(TestMaskPtr);
			JetMemory.free_func(TestMaskPtr1);
			JetMemory.free_func(TestMaskPtr2);
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MaskCompareOpenL.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr, true);
			SaveFileList.push_back(str);			
		}
	#endif//_DEBUG
	}

	const int NoiseFilterClose = GetProjectParameter().m_DropOutPartFilterCloseSize;		
	if ( 0 == NoiseFilterClose ) 
	{	::memcpy(TestMaskPtr2, TestMaskPtr, sizeof(MASK_DATA)*MaskBufferSize);	}
	else
	{
		MorphMode = MORPH_CLOSE;
		const int CloseSize = ImageAPI.GetKernelSize(NoiseFilterClose);
		if ( ImageAPI.MorphGrayImage3(MaskW, MaskH, MaskStep, TestMaskPtr, MorphMode, ShapeMode, CloseSize, 1, TestMaskPtr2) == false )	
		{	
			JetMemory.free_func(CellPtrRaw);	
			JetMemory.free_func(MapRoiPtrRaw);
			JetMemory.free_func(MapMaskRoiPtr);	
			JetMemory.free_func(RawMaskPtrL);		
			JetMemory.free_func(TestMaskPtr);
			JetMemory.free_func(TestMaskPtr1);
			JetMemory.free_func(TestMaskPtr2);
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MaskCompareClose.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
	}
	
	//剔除外圍的部分
	const bool ExcludeOutside = false;
	const double FieldStageSizeW_Real = FieldPtr->GetFieldSizeW_Real();
	const double FieldStageSizeH_Real = FieldPtr->GetFieldSizeH_Real();
	const double FieldStageSizeW_Inner = FieldPtr->GetFieldSizeW_Inner();
	const double FieldStageSizeH_Inner = FieldPtr->GetFieldSizeH_Inner();
	if ( true == ExcludeOutside )
	{
		double dMarginW = (FieldStageSizeW_Real-FieldStageSizeW_Inner)/MapRes.x;
		double dMarginH = (FieldStageSizeH_Real-FieldStageSizeH_Inner)/MapRes.y;		
		int MarginW = (int)(dMarginW+0.5);
		int MarginH = (int)(dMarginH+0.5);
		MarginW = MarginW/2;
		MarginH = MarginH/2;
		if ( MarginW>=0 && MarginH>=0 )
		{
			//Top && Bottom
			for ( i=0; i<MarginH; i++ )
			{
				MaskIdx = i*MaskStep;
				::memset(&TestMaskPtr2[MaskIdx], 0x00, sizeof(MASK_DATA)*MaskStep);

				MaskIdx = (MaskH-i-1)*MaskStep;
				::memset(&TestMaskPtr2[MaskIdx], 0x00, sizeof(MASK_DATA)*MaskStep);
			}
			
			//Left & Right
			for ( i=MarginH; i<MaskH-MarginH; i++ )
			{
				//Left
				MaskIdx = i*MaskStep;
				::memset(&TestMaskPtr2[MaskIdx], 0x00, sizeof(MASK_DATA)*MarginW);

				MaskIdx = (i*MaskStep)+MaskW-MarginW;
				::memset(&TestMaskPtr2[MaskIdx], 0x00, sizeof(MASK_DATA)*MarginW);
			}
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MaskExcludeOutside.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
	}
	
	const bool bMeasure = false;
	const double MinThickness=0;
	std::vector<CAOIComponent*> NewComponentList;
	const double PartW = GetProjectParameter().m_DropOutPartMinSizeW;
	const double PartH = GetProjectParameter().m_DropOutPartMinSizeH;
	const double PartWHRatio = GetProjectParameter().m_DropOutPartMaxSizeR;
	if ( CreateProjectComponentByBlob(MaskW, MaskH, MaskStep, TestMaskPtr2, NULL, NULL, FieldRgn, MapRes, PartW, PartH, PartWHRatio, MinThickness, bMeasure, NewComponentList) == false )
	{	
		JetMemory.free_func(CellPtrRaw);	
		JetMemory.free_func(MapRoiPtrRaw);
		JetMemory.free_func(MapMaskRoiPtr);	
		JetMemory.free_func(RawMaskPtrL);		
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);		
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}	
	JetMemory.free_func(RawMaskPtrL);	
	JetMemory.free_func(TestMaskPtr);
	JetMemory.free_func(TestMaskPtr1);
	JetMemory.free_func(TestMaskPtr2);

	const size_t MaxComponentInFov = 36;
	const size_t NewComponentCount = NewComponentList.size();
	if ( 0 == NewComponentCount )//|| NewComponentCount>MaxComponentInFov )
	{
		JetMemory.free_func(CellPtrRaw);	
		JetMemory.free_func(MapRoiPtrRaw);
		JetMemory.free_func(MapMaskRoiPtr);	
	#ifdef _DEBUG
		JetAPI::DeleteFileList(SaveFileList);
	#endif//_DEBUG
		SaveProjectDropOutPartLog(_T("0 == NewComponentCount"));		
		return true;
	}
	
	int  nSaveTxt=SaveImage;
#ifdef _DEBUG
	nSaveTxt=FN_ENABLE;
#endif//_DEBUG
	if ( FN_ENABLE == nSaveTxt )
	{
		FILE *pfile = NULL;
		str.Format(_T("%s\\%s_%s_MatchResult.TXT"), Folder, KeyName, MainName);
		pfile = _tfopen(str, _T("w+"));
		if ( NULL != pfile )
		{
			::_ftprintf(pfile, _T("Result X:%.4f\n"), ResultX);
			::_ftprintf(pfile, _T("Result Y:%.4f\n"), ResultY);
			::_ftprintf(pfile, _T("Result Angle:%.4f\n"), ResultA);
			::_ftprintf(pfile, _T("Result Score:%.4f\n"), ResultS);
			::_ftprintf(pfile, _T("Result Scale X:%.4f\n"), ResultSX);
			::_ftprintf(pfile, _T("Result Scale Y:%.4f\n"), ResultSY);

			::_ftprintf(pfile, _T("Result X Start:%.4f\n"), ResultX_Start);
			::_ftprintf(pfile, _T("Result Y Start:%.4f\n"), ResultY_Start);
			::_ftprintf(pfile, _T("Cell Start Pos X:%d\n"), nCellPosX);
			::_ftprintf(pfile, _T("Cell Start Pos Y:%d\n"), nCellPosY);		
			::fclose(pfile);	pfile = NULL;
		}
	}

#ifndef _DEBUG
	if ( FN_ENABLE == SaveImage )
	{
		str.Format(_T("%s\\%s_%s_MapRoi.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtrRaw, true);
		str.Format(_T("%s\\%s_%s_MapMaskRoi.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiPtr, true);
		str.Format(_T("%s\\%s_%s_Cell3.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtrRaw, true);
	}
#endif//_DEBUG
	JetMemory.free_func(CellPtrRaw);	
	JetMemory.free_func(MapRoiPtrRaw);
	JetMemory.free_func(MapMaskRoiPtr);	

	if ( CreateProjectDropOutPartByComponentList(FieldPtr, NewComponentList) == false )
	{	
		SaveProjectDropOutPartLog(m_ErrorString);
		return false; 
	}	
	if ( SaveProjectSpecTestField(FieldPtr) == false )
	{
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::TestProjectDropOutPartByColorFilter(CAOIField *FieldPtr)
{
	const char fnName[] = "CAOIProject::TestProjectDropOutPartByColorFilter";
	const int TestDropOutPart = GetProjectTestDropOutPart();
	if ( FN_DISABLE == TestDropOutPart ) { return true; }
	
	if ( NULL == FieldPtr ) { return false; }
	if ( FieldPtr->CheckFieldMergeFinish() == false ) { return true; }
	//const int TestFrameIndex = 1;//暫時使用Low燈源
	const int TestFrameIndex = 2;//使用高角度燈
	CAOIFrame *FramePtr = FieldPtr->GetFieldFramePtr(TestFrameIndex, true);
	if ( NULL == FramePtr )	{	return true; }

	const size_t ColorIndex = PROJECT_COLOR_ID_BOARD_BEGIN+1;
	CColorGroup *ColorGroupPtr = GetProjectColorGroupPtr(ColorIndex, true);
	if ( NULL == ColorGroupPtr ) { return false; }

	CString      str;
#ifdef _DEBUG
	bool         bSave=true;
#endif//_DEBUG
	CString      Folder;
	CString      MainName;
	CString      KeyName=_T("DropOut");	
	CString      FrameFileName = FramePtr->GetFrameFileName();
	JetAPI::ExtractMainFileName(FrameFileName, MainName);
	Folder = GetProjectDebugFolderDropOut();
	
	const int    nAlign = 4;
	bool         bAllocated = false;	
	FRAME_TYPE   FrameType = FramePtr->GetFrameType();
	IMAGE_SIZE   ImageW = FramePtr->GetFrameImageW();
	IMAGE_SIZE   ImageH = FramePtr->GetFrameImageH();
	IMAGE_SIZE   ImageStep = FramePtr->GetFrameImageStep();
	IMAGE_SIZE   BitCount = FramePtr->GetFrameImageBitCount();
	IMAGE_PTR    ImagePtr = FramePtr->GetFrameImagePtr();
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);

	if ( FRAME_BAYER == FrameType )
	{		
		IMAGE_SIZE   DeBayerBit=0;
		IMAGE_SIZE   DeBayerStep=0;
		IMAGE_PTR    DeBayerPtr=NULL;	
		BAYER_PATTERN_MODE BayerPattern = FramePtr->GetFrameBayerPattern();		
		if ( AOIDataCollect.ExecDebayerImage(fnName, ImageW, ImageH, ImageStep, ImagePtr, BayerPattern, DeBayerStep, DeBayerBit, DeBayerPtr) == false)		
		{
			m_ErrorString = AOIDataCollect.GetErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
		ImagePtr = DeBayerPtr;
		BitCount = DeBayerBit;
		ImageStep = DeBayerStep;
		bAllocated = true;
		DeBayerPtr = NULL;
	}

	bool         IsOK = true;
	RECT         RoiRect={0, 0, 0, 0};
	MASK_PTR     TestMaskPtr = NULL;
	MASK_PTR     TestMaskPtr2 = NULL;	
	const IMAGE_SIZE MaskBitCount = 8;
	const IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, MaskBitCount, 4);
	const size_t MaskBufferSize = ImageAPI.CalcBufferSize(MaskStep, ImageH);

	if ( JetMemory.alloc_func(BufferSize, TestMaskPtr, fnName, "TestMaskPtr") == false ||
		JetMemory.alloc_func(BufferSize, TestMaskPtr2, fnName, "TestMaskPtr2") == false )
	{
		m_ErrorString = JetMemory.GetErrorString();		
		JetMemory.free_func(TestMaskPtr);		
		JetMemory.free_func(TestMaskPtr2);
		if ( true == bAllocated )
		{	JetMemory.free_func(ImagePtr);	}
		SaveProjectDropOutPartLog(m_ErrorString);
		return false; 
	}	
	
	//基板抽色	
	const bool bOpenMP = false;
	JetAPI::SizeToRect(ImageW, ImageH, RoiRect);
	if ( 24 == BitCount )
	{	IsOK = ImageAPI.ColorImageColorFilter3(ImageW, ImageH, ImageStep, ImagePtr, *ColorGroupPtr, RoiRect, MaskStep, TestMaskPtr, true, bOpenMP); }
	else
	{	IsOK = ImageAPI.RGBImageColorFilter3(ImageW, ImageH, ImageStep, ImagePtr, ImagePtr, ImagePtr, *ColorGroupPtr, RoiRect, MaskStep, TestMaskPtr, true); }
	if ( false == IsOK )
	{
		m_ErrorString = ImageAPI.GetImageApiErrorString();		
		JetMemory.free_func(TestMaskPtr);		
		JetMemory.free_func(TestMaskPtr2);
		if ( true == bAllocated )
		{	JetMemory.free_func(ImagePtr);	}
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_Raw.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
		str.Format(_T("%s\\%s_%s_MaskBoard.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, ImageW, ImageH, MaskStep, MaskBitCount, TestMaskPtr, true);
	}
#endif//_DEBUG

	//抽色反向
	ImageAPI.InvertMaskImage3(ImageW, ImageH, MaskStep, TestMaskPtr, RoiRect, MaskStep, TestMaskPtr);
	if ( false == IsOK )
	{
		m_ErrorString = ImageAPI.GetImageApiErrorString();				
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr2);
		if ( true == bAllocated )
		{	JetMemory.free_func(ImagePtr);	}
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}	
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskInvert.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, ImageW, ImageH, MaskStep, MaskBitCount, TestMaskPtr, true);
	}
#endif//_DEBUG

	if ( true == bAllocated )
	{	JetMemory.free_func(ImagePtr);	}
	//剔除此FOV內的零件
	size_t       i=0;	
	TREGION4D    FieldRgn;	
	TPOINT2D     ImageRes;
	const double FieldStagePosX = FieldPtr->GetFieldStagePosX();
	const double FieldStagePosY = FieldPtr->GetFieldStagePosY();	
	const double FieldStageSizeW = FieldPtr->GetFieldSizeW_Real();
	const double FieldStageSizeH = FieldPtr->GetFieldSizeH_Real();	
	const double ImageCpX = ImageW*0.5;
	const double ImageCpY = ImageH*0.5;
	ImageRes.x = (double)(FieldStageSizeW/ImageW);
	ImageRes.y = (double)(FieldStageSizeH/ImageH);	
	FieldRgn.minX = FieldStagePosX-(FieldStageSizeW*0.5);
	FieldRgn.minY = FieldStagePosY-(FieldStageSizeH*0.5);
	FieldRgn.maxX = FieldStagePosX+(FieldStageSizeW*0.5);
	FieldRgn.maxY = FieldStagePosY+(FieldStageSizeH*0.5);	
	const int    MaskMode=PROJECT_PART_MASK_BODY|PROJECT_CODE_MASK_BODY;
	//::memset(TestMaskPtr, 0xFF, sizeof(MASK_DATA)*MaskStep*ImageH);
	if ( CreateProjectPartMaskImage_v2(ImageW, ImageH, MaskStep, TestMaskPtr, FieldRgn, ImageRes, MaskMode, 0x00) == false )
	{
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr2);
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}	
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskNoPart.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, ImageW, ImageH, MaskStep, MaskBitCount, TestMaskPtr, true);
	}
#endif//_DEBUG

	//影像處理
	int    MorphMode = MORPH_OPEN;
	int    ShapeMode = MORPH_SHAPE_ELLIPSE;//MORPH_SHAPE_RECT, MORPH_SHAPE_CROSS, MORPH_SHAPE_ELLIPSE
	const size_t OpenSize=31;
	
	MorphMode = MORPH_OPEN;
	if ( ImageAPI.MorphGrayImage3(ImageW, ImageH, MaskStep, TestMaskPtr, MorphMode, ShapeMode, OpenSize, 1, TestMaskPtr2) == false )
	{		
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr2);
		m_ErrorString = ImageAPI.GetImageApiErrorString();
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskOpen.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, ImageW, ImageH, MaskStep, MaskBitCount, TestMaskPtr2, true);
	}
#endif//_DEBUG

	MorphMode = MORPH_CLOSE;
	const size_t CloseSize = 31;		
	if ( ImageAPI.MorphGrayImage3(ImageW, ImageH, MaskStep, TestMaskPtr2, MorphMode, ShapeMode, CloseSize, 1, TestMaskPtr) == false )
	{		
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr2);
		m_ErrorString = ImageAPI.GetImageApiErrorString();
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskClose.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, ImageW, ImageH, MaskStep, MaskBitCount, TestMaskPtr, true);
	}
#endif//_DEBUG

	const bool bMeasure = false;
	const double MinThickness=0;
	std::vector<CAOIComponent*> NewComponentList;
	const double PartW = GetProjectParameter().m_DropOutPartMinSizeW;
	const double PartH = GetProjectParameter().m_DropOutPartMinSizeH;
	const double PartWHRatio = GetProjectParameter().m_DropOutPartMaxSizeR;
	if ( CreateProjectComponentByBlob(ImageW, ImageH, MaskStep, TestMaskPtr, NULL, NULL, FieldRgn, ImageRes, PartW, PartH, PartWHRatio, MinThickness, bMeasure, NewComponentList) == false )
	{
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr2);		
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}
	JetMemory.free_func(TestMaskPtr);
	JetMemory.free_func(TestMaskPtr2);	

	if ( CreateProjectDropOutPartByComponentList(FieldPtr, NewComponentList) == false )
	{
		SaveProjectDropOutPartLog(m_ErrorString);
		return false; 
	}	
	if ( SaveProjectSpecTestField(FieldPtr) == false )
	{
		SaveProjectDropOutPartLog(m_ErrorString);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::TestProjectDropOutPartByMap()//利用底圖定位並且判定
{
	return true;//以下方式無效益
	const int TestDropOutPart = GetProjectTestDropOutPart();
	if ( FN_DISABLE == TestDropOutPart ) { return true; }
	if ( CheckProjectSaveProjectTestMap() == false ) 
	{	return true; }
	
	const int nAlign = 4;
	const int MapIndex = 1; //
	const int nTestMapW = (int)(m_ProjectTestMapW[MapIndex]);
	const int nTestMapH = (int)(m_ProjectTestMapH[MapIndex]);
	const int nTestMapStep = (int)(m_ProjectTestMapStep[MapIndex]);
	const int nTestMapBitCount = (int)(m_ProjectTestBitCount[MapIndex]);
	const int nTestMapW2 = nTestMapW/2;
	const int nTestMapH2 = nTestMapH/2;
	IMAGE_PTR TestMapPtr = (m_ProjectTestMapPtr[MapIndex]);
	
	const int nTeachMapW = (int)(m_ProjectMapW[MapIndex]);//專案底圖寬度
	const int nTeachMapH = (int)(m_ProjectMapH[MapIndex]);//專案底圖長度
	const int nTeachMapStep = (int)(m_ProjectMapStep[MapIndex]);//專案底圖步長
	const int nTeachMapBitCount = (int)(m_ProjectBitCount[MapIndex]);//專案底圖位元數
	const int nTeachMapW2 = nTeachMapW/2;
	const int nTeachMapH2 = nTeachMapH/2;
	IMAGE_PTR TeachMapPtr = m_ProjectMapPtr[MapIndex];//專案底圖指標

#ifdef _DEBUG
	CString str;
	BOOL   bSave = TRUE;
#endif//_DEBUG

	if ( nTestMapBitCount != nTeachMapBitCount )
	{
		m_ErrorString = _T("Error, nTestMapBitCount != nTeachMapBitCount");
		return false;
	}
	if ( NULL==TestMapPtr || NULL==TeachMapPtr )
	{
		m_ErrorString = _T("Error, NULL==TestMapPtr || NULL==TeachMapPtr");
		return false;
	}

	RECT      PatRect={0};
	IMAGE_PTR PatPtr = NULL;
	const int nPatW = MIN(nTeachMapW, nTestMapW);
	const int nPatH = MIN(nTeachMapH, nTestMapH);	
	const int nPatBitCount = nTeachMapBitCount;	
	const int nPatStep = JetAPI::GetBMPImagePixelsPerLine(nPatW, nPatBitCount, nAlign);
	const size_t nPatBufferSize = ImageAPI.CalcBufferSize(nPatStep, nPatH);
	if ( JetMemory.alloc_func(nPatBufferSize, PatPtr, "CAOIProject::TestProjectDropOutPartByMap", "PatPtr") == false )
	{
		m_ErrorString = JetMemory.GetErrorString();
		return false;
	}	
	PatRect.left = 0;
	PatRect.top = 0;
	PatRect.right = nPatW;
	PatRect.bottom = nPatH;
	if ( ImageAPI.ExtractRoiImage3(nTestMapW, nTestMapH, nTestMapStep, nTestMapBitCount, TestMapPtr, PatRect, nPatStep, PatPtr, false) == false )
	{
		m_ErrorString = JetMemory.GetErrorString();
		return false;
	}
#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("TestRoi.PNG"));
		ImageAPI.SaveImage(str, nPatW, nPatH, nPatStep, nPatBitCount, PatPtr, true);
	}
#endif//_DEBUG
	//底圖定位	
	CJetMatch  Match;
	RECT       AlignRect={0,0,0,0};
	const bool bRobustness = true;
	const int  nMinReduceArea = 2048;
	const int  nFinalReduction = 1;
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();
	if ( Match.SetMatchLibType(MatchLibType)==false )
	{
		JetMemory.free_func(PatPtr);
		m_ErrorString = Match.GetErrorString();
		return false;
	}
	
	//使用內部4分之一來定位
	AlignRect.left = nTeachMapW2-(nTeachMapW2/2);
	AlignRect.top = nTeachMapH2-(nTeachMapH2/2);
	AlignRect.right = AlignRect.left+(nTeachMapW2);
	AlignRect.bottom = AlignRect.top+(nTeachMapH2);

	//initial eMatch
	Match.SetMatchDefaultParam();	
	Match.SetRobustness(bRobustness);
	Match.SetMinReducedArea(nMinReduceArea);
	Match.SetFinalReduction(nFinalReduction);	
	if ( Match.LearnPattern(nPatW, nPatH, nPatStep, nPatBitCount, PatPtr, true) == false )	
	{	
		JetMemory.free_func(PatPtr);
		m_ErrorString = Match.GetErrorString();
		return false;
	}
	
	Match.SetInterpolate(true);
	Match.SetMinScore(-1);
	Match.SetMaxPositions(1);
	Match.SetMaxInitialPositions(4);
	if ( Match.Match(nTeachMapW, nTeachMapH, nTeachMapStep, nTeachMapBitCount, TeachMapPtr, true) == false )
	{
		JetMemory.free_func(PatPtr);
		m_ErrorString = Match.GetErrorString();
		return false;	
	}
	JetMemory.free_func(PatPtr);
	const int NResults = Match.GetNumPositions();
	if ( 0 == NResults )
	{
		m_ErrorString = _T("Error, Map Match Fault");
		return false;	
	}
	const int ResultIdx = 0;
	const double ResultX = Match.GetResultPosX(ResultIdx);
	const double ResultY = Match.GetResultPosY(ResultIdx);
	const double ResultA = Match.GetResultAngle(ResultIdx);
	const double ResultS = Match.GetResultScore(ResultIdx)*100.0;
	const double ResultSX = Match.GetResultScaleX(ResultIdx);
	const double ResultSY = Match.GetResultScaleY(ResultIdx);

#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		FILE *pfile = NULL;
		TCHAR TMode[32] = _T("");
		_tcscpy(TMode, _T("w+"));
		JetAPI::ModifyOpenFileMode_Write(TMode);
		str.Format(_T("%s\\MapMatch.Txt"), AOIDataCollect.GetAOITempDirectory());
		pfile = _tfopen(str, TMode);
		if ( NULL != pfile )
		{
			::_ftprintf(pfile, _T("X, Y, A, Score, SX, SY\n"));
			::_ftprintf(pfile, _T("%.2f, %.2f, %.2f, %.2f, %.2f, %.2f\n"), ResultX, ResultY, ResultA, ResultS, ResultSX, ResultSY);
			::fclose(pfile);	pfile = NULL;
		}
	}
#endif//_DEBUG

	int       i=0, j=0;
	int       idx1=0, idx2=0, idx=0;
	IMAGE_PTR TestMaskPtr = NULL;
	const int  nMaskBitCount = 8;
	const int  nTestMaskStep = JetAPI::GetBMPImagePixelsPerLine(nPatW, nMaskBitCount, nAlign);
	const size_t TestMaskBufferSize = ImageAPI.CalcBufferSize(nTestMaskStep, nPatH);
	if ( JetMemory.alloc_func(TestMaskBufferSize, TestMaskPtr, "CAOIProject::TestProjectDropOutPartByMap", "TestMaskPtr") == false )
	{
		m_ErrorString = JetMemory.GetErrorString();
		return false;
	}
	::memset(TestMaskPtr, 0xFF, sizeof(IMAGE_DATA)*TestMaskBufferSize);
	//建立檢測的遮罩
	for ( i=0; i<nPatH; i++ )
	{		
		idx1 = i*nTestMaskStep;
		idx2 = i*nTestMapStep;
		for ( j=0; j<nPatW; j++ )
		{	
			if ( 24 != nTestMapBitCount )
			{	idx=j; }
			else
			{	idx=j*3;}
			if ( 0 < TestMapPtr[idx2+idx] ) { continue; }
			if ( 0 < TestMapPtr[idx2+idx+1] ) { continue; }
			if ( 0 < TestMapPtr[idx2+idx+2] ) { continue; }
			TestMaskPtr[idx1+j] = 0x00;
		}
	}	
#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("TestRoi.PNG"));
		ImageAPI.SaveImage(str, nPatW, nPatH, nTestMaskStep, nMaskBitCount, TestMaskPtr, true);
	}
#endif//_DEBUG
	JetMemory.free_func(TestMaskPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
size_t  CAOIProject::GetProjectScratchPartCount() const//取得專案刮傷數量
{
	return GetProjectScratchPartCount_Inline();
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectScratchPartPtr(size_t index, bool check) const//取得專案刮傷指標
{
	if ( check )
	{
		const size_t count = GetProjectScratchPartCount_Inline();
		if ( index >= count )
		{	return NULL; }
	}
	return CAOIProject::GetProjectScratchPartPtr_Inline(index);	
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::AddProjectScratchPartPtr(CAOIComponent *ComponentPtr, bool clone)//增加專案刮傷
{
	if ( NULL == ComponentPtr )
	{
		this->m_ErrorString.Format(_T("Error, AddProjectScratchPartPtr Fault"));
		return NULL;
	}

	CAOIComponent *NewComponentPtr = ComponentPtr;
	const unsigned int ComponentIndex = (unsigned int)(GetProjectScratchPartCount_Inline());
	if ( true == clone )
	{	
		NewComponentPtr = ComponentPtr->CloneComponentObj();
		if ( NewComponentPtr == NULL ) { return NULL; }		
		NewComponentPtr->SetComponentIndex_Project(ComponentIndex);			
	}	
	else
	{	NewComponentPtr->SetComponentIndex_Project(ComponentIndex);	 }

	NewComponentPtr->SetComponentUniqueID(ComponentIndex);	
	NewComponentPtr->SetComponentProjectPtr(this);	
	AddProjectScratchPartPtr_Inline(NewComponentPtr);		
	return NewComponentPtr;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ClearProjectAllScratchParts()//刪除專案刮傷
{
	size_t i = 0;	
	CAOIComponent *ComponentPtr = NULL;	
	const size_t ComponentCount = GetProjectScratchPartCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectScratchPartPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		AOIObjManager.DestroyComponentObj(m_ProjectScratchPartList[i]);
		ComponentPtr = NULL;		
	}		
	this->m_ProjectScratchPartList.clear();	
	return true;
}
//---------------------------------------------------------------------------------//
int CAOIProject::GetProjectTestScratchPart() const//檢測刮傷-啟用
{
	return m_ProjectParameter.m_ScratchPartEnable; 
}
//-------------------------------------------------------------------------------------//
void CAOIProject::SetProjectTestScratchPart(int val)//檢測刮傷-啟用
{
	m_ProjectParameter.m_ScratchPartEnable = val;
}
//-------------------------------------------------------------------------------------//
int CAOIProject::GetProjectScratchPartSaveImage() const//檢測刮傷-存圖
{
	return m_ProjectParameter.m_ScratchPartSaveImage;
}
//-------------------------------------------------------------------------------------//
void CAOIProject::SetProjectScratchPartSaveImage(int val)//檢測刮傷-存圖		
{
	m_ProjectParameter.m_ScratchPartSaveImage = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::BuildProjectScratchComponentList()//合併程專案刮傷列表
{
	size_t          i=0, j=0, k=0;
	size_t          ModelWndCount=0;
	bool            bModifyRgn=false;
	double          BodySizeW=0;
	double          BodySizeH=0;		
	CString         ComponentFullName;
	CString         ComponentImageName;
	double          ComponentCadPosX=0;
	double          ComponentCadPosY=0;
	double          ComponentStagePosX=0;
	double          ComponentStagePosY=0;	
	TREGION4D       ComponentRgn1;
	TREGION4D       ComponentRgn2;
	CAOIWnd        *WndPtr = NULL;
	CAOIModel      *ModelPtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;
	CAOIComponent  *ComponentPtr2 = NULL;		
	DISTRICT_ID     DistrictID = GetProjectActDistrictID();
	CAOIPanel *PanelPtr = GetProjectPanelPtr(0, true);
	if ( NULL == PanelPtr ) { return true; }
	CAOIBoard *BoardPtr = PanelPtr->GetPanelBoardPtr(0, true);
	if ( NULL == BoardPtr ) { return true; }	
	
	CMapCoordinate *STCMapPtr = BoardPtr->GetBoardMapSTCPtr(DistrictID);
	CMapCoordinate *CTSMapPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);
	std::vector<CAOIComponent*>    ProjectScratchPartList=m_ProjectScratchPartList;//專案拋件指標列表	
	
	m_ProjectScratchPartList.clear();		
	CString   SpcImageFolder = GetProjectSpcImageFolder();	
	const size_t    ScratchCount = ProjectScratchPartList.size();		
	const size_t    FrameUniqueIDCount = GetProjectFrameUniqueIDCount();

	if ( GetProjectUseLocalFolder() )
	{	SpcImageFolder = GetProjectSpcImageFolderLocal();	}

	for ( i=0; i<ScratchCount; i++ )
	{
		ComponentPtr = ProjectScratchPartList[i];
		if ( NULL == ComponentPtr ) { continue; }		
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		bModifyRgn=false;
		ComponentPtr->GetComponentBodyStageRegion(ComponentRgn1);
		for ( j=i+1; j<ScratchCount; j++ )
		{
			ComponentPtr2 = ProjectScratchPartList[j];
			if ( NULL == ComponentPtr2 ) { continue; }
			ComponentPtr2->GetComponentBodyStageRegion(ComponentRgn2);
			if ( ComponentRgn2.minX > ComponentRgn1.maxX ) { continue; }
			if ( ComponentRgn2.minY > ComponentRgn1.maxY ) { continue; }
			if ( ComponentRgn2.maxX < ComponentRgn1.minX ) { continue; }
			if ( ComponentRgn2.maxY < ComponentRgn1.minY ) { continue; }
			bModifyRgn = true;
			//剔除重複的零件
			ComponentFullName = ComponentPtr2->GetComponentFullName();
			for ( k=0; k<FrameUniqueIDCount; k++ )
			{
				ComponentImageName.Format(_T("%s\\%s#%d.JPG"), SpcImageFolder, ComponentFullName, k+1);
				::DeleteFile(ComponentImageName);
				ComponentImageName.Format(_T("%s\\%s#%d.Z3D"), SpcImageFolder, ComponentFullName, k+1);
				::DeleteFile(ComponentImageName);
			}
			AOIObjManager.DestroyComponentObj(ProjectScratchPartList[j]);
			ProjectScratchPartList[j] = NULL;
			JetAPI::UnionRegion(ComponentRgn1, ComponentRgn2, ComponentRgn1);
		}	
		if ( true == bModifyRgn )
		{
			BodySizeW = ComponentRgn1.GetWidth();
			BodySizeH = ComponentRgn1.GetHeight();
			ComponentStagePosX = ComponentRgn1.GetCpX();
			ComponentStagePosY = ComponentRgn1.GetCpY();
			
			ComponentCadPosX = ComponentPtr->GetComponentCadPosX();
			ComponentCadPosY = ComponentPtr->GetComponentCadPosY();
			STCMapPtr->Map2D(ComponentStagePosX, ComponentStagePosY, ComponentCadPosX, ComponentCadPosY);

			ComponentPtr->SetComponentBodySizeW(BodySizeW);
			ComponentPtr->SetComponentBodySizeH(BodySizeH);
			ComponentPtr->SetComponentCadPosX(ComponentCadPosX);
			ComponentPtr->SetComponentCadPosY(ComponentCadPosY);
			ComponentPtr->SetComponentStagePosX(ComponentStagePosX);
			ComponentPtr->SetComponentStagePosY(ComponentStagePosY);		
			ComponentPtr->SetComponentRoiSizeW(BodySizeW);
			ComponentPtr->SetComponentRoiSizeH(BodySizeH);
			ComponentPtr->CalcComponentCadCornerPos();
			ComponentPtr->LayoutComponentStageCornerPos();

			ModelPtr = ComponentPtr->GetComponentModelPtr();
			ModelWndCount = ModelPtr->GetModelWndCount();
			for ( j=0; j<ModelWndCount; j++ )
			{
				WndPtr = ModelPtr->GetModelWndPtr(j, false);
				if ( NULL == WndPtr ) { continue; }
				WndPtr->GetWndBox().SetBoxSize(BodySizeW, BodySizeH);				
			}
			ModelPtr->GetModelBodyBox().SetBoxSize(BodySizeW, BodySizeH);
			ModelPtr->SetModelAttachedPosCad(ComponentCadPosX, ComponentCadPosY);
			ModelPtr->SetModelAttachedPosStage(ComponentStagePosX, ComponentStagePosY);
		}	
		AddProjectScratchPartPtr_Inline(ComponentPtr);		
	}	
	const size_t    DropOutCount2 = m_ProjectScratchPartList.size();	
	if ( DropOutCount2 != ScratchCount )
	{	i = 3;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::SaveProjectScratchPartLog(LPCTSTR str)
{
	CString msg;
	msg.Format(_T("SCRATCH::%s"), str);
	AOIDataCollect.SaveMovingTimeMsg(msg);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::TestProjectScratchPart(CAOIField *FieldPtr)
{
	if ( TestProjectScratchPartFn(FieldPtr) == false )
	{
		SetProjectExceptionCode(AOI_EXCEPTION_PROJECT_TEST_SCRATCH);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::TestProjectScratchPartFn(CAOIField *FieldPtr)
{
#define SCRATCH_ALAN
#ifdef SCRATCH_ALAN
	//return TestProjectScratchPartByColorFilter_v3(FieldPtr);
	bool isok = TestProjectScratchPart_v4(FieldPtr);
	//bool isok = TestProjectScratchPart_v2(FieldPtr);
	return isok;
#else 
	return TestProjectScratchPartByColorFilter(FieldPtr);
#endif // SCRATCH_ALAN		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::TestProjectScratchPartByColorFilter(CAOIField *FieldPtr)//檢測刮傷-使用抽色
{
	const char fnName[] = "CAOIProject::TestProjectScratchPartByColorFilter";
	const int TestScratchPart = GetProjectTestScratchPart();
	if ( FN_DISABLE == TestScratchPart ) { return true; }

	if ( NULL == FieldPtr ) { return false; }
	if ( FieldPtr->CheckFieldMergeFinish() == false ) { return true; }	
	const int TestFrameIndex = GetProjectParameter().m_ScratchPartFrameIndex;	
	CAOIFrame *FramePtr = FieldPtr->GetFieldFramePtr(TestFrameIndex, true);
	if ( NULL == FramePtr )
	{
		m_ErrorString.Format(_T("Error, Project Test Scratch Part Frame index Execption [%d]"), TestFrameIndex+1);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	
	CString      str;
	bool         bAllocated=false;
	const int    MapIndex = TestFrameIndex;
	FRAME_TYPE   FrameType = FramePtr->GetFrameType();
	IMAGE_SIZE   ImageW = FramePtr->GetFrameImageW();
	IMAGE_SIZE   ImageH = FramePtr->GetFrameImageH();
	IMAGE_SIZE   ImageStep = FramePtr->GetFrameImageStep();
	IMAGE_SIZE   BitCount = FramePtr->GetFrameImageBitCount();
	IMAGE_PTR    ImagePtr = FramePtr->GetFrameImagePtr();
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const IMAGE_SIZE MaskBitCount = 8;
	const int SaveImage = GetProjectScratchPartSaveImage();	

	const int nAlign = 4;
	const int nTeachMapW = (int)(m_ProjectMapW[MapIndex]);//專案底圖寬度
	const int nTeachMapH = (int)(m_ProjectMapH[MapIndex]);//專案底圖長度
	const int nTeachMapStep = (int)(m_ProjectMapStep[MapIndex]);//專案底圖步長
	const int nTeachMapBitCount = (int)(m_ProjectBitCount[MapIndex]);//專案底圖位元數
	const int nTeachMapW2 = nTeachMapW/2;
	const int nTeachMapH2 = nTeachMapH/2;
	IMAGE_PTR TeachMapPtr = m_ProjectMapPtr[MapIndex];//專案底圖指標

	const int nTeachMaskW = (int)(m_ProjectMapMaskW);
	const int nTeachMaskH = (int)(m_ProjectMapMaskH);
	const int nTeachMaskStep = (int)(m_ProjectMapMaskStep);
	const int nTeachMaskBitCount = (int)(m_ProjectMapMaskBitCount);
	IMAGE_PTR TeachMaskPtr = m_ProjectMapMaskPtr;	

	if ( NULL == TeachMapPtr )
	{
		m_ErrorString.Format(_T("Error, Project Test Scratch Part NULL == TeachMapPtr [%d]"), MapIndex+1);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	if ( FRAME_BAYER == FrameType )
	{		
		IMAGE_SIZE   DeBayerBit=0;
		IMAGE_SIZE   DeBayerStep=0;
		IMAGE_PTR    DeBayerPtr=NULL;
		BAYER_PATTERN_MODE BayerPattern = FramePtr->GetFrameBayerPattern();		
		if ( AOIDataCollect.ExecDebayerImage(fnName, ImageW, ImageH, ImageStep, ImagePtr, BayerPattern, DeBayerStep, DeBayerBit, DeBayerPtr) == false)		
		{
			m_ErrorString = AOIDataCollect.GetErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		ImagePtr = DeBayerPtr;
		BitCount = DeBayerBit;
		ImageStep = DeBayerStep;				
		bAllocated = true;
		DeBayerPtr = NULL;
	}
	if ( BitCount != nTeachMapBitCount )
	{
		if ( true == bAllocated )
		{	JetMemory.free_func(ImagePtr); }
		m_ErrorString.Format(_T("Error, Project Test Scratch Part BitCount != nTeachMapBitCount [%d]"), MapIndex+1);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}	
	const int    NPixels = GetProjectMapScaleMode();
	IMAGE_PTR    CellPtr = NULL;
	IMAGE_SIZE   uCellW = 0;
	IMAGE_SIZE   uCellH = 0;
	IMAGE_SIZE   uCellStep = 0;	

#ifdef _DEBUG
	bool         bSave=true;	
#endif _DEBUG
	CString      Folder;
	CString      MainName;
	CString      KeyName=_T("Scratch");	
	CString      FrameFileName = FramePtr->GetFrameFileName();
	std::vector<CString> SaveFileList;
	Folder = GetProjectDebugFolderScratch();
	JetAPI::ExtractMainFileName(FrameFileName, MainName);	

	if ( JetMemory.alloc_func(BufferSize, CellPtr, fnName, "CellPtr") == false )
	{
		m_ErrorString = JetMemory.GetErrorString();		
		JetMemory.free_func(CellPtr);		
		if ( true == bAllocated )
		{	JetMemory.free_func(ImagePtr);	}
		SaveProjectScratchPartLog(m_ErrorString);
		return false; 
	}	
	const double ScaleVal = 1.00/NPixels;
	ImageAPI.CalcScaleSize(ImageW, ImageH, ImageStep, BitCount, ScaleVal, uCellW, uCellH, uCellStep);
	if ( ImageAPI.ScaleImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ScaleVal, uCellW, uCellH, uCellStep, CellPtr) == false )
	//if ( ImageAPI.FastScaleImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, NPixels, uCellW, uCellH, uCellStep, CellPtr) == false )	
	{
		JetMemory.free_func(CellPtr);		
		if ( true == bAllocated )
		{	JetMemory.free_func(ImagePtr);	}
		m_ErrorString = ImageAPI.GetImageApiErrorString();	
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_Cell.PNG"), Folder, KeyName, MainName);
		//ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtr, true);
		//SaveFileList.push_back(str);
	}
#endif//_DEBUG

	/*
	if ( AOIDataCollect.ExecEnhanceDisplayImage(uCellW, uCellH, uCellStep, BitCount, CellPtr, CellPtr) == false )
	{
		JetMemory.free_func(CellPtr);
		if ( true == bAllocated )
		{	JetMemory.free_func(ImagePtr); }
		m_ErrorString = ImageAPI.GetImageApiErrorString();		
		SaveProjectScratchPartLog(m_ErrorString);
		return false;	
	}
	*/
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_Cell2.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG

	if ( true == bAllocated )
	{	JetMemory.free_func(ImagePtr);	}
	//尋找在專案底圖的基本方位	
	TPOINT2D MapRes;	
	TREGION4D CadRgn;
	TREGION4D CellRgn;
	TREGION4D StageRgn;	
	TREGION4D TeachRgn;
	const double FieldStagePosX = FieldPtr->GetFieldStagePosX();
	const double FieldStagePosY = FieldPtr->GetFieldStagePosY();	
	const double FieldStageSizeW = FieldPtr->GetFieldSizeW_Real();
	const double FieldStageSizeH = FieldPtr->GetFieldSizeH_Real();	
	
	GetProjectMapInfo(MapRes, CadRgn, StageRgn);
	GetProjectMapCalcRgn(TeachRgn);
	TeachRgn = StageRgn;

	CellRgn.minX = FieldStagePosX-(FieldStageSizeW*0.5);
	CellRgn.minY = FieldStagePosY-(FieldStageSizeH*0.5);
	CellRgn.maxX = FieldStagePosX+(FieldStageSizeW*0.5);
	CellRgn.maxY = FieldStagePosY+(FieldStageSizeH*0.5);

	int MapRoiPosX = 0;
	int MapRoiPosY = 0;	
	const double MapTeachCpX = TeachRgn.GetCpX();
	const double MapTeachCpY = TeachRgn.GetCpY();
	const double RgnOffsetX = (FieldStagePosX-MapTeachCpX);
	const double RgnOffsetY = (FieldStagePosY-MapTeachCpY);
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	if ( true == SignX )
	{	MapRoiPosX = (int)(nTeachMapW2+(RgnOffsetX/MapRes.x));	}
	else
	{	MapRoiPosX = (int)(nTeachMapW2-(RgnOffsetX/MapRes.x)); }	
	if ( true == SignY )
	{	MapRoiPosY = (int)(nTeachMapH2+(RgnOffsetY/MapRes.y));	}
	else
	{	MapRoiPosY = (int)(nTeachMapH2-(RgnOffsetY/MapRes.y));	}
	MapRoiPosY = nTeachMapH-MapRoiPosY;	

	bool  ModifyCellRoi=false;
	RECT  MapRoiRect={0};	
	RECT  CellRoiRect={0};
	IMAGE_SIZE RoiExt = 64;
	IMAGE_SIZE MapRoiW = uCellW+RoiExt;
	IMAGE_SIZE MapRoiH = uCellH+RoiExt;
	IMAGE_SIZE MapRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, BitCount, nAlign);
	MapRoiRect.left   = MapRoiPosX-(MapRoiW/2);
	MapRoiRect.top    = MapRoiPosY-(MapRoiH/2);
	MapRoiRect.right  = MapRoiPosX+(MapRoiW/2);
	MapRoiRect.bottom = MapRoiPosY+(MapRoiH/2);
	CellRoiRect.left = 0;
	CellRoiRect.top = 0;
	CellRoiRect.right= uCellW;
	CellRoiRect.bottom = uCellH;
	ModifyCellRoi = false;
	if ( MapRoiRect.left < 0 )
	{	
		ModifyCellRoi = true;
		CellRoiRect.left = CellRoiRect.left+(0-MapRoiRect.left);
		MapRoiRect.left = 0;
	}
	if ( MapRoiRect.right > nTeachMapW )
	{
		ModifyCellRoi = true;
		CellRoiRect.right = CellRoiRect.right-(MapRoiRect.right-nTeachMapW);
		MapRoiRect.right = nTeachMapW;		
	}
	if ( MapRoiRect.top < 0 )
	{	
		ModifyCellRoi = true;
		CellRoiRect.top = CellRoiRect.top+(0-MapRoiRect.top);
		MapRoiRect.top = 0;
	}
	if ( MapRoiRect.bottom > nTeachMapH )
	{
		ModifyCellRoi = true;
		CellRoiRect.bottom = CellRoiRect.bottom-(MapRoiRect.bottom-nTeachMapH);
		MapRoiRect.bottom = nTeachMapH;		
	}
	if ( CellRoiRect.right<=CellRoiRect.left || CellRoiRect.bottom<=CellRoiRect.top )
	{
		JetMemory.free_func(CellPtr);
	#ifdef _DEBUG
		JetAPI::DeleteFileList(SaveFileList);
	#endif//_DEBUG
		SaveProjectScratchPartLog(_T("CellRoiRect.right<=CellRoiRect.left || CellRoiRect.bottom<=CellRoiRect.top"));		
		return true; 			
	}
	if ( true == ModifyCellRoi )
	{
		if ( true == SignX )
		{
			CellRgn.minX += (CellRoiRect.left-0)*MapRes.x;
			CellRgn.maxX -= (uCellW-CellRoiRect.right)*MapRes.x;
		}
		else
		{	
			CellRgn.maxX -= (CellRoiRect.left-0)*MapRes.x;
			CellRgn.minX += (uCellW-CellRoiRect.right)*MapRes.x;		
		}	
		if ( true == SignY )
		{	
			CellRgn.maxY -= (CellRoiRect.top-0)*MapRes.y;
			CellRgn.minY += (uCellH-CellRoiRect.bottom)*MapRes.y;
		}
		else
		{
			CellRgn.minY += (CellRoiRect.top-0)*MapRes.y;
			CellRgn.maxY -= (uCellH-CellRoiRect.bottom)*MapRes.y;
		}

		IMAGE_PTR  CellRoiPtr=NULL;
		IMAGE_SIZE CellRoiW=CellRoiRect.right-CellRoiRect.left;
		IMAGE_SIZE CellRoiH=CellRoiRect.bottom-CellRoiRect.top;
		IMAGE_SIZE CellRoiStep=JetAPI::GetBMPImagePixelsPerLine(CellRoiW, BitCount, nAlign);;
		const size_t CellRoiBuffserSize = ImageAPI.CalcBufferSize(CellRoiStep, CellRoiH);
		if ( JetMemory.alloc_func(CellRoiBuffserSize, CellRoiPtr, fnName, "CellRoiPtr") == false )
		{
			JetMemory.free_func(CellPtr);			
			m_ErrorString = JetMemory.GetErrorString();		
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		if ( ImageAPI.ExtractRoiImage3(uCellW, uCellH, uCellStep, BitCount, CellPtr, CellRoiRect, CellRoiStep, CellRoiPtr, false) == false )
		{
			JetMemory.free_func(CellPtr);
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		JetMemory.free_func(CellPtr);
		uCellW = CellRoiW;
		uCellH = CellRoiH;
		uCellStep = CellRoiStep;
		CellPtr = CellRoiPtr;
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_Cell3.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtr, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
		MapRoiW = MapRoiRect.right-MapRoiRect.left;
		MapRoiH = MapRoiRect.bottom-MapRoiRect.top;
		MapRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, BitCount, nAlign);
	}
	
	IMAGE_PTR    MapRoiPtr = NULL;
	const size_t MapRoiBufferSize = ImageAPI.CalcBufferSize(MapRoiStep, MapRoiH);
	if ( JetMemory.alloc_func(MapRoiBufferSize, MapRoiPtr, fnName, "MapRoiPtr") == false )
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);		
		m_ErrorString = JetMemory.GetErrorString();		
		SaveProjectScratchPartLog(m_ErrorString);
		return false; 
	}		

	if ( ImageAPI.ExtractRoiImage3(nTeachMapW, nTeachMapH, nTeachMapStep, BitCount, TeachMapPtr, MapRoiRect, MapRoiStep, MapRoiPtr, false) == false )
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);		
		m_ErrorString = ImageAPI.GetImageApiErrorString();		
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MapRoi.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG

	IMAGE_PTR    MapMaskRoiPtr = NULL;
	const IMAGE_SIZE MapMaskRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, MaskBitCount, nAlign);
	const size_t MapMaskRoiBufferSize = ImageAPI.CalcBufferSize(MapMaskRoiStep, MapRoiH);
	if ( NULL!=TeachMaskPtr && nTeachMaskW==nTeachMapW && nTeachMaskH==nTeachMapH )
	{
		if ( JetMemory.alloc_func(MapMaskRoiBufferSize, MapMaskRoiPtr, fnName, "MapMaskRoiPtr") == false )
		{
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(MapRoiPtr);
			JetMemory.free_func(MapMaskRoiPtr);			
			m_ErrorString = JetMemory.GetErrorString();				
			SaveProjectScratchPartLog(m_ErrorString);
			return false; 
		}			
		if ( ImageAPI.ExtractRoiImage3(nTeachMaskW, nTeachMaskH, nTeachMaskStep, MaskBitCount, TeachMaskPtr, MapRoiRect, MapMaskRoiStep, MapMaskRoiPtr, false) == false )
		{
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(MapRoiPtr);
			JetMemory.free_func(MapMaskRoiPtr);			
			m_ErrorString = ImageAPI.GetImageApiErrorString();		
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MapMaskRoi.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiPtr, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
	}

	//影像對位
	CJetMatch  Match;	
	const bool bRobustness = true;
	const int  nMinReduceArea = 4096;
	const int  nFinalReduction = 0;
	const int  nEnableScaleMatch = GetProjectParameter().m_ScratchPartMatchUseScale;
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();
	if ( Match.SetMatchLibType(MatchLibType)==false )
	{	
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);		
		m_ErrorString = Match.GetErrorString();		
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	//initial eMatch
	Match.SetMatchDefaultParam();	
	Match.SetRobustness(bRobustness);
	Match.SetMinReducedArea(nMinReduceArea);
	Match.SetFinalReduction(nFinalReduction);	
	if ( Match.LearnPattern(uCellW, uCellH, uCellStep, BitCount, CellPtr, true) == false )	
	{	
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);		
		m_ErrorString = Match.GetErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	
	Match.SetInterpolate(true);
	Match.SetMinScore(-1);
	Match.SetMaxPositions(1);
	Match.SetMaxInitialPositions(4);
	if ( Match.Match(MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, true) == false )
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);		
		m_ErrorString = Match.GetErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;	
	}
	const int NResults = Match.GetNumPositions();
	if ( 0 == NResults )
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);		
		m_ErrorString = _T("Error, Map Match Fault");
		SaveProjectScratchPartLog(m_ErrorString);
		return false;	
	}
	const int ResultIdx = 0;
	double ResultX = Match.GetResultPosX(ResultIdx);
	double ResultY = Match.GetResultPosY(ResultIdx);
	double ResultA = Match.GetResultAngle(ResultIdx);
	double ResultS = Match.GetResultScore(ResultIdx)*100.0;
	double ResultSX = Match.GetResultScaleX(ResultIdx);
	double ResultSY = Match.GetResultScaleY(ResultIdx);	
	if ( FN_ENABLE == nEnableScaleMatch )
	{
		const double ResultX_N = Match.GetResultPosX(ResultIdx);
		const double ResultY_N = Match.GetResultPosY(ResultIdx);
		const double ResultA_N = Match.GetResultAngle(ResultIdx);
		const double ResultS_N = Match.GetResultScore(ResultIdx)*100.0;
		const double ResultSX_N = Match.GetResultScaleX(ResultIdx);
		const double ResultSY_N = Match.GetResultScaleY(ResultIdx);

		//再以Scale計算一次
		const float ScaleRange=0.1f;
		const float ScaleMin=1.0f-ScaleRange;
		const float ScaleMax=1.0f+ScaleRange;

		Match.SetMinScale(ScaleMin);
		Match.SetMinScaleX(ScaleMin);
		Match.SetMinScaleY(ScaleMin);
		Match.SetMaxScale(ScaleMax);
		Match.SetMaxScaleX(ScaleMax);
		Match.SetMaxScaleY(ScaleMax);
		Match.SetUseScale(true);
		if ( Match.Match(MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, true) == false )
		{
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(MapRoiPtr);
			JetMemory.free_func(MapMaskRoiPtr);		
			m_ErrorString = Match.GetErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;	
		}
		const int NResults_F = Match.GetNumPositions();			
		if ( NResults_F > 0 )
		{
			double ResultX_F = Match.GetResultPosX(ResultIdx);
			double ResultY_F = Match.GetResultPosY(ResultIdx);
			double ResultA_F = Match.GetResultAngle(ResultIdx);
			double ResultS_F = Match.GetResultScore(ResultIdx)*100.0;
			double ResultSX_F = Match.GetResultScaleX(ResultIdx);
			double ResultSY_F = Match.GetResultScaleY(ResultIdx);		
			//差太多使用沒有內插的, 以及找最相似度最高的
			if ( fabs(ResultX_F-ResultX_N)>10 || fabs(ResultY_F-ResultY_N)>10 || ResultS_F<ResultS_N )
			{
				ResultX = ResultX_N;
				ResultY = ResultY_N;
				ResultA = ResultA_N;
				ResultS = ResultS_N;
				ResultSX = ResultSX_N;
				ResultSY = ResultSY_N;
			}
			else
			{				
				ResultX = ResultX_F;
				ResultY = ResultY_F;
				ResultA = ResultA_F;
				ResultS = ResultS_F;
				ResultSX = ResultSX_F;
				ResultSY = ResultSY_F;	
			}
		}				
	}
	
	const int    nResultX = (int)(ResultX+0.5);
	const int    nResultY = (int)(ResultY+0.5);
	const double ResultX_Start = nResultX-(uCellW/2);
	const double ResultY_Start = nResultY-(uCellH/2);
	//const double ResultX_Start = ResultX-(uCellW*0.5);
	//const double ResultY_Start = ResultY-(uCellH*0.5);
	//const int    nCellPosX = (int)(ResultX_Start+0.5);
	//const int    nCellPosY = (int)(ResultY_Start+0.5);	
	const int    nCellPosX = JetAPI::Floor(ResultX_Start);
	const int    nCellPosY = JetAPI::Floor(ResultY_Start);
	const int    nCellPosXEnd = nCellPosX+uCellW;
	const int    nCellPosYEnd = nCellPosY+uCellH;
	if ( nCellPosX<0 || nCellPosY<0 || ResultS<50.0 || nCellPosXEnd>MapRoiW || nCellPosYEnd>MapRoiH )
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);		
		m_ErrorString = _T("Error, Map Match Result Fault");
	#ifdef _DEBUG
		JetAPI::DeleteFileList(SaveFileList);
	#endif//_DEBUG		
		SaveProjectScratchPartLog(m_ErrorString);
		return false;	
	}	

	MASK_PTR     RawMaskPtrL = NULL;	
	MASK_PTR     TestMaskPtr = NULL;
	MASK_PTR     TestMaskPtr1 = NULL;
	MASK_PTR     TestMaskPtr2 = NULL;
	MASK_PTR     TestMaskPtrR = NULL;
	MASK_PTR     TestMaskPtrG = NULL;
	MASK_PTR     TestMaskPtrB = NULL;
	MASK_PTR     TestMaskPtrV = NULL;
	const IMAGE_SIZE MaskW  = uCellW;
	const IMAGE_SIZE MaskH  = uCellH;	
	const IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(MaskW, MaskBitCount, 4);
	const size_t MaskBufferSize = ImageAPI.CalcBufferSize(MaskStep, MaskH);
	if ( JetMemory.alloc_func(MaskBufferSize, RawMaskPtrL, fnName, "RawMaskPtrL") == false || 		 
		 JetMemory.alloc_func(MaskBufferSize, TestMaskPtr, fnName, "TestMaskPtr") == false || 
		 JetMemory.alloc_func(MaskBufferSize, TestMaskPtr1, fnName, "TestMaskPtr1") == false || 
		 JetMemory.alloc_func(MaskBufferSize, TestMaskPtr2, fnName, "TestMaskPtr2") == false ||
		 JetMemory.alloc_func(MaskBufferSize, TestMaskPtrR, fnName, "TestMaskPtrR") == false ||
		 JetMemory.alloc_func(MaskBufferSize, TestMaskPtrG, fnName, "TestMaskPtrG") == false ||
		 JetMemory.alloc_func(MaskBufferSize, TestMaskPtrB, fnName, "TestMaskPtrB") == false ||
		 JetMemory.alloc_func(MaskBufferSize, TestMaskPtrV, fnName, "TestMaskPtrV") == false )
	{
		m_ErrorString = JetMemory.GetErrorString();		
		JetMemory.free_func(CellPtr);		
		JetMemory.free_func(MapRoiPtr);		
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);		
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		JetMemory.free_func(TestMaskPtrR);
		JetMemory.free_func(TestMaskPtrG);
		JetMemory.free_func(TestMaskPtrB);
		JetMemory.free_func(TestMaskPtrV);
		SaveProjectScratchPartLog(m_ErrorString);
		return false; 
	}

	//剔除此FOV內的零件
	size_t       i=0, j=0;
	TREGION4D    FieldRgn=CellRgn;
	const int    MaskMode=PROJECT_PART_MASK_BODY|PROJECT_CODE_MASK_BODY;
	::memset(TestMaskPtr1, 0xFF, sizeof(MASK_DATA)*MaskBufferSize);
	if ( CreateProjectPartMaskImage_v2(MaskW, MaskH, MaskStep, TestMaskPtr1, FieldRgn, MapRes, MaskMode, 0x00) == false )
	{
		JetMemory.free_func(CellPtr);		
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);		
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		JetMemory.free_func(TestMaskPtrR);
		JetMemory.free_func(TestMaskPtrG);
		JetMemory.free_func(TestMaskPtrB);
		JetMemory.free_func(TestMaskPtrV);
		SaveProjectScratchPartLog(m_ErrorString);
		return false; 
	}		
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskNoPart.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr1, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG
	if ( ImageAPI.MorphGrayImage3(MaskW, MaskH, MaskStep, TestMaskPtr1, MORPH_OPEN, MORPH_SHAPE_RECT, 9, 1, TestMaskPtr2) == false )
	{		
		JetMemory.free_func(CellPtr);	
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);		
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		JetMemory.free_func(TestMaskPtrR);
		JetMemory.free_func(TestMaskPtrG);
		JetMemory.free_func(TestMaskPtrB);
		JetMemory.free_func(TestMaskPtrV);
		m_ErrorString = ImageAPI.GetImageApiErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskNoPartOpen.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG	
	
	//::memcpy(TestMaskPtr, TestMaskPtr2, sizeof(MASK_DATA)*MaskBufferSize);
	//::memset(TestMaskPtr, 0xFF, sizeof(MASK_DATA)*MaskBufferSize);

	//依據遮罩來比較影像是否有問題	
	size_t MaskIdx=0;
	size_t MapMaskIdx=0;
	size_t CellIdx=0, RoiIdx=0;	
	IMAGE_PTR  CellPtrRaw=NULL;
	IMAGE_PTR  MapRoiPtrRaw=NULL;
	const size_t CellSize=ImageAPI.CalcBufferSize(uCellStep, uCellH);
	const size_t MapRoiSize=ImageAPI.CalcBufferSize(MapRoiStep, MapRoiH);
	if ( JetMemory.alloc_func(CellSize, CellPtrRaw, fnName, "CellPtrRaw") == false ||
		JetMemory.alloc_func(MapRoiSize, MapRoiPtrRaw, fnName, "MapRoiPtrRaw") == false )
	{
		m_ErrorString = JetMemory.GetErrorString();
		JetMemory.free_func(CellPtr);	
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(CellPtrRaw);	
		JetMemory.free_func(MapRoiPtrRaw);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);		
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		JetMemory.free_func(TestMaskPtrR);
		JetMemory.free_func(TestMaskPtrG);
		JetMemory.free_func(TestMaskPtrB);
		JetMemory.free_func(TestMaskPtrV);		
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	::memset(RawMaskPtrL, 0x00, sizeof(MASK_DATA)*MaskBufferSize);
	const int SmoothSize = GetProjectParameter().m_DropOutPartSmoothSize;
	if ( SmoothSize > 0 )
	{
		IMAGE_PTR SwapPtr=NULL;
		const int nSmoothSize = ImageAPI.GetKernelSize(SmoothSize);							
		if ( ImageAPI.SmoothImage3(uCellW, uCellH, uCellStep, BitCount, CellPtr, nSmoothSize, CellPtrRaw) == true )
		{	
			SwapPtr = CellPtrRaw;
			CellPtrRaw = CellPtr;
			CellPtr = SwapPtr;			
		}
		else
		{	::memcpy(CellPtrRaw, CellPtr, sizeof(IMAGE_DATA)*CellSize); }
		
		if ( ImageAPI.SmoothImage3(MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, nSmoothSize, MapRoiPtrRaw) == true )
		{	
			SwapPtr = MapRoiPtrRaw;
			MapRoiPtrRaw = MapRoiPtr;
			MapRoiPtr = SwapPtr;			
		}
		else
		{	::memcpy(MapRoiPtrRaw, MapRoiPtr, sizeof(IMAGE_DATA)*MapRoiSize); }
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_CellSmooth.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtr, true);
			SaveFileList.push_back(str);

			str.Format(_T("%s\\%s_%s_MapRoiSmooth.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
	}
	else
	{
		::memcpy(CellPtrRaw, CellPtr, sizeof(IMAGE_DATA)*CellSize);
		::memcpy(MapRoiPtrRaw, MapRoiPtr, sizeof(IMAGE_DATA)*MapRoiSize);		
	}	
	
	size_t s=0, t=0, usedCnt=0;	
	unsigned char R=0, G=0, B=0;	
	unsigned char IR=0, IG=0, IB=0, IV=0;
	int MinR=0, MinG=0, MinB=0, MinV=0;
	int MaxR=0, MaxG=0, MaxB=0, MaxV=0;
	const size_t CalcSizeMin = 4;	
	const int     ColorExpand = GetProjectParameter().m_ScratchPartColorExpand;	
	const double  CalcSizeWum = GetProjectParameter().m_ScratchPartCalcSizeW;
	const double  CalcSizeHum = GetProjectParameter().m_ScratchPartCalcSizeH;	
	size_t WndSizeW = (size_t)((CalcSizeWum/MapRes.x)+0.5);
	size_t WndSizeH = (size_t)((CalcSizeHum/MapRes.y)+0.5);
	if ( WndSizeW < CalcSizeMin ) { WndSizeW = CalcSizeMin; }
	if ( WndSizeH < CalcSizeMin ) { WndSizeH = CalcSizeMin; }
	const size_t WndSizeW2 = WndSizeW/2;
	const size_t WndSizeH2 = WndSizeH/2;
	
	::memset(RawMaskPtrL, 0x00, sizeof(MASK_DATA)*MaskBufferSize);
	if ( 24 == BitCount )
	{
		//將RGB轉成RGBV
		for ( i=0; i<MaskH; i++ )
		{
			for ( j=0; j<MaskW; j++ )
			{
				MaskIdx = i*MaskStep+j;
				
				RoiIdx = ((i+nCellPosY)*MapRoiStep)+(j+nCellPosX)*3;
				B = MapRoiPtr[RoiIdx];
				G = MapRoiPtr[RoiIdx+1];
				R = MapRoiPtr[RoiIdx+2];
				
				if ( ImageAPI.RGBConvertToRGBV(R, G, B, IR, IG, IB, IV) == false ) 
				{
					TestMaskPtrR[MaskIdx] = 0;
					TestMaskPtrG[MaskIdx] = 0;
					TestMaskPtrB[MaskIdx] = 0;
					TestMaskPtrV[MaskIdx] = 0;
					continue; 
				}
				TestMaskPtrR[MaskIdx] = IR;
				TestMaskPtrG[MaskIdx] = IG;
				TestMaskPtrB[MaskIdx] = IB;
				TestMaskPtrV[MaskIdx] = IV;
			}
		}		
		

		for ( i=WndSizeH2; i<MaskH-WndSizeH2; i++ )
		{
			for ( j=WndSizeW2; j<MaskW-WndSizeW2; j++ )
			{
				MaskIdx = i*MaskStep+j;
				if ( 0 == TestMaskPtr2[MaskIdx] )
				{	continue;	}
				if ( NULL != MapMaskRoiPtr )
				{				
					MapMaskIdx = ((i+nCellPosY)*MapMaskRoiStep)+(j+nCellPosX);				
					if ( 0 == MapMaskRoiPtr[MapMaskIdx] )
					{	continue;	}
				}
				
				//Get Color Filter Range (at Map)
				usedCnt=0;
				MaxR = MaxG = MaxB = MaxV = 0;
				MinR = MinG = MinB = MinV = 255;
				for ( t=i-WndSizeH2; t<i+WndSizeH2; t++ )
				{
					for ( s=j-WndSizeW2; s<j+WndSizeW2; s++ )
					{					
						MaskIdx = t*MaskStep+s;
						if ( 0 == TestMaskPtr2[MaskIdx] )
						{	continue;	}
						if ( NULL != MapMaskRoiPtr )
						{				
							MapMaskIdx = ((t+nCellPosY)*MapMaskRoiStep)+(s+nCellPosX);				
							if ( 0 == MapMaskRoiPtr[MapMaskIdx] )
							{	continue;	}
						}

						//CellIdx = (t*uCellStep)+(s*3);
						//B = CellPtr[CellIdx];
						//G = CellPtr[CellIdx+1];
						//R = CellPtr[CellIdx+2];
						/*
						RoiIdx = ((t+nCellPosY)*MapRoiStep)+(s+nCellPosX)*3;
						B = MapRoiPtr[RoiIdx];
						G = MapRoiPtr[RoiIdx+1];
						R = MapRoiPtr[RoiIdx+2];
						if ( ImageAPI.RGBConvertToRGBV(R, G, B, IR, IG, IB, IV) == false ) { continue; }

						if ( IR != TestMaskPtrR[MaskIdx] )
						{	IR = IR; }
						if ( IG != TestMaskPtrG[MaskIdx] )
						{	IG = IG; }
						if ( IB != TestMaskPtrB[MaskIdx] )
						{	IB = IB; }
						if ( IV != TestMaskPtrV[MaskIdx] )
						{	IV = IV; }
						*/
						IR = TestMaskPtrR[MaskIdx];
						IG = TestMaskPtrG[MaskIdx];
						IB = TestMaskPtrB[MaskIdx];
						IV = TestMaskPtrV[MaskIdx];

						if ( IR < MinR ) { MinR = IR; }
						if ( IG < MinG ) { MinG = IG; }
						if ( IB < MinB ) { MinB = IB; }
						if ( IV < MinV ) { MinV = IV; }

						if ( IR > MaxR ) { MaxR = IR; }
						if ( IG > MaxG ) { MaxG = IG; }
						if ( IB > MaxB ) { MaxB = IB; }
						if ( IV > MaxV ) { MaxV = IV; }
						usedCnt ++;
					}
				}
				if ( usedCnt < CalcSizeMin ) { continue; }

				MaskIdx = i*MaskStep+j;
				MinR = MAX(MinR-ColorExpand, 0);
				MinG = MAX(MinG-ColorExpand, 0);
				MinB = MAX(MinB-ColorExpand, 0);
				MinV = MAX(MinV-ColorExpand, 0);
				MaxR = MIN(MaxR+ColorExpand, 255);
				MaxG = MIN(MaxG+ColorExpand, 255);
				MaxB = MIN(MaxB+ColorExpand, 255);
				MaxV = MIN(MaxV+ColorExpand, 255);
				
				CellIdx = (i*uCellStep)+(j*3);				
				B = CellPtr[CellIdx];
				G = CellPtr[CellIdx+1];
				R = CellPtr[CellIdx+2];
				//RoiIdx = ((i+nCellPosY)*MapRoiStep)+(j+nCellPosX)*3;
				//nB2 = MapRoiPtr[RoiIdx];
				//nG2 = MapRoiPtr[RoiIdx+1];
				//nR2 = MapRoiPtr[RoiIdx+2];
			#ifdef _DEBUG
				if ( i > 120 )
				{
					if ( j>50 && j<75 )
					{
						CellIdx = CellIdx;
					}
				}
			#endif//_DEBUG
				if ( ImageAPI.RGBConvertToRGBV(R, G, B, IR, IG, IB, IV) == false ) { continue; }
				
				if ( IR<MinR || IR>MaxR ) { RawMaskPtrL[MaskIdx] = 255; }
				if ( IG<MinG || IG>MaxG ) { RawMaskPtrL[MaskIdx] = 255; }
				if ( IB<MinB || IB>MaxB ) { RawMaskPtrL[MaskIdx] = 255; }
				if ( IV<MinV || IV>MaxV ) { RawMaskPtrL[MaskIdx] = 255; }
				if ( 0 != RawMaskPtrL[MaskIdx] )
				{	CellIdx = CellIdx;	}
											
			}
		}
	}
	else
	{	//8-Bit Image
		for ( i=WndSizeH2; i<MaskH-WndSizeH2; i++ )
		{
			for ( j=WndSizeW2; j<MaskW-WndSizeW2; j++ )
			{
				MaskIdx = i*MaskStep+j;
				if ( 0 == TestMaskPtr2[MaskIdx] )
				{	continue;	}
				if ( NULL != MapMaskRoiPtr )
				{				
					MapMaskIdx = ((i+nCellPosY)*MapMaskRoiStep)+(j+nCellPosX);				
					if ( 0 == MapMaskRoiPtr[MapMaskIdx] )
					{	continue;	}
				}
				
				//Get Gray Range (at Map)
				usedCnt=0;
				MaxR = 0;
				MinR = 255;
				for ( t=i-WndSizeH2; t<i+WndSizeH2; t++ )
				{
					for ( s=j-WndSizeW2; s<j+WndSizeW2; s++ )
					{
						MaskIdx = t*MaskStep+s;
						if ( 0 == TestMaskPtr2[MaskIdx] )
						{	continue;	}
						if ( NULL != MapMaskRoiPtr )
						{				
							MapMaskIdx = ((t+nCellPosY)*MapMaskRoiStep)+(s+nCellPosX);
							if ( 0 == MapMaskRoiPtr[MapMaskIdx] )
							{	continue;	}
						}

						//CellIdx = (t*uCellStep)+(s);
						RoiIdx = ((t+nCellPosY)*MapRoiStep)+(s+nCellPosX);

						//IR = CellPtr[CellIdx];
						IR = MapRoiPtr[RoiIdx];
						if ( IR < MinR ) { MinR = IR; }
						if ( IR > MaxR ) { MinR = IR; }
						usedCnt ++;
					}
				}
				if ( usedCnt < CalcSizeMin ) { continue; }

				MaskIdx = i*MaskStep+j;
				MinR = MAX(MinR-ColorExpand, 0);
				MaxR = MIN(MaxR+ColorExpand, 255);		

				CellIdx = (i*uCellStep)+(j);
				//RoiIdx = ((i+nCellPosY)*MapRoiStep)+(j+nCellPosX);
				IR = CellPtr[CellIdx];
				if ( IR<MinR || IR>MaxR ) { RawMaskPtrL[MaskIdx] = 255; }
			}
		}
	}
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskCompareRaw.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, RawMaskPtrL, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG

	//遮罩濾除
	/*
	for ( i=WndSizeH2; i<MaskH-WndSizeH2; i++ )
	{
		for ( j=WndSizeW2; j<MaskW-WndSizeW2; j++ )
		{
			MaskIdx = i*MaskStep+j;
			if ( 0 == TestMaskPtr2[MaskIdx] )
			{
				RawMaskPtrL[MaskIdx] = 0;
				continue; 
			}
			if ( NULL != MapMaskRoiPtr )
			{				
				MapMaskIdx = ((i+nCellPosY)*MapMaskRoiStep)+(j+nCellPosX);				
				if ( 0 == MapMaskRoiPtr[MapMaskIdx] )
				{
					RawMaskPtrL[MaskIdx] = 0;
					continue;
				}
			}
			if ( RawMaskPtrL[MaskIdx] < ThresholdL )
			{	RawMaskPtrL[MaskIdx] = 0;	}
			else
			{	RawMaskPtrL[MaskIdx] = 255; }
		}
	}
	*/		
	bool KeepImage=false;
#ifndef _DEBUG
	KeepImage = SaveImage;
#endif//_DEBUG
	JetMemory.free_func(CellPtr);	
	JetMemory.free_func(MapRoiPtr);
	if ( false == KeepImage )
	{		
		JetMemory.free_func(CellPtrRaw);	
		JetMemory.free_func(MapRoiPtrRaw);
		JetMemory.free_func(MapMaskRoiPtr);	
	}

	::memcpy(TestMaskPtr, RawMaskPtrL, sizeof(MASK_DATA)*MaskBufferSize);
	::memcpy(TestMaskPtr2, RawMaskPtrL, sizeof(MASK_DATA)*MaskBufferSize);

	int MorphMode = 0;
	int   ShapeMode = MORPH_SHAPE_ELLIPSE;//MORPH_SHAPE_RECT, MORPH_SHAPE_CROSS, MORPH_SHAPE_ELLIPSE
	const int NoiseFilterOpen = GetProjectParameter().m_ScratchPartFilterOpenSize;
	if ( NoiseFilterOpen > 0 ) 	
	{
		MorphMode = MORPH_OPEN;
		const int OpenSize = ImageAPI.GetKernelSize(NoiseFilterOpen);
		if ( ImageAPI.MorphGrayImage3(MaskW, MaskH, MaskStep, TestMaskPtr, MorphMode, ShapeMode, OpenSize, 1, TestMaskPtr2) == false )	
		{	
			JetMemory.free_func(CellPtrRaw);	
			JetMemory.free_func(MapRoiPtrRaw);
			JetMemory.free_func(MapMaskRoiPtr);	
			JetMemory.free_func(RawMaskPtrL);		
			JetMemory.free_func(TestMaskPtr);
			JetMemory.free_func(TestMaskPtr1);
			JetMemory.free_func(TestMaskPtr2);
			JetMemory.free_func(TestMaskPtrR);
			JetMemory.free_func(TestMaskPtrG);
			JetMemory.free_func(TestMaskPtrB);
			JetMemory.free_func(TestMaskPtrV);
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		::memcpy(TestMaskPtr, TestMaskPtr2, sizeof(MASK_DATA)*MaskBufferSize);
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MaskCompareOpen.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
	}	
	const int NoiseFilterClose = GetProjectParameter().m_ScratchPartFilterCloseSize;	
	if ( NoiseFilterClose > 0 ) 	
	{
		MorphMode = MORPH_CLOSE;
		const int CloseSize = ImageAPI.GetKernelSize(NoiseFilterClose);
		if ( ImageAPI.MorphGrayImage3(MaskW, MaskH, MaskStep, TestMaskPtr, MorphMode, ShapeMode, CloseSize, 1, TestMaskPtr2) == false )	
		{	
			JetMemory.free_func(CellPtrRaw);	
			JetMemory.free_func(MapRoiPtrRaw);
			JetMemory.free_func(MapMaskRoiPtr);	
			JetMemory.free_func(RawMaskPtrL);		
			JetMemory.free_func(TestMaskPtr);
			JetMemory.free_func(TestMaskPtr1);
			JetMemory.free_func(TestMaskPtr2);
			JetMemory.free_func(TestMaskPtrR);
			JetMemory.free_func(TestMaskPtrG);
			JetMemory.free_func(TestMaskPtrB);
			JetMemory.free_func(TestMaskPtrV);
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		::memcpy(TestMaskPtr, TestMaskPtr2, sizeof(MASK_DATA)*MaskBufferSize);
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MaskCompareClose.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
	}
	
	const bool bMeasure = false;
	const double MinThickness=0;
	std::vector<CAOIComponent*> NewComponentList;
	const double PartWHRatio = -1;
	const double PartW = GetProjectParameter().m_ScratchPartMinSizeW;
	const double PartH = GetProjectParameter().m_ScratchPartMinSizeH;	
	if ( CreateProjectComponentByBlob(MaskW, MaskH, MaskStep, TestMaskPtr2, NULL, NULL, FieldRgn, MapRes, PartW, PartH, PartWHRatio, MinThickness, bMeasure, NewComponentList) == false )
	{	
		JetMemory.free_func(CellPtrRaw);	
		JetMemory.free_func(MapRoiPtrRaw);
		JetMemory.free_func(MapMaskRoiPtr);	
		JetMemory.free_func(RawMaskPtrL);		
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		JetMemory.free_func(TestMaskPtrR);
		JetMemory.free_func(TestMaskPtrG);
		JetMemory.free_func(TestMaskPtrB);
		JetMemory.free_func(TestMaskPtrV);		
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}	
	JetMemory.free_func(RawMaskPtrL);	
	JetMemory.free_func(TestMaskPtr);
	JetMemory.free_func(TestMaskPtr1);
	JetMemory.free_func(TestMaskPtr2);
	JetMemory.free_func(TestMaskPtrR);
	JetMemory.free_func(TestMaskPtrG);
	JetMemory.free_func(TestMaskPtrB);
	JetMemory.free_func(TestMaskPtrV);

	const size_t MaxComponentInFov = 36;
	const size_t NewComponentCount = NewComponentList.size();
	if ( 0 == NewComponentCount )//|| NewComponentCount>MaxComponentInFov )
	{
		JetMemory.free_func(CellPtrRaw);	
		JetMemory.free_func(MapRoiPtrRaw);
		JetMemory.free_func(MapMaskRoiPtr);	
	#ifdef _DEBUG
		JetAPI::DeleteFileList(SaveFileList);
	#endif//_DEBUG
		SaveProjectScratchPartLog(_T("0 == NewComponentCount"));		
		return true;
	}
	
	int  nSaveTxt=SaveImage;
#ifdef _DEBUG
	nSaveTxt=FN_ENABLE;
#endif//_DEBUG
	if ( FN_ENABLE == nSaveTxt )
	{
		FILE *pfile = NULL;
		str.Format(_T("%s\\%s_%s_MatchResult.TXT"), Folder, KeyName, MainName);
		pfile = _tfopen(str, _T("w+"));
		if ( NULL != pfile )
		{
			::_ftprintf(pfile, _T("Result X:%.4f\n"), ResultX);
			::_ftprintf(pfile, _T("Result Y:%.4f\n"), ResultY);
			::_ftprintf(pfile, _T("Result Angle:%.4f\n"), ResultA);
			::_ftprintf(pfile, _T("Result Score:%.4f\n"), ResultS);
			::_ftprintf(pfile, _T("Result Scale X:%.4f\n"), ResultSX);
			::_ftprintf(pfile, _T("Result Scale Y:%.4f\n"), ResultSY);

			::_ftprintf(pfile, _T("Result X Start:%.4f\n"), ResultX_Start);
			::_ftprintf(pfile, _T("Result Y Start:%.4f\n"), ResultY_Start);
			::_ftprintf(pfile, _T("Cell Start Pos X:%d\n"), nCellPosX);
			::_ftprintf(pfile, _T("Cell Start Pos Y:%d\n"), nCellPosY);		
			::fclose(pfile);	pfile = NULL;
		}
	}

#ifndef _DEBUG
	if ( FN_ENABLE == SaveImage )
	{
		str.Format(_T("%s\\%s_%s_MapRoi.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtrRaw, true);
		str.Format(_T("%s\\%s_%s_MapMaskRoi.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiPtr, true);
		str.Format(_T("%s\\%s_%s_Cell3.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtrRaw, true);
	}
#endif//_DEBUG
	JetMemory.free_func(CellPtrRaw);	
	JetMemory.free_func(MapRoiPtrRaw);
	JetMemory.free_func(MapMaskRoiPtr);	

	if ( CreateProjectScratchPartByComponentList(FieldPtr, NewComponentList) == false)
	{
		SaveProjectScratchPartLog(m_ErrorString);
		return false; 
	}
	if ( SaveProjectSpecTestField(FieldPtr) == false )
	{
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::TestProjectScratchPart_v2(CAOIField *FieldPtr)//檢測刮傷
{
	const char fnName[] = "CAOIProject::TestProjectScratchPartByColorFilter";
	const int TestScratchPart = GetProjectTestScratchPart();
	if (FN_DISABLE == TestScratchPart) { return true; }

	if (NULL == FieldPtr) {
		m_ErrorString.Format(_T("Error, Project Test Scratch Part Field Execption"));
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	if (FieldPtr->CheckFieldMergeFinish() == false) { return true; }
	const int TestFrameIndex = GetProjectParameter().m_ScratchPartFrameIndex;
	CAOIFrame *FramePtr = FieldPtr->GetFieldFramePtr(TestFrameIndex, true);
	if (NULL == FramePtr)
	{
		m_ErrorString.Format(_T("Error, Project Test Scratch Part Frame index Execption [%d]"), TestFrameIndex + 1);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	CString      str;
	bool         bAllocated = false;
	const int    MapIndex = TestFrameIndex;
	FRAME_TYPE   FrameType = FramePtr->GetFrameType();
	IMAGE_SIZE   ImageW = FramePtr->GetFrameImageW();
	IMAGE_SIZE   ImageH = FramePtr->GetFrameImageH();
	IMAGE_SIZE   ImageStep = FramePtr->GetFrameImageStep();
	IMAGE_SIZE   BitCount = FramePtr->GetFrameImageBitCount();
	IMAGE_PTR    ImagePtr = FramePtr->GetFrameImagePtr();
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const IMAGE_SIZE MaskBitCount = 8;
	const int SaveImage = GetProjectScratchPartSaveImage();

	const int nAlign = 4;
	const int nTeachMapW = (int)(m_ProjectMapW[MapIndex]);//專案底圖寬度
	const int nTeachMapH = (int)(m_ProjectMapH[MapIndex]);//專案底圖長度
	const int nTeachMapStep = (int)(m_ProjectMapStep[MapIndex]);//專案底圖步長
	const int nTeachMapBitCount = (int)(m_ProjectBitCount[MapIndex]);//專案底圖位元數
	const int nTeachMapW2 = nTeachMapW / 2;
	const int nTeachMapH2 = nTeachMapH / 2;
	IMAGE_PTR TeachMapPtr = m_ProjectMapPtr[MapIndex];//專案底圖指標

	const int nTeachMaskW = (int)(m_ProjectMapMaskW);
	const int nTeachMaskH = (int)(m_ProjectMapMaskH);
	const int nTeachMaskStep = (int)(m_ProjectMapMaskStep);
	const int nTeachMaskBitCount = (int)(m_ProjectMapMaskBitCount);
	IMAGE_PTR TeachMaskPtr = m_ProjectMapMaskPtr;

	if (NULL == TeachMapPtr)
	{
		m_ErrorString.Format(_T("Error, Project Test Scratch Part NULL == TeachMapPtr [%d]"), MapIndex + 1);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	if (FRAME_BAYER == FrameType)
	{
		IMAGE_SIZE   DeBayerBit = 0;
		IMAGE_SIZE   DeBayerStep = 0;
		IMAGE_PTR    DeBayerPtr = NULL;
		BAYER_PATTERN_MODE BayerPattern = FramePtr->GetFrameBayerPattern();
		if (AOIDataCollect.ExecDebayerImage(fnName, ImageW, ImageH, ImageStep, ImagePtr, BayerPattern, DeBayerStep, DeBayerBit, DeBayerPtr) == false)
		{
			m_ErrorString = AOIDataCollect.GetErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		ImagePtr = DeBayerPtr;
		BitCount = DeBayerBit;
		ImageStep = DeBayerStep;
		bAllocated = true;
		DeBayerPtr = NULL;
	}
	if (BitCount != nTeachMapBitCount)
	{
		if (true == bAllocated)
		{
			JetMemory.free_func(ImagePtr);
		}
		m_ErrorString.Format(_T("Error, Project Test Scratch Part BitCount != nTeachMapBitCount [%d]"), MapIndex + 1);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	const int    NPixels = GetProjectMapScaleMode();
	IMAGE_PTR    CellPtr = NULL;
	IMAGE_SIZE   uCellW = 0;
	IMAGE_SIZE   uCellH = 0;
	IMAGE_SIZE   uCellStep = 0;

#ifdef _DEBUG
	bool         bSave = true;
#endif _DEBUG
	CString      Folder;
	CString      MainName;
	CString      KeyName = _T("Scratch");
	CString      FrameFileName = FramePtr->GetFrameFileName();
	std::vector<CString> SaveFileList;
	Folder = GetProjectDebugFolderScratch();
	JetAPI::ExtractMainFileName(FrameFileName, MainName);

	if (JetMemory.alloc_func(BufferSize, CellPtr, fnName, "CellPtr") == false)
	{
		m_ErrorString = JetMemory.GetErrorString();
		JetMemory.free_func(CellPtr);
		if (true == bAllocated)
		{
			JetMemory.free_func(ImagePtr);
		}
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	const double ScaleVal = 1.00 / NPixels;
	ImageAPI.CalcScaleSize(ImageW, ImageH, ImageStep, BitCount, ScaleVal, uCellW, uCellH, uCellStep);
	if (ImageAPI.ScaleImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ScaleVal, uCellW, uCellH, uCellStep, CellPtr) == false)
		//if ( ImageAPI.FastScaleImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, NPixels, uCellW, uCellH, uCellStep, CellPtr) == false )	
	{
		JetMemory.free_func(CellPtr);
		if (true == bAllocated)
		{
			JetMemory.free_func(ImagePtr);
		}
		m_ErrorString = ImageAPI.GetImageApiErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

#ifdef _DEBUG
	if (true == bSave)
	{
		str.Format(_T("%s\\%s_%s_Cell.PNG"), Folder, KeyName, MainName);
		//ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtr, true);
		//SaveFileList.push_back(str);
	}
#endif//_DEBUG

	/*
	if ( AOIDataCollect.ExecEnhanceDisplayImage(uCellW, uCellH, uCellStep, BitCount, CellPtr, CellPtr) == false )
	{
	JetMemory.free_func(CellPtr);
	if ( true == bAllocated )
	{	JetMemory.free_func(ImagePtr); }
	m_ErrorString = ImageAPI.GetImageApiErrorString();
	SaveProjectScratchPartLog(m_ErrorString);
	return false;
	}
	*/
#ifdef _DEBUG
	if (true == bSave)
	{
		str.Format(_T("%s\\%s_%s_Cell2.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG



	if (true == bAllocated)
	{
		JetMemory.free_func(ImagePtr);
	}
	//尋找在專案底圖的基本方位	
	TPOINT2D MapRes;
	TREGION4D CadRgn;
	TREGION4D CellRgn;
	TREGION4D StageRgn;
	TREGION4D TeachRgn;
	const double FieldStagePosX = FieldPtr->GetFieldStagePosX();
	const double FieldStagePosY = FieldPtr->GetFieldStagePosY();
	const double FieldStageSizeW = FieldPtr->GetFieldSizeW_Real();
	const double FieldStageSizeH = FieldPtr->GetFieldSizeH_Real();

	GetProjectMapInfo(MapRes, CadRgn, StageRgn);
	GetProjectMapCalcRgn(TeachRgn);
	TeachRgn = StageRgn;

	CellRgn.minX = FieldStagePosX - (FieldStageSizeW*0.5);
	CellRgn.minY = FieldStagePosY - (FieldStageSizeH*0.5);
	CellRgn.maxX = FieldStagePosX + (FieldStageSizeW*0.5);
	CellRgn.maxY = FieldStagePosY + (FieldStageSizeH*0.5);

	int MapRoiPosX = 0;
	int MapRoiPosY = 0;
	const double MapTeachCpX = TeachRgn.GetCpX();
	const double MapTeachCpY = TeachRgn.GetCpY();
	const double RgnOffsetX = (FieldStagePosX - MapTeachCpX);
	const double RgnOffsetY = (FieldStagePosY - MapTeachCpY);
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	if (true == SignX)
	{
		MapRoiPosX = (int)(nTeachMapW2 + (RgnOffsetX / MapRes.x));
	}
	else
	{
		MapRoiPosX = (int)(nTeachMapW2 - (RgnOffsetX / MapRes.x));
	}
	if (true == SignY)
	{
		MapRoiPosY = (int)(nTeachMapH2 + (RgnOffsetY / MapRes.y));
	}
	else
	{
		MapRoiPosY = (int)(nTeachMapH2 - (RgnOffsetY / MapRes.y));
	}
	MapRoiPosY = nTeachMapH - MapRoiPosY;

	bool  ModifyCellRoi = false;
	RECT  MapRoiRect = { 0 };
	RECT  CellRoiRect = { 0 };
	IMAGE_SIZE RoiExt = 64;
	IMAGE_SIZE MapRoiW = uCellW + RoiExt;
	IMAGE_SIZE MapRoiH = uCellH + RoiExt;
	IMAGE_SIZE MapRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, BitCount, nAlign);
	MapRoiRect.left = MapRoiPosX - (MapRoiW / 2);
	MapRoiRect.top = MapRoiPosY - (MapRoiH / 2);
	MapRoiRect.right = MapRoiPosX + (MapRoiW / 2);
	MapRoiRect.bottom = MapRoiPosY + (MapRoiH / 2);
	CellRoiRect.left = 0;
	CellRoiRect.top = 0;
	CellRoiRect.right = uCellW;
	CellRoiRect.bottom = uCellH;
	ModifyCellRoi = false;
	if (MapRoiRect.left < 0)
	{
		ModifyCellRoi = true;
		CellRoiRect.left = CellRoiRect.left + (0 - MapRoiRect.left);
		MapRoiRect.left = 0;
	}
	if (MapRoiRect.right > nTeachMapW)
	{
		ModifyCellRoi = true;
		CellRoiRect.right = CellRoiRect.right - (MapRoiRect.right - nTeachMapW);
		MapRoiRect.right = nTeachMapW;
	}
	if (MapRoiRect.top < 0)
	{
		ModifyCellRoi = true;
		CellRoiRect.top = CellRoiRect.top + (0 - MapRoiRect.top);
		MapRoiRect.top = 0;
	}
	if (MapRoiRect.bottom > nTeachMapH)
	{
		ModifyCellRoi = true;
		CellRoiRect.bottom = CellRoiRect.bottom - (MapRoiRect.bottom - nTeachMapH);
		MapRoiRect.bottom = nTeachMapH;
	}
	if (CellRoiRect.right <= CellRoiRect.left || CellRoiRect.bottom <= CellRoiRect.top)
	{
		JetMemory.free_func(CellPtr);
#ifdef _DEBUG
		JetAPI::DeleteFileList(SaveFileList);
#endif//_DEBUG
		SaveProjectScratchPartLog(_T("CellRoiRect.right<=CellRoiRect.left || CellRoiRect.bottom<=CellRoiRect.top"));
		return true;
	}
	if (true == ModifyCellRoi)
	{
		if (true == SignX)
		{
			CellRgn.minX += (CellRoiRect.left - 0)*MapRes.x;
			CellRgn.maxX -= (uCellW - CellRoiRect.right)*MapRes.x;
		}
		else
		{
			CellRgn.maxX -= (CellRoiRect.left - 0)*MapRes.x;
			CellRgn.minX += (uCellW - CellRoiRect.right)*MapRes.x;
		}
		if (true == SignY)
		{
			CellRgn.maxY -= (CellRoiRect.top - 0)*MapRes.y;
			CellRgn.minY += (uCellH - CellRoiRect.bottom)*MapRes.y;
		}
		else
		{
			CellRgn.minY += (CellRoiRect.top - 0)*MapRes.y;
			CellRgn.maxY -= (uCellH - CellRoiRect.bottom)*MapRes.y;
		}

		IMAGE_PTR  CellRoiPtr = NULL;
		IMAGE_SIZE CellRoiW = CellRoiRect.right - CellRoiRect.left;
		IMAGE_SIZE CellRoiH = CellRoiRect.bottom - CellRoiRect.top;
		IMAGE_SIZE CellRoiStep = JetAPI::GetBMPImagePixelsPerLine(CellRoiW, BitCount, nAlign);;
		const size_t CellRoiBuffserSize = ImageAPI.CalcBufferSize(CellRoiStep, CellRoiH);
		if (JetMemory.alloc_func(CellRoiBuffserSize, CellRoiPtr, fnName, "CellRoiPtr") == false)
		{
			JetMemory.free_func(CellPtr);
			m_ErrorString = JetMemory.GetErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		if (ImageAPI.ExtractRoiImage3(uCellW, uCellH, uCellStep, BitCount, CellPtr, CellRoiRect, CellRoiStep, CellRoiPtr, false) == false)
		{
			JetMemory.free_func(CellPtr);
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		JetMemory.free_func(CellPtr);
		uCellW = CellRoiW;
		uCellH = CellRoiH;
		uCellStep = CellRoiStep;
		CellPtr = CellRoiPtr;
#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_Cell3.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtr, true);
			SaveFileList.push_back(str);
		}
#endif//_DEBUG
		MapRoiW = MapRoiRect.right - MapRoiRect.left;
		MapRoiH = MapRoiRect.bottom - MapRoiRect.top;
		MapRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, BitCount, nAlign);
	}
	//--------------------------------------------------------------------------------------//
	//
	IMAGE_PTR    CellMedianPtr = NULL;
	IMAGE_PTR    CellEdgePtr = NULL;
	const IMAGE_SIZE uCellMaskStep = JetAPI::GetBMPImagePixelsPerLine(uCellW, MaskBitCount, nAlign);
	const size_t CellMaskSize = uCellMaskStep*uCellH;
	if ((JetMemory.alloc_func(BufferSize, CellMedianPtr, fnName, "CellMedianPtr") == false) ||
		(JetMemory.alloc_func(BufferSize, CellEdgePtr, fnName, "CellEdgePtr") == false))
	{
		m_ErrorString = JetMemory.GetErrorString();
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(CellMedianPtr);
		JetMemory.free_func(CellEdgePtr);
		if (true == bAllocated)
		{
			JetMemory.free_func(ImagePtr);
		}
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	{
		const int KerSize = 3;
		if (ImageAPI.MedianImage3(uCellW, uCellH, uCellStep, BitCount, CellPtr, KerSize, CellMedianPtr) == false) {
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(CellMedianPtr);
			JetMemory.free_func(CellEdgePtr);
			if (true == bAllocated)
			{
				JetMemory.free_func(ImagePtr);
			}
		}
	}

	{//刮起來單純為了避免重複宣告
		int MorphMode = MORPH_GRADIENT;
		int ShapeMode = MORPH_SHAPE_ELLIPSE;//MORPH_SHAPE_RECT, MORPH_SHAPE_CROSS, MORPH_SHAPE_ELLIPSE
		const int KerSize = 3;
		if (ImageAPI.MorphColorImage3(uCellW, uCellH, uCellStep, CellMedianPtr, MorphMode, ShapeMode, KerSize, 1, CellEdgePtr) == false)
			//if (ImageAPI.MorphColorImage3(uCellW, uCellH, uCellStep, CellPtr, MorphMode, ShapeMode, KerSize, 1, CellEdgePtr) == false)
		{
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(CellMedianPtr);
			JetMemory.free_func(CellEdgePtr);
			if (true == bAllocated)
			{
				JetMemory.free_func(ImagePtr);
			}
		}
#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_CellMedianPtr.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellMedianPtr, true);
			SaveFileList.push_back(str);
			str.Format(_T("%s\\%s_%s_CellEdge.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellEdgePtr, true);
			SaveFileList.push_back(str);
		}
#endif//_DEBUG
		JetMemory.free_func(CellMedianPtr);
	}
	//--------------------------------------------------------------------------------------//

	IMAGE_PTR    MapRoiPtr = NULL;
	const size_t MapRoiBufferSize = ImageAPI.CalcBufferSize(MapRoiStep, MapRoiH);

	if (JetMemory.alloc_func(MapRoiBufferSize, MapRoiPtr, fnName, "MapRoiPtr") == false)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(CellEdgePtr);
		JetMemory.free_func(MapRoiPtr);
		m_ErrorString = JetMemory.GetErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	if (ImageAPI.ExtractRoiImage3(nTeachMapW, nTeachMapH, nTeachMapStep, BitCount, TeachMapPtr, MapRoiRect, MapRoiStep, MapRoiPtr, false) == false)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(CellEdgePtr);
		JetMemory.free_func(MapRoiPtr);
		m_ErrorString = ImageAPI.GetImageApiErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
#ifdef _DEBUG
	if (true == bSave)
	{
		str.Format(_T("%s\\%s_%s_MapRoi.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG

	IMAGE_PTR    MapMaskRoiPtr = NULL;
	const IMAGE_SIZE MapMaskRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, MaskBitCount, nAlign);
	const size_t MapMaskRoiBufferSize = ImageAPI.CalcBufferSize(MapMaskRoiStep, MapRoiH);
	if (NULL != TeachMaskPtr && nTeachMaskW == nTeachMapW && nTeachMaskH == nTeachMapH)
	{
		if (JetMemory.alloc_func(MapMaskRoiBufferSize, MapMaskRoiPtr, fnName, "MapMaskRoiPtr") == false)
		{
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(CellEdgePtr);
			JetMemory.free_func(MapRoiPtr);
			JetMemory.free_func(MapMaskRoiPtr);
			m_ErrorString = JetMemory.GetErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		if (ImageAPI.ExtractRoiImage3(nTeachMaskW, nTeachMaskH, nTeachMaskStep, MaskBitCount, TeachMaskPtr, MapRoiRect, MapMaskRoiStep, MapMaskRoiPtr, false) == false)
		{
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(CellEdgePtr);
			JetMemory.free_func(MapRoiPtr);
			JetMemory.free_func(MapMaskRoiPtr);
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_MapMaskRoi.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiPtr, true);
			SaveFileList.push_back(str);
		}
#endif//_DEBUG
		//--------------------------------------------------------------------------------------//
		//const int EdgeExpandSize = GetProjectParameter().m_ScratchPartEdgeExpand;
		const int EdgeExpandSize = 0;
//		if (EdgeExpandSize > 0) {
//			int MorphMode = MORPH_CLOSE;
//			int ShapeMode = MORPH_SHAPE_ELLIPSE;//MORPH_SHAPE_RECT, MORPH_SHAPE_CROSS, MORPH_SHAPE_ELLIPSE
//			const int KerSize = 3; // 去噪
//			const int ErodeKerSize = ImageAPI.GetKernelSize(EdgeExpandSize);//加強邊緣
//			IMAGE_PTR   MapMaskRoiDenoisePtr = NULL;
//			IMAGE_PTR   MapMaskRoiErodePtr = NULL;
//			if (
//				(JetMemory.alloc_func(MapMaskRoiBufferSize, MapMaskRoiDenoisePtr, fnName, "MapMaskRoiDenoisePtr") == false) ||
//				(JetMemory.alloc_func(MapMaskRoiBufferSize, MapMaskRoiErodePtr, fnName, "MapMaskRoiErodePtr") == false)
//				) {
//				JetMemory.free_func(CellPtr);
//				JetMemory.free_func(CellEdgePtr);
//				JetMemory.free_func(MapRoiPtr);
//				JetMemory.free_func(MapMaskRoiPtr);
//				JetMemory.free_func(MapMaskRoiDenoisePtr);
//				JetMemory.free_func(MapMaskRoiErodePtr);
//				m_ErrorString = JetMemory.GetErrorString();
//				SaveProjectScratchPartLog(m_ErrorString);
//				return false;
//			}
//			//去噪
//			if (ImageAPI.MorphGrayImage3(MapRoiW, MapRoiH, MapMaskRoiStep, MapMaskRoiPtr, MorphMode, ShapeMode, KerSize, 1, MapMaskRoiDenoisePtr) == false)
//			{
//				JetMemory.free_func(CellPtr);
//				JetMemory.free_func(CellEdgePtr);
//				JetMemory.free_func(MapRoiPtr);
//				JetMemory.free_func(MapMaskRoiPtr);
//				JetMemory.free_func(MapMaskRoiDenoisePtr);
//				JetMemory.free_func(MapMaskRoiErodePtr);
//				m_ErrorString = ImageAPI.GetImageApiErrorString();
//				SaveProjectScratchPartLog(m_ErrorString);
//				return false;
//			}
//			//加強邊緣
//			if (ImageAPI.ErodeGrayImage3(MapRoiW, MapRoiH, MapMaskRoiStep, MapMaskRoiDenoisePtr, ErodeKerSize, 1, MapMaskRoiErodePtr) == false)
//			{
//				JetMemory.free_func(CellPtr);
//				JetMemory.free_func(CellEdgePtr);
//				JetMemory.free_func(MapRoiPtr);
//				JetMemory.free_func(MapMaskRoiPtr);
//				JetMemory.free_func(MapMaskRoiDenoisePtr);
//				JetMemory.free_func(MapMaskRoiErodePtr);
//				m_ErrorString = ImageAPI.GetImageApiErrorString();
//				SaveProjectScratchPartLog(m_ErrorString);
//				return false;
//			}
//#ifdef _DEBUG
//			if (true == bSave)
//			{
//				str.Format(_T("%s\\%s_%s_MapMaskDenoisePtr.PNG"), Folder, KeyName, MainName);
//				ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiDenoisePtr, true);
//				SaveFileList.push_back(str);
//				str.Format(_T("%s\\%s_%s_MapMaskErode.PNG"), Folder, KeyName, MainName);
//				ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiErodePtr, true);
//				SaveFileList.push_back(str);
//			}
//#endif//_DEBUG
//			JetMemory.free_func(MapMaskRoiPtr);
//			JetMemory.free_func(MapMaskRoiDenoisePtr);
//			MapMaskRoiPtr = MapMaskRoiErodePtr;
//		}
		//--------------------------------------------------------------------------------------//
	}

	//影像對位
	CJetMatch  Match;
	const bool bRobustness = true;
	const int  nMinReduceArea = 4096;
	const int  nFinalReduction = 0;
	const int  nEnableScaleMatch = GetProjectParameter().m_ScratchPartMatchUseScale;
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();
	if (Match.SetMatchLibType(MatchLibType) == false)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(CellEdgePtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		m_ErrorString = Match.GetErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	//initial eMatch
	Match.SetMatchDefaultParam();
	Match.SetRobustness(bRobustness);
	Match.SetMinReducedArea(nMinReduceArea);
	Match.SetFinalReduction(nFinalReduction);
	if (Match.LearnPattern(uCellW, uCellH, uCellStep, BitCount, CellPtr, true) == false)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(CellEdgePtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		m_ErrorString = Match.GetErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	Match.SetInterpolate(true);
	Match.SetMinScore(-1);
	Match.SetMaxPositions(1);
	Match.SetMaxInitialPositions(4);
	if (Match.Match(MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, true) == false)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(CellEdgePtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		m_ErrorString = Match.GetErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	const int NResults = Match.GetNumPositions();
	if (0 == NResults)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(CellEdgePtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		m_ErrorString = _T("Error, Map Match Fault");
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	const int ResultIdx = 0;
	double ResultX = Match.GetResultPosX(ResultIdx);
	double ResultY = Match.GetResultPosY(ResultIdx);
	double ResultA = Match.GetResultAngle(ResultIdx);
	double ResultS = Match.GetResultScore(ResultIdx)*100.0;
	double ResultSX = Match.GetResultScaleX(ResultIdx);
	double ResultSY = Match.GetResultScaleY(ResultIdx);
	if (FN_ENABLE == nEnableScaleMatch)
	{
		const double ResultX_N = Match.GetResultPosX(ResultIdx);
		const double ResultY_N = Match.GetResultPosY(ResultIdx);
		const double ResultA_N = Match.GetResultAngle(ResultIdx);
		const double ResultS_N = Match.GetResultScore(ResultIdx)*100.0;
		const double ResultSX_N = Match.GetResultScaleX(ResultIdx);
		const double ResultSY_N = Match.GetResultScaleY(ResultIdx);

		//再以Scale計算一次
		const float ScaleRange = 0.1f;
		const float ScaleMin = 1.0f - ScaleRange;
		const float ScaleMax = 1.0f + ScaleRange;

		Match.SetMinScale(ScaleMin);
		Match.SetMinScaleX(ScaleMin);
		Match.SetMinScaleY(ScaleMin);
		Match.SetMaxScale(ScaleMax);
		Match.SetMaxScaleX(ScaleMax);
		Match.SetMaxScaleY(ScaleMax);
		Match.SetUseScale(true);
		if (Match.Match(MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, true) == false)
		{
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(CellEdgePtr);
			JetMemory.free_func(MapRoiPtr);
			JetMemory.free_func(MapMaskRoiPtr);
			m_ErrorString = Match.GetErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		const int NResults_F = Match.GetNumPositions();
		if (NResults_F > 0)
		{
			double ResultX_F = Match.GetResultPosX(ResultIdx);
			double ResultY_F = Match.GetResultPosY(ResultIdx);
			double ResultA_F = Match.GetResultAngle(ResultIdx);
			double ResultS_F = Match.GetResultScore(ResultIdx)*100.0;
			double ResultSX_F = Match.GetResultScaleX(ResultIdx);
			double ResultSY_F = Match.GetResultScaleY(ResultIdx);
			//差太多使用沒有內插的, 以及找最相似度最高的
			if (fabs(ResultX_F - ResultX_N)>10 || fabs(ResultY_F - ResultY_N)>10 || ResultS_F<ResultS_N)
			{
				ResultX = ResultX_N;
				ResultY = ResultY_N;
				ResultA = ResultA_N;
				ResultS = ResultS_N;
				ResultSX = ResultSX_N;
				ResultSY = ResultSY_N;
			}
			else
			{
				ResultX = ResultX_F;
				ResultY = ResultY_F;
				ResultA = ResultA_F;
				ResultS = ResultS_F;
				ResultSX = ResultSX_F;
				ResultSY = ResultSY_F;
			}
		}
	}
	const int    nResultX = (int)(ResultX + 0.5);
	const int    nResultY = (int)(ResultY + 0.5);
	const double ResultX_Start = nResultX - (uCellW / 2);
	const double ResultY_Start = nResultY - (uCellH / 2);
	//const double ResultX_Start = ResultX-(uCellW*0.5);
	//const double ResultY_Start = ResultY-(uCellH*0.5);
	//const int    nCellPosX = (int)(ResultX_Start+0.5);
	//const int    nCellPosY = (int)(ResultY_Start+0.5);	
	const int    nCellPosX = JetAPI::Floor(ResultX_Start);
	const int    nCellPosY = JetAPI::Floor(ResultY_Start);
	const int    nCellPosXEnd = nCellPosX + uCellW;
	const int    nCellPosYEnd = nCellPosY + uCellH;
	if (nCellPosX<0 || nCellPosY<0 || ResultS<50.0 || nCellPosXEnd>MapRoiW || nCellPosYEnd>MapRoiH)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(CellEdgePtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		m_ErrorString = _T("Error, Map Match Result Fault");

		FILE *pfile = NULL;
		str.Format(_T("%s\\%s_%s_MatchFaultReportScale.TXT"), Folder, KeyName, MainName);
		pfile = _tfopen(str, _T("w+"));
		if (NULL != pfile)
		{
			::_ftprintf(pfile, _T("Result X:%.4f\n"), ResultX);
			::_ftprintf(pfile, _T("Result Y:%.4f\n"), ResultY);
			::_ftprintf(pfile, _T("Result Angle:%.4f\n"), ResultA);
			::_ftprintf(pfile, _T("Result Score:%.4f\n"), ResultS);
			::_ftprintf(pfile, _T("Result Scale X:%.4f\n"), ResultSX);
			::_ftprintf(pfile, _T("Result Scale Y:%.4f\n"), ResultSY);

			::_ftprintf(pfile, _T("Result X Start:%.4f\n"), ResultX_Start);
			::_ftprintf(pfile, _T("Result Y Start:%.4f\n"), ResultY_Start);
			::_ftprintf(pfile, _T("Cell Start Pos X:%d\n"), nCellPosX);
			::_ftprintf(pfile, _T("Cell Start Pos Y:%d\n"), nCellPosY);
			::fclose(pfile);	pfile = NULL;
		}
		// 
#ifdef _DEBUG
		//JetAPI::DeleteFileList(SaveFileList);
#endif//_DEBUG		
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	MASK_PTR     RawMaskPtrL = NULL;
	MASK_PTR     TestMaskPtr = NULL;
	MASK_PTR     TestMaskPtr1 = NULL;
	MASK_PTR     TestMaskPtr2 = NULL;
	MASK_PTR     TestMaskPtrR = NULL;
	MASK_PTR     TestMaskPtrG = NULL;
	MASK_PTR     TestMaskPtrB = NULL;
	MASK_PTR     TestMaskPtrV = NULL;
	const IMAGE_SIZE MaskW = uCellW;
	const IMAGE_SIZE MaskH = uCellH;
	const IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(MaskW, MaskBitCount, 4);
	const size_t MaskBufferSize = ImageAPI.CalcBufferSize(MaskStep, MaskH);
	if (JetMemory.alloc_func(MaskBufferSize, RawMaskPtrL, fnName, "RawMaskPtrL") == false ||
		JetMemory.alloc_func(MaskBufferSize, TestMaskPtr, fnName, "TestMaskPtr") == false ||
		JetMemory.alloc_func(MaskBufferSize, TestMaskPtr1, fnName, "TestMaskPtr1") == false ||
		JetMemory.alloc_func(MaskBufferSize, TestMaskPtr2, fnName, "TestMaskPtr2") == false ||
		JetMemory.alloc_func(MaskBufferSize, TestMaskPtrR, fnName, "TestMaskPtrR") == false ||
		JetMemory.alloc_func(MaskBufferSize, TestMaskPtrG, fnName, "TestMaskPtrG") == false ||
		JetMemory.alloc_func(MaskBufferSize, TestMaskPtrB, fnName, "TestMaskPtrB") == false ||
		JetMemory.alloc_func(MaskBufferSize, TestMaskPtrV, fnName, "TestMaskPtrV") == false)
	{
		m_ErrorString = JetMemory.GetErrorString();
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(CellEdgePtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		JetMemory.free_func(TestMaskPtrR);
		JetMemory.free_func(TestMaskPtrG);
		JetMemory.free_func(TestMaskPtrB);
		JetMemory.free_func(TestMaskPtrV);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	//剔除此FOV內的零件
	size_t       i = 0, j = 0;
	TREGION4D    FieldRgn = CellRgn;
	const int    MaskMode = PROJECT_PART_MASK_BODY | PROJECT_CODE_MASK_BODY;
	::memset(TestMaskPtr1, 0xFF, sizeof(MASK_DATA)*MaskBufferSize);
	if (CreateProjectPartMaskImage_v2(MaskW, MaskH, MaskStep, TestMaskPtr1, FieldRgn, MapRes, MaskMode, 0x00) == false)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(CellEdgePtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		JetMemory.free_func(TestMaskPtrR);
		JetMemory.free_func(TestMaskPtrG);
		JetMemory.free_func(TestMaskPtrB);
		JetMemory.free_func(TestMaskPtrV);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
#ifdef _DEBUG
	if (true == bSave)
	{
		str.Format(_T("%s\\%s_%s_MaskNoPart.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr1, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG
	if (ImageAPI.MorphGrayImage3(MaskW, MaskH, MaskStep, TestMaskPtr1, MORPH_OPEN, MORPH_SHAPE_RECT, 9, 1, TestMaskPtr2) == false)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(CellEdgePtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		JetMemory.free_func(TestMaskPtrR);
		JetMemory.free_func(TestMaskPtrG);
		JetMemory.free_func(TestMaskPtrB);
		JetMemory.free_func(TestMaskPtrV);
		m_ErrorString = ImageAPI.GetImageApiErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
#ifdef _DEBUG
	if (true == bSave)
	{
		str.Format(_T("%s\\%s_%s_MaskNoPartOpen.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG	

	//::memcpy(TestMaskPtr, TestMaskPtr2, sizeof(MASK_DATA)*MaskBufferSize);
	//::memset(TestMaskPtr, 0xFF, sizeof(MASK_DATA)*MaskBufferSize);

	//依據遮罩來比較影像是否有問題	
	size_t MaskIdx = 0;
	size_t MapMaskIdx = 0;
	size_t CellIdx = 0, RoiIdx = 0;
	IMAGE_PTR  CellPtrRaw = NULL;
	IMAGE_PTR  MapRoiPtrRaw = NULL;
	const size_t CellSize = ImageAPI.CalcBufferSize(uCellStep, uCellH);
	const size_t MapRoiSize = ImageAPI.CalcBufferSize(MapRoiStep, MapRoiH);
	if (JetMemory.alloc_func(CellSize, CellPtrRaw, fnName, "CellPtrRaw") == false ||
		JetMemory.alloc_func(MapRoiSize, MapRoiPtrRaw, fnName, "MapRoiPtrRaw") == false)
	{
		m_ErrorString = JetMemory.GetErrorString();
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(CellEdgePtr);
		JetMemory.free_func(CellPtrRaw);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapRoiPtrRaw);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		JetMemory.free_func(TestMaskPtrR);
		JetMemory.free_func(TestMaskPtrG);
		JetMemory.free_func(TestMaskPtrB);
		JetMemory.free_func(TestMaskPtrV);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	::memset(RawMaskPtrL, 0x00, sizeof(MASK_DATA)*MaskBufferSize);
	const int SmoothSize = GetProjectParameter().m_ScratchPartSmoothSize;
	//const int SmoothSize = 0;
	if (SmoothSize > 0)
	{
		IMAGE_PTR SwapPtr = NULL;
		const int nSmoothSize = ImageAPI.GetKernelSize(SmoothSize);
		if (ImageAPI.SmoothImage3(uCellW, uCellH, uCellStep, BitCount, CellPtr, nSmoothSize, CellPtrRaw) == true)
		{
			SwapPtr = CellPtrRaw;
			CellPtrRaw = CellPtr;
			CellPtr = SwapPtr;
		}
		else
		{
			::memcpy(CellPtrRaw, CellPtr, sizeof(IMAGE_DATA)*CellSize);
		}

		if (ImageAPI.SmoothImage3(MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, nSmoothSize, MapRoiPtrRaw) == true)
		{
			SwapPtr = MapRoiPtrRaw;
			MapRoiPtrRaw = MapRoiPtr;
			MapRoiPtr = SwapPtr;
		}
		else
		{
			::memcpy(MapRoiPtrRaw, MapRoiPtr, sizeof(IMAGE_DATA)*MapRoiSize);
		}
#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_CellSmooth.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtr, true);
			SaveFileList.push_back(str);

			str.Format(_T("%s\\%s_%s_MapRoiSmooth.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, true);
			SaveFileList.push_back(str);
		}
#endif//_DEBUG
	}
	else
	{
		::memcpy(CellPtrRaw, CellPtr, sizeof(IMAGE_DATA)*CellSize);
		::memcpy(MapRoiPtrRaw, MapRoiPtr, sizeof(IMAGE_DATA)*MapRoiSize);
	}

	const int MinGrayscale = GetProjectParameter().m_ScratchPartMinGrayscale;
	const int MaxGrayscale = GetProjectParameter().m_ScratchPartMaxGrayscale;
	size_t s = 0, t = 0, usedCnt = 0;
	unsigned char R = 0, G = 0, B = 0, GrayScale = 0;
	unsigned char IR = 0, IG = 0, IB = 0, IV = 0;
	float wR = 0.299f, wG = 0.587f, wB = 0.114f;
	//--------------------------------------------------------------------------------------//
	unsigned char REdge = 0, GEdge = 0, BEdge = 0;
	unsigned char IREdge = 0, IGEdge = 0, IBEdge = 0, IVEdge = 0;
	const int MinVEdge = GetProjectParameter().m_ScratchPartEdgeThreshold;

	//--------------------------------------------------------------------------------------//
	int MinR = 0, MinG = 0, MinB = 0, MinV = 0;
	int MaxR = 0, MaxG = 0, MaxB = 0, MaxV = 0;
	const size_t CalcSizeMin = 4;
	const int     ColorExpand = GetProjectParameter().m_ScratchPartColorExpand;
	const double  CalcSizeWum = GetProjectParameter().m_ScratchPartCalcSizeW;
	const double  CalcSizeHum = GetProjectParameter().m_ScratchPartCalcSizeH;
	size_t WndSizeW = (size_t)((CalcSizeWum / MapRes.x) + 0.5);
	size_t WndSizeH = (size_t)((CalcSizeHum / MapRes.y) + 0.5);
	if (WndSizeW < CalcSizeMin) { WndSizeW = CalcSizeMin; }
	if (WndSizeH < CalcSizeMin) { WndSizeH = CalcSizeMin; }
	const size_t WndSizeW2 = WndSizeW / 2;
	const size_t WndSizeH2 = WndSizeH / 2;

	::memset(RawMaskPtrL, 0x00, sizeof(MASK_DATA)*MaskBufferSize);
	if (24 == BitCount)
	{
		//將RGB轉成RGBV
		for (i = 0; i<MaskH; i++)
		{
			for (j = 0; j<MaskW; j++)
			{
				MaskIdx = i*MaskStep + j;
#ifdef SCRATCH_DEBUG
				int cornerSize = 20;
				//bool bCorner =
				//	(i < cornerSize && j < cornerSize) ||                                 // 左上角
				//	(i < cornerSize && j > MaskW - cornerSize - 1) ||                     // 右上角
				//	(i > MaskH - cornerSize - 1 && j < cornerSize) ||                     // 左下角
				//	(i > MaskH - cornerSize - 1 && j > MaskW - cornerSize - 1);           // 右下角
				bool bFilled =
					(((i / cornerSize) + (j / cornerSize)) % 2) &&
					(i < cornerSize || j < cornerSize) ||
					(i < cornerSize && j > MaskW - cornerSize - 1) ||                     // 右上角
					(i > MaskH - cornerSize - 1 && j < cornerSize) ||                     // 左下角
					(i > MaskH - cornerSize - 1 && j > MaskW - cornerSize - 1);           // 右下角
				;

				if (bFilled) {//bCorner) {
					RawMaskPtrL[MaskIdx] = 255;
					continue;
				}
#endif // _SCRATCH_DEBUG

				RoiIdx = ((i + nCellPosY)*MapRoiStep) + (j + nCellPosX) * 3;
				B = MapRoiPtr[RoiIdx];
				G = MapRoiPtr[RoiIdx + 1];
				R = MapRoiPtr[RoiIdx + 2];

				if (ImageAPI.RGBConvertToRGBV(R, G, B, IR, IG, IB, IV) == false)
				{
					TestMaskPtrR[MaskIdx] = 0;
					TestMaskPtrG[MaskIdx] = 0;
					TestMaskPtrB[MaskIdx] = 0;
					TestMaskPtrV[MaskIdx] = 0;
					continue;
				}
				TestMaskPtrR[MaskIdx] = IR;
				TestMaskPtrG[MaskIdx] = IG;
				TestMaskPtrB[MaskIdx] = IB;
				TestMaskPtrV[MaskIdx] = IV;
			}
		}


		for (i = WndSizeH2; i<MaskH - WndSizeH2; i++)
		{
			for (j = WndSizeW2; j<MaskW - WndSizeW2; j++)
			{
				MaskIdx = i*MaskStep + j;
				if (0 == TestMaskPtr2[MaskIdx])
				{
					continue;
				}
				if (NULL != MapMaskRoiPtr)
				{
					MapMaskIdx = ((i + nCellPosY)*MapMaskRoiStep) + (j + nCellPosX);
					if (0 == MapMaskRoiPtr[MapMaskIdx])
					{
						continue;
					}
				}

				//Get Color Filter Range (at Map)
				usedCnt = 0;
				MaxR = MaxG = MaxB = MaxV = 0;
				MinR = MinG = MinB = MinV = 255;
				for (t = i - WndSizeH2; t<i + WndSizeH2; t++)
				{
					for (s = j - WndSizeW2; s<j + WndSizeW2; s++)
					{
						MaskIdx = t*MaskStep + s;
						if (0 == TestMaskPtr2[MaskIdx])
						{
							continue;
						}
						if (NULL != MapMaskRoiPtr)
						{
							MapMaskIdx = ((t + nCellPosY)*MapMaskRoiStep) + (s + nCellPosX);
							if (0 == MapMaskRoiPtr[MapMaskIdx])
							{
								continue;
							}
						}

						IR = TestMaskPtrR[MaskIdx];
						IG = TestMaskPtrG[MaskIdx];
						IB = TestMaskPtrB[MaskIdx];
						IV = TestMaskPtrV[MaskIdx];

						if (IR < MinR) { MinR = IR; }
						if (IG < MinG) { MinG = IG; }
						if (IB < MinB) { MinB = IB; }
						if (IV < MinV) { MinV = IV; }

						if (IR > MaxR) { MaxR = IR; }
						if (IG > MaxG) { MaxG = IG; }
						if (IB > MaxB) { MaxB = IB; }
						if (IV > MaxV) { MaxV = IV; }
						usedCnt++;
					}
				}
				if (usedCnt < CalcSizeMin) { continue; }

				MaskIdx = i*MaskStep + j;
				MinR = MAX(MinR - ColorExpand, 0);
				MinG = MAX(MinG - ColorExpand, 0);
				MinB = MAX(MinB - ColorExpand, 0);
				MinV = MAX(MinV - ColorExpand, 0);
				MaxR = MIN(MaxR + ColorExpand, 255);
				MaxG = MIN(MaxG + ColorExpand, 255);
				MaxB = MIN(MaxB + ColorExpand, 255);
				MaxV = MIN(MaxV + ColorExpand, 255);

				CellIdx = (i*uCellStep) + (j * 3);
				B = CellPtr[CellIdx];
				G = CellPtr[CellIdx + 1];
				R = CellPtr[CellIdx + 2];
				GrayScale = B*wB + G*wG + R*wR;
				if (GrayScale<MinGrayscale || GrayScale>MaxGrayscale) { continue; }

				BEdge = CellEdgePtr[CellIdx];
				GEdge = CellEdgePtr[CellIdx + 1];
				REdge = CellEdgePtr[CellIdx + 2];
				//if (ImageAPI.RGBConvertToRGBV(REdge, GEdge, BEdge, IREdge, IGEdge, IBEdge, IVEdge) == false) { continue; }
				IVEdge = (BEdge + GEdge + REdge) / 3;
				if (IVEdge < MinVEdge) { continue; }//邊緣圖的亮度與設定的Threshold比較
				if (ImageAPI.RGBConvertToRGBV(R, G, B, IR, IG, IB, IV) == false) { continue; }
				if (IR<MinR || IR>MaxR) { RawMaskPtrL[MaskIdx] = 255; }
				if (IG<MinG || IG>MaxG) { RawMaskPtrL[MaskIdx] = 255; }
				if (IB<MinB || IB>MaxB) { RawMaskPtrL[MaskIdx] = 255; }
				if (IV<MinV || IV>MaxV) { RawMaskPtrL[MaskIdx] = 255; }
			}
		}
	}
	else
	{	//8-Bit Image 
		// 未修改
		for (i = WndSizeH2; i<MaskH - WndSizeH2; i++)
		{
			for (j = WndSizeW2; j<MaskW - WndSizeW2; j++)
			{
				MaskIdx = i*MaskStep + j;
				if (0 == TestMaskPtr2[MaskIdx])
				{
					continue;
				}
				if (NULL != MapMaskRoiPtr)
				{
					MapMaskIdx = ((i + nCellPosY)*MapMaskRoiStep) + (j + nCellPosX);
					if (0 == MapMaskRoiPtr[MapMaskIdx])
					{
						continue;
					}
				}

				//Get Gray Range (at Map)
				usedCnt = 0;
				MaxR = 0;
				MinR = 255;
				for (t = i - WndSizeH2; t<i + WndSizeH2; t++)
				{
					for (s = j - WndSizeW2; s<j + WndSizeW2; s++)
					{
						MaskIdx = t*MaskStep + s;
						if (0 == TestMaskPtr2[MaskIdx])
						{
							continue;
						}
						if (NULL != MapMaskRoiPtr)
						{
							MapMaskIdx = ((t + nCellPosY)*MapMaskRoiStep) + (s + nCellPosX);
							if (0 == MapMaskRoiPtr[MapMaskIdx])
							{
								continue;
							}
						}

						//CellIdx = (t*uCellStep)+(s);
						RoiIdx = ((t + nCellPosY)*MapRoiStep) + (s + nCellPosX);

						//IR = CellPtr[CellIdx];
						IR = MapRoiPtr[RoiIdx];
						if (IR < MinR) { MinR = IR; }
						if (IR > MaxR) { MinR = IR; }
						usedCnt++;
					}
				}
				if (usedCnt < CalcSizeMin) { continue; }

				MaskIdx = i*MaskStep + j;
				MinR = MAX(MinR - ColorExpand, 0);
				MaxR = MIN(MaxR + ColorExpand, 255);

				CellIdx = (i*uCellStep) + (j);
				//RoiIdx = ((i+nCellPosY)*MapRoiStep)+(j+nCellPosX);
				IR = CellPtr[CellIdx];
				if (IR<MinR || IR>MaxR) { RawMaskPtrL[MaskIdx] = 255; }
			}
		}
	}
#ifdef _DEBUG
	if (true == bSave)
	{
		str.Format(_T("%s\\%s_%s_MaskCompareRaw.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, RawMaskPtrL, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG
	bool KeepImage = false;
#ifndef _DEBUG
	KeepImage = SaveImage;
#endif//_DEBUG
	JetMemory.free_func(CellPtr);
	JetMemory.free_func(CellEdgePtr);
	JetMemory.free_func(MapRoiPtr);
	if (false == KeepImage)
	{
		JetMemory.free_func(CellPtrRaw);
		JetMemory.free_func(MapRoiPtrRaw);
		JetMemory.free_func(MapMaskRoiPtr);
	}

	::memcpy(TestMaskPtr, RawMaskPtrL, sizeof(MASK_DATA)*MaskBufferSize);
	::memcpy(TestMaskPtr2, RawMaskPtrL, sizeof(MASK_DATA)*MaskBufferSize);

	int MorphMode = 0;
	int   ShapeMode = MORPH_SHAPE_ELLIPSE;//MORPH_SHAPE_RECT, MORPH_SHAPE_CROSS, MORPH_SHAPE_ELLIPSE
	const int NoiseFilterOpen = GetProjectParameter().m_ScratchPartFilterOpenSize;
	if (NoiseFilterOpen > 0)
	{
		MorphMode = MORPH_OPEN;
		const int OpenSize = ImageAPI.GetKernelSize(NoiseFilterOpen);
		if (ImageAPI.MorphGrayImage3(MaskW, MaskH, MaskStep, TestMaskPtr, MorphMode, ShapeMode, OpenSize, 1, TestMaskPtr2) == false)
		{
			JetMemory.free_func(CellPtrRaw);
			JetMemory.free_func(MapRoiPtrRaw);
			JetMemory.free_func(MapMaskRoiPtr);
			JetMemory.free_func(RawMaskPtrL);
			JetMemory.free_func(TestMaskPtr);
			JetMemory.free_func(TestMaskPtr1);
			JetMemory.free_func(TestMaskPtr2);
			JetMemory.free_func(TestMaskPtrR);
			JetMemory.free_func(TestMaskPtrG);
			JetMemory.free_func(TestMaskPtrB);
			JetMemory.free_func(TestMaskPtrV);
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		::memcpy(TestMaskPtr, TestMaskPtr2, sizeof(MASK_DATA)*MaskBufferSize);
#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_MaskCompareOpen.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
			SaveFileList.push_back(str);
		}
#endif//_DEBUG
	}
	const int NoiseFilterClose = GetProjectParameter().m_ScratchPartFilterCloseSize;
	if (NoiseFilterClose > 0)
	{
		MorphMode = MORPH_CLOSE;
		const int CloseSize = ImageAPI.GetKernelSize(NoiseFilterClose);
		if (ImageAPI.MorphGrayImage3(MaskW, MaskH, MaskStep, TestMaskPtr, MorphMode, ShapeMode, CloseSize, 1, TestMaskPtr2) == false)
		{
			JetMemory.free_func(CellPtrRaw);
			JetMemory.free_func(MapRoiPtrRaw);
			JetMemory.free_func(MapMaskRoiPtr);
			JetMemory.free_func(RawMaskPtrL);
			JetMemory.free_func(TestMaskPtr);
			JetMemory.free_func(TestMaskPtr1);
			JetMemory.free_func(TestMaskPtr2);
			JetMemory.free_func(TestMaskPtrR);
			JetMemory.free_func(TestMaskPtrG);
			JetMemory.free_func(TestMaskPtrB);
			JetMemory.free_func(TestMaskPtrV);
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		::memcpy(TestMaskPtr, TestMaskPtr2, sizeof(MASK_DATA)*MaskBufferSize);
#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_MaskCompareClose.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
			SaveFileList.push_back(str);
		}
#endif//_DEBUG
	}

	const bool bMeasure = false;
	const double MinThickness = 0;
	std::vector<CAOIComponent*> NewComponentList;
	std::vector<RECT> NewRectList;
	const double PartWHRatio = -1;
	const double PartW = GetProjectParameter().m_ScratchPartMinSizeW;
	const double PartH = GetProjectParameter().m_ScratchPartMinSizeH;
	const double PartD = GetProjectParameter().m_ScratchPartMinSizeD;
	const double PartArea = GetProjectParameter().m_ScratchPartMinPixels;
	if (CreateProjectComponentScatchByBlob(MaskW, MaskH, MaskStep, TestMaskPtr2, NULL, NULL, FieldRgn, MapRes, PartW, PartH, PartD, PartArea, PartWHRatio, MinThickness, bMeasure, NewComponentList, NewRectList) == false)
		//if (CreateProjectComponentScatchByBlob(MaskW, MaskH, MaskStep, TestMaskPtr2, NULL, NULL, FieldRgn, MapRes, PartW, PartH, PartD, PartArea, PartWHRatio, MinThickness, bMeasure, NewComponentList) == false)
	{
		JetMemory.free_func(CellPtrRaw);
		JetMemory.free_func(MapRoiPtrRaw);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		JetMemory.free_func(TestMaskPtrR);
		JetMemory.free_func(TestMaskPtrG);
		JetMemory.free_func(TestMaskPtrB);
		JetMemory.free_func(TestMaskPtrV);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	JetMemory.free_func(RawMaskPtrL);
	JetMemory.free_func(TestMaskPtr);
	JetMemory.free_func(TestMaskPtr1);
	JetMemory.free_func(TestMaskPtr2);
	JetMemory.free_func(TestMaskPtrR);
	JetMemory.free_func(TestMaskPtrG);
	JetMemory.free_func(TestMaskPtrB);
	JetMemory.free_func(TestMaskPtrV);

	const size_t MaxComponentInFov = 36;
	const size_t NewComponentCount = NewComponentList.size();
	if (0 == NewComponentCount)//|| NewComponentCount>MaxComponentInFov )
	{
		JetMemory.free_func(CellPtrRaw);
		JetMemory.free_func(MapRoiPtrRaw);
		JetMemory.free_func(MapMaskRoiPtr);
#ifdef _DEBUG
		JetAPI::DeleteFileList(SaveFileList);
#endif//_DEBUG
		SaveProjectScratchPartLog(_T("0 == NewComponentCount"));
		return true;
	}

	int  nSaveTxt = SaveImage;
#ifdef _DEBUG
	nSaveTxt = FN_ENABLE;
#endif//_DEBUG
	if (FN_ENABLE == nSaveTxt)
	{
		FILE *pfile = NULL;
		str.Format(_T("%s\\%s_%s_MatchResult.TXT"), Folder, KeyName, MainName);
		pfile = _tfopen(str, _T("w+"));
		if (NULL != pfile)
		{
			::_ftprintf(pfile, _T("Result X:%.4f\n"), ResultX);
			::_ftprintf(pfile, _T("Result Y:%.4f\n"), ResultY);
			::_ftprintf(pfile, _T("Result Angle:%.4f\n"), ResultA);
			::_ftprintf(pfile, _T("Result Score:%.4f\n"), ResultS);
			::_ftprintf(pfile, _T("Result Scale X:%.4f\n"), ResultSX);
			::_ftprintf(pfile, _T("Result Scale Y:%.4f\n"), ResultSY);

			::_ftprintf(pfile, _T("Result X Start:%.4f\n"), ResultX_Start);
			::_ftprintf(pfile, _T("Result Y Start:%.4f\n"), ResultY_Start);
			::_ftprintf(pfile, _T("Cell Start Pos X:%d\n"), nCellPosX);
			::_ftprintf(pfile, _T("Cell Start Pos Y:%d\n"), nCellPosY);
			::fclose(pfile);	pfile = NULL;
		}
	}

#ifndef _DEBUG
	if (FN_ENABLE == SaveImage)
	{
		str.Format(_T("%s\\%s_%s_MapRoi.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtrRaw, true);
		str.Format(_T("%s\\%s_%s_MapMaskRoi.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiPtr, true);
		str.Format(_T("%s\\%s_%s_Cell3.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtrRaw, true);
	}
#endif//_DEBUG
	JetMemory.free_func(CellPtrRaw);
	JetMemory.free_func(MapRoiPtrRaw);
	JetMemory.free_func(MapMaskRoiPtr);

	if (CreateProjectScratchPartByComponentList(FieldPtr, NewComponentList) == false)
	{
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	if (SaveProjectSpecTestField(FieldPtr) == false)
	{
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::TestProjectScratchPart_v4(CAOIField * FieldPtr)
{
	const char fnName[] = "CAOIProject::TestProjectScratchPartByColorFilter";
	const int TestScratchPart = GetProjectTestScratchPart();
	if (FN_DISABLE == TestScratchPart) { return true; }

	if (NULL == FieldPtr) {
		m_ErrorString.Format(_T("Error, Project Test Scratch Part Field Execption"));
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	if (FieldPtr->CheckFieldMergeFinish() == false) { return true; }
	const int TestFrameIndex = GetProjectParameter().m_ScratchPartFrameIndex;
	CAOIFrame *FramePtr = FieldPtr->GetFieldFramePtr(TestFrameIndex, true);
	if (NULL == FramePtr)
	{
		m_ErrorString.Format(_T("Error, Project Test Scratch Part Frame index Execption [%d]"), TestFrameIndex + 1);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	CString      str;
	bool         bAllocated = false;
	const int    MapIndex = TestFrameIndex;
	FRAME_TYPE   FrameType = FramePtr->GetFrameType();
	IMAGE_SIZE   ImageW = FramePtr->GetFrameImageW();
	IMAGE_SIZE   ImageH = FramePtr->GetFrameImageH();
	IMAGE_SIZE   ImageStep = FramePtr->GetFrameImageStep();
	IMAGE_SIZE   BitCount = FramePtr->GetFrameImageBitCount();
	IMAGE_PTR    ImagePtr = FramePtr->GetFrameImagePtr();
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const IMAGE_SIZE MaskBitCount = 8;
	const int SaveImage = GetProjectScratchPartSaveImage();

	const int nAlign = 4;
	const int nTeachMapW = (int)(m_ProjectMapW[MapIndex]);//專案底圖寬度
	const int nTeachMapH = (int)(m_ProjectMapH[MapIndex]);//專案底圖長度
	const int nTeachMapStep = (int)(m_ProjectMapStep[MapIndex]);//專案底圖步長
	const int nTeachMapBitCount = (int)(m_ProjectBitCount[MapIndex]);//專案底圖位元數
	const int nTeachMapW2 = nTeachMapW / 2;
	const int nTeachMapH2 = nTeachMapH / 2;
	IMAGE_PTR TeachMapPtr = m_ProjectMapPtr[MapIndex];//專案底圖指標

	const int nTeachMaskW = (int)(m_ProjectMapMaskW);
	const int nTeachMaskH = (int)(m_ProjectMapMaskH);
	const int nTeachMaskStep = (int)(m_ProjectMapMaskStep);
	const int nTeachMaskBitCount = (int)(m_ProjectMapMaskBitCount);
	IMAGE_PTR TeachMaskPtr = m_ProjectMapMaskPtr;
	
	if (NULL == TeachMapPtr)
	{
		m_ErrorString.Format(_T("Error, Project Test Scratch Part NULL == TeachMapPtr [%d]"), MapIndex + 1);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	if (FRAME_BAYER == FrameType)
	{
		IMAGE_SIZE   DeBayerBit = 0;
		IMAGE_SIZE   DeBayerStep = 0;
		IMAGE_PTR    DeBayerPtr = NULL;
		BAYER_PATTERN_MODE BayerPattern = FramePtr->GetFrameBayerPattern();
		if (AOIDataCollect.ExecDebayerImage(fnName, ImageW, ImageH, ImageStep, ImagePtr, BayerPattern, DeBayerStep, DeBayerBit, DeBayerPtr) == false)
		{
			m_ErrorString = AOIDataCollect.GetErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		ImagePtr = DeBayerPtr;
		BitCount = DeBayerBit;
		ImageStep = DeBayerStep;
		bAllocated = true;
		DeBayerPtr = NULL;
	}
	if (BitCount != nTeachMapBitCount)
	{
		if (true == bAllocated)
		{
			JetMemory.free_func(ImagePtr);
		}
		m_ErrorString.Format(_T("Error, Project Test Scratch Part BitCount != nTeachMapBitCount [%d]"), MapIndex + 1);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	const int    NPixels = GetProjectMapScaleMode();
	IMAGE_PTR    CellPtr = NULL;
	IMAGE_SIZE   uCellW = 0;
	IMAGE_SIZE   uCellH = 0;
	IMAGE_SIZE   uCellStep = 0;

#ifdef _DEBUG
	bool         bSave = true;
#endif _DEBUG
	CString      Folder;
	CString      MainName;
	CString      KeyName = _T("Scratch");
	CString      FrameFileName = FramePtr->GetFrameFileName();
	std::vector<CString> SaveFileList;
	Folder = GetProjectDebugFolderScratch();
	JetAPI::ExtractMainFileName(FrameFileName, MainName);

	if (JetMemory.alloc_func(BufferSize, CellPtr, fnName, "CellPtr") == false)
	{
		m_ErrorString = JetMemory.GetErrorString();
		JetMemory.free_func(CellPtr);
		if (true == bAllocated)
		{
			JetMemory.free_func(ImagePtr);
		}
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	const double ScaleVal = 1.00 / NPixels;
	ImageAPI.CalcScaleSize(ImageW, ImageH, ImageStep, BitCount, ScaleVal, uCellW, uCellH, uCellStep);
	if (ImageAPI.ScaleImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ScaleVal, uCellW, uCellH, uCellStep, CellPtr) == false)
		//if ( ImageAPI.FastScaleImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, NPixels, uCellW, uCellH, uCellStep, CellPtr) == false )	
	{
		JetMemory.free_func(CellPtr);
		if (true == bAllocated)
		{
			JetMemory.free_func(ImagePtr);
		}
		m_ErrorString = ImageAPI.GetImageApiErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	

#ifdef _DEBUG
	if (true == bSave)
	{
		str.Format(_T("%s\\%s_%s_Cell.PNG"), Folder, KeyName, MainName);
		//ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtr, true);
		//SaveFileList.push_back(str);
	}
#endif//_DEBUG

	/*
	if ( AOIDataCollect.ExecEnhanceDisplayImage(uCellW, uCellH, uCellStep, BitCount, CellPtr, CellPtr) == false )
	{
	JetMemory.free_func(CellPtr);
	if ( true == bAllocated )
	{	JetMemory.free_func(ImagePtr); }
	m_ErrorString = ImageAPI.GetImageApiErrorString();
	SaveProjectScratchPartLog(m_ErrorString);
	return false;
	}
	*/
#ifdef _DEBUG
	if (true == bSave)
	{
		str.Format(_T("%s\\%s_%s_Cell2.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG
	if (true == bAllocated)
	{
		JetMemory.free_func(ImagePtr);
	}
	//尋找在專案底圖的基本方位	
	TPOINT2D MapRes;
	TREGION4D CadRgn;
	TREGION4D CellRgn;
	TREGION4D StageRgn;
	TREGION4D TeachRgn;
	const double FieldStagePosX = FieldPtr->GetFieldStagePosX();
	const double FieldStagePosY = FieldPtr->GetFieldStagePosY();
	const double FieldStageSizeW = FieldPtr->GetFieldSizeW_Real();
	const double FieldStageSizeH = FieldPtr->GetFieldSizeH_Real();

	GetProjectMapInfo(MapRes, CadRgn, StageRgn);
	GetProjectMapCalcRgn(TeachRgn);
	TeachRgn = StageRgn;

	CellRgn.minX = FieldStagePosX - (FieldStageSizeW*0.5);
	CellRgn.minY = FieldStagePosY - (FieldStageSizeH*0.5);
	CellRgn.maxX = FieldStagePosX + (FieldStageSizeW*0.5);
	CellRgn.maxY = FieldStagePosY + (FieldStageSizeH*0.5);

	int MapRoiPosX = 0;
	int MapRoiPosY = 0;
	const double MapTeachCpX = TeachRgn.GetCpX();
	const double MapTeachCpY = TeachRgn.GetCpY();
	const double RgnOffsetX = (FieldStagePosX - MapTeachCpX);
	const double RgnOffsetY = (FieldStagePosY - MapTeachCpY);
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	if (true == SignX)
	{
		MapRoiPosX = (int)(nTeachMapW2 + (RgnOffsetX / MapRes.x));
	}
	else
	{
		MapRoiPosX = (int)(nTeachMapW2 - (RgnOffsetX / MapRes.x));
	}
	if (true == SignY)
	{
		MapRoiPosY = (int)(nTeachMapH2 + (RgnOffsetY / MapRes.y));
	}
	else
	{
		MapRoiPosY = (int)(nTeachMapH2 - (RgnOffsetY / MapRes.y));
	}
	MapRoiPosY = nTeachMapH - MapRoiPosY;

	bool  ModifyCellRoi = false;
	RECT  MapRoiRect = { 0 };
	RECT  CellRoiRect = { 0 };
	IMAGE_SIZE RoiExt = 64;
	IMAGE_SIZE MapRoiW = uCellW + RoiExt;
	IMAGE_SIZE MapRoiH = uCellH + RoiExt;
	IMAGE_SIZE MapRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, BitCount, nAlign);
	MapRoiRect.left = MapRoiPosX - (MapRoiW / 2);
	MapRoiRect.top = MapRoiPosY - (MapRoiH / 2);
	MapRoiRect.right = MapRoiPosX + (MapRoiW / 2);
	MapRoiRect.bottom = MapRoiPosY + (MapRoiH / 2);
	CellRoiRect.left = 0;
	CellRoiRect.top = 0;
	CellRoiRect.right = uCellW;
	CellRoiRect.bottom = uCellH;
	ModifyCellRoi = false;
	if (MapRoiRect.left < 0)
	{
		ModifyCellRoi = true;
		CellRoiRect.left = CellRoiRect.left + (0 - MapRoiRect.left);
		MapRoiRect.left = 0;
	}
	if (MapRoiRect.right > nTeachMapW)
	{
		ModifyCellRoi = true;
		CellRoiRect.right = CellRoiRect.right - (MapRoiRect.right - nTeachMapW);
		MapRoiRect.right = nTeachMapW;
	}
	if (MapRoiRect.top < 0)
	{
		ModifyCellRoi = true;
		CellRoiRect.top = CellRoiRect.top + (0 - MapRoiRect.top);
		MapRoiRect.top = 0;
	}
	if (MapRoiRect.bottom > nTeachMapH)
	{
		ModifyCellRoi = true;
		CellRoiRect.bottom = CellRoiRect.bottom - (MapRoiRect.bottom - nTeachMapH);
		MapRoiRect.bottom = nTeachMapH;
	}
	if (CellRoiRect.right <= CellRoiRect.left || CellRoiRect.bottom <= CellRoiRect.top)
	{
		JetMemory.free_func(CellPtr);
#ifdef _DEBUG
		JetAPI::DeleteFileList(SaveFileList);
#endif//_DEBUG
		SaveProjectScratchPartLog(_T("CellRoiRect.right<=CellRoiRect.left || CellRoiRect.bottom<=CellRoiRect.top"));
		return true;
	}
	if (true == ModifyCellRoi)
	{
		if (true == SignX)
		{
			CellRgn.minX += (CellRoiRect.left - 0)*MapRes.x;
			CellRgn.maxX -= (uCellW - CellRoiRect.right)*MapRes.x;
		}
		else
		{
			CellRgn.maxX -= (CellRoiRect.left - 0)*MapRes.x;
			CellRgn.minX += (uCellW - CellRoiRect.right)*MapRes.x;
		}
		if (true == SignY)
		{
			CellRgn.maxY -= (CellRoiRect.top - 0)*MapRes.y;
			CellRgn.minY += (uCellH - CellRoiRect.bottom)*MapRes.y;
		}
		else
		{
			CellRgn.minY += (CellRoiRect.top - 0)*MapRes.y;
			CellRgn.maxY -= (uCellH - CellRoiRect.bottom)*MapRes.y;
		}

		IMAGE_PTR  CellRoiPtr = NULL;
		IMAGE_SIZE CellRoiW = CellRoiRect.right - CellRoiRect.left;
		IMAGE_SIZE CellRoiH = CellRoiRect.bottom - CellRoiRect.top;
		IMAGE_SIZE CellRoiStep = JetAPI::GetBMPImagePixelsPerLine(CellRoiW, BitCount, nAlign);;
		const size_t CellRoiBuffserSize = CellRoiStep*CellRoiH;
		if (JetMemory.alloc_func(CellRoiBuffserSize, CellRoiPtr, fnName, "CellRoiPtr") == false)
		{
			JetMemory.free_func(CellPtr);
			m_ErrorString = JetMemory.GetErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		if (ImageAPI.ExtractRoiImage3(uCellW, uCellH, uCellStep, BitCount, CellPtr, CellRoiRect, CellRoiStep, CellRoiPtr, false) == false)
		{
			JetMemory.free_func(CellPtr);
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		JetMemory.free_func(CellPtr);
		uCellW = CellRoiW;
		uCellH = CellRoiH;
		uCellStep = CellRoiStep;
		CellPtr = CellRoiPtr;
#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_Cell3.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtr, true);
			SaveFileList.push_back(str);
		}
#endif//_DEBUG
		MapRoiW = MapRoiRect.right - MapRoiRect.left;
		MapRoiH = MapRoiRect.bottom - MapRoiRect.top;
		MapRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, BitCount, nAlign);
	}
	//--------------------------------------------------------------------------------------//

	IMAGE_PTR    MapRoiPtr = NULL;
	const size_t MapRoiBufferSize = MapRoiStep*MapRoiH;

	if (JetMemory.alloc_func(MapRoiBufferSize, MapRoiPtr, fnName, "MapRoiPtr") == false)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		m_ErrorString = JetMemory.GetErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	if (ImageAPI.ExtractRoiImage3(nTeachMapW, nTeachMapH, nTeachMapStep, BitCount, TeachMapPtr, MapRoiRect, MapRoiStep, MapRoiPtr, false) == false)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		m_ErrorString = ImageAPI.GetImageApiErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
#ifdef _DEBUG
	if (true == bSave)
	{
		str.Format(_T("%s\\%s_%s_MapRoi.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG

	IMAGE_PTR    MapMaskRoiPtr = NULL;
	const IMAGE_SIZE MapMaskRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, MaskBitCount, nAlign);
	const size_t MapMaskRoiBufferSize = MapMaskRoiStep*MapRoiH;
	if (NULL != TeachMaskPtr && nTeachMaskW == nTeachMapW && nTeachMaskH == nTeachMapH)
	{
		if (JetMemory.alloc_func(MapMaskRoiBufferSize, MapMaskRoiPtr, fnName, "MapMaskRoiPtr") == false)
		{
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(MapRoiPtr);
			JetMemory.free_func(MapMaskRoiPtr);
			m_ErrorString = JetMemory.GetErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		if (ImageAPI.ExtractRoiImage3(nTeachMaskW, nTeachMaskH, nTeachMaskStep, MaskBitCount, TeachMaskPtr, MapRoiRect, MapMaskRoiStep, MapMaskRoiPtr, false) == false)
		{
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(MapRoiPtr);
			JetMemory.free_func(MapMaskRoiPtr);
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_MapMaskRoi.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiPtr, true);
			SaveFileList.push_back(str);
		}
#endif//_DEBUG
		if (false == CreateProjectColorMaskImage(MapRoiW, MapRoiH, MapMaskRoiStep, MapMaskRoiPtr, MapRoiRect, 0xff, fnName, SaveFileList)) {
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(MapRoiPtr);
			JetMemory.free_func(MapMaskRoiPtr);
			//m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_MapMaskRoi2.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiPtr, true);
			SaveFileList.push_back(str);
		}
#endif//_DEBUG
		if (false == CreateProjectSkewMaskImage(FieldPtr,MapRoiW, MapRoiH, MapMaskRoiStep, MapMaskRoiPtr, MapRoiRect, 0xff, fnName, SaveFileList)) {
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(MapRoiPtr);
			JetMemory.free_func(MapMaskRoiPtr);
			//m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectDropOutPartLog(m_ErrorString);
			return false;
		}
#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_MapMaskRoi3.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiPtr, true);
			SaveFileList.push_back(str);
		}
#endif//_DEBUG
	}

	//影像對位
	CJetMatch  Match;
	const bool bRobustness = true;
	const int  nMinReduceArea = 4096;
	const int  nFinalReduction = 0;
	const int  nEnableScaleMatch = GetProjectParameter().m_ScratchPartMatchUseScale;
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();
	if (Match.SetMatchLibType(MatchLibType) == false)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		m_ErrorString = Match.GetErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	//initial eMatch
	Match.SetMatchDefaultParam();
	Match.SetRobustness(bRobustness);
	Match.SetMinReducedArea(nMinReduceArea);
	Match.SetFinalReduction(nFinalReduction);
	if (Match.LearnPattern(uCellW, uCellH, uCellStep, BitCount, CellPtr, true) == false)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		m_ErrorString = Match.GetErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	Match.SetInterpolate(true);
	Match.SetMinScore(-1);
	Match.SetMaxPositions(1);
	Match.SetMaxInitialPositions(4);
	if (Match.Match(MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, true) == false)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		m_ErrorString = Match.GetErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	const int NResults = Match.GetNumPositions();
	if (0 == NResults)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		m_ErrorString = _T("Error, Map Match Fault");
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	const int ResultIdx = 0;
	double ResultX = Match.GetResultPosX(ResultIdx);
	double ResultY = Match.GetResultPosY(ResultIdx);
	double ResultA = Match.GetResultAngle(ResultIdx);
	double ResultS = Match.GetResultScore(ResultIdx)*100.0;
	double ResultSX = Match.GetResultScaleX(ResultIdx);
	double ResultSY = Match.GetResultScaleY(ResultIdx);
	if (FN_ENABLE == nEnableScaleMatch)
	{
		const double ResultX_N = Match.GetResultPosX(ResultIdx);
		const double ResultY_N = Match.GetResultPosY(ResultIdx);
		const double ResultA_N = Match.GetResultAngle(ResultIdx);
		const double ResultS_N = Match.GetResultScore(ResultIdx)*100.0;
		const double ResultSX_N = Match.GetResultScaleX(ResultIdx);
		const double ResultSY_N = Match.GetResultScaleY(ResultIdx);

		//再以Scale計算一次
		const float ScaleRange = 0.1f;
		const float ScaleMin = 1.0f - ScaleRange;
		const float ScaleMax = 1.0f + ScaleRange;

		Match.SetMinScale(ScaleMin);
		Match.SetMinScaleX(ScaleMin);
		Match.SetMinScaleY(ScaleMin);
		Match.SetMaxScale(ScaleMax);
		Match.SetMaxScaleX(ScaleMax);
		Match.SetMaxScaleY(ScaleMax);
		Match.SetUseScale(true);
		if (Match.Match(MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, true) == false)
		{
			JetMemory.free_func(CellPtr);
			JetMemory.free_func(MapRoiPtr);
			JetMemory.free_func(MapMaskRoiPtr);
			m_ErrorString = Match.GetErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		const int NResults_F = Match.GetNumPositions();
		if (NResults_F > 0)
		{
			double ResultX_F = Match.GetResultPosX(ResultIdx);
			double ResultY_F = Match.GetResultPosY(ResultIdx);
			double ResultA_F = Match.GetResultAngle(ResultIdx);
			double ResultS_F = Match.GetResultScore(ResultIdx)*100.0;
			double ResultSX_F = Match.GetResultScaleX(ResultIdx);
			double ResultSY_F = Match.GetResultScaleY(ResultIdx);
			//差太多使用沒有內插的, 以及找最相似度最高的
			if (fabs(ResultX_F - ResultX_N)>10 || fabs(ResultY_F - ResultY_N)>10 || ResultS_F<ResultS_N)
			{
				ResultX = ResultX_N;
				ResultY = ResultY_N;
				ResultA = ResultA_N;
				ResultS = ResultS_N;
				ResultSX = ResultSX_N;
				ResultSY = ResultSY_N;
			}
			else
			{
				ResultX = ResultX_F;
				ResultY = ResultY_F;
				ResultA = ResultA_F;
				ResultS = ResultS_F;
				ResultSX = ResultSX_F;
				ResultSY = ResultSY_F;
			}
		}
	}
	const int    nResultX = (int)(ResultX + 0.5);
	const int    nResultY = (int)(ResultY + 0.5);
	const double ResultX_Start = nResultX - (uCellW / 2);
	const double ResultY_Start = nResultY - (uCellH / 2);
	//const double ResultX_Start = ResultX-(uCellW*0.5);
	//const double ResultY_Start = ResultY-(uCellH*0.5);
	//const int    nCellPosX = (int)(ResultX_Start+0.5);
	//const int    nCellPosY = (int)(ResultY_Start+0.5);	
	const int    nCellPosX = JetAPI::Floor(ResultX_Start);
	const int    nCellPosY = JetAPI::Floor(ResultY_Start);
	const int    nCellPosXEnd = nCellPosX + uCellW;
	const int    nCellPosYEnd = nCellPosY + uCellH;
	if (nCellPosX<0 || nCellPosY<0 || ResultS<50.0 || nCellPosXEnd>MapRoiW || nCellPosYEnd>MapRoiH)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		m_ErrorString = _T("Error, Map Match Result Fault");

		FILE *pfile = NULL;
		str.Format(_T("%s\\%s_%s_MatchFaultReportScale.TXT"), Folder, KeyName, MainName);
		pfile = _tfopen(str, _T("w+"));
		if (NULL != pfile)
		{
			::_ftprintf(pfile, _T("Result X:%.4f\n"), ResultX);
			::_ftprintf(pfile, _T("Result Y:%.4f\n"), ResultY);
			::_ftprintf(pfile, _T("Result Angle:%.4f\n"), ResultA);
			::_ftprintf(pfile, _T("Result Score:%.4f\n"), ResultS);
			::_ftprintf(pfile, _T("Result Scale X:%.4f\n"), ResultSX);
			::_ftprintf(pfile, _T("Result Scale Y:%.4f\n"), ResultSY);

			::_ftprintf(pfile, _T("Result X Start:%.4f\n"), ResultX_Start);
			::_ftprintf(pfile, _T("Result Y Start:%.4f\n"), ResultY_Start);
			::_ftprintf(pfile, _T("Cell Start Pos X:%d\n"), nCellPosX);
			::_ftprintf(pfile, _T("Cell Start Pos Y:%d\n"), nCellPosY);
			::fclose(pfile);	pfile = NULL;
		}
		// 
#ifdef _DEBUG
		//JetAPI::DeleteFileList(SaveFileList);
#endif//_DEBUG		
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	MASK_PTR     RawMaskPtrL = NULL;
	MASK_PTR     TestMaskPtr = NULL;
	MASK_PTR     TestMaskPtr1 = NULL;
	MASK_PTR     TestMaskPtr2 = NULL;
	MASK_PTR     TestMaskPtrR = NULL;
	MASK_PTR     TestMaskPtrG = NULL;
	MASK_PTR     TestMaskPtrB = NULL;
	MASK_PTR     TestMaskPtrV = NULL;
	const IMAGE_SIZE MaskW = uCellW;
	const IMAGE_SIZE MaskH = uCellH;
	const IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(MaskW, MaskBitCount, 4);
	const size_t MaskBufferSize = ImageAPI.CalcBufferSize(MaskStep, MaskH);
	if (JetMemory.alloc_func(MaskBufferSize, RawMaskPtrL, fnName, "RawMaskPtrL") == false ||
		JetMemory.alloc_func(MaskBufferSize, TestMaskPtr, fnName, "TestMaskPtr") == false ||
		JetMemory.alloc_func(MaskBufferSize, TestMaskPtr1, fnName, "TestMaskPtr1") == false ||
		JetMemory.alloc_func(MaskBufferSize, TestMaskPtr2, fnName, "TestMaskPtr2") == false ||
		JetMemory.alloc_func(MaskBufferSize, TestMaskPtrR, fnName, "TestMaskPtrR") == false ||
		JetMemory.alloc_func(MaskBufferSize, TestMaskPtrG, fnName, "TestMaskPtrG") == false ||
		JetMemory.alloc_func(MaskBufferSize, TestMaskPtrB, fnName, "TestMaskPtrB") == false ||
		JetMemory.alloc_func(MaskBufferSize, TestMaskPtrV, fnName, "TestMaskPtrV") == false)
	{
		m_ErrorString = JetMemory.GetErrorString();
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		JetMemory.free_func(TestMaskPtrR);
		JetMemory.free_func(TestMaskPtrG);
		JetMemory.free_func(TestMaskPtrB);
		JetMemory.free_func(TestMaskPtrV);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}

	//剔除此FOV內的零件
	size_t       i = 0, j = 0;
	TREGION4D    FieldRgn = CellRgn;
	const int    MaskMode = PROJECT_PART_MASK_BODY | PROJECT_CODE_MASK_BODY;
	::memset(TestMaskPtr1, 0xff, sizeof(MASK_DATA)*MaskBufferSize);
	//if (CreateProjectPartMaskImage(MaskW, MaskH, MaskStep, TestMaskPtr1, FieldRgn, MapRes, MaskMode, 0x00) == false)
	//{
	//	JetMemory.free_func(CellPtr);
	//	JetMemory.free_func(MapRoiPtr);
	//	JetMemory.free_func(MapMaskRoiPtr);
	//	JetMemory.free_func(RawMaskPtrL);
	//	JetMemory.free_func(TestMaskPtr);
	//	JetMemory.free_func(TestMaskPtr1);
	//	JetMemory.free_func(TestMaskPtr2);
	//	JetMemory.free_func(TestMaskPtrR);
	//	JetMemory.free_func(TestMaskPtrG);
	//	JetMemory.free_func(TestMaskPtrB);
	//	JetMemory.free_func(TestMaskPtrV);
	//	SaveProjectScratchPartLog(m_ErrorString);
	//	return false;
	//}

	if (CreateProjectPartMaskImage_v2(MaskW, MaskH, MaskStep, TestMaskPtr1, FieldRgn, MapRes, MaskMode, 0x00) == false)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		JetMemory.free_func(TestMaskPtrR);
		JetMemory.free_func(TestMaskPtrG);
		JetMemory.free_func(TestMaskPtrB);
		JetMemory.free_func(TestMaskPtrV);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
#ifdef _DEBUG
	if (true == bSave)
	{
		str.Format(_T("%s\\%s_%s_MaskNoPart.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr1, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG
	if (ImageAPI.MorphGrayImage3(MaskW, MaskH, MaskStep, TestMaskPtr1, MORPH_OPEN, MORPH_SHAPE_RECT, 9, 1, TestMaskPtr2) == false)
	{
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		JetMemory.free_func(TestMaskPtrR);
		JetMemory.free_func(TestMaskPtrG);
		JetMemory.free_func(TestMaskPtrB);
		JetMemory.free_func(TestMaskPtrV);
		m_ErrorString = ImageAPI.GetImageApiErrorString();
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
#ifdef _DEBUG
	if (true == bSave)
	{
		str.Format(_T("%s\\%s_%s_MaskNoPartOpen.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG	

	//::memcpy(TestMaskPtr, TestMaskPtr2, sizeof(MASK_DATA)*MaskBufferSize);
	//::memset(TestMaskPtr, 0xFF, sizeof(MASK_DATA)*MaskBufferSize);

	//依據遮罩來比較影像是否有問題	
	size_t MaskIdx = 0;
	size_t MapMaskIdx = 0;
	size_t CellIdx = 0, RoiIdx = 0;
	IMAGE_PTR  CellPtrRaw = NULL;
	IMAGE_PTR  MapRoiPtrRaw = NULL;
	const size_t CellSize = uCellStep*uCellH;
	const size_t MapRoiSize = MapRoiStep*MapRoiH;
	if (JetMemory.alloc_func(CellSize, CellPtrRaw, fnName, "CellPtrRaw") == false ||
		JetMemory.alloc_func(MapRoiSize, MapRoiPtrRaw, fnName, "MapRoiPtrRaw") == false)
	{
		m_ErrorString = JetMemory.GetErrorString();
		JetMemory.free_func(CellPtr);
		JetMemory.free_func(CellPtrRaw);
		JetMemory.free_func(MapRoiPtr);
		JetMemory.free_func(MapRoiPtrRaw);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		JetMemory.free_func(TestMaskPtrR);
		JetMemory.free_func(TestMaskPtrG);
		JetMemory.free_func(TestMaskPtrB);
		JetMemory.free_func(TestMaskPtrV);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	::memset(RawMaskPtrL, 0x00, sizeof(MASK_DATA)*MaskBufferSize);
	const int SmoothSize = GetProjectParameter().m_ScratchPartSmoothSize;
	//const int SmoothSize = 0;
	if (SmoothSize > 0)
	{
		IMAGE_PTR SwapPtr = NULL;
		const int nSmoothSize = ImageAPI.GetKernelSize(SmoothSize);
		if (ImageAPI.SmoothImage3(uCellW, uCellH, uCellStep, BitCount, CellPtr, nSmoothSize, CellPtrRaw) == true)
		{
			SwapPtr = CellPtrRaw;
			CellPtrRaw = CellPtr;
			CellPtr = SwapPtr;
		}
		else
		{
			::memcpy(CellPtrRaw, CellPtr, sizeof(IMAGE_DATA)*CellSize);
		}

		if (ImageAPI.SmoothImage3(MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, nSmoothSize, MapRoiPtrRaw) == true)
		{
			SwapPtr = MapRoiPtrRaw;
			MapRoiPtrRaw = MapRoiPtr;
			MapRoiPtr = SwapPtr;
		}
		else
		{
			::memcpy(MapRoiPtrRaw, MapRoiPtr, sizeof(IMAGE_DATA)*MapRoiSize);
		}
#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_CellSmooth.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtr, true);
			SaveFileList.push_back(str);

			str.Format(_T("%s\\%s_%s_MapRoiSmooth.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr, true);
			SaveFileList.push_back(str);
		}
#endif//_DEBUG
	}
	else
	{
		::memcpy(CellPtrRaw, CellPtr, sizeof(IMAGE_DATA)*CellSize);
		::memcpy(MapRoiPtrRaw, MapRoiPtr, sizeof(IMAGE_DATA)*MapRoiSize);
	}

	unsigned char IR = 0, IG = 0, IB = 0, IV = 0;
	size_t s = 0, t = 0, usedCnt = 0;
	int MaxR = 0, MinR = 0;

	const size_t CalcSizeMin = 4;
	const int MinGrayscale = GetProjectParameter().m_ScratchPartMinGrayscale;
	const int MaxGrayscale = GetProjectParameter().m_ScratchPartMaxGrayscale;
	const int MinEdge = GetProjectParameter().m_ScratchPartEdgeThreshold;
	const int     ColorExpand = GetProjectParameter().m_ScratchPartColorExpand;
	const double  CalcSizeWum = GetProjectParameter().m_ScratchPartCalcSizeW;
	const double  CalcSizeHum = GetProjectParameter().m_ScratchPartCalcSizeH;
	size_t WndSizeW = (size_t)((CalcSizeWum / MapRes.x) + 0.5);
	size_t WndSizeH = (size_t)((CalcSizeHum / MapRes.y) + 0.5);
	if (WndSizeW < CalcSizeMin) { WndSizeW = CalcSizeMin; }
	if (WndSizeH < CalcSizeMin) { WndSizeH = CalcSizeMin; }
	const size_t WndSizeW2 = WndSizeW / 2;
	const size_t WndSizeH2 = WndSizeH / 2;
	POINT Result_Start = { ResultX_Start ,ResultY_Start };
	double R = 0, G = 0, B = 0, GrayScale;
	float wR = 0.299f, wG = 0.587f, wB = 0.114f;

	::memset(RawMaskPtrL, 0x00, sizeof(MASK_DATA)*MaskBufferSize);
	if (24 == BitCount)
	{
		ImageAPI.ExecFindAbnormal(
			uCellW, uCellH, uCellStep, BitCount, CellPtr,
			MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtr,
			Result_Start, ColorExpand, WndSizeW, WndSizeH, MinEdge, RawMaskPtrL
		);
		for (i = 0; i<MaskH; i++)
		{
			for (j = 0; j<MaskW; j++)
			{
				MaskIdx = i*MaskStep + j;
				if (0 == RawMaskPtrL[MaskIdx]) {
					continue;
				}
				if (0 == TestMaskPtr2[MaskIdx])
				{
					RawMaskPtrL[MaskIdx] = 0;

				}
				if (NULL != MapMaskRoiPtr)
				{
					MapMaskIdx = ((i + nCellPosY)*MapMaskRoiStep) + (j + nCellPosX);
					if (0 == MapMaskRoiPtr[MapMaskIdx])
					{
						RawMaskPtrL[MaskIdx] = 0;
						continue;
					}
				}
				CellIdx = (i*uCellStep) + (j * 3);
				B = CellPtr[CellIdx];
				G = CellPtr[CellIdx + 1];
				R = CellPtr[CellIdx + 2];
				GrayScale = B*wB + G*wG + R*wR;
				if (GrayScale<MinGrayscale || GrayScale>MaxGrayscale) { RawMaskPtrL[MaskIdx] = 0; }
			}
		}
	}
	else
	{	//8-Bit Image
		for (i = WndSizeH2; i<MaskH - WndSizeH2; i++)
		{
			for (j = WndSizeW2; j<MaskW - WndSizeW2; j++)
			{
				MaskIdx = i*MaskStep + j;
				if (0 == TestMaskPtr2[MaskIdx])
				{
					continue;
				}
				if (NULL != MapMaskRoiPtr)
				{
					MapMaskIdx = ((i + nCellPosY)*MapMaskRoiStep) + (j + nCellPosX);
					if (0 == MapMaskRoiPtr[MapMaskIdx])
					{
						continue;
					}
				}

				//Get Gray Range (at Map)
				usedCnt = 0;
				MaxR = 0;
				MinR = 255;
				for (t = i - WndSizeH2; t<i + WndSizeH2; t++)
				{
					for (s = j - WndSizeW2; s<j + WndSizeW2; s++)
					{
						MaskIdx = t*MaskStep + s;
						if (0 == TestMaskPtr2[MaskIdx])
						{
							continue;
						}
						if (NULL != MapMaskRoiPtr)
						{
							MapMaskIdx = ((t + nCellPosY)*MapMaskRoiStep) + (s + nCellPosX);
							if (0 == MapMaskRoiPtr[MapMaskIdx])
							{
								continue;
							}
						}

						//CellIdx = (t*uCellStep)+(s);
						RoiIdx = ((t + nCellPosY)*MapRoiStep) + (s + nCellPosX);

						//IR = CellPtr[CellIdx];
						IR = MapRoiPtr[RoiIdx];
						if (IR < MinR) { MinR = IR; }
						if (IR > MaxR) { MinR = IR; }
						usedCnt++;
					}
				}
				if (usedCnt < CalcSizeMin) { continue; }

				MaskIdx = i*MaskStep + j;
				MinR = MAX(MinR - ColorExpand, 0);
				MaxR = MIN(MaxR + ColorExpand, 255);

				CellIdx = (i*uCellStep) + (j);
				//RoiIdx = ((i+nCellPosY)*MapRoiStep)+(j+nCellPosX);
				IR = CellPtr[CellIdx];
				if (IR<MinR || IR>MaxR) { RawMaskPtrL[MaskIdx] = 255; }
			}
		}
	}

#ifdef _DEBUG
	if (true == bSave)
	{
		str.Format(_T("%s\\%s_%s_MaskCompareRaw.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, RawMaskPtrL, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG
	bool KeepImage = false;
#ifndef _DEBUG
	KeepImage = SaveImage;
#endif//_DEBUG

	::memcpy(TestMaskPtr, RawMaskPtrL, sizeof(MASK_DATA)*MaskBufferSize);
	::memcpy(TestMaskPtr2, RawMaskPtrL, sizeof(MASK_DATA)*MaskBufferSize);

	int MorphMode = 0;
	int   ShapeMode = MORPH_SHAPE_ELLIPSE;//MORPH_SHAPE_RECT, MORPH_SHAPE_CROSS, MORPH_SHAPE_ELLIPSE
	const int NoiseFilterOpen = GetProjectParameter().m_ScratchPartFilterOpenSize;
	if (NoiseFilterOpen > 0)
	{
		MorphMode = MORPH_OPEN;
		const int OpenSize = ImageAPI.GetKernelSize(NoiseFilterOpen);
		if (ImageAPI.MorphGrayImage3(MaskW, MaskH, MaskStep, TestMaskPtr, MorphMode, ShapeMode, OpenSize, 1, TestMaskPtr2) == false)
		{
			JetMemory.free_func(CellPtrRaw);
			JetMemory.free_func(MapRoiPtrRaw);
			JetMemory.free_func(MapMaskRoiPtr);
			JetMemory.free_func(RawMaskPtrL);
			JetMemory.free_func(TestMaskPtr);
			JetMemory.free_func(TestMaskPtr1);
			JetMemory.free_func(TestMaskPtr2);
			JetMemory.free_func(TestMaskPtrR);
			JetMemory.free_func(TestMaskPtrG);
			JetMemory.free_func(TestMaskPtrB);
			JetMemory.free_func(TestMaskPtrV);
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		::memcpy(TestMaskPtr, TestMaskPtr2, sizeof(MASK_DATA)*MaskBufferSize);
#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_MaskCompareOpen.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
			SaveFileList.push_back(str);
		}
#endif//_DEBUG
	}
	const int NoiseFilterClose = GetProjectParameter().m_ScratchPartFilterCloseSize;
	if (NoiseFilterClose > 0)
	{
		MorphMode = MORPH_CLOSE;
		const int CloseSize = ImageAPI.GetKernelSize(NoiseFilterClose);
		if (ImageAPI.MorphGrayImage3(MaskW, MaskH, MaskStep, TestMaskPtr, MorphMode, ShapeMode, CloseSize, 1, TestMaskPtr2) == false)
		{
			JetMemory.free_func(CellPtrRaw);
			JetMemory.free_func(MapRoiPtrRaw);
			JetMemory.free_func(MapMaskRoiPtr);
			JetMemory.free_func(RawMaskPtrL);
			JetMemory.free_func(TestMaskPtr);
			JetMemory.free_func(TestMaskPtr1);
			JetMemory.free_func(TestMaskPtr2);
			JetMemory.free_func(TestMaskPtrR);
			JetMemory.free_func(TestMaskPtrG);
			JetMemory.free_func(TestMaskPtrB);
			JetMemory.free_func(TestMaskPtrV);
			m_ErrorString = ImageAPI.GetImageApiErrorString();
			SaveProjectScratchPartLog(m_ErrorString);
			return false;
		}
		::memcpy(TestMaskPtr, TestMaskPtr2, sizeof(MASK_DATA)*MaskBufferSize);
#ifdef _DEBUG
		if (true == bSave)
		{
			str.Format(_T("%s\\%s_%s_MaskCompareClose.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
			SaveFileList.push_back(str);
		}
#endif//_DEBUG
	}

	JetMemory.free_func(CellPtr);
	JetMemory.free_func(MapRoiPtr);
	if (false == KeepImage)
	{
		JetMemory.free_func(CellPtrRaw);
		JetMemory.free_func(MapRoiPtrRaw);
		JetMemory.free_func(MapMaskRoiPtr);
	}

	const bool bMeasure = false;
	const double MinThickness = 0;
	std::vector<CAOIComponent*> NewComponentList;
	std::vector<RECT> NewRectList;

	const double PartWHRatio = -1;
	const double PartW = GetProjectParameter().m_ScratchPartMinSizeW;
	const double PartH = GetProjectParameter().m_ScratchPartMinSizeH;
	const double PartD = GetProjectParameter().m_ScratchPartMinSizeD;
	const double PartArea = GetProjectParameter().m_ScratchPartMinPixels;
	if (CreateProjectComponentScatchByBlob(MaskW, MaskH, MaskStep, TestMaskPtr2, NULL, NULL, FieldRgn, MapRes, PartW, PartH, PartD, PartArea, PartWHRatio, MinThickness, bMeasure, NewComponentList, NewRectList) == false)
	{
		JetMemory.free_func(CellPtrRaw);
		JetMemory.free_func(MapRoiPtrRaw);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtrL);
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		JetMemory.free_func(TestMaskPtrR);
		JetMemory.free_func(TestMaskPtrG);
		JetMemory.free_func(TestMaskPtrB);
		JetMemory.free_func(TestMaskPtrV);
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	JetMemory.free_func(RawMaskPtrL);
	JetMemory.free_func(TestMaskPtr);
	JetMemory.free_func(TestMaskPtr1);
	JetMemory.free_func(TestMaskPtr2);
	JetMemory.free_func(TestMaskPtrR);
	JetMemory.free_func(TestMaskPtrG);
	JetMemory.free_func(TestMaskPtrB);
	JetMemory.free_func(TestMaskPtrV);

	const size_t MaxComponentInFov = 36;
	const size_t NewComponentCount = NewComponentList.size();
	if (0 == NewComponentCount)//|| NewComponentCount>MaxComponentInFov )
	{
		JetMemory.free_func(CellPtrRaw);
		JetMemory.free_func(MapRoiPtrRaw);
		JetMemory.free_func(MapMaskRoiPtr);
#ifdef _DEBUG
		JetAPI::DeleteFileList(SaveFileList);
#endif//_DEBUG
		SaveProjectScratchPartLog(_T("0 == NewComponentCount"));
		return true;
	}

	int  nSaveTxt = SaveImage;
#ifdef _DEBUG
	nSaveTxt = FN_ENABLE;
#endif//_DEBUG
	if (FN_ENABLE == nSaveTxt)
	{
		FILE *pfile = NULL;
		str.Format(_T("%s\\%s_%s_MatchResult.TXT"), Folder, KeyName, MainName);
		pfile = _tfopen(str, _T("w+"));
		if (NULL != pfile)
		{
			::_ftprintf(pfile, _T("Result X:%.4f\n"), ResultX);
			::_ftprintf(pfile, _T("Result Y:%.4f\n"), ResultY);
			::_ftprintf(pfile, _T("Result Angle:%.4f\n"), ResultA);
			::_ftprintf(pfile, _T("Result Score:%.4f\n"), ResultS);
			::_ftprintf(pfile, _T("Result Scale X:%.4f\n"), ResultSX);
			::_ftprintf(pfile, _T("Result Scale Y:%.4f\n"), ResultSY);

			::_ftprintf(pfile, _T("Result X Start:%.4f\n"), ResultX_Start);
			::_ftprintf(pfile, _T("Result Y Start:%.4f\n"), ResultY_Start);
			::_ftprintf(pfile, _T("Cell Start Pos X:%d\n"), nCellPosX);
			::_ftprintf(pfile, _T("Cell Start Pos Y:%d\n"), nCellPosY);
			::fclose(pfile);	pfile = NULL;
		}
	}

#ifndef _DEBUG
	if (FN_ENABLE == SaveImage)
	{
		str.Format(_T("%s\\%s_%s_MapRoi.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapRoiStep, BitCount, MapRoiPtrRaw, true);
		str.Format(_T("%s\\%s_%s_MapMaskRoi.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiPtr, true);
		str.Format(_T("%s\\%s_%s_Cell3.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellPtrRaw, true);
	}
#endif//_DEBUG
	JetMemory.free_func(CellPtrRaw);
	JetMemory.free_func(MapRoiPtrRaw);
	JetMemory.free_func(MapMaskRoiPtr);

	if (CreateProjectScratchPartByComponentList(FieldPtr, NewComponentList) == false)
	{
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	if (SaveProjectSpecTestField(FieldPtr) == false)
	{
		SaveProjectScratchPartLog(m_ErrorString);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectScratchPartByComponentList(CAOIField *FieldPtr, std::vector<CAOIComponent*> &ComponentList)//依照零件列表建立刮傷列表
{
	const char fnName[] = "CAOIProject::CreateProjectScratchPartByComponentList";
	if ( NULL == FieldPtr ) { return false; }

	CString        str;
	size_t         i=0, j=0;
	unsigned int   UniqueID=0;
	wchar_t        ComponentName[MAX_JET_PATH]=L"";
	const wchar_t  PartNumber[32]=L"SCRATCH";
	const wchar_t  ModelName[32]=L"SCRATCH";
	const wchar_t  NozzleName[32]=L"SCRATCH";
	CString        SpcImageFolder;
	CString        ComponentFullName;
	double         BodySizeW=0;
	double         BodySizeH=0;		
	TPOINT2D       FrameRes;
	TPOINT2D       FieldStageCp;	
	TREGION4D      ModelStageRegion;
	RECT           ComponentImageRect={0,0,0,0};
	double         ComponentCadPosX=0;
	double         ComponentCadPosY=0;
	double         ComponentStagePosX=0;
	double         ComponentStagePosY=0;	
	TREGION4D      ComponentStageRegion;
	TREGION4D      ComponentImageRegion;
	CAOIWnd       *WndPtr = NULL;
	CAOIModel     *ModelPtr = NULL;	
	CAOIComponent *NewComponentPtr=NULL;
	const bool     bAppend=false;
	const bool     bEnhance=true;
	const bool     bSave3D=true;
	const bool     bSaveDefectImage = AOIDataCollect.GetSaveDefectImage();
	TUNI_FRAME     FrameUniFrame;
	TUNI_FRAME     ComponentUniFrame;
	std::vector<TUNI_FRAME> ComponentUniFrameList;
	const size_t   NewComponentCount = ComponentList.size();
	const double   PanelBasePlane = FieldPtr->GetFieldPanelBasePlane();
	const size_t   OldScratchCount = GetProjectScratchPartCount_Inline();
	const unsigned int ScratchFrameIndex = GetProjectParameter().m_ScratchPartFrameIndex;
	const unsigned int ScratchFrameUniqueID = GetProjectParameter().m_ScratchPartFrameUniqueID;
	TNoiseFilterParam  NoiseFilterParam;

	FieldStageCp.x = FieldPtr->GetFieldStagePosX();
	FieldStageCp.y = FieldPtr->GetFieldStagePosY();
	SpcImageFolder = GetProjectSpcImageFolder();	

	if ( GetProjectUseLocalFolder() )
	{	SpcImageFolder = GetProjectSpcImageFolderLocal();	}

	LockProject();	
	for ( i=0; i<NewComponentCount; i++ )
	{
		NewComponentPtr = ComponentList[i];
		if ( NULL == NewComponentPtr ) { continue; }

		WndPtr = AOIObjManager.CreateWndObj();		
		if ( NULL==WndPtr ) 
		{	continue;	}

		UniqueID = OldScratchCount+i;
		::swprintf(ComponentName, L"%s_%d", ModelName, UniqueID+1);
		NewComponentPtr->SetComponentFieldPtr(FieldPtr);
		NewComponentPtr->SetComponentName(ComponentName);	
		NewComponentPtr->SetComponentModelName(ModelName);
		NewComponentPtr->SetComponentPartNumber(PartNumber);
		NewComponentPtr->SetComponentNozzleName(NozzleName);
		NewComponentPtr->SetComponentType(COMPONENT_TYPE_SCRATCH);
		NewComponentPtr->SetComponentResultID_AOI(RESULT_ID_NG);
		NewComponentPtr->SetComponentPanelBasePlane(PanelBasePlane);

		BodySizeW = NewComponentPtr->GetComponentBodySizeW();
		BodySizeH = NewComponentPtr->GetComponentBodySizeH();
		ComponentCadPosX = NewComponentPtr->GetComponentCadPosX();
		ComponentCadPosY = NewComponentPtr->GetComponentCadPosY();
		ComponentStagePosX = NewComponentPtr->GetComponentStagePosX();
		ComponentStagePosY = NewComponentPtr->GetComponentStagePosY();
		NewComponentPtr->SetComponentResultWidth(BodySizeW);
		NewComponentPtr->SetComponentResultLength(BodySizeH);
		NewComponentPtr->GetComponentRoiStageRegion(ComponentStageRegion);	
		NoiseFilterParam = NewComponentPtr->GetComponentSpaceNoiseFilterParam();
		NoiseFilterParam.BasePlaneParam = NewComponentPtr->GetComponentSpaceBasePlaneParam();	


		WndPtr->SetWndResultID(RESULT_ID_NG);
		WndPtr->SetWndDefectID(WND_DEFECT_PAD_SCRATCH);
		WndPtr->GetWndBox().SetBoxSize(BodySizeW, BodySizeH);		
		WndPtr->GetWndAlgParam().SetAlgResultID(RESULT_ID_NG);
		WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameIndex(ScratchFrameIndex);
		WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameUniqueID(ScratchFrameUniqueID);		

		ModelPtr = NewComponentPtr->GetComponentModelPtr();
		ModelPtr->SetModelName(ModelName);
		ModelPtr->SetModelType(MODEL_TYPE_OTHERS);		
		ModelPtr->GetModelBodyBox().SetBoxSize(BodySizeW, BodySizeH);
		ModelPtr->AddModelWndPtr(WndPtr, false);
		ModelPtr->SetModelResultID(RESULT_ID_NG);		
		ModelPtr->SetModelAttachedPosCad(ComponentCadPosX, ComponentCadPosY);
		ModelPtr->SetModelAttachedPosStage(ComponentStagePosX, ComponentStagePosY);
		ModelPtr->CalcModelTotalRegionAll();
		ModelPtr->GetModelTotalRegionStage(ModelStageRegion);		
		AddProjectScratchPartPtr(NewComponentPtr, false);//加入拋件列表內-暫存
		
		//存出圖片
		if ( true == bSaveDefectImage )
		{
			CAOIFrame   *FieldFramePtr = NULL;
			const size_t FiledFrameCount = FieldPtr->GetFieldFramePtrCount();

			ComponentStageRegion = ModelStageRegion;
			ComponentFullName = NewComponentPtr->GetComponentFullName();			
			str.Format(_T("%s\\%s.%s"), SpcImageFolder, ComponentFullName, _T("JPG"));			
			for ( j=0; j<FiledFrameCount; j++ )
			{
				FieldFramePtr = FieldPtr->GetFieldFramePtr(j, false);
				if ( NULL == FieldFramePtr ) { continue; }				
				FrameRes.x = FieldFramePtr->GetFrameResolutionX();
				FrameRes.y = FieldFramePtr->GetFrameResolutionY();
				FieldFramePtr->GetFrameUniFrame(FrameUniFrame);
				if ( 0 == FrameUniFrame.BitCount ) 
				{	continue; }				
				AOIDataCollect.MapStageRegionToCamera(FrameUniFrame.ImageW, FrameUniFrame.ImageH, FrameRes, ComponentStageRegion, FieldStageCp, ComponentImageRegion);
				JetAPI::Region4DToRect(ComponentImageRegion, ComponentImageRect, true);
				JetAPI::BoundaryRect(FrameUniFrame.ImageW, FrameUniFrame.ImageH, ComponentImageRect);//調整尺寸, 確保小於相機影像內

				ComponentUniFrame = TUNI_FRAME();
				ComponentUniFrame.ImageW = (ComponentImageRect.right-ComponentImageRect.left);
				ComponentUniFrame.ImageH = (ComponentImageRect.bottom-ComponentImageRect.top);
				ComponentUniFrame.BitCount = FrameUniFrame.BitCount;
				ComponentUniFrame.ImageStep = JetAPI::GetBMPImagePixelsPerLine(ComponentUniFrame.ImageW, FrameUniFrame.BitCount, 4);
				if ( NULL==FrameUniFrame.SpacePtr || NULL==FrameUniFrame.MaskPtr )
				{
					if ( ImageAPI.ExtractRoiImage(FrameUniFrame.ImageW, FrameUniFrame.ImageH, FrameUniFrame.ImageStep, FrameUniFrame.BitCount, FrameUniFrame.ImagePtr, ComponentImageRect, ComponentUniFrame.ImageStep, ComponentUniFrame.ImagePtr, false) == false )
					{
						JetAPI::ClearUniFrame(ComponentUniFrame);
						continue; 
					}
				}
				else
				{
					if ( ImageAPI.ExtractRoiImage(FrameUniFrame.ImageW, FrameUniFrame.ImageH, FrameUniFrame.ImageStep, FrameUniFrame.BitCount, FrameUniFrame.MaskPtr, ComponentImageRect, ComponentUniFrame.ImageStep, ComponentUniFrame.MaskPtr, false) == false )
					{	
						JetAPI::ClearUniFrame(ComponentUniFrame);
						continue; 
					}

					if ( ImageAPI.ExtractSpaceRoiImage(FrameUniFrame.ImageW, FrameUniFrame.ImageH, FrameUniFrame.ImageStep, FrameUniFrame.BitCount, FrameUniFrame.SpacePtr, ComponentImageRect, ComponentUniFrame.ImageStep, ComponentUniFrame.SpacePtr, false) == false )
					{	
						JetAPI::ClearUniFrame(ComponentUniFrame);
						continue; 
					}

					MASK_PTR     Mask2DPtr=NULL;
					MASK_PTR     DstMaskPtr=NULL;
					SPACE_PTR    DstSpacePtr=NULL;										
					IMAGE_SIZE   SrcW=ComponentUniFrame.ImageW;
					IMAGE_SIZE   SrcH=ComponentUniFrame.ImageH;
					IMAGE_SIZE   SrcStep=ComponentUniFrame.ImageStep;
					MASK_PTR     SrcMaskPtr=ComponentUniFrame.MaskPtr;
					SPACE_PTR    SrcSpacePtr=ComponentUniFrame.SpacePtr;
					const size_t SrcSize=ImageAPI.CalcBufferSize(SrcStep, SrcH);					
					if ( JetMemory.alloc_func(SrcSize, DstMaskPtr, fnName, "DstMaskPtr") == false || 
						 JetMemory.alloc_func(SrcSize, DstSpacePtr, fnName, "DstSpacePtr") == false )
					{
						JetMemory.free_func(DstMaskPtr);
						JetMemory.free_func(DstSpacePtr);
					}
					else
					{
						const int    OpenMPCnt = AOIDataCollect.GetOpenMPCount_Inspection();
						if ( ImageAPI.BuildSpaceData3(SrcW, SrcH, SrcStep, SrcSpacePtr, SrcMaskPtr, Mask2DPtr, OpenMPCnt, NoiseFilterParam, DstSpacePtr, DstMaskPtr) == false)
						{
							JetMemory.free_func(DstMaskPtr);
							JetMemory.free_func(DstSpacePtr);
						}
						else
						{
							JetMemory.free_func(SrcMaskPtr);
							JetMemory.free_func(SrcSpacePtr);
							ComponentUniFrame.MaskPtr = DstMaskPtr;
							ComponentUniFrame.SpacePtr = DstSpacePtr;
						}
					}
				}
				ComponentUniFrameList.push_back(ComponentUniFrame);				
			}
			double   SpaceRatio = GetProjectSpaceToGrayRatio();
			ImageAPI.SaveUniFrameImage(str, ComponentUniFrameList, true, bEnhance, bSave3D, bAppend, SpaceRatio);
			JetAPI::ClearUniFrameList(ComponentUniFrameList);
		}	
	}
	UnlockProject();	
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectPartDimensionCount() const//取得專案尺寸數量
{
	return GetProjectPartDimensionCount_Inline();
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectPartDimensionPtr(size_t index, bool check) const//取得專案尺寸指標
{
	if ( check )
	{
		const size_t count = GetProjectPartDimensionCount_Inline();
		if ( index >= count )
		{	return NULL; }
	}
	return CAOIProject::GetProjectPartDimensionPtr_Inline(index);	
}
//---------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::AddProjectPartDimensionPtr(CAOIComponent *ComponentPtr, bool clone)//增加專案尺寸
{
	if ( NULL == ComponentPtr )
	{
		this->m_ErrorString.Format(_T("Error, AddProjectPartDimensionPtr Fault"));
		return NULL;
	}
	
	CAOIComponent *NewComponentPtr = ComponentPtr;
	const unsigned int ComponentIndex = (unsigned int)(GetProjectPartDimensionCount_Inline());
	if ( true == clone )
	{	
		NewComponentPtr = ComponentPtr->CloneComponentObj();
		if ( NewComponentPtr == NULL ) { return NULL; }		
		NewComponentPtr->SetComponentIndex_Project(ComponentIndex);			
	}	
	else
	{	NewComponentPtr->SetComponentIndex_Project(ComponentIndex);	 }

	NewComponentPtr->SetComponentUniqueID(ComponentIndex);	
	NewComponentPtr->SetComponentProjectPtr(this);	
	AddProjectPartDimensionPtr_Inline(NewComponentPtr);		
	return NewComponentPtr;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ClearProjectAllPartDimension()//刪除專案尺寸
{
	size_t i = 0;	
	CAOIComponent *ComponentPtr = NULL;	
	const size_t ComponentCount = GetProjectPartDimensionCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectPartDimensionPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		AOIObjManager.DestroyComponentObj(m_ProjectPartDimensionList[i]);
		ComponentPtr = NULL;		
	}		
	this->m_ProjectPartDimensionList.clear();	
	return true;
}
//---------------------------------------------------------------------------------//
int CAOIProject::GetProjectPartDimensionEnabled() const//檢測尺寸-啟用
{
	return FN_DISABLE;	
}
//-------------------------------------------------------------------------------------//
int CAOIProject::GetProjectPartDimensionSaveImage() const//檢測尺寸-存圖
{
	//return FN_ENABLE;
	return FN_DISABLE;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::ApplyProjectDimensionComponentList()//套用程專案尺寸列表
{
	const int TestDimonsion = GetProjectPartDimensionEnabled();
	if ( FN_DISABLE == TestDimonsion ) { return true; }

	size_t          i=0, j=0, k=0;
	double          dTemp=0;
	double          BodySizeW=0;
	double          BodySizeH=0;	
	double          BodyHeight=0;
	double          ComponentAngle=0.0;
	double          ComponentStagePosX=0;
	double          ComponentStagePosY=0;	
	TREGION4D       ComponentRgn1;
	TREGION4D       ComponentRgn2;	
	CAOIModel      *ModelPtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;
	CAOIComponent  *ComponentPtr2 = NULL;		
	DISTRICT_ID     DistrictID = GetProjectActDistrictID();
	CAOIPanel *PanelPtr = GetProjectPanelPtr(0, true);
	if ( NULL == PanelPtr ) { return true; }
	CAOIBoard *BoardPtr = PanelPtr->GetPanelBoardPtr(0, true);
	if ( NULL == BoardPtr ) { return true; }	
	
	std::vector<CString> PartNameList;
	const bool      bUseRgnChk = false;
	const int       FnEnable  = FN_ENABLE;
	const int       FnDisable = FN_DISABLE;	
	CMapCoordinate *STCMapPtr = BoardPtr->GetBoardMapSTCPtr(DistrictID);
	CMapCoordinate *CTSMapPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);	
	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();
	const size_t    DimonsionCount = GetProjectPartDimensionCount_Inline();		

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentTempInt(FnDisable);
	}
	
	for ( i=0; i<DimonsionCount; i++ )
	{
		ComponentPtr = GetProjectPartDimensionPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		BodyHeight = ComponentPtr->GetComponentResultHeight();
		ComponentPtr->GetComponentBodyStageRegion(ComponentRgn1);
		for ( j=0; j<ComponentCount; j++ )
		{
			ComponentPtr2 = GetProjectComponentPtr_Inline(j);
			if ( NULL == ComponentPtr2 ) { continue; }
			if ( ComponentPtr2->GetComponentDeleted() == true ) { continue; }
			if ( FnEnable == ComponentPtr2->GetComponentTempInt() ) { continue; }
			if ( true == bUseRgnChk )
			{
				ComponentPtr2->GetComponentBodyStageRegion(ComponentRgn2);			
				if ( ComponentRgn2.minX > ComponentRgn1.maxX ) { continue; }
				if ( ComponentRgn2.minY > ComponentRgn1.maxY ) { continue; }
				if ( ComponentRgn2.maxX < ComponentRgn1.minX ) { continue; }
				if ( ComponentRgn2.maxY < ComponentRgn1.minY ) { continue; }
			}			
			ComponentStagePosX = ComponentPtr2->GetComponentStagePosX();
			ComponentStagePosY = ComponentPtr2->GetComponentStagePosY();			
			if ( JetAPI::CheckPtInRegion(ComponentStagePosX, ComponentStagePosY, ComponentRgn1) == false ) 
			{	continue; }
			ModelPtr = ComponentPtr2->GetComponentModelPtr();
			BodySizeW = ComponentRgn1.GetWidth();
			BodySizeH = ComponentRgn1.GetHeight();
			ModelPtr->GetModelBodyBox().SetBoxSizeX(BodySizeW);
			ModelPtr->GetModelBodyBox().SetBoxSizeY(BodySizeH);
			ModelPtr->GetModelBodyBox().SetBoxSizeResX(BodySizeW);
			ModelPtr->GetModelBodyBox().SetBoxSizeResY(BodySizeH);
			ModelPtr->CalcModelTotalRegionAll();
			
			ComponentAngle = ComponentPtr2->GetComponentAngle();
			int AngleLabel = JetAPI::GetAngleLabel(ComponentAngle);
			switch ( AngleLabel )
			{
			case 90:
			case 270:
				dTemp = BodySizeW;
				BodySizeW = BodySizeH;
				BodySizeH = dTemp;
				break;
			}
			ComponentPtr2->SetComponentTempInt(FnEnable);
			ComponentPtr2->SetComponentBodySizeW(BodySizeW);
			ComponentPtr2->SetComponentBodySizeH(BodySizeH);			
			ComponentPtr2->SetComponentRoiSizeW(BodySizeW);
			ComponentPtr2->SetComponentRoiSizeH(BodySizeH);
			ComponentPtr2->SetComponentResultWidth(BodySizeW);
			ComponentPtr2->SetComponentResultLength(BodySizeH);
			ComponentPtr2->SetComponentResultHeight(BodyHeight);
			ComponentPtr2->CalcComponentCadCornerPos();
			ComponentPtr2->LayoutComponentStageCornerPos();
			
			PartNameList.push_back(CString(ComponentPtr2->GetComponentFullName()));
			break;
		}			
	}		

	//調整尺寸
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentTempInt_01(FnDisable);
	}
	
	CString PartNumber;
	double  BodySizeX=0;
	double  BodySizeY=0;
	double  BodyRatioX=0;
	double  BodyRatioY=0;
	int     BodySizeCount=0;
	size_t  ComponentPtrCount=0;
	std::vector<CAOIComponent*> ComponentPtrList;
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( FnEnable == ComponentPtr->GetComponentTempInt_01() ) { continue; }
		ComponentPtrList.clear();
		PartNumber = ComponentPtr->GetComponentPartNumber();		
		ComponentPtrList.push_back(ComponentPtr);
		for ( j=i+1; j<ComponentCount; j++ )
		{
			ComponentPtr2 = GetProjectComponentPtr_Inline(j);
			if ( NULL == ComponentPtr2 ) { continue; }
			if ( ComponentPtr2->GetComponentDeleted() == true ) { continue; }
			if ( FnEnable == ComponentPtr2->GetComponentTempInt_01() ) { continue; }
			if ( PartNumber.CompareNoCase(ComponentPtr2->GetComponentPartNumber()) != 0 ) { continue; }
			ComponentPtrList.push_back(ComponentPtr2);
		}

		BodySizeCount = 0;
		BodyHeight = BodySizeW = BodySizeH = 0;
		ComponentPtrCount = ComponentPtrList.size();
		for ( j=0; j<ComponentPtrCount; j++ )
		{
			ComponentPtr2 = ComponentPtrList[j];
			if ( FnDisable == ComponentPtr2->GetComponentTempInt() ) { continue; }			
			BodySizeCount ++;
			BodySizeW += ComponentPtr2->GetComponentBodySizeW();
			BodySizeH += ComponentPtr2->GetComponentBodySizeH();
			BodyHeight+= ComponentPtr2->GetComponentResultHeight();
		}
		if ( 0 == BodySizeCount )
		{	continue; }
		BodySizeW /= BodySizeCount;
		BodySizeH /= BodySizeCount;
		BodyHeight /= BodySizeCount;
		const double BodySizeW_Ave = BodySizeW;
		const double BodySizeH_Ave = BodySizeH;
		const double BodyHeight_Ave = BodyHeight;

		BodySizeCount = 0;
		BodyHeight = BodySizeW = BodySizeH = 0;
		for ( j=0; j<ComponentPtrCount; j++ )
		{
			ComponentPtr2 = ComponentPtrList[j];
			if ( FnDisable == ComponentPtr2->GetComponentTempInt() ) { continue; }
			BodySizeX = ComponentPtr2->GetComponentBodySizeW();
			BodySizeY = ComponentPtr2->GetComponentBodySizeH();
			BodyRatioX = BodySizeX/BodySizeW_Ave;
			BodyRatioY = BodySizeY/BodySizeH_Ave;
			if ( BodyRatioX > 1.5 || BodyRatioX < 0.5) { continue; }
			if ( BodyRatioY > 1.5 || BodyRatioY < 0.5) { continue; }
			BodySizeCount ++;
			BodySizeW += BodySizeX;
			BodySizeH += BodySizeY;
			BodyHeight+= ComponentPtr2->GetComponentResultHeight();
		}
		if ( 0 == BodySizeCount )
		{	continue; }		
		BodySizeW /= BodySizeCount;
		BodySizeH /= BodySizeCount;
		BodyHeight /= BodySizeCount;
		const double BodySizeW_Std = BodySizeW;
		const double BodySizeH_Std = BodySizeH;
		const double BodyHeight_Std = BodyHeight;

		for ( j=0; j<ComponentPtrCount; j++ )
		{
			ComponentPtr2 = ComponentPtrList[j];
			ModelPtr = ComponentPtr2->GetComponentModelPtr();
			
			ComponentAngle = ComponentPtr2->GetComponentAngle();
			int AngleLabel = JetAPI::GetAngleLabel(ComponentAngle);
			switch ( AngleLabel )
			{
			case 90:
			case 270:
				BodySizeW = BodySizeH_Std;
				BodySizeH = BodySizeW_Std;
				break;
			default:
				BodySizeW = BodySizeW_Std;
				BodySizeH = BodySizeH_Std;
				break;
			}

			ModelPtr->GetModelBodyBox().SetBoxSizeX(BodySizeW);
			ModelPtr->GetModelBodyBox().SetBoxSizeY(BodySizeH);
			ModelPtr->GetModelBodyBox().SetBoxSizeResX(BodySizeW);
			ModelPtr->GetModelBodyBox().SetBoxSizeResY(BodySizeH);
			ModelPtr->CalcModelTotalRegionAll();			
						
			ComponentPtr2->SetComponentBodySizeW(BodySizeW_Std);
			ComponentPtr2->SetComponentBodySizeH(BodySizeH_Std);
			ComponentPtr2->SetComponentRoiSizeW(BodySizeW_Std);
			ComponentPtr2->SetComponentRoiSizeH(BodySizeH_Std);			
			ComponentPtr2->SetComponentResultWidth(BodySizeW_Std);
			ComponentPtr2->SetComponentResultLength(BodySizeH_Std);
			ComponentPtr2->SetComponentResultHeight(BodyHeight_Std);
			ComponentPtr2->CalcComponentCadCornerPos();
			ComponentPtr2->LayoutComponentStageCornerPos();
			ComponentPtr2->SetComponentTempInt_01(FnEnable);
			if ( FnDisable == ComponentPtr2->GetComponentTempInt() ) 
			{	ComponentPtr2->SetComponentTempInt(FnEnable); }	
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::BuildProjectDimensionComponentList()//合併程專案尺寸列表
{
	int             UnionCnt=0;
	size_t          i=0, j=0, k=0;
	size_t          ModelWndCount=0;
	bool            bModifyRgn=false;
	double          BodySizeW=0;
	double          BodySizeH=0;		
	double          BodyHeight=0.0;
	CString         ComponentFullName;
	CString         ComponentImageName;
	double          ComponentCadPosX=0;
	double          ComponentCadPosY=0;
	double          ComponentStagePosX=0;
	double          ComponentStagePosY=0;	
	TREGION4D       ComponentRgn1;
	TREGION4D       ComponentRgn2;
	CAOIWnd        *WndPtr = NULL;
	CAOIModel      *ModelPtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;
	CAOIComponent  *ComponentPtr2 = NULL;		
	DISTRICT_ID     DistrictID = GetProjectActDistrictID();
	CAOIPanel *PanelPtr = GetProjectPanelPtr(0, true);
	if ( NULL == PanelPtr ) { return true; }
	CAOIBoard *BoardPtr = PanelPtr->GetPanelBoardPtr(0, true);
	if ( NULL == BoardPtr ) { return true; }	
	
	CMapCoordinate *STCMapPtr = BoardPtr->GetBoardMapSTCPtr(DistrictID);
	CMapCoordinate *CTSMapPtr = BoardPtr->GetBoardMapCTSPtr(DistrictID);
	std::vector<CAOIComponent*>    ProjectPartDimonsionList=m_ProjectPartDimensionList;//專案拋件指標列表	
	
	m_ProjectPartDimensionList.clear();		
	CString   SpcImageFolder = GetProjectSpcImageFolder();	
	const size_t    DimonsionCount = ProjectPartDimonsionList.size();		
	const size_t    FrameUniqueIDCount = GetProjectFrameUniqueIDCount();

	if ( GetProjectUseLocalFolder() )
	{	SpcImageFolder = GetProjectSpcImageFolderLocal();	}	

	for ( i=0; i<DimonsionCount; i++ )
	{
		ComponentPtr = ProjectPartDimonsionList[i];
		if ( NULL == ComponentPtr ) { continue; }		
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		UnionCnt = 1;
		bModifyRgn=false;		
		BodyHeight = ComponentPtr->GetComponentResultHeight();
		ComponentPtr->GetComponentBodyStageRegion(ComponentRgn1);
		for ( j=i+1; j<DimonsionCount; j++ )
		{
			ComponentPtr2 = ProjectPartDimonsionList[j];
			if ( NULL == ComponentPtr2 ) { continue; }
			ComponentPtr2->GetComponentBodyStageRegion(ComponentRgn2);
			if ( ComponentRgn2.minX > ComponentRgn1.maxX ) { continue; }
			if ( ComponentRgn2.minY > ComponentRgn1.maxY ) { continue; }
			if ( ComponentRgn2.maxX < ComponentRgn1.minX ) { continue; }
			if ( ComponentRgn2.maxY < ComponentRgn1.minY ) { continue; }
			bModifyRgn = true;
			//剔除重複的零件
			UnionCnt ++;
			BodyHeight += ComponentPtr2->GetComponentResultHeight();
			ComponentFullName = ComponentPtr2->GetComponentFullName();
			for ( k=0; k<FrameUniqueIDCount; k++ )
			{
				ComponentImageName.Format(_T("%s\\%s#%d.JPG"), SpcImageFolder, ComponentFullName, k+1);
				::DeleteFile(ComponentImageName);
				ComponentImageName.Format(_T("%s\\%s#%d.Z3D"), SpcImageFolder, ComponentFullName, k+1);
				::DeleteFile(ComponentImageName);
			}
			AOIObjManager.DestroyComponentObj(ProjectPartDimonsionList[j]);
			ProjectPartDimonsionList[j] = NULL;
			JetAPI::UnionRegion(ComponentRgn1, ComponentRgn2, ComponentRgn1);
		}	
		if ( true == bModifyRgn )
		{
			BodyHeight /= UnionCnt;
			BodySizeW = ComponentRgn1.GetWidth();
			BodySizeH = ComponentRgn1.GetHeight();
			ComponentStagePosX = ComponentRgn1.GetCpX();
			ComponentStagePosY = ComponentRgn1.GetCpY();
			
			ComponentCadPosX = ComponentPtr->GetComponentCadPosX();
			ComponentCadPosY = ComponentPtr->GetComponentCadPosY();
			STCMapPtr->Map2D(ComponentStagePosX, ComponentStagePosY, ComponentCadPosX, ComponentCadPosY);

			ComponentPtr->SetComponentBodySizeW(BodySizeW);
			ComponentPtr->SetComponentBodySizeH(BodySizeH);			
			ComponentPtr->SetComponentCadPosX(ComponentCadPosX);
			ComponentPtr->SetComponentCadPosY(ComponentCadPosY);
			ComponentPtr->SetComponentStagePosX(ComponentStagePosX);
			ComponentPtr->SetComponentStagePosY(ComponentStagePosY);		
			ComponentPtr->SetComponentRoiSizeW(BodySizeW);
			ComponentPtr->SetComponentRoiSizeH(BodySizeH);
			ComponentPtr->SetComponentResultHeight(BodyHeight);
			ComponentPtr->CalcComponentCadCornerPos();
			ComponentPtr->LayoutComponentStageCornerPos();

			ModelPtr = ComponentPtr->GetComponentModelPtr();
			ModelWndCount = ModelPtr->GetModelWndCount();
			for ( j=0; j<ModelWndCount; j++ )
			{
				WndPtr = ModelPtr->GetModelWndPtr(j, false);
				if ( NULL == WndPtr ) { continue; }
				WndPtr->GetWndBox().SetBoxSize(BodySizeW, BodySizeH);				
			}
			ModelPtr->GetModelBodyBox().SetBoxSize(BodySizeW, BodySizeH);
			ModelPtr->SetModelAttachedPosCad(ComponentCadPosX, ComponentCadPosY);
			ModelPtr->SetModelAttachedPosStage(ComponentStagePosX, ComponentStagePosY);
		}	
		AddProjectPartDimensionPtr_Inline(ComponentPtr);		
	}	
	const size_t    DimonsionCount2 = m_ProjectPartDimensionList.size();	
	if ( DimonsionCount2 != DimonsionCount )
	{	i = 3;	}	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::TestProjectPartDimension(CAOIField *FieldPtr)
{
	if ( TestProjectPartDimensionFn(FieldPtr) == false )
	{
		SetProjectExceptionCode(AOI_EXCEPTION_PROJECT_TEST_DIMENSION);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::TestProjectPartDimensionFn(CAOIField *FieldPtr)
{
	bool IsOK = true;
	const unsigned int DimonsionFrameUniqueID = GetProjectParameter().m_PartDimensionFrameUniqueID;	
	if ( FRAME_UNIQUE_ID_DLP == DimonsionFrameUniqueID )
	{	IsOK = TestProjectPartDimensionBy3DHeight(FieldPtr);	}	
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::TestProjectPartDimensionBy3DHeight(CAOIField *FieldPtr)//檢測尺寸-使用高度
{
	const char fnName[] = "CAOIProject::TestProjectPartDimensionBy3DHeight";
	const int TestDimonsion = GetProjectPartDimensionSaveImage();
	if ( FN_DISABLE == TestDimonsion ) { return true; }

	if ( NULL == FieldPtr ) { return false; }
	if ( FieldPtr->CheckFieldMergeFinish() == false ) { return true; }	
	const int TestFrameIndex = GetProjectParameter().m_PartDimensionFrameIndex;
	CAOIFrame *FramePtr = FieldPtr->GetFieldFramePtr(TestFrameIndex, true);
	if ( NULL == FramePtr )
	{
		m_ErrorString.Format(_T("Error, Project Test Part Dimension Frame index Execption [%d]"), TestFrameIndex+1);
		return false;
	}		

	const int    MapIndex = TestFrameIndex;
	FRAME_TYPE   FrameType = FramePtr->GetFrameType();
	IMAGE_SIZE   ImageW = FramePtr->GetFrameImageW();
	IMAGE_SIZE   ImageH = FramePtr->GetFrameImageH();
	IMAGE_SIZE   ImageStep = FramePtr->GetFrameImageStep();
	IMAGE_SIZE   BitCount = FramePtr->GetFrameImageBitCount();
	MASK_PTR     MaskPtr = FramePtr->GetFrameMaskPtr();	
	SPACE_PTR    SpacePtr = FramePtr->GetFrameSpacePtr();	
	IMAGE_PTR    ImagePtr = FramePtr->GetFrameImagePtr();	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const IMAGE_SIZE MaskBitCount = 8;
	const int SaveImage = FN_ENABLE;	

	const int nAlign = 4;
	const int nTeachMapW = (int)(m_ProjectMapW[MapIndex]);//專案底圖寬度
	const int nTeachMapH = (int)(m_ProjectMapH[MapIndex]);//專案底圖長度
	const int nTeachMapStep = (int)(m_ProjectMapStep[MapIndex]);//專案底圖步長
	const int nTeachMapBitCount = (int)(m_ProjectBitCount[MapIndex]);//專案底圖位元數
	const int nTeachMapW2 = nTeachMapW/2;
	const int nTeachMapH2 = nTeachMapH/2;
	IMAGE_PTR TeachMapPtr = m_ProjectMapPtr[MapIndex];//專案底圖指標

	const int nTeachMaskW = (int)(m_ProjectMapMaskW);
	const int nTeachMaskH = (int)(m_ProjectMapMaskH);
	const int nTeachMaskStep = (int)(m_ProjectMapMaskStep);
	const int nTeachMaskBitCount = (int)(m_ProjectMapMaskBitCount);
	IMAGE_PTR TeachMaskPtr = m_ProjectMapMaskPtr;

	if ( 8 != BitCount ) { return false; }
	if ( BitCount != nTeachMapBitCount )
	{	
		m_ErrorString.Format(_T("Error, Project Test Part Dimension BitCount != nTeachMapBitCount [%d]"), MapIndex+1);
		return false;
	}
	
	MASK_PTR     DstMskPtr=NULL;
	SPACE_PTR    DstSpcPtr=NULL;
	MASK_PTR     CellMskPtr=NULL;
	SPACE_PTR    CellSpcPtr=NULL;
	MASK_PTR     CellMsk2DPtr=NULL;	
	IMAGE_SIZE   uCellW = 0;
	IMAGE_SIZE   uCellH = 0;
	IMAGE_SIZE   uCellStep = 0;	
	const int    NPixels = GetProjectMapScaleMode();

#ifdef _DEBUG
	bool         bSave=true;	
#endif _DEBUG
	CString      str;
	CString      Folder;
	CString      MainName;
	CString      KeyName=_T("Dimension");	
	CString      FrameFileName = FramePtr->GetFrameFileName();
	std::vector<CString> SaveFileList;
	Folder = GetProjectDebugFolderDimonsion();
	JetAPI::ExtractMainFileName(FrameFileName, MainName);	

	if ( JetMemory.alloc_func(BufferSize, DstMskPtr, fnName, "DstMskPtr")==false ||
		 JetMemory.alloc_func(BufferSize, DstSpcPtr, fnName, "DstSpcPtr")==false ||
		 JetMemory.alloc_func(BufferSize, CellMskPtr, fnName, "CellMskPtr")==false ||
		 JetMemory.alloc_func(BufferSize, CellSpcPtr, fnName, "CellSpcPtr")==false  )
	{
		m_ErrorString = JetMemory.GetErrorString();		
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(CellSpcPtr);
		JetMemory.free_func(CellMskPtr);
		JetMemory.free_func(CellMsk2DPtr);
		return false; 
	}	
	const double ScaleVal = 1.00/NPixels;
	ImageAPI.CalcScaleSize(ImageW, ImageH, ImageStep, BitCount, ScaleVal, uCellW, uCellH, uCellStep);
	if ( ImageAPI.ScaleMask3(ImageW, ImageH, ImageStep, MaskPtr, ScaleVal, uCellW, uCellH, uCellStep, CellMskPtr) == false ||
		 ImageAPI.ScaleSpace3(ImageW, ImageH, ImageStep, SpacePtr, ScaleVal, uCellW, uCellH, uCellStep, CellSpcPtr) == false )	
	{
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(CellSpcPtr);
		JetMemory.free_func(CellMskPtr);
		JetMemory.free_func(CellMsk2DPtr);
		m_ErrorString = ImageAPI.GetImageApiErrorString();		
		return false;
	}

#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_CellMask.PNG"), Folder, KeyName, MainName);
		//ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellMskPtr, true);
		//SaveFileList.push_back(str);
	}
#endif//_DEBUG
	
	/*
	if ( AOIDataCollect.ExecEnhanceDisplayImage(uCellW, uCellH, uCellStep, BitCount, CellPtr, CellPtr) == false )
	{
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(CellSpcPtr);
		JetMemory.free_func(CellMskPtr);
		JetMemory.free_func(CellMsk2DPtr);
		m_ErrorString = AOIDataCollect.GetErrorString();		
		return false;	
	}
	*/
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_CellMask2.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, CellMskPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG	
	
	const size_t CellBufSize=ImageAPI.CalcBufferSize(uCellStep, uCellH);	
	const int    OpenMPCnt = AOIDataCollect.GetOpenMPCount_Inspection();	
	const double PanelBasePlane = FieldPtr->GetFieldPanelBasePlane();	
	TNoiseFilterParam NoiseParam = GetProjectParameter().m_PartDimensionSpaceNoiseFilter;	
	NoiseParam.BasePlaneParam.PanelBasePlane = PanelBasePlane;	
	if ( ImageAPI.BuildSpaceData3(uCellW, uCellH, uCellStep, CellSpcPtr, CellMskPtr, CellMsk2DPtr, OpenMPCnt, NoiseParam, DstSpcPtr, DstMskPtr) == false )
	{	
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(CellSpcPtr);
		JetMemory.free_func(CellMskPtr);
		JetMemory.free_func(CellMsk2DPtr);
		m_ErrorString = ImageAPI.GetImageApiErrorString();	
		return false;
	}
#ifdef _DEBUG
	if ( true == bSave )
	{
		RECT LocRect={0};
		IMAGE_PTR DstImage=CellMskPtr;
		JetAPI::SizeToRect(uCellW, uCellH, LocRect);
		const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
		ImageAPI.SpaceGrayImageConvertToGray3(uCellW, uCellH, uCellStep, DstSpcPtr, DstMskPtr, LocRect, uCellStep, DstImage, SpaceRatio, false);
		str.Format(_T("%s\\%s_%s_Space.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, uCellW, uCellH, uCellStep, BitCount, DstImage, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG
	JetMemory.free_func(CellSpcPtr);
	JetMemory.free_func(CellMskPtr);
	JetMemory.free_func(CellMsk2DPtr);

	//尋找在專案底圖的基本方位		
	TPOINT2D MapRes;	
	TREGION4D CadRgn;
	TREGION4D CellRgn;
	TREGION4D StageRgn;	
	TREGION4D TeachRgn;
	const double FieldStagePosX = FieldPtr->GetFieldStagePosX();
	const double FieldStagePosY = FieldPtr->GetFieldStagePosY();
	const double FieldStageSizeW = FieldPtr->GetFieldSizeW_Real();
	const double FieldStageSizeH = FieldPtr->GetFieldSizeH_Real();	

	GetProjectMapInfo(MapRes, CadRgn, StageRgn);
	GetProjectMapCalcRgn(TeachRgn);
	TeachRgn = StageRgn;

	CellRgn.minX = FieldStagePosX-(FieldStageSizeW*0.5);
	CellRgn.minY = FieldStagePosY-(FieldStageSizeH*0.5);
	CellRgn.maxX = FieldStagePosX+(FieldStageSizeW*0.5);
	CellRgn.maxY = FieldStagePosY+(FieldStageSizeH*0.5);

	int MapRoiPosX = 0;
	int MapRoiPosY = 0;	
	const double MapTeachCpX = TeachRgn.GetCpX();
	const double MapTeachCpY = TeachRgn.GetCpY();
	const double RgnOffsetX = (FieldStagePosX-MapTeachCpX);
	const double RgnOffsetY = (FieldStagePosY-MapTeachCpY);
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	if ( true == SignX )
	{	MapRoiPosX = (int)(nTeachMapW2+(RgnOffsetX/MapRes.x));	}
	else
	{	MapRoiPosX = (int)(nTeachMapW2-(RgnOffsetX/MapRes.x)); }	
	if ( true == SignY )
	{	MapRoiPosY = (int)(nTeachMapH2+(RgnOffsetY/MapRes.y));	}
	else
	{	MapRoiPosY = (int)(nTeachMapH2-(RgnOffsetY/MapRes.y));	}
	MapRoiPosY = nTeachMapH-MapRoiPosY;	

	RECT  MapRoiRect={0};	
	RECT  CellRoiRect={0};
	bool  ModifyCellRoi=false;
	IMAGE_SIZE RoiExt = 0;//沒有在重新對位
	IMAGE_SIZE MapRoiW = uCellW+RoiExt;
	IMAGE_SIZE MapRoiH = uCellH+RoiExt;
	IMAGE_SIZE MapRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, BitCount, nAlign);
	MapRoiRect.left   = MapRoiPosX-(MapRoiW/2);
	MapRoiRect.top    = MapRoiPosY-(MapRoiH/2);
	MapRoiRect.right  = MapRoiPosX+(MapRoiW/2);
	MapRoiRect.bottom = MapRoiPosY+(MapRoiH/2);		
	ModifyCellRoi = false;
	JetAPI::SizeToRect(uCellW, uCellH, CellRoiRect);	
	if ( MapRoiRect.left < 0 )
	{	
		ModifyCellRoi = true;
		CellRoiRect.left = CellRoiRect.left+(0-MapRoiRect.left);
		MapRoiRect.left = 0;
	}
	if ( MapRoiRect.right > nTeachMapW )
	{
		ModifyCellRoi = true;
		CellRoiRect.right = CellRoiRect.right-(MapRoiRect.right-nTeachMapW);
		MapRoiRect.right = nTeachMapW;		
	}
	if ( MapRoiRect.top < 0 )
	{	
		ModifyCellRoi = true;
		CellRoiRect.top = CellRoiRect.top+(0-MapRoiRect.top);
		MapRoiRect.top = 0;
	}
	if ( MapRoiRect.bottom > nTeachMapH )
	{
		ModifyCellRoi = true;
		CellRoiRect.bottom = CellRoiRect.bottom-(MapRoiRect.bottom-nTeachMapH);
		MapRoiRect.bottom = nTeachMapH;		
	}
	if ( CellRoiRect.right<=CellRoiRect.left || CellRoiRect.bottom<=CellRoiRect.top )
	{
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
	#ifdef _DEBUG
		JetAPI::DeleteFileList(SaveFileList);
	#endif//_DEBUG
		return true; 			
	}
	if ( true == ModifyCellRoi )
	{
		if ( true == SignX )
		{
			CellRgn.minX += (CellRoiRect.left-0)*MapRes.x;
			CellRgn.maxX -= (uCellW-CellRoiRect.right)*MapRes.x;
		}
		else
		{	
			CellRgn.maxX -= (CellRoiRect.left-0)*MapRes.x;
			CellRgn.minX += (uCellW-CellRoiRect.right)*MapRes.x;		
		}	
		if ( true == SignY )
		{	
			CellRgn.maxY -= (CellRoiRect.top-0)*MapRes.y;
			CellRgn.minY += (uCellH-CellRoiRect.bottom)*MapRes.y;
		}
		else
		{
			CellRgn.minY += (CellRoiRect.top-0)*MapRes.y;
			CellRgn.maxY -= (uCellH-CellRoiRect.bottom)*MapRes.y;
		}

		MASK_PTR   CellRoiMskPtr=NULL;
		SPACE_PTR  CellRoiSpcPtr=NULL;
		IMAGE_SIZE CellRoiW=CellRoiRect.right-CellRoiRect.left;
		IMAGE_SIZE CellRoiH=CellRoiRect.bottom-CellRoiRect.top;
		IMAGE_SIZE CellRoiStep=JetAPI::GetBMPImagePixelsPerLine(CellRoiW, BitCount, nAlign);;
		const size_t CellRoiBuffserSize = ImageAPI.CalcBufferSize(CellRoiStep, CellRoiH);
		if ( JetMemory.alloc_func(CellRoiBuffserSize, CellRoiMskPtr, fnName, "CellRoiMskPtr") == false ||
			 JetMemory.alloc_func(CellRoiBuffserSize, CellRoiSpcPtr, fnName, "CellRoiSpcPtr") == false  )
		{
			JetMemory.free_func(DstSpcPtr);
			JetMemory.free_func(DstMskPtr);		
			JetMemory.free_func(CellRoiSpcPtr);
			JetMemory.free_func(CellRoiMskPtr);		
			m_ErrorString = JetMemory.GetErrorString();		
			return false;
		}
		if ( ImageAPI.ExtractRoiImage3(uCellW, uCellH, uCellStep, BitCount, DstMskPtr, CellRoiRect, CellRoiStep, CellRoiMskPtr, false) == false )
		{
			JetMemory.free_func(DstSpcPtr);
			JetMemory.free_func(DstMskPtr);		
			JetMemory.free_func(CellRoiSpcPtr);
			JetMemory.free_func(CellRoiMskPtr);		
			m_ErrorString = JetMemory.GetErrorString();		
			return false;
		}
		if ( ImageAPI.ExtractSpaceRoiImage3(uCellW, uCellH, uCellStep, BitCount, DstSpcPtr, CellRoiRect, CellRoiStep, CellRoiSpcPtr, false) == false )
		{
			JetMemory.free_func(DstSpcPtr);
			JetMemory.free_func(DstMskPtr);		
			JetMemory.free_func(CellRoiSpcPtr);
			JetMemory.free_func(CellRoiMskPtr);		
			m_ErrorString = JetMemory.GetErrorString();		
			return false;
		}		
	#ifdef _DEBUG
		if ( true == bSave )
		{
			RECT LocRect={0};
			IMAGE_PTR DstImage=DstMskPtr;
			JetAPI::SizeToRect(CellRoiW, CellRoiH, LocRect);
			const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
			ImageAPI.SpaceGrayImageConvertToGray3(CellRoiW, CellRoiH, CellRoiStep, CellRoiSpcPtr, CellRoiMskPtr, LocRect, CellRoiStep, DstImage, SpaceRatio, false);
			str.Format(_T("%s\\%s_%s_Space3.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, CellRoiW, CellRoiH, CellRoiStep, BitCount, DstImage, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		uCellW = CellRoiW;
		uCellH = CellRoiH;
		uCellStep = CellRoiStep;
		DstSpcPtr = CellRoiSpcPtr;
		DstMskPtr = CellRoiMskPtr;
		MapRoiW = MapRoiRect.right-MapRoiRect.left;
		MapRoiH = MapRoiRect.bottom-MapRoiRect.top;
		MapRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, BitCount, nAlign);
	}
	
	IMAGE_PTR    MapMaskRoiPtr = NULL;
	const IMAGE_SIZE MapMaskRoiStep = JetAPI::GetBMPImagePixelsPerLine(MapRoiW, MaskBitCount, nAlign);
	const size_t MapMaskRoiBufferSize = ImageAPI.CalcBufferSize(MapMaskRoiStep, MapRoiH);
	if ( NULL!=TeachMaskPtr && nTeachMaskW==nTeachMapW && nTeachMaskH==nTeachMapH )
	{
		if ( JetMemory.alloc_func(MapMaskRoiBufferSize, MapMaskRoiPtr, fnName, "MapMaskRoiPtr") == false )
		{
			JetMemory.free_func(DstSpcPtr);
			JetMemory.free_func(DstMskPtr);
			JetMemory.free_func(MapMaskRoiPtr);			
			m_ErrorString = JetMemory.GetErrorString();				
			return false; 
		}			
		if ( ImageAPI.ExtractRoiImage3(nTeachMaskW, nTeachMaskH, nTeachMaskStep, MaskBitCount, TeachMaskPtr, MapRoiRect, MapMaskRoiStep, MapMaskRoiPtr, false) == false )
		{
			JetMemory.free_func(DstSpcPtr);
			JetMemory.free_func(DstMskPtr);
			JetMemory.free_func(MapMaskRoiPtr);			
			m_ErrorString = ImageAPI.GetImageApiErrorString();		
			return false;
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MapMaskRoi.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MapRoiW, MapRoiH, MapMaskRoiStep, MaskBitCount, MapMaskRoiPtr, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
	}

	MASK_PTR     RawMaskPtr = NULL;	
	MASK_PTR     TmpMaskPtr = NULL;	
	MASK_PTR     PartMaskPtr = NULL;	
	const IMAGE_SIZE MaskW  = uCellW;
	const IMAGE_SIZE MaskH  = uCellH;	
	const IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(MaskW, MaskBitCount, nAlign);
	const size_t MaskBufferSize = ImageAPI.CalcBufferSize(MaskStep, MaskH);
	if ( JetMemory.alloc_func(MaskBufferSize, RawMaskPtr, fnName, "RawMaskPtr") == false || 		 
		 JetMemory.alloc_func(MaskBufferSize, TmpMaskPtr, fnName, "TmpMaskPtr") == false ||
		 JetMemory.alloc_func(MaskBufferSize, PartMaskPtr, fnName, "PartMaskPtr") == false )
	{
		m_ErrorString = JetMemory.GetErrorString();		
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtr);		
		JetMemory.free_func(TmpMaskPtr);
		JetMemory.free_func(PartMaskPtr);
		return false; 
	}

	//剔除此FOV內的零件
	size_t       i=0, j=0;	
	TREGION4D    FieldRgn=CellRgn;
	const int    MaskMode=PROJECT_PART_MASK_ALL;
	::memset(RawMaskPtr, 0xFF, sizeof(MASK_DATA)*MaskBufferSize);
	if ( CreateProjectPartMaskImage(MaskW, MaskH, MaskStep, RawMaskPtr, FieldRgn, MapRes, MaskMode, 0x00) == false )
	{
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtr);		
		JetMemory.free_func(TmpMaskPtr);
		JetMemory.free_func(PartMaskPtr);		
		return false; 
	}		
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskNoPart.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, RawMaskPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG	
	if ( ImageAPI.ErodeGrayImage3(MaskW, MaskH, MaskStep, RawMaskPtr, 9, 1, PartMaskPtr) == false )
	{		
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(MapMaskRoiPtr);
		JetMemory.free_func(RawMaskPtr);		
		JetMemory.free_func(TmpMaskPtr);
		JetMemory.free_func(PartMaskPtr);
		return false;
	}
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskNoPartErode.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, PartMaskPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG
	::memset(PartMaskPtr, 0xFF, sizeof(MASK_DATA)*MaskBufferSize);//清除有零件的特徵

	const SPACE_DATA PartOverLow  = GetProjectParameter().m_PartDimensionOverLow;
	const SPACE_DATA PartOverHigh = GetProjectParameter().m_PartDimensionOverHigh;
	for ( i=0; i<MaskBufferSize; i++ )
	{
		RawMaskPtr[i] = 0;
		//Space Noise
		if ( ImageAPI.CheckSpaceMaskValid(DstMskPtr[i]) == false ) { continue; }
		//Part In Fov
		if ( 0 == PartMaskPtr[i] ) { continue; }
		if ( DstSpcPtr[i] < PartOverLow ) { continue; }
		if ( DstSpcPtr[i] > PartOverHigh ) { continue; }
		RawMaskPtr[i] = 255;
	}
	if ( NULL != MapMaskRoiPtr )
	{
		for ( i=0; i<MaskBufferSize; i++ )
		{
			if ( 0xFF == MapMaskRoiPtr[i] ) { continue; }
			RawMaskPtr[i] = 0;			
		}
	}

#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskRaw.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, RawMaskPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG	

	//JetMemory.free_func(DstSpcPtr);
	//JetMemory.free_func(DstMskPtr);
	JetMemory.free_func(PartMaskPtr);
	JetMemory.free_func(MapMaskRoiPtr);	

	if ( ExecProjectPartDimensionMaskFilter(FieldPtr, MapRes, MaskW, MaskH, MaskStep, RawMaskPtr, TmpMaskPtr, SaveFileList) == false )
	{
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(RawMaskPtr);		
		JetMemory.free_func(TmpMaskPtr);
		return false;
	}
#ifdef _DEBUG
	if ( true == bSave )
	{
		str.Format(_T("%s\\%s_%s_MaskAfter.PNG"), Folder, KeyName, MainName);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TmpMaskPtr, true);
		SaveFileList.push_back(str);
	}
#endif//_DEBUG	

	::memcpy(RawMaskPtr, TmpMaskPtr, sizeof(MASK_DATA)*MaskBufferSize);
	JetMemory.free_func(TmpMaskPtr);	
	
	const bool bMeasure = true;
	const double MinThickness=0;
	std::vector<CAOIComponent*> NewComponentList;
	const double PartW = GetProjectParameter().m_PartDimensionMinSizeW;
	const double PartH = GetProjectParameter().m_PartDimensionMinSizeH;
	const double PartWHRatio = GetProjectParameter().m_PartDimensionMaxSizeR;
	if ( CreateProjectComponentByBlob(MaskW, MaskH, MaskStep, RawMaskPtr, DstMskPtr, DstSpcPtr, FieldRgn, MapRes, PartW, PartH, PartWHRatio, MinThickness, bMeasure, NewComponentList) == false )
	{	
		JetMemory.free_func(DstSpcPtr);
		JetMemory.free_func(DstMskPtr);
		JetMemory.free_func(RawMaskPtr);		
		JetMemory.free_func(TmpMaskPtr);
		return false;
	}	
	JetMemory.free_func(DstSpcPtr);
	JetMemory.free_func(DstMskPtr);
	JetMemory.free_func(RawMaskPtr);
	JetMemory.free_func(TmpMaskPtr);

	const size_t MaxComponentInFov = 10000;
	const size_t NewComponentCount = NewComponentList.size();
	if ( 0 == NewComponentCount )//|| NewComponentCount>MaxComponentInFov )
	{
	#ifdef _DEBUG
		JetAPI::DeleteFileList(SaveFileList);
	#endif//_DEBUG
		return true;
	}	
	
	if ( CreateProjectPartDimensionByComponentList(FieldPtr, NewComponentList) == false )
	{	return false;	}		
	//if ( SaveProjectSpecTestField(FieldPtr) == false )
	//{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::ExecProjectPartDimensionMaskFilter(CAOIField *FieldPtr, const TPOINT2D &MapRes, IMAGE_SIZE MaskW, IMAGE_SIZE MaskH, IMAGE_SIZE MaskStep, MASK_PTR MaskPtr, MASK_PTR DstPtr, std::vector<CString> &SaveFileList)//執行尺寸遮罩過濾
{
	const char fnName[] = "CAOIProject::ExecProjectPartDimensionMaskFilter";
	if ( NULL == FieldPtr ) { return false; }
	if ( NULL == MaskPtr || NULL == DstPtr )
	{	return false; }

	CString      str;
	MASK_PTR     RawMaskPtr = NULL;
	MASK_PTR     TestMaskPtr = NULL;
	MASK_PTR     TestMaskPtr1 = NULL;
	MASK_PTR     TestMaskPtr2 = NULL;
	IMAGE_SIZE   MaskBitCount = 8;
#ifdef _DEBUG
	bool         bSave=true;	
#endif _DEBUG	
	CString      MainName;
	CString      KeyName=_T("Dimension");	
	CString      Folder = GetProjectDebugFolderDimonsion();
	const size_t MaskBufferSize = ImageAPI.CalcBufferSize(MaskStep, MaskH);
	if ( JetMemory.alloc_func(MaskBufferSize, RawMaskPtr, fnName, "RawMaskPtr") == false ||
		 JetMemory.alloc_func(MaskBufferSize, TestMaskPtr, fnName, "TestMaskPtr") == false ||
		 JetMemory.alloc_func(MaskBufferSize, TestMaskPtr1, fnName, "TestMaskPtr1") == false ||
		 JetMemory.alloc_func(MaskBufferSize, TestMaskPtr2, fnName, "TestMaskPtr2") == false  )
	{
		JetMemory.free_func(RawMaskPtr);
		JetMemory.free_func(TestMaskPtr);
		JetMemory.free_func(TestMaskPtr1);
		JetMemory.free_func(TestMaskPtr2);
		return false;
	}		
	//影像處理
	
	//膨脹-Dilate, In::MaskPtr, Out:RawMaskPtrL;
	const int DilateSize = GetProjectParameter().m_PartDimensionDilateSize;		
	if ( 0 == DilateSize ) 
	{	::memcpy(RawMaskPtr, MaskPtr, sizeof(MASK_DATA)*MaskBufferSize);	}
	else
	{			
		const int nDilateSize=ImageAPI.GetKernelSize(DilateSize);
		//::memcpy(RawMaskPtr, MaskPtr, sizeof(MASK_DATA)*MaskBufferSize);
		if ( ImageAPI.DilateGrayImage3(MaskW, MaskH, MaskStep, MaskPtr, nDilateSize, 1, RawMaskPtr) == false )
		{	
			JetMemory.free_func(RawMaskPtr);
			JetMemory.free_func(TestMaskPtr);
			JetMemory.free_func(TestMaskPtr1);
			JetMemory.free_func(TestMaskPtr2);
			return false;
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MaskCompareDilateL.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, RawMaskPtr, true);
			SaveFileList.push_back(str);			
		}
	#endif//_DEBUG
	}

	//Open, In::RawMaskPtrL, Out:TestMaskPtr;
	int   MorphMode = 0;
	int   ShapeMode = MORPH_SHAPE_ELLIPSE;//MORPH_SHAPE_RECT, MORPH_SHAPE_CROSS, MORPH_SHAPE_ELLIPSE
	const int NoiseFilterOpen = GetProjectParameter().m_PartDimensionFilterOpenSize;	
	if ( 0 == NoiseFilterOpen ) 
	{	::memcpy(TestMaskPtr, RawMaskPtr, sizeof(MASK_DATA)*MaskBufferSize);	}
	else
	{		
		MorphMode = MORPH_OPEN;		
		const int OpenSizeL=ImageAPI.GetKernelSize(NoiseFilterOpen);
		if ( ImageAPI.MorphGrayImage3(MaskW, MaskH, MaskStep, RawMaskPtr, MorphMode, ShapeMode, OpenSizeL, 1, TestMaskPtr) == false )
		{	
			JetMemory.free_func(RawMaskPtr);			
			JetMemory.free_func(TestMaskPtr);
			JetMemory.free_func(TestMaskPtr1);
			JetMemory.free_func(TestMaskPtr2);
			return false;
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MaskCompareOpenL.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr, true);
			SaveFileList.push_back(str);			
		}
	#endif//_DEBUG
	}

	//Close, In::TestMaskPtr, Out:TestMaskPtr2;
	const int NoiseFilterClose = GetProjectParameter().m_PartDimensionFilterCloseSize;		
	if ( 0 == NoiseFilterClose ) 
	{	::memcpy(TestMaskPtr2, TestMaskPtr, sizeof(MASK_DATA)*MaskBufferSize);	}
	else
	{
		MorphMode = MORPH_CLOSE;
		const int CloseSize = ImageAPI.GetKernelSize(NoiseFilterClose);
		if ( ImageAPI.MorphGrayImage3(MaskW, MaskH, MaskStep, TestMaskPtr, MorphMode, ShapeMode, CloseSize, 1, TestMaskPtr2) == false )	
		{	
			JetMemory.free_func(RawMaskPtr);		
			JetMemory.free_func(TestMaskPtr);
			JetMemory.free_func(TestMaskPtr1);
			JetMemory.free_func(TestMaskPtr2);
			return false;
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MaskCompareClose.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
	}
	
	//剔除外圍的部分, In::TestMaskPtr2, Out:TestMaskPtr2;
	size_t       i=0;
	size_t       MaskIdx=0;
	const bool   ExcludeOutside = false;
	const double FieldStageSizeW_Real = FieldPtr->GetFieldSizeW_Real();
	const double FieldStageSizeH_Real = FieldPtr->GetFieldSizeH_Real();
	const double FieldStageSizeW_Inner = FieldPtr->GetFieldSizeW_Inner();
	const double FieldStageSizeH_Inner = FieldPtr->GetFieldSizeH_Inner();
	if ( true == ExcludeOutside )
	{
		double dMarginW = (FieldStageSizeW_Real-FieldStageSizeW_Inner)/MapRes.x;
		double dMarginH = (FieldStageSizeH_Real-FieldStageSizeH_Inner)/MapRes.y;		
		int MarginW = (int)(dMarginW+0.5);
		int MarginH = (int)(dMarginH+0.5);
		MarginW = MarginW/2;
		MarginH = MarginH/2;
		if ( MarginW>=0 && MarginH>=0 )
		{
			//Top && Bottom
			for ( i=0; i<MarginH; i++ )
			{
				MaskIdx = i*MaskStep;
				::memset(&TestMaskPtr2[MaskIdx], 0x00, sizeof(MASK_DATA)*MaskStep);

				MaskIdx = (MaskH-i-1)*MaskStep;
				::memset(&TestMaskPtr2[MaskIdx], 0x00, sizeof(MASK_DATA)*MaskStep);
			}
			
			//Left & Right
			for ( i=MarginH; i<MaskH-MarginH; i++ )
			{
				//Left
				MaskIdx = i*MaskStep;
				::memset(&TestMaskPtr2[MaskIdx], 0x00, sizeof(MASK_DATA)*MarginW);

				MaskIdx = (i*MaskStep)+MaskW-MarginW;
				::memset(&TestMaskPtr2[MaskIdx], 0x00, sizeof(MASK_DATA)*MarginW);
			}
		}
	#ifdef _DEBUG
		if ( true == bSave )
		{
			str.Format(_T("%s\\%s_%s_MaskExcludeOutside.PNG"), Folder, KeyName, MainName);
			ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, TestMaskPtr2, true);
			SaveFileList.push_back(str);
		}
	#endif//_DEBUG
	}

	//Output
	::memcpy(DstPtr, TestMaskPtr2, sizeof(MASK_DATA)*MaskBufferSize);

	JetMemory.free_func(RawMaskPtr);		
	JetMemory.free_func(TestMaskPtr);
	JetMemory.free_func(TestMaskPtr1);
	JetMemory.free_func(TestMaskPtr2);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CreateProjectPartDimensionByComponentList(CAOIField *FieldPtr, std::vector<CAOIComponent*> &ComponentList)//依照零件列表建立尺寸列表
{
	const char fnName[] = "CAOIProject::CreateProjectPartDimensionByComponentList";
	if ( NULL == FieldPtr ) { return false; }
	
	CString        str;
	size_t         i=0, j=0;	
	unsigned int UniqueID=0;
	wchar_t ComponentName[MAX_JET_PATH]=L"";
	const wchar_t  PartNumber[32]=L"DIMENSION";
	const wchar_t  ModelName[32]=L"DIMENSION";
	const wchar_t  NozzleName[32]=L"DIMENSION";	
	CString        SpcImageFolder;
	CString        ComponentFullName;
	double         BodySizeW=0;
	double         BodySizeH=0;		
	TPOINT2D       FrameRes;
	TPOINT2D       FieldStageCp;	
	TREGION4D      FieldStageRegion;
	TREGION4D      ModelStageRegion;
	RECT           ComponentImageRect={0,0,0,0};
	double         ComponentCadPosX=0;
	double         ComponentCadPosY=0;
	double         ComponentStagePosX=0;
	double         ComponentStagePosY=0;	
	TREGION4D      ComponentStageRegion;
	TREGION4D      ComponentImageRegion;
	CAOIWnd       *WndPtr = NULL;
	CAOIModel     *ModelPtr = NULL;	
	CAOIComponent *NewComponentPtr=NULL;
	const bool     bAppend=false;
	const bool     bEnhance=true;
	const bool     bSave3D=true;
	const bool     bSaveDefectImage = false;//AOIDataCollect.GetSaveDefectImage();
	TUNI_FRAME     FrameUniFrame;
	TUNI_FRAME     ComponentUniFrame;
	std::vector<TUNI_FRAME> ComponentUniFrameList;	
	const size_t   NewComponentCount = ComponentList.size();
	const double   PanelBasePlane = FieldPtr->GetFieldPanelBasePlane();
	const size_t   OldDimonsionCount = GetProjectPartDimensionCount_Inline();	
	const unsigned int DimonsionFrameIndex = GetProjectParameter().m_PartDimensionFrameIndex;
	const unsigned int DimonsionFrameUniqueID = GetProjectParameter().m_PartDimensionFrameUniqueID;
	TNoiseFilterParam  NoiseFilterParam = GetProjectParameter().m_PartDimensionSpaceNoiseFilter;	
	
	FieldStageCp.x = FieldPtr->GetFieldStagePosX();
	FieldStageCp.y = FieldPtr->GetFieldStagePosY();	
	SpcImageFolder = GetProjectSpcImageFolder();	
	NoiseFilterParam.BasePlaneParam.PanelBasePlane = PanelBasePlane;

	if ( GetProjectUseLocalFolder() )
	{	SpcImageFolder = GetProjectSpcImageFolderLocal();	}	

	LockProject();	
	for ( i=0; i<NewComponentCount; i++ )
	{
		NewComponentPtr = ComponentList[i];
		if ( NULL == NewComponentPtr ) { continue; }

		WndPtr = AOIObjManager.CreateWndObj();		
		if ( NULL==WndPtr ) 
		{	continue;	}

		UniqueID = OldDimonsionCount+i;
		::swprintf(ComponentName, L"%s_%d", ModelName, UniqueID+1);
		NewComponentPtr->SetComponentFieldPtr(FieldPtr);
		NewComponentPtr->SetComponentName(ComponentName);	
		NewComponentPtr->SetComponentModelName(ModelName);
		NewComponentPtr->SetComponentPartNumber(PartNumber);
		NewComponentPtr->SetComponentNozzleName(NozzleName);
		NewComponentPtr->SetComponentType(COMPONENT_TYPE_TEMPORARY);
		NewComponentPtr->SetComponentResultID_AOI(RESULT_ID_NG);
		NewComponentPtr->SetComponentPanelBasePlane(PanelBasePlane);
		NewComponentPtr->SetComponentSpaceBasePlaneParam(NoiseFilterParam.BasePlaneParam);
		NewComponentPtr->SetComponentSpaceNoiseFilterParam(NoiseFilterParam);

		BodySizeW = NewComponentPtr->GetComponentBodySizeW();
		BodySizeH = NewComponentPtr->GetComponentBodySizeH();
		ComponentCadPosX = NewComponentPtr->GetComponentCadPosX();
		ComponentCadPosY = NewComponentPtr->GetComponentCadPosY();
		ComponentStagePosX = NewComponentPtr->GetComponentStagePosX();
		ComponentStagePosY = NewComponentPtr->GetComponentStagePosY();		
		NewComponentPtr->SetComponentResultWidth(BodySizeW);
		NewComponentPtr->SetComponentResultLength(BodySizeH);
		NewComponentPtr->GetComponentRoiStageRegion(ComponentStageRegion);

		WndPtr->SetWndResultID(RESULT_ID_NG);
		WndPtr->SetWndDefectID(WND_DEFECT_FOREIGN_BODY);		
		WndPtr->GetWndBox().SetBoxSize(BodySizeW, BodySizeH);				
		WndPtr->GetWndAlgParam().SetAlgResultID(RESULT_ID_NG);
		WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameIndex(DimonsionFrameIndex);
		WndPtr->GetWndAlgParam().GetAlgImageBinParam().SetBinaryFrameUniqueID(DimonsionFrameUniqueID);		

		ModelPtr = NewComponentPtr->GetComponentModelPtr();
		ModelPtr->SetModelName(ModelName);
		ModelPtr->SetModelType(MODEL_TYPE_OTHERS);		
		ModelPtr->GetModelBodyBox().SetBoxSize(BodySizeW, BodySizeH);
		ModelPtr->AddModelWndPtr(WndPtr, false);
		ModelPtr->SetModelResultID(RESULT_ID_NG);		
		ModelPtr->SetModelAttachedPosCad(ComponentCadPosX, ComponentCadPosY);
		ModelPtr->SetModelAttachedPosStage(ComponentStagePosX, ComponentStagePosY);		
		ModelPtr->CalcModelTotalRegionAll();
		ModelPtr->GetModelTotalRegionStage(ModelStageRegion);		
		AddProjectPartDimensionPtr(NewComponentPtr, false);//加入拋件列表內-暫存
		
		//存出圖片
		if ( true == bSaveDefectImage )
		{
			CAOIFrame   *FieldFramePtr = NULL;
			const size_t FiledFrameCount = FieldPtr->GetFieldFramePtrCount();
			ComponentStageRegion = ModelStageRegion;
			ComponentFullName = NewComponentPtr->GetComponentFullName();			
			str.Format(_T("%s\\%s.%s"), SpcImageFolder, ComponentFullName, _T("JPG"));			
			for ( j=0; j<FiledFrameCount; j++ )
			{
				FieldFramePtr = FieldPtr->GetFieldFramePtr(j, false);
				if ( NULL == FieldFramePtr ) { continue; }				
				FrameRes.x = FieldFramePtr->GetFrameResolutionX();
				FrameRes.y = FieldFramePtr->GetFrameResolutionY();
				FieldFramePtr->GetFrameUniFrame(FrameUniFrame);
				if ( 0 == FrameUniFrame.BitCount ) 
				{	continue; }

				AOIDataCollect.MapStageRegionToCamera(FrameUniFrame.ImageW, FrameUniFrame.ImageH, FrameRes, ComponentStageRegion, FieldStageCp, ComponentImageRegion);
				JetAPI::Region4DToRect(ComponentImageRegion, ComponentImageRect, true);
				JetAPI::BoundaryRect(FrameUniFrame.ImageW, FrameUniFrame.ImageH, ComponentImageRect);//調整尺寸, 確保小於相機影像內

				ComponentUniFrame = TUNI_FRAME();
				ComponentUniFrame.ImageW = (ComponentImageRect.right-ComponentImageRect.left);
				ComponentUniFrame.ImageH = (ComponentImageRect.bottom-ComponentImageRect.top);
				ComponentUniFrame.BitCount = FrameUniFrame.BitCount;
				ComponentUniFrame.ImageStep = JetAPI::GetBMPImagePixelsPerLine(ComponentUniFrame.ImageW, FrameUniFrame.BitCount, 4);
				if ( NULL==FrameUniFrame.SpacePtr || NULL==FrameUniFrame.MaskPtr )
				{
					if ( ImageAPI.ExtractRoiImage(FrameUniFrame.ImageW, FrameUniFrame.ImageH, FrameUniFrame.ImageStep, FrameUniFrame.BitCount, FrameUniFrame.ImagePtr, ComponentImageRect, ComponentUniFrame.ImageStep, ComponentUniFrame.ImagePtr, false) == false )
					{
						JetAPI::ClearUniFrame(ComponentUniFrame);
						continue; 
					}
				}
				else
				{
					if ( ImageAPI.ExtractRoiImage(FrameUniFrame.ImageW, FrameUniFrame.ImageH, FrameUniFrame.ImageStep, FrameUniFrame.BitCount, FrameUniFrame.MaskPtr, ComponentImageRect, ComponentUniFrame.ImageStep, ComponentUniFrame.MaskPtr, false) == false )
					{	
						JetAPI::ClearUniFrame(ComponentUniFrame);
						continue; 
					}

					if ( ImageAPI.ExtractSpaceRoiImage(FrameUniFrame.ImageW, FrameUniFrame.ImageH, FrameUniFrame.ImageStep, FrameUniFrame.BitCount, FrameUniFrame.SpacePtr, ComponentImageRect, ComponentUniFrame.ImageStep, ComponentUniFrame.SpacePtr, false) == false )
					{	
						JetAPI::ClearUniFrame(ComponentUniFrame);
						continue; 
					}
					
					MASK_PTR     Mask2DPtr=NULL;
					MASK_PTR     DstMaskPtr=NULL;
					SPACE_PTR    DstSpacePtr=NULL;										
					IMAGE_SIZE   SrcW=ComponentUniFrame.ImageW;
					IMAGE_SIZE   SrcH=ComponentUniFrame.ImageH;
					IMAGE_SIZE   SrcStep=ComponentUniFrame.ImageStep;
					MASK_PTR     SrcMaskPtr=ComponentUniFrame.MaskPtr;
					SPACE_PTR    SrcSpacePtr=ComponentUniFrame.SpacePtr;
					const size_t SrcSize=ImageAPI.CalcBufferSize(SrcStep, SrcH);					
					if ( JetMemory.alloc_func(SrcSize, DstMaskPtr, fnName, "DstMaskPtr") == false || 
						 JetMemory.alloc_func(SrcSize, DstSpacePtr, fnName, "DstSpacePtr") == false )
					{
						JetMemory.free_func(DstMaskPtr);
						JetMemory.free_func(DstSpacePtr);
					}
					else
					{
						const int    OpenMPCnt = AOIDataCollect.GetOpenMPCount_Inspection();
						if ( ImageAPI.BuildSpaceData3(SrcW, SrcH, SrcStep, SrcSpacePtr, SrcMaskPtr, Mask2DPtr, OpenMPCnt, NoiseFilterParam, DstSpacePtr, DstMaskPtr) == false)
						{
							JetMemory.free_func(DstMaskPtr);
							JetMemory.free_func(DstSpacePtr);
						}
						else
						{
							JetMemory.free_func(SrcMaskPtr);
							JetMemory.free_func(SrcSpacePtr);
							ComponentUniFrame.MaskPtr = DstMaskPtr;
							ComponentUniFrame.SpacePtr = DstSpacePtr;
						}
					}
				}
				ComponentUniFrameList.push_back(ComponentUniFrame);				
			}
			double   SpaceRatio = GetProjectSpaceToGrayRatio();
			ImageAPI.SaveUniFrameImage(str, ComponentUniFrameList, true, bEnhance, bSave3D, bAppend, SpaceRatio);
			JetAPI::ClearUniFrameList(ComponentUniFrameList);
		}	
	}
	UnlockProject();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::BuildProjectDefectComponentList()//建立專案瑕疵零件列表	
{
	bool IsOK = true;
	LockProject();
	IsOK = BuildProjectDefectComponentListKernel();
#ifdef OFFLINE_VERSION
	//AddProjectResultCpkList_Lane(GetProjectActLaneID());//離線版本測試用
#endif//OFFLINE_VERSION
	UnlockProject();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::BuildProjectDefectComponentListKernel()//建立專案瑕疵零件列表	
{
	size_t          i=0, j=0;	
	unsigned int    Index=0;
	size_t          FdNgCount=0;
	size_t          PanelBoardCount=0;
	size_t          BoardComponentCount=0;			
	bool            IsBadBoard=false;
	bool            IsBadBoard_Alarm=false;
	bool            IsBadPanel=false;
	bool            IsBadPanel_Alarm=false;	
	TResultCnt      ResultCount;
	TTestResult     TestResult; 
	TTestResult     AlarmResult; 
	RESULT_ID       ResultID = RESULT_ID_NONE;
	RESULT_ID       ResultID_Alarm = RESULT_ID_NONE;	
	CAOIFd         *FdPtr = NULL;
	CAOIMark       *MarkPtr = NULL;
	CAOIPanel      *PanelPtr = NULL;
	CAOIBoard      *BoardPtr = NULL;
	CAOIBarcode    *BarcodePtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;	
	const LANE_ID   LaneID = GetProjectActLaneID();
	const size_t    FdCount = GetProjectFdCount();
	const bool      FdException =  GetProjectFdException();
	const size_t    PanelCount = GetProjectPanelCount();
	const size_t    BoardCount = GetProjectBoardCount();		
	std::vector<CAOIComponent*> ProjectDefectComponentPtrList_Alarm;//專案瑕疵零件指標列表
	
	switch ( LaneID )
	{
	case LANE_ID_A:	ClearProjectDefectComponentPtrList_LA_Inline();	break;
	case LANE_ID_B:	ClearProjectDefectComponentPtrList_LB_Inline();	break;
	}
	ClearProjectDefectComponentPtrList_Inline();
	ClearProjectDefectComponentPtrList_All_Inline();

	//要取回時間
	TestResult = GetProjectResultCurrent();
	AlarmResult = GetProjectResultAlarm();
	//CTime       sDateTime;//檢測開始日期
	
	
	//清除上一次拋件
	//DeleteProjectComponentType(COMPONENT_TYPE_SCRATCH);
	//DeleteProjectComponentType(COMPONENT_TYPE_DROP_OUT);
	//DeleteProjectComponentType(COMPONENT_TYPE_TEMPORARY);	

	//合併出拋件列表
	if ( BuildProjectDropOutComponentList() == false )
	{	return false; }
	if ( BuildProjectScratchComponentList() == false )
	{	return false; }
	if ( BuildProjectDimensionComponentList() == false )
	{	return false; }

	//放入這一次的拋件		
	if ( AddProjectSpecTestComonentList(m_ProjectDropOutPartList) == false )
	{	return false; }	

	//放入這一次的刮傷
	if ( AddProjectSpecTestComonentList(m_ProjectScratchPartList) == false )
	{	return false; }

	//套用這一次的尺寸
	if ( ApplyProjectDimensionComponentList() == false )
	{	return false; }
	if ( ClearProjectAllPartDimension() == false )
	{	return false; }
	
	//確認專案特殊檢測視野瑕疵零件-整板零件
	if ( CheckProjectSpecTestFieldDefectPart() == false )
	{	return false;	}	

	//Fd 
	FdNgCount = 0;
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		ResultID = FdPtr->GetFdResultID_AOI();
		ResultID_Alarm = FdPtr->GetFdResultID_Alarm();
		FdPtr->UpdateFdResultID_AOI_Lane(LaneID);
		if ( RESULT_ID_NONE==ResultID || RESULT_ID_OK==ResultID || RESULT_ID_BYPASS==ResultID ||RESULT_ID_SKIP==ResultID)
		{ continue;		}		
		FdNgCount ++;
	}
	//Mark
	const size_t    MarkCount = GetProjectMarkCount();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }		
		MarkPtr->UpdateMarkResultID_AOI_Lane(LaneID);		
	}
	//Barcode
	const size_t    BarcodeCount = GetProjectBarcodeCount();		
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }		
		BarcodePtr->UpdateBarcodeResultID_AOI_Lane(LaneID);		
	}
	//Component	
	const size_t    ComponentCount = GetProjectComponentCount_Inline();		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }
		if ( ComponentPtr->CheckComponentType_FullMap() == true )
		{
			if ( false == FdException )
			{
				if ( ComponentPtr->CheckComponentResultID_AOI() == RESULT_ID_NG )
				{	AddProjectDefectComponentPtr_All_Inline(ComponentPtr); }
			}
			continue; 
		}

		TestResult.sComponent.Total ++;
		AlarmResult.sComponent.Total ++;
		if ( true == FdException )
		{	continue;	}
		ResultID = ComponentPtr->CheckComponentResultID_AOI();
		ComponentPtr->UpdateComponentResultID_AOI_Lane(LaneID);
		ComponentPtr->UpdateComponentResultOffset_Lane(LaneID);
		if ( RESULT_ID_NONE == ResultID )
		{ 
			TestResult.sComponent.Bypass ++;
			AlarmResult.sComponent.Bypass ++;			
			continue; 
		}
		if ( RESULT_ID_OK == ResultID ) 
		{
			TestResult.sComponent.OK ++;
			AlarmResult.sComponent.OK ++;
			continue; 
		}
		if ( RESULT_ID_BYPASS==ResultID || RESULT_ID_SKIP==ResultID) 
		{ 
			TestResult.sComponent.Bypass ++;
			AlarmResult.sComponent.Bypass ++;
			continue; 
		}
		
		TestResult.sComponent.NG ++;
		AddProjectDefectComponentPtr_Inline(ComponentPtr);
		AddProjectDefectComponentPtr_All_Inline(ComponentPtr);	

		if ( ComponentPtr->GetComponentDefectAlarmResultOnAOI() == true )
		{
			AlarmResult.sComponent.NG ++;		
			ProjectDefectComponentPtrList_Alarm.push_back(ComponentPtr);
		}		
	}

	//Board Bypass
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
		TestResult.sBoard.Total ++;
		AlarmResult.sBoard.Total ++;
		if ( true == FdException )
		{	continue;	}
		ResultID = BoardPtr->GetBoardResultID();
		ResultID_Alarm = BoardPtr->GetBoardResultID_Alarm();
		if ( BoardPtr->GetBoardBypassed()==true || RESULT_ID_BYPASS==ResultID || RESULT_ID_SKIP==ResultID )
		{
			TestResult.sBoard.Bypass ++;
			AlarmResult.sBoard.Bypass ++;
			BoardPtr->UpdateBoardResultID_AOI_Lane(LaneID);
			continue;
		}

		IsBadBoard = false;
		IsBadBoard_Alarm = false;
		ResultID = RESULT_ID_OK;
		ResultCount.ResetCount();
		BoardPtr->SetBoardResultID(ResultID);
		BoardPtr->SetBoardResultID_Alarm(ResultID);
		BoardComponentCount = BoardPtr->GetBoardComponentCount();
		for ( j=0; j<BoardComponentCount; j++ )
		{
			ComponentPtr = BoardPtr->GetBoardComponentPtr(j, false);
			if ( NULL == ComponentPtr ) { continue; }
			if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
			if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }
			if ( ComponentPtr->CheckComponentModelEnabled() == false )	{ continue; }

			ResultID = ComponentPtr->CheckComponentResultID_AOI();
			ResultCount.AddCount(ResultID);
			if ( RESULT_ID_NONE == ResultID ) { continue; }
			if ( RESULT_ID_OK == ResultID ) { continue; }
			if ( RESULT_ID_SKIP == ResultID ) { continue; }
			if ( RESULT_ID_BYPASS == ResultID ) { continue; }

			IsBadBoard = true;			
			if ( ComponentPtr->GetComponentDefectAlarmResultOnAOI() == true )
			{	
				IsBadBoard_Alarm = true; 
				break;
			}
		}

		ResultID = ResultCount.CheckResultID();
		BoardPtr->SetBoardResultID(ResultID);
		if ( true == IsBadBoard )
		{
			TestResult.sBoard.NG ++;
		}
		else
		{	TestResult.sBoard.OK ++;	}
		BoardPtr->UpdateBoardResultID_AOI_Lane(LaneID);

		if ( true == IsBadBoard_Alarm )
		{	
			AlarmResult.sBoard.NG ++;	
			BoardPtr->SetBoardResultID_Alarm(RESULT_ID_NG);
		}
		else
		{	AlarmResult.sBoard.OK ++;	}
	}

	//Panel Bypass
	for ( i=0; i<PanelCount; i++ )
	{
		PanelPtr = GetProjectPanelPtr(i, false);
		if ( NULL == PanelPtr ) { continue; }
		if ( PanelPtr->GetPanelDeleted() == true ) { continue; }
		TestResult.sPanel.Total ++;
		AlarmResult.sPanel.Total ++;
		if ( true == FdException )
		{	continue;	}
		ResultID = PanelPtr->GetPanelResultID();
		ResultID_Alarm = PanelPtr->GetPanelResultID_Alarm();
		if ( PanelPtr->GetPanelBypassed()==true || RESULT_ID_BYPASS==ResultID || RESULT_ID_SKIP==ResultID )
		{
			TestResult.sPanel.Bypass ++;
			AlarmResult.sPanel.Bypass ++;
			PanelPtr->UpdatePanelResultID_AOI_Lane(LaneID);
			continue;
		}

		IsBadPanel = false;
		IsBadPanel_Alarm = false;
		ResultCount.ResetCount();
		PanelPtr->SetPanelResultID(RESULT_ID_OK);		
		PanelPtr->SetPanelResultID_Alarm(RESULT_ID_OK);		
		PanelBoardCount = PanelPtr->GetPanelBoardCount();
		for ( j=0; j<PanelBoardCount; j++ )
		{
			BoardPtr = PanelPtr->GetPanelBoardPtr(j, false);
			if ( NULL == BoardPtr ) { continue; }
			if ( BoardPtr->GetBoardDeleted() == true ) { continue; }
			ResultID = BoardPtr->GetBoardResultID();
			ResultID_Alarm = BoardPtr->GetBoardResultID_Alarm();
			ResultCount.AddCount(ResultID);
			if ( RESULT_ID_NONE == ResultID ) { continue; }
			if ( RESULT_ID_OK == ResultID ) { continue; }
			if ( RESULT_ID_SKIP == ResultID ) { continue; }
			if ( RESULT_ID_BYPASS == ResultID ) { continue; }

			IsBadPanel = true;			
			if ( RESULT_ID_NG==ResultID_Alarm || RESULT_ID_EXCEPTION==ResultID_Alarm )
			{	IsBadPanel_Alarm = true; }			
		}

		ResultID = ResultCount.CheckResultID();
		PanelPtr->SetPanelResultID(ResultID);
		if ( true == IsBadPanel )
		{	TestResult.sPanel.NG ++;	}
		else
		{	TestResult.sPanel.OK ++;	}
		PanelPtr->UpdatePanelResultID_AOI_Lane(LaneID);

		if ( true == IsBadPanel_Alarm )
		{
			AlarmResult.sPanel.NG ++;		
			PanelPtr->SetPanelResultID_Alarm(RESULT_ID_NG);
		}
		else
		{	AlarmResult.sPanel.OK ++;	}
	}			
	
	SortProjectDefectComponentListByName();
	switch ( LaneID )
	{
	case LANE_ID_A:	SetProjectDefectComponentList_LA(m_ProjectDefectComponentPtrList);	break;
	case LANE_ID_B:	SetProjectDefectComponentList_LB(m_ProjectDefectComponentPtrList);	break;
	}
	const size_t DefectComponentCount = GetProjectDefectComponentCount_Inline();
	const size_t DefectComponentCount_Alarm = ProjectDefectComponentPtrList_Alarm.size();
	TestResult.sTest.Total ++;
	AlarmResult.sTest.Total ++;	
	if ( 0 == DefectComponentCount )
	{	TestResult.sTest.OK ++;	}
	else
	{	TestResult.sTest.NG ++;	}	

	if ( 0 == DefectComponentCount_Alarm )
	{	AlarmResult.sTest.OK ++;	}
	else
	{	AlarmResult.sTest.NG ++;	}	
	
	//需考慮到定位點異常	
	if ( false == FdException )
	{
		if ( 0 == TestResult.sTest.NG )
		{	TestResult.sResultID = TEST_RESULT_OK;	}
		else
		{	TestResult.sResultID = TEST_RESULT_NG;	}

		if ( 0 == AlarmResult.sTest.NG )
		{	AlarmResult.sResultID = TEST_RESULT_OK;	}
		else
		{	AlarmResult.sResultID = TEST_RESULT_NG;	}	
	}
	else
	{
		TestResult.sResultID = TEST_RESULT_FD;
		AlarmResult.sResultID = TEST_RESULT_FD;
	}	

	TestResult.sLaneID = LaneID;
	AlarmResult.sLaneID = LaneID;	

	DWORD CTStamp=0;
	DWORD LastStamp = m_ProjectTickCountStamp;
	DWORD CurrentStamp = ::GetTickCount();
	if ( 0!=LastStamp && CurrentStamp>LastStamp )
	{	CTStamp = CurrentStamp-LastStamp;	}
	SetProjectTickCountStamp(CurrentStamp);
	CalcProjectTestTime(TestResult.sTestTime, CTStamp);	
	SetProjectResultAlarm(AlarmResult);
	SetProjectResultLatest(TestResult);	
	SetProjectResultCurrent(TestResult);
	SetProjectResultLatest_Lane(LaneID, TestResult);
	ProjectDefectComponentPtrList_Alarm.clear();

	ComponentPtr = GetProjectDefectComponentPtr(0, true);
	if ( NULL != ComponentPtr )
	{	SetProjectActiveComponent(ComponentPtr); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::SortProjectDefectComponentListByName()//排序專案瑕疵零件列表-根據名稱
{	
	std::vector<CAOIComponent*> DefectComponentList=m_ProjectDefectComponentPtrList;
	const size_t DefectComponentCount=DefectComponentList.size();	
	if ( 0 == DefectComponentCount ) { return true; }

	CSortObj       SortObj;	
	SORT_MODE      SortMode=SORT_BY_TXT;
	std::vector<CSortObj> SortList(DefectComponentCount);
	
	size_t         i=0;
	CString        ComponentName;	
	CAOIComponent *ComponentPtr=NULL;

	SortObj.SetSortMode(SortMode);
	for ( i=0; i<DefectComponentCount; i++ )
	{
		ComponentPtr = DefectComponentList[i];
		if ( NULL == ComponentPtr ) { continue; }
		ComponentName = ComponentPtr->GetComponentName();
		SortList[i].SetID(i);
		SortList[i].SetSortMode(SortMode);
		SortList[i].SetPtr(ComponentPtr);
		SortList[i].SetValueStr(ComponentName);			
	}

	m_ProjectDefectComponentPtrList.clear();
	std::sort(SortList.begin(), SortList.end());
	for ( i=0; i<DefectComponentCount; i++ )
	{
		ComponentPtr = (CAOIComponent*)(SortList[i].GetPtr());
		m_ProjectDefectComponentPtrList.push_back(ComponentPtr);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectDefectComponentCount() const//取得專案瑕疵零件數量	
{
	return GetProjectDefectComponentCount_Inline();
}
//-------------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectDefectComponentCount(DISTRICT_ID DistrictID) const//取得專案瑕疵零件數量	
{
	size_t Count=0;
	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount=GetProjectDefectComponentCount();
	for ( size_t i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectDefectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDistrictID() != DistrictID ) { continue; }
		Count ++;
	}
	return Count;
}
//-------------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectDefectComponentPtr(size_t index, bool bCheck) const//取得專案瑕疵零件指標	
{
	if ( true == bCheck )
	{
		const size_t Size = m_ProjectDefectComponentPtrList.size();
		if ( index >= Size ) 
		{	return NULL; }
	}
	return GetProjectDefectComponentPtr_Inline(index);
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::RemoveProjectDefectComponentSelected()//移除專案零件選取到的指標
{
	size_t i = 0;
	int    index = 0;
	CAOIComponent*  ComponentPtr = NULL;
	std::vector<CAOIComponent*> ProjectDefectPtrList = this->m_ProjectDefectComponentPtrList;
	const size_t ComponentCount = ProjectDefectPtrList.size();

	index = 0;
	ClearProjectDefectComponentPtrList_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectDefectPtrList[i];
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == true ) { continue; }
		AddProjectDefectComponentPtr_Inline(ComponentPtr);
		index ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIProject::ClearProjectAllDefectComponents()//移除專案瑕疵零件列表
{
	ClearProjectDefectComponentPtrList_Inline();
	ClearProjectDefectComponentPtrList_LA_Inline();
	ClearProjectDefectComponentPtrList_LB_Inline();
	ClearProjectDefectComponentPtrList_All_Inline();
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::RemoveProjectComponentSelected(std::vector<CAOIComponent*> &ComponentList)//移除專案零件選取到的指標
{
	size_t i = 0;	
	CAOIComponent*  ComponentPtr = NULL;
	std::vector<CAOIComponent*> ProjectDefectPtrList = ComponentList;
	const size_t ComponentCount = ProjectDefectPtrList.size();

	ComponentList.clear();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectDefectPtrList[i];
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == true ) { continue; }
		ComponentList.push_back(ComponentPtr);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectComponentList(std::vector<CAOIComponent*> &ComponentList, bool val)//選取全部零件選取到的指標
{
	const size_t Count=ComponentList.size();
	for ( size_t i=0; i<Count; i++ )
	{
		CAOIComponent *ComponentPtr=ComponentList[i];
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentSelected(val);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectDefectComponentCount_Lane(LANE_ID LaneID) const//取得專案瑕疵零件數量	
{
	size_t Count=0;
	switch ( LaneID )
	{
	case LANE_ID_A:	Count = GetProjectDefectComponentCount_LA_Inline();	break;
	case LANE_ID_B:	Count = GetProjectDefectComponentCount_LB_Inline();	break;
	}
	return Count;
}
//-------------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectDefectComponentPtr_Lane(LANE_ID LaneID, size_t index, bool bCheck) const//取得專案瑕疵零件指標	
{
	CAOIComponent *ComponentPtr=NULL;
	switch ( LaneID )
	{
	case LANE_ID_A:	ComponentPtr = GetProjectDefectComponentPtr_LA(index, bCheck);	break;
	case LANE_ID_B:	ComponentPtr = GetProjectDefectComponentPtr_LB(index, bCheck);	break;
	}
	return ComponentPtr;
}
//-------------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectDefectComponentCount_LA() const//取得專案瑕疵零件數量	
{
	return GetProjectDefectComponentCount_LA_Inline();
}
//-------------------------------------------------------------------------------------//
CAOIComponent*  CAOIProject::GetProjectDefectComponentPtr_LA(size_t index, bool bCheck) const//取得專案瑕疵零件指標	
{
	if ( true == bCheck )
	{
		const size_t Size = m_ProjectDefectComponentPtrList_LA.size();
		if ( index >= Size ) 
		{	return NULL; }
	}
	return GetProjectDefectComponentPtr_LA_Inline(index);
}
//-------------------------------------------------------------------------------------//
bool  CAOIProject::RemoveProjectDefectComponentSelected_LA()//移除專案零件選取到的指標
{
	return RemoveProjectComponentSelected(m_ProjectDefectComponentPtrList_LA);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIProject::SetProjectDefectComponentList_LA(const std::vector<CAOIComponent*> &ComponentList)//設定專案瑕疵零件列表
{
	m_ProjectDefectComponentPtrList_LA = ComponentList;
}
//-------------------------------------------------------------------------------------//
size_t  CAOIProject::GetProjectDefectComponentCount_LB() const//取得專案瑕疵零件數量	
{
	return GetProjectDefectComponentCount_LB_Inline();
}
//-------------------------------------------------------------------------------------//
CAOIComponent* CAOIProject::GetProjectDefectComponentPtr_LB(size_t index, bool bCheck) const//取得專案瑕疵零件指標	
{
	if ( true == bCheck )
	{
		const size_t Size = m_ProjectDefectComponentPtrList_LB.size();
		if ( index >= Size ) 
		{	return NULL; }
	}
	return GetProjectDefectComponentPtr_LB_Inline(index);
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::RemoveProjectDefectComponentSelected_LB()//移除專案零件選取到的指標
{
	return RemoveProjectComponentSelected(m_ProjectDefectComponentPtrList_LB);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIProject::SetProjectDefectComponentList_LB(const std::vector<CAOIComponent*> &ComponentList)//設定專案瑕疵零件列表
{
	m_ProjectDefectComponentPtrList_LB = ComponentList;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::RemoveProjectDefectComponentSelected_All()//移除專案零件選取到的指標
{
	return RemoveProjectComponentSelected(m_ProjectDefectComponentPtrList_All);	
}
//-------------------------------------------------------------------------------------//
void CAOIProject::SelectProjectDefectComponentList_All(bool val)//選取專案瑕疵零件列表-全部
{		
	SelectProjectComponentList(m_ProjectDefectComponentPtrList_All, val);
}
//-------------------------------------------------------------------------------------//
