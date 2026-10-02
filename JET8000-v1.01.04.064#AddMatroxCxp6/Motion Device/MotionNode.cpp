// MotionNode.cpp: implementation of the CMotionNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MotionNode.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#define GANTRY_DEFAULT_STD_OFFSET           INT_MAX
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CMotionNode::CMotionNode()
{
	PreInitial();
}
//-------------------------------------------------------------------------------------//
CMotionNode::~CMotionNode()
{

}
//-------------------------------------------------------------------------------------//
void CMotionNode::PreInitial()
{
	m_AxisID = -1;
	m_CardID = -1;
	m_NodeID = -1;
	m_SlotID = -1;
	m_GantryID = -1;
	m_AxisBypass = false;
	m_DriverModel = ECAT_DRIVER_NONE;
	m_AxisType = MOTION_AXIS_MASTER ;
	strcpy(m_AxisName, "");
	m_UseHomeSensor = true;
	m_InputIoBit_INP = -1;

	m_SignPositive = FN_ENABLE;	
	m_SignPositiveConvert = FN_ENABLE;	

	m_HomePreMoveDis = 100000;
	m_HomeOrgOffset = 50000;	
	m_StartVelocity = 0;
	m_HomeVelocity = 50000;
	m_MaxVelocity = 500000;
	m_BoardInVelocity = 200000;
	m_BoardOutVelocity = 200000;
	m_GrabbingVelocity = 500000;
	m_GrabMapVelocity  = 500000;
	
	m_StageStartPos = 0;
	m_StageLeavePos = 0;
	m_BeforePCBInPos = 0;
	m_PCBStopRPos_LA = 0;
	m_PCBStopRPos_LB = 0;
	m_PCBStopLPos_LA = 0;
	m_PCBStopLPos_LB = 0;

	m_LaneLedStopRPos_LA = 0;
	m_LaneLedSlowRPos_LA = 0;
	m_LaneLedStopRPos_LB = 0;
	m_LaneLedSlowRPos_LB = 0;
	m_LaneLedStopLPos_LA = 0;
	m_LaneLedSlowLPos_LA = 0;
	m_LaneLedStopLPos_LB = 0;
	m_LaneLedSlowLPos_LB = 0;
	
	m_JogStartVelocity = 0;
	m_JogAccelerationTime = 0.15;
	m_JogDecelerationTime = 0.15;
	m_JogMaxVelocity = 150000;
	m_JogMovingVelocity = 50000;
	
	m_AccelerationValue = 2000000;
	m_AccelerationTime = 0.1;
	m_DecelerationTime = 0.1;
	m_AccelerationMinTime = 0.05;
	m_AdjustAccclerationDist = 0.0;
	m_MovingCurveMode = MOVE_CURVE_T;
	m_AccTimeAdjustMode = ACC_TIME_ADJUST_MIN_T;
	m_TriangleCorrection = false;
	
	m_TestTimeDistance = 40000;
	
	m_LimitMax = 10000000;
	m_LimitMin =-10000000; 
	m_LimitMargin = 5000;
	
	m_LatchPos = 0.0;	
	m_CommandPos = 0.0;
	m_CommandOffline = false;
	m_CommandMaxVelocity = 0.0;
	m_CommandAccelerationTime = 0.0;
	m_FreeRunVel = 0.0;

	m_GantryOffset = 0.0;
	m_GantryCalcPos = 0.0;
	m_GantryEnableOffset = FN_ENABLE;
	m_GantryStdOffset = GANTRY_DEFAULT_STD_OFFSET;

	m_IsHomed = false;
	m_IsEnabled = false;
	m_JogDirection = 1;	
	m_IsWaitForInp = true;
	return ;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::LoadIniFile()
{
	if ( LoadIniFile(m_IniSection, m_IniFilename) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::SaveIniFile()
{
	if ( SaveIniFile(m_IniSection, m_IniFilename) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::LoadIniFile(LPCTSTR Section, LPCTSTR Filename)
{
	const size_t textlen = 128;	
	CString KeyName = _T("");
	CString Default = _T("");
	CString ErrorString = _T("");
	TCHAR   String[textlen]=_T("");	

	SetIniSection(Section);
	SetIniFilename(Filename);

	KeyName.Format(_T("Axis ID"));
	Default.Format(_T("%d"), GetAxisID());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, ErrorString) == true ) 	
	{	SetAxisID(::_ttoi(String)); }	

	KeyName.Format(_T("Card ID"));
	Default.Format(_T("%d"), GetCardID());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, ErrorString) == true ) 	
	{	SetCardID(::_ttoi(String)); }	

	KeyName.Format(_T("Node ID"));
	Default.Format(_T("%d"), GetNodeID());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, ErrorString) == true ) 	
	{	SetNodeID(::_ttoi(String)); }	

	KeyName.Format(_T("Slot ID"));
	Default.Format(_T("%d"), GetSlotID());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, ErrorString) == true ) 	
	{	SetSlotID(::_ttoi(String)); }	

	KeyName.Format(_T("Gantry ID"));
	Default.Format(_T("%d"), GetGantryID());
	//if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, ErrorString) == true ) 	
	//{	SetGantryID(::_ttoi(String)); }	

	KeyName.Format(_T("Bypass"));
	Default.Format(_T("%d"), GetAxisBypass());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, ErrorString) == true ) 	
	{	SetAxisBypass(::_ttoi(String)); }	

	KeyName.Format(_T("Axis Type"));
	Default.Format(_T("%d"), GetAxisType());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, ErrorString) == true ) 	
	{	SetAxisType((MOTION_AXIS_TYPE)::_ttoi(String)); }	

	KeyName.Format(_T("Gantry STD Offset"));
	Default.Format(_T("%.2f"), GetGantryStdOffset());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, ErrorString) == true ) 	
	{	SetGantryStdOffset(::_ttof(String)); }

	KeyName.Format(_T("Gantry Enable Offset Calibration"));
	Default.Format(_T("%d"), GetGantryEnableOffset());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, ErrorString) == true ) 	
	{	SetGantryEnableOffset((bool)(::_ttoi(String))); }

	KeyName.Format(_T("Use Home Sensor"));
	Default.Format(_T("%d"), GetUseHomeSensor());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, ErrorString) == true ) 	
	{	SetUseHomeSensor((::_ttoi(String))); }	

	KeyName.Format(_T("GPIO Bit In-Position"));
	Default.Format(_T("%d"), GetInputIoBit_INP());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, ErrorString) == true ) 	
	{	SetInputIoBit_INP((::_ttoi(String))); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::SaveIniFile(LPCTSTR Section, LPCTSTR Filename)
{
	const size_t textlen = 128;	
	CString KeyName = _T("");
	CString String = _T("");
	CString ErrorString = _T("");
	
	SetIniSection(Section);
	SetIniFilename(Filename);

	KeyName.Format(_T("Axis ID"));
	String.Format(_T("%d"), GetAxisID());
	if ( SaveINIData(Section, KeyName, String, Filename, ErrorString) == false ) 
	{	return false;	}		

	KeyName.Format(_T("Card ID"));
	String.Format(_T("%d"), GetCardID());
	if ( SaveINIData(Section, KeyName, String, Filename, ErrorString) == false ) 
	{	return false;	}		

	KeyName.Format(_T("Node ID"));
	String.Format(_T("%d"), GetNodeID());
	if ( SaveINIData(Section, KeyName, String, Filename, ErrorString) == false ) 
	{	return false;	}		

	KeyName.Format(_T("Slot ID"));
	String.Format(_T("%d"), GetSlotID());
	if ( SaveINIData(Section, KeyName, String, Filename, ErrorString) == false ) 
	{	return false;	}

	KeyName.Format(_T("Gantry ID"));
	String.Format(_T("%d"), GetGantryID());
	//if ( SaveINIData(Section, KeyName, String, Filename, ErrorString) == false ) 
	//{	return false;	}

	KeyName.Format(_T("Axis Type"));
	String.Format(_T("%d"), GetAxisType());
	if ( SaveINIData(Section, KeyName, String, Filename, ErrorString) == false ) 
	{	return false;	}

	KeyName.Format(_T("Gantry STD Offset"));
	String.Format(_T("%.2f"), GetGantryStdOffset());
	if ( SaveINIData(Section, KeyName, String, Filename, ErrorString) == false ) 
	{	return false;	}

	KeyName.Format(_T("Gantry Enable Offset Calibration"));
	String.Format(_T("%d"), GetGantryEnableOffset());
	if ( SaveINIData(Section, KeyName, String, Filename, ErrorString) == false ) 
	{	return false;	}

	KeyName.Format(_T("Use Home Sensor"));
	String.Format(_T("%d"), GetUseHomeSensor());
	if ( SaveINIData(Section, KeyName, String, Filename, ErrorString) == false ) 
	{	return false;	}

	KeyName.Format(_T("GPIO Bit In-Position"));
	String.Format(_T("%d"), GetInputIoBit_INP());
	if ( SaveINIData(Section, KeyName, String, Filename, ErrorString) == false ) 
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CMotionNode::GetIniSection() const
{
	return m_IniSection;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetIniSection(LPCTSTR val)
{
	m_IniSection = val;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CMotionNode::GetIniFilename() const
{
	return m_IniFilename;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetIniFilename(LPCTSTR val)
{
	m_IniFilename = val;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetAxisID(int val)
{
	m_AxisID = val;
}
//-------------------------------------------------------------------------------------//
int CMotionNode::GetAxisID() const
{
	return m_AxisID;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::CheckAxisIDValid() const//確認軸號有效
{
	if ( -1 == GetAxisID() ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetAxisName(const char *val)
{
	strcpy(m_AxisName, val);
}
//-------------------------------------------------------------------------------------//
const char* CMotionNode::GetAxisName() const
{
	return m_AxisName;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetCardID(int val)
{
	m_CardID = val;
}
//-------------------------------------------------------------------------------------//
int CMotionNode::GetCardID() const
{
	return m_CardID;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetNodeID(int val)
{
	m_NodeID = val;
}
//-------------------------------------------------------------------------------------//
int CMotionNode::GetNodeID() const
{
	return m_NodeID;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetSlotID(int val)
{
	m_SlotID = val;
}
//-------------------------------------------------------------------------------------//
int CMotionNode::GetSlotID() const
{
	return m_SlotID;
}
//-------------------------------------------------------------------------------------//	
void CMotionNode::SetGantryID(int val)
{
	m_GantryID = val;
}
//-------------------------------------------------------------------------------------//
int CMotionNode::GetGantryID() const
{
	return m_GantryID;
}
//-------------------------------------------------------------------------------------//

void CMotionNode::SetAxisType(MOTION_AXIS_TYPE val)
{
	m_AxisType = val;
}
//-------------------------------------------------------------------------------------//
MOTION_AXIS_TYPE CMotionNode::GetAxisType() const
{
	return m_AxisType;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetAxisBypass(bool val)
{
	m_AxisBypass = val;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::GetAxisBypass() const
{
	return m_AxisBypass;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetDriverModel(ECAT_DRIVER_MODEL val)
{
	m_DriverModel = val;
}
//-------------------------------------------------------------------------------------//
ECAT_DRIVER_MODEL CMotionNode::GetDriverModel() const
{
	return m_DriverModel;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetUseHomeSensor(bool val)
{
	m_UseHomeSensor = val;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::GetUseHomeSensor() const
{
	return m_UseHomeSensor;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetInputIoBit_INP(DWORD val)
{
	m_InputIoBit_INP = val;
}
//-------------------------------------------------------------------------------------//
DWORD CMotionNode::GetInputIoBit_INP() const
{
	return m_InputIoBit_INP;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::CheckUseInputIoBit_INP() const
{
	const DWORD NoUse=-1;
	if ( NoUse == GetInputIoBit_INP() )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::UpdateMotionParam(const CMotionNode &NodeRef)
{
	//Check
	if ( GetAxisID() != NodeRef.GetAxisID() ) { return false; }
	if ( GetGantryID() != NodeRef.GetGantryID() ) { return false; }

	const int CardID = GetCardID();
	const int NodeID = GetNodeID();
	const int SlotID = GetSlotID();
	const int GantryID=GetGantryID();
	CString   IniSection=GetIniSection();
	CString   IniFilename=GetIniFilename();
	MOTION_AXIS_TYPE AxisType=GetAxisType();
	ECAT_DRIVER_MODEL DriverModel=GetDriverModel();
	const double GantryStdOffset=GetGantryStdOffset();
	const bool   GantryEnableOffset=GetGantryEnableOffset();

	*this=NodeRef;
	SetCardID(CardID);
	SetNodeID(NodeID);
	SetSlotID(SlotID);
	SetGantryID(AxisType);
	SetDriverModel(DriverModel);
	SetIniSection(IniSection);
	SetIniFilename(IniFilename);
	SetGantryStdOffset(GantryStdOffset);
	SetGantryEnableOffset(GantryEnableOffset);
	return true;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetSignPositive(int val)
{
	m_SignPositive = val;
}
//-------------------------------------------------------------------------------------//
int CMotionNode::GetSignPositive() const
{
	return m_SignPositive;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetSignPositiveConvert(int val)
{
	m_SignPositiveConvert = val;
}
//-------------------------------------------------------------------------------------//
int CMotionNode::GetSignPositiveConvert() const
{
	return m_SignPositiveConvert;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::CheckConvertSignPositive() const//確認是否要轉換方向性
{
	if ( GetSignPositive() == FN_ENABLE ) { return false; }//正向不轉換
	if ( GetSignPositiveConvert() == FN_DISABLE ) { return false; }//不啟用轉換模式
	return true;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetHomePreMoveDis(double val)
{
	m_HomePreMoveDis = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetHomePreMoveDis() const
{
	return m_HomePreMoveDis;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetHomeOrgOffset(double val)
{
	m_HomeOrgOffset = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetHomeOrgOffset() const
{
	return m_HomeOrgOffset;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetStartVelocity(double val)
{
	m_StartVelocity = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetStartVelocity() const
{
	return m_StartVelocity;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetHomeVelocity(double val)
{
	m_HomeVelocity = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetHomeVelocity() const
{
	return m_HomeVelocity;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetMaxVelocity(double val)
{
	m_MaxVelocity = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetMaxVelocity() const
{
	return m_MaxVelocity;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetBoardInVelocity(double val)
{
	m_BoardInVelocity = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetBoardInVelocity() const
{
	return m_BoardInVelocity;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetBoardOutVelocity(double val)
{
	m_BoardOutVelocity = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetBoardOutVelocity() const
{
	return m_BoardOutVelocity;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetGrabbingVelocity(double val)
{
	m_GrabbingVelocity = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetGrabbingVelocity() const
{
	return m_GrabbingVelocity;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetGrabMapVelocity(double val)
{
	m_GrabMapVelocity = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetGrabMapVelocity() const
{
	return m_GrabMapVelocity;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetStageStartPos(double val)
{
	m_StageStartPos = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetStageStartPos() const
{
	return m_StageStartPos;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetStageLeavePos(double val)
{
	m_StageLeavePos = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetStageLeavePos() const
{
	return m_StageLeavePos;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetBeforePCBInPos(double val)
{
	m_BeforePCBInPos = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetBeforePCBInPos() const
{
	return m_BeforePCBInPos;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetPCBStopRPos_LA(double val)
{
	m_PCBStopRPos_LA = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetPCBStopRPos_LA() const
{
	return m_PCBStopRPos_LA;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetPCBStopRPos_LB(double val)
{
	m_PCBStopRPos_LB = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetPCBStopRPos_LB() const
{
	return m_PCBStopRPos_LB;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetPCBStopLPos_LA(double val)
{
	m_PCBStopLPos_LA = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetPCBStopLPos_LA() const
{
	return m_PCBStopLPos_LA;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetPCBStopLPos_LB(double val)
{
	m_PCBStopLPos_LB = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetPCBStopLPos_LB() const
{
	return m_PCBStopLPos_LB;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetLaneLedStopRPos_LA(double val)
{
	m_LaneLedStopRPos_LA = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetLaneLedStopRPos_LA() const
{
	return m_LaneLedStopRPos_LA;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetLaneLedSlowRPos_LA(double val)
{
	m_LaneLedSlowRPos_LA = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetLaneLedSlowRPos_LA() const
{
	return m_LaneLedSlowRPos_LA;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetLaneLedStopRPos_LB(double val)
{
	m_LaneLedStopRPos_LB = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetLaneLedStopRPos_LB() const
{
	return m_LaneLedStopRPos_LB;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetLaneLedSlowRPos_LB(double val)
{
	m_LaneLedSlowRPos_LB = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetLaneLedSlowRPos_LB() const
{
	return m_LaneLedSlowRPos_LB;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetLaneLedStopLPos_LA(double val)
{
	m_LaneLedStopLPos_LA = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetLaneLedStopLPos_LA() const
{
	return m_LaneLedStopLPos_LA;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetLaneLedSlowLPos_LA(double val)
{
	m_LaneLedSlowLPos_LA = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetLaneLedSlowLPos_LA() const
{
	return m_LaneLedSlowLPos_LA;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetLaneLedStopLPos_LB(double val)
{
	m_LaneLedStopLPos_LB = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetLaneLedStopLPos_LB() const
{
	return m_LaneLedStopLPos_LB;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetLaneLedSlowLPos_LB(double val)
{
	m_LaneLedSlowLPos_LB = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetLaneLedSlowLPos_LB() const
{
	return m_LaneLedSlowLPos_LB;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetJogStartVelocity(double val)
{
	m_JogStartVelocity = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetJogStartVelocity() const
{
	return m_JogStartVelocity;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetJogAccelerationTime(double val)
{
	m_JogAccelerationTime = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetJogAccelerationTime() const
{
	return m_JogAccelerationTime;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetJogDecelerationTime(double val)
{
	m_JogDecelerationTime = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetJogDecelerationTime() const
{
	return m_JogDecelerationTime;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetJogMaxVelocity(double val)
{
	m_JogMaxVelocity = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetJogMaxVelocity() const
{
	return m_JogMaxVelocity;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetJogMovingVelocity(double val)
{
	m_JogMovingVelocity = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetJogMovingVelocity() const
{
	return m_JogMovingVelocity;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetAccelerationValue(double val)
{
	m_AccelerationValue = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetAccelerationValue() const
{
	return m_AccelerationValue;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetAccelerationTime(double val)
{
	m_AccelerationTime = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetAccelerationTime() const
{
	return m_AccelerationTime;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetDecelerationTime(double val)
{
	m_DecelerationTime = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetDecelerationTime() const
{
	return m_DecelerationTime;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetAccelerationMinTime(double val)
{
	m_AccelerationMinTime = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetAccelerationMinTime() const
{
	return m_AccelerationMinTime;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetAdjustAccclerationDist(double val)
{
	m_AdjustAccclerationDist = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetAdjustAccclerationDist() const
{
	return m_AdjustAccclerationDist;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetMovingCurveMode(MOVE_CURVE_MODE val)
{
	m_MovingCurveMode = val;
}
//-------------------------------------------------------------------------------------//
MOVE_CURVE_MODE CMotionNode::GetMovingCurveMode() const
{
	return m_MovingCurveMode;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetAccTimeAdjustMode(ACC_TIME_ADJUST_MODE val)
{
	m_AccTimeAdjustMode = val;
}
//-------------------------------------------------------------------------------------//
ACC_TIME_ADJUST_MODE CMotionNode::GetAccTimeAdjustMode() const
{
	return m_AccTimeAdjustMode;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetTriangleCorrection(bool val)
{
	m_TriangleCorrection = val;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::GetTriangleCorrection() const
{
	return m_TriangleCorrection;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetTestTimeDistance(double val)
{
	m_TestTimeDistance = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetTestTimeDistance() const
{
	return m_TestTimeDistance;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetLimitMax(double val)
{
	m_LimitMax = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetLimitMax() const
{
	return m_LimitMax;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetLimitMin(double val)
{
	m_LimitMin = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetLimitMin() const
{
	return m_LimitMin;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetLimitMargin(double val)
{
	m_LimitMargin = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetLimitMargin() const
{
	return m_LimitMargin;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetLatchPos(double val)
{
	m_LatchPos = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetLatchPos() const
{
	return m_LatchPos;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetCommandPos(double val)
{
	m_CommandPos = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetCommandPos() const
{
	return m_CommandPos;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetCommandOffline(double val)
{
	m_CommandOffline = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetCommandOffline() const
{
	return m_CommandOffline;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetCommandMaxVelocity(double val)
{
	m_CommandMaxVelocity = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetCommandMaxVelocity() const
{
	return m_CommandMaxVelocity;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetCommandAccelerationTime(double val)
{
	m_CommandAccelerationTime = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetCommandAccelerationTime() const
{
	return m_CommandAccelerationTime;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetPosTolerance() const
{
	return 1.0;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetFreeRunVel(double val)
{
	m_FreeRunVel = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetFreeRunVel() const
{
	return m_FreeRunVel;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetGantryOffset(double val)
{
	m_GantryOffset = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetGantryOffset() const	
{
	return m_GantryOffset;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetGantryEnableOffset(bool val)
{
	m_GantryEnableOffset = val;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::GetGantryEnableOffset() const
{
	return m_GantryEnableOffset;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetGantryStdOffsetDefault()
{
	SetGantryStdOffset(GANTRY_DEFAULT_STD_OFFSET);
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::CheckGantryStdOffsetValid() const
{
	double Std=GetGantryStdOffset();
	double Default=GANTRY_DEFAULT_STD_OFFSET;
	if ( fabs(Std-Default) < 1 )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetGantryStdOffset(double val)
{
	m_GantryStdOffset = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetGantryStdOffset() const
{
	return m_GantryStdOffset;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetGantryCalcPos(double val)
{
	m_GantryCalcPos = val;
}
//-------------------------------------------------------------------------------------//
double CMotionNode::GetGantryCalcPos() const
{
	return m_GantryCalcPos;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetIsHomed(bool val)
{
	m_IsHomed = val;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::GetIsHomed() const
{
	return m_IsHomed;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetIsEnabled(bool val)
{
	m_IsEnabled = val;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::GetIsEnabled() const
{
	return m_IsEnabled;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetJogDirection(int val)
{
	m_JogDirection = val;
}
//-------------------------------------------------------------------------------------//
int CMotionNode::GetJogDirection() const
{
	return m_JogDirection;
}
//-------------------------------------------------------------------------------------//
void CMotionNode::SetIsWaitForInp(bool val)
{
	m_IsWaitForInp = val;
}
//-------------------------------------------------------------------------------------//
bool CMotionNode::GetIsWaitForInp() const	
{
	return m_IsWaitForInp;
}
//-------------------------------------------------------------------------------------//