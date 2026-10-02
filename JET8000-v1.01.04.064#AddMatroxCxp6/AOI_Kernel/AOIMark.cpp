// AOIMark.cpp: implementation of the CAOIMark class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIMark.h"
//-------------------------------------------------------------------------------------//
#include "AOIFileIO.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
CRITICAL_SECTION  CAOIMark::m_csMark;//同步機制-關鍵區間
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CAOIMark, CAOIRgn)
//-------------------------------------------------------------------------------------//
void CAOIMark::InitialMarkLock()//初始化特徵點的關鍵區間
{
	::InitializeCriticalSection(&m_csMark);
}
//-------------------------------------------------------------------------------------//
void CAOIMark::DeleteMarkLock() //刪除特徵點的關鍵區間
{
	::DeleteCriticalSection(&m_csMark);
}
//-------------------------------------------------------------------------------------//
void CAOIMark::LockMark()        //進入特徵點的關鍵區間
{
	::EnterCriticalSection(&m_csMark);
}
//-------------------------------------------------------------------------------------//
void CAOIMark::UnlockMark()      //離開特徵點的關鍵區間
{
	::LeaveCriticalSection(&m_csMark);
}
//-------------------------------------------------------------------------------------//
CAOIMark::CAOIMark():CAOIRgn(AOI_OBJ_MARK)
{
	PreInitMark();
	InitialMark();
}
//-------------------------------------------------------------------------------------//
CAOIMark::CAOIMark(const CAOIMark &others):CAOIRgn(others)
{
	PreInitMark();
	CloneMark(others);
}
//-------------------------------------------------------------------------------------//
CAOIMark::~CAOIMark()
{

}
//-------------------------------------------------------------------------------------//
CAOIMark& CAOIMark::operator=(const CAOIMark &others)
{
	if ( this == &others ) { return *this; }
	CAOIRgn::operator=(others);
	CloneMark(others);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CAOIMark::PreInitMark()
{		
}
//-------------------------------------------------------------------------------------//
void CAOIMark::InitialMark()
{	
	m_MarkDeleted = false;//特徵點是否刪除
	m_MarkSelected = false;//特徵點是否選取到
	m_MarkUniqueID = -1;
	m_MarkGroupID = -1;		
	m_MarkTempInt[0] = 0;
	m_MarkTempInt[1] = 0;
	m_MarkTempInt[2] = 0;
	m_MarkTempInt[3] = 0;	
	m_MarkTypeMode = MARK_TASK_NONE;	
	m_MarkGroundEquation_Online = CJetGroundEquation();
	//---------------------------------------------------------------------------------//	
	m_MarkModel.SetModelMarkPtr(this);
	m_MarkModel.SetModelIsolated(false);
	m_MarkModel.SetModelExtendRangeX(0);
	m_MarkModel.SetModelExtendRangeY(0);
	m_MarkModel.SetModelExtendAutoAdjust(false);
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
void CAOIMark::CloneMark(const CAOIMark &others)
{	
	m_MarkDeleted = others.m_MarkDeleted;//特徵點是否刪除
	m_MarkSelected = others.m_MarkSelected;//特徵點是否選取到
	m_MarkUniqueID = others.m_MarkUniqueID;
	m_MarkGroupID = others.m_MarkGroupID;	
	m_MarkTempInt[0] = others.m_MarkTempInt[0];
	m_MarkTempInt[1] = others.m_MarkTempInt[1];
	m_MarkTempInt[2] = others.m_MarkTempInt[2];
	m_MarkTempInt[3] = others.m_MarkTempInt[3];	
	m_MarkTypeMode = others.m_MarkTypeMode;	
	m_MarkGroundEquation_Online = others.m_MarkGroundEquation_Online;
	//---------------------------------------------------------------------------------//
	m_MarkModel = others.m_MarkModel;
	m_MarkModel.SetModelMarkPtr(this);	
	//---------------------------------------------------------------------------------//				
}
//-------------------------------------------------------------------------------------//
CAOIMark* CAOIMark::CloneMarkObj() const//建立且複製一個特徵點
{
	CAOIMark *ObjPtr = AOIObjManager.CreateMarkObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::WriteMarkFile(CAOIFileIO &FileIO)//儲存特徵點檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	CAOIMark   * pMark = this;
	char         uuidStr[MAX_JET_PATH]="";	
	wchar_t      uuidWStr[MAX_JET_PATH]=L"";	
	UUID         uuid = pMark->GetObjUuid();	
	FileIO.SetFnName(_T("CAOIMark::WriteMarkFile"));
	JetAPI::UUIDToStringA(uuid, uuidStr);
	JetAPI::UUIDToStringW(uuid, uuidWStr);
	//----------------------------------------------------------------------------------------//
	//特徵點參數
	if ( FileIO.SaveChunk_INT(FILE_IO_MARK_START, 0) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_MARK_UNIQUE_ID, pMark->GetMarkUniqueID()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_MARK_INDEX_PROJECT, pMark->GetMarkIndex_Project()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_MARK_PANEL_INDEX, pMark->GetMarkPanelIndex_Project()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_MARK_BOARD_INDEX, pMark->GetMarkBoardIndex_Project()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_MARK_ANGLE, pMark->GetMarkAngle()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_MARK_BODY_SIZE_CX, pMark->GetMarkBodySizeW()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_MARK_BODY_SIZE_CY, pMark->GetMarkBodySizeH()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_MARK_ROI_SIZE_CX, pMark->GetMarkRoiSizeW()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_MARK_ROI_SIZE_CY, pMark->GetMarkRoiSizeH()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_MARK_CAD_POS_X, pMark->GetMarkCadPosX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_MARK_CAD_POS_Y, pMark->GetMarkCadPosY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_MARK_STAGE_POS_X, pMark->GetMarkStagePosX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_MARK_STAGE_POS_Y, pMark->GetMarkStagePosY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_MARK_STAGE_POS_Z, pMark->GetMarkStagePosZ()) == false ) { return false; }
	if ( FileIO.GetSaveWStr() == true ) 
	{	if ( FileIO.SaveChunk_STR(FILE_IO_MARK_OBJ_UUID, uuidWStr) == false ) { return false; } }
	else
	{	if ( FileIO.SaveChunk_STR(FILE_IO_MARK_OBJ_UUID, uuidStr) == false ) { return false; } }		
	if ( FileIO.SaveChunk_BOL(FILE_IO_MARK_BYPASSED, pMark->GetMarkBypassed()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_MARK_DISTRICT_ID, pMark->GetMarkDistrictID()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_MARK_GROUP_ID, pMark->GetMarkGroupID()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_MARK_LOCAL_BASE_PLANE_ID, pMark->GetMarkLocalBasePlaneID()) == false ) { return false; }	             
	if ( FileIO.SaveChunk_INT(FILE_IO_MARK_TYPE_MODE, pMark->GetMarkTypeMode()) == false ) { return false; }

	const TNoiseFilterParam &NoiseFilterParam = pMark->GetMarkSpaceNoiseFilterParam();	
	if ( FileIO.SaveChunk_INT(FILE_IO_MARK_3D_NOISE_FILTER_NODE, 0) == false ) { return false; }
	if ( FileIO.WriteSpaceNoistFilterParamFile(NoiseFilterParam) == false ) { return false; }

	//特徵點-模組參數
	CAOIModel *ModelPtr = pMark->GetMarkModelPtr();
	if ( FileIO.SaveChunk_INT(FILE_IO_MARK_MODEL_NODE, 0) == false ) { return false; }
	if ( ModelPtr->WriteModelFile(FileIO) == false )
	{	return false; }
	
	if ( FileIO.SaveChunk_INT(FILE_IO_MARK_END, 0) == false ) { return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::ReadMarkFile(CAOIFileIO &FileIO)//載入特徵點檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	UUID         uuid;
	int          index = 0;
	unsigned int uValue = 0;
	CString      ModelName;
	CString      ModelFolder;
	CString      BarcodeName;
	CAOIWnd     *WndPtr = NULL;
	CAOIModel   *ModelPtr = NULL;
	CAOIMark    *pMark = this;	
	FileIO.SetFnName(_T("CAOIMark::ReadMarkFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		
		switch ( index )
		{
		case FILE_IO_MARK_START://特徵點參數-起點
			break;
		case FILE_IO_MARK_END://特徵點參數-終點
			pMark->CalcMarkCadCornerPos();
			pMark->LayoutMarkStageCornerPos();
			pMark->UpdateMarkParamToModel();

			ModelPtr = pMark->GetMarkModelPtr();
			ModelPtr->UnSelectModel();
			WndPtr = pMark->GetMarkWndPtr();
			if ( NULL != WndPtr )
			{	WndPtr->SetWndSelected(true);	}
			ModelPtr->SetModelWndActived(WndPtr);		
			return true;
			break;	
		case FILE_IO_MARK_UNIQUE_ID:
			pMark->SetMarkUniqueID(FileIO.GetData_INT());
			break;
		case FILE_IO_MARK_INDEX_PROJECT:
			pMark->SetMarkIndex_Project(FileIO.GetData_INT());
			break;
		case FILE_IO_MARK_PANEL_INDEX:
			pMark->SetMarkPanelIndex_Project(FileIO.GetData_INT());
			break;
		case FILE_IO_MARK_BOARD_INDEX:
			pMark->SetMarkBoardIndex_Project(FileIO.GetData_INT());
			break;
		case FILE_IO_MARK_ANGLE:
			pMark->SetMarkAngle(FileIO.GetData_DBL());
			break;
		case FILE_IO_MARK_BODY_SIZE_CX:
			pMark->SetMarkBodySizeW(FileIO.GetData_DBL());
			break;
		case FILE_IO_MARK_BODY_SIZE_CY:
			pMark->SetMarkBodySizeH(FileIO.GetData_DBL());
			break;
		case FILE_IO_MARK_ROI_SIZE_CX:
			pMark->SetMarkRoiSizeW(FileIO.GetData_DBL());
			break;
		case FILE_IO_MARK_ROI_SIZE_CY:
			pMark->SetMarkRoiSizeH(FileIO.GetData_DBL());
			break;		
		case FILE_IO_MARK_CAD_POS_X:
			pMark->SetMarkCadPosX(FileIO.GetData_DBL());
			break;
		case FILE_IO_MARK_CAD_POS_Y:
			pMark->SetMarkCadPosY(FileIO.GetData_DBL());
			break;
		case FILE_IO_MARK_STAGE_POS_X:
			pMark->SetMarkStagePosX(FileIO.GetData_DBL());
			break;
		case FILE_IO_MARK_STAGE_POS_Y:
			pMark->SetMarkStagePosY(FileIO.GetData_DBL());
			break;
		case FILE_IO_MARK_STAGE_POS_Z:
			pMark->SetMarkStagePosZ(FileIO.GetData_DBL());
			break;
		case FILE_IO_MARK_OBJ_UUID://特徵點參數-OBJ-UUID
			if ( FileIO.GetLoadWStr()==true )
			{
				if ( JetAPI::UUIDFromStringW(FileIO.GetData_WSTR(), uuid) == true )
				{	pMark->SetObjUuid(uuid);	}
			}
			else
			{
				if ( JetAPI::UUIDFromStringA(FileIO.GetData_STR(), uuid) == true )
				{	pMark->SetObjUuid(uuid);	}
			}
			break;		
		case FILE_IO_MARK_BYPASSED://特徵點參數-不檢測
			pMark->SetMarkBypassed(FileIO.GetData_BOL());
			break;
		case FILE_IO_MARK_DISTRICT_ID://特徵點參數-分段編號
			pMark->SetMarkDistrictID((DISTRICT_ID)(FileIO.GetData_INT()));
			break;
		case FILE_IO_MARK_GROUP_ID://特徵點參數-群組編號
			pMark->SetMarkGroupID(FileIO.GetData_INT());
			break;
		case FILE_IO_MARK_LOCAL_BASE_PLANE_ID://特徵點參數-局部基準面編號
			pMark->SetMarkLocalBasePlaneID(FileIO.GetData_INT());
			break;
		case FILE_IO_MARK_TYPE_MODE://特徵點參數-樣式模式
			pMark->SetMarkTypeMode((MARK_TYPE_MODE)(FileIO.GetData_INT()));
			break;

		case FILE_IO_MARK_3D_NOISE_FILTER_NODE://特徵點參數-3D雜訊過濾參數
			if ( FileIO.ReadSpaceNoiseFilterParamFile(m_RgnSpaceNoiseFilterParam) == false ) { return false; }
			break;	
		case FILE_IO_MARK_MODEL_NODE:
			ModelPtr = pMark->GetMarkModelPtr();
			if ( NULL != ModelPtr )
			{
				ModelName = ModelPtr->GetModelName();
				ModelFolder.Format(_T("%s\\%s"), FileIO.GetLibraryFolder(), ModelName);
				ModelPtr->SetModelFolderModel(ModelFolder);

				uValue = pMark->GetMarkIndex_Project();
				BarcodeName = AOIDataDefine.GetMarkFullName(uValue, _T("Mark"));
				ModelFolder.Format(_T("%s\\%s"), FileIO.GetPartLibraryFolder(), BarcodeName);										
				ModelPtr->SetModelFolderComponent(ModelFolder);				
				if ( ModelPtr->ReadModelFile(FileIO) == false ) { return false; }
			}
			break;
		default:
			break;
		}
	};	
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIModel* CAOIMark::GetMarkModelPtr()//取得特徵點模組
{
	return &m_MarkModel;
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::UpdateMarkParamToModel()//更新特徵點參數至模組內
{	
	TPOINT2D PosCad, PosStage;	
	CAOIModel  &ModelObj=m_MarkModel;	
	double SizeW = GetMarkRoiSizeW();
	double SizeH = GetMarkRoiSizeH();
	double BodyW = GetMarkBodySizeW();
	double BodyH = GetMarkBodySizeH();
	double ModelBodySizeW = BodyW;
	double ModelBodySizeH = BodyH;
	const double Angle = GetMarkAngle();
	const double CadPosX = GetMarkCadPosX();
	const double CadPosY = GetMarkCadPosY();
	const double StagePosX = GetMarkStagePosX();
	const double StagePosY = GetMarkStagePosY();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(Angle);
	CAOIBox *BoxPtr = ModelObj.GetModelBodyBoxPtr();
	
	PosCad.x = CadPosX;
	PosCad.y = CadPosY;
	PosStage.x = StagePosX;
	PosStage.y = StagePosY;
	
	JetAPI::RotateSize(Angle, ModelBodySizeW, ModelBodySizeH);
	BoxPtr->SetBoxSize(ModelBodySizeW, ModelBodySizeH, true);

	if ( false == IsExceptionAngle )
	{	ModelObj.SetModelAttachedAngle(Angle);	}
	else
	{	
		ModelObj.SetModelAttachedAngle(0);
		ModelObj.RotateModel(Angle, 0, 0);
	}		
	ModelObj.SetModelMarkPtr(this);
	ModelObj.SetModelAttachedPosCad(PosCad);
	ModelObj.SetModelAttachedPosStage(PosStage);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::UpdateMarkModelFromLibrary(CAOIModel *RefModelPtr)//更新特徵點模組
{
	CAOIMark *MarkPtr = this;
	if ( NULL == MarkPtr ) { return false; }
	if ( NULL == RefModelPtr ) { return false; }
	
	const bool bClearDst=true;
	const bool bClearSrc=false;	
	CAOIModel  &ModelObj=m_MarkModel;	
	CString ModelName = ModelObj.GetModelName();
	CString ModelFolder = ModelObj.GetModelFolder();
	CString RefModelFolder = RefModelPtr->GetModelFolder();			
	if ( ModelFolder.CompareNoCase(RefModelFolder) != 0 ) 
	{	JetAPI::CopyFolderAToFolderB(RefModelFolder, ModelFolder, bClearSrc, bClearDst, _T(""), -1, -1);	}	

	const double MarkAngle = GetMarkAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(MarkAngle);
	
	ModelObj = *RefModelPtr;
	ModelObj.SetModelAttachedAngle(0);	
	ModelObj.RotateModel(MarkAngle, 0, 0);

	ModelObj.SetModelMarkPtr(this);
	ModelObj.SetModelName(ModelName);
	ModelObj.SetModelFolderModel(ModelFolder);
	ModelObj.AssignModelFolder();	
	ModelObj.UpdateModelRegionToAttached();	

	const TNoiseFilterParam &NoiseFilterParam = GetMarkSpaceNoiseFilterParam();
	ModelObj.SetModelSpaceBasePlaneParam(NoiseFilterParam.BasePlaneParam);
	ModelObj.SetModelSpaceNoiseFilterParam(NoiseFilterParam);

	//特殊遮罩-基板顏色
	bool RgnMaskEnable_Base = GetRgnMaskEnable_Base();		
	unsigned int RgnMaskFrameIndex_Base = GetRgnMaskFrameIndex_Base();	
	unsigned int RgnMaskFrameUniqueID_Base = GetRgnMaskFrameUniqueID_Base();	
	const int RgnMaskColorGroupLinkIndex = GetRgnMaskColorGroupLinkIndex();	

	ModelObj.SetModelMaskEnable_Base(RgnMaskEnable_Base);
	ModelObj.SetModelMaskFrameIndex_Base(RgnMaskFrameIndex_Base);
	ModelObj.SetModelMaskFrameUniqueID_Base(RgnMaskFrameUniqueID_Base);
	ModelObj.SetModelMaskColorGroupLinkIndex(RgnMaskColorGroupLinkIndex);	

	const double PanelBasePlane=GetMarkPanelBasePlane();
	ModelObj.SetModelPanelBasePlane(PanelBasePlane);		

	const bool DataModelEnabled=GetMarkDataModelEnabled();
	ModelObj.SetModelDataModelEnabled(DataModelEnabled);
	const int DataModelLevelID=GetMarkDataModelLevelID();
	ModelObj.SetModelDataModelLevelID(DataModelLevelID);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::CreateMarkSelfFieldPtr()//建立專屬Field指標	
{
	bool bIsOK = true;
	bIsOK = CAOIRgn::CreateRgnSelfFieldPtr();
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkFieldPtr(CAOIField *Ptr)
{
	CAOIRgn::SetRgnFieldPtr(Ptr);
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::GetMarkModelImageIsSaved() const//特徵點是否儲存過圖像
{
	return CAOIRgn::GetRgnModelImageIsSaved();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkModelImageIsSaved(bool val)//特徵點是否儲存過圖像
{
	CAOIRgn::SetRgnModelImageIsSaved(val);
}
//-------------------------------------------------------------------------------------//
int CAOIMark::GetMarkUniqueID() const
{
	return m_MarkUniqueID;
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkUniqueID(int value)
{
	m_MarkUniqueID = value;
}
//-------------------------------------------------------------------------------------//
int CAOIMark::GetMarkGroupID() const
{
	return m_MarkGroupID;
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkGroupID(int value)
{
	m_MarkGroupID = value;
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::CheckMarkGroupIDValid() const//確認特徵點群組編號有效
{
	if ( -1 == m_MarkGroupID ) { return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
MARK_TYPE_MODE CAOIMark::GetMarkTypeMode() const
{
	return m_MarkTypeMode;
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkTypeMode(MARK_TYPE_MODE value)
{
	m_MarkTypeMode = value;
}
//-------------------------------------------------------------------------------------//
LANE_ID CAOIMark::GetMarkLaneID() const
{
	return CAOIRgn::GetRgnLaneID();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkLaneID(LANE_ID value)
{
	CAOIRgn::SetRgnLaneID(value);
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkBypassed(bool value)
{
	CAOIRgn::SetRgnBypassed(value);
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::UpdateMarkBypassed()//確認特徵點是否為不檢測
{
	bool bBypassed = false;
	CAOIPanel *PanelPtr = NULL;
	CAOIBoard *BoardPtr = NULL;
	
	bBypassed = GetMarkBypassed();
	if ( false == bBypassed )
	{
		BoardPtr = GetMarkBoardPtr();
		if ( NULL != BoardPtr )
		{
			if ( BoardPtr->GetBoardBypassed() == true ) 
			{	bBypassed = true; }
		}
	}
	if ( false == bBypassed )
	{
		PanelPtr = GetMarkPanelPtr();
		if ( NULL != PanelPtr )
		{
			if ( PanelPtr->GetPanelBypassed() == true ) 
			{	bBypassed = true; }
		}
	}
	
	RESULT_ID  ModelResultID=RESULT_ID_NONE;
	CAOIModel *ModelPtr = GetMarkModelPtr();
	if ( NULL != ModelPtr )
	{
		ModelResultID = ModelPtr->GetModelResultID();		
		if ( true == bBypassed )
		{
			ModelPtr->SetModelResultID(RESULT_ID_BYPASS);
			ModelPtr->SetModelResultID_Alarm(RESULT_ID_BYPASS);
			ModelPtr->SetModelWndResultID(RESULT_ID_BYPASS);
		}
		else
		{
			if ( ModelResultID == RESULT_ID_BYPASS)
			{
				ModelPtr->SetModelResultID(RESULT_ID_NONE);
				ModelPtr->SetModelResultID_Alarm(RESULT_ID_NONE);
				ModelPtr->SetModelWndResultID(RESULT_ID_NONE);
			}
		}
	}	
	if ( true == bBypassed )
	{
		SetMarkNeedToCalculate(false);
		SetMarkResultID_AOI(RESULT_ID_BYPASS);
		SetMarkResultID_Alarm(RESULT_ID_BYPASS);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
int CAOIMark::CalcMarkOpenMPCountByPixels()
{
	return CalcRgnOpenMPCountByPixels();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkOpenMPCount(int value)
{
	SetRgnOpenMPCount(value); 
	CAOIModel *ModelPtr = GetMarkModelPtr();	
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelOpenMPCount(value); }
}
//-------------------------------------------------------------------------------------//
void CAOIMark::BypassSkipMark(RESULT_ID value)//不檢測或跳過特徵點
{
	CAOIMark *MarkPtr = this;
	LANE_ID LaneID = MarkPtr->GetMarkLaneID();
	MarkPtr->SetMarkNeedToCalculate(false);	
	MarkPtr->SetMarkNeedToCalculateBackup(false);
	MarkPtr->SetMarkResultID_AOI(value);
	MarkPtr->SetMarkResultID_Alarm(value);
	MarkPtr->UpdateMarkResultID_AOI_Lane(LaneID);

	CAOIModel *ModelPtr = MarkPtr->GetMarkModelPtr();
	ModelPtr->SetModelResultID(value);
	ModelPtr->SetModelResultID_Alarm(value);
	ModelPtr->BypassSkipModelWnd(value);
	return;
}
//-------------------------------------------------------------------------------------//
RESULT_ID  CAOIMark::GetMarkResultID_AOI() const//取得特徵點結果編號
{
	return CAOIRgn::GetRgnResultID_AOI();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkResultID_AOI(RESULT_ID value)//設定特徵點結果編號
{
	CAOIRgn::SetRgnResultID_AOI(value);
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIMark::GetMarkResultID_AOI_LA() const//取得特徵點結果編號-A軌
{
	return CAOIRgn::GetRgnResultID_AOI_LA();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkResultID_AOI_LA(RESULT_ID value)//設定特徵點結果編號-A軌
{
	CAOIRgn::SetRgnResultID_AOI_LA(value);
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIMark::GetMarkResultID_AOI_LB() const//取得特徵點結果編號-B軌
{
	return CAOIRgn::GetRgnResultID_AOI_LB();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkResultID_AOI_LB(RESULT_ID value)	//設定特徵點結果編號-B軌
{
	CAOIRgn::SetRgnResultID_AOI_LB(value);
}
//-------------------------------------------------------------------------------------//
void CAOIMark::UpdateMarkResultID_AOI_Lane(LANE_ID LaneID)	//更新特徵點結果編號-軌道
{
	RESULT_ID ResultID = GetMarkResultID_AOI();
	switch ( LaneID )
	{
	case LANE_ID_A:	SetMarkResultID_AOI_LA(ResultID);	break;
	case LANE_ID_B:	SetMarkResultID_AOI_LB(ResultID);	break;	
	}
	return;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIMark::GetMarkResultID_AOI_Lane(LANE_ID LaneID) const//取得特徵點結果編號-軌道	
{
	RESULT_ID ResultID;
	switch ( LaneID )
	{
	case LANE_ID_A:	ResultID = GetMarkResultID_AOI_LA();	break;
	case LANE_ID_B:	ResultID = GetMarkResultID_AOI_LB();	break;
	default:		ResultID = GetMarkResultID_AOI();	break;
	}
	return ResultID;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIMark::GetMarkResultID_ARS() const//取得特徵點結果編號
{
	return CAOIRgn::GetRgnResultID_ARS();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkResultID_ARS(RESULT_ID value)//設定特徵點結果編號	
{
	CAOIRgn::SetRgnResultID_ARS(value);
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIMark::GetMarkResultID_ARS_LA() const//取得特徵點結果編號-A軌
{
	return CAOIRgn::GetRgnResultID_ARS_LA();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkResultID_ARS_LA(RESULT_ID value)	//設定特徵點結果編號-A軌
{
	CAOIRgn::SetRgnResultID_ARS_LA(value);
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIMark::GetMarkResultID_ARS_LB() const//取得特徵點結果編號-B軌
{
	return CAOIRgn::GetRgnResultID_ARS_LB();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkResultID_ARS_LB(RESULT_ID value)//設定特徵點結果編號-B軌
{
	CAOIRgn::SetRgnResultID_ARS_LB(value);
}
//-------------------------------------------------------------------------------------//
void CAOIMark::UpdateMarkResultID_ARS_Lane(LANE_ID LaneID)//更新特徵點結果編號-軌道
{
	RESULT_ID ResultID = GetMarkResultID_ARS();
	switch ( LaneID )
	{
	case LANE_ID_A:	SetMarkResultID_ARS_LA(ResultID);	break;
	case LANE_ID_B:	SetMarkResultID_ARS_LB(ResultID);	break;	
	}
	return;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIMark::GetMarkResultID_ARS_Lane(LANE_ID LaneID) const//取得特徵點結果編號-軌道	
{
	RESULT_ID ResultID;
	switch ( LaneID )
	{
	case LANE_ID_A:	ResultID = GetMarkResultID_ARS_LA();	break;
	case LANE_ID_B:	ResultID = GetMarkResultID_ARS_LB();	break;
	default:		ResultID = GetMarkResultID_ARS();	break;
	}
	return ResultID;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIMark::GetMarkResultID_Alarm() const//取得特徵點結果編號-警報
{
	return CAOIRgn::GetRgnResultID_Alarm();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkResultID_Alarm(RESULT_ID value)//設定特徵點結果編號-警報
{
	CAOIRgn::SetRgnResultID_Alarm(value);
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIMark::GetMarkWndPtr()//取得特徵點檢測框指標
{
	CAOIModel *ModelPtr = GetMarkModelPtr();
	if ( NULL == ModelPtr ) { return NULL; }
	CAOIWnd   *WndPtr = ModelPtr->GetModelWndPtrByDefectID(WND_DEFECT_BASE_VALUE);
	return WndPtr;
}
//-------------------------------------------------------------------------------------//
WND_LOGIC_TYPE CAOIMark::GetMarkLogicType()//檢測框邏輯樣式	
{
	WND_LOGIC_TYPE WndLogicType=WND_LOGIC_NONE;	
	CAOIWnd   *WndPtr = GetMarkWndPtr();
	if ( NULL == WndPtr ) { return WndLogicType; }
	WndLogicType = WndPtr->GetWndLogicType();
	return WndLogicType;
}
//-------------------------------------------------------------------------------------//		
int CAOIMark::GetMarkLogicGroupID()//檢測框邏輯群組編號	
{
	int WndLogicGroupID=0;
	CAOIWnd   *WndPtr = GetMarkWndPtr();
	if ( NULL == WndPtr ) { return WndLogicGroupID; }
	WndLogicGroupID = WndPtr->GetWndLogicGroupID();
	return WndLogicGroupID;
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIMark::GetMarkRoiRgnCad() const//特徵點範圍-Cad
{
	TREGION4D Region;
	JetAPI::PointsToRegion(m_RgnRoiCadCornerPos, 4, Region);
	return Region;	
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIMark::GetMarkBodyRgnCad() const//特徵點範圍-Cad
{
	TREGION4D Region;
	JetAPI::PointsToRegion(m_RgnBodyCadCornerPos, 4, Region);
	return Region;
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIMark::GetMarkRoiRgnStage() const//特徵點範圍-Stage
{
	TREGION4D Region;
	JetAPI::PointsToRegion(m_RgnRoiStageCornerPos, 4, Region);
	return Region;
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIMark::GetMarkBodyRgnStage() const//特徵點範圍-Stage
{
	TREGION4D Region;
	JetAPI::PointsToRegion(m_RgnBodyStageCornerPos, 4, Region);
	return Region;
}
//-------------------------------------------------------------------------------------//
void CAOIMark::GetMarkRoiCadRegion(TREGION4D &Region)//取得特徵點在Cad的範圍
{
	Region.minX = Region.maxX = CAOIRgn::m_RgnRoiCadCornerPos[0].x;
	Region.minY = Region.maxY = CAOIRgn::m_RgnRoiCadCornerPos[0].y;

	if ( Region.minX > CAOIRgn::m_RgnRoiCadCornerPos[1].x ) { Region.minX = CAOIRgn::m_RgnRoiCadCornerPos[1].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiCadCornerPos[1].y ) { Region.minY = CAOIRgn::m_RgnRoiCadCornerPos[1].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiCadCornerPos[1].x ) { Region.maxX = CAOIRgn::m_RgnRoiCadCornerPos[1].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiCadCornerPos[1].y ) { Region.maxY = CAOIRgn::m_RgnRoiCadCornerPos[1].y; }

	if ( Region.minX > CAOIRgn::m_RgnRoiCadCornerPos[2].x ) { Region.minX = CAOIRgn::m_RgnRoiCadCornerPos[2].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiCadCornerPos[2].y ) { Region.minY = CAOIRgn::m_RgnRoiCadCornerPos[2].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiCadCornerPos[2].x ) { Region.maxX = CAOIRgn::m_RgnRoiCadCornerPos[2].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiCadCornerPos[2].y ) { Region.maxY = CAOIRgn::m_RgnRoiCadCornerPos[2].y; }

	if ( Region.minX > CAOIRgn::m_RgnRoiCadCornerPos[3].x ) { Region.minX = CAOIRgn::m_RgnRoiCadCornerPos[3].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiCadCornerPos[3].y ) { Region.minY = CAOIRgn::m_RgnRoiCadCornerPos[3].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiCadCornerPos[3].x ) { Region.maxX = CAOIRgn::m_RgnRoiCadCornerPos[3].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiCadCornerPos[3].y ) { Region.maxY = CAOIRgn::m_RgnRoiCadCornerPos[3].y; }
}
//-------------------------------------------------------------------------------------//
void CAOIMark::GetMarkBodyCadRegion(TREGION4D &Region)//取得特徵點在Cad的	
{
	Region.minX = Region.maxX = CAOIRgn::m_RgnBodyCadCornerPos[0].x;
	Region.minY = Region.maxY = CAOIRgn::m_RgnBodyCadCornerPos[0].y;

	if ( Region.minX > CAOIRgn::m_RgnBodyCadCornerPos[1].x ) { Region.minX = CAOIRgn::m_RgnBodyCadCornerPos[1].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyCadCornerPos[1].y ) { Region.minY = CAOIRgn::m_RgnBodyCadCornerPos[1].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyCadCornerPos[1].x ) { Region.maxX = CAOIRgn::m_RgnBodyCadCornerPos[1].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyCadCornerPos[1].y ) { Region.maxY = CAOIRgn::m_RgnBodyCadCornerPos[1].y; }

	if ( Region.minX > CAOIRgn::m_RgnBodyCadCornerPos[2].x ) { Region.minX = CAOIRgn::m_RgnBodyCadCornerPos[2].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyCadCornerPos[2].y ) { Region.minY = CAOIRgn::m_RgnBodyCadCornerPos[2].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyCadCornerPos[2].x ) { Region.maxX = CAOIRgn::m_RgnBodyCadCornerPos[2].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyCadCornerPos[2].y ) { Region.maxY = CAOIRgn::m_RgnBodyCadCornerPos[2].y; }

	if ( Region.minX > CAOIRgn::m_RgnBodyCadCornerPos[3].x ) { Region.minX = CAOIRgn::m_RgnBodyCadCornerPos[3].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyCadCornerPos[3].y ) { Region.minY = CAOIRgn::m_RgnBodyCadCornerPos[3].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyCadCornerPos[3].x ) { Region.maxX = CAOIRgn::m_RgnBodyCadCornerPos[3].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyCadCornerPos[3].y ) { Region.maxY = CAOIRgn::m_RgnBodyCadCornerPos[3].y; }
}
//-------------------------------------------------------------------------------------//
void CAOIMark::GetMarkRoiStageRegion(TREGION4D &Region)//取得特徵點在Stag的範圍		
{
	Region.minX = Region.maxX = CAOIRgn::m_RgnRoiStageCornerPos[0].x;
	Region.minY = Region.maxY = CAOIRgn::m_RgnRoiStageCornerPos[0].y;

	if ( Region.minX > CAOIRgn::m_RgnRoiStageCornerPos[1].x ) { Region.minX = CAOIRgn::m_RgnRoiStageCornerPos[1].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiStageCornerPos[1].y ) { Region.minY = CAOIRgn::m_RgnRoiStageCornerPos[1].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiStageCornerPos[1].x ) { Region.maxX = CAOIRgn::m_RgnRoiStageCornerPos[1].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiStageCornerPos[1].y ) { Region.maxY = CAOIRgn::m_RgnRoiStageCornerPos[1].y; }

	if ( Region.minX > CAOIRgn::m_RgnRoiStageCornerPos[2].x ) { Region.minX = CAOIRgn::m_RgnRoiStageCornerPos[2].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiStageCornerPos[2].y ) { Region.minY = CAOIRgn::m_RgnRoiStageCornerPos[2].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiStageCornerPos[2].x ) { Region.maxX = CAOIRgn::m_RgnRoiStageCornerPos[2].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiStageCornerPos[2].y ) { Region.maxY = CAOIRgn::m_RgnRoiStageCornerPos[2].y; }

	if ( Region.minX > CAOIRgn::m_RgnRoiStageCornerPos[3].x ) { Region.minX = CAOIRgn::m_RgnRoiStageCornerPos[3].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiStageCornerPos[3].y ) { Region.minY = CAOIRgn::m_RgnRoiStageCornerPos[3].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiStageCornerPos[3].x ) { Region.maxX = CAOIRgn::m_RgnRoiStageCornerPos[3].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiStageCornerPos[3].y ) { Region.maxY = CAOIRgn::m_RgnRoiStageCornerPos[3].y; }
}
//-------------------------------------------------------------------------------------//
void CAOIMark::GetMarkBodyStageRegion(TREGION4D &Region)//取得特徵點在Stage的範圍		
{
	Region.minX = Region.maxX = CAOIRgn::m_RgnBodyStageCornerPos[0].x;
	Region.minY = Region.maxY = CAOIRgn::m_RgnBodyStageCornerPos[0].y;

	if ( Region.minX > CAOIRgn::m_RgnBodyStageCornerPos[1].x ) { Region.minX = CAOIRgn::m_RgnBodyStageCornerPos[1].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyStageCornerPos[1].y ) { Region.minY = CAOIRgn::m_RgnBodyStageCornerPos[1].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyStageCornerPos[1].x ) { Region.maxX = CAOIRgn::m_RgnBodyStageCornerPos[1].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyStageCornerPos[1].y ) { Region.maxY = CAOIRgn::m_RgnBodyStageCornerPos[1].y; }

	if ( Region.minX > CAOIRgn::m_RgnBodyStageCornerPos[2].x ) { Region.minX = CAOIRgn::m_RgnBodyStageCornerPos[2].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyStageCornerPos[2].y ) { Region.minY = CAOIRgn::m_RgnBodyStageCornerPos[2].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyStageCornerPos[2].x ) { Region.maxX = CAOIRgn::m_RgnBodyStageCornerPos[2].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyStageCornerPos[2].y ) { Region.maxY = CAOIRgn::m_RgnBodyStageCornerPos[2].y; }

	if ( Region.minX > CAOIRgn::m_RgnBodyStageCornerPos[3].x ) { Region.minX = CAOIRgn::m_RgnBodyStageCornerPos[3].x; }
	if ( Region.minY > CAOIRgn::m_RgnBodyStageCornerPos[3].y ) { Region.minY = CAOIRgn::m_RgnBodyStageCornerPos[3].y; }
	if ( Region.maxX < CAOIRgn::m_RgnBodyStageCornerPos[3].x ) { Region.maxX = CAOIRgn::m_RgnBodyStageCornerPos[3].x; }
	if ( Region.maxY < CAOIRgn::m_RgnBodyStageCornerPos[3].y ) { Region.maxY = CAOIRgn::m_RgnBodyStageCornerPos[3].y; }
}
//-------------------------------------------------------------------------------------//
void CAOIMark::MoveMarkCadPos(double dX, double dY)//移動特徵點座標	
{
	CAOIRgn::MoveRgnCadPos(dX, dY);
	m_MarkModel.SetModelAttachedPosCad(m_RgnCadPos);
}
//-------------------------------------------------------------------------------------//
void CAOIMark::MoveMarkStagePos(double dX, double dY)//移動特徵點座標	
{
	CAOIRgn::MoveRgnStagePos(dX, dY);
	m_MarkModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);	
}
//-------------------------------------------------------------------------------------//
void CAOIMark::MoveMarkPos(double dX, double dY, CMapCoordinate *MapPtr)//移動特徵點座標	
{
	MoveMarkCadPos(dX, dY);	
	if ( NULL != MapPtr )
	{
		MapMarkCadToStagePos(*MapPtr);
		m_MarkModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);	
	}
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::CheckMarkBePickByCad(const TPOINT2D &PickPos)//確認特徵點被點擊到
{
	TREGION4D    Region = GetMarkRoiRgnCad();
	if ( PickPos.x<Region.minX || PickPos.y<Region.minY || 
		 PickPos.x>Region.maxX || PickPos.y>Region.maxY )
	{	return false; }
	return true; 
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::CheckMarkBePickByStage(const TPOINT2D &PickPos)//確認特徵點被點擊到
{
	TREGION4D    Region = GetMarkRoiRgnStage();		
	if ( PickPos.x<Region.minX || PickPos.y<Region.minY || 
		 PickPos.x>Region.maxX || PickPos.y>Region.maxY )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::CheckMarkInRegionByCad(const TREGION4D &SelRgn, bool bEntireIn)//確認特徵點在範圍內
{
	TREGION4D    Region = GetMarkRoiRgnCad();		
	return JetAPI::CheckRgnInRegion(Region, SelRgn, bEntireIn);
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::CheckMarkInRegionByStage(const TREGION4D &SelRgn, bool bEntireIn)//確認特徵點在範圍內
{
	TREGION4D    Region = GetMarkRoiRgnStage();		
	return JetAPI::CheckRgnInRegion(Region, SelRgn, bEntireIn);
}
//-------------------------------------------------------------------------------------//
void CAOIMark::CalcMarkCadCornerPos()//計算特徵點Cad端點座標
{
	CAOIRgn::CalcRgnCadCornerPos();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::LayoutMarkStageCornerPos()//更新特徵點機台端點座標
{
	CAOIRgn::LayoutRgnStageCornerPos();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkFrameImageSize_um(const TSIZE2D &value)
{
	CAOIRgn::SetRgnFrameImageSize_um(value);
	m_MarkModel.SetModelImageSize_um(value);
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkFrameImageCadOffset_um(const TPOINT2D &value)
{
	CAOIRgn::SetRgnFrameImageCadOffset_um(value);
	m_MarkModel.SetModelImageCadOffset_um(value);		
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkFrameImageStageOffset_um(const TPOINT2D &value)
{
	TPOINT2D CadOffset;
	AOIDataCollect.MapStageOffsetPtToCad(value, CadOffset);
	SetMarkFrameImageCadOffset_um(CadOffset);	
}
//-------------------------------------------------------------------------------------//
void CAOIMark::MapMarkCadToStagePos(const CMapCoordinate &Map)//將特徵點CAD轉成機台座標
{
	CAOIRgn::MapRgnCadToStagePos(Map);	
	const double SagePosX = CAOIRgn::GetRgnStagePosX();
	const double SagePosY = CAOIRgn::GetRgnStagePosY();
	m_MarkModel.SetModelAttachedPosStage(SagePosX, SagePosY);
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::SpinMark(double Angle, CMapCoordinate *MapPtr)//特徵點自旋轉
{
	double CadAngle = 0;
	double StageAngle = 0;	
	CadAngle = Angle;
	AOIDataCollect.MapCadAngleToStage(CadAngle, StageAngle);
	const double CadCpX = m_RgnCadPos.x;
	const double CadCpY = m_RgnCadPos.y;
	const double StageCpX = m_RgnStagePos.x;
	const double StageCpY = m_RgnStagePos.y;

	RotateMark(CadAngle, CadCpX, CadCpY, MapPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::RotateMark(double Angle, double CpX, double CpY, CMapCoordinate *MapPtr)//特徵點旋轉	
{
	CAOIRgn::RotateRgnCad(Angle, CpX, CpY);
	CAOIRgn::LayoutRgnStageCornerPos();
	
	CAOIModel &ModelObj = m_MarkModel;
	ModelObj.RotateModel(Angle, 0, 0);	
	ModelObj.SetModelAttachedPosCad(m_RgnCadPos);
	if ( NULL != MapPtr )
	{
		MapMarkCadToStagePos(*MapPtr);
		ModelObj.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::MirrorXMark(double CpX, CMapCoordinate *MapPtr)//特徵點鏡射-X
{
	CAOIRgn::MirrorXRgnCad(CpX);
	CAOIModel &ModelObj = m_MarkModel;
	ModelObj.MirrorModelYAxis(0);
	ModelObj.SetModelAttachedPosCad(m_RgnCadPos);	
	ModelObj.SetModelAttachedAngle(m_RgnAngle);
	if ( NULL != MapPtr )
	{
		MapMarkCadToStagePos(*MapPtr);
		ModelObj.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::MirrorYMark(double CpY, CMapCoordinate *MapPtr)//特徵點鏡射-Y	
{
	CAOIRgn::MirrorYRgnCad(CpY);
	CAOIModel &ModelObj = m_MarkModel;
	ModelObj.MirrorModelXAxis(0);
	ModelObj.SetModelAttachedPosCad(m_RgnCadPos);	
	ModelObj.SetModelAttachedAngle(m_RgnAngle);
	if ( NULL != MapPtr )
	{
		MapMarkCadToStagePos(*MapPtr);
		ModelObj.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIMark::GetMarkFullName() const//取得特徵點的名稱	
{
	unsigned int BarcodeIndex = 0;
	CAOIProject *ProjectPtr = GetMarkProjectPtr();
	if ( NULL == ProjectPtr )
	{	BarcodeIndex = m_RgnIdx;	}
	else
	{	BarcodeIndex = m_RgnIndex_Project;	}
	CString Name=AOIDataDefine.GetMarkFullName(BarcodeIndex, _T("Mark"));	
	return Name;
}
//-------------------------------------------------------------------------------------//
CString CAOIMark::GetRgnDerivedName() const//取得特徵點的名稱	
{
	return GetMarkFullName();
}
//-------------------------------------------------------------------------------------//
CString CAOIMark::GetRgnDerivedKeyName() const//取得特徵點的名稱	
{
	return CString(_T("Mark"));	
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::ExtractRgnDerivedFrame(bool &Finished)//挖取特徵點圖片
{
	if ( CAOIRgn::ExtractRgnFrame(Finished) == false ) { return false; }

	if ( true == Finished )
	{	
		bool IsOK = true;
		IsOK = ExecMarkInspection();
		ClearRgnMaskBuffer_Base();
		if ( GetMarkKeepImage() == false )
		{	CAOIRgn::ClearRgnImageBuffer();		}
		if ( false == IsOK )
		{	return false; }		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::ExecRgnDerivedInspection()//執行特徵點檢測
{
	return ExecMarkInspection();	
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::CreateMarkSubRgnList(FIELD_BUILD_MODE BuildMode, bool ByCadRegion)//建立特徵點子檢測區域列表
{
	switch ( BuildMode )
	{	
	case FIELD_BUILD_RANDOM_PANEL:
	case FIELD_BUILD_RANDOM_BOARD:
	case FIELD_BUILD_RANDOM_PROJECT:
		if ( CAOIRgn::CreateRgnSubList_RandomField() == false )
		{	return false; }
		break;
	default:
		if ( CAOIRgn::CreateRgnSubList_MatrixField(ByCadRegion) == false )
		{	return false; }		
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIMark::ClearMarkSubRgnList()//清除特徵點的子列表
{
	CAOIRgn::ClearRgnSubList();
}
//-------------------------------------------------------------------------------------//
size_t CAOIMark::GetMarkSubRgnCount() const//取得特徵點的子數量
{
	return CAOIRgn::m_RgnSubList.size();
}
//-------------------------------------------------------------------------------------//
CAOIRgn* CAOIMark::GetMarkSubRgnPtr(size_t index, bool check) const//取得特徵點的子指標
{
	if ( true == check )
	{
		const size_t Count = m_RgnSubList.size();
		if ( index >= Count )
		{	return NULL; }
	}
	return CAOIRgn::m_RgnSubList[index];
}
//-------------------------------------------------------------------------------------//
void CAOIMark::InitMarkInspection()//初始化特徵點檢測
{
	LANE_ID LaneID = GetMarkLaneID();

	ClearRgnImageBuffer();
	SetMarkKeepImage(false);		
	SetMarkFillImageTime(0.0);	
	SetMarkModelImageIsSaved(false);
	SetMarkResultID_AOI(RESULT_ID_NONE);
	SetMarkResultID_Alarm(RESULT_ID_NONE);			
	UpdateMarkResultID_AOI_Lane(LaneID);
	//條碼的3D處理需要降低
	CAOIModel &ObjModel = m_MarkModel;
	double PanelBasePlane=GetMarkPanelBasePlane();
	TNoiseFilterParam &NoiseFilterParam = GetRgnSpaceNoiseFilterParam();
	NoiseFilterParam.BasePlaneParam.InitialBasePlaneParam();	
	NoiseFilterParam.BasePlaneParam.PanelBasePlane = PanelBasePlane;
	/*
	NoiseFilterParam.DataVoidExpandEnabled = false;
	NoiseFilterParam.DataFirstFilterMode = DATA_NF_DISABLE;
	NoiseFilterParam.DataOverLowFTMode = DATA_NF_DISABLE;
	NoiseFilterParam.DataHeightFTMode = DATA_NF_DISABLE;
	NoiseFilterParam.DataVoidReContructed = false;
	NoiseFilterParam.DataFinalFilterMode = DATA_NF_DISABLE;
	NoiseFilterParam.DataFinalFilterMode2 = DATA_NF_DISABLE;
	NoiseFilterParam.BasePlaneParam.PanelBasePlane=PanelBasePlane;
	*/	
	ObjModel.SetModelPanelBasePlane(PanelBasePlane);
	ObjModel.SetModelSpaceBasePlaneParam(NoiseFilterParam.BasePlaneParam);
	ObjModel.SetModelSpaceNoiseFilterParam(NoiseFilterParam);

	ObjModel.InitModelInspection(false);	
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::ExecMarkInspection()//執行特徵點檢測
{
	std::vector<TUNI_FRAME> UniFrameList;
	if ( CAOIRgn::GetRgnUniFrameList(UniFrameList) == false ) { return false; }	
	CAOIModel *ModelPtr = GetMarkModelPtr();
	CAOIProject *ProjectPtr = GetMarkProjectPtr();
	const size_t UniFrameCount = UniFrameList.size();
	if ( 0 == UniFrameCount ) 
	{
		LANE_ID LaneID = GetMarkLaneID();
		RESULT_ID ResultID = RESULT_ID_EXCEPTION;
		SetMarkResultID_AOI(ResultID);
		SetMarkResultID_Alarm(ResultID);
		UpdateMarkResultID_AOI_Lane(LaneID);
		ModelPtr->SetModelResultID(ResultID);
		ModelPtr->SetModelResultID_Alarm(ResultID);		
		return true; 		
	}
	
	CString    str;
	CString    MarkName;
	CString    ModelName;	
	const unsigned int MarkIndex = GetMarkIndex_Project();	
	ModelName = ModelPtr->GetModelName();
	MarkName = AOIDataDefine.GetMarkFullName(MarkIndex, _T("Mark"));	

#ifdef _DEBUG
	BOOL bSave = FALSE;
	if ( bSave == TRUE )
	{
		size_t     i=0;		
		CString    AttachedName;
		TUNI_FRAME UniFrame;
		AttachedName = MarkName;
		for ( i=0; i<UniFrameCount; i++ )
		{
			UniFrame = UniFrameList[i];
			if ( NULL != UniFrame.ImagePtr )
			{
				str.Format(_T("%s\\%s[%d].PNG"), AOIDataCollect.GetAOITempDirectory(), AttachedName, i+1);
				ImageAPI.SavePNGImage(str, UniFrame.ImageW, UniFrame.ImageH, UniFrame.ImageStep, UniFrame.BitCount, UniFrame.ImagePtr, true); 
			}
			else if ( NULL!=UniFrame.MaskPtr && NULL!=UniFrame.SpacePtr )
			{
				RECT           RoiRect={0,0,0,0};
				IMAGE_SIZE     RoiBitCount = 8;
				unsigned char *pGray = NULL;
				RoiRect.left = 0; 
				RoiRect.top = 0;
				RoiRect.right = (int)(UniFrame.ImageW);
				RoiRect.bottom = (int)(UniFrame.ImageH);
				IMAGE_SIZE RoiStep = JetAPI::GetBMPImagePixelsPerLine(UniFrame.ImageW, RoiBitCount, 4);
				const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();

				str.Format(_T("%s\\%s[%d]_Mask.PNG"), AOIDataCollect.GetAOITempDirectory(), AttachedName, i+1);
				ImageAPI.SavePNGImage(str, UniFrame.ImageW, UniFrame.ImageH, UniFrame.ImageStep, UniFrame.BitCount, UniFrame.MaskPtr, true); 

				str.Format(_T("%s\\%s[%d].PNG"), AOIDataCollect.GetAOITempDirectory(), AttachedName, i+1);
				ImageAPI.SpaceGrayImageConvertToGray(UniFrame.ImageW, UniFrame.ImageH, UniFrame.ImageStep, UniFrame.SpacePtr, UniFrame.MaskPtr, RoiRect, RoiStep, pGray, SpaceRatio, true);
				ImageAPI.SavePNGImage(str, UniFrame.ImageW, UniFrame.ImageH, UniFrame.ImageStep, UniFrame.BitCount, pGray, true); 
				JetMemory.free_func(pGray);
			}				
		}
	}
#endif//_DEBUG	

	TREGION4D  rgnModel;
	TSIZE2D    ImageSizeUm;
	CAMERA_ID  CameraID = PRIMARY_CAMERA_ID;
	IMAGE_SIZE ImageW = UniFrameList[0].ImageW;
	IMAGE_SIZE ImageH = UniFrameList[0].ImageH;	
	const double MarkW = GetMarkRoiSizeW();
	const double MarkH = GetMarkRoiSizeH();	
	AOIDataCollect.MapImageSizeToReal(CameraID, ImageW, ImageH, ImageSizeUm);
	SetMarkFrameImageSize_um(ImageSizeUm);//修正正跨FOV的尺寸
	
	ModelPtr->GetModelTotalRegion(rgnModel);
	const double ModelW = rgnModel.GetWidth();
	const double ModelH = rgnModel.GetHeight();
	ModelPtr->ExecModelInspection(UniFrameList);	
	UpdateMarkResultID();
	ModelPtr->CalcModelImageRect_CustomerAI(ImageW, ImageH);
	ExecMarkSaveDefectImage(UniFrameList);

	MARK_TYPE_MODE MarkTypeMode=GetMarkTypeMode();
	if ( MARK_TASK_BASE_PLANE == MarkTypeMode )
	{
		if ( CheckMarkPanelBasePlaneParamValid() == false )
		{
			TBasePlaneParam &BasePlaneParamRef=GetMarkSpaceBasePlaneParam();	
			BasePlaneParamRef.GroundEquation=GetMarkGroundEquation_Online();
		}
		
		if ( GetMarkBypassed() == false )
		{
			int bb=0;
			LockMark();			
			if ( AnalysisMarkGroundPlaneParam() == false )
			{
				UnlockMark();
				return false;
			}
			UnlockMark();	
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::ExecMarkSaveDefectImage(const std::vector<TUNI_FRAME> &UniFrameList)//執行特徵點儲存瑕疵圖片
{
	bool IsOK = true;
	IsOK = CAOIRgn::ExecRgnSaveDefectImage(UniFrameList);
	if ( false == IsOK )
	{	return false; }

	CAOIProject *ProjectPtr = GetMarkProjectPtr();
	const bool bSaveModelImage = GetMarkModelImageIsSaved();
	if ( true==bSaveModelImage && NULL!=ProjectPtr )
	{			
		const int MarkSaveImage = false;//ProjectPtr->GetProjectParameter().m_BarcodeCameraSaveImageEnabled;
		if ( FN_ENABLE == MarkSaveImage )
		{
			/*
			unsigned int FrameIndex=0;
			CAOIWnd *WndPtr = GetMarkWndPtr();
			std::vector<TUNI_FRAME> UniFrameListTmp;
			CString  DerivedName=GetRgnDerivedName();
			double   SpaceRatio = ProjectPtr->GetProjectSpaceToGrayRatio();
			CString  MarkFolder = ProjectPtr->GetProjectOnlineBarcodeFolder();
			CString  InspectionDateTime = ProjectPtr->GetProjectInspectionDateTime();		
			const size_t UniFrameCount = UniFrameList.size();
			if ( UniFrameCount > 0 ) 
			{
				if ( NULL == WndPtr )
				{	UniFrameListTmp.push_back(UniFrameList[0]); }
				else
				{
					FrameIndex = WndPtr->GetWndAlgParam().GetAlgImageBinParam().GetBinaryFrameIndex();
					if ( FrameIndex < UniFrameCount )
					{	UniFrameListTmp.push_back(UniFrameList[FrameIndex]);		}
					else
					{	UniFrameListTmp.push_back(UniFrameList[0]); }
				}
				CString  Filename;		
				bool     bEnhance = false;
				bool     bSave3D = false;
				bool     bAppend = false;
				Filename.Format(_T("%s\\%s#%s.PNG"), MarkFolder, InspectionDateTime, DerivedName);
				ImageAPI.SaveUniFrameImage(Filename, UniFrameListTmp, true, bEnhance, bSave3D, bAppend, SpaceRatio);
			}
			*/
		}
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::UpdateMarkResultID()//更新檢測框檢測結果
{
	size_t     i=0, idx=0;	
	ALG_TYPE   AlgType;
	WND_DEFECT_ID  WndDefectID;	
	CAOIWnd   *WndPtr = NULL;	
	RESULT_ID  ResultID = RESULT_ID_NONE;	
	CAOIModel   *ModelPtr = &(m_MarkModel);	
	const LANE_ID LaneID = GetMarkLaneID();
	const size_t WndOrderCount = ModelPtr->GetModelWndOrderCount();

	for ( i=0; i<WndOrderCount; i++ )
	{
		idx = WndOrderCount-i-1;
		WndPtr = ModelPtr->GetModelWndOrderPtr(idx, false);
		if ( NULL == WndPtr ) { continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		if ( WND_DEFECT_BASE_VALUE != WndDefectID ) { continue; }

		CAlgParam &AlgParam = WndPtr->GetWndAlgParam();		
		AlgType = AlgParam.GetAlgType();		
		ResultID = AlgParam.GetAlgResultID();
		SetMarkResultID_AOI(ResultID);
		SetMarkResultID_Alarm(ResultID);
		UpdateMarkResultID_AOI_Lane(LaneID);
		break;
	}

	if ( RESULT_ID_NONE == ResultID )
	{	
		SetMarkResultID_AOI(RESULT_ID_SKIP);	
		SetMarkResultID_Alarm(RESULT_ID_SKIP);	
		UpdateMarkResultID_AOI_Lane(LaneID);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::AnalysisMarkGroundPlaneParam()
{	
	CAOIBoard *BoardPtr = GetMarkBoardPtr();
	if ( NULL == BoardPtr ) { return false; }
	const unsigned int BoardIndex=BoardPtr->GetBoardIndex_Project();
	size_t       i=0;
	RESULT_ID    ResultID;
	CAOIMark    *MarkPtr = NULL;	
	std::vector<CAOIMark*> MarkUsedList;
	const int    LocalBasePlaneID = GetMarkLocalBasePlaneID();
	const size_t MarkCount = BoardPtr->GetBoardMarkCount();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = BoardPtr->GetBoardMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkBypassed() == true ) { continue; }
		if ( MarkPtr->GetMarkLocalBasePlaneID() != LocalBasePlaneID ) { continue; }
		ResultID = MarkPtr->GetMarkResultID_AOI();
		if ( RESULT_ID_NONE == ResultID )
		{	return true; }		
		if ( RESULT_ID_BYPASS==ResultID )
		{	continue; }
		if ( MarkPtr->CheckMarkPanelBasePlaneParamValid() == false )
		{	continue; }
		MarkUsedList.push_back(MarkPtr);
	}

	size_t MarkUsedCount=MarkUsedList.size();
	if ( 0 == MarkUsedCount ) { return true; }	
	/*
	CSortObj SortObj;
	std::vector<CSortObj> SortList;
	SortObj.SetSortMode(SORT_BY_INT);
	for ( i=0; i<MarkUsedCount; i++ )
	{
		MarkPtr = MarkUsedList[i];
		if ( NULL == MarkPtr ) { continue; }
		SortObj.SetValueInt(MarkPtr->GetMarkUniqueID());
		SortObj.SetPtr(MarkPtr);
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());

	MarkUsedList.clear();
	for ( i=0; i<SortList.size(); i++ )
	{	MarkUsedList.push_back((CAOIMark*)(SortList[i].GetPtr()));	}
	*/
	MarkPtr = MarkUsedList[0];
	
	TREGION4D MarkRgn;	
	TREGION4D BoardRgn;	
	TREGION4D GroundRgn;
	RECT      MarkImageRgn;
	RECT      BoardImageRgn;
	const double ExpandW = 0;
	const double ExpandH = 0;
	CAMERA_ID CameraID = GetMarkCameraID();

	BoardPtr->GetBoardRgnCad(BoardRgn);
	BoardRgn.minX -= ExpandW;
	BoardRgn.minY -= ExpandH;
	BoardRgn.maxX += ExpandW;
	BoardRgn.maxY += ExpandH;
	const double BoardStartX=BoardRgn.minX;
	const double BoardStartY=BoardRgn.minY;
	const double BoardRegionW=BoardRgn.GetWidth();
	const double BoardRegionH=BoardRgn.GetHeight();
	
	const double ResX = AOIDataCollect.GetCameraResolutionX(CameraID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(CameraID);
	const int    BoardImageW = (int)((BoardRegionW/ResX)+0.5);
	const int    BoardImageH = (int)((BoardRegionH/ResY)+0.5);

	double x=0, y=0, z=0;
	const int Step=2;
	const bool bInvertY = true;		
	CJetGroundEquation BoardGround;
	BoardGround.InitGroundParam(GROUND_EQUATION_CURVE);

	for ( i=0; i<MarkUsedCount; i++ )
	{		
		MarkPtr = MarkUsedList[i];
		if ( NULL == MarkPtr ) { continue; }
		MarkPtr->GetMarkRoiCadRegion(MarkRgn);

		double MarkStartX=MarkRgn.minX;
		double MarkStartY=MarkRgn.minY;
		//局限制單板範圍內
		GroundRgn.minX = MAX(MarkRgn.minX, BoardRgn.minX);
		GroundRgn.minY = MAX(MarkRgn.minY, BoardRgn.minY);
		GroundRgn.maxX = MIN(MarkRgn.maxX, BoardRgn.maxX);
		GroundRgn.maxY = MIN(MarkRgn.maxY, BoardRgn.maxY);	

		MarkImageRgn.left   = (int)(((GroundRgn.minX-MarkStartX)/ResX)+0.5);
		MarkImageRgn.top    = (int)(((GroundRgn.minY-MarkStartY)/ResY)+0.5);
		MarkImageRgn.right  = (int)(((GroundRgn.maxX-MarkStartX)/ResX)+0.5);
		MarkImageRgn.bottom = (int)(((GroundRgn.maxY-MarkStartY)/ResY)+0.5);
		
		const TBasePlaneParam &BasePlaneParamRef=MarkPtr->GetMarkSpaceBasePlaneParam();
		const CJetGroundEquation &MarkGroundEquationRef=BasePlaneParamRef.GroundEquation;
		for ( int v=MarkImageRgn.top; v<MarkImageRgn.bottom; v+=Step )
		{
			for ( int u=MarkImageRgn.left; u<MarkImageRgn.right; u+=Step )
			{
				//Mark的影像座標
				x = u;
				y = v;
				z = MarkGroundEquationRef.CalcGroundValue(x, y);//由Mark基準面參數計算出高度值		
				
				//原來CAD座標
				x = (u*ResX)+MarkStartX;
				if ( false == bInvertY )
				{	y = (v*ResY)+MarkStartY; }
				else
				{	y = ((MarkImageRgn.bottom-v-1)*ResY)+MarkStartY; }

				//單板的影像座標
				x = (x-BoardStartX)/ResX;
				if ( false == bInvertY )
				{	y = (y-BoardStartY)/ResY; }
				else
				{	y = BoardImageH-((y-BoardStartY)/ResY); }

				BoardGround.AddGroundValue(x, y, z);
			}
		}			
	}
	BoardGround.CalcGroundParam();	

	bool bSaveParam=false;
	if ( true==bSaveParam )
	{
		CString filename;
		CString str;
		CString strOutput;
		CString strMark;
		CString strBoard;
		FILE *pfile = NULL;
		double nX=0, nY=0, nZ=0;
		BoardGround.GetGroundNormal(nX, nY, nZ);
		filename.Format(_T("%s\\MarkResult.TXT"), AOIDataCollect.GetAOITempDirectory());
		pfile = ::_tfopen(filename, _T("a+"));
		if ( NULL != pfile )
		{
			strBoard.Format(_T("Board_%05d, %02f, %02f, %02f, %02f, %6f, %6f, %6f"), 
				BoardPtr->GetBoardIndex_Panel()+1, 
				BoardRgn.minX, BoardRgn.minY, BoardRgn.maxX, BoardRgn.maxY,
				nX, nY, nZ);
			strOutput = strBoard;
			for ( i=0; i<MarkUsedCount; i++ )
			{		
				MarkPtr = MarkUsedList[i];			
				const TBasePlaneParam &BasePlaneParamRef=MarkPtr->GetMarkSpaceBasePlaneParam();
				const CJetGroundEquation &MarkGroundEquationRef=BasePlaneParamRef.GroundEquation;
				MarkGroundEquationRef.GetGroundNormal(nX, nY, nZ);

				str = strOutput;
				strMark.Format(_T("%.6f, %.6f, %.6f"), nX, nY, nZ);
				strOutput.Format(_T("%s, %s"), str, strMark);
			}
			::_ftprintf(pfile, _T("\n%s"), (LPCTSTR)strOutput);
			::fclose(pfile); pfile=NULL;
		}
	}

	bool bSaveSpace=false;
	if ( true == bSaveSpace )
	{
		CString filename;
		IMAGE_PTR ImagePtr=NULL;
		SPACE_PTR SpacePtr=NULL;
		IMAGE_SIZE BoardImageStep=JetAPI::GetBMPImagePixelsPerLine(BoardImageW, 4);		
		if ( ImageAPI.BuildSpaceDataByGroundEquation(BoardGround, BoardImageW, BoardImageH, BoardImageStep, SpacePtr) == true )
		{	
			filename.Format(_T("%s\\BoardPlane[%06d].Z3D"), AOIDataCollect.GetAOITempDirectory(), BoardIndex+1);
			ImageAPI.SaveSpaceGrayImage(filename, BoardImageW, BoardImageH, BoardImageStep, SpacePtr, true);
			JetMemory.free_func(SpacePtr);

			const size_t BufferSize=ImageAPI.CalcBufferSize(BoardImageStep, BoardImageH);
			if ( JetMemory.alloc_func(BufferSize, ImagePtr, "fnName", "ImagePtr") == true )
			{
				::memset(ImagePtr, 0xFF, sizeof(IMAGE_DATA)*BufferSize);
				filename.Format(_T("%s\\BoardPlane[%06d].PNG"), AOIDataCollect.GetAOITempDirectory(), BoardIndex+1);
				ImageAPI.SaveImage(filename, BoardImageW, BoardImageH, BoardImageStep, 8, ImagePtr, true);
				JetMemory.free_func(ImagePtr);
			}
		}
	}

	if ( BoardPtr->AssignBoardLocalPlaneParam(LocalBasePlaneID, bInvertY, BoardGround) == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkPanelBasePlane(double val)
{
	SetRgnPanelBasePlane(val);
	CAOIModel *ModelPtr = GetMarkModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelPanelBasePlane(val); }
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::CheckMarkPanelBasePlaneParamValid() const//確認空間基準面參數有效
{
	//如果離線編程下讀取不到3D資料, 就用線上檢測的參數值
	const TBasePlaneParam &BasePlaneParamRef=GetMarkSpaceBasePlaneParam();	
	const CJetGroundEquation &GroundEquationRef=BasePlaneParamRef.GroundEquation;	
	return GroundEquationRef.CheckGroundParamValid();	
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkSpaceBasePlaneParam(const TBasePlaneParam& Param)
{
	SetRgnSpaceBasePlaneParam(Param);
	CAOIModel *ModelPtr = GetMarkModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelSpaceBasePlaneParam(Param); }
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkSpaceNoiseFilterParam(const TNoiseFilterParam& Param)
{
	SetRgnSpaceNoiseFilterParam(Param); 
	CAOIModel *ModelPtr = GetMarkModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelSpaceNoiseFilterParam(Param); }
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkMaskEnable_Base(bool val)
{
	CAOIRgn::SetRgnMaskEnable_Base(val);

	CAOIModel *ModelPtr = GetMarkModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelMaskEnable_Base(val); }
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::GetMarkMaskEnable_Base() const
{
	return GetRgnMaskEnable_Base();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkMaskFrameIndex_Base(unsigned int val)//影像序號
{
	CAOIRgn::SetRgnMaskFrameIndex_Base(val);

	CAOIModel *ModelPtr = GetMarkModelPtr();
	ModelPtr->SetModelMaskFrameIndex_Base(val);
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIMark::GetMarkMaskFrameIndex_Base() const//影像序號
{
	return GetRgnMaskFrameIndex_Base();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkMaskFrameUniqueID_Base(unsigned int val)//影像唯一碼
{
	CAOIRgn::SetRgnMaskFrameUniqueID_Base(val);

	CAOIModel *ModelPtr = GetMarkModelPtr();
	ModelPtr->SetModelMaskFrameUniqueID_Base(val);
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIMark::GetMarkMaskFrameUniqueID_Base() const//影像唯一碼
{
	return GetRgnMaskFrameUniqueID_Base();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkMaskColorGroupLinkIndex(int val)//彩色過濾的連動編號	
{
	CAOIRgn::SetRgnMaskColorGroupLinkIndex(val);

	CAOIModel *ModelPtr = GetMarkModelPtr();
	ModelPtr->SetModelMaskColorGroupLinkIndex(val);
}
//-------------------------------------------------------------------------------------//
int CAOIMark::GetMarkMaskColorGroupLinkIndex() const//彩色過濾的連動編號	
{
	return GetRgnMaskColorGroupLinkIndex();
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::UpdateMarkColorGroupLinkIndex(const std::vector<CColorGroup> &ColorGroupList)//更新模組內的彩色過濾連動
{
	size_t ColorGroupIndex = 0;
	unsigned int FrameIndex = 0;
	unsigned int FrameUniqueID = 0;
	const size_t ColorGroupCount = ColorGroupList.size();

	//基板顏色
	ColorGroupIndex = GetMarkMaskColorGroupLinkIndex();
	if ( ColorGroupIndex>=0 && ColorGroupIndex<ColorGroupCount )
	{
		FrameIndex = ColorGroupList[ColorGroupIndex].GetColorGroupFrameIndex();
		FrameUniqueID = ColorGroupList[ColorGroupIndex].GetColorGroupFrameUniqueID();

		SetMarkMaskFrameIndex_Base(FrameIndex);
		SetMarkMaskFrameUniqueID_Base(FrameUniqueID);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::UpdateMarkFrameIndex(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList)
{
	unsigned int  FrameIndex=0;
	unsigned int  FrameUniqueID = 0;
	const size_t  FrameIndexMapSize = FrameIndexMapList.size();

	FrameUniqueID = GetMarkFrameUniqueID();
	if ( FrameUniqueID<0 || FrameUniqueID>=FrameIndexMapSize )
	{	
		FrameIndex = DefaultIndex;	
		FrameUniqueID = DefaultUniqueID;
	}
	else
	{	
		FrameIndex = (FrameIndexMapList[FrameUniqueID]);
		if ( -1 == FrameIndex )
		{ 
			FrameIndex = DefaultIndex;	
			FrameUniqueID = DefaultUniqueID;
		}
	}	
	SetMarkFrameIndex(FrameIndex);
	SetMarkFrameUniqueID(FrameUniqueID);

	FrameUniqueID = CAOIRgn::GetRgnMaskFrameUniqueID_Base();
	if ( FrameUniqueID<0 || FrameUniqueID>=FrameIndexMapSize )
	{	
		FrameIndex = DefaultIndex;	
		FrameUniqueID = DefaultUniqueID;
	}
	else
	{	
		FrameIndex = (FrameIndexMapList[FrameUniqueID]);
		if ( -1 == FrameIndex )
		{ 
			FrameIndex = DefaultIndex;	
			FrameUniqueID = DefaultUniqueID;
		}
	}	
	SetMarkMaskFrameIndex_Base(FrameIndex);
	SetMarkMaskFrameUniqueID_Base(FrameUniqueID);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIMark::GetMarkDataModelEnabled() const//取得特徵資料模型啟用
{
	return CAOIRgn::GetRgnDataModelEnabled();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkDataModelEnabled(bool val)//設定特徵資料模型啟用
{
	CAOIRgn::SetRgnDataModelEnabled(val);
	CAOIModel *ModelPtr = GetMarkModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelDataModelEnabled(val); }
}
//-------------------------------------------------------------------------------------//
int CAOIMark::GetMarkDataModelLevelID() const//取得特徵資料模型等級
{
	return CAOIRgn::GetRgnDataModelLevelID();
}
//-------------------------------------------------------------------------------------//
void CAOIMark::SetMarkDataModelLevelID(int val)//設定特徵資料模型等級
{
	CAOIRgn::SetRgnDataModelLevelID(val);
	CAOIModel *ModelPtr = GetMarkModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelDataModelLevelID(val); }
}
//-------------------------------------------------------------------------------------//