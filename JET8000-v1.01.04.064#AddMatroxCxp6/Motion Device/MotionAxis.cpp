// MotionAxis.cpp: implementation of the CMotionAxis class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "MotionAxis.h"
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
CMotionAxis::CMotionAxis()
{
	PreInitial();
}
//-------------------------------------------------------------------------------------//
CMotionAxis::CMotionAxis(const CMotionNode &Node)
{
	PreInitial();
	CloneMotionNode(Node);
}
//-------------------------------------------------------------------------------------//
CMotionAxis::~CMotionAxis()
{

}
//-------------------------------------------------------------------------------------//
CMotionAxis& CMotionAxis::operator=(const CMotionNode &Node)
{
	CloneMotionNode(Node);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::PreInitial()
{	
	return ;
}
//-------------------------------------------------------------------------------------//
void  CMotionAxis::CloneMotionNode(const CMotionNode &Node)
{
	m_AxisNode=Node;
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::Reset()
{
	m_AxisNode=CMotionNode();
	ClearSlaveNodeList();	
}
//-------------------------------------------------------------------------------------//
CMotionNode* CMotionAxis::GetMotionNodePtr()
{
	return &m_AxisNode;
}
//-------------------------------------------------------------------------------------//
CMotionNode& CMotionAxis::GetMotionNodeRef()
{
	return m_AxisNode;
}
//-------------------------------------------------------------------------------------//
const CMotionNode& CMotionAxis::GetMotionNodeRef() const
{
	return m_AxisNode;
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetMotionNode(const CMotionNode &Node)
{
	CloneMotionNode(Node);
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetAxisID(int val)
{
	GetMotionNodeRef().SetAxisID(val);
}
//-------------------------------------------------------------------------------------//
int CMotionAxis::GetAxisID() const
{
	return GetMotionNodeRef().GetAxisID();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetAxisName(const char *val)
{
	GetMotionNodeRef().SetAxisName(val);
}
//-------------------------------------------------------------------------------------//
const char* CMotionAxis::GetAxisName() const
{
	return GetMotionNodeRef().GetAxisName();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetCardID(int val)
{
	GetMotionNodeRef().SetCardID(val);
}
//-------------------------------------------------------------------------------------//
int CMotionAxis::GetCardID() const
{
	return GetMotionNodeRef().GetCardID();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetNodeID(int val)
{
	GetMotionNodeRef().SetNodeID(val);
}
//-------------------------------------------------------------------------------------//
int CMotionAxis::GetNodeID() const
{
	return GetMotionNodeRef().GetNodeID();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetSlotID(int val)
{
	GetMotionNodeRef().SetSlotID(val);
}
//-------------------------------------------------------------------------------------//
int CMotionAxis::GetSlotID() const
{
	return GetMotionNodeRef().GetSlotID();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetGantryID(int val)
{
	GetMotionNodeRef().SetGantryID(val);
}
//-------------------------------------------------------------------------------------//
int CMotionAxis::GetGantryID() const
{
	return GetMotionNodeRef().GetGantryID();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetAxisType(MOTION_AXIS_TYPE val)
{
	GetMotionNodeRef().SetAxisType(val);
}
//-------------------------------------------------------------------------------------//
MOTION_AXIS_TYPE CMotionAxis::GetAxisType() const
{
	return GetMotionNodeRef().GetAxisType();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetAxisBypass(bool val)
{
	return GetMotionNodeRef().SetAxisBypass(val);
}
//-------------------------------------------------------------------------------------//
bool CMotionAxis::GetAxisBypass() const
{
	return GetMotionNodeRef().GetAxisBypass();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetDriverModel(ECAT_DRIVER_MODEL val)
{
	return GetMotionNodeRef().SetDriverModel(val);
}
//-------------------------------------------------------------------------------------//
ECAT_DRIVER_MODEL CMotionAxis::GetDriverModel() const
{
	return GetMotionNodeRef().GetDriverModel();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetUseHomeSensor(bool val)
{
	GetMotionNodeRef().SetUseHomeSensor(val);
}
//-------------------------------------------------------------------------------------//
bool CMotionAxis::GetUseHomeSensor() const
{
	return GetMotionNodeRef().GetUseHomeSensor();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetInputIoBit_INP(DWORD val)
{
	GetMotionNodeRef().SetInputIoBit_INP(val);
}
//-------------------------------------------------------------------------------------//
DWORD CMotionAxis::GetInputIoBit_INP() const
{
	return GetMotionNodeRef().GetInputIoBit_INP();
}
//-------------------------------------------------------------------------------------//
bool CMotionAxis::CheckUseInputIoBit_INP() const
{
	return GetMotionNodeRef().CheckUseInputIoBit_INP();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::ClearSlaveNodeList()
{
	m_SlaveNodeList.clear();
}
//-------------------------------------------------------------------------------------//
bool CMotionAxis::CheckGantryAxis() const
{
	if ( GetSlaveNodeCount() == 0 )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CMotionAxis::GetSlaveNodeCount() const
{
	return m_SlaveNodeList.size();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::AddSlaveNode(const CMotionNode &Node)
{
	m_SlaveNodeList.push_back(Node);
}
//-------------------------------------------------------------------------------------//
CMotionNode* CMotionAxis::GetSlaveNodePtr(size_t idx, bool bCheck)
{
	if ( true == bCheck )
	{
		const size_t Cnt=m_SlaveNodeList.size();
		if ( idx >= Cnt )
		{	return NULL; }
	}
	return &m_SlaveNodeList[idx];
}
//-------------------------------------------------------------------------------------//
bool CMotionAxis::UpdateToSlaveNodeList()//將軸參數更新至子節點列表
{
	size_t i=0;
	const size_t Count=GetSlaveNodeCount();
	for ( i=0; i<Count; i++ )
	{
		CMotionNode *Ptr=GetSlaveNodePtr(i, false);
		if ( NULL == Ptr ) { continue; }
		Ptr->UpdateMotionParam(GetMotionNodeRef());
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetGantryEnableOffset(bool val)
{
	GetMotionNodeRef().SetGantryEnableOffset(val);
}
//-------------------------------------------------------------------------------------//
bool CMotionAxis::GetGantryEnableOffset() const
{
	return GetMotionNodeRef().GetGantryEnableOffset();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetSignPositive(int val)
{
	GetMotionNodeRef().SetSignPositive(val);
}
//-------------------------------------------------------------------------------------//
int CMotionAxis::GetSignPositive() const
{
	return GetMotionNodeRef().GetSignPositive();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetSignPositiveConvert(int val)
{
	GetMotionNodeRef().SetSignPositiveConvert(val);
}
//-------------------------------------------------------------------------------------//
int CMotionAxis::GetSignPositiveConvert() const
{
	return GetMotionNodeRef().GetSignPositiveConvert();
}
//-------------------------------------------------------------------------------------//
bool CMotionAxis::CheckConvertSignPositive() const
{
	return GetMotionNodeRef().CheckConvertSignPositive();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetHomePreMoveDis(double val)
{
	GetMotionNodeRef().SetHomePreMoveDis(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetHomePreMoveDis() const
{
	return GetMotionNodeRef().GetHomePreMoveDis();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetHomeOrgOffset(double val)
{
	GetMotionNodeRef().SetHomeOrgOffset(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetHomeOrgOffset() const
{
	return GetMotionNodeRef().GetHomeOrgOffset();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetStartVelocity(double val)
{
	GetMotionNodeRef().SetStartVelocity(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetStartVelocity() const
{
	return GetMotionNodeRef().GetStartVelocity();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetHomeVelocity(double val)
{
	GetMotionNodeRef().SetHomeVelocity(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetHomeVelocity() const
{
	return GetMotionNodeRef().GetHomeVelocity();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetMaxVelocity(double val)
{
	GetMotionNodeRef().SetMaxVelocity(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetMaxVelocity() const
{
	return GetMotionNodeRef().GetMaxVelocity();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetBoardInVelocity(double val)
{
	GetMotionNodeRef().SetBoardInVelocity(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetBoardInVelocity() const
{
	return GetMotionNodeRef().GetBoardInVelocity();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetBoardOutVelocity(double val)
{
	GetMotionNodeRef().SetBoardOutVelocity(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetBoardOutVelocity() const
{
	return GetMotionNodeRef().GetBoardOutVelocity();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetGrabbingVelocity(double val)
{
	GetMotionNodeRef().SetGrabbingVelocity(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetGrabbingVelocity() const
{
	return GetMotionNodeRef().GetGrabbingVelocity();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetGrabMapVelocity(double val)
{
	GetMotionNodeRef().SetGrabMapVelocity(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetGrabMapVelocity() const
{
	return GetMotionNodeRef().GetGrabMapVelocity();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetStageStartPos(double val)
{
	GetMotionNodeRef().SetStageStartPos(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetStageStartPos() const
{
	return GetMotionNodeRef().GetStageStartPos();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetStageLeavePos(double val)
{
	GetMotionNodeRef().SetStageLeavePos(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetStageLeavePos() const
{
	return GetMotionNodeRef().GetStageLeavePos();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetBeforePCBInPos(double val)
{
	GetMotionNodeRef().SetBeforePCBInPos(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetBeforePCBInPos() const
{
	return GetMotionNodeRef().GetBeforePCBInPos();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetPCBStopRPos_LA(double val)
{
	GetMotionNodeRef().SetPCBStopRPos_LA(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis:: GetPCBStopRPos_LA() const
{
	return GetMotionNodeRef().GetPCBStopRPos_LA();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetPCBStopRPos_LB(double val)
{
	GetMotionNodeRef().SetPCBStopRPos_LB(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetPCBStopRPos_LB() const
{
	return GetMotionNodeRef().GetPCBStopRPos_LB();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetPCBStopLPos_LA(double val)
{
	GetMotionNodeRef().SetPCBStopLPos_LA(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetPCBStopLPos_LA() const
{
	return GetMotionNodeRef().GetPCBStopLPos_LA();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetPCBStopLPos_LB(double val)
{
	GetMotionNodeRef().SetPCBStopLPos_LB(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetPCBStopLPos_LB() const
{
	return GetMotionNodeRef().GetPCBStopLPos_LB();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetLaneLedStopRPos_LA(double val)
{
	GetMotionNodeRef().SetLaneLedStopRPos_LA(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetLaneLedStopRPos_LA() const
{
	return GetMotionNodeRef().GetLaneLedStopRPos_LA();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetLaneLedSlowRPos_LA(double val)
{
	GetMotionNodeRef().SetLaneLedSlowRPos_LA(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetLaneLedSlowRPos_LA() const
{
	return GetMotionNodeRef().GetLaneLedSlowRPos_LA();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetLaneLedStopRPos_LB(double val)
{
	GetMotionNodeRef().SetLaneLedStopRPos_LB(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetLaneLedStopRPos_LB() const
{
	return GetMotionNodeRef().GetLaneLedStopRPos_LB();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetLaneLedSlowRPos_LB(double val)
{
	GetMotionNodeRef().SetLaneLedSlowRPos_LB(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetLaneLedSlowRPos_LB() const
{
	return GetMotionNodeRef().GetLaneLedSlowRPos_LB();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetLaneLedStopLPos_LA(double val)
{
	GetMotionNodeRef().SetLaneLedStopLPos_LA(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetLaneLedStopLPos_LA() const
{
	return GetMotionNodeRef().GetLaneLedStopLPos_LA();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetLaneLedSlowLPos_LA(double val)
{
	GetMotionNodeRef().SetLaneLedSlowLPos_LA(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetLaneLedSlowLPos_LA() const
{
	return GetMotionNodeRef().GetLaneLedSlowLPos_LA();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetLaneLedStopLPos_LB(double val)
{
	GetMotionNodeRef().SetLaneLedStopLPos_LB(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetLaneLedStopLPos_LB() const
{
	return GetMotionNodeRef().GetLaneLedStopLPos_LB();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetLaneLedSlowLPos_LB(double val)
{
	GetMotionNodeRef().SetLaneLedSlowLPos_LB(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetLaneLedSlowLPos_LB() const
{
	return GetMotionNodeRef().GetLaneLedSlowLPos_LB();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetJogStartVelocity(double val)
{
	GetMotionNodeRef().SetJogStartVelocity(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetJogStartVelocity() const
{
	return GetMotionNodeRef().GetJogStartVelocity();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetJogAccelerationTime(double val)
{
	GetMotionNodeRef().SetJogAccelerationTime(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetJogAccelerationTime() const
{
	return GetMotionNodeRef().GetJogAccelerationTime();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetJogDecelerationTime(double val)
{
	GetMotionNodeRef().SetJogDecelerationTime(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetJogDecelerationTime() const
{
	return GetMotionNodeRef().GetJogDecelerationTime();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetJogMaxVelocity(double val)
{
	GetMotionNodeRef().SetJogMaxVelocity(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetJogMaxVelocity() const
{
	return GetMotionNodeRef().GetJogMaxVelocity();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetJogMovingVelocity(double val)
{
	GetMotionNodeRef().SetJogMovingVelocity(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetJogMovingVelocity() const
{
	return GetMotionNodeRef().GetJogMovingVelocity();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetAccelerationValue(double val)
{
	GetMotionNodeRef().SetAccelerationValue(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetAccelerationValue() const
{
	return GetMotionNodeRef().GetAccelerationValue();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetAccelerationTime(double val)
{
	GetMotionNodeRef().SetAccelerationTime(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetAccelerationTime() const
{
	return GetMotionNodeRef().GetAccelerationTime();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetDecelerationTime(double val)
{
	GetMotionNodeRef().SetDecelerationTime(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetDecelerationTime() const
{
	return GetMotionNodeRef().GetDecelerationTime();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetAccelerationMinTime(double val)
{
	GetMotionNodeRef().SetAccelerationMinTime(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetAccelerationMinTime() const
{
	return GetMotionNodeRef().GetAccelerationMinTime();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetAdjustAccclerationDist(double val)
{
	GetMotionNodeRef().SetAdjustAccclerationDist(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetAdjustAccclerationDist() const
{
	return GetMotionNodeRef().GetAdjustAccclerationDist();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetMovingCurveMode(MOVE_CURVE_MODE val)
{
	GetMotionNodeRef().SetMovingCurveMode(val);
}
//-------------------------------------------------------------------------------------//
MOVE_CURVE_MODE CMotionAxis::GetMovingCurveMode() const
{
	return GetMotionNodeRef().GetMovingCurveMode();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetAccTimeAdjustMode(ACC_TIME_ADJUST_MODE val)
{
	GetMotionNodeRef().SetAccTimeAdjustMode(val);
}
//-------------------------------------------------------------------------------------//
ACC_TIME_ADJUST_MODE CMotionAxis::GetAccTimeAdjustMode() const
{
	return GetMotionNodeRef().GetAccTimeAdjustMode();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetTriangleCorrection(bool val)
{
	GetMotionNodeRef().SetTriangleCorrection(val);
}
//-------------------------------------------------------------------------------------//
bool CMotionAxis::GetTriangleCorrection() const
{
	return GetMotionNodeRef().GetTriangleCorrection();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetTestTimeDistance(double val)
{
	GetMotionNodeRef().SetTestTimeDistance(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetTestTimeDistance() const
{
	return GetMotionNodeRef().GetTestTimeDistance();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetLimitMax(double val)
{
	GetMotionNodeRef().SetLimitMax(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetLimitMax() const
{
	return GetMotionNodeRef().GetLimitMax();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetLimitMin(double val)
{
	GetMotionNodeRef().SetLimitMin(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetLimitMin() const
{
	return GetMotionNodeRef().GetLimitMin();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetLimitMargin(double val)
{
	GetMotionNodeRef().SetLimitMargin(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetLimitMargin() const
{
	return GetMotionNodeRef().GetLimitMargin();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetCommandPos(double val)
{
	GetMotionNodeRef().SetCommandPos(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetCommandPos() const
{
	return GetMotionNodeRef().GetCommandPos();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetCommandOffline(double val)
{
	GetMotionNodeRef().SetCommandOffline(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetCommandOffline() const
{
	return GetMotionNodeRef().GetCommandOffline();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetCommandMaxVelocity(double val)
{
	GetMotionNodeRef().SetCommandMaxVelocity(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetCommandMaxVelocity() const
{
	return GetMotionNodeRef().GetCommandMaxVelocity();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetCommandAccelerationTime(double val)
{
	GetMotionNodeRef().SetCommandAccelerationTime(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetCommandAccelerationTime() const
{
	return GetMotionNodeRef().GetCommandAccelerationTime();
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetPosTolerance() const
{
	return GetMotionNodeRef().GetPosTolerance();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetFreeRunVel(double val)
{
	GetMotionNodeRef().SetFreeRunVel(val);
}
//-------------------------------------------------------------------------------------//
double CMotionAxis::GetFreeRunVel() const
{
	return GetMotionNodeRef().GetFreeRunVel();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetIsHomed(bool val)
{
	GetMotionNodeRef().SetIsHomed(val);
}
//-------------------------------------------------------------------------------------//
bool CMotionAxis::GetIsHomed() const
{
	return GetMotionNodeRef().GetIsHomed();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetIsEnabled(bool val)
{
	GetMotionNodeRef().SetIsEnabled(val);
}
//-------------------------------------------------------------------------------------//
bool CMotionAxis::GetIsEnabled() const
{
	return GetMotionNodeRef().GetIsEnabled();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetJogDirection(int val)
{
	GetMotionNodeRef().SetJogDirection(val);
}
//-------------------------------------------------------------------------------------//
int CMotionAxis::GetJogDirection() const
{
	return GetMotionNodeRef().GetJogDirection();
}
//-------------------------------------------------------------------------------------//
void CMotionAxis::SetIsWaitForInp(bool val)
{
	GetMotionNodeRef().SetIsWaitForInp(val);
}
//-------------------------------------------------------------------------------------//
bool CMotionAxis::GetIsWaitForInp() const
{
	return GetMotionNodeRef().GetIsWaitForInp();
}
//-------------------------------------------------------------------------------------//