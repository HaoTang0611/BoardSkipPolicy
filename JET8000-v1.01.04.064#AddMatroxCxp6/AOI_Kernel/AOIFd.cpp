// AOIFd.cpp: implementation of the CAOIFd class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIFd.h"
//-------------------------------------------------------------------------------------//
#include "JetMatch.h"
#include "AOIFileIO.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
CRITICAL_SECTION    CAOIFd::m_csFd;//同步機制-關鍵區間
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CAOIFd, CAOIRgn)
//-------------------------------------------------------------------------------------//
void CAOIFd::InitialFdLock()//初始化定位點的關鍵區間
{
	::InitializeCriticalSection(&m_csFd);
}
//-------------------------------------------------------------------------------------//
void CAOIFd::DeleteFdLock()//刪除定位點的關鍵區間
{
	::DeleteCriticalSection(&m_csFd);
}
//-------------------------------------------------------------------------------------//
void CAOIFd::LockFd()//進入定位點的關鍵區間
{
	::EnterCriticalSection(&m_csFd);
}
//-------------------------------------------------------------------------------------//
void CAOIFd::UnlockFd()//離開定位點的關鍵區間
{
	::LeaveCriticalSection(&m_csFd);
}
//-------------------------------------------------------------------------------------//
CAOIFd::CAOIFd():CAOIRgn(AOI_OBJ_FD)
{
	PreInitFd();
	InitialFd();
}
//-------------------------------------------------------------------------------------//
CAOIFd::CAOIFd(const CAOIFd &fd):CAOIRgn(fd)
{
	PreInitFd();
	CloneFd(fd);
}
//-------------------------------------------------------------------------------------//
CAOIFd::~CAOIFd()
{	
	ReleaseFdAllImageBuffer();	
}
//-------------------------------------------------------------------------------------//
CAOIFd& CAOIFd::operator=(const CAOIFd &fd)
{
	if ( this == &fd ) { return *this; }	
	CAOIRgn::operator=(fd);
	CloneFd(fd);
	return *this; 
}
//-------------------------------------------------------------------------------------//
inline void CAOIFd::PreInitFd()
{
	size_t i = 0;
	//定位點影像資料
	for ( i=0; i<FD_MAX_PATTERN; i++ )
	{	m_FdPatternPtr[i] = NULL;	}	
}
//-------------------------------------------------------------------------------------//
inline void CAOIFd::InitialFd()
{
	//---------------------------------------------------------------------------------//
	//CAOIRgn::InitialRgn();//CAOIRgn建構子會自動呼叫
	//---------------------------------------------------------------------------------//
	CAOIFd::ReleaseFdAllImageBuffer();
	//---------------------------------------------------------------------------------//		
	CAOIRgn::m_RgnFrameIndex = 0;//影像序號
	CAOIRgn::m_RgnFrameUniqueID = FRAME_UNIQUE_ID_FD;//影像唯一碼
	//---------------------------------------------------------------------------------//		
	m_FdSelected = false;//定位點是否選取到
	m_FdDeleted = false;//定位點是否刪除
	m_FdGroupID = -1;
	m_FdUniqueID = -1;	
	m_FdNeedCalcMap = false;//定位點需要計算作標轉換
	SetFdSaveTestImageMode(SAVE_TEST_IMAGE_DISABLE);
	//---------------------------------------------------------------------------------//
	m_FdMinScore = 50.0;
	//---------------------------------------------------------------------------------//	
	m_FdTempInt[0]=0;//定位點暫存整數
	m_FdTempInt[1]=0;//定位點暫存整數
	m_FdTempInt[2]=0;//定位點暫存整數
	m_FdTempInt[3]=0;//定位點暫存整數
	//---------------------------------------------------------------------------------//	
	m_FdSortID = 0;//定位點檢測順序
	m_FdConfirmUIResultID = 0;
	//---------------------------------------------------------------------------------//	
	m_FdModel.SetModelFdPtr(this);
	m_FdModel.SetModelIsolated(false);
	m_FdModel.SetModelExtendRangeX(0);
	m_FdModel.SetModelExtendRangeY(0);
	m_FdModel.SetModelExtendAutoAdjust(false);
	//---------------------------------------------------------------------------------//		
	m_FdPatExtendSize = TSIZE2D();//定位點樣板外擴尺寸
	m_FdTeachStagePos = TPOINT3D();//定位點教導時機台座標	
	m_FdSpaceBasePlane = 0;
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
inline void CAOIFd::CloneFd(const CAOIFd &fd)
{
	size_t i = 0;
	//---------------------------------------------------------------------------------//	
	CAOIFd::CloneFdImage(fd);
	//---------------------------------------------------------------------------------//	
	m_FdSelected = fd.m_FdSelected;//定位點是否選取到
	m_FdDeleted = fd.m_FdDeleted;//定位點是否刪除
	m_FdGroupID = fd.m_FdGroupID;
	m_FdUniqueID = fd.m_FdUniqueID;
	m_FdNeedCalcMap = fd.m_FdNeedCalcMap;//定位點需要計算作標轉換	
	//---------------------------------------------------------------------------------//	
	m_FdMinScore = fd.m_FdMinScore;
	//---------------------------------------------------------------------------------//	
	m_FdModel = fd.m_FdModel;
	m_FdModel.SetModelFdPtr(this);	
	//---------------------------------------------------------------------------------//		
	m_FdTempInt[0] = fd.m_FdTempInt[0];
	m_FdTempInt[1] = fd.m_FdTempInt[1];
	m_FdTempInt[2] = fd.m_FdTempInt[2];
	m_FdTempInt[3] = fd.m_FdTempInt[3];
	//---------------------------------------------------------------------------------//	
	m_FdSortID = fd.m_FdSortID;//定位點檢測順序	
	m_FdConfirmUIResultID = fd.m_FdConfirmUIResultID;	
	//---------------------------------------------------------------------------------//
	for ( i=0; i<FD_MAX_PATTERN; i++ )
	{
		m_FdImageNameORG[i] = fd.m_FdImageNameORG[i];//定位點圖檔名稱-原圖
		m_FdImageNameMSK[i] = fd.m_FdImageNameMSK[i];//定位點圖檔名稱-遮罩圖
	}
	//---------------------------------------------------------------------------------//	
	m_FdPatExtendSize = fd.m_FdPatExtendSize;//定位點樣板外擴尺寸
	m_FdTeachStagePos = fd.m_FdTeachStagePos;//定位點教導時機台座標		
	m_FdSpaceBasePlane = fd.m_FdSpaceBasePlane;
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
inline void  CAOIFd::CloneFdImage(const CAOIFd &fd)
{
	const char fnName[] = "CAOIFd::CloneFdImage";
	size_t i = 0;
	size_t BufferSize = 0;
	CAOIFd::ReleaseFdAllImageBuffer();
	for ( i=0; i<FD_MAX_PATTERN; i++ )
	{
		if ( NULL == fd.m_FdPatternPtr[i] ) { continue; }
		BufferSize = ImageAPI.CalcBufferSize(fd.m_FdPatternStep[i], fd.m_FdPatternH[i]);
		if ( JetMemory.alloc_func(BufferSize, m_FdPatternPtr[i], fnName, "m_FDImage") == false )
		{	return;	}
		::memcpy(m_FdPatternPtr[i], fd.m_FdPatternPtr[i], sizeof(IMAGE_DATA)*BufferSize);
		m_FdPatternW[i]    = fd.m_FdPatternW[i];//定位點影像寬度
		m_FdPatternH[i]    = fd.m_FdPatternH[i];//定位點影像高度
		m_FdPatternStep[i] = fd.m_FdPatternStep[i];//定位點影像步長
		m_FdBitCount[i]  = fd.m_FdBitCount[i];//定位點影像位元數
	}
}
//-------------------------------------------------------------------------------------//
CAOIFd* CAOIFd::CloneFdObj() const//建立且複製一個定位點
{
	CAOIFd *ObjPtr = AOIObjManager.CreateFdObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::CreateFdSelfFieldPtr()//建立專屬Field指標	
{
	bool bIsOK = true;
	bIsOK = CAOIRgn::CreateRgnSelfFieldPtr();
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CAOIFd::GetFdWndPtr()//取得定位點檢測框
{
	CAOIModel *ModelPtr = GetFdModelPtr();
	if ( NULL == ModelPtr ) { return NULL; }

	size_t       i=0;
	CAOIWnd     *WndPtr = NULL;
	const size_t WndCount = ModelPtr->GetModelWndCount();
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }
		if ( WndPtr->GetWndEnabled() == false ) { continue; }
		if ( WndPtr->GetWndAlgParam().GetAlgPatternFileUsed() == false ) { continue; }				
		return WndPtr;
	}
	for ( i=0; i<WndCount; i++ )
	{
		WndPtr = ModelPtr->GetModelWndPtr(i, false);
		if ( NULL == WndPtr ) { continue; }		
		if ( WndPtr->GetWndAlgParam().GetAlgPatternFileUsed() == false ) { continue; }				
		return WndPtr;
	}
	WndPtr = ModelPtr->GetModelWndPtr(0, true);
	return WndPtr;
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdFieldPtr(CAOIField* Ptr)
{
	CAOIRgn::SetRgnFieldPtr(Ptr);
}
//-------------------------------------------------------------------------------------//
inline bool CAOIFd::CheckPatternIdx(unsigned int idx)
{
	if ( idx >= FD_MAX_PATTERN ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline void CAOIFd::ReleaseFdPatternBuffer(unsigned int idx)
{
	if ( CAOIFd::CheckPatternIdx(idx) == false ) { return; }
	if ( NULL != m_FdPatternPtr[idx] )
	{	JetMemory.free_func(m_FdPatternPtr[idx]); }

	m_FdPatternW[idx] = 0;
	m_FdPatternH[idx] = 0;
	m_FdPatternStep[idx] = 0;
	m_FdBitCount[idx] = 8;
	return;
}
//-------------------------------------------------------------------------------------//
inline void  CAOIFd::ReleaseFdAllImageBuffer()
{
	size_t i = 0;	
	for ( i=0; i<FD_MAX_PATTERN; i++ )
	{	
		if ( NULL != m_FdPatternPtr[i] )
		{	JetMemory.free_func(m_FdPatternPtr[i]); }

		m_FdPatternW[i] = 0;
		m_FdPatternH[i] = 0;
		m_FdPatternStep[i] = 0;
		m_FdBitCount[i] = 8;
	}
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::CheckFdGroupIDValid() const//確認定位點群組編號有效
{
	if ( -1 == m_FdGroupID ) { return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
CString CAOIFd::GetFdFullName() const//取得定位點全名
{
	CString FullName;
	CAOIPanel *PanelPtr = GetFdPanelPtr();
	CAOIBoard *BoardPtr = GetFdBoardPtr();
	if ( NULL == PanelPtr )
	{	FullName.Format(_T("Fd%05d"), GetFdIndex_Project()+1);	}
	else
	{
		if ( NULL == BoardPtr )
		{	FullName.Format(_T("P%05d_Fd%05d"), PanelPtr->GetPanelIndex_Project()+1, GetFdIndex_Panel()+1);	}
		else
		{	FullName.Format(_T("P%05d_B%05d_Fd%05d"), PanelPtr->GetPanelIndex_Project()+1, BoardPtr->GetBoardIndex_Panel()+1, GetFdIndex_Board()+1);	}
	}
	return FullName;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::GetFdModelImageIsSaved() const//取得定位點模組影像是否儲存
{
	return CAOIRgn::GetRgnModelImageIsSaved();
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdModelImageIsSaved(bool value)//設定定位點模組影像是否儲存
{
	CAOIRgn::SetRgnModelImageIsSaved(value);
}
//-------------------------------------------------------------------------------------//
SAVE_TEST_IMAGE_MODE CAOIFd::GetFdSaveTestImageMode() const//取得定位點儲存影像模式
{
	return CAOIRgn::GetRgnSaveTestImageMode();
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdSaveTestImageMode(SAVE_TEST_IMAGE_MODE value)//設定定位點儲存影像模式	
{
	CAOIRgn::SetRgnSaveTestImageMode(value);
}
//-------------------------------------------------------------------------------------//
LANE_ID CAOIFd::GetFdLaneID() const
{ 
	return CAOIRgn::GetRgnLaneID();	
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdLaneID(LANE_ID value)
{
	CAOIRgn::SetRgnLaneID(value);
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdSortID(unsigned int value)
{
	m_FdSortID = value; 
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIFd::GetFdSortID() const
{ 
	return m_FdSortID; 
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::CheckFdIsPanelFd() const//確認是否為整板條碼
{
	if ( NULL == GetFdPanelPtr() ) { return false; }
	if ( NULL != GetFdBoardPtr() ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::CheckFdIsBoardFd() const//確認是否為單板條碼
{
	if ( NULL == GetFdPanelPtr() ) { return false; }
	if ( NULL == GetFdBoardPtr() ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
int CAOIFd::CalcFdOpenMPCountByPixels()
{
	return CalcRgnOpenMPCountByPixels();
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdOpenMPCount(int value)
{
	SetRgnOpenMPCount(value);
	CAOIModel *ModelPtr = GetFdModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelOpenMPCount(value); }
}
//-------------------------------------------------------------------------------------//
void CAOIFd::BypassSkipFd(RESULT_ID value)//不檢測或跳過定位點
{
	CAOIFd *FdPtr = this;	
	LANE_ID LaneID = FdPtr->GetFdLaneID();
	FdPtr->SetFdNeedToCalculate(false);	
	FdPtr->SetFdNeedToCalculateBackup(false);
	FdPtr->SetFdResultID_AOI(value);
	FdPtr->SetFdResultID_Alarm(value);
	FdPtr->UpdateFdResultID_AOI_Lane(LaneID);

	CAOIModel *ModelPtr = FdPtr->GetFdModelPtr();
	ModelPtr->SetModelResultID(value);
	ModelPtr->SetModelResultID_Alarm(value);
	ModelPtr->BypassSkipModelWnd(value);
	return;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIFd::GetFdResultID_AOI() const//取得定位點結果編號
{
	return CAOIRgn::GetRgnResultID_AOI();
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdResultID_AOI(RESULT_ID value)//設定定位點結果編號
{
	CAOIRgn::SetRgnResultID_AOI(value);
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIFd::GetFdResultID_AOI_LA() const//取得定位點結果編號-A軌
{
	return CAOIRgn::GetRgnResultID_AOI_LA();
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdResultID_AOI_LA(RESULT_ID value)//設定定位點結果編號-A軌
{
	CAOIRgn::SetRgnResultID_AOI_LA(value);
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIFd::GetFdResultID_AOI_LB() const//取得定位點結果編號-B軌
{
	return CAOIRgn::GetRgnResultID_AOI_LB();
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdResultID_AOI_LB(RESULT_ID value)//設定定位點結果編號-B軌
{
	CAOIRgn::SetRgnResultID_AOI_LB(value);
}
//-------------------------------------------------------------------------------------//
void CAOIFd::UpdateFdResultID_AOI_Lane(LANE_ID LaneID)//更新定位點結果編號-軌道
{
	RESULT_ID ResultID=GetFdResultID_AOI();
	switch ( LaneID )
	{
	case LANE_ID_A:	SetFdResultID_AOI_LA(ResultID);	break;
	case LANE_ID_B:	SetFdResultID_AOI_LB(ResultID);	break;	
	}
	return;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIFd::GetFdResultID_AOI_Lane(LANE_ID LaneID) const//取得定位點結果編號-軌道
{
	RESULT_ID ResultID;
	switch ( LaneID )
	{
	case LANE_ID_A:	ResultID = GetFdResultID_AOI_LA();	break;
	case LANE_ID_B:	ResultID = GetFdResultID_AOI_LB();	break;
	default:		ResultID = GetFdResultID_AOI();	break;
	}
	return ResultID;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIFd::GetFdResultID_ARS() const//取得定位點結果編號
{
	return CAOIRgn::GetRgnResultID_ARS();
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdResultID_ARS(RESULT_ID value)//設定定位點結果編號
{
	CAOIRgn::SetRgnResultID_ARS(value);
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIFd::GetFdResultID_ARS_LA() const//取得定位點結果編號-A軌
{
	return CAOIRgn::GetRgnResultID_ARS_LA();
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdResultID_ARS_LA(RESULT_ID value)//設定定位點結果編號-A軌
{
	CAOIRgn::SetRgnResultID_ARS_LA(value);
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIFd::GetFdResultID_ARS_LB() const//取得定位點結果編號-B軌
{
	return CAOIRgn::GetRgnResultID_ARS_LB();
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdResultID_ARS_LB(RESULT_ID value)//設定定位點結果編號-B軌
{
	CAOIRgn::SetRgnResultID_ARS_LB(value);
}
//-------------------------------------------------------------------------------------//
void CAOIFd::UpdateFdResultID_ARS_Lane(LANE_ID LaneID)//更新定位點結果編號-軌道
{
	RESULT_ID ResultID=GetFdResultID_ARS();
	switch ( LaneID )
	{
	case LANE_ID_A:	SetFdResultID_ARS_LA(ResultID);	break;
	case LANE_ID_B:	SetFdResultID_ARS_LB(ResultID);	break;	
	}
	return;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIFd::GetFdResultID_ARS_Lane(LANE_ID LaneID) const//取得定位點結果編號-軌道
{
	RESULT_ID ResultID;
	switch ( LaneID )
	{
	case LANE_ID_A:	ResultID = GetFdResultID_ARS_LA();	break;
	case LANE_ID_B:	ResultID = GetFdResultID_ARS_LB();	break;
	default:		ResultID = GetFdResultID_ARS();	break;
	}
	return ResultID;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CAOIFd::GetFdResultID_Alarm() const//取得定位點結果編號-停機
{
	return CAOIRgn::GetRgnResultID_Alarm();
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdResultID_Alarm(RESULT_ID value)//設定定位點結果編號-停機
{
	CAOIRgn::SetRgnResultID_Alarm(value);
}
//-------------------------------------------------------------------------------------//
const TPOINT3D& CAOIFd::GetFdTeachStagePos() const
{	
	return m_FdTeachStagePos;
}
//-------------------------------------------------------------------------------------//
void CAOIFd::GetFdTeachStagePos(TPOINT2D &value) const
{
	value.x = m_FdTeachStagePos.x;
	value.y = m_FdTeachStagePos.y;
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdTeachStagePos(const TPOINT2D &value)
{
	m_FdTeachStagePos.x = value.x;
	m_FdTeachStagePos.y = value.y;
}
//-------------------------------------------------------------------------------------//
void CAOIFd::GetFdTeachStagePos(TPOINT3D &value) const
{
	value = m_FdTeachStagePos;	
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdTeachStagePos(const TPOINT3D &value)
{	
	m_FdTeachStagePos = value;	
}
//-------------------------------------------------------------------------------------//
double CAOIFd::GetFdTeachStagePosX() const
{ 
	return m_FdTeachStagePos.x; 
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdTeachStagePosX(double value)
{ 
	m_FdTeachStagePos.x = value; 
}
//-------------------------------------------------------------------------------------//
double CAOIFd::GetFdTeachStagePosY() const
{ 
	return m_FdTeachStagePos.y; 
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdTeachStagePosY(double value)
{ 
	m_FdTeachStagePos.y = value; 
}
//-------------------------------------------------------------------------------------//
double CAOIFd::GetFdTeachStagePosZ() const
{ 
	return m_FdTeachStagePos.z; 
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdTeachStagePosZ(double value)
{ 
	m_FdTeachStagePos.z = value; 
}
//-------------------------------------------------------------------------------------//
void CAOIFd::MoveFdCadPos(double dX, double dY)//移動定位點座標
{
	CAOIRgn::MoveRgnCadPos(dX, dY);
	m_FdModel.SetModelAttachedPosCad(m_RgnCadPos);	
}
//-------------------------------------------------------------------------------------//
void CAOIFd::MoveFdStagePos(double dX, double dY)//移動定位點座標	
{
	CAOIRgn::MoveRgnStagePos(dX, dY);
	m_FdModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);	
}
//-------------------------------------------------------------------------------------//
void CAOIFd::MoveFdPos(double dX, double dY, CMapCoordinate *MapPtr)//移動定位點座標
{	
	MoveFdCadPos(dX, dY);
	if ( NULL!=MapPtr )
	{	
		MapFdCadToStagePos(*MapPtr);		
		SetFdTeachStagePos(m_RgnStagePos);		
		m_FdModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);	
	}
}
//-------------------------------------------------------------------------------------//
void CAOIFd::ModifyFdRoiRegion(const TREGION4D &dRgn)//修正定位點尺寸
{
	CAOIRgn::ModifyRgnRoiRegion(dRgn);
}
//-------------------------------------------------------------------------------------//
void CAOIFd::ModifyFdBodyRegion(const TREGION4D &dRgn)//修正定位點尺寸
{
	CAOIRgn::ModifyRgnBodyRegion(dRgn);
}
//-------------------------------------------------------------------------------------//
void CAOIFd::CalcFdCadCornerPos()//計算定位點Cad端點座標	
{
	CAOIRgn::CalcRgnCadCornerPos();
}
//-------------------------------------------------------------------------------------//
void CAOIFd::LayoutFdStageCornerPos()//更新定位點機台端點座標
{
	CAOIRgn::LayoutRgnStageCornerPos();
}
//-------------------------------------------------------------------------------------//
void CAOIFd::CalcFdExtendCadCornerPos(TPOINT2D CornerPt[]) const//計算定位點外擴Cad端點座標
{
	const double Angle = 0;
	const double SizeW = GetFdBodySizeW()+GetFdRoiExtendSizeW();
	const double SizeH = GetFdBodySizeH()+GetFdRoiExtendSizeH();		
	CAOIRgn::CalcRgnCornerPos(SizeW, SizeH, Angle, CornerPt);
}
//-------------------------------------------------------------------------------------//
void CAOIFd::CalcFdExtendStageCornerPos(TPOINT2D CornerPt[])  const//計算定位點外擴機台端點座標
{
	TPOINT2D  CadCornerPt[4];
	TPOINT2D  CadCenterPos = m_RgnCadPos;
	TPOINT3D  StageCenterPos = m_RgnStagePos;

	const double Angle = 0;
	const double SizeW = GetFdBodySizeW()+GetFdRoiExtendSizeW();
	const double SizeH = GetFdBodySizeH()+GetFdRoiExtendSizeH();	
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();

	CAOIRgn::CalcRgnCornerPos(SizeW, SizeH, Angle, CadCornerPt);
	if ( true == SignX )
	{
		CornerPt[0].x = StageCenterPos.x+CadCornerPt[0].x-CadCenterPos.x;
		CornerPt[1].x = StageCenterPos.x+CadCornerPt[1].x-CadCenterPos.x;
		CornerPt[2].x = StageCenterPos.x+CadCornerPt[2].x-CadCenterPos.x;
		CornerPt[3].x = StageCenterPos.x+CadCornerPt[3].x-CadCenterPos.x;
	}
	else
	{
		CornerPt[0].x = StageCenterPos.x-CadCornerPt[0].x+CadCenterPos.x;
		CornerPt[1].x = StageCenterPos.x-CadCornerPt[1].x+CadCenterPos.x;
		CornerPt[2].x = StageCenterPos.x-CadCornerPt[2].x+CadCenterPos.x;
		CornerPt[3].x = StageCenterPos.x-CadCornerPt[3].x+CadCenterPos.x;
	}

	if ( true == SignY )
	{
		CornerPt[0].y = StageCenterPos.y+CadCornerPt[0].y-CadCenterPos.y;	
		CornerPt[1].y = StageCenterPos.y+CadCornerPt[1].y-CadCenterPos.y;	
		CornerPt[2].y = StageCenterPos.y+CadCornerPt[2].y-CadCenterPos.y;	
		CornerPt[3].y = StageCenterPos.y+CadCornerPt[3].y-CadCenterPos.y;
	}
	else
	{
		CornerPt[0].y = StageCenterPos.y-CadCornerPt[0].y+CadCenterPos.y;	
		CornerPt[1].y = StageCenterPos.y-CadCornerPt[1].y+CadCenterPos.y;	
		CornerPt[2].y = StageCenterPos.y-CadCornerPt[2].y+CadCenterPos.y;	
		CornerPt[3].y = StageCenterPos.y-CadCornerPt[3].y+CadCenterPos.y;
	}	
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIFd::MapFdCadToStagePos(const CMapCoordinate &Map)//將CAD轉成機台座標
{
	CAOIRgn::MapRgnCadToStagePos(Map);
	//CAOIFd::m_FdTeachStagePos = CAOIRgn::m_RgnStagePos;
	const double SagePosX = CAOIRgn::GetRgnStagePosX();
	const double SagePosY = CAOIRgn::GetRgnStagePosY();
	CAOIFd::m_FdModel.SetModelAttachedPosStage(SagePosX, SagePosY);
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdFrameImageSize_um(const TSIZE2D &value)
{ 
	CAOIRgn::SetRgnFrameImageSize_um(value);
	m_FdModel.SetModelImageSize_um(value);
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdFrameImageCadOffset_um(const TPOINT2D &value)
{
	CAOIRgn::SetRgnFrameImageCadOffset_um(value);
	m_FdModel.SetModelImageCadOffset_um(value);
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdFrameImageStageOffset_um(const TPOINT2D &value)
{
	TPOINT2D CadOffset;
	AOIDataCollect.MapStageOffsetPtToCad(value, CadOffset);
	SetFdFrameImageCadOffset_um(CadOffset);	
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::SpinFd(double Angle, CMapCoordinate *MapPtr)//定位點自旋轉
{
	double CadAngle = 0;
	double StageAngle = 0;
	CadAngle = Angle;
	AOIDataCollect.MapCadAngleToStage(CadAngle, StageAngle);
	const double CadCpX = CAOIRgn::m_RgnCadPos.x;
	const double CadCpY = CAOIRgn::m_RgnCadPos.y;
	const double StageCpX = CAOIRgn::m_RgnStagePos.x;
	const double StageCpY = CAOIRgn::m_RgnStagePos.y;

	CAOIFd::RotateFd(CadAngle, CadCpX, CadCpY, MapPtr);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::RotateFd(double Angle, double CpX, double CpY, CMapCoordinate *MapPtr)//定位點旋轉
{
	CAOIRgn::RotateRgnCad(Angle, CpX, CpY);
	CAOIRgn::LayoutRgnStageCornerPos();
	
	CAOIFd::m_FdModel.RotateModel(Angle, 0, 0);	
	CAOIFd::m_FdModel.SetModelAttachedPosCad(m_RgnCadPos);	
	if ( NULL != MapPtr )
	{
		MapFdCadToStagePos(*MapPtr);		
		SetFdTeachStagePos(CAOIRgn::m_RgnStagePos);		
		m_FdModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::MirrorXFd(double CpX, CMapCoordinate *MapPtr)//定位點鏡射-X
{
	CAOIRgn::MirrorXRgnCad(CpX);
	CAOIFd::m_FdModel.MirrorModelYAxis(0);
	CAOIFd::m_FdModel.SetModelAttachedPosCad(m_RgnCadPos);	
	CAOIFd::m_FdModel.SetModelAttachedAngle(m_RgnAngle);
	if ( NULL != MapPtr )
	{
		MapFdCadToStagePos(*MapPtr);		
		SetFdTeachStagePos(CAOIRgn::m_RgnStagePos);		
		m_FdModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::MirrorYFd(double CpY, CMapCoordinate *MapPtr)//定位點鏡射-Y
{
	CAOIRgn::MirrorYRgnCad(CpY);
	CAOIFd::m_FdModel.MirrorModelXAxis(0);
	CAOIFd::m_FdModel.SetModelAttachedPosCad(m_RgnCadPos);	
	CAOIFd::m_FdModel.SetModelAttachedAngle(m_RgnAngle);
	if ( NULL != MapPtr )
	{
		MapFdCadToStagePos(*MapPtr);		
		SetFdTeachStagePos(CAOIRgn::m_RgnStagePos);		
		m_FdModel.SetModelAttachedPosStage(m_RgnStagePos.x, m_RgnStagePos.y);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdImageNameORG(unsigned int idx, LPCTSTR filename)
{
	if ( idx >= FD_MAX_PATTERN ) { return; }
	this->m_FdImageNameORG[idx] = filename;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIFd::GetFdImageNameORG(unsigned int idx) const
{
	if ( idx >= FD_MAX_PATTERN ) 
	{	return m_FdImageNameORG[0];	}
	return this->m_FdImageNameORG[idx];
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdImageNameMSK(unsigned int idx, LPCTSTR filename)
{
	if ( idx >= FD_MAX_PATTERN ) { return; }
	this->m_FdImageNameMSK[idx] = filename;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIFd::GetFdImageNameMSK(unsigned int idx) const
{
	if ( idx >= FD_MAX_PATTERN ) 
	{	return m_FdImageNameMSK[0];	}
	return this->m_FdImageNameMSK[idx];
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::SetFdPattern(unsigned int idx, IMAGE_SIZE PatW, IMAGE_SIZE PatH, IMAGE_SIZE PatStep, IMAGE_SIZE BitCount, IMAGE_PTR PatPtr, bool Invert)
{
	if ( CAOIFd::CheckPatternIdx(idx) == false ) { return false; }
	const char fnName[] = "CAOIFd::SetFdPattern";	
	CAOIFd::ReleaseFdPatternBuffer(idx);
	if ( NULL == PatPtr ) { return true; }

	const size_t BufferSize = ImageAPI.CalcBufferSize(PatStep, PatH);
	if ( JetMemory.alloc_func(BufferSize, m_FdPatternPtr[idx], fnName, "m_FdImagePtr") == false )
	{	return false; }

	if ( true == Invert )
	{
		size_t i=0;
		size_t srcIdx=0, dstIdx=0;
		const size_t CopyLen = sizeof(IMAGE_DATA)*PatW;
		for ( i=0; i<PatH; i++ )
		{
			srcIdx = i*PatStep;
			dstIdx = (PatH-i-1)*PatStep;
			::memcpy(&(m_FdPatternPtr[idx][dstIdx]), &(PatPtr[srcIdx]), CopyLen);
		}
	}
	else
	{	::memcpy(m_FdPatternPtr[idx], PatPtr, sizeof(IMAGE_DATA)*BufferSize);	}
	m_FdPatternW[idx] = PatW;
	m_FdPatternH[idx] = PatH;
	m_FdPatternStep[idx] = PatStep;
	m_FdBitCount[idx] = BitCount;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::GetFdPattern(unsigned int idx, IMAGE_SIZE &PatW, IMAGE_SIZE &PatH, IMAGE_SIZE &PatStep, IMAGE_SIZE &BitCount, IMAGE_PTR &PatPtr)
{
	if ( CAOIFd::CheckPatternIdx(idx) == false ) { return false; }
	PatW        = m_FdPatternW[idx];
	PatH        = m_FdPatternH[idx];
	PatPtr      = m_FdPatternPtr[idx];
	PatStep     = m_FdPatternStep[idx];
	BitCount    = m_FdBitCount[idx];
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::CloneFdPattern(unsigned int idx, IMAGE_SIZE &PatW, IMAGE_SIZE &PatH, IMAGE_SIZE &PatStep, IMAGE_SIZE &BitCount, IMAGE_PTR &PatPtr)
{
	if ( CAOIFd::CheckPatternIdx(idx) == false ) { return false; }
	if ( NULL == m_FdPatternPtr[idx] ) { return false; }

	const char fnName[] = "CAOIFd::CloneFdPattern";	
	JetMemory.free_func(PatPtr);
	const size_t BufferSize = ImageAPI.CalcBufferSize(m_FdPatternStep[idx], m_FdPatternH[idx]);
	if ( JetMemory.alloc_func(BufferSize, PatPtr, fnName, "PatPtr") == false )
	{	return false; }
	if ( this->CloneFdPattern3(idx, PatW, PatH, PatStep, BitCount, PatPtr) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::CloneFdPattern3(unsigned int idx, IMAGE_SIZE &PatW, IMAGE_SIZE &PatH, IMAGE_SIZE &PatStep, IMAGE_SIZE &BitCount, IMAGE_PTR PatPtr)
{
	if ( CAOIFd::CheckPatternIdx(idx) == false ) { return false; }
	if ( NULL == PatPtr ) { return false; }	
	if ( NULL == m_FdPatternPtr[idx] ) { return false; }	
	const size_t BufferSize = m_FdPatternStep[idx]*m_FdPatternH[idx];
	::memcpy(PatPtr, m_FdPatternPtr[idx], sizeof(IMAGE_DATA)*BufferSize);

	PatW        = m_FdPatternW[idx];
	PatH        = m_FdPatternH[idx];	
	PatStep     = m_FdPatternStep[idx];
	BitCount    = m_FdBitCount[idx];
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::SaveFdPattern(unsigned int idx, LPCTSTR filename)//儲存定位點樣版圖檔
{
	if ( CAOIFd::CheckPatternIdx(idx) == false ) { return false; }
	if ( NULL == m_FdPatternPtr[idx] ) { return false; }	
	
	IMAGE_PTR PatPtr      = m_FdPatternPtr[idx];
	const IMAGE_SIZE PatW     = m_FdPatternW[idx];
	const IMAGE_SIZE PatH     = m_FdPatternH[idx];	
	const IMAGE_SIZE PatStep  = m_FdPatternStep[idx];
	const IMAGE_SIZE BitCount = m_FdBitCount[idx];

	if ( ImageAPI.SaveBMPImage(filename, PatW, PatH, PatStep, BitCount, PatPtr, true) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::LoadFdPattern(unsigned int idx, LPCTSTR filename)//載入定位點樣版圖檔
{
	if ( CAOIFd::CheckPatternIdx(idx) == false ) { return false; }	

	CDib dib;
	CAOIFd::ReleaseFdPatternBuffer(idx);
	if ( dib.Load(filename) == false )
	{	return false; }

	const char fnName[] = "CAOIFd::LoadFdPattern";

	size_t           i=0;
	IMAGE_PTR        PatPtr   = dib.GetDIBBits();
	const IMAGE_SIZE PatW     = dib.GetImageW();
	const IMAGE_SIZE PatH     = dib.GetImageH();
	const IMAGE_SIZE PatStep  = dib.GetImageBytePerLine();
	const IMAGE_SIZE BitCount = dib.GetImageBitCount();
	const size_t     BufferSize = ImageAPI.CalcBufferSize(PatStep, PatH);

	if ( (8!=BitCount) && (24!=BitCount) ) { return false; }

	if ( JetMemory.alloc_func(BufferSize, m_FdPatternPtr[idx], fnName, "m_FdPatternPtr") == false )
	{	return false; }
	
	if ( ImageAPI.ReverseImage(PatW, PatH, PatStep, BitCount, PatPtr, m_FdPatternPtr[idx]) == false )
	{
		JetMemory.free_func(m_FdPatternPtr[idx]);
		return false;
	}

	m_FdPatternW[idx] = PatW;
	m_FdPatternH[idx] = PatH;
	m_FdPatternStep[idx] = PatStep;
	m_FdBitCount[idx] = BitCount;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::FindFiducial()//尋找定位點
{
	const char fnName[] = "CAOIFd::FindFiducial";
	CAOIField* FieldPtr = GetRgnFieldPtr();
	if ( NULL == FieldPtr ) { return false; }	
	CAOIFrame *FramePtr = NULL;	
	const size_t FrameIdx = GetFdFrameIndex();	
	FramePtr = FieldPtr->GetFieldFramePtr(FrameIdx, true);
	if ( NULL == FramePtr ) { return false; }
	FRAME_TYPE   FrameType = FramePtr->GetFrameType();
	if ( FRAME_SPACE == FrameType ) { return false; }		

	CString    str;	
	BOOL       bSave=TRUE;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;	
	TREGION4D  ImageRgn;
	TREGION4D  FdRoiRgn;
	IMAGE_PTR  ImagePtr = NULL;
	const int  nAlign = 4;
	const CAMERA_ID CameraID = GetFdCameraID();
	const size_t FdIndex = GetFdIndex_Project();	
	const double FdSizeW = GetRgnRoiSizeW();//區域尺寸寬
	const double FdSizeH = GetRgnRoiSizeH();//區域尺寸長
	TSIZE2D  PatExtend = GetFdPatExtendSize();
	TSIZE2D  RoiSize = GetFdRoiSize();	
	TPOINT2D StagePos2D;
	TPOINT3D StagePos3D = FieldPtr->GetFieldStagePos();	
	FramePtr->GetFrameImagePtr(ImageW, ImageH, ImageStep, BitCount, ImagePtr);	

	StagePos2D.x = StagePos3D.x;
	StagePos2D.y = StagePos3D.y;
	FdRoiRgn.minX = StagePos3D.x-(RoiSize.cx/2);
	FdRoiRgn.maxX = StagePos3D.x+(RoiSize.cx/2);
	FdRoiRgn.minY = StagePos3D.y-(RoiSize.cy/2);
	FdRoiRgn.maxY = StagePos3D.y+(RoiSize.cy/2);
	AOIDataCollect.MapStageRegionToCamera(CameraID, FdRoiRgn, StagePos2D, ImageRgn);
	
	m_RgnFrameImageRect.left   = JetAPI::Floor(MIN(ImageRgn.minX, ImageRgn.maxX));
	m_RgnFrameImageRect.right  = JetAPI::Floor(MAX(ImageRgn.minX, ImageRgn.maxX));
	m_RgnFrameImageRect.top    = JetAPI::Floor(MIN(ImageRgn.minY, ImageRgn.maxY));
	m_RgnFrameImageRect.bottom = JetAPI::Floor(MAX(ImageRgn.minY, ImageRgn.maxY));

	IMAGE_SIZE RoiStep=0;
	IMAGE_SIZE RoiBitCount=0;
	IMAGE_PTR  RoiPtr=NULL;
	const IMAGE_SIZE RoiW=m_RgnFrameImageRect.right-m_RgnFrameImageRect.left;
	const IMAGE_SIZE RoiH=m_RgnFrameImageRect.bottom-m_RgnFrameImageRect.top;
#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		str.Format(_T("%s\\FdFov#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), FdIndex+1);
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	}
#endif//_DEBUG

	RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, nAlign);
	RoiBitCount=BitCount;
	if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, m_RgnFrameImageRect, RoiStep, RoiPtr, false) == false )
	{
		FramePtr->ClearFrameBuffer();
		return false;
	}

	if ( FRAME_BAYER == FrameType )
	{		
		IMAGE_SIZE   DeBayerBit=0;
		IMAGE_SIZE   DeBayerStep=0;
		IMAGE_PTR    DeBayerPtr=NULL;	
		BAYER_PATTERN_MODE BayerPattern = FramePtr->GetFrameBayerPattern();
		BAYER_PATTERN_MODE BayerRoi=ImageAPI.ShiftBayerPattern(BayerPattern, m_RgnFrameImageRect.left, m_RgnFrameImageRect.top);
		if ( AOIDataCollect.ExecDebayerImage(fnName, RoiW, RoiH, RoiStep, RoiPtr, BayerRoi, DeBayerStep, DeBayerBit, DeBayerPtr) == false)		
		{
			FramePtr->ClearFrameBuffer();			
			return false; 
		}
		JetMemory.free_func(RoiPtr);
		RoiPtr = DeBayerPtr;
		RoiBitCount = DeBayerBit;		
		RoiStep = DeBayerStep;
		DeBayerPtr = NULL;
	}	

	if ( CAOIFd::FindFiducial(RoiW, RoiH, RoiStep, RoiBitCount, RoiPtr) == false )
	{
		JetMemory.free_func(RoiPtr);
		FramePtr->ClearFrameBuffer();
		return false;	
	}
	JetMemory.free_func(RoiPtr);
	FramePtr->ClearFrameBuffer();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::FindFiducial(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr)//尋找定位點
{
	CString    str;
	int        ResCount = 0;
	size_t     i = 0;	
	IMAGE_SIZE PatW     = 0;
	IMAGE_SIZE PatH     = 0;	
	IMAGE_SIZE PatStep  = 0;
	IMAGE_SIZE PatBitCount = 0;
	IMAGE_PTR  PatPtr= NULL;
	
	IMAGE_PTR  PatSobelPtr= NULL;
	IMAGE_PTR  ImageSobelPtr = NULL;

	BOOL   bSave=TRUE;
	char   filename[256]="";
	double BestResultX=0;
	double BestResultY=0;
	double BestResultS=0;
	double BestResultA=0;
	double PatResultX[FD_MAX_PATTERN]={0};
	double PatResultY[FD_MAX_PATTERN]={0};
	double PatResultS[FD_MAX_PATTERN]={0};
	double PatResultA[FD_MAX_PATTERN]={0};
	LANE_ID  LaneID = GetFdLaneID();
	TPOINT3D TeachPt = GetFdTeachStagePos();
	const size_t FdIndex = GetFdIndex_Project();	
	const double MinScore = GetFdMinScore();
	const bool SobelMode = false;
	AOIDataCollect.MapStagePosLaneByLaneID(TeachPt.x, TeachPt.y, LaneID);
#ifdef _DEBUG
	if ( TRUE == bSave )
	{
		str.Format(_T("%s\\FdRoi#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), FdIndex+1);
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
	}
#endif//_DEBUG

	if ( true == SobelMode )
	{
		if ( ImageAPI.SobelImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, ImageSobelPtr) == false ) 
		{	return false; }	
	#ifdef _DEBUG
		if ( TRUE == bSave )
		{
			str.Format(_T("%s\\FdRoiSobel#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), FdIndex+1);
			ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImageSobelPtr, true);
		}
	#endif//_DEBUG
		ImagePtr = ImageSobelPtr;
	}

#ifndef OFFLINE_VERSION	
	CJetMatch     Match;	
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();
	if ( Match.SetMatchLibType(MatchLibType)==false )
	{	
		JetMemory.free_func(ImageSobelPtr);
		return false;	
	}
	Match.SetMatchDefaultParam();	
	for ( i=0; i<FD_MAX_PATTERN; i++ )
	{
		JetMemory.free_func(PatSobelPtr);
		if ( NULL == m_FdPatternPtr[i] ) { continue; }
		PatPtr      = m_FdPatternPtr[i];
		PatW        = m_FdPatternW[i];
		PatH        = m_FdPatternH[i];	
		PatStep     = m_FdPatternStep[i];
		PatBitCount = m_FdBitCount[i];
		if ( PatBitCount != BitCount ) { continue; }

	#ifdef _DEBUG
		if ( TRUE == bSave )
		{
			str.Format(_T("%s\\Fd%d_Pattern#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), FdIndex+1, i+1);
			ImageAPI.SaveBMPImage(str, PatW, PatH, PatStep, PatBitCount, PatPtr, true);
		}
	#endif//_DEBUG

		if ( true == SobelMode )
		{
			if ( ImageAPI.SobelImage(PatW, PatH, PatStep, PatBitCount, PatPtr, PatSobelPtr) == false ) 
			{	continue; }
		#ifdef _DEBUG
			if ( TRUE == bSave )
			{
				str.Format(_T("%s\\Fd%d_PatternSobel#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), FdIndex+1, i+1);
				ImageAPI.SaveBMPImage(str, PatW, PatH, PatStep, PatBitCount, PatSobelPtr, true);
			}
		#endif//_DEBUG
			PatPtr = PatSobelPtr;
		}

		//學習樣板
		if ( Match.LearnPattern(PatW, PatH, PatStep, PatBitCount, PatPtr, true) == false )			
		{	continue; }		
		if ( Match.GetPatternLearnt() == FALSE ) 
		{	continue; }

		Match.SetInterpolate(true);
		//尋找樣板
		if ( Match.Match(ImageW, ImageH, ImageStep, BitCount, ImagePtr, true) == false )
		{	continue; }

		ResCount = Match.GetNumPositions();
		if ( 0 == ResCount ) { continue; }

		PatResultX[i]=Match.GetResultPosX(0);
		PatResultY[i]=Match.GetResultPosY(0);
		PatResultS[i]=Match.GetResultScore(0);
		PatResultA[i]=Match.GetResultAngle(0);

		if ( PatResultS[i] > BestResultS )
		{
			BestResultS = PatResultS[i];
			BestResultX = PatResultX[i];
			BestResultY = PatResultY[i];
			BestResultA = PatResultA[i];			
		}		
	}
	JetMemory.free_func(PatSobelPtr);
	JetMemory.free_func(ImageSobelPtr);

	if ( BestResultS < 0.00001 ) 
	{
		this->SetFdStagePosX(TeachPt.x);
		this->SetFdStagePosY(TeachPt.y);
		this->LayoutRgnStageCornerPos();	
		return true; 
	}	

	TPOINT2D  RoiCp;
	TPOINT2D  ImageCP;
	TPOINT2D  ImagePt;
	TPOINT2D  StageCP;
	TPOINT2D  StagePt;	
	TRECT4D   ImageRect = this->GetRgnFrameImageRect();
	RECT      ImageRectInt;
	CAMERA_ID CameraID = this->GetRgnCameraID();
	
	JetAPI::Rect4DToRect(ImageRect, ImageRectInt);
	RoiCp.x = (ImageRectInt.right-ImageRectInt.left)/2;
	RoiCp.y = (ImageRectInt.bottom-ImageRectInt.top)/2;	
	ImageCP.x = (ImageRectInt.left+ImageRectInt.right)/2;
	ImageCP.y = (ImageRectInt.top+ImageRectInt.bottom)/2;
	ImagePt.x = BestResultX-RoiCp.x+ImageCP.x;
	ImagePt.y = BestResultY-RoiCp.y+ImageCP.y;
	StageCP.x = this->GetRgnFovStagePosX();
	StageCP.y = this->GetRgnFovStagePosY();
	if ( AOIDataCollect.MapCameraPtToStage(CameraID, ImagePt, StageCP, StagePt) == false ) 
	{	return false; }
	
	RESULT_ID ResultID;
	if ( BestResultS < MinScore )
	{	ResultID = RESULT_ID_NG;	}
	else
	{	ResultID = RESULT_ID_OK;	}
	SetFdResultID_AOI(ResultID);
	SetFdResultID_Alarm(ResultID);
	this->SetFdStagePosX(StagePt.x);
	this->SetFdStagePosY(StagePt.y);
	this->LayoutRgnStageCornerPos();
#else	
	this->SetFdStagePosX(TeachPt.x);
	this->SetFdStagePosY(TeachPt.y);
	this->LayoutRgnStageCornerPos();	
#endif// OFFLINE_VERSION

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::WriteFdFile(CAOIFileIO &FileIO)//儲存定位點
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	
	CAOIFd    *pFd = this;
	CString    FdModelFolder;
	char       uuidStr[MAX_JET_PATH]="";	
	wchar_t    uuidWStr[MAX_JET_PATH]=L"";	
	UUID       uuid = pFd->GetObjUuid();	
	CAOIModel *ModelPtr = pFd->GetFdModelPtr();	
	FileIO.SetFnName(_T("CAOIFd::WriteFdFile"));
	JetAPI::UUIDToStringA(uuid, uuidStr);
	JetAPI::UUIDToStringW(uuid, uuidWStr);
	//----------------------------------------------------------------------------------------//
	//定位點參數
	if ( FileIO.SaveChunk_INT(FILE_IO_FD_START, 0) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_FD_UNIQUE_ID, pFd->GetFdUniqueID()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_FD_INDEX_PROJECT, pFd->GetFdIndex_Project()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_FD_PANEL_INDEX, pFd->GetFdPanelIndex_Project()) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_FD_BOARD_INDEX, pFd->GetFdBoardIndex_Project()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_FD_FRAME_UNIQUE_ID, pFd->GetFdFrameUniqueID()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_FD_FIELD_INDEX_RANDOM, pFd->GetFdFieldIndex()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_ANGLE, pFd->GetFdAngle()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_BODY_SIZE_CX, pFd->GetFdBodySizeW()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_BODY_SIZE_CY, pFd->GetFdBodySizeH()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_ROI_SIZE_CX, pFd->GetFdRoiSizeW()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_ROI_SIZE_CY, pFd->GetFdRoiSizeH()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_CAD_POS_X, pFd->GetFdCadPosX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_CAD_POS_Y, pFd->GetFdCadPosY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_STAGE_TEACH_POS_X, pFd->GetFdTeachStagePosX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_STAGE_TEACH_POS_Y, pFd->GetFdTeachStagePosY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_STAGE_TEACH_POS_Z, pFd->GetFdTeachStagePosZ()) == false ) { return false; }	
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_STAGE_RESULT_POS_X, pFd->GetFdStagePosX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_STAGE_RESULT_POS_Y, pFd->GetFdStagePosY()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_STAGE_RESULT_POS_Z, pFd->GetFdStagePosZ()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_PATTERN_EXTEND_ROI_CX, pFd->GetFdPatExtendSizeW()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_PATTERN_EXTEND_ROI_CY, pFd->GetFdPatExtendSizeH()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_ROI_EXTEND_ROI_CX, pFd->GetFdRoiExtendSizeW()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_ROI_EXTEND_ROI_CY, pFd->GetFdRoiExtendSizeH()) == false ) { return false; }	
	if ( FileIO.GetSaveWStr() == true ) 
	{	if ( FileIO.SaveChunk_STR(FILE_IO_FD_OBJ_UUID, uuidWStr) == false ) { return false; } }
	else
	{	if ( FileIO.SaveChunk_STR(FILE_IO_FD_OBJ_UUID, uuidStr) == false ) { return false; } }	
	if ( FileIO.SaveChunk_INT(FILE_IO_FD_SORT_ID, pFd->GetFdSortID()) == false ) { return false; }		
	if ( FileIO.SaveChunk_INT(FILE_IO_FD_DISTRICT_ID, pFd->GetFdDistrictID()) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_FD_GROUP_ID, pFd->GetFdGroupID()) == false ) { return false; }

	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_CAD_SPECIAL_POS_X, pFd->GetFdSpecialCadPosX()) == false ) { return false; }
	if ( FileIO.SaveChunk_DBL(FILE_IO_FD_CAD_SPECIAL_POS_Y, pFd->GetFdSpecialCadPosY()) == false ) { return false; }

	//定位點的模組節點
	FdModelFolder = ModelPtr->GetModelFolderModel();
	if ( FileIO.SaveChunk_INT(FILE_IO_FD_MODEL_NODE, 0) == false ) { return false; }
	if ( ModelPtr->WriteModelFile(FileIO) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_FD_END, 0) == false ) { return false; }

	//儲存圖檔	
	unsigned int  i = 0;
	unsigned char *PatPtr = NULL;
	const unsigned int  FdIndex = pFd->GetFdIndex_Project();
	IMAGE_SIZE   PatW=0, PatH=0, PatStep=0, BitCount = 0;
	CString FdNameOrg, FdNameMsk, FdNameOld;
	const unsigned int  FdPatCount = FD_MAX_PATTERN;
	for ( i=0; i<FdPatCount; i++ )
	{		
		FdNameOrg = AOIDataDefine.GetFdPatternName(FileIO.GetFileFolder(), FdIndex, i, false);
		FdNameMsk = AOIDataDefine.GetFdPatternName(FileIO.GetFileFolder(), FdIndex, i, true);

		if ( pFd->GetFdPattern(i, PatW, PatH, PatStep, BitCount, PatPtr) == false ) 
		{
			::DeleteFile(FdNameOrg);
			::DeleteFile(FdNameMsk);
			continue; 
		}		
		if ( NULL == PatPtr ) 
		{ 
			::DeleteFile(FdNameOrg);
			::DeleteFile(FdNameMsk);
			continue; 
		}
		//遮罩圖
		if ( ImageAPI.SaveBMPImage(FdNameMsk, PatW, PatH, PatStep, BitCount, PatPtr, true) == false )
		{
			FileIO.SetErrorString(ImageAPI.GetImageApiErrorString());			
			return false;
		}

		FdNameOld = pFd->GetFdImageNameORG(i);
		if ( JetAPI::IsFileExist(FdNameOld) == true )
		{	::CopyFile(FdNameOld, FdNameOrg, false);	}
		else
		{	::CopyFile(FdNameMsk, FdNameOrg, false);	}
		pFd->SetFdImageNameORG(i, FdNameOrg);
		pFd->SetFdImageNameMSK(i, FdNameMsk);

		PatPtr = NULL;
		PatW = PatH = PatStep = BitCount = 0;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::ReadFdFile(CAOIFileIO &FileIO)//載入定位點
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	FileIO.SetFnName(_T("CAOIFd::ReadFdFile"));

	CString      str;
	UUID         uuid;
	int          index = 0;	
	int          PanelIndex = 0;			
	int          FdUniqueID = -1;	
	TPOINT3D     StagePos;
	CAOIFd      *pFd = this;
	CAOIWnd     *WndPtr = NULL;
	CAOIModel   *ModelPtr = pFd->GetFdModelPtr();	
	//----------------------------------------------------------------------------------------//	
	unsigned int  i = 0;
	unsigned int  FdIndex = 0;	
	CString FdNameOrg, FdNameMsk, FdFolder, FdModelFolder, ModelFolder, ModelName;
	const unsigned int FdPatCount = FD_MAX_PATTERN;
	const bool bCheckPattrn = false;
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		
		switch ( index )
		{
		case FILE_IO_FD_START://定位點參數-起點
			break;		
		case FILE_IO_FD_END://定位點參數-終點
			//載入樣板圖檔
			FdUniqueID = pFd->GetFdUniqueID();
			FdIndex = pFd->GetFdIndex_Project();
			if ( FdUniqueID >= 0 ) 
			{
				FdFolder = FileIO.GetFdFolder();
				ModelFolder = ModelPtr->GetModelFolder();//for Debug				
				FdModelFolder = AOIDataDefine.GetFdModelFolder(FdFolder, FdUniqueID);
				JetAPI::CreateFolder(FdModelFolder);
				JetAPI::ExtractTopFolder(FdModelFolder, ModelName);								
				ModelPtr->SetModelName(ModelName);
				ModelPtr->SetModelFolderModel(FdModelFolder);
				ModelPtr->AssignModelFolder();
				ModelPtr->UnSelectModel();
				WndPtr = pFd->GetFdWndPtr();
				if ( NULL != WndPtr )
				{	WndPtr->SetWndSelected(true);	}
				ModelPtr->SetModelWndActived(WndPtr);		
			}

			for ( i=0; i<FdPatCount; i++ )
			{
				FdNameOrg = AOIDataDefine.GetFdPatternName(FileIO.GetFileFolder(), FdIndex, i, false);
				FdNameMsk = AOIDataDefine.GetFdPatternName(FileIO.GetFileFolder(), FdIndex, i, true);
				if ( JetAPI::IsFileExist(FdNameOrg) == false )
				{
					if ( 0==i && true==bCheckPattrn )
					{
						str.Format(_T("Error, Load Fd Image Fault(Fd:%d, FileName:%s)"), FdIndex+1, FdNameOrg);
						FileIO.SetErrorString(str);
						return false;
					}
					else
					{	continue;  }
				}
				if ( JetAPI::IsFileExist(FdNameMsk) == false )
				{	::CopyFile(FdNameOrg, FdNameMsk, false);	}
				if ( pFd->LoadFdPattern(i, FdNameMsk) == false )
				{
					str.Format(_T("Error, Load Fd Image Fault(Fd:%d, FileName:%s)"), FdIndex+1, FdNameMsk);
					FileIO.SetErrorString(str);
					return false;
				}
				pFd->SetFdImageNameORG(i, FdNameOrg);
				pFd->SetFdImageNameMSK(i, FdNameMsk);
			}			
			pFd->CalcFdCadCornerPos();
			pFd->LayoutFdStageCornerPos();
			pFd->UpdateFdParamToModel();
			pFd->SetFdResultID_AOI(RESULT_ID_OK);
			return true;
			break;
		case FILE_IO_FD_UNIQUE_ID:			
			pFd->SetFdUniqueID(FileIO.GetData_INT());
			break;
		case FILE_IO_FD_INDEX_PROJECT:
			pFd->SetFdIndex_Project(FileIO.GetData_INT());
			break;
		case FILE_IO_FD_PANEL_INDEX:
			pFd->SetFdPanelIndex_Project(FileIO.GetData_INT());
			break;
		case FILE_IO_FD_BOARD_INDEX:
			pFd->SetFdBoardIndex_Project(FileIO.GetData_INT());
			break;
		case FILE_IO_FD_FRAME_UNIQUE_ID:
			pFd->SetFdFrameUniqueID(FileIO.GetData_INT());
			break;
		case FILE_IO_FD_FIELD_INDEX_RANDOM:
			pFd->SetFdFieldIndex(FileIO.GetData_INT());
			break;
		case FILE_IO_FD_ANGLE:			
			pFd->SetFdAngle(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_BODY_SIZE_CX:
			pFd->SetFdBodySizeW(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_BODY_SIZE_CY:
			pFd->SetFdBodySizeH(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_ROI_SIZE_CX:
			pFd->SetFdRoiSizeW(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_ROI_SIZE_CY:
			pFd->SetFdRoiSizeH(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_CAD_POS_X:
			pFd->SetFdCadPosX(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_CAD_POS_Y:
			pFd->SetFdCadPosY(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_STAGE_TEACH_POS_X:
			pFd->SetFdTeachStagePosX(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_STAGE_TEACH_POS_Y:
			pFd->SetFdTeachStagePosY(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_STAGE_TEACH_POS_Z:
			pFd->SetFdTeachStagePosZ(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_STAGE_RESULT_POS_X:
			pFd->SetFdStagePosX(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_STAGE_RESULT_POS_Y:
			pFd->SetFdStagePosY(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_STAGE_RESULT_POS_Z:
			pFd->SetFdStagePosZ(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_PATTERN_EXTEND_ROI_CX:
			pFd->SetFdPatExtendSizeW(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_PATTERN_EXTEND_ROI_CY:
			pFd->SetFdPatExtendSizeH(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_ROI_EXTEND_ROI_CX:
			pFd->SetFdRoiExtendSizeW(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_ROI_EXTEND_ROI_CY:
			pFd->SetFdRoiExtendSizeH(FileIO.GetData_DBL());
			break;		
		case FILE_IO_FD_OBJ_UUID:
			if ( FileIO.GetLoadWStr()==true )
			{
				if ( JetAPI::UUIDFromStringW(FileIO.GetData_WSTR(), uuid) == true )
				{	pFd->SetObjUuid(uuid);	}
			}
			else
			{
				if ( JetAPI::UUIDFromStringA(FileIO.GetData_STR(), uuid) == true )
				{	pFd->SetObjUuid(uuid);	}
			}
			break;		
		case FILE_IO_FD_SORT_ID://定位點參數-的排序編號
			pFd->SetFdSortID(FileIO.GetData_INT());
			break;
		case FILE_IO_FD_DISTRICT_ID://定位點參數-分段編號
			pFd->SetFdDistrictID((DISTRICT_ID)(FileIO.GetData_INT()));
			break;
		case FILE_IO_FD_GROUP_ID://定位點參數-群組編號
			pFd->SetFdGroupID(FileIO.GetData_INT());
			break;
		case FILE_IO_FD_CAD_SPECIAL_POS_X:
			pFd->SetFdSpecialCadPosX(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_CAD_SPECIAL_POS_Y:
			pFd->SetFdSpecialCadPosY(FileIO.GetData_DBL());
			break;
		case FILE_IO_FD_MODEL_NODE:
			pFd->GetFdTeachStagePos(StagePos);
			pFd->SetFdStagePos(StagePos);
			if ( ModelPtr->ReadModelFile(FileIO) == false )
			{	return false;	}
			break;
		default:
			break;
		}
	};		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::WriteFdSpcFile_JSON_VRS(FILE *pfile, CAOIProject *ProjectPtr)
{
	if ( NULL == pfile ) { return false; }
	if ( NULL == ProjectPtr ) { return false; }

	CAOIFd  *FdPtr=this;
	if ( NULL == FdPtr ) { return false; }	
	
	size_t           j=0;
	CString          strText;
	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";
	CAOIWnd         *WndPtr = NULL;
	CAOIBox         *BoxPtr = NULL;
	CAOILand        *LandPtr = NULL;
	CAOIModel       *ModelPtr = NULL;
	double			 FdScore=0;
	double			 FdTeachX=0, FdTeachY=0;
	double			 FdStageX=0, FdStageY=0;
	double			 FdOffsetX=0, FdOffsetY=0;
	const bool       SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool       SignY = AOIDataCollect.GetStageSignPositiveY();

	LANE_ID    LaneID = ProjectPtr->GetProjectActLaneID();	

	ModelPtr = FdPtr->GetFdModelPtr();
	WndPtr = ModelPtr->GetModelWndPtr(0, true);
	if ( NULL == WndPtr )
	{	FdScore = 0; }
	else
	{	FdScore = WndPtr->GetWndAlgParam().GetAlgPatternSimilarityReading(); }
	FdTeachX = FdPtr->GetFdTeachStagePosX();
	FdTeachY = FdPtr->GetFdTeachStagePosY();
	AOIDataCollect.MapStagePosLaneByLaneID(FdTeachX, FdTeachY, LaneID);	
	FdStageX = FdPtr->GetFdStagePosX();
	FdStageY = FdPtr->GetFdStagePosY();
	if ( true == SignX )
	{	FdOffsetX=FdStageX-FdTeachX; }
	else
	{	FdOffsetX=FdTeachX-FdStageX; }
	if ( true == SignY )
	{	FdOffsetY=FdStageY-FdTeachY; }
	else
	{	FdOffsetY=FdTeachY-FdStageY; }			

	::wcscpy(strTag, L"Offset_X");			
	::fwprintf(pfile, L"      \"%s\": %.2f,\n", strTag, FdOffsetX);

	::wcscpy(strTag, L"Offset_Y");			
	::fwprintf(pfile, L"      \"%s\": %.2f,\n", strTag, FdOffsetY);

	::wcscpy(strTag, L"Value");			
	::fwprintf(pfile, L"      \"%s\": %.2f\n", strTag, FdScore);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::WriteFdSpcFile_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr)
{
	if ( NULL == pfile ) { return false; }
	if ( NULL == ProjectPtr ) { return false; }

	CAOIFd  *FdPtr=this;
	if ( NULL == FdPtr ) { return false; }	
	
	size_t           j=0;
	CString          strText;
	const size_t     szBuffer = 256;
	wchar_t          strTag[szBuffer] = L"";
	wchar_t          strBuffer[szBuffer] = L"";
	CAOIWnd         *WndPtr = NULL;
	CAOIBox         *BoxPtr = NULL;
	CAOILand        *LandPtr = NULL;
	CAOIModel       *ModelPtr = NULL;
	double			 FdScore=0;
	double			 FdTeachX=0, FdTeachY=0;
	double			 FdStageX=0, FdStageY=0;
	double			 FdOffsetX=0, FdOffsetY=0;
	const bool       SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool       SignY = AOIDataCollect.GetStageSignPositiveY();

	LANE_ID    LaneID = ProjectPtr->GetProjectActLaneID();	
	//::fwprintf(pfile, L"      \"ID\": %d,\n", j+1);
			
	ModelPtr = FdPtr->GetFdModelPtr();
	WndPtr = ModelPtr->GetModelWndPtr(0, true);
	if ( NULL == WndPtr )
	{	FdScore = 0; }
	else
	{	FdScore = WndPtr->GetWndAlgParam().GetAlgPatternSimilarityReading(); }
	FdTeachX = FdPtr->GetFdTeachStagePosX();
	FdTeachY = FdPtr->GetFdTeachStagePosY();
	AOIDataCollect.MapStagePosLaneByLaneID(FdTeachX, FdTeachY, LaneID);	
	FdStageX = FdPtr->GetFdStagePosX();
	FdStageY = FdPtr->GetFdStagePosY();
	if ( true == SignX )
	{	FdOffsetX=FdStageX-FdTeachX; }
	else
	{	FdOffsetX=FdTeachX-FdStageX; }
	if ( true == SignY )
	{	FdOffsetY=FdStageY-FdTeachY; }
	else
	{	FdOffsetY=FdTeachY-FdStageY; }			

	::wcscpy(strTag, L"Offset_X");			
	::fwprintf(pfile, L"      \"%s\": %.2f,\n", strTag, FdOffsetX);

	::wcscpy(strTag, L"Offset_Y");			
	::fwprintf(pfile, L"      \"%s\": %.2f,\n", strTag, FdOffsetY);

	::wcscpy(strTag, L"Value");			
	::fwprintf(pfile, L"      \"%s\": %.2f\n", strTag, FdScore);

	return true;	
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIFd::GetFdRgnCad() const//定位點範圍-Cad
{
	TREGION4D Region;
	JetAPI::PointsToRegion(m_RgnRoiCadCornerPos, 4, Region);
	return Region;	
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIFd::GetFdRgnStage() const//定位點範圍-Stage
{
	TREGION4D Region;
	JetAPI::PointsToRegion(m_RgnRoiStageCornerPos, 4, Region);
	return Region;
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIFd::GetFdExtendRgnCad() const//定位點搜尋範圍-Cad
{
	TREGION4D Region;
	TPOINT2D  CadCornerPt[4];
	CAOIFd::CalcFdExtendCadCornerPos(CadCornerPt);	
	JetAPI::PointsToRegion(CadCornerPt, 4, Region);
	return Region;
}
//-------------------------------------------------------------------------------------//
TREGION4D CAOIFd::GetFdExtendRgnStage() const//定位點搜尋範圍-Stage
{
	TREGION4D Region;
	TPOINT2D  StageCornerPt[4];
	CAOIFd::CalcFdExtendStageCornerPos(StageCornerPt);	
	JetAPI::PointsToRegion(StageCornerPt, 4, Region);
	return Region;
}
//-------------------------------------------------------------------------------------//
void CAOIFd::GetFdRoiCadRegion(TREGION4D &Region)//取得定位點在Cad的範圍	
{
	JetAPI::PointsToRegion(m_RgnRoiCadCornerPos, 4, Region);
}
//-------------------------------------------------------------------------------------//
void CAOIFd::GetFdBodyCadRegion(TREGION4D &Region)//取得定位點在Cad的範圍	
{
	JetAPI::PointsToRegion(m_RgnBodyCadCornerPos, 4, Region);
}
//-------------------------------------------------------------------------------------//
void CAOIFd::GetFdRoiStageRegion(TREGION4D &Region)//取得定位點在Stage的範圍	
{
	JetAPI::PointsToRegion(m_RgnRoiStageCornerPos, 4, Region);	
}
//-------------------------------------------------------------------------------------//
void CAOIFd::GetFdBodyStageRegion(TREGION4D &Region)//取得定位點在Stage的範圍		
{
	JetAPI::PointsToRegion(m_RgnBodyStageCornerPos, 4, Region);
}
//-------------------------------------------------------------------------------------//
void CAOIFd::GetFdExtendCadRegion(TREGION4D &Region)//取得定位點在Cad的搜尋範圍
{
	TPOINT2D  CadCornerPt[4];
	CAOIFd::CalcFdExtendCadCornerPos(CadCornerPt);	
	JetAPI::PointsToRegion(CadCornerPt, 4, Region);
}
//-------------------------------------------------------------------------------------//
void CAOIFd::GetFdExtendStageRegion(TREGION4D &Region)//取得定位點在Stage的搜尋範圍		
{
	TPOINT2D  StageCornerPt[4];
	CAOIFd::CalcFdExtendStageCornerPos(StageCornerPt);	
	JetAPI::PointsToRegion(StageCornerPt, 4, Region);
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::CheckFdBePickByCad(const TPOINT2D &PickPos)//確認定位點被點擊到
{
	TREGION4D    Region = CAOIFd::GetFdRgnCad();
	if ( PickPos.x<Region.minX || PickPos.y<Region.minY || 
		 PickPos.x>Region.maxX || PickPos.y>Region.maxY )
	{	return false; }
	return true; 	
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::CheckFdBePickByStage(const TPOINT2D &PickPos)//確認定位點被點擊到
{
	TREGION4D    Region = CAOIFd::GetFdRgnStage();		
	if ( PickPos.x<Region.minX || PickPos.y<Region.minY || 
		 PickPos.x>Region.maxX || PickPos.y>Region.maxY )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::CheckFdInRegionByCad(const TREGION4D &SelRgn, bool bEntireIn)//確認定位點在範圍內
{
	TREGION4D    Region = CAOIFd::GetFdRgnCad();		
	return JetAPI::CheckRgnInRegion(Region, SelRgn, bEntireIn);
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::CheckFdInRegionByStage(const TREGION4D &SelRgn, bool bEntireIn)//確認定位點在範圍內
{
	TREGION4D    Region = CAOIFd::GetFdRgnStage();		
	return JetAPI::CheckRgnInRegion(Region, SelRgn, bEntireIn);
}
//-------------------------------------------------------------------------------------//
CString CAOIFd::GetRgnDerivedName() const//取得定位點的名稱
{
	return AOIDataDefine.GetFdFullName(m_FdUniqueID, _T("Fd"));
}
//-------------------------------------------------------------------------------------//
CString CAOIFd::GetRgnDerivedKeyName() const//取得定位點的名稱	
{
	return CString(_T("Fd"));
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::ExtractRgnDerivedFrame(bool &Finished)//挖取定位點圖片
{
	if ( CAOIRgn::ExtractRgnFrame(Finished) == false ) { return false; }

	if ( true == Finished )
	{	
		bool IsOK = true;
		IsOK = CAOIFd::ExecFdInspection();
		ClearRgnMaskBuffer_Base();
		if ( GetFdKeepImage() == false )
		{	CAOIRgn::ClearRgnImageBuffer();		}
		if ( false == IsOK )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::ExecRgnDerivedInspection()//執行定位點檢測
{
	return ExecFdInspection();	
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::CreateFdSubRgnList(FIELD_BUILD_MODE BuildMode, bool ByCadRegion)//建立定位點子檢測區域列表
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
void CAOIFd::ClearFdSubRgnList()//清除定位點的子列表
{
	CAOIRgn::ClearRgnSubList();
}
//-------------------------------------------------------------------------------------//
size_t CAOIFd::GetFdSubRgnCount() const//取得定位點的子數量
{
	return CAOIRgn::m_RgnSubList.size();
}
//-------------------------------------------------------------------------------------//
CAOIRgn* CAOIFd::GetFdSubRgnPtr(size_t index, bool check) const//取得定位點的子指標
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
CAOIModel* CAOIFd::GetFdModelPtr()//取得定位點模組
{
	return &m_FdModel;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::UpdateFdParamToModel()//更新定位點參數至模組內
{
	TPOINT2D PosCad, PosStage;	
	double SizeW = CAOIFd::GetFdRoiSizeW();
	double SizeH = CAOIFd::GetFdRoiSizeH();
	double BodyW = CAOIFd::GetFdBodySizeW();
	double BodyH = CAOIFd::GetFdBodySizeH();
	double ModelBodySizeW = BodyW;
	double ModelBodySizeH = BodyH;
	const double Angle = CAOIFd::GetFdAngle();
	const double CadPosX = CAOIFd::GetFdCadPosX();
	const double CadPosY = CAOIFd::GetFdCadPosY();
	const double StagePosX = CAOIFd::GetFdStagePosX();
	const double StagePosY = CAOIFd::GetFdStagePosY();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(Angle);
	CAOIBox *BoxPtr = m_FdModel.GetModelBodyBoxPtr();
	
	PosCad.x = CadPosX;
	PosCad.y = CadPosY;
	PosStage.x = StagePosX;
	PosStage.y = StagePosY;
	
	JetAPI::RotateSize(Angle, ModelBodySizeW, ModelBodySizeH);
	BoxPtr->SetBoxSize(ModelBodySizeW, ModelBodySizeH, true);

	if ( false == IsExceptionAngle )
	{	m_FdModel.SetModelAttachedAngle(Angle);	}
	else
	{	
		m_FdModel.SetModelAttachedAngle(0);
		m_FdModel.RotateModel(Angle, 0, 0);
	}		
	m_FdModel.SetModelFdPtr(this);
	m_FdModel.SetModelAttachedPosCad(PosCad);
	m_FdModel.SetModelAttachedPosStage(PosStage);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::UpdateFdModelFromLibrary(CAOIModel *RefModelPtr)//更新定位點模組
{
	CAOIFd *FdPtr = this;
	if ( NULL == FdPtr ) { return false; }
	if ( NULL == RefModelPtr ) { return false; }

	const bool bClearDst=true;
	const bool bClearSrc=false;	
	CAOIModel  &ModelObj=m_FdModel;	
	CString ModelName = ModelObj.GetModelName();
	CString ModelFolder = ModelObj.GetModelFolder();
	CString RefModelFolder = RefModelPtr->GetModelFolder();			
	if ( ModelFolder.CompareNoCase(RefModelFolder) != 0 ) 
	{	JetAPI::CopyFolderAToFolderB(RefModelFolder, ModelFolder, bClearSrc, bClearDst, _T(""), -1, -1);	}	

	const double FdAngle = GetFdAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(FdAngle);
	
	ModelObj = *RefModelPtr;
	ModelObj.SetModelAttachedAngle(0);	
	ModelObj.RotateModel(FdAngle, 0, 0);

	ModelObj.SetModelFdPtr(this);
	ModelObj.SetModelName(ModelName);
	ModelObj.SetModelFolderModel(ModelFolder);
	ModelObj.AssignModelFolder();	
	ModelObj.UpdateModelBodyToFd();	

	const TNoiseFilterParam &NoiseFilterParam = GetFdSpaceNoiseFilterParam();
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

	const double PanelBasePlane=GetFdPanelBasePlane();
	ModelObj.SetModelPanelBasePlane(PanelBasePlane);	

	const bool DataModelEnabled=GetFdDataModelEnabled();
	ModelObj.SetModelDataModelEnabled(DataModelEnabled);
	const int DataModelLevelID=GetFdDataModelLevelID();
	ModelObj.SetModelDataModelLevelID(DataModelLevelID);
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIFd::InitFdInspection(LANE_ID LaneID)//初始化定位點檢測
{	
	TREGION4D ModelBodyRgn;
	TREGION4D ModelTotalRgn;
	const bool RightSide = true;
	double StageX = GetFdTeachStagePosX();
	double StageY = GetFdTeachStagePosY();
	double StageZ = GetFdTeachStagePosZ();		
	AOIDataCollect.MapStagePosLaneByLaneID(StageX, StageY, LaneID);
	StageZ = MotionCtrlPtr->GetMotionPCBStopPosZ(LaneID, RightSide);

	SetFdLaneID(LaneID);	
	ClearRgnImageBuffer();
	SetFdKeepImage(false);
	SetFdStagePosX(StageX);
	SetFdStagePosY(StageY);
	SetFdStagePosZ(StageZ);
	SetFdConfirmUIResultID(0);
	SetFdFillImageTime(0.0);
	SetFdModelImageIsSaved(false);
	SetFdResultID_AOI(RESULT_ID_NONE);
	SetFdResultID_Alarm(RESULT_ID_NONE);
	UpdateFdResultID_AOI_Lane(LaneID);	

	//定位點的3D處理需要降低
	double PanelBasePlane=GetFdPanelBasePlane();
	TNoiseFilterParam &NoiseFilterParam = GetRgnSpaceNoiseFilterParam();
	
	NoiseFilterParam.DataVoidExpandEnabled = false;
	NoiseFilterParam.DataFirstFilterMode = DATA_NF_DISABLE;
	NoiseFilterParam.DataOverLowFTMode = DATA_NF_DISABLE;
	NoiseFilterParam.DataHeightFTMode = DATA_NF_DISABLE;
	NoiseFilterParam.DataVoidReContructed = false;
	NoiseFilterParam.DataFinalFilterMode = DATA_NF_DISABLE;
	NoiseFilterParam.DataFinalFilterMode2 = DATA_NF_DISABLE;
	NoiseFilterParam.BasePlaneParam.PanelBasePlane=PanelBasePlane;

	m_FdModel.SetModelPanelBasePlane(PanelBasePlane);
	m_FdModel.SetModelSpaceBasePlaneParam(NoiseFilterParam.BasePlaneParam);
	m_FdModel.SetModelSpaceNoiseFilterParam(NoiseFilterParam);

	/*
	//以下先拿掉, 定位點有旋轉角度的需求, 因此定位點寬長是有變化
	m_FdModel.CalcModelTotalRegionAll();
	m_FdModel.GetModelTotalRegion(ModelTotalRgn);
	m_FdModel.GetModelBodyBox().GetBoxRegion(ModelBodyRgn);	
	const double BodySizeW = ModelBodyRgn.GetWidth();
	const double BodySizeH = ModelBodyRgn.GetHeight();
	const double TotalSizeW = ModelTotalRgn.GetWidth();
	const double TotalSizeH = ModelTotalRgn.GetHeight();

	SetFdRoiExtendSizeW(TotalSizeW);
	SetFdRoiExtendSizeH(TotalSizeH);
	CalcRgnCadCornerPos();	
	*/
	LayoutFdStageCornerPos();	
	m_FdModel.InitModelInspection(false);	
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::ExecFdInspection()//執行定位點檢測
{
	std::vector<TUNI_FRAME> UniFrameList;
	if ( CAOIRgn::GetRgnUniFrameList(UniFrameList) == false ) { return false; }	
	CAOIModel *ModelPtr = GetFdModelPtr();
	CAOIProject *ProjectPtr = GetFdProjectPtr();
	const size_t UniFrameCount = UniFrameList.size();
	if ( 0==UniFrameCount || NULL==ProjectPtr ) 
	{
		LANE_ID LaneID = GetFdLaneID();
		RESULT_ID ResultID = RESULT_ID_EXCEPTION;
		SetFdResultID_AOI(ResultID);
		SetFdResultID_Alarm(ResultID);
		UpdateFdResultID_AOI_Lane(LaneID);
		ModelPtr->SetModelResultID(ResultID);
		ModelPtr->SetModelResultID_Alarm(ResultID);		
		return true;		 
	}
	
	CString    FdName;
	CString    ModelName;		
	const unsigned int FdIndex = GetFdIndex_Project();
	ModelName = ModelPtr->GetModelName();
	FdName = AOIDataDefine.GetFdFullName(FdIndex, _T("Fd"));	

#ifdef _DEBUG
	BOOL bSave = FALSE;//TRUE, FALSE
	if ( bSave == TRUE )
	{
		size_t     i=0;
		CString    str;		
		CString    AttachedName;
		TUNI_FRAME UniFrame;
		AttachedName = FdName;
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
	const double FdW = GetFdRoiSizeW();
	const double FdH = GetFdRoiSizeH();
	AOIDataCollect.MapImageSizeToReal(CameraID, ImageW, ImageH, ImageSizeUm);
	SetFdFrameImageSize_um(ImageSizeUm);//修正正跨FOV的尺寸

	ModelPtr->GetModelTotalRegion(rgnModel);
	const double ModelW = rgnModel.GetWidth();
	const double ModelH = rgnModel.GetHeight();
	ModelPtr->ExecModelInspection(UniFrameList);	
	UpdateFdResultID();	
	RESULT_ID FdResultID = GetFdResultID_AOI();
	if ( RESULT_ID_OK == FdResultID )
	{	CalcFdSpaceBasePlane(UniFrameList); }
	ModelPtr->CalcModelImageRect_CustomerAI(ImageW, ImageH);
	ExecFdSaveDefectImage(UniFrameList);//執行零件儲存瑕疵圖片
	SetFdKeepImage(true);	
	const bool FdNeedCalcMap = GetFdNeedCalcMap();
	if ( false == FdNeedCalcMap )
	{	return true;	}

	//鍵入關鍵區間, 並且判定是否全部都算完
	CString str;
	CAOIFd::LockFd();	
	size_t     FdNGCount = 0;
	size_t     PanelFdCount=0;
	size_t     BoardFdCount=0;
	size_t     FdNGSkipCount = 0;
	CAOIPanel *PanelPtr = GetFdPanelPtr();
	CAOIBoard *BoardPtr = GetFdBoardPtr();	
	DISTRICT_ID DistrictID = GetFdDistrictID();
	LANE_ID     LaneID = ProjectPtr->GetProjectActLaneID();
	const TProjectParameter &ProjectParam = ProjectPtr->GetProjectParameter();
	const size_t PanelFdNGSkipCount = ProjectParam.m_PanelFdNGSkipCount;	
	const size_t BoardFdNGSkipCount = ProjectParam.m_BoardFdNGSkipCount;		
	FD_NG_HANDLE_MODE PanelFdNGHandleMode = ProjectParam.m_PanelFdNGHandleMode;	
	FD_NG_HANDLE_MODE BoardFdNGHandleMode = ProjectParam.m_BoardFdNGHandleMode;	
	if ( NULL != PanelPtr )
	{	
		const unsigned int PanelIndex = PanelPtr->GetPanelIndex_Project();
		if ( NULL == BoardPtr )
		{	
			if ( PanelPtr->GetPanelCalcMapFinish() == true ) 
			{	
				CAOIFd::UnlockFd();
				return true; 
			}

			if ( PanelPtr->CheckPanelFdCalculated(DistrictID) == false )
			{
				CAOIFd::UnlockFd();
				return true; 
			}		

			if ( FD_NG_HANDLE_XBOARD == PanelFdNGHandleMode )
			{	
				PanelFdCount = PanelPtr->CalcPanelFdCount(DistrictID);
				FdNGCount = PanelPtr->CalcPanelFdDefectCount(DistrictID);
				FdNGSkipCount = MIN(PanelFdCount, PanelFdNGSkipCount);
				if ( FdNGCount >= FdNGSkipCount )
				{
					PanelPtr->ExecPanelBeXBoard(LaneID);
					PanelPtr->SetPanelCalcMapFinish(true);
					CAOIFd::UnlockFd();
					return true; 
				}
			}
			//建立座標轉換公式
			if ( PanelPtr->CalcPanelMapParam(DistrictID) == false )
			{
				str.Format(_T("Error, Calc Panel#%d Coordinate Transform Fault"), PanelIndex+1);
				AOIDataCollect.SetErrorString(str);
				CAOIFd::UnlockFd();
				return false; 
			}
			//將整板座標轉換套至各自單板內
			if ( PanelPtr->AssignPanelMapParamToBoards(DistrictID) == false )
			{
				str.Format(_T("Error, Assign Panel#%d Coordinate Transform to Boards Fault"), PanelIndex+1);
				AOIDataCollect.SetErrorString(str);
				CAOIFd::UnlockFd();
				return false; 
			}
			//計算座標轉換
			if ( PanelPtr->CalcPanelStagePosition(DistrictID) == false )
			{
				str.Format(_T("Error, Calc Panel#%d Position Fault"), PanelIndex+1);
				AOIDataCollect.SetErrorString(str);
				CAOIFd::UnlockFd();
				return false; 
			}
			//重新計算影像位置
			if ( PanelPtr->ResetPanelFrameImageRect(DistrictID) == false )
			{
				str.Format(_T("Error, Reset Panel#%d Frame Image Rect Fault"), PanelIndex+1);
				AOIDataCollect.SetErrorString(str);
				CAOIFd::UnlockFd();
				return false; 
			}
			PanelPtr->SetPanelCalcMapFinish(true);
			PanelPtr->ClearPanelFdImageBuffer(DistrictID);
		}
		else
		{
			const unsigned int BoardIndex = BoardPtr->GetBoardIndex_Project();
			if ( BoardPtr->GetBoardCalcMapFinish() == true )
			{
				CAOIFd::UnlockFd();
				return true; 
			}

			if ( BoardPtr->CheckBoardFdCalculated(DistrictID) == false )
			{
				CAOIFd::UnlockFd();
				return true; 
			}

			if ( FD_NG_HANDLE_XBOARD == BoardFdNGHandleMode )
			{
				BoardFdCount = BoardPtr->CalcBoardFdCount(DistrictID);
				FdNGCount = BoardPtr->CalcBoardFdDefectCount(DistrictID);
				FdNGSkipCount = MIN(BoardFdCount, BoardFdNGSkipCount);	
				if ( FdNGCount >= FdNGSkipCount )
				{
					BoardPtr->ExecBoardBeXBoard(LaneID);
					BoardPtr->SetBoardCalcMapFinish(true);
					CAOIFd::UnlockFd();
					return true; 
				}
			}
			//建立座標轉換公式
			if ( BoardPtr->CalcBoardMapParam(DistrictID) == false )
			{
				str.Format(_T("Error, Calc Board#%d Coordinate Transform Fault"), BoardIndex+1);
				AOIDataCollect.SetErrorString(str);
				CAOIFd::UnlockFd();
				return false; 
			}			
			//計算座標轉換
			const bool bCalcFov = false;
			if ( BoardPtr->CalcBoardStagePosition(DistrictID, bCalcFov) == false )
			{
				str.Format(_T("Error, Calc Board#%d Position Fault"), BoardIndex+1);
				AOIDataCollect.SetErrorString(str);
				CAOIFd::UnlockFd();
				return false; 
			}
			//重新計算影像位置
			if ( BoardPtr->ResetBoardFrameImageRect(DistrictID) == false )
			{
				str.Format(_T("Error, Reset Board#%d Frame Image Rect Fault"), BoardIndex+1);
				AOIDataCollect.SetErrorString(str);
				CAOIFd::UnlockFd();
				return false; 
			}
			BoardPtr->SetBoardCalcMapFinish(true);
			BoardPtr->ClearBoardFdImageBuffer(DistrictID);
		}
	}
	CAOIFd::UnlockFd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::CalcFdSpaceBasePlane(const std::vector<TUNI_FRAME> &UniFrameList)//計算定位點基準面
{
	SetFdSpaceBasePlane(0);
	size_t       i=0, j=0, idx=0;
	CAOIWnd     *WndPtr = GetFdWndPtr();	
	if ( NULL == WndPtr ) { return false; }
	const size_t FrameCount=UniFrameList.size();
	for ( i=0; i<FrameCount; i++  )
	{
		if ( NULL == UniFrameList[i].MaskPtr ) { continue; }
		if ( NULL == UniFrameList[i].SpacePtr ) { continue; }
		break;
	}
	if ( FrameCount == i ) { return true; }

	RECT         FdRect;
	RECT         ResRect;
	TUNI_FRAME   UniFrame=UniFrameList[i];
	CAlgParam   &AlgParam=WndPtr->GetWndAlgParam();

	size_t       BasePlaneCnt=0;
	double       BasePlaneVal=0.0;
	IMAGE_SIZE   ImageW=UniFrame.ImageW;
	IMAGE_SIZE   ImageH=UniFrame.ImageH;
	IMAGE_SIZE   ImageStep=UniFrame.ImageStep;
	MASK_PTR     MaskPtr  = UniFrame.MaskPtr;
	SPACE_PTR    SpacePtr = UniFrame.SpacePtr;
	const double ImageOffsetX=AlgParam.GetAlgImageOffsetX();
	const double ImageOffsetY=AlgParam.GetAlgImageOffsetY();	
	const int    nImageOffsetX=(int)(ImageOffsetX+0.5);
	const int    nImageOffsetY=(int)(ImageOffsetY+0.5);

	BasePlaneCnt=0;
	BasePlaneVal=0.0;
	WndPtr->GetWndImageRect(FdRect);
	const int FdRectW=FdRect.right-FdRect.left;
	const int FdRectH=FdRect.bottom-FdRect.top;
	ResRect.left = FdRect.left+nImageOffsetX;
	ResRect.top  = FdRect.top-nImageOffsetY;
	ResRect.right = FdRect.right+nImageOffsetX;
	ResRect.bottom  = FdRect.bottom-nImageOffsetY;
	JetAPI::BoundaryRect(ImageW, ImageH, ResRect);
	for ( i=ResRect.top; i<ResRect.bottom; i++ )
	{
		for ( j=ResRect.left; j<ResRect.right; j++ )
		{
			idx = (i*ImageStep)+(j);
			if ( ImageAPI.CheckSpaceMaskValid(MaskPtr[idx]) == false ) 
			{	continue; }
			
			BasePlaneCnt ++;
			BasePlaneVal += SpacePtr[idx];
		}
	}	
	if ( 0 == BasePlaneCnt ) { return true; }
	BasePlaneVal /= BasePlaneCnt;
	SetFdSpaceBasePlane(BasePlaneVal);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::ExecFdSaveDefectImage(const std::vector<TUNI_FRAME> &UniFrameList)//執行零件儲存瑕疵圖片
{
	return true;
	bool IsOK = true;
	IsOK = CAOIRgn::ExecRgnSaveDefectImage(UniFrameList);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::UpdateFdResultID()//更新定位點檢測結果
{
	size_t     i=0, idx=0;
	double     dSkewA=INVALID_DOUBLE;
	double     dOffsetX=INVALID_DOUBLE;
	double     dOffsetY=INVALID_DOUBLE;	
	ALG_TYPE   AlgType;	
	WND_DEFECT_ID  WndDefectID;	
	CAOIWnd   *WndPtr = NULL;	
	RESULT_ID  ResultID = RESULT_ID_NONE;	
	CAOIModel   *ModelPtr = &(m_FdModel);	
	const size_t WndOrderCount = ModelPtr->GetModelWndOrderCount();
	for ( i=0; i<WndOrderCount; i++ )
	{
		idx = WndOrderCount-i-1;
		WndPtr = ModelPtr->GetModelWndOrderPtr(idx, false);
		if ( NULL == WndPtr ) { continue; }
		WndDefectID = WndPtr->GetWndDefectID();
		if ( WND_DEFECT_PAD_ALIGN != WndDefectID ) { continue; }

		CAlgParam &AlgParam = WndPtr->GetWndAlgParam();	
		AlgType = AlgParam.GetAlgType();
		switch ( AlgType )
		{
		case ALG_MODEL_MATCH:			
		case ALG_IMAGE_MATCH:			
		case ALG_FD_MATCH:
		case ALG_EDGE_SEARCH:
			if ( INVALID_DOUBLE==dSkewA && true==AlgParam.GetAlgSkewEnabled() )
			{	dSkewA = AlgParam.GetAlgSkewReading();	}
			if ( INVALID_DOUBLE==dOffsetX && true==AlgParam.GetAlgOffsetXEnabled() )
			{	dOffsetX = AlgParam.GetAlgOffsetXReading();	}
			if ( INVALID_DOUBLE==dOffsetY && true==AlgParam.GetAlgOffsetYEnabled() )
			{	dOffsetY = AlgParam.GetAlgOffsetYReading();	}
			break;
		}
	}

	double dStageOffsetX=0, dStageOffsetY=0;
	double StagePosX = CAOIFd::GetFdStagePosX();
	double StagePosY = CAOIFd::GetFdStagePosY();
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();

	if ( INVALID_DOUBLE != dSkewA )
	{	}
	if ( INVALID_DOUBLE != dOffsetX )
	{	
		if ( true == SignX ) 
		{	dStageOffsetX = dOffsetX; }
		else
		{	dStageOffsetX = -dOffsetX; }
		StagePosX += dStageOffsetX;
		SetFdStagePosX(StagePosX);
	}
	if ( INVALID_DOUBLE != dOffsetY )
	{	
		if ( true == SignY ) 
		{	dStageOffsetY = dOffsetY; }
		else
		{	dStageOffsetY = -dOffsetY; }
		StagePosY += dStageOffsetY;
		SetFdStagePosY(StagePosY);
	}
	LayoutFdStageCornerPos();
	const LANE_ID LaneID = GetFdLaneID();
	ResultID = ModelPtr->GetModelResultID();
	if ( RESULT_ID_NONE != ResultID )
	{	
		SetFdResultID_AOI(ResultID);	
		SetFdResultID_Alarm(ResultID);
	}
	else
	{	
		SetFdResultID_AOI(RESULT_ID_SKIP); 
		SetFdResultID_Alarm(RESULT_ID_SKIP);
	}
	UpdateFdResultID_AOI_Lane(LaneID);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::ExtractFdFrame()//挖取定位點的影像
{
	const char fnName[] = "CAOIFd::ExtractFdFrame";
	CAOIField* FieldPtr = CAOIRgn::GetRgnFieldPtr();
	if ( NULL == FieldPtr ) { return false; }	
	if ( CheckRgnFieldAllFrameMergeFinish() == false ) { return false; }
	const size_t FrameCount = FieldPtr->GetFieldFramePtrCount();
	if ( FrameCount > FRAME_MAX_COUNT ) { return false; }
	size_t       i=0;	
	CAOIFrame   *FramePtr  = NULL;
	FRAME_TYPE   FrameType = FRAME_NULL;		
	TUNI_FRAME   UniFrameList[FRAME_MAX_COUNT];

	CString    str;	
	bool       Exception = false;
	BOOL       bSaveFov=FALSE;	
	BOOL       bSaveRoi=TRUE;	
	const int  nAlign = 4;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	TREGION4D  ImageRgn;
	TREGION4D  FdRoiRgn;
	MASK_PTR   MaskPtr = NULL;
	IMAGE_PTR  RawPtr = NULL;	
	SPACE_PTR  SpacePtr = NULL;
	IMAGE_PTR  ImagePtr = NULL;

	RECT       RoiRect;	
	TSIZE2D    ImageSizeUm;
	const CAMERA_ID CameraID = CAOIFd::GetFdCameraID();
	const size_t FdIndex = GetFdIndex_Project();	
	const double FdSizeW = GetRgnRoiSizeW();//區域尺寸寬
	const double FdSizeH = GetRgnRoiSizeH();//區域尺寸長
	TSIZE2D  PatExtend = CAOIFd::GetFdPatExtendSize();
	TSIZE2D  RoiSize = CAOIFd::GetFdRoiSize();	
	TPOINT2D StagePos2D;
	TPOINT3D StagePos3D = FieldPtr->GetFieldStagePos();	

	StagePos2D.x = StagePos3D.x;
	StagePos2D.y = StagePos3D.y;
	FdRoiRgn.minX = StagePos3D.x-(RoiSize.cx/2);
	FdRoiRgn.maxX = StagePos3D.x+(RoiSize.cx/2);
	FdRoiRgn.minY = StagePos3D.y-(RoiSize.cy/2);
	FdRoiRgn.maxY = StagePos3D.y+(RoiSize.cy/2);
	CAOIFd::GetRgnRoiStageRegion(FdRoiRgn);
	AOIDataCollect.MapStageRegionToCamera(CameraID, FdRoiRgn, StagePos2D, ImageRgn);	
	JetAPI::Region4DToRect(ImageRgn, RoiRect, true);		
	
	IMAGE_SIZE RoiStep=0;
	IMAGE_SIZE RoiBitCount=0;	
	MASK_PTR   RoiMaskPtr=NULL;
	IMAGE_PTR  RoiImagePtr=NULL;
	SPACE_PTR  RoiSpacePtr=NULL;
	const IMAGE_SIZE RoiW = RoiRect.right-RoiRect.left;
	const IMAGE_SIZE RoiH = RoiRect.bottom-RoiRect.top;	

	Exception = false;
	AOIDataCollect.MapImageSizeToReal(CameraID, RoiRect, ImageSizeUm);
	CAOIFd::SetFdFrameImageRect(RoiRect);	
	CAOIFd::SetFdFrameImageSize_um(ImageSizeUm);

	TPOINT2D   ImageOffsetUm;
	TRECT4D    ImageRegion;
	AOIDataCollect.MapCameraRectToStage(CameraID, RoiRect, StagePos2D, ImageRegion);		
	ImageOffsetUm.x=ImageRegion.GetCpX()-FdRoiRgn.GetCpX();
	ImageOffsetUm.y=ImageRegion.GetCpY()-FdRoiRgn.GetCpY();	
	SetFdFrameImageStageOffset_um(ImageOffsetUm);

	JetAPI::InitialUniFrameList(UniFrameList, FRAME_MAX_COUNT);	
	for ( i=0; i<FrameCount; i++ )
	{
		FramePtr = FieldPtr->GetFieldFramePtr(i, false);
		if ( NULL == FramePtr )
		{ 
			Exception = true;
			break;
		}
		FrameType = FramePtr->GetFrameType();
		if ( FRAME_SPACE == FrameType )
		{	FramePtr->GetFrameSpacePtr(ImageW, ImageH, ImageStep, BitCount, MaskPtr, SpacePtr); }
		else
		{	FramePtr->GetFrameImagePtr(ImageW, ImageH, ImageStep, BitCount, RawPtr);	}
		if ( FRAME_BAYER == FrameType )
		{			
			IMAGE_SIZE   DeBayerBit=0;
			IMAGE_SIZE   DeBayerStep=0;		
			IMAGE_PTR    DeBayerPtr=NULL;
			BAYER_PATTERN_MODE BayerPattern = FramePtr->GetFrameBayerPattern();			
			if ( AOIDataCollect.ExecDebayerImage(fnName, ImageW, ImageH, ImageStep, RawPtr, BayerPattern, DeBayerStep, DeBayerBit, DeBayerPtr) == false)		
			{
				Exception = true;
				break;
			}
			ImagePtr = DeBayerPtr;
			BitCount = DeBayerBit;
			ImageStep = DeBayerStep;			
			DeBayerPtr = NULL;
		}
		else
		{	ImagePtr = RawPtr; }		

	#ifdef _DEBUG
		if ( TRUE==bSaveFov && FRAME_SPACE!=FrameType )
		{
			str.Format(_T("%s\\Fd[%d]Fov#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), FdIndex+1, i+1);
			ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
		}
	#endif//_DEBUG
		RoiBitCount=BitCount;
		RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, 4);
		RoiMaskPtr = NULL;
		RoiImagePtr = NULL;
		RoiSpacePtr = NULL;
		if ( FRAME_SPACE == FrameType )
		{
			if ( ImageAPI.ExtractGrayRoiImage(ImageW, ImageH, ImageStep, MaskPtr, RoiRect, RoiStep, RoiMaskPtr, false) == false )
			{
				Exception = true;
				break;
			}
			if ( ImageAPI.ExtractSpaceRoiImage(ImageW, ImageH, ImageStep, BitCount, SpacePtr, RoiRect, RoiStep, RoiSpacePtr, false) == false )
			{
				Exception = true;
				break;
			}			
		}
		else
		{
			if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, RoiStep, RoiImagePtr, false) == false )
			{
				Exception = true;
				break;
			}
		}

		if ( RawPtr != ImagePtr )
		{	JetMemory.free_func(ImagePtr); }

	#ifdef _DEBUG
		if ( TRUE==bSaveRoi && NULL!=RoiImagePtr )
		{
			str.Format(_T("%s\\Fd[%d]Roi#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), FdIndex+1, i+1);
			ImageAPI.SaveBMPImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiImagePtr, true);
		}
	#endif//_DEBUG

		UniFrameList[i].ImageW = RoiW;
		UniFrameList[i].ImageH = RoiH;
		UniFrameList[i].ImageStep = RoiStep;
		UniFrameList[i].BitCount  = RoiBitCount;
		UniFrameList[i].ImagePtr = RoiImagePtr;
		UniFrameList[i].MaskPtr = RoiMaskPtr;
		UniFrameList[i].SpacePtr = RoiSpacePtr;		
		RawPtr = NULL;
		RoiMaskPtr = NULL;
		RoiImagePtr = NULL;
		RoiSpacePtr = NULL;
	}
	for ( i=0; i<FrameCount; i++ )
	{
		FramePtr = FieldPtr->GetFieldFramePtr(i, false);
		if ( NULL == FramePtr ) { continue; }
		FramePtr->ClearFrameBuffer();	
	}

	for ( i=0; i<FrameCount; i++ )
	{			
		if ( CAOIRgn::SetRgnUniFrame(i, UniFrameList[i]) == false )
		{
			Exception = true;
			break; 
		}
		JetAPI::InitialUniFrame(UniFrameList[i]);	
	}	
	if ( true == Exception )
	{	
		CAOIRgn::ClearRgnImageBuffer();
		JetAPI::ClearUniFrameList(UniFrameList, FRAME_MAX_COUNT);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::BuildNewFd(TPOINT2D CadPos, TPOINT2D StagePos, TSIZE2D szFd, TSIZE2D szRoi, unsigned int FrameIndex, unsigned int FrameUniqueID, FRAME_TYPE FrameType, LPCTSTR FdFolder, TUNI_FRAME UniFrame)//創建新的定位點
{	
	CAOIFd     *FdPtr = this;
	CAOIBox    *BoxPtr = NULL;
	CAOIWnd    *WndPtr = NULL;	
	CAOIModel  *ModelPtr = NULL;
	if ( NULL == FdPtr ) { return false; }
	const bool IsExceptionAngle = false;	
	CString       strFdUniqueName;
	TPOINT2D      StagePos_LA = StagePos;
	const size_t  BufferSize = MAX_JET_PATH;
	char          FdUniqueName[BufferSize]="";
	MODEL_TYPE    ModelType = MODEL_TYPE_FD;
	ALG_TYPE      AlgType     = ALG_FD_MATCH;	
	WND_DEFECT_ID WndDefectID = WND_DEFECT_PAD_ALIGN;	
	const double ExtanedXum = (szRoi.cx-szFd.cx)/2;
	const double ExtanedYum = (szRoi.cy-szFd.cy)/2;
	const LANE_ID LaneID = AOIDataCollect.GetActiveLaneID();
	
	ModelPtr = FdPtr->GetFdModelPtr();
	JetAPI::ExtractMainFileNameNoPath(FdFolder, strFdUniqueName);
	JetAPI::TCHAR2char(strFdUniqueName, FdUniqueName, BufferSize);

	FdPtr->SetFdLaneID(LaneID);
	FdPtr->SetFdSelected(true);
	FdPtr->SetFdAngle(0);	
	FdPtr->SetFdCadPosX(CadPos.x);
	FdPtr->SetFdCadPosY(CadPos.y);
	FdPtr->SetFdBodySizeW(szFd.cx);
	FdPtr->SetFdBodySizeH(szFd.cy);
	FdPtr->SetFdStagePosX(StagePos.x);
	FdPtr->SetFdStagePosY(StagePos.y);
	AOIDataCollect.MapStagePosToLaneA(StagePos_LA.x, StagePos_LA.y, LaneID);
	FdPtr->SetFdTeachStagePosX(StagePos_LA.x);
	FdPtr->SetFdTeachStagePosY(StagePos_LA.y);	
	FdPtr->SetFdRoiExtendSizeW(szRoi.cx);
	FdPtr->SetFdRoiExtendSizeH(szRoi.cy);
	FdPtr->CalcFdCadCornerPos();//計算軟體條碼Cad端點座標		
	FdPtr->LayoutFdStageCornerPos();//更新軟體條碼機台端點座標	
	
	JetAPI::CreateFolder(FdFolder);
	ModelPtr->SetModelName(FdUniqueName);
	ModelPtr->SetModelFolderModel(FdFolder);

	ModelPtr->SetModelType(ModelType);
	ModelPtr->SetModelAttachedAngle(0);
	ModelPtr->SetModelAttachedPosCad(CadPos.x, CadPos.y);
	ModelPtr->SetModelAttachedPosStage(StagePos);
	
	BoxPtr = ModelPtr->GetModelBodyBoxPtr();
	if ( NULL != BoxPtr )
	{
		BoxPtr->SetBoxSizeX(szFd.cx);
		BoxPtr->SetBoxSizeY(szFd.cy);		
		BoxPtr->LayoutBoxCornerPos();		
		BoxPtr->ResetBoxRegionRes();
	}
	
	TREGION4D WndCadRgn;
	WndCadRgn.minX = -(szFd.cx/2);
	WndCadRgn.minY = -(szFd.cy/2);
	WndCadRgn.maxX =  (szFd.cx/2);
	WndCadRgn.maxY =  (szFd.cy/2);
	WndPtr = ModelPtr->CreateModelWnd(WndDefectID, WndCadRgn, NULL);
	if ( NULL == WndPtr )
	{
		JetAPI::ShowMessageBox(_T("Error, Create Fd Model Wnd Ptr Fault"));
		return false;
	}	
	CAlgBinaryParam  BinaryParam;
	CAlgParam  &AlgParam = WndPtr->GetWndAlgParam();
	AlgParam.ChangeAlgType(AlgType, true);//順序不要顛倒
	
	BinaryParam = (AlgParam.GetAlgImageBinParam());
	BinaryParam.SetBinaryFrameIndex(FrameIndex);
	BinaryParam.SetBinaryFrameUniqueID(FrameUniqueID);
	AlgParam.AdjustAlgBinaryParam(FrameType, BinaryParam);
	AlgParam.SetAlgImageBinParam(BinaryParam);
	WndPtr->ChangeWndDefectID(ModelType, WndDefectID);	
	WndPtr->SetWndExtendRangeX(ExtanedXum);
	WndPtr->SetWndExtendRangeY(ExtanedYum);
	WndPtr->UpdateWndExtendBox();
	const bool WndRgnLinkAuto = WndPtr->GetWndRgnLinkAuto();

	ModelPtr->UnSelectModel();	
	ModelPtr->AddModelWndPtr(WndPtr, false);	
	if ( true == WndRgnLinkAuto )
	{	ModelPtr->UpdateModelWndRgnByLinkMode();	}

	ModelPtr->CalcModelTotalRegionAll();
	ModelPtr->UpdateModelBodyToFd();	

	if ( NULL!=UniFrame.ImagePtr || NULL!=UniFrame.SpacePtr )
	{	
		int PolarityIdx = 0;
		BOX_TOWARD Toward = WndPtr->GetWndToward();
		IMAGE_SIZE PatW = UniFrame.ImageW;
		IMAGE_SIZE PatH = UniFrame.ImageH;
		IMAGE_SIZE PatStep = UniFrame.ImageStep;
		IMAGE_SIZE BitCount = UniFrame.BitCount;
		IMAGE_PTR  PatPtr = UniFrame.ImagePtr;		
		AlgParam.AddAlgPatternImage(PatW, PatH, PatStep, BitCount, PatPtr, FdFolder, Toward, PolarityIdx, BinaryParam);
	}
	WndPtr->SetWndSelected(true);	
	ModelPtr->SetModelWndActived(WndPtr);
	ModelPtr->SetModelNeedSaveFiles(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::UpdateFdFrameIndex(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList)
{
	unsigned int  FrameIndex=0;
	unsigned int  FrameUniqueID = 0;
	const size_t  FrameIndexMapSize = FrameIndexMapList.size();

	FrameUniqueID = GetFdFrameUniqueID();
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
	SetFdFrameIndex(FrameIndex);
	SetFdFrameUniqueID(FrameUniqueID);

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
	//SetFdMaskFrameIndex_Base(FrameIndex);
	//SetFdMaskFrameUniqueID_Base(FrameUniqueID);	
	return true;
}
//-------------------------------------------------------------------------------------//
double CAOIFd::GetFdSpaceBasePlane() const
{
	return m_FdSpaceBasePlane;
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdSpaceBasePlane(double val)
{
	m_FdSpaceBasePlane = val;
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdPanelBasePlane(double val)
{
	SetRgnPanelBasePlane(val);
	CAOIModel *ModelPtr = GetFdModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelPanelBasePlane(val); }
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdSpaceBasePlaneParam(const TBasePlaneParam& Param)
{
	SetRgnSpaceBasePlaneParam(Param); 
	CAOIModel *ModelPtr = GetFdModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelSpaceBasePlaneParam(Param); }
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdSpaceNoiseFilterParam(const TNoiseFilterParam& Param)
{ 
	SetRgnSpaceNoiseFilterParam(Param); 
	CAOIModel *ModelPtr = GetFdModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelSpaceNoiseFilterParam(Param); }
}
//-------------------------------------------------------------------------------------//
bool CAOIFd::GetFdDataModelEnabled() const//取得定位點資料模型啟用
{
	return CAOIRgn::GetRgnDataModelEnabled();
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdDataModelEnabled(bool val)//設定定位點資料模型啟用
{
	CAOIRgn::SetRgnDataModelEnabled(val);	
	CAOIModel *ModelPtr = GetFdModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelDataModelEnabled(val); }
}
//-------------------------------------------------------------------------------------//
int CAOIFd::GetFdDataModelLevelID() const//取得定位點資料模型等級
{
	return CAOIRgn::GetRgnDataModelLevelID();
}
//-------------------------------------------------------------------------------------//
void CAOIFd::SetFdDataModelLevelID(int val)//設定定位點資料模型等級
{
	CAOIRgn::SetRgnDataModelLevelID(val);	
	CAOIModel *ModelPtr = GetFdModelPtr();
	if ( NULL != ModelPtr )
	{	ModelPtr->SetModelDataModelLevelID(val); }
}
//-------------------------------------------------------------------------------------//