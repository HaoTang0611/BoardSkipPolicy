// AOIGroup.cpp: implementation of the AOIGroup class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIPartGroup.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
bool TPartGroupNode::CheckPartGroupNodeValid() const
{
	if ( -1 == ComponentIndex )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void TPartGroupNode::SetComponentPtr(CAOIComponent *Ptr)
{
	TPartGroupNode();		
	if ( NULL == Ptr ) { return; }
	ComponentPtr = Ptr;
	PanelPtr = Ptr->GetComponentPanelPtr();
	BoardPtr = Ptr->GetComponentBoardPtr();
	if ( NULL != PanelPtr )
	{	PanelIndex = PanelPtr->GetPanelIndex_Project();	}
	if ( NULL != BoardPtr )
	{	BoardIndex = BoardPtr->GetBoardIndex_Project();	}
	if ( NULL != ComponentPtr )
	{	ComponentIndex = ComponentPtr->GetComponentIndex_Project();	}
}
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CAOIPartGroup, CAOIObj)
//-------------------------------------------------------------------------------------//
CAOIPartGroup::CAOIPartGroup():CAOIObj(AOI_OBJ_PART_GROUP)
{
	PreInitPartGroup();
	InitialPartGroup();
}
//-------------------------------------------------------------------------------------//
CAOIPartGroup::CAOIPartGroup(const CAOIPartGroup &others):CAOIObj(others)
{
	PreInitPartGroup();
	ClonePartGroup(others);
}
//-------------------------------------------------------------------------------------//
CAOIPartGroup::~CAOIPartGroup()
{
}
//-------------------------------------------------------------------------------------//
CAOIPartGroup& CAOIPartGroup::operator=(const CAOIPartGroup &others)
{
	if ( this == &others ) { return *this; }
	CAOIObj::operator=(others);
	ClonePartGroup(others);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::PreInitPartGroup()
{	
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::InitialPartGroup()
{			
	//---------------------------------------------------------------------------------//
	m_PartGroupProjectPtr = NULL;
	//---------------------------------------------------------------------------------//
	m_PartGroupMode = PART_GROUP_RETURN;
	m_PartGroupName = L"PartGroup";
	m_PartGroupIndex = -1;	
	m_PartGroupUniqueID = -1;//群組唯一碼
	m_PartGroupGroupID = -1;//群組群組編號
	m_PartGroupInOneBoard=false;
	m_PartGroupDeleted = false;
	m_PartGroupSelected = false;	
	//---------------------------------------------------------------------------------//
	m_PartGroupNode1 = TPartGroupNode();
	m_PartGroupNode2 = TPartGroupNode();
	m_PartGroupNode3 = TPartGroupNode();
	m_PartGroupNode4 = TPartGroupNode();
	m_PartGroupNodeList.clear();
	//---------------------------------------------------------------------------------//
	m_ColinearityMode=PART_GROUP_COLINEARITY_X;
	m_ColinearityTargetMode=PART_GROUP_TARGET_AVE;
	m_ColinearityGapStd =    0;
	m_ColinearityGapUSL =  250;
	m_ColinearityGapLSL = -250;
	//---------------------------------------------------------------------------------//	
	m_DistanceGapEnbX = true;
	m_DistanceGapEnbY = true;
	m_DistanceGapEnbL = true;
	m_DistanceGapUSLX =  250;
	m_DistanceGapLSLX = -250;
	m_DistanceGapUSLY =  250;
	m_DistanceGapLSLY = -250;
	m_DistanceGapUSLL =  250;
	m_DistanceGapLSLL = -250;
	m_DistanceGapStdEnb = false;
	m_DistanceGapStdX = 0;
	m_DistanceGapStdY = 0;
	m_DistanceGapStdL = 0;
	m_DistanceGapAddX = 0;
	m_DistanceGapAddY = 0;
	m_DistanceGapEnbAbs = false;
	m_DistanceGapScaleX = 1.0;
	m_DistanceGapScaleY = 1.0;
	m_DistanceGapScaleL = 1.0;
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::ClonePartGroup(const CAOIPartGroup &others)
{	
	//---------------------------------------------------------------------------------//
	m_PartGroupProjectPtr = others.m_PartGroupProjectPtr;
	//---------------------------------------------------------------------------------//
	m_PartGroupMode = others.m_PartGroupMode;
	m_PartGroupName = others.m_PartGroupName;
	m_PartGroupIndex = others.m_PartGroupIndex;			
	m_PartGroupGroupID = others.m_PartGroupGroupID;	
	m_PartGroupInOneBoard=others.m_PartGroupInOneBoard;
	m_PartGroupUniqueID = others.m_PartGroupUniqueID;		
	m_PartGroupDeleted = others.m_PartGroupDeleted;
	m_PartGroupSelected = others.m_PartGroupSelected;
	//---------------------------------------------------------------------------------//	
	m_PartGroupNode1 = others.m_PartGroupNode1;//第1個資料點
	m_PartGroupNode2 = others.m_PartGroupNode2;//第2個資料點
	m_PartGroupNode3 = others.m_PartGroupNode3;//第3個資料點
	m_PartGroupNode4 = others.m_PartGroupNode4;//第4個資料點
	m_PartGroupNodeList = others.m_PartGroupNodeList;//資料點列表
	//---------------------------------------------------------------------------------//
	m_ColinearityMode = others.m_ColinearityMode;	
	m_ColinearityTargetMode = others.m_ColinearityTargetMode;
	m_ColinearityGapStd = others.m_ColinearityGapStd;
	m_ColinearityGapUSL = others.m_ColinearityGapUSL;
	m_ColinearityGapLSL = others.m_ColinearityGapLSL;	
	//---------------------------------------------------------------------------------//	
	m_DistanceGapEnbX = others.m_DistanceGapEnbX;	
	m_DistanceGapEnbY = others.m_DistanceGapEnbY;	
	m_DistanceGapEnbL = others.m_DistanceGapEnbL;	
	m_DistanceGapUSLX = others.m_DistanceGapUSLX;	
	m_DistanceGapLSLX = others.m_DistanceGapLSLX;	
	m_DistanceGapUSLY = others.m_DistanceGapUSLY;	
	m_DistanceGapLSLY = others.m_DistanceGapLSLY;	
	m_DistanceGapUSLL = others.m_DistanceGapUSLL;	
	m_DistanceGapLSLL = others.m_DistanceGapLSLL;	
	m_DistanceGapStdEnb = others.m_DistanceGapStdEnb;
	m_DistanceGapStdX = others.m_DistanceGapStdX;	
	m_DistanceGapStdY = others.m_DistanceGapStdY;	
	m_DistanceGapStdL = others.m_DistanceGapStdL;	
	m_DistanceGapAddX = others.m_DistanceGapAddX;
	m_DistanceGapAddY = others.m_DistanceGapAddY;
	m_DistanceGapEnbAbs = others.m_DistanceGapEnbAbs;
	m_DistanceGapScaleX = others.m_DistanceGapScaleX;
	m_DistanceGapScaleY = others.m_DistanceGapScaleY;
	m_DistanceGapScaleL = others.m_DistanceGapScaleL;
	//---------------------------------------------------------------------------------//		
}
//-------------------------------------------------------------------------------------//
CAOIPartGroup* CAOIPartGroup::ClonePartGroupObj() const//建立且複製一個零件群組物件
{
	CAOIPartGroup *ObjPtr = AOIObjManager.CreatePartGroupObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::WritePartGroupFile(CAOIFileIO &FileIO)//儲存零件群組檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	size_t       i=0;	
	CAOIPartGroup   *GroupPtr = this;
	TPartGroupNode  *GroupNodePtr = NULL;
	char         uuidStr[MAX_JET_PATH]="";	
	wchar_t      uuidWStr[MAX_JET_PATH]=L"";	
	UUID         uuid = GroupPtr->GetObjUuid();	
	FileIO.SetFnName(_T("CAOIPartGroup::WritePartGroupFile"));
	JetAPI::UUIDToStringA(uuid, uuidStr);
	JetAPI::UUIDToStringW(uuid, uuidWStr);
	//----------------------------------------------------------------------------------------//
	//零件群組參數
	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_START, 0) == false ) { return false; }
	
	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_UNIQUE_ID, GroupPtr->GetPartGroupUniqueID()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_INDEX, GroupPtr->GetPartGroupIndex()) == false ) { return false; }	
	if ( FileIO.GetSaveWStr() == true ) 
	{	if ( FileIO.SaveChunk_STR(FILE_IO_PART_GROUP_OBJ_UUID, uuidWStr) == false ) { return false; } }
	else
	{	if ( FileIO.SaveChunk_STR(FILE_IO_PART_GROUP_OBJ_UUID, uuidStr) == false ) { return false; } }		
	//if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_DISTRICT_ID, GroupPtr->GetGroupDistrictID()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_GROUP_ID, GroupPtr->GetPartGroupGroupID()) == false ) { return false; }
	if ( FileIO.SaveChunk_STR(FILE_IO_PART_GROUP_GROUP_NAME, GroupPtr->GetPartGroupName()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_GROUP_MODE, GroupPtr->GetPartGroupMode()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_PART_GROUP_IN_ONE_BOARD, GroupPtr->GetPartGroupInOneBoard()) == false ) { return false; }	

	//共線性
	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_COLINEARITY_MODE, GroupPtr->GetColinearityMode()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_COLINEARITY_GAP_USL, GroupPtr->GetColinearityGapUSL()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_COLINEARITY_GAP_LSL, GroupPtr->GetColinearityGapLSL()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_COLINEARITY_GAP_STD, GroupPtr->GetColinearityGapStd()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_COLINEARITY_TARGET_MODE, GroupPtr->GetColinearityTargetMode()) == false ) { return false; }

	//距離模式	
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_DISTANCE_GAP_USL_X, GroupPtr->GetDistanceGapUSLX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_DISTANCE_GAP_LSL_X, GroupPtr->GetDistanceGapLSLX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_DISTANCE_GAP_USL_Y, GroupPtr->GetDistanceGapUSLY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_DISTANCE_GAP_LSL_Y, GroupPtr->GetDistanceGapLSLY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_DISTANCE_GAP_USL_L, GroupPtr->GetDistanceGapUSLL()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_DISTANCE_GAP_LSL_L, GroupPtr->GetDistanceGapLSLL()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_PART_GROUP_DISTANCE_GAP_ENB_X, GroupPtr->GetDistanceGapEnbX()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_PART_GROUP_DISTANCE_GAP_ENB_Y, GroupPtr->GetDistanceGapEnbY()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_PART_GROUP_DISTANCE_GAP_ENB_L, GroupPtr->GetDistanceGapEnbL()) == false ) { return false; }
	
	//自訂標準
	if ( FileIO.SaveChunk_BOL(FILE_IO_PART_GROUP_DISTANCE_GAP_STD_ENB, GroupPtr->GetDistanceGapStdEnb()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_DISTANCE_GAP_STD_X, GroupPtr->GetDistanceGapStdX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_DISTANCE_GAP_STD_Y, GroupPtr->GetDistanceGapStdY()) == false ) { return false; }

	//偏差增加值(補償值)
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_DISTANCE_GAP_ADD_X, GroupPtr->GetDistanceGapAddX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_DISTANCE_GAP_ADD_Y, GroupPtr->GetDistanceGapAddY()) == false ) { return false; }

	if ( FileIO.SaveChunk_BOL(FILE_IO_PART_GROUP_DISTANCE_GAP_ENB_ABS, GroupPtr->GetDistanceGapEnbAbs()) == false ) { return false; }

	//偏差倍率
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_DISTANCE_GAP_SCALE_X, GroupPtr->GetDistanceGapScaleX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_DISTANCE_GAP_SCALE_Y, GroupPtr->GetDistanceGapScaleY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_DISTANCE_GAP_SCALE_L, GroupPtr->GetDistanceGapScaleL()) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_NODE_PARAM_1, 0) == false ) { return false; }
	if ( WritePartGroupNodeFile(GroupPtr->m_PartGroupNode1, FileIO) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_NODE_PARAM_2, 0) == false ) { return false; }
	if ( WritePartGroupNodeFile(GroupPtr->m_PartGroupNode2, FileIO) == false ) { return false; }
	
	const size_t GroupNodeCount=GroupPtr->GetPartGroupNodeCount();
	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_NODE_PARAM_LIST, 0) == false ) { return false; }
	if ( WritePartGroupNodeListFile(GroupPtr->m_PartGroupNodeList, FileIO) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_NODE_PARAM_3, 0) == false ) { return false; }
	if ( WritePartGroupNodeFile(GroupPtr->m_PartGroupNode3, FileIO) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_NODE_PARAM_4, 0) == false ) { return false; }
	if ( WritePartGroupNodeFile(GroupPtr->m_PartGroupNode4, FileIO) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_END, 0) == false ) { return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::ReadPartGroupFile(CAOIFileIO &FileIO)//載入零件群組檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	UUID         uuid;
	int          index = 0;
	unsigned int uValue = 0;		
	CAOIPartGroup   *GroupPtr = this;	
	FileIO.SetFnName(_T("CAOIPartGroup::ReadPartGroupFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		
		switch ( index )
		{
		case FILE_IO_PART_GROUP_START://零件群組參數-起點			
			GroupPtr->InitialPartGroup();
			break;
		case FILE_IO_PART_GROUP_END://零件群組參數-終點			
			return true;
			break;
		case FILE_IO_PART_GROUP_UNIQUE_ID://唯一碼
			GroupPtr->SetPartGroupUniqueID(FileIO.GetData_INT());
			break;

		case FILE_IO_PART_GROUP_INDEX://在專案的引數-Debug
			GroupPtr->SetPartGroupIndex(FileIO.GetData_INT());
			break;		

		case FILE_IO_PART_GROUP_OBJ_UUID://OBJ-UUID			
			if ( FileIO.GetLoadWStr()==true )
			{
				if ( JetAPI::UUIDFromStringW(FileIO.GetData_WSTR(), uuid) == true )
				{	GroupPtr->SetObjUuid(uuid);	}
			}
			else
			{
				if ( JetAPI::UUIDFromStringA(FileIO.GetData_STR(), uuid) == true )
				{	GroupPtr->SetObjUuid(uuid);	}
			}
			break;
		case FILE_IO_PART_GROUP_DISTRICT_ID://分段編號
			break;

		case FILE_IO_PART_GROUP_GROUP_ID://零件群組編號
			GroupPtr->SetPartGroupGroupID(FileIO.GetData_INT());
			break;
		case FILE_IO_PART_GROUP_GROUP_NAME://群組名稱
			if ( FileIO.GetLoadWStr()==true )
			{	GroupPtr->SetPartGroupName(FileIO.GetData_WSTR());	}
			else
			{	GroupPtr->SetPartGroupName(FileIO.GetData_STR()); }			
			break;
		case FILE_IO_PART_GROUP_GROUP_MODE://零件群組模式
			GroupPtr->SetPartGroupMode((PART_GROUP_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_PART_GROUP_IN_ONE_BOARD://同個單板內
			GroupPtr->SetPartGroupInOneBoard(FileIO.GetData_BOL());
			break;

		case FILE_IO_PART_GROUP_COLINEARITY_MODE://共線性模式
			GroupPtr->SetColinearityMode((PART_GROUP_COLINEARITY_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_PART_GROUP_COLINEARITY_GAP_USL://共線性偏差上限
			GroupPtr->SetColinearityGapUSL(FileIO.GetData_DBL());
			break;
		case FILE_IO_PART_GROUP_COLINEARITY_GAP_LSL://共線性偏差下限
			GroupPtr->SetColinearityGapLSL(FileIO.GetData_DBL());
			break;
		case FILE_IO_PART_GROUP_COLINEARITY_GAP_STD://群組參數-共線性偏差標準
			GroupPtr->SetColinearityGapStd(FileIO.GetData_DBL());
			break;
		case FILE_IO_PART_GROUP_COLINEARITY_TARGET_MODE://共線性目標值模式
			GroupPtr->SetColinearityTargetMode((PART_GROUP_TARGET_MODE)(FileIO.GetData_INT()));
			break;

		case FILE_IO_PART_GROUP_DISTANCE_GAP_USL_X://距離偏差上限-X
			GroupPtr->SetDistanceGapUSLX(FileIO.GetData_DBL());
			break;
		case FILE_IO_PART_GROUP_DISTANCE_GAP_LSL_X://距離偏差下限-X
			GroupPtr->SetDistanceGapLSLX(FileIO.GetData_DBL());
			break;
		case FILE_IO_PART_GROUP_DISTANCE_GAP_USL_Y://距離偏差上限-Y
			GroupPtr->SetDistanceGapUSLY(FileIO.GetData_DBL());
			break;
		case FILE_IO_PART_GROUP_DISTANCE_GAP_LSL_Y://距離偏差下限-Y
			GroupPtr->SetDistanceGapLSLY(FileIO.GetData_DBL());
			break;
		case FILE_IO_PART_GROUP_DISTANCE_GAP_USL_L://距離偏差上限-L
			GroupPtr->SetDistanceGapUSLL(FileIO.GetData_DBL());
			break;
		case FILE_IO_PART_GROUP_DISTANCE_GAP_LSL_L:;//距離偏差下限-L
			GroupPtr->SetDistanceGapLSLL(FileIO.GetData_DBL());
			break;
		case FILE_IO_PART_GROUP_DISTANCE_GAP_ENB_X:;//距離偏差啟用-X
			GroupPtr->SetDistanceGapEnbX(FileIO.GetData_BOL());
			break;
		case FILE_IO_PART_GROUP_DISTANCE_GAP_ENB_Y:;//距離偏差啟用-Y
			GroupPtr->SetDistanceGapEnbY(FileIO.GetData_BOL());
			break;
		case FILE_IO_PART_GROUP_DISTANCE_GAP_ENB_L:;//距離偏差啟用-L
			GroupPtr->SetDistanceGapEnbL(FileIO.GetData_BOL());
			break;		

		case FILE_IO_PART_GROUP_DISTANCE_GAP_STD_ENB:;//相對座標標準值-啟用
			GroupPtr->SetDistanceGapStdEnb(FileIO.GetData_BOL());
			break;
		case FILE_IO_PART_GROUP_DISTANCE_GAP_STD_X:;//相對座標標準值-X
			GroupPtr->SetDistanceGapStdX(FileIO.GetData_DBL());
			break;
		case FILE_IO_PART_GROUP_DISTANCE_GAP_STD_Y:;//相對座標標準值-Y
			GroupPtr->SetDistanceGapStdY(FileIO.GetData_DBL());
			break;

		case FILE_IO_PART_GROUP_DISTANCE_GAP_ADD_X://相對座標偏差增加值-X
			GroupPtr->SetDistanceGapAddX(FileIO.GetData_DBL());
			break;
		case FILE_IO_PART_GROUP_DISTANCE_GAP_ADD_Y://相對座標偏差增加值-Y
			GroupPtr->SetDistanceGapAddY(FileIO.GetData_DBL());
			break;

		case FILE_IO_PART_GROUP_DISTANCE_GAP_ENB_ABS://群組參數-相對座標絕對值啟用
			GroupPtr->SetDistanceGapEnbAbs(FileIO.GetData_BOL());
			break;

		case FILE_IO_PART_GROUP_DISTANCE_GAP_SCALE_X://相對座標偏差倍率-X
			GroupPtr->SetDistanceGapScaleX(FileIO.GetData_DBL());
			break;
		case FILE_IO_PART_GROUP_DISTANCE_GAP_SCALE_Y://相對座標偏差倍率-Y
			GroupPtr->SetDistanceGapScaleY(FileIO.GetData_DBL());
			break;
		case FILE_IO_PART_GROUP_DISTANCE_GAP_SCALE_L://相對座標偏差倍率-L
			GroupPtr->SetDistanceGapScaleL(FileIO.GetData_DBL());
			break;

		case FILE_IO_PART_GROUP_NODE_PARAM_1://節點參數1
			if ( GroupPtr->ReadPartGroupNodeFile(GroupPtr->m_PartGroupNode1, FileIO) == false )
			{	return false; }
			break;
		case FILE_IO_PART_GROUP_NODE_PARAM_2://節點參數2
			if ( GroupPtr->ReadPartGroupNodeFile(GroupPtr->m_PartGroupNode2, FileIO) == false )
			{	return false; }
			break;
		case FILE_IO_PART_GROUP_NODE_PARAM_LIST://節點參數列表
			if ( GroupPtr->ReadPartGroupNodeListFile(GroupPtr->m_PartGroupNodeList, FileIO) == false )
			{	return false; }
			break;
		case FILE_IO_PART_GROUP_NODE_PARAM_3://節點參數3
			if ( GroupPtr->ReadPartGroupNodeFile(GroupPtr->m_PartGroupNode3, FileIO) == false )
			{	return false; }
			break;
		case FILE_IO_PART_GROUP_NODE_PARAM_4://節點參數4
			if ( GroupPtr->ReadPartGroupNodeFile(GroupPtr->m_PartGroupNode4, FileIO) == false )
			{	return false; }
			break;

		default:
			break;
		}
	};		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::WritePartGroupNodeFile(const TPartGroupNode &Node, CAOIFileIO &FileIO)//儲存零件群組節點檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	FileIO.SetFnName(_T("CAOIPartGroup::WritePartGroupNodeFile"));	
	//----------------------------------------------------------------------------------------//
	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_NODE_START, 0) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_NODE_COMPONENT_INDEX, Node.ComponentIndex) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_NODE_MODEL_WND_INDEX, Node.ModelWndIndex) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_NODE_MAP_DIR_MODE, Node.MapDirMode) == false ) { return false; }	

	if ( FileIO.SaveChunk_BOL(FILE_IO_PART_GROUP_NODE_USER_MAP_CAD_ENABLE, Node.UserMapCadEnable) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_NODE_USER_MAP_CAD_POS_X, Node.UserMapCadPosX) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_NODE_USER_MAP_CAD_POS_Y, Node.UserMapCadPosY) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_PART_GROUP_NODE_USER_MAP_CAD_DIS_L, Node.UserMapCadDisL) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_NODE_END, 0) == false ) { return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::ReadPartGroupNodeFile(TPartGroupNode &Node, CAOIFileIO &FileIO)//載入零件群組節點檔案	
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	UUID         uuid;
	int          index = 0;
	unsigned int uValue = 0;		
	FileIO.SetFnName(_T("CAOIPartGroup::ReadPartGroupNodeFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		
		switch ( index )
		{
		case FILE_IO_PART_GROUP_NODE_START://零件群組節點參數-起點
			break;
		case FILE_IO_PART_GROUP_NODE_END://零件群組節點參數-終點			
			return true;
			break;
		case FILE_IO_PART_GROUP_NODE_COMPONENT_INDEX://零件引數
			Node.ComponentIndex = FileIO.GetData_INT();
			break;
		case FILE_IO_PART_GROUP_NODE_MODEL_WND_INDEX://檢測框引數
			Node.ModelWndIndex = FileIO.GetData_INT();
			break;
		case FILE_IO_PART_GROUP_NODE_MAP_DIR_MODE://座標轉換方向
			Node.MapDirMode = (PART_GROUP_MAP_DIR_MODE)(FileIO.GetData_INT());
			break;

		case FILE_IO_PART_GROUP_NODE_USER_MAP_CAD_ENABLE://自訂映射座標啟用
			Node.UserMapCadEnable = FileIO.GetData_BOL();
			break;
		case FILE_IO_PART_GROUP_NODE_USER_MAP_CAD_POS_X://自訂映射座標-X
			Node.UserMapCadPosX = FileIO.GetData_DBL();
			break;
		case FILE_IO_PART_GROUP_NODE_USER_MAP_CAD_POS_Y://自訂映射座標-Y
			Node.UserMapCadPosY = FileIO.GetData_DBL();
			break;
		case FILE_IO_PART_GROUP_NODE_USER_MAP_CAD_DIS_L://自訂映射長度-L
			Node.UserMapCadDisL = FileIO.GetData_DBL();
			break;
		
		default:
			break;
		}
	};			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::WritePartGroupNodeListFile(const std::vector<TPartGroupNode> &NodeList, CAOIFileIO &FileIO)//儲存節點列表檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	FileIO.SetFnName(_T("CAOIPartGroup::WritePartGroupNodeListFile"));	
	//----------------------------------------------------------------------------------------//
	size_t       i=0;
	const size_t NodeCount=NodeList.size();
	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_NODE_SECTION_START, 0) == false ) { return false; }	
	for ( i=0; i<NodeCount; i++ )
	{
		if ( WritePartGroupNodeFile(NodeList[i], FileIO) == false )
		{	return false; }
	}		
	if ( FileIO.SaveChunk_INT(FILE_IO_PART_GROUP_NODE_SECTION_END, 0) == false ) { return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::ReadPartGroupNodeListFile(std::vector<TPartGroupNode> &NodeList, CAOIFileIO &FileIO)//載入節點列表檔案	
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	UUID         uuid;
	int          index = 0;
	unsigned int uValue = 0;		
	TPartGroupNode   GroupNode;
	FileIO.SetFnName(_T("CAOIPartGroup::ReadPartGroupNodeListFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		
		switch ( index )
		{
		case FILE_IO_PART_GROUP_NODE_SECTION_START://零件群組節點參數區間-起點
			break;
		case FILE_IO_PART_GROUP_NODE_SECTION_END://零件群組節點參數區間-終點			
			return true;
			break;
		case FILE_IO_PART_GROUP_NODE_START://起點
			GroupNode = TPartGroupNode();
			if ( ReadPartGroupNodeFile(GroupNode, FileIO) == false ) 
			{	return false; }
			NodeList.push_back(GroupNode);			
			break;

		default:
			break;
		}
	};			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::WritePartGroupSpcFile_JSON_VRS(FILE *pfile, CAOIProject *ProjectPtr)//儲存檢測結果檔案JSON-VRS
{
	if ( NULL == pfile ) { return false; }
	if ( NULL == ProjectPtr ) { return false; }

	CAOIPartGroup *PartGroupPtr = this;
	if ( NULL == PartGroupPtr ) { return false; }	

	size_t           i=0;	
	CString          strText;
	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";

	::wcscpy(strTag, L"Mode");
	::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, PartGroupPtr->GetPartGroupMode());

	::wcscpy(strTag, L"Name");	
	strText = PartGroupPtr->GetPartGroupName();
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	::fwprintf(pfile, L"        \"%s\": \"%s\",\n", strTag, strBuffer);

	//::wcscpy(strTag, L"Skew");
	//::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, PartGroupPtr->GetComponentResultSkewAngle());

	//For Colinearity 
	::fwprintf(pfile, L"        \"Colinearity\":\n");
	::fwprintf(pfile, L"        {\n");	
	::wcscpy(strTag, L"Gap_USL");
	::fwprintf(pfile, L"          \"%s\": %.2f,\n", strTag, PartGroupPtr->GetColinearityGapUSL());
	::wcscpy(strTag, L"Gap_LSL");
	::fwprintf(pfile, L"          \"%s\": %.2f,\n", strTag, PartGroupPtr->GetColinearityGapLSL());
	::wcscpy(strTag, L"Mode");
	::fwprintf(pfile, L"          \"%s\": %d\n", strTag, PartGroupPtr->GetColinearityMode());
	::fwprintf(pfile, L"        },\n");

	//For Relative Pos
	::fwprintf(pfile, L"        \"Distance\":\n");
	::fwprintf(pfile, L"        {\n");
	::wcscpy(strTag, L"Enable_X");
	::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, PartGroupPtr->GetDistanceGapEnbX());
	::wcscpy(strTag, L"Gap_USL_X");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, PartGroupPtr->GetDistanceGapUSLX());
	::wcscpy(strTag, L"Gap_LSL_X");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, PartGroupPtr->GetDistanceGapLSLX());

	::wcscpy(strTag, L"Enable_Y");
	::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, PartGroupPtr->GetDistanceGapEnbY());
	::wcscpy(strTag, L"Gap_USL_Y");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, PartGroupPtr->GetDistanceGapUSLY());
	::wcscpy(strTag, L"Gap_LSL_Y");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, PartGroupPtr->GetDistanceGapLSLY());

	::wcscpy(strTag, L"Enable_L");
	::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, PartGroupPtr->GetDistanceGapEnbL());
	::wcscpy(strTag, L"Gap_USL_L");
	::fwprintf(pfile, L"        \"%s\": %.2f,\n", strTag, PartGroupPtr->GetDistanceGapUSLL());
	::wcscpy(strTag, L"Gap_LSL_L");
	::fwprintf(pfile, L"        \"%s\": %.2f\n", strTag, PartGroupPtr->GetDistanceGapLSLL());

	::fwprintf(pfile, L"        },\n");
	
	::fwprintf(pfile, L"      \"Node_1\":\n");
	::fwprintf(pfile, L"      {\n");
	if ( WritePartGroupNodeSpcFile_JSON_VRS(pfile, m_PartGroupNode1, ProjectPtr) == false )
	{	return false; }	
	::fwprintf(pfile, L"      },\n");

	::fwprintf(pfile, L"      \"Node_2\":\n");
	::fwprintf(pfile, L"      {\n");
	if ( WritePartGroupNodeSpcFile_JSON_VRS(pfile, m_PartGroupNode2, ProjectPtr) == false )
	{	return false; }
	::fwprintf(pfile, L"      },\n");

	const size_t PartGroupNodeCnt=PartGroupPtr->GetPartGroupNodeCount();
	// "N_PartGroupNodes";
	::wcscpy(strTag, L"N_PartGroupNodes");	
	::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, PartGroupNodeCnt);
	for ( i=0; i<PartGroupNodeCnt; i++ )
	{
		::fwprintf(pfile, L"      \"Node_%04d\":\n", i+1);
		::fwprintf(pfile, L"      {\n");
		if ( WritePartGroupNodeSpcFile_JSON_VRS(pfile, m_PartGroupNodeList[i], ProjectPtr) == false )
		{	return false; }
		::fwprintf(pfile, L"      },\n");
	}
	
	::wcscpy(strTag, L"Index");
	::fwprintf(pfile, L"        \"%s\": %d\n", strTag, PartGroupPtr->GetPartGroupIndex()+1);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::WritePartGroupNodeSpcFile_JSON_VRS(FILE *pfile, const TPartGroupNode &Node, CAOIProject *ProjectPtr)//儲存檢測結果檔案JSON-VRS
{
	if ( NULL == pfile ) { return false; }
	if ( NULL == ProjectPtr ) { return false; }

	size_t           i=0;
	int              PanelIndex=0;
	int              BoardIndex=0;
	CString          strText;
	CString          strName;
	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";	

	CAOIPanel *PanelPtr = NULL;
	CAOIBoard *BoardPtr = NULL;
	CAOIComponent *ComponentPtr = Node.ComponentPtr;
	if ( NULL == ComponentPtr )
	{	strName = _T("N_A"); }
	else
	{
		strName = ComponentPtr->GetComponentName();
		PanelPtr = ComponentPtr->GetComponentPanelPtr();
		BoardPtr = ComponentPtr->GetComponentBoardPtr();
	}

	if ( NULL == PanelPtr ) 
	{	PanelIndex = -1; }
	else 
	{	PanelIndex = PanelPtr->GetPanelIndex_Project(); }

	if ( NULL == BoardPtr ) 
	{	BoardIndex = -1; }
	else
	{	BoardIndex = BoardPtr->GetBoardIndex_Panel(); }

	::wcscpy(strTag, L"Panel_ID");
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, PanelIndex+1);

	::wcscpy(strTag, L"Board_ID");
	::fwprintf(pfile, L"          \"%s\": %d,\n", strTag, BoardIndex+1);

	::wcscpy(strTag, L"Name");	
	strText = strName;
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	::fwprintf(pfile, L"          \"%s\": \"%s\",\n", strTag, strBuffer);

	::fwprintf(pfile, L"          \"Result\":\n");
	::fwprintf(pfile, L"          {\n");
	
	::wcscpy(strTag, L"STD_X");	
	::fwprintf(pfile, L"            \"%s\": %.2f,\n", strTag, Node.ResultStdX);
	::wcscpy(strTag, L"Gap_X");	
	::fwprintf(pfile, L"            \"%s\": %.2f,\n", strTag, Node.ResultGapX);

	::wcscpy(strTag, L"STD_Y");	
	::fwprintf(pfile, L"            \"%s\": %.2f,\n", strTag, Node.ResultStdY);
	::wcscpy(strTag, L"Gap_Y");	
	::fwprintf(pfile, L"            \"%s\": %.2f,\n", strTag, Node.ResultGapY);

	::wcscpy(strTag, L"STD_L");	
	::fwprintf(pfile, L"            \"%s\": %.2f,\n", strTag, Node.ResultStdL);	
	::wcscpy(strTag, L"Gap_L");	
	::fwprintf(pfile, L"            \"%s\": %.2f,\n", strTag, Node.ResultGapL);

	::wcscpy(strTag, L"STD_Skew");	
	::fwprintf(pfile, L"            \"%s\": %.2f,\n", strTag, Node.ResultSkew);	
	::wcscpy(strTag, L"Gap_Skew");	
	::fwprintf(pfile, L"            \"%s\": %.2f,\n", strTag, Node.ResultGapSkew);

	::wcscpy(strTag, L"Result_Text");	
	strText = Node.ResultText;
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);	
	::fwprintf(pfile, L"            \"%s\": \"%s\",\n", strTag, strBuffer);
	
	SPC_RESULT_ID SpcResultID = AOIDataCollect.MapResultIDToSpcResultID(Node.ResultID);
	::wcscpy(strTag, L"Test_Result");
	::fwprintf(pfile, L"            \"%s\": %d\n", strTag, SpcResultID);

	::fwprintf(pfile, L"          }\n");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::WritePartGroupSpcFile_JSON_RSM(FILE *pfile, bool &bFirst)
{
	if ( NULL == pfile ) { return false; }	

	CAOIPartGroup *PartGroupPtr = this;
	if ( NULL == PartGroupPtr ) { return false; }	
	bool bAddPartGroupNodeList=true;
	PART_GROUP_MODE  PartGroupMode=GetPartGroupMode();
	std::vector<TPartGroupNode*> SavePartGroupNodeList;

	bAddPartGroupNodeList = true;
	switch ( PartGroupMode )
	{
	
	case PART_GROUP_DIST_PART_TO_PART:
		bAddPartGroupNodeList = false;
		SavePartGroupNodeList.push_back(GetPartGroupNodePtr1());
		SavePartGroupNodeList.push_back(GetPartGroupNodePtr2());
		break;

	case PART_GROUP_DIST_PART_TO_GROUP:
		bAddPartGroupNodeList = false;
		SavePartGroupNodeList.push_back(GetPartGroupNodePtr1());		
		break;

	default:
	case PART_GROUP_COLINEARITY:
	case PART_GROUP_COLINEARITY_TO_LINE:
	case PART_GROUP_DIST_PART_NEIGHBOR:	
	case PART_GROUP_DIST_GROUP_TO_PART:
	case PART_GROUP_DIST_GROUP_COORD_MAP:
		bAddPartGroupNodeList = true;
		break;
	}
	if ( true == bAddPartGroupNodeList )
	{
		const size_t PartGroupNodeCount=GetPartGroupNodeCount();
		for ( size_t i=0; i<PartGroupNodeCount; i++ )
		{
			TPartGroupNode *GroupNodePtr=GetPartGroupNodePtr(i, false);
			if ( NULL == GroupNodePtr ) { continue; }
			SavePartGroupNodeList.push_back(GroupNodePtr);
		}
	}

	const size_t SavePartGroupNodeCount=SavePartGroupNodeList.size();
	for ( size_t i=0; i<SavePartGroupNodeCount; i++ )
	{
		TPartGroupNode *GroupNodePtr=SavePartGroupNodeList[i];
		if ( NULL == GroupNodePtr ) { continue; }		
		if ( WritePartGroupNodeSpcFile_JSON_RSM(pfile, *GroupNodePtr, bFirst) == false )
		{	return false; }		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::WritePartGroupNodeSpcFile_JSON_RSM(FILE *pfile, const TPartGroupNode &Node, bool &bFirst)//儲存檢測結果檔案JSON-RSM
{	
	if ( NULL == pfile ) { return false; }	
	CAOIPartGroup *PartGroupPtr = this;
	if ( NULL == PartGroupPtr ) { return false; }	
	if ( NULL == Node.ComponentPtr ) { return false; }

	size_t           i=0;	
	double           Gap=0.0;
	double           STD=0.0;
	double           USL=0.0;
	double           LSL=0.0;
	double           Result=0.0;		
	const size_t     szBuffer = 256;	
	wchar_t          strDir[szBuffer] = L"";
	wchar_t          strUnit[szBuffer] = L"";
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strText[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";	
	wchar_t          strPartName[szBuffer] = L"";	
	CAOIComponent   *ComponentPtr = Node.ComponentPtr;	
	const int        PartGroupID=GetPartGroupGroupID_UI();
	PART_GROUP_MODE  PartGroupMode = GetPartGroupMode();
	PART_GROUP_COLINEARITY_MODE ColinearityMode=GetColinearityMode();	

	::wcscpy(strPartName, ComponentPtr->GetComponentName());
	::_swprintf(strBuffer, L"%d_%s", PartGroupID, strPartName);	

	if ( PART_GROUP_COLINEARITY == PartGroupMode )
	{		
		switch ( ColinearityMode )
		{
		case PART_GROUP_COLINEARITY_X:
			STD = Node.ResultStdX;
			Gap = Node.ResultGapX;
			::wcscpy(strDir, L"X");
			::wcscpy(strUnit, L"um");			
			break;
		case PART_GROUP_COLINEARITY_Y:
			STD = Node.ResultStdY;
			Gap = Node.ResultGapY;
			::wcscpy(strDir, L"Y");
			::wcscpy(strUnit, L"um");
			break;
		case PART_GROUP_COLINEARITY_SKEW:
			STD = Node.ResultSkew;
			Gap = Node.ResultGapSkew;
			::wcscpy(strDir, L"Skew");
			::wcscpy(strUnit, L"");
			break;
		case PART_GROUP_COLINEARITY_HEIGHT:
			STD = Node.ResultStdHeight;
			Gap = Node.ResultGapHeight;
			::wcscpy(strDir, L"H");
			::wcscpy(strUnit, L"um");
			break;
		}
		Result = STD+Gap;				
		USL=PartGroupPtr->GetColinearityGapUSL();
		LSL=PartGroupPtr->GetColinearityGapLSL();
		::_swprintf(strPartName, L"%s_%s", strBuffer, strDir);
		if ( WritePartGroupNodeSpcFile_JSON_RSM(pfile, strPartName, Result, STD, USL, LSL, strUnit, strText, bFirst) == false )
		{	return false; }
	}
	else if ( PART_GROUP_COLINEARITY_TO_LINE == PartGroupMode ) //共線性-對線
	{
		::wcscpy(strDir, L"L");
		::wcscpy(strUnit, L"um");
		STD = Node.ResultStdL;
		Gap = Node.ResultGapL;
		Result = STD+Gap;
		USL=PartGroupPtr->GetColinearityGapUSL();
		LSL=PartGroupPtr->GetColinearityGapLSL();
		::_swprintf(strPartName, L"%s_%s", strBuffer, strDir);
		if ( WritePartGroupNodeSpcFile_JSON_RSM(pfile, strPartName, Result, STD, USL, LSL, strUnit, strText, bFirst) == false )
		{	return false; }
	}
	else
	{	
		bool bDistUsedX = false;
		bool bDistUsedY = false;
		bool bDistUsedL = false;
		switch ( PartGroupMode )
		{		
		case PART_GROUP_DIST_PART_TO_PART:	bDistUsedX = bDistUsedY = bDistUsedL = true;	break;
		case PART_GROUP_DIST_PART_NEIGHBOR:	bDistUsedX = bDistUsedY = bDistUsedL = true;	break;
		case PART_GROUP_DIST_PART_TO_GROUP:	bDistUsedX = bDistUsedY = bDistUsedL = true;	break;
		case PART_GROUP_DIST_GROUP_TO_PART:	bDistUsedX = bDistUsedY = bDistUsedL = true;	break;

		case PART_GROUP_DIST_GROUP_COORD_MAP:	bDistUsedX = bDistUsedY = bDistUsedL = true;	break;
		}
		if ( true==bDistUsedX && GetDistanceGapEnbX() == true )
		{
			::wcscpy(strDir, L"X");
			::wcscpy(strUnit, L"um");
			STD = Node.ResultStdX;
			Gap = Node.ResultGapX;
			Result = STD+Gap;
			USL = GetDistanceGapUSLX();
			LSL = GetDistanceGapLSLX();	
			::_swprintf(strPartName, L"%s_%s", strBuffer, strDir);
			if ( WritePartGroupNodeSpcFile_JSON_RSM(pfile, strPartName, Result, STD, USL, LSL, strUnit, strText, bFirst) == false )
			{	return false; }
		}
		if ( true==bDistUsedY && GetDistanceGapEnbY() == true )
		{
			::wcscpy(strDir, L"Y");
			::wcscpy(strUnit, L"um");
			STD = Node.ResultStdY;
			Gap = Node.ResultGapY;
			Result = STD+Gap;
			USL = GetDistanceGapUSLY();
			LSL = GetDistanceGapLSLY();	
			::_swprintf(strPartName, L"%s_%s", strBuffer, strDir);
			if ( WritePartGroupNodeSpcFile_JSON_RSM(pfile, strPartName, Result, STD, USL, LSL, strUnit, strText, bFirst) == false )
			{	return false; }
		}
		if ( true==bDistUsedL && GetDistanceGapEnbL() == true )
		{
			::wcscpy(strDir, L"L");
			::wcscpy(strUnit, L"um");
			STD = Node.ResultStdL;
			Gap = Node.ResultGapL;
			Result = STD+Gap;
			USL = GetDistanceGapUSLL();
			LSL = GetDistanceGapLSLL();	
			::_swprintf(strPartName, L"%s_%s", strBuffer, strDir);
			if ( WritePartGroupNodeSpcFile_JSON_RSM(pfile, strPartName, Result, STD, USL, LSL, strUnit, strText, bFirst) == false )
			{	return false; }
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::WritePartGroupNodeSpcFile_JSON_RSM(FILE *pfile, wchar_t *pName, double Val, double Std, double Usl, double Lsl, wchar_t *pUnit, wchar_t *pText, bool &bFirst)//儲存檢測結果檔案JSON-RSM
{
	if ( NULL == pfile ) { return false; }	

	if ( false == bFirst )
	{	::fwprintf(pfile, L",\n");	}

	::fwprintf(pfile, L"          {\n");
	
	::fwprintf(pfile, L"          \"%s\": \"%s\",\n", L"measurementItem", pName);
	::fwprintf(pfile, L"          \"%s\": %.2f,\n", L"Value", Val);
	::fwprintf(pfile, L"          \"%s\": %.2f,\n", L"STD", Std);
	::fwprintf(pfile, L"          \"%s\": %.2f,\n", L"USL", Usl);
	::fwprintf(pfile, L"          \"%s\": %.2f,\n", L"LSL", Lsl);
	::fwprintf(pfile, L"          \"%s\": \"%s\",\n", L"Unit", pUnit);
	::fwprintf(pfile, L"          \"%s\": \"%s\",\n", L"Text", pText);//Node.ResultText	
	::fwprintf(pfile, L"          \"%s\": %d\n", L"ID", GetPartGroupMode());

	::fwprintf(pfile, L"          }");
	bFirst = false;
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIProject*  CAOIPartGroup::GetPartGroupProjectPtr() const//取得零件群組專案指標
{
	return m_PartGroupProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetPartGroupProjectPtr(CAOIProject* Ptr)//設定零件群組專案指標
{
	m_PartGroupProjectPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
PART_GROUP_MODE CAOIPartGroup::GetPartGroupMode() const//取得零件群組模式
{
	return m_PartGroupMode;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetPartGroupMode(PART_GROUP_MODE Mode)//設定零件群組模式	
{
	m_PartGroupMode = Mode;
}
//-------------------------------------------------------------------------------------//
const wchar_t* CAOIPartGroup::GetPartGroupName() const//取得零件群組名稱
{
	return m_PartGroupName.c_str();
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetPartGroupName(const char *Name)//設定零件群組名稱
{
	if ( NULL == Name ) { return; }
	JetAPI::char2wstring(Name, m_PartGroupName);
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetPartGroupName(const wchar_t *Name)//設定零件群組名稱
{
	if ( NULL == Name ) { return; }
	m_PartGroupName = Name;
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIPartGroup::GetPartGroupIndex() const//取得零件群組引數
{
	return m_PartGroupIndex;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetPartGroupIndex(unsigned int val)//設定零件群組引數
{
	m_PartGroupIndex = val;
}
//-------------------------------------------------------------------------------------//
int CAOIPartGroup::GetPartGroupUniqueID() const//取得零件群組唯一碼
{
	return m_PartGroupUniqueID;
}
//-------------------------------------------------------------------------------------//
void  CAOIPartGroup::SetPartGroupUniqueID(int val)//設定零件群組唯一碼
{
	m_PartGroupUniqueID = val;
}
//-------------------------------------------------------------------------------------//
int  CAOIPartGroup::GetPartGroupGroupID() const//取得零件群組編號
{
	return m_PartGroupGroupID;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetPartGroupGroupID(int val)//設定零件群組編號
{
	m_PartGroupGroupID = val;
}
//-------------------------------------------------------------------------------------//
int CAOIPartGroup::GetPartGroupGroupID_UI() const//取得零件群組編號-介面
{
	return GetPartGroupGroupID()+1;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetPartGroupGroupID_UI(int val)//設定零件群組編號-介面
{
	if ( val >= 1 )
	{	SetPartGroupGroupID(val-1);	}
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::GetPartGroupInOneBoard() const//取得零件群組同個單板內
{	
	return m_PartGroupInOneBoard;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetPartGroupInOneBoard(bool val)//設定零件群組同個單板內
{
	m_PartGroupInOneBoard = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::GetPartGroupDeleted() const//取得零件群組刪除掉
{
	return m_PartGroupDeleted;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetPartGroupDeleted(bool val)//設定零件群組刪除掉
{
	m_PartGroupDeleted = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::GetPartGroupSelected() const//取得零件群組選取到
{
	return m_PartGroupSelected;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetPartGroupSelected(bool val)//設定零件群組選取到
{
	m_PartGroupSelected = val;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::ClearPartGroupNode1()//清除資料點1
{
	m_PartGroupNode1 = TPartGroupNode();
}
//-------------------------------------------------------------------------------------//
TPartGroupNode* CAOIPartGroup::GetPartGroupNodePtr1()//取得資料點1
{
	return &m_PartGroupNode1;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::GetPartGroupNode1(TPartGroupNode &Node)//取得資料點1
{
	Node = m_PartGroupNode1;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetPartGroupNode1(const TPartGroupNode &Node)//設定資料點1
{
	m_PartGroupNode1 = Node;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::ClearPartGroupNode2()//清除資料點2
{
	m_PartGroupNode2 = TPartGroupNode();
}
//-------------------------------------------------------------------------------------//
TPartGroupNode* CAOIPartGroup::GetPartGroupNodePtr2()//取得資料點2
{
	return &m_PartGroupNode2;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::GetPartGroupNode2(TPartGroupNode &Node)//取得資料點2
{
	Node = m_PartGroupNode2;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetPartGroupNode2(const TPartGroupNode &Node)//設定資料點2
{
	m_PartGroupNode2 = Node;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::ClearPartGroupNode3()//清除資料點3
{
	m_PartGroupNode3 = TPartGroupNode();
}
//-------------------------------------------------------------------------------------//
TPartGroupNode* CAOIPartGroup::GetPartGroupNodePtr3()//取得資料點3
{
	return &m_PartGroupNode3;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::GetPartGroupNode3(TPartGroupNode &Node)//取得資料點3
{
	Node = m_PartGroupNode3;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetPartGroupNode3(const TPartGroupNode &Node)//設定資料點3
{
	m_PartGroupNode3 = Node;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::ClearPartGroupNode4()//清除資料點4
{
	m_PartGroupNode4 = TPartGroupNode();
}
//-------------------------------------------------------------------------------------//
TPartGroupNode* CAOIPartGroup::GetPartGroupNodePtr4()//取得資料點4
{
	return &m_PartGroupNode4;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::GetPartGroupNode4(TPartGroupNode &Node)//取得資料點4
{
	Node = m_PartGroupNode4;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetPartGroupNode4(const TPartGroupNode &Node)//設定資料點4
{
	m_PartGroupNode4 = Node;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::ClearPartGroupNodeList()//清除零件群組節點列表
{
	m_PartGroupNodeList.clear();
}
//-------------------------------------------------------------------------------------//
size_t CAOIPartGroup::GetPartGroupNodeCount() const//取得零件群組節點數
{
	return m_PartGroupNodeList.size();
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::RemovePartGroupNodePanelSelected()//清除選取到的零件群組節點列表
{
	size_t     i=0;
	std::vector<TPartGroupNode> TempNodeList=m_PartGroupNodeList;
	const size_t NodeCount=TempNodeList.size();
	m_PartGroupNodeList.clear();
	for ( i=0; i<NodeCount; i++ )
	{
		TPartGroupNode &GroupNode=TempNodeList[i];
		if ( NULL != GroupNode.PanelPtr ) 
		{
			if ( GroupNode.PanelPtr->GetPanelSelected() == true ) 
			{	continue; }
		}		
		m_PartGroupNodeList.push_back(GroupNode);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::RemovePartGroupNodeBoardSelected()//清除選取到的零件群組節點列表
{
	size_t     i=0;
	std::vector<TPartGroupNode> TempNodeList=m_PartGroupNodeList;
	const size_t NodeCount=TempNodeList.size();
	m_PartGroupNodeList.clear();
	for ( i=0; i<NodeCount; i++ )
	{
		TPartGroupNode &GroupNode=TempNodeList[i];
		if ( NULL != GroupNode.BoardPtr ) 
		{
			if ( GroupNode.BoardPtr->GetBoardSelected() == true ) 
			{	continue; }
		}		
		m_PartGroupNodeList.push_back(GroupNode);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::RemovePartGroupNodeComponentSelected()//清除選取到的零件群組節點列表
{		
	TPartGroupNode* GorupNodePtr=NULL;
	GorupNodePtr=GetPartGroupNodePtr1();
	if ( NULL != GorupNodePtr )
	{
		if ( NULL != GorupNodePtr->ComponentPtr )
		{
			if ( GorupNodePtr->ComponentPtr->GetComponentSelected() == true )
			{	ClearPartGroupNode1();	}
		}
	}
	GorupNodePtr=GetPartGroupNodePtr2();
	if ( NULL != GorupNodePtr )
	{
		if ( NULL != GorupNodePtr->ComponentPtr )
		{
			if ( GorupNodePtr->ComponentPtr->GetComponentSelected() == true )
			{	ClearPartGroupNode2();	}
		}
	}
	GorupNodePtr=GetPartGroupNodePtr3();
	if ( NULL != GorupNodePtr )
	{
		if ( NULL != GorupNodePtr->ComponentPtr )
		{
			if ( GorupNodePtr->ComponentPtr->GetComponentSelected() == true )
			{	ClearPartGroupNode3();	}
		}
	}
	GorupNodePtr=GetPartGroupNodePtr4();
	if ( NULL != GorupNodePtr )
	{
		if ( NULL != GorupNodePtr->ComponentPtr )
		{
			if ( GorupNodePtr->ComponentPtr->GetComponentSelected() == true )
			{	ClearPartGroupNode4();	}
		}
	}

	size_t     i=0;	
	std::vector<TPartGroupNode> TempNodeList=m_PartGroupNodeList;
	const size_t NodeCount=TempNodeList.size();
	m_PartGroupNodeList.clear();
	for ( i=0; i<NodeCount; i++ )
	{
		TPartGroupNode &GroupNode=TempNodeList[i];
		if ( NULL != GroupNode.ComponentPtr ) 
		{
			if ( GroupNode.ComponentPtr->GetComponentSelected() == true ) 
			{	continue; }
		}		
		m_PartGroupNodeList.push_back(GroupNode);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::AddPartGroupNode(const TPartGroupNode &Node)//新增零件群組節點
{
	m_PartGroupNodeList.push_back(Node);
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::AddPartGroupNodeList(const std::vector<TPartGroupNode> &NodeList)//新增零件群組節點
{
	size_t i=0;
	const size_t Count=NodeList.size();
	for ( i=0; i<Count; i++ )
	{	m_PartGroupNodeList.push_back(NodeList[i]); }
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetPartGroupNodeList(const std::vector<TPartGroupNode> &NodeList)//新增零件群組節點
{
	m_PartGroupNodeList = NodeList;
}
//-------------------------------------------------------------------------------------//
TPartGroupNode* CAOIPartGroup::GetPartGroupNodePtr(size_t idx, bool bCheck)//取得零件群組節點指標		
{
	if ( true == bCheck )
	{
		const size_t Count = m_PartGroupNodeList.size();
		if ( idx >= Count ) { return NULL; }
	}
	return &(m_PartGroupNodeList[idx]);
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::ArrangePartGroupNodeList()//整理零件群組節點列表
{
	size_t   i=0, j=0;	
	size_t   GroupNodeNewCnt=0;
	const std::vector<TPartGroupNode> &GroupNodeList=m_PartGroupNodeList;//資料點列表
	const size_t GroupNodeCount = GroupNodeList.size();	
	std::vector<TPartGroupNode> NewGroupNodeList;//資料點列表
	for ( i=0; i<GroupNodeCount; i++ )
	{
		const TPartGroupNode &NodeRef=GroupNodeList[i];
		if ( NULL == NodeRef.ComponentPtr ) { continue; }
		GroupNodeNewCnt = NewGroupNodeList.size();
		for ( j=0; j<GroupNodeNewCnt; j++ )
		{
			const TPartGroupNode &NewNodeRef=NewGroupNodeList[j];
			if ( NULL == NewNodeRef.ComponentPtr ) { continue; }
			if ( NodeRef.ComponentPtr != NewNodeRef.ComponentPtr ) { continue; }
			if ( NodeRef.ModelWndIndex != NewNodeRef.ModelWndIndex ) { continue; }
			break;
		}
		if ( j != GroupNodeNewCnt ) { continue; }
		NewGroupNodeList.push_back(NodeRef);
	}
	m_PartGroupNodeList = NewGroupNodeList;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::SortPartGroupNodeListByPosX()//排序零件群組節點列表-X座標
{
	size_t   i=0;
	size_t   idx=0;
	double   value=0;
	CSortObj SortObj;	
	std::vector<CSortObj>   SortList;	
	const std::vector<TPartGroupNode> &GroupNodeList=m_PartGroupNodeList;//資料點列表
	const size_t GroupNodeCount = GroupNodeList.size();

	SortObj.SetSortMode(SORT_BY_DBL);
	for ( i=0; i<GroupNodeCount; i++ )
	{
		const TPartGroupNode &NodeRef=GroupNodeList[i];
		if ( NULL == NodeRef.ComponentPtr ) { continue; }
		value = NodeRef.ComponentPtr->GetComponentCadPosX();

		SortObj.SetID(i);
		SortObj.SetValueDbl(value);
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());	
	const size_t SortCount=SortList.size();
	std::vector<TPartGroupNode> NewGroupNodeList;//資料點列表
	for ( i=0; i<SortCount; i++ )
	{
		idx = SortList[i].GetID();
		if ( idx >= GroupNodeCount )
		{	return false; }
		NewGroupNodeList.push_back(GroupNodeList[idx]);
	}
	m_PartGroupNodeList = NewGroupNodeList;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::SortPartGroupNodeListByPosY()//排序零件群組節點列表-Y座標
{
	size_t   i=0;
	size_t   idx=0;
	double   value=0;
	CSortObj SortObj;	
	std::vector<CSortObj>   SortList;	
	const std::vector<TPartGroupNode> &GroupNodeList=m_PartGroupNodeList;//資料點列表
	const size_t GroupNodeCount = GroupNodeList.size();

	SortObj.SetSortMode(SORT_BY_DBL);
	for ( i=0; i<GroupNodeCount; i++ )
	{
		const TPartGroupNode &NodeRef=GroupNodeList[i];
		if ( NULL == NodeRef.ComponentPtr ) { continue; }
		value = NodeRef.ComponentPtr->GetComponentCadPosY();

		SortObj.SetID(i);
		SortObj.SetValueDbl(value);
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());	
	const size_t SortCount=SortList.size();
	std::vector<TPartGroupNode> NewGroupNodeList;//資料點列表
	for ( i=0; i<SortCount; i++ )
	{
		idx = SortList[i].GetID();
		if ( idx >= GroupNodeCount )
		{	return false; }
		NewGroupNodeList.push_back(GroupNodeList[idx]);
	}
	m_PartGroupNodeList = NewGroupNodeList;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::SetPartGroupNodeListWndIndex(unsigned int WndIndex)//設定零件群組節點列表-檢測框編號
{
	size_t         i=0;
	size_t         WndCount=0;
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	std::vector<TPartGroupNode> &GroupNodeList=m_PartGroupNodeList;//資料點列表
	const size_t GroupNodeCount = GroupNodeList.size();
	for ( i=0; i<GroupNodeCount; i++ )
	{
		TPartGroupNode &NodeRef=GroupNodeList[i];
		ComponentPtr = NodeRef.ComponentPtr;
		if ( NULL == ComponentPtr ) { continue; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL == ModelPtr ) { continue; }
		WndCount = ModelPtr->GetModelWndCount();
		if ( WndIndex >= WndCount ) { continue; }
		NodeRef.ModelWndIndex = WndIndex;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::SetPartGroupParameterStringByID(PART_GROUP_PARAM_ID ParamID, LPCTSTR String)//設定零件群組參數
{
	int  nValue=0;
	bool bValue=0;
	switch ( ParamID )
	{
	case PART_GROUP_PARAM_GROUP_ID:
		SetPartGroupGroupID_UI(::_ttoi(String));		
		break;
	case PART_GROUP_PARAM_GROUP_NAME://零件群組名稱
		SetPartGroupName(String);
		break;
	case PART_GROUP_PARAM_IN_ONE_BOARD:
		nValue = ::_ttoi(String);
		if ( 0 == nValue ) { bValue = false; }
		else { bValue = true; }
		SetPartGroupInOneBoard(bValue);		
		break;
	case PART_GROUP_PARAM_COLINEARITY_MODE:
		SetColinearityMode((PART_GROUP_COLINEARITY_MODE)::_ttoi(String));
		break;
	case PART_GROUP_PARAM_COLINEARITY_TARGET_MODE:
		SetColinearityTargetMode((PART_GROUP_TARGET_MODE)::_ttoi(String));		
		break;
	case PART_GROUP_PARAM_COLINEARITY_GAP_STD:
		SetColinearityGapStd(::_ttof(String));
		break;
	case PART_GROUP_PARAM_COLINEARITY_GAP_USL:
		SetColinearityGapUSL(::_ttof(String));
		break;
	case PART_GROUP_PARAM_COLINEARITY_GAP_LSL:
		SetColinearityGapLSL(::_ttof(String));
		break;

	case PART_GROUP_PARAM_DISTANCE_GAP_USL_X:
		SetDistanceGapUSLX(::_ttof(String));
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_LSL_X:
		SetDistanceGapLSLX(::_ttof(String));
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_USL_Y:
		SetDistanceGapUSLY(::_ttof(String));
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_LSL_Y:
		SetDistanceGapLSLY(::_ttof(String));
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_USL_L:
		SetDistanceGapUSLL(::_ttof(String));
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_LSL_L:
		SetDistanceGapLSLL(::_ttof(String));
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_ENB_X:
		nValue = ::_ttoi(String);
		if ( 0 == nValue ) { bValue = false; }
		else { bValue = true; }
		SetDistanceGapEnbX(bValue);
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_ENB_Y:
		nValue = ::_ttoi(String);
		if ( 0 == nValue ) { bValue = false; }
		else { bValue = true; }
		SetDistanceGapEnbY(bValue);
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_ENB_L:
		nValue = ::_ttoi(String);
		if ( 0 == nValue ) { bValue = false; }
		else { bValue = true; }
		SetDistanceGapEnbL(bValue);
		break;	
	case PART_GROUP_PARAM_DISTANCE_GAP_STD_ENB:
		nValue = ::_ttoi(String);
		if ( 0 == nValue ) { bValue = false; }
		else { bValue = true; }
		SetDistanceGapStdEnb(bValue);
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_STD_X:
		SetDistanceGapStdX(::_ttof(String));		
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_STD_Y:
		SetDistanceGapStdY(::_ttof(String));		
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_STD_L:
		SetDistanceGapStdL(::_ttof(String));		
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_ADD_X:
		SetDistanceGapAddX(::_ttof(String));
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_ADD_Y:
		SetDistanceGapAddY(::_ttof(String));
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_ENB_ABS:
		nValue = ::_ttoi(String);
		if ( 0 == nValue ) { bValue = false; }
		else { bValue = true; }
		SetDistanceGapEnbAbs(bValue);
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_SCALE_X:
		SetDistanceGapScaleX(::_ttof(String));		
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_SCALE_Y:
		SetDistanceGapScaleY(::_ttof(String));		
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_SCALE_L:
		SetDistanceGapScaleL(::_ttof(String));		
		break;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::GetPartGroupParameterStringByID(PART_GROUP_PARAM_ID ParamID, CString &String)//取得零件群組參數	
{
	switch ( ParamID )
	{
	case PART_GROUP_PARAM_GROUP_ID:
		String.Format(_T("%d"), GetPartGroupGroupID_UI());
		break;
	case PART_GROUP_PARAM_GROUP_NAME:
		String = GetPartGroupName();
		break;
	case PART_GROUP_PARAM_IN_ONE_BOARD:
		String.Format(_T("%d"), GetPartGroupInOneBoard());
		break;
	case PART_GROUP_PARAM_COLINEARITY_MODE:
		String = AOIDataDefine.GetPartGroupColinearityeModeText(GetColinearityMode());		
		break;
	case PART_GROUP_PARAM_COLINEARITY_TARGET_MODE:
		String = AOIDataDefine.GetPartGroupTargetModeText(GetColinearityTargetMode());
		break;
	case PART_GROUP_PARAM_COLINEARITY_GAP_STD:
		String.Format(_T("%.2f"), GetColinearityGapStd());
		break;
	case PART_GROUP_PARAM_COLINEARITY_GAP_USL:
		String.Format(_T("%.2f"), GetColinearityGapUSL());
		break;
	case PART_GROUP_PARAM_COLINEARITY_GAP_LSL:
		String.Format(_T("%.2f"), GetColinearityGapLSL());
		break;

	case PART_GROUP_PARAM_DISTANCE_GAP_USL_X:		
		String.Format(_T("%.2f"), GetDistanceGapUSLX());
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_LSL_X:
		String.Format(_T("%.2f"), GetDistanceGapLSLX());		
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_USL_Y:
		String.Format(_T("%.2f"), GetDistanceGapUSLY());		
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_LSL_Y:
		String.Format(_T("%.2f"), GetDistanceGapLSLY());		
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_USL_L:
		String.Format(_T("%.2f"), GetDistanceGapUSLL());		
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_LSL_L:
		String.Format(_T("%.2f"), GetDistanceGapLSLL());		
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_ENB_X:
		String.Format(_T("%d"), GetDistanceGapEnbX());		
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_ENB_Y:
		String.Format(_T("%d"), GetDistanceGapEnbY());		
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_ENB_L:
		String.Format(_T("%d"), GetDistanceGapEnbL());		
		break;	
	case PART_GROUP_PARAM_DISTANCE_GAP_STD_ENB:
		String.Format(_T("%d"), GetDistanceGapStdEnb());		
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_STD_X:
		String.Format(_T("%.2f"), GetDistanceGapStdX());
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_STD_Y:
		String.Format(_T("%.2f"), GetDistanceGapStdY());
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_STD_L:
		String.Format(_T("%.2f"), GetDistanceGapStdL());
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_ADD_X:
		String.Format(_T("%.2f"), GetDistanceGapAddX());		
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_ADD_Y:
		String.Format(_T("%.2f"), GetDistanceGapAddY());
		break;
	case PART_GROUP_PARAM_DISTANCE_GAP_ENB_ABS:
		String.Format(_T("%d"), GetDistanceGapEnbAbs());		
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::CheckPartGroupNodeParamID(PART_GROUP_PARAM_ID ParamID) const//確認是零件群組節點參數編號
{
	if ( ParamID < PART_GROUP_NODE_BEGIN ) { return false; }
	if ( ParamID > PART_GROUP_NODE_END ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::SetPartGroupNodeParameterStringByID(TPartGroupNode &Node, PART_GROUP_PARAM_ID ParamID, LPCTSTR String)//設定零件群組節點參數
{
	int  nValue=0;
	bool bValue=0;
	switch ( ParamID )
	{
	case PART_GROUP_NODE_MAP_DIR_MODE:
		Node.MapDirMode = (PART_GROUP_MAP_DIR_MODE)(::_ttoi(String));
		break;
	case PART_GROUP_NODE_USER_MAP_CAD_POS_X:
		Node.UserMapCadPosX = ::_ttof(String);
		break;
	case PART_GROUP_NODE_USER_MAP_CAD_POS_Y:
		Node.UserMapCadPosY = ::_ttof(String);
		break;
	case PART_GROUP_NODE_USER_MAP_CAD_DIS_L:
		Node.UserMapCadDisL = ::_ttof(String);
		break;
	case PART_GROUP_NODE_USER_MAP_CAD_ENABLED:
		nValue = ::_ttoi(String);
		if ( 0 == nValue ) { bValue = false; }
		else { bValue = true; }
		Node.UserMapCadEnable = bValue;
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::GetPartGroupNodeParameterStringByID(TPartGroupNode &Node, PART_GROUP_PARAM_ID ParamID, CString &String)//取得零件群組節點參數
{
	switch ( ParamID )
	{
	case PART_GROUP_NODE_MAP_DIR_MODE:
		String.Format(_T("%d"), Node.MapDirMode);
		break;
	case PART_GROUP_NODE_USER_MAP_CAD_POS_X:
		String.Format(_T("%.2f"), Node.UserMapCadPosX);
		break;	
	case PART_GROUP_NODE_USER_MAP_CAD_POS_Y:
		String.Format(_T("%.2f"), Node.UserMapCadPosY);
		break;
	case PART_GROUP_NODE_USER_MAP_CAD_DIS_L:
		String.Format(_T("%.2f"), Node.UserMapCadDisL);
		break;
	case PART_GROUP_NODE_USER_MAP_CAD_ENABLED:
		String.Format(_T("%d"), Node.UserMapCadEnable);
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
PART_GROUP_COLINEARITY_MODE CAOIPartGroup::GetColinearityMode() const//取得共線性模式
{
	return m_ColinearityMode;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetColinearityMode(PART_GROUP_COLINEARITY_MODE val)//設定共線性模式
{
	m_ColinearityMode = val;
}
//-------------------------------------------------------------------------------------//
PART_GROUP_TARGET_MODE CAOIPartGroup::GetColinearityTargetMode() const//取得共線性標準值模式
{
	return m_ColinearityTargetMode;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetColinearityTargetMode(PART_GROUP_TARGET_MODE val)//設定共線性標準值模式
{
	m_ColinearityTargetMode = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetColinearityGapStd() const//取得共線性偏差標準
{
	return m_ColinearityGapStd;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetColinearityGapStd(double val)//設定共線性偏差標準
{
	m_ColinearityGapStd = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetColinearityGapUSL() const//取得共線性偏差上限
{
	return m_ColinearityGapUSL;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetColinearityGapUSL(double val)//設定共線性偏差上限
{
	m_ColinearityGapUSL = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetColinearityGapLSL() const//取得共線性偏差下限
{
	return m_ColinearityGapLSL;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetColinearityGapLSL(double val)//設定共線性偏差下限
{
	m_ColinearityGapLSL = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::GetDistanceGapEnbX() const//取得距離偏差啟用-X
{
	return m_DistanceGapEnbX;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapEnbX(bool val)//設定距離偏差啟用-X
{
	m_DistanceGapEnbX = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::GetDistanceGapEnbY() const//取得距離偏差啟用-Y
{
	return m_DistanceGapEnbY;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapEnbY(bool val)//設定距離偏差啟用-Y
{
	m_DistanceGapEnbY = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::GetDistanceGapEnbL() const//取得距離偏差啟用-L
{
	return m_DistanceGapEnbL;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapEnbL(bool val)//設定距離偏差啟用-L
{
	m_DistanceGapEnbL = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetDistanceGapUSLX() const//取得距離偏差上限-X
{
	return m_DistanceGapUSLX;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapUSLX(double val)//設定距離偏差上限-X
{
	m_DistanceGapUSLX = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetDistanceGapLSLX() const//取得距離偏差下限-X
{
	return m_DistanceGapLSLX;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapLSLX(double val)//設定距離偏差下限-X
{
	m_DistanceGapLSLX = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetDistanceGapUSLY() const//取得距離偏差上限-Y
{
	return m_DistanceGapUSLY;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapUSLY(double val)//設定距離偏差上限-Y
{
	m_DistanceGapUSLY = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetDistanceGapLSLY() const//取得距離偏差下限-Y
{
	return m_DistanceGapLSLY;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapLSLY(double val)//設定距離偏差下限-Y
{
	m_DistanceGapLSLY = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetDistanceGapUSLL() const//取得距離偏差上限-L
{
	return m_DistanceGapUSLL;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapUSLL(double val)//設定距離偏差上限-L
{
	m_DistanceGapUSLL = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetDistanceGapLSLL() const//取得距離偏差下限-L
{
	return m_DistanceGapLSLL;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapLSLL(double val)//設定距離偏差下限-L
{
	m_DistanceGapLSLL = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::GetDistanceGapStdEnb() const//取得距離偏差啟用標準
{
	return m_DistanceGapStdEnb;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapStdEnb(bool val)//設定距離偏差啟用標準
{
	m_DistanceGapStdEnb = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetDistanceGapStdX() const//取得距離偏差標準-X
{
	return m_DistanceGapStdX;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapStdX(double val)//設定距離偏差標準-X
{
	m_DistanceGapStdX = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetDistanceGapStdY() const//取得距離偏差標準-Y
{
	return m_DistanceGapStdY;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapStdY(double val)//設定距離偏差標準-Y
{
	m_DistanceGapStdY = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetDistanceGapStdL() const//取得距離偏差標準-L
{
	return m_DistanceGapStdL;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapStdL(double val)//設定距離偏差標準-L
{
	m_DistanceGapStdL = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetDistanceGapAddX() const//取得距離偏差加值-X
{
	return m_DistanceGapAddX;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapAddX(double val)//設定距離偏差加值-X
{
	m_DistanceGapAddX = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetDistanceGapAddY() const//取得距離偏差加值-Y
{
	return m_DistanceGapAddY;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapAddY(double val)//設定距離偏差加值-Y
{
	m_DistanceGapAddY = val;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::GetDistanceGapEnbAbs() const//取得距離偏差啟用絕對值
{
	return m_DistanceGapEnbAbs;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapEnbAbs(bool val)//設定距離偏差啟用絕對值
{
	m_DistanceGapEnbAbs = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetDistanceGapScaleX() const//距離偏差倍率-X
{
	return m_DistanceGapScaleX;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapScaleX(double val)//距離偏差倍率-X
{
	m_DistanceGapScaleX = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetDistanceGapScaleY() const//距離偏差倍率-Y
{
	return m_DistanceGapScaleY;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapScaleY(double val)//距離偏差倍率-Y
{
	m_DistanceGapScaleY = val;
}
//-------------------------------------------------------------------------------------//
double CAOIPartGroup::GetDistanceGapScaleL() const//距離偏差倍率-L
{
	return m_DistanceGapScaleL;
}
//-------------------------------------------------------------------------------------//
void CAOIPartGroup::SetDistanceGapScaleL(double val)//距離偏差倍率-L
{
	m_DistanceGapScaleL = val;
}
//-------------------------------------------------------------------------------------//	
bool  CAOIPartGroup::InitPartGroupInspection()//初始化零件群組檢測
{
	size_t         i=0;	
	const size_t GroupNodeCount=m_PartGroupNodeList.size();
	m_PartGroupNode1.InitPartGroupNodeTest();	
	m_PartGroupNode2.InitPartGroupNodeTest();
	m_PartGroupNode3.InitPartGroupNodeTest();	
	m_PartGroupNode4.InitPartGroupNodeTest();
	for ( i=0; i<GroupNodeCount; i++ )
	{	m_PartGroupNodeList[i].InitPartGroupNodeTest();		}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::SetGroupComponentKeepImage()//設定零件群組保留圖像
{
	size_t       i=0;
	const bool   bKeepImage=true;
	const size_t GroupNodeCount=m_PartGroupNodeList.size();
	if ( NULL != m_PartGroupNode1.ComponentPtr )
	{	m_PartGroupNode1.ComponentPtr->SetComponentKeepImage(bKeepImage); }
	if ( NULL != m_PartGroupNode2.ComponentPtr )
	{	m_PartGroupNode2.ComponentPtr->SetComponentKeepImage(bKeepImage); }	
	if ( NULL != m_PartGroupNode3.ComponentPtr )
	{	m_PartGroupNode3.ComponentPtr->SetComponentKeepImage(bKeepImage); }	
	if ( NULL != m_PartGroupNode4.ComponentPtr )
	{	m_PartGroupNode4.ComponentPtr->SetComponentKeepImage(bKeepImage); }	
	for ( i=0; i<GroupNodeCount; i++ )
	{	
		TPartGroupNode &PartGroupNode = m_PartGroupNodeList[i];		
		if ( NULL != PartGroupNode.ComponentPtr )
		{	PartGroupNode.ComponentPtr->SetComponentKeepImage(bKeepImage); }	
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIPartGroup::BuildPartGroupDefectText(LPCTSTR Text) const//建立零件群組瑕疵文字
{
	CString str;
	str.Format(_T("[%s]-[%s]"), CString(GetPartGroupName()), Text);
	return str;
}
//-------------------------------------------------------------------------------------//
CAOIBoard* CAOIPartGroup::GetPartGroupNodeBoardPtr()//取得零件群組節點單板指標
{
	TPartGroupNode *PartGroupNodePtr = NULL;

	PartGroupNodePtr = GetPartGroupNodePtr1();
	if ( NULL != PartGroupNodePtr )
	{
		if ( NULL != PartGroupNodePtr->ComponentPtr ) 
		{	return PartGroupNodePtr->ComponentPtr->GetComponentBoardPtr(); }
	}
	PartGroupNodePtr = GetPartGroupNodePtr2();
	if ( NULL != PartGroupNodePtr )
	{
		if ( NULL != PartGroupNodePtr->ComponentPtr ) 
		{	return PartGroupNodePtr->ComponentPtr->GetComponentBoardPtr(); }
	}
	PartGroupNodePtr = GetPartGroupNodePtr3();
	if ( NULL != PartGroupNodePtr )
	{
		if ( NULL != PartGroupNodePtr->ComponentPtr ) 
		{	return PartGroupNodePtr->ComponentPtr->GetComponentBoardPtr(); }
	}
	PartGroupNodePtr = GetPartGroupNodePtr4();
	if ( NULL != PartGroupNodePtr )
	{
		if ( NULL != PartGroupNodePtr->ComponentPtr ) 
		{	return PartGroupNodePtr->ComponentPtr->GetComponentBoardPtr(); }
	}

	size_t       i=0;
	const size_t PartGroupNodeCnt=GetPartGroupNodeCount();
	for ( i=0; i<PartGroupNodeCnt; i++ )
	{
		PartGroupNodePtr = GetPartGroupNodePtr(i, false);
		if ( NULL == PartGroupNodePtr ) { continue; }
		if ( NULL == PartGroupNodePtr->ComponentPtr ) { continue; }
		return PartGroupNodePtr->ComponentPtr->GetComponentBoardPtr();
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::CheckPartGroupNodeInOneBoard()//確認零件群組節點在同一個單板內	
{	
	CAOIBoard      *RefBoardPtr = NULL;
	TPartGroupNode *PartGroupNodePtr = NULL;

	RefBoardPtr = GetPartGroupNodeBoardPtr();
	if ( NULL == RefBoardPtr ) { return false; }

	PartGroupNodePtr = GetPartGroupNodePtr1();
	if ( NULL != PartGroupNodePtr )
	{
		if ( NULL != PartGroupNodePtr->ComponentPtr ) 
		{	
			if ( PartGroupNodePtr->ComponentPtr->GetComponentBoardPtr() != RefBoardPtr )
			{	return false; }
		}
	}
	PartGroupNodePtr = GetPartGroupNodePtr2();
	if ( NULL != PartGroupNodePtr )
	{
		if ( NULL != PartGroupNodePtr->ComponentPtr ) 
		{
			if ( PartGroupNodePtr->ComponentPtr->GetComponentBoardPtr() != RefBoardPtr )
			{	return false; }
		}
	}
	PartGroupNodePtr = GetPartGroupNodePtr3();
	if ( NULL != PartGroupNodePtr )
	{
		if ( NULL != PartGroupNodePtr->ComponentPtr ) 
		{
			if ( PartGroupNodePtr->ComponentPtr->GetComponentBoardPtr() != RefBoardPtr )
			{	return false; }
		}
	}
	PartGroupNodePtr = GetPartGroupNodePtr4();
	if ( NULL != PartGroupNodePtr )
	{
		if ( NULL != PartGroupNodePtr->ComponentPtr ) 
		{
			if ( PartGroupNodePtr->ComponentPtr->GetComponentBoardPtr() != RefBoardPtr )
			{	return false; }
		}
	}

	size_t       i=0;
	const size_t PartGroupNodeCnt=GetPartGroupNodeCount();
	for ( i=0; i<PartGroupNodeCnt; i++ )
	{
		PartGroupNodePtr = GetPartGroupNodePtr(i, false);
		if ( NULL == PartGroupNodePtr ) { continue; }
		if ( NULL == PartGroupNodePtr->ComponentPtr ) { continue; }
		if ( PartGroupNodePtr->ComponentPtr->GetComponentBoardPtr() != RefBoardPtr )
		{	return false; }		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::ChangePartGroupNodeBoardPtr(CAOIBoard *BoardPtr)//變更零件群組節點的單板
{
	CString PartName;
	CAOIComponent  *ComponentPtr = NULL;
	TPartGroupNode *PartGroupNodePtr = NULL;

	PartGroupNodePtr = GetPartGroupNodePtr1();
	if ( NULL != PartGroupNodePtr )
	{
		if ( NULL != PartGroupNodePtr->ComponentPtr ) 
		{	
			PartName = PartGroupNodePtr->ComponentPtr->GetComponentName();
			ComponentPtr = BoardPtr->GetBoardComponentPtrByName(PartName);
			if ( NULL == ComponentPtr ) { return false; }			
			PartGroupNodePtr->SetComponentPtr(ComponentPtr);
		}
	}
	PartGroupNodePtr = GetPartGroupNodePtr2();
	if ( NULL != PartGroupNodePtr )
	{
		if ( NULL != PartGroupNodePtr->ComponentPtr ) 
		{	
			PartName = PartGroupNodePtr->ComponentPtr->GetComponentName();
			ComponentPtr = BoardPtr->GetBoardComponentPtrByName(PartName);
			if ( NULL == ComponentPtr ) { return false; }		
			PartGroupNodePtr->SetComponentPtr(ComponentPtr);
		}
	}
	PartGroupNodePtr = GetPartGroupNodePtr3();
	if ( NULL != PartGroupNodePtr )
	{
		if ( NULL != PartGroupNodePtr->ComponentPtr ) 
		{	
			PartName = PartGroupNodePtr->ComponentPtr->GetComponentName();
			ComponentPtr = BoardPtr->GetBoardComponentPtrByName(PartName);
			if ( NULL == ComponentPtr ) { return false; }
			PartGroupNodePtr->SetComponentPtr(ComponentPtr);
		}
	}
	PartGroupNodePtr = GetPartGroupNodePtr4();
	if ( NULL != PartGroupNodePtr )
	{
		if ( NULL != PartGroupNodePtr->ComponentPtr ) 
		{	
			PartName = PartGroupNodePtr->ComponentPtr->GetComponentName();
			ComponentPtr = BoardPtr->GetBoardComponentPtrByName(PartName);
			if ( NULL == ComponentPtr ) { return false; }
			PartGroupNodePtr->SetComponentPtr(ComponentPtr);
		}
	}

	size_t       i=0;
	const size_t PartGroupNodeCnt=GetPartGroupNodeCount();
	for ( i=0; i<PartGroupNodeCnt; i++ )
	{
		PartGroupNodePtr = GetPartGroupNodePtr(i, false);
		if ( NULL == PartGroupNodePtr ) { continue; }
		if ( NULL == PartGroupNodePtr->ComponentPtr ) { continue; }
		PartName = PartGroupNodePtr->ComponentPtr->GetComponentName();
		ComponentPtr = BoardPtr->GetBoardComponentPtrByName(PartName);
		if ( NULL == ComponentPtr ) { return false; }		
		PartGroupNodePtr->SetComponentPtr(ComponentPtr);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::CopyPartGroupParam(const CAOIPartGroup &PartGroup)
{
	if ( this == &PartGroup ) { return true; }
	if ( GetPartGroupGroupID() != PartGroup.GetPartGroupGroupID() ) { return false; }

	unsigned int    PartGroupIndex=m_PartGroupIndex;
	int             PartGroupUniqueID=m_PartGroupUniqueID;
	TPartGroupNode  PartGroupNode1=m_PartGroupNode1;
	TPartGroupNode  PartGroupNode2=m_PartGroupNode2;
	TPartGroupNode  PartGroupNode3=m_PartGroupNode3;
	TPartGroupNode  PartGroupNode4=m_PartGroupNode4;	
	std::vector<TPartGroupNode> PartGroupNodeList=m_PartGroupNodeList;

	ClonePartGroup(PartGroup);

	m_PartGroupIndex=PartGroupIndex;
	m_PartGroupUniqueID=PartGroupUniqueID;
	m_PartGroupNode1=PartGroupNode1;
	m_PartGroupNode2=PartGroupNode2;
	m_PartGroupNode3=PartGroupNode3;
	m_PartGroupNode4=PartGroupNode4;
	m_PartGroupNodeList=PartGroupNodeList;

	bool bSucc = true;
	if ( CopyPartNodeParam(m_PartGroupNode1, PartGroup.m_PartGroupNode1) == false )
	{	bSucc = false; }
	if ( CopyPartNodeParam(m_PartGroupNode2, PartGroup.m_PartGroupNode2) == false )
	{	bSucc = false; }
	if ( CopyPartNodeParam(m_PartGroupNode3, PartGroup.m_PartGroupNode3) == false )
	{	bSucc = false; }
	if ( CopyPartNodeParam(m_PartGroupNode4, PartGroup.m_PartGroupNode4) == false )
	{	bSucc = false; }
	
	const size_t NodeCount=m_PartGroupNodeList.size();
	if ( NodeCount == PartGroup.m_PartGroupNodeList.size() )
	{
		for ( size_t i=0; i<NodeCount; i++ )
		{
			if ( CopyPartNodeParam(m_PartGroupNodeList[i], PartGroup.m_PartGroupNodeList[i]) == false )
			{	bSucc = false; }
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPartGroup::CopyPartNodeParam(TPartGroupNode &Node, const TPartGroupNode &RefNode)//複製其他同群組節點的資料
{
	if ( NULL == Node.ComponentPtr ) { return true; }
	if ( NULL == RefNode.ComponentPtr ) { return true; }
	if ( Node.ComponentPtr == RefNode.ComponentPtr ) { return true; }
	CAOIComponent *ComponentPtr = Node.ComponentPtr;
	const CAOIComponent *RefComponentPtr = RefNode.ComponentPtr;
	CString ComponentName = ComponentPtr->GetComponentName();
	CString RefComponentName = RefComponentPtr->GetComponentName();
	if ( ComponentName.CompareNoCase(RefComponentName) != 0 ) { return false; }

	//Node.PanelIndex = -1;
	//Node.BoardIndex = -1;
	//Node.ComponentIndex = -1;
	//Node.ModelWndIndex = -1;
	Node.MapDirMode = RefNode.MapDirMode;	

	Node.UserMapCadPosX = RefNode.UserMapCadPosX;
	Node.UserMapCadPosY = RefNode.UserMapCadPosY;
	Node.UserMapCadDisL = RefNode.UserMapCadDisL;
	Node.UserMapCadEnable = RefNode.UserMapCadEnable;	
	return true;
}
//-------------------------------------------------------------------------------------//