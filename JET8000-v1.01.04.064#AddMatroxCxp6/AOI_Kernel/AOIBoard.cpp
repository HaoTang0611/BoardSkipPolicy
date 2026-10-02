// AOIBoard.cpp: implementation of the CAOIBoard class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIBoard.h"
//-------------------------------------------------------------------------------------//
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
IMPLEMENT_DYNAMIC(CAOIBoard, CAOIObj)
//-------------------------------------------------------------------------------------//
CAOIBoard::CAOIBoard():CAOIObj(AOI_OBJ_BOARD)
{	
	PreInitBoard();
	InitialBoard();
}
//-------------------------------------------------------------------------------------//
CAOIBoard::CAOIBoard(const CAOIBoard &board):CAOIObj(board)
{
	PreInitBoard();
	CloneBoard(board);
}
//-------------------------------------------------------------------------------------//
CAOIBoard::~CAOIBoard()
{
	RemoveBoardAllObjects();
}
//-------------------------------------------------------------------------------------//
CAOIBoard& CAOIBoard::operator=(const CAOIBoard &board)
{
	if ( this == &board ) { return *this; }
	CAOIObj::operator=(board);
	CloneBoard(board);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CAOIBoard::PreInitBoard()
{	
	m_BoardBarcode.clear();
	m_BoardBarcodeBackup.clear();
}
//-------------------------------------------------------------------------------------//
inline void CAOIBoard::InitialBoard()
{
	//---------------------------------------------------------------------------------//	
	//CAOIRgn::InitialRgn();//CAOIRgn建構子會自動呼叫
	//---------------------------------------------------------------------------------//			
	//---------------------------------------------------------------------------------//
	m_BoardIndex_Project = -1;//單板在專案的引數編號
	m_BoardIndex_Panel = -1;//單板在整板的引數編號
	//---------------------------------------------------------------------------------//
	m_BoardProjectPtr = NULL;//單板的專案指標
	//---------------------------------------------------------------------------------//
	m_BoardPanelPtr = NULL;//單板的整板指標
	m_BoardPanelIndex_Project = -1;//單板的整板引數編號
	//---------------------------------------------------------------------------------//
	m_BoardPanelFieldPtr_DA = NULL;
	m_BoardPanelFieldPtr_DB = NULL;	
	//---------------------------------------------------------------------------------//
	m_BoardTempInt[0] = 0;//單板暫存整數
	m_BoardTempInt[1] = 0;//單板暫存整數
	m_BoardTempInt[2] = 0;//單板暫存整數
	m_BoardTempInt[3] = 0;//單板暫存整數
	//---------------------------------------------------------------------------------//
	m_BoardDeleted = false;//單板是否刪除
	m_BoardSelected = false;//單板是否選取到	
	m_BoardBypassed = false;//單板是否不檢測
	m_BoardRoatedAngle = 0.0;//單板旋轉角度
	m_BoardType = BOARD_TYPE_NORMAL;
	m_BoardMapRect = TRECT4D();
	m_BoardSideMode=BOARD_SIDE_TOP;//單板板面方向
	m_BoardOrientationMode = BOARD_ORIENTATION_030;//單板方向
	m_BoardModified = false;//單板變更過
	m_BoardBarcodeEnabled = true;//單板條碼啟用
	m_BoardMultiDistrictMode = false;//單板多段模式
	m_BoardXBoardCheckRatio = 100.0;//單板X板確認比例
	m_BoardResultID_AOI = RESULT_ID_NONE;//單板檢測結果
	m_BoardResultID_AOI_LA = RESULT_ID_NONE;//單板檢測結果-AOI-A軌
	m_BoardResultID_AOI_LB = RESULT_ID_NONE;//單板檢測結果-AOI-B軌
	m_BoardResultID_ARS = RESULT_ID_NONE;//單板檢測結果_ARS
	m_BoardResultID_ARS_LA = RESULT_ID_NONE;//單板檢測結果-ARS-A軌
	m_BoardResultID_ARS_LB = RESULT_ID_NONE;//單板檢測結果-ARS-B軌
	m_BoardResultID_Alarm = RESULT_ID_NONE;//單板檢測結果-警報
	m_BoardActDistrictID = DISTRICT_ID_A;//單板分段編號
	m_BoardBoardFdGrabMode = BOARD_FD_GRAB_INSPECTING;//單板定位點取像模式
	//---------------------------------------------------------------------------------//			
	m_BoardXBoardUnitCount = 0;//報廢件的單位數量
	m_BoardXBoardUnitTotalCount = 0;//報廢件的單位總數量
	//---------------------------------------------------------------------------------//			
	m_BoardBarcode = L"Barcode";
	m_BoardBarcodeBackup = L"Barcode";
	m_BoardIsGetBarcode = false;
	m_BoardBarcodeBelongMode = BARCODE_BELONG_NONE;
	//---------------------------------------------------------------------------------//	
	m_BoardBarcodeDeviceIndex = 0;
	m_BoardBarcodeDeviceCodeIndex = 0;
	//---------------------------------------------------------------------------------//	
	m_BoardRgnCad = TREGION4D();//單板範圍-Cad
	m_BoardRgnStage_DA = TREGION4D();//單板範圍-Stage	
	m_BoardRgnStage_DB = TREGION4D();//單板範圍-Stage
	//---------------------------------------------------------------------------------//
	m_BoardMapEnable = true;//單板的座標轉換啟用
	m_BoardMapCTS.Identity();//單板的座標轉換-Cad to Stage
	m_BoardMapSTC.Identity();//單板的座標轉換-Stage to Cad
	m_BoardMapCTS_DB.Identity();//單板的座標轉換-Cad to Stage
	m_BoardMapSTC_DB.Identity();//單板的座標轉換-Stage to Cad
	m_BoardCalcMapFinish = false;//計算整板的座標轉換完成
	//---------------------------------------------------------------------------------//
	m_BoardFdPtrList.clear();
	m_BoardMarkPtrList.clear();
	m_BoardBarcodePtrList.clear();
	m_BoardFieldPtrList.clear();
	m_BoardComponentPtrList.clear();
	//---------------------------------------------------------------------------------//
	m_BoardFieldPtrListTemp.clear();
	m_BoardComponentPtrListTemp.clear();
	//---------------------------------------------------------------------------------//
	m_BoardPartGroupPtrList.clear();
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
inline void CAOIBoard::CloneBoard(const CAOIBoard &board)
{
	//---------------------------------------------------------------------------------//			
	m_BoardProjectPtr = board.m_BoardProjectPtr;//單板的專案指標
	//---------------------------------------------------------------------------------//
	m_BoardPanelPtr = board.m_BoardPanelPtr;//單板的整板指標
	m_BoardPanelIndex_Project = board.m_BoardPanelIndex_Project;//單板的整板引數編號
	//---------------------------------------------------------------------------------//
	m_BoardPanelFieldPtr_DA = board.m_BoardPanelFieldPtr_DA;
	m_BoardPanelFieldPtr_DB = board.m_BoardPanelFieldPtr_DB;	
	//---------------------------------------------------------------------------------//
	m_BoardErrorString = board.m_BoardErrorString;
	//---------------------------------------------------------------------------------//
	m_BoardIndex_Project = board.m_BoardIndex_Project;//單板在專案的引數編號
	m_BoardIndex_Panel = board.m_BoardIndex_Panel;//單板在整板的引數編號
	//---------------------------------------------------------------------------------//
	m_BoardTempInt[0] = board.m_BoardTempInt[0];//單板暫存整數
	m_BoardTempInt[1] = board.m_BoardTempInt[1];//單板暫存整數
	m_BoardTempInt[2] = board.m_BoardTempInt[2];//單板暫存整數
	m_BoardTempInt[3] = board.m_BoardTempInt[3];//單板暫存整數
	//---------------------------------------------------------------------------------//
	m_BoardDeleted = board.m_BoardDeleted;//單板是否刪除
	m_BoardSelected = board.m_BoardSelected;//單板是否選取到	
	m_BoardBypassed = board.m_BoardBypassed;//單板是否不檢測
	m_BoardRoatedAngle = board.m_BoardRoatedAngle;//單板旋轉角度	
	m_BoardType = board.m_BoardType;
	m_BoardMapRect = board.m_BoardMapRect;
	
	m_BoardSideMode = board.m_BoardSideMode;//單板板面方向
	m_BoardOrientationMode = board.m_BoardOrientationMode;//單板方向
	m_BoardModified = board.m_BoardModified;//單板變更過	
	m_BoardBarcodeEnabled = board.m_BoardBarcodeEnabled;//單板條碼啟用	
	m_BoardMultiDistrictMode = board.m_BoardMultiDistrictMode;//單板多段模式	
	m_BoardXBoardCheckRatio = board.m_BoardXBoardCheckRatio;//單板X板確認比例		
	m_BoardResultID_AOI = board.m_BoardResultID_AOI;//單板檢測結果
	m_BoardResultID_AOI_LA = board.m_BoardResultID_AOI_LA;//單板檢測結果-AOI-A軌
	m_BoardResultID_AOI_LB = board.m_BoardResultID_AOI_LB;//單板檢測結果-AOI-B軌	
	m_BoardResultID_ARS = board.m_BoardResultID_ARS;//單板檢測結果_ARS
	m_BoardResultID_ARS_LA = board.m_BoardResultID_ARS_LA;//單板檢測結果_ARS-A軌
	m_BoardResultID_ARS_LB = board.m_BoardResultID_ARS_LB;//單板檢測結果_ARS-B軌	
	m_BoardResultID_Alarm = board.m_BoardResultID_Alarm;//單板檢測結果-警報	
	m_BoardActDistrictID = board.m_BoardActDistrictID;//單板分段編號
	m_BoardBoardFdGrabMode = board.m_BoardBoardFdGrabMode;//單板定位點取像模式
	//---------------------------------------------------------------------------------//	
	m_BoardXBoardUnitCount = board.m_BoardXBoardUnitCount;//報廢件的單位數量
	m_BoardXBoardUnitTotalCount = board.m_BoardXBoardUnitTotalCount;//報廢件的單位總數量	
	//---------------------------------------------------------------------------------//	
	m_BoardBarcode = board.m_BoardBarcode;
	m_BoardBarcodeBackup = board.m_BoardBarcodeBackup;	
	m_BoardIsGetBarcode = board.m_BoardIsGetBarcode;
	m_BoardBarcodeBelongMode = board.m_BoardBarcodeBelongMode;
	//---------------------------------------------------------------------------------//
	m_BoardBarcodeDeviceIndex = board.m_BoardBarcodeDeviceIndex;
	m_BoardBarcodeDeviceCodeIndex = board.m_BoardBarcodeDeviceCodeIndex;
	//---------------------------------------------------------------------------------//	
	m_BoardRgnCad = board.m_BoardRgnCad;//單板範圍-Cad
	m_BoardRgnStage_DA = board.m_BoardRgnStage_DA;//單板範圍-Stage	
	m_BoardRgnStage_DB = board.m_BoardRgnStage_DB;//單板範圍-Stage
	//---------------------------------------------------------------------------------//	
	m_BoardMapEnable = board.m_BoardMapEnable;//單板的座標轉換啟用
	m_BoardMapCTS = board.m_BoardMapCTS;//單板的座標轉換-Cad to Stage
	m_BoardMapSTC = board.m_BoardMapSTC;//單板的座標轉換-Stage to Cad
	m_BoardMapCTS_DB = board.m_BoardMapCTS_DB;//單板的座標轉換-Cad to Stage
	m_BoardMapSTC_DB = board.m_BoardMapSTC_DB;//單板的座標轉換-Stage to Cad
	m_BoardCalcMapFinish = board.m_BoardCalcMapFinish;//計算整板的座標轉換完成
	//---------------------------------------------------------------------------------//
	m_BoardFdPtrList = board.m_BoardFdPtrList;
	m_BoardMarkPtrList = board.m_BoardMarkPtrList;
	m_BoardBarcodePtrList = board.m_BoardBarcodePtrList;
	m_BoardFieldPtrList = board.m_BoardFieldPtrList;
	m_BoardComponentPtrList = board.m_BoardComponentPtrList;	
	//---------------------------------------------------------------------------------//	
	m_BoardFieldPtrListTemp = board.m_BoardFieldPtrListTemp;	
	m_BoardComponentPtrListTemp = board.m_BoardComponentPtrListTemp;
	//---------------------------------------------------------------------------------//	
	m_BoardPartGroupPtrList = board.m_BoardPartGroupPtrList;
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
CAOIBoard* CAOIBoard::CloneBoardObj() const
{
	CAOIBoard *ObjPtr = AOIObjManager.CreateBoardObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIBoard::GetBoardFdCount_Inline() const//取得單板的定位點數量
{
	return m_BoardFdPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIBoard::AddBoardFdPtr_Inline(CAOIFd *FdPtr)//增加單板的定位點
{
	m_BoardFdPtrList.push_back(FdPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIFd* CAOIBoard::GetBoardFdPtr_Inline(size_t index)  const//取得單板的定位點指標
{	
	return m_BoardFdPtrList[index];
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIBoard::GetBoardMarkCount_Inline() const//取得單板的特徵點數量
{
	return m_BoardMarkPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIBoard::AddBoardMarkPtr_Inline(CAOIMark *MarkPtr)//增加單板的特徵點
{
	m_BoardMarkPtrList.push_back(MarkPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIMark* CAOIBoard::GetBoardMarkPtr_Inline(size_t index) const//取得單板的特徵點指標	
{
	return m_BoardMarkPtrList[index];
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIBoard::GetBoardBarcodeCount_Inline() const//取得單板的軟體條碼數量
{
	return m_BoardBarcodePtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIBoard::AddBoardBarcodePtr_Inline(CAOIBarcode *BarcodePtr)//增加單板的軟體條碼
{
	m_BoardBarcodePtrList.push_back(BarcodePtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIBarcode* CAOIBoard::GetBoardBarcodePtr_Inline(size_t index) const//取得單板的軟體條碼指標	
{	
	return m_BoardBarcodePtrList[index];
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIBoard::GetBoardFieldCount_Inline() const//取得單板的區域數量
{
	return m_BoardFieldPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIBoard::AddBoardFieldPtr_Inline(CAOIField *FieldPtr)//增加單板的區域
{
	m_BoardFieldPtrList.push_back(FieldPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIField*  CAOIBoard::GetBoardFieldPtr_Inline(size_t index) const//取得單板的區域指標	
{
	return m_BoardFieldPtrList[index];
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIBoard::GetBoardComponentCount_Inline() const//取得單板的零件數量
{
	return m_BoardComponentPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIBoard::AddBoardComponentPtr_Inline(CAOIComponent *ComponentPtr)//增加單板的零件
{
	m_BoardComponentPtrList.push_back(ComponentPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIComponent* CAOIBoard::GetBoardComponentPtr_Inline(size_t index) const//取得單板的零件指標	
{	
	return m_BoardComponentPtrList[index];
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIBoard::GetBoardPartGroupCount_Inline() const//取得單板的零件群組數量
{
	return m_BoardPartGroupPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIBoard::AddBoardPartGroupPtr_Inline(CAOIPartGroup *PartGroupPtr)//增加單板的零件群組
{
	m_BoardPartGroupPtrList.push_back(PartGroupPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIPartGroup* CAOIBoard::GetBoardPartGroupPtr_Inline(size_t index) const//取得單板的零件群組指標	
{
	return m_BoardPartGroupPtrList[index];
}
//-------------------------------------------------------------------------------------//
DISTRICT_ID CAOIBoard::GetBoardActDistrictID() const
{
	return m_BoardActDistrictID; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardActDistrictID(DISTRICT_ID value)
{ 
	m_BoardActDistrictID = value; 
}
//-------------------------------------------------------------------------------------//
BOARD_FD_GRAB_MODE CAOIBoard::GetBoardBoardFdGrabMode() const
{ 
	return m_BoardBoardFdGrabMode; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardBoardFdGrabMode(BOARD_FD_GRAB_MODE value)
{
	m_BoardBoardFdGrabMode = value; 
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIBoard::GetBoardRgnCad() const//單板範圍-Cad	
{
	return m_BoardRgnCad;	
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::GetBoardRgnCad(TREGION4D &Rgn) const//單板範圍-Cad	
{
	Rgn = m_BoardRgnCad;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardRgnCad(const TREGION4D &Rgn)//單板範圍-Cad	
{
	m_BoardRgnCad = Rgn;	
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIBoard::GetBoardRgnStage(DISTRICT_ID DistrictID) const//單板範圍-Stage
{
	TREGION4D Rgn;
	switch ( DistrictID )
	{
	case DISTRICT_ID_A:	Rgn = m_BoardRgnStage_DA;	break;
	case DISTRICT_ID_B:	Rgn = m_BoardRgnStage_DB;	break;
	}
	return Rgn;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::GetBoardRgnStage(DISTRICT_ID DistrictID, TREGION4D &Rgn) const//單板範圍-Stage
{
	switch ( DistrictID )
	{
	case DISTRICT_ID_A:	Rgn = m_BoardRgnStage_DA;	break;
	case DISTRICT_ID_B:	Rgn = m_BoardRgnStage_DB;	break;
	}
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardRgnStage(DISTRICT_ID DistrictID, const TREGION4D &Rgn)//單板範圍-Stage
{
	switch ( DistrictID )
	{
	case DISTRICT_ID_A:	m_BoardRgnStage_DA = Rgn;	break;
	case DISTRICT_ID_B:	m_BoardRgnStage_DB = Rgn;	break;
	}
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CheckBoardBePickByCad(const TPOINT2D &PickPos)//確認單板被點擊到
{
	TREGION4D    Region = GetBoardRgnCad();
	if ( PickPos.x<Region.minX || PickPos.y<Region.minY || 
		 PickPos.x>Region.maxX || PickPos.y>Region.maxY )
	{	return false; }
	return true; 
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CheckBoardBePickByStage(DISTRICT_ID DistrictID, const TPOINT2D &PickPos)//確認單板被點擊到
{
	TREGION4D    Region = GetBoardRgnStage(DistrictID);		
	if ( PickPos.x<Region.minX || PickPos.y<Region.minY || 
		 PickPos.x>Region.maxX || PickPos.y>Region.maxY )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CheckBoardInRegionByCad(const TREGION4D &SelRgn, bool bEntireIn)//確認單板在範圍內
{
	TREGION4D    Region = GetBoardRgnCad();		
	return JetAPI::CheckRgnInRegion(Region, SelRgn, bEntireIn);
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CheckBoardInRegionByStage(DISTRICT_ID DistrictID, const TREGION4D &SelRgn, bool bEntireIn)//確認單板在範圍內
{
	TREGION4D    Region = GetBoardRgnStage(DistrictID);		
	return JetAPI::CheckRgnInRegion(Region, SelRgn, bEntireIn);	
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardBarcode(const char *barcode)
{
	if ( NULL == barcode ) { return; }
	JetAPI::char2wstring(barcode, m_BoardBarcode);
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardBarcode(const wchar_t *barcode)//設定單板條碼
{
	if ( NULL == barcode ) { return; }	
	m_BoardBarcode = barcode;
}
//-------------------------------------------------------------------------------------//
const wchar_t* CAOIBoard::GetBoardBarcode() const
{
	return m_BoardBarcode.c_str();
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::BackupBoardBarcode()//備份單板條碼
{
	m_BoardBarcodeBackup = m_BoardBarcode;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::RestoreBoardBarcode()//恢復單板條碼
{
	m_BoardBarcode = m_BoardBarcodeBackup;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardIsGetBarcode(bool value)
{ 
	m_BoardIsGetBarcode = value; 
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::GetBoardIsGetBarcode() const 
{ 
	return m_BoardIsGetBarcode; 
}	
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardBarcodeDeviceIndex(unsigned int value)
{ 
	m_BoardBarcodeDeviceIndex = value; 
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIBoard::GetBoardBarcodeDeviceIndex() const 
{ 
	return m_BoardBarcodeDeviceIndex; 
}
//-------------------------------------------------------------------------------------//	
void CAOIBoard::SetBoardBarcodeDeviceCodeIndex(unsigned int value) 
{ 
	m_BoardBarcodeDeviceCodeIndex = value; 
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIBoard::GetBoardBarcodeDeviceCodeIndex() const 
{ 
	return m_BoardBarcodeDeviceCodeIndex; 
}
//-------------------------------------------------------------------------------------//
BARCODE_BELONG_MODE CAOIBoard::GetBoardBarcodeBelongMode() const//單板條碼屬於模式
{
	return m_BoardBarcodeBelongMode;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardBarcodeBelongMode(BARCODE_BELONG_MODE value)//單板條碼屬於模式
{
	m_BoardBarcodeBelongMode = value;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::WriteBoardFile(CAOIFileIO &FileIO)//儲存單板檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	CAOIBoard *pBoard = this;
	char       uuidStr[MAX_JET_PATH]="";	
	wchar_t    uuidWStr[MAX_JET_PATH]=L"";	
	UUID     uuid = pBoard->GetObjUuid();	
	FileIO.SetFnName(_T("CAOIBoard::WriteBoardFile"));
	JetAPI::UUIDToStringA(uuid, uuidStr);
	JetAPI::UUIDToStringW(uuid, uuidWStr);
	//----------------------------------------------------------------------------------------//
	//單板參數
	if ( FileIO.SaveChunk_INT( FILE_IO_BOARD_START, 0) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_BOARD_INDEX_PROJECT, pBoard->GetBoardIndex_Project()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BOARD_PANEL_INDEX, pBoard->GetBoardPanelIndex_Project()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_BOARD_BYPASS, pBoard->GetBoardBypassed()) == false ) { return false; }	
	if ( FileIO.SaveChunk_BOL(FILE_IO_BOARD_MAP_ENABLE, pBoard->GetBoardMapEnable()) == false ) { return false; }			
	if ( FileIO.GetSaveWStr() == true ) 
	{	if ( FileIO.SaveChunk_STR(FILE_IO_BOARD_OBJ_UUID, uuidWStr) == false ) { return false; } }
	else
	{	if ( FileIO.SaveChunk_STR(FILE_IO_BOARD_OBJ_UUID, uuidStr) == false ) { return false; } }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BOARD_ROTATED_ANGLE, pBoard->GetBoardRoatedAngle()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_BOARD_ORIENTATION, pBoard->GetBoardOrientationMode()) == false ) { return false; }	                    
	if ( FileIO.SaveChunk_INT(FILE_IO_BOARD_BARCODE_DEVICDE_INDEX, pBoard->GetBoardBarcodeDeviceIndex()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BOARD_BARCODE_DEVICDE_CODE_INDEX, pBoard->GetBoardBarcodeDeviceCodeIndex()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_BOARD_SIDE_MODE, pBoard->GetBoardSideMode()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_BOARD_TYPE, pBoard->GetBoardType()) == false ) { return false; }		
	if ( FileIO.SaveChunk_BOL(FILE_IO_BOARD_BARCODE_ENABLED, pBoard->GetBoardBarcodeEnabled()) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_BOARD_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::ReadBoardFile(CAOIFileIO &FileIO)//載入單板檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	UUID       uuid;
	int        index = 0;
	CAOIBoard *pBoard = this;
	FileIO.SetFnName(_T("CAOIBoard::ReadBoardFile"));

	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_BOARD_START://單板參數-起點
			break;
		case FILE_IO_BOARD_INDEX_PROJECT:
			pBoard->SetBoardIndex_Project(FileIO.GetData_INT());
			break;
		case FILE_IO_BOARD_PANEL_INDEX:
			pBoard->SetBoardPanelIndex_Project(FileIO.GetData_INT());
			break;
		case FILE_IO_BOARD_BYPASS:
			pBoard->SetBoardBypassed(FileIO.GetData_BOL());
			break;
		case FILE_IO_BOARD_MAP_ENABLE:			
			pBoard->SetBoardMapEnable(FileIO.GetData_BOL());			
			break;
		case FILE_IO_BOARD_OBJ_UUID://單板參數-OBJ-UUID
			if ( FileIO.GetLoadWStr()==true )
			{
				if ( JetAPI::UUIDFromStringW(FileIO.GetData_WSTR(), uuid) == true )
				{	pBoard->SetObjUuid(uuid);	}
			}
			else
			{
				if ( JetAPI::UUIDFromStringA(FileIO.GetData_STR(), uuid) == true )
				{	pBoard->SetObjUuid(uuid);	}
			}
			break;
		case FILE_IO_BOARD_ROTATED_ANGLE://單板參數-旋轉角度
			pBoard->SetBoardRoatedAngle(FileIO.GetData_DBL());
			break;
		case FILE_IO_BOARD_ORIENTATION://單板參數-方向角度-此定義方能解決鏡射問題
			pBoard->SetBoardOrientationMode((BOARD_ORIENTATION_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_BOARD_BARCODE_DEVICDE_INDEX://單板參數-條碼機條碼引數
			pBoard->SetBoardBarcodeDeviceIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_BOARD_BARCODE_DEVICDE_CODE_INDEX://單板參數-條碼機條碼引數
			pBoard->SetBoardBarcodeDeviceCodeIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_BOARD_SIDE_MODE://單板參數-正背面
			pBoard->SetBoardSideMode((BOARD_SIDE_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_BOARD_TYPE://單板參數-樣式
			pBoard->SetBoardType((BOARD_TYPE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_BOARD_BARCODE_ENABLED://單板參數-條碼啟用
			pBoard->SetBoardBarcodeEnabled(FileIO.GetData_BOL());
			break;
		case FILE_IO_BOARD_END://單板參數-終點
			return true;
			break;
		default:
			break;
		}
	};			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::WriteBoardSpcFile_JSON_VRS(FILE *pfile, CAOIProject *ProjectPtr)
{
	if ( NULL == pfile ) { return false; }
	if ( NULL == ProjectPtr ) { return false; }

	CAOIBoard *BoardPtr = this;
	if ( NULL == BoardPtr ) { return false; }	

	size_t           k=0;
	TREGION4D        MapRgn;	
	TREGION4D        Region;
	CString          strText;	
	const size_t     szBuffer = 256;
	int              InternalBarcode=0;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";		
	CAOIComponent   *ComponentPtr = NULL;
	size_t BoardComponentCount = BoardPtr->GetBoardComponentCount();
	size_t ComponentNotAgentCount = BoardPtr->GetBoardComponentNotAgentCount();

	LANE_ID    LaneID = ProjectPtr->GetProjectActLaneID();	
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();	

	//"Side" : 0,			// 板子正反面(整數-1 Byte)( 0:正 1:反 )
	::wcscpy(strTag, L"Side");			
	::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, BoardPtr->GetBoardSideMode());

	::wcscpy(strTag, L"Barcode");	
	strText = BoardPtr->GetBoardBarcode();
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	JetAPI::FilterJSONStringW(strBuffer);
	::fwprintf(pfile, L"      \"%s\": \"%s\",\n", strTag, strBuffer);

	::wcscpy(strTag, L"Internal_Barcode");
	if ( BoardPtr->GetBoardIsGetBarcode() == false ) { InternalBarcode = 1; }
	else { InternalBarcode = 0; }	
	::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, InternalBarcode);

	// 在底圖上的位置(LTx LTy RTx RTy LBx LBy RBx RBy)(pixel)
	Region = BoardPtr->GetBoardRgnStage(DistrictID);			
	ProjectPtr->MapProjectStageToMapPos(Region.minX, Region.minY, MapRgn.minX, MapRgn.minY, LaneID, DistrictID);
	ProjectPtr->MapProjectStageToMapPos(Region.maxX, Region.maxY, MapRgn.maxX, MapRgn.maxY, LaneID, DistrictID);
	Region.minX = MIN(MapRgn.minX, MapRgn.maxX);
	Region.minY = MIN(MapRgn.minY, MapRgn.maxY);
	Region.maxX = MAX(MapRgn.minX, MapRgn.maxX);
	Region.maxY = MAX(MapRgn.minY, MapRgn.maxY);		
	::wcscpy(strTag, L"Map_Location");//"Location" : [100,100,300,200,100,100,300,200],	// 在底圖上的位置(LTx LTy RTx RTy LBx LBy RBx RBy)(pixel)			
	//::fwprintf(pfile, L"      \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, Region.minX, Region.minY, Region.maxX, Region.minY, Region.minX, Region.maxY, Region.maxX, Region.maxY);
	::fwprintf(pfile, L"      \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, Region.minX, Region.minY, Region.minX, Region.maxY, Region.maxX, Region.maxY, Region.maxX, Region.minY);//20190919
			
	::wcscpy(strTag, L"Type");			
	::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, BoardPtr->GetBoardType());//1:normal

	//0:No Test  1:Skip 2:Bypass 3:Sys-Ok 4:Sys-Ng 5:Op-Ok 6:Op-Ng 7:Fd-Ng 
	RESULT_ID BoardResultID = BoardPtr->GetBoardResultID();			
	SPC_RESULT_ID SpcResultID = AOIDataCollect.MapResultIDToSpcResultID(BoardResultID);
	::wcscpy(strTag, L"Test_Result");			
	::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, SpcResultID);				
			
	::wcscpy(strTag, L"N_Parts");			
	::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, ComponentNotAgentCount);

	size_t BoardComponentCountNG = BoardPtr->GetBoardComponentNGCount();
	::wcscpy(strTag, L"N_NgParts");			
	::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, BoardComponentCountNG);

	for ( k=0; k<BoardComponentCount; k++ )
	{
		ComponentPtr = BoardPtr->GetBoardComponentPtr(k, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }

		::fwprintf(pfile, L"      \"Part_%d\":\n", k+1);
		::fwprintf(pfile, L"      {\n");

		if ( ComponentPtr->WriteComponentSpcFile_JSON_VRS(pfile, ProjectPtr) == false )
		{	return false; }
		
		::fwprintf(pfile, L"      },\n");
	}

	//Part Barcode List
	std::vector<TPartBarcode> PartBarcodeList;
	BoardPtr->BuildBoardPartBarcodeList(PartBarcodeList);
	const size_t PartBarcodeCount=PartBarcodeList.size();	
	::wcscpy(strTag, L"N_SN");			
	::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, PartBarcodeCount);

	::wcscpy(strTag, L"SN_List");//SN_List
	::fwprintf(pfile, L"      \"%s\": %s\n", strTag, L"{");//SN_List {
	for ( k=0; k<PartBarcodeCount; k++ )
	{
		JetAPI::FilterJSONStringW(PartBarcodeList[k].wsCode);
		//::fwprintf(pfile, L"      [\"%s\",\"%s\"]", PartBarcodeList[k].wsName.c_str(), PartBarcodeList[k].wsCode.c_str());		
		::fwprintf(pfile, L"      \"%d\":[\"%s\",\"%s\"]", k, PartBarcodeList[k].wsName.c_str(), PartBarcodeList[k].wsCode.c_str());
		if ( k < (PartBarcodeCount-1) )
		{	::fwprintf(pfile, L",\n"); }
		else
		{	::fwprintf(pfile, L"\n"); }
	}
	::fwprintf(pfile, L"      },\n");//SN_List }

	::wcscpy(strTag, L"Check_Result");//Board Check Result
	::fwprintf(pfile, L"      \"%s\": %d\n", strTag, 0);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::WriteBoardSpcFile_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr)
{
	if ( NULL == pfile ) { return false; }
	if ( NULL == ProjectPtr ) { return false; }

	CAOIBoard *BoardPtr = this;
	if ( NULL == BoardPtr ) { return false; }	

	size_t           k=0;
	TREGION4D        MapRgn;	
	TREGION4D        Region;
	CString          strText;	
	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";		
	CAOIComponent   *ComponentPtr = NULL;
	size_t BoardComponentCount = BoardPtr->GetBoardComponentCount();
	size_t ComponentNotAgentCount = BoardPtr->GetBoardComponentNotAgentCount();

	LANE_ID    LaneID = ProjectPtr->GetProjectActLaneID();	
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();		

	//"Side" : 0,			// 板子正反面(整數-1 Byte)( 0:正 1:反 )
	::wcscpy(strTag, L"Side");			
	::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, BoardPtr->GetBoardSideMode());

	::wcscpy(strTag, L"Barcode");	
	strText = BoardPtr->GetBoardBarcode();
	if ( BoardPtr->GetBoardIsGetBarcode() == false )
	{	strText=JetAPI::AddSpcBarcodeInternalCode(strText);	}
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	JetAPI::FilterJSONStringW(strBuffer);
	::fwprintf(pfile, L"      \"%s\": \"%s\",\n", strTag, strBuffer);

	// 在底圖上的位置(LTx LTy RTx RTy LBx LBy RBx RBy)(pixel)
	Region = BoardPtr->GetBoardRgnStage(DistrictID);			
	ProjectPtr->MapProjectStageToMapPos(Region.minX, Region.minY, MapRgn.minX, MapRgn.minY, LaneID, DistrictID);
	ProjectPtr->MapProjectStageToMapPos(Region.maxX, Region.maxY, MapRgn.maxX, MapRgn.maxY, LaneID, DistrictID);
	Region.minX = MIN(MapRgn.minX, MapRgn.maxX);
	Region.minY = MIN(MapRgn.minY, MapRgn.maxY);
	Region.maxX = MAX(MapRgn.minX, MapRgn.maxX);
	Region.maxY = MAX(MapRgn.minY, MapRgn.maxY);		
	::wcscpy(strTag, L"Map_Location");//"Location" : [100,100,300,200,100,100,300,200],	// 在底圖上的位置(LTx LTy RTx RTy LBx LBy RBx RBy)(pixel)			
	//::fwprintf(pfile, L"      \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, Region.minX, Region.minY, Region.maxX, Region.minY, Region.minX, Region.maxY, Region.maxX, Region.maxY);
	::fwprintf(pfile, L"      \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, Region.minX, Region.minY, Region.minX, Region.maxY, Region.maxX, Region.maxY, Region.maxX, Region.minY);//20190919
			
	::wcscpy(strTag, L"Type");			
	::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, BoardPtr->GetBoardType());//1:normal

	//0:No Test  1:Skip 2:Bypass 3:Sys-Ok 4:Sys-Ng 5:Op-Ok 6:Op-Ng 7:Fd-Ng 
	RESULT_ID BoardResultID = BoardPtr->GetBoardResultID();			
	SPC_RESULT_ID SpcResultID = AOIDataCollect.MapResultIDToSpcResultID(BoardResultID);
	::wcscpy(strTag, L"Test_Result");			
	::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, SpcResultID);				
			
	::wcscpy(strTag, L"N_Parts");			
	::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, ComponentNotAgentCount);

	size_t BoardComponentCountNG = BoardPtr->GetBoardComponentNGCount();
	::wcscpy(strTag, L"N_NG_Parts");			
	::fwprintf(pfile, L"      \"%s\": %d,\n", strTag, BoardComponentCountNG);
	
	size_t PartId=0;
	::wcscpy(strTag, L"Part_List");//Part_List
	::fwprintf(pfile, L"      \"%s\": %s\n", strTag, L"[");//Part_List [
	for ( k=0; k<BoardComponentCount; k++ )
	{
		ComponentPtr = BoardPtr->GetBoardComponentPtr(k, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }

		::fwprintf(pfile, L"      {\n");

		::wcscpy(strTag, L"ID");
		::fwprintf(pfile, L"        \"%s\": %d,\n", strTag, PartId+1);

		if ( ComponentPtr->WriteComponentSpcFile_JSON_RSM(pfile, ProjectPtr) == false ) 
		{	return false; }
		//if ( k < (BoardComponentCount-1) )
		if ( PartId < (ComponentNotAgentCount-1) )
		{	::fwprintf(pfile, L"      },\n"); }
		else
		{	::fwprintf(pfile, L"      }\n"); }

		PartId ++;
	}
	::fwprintf(pfile, L"      ],\n");//Part_List ]			

	//Part Barcode List
	std::vector<TPartBarcode> PartBarcodeList;
	BoardPtr->BuildBoardPartBarcodeList(PartBarcodeList);
	const size_t PartBarcodeCount=PartBarcodeList.size();	
	::wcscpy(strTag, L"SN_List");//SN_List
	::fwprintf(pfile, L"      \"%s\": %s\n", strTag, L"[");//SN_List [
	for ( k=0; k<PartBarcodeCount; k++ )
	{
		JetAPI::FilterJSONStringW(PartBarcodeList[k].wsCode);
		::fwprintf(pfile, L"      [\"%s\",\"%s\"]", PartBarcodeList[k].wsName.c_str(), PartBarcodeList[k].wsCode.c_str());
		if ( k < (PartBarcodeCount-1) )
		{	::fwprintf(pfile, L",\n"); }
		else
		{	::fwprintf(pfile, L"\n"); }
	}
	::fwprintf(pfile, L"      ],\n");//SN_List ]		

	//Part Group List	
	bool bSavePartGroupFirst=true;
	const size_t BoardPartGroupCount=GetBoardPartGroupCount();
	::wcscpy(strTag, L"Cal_Result_List");//Cal_Result_List
	::fwprintf(pfile, L"      \"%s\": %s\n", strTag, L"[");//SN_List [
	for ( k=0; k<BoardPartGroupCount; k++ )
	{
		CAOIPartGroup *PartGroupPtr = GetBoardPartGroupPtr(k, false);
		if ( NULL == PartGroupPtr ) { continue; }
		if ( PartGroupPtr->WritePartGroupSpcFile_JSON_RSM(pfile, bSavePartGroupFirst) == false )
		{	return false; }
	}
	if ( false == bSavePartGroupFirst )
	{	::fwprintf(pfile, L"\n");	}
	::fwprintf(pfile, L"      ],\n");//Cal_Result_List ]

	::wcscpy(strTag, L"Check_Result");//Board Check Result
	::fwprintf(pfile, L"      \"%s\": %d\n", strTag, 0);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::WriteBoardSpcHeader_JSON_detail(FILE *pfile, CAOIProject *ProjectPtr) {

	CAOIBoard *BoardPtr = this;
	CAOIComponent *ComponentPtr = NULL;

	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";

	const size_t ComponentCount = BoardPtr->GetBoardComponentCount();
	size_t i;

	::fwprintf(pfile, L"        {\n");
	::wcscpy(strTag, L"ID");
	::fwprintf(pfile, L"          \"%s\":%d,\n", strTag, BoardPtr->GetBoardIndex_Panel() + 1);

	::wcscpy(strTag, L"N_Parts");
	::fwprintf(pfile, L"          \"%s\":%d,\n", strTag, ComponentCount);

	::wcscpy(strTag, L"Part_List");
	::fwprintf(pfile, L"          \"%s\":[\n", strTag);
	for (i = 0; i < ComponentCount; i++) {
		//if (i > 2)break;
		ComponentPtr = BoardPtr->GetBoardComponentPtr(i, false);
		if (NULL == ComponentPtr) { continue; }
		if (ComponentPtr->WriteComponentSpcHeader_JSON_detail(pfile, ProjectPtr) == false) { return FALSE; }
		//Prevent Trailing Comma
		if (i == ComponentCount - 1) { ::fwprintf(pfile, L"\n"); }
		else { ::fwprintf(pfile, L",\n"); }
	}
	::fwprintf(pfile, L"          ]\n");
	::fwprintf(pfile, L"        }");
	return TRUE;
}

//-------------------------------------------------------------------------------------//
bool CAOIBoard::ConvertToSpcBoard(DISTRICT_ID DistrictID, TSpcBoard &SpcBoard)//轉成SPC單板
{
	CAOIBoard *BoardPtr = this;
	if ( NULL == BoardPtr ) { return false; }
	TREGION4D Region;
	Region = BoardPtr->GetBoardRgnStage(DistrictID);

	::memset(&SpcBoard, 0x00, sizeof(SpcBoard));
	SpcBoard.uuidBoard = BoardPtr->GetObjUuid();
	SpcBoard.uPanelIndex = BoardPtr->GetBoardPanelIndex_Project();
	SpcBoard.uBoardIndex = BoardPtr->GetBoardIndex_Project();
	::wcscpy(SpcBoard.sBoardBarcode, BoardPtr->GetBoardBarcode());//單板條碼
	SpcBoard.nBoardTestResultID = BoardPtr->GetBoardResultID();
	SpcBoard.nBoardCheckResultID = BoardPtr->GetBoardResultID();
	
	SpcBoard.fBoardRgnMinX = Region.minX;
	SpcBoard.fBoardRgnMinY = Region.minY;
	SpcBoard.fBoardRgnMaxX = Region.maxX;
	SpcBoard.fBoardRgnMaxY = Region.maxY;
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::RemoveBoardAllObjects()//移除單板的所有物件
{
	RemoveBoardAllFds();
	RemoveBoardAllMarks();
	RemoveBoardAllBarcodes();
	RemoveBoardAllComponents();
	RemoveBoardAllPartGroups();
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::RemoveBoardObjectSelected()//移除單板內選取到的物件
{
	RemoveBoardFdSelected();
	RemoveBoardMarkSelected();
	RemoveBoardBarcodeSelected();
	RemoveBoardComponentSelected();
	LayoutBoardRegion();
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SelectBoardAllObjects(bool value)//選取專案所有物件
{
	SelectBoardAllFds(value);
	SelectBoardAllMarks(value);
	SelectBoardAllBarcodes(value);
	SelectBoardAllComponents(value);	
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::SetBoardAllObjectTempInt(int val, int idx)//設定單板內物件暫存參數
{
	//Fiducial
	const size_t FdCount=GetBoardFdCount();
	for ( size_t i=0; i<FdCount; i++ )
	{
		CAOIFd *FdPtr = GetBoardFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		FdPtr->SetFdTempInt(val, idx);
	}
	
	//Mark
	const size_t MarkCount=GetBoardMarkCount();
	for ( size_t i=0; i<MarkCount; i++ )
	{	
		CAOIMark *MarkPtr = GetBoardMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		MarkPtr->SetMarkTempInt(val, idx);
	}
	
	//Barcode
	const size_t BarcodeCount=GetBoardBarcodeCount();
	for ( size_t i=0; i<BarcodeCount; i++ )
	{				
		CAOIBarcode *BarcodePtr = GetBoardBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		BarcodePtr->SetBarcodeTempInt(val, idx);
	}

	//Component
	const size_t ComponentCount=GetBoardComponentCount();
	for ( size_t i=0; i<ComponentCount; i++ )
	{	
		CAOIComponent *ComponentPtr = GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentTempInt(val, idx);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::SetBoardAllObjectTempInt(DISTRICT_ID DistrictID, int val, int idx)//設定單板內物件暫存參數
{
	//Fiducial
	const size_t FdCount=GetBoardFdCount();
	for ( size_t i=0; i<FdCount; i++ )
	{
		CAOIFd *FdPtr = GetBoardFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }		
		FdPtr->SetFdTempInt(val, idx);
	}
	
	//Mark
	const size_t MarkCount=GetBoardMarkCount();
	for ( size_t i=0; i<MarkCount; i++ )
	{	
		CAOIMark *MarkPtr = GetBoardMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		MarkPtr->SetMarkTempInt(val, idx);
	}
	
	//Barcode
	const size_t BarcodeCount=GetBoardBarcodeCount();
	for ( size_t i=0; i<BarcodeCount; i++ )
	{				
		CAOIBarcode *BarcodePtr = GetBoardBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		BarcodePtr->SetBarcodeTempInt(val, idx);
	}

	//Component
	const size_t ComponentCount=GetBoardComponentCount();
	for ( size_t i=0; i<ComponentCount; i++ )
	{	
		CAOIComponent *ComponentPtr = GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		ComponentPtr->SetComponentTempInt(val, idx);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CheckBoardAllObjectInOneField(DISTRICT_ID DistrictID) const//確認單板所有物件視野是同一個
{
	if ( CheckBoardNeedToCalculate(DistrictID) == false ) { return true; }

	int NgCount=0;
	CAOIField *FieldPtr=GetBoardPanelFieldPtr(DistrictID);	

	//Fiducial	
	const size_t FdCount=GetBoardFdCount();
	for ( size_t i=0; i<FdCount; i++ )
	{
		CAOIFd *FdPtr=GetBoardFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }				
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		if ( FdPtr->GetFdNeedToCalculate() == false ) { continue; }
		if ( NULL == FieldPtr )
		{
			FieldPtr = FdPtr->GetFdFieldPtr();
			continue;
		}
		if ( FdPtr->GetFdFieldPtr() == FieldPtr ) { continue; }
		//return false;
		NgCount ++;		
	}
	
	//Mark
	const size_t MarkCount=GetBoardMarkCount();
	for ( size_t i=0; i<MarkCount; i++ )
	{
		CAOIMark *MarkPtr=GetBoardMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		if ( MarkPtr->GetMarkNeedToCalculate() == false ) { continue; }
		if ( NULL == FieldPtr )
		{
			FieldPtr = MarkPtr->GetMarkFieldPtr();
			continue;
		}
		if ( MarkPtr->GetMarkFieldPtr() == FieldPtr ) { continue; }
		//return false;
		NgCount ++;		
	}

	//Barcode
	const size_t BarcodeCount=GetBoardBarcodeCount();
	for ( size_t i=0; i<BarcodeCount; i++ )
	{
		CAOIBarcode *BarcodePtr=GetBoardBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		if ( BarcodePtr->GetBarcodeNeedToCalculate() == false ) { continue; }
		if ( NULL == FieldPtr )
		{
			FieldPtr = BarcodePtr->GetBarcodeFieldPtr();
			continue;
		}
		if ( BarcodePtr->GetBarcodeFieldPtr() == FieldPtr ) { continue; }
		//return false;
		NgCount ++;		
	}

	//Component
	const size_t ComponentCount=GetBoardComponentCount();
	for ( size_t i=0; i<ComponentCount; i++ )
	{
		CAOIComponent *ComponentPtr=GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }		
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		if ( ComponentPtr->GetComponentNeedToCalculate() == false ) { continue; }
		if ( ComponentPtr->GetComponentSelfFieldEnabled() == true ) { continue; }
		if ( NULL == FieldPtr )
		{
			FieldPtr = ComponentPtr->GetComponentFieldPtr();
			continue;
		}
		if ( ComponentPtr->GetComponentFieldPtr() == FieldPtr ) { continue; }
		//return false;
		NgCount ++;		
	}
	if ( NgCount > 0 )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::BuildBoardObjectListInRegion(DISTRICT_ID DistrictID, const TREGION4D &FieldRgn, bool ByCadRegion, std::vector<CAOIRgn*> &RgnList)
{	
	int NgCount=0;
	TREGION4D Region;
	bool PartWndsInOneFOV=false;
	RgnList.clear();
{
	//Fiducial
	const size_t FdCount=GetBoardFdCount();
	for ( size_t i=0; i<FdCount; i++ )
	{
		CAOIFd *FdPtr=GetBoardFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }		
		if ( FdPtr->GetFdNeedToCalculate() == false ) { continue; }		
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		const size_t SubRgnCount = FdPtr->GetFdSubRgnCount();
		if ( 0 == SubRgnCount )
		{
			if ( true == ByCadRegion )
			{	FdPtr->GetFdExtendCadRegion(Region);	}
			else
			{	FdPtr->GetFdExtendStageRegion(Region);	}
			PartWndsInOneFOV = JetAPI::CheckRgnInRegion(Region, FieldRgn, true);
			if ( false == PartWndsInOneFOV )
			{
				NgCount ++;
				continue; 
			}
		}		
		RgnList.push_back(FdPtr);
		for ( size_t j=0; j<SubRgnCount; j++ )
		{
			CAOIRgn *RgnSubPtr=FdPtr->GetFdSubRgnPtr(j, false);
			if ( NULL == RgnSubPtr ) { continue; }
			if ( true == ByCadRegion )
			{	RgnSubPtr->GetRgnRoiCadRegion(Region);	}
			else
			{	RgnSubPtr->GetRgnRoiStageRegion(Region); }
			PartWndsInOneFOV = JetAPI::CheckRgnInRegion(Region, FieldRgn, true);
			if ( false == PartWndsInOneFOV )
			{	
				NgCount ++;
				continue; 
			}
			RgnList.push_back(RgnSubPtr);
		}
	}
}
	
{
	//Mark
	const size_t MarkCount=GetBoardMarkCount();
	for ( size_t i=0; i<MarkCount; i++ )
	{
		CAOIMark *MarkPtr=GetBoardMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkNeedToCalculate() == false ) { continue; }		
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		const size_t SubRgnCount = MarkPtr->GetMarkSubRgnCount();
		if ( 0 == SubRgnCount )
		{
			if ( true == ByCadRegion )
			{	MarkPtr->GetMarkRoiCadRegion(Region);	}
			else
			{	MarkPtr->GetMarkRoiStageRegion(Region);	}
			PartWndsInOneFOV = JetAPI::CheckRgnInRegion(Region, FieldRgn, true);
			if ( false == PartWndsInOneFOV )
			{
				NgCount ++;
				continue; 
			}
		}

		RgnList.push_back(MarkPtr);		
		for ( size_t j=0; j<SubRgnCount; j++ )
		{
			CAOIRgn *RgnSubPtr=MarkPtr->GetMarkSubRgnPtr(j, false);
			if ( NULL == RgnSubPtr ) { continue; }
			if ( true == ByCadRegion )
			{	RgnSubPtr->GetRgnRoiCadRegion(Region);	}
			else
			{	RgnSubPtr->GetRgnRoiStageRegion(Region); }
			PartWndsInOneFOV = JetAPI::CheckRgnInRegion(Region, FieldRgn, true);
			if ( false == PartWndsInOneFOV )
			{
				NgCount ++;
				continue; 
			}
			RgnList.push_back(RgnSubPtr);
		}
	}
}
	
{
	//Barcode
	const size_t BarcodeCount=GetBoardBarcodeCount();
	for ( size_t i=0; i<BarcodeCount; i++ )
	{
		CAOIBarcode *BarcodePtr=GetBoardBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeNeedToCalculate() == false ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		const size_t SubRgnCount = BarcodePtr->GetBarcodeSubRgnCount();		
		if ( 0 == SubRgnCount )
		{
			if ( true == ByCadRegion )
			{	BarcodePtr->GetBarcodeRoiCadRegion(Region);	}
			else
			{	BarcodePtr->GetBarcodeRoiStageRegion(Region);	}
			PartWndsInOneFOV = JetAPI::CheckRgnInRegion(Region, FieldRgn, true);
			if ( false == PartWndsInOneFOV )
			{
				NgCount ++;
				continue; 
			}
		}

		RgnList.push_back(BarcodePtr);
		for ( size_t j=0; j<SubRgnCount; j++ )
		{
			CAOIRgn *RgnSubPtr=BarcodePtr->GetBarcodeSubRgnPtr(j, false);
			if ( NULL == RgnSubPtr ) { continue; }
			if ( true == ByCadRegion )
			{	RgnSubPtr->GetRgnRoiCadRegion(Region);	}
			else
			{	RgnSubPtr->GetRgnRoiStageRegion(Region); }
			PartWndsInOneFOV = JetAPI::CheckRgnInRegion(Region, FieldRgn, true);
			if ( false == PartWndsInOneFOV )
			{
				NgCount ++;
				continue; 
			}
			RgnList.push_back(RgnSubPtr);
		}
	}
}
	
{
	//Component
	const size_t ComponentCount=GetBoardComponentCount();
	for ( size_t i=0; i<ComponentCount; i++ )
	{
		CAOIComponent *ComponentPtr=GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }		
		if ( ComponentPtr->GetComponentNeedToCalculate() == false ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		const size_t SubRgnCount = ComponentPtr->GetComponentSubRgnCount();
		if ( 0 == SubRgnCount )
		{
			if ( true == ByCadRegion )
			{	ComponentPtr->GetComponentWindowCadRegion(Region); }
			else
			{	ComponentPtr->GetComponentWindowStageRegion(Region); }
			PartWndsInOneFOV = JetAPI::CheckRgnInRegion(Region, FieldRgn, true);
			if ( false == PartWndsInOneFOV )
			{
				NgCount ++;
				continue; 
			}
		}
		RgnList.push_back(ComponentPtr);
		for ( size_t j=0; j<SubRgnCount; j++ )
		{
			CAOIRgn *RgnSubPtr=ComponentPtr->GetComponentSubRgnPtr(j, false);
			if ( NULL == RgnSubPtr ) { continue; }
			if ( true == ByCadRegion )
			{	RgnSubPtr->GetRgnRoiCadRegion(Region);	}
			else
			{	RgnSubPtr->GetRgnRoiStageRegion(Region); }
			PartWndsInOneFOV = JetAPI::CheckRgnInRegion(Region, FieldRgn, true);
			if ( false == PartWndsInOneFOV )
			{
				NgCount ++;
				continue; 
			}
			RgnList.push_back(RgnSubPtr);
		}
	}
}
	if ( NgCount > 0 )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardProjectPtr(CAOIProject *Ptr)
{	
	m_BoardProjectPtr = Ptr;	
}
//-------------------------------------------------------------------------------------//
CAOIProject* CAOIBoard::GetBoardProjectPtr() const 
{	
	return m_BoardProjectPtr;	
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardPanelPtr(CAOIPanel *Ptr)
{	
	m_BoardPanelPtr = Ptr;	
}
//-------------------------------------------------------------------------------------//
CAOIPanel* CAOIBoard::GetBoardPanelPtr() const
{	
	return m_BoardPanelPtr; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::ResetBoardPanelFieldParam(DISTRICT_ID DistrictID)//清除單板的整板區域
{	
	SetBoardPanelFieldPtr(NULL, DistrictID);	
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardPanelFieldPtr(CAOIField *Ptr, DISTRICT_ID DistrictID)//單板的整板區域指標
{
	switch ( DistrictID )
	{
	case DISTRICT_ID_B:	SetBoardPanelFieldPtr_DB(Ptr);	break;
	case DISTRICT_ID_A:	SetBoardPanelFieldPtr_DA(Ptr);	break;		
	}
}
//-------------------------------------------------------------------------------------//
CAOIField* CAOIBoard::GetBoardPanelFieldPtr(DISTRICT_ID DistrictID) const//單板的整板區域指標
{
	CAOIField *Ptr=NULL;
	switch ( DistrictID )
	{
	case DISTRICT_ID_B:	Ptr=GetBoardPanelFieldPtr_DB();	break;
	case DISTRICT_ID_A:	Ptr=GetBoardPanelFieldPtr_DA();	break;
	}
	return Ptr;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardPanelFieldPtr_DA(CAOIField *Ptr)//單板的整板區域指標
{
	m_BoardPanelFieldPtr_DA = Ptr;
}
//-------------------------------------------------------------------------------------//
CAOIField* CAOIBoard::GetBoardPanelFieldPtr_DA() const//單板的整板區域指標
{
	return m_BoardPanelFieldPtr_DA;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardPanelFieldPtr_DB(CAOIField *Ptr)//單板的整板區域指標
{
	m_BoardPanelFieldPtr_DB = Ptr;
}
//-------------------------------------------------------------------------------------//
CAOIField* CAOIBoard::GetBoardPanelFieldPtr_DB() const//單板的整板區域指標
{
	return m_BoardPanelFieldPtr_DB;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::SetBoardPanelFieldToObj(CAOIField *FieldPtr)//設定單板的整板區域指標至底層物件
{
	if ( NULL == FieldPtr ) { return false; }
	DISTRICT_ID DistrictID=FieldPtr->GetFieldDistrictID();
{
	//Fd
	const size_t FdCount=GetBoardFdCount();
	for ( size_t i=0; i<FdCount; i++ )
	{
		CAOIFd *FdPtr = GetBoardFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		//if ( FdPtr->GetFdNeedToCalculate() == false ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		FdPtr->SetFdFieldPtr(FieldPtr);

		const size_t SubRgnCount=FdPtr->GetFdSubRgnCount();
		for ( size_t j=0; j<SubRgnCount; j++ )
		{
			CAOIRgn *SubRgnPtr = FdPtr->GetFdSubRgnPtr(j, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->SetRgnFieldPtr(FieldPtr);
		}
	}
}

{
	//Mark
	const size_t MarkCount=GetBoardMarkCount();
	for ( size_t i=0; i<MarkCount; i++ )
	{
		CAOIMark *MarkPtr = GetBoardMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		//if ( MarkPtr->GetMarkNeedToCalculate() == false ) { continue; }
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		MarkPtr->SetMarkFieldPtr(FieldPtr);

		const size_t SubRgnCount=MarkPtr->GetMarkSubRgnCount();
		for ( size_t j=0; j<SubRgnCount; j++ )
		{
			CAOIRgn *SubRgnPtr = MarkPtr->GetMarkSubRgnPtr(j, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->SetRgnFieldPtr(FieldPtr);
		}
	}
}

{
	//Barcode
	const size_t BarcodeCount=GetBoardBarcodeCount();
	for ( size_t i=0; i<BarcodeCount; i++ )
	{
		CAOIBarcode *BarcodePtr = GetBoardBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		//if ( BarcodePtr->GetBarcodeNeedToCalculate() == false ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		BarcodePtr->SetBarcodeFieldPtr(FieldPtr);

		const size_t SubRgnCount=BarcodePtr->GetBarcodeSubRgnCount();
		for ( size_t j=0; j<SubRgnCount; j++ )
		{
			CAOIRgn *SubRgnPtr = BarcodePtr->GetBarcodeSubRgnPtr(j, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->SetRgnFieldPtr(FieldPtr);
		}
	}
}

{
	//Component
	const size_t ComponentCount=GetBoardComponentCount();
	for ( size_t i=0; i<ComponentCount; i++ )
	{
		CAOIComponent *ComponentPtr = GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		//if ( ComponentPtr->GetComponentNeedToCalculate() == false ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		ComponentPtr->SetComponentFieldPtr(FieldPtr);

		const size_t SubRgnCount=ComponentPtr->GetComponentSubRgnCount();
		for ( size_t j=0; j<SubRgnCount; j++ )
		{
			CAOIRgn *SubRgnPtr = ComponentPtr->GetComponentSubRgnPtr(j, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->SetRgnFieldPtr(FieldPtr);
		}
	}
}
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIBoard::GetBoardErrorString() const
{	
	return m_BoardErrorString; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardErrorString(LPCTSTR val)
{	
	m_BoardErrorString = val; 
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIBoard::GetBoardIndex_Project() const
{	
	return m_BoardIndex_Project; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardIndex_Project(unsigned int value)
{	
	m_BoardIndex_Project = value; 
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIBoard::GetBoardIndex_Panel() const
{	
	return m_BoardIndex_Panel; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardIndex_Panel(unsigned int value)
{	
	m_BoardIndex_Panel = value; 
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIBoard::GetBoardPanelIndex_Project() const
{ 
	return m_BoardPanelIndex_Project; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardPanelIndex_Project(unsigned int value)
{	
	m_BoardPanelIndex_Project = value; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::UpdateBoardIndexToObjList()//更新單板引數至單板內的物件
{
	CAOIBoard *BoardPtr = this;
	if ( NULL == BoardPtr ) { return ; }

	size_t         i=0;
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const size_t   FdCount = BoardPtr->GetBoardFdCount_Inline();
	const size_t   MarkCount = BoardPtr->GetBoardMarkCount_Inline();
	const size_t   BarcodeCount = BoardPtr->GetBoardBarcodeCount_Inline();
	const size_t   ComponentCount = BoardPtr->GetBoardComponentCount_Inline();
	const unsigned int BoardIndex_Panel = BoardPtr->GetBoardIndex_Panel();
	const unsigned int BoardIndex_Project = BoardPtr->GetBoardIndex_Project();

	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = BoardPtr->GetBoardFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		FdPtr->SetFdBoardIndex_Panel(BoardIndex_Panel);
		FdPtr->SetFdBoardIndex_Project(BoardIndex_Project);		
	}

	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = BoardPtr->GetBoardMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		MarkPtr->SetMarkBoardIndex_Panel(BoardIndex_Panel);
		MarkPtr->SetMarkBoardIndex_Project(BoardIndex_Project);		
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = BoardPtr->GetBoardBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		BarcodePtr->SetBarcodeBoardIndex_Panel(BoardIndex_Panel);
		BarcodePtr->SetBarcodeBoardIndex_Project(BoardIndex_Project);		
	}

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = BoardPtr->GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentBoardIndex_Panel(BoardIndex_Panel);
		ComponentPtr->SetComponentBoardIndex_Project(BoardIndex_Project);		
	}
	return;
}
//-------------------------------------------------------------------------------------//
int  CAOIBoard::GetBoardTempInt() const 
{
	return m_BoardTempInt[0]; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardTempInt(int value) 
{
	m_BoardTempInt[0] = value; 
}
//-------------------------------------------------------------------------------------//
int CAOIBoard::GetBoardTempInt_01() const//單板暫存整數
{
	return m_BoardTempInt[1]; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardTempInt_01(int value)//單板暫存整數
{
	m_BoardTempInt[1] = value; 
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::GetBoardDeleted() const 
{ 
	return m_BoardDeleted; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardDeleted(bool value)
{	
	m_BoardDeleted = value; 
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::GetBoardSelected() const 
{ 
	return m_BoardSelected; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardSelected(bool value)
{ 
	m_BoardSelected = value; 
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CheckBoardNeedToCalculate(DISTRICT_ID DistrictID) const
{
	if ( GetBoardBypassed() == true ) { return false; }

	//Fiducial
	const size_t FdCount=GetBoardFdCount();
	for ( size_t i=0; i<FdCount; i++ )
	{
		CAOIFd *FdPtr=GetBoardFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }				
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		if ( FdPtr->GetFdNeedToCalculate() == false ) { continue; }		
		return true;		
	}

	//Mark
	const size_t MarkCount=GetBoardMarkCount();
	for ( size_t i=0; i<MarkCount; i++ )
	{
		CAOIMark *MarkPtr=GetBoardMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		if ( MarkPtr->GetMarkNeedToCalculate() == false ) { continue; }		
		return true;
	}

	//Barcode
	const size_t BarcodeCount=GetBoardBarcodeCount();
	for ( size_t i=0; i<BarcodeCount; i++ )
	{
		CAOIBarcode *BarcodePtr=GetBoardBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		if ( BarcodePtr->GetBarcodeNeedToCalculate() == false ) { continue; }
		return true;		
	}

	//Component
	const size_t ComponentCount=GetBoardComponentCount();
	for ( size_t i=0; i<ComponentCount; i++ )
	{
		CAOIComponent *ComponentPtr=GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }		
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		if ( ComponentPtr->GetComponentNeedToCalculate() == false ) { continue; }		
		return true;		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::ChangeBoardPanel(CAOIPanel *RefPanelPtr)//變更單板的整板
{
	CAOIBoard     *BoardPtr=this;
	if ( NULL == BoardPtr ) { return false; }
	if ( NULL == RefPanelPtr ) { return false; }

	size_t         i=0, j=0;	
	double         CadDifX=0, CadDifY=0;	
	double         CadPosX=0, CadPosY=0;	
	double         StagePosX=0, StagePosY=0;
	double         CadPosXNew=0, CadPosYNew=0;	
	CAOIFd        *FdPtr=NULL;
	CAOIMark      *MarkPtr=NULL;
	CAOIPanel     *PanelPtr=NULL;	
	CAOIField     *FieldPtr = NULL;
	CAOIBarcode   *BarcodePtr=NULL;
	CAOIComponent *ComponentPtr=NULL;
	DISTRICT_ID    DistrictID;
	CMapCoordinate PanelMapCTS;
	CMapCoordinate PanelMapSTC;
	CMapCoordinate PanelMapCTS_DB;
	CMapCoordinate PanelMapSTC_DB;	
	const size_t   FdCount = BoardPtr->GetBoardFdCount_Inline();
	const size_t   MarkCount = BoardPtr->GetBoardMarkCount_Inline();
	const size_t   FieldCount = BoardPtr->GetBoardFieldCount_Inline();
	const size_t   BarcodeCount = BoardPtr->GetBoardBarcodeCount_Inline();	
	const size_t   ComponentCount = BoardPtr->GetBoardComponentCount_Inline();	

	PanelPtr = GetBoardPanelPtr();
	if ( PanelPtr == RefPanelPtr ) { return true; }
	RefPanelPtr->GetPanelMapCTS(DISTRICT_ID_A, PanelMapCTS);
	RefPanelPtr->GetPanelMapSTC(DISTRICT_ID_A, PanelMapSTC);
	RefPanelPtr->GetPanelMapCTS(DISTRICT_ID_B, PanelMapCTS_DB);
	RefPanelPtr->GetPanelMapSTC(DISTRICT_ID_B, PanelMapSTC_DB);

	PanelPtr->RemovePanelBoard(BoardPtr);
	PanelPtr->LayoutPanelRegion();
	RefPanelPtr->AddPanelBoardPtr(BoardPtr);	
	const size_t BoardIndexPanel = BoardPtr->GetBoardIndex_Panel();

	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = BoardPtr->GetBoardFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		DistrictID = FdPtr->GetFdDistrictID();

		CadPosX = FdPtr->GetFdCadPosX();
		CadPosY = FdPtr->GetFdCadPosY();
		StagePosX = FdPtr->GetFdStagePosX();
		StagePosY = FdPtr->GetFdStagePosY();

		if ( DISTRICT_ID_A == DistrictID )
		{	PanelMapSTC.Map2D(StagePosX, StagePosY, CadPosXNew, CadPosYNew);	}
		if ( DISTRICT_ID_B == DistrictID )
		{	PanelMapSTC_DB.Map2D(StagePosX, StagePosY, CadPosXNew, CadPosYNew);	}		

		CadDifX = CadPosXNew-CadPosX;
		CadDifY = CadPosYNew-CadPosY;
		if ( DISTRICT_ID_A == DistrictID )
		{	FdPtr->MoveFdPos(CadDifX, CadDifY, &PanelMapCTS); }
		if ( DISTRICT_ID_B == DistrictID )
		{	FdPtr->MoveFdPos(CadDifX, CadDifY, &PanelMapCTS_DB); }
		RefPanelPtr->AddPanelFdPtr(FdPtr);
		FdPtr->SetFdBoardIndex_Panel(BoardIndexPanel);
	}

	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = BoardPtr->GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		DistrictID = MarkPtr->GetMarkDistrictID();

		CadPosX = MarkPtr->GetMarkCadPosX();
		CadPosY = MarkPtr->GetMarkCadPosY();
		StagePosX = MarkPtr->GetMarkStagePosX();
		StagePosY = MarkPtr->GetMarkStagePosY();

		if ( DISTRICT_ID_A == DistrictID )
		{	PanelMapSTC.Map2D(StagePosX, StagePosY, CadPosXNew, CadPosYNew);	}
		if ( DISTRICT_ID_B == DistrictID )
		{	PanelMapSTC_DB.Map2D(StagePosX, StagePosY, CadPosXNew, CadPosYNew);	}		

		CadDifX = CadPosXNew-CadPosX;
		CadDifY = CadPosYNew-CadPosY;
		if ( DISTRICT_ID_A == DistrictID )
		{	MarkPtr->MoveMarkPos(CadDifX, CadDifY, &PanelMapCTS); }
		if ( DISTRICT_ID_B == DistrictID )
		{	MarkPtr->MoveMarkPos(CadDifX, CadDifY, &PanelMapCTS_DB); }
		RefPanelPtr->AddPanelMarkPtr(MarkPtr);
		MarkPtr->SetMarkBoardIndex_Panel(BoardIndexPanel);
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = BoardPtr->GetBoardBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		DistrictID = BarcodePtr->GetBarcodeDistrictID();

		CadPosX = BarcodePtr->GetBarcodeCadPosX();
		CadPosY = BarcodePtr->GetBarcodeCadPosY();
		StagePosX = BarcodePtr->GetBarcodeStagePosX();
		StagePosY = BarcodePtr->GetBarcodeStagePosY();

		if ( DISTRICT_ID_A == DistrictID )
		{	PanelMapSTC.Map2D(StagePosX, StagePosY, CadPosXNew, CadPosYNew);	}
		if ( DISTRICT_ID_B == DistrictID )
		{	PanelMapSTC_DB.Map2D(StagePosX, StagePosY, CadPosXNew, CadPosYNew);	}		

		CadDifX = CadPosXNew-CadPosX;
		CadDifY = CadPosYNew-CadPosY;
		if ( DISTRICT_ID_A == DistrictID )
		{	BarcodePtr->MoveBarcodePos(CadDifX, CadDifY, &PanelMapCTS); }
		if ( DISTRICT_ID_B == DistrictID )
		{	BarcodePtr->MoveBarcodePos(CadDifX, CadDifY, &PanelMapCTS_DB); }
		RefPanelPtr->AddPanelBarcodePtr(BarcodePtr);
		BarcodePtr->SetBarcodeBoardIndex_Panel(BoardIndexPanel);
	}

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = BoardPtr->GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		DistrictID = ComponentPtr->GetComponentDistrictID();

		CadPosX = ComponentPtr->GetComponentCadPosX();
		CadPosY = ComponentPtr->GetComponentCadPosY();
		StagePosX = ComponentPtr->GetComponentStagePosX();
		StagePosY = ComponentPtr->GetComponentStagePosY();
		
		if ( DISTRICT_ID_A == DistrictID )
		{	PanelMapSTC.Map2D(StagePosX, StagePosY, CadPosXNew, CadPosYNew);	}
		if ( DISTRICT_ID_B == DistrictID )
		{	PanelMapSTC_DB.Map2D(StagePosX, StagePosY, CadPosXNew, CadPosYNew);	}		

		CadDifX = CadPosXNew-CadPosX;
		CadDifY = CadPosYNew-CadPosY;
		if ( DISTRICT_ID_A == DistrictID )
		{	ComponentPtr->MoveComponentPos(CadDifX, CadDifY, &PanelMapCTS); }
		if ( DISTRICT_ID_B == DistrictID )
		{	ComponentPtr->MoveComponentPos(CadDifX, CadDifY, &PanelMapCTS_DB); }
		RefPanelPtr->AddPanelComponentPtr(ComponentPtr);
		ComponentPtr->SetComponentBoardIndex_Panel(BoardIndexPanel);

		if ( ComponentPtr->GetComponentModelIsolated() == true )
		{			
			CAOIModel *ModelPtr = ComponentPtr->GetComponentModelPtr();
			if ( NULL != ModelPtr )
			{
				CString ModelFolder;
				CString ModelFolderOld;
				CString ModelFolderNew;
				ModelFolderOld = ModelPtr->GetModelFolderComponent();	
				JetAPI::ExtractLastPath(ModelFolderOld, ModelFolder);
				CString FullComponentName = ComponentPtr->GetComponentFullName();		
				ModelFolderNew.Format(_T("%s\\%s"), ModelFolder, FullComponentName);
				::MoveFile(ModelFolderOld, ModelFolderNew);//直接變更資料夾名稱			
				ModelPtr->SetModelFolderComponent(ModelFolderNew);
				ModelPtr->AssignModelFolder();
			}
		}		
	}

	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = BoardPtr->GetBoardFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		DistrictID = FieldPtr->GetFieldDistrictID();

		CadPosX = FieldPtr->GetFieldCadPosX();
		CadPosY = FieldPtr->GetFieldCadPosY();
		StagePosX = FieldPtr->GetFieldStagePosX();
		StagePosY = FieldPtr->GetFieldStagePosY();
		
		if ( DISTRICT_ID_A == DistrictID )
		{	PanelMapSTC.Map2D(StagePosX, StagePosY, CadPosXNew, CadPosYNew);	}
		if ( DISTRICT_ID_B == DistrictID )
		{	PanelMapSTC_DB.Map2D(StagePosX, StagePosY, CadPosXNew, CadPosYNew);	}		

		FieldPtr->SetFieldCadPosX(CadPosXNew);
		FieldPtr->SetFieldCadPosY(CadPosYNew);
		FieldPtr->SetFieldBoardIndex(BoardIndexPanel);
	}
	BoardPtr->LayoutBoardRegion();	
	RefPanelPtr->LayoutPanelRegion();
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::GetBoardFdCount() const//取得單板的定位點數量
{
	return GetBoardFdCount_Inline();
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::CalcBoardFdCount(DISTRICT_ID DistrictID)//計算只屬於單板定位點的數量
{
	size_t  i = 0;
	size_t  count=0;	
	CAOIFd *FdPtr = NULL;		
	const size_t FdCount = GetBoardFdCount_Inline();

	count = 0;	
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetBoardFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		count ++;
	}
	return count;
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::CalcBoardFdDefectCount(DISTRICT_ID DistrictID)//計算單板定位點瑕疵的數量
{
	size_t  i = 0;
	size_t  count=0;
	RESULT_ID ResultID;
	CAOIFd *FdPtr = NULL;		
	const size_t FdCount = GetBoardFdCount_Inline();

	count = 0;	
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetBoardFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }

		ResultID = FdPtr->GetFdResultID_AOI();
		if ( RESULT_ID_OK==ResultID || RESULT_ID_BYPASS == ResultID || RESULT_ID_SKIP==ResultID )
		{ continue;		}
		count ++;
	}
	return count;
}
//-------------------------------------------------------------------------------------//
CAOIFd* CAOIBoard::GetBoardFdPtr(size_t index, bool check) const//取得單板的定位點指標
{
	if ( check )
	{
		const size_t Count = GetBoardFdCount_Inline();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return GetBoardFdPtr_Inline(index);	
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::AddBoardFdPtr(CAOIFd *FdPtr)//增加單板的定位點
{
	const int FdIndex = (int)(GetBoardFdCount_Inline());	
	FdPtr->SetFdPanelPtr(m_BoardPanelPtr);
	FdPtr->SetFdPanelIndex_Project(m_BoardPanelIndex_Project);
	
	FdPtr->SetFdBoardPtr(this);
	FdPtr->SetFdBoardIndex_Project(m_BoardIndex_Project);
	FdPtr->SetFdBoardIndex_Panel(m_BoardIndex_Panel);
	FdPtr->SetFdIndex_Board(FdIndex);	
	AddBoardFdPtr_Inline(FdPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::SelectBoardAllFds(bool Select)//選取單板的定位點
{
	size_t i = 0;
	CAOIFd*  FdPtr = NULL;
	const size_t FdCount = GetBoardFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetBoardFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		FdPtr->SetFdSelected(Select);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::RemoveBoardFdSelected()//移除選取到的單板的定位點
{
	size_t i=0;
	int    index=0;
	CAOIFd*  FdPtr = NULL;;
	std::vector<CAOIFd*> BoardFdPtrList = m_BoardFdPtrList;
	const size_t FdCount = BoardFdPtrList.size();

	index = 0;
	m_BoardFdPtrList.clear();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = BoardFdPtrList[i];
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdSelected() == true ) { continue; }
		
		FdPtr->SetFdIndex_Board(index);
		AddBoardFdPtr_Inline(FdPtr);		
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::RemoveBoardAllFds()//移除單板的定位點
{
	m_BoardFdPtrList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::LayoutBoardFdList()//重整單板的定位點列表
{
	size_t i=0;
	int    index=0;
	CAOIFd*  FdPtr = NULL;;
	std::vector<CAOIFd*> BoardFdPtrList = m_BoardFdPtrList;
	const size_t FdCount = BoardFdPtrList.size();

	index = 0;
	m_BoardFdPtrList.clear();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = BoardFdPtrList[i];
		if ( NULL == FdPtr ) { continue; }
		
		FdPtr->SetFdIndex_Board(index);
		AddBoardFdPtr_Inline(FdPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CheckBoardFdCalculated(DISTRICT_ID DistrictID)//確認單板的定位點都計算過
{
	size_t    i = 0;
	RESULT_ID ResultID;
	CAOIFd*   FdPtr = NULL;	
	const size_t FdCount = GetBoardFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetBoardFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdDistrictID() != DistrictID ) { continue; }
		ResultID = FdPtr->GetFdResultID_AOI();
		if ( RESULT_ID_NONE == ResultID ) { return false; }		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::ClearBoardFdImageBuffer(DISTRICT_ID DistrictID)//清除單板定位點影像資料
{		
	CAOIFd*   FdPtr = NULL;	
	const size_t FdCount = GetBoardFdCount_Inline();
	for ( size_t i=0; i<FdCount; i++ )
	{
		FdPtr = GetBoardFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdDistrictID() != DistrictID ) { continue; }
		FdPtr->SetFdKeepImage(false);
		FdPtr->ClearRgnImageBuffer();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::GetBoardMarkCount() const//取得單板的特徵點數量
{
	return GetBoardMarkCount_Inline();
}
//-------------------------------------------------------------------------------------//
CAOIMark* CAOIBoard::GetBoardMarkPtr(size_t index, bool check) const//取得單板的特徵點指標
{
	if ( check )
	{
		const size_t Count = GetBoardMarkCount_Inline();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return GetBoardMarkPtr_Inline(index);
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::AddBoardMarkPtr(CAOIMark *MarkPtr)//增加單板的特徵點
{
	unsigned int MarkIndex = (unsigned int)(GetBoardMarkCount_Inline());	
	MarkPtr->SetMarkPanelPtr(m_BoardPanelPtr);
	MarkPtr->SetMarkPanelIndex_Project(m_BoardPanelIndex_Project);
	
	MarkPtr->SetMarkBoardPtr(this);
	MarkPtr->SetMarkBoardIndex_Project(m_BoardIndex_Project);
	MarkPtr->SetMarkBoardIndex_Panel(m_BoardIndex_Panel);
	MarkPtr->SetMarkIndex_Board(MarkIndex);	
	AddBoardMarkPtr_Inline(MarkPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIMark* CAOIBoard::GetBoardMarkPtrBySelected()//取得單板的選到的特徵點
{
	size_t i = 0;
	CAOIMark*  MarkPtr = NULL;
	const size_t MarkCount = GetBoardMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }
		return MarkPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
int CAOIBoard::GetBoardMarkMaxLocalBasePlaneID()//取得單板的特徵點最大局部基準面編號
{
	size_t i = 0;
	int    MaxID = 0;
	CAOIMark*  MarkPtr = NULL;
	const size_t MarkCount = GetBoardMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkBypassed() == true ) { continue; }
		if ( MaxID < MarkPtr->GetMarkLocalBasePlaneID() ) 
		{	MaxID = MarkPtr->GetMarkLocalBasePlaneID();	}
	}
	return MaxID;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::SelectBoardAllMarks(bool Select)//選取單板的特徵點
{
	size_t i = 0;
	CAOIMark*  MarkPtr = NULL;
	const size_t MarkCount = GetBoardMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		MarkPtr->SetMarkSelected(Select);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::RemoveBoardMarkSelected()//移除選取到的單板的特徵點
{
	size_t i=0;
	int    index=0;
	CAOIMark*  MarkPtr = NULL;
	std::vector<CAOIMark*> BoardMarkPtrList = m_BoardMarkPtrList;
	const size_t MarkCount = BoardMarkPtrList.size();

	index = 0;
	m_BoardMarkPtrList.clear();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = BoardMarkPtrList[i];
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkSelected() == true ) { continue; }
		
		MarkPtr->SetMarkIndex_Board(index);
		AddBoardMarkPtr_Inline(MarkPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::RemoveBoardAllMarks()//移除單板的特徵點
{
	m_BoardMarkPtrList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::LayoutBoardMarkList()//重整單板的特徵點列表
{
	size_t i=0;
	int    index=0;
	CAOIMark*  MarkPtr = NULL;
	std::vector<CAOIMark*> BoardMarkPtrList = m_BoardMarkPtrList;
	const size_t MarkCount = BoardMarkPtrList.size();

	index = 0;
	m_BoardMarkPtrList.clear();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = BoardMarkPtrList[i];
		if ( NULL == MarkPtr ) { continue; }		
		
		MarkPtr->SetMarkIndex_Board(index);
		AddBoardMarkPtr_Inline(MarkPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::GetBoardBarcodeCount() const//取得單板的軟體條碼數量
{
	return GetBoardBarcodeCount_Inline();
}
//-------------------------------------------------------------------------------------//
CAOIBarcode* CAOIBoard::GetBoardBarcodePtr(size_t index, bool check) const//取得單板的軟體條碼指標
{
	if ( check )
	{
		const size_t Count = GetBoardBarcodeCount_Inline();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return GetBoardBarcodePtr_Inline(index);	
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::AddBoardBarcodePtr(CAOIBarcode *BarcodePtr)//增加單板的軟體條碼
{
	const int SBIndex = (int)(GetBoardBarcodeCount_Inline());	
	BarcodePtr->SetBarcodePanelPtr(m_BoardPanelPtr);
	BarcodePtr->SetBarcodePanelIndex_Project(m_BoardPanelIndex_Project);
	
	BarcodePtr->SetBarcodeBoardPtr(this);
	BarcodePtr->SetBarcodeBoardIndex_Project(m_BoardIndex_Project);
	BarcodePtr->SetBarcodeBoardIndex_Panel(m_BoardIndex_Panel);
	BarcodePtr->SetBarcodeIndex_Board(SBIndex);	
	AddBoardBarcodePtr_Inline(BarcodePtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::SelectBoardAllBarcodes(bool Select)//選取單板的軟體條碼
{
	size_t i = 0;
	CAOIBarcode*  BarcodePtr = NULL;
	const size_t FdCount = GetBoardBarcodeCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		BarcodePtr = GetBoardBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		BarcodePtr->SetBarcodeSelected(Select);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::RemoveBoardBarcodeSelected()//移除選取到的單板的軟體條碼
{
	size_t i=0;
	int    index=0;
	CAOIBarcode*  BarcodePtr = NULL;;
	std::vector<CAOIBarcode*> BoardSBPtrList = m_BoardBarcodePtrList;
	const size_t BarcodeCount = BoardSBPtrList.size();

	index = 0;
	m_BoardBarcodePtrList.clear();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = BoardSBPtrList[i];
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeSelected() == true ) { continue; }
		
		BarcodePtr->SetBarcodeIndex_Board(index);
		AddBoardBarcodePtr_Inline(BarcodePtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::RemoveBoardAllBarcodes()//移除單板的軟體條碼
{
	m_BoardBarcodePtrList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::LayoutBoardBarcodeList()//重整單板的軟體條碼列表
{
	size_t i=0;
	int    index=0;
	CAOIBarcode*  BarcodePtr = NULL;;
	std::vector<CAOIBarcode*> BoardSBPtrList = m_BoardBarcodePtrList;
	const size_t BarcodeCount = BoardSBPtrList.size();

	index = 0;
	m_BoardBarcodePtrList.clear();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = BoardSBPtrList[i];
		if ( NULL == BarcodePtr ) { continue; }
		
		BarcodePtr->SetBarcodeIndex_Board(index);
		AddBoardBarcodePtr_Inline(BarcodePtr);		
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::AssignBoardFieldList(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID)//分配區域列表
{	
	size_t         i=0, j=0, k=0;	
	size_t         RgnIdx = 0;
	TREGION4D      Region, CameraRgn;		
	size_t         SubRgnCount = 0;
	size_t         WindowCount = 0;
	RECT           FrameRect={0};	
	IMAGE_SIZE     ImageW=0, ImageH=0;
	CAMERA_ID      CameraID;
	CAOIFd*        FdPtr = NULL;
	CAOIRgn*       RgnPtr = NULL;
	CAOIRgn*       SubRgnPtr = NULL;
	CAOIMark*      MarkPtr = NULL;
	CAOIField*     FieldPtr = NULL;		
	CAOIWindow*    WindowPtr = NULL;
	CAOIBarcode*   BarcodePtr = NULL;
	CAOIComponent* ComponentPtr = NULL;
	TSIZE2D        FrameSizeUm;
	TPOINT2D       StagePos2D;
	TPOINT3D       StagePos3D;	
	const int      nAlign = 4;
	const bool     CheckInner = false;
	const bool     ByCadRegion = true;
	BOARD_FD_GRAB_MODE BoardFdGrabMode = GetBoardBoardFdGrabMode();
	const size_t   FdCount = GetBoardFdCount_Inline();
	const size_t   MarkCount = GetBoardMarkCount_Inline();
	const size_t   FieldCount = GetBoardFieldCount_Inline();
	const size_t   BarcodeCount = GetBoardBarcodeCount_Inline();
	const size_t   ComponentCount = GetBoardComponentCount();	

	SetBoardErrorString(_T(""));
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetBoardFieldPtr_Inline(i);
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

	//Assign Field To Fd		
	if ( BOARD_FD_GRAB_INSPECTING==BoardFdGrabMode )
	{		
		const bool BoardMapEnable = GetBoardMapEnable();
		for ( i=0; i<FdCount; i++ )
		{
			FdPtr = GetBoardFdPtr_Inline(i);
			if ( NULL == FdPtr ) { continue; }			
			if ( false == BoardMapEnable ) {	continue;	}
			if ( FdPtr->GetFdNeedToCalculate() == false ) { continue; }
			if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }

			SetBoardErrorString(FdPtr->GetRgnDerivedName());
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
					FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
					if ( NULL == FieldPtr )
					{	return false;	}
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
				{	return false; }
				//是否計算四個端點呢!?
				AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
				SubRgnPtr->SetRgnFrameImageRect(FrameRect);		
				SubRgnPtr->SetRgnFrameImageSize_um(FrameSizeUm);
			}

			FieldPtr = FdPtr->GetFdFieldPtr();
			if ( NULL == FieldPtr )
			{	
				if ( true == ByCadRegion )
				{	FdPtr->GetFdRoiCadRegion(Region); }
				else
				{	FdPtr->GetFdRoiStageRegion(Region); }
				FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{	return false;	}
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
				{	return false; }
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

	//Assign Filed To Mark
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }		
		if ( MarkPtr->GetMarkNeedToCalculate() == false ) { continue; }
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		
		SetBoardErrorString(MarkPtr->GetRgnDerivedName());
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
				FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{	return false;	}
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
			{	return false; }
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
			FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
			if ( NULL == FieldPtr )
			{	return false;	}
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
			{	return false; }
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
		BarcodePtr = GetBoardBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		if ( BarcodePtr->GetBarcodeNeedToCalculate() == false ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		
		SetBoardErrorString(BarcodePtr->GetRgnDerivedName());
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
				FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{	return false;	}
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
			{	return false; }
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
			FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
			if ( NULL == FieldPtr )
			{	return false;	}
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
			{	return false; }
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
		ComponentPtr = GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }			
		if ( ComponentPtr->GetComponentNeedToCalculate() == false ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		
		SetBoardErrorString(ComponentPtr->GetRgnDerivedName());
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
					FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
					if ( NULL == FieldPtr )
					{	return false;	}
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
				{	return false; }
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
				FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{	return false;	}
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
				{	return false; }
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
				FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{	return false;	}
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
			{	return false; }
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
			FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
			if ( NULL == FieldPtr )
			{	return false;	}
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
			{	return false; }
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
bool CAOIBoard::CreateBoardFieldList(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, std::vector<CAOIField*> &FieldPtrList, FIELD_DIVISION_MODE DivMode, bool ByCadRegion)//建立整板區域列表
{
	TJetRgnList    RgnList;
	TJetFieldList  FieldList;
	const bool     CheckOldField = true;	
	FIELD_DIVISION_MODE DivisionMode=DivMode;
	CAOIProject   *ProjectPtr = GetBoardProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	if ( CreateBoardFieldListKernel(BuildMode, DistrictID, DivisionMode, ByCadRegion, CheckOldField, RgnList, FieldList) == false )
	{	return false; }	
	
	//Create CAOIField Obj and Assign Index	
	size_t         i=0, j=0, k=0;	
	size_t         RgnIdx = 0;
	CAMERA_ID      CameraID;
	size_t         RgnCount = 0;
	size_t         SubRgnCount = 0;
	RECT           FrameRect={0};	
	AOI_OBJ_TYPE   AOIType;
	CAOIRgn*       RgnPtr = NULL;
	CAOIField*     FieldPtr = NULL;
	TJetRgn*       JetRgnPtr = NULL;
	TJetField*     JetFieldPtr = NULL;
	CAOIBarcode*   BarcodePtr = NULL;
	CAOIWindow*    WindowPtr = NULL;
	CAOIComponent* ComponentPtr = NULL;
	TSIZE2D        FrameSizeUm;
	TSIZE2D        FovSizeReal;
	TSIZE2D        FovSizeInner;
	TSIZE2D        FovSizeOuter;
	TPOINT2D       CadPos, StagePos2D;
	TPOINT3D       StagePos;	
	TREGION4D      Region, CameraRgn;
	TREGION4D      rgnFOVCad, rgnFOVStage;
	const int      nAlign = 4;
	const size_t   JetRgnCount = RgnList.size();
	const size_t   JetFieldCount = FieldList.size();

	AOIDataCollect.GetFovSizeReal(FovSizeReal.cx, FovSizeReal.cy);
	AOIDataCollect.GetFovSizeInner(FovSizeInner.cx, FovSizeInner.cy);
	AOIDataCollect.GetFovSizeOuter(FovSizeOuter.cx, FovSizeOuter.cy);
	ProjectPtr->ModifyProjectFieldSize(FovSizeInner.cx, FovSizeInner.cy);
	
	CMapCoordinate PanelMapCTS;
	CMapCoordinate PanelMapSTC;
	const double FOVW2 = FovSizeInner.cx/2;
	const double FOVH2 = FovSizeInner.cy/2;		
	if ( GetBoardMapCTS(DistrictID, PanelMapCTS) == false ) { return false; }
	if ( GetBoardMapSTC(DistrictID, PanelMapSTC) == false ) { return false; }

	for ( i=0; i<JetFieldCount; i++ )
	{
		JetFieldPtr = &(FieldList[i]);

		FieldPtr = AOIObjManager.CreateFieldObj();
		if ( FieldPtr == NULL ) { continue; }

		JetFieldPtr->Ptr = FieldPtr;

		CadPos.x = JetFieldPtr->PosCadX;
		CadPos.y = JetFieldPtr->PosCadY;
		PanelMapCTS.Map2D(CadPos.x, CadPos.y, StagePos.x, StagePos.y);		
		FieldPtr->SetFieldCadPos(CadPos);
		FieldPtr->SetFieldStagePos(StagePos);
		FieldPtr->SetFieldDistrictID(DistrictID);
		FieldPtr->SetFieldSize_Real(FovSizeReal);
		FieldPtr->SetFieldSize_Inner(FovSizeInner);
		FieldPtr->SetFieldSize_Outer(FovSizeOuter);		
		FieldPtrList.push_back(FieldPtr);

		StagePos2D.x = StagePos.x;
		StagePos2D.y = StagePos.y;
		rgnFOVStage.minX = StagePos.x-FOVW2;
		rgnFOVStage.minY = StagePos.y-FOVH2;
		rgnFOVStage.maxX = StagePos.x+FOVW2;
		rgnFOVStage.maxY = StagePos.y+FOVH2;
		//將CAOIField指標設定給予檢測區域內的指標
		RgnCount = JetFieldPtr->RgnIdxList.size();
		for ( j=0; j<RgnCount; j++ )
		{
			RgnIdx = JetFieldPtr->RgnIdxList[j];
			if ( RgnIdx >= JetRgnCount )
			{	continue;	}
			JetRgnPtr = &(RgnList[RgnIdx]);
			if ( NULL == JetRgnPtr->Ptr ) { continue; }
			RgnPtr = (CAOIRgn*)JetRgnPtr->Ptr;
			RgnPtr->SetRgnFieldPtr(FieldPtr);

			AOIType = RgnPtr->GetObjType();
			switch ( AOIType )
			{
			case AOI_OBJ_FD:
				break;			
			case AOI_OBJ_RGN:
				break;
			case AOI_OBJ_WINDOW:
				break;
			case AOI_OBJ_BARCODE:
				break;
			case AOI_OBJ_COMPONENT:
				break;
			}

			//設定區域影像位置
			CameraID = PRIMARY_CAMERA_ID;			
			SubRgnCount = RgnPtr->GetRgnSubRgnCount();
			RgnPtr->GetRgnRoiStageRegion(Region);
			if ( SubRgnCount > 0 )			
			{
				if ( Region.minX < rgnFOVStage.minX ) { Region.minX = rgnFOVStage.minX; }
				if ( Region.minY < rgnFOVStage.minY ) { Region.minY = rgnFOVStage.minY; }
				if ( Region.maxX > rgnFOVStage.maxX ) { Region.maxX = rgnFOVStage.maxX; }
				if ( Region.maxY > rgnFOVStage.maxY ) { Region.maxY = rgnFOVStage.maxY; }
			}
			AOIDataCollect.MapStageRegionToCamera(CameraID, Region, StagePos2D, CameraRgn);
			JetAPI::Region4DToRect(CameraRgn, FrameRect, true);
			JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬
			//是否計算四個端點呢!?
			AOIDataCollect.MapImageSizeToReal(CameraID, FrameRect, FrameSizeUm);
			RgnPtr->SetRgnFrameImageRect(FrameRect);			
			RgnPtr->SetRgnFrameImageSize_um(FrameSizeUm);
		}
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::GetBoardFieldCount() const//取得單板的區域數量
{
	return GetBoardFieldCount_Inline();
}
//-------------------------------------------------------------------------------------//
CAOIField* CAOIBoard::GetBoardFieldPtr(size_t index, bool check) const//取得單板的區域指標
{
	if ( check )
	{
		const size_t Count = GetBoardFieldCount_Inline();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return GetBoardFieldPtr_Inline(index);	
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::AddBoardFieldPtr(CAOIField *FieldPtr)//增加單板的區域
{
	AddBoardFieldPtr_Inline(FieldPtr);	
	FieldPtr->SetFieldBoardPtr(this);	
	FieldPtr->SetFieldBoardIndex(GetBoardIndex_Project());
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::SelectBoardAllFields(bool Select)//選取單板的區域
{
	size_t i = 0;
	CAOIField*  FieldPtr = NULL;
	const size_t FieldCount = GetBoardFieldCount_Inline();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetBoardFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		FieldPtr->SetFieldSelected(Select);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::RemoveBoardFieldSelected()//移除選取到的單板的區域
{
	size_t i = 0;
	int    index = 0;
	CAOIField*  FieldPtr = NULL;
	std::vector<CAOIField*> TmpFieldPtrList = m_BoardFieldPtrList;
	const size_t FieldCount = TmpFieldPtrList.size();

	index = 0;
	m_BoardFieldPtrList.clear();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = TmpFieldPtrList[i];
		if ( NULL == FieldPtr ) { continue; }
		if ( FieldPtr->GetFieldSelected() == true ) { continue; }
		AddBoardFieldPtr_Inline(FieldPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::RemoveBoardAllFields()//移除單板的區域
{
	m_BoardFieldPtrList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::RemoveBoardAllFields(DISTRICT_ID DistrictID)//移除單板的區域	
{
	size_t       i=0;
	CAOIField   *FieldPtr = NULL;
	std::vector<CAOIField*>   FieldPtrList;
	const size_t FieldCount = GetBoardFieldCount_Inline();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetBoardFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		if ( DistrictID == FieldPtr->GetFieldDistrictID() ) { continue; }
		FieldPtrList.push_back(FieldPtr);
	}
	m_BoardFieldPtrList = FieldPtrList;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::LayoutBoardFieldList()//重整單板的區域列表
{
	size_t i = 0;
	int    index = 0;
	CAOIField*  FieldPtr = NULL;
	std::vector<CAOIField*> TmpFieldPtrList = m_BoardFieldPtrList;
	const size_t FieldCount = TmpFieldPtrList.size();

	index = 0;
	m_BoardFieldPtrList.clear();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = TmpFieldPtrList[i];
		if ( NULL == FieldPtr ) { continue; }
		AddBoardFieldPtr_Inline(FieldPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::CalcBoardLastFieldGrabIndex(bool FdFirst) const//計算單板最後區域取像的引數
{	
	size_t         i = 0;	
	bool           First=true;
	bool           MapEnable = GetBoardMapEnable();
	size_t         LastGrabIndex = 0;	
	size_t         FieldGrabIndex = 0;
	CAOIFd         *FdPtr = NULL;
	CAOIMark       *MarkPtr = NULL;	
	CAOIField      *FieldPtr = NULL;	
	CAOIBarcode    *BarcodePtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;	
	const size_t FdCount = GetBoardFdCount_Inline();
	const size_t MarkCount = GetBoardMarkCount_Inline();
	const size_t BarcodeCount = GetBoardBarcodeCount_Inline();
	const size_t ComponentCount = GetBoardComponentCount_Inline();
	
	First=true;
	LastGrabIndex = 0;
	if ( true == MapEnable )
	{
		for ( i=0; i<FdCount; i++ )
		{
			FdPtr = GetBoardFdPtr_Inline(i);
			if ( NULL == FdPtr ) { continue; }
			FieldPtr = FdPtr->GetFdFieldPtr();
			if ( NULL == FieldPtr ) { continue; }		
			FieldGrabIndex = FieldPtr->GetFieldGrabIndex();
			if ( true == First )
			{
				First = false;
				LastGrabIndex = FieldGrabIndex;	
			}
			else
			{
				if ( LastGrabIndex < FieldGrabIndex ) 
				{	LastGrabIndex = FieldGrabIndex;	}
			}
		}
	}
	if ( true == FdFirst )
	{
		if ( false == First ) 
		{	return LastGrabIndex; }
	}

	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		FieldPtr = MarkPtr->GetMarkFieldPtr();
		if ( NULL == FieldPtr ) { continue; }		
		FieldGrabIndex = FieldPtr->GetFieldGrabIndex();
		if ( true == First )
		{
			First = false;
			LastGrabIndex = FieldGrabIndex;	
		}
		else
		{
			if ( LastGrabIndex < FieldGrabIndex ) 
			{	LastGrabIndex = FieldGrabIndex;	}
		}
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetBoardBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		FieldPtr = BarcodePtr->GetBarcodeFieldPtr();
		if ( NULL == FieldPtr ) { continue; }		
		FieldGrabIndex = FieldPtr->GetFieldGrabIndex();
		if ( true == First )
		{
			First = false;
			LastGrabIndex = FieldGrabIndex;	
		}
		else
		{
			if ( LastGrabIndex < FieldGrabIndex ) 
			{	LastGrabIndex = FieldGrabIndex;	}
		}
	}

	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		FieldPtr = ComponentPtr->GetComponentFieldPtr();
		if ( NULL == FieldPtr ) { continue; }		
		FieldGrabIndex = FieldPtr->GetFieldGrabIndex();
		if ( true == First )
		{
			First = false;
			LastGrabIndex = FieldGrabIndex;	
		}
		else
		{
			if ( LastGrabIndex < FieldGrabIndex ) 
			{	LastGrabIndex = FieldGrabIndex;	}
		}
	}
	if ( true == First )
	{	return -1; }
	return LastGrabIndex;
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::GetBoardRgnTempCount() const//取得單板的暫時檢測區域數量
{
	return m_BoardRgnPtrListTemp.size();
}
//-------------------------------------------------------------------------------------//
CAOIRgn* CAOIBoard::GetBoardRgnTempPtr(size_t index, bool check) const//取得單板的暫時檢測區域指標
{
	if ( true == check )
	{
		const size_t count = m_BoardRgnPtrListTemp.size();
		if ( index >= count ) 
		{	return NULL; }
	}
	return m_BoardRgnPtrListTemp[index];
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::AddBoardRgnTempPtr(CAOIRgn *RgnPtr, bool chkExist)//增加單板的暫時檢測區域
{
	if ( true == chkExist )
	{
		size_t i=0;
		const size_t count = m_BoardRgnPtrListTemp.size();
		for ( i=0; i<count; i++ )
		{
			if ( RgnPtr == m_BoardRgnPtrListTemp[i] ) { return true; }
		}
	}
	m_BoardRgnPtrListTemp.push_back(RgnPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::ClearBoardRgnTempList()//移除單板的暫時檢測區域	
{
	m_BoardRgnPtrListTemp.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::LayoutBoardRgnTempList()//排列單板的暫時檢測區域
{	
	size_t       i=0, j=0;
	size_t       FieldRgnCount=0;
	AOI_OBJ_TYPE ObjType;
	CAOIRgn     *RgnPtr1 = NULL;
	CAOIRgn     *RgnPtr2 = NULL;
	CAOIBoard   *BoardPtr = NULL;
	CAOIField   *FieldPtr = NULL;
	std::vector<CAOIRgn*> TempRgnList;
	const size_t FieldCount = GetBoardFieldTempCount();
	ClearBoardRgnTempList();

	if ( 0 == FieldCount ) { return true; }
	//避免後面重複迴圈, 先行放入一個列表內
	TempRgnList.clear();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = CAOIBoard::GetBoardFieldTempPtr(i, false);
		if ( NULL == FieldPtr ) { continue; }
		FieldRgnCount = FieldPtr->GetFieldRgnPtrCount();
		for ( j=0; j<FieldRgnCount; j++ )
		{
			RgnPtr1 = FieldPtr->GetFieldRgnPtr(j, false);
			if ( NULL == RgnPtr1 ) { continue; }
			if ( RgnPtr1->GetRgnTempInt() == FN_ENABLE ) { continue; }
			TempRgnList.push_back(RgnPtr1);
		}
	}
	const size_t TempRgnCount = TempRgnList.size();
	//先計算單板內定位點
	for ( i=0; i<TempRgnCount; i++ )
	{
		RgnPtr1 = TempRgnList[i];
		if ( NULL == RgnPtr1 ) { continue; }
		if ( RgnPtr1->GetRgnTempInt() == FN_ENABLE ) { continue; }

		RgnPtr2 = RgnPtr1->GetRgnParent();
		if ( NULL != RgnPtr2 )
		{	
			ObjType = RgnPtr2->GetObjType();	
			BoardPtr = RgnPtr2->GetRgnBoardPtr();
		}
		else
		{	
			ObjType = RgnPtr1->GetObjType();	
			BoardPtr = RgnPtr1->GetRgnBoardPtr();
		}
		if ( AOI_OBJ_FD != ObjType ) { continue; }
		if ( this != BoardPtr ) { continue; }//同一個單板內的定位點
		RgnPtr1->SetRgnTempInt(FN_ENABLE);
		CAOIBoard::AddBoardRgnTempPtr(RgnPtr1, false);		
	}	

	//再放剩餘的同單板內的檢測數	
	for ( i=0; i<TempRgnCount; i++ )
	{
		RgnPtr1 = TempRgnList[i];
		if ( NULL == RgnPtr1 ) { continue; }
		if ( RgnPtr1->GetRgnTempInt() == FN_ENABLE ) { continue; }

		RgnPtr2 = RgnPtr1->GetRgnParent();
		if ( NULL != RgnPtr2 )
		{	
			ObjType = RgnPtr2->GetObjType();	
			BoardPtr = RgnPtr2->GetRgnBoardPtr();
		}
		else
		{	
			ObjType = RgnPtr1->GetObjType();	
			BoardPtr = RgnPtr1->GetRgnBoardPtr();
		}		
		if ( NULL != BoardPtr )
		{	//不同單板內的檢測區域
			if ( this != BoardPtr ) { continue; }
		}
		RgnPtr1->SetRgnTempInt(FN_ENABLE);
		CAOIBoard::AddBoardRgnTempPtr(RgnPtr1, false);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::GetBoardFieldTempCount() const//取得單板的暫時區域數量
{
	return m_BoardFieldPtrListTemp.size();
}
//-------------------------------------------------------------------------------------//
CAOIField* CAOIBoard::GetBoardFieldTempPtr(size_t index, bool check) const//取得單板的暫時區域指標
{
	if ( true == check )
	{
		const size_t count = m_BoardFieldPtrListTemp.size();
		if ( index >= count ) 
		{	return NULL; }
	}
	return m_BoardFieldPtrListTemp[index];
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::AddBoardFieldTempPtr(CAOIField *FieldPtr, bool chkExist)//增加單板的暫時區域
{
	if ( true == chkExist )
	{
		size_t i=0;
		const size_t count = m_BoardFieldPtrListTemp.size();
		for ( i=0; i<count; i++ )
		{
			if ( FieldPtr == m_BoardFieldPtrListTemp[i] ) { return true; }
		}
	}
	m_BoardFieldPtrListTemp.push_back(FieldPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::ClearBoardFieldTempList()//移除單板的暫時區域	
{
	m_BoardFieldPtrListTemp.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
std::vector<CAOIComponent*>& CAOIBoard::GetBoardComponentPtrTempList()//取得單板暫時的零件列表	
{
	return m_BoardComponentPtrListTemp;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::ClearBoardComponentPtrTempList()//清除單板暫時的零件列表	
{
	m_BoardComponentPtrListTemp.clear();
	return true;
}
//-------------------------------------------------------------------------------------//	
size_t CAOIBoard::GetBoardComponentPtrTempCount() const//取得單板暫時的零件數量
{
	return m_BoardComponentPtrListTemp.size();
}
//-------------------------------------------------------------------------------------//	
CAOIComponent* CAOIBoard::GetBoardComponentPtrTempPtr(size_t index, bool check)//取得單板暫時的零件指標
{
	if ( true == check )
	{
		const size_t count=GetBoardComponentPtrTempCount();
		if ( index >= count )
		{	return NULL; }
	}
	return m_BoardComponentPtrListTemp[index];
}
//-------------------------------------------------------------------------------------//	
bool CAOIBoard::SetBoardComponentPtrTempPtr(size_t index, CAOIComponent *Ptr)//設定單板暫時的零件指標
{
	if ( index >= GetBoardComponentPtrTempCount() ) { return false; }
	m_BoardComponentPtrListTemp[index]=Ptr;
	return true;
}
//-------------------------------------------------------------------------------------//	
void CAOIBoard::SetBoardComponentPtrTempList(const std::vector<CAOIComponent*> &List)//設定單板暫時的零件列表
{
	m_BoardComponentPtrListTemp = List;
}
//-------------------------------------------------------------------------------------//	
size_t CAOIBoard::GetBoardComponentCount() const//取得單板的零件數量
{
	return GetBoardComponentCount_Inline();
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::GetBoardComponentNGCount() const//取得單板的瑕疵零件數量
{
	std::vector<CAOIComponent*> List;
	if ( GetBoardComponentNGList(List) == false )
	{	return 0; }
	return List.size();
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::GetBoardComponentNGList(std::vector<CAOIComponent*> &List) const//取得單板的瑕疵零件列表
{
	size_t i=0;
	size_t ComponentCountNG = 0;
	RESULT_ID ResultID=RESULT_ID_NONE;	
	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = GetBoardComponentCount();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }
		ResultID = ComponentPtr->CheckComponentResultID_AOI();
		if ( RESULT_ID_NONE == ResultID ) { continue; }
		if ( RESULT_ID_OK == ResultID ) { continue; }		
		if ( RESULT_ID_SKIP == ResultID ) { continue; }
		if ( RESULT_ID_BYPASS == ResultID ) { continue; }
	    List.push_back(ComponentPtr);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::GetBoardComponentNotAgentCount() const//取得單板的非代理零件數量	
{
	size_t i=0;
	size_t Count = 0;	
	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = GetBoardComponentCount();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }		
		Count ++;
	}
	return Count;
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::CalcBoardComponentBypassCount() const//計算單板的不檢測零件數量
{
	size_t i=0;
	size_t ComponentCountBypass = 0;	
	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = GetBoardComponentCount();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }		
		if ( ComponentPtr->GetComponentBypassed() == false ) { continue; }
		ComponentCountBypass ++;
	}
	return ComponentCountBypass;
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::CalcBoardComponentXBoardUnitCount() const//計算單板的報廢板零件數量
{
	size_t i=0;
	size_t ComponentCountXBoardUnit = 0;	
	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = GetBoardComponentCount();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }		
		if ( ComponentPtr->GetComponentXBoardUnit() == false ) { continue; }
		ComponentCountXBoardUnit ++;
	}
	return ComponentCountXBoardUnit;
}
//-------------------------------------------------------------------------------------//
CAOIComponent* CAOIBoard::GetBoardComponentPtr(size_t index, bool check) const//取得單板的零件指標
{
	if ( check )
	{
		const size_t Count = CAOIBoard::GetBoardComponentCount_Inline();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return CAOIBoard::GetBoardComponentPtr_Inline(index);	
}
//-------------------------------------------------------------------------------------//
CAOIComponent* CAOIBoard::GetBoardComponentPtrByName(LPCSTR ComponentName) const//取得單板的零件指標
{
	return GetBoardComponentPtrByName(ComponentName, GetBoardComponentCount());
}
//-------------------------------------------------------------------------------------//
CAOIComponent* CAOIBoard::GetBoardComponentPtrByName(LPCWSTR ComponentName) const//取得單板的零件指標
{
	return GetBoardComponentPtrByName(ComponentName, GetBoardComponentCount());
}
//-------------------------------------------------------------------------------------//
CAOIComponent* CAOIBoard::GetBoardComponentPtrByName(LPCSTR ComponentName, size_t Count) const//取得單板的零件指標
{
	size_t          i = 0;
	CString         strComponentName;
	CString         refComponentName = ComponentName;
	CAOIComponent*  ComponentPtr = NULL;
	const size_t ComponentCount = MIN(Count, GetBoardComponentCount_Inline());
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		strComponentName = ComponentPtr->GetComponentName();
		if ( strComponentName.CompareNoCase(refComponentName) == 0 ) 
		{	return ComponentPtr; }
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
CAOIComponent* CAOIBoard::GetBoardComponentPtrByName(LPCWSTR ComponentName, size_t Count) const//取得單板的零件指標
{
	size_t          i = 0;
	CString         strComponentName;
	CString         refComponentName = ComponentName;
	CAOIComponent*  ComponentPtr = NULL;
	const size_t ComponentCount = MIN(Count, GetBoardComponentCount_Inline());
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		strComponentName = ComponentPtr->GetComponentName();
		if ( strComponentName.CompareNoCase(refComponentName) == 0 ) 
		{	return ComponentPtr; }
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::GetBoardComponentFreeNameIdx(LPCTSTR BaseName) const//取得單板的零件無使用的引數
{	
	size_t MaxNameIdx = 0;
	size_t FreeNameIdx= 0;
	if ( GetBoardComponentMaxFreeNameIdx(BaseName, MaxNameIdx, FreeNameIdx) == false )
	{	return 0; }
	return FreeNameIdx;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::GetBoardComponentMaxFreeNameIdx(LPCTSTR BaseName, size_t &MaxIdx, size_t &FreeIdx) const//取得單板的零件最大名稱引數
{
	size_t          i = 0;
	size_t          NameIndex=-1;
	CString         strBaseName;
	CString         strComponentName;
	CString         strComponentNameOrg;
	CString         refBaseName = BaseName;
	CAOIComponent*  ComponentPtr = NULL;
	std::map<size_t, bool> IdxUsedMap;
	const size_t ComponentCount = GetBoardComponentCount_Inline();
	MaxIdx = 0;
	FreeIdx = 0;	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		strComponentNameOrg = ComponentPtr->GetComponentName();
		if ( JetAPI::ExtractComponentBaseNameIndex(strComponentNameOrg, strBaseName, NameIndex) == false )
		{	continue; }
		if ( strBaseName != refBaseName )
		{	continue; }						
		if ( MaxIdx < NameIndex )
		{	MaxIdx = NameIndex;	}
		IdxUsedMap[NameIndex]=true;		
	}	
	if ( IdxUsedMap.empty() == false )	
	{
		for ( size_t i=0; i<=MaxIdx+1; i++ )
		{
			auto iter = IdxUsedMap.find(i);
			if ( iter == IdxUsedMap.end() )
			{
				FreeIdx = i;
				break;
			}
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::AddBoardComponentList(std::vector<CAOIComponent*> &List)//加入單板的零件列表
{
	size_t i=0;	
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	AddBoardComponentPtr(List[i]);	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::CloneBoardComponentList(std::vector<CAOIComponent*> &List) const//複製單板的零件列表
{
	List = m_BoardComponentPtrList;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::AddBoardComponentPtr(CAOIComponent *ComponentPtr)//增加單板的零件
{
	const int ComponentIndex = (int)(CAOIBoard::GetBoardComponentCount_Inline());	
	ComponentPtr->SetComponentPanelPtr(this->m_BoardPanelPtr);
	ComponentPtr->SetComponentPanelIndex_Project(this->m_BoardPanelIndex_Project);
	
	ComponentPtr->SetComponentBoardPtr(this);
	ComponentPtr->SetComponentBoardIndex_Project(this->m_BoardIndex_Project);
	ComponentPtr->SetComponentBoardIndex_Panel(this->m_BoardIndex_Panel);
	ComponentPtr->SetComponentIndex_Board(ComponentIndex);	
	AddBoardComponentPtr_Inline(ComponentPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::SelectBoardAllComponents(bool Select)//選取單板的零件
{
	size_t i = 0;
	CAOIComponent*  ComponentPtr = NULL;
	const size_t ComponentCount = CAOIBoard::GetBoardComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = CAOIBoard::GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentSelected(Select);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::RemoveBoardComponentSelected()//移除選取到的單板的零件
{
	size_t i=0;
	int    index=0;
	CAOIComponent*  ComponentPtr = NULL;
	std::vector<CAOIComponent*> BoardComponentPtrList = this->m_BoardComponentPtrList;
	const size_t ComponentCount = BoardComponentPtrList.size();

	index = 0;
	this->m_BoardComponentPtrList.clear();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = BoardComponentPtrList[i];
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == true ) { continue; }
		
		ComponentPtr->SetComponentIndex_Board(index);
		CAOIBoard::AddBoardComponentPtr_Inline(ComponentPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::RemoveBoardAllComponents()//移除單板的零件
{
	this->m_BoardComponentPtrList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::LayoutBoardComponentList()//重整單板的零件列表
{
	size_t       i = 0;
	unsigned int index=0;
	CAOIComponent*  ComponentPtr = NULL;
	std::vector<CAOIComponent*> BoardComponentPtrList = this->m_BoardComponentPtrList;
	const size_t ComponentCount = BoardComponentPtrList.size();

	index = 0;
	this->m_BoardComponentPtrList.clear();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = BoardComponentPtrList[i];
		if ( NULL == ComponentPtr ) { continue; }		
		
		ComponentPtr->SetComponentIndex_Board(index);
		CAOIBoard::AddBoardComponentPtr_Inline(ComponentPtr);
		index ++;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::InvertSelectBoardComponent()//反向選取零件
{
	size_t          i = 0;
	bool            Selected = false;
	CAOIComponent*  ComponentPtr = NULL;
	const size_t ComponentCount = CAOIBoard::GetBoardComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = CAOIBoard::GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == true ) 
		{	Selected = false; }
		else
		{	Selected = true; }
		ComponentPtr->SetComponentSelected(Selected);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::ChceckBoardComponentNameExist(LPCSTR ComponentName)//確認單板零件名稱存在
{
	CAOIComponent*  ComponentPtr = NULL;
	ComponentPtr = GetBoardComponentPtrByName(ComponentName);
	if ( NULL == ComponentPtr ) 
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::ChceckBoardComponentNameExist(LPCWSTR ComponentName)//確認單板零件名稱存在
{
	CAOIComponent*  ComponentPtr = NULL;
	ComponentPtr = GetBoardComponentPtrByName(ComponentName);
	if ( NULL == ComponentPtr ) 
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::ChceckBoardComponentNameExist(LPCSTR ComponentName, size_t Count)//確認單板零件名稱存在
{
	CAOIComponent*  ComponentPtr = NULL;
	ComponentPtr = GetBoardComponentPtrByName(ComponentName, Count);
	if ( NULL == ComponentPtr ) 
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::ChceckBoardComponentNameExist(LPCWSTR ComponentName, size_t Count)//確認單板零件名稱存在
{
	CAOIComponent*  ComponentPtr = NULL;
	ComponentPtr = GetBoardComponentPtrByName(ComponentName, Count);
	if ( NULL == ComponentPtr ) 
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::GetBoardPartGroupCount() const//取得單板的零件群組數量
{
	return GetBoardPartGroupCount_Inline();
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::AddBoardPartGroupPtr(CAOIPartGroup *PartGroupPtr)//增加單板的零件群組
{
	AddBoardPartGroupPtr_Inline(PartGroupPtr);
}
//-------------------------------------------------------------------------------------//
CAOIPartGroup* CAOIBoard::GetBoardPartGroupPtr(size_t index, bool check) const//取得單板的零件群組指標	
{
	if ( true == check )
	{
		if ( index >= m_BoardPartGroupPtrList.size() )
		{	return NULL; }
	}
	return GetBoardPartGroupPtr_Inline(index);
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::RemoveBoardAllPartGroups()//移除單板的零件群組
{
	m_BoardPartGroupPtrList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardCalcMapFinish(bool value) 
{ 
	m_BoardCalcMapFinish = value; 
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::GetBoardCalcMapFinish() const 
{ 
	return m_BoardCalcMapFinish; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardMapEnable(bool value) 
{ 
	m_BoardMapEnable = value; 
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::GetBoardMapEnable() const 
{ 
	return m_BoardMapEnable; 
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::BuildBoardDefaultMap(DISTRICT_ID DistrictID)//建立整板基本座標轉換參數	
{	
	double CadPosX[4]={0};
	double CadPosY[4]={0};
	double StagePosX[4]={0};
	double StagePosY[4]={0};	
	TREGION4D BoardRgnCad = GetBoardRgnCad();
	TREGION4D BoardRgnStage = GetBoardRgnStage(DistrictID);

	CadPosX[0] = BoardRgnCad.GetCpX();
	CadPosY[0] = BoardRgnCad.GetCpY();
	StagePosX[0] = BoardRgnStage.GetCpX();
	StagePosY[0] = BoardRgnStage.GetCpY();
	switch ( DistrictID )
	{
	case DISTRICT_ID_A:
		m_BoardMapCTS.CalcMatrix2D(CadPosX, CadPosY, StagePosX, StagePosY, 1);
		m_BoardMapSTC.CalcMatrix2D(StagePosX, StagePosY, CadPosX, CadPosY, 1);
		break;
	case DISTRICT_ID_B:
		m_BoardMapCTS_DB.CalcMatrix2D(CadPosX, CadPosY, StagePosX, StagePosY, 1);
		m_BoardMapSTC_DB.CalcMatrix2D(StagePosX, StagePosY, CadPosX, CadPosY, 1);	
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CMapCoordinate* CAOIBoard::GetBoardMapCTSPtr(DISTRICT_ID DistrictID)//整板的座標轉換-Cad to Stage
{
	if ( DISTRICT_ID_A == DistrictID ) 
	{	return &m_BoardMapCTS;	}
	if ( DISTRICT_ID_B == DistrictID ) 
	{	return &m_BoardMapCTS_DB;	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
CMapCoordinate* CAOIBoard::GetBoardMapSTCPtr(DISTRICT_ID DistrictID)//整板的座標轉換-Stage to Cad
{
	if ( DISTRICT_ID_A == DistrictID ) 
	{	return &m_BoardMapSTC;	}
	if ( DISTRICT_ID_B == DistrictID ) 
	{	return &m_BoardMapSTC_DB;	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
bool  CAOIBoard::GetBoardMapCTS(DISTRICT_ID DistrictID, CMapCoordinate &Map)//整板的座標轉換-Cad to Stage
{
	if ( DISTRICT_ID_A == DistrictID ) 
	{
		Map = m_BoardMapCTS;
		return true;
	}
	if ( DISTRICT_ID_B == DistrictID ) 
	{
		Map = m_BoardMapCTS_DB;
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::GetBoardMapSTC(DISTRICT_ID DistrictID, CMapCoordinate &Map)//整板的座標轉換-Stage to Cad
{
	if ( DISTRICT_ID_A == DistrictID ) 
	{
		Map = m_BoardMapSTC;
		return true;
	}
	if ( DISTRICT_ID_B == DistrictID ) 
	{
		Map = m_BoardMapSTC_DB;
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::SetBoardMapCTS(DISTRICT_ID DistrictID, CMapCoordinate *MapPtr)
{
	if ( NULL == MapPtr ) { return false; }
	if ( DISTRICT_ID_A == DistrictID ) 
	{
		m_BoardMapCTS = *MapPtr;
		return true;
	}
	if ( DISTRICT_ID_B == DistrictID ) 
	{
		m_BoardMapCTS_DB = *MapPtr;
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::SetBoardMapSTC(DISTRICT_ID DistrictID, CMapCoordinate *MapPtr)
{
	if ( NULL == MapPtr ) { return false; }
	if ( DISTRICT_ID_A == DistrictID ) 
	{
		m_BoardMapSTC = *MapPtr;
		return true;
	}
	if ( DISTRICT_ID_B == DistrictID ) 
	{
		m_BoardMapSTC_DB = *MapPtr;
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CheckBoardMapCoordinate()//確認整板的座標轉換機制
{
	if ( CheckBoardMapCoordinate(DISTRICT_ID_A) == false ) 
	{	return false; }
	if ( GetBoardMultiDistrictMode() )
	{
		if ( CheckBoardMapCoordinate(DISTRICT_ID_B) == false ) 
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CheckBoardMapCoordinate(DISTRICT_ID DistrictID)//確認整板的座標轉換機制
{
	size_t i = 0;	
	CMapCoordinate MapCTS;
	CMapCoordinate MapSTC;
	TPOINT2D CadPos1, StagePos1;
	TPOINT2D CadPos2, StagePos2;
	TPOINT2D CadPosDif, StagePosDif;	
	const double Precision = DBL_PRECISION;
	CAOIComponent *pComponent = NULL;
	const size_t ComponentCount = GetBoardComponentCount_Inline();	

	if ( GetBoardMapCTS(DistrictID, MapCTS) == false ) { return false; }
	if ( GetBoardMapSTC(DistrictID, MapSTC) == false ) { return false; }
	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetBoardComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }
		if ( DistrictID != pComponent->GetComponentDistrictID() ) { continue; }

		CadPos1.x = pComponent->GetComponentCadPosX();
		CadPos1.y = pComponent->GetComponentCadPosY();
		StagePos1.x = pComponent->GetComponentStagePosX();
		StagePos1.y = pComponent->GetComponentStagePosY();

		MapCTS.Map2D(CadPos1.x, CadPos1.y, StagePos2.x, StagePos2.y);
		MapSTC.Map2D(StagePos1.x, StagePos1.y, CadPos2.x, CadPos2.y);

		CadPosDif.x = ::fabs(CadPos2.x-CadPos1.x);
		CadPosDif.y = ::fabs(CadPos2.y-CadPos1.y);
		StagePosDif.x = ::fabs(StagePos2.x-StagePos1.x);
		StagePosDif.y = ::fabs(StagePos2.y-StagePos1.y);

		if ( CadPosDif.x>Precision || CadPosDif.y > Precision )
		{	break;	}
		if ( StagePosDif.x>Precision || StagePosDif.y > Precision )
		{	break; }
	}
	if ( ComponentCount != i )
	{
		JetAPI::ShowMessageBox(_T("Error, Board Coordinate Transformation Check Exception"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::SpinBoard(double Angle)//自轉單板
{
	double CadAngle = 0;
	double StageAngle = 0;
	CadAngle = Angle;
	AOIDataCollect.MapCadAngleToStage(CadAngle, StageAngle);
	LayoutBoardRegionCad();	
	TREGION4D BoardRgnCad=GetBoardRgnCad();	
	const double BoardCadCpX = (BoardRgnCad.minX+BoardRgnCad.maxX)*0.5;
	const double BoardCadCpY = (BoardRgnCad.minY+BoardRgnCad.maxY)*0.5;		
	if ( RotateBoard(CadAngle, BoardCadCpX, BoardCadCpY) == false )
	{	return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::MoveBoardPos(double dX, double dY)//移動單板
{
	size_t          i = 0;
	DISTRICT_ID     DistrictID;
	CAOIFd         *pFd = NULL;
	CAOIMark       *pMark = NULL;
	CAOIBarcode    *pBarcode = NULL;
	CAOIPanel      *PanelPtr = NULL;		
	CAOIComponent  *pComponent = NULL;
	CMapCoordinate *MapCTSPtrDA = GetBoardMapCTSPtr(DISTRICT_ID_A);
	CMapCoordinate *MapCTSPtrDB = GetBoardMapCTSPtr(DISTRICT_ID_B);
	const size_t    FdCount = GetBoardFdCount_Inline();
	const size_t    MarkCount = GetBoardMarkCount_Inline();
	const size_t    BarcodeCount = GetBoardBarcodeCount_Inline();
	const size_t    ComponentCount = GetBoardComponentCount_Inline();

	PanelPtr = GetBoardPanelPtr();	
	if ( NULL==MapCTSPtrDA && NULL!=PanelPtr ) 
	{	MapCTSPtrDA = PanelPtr->GetPanelMapCTSPtr(DISTRICT_ID_A);	}	
	if ( NULL==MapCTSPtrDB && NULL!=PanelPtr ) 
	{	MapCTSPtrDB = PanelPtr->GetPanelMapCTSPtr(DISTRICT_ID_B);	}	
	if ( NULL==MapCTSPtrDA || NULL==MapCTSPtrDB ) { return ; }

	for ( i=0; i<FdCount; i++ )
	{
		pFd = GetBoardFdPtr_Inline(i);
		if ( NULL == pFd ) { continue; }
		DistrictID = pFd->GetFdDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pFd->MoveFdPos(dX, dY, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pFd->MoveFdPos(dX, dY, MapCTSPtrDB);	break;
		}
	}

	for ( i=0; i<MarkCount; i++ )
	{
		pMark = GetBoardMarkPtr_Inline(i);
		if ( NULL == pMark ) { continue; }
		DistrictID = pMark->GetMarkDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pMark->MoveMarkPos(dX, dY, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pMark->MoveMarkPos(dX, dY, MapCTSPtrDB);	break;
		}
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		pBarcode = GetBoardBarcodePtr_Inline(i);
		if ( NULL == pBarcode ) { continue; }
		DistrictID = pBarcode->GetBarcodeDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pBarcode->MoveBarcodePos(dX, dY, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pBarcode->MoveBarcodePos(dX, dY, MapCTSPtrDB);	break;
		}
	}

	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetBoardComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }
		DistrictID = pComponent->GetComponentDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pComponent->MoveComponentPos(dX, dY, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pComponent->MoveComponentPos(dX, dY, MapCTSPtrDB);	break;
		}
	}	

	CAOIField   *FieldPtr=NULL;
	const size_t FieldCount = GetBoardFieldCount_Inline();		
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetBoardFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		DistrictID = FieldPtr->GetFieldDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	FieldPtr->MoveFieldPos(dX, dY, MapCTSPtrDA); break;
		case DISTRICT_ID_B:	FieldPtr->MoveFieldPos(dX, dY, MapCTSPtrDB); break;
		}
	}
	CalcBoardStagePosition();	
	LayoutBoardRegion();	
	return ;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::RotateBoard(double Angle, double CpX, double CpY)//旋轉單板
{
	size_t         i = 0;
	DISTRICT_ID    DistrictID;
	CAOIFd        *pFd = NULL;
	CAOIMark      *pMark = NULL;
	CAOIBarcode   *pBarcode = NULL;
	CAOIComponent *pComponent = NULL;		
	double       BoardRotatedAngle = GetBoardRoatedAngle();	
	const size_t FdCount = GetBoardFdCount_Inline();
	const size_t MarkCount = GetBoardMarkCount_Inline();
	const size_t BarcodeCount = GetBoardBarcodeCount_Inline();
	const size_t ComponentCount = GetBoardComponentCount_Inline();
	BOARD_ORIENTATION_MODE OrientationMode = GetBoardOrientationMode();
	CMapCoordinate *MapCTSPtrDA = GetBoardMapCTSPtr(DISTRICT_ID_A);
	CMapCoordinate *MapCTSPtrDB = GetBoardMapCTSPtr(DISTRICT_ID_B);
	if ( NULL==MapCTSPtrDA || NULL==MapCTSPtrDB ) { return false; }

	for ( i=0; i<FdCount; i++ )
	{
		pFd = GetBoardFdPtr_Inline(i);
		if ( NULL == pFd ) { continue; }
		DistrictID = pFd->GetFdDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pFd->RotateFd(Angle, CpX, CpY, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pFd->RotateFd(Angle, CpX, CpY, MapCTSPtrDB);	break;
		}		
	}

	for ( i=0; i<MarkCount; i++ )
	{
		pMark = GetBoardMarkPtr_Inline(i);
		if ( NULL == pMark ) { continue; }
		DistrictID = pMark->GetMarkDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pMark->RotateMark(Angle, CpX, CpY, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pMark->RotateMark(Angle, CpX, CpY, MapCTSPtrDB);	break;
		}
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		pBarcode = GetBoardBarcodePtr_Inline(i);
		if ( NULL == pBarcode ) { continue; }
		DistrictID = pBarcode->GetBarcodeDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pBarcode->RotateBarcode(Angle, CpX, CpY, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pBarcode->RotateBarcode(Angle, CpX, CpY, MapCTSPtrDB);	break;
		}
	}

	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetBoardComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }
		DistrictID = pComponent->GetComponentDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pComponent->RotateComponent(Angle, CpX, CpY, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pComponent->RotateComponent(Angle, CpX, CpY, MapCTSPtrDB);	break;
		}
		pComponent->RotateComponentBasePlaneParam(Angle);
	}	

	BoardRotatedAngle = JetAPI::RotateAngle(BoardRotatedAngle, Angle);	
	OrientationMode = JetAPI::RotateBoardOrientationMode(OrientationMode, Angle);
	SetBoardRoatedAngle(BoardRotatedAngle);
	SetBoardOrientationMode(OrientationMode);
	CalcBoardStagePosition();
	LayoutBoardRegion();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::MirrorXBoard(double CpX)//鏡射單板-X值
{
	size_t          i = 0;	
	DISTRICT_ID     DistrictID;
	CAOIFd         *pFd = NULL;
	CAOIMark       *pMark = NULL;
	CAOIPanel      *PanelPtr = NULL;
	CAOIBarcode    *pBarcode = NULL;
	CAOIComponent  *pComponent = NULL;	
	CMapCoordinate *MapCTSPtrDA = GetBoardMapCTSPtr(DISTRICT_ID_A);	
	CMapCoordinate *MapCTSPtrDB = GetBoardMapCTSPtr(DISTRICT_ID_B);	
	double          BoardRotatedAngle = GetBoardRoatedAngle();	
	const size_t    FdCount = GetBoardFdCount_Inline();
	const size_t    MarkCount = GetBoardMarkCount_Inline();
	const size_t    BarcodeCount = GetBoardBarcodeCount_Inline();
	const size_t    ComponentCount = GetBoardComponentCount_Inline();
	BOARD_ORIENTATION_MODE OrientationMode = GetBoardOrientationMode();

	PanelPtr = GetBoardPanelPtr();	
	if ( NULL==MapCTSPtrDA && NULL!=PanelPtr ) 
	{	MapCTSPtrDA = PanelPtr->GetPanelMapCTSPtr(DISTRICT_ID_A);	}	
	if ( NULL==MapCTSPtrDB && NULL!=PanelPtr ) 
	{	MapCTSPtrDB = PanelPtr->GetPanelMapCTSPtr(DISTRICT_ID_B);	}	
	if ( NULL==MapCTSPtrDA || NULL==MapCTSPtrDB ) { return false; }

	for ( i=0; i<FdCount; i++ )
	{
		pFd = GetBoardFdPtr_Inline(i);
		if ( NULL == pFd ) { continue; }
		DistrictID = pFd->GetFdDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pFd->MirrorXFd(CpX, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pFd->MirrorXFd(CpX, MapCTSPtrDB);	break;
		}
	}

	for ( i=0; i<MarkCount; i++ )
	{
		pMark = GetBoardMarkPtr_Inline(i);
		if ( NULL == pMark ) { continue; }
		DistrictID = pMark->GetMarkDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pMark->MirrorXMark(CpX, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pMark->MirrorXMark(CpX, MapCTSPtrDB);	break;
		}
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		pBarcode = GetBoardBarcodePtr_Inline(i);
		if ( NULL == pBarcode ) { continue; }
		DistrictID = pBarcode->GetBarcodeDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pBarcode->MirrorXBarcode(CpX, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pBarcode->MirrorXBarcode(CpX, MapCTSPtrDB);	break;
		}
	}

	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetBoardComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }
		DistrictID = pComponent->GetComponentDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pComponent->MirrorXComponent(CpX, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pComponent->MirrorXComponent(CpX, MapCTSPtrDB);	break;
		}
	}

	BoardRotatedAngle = JetAPI::MirrorYAxisAngle(BoardRotatedAngle);	
	OrientationMode = JetAPI::MirrorYAxisBoardOrientationMode(OrientationMode);
	SetBoardRoatedAngle(BoardRotatedAngle);
	SetBoardOrientationMode(OrientationMode);
	CalcBoardStagePosition();
	LayoutBoardRegion();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::MirrorYBoard(double CpY)//鏡射單板-Y值
{
	DISTRICT_ID     DistrictID;
	size_t          i = 0;
	CAOIFd         *pFd = NULL;
	CAOIMark       *pMark = NULL;
	CAOIPanel      *PanelPtr = NULL;
	CAOIBarcode    *pBarcode = NULL;	
	CAOIComponent  *pComponent = NULL;
	CMapCoordinate *MapCTSPtrDA = GetBoardMapCTSPtr(DISTRICT_ID_A);	
	CMapCoordinate *MapCTSPtrDB = GetBoardMapCTSPtr(DISTRICT_ID_B);	
	double          BoardRotatedAngle = GetBoardRoatedAngle();	
	const size_t    FdCount = GetBoardFdCount_Inline();
	const size_t    MarkCount = GetBoardMarkCount_Inline();
	const size_t    BarcodeCount = GetBoardBarcodeCount_Inline();
	const size_t    ComponentCount = GetBoardComponentCount_Inline();
	BOARD_ORIENTATION_MODE OrientationMode = GetBoardOrientationMode();

	PanelPtr = GetBoardPanelPtr();	
	if ( NULL==MapCTSPtrDA && NULL!=PanelPtr ) 
	{	MapCTSPtrDA = PanelPtr->GetPanelMapCTSPtr(DISTRICT_ID_A);	}	
	if ( NULL==MapCTSPtrDB && NULL!=PanelPtr ) 
	{	MapCTSPtrDB = PanelPtr->GetPanelMapCTSPtr(DISTRICT_ID_B);	}	
	if ( NULL==MapCTSPtrDA || NULL==MapCTSPtrDB ) { return false; }

	for ( i=0; i<FdCount; i++ )
	{
		pFd = GetBoardFdPtr_Inline(i);
		if ( NULL == pFd ) { continue; }
		DistrictID = pFd->GetFdDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pFd->MirrorYFd(CpY, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pFd->MirrorYFd(CpY, MapCTSPtrDB);	break;
		}
	}

	for ( i=0; i<MarkCount; i++ )
	{
		pMark = GetBoardMarkPtr_Inline(i);
		if ( NULL == pMark ) { continue; }
		DistrictID = pMark->GetMarkDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pMark->MirrorYMark(CpY, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pMark->MirrorYMark(CpY, MapCTSPtrDB);	break;
		}
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		pBarcode = GetBoardBarcodePtr_Inline(i);
		if ( NULL == pBarcode ) { continue; }
		DistrictID = pBarcode->GetBarcodeDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pBarcode->MirrorYBarcode(CpY, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pBarcode->MirrorYBarcode(CpY, MapCTSPtrDB);	break;
		}
	}

	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetBoardComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }
		DistrictID = pComponent->GetComponentDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pComponent->MirrorYComponent(CpY, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pComponent->MirrorYComponent(CpY, MapCTSPtrDB);	break;
		}
	}

	BoardRotatedAngle = JetAPI::MirrorXAxisAngle(BoardRotatedAngle);	
	OrientationMode = JetAPI::MirrorXAxisBoardOrientationMode(OrientationMode);
	SetBoardRoatedAngle(BoardRotatedAngle);
	SetBoardOrientationMode(OrientationMode);
	CalcBoardStagePosition();
	LayoutBoardRegion();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::LayoutBoardRegionCad()//重整單板範圍
{
	size_t i = 0;	
	bool bGetRgn=false;
	TREGION4D CADRegion;
	double CADPosX=0, CADPosY=0;
	double CADCornerPosX[4]={0};
	double CADCornerPosY[4]={0};
	TREGION4D      CadRgn;
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIComponent *pComponent = NULL;	
	const size_t FdCount = GetBoardFdCount_Inline();
	const size_t MarkCount = GetBoardMarkCount_Inline();
	const size_t BarcodeCount = GetBoardBarcodeCount_Inline();	
	const size_t ComponentCount = GetBoardComponentCount_Inline();	

	bGetRgn=false;
	CadRgn.Limit(true);	
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetBoardFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }		
		CADPosX = FdPtr->GetFdCadPosX();
		CADPosY = FdPtr->GetFdCadPosY();

		FdPtr->GetFdCadCornerPosX(CADCornerPosX);
		FdPtr->GetFdCadCornerPosY(CADCornerPosY);
		JetAPI::CornerXYToRegion(CADCornerPosX, CADCornerPosY, CADRegion);
		bGetRgn = true;
		CadRgn.Expand(CADRegion);		
	}

	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }		
		CADPosX = MarkPtr->GetMarkCadPosX();
		CADPosY = MarkPtr->GetMarkCadPosY();

		MarkPtr->GetMarkCadCornerPosX(CADCornerPosX);
		MarkPtr->GetMarkCadCornerPosY(CADCornerPosY);
		JetAPI::CornerXYToRegion(CADCornerPosX, CADCornerPosY, CADRegion);
		bGetRgn = true;
		CadRgn.Expand(CADRegion);		
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetBoardBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		CADPosX = BarcodePtr->GetBarcodeCadPosX();
		CADPosY = BarcodePtr->GetBarcodeCadPosY();

		BarcodePtr->GetBarcodeCadCornerPosX(CADCornerPosX);
		BarcodePtr->GetBarcodeCadCornerPosY(CADCornerPosY);
		JetAPI::CornerXYToRegion(CADCornerPosX, CADCornerPosY, CADRegion);
		bGetRgn = true;
		CadRgn.Expand(CADRegion);		
	}

	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetBoardComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }
		CADPosX = pComponent->GetComponentCadPosX();
		CADPosY = pComponent->GetComponentCadPosY();

		pComponent->GetComponentCadCornerPosX(CADCornerPosX);
		pComponent->GetComponentCadCornerPosY(CADCornerPosY);
		JetAPI::CornerXYToRegion(CADCornerPosX, CADCornerPosY, CADRegion);
		bGetRgn = true;
		CadRgn.Expand(CADRegion);		
	}	
	if ( false == bGetRgn )
	{	CadRgn = TREGION4D();	}
	SetBoardRgnCad(CadRgn);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::LayoutBoardRegionStage()//重整單板範圍
{	
	size_t         i = 0;
	TREGION4D      StageRegion;
	double         StagePosX=0, StagePosY=0;
	double         StageCornerPosX[4]={0};
	double         StageCornerPosY[4]={0};	
	TREGION4D      RgnCad;
	TREGION4D      RgnStage_DA;
	TREGION4D      RgnStage_DB;
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIComponent *pComponent = NULL;
	const size_t   FdCount = GetBoardFdCount_Inline();
	const size_t   MarkCount = GetBoardMarkCount_Inline();	
	const size_t   BarcodeCount = GetBoardBarcodeCount_Inline();	
	const size_t   ComponentCount = GetBoardComponentCount_Inline();
	CMapCoordinate *MapCTSPtrDA = GetBoardMapCTSPtr(DISTRICT_ID_A);
	CMapCoordinate *MapCTSPtrDB = GetBoardMapCTSPtr(DISTRICT_ID_B);

	GetBoardRgnCad(RgnCad);
	MapCTSPtrDA->MapRegion2D(RgnCad, RgnStage_DA);
	MapCTSPtrDB->MapRegion2D(RgnCad, RgnStage_DB);
	JetAPI::AdjustRegion(RgnStage_DA, RgnStage_DA);
	JetAPI::AdjustRegion(RgnStage_DB, RgnStage_DB);	
	/*
	RgnStage_DA.Limit(true);
	RgnStage_DB.Limit(true);
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetBoardFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }		
		StagePosX = FdPtr->GetFdStagePosX();
		StagePosY = FdPtr->GetFdStagePosY();
		DistrictID = FdPtr->GetFdDistrictID();

		FdPtr->GetFdRoiStageCornerPosX(StageCornerPosX);
		FdPtr->GetFdRoiStageCornerPosY(StageCornerPosY);
		JetAPI::CornerXYToRegion(StageCornerPosX, StageCornerPosY, StageRegion);
		
		if ( DISTRICT_ID_A == DistrictID )
		{	RgnStage_DA.Expand(StageRegion);	}
		if ( DISTRICT_ID_B == DistrictID )
		{	RgnStage_DB.Expand(StageRegion);	}
	}

	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }		
		StagePosX = MarkPtr->GetMarkStagePosX();
		StagePosY = MarkPtr->GetMarkStagePosY();
		DistrictID = MarkPtr->GetMarkDistrictID();

		MarkPtr->GetMarkRoiStageCornerPosX(StageCornerPosX);
		MarkPtr->GetMarkRoiStageCornerPosY(StageCornerPosY);
		JetAPI::CornerXYToRegion(StageCornerPosX, StageCornerPosY, StageRegion);
		
		if ( DISTRICT_ID_A == DistrictID )
		{	RgnStage_DA.Expand(StageRegion);	}
		if ( DISTRICT_ID_B == DistrictID )
		{	RgnStage_DB.Expand(StageRegion);	}
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetBoardBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		StagePosX = BarcodePtr->GetBarcodeStagePosX();
		StagePosY = BarcodePtr->GetBarcodeStagePosY();
		DistrictID = BarcodePtr->GetBarcodeDistrictID();

		BarcodePtr->GetBarcodeRoiStageCornerPosX(StageCornerPosX);
		BarcodePtr->GetBarcodeRoiStageCornerPosY(StageCornerPosY);
		JetAPI::CornerXYToRegion(StageCornerPosX, StageCornerPosY, StageRegion);
		
		if ( DISTRICT_ID_A == DistrictID )
		{	RgnStage_DA.Expand(StageRegion);	}
		if ( DISTRICT_ID_B == DistrictID )
		{	RgnStage_DB.Expand(StageRegion);	}
	}

	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetBoardComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }				
		StagePosX = pComponent->GetComponentStagePosX();
		StagePosY = pComponent->GetComponentStagePosY();		
		DistrictID = pComponent->GetComponentDistrictID();

		pComponent->GetComponentRoiStageCornerPosX(StageCornerPosX);
		pComponent->GetComponentRoiStageCornerPosY(StageCornerPosY);
		JetAPI::CornerXYToRegion(StageCornerPosX, StageCornerPosY, StageRegion);

		if ( DISTRICT_ID_A == DistrictID )
		{	RgnStage_DA.Expand(StageRegion);	}
		if ( DISTRICT_ID_B == DistrictID )
		{	RgnStage_DB.Expand(StageRegion);	}
	}
	*/
	SetBoardRgnStage(DISTRICT_ID_A, RgnStage_DA);
	SetBoardRgnStage(DISTRICT_ID_B, RgnStage_DB);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::LayoutBoardRegionStage(DISTRICT_ID DistrictID)//重整單板範圍		
{
	size_t         i = 0;
	TREGION4D      StageRegion;
	double         StagePosX=0, StagePosY=0;
	double         StageCornerPosX[4]={0};
	double         StageCornerPosY[4]={0};
	TREGION4D      RgnCad;
	TREGION4D      RgnStage;
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIComponent *pComponent = NULL;
	const size_t FdCount = GetBoardFdCount_Inline();
	const size_t MarkCount = GetBoardMarkCount_Inline();	
	const size_t BarcodeCount = GetBoardBarcodeCount_Inline();	
	const size_t ComponentCount = GetBoardComponentCount_Inline();
	CMapCoordinate *MapCTSPtr = GetBoardMapCTSPtr(DistrictID);	

	GetBoardRgnCad(RgnCad);
	MapCTSPtr->MapRegion2D(RgnCad, RgnStage);
	JetAPI::AdjustRegion(RgnStage, RgnStage);
	/*
	RgnStage.Limit(true);	
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetBoardFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		StagePosX = FdPtr->GetFdStagePosX();
		StagePosY = FdPtr->GetFdStagePosY();

		FdPtr->GetFdRoiStageCornerPosX(StageCornerPosX);
		FdPtr->GetFdRoiStageCornerPosY(StageCornerPosY);
		JetAPI::CornerXYToRegion(StageCornerPosX, StageCornerPosY, StageRegion);
		RgnStage.Expand(StageRegion);		
	}

	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }		
		StagePosX = MarkPtr->GetMarkStagePosX();
		StagePosY = MarkPtr->GetMarkStagePosY();
		DistrictID = MarkPtr->GetMarkDistrictID();

		MarkPtr->GetMarkRoiStageCornerPosX(StageCornerPosX);
		MarkPtr->GetMarkRoiStageCornerPosY(StageCornerPosY);
		JetAPI::CornerXYToRegion(StageCornerPosX, StageCornerPosY, StageRegion);
		
		if ( DISTRICT_ID_A == DistrictID )
		{	RgnStage_DA.Expand(StageRegion);	}
		if ( DISTRICT_ID_B == DistrictID )
		{	RgnStage_DB.Expand(StageRegion);	}
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetBoardBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		StagePosX = BarcodePtr->GetBarcodeStagePosX();
		StagePosY = BarcodePtr->GetBarcodeStagePosY();

		BarcodePtr->GetBarcodeRoiStageCornerPosX(StageCornerPosX);
		BarcodePtr->GetBarcodeRoiStageCornerPosY(StageCornerPosY);
		JetAPI::CornerXYToRegion(StageCornerPosX, StageCornerPosY, StageRegion);
		RgnStage.Expand(StageRegion);
	}


	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetBoardComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }
		if ( DistrictID != pComponent->GetComponentDistrictID() ) { continue; }
		StagePosX = pComponent->GetComponentStagePosX();
		StagePosY = pComponent->GetComponentStagePosY();

		pComponent->GetComponentRoiStageCornerPosX(StageCornerPosX);
		pComponent->GetComponentRoiStageCornerPosY(StageCornerPosY);
		JetAPI::CornerXYToRegion(StageCornerPosX, StageCornerPosY, StageRegion);
		RgnStage.Expand(StageRegion);
	}	
	*/
	SetBoardRgnStage(DistrictID, RgnStage);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::LayoutBoardRegion()//重整單板範圍
{
	LayoutBoardRegionCad();
	LayoutBoardRegionStage();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::LayoutBoardRegion(DISTRICT_ID DistrictID)//重整單板範圍	
{
	LayoutBoardRegionCad();
	LayoutBoardRegionStage(DistrictID);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CalcBoardMapParam()//計算單板座標轉換參數
{
	if ( CalcBoardMapParam(DISTRICT_ID_A) == false ) 
	{	return false; }
	if ( GetBoardMultiDistrictMode() )
	{
		if ( CalcBoardMapParam(DISTRICT_ID_B) == false ) 
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CalcBoardMapParam(DISTRICT_ID DistrictID)//計算單板座標轉換參數
{
	if ( false == m_BoardMapEnable ) { return true; }
	const size_t   FdCount = GetBoardFdCount_Inline();	
	if ( 0 == FdCount ) { return true; }

	size_t         i=0;
	double         CadX=0, CadY=0;
	double         StageX=0, StageY=0;
	CAOIFd        *FdPtr = NULL;
	RESULT_ID      FdResultID;
	std::vector<CAOIFd*> BoardFdPtrList;
	const size_t   MaxFdCount = BOARD_MAX_FD_COUNT;
	CMapCoordinate  TmpBoardMapCTS;
	CMapCoordinate  TmpBoardMapSTC;
	double CadPosX[MaxFdCount]={0};
	double CadPosY[MaxFdCount]={0};
	double StagePosX[MaxFdCount]={0};
	double StagePosY[MaxFdCount]={0};

	GetBoardMapCTS(DistrictID, TmpBoardMapCTS);
	GetBoardMapSTC(DistrictID, TmpBoardMapSTC);

	//Board Fiducial 
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetBoardFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }		
		if ( FdPtr->GetFdBoardPtr() == NULL ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		FdResultID = FdPtr->GetFdResultID_AOI();		
		if ( RESULT_ID_OK != FdResultID ) { continue; }		
		BoardFdPtrList.push_back(FdPtr);				
	}
	const size_t BoardFdCount = BoardFdPtrList.size();
	const size_t UsedFdCount = MIN(BoardFdCount, MaxFdCount);
	if ( 0 == UsedFdCount ) 
	{	return true;	}
	
	for ( i=0; i<UsedFdCount; i++ )
	{
		FdPtr = BoardFdPtrList[i];
		CadPosX[i] = FdPtr->GetFdCadPosX();
		CadPosY[i] = FdPtr->GetFdCadPosY();
		StagePosX[i] = FdPtr->GetFdStagePosX();
		StagePosY[i] = FdPtr->GetFdStagePosY();
	}

	if ( DISTRICT_ID_B == DistrictID )
	{
		m_BoardMapCTS_DB.Identity();
		m_BoardMapSTC_DB.Identity();
		m_BoardMapCTS_DB.CalcMatrix2D(CadPosX, CadPosY, StagePosX, StagePosY, UsedFdCount);
		m_BoardMapSTC_DB.CalcMatrix2D(StagePosX, StagePosY, CadPosX, CadPosY, UsedFdCount);	
	}
	else
	{
		m_BoardMapCTS.Identity();
		m_BoardMapSTC.Identity();
		m_BoardMapCTS.CalcMatrix2D(CadPosX, CadPosY, StagePosX, StagePosY, UsedFdCount);
		m_BoardMapSTC.CalcMatrix2D(StagePosX, StagePosY, CadPosX, CadPosY, UsedFdCount);	
	}		

	//單板定位點參考整板定位點
	int CopyMode=0;	
	switch ( UsedFdCount )
	{
	case 1:	CopyMode = COORDINATE_MATRIX_ALL-COORDINATE_MATRIX_TRANSLATION;	break;		
	}	
	if ( 0 != CopyMode)
	{
		CMapCoordinate  LocalMapCTS;
		CMapCoordinate  LocalMapSTC;

		double CTS_DiffX=0, CTS_DiffY=0;
		double STC_DiffX=0, STC_DiffY=0;
		double CadPosX2=0, CadPosY2=0;		
		double StagePosX2=0, StagePosY2=0;		

		LocalMapCTS.Identity();
		LocalMapSTC.Identity();
		LocalMapCTS.CopyMatrix(TmpBoardMapCTS, CopyMode);
		LocalMapSTC.CopyMatrix(TmpBoardMapSTC, CopyMode);				
		for ( i=0; i<UsedFdCount; i++ )
		{
			LocalMapCTS.Map2D(CadPosX[i], CadPosY[i], StagePosX2, StagePosY2);
			LocalMapSTC.Map2D(StagePosX[i], StagePosY[i], CadPosX2, CadPosY2);
			CTS_DiffX += StagePosX[i]-StagePosX2;
			CTS_DiffY += StagePosY[i]-StagePosY2;
			STC_DiffX += CadPosX[i]-CadPosX2;
			STC_DiffY += CadPosY[i]-CadPosY2;
		}
		CTS_DiffX /= UsedFdCount;
		CTS_DiffY /= UsedFdCount;
		STC_DiffX /= UsedFdCount;
		STC_DiffY /= UsedFdCount;
		LocalMapCTS.MoveMatrix2D(CTS_DiffX, CTS_DiffY);
		LocalMapSTC.MoveMatrix2D(STC_DiffX, STC_DiffY);
#ifdef _DEBUG
		CTS_DiffX = CTS_DiffY = 0;
		STC_DiffX = STC_DiffY = 0;
		for ( i=0; i<UsedFdCount; i++ )
		{
			LocalMapCTS.Map2D(CadPosX[i], CadPosY[i], StagePosX2, StagePosY2);
			LocalMapSTC.Map2D(StagePosX[i], StagePosY[i], CadPosX2, CadPosY2);
			CTS_DiffX += StagePosX[i]-StagePosX2;
			CTS_DiffY += StagePosY[i]-StagePosY2;
			STC_DiffX += CadPosX[i]-CadPosX2;
			STC_DiffY += CadPosY[i]-CadPosY2;
		}
		CTS_DiffX /= UsedFdCount;
		CTS_DiffY /= UsedFdCount;
		STC_DiffX /= UsedFdCount;
		STC_DiffY /= UsedFdCount;
#endif//_DEBUG
		if ( DISTRICT_ID_B == DistrictID )
		{	
			m_BoardMapCTS_DB = LocalMapCTS;
			m_BoardMapSTC_DB = LocalMapSTC;
		}
		else
		{
			m_BoardMapCTS = LocalMapCTS;
			m_BoardMapSTC = LocalMapSTC;			
		}	
		i = i;	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CalcBoardStagePosition(bool bCalcFov)//計算單板座標
{
	if ( CalcBoardStagePosition(DISTRICT_ID_A, bCalcFov) == false )
	{	return false; }
	if ( GetBoardMultiDistrictMode() )
	{
		if ( CalcBoardStagePosition(DISTRICT_ID_B, bCalcFov) == false )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CalcBoardStagePosition(DISTRICT_ID DistrictID, bool bCalcFov)//計算單板座標
{
	if ( false == m_BoardMapEnable ) { return true; }
	if ( DISTRICT_ID_A == DistrictID )
	{
		if ( CalcBoardStagePosition(DistrictID, m_BoardMapCTS, bCalcFov) == false )
		{	return false;	}
	}
	if ( DISTRICT_ID_B == DistrictID )
	{
		if ( CalcBoardStagePosition(DistrictID, m_BoardMapCTS_DB, bCalcFov) == false )
		{	return false;	}
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CalcBoardStagePosition(DISTRICT_ID DistrictID, const CMapCoordinate &Map, bool bCalcFov)////計算單板座標
{
	size_t         i=0, j=0, k=0;
	size_t         WindowCount = 0;
	size_t         SubRgnCount = 0;
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIField     *FieldPtr = NULL;
	CAOIRgn       *SubRgnPtr = NULL;	
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIWindow    *WindowPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	const size_t   FdCount = GetBoardFdCount_Inline();
	const size_t   MarkCount = GetBoardMarkCount_Inline();
	const size_t   FieldCount = GetBoardFieldCount_Inline();
	const size_t   BarcodeCount = GetBoardBarcodeCount_Inline();
	const size_t   ComponentCount = GetBoardComponentCount_Inline();

	//Field
	if ( true == bCalcFov )
	{
		for ( i=0; i<FieldCount; i++ )
		{
			FieldPtr = GetBoardFieldPtr_Inline(i);
			if ( NULL == FieldPtr ) { continue; }
			if ( DistrictID != FieldPtr->GetFieldDistrictID() ) { continue; }
			FieldPtr->MapFieldCadToStagePos(Map);
		}
	}

	//Fd
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetBoardFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdPanelPtr() == NULL ) { continue; }
		if ( FdPtr->GetFdBoardPtr() != this ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		FdPtr->MapFdCadToStagePos(Map);
	}	

	//Mark
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }		
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		//MarkPtr->ClearMarkSubRgnList();
		MarkPtr->MapMarkCadToStagePos(Map);

		SubRgnCount = MarkPtr->GetMarkSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = MarkPtr->GetMarkSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->MapRgnCadToStagePos(Map);
		}
	}	

	//Barcode
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetBoardBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		//BarcodePtr->ClearBarcodeSubRgnList();
		BarcodePtr->MapBarcodeCadToStagePos(Map);		

		SubRgnCount = BarcodePtr->GetBarcodeSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = BarcodePtr->GetBarcodeSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->MapRgnCadToStagePos(Map);
		}
	}

	//Component
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		//if ( ComponentPtr->GetComponentNeedToUpdate() == false ) { continue; }
		//ComponentPtr->ClearComponentSubRgnList();
		ComponentPtr->MapComponentCadToStagePos(Map);

		if ( true == bCalcFov )
		{
			FieldPtr = ComponentPtr->GetComponentSelfFieldPtr();
			if ( NULL != FieldPtr )
			{	FieldPtr->MapFieldCadToStagePos(Map);	}
		}

		SubRgnCount = ComponentPtr->GetComponentSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = ComponentPtr->GetComponentSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			SubRgnPtr->MapRgnCadToStagePos(Map);
		}

		WindowCount = ComponentPtr->GetComponentWindowCount();
		for ( j=0; j<WindowCount; j++ )
		{
			WindowPtr = ComponentPtr->GetComponentWindowPtr(j, false);
			if ( NULL == WindowPtr ) { continue; }
			//WindowPtr->ClearWindowSubRgnList();
			WindowPtr->MapWindowCadToStagePos(Map);			

			SubRgnCount = WindowPtr->GetWindowSubRgnCount();
			for ( k=0; k<SubRgnCount; k++ )
			{
				SubRgnPtr = WindowPtr->GetWindowSubRgnPtr(k, false);
				if ( NULL == SubRgnPtr ) { continue; }
				SubRgnPtr->MapRgnCadToStagePos(Map);
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CalcBoardCadResPosition_All()//計算單板Cad結果座標
{
	if ( CalcBoardCadResPosition(DISTRICT_ID_A) == false )
	{	return false; }
	if ( GetBoardMultiDistrictMode() )
	{
		if ( CalcBoardCadResPosition(DISTRICT_ID_B) == false )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CalcBoardCadResPosition(DISTRICT_ID DistrictID)//計算單板Cad結果座標
{
	if ( false == m_BoardMapEnable ) { return true; }
	CMapCoordinate Map;
	if ( GetBoardMapSTC(DistrictID, Map) == false )	{	return false; }	
	return CalcBoardCadResPosition(	DistrictID, Map);
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CalcBoardCadResPosition(DISTRICT_ID DistrictID, const CMapCoordinate &Map)////計算單板Cad結果座標	
{
	if ( false == m_BoardMapEnable ) { return true; }
	
	size_t         i=0, j=0, k=0;	
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;	
	CAOIComponent *ComponentPtr = NULL;
	const size_t   FdCount = GetBoardFdCount_Inline();
	const size_t   MarkCount = GetBoardMarkCount_Inline();	
	const size_t   BarcodeCount = GetBoardBarcodeCount_Inline();
	const size_t   ComponentCount = GetBoardComponentCount_Inline();	
	bool bMapOthers=false;//是否更新至其他物件上, 目前先使用在零件上

if ( true == bMapOthers )
{
	//Fd
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetBoardFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdPanelPtr() == NULL ) { continue; }
		if ( FdPtr->GetFdBoardPtr() != this ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		if ( FdPtr->GetFdNeedToCalculate() == false ) { continue; }
		//FdPtr->MapFdCadToStagePos(Map);
	}	

	//Mark
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }		
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		if ( MarkPtr->GetMarkNeedToCalculate() == false ) { continue; }		
		//MarkPtr->MapMarkCadToStagePos(Map);
	}	

	//Barcode
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetBoardBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		if ( BarcodePtr->GetBarcodeNeedToCalculate() == false ) { continue; }
		//BarcodePtr->MapBarcodeCadToStagePos(Map);
	}
}

	//Component
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		if ( ComponentPtr->GetComponentNeedToCalculate() == false ) { continue; }		
		ComponentPtr->MapComponentStageResultToCadPos(Map);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::GetBoardBypassed() const
{ 
	return m_BoardBypassed; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardBypassed(bool value)
{
	m_BoardBypassed = value; 
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::UpdateBoardBypassed()//更新單板不檢測至零件內
{
	size_t         i=0;	
	CAOIMark     *MarkPtr = NULL;
	const size_t  MarkCount = GetBoardMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		MarkPtr->UpdateMarkBypassed();
	}

	CAOIBarcode   *BarcodePtr = NULL;
	const size_t  BarcodeCount = GetBoardBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetBoardBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		BarcodePtr->UpdateBarcodeBypassed();
	}

	CAOIComponent *ComponentPtr = NULL;
	const size_t  ComponentCount = GetBoardComponentCount_Inline();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->UpdateComponentBypassed();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::ExecBoardBypassed(LANE_ID LaneID)//執行單板是為不檢測
{
	size_t     i=0;
	RESULT_ID PartResultID = RESULT_ID_BYPASS;
	RESULT_ID BoardResultID = RESULT_ID_BYPASS;		
	SetBoardResultID(BoardResultID);
	SetBoardResultID_Alarm(BoardResultID);

	CAOIFd *FdPtr = NULL;
	const size_t FdCount = GetBoardFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetBoardFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }		
		FdPtr->BypassSkipFd(PartResultID);		
	}

	CAOIMark    *MarkPtr = NULL;
	const size_t MarkCount = GetBoardMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }		
		MarkPtr->BypassSkipMark(PartResultID);
	}

	CAOIBarcode *BarcodePtr = NULL;
	const size_t BarcodeCount = GetBoardBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetBoardBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		BarcodePtr->BypassSkipBarcode(PartResultID);		
	}

	CAOIComponent *ComponentPtr = NULL;
	const size_t  ComponentCount = GetBoardComponentCount_Inline();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		ComponentPtr->BypassSkipComponent(PartResultID);	
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::GetBoardSkipped() const//取得單板是否跳過檢測
{
	if ( RESULT_ID_SKIP == GetBoardResultID() )
	{	return true; }
	if ( RESULT_ID_BYPASS == GetBoardResultID() )
	{	return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CheckBoardBypassedSkipped() const//確認單板是否不用檢測
{	
	if ( GetBoardBypassed() == true ) { return true; }
	if ( GetBoardSkipped() == true ) { return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardType(BOARD_TYPE value) 
{ 
	m_BoardType = value; 
}
//-------------------------------------------------------------------------------------//
BOARD_TYPE CAOIBoard::GetBoardType() const 
{ 
	return m_BoardType; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::GetBoardMapRect(TRECT4D &val) const//單板在底圖的範圍
{
	val = m_BoardMapRect;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardMapRect(const TRECT4D &val)	//單板在底圖的範圍
{
	m_BoardMapRect = val;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardSideMode(BOARD_SIDE_MODE value) 
{ 
	m_BoardSideMode = value; 
}
//-------------------------------------------------------------------------------------//
BOARD_SIDE_MODE CAOIBoard::GetBoardSideMode() const 
{ 
	return m_BoardSideMode; 
}
//-------------------------------------------------------------------------------------//	
void CAOIBoard::SetBoardOrientationMode(BOARD_ORIENTATION_MODE value) 
{ 
	m_BoardOrientationMode = value; 
}
//-------------------------------------------------------------------------------------//
BOARD_ORIENTATION_MODE CAOIBoard::GetBoardOrientationMode() const 
{ 
	return m_BoardOrientationMode; 
}
//-------------------------------------------------------------------------------------//	
void CAOIBoard::SetBoardModified(bool value) 
{ 
	m_BoardModified = value; 
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::GetBoardModified() const 
{ 
	return m_BoardModified; 
}
//-------------------------------------------------------------------------------------//	
bool CAOIBoard::GetBoardBarcodeEnabled() const//單板條碼啟用
{
	return m_BoardBarcodeEnabled;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardBarcodeEnabled(bool value)//單板條碼啟用
{
	m_BoardBarcodeEnabled = value;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardMultiDistrictMode(bool value) 
{ 
	m_BoardMultiDistrictMode = value; 
}
//-------------------------------------------------------------------------------------//
double CAOIBoard::GetBoardXBoardCheckRatio() const//單板X板確認比例
{
	return m_BoardXBoardCheckRatio;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardXBoardCheckRatio(double value)//單板X板確認比例	
{
	m_BoardXBoardCheckRatio = value;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::GetBoardMultiDistrictMode() const 
{ 
	return m_BoardMultiDistrictMode; 
}
//-------------------------------------------------------------------------------------//	
void CAOIBoard::SetBoardResultID(RESULT_ID value) 
{ 
	m_BoardResultID_AOI = value; 
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIBoard::GetBoardResultID() const 
{ 
	return m_BoardResultID_AOI; 
}
//-------------------------------------------------------------------------------------//	
void CAOIBoard::SetBoardResultID_AOI_LA(RESULT_ID value)//單板檢測結果-AOI-A軌
{
	m_BoardResultID_AOI_LA = value;
}
//-------------------------------------------------------------------------------------//	
RESULT_ID CAOIBoard::GetBoardResultID_AOI_LA() const//單板檢測結果-AOI-A軌
{
	return m_BoardResultID_AOI_LA;
}
//-------------------------------------------------------------------------------------//	
void CAOIBoard::SetBoardResultID_AOI_LB(RESULT_ID value)//單板檢測結果-AOI-B軌
{
	m_BoardResultID_AOI_LB = value;
}
//-------------------------------------------------------------------------------------//	
RESULT_ID CAOIBoard::GetBoardResultID_AOI_LB() const//單板檢測結果-AOI-B軌
{
	return m_BoardResultID_AOI_LB;
}
//-------------------------------------------------------------------------------------//	
void CAOIBoard::UpdateBoardResultID_AOI_Lane(LANE_ID LaneID)//單板檢測結果-AOI-軌道
{
	RESULT_ID ResultID = GetBoardResultID();
	switch ( LaneID )
	{
	case LANE_ID_A:	SetBoardResultID_AOI_LA(ResultID); break;
	case LANE_ID_B:	SetBoardResultID_AOI_LB(ResultID); break;
	}
	return;
}
//-------------------------------------------------------------------------------------//	
RESULT_ID CAOIBoard::GetBoardResultID_AOI_Lane(LANE_ID LaneID) const//單板檢測結果-AOI-軌道
{
	RESULT_ID ResultID;
	switch ( LaneID )
	{
	case LANE_ID_A:	ResultID = GetBoardResultID_AOI_LA(); break;
	case LANE_ID_B:	ResultID = GetBoardResultID_AOI_LB(); break;
	default:		ResultID = GetBoardResultID(); break;
	}
	return ResultID;
}
//-------------------------------------------------------------------------------------//	
void CAOIBoard::SetBoardResultID_ARS(RESULT_ID value) 
{ 
	m_BoardResultID_ARS = value; 
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIBoard::GetBoardResultID_ARS() const 
{ 
	return m_BoardResultID_ARS; 
}
//-------------------------------------------------------------------------------------//	
RESULT_ID CAOIBoard::GetBoardResultID_ARS_LA() const//單板檢測結果-ARS-A軌
{
	return m_BoardResultID_ARS_LA;
}
//-------------------------------------------------------------------------------------//	
void CAOIBoard::SetBoardResultID_ARS_LA(RESULT_ID value)//單板檢測結果-ARS-A軌
{
	m_BoardResultID_ARS_LA = value;
}
//-------------------------------------------------------------------------------------//	
RESULT_ID CAOIBoard::GetBoardResultID_ARS_LB() const//單板檢測結果-ARS-B軌
{
	return m_BoardResultID_ARS_LB;
}
//-------------------------------------------------------------------------------------//	
void CAOIBoard::SetBoardResultID_ARS_LB(RESULT_ID value)//單板檢測結果-ARS-B軌
{
	m_BoardResultID_ARS_LB = value;
}
//-------------------------------------------------------------------------------------//	
void CAOIBoard::UpdateBoardResultID_ARS_Lane(LANE_ID LaneID)//單板檢測結果-ARS-軌道
{
	RESULT_ID ResultID = GetBoardResultID_ARS();
	switch ( LaneID )
	{
	case LANE_ID_A:	SetBoardResultID_ARS_LA(ResultID); break;
	case LANE_ID_B:	SetBoardResultID_ARS_LB(ResultID); break;
	}
	return;
}
//-------------------------------------------------------------------------------------//	
RESULT_ID CAOIBoard::GetBoardResultID_ARS_Lane(LANE_ID LaneID) const//單板檢測結果-ARS--軌道
{
	RESULT_ID ResultID;
	switch ( LaneID )
	{
	case LANE_ID_A:	ResultID = GetBoardResultID_ARS_LA(); break;
	case LANE_ID_B:	ResultID = GetBoardResultID_ARS_LB(); break;
	default:		ResultID = GetBoardResultID_ARS(); break;
	}
	return ResultID;
}
//-------------------------------------------------------------------------------------//	
void CAOIBoard::SetBoardResultID_Alarm(RESULT_ID value) 
{ 
	m_BoardResultID_Alarm = value; 
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIBoard::GetBoardResultID_Alarm() const 
{ 
	return m_BoardResultID_Alarm; 
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::ExecBoardBeXBoard(LANE_ID LaneID)//執行單板是為報廢板
{	//注意報廢板會導致RgnCalcState不會跑完至REGION_CALC_DONE
	size_t     i=0;
	RESULT_ID PartResultID = RESULT_ID_SKIP;
	RESULT_ID BoardResultID = RESULT_ID_SKIP;	
	SetBoardResultID(BoardResultID);
	SetBoardResultID_Alarm(BoardResultID);

	CAOIFd *FdPtr = NULL;
	const size_t FdCount = GetBoardFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetBoardFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( RESULT_ID_BYPASS == FdPtr->GetFdResultID_AOI() )
		{	continue; }
		FdPtr->BypassSkipFd(PartResultID);		
	}

	CAOIMark    *MarkPtr = NULL;
	const size_t MarkCount = GetBoardMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( RESULT_ID_BYPASS == MarkPtr->GetMarkResultID_AOI() )
		{	continue; }
		MarkPtr->BypassSkipMark(PartResultID);
	}

	CAOIBarcode *BarcodePtr = NULL;
	const size_t BarcodeCount = GetBoardBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetBoardBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( RESULT_ID_BYPASS == BarcodePtr->GetBarcodeResultID_AOI() )
		{	continue; }
		BarcodePtr->BypassSkipBarcode(PartResultID);		
	}

	CAOIComponent *ComponentPtr = NULL;
	const size_t  ComponentCount = GetBoardComponentCount_Inline();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( RESULT_ID_BYPASS == ComponentPtr->GetComponentResultID_AOI() )
		{	continue; }
		ComponentPtr->BypassSkipComponent(PartResultID);	
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::AnalyzeBoardIsXBoard()//分析單板是否為報廢板
{
	size_t         i=0;		
	bool           bIsXBoard=false;
	RESULT_ID      ResultID=RESULT_ID_NONE;
	CAOIComponent *ComponentPtr = NULL;
	const size_t  ComponentCount = GetBoardComponentCount_Inline();	

	bIsXBoard=false;
	if ( false==bIsXBoard && 0!=m_BoardXBoardUnitTotalCount )
	{
		if ( m_BoardXBoardUnitCount == m_BoardXBoardUnitTotalCount )
		{	bIsXBoard = true; }
	}
	if ( false==bIsXBoard && fabs(m_BoardXBoardCheckRatio)>0.00001 )
	{		
		int nNGCnt=0;
		int nTestCnt=0;		
		double fNGRatio=0;
		for ( i=0; i<ComponentCount; i++ )
		{
			ComponentPtr = GetBoardComponentPtr_Inline(i);
			if ( NULL == ComponentPtr ) { continue; }
			if ( ComponentPtr->GetComponentBypassed() == true ) 
			{	continue; }
			ResultID = ComponentPtr->GetComponentResultID_AOI();			
			nTestCnt ++;			
			if ( RESULT_ID_NG == ResultID )
			{	nNGCnt ++;	}
		}
		if ( 0 < nTestCnt )
		{
			fNGRatio = nNGCnt*100.0/nTestCnt;
			if ( fNGRatio > m_BoardXBoardCheckRatio )
			{	bIsXBoard = true;	}
		}
	}
	return bIsXBoard;	
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::AnalyzeBoardXBoardUnitCount()//分析單板報廢板數量
{
	size_t         i=0;
	size_t         XBoardUnitTotalCount=0;
	CAOIComponent *ComponentPtr = NULL;	
	const size_t   ComponentCount = GetBoardComponentCount_Inline();	

	XBoardUnitTotalCount=0;
	SetBoardXBoardUnitCount(0);
	SetBoardXBoardUnitTotalCount(0);
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentXBoardUnit() == false ) { continue; }
		XBoardUnitTotalCount ++;		
	}
	SetBoardXBoardUnitTotalCount(XBoardUnitTotalCount);	
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::GetBoardXBoardUnitCount() const
{
	return m_BoardXBoardUnitCount; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardXBoardUnitCount(size_t val)
{
	m_BoardXBoardUnitCount=val; 
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::IncrementBoardXBoardUnitCount()//增加單板報廢件數量	
{
	m_BoardXBoardUnitCount ++;
}
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardXBoardUnitTotalCount(size_t val)
{
	m_BoardXBoardUnitTotalCount=val; 
}
//-------------------------------------------------------------------------------------//
size_t CAOIBoard::GetBoardXBoardUnitTotalCount() const 
{ 
	return m_BoardXBoardUnitTotalCount; 
}	
//-------------------------------------------------------------------------------------//
void CAOIBoard::SetBoardRoatedAngle(double value)
{
	m_BoardRoatedAngle = value; 
}
//-------------------------------------------------------------------------------------//
double CAOIBoard::GetBoardRoatedAngle() const
{
	return m_BoardRoatedAngle; 
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::CreateBoardFieldListKernel(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, FIELD_DIVISION_MODE DivisionMode, bool ByCadRegion, bool CheckOldField, TJetRgnList &RgnList, TJetFieldList &FieldList)//建立區塊分割資料
{
	CAOIProject   *ProjectPtr = GetBoardProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	size_t         i=0, j=0, k=0;
	TREGION4D      Region, RegionAll;
	TJetRgn        JetRgn;	
	size_t         WindowCount=0;
	size_t         SubRngCount=0;	
	CAOIRgn*       RgnSubPtr = NULL;	
	CAOIField*     FieldPtr = NULL;	
	CAOIFd*        FdPtr = NULL;
	CAOIMark*      MarkPtr = NULL;
	CAOIBarcode*   BarcodePtr = NULL;
	CAOIWindow*    WindowPtr = NULL;
	CAOIComponent* ComponentPtr = NULL;
	bool           PartWndsInOneFOV = false;
	double         PosX=0, PosY=0;		
	double         FOVW_Inner=0, FOVH_Inner=0;
	double         FOVW_Outer=0, FOVH_Outer=0;		
	double         FOVW_Inside=0, FOVH_Inside=0;
	const bool     CheckInner = false;		
	const size_t   FdCount = GetBoardFdCount_Inline();
	const size_t   MarkCount = GetBoardMarkCount_Inline();
	const size_t   BarcodeCount = GetBoardBarcodeCount_Inline();
	const size_t   ComponentCount = GetBoardComponentCount_Inline();	
	CJetFieldDivider FieldDivider;//區域分割	
	AOIDataCollect.GetFovSizeInner(FOVW_Inner, FOVH_Inner);
	AOIDataCollect.GetFovSizeOuter(FOVW_Outer, FOVH_Outer);
	ProjectPtr->ModifyProjectFieldSize(FOVW_Inner, FOVH_Inner);
	//const double DummyW = JetAPI::Floor(FOVW_Inner/2) - 1;//避免浮點數計算誤差, 所以扣掉一個單位
	//const double DummyH = JetAPI::Floor(FOVH_Inner/2) - 1;//避免浮點數計算誤差, 所以扣掉一個單位
	const double DummyW2 = 250;//小尺寸寬
	const double DummyH2 = 250;//小尺寸長	
	if ( true == CheckInner )
	{
		FOVW_Inside = FOVW_Inner;
		FOVH_Inside = FOVH_Inner;
	}
	else
	{
		FOVW_Inside = FOVW_Outer;
		FOVH_Inside = FOVH_Outer;
	}

	//Fiducial
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetBoardFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdNeedToCalculate() == false ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		
		if ( true == ByCadRegion )
		{	FdPtr->GetFdExtendCadRegion(Region);	}
		else
		{	FdPtr->GetFdExtendStageRegion(Region);	}
		//確認是否跨FOV
		if ( (Region.maxX-Region.minX)>FOVW_Inside || (Region.maxY-Region.minY)>FOVH_Inside ) 
		{	PartWndsInOneFOV = false;	}
		else
		{	PartWndsInOneFOV = true;	}
		if ( false == PartWndsInOneFOV )
		{
			PosX = Region.GetCpX();
			PosY = Region.GetCpY();			
			Region.minX = PosX-DummyW2;
			Region.maxX = PosX+DummyW2;
			Region.minY = PosY-DummyH2;
			Region.maxY = PosY+DummyH2;
		}
		if ( true == CheckOldField )
		{	
			FieldPtr = FdPtr->GetFdFieldPtr();	
			if ( NULL != FieldPtr )
			{
				if ( FieldPtr->CheckRegionInField(Region, ByCadRegion, CheckInner) == false )
				{	
					FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
					FdPtr->SetFdFieldPtr(FieldPtr);
				}
			}
			else 
			{	
				FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				FdPtr->SetFdFieldPtr(FieldPtr);
			}
		}

		if ( false==CheckOldField || NULL==FieldPtr )
		{			
			JetRgn.minX = Region.minX;
			JetRgn.minY = Region.minY;
			JetRgn.maxX = Region.maxX;
			JetRgn.maxY = Region.maxY;
			JetRgn.Ptr  = FdPtr;
			FieldDivider.AddJetRgn(JetRgn);
		}

		SubRngCount = FdPtr->GetFdSubRgnCount();
		for ( k=0; k<SubRngCount; k++ )
		{
			RgnSubPtr = FdPtr->GetFdSubRgnPtr(k, false);
			if ( NULL == RgnSubPtr ) { continue; }

			if ( true == ByCadRegion )
			{	RgnSubPtr->GetRgnRoiCadRegion(Region);	}
			else
			{	RgnSubPtr->GetRgnRoiStageRegion(Region); }

			if ( true == CheckOldField )
			{
				FieldPtr = RgnSubPtr->GetRgnFieldPtr();
				if ( NULL != FieldPtr  ) 
				{
					if ( FieldPtr->CheckRegionInField(Region, ByCadRegion, CheckInner) == true )
					{	continue; }					
				}
				FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL != FieldPtr  ) 
				{	
					RgnSubPtr->SetRgnFieldPtr(FieldPtr);
					continue;	
				}
				RgnSubPtr->SetRgnFieldPtr(NULL);
			}
			JetRgn.minX = Region.minX;
			JetRgn.minY = Region.minY;
			JetRgn.maxX = Region.maxX;
			JetRgn.maxY = Region.maxY;
			JetRgn.Ptr  = RgnSubPtr;
			FieldDivider.AddJetRgn(JetRgn);
		}
	}

	//特徵點-Mark
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkNeedToCalculate() == false ) { continue; }
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }

		if ( true == ByCadRegion )
		{	MarkPtr->GetMarkRoiCadRegion(Region);	}
		else
		{	MarkPtr->GetMarkRoiStageRegion(Region);	}
		//確認是否跨FOV
		if ( (Region.maxX-Region.minX)>FOVW_Inside || (Region.maxY-Region.minY)>FOVH_Inside ) 
		{	PartWndsInOneFOV = false;	}
		else
		{	PartWndsInOneFOV = true;	}
		if ( false == PartWndsInOneFOV )
		{
			PosX = Region.GetCpX();
			PosY = Region.GetCpY();			
			Region.minX = PosX-DummyW2;
			Region.maxX = PosX+DummyW2;
			Region.minY = PosY-DummyH2;
			Region.maxY = PosY+DummyH2;
		}
		if ( true == CheckOldField )
		{	
			FieldPtr = MarkPtr->GetMarkFieldPtr();	
			if ( NULL != FieldPtr )
			{
				if ( FieldPtr->CheckRegionInField(Region, ByCadRegion, CheckInner) == false )
				{	
					FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
					MarkPtr->SetMarkFieldPtr(FieldPtr);
				}
			}
			else 
			{	
				FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				MarkPtr->SetMarkFieldPtr(FieldPtr);
			}
		}

		if ( false==CheckOldField || NULL==FieldPtr )
		{			
			JetRgn.minX = Region.minX;
			JetRgn.minY = Region.minY;
			JetRgn.maxX = Region.maxX;
			JetRgn.maxY = Region.maxY;
			JetRgn.Ptr  = MarkPtr;
			FieldDivider.AddJetRgn(JetRgn);
		}

		SubRngCount = MarkPtr->GetMarkSubRgnCount();
		for ( k=0; k<SubRngCount; k++ )
		{
			RgnSubPtr = MarkPtr->GetMarkSubRgnPtr(k, false);
			if ( NULL == RgnSubPtr ) { continue; }

			if ( true == ByCadRegion )
			{	RgnSubPtr->GetRgnRoiCadRegion(Region);	}
			else
			{	RgnSubPtr->GetRgnRoiStageRegion(Region); }

			if ( true == CheckOldField )
			{
				FieldPtr = RgnSubPtr->GetRgnFieldPtr();
				if ( NULL != FieldPtr  ) 
				{
					if ( FieldPtr->CheckRegionInField(Region, ByCadRegion, CheckInner) == true )
					{	continue; }					
				}
				FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL != FieldPtr  ) 
				{	
					RgnSubPtr->SetRgnFieldPtr(FieldPtr);
					continue;	
				}
				RgnSubPtr->SetRgnFieldPtr(NULL);
			}
			JetRgn.minX = Region.minX;
			JetRgn.minY = Region.minY;
			JetRgn.maxX = Region.maxX;
			JetRgn.maxY = Region.maxY;
			JetRgn.Ptr  = RgnSubPtr;
			FieldDivider.AddJetRgn(JetRgn);
		}
	}

	//軟體條碼
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetBoardBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeNeedToCalculate() == false ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }

		if ( true == ByCadRegion )
		{	BarcodePtr->GetBarcodeRoiCadRegion(Region);	}
		else
		{	BarcodePtr->GetBarcodeRoiStageRegion(Region);	}
		//確認是否跨FOV
		if ( (Region.maxX-Region.minX)>FOVW_Inside || (Region.maxY-Region.minY)>FOVH_Inside ) 
		{	PartWndsInOneFOV = false;	}
		else
		{	PartWndsInOneFOV = true;	}
		if ( false == PartWndsInOneFOV )
		{
			PosX = Region.GetCpX();
			PosY = Region.GetCpY();			
			Region.minX = PosX-DummyW2;
			Region.maxX = PosX+DummyW2;
			Region.minY = PosY-DummyH2;
			Region.maxY = PosY+DummyH2;
		}
		if ( true == CheckOldField )
		{	
			FieldPtr = BarcodePtr->GetBarcodeFieldPtr();	
			if ( NULL != FieldPtr )
			{
				if ( FieldPtr->CheckRegionInField(Region, ByCadRegion, CheckInner) == false )
				{	
					FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
					BarcodePtr->SetBarcodeFieldPtr(FieldPtr);
				}
			}
			else 
			{	
				FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				BarcodePtr->SetBarcodeFieldPtr(FieldPtr);
			}
		}

		if ( false==CheckOldField || NULL==FieldPtr )
		{			
			JetRgn.minX = Region.minX;
			JetRgn.minY = Region.minY;
			JetRgn.maxX = Region.maxX;
			JetRgn.maxY = Region.maxY;
			JetRgn.Ptr  = BarcodePtr;
			FieldDivider.AddJetRgn(JetRgn);
		}

		SubRngCount = BarcodePtr->GetBarcodeSubRgnCount();
		for ( k=0; k<SubRngCount; k++ )
		{
			RgnSubPtr = BarcodePtr->GetBarcodeSubRgnPtr(k, false);
			if ( NULL == RgnSubPtr ) { continue; }

			if ( true == ByCadRegion )
			{	RgnSubPtr->GetRgnRoiCadRegion(Region);	}
			else
			{	RgnSubPtr->GetRgnRoiStageRegion(Region); }

			if ( true == CheckOldField )
			{
				FieldPtr = RgnSubPtr->GetRgnFieldPtr();
				if ( NULL != FieldPtr  ) 
				{
					if ( FieldPtr->CheckRegionInField(Region, ByCadRegion, CheckInner) == true )
					{	continue; }					
				}
				FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL != FieldPtr  ) 
				{	
					RgnSubPtr->SetRgnFieldPtr(FieldPtr);
					continue;	
				}
				RgnSubPtr->SetRgnFieldPtr(NULL);
			}
			JetRgn.minX = Region.minX;
			JetRgn.minY = Region.minY;
			JetRgn.maxX = Region.maxX;
			JetRgn.maxY = Region.maxY;
			JetRgn.Ptr  = RgnSubPtr;
			FieldDivider.AddJetRgn(JetRgn);
		}
	}

	//零件
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentNeedToCalculate() == false ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }

		if ( true == ByCadRegion )
		{	ComponentPtr->GetComponentWindowCadRegion(RegionAll); }
		else
		{	ComponentPtr->GetComponentWindowStageRegion(RegionAll); }

		//零件下整個範圍是否可落於同一個FOV內
		if ( (RegionAll.maxX-RegionAll.minX)>FOVW_Inside || (RegionAll.maxY-RegionAll.minY)>FOVH_Inside ) 
		{	PartWndsInOneFOV = false;	}
		else
		{	PartWndsInOneFOV = true;	}
		
		if ( PartWndsInOneFOV == false )
		{
			/*
			if ( true == ByCadRegion )
			{	ComponentPtr->GetComponentRoiCadRegion(Region);	}
			else
			{	ComponentPtr->GetComponentRoiStageRegion(Region);	}

			if ( (Region.maxX-Region.minX) > FOVW_Inner )
			{
				if ( true == ByCadRegion )
				{	PosX = ComponentPtr->GetComponentCadPosX();	}
				else
				{	PosX = ComponentPtr->GetComponentStagePosX();	}
				Region.minX = PosX-DummyW;
				Region.maxX = PosX+DummyW;
			}
			if ( (Region.maxY-Region.minY) > FOVH_Inner )
			{
				if ( true == ByCadRegion )
				{	PosY = ComponentPtr->GetComponentCadPosY();	}
				else
				{	PosY = ComponentPtr->GetComponentStagePosY();	}
				Region.minY = PosY-DummyH;
				Region.maxY = PosY+DummyH;
			}
			*/
			//本體尺寸過大會導致FOV過多, 因此只給予單一個小的範圍//20190118
			if ( true == ByCadRegion )
			{	
				PosX = ComponentPtr->GetComponentCadPosX();	
				PosY = ComponentPtr->GetComponentCadPosY();
			}
			else
			{	
				PosX = ComponentPtr->GetComponentStagePosX();	
				PosY = ComponentPtr->GetComponentStagePosY();
			}
			Region.minX = PosX-DummyW2;
			Region.maxX = PosX+DummyW2;
			Region.minY = PosY-DummyH2;
			Region.maxY = PosY+DummyH2;
		}
		else
		{	Region = RegionAll;	}

		if ( true == CheckOldField )
		{	
			FieldPtr = ComponentPtr->GetComponentFieldPtr();	
			if ( NULL != FieldPtr )
			{
				if ( FieldPtr->CheckRegionInField(Region, ByCadRegion, CheckInner) == false )
				{	
					FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
					ComponentPtr->SetComponentFieldPtr(FieldPtr);
				}
			}
			else
			{
				FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				ComponentPtr->SetComponentFieldPtr(FieldPtr);
			}
		}

		if ( false==CheckOldField || NULL==FieldPtr )
		{	
			JetRgn.minX = Region.minX;
			JetRgn.minY = Region.minY;
			JetRgn.maxX = Region.maxX;
			JetRgn.maxY = Region.maxY;
			JetRgn.Ptr  = ComponentPtr;
			FieldDivider.AddJetRgn(JetRgn);
		}
		
		SubRngCount = ComponentPtr->GetComponentSubRgnCount();
		for ( k=0; k<SubRngCount; k++ )
		{
			RgnSubPtr = ComponentPtr->GetComponentSubRgnPtr(k, false);
			if ( NULL == RgnSubPtr ) { continue; }
			if ( true == ByCadRegion )
			{	RgnSubPtr->GetRgnRoiCadRegion(Region);	}
			else
			{	RgnSubPtr->GetRgnRoiStageRegion(Region); }
			if ( true == CheckOldField )
			{
				FieldPtr = RgnSubPtr->GetRgnFieldPtr();
				if ( NULL != FieldPtr  ) 
				{
					if ( FieldPtr->CheckRegionInField(Region, ByCadRegion, CheckInner) == true )
					{	continue; }					
				}
				FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL != FieldPtr  ) 
				{	
					RgnSubPtr->SetRgnFieldPtr(FieldPtr);
					continue;	
				}
				RgnSubPtr->SetRgnFieldPtr(NULL);
			}		

			JetRgn.minX = Region.minX;
			JetRgn.minY = Region.minY;
			JetRgn.maxX = Region.maxX;
			JetRgn.maxY = Region.maxY;
			JetRgn.Ptr  = RgnSubPtr;
			FieldDivider.AddJetRgn(JetRgn);
		}//SubRgn Loop

		
		//檢測框
		WindowCount = ComponentPtr->GetComponentWindowCount();
		for ( j=0; j<WindowCount; j++ )
		{
			WindowPtr = ComponentPtr->GetComponentWindowPtr(j, false);
			if ( NULL == WindowPtr ) { continue; }
			if ( PartWndsInOneFOV == false )
			{
				/*
				if ( true == ByCadRegion )
				{	WindowPtr->GetWindowRoiCadRegion(Region);	}
				else
				{	WindowPtr->GetWindowRoiStageRegion(Region);	}

				if ( (Region.maxX-Region.minX) > FOVW_Inner )
				{
					if ( true == ByCadRegion )
					{	PosX = WindowPtr->GetWindowCadPosX();	}
					else
					{	PosX = WindowPtr->GetWindowStagePosX();	}
					Region.minX = PosX-DummyW;
					Region.maxX = PosX+DummyW;
				}
				if ( (Region.maxY-Region.minY) > FOVH_Inner )
				{
					if ( true == ByCadRegion )
					{	PosY = WindowPtr->GetWindowCadPosY();	}
					else
					{	PosY = WindowPtr->GetWindowStagePosY();	}
					Region.minY = PosY-DummyH;
					Region.maxY = PosY+DummyH;
				}	
				*/
				//本體尺寸過大會導致FOV過多, 因此只給予單一個小的範圍//20190118
				if ( true == ByCadRegion )
				{	
					PosX = WindowPtr->GetWindowCadPosX();	
					PosY = WindowPtr->GetWindowCadPosY();
				}
				else
				{	
					PosX = WindowPtr->GetWindowStagePosX();	
					PosY = WindowPtr->GetWindowStagePosY();
				}
				Region.minX = PosX-DummyW2;
				Region.maxX = PosX+DummyW2;
				Region.minY = PosY-DummyH2;
				Region.maxY = PosY+DummyH2;
			}
			else
			{	Region = RegionAll;	}

			if ( true == CheckOldField )
			{	
				FieldPtr = WindowPtr->GetWindowFieldPtr();	
				if ( NULL != FieldPtr )
				{
					if ( FieldPtr->CheckRegionInField(Region, ByCadRegion, CheckInner) == false )
					{	
						FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
						WindowPtr->SetWindowFieldPtr(FieldPtr);
					}
				}
				else
				{
					FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
					WindowPtr->SetWindowFieldPtr(FieldPtr);
				}
			}

			if ( false==CheckOldField || NULL==FieldPtr )
			{	
				JetRgn.minX = Region.minX;
				JetRgn.minY = Region.minY;
				JetRgn.maxX = Region.maxX;
				JetRgn.maxY = Region.maxY;
				JetRgn.Ptr  = WindowPtr;
				FieldDivider.AddJetRgn(JetRgn);
			}

			SubRngCount = WindowPtr->GetWindowSubRgnCount();
			for ( k=0; k<SubRngCount; k++ )
			{
				RgnSubPtr = WindowPtr->GetWindowSubRgnPtr(k, false);
				if ( NULL == RgnSubPtr ) { continue; }
				if ( true == ByCadRegion )
				{	RgnSubPtr->GetRgnRoiCadRegion(Region);	}
				else
				{	RgnSubPtr->GetRgnRoiStageRegion(Region); }
				if ( true == CheckOldField )
				{
					FieldPtr = RgnSubPtr->GetRgnFieldPtr();
					if ( NULL != FieldPtr  ) 
					{
						if ( FieldPtr->CheckRegionInField(Region, ByCadRegion, CheckInner) == true )
						{	continue; }					
					}
					FieldPtr = MatchBoardFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
					if ( NULL != FieldPtr  ) 
					{	
						RgnSubPtr->SetRgnFieldPtr(FieldPtr);
						continue;	
					}
					RgnSubPtr->SetRgnFieldPtr(NULL);
				}				
				JetRgn.minX = Region.minX;
				JetRgn.minY = Region.minY;
				JetRgn.maxX = Region.maxX;
				JetRgn.maxY = Region.maxY;
				JetRgn.Ptr  = RgnSubPtr;
				FieldDivider.AddJetRgn(JetRgn);
			}//SubRgn Loop
		}//Window Loop
	}//Component Loop

	const size_t JetRgnCount = FieldDivider.GetJetRgnCount();	
	if ( JetRgnCount == 0 ) { return true; }	
	
	FieldDivider.SetFieldMaxW(FOVW_Inner);
	FieldDivider.SetFieldMaxH(FOVH_Inner);	
	FieldDivider.SetDivisionMode(DivisionMode);	
	if ( FieldDivider.ExecDivide() == false )
	{
		return false;
	}
	
	if ( true == ByCadRegion ) //座標轉換
	{		
		TJetRgn*     JetRgnPtr = NULL;
		TJetField*   JetFieldPtr=NULL;
		const size_t FieldCount2 = FieldDivider.GetJetFieldCount();
		double CADX=0, CADY=0;
		double StageX=0, StageY=0;
		CMapCoordinate PanelMapCTS;
		CMapCoordinate PanelMapSTC;
		const double FOVW2 = FOVW_Inner/2;
		const double FOVH2 = FOVH_Inner/2;		
		if ( GetBoardMapCTS(DistrictID, PanelMapCTS) == false ) { return false; }
		if ( GetBoardMapSTC(DistrictID, PanelMapSTC) == false ) { return false; }

		for ( i=0; i<FieldCount2; i++ )
		{
			JetFieldPtr = FieldDivider.GetJetFieldPtr(i, false);
			if ( NULL == JetFieldPtr ) { continue; }

			CADX = (JetFieldPtr->minX+JetFieldPtr->maxX)*0.5;
			CADY = (JetFieldPtr->minY+JetFieldPtr->maxY)*0.5;

			JetFieldPtr->PosCadX = CADX;
			JetFieldPtr->PosCadY = CADY;
			PanelMapCTS.Map2D(CADX, CADY, StageX, StageY);			
			JetFieldPtr->minX  = StageX-FOVW2;
			JetFieldPtr->maxX  = StageX+FOVW2;
			JetFieldPtr->minY  = StageY-FOVH2;
			JetFieldPtr->maxY  = StageY+FOVH2;
		}

		const size_t JetRgnCount2 = FieldDivider.GetJetRgnCount();		
		for ( i=0; i<JetRgnCount2; i++ )
		{
			JetRgnPtr = FieldDivider.GetJetRgnPtr(i, false);
			if ( NULL == JetRgnPtr ) { continue; }

			PanelMapCTS.Map2D(JetRgnPtr->minX, JetRgnPtr->minY, Region.minX, Region.minY);
			PanelMapCTS.Map2D(JetRgnPtr->maxX, JetRgnPtr->maxY, Region.maxX, Region.maxY);

			JetRgnPtr->minX = MIN(Region.minX, Region.maxX);
			JetRgnPtr->maxX = MAX(Region.minX, Region.maxX);
			JetRgnPtr->minY = MIN(Region.minY, Region.maxY);
			JetRgnPtr->maxY = MAX(Region.minY, Region.maxY);			
		}
	}
	FieldDivider.CloneJetRgnList(RgnList);
	FieldDivider.CloneJetFieldList(FieldList);	
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIField* CAOIBoard::MatchBoardFieldPtr(const TREGION4D &Region, bool CadMode, bool Inner, DISTRICT_ID DistrictID)
{
	size_t      i=0;
	double      Dist = 0;
	double      DistX=0, DistY=0;
	double      FieldCpX=0, FieldCpY=0;
	double      BestDist = 0;
	CAOIField*  FieldPtr = NULL;
	CAOIField*  BestFieldPtr = NULL;
	const double RegionCpX = Region.GetCpX();
	const double RegionCpY = Region.GetCpY();
	const size_t FieldCount = GetBoardFieldCount_Inline();

	BestFieldPtr = NULL;
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetBoardFieldPtr_Inline(i);
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
bool CAOIBoard::ResetBoardFrameImageRect(DISTRICT_ID DistrictID)//重新設定單板下物件的畫面影像位置
{
	size_t         i=0, j=0, k=0;	
	size_t         RgnIdx = 0;
	TREGION4D      Region, CameraRgn;		
	size_t         SubRgnCount = 0;
	size_t         WindowCount = 0;
	CAOIRgn*       RgnPtr = NULL;
	CAOIRgn*       SubRgnPtr = NULL;
	CAOIMark*      MarkPtr = NULL;	
	CAOIField*     FieldPtr = NULL;	
	CAOIWindow*    WindowPtr = NULL;
	CAOIBarcode*   BarcodePtr = NULL;	
	CAOIComponent* ComponentPtr = NULL;	
	const bool     CheckInner = false;
	const bool     ByCadRegion = true;	
	const size_t   MarkCount = GetBoardMarkCount_Inline();
	const size_t   FieldCount = GetBoardFieldCount_Inline();
	const size_t   BarcodeCount = GetBoardBarcodeCount_Inline();
	const size_t   ComponentCount = GetBoardComponentCount();		
	
	//Assign Field To Mark
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }		
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }

		SubRgnCount = MarkPtr->GetMarkSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = MarkPtr->GetMarkSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			FieldPtr = SubRgnPtr->GetRgnFieldPtr();
			if ( NULL == FieldPtr ) { continue; }
			if ( SubRgnPtr->CalcRgnFrameImageRect() == false )
			{	return false; }			
		}

		FieldPtr = MarkPtr->GetMarkFieldPtr();
		if ( NULL == FieldPtr ) { continue; }
		if ( MarkPtr->CalcRgnFrameImageRect() == false )
		{	return false; }		
	}

	//Assign Field To Barcode
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetBoardBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }

		SubRgnCount = BarcodePtr->GetBarcodeSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = BarcodePtr->GetBarcodeSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }
			FieldPtr = SubRgnPtr->GetRgnFieldPtr();
			if ( NULL == FieldPtr ) { continue; }
			if ( SubRgnPtr->CalcRgnFrameImageRect() == false )
			{	return false; }			
		}

		FieldPtr = BarcodePtr->GetBarcodeFieldPtr();
		if ( NULL == FieldPtr ) { continue; }
		if ( BarcodePtr->CalcRgnFrameImageRect() == false )
		{	return false; }		
	}
	
	//Assign Field To Component Window, window first then component
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }			
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }

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
				if ( NULL == FieldPtr ) { continue; }
				if ( SubRgnPtr->CalcRgnFrameImageRect() == false )
				{	return false; }					
			}

			FieldPtr = WindowPtr->GetWindowFieldPtr();
			if ( NULL == FieldPtr ) { continue; }
			if ( WindowPtr->CalcRgnFrameImageRect() == false )
			{	return false; }			
		}

		SubRgnCount = ComponentPtr->GetComponentSubRgnCount();
		for ( k=0; k<SubRgnCount; k++ )
		{
			SubRgnPtr = ComponentPtr->GetComponentSubRgnPtr(k, false);
			if ( NULL == SubRgnPtr ) { continue; }			
			FieldPtr = SubRgnPtr->GetRgnFieldPtr();
			if ( NULL == FieldPtr ) { continue; }
			if ( SubRgnPtr->CalcRgnFrameImageRect() == false )
			{	return false; }			
		}

		FieldPtr = ComponentPtr->GetComponentFieldPtr();
		if ( NULL == FieldPtr ) { continue; }
		if ( ComponentPtr->CalcRgnFrameImageRect() == false )
		{	return false; }		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::AssignBoardLocalPlaneParam(int GroupID, bool bInvertY, const CJetGroundEquation &BoardGround)//曲面參數
{	
	CAOIBoard *BoardPtr = this;
	if ( NULL == BoardPtr ) { return false; }
	size_t   i=0;	
	double x=0, y=0, z=0;
	
	TPOINT2D ObjectPt;	
	int    ObjectImageW = 0;
	int    ObjectImageH = 0;
	double ObjectStartX = 0;
	double ObjectStartY = 0;
	double ObjectRegionW = 0;
	double ObjectRegionH = 0;	
	TREGION4D BoardRgn;
	TREGION4D ObjectRgn;
	TPOINT3D ObjectPlaneParam;	
	CAOIModel     *ModelPtr = NULL;		
	const double ExpandW = 0;
	const double ExpandH = 0;
	CJetGroundEquation ObjectGround;
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;		
	GROUND_EQUATION_MODE ObjectGroundMode=GROUND_EQUATION_PLANE;

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

	CAOIMark    *MarkPtr = NULL;	
	const size_t MarkCount = GetBoardMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{		
		MarkPtr = GetBoardMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkLocalBasePlaneID() != GroupID ) { continue; }		

		MarkPtr->GetMarkRoiCadRegion(ObjectRgn);
		ObjectStartX=ObjectRgn.minX;
		ObjectStartY=ObjectRgn.minY;
		ObjectRegionW=ObjectRgn.GetWidth();
		ObjectRegionH=ObjectRgn.GetHeight();
		ObjectImageW = (int)((ObjectRegionW/ResX)+0.5);
		ObjectImageH = (int)((ObjectRegionH/ResY)+0.5);
		ObjectGround.InitGroundParam(ObjectGroundMode);

		//Point 1
		ObjectPt.x=ObjectRgn.minX;
		ObjectPt.y=ObjectRgn.maxY;
		x = ObjectPt.x;
		y = ObjectPt.y;
		x = (x-BoardStartX)/ResX;
		if ( false == bInvertY ) 
		{	y = (y-BoardStartY)/ResY;	}
		else
		{	y = BoardImageH-((y-BoardStartY)/ResY); }
		z = BoardGround.CalcGroundValue(x, y);
		x = ((ObjectPt.x-ObjectStartX)/ResX);//轉成零件影像的座標
		y = ObjectImageH-((ObjectPt.y-ObjectStartY)/ResY);
		ObjectGround.AddGroundValue(x, y, z);

		//Point 2
		ObjectPt.x=ObjectRgn.maxX;
		ObjectPt.y=ObjectRgn.maxY;
		x = ObjectPt.x;
		y = ObjectPt.y;
		x = (x-BoardStartX)/ResX;
		if ( false == bInvertY ) 
		{	y = (y-BoardStartY)/ResY;	}
		else
		{	y = BoardImageH-((y-BoardStartY)/ResY); }
		z = BoardGround.CalcGroundValue(x, y);
		x = ((ObjectPt.x-ObjectStartX)/ResX);//轉成零件影像的座標
		y = ObjectImageH-((ObjectPt.y-ObjectStartY)/ResY);		
		ObjectGround.AddGroundValue(x, y, z);	

		//Point 3		
		ObjectPt.x=ObjectRgn.minX;
		ObjectPt.y=ObjectRgn.minY;
		x = ObjectPt.x;
		y = ObjectPt.y;
		x = (x-BoardStartX)/ResX;
		if ( false == bInvertY ) 
		{	y = (y-BoardStartY)/ResY;	}
		else
		{	y = BoardImageH-((y-BoardStartY)/ResY); }
		z = BoardGround.CalcGroundValue(x, y);
		x = ((ObjectPt.x-ObjectStartX)/ResX);//轉成零件影像的座標
		y = ObjectImageH-((ObjectPt.y-ObjectStartY)/ResY);
		ObjectGround.AddGroundValue(x, y, z);

		//Point 4
		ObjectPt.x=ObjectRgn.maxX;
		ObjectPt.y=ObjectRgn.minY;
		x = ObjectPt.x;
		y = ObjectPt.y;
		x = (x-BoardStartX)/ResX;
		if ( false == bInvertY ) 
		{	y = (y-BoardStartY)/ResY;	}
		else
		{	y = BoardImageH-((y-BoardStartY)/ResY); }
		z = BoardGround.CalcGroundValue(x, y);
		x = ((ObjectPt.x-ObjectStartX)/ResX);//轉成零件影像的座標
		y = ObjectImageH-((ObjectPt.y-ObjectStartY)/ResY);
		ObjectGround.AddGroundValue(x, y, z);

		//Point 5
		ObjectPt.x=(ObjectRgn.minX+ObjectRgn.maxX)*0.5;
		ObjectPt.y=(ObjectRgn.maxY+ObjectRgn.maxY)*0.5;
		x = ObjectPt.x;
		y = ObjectPt.y;
		x = (x-BoardStartX)/ResX;
		if ( false == bInvertY ) 
		{	y = (y-BoardStartY)/ResY;	}
		else
		{	y = BoardImageH-((y-BoardStartY)/ResY); }
		z = BoardGround.CalcGroundValue(x, y);
		x = ((ObjectPt.x-ObjectStartX)/ResX);//轉成零件影像的座標
		y = ObjectImageH-((ObjectPt.y-ObjectStartY)/ResY);
		ObjectGround.AddGroundValue(x, y, z);

		ObjectGround.CalcGroundParam();		
		ObjectGround.GetGroundNormal(ObjectPlaneParam.x, ObjectPlaneParam.y, ObjectPlaneParam.z);		
		MarkPtr->SetMarkLocalBasePlaneParam(ObjectPlaneParam);
		TBasePlaneParam &BasePlaneParamRef = MarkPtr->GetMarkSpaceBasePlaneParam();
		BasePlaneParamRef.LocalGroundEquation = ObjectGround;

		ModelPtr = MarkPtr->GetMarkModelPtr();
		if ( NULL != ModelPtr )
		{
			TBasePlaneParam &ModelBasePlaneParamRef = ModelPtr->GetModelSpaceBasePlaneParam();
			ModelBasePlaneParamRef.LocalGroundEquation = ObjectGround;	
		}
		MarkPtr->SetMarkLocalBasePlaneFinish(true);
	}

	//Barcode
	CAOIBarcode *BarcodePtr = NULL;	
	const size_t BarcodeCount = GetBoardBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{		
		BarcodePtr = GetBoardBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeLocalBasePlaneID() != GroupID ) { continue; }

		BarcodePtr->GetBarcodeRoiCadRegion(ObjectRgn);
		ObjectStartX=ObjectRgn.minX;
		ObjectStartY=ObjectRgn.minY;
		ObjectRegionW=ObjectRgn.GetWidth();
		ObjectRegionH=ObjectRgn.GetHeight();
		ObjectImageW = (int)((ObjectRegionW/ResX)+0.5);
		ObjectImageH = (int)((ObjectRegionH/ResY)+0.5);
		ObjectGround.InitGroundParam(ObjectGroundMode);

		//Point 1
		ObjectPt.x=ObjectRgn.minX;
		ObjectPt.y=ObjectRgn.maxY;
		x = ObjectPt.x;
		y = ObjectPt.y;
		x = (x-BoardStartX)/ResX;
		if ( false == bInvertY ) 
		{	y = (y-BoardStartY)/ResY;	}
		else
		{	y = BoardImageH-((y-BoardStartY)/ResY); }
		z = BoardGround.CalcGroundValue(x, y);
		x = ((ObjectPt.x-ObjectStartX)/ResX);//轉成零件影像的座標
		y = ObjectImageH-((ObjectPt.y-ObjectStartY)/ResY);
		ObjectGround.AddGroundValue(x, y, z);

		//Point 2
		ObjectPt.x=ObjectRgn.maxX;
		ObjectPt.y=ObjectRgn.maxY;
		x = ObjectPt.x;
		y = ObjectPt.y;
		x = (x-BoardStartX)/ResX;
		if ( false == bInvertY ) 
		{	y = (y-BoardStartY)/ResY;	}
		else
		{	y = BoardImageH-((y-BoardStartY)/ResY); }
		z = BoardGround.CalcGroundValue(x, y);
		x = ((ObjectPt.x-ObjectStartX)/ResX);//轉成零件影像的座標
		y = ObjectImageH-((ObjectPt.y-ObjectStartY)/ResY);		
		ObjectGround.AddGroundValue(x, y, z);	

		//Point 3		
		ObjectPt.x=ObjectRgn.minX;
		ObjectPt.y=ObjectRgn.minY;
		x = ObjectPt.x;
		y = ObjectPt.y;
		x = (x-BoardStartX)/ResX;
		if ( false == bInvertY ) 
		{	y = (y-BoardStartY)/ResY;	}
		else
		{	y = BoardImageH-((y-BoardStartY)/ResY); }
		z = BoardGround.CalcGroundValue(x, y);
		x = ((ObjectPt.x-ObjectStartX)/ResX);//轉成零件影像的座標
		y = ObjectImageH-((ObjectPt.y-ObjectStartY)/ResY);
		ObjectGround.AddGroundValue(x, y, z);

		//Point 4
		ObjectPt.x=ObjectRgn.maxX;
		ObjectPt.y=ObjectRgn.minY;
		x = ObjectPt.x;
		y = ObjectPt.y;
		x = (x-BoardStartX)/ResX;
		if ( false == bInvertY ) 
		{	y = (y-BoardStartY)/ResY;	}
		else
		{	y = BoardImageH-((y-BoardStartY)/ResY); }
		z = BoardGround.CalcGroundValue(x, y);
		x = ((ObjectPt.x-ObjectStartX)/ResX);//轉成零件影像的座標
		y = ObjectImageH-((ObjectPt.y-ObjectStartY)/ResY);
		ObjectGround.AddGroundValue(x, y, z);

		//Point 5
		ObjectPt.x=(ObjectRgn.minX+ObjectRgn.maxX)*0.5;
		ObjectPt.y=(ObjectRgn.maxY+ObjectRgn.maxY)*0.5;
		x = ObjectPt.x;
		y = ObjectPt.y;
		x = (x-BoardStartX)/ResX;
		if ( false == bInvertY ) 
		{	y = (y-BoardStartY)/ResY;	}
		else
		{	y = BoardImageH-((y-BoardStartY)/ResY); }
		z = BoardGround.CalcGroundValue(x, y);
		x = ((ObjectPt.x-ObjectStartX)/ResX);//轉成零件影像的座標
		y = ObjectImageH-((ObjectPt.y-ObjectStartY)/ResY);
		ObjectGround.AddGroundValue(x, y, z);

		ObjectGround.CalcGroundParam();		
		ObjectGround.GetGroundNormal(ObjectPlaneParam.x, ObjectPlaneParam.y, ObjectPlaneParam.z);		
		BarcodePtr->SetBarcodeLocalBasePlaneParam(ObjectPlaneParam);
		TBasePlaneParam &BasePlaneParamRef = BarcodePtr->GetBarcodeSpaceBasePlaneParam();
		BasePlaneParamRef.LocalGroundEquation = ObjectGround;

		ModelPtr = BarcodePtr->GetBarcodeModelPtr();
		if ( NULL != ModelPtr )
		{
			TBasePlaneParam &ModelBasePlaneParamRef = ModelPtr->GetModelSpaceBasePlaneParam();
			ModelBasePlaneParamRef.LocalGroundEquation = ObjectGround;
		}
		BarcodePtr->SetBarcodeLocalBasePlaneFinish(true);
	}
	
	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = BoardPtr->GetBoardComponentCount();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = BoardPtr->GetBoardComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentLocalBasePlaneID() != GroupID ) { continue; }		
		//if ( ComponentPtr->GetComponentNeedToCalculate() == false ) { continue; }

		ComponentPtr->GetComponentRoiCadRegion(ObjectRgn);		
		ObjectStartX=ObjectRgn.minX;
		ObjectStartY=ObjectRgn.minY;
		ObjectRegionW=ObjectRgn.GetWidth();
		ObjectRegionH=ObjectRgn.GetHeight();
		ObjectImageW = (int)((ObjectRegionW/ResX)+0.5);
		ObjectImageH = (int)((ObjectRegionH/ResY)+0.5);
		ObjectGround.InitGroundParam(ObjectGroundMode);

		//Point 1
		ObjectPt.x=ObjectRgn.minX;
		ObjectPt.y=ObjectRgn.maxY;
		x = ObjectPt.x;
		y = ObjectPt.y;
		x = (x-BoardStartX)/ResX;
		if ( false == bInvertY ) 
		{	y = (y-BoardStartY)/ResY;	}
		else
		{	y = BoardImageH-((y-BoardStartY)/ResY); }
		z = BoardGround.CalcGroundValue(x, y);
		x = ((ObjectPt.x-ObjectStartX)/ResX);//轉成零件影像的座標
		y = ObjectImageH-((ObjectPt.y-ObjectStartY)/ResY);
		ObjectGround.AddGroundValue(x, y, z);

		//Point 2
		ObjectPt.x=ObjectRgn.maxX;
		ObjectPt.y=ObjectRgn.maxY;
		x = ObjectPt.x;
		y = ObjectPt.y;
		x = (x-BoardStartX)/ResX;
		if ( false == bInvertY ) 
		{	y = (y-BoardStartY)/ResY;	}
		else
		{	y = BoardImageH-((y-BoardStartY)/ResY); }
		z = BoardGround.CalcGroundValue(x, y);
		x = ((ObjectPt.x-ObjectStartX)/ResX);//轉成零件影像的座標
		y = ObjectImageH-((ObjectPt.y-ObjectStartY)/ResY);		
		ObjectGround.AddGroundValue(x, y, z);

		//Point 3		
		ObjectPt.x=ObjectRgn.minX;
		ObjectPt.y=ObjectRgn.minY;
		x = ObjectPt.x;
		y = ObjectPt.y;
		x = (x-BoardStartX)/ResX;
		if ( false == bInvertY ) 
		{	y = (y-BoardStartY)/ResY;	}
		else
		{	y = BoardImageH-((y-BoardStartY)/ResY); }
		z = BoardGround.CalcGroundValue(x, y);
		x = ((ObjectPt.x-ObjectStartX)/ResX);//轉成零件影像的座標
		y = ObjectImageH-((ObjectPt.y-ObjectStartY)/ResY);
		ObjectGround.AddGroundValue(x, y, z);

		//Point 4
		ObjectPt.x=ObjectRgn.maxX;
		ObjectPt.y=ObjectRgn.minY;
		x = ObjectPt.x;
		y = ObjectPt.y;
		x = (x-BoardStartX)/ResX;
		if ( false == bInvertY ) 
		{	y = (y-BoardStartY)/ResY;	}
		else
		{	y = BoardImageH-((y-BoardStartY)/ResY); }
		z = BoardGround.CalcGroundValue(x, y);
		x = ((ObjectPt.x-ObjectStartX)/ResX);//轉成零件影像的座標
		y = ObjectImageH-((ObjectPt.y-ObjectStartY)/ResY);
		ObjectGround.AddGroundValue(x, y, z);

		//Point 5
		ObjectPt.x=(ObjectRgn.minX+ObjectRgn.maxX)*0.5;
		ObjectPt.y=(ObjectRgn.maxY+ObjectRgn.maxY)*0.5;
		x = ObjectPt.x;
		y = ObjectPt.y;
		x = (x-BoardStartX)/ResX;
		if ( false == bInvertY ) 
		{	y = (y-BoardStartY)/ResY;	}
		else
		{	y = BoardImageH-((y-BoardStartY)/ResY); }
		z = BoardGround.CalcGroundValue(x, y);
		x = ((ObjectPt.x-ObjectStartX)/ResX);//轉成零件影像的座標
		y = ObjectImageH-((ObjectPt.y-ObjectStartY)/ResY);
		ObjectGround.AddGroundValue(x, y, z);

		ObjectGround.CalcGroundParam();		
		ObjectGround.GetGroundNormal(ObjectPlaneParam.x, ObjectPlaneParam.y, ObjectPlaneParam.z);		
		ComponentPtr->SetComponentLocalBasePlaneParam(ObjectPlaneParam);
		TBasePlaneParam &BasePlaneParamRef = ComponentPtr->GetComponentSpaceBasePlaneParam();
		BasePlaneParamRef.LocalGroundEquation = ObjectGround;

		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL != ModelPtr )
		{
			TBasePlaneParam &ModelBasePlaneParamRef = ModelPtr->GetModelSpaceBasePlaneParam();
			ModelBasePlaneParamRef.LocalGroundEquation = ObjectGround;
		}
		ComponentPtr->SetComponentLocalBasePlaneFinish(true);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBoard::BuildBoardPartBarcodeList(std::vector<TPartBarcode> &PartBarcodeList)//建立單板內零件條碼列表
{
	size_t         i=0, j=0;		
	TPartBarcode   PartBarcode;	
	CAOIComponent *ComponentPtr = NULL;		
	std::vector<std::wstring> BarcodeList;
	const size_t   ComponentCount = GetBoardComponentCount();		
	
	PartBarcodeList.clear();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetBoardComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->GetComponentBypassed() == true ) { continue; }

		BarcodeList.clear();
		if ( ComponentPtr->BuildComponentBarcodeList(BarcodeList) == false )
		{	continue; }	

		const size_t BarcodeCount=BarcodeList.size();
		for ( j=0; j<BarcodeCount; j++ )
		{
			PartBarcode.wsName = ComponentPtr->GetComponentName();		
			PartBarcode.wsCode = BarcodeList[j];
			PartBarcodeList.push_back(PartBarcode);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//