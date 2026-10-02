// AOIPanel.cpp: implementation of the CAOIPanel class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIPanel.h"
//-------------------------------------------------------------------------------------//
#include "AOIFileIO.h"
#include "JetFieldDivider.h"
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
IMPLEMENT_DYNAMIC(CAOIPanel, CAOIObj)
//-------------------------------------------------------------------------------------//
CAOIPanel::CAOIPanel():CAOIObj(AOI_OBJ_PANEL)
{
	PreInitPanel();
	InitialPanel();
}
//-------------------------------------------------------------------------------------//
CAOIPanel::CAOIPanel(const CAOIPanel &panel):CAOIObj(panel)
{
	PreInitPanel();
	ClonePanel(panel);
}
//-------------------------------------------------------------------------------------//
CAOIPanel::~CAOIPanel()
{
	RemovePanelAllObjects();
}
//-------------------------------------------------------------------------------------//
CAOIPanel& CAOIPanel::operator=(const CAOIPanel &panel)
{
	if ( this == &panel ) { return *this; }
	CAOIObj::operator=(panel);
	ClonePanel(panel);
	return *this; 
}
//-------------------------------------------------------------------------------------//
inline void CAOIPanel::PreInitPanel()
{	
	m_PanelBarcode.clear();
	m_PanelBarcodeBackup.clear();
}
//-------------------------------------------------------------------------------------//
inline void CAOIPanel::InitialPanel()
{
	//---------------------------------------------------------------------------------//
	
	//---------------------------------------------------------------------------------//
	m_PanelProjectPtr = NULL;//整板所屬的專案指標
	//---------------------------------------------------------------------------------//
	m_PanelIndex_Project = -1;//整板的引數編號
	//---------------------------------------------------------------------------------//
	m_PanelTempInt[0] = 0;//整板暫存整數
	m_PanelTempInt[1] = 0;//整板暫存整數
	m_PanelTempInt[2] = 0;//整板暫存整數
	m_PanelTempInt[3] = 0;//整板暫存整數
	//---------------------------------------------------------------------------------//
	m_PanelDeleted = false;//是否刪除
	m_PanelSelected = false;//是否選取到	
	m_PanelBypassed = false;//是否不檢測
	m_PanelType = PANEL_TYPE_NORMAL;
	m_PanelMapRect = TRECT4D();
	m_PanelBoardRowCount = 1;//整板的單板列數
	m_PanelBoardColCount = 1;//整板的單板欄數
	m_PanelBoardColBlockCount = 1;//整板的單板欄區塊數
	m_PanelModified = false;//整板變更過
	m_PanelBarcodeEnabled = true;//整板條碼啟用
	m_PanelMultiDistrictMode = false;//整板多區段模式
	m_PanelResultID_AOI = RESULT_ID_NONE;//整板檢測結果
	m_PanelResultID_AOI_LA = RESULT_ID_NONE;;//整板檢測結果_A軌
	m_PanelResultID_AOI_LB = RESULT_ID_NONE;;//整板檢測結果_A軌
	m_PanelResultID_ARS = RESULT_ID_NONE;//整板檢測結果_ARS
	m_PanelResultID_ARS_LA = RESULT_ID_NONE;//整板檢測結果-ARS-A軌
	m_PanelResultID_ARS_LB = RESULT_ID_NONE;//整板檢測結果-ARS-B軌
	m_PanelResultID_Alarm = RESULT_ID_NONE;//整板檢測結果-警報
	m_PanelActDistrictID = DISTRICT_ID_A;//整板分段編號
	m_PanelBoardFdGrabMode = BOARD_FD_GRAB_INSPECTING;//單板定位點取像模式
	//---------------------------------------------------------------------------------//		
	m_PanelBarcode=L"Barcode";
	m_PanelBarcodeBackup=L"Barcode";
	m_PanelIsGetBarcode = false;
	//---------------------------------------------------------------------------------//		
	m_PanelBarcodeDeviceIndex = 0;
	m_PanelBarcodeDeviceCodeIndex = 0;
	m_PanelBarcodeBelongMode = BARCODE_BELONG_NONE;
	//---------------------------------------------------------------------------------//		
	m_PanelRgnCad = TREGION4D();//整板範圍-Cad
	m_PanelRgnStage_DA = TREGION4D();//整板範圍-Stage	
	m_PanelRgnStage_DB = TREGION4D();//整板範圍-Stage
	//---------------------------------------------------------------------------------//		
	m_PanelBasePlaneNormal_DA = TPOINT3D();
	m_PanelBasePlaneNormal_DB = TPOINT3D();	
	//---------------------------------------------------------------------------------//		
	m_PanelMapCTS.Identity();//整板的座標轉換-Cad to Stage
	m_PanelMapSTC.Identity();//整板的座標轉換-Stage to Cad
	m_PanelMapCTS_DB.Identity();//整板的座標轉換-Cad to Stage
	m_PanelMapSTC_DB.Identity();//整板的座標轉換-Stage to Cad
	CAOIPanel::m_PanelCalcMapFinish = false;
	//---------------------------------------------------------------------------------//
	m_PanelFdPtrList.clear();
	m_PanelMarkPtrList.clear();
	m_PanelBarcodePtrList.clear();
	m_PanelBoardPtrList.clear();
	m_PanelFieldPtrList.clear();
	m_PanelComponentPtrList.clear();
	//---------------------------------------------------------------------------------//
	m_PanelSkew = 0;
}
//-------------------------------------------------------------------------------------//
inline void CAOIPanel::ClonePanel(const CAOIPanel &panel)
{	
	//---------------------------------------------------------------------------------//
	m_PanelProjectPtr = panel.m_PanelProjectPtr;//整板所屬的專案指標
	//---------------------------------------------------------------------------------//
	m_PanelErrorString = panel.m_PanelErrorString;
	m_PanelIndex_Project = panel.m_PanelIndex_Project;//整板的引數編號
	m_PanelTempInt[0] = panel.m_PanelTempInt[0];
	m_PanelTempInt[1] = panel.m_PanelTempInt[1];
	m_PanelTempInt[2] = panel.m_PanelTempInt[2];
	m_PanelTempInt[3] = panel.m_PanelTempInt[3];
	m_PanelDeleted = panel.m_PanelDeleted;//是否刪除
	m_PanelSelected = panel.m_PanelSelected;//是否選取到	
	m_PanelBypassed = panel.m_PanelBypassed;//是否不檢測
	m_PanelModified = panel.m_PanelModified;//整板變更過
	m_PanelBarcodeEnabled = panel.m_PanelBarcodeEnabled;//整板條碼啟用	
	m_PanelMultiDistrictMode = panel.m_PanelMultiDistrictMode;//整板多區段模式	
	m_PanelType = panel.m_PanelType;
	m_PanelMapRect = panel.m_PanelMapRect;	
	m_PanelBoardRowCount = panel.m_PanelBoardRowCount;	
	m_PanelBoardColCount = panel.m_PanelBoardColCount;
	m_PanelBoardColBlockCount = panel.m_PanelBoardColBlockCount;	
	m_PanelResultID_AOI = panel.m_PanelResultID_AOI;//整板檢測結果
	m_PanelResultID_AOI_LA = panel.m_PanelResultID_AOI_LA;//整板檢測結果-A軌
	m_PanelResultID_AOI_LB = panel.m_PanelResultID_AOI_LB;//整板檢測結果-B軌
	m_PanelResultID_ARS = panel.m_PanelResultID_ARS;//整板檢測結果_ARS	
	m_PanelResultID_ARS_LA = panel.m_PanelResultID_ARS_LA;//整板檢測結果_ARS-A軌
	m_PanelResultID_ARS_LB = panel.m_PanelResultID_ARS_LB;//整板檢測結果_ARS-A軌	
	m_PanelResultID_Alarm = panel.m_PanelResultID_Alarm;//整板檢測結果-警報	
	m_PanelActDistrictID = panel.m_PanelActDistrictID;//整板分段編號
	m_PanelBoardFdGrabMode = panel.m_PanelBoardFdGrabMode;//單板定位點取像模式
	//---------------------------------------------------------------------------------//
	m_PanelBarcode = m_PanelBarcode;
	m_PanelBarcodeBackup = m_PanelBarcodeBackup;
	m_PanelIsGetBarcode = panel.m_PanelIsGetBarcode;
	m_PanelBarcodeBelongMode = panel.m_PanelBarcodeBelongMode;
	//---------------------------------------------------------------------------------//
	m_PanelBarcodeDeviceIndex = panel.m_PanelBarcodeDeviceIndex;
	m_PanelBarcodeDeviceCodeIndex = panel.m_PanelBarcodeDeviceCodeIndex;
	//---------------------------------------------------------------------------------//
	m_PanelRgnCad = panel.m_PanelRgnCad;//整板範圍-Cad
	m_PanelRgnStage_DA = panel.m_PanelRgnStage_DA;//整板範圍-Stage	
	m_PanelRgnStage_DB = panel.m_PanelRgnStage_DB;//整板範圍-Stage
	//---------------------------------------------------------------------------------//
	m_PanelBasePlaneNormal_DA = panel.m_PanelBasePlaneNormal_DA;
	m_PanelBasePlaneNormal_DB = panel.m_PanelBasePlaneNormal_DB;	
	//---------------------------------------------------------------------------------//
	m_PanelMapCTS = panel.m_PanelMapCTS;//整板的座標轉換-Cad to Stage
	m_PanelMapSTC = panel.m_PanelMapSTC;//整板的座標轉換-Stage to Cad
	m_PanelMapCTS_DB = panel.m_PanelMapCTS_DB;//整板的座標轉換-Cad to Stage
	m_PanelMapSTC_DB = panel.m_PanelMapSTC_DB;//整板的座標轉換-Stage to Cad	
	m_PanelCalcMapFinish = panel.m_PanelCalcMapFinish;
	//---------------------------------------------------------------------------------//	
	m_PanelFdPtrList = panel.m_PanelFdPtrList;
	m_PanelMarkPtrList = panel.m_PanelMarkPtrList;
	m_PanelBarcodePtrList = panel.m_PanelBarcodePtrList;	
	m_PanelBoardPtrList = panel.m_PanelBoardPtrList;
	m_PanelFieldPtrList = panel.m_PanelFieldPtrList;
	m_PanelComponentPtrList = panel.m_PanelComponentPtrList;	
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
CAOIPanel* CAOIPanel::ClonePanelObj() const
{
	CAOIPanel *ObjPtr = AOIObjManager.CreatePanelObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIPanel::GetPanelFdCount_Inline() const//取得整板的定位點數量
{
	return m_PanelFdPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIPanel::AddPanelFdPtr_Inline(CAOIFd *FdPtr)//增加整板的定位點
{
	m_PanelFdPtrList.push_back(FdPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIFd* CAOIPanel::GetPanelFdPtr_Inline(size_t index) const//取得整板的定位點指標	
{
	return m_PanelFdPtrList[index];
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIPanel::GetPanelMarkCount_Inline() const//取得整板的特徵點數量
{
	return m_PanelMarkPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIPanel::AddPanelMarkPtr_Inline(CAOIMark *MarkPtr)//增加整板的特徵點
{
	m_PanelMarkPtrList.push_back(MarkPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIMark* CAOIPanel::GetPanelMarkPtr_Inline(size_t index) const//取得整板的特徵點指標	
{
	return m_PanelMarkPtrList[index];
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIPanel::GetPanelBarcodeCount_Inline() const//取得整板的軟體條碼數量
{
	return m_PanelBarcodePtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIPanel::AddPanelBarcodePtr_Inline(CAOIBarcode *BarcodePtr)//增加整板的軟體條碼
{
	m_PanelBarcodePtrList.push_back(BarcodePtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIBarcode* CAOIPanel::GetPanelBarcodePtr_Inline(size_t index) const//取得整板的軟體條碼指標	
{
	return m_PanelBarcodePtrList[index];
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIPanel::GetPanelBoardCount_Inline() const//取得整板的單板數量
{
	return m_PanelBoardPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIPanel::AddPanelBoardPtr_Inline(CAOIBoard *BoardPtr)//增加整板的單板
{
	m_PanelBoardPtrList.push_back(BoardPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIBoard* CAOIPanel::GetPanelBoardPtr_Inline(size_t index) const//取得整板的單板指標	
{	
	return m_PanelBoardPtrList[index];
}
//-------------------------------------------------------------------------------------//
inline size_t CAOIPanel::GetPanelFieldCount_Inline() const//取得整板的區域數量
{
	return m_PanelFieldPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline void CAOIPanel::AddPanelFieldPtr_Inline(CAOIField *FieldPtr)//增加整板的區域
{
	m_PanelFieldPtrList.push_back(FieldPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIField* CAOIPanel::GetPanelFieldPtr_Inline(size_t index) const//取得整板的區域指標	
{	
	return m_PanelFieldPtrList[index];	
}
//-------------------------------------------------------------------------------------//
size_t CAOIPanel::GetPanelComponentCount_Inline() const//取得整板的零件數量
{
	return m_PanelComponentPtrList.size();
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::AddPanelComponentPtr_Inline(CAOIComponent *ComponentPtr)//增加整板的零件
{
	m_PanelComponentPtrList.push_back(ComponentPtr);
}
//-------------------------------------------------------------------------------------//
inline CAOIComponent* CAOIPanel::GetPanelComponentPtr_Inline(size_t index) const//取得整板的零件指標	
{	
	return m_PanelComponentPtrList[index];	
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelProjectPtr(CAOIProject* Ptr)
{
	 m_PanelProjectPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
CAOIProject* CAOIPanel::GetPanelProjectPtr() const
{
	return m_PanelProjectPtr;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIPanel::GetPanelErrorString() const
{
	return m_PanelErrorString;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelErrorString(LPCTSTR val)
{
	m_PanelErrorString = val;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelIndex_Project(unsigned int value)
{
	m_PanelIndex_Project = value;
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIPanel::GetPanelIndex_Project() const
{
	return m_PanelIndex_Project;
}
//-------------------------------------------------------------------------------------//
int CAOIPanel::GetPanelTempInt() const
{
	return m_PanelTempInt[0];
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelTempInt(int value)
{
	m_PanelTempInt[0] = value;
}
//-------------------------------------------------------------------------------------//
int CAOIPanel::GetPanelTempInt_01() const
{
	return m_PanelTempInt[1];
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelTempInt_01(int value)
{
	m_PanelTempInt[1] = value;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelDeleted(bool value)
{
	m_PanelDeleted = value;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::GetPanelDeleted() const
{
	return m_PanelDeleted;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelSelected(bool value)
{
	m_PanelSelected = value;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::GetPanelSelected() const
{
	return m_PanelSelected;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::GetPanelBypassed() const
{
	return m_PanelBypassed;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelBypassed(bool value)
{
	m_PanelBypassed = value;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::UpdatePanelBypassed()
{
	size_t i = 0;
	CAOIMark     *MarkPtr = NULL;
	const size_t  MarkCount = GetPanelMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetPanelMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		MarkPtr->UpdateMarkBypassed();
	}

	CAOIBarcode  *BarcodePtr = NULL;
	const size_t  BarcodeCount = GetPanelBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetPanelBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		BarcodePtr->UpdateBarcodeBypassed();
	}

	CAOIComponent *pComponent = NULL;	
	const size_t ComponentCount = GetPanelComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetPanelComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }
		pComponent->UpdateComponentBypassed();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::ExecPanelBypassed(LANE_ID LaneID)//執行整板是為不檢測
{
	size_t     i=0;
	RESULT_ID PartResultID = RESULT_ID_BYPASS;
	RESULT_ID PanelResultID = RESULT_ID_BYPASS;		
	SetPanelResultID(PanelResultID);
	SetPanelResultID_Alarm(PanelResultID);

	CAOIBoard *BoardPtr = NULL;
	const size_t BoardCount = GetPanelBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetPanelBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }		
		BoardPtr->SetBoardResultID(PartResultID);
		BoardPtr->SetBoardResultID_Alarm(PartResultID);
		BoardPtr->UpdateBoardResultID_AOI_Lane(LaneID);
	}

	CAOIFd *FdPtr = NULL;
	const size_t FdCount = GetPanelFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }		
		FdPtr->BypassSkipFd(PartResultID);		
	}

	CAOIMark    *MarkPtr = NULL;
	const size_t MarkCount = GetPanelMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetPanelMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }		
		MarkPtr->BypassSkipMark(PartResultID);	
	}

	CAOIBarcode *BarcodePtr = NULL;
	const size_t BarcodeCount = GetPanelBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetPanelBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		BarcodePtr->BypassSkipBarcode(PartResultID);		
	}

	CAOIComponent *ComponentPtr = NULL;
	const size_t  ComponentCount = GetPanelComponentCount_Inline();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetPanelComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		ComponentPtr->BypassSkipComponent(PartResultID);
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::GetPanelSkiped() const
{
	if ( RESULT_ID_SKIP == GetPanelResultID() ) { return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::CheckPanelBypassedSkipped() const
{		
	if ( GetPanelBypassed() == true ) { return true; }		
	if ( GetPanelSkiped() == true ) { return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
void  CAOIPanel::SetPanelType(PANEL_TYPE value)
{ 
	m_PanelType = value; 
}
//-------------------------------------------------------------------------------------//
PANEL_TYPE  CAOIPanel::GetPanelType() const 
{ 
	return m_PanelType; 
}
//-------------------------------------------------------------------------------------//
PANEL_SIDE_MODE CAOIPanel::CheckPanelSideMode() const
{
	size_t       i=0;
	CAOIBoard   *BoardPtr=NULL;	
	BOARD_SIDE_MODE BoardSideMode=BOARD_SIDE_NONE;
	PANEL_SIDE_MODE PanelSideMode=PANEL_SIDE_RETURN;
	const size_t BoardCount=GetPanelBoardCount();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetPanelBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }		
		if ( BOARD_SIDE_NONE == BoardSideMode )
		{	
			BoardSideMode = BoardPtr->GetBoardSideMode(); 
			continue; 
		}
		if ( BoardSideMode != BoardPtr->GetBoardSideMode() )
		{
			PanelSideMode = PANEL_SIDE_HYBRID;
			break;
		}
		BoardSideMode = BoardPtr->GetBoardSideMode(); 
	}
	if ( PANEL_SIDE_HYBRID != PanelSideMode )
	{
		switch ( BoardSideMode )
		{
		case BOARD_SIDE_TOP:	PanelSideMode=PANEL_SIDE_TOP;	break;
		case BOARD_SIDE_BOT:	PanelSideMode=PANEL_SIDE_BOTTOM;	break;
			break;
		}
	}
	return PanelSideMode;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelMapRect(const TRECT4D val)
{
	m_PanelMapRect = val;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::GetPanelMapRect(TRECT4D &val) const
{
	val = m_PanelMapRect;
}
//-------------------------------------------------------------------------------------//
int CAOIPanel::GetPanelBoardRowCount() const
{
	return m_PanelBoardRowCount;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelBoardRowCount(int value)
{
	m_PanelBoardRowCount = value;
}
//-------------------------------------------------------------------------------------//
int CAOIPanel::GetPanelBoardColCount() const
{
	return m_PanelBoardColCount;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelBoardColCount(int value)
{
	m_PanelBoardColCount = value;
}
//-------------------------------------------------------------------------------------//
int CAOIPanel::GetPanelBoardColBlockCount() const
{
	return m_PanelBoardColBlockCount;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelBoardColBlockCount(int value)
{
	m_PanelBoardColBlockCount = value;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelBarcodeDeviceIndex(unsigned int value)
{
	m_PanelBarcodeDeviceIndex = value; 
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIPanel::GetPanelBarcodeDeviceIndex() const
{ 
	return m_PanelBarcodeDeviceIndex; 
}
//-------------------------------------------------------------------------------------//	
void CAOIPanel::SetPanelBarcodeDeviceCodeIndex(unsigned int value)
{ 
	m_PanelBarcodeDeviceCodeIndex = value; 
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIPanel::GetPanelBarcodeDeviceCodeIndex() const
{ 
	return m_PanelBarcodeDeviceCodeIndex; 
}
//-------------------------------------------------------------------------------------//
BARCODE_BELONG_MODE CAOIPanel::GetPanelBarcodeBelongMode() const//整板條碼屬於模式
{
	return m_PanelBarcodeBelongMode;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelBarcodeBelongMode(BARCODE_BELONG_MODE value)//整板條碼屬於模式	
{
	m_PanelBarcodeBelongMode = value;
}
//-------------------------------------------------------------------------------------//		
void CAOIPanel::SetPanelModified(bool value) 
{ 
	m_PanelModified = value; 
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::GetPanelModified() const 
{ 
	return m_PanelModified; 
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::GetPanelBarcodeEnabled() const//整板條碼啟用
{
	return m_PanelBarcodeEnabled;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelBarcodeEnabled(bool value)//整板條碼啟用
{
	m_PanelBarcodeEnabled = value;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::GetPanelMultiDistrictMode() const//整板多區段模式
{
	return m_PanelMultiDistrictMode;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelMultiDistrictMode(bool value)//整板多區段模式	
{
	m_PanelMultiDistrictMode = value;
}
//-------------------------------------------------------------------------------------//	
void CAOIPanel::SetPanelResultID(RESULT_ID value)
{ 
	m_PanelResultID_AOI = value; 
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIPanel::GetPanelResultID() const 
{ 
	return m_PanelResultID_AOI; 
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIPanel::GetPanelResultID_AOI_LA() const //整板檢測結果-A軌
{ 
	return m_PanelResultID_AOI_LA; 
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelResultID_AOI_LA(RESULT_ID value)//整板檢測結果-A軌
{ 
	m_PanelResultID_AOI_LA = value; 
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIPanel::GetPanelResultID_AOI_LB() const //整板檢測結果-A軌
{ 
	return m_PanelResultID_AOI_LB; 
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelResultID_AOI_LB(RESULT_ID value)//整板檢測結果-B軌
{ 
	m_PanelResultID_AOI_LB = value; 
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::UpdatePanelResultID_AOI_Lane(LANE_ID LaneID)//更新整板檢測結果-軌道
{
	RESULT_ID ResultID = GetPanelResultID();
	switch ( LaneID )
	{
	case LANE_ID_A:	SetPanelResultID_AOI_LA(ResultID); break;
	case LANE_ID_B:	SetPanelResultID_AOI_LB(ResultID); break;	
	}
	return;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIPanel::GetPanelResultID_AOI_Lane(LANE_ID LaneID) const//整板檢測結果-軌道	
{
	RESULT_ID ResultID;
	switch ( LaneID )
	{
	case LANE_ID_A:	ResultID = GetPanelResultID_AOI_LA(); break;
	case LANE_ID_B:	ResultID = GetPanelResultID_AOI_LB(); break;
	default:		ResultID = GetPanelResultID(); break;		
	}
	return ResultID;
}
//-------------------------------------------------------------------------------------//
void  CAOIPanel::SetPanelResultID_ARS(RESULT_ID value) 
{ 
	m_PanelResultID_ARS = value; 
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIPanel::GetPanelResultID_ARS() const 
{ 
	return m_PanelResultID_ARS; 
}
//-------------------------------------------------------------------------------------//	
RESULT_ID CAOIPanel::GetPanelResultID_ARS_LA() const//整板檢測結果-ARS-A軌
{
	return m_PanelResultID_ARS_LA; 
}
//-------------------------------------------------------------------------------------//	
void CAOIPanel::SetPanelResultID_ARS_LA(RESULT_ID value)//整板檢測結果-ARS-A軌
{
	m_PanelResultID_ARS_LA = value;
}
//-------------------------------------------------------------------------------------//	
RESULT_ID CAOIPanel::GetPanelResultID_ARS_LB() const//整板檢測結果-ARS-B軌
{
	return m_PanelResultID_ARS_LB;
}
//-------------------------------------------------------------------------------------//	
void CAOIPanel::SetPanelResultID_ARS_LB(RESULT_ID value)//整板檢測結果-ARS-B軌
{
	m_PanelResultID_ARS_LB = value;
}
//-------------------------------------------------------------------------------------//		
void CAOIPanel::UpdatePanelResultID_ARS_Lane(LANE_ID LaneID)//更新整板檢測結果-ARS-軌道
{
	RESULT_ID ResultID = GetPanelResultID_ARS();
	switch ( LaneID )
	{
	case LANE_ID_A:	SetPanelResultID_ARS_LA(ResultID); break;
	case LANE_ID_B:	SetPanelResultID_ARS_LB(ResultID); break;	
	}
	return;
}
//-------------------------------------------------------------------------------------//	
RESULT_ID  CAOIPanel::GetPanelResultID_ARS_Lane(LANE_ID LaneID) const//整板檢測結果-ARS-軌道	
{
	RESULT_ID ResultID;
	switch ( LaneID )
	{
	case LANE_ID_A:	ResultID = GetPanelResultID_ARS_LA(); break;
	case LANE_ID_B:	ResultID = GetPanelResultID_ARS_LB(); break;
	default:		ResultID = GetPanelResultID_ARS(); break;		
	}
	return ResultID;
}
//-------------------------------------------------------------------------------------//	
void CAOIPanel::SetPanelResultID_Alarm(RESULT_ID value) 
{ 
	m_PanelResultID_Alarm = value; 
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIPanel::GetPanelResultID_Alarm() const 
{ 
	return m_PanelResultID_Alarm; 
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelBoardFdGrabMode(BOARD_FD_GRAB_MODE value)
{ 
	m_PanelBoardFdGrabMode = value; 
}
//-------------------------------------------------------------------------------------//
BOARD_FD_GRAB_MODE CAOIPanel::GetPanelBoardFdGrabMode() const
{ 
	return m_PanelBoardFdGrabMode; 
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::RemovePanelAllObjects()//移除整板所有物件
{
	RemovePanelAllFds();
	RemovePanelAllMarks();
	RemovePanelAllBarcodes();
	RemovePanelAllBoards();
	RemovePanelAllComponents();
	//RemovePanelAllFields();
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SelectPanelAllObjects(bool value)//選取專案所有物件
{
	SelectPanelAllFds(value);
	SelectPanelAllMarks(value);
	SelectPanelAllBarcodes(value);
	SelectPanelAllBoards(value);
	SelectPanelAllComponents(value);
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::RemovePanelObjectSelected()//移除整板內選取到的物件
{
	RemovePanelFdSelected();
	RemovePanelMarkSelected();
	RemovePanelBarcodeSelected();
	RemovePanelBoardSelected();
	RemovePanelComponentSelected();
	LayoutPanelRegion();
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::SetPanelAllObjectTempInt(int val, int idx)//設定整板內物件暫存參數
{
	//Fiducial
	const size_t FdCount=GetPanelFdCount();
	for ( size_t i=0; i<FdCount; i++ )
	{
		CAOIFd *FdPtr = GetPanelFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		FdPtr->SetFdTempInt(val, idx);
	}
	
	//Mark
	const size_t MarkCount=GetPanelMarkCount();
	for ( size_t i=0; i<MarkCount; i++ )
	{	
		CAOIMark *MarkPtr = GetPanelMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		MarkPtr->SetMarkTempInt(val, idx);
	}
	
	//Barcode
	const size_t BarcodeCount=GetPanelBarcodeCount();
	for ( size_t i=0; i<BarcodeCount; i++ )
	{				
		CAOIBarcode *BarcodePtr = GetPanelBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		BarcodePtr->SetBarcodeTempInt(val, idx);
	}

	//Component
	const size_t ComponentCount=GetPanelComponentCount();
	for ( size_t i=0; i<ComponentCount; i++ )
	{	
		CAOIComponent *ComponentPtr = GetPanelComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentTempInt(val, idx);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::SetPanelAllObjectTempInt(DISTRICT_ID DistrictID, int val, int idx)//設定整板內物件暫存參數
{
	//Fiducial
	const size_t FdCount=GetPanelFdCount();
	for ( size_t i=0; i<FdCount; i++ )
	{
		CAOIFd *FdPtr = GetPanelFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }		
		FdPtr->SetFdTempInt(val, idx);
	}
	
	//Mark
	const size_t MarkCount=GetPanelMarkCount();
	for ( size_t i=0; i<MarkCount; i++ )
	{	
		CAOIMark *MarkPtr = GetPanelMarkPtr(i, false);
		if ( NULL == MarkPtr ) { continue; }
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		MarkPtr->SetMarkTempInt(val, idx);
	}
	
	//Barcode
	const size_t BarcodeCount=GetPanelBarcodeCount();
	for ( size_t i=0; i<BarcodeCount; i++ )
	{				
		CAOIBarcode *BarcodePtr = GetPanelBarcodePtr(i, false);
		if ( NULL == BarcodePtr ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		BarcodePtr->SetBarcodeTempInt(val, idx);
	}

	//Component
	const size_t ComponentCount=GetPanelComponentCount();
	for ( size_t i=0; i<ComponentCount; i++ )
	{	
		CAOIComponent *ComponentPtr = GetPanelComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		ComponentPtr->SetComponentTempInt(val, idx);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIPanel::GetPanelFdCount() const//取得整板的定位點數量
{
	return GetPanelFdCount_Inline();
}
//-------------------------------------------------------------------------------------//
size_t CAOIPanel::GetPanelFdCount(DISTRICT_ID DistrictID) const//取得整板的定位點數量
{
	size_t       i=0;
	size_t       Count=0;
	CAOIFd      *FdPtr = NULL;
	const size_t FdCount = GetPanelFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		Count ++;
	}
	return Count;
}
//-------------------------------------------------------------------------------------//
CAOIFd* CAOIPanel::GetPanelFdPtr(size_t index, bool check) const//取得整板的定位點指標
{
	if ( check )
	{
		const size_t Count = GetPanelFdCount_Inline();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return GetPanelFdPtr_Inline(index);	
}
//-------------------------------------------------------------------------------------//
CAOIFd* CAOIPanel::GetPanelFdPtr(size_t index, DISTRICT_ID DistrictID) const//取得整板的定位點指標
{
	size_t       i=0;
	size_t       Count=0;
	CAOIFd      *FdPtr = NULL;
	const size_t FdCount = GetPanelFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		if ( Count == index ) 
		{	return FdPtr; }
		Count ++;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::AddPanelFdPtr(CAOIFd *FdPtr)//增加整板的定位點
{
	const int FdIndex = (int)(GetPanelFdCount_Inline());	
	FdPtr->SetFdPanelPtr(this);
	FdPtr->SetFdIndex_Panel(FdIndex);
	FdPtr->SetFdPanelIndex_Project(m_PanelIndex_Project);	
	AddPanelFdPtr_Inline(FdPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::SelectPanelAllFds(bool Select)//選取整板的定位點
{
	size_t i = 0;
	CAOIFd*  FdPtr = NULL;
	const size_t FdCount = GetPanelFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		FdPtr->SetFdSelected(Select);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::RemovePanelFdSelected()//移除選取到的整板的定位點
{
	size_t i = 0;
	int    index = 0;
	CAOIFd*  FdPtr = NULL;
	std::vector<CAOIFd*> PanelFdPtrList = m_PanelFdPtrList;
	const size_t FdCount = PanelFdPtrList.size();

	index = 0;
	m_PanelFdPtrList.clear();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = PanelFdPtrList[i];
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdSelected() == TRUE ) { continue; }
		
		FdPtr->SetFdIndex_Panel(index);
		AddPanelFdPtr_Inline(FdPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::RemovePanelAllFds()//移除整板的定位點
{
	m_PanelFdPtrList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::LayoutPanelFdList()//重整整板的定位點列表
{
	size_t i = 0;
	int    index = 0;
	CAOIFd*  FdPtr = NULL;
	std::vector<CAOIFd*> PanelFdPtrList = m_PanelFdPtrList;
	const size_t FdCount = PanelFdPtrList.size();

	index = 0;
	m_PanelFdPtrList.clear();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = PanelFdPtrList[i];
		if ( NULL == FdPtr ) { continue; }
		
		FdPtr->SetFdIndex_Panel(index);
		AddPanelFdPtr_Inline(FdPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::CheckPanelFdCalculated(DISTRICT_ID DistrictID)//確認整板的定位點都計算過
{
	size_t     i = 0;
	RESULT_ID  ResultID;
	CAOIFd    *FdPtr = NULL;	
	const size_t FdCount = GetPanelFdCount_Inline();	
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }		
		if ( NULL != FdPtr->GetFdBoardPtr() ) { continue; }
		if ( FdPtr->GetFdDistrictID() != DistrictID ) { continue; }
		ResultID = FdPtr->GetFdResultID_AOI();
		if ( RESULT_ID_NONE == ResultID ) { return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::ClearPanelFdImageBuffer(DISTRICT_ID DistrictID)//清除整板定位點影像資料
{	
	CAOIFd    *FdPtr = NULL;	
	const size_t FdCount = GetPanelFdCount_Inline();	
	for ( size_t i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }		
		if ( NULL != FdPtr->GetFdBoardPtr() ) { continue; }
		if ( FdPtr->GetFdDistrictID() != DistrictID ) { continue; }
		FdPtr->SetFdKeepImage(false);
		FdPtr->ClearRgnImageBuffer();
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIPanel::CalcPanelFdCount(DISTRICT_ID DistrictID)//計算只屬於整板定位點的數量
{
	size_t  i = 0;
	size_t  count=0;
	CAOIFd *FdPtr = NULL;	
	const size_t FdCount = GetPanelFdCount_Inline();

	count = 0;	
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }		
		if ( NULL != FdPtr->GetFdBoardPtr() ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }

		count ++;
	}
	return count;
}
//-------------------------------------------------------------------------------------//
size_t CAOIPanel::CalcPanelFdDefectCount(DISTRICT_ID DistrictID)//計算只屬於整板定位點瑕疵的數量
{
	size_t  i = 0;
	size_t  count=0;
	RESULT_ID ResultID;
	CAOIFd *FdPtr = NULL;		
	const size_t FdCount = GetPanelFdCount_Inline();

	count = 0;	
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }		
		if ( NULL != FdPtr->GetFdBoardPtr() ) { continue; }		
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }

		ResultID = FdPtr->GetFdResultID_AOI();
		if ( RESULT_ID_OK==ResultID || RESULT_ID_BYPASS == ResultID || RESULT_ID_SKIP==ResultID )
		{ continue;		}
		count ++;
	}
	return count;
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIPanel::CalcPanelFdInex(const CAOIFd *RefFdPtr)//確認整板的定位點引數-會忽略掛在單板下的定位點
{
	size_t       i = 0;
	unsigned int index=0;
	CAOIFd      *FdPtr = NULL;	
	const size_t FdCount = GetPanelFdCount_Inline();

	index = 0;	
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }		
		if ( NULL != FdPtr->GetFdBoardPtr() ) { continue; }				
		if ( RefFdPtr == FdPtr )
		{	break; }
		index ++;
	}
	return index;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::ExecPanelBeXBoard(LANE_ID LaneID)//執行整板是為報廢板
{
	size_t     i=0;
	RESULT_ID PartResultID = RESULT_ID_SKIP;
	RESULT_ID PanelResultID = RESULT_ID_SKIP;		
	SetPanelResultID(PanelResultID);
	SetPanelResultID_Alarm(PanelResultID);

	CAOIBoard *BoardPtr = NULL;
	const size_t BoardCount = GetPanelBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetPanelBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		if ( RESULT_ID_BYPASS == BoardPtr->GetBoardResultID() )
		{	continue; }
		BoardPtr->SetBoardResultID(PartResultID);
		BoardPtr->SetBoardResultID_Alarm(PartResultID);
		BoardPtr->UpdateBoardResultID_AOI_Lane(LaneID);
	}

	CAOIFd *FdPtr = NULL;
	const size_t FdCount = GetPanelFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( RESULT_ID_BYPASS == FdPtr->GetFdResultID_AOI() )
		{	continue; }
		FdPtr->BypassSkipFd(PartResultID);		
	}

	CAOIMark    *MarkPtr = NULL;
	const size_t MarkCount = GetPanelMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetPanelMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( RESULT_ID_BYPASS == MarkPtr->GetMarkResultID_AOI() )
		{	continue; }	
		MarkPtr->BypassSkipMark(PartResultID);	
	}

	CAOIBarcode *BarcodePtr = NULL;
	const size_t BarcodeCount = GetPanelBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetPanelBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( RESULT_ID_BYPASS == BarcodePtr->GetBarcodeResultID_AOI() )
		{	continue; }	
		BarcodePtr->BypassSkipBarcode(PartResultID);		
	}

	CAOIComponent *ComponentPtr = NULL;
	const size_t  ComponentCount = GetPanelComponentCount_Inline();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetPanelComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( RESULT_ID_BYPASS == ComponentPtr->GetComponentResultID_AOI() )
		{	continue; }
		ComponentPtr->BypassSkipComponent(PartResultID);		
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
size_t CAOIPanel::GetPanelMarkCount() const//取得整板的特徵點數量
{
	return GetPanelMarkCount_Inline();
}
//-------------------------------------------------------------------------------------//
CAOIMark* CAOIPanel::GetPanelMarkPtr(size_t index, bool check) const//取得整板的特徵點指標
{
	if ( check )
	{
		const size_t Count = GetPanelMarkCount_Inline();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return GetPanelMarkPtr_Inline(index);	
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::AddPanelMarkPtr(CAOIMark *MarkPtr)//增加整板的特徵點
{
	const int MarkIndex = (int)(GetPanelMarkCount_Inline());
	MarkPtr->SetMarkPanelIndex_Project(m_PanelIndex_Project);
	MarkPtr->SetMarkPanelPtr(this);
	MarkPtr->SetMarkIndex_Panel(MarkIndex);	
	AddPanelMarkPtr_Inline(MarkPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIMark* CAOIPanel::GetPanelMarkPtrBySelected()//取得整板的選取的特徵點
{
	size_t i = 0;
	CAOIMark*  MarkPtr = NULL;
	const size_t MarkCount = GetPanelMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetPanelMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkSelected() == false ) { continue; }
		return MarkPtr;
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::SelectPanelAllMarks(bool Select)//選取整板的特徵點
{
	size_t i = 0;
	CAOIMark*  MarkPtr = NULL;
	const size_t MarkCount = GetPanelMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetPanelMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		MarkPtr->SetMarkSelected(Select);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::RemovePanelMarkSelected()//移除選取到的整板的特徵點
{
	size_t i = 0;
	int    index = 0;
	CAOIMark *MarkPtr = NULL;
	std::vector<CAOIMark*> PanelMarkPtrList = m_PanelMarkPtrList;
	const size_t MarkCount = PanelMarkPtrList.size();

	index = 0;
	m_PanelMarkPtrList.clear();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = PanelMarkPtrList[i];
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkSelected() == true ) { continue; }
		
		MarkPtr->SetMarkIndex_Panel(index);
		AddPanelMarkPtr_Inline(MarkPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::RemovePanelAllMarks()//移除整板的特徵點
{
	m_PanelMarkPtrList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::LayoutPanelMarkList()//重整整板的特徵點列表	
{
	size_t i = 0;
	int    index = 0;
	CAOIMark *MarkPtr = NULL;
	std::vector<CAOIMark*> PanelMarkPtrList = m_PanelMarkPtrList;
	const size_t MarkCount = PanelMarkPtrList.size();

	index = 0;
	m_PanelMarkPtrList.clear();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = PanelMarkPtrList[i];
		if ( NULL == MarkPtr ) { continue; }

		MarkPtr->SetMarkIndex_Panel(index);
		AddPanelMarkPtr_Inline(MarkPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIPanel::CalcPanelMarkCount()//計算只屬於整板特徵點的數量
{
	size_t i = 0;
	size_t count = 0;
	CAOIMark *MarkPtr = NULL;	
	const size_t MarkCount = GetPanelMarkCount_Inline();

	count = 0;	
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetPanelMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( NULL != MarkPtr->GetMarkBoardPtr() ) { continue; }						
		count ++;
	}
	return count;
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIPanel::CalcPanelMarkInex(const CAOIMark *RefMarkPtr)//確認整板的特徵點引數-會忽略掛在單板下的特徵點
{
	size_t        i = 0;
	unsigned int  index = 0;
	CAOIMark  *MarkPtr = NULL;	
	const size_t  MarkCount = GetPanelMarkCount_Inline();

	index = 0;	
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetPanelMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( NULL != MarkPtr->GetMarkBoardPtr() ) { continue; }				
		if ( RefMarkPtr == MarkPtr )
		{	break; }
		index ++;
	}
	return index;
}
//-------------------------------------------------------------------------------------//
size_t CAOIPanel::GetPanelBarcodeCount() const//取得整板的軟體條碼數量
{
	return GetPanelBarcodeCount_Inline();
}
//-------------------------------------------------------------------------------------//
CAOIBarcode* CAOIPanel::GetPanelBarcodePtr(size_t index, bool check) const//取得整板的軟體條碼指標
{
	if ( check )
	{
		const size_t Count = GetPanelBarcodeCount_Inline();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return GetPanelBarcodePtr_Inline(index);	
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::AddPanelBarcodePtr(CAOIBarcode *BarcodePtr)//增加整板的軟體條碼
{
	const int SBIndex = (int)(GetPanelBarcodeCount_Inline());
	BarcodePtr->SetBarcodePanelIndex_Project(m_PanelIndex_Project);
	BarcodePtr->SetBarcodePanelPtr(this);
	BarcodePtr->SetBarcodeIndex_Panel(SBIndex);	
	AddPanelBarcodePtr_Inline(BarcodePtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::SelectPanelAllBarcodes(bool Select)//選取整板的軟體條碼
{
	size_t i = 0;
	CAOIBarcode*  BarcodePtr = NULL;
	const size_t FdCount = GetPanelBarcodeCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		BarcodePtr = GetPanelBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		BarcodePtr->SetBarcodeSelected(Select);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::RemovePanelBarcodeSelected()//移除選取到的整板的軟體條碼
{
	size_t i = 0;
	int    index = 0;
	CAOIBarcode *BarcodePtr = NULL;
	std::vector<CAOIBarcode*> PanelSBPtrList = m_PanelBarcodePtrList;
	const size_t BarcodeCount = PanelSBPtrList.size();

	index = 0;
	m_PanelBarcodePtrList.clear();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = PanelSBPtrList[i];
		if ( NULL == BarcodePtr ) { continue; }
		if ( BarcodePtr->GetBarcodeSelected() == true ) { continue; }
		
		BarcodePtr->SetBarcodeIndex_Panel(index);
		AddPanelBarcodePtr_Inline(BarcodePtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::RemovePanelAllBarcodes()//移除整板的軟體條碼
{
	m_PanelBarcodePtrList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::LayoutPanelBarcodeList()//重整整板的軟體條碼列表	
{
	size_t i = 0;
	int    index = 0;
	CAOIBarcode *BarcodePtr = NULL;
	std::vector<CAOIBarcode*> PanelSBPtrList = m_PanelBarcodePtrList;
	const size_t BarcodeCount = PanelSBPtrList.size();

	index = 0;
	m_PanelBarcodePtrList.clear();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = PanelSBPtrList[i];
		if ( NULL == BarcodePtr ) { continue; }
		
		BarcodePtr->SetBarcodeIndex_Panel(index);
		CAOIPanel::AddPanelBarcodePtr_Inline(BarcodePtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIPanel::CalcPanelBarcodeCount()//計算只屬於整板軟體條碼的數量
{
	size_t i = 0;
	size_t count = 0;
	CAOIBarcode *BarcodePtr = NULL;	
	const size_t BarcodeCount = GetPanelBarcodeCount_Inline();

	count = 0;	
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetPanelBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( NULL != BarcodePtr->GetBarcodeBoardPtr() ) { continue; }						
		count ++;
	}
	return count;
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIPanel::CalcPanelBarcodeInex(const CAOIBarcode *RefBarcodePtr)//確認整板的軟體條碼引數-會忽略掛在單板下的條碼
{
	size_t        i = 0;
	unsigned int  index = 0;
	CAOIBarcode  *BarcodePtr = NULL;	
	const size_t  BarcodeCount = GetPanelBarcodeCount_Inline();

	index = 0;	
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetPanelBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( NULL != BarcodePtr->GetBarcodeBoardPtr() ) { continue; }				
		if ( RefBarcodePtr == BarcodePtr )
		{	break; }
		index ++;
	}
	return index;
}
//-------------------------------------------------------------------------------------//
size_t CAOIPanel::GetPanelBoardCount() const
{
	return GetPanelBoardCount_Inline();
}
//-------------------------------------------------------------------------------------//
CAOIBoard* CAOIPanel::GetPanelBoardPtr(size_t index, bool check) const
{
	if ( check )
	{
		const size_t Count = GetPanelBoardCount_Inline();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return GetPanelBoardPtr_Inline(index);	
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::AddPanelBoardPtr(CAOIBoard *BoardPtr)
{
	const int BoardIndex = (int)(GetPanelBoardCount_Inline());	
	BoardPtr->SetBoardPanelPtr(this);
	BoardPtr->SetBoardIndex_Panel(BoardIndex);
	BoardPtr->SetBoardPanelIndex_Project(m_PanelIndex_Project);	
	BoardPtr->SetBoardActDistrictID(m_PanelActDistrictID);
	BoardPtr->SetBoardMapCTS(DISTRICT_ID_A, GetPanelMapCTSPtr(DISTRICT_ID_A));
	BoardPtr->SetBoardMapSTC(DISTRICT_ID_A, GetPanelMapSTCPtr(DISTRICT_ID_A));
	BoardPtr->SetBoardMapCTS(DISTRICT_ID_B, GetPanelMapCTSPtr(DISTRICT_ID_B));	
	BoardPtr->SetBoardMapSTC(DISTRICT_ID_B, GetPanelMapSTCPtr(DISTRICT_ID_B));
	AddPanelBoardPtr_Inline(BoardPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::SelectPanelAllBoards(bool Select)//選取整板的單板
{
	size_t i = 0;
	CAOIBoard*  BoardPtr = NULL;
	const size_t BoardCount = GetPanelBoardCount_Inline();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetPanelBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }
		BoardPtr->SetBoardSelected(Select);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::RemovePanelBoard(CAOIBoard *RefBoardPtr)//移除整板的單板
{
	if ( NULL == RefBoardPtr ) { return false; }	
	
	//Remove Fds
	SelectPanelAllFds(false);
	RefBoardPtr->SelectBoardAllFds(true);
	RemovePanelFdSelected();

	//Remove Mark
	SelectPanelAllMarks(false);	
	RefBoardPtr->SelectBoardAllMarks(true);
	RemovePanelMarkSelected();

	//Remove Barcode
	SelectPanelAllBarcodes(false);	
	RefBoardPtr->SelectBoardAllBarcodes(true);
	RemovePanelBarcodeSelected();

	//Remove Component	
	SelectPanelAllComponents(false);
	RefBoardPtr->SelectBoardAllComponents(true);
	RemovePanelComponentSelected();

	//Remove Board
	//RemovePanelBoard(RefBoardPtr);
	SelectPanelAllBoards(false);
	RefBoardPtr->SetBoardSelected(true);
	RemovePanelBoardSelected();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::RemovePanelBoardSelected()
{
	size_t i = 0;
	int    index = 0;
	CAOIBoard*  BoardPtr = NULL;
	std::vector<CAOIBoard*> PanelBoardPtrList = m_PanelBoardPtrList;
	const size_t BoardCount = PanelBoardPtrList.size();

	index = 0;
	m_PanelBoardPtrList.clear();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = PanelBoardPtrList[i];
		if ( NULL == BoardPtr ) { continue; }
		if ( BoardPtr->GetBoardSelected() == true ) { continue; }
		
		BoardPtr->SetBoardIndex_Panel(index);
		AddPanelBoardPtr_Inline(BoardPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::RemovePanelAllBoards()//移除整板的單板
{
	m_PanelBoardPtrList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::LayoutPanelBoardList()//重整整板的單板列表
{
	size_t i = 0;
	int    index = 0;
	CAOIBoard*  BoardPtr = NULL;
	std::vector<CAOIBoard*> PanelBoardPtrList = m_PanelBoardPtrList;
	const size_t BoardCount = PanelBoardPtrList.size();

	index = 0;
	m_PanelBoardPtrList.clear();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = PanelBoardPtrList[i];
		if ( NULL == BoardPtr ) { continue; }		
		
		BoardPtr->SetBoardIndex_Panel(index);
		AddPanelBoardPtr_Inline(BoardPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::LayoutPanelBoardListRegion()//重整整板的單板區域列表
{
	size_t       i = 0;	
	CAOIBoard*   BoardPtr = NULL;	
	const size_t BoardCount = GetPanelBoardCount();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetPanelBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }				
		BoardPtr->LayoutBoardRegion();
	}
	LayoutPanelRegion();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::LayoutPanelBoardListRegion(DISTRICT_ID DistrictID)//重整整板的單板區域列表	
{
	size_t       i = 0;	
	CAOIBoard*   BoardPtr = NULL;	
	const size_t BoardCount = GetPanelBoardCount();
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetPanelBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }				
		BoardPtr->LayoutBoardRegion(DistrictID);
	}
	LayoutPanelRegion(DistrictID);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::AnalyzePanelBoardOrientationMode()//分析整板內單板的方向性
{
	CAOIBoard *BoardPtr=NULL;
	CAOIBoard *RefBoardPtr=NULL;
	const size_t BoardCount=GetPanelBoardCount();
	const size_t PanelComponentCount=GetPanelComponentCount();	
	if ( 0==BoardCount || 1==BoardCount ) { return true; }	
	const size_t AveBoardComponentCount=PanelComponentCount/BoardCount;//找出平均零件數	
	const size_t AveBoardComponentCount_070=(size_t)(AveBoardComponentCount*0.7);
	//找零件數最少的
	size_t MinBoardComponentCount=0;
	for ( size_t i=0; i<BoardCount; i++ )
	{
		BoardPtr=GetPanelBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		size_t BoardComCount=BoardPtr->GetBoardComponentCount();
		if ( NULL==RefBoardPtr || MinBoardComponentCount>BoardComCount )
		{
			if ( BoardComCount > AveBoardComponentCount_070 )
			{
				RefBoardPtr = BoardPtr;	
				MinBoardComponentCount=BoardComCount;
			}
		}		
	}
	if ( NULL == RefBoardPtr )
	{	return false; }
	BOARD_ORIENTATION_MODE Mode;	
	TREGION4D RefBoardCadRgn;	
	RefBoardPtr->LayoutBoardRegionCad();
	RefBoardPtr->GetBoardRgnCad(RefBoardCadRgn);
	
	CAOIComponent *RefComponentPtrLB=NULL;
	CAOIComponent *RefComponentPtrRB=NULL;
	CAOIComponent *RefComponentPtrLT=NULL;
	CAOIComponent *RefComponentPtrRT=NULL;	
	double MaxDistLB=0, MaxDistRB=0, MaxDistLT=0, MaxDistRT=0;
	BOARD_ORIENTATION_MODE RefOrientation=RefBoardPtr->GetBoardOrientationMode();
	const size_t RefBoardComponentCount=RefBoardPtr->GetBoardComponentCount();		
	const double RefBoardCadRgnCpX=RefBoardCadRgn.GetCpX();
	const double RefBoardCadRgnCpY=RefBoardCadRgn.GetCpY();
	for ( size_t i=0; i<RefBoardComponentCount; i++ )
	{
		CAOIComponent *RefComponentPtr=RefBoardPtr->GetBoardComponentPtr(i, false);
		if ( NULL == RefComponentPtr ) { continue; }
		const TPOINT2D &ComponentPos=RefComponentPtr->GetComponentCadPos();
		const double Dist=JetAPI::CalcDistance(ComponentPos.x-RefBoardCadRgnCpX, ComponentPos.y-RefBoardCadRgnCpY);
		if ( ComponentPos.x < RefBoardCadRgnCpX )//Left
		{
			if ( ComponentPos.y < RefBoardCadRgnCpY )//Bottom
			{
				if ( NULL==RefComponentPtrLB || Dist>MaxDistLB )
				{
					MaxDistLB=Dist;
					RefComponentPtrLB=RefComponentPtr;
				}
			}
			else //Top
			{
				if ( NULL==RefComponentPtrLT || Dist>MaxDistLT )
				{
					MaxDistLT=Dist;
					RefComponentPtrLT=RefComponentPtr;
				}
			}
		}
		else//Right
		{
			if ( ComponentPos.y < RefBoardCadRgnCpY )//Bottom
			{
				if ( NULL==RefComponentPtrRB || Dist>MaxDistRB )
				{
					MaxDistRB=Dist;
					RefComponentPtrRB=RefComponentPtr;
				}
			}
			else //Top
			{
				if ( NULL==RefComponentPtrRT || Dist>MaxDistRT )
				{
					MaxDistRT=Dist;
					RefComponentPtrRT=RefComponentPtr;
				}
			}
		}		
	}
	std::vector<CAOIComponent*> RefComponentList;
	std::vector<CAOIComponent*> RefComponentList_030, RefComponentList_060, RefComponentList_120, RefComponentList_150;
	std::vector<CAOIComponent*> RefComponentList_210, RefComponentList_240, RefComponentList_300, RefComponentList_330;
	if ( NULL != RefComponentPtrLB)
	{	RefComponentList.push_back(RefComponentPtrLB);	}
	if ( NULL != RefComponentPtrRB)
	{	RefComponentList.push_back(RefComponentPtrRB);	}
	if ( NULL != RefComponentPtrLT)
	{	RefComponentList.push_back(RefComponentPtrLT);	}
	if ( NULL != RefComponentPtrRT)
	{	RefComponentList.push_back(RefComponentPtrRT);	}
	const size_t RefComponentCount=RefComponentList.size();	
	for ( size_t i=0; i<RefComponentCount; i++ )
	{
		CAOIComponent *NewComponentPtr=NULL;
		CAOIComponent *RefComponentPtr=RefComponentList[i];
		if ( NULL == RefComponentPtr ) { continue; }
		//BOARD_ORIENTATION_030
		NewComponentPtr = RefComponentPtr->CloneComponentObj();
		if ( NULL != NewComponentPtr )
		{	RefComponentList_030.push_back(NewComponentPtr);	}
		//BOARD_ORIENTATION_060-Rotate090+MirrorYAxis
		NewComponentPtr = RefComponentPtr->CloneComponentObj();
		if ( NULL != NewComponentPtr )
		{
			NewComponentPtr->RotateRgnCad(90, RefBoardCadRgnCpX, RefBoardCadRgnCpY);
			NewComponentPtr->MirrorXComponent(RefBoardCadRgnCpX, NULL);
			RefComponentList_060.push_back(NewComponentPtr);
		}
		//BOARD_ORIENTATION_120-Rotate090
		NewComponentPtr = RefComponentPtr->CloneComponentObj();
		if ( NULL != NewComponentPtr )
		{
			NewComponentPtr->RotateRgnCad(90, RefBoardCadRgnCpX, RefBoardCadRgnCpY);			
			RefComponentList_120.push_back(NewComponentPtr);
		}
		//BOARD_ORIENTATION_150-MirrorYAxis
		NewComponentPtr = RefComponentPtr->CloneComponentObj();
		if ( NULL != NewComponentPtr )
		{
			NewComponentPtr->MirrorXComponent(RefBoardCadRgnCpX, NULL);
			RefComponentList_150.push_back(NewComponentPtr);
		}
		//BOARD_ORIENTATION_210-Rotate180
		NewComponentPtr = RefComponentPtr->CloneComponentObj();
		if ( NULL != NewComponentPtr )
		{
			NewComponentPtr->RotateRgnCad(180, RefBoardCadRgnCpX, RefBoardCadRgnCpY);			
			RefComponentList_210.push_back(NewComponentPtr);
		}
		//BOARD_ORIENTATION_240-Rotate270+MirrorYAxis
		NewComponentPtr = RefComponentPtr->CloneComponentObj();
		if ( NULL != NewComponentPtr )
		{
			NewComponentPtr->RotateRgnCad(270, RefBoardCadRgnCpX, RefBoardCadRgnCpY);		
			NewComponentPtr->MirrorXComponent(RefBoardCadRgnCpX, NULL);
			RefComponentList_240.push_back(NewComponentPtr);
		}
		//BOARD_ORIENTATION_300-Rotate270
		NewComponentPtr = RefComponentPtr->CloneComponentObj();
		if ( NULL != NewComponentPtr )
		{
			NewComponentPtr->RotateRgnCad(270, RefBoardCadRgnCpX, RefBoardCadRgnCpY);					
			RefComponentList_300.push_back(NewComponentPtr);
		}
		//BOARD_ORIENTATION_330-MirrorXAxis
		NewComponentPtr = RefComponentPtr->CloneComponentObj();
		if ( NULL != NewComponentPtr )
		{
			NewComponentPtr->MirrorYComponent(RefBoardCadRgnCpY, NULL);
			RefComponentList_330.push_back(NewComponentPtr);
		}
	}
	const size_t RefComponentCount_030=RefComponentList_030.size();	
	const size_t RefComponentCount_060=RefComponentList_060.size();	
	const size_t RefComponentCount_120=RefComponentList_120.size();	
	const size_t RefComponentCount_150=RefComponentList_150.size();	
	const size_t RefComponentCount_210=RefComponentList_210.size();	
	const size_t RefComponentCount_240=RefComponentList_240.size();	
	const size_t RefComponentCount_300=RefComponentList_300.size();	
	const size_t RefComponentCount_330=RefComponentList_330.size();	
	if ( RefComponentCount_030!=RefComponentCount ||
		 RefComponentCount_060!=RefComponentCount ||
		 RefComponentCount_120!=RefComponentCount ||
		 RefComponentCount_150!=RefComponentCount ||
		 RefComponentCount_210!=RefComponentCount ||
		 RefComponentCount_240!=RefComponentCount ||
		 RefComponentCount_300!=RefComponentCount ||
		 RefComponentCount_330!=RefComponentCount )
	{
		AOIObjManager.DestroyComponentList(RefComponentList_030);
		AOIObjManager.DestroyComponentList(RefComponentList_060);
		AOIObjManager.DestroyComponentList(RefComponentList_120);
		AOIObjManager.DestroyComponentList(RefComponentList_150);
		AOIObjManager.DestroyComponentList(RefComponentList_210);
		AOIObjManager.DestroyComponentList(RefComponentList_240);
		AOIObjManager.DestroyComponentList(RefComponentList_300);
		AOIObjManager.DestroyComponentList(RefComponentList_330);
		return false;
	}
	
	const bool bCmpPos=true;
	const bool bCmpAngle=false;	
	for ( size_t i=0; i<BoardCount; i++ )
	{
		BoardPtr=GetPanelBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		if ( RefBoardPtr == BoardPtr ) { continue; }
		bool bAdded=false;
		std::vector<CAOIComponent*> TmpComponentList;
		const size_t BoardComponentCount=BoardPtr->GetBoardComponentCount();
		for ( size_t j=0; j<RefComponentCount; j++ )
		{
			CAOIComponent *RefComponentPtr=RefComponentList[j];
			if ( NULL == RefComponentPtr ) { continue; }
			CString RefComName=RefComponentPtr->GetComponentName();
			for ( size_t k=0; k<BoardComponentCount; k++ )
			{
				bAdded=false;
				CAOIComponent *ComponentPtr=BoardPtr->GetBoardComponentPtr(k, false);
				if ( NULL == ComponentPtr ) { continue; }
				CString ComName=ComponentPtr->GetComponentName();
				if ( ComName.CompareNoCase(RefComName) != 0 ) { continue; }
				bAdded=true;
				TmpComponentList.push_back(ComponentPtr);
				break;
			}			
		}
		const size_t TmpComponentCount=TmpComponentList.size();
		if ( TmpComponentCount != RefComponentCount )
		{	continue; }
		BOARD_ORIENTATION_MODE BoardOrientationMode=BoardPtr->GetBoardOrientationMode();
		if ( CompareComponentList(bCmpPos, bCmpAngle, TmpComponentList, RefComponentList_030) == true )
		{	BoardOrientationMode = BOARD_ORIENTATION_030;	}
		else if ( CompareComponentList(bCmpPos, bCmpAngle, TmpComponentList, RefComponentList_060) == true )
		{	BoardOrientationMode = BOARD_ORIENTATION_060;	}
		else if ( CompareComponentList(bCmpPos, bCmpAngle, TmpComponentList, RefComponentList_120) == true )
		{	BoardOrientationMode = BOARD_ORIENTATION_120;	}
		else if ( CompareComponentList(bCmpPos, bCmpAngle, TmpComponentList, RefComponentList_150) == true )
		{	BoardOrientationMode = BOARD_ORIENTATION_150;	}
		else if ( CompareComponentList(bCmpPos, bCmpAngle, TmpComponentList, RefComponentList_210) == true )
		{	BoardOrientationMode = BOARD_ORIENTATION_210;	}
		else if ( CompareComponentList(bCmpPos, bCmpAngle, TmpComponentList, RefComponentList_240) == true )
		{	BoardOrientationMode = BOARD_ORIENTATION_240;	}
		else if ( CompareComponentList(bCmpPos, bCmpAngle, TmpComponentList, RefComponentList_300) == true )
		{	BoardOrientationMode = BOARD_ORIENTATION_300;	}
		else if ( CompareComponentList(bCmpPos, bCmpAngle, TmpComponentList, RefComponentList_330) == true )
		{	BoardOrientationMode = BOARD_ORIENTATION_330;	}
		BoardPtr->SetBoardOrientationMode(BoardOrientationMode);
	}
	AOIObjManager.DestroyComponentList(RefComponentList_030);
	AOIObjManager.DestroyComponentList(RefComponentList_060);
	AOIObjManager.DestroyComponentList(RefComponentList_120);
	AOIObjManager.DestroyComponentList(RefComponentList_150);
	AOIObjManager.DestroyComponentList(RefComponentList_210);
	AOIObjManager.DestroyComponentList(RefComponentList_240);
	AOIObjManager.DestroyComponentList(RefComponentList_300);
	AOIObjManager.DestroyComponentList(RefComponentList_330);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::CompareComponentList(bool CmpPos, bool CmpAngle, const std::vector<CAOIComponent*> &List1, const std::vector<CAOIComponent*> &List2) const
{
	const size_t Count1=List1.size();
	const size_t Count2=List2.size();
	if ( Count1 != Count2 )
	{	return false; }
	if ( 0==Count1 || 0==Count2 )
	{	return true;	}
	const size_t Count=Count1;
	if ( true == CmpAngle )
	{
		for ( size_t i=0; i<Count; i++ )
		{
			CAOIComponent *Ptr1=List1[i];
			CAOIComponent *Ptr2=List2[i];
			if ( NULL == Ptr1 ) { continue; }
			if ( NULL == Ptr2 ) { continue; }
			const double Angle1=Ptr1->GetComponentAngle();
			const double Angle2=Ptr2->GetComponentAngle();
			if ( fabs(Angle1-Angle2) > 1.0 )
			{	return false;	}
		}	
	}	
	if ( true == CmpPos )
	{
		TREGION4D Rgn1;
		TREGION4D Rgn2;
		Rgn1.minX=DBL_MAX;
		Rgn2.minX=DBL_MAX;
		for ( size_t i=0; i<Count; i++ )
		{
			CAOIComponent *Ptr1=List1[i];
			CAOIComponent *Ptr2=List2[i];			
			if ( NULL != Ptr1 ) 
			{ 
				TREGION4D &Rgn=Rgn1;
				const TPOINT2D Pos=Ptr1->GetComponentCadPos();
				if ( DBL_MAX==Rgn.minX )
				{
					Rgn.minX=Rgn.maxX=Pos.x;
					Rgn.minY=Rgn.maxY=Pos.y;
				}
				else
				{
					if ( Rgn.minX > Pos.x )
					{	Rgn.minX = Pos.x;	}
					if ( Rgn.minY > Pos.y )
					{	Rgn.minY = Pos.y;	}
					if ( Rgn.maxX < Pos.x )
					{	Rgn.maxX = Pos.x;	}
					if ( Rgn.maxY < Pos.y )
					{	Rgn.maxY = Pos.y;	}
				}
			}
			if ( NULL != Ptr2 )
			{ 
				TREGION4D &Rgn=Rgn2;
				const TPOINT2D Pos=Ptr2->GetComponentCadPos();
				if ( DBL_MAX==Rgn.minX )
				{
					Rgn.minX=Rgn.maxX=Pos.x;
					Rgn.minY=Rgn.maxY=Pos.y;
				}
				else
				{
					if ( Rgn.minX > Pos.x )
					{	Rgn.minX = Pos.x;	}
					if ( Rgn.minY > Pos.y )
					{	Rgn.minY = Pos.y;	}
					if ( Rgn.maxX < Pos.x )
					{	Rgn.maxX = Pos.x;	}
					if ( Rgn.maxY < Pos.y )
					{	Rgn.maxY = Pos.y;	}
				}
			}
		}
		const double MaxErr=10.0;//um
		const double RgnCpX1=Rgn1.GetCpX();
		const double RgnCpY1=Rgn1.GetCpY();
		const double RgnCpX2=Rgn2.GetCpX();
		const double RgnCpY2=Rgn2.GetCpY();
		for ( size_t i=0; i<Count; i++ )
		{
			CAOIComponent *Ptr1=List1[i];
			CAOIComponent *Ptr2=List2[i];
			if ( NULL == Ptr1 ) { continue; }
			if ( NULL == Ptr2 ) { continue; }
			const TPOINT2D Pos1=Ptr1->GetComponentCadPos();
			const TPOINT2D Pos2=Ptr2->GetComponentCadPos();
			const double DistX1=Pos1.x-RgnCpX1;
			const double DistY1=Pos1.y-RgnCpY1;
			const double DistX2=Pos2.x-RgnCpX2;
			const double DistY2=Pos2.y-RgnCpY2;
			const double ErrX=DistX2-DistX1;
			const double ErrY=DistY2-DistY1;
			if ( fabs(ErrX) > MaxErr )
			{	return false; }
			if ( fabs(ErrY) > MaxErr )
			{	return false; }
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::AssignPanelFieldList(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, FIELD_BUILD_AREA_MODE AreaMode)//分配區域列表
{		
	CString        str;
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
	const bool     ByCadRegion = true;		
	BOARD_FD_GRAB_MODE BoardFdGrabMode = GetPanelBoardFdGrabMode();
	//ONLINE_STATE_MODE OnlineStateMode = AOIDataCollect.GetOnlineStateMode();
	const size_t   FdCount = GetPanelFdCount_Inline();
	const size_t   MarkCount = GetPanelMarkCount_Inline();
	const size_t   BoardCount = GetPanelBoardCount_Inline();
	const size_t   BarcodeCount = GetPanelBarcodeCount_Inline();
	const size_t   FieldCount = GetPanelFieldCount_Inline();
	const size_t   ComponentCount = GetPanelComponentCount();		

	SetPanelErrorString(_T(""));
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetPanelFieldPtr_Inline(i);
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

	if ( FIELD_BUILD_AREA_BOARD == AreaMode )
	{
		for ( i=0; i<BoardCount; i++ )
		{
			BoardPtr = GetPanelBoardPtr(i, false);
			if ( NULL == BoardPtr ) { continue; }
			if ( BoardPtr->CheckBoardNeedToCalculate(DistrictID) == false ) { continue; }			
			if ( true == ByCadRegion )
			{	BoardPtr->GetBoardRgnCad(Region);	}
			else
			{	BoardPtr->GetBoardRgnStage(DistrictID, Region);	}
			FieldPtr = BoardPtr->GetBoardPanelFieldPtr(DistrictID);
			if ( NULL != FieldPtr )
			{
				if ( FieldPtr->CheckRegionInField(Region, ByCadRegion, CheckInner) == false )
				{	FieldPtr = NULL; }
			}
			if ( NULL == FieldPtr )
			{	
				FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
				if ( NULL == FieldPtr )
				{						
					BoardPtr->ResetBoardPanelFieldParam(DistrictID);					
					continue;	
				}				
			}
			FieldPtr->AddFieldBoardPtr(BoardPtr);			
			BoardPtr->SetBoardPanelFieldToObj(FieldPtr);
		}
	}

	//Assign Field To Fd	
	if (BOARD_FD_GRAB_INSPECTING == BoardFdGrabMode )	
	{
		for ( i=0; i<FdCount; i++ )
		{
			FdPtr = GetPanelFdPtr_Inline(i);
			if ( NULL == FdPtr ) { continue; }		
			BoardPtr = FdPtr->GetFdBoardPtr();
			if ( NULL == BoardPtr ) { continue; }			
			if ( BoardPtr->GetBoardBypassed() == true ) { continue; }
			if ( BoardPtr->GetBoardMapEnable() == false ) { continue; }
			if ( FdPtr->GetFdNeedToCalculate() == false ) { continue; }
			if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
			if ( FIELD_BUILD_RANDOM_BOARD == BuildMode )
			{
				if ( NULL != BoardPtr )
				{	continue; }			
			}

			SetPanelErrorString(FdPtr->GetRgnDerivedName());
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
					FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
				FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
	
	//Assign Field To Mark
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetPanelMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }		
		if ( MarkPtr->GetMarkNeedToCalculate() == false ) { continue; }
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		if ( FIELD_BUILD_RANDOM_BOARD == BuildMode )
		{
			if ( MarkPtr->GetMarkBoardPtr() != NULL )
			{	continue; }		
		}

		SetPanelErrorString(MarkPtr->GetRgnDerivedName());
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
				FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
			FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
		BarcodePtr = GetPanelBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		if ( BarcodePtr->GetBarcodeNeedToCalculate() == false ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		if ( FIELD_BUILD_RANDOM_BOARD == BuildMode )
		{
			if ( BarcodePtr->GetBarcodeBoardPtr() != NULL )
			{	continue; }
		}

		SetPanelErrorString(BarcodePtr->GetRgnDerivedName());
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
				FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
			FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
		ComponentPtr = GetPanelComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }			
		if ( ComponentPtr->GetComponentNeedToCalculate() == false ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		if ( FIELD_BUILD_RANDOM_BOARD == BuildMode )
		{
			if ( ComponentPtr->GetComponentBoardPtr() != NULL )
			{	continue; }			
		}

		SetPanelErrorString(ComponentPtr->GetRgnDerivedName());
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
					FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
				FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
				FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
			FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
		const RECT FrameRectBefore = FrameRect;
		JetAPI::AdjustRectByAlignW(FrameRect, nAlign);//調成4倍寬
		if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
		{
			if ( 0 == SubRgnCount )
			{	
				//TREGION4D FieldRgn;
				//FieldPtr->GetFieldStageRgn_Outer(FieldRgn);
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
bool CAOIPanel::CreatePanelFieldList(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, std::vector<CAOIField*> &FieldPtrList, FIELD_DIVISION_MODE DivMode, bool ByCadRegion, FIELD_BUILD_AREA_MODE AreaMode)//建立整板區域列表
{
	TJetRgnList    RgnList;
	TJetFieldList  FieldList;
	const bool     CheckOldField = true;	
	FIELD_DIVISION_MODE DivisionMode=DivMode;
	CAOIProject   *ProjectPtr = GetPanelProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }
	if ( CreatePanelFieldListKernel(BuildMode, DistrictID, DivisionMode, ByCadRegion, CheckOldField, AreaMode, RgnList, FieldList) == false )
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
	const double FOVWOut2 = FovSizeOuter.cx/2;
	const double FOVHOut2 = FovSizeOuter.cy/2;		
	if ( GetPanelMapCTS(DistrictID, PanelMapCTS) == false ) { return false; }
	if ( GetPanelMapSTC(DistrictID, PanelMapSTC) == false ) { return false; }

	std::vector<CAOIRgn*> FieldRgnList;
	for ( i=0; i<JetFieldCount; i++ )
	{
		JetFieldPtr = &(FieldList[i]);

		FieldPtr = AOIObjManager.CreateFieldObj();
		if ( FieldPtr == NULL ) { continue; }

		FieldRgnList.clear();
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
			CAOIObj *ObjPtr = (CAOIObj*)(JetRgnPtr->Ptr);
			if ( NULL == ObjPtr ) { continue; }
			AOIType = ObjPtr->GetObjType();

			if ( AOI_OBJ_BOARD == AOIType )
			{
				TREGION4D rgnFOVStageOut;
				std::vector<CAOIRgn*> BoardRgnList;
				CAOIBoard *BoardPtr = (CAOIBoard*)JetRgnPtr->Ptr;
				rgnFOVStageOut.minX = StagePos.x-FOVWOut2;
				rgnFOVStageOut.minY = StagePos.y-FOVHOut2;
				rgnFOVStageOut.maxX = StagePos.x+FOVWOut2;
				rgnFOVStageOut.maxY = StagePos.y+FOVHOut2;
				FieldPtr->AddFieldBoardPtr(BoardPtr);
				BoardPtr->BuildBoardObjectListInRegion(DistrictID, rgnFOVStageOut, false, BoardRgnList);				
				for ( size_t k=0; k<BoardRgnList.size(); k++ )
				{	FieldRgnList.push_back(BoardRgnList[k]);	}				
			}
			else
			{
				RgnPtr = (CAOIRgn*)JetRgnPtr->Ptr;
				FieldRgnList.push_back(RgnPtr);
			}
		}
		
		RgnCount = FieldRgnList.size();
		for ( j=0; j<RgnCount; j++ )
		{	
			RgnPtr = FieldRgnList[j];			
			if ( NULL == RgnPtr ) { continue; }
			AOIType = RgnPtr->GetObjType();			
			RgnPtr->SetRgnFieldPtr(FieldPtr);
			switch ( AOIType )
			{
			case AOI_OBJ_FD:
				break;			
			case AOI_OBJ_RGN:
				break;
			case AOI_OBJ_MARK:
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
size_t CAOIPanel::GetPanelFieldCount() const//取得整板的區域數量
{
	return GetPanelFieldCount_Inline();
}
//-------------------------------------------------------------------------------------//
CAOIField* CAOIPanel::GetPanelFieldPtr(size_t index, bool check) const//取得整板的區域指標
{
	if ( check )
	{
		const size_t Count = GetPanelFieldCount_Inline();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return GetPanelFieldPtr_Inline(index);	
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::AddPanelFieldPtr(CAOIField *FieldPtr)//增加整板的區域
{	
	AddPanelFieldPtr_Inline(FieldPtr);	
	FieldPtr->SetFieldPanelPtr(this);	
	FieldPtr->SetFieldPanelIndex(GetPanelIndex_Project());	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::SelectPanelAllFields(bool Select)//選取整板的區域
{
	size_t i = 0;
	CAOIField*  FieldPtr = NULL;
	const size_t FieldCount = GetPanelFieldCount_Inline();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetPanelFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		FieldPtr->SetFieldSelected(Select);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::RemovePanelFieldSelected()//移除選取到的整板的區域
{
	size_t i = 0;
	int    index = 0;
	CAOIField*  FieldPtr = NULL;
	std::vector<CAOIField*> PanelFieldPtrList = m_PanelFieldPtrList;
	const size_t FieldCount = PanelFieldPtrList.size();

	index = 0;
	m_PanelFieldPtrList.clear();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = PanelFieldPtrList[i];
		if ( NULL == FieldPtr ) { continue; }
		if ( FieldPtr->GetFieldSelected() == true ) { continue; }
		AddPanelFieldPtr_Inline(FieldPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::RemovePanelAllFields()//移除整板的區域
{
	m_PanelFieldPtrList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::RemovePanelAllFields(DISTRICT_ID DistrictID)//移除整板的區域
{
	size_t       i=0;
	CAOIField   *FieldPtr = NULL;
	std::vector<CAOIField*>   FieldPtrList;
	const size_t FieldCount = GetPanelFieldCount_Inline();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetPanelFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		if ( DistrictID == FieldPtr->GetFieldDistrictID() ) { continue; }
		FieldPtrList.push_back(FieldPtr);
	}
	m_PanelFieldPtrList = FieldPtrList;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::LayoutPanelFieldList()//重整整板的區域列表
{
	size_t i = 0;
	int    index = 0;
	CAOIField*  FieldPtr = NULL;
	std::vector<CAOIField*> PanelFieldPtrList = m_PanelFieldPtrList;
	const size_t FieldCount = PanelFieldPtrList.size();

	index = 0;
	m_PanelFieldPtrList.clear();
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = PanelFieldPtrList[i];
		if ( NULL == FieldPtr ) { continue; }				
		AddPanelFieldPtr_Inline(FieldPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIPanel::GetPanelComponentCount() const//取得整板的零件數量
{
	return GetPanelComponentCount_Inline();
}
//-------------------------------------------------------------------------------------//
size_t CAOIPanel::GetPanelComponentNGCount() const//取得整板的瑕疵零件數量	
{
	std::vector<CAOIComponent*> List;
	if ( GetPanelComponentNGList(List) == false )
	{	return 0; }
	return List.size();
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::GetPanelComponentNGList(std::vector<CAOIComponent*> &List) const//取得整板的瑕疵零件列表
{
	size_t i=0;
	size_t ComponentCountNG = 0;
	RESULT_ID ResultID=RESULT_ID_NONE;	
	CAOIComponent *ComponentPtr = NULL;
	const size_t ComponentCount = GetPanelComponentCount();	
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetPanelComponentPtr(i, false);
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
CAOIComponent* CAOIPanel::GetPanelComponentPtr(size_t index, bool check) const//取得整板的零件指標
{
	if ( check )
	{
		const size_t Count = GetPanelComponentCount_Inline();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return GetPanelComponentPtr_Inline(index);	
}
//-------------------------------------------------------------------------------------//
CAOIComponent* CAOIPanel::GetPanelComponentPtrByName(LPCSTR ComponentName) const//取得整板的零件指標
{
	size_t          i = 0;
	CString         strComponentName;
	CString         refComponentName = ComponentName;
	CAOIComponent*  ComponentPtr = NULL;
	const size_t ComponentCount = GetPanelComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetPanelComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		strComponentName = ComponentPtr->GetComponentName();
		if ( strComponentName.CompareNoCase(refComponentName) == 0 ) 
		{	return ComponentPtr; }
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
CAOIComponent* CAOIPanel::GetPanelComponentPtrByName(LPCWSTR ComponentName) const//取得整板的零件指標
{
	size_t          i = 0;
	CString         strComponentName;
	CString         refComponentName = ComponentName;
	CAOIComponent*  ComponentPtr = NULL;
	const size_t ComponentCount = GetPanelComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetPanelComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		strComponentName = ComponentPtr->GetComponentName();
		if ( strComponentName.CompareNoCase(refComponentName) == 0 ) 
		{	return ComponentPtr; }
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::AddPanelComponentPtr(CAOIComponent *ComponentPtr)//增加整板的零件
{
	const int ComponentIndex = (int)(GetPanelComponentCount_Inline());	
	ComponentPtr->SetComponentPanelPtr(this);
	ComponentPtr->SetComponentPanelIndex_Project(m_PanelIndex_Project);
	ComponentPtr->SetComponentIndex_Panel(ComponentIndex);	
	AddPanelComponentPtr_Inline(ComponentPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::AddPanelComponentList(std::vector<CAOIComponent*> &List)//增加整板的零件列表 
{
	size_t i=0;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	AddPanelComponentPtr(List[i]);	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::SelectPanelAllComponents(bool Select)//選取整板的零件
{
	size_t i = 0;
	CAOIComponent*  ComponentPtr = NULL;
	const size_t ComponentCount = GetPanelComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetPanelComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		ComponentPtr->SetComponentSelected(Select);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::RemovePanelComponentSelected()//移除選取到的整板的零件
{
	size_t i = 0;
	int    index = 0;
	CAOIComponent*  ComponentPtr = NULL;
	std::vector<CAOIComponent*> PanelComponentPtrList = m_PanelComponentPtrList;
	const size_t ComponentCount = PanelComponentPtrList.size();

	index = 0;
	m_PanelComponentPtrList.clear();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = PanelComponentPtrList[i];
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentSelected() == true ) { continue; }
		
		ComponentPtr->SetComponentIndex_Panel(index);
		m_PanelComponentPtrList.push_back(ComponentPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::RemovePanelAllComponents()//移除整板的零件
{
	this->m_PanelComponentPtrList.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::LayoutPanelComponentList()//重整整板的零件列表
{
	size_t i = 0;
	int    index = 0;
	CAOIComponent*  ComponentPtr = NULL;
	std::vector<CAOIComponent*> PanelComponentPtrList = this->m_PanelComponentPtrList;
	const size_t ComponentCount = PanelComponentPtrList.size();

	index = 0;
	this->m_PanelComponentPtrList.clear();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = PanelComponentPtrList[i];
		if ( NULL == ComponentPtr ) { continue; }		
		
		ComponentPtr->SetComponentIndex_Panel(index);
		this->m_PanelComponentPtrList.push_back(ComponentPtr);
		index ++;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::InvertSelectPanelComponent()//反向選取零件
{
	size_t          i = 0;
	bool            Selected = false;
	CAOIComponent*  ComponentPtr = NULL;
	const size_t ComponentCount = CAOIPanel::GetPanelComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = CAOIPanel::GetPanelComponentPtr_Inline(i);
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
bool CAOIPanel::SetPanelComponentOrgCadPos()//設定整板零件Cad座標為原始Cad座標
{
	size_t          i = 0;
	TPOINT2D        CadPos;
	CAOIComponent*  ComponentPtr = NULL;
	const size_t ComponentCount = CAOIPanel::GetPanelComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = CAOIPanel::GetPanelComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		CadPos = ComponentPtr->GetComponentCadPos();
		ComponentPtr->SetComponentOrgCadPos(CadPos);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::ReplacePanelComponentModelName(std::vector<TAliasNode> &AliasList)//取代整板零件的模組名稱
{
	size_t          i=0, j=0;	
	std::wstring    PartNumber;		
	TAliasNode     *AliasPtr = NULL;	
	CAOIModel      *ModelPtr = NULL;
	CAOIComponent  *ComponentPtr = NULL;
	const size_t AliasCount = AliasList.size();
	const size_t ComponentCount = CAOIPanel::GetPanelComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = CAOIPanel::GetPanelComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		
		PartNumber = ComponentPtr->GetComponentPartNumber();
		JetAPI::wstring2upper(PartNumber);
		for ( j=0; j<AliasCount; j++ )
		{
			AliasPtr = &(AliasList[j]);
			if ( PartNumber.compare(AliasPtr->wsPartNumber) != 0 ) { continue; }			
			break;
		}
		if ( AliasCount == j ) { continue; }		
		ComponentPtr->SetComponentModelName(AliasPtr->wsModelName.c_str());

		ModelPtr = ComponentPtr->GetComponentModelPtr();
		if ( NULL != ModelPtr )
		{	ModelPtr->SetModelName(AliasPtr->wsModelName.c_str()); }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::SpinPanel(double Angle)//自轉整板
{
	double CadAngle = 0;
	double StageAngle = 0;		
	CadAngle = Angle;
	AOIDataCollect.MapCadAngleToStage(CadAngle, StageAngle);

	LayoutPanelRegionCad();	
	TREGION4D CadRgn = GetPanelRgnCad();	
	const double PanelCadCpX = (CadRgn.minX+CadRgn.maxX)*0.5;
	const double PanelCadCpY = (CadRgn.minY+CadRgn.maxY)*0.5;		

	if ( RotatePanel(CadAngle, PanelCadCpX, PanelCadCpY) == false )
	{	return false; }	
	//if ( CAOIPanel::UpdatePanelComponentToModel() == false )
	//{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::MovePanelPos(double dX, double dY)//移動整板
{	
	size_t         i=0, j=0;
	DISTRICT_ID    DistrictID;
	CAOIFd        *pFd = NULL;
	CAOIMark      *pMark = NULL;
	CAOIBarcode   *pBarcode = NULL;
	CAOIBoard     *pBoard = NULL;	
	CAOIComponent *pComponent = NULL;	
	CMapCoordinate *MapCTSPtrDA = GetPanelMapCTSPtr(DISTRICT_ID_A);
	CMapCoordinate *MapCTSPtrDB = GetPanelMapCTSPtr(DISTRICT_ID_B);
	if ( NULL==MapCTSPtrDA || NULL==MapCTSPtrDB ) { return; }

	const size_t FdCount = GetPanelFdCount_Inline();	
	const size_t MarkCount = GetPanelMarkCount_Inline();
	const size_t BoardCount = GetPanelBoardCount_Inline();
	const size_t BarcodeCount = GetPanelBarcodeCount_Inline();
	const size_t ComponentCount = GetPanelComponentCount_Inline();

	for ( i=0; i<FdCount; i++ )
	{
		pFd = GetPanelFdPtr_Inline(i);
		if ( NULL == pFd ) { continue; }
		DistrictID = pFd->GetFdDistrictID();
		switch ( DistrictID ) 
		{
		case DISTRICT_ID_A:	pFd->MoveFdPos(dX, dY, MapCTSPtrDA); break;
		case DISTRICT_ID_B:	pFd->MoveFdPos(dX, dY, MapCTSPtrDB); break;
		}		
	}

	for ( i=0; i<MarkCount; i++ )
	{
		pMark = GetPanelMarkPtr_Inline(i);
		if ( NULL == pMark ) { continue; }
		DistrictID = pMark->GetMarkDistrictID();
		switch ( DistrictID ) 
		{
		case DISTRICT_ID_A:	pMark->MoveMarkPos(dX, dY, MapCTSPtrDA); break;
		case DISTRICT_ID_B:	pMark->MoveMarkPos(dX, dY, MapCTSPtrDB); break;
		}
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		pBarcode = GetPanelBarcodePtr_Inline(i);
		if ( NULL == pBarcode ) { continue; }
		DistrictID = pBarcode->GetBarcodeDistrictID();
		switch ( DistrictID ) 
		{
		case DISTRICT_ID_A:	pBarcode->MoveBarcodePos(dX, dY, MapCTSPtrDA); break;
		case DISTRICT_ID_B:	pBarcode->MoveBarcodePos(dX, dY, MapCTSPtrDB); break;
		}
	}

	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetPanelComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }
		DistrictID = pComponent->GetComponentDistrictID();
		switch ( DistrictID ) 
		{
		case DISTRICT_ID_A:	pComponent->MoveComponentPos(dX, dY, MapCTSPtrDA); break;
		case DISTRICT_ID_B:	pComponent->MoveComponentPos(dX, dY, MapCTSPtrDB); break;
		}
	}

	CAOIField   *FieldPtr=NULL;
	const size_t FieldCount = GetPanelFieldCount_Inline();		
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetPanelFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		DistrictID = FieldPtr->GetFieldDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	FieldPtr->MoveFieldPos(dX, dY, MapCTSPtrDA); break;
		case DISTRICT_ID_B:	FieldPtr->MoveFieldPos(dX, dY, MapCTSPtrDB); break;
		}
	}

	size_t  BoardFieldCount=0;
	for ( i=0; i<BoardCount; i++ )
	{
		pBoard = GetPanelBoardPtr_Inline(i);
		if ( NULL == pBoard ) { continue; }
		BoardFieldCount = pBoard->GetBoardFieldCount();
		for ( j=0; j<BoardFieldCount; j++ )
		{
			FieldPtr = pBoard->GetBoardFieldPtr(j, false);
			if ( NULL == FieldPtr ) { continue; }
			DistrictID = FieldPtr->GetFieldDistrictID();
			switch ( DistrictID )
			{
			case DISTRICT_ID_A:	FieldPtr->MoveFieldPos(dX, dY, MapCTSPtrDA); break;
			case DISTRICT_ID_B:	FieldPtr->MoveFieldPos(dX, dY, MapCTSPtrDB); break;
			}			
		}
	}	
	CalcPanelStagePosition();	
	LayoutPanelBoardListRegion();
	return ;	
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::MovePanelStagePos(double dX, double dY, DISTRICT_ID DistrictID)//移動整板	
{
	size_t i=0, j=0;
	TPOINT2D       StagePos;
	CAOIFd        *pFd = NULL;
	CAOIMark      *pMark = NULL;
	CAOIBarcode   *pBarcode = NULL;
	CAOIBoard     *pBoard = NULL;	
	CAOIComponent *pComponent = NULL;	
	CMapCoordinate *MapCTSPtr = GetPanelMapCTSPtr(DistrictID);
	CMapCoordinate *MapSTCPtr = GetPanelMapSTCPtr(DistrictID);
	if ( NULL == MapCTSPtr ) { return; }
	if ( NULL == MapSTCPtr ) { return; }

	const size_t FdCount = GetPanelFdCount_Inline();	
	const size_t MarkCount = GetPanelMarkCount_Inline();
	const size_t BoardCount = GetPanelBoardCount_Inline();
	const size_t BarcodeCount = GetPanelBarcodeCount_Inline();
	const size_t ComponentCount = GetPanelComponentCount_Inline();
	MapCTSPtr->MoveMatrix2D(dX, dY);

	for ( i=0; i<FdCount; i++ )
	{
		pFd = GetPanelFdPtr_Inline(i);
		if ( NULL == pFd ) { continue; }
		if ( DistrictID != pFd->GetFdDistrictID() ) { continue; }
		//pFd->MoveFdStagePos(dX, dY);
		pFd->MapFdCadToStagePos(*MapCTSPtr);
		StagePos.x = pFd->GetFdStagePosX();
		StagePos.y = pFd->GetFdStagePosY();
		pFd->SetFdTeachStagePosX(StagePos.x);
		pFd->SetFdTeachStagePosY(StagePos.y);
	}

	for ( i=0; i<MarkCount; i++ )
	{
		pMark = GetPanelMarkPtr_Inline(i);
		if ( NULL == pMark ) { continue; }
		if ( DistrictID != pMark->GetMarkDistrictID() ) { continue; }
		//pMark->MoveMarkStagePos(dX, dY);
		pMark->MapMarkCadToStagePos(*MapCTSPtr);
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		pBarcode = GetPanelBarcodePtr_Inline(i);
		if ( NULL == pBarcode ) { continue; }
		if ( DistrictID != pBarcode->GetBarcodeDistrictID() ) { continue; }
		//pBarcode->MoveBarcodeStagePos(dX, dY);
		pBarcode->MapBarcodeCadToStagePos(*MapCTSPtr);
	}

	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetPanelComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }
		if ( DistrictID != pComponent->GetComponentDistrictID() )
		{	continue; }
		//pComponent->MoveComponentStagePos(dX, dY);
		pComponent->MapComponentCadToStagePos(*MapCTSPtr);
	}
	
	CAOIField   *FieldPtr=NULL;
	const size_t FieldCount = GetPanelFieldCount_Inline();		
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetPanelFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		FieldPtr->MapFieldCadToStagePos(*MapCTSPtr);		
	}

	size_t  BoardFieldCount=0;
	for ( i=0; i<BoardCount; i++ )
	{
		pBoard = GetPanelBoardPtr_Inline(i);
		if ( NULL == pBoard ) { continue; }
		BoardFieldCount = pBoard->GetBoardFieldCount();
		for ( j=0; j<BoardFieldCount; j++ )
		{
			FieldPtr = pBoard->GetBoardFieldPtr(j, false);
			if ( NULL == FieldPtr ) { continue; }
			FieldPtr->MapFieldCadToStagePos(*MapCTSPtr);
		}
		pBoard->SetBoardMapCTS(DistrictID, MapCTSPtr);
		pBoard->LayoutBoardRegionStage(DistrictID);
	}	
	LayoutPanelRegionStage(DistrictID);
	return ;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::RotatePanel(double Angle, double CpX, double CpY)//旋轉整板
{
	size_t         i = 0;
	DISTRICT_ID    DistrictID;
	CAOIFd        *pFd = NULL;
	CAOIMark      *pMark = NULL;
	CAOIBarcode   *pBarcode = NULL;	
	CAOIComponent *pComponent = NULL;	
	CMapCoordinate *MapCTSPtrDA = GetPanelMapCTSPtr(DISTRICT_ID_A);
	CMapCoordinate *MapCTSPtrDB = GetPanelMapCTSPtr(DISTRICT_ID_B);
	if ( NULL==MapCTSPtrDA || NULL==MapCTSPtrDB ) { return false; }

	const size_t FdCount = GetPanelFdCount_Inline();
	const size_t MarkCount = GetPanelMarkCount_Inline();
	const size_t BarcodeCount = GetPanelBarcodeCount_Inline();	
	const size_t ComponentCount = GetPanelComponentCount_Inline();

	for ( i=0; i<FdCount; i++ )
	{
		pFd = GetPanelFdPtr_Inline(i);
		if ( NULL == pFd ) { continue; }
		DistrictID = pFd->GetFdDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pFd->RotateFd(Angle, CpX, CpY, MapCTSPtrDA); break;
		case DISTRICT_ID_B:	pFd->RotateFd(Angle, CpX, CpY, MapCTSPtrDB); break;
		}		
	}

	for ( i=0; i<MarkCount; i++ )
	{
		pMark = GetPanelMarkPtr_Inline(i);
		if ( NULL == pMark ) { continue; }
		DistrictID = pMark->GetMarkDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pMark->RotateMark(Angle, CpX, CpY, MapCTSPtrDA); break;
		case DISTRICT_ID_B:	pMark->RotateMark(Angle, CpX, CpY, MapCTSPtrDB); break;
		}
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		pBarcode = GetPanelBarcodePtr_Inline(i);
		if ( NULL == pBarcode ) { continue; }
		DistrictID = pBarcode->GetBarcodeDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pBarcode->RotateBarcode(Angle, CpX, CpY, MapCTSPtrDA); break;
		case DISTRICT_ID_B:	pBarcode->RotateBarcode(Angle, CpX, CpY, MapCTSPtrDB); break;
		}
	}

	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetPanelComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }
		DistrictID = pComponent->GetComponentDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pComponent->RotateComponent(Angle, CpX, CpY, MapCTSPtrDA); break;
		case DISTRICT_ID_B:	pComponent->RotateComponent(Angle, CpX, CpY, MapCTSPtrDB); break;
		}
		pComponent->RotateComponentBasePlaneParam(Angle);
	}
	LayoutPanelBoardListRegion();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::MirrorXPanel(double CpX)//鏡射整板-X值
{
	size_t         i = 0;
	DISTRICT_ID    DistrictID;
	CAOIFd        *pFd = NULL;
	CAOIMark      *pMark = NULL;
	CAOIBarcode   *pBarcode = NULL;	
	CAOIComponent *pComponent = NULL;	
	CMapCoordinate *MapCTSPtrDA = GetPanelMapCTSPtr(DISTRICT_ID_A);
	CMapCoordinate *MapCTSPtrDB = GetPanelMapCTSPtr(DISTRICT_ID_B);
	if ( NULL==MapCTSPtrDA || NULL==MapCTSPtrDB ) { return false; }

	const size_t FdCount = GetPanelFdCount_Inline();
	const size_t MarkCount = GetPanelMarkCount_Inline();
	const size_t BarcodeCount = GetPanelBarcodeCount_Inline();	
	const size_t ComponentCount = GetPanelComponentCount_Inline();

	for ( i=0; i<FdCount; i++ )
	{
		pFd = GetPanelFdPtr_Inline(i);
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
		pMark = GetPanelMarkPtr_Inline(i);
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
		pBarcode = GetPanelBarcodePtr_Inline(i);
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
		pComponent = GetPanelComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }
		DistrictID = pComponent->GetComponentDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pComponent->MirrorXComponent(CpX, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pComponent->MirrorXComponent(CpX, MapCTSPtrDB);	break;
		}
	}	
	CalcPanelStagePosition();	
	LayoutPanelBoardListRegion();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::MirrorYPanel(double CpY)//鏡射整板-Y值
{
	size_t i = 0;
	DISTRICT_ID    DistrictID;
	CAOIFd        *pFd = NULL;
	CAOIMark      *pMark = NULL;
	CAOIBarcode   *pBarcode = NULL;
	CAOIBoard     *pBoard = NULL;
	CAOIComponent *pComponent = NULL;	
	CMapCoordinate *MapCTSPtrDA = GetPanelMapCTSPtr(DISTRICT_ID_A);
	CMapCoordinate *MapCTSPtrDB = GetPanelMapCTSPtr(DISTRICT_ID_B);
	if ( NULL==MapCTSPtrDA || NULL==MapCTSPtrDB ) { return false; }

	const size_t FdCount = GetPanelFdCount_Inline();
	const size_t MarkCount = GetPanelMarkCount_Inline();
	const size_t BarcodeCount = GetPanelBarcodeCount_Inline();
	const size_t BoardCount = GetPanelBoardCount_Inline();
	const size_t ComponentCount = GetPanelComponentCount_Inline();

	for ( i=0; i<FdCount; i++ )
	{
		pFd = GetPanelFdPtr_Inline(i);
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
		pMark = GetPanelMarkPtr_Inline(i);
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
		pBarcode = GetPanelBarcodePtr_Inline(i);
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
		pComponent = GetPanelComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }
		DistrictID = pComponent->GetComponentDistrictID();
		switch ( DistrictID )
		{
		case DISTRICT_ID_A:	pComponent->MirrorYComponent(CpY, MapCTSPtrDA);	break;
		case DISTRICT_ID_B:	pComponent->MirrorYComponent(CpY, MapCTSPtrDB);	break;
		}
	}
	CalcPanelStagePosition();	
	LayoutPanelBoardListRegion();
	return true;
}
//-------------------------------------------------------------------------------------//
DISTRICT_ID CAOIPanel::GetPanelActDistrictID() const
{
	return m_PanelActDistrictID; 
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelActDistrictID(DISTRICT_ID value)
{
	m_PanelActDistrictID = value; 
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIPanel::GetPanelRgnCad() const
{
	return m_PanelRgnCad;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::GetPanelRgnCad(TREGION4D &Rgn) const
{
	Rgn=m_PanelRgnCad;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelRgnCad(const TREGION4D &Rgn)
{	
	m_PanelRgnCad=Rgn;
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIPanel::CalcPanelRgnCad() const
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
	const size_t FdCount = GetPanelFdCount_Inline();
	const size_t MarkCount = GetPanelMarkCount_Inline();
	const size_t BarcodeCount = GetPanelBarcodeCount_Inline();
	const size_t ComponentCount = GetPanelComponentCount_Inline();	

	bGetRgn=false;
	CadRgn.Limit(true);	
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
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
		MarkPtr = GetPanelMarkPtr_Inline(i);
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
		BarcodePtr = GetPanelBarcodePtr_Inline(i);
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
		pComponent = GetPanelComponentPtr_Inline(i);
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
	return CadRgn;
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIPanel::CalcPanelRgnCad(DISTRICT_ID DistrictID) const
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
	const size_t FdCount = GetPanelFdCount_Inline();
	const size_t MarkCount = GetPanelMarkCount_Inline();
	const size_t BarcodeCount = GetPanelBarcodeCount_Inline();
	const size_t ComponentCount = GetPanelComponentCount_Inline();	

	bGetRgn=false;
	CadRgn.Limit(true);	
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdDistrictID() != DistrictID )
		{	continue; }
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
		MarkPtr = GetPanelMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( MarkPtr->GetMarkDistrictID() != DistrictID )
		{	continue; }
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
		BarcodePtr = GetPanelBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		if ( BarcodePtr->GetBarcodeDistrictID() != DistrictID )
		{	continue; }
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
		pComponent = GetPanelComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }		
		if ( pComponent->GetComponentDistrictID() != DistrictID )
		{	continue; }
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
	return CadRgn;
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIPanel::GetPanelRgnStage(DISTRICT_ID DistrictID) const
{
	TREGION4D Rgn;
	switch ( DistrictID )
	{
	case DISTRICT_ID_A:	Rgn = m_PanelRgnStage_DA;	break;
	case DISTRICT_ID_B:	Rgn = m_PanelRgnStage_DB;	break;
	}
	return Rgn;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::GetPanelRgnStage(DISTRICT_ID DistrictID, TREGION4D &Rgn) const
{
	switch ( DistrictID )
	{
	case DISTRICT_ID_A:	Rgn=m_PanelRgnStage_DA;	break;
	case DISTRICT_ID_B:	Rgn=m_PanelRgnStage_DB;	break;
	}
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelRgnStage(DISTRICT_ID DistrictID, const TREGION4D &Rgn)
{
	switch ( DistrictID )
	{
	case DISTRICT_ID_A:	m_PanelRgnStage_DA=Rgn;	break;
	case DISTRICT_ID_B:	m_PanelRgnStage_DB=Rgn;	break;
	}
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelBarcode(const char *barcode)
{
	if ( NULL == barcode ) { return; }
	JetAPI::char2wstring(barcode, m_PanelBarcode);	
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelBarcode(const wchar_t *barcode)
{
	if ( NULL == barcode ) { return; }
	m_PanelBarcode = barcode;	
}
//-------------------------------------------------------------------------------------//
const wchar_t* CAOIPanel::GetPanelBarcode() const
{
	return m_PanelBarcode.c_str();
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::BackupPanelBarcode()//備份整板條碼
{
	m_PanelBarcodeBackup = m_PanelBarcode;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::RestorePanelBarcode()//恢復整板條碼
{
	m_PanelBarcode = m_PanelBarcodeBackup;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelIsGetBarcode(bool value) 
{ 
	m_PanelIsGetBarcode = value; 
}
//-------------------------------------------------------------------------------------//
double CAOIPanel::GetPanelSkew() const
{
	return m_PanelSkew;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelSkew(double value)
{
	m_PanelSkew = value;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::GetPanelIsGetBarcode() const 
{ 
	return m_PanelIsGetBarcode; 
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::WritePanelFile(CAOIFileIO &FileIO)//儲存整板檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	CAOIPanel *pPanel = this;
	char       uuidStr[MAX_JET_PATH]="";	
	wchar_t    uuidWStr[MAX_JET_PATH]=L"";	
	UUID       uuid = pPanel->GetObjUuid();	
	FileIO.SetFnName(_T("CAOIPanel::WritePanelFile"));
	JetAPI::UUIDToStringA(uuid, uuidStr);
	JetAPI::UUIDToStringW(uuid, uuidWStr);
	//----------------------------------------------------------------------------------------//
	//整板參數
	if ( FileIO.SaveChunk_INT(FILE_IO_PANEL_START, 0) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_PANEL_INDEX_PROJECT, pPanel->GetPanelIndex_Project()) == false ) { return false; }
	if ( FileIO.SaveChunk_BOL(FILE_IO_PANEL_BYPASS, pPanel->GetPanelBypassed()) == false ) { return false; }
	if ( FileIO.GetSaveWStr() == true ) 
	{	if ( FileIO.SaveChunk_STR(FILE_IO_PANEL_OBJ_UUID, uuidWStr) == false ) { return false; } }
	else
	{	if ( FileIO.SaveChunk_STR(FILE_IO_PANEL_OBJ_UUID, uuidStr) == false ) { return false; } }
	if ( FileIO.SaveChunk_INT(FILE_IO_PANEL_BARCODE_DEVICE_INDEX, pPanel->GetPanelBarcodeDeviceIndex()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_PANEL_BARCODE_DEVICE_CODE_INDEX, pPanel->GetPanelBarcodeDeviceCodeIndex()) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_PANEL_TYPE, pPanel->GetPanelType()) == false ) { return false; }		
	if ( FileIO.SaveChunk_BOL(FILE_IO_PANEL_BARCODE_ENABLED, pPanel->GetPanelBarcodeEnabled()) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_PANEL_BOARD_ROW_COUNT, pPanel->GetPanelBoardRowCount()) == false ) { return false; }		
	if ( FileIO.SaveChunk_INT(FILE_IO_PANEL_BOARD_COL_COUNT, pPanel->GetPanelBoardColCount()) == false ) { return false; }		
	if ( FileIO.SaveChunk_INT(FILE_IO_PANEL_BOARD_COL_BLOCK_COUNT, pPanel->GetPanelBoardColBlockCount()) == false ) { return false; }		

	if ( FileIO.SaveChunk_INT(FILE_IO_PANEL_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::ReadPanelFile(CAOIFileIO &FileIO)//載入整板檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	UUID       uuid;
	int        index = 0;	
	CAOIPanel *pPanel = this;	
	FileIO.SetFnName(_T("CAOIPanel::ReadPanelFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )
		{	continue; }

		switch ( index )
		{
		case FILE_IO_PANEL_START://整板參數-起點
			break;
		case FILE_IO_PANEL_END://整板參數-終點
			return true;
			break;
		case FILE_IO_PANEL_INDEX_PROJECT:
			pPanel->SetPanelIndex_Project(FileIO.GetData_INT());
			break;
		case FILE_IO_PANEL_BYPASS:
			pPanel->SetPanelBypassed(FileIO.GetData_BOL());
			break;
		case FILE_IO_PANEL_OBJ_UUID://整板參數-不檢測
			if ( FileIO.GetLoadWStr()==true )
			{
				if ( JetAPI::UUIDFromStringW(FileIO.GetData_WSTR(), uuid) == true )
				{	pPanel->SetObjUuid(uuid);	}
			}
			else
			{
				if ( JetAPI::UUIDFromStringA(FileIO.GetData_STR(), uuid) == true )
				{	pPanel->SetObjUuid(uuid);	}
			}
			break;
		case FILE_IO_PANEL_BARCODE_DEVICE_INDEX://整板參數-條碼機引數
			pPanel->SetPanelBarcodeDeviceIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_PANEL_BARCODE_DEVICE_CODE_INDEX://整板參數-條碼機條碼引數
			pPanel->SetPanelBarcodeDeviceCodeIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_PANEL_TYPE://整板參數-樣式
			pPanel->SetPanelType((PANEL_TYPE)FileIO.GetData_INT());
			break;
		case FILE_IO_PANEL_BARCODE_ENABLED://整板參數-條碼啟用
			pPanel->SetPanelBarcodeEnabled(FileIO.GetData_BOL());
			break;

		case FILE_IO_PANEL_BOARD_ROW_COUNT://整板參數-單板列數
			pPanel->SetPanelBoardRowCount(FileIO.GetData_INT());
			break;
		case FILE_IO_PANEL_BOARD_COL_COUNT://整板參數-單板欄數
			pPanel->SetPanelBoardColCount(FileIO.GetData_INT());
			break;
		case FILE_IO_PANEL_BOARD_COL_BLOCK_COUNT://整板參數-單板欄區塊數
			pPanel->SetPanelBoardColBlockCount(FileIO.GetData_INT());
			break;

		default:
			break;
		}
	};		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::WritePanelSpcFile_JSON_VRS(FILE *pfile, CAOIProject *ProjectPtr)
{
	if ( NULL == pfile ) { return false; }
	if ( NULL == ProjectPtr ) { return false; }

	CAOIPanel *PanelPtr = this;
	if ( NULL == PanelPtr ) { return false; }	
	
	size_t           j=0;	
	TREGION4D        MapRgn;	
	TREGION4D        Region;
	CString          strText;
	const size_t     szBuffer = 256;
	int              InternalBarcode = 0;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";	
	
	CAOIFd          *FdPtr=NULL;
	CAOIBoard       *BoardPtr=NULL;
	size_t PanelFdCount = PanelPtr->GetPanelFdCount();
	size_t PanelBoardCount = PanelPtr->GetPanelBoardCount();	

	LANE_ID    LaneID = ProjectPtr->GetProjectActLaneID();	
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();	

	::wcscpy(strTag, L"Barcode");	
	strText = PanelPtr->GetPanelBarcode();
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	JetAPI::FilterJSONStringW(strBuffer);
	::fwprintf(pfile, L"    \"%s\": \"%s\",\n", strTag, strBuffer);
	
	::wcscpy(strTag, L"Internal_Barcode");
	if ( PanelPtr->GetPanelIsGetBarcode() == false ) { InternalBarcode = 1; }
	else { InternalBarcode = 0; }	
	::fwprintf(pfile, L"    \"%s\": %d,\n", strTag, InternalBarcode);

	Region = PanelPtr->GetPanelRgnStage(DistrictID);
	ProjectPtr->MapProjectStageToMapPos(Region.minX, Region.minY, MapRgn.minX, MapRgn.minY, LaneID, DistrictID);
	ProjectPtr->MapProjectStageToMapPos(Region.maxX, Region.maxY, MapRgn.maxX, MapRgn.maxY, LaneID, DistrictID);
	Region.minX = MIN(MapRgn.minX, MapRgn.maxX);
	Region.minY = MIN(MapRgn.minY, MapRgn.maxY);
	Region.maxX = MAX(MapRgn.minX, MapRgn.maxX);
	Region.maxY = MAX(MapRgn.minY, MapRgn.maxY);		
	::wcscpy(strTag, L"Map_Location");//"Location" : [100,100,300,200,100,100,300,200],	// 在底圖上的位置(LTx LTy RTx RTy LBx LBy RBx RBy)(pixel)
	//::fwprintf(pfile, L"    \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, Region.minX, Region.minY, Region.maxX, Region.minY, Region.minX, Region.maxY, Region.maxX, Region.maxY);
	::fwprintf(pfile, L"    \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, Region.minX, Region.minY, Region.minX, Region.maxY, Region.maxX, Region.maxY, Region.maxX, Region.minY);//20190919
		
	::wcscpy(strTag, L"Type");			
	::fwprintf(pfile, L"    \"%s\": %d,\n", strTag, PanelPtr->GetPanelType());//1:normal

	//0:No Test  1:Skip 2:Bypass 3:Sys-Ok 4:Sys-Ng 5:Op-Ok 6:Op-Ng 7:Fd-Ng 
	RESULT_ID PanelResultID = PanelPtr->GetPanelResultID();		
	SPC_RESULT_ID SpcResultID = AOIDataCollect.MapResultIDToSpcResultID(PanelResultID);
	::wcscpy(strTag, L"Test_Result");			
	::fwprintf(pfile, L"    \"%s\": %d,\n", strTag, SpcResultID);		
	
	::wcscpy(strTag, L"N_FDs");// "N FD" : 2,			// 定位點數
	::fwprintf(pfile, L"    \"%s\": %d,\n", strTag, PanelFdCount);
	for ( j=0; j<PanelFdCount; j++ )
	{
		FdPtr = PanelPtr->GetPanelFdPtr(j, false);
		if ( NULL == FdPtr ) { continue; }
		::fwprintf(pfile, L"    \"FD_%d\":\n", j+1);
		::fwprintf(pfile, L"    {\n");
		if ( FdPtr->WriteFdSpcFile_JSON_VRS(pfile, ProjectPtr) == false )
		{	return false; }
		::fwprintf(pfile, L"    },\n");
	}

	::wcscpy(strTag, L"N_Boards");// "N Board" : 2,			// 單板數
	::fwprintf(pfile, L"    \"%s\": %d,\n", strTag, PanelBoardCount);
	for ( j=0; j<PanelBoardCount; j++ )
	{
		BoardPtr = PanelPtr->GetPanelBoardPtr(j, false);
		if ( NULL == BoardPtr ) { continue; }

		::fwprintf(pfile, L"    \"Board_%d\":\n", j+1);
		::fwprintf(pfile, L"    {\n");			
		if ( BoardPtr->WriteBoardSpcFile_JSON_VRS(pfile, ProjectPtr) == false )
		{	return false; }		
		::fwprintf(pfile, L"    },\n");
	}		
	::wcscpy(strTag, L"Check_Result");//Panel Check Result
	::fwprintf(pfile, L"    \"%s\": %d\n", strTag, 0);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::WritePanelSpcFile_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr)
{	
	if ( NULL == pfile ) { return false; }
	if ( NULL == ProjectPtr ) { return false; }

	CAOIPanel *PanelPtr = this;
	if ( NULL == PanelPtr ) { return false; }	
	
	size_t           j=0;	
	TREGION4D        MapRgn;	
	TREGION4D        Region;
	CString          strText;
	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";	
	
	CAOIFd          *FdPtr=NULL;
	CAOIBoard       *BoardPtr=NULL;
	size_t PanelFdCount = PanelPtr->GetPanelFdCount();
	size_t PanelBoardCount = PanelPtr->GetPanelBoardCount();	

	LANE_ID    LaneID = ProjectPtr->GetProjectActLaneID();	
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();		

	::wcscpy(strTag, L"Barcode");	
	strText = PanelPtr->GetPanelBarcode();
	if ( PanelPtr->GetPanelIsGetBarcode() == false )
	{	strText=JetAPI::AddSpcBarcodeInternalCode(strText);	}
	JetAPI::TCHAR2wchar(strText, strBuffer, szBuffer);
	JetAPI::FilterJSONStringW(strBuffer);
	::fwprintf(pfile, L"    \"%s\": \"%s\",\n", strTag, strBuffer);		

	Region = PanelPtr->GetPanelRgnStage(DistrictID);
	ProjectPtr->MapProjectStageToMapPos(Region.minX, Region.minY, MapRgn.minX, MapRgn.minY, LaneID, DistrictID);
	ProjectPtr->MapProjectStageToMapPos(Region.maxX, Region.maxY, MapRgn.maxX, MapRgn.maxY, LaneID, DistrictID);
	Region.minX = MIN(MapRgn.minX, MapRgn.maxX);
	Region.minY = MIN(MapRgn.minY, MapRgn.maxY);
	Region.maxX = MAX(MapRgn.minX, MapRgn.maxX);
	Region.maxY = MAX(MapRgn.minY, MapRgn.maxY);		
	::wcscpy(strTag, L"Map_Location");//"Location" : [100,100,300,200,100,100,300,200],	// 在底圖上的位置(LTx LTy RTx RTy LBx LBy RBx RBy)(pixel)
	//::fwprintf(pfile, L"    \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, Region.minX, Region.minY, Region.maxX, Region.minY, Region.minX, Region.maxY, Region.maxX, Region.maxY);
	::fwprintf(pfile, L"    \"%s\": [%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f],\n", strTag, Region.minX, Region.minY, Region.minX, Region.maxY, Region.maxX, Region.maxY, Region.maxX, Region.minY);//20190919
		
	::wcscpy(strTag, L"Type");			
	::fwprintf(pfile, L"    \"%s\": %d,\n", strTag, PanelPtr->GetPanelType());//1:normal

	//0:No Test  1:Skip 2:Bypass 3:Sys-Ok 4:Sys-Ng 5:Op-Ok 6:Op-Ng 7:Fd-Ng 
	RESULT_ID PanelResultID = PanelPtr->GetPanelResultID();		
	SPC_RESULT_ID SpcResultID = AOIDataCollect.MapResultIDToSpcResultID(PanelResultID);
	::wcscpy(strTag, L"Test_Result");			
	::fwprintf(pfile, L"    \"%s\": %d,\n", strTag, SpcResultID);
		
	
	//Fd List	
	::wcscpy(strTag, L"N_FDs");// "N FD" : 2,			// 定位點數
	::fwprintf(pfile, L"    \"%s\": %d,\n", strTag, PanelFdCount);

	::wcscpy(strTag, L"FD_List");//FD_List
	::fwprintf(pfile, L"    \"%s\": %s\n", strTag, L"[");//FD_List [	
	for ( j=0; j<PanelFdCount; j++ )
	{
		FdPtr = PanelPtr->GetPanelFdPtr(j, false);
		if ( NULL == FdPtr ) { continue; }			
		::fwprintf(pfile, L"    {\n");
		::fwprintf(pfile, L"      \"ID\": %d,\n", j+1);
		if ( FdPtr->WriteFdSpcFile_JSON_RSM(pfile, ProjectPtr) == false )
		{	return false; }
		if ( j < (PanelFdCount-1) )
		{	::fwprintf(pfile, L"    },\n"); }
		else
		{	::fwprintf(pfile, L"    }\n"); }
	}
	::fwprintf(pfile, L"    ],\n");//FD_List ]	
	
	//Board List
	::wcscpy(strTag, L"N_Boards");// "N Board" : 2,			// 單板數
	::fwprintf(pfile, L"    \"%s\": %d,\n", strTag, PanelBoardCount);
	::wcscpy(strTag, L"Board_List");// Board_List
	::fwprintf(pfile, L"    \"%s\": %s\n", strTag, L"[");//Board_List [
	for ( j=0; j<PanelBoardCount; j++ )
	{
		BoardPtr = PanelPtr->GetPanelBoardPtr(j, false);
		if ( NULL == BoardPtr ) { continue; }						

		::fwprintf(pfile, L"    {\n");
		::fwprintf(pfile, L"      \"ID\": %d,\n", j+1);			
		if ( BoardPtr->WriteBoardSpcFile_JSON_RSM(pfile, ProjectPtr) == false )
		{	return false; }

		if ( j < (PanelBoardCount-1) )
		{	::fwprintf(pfile, L"    },\n"); }
		else
		{	::fwprintf(pfile, L"    }\n"); }
	}
	::fwprintf(pfile, L"    ],\n");//Board_List ]

	::wcscpy(strTag, L"Check_Result");//Panel Check Result
	::fwprintf(pfile, L"    \"%s\": %d\n", strTag, 0);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::WritePanelSpcHeader_JSON_detail(FILE *pfile, CAOIProject *ProjectPtr)
{
	CAOIPanel *PanelPtr = this;
	CAOIBoard *BoardPtr = NULL;
	const size_t BoardCount = this->GetPanelBoardCount();

	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";

	size_t i;

	::fwprintf(pfile, L"    {\n");
	::wcscpy(strTag, L"ID");
	::fwprintf(pfile, L"      \"%s\":%d,\n", strTag, PanelPtr->GetPanelIndex_Project() + 1);
	::wcscpy(strTag, L"Board_List");
	::fwprintf(pfile, L"      \"%s\":[\n", strTag);
	for (i = 0; i < BoardCount; i++) {
		BoardPtr = PanelPtr->GetPanelBoardPtr(i, false);
		if (NULL == BoardPtr) { continue; }
		if (BoardPtr->WriteBoardSpcHeader_JSON_detail(pfile, ProjectPtr) == false) { return FALSE; }
		//Prevent Trailing Comma
		if (i == BoardCount - 1) { ::fwprintf(pfile, L"\n"); }
		else { ::fwprintf(pfile, L",\n"); }
	}
	::fwprintf(pfile, L"      ]\n");
	::fwprintf(pfile, L"    }");
	return TRUE;
}

//-------------------------------------------------------------------------------------//
bool CAOIPanel::ConvertToSpcPanel(DISTRICT_ID DistrictID, TSpcPanel &SpcPanel)//轉成Spc整板圖
{
	CAOIPanel *PanelPtr = this;
	if ( NULL == PanelPtr ) { return false; }
	TREGION4D Region;
	Region = PanelPtr->GetPanelRgnStage(DistrictID);

	::memset(&SpcPanel, 0x00, sizeof(SpcPanel));
	SpcPanel.uuidPanel = PanelPtr->GetObjUuid();
	SpcPanel.uPanelIndex = PanelPtr->GetPanelIndex_Project();
	::wcscpy(SpcPanel.sPanelBarcode, PanelPtr->GetPanelBarcode());
	SpcPanel.nPanelTestResultID  = PanelPtr->GetPanelResultID();
	SpcPanel.nPanelCheckResultID = PanelPtr->GetPanelResultID();
	
	SpcPanel.fPanelRgnMinX = Region.minX;
	SpcPanel.fPanelRgnMinY = Region.minY;
	SpcPanel.fPanelRgnMaxX = Region.maxX;
	SpcPanel.fPanelRgnMaxY = Region.maxY;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::CheckPanelBePickByCad(const TPOINT2D &PickPos)//確認整板被點擊到
{	
	TREGION4D    Region = GetPanelRgnCad();
	if ( PickPos.x<Region.minX || PickPos.y<Region.minY || 
		 PickPos.x>Region.maxX || PickPos.y>Region.maxY )
	{	return false; }
	return true; 	
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::CheckPanelBePickByStage(DISTRICT_ID DistrictID, const TPOINT2D &PickPos)//確認整板被點擊到
{		
	TREGION4D    Region = GetPanelRgnStage(DistrictID);		
	if ( PickPos.x<Region.minX || PickPos.y<Region.minY || 
		 PickPos.x>Region.maxX || PickPos.y>Region.maxY )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::CheckPanelInRegionByCad(const TREGION4D &SelRgn, bool bEntireIn)//確認整板在範圍內
{
	TREGION4D    Region = GetPanelRgnCad();		
	return JetAPI::CheckRgnInRegion(Region, SelRgn, bEntireIn);
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::CheckPanelInRegionByStage(DISTRICT_ID DistrictID, const TREGION4D &SelRgn, bool bEntireIn)//確認整板在範圍內
{
	TREGION4D    Region = GetPanelRgnStage(DistrictID);		
	return JetAPI::CheckRgnInRegion(Region, SelRgn, bEntireIn);
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::LayoutPanelRegionCad()//重整整板範圍
{
	TREGION4D CadRgn=CalcPanelRgnCad();
	SetPanelRgnCad(CadRgn);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::LayoutPanelRegionStage()//重整整板範圍
{
	size_t i = 0;
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
	const size_t FdCount = GetPanelFdCount_Inline();
	const size_t MarkCount = GetPanelMarkCount_Inline();
	const size_t BarcodeCount = GetPanelBarcodeCount_Inline();
	const size_t ComponentCount = GetPanelComponentCount_Inline();		
	CMapCoordinate *MapCTSPtrDA = GetPanelMapCTSPtr(DISTRICT_ID_A);
	CMapCoordinate *MapCTSPtrDB = GetPanelMapCTSPtr(DISTRICT_ID_B);

	GetPanelRgnCad(RgnCad);
	MapCTSPtrDA->MapRegion2D(RgnCad, RgnStage_DA);
	MapCTSPtrDB->MapRegion2D(RgnCad, RgnStage_DB);
	JetAPI::AdjustRegion(RgnStage_DA, RgnStage_DA);
	JetAPI::AdjustRegion(RgnStage_DB, RgnStage_DB);
	/*
	RgnStage_DA.Limit(true);
	RgnStage_DB.Limit(true);
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
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
		MarkPtr = GetPanelMarkPtr_Inline(i);
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
		BarcodePtr = GetPanelBarcodePtr_Inline(i);
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
		pComponent = GetPanelComponentPtr_Inline(i);
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
	}*/
	SetPanelRgnStage(DISTRICT_ID_A, RgnStage_DA);
	SetPanelRgnStage(DISTRICT_ID_B, RgnStage_DB);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::LayoutPanelRegionStage(DISTRICT_ID DistrictID)//重整整板範圍	
{
	size_t         i = 0;
	TREGION4D      RgnCad;
	TREGION4D      RgnStage;
	TREGION4D      StageRegion;
	double         StagePosX=0, StagePosY=0;
	double         StageCornerPosX[4]={0};
	double         StageCornerPosY[4]={0};	
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIComponent *pComponent = NULL;
	const size_t  FdCount = GetPanelFdCount_Inline();
	const size_t  MarkCount = GetPanelMarkCount_Inline();
	const size_t  BarcodeCount = GetPanelBarcodeCount_Inline();
	const size_t  ComponentCount = GetPanelComponentCount_Inline();		
	CMapCoordinate *MapCTSPtr = GetPanelMapCTSPtr(DistrictID);	

	GetPanelRgnCad(RgnCad);
	MapCTSPtr->MapRegion2D(RgnCad, RgnStage);
	JetAPI::AdjustRegion(RgnStage, RgnStage);
	/*
	RgnStage.maxY = RgnStage.maxX = -DBL_MAX;//整板範圍-Stage
	RgnStage.minY = RgnStage.minX =  DBL_MAX;//整板範圍-Stage
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		StagePosX = FdPtr->GetFdStagePosX();
		StagePosY = FdPtr->GetFdStagePosY();		

		FdPtr->GetFdRoiStageCornerPosX(StageCornerPosX);
		FdPtr->GetFdRoiStageCornerPosY(StageCornerPosY);
		JetAPI::CornerXYToRegion(StageCornerPosX, StageCornerPosY, StageRegion);		
		
		if ( RgnStage.maxX < StageRegion.maxX ) { RgnStage.maxX = StageRegion.maxX; }
		if ( RgnStage.maxY < StageRegion.maxY ) { RgnStage.maxY = StageRegion.maxY; }
		if ( RgnStage.minX > StageRegion.minX ) { RgnStage.minX = StageRegion.minX; }
		if ( RgnStage.minY > StageRegion.minY ) { RgnStage.minY = StageRegion.minY; }		
	}

	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetPanelMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		StagePosX = MarkPtr->GetMarkStagePosX();
		StagePosY = MarkPtr->GetMarkStagePosY();		

		MarkPtr->GetMarkRoiStageCornerPosX(StageCornerPosX);
		MarkPtr->GetMarkRoiStageCornerPosY(StageCornerPosY);
		JetAPI::CornerXYToRegion(StageCornerPosX, StageCornerPosY, StageRegion);
		
		if ( RgnStage.maxX < StageRegion.maxX ) { RgnStage.maxX = StageRegion.maxX; }
		if ( RgnStage.maxY < StageRegion.maxY ) { RgnStage.maxY = StageRegion.maxY; }
		if ( RgnStage.minX > StageRegion.minX ) { RgnStage.minX = StageRegion.minX; }
		if ( RgnStage.minY > StageRegion.minY ) { RgnStage.minY = StageRegion.minY; }
	}

	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetPanelBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		StagePosX = BarcodePtr->GetBarcodeStagePosX();
		StagePosY = BarcodePtr->GetBarcodeStagePosY();		

		BarcodePtr->GetBarcodeRoiStageCornerPosX(StageCornerPosX);
		BarcodePtr->GetBarcodeRoiStageCornerPosY(StageCornerPosY);
		JetAPI::CornerXYToRegion(StageCornerPosX, StageCornerPosY, StageRegion);
		
		if ( RgnStage.maxX < StageRegion.maxX ) { RgnStage.maxX = StageRegion.maxX; }
		if ( RgnStage.maxY < StageRegion.maxY ) { RgnStage.maxY = StageRegion.maxY; }
		if ( RgnStage.minX > StageRegion.minX ) { RgnStage.minX = StageRegion.minX; }
		if ( RgnStage.minY > StageRegion.minY ) { RgnStage.minY = StageRegion.minY; }
	}

	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetPanelComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }
		if ( DistrictID != pComponent->GetComponentDistrictID() ) { continue; }
		StagePosX = pComponent->GetComponentStagePosX();
		StagePosY = pComponent->GetComponentStagePosY();		

		pComponent->GetComponentRoiStageCornerPosX(StageCornerPosX);
		pComponent->GetComponentRoiStageCornerPosY(StageCornerPosY);
		JetAPI::CornerXYToRegion(StageCornerPosX, StageCornerPosY, StageRegion);
		
		if ( RgnStage.maxX < StageRegion.maxX ) { RgnStage.maxX = StageRegion.maxX; }
		if ( RgnStage.maxY < StageRegion.maxY ) { RgnStage.maxY = StageRegion.maxY; }
		if ( RgnStage.minX > StageRegion.minX ) { RgnStage.minX = StageRegion.minX; }
		if ( RgnStage.minY > StageRegion.minY ) { RgnStage.minY = StageRegion.minY; }
	}*/
	SetPanelRgnStage(DistrictID, RgnStage);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::CalcPanelBasePlaneParam(DISTRICT_ID DistrictID)//計算整板基準面
{
	size_t  i=0, j=0;	
	RESULT_ID ResultID;
	CAOIFd *FdPtr = NULL;
	std::vector<CAOIFd*>   CalcFdList;
	const size_t FdCount = GetPanelFdCount_Inline();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdDeleted() == true ) { continue; }		
		if ( FdPtr->GetFdBoardPtr() != NULL ) { continue; }
		if ( FdPtr->GetFdDistrictID() != DistrictID ) { continue; }
		ResultID = FdPtr->GetFdResultID_AOI();
		if ( RESULT_ID_OK == ResultID )
		{	CalcFdList.push_back(FdPtr); }
	}	
	double PosX=0, PosY=0, PosZ=0, AveZ=0;		
	const size_t CalcFdCount = CalcFdList.size();
	
	TPOINT3D    Point;
	std::vector<TPOINT3D>  PtList;	
	for ( i=0; i<CalcFdCount; i++ )
	{
		FdPtr = CalcFdList[i];
		if ( NULL == FdPtr ) { continue; }

		PosX = FdPtr->GetFdStagePosX();
		PosY = FdPtr->GetFdStagePosY();
		PosZ = FdPtr->GetFdSpaceBasePlane();
		if ( fabs(PosZ) < 0.01 )//沒有找到
		{	continue; }
		/*
		if ( 0 == i ) 
		{	PosZ = 902;	}
		else
		{	PosZ = 1339;	}		
		FdPtr->SetFdSpaceBasePlane(PosZ);
		*/
		Point.x = PosX;
		Point.y = PosY;
		Point.z = PosZ;
		PtList.push_back(Point);
	}

	double nX=0, nY=0, nZ=0, k=0;
	size_t PtCount=PtList.size();		
	if ( 0 == PtCount )
	{	nX = nY = nZ = 0; }
	else if ( 1 == PtCount ) 
	{	nZ = PtList[0].z;	}
	else
	{
		if ( 2 == PtCount ) 
		{	//因軌道為Y方向, 同Y值使用相同高度
			Point.x=PtList[1].x;
			Point.y=PtList[0].y;
			Point.z=PtList[0].z;
			//JetAPI::BilinearInterpolation(PtList, Point.x, Point.y, Point.z);		
			PtList.push_back(Point);

			Point.x=PtList[0].x;
			Point.y=PtList[1].y;
			Point.z=PtList[1].z;
			//JetAPI::BilinearInterpolation(PtList, Point.x, Point.y, Point.z);		
			PtList.push_back(Point);
		}
		PtCount=PtList.size();
		if ( JetAPI::CalcFittingPlane(PtList, nX, nY, nZ) == false )
		{
			nX = nY = nZ = 0;
			PtCount=PtList.size();	
			for ( i=0; i<PtCount; i++ )
			{	nZ += PtList[i].z;	}
			nZ /= PtCount;
		}

		double tX=0, tY=0, tZ=0;
		JetAPI::CalcFittingPlane(PtList, tX, tY, tZ);//結果相同

		double Err=0.0;
		bool bVerifyParam=true;
		if ( true==bVerifyParam )
		{
			for ( i=0; i<PtCount; i++ )
			{
				PosX = PtList[i].x;
				PosY = PtList[i].y;
				PosZ = PtList[i].z;
				k = (nX*PosX) + (nY*PosY) + nZ;
				Err += fabs(k-PosZ);
			}
			Err /= PtCount;
			Err = Err;
		}
	}	
	SetPanelBasePlaneParam(DistrictID, nX, nY, nZ);	

	CAOIField   *FieldPtr = NULL;
	const size_t FieldCount = GetPanelFieldCount_Inline();
	//Field in Panel List
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetPanelFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		if ( FieldPtr->GetFieldDistrictID() != DistrictID ) { continue; }
		PosX = FieldPtr->GetFieldStagePosX();
		PosY = FieldPtr->GetFieldStagePosY();
		PosZ = (nX*PosX) + (nY*PosY) + nZ;
		FieldPtr->SetFieldPanelBasePlane(PosZ);
	}

	//Field in Board List
	size_t       BoardFieldCount=0;
	CAOIBoard   *BoardPtr = NULL;
	const size_t BoardCount = GetPanelBoardCount_Inline();
	for ( j=0; j<BoardCount; j++  )
	{
		BoardPtr = GetPanelBoardPtr_Inline(j);
		if ( NULL == BoardPtr ) { continue; }
		BoardFieldCount = BoardPtr->GetBoardFieldCount();
		for ( i=0; i<BoardFieldCount; i++ )
		{
			FieldPtr = BoardPtr->GetBoardFieldPtr(i, false);
			if ( NULL == FieldPtr ) { continue; }
			if ( FieldPtr->GetFieldDistrictID() != DistrictID ) { continue; }
			PosX = FieldPtr->GetFieldStagePosX();
			PosY = FieldPtr->GetFieldStagePosY();
			PosZ = (nX*PosX) + (nY*PosY) + nZ;
			FieldPtr->SetFieldPanelBasePlane(PosZ);
		}
	}

	CAOIModel  *ModelPtr = NULL;
	//Fd List	
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }		
		if ( FdPtr->GetFdDistrictID() != DistrictID ) { continue; }
		PosX = FdPtr->GetFdStagePosX();
		PosY = FdPtr->GetFdStagePosY();		
		PosZ = (nX*PosX) + (nY*PosY) + nZ;
		FdPtr->SetFdPanelBasePlane(PosZ);		
	}

	//Mark List	
	CAOIMark    *MarkPtr = NULL;
	const size_t MarkCount = GetPanelMarkCount_Inline();
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetPanelMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }		
		if ( MarkPtr->GetMarkDistrictID() != DistrictID ) { continue; }		
		PosX = MarkPtr->GetMarkStagePosX();
		PosY = MarkPtr->GetMarkStagePosY();		
		PosZ = (nX*PosX) + (nY*PosY) + nZ;
		MarkPtr->SetMarkPanelBasePlane(PosZ);		
	}

	//Barcode List	
	CAOIBarcode *BarcodePtr = NULL;
	const size_t BarcodeCount = GetPanelBarcodeCount_Inline();
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetPanelBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }		
		if ( BarcodePtr->GetBarcodeDistrictID() != DistrictID ) { continue; }		
		PosX = BarcodePtr->GetBarcodeStagePosX();
		PosY = BarcodePtr->GetBarcodeStagePosY();		
		PosZ = (nX*PosX) + (nY*PosY) + nZ;
		BarcodePtr->SetBarcodePanelBasePlane(PosZ);		
	}

	//Component List
	CAOIComponent *ComponentPtr = NULL;
	const size_t   ComponentCount = GetPanelComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		//if ( i > (ComponentCount-3) )
		//{	i = i; }
		ComponentPtr = GetPanelComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }		
		if ( ComponentPtr->GetComponentDistrictID() != DistrictID ) { continue; }		
		PosX = ComponentPtr->GetComponentStagePosX();
		PosY = ComponentPtr->GetComponentStagePosY();
		PosZ = (nX*PosX) + (nY*PosY) + nZ;
		ComponentPtr->SetComponentPanelBasePlane(PosZ);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::SetPanelBasePlaneParam(DISTRICT_ID DistrictID, double nX, double nY, double nZ)//設定整板基準面向量
{
	if ( DISTRICT_ID_B == DistrictID )
	{
		m_PanelBasePlaneNormal_DB.x  = nX;
		m_PanelBasePlaneNormal_DB.y  = nY;
		m_PanelBasePlaneNormal_DB.z  = nZ;
	}
	else
	{
		m_PanelBasePlaneNormal_DA.x  = nX;
		m_PanelBasePlaneNormal_DA.y  = nY;
		m_PanelBasePlaneNormal_DA.z  = nZ;
	}	
}
//-------------------------------------------------------------------------------------//
void CAOIPanel::GetPanelBasePlaneParam(DISTRICT_ID DistrictID, double &nX, double &nY, double &nZ) const//取得整板基準面向量
{
	if ( DISTRICT_ID_B == DistrictID )
	{
		nX = m_PanelBasePlaneNormal_DB.x;
		nY = m_PanelBasePlaneNormal_DB.y;
		nZ = m_PanelBasePlaneNormal_DB.z;
	}
	else
	{
		nX = m_PanelBasePlaneNormal_DA.x;
		nY = m_PanelBasePlaneNormal_DA.y;
		nZ = m_PanelBasePlaneNormal_DA.z;
	}
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::BuildPanelDefaultMap(DISTRICT_ID DistrictID)//建立整板基本座標轉換參數
{	
	double CadPosX[4]={0};
	double CadPosY[4]={0};
	double StagePosX[4]={0};
	double StagePosY[4]={0};	
	TREGION4D PanelRgnCad   = GetPanelRgnCad();
	TREGION4D PanelRgnStage = GetPanelRgnStage(DistrictID);
	
	CadPosX[0] = PanelRgnCad.GetCpX();
	CadPosY[0] = PanelRgnCad.GetCpY();
	StagePosX[0] = PanelRgnStage.GetCpX();
	StagePosY[0] = PanelRgnStage.GetCpY();

	if ( DISTRICT_ID_A == DistrictID )
	{
		m_PanelMapCTS.CalcMatrix2D(CadPosX, CadPosY, StagePosX, StagePosY, 1);
		m_PanelMapSTC.CalcMatrix2D(StagePosX, StagePosY, CadPosX, CadPosY, 1);
	}
	if ( DISTRICT_ID_B == DistrictID )
	{
		m_PanelMapCTS_DB.CalcMatrix2D(CadPosX, CadPosY, StagePosX, StagePosY, 1);
		m_PanelMapSTC_DB.CalcMatrix2D(StagePosX, StagePosY, CadPosX, CadPosY, 1);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CMapCoordinate* CAOIPanel::GetPanelMapCTSPtr(DISTRICT_ID DistrictID)//整板的座標轉換-Cad to Stage
{
	if ( DISTRICT_ID_A == DistrictID ) 
	{	return &m_PanelMapCTS;	}
	if ( DISTRICT_ID_B == DistrictID ) 
	{	return &m_PanelMapCTS_DB;	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
CMapCoordinate* CAOIPanel::GetPanelMapSTCPtr(DISTRICT_ID DistrictID)//整板的座標轉換-Stage to Cad
{
	if ( DISTRICT_ID_A == DistrictID ) 
	{	return &m_PanelMapSTC;	}
	if ( DISTRICT_ID_B == DistrictID ) 
	{	return &m_PanelMapSTC_DB;	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::GetPanelMapCTS(DISTRICT_ID DistrictID, CMapCoordinate &Map)
{
	if ( DISTRICT_ID_A == DistrictID ) 
	{	
		Map = m_PanelMapCTS; 
		return true;
	}
	if ( DISTRICT_ID_B == DistrictID ) 
	{	
		Map = m_PanelMapCTS_DB; 
		return true;
	}	
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::GetPanelMapSTC(DISTRICT_ID DistrictID, CMapCoordinate &Map)
{
	if ( DISTRICT_ID_A == DistrictID ) 
	{	
		Map = m_PanelMapSTC; 
		return true;
	}
	if ( DISTRICT_ID_B == DistrictID ) 
	{	
		Map = m_PanelMapSTC_DB; 
		return true;
	}	
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::CheckPanelMapCoordinate(DISTRICT_ID DistrictID, double dChkRatio)//確認整板的座標轉換機制
{	
	size_t i = 0;	
	CString  str;
	CMapCoordinate MapCTS;
	CMapCoordinate MapSTC;
	TPOINT2D CadPos1, StagePos1;
	TPOINT2D CadPos2, StagePos2;
	TPOINT2D CadPosDif, StagePosDif;	
	const double Precision = DBL_PRECISION*dChkRatio;
	CAOIComponent *pComponent = NULL;
	const size_t ComponentCount = GetPanelComponentCount_Inline();			

	if ( GetPanelMapCTS(DistrictID, MapCTS) == false ) { return false; }
	if ( GetPanelMapSTC(DistrictID, MapSTC) == false ) { return false; }

	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetPanelComponentPtr_Inline(i);
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
		str.Format(_T("Error, Panel Coordinate Transformation Check Exception\nCad(%.6f, %.6f), Stage(%.6f, %.6f) um"), CadPosDif.x, CadPosDif.y, StagePosDif.x, StagePosDif.y);
		JetAPI::ShowMessageBox(str);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CAOIPanel::CalcPanelMapParam()//計算整板座標轉換參數
{
	if ( CalcPanelMapParam(DISTRICT_ID_A) == false ) 
	{	return false; }
	if ( GetPanelMultiDistrictMode() )
	{
		if ( CalcPanelMapParam(DISTRICT_ID_B) == false ) 
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::CalcPanelMapParam(DISTRICT_ID DistrictID)//計算整板座標轉換參數
{
	size_t         i=0;
	double         CadX=0, CadY=0;
	double         StageX=0, StageY=0;
	CAOIFd        *FdPtr = NULL;
	LANE_ID        FdLaneID;
	RESULT_ID      FdResultID;
	std::vector<CAOIFd*> PanelFdPtrList;
	std::vector<CAOIFd*> PanelFdPtrListAll;
	const size_t MaxFdCount = PANEL_MAX_FD_COUNT;
	const size_t   FdCount = GetPanelFdCount_Inline();
	double CadPosX[MaxFdCount]={0};
	double CadPosY[MaxFdCount]={0};
	double StagePosX[MaxFdCount]={0};
	double StagePosY[MaxFdCount]={0};

	//Panel Fiducial 
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }		
		if ( FdPtr->GetFdBoardPtr() != NULL ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		//確認Offline有讀到定位點資料
		FdResultID = FdPtr->GetFdResultID_AOI();
		switch ( FdResultID )
		{
		case RESULT_ID_OK:
		case RESULT_ID_SKIP:
			PanelFdPtrList.push_back(FdPtr);
			break;
		}
		PanelFdPtrListAll.push_back(FdPtr);
	}
	//如果沒有可用的定位點, 則以教導位置來計算
	if ( 0 == PanelFdPtrList.size() )
	{	PanelFdPtrList = PanelFdPtrListAll;	}
	const size_t PanelFdCount = PanelFdPtrList.size();
	const int UsedFdCount=MIN(MaxFdCount, PanelFdCount);
	if ( 0 == UsedFdCount )
	{	return true; }
	for ( i=0; i<UsedFdCount; i++ )
	{
		FdPtr = PanelFdPtrList[i];
		FdLaneID = FdPtr->GetFdLaneID();
		CadPosX[i] = FdPtr->GetFdCadPosX();
		CadPosY[i] = FdPtr->GetFdCadPosY();
		FdResultID = FdPtr->GetFdResultID_AOI();
		switch ( FdResultID )
		{
		case RESULT_ID_OK:
			StagePosX[i] = FdPtr->GetFdStagePosX();
			StagePosY[i] = FdPtr->GetFdStagePosY();
			break;
		default:
			StagePosX[i] = FdPtr->GetFdTeachStagePosX();
			StagePosY[i] = FdPtr->GetFdTeachStagePosY();
			if ( LANE_ID_B == FdLaneID )
			{	AOIDataCollect.MapStagePosLaneAtoB(StagePosX[i], StagePosY[i]);	}
			break;
		}
	}

#ifdef BURNING_TEST_FD_USE	
	double TempR[MaxFdCount];//直線
	double TempA[MaxFdCount];//角度
	for ( i=0; i<UsedFdCount; i++ )
	{
		if ( 0 == i )
		{	TempA[i] = TempR[i] = 0.0;	}
		else
		{
			TPOINT2D pt1(StagePosX[0], StagePosY[0]);
			TPOINT2D pt2(StagePosX[i], StagePosY[i]);			
			TempA[i] = JetAPI::CalcAngle2D(pt1, pt2);
			TempR[i] = JetAPI::CalcDistance(pt1, pt2);			
		}
	}

	srand(time(NULL));
	const int MaxOffseX=1000;//1000um
	const int MaxOffseY=1000;//1000um
	const int MaxSkew=100;//1000um
	const double RandX=rand()%MaxOffseX;	::Sleep(0);
	const double RandY=rand()%MaxOffseY;	::Sleep(0);
	const double RandA=rand()%MaxSkew;		::Sleep(0);
	StagePosX[0] += RandX;
	StagePosY[0] += RandY;
	for ( i=1; i<UsedFdCount; i++ )
	{
		const double RandX2=rand()%100;	
		const double RandY2=rand()%100;
		const double DegA=TempA[i]+(RandA/MaxSkew);
		const double RadA=DegA*DEG_TO_RAD_DBL;
		const double StageNewX=StagePosX[0]+(TempR[i]*cos(RadA));
		const double StageNewY=StagePosY[0]+(TempR[i]*sin(RadA));
		StagePosX[i] = StageNewX+RandX2;
		StagePosY[i] = StageNewY+RandY2;
		::Sleep(0);
	}
#endif//BURNING_TEST_FD_USE

	if ( DISTRICT_ID_B == DistrictID )
	{
		m_PanelMapCTS_DB.Identity();
		m_PanelMapSTC_DB.Identity();
		m_PanelMapCTS_DB.CalcMatrix2D(CadPosX, CadPosY, StagePosX, StagePosY, UsedFdCount);
		m_PanelMapSTC_DB.CalcMatrix2D(StagePosX, StagePosY, CadPosX, CadPosY, UsedFdCount);	
	}
	else
	{
		m_PanelMapCTS.Identity();
		m_PanelMapSTC.Identity();
		m_PanelMapCTS.CalcMatrix2D(CadPosX, CadPosY, StagePosX, StagePosY, UsedFdCount);
		m_PanelMapSTC.CalcMatrix2D(StagePosX, StagePosY, CadPosX, CadPosY, UsedFdCount);	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::CalcPanelStagePosition()//計算整板座標
{	
	if ( CalcPanelStagePosition(DISTRICT_ID_A) == false ) 
	{	return false; }	
	if ( GetPanelMultiDistrictMode() )
	{
		if ( CalcPanelStagePosition(DISTRICT_ID_B) == false ) 
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::CalcPanelStagePosition(DISTRICT_ID DistrictID)//計算整板座標
{
	size_t         i=0, j=0, k=0;
	size_t         SubRgnCount = 0;
	size_t         WindowCount = 0;
	double         CadX=0, CadY=0;
	double         StageX=0, StageY=0;	
	CMapCoordinate Map;
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIRgn       *SubRgnPtr = NULL;
	CAOIBoard     *BoardPtr = NULL;
	CAOIField     *FieldPtr = NULL;
	CAOIWindow    *WindowPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;		
	const size_t   FdCount = GetPanelFdCount_Inline();	
	const size_t   MarkCount = GetPanelMarkCount_Inline();
	const size_t   BoardCount = GetPanelBoardCount_Inline();
	const size_t   FieldCount = GetPanelFieldCount_Inline();
	const size_t   BarcodeCount = GetPanelBarcodeCount_Inline();
	const size_t   ComponentCount = GetPanelComponentCount_Inline();	
	if ( GetPanelMapCTS(DistrictID, Map) == false ) { return false; }	

	//Field
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetPanelFieldPtr_Inline(i);
		if ( NULL == FieldPtr ) { continue; }
		if ( DistrictID != FieldPtr->GetFieldDistrictID() ) { continue; }
		FieldPtr->MapFieldCadToStagePos(Map);
	}

	//Board Fiducial 
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdPanelPtr() != this ) {	continue;	}
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		FdPtr->MapFdCadToStagePos(Map);
		if ( FdPtr->CheckFdIsBoardFd() == true )
		{	FdPtr->SetFdTeachStagePos(FdPtr->GetFdStagePos());	}
	}

	//Board Field
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetPanelBoardPtr(i, false);
		if ( NULL == BoardPtr ) { continue; }
		size_t BoardFieldCount=BoardPtr->GetBoardFieldCount();
		for ( j=0; j<BoardFieldCount; j++ )
		{
			FieldPtr = BoardPtr->GetBoardFieldPtr(j, false);
			if ( NULL == FieldPtr ) { continue; }
			if ( DistrictID != FieldPtr->GetFieldDistrictID() ) { continue; }
			FieldPtr->MapFieldCadToStagePos(Map);
		}
	}

	//Mark
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetPanelMarkPtr_Inline(i);
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
		BarcodePtr = GetPanelBarcodePtr_Inline(i);
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
		ComponentPtr = GetPanelComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		//ComponentPtr->SetComponentNeedToUpdate(true);//測試用
		//if ( ComponentPtr->GetComponentNeedToUpdate() == false ) { continue; }
		//ComponentPtr->ClearComponentSubRgnList();
		ComponentPtr->MapComponentCadToStagePos(Map);		

		FieldPtr = ComponentPtr->GetComponentSelfFieldPtr();
		if ( NULL != FieldPtr )
		{	FieldPtr->MapFieldCadToStagePos(Map);	}

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

	if ( CalcPanelBasePlaneParam(DistrictID) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::CalcPanelCadResPosition_All()//計算整板Cad結果座標
{
	if ( CalcPanelCadResPosition(DISTRICT_ID_A) == false ) 
	{	return false; }	
	if ( GetPanelMultiDistrictMode() )
	{
		if ( CalcPanelCadResPosition(DISTRICT_ID_B) == false ) 
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::CalcPanelCadResPosition(DISTRICT_ID DistrictID)//計算整板Cad結果座標
{
	size_t         i=0, j=0, k=0;
	size_t         SubRgnCount = 0;
	size_t         WindowCount = 0;
	double         CadX=0, CadY=0;
	double         StageX=0, StageY=0;	
	CMapCoordinate Map;
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;	
	CAOIBoard     *BoardPtr = NULL;	
	CAOIComponent *ComponentPtr = NULL;		
	const size_t   FdCount = GetPanelFdCount_Inline();	
	const size_t   MarkCount = GetPanelMarkCount_Inline();
	const size_t   BoardCount = GetPanelBoardCount_Inline();	
	const size_t   BarcodeCount = GetPanelBarcodeCount_Inline();
	const size_t   ComponentCount = GetPanelComponentCount_Inline();	
	if ( GetPanelMapSTC(DistrictID, Map) == false ) { return false; }	
	bool bMapOthers=false;//是否更新至其他物件上, 目前先使用在零件上

	//Field
if ( true == bMapOthers )
{
	//Board Fiducial 
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( FdPtr->GetFdPanelPtr() != this ) {	continue;	}
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		if ( FdPtr->GetFdNeedToCalculate() == false ) { continue; }
	}

	//Mark
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetPanelMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		if ( MarkPtr->GetMarkNeedToCalculate() == false ) { continue; }
	}

	//Barcode
	for ( i=0; i<BarcodeCount; i++ )
	{
		BarcodePtr = GetPanelBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }
		if ( BarcodePtr->GetBarcodeNeedToCalculate() == false ) { continue; }
	}
}
	//Component 
	for ( i=0; i<ComponentCount; i++ )
	{
		ComponentPtr = GetPanelComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		if ( ComponentPtr->GetComponentNeedToCalculate() == false ) { continue; }		
		ComponentPtr->MapComponentStageResultToCadPos(Map);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::CalcPanelSkew(DISTRICT_ID DistrictID)
{
	size_t         i = 0;
	double         CadX = 0, CadY = 0;
	double         StageX = 0, StageY = 0;
	CAOIFd        *FdPtr = NULL;
	LANE_ID        FdLaneID;
	RESULT_ID      FdResultID;
	std::vector<CAOIFd*> PanelFdPtrList;
	std::vector<CAOIFd*> PanelFdPtrListAll;
	const size_t MaxFdCount = PANEL_MAX_FD_COUNT;
	const size_t   FdCount = GetPanelFdCount_Inline();
	double CadPosX[MaxFdCount] = { 0 };
	double CadPosY[MaxFdCount] = { 0 };
	double StagePosX[MaxFdCount] = { 0 };
	double StagePosY[MaxFdCount] = { 0 };
	double TeachStagePosX[MaxFdCount] = { 0 };
	double TeachStagePosY[MaxFdCount] = { 0 };
	CMapCoordinate             PanelMapCTS;
	CMapCoordinate             TeachPanelMapCTS;
	//Panel Fiducial 
	for (i = 0; i<FdCount; i++)
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if (NULL == FdPtr) { continue; }
		if (FdPtr->GetFdBoardPtr() != NULL) { continue; }
		if (DistrictID != FdPtr->GetFdDistrictID()) { continue; }
		//確認Offline有讀到定位點資料
		FdResultID = FdPtr->GetFdResultID_AOI();
		switch (FdResultID)
		{
		case RESULT_ID_OK:
		case RESULT_ID_SKIP:
			PanelFdPtrList.push_back(FdPtr);
			break;
		}
		PanelFdPtrListAll.push_back(FdPtr);
	}
	//如果沒有可用的定位點, 則以教導位置來計算
	if (0 == PanelFdPtrList.size())
	{
		PanelFdPtrList = PanelFdPtrListAll;
	}
	const size_t PanelFdCount = PanelFdPtrList.size();
	const int UsedFdCount = MIN(MaxFdCount, PanelFdCount);
	if (0 == UsedFdCount)
	{
		return true;
	}
	for (i = 0; i<UsedFdCount; i++)
	{
		FdPtr = PanelFdPtrList[i];
		FdLaneID = FdPtr->GetFdLaneID();
		CadPosX[i] = FdPtr->GetFdCadPosX();
		CadPosY[i] = FdPtr->GetFdCadPosY();
		FdResultID = FdPtr->GetFdResultID_AOI();
		switch (FdResultID)
		{
		case RESULT_ID_OK:
			StagePosX[i] = FdPtr->GetFdStagePosX();
			StagePosY[i] = FdPtr->GetFdStagePosY();
			break;
		default:
			StagePosX[i] = FdPtr->GetFdTeachStagePosX();
			StagePosY[i] = FdPtr->GetFdTeachStagePosY();
			if (LANE_ID_B == FdLaneID)
			{
				AOIDataCollect.MapStagePosLaneAtoB(StagePosX[i], StagePosY[i]);
			}
			break;
		}
		TeachStagePosX[i] = FdPtr->GetFdTeachStagePosX();
		TeachStagePosY[i] = FdPtr->GetFdTeachStagePosY();
	}
	PanelMapCTS.Identity();
	TeachPanelMapCTS.Identity();
	PanelMapCTS.CalcMatrix2D(CadPosX, CadPosY, StagePosX, StagePosY, UsedFdCount);
	TeachPanelMapCTS.CalcMatrix2D(CadPosX, CadPosY, TeachStagePosX, TeachStagePosY, UsedFdCount);
	//Calc Mapping function angle
	double MapPara[9], TeachMapPara[9], Angle, TeachAngle, Skew;
	PanelMapCTS.GetMatrix2D(MapPara[0], MapPara[1], MapPara[2], MapPara[3], MapPara[4], MapPara[5], MapPara[6], MapPara[7], MapPara[8]);
	TeachPanelMapCTS.GetMatrix2D(TeachMapPara[0], TeachMapPara[1], TeachMapPara[2], TeachMapPara[3], TeachMapPara[4], TeachMapPara[5], TeachMapPara[6], TeachMapPara[7], TeachMapPara[8]);
	Angle = atan2(MapPara[3], MapPara[0]);
	TeachAngle = atan2(TeachMapPara[3], TeachMapPara[0]);
	Skew = (Angle - TeachAngle)*RAD_TO_DEG_DBL;
	SetPanelSkew(Skew);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::AssignPanelMapParamToBoards()//將整板的座標轉換轉至各自單板內
{
	if ( AssignPanelMapParamToBoards(DISTRICT_ID_A) == false )
	{	return false; }
	if ( GetPanelMultiDistrictMode() )
	{
		if ( AssignPanelMapParamToBoards(DISTRICT_ID_B) == false )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::AssignPanelMapParamToBoards(DISTRICT_ID DistrictID)//將整板的座標轉換轉至各自單板內
{
	size_t          i=0, j=0, k=0;	
	CAOIBoard      *BoardPtr = NULL;
	CMapCoordinate *MapCTSPtr=GetPanelMapCTSPtr(DistrictID);
	CMapCoordinate *MapSTCPtr=GetPanelMapSTCPtr(DistrictID);
	const size_t    BoardCount = GetPanelBoardCount_Inline();
	if ( NULL==MapCTSPtr || NULL==MapSTCPtr ) { return false; }
	for ( i=0; i<BoardCount; i++ )
	{
		BoardPtr = GetPanelBoardPtr_Inline(i);
		if ( NULL == BoardPtr ) { continue; }		
		BoardPtr->SetBoardMapCTS(DistrictID, MapCTSPtr);
		BoardPtr->SetBoardMapSTC(DistrictID, MapSTCPtr);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::LayoutPanelRegion()//重整整板範圍
{
	LayoutPanelRegionCad();
	LayoutPanelRegionStage();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::LayoutPanelRegion(DISTRICT_ID DistrictID)//重整整板範圍	
{
	LayoutPanelRegionCad();
	LayoutPanelRegionStage(DistrictID);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::SetPanelCadPos(double PanelCadPosX, double PanelCadPosY)//設定整板的機台中心
{
	if ( SetPanelCadPos(PanelCadPosX, PanelCadPosY, DISTRICT_ID_A) == false )
	{	return false; }
	if ( SetPanelCadPos(PanelCadPosX, PanelCadPosY, DISTRICT_ID_B) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::SetPanelCadPos(double PanelCadPosX, double PanelCadPosY, DISTRICT_ID DistrictID)//設定整板的機台中心
{
	return true;
	size_t i=0, j=0;
	size_t WindowCount=0;
	TPOINT2D StageOffset, CadOffset;
	double NewCadX=0, NewCadY=0;
	double StagePosX=0, StagePosY=0;	

	CAOIBoard     *pBoard = NULL;
	CAOIWindow    *pWindow=NULL;
	CAOIComponent *pComponent = NULL;		

	const size_t FdCount = GetPanelFdCount_Inline();
	const size_t BoardCount = GetPanelBoardCount_Inline();
	const size_t ComponentCount = GetPanelComponentCount_Inline();

	//Find Panel StageXY			
	LayoutPanelRegionStage();
	TREGION4D    PanelRgnStage = GetPanelRgnStage(DistrictID);
	const double PanelStagePosX = (PanelRgnStage.minX+PanelRgnStage.maxX)*0.5;
	const double PanelStagePosY = (PanelRgnStage.minY+PanelRgnStage.maxY)*0.5;		
	
	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetPanelComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }
		if ( DistrictID != pComponent->GetComponentDistrictID() ) { continue; }

		StagePosX = pComponent->GetComponentStagePosX();
		StagePosY = pComponent->GetComponentStagePosY();
		StageOffset.x = StagePosX-PanelStagePosX;		
		StageOffset.y = StagePosY-PanelStagePosY;
		AOIDataCollect.MapStageOffsetPtToCad(StageOffset, CadOffset);
		NewCadX = CadOffset.x+PanelCadPosX;
		NewCadY = CadOffset.y+PanelCadPosY;

		pComponent->SetComponentCadPosX(NewCadX);
		pComponent->SetComponentCadPosY(NewCadY);
		pComponent->CalcComponentCadCornerPos();
		pComponent->LayoutComponentStageCornerPos();		
		pComponent->UpdateComponentParamToModel(false);

		WindowCount = pComponent->GetComponentWindowCount();
		for ( j=0; j<WindowCount; j++ )
		{
			pWindow = pComponent->GetComponentWindowPtr(j, false);
			if ( NULL == pWindow ) { continue;; }
			/*
			StagePosX = pWindow->GetWindowStagePosX();
			StagePosY = pWindow->GetWindowStagePosY();
			StageOffset.x = StagePosX-PanelStagePosX;		
			StageOffset.y = StagePosY-PanelStagePosY;
			AOIDataCollect.MapStageOffsetPtToCad(StageOffset, CadOffset);
			NewCadX = CadOffset.x+PanelCadPosX;
			NewCadY = CadOffset.y+PanelCadPosY;

			pWindow->SetWindowCadPosX(NewCadX);
			pWindow->SetWindowCadPosX(NewCadY);
			*/
		}
	}		
	LayoutPanelRegionCad();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::SetPanelStagePos(double PanelStagePosX, double PanelStagePosY)//設定整板的機台中心
{
	if ( SetPanelStagePos(PanelStagePosX, PanelStagePosY, DISTRICT_ID_A) == false ) 
	{	return false; }
	if ( SetPanelStagePos(PanelStagePosX, PanelStagePosY, DISTRICT_ID_B) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::SetPanelStagePos(double PanelStagePosX, double PanelStagePosY, DISTRICT_ID DistrictID)//設定整板的機台中心
{	
	CMapCoordinate *CTSPtr = GetPanelMapCTSPtr(DistrictID);//整板的座標轉換-Cad to Stage
	CMapCoordinate *STCPtr = GetPanelMapSTCPtr(DistrictID);//整板的座標轉換-Stage to Cad
	if ( NULL==CTSPtr || NULL==STCPtr )
	{	return false; }

	//Find Panel CADXY		
	LayoutPanelRegionCad();	
	//LayoutPanelRegionStage();

	TREGION4D PanelRgnCad   = GetPanelRgnCad();	
	const double PanelCadPosX = PanelRgnCad.GetCpX();
	const double PanelCadPosY = PanelRgnCad.GetCpY();
	
	double CadPosX[4] = {0,0,0,0};
	double CadPosY[4] = {0,0,0,0};
	double StagePosX[4] = {0,0,0,0};
	double StagePosY[4] = {0,0,0,0};
	TPOINT2D StageOffset, CadOffset;	

	CadPosX[0] = PanelCadPosX;
	CadPosY[0] = PanelCadPosY;
	StagePosX[0] = PanelStagePosX;
	StagePosY[0] = PanelStagePosY;
	CTSPtr->CalcMatrix2D(CadPosX, CadPosY, StagePosX, StagePosY, 1);
	STCPtr->CalcMatrix2D(StagePosX, StagePosY, CadPosX, CadPosY, 1);
	AssignPanelMapParamToBoards(DistrictID);

	size_t i=0, j=0;
	size_t WindowCount=0;	
	double CADPosX=0, CADPosY=0;	
	double NewStageX=0, NewStageY=0;

	CAOIFd        *pFd = NULL;
	CAOIMark      *pMark = NULL;
	CAOIBoard     *pBoard = NULL;
	CAOIWindow    *pWindow= NULL;
	CAOIBarcode   *pBarcode = NULL;
	CAOIComponent *pComponent = NULL;		
	const size_t   FdCount = GetPanelFdCount_Inline();
	const size_t   MarkCount = GetPanelMarkCount_Inline();
	const size_t   BoardCount = GetPanelBoardCount_Inline();
	const size_t   BarcodeCount = GetPanelBarcodeCount_Inline();
	const size_t   ComponentCount = GetPanelComponentCount_Inline();
	
	//修正整板的定位點位置	
	for ( i=0; i<FdCount; i++ )
	{
		pFd = GetPanelFdPtr_Inline(i);
		if ( NULL == pFd ) { continue; }		
		if ( DistrictID != pFd->GetFdDistrictID() ) { continue; }

		CADPosX = pFd->GetFdCadPosX();
		CADPosY = pFd->GetFdCadPosY();
		CadOffset.x = CADPosX-PanelCadPosX;		
		CadOffset.y = CADPosY-PanelCadPosY;
		AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);
		NewStageX = StageOffset.x+PanelStagePosX;
		NewStageY = StageOffset.y+PanelStagePosY;

		pFd->SetFdStagePosX(NewStageX);
		pFd->SetFdStagePosY(NewStageY);
		pFd->LayoutFdStageCornerPos();		
		pFd->UpdateFdParamToModel();
	}

	//修正整板的特徵點位置	
	for ( i=0; i<MarkCount; i++ )
	{
		pMark = GetPanelMarkPtr_Inline(i);
		if ( NULL == pMark ) { continue; }		
		if ( DistrictID != pMark->GetMarkDistrictID() ) { continue; }

		CADPosX = pMark->GetMarkCadPosX();
		CADPosY = pMark->GetMarkCadPosY();
		CadOffset.x = CADPosX-PanelCadPosX;		
		CadOffset.y = CADPosY-PanelCadPosY;
		AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);
		NewStageX = StageOffset.x+PanelStagePosX;
		NewStageY = StageOffset.y+PanelStagePosY;

		pMark->SetMarkStagePosX(NewStageX);
		pMark->SetMarkStagePosY(NewStageY);
		pMark->LayoutMarkStageCornerPos();		
		pMark->UpdateMarkParamToModel();
	}

	//修正整板的條碼位置	
	for ( i=0; i<BarcodeCount; i++ )
	{
		pBarcode = GetPanelBarcodePtr_Inline(i);
		if ( NULL == pBarcode ) { continue; }		
		if ( DistrictID != pBarcode->GetBarcodeDistrictID() ) { continue; }

		CADPosX = pBarcode->GetBarcodeCadPosX();
		CADPosY = pBarcode->GetBarcodeCadPosY();
		CadOffset.x = CADPosX-PanelCadPosX;		
		CadOffset.y = CADPosY-PanelCadPosY;
		AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);
		NewStageX = StageOffset.x+PanelStagePosX;
		NewStageY = StageOffset.y+PanelStagePosY;

		pBarcode->SetBarcodeStagePosX(NewStageX);
		pBarcode->SetBarcodeStagePosY(NewStageY);
		pBarcode->LayoutBarcodeStageCornerPos();		
		pBarcode->UpdateBarcodeParamToModel();
	}

	//修正整板的定位點位置	
	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetPanelComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }		
		if ( DistrictID != pComponent->GetComponentDistrictID() ) { continue; }

		CADPosX = pComponent->GetComponentCadPosX();
		CADPosY = pComponent->GetComponentCadPosY();
		CadOffset.x = CADPosX-PanelCadPosX;		
		CadOffset.y = CADPosY-PanelCadPosY;
		AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);
		NewStageX = StageOffset.x+PanelStagePosX;
		NewStageY = StageOffset.y+PanelStagePosY;

		pComponent->SetComponentStagePosX(NewStageX);
		pComponent->SetComponentStagePosY(NewStageY);
		pComponent->LayoutComponentStageCornerPos();		
		pComponent->UpdateComponentParamToModel(false);

		WindowCount = pComponent->GetComponentWindowCount();
		for ( j=0; j<WindowCount; j++ )
		{
			pWindow = pComponent->GetComponentWindowPtr(j, false);
			if ( NULL == pWindow ) { continue;; }
			/*
			CADPosX = pWindow->GetCADPositionX_W();
			CADPosY = pWindow->GetCADPositionY_W();			
			CadOffset.x = CADPosX-PanelCadPosX;		
			CadOffset.y = CADPosY-PanelCadPosY;
			AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);
			NewStageX = StageOffset.x+PanelStagePosX;
			NewStageY = StageOffset.y+PanelStagePosY;
			pWindow->SetStagePositionX_W(NewStageX);
			pWindow->SetStagePositionY_W(NewStageY);
			*/
		}
	}

	//v1.01.01.057
	for ( i=0; i<BoardCount; i++ )
	{
		pBoard = GetPanelBoardPtr(i, false);
		if ( NULL == pBoard ) { continue; }
		pBoard->LayoutBoardRegionStage(DistrictID);
	}
	LayoutPanelRegionStage(DistrictID);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::GetPanelCornerComponent(CAOIComponent *&Cp1, CAOIComponent *&Cp2, CAOIComponent *&Cp3, CAOIComponent *&Cp4, DISTRICT_ID DistrictID)//取得整板最靠近四端點的零件
{
	size_t i=0, j=0;
	double CornerLMin[4]={0};
	double CadPosX=0, CadPosY=0;
	double CornerX=0, CornerY=0, CornerL=0;	
	CAOIComponent *pComponent = NULL;
	const size_t ComponentCount = GetPanelComponentCount_Inline();
	//Find Panel CADXY		
	TREGION4D PanelRgnCad   = CalcPanelRgnCad(DistrictID); 
	const double PanelCadPosX = (PanelRgnCad.minX+PanelRgnCad.maxX)*0.5;
	const double PanelCadPosY = (PanelRgnCad.minY+PanelRgnCad.maxY)*0.5;
	
	Cp1 = Cp2 = Cp3 = Cp4 = NULL;
	CornerLMin[0] = DBL_MAX;
	CornerLMin[1] = DBL_MAX;
	CornerLMin[2] = DBL_MAX;
	CornerLMin[3] = DBL_MAX;
	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = GetPanelComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }	
		if ( DistrictID != pComponent->GetComponentDistrictID() ) { continue; }

		CadPosX = pComponent->GetComponentCadPosX();
		CadPosY = pComponent->GetComponentCadPosY();

		//Left Bottom
		CornerX = (CadPosX-PanelRgnCad.minX);
		CornerY = (CadPosY-PanelRgnCad.minY);
		CornerL = sqrt((CornerX*CornerX)+(CornerY*CornerY));
		if ( CornerL < CornerLMin[0] )
		{	
			Cp1 = pComponent;	
			CornerLMin[0] = CornerL;
		}

		//Right Top
		CornerX = (CadPosX-PanelRgnCad.maxX);
		CornerY = (CadPosY-PanelRgnCad.maxY);
		CornerL = sqrt((CornerX*CornerX)+(CornerY*CornerY));
		if ( CornerL < CornerLMin[1] )
		{	
			Cp2 = pComponent;	
			CornerLMin[1] = CornerL;
		}

		//Right Bottom
		CornerX = (CadPosX-PanelRgnCad.maxX);
		CornerY = (CadPosY-PanelRgnCad.minY);
		CornerL = sqrt((CornerX*CornerX)+(CornerY*CornerY));
		if ( CornerL < CornerLMin[2] )
		{	
			Cp3 = pComponent;	
			CornerLMin[2] = CornerL;
		}

		//Left Top
		CornerX = (CadPosX-PanelRgnCad.minX);
		CornerY = (CadPosY-PanelRgnCad.maxY);
		CornerL = sqrt((CornerX*CornerX)+(CornerY*CornerY));
		if ( CornerL < CornerLMin[3] )
		{	
			Cp4 = pComponent;	
			CornerLMin[3] = CornerL;
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIField* CAOIPanel::MatchPanelFieldPtr(const TREGION4D &Region, bool CadMode, bool Inner, DISTRICT_ID DistrictID)
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
	const size_t FieldCount = GetPanelFieldCount_Inline();

	BestFieldPtr = NULL;
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = GetPanelFieldPtr_Inline(i);
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
bool CAOIPanel::CreatePanelFieldListKernel(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, FIELD_DIVISION_MODE DivisionMode, bool ByCadRegion, bool CheckOldField, FIELD_BUILD_AREA_MODE AreaMode, TJetRgnList &RgnList, TJetFieldList &FieldList)//建立區塊分割資料
{
	CAOIProject   *ProjectPtr = GetPanelProjectPtr();
	if ( NULL == ProjectPtr ) { return false; }

	size_t         i=0, j=0, k=0;
	TREGION4D      Region, RegionAll;
	TJetRgn        JetRgn;		
	size_t         WindowCount=0;
	size_t         SubRngCount=0;		
	CAOIFd*        FdPtr = NULL;	
	CAOIRgn*       RgnSubPtr = NULL;
	CAOIMark*      MarkPtr = NULL;	
	CAOIField*     FieldPtr = NULL;	
	CAOIBoard*     BoardPtr = NULL;
	CAOIBarcode*   BarcodePtr = NULL;
	CAOIWindow*    WindowPtr = NULL;
	CAOIComponent* ComponentPtr = NULL;	
	bool           PartWndsInOneFOV = false;
	double         PosX=0, PosY=0;	
	double         FOVW_Inner=0, FOVH_Inner=0;
	double         FOVW_Outer=0, FOVH_Outer=0;
	double         FOVW_Inside=0, FOVH_Inside=0;
	const bool     CheckInner = false;		
	const size_t   FdCount = GetPanelFdCount_Inline();
	const size_t   MarkCount = GetPanelMarkCount_Inline();
	const size_t   BoardCount = GetPanelBoardCount_Inline();
	const size_t   BarcodeCount = GetPanelBarcodeCount_Inline();
	const size_t   ComponentCount = GetPanelComponentCount_Inline();	
	const size_t   PanelFieldCount = GetPanelFieldCount_Inline();
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

	//Reset Temp
	const int      SetFieldNone=0;
	const int      SetFieldDone=1;
	SetPanelAllObjectTempInt(DistrictID, SetFieldNone);

	//Board	
	if ( FIELD_BUILD_AREA_BOARD == AreaMode )
	{
		for ( i=0; i<BoardCount; i++ )
		{
			BoardPtr = GetPanelBoardPtr_Inline(i);
			if ( NULL == BoardPtr ) { continue; }
			if ( BoardPtr->CheckBoardNeedToCalculate(DistrictID) == false ) { continue; }
			if ( true == ByCadRegion )
			{	BoardPtr->GetBoardRgnCad(Region);	}
			else
			{	BoardPtr->GetBoardRgnStage(DistrictID, Region);	}
			//確認是否跨FOV
			if ( (Region.maxX-Region.minX)>FOVW_Inside || (Region.maxY-Region.minY)>FOVH_Inside ) 
			{	PartWndsInOneFOV = false;	}
			else
			{	PartWndsInOneFOV = true;	}
			if ( false == PartWndsInOneFOV )
			{	
				continue; 
				PosX = Region.GetCpX();
				PosY = Region.GetCpY();			
				Region.minX = PosX-DummyW2;
				Region.maxX = PosX+DummyW2;
				Region.minY = PosY-DummyH2;
				Region.maxY = PosY+DummyH2;
			}
			if ( true == CheckOldField )
			{	
				FieldPtr = BoardPtr->GetBoardPanelFieldPtr(DistrictID);	
				if ( NULL != FieldPtr )
				{
					if ( FieldPtr->CheckRegionInField(Region, ByCadRegion, CheckInner) == false )
					{	FieldPtr = NULL;	}
				}
				if ( NULL == FieldPtr )
				{	FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);	}

				if ( NULL == FieldPtr )
				{	BoardPtr->ResetBoardPanelFieldParam(DistrictID);	}
				else
				{	FieldPtr->AddFieldBoardPtr(BoardPtr);	}
			}

			if ( false==CheckOldField || NULL==FieldPtr )
			{			
				JetRgn.minX = Region.minX;
				JetRgn.minY = Region.minY;
				JetRgn.maxX = Region.maxX;
				JetRgn.maxY = Region.maxY;
				JetRgn.Ptr  = BoardPtr;
				FieldDivider.AddJetRgn(JetRgn);
			}			
			BoardPtr->SetBoardAllObjectTempInt(SetFieldDone);
		}
	}

	//Fiducial
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = GetPanelFdPtr_Inline(i);
		if ( NULL == FdPtr ) { continue; }
		if ( SetFieldDone == FdPtr->GetFdTempInt() ) { continue; }
		if ( FdPtr->GetFdNeedToCalculate() == false ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }		
		BoardPtr = FdPtr->GetFdBoardPtr();		
		if ( NULL == BoardPtr ) { continue; }		
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
					FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
					FdPtr->SetFdFieldPtr(FieldPtr);
				}
			}
			else 
			{	
				FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
				FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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

	//特徵點
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetPanelMarkPtr_Inline(i);
		if ( NULL == MarkPtr ) { continue; }
		if ( SetFieldDone == MarkPtr->GetMarkTempInt() ) { continue; }
		if ( MarkPtr->GetMarkNeedToCalculate() == false ) { continue; }
		if ( DistrictID != MarkPtr->GetMarkDistrictID() ) { continue; }
		if ( FIELD_BUILD_RANDOM_BOARD == BuildMode )
		{
			if ( MarkPtr->GetMarkBoardPtr() != NULL )
			{	continue; }
		}
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
					FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
					MarkPtr->SetMarkFieldPtr(FieldPtr);
				}
			}
			else 
			{	
				FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
				FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
		BarcodePtr = GetPanelBarcodePtr_Inline(i);
		if ( NULL == BarcodePtr ) { continue; }
		if ( SetFieldDone == BarcodePtr->GetBarcodeTempInt() ) { continue; }
		if ( BarcodePtr->GetBarcodeNeedToCalculate() == false ) { continue; }
		if ( DistrictID != BarcodePtr->GetBarcodeDistrictID() ) { continue; }		
		if ( FIELD_BUILD_RANDOM_BOARD == BuildMode )
		{
			if ( BarcodePtr->GetBarcodeBoardPtr() != NULL )
			{	continue; }
		}
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
					FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
					BarcodePtr->SetBarcodeFieldPtr(FieldPtr);
				}
			}
			else 
			{	
				FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
				FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
		ComponentPtr = GetPanelComponentPtr_Inline(i);
		if ( NULL == ComponentPtr ) { continue; }
		if ( SetFieldDone == ComponentPtr->GetComponentTempInt() ) { continue; }
		if ( ComponentPtr->GetComponentNeedToCalculate() == false ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		if ( FIELD_BUILD_RANDOM_BOARD == BuildMode )
		{
			if ( ComponentPtr->GetComponentBoardPtr() != NULL )
			{	continue; }
		}
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
					FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
					ComponentPtr->SetComponentFieldPtr(FieldPtr);
				}
			}
			else
			{
				FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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

		if ( true==ByCadRegion && NULL!=FieldPtr )//check stage region
		{
			/*
			bool IsMatchCad=false;
			bool IsMatchStage=false;
			CAOIField *FieldPtr_Cad=NULL;
			CAOIField *FieldPtr_Stage=NULL;			
			
			RECT      FrameRect={0};
			TREGION4D CameraRgn;
			TREGION4D CadRgn;
			TREGION4D StageRgn;			
			TREGION4D FieldCadRgn;
			TREGION4D FieldStageRgn;
			TREGION4D FieldStageRgnFull;
			TPOINT2D  CadPos;
			TPOINT2D  CadOffset;
			TPOINT2D  StagePos2D;
			TPOINT3D  StagePos;
			TPOINT3D  StagePos2;
			TPOINT3D  StageOffset;
			TPOINT2D  FieldCadPos;
			TPOINT3D  FieldStagePos;
			TPOINT3D  FieldStagePos2;
			ComponentPtr->GetComponentWindowCadRegion(CadRgn);
			ComponentPtr->GetComponentWindowStageRegion(StageRgn);
			if ( true == CheckInner )
			{	
				FieldPtr->GetFieldCadRgn_Inner(FieldCadRgn); 
				FieldPtr->GetFieldStageRgn_Inner(FieldStageRgn); 
			}
			else 
			{	
				FieldPtr->GetFieldCadRgn_Outer(FieldCadRgn);	
				FieldPtr->GetFieldStageRgn_Outer(FieldStageRgn);	
			}
			FieldPtr->GetFieldStageRgn_Real(FieldStageRgnFull);
			CadPos = ComponentPtr->GetComponentCadPos();
			StagePos = ComponentPtr->GetComponentStagePos();
			FieldCadPos = FieldPtr->GetFieldCadPos();
			FieldStagePos = FieldPtr->GetFieldStagePos();
			if ( JetAPI::CheckRgnInRegion(StageRgn, FieldStageRgn, true) == false )
			{	
				CMapCoordinate Map = m_PanelMapCTS;
				CadOffset.x = CadPos.x-FieldCadPos.x;
				CadOffset.y = CadPos.y-FieldCadPos.y;				
				
				Map.Map2D(CadPos.x, CadPos.y, StagePos2.x, StagePos2.y);
				Map.Map2D(FieldCadPos.x, FieldCadPos.y, FieldStagePos2.x, FieldStagePos2.y);
				Map.Map2D(CadOffset.x, CadOffset.y, StageOffset.x, StageOffset.y);

				IsMatchCad = FieldPtr->CheckRegionInField(CadRgn, true, CheckInner);
				IsMatchStage = FieldPtr->CheckRegionInField(StageRgn, false, CheckInner);
				FieldPtr_Cad = MatchPanelFieldPtr(CadRgn, true, CheckInner, DistrictID);
				FieldPtr_Stage = MatchPanelFieldPtr(StageRgn, false, CheckInner, DistrictID);
				
				StagePos2D.x = FieldStagePos.x;
				StagePos2D.y = FieldStagePos.y;
				IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(PRIMARY_CAMERA_ID);
				IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(PRIMARY_CAMERA_ID);
				AOIDataCollect.MapStageRegionToCamera(PRIMARY_CAMERA_ID, StageRgn, StagePos2D, CameraRgn);		
				JetAPI::Region4DToRect(CameraRgn, FrameRect, true);	
				if ( JetAPI::CheckImageRoi(ImageW, ImageH, FrameRect) == false )
				{
					ComponentPtr = ComponentPtr;	
				}

				ComponentPtr = ComponentPtr;	
				return false;
			}
			*/
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
				FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
						FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
						WindowPtr->SetWindowFieldPtr(FieldPtr);
					}
				}
				else
				{
					FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
					FieldPtr = MatchPanelFieldPtr(Region, ByCadRegion, CheckInner, DistrictID);
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
		if ( GetPanelMapCTS(DistrictID, PanelMapCTS) == false ) { return false; }
		if ( GetPanelMapSTC(DistrictID, PanelMapSTC) == false ) { return false; }

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
bool CAOIPanel::UpdatePanelComponentToModel()//更新整板內的零件至零件模組內
{
	size_t i = 0;
	CAOIComponent *pComponent = NULL;	
	const size_t ComponentCount = CAOIPanel::GetPanelComponentCount_Inline();
	for ( i=0; i<ComponentCount; i++ )
	{
		pComponent = CAOIPanel::GetPanelComponentPtr_Inline(i);
		if ( NULL == pComponent ) { continue; }
		pComponent->UpdateComponentParamToModel(false);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIPanel::ResetPanelFrameImageRect(DISTRICT_ID DistrictID)//重新設定整板下物件的畫面影像位置
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
	const size_t   MarkCount = GetPanelMarkCount_Inline();
	const size_t   FieldCount = GetPanelFieldCount_Inline();
	const size_t   BarcodeCount = GetPanelBarcodeCount_Inline();
	const size_t   ComponentCount = GetPanelComponentCount();		
	
	//Assign Field To Mark
	for ( i=0; i<MarkCount; i++ )
	{
		MarkPtr = GetPanelMarkPtr_Inline(i);
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
		BarcodePtr = GetPanelBarcodePtr_Inline(i);
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
		ComponentPtr = GetPanelComponentPtr_Inline(i);
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