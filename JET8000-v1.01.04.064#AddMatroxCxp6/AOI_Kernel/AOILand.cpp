// AOILand.cpp: implementation of the CAOILand class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOILand.h"
//-------------------------------------------------------------------------------------//
#include "AOIModel.h"
#include "AOIFileIO.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CAOILand, CAOIObj)
//-------------------------------------------------------------------------------------//
bool CAOILand::GetLandTypeUsePad(LAND_TYPE LandType)//確認焊接樣式使用焊盤
{
	if ( LAND_TYPE_NULL == LandType ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::GetLandTypeUseLead(LAND_TYPE LandType)//確認焊接樣式使用引腳
{
	if ( LAND_TYPE_ELECTRODE == LandType ) { return true; }
	if ( LAND_TYPE_IC_LEAD == LandType ) { return true; }
	//if ( LAND_TYPE_CON_LEAD == LandType ) { return true; }
	if ( LAND_TYPE_DIP_LEAD == LandType ) { return true; }
	return false;	
}
//-------------------------------------------------------------------------------------//
bool CAOILand::GetLandTypeUseLeadTip(LAND_TYPE LandType)//確認焊接樣式使用引腳前端
{
	if ( LAND_TYPE_IC_LEAD == LandType ) { return true; }
	if ( LAND_TYPE_CON_LEAD == LandType ) { return true; }
	return false;	
}
//-------------------------------------------------------------------------------------//
bool CAOILand::GetLandTypeUseLeadShoulder(LAND_TYPE LandType)//確認焊接樣式使用引腳肩部
{
	if ( LAND_TYPE_IC_LEAD == LandType ) { return true; }
	if ( LAND_TYPE_CON_LEAD == LandType ) { return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::GetLandTypeUseLeadTipShoulder(LAND_TYPE LandType)//確認焊接樣式使用引腳前端+肩部
{
	if ( LAND_TYPE_IC_LEAD == LandType ) { return true; }
	if ( LAND_TYPE_CON_LEAD == LandType ) { return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
CAOILand::CAOILand():CAOIObj(AOI_OBJ_LAND)
{
	PreInitLand();
	InitialLand();
}
//-------------------------------------------------------------------------------------//
CAOILand::CAOILand(const CAOILand &Land):CAOIObj(Land)
{
	PreInitLand();
	CloneLand(Land);
}
//-------------------------------------------------------------------------------------//
CAOILand::~CAOILand()
{
	ReleaseLandObj();
}
//-------------------------------------------------------------------------------------//
CAOILand& CAOILand::operator=(const CAOILand &Land)
{
	if ( &Land == this ) { return *this; }
	CAOIObj::operator=(Land);
	CloneLand(Land);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CAOILand::PreInitLand()
{	
}
//-------------------------------------------------------------------------------------//
inline void CAOILand::InitialLand()
{
	m_LandType = LAND_TYPE_ELECTRODE;
	m_LandIndex = -1;
	m_LandGroupID = 0;
	m_LandAlignID = 0;
	m_LandModified = false;
	m_LandFirstOne = false;
	m_LandLastOne = false;
	m_LandIncludePadAlign = true;
	m_LandIncludePartAlign = true;	
	
	m_LandTempInt[0] = 0;
	m_LandTempInt[1] = 0;
	m_LandTempInt[2] = 0;
	m_LandTempInt[3] = 0;
	m_LandLeadSizeX = 0;
	m_LandLeadSizeY = 0;
	m_LandLeadHeight = 0;	
	m_LandLeadTipSizeX = 0;
	m_LandLeadTipSizeY = 0;
	m_LandLeadTipHeight = 0;
	m_LandLeadShoulderSizeX = 0;
	m_LandLeadShoulderSizeY = 0;
	m_LandLeadShoulderHeight = 0;	
	m_LandModelPtr = NULL;
	m_LandWndList.clear();
	m_LandLogicList.clear();
	m_LandRegion = TREGION4D();
	m_LandName = L"";
	InitialLandColorGroup();
	UpdateLandBoxPtr();	
}
//-------------------------------------------------------------------------------------//
void CAOILand::InitialLandColorGroup()
{
	m_LandLeadColorGroup.BuildColorGroup_Test();
	m_LandLeadColorGroup.SetColorGroupFrameIndex(0);
	m_LandLeadColorGroup.SetColorGroupFrameUniqueID(FRAME_UNIQUE_ID_TOP);
	return;
}
//-------------------------------------------------------------------------------------//
inline void CAOILand::CloneLand(const CAOILand &Land)
{
	m_LandType = Land.m_LandType;
	m_LandIndex = Land.m_LandIndex;
	m_LandGroupID = Land.m_LandGroupID;	
	m_LandAlignID = Land.m_LandAlignID;	
	m_LandModified = Land.m_LandModified;	
	m_LandFirstOne = Land.m_LandFirstOne;
	m_LandLastOne = Land.m_LandLastOne;	
	m_LandIncludePadAlign = Land.m_LandIncludePadAlign;	
	m_LandIncludePartAlign = Land.m_LandIncludePartAlign;	
	m_LandTempInt[0] = Land.m_LandTempInt[0];
	m_LandTempInt[1] = Land.m_LandTempInt[1];
	m_LandTempInt[2] = Land.m_LandTempInt[2];
	m_LandTempInt[3] = Land.m_LandTempInt[3];

	m_LandLeadSizeX = Land.m_LandLeadSizeX;
	m_LandLeadSizeY = Land.m_LandLeadSizeY;
	m_LandLeadHeight = Land.m_LandLeadHeight;
	m_LandLeadTipSizeX = Land.m_LandLeadTipSizeX;
	m_LandLeadTipSizeY = Land.m_LandLeadTipSizeY;
	m_LandLeadTipHeight = Land.m_LandLeadTipHeight;
	m_LandLeadShoulderSizeX = Land.m_LandLeadShoulderSizeX;
	m_LandLeadShoulderSizeY = Land.m_LandLeadShoulderSizeY;
	m_LandLeadShoulderHeight = Land.m_LandLeadShoulderHeight;
	m_LandLeadColorGroup = Land.m_LandLeadColorGroup;

	m_LandModelPtr = Land.m_LandModelPtr;

	m_LandWndList = Land.m_LandWndList;
	m_LandLogicList = Land.m_LandLogicList;

	m_LandRegion = Land.m_LandRegion;	

	m_LandPadBox = Land.m_LandPadBox;
	m_LandLeadBox = Land.m_LandLeadBox;
	m_LandLeadTipBox = Land.m_LandLeadTipBox;
	m_LandLeadShoulderBox = Land.m_LandLeadShoulderBox;
	m_LandBodyEdgeBox = Land.m_LandBodyEdgeBox;
	UpdateLandBoxPtr();
}
//-------------------------------------------------------------------------------------//
void CAOILand::ResetLandObj()//復歸Land物件
{
	InitialLand();
}
//-------------------------------------------------------------------------------------//
void CAOILand::ReleaseLandObj()//釋放Land物件
{
}
//-------------------------------------------------------------------------------------//
CAOILand* CAOILand::CloneLandObj() const//建立且複製一個特徵框
{
	CAOILand *ObjPtr = AOIObjManager.CreateLandObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::WriteLandFile(CAOIFileIO &FileIO)//儲存特徵框檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	
	CAOILand *LandPtr = this;
	char      uuidStr[MAX_JET_PATH]="";	
	wchar_t   uuidWStr[MAX_JET_PATH]=L"";	
	UUID      uuid = LandPtr->GetObjUuid();	
	FileIO.SetFnName(_T("CAOILand::WriteLandFile"));
	JetAPI::UUIDToStringA(uuid, uuidStr);
	JetAPI::UUIDToStringW(uuid, uuidWStr);
	//----------------------------------------------------------------------------------------//
	CAOIBox  *BoxPtr = NULL;

	if ( FileIO.SaveChunk_INT(FILE_IO_LAND_START, 0) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_LAND_TYPE, LandPtr->GetLandType()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_LAND_GROUP_ID, LandPtr->GetLandGroupID()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_LAND_ALIGN_ID, LandPtr->GetLandAlignID()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_LAND_FIRST_ONE, LandPtr->GetLandFirstOne()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_LAND_LAST_ONE, LandPtr->GetLandLastOne()) == false ) { return false; }
	if ( FileIO.GetSaveWStr() == true ) 
	{	if ( FileIO.SaveChunk_STR(FILE_IO_LAND_OBJ_UUID, uuidWStr) == false ) { return false; } }
	else
	{	if ( FileIO.SaveChunk_STR(FILE_IO_LAND_OBJ_UUID, uuidStr) == false ) { return false; } }
	if ( FileIO.SaveChunk_BOL(FILE_IO_LAND_INCLUDE_PAD_ALIGN, LandPtr->GetLandIncludePadAlign()) == false ) { return false; }	
	if ( FileIO.SaveChunk_BOL(FILE_IO_LAND_INCLUDE_PART_ALIGN, LandPtr->GetLandIncludePartAlign()) == false ) { return false; }		

	if ( FileIO.SaveChunk_DBL(FILE_IO_LAND_LEAD_SIZE_X, LandPtr->GetLandLeadSizeX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_LAND_LEAD_SIZE_Y, LandPtr->GetLandLeadSizeY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_LAND_LEAD_HEIGHT, LandPtr->GetLandLeadHeight()) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_LAND_LEAD_TIP_SIZE_X, LandPtr->GetLandLeadTipSizeX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_LAND_LEAD_TIP_SIZE_Y, LandPtr->GetLandLeadTipSizeY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_LAND_LEAD_TIP_HEIGHT, LandPtr->GetLandLeadTipHeight()) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_LAND_LEAD_SHOULDER_SIZE_X, LandPtr->GetLandLeadShoulderSizeX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_LAND_LEAD_SHOULDER_SIZE_Y, LandPtr->GetLandLeadShoulderSizeY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_LAND_LEAD_SHOULDER_HEIGHT, LandPtr->GetLandLeadShoulderHeight()) == false ) { return false; }
	
	//基本框參數
	BoxPtr = LandPtr->GetLandPadBoxPtr();
	if ( NULL != BoxPtr )
	{
		if ( FileIO.SaveChunk_INT(FILE_IO_PAD_BOX, 0) == false ) { return false; }
		if ( BoxPtr->WriteBoxFile(FileIO) == false ) { return false; }		
	}
	BoxPtr = LandPtr->GetLandLeadBoxPtr();
	if ( NULL != BoxPtr )
	{
		if ( FileIO.SaveChunk_INT(FILE_IO_LEAD_BOX, 0) == false ) { return false; }
		if ( BoxPtr->WriteBoxFile(FileIO) == false ) { return false; }
	}
	BoxPtr = LandPtr->GetLandLeadTipBoxPtr();
	if ( NULL != BoxPtr )
	{
		if ( FileIO.SaveChunk_INT(FILE_IO_LEAD_TIP_BOX, 0) == false ) { return false; }
		if ( BoxPtr->WriteBoxFile(FileIO) == false ) { return false; }
	}
	BoxPtr = LandPtr->GetLandLeadShoulderBoxPtr();
	if ( NULL != BoxPtr )
	{
		if ( FileIO.SaveChunk_INT(FILE_IO_LEAD_SHOULDER_BOX, 0) == false ) { return false; }
		if ( BoxPtr->WriteBoxFile(FileIO) == false ) { return false; }
	}
	BoxPtr = LandPtr->GetLandBodyEdgeBoxPtr();
	if ( NULL != BoxPtr )
	{
		if ( FileIO.SaveChunk_INT(FILE_IO_BODY_EDGE_BOX, 0) == false ) { return false; }
		if ( BoxPtr->WriteBoxFile(FileIO) == false ) { return false; }
	}

	if ( FileIO.SaveChunk_INT(FILE_IO_LAND_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::ReadLandFile(CAOIFileIO &FileIO)//載入特徵框檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	
	UUID      uuid;
	int       index = 0;	
	double    dValue = 0.0;
	CAOILand *LandPtr = this;
	CAOIBox  *BoxPtr = NULL;	
	FileIO.SetFnName(_T("CAOILand::ReadLandFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }		
		switch ( index )
		{
		case FILE_IO_LAND_START://特徵框參數-起點
			break;
		case FILE_IO_LAND_END://特徵框參數-終點
			//LandPtr->UpdateWndExtendBox();			
			LandPtr->SetLandModified(false);
			return true;
			break;
		case FILE_IO_LAND_TYPE://特徵框參數-樣式
			LandPtr->SwitchLandType((LAND_TYPE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_LAND_GROUP_ID://特徵框參數-群組編號
			LandPtr->SetLandGroupID((FileIO.GetData_INT()));
			break;
		case FILE_IO_LAND_ALIGN_ID://特徵框參數-對齊編號
			LandPtr->SetLandAlignID((FileIO.GetData_INT()));
			break;
		case FILE_IO_LAND_FIRST_ONE://特徵框參數-單側第一個位置			
			LandPtr->SetLandFirstOne(FileIO.GetData_BOL());
			break;
		case FILE_IO_LAND_LAST_ONE://特徵框參數-單側第末個位置			
			LandPtr->SetLandLastOne(FileIO.GetData_BOL());
			break;
		case FILE_IO_LAND_OBJ_UUID:
			if ( FileIO.GetLoadWStr()==true )
			{
				if ( JetAPI::UUIDFromStringW(FileIO.GetData_WSTR(), uuid) == true )
				{	LandPtr->SetObjUuid(uuid);	}
			}
			else
			{
				if ( JetAPI::UUIDFromStringA(FileIO.GetData_STR(), uuid) == true )
				{	LandPtr->SetObjUuid(uuid);	}
			}
			break;
		case FILE_IO_LAND_INCLUDE_PAD_ALIGN://特徵框參數-加入焊盤定位
			LandPtr->SetLandIncludePadAlign(FileIO.GetData_BOL());
			break;
		case FILE_IO_LAND_INCLUDE_PART_ALIGN://特徵框參數-加入本體定位
			LandPtr->SetLandIncludePartAlign(FileIO.GetData_BOL());
			break;		
			
		case FILE_IO_LAND_LEAD_SIZE_X://特徵框參數-引腳尺寸-X
			LandPtr->SetLandLeadSizeX(FileIO.GetData_DBL());
			break;
		case FILE_IO_LAND_LEAD_SIZE_Y://特徵框參數-引腳尺寸-Y
			LandPtr->SetLandLeadSizeY(FileIO.GetData_DBL());
			break;
		case FILE_IO_LAND_LEAD_HEIGHT://特徵框參數-引腳高度
			LandPtr->SetLandLeadHeight(FileIO.GetData_DBL());
			break;
			
		case FILE_IO_LAND_LEAD_TIP_SIZE_X://特徵框參數-引腳前端尺寸-X
			LandPtr->SetLandLeadTipSizeX(FileIO.GetData_DBL());
			break;
		case FILE_IO_LAND_LEAD_TIP_SIZE_Y://特徵框參數-引腳前端尺寸-Y
			LandPtr->SetLandLeadTipSizeY(FileIO.GetData_DBL());
			break;
		case FILE_IO_LAND_LEAD_TIP_HEIGHT://特徵框參數-引腳前端高度
			LandPtr->SetLandLeadTipHeight(FileIO.GetData_DBL());
			break;

		case FILE_IO_LAND_LEAD_SHOULDER_SIZE_X://特徵框參數-引腳根部尺寸-X
			LandPtr->SetLandLeadShoulderSizeX(FileIO.GetData_DBL());
			break;
		case FILE_IO_LAND_LEAD_SHOULDER_SIZE_Y://特徵框參數-引腳根部尺寸-Y
			LandPtr->SetLandLeadShoulderSizeY(FileIO.GetData_DBL());
			break;
		case FILE_IO_LAND_LEAD_SHOULDER_HEIGHT://特徵框參數-引腳根部高度
			LandPtr->SetLandLeadShoulderHeight(FileIO.GetData_DBL());
			break;
			
		case FILE_IO_PAD_BOX://特徵框參數-焊盤框
			BoxPtr = LandPtr->GetLandPadBoxPtr();
			if ( NULL != BoxPtr )
			{
				if ( BoxPtr->ReadBoxFile(FileIO) == false )
				{	return false; }
			}
			break;
		case FILE_IO_LEAD_BOX://特徵框參數-電極框
			BoxPtr = LandPtr->GetLandLeadBoxPtr();
			if ( NULL != BoxPtr )
			{
				if ( BoxPtr->ReadBoxFile(FileIO) == false )
				{	return false; }
			}
			break;
		case FILE_IO_LEAD_TIP_BOX://特徵框參數-引腳前端框
			BoxPtr = LandPtr->GetLandLeadTipBoxPtr();
			if ( NULL != BoxPtr )
			{
				if ( BoxPtr->ReadBoxFile(FileIO) == false )
				{	return false; }
			}
			break;
		case FILE_IO_LEAD_SHOULDER_BOX://特徵框參數-引腳根部框
			BoxPtr = LandPtr->GetLandLeadShoulderBoxPtr();
			if ( NULL != BoxPtr )
			{
				if ( BoxPtr->ReadBoxFile(FileIO) == false )
				{	return false; }
			}
			break;
		case FILE_IO_BODY_EDGE_BOX://特徵框參數-本體邊緣框
			BoxPtr = LandPtr->GetLandBodyEdgeBoxPtr();
			if ( NULL != BoxPtr )
			{
				if ( BoxPtr->ReadBoxFile(FileIO) == false )
				{	return false; }
			}
			break;
		default:
			break;
		}
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::WriteLandLeadSpcHeader_JSON_detail(FILE *pfile, CAOIProject *ProjectPtr) {

	CAOILand *LandPtr = this;
	CAOIBox *BoxPtr = NULL;
	CAOIModel *ModelPtr = LandPtr->GetLandModelPtr();
	if ( NULL == ModelPtr ) { return false; }	

	CString          strText;
	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";

	TPOINT2D BoxCad;
	TSIZE2D BoxSize;

	BoxPtr = LandPtr->GetLandLeadBoxPtr();
	BoxPtr->GetBoxSize(BoxSize);
	BoxPtr->GetBoxPos(BoxCad);

	::fwprintf(pfile, L"        {\n");	

	::wcscpy(strTag, L"Lead_ID");
	::fwprintf(pfile, L"          \"%s\":%d,\n", strTag, LandPtr->GetLandIndex() + 1);

	::wcscpy(strTag, L"CenterX");
	::fwprintf(pfile, L"          \"%s\":%.3f,\n", strTag, BoxCad.x);

	::wcscpy(strTag, L"CenterY");
	::fwprintf(pfile, L"          \"%s\":%.3f,\n", strTag, BoxCad.y);

	::wcscpy(strTag, L"Width");
	::fwprintf(pfile, L"          \"%s\":%.3f,\n", strTag, BoxSize.cx);

	::wcscpy(strTag, L"Height");
	::fwprintf(pfile, L"          \"%s\":%.3f,\n", strTag, BoxSize.cy);

	//Add Loc	
	TREGION4D ModelRgn;
	TPOINT2D ModelRgnCp;
	TPOINT2D CadCornerPos[4];
	TPOINT2D ImageCornerPos[4];	
	const double     ImageResX = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	const double     ImageResY = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	const TPOINT2D   ImageRes(ImageResX, ImageResY);
	ModelPtr->GetModelTotalRegion(ModelRgn);
	ModelRgnCp.x = ModelRgn.GetCpX();
	ModelRgnCp.y = ModelRgn.GetCpY();
	IMAGE_SIZE ModelImageW = JetAPI::Floor(ModelRgn.GetWidth()/ImageRes.x);
	IMAGE_SIZE ModelImageH = JetAPI::Floor(ModelRgn.GetHeight()/ImageRes.y);
	LandPtr->GetLandLeadBox().GetBoxCornerPos(CadCornerPos);	
	AOIDataCollect.MapCadCornerToCamera(ModelImageW, ModelImageH, ImageRes, CadCornerPos, ModelRgnCp, ImageCornerPos);						
	::wcscpy(strTag, L"Loc");//"Location" : [100,100,300,200,100,100,300,200],	// 在零件圖上的位置(x1 y1 x2 y2)(pixel)
	::fwprintf(pfile, L"          \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, 
		ImageCornerPos[0].x, ImageCornerPos[0].y, ImageCornerPos[1].x, ImageCornerPos[1].y, 
		ImageCornerPos[2].x, ImageCornerPos[2].y, ImageCornerPos[3].x, ImageCornerPos[3].y);

	::wcscpy(strTag, L"Group_ID");
	::fwprintf(pfile, L"          \"%s\":%d\n", strTag, LandPtr->GetLandGroupID() + 1);
	::fwprintf(pfile, L"        }");
	return TRUE;
}
bool CAOILand::WriteLandPadSpcHeader_JSON_detail(FILE *pfile, CAOIProject *ProjectPtr) {

	CAOILand *LandPtr = this;
	CAOIBox *BoxPtr = NULL;
	CAOIModel *ModelPtr = LandPtr->GetLandModelPtr();
	if ( NULL == ModelPtr ) { return false; }	

	CString          strText;
	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";

	TPOINT2D BoxCad;
	TSIZE2D BoxSize;

	BoxPtr = LandPtr->GetLandPadBoxPtr();
	BoxPtr->GetBoxSize(BoxSize);
	BoxPtr->GetBoxPos(BoxCad);

	::fwprintf(pfile, L"        {\n");
	::wcscpy(strTag, L"Pad_ID");
	::fwprintf(pfile, L"          \"%s\":%d,\n", strTag, LandPtr->GetLandIndex() + 1);

	::wcscpy(strTag, L"CenterX");
	::fwprintf(pfile, L"          \"%s\":%.3f,\n", strTag, BoxCad.x);

	::wcscpy(strTag, L"CenterY");
	::fwprintf(pfile, L"          \"%s\":%.3f,\n", strTag, BoxCad.y);

	::wcscpy(strTag, L"Width");
	::fwprintf(pfile, L"          \"%s\":%.3f,\n", strTag, BoxSize.cx);

	::wcscpy(strTag, L"Height");
	::fwprintf(pfile, L"          \"%s\":%.3f,\n", strTag, BoxSize.cy);

	//Add Loc	
	TREGION4D ModelRgn;
	TPOINT2D ModelRgnCp;
	TPOINT2D CadCornerPos[4];
	TPOINT2D ImageCornerPos[4];	
	const double     ImageResX = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	const double     ImageResY = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	const TPOINT2D   ImageRes(ImageResX, ImageResY);
	ModelPtr->GetModelTotalRegion(ModelRgn);
	ModelRgnCp.x = ModelRgn.GetCpX();
	ModelRgnCp.y = ModelRgn.GetCpY();
	IMAGE_SIZE ModelImageW = JetAPI::Floor(ModelRgn.GetWidth()/ImageRes.x);
	IMAGE_SIZE ModelImageH = JetAPI::Floor(ModelRgn.GetHeight()/ImageRes.y);
	LandPtr->GetLandPadBox().GetBoxCornerPos(CadCornerPos);	
	AOIDataCollect.MapCadCornerToCamera(ModelImageW, ModelImageH, ImageRes, CadCornerPos, ModelRgnCp, ImageCornerPos);						
	::wcscpy(strTag, L"Loc");//"Location" : [100,100,300,200,100,100,300,200],	// 在零件圖上的位置(x1 y1 x2 y2)(pixel)
	::fwprintf(pfile, L"          \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, 
		ImageCornerPos[0].x, ImageCornerPos[0].y, ImageCornerPos[1].x, ImageCornerPos[1].y, 
		ImageCornerPos[2].x, ImageCornerPos[2].y, ImageCornerPos[3].x, ImageCornerPos[3].y);

	::wcscpy(strTag, L"Group_ID");
	::fwprintf(pfile, L"          \"%s\":%d\n", strTag, LandPtr->GetLandGroupID() + 1);
	::fwprintf(pfile, L"        }");
	return TRUE;
}
//-------------------------------------------------------------------------------------//
TLandProperty CAOILand::GetLandProperty() const
{
	TLandProperty Propty;
	GetLandProperty(Propty);
	return Propty;
}
//-------------------------------------------------------------------------------------//
void CAOILand::GetLandProperty(TLandProperty &Propty) const
{
	Propty.nLandCount = 1;
	Propty.nLandGroupID = GetLandGroupID();	
	Propty.eLandType = GetLandType();
	Propty.eLandToward = GetLandToward();
	Propty.bPadAlign = GetLandIncludePadAlign();
	Propty.bPartAlign = GetLandIncludePartAlign();

	Propty.dLeadSizeX = GetLandLeadSizeX();
	Propty.dLeadSizeY = GetLandLeadSizeY();
	Propty.dLeadHeight = GetLandLeadHeight();
	Propty.dLeadTipSizeX = GetLandLeadTipSizeX();
	Propty.dLeadTipSizeY = GetLandLeadTipSizeY();
	Propty.dLeadTipHeight = GetLandLeadTipHeight();
	Propty.dLeadShoulderSizeX = GetLandLeadShoulderSizeX();
	Propty.dLeadShoulderSizeY = GetLandLeadShoulderSizeY();
	Propty.dLeadShoulderHeight = GetLandLeadShoulderHeight();
	return;
}
//-------------------------------------------------------------------------------------//
void CAOILand::SetLandProperty(const TLandProperty &Propty)
{
	//Propty.nLandCount = 1;
	//SetLandGroupID(Propty.nLandGroupID);	
	//SetLandType(Propty.eLandType);
	//SetLandToward(Propty.eLandToward);	
	SetLandIncludePadAlign(Propty.bPadAlign);
	SetLandIncludePartAlign(Propty.bPartAlign);	

	SetLandLeadSizeX(Propty.dLeadSizeX);
	SetLandLeadSizeY(Propty.dLeadSizeY);
	SetLandLeadHeight(Propty.dLeadHeight);
	SetLandLeadTipSizeX(Propty.dLeadTipSizeX);
	SetLandLeadTipSizeY(Propty.dLeadTipSizeY);
	SetLandLeadTipHeight(Propty.dLeadTipHeight);
	SetLandLeadShoulderSizeX(Propty.dLeadShoulderSizeX);
	SetLandLeadShoulderSizeY(Propty.dLeadShoulderSizeY);
	SetLandLeadShoulderHeight(Propty.dLeadShoulderHeight);
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::CheckLandLinkPos(CAOILand *LandPtr) const//確認特徵框位置連動
{
	if ( NULL == LandPtr ) { return false; }
	if ( GetLandToward() != LandPtr->GetLandToward() ) { return false; }
	if ( GetLandGroupID() != LandPtr->GetLandGroupID() ) { return false; }
	if ( GetLandAlignID() != LandPtr->GetLandAlignID() ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::CheckLandLinkSize(CAOILand *LandPtr) const//確認特徵框尺寸連動
{
	if ( NULL == LandPtr ) { return false; }	
	if ( GetLandGroupID() != LandPtr->GetLandGroupID() ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
inline void CAOILand::UpdateLandBoxPtr()//更新特徵框的主框
{
	switch ( m_LandType )
	{		
	case LAND_TYPE_ELECTRODE:
		m_LandBoxPtr = &(m_LandLeadBox);
		break;
	case LAND_TYPE_IC_LEAD:
		m_LandBoxPtr = &(m_LandLeadBox);
		break;	
	case LAND_TYPE_CON_LEAD:
		//m_LandBoxPtr = &(m_LandLeadTipBox);
		m_LandBoxPtr = &(m_LandLeadShoulderBox);
		break;
	case LAND_TYPE_DIP_LEAD:
		m_LandBoxPtr = &(m_LandPadBox);
		break;
	case LAND_TYPE_PAD:
		m_LandBoxPtr = &(m_LandPadBox);
		break;
	default:
		m_LandBoxPtr = &(m_LandPadBox);
		break;
	}
}
//-------------------------------------------------------------------------------------//
inline void CAOILand::EnableLandBasicBox(CAOIBox &Box)//啟用特徵基本框
{
	Box.SetBoxEnabled(true);
	Box.SetBoxVisibled(true);
	Box.SetBoxEditabled(true);
	return;
}
//-------------------------------------------------------------------------------------//
inline void CAOILand::DisableLandBasicBox(CAOIBox &Box)//關閉特徵基本框
{
	Box.SetBoxEnabled(false);
	Box.SetBoxSelected(false);
	Box.SetBoxVisibled(false);	
	Box.SetBoxEditabled(false);
	return;
}
//-------------------------------------------------------------------------------------//
inline size_t CAOILand::GetLandWndCount_Inline() const//取得特徵框的檢測框數量
{
	return CAOILand::m_LandWndList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOILand::AddLandWndPtr_Inline(CAOIWnd *WndPtr)//加入特徵框的檢測框
{
	CAOILand::m_LandWndList.push_back(WndPtr);
}
//-------------------------------------------------------------------------------------//
inline void CAOILand::RemoveLandWndList_Inline()//移除特徵框的檢測框
{
	CAOILand::m_LandWndList.clear();
}
//-------------------------------------------------------------------------------------//
inline CAOIWnd* CAOILand::GetLandWndPtr_Inline(size_t idx) const//取得特徵框的檢測框指標	
{	
	return (CAOILand::m_LandWndList[idx]);
}
//-------------------------------------------------------------------------------------//
inline size_t CAOILand::GetLandLogicCount_Inline() const
{
	return CAOILand::m_LandLogicList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOILand::AddLandLogicPtr_Inline(CAOILogic *LogicPtr)//加入特徵框的邏輯閘
{
	CAOILand::m_LandLogicList.push_back(LogicPtr);
}
//-------------------------------------------------------------------------------------//
inline void CAOILand::RemoveLandLogicList_Inline()
{
	CAOILand::m_LandLogicList.clear();
}
//-------------------------------------------------------------------------------------//
inline CAOILogic* CAOILand::GetLandLogicPtr_Inline(size_t index) const
{	
	return (CAOILand::m_LandLogicList[index]);
}
//-------------------------------------------------------------------------------------//
bool CAOILand::SwitchLandType(LAND_TYPE Type)
{
	DisableLandBasicBox(m_LandBodyEdgeBox);

	if ( GetLandTypeUsePad(Type) == true )
	{	EnableLandBasicBox(m_LandPadBox);	}
	else
	{	DisableLandBasicBox(m_LandPadBox);	}

	if ( GetLandTypeUseLead(Type) == true )
	{	EnableLandBasicBox(m_LandLeadBox);	}
	else
	{	DisableLandBasicBox(m_LandLeadBox);	}

	if ( GetLandTypeUseLeadTip(Type) == true )
	{	EnableLandBasicBox(m_LandLeadTipBox);	}
	else
	{	DisableLandBasicBox(m_LandLeadTipBox);	}

	if ( GetLandTypeUseLeadShoulder(Type) == true )
	{	EnableLandBasicBox(m_LandLeadShoulderBox);	}
	else
	{	DisableLandBasicBox(m_LandLeadShoulderBox);	}

	m_LandType = Type;
	UpdateLandBoxPtr();
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIBox* CAOILand::GetLandBasicBoxPtrSelected()
{
	CAOIBox *BoxPtr = NULL;

	BoxPtr = &m_LandPadBox;
	if ( BoxPtr->GetBoxEnabled() == true )
	{
		if ( BoxPtr->GetBoxSelected() == true )
		{	return BoxPtr; }
	}

	BoxPtr = &m_LandLeadBox;
	if ( BoxPtr->GetBoxEnabled() == true )
	{
		if ( BoxPtr->GetBoxSelected() == true )
		{	return BoxPtr; }
	}

	BoxPtr = &m_LandLeadTipBox;
	if ( BoxPtr->GetBoxEnabled() == true )
	{
		if ( BoxPtr->GetBoxSelected() == true )
		{	return BoxPtr; }
	}

	BoxPtr = &m_LandLeadShoulderBox;
	if ( BoxPtr->GetBoxEnabled() == true )
	{
		if ( BoxPtr->GetBoxSelected() == true )
		{	return BoxPtr; }
	}

	BoxPtr = &m_LandBodyEdgeBox;
	if ( BoxPtr->GetBoxEnabled() == true )
	{
		if ( BoxPtr->GetBoxSelected() == true )
		{	return BoxPtr; }
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
int CAOILand::GetLandBasicBoxID(const CAOIBox *BoxPtr) const
{
	if ( BoxPtr == &m_LandPadBox ) 
	{	return LAND_BOX_PAD; }
	if ( BoxPtr == &m_LandLeadBox ) 
	{	return LAND_BOX_LEAD; }
	if ( BoxPtr == &m_LandLeadTipBox ) 
	{	return LAND_BOX_LEAD_TIP; }
	if ( BoxPtr == &m_LandLeadShoulderBox ) 
	{	return LAND_BOX_LEAD_SHOULDER; }
	if ( BoxPtr == &m_LandBodyEdgeBox ) 
	{	return LAND_BOX_BODY_EDGE; }
	return 0;
}
//-------------------------------------------------------------------------------------//
CAOIBox* CAOILand::GetLandBasicBoxPtr(int BasicBoxID)
{
	CAOIBox *BoxPtr = NULL;
	switch ( BasicBoxID )
	{
	case LAND_BOX_PAD:				BoxPtr =&m_LandPadBox;	break;
	case LAND_BOX_LEAD:				BoxPtr =&m_LandLeadBox;	break;
	case LAND_BOX_LEAD_TIP:			BoxPtr =&m_LandLeadTipBox;	break;
	case LAND_BOX_LEAD_SHOULDER:	BoxPtr =&m_LandLeadShoulderBox;	break;
	case LAND_BOX_BODY_EDGE:		BoxPtr =&m_LandBodyEdgeBox;	break;
	}
	return BoxPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::LayoutLeadBox(CAOIBox *BoxPtr, bool bResult)
{
	if ( BoxPtr == &(CAOILand::m_LandLeadBox) )
	{	return CAOILand::LayoutLeadBox(bResult);	}

	if ( BoxPtr == &(CAOILand::m_LandLeadTipBox) )
	{	return CAOILand::LayoutLeadTipBox(bResult);	}

	if ( BoxPtr == &(CAOILand::m_LandLeadShoulderBox) )
	{	return CAOILand::LayoutLeadShoulderBox(bResult);	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOILand::LayoutLeadBox(bool bResult)
{
	TREGION4D  RgnPad;
	TREGION4D  RgnLead;
	TREGION4D  RgnLeadTip;
	TREGION4D  RgnLeadShoulder;
	CAOIBox   *BoxPtr = CAOILand::GetLandLeadBoxPtr();
	BOX_TOWARD LandToward = CAOILand::GetLandToward();

	if ( LAND_TYPE_PAD == m_LandType )
	{
		if ( true == bResult )
		{	m_LandPadBox.GetBoxRegionRes(RgnPad);	}
		else
		{	m_LandPadBox.GetBoxRegion(RgnPad);	}		
		RgnLeadShoulder = RgnLeadTip = RgnLead = RgnPad;		
	}
	else if ( LAND_TYPE_ELECTRODE == m_LandType )
	{
		if ( true == bResult )
		{	m_LandLeadBox.GetBoxRegionRes(RgnLead);	}
		else
		{	m_LandLeadBox.GetBoxRegion(RgnLead);	}
		RgnLeadShoulder = RgnLeadTip = RgnLead;
	}
	else if ( LAND_TYPE_IC_LEAD == m_LandType )
	{
		if ( true == bResult )
		{	
			m_LandLeadBox.GetBoxRegionRes(RgnLead);	
			m_LandLeadTipBox.GetBoxRegionRes(RgnLeadTip);	
			m_LandLeadShoulderBox.GetBoxRegionRes(RgnLeadShoulder);	
		}
		else
		{	m_LandLeadBox.GetBoxRegion(RgnLead);	
			m_LandLeadTipBox.GetBoxRegion(RgnLeadTip);	
			m_LandLeadShoulderBox.GetBoxRegion(RgnLeadShoulder);
		}
		switch ( LandToward )
		{
		case BOX_TOWARD_UP:
			RgnLeadTip.maxY =  RgnLead.maxY;
			RgnLeadShoulder.minY =  RgnLead.minY;

			if ( RgnLeadTip.minY < RgnLead.minY ) 
			{	RgnLeadTip.minY =  RgnLead.minY; }
			if ( RgnLeadShoulder.maxY > RgnLead.maxY ) 
			{	RgnLeadShoulder.maxY =  RgnLead.maxY; }

			RgnLeadTip.minX =  RgnLead.minX;
			RgnLeadTip.maxX =  RgnLead.maxX;
			RgnLeadShoulder.minX =  RgnLead.minX;
			RgnLeadShoulder.maxX =  RgnLead.maxX;
			break;
		case BOX_TOWARD_LEFT:
			RgnLeadTip.minX =  RgnLead.minX;
			RgnLeadShoulder.maxX =  RgnLead.maxX;

			if ( RgnLeadTip.maxX > RgnLead.maxX ) 
			{	RgnLeadTip.maxX =  RgnLead.maxX; }
			if ( RgnLeadShoulder.minX < RgnLead.minX ) 
			{	RgnLeadShoulder.minX =  RgnLead.minX; }

			RgnLeadTip.minY =  RgnLead.minY;
			RgnLeadTip.maxY =  RgnLead.maxY;
			RgnLeadShoulder.minY =  RgnLead.minY;
			RgnLeadShoulder.maxY =  RgnLead.maxY;
			break;
		case BOX_TOWARD_DOWN:
			RgnLeadTip.minY =  RgnLead.minY;
			RgnLeadShoulder.maxY =  RgnLead.maxY;

			if ( RgnLeadTip.maxY > RgnLead.maxY ) 
			{	RgnLeadTip.maxY =  RgnLead.maxY; }
			if ( RgnLeadShoulder.minY < RgnLead.minY ) 
			{	RgnLeadShoulder.minY =  RgnLead.minY; }

			RgnLeadTip.minX =  RgnLead.minX;
			RgnLeadTip.maxX =  RgnLead.maxX;
			RgnLeadShoulder.minX =  RgnLead.minX;
			RgnLeadShoulder.maxX =  RgnLead.maxX;
			break;
		case BOX_TOWARD_RIGHT:
			RgnLeadTip.maxX =  RgnLead.maxX;
			RgnLeadShoulder.minX =  RgnLead.minX;

			if ( RgnLeadTip.minX < RgnLead.minX ) 
			{	RgnLeadTip.minX =  RgnLead.minX; }
			if ( RgnLeadShoulder.maxX > RgnLead.maxX ) 
			{	RgnLeadShoulder.maxX =  RgnLead.maxX; }

			RgnLeadTip.minY =  RgnLead.minY;
			RgnLeadTip.maxY =  RgnLead.maxY;
			RgnLeadShoulder.minY =  RgnLead.minY;
			RgnLeadShoulder.maxY =  RgnLead.maxY;
			break;
		}  
	}
	else if ( LAND_TYPE_CON_LEAD == m_LandType )
	{
		if ( true == bResult )
		{	
			m_LandLeadBox.GetBoxRegionRes(RgnLead);	
			m_LandLeadTipBox.GetBoxRegionRes(RgnLeadTip);	
			m_LandLeadShoulderBox.GetBoxRegionRes(RgnLeadShoulder);	
		}
		else
		{	m_LandLeadBox.GetBoxRegion(RgnLead);	
			m_LandLeadTipBox.GetBoxRegion(RgnLeadTip);	
			m_LandLeadShoulderBox.GetBoxRegion(RgnLeadShoulder);
		}
		switch ( LandToward )
		{
		case BOX_TOWARD_UP:
			RgnLeadTip.maxY =  RgnLead.maxY;
			RgnLeadShoulder.minY =  RgnLead.minY;

			if ( RgnLeadTip.minY < RgnLead.minY ) 
			{	RgnLeadTip.minY =  RgnLead.minY; }
			if ( RgnLeadShoulder.maxY > RgnLead.maxY ) 
			{	RgnLeadShoulder.maxY =  RgnLead.maxY; }

			RgnLeadTip.minX =  RgnLead.minX;
			RgnLeadTip.maxX =  RgnLead.maxX;
			//RgnLeadShoulder.minX =  RgnLead.minX;
			//RgnLeadShoulder.maxX =  RgnLead.maxX;
			break;
		case BOX_TOWARD_LEFT:
			RgnLeadTip.minX =  RgnLead.minX;
			RgnLeadShoulder.maxX =  RgnLead.maxX;

			if ( RgnLeadTip.maxX > RgnLead.maxX ) 
			{	RgnLeadTip.maxX =  RgnLead.maxX; }
			if ( RgnLeadShoulder.minX < RgnLead.minX ) 
			{	RgnLeadShoulder.minX =  RgnLead.minX; }

			RgnLeadTip.minY =  RgnLead.minY;
			RgnLeadTip.maxY =  RgnLead.maxY;
			//RgnLeadShoulder.minY =  RgnLead.minY;
			//RgnLeadShoulder.maxY =  RgnLead.maxY;
			break;
		case BOX_TOWARD_DOWN:
			RgnLeadTip.minY =  RgnLead.minY;
			RgnLeadShoulder.maxY =  RgnLead.maxY;

			if ( RgnLeadTip.maxY > RgnLead.maxY ) 
			{	RgnLeadTip.maxY =  RgnLead.maxY; }
			if ( RgnLeadShoulder.minY < RgnLead.minY ) 
			{	RgnLeadShoulder.minY =  RgnLead.minY; }

			RgnLeadTip.minX =  RgnLead.minX;
			RgnLeadTip.maxX =  RgnLead.maxX;
			//RgnLeadShoulder.minX =  RgnLead.minX;
			//RgnLeadShoulder.maxX =  RgnLead.maxX;
			break;
		case BOX_TOWARD_RIGHT:
			RgnLeadTip.maxX =  RgnLead.maxX;
			RgnLeadShoulder.minX =  RgnLead.minX;

			if ( RgnLeadTip.minX < RgnLead.minX ) 
			{	RgnLeadTip.minX =  RgnLead.minX; }
			if ( RgnLeadShoulder.maxX > RgnLead.maxX ) 
			{	RgnLeadShoulder.maxX =  RgnLead.maxX; }

			RgnLeadTip.minY =  RgnLead.minY;
			RgnLeadTip.maxY =  RgnLead.maxY;
			//RgnLeadShoulder.minY =  RgnLead.minY;
			//RgnLeadShoulder.maxY =  RgnLead.maxY;
			break;
		}  
	}	
	else if ( LAND_TYPE_DIP_LEAD == m_LandType )
	{
		if ( true == bResult )
		{	m_LandLeadBox.GetBoxRegionRes(RgnLead);	}
		else
		{	m_LandLeadBox.GetBoxRegion(RgnLead);	}
		RgnLeadShoulder = RgnLeadTip = RgnLead;
	}

	JetAPI::AdjustRegion(RgnLead, RgnLead);
	JetAPI::AdjustRegion(RgnLeadTip, RgnLeadTip);
	JetAPI::AdjustRegion(RgnLeadShoulder, RgnLeadShoulder);
	if ( true == bResult )
	{	
		m_LandLeadBox.SetBoxRegionRes(RgnLead);	
		m_LandLeadTipBox.SetBoxRegionRes(RgnLeadTip);	
		m_LandLeadShoulderBox.SetBoxRegionRes(RgnLeadShoulder);	
	}
	else
	{
		m_LandLeadBox.SetBoxRegion(RgnLead);	
		m_LandLeadTipBox.SetBoxRegion(RgnLeadTip);	
		m_LandLeadShoulderBox.SetBoxRegion(RgnLeadShoulder);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOILand::LayoutLeadTipBox(bool bResult)
{
	TREGION4D  RgnLead;
	TREGION4D  RgnLeadTip;
	TREGION4D  RgnLeadShoulder;
	CAOIBox   *BoxPtr = GetLandLeadBoxPtr();
	LAND_TYPE  LandType = GetLandType();
	BOX_TOWARD LandToward = GetLandToward();

	if ( true == bResult )
	{	
		m_LandLeadBox.GetBoxRegionRes(RgnLead);	
		m_LandLeadTipBox.GetBoxRegionRes(RgnLeadTip);	
		m_LandLeadShoulderBox.GetBoxRegionRes(RgnLeadShoulder);	
	}
	else
	{	m_LandLeadBox.GetBoxRegion(RgnLead);	
		m_LandLeadTipBox.GetBoxRegion(RgnLeadTip);	
		m_LandLeadShoulderBox.GetBoxRegion(RgnLeadShoulder);
	}

	switch ( LandToward )
	{
	case BOX_TOWARD_UP:
		RgnLead.maxY =  RgnLeadTip.maxY;		

		if ( RgnLeadShoulder.maxY > RgnLeadTip.minY ) 
		{	RgnLeadShoulder.maxY =  RgnLeadTip.minY; }
		if ( RgnLeadShoulder.minY > RgnLeadTip.minY ) 
		{	RgnLeadShoulder.minY =  RgnLeadTip.minY; }
		RgnLead.minY =  RgnLeadShoulder.minY;

		RgnLead.minX =  RgnLeadTip.minX;
		RgnLead.maxX =  RgnLeadTip.maxX;
		if ( LAND_TYPE_CON_LEAD != LandType )
		{
			RgnLeadShoulder.minX =  RgnLeadTip.minX;
			RgnLeadShoulder.maxX =  RgnLeadTip.maxX;
		}
		break;
	case BOX_TOWARD_LEFT:
		RgnLead.minX =  RgnLeadTip.minX;		

		if ( RgnLeadShoulder.minX < RgnLeadTip.maxX ) 
		{	RgnLeadShoulder.minX =  RgnLeadTip.maxX; }
		if ( RgnLeadShoulder.maxX < RgnLeadTip.maxX ) 
		{	RgnLeadShoulder.maxX =  RgnLeadTip.maxX; }		
		RgnLead.maxX =  RgnLeadShoulder.maxX;

		RgnLead.minY =  RgnLeadTip.minY;
		RgnLead.maxY =  RgnLeadTip.maxY;
		if ( LAND_TYPE_CON_LEAD != LandType )
		{
			RgnLeadShoulder.minY =  RgnLeadTip.minY;
			RgnLeadShoulder.maxY =  RgnLeadTip.maxY;
		}
		break;
	case BOX_TOWARD_DOWN:
		RgnLead.minY =  RgnLeadTip.minY;		

		if ( RgnLeadShoulder.minY < RgnLeadTip.maxY ) 
		{	RgnLeadShoulder.minY =  RgnLeadTip.maxY; }
		if ( RgnLeadShoulder.maxY < RgnLeadTip.maxY ) 
		{	RgnLeadShoulder.maxY =  RgnLeadTip.maxY; }
		RgnLead.maxY =  RgnLeadShoulder.maxY;

		RgnLead.minX =  RgnLeadTip.minX;
		RgnLead.maxX =  RgnLeadTip.maxX;
		if ( LAND_TYPE_CON_LEAD != LandType )
		{
			RgnLeadShoulder.minX =  RgnLeadTip.minX;
			RgnLeadShoulder.maxX =  RgnLeadTip.maxX;
		}
		break;
	case BOX_TOWARD_RIGHT:
		RgnLead.maxX =  RgnLeadTip.maxX;
		if ( RgnLeadShoulder.maxX > RgnLeadTip.minX ) 
		{	RgnLeadShoulder.maxX =  RgnLeadTip.minX; }
		if ( RgnLeadShoulder.minX > RgnLeadTip.minX ) 
		{	RgnLeadShoulder.minX =  RgnLeadTip.minX; }		
		RgnLead.minX =  RgnLeadShoulder.minX;

		RgnLead.minY =  RgnLeadTip.minY;
		RgnLead.maxY =  RgnLeadTip.maxY;
		if ( LAND_TYPE_CON_LEAD != LandType )
		{
			RgnLeadShoulder.minY =  RgnLeadTip.minY;
			RgnLeadShoulder.maxY =  RgnLeadTip.maxY;
		}
		break;
	}  

	JetAPI::AdjustRegion(RgnLead, RgnLead);
	JetAPI::AdjustRegion(RgnLeadTip, RgnLeadTip);
	JetAPI::AdjustRegion(RgnLeadShoulder, RgnLeadShoulder);
	if ( true == bResult )
	{	
		m_LandLeadBox.SetBoxRegionRes(RgnLead);	
		m_LandLeadTipBox.SetBoxRegionRes(RgnLeadTip);	
		m_LandLeadShoulderBox.SetBoxRegionRes(RgnLeadShoulder);	
	}
	else
	{
		m_LandLeadBox.SetBoxRegion(RgnLead);	
		m_LandLeadTipBox.SetBoxRegion(RgnLeadTip);	
		m_LandLeadShoulderBox.SetBoxRegion(RgnLeadShoulder);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool CAOILand::LayoutLeadShoulderBox(bool bResult)
{
	TREGION4D  RgnLead;
	TREGION4D  RgnLeadTip;
	TREGION4D  RgnLeadShoulder;
	CAOIBox   *BoxPtr = GetLandLeadBoxPtr();
	LAND_TYPE  LandType = GetLandType();
	BOX_TOWARD LandToward = GetLandToward();
	
	if ( LAND_TYPE_CON_LEAD == LandType ) { return true; }

	if ( true == bResult )
	{	
		m_LandLeadBox.GetBoxRegionRes(RgnLead);	
		m_LandLeadTipBox.GetBoxRegionRes(RgnLeadTip);	
		m_LandLeadShoulderBox.GetBoxRegionRes(RgnLeadShoulder);	
	}
	else
	{	m_LandLeadBox.GetBoxRegion(RgnLead);	
		m_LandLeadTipBox.GetBoxRegion(RgnLeadTip);	
		m_LandLeadShoulderBox.GetBoxRegion(RgnLeadShoulder);
	}

	switch ( LandToward )
	{
	case BOX_TOWARD_UP:
		RgnLead.minY =  RgnLeadShoulder.minY;		

		if ( RgnLeadTip.minY < RgnLeadShoulder.maxY ) 
		{	RgnLeadTip.minY =  RgnLeadShoulder.maxY; }
		if ( RgnLeadTip.maxY < RgnLeadShoulder.maxY ) 
		{	RgnLeadTip.maxY =  RgnLeadShoulder.maxY; }		
		RgnLead.maxY =  RgnLeadTip.maxY;

		RgnLead.minX =  RgnLeadShoulder.minX;
		RgnLead.maxX =  RgnLeadShoulder.maxX;
		RgnLeadTip.minX =  RgnLeadShoulder.minX;
		RgnLeadTip.maxX =  RgnLeadShoulder.maxX;
		break;
	case BOX_TOWARD_LEFT:
		RgnLead.maxX =  RgnLeadShoulder.maxX;

		if ( RgnLeadTip.maxX > RgnLeadShoulder.minX ) 
		{	RgnLeadTip.maxX =  RgnLeadShoulder.minX; }
		if ( RgnLeadTip.minX > RgnLeadShoulder.minX ) 
		{	RgnLeadTip.minX =  RgnLeadShoulder.minX; }
		RgnLead.minX =  RgnLeadTip.minX;

		RgnLead.minY =  RgnLeadShoulder.minY;
		RgnLead.maxY =  RgnLeadShoulder.maxY;
		RgnLeadTip.minY =  RgnLeadShoulder.minY;
		RgnLeadTip.maxY =  RgnLeadShoulder.maxY;
		break;
	case BOX_TOWARD_DOWN:
		RgnLead.maxY =  RgnLeadShoulder.maxY;

		if ( RgnLeadTip.maxY > RgnLeadShoulder.minY ) 
		{	RgnLeadTip.maxY =  RgnLeadShoulder.minY; }
		if ( RgnLeadTip.minY > RgnLeadShoulder.minY ) 
		{	RgnLeadTip.minY =  RgnLeadShoulder.minY; }
		RgnLead.minY =  RgnLeadTip.minY;

		RgnLead.minX =  RgnLeadShoulder.minX;
		RgnLead.maxX =  RgnLeadShoulder.maxX;
		RgnLeadTip.minX =  RgnLeadShoulder.minX;
		RgnLeadTip.maxX =  RgnLeadShoulder.maxX;
		break;
	case BOX_TOWARD_RIGHT:
		RgnLead.minX =  RgnLeadShoulder.minX;		

		if ( RgnLeadTip.minX < RgnLeadShoulder.maxX ) 
		{	RgnLeadTip.minX =  RgnLeadShoulder.maxX; }
		if ( RgnLeadTip.maxX < RgnLeadShoulder.maxX ) 
		{	RgnLeadTip.maxX =  RgnLeadShoulder.maxX; }
		RgnLead.maxX =  RgnLeadTip.maxX;

		RgnLead.minY =  RgnLeadShoulder.minY;
		RgnLead.maxY =  RgnLeadShoulder.maxY;
		RgnLeadTip.minY =  RgnLeadShoulder.minY;
		RgnLeadTip.maxY =  RgnLeadShoulder.maxY;
		break;
	}  

	JetAPI::AdjustRegion(RgnLead, RgnLead);
	JetAPI::AdjustRegion(RgnLeadTip, RgnLeadTip);
	JetAPI::AdjustRegion(RgnLeadShoulder, RgnLeadShoulder);
	if ( true == bResult )
	{	
		m_LandLeadBox.SetBoxRegionRes(RgnLead);	
		m_LandLeadTipBox.SetBoxRegionRes(RgnLeadTip);	
		m_LandLeadShoulderBox.SetBoxRegionRes(RgnLeadShoulder);	
	}
	else
	{
		m_LandLeadBox.SetBoxRegion(RgnLead);	
		m_LandLeadTipBox.SetBoxRegion(RgnLeadTip);	
		m_LandLeadShoulderBox.SetBoxRegion(RgnLeadShoulder);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOILand::SetLandAllBoxActived(bool value)//設定是否焦點狀態
{
	m_LandPadBox.SetBoxActived(value);
	m_LandLeadBox.SetBoxActived(value);
	m_LandLeadTipBox.SetBoxActived(value);
	m_LandLeadShoulderBox.SetBoxActived(value);
	m_LandBodyEdgeBox.SetBoxActived(value);
}
//-------------------------------------------------------------------------------------//
void CAOILand::SetLandAllBoxSelected(bool value)
{
	m_LandPadBox.SetBoxSelected(value);
	m_LandLeadBox.SetBoxSelected(value);
	m_LandLeadTipBox.SetBoxSelected(value);
	m_LandLeadShoulderBox.SetBoxSelected(value);
	m_LandBodyEdgeBox.SetBoxSelected(value);
}
//-------------------------------------------------------------------------------------//
void CAOILand::SetLandAllBoxVisibled(bool value)
{
	m_LandPadBox.SetBoxVisibled(value);
	m_LandLeadBox.SetBoxVisibled(value);
	m_LandLeadTipBox.SetBoxVisibled(value);
	m_LandLeadShoulderBox.SetBoxVisibled(value);
	m_LandBodyEdgeBox.SetBoxVisibled(value);	
}
//-------------------------------------------------------------------------------------//
bool CAOILand::UnSelectLand(bool ToWnd)
{
	size_t i=0;		
	const bool bSelected = false;
	m_LandPadBox.SetBoxSelected(bSelected);
	m_LandLeadBox.SetBoxSelected(bSelected);
	m_LandLeadTipBox.SetBoxSelected(bSelected);
	m_LandLeadShoulderBox.SetBoxSelected(bSelected);
	m_LandBodyEdgeBox.SetBoxSelected(bSelected);	
	if ( false == ToWnd ) { return true; }

	CAOIWnd     *WndPtr=NULL;
	const size_t WndCount = CAOILand::GetLandWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = CAOILand::GetLandWndPtr_Inline(i);
		if ( WndPtr == NULL ) { continue; }
		WndPtr->SetWndSelected(bSelected);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::InvisibleLand(bool ToWnd)
{
	size_t i=0;	
	const bool Visibled = false;
	m_LandPadBox.SetBoxVisibled(Visibled);
	m_LandLeadBox.SetBoxVisibled(Visibled);
	m_LandLeadTipBox.SetBoxVisibled(Visibled);
	m_LandLeadShoulderBox.SetBoxVisibled(Visibled);
	m_LandBodyEdgeBox.SetBoxVisibled(Visibled);	
	if ( false == ToWnd ) { return true; }

	CAOIWnd     *WndPtr=NULL;
	const size_t WndCount = CAOILand::GetLandWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = CAOILand::GetLandWndPtr_Inline(i);
		if ( WndPtr == NULL ) { continue; }		
		WndPtr->SetWndVisibled(Visibled);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::VisibleLand(bool VisibleWnd)//開?特徵框顯示狀態
{
	size_t i=0;	
	const bool Visibled = true;
	m_LandPadBox.SetBoxVisibled(Visibled);
	m_LandLeadBox.SetBoxVisibled(Visibled);
	m_LandLeadTipBox.SetBoxVisibled(Visibled);
	m_LandLeadShoulderBox.SetBoxVisibled(Visibled);
	m_LandBodyEdgeBox.SetBoxVisibled(Visibled);	
	if ( false == VisibleWnd ) { return true; }

	CAOIWnd     *WndPtr=NULL;
	const size_t WndCount = GetLandWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetLandWndPtr_Inline(i);
		if ( WndPtr == NULL ) { continue; }		
		WndPtr->SetWndVisibled(Visibled);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::GetLandActived()//取得是否為焦點狀態
{
	CAOIBox  *BoxPtr = NULL;

	BoxPtr = GetLandBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true ) 
	{
		if ( BoxPtr->GetBoxActived() == true ) 
		{	return true; }
	}

	BoxPtr = GetLandPadBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true ) 
	{
		if ( BoxPtr->GetBoxActived() == true ) 
		{	return true; }
	}

	BoxPtr = GetLandLeadBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true ) 
	{
		if ( BoxPtr->GetBoxActived() == true ) 
		{	return true; }
	}	

	BoxPtr = GetLandLeadTipBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true ) 
	{
		if ( BoxPtr->GetBoxActived() == true ) 
		{	return true; }
	}	

	BoxPtr = GetLandLeadShoulderBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true ) 
	{
		if ( BoxPtr->GetBoxActived() == true ) 
		{	return true; }
	}	

	BoxPtr = GetLandBodyEdgeBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true ) 
	{
		if ( BoxPtr->GetBoxActived() == true ) 
		{	return true; }
	}	
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::GetLandSelected()
{
	CAOIBox  *BoxPtr = NULL;

	BoxPtr = GetLandBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true ) 
	{
		if ( BoxPtr->GetBoxSelected() == true ) 
		{	return true; }
	}

	BoxPtr = GetLandPadBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true ) 
	{
		if ( BoxPtr->GetBoxSelected() == true ) 
		{	return true; }
	}

	BoxPtr = GetLandLeadBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true ) 
	{
		if ( BoxPtr->GetBoxSelected() == true ) 
		{	return true; }
	}	

	BoxPtr = GetLandLeadTipBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true ) 
	{
		if ( BoxPtr->GetBoxSelected() == true ) 
		{	return true; }
	}	

	BoxPtr = GetLandLeadShoulderBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true ) 
	{
		if ( BoxPtr->GetBoxSelected() == true ) 
		{	return true; }
	}	

	BoxPtr = GetLandBodyEdgeBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true ) 
	{
		if ( BoxPtr->GetBoxSelected() == true ) 
		{	return true; }
	}	
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::SetLandToward(BOX_TOWARD value)//設定特徵框朝向
{
	m_LandBoxPtr->SetBoxToward(value);
	m_LandPadBox.SetBoxToward(value);
	m_LandLeadBox.SetBoxToward(value);
	m_LandLeadTipBox.SetBoxToward(value);
	m_LandLeadShoulderBox.SetBoxToward(value);
	m_LandBodyEdgeBox.SetBoxToward(value);
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOILand::GetLandWndCount() const//取得特徵框的檢測框數量
{
	return CAOILand::GetLandWndCount_Inline();	
}
//-------------------------------------------------------------------------------------//
bool CAOILand::AddLandWndPtr(CAOIWnd *WndPtr)//增加特徵框的檢測框	
{
	if ( NULL == WndPtr ) { return false; }		
	WndPtr->SetWndToward(CAOILand::GetLandPadBoxPtr()->GetBoxToward());	
	WndPtr->SetWndLandPtr(this);
	WndPtr->SetWndLandIndex(CAOILand::GetLandIndex());	
//	WndPtr->SetWndIndexLand(CAOILand::GetLandWndCount());
	CAOILand::AddLandWndPtr_Inline(WndPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOILand::GetLandWndPtrLastOne() const//取得特徵框的檢測框指標	
{
	const size_t size = GetLandWndCount_Inline();
	if ( 0 == size ) { return NULL; }
	return GetLandWndPtr_Inline(size-1);
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOILand::GetLandWndPtrFirstOne() const//取得特徵框的第1個檢測框指標	
{
	const size_t size = GetLandWndCount_Inline();
	if ( 0 == size ) { return NULL; }
	return GetLandWndPtr_Inline(0);
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOILand::GetLandWndPtr(size_t Index, bool Check) const//取得特徵框的檢測框指標
{
	if ( Check == true )
	{
		const size_t size = GetLandWndCount_Inline();
		if ( Index >= size ) 
		{	return NULL; }
	}
	return GetLandWndPtr_Inline(Index);	
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOILand::GetLandWndPtrByGroupID(int WndGroupID, int WndBandID) const
{
	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = CAOILand::GetLandWndCount_Inline();
	if ( WndBandID < 0 ) 
	{
		for ( i=0; i<WndCount; i++ )
		{
			WndPtr = CAOILand::GetLandWndPtr_Inline(i);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }			
			return WndPtr;	
		}
	}
	else
	{
		for ( i=0; i<WndCount; i++ )
		{
			WndPtr = CAOILand::GetLandWndPtr_Inline(i);
			if ( NULL == WndPtr ) { continue; }
			if ( WndPtr->GetWndGroupID() != WndGroupID ) { continue; }
			if ( WndPtr->GetWndBandID() != WndBandID ) { continue; }			
			return WndPtr;	
		}
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
void CAOILand::SetLandWndSelected(bool val)
{
	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetLandWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetLandWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		WndPtr->SetWndSelected(val);		
	}
	return ;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOILand::GetLandWndSelected() const
{
	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = GetLandWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetLandWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndSelected() == false ) { continue; }
		return WndPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
void CAOILand::RemoveLandWndList()//移除特徵框的檢測框
{
	CAOILand::RemoveLandWndList_Inline();
}
//-------------------------------------------------------------------------------------//
bool CAOILand::RemoveLandWndSelected()
{
	size_t       i=0;
	unsigned int WndIndexLand = 0;
	size_t       size = 0;
	CAOIWnd     *WndPtr = NULL;		
	std::vector<CAOIWnd*>  LandWndList = CAOILand::m_LandWndList;		
	
	WndIndexLand = 0;	
	size = LandWndList.size();
	CAOILand::RemoveLandWndList_Inline();
	for ( i=0; i<size; i++ )
	{
		WndPtr = LandWndList[i];
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndSelected() == true ) {	continue; }
		//WndPtr->SetWndIndexLand(WndIndexLand);
		CAOILand::AddLandWndPtr_Inline(WndPtr);
		WndIndexLand ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::LayoutLandWndList()
{
	size_t       i = 0;
	unsigned int WndIndex = 0;
	int          WndGroupID = 0;
	CAOIWnd     *WndPtr = NULL;	
	std::vector<CAOIWnd*>  LandWndList = m_LandWndList;
	const size_t LandWndCount = LandWndList.size();	

	CSortObj              SortNode, *SortNodePtr=NULL;
	std::vector<CSortObj> SortList;

	SortList.clear();
	SortNode.SetSortMode(SORT_BY_INT);	
	for ( i=0; i<LandWndCount; i++ )
	{
		WndPtr = LandWndList[i];
		if ( WndPtr == NULL ) { continue; }
		WndGroupID = WndPtr->GetWndGroupID();

		SortNode.SetID(i);
		SortNode.SetValueInt(WndGroupID);
		SortNode.SetPtr(WndPtr);
		SortList.push_back(SortNode);
	}
	std::sort(SortList.begin(), SortList.end());
	const size_t SortNodeCount = SortList.size();

	WndIndex = 0;
	RemoveLandWndList_Inline();
	for ( i=0; i<SortNodeCount; i++ )
	{
		SortNodePtr = &(SortList[i]);
		WndGroupID = SortNodePtr->GetValueInt();
		WndPtr     = (CAOIWnd*)(SortNodePtr->GetPtr());		
		AddLandWndPtr_Inline(WndPtr);
		WndIndex ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOILand::GetLandLogicCount() const
{
	return CAOILand::GetLandLogicCount_Inline();
}
//-------------------------------------------------------------------------------------//
CAOILogic* CAOILand::GetLandLogicPtr(size_t index, bool Check) const
{
	if ( Check == true )
	{
		const size_t size = CAOILand::GetLandLogicCount_Inline();
		if ( index >= size ) 
		{	return NULL; }
	}
	return CAOILand::GetLandLogicPtr_Inline(index);	
}
//-------------------------------------------------------------------------------------//
CAOILogic* CAOILand::GetLandLogicPtrByGroupID(int LogicGroupID) const
{
	size_t i=0;
	CAOILogic *LogicPtr = NULL;
	const size_t LogicCount = CAOILand::GetLandLogicCount_Inline();
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = CAOILand::GetLandLogicPtr_Inline(i);
		if ( NULL == LogicPtr ) { continue; }
		if ( LogicPtr->GetLogicGroupID() != LogicGroupID ) { continue; }
		return LogicPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
void CAOILand::SetLandLogicSelected(bool val)
{
	size_t i=0;
	CAOILogic *LogicPtr = NULL;
	const size_t LogicCount = GetLandLogicCount_Inline();
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = GetLandLogicPtr_Inline(i);
		if ( NULL == LogicPtr ) { continue; }
		LogicPtr->SetLogicSelected(val);		
	}
	return ;
}
//-------------------------------------------------------------------------------------//
CAOILogic* CAOILand::GetLandLogicSelected() const
{
	size_t i=0;
	CAOILogic *LogicPtr = NULL;
	const size_t LogicCount = CAOILand::GetLandLogicCount_Inline();
	for ( i=0; i<LogicCount; i++ )
	{
		LogicPtr = CAOILand::GetLandLogicPtr_Inline(i);
		if ( NULL == LogicPtr ) { continue; }
		if ( LogicPtr->GetLogicSelected() == false ) { continue; }
		return LogicPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::AddLandLogicPtr(CAOILogic *LogicPtr)
{	
	if ( NULL == LogicPtr ) { return false; }
	LogicPtr->SetLogicLandIndex(CAOILand::GetLandIndex());	
	LogicPtr->SetLogicLandPtr(this);
	//LogicPtr->SetLogicIndexLand(CAOILand::GetLandLogicCount());
	CAOILand::AddLandLogicPtr_Inline(LogicPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOILand::RemoveLandLogicList()
{
	CAOILand::RemoveLandLogicList_Inline();
}
//-------------------------------------------------------------------------------------//
bool CAOILand::RemoveLandLogicSelected()
{
	size_t i=0;	
	size_t size = 0;
	CAOILogic *LogicPtr = NULL;		
	std::vector<CAOILogic*>  LandLogicList = CAOILand::m_LandLogicList;
	
	CAOILand::RemoveLandLogicList_Inline();
	size = LandLogicList.size();
	for ( i=0; i<size; i++ )
	{
		LogicPtr = LandLogicList[i];
		if ( NULL == LogicPtr ) { continue; }
		if ( LogicPtr->GetLogicSelected() == true ) {	continue; }		
		CAOILand::AddLandLogicPtr_Inline(LogicPtr);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOILand::CalcLandTotalRegionStage(TREGION4D &LandRegion)//計算特徵框的整個基台範圍-
{
	size_t i = 0;	
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	TREGION4D    BoxRegion;
	double MinX=0, MinY=0, MaxX=0, MaxY=0;
	const size_t WndCount = CAOILand::GetLandWndCount_Inline();
	
	//Base
	BoxPtr = CAOILand::GetLandBoxPtr();//GetLandPadBoxPtr();
	BoxPtr->GetBoxRegionStage(LandRegion);	

	//Pad Region
	BoxPtr = CAOILand::GetLandPadBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true )
	{
		BoxPtr->GetBoxRegionStage(BoxRegion);
		JetAPI::UnionRegion(BoxRegion, LandRegion, LandRegion);
	}			

	//Lead Region
	BoxPtr = CAOILand::GetLandLeadBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true )
	{
		BoxPtr->GetBoxRegionStage(BoxRegion);
		JetAPI::UnionRegion(BoxRegion, LandRegion, LandRegion);
	}		

	//Body Region
	BoxPtr = CAOILand::GetLandBodyEdgeBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true )
	{
		BoxPtr->GetBoxRegionStage(BoxRegion);
		JetAPI::UnionRegion(BoxRegion, LandRegion, LandRegion);
	}	
	/*
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = CAOILand::GetLandWndPtr_Inline(i);
		if ( WndPtr == NULL ) { continue; }

		if ( WndPtr->GetWndExtendBoxUsed() == true )
		{	BoxPtr = WndPtr->GetWndExtendBoxPtr();	}
		else
		{	BoxPtr = WndPtr->GetWndBoxPtr();	}

		BoxPtr->GetBoxRegionStage(BoxRegion);
		JetAPI::UnionRegion(BoxRegion, LandRegion, LandRegion);
	}	
	*/
	return ;
}
//-------------------------------------------------------------------------------------//
void CAOILand::UpdateLandTotalRegion()
{
	size_t i = 0;	
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	TREGION4D    Region;
	double MinX=0, MinY=0, MaxX=0, MaxY=0;
	const size_t WndCount = CAOILand::GetLandWndCount_Inline();
	
	//Base
	BoxPtr = CAOILand::GetLandBoxPtr();//GetLandPadBoxPtr();
	BoxPtr->GetBoxRegion(m_LandRegion);	

	//Pad Region
	BoxPtr = CAOILand::GetLandPadBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true )
	{
		BoxPtr->GetBoxRegion(Region);
		JetAPI::UnionRegion(Region, m_LandRegion, m_LandRegion);
	}			

	//Lead Region
	BoxPtr = CAOILand::GetLandLeadBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true )
	{
		BoxPtr->GetBoxRegion(Region);
		JetAPI::UnionRegion(Region, m_LandRegion, m_LandRegion);
	}		

	//Body Region
	BoxPtr = CAOILand::GetLandBodyEdgeBoxPtr();
	if ( BoxPtr->GetBoxEnabled() == true )
	{
		BoxPtr->GetBoxRegion(Region);
		JetAPI::UnionRegion(Region, m_LandRegion, m_LandRegion);
	}	
	
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = CAOILand::GetLandWndPtr_Inline(i);
		if ( WndPtr == NULL ) { continue; }

		if ( WndPtr->GetWndExtendBoxUsed() == true )
		{	BoxPtr = WndPtr->GetWndExtendBoxPtr();	}
		else
		{	BoxPtr = WndPtr->GetWndBoxPtr();	}

		BoxPtr->GetBoxRegion(Region);
		JetAPI::UnionRegion(Region, m_LandRegion, m_LandRegion);
	}	
}
//--------------------------------------------------------------------------------------------//
void CAOILand::AlignLandU()//特徵框的軸向相同
{	
	TPOINT2D SrcPos;
	TPOINT2D DstPos;	
	TPOINT2D MovPos;
	const bool bIncludeRes = true;
	BOX_TOWARD LandToward = GetLandToward();	
	GetLandBoxPtr()->GetBoxPos(DstPos);

	m_LandPadBox.GetBoxPos(SrcPos);
	switch ( LandToward )
	{
	case BOX_TOWARD_UP:
	case BOX_TOWARD_DOWN:
		MovPos.y = 0;
		MovPos.x = DstPos.x-SrcPos.x;		
		break;
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		MovPos.x = 0;
		MovPos.y = DstPos.y-SrcPos.y;
		break;
	}
	m_LandPadBox.MoveBox(MovPos.x, MovPos.y, bIncludeRes);
	
	m_LandLeadBox.GetBoxPos(SrcPos);
	switch ( LandToward )
	{
	case BOX_TOWARD_UP:
	case BOX_TOWARD_DOWN:
		MovPos.y = 0;
		MovPos.x = DstPos.x-SrcPos.x;		
		break;
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		MovPos.x = 0;
		MovPos.y = DstPos.y-SrcPos.y;
		break;
	}
	m_LandLeadBox.MoveBox(MovPos.x, MovPos.y, bIncludeRes);

	m_LandLeadTipBox.GetBoxPos(SrcPos);
	switch ( LandToward )
	{
	case BOX_TOWARD_UP:
	case BOX_TOWARD_DOWN:
		MovPos.y = 0;
		MovPos.x = DstPos.x-SrcPos.x;		
		break;
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		MovPos.x = 0;
		MovPos.y = DstPos.y-SrcPos.y;
		break;
	}
	m_LandLeadTipBox.MoveBox(MovPos.x, MovPos.y, bIncludeRes);

	m_LandLeadShoulderBox.GetBoxPos(SrcPos);
	switch ( LandToward )
	{
	case BOX_TOWARD_UP:
	case BOX_TOWARD_DOWN:
		MovPos.y = 0;
		MovPos.x = DstPos.x-SrcPos.x;		
		break;
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		MovPos.x = 0;
		MovPos.y = DstPos.y-SrcPos.y;
		break;
	}
	m_LandLeadShoulderBox.MoveBox(MovPos.x, MovPos.y, bIncludeRes);

	m_LandBodyEdgeBox.GetBoxPos(SrcPos);
	switch ( LandToward )
	{
	case BOX_TOWARD_UP:
	case BOX_TOWARD_DOWN:
		MovPos.y = 0;
		MovPos.x = DstPos.x-SrcPos.x;		
		break;
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		MovPos.x = 0;
		MovPos.y = DstPos.y-SrcPos.y;
		break;
	}
	m_LandBodyEdgeBox.MoveBox(MovPos.x, MovPos.y, bIncludeRes);
	return;
}
//--------------------------------------------------------------------------------------------//
bool CAOILand::CheckLandLeadFollowPad() const//確認引腳跟焊盤移動
{
	if ( LAND_TYPE_DIP_LEAD == GetLandType() )
	{	return true; }
	return false;
}
//--------------------------------------------------------------------------------------------//
void CAOILand::ScaleLandProperty(double sx, double sy)//縮放特徵框屬性
{
	m_LandLeadSizeX *= sx;
	m_LandLeadSizeY *= sy;
	m_LandLeadTipSizeX *= sx;
	m_LandLeadTipSizeY *= sy;
	m_LandLeadShoulderSizeX *= sx;
	m_LandLeadShoulderSizeY *= sy;
	return;
}
//--------------------------------------------------------------------------------------------//
void CAOILand::ScaleLand(double sx, double sy, bool bIncludeRes)//縮放特徵框
{
	size_t i=0;		

	ScaleLandProperty(sx, sy);	
	m_LandPadBox.ScaleBox(sx, sy, bIncludeRes);
	m_LandLeadBox.ScaleBox(sx, sy, bIncludeRes);
	m_LandLeadTipBox.ScaleBox(sx, sy, bIncludeRes);
	m_LandLeadShoulderBox.ScaleBox(sx, sy, bIncludeRes);
	m_LandBodyEdgeBox.ScaleBox(sx, sy, bIncludeRes);
	
	CAOIWnd     *WndPtr=NULL;
	const size_t WndCount = GetLandWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetLandWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		WndPtr->ScaleWnd(sx, sy, bIncludeRes);	
	}
	return ;
}
//--------------------------------------------------------------------------------------------//
void CAOILand::MoveLand(double x, double y, bool bIncludeRes)
{
	size_t i=0;		
	m_LandPadBox.MoveBox(x, y, bIncludeRes);
	m_LandLeadBox.MoveBox(x, y, bIncludeRes);
	m_LandLeadTipBox.MoveBox(x, y, bIncludeRes);
	m_LandLeadShoulderBox.MoveBox(x, y, bIncludeRes);
	m_LandBodyEdgeBox.MoveBox(x, y, bIncludeRes);
	
	CAOIWnd     *WndPtr=NULL;
	const size_t WndCount = GetLandWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetLandWndPtr_Inline(i);
		if ( NULL == WndPtr ) { continue; }
		WndPtr->MoveWnd(x, y, bIncludeRes);		
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CAOILand::MoveLandResult(double x, double y)
{
	size_t i=0;	
	m_LandPadBox.MoveBoxRes(x, y);
	m_LandLeadBox.MoveBoxRes(x, y);
	m_LandLeadTipBox.MoveBoxRes(x, y);
	m_LandLeadShoulderBox.MoveBoxRes(x, y);
	m_LandBodyEdgeBox.MoveBoxRes(x, y);

//	CAOILand::m_LandPadBox.MoveBoxResStage(-x, -y);
//	CAOILand::m_LandLeadBox.MoveBoxResStage(-x, -y);
//	CAOILand::m_LandLeadTipBox.MoveBoxResStage(-x, -y);
//	CAOILand::m_LandLeadShoulderBox.MoveBoxResStage(-x, -y);
//	CAOILand::m_LandBodyEdgeBox.MoveBoxResStage(-x, -y);
	
//	CAOIWnd     *WndPtr=NULL;
//	const size_t WndCount = GetLandWndCount_Inline();
//	for ( i=0; i<WndCount; i++ )
//	{
//		WndPtr = GetLandWndPtr_Inline(i);
//		if ( NULL == WndPtr ) { continue; }
//		WndPtr->MoveWndResult(x, y);		
//	}	
	return ;
}
//-------------------------------------------------------------------------------------//
void CAOILand::MoveLandPad(double x, double y, bool bIncludeRes)
{
	m_LandPadBox.MoveBox(x, y, bIncludeRes);
}
//-------------------------------------------------------------------------------------//
void CAOILand::MoveLandLead(double x, double y, bool bIncludeRes)
{
	m_LandLeadBox.MoveBox(x, y, bIncludeRes);
	m_LandLeadTipBox.MoveBox(x, y, bIncludeRes);
	m_LandLeadShoulderBox.MoveBox(x, y, bIncludeRes);
	return ;
}
//-------------------------------------------------------------------------------------//
void CAOILand::MoveLandPadResult(double x, double y)
{
	m_LandPadBox.MoveBoxRes(x, y);
}
//-------------------------------------------------------------------------------------//
void CAOILand::MoveLandLeadResult(double x, double y)
{	
	m_LandLeadBox.MoveBoxRes(x, y);
	m_LandLeadTipBox.MoveBoxRes(x, y);
	m_LandLeadShoulderBox.MoveBoxRes(x, y);	
	return ;
}
//-------------------------------------------------------------------------------------//
void CAOILand::ModifyLandLeadSize(const CAOIBox *BoxPtr, const TREGION4D &dPos, bool bIncludeRes)
{	
	if ( &m_LandLeadBox == BoxPtr )
	{	
		m_LandLeadBox.ModifyBoxRegion(dPos, bIncludeRes);	
		LayoutLeadBox(false);
		if ( true == bIncludeRes )
		{	LayoutLeadBox(true);	}
	}
	if ( &m_LandLeadTipBox == BoxPtr )
	{	
		m_LandLeadTipBox.ModifyBoxRegion(dPos, bIncludeRes);	
		LayoutLeadTipBox(false);
		if ( true == bIncludeRes )
		{	LayoutLeadTipBox(true);	}
	}
	if ( &m_LandLeadShoulderBox == BoxPtr )
	{	
		m_LandLeadShoulderBox.ModifyBoxRegion(dPos, bIncludeRes);	
		LayoutLeadShoulderBox(false);
		if ( true == bIncludeRes )
		{	LayoutLeadShoulderBox(true);	}
	}
}
//-------------------------------------------------------------------------------------//
void CAOILand::ModifyLandLeadSizeRes(const CAOIBox *BoxPtr, const TREGION4D &dPos)
{
	if ( &m_LandLeadBox == BoxPtr )
	{	
		m_LandLeadBox.ModifyBoxRegionRes(dPos);	
		CAOILand::LayoutLeadBox(true);		
	}
	if ( &m_LandLeadTipBox == BoxPtr )
	{	
		m_LandLeadTipBox.ModifyBoxRegionRes(dPos);	
		CAOILand::LayoutLeadTipBox(true);		
	}
	if ( &m_LandLeadShoulderBox == BoxPtr )
	{	
		m_LandLeadShoulderBox.ModifyBoxRegionRes(dPos);	
		CAOILand::LayoutLeadShoulderBox(true);	}	
	}
//-------------------------------------------------------------------------------------//
void CAOILand::RotateLand(double Angle, double CPX, double CPY)
{
	size_t    i=0;		
	const int AngleLable = JetAPI::GetAngleLabel(Angle);
	switch ( AngleLable )
	{
	case  90:
	case 270:
		JetAPI::Swap(m_LandLeadSizeX, m_LandLeadSizeY);
		JetAPI::Swap(m_LandLeadTipSizeX, m_LandLeadTipSizeY);
		JetAPI::Swap(m_LandLeadShoulderSizeX, m_LandLeadShoulderSizeY);		
		break;
	}
	
	m_LandPadBox.RotateBox(Angle, CPX, CPY);
	m_LandLeadBox.RotateBox(Angle, CPX, CPY);
	m_LandLeadTipBox.RotateBox(Angle, CPX, CPY);
	m_LandLeadShoulderBox.RotateBox(Angle, CPX, CPY);
	m_LandBodyEdgeBox.RotateBox(Angle, CPX, CPY);		

	CAOIWnd     *WndPtr=NULL;
	const size_t WndCount = GetLandWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetLandWndPtr_Inline(i);
		if ( WndPtr == NULL ) { continue; }
		WndPtr->RotateWnd(Angle, CPX, CPY);
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CAOILand::MirrorLandXAxis(double CPY)
{
	size_t i=0;		
	m_LandPadBox.MirrorBoxXAxis(CPY);
	m_LandLeadBox.MirrorBoxXAxis(CPY);
	m_LandLeadTipBox.MirrorBoxXAxis(CPY);
	m_LandLeadShoulderBox.MirrorBoxXAxis(CPY);
	m_LandBodyEdgeBox.MirrorBoxXAxis(CPY);	

	CAOIWnd     *WndPtr=NULL;
	const size_t WndCount = GetLandWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetLandWndPtr_Inline(i);;
		if ( WndPtr == NULL ) { continue; }
		WndPtr->MirrorWndXAxis(CPY);
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CAOILand::MirrorLandYAxis(double CPX)
{
	size_t i=0;		
	m_LandPadBox.MirrorBoxYAxis(CPX);
	m_LandLeadBox.MirrorBoxYAxis(CPX);
	m_LandLeadTipBox.MirrorBoxYAxis(CPX);
	m_LandLeadShoulderBox.MirrorBoxYAxis(CPX);
	m_LandBodyEdgeBox.MirrorBoxYAxis(CPX);	

	CAOIWnd     *WndPtr=NULL;
	const size_t WndCount = GetLandWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetLandWndPtr_Inline(i);;
		if ( WndPtr == NULL ) { continue; }
		WndPtr->MirrorWndYAxis(CPX);
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CAOILand::SetLandAttachedAngle(double Angle)
{
	m_LandPadBox.SetBoxAttachedAngle(Angle);
	m_LandLeadBox.SetBoxAttachedAngle(Angle);
	m_LandLeadTipBox.SetBoxAttachedAngle(Angle);
	m_LandLeadShoulderBox.SetBoxAttachedAngle(Angle);
	m_LandBodyEdgeBox.SetBoxAttachedAngle(Angle);

	size_t       i=0;	
	CAOIWnd     *WndPtr=NULL;
	const size_t WndCount = GetLandWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetLandWndPtr_Inline(i);;
		if ( WndPtr == NULL ) { continue; }
		WndPtr->SetWndAttachedAngle(Angle);
	}
}
//-------------------------------------------------------------------------------------//
void CAOILand::SetLandAttachedPosCad(const TPOINT2D &Pos)
{
	m_LandPadBox.SetBoxAttachedPosCad(Pos);
	m_LandLeadBox.SetBoxAttachedPosCad(Pos);
	m_LandLeadTipBox.SetBoxAttachedPosCad(Pos);
	m_LandLeadShoulderBox.SetBoxAttachedPosCad(Pos);
	m_LandBodyEdgeBox.SetBoxAttachedPosCad(Pos);	

	size_t       i=0;	
	CAOIWnd     *WndPtr=NULL;
	const size_t WndCount = GetLandWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetLandWndPtr_Inline(i);;
		if ( WndPtr == NULL ) { continue; }
		WndPtr->SetWndAttachedPosCad(Pos);
	}
}
//-------------------------------------------------------------------------------------//
void CAOILand::SetLandAttachedPosCad(double PosX, double PosY)
{
	m_LandPadBox.SetBoxAttachedPosCad(PosX, PosY);
	m_LandLeadBox.SetBoxAttachedPosCad(PosX, PosY);
	m_LandLeadTipBox.SetBoxAttachedPosCad(PosX, PosY);
	m_LandLeadShoulderBox.SetBoxAttachedPosCad(PosX, PosY);
	m_LandBodyEdgeBox.SetBoxAttachedPosCad(PosX, PosY);

	size_t       i=0;	
	CAOIWnd     *WndPtr=NULL;
	const size_t WndCount = GetLandWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetLandWndPtr_Inline(i);
		if ( WndPtr == NULL ) { continue; }
		WndPtr->SetWndAttachedPosCad(PosX, PosY);
	}
}
//-------------------------------------------------------------------------------------//
void CAOILand::SetLandAttachedPosStage(const TPOINT2D &Pos)
{
	m_LandPadBox.SetBoxAttachedPosStage(Pos);
	m_LandLeadBox.SetBoxAttachedPosStage(Pos);
	m_LandLeadTipBox.SetBoxAttachedPosStage(Pos);
	m_LandLeadShoulderBox.SetBoxAttachedPosStage(Pos);
	m_LandBodyEdgeBox.SetBoxAttachedPosStage(Pos);	

	size_t       i=0;	
	CAOIWnd     *WndPtr=NULL;
	const size_t WndCount = GetLandWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetLandWndPtr_Inline(i);
		if ( WndPtr == NULL ) { continue; }
		WndPtr->SetWndAttachedPosStage(Pos);
	}
}
//-------------------------------------------------------------------------------------//
void CAOILand::SetLandAttachedPosStage(double PosX, double PosY)
{
	CAOILand::m_LandPadBox.SetBoxAttachedPosStage(PosX, PosY);
	CAOILand::m_LandLeadBox.SetBoxAttachedPosStage(PosX, PosY);
	CAOILand::m_LandLeadTipBox.SetBoxAttachedPosStage(PosX, PosY);
	CAOILand::m_LandLeadShoulderBox.SetBoxAttachedPosStage(PosX, PosY);
	CAOILand::m_LandBodyEdgeBox.SetBoxAttachedPosStage(PosX, PosY);

	size_t       i=0;	
	CAOIWnd     *WndPtr=NULL;
	const size_t WndCount = GetLandWndCount_Inline();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = GetLandWndPtr_Inline(i);
		if ( WndPtr == NULL ) { continue; }
		WndPtr->SetWndAttachedPosStage(PosX, PosY);
	}
}
//-------------------------------------------------------------------------------------//
bool CAOILand::InitLandInspection()//初始化特徵框檢測
{
	m_LandPadBox.ResetBoxRegionRes();
	m_LandLeadBox.ResetBoxRegionRes();
	m_LandLeadTipBox.ResetBoxRegionRes();
	m_LandLeadShoulderBox.ResetBoxRegionRes();
	m_LandBodyEdgeBox.ResetBoxRegionRes();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::ApplyLand(const CAOILand *RefLandPtr)//套用相同群組的特徵框
{
	if ( NULL == RefLandPtr ) { return false; }

	m_LandType = RefLandPtr->m_LandType;
	m_LandIndex = RefLandPtr->m_LandIndex;
	m_LandGroupID = RefLandPtr->m_LandGroupID;
	m_LandAlignID = RefLandPtr->m_LandAlignID;
	m_LandModified = RefLandPtr->m_LandModified;	
	m_LandIncludePadAlign = RefLandPtr->m_LandIncludePadAlign;
	m_LandIncludePartAlign = RefLandPtr->m_LandIncludePartAlign;
	
	//m_LandFirstOne = RefLandPtr->m_LandFirstOne;
	//m_LandLastOne = RefLandPtr->m_LandLastOne;	
	//CAOIModel                 *m_LandModelPtr;               //特徵框所屬的模組
	//---------------------------------------------------------------------------------//	
	m_LandPadBox.ApplyBox(&(RefLandPtr->m_LandPadBox));
	m_LandLeadBox.ApplyBox(&(RefLandPtr->m_LandLeadBox));
	m_LandLeadTipBox.ApplyBox(&(RefLandPtr->m_LandLeadTipBox));
	m_LandLeadShoulderBox.ApplyBox(&(RefLandPtr->m_LandLeadShoulderBox));
	m_LandBodyEdgeBox.ApplyBox(&(RefLandPtr->m_LandBodyEdgeBox));
	//TREGION4D                  m_LandRegion;               //特徵框的範圍-模組內
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOILand::SynchronousLand(const CAOILand *RefLandPtr)//同步化同一個特徵框
{
	if ( NULL == RefLandPtr ) { return false; }

	m_LandType = RefLandPtr->m_LandType;
	m_LandIndex = RefLandPtr->m_LandIndex;
	m_LandGroupID = RefLandPtr->m_LandGroupID;
	m_LandAlignID = RefLandPtr->m_LandAlignID;
	m_LandFirstOne = RefLandPtr->m_LandFirstOne;
	m_LandLastOne = RefLandPtr->m_LandLastOne;	
	m_LandIncludePadAlign = RefLandPtr->m_LandIncludePadAlign;
	m_LandIncludePartAlign = RefLandPtr->m_LandIncludePartAlign;
	
	m_LandModified = RefLandPtr->m_LandModified;
	//CAOIModel                 *m_LandModelPtr;               //特徵框所屬的模組
	//---------------------------------------------------------------------------------//	
	m_LandPadBox.SynchronousBox(&(RefLandPtr->m_LandPadBox));
	m_LandLeadBox.SynchronousBox(&(RefLandPtr->m_LandLeadBox));
	m_LandLeadTipBox.SynchronousBox(&(RefLandPtr->m_LandLeadTipBox));
	m_LandLeadShoulderBox.SynchronousBox(&(RefLandPtr->m_LandLeadShoulderBox));
	m_LandBodyEdgeBox.SynchronousBox(&(RefLandPtr->m_LandBodyEdgeBox));
	//TREGION4D                  m_LandRegion;               //特徵框的範圍-模組內
	return true;
}
//-------------------------------------------------------------------------------------//