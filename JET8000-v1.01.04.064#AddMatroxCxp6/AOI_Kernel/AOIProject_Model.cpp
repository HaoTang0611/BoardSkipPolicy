// AOIProject_Model.cpp: implementation of the CAOIProject class.
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
inline size_t CAOIProject::GetProjectModelCount_Inline() const//取得專案模組數量
{
	return CAOIProject::m_ProjectModelPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::AddProjectModelPtr_Inline(CAOIModel *ModelPtr)//增加專案模組
{
	CAOIProject::m_ProjectModelPtrList.push_back(ModelPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIModel* CAOIProject::GetProjectModelPtr_Inline(size_t index) const//取得專案模組指標	
{	
	return m_ProjectModelPtrList[index];
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::SetProjectModelSelectedSaveLeadReport(bool bSaved)//設定專案所有模組存儲引腳報告
{
	CAOIModel *ModelPtr=NULL;
	const size_t ModelCount=GetProjectModelCount();
	for ( size_t i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr(i, false);
		if ( NULL == ModelPtr ) { continue; }
		if ( ModelPtr->GetModelSelected() == false ) { continue; }
		ModelPtr->SetModelSaveLeadReport(bSaved);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectModelSelectedDefectItemEssential(const CWndDefectItem &DefectItem)//設定專案選取到模組瑕疵必要項目
{
	CAOIModel *ModelPtr=NULL;
	CWndDefectItem WndDefectItemOld;
	const size_t ModelCount=GetProjectModelCount();
	for ( size_t i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr(i, false);
		if ( NULL == ModelPtr ) { continue; }
		if ( ModelPtr->GetModelSelected() == false ) { continue; }		
		WndDefectItemOld=ModelPtr->GetModelDefectItemEssential(); 
		ModelPtr->SetModelDefectItemEssential(DefectItem);	
		LogOperCtrl.SaveLogModelWndDefectItemCompare(ModelPtr, _T("Test Essential"), WndDefectItemOld, DefectItem); 
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SetProjectModelSelectedDefectItemRecheck_ARS(const CWndDefectItem &DefectItem)//設定專案選取到模組瑕疵重複確認-ARS
{
	CAOIModel *ModelPtr=NULL;
	CWndDefectItem WndDefectItemOld;
	const size_t ModelCount=GetProjectModelCount();
	for ( size_t i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr(i, false);
		if ( NULL == ModelPtr ) { continue; }
		if ( ModelPtr->GetModelSelected() == false ) { continue; }
		WndDefectItemOld=ModelPtr->GetModelDefectItemRecheck_ARS(); 
		ModelPtr->SetModelDefectItemRecheck_ARS(DefectItem);
		LogOperCtrl.SaveLogModelWndDefectItemCompare(ModelPtr, _T("Defect Recheck (ARS)"), WndDefectItemOld, DefectItem); 
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ApplyProjectModelToComponents(LPCTSTR ModelName)//更新模組至零件上
{
	CAOIModel *ModelPtr = CAOIProject::GetProjectModelPtrByModelName(ModelName);	
	if ( ApplyProjectModelToComponents(ModelPtr) == false )
	{	return false; }	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ApplyProjectModelToComponents(CAOIModel *ModelPtr)//更新模組至選零件上
{
	if ( NULL == ModelPtr ) 
	{
		m_ErrorString = _T("Error, No Library Model Ptr");
		return false; 
	}

	bool      bSucc=true;
	CString   ModelNameM;
	CString   ModelName = ModelPtr->GetModelName();			
	const int OpenMPCnt = GetProjectEditOpenMpCount();
	const int ComponentCount = (int)(GetProjectComponentCount());
	const int OpenMPCntUsd=MIN(ComponentCount, OpenMPCnt);

	ModelNameM = ModelName;
	ModelNameM.MakeUpper();
	AOIObjManager.LayoutRecycleObjList(OpenMPCntUsd);

#ifdef OPEN_MP_USE
	#pragma omp parallel for num_threads(OpenMPCntUsd)
#endif//OPEN_MP_USE	
	for ( int i=0; i<ComponentCount; i++ )
	{
		if ( false == bSucc ) { break; }
		if ( i >= ComponentCount ) { continue; }
		CAOIComponent *ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }		
		if ( ComponentPtr->GetComponentModelIsolated() == true ) { continue; }
		CString ModelNameC = ComponentPtr->GetComponentModelName();
		if ( ModelNameC.CompareNoCase(ModelNameM) != 0 ) { continue; }
		if ( ComponentPtr->UpdateComponentModelFromLibrary(ModelPtr) == false ) 
		{			
			LockProject();
			if ( false == bSucc )
			{
				UnlockProject();
				break;
			}
			bSucc = false;
			CString FullName = ComponentPtr->GetComponentFullName();
			this->m_ErrorString.Format(_T("Error, Update Component Model From Library Fault[%s -> %s]"), FullName, ModelName);
			UnlockProject();
			break;
		}
	}	
	AOIObjManager.LayoutRecycleObjList();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ApplyProjectModelToComponentsSelected(LPCTSTR ModelName)//更新模組至選取到的零件上
{
	CAOIModel *ModelPtr = CAOIProject::GetProjectModelPtrByModelName(ModelName);	
	if ( ApplyProjectModelToComponentsSelected(ModelPtr) == false )
	{	return false; }	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ApplyProjectModelToComponentsSelected(CAOIModel *ModelPtr)//更新模組至選取到的零件上
{
	if ( NULL == ModelPtr ) 
	{
		m_ErrorString = _T("Error, No Library Model Ptr");
		return false; 
	}

	bool      bSucc=true;
	CString   ModelNameM;
	CString   ModelName = ModelPtr->GetModelName();	
	const int OpenMPCnt = GetProjectEditOpenMpCount();
	const int ComponentCount = (int)(GetProjectComponentCount());
	const int OpenMPCntUsd=MIN(ComponentCount, OpenMPCnt);

	bSucc=true;
	ModelNameM = ModelName;
	ModelNameM.MakeUpper();
	AOIObjManager.LayoutRecycleObjList(OpenMPCntUsd);

#ifdef OPEN_MP_USE
	#pragma omp parallel for num_threads(OpenMPCntUsd)
#endif//OPEN_MP_USE	
	for ( int i=0; i<ComponentCount; i++ )
	{
		if ( false == bSucc ) { break; }
		if ( i >= ComponentCount ) { continue; }
		CAOIComponent *ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		if ( ComponentPtr->GetComponentModelIsolated() == true ) { continue; }

		if ( ComponentPtr->UpdateComponentModelFromLibrary(ModelPtr) == false ) 
		{		
			LockProject();
			if ( false == bSucc )
			{
				UnlockProject();
				break;
			}
			bSucc = false;
			CString FullName = ComponentPtr->GetComponentFullName();
			this->m_ErrorString.Format(_T("Error, Update Component Model From Library Fault[%s -> %s]"), FullName, ModelName);
			UnlockProject();
			break;
		}
	}	
	AOIObjManager.LayoutRecycleObjList();
	if ( false == bSucc )
	{	return false; }
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectModelValid(const CAOIModel *RefModelPtr)//確認專案的模組指標有效
{
	if ( NULL == RefModelPtr ) { return false; }
	size_t         i = 0;	
	CAOIModel     *ModelPtr = NULL;	
	const size_t   ModelCount = GetProjectModelCount_Inline();			
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		if ( RefModelPtr == ModelPtr ) 
		{	return true; }
	}
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::GetProjectModelNameList(std::vector<CString> &ModelNameList)//取得專案模組名稱列表
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPtr(ProjectPtr) == false )
	{	return false; }

	size_t       i=0;
	CString      ModelName;
	CAOIModel   *ModelPtr = NULL;
	const size_t ModelCount = ProjectPtr->GetProjectModelCount_Inline();
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = ProjectPtr->GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		ModelName = ModelPtr->GetModelName();
		ModelNameList.push_back(ModelName);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::GetProjectModelInfoList(std::vector<TModelInfo> &ModelInfoList)//取得專案模組資訊列表
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPtr(ProjectPtr) == false )
	{	return false; }

	size_t       i=0;
	TModelInfo   ModelInfo;
	CAOIModel   *ModelPtr = NULL;
	const size_t ModelCount = ProjectPtr->GetProjectModelCount_Inline();
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = ProjectPtr->GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		ModelPtr->CloneModelInfo(ModelInfo);
		ModelInfoList.push_back(ModelInfo);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ActiveProjectComponentModelWnd(LPCTSTR ModelName, size_t WndIdx)//選取專案模組視窗
{
	size_t         i = 0;	
	CString        strModelName;
	CAOIWnd       *WndPtr = NULL;
	CAOIModel     *ModelPtr = NULL;	
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ComponentCount = GetProjectComponentCount();			
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		strModelName = ModelPtr->GetModelName();
		if ( strModelName.CompareNoCase(ModelName) != 0 ) { continue; }

		WndPtr = ModelPtr->GetModelWndPtr(WndIdx, true);
		if ( NULL == WndPtr ) { continue; }
		//ModelPtr->UnSelectModel();
		//WndPtr->SetWndSelected(true);
		ModelPtr->SetModelWndActived(WndPtr);		
	}
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ActiveProjectComponentModelWnd(LPCTSTR ModelName, CAOIModel *RefModelPtr, size_t WndIdx)//選取專案模組視窗
{
	size_t         i = 0;	
	CString        strModelName;
	CAOIWnd       *WndPtr = NULL;
	CAOIModel     *ModelPtr = NULL;	
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ComponentCount = GetProjectComponentCount();			
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		if ( ModelPtr == RefModelPtr ) { continue; }
		strModelName = ModelPtr->GetModelName();
		if ( strModelName.CompareNoCase(ModelName) != 0 ) { continue; }

		WndPtr = ModelPtr->GetModelWndPtr(WndIdx, true);
		if ( NULL == WndPtr ) { continue; }
		//ModelPtr->UnSelectModel();
		//WndPtr->SetWndSelected(true);
		ModelPtr->SetModelWndActived(WndPtr);		
	}
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ModifyProjectModelDefaultWndParam(TMODEL_DEFAULT_WND_PARAM &Param)//修正模組預設檢測框參數
{
	CAOIProject *ProjectPtr = this;
	unsigned int FrameIndex = 0;
	unsigned int FrameUniqueID = 0;
	unsigned int DefaultFrameIndex = 0;
	unsigned int DefaultFrameUniqueID = 0;
	TFrameParam *FramePaamPtr = NULL;	
	std::vector<unsigned int>   FrameIndexMapList;//專案燈源映射列表		
	if ( ProjectPtr->BuildProjectFrameIndexMapParam(FrameIndexMapList, DefaultFrameIndex, DefaultFrameUniqueID) == false ) 
	{	return false;	}	

	Param.nDefaultFrameIndex = DefaultFrameIndex;
	Param.nDefaultFrameUniqueID = DefaultFrameUniqueID;
	Param.FrameIndexMapList = FrameIndexMapList;

	//對位光源	
	FrameUniqueID = Param.nFrameUniqueID_Align;
	FrameIndex = ProjectPtr->GetProjectFrameIndexByUniqueID(FrameUniqueID);
	FramePaamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
	if ( -1==FrameIndex || NULL==FramePaamPtr )
	{
		Param.nFrameIndex_Align = DefaultFrameIndex;
		Param.nFrameUniqueID_Align = DefaultFrameUniqueID;	
	}
	else
	{	Param.nFrameIndex_Align = FrameIndex;	}	

	//文字光源
	FrameUniqueID = Param.nFrameUniqueID_Text;
	FrameIndex = ProjectPtr->GetProjectFrameIndexByUniqueID(FrameUniqueID);
	FramePaamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
	if ( -1==FrameIndex || NULL==FramePaamPtr )
	{
		Param.nFrameIndex_Text = DefaultFrameIndex;
		Param.nFrameUniqueID_Text = DefaultFrameUniqueID;	
	}
	else
	{	Param.nFrameIndex_Text = FrameIndex;	}	

	//低角度燈源
	FrameUniqueID = Param.nFrameUniqueID_Low;
	FrameIndex = ProjectPtr->GetProjectFrameIndexByUniqueID(FrameUniqueID);
	FramePaamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
	if ( -1==FrameIndex || NULL==FramePaamPtr )
	{
		Param.nFrameIndex_Low = DefaultFrameIndex;
		Param.nFrameUniqueID_Low = DefaultFrameUniqueID;	
	}
	else
	{	Param.nFrameIndex_Low = FrameIndex;	}	

	//高角度燈源
	FrameUniqueID = Param.nFrameUniqueID_High;
	FrameIndex = ProjectPtr->GetProjectFrameIndexByUniqueID(FrameUniqueID);
	FramePaamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
	if ( -1==FrameIndex || NULL==FramePaamPtr )
	{
		Param.nFrameIndex_High = DefaultFrameIndex;
		Param.nFrameUniqueID_High = DefaultFrameUniqueID;	
	}
	else
	{	Param.nFrameIndex_High = FrameIndex;	}	


	//焊錫光源
	FrameUniqueID = Param.nFrameUniqueID_Solder;
	FrameIndex = ProjectPtr->GetProjectFrameIndexByUniqueID(FrameUniqueID);
	FramePaamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
	if ( -1==FrameIndex || NULL==FramePaamPtr )
	{
		Param.nFrameIndex_Solder = DefaultFrameIndex;
		Param.nFrameUniqueID_Solder = DefaultFrameUniqueID;	
	}
	else
	{	Param.nFrameIndex_Solder = FrameIndex;	}

	//3D光源
	FrameUniqueID = Param.nFrameUniqueID_3D;
	FrameIndex = ProjectPtr->GetProjectFrameIndexByUniqueID(FrameUniqueID);
	FramePaamPtr = AOIDataCollect.GetSystemFrameParamPtrByUniqueID(FrameUniqueID);
	if ( -1==FrameIndex || NULL==FramePaamPtr )
	{
		Param.nFrameIndex_3D = DefaultFrameIndex;
		Param.nFrameUniqueID_3D = DefaultFrameUniqueID;	
	}
	else
	{	Param.nFrameIndex_3D = FrameIndex;	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::UpdateProjectModellToOtherModels(const CAOIModel *RefModelPtr, LPCTSTR KeyName, bool bIncluedUsed)//套用專案模組至其他模組內
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }
	if ( NULL==RefModelPtr || NULL==KeyName) { return false; }

	size_t         i=0,j=0;  
	MODEL_TYPE     ModelType;
	MODEL_TYPE     RefModelType;	
	std::wstring   TempGroupName=L"";
	std::wstring   TempModelName=L"";
	CString        ModelName;
	CString        ModelBKName;
	CString        ModelFolder;
	CString        RefModelBKName;
	CString        RefModelFolder;
	CString        LibraryFolder;
	CString        RefGroupName;
	CString        RefModelName;
	CAOIModel     *ModelPtr = NULL;
	CAOIModel     *ModelPtr_Cloned = NULL;	
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ModelCount = ProjectPtr->GetProjectModelCount_Inline();
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount();
	
	RefModelType = RefModelPtr->GetModelType();
	RefModelName = RefModelPtr->GetModelName();
	RefGroupName = RefModelPtr->GetModelGroupName();
	RefModelFolder = RefModelPtr->GetModelFolderModel();
	LibraryFolder = ProjectPtr->GetProjectLibraryFolder();

	JetAPI::TCHAR2wstring(RefGroupName, TempGroupName);

	//更新模組列表-已經建立的
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = ProjectPtr->GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		ModelType = ModelPtr->GetModelType();
		if ( true != bIncluedUsed )
		{
			if ( MODEL_TYPE_NULL != ModelType )
			{	continue; }
		}
		if ( ModelType != RefModelType ) { continue; }

		ModelName = ModelPtr->GetModelName();
		if ( ModelName.CompareNoCase(RefModelName) == 0 ) { continue; }
		ModelName.MakeUpper();
		if ( JetAPI::FindTextInString(KeyName, ModelName) == false ) 
		{	continue; }

		ModelPtr_Cloned = RefModelPtr->CloneModelObj();
		if ( NULL == ModelPtr_Cloned ) { continue; }

		JetAPI::TCHAR2wstring(ModelName, TempModelName);
		ModelFolder.Format(_T("%s\\%s"), LibraryFolder, ModelName);
		JetAPI::CreateFolder(ModelFolder);
		ModelPtr_Cloned->SetModelGroupName(TempGroupName.c_str());
		ModelPtr_Cloned->SetModelName(TempModelName.c_str());
		ModelPtr_Cloned->SetModelFolderModel(ModelFolder);
		
  		JetAPI::CopyFolderAToFolderB(RefModelFolder, ModelFolder, false, true, _T(""), -1, -1);
		//更改底圖名字
		for ( i=0; i<FRAME_MAX_COUNT; i++ )
		{	
			ModelBKName = ModelPtr_Cloned->GetModelBKImageFilename(i);
			RefModelName = RefModelPtr->GetModelBKImageFilename(i);			
			if ( RefModelName.CompareNoCase(ModelBKName) == 0 ) { continue; }			
			::CopyFile(RefModelName, ModelBKName, FALSE);			
		}
		ModelPtr_Cloned->SetModelIndex((unsigned int)i);
		ModelPtr_Cloned->UnSelectModel();
		ModelPtr_Cloned->AssignModelFolder();
		ModelPtr_Cloned->SetModelNeedSaveFiles(true);
		ModelPtr_Cloned->SetModelBodyBoxActived(true);				
		ModelPtr_Cloned->SetModelBKImageNeedToGrab(true);

		//清除舊的模組
		AOIObjManager.DestroyModelObj(m_ProjectModelPtrList[i]);
		m_ProjectModelPtrList[i] = ModelPtr_Cloned;
		ProjectPtr->SelectProjectAllComponents(false);
		if ( false == bIncluedUsed )
		{	ProjectPtr->SelectProjectComponentsByModelName(ModelName, true); }
		else
		{	ProjectPtr->SelectProjectComponentsByModelName(ModelName, false); }
		ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtr_Cloned);
		ModelPtr_Cloned = NULL;
	}

	//更新零件列表-尚未建立的
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }		
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		ModelType = ModelPtr->GetModelType();
		if ( MODEL_TYPE_NULL != ModelType )
		{	continue; }

		//尚未建立不管是否模組獨立
		//if ( ComponentPtr->GetComponentModelIsolated() == true ) { continue; }

		ModelName = ComponentPtr->GetComponentModelName();
		if ( ModelName.CompareNoCase(RefModelName) == 0 ) { continue; }
		ModelName.MakeUpper();
		if ( JetAPI::FindTextInString(KeyName, ModelName) == false ) 
		{	continue; }

		ModelPtr_Cloned = RefModelPtr->CloneModelObj();
		if ( NULL == ModelPtr_Cloned ) { continue; }

		JetAPI::TCHAR2wstring(ModelName, TempModelName);
		ModelFolder.Format(_T("%s\\%s"), LibraryFolder, ModelName);
		JetAPI::CreateFolder(ModelFolder);
		ModelPtr_Cloned->SetModelGroupName(TempGroupName.c_str());
		ModelPtr_Cloned->SetModelName(TempModelName.c_str());
		ModelPtr_Cloned->SetModelFolderModel(ModelFolder);
		
  		JetAPI::CopyFolderAToFolderB(RefModelFolder, ModelFolder, false, true, _T(""), -1, -1);
		//更改底圖名字
		for ( j=0; j<FRAME_MAX_COUNT; j++ )
		{	
			ModelBKName = ModelPtr_Cloned->GetModelBKImageFilename(j);
			RefModelName = RefModelPtr->GetModelBKImageFilename(j);			
			if ( RefModelName.CompareNoCase(ModelBKName) == 0 ) { continue; }			
			::CopyFile(RefModelName, ModelBKName, FALSE);			
		}		
		ModelPtr_Cloned->UnSelectModel();
		ModelPtr_Cloned->AssignModelFolder();
		ModelPtr_Cloned->SetModelNeedSaveFiles(true);
		ModelPtr_Cloned->SetModelBodyBoxActived(true);
		ModelPtr_Cloned->SetModelBKImageNeedToGrab(true);
		ProjectPtr->AddProjectModelPtr(ModelPtr_Cloned, false);
		
		ProjectPtr->SelectProjectAllComponents(false);
		ProjectPtr->SelectProjectComponentsByModelName(ModelName, true);
		ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtr_Cloned);
		ModelPtr_Cloned = NULL;
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ListProjectModelByGroupName(CAOIModel *ModelPtr, std::vector<CAOIModel*> &ModelList)//依群組名稱列出專案模組
{
	if ( NULL == ModelPtr ) { return true; }
	MODEL_TYPE ModelType = ModelPtr->GetModelType();
	CString GroupName = ModelPtr->GetModelGroupName();
	return ListProjectModelByGroupName(ModelType, GroupName, ModelList);
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ListProjectModelByGroupName(MODEL_TYPE ModelType, LPCTSTR GroupName, std::vector<CAOIModel*> &ModelList)//依群組名稱列出專案模組
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t       i=0;
	CString      GroupNameM;
	CAOIModel   *ModelPtr = NULL;
	const size_t ModelCount = ProjectPtr->GetProjectModelCount_Inline();
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = ProjectPtr->GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		if ( ModelPtr->GetModelType() != ModelType ) { continue; }
		GroupNameM = ModelPtr->GetModelGroupName();
		if ( GroupNameM.CompareNoCase(GroupName) != 0 ) { continue; }
		ModelList.push_back(ModelPtr);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::BuildProjectModelList_Saved()//建立專案存檔模組列表
{
	size_t       i=0;
	CAOIModel   *ModelPtr = NULL;
	const size_t ModelCount = GetProjectModelCount_Inline();

	ClearProjectModelList_Saved();
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		if ( ModelPtr->GetModelNeedSaveFiles() == false ) { continue; }
		AddProjectModelPtr_Saved(ModelPtr);
	}
	return true;
}
//---------------------------------------------------------------------------------//
void CAOIProject::ClearProjectModelList_Saved()//清除專案存檔模組列表
{
	m_ProjectModelPtrList_Saved.clear();
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectModelCount_Saved() const//取得專案存檔模組數量	
{
	return m_ProjectModelPtrList_Saved.size();
}
//---------------------------------------------------------------------------------//
void CAOIProject::AddProjectModelPtr_Saved(CAOIModel *ModelPtr)//增加專案存檔模組
{
	m_ProjectModelPtrList_Saved.push_back(ModelPtr);	
}
//---------------------------------------------------------------------------------//
CAOIModel* CAOIProject::GetProjectModelPtr_Saved(size_t index, bool check) const//取得專案存檔模組指標
{
	if ( true == check )
	{
		const size_t Count = m_ProjectModelPtrList_Saved.size();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return m_ProjectModelPtrList_Saved[index];
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CopyProjectModelFolderTo(LPCTSTR DstFolder)//複製專案
{
	size_t       i=0;	
	CString      FolderLoc;
	CString      FolderSrc;	
	CString      FolderDst;		
	CAOIModel   *ModelPtr = NULL;
	const bool   bDeleteDst = true;	
	const bool   bDeleteSrc = false;	
	const size_t ModelCount = GetProjectModelCount_Saved();
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Saved(i, false);
		if ( NULL == ModelPtr )  { continue; }		
		FolderSrc = ModelPtr->GetModelFolder();
		JetAPI::ExtractTopFolder(FolderSrc, FolderLoc);
		FolderDst.Format(_T("%s\\%s"), DstFolder, FolderLoc);
		if ( JetAPI::CopyFolderAToFolderB(FolderSrc, FolderDst, bDeleteSrc, bDeleteDst, _T(""), -1, -1) == false )
		{	continue;	}
		ModelPtr->SetModelNeedSaveFiles(false);
	}
	
	//刪除多的資料夾
	CString FolderName;
	std::vector<CString> FolderList;
	JetAPI::ListFolder(DstFolder, FolderList);
	const size_t FolderCount = FolderList.size();
	for ( i=0; i<FolderCount; i++ )
	{
		FolderLoc = FolderList[i];
		ModelPtr = GetProjectModelPtrByModelName(FolderLoc);
		if ( NULL != ModelPtr ) { continue; }
		FolderDst.Format(_T("%s\\%s"), DstFolder, FolderLoc);
		JetAPI::RemoveFolder(FolderDst);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::BuildProjectPartModelList_Saved()//建立專案零件存檔模組列表
{
	size_t         i=0;
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ComponentCount = GetProjectComponentCount();

	ClearProjectPartModelList_Saved();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentModelIsolated() == false ) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		if ( ModelPtr->GetModelNeedSaveFiles() == false ) { continue; }
		AddProjectPartModelPtr_Saved(ModelPtr);
	}
	return true;
}
//---------------------------------------------------------------------------------//
void CAOIProject::ClearProjectPartModelList_Saved()//清除專案零件存檔模組列表
{
	m_ProjectPartModelPtrList_Saved.clear();
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectPartModelCount_Saved() const//取得專案零件存檔模組數量	
{
	return m_ProjectPartModelPtrList_Saved.size();
}
//---------------------------------------------------------------------------------//
void CAOIProject::AddProjectPartModelPtr_Saved(CAOIModel *ModelPtr)//增加專案零件存檔模組	
{
	m_ProjectPartModelPtrList_Saved.push_back(ModelPtr);
}
//---------------------------------------------------------------------------------//
CAOIModel* CAOIProject::GetProjectPartModelPtr_Saved(size_t index, bool check) const//取得專案零件存檔模組指標
{
	if ( true == check )
	{
		const size_t Count = m_ProjectPartModelPtrList_Saved.size();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return m_ProjectPartModelPtrList_Saved[index];
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CopyProjectPartModelFolderTo(LPCTSTR DstFolder)//複製專案
{
	size_t       i=0,j=0;	
	CString      FolderLoc;
	CString      FolderSrc;	
	CString      FolderDst;		
	CAOIModel   *ModelPtr = NULL;
	const bool   bDeleteDst = true;	
	const bool   bDeleteSrc = false;	
	const size_t ModelCount = GetProjectPartModelCount_Saved();
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectPartModelPtr_Saved(i, false);
		if ( NULL == ModelPtr )  { continue; }		
		FolderSrc = ModelPtr->GetModelFolder();		
		JetAPI::ExtractTopFolder(FolderSrc, FolderLoc);
		FolderDst.Format(_T("%s\\%s"), DstFolder, FolderLoc);
		if ( JetAPI::CopyFolderAToFolderB(FolderSrc, FolderDst, bDeleteSrc, bDeleteDst, _T(""), -1, -1) == false )
		{	continue;	}
		ModelPtr->SetModelNeedSaveFiles(false);
	}

	//刪除多的資料夾
	CString        FolderName;
	CAOIComponent *ComponentPtr = NULL;
	std::vector<CString> FolderList;
	std::vector<CAOIComponent*> ComponentList;
	JetAPI::ListFolder(DstFolder, FolderList);
	const size_t FolderCount = FolderList.size();
	
	//先將有模組隔離的零件記錄下來
	GetProjectComponentModelIsolated(ComponentList);
	const size_t ComponentCount = ComponentList.size();
	for ( i=0; i<FolderCount; i++ )
	{
		ModelPtr = NULL;
		FolderLoc = FolderList[i];
		for ( j=0; j<ComponentCount; j++ )
		{
			ComponentPtr = ComponentList[j];
			if ( NULL == ComponentPtr ) { continue; }
			if ( FolderLoc.CompareNoCase(ComponentPtr->GetComponentFullName()) != 0 ) { continue; }
			ModelPtr = ComponentPtr->GetComponentModelPtr();
			break;
		}
		if ( NULL != ModelPtr ) { continue; }
		FolderDst.Format(_T("%s\\%s"), DstFolder, FolderLoc);
		JetAPI::RemoveFolder(FolderDst);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SyncProjectModelToComponentsSelected(LPCTSTR ModelName, bool bPartial)//同步化模組至選取到的零件上-不變更模組的框數量下的更新
{
	CAOIModel *ModelPtr = CAOIProject::GetProjectModelPtrByModelName(ModelName);	
	if ( SyncProjectModelToComponentsSelected(ModelPtr, bPartial) == false )
	{	return false; }	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SyncProjectModelToComponentsSelected(CAOIModel *ModelPtr, bool bPartial)//同步化模組至選取到的零件上-不變更模組的框數量下的更新
{
	if ( NULL == ModelPtr ) 
	{
		m_ErrorString = _T("Error, No Library Model Ptr");
		return false; 
	}

	bool           bSucc=true;
	CString        ModelName = ModelPtr->GetModelName();
	CString        ModelNameC, ModelNameM;	
	const int      OpenMPCnt = GetProjectEditOpenMpCount();
	const int      ComponentCount = (int)(GetProjectComponentCount());		

	bSucc=true;
	ModelNameM = ModelName;
	ModelNameM.MakeUpper();

#ifdef OPEN_MP_USE
	#pragma omp parallel for num_threads(OpenMPCnt)
#endif//OPEN_MP_USE	
	for ( int i=0; i<ComponentCount; i++ )
	{
		if ( false == bSucc ) { break; }
		if ( i >= ComponentCount ) { continue; }
		CAOIComponent *ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		if ( ComponentPtr->GetComponentModelIsolated() == true ) { continue; }

		if ( ComponentPtr->SyncComponentModelFromLibrary(ModelPtr, bPartial) == false ) 
		{			
			LockProject();
			if ( false == bSucc )
			{
				UnlockProject();
				break;
			}
			bSucc = false;
			CString FullName = ComponentPtr->GetComponentFullName();
			this->m_ErrorString.Format(_T("Error, Synchronous Component Model From Library Fault[%s -> %s]"), FullName, ModelName);
			UnlockProject();
			break;
		}
	}	
	if ( false == bSucc )
	{	return false; }
	return true;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectModelCount() const//取得專案模組數量
{
	return GetProjectModelCount_Inline();
}
//---------------------------------------------------------------------------------//
CAOIModel* CAOIProject::ReplaceProjectModel(CAOIModel *ModelPtr)//取代專案模組
{
	if ( NULL == ModelPtr )
	{
		this->m_ErrorString.Format(_T("Error, ReplaceProjectModel Fault"));
		return NULL;
	}	
	CString ModelName = ModelPtr->GetModelName();	
	const size_t ModelIndex = GetProjectModelIndexByModelName(ModelName);
	if ( -1 == ModelIndex ) { return NULL; }
	const size_t ModelCount = GetProjectModelCount_Inline();
	if ( ModelIndex >= ModelCount ) { return NULL; }
	AOIObjManager.DestroyModelObj(m_ProjectModelPtrList[ModelIndex]);

	const unsigned int uModelIndex = (unsigned int)(ModelIndex);	
	ModelPtr->SetModelIndex(uModelIndex);
	ModelPtr->SetModelNeedSaveFiles(true);
	m_ProjectModelPtrList[ModelIndex] = ModelPtr;
	return ModelPtr;
}
//---------------------------------------------------------------------------------//
CAOIModel* CAOIProject::GetProjectModelPtr(size_t index, bool check) const//取得專案模組指標
{
	if ( check )
	{
		const size_t count = GetProjectModelCount_Inline();
		if ( index >= count )
		{	return NULL; }
	}
	return GetProjectModelPtr_Inline(index);
}
//---------------------------------------------------------------------------------//
CAOIModel* CAOIProject::AddProjectModelPtr(CAOIModel *ModelPtr, bool clone)//增加專案模組
{
	if ( NULL == ModelPtr )
	{
		this->m_ErrorString.Format(_T("Error, AddProjectModelPtr Fault"));
		return NULL;
	}

	CAOIModel *NewModelPtr = ModelPtr;
	const unsigned int ModelIndex = (unsigned int)(GetProjectModelCount_Inline());
	if ( true == clone )
	{
		NewModelPtr = ModelPtr->CloneModelObj();
		if ( NULL == NewModelPtr ) { return NULL; }		
		NewModelPtr->SetModelIndex(ModelIndex);
	}	
	else
	{	NewModelPtr->SetModelIndex(ModelIndex);	 }
	AddProjectModelPtr_Inline(NewModelPtr);		
	return NewModelPtr;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DestroyProjectModelSelected()//摧毀選取到的專案模組
{
	size_t i = 0;
	unsigned int  index = 0;
	CAOIModel *ModelPtr = NULL;
	std::vector<CAOIModel*> ProjectModelPtrList = this->m_ProjectModelPtrList;
	
	index = 0;
	this->m_ProjectModelPtrList.clear();
	const size_t ModelCount = ProjectModelPtrList.size();
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = ProjectModelPtrList[i];
		if ( NULL == ModelPtr ) { continue; }
		if ( ModelPtr->GetModelSelected() == true )
		{
			AOIObjManager.DestroyModelObj(ProjectModelPtrList[i]);
			ModelPtr = NULL;
			continue;
		}

		ModelPtr->SetModelIndex(index);
		CAOIProject::AddProjectModelPtr_Inline(ModelPtr);
		index ++;
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ClearProjectAllModels()//刪除專案模組
{
	size_t i = 0;	
	CAOIModel *ModelPtr = NULL;	
	const size_t ModelCount = GetProjectModelCount_Inline();
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }		
		AOIObjManager.DestroyModelObj(m_ProjectModelPtrList[i]);
		ModelPtr = NULL;		
		m_ProjectModelPtrList[i] = NULL;
	}		
	m_ProjectModelPtrList.clear();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::LayoutProjectModelList()//重整專案的模組列表
{
	size_t       i = 0;
	unsigned int index = 0;
	CAOIModel *ModelPtr = NULL;	
	std::vector<CAOIModel*> ProjectModelPtrList = this->m_ProjectModelPtrList;
	
	index = 0;
	this->m_ProjectModelPtrList.clear();
	const size_t ModelCount = ProjectModelPtrList.size();
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = ProjectModelPtrList[i];
		if ( NULL == ModelPtr ) { continue; }

		ModelPtr->SetModelIndex(index);
		CAOIProject::AddProjectModelPtr_Inline(ModelPtr);
		index ++;
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectAllModels(bool value)//選取專案的所有模組
{
	size_t i = 0;
	CAOIModel *ModelPtr = NULL;	
	const size_t ModelCount = GetProjectModelCount_Inline();
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		ModelPtr->SetModelSelected(value);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectModelSelected()//刪除選取到的專案模組
{	
	size_t       i=0;
	unsigned int ModelIndex = 0;
	unsigned int OldModelIndex = 0;
	unsigned int NewModelIndex = 0;
	CAOIModel   *ModelPtr = NULL;
	std::vector<unsigned int>  ModelIndexMap;
	std::vector<CAOIModel*>    ProjectModelPtrList = m_ProjectModelPtrList;
	const size_t ModelCount = ProjectModelPtrList.size();

	ModelIndex = 0;
	ModelIndexMap.clear();
	m_ProjectModelPtrList.clear();
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = ProjectModelPtrList[i];
		if ( NULL == ModelPtr ) { continue; }
		if ( ModelPtr->GetModelSelected() == true )
		{
			ModelIndexMap.push_back(-1);
			AOIObjManager.DestroyModelObj(ProjectModelPtrList[i]);
			continue; 
		}
		ModelIndexMap.push_back(ModelIndex);
		ModelPtr->SetModelIndex(ModelIndex);
		AddProjectModelPtr_Inline(ModelPtr);
		ModelIndex ++;
	}

	//更新至零件內
	CAOIComponent *ComponentPtr = NULL;
	const size_t ModelIndexMapSize = ModelIndexMap.size();
	const size_t ComponentCount = GetProjectComponentCount();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		OldModelIndex = ComponentPtr->GetComponentModelIndex();
		if ( OldModelIndex >= ModelIndexMapSize )
		{	NewModelIndex = -1; }
		else
		{	NewModelIndex = ModelIndexMap[OldModelIndex];	}

		ModelPtr->SetModelIndex(NewModelIndex);
		ComponentPtr->SetComponentModelIndex(NewModelIndex);

		if ( -1 == NewModelIndex )
		{	
			if ( ComponentPtr->GetComponentModelIsolated() == false )
			{	ComponentPtr->ResetComponentModel();	}
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectModelNameRepeated()//刪除重複名稱的專案模組	
{
	size_t       i=0, j=0;
	CString      ModelName;
	CString      ModelNameNext;
	CAOIModel   *ModelPtr = NULL;
	CAOIModel   *ModelPtrNext = NULL;		
	const size_t ModelCount = GetProjectModelCount_Inline();

	SelectProjectAllModels(false);
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		if ( ModelPtr->GetModelSelected() == true )
		{	continue; }
		ModelName = ModelPtr->GetModelName();
		for ( j=i+1; j<ModelCount; j++ )
		{
			ModelPtrNext = GetProjectModelPtr_Inline(j);
			if ( NULL == ModelPtrNext ) { continue; }
			if ( ModelPtrNext->GetModelSelected() == true )
			{	continue; }
			ModelNameNext = ModelPtrNext->GetModelName();
			if ( ModelName.CompareNoCase(ModelNameNext) != 0 )
			{	continue; }			
			ModelPtrNext->SetModelSelected(true);
			ModelPtrNext->SetModelAutoDeleteImageFolder(false);
		}		
	}
	if ( DeleteProjectModelSelected() == false )
	{	return false; }
	return true;
}
//---------------------------------------------------------------------------------/
bool CAOIProject::ClearProjectLibrary()//清除專案資料庫
{
	CAOIProject *ProjectPtr = GetProjectPtr_Direct();
	if ( NULL == ProjectPtr ) { return false; }

	size_t         i=0;	
	CString        PartNumber;
	CString        ModelFolder;
	CString        LibraryFolder;	
	CString        PartLibraryFolder;
	bool           bModelIsolated=false;	
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount();

	LibraryFolder = ProjectPtr->GetProjectLibraryFolder();
	PartLibraryFolder = GetProjectPartLibraryFolder();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		PartNumber = ComponentPtr->GetComponentPartNumber();
		bModelIsolated = ModelPtr->GetModelIsolated();
		ModelPtr->ClearModelAllObjList();
		ModelPtr->ReleaseModelUniFrameList();
		if ( true == bModelIsolated )
		{	ModelPtr->ClearModelFolder(MODEL_CLEAR_FOLDER_ALL_FILES);	}

		ComponentPtr->SetComponentModelIsolated(false);
		ModelPtr->SetModelType(MODEL_TYPE_NULL);
		ModelPtr->SetModelName(PartNumber);
		ModelFolder.Format(_T("%s\\%s"), LibraryFolder, PartNumber);
		ModelPtr->SetModelFolderModel(ModelFolder);
		ModelPtr->AssignModelFolder();
	}
	ClearProjectAllModels();
	
	JetAPI::ClearFolder(LibraryFolder);
	JetAPI::ClearFolder(PartLibraryFolder);
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ArrangeProjectLibrary()//整理專案資料庫
{
	size_t         i=0, j=0;	
	unsigned int   ModelIndex=0;
	CString        ModelName;	
	CString        ModelName_C;	
	CString        ModelName_V;
	MODEL_TYPE     ModelType;
	MODEL_TYPE     ModelType_C;
	size_t         ModelCount=0;
	unsigned int   ModelUsedCount=0;
	CAOIModel     *ModelPtr = NULL;
	CAOIModel     *ModelPtr_C = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetProjectComponentCount();

	//歸零
	ModelCount = GetProjectModelCount_Inline();
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		ModelPtr->SetModelUsedCount(0);
	}

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ModelPtr_C = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr_C ) { continue; }
		ModelType_C = ModelPtr_C->GetModelType();
		if ( MODEL_TYPE_NULL == ModelType_C ) { continue; }
		ModelName_C = ComponentPtr->GetComponentModelName();
		ModelPtr = GetProjectModelPtrByModelName(ModelName_C);
		if ( NULL == ModelPtr ) { continue; }
		ModelPtr->IncreaseModelUsedCount();
	}

	const size_t VersionCodeCount = GetProjectVersionCodeCount();
	const size_t ActiveVersionCodeIdx = GetProjectParameter().m_VersionCodeActiveIndex;
	for ( j=0; j<VersionCodeCount; j++ )
	{		
		if ( j == ActiveVersionCodeIdx ) { continue; }
		//if ( j == PROJECT_VERSION_CODE_BASE_INDEX ) { continue; }
		TVersionCode *VersionCodePtr = GetProjectVersionCodePtr(j, false);
		if ( NULL == VersionCodePtr ) { continue; }
		if ( false == VersionCodePtr->bEnabled ) { continue; }
		for ( i=0; i<ComponentCount; i++ )
		{
			ComponentPtr = GetProjectComponentPtr(i, false);
			if ( NULL == ComponentPtr ) { continue; }
			const TVersionParam *VersionParamPtr=ComponentPtr->GetComponentVersionParamPtr(j, true);
			if ( NULL == VersionParamPtr ) { continue; }
			const std::wstring wsModelName=VersionParamPtr->wsModelName;
			if ( 0 == wsModelName.length() ) { continue; }
			ModelName_V = wsModelName.c_str();
			ModelName_C = ComponentPtr->GetComponentModelName();
			if ( ModelName_V.CompareNoCase(ModelName_C) == 0 )
			{	continue; }			
			ModelPtr = GetProjectModelPtrByModelName(ModelName_V);
			if ( NULL == ModelPtr ) { continue; }
			ModelPtr->IncreaseModelUsedCount();
		}
	}

	//選取未套用的模組
	ModelCount = GetProjectModelCount_Inline();
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		ModelPtr->SetModelSelected(false);
		ModelUsedCount = ModelPtr->GetModelUsedCount();
		if ( ModelUsedCount > 0 ) { continue; }
		ModelPtr->SetModelSelected(true);
	}
	if ( DestroyProjectModelSelected() == false )
	{	return false; }

	//重新整理模組與零件的的引數	
	const bool bModelBkImage=AOIDataCollect.GetAutoArrangeModelBkImageFiles();
	ModelCount = GetProjectModelCount_Inline();
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }	
		ModelType = ModelPtr->GetModelType();
		ModelName = ModelPtr->GetModelName();
		ModelIndex = ModelPtr->GetModelIndex();		
		if ( true == bModelBkImage )
		{	ModelPtr->ArrangeModelBKImageFiles();	}
		for ( j=0; j<ComponentCount; j++ )
		{
			ComponentPtr = GetProjectComponentPtr(j, false);
			if ( NULL == ComponentPtr ) { continue; }
			ModelPtr_C = ComponentPtr->GetComponentModelPtr();
			if ( NULL == ModelPtr_C ) { continue; }
			ModelType_C = ModelPtr_C->GetModelType();
			if ( ModelType != ModelType_C ) { continue; }
			ModelName_C = ComponentPtr->GetComponentModelName();
			if ( ModelName.CompareNoCase(ModelName_C) != 0 ) { continue; }
			ModelPtr_C->SetModelIndex(ModelIndex);
			ComponentPtr->SetComponentModelIndex(ModelIndex);
			break;
		}
	}

	if ( true == bModelBkImage )
	{
		for ( i=0; i<ComponentCount; i++ )
		{
			ComponentPtr = GetProjectComponentPtr(i, false);
			if ( NULL == ComponentPtr ) { continue; }
			if ( ComponentPtr->GetComponentModelIsolated() == false ) { continue; }
			ModelPtr_C = ComponentPtr->GetComponentModelPtr();
			if ( NULL == ModelPtr_C ) { continue; }
			ModelPtr_C->ArrangeModelBKImageFiles();			
		}
	}

	//移除專案資料庫多餘資料夾
	if (RemoveProjectLibraryFolderNoUsed() == false )
	{	return false; }
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::UpdateProjectLibraryFolder()//更新專案資料庫資料夾
{
	CAOIProject *ProjectPtr = this;
	if ( NULL == ProjectPtr ) { return false; }

	size_t         i=0;
	bool           ModelIsolated=false;
	CString        ModelName;
	CString        ModelFolder;		
	CString        LibraryFolder;
	CString        ComponentFolder;	
	CString        PartLibraryFolder;		
	CString        FullComponentName;
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ModelCount = ProjectPtr->GetProjectModelCount();
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount();

	LibraryFolder = ProjectPtr->GetProjectLibraryFolder();	
	PartLibraryFolder = ProjectPtr->GetProjectPartLibraryFolder();	
	
	//資料庫的部分
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = ProjectPtr->GetProjectModelPtr(i, false);
		if ( NULL == ModelPtr ) { continue; }
		ModelName = ModelPtr->GetModelName();
		ModelFolder.Format(_T("%s\\%s"), LibraryFolder, ModelName);
		ModelPtr->SetModelFolderModel(ModelFolder);
		ModelPtr->AssignModelFolder();
	}

	//零件上的資料庫
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }

		ModelIsolated = ComponentPtr->GetComponentModelIsolated();
		ModelName = ModelPtr->GetModelName();
		ModelFolder.Format(_T("%s\\%s"), LibraryFolder, ModelName);
		ModelPtr->SetModelFolderModel(ModelFolder);
		if ( true == ModelIsolated )
		{
			FullComponentName = ComponentPtr->GetComponentFullName();
			ComponentFolder.Format(_T("%s\\%s"), PartLibraryFolder, FullComponentName);
			ModelPtr->SetModelFolderComponent(ComponentFolder);
		}		
		ModelPtr->AssignModelFolder();
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RemoveProjectLibraryFolderNoUsed()//移除專案資料庫資料夾-未使用
{
	std::vector<CString> ModelFolderList;
	CString LibraryFolder=GetProjectLibraryFolder();
	if ( JetAPI::IsFolderExist(LibraryFolder) == false ) { return true; }
	if ( JetAPI::ListFolder(LibraryFolder, ModelFolderList) == false )
	{
		m_ErrorString.Format(_T("Error, List Project Library Folder Fault[%s]"), LibraryFolder);
		return false;
	}

	size_t     i=0, j=0;
	CString    ModelFolder;
	CAOIModel *ModelPtr=NULL;
	std::vector<CString> RemoveFolderList;
	const size_t ModelCount=GetProjectModelCount();
	const size_t FolderCount=ModelFolderList.size();
	const int    Checked   = 1;
	const int    Unchecked = 0;
	for ( j=0; j<ModelCount; j++ )
	{
		ModelPtr = GetProjectModelPtr(j, false);
		if ( NULL == ModelPtr ) { continue; }
		ModelPtr->SetModelTempInt(Unchecked);
	}

	RemoveFolderList.resize(FolderCount, _T(""));
	RemoveFolderList.clear();
	for ( i=0; i<FolderCount; i++ )
	{
		ModelFolder = ModelFolderList[i];
		for ( j=0; j<ModelCount; j++ )
		{
			ModelPtr = GetProjectModelPtr(j, false);
			if ( NULL == ModelPtr ) { continue; }
			if ( Checked == ModelPtr->GetModelTempInt() ) { continue; }
			if ( ModelFolder.CompareNoCase(ModelPtr->GetModelName()) != 0 ) { continue; }
			ModelPtr->SetModelTempInt(Checked);
			break;
		}
		if ( j != ModelCount ) { continue; }
		RemoveFolderList.push_back(ModelFolder);
	}

	CString RemoveFolder;
	const size_t RemoveFolderCount=RemoveFolderList.size();
	for ( i=0; i<RemoveFolderCount; i++ )
	{
		RemoveFolder = RemoveFolderList[i];
		if ( RemoveFolder.GetLength() == 0 ) { continue; }
		ModelFolder.Format(_T("%s\\%s"), LibraryFolder, RemoveFolder);
		JetAPI::RemoveFolder(ModelFolder);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::BuildProjectDefaultLibrary()//建立專案預設的資料庫
{
	CAOIProject::ClearProjectAllModels();
	return true;

	wchar_t     strGroupName[256]=L"";	
	CAOIModel  *ModelPtr = NULL;	
	MODEL_TYPE  ModelType=MODEL_TYPE_NULL;
	std::vector<MODEL_TYPE> ModelTypeList;
	CAOIModel::GetModelTypeList(ModelTypeList);	
	const size_t ModelTypeCount=ModelTypeList.size();
	for ( size_t i=0; i<ModelTypeCount; i++ )
	{
		ModelType = ModelTypeList[i];
		if ( MODEL_TYPE_NULL == ModelType ) { continue; }
		ModelPtr = AOIObjManager.CreateModelObj();
		if ( NULL == ModelPtr ) { return false; }		
		ModelPtr->BuildModelType(ModelType);
		CAOIModel::GetModelTypeText(ModelType, strGroupName);
		ModelPtr->SetModelGroupName(strGroupName);
		ModelPtr->SetModelName(strGroupName);
		CAOIProject::AddProjectModelPtr(ModelPtr, false);
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ApplyProjectLibraryConfiguration()//套用專案資料庫組態
{
	size_t         i=0;
	CAOIModel     *ModelPtr = NULL;
	unsigned int   FrameIndex = 0;
	unsigned int   FrameUniqueID = 0;
	unsigned int   DefaultFrameIndex=0;
	unsigned int   DefaultFrameUniqueID = 0;
	std::vector<unsigned int> FrameIndexMapList;
	TProjectParameter &ProParam = GetProjectParameter();
	const size_t  ModelCount = GetProjectModelCount_Inline();
	if ( BuildProjectFrameIndexMapParam(FrameIndexMapList, DefaultFrameIndex, DefaultFrameUniqueID) == false )
	{	return false; }	

	//專案抽色
	CColorGroup *ColorGroupPtr = NULL;
	const size_t ColorGroupCount = GetProjectColorGroupCount();
	for ( i=0; i<ColorGroupCount; i++ )
	{
		ColorGroupPtr = GetProjectColorGroupPtr(i, false);
		if ( NULL == ColorGroupPtr ) { continue; }		
		ColorGroupPtr->UpdateColorGroupFrameUniqueID(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);
	}
	std::vector<CColorGroup>  ColorGroupList;
	CloneProjectColorGroupList(ColorGroupList);	

	//拋件的基準面參數
	TBasePlaneParam &DropBasePlaneParamRef=ProParam.m_DropOutPartSpaceNoiseFilter.BasePlaneParam;	
	UpdateProjectBasePlaneColorGroupLinkIndex(ColorGroupList, DropBasePlaneParamRef);	
	UpdateProjectBasePlaneFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList, DropBasePlaneParamRef);

	//模組資料
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		ModelPtr->UpdateModelColorGroupLinkIndex(ColorGroupList);
		ModelPtr->UpdateModelFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);
	}

	//定位點
	CAOIFd       *FdPtr = NULL;
	const size_t  FdCount = GetProjectFdCount();	
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }		
		//FdPtr->UpdateFdColorGroupLinkIndex(ColorGroupList);
		FdPtr->UpdateFdFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);

		ModelPtr = FdPtr->GetFdModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		ModelPtr->UpdateModelColorGroupLinkIndex(ColorGroupList);
		ModelPtr->UpdateModelFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);
	}

	//特徵點
	CAOIMark    *MarkPtr = NULL;
	const size_t MarkCount = GetProjectMarkCount();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetProjectMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }	
		//MarkPtr->UpdateMarkColorGroupLinkIndex(ColorGroupList);
		MarkPtr->UpdateMarkFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);

		ModelPtr = MarkPtr->GetMarkModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		ModelPtr->UpdateModelColorGroupLinkIndex(ColorGroupList);
		ModelPtr->UpdateModelFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);
	}

	//條碼
	CAOIBarcode  *BarcodePtr = NULL;
	const size_t  BarcodeCount = GetProjectBarcodeCount();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetProjectBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }		
		//BarcodePtr->UpdateBarcodeColorGroupLinkIndex(ColorGroupList);
		BarcodePtr->UpdateBarcodeFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);

		ModelPtr = BarcodePtr->GetBarcodeModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		ModelPtr->UpdateModelColorGroupLinkIndex(ColorGroupList);
		ModelPtr->UpdateModelFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);
	}

	//零件
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ComponentCount = GetProjectComponentCount();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }				
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		ComponentPtr->UpdateComponentColorGroupLinkIndex(ColorGroupList);
		ComponentPtr->UpdateComponentFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);
		ModelPtr->UpdateModelColorGroupLinkIndex(ColorGroupList);
		ModelPtr->UpdateModelFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ApplyProjectLibraryToComponents()//套用專案資料庫至零件上
{
	size_t         i=0, j=0;
	CString        ModelName;
	MODEL_TYPE     ModelType;
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t ModelCount = GetProjectModelCount_Inline();
	const size_t ComponentCount = GetProjectComponentCount();
	
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		ModelType = ModelPtr->GetModelType();
		//if ( MODEL_TYPE_NULL == ModelType ) { continue; }
		if ( CAOIModel::CheckModelTypeEnabled(ModelType) == false )
		{	continue;	}

		ModelName = ModelPtr->GetModelName();
		ModelName.MakeUpper();
		SelectProjectAllComponents(false);
		SelectProjectComponentsByModelName(ModelName, false);
		ApplyProjectModelToComponentsSelected(ModelPtr);		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ApplyProjectLibraryToComponentsSelected()//更新模組至選取到的零件上
{
	const int OpenMPCnt = GetProjectEditOpenMpCount();
	const int ComponentCount = (int)(GetProjectComponentCount());
	const int OpenMPCntUsd=MIN(ComponentCount, OpenMPCnt);
	AOIObjManager.LayoutRecycleObjList(OpenMPCntUsd);

#ifdef OPEN_MP_USE
	#pragma omp parallel for num_threads(OpenMPCntUsd)
#endif//OPEN_MP_USE	
	for ( int i=0; i<ComponentCount; i++ )
	{
		if ( i >= ComponentCount ) { break; }
		CAOIComponent *ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		if ( ComponentPtr->GetComponentModelIsolated() == true ) { continue; }

		CString ModelName = ComponentPtr->GetComponentModelName();
		CAOIModel *ModelPtr = this->GetProjectModelPtrByModelName(ModelName);
		if ( NULL == ModelPtr ) { continue; }
		ComponentPtr->UpdateComponentModelFromLibrary(ModelPtr);
	}
	AOIObjManager.LayoutRecycleObjList();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectLibraryGroupWnd(CAOIModel *RefModelPtr)//刪除專案資料庫群組檢測框
{
	CAOIProject *ProjectPtr = this;	
	if ( CheckProjectPtr(ProjectPtr) == false )
	{	return false; }	
	if ( NULL == RefModelPtr ) { return false; }	

	std::vector<CAOIModel*> ModelList;
	if ( ListProjectModelByGroupName(RefModelPtr, ModelList) == false ) 
	{	return false; }

	bool          bModified=false;	
	size_t        i=0, j=0, k=0;
	size_t        WndCount;
	CString       ModelName;
	CAOIWnd      *WndPtr = NULL;
	CAOIWnd      *RefWndPtr = NULL;	
	CAOIModel    *ModelPtr = NULL;		
	WND_DEFECT_ID WndDefectID;
	WND_DEFECT_ID RefWndDefectID;	
	int           DefectGroupID=0;
	int           RefDefectGroupID=0;
	std::vector<unsigned int> SelWndIndexList;
	const size_t ModelCount = ModelList.size();	
	
	std::vector<CAOIWnd*> SelWndList;
	std::pair<WND_DEFECT_ID, int> WndSelPair;	
	std::vector<std::pair<WND_DEFECT_ID, int>> WndSelList;
	RefModelPtr->GetModelWndSelectedList(SelWndList);	
	size_t RefWndCount = SelWndList.size();
	for ( j=0; j<RefWndCount; j++ )
	{
		RefWndPtr = SelWndList[j];
		if ( NULL == RefWndPtr ) { continue; }			
		RefWndDefectID = RefWndPtr->GetWndDefectID();
		RefDefectGroupID = RefWndPtr->GetWndDefectGroupID();
		WndSelPair.first = RefWndDefectID;
		WndSelPair.second = RefDefectGroupID;
		WndSelList.push_back(WndSelPair);
	}
	SelWndList.clear();
	RefWndCount = WndSelList.size();

	ProjectPtr->SelectProjectAllComponents(false);	
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = ModelList[i];
		if ( NULL == ModelPtr ) { continue; }
		bModified=false;
		SelWndIndexList.clear();
		for ( j=0; j<RefWndCount; j++ )
		{
			WndSelPair = WndSelList[j];			
			RefWndDefectID = WndSelPair.first;
			RefDefectGroupID = WndSelPair.second;

			WndCount = ModelPtr->GetModelWndCount();
			for ( k=0; k<WndCount; k++ )
			{
				WndPtr = ModelPtr->GetModelWndPtr(k, false);
				if ( NULL == WndPtr ) { continue; }				
				WndDefectID = WndPtr->GetWndDefectID();
				DefectGroupID = WndPtr->GetWndDefectGroupID();
				if ( WndDefectID != RefWndDefectID ) { continue; }
				if ( DefectGroupID != RefDefectGroupID ) { continue; }
				bModified = true;				
				SelWndIndexList.push_back(k);
			}
			if ( false == bModified )
			{	continue; }

			ModelPtr->DeleteModelWndIndexList(SelWndIndexList);
			ModelPtr->CalcModelTotalRegionAll();
		}
		if ( false == bModified ) { continue; }

		//複製模組出來		
		ModelName = ModelPtr->GetModelName();
		ProjectPtr->SelectProjectComponentsByModelName(ModelName, false);
		ProjectPtr->ApplyProjectModelToComponentsSelected(ModelPtr);
		ProjectPtr->SelectProjectAllComponents(false);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::UnSelectProjectModel()//啟用取消啟用-不選取模組
{
	size_t                  i=0;		
	CAOIModel              *ModelPtr = NULL;	
	CAOIComponent          *ComponentPtr = NULL;
	const size_t ComponentCount = GetProjectComponentCount();
	const size_t ModelCount = GetProjectModelCount_Inline();
		
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		ModelPtr->UnSelectModel();
	}

	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		ModelPtr->UnSelectModel();
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectModelNameExist(LPCTSTR ModelName)
{
	CAOIModel *ModelPtr = CAOIProject::GetProjectModelPtrByModelName(ModelName);
	if ( NULL == ModelPtr ) { return false; }	
	return true;
}
//---------------------------------------------------------------------------------//
CAOIModel* CAOIProject::GetProjectModelPtrByModelName(LPCTSTR ModelName)
{
	size_t ModelIndex = GetProjectModelIndexByModelName(ModelName);	
	return GetProjectModelPtr(ModelIndex, true);		
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectModelIndexByModelName(LPCTSTR ModelName)
{
	size_t                  i=0;	
	CString                 ModelName_M, ModelName_S;
	CAOIModel              *ModelPtr = NULL;	
	const size_t ModelCount = GetProjectModelCount_Inline();
	
	ModelName_S = ModelName;
	ModelName_S.MakeUpper();
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		ModelName_M = ModelPtr->GetModelName();
		ModelName_M.MakeUpper();
		if ( ModelName_M != ModelName_S ) { continue; }
		return i;
	}	
	return -1;
}
//---------------------------------------------------------------------------------//
CAOIModel* CAOIProject::CloneProjectModel(LPCTSTR ModelName, LPCTSTR NewModelName)//複製模組		
{
	int                     j = 0;
	size_t                  i=0;	
	CString                 LibraryFolder;
	CString                 OldModelFolder;
	CString                 NewModelFolder;
	CString                 OldModelBKName;
	CString                 NewModelBKName;	
	CString                 ModelName_M, ModelName_S, ModelName_N;
	CAOIModel              *ModelPtr = NULL;	
	CAOIModel              *NewModelPtr = NULL;		
	const size_t TextBuffer = 256;
	char  ModelNameBuffer[TextBuffer]="";
	const size_t ModelCount = GetProjectModelCount_Inline();	
	
	ModelName_S = ModelName;
	ModelName_S.MakeUpper();
	ModelName_N = NewModelName;
	ModelName_N.MakeUpper();
	if ( ModelName_N == ModelName_S ) 
	{	
		m_ErrorString.Format(_T("Error, Can not clone the same name model [%s]"), ModelName);
		return NULL; 
	}
	
	LibraryFolder = GetProjectLibraryFolder();
	NewModelFolder.Format(_T("%s\\%s"), LibraryFolder, NewModelName);
	JetAPI::TCHAR2char(ModelName_N, ModelNameBuffer, TextBuffer);	
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }		

		ModelName_M = ModelPtr->GetModelName();
		ModelName_M.MakeUpper();
		if ( ModelName_M != ModelName_S ) { continue; }

		NewModelPtr = ModelPtr->CloneModelObj();
		if ( NULL == NewModelPtr )
		{
			m_ErrorString.Format(_T("Error, Clone Model Object Fault[%s]"), ModelName);
			return false;
		}
		//要複製資料夾
		OldModelFolder = ModelPtr->GetModelFolderModel();
		JetAPI::CopyFolderAToFolderB(OldModelFolder, NewModelFolder, false, true, _T(""), -1, -1);
		NewModelPtr->SetModelName(ModelNameBuffer);		
		NewModelPtr->SetModelFolderModel(NewModelFolder);
		for ( j=0; j<FRAME_MAX_COUNT; j++ )
		{	
			OldModelBKName = AOIDataDefine.GetModelBKImageFilename(NewModelFolder, ModelName_M, j);
			NewModelBKName = AOIDataDefine.GetModelBKImageFilename(NewModelFolder, NewModelName, j);			
			if ( OldModelBKName.CompareNoCase(NewModelBKName) == 0 ) { continue; }			
			::MoveFile(OldModelBKName, NewModelBKName);
			//::CopyFile(OldModelBKName, NewModelBKName, FALSE);
		}
		NewModelPtr->UnSelectModel();
		NewModelPtr->AssignModelFolder();		
		NewModelPtr->SetModelNeedSaveFiles(true);
		NewModelPtr->SetModelBodyBoxActived(true);
		NewModelPtr->SetModelBKImageNeedToGrab(true);
		return NewModelPtr;
	}
	return NULL;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RenameProjectModel(LPCTSTR ModelName, LPCTSTR NewModelName)//變更模組名稱
{
	size_t                  i=0, j=0;		
	CString                 LibraryFolder;
	CString                 OldModelFolder;
	CString                 NewModelFolder;
	CString                 OldModelBKName;
	CString                 NewModelBKName;	
	CString                 ModelName_M, ModelName_S, ModelName_N;
	CAOIModel              *ModelPtr = NULL;	
	CAOIComponent          *ComponentPtr = NULL;
	const size_t TextBuffer = 256;
	char  ModelNameBuffer[TextBuffer]="";
	const size_t ModelCount = GetProjectModelCount_Inline();
	const size_t ComponentCount = GetProjectComponentCount();
	
	ModelName_S = ModelName;
	ModelName_S.MakeUpper();
	ModelName_N = NewModelName;
	ModelName_N.MakeUpper();
	if ( ModelName_N == ModelName_S ) 
	{	return true; }
	
	LibraryFolder = GetProjectLibraryFolder();
	NewModelFolder.Format(_T("%s\\%s"), LibraryFolder, NewModelName);
	JetAPI::TCHAR2char(ModelName_N, ModelNameBuffer, TextBuffer);	
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }		

		ModelName_M = ModelPtr->GetModelName();
		ModelName_M.MakeUpper();
		if ( ModelName_M != ModelName_S ) { continue; }

		//要複製資料夾
		OldModelFolder = ModelPtr->GetModelFolderModel();
		JetAPI::CopyFolderAToFolderB(OldModelFolder, NewModelFolder, true, true, _T(""), -1, -1);
		ModelPtr->SetModelName(ModelNameBuffer);		
		ModelPtr->SetModelFolderModel(NewModelFolder);
		for ( j=0; j<FRAME_MAX_COUNT; j++ )
		{	
			OldModelBKName = AOIDataDefine.GetModelBKImageFilename(NewModelFolder, ModelName_M, j);
			NewModelBKName = AOIDataDefine.GetModelBKImageFilename(NewModelFolder, NewModelName, j);
			//::MoveFileEx(OldModelBKName, NewModelBKName, MOVEFILE_REPLACE_EXISTING);
			if ( OldModelBKName.CompareNoCase(NewModelBKName) == 0 ) { continue; }			
			::MoveFile(OldModelBKName, NewModelBKName);
			//::CopyFile(OldModelBKName, NewModelBKName, FALSE);		
		}		
		ModelPtr->UnSelectModel();
		ModelPtr->AssignModelFolder();
		ModelPtr->SetModelNeedSaveFiles(true);
		ModelPtr->SetModelBodyBoxActived(true);		
		break;
	}	

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }		

		ModelName_M = ModelPtr->GetModelName();
		ModelName_M.MakeUpper();
		if ( ModelName_M != ModelName_S ) { continue; }
		ModelPtr->SetModelName(ModelNameBuffer);
		ModelPtr->SetModelFolderModel(NewModelFolder);
		if ( ModelPtr->GetModelIsolated() == false )
		{	ModelPtr->AssignModelFolder(); }
		ComponentPtr->SetComponentModelName(ModelNameBuffer);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::GetProjectModelGroupList(MODEL_TYPE ModelType, std::vector<CString> &GroupNameList)
{
	GroupNameList.clear();

	size_t                  i=0, j=0;
	size_t                  SortCount=0;
	CString                 GroupName_M, GroupName_S;
	CAOIModel              *ModelPtr = NULL;
	CSortObj                SortNode, *pSortNode=NULL;
	std::vector<CSortObj>   SortList;
	const size_t ModelCount = GetProjectModelCount_Inline();

	SortNode.SetSortMode(SORT_BY_TXT);
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		if ( MODEL_TYPE_NULL != ModelType )
		{
			if ( ModelPtr->GetModelType() != ModelType ) { continue; }
		}

		GroupName_M = ModelPtr->GetModelGroupName();
		GroupName_M.MakeUpper();

		SortCount = SortList.size();
		for ( j=0; j<SortCount; j++ )
		{
			pSortNode = &(SortList[j]);
			if ( GroupName_M == pSortNode->GetValueStr() )
			{	break; }
		}
		if ( j != SortCount ) { continue; }
		SortNode.SetID(SortCount);
		SortNode.SetValueStr(GroupName_M);
		SortList.push_back(SortNode);
	}
	std::sort(SortList.begin(), SortList.end());

	//Add All Group
	GroupName_M = AOIDataDefine.GetModelGroupAllText();
	GroupNameList.push_back(GroupName_M);

	SortCount = SortList.size();
	for ( i=0; i<SortCount; i++ )
	{
		pSortNode = &(SortList[i]);
		GroupNameList.push_back(pSortNode->GetValueStr());
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectModelGroupNameExist(LPCTSTR GroupName)
{
	MODEL_TYPE ModelType;
	ModelType = CheckProjectModelGroupNameModelType(GroupName);
	if ( MODEL_TYPE_NULL == ModelType ) { return false; }
	return true;
}
//---------------------------------------------------------------------------------//
MODEL_TYPE CAOIProject::CheckProjectModelGroupNameModelType(LPCTSTR GroupName)//確認模組群組名稱的模組樣式
{
	size_t       i=0;	
	CString      GroupName_M, GroupName_S;	
	CAOIModel   *ModelPtr = NULL;	
	CString      strGroupAllName = AOIDataDefine.GetModelGroupAllText();
	const size_t ModelCount = GetProjectModelCount_Inline();		

	GroupName_S = GroupName;
	GroupName_S.MakeUpper();
	
	strGroupAllName.MakeUpper();
	if ( strGroupAllName == GroupName_S ) { return MODEL_TYPE_ALL; }

	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		//if ( ModelPtr->GetModelType() != ModelType ) { continue; }		

		GroupName_M = ModelPtr->GetModelGroupName();
		GroupName_M.MakeUpper();
		if ( GroupName_M != GroupName_S ) { continue; }
		return ModelPtr->GetModelType();
	}	
	return MODEL_TYPE_NULL;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectModelGroupNameExist(MODEL_TYPE ModelType, LPCTSTR GroupName)
{
	size_t       i=0;	
	CString      GroupName_M, GroupName_S;
	CAOIModel   *ModelPtr = NULL;	
	const size_t ModelCount = GetProjectModelCount_Inline();
	CString      strGroupAllName = AOIDataDefine.GetModelGroupAllText();	
	
	GroupName_S = GroupName;
	GroupName_S.MakeUpper();

	strGroupAllName.MakeUpper();
	if ( strGroupAllName == GroupName_S ) { return true; }

	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		if ( ModelPtr->GetModelType() == ModelType ) { continue; }

		GroupName_M = ModelPtr->GetModelGroupName();
		GroupName_M.MakeUpper();
		if ( GroupName_M == GroupName_S ) { return true; }		
	}	
	return false;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RenameProjectModelGroupName(MODEL_TYPE ModelType, LPCTSTR GroupName, LPCTSTR NewGroupName)
{
	size_t        i=0;	
	CString       GroupName_M, GroupName_S, GroupName_N;	
	std::wstring  GroupNameStr=L"";
	CAOIModel    *ModelPtr = NULL;	
	CAOIComponent*ComponentPtr = NULL;
	CString       strGroupAllName = AOIDataDefine.GetModelGroupAllText();
	const size_t  ModelCount = GetProjectModelCount_Inline();
	const size_t  ComponentCount = GetProjectComponentCount();
	
	GroupName_S = GroupName;
	GroupName_S.MakeUpper();
	GroupName_N = NewGroupName;
	GroupName_N.MakeUpper();
	if ( GroupName_N == GroupName_S ) 
	{	return true; }

	strGroupAllName.MakeUpper();
	if ( strGroupAllName == GroupName_S ) { return false; }

	JetAPI::TCHAR2wstring(GroupName_N, GroupNameStr);
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		if ( ModelPtr->GetModelType() != ModelType ) { continue; }

		GroupName_M = ModelPtr->GetModelGroupName();
		GroupName_M.MakeUpper();
		if ( GroupName_M != GroupName_S ) { continue; }
		ModelPtr->SetModelGroupName(GroupNameStr.c_str());
	}	

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		if ( ModelPtr->GetModelType() != ModelType ) { continue; }

		GroupName_M = ModelPtr->GetModelGroupName();
		GroupName_M.MakeUpper();
		if ( GroupName_M != GroupName_S ) { continue; }
		ModelPtr->SetModelGroupName(GroupNameStr.c_str());		
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ChangeProjectModelTypeByName(LPCTSTR ModelName, MODEL_TYPE NewModelType, LPCTSTR NewGroupName)//變更專案模組樣式
{
	size_t        i=0;	
	CString       ModelName_M;
	CString       GroupName_M;	
	std::wstring  GroupNameStr=L"";
	CAOIModel    *ModelPtr = NULL;	
	CAOIComponent*ComponentPtr = NULL;	
	const size_t  ModelCount = GetProjectModelCount_Inline();
	const size_t  ComponentCount = GetProjectComponentCount();	
	
	GroupName_M = NewGroupName;
	GroupName_M.MakeUpper();	
	JetAPI::TCHAR2wstring(GroupName_M, GroupNameStr);
	for ( i=0; i<ModelCount; i++ )
	{
		ModelPtr = GetProjectModelPtr_Inline(i);
		if ( NULL == ModelPtr ) { continue; }
		ModelName_M = ModelPtr->GetModelName();
		if ( ModelName_M.CompareNoCase(ModelName) != 0 ) { continue; }

		ModelPtr->SetModelType(NewModelType);
		ModelPtr->SetModelGroupName(GroupNameStr.c_str());
	}	

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		ModelName_M = ModelPtr->GetModelName();
		if ( ModelName_M.CompareNoCase(ModelName) != 0 ) { continue; }

		ModelPtr->SetModelType(NewModelType);
		ModelPtr->SetModelGroupName(GroupNameStr.c_str());
	}	
	return true;
}
//---------------------------------------------------------------------------------//
