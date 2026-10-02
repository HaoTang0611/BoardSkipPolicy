// AOIProject.cpp: implementation of the CAOIProject class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIProject.h"
//-------------------------------------------------------------------------------------//
#include <map>
#include "SortObj.h"
#include "JetBlob.h"
#include "AOIFileIO.h"
#include "SmartChart\SmartChart.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
inline size_t CAOIProject::GetProjectPartGroupCount_Inline() const//取得專案零件群組數量
{
	return m_ProjectPartGroupPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIProject::AddProjectPartGroupPtr_Inline(CAOIPartGroup *PartGroupPtr)//增加專案零件群組
{
	m_ProjectPartGroupPtrList.push_back(PartGroupPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIPartGroup* CAOIProject::GetProjectPartGroupPtr_Inline(size_t index) const//取得專案零件群組指標	
{
	return m_ProjectPartGroupPtrList[index];
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectPartGroupSetting()//確認專案零件群組的設定
{
	size_t  i = 0;		
	CString Error;
	CString GroupName;	
	CString strIsNotInOneBoard;
	std::vector<CString> ErrList;
	CAOIPartGroup *PartGroupPtr = NULL;		
	CString GroupText=AOIDataDefine.GetGroupText();
	const size_t PartGroupCount = GetProjectPartGroupCount_Inline();
	strIsNotInOneBoard = _T("is not in one Board");
	strIsNotInOneBoard = LoadMultiLanguageString(strIsNotInOneBoard, strIsNotInOneBoard);
	for ( i=0; i<PartGroupCount; i++ )
	{
		PartGroupPtr = GetProjectPartGroupPtr_Inline(i);
		if ( NULL == PartGroupPtr ) { continue; }
		if ( PartGroupPtr->GetPartGroupInOneBoard() == true )
		{
			if ( PartGroupPtr->CheckPartGroupNodeInOneBoard() == false )
			{
				GroupName=PartGroupPtr->GetPartGroupName();
				Error.Format(_T("[%d] %s [%s:%d] %s"), i+1, GroupName, GroupText, PartGroupPtr->GetPartGroupGroupID_UI(), strIsNotInOneBoard);
				ErrList.push_back(Error);
				continue;
			}
		}		
	}		

	const size_t ErrCount=ErrList.size();
	if ( ErrCount > 0 )
	{
		FILE *pfile=NULL;
		CString Filename;
		TCHAR TMode[32] = _T("w+");	
		JetAPI::ModifyOpenFileMode_Write(TMode);
		Filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("PartGroupErrList.TXT"));
		pfile = ::_tfopen(Filename, TMode);
		if ( NULL != pfile )
		{
			for ( i=0; i<ErrCount; i++ )
			{	::_ftprintf(pfile, _T("%s\n"), ErrList[i]);	}
			::fclose(pfile); pfile=NULL;
		}
		Error = _T("Error, Check Part Group Setting Fault");
		Error = LoadMultiLanguageString(Error, Error);
		SetErrorString(Error);
		::ShellExecute(NULL, _T("open"), Filename, NULL, NULL, SW_SHOW);
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectPartGroupCount() const//取得專案零件群組數量
{
	return GetProjectPartGroupCount_Inline();
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectPartGroupPtr(CAOIPartGroup *PartGroupPtr)//增加專案零件群組
{
	if ( NULL == PartGroupPtr ) 
	{ 
		m_ErrorString = _T("Erorr, PartGroupPtr == NULL");
		return false; 
	}
	return true;
}
//---------------------------------------------------------------------------------//
CAOIPartGroup* CAOIProject::GetProjectPartGroupPtr(size_t index, bool check) const//取得專案零件群組指標
{
	if ( check )
	{
		const size_t count = GetProjectPartGroupCount_Inline();
		if ( index >= count )
		{	return NULL; }
	}
	return GetProjectPartGroupPtr_Inline(index);
}
//---------------------------------------------------------------------------------//
CAOIPartGroup* CAOIProject::AddProjectPartGroupPtr(CAOIPartGroup *PartGroupPtr)//增加專案零件群組
{
	if ( NULL == PartGroupPtr )
	{
		this->m_ErrorString.Format(_T("Error, AddProjectPartGroupPtr Fault"));
		return NULL;
	}

	CAOIPartGroup *NewGroupPtr = PartGroupPtr;
	unsigned int PartGroupIndex = (unsigned int)(GetProjectPartGroupCount_Inline());	
	NewGroupPtr->SetPartGroupIndex(PartGroupIndex);
	//int GroupUniqueID = NewGroupPtr->GetPartGroupUniqueID();
	//if ( GroupUniqueID < 0 )
	//{
	//	NewGroupPtr->SetPartGroupUniqueID(m_ProjectGroupMaxUniqueID);
	//	m_ProjectGroupMaxUniqueID ++;
	//}
	//else
	//{
	//	if ( m_ProjectGroupMaxUniqueID <= GroupUniqueID )
	//	{	m_ProjectGroupMaxUniqueID = GroupUniqueID+1; }
	//}
	NewGroupPtr->SetPartGroupProjectPtr(this);	
	AddProjectPartGroupPtr_Inline(NewGroupPtr);	
	return NewGroupPtr;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DestroyProjectPartGroupSelected()//摧毀選取到的專案零件群組
{
	size_t i = 0;
	int    index = 0;
	CAOIPartGroup *GroupPtr = NULL;
	std::vector<CAOIPartGroup*> PartGroupPtrList = m_ProjectPartGroupPtrList;
	
	index = 0;
	m_ProjectPartGroupPtrList.clear();
	const size_t GroupCount = PartGroupPtrList.size();
	for ( i=0; i<GroupCount; i++ )
	{
		GroupPtr = PartGroupPtrList[i];
		if ( NULL == GroupPtr ) { continue; }
		if ( GroupPtr->GetPartGroupSelected() == true )
		{
			AOIObjManager.DestroyPartGroupObj(PartGroupPtrList[i]);			
			GroupPtr = NULL;
			continue;
		}

		GroupPtr->SetPartGroupIndex(index);
		AddProjectPartGroupPtr_Inline(GroupPtr);
		index ++;
	}		
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::ClearProjectAllPartGroups()//刪除專案零件群組
{
	size_t i = 0;
	CAOIPartGroup *PartGroupPtr = NULL;
	const size_t PartGroupCount = GetProjectPartGroupCount_Inline();
	for ( i=0; i<PartGroupCount; i++ )
	{
		PartGroupPtr = GetProjectPartGroupPtr_Inline(i);
		if ( NULL == PartGroupPtr ) { continue; }		
		AOIObjManager.DestroyPartGroupObj(m_ProjectPartGroupPtrList[i]);			
		PartGroupPtr = NULL;		
	}		
	m_ProjectPartGroupPtrList.clear();
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::LayoutProjectPartGroupList()//重整專案的零件群組列表	
{
	size_t i = 0;
	int    index = 0;
	CAOIPartGroup *GroupPtr = NULL;
	std::vector<CAOIPartGroup*> PartGroupPtrList = m_ProjectPartGroupPtrList;
	
	index = 0;
	m_ProjectPartGroupPtrList.clear();
	const size_t GroupCount = PartGroupPtrList.size();
	for ( i=0; i<GroupCount; i++ )
	{
		GroupPtr = PartGroupPtrList[i];
		if ( NULL == GroupPtr ) { continue; }

		GroupPtr->SetPartGroupIndex(index);
		AddProjectPartGroupPtr_Inline(GroupPtr);
		index ++;
	}		
	return true;	
}
//---------------------------------------------------------------------------------//
bool CAOIProject::SelectProjectAllPartGroups(bool value)//選取專案的所有零件群組
{
	size_t i = 0;	
	CAOIPartGroup *PartGroupPtr = NULL;	
	const size_t PartGroupCount = GetProjectPartGroupCount_Inline();
	for ( i=0; i<PartGroupCount; i++ )
	{
		PartGroupPtr = GetProjectPartGroupPtr_Inline(i);
		if ( NULL == PartGroupPtr ) { continue; }
		PartGroupPtr->SetPartGroupSelected(value);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
CAOIPartGroup* CAOIProject::GetProjectPartGroupPtrBySelected()//取得專案零件群組指標-依據選取到
{
	size_t i = 0;	
	CAOIPartGroup *PartGroupPtr = NULL;	
	const size_t PartGroupCount = GetProjectPartGroupCount_Inline();
	for ( i=0; i<PartGroupCount; i++ )
	{
		PartGroupPtr = GetProjectPartGroupPtr_Inline(i);
		if ( NULL == PartGroupPtr ) { continue; }
		if ( PartGroupPtr->GetPartGroupDeleted() == true ) { continue; }
		if ( PartGroupPtr->GetPartGroupSelected() == false ) { continue; }
		return PartGroupPtr;		
	}		
	return NULL;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::GetProjectPartGroupSelected(std::vector<CAOIPartGroup*> &SelGroupList)//取得專案零件群組選取到
{
	size_t     i = 0;	
	CAOIPartGroup *PartGroupPtr = NULL;	
	const size_t PartGroupCount = GetProjectPartGroupCount_Inline();
	for ( i=0; i<PartGroupCount; i++ )
	{
		PartGroupPtr = GetProjectPartGroupPtr_Inline(i);
		if ( NULL == PartGroupPtr ) { continue; }
		if ( PartGroupPtr->GetPartGroupDeleted() == true ) { continue; }
		if ( PartGroupPtr->GetPartGroupSelected() == false ) { continue; }
		SelGroupList.push_back(PartGroupPtr);
	}		
	return true;
}
//---------------------------------------------------------------------------------//
size_t CAOIProject::GetProjectPartGroupSelectedCount() const//計算專案零件群組選取到數量	
{
	size_t  i = 0;	
	size_t  Count=0;
	CAOIPartGroup *PartGroupPtr = NULL;	
	const size_t PartGroupCount = GetProjectPartGroupCount_Inline();
	for ( i=0; i<PartGroupCount; i++ )
	{
		PartGroupPtr = GetProjectPartGroupPtr_Inline(i);
		if ( NULL == PartGroupPtr ) { continue; }
		if ( PartGroupPtr->GetPartGroupDeleted() == true ) { continue; }
		if ( PartGroupPtr->GetPartGroupSelected() == false ) { continue; }
		Count ++;
	}		
	return Count;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::RemoveProjectPartGroupComponentSelected()//移除專案零件選取到的零件群組
{
	size_t  i = 0;		
	CAOIPartGroup *PartGroupPtr = NULL;	
	const size_t PartGroupCount = GetProjectPartGroupCount_Inline();
	for ( i=0; i<PartGroupCount; i++ )
	{
		PartGroupPtr = GetProjectPartGroupPtr_Inline(i);
		if ( NULL == PartGroupPtr ) { continue; }
		PartGroupPtr->RemovePartGroupNodeComponentSelected();		
	}		
	return true;
}
//---------------------------------------------------------------------------------//
/*
bool CAOIProject::DeleteProjectPartGroupkGroup()//刪除同群組的專案零件群組
{
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectPartGroupSelected()//刪除選取到的專案零件群組
{
}
//---------------------------------------------------------------------------------//
bool CAOIProject::DeleteProjectPartGroupUnselected()//刪除未選取到的專案零件群組
{
}
//---------------------------------------------------------------------------------//
bool CAOIProject::CheckProjectPartGroupValid(const CAOIPartGroup *RefGroupPtr)//確認專案的零件群組指標有效	
{
}
//---------------------------------------------------------------------------------//
bool CAOIProject::PasteProjectPartGroupToOtherBoards(std::vector<CAOIPartGroup*> &CloneGroupList)//將零件群組貼上其餘整板單板上		
{
}
//---------------------------------------------------------------------------------//
bool CAOIProject::UpdateProjectPartGroupToOtherByGroupID(CAOIPartGroup *GroupPtr)//更新專案零件群組至其他群組-依據群組編號
{
}
//---------------------------------------------------------------------------------//
*/
bool CAOIProject::AssignProjectPartGroupToPanelBoard()//分配專案的零件群組至整板與單板
{
	const size_t BoardCount = GetProjectBoardCount();
	for ( size_t i=0; i<BoardCount; i++ )
	{
		CAOIBoard *BoardPtr = GetProjectBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }		
		BoardPtr->RemoveBoardAllPartGroups();
	}

	const size_t PartGroupCount = GetProjectPartGroupCount();
	for ( size_t i=0; i<PartGroupCount; i++ )
	{
		CAOIPartGroup *PartGroupPtr = GetProjectPartGroupPtr(i, false);
		if ( NULL == PartGroupPtr ) { continue; }
		//if ( PartGroupPtr->GetPartGroupDistrictID() != DistrictID ) { continue; }

		if ( PartGroupPtr->CheckPartGroupNodeInOneBoard() )
		{
			CAOIBoard *BoardPtr = PartGroupPtr->GetPartGroupNodeBoardPtr();
			if ( NULL != BoardPtr )
			{	BoardPtr->AddBoardPartGroupPtr(PartGroupPtr); }
		}
	}	
	return true;
}
//---------------------------------------------------------------------------------//
bool CAOIProject::InitialProjectGroupInspection()//初始化專案群組檢測
{	
	size_t         i=0;
	CAOIPartGroup *PartGroupPtr = NULL;
	const size_t   PartGroupCount = GetProjectPartGroupCount_Inline();	
	for ( i=0; i<PartGroupCount; i++ )
	{
		PartGroupPtr = GetProjectPartGroupPtr_Inline(i);
		if ( NULL == PartGroupPtr ) { continue; }
		PartGroupPtr->InitPartGroupInspection();
	}	
	return true;

}
//-------------------------------------------------------------------------------------//
bool CAOIProject::AnalysisProjectGroupInspection()//分析專案群組檢測
{		
	CAOIProject *ProjectPtr = this;
	size_t         i=0, j=0, k=0;
	PART_GROUP_MODE GroupMode;	
	CAOIPartGroup *PartGroupPtr = NULL;
	const size_t   PartGroupCount = GetProjectPartGroupCount_Inline();	
	for ( i=0; i<PartGroupCount; i++ )
	{
		PartGroupPtr = ProjectPtr->GetProjectPartGroupPtr_Inline(i);
		if ( NULL == PartGroupPtr ) { continue; }
		GroupMode = PartGroupPtr->GetPartGroupMode();
		if ( PART_GROUP_COLINEARITY == GroupMode )
		{
			if ( ExecProjectGroupInspection_Colinearity(PartGroupPtr) == false )
			{	return false; }
			continue;
		}
		if ( PART_GROUP_COLINEARITY_TO_LINE == GroupMode )
		{
			if ( ExecProjectGroupInspection_ColinearityToLine(PartGroupPtr) == false )
			{	return false; }
			continue;
		}
		if ( PART_GROUP_DIST_PART_TO_PART == GroupMode )
		{
			if ( ExecProjectGroupInspection_PartToPart(PartGroupPtr) == false )
			{	return false; }
			continue;
		}
		if ( PART_GROUP_DIST_PART_NEIGHBOR == GroupMode )
		{
			if ( ExecProjectGroupInspection_NeighborPart(PartGroupPtr) == false )
			{	return false; }
			continue;
		}
		if ( PART_GROUP_DIST_PART_TO_GROUP == GroupMode )
		{
			if ( ExecProjectGroupInspection_PartToGroup(PartGroupPtr) == false )
			{	return false; }
			continue;
		}
		if ( PART_GROUP_DIST_GROUP_TO_PART == GroupMode )
		{
			if ( ExecProjectGroupInspection_GroupToPart(PartGroupPtr) == false )
			{	return false; }
			continue;
		}
		if ( PART_GROUP_DIST_GROUP_COORD_MAP == GroupMode )
		{
			if ( ExecProjectGroupInspection_GroupCoordMap(PartGroupPtr) == false )
			{	return false; }
			continue;
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::ExecProjectGroupInspection_Colinearity(CAOIPartGroup *GroupPtr)//執行專案群組檢測-直線度
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPartGroupPtr(GroupPtr) == false )
	{	return false; }	
	if ( PART_GROUP_COLINEARITY != GroupPtr->GetPartGroupMode() ) 
	{	return true; }

	size_t         i=0;
	bool           bPass=true;
	int            UsedCount=0;
	CString        ResultText;
	CString        DefectText;
	PART_GROUP_MODE GroupMode;
	TPartGroupNode    *GroupNodePtr=NULL;
	CAOIComponent *ComponentPtr = NULL;	
	TPOINT3D       CadGap, CadAve, CadSum, CadMin;
	TPOINT3D       CadPos, CadOffset, CadResult;
	TPOINT3D       StageGap, StageAve, StageSum;	
	TPOINT3D       StagePos, StageOffset, StageResult;
	double         CadOffsetL1=0, CadOffsetL2=0, CadGapL=0;
	double         CadGapH=0, CadAveH=0, CadSumH=0, CadResultH=0, CadMinH=0;;
	const double   USL = GroupPtr->GetColinearityGapUSL();
	const double   LSL = GroupPtr->GetColinearityGapLSL();	
	const size_t GroupNodeCount=GroupPtr->GetPartGroupNodeCount();
	PART_GROUP_COLINEARITY_MODE ColinearityMode=GroupPtr->GetColinearityMode();
	PART_GROUP_TARGET_MODE ColinearityTargetMode=GroupPtr->GetColinearityTargetMode();

	UsedCount = 0;
	CadSum.x = CadSum.y = CadSum.z = CadSumH = 0;
	StageSum.x = StageSum.y = StageSum.z = 0;
	CadMin.x = CadMin.y = CadMin.z = CadMinH = DBL_MAX;
	for ( i=0; i<GroupNodeCount; i++ )
	{
		GroupNodePtr = GroupPtr->GetPartGroupNodePtr(i, false);
		if ( NULL == GroupNodePtr ) { continue; }
		ComponentPtr = GroupNodePtr->ComponentPtr;
		if ( NULL == ComponentPtr ) { continue; }
		CadPos.x = ComponentPtr->GetComponentCadPosX();
		CadPos.y = ComponentPtr->GetComponentCadPosY();
		StagePos.x = ComponentPtr->GetComponentStagePosX();
		StagePos.y = ComponentPtr->GetComponentStagePosY();

		CadOffset.x = ComponentPtr->GetComponentResultOffsetX();
		CadOffset.y = ComponentPtr->GetComponentResultOffsetY();
		StageOffset.x = ComponentPtr->GetComponentStageOffsetX();
		StageOffset.y = ComponentPtr->GetComponentStageOffsetY();

		CadResult.x = CadPos.x + CadOffset.x; 
		CadResult.y = CadPos.y + CadOffset.y; 
		CadResult.x = ComponentPtr->GetComponentCadResultX();
		CadResult.y = ComponentPtr->GetComponentCadResultY();	
		CadResult.z = ComponentPtr->GetComponentResultSkewAngle();

		StageResult.x = StageOffset.x + StagePos.x; 
		StageResult.y = StageOffset.y + StagePos.y; 
		StageResult.x = ComponentPtr->GetComponentStageResultX(); 
		StageResult.y = ComponentPtr->GetComponentStageResultY(); 
		StageResult.z = ComponentPtr->GetComponentResultSkewAngle();
			
		CadSum.x += CadResult.x;
		CadSum.y += CadResult.y;
		CadSum.z += CadResult.z;

		CadResultH = ComponentPtr->GetComponentResultHeight();
		CadSumH += CadResultH;

		StageSum.x += StageResult.x;
		StageSum.y += StageResult.y;
		StageSum.z += StageResult.z;

		if ( CadMin.x > CadResult.x ) { CadMin.x = CadResult.x;	}
		if ( CadMin.y > CadResult.y ) { CadMin.y = CadResult.y;	}
		if ( CadMin.z > CadResult.z ) { CadMin.z = CadResult.z;	}
		if ( CadMinH > CadResultH ) { CadMinH = CadResultH; }

		UsedCount ++;
	}
	if ( 0 == UsedCount )
	{	return true; }

	CadAve.x = CadSum.x/UsedCount;
	CadAve.y = CadSum.y/UsedCount;
	CadAve.z = CadSum.z/UsedCount;
	CadAveH = CadSumH/UsedCount;
	CadAve.z = 0.0;

	StageAve.x = StageSum.x/UsedCount;
	StageAve.y = StageSum.y/UsedCount;
	StageAve.z = StageSum.z/UsedCount;

	double CadStdH = CadAveH;
	TPOINT3D CadStd = CadAve;
	if ( PART_GROUP_TARGET_MIN == ColinearityTargetMode )
	{	
		CadStd = CadMin;	
		CadStdH = CadMinH;
	}

	for ( i=0; i<GroupNodeCount; i++ )
	{
		GroupNodePtr = GroupPtr->GetPartGroupNodePtr(i, false);
		if ( NULL == GroupNodePtr ) { continue; }
		ComponentPtr = GroupNodePtr->ComponentPtr;
		if ( NULL == ComponentPtr ) { continue; }

		bPass=true;
		CadPos.x = ComponentPtr->GetComponentCadPosX();
		CadPos.y = ComponentPtr->GetComponentCadPosY();
		StagePos.x = ComponentPtr->GetComponentStagePosX();
		StagePos.y = ComponentPtr->GetComponentStagePosY();

		CadOffset.x = ComponentPtr->GetComponentResultOffsetX();
		CadOffset.y = ComponentPtr->GetComponentResultOffsetY();
		StageOffset.x = ComponentPtr->GetComponentStageOffsetX();
		StageOffset.y = ComponentPtr->GetComponentStageOffsetY();

		CadResult.x = CadPos.x + CadOffset.x; 
		CadResult.y = CadPos.y + CadOffset.y; 
		CadResult.x = ComponentPtr->GetComponentCadResultX();
		CadResult.y = ComponentPtr->GetComponentCadResultY();	
		CadResult.z = ComponentPtr->GetComponentResultSkewAngle();
		CadResultH = ComponentPtr->GetComponentResultHeight();

		StageResult.x = StageOffset.x + StagePos.x; 
		StageResult.y = StageOffset.y + StagePos.y; 
		StageResult.x = ComponentPtr->GetComponentStageResultX(); 
		StageResult.y = ComponentPtr->GetComponentStageResultY(); 
		StageResult.z = ComponentPtr->GetComponentResultSkewAngle();
			
		CadGap.x = CadResult.x-CadStd.x;
		CadGap.y = CadResult.y-CadStd.y;
		CadGap.z = CadResult.z-CadStd.z;
		CadGapH = CadResultH-CadStdH;

		StageGap.x = StageResult.x-StageAve.x;
		StageGap.y = StageResult.y-StageAve.y;
		StageGap.z = StageResult.z-StageAve.z;
		
		GroupNodePtr->ResultID = RESULT_ID_OK;
		GroupNodePtr->ResultStdX = CadStd.x;
		GroupNodePtr->ResultStdY = CadStd.y;
		GroupNodePtr->ResultSkew = CadStd.z;
		GroupNodePtr->ResultStdHeight = CadStdH;

		GroupNodePtr->ResultGapX = CadGap.x;
		GroupNodePtr->ResultGapY = CadGap.y;
		GroupNodePtr->ResultGapSkew = CadGap.z;
		GroupNodePtr->ResultGapHeight = CadGapH;

		ResultText = DefectText = _T("");
		if ( PART_GROUP_COLINEARITY_X == ColinearityMode )
		{
			if ( CadGap.x > 0 )
			{	ResultText.Format(_T("X:%.0f + %.0f"), CadStd.x, CadGap.x); }
			else
			{	ResultText.Format(_T("X:%.0f - %.0f"), CadStd.x, -CadGap.x); }
			if ( CadGap.x>USL || CadGap.x<LSL ) 
			{	
				bPass = false;	
				DefectText = ResultText;
			}
		}
		if ( PART_GROUP_COLINEARITY_Y == ColinearityMode )
		{
			if ( CadGap.y > 0 )
			{	ResultText.Format(_T("Y:%.0f + %.0f"), CadStd.y, CadGap.y); }
			else
			{	ResultText.Format(_T("Y:%.0f - %.0f"), CadStd.y, -CadGap.y); }
			if ( CadGap.y>USL || CadGap.y<LSL ) 
			{	
				bPass = false;	
				DefectText = ResultText;
			}
		}	
		if ( PART_GROUP_COLINEARITY_SKEW == ColinearityMode )
		{
			if ( CadGap.z > 0 )
			{	ResultText.Format(_T("Angle:%.2f + %.2f"), CadStd.z, CadGap.z); }
			else
			{	ResultText.Format(_T("Angle:%.2f - %.2f"), CadStd.z, -CadGap.z); }
			if ( CadGap.z>USL || CadGap.z<LSL ) 
			{	
				bPass = false;	
				DefectText = ResultText;
			}
		}	
		if ( PART_GROUP_COLINEARITY_HEIGHT == ColinearityMode )
		{
			if ( CadGapH > 0 )
			{	ResultText.Format(_T("Height:%.2f + %.2f"), CadStdH, CadGapH); }
			else
			{	ResultText.Format(_T("Height:%.2f - %.2f"), CadStdH, -CadGapH); }
			if ( CadGapH>USL || CadGapH<LSL ) 
			{	
				bPass = false;	
				DefectText = ResultText;
			}
		}
		GroupNodePtr->ResultText = ResultText;
		if ( true == bPass )
		{	continue; }		
		//Set Component NG		
		GroupNodePtr->ResultID = RESULT_ID_NG;
		GroupNodePtr->ResultText = DefectText;
		const size_t WndIndex=GroupNodePtr->ModelWndIndex;
		DefectText = GroupPtr->BuildPartGroupDefectText(DefectText);
		ComponentPtr->SetComponentGroupResult(WndIndex, RESULT_ID_NG, DefectText);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::ExecProjectGroupInspection_ColinearityToLine(CAOIPartGroup *GroupPtr)//執行專案群組檢測-共線性-對線
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPartGroupPtr(GroupPtr) == false )
	{	return false; }	
	if ( PART_GROUP_COLINEARITY_TO_LINE != GroupPtr->GetPartGroupMode() ) 
	{	return true; }

	size_t         i=0;
	bool           bPass=true;
	double         CadGapL=0;
	CString        ResultText;
	CString        DefectText;	
	PART_GROUP_MODE GroupMode;
	TPartGroupNode *GroupNodePtr=NULL;
	TPartGroupNode *GroupNodePtr1=NULL;
	TPartGroupNode *GroupNodePtr2=NULL;
	CAOIComponent *ComponentPtr = NULL;	
	CAOIComponent *ComponentPtr1 = NULL;	
	CAOIComponent *ComponentPtr2 = NULL;
	const double   Std = GroupPtr->GetColinearityGapStd();
	const double   USL = GroupPtr->GetColinearityGapUSL();
	const double   LSL = GroupPtr->GetColinearityGapLSL();	
	const size_t GroupNodeCount=GroupPtr->GetPartGroupNodeCount();	
	
	GroupNodePtr1 = GroupPtr->GetPartGroupNodePtr1();
	GroupNodePtr2 = GroupPtr->GetPartGroupNodePtr2();
	ComponentPtr1 = GroupNodePtr1->ComponentPtr;
	ComponentPtr2 = GroupNodePtr2->ComponentPtr;
	if ( NULL==ComponentPtr1 || NULL==ComponentPtr2 )
	{
		m_ErrorString = _T("Error, ExecProjectGroupInspection_ColinearityToLine Fault(ComponentPtr==NULL)");
		return false;
	}

	TLINE2D  MarkLine, OrthLine;
	TPOINT2D MarkPt1, MarkPt2, PartPt, OrthPt;
	MarkPt1.x = ComponentPtr1->GetComponentCadResultX();
	MarkPt1.y = ComponentPtr1->GetComponentCadResultY();
	MarkPt2.x = ComponentPtr2->GetComponentCadResultX();
	MarkPt2.y = ComponentPtr2->GetComponentCadResultY();
	JetAPI::Calc2PointLine(MarkPt1, MarkPt2, MarkLine);	

	for ( i=0; i<GroupNodeCount; i++ )
	{
		GroupNodePtr = GroupPtr->GetPartGroupNodePtr(i, false);
		if ( NULL == GroupNodePtr ) { continue; }
		ComponentPtr = GroupNodePtr->ComponentPtr;
		if ( NULL == ComponentPtr ) { continue; }

		bPass=true;		
		PartPt.x = ComponentPtr->GetComponentCadResultX();
		PartPt.y = ComponentPtr->GetComponentCadResultY();
		JetAPI::CalcOrthogonalLine(PartPt, MarkLine, OrthLine);
		JetAPI::Calc2LinePoint(MarkLine, OrthLine, OrthPt);
		CadGapL = JetAPI::CalcDistance(PartPt, OrthPt);
		CadGapL -= Std;

		GroupNodePtr->ResultID = RESULT_ID_OK;
		GroupNodePtr->ResultStdL = Std;
		GroupNodePtr->ResultGapL = CadGapL;

		ResultText = DefectText = _T("");
		if ( CadGapL> 0 )
		{	ResultText.Format(_T("L:%.0f + %.0f"), Std, CadGapL); }
		else
		{	ResultText.Format(_T("L:%.0f - %.0f"), Std, -CadGapL); }
		if ( CadGapL>USL || CadGapL<LSL ) 
		{	
			bPass = false;	
			DefectText = ResultText;
		}		
		GroupNodePtr->ResultText = ResultText;		
		if ( true == bPass )
		{	continue; }
		//Set Component NG		
		GroupNodePtr->ResultID = RESULT_ID_NG;
		GroupNodePtr->ResultText = DefectText;
		const size_t WndIndex=GroupNodePtr->ModelWndIndex;
		DefectText = GroupPtr->BuildPartGroupDefectText(DefectText);
		ComponentPtr->SetComponentGroupResult(WndIndex, RESULT_ID_NG, DefectText);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::ExecProjectGroupInspection_PartToPart(CAOIPartGroup *GroupPtr)//執行專案群組檢測-點對點
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPartGroupPtr(GroupPtr) == false )
	{	return false; }	
	if ( PART_GROUP_DIST_PART_TO_PART != GroupPtr->GetPartGroupMode() ) 
	{	return true; }

	size_t          i=0;
	bool            bPass=true;
	int             UsedCount=0;
	CString         ResultText;
	CString         DefectText;
	CString         DefectTextFull;
	TPartGroupNode *GroupNodePtr1=NULL;
	TPartGroupNode *GroupNodePtr2=NULL;
	CAOIComponent  *ComponentPtr1 = NULL;	
	CAOIComponent  *ComponentPtr2 = NULL;
	TPOINT3D        CadGap, StageGap;
	TPOINT3D        CadPos1, CadOffset1, CadResult1;
	TPOINT3D        CadPos2, CadOffset2, CadResult2;	
	TPOINT3D        StagePos1, StageOffset1, StageResult1;
	TPOINT3D        StagePos2, StageOffset2, StageResult2;
	double          CadOffsetL1=0, CadOffsetL2=0, CadGapL=0;
	const bool     EnbX = GroupPtr->GetDistanceGapEnbX();
	const bool     EnbY = GroupPtr->GetDistanceGapEnbY();
	const bool     EnbL = GroupPtr->GetDistanceGapEnbL();
	const double   USLX = GroupPtr->GetDistanceGapUSLX();
	const double   LSLX = GroupPtr->GetDistanceGapLSLX();
	const double   USLY = GroupPtr->GetDistanceGapUSLY();
	const double   LSLY = GroupPtr->GetDistanceGapLSLY();
	const double   USLL = GroupPtr->GetDistanceGapUSLL();
	const double   LSLL = GroupPtr->GetDistanceGapLSLL();
	const size_t GroupNodeCount=GroupPtr->GetPartGroupNodeCount();	
	
	GroupNodePtr1 = GroupPtr->GetPartGroupNodePtr1();
	if ( NULL == GroupNodePtr1 ) { return false; }
	ComponentPtr1 = GroupNodePtr1->ComponentPtr;
	GroupNodePtr2 = GroupPtr->GetPartGroupNodePtr2();
	if ( NULL == GroupNodePtr2 ) { return false; }
	ComponentPtr2 = GroupNodePtr2->ComponentPtr;
	if ( NULL==ComponentPtr1 || NULL==ComponentPtr2 )
	{
		m_ErrorString = _T("Error, ExecProjectGroupInspection_PartToPart Fault(ComponentPtr==NULL)");
		return false;
	}
	
	CadPos1.x = ComponentPtr1->GetComponentCadPosX();
	CadPos1.y = ComponentPtr1->GetComponentCadPosY();
	StagePos1.x = ComponentPtr1->GetComponentStagePosX();
	StagePos1.y = ComponentPtr1->GetComponentStagePosY();
	
	CadResult1.x = ComponentPtr1->GetComponentCadResultX(); 
	CadResult1.y = ComponentPtr1->GetComponentCadResultY(); 
	CadResult1.z = ComponentPtr1->GetComponentResultSkewAngle();
	StageResult1.x = ComponentPtr1->GetComponentStageResultX(); 
	StageResult1.y = ComponentPtr1->GetComponentStageResultY(); 
	StageResult1.z = ComponentPtr1->GetComponentResultSkewAngle();				
	
	CadPos2.x = ComponentPtr2->GetComponentCadPosX();
	CadPos2.y = ComponentPtr2->GetComponentCadPosY();
	StagePos2.x = ComponentPtr2->GetComponentStagePosX();
	StagePos2.y = ComponentPtr2->GetComponentStagePosY();

	CadResult2.x = ComponentPtr2->GetComponentCadResultX(); 
	CadResult2.y = ComponentPtr2->GetComponentCadResultY(); 
	CadResult2.z = ComponentPtr2->GetComponentResultSkewAngle();
	StageResult2.x = ComponentPtr2->GetComponentStageResultX(); 
	StageResult2.y = ComponentPtr2->GetComponentStageResultY(); 
	StageResult2.z = ComponentPtr2->GetComponentResultSkewAngle();	
			
	CadOffset1.x = CadPos2.x-CadPos1.x;
	CadOffset1.y = CadPos2.y-CadPos1.y;
	CadOffset1.z = CadPos2.z-CadPos1.z;	
	CadOffset2.x = CadResult2.x-CadResult1.x;
	CadOffset2.y = CadResult2.y-CadResult1.y;
	CadOffset2.z = CadResult2.z-CadResult1.z;

	//Std
	if ( GroupPtr->GetDistanceGapStdEnb() == true )
	{
		CadOffset1.x = GroupPtr->GetDistanceGapStdX();
		CadOffset1.y = GroupPtr->GetDistanceGapStdY();
	}
	//Abs
	if ( GroupPtr->GetDistanceGapEnbAbs() == true )
	{
		JetAPI::AbsPoint(CadOffset1);
		JetAPI::AbsPoint(CadOffset2);		
	}
	//Add
	CadOffset2.x += GroupPtr->GetDistanceGapAddX();
	CadOffset2.y += GroupPtr->GetDistanceGapAddY();

	CadOffsetL1 = JetAPI::CalcDistance(CadOffset1.x, CadOffset1.y);
	CadOffsetL2 = JetAPI::CalcDistance(CadOffset2.x, CadOffset2.y);

	StageOffset1.x = StagePos2.x-StagePos1.x;
	StageOffset1.y = StagePos2.y-StagePos1.y;
	StageOffset1.z = StagePos2.z-StagePos1.z;
	StageOffset2.x = StageResult2.x-StageResult1.x;
	StageOffset2.y = StageResult2.y-StageResult1.y;
	StageOffset2.z = StageResult2.z-StageResult1.z;

	CadGapL = CadOffsetL2-CadOffsetL1;
	CadGap.x = CadOffset2.x-CadOffset1.x;
	CadGap.y = CadOffset2.y-CadOffset1.y;
	CadGap.z = CadOffset2.z-CadOffset1.z;
	StageGap.x = StageOffset2.x-StageOffset1.x;
	StageGap.y = StageOffset2.y-StageOffset1.y;
	StageGap.z = StageOffset2.z-StageOffset1.z;	

	GroupNodePtr2->ResultID = RESULT_ID_OK;
	GroupNodePtr2->ResultStdX = CadOffset1.x;
	GroupNodePtr2->ResultStdY = CadOffset1.y;	
	GroupNodePtr2->ResultStdL = CadOffsetL1;
	GroupNodePtr2->ResultGapSkew = CadOffset2.z;	

	GroupNodePtr2->ResultGapX = CadGap.x;
	GroupNodePtr2->ResultGapY = CadGap.y;
	GroupNodePtr2->ResultGapL = CadGapL;
	GroupNodePtr2->ResultGapSkew = CadGap.z;

	DefectTextFull = _T("");
	ResultText = DefectText = _T("");
	if ( true == EnbX )
	{	
		if ( CadGap.x > 0 ) 
		{	ResultText.Format(_T("X:%.0f + %.0f"), CadOffset1.x, CadGap.x);	}
		else
		{	ResultText.Format(_T("X:%.0f - %.0f"), CadOffset1.x, -CadGap.x); }		
		if ( CadGap.x>USLX || CadGap.x<LSLX ) 
		{	
			bPass = false; 						
			DefectText = ResultText;
			JetAPI::AddSubString(DefectTextFull, ResultText, _T(": "));
		}
	}
	if ( true == EnbY )
	{
		if ( CadGap.y > 0 ) 
		{	ResultText.Format(_T("Y:%.0f + %.0f"), CadOffset1.y, CadGap.y);	}
		else
		{	ResultText.Format(_T("Y:%.0f - %.0f"), CadOffset1.y, -CadGap.y); }		
		if ( CadGap.y>USLY || CadGap.y<LSLY ) 
		{	
			bPass = false; 
			DefectText = ResultText;
			JetAPI::AddSubString(DefectTextFull, ResultText, _T(": "));			
		}
	}	
	if ( true == EnbL )
	{		
		if ( CadGapL > 0 ) 
		{	ResultText.Format(_T("L:%.0f + %.0f"), CadOffsetL1, CadGapL);	}
		else
		{	ResultText.Format(_T("L:%.0f - %.0f"), CadOffsetL1, -CadGapL); }		
		if ( CadGapL>USLL || CadGapL<LSLL ) 
		{	
			bPass = false; 
			DefectText = ResultText;
			JetAPI::AddSubString(DefectTextFull, ResultText, _T(": "));
		}
	}
	ResultText = DefectText = DefectTextFull;

	GroupNodePtr2->ResultText = ResultText;	
	if ( false == bPass )
	{		
		GroupNodePtr2->ResultID = RESULT_ID_NG;
		GroupNodePtr2->ResultText = DefectText;
		const size_t WndIndex=GroupNodePtr2->ModelWndIndex;
		DefectText = GroupPtr->BuildPartGroupDefectText(DefectText);
		ComponentPtr2->SetComponentGroupResult(WndIndex, RESULT_ID_NG, DefectText);				
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::ExecProjectGroupInspection_NeighborPart(CAOIPartGroup *GroupPtr)//執行專案群組檢測-相鄰點
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPartGroupPtr(GroupPtr) == false )
	{	return false; }	
	if ( PART_GROUP_DIST_PART_NEIGHBOR != GroupPtr->GetPartGroupMode() ) 
	{	return true; }

	size_t         i=0;
	bool           bPass=true;
	int            UsedCount=0;
	CString        ResultText;
	CString        DefectText;
	PART_GROUP_MODE GroupMode;
	TPartGroupNode    *GroupNodePtr1=NULL;
	TPartGroupNode    *GroupNodePtr2=NULL;
	CAOIComponent *ComponentPtr1 = NULL;	
	CAOIComponent *ComponentPtr2 = NULL;	
	TPOINT3D       CadGap, StageGap;
	TPOINT3D       CadPos1, CadOffset1, CadResult1;
	TPOINT3D       CadPos2, CadOffset2, CadResult2;	
	TPOINT3D       StagePos1, StageOffset1, StageResult1;
	TPOINT3D       StagePos2, StageOffset2, StageResult2;
	double         CadOffsetL1=0, CadOffsetL2=0, CadGapL=0;
	const bool     EnbX = GroupPtr->GetDistanceGapEnbX();
	const bool     EnbY = GroupPtr->GetDistanceGapEnbY();
	const bool     EnbL = GroupPtr->GetDistanceGapEnbL();
	const double   USLX = GroupPtr->GetDistanceGapUSLX();
	const double   LSLX = GroupPtr->GetDistanceGapLSLX();
	const double   USLY = GroupPtr->GetDistanceGapUSLY();
	const double   LSLY = GroupPtr->GetDistanceGapLSLY();
	const double   USLL = GroupPtr->GetDistanceGapUSLL();
	const double   LSLL = GroupPtr->GetDistanceGapLSLL();
	const size_t GroupNodeCount=GroupPtr->GetPartGroupNodeCount();	

	if ( GroupNodeCount < 2 ) 
	{	
		m_ErrorString = _T("Error, ExecProjectGroupInspection_NeighborPart Fault(GroupNodeCount < 2)");
		return false; 
	}

	for ( i=0; i<GroupNodeCount-1; i++ )
	{
		GroupNodePtr1 = GroupPtr->GetPartGroupNodePtr(i, false);
		GroupNodePtr2 = GroupPtr->GetPartGroupNodePtr(i+1, false);
		if ( NULL == GroupNodePtr1 ) { continue; }
		if ( NULL == GroupNodePtr2 ) { continue; }
		ComponentPtr1 = GroupNodePtr1->ComponentPtr;
		ComponentPtr2 = GroupNodePtr2->ComponentPtr;
		if ( NULL==ComponentPtr1 || NULL==ComponentPtr2 )
		{
			m_ErrorString = _T("Error, ExecProjectGroupInspection_NeighborPart Fault(ComponentPtr==NULL)");
			return false;
		}
	
		bPass = true;
		CadPos1.x = ComponentPtr1->GetComponentCadPosX();
		CadPos1.y = ComponentPtr1->GetComponentCadPosY();
		StagePos1.x = ComponentPtr1->GetComponentStagePosX();
		StagePos1.y = ComponentPtr1->GetComponentStagePosY();
	
		CadResult1.x = ComponentPtr1->GetComponentCadResultX(); 
		CadResult1.y = ComponentPtr1->GetComponentCadResultY(); 
		CadResult1.z = ComponentPtr1->GetComponentResultSkewAngle();
		StageResult1.x = ComponentPtr1->GetComponentStageResultX(); 
		StageResult1.y = ComponentPtr1->GetComponentStageResultY(); 
		StageResult1.z = ComponentPtr1->GetComponentResultSkewAngle();			
	
		CadPos2.x = ComponentPtr2->GetComponentCadPosX();
		CadPos2.y = ComponentPtr2->GetComponentCadPosY();
		StagePos2.x = ComponentPtr2->GetComponentStagePosX();
		StagePos2.y = ComponentPtr2->GetComponentStagePosY();

		CadResult2.x = ComponentPtr2->GetComponentCadResultX(); 
		CadResult2.y = ComponentPtr2->GetComponentCadResultY(); 
		CadResult2.z = ComponentPtr2->GetComponentResultSkewAngle();
		StageResult2.x = ComponentPtr2->GetComponentStageResultX(); 
		StageResult2.y = ComponentPtr2->GetComponentStageResultY(); 
		StageResult2.z = ComponentPtr2->GetComponentResultSkewAngle();	
			
		CadOffset1.x = CadPos2.x-CadPos1.x;
		CadOffset1.y = CadPos2.y-CadPos1.y;
		CadOffset1.z = CadPos2.z-CadPos1.z;
		CadOffset2.x = CadResult2.x-CadResult1.x;
		CadOffset2.y = CadResult2.y-CadResult1.y;
		CadOffset2.z = CadResult2.z-CadResult1.z;
		CadOffsetL1 = JetAPI::CalcDistance(CadOffset1.x, CadOffset1.y);
		CadOffsetL2 = JetAPI::CalcDistance(CadOffset2.x, CadOffset2.y);

		StageOffset1.x = StagePos2.x-StagePos1.x;
		StageOffset1.y = StagePos2.y-StagePos1.y;
		StageOffset1.z = StagePos2.z-StagePos1.z;
		StageOffset2.x = StageResult2.x-StageResult1.x;
		StageOffset2.y = StageResult2.y-StageResult1.y;
		StageOffset2.z = StageResult2.z-StageResult1.z;

		CadGapL = CadOffsetL2-CadOffsetL1;
		CadGap.x = CadOffset2.x-CadOffset1.x;
		CadGap.y = CadOffset2.y-CadOffset1.y;
		CadGap.z = CadOffset2.z-CadOffset1.z;
		StageGap.x = StageOffset2.x-StageOffset1.x;
		StageGap.y = StageOffset2.y-StageOffset1.y;
		StageGap.z = StageOffset2.z-StageOffset1.z;

		GroupNodePtr2->ResultID = RESULT_ID_OK;
		GroupNodePtr2->ResultStdX = CadOffset1.x;
		GroupNodePtr2->ResultStdY = CadOffset1.y;
		GroupNodePtr2->ResultStdL = CadOffsetL1;
		GroupNodePtr2->ResultGapSkew = CadOffset2.z;

		GroupNodePtr2->ResultGapX = CadGap.x;
		GroupNodePtr2->ResultGapY = CadGap.y;
		GroupNodePtr2->ResultGapL = CadGapL;
		GroupNodePtr2->ResultGapSkew = CadGap.z;

		ResultText = DefectText = _T("");
		if ( true == EnbX )
		{
			if ( CadGap.x > 0 ) 
			{	ResultText.Format(_T("X:%.0f + %.0f"), CadOffset1.x, CadGap.x);	}
			else
			{	ResultText.Format(_T("X:%.0f - %.0f"), CadOffset1.x, -CadGap.x); }
			if ( CadGap.x>USLX || CadGap.x<LSLX ) 
			{	
				bPass = false; 
				DefectText = ResultText;
			}
		}
		if ( true == EnbY )
		{
			if ( CadGap.y > 0 ) 
			{	ResultText.Format(_T("Y:%.0f + %.0f"), CadOffset1.y, CadGap.y);	}
			else
			{	ResultText.Format(_T("Y:%.0f - %.0f"), CadOffset1.y, -CadGap.y); }	
			if ( CadGap.y>USLY || CadGap.y<LSLY ) 
			{	
				bPass = false; 
				DefectText = ResultText;
			}
		}	
		if ( true == EnbL )
		{		
			if ( CadGapL > 0 ) 
			{	ResultText.Format(_T("L:%.0f + %.0f"), CadOffsetL1, CadGapL);	}
			else
			{	ResultText.Format(_T("L:%.0f - %.0f"), CadOffsetL1, -CadGapL); }	
			if ( CadGapL>USLL || CadGapL<LSLL ) 
			{	
				bPass = false; 
				DefectText = ResultText;
			}
		}
		GroupNodePtr2->ResultText = ResultText;
		if ( true == bPass )
		{	continue;	}

		//Set NG Component
		GroupNodePtr2->ResultID = RESULT_ID_NG;
		GroupNodePtr2->ResultText = DefectText;
		const size_t WndIndex=GroupNodePtr2->ModelWndIndex;
		DefectText = GroupPtr->BuildPartGroupDefectText(DefectText);
		ComponentPtr2->SetComponentGroupResult(WndIndex, RESULT_ID_NG, DefectText);	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::ExecProjectGroupInspection_PartToGroup(CAOIPartGroup *GroupPtr)//執行專案群組檢測-點對群組
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPartGroupPtr(GroupPtr) == false )
	{	return false; }
	if ( PART_GROUP_DIST_PART_TO_GROUP != GroupPtr->GetPartGroupMode() ) 
	{	return true; }

	size_t         i=0;
	int            OrgCount=0;
	bool           bPass=true;
	CString        ResultText;
	CString        DefectText;
	PART_GROUP_MODE GroupMode;	
	TPartGroupNode    *GroupNodePtr=NULL;
	CAOIComponent *ComponentPtr = NULL;		
	TPOINT3D       CadOrg, CadOrg2, CadGap;
	TPOINT3D       CadPos, CadOffset1, CadOffset2, CadResult;	
	TPOINT3D       StageOrg, StageOrg2, StageGap;	
	TPOINT3D       StagePos, StageOffset1, StageOffset2, StageResult;	
	double         CadOffsetL1=0, CadOffsetL2=0, CadGapL=0;
	const bool     EnbX = GroupPtr->GetDistanceGapEnbX();
	const bool     EnbY = GroupPtr->GetDistanceGapEnbY();
	const bool     EnbL = GroupPtr->GetDistanceGapEnbL();
	const double   USLX = GroupPtr->GetDistanceGapUSLX();
	const double   LSLX = GroupPtr->GetDistanceGapLSLX();
	const double   USLY = GroupPtr->GetDistanceGapUSLY();
	const double   LSLY = GroupPtr->GetDistanceGapLSLY();
	const double   USLL = GroupPtr->GetDistanceGapUSLL();
	const double   LSLL = GroupPtr->GetDistanceGapLSLL();
	const size_t GroupNodeCount=GroupPtr->GetPartGroupNodeCount();	

	//Component Org	
	for ( i=0; i<GroupNodeCount; i++ )
	{
		GroupNodePtr = GroupPtr->GetPartGroupNodePtr(i, false);
		if ( NULL == GroupNodePtr ) { continue; }
		ComponentPtr = GroupNodePtr->ComponentPtr;
		if ( NULL == ComponentPtr ) { continue; }

		CadOrg.x += ComponentPtr->GetComponentCadPosX();
		CadOrg.y += ComponentPtr->GetComponentCadPosY();
		CadOrg.z += ComponentPtr->GetComponentResultSkewAngle();
		StageOrg.x += ComponentPtr->GetComponentStagePosX();
		StageOrg.y += ComponentPtr->GetComponentStagePosY();
		StageOrg.z += ComponentPtr->GetComponentResultSkewAngle();

		CadOrg2.x += ComponentPtr->GetComponentCadResultX(); 
		CadOrg2.y += ComponentPtr->GetComponentCadResultY(); 
		CadOrg2.z += ComponentPtr->GetComponentResultSkewAngle();
		StageOrg2.x += ComponentPtr->GetComponentStageResultX(); 
		StageOrg2.y += ComponentPtr->GetComponentStageResultY(); 
		StageOrg2.z += ComponentPtr->GetComponentResultSkewAngle();		
		OrgCount ++;
	}
	if ( 0 == OrgCount ) 
	{
		m_ErrorString = _T("Error, ExecProjectGroupInspection_PartToGroup Fault(0==OrgCount)");
		return false;	
	}
	CadOrg.x /= OrgCount;
	CadOrg.y /= OrgCount;
	CadOrg.z /= OrgCount;
	StageOrg.x /= OrgCount;
	StageOrg.y /= OrgCount;
	StageOrg.z /= OrgCount;

	CadOrg2.x /= OrgCount;
	CadOrg2.y /= OrgCount;
	CadOrg2.z /= OrgCount;
	StageOrg2.x /= OrgCount;
	StageOrg2.y /= OrgCount;
	StageOrg2.z /= OrgCount;
	
	GroupNodePtr = GroupPtr->GetPartGroupNodePtr1();
	if ( NULL == GroupNodePtr )
	{ 
		m_ErrorString = _T("Error, ExecProjectGroupInspection_PartToGroup Fault(NULL==GroupNodePtr)");
		return false;
	}
	bPass=true;	
	ComponentPtr = GroupNodePtr->ComponentPtr;
	if ( NULL == ComponentPtr )
	{ 
		m_ErrorString = _T("Error, ExecProjectGroupInspection_PartToGroup Fault(NULL==ComponentPtr)");
		return false;
	}
	CadPos.x = ComponentPtr->GetComponentCadPosX();
	CadPos.y = ComponentPtr->GetComponentCadPosY();
	StagePos.x = ComponentPtr->GetComponentStagePosX();
	StagePos.y = ComponentPtr->GetComponentStagePosY();

	CadResult.x = ComponentPtr->GetComponentCadResultX(); 
	CadResult.y = ComponentPtr->GetComponentCadResultY(); 
	CadResult.z = ComponentPtr->GetComponentResultSkewAngle();
	StageResult.x = ComponentPtr->GetComponentStageResultX(); 
	StageResult.y = ComponentPtr->GetComponentStageResultY(); 
	StageResult.z = ComponentPtr->GetComponentResultSkewAngle();
		
	//原來座標差距
	CadOffset1.x = CadPos.x-CadOrg.x;
	CadOffset1.y = CadPos.y-CadOrg.y;
	CadOffset1.z = CadPos.z-CadOrg.z;
	StageOffset1.x = StagePos.x-StageOrg.x;
	StageOffset1.y = StagePos.y-StageOrg.y;
	StageOffset1.z = StagePos.z-StageOrg.z;

	//實際座標差距
	CadOffset2.x = CadResult.x-CadOrg2.x;
	CadOffset2.y = CadResult.y-CadOrg2.y;
	CadOffset2.z = CadResult.z-CadOrg2.z;	
	StageOffset2.x = StageResult.x-StageOrg2.x;
	StageOffset2.y = StageResult.y-StageOrg2.y;
	StageOffset2.z = StageResult.z-StageOrg2.z;

	CadOffsetL1 = JetAPI::CalcDistance(CadOffset1.x, CadOffset1.y);
	CadOffsetL2 = JetAPI::CalcDistance(CadOffset2.x, CadOffset2.y);

	//相對偏移量
	CadGapL = CadOffsetL2-CadOffsetL1;
	CadGap.x = CadOffset2.x-CadOffset1.x;
	CadGap.y = CadOffset2.y-CadOffset1.y;
	CadGap.z = CadOffset2.z-CadOffset1.z;
	StageGap.x = StageOffset2.x-StageOffset1.x;
	StageGap.y = StageOffset2.y-StageOffset1.y;
	StageGap.z = StageOffset2.z-StageOffset1.z;

	GroupNodePtr->ResultID = RESULT_ID_OK;
	GroupNodePtr->ResultStdX = CadOffset1.x;
	GroupNodePtr->ResultStdY = CadOffset1.y;
	GroupNodePtr->ResultStdL = CadOffsetL1;
	GroupNodePtr->ResultGapSkew = CadOffset2.z;

	GroupNodePtr->ResultGapX = CadGap.x;
	GroupNodePtr->ResultGapY = CadGap.y;
	GroupNodePtr->ResultGapL = CadGapL;
	GroupNodePtr->ResultGapSkew = CadGap.z;

	ResultText = DefectText = _T("");
	if ( true == EnbX )
	{
		if ( CadGap.x > 0 ) 
		{	ResultText.Format(_T("X:%.0f + %.0f"), CadOffset1.x, CadGap.x);	}
		else
		{	ResultText.Format(_T("X:%.0f - %.0f"), CadOffset1.x, -CadGap.x); }		
		if ( CadGap.x>USLX || CadGap.x<LSLX ) 
		{	
			bPass = false; 
			DefectText = ResultText;
		}
	}
	if ( true == EnbY )
	{
		if ( CadGap.y > 0 ) 
		{	ResultText.Format(_T("Y:%.0f + %.0f"), CadOffset1.y, CadGap.y);	}
		else
		{	ResultText.Format(_T("Y:%.0f - %.0f"), CadOffset1.y, -CadGap.y); }		
		if ( CadGap.y>USLY || CadGap.y<LSLY ) 
		{	
			bPass = false; 
			DefectText = ResultText;
		}
	}	
	if ( true == EnbL )
	{		
		if ( CadGapL > 0 ) 
		{	ResultText.Format(_T("L:%.0f + %.0f"), CadOffsetL1, CadGapL);	}
		else
		{	ResultText.Format(_T("L:%.0f - %.0f"), CadOffsetL1, -CadGapL); }	
		if ( CadGapL>USLL || CadGapL<LSLL ) 
		{	
			bPass = false; 
			DefectText = ResultText;
		}
	}
	
	GroupNodePtr->ResultText = ResultText;
	if ( false == bPass )
	{
		//Set Component NG
		GroupNodePtr->ResultID = RESULT_ID_NG;
		GroupNodePtr->ResultText = DefectText;
		const size_t WndIndex=GroupNodePtr->ModelWndIndex;
		DefectText = GroupPtr->BuildPartGroupDefectText(DefectText);
		ComponentPtr->SetComponentGroupResult(WndIndex, RESULT_ID_NG, DefectText);	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::ExecProjectGroupInspection_GroupToPart(CAOIPartGroup *GroupPtr)//執行專案群組檢測-群組對點
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPartGroupPtr(GroupPtr) == false )
	{	return false; }
	if ( PART_GROUP_DIST_GROUP_TO_PART != GroupPtr->GetPartGroupMode() ) 
	{	return true; }

	size_t         i=0;	
	bool           bPass=true;
	CString        ResultText;
	CString        DefectText;
	PART_GROUP_MODE GroupMode;	
	TPartGroupNode    *GroupNodePtr=NULL;
	CAOIComponent *ComponentPtr = NULL;	
	CAOIComponent *ComponentPtr_Org = NULL;	
	TPOINT3D       CadOrg, CadOrg2, CadGap;
	TPOINT3D       CadPos, CadOffset1, CadOffset2, CadResult;	
	TPOINT3D       StageOrg, StageOrg2, StageGap;	
	TPOINT3D       StagePos, StageOffset1, StageOffset2, StageResult;	
	double         CadOffsetL1=0, CadOffsetL2=0, CadGapL=0;
	const bool     EnbX = GroupPtr->GetDistanceGapEnbX();
	const bool     EnbY = GroupPtr->GetDistanceGapEnbY();
	const bool     EnbL = GroupPtr->GetDistanceGapEnbL();
	const double   USLX = GroupPtr->GetDistanceGapUSLX();
	const double   LSLX = GroupPtr->GetDistanceGapLSLX();
	const double   USLY = GroupPtr->GetDistanceGapUSLY();
	const double   LSLY = GroupPtr->GetDistanceGapLSLY();
	const double   USLL = GroupPtr->GetDistanceGapUSLL();
	const double   LSLL = GroupPtr->GetDistanceGapLSLL();
	const size_t GroupNodeCount=GroupPtr->GetPartGroupNodeCount();
	
	//Component Org
	GroupNodePtr = GroupPtr->GetPartGroupNodePtr1();
	ComponentPtr_Org = GroupNodePtr->ComponentPtr;
	if ( NULL == ComponentPtr_Org )
	{
		m_ErrorString = _T("Error, ExecProjectGroupInspection_GroupToPart Fault(NULL == ComponentPtr_Org)");
		return false;
	}
	CadOrg.x = ComponentPtr_Org->GetComponentCadPosX();
	CadOrg.y = ComponentPtr_Org->GetComponentCadPosY();
	CadOrg.z = ComponentPtr_Org->GetComponentResultSkewAngle();
	StageOrg.x = ComponentPtr_Org->GetComponentStagePosX();
	StageOrg.y = ComponentPtr_Org->GetComponentStagePosY();
	StageOrg.z = ComponentPtr_Org->GetComponentResultSkewAngle();

	CadOrg2.x = ComponentPtr_Org->GetComponentCadResultX(); 
	CadOrg2.y = ComponentPtr_Org->GetComponentCadResultY(); 
	CadOrg2.z = ComponentPtr_Org->GetComponentResultSkewAngle();
	StageOrg2.x = ComponentPtr_Org->GetComponentStageResultX(); 
	StageOrg2.y = ComponentPtr_Org->GetComponentStageResultY(); 
	StageOrg2.z = ComponentPtr_Org->GetComponentResultSkewAngle();		
	for ( i=0; i<GroupNodeCount; i++ )
	{
		GroupNodePtr = GroupPtr->GetPartGroupNodePtr(i, false);
		if ( NULL == GroupNodePtr ) { continue; }
		bPass=true;	
		DefectText = _T("");
		ComponentPtr = GroupNodePtr->ComponentPtr;
		CadPos.x = ComponentPtr->GetComponentCadPosX();
		CadPos.y = ComponentPtr->GetComponentCadPosY();
		StagePos.x = ComponentPtr->GetComponentStagePosX();
		StagePos.y = ComponentPtr->GetComponentStagePosY();

		CadResult.x = ComponentPtr->GetComponentCadResultX(); 
		CadResult.y = ComponentPtr->GetComponentCadResultY(); 
		CadResult.z = ComponentPtr->GetComponentResultSkewAngle();
		StageResult.x = ComponentPtr->GetComponentStageResultX(); 
		StageResult.y = ComponentPtr->GetComponentStageResultY(); 
		StageResult.z = ComponentPtr->GetComponentResultSkewAngle();
		
		//原來座標差距
		CadOffset1.x = CadPos.x-CadOrg.x;
		CadOffset1.y = CadPos.y-CadOrg.y;
		CadOffset1.z = CadPos.z-CadOrg.z;
		StageOffset1.x = StagePos.x-StageOrg.x;
		StageOffset1.y = StagePos.y-StageOrg.y;
		StageOffset1.z = StagePos.z-StageOrg.z;

		//實際座標差距
		CadOffset2.x = CadResult.x-CadOrg2.x;
		CadOffset2.y = CadResult.y-CadOrg2.y;
		CadOffset2.z = CadResult.z-CadOrg2.z;
		StageOffset2.x = StageResult.x-StageOrg2.x;
		StageOffset2.y = StageResult.y-StageOrg2.y;
		StageOffset2.z = StageResult.z-StageOrg2.z;

		CadOffsetL1 = JetAPI::CalcDistance(CadOffset1.x, CadOffset1.y);
		CadOffsetL2 = JetAPI::CalcDistance(CadOffset2.x, CadOffset2.y);

		//相對偏移量
		CadGapL = CadOffsetL2-CadOffsetL1;
		CadGap.x = CadOffset2.x-CadOffset1.x;
		CadGap.y = CadOffset2.y-CadOffset1.y;
		CadGap.z = CadOffset2.z-CadOffset1.z;
		StageGap.x = StageOffset2.x-StageOffset1.x;
		StageGap.y = StageOffset2.y-StageOffset1.y;
		StageGap.z = StageOffset2.z-StageOffset1.z;

		GroupNodePtr->ResultID = RESULT_ID_OK;
		GroupNodePtr->ResultStdX = CadOffset1.x;
		GroupNodePtr->ResultStdY = CadOffset1.y;
		GroupNodePtr->ResultStdL = CadOffsetL1;
		GroupNodePtr->ResultGapSkew = CadOffset2.z;

		GroupNodePtr->ResultGapX = CadGap.x;
		GroupNodePtr->ResultGapY = CadGap.y;
		GroupNodePtr->ResultGapL = CadGapL;
		GroupNodePtr->ResultGapSkew = CadGap.z;

		ResultText = DefectText = _T("");
		if ( true == EnbX )
		{
			if ( CadGap.x > 0 ) 
			{	ResultText.Format(_T("X:%.0f + %.0f"), CadOffset1.x, CadGap.x);	}
			else
			{	ResultText.Format(_T("X:%.0f - %.0f"), CadOffset1.x, -CadGap.x); }
			if ( CadGap.x>USLX || CadGap.x<LSLX ) 
			{	
				bPass = false; 
				DefectText = ResultText;
			}
		}
		if ( true == EnbY )
		{
			if ( CadGap.y > 0 ) 
			{	ResultText.Format(_T("Y:%.0f + %.0f"), CadOffset1.y, CadGap.y); }
			else
			{	ResultText.Format(_T("Y:%.0f - %.0f"), CadOffset1.y, -CadGap.y); }
			if ( CadGap.y>USLY || CadGap.y<LSLY ) 
			{	
				bPass = false; 
				DefectText = ResultText;
			}
		}
		if ( true == EnbL )
		{
			if ( CadGapL > 0 ) 
			{	ResultText.Format(_T("L:%.0f + %.0f"), CadOffsetL1, CadGapL); }
			else
			{	ResultText.Format(_T("L:%.0f - %.0f"), CadOffsetL1, -CadGapL); }
			if ( CadGapL>USLL || CadGapL<LSLL ) 
			{	
				bPass = false; 
				DefectText = ResultText;
			}
		}
		GroupNodePtr->ResultText = ResultText;
		if ( true == bPass ) { continue; }
		//Set Component NG		
		GroupNodePtr->ResultID = RESULT_ID_NG;
		GroupNodePtr->ResultText = DefectText;
		const size_t WndIndex=GroupNodePtr->ModelWndIndex;
		DefectText = GroupPtr->BuildPartGroupDefectText(DefectText);
		ComponentPtr->SetComponentGroupResult(WndIndex, RESULT_ID_NG, DefectText);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIProject::ExecProjectGroupInspection_GroupCoordMap(CAOIPartGroup *GroupPtr)//執行專案群組檢測-群組對線(2點)
{
	CAOIProject *ProjectPtr = this;
	if ( CheckProjectPartGroupPtr(GroupPtr) == false )
	{	return false; }
	if ( PART_GROUP_DIST_GROUP_COORD_MAP != GroupPtr->GetPartGroupMode() ) 
	{	return true; }

	size_t         i=0;
	bool           bPass=true;
	double         CadGapX=0;
	double         CadGapY=0;
	double         CadGapL=0;
	CString        ResultText;
	CString        DefectText;	
	CString        ResultText1;
	CString        ResultText2;
	PART_GROUP_MODE GroupMode;
	TPartGroupNode *GroupNodePtr=NULL;
	TPartGroupNode *GroupNodePtr1=NULL;
	TPartGroupNode *GroupNodePtr2=NULL;
	TPartGroupNode *GroupNodePtr3=NULL;
	TPartGroupNode *GroupNodePtr4=NULL;
	CAOIWnd       *WndPtr = NULL;
	CAOIWnd       *WndPtr1 = NULL;
	CAOIWnd       *WndPtr2 = NULL;
	CAOIWnd       *WndPtr3 = NULL;
	CAOIWnd       *WndPtr4 = NULL;
	CAOIComponent *ComponentPtr = NULL;	
	CAOIComponent *ComponentPtr1 = NULL;
	CAOIComponent *ComponentPtr2 = NULL;
	CAOIComponent *ComponentPtr3 = NULL;
	CAOIComponent *ComponentPtr4 = NULL;	
	PART_GROUP_MAP_DIR_MODE MapDirMode1=PART_GROUP_MAP_DIR_NONE;
	PART_GROUP_MAP_DIR_MODE MapDirMode2=PART_GROUP_MAP_DIR_NONE;
	PART_GROUP_MAP_DIR_MODE MapDirMode3=PART_GROUP_MAP_DIR_NONE;
	PART_GROUP_MAP_DIR_MODE MapDirMode4=PART_GROUP_MAP_DIR_NONE;	
	const size_t GroupNodeCount=GroupPtr->GetPartGroupNodeCount();	
	const double DistanceGapScaleX=GroupPtr->GetDistanceGapScaleX();
	const double DistanceGapScaleY=GroupPtr->GetDistanceGapScaleY();
	const double DistanceGapScaleL=GroupPtr->GetDistanceGapScaleL();

	GroupNodePtr1 = GroupPtr->GetPartGroupNodePtr1();
	GroupNodePtr2 = GroupPtr->GetPartGroupNodePtr2();
	GroupNodePtr3 = GroupPtr->GetPartGroupNodePtr3();
	GroupNodePtr4 = GroupPtr->GetPartGroupNodePtr4();
	WndPtr1 = GroupNodePtr1->WndPtr;
	WndPtr2 = GroupNodePtr2->WndPtr;
	WndPtr3 = GroupNodePtr3->WndPtr;
	WndPtr4 = GroupNodePtr4->WndPtr;
	ComponentPtr1 = GroupNodePtr1->ComponentPtr;
	ComponentPtr2 = GroupNodePtr2->ComponentPtr;	
	ComponentPtr3 = GroupNodePtr3->ComponentPtr;
	ComponentPtr4 = GroupNodePtr4->ComponentPtr;	
	MapDirMode1 = GroupNodePtr1->MapDirMode;
	MapDirMode2 = GroupNodePtr2->MapDirMode;
	MapDirMode3 = GroupNodePtr3->MapDirMode;
	MapDirMode4 = GroupNodePtr4->MapDirMode;

	const bool bUsedWndPos=false;
	TPOINT3D PartPt, PartMapPt;		
	TPOINT3D PartPtCad, PartMapPtCad;
	TPOINT3D PartOffset, PartMapOffset;
	TPOINT2D MarkPt1, MarkPt2, MarkPt3, MarkPt4, MarkOrgPt;	
	TPOINT2D MarkPtCad1, MarkPtCad2, MarkPtCad3, MarkPtCad4, MarkOrgPtCad;
	std::vector<CAOIWnd*> BaseLineWndList;	
	if ( NULL == WndPtr1 )
	{
		m_ErrorString = _T("Error, ExecProjectGroupInspection_GroupCoordMap Fault(WndPtr==NULL)");
		return false;
	}
	if ( NULL == ComponentPtr1 )
	{
		m_ErrorString = _T("Error, ExecProjectGroupInspection_GroupCoordMap Fault(ComponentPtr==NULL)");
		return false;
	}

	if ( NULL != WndPtr1 ) { BaseLineWndList.push_back(WndPtr1); }
	if ( NULL != WndPtr2 ) { BaseLineWndList.push_back(WndPtr2); }
	if ( NULL != WndPtr3 ) { BaseLineWndList.push_back(WndPtr3); }
	if ( NULL != WndPtr4 ) { BaseLineWndList.push_back(WndPtr4); }
	const size_t BaseLineWndCount=BaseLineWndList.size();

	if ( NULL != ComponentPtr1 )
	{
		MarkPt1.x = ComponentPtr1->GetComponentCadResultX();
		MarkPt1.y = ComponentPtr1->GetComponentCadResultY();
		MarkPtCad1.x = ComponentPtr1->GetComponentCadPosX();
		MarkPtCad1.y = ComponentPtr1->GetComponentCadPosY();
	}
	if ( NULL != WndPtr1 && true==bUsedWndPos )
	{
		WndPtr1->GetWndBox().GetBoxPosCadRes(MarkPt1);
		WndPtr1->GetWndBox().GetBoxPosCad(MarkPtCad1);
	}
	if ( NULL != ComponentPtr2 )
	{
		MarkPt2.x = ComponentPtr2->GetComponentCadResultX();
		MarkPt2.y = ComponentPtr2->GetComponentCadResultY();	
		MarkPtCad2.x = ComponentPtr2->GetComponentCadPosX();
		MarkPtCad2.y = ComponentPtr2->GetComponentCadPosY();
	}
	if ( NULL != WndPtr2 && true==bUsedWndPos )
	{
		WndPtr2->GetWndBox().GetBoxPosCadRes(MarkPt2);		
		WndPtr2->GetWndBox().GetBoxPosCad(MarkPtCad2);	
	}

	if ( NULL != ComponentPtr3 )
	{
		MarkPt3.x = ComponentPtr3->GetComponentCadResultX();
		MarkPt3.y = ComponentPtr3->GetComponentCadResultY();
		MarkPtCad3.x = ComponentPtr3->GetComponentCadPosX();
		MarkPtCad3.y = ComponentPtr3->GetComponentCadPosY();
	}
	if ( NULL != WndPtr3 && true==bUsedWndPos )
	{
		WndPtr3->GetWndBox().GetBoxPosCadRes(MarkPt3);
		WndPtr3->GetWndBox().GetBoxPosCad(MarkPtCad3);
	}

	if ( NULL != ComponentPtr4 )
	{
		MarkPt4.x = ComponentPtr4->GetComponentCadResultX();
		MarkPt4.y = ComponentPtr4->GetComponentCadResultY();
		MarkPtCad4.x = ComponentPtr4->GetComponentCadPosX();
		MarkPtCad4.y = ComponentPtr4->GetComponentCadPosY();
	}
	if ( NULL != WndPtr4 && true==bUsedWndPos )
	{
		WndPtr4->GetWndBox().GetBoxPosCadRes(MarkPt4);
		WndPtr4->GetWndBox().GetBoxPosCad(MarkPtCad4);
	}
	
	
	int MapCnt=0;
	CMapCoordinate Map;
	CMapCoordinate MapCad;
	int MapDirCount[PART_GROUP_MAP_DIR_RETURN];
	double SrcX[4], SrcY[4], DstX[4], DstY[4];	
	double SrcXCad[4], SrcYCad[4], DstXCad[4], DstYCad[4];	
	::memset(MapDirCount, 0x00, sizeof(MapDirCount));
	::memset(SrcX, 0x00, sizeof(SrcX));	::memset(SrcY, 0x00, sizeof(SrcY));
	::memset(DstX, 0x00, sizeof(DstX));	::memset(DstY, 0x00, sizeof(DstY));
	::memset(SrcXCad, 0x00, sizeof(SrcXCad));	::memset(SrcYCad, 0x00, sizeof(SrcYCad));
	::memset(DstXCad, 0x00, sizeof(DstXCad));	::memset(DstYCad, 0x00, sizeof(DstYCad));

	//Mark 1
	SrcX[0] = MarkPt1.x;	SrcY[0] = MarkPt1.y;	
	DstX[0] = 0;			DstY[0] = 0;
	SrcXCad[0] = MarkPtCad1.x;	SrcYCad[0] = MarkPtCad1.y;	
	DstXCad[0] = 0;			DstYCad[0] = 0;
	MapCnt ++;
	MapDirCount[MapDirMode1] ++;

	//Mark 2
	if ( NULL!=ComponentPtr2 || NULL!=WndPtr2 )
	{
		bool bUsed=true;
		const TPOINT2D &rMarkPt=MarkPt2;
		const TPOINT2D &rMarkPtCad=MarkPtCad2;
		double &rSrcX=SrcX[1], &rSrcY=SrcY[1];
		double &rDstX=DstX[1], &rDstY=DstY[1];
		double &rSrcXCad=SrcXCad[1], &rSrcYCad=SrcYCad[1];
		double &rDstXCad=DstXCad[1], &rDstYCad=DstYCad[1];
		PART_GROUP_MAP_DIR_MODE &rMapDirMode = MapDirMode2;

		rSrcX = rMarkPt.x;	rSrcY = rMarkPt.y;
		rDstX = 0;			rDstY = 0;
		rSrcXCad = rMarkPtCad.x;	rSrcYCad = rMarkPtCad.y;
		rDstXCad = 0;		rDstYCad = 0;
		switch ( rMapDirMode )
		{
		case PART_GROUP_MAP_DIR_POS_X:			
			rDstX = JetAPI::CalcDistance(MarkPt1, rMarkPt);
			rDstXCad = JetAPI::CalcDistance(MarkPtCad1, rMarkPtCad);
			break;
		case PART_GROUP_MAP_DIR_POS_Y:	
			rDstY = JetAPI::CalcDistance(MarkPt1, rMarkPt);	
			rDstYCad = JetAPI::CalcDistance(MarkPtCad1, rMarkPtCad);
			break;
		case PART_GROUP_MAP_DIR_NEG_X:	
			rDstX = -1*(JetAPI::CalcDistance(MarkPt1, rMarkPt));	
			rDstXCad = -1*(JetAPI::CalcDistance(MarkPtCad1, rMarkPtCad));
			break;
		case PART_GROUP_MAP_DIR_NEG_Y:	
			rDstY = -1*(JetAPI::CalcDistance(MarkPt1, rMarkPt));	
			rDstYCad = -1*(JetAPI::CalcDistance(MarkPtCad1, rMarkPtCad));
			break;
		default:
			bUsed = false;
			break;
		}
		if ( true == bUsed )
		{	MapCnt ++;	}
		MapDirCount[rMapDirMode] ++;
	}

	//Mark 3
	if ( NULL!=ComponentPtr3 || NULL!=WndPtr3 )
	{
		bool bUsed=true;
		const TPOINT2D &rMarkPt=MarkPt3;
		const TPOINT2D &rMarkPtCad=MarkPtCad3;
		double &rSrcX=SrcX[2], &rSrcY=SrcY[2];
		double &rDstX=DstX[2], &rDstY=DstY[2];
		double &rSrcXCad=SrcXCad[2], &rSrcYCad=SrcYCad[2];
		double &rDstXCad=DstXCad[2], &rDstYCad=DstYCad[2];
		PART_GROUP_MAP_DIR_MODE &rMapDirMode = MapDirMode3;

		rSrcX = rMarkPt.x;	rSrcY = rMarkPt.y;
		rDstX = 0;			rDstY = 0;
		rSrcXCad = rMarkPtCad.x;	rSrcYCad = rMarkPtCad.y;
		rDstXCad = 0;		rDstYCad = 0;
		switch ( rMapDirMode )
		{
		case PART_GROUP_MAP_DIR_POS_X:	
			rDstX = JetAPI::CalcDistance(MarkPt1, rMarkPt);
			rDstXCad = JetAPI::CalcDistance(MarkPtCad1, rMarkPtCad);
			break;
		case PART_GROUP_MAP_DIR_POS_Y:	
			rDstY = JetAPI::CalcDistance(MarkPt1, rMarkPt);	
			rDstYCad = JetAPI::CalcDistance(MarkPtCad1, rMarkPtCad);
			break;
		case PART_GROUP_MAP_DIR_NEG_X:	
			rDstX = -1*(JetAPI::CalcDistance(MarkPt1, rMarkPt));	
			rDstXCad = -1*(JetAPI::CalcDistance(MarkPtCad1, rMarkPtCad));
			break;
		case PART_GROUP_MAP_DIR_NEG_Y:	
			rDstY = -1*(JetAPI::CalcDistance(MarkPt1, rMarkPt));	
			rDstYCad = -1*(JetAPI::CalcDistance(MarkPtCad1, rMarkPtCad));
			break;
		default:
			bUsed = false;
			break;
		}
		if ( true == bUsed )
		{	MapCnt ++;	}
		MapDirCount[rMapDirMode] ++;
	}
	for ( i=0; i<PART_GROUP_MAP_DIR_RETURN; i++ )
	{
		if ( MapDirCount[i] > 1 )
		{
			m_ErrorString = _T("Error, ExecProjectGroupInspection_GroupCoordMap Fault(MapDirection Repeat)");		
			return false;
		}
	}
	Map.CalcMatrix2D(SrcX, SrcY, DstX, DstY, MapCnt);
	MapCad.CalcMatrix2D(SrcXCad, SrcYCad, DstXCad, DstYCad, MapCnt);

	const bool bUseComponentCadOrg=true;//用零件Cad為原點
	for ( i=0; i<GroupNodeCount; i++ )
	{
		GroupNodePtr = GroupPtr->GetPartGroupNodePtr(i, false);
		if ( NULL == GroupNodePtr ) { continue; }
		WndPtr = GroupNodePtr->WndPtr;
		if ( NULL == WndPtr ) { continue; }
		ComponentPtr = GroupNodePtr->ComponentPtr;
		if ( NULL == ComponentPtr ) { continue; }

		bPass=true;		
		PartPt.x = ComponentPtr->GetComponentCadResultX();
		PartPt.y = ComponentPtr->GetComponentCadResultY();
		PartPtCad.x = ComponentPtr->GetComponentCadPosX();
		PartPtCad.y = ComponentPtr->GetComponentCadPosY();

		if ( true == bUsedWndPos )
		{
			PartPt.x = WndPtr->GetWndBox().GetBoxPosCadResX();
			PartPt.y = WndPtr->GetWndBox().GetBoxPosCadResY();
			PartPtCad.x = WndPtr->GetWndBox().GetBoxPosCadX();
			PartPtCad.y = WndPtr->GetWndBox().GetBoxPosCadY();
		}

		Map.Map2D(PartPt.x, PartPt.y, PartMapPt.x, PartMapPt.y);		
		MapCad.Map2D(PartPtCad.x, PartPtCad.y, PartMapPtCad.x, PartMapPtCad.y);

		PartPt.z = JetAPI::CalcDistance(PartPt.x, PartPt.y);
		PartMapPt.z = JetAPI::CalcDistance(PartMapPt.x, PartMapPt.y);
		PartPtCad.z = JetAPI::CalcDistance(PartPtCad.x, PartPtCad.y);
		PartMapPtCad.z = JetAPI::CalcDistance(PartMapPtCad.x, PartMapPtCad.y);

		if ( true == GroupNodePtr->UserMapCadEnable )
		{
			PartMapPtCad.x = GroupNodePtr->UserMapCadPosX;
			PartMapPtCad.y = GroupNodePtr->UserMapCadPosY;
			PartMapPtCad.z = GroupNodePtr->UserMapCadDisL;
		}

		PartOffset.x = PartPt.x-PartPtCad.x;
		PartOffset.y = PartPt.y-PartPtCad.y;
		PartOffset.z = PartPt.z-PartPtCad.z;

		PartMapOffset.x = PartMapPt.x-PartMapPtCad.x;
		PartMapOffset.y = PartMapPt.y-PartMapPtCad.y;
		PartMapOffset.z = PartMapPt.z-PartMapPtCad.z;
		
		if ( true == bUseComponentCadOrg )
		{
			PartPtCad.z = 0;
			PartMapPtCad.z = 0;;
			PartOffset.z = JetAPI::CalcDistance(PartOffset.x, PartOffset.y);
			PartMapOffset.z = JetAPI::CalcDistance(PartMapOffset.x, PartMapOffset.y);
		}

		PartOffset.x *= DistanceGapScaleX;
		PartOffset.y *= DistanceGapScaleY;
		PartOffset.z *= DistanceGapScaleL;

		PartMapOffset.x *= DistanceGapScaleX;
		PartMapOffset.y *= DistanceGapScaleY;
		PartMapOffset.z *= DistanceGapScaleL;		

		CadGapX = PartMapOffset.x;
		CadGapY = PartMapOffset.y;
		CadGapL = PartMapOffset.z;
		
		GroupNodePtr->ResultID = RESULT_ID_OK;
		ResultText2 = ResultText1 = _T("");
		ResultText = DefectText = _T("");
		if ( GroupPtr->GetDistanceGapEnbX() )
		{
			double Std=PartMapPtCad.x;
			GroupNodePtr->ResultStdX = Std;
			GroupNodePtr->ResultGapX = CadGapX;
			if ( CadGapX >= 0 )
			{	ResultText.Format(_T("X:%.0f+%.0f"), Std, CadGapX); }
			else
			{	ResultText.Format(_T("X:%.0f-%.0f"), Std, -CadGapX); }
			if ( CadGapX >GroupPtr->GetDistanceGapUSLX() || CadGapX<GroupPtr->GetDistanceGapLSLX() )
			{
				bPass = false;	
				DefectText = ResultText;
			}
			if ( ResultText2.GetLength() == 0 )
			{	ResultText2 = ResultText; }
			else
			{
				ResultText1 = ResultText2;
				ResultText2.Format(_T("%s, %s"), ResultText1, ResultText);
			}
		}

		if ( GroupPtr->GetDistanceGapEnbY() )
		{
			double Std=PartMapPtCad.y;
			GroupNodePtr->ResultStdY = Std;
			GroupNodePtr->ResultGapY = CadGapY;
			if ( CadGapY >= 0 )
			{	ResultText.Format(_T("Y:%.0f+%.0f"), Std, CadGapY); }
			else
			{	ResultText.Format(_T("Y:%.0f-%.0f"), Std, -CadGapY); }
			if ( CadGapY >GroupPtr->GetDistanceGapUSLY() || CadGapY<GroupPtr->GetDistanceGapLSLY() )
			{
				bPass = false;	
				DefectText = ResultText;
			}
			if ( ResultText2.GetLength() == 0 )
			{	ResultText2 = ResultText; }
			else
			{
				ResultText1 = ResultText2;
				ResultText2.Format(_T("%s, %s"), ResultText1, ResultText);
			}
		}

		if ( GroupPtr->GetDistanceGapEnbL() )
		{
			double Std=PartMapPtCad.z;
			GroupNodePtr->ResultStdL = Std;
			GroupNodePtr->ResultGapL = CadGapL;
			if ( CadGapL >= 0 )
			{	ResultText.Format(_T("L:%.0f+%.0f"), Std, CadGapL); }
			else
			{	ResultText.Format(_T("L:%.0f-%.0f"), Std, -CadGapL); }			
			if ( CadGapL >GroupPtr->GetDistanceGapUSLL() || CadGapL<GroupPtr->GetDistanceGapLSLL() )
			{
				bPass = false;	
				DefectText = ResultText;
			}
			if ( ResultText2.GetLength() == 0 )
			{	ResultText2 = ResultText; }
			else
			{
				ResultText1 = ResultText2;
				ResultText2.Format(_T("%s, %s"), ResultText1, ResultText);
			}
		}
		ResultText = ResultText2;
		GroupNodePtr->ResultText = ResultText;
		if ( true == bPass )
		{	continue; }
		//Set Component NG		
		GroupNodePtr->ResultID = RESULT_ID_NG;
		GroupNodePtr->ResultText = ResultText;
		const size_t WndIndex=GroupNodePtr->ModelWndIndex;
		DefectText = GroupPtr->BuildPartGroupDefectText(DefectText);
		ComponentPtr->SetComponentGroupResult(WndIndex, RESULT_ID_NG, DefectText);
	}	
	return true;	
}
//-------------------------------------------------------------------------------------//
