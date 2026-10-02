// AOIBarcode.cpp: implementation of the CAOIBarcode class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIBarcode.h"
//-------------------------------------------------------------------------------------//
#include "AOIFileIO.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CAOIBarcode, CAOIRgn)
//-------------------------------------------------------------------------------------//
CAOIBarcode::CAOIBarcode():CAOIRgn(AOI_OBJ_BARCODE)
{
	PreInitBarcode();
	InitialBarcode();
}
//-------------------------------------------------------------------------------------//
CAOIBarcode::CAOIBarcode(const CAOIBarcode &barcode):CAOIRgn(barcode)
{
	PreInitBarcode();
	CloneBarcode(barcode);
}
//-------------------------------------------------------------------------------------//
CAOIBarcode::~CAOIBarcode()
{

}
//-------------------------------------------------------------------------------------//
CAOIBarcode& CAOIBarcode::operator=(const CAOIBarcode &barcode)
{
	if ( this == &barcode ) { return *this; }
	CAOIRgn::operator=(barcode);
	CloneBarcode(barcode);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::PreInitBarcode()
{	
	m_BarcodeResultText.clear();
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::InitialBarcode()
{	
	m_BarcodeDeleted = false;//軟體條碼是否刪除
	m_BarcodeSelected = false;//軟體條碼是否選取到
	m_BarcodeUniqueID = -1;
	m_BarcodeGroupID = -1;
	m_BarcodeSpreadMode = BARCODE_SPREAD_OFF;
	m_BarcodeBelongMode = BARCODE_BELONG_NONE;
	SetBarcodeSaveTestImageMode(SAVE_TEST_IMAGE_DEFECT);
	//---------------------------------------------------------------------------------//	
	m_BarcodeTempInt[0] = 0;
	m_BarcodeTempInt[1] = 0;
	m_BarcodeTempInt[2] = 0;
	m_BarcodeTempInt[3] = 0;
	//---------------------------------------------------------------------------------//	
	m_BarcodeModel.SetModelBarcodePtr(this);
	m_BarcodeModel.SetModelIsolated(false);
	m_BarcodeModel.SetModelExtendRangeX(0);
	m_BarcodeModel.SetModelExtendRangeY(0);
	m_BarcodeModel.SetModelExtendAutoAdjust(false);
	//---------------------------------------------------------------------------------//		
	m_BarcodeResultText=L"";
	//---------------------------------------------------------------------------------//
	m_BarcodeConfirmUIResultID = 0;
	//---------------------------------------------------------------------------------//		
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::CloneBarcode(const CAOIBarcode &barcode)
{	
	m_BarcodeDeleted = barcode.m_BarcodeDeleted;//軟體條碼是否刪除
	m_BarcodeSelected = barcode.m_BarcodeSelected;//軟體條碼是否選取到
	m_BarcodeUniqueID = barcode.m_BarcodeUniqueID;
	m_BarcodeGroupID = barcode.m_BarcodeGroupID;
	m_BarcodeSpreadMode = barcode.m_BarcodeSpreadMode;
	m_BarcodeBelongMode = barcode.m_BarcodeBelongMode;
	//---------------------------------------------------------------------------------//
	m_BarcodeTempInt[0] = barcode.m_BarcodeTempInt[0];
	m_BarcodeTempInt[1] = barcode.m_BarcodeTempInt[1];
	m_BarcodeTempInt[2] = barcode.m_BarcodeTempInt[2];
	m_BarcodeTempInt[3] = barcode.m_BarcodeTempInt[3];
	//---------------------------------------------------------------------------------//
	m_BarcodeModel = barcode.m_BarcodeModel;
	m_BarcodeModel.SetModelBarcodePtr(this);	
	//---------------------------------------------------------------------------------//		
	m_BarcodeResultText=barcode.m_BarcodeResultText;
	m_BarcodeConfirmUIResultID = barcode.m_BarcodeConfirmUIResultID;	
	//---------------------------------------------------------------------------------//		
}
//-------------------------------------------------------------------------------------//
CAOIBarcode* CAOIBarcode::CloneBarcodeObj() const//建立且複製一個軟體條碼
{
	CAOIBarcode *ObjPtr = AOIObjManager.CreateBarcodeObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
CString CAOIBarcode::GetBarcodeFullName() const//取得軟體條碼的名稱	
{	
	unsigned int BarcodeIndex = 0;
	CAOIProject *ProjectPtr = GetBarcodeProjectPtr();
	if ( NULL == ProjectPtr )
	{	BarcodeIndex = m_RgnIdx;	}
	else
	{	BarcodeIndex = m_RgnIndex_Project;	}
	CString Name=AOIDataDefine.GetBarcodeFullName(BarcodeIndex, _T("Barcode"));	
	return Name;
}
//-------------------------------------------------------------------------------------//
CString CAOIBarcode::GetRgnDerivedName() const//取得區域的名稱
{
	return GetBarcodeFullName();	
}
//-------------------------------------------------------------------------------------//
CString CAOIBarcode::GetRgnDerivedKeyName() const//取得軟體條碼的名稱	
{
	return CString(_T("Barcode"));
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::CreateBarcodeSelfFieldPtr()//建立專屬Field指標	
{
	bool bIsOK = true;
	bIsOK = CAOIRgn::CreateRgnSelfFieldPtr();
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeFieldPtr(CAOIField *Ptr)
{
	CAOIRgn::SetRgnFieldPtr(Ptr);
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::GetBarcodeModelImageIsSaved() const//條碼是否儲存過圖像
{
	return CAOIRgn::GetRgnModelImageIsSaved();
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeModelImageIsSaved(bool val)//條碼是否儲存過圖像
{
	CAOIRgn::SetRgnModelImageIsSaved(val);
}
//-------------------------------------------------------------------------------------//
SAVE_TEST_IMAGE_MODE CAOIBarcode::GetBarcodeSaveTestImageMode() const//取得條碼儲存影像模式
{
	return CAOIRgn::GetRgnSaveTestImageMode();
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeSaveTestImageMode(SAVE_TEST_IMAGE_MODE value)//設定條碼儲存影像模式
{
	CAOIRgn::SetRgnSaveTestImageMode(value);
}
//-------------------------------------------------------------------------------------//
int CAOIBarcode::GetBarcodeUniqueID() const
{ 
	return m_BarcodeUniqueID; 
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeUniqueID(int value)
{ 
	m_BarcodeUniqueID = value; 
}
//-------------------------------------------------------------------------------------//
int CAOIBarcode::GetBarcodeGroupID() const
{
	return m_BarcodeGroupID;
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeGroupID(int value)
{
	m_BarcodeGroupID = value;
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::CheckBarcodeGroupIDValid() const//確認條碼群組編號有效
{
	if ( -1 == m_BarcodeGroupID ) { return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
BARCODE_SPREAD_MODE CAOIBarcode::GetBarcodeSpreadMode() const//取得條碼擴散模式
{
	return m_BarcodeSpreadMode;
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeSpreadMode(BARCODE_SPREAD_MODE value)//設定條碼擴散模式
{
	m_BarcodeSpreadMode = value;
}
//-------------------------------------------------------------------------------------//
BARCODE_BELONG_MODE CAOIBarcode::CheckBarcodeBelongMode()//確認條碼屬於模式
{
	BARCODE_BELONG_MODE BarcodeBelongMode=GetBarcodeBelongMode();
	if ( BARCODE_BELONG_NONE != BarcodeBelongMode ) { return BarcodeBelongMode; }
	const int PanelIndex=GetBarcodePanelIndex_Project();
	const int BoardIndex=GetBarcodeBoardIndex_Project();
	if ( -1 == PanelIndex )
	{	BarcodeBelongMode = BARCODE_BELONG_PROJECT; }
	else
	{
		if ( -1 != BoardIndex ) { BarcodeBelongMode = BARCODE_BELONG_BOARD; }
		else { BarcodeBelongMode = BARCODE_BELONG_PANEL; }
	}
	SetBarcodeBelongMode(BarcodeBelongMode);
	return BarcodeBelongMode;
}
//-------------------------------------------------------------------------------------//
BARCODE_BELONG_MODE CAOIBarcode::GetBarcodeBelongMode() const
{
	return m_BarcodeBelongMode;
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeBelongMode(BARCODE_BELONG_MODE value)
{
	m_BarcodeBelongMode = value;
}
//-------------------------------------------------------------------------------------//
LANE_ID CAOIBarcode::GetBarcodeLaneID() const
{
	return CAOIRgn::GetRgnLaneID();
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeLaneID(LANE_ID value)
{
	CAOIRgn::SetRgnLaneID(value);
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeBypassed(bool value)
{
	CAOIRgn::SetRgnBypassed(value);
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::UpdateBarcodeBypassed()//確認零件是否為不檢測
{
	bool bBypassed = false;
	CAOIPanel *PanelPtr = NULL;
	CAOIBoard *BoardPtr = NULL;
	
	bBypassed = GetBarcodeBypassed();
	if ( false == bBypassed )
	{
		BoardPtr = GetBarcodeBoardPtr();
		if ( NULL != BoardPtr )
		{
			if ( BoardPtr->GetBoardBypassed() == true ) 
			{	bBypassed = true; }
		}
	}
	if ( false == bBypassed )
	{
		PanelPtr = GetBarcodePanelPtr();
		if ( NULL != PanelPtr )
		{
			if ( PanelPtr->GetPanelBypassed() == true ) 
			{	bBypassed = true; }
		}
	}
	
	RESULT_ID  ModelResultID=RESULT_ID_NONE;
	CAOIModel *ModelPtr = GetBarcodeModelPtr();
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
		SetBarcodeNeedToCalculate(false);
		SetBarcodeResultID_AOI(RESULT_ID_BYPASS);
		SetBarcodeResultID_Alarm(RESULT_ID_BYPASS);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
int CAOIBarcode::CalcBarcodeOpenMPCountByPixels()
{
	return CalcRgnOpenMPCountByPixels();
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeOpenMPCount(int value)
{ 
	SetRgnOpenMPCount(value); 
	CAOIModel *ModelPtr = GetBarcodeModelPtr();	
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelOpenMPCount(value); }
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeResultText(const char *value)
{
	if ( NULL == value ) { return; }
	JetAPI::char2wstring(value, m_BarcodeResultText);	
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeResultText(const wchar_t *value)
{
	if ( NULL == value ) { return; }
	m_BarcodeResultText = value;
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::BypassSkipBarcode(RESULT_ID value)//不檢測或跳過條碼
{
	CAOIBarcode *BarcodePtr=this;
	LANE_ID LaneID = BarcodePtr->GetBarcodeLaneID();
	BarcodePtr->SetBarcodeNeedToCalculate(false);		
	BarcodePtr->SetBarcodeNeedToCalculateBackup(false);
	BarcodePtr->SetBarcodeResultID_AOI(value);
	BarcodePtr->SetBarcodeResultID_Alarm(value);
	BarcodePtr->UpdateBarcodeResultID_AOI_Lane(LaneID);

	CAOIModel *ModelPtr = BarcodePtr->GetBarcodeModelPtr();
	ModelPtr->SetModelResultID(value);
	ModelPtr->SetModelResultID_Alarm(value);
	ModelPtr->BypassSkipModelWnd(value);
	return;
}
//-------------------------------------------------------------------------------------//
RESULT_ID  CAOIBarcode::GetBarcodeResultID_AOI() const//取得條碼結果編號
{
	return CAOIRgn::GetRgnResultID_AOI();
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeResultID_AOI(RESULT_ID value)	//設定條碼結果編號
{
	CAOIRgn::SetRgnResultID_AOI(value);
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIBarcode::GetBarcodeResultID_AOI_LA() const//取得條碼結果編號-A軌
{
	return CAOIRgn::GetRgnResultID_AOI_LA();
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeResultID_AOI_LA(RESULT_ID value)	//設定條碼結果編號-A軌
{
	CAOIRgn::SetRgnResultID_AOI_LA(value);
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIBarcode::GetBarcodeResultID_AOI_LB() const//取得條碼結果編號-B軌
{
	return CAOIRgn::GetRgnResultID_AOI_LB();
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeResultID_AOI_LB(RESULT_ID value)	//設定條碼結果編號-B軌
{
	CAOIRgn::SetRgnResultID_AOI_LB(value);
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::UpdateBarcodeResultID_AOI_Lane(LANE_ID LaneID)//更新條碼結果編號-軌道
{
	RESULT_ID ResultID = GetBarcodeResultID_AOI();
	switch ( LaneID )
	{
	case LANE_ID_A:	SetBarcodeResultID_AOI_LA(ResultID);	break;
	case LANE_ID_B:	SetBarcodeResultID_AOI_LB(ResultID);	break;	
	}
	return;
}
//-------------------------------------------------------------------------------------//
RESULT_ID  CAOIBarcode::GetBarcodeResultID_AOI_Lane(LANE_ID LaneID) const//取得條碼結果編號-軌道
{
	RESULT_ID ResultID;
	switch ( LaneID )
	{
	case LANE_ID_A:	ResultID = GetBarcodeResultID_AOI_LA();	break;
	case LANE_ID_B:	ResultID = GetBarcodeResultID_AOI_LB();	break;
	default:		ResultID = GetBarcodeResultID_AOI();	break;
	}
	return ResultID;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIBarcode::GetBarcodeResultID_ARS() const//取得條碼結果編號
{
	return CAOIRgn::GetRgnResultID_ARS();
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeResultID_ARS(RESULT_ID value)//設定條碼結果編號	
{
	CAOIRgn::SetRgnResultID_ARS(value);
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIBarcode::GetBarcodeResultID_ARS_LA() const//取得條碼結果編號-A軌
{
	return CAOIRgn::GetRgnResultID_ARS_LA();
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeResultID_ARS_LA(RESULT_ID value)	//設定條碼結果編號-A軌
{
	CAOIRgn::SetRgnResultID_ARS_LA(value);
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIBarcode::GetBarcodeResultID_ARS_LB() const//取得條碼結果編號-B軌
{
	return CAOIRgn::GetRgnResultID_ARS_LB();
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeResultID_ARS_LB(RESULT_ID value)	//設定條碼結果編號-B軌
{
	CAOIRgn::SetRgnResultID_ARS_LB(value);
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::UpdateBarcodeResultID_ARS_Lane(LANE_ID LaneID)//更新條碼結果編號-軌道
{
	RESULT_ID ResultID = GetBarcodeResultID_ARS();
	switch ( LaneID )
	{
	case LANE_ID_A:	SetBarcodeResultID_ARS_LA(ResultID);	break;
	case LANE_ID_B:	SetBarcodeResultID_ARS_LB(ResultID);	break;	
	}
	return;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIBarcode::GetBarcodeResultID_ARS_Lane(LANE_ID LaneID) const//取得條碼結果編號-軌道
{
	RESULT_ID ResultID;
	switch ( LaneID )
	{
	case LANE_ID_A:	ResultID = GetBarcodeResultID_ARS_LA();	break;
	case LANE_ID_B:	ResultID = GetBarcodeResultID_ARS_LB();	break;
	default:		ResultID = GetBarcodeResultID_ARS();	break;
	}
	return ResultID;
}
//-------------------------------------------------------------------------------------//
RESULT_ID  CAOIBarcode::GetBarcodeResultID_Alarm() const//取得條碼結果編號-警報
{
	return CAOIRgn::GetRgnResultID_Alarm();
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeResultID_Alarm(RESULT_ID value)//設定條碼結果編號-警報
{
	CAOIRgn::SetRgnResultID_Alarm(value);
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::MoveBarcodeCadPos(double dX, double dY)//移動軟體條碼座標	
{
	CAOIRgn::MoveRgnCadPos(dX, dY);
	m_BarcodeModel.SetModelAttachedPosCad(m_RgnCadPos);
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::MoveBarcodeStagePos(double dX, double dY)//移動軟體條碼座標	
{
	CAOIRgn::MoveRgnStagePos(dX, dY);
	m_BarcodeModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);	
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::MoveBarcodePos(double dX, double dY, CMapCoordinate *MapPtr)//移動軟體條碼座標
{
	MoveBarcodeCadPos(dX, dY);	
	if ( NULL != MapPtr )
	{
		MapBarcodeCadToStagePos(*MapPtr);
		m_BarcodeModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);	
	}
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIBarcode::GetBarcodeRoiRgnCad() const//軟體條碼範圍-Cad
{
	TREGION4D Region;
	JetAPI::PointsToRegion(m_RgnRoiCadCornerPos, 4, Region);
	return Region;	
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIBarcode::GetBarcodeBodyRgnCad() const//軟體條碼範圍-Cad
{
	TREGION4D Region;
	JetAPI::PointsToRegion(m_RgnBodyCadCornerPos, 4, Region);
	return Region;
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIBarcode::GetBarcodeRoiRgnStage() const//軟體條碼範圍-Stage
{
	TREGION4D Region;
	JetAPI::PointsToRegion(m_RgnRoiStageCornerPos, 4, Region);
	return Region;
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIBarcode::GetBarcodeBodyRgnStage() const//軟體條碼範圍-Stage
{
	TREGION4D Region;
	JetAPI::PointsToRegion(m_RgnBodyStageCornerPos, 4, Region);
	return Region;
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::GetBarcodeRoiCadRegion(TREGION4D &Region)//取得軟體條碼在Cad的範圍
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
void CAOIBarcode::GetBarcodeBodyCadRegion(TREGION4D &Region)//取得軟體條碼在Cad的範圍	
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
void CAOIBarcode::GetBarcodeRoiStageRegion(TREGION4D &Region)//取得軟體條碼在Stage的範圍
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
void CAOIBarcode::GetBarcodeBodyStageRegion(TREGION4D &Region)//取得軟體條碼在Stage的範圍		
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
bool CAOIBarcode::CheckBarcodeBePickByCad(const TPOINT2D &PickPos)//確認軟體條碼被點擊到
{
	TREGION4D    Region = CAOIBarcode::GetBarcodeRoiRgnCad();
	if ( PickPos.x<Region.minX || PickPos.y<Region.minY || 
		 PickPos.x>Region.maxX || PickPos.y>Region.maxY )
	{	return false; }
	return true; 
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::CheckBarcodeBePickByStage(const TPOINT2D &PickPos)//確認軟體條碼被點擊到
{
	TREGION4D    Region = CAOIBarcode::GetBarcodeRoiRgnStage();		
	if ( PickPos.x<Region.minX || PickPos.y<Region.minY || 
		 PickPos.x>Region.maxX || PickPos.y>Region.maxY )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::CheckBarcodeInRegionByCad(const TREGION4D &SelRgn, bool bEntireIn)//確認軟體條碼在範圍內
{
	TREGION4D    Region = CAOIBarcode::GetBarcodeRoiRgnCad();		
	return JetAPI::CheckRgnInRegion(Region, SelRgn, bEntireIn);
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::CheckBarcodeInRegionByStage(const TREGION4D &SelRgn, bool bEntireIn)//確認軟體條碼在範圍內
{
	TREGION4D    Region = CAOIBarcode::GetBarcodeRoiRgnStage();		
	return JetAPI::CheckRgnInRegion(Region, SelRgn, bEntireIn);
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::CalcBarcodeCadCornerPos()//計算軟體條碼Cad端點座標	
{
	CAOIRgn::CalcRgnCadCornerPos();
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::LayoutBarcodeStageCornerPos()//更新軟體條碼機台端點座標
{
	CAOIRgn::LayoutRgnStageCornerPos();
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::MapBarcodeCadToStagePos(const CMapCoordinate &Map)//將軟體條碼CAD轉成機台座標
{
	CAOIRgn::MapRgnCadToStagePos(Map);	
	const double SagePosX = CAOIRgn::GetRgnStagePosX();
	const double SagePosY = CAOIRgn::GetRgnStagePosY();
	CAOIBarcode::m_BarcodeModel.SetModelAttachedPosStage(SagePosX, SagePosY);
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeFrameImageSize_um(const TSIZE2D &value)
{ 
	CAOIRgn::SetRgnFrameImageSize_um(value);
	m_BarcodeModel.SetModelImageSize_um(value);
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeFrameImageCadOffset_um(const TPOINT2D &value)
{
	CAOIRgn::SetRgnFrameImageCadOffset_um(value);
	m_BarcodeModel.SetModelImageCadOffset_um(value);
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeFrameImageStageOffset_um(const TPOINT2D &value)
{
	TPOINT2D CadOffset;
	AOIDataCollect.MapStageOffsetPtToCad(value, CadOffset);
	SetBarcodeFrameImageCadOffset_um(CadOffset);
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::SpinBarcode(double Angle, CMapCoordinate *MapPtr)//軟體條碼自旋轉
{
	double CadAngle = 0;
	double StageAngle = 0;
	CadAngle = Angle;
	AOIDataCollect.MapCadAngleToStage(CadAngle, StageAngle);
	const double CadCpX = CAOIRgn::m_RgnCadPos.x;
	const double CadCpY = CAOIRgn::m_RgnCadPos.y;
	const double StageCpX = CAOIRgn::m_RgnStagePos.x;
	const double StageCpY = CAOIRgn::m_RgnStagePos.y;

	CAOIBarcode::RotateBarcode(CadAngle, CadCpX, CadCpY, MapPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::RotateBarcode(double Angle, double CpX, double CpY, CMapCoordinate *MapPtr)//軟體條碼旋轉
{
	CAOIRgn::RotateRgnCad(Angle, CpX, CpY);
	CAOIRgn::LayoutRgnStageCornerPos();
	
	CAOIBarcode::m_BarcodeModel.RotateModel(Angle, 0, 0);	
	CAOIBarcode::m_BarcodeModel.SetModelAttachedPosCad(m_RgnCadPos);
	if ( NULL != MapPtr )
	{
		MapBarcodeCadToStagePos(*MapPtr);
		CAOIBarcode::m_BarcodeModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::MirrorXBarcode(double CpX, CMapCoordinate *MapPtr)//軟體條碼鏡射-X
{
	CAOIRgn::MirrorXRgnCad(CpX);
	CAOIBarcode::m_BarcodeModel.MirrorModelYAxis(0);
	CAOIBarcode::m_BarcodeModel.SetModelAttachedPosCad(m_RgnCadPos);	
	CAOIBarcode::m_BarcodeModel.SetModelAttachedAngle(m_RgnAngle);
	if ( NULL != MapPtr )
	{
		MapBarcodeCadToStagePos(*MapPtr);
		CAOIBarcode::m_BarcodeModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::MirrorYBarcode(double CpY, CMapCoordinate *MapPtr)//軟體條碼鏡射-Y
{
	CAOIRgn::MirrorYRgnCad(CpY);
	CAOIBarcode::m_BarcodeModel.MirrorModelXAxis(0);
	CAOIBarcode::m_BarcodeModel.SetModelAttachedPosCad(m_RgnCadPos);	
	CAOIBarcode::m_BarcodeModel.SetModelAttachedAngle(m_RgnAngle);
	if ( NULL != MapPtr )
	{
		MapBarcodeCadToStagePos(*MapPtr);
		CAOIBarcode::m_BarcodeModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::ExtractRgnDerivedFrame(bool &Finished)//挖取零件圖片
{
	if ( CAOIRgn::ExtractRgnFrame(Finished) == false ) { return false; }

	if ( true == Finished )
	{	
		bool IsOK = true;
		IsOK = CAOIBarcode::ExecBarcodeInspection();
		ClearRgnMaskBuffer_Base();
		if ( GetBarcodeKeepImage() == false )
		{	CAOIRgn::ClearRgnImageBuffer();		}
		if ( false == IsOK )
		{	return false; }		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::CreateBarcodeSubRgnList(FIELD_BUILD_MODE BuildMode, bool ByCadRegion)//建立軟體條碼子檢測區域列表
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
void CAOIBarcode::ClearBarcodeSubRgnList()//清除軟體條碼的子列表
{
	CAOIRgn::ClearRgnSubList();
}
//-------------------------------------------------------------------------------------//
size_t CAOIBarcode::GetBarcodeSubRgnCount() const//取得軟體條碼的子數量
{
	return CAOIRgn::m_RgnSubList.size();
}
//-------------------------------------------------------------------------------------//
CAOIRgn* CAOIBarcode::GetBarcodeSubRgnPtr(size_t index, bool check) const//取得軟體條碼的子指標	
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
bool CAOIBarcode::ExecRgnDerivedInspection()//執行軟體條碼檢測
{
	return ExecBarcodeInspection();	
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::WriteBarcodeFile(CAOIFileIO &FileIO)//儲存軟體條碼檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	CAOIBarcode *pBarcode = this;
	char         uuidStr[MAX_JET_PATH]="";	
	wchar_t      uuidWStr[MAX_JET_PATH]=L"";	
	UUID         uuid = pBarcode->GetObjUuid();	
	FileIO.SetFnName(_T("CAOIBarcode::WriteBarcodeFile"));
	JetAPI::UUIDToStringA(uuid, uuidStr);
	JetAPI::UUIDToStringW(uuid, uuidWStr);
	//----------------------------------------------------------------------------------------//
	//軟體條碼參數
	if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_START, 0) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_UNIQUE_ID, pBarcode->GetBarcodeUniqueID()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_INDEX_PROJECT, pBarcode->GetBarcodeIndex_Project()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_PANEL_INDEX, pBarcode->GetBarcodePanelIndex_Project()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_BOARD_INDEX, pBarcode->GetBarcodeBoardIndex_Project()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BARCODE_ANGLE, pBarcode->GetBarcodeAngle()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BARCODE_BODY_SIZE_CX, pBarcode->GetBarcodeBodySizeW()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BARCODE_BODY_SIZE_CY, pBarcode->GetBarcodeBodySizeH()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BARCODE_ROI_SIZE_CX, pBarcode->GetBarcodeRoiSizeW()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BARCODE_ROI_SIZE_CY, pBarcode->GetBarcodeRoiSizeH()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_BARCODE_CAD_POS_X, pBarcode->GetBarcodeCadPosX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BARCODE_CAD_POS_Y, pBarcode->GetBarcodeCadPosY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BARCODE_STAGE_POS_X, pBarcode->GetBarcodeStagePosX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BARCODE_STAGE_POS_Y, pBarcode->GetBarcodeStagePosY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_BARCODE_STAGE_POS_Z, pBarcode->GetBarcodeStagePosZ()) == false ) { return false; }
	if ( FileIO.GetSaveWStr() == true ) 
	{	if ( FileIO.SaveChunk_STR(FILE_IO_BARCODE_OBJ_UUID, uuidWStr) == false ) { return false; } }
	else
	{	if ( FileIO.SaveChunk_STR(FILE_IO_BARCODE_OBJ_UUID, uuidStr) == false ) { return false; } }	
	if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_SAVE_TEST_IMAGE_MODE, pBarcode->GetBarcodeSaveTestImageMode()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_DISTRICT_ID, pBarcode->GetBarcodeDistrictID()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_GROUP_ID, pBarcode->GetBarcodeGroupID()) == false ) { return false; }		                     
	if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_LOCAL_BASE_PLANE_ID, pBarcode->GetBarcodeLocalBasePlaneID()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_BELONG_MODE, pBarcode->GetBarcodeBelongMode()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_SPREAD_OUT, pBarcode->GetBarcodeSpreadMode()) == false ) { return false; }	

	//軟體條碼-模組參數
	CAOIModel *ModelPtr = pBarcode->GetBarcodeModelPtr();
	if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_MODEL_NODE, 0) == false ) { return false; }
	if ( ModelPtr->WriteModelFile(FileIO) == false )
	{	return false; }
	
	if ( FileIO.SaveChunk_INT(FILE_IO_BARCODE_END, 0) == false ) { return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::ReadBarcodeFile(CAOIFileIO &FileIO)//載入軟體條碼檔案
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
	CAOIBarcode *pBarcode = this;		
	FileIO.SetFnName(_T("CAOIBarcode::ReadBarcodeFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		
		switch ( index )
		{
		case FILE_IO_BARCODE_START://軟體條碼參數-起點//未必會進來
			break;
		case FILE_IO_BARCODE_END://軟體條碼參數-終點			
			pBarcode->CheckBarcodeBelongMode();
			pBarcode->CalcBarcodeCadCornerPos();
			pBarcode->LayoutBarcodeStageCornerPos();
			pBarcode->UpdateBarcodeParamToModel();

			ModelPtr = pBarcode->GetBarcodeModelPtr();
			ModelPtr->UnSelectModel();
			WndPtr = pBarcode->GetBarcodeWndPtr();
			if ( NULL != WndPtr )
			{	WndPtr->SetWndSelected(true);	}
			ModelPtr->SetModelWndActived(WndPtr);		
			return true;
			break;
		case FILE_IO_BARCODE_UNIQUE_ID:
			pBarcode->SetBarcodeUniqueID(FileIO.GetData_INT());
			break;
		case FILE_IO_BARCODE_INDEX_PROJECT:
			pBarcode->SetBarcodeIndex_Project(FileIO.GetData_INT());
			break;
		case FILE_IO_BARCODE_PANEL_INDEX:
			pBarcode->SetBarcodePanelIndex_Project(FileIO.GetData_INT());
			break;
		case FILE_IO_BARCODE_BOARD_INDEX:
			pBarcode->SetBarcodeBoardIndex_Project(FileIO.GetData_INT());
			break;
		case FILE_IO_BARCODE_ANGLE:
			pBarcode->SetBarcodeAngle(FileIO.GetData_DBL());
			break;
		case FILE_IO_BARCODE_BODY_SIZE_CX:
			pBarcode->SetBarcodeBodySizeW(FileIO.GetData_DBL());
			break;
		case FILE_IO_BARCODE_BODY_SIZE_CY:
			pBarcode->SetBarcodeBodySizeH(FileIO.GetData_DBL());
			break;
		case FILE_IO_BARCODE_ROI_SIZE_CX:
			pBarcode->SetBarcodeRoiSizeW(FileIO.GetData_DBL());
			break;
		case FILE_IO_BARCODE_ROI_SIZE_CY:
			pBarcode->SetBarcodeRoiSizeH(FileIO.GetData_DBL());
			break;		
		case FILE_IO_BARCODE_CAD_POS_X:
			pBarcode->SetBarcodeCadPosX(FileIO.GetData_DBL());
			break;
		case FILE_IO_BARCODE_CAD_POS_Y:
			pBarcode->SetBarcodeCadPosY(FileIO.GetData_DBL());
			break;
		case FILE_IO_BARCODE_STAGE_POS_X:
			pBarcode->SetBarcodeStagePosX(FileIO.GetData_DBL());
			break;
		case FILE_IO_BARCODE_STAGE_POS_Y:
			pBarcode->SetBarcodeStagePosY(FileIO.GetData_DBL());
			break;
		case FILE_IO_BARCODE_STAGE_POS_Z:
			pBarcode->SetBarcodeStagePosZ(FileIO.GetData_DBL());
			break;
		case FILE_IO_BARCODE_OBJ_UUID://軟體條碼參數-OBJ-UUID
			if ( FileIO.GetLoadWStr()==true )
			{
				if ( JetAPI::UUIDFromStringW(FileIO.GetData_WSTR(), uuid) == true )
				{	pBarcode->SetObjUuid(uuid);	}
			}
			else
			{
				if ( JetAPI::UUIDFromStringA(FileIO.GetData_STR(), uuid) == true )
				{	pBarcode->SetObjUuid(uuid);	}
			}
			break;
		case FILE_IO_BARCODE_SAVE_TEST_IMAGE_MODE://軟體條碼參數-儲存檢測圖檔模式
			pBarcode->SetBarcodeSaveTestImageMode((SAVE_TEST_IMAGE_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_BARCODE_DISTRICT_ID://軟體條碼參數-分段編號
			pBarcode->SetBarcodeDistrictID((DISTRICT_ID)(FileIO.GetData_INT()));
			break;
		case FILE_IO_BARCODE_GROUP_ID://軟體條碼參數-群組編號
			pBarcode->SetBarcodeGroupID(FileIO.GetData_INT());
			break;
		case FILE_IO_BARCODE_LOCAL_BASE_PLANE_ID://軟體條碼參數-局部基準面編號
			pBarcode->SetBarcodeLocalBasePlaneID(FileIO.GetData_INT());
			break;
		case FILE_IO_BARCODE_BELONG_MODE://軟體條碼參數-屬於模式
			pBarcode->SetBarcodeBelongMode((BARCODE_BELONG_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_BARCODE_SPREAD_OUT://軟體條碼參數-擴展模式
			pBarcode->SetBarcodeSpreadMode((BARCODE_SPREAD_MODE)(FileIO.GetData_INT()));
			break;
		case FILE_IO_BARCODE_MODEL_NODE:
			ModelPtr = pBarcode->GetBarcodeModelPtr();
			if ( NULL != ModelPtr )
			{
				ModelName = ModelPtr->GetModelName();
				ModelFolder.Format(_T("%s\\%s"), FileIO.GetLibraryFolder(), ModelName);
				ModelPtr->SetModelFolderModel(ModelFolder);

				uValue = pBarcode->GetBarcodeIndex_Project();
				BarcodeName = AOIDataDefine.GetBarcodeFullName(uValue, _T("Barcode"));
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
CAOIModel* CAOIBarcode::GetBarcodeModelPtr()//取得條碼模組
{
	return &m_BarcodeModel;
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::UpdateBarcodeParamToModel()//更新條碼參數至模組內
{
	TPOINT2D PosCad, PosStage;	
	double SizeW = CAOIBarcode::GetBarcodeRoiSizeW();
	double SizeH = CAOIBarcode::GetBarcodeRoiSizeH();
	double BodyW = CAOIBarcode::GetBarcodeBodySizeW();
	double BodyH = CAOIBarcode::GetBarcodeBodySizeH();
	double ModelBodySizeW = BodyW;
	double ModelBodySizeH = BodyH;
	const double Angle = CAOIBarcode::GetBarcodeAngle();
	const double CadPosX = CAOIBarcode::GetBarcodeCadPosX();
	const double CadPosY = CAOIBarcode::GetBarcodeCadPosY();
	const double StagePosX = CAOIBarcode::GetBarcodeStagePosX();
	const double StagePosY = CAOIBarcode::GetBarcodeStagePosY();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(Angle);
	CAOIBox *BoxPtr = m_BarcodeModel.GetModelBodyBoxPtr();
	
	PosCad.x = CadPosX;
	PosCad.y = CadPosY;
	PosStage.x = StagePosX;
	PosStage.y = StagePosY;
	
	JetAPI::RotateSize(Angle, ModelBodySizeW, ModelBodySizeH);
	BoxPtr->SetBoxSize(ModelBodySizeW, ModelBodySizeH, true);

	if ( false == IsExceptionAngle )
	{	m_BarcodeModel.SetModelAttachedAngle(Angle);	}
	else
	{	
		m_BarcodeModel.SetModelAttachedAngle(0);
		m_BarcodeModel.RotateModel(Angle, 0, 0);
	}		
	m_BarcodeModel.SetModelBarcodePtr(this);
	m_BarcodeModel.SetModelAttachedPosCad(PosCad);
	m_BarcodeModel.SetModelAttachedPosStage(PosStage);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::UpdateBarcodeModelFromLibrary(CAOIModel *RefModelPtr)//更新條碼模組
{
	CAOIBarcode *BarcodePtr = this;
	if ( NULL == BarcodePtr ) { return false; }
	if ( NULL == RefModelPtr ) { return false; }

	const bool bClearDst=true;
	const bool bClearSrc=false;	
	CAOIModel  &ModelObj=m_BarcodeModel;	
	CString ModelName = ModelObj.GetModelName();
	CString ModelFolder = ModelObj.GetModelFolder();
	CString RefModelFolder = RefModelPtr->GetModelFolder();			
	if ( ModelFolder.CompareNoCase(RefModelFolder) != 0 ) 
	{	JetAPI::CopyFolderAToFolderB(RefModelFolder, ModelFolder, bClearSrc, bClearDst, _T(""), -1, -1);	}	

	const double BarcodeAngle = GetBarcodeAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(BarcodeAngle);
	
	ModelObj = *RefModelPtr;
	ModelObj.SetModelAttachedAngle(0);	
	ModelObj.RotateModel(BarcodeAngle, 0, 0);

	ModelObj.SetModelBarcodePtr(this);
	ModelObj.SetModelName(ModelName);
	ModelObj.SetModelFolderModel(ModelFolder);
	ModelObj.AssignModelFolder();	
	ModelObj.UpdateModelBodyToBarcode();	

	const TNoiseFilterParam &NoiseFilterParam = GetBarcodeSpaceNoiseFilterParam();
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

	const double PanelBasePlane=GetBarcodePanelBasePlane();
	ModelObj.SetModelPanelBasePlane(PanelBasePlane);	

	const bool DataModelEnabled=GetBarcodeDataModelEnabled();
	ModelObj.SetModelDataModelEnabled(DataModelEnabled);
	const int DataModelLevelID=GetBarcodeDataModelLevelID();
	ModelObj.SetModelDataModelLevelID(DataModelLevelID);
	return true;	
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::InitBarcodeInspection()//初始化條碼檢測
{
	LANE_ID LaneID = GetBarcodeLaneID();
	m_BarcodeResultText=L"";
	ClearRgnImageBuffer();
	SetBarcodeKeepImage(false);	
	SetBarcodeConfirmUIResultID(0);
	SetBarcodeFillImageTime(0.0);
	SetBarcodeModelImageIsSaved(false);
	SetBarcodeResultID_AOI(RESULT_ID_NONE);
	SetBarcodeResultID_Alarm(RESULT_ID_NONE);		
	UpdateBarcodeResultID_AOI_Lane(LaneID);
	//條碼的3D處理需要降低
	double PanelBasePlane=GetBarcodePanelBasePlane();
	TNoiseFilterParam &NoiseFilterParam = GetRgnSpaceNoiseFilterParam();
	
	NoiseFilterParam.DataVoidExpandEnabled = false;
	NoiseFilterParam.DataFirstFilterMode = DATA_NF_DISABLE;
	NoiseFilterParam.DataOverLowFTMode = DATA_NF_DISABLE;
	NoiseFilterParam.DataHeightFTMode = DATA_NF_DISABLE;
	NoiseFilterParam.DataVoidReContructed = false;
	NoiseFilterParam.DataFinalFilterMode = DATA_NF_DISABLE;
	NoiseFilterParam.DataFinalFilterMode2 = DATA_NF_DISABLE;
	NoiseFilterParam.BasePlaneParam.PanelBasePlane=PanelBasePlane;

	m_BarcodeModel.SetModelPanelBasePlane(PanelBasePlane);
	m_BarcodeModel.SetModelSpaceBasePlaneParam(NoiseFilterParam.BasePlaneParam);
	m_BarcodeModel.SetModelSpaceNoiseFilterParam(NoiseFilterParam);

	m_BarcodeModel.InitModelInspection(false);	
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::ExecBarcodeInspection()//執行條碼檢測
{
	std::vector<TUNI_FRAME> UniFrameList;
	if ( CAOIRgn::GetRgnUniFrameList(UniFrameList) == false ) { return false; }	
	CAOIModel *ModelPtr = GetBarcodeModelPtr();
	CAOIProject *ProjectPtr = GetBarcodeProjectPtr();
	const size_t UniFrameCount = UniFrameList.size();
	if ( 0 == UniFrameCount ) 
	{
		LANE_ID LaneID = GetBarcodeLaneID();
		RESULT_ID ResultID = RESULT_ID_EXCEPTION;
		SetBarcodeResultID_AOI(ResultID);
		SetBarcodeResultID_Alarm(ResultID);
		UpdateBarcodeResultID_AOI_Lane(LaneID);
		ModelPtr->SetModelResultID(ResultID);
		ModelPtr->SetModelResultID_Alarm(ResultID);		
		return true; 		
	}
	
	CString    str;		
	CString    ModelName;
	CString    BarcodeName;	
	const unsigned int BarcodeIndex = GetBarcodeIndex_Project();
	ModelName = ModelPtr->GetModelName();
	BarcodeName = AOIDataDefine.GetBarcodeFullName(BarcodeIndex, _T("Barcode"));	

#ifdef _DEBUG
	BOOL bSave = FALSE;
	if ( bSave == TRUE )
	{
		size_t     i=0;		
		CString    AttachedName;
		TUNI_FRAME UniFrame;
		AttachedName = BarcodeName;
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
	const double BarcodeW = GetBarcodeRoiSizeW();
	const double BarcodeH = GetBarcodeRoiSizeH();
	AOIDataCollect.MapImageSizeToReal(CameraID, ImageW, ImageH, ImageSizeUm);
	SetBarcodeFrameImageSize_um(ImageSizeUm);//修正正跨FOV的尺寸

	ModelPtr->GetModelTotalRegion(rgnModel);
	const double ModelW = rgnModel.GetWidth();
	const double ModelH = rgnModel.GetHeight();
	ModelPtr->ExecModelInspection(UniFrameList);	
	UpdateBarcodeResultID();
	ModelPtr->CalcModelImageRect_CustomerAI(ImageW, ImageH);
	ExecBarcodeSaveDefectImage(UniFrameList);	
	RESULT_ID Result = GetBarcodeResultID_AOI();
	if ( RESULT_ID_NG==Result || RESULT_ID_EXCEPTION==Result )
	{	SetBarcodeKeepImage(true);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::ExecBarcodeSaveDefectImage(const std::vector<TUNI_FRAME> &UniFrameList)//執行條碼儲存瑕疵圖片
{
	bool IsOK = true;
	IsOK = CAOIRgn::ExecRgnSaveDefectImage(UniFrameList);
	if ( false == IsOK )
	{	return false; }

	CAOIProject *ProjectPtr = GetBarcodeProjectPtr();
	const bool bSaveModelImage = GetBarcodeModelImageIsSaved();
	if ( true==bSaveModelImage && NULL!=ProjectPtr )
	{			
		const int BarcodeSaveImage = ProjectPtr->GetProjectParameter().m_BarcodeCameraSaveImageEnabled;
		if ( FN_ENABLE == BarcodeSaveImage )
		{
			unsigned int FrameIndex=0;
			CAOIWnd *WndPtr = GetBarcodeWndPtr();
			std::vector<TUNI_FRAME> UniFrameListTmp;
			CString  DerivedName=GetRgnDerivedName();
			const double SpaceRatio = ProjectPtr->GetProjectSpaceToGrayRatio();
			CString  BarcodeFolder = ProjectPtr->GetProjectOnlineBarcodeFolder();
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
				Filename.Format(_T("%s\\%s#%s.PNG"), BarcodeFolder, InspectionDateTime, DerivedName);
				ImageAPI.SaveUniFrameImage(Filename, UniFrameListTmp, true, bEnhance, bSave3D, bAppend, SpaceRatio);
			}
		}
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::UpdateBarcodeResultID()//更新檢測框檢測結果
{
	size_t     i=0, idx=0;	
	ALG_TYPE   AlgType;
	WND_DEFECT_ID  WndDefectID;	
	CAOIWnd   *WndPtr = NULL;	
	RESULT_ID  ResultID = RESULT_ID_NONE;	
	RESULT_ID  LogicResultID = RESULT_ID_NONE;	
	CAOIModel   *ModelPtr = &(m_BarcodeModel);	
	const LANE_ID LaneID = GetBarcodeLaneID();
	const size_t WndOrderCount = ModelPtr->GetModelWndOrderCount();

	for ( i=0; i<WndOrderCount; i++ )
	{
		idx = i;
		//idx = WndOrderCount-i-1;
		WndPtr = ModelPtr->GetModelWndOrderPtr(idx, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		if ( WND_DEFECT_BODY_WRONG_CODE != WndDefectID ) { continue; }

		CAlgParam &AlgParam = WndPtr->GetWndAlgParam();		
		AlgType = AlgParam.GetAlgType();
		if ( ALG_BARCODE_RECOGNIZE != AlgType ) { continue; }
		ResultID = WndPtr->GetWndResultID();
		LogicResultID = WndPtr->GetWndLogicResultID();
		TALG_PARAM_BARCODE_RECOGNIZE &barParam = AlgParam.GetAlgParamBarcodeRecognize();
		SetBarcodeResultText(barParam.brBarcodeResult.c_str());
		ResultID = AlgParam.GetAlgResultID();
		SetBarcodeResultID_AOI(ResultID);
		SetBarcodeResultID_Alarm(ResultID);
		UpdateBarcodeResultID_AOI_Lane(LaneID);
		if ( LogicResultID == ResultID )//邏輯結果與檢測結果相同視為找到
		{
			if ( RESULT_ID_OK == ResultID )
			{	break;	}
		}
	}

	if ( RESULT_ID_NONE == ResultID )
	{	
		SetBarcodeResultID_AOI(RESULT_ID_SKIP);	
		SetBarcodeResultID_Alarm(RESULT_ID_SKIP);	
		UpdateBarcodeResultID_AOI_Lane(LaneID);
	}		
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::UpdateBarcodeFrameIndex(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList)
{
	unsigned int  FrameIndex=0;
	unsigned int  FrameUniqueID = 0;
	const size_t  FrameIndexMapSize = FrameIndexMapList.size();

	FrameUniqueID = GetBarcodeFrameUniqueID();
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
	SetBarcodeFrameIndex(FrameIndex);
	SetBarcodeFrameUniqueID(FrameUniqueID);

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
	//SetComponentMaskFrameIndex_Base(FrameIndex);
	//SetComponentMaskFrameUniqueID_Base(FrameUniqueID);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodePanelBasePlane(double val)
{
	SetRgnPanelBasePlane(val);
	CAOIModel *ModelPtr = GetBarcodeModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelPanelBasePlane(val); }
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeSpaceBasePlaneParam(const TBasePlaneParam& Param)
{
	SetRgnSpaceBasePlaneParam(Param);
	CAOIModel *ModelPtr = GetBarcodeModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelSpaceBasePlaneParam(Param); }
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeSpaceNoiseFilterParam(const TNoiseFilterParam& Param)
{ 
	SetRgnSpaceNoiseFilterParam(Param); 
	CAOIModel *ModelPtr = GetBarcodeModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelSpaceNoiseFilterParam(Param); }
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIBarcode::GetBarcodeWndPtr()//取得條碼檢測框指標
{
	CAOIModel *ModelPtr = GetBarcodeModelPtr();
	if ( NULL == ModelPtr ) { return NULL; }
	CAOIWnd   *WndPtr = ModelPtr->GetModelWndPtrByDefectID(WND_DEFECT_BODY_WRONG_CODE);
	return WndPtr;
}
//-------------------------------------------------------------------------------------//
WND_LOGIC_TYPE CAOIBarcode::GetBarcodeLogicType()//檢測框邏輯樣式	
{
	WND_LOGIC_TYPE WndLogicType=WND_LOGIC_NONE;	
	CAOIWnd   *WndPtr = GetBarcodeWndPtr();
	if ( NULL == WndPtr ) { return WndLogicType; }
	WndLogicType = WndPtr->GetWndLogicType();
	return WndLogicType;
}
//-------------------------------------------------------------------------------------//		
int CAOIBarcode::GetBarcodeLogicGroupID()//檢測框邏輯群組編號	
{
	int WndLogicGroupID=0;
	CAOIWnd   *WndPtr = GetBarcodeWndPtr();
	if ( NULL == WndPtr ) { return WndLogicGroupID; }
	WndLogicGroupID = WndPtr->GetWndLogicGroupID();
	return WndLogicGroupID;
}
//-------------------------------------------------------------------------------------//
bool CAOIBarcode::GetBarcodeDataModelEnabled() const//取得條碼資料模型啟用
{
	return CAOIRgn::GetRgnDataModelEnabled();
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeDataModelEnabled(bool val)//設定條碼資料模型啟用
{
	CAOIRgn::SetRgnDataModelEnabled(val);	
	CAOIModel *ModelPtr = GetBarcodeModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelDataModelEnabled(val); }
}
//-------------------------------------------------------------------------------------//
int CAOIBarcode::GetBarcodeDataModelLevelID() const//取得條碼資料模型等級
{
	return CAOIRgn::GetRgnDataModelLevelID();
}
//-------------------------------------------------------------------------------------//
void CAOIBarcode::SetBarcodeDataModelLevelID(int val)//設定條碼資料模型等級
{
	CAOIRgn::SetRgnDataModelLevelID(val);	
	CAOIModel *ModelPtr = GetBarcodeModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelDataModelLevelID(val); }
}
//-------------------------------------------------------------------------------------//