// MotionNode.h: interface for the CMotionNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOTIONNODE_H__4A031380_6EE2_43D7_B3E4_580907488214__INCLUDED_)
#define AFX_MOTIONNODE_H__4A031380_6EE2_43D7_B3E4_580907488214__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Motion_Define.h"
//-------------------------------------------------------------------------------------//
enum MOTION_AXIS_TYPE//運動軸樣式
{
	MOTION_AXIS_MASTER  = 1,
	MOTION_AXIS_SLAVE   = 2,
	MOTION_AXIS_RETURN
};
//-------------------------------------------------------------------------------------//
class CMotionAxis;
//-------------------------------------------------------------------------------------//
class CMotionNode  
{
private:
	//---------------------------------------------------------------------------------//	
	CString                    m_IniSection;
	CString                    m_IniFilename;
	//---------------------------------------------------------------------------------//		
	int                        m_AxisID;//軸號
	int                        m_CardID;//第幾張卡
	int                        m_NodeID;//通道號或站號
	int                        m_SlotID;//子號
	int                        m_GantryID;//龍門編號
	bool                       m_AxisBypass;//軸跳過
	MOTION_AXIS_TYPE           m_AxisType;//軸樣式	
	ECAT_DRIVER_MODEL          m_DriverModel;//驅動器型號
	char                       m_AxisName[32];//軸名
	bool                       m_UseHomeSensor;//使用原點感應器
	DWORD                      m_InputIoBit_INP;//輸入IO的狀態-定位訊號-PCIE_L221_B1D0
	//---------------------------------------------------------------------------------//	
	int                        m_SignPositive;//軸方向與Cad的正負方向
	int                        m_SignPositiveConvert;//內部方向性轉換
	//---------------------------------------------------------------------------------//	
	double                     m_HomePreMoveDis;//歸零前移動距離
	double                     m_HomeOrgOffset;//歸零後的偏移值
	//---------------------------------------------------------------------------------//	
	double                     m_StartVelocity;//起始速度  
	double                     m_HomeVelocity;//歸零速度
	double                     m_MaxVelocity;//一般移動最大速度, 	
	double                     m_BoardInVelocity;//進板時的速度	
	double                     m_BoardOutVelocity;//出板時的速度	
	double                     m_GrabbingVelocity;//取像時移動最大速度, 
	double                     m_GrabMapVelocity;//取底圖時移動最大速度, 
	//---------------------------------------------------------------------------------//	
	double                     m_StageStartPos;//機台起始的位置
	double                     m_StageLeavePos;//機台離開的位置
	double                     m_BeforePCBInPos;//進板前座標
	double                     m_PCBStopRPos_LA;//A軌道右停板的位置	
	double                     m_PCBStopRPos_LB;//B軌道右停板的位置
	double                     m_PCBStopLPos_LA;//A軌道左停板的位置	
	double                     m_PCBStopLPos_LB;//B軌道左停板的位置
	double                     m_LaneLedStopRPos_LA;//A軌道LED右停板的位置
	double                     m_LaneLedSlowRPos_LA;//A軌道LED右減速的位置
	double                     m_LaneLedStopRPos_LB;//B軌道LED右停板的位置
	double                     m_LaneLedSlowRPos_LB;//B軌道LED右減速的位置
	double                     m_LaneLedStopLPos_LA;//A軌道LED左停板的位置
	double                     m_LaneLedSlowLPos_LA;//A軌道LED左減速的位置
	double                     m_LaneLedStopLPos_LB;//B軌道LED左停板的位置
	double                     m_LaneLedSlowLPos_LB;//B軌道LED左減速的位置
	//---------------------------------------------------------------------------------//	
	//JOG
	double                     m_JogStartVelocity;//自由移動時的初始速度	
	double                     m_JogAccelerationTime;//自由移動時的加速度時間
	double                     m_JogDecelerationTime;//自由移動時的減速度時間
	double                     m_JogMaxVelocity;//自由移動時的最大速度 mm/s/s
	double                     m_JogMovingVelocity;//自由移動時的移動速度 mm/s/s
	//---------------------------------------------------------------------------------//	
	double                     m_AccelerationValue;//加速度, 單位mm/s/s	
	double                     m_AccelerationTime;//加速度時間, sec	
	double                     m_DecelerationTime;//減速度時間, sec	
	double                     m_AccelerationMinTime;//加速度最小時間, sec	
	double                     m_AdjustAccclerationDist;//調整加速度啟動距離-單位um
	MOVE_CURVE_MODE            m_MovingCurveMode;//移動曲線	
	ACC_TIME_ADJUST_MODE       m_AccTimeAdjustMode;//加減速度時間調整模式	
	bool                       m_TriangleCorrection;//三角波抑制	
	//---------------------------------------------------------------------------------//	
	double                     m_TestTimeDistance;//測驗時間的偏移量, um	
	//---------------------------------------------------------------------------------//	
	double                     m_LimitMax; //最大範圍
	double                     m_LimitMin; //最小範圍
	double                     m_LimitMargin;//極限範圍內縮值	
	//---------------------------------------------------------------------------------//
	double                     m_GantryStdOffset;//龍門標準偏差值
	bool                       m_GantryEnableOffset;//龍門啟用偏差值
	//---------------------------------------------------------------------------------//
	//以下為暫存的參數
	double                     m_LatchPos;//擷取到位置
	double                     m_CommandPos;//下達位置
	bool                       m_CommandOffline;//下達位置-離線模式
	double                     m_CommandMaxVelocity;//下達最高速度
	double                     m_CommandAccelerationTime;//下達的加速度時間
	double                     m_FreeRunVel;//搖桿模式的速度
	double                     m_GantryOffset;//龍門偏差值
	double                     m_GantryCalcPos;//龍門校正位置	
	bool                       m_IsHomed;//是否已經歸零過
	bool                       m_IsEnabled;//是否啟用過
	int                        m_JogDirection;//搖桿方向
	bool                       m_IsWaitForInp;//是否等INP訊號
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitial();
	//---------------------------------------------------------------------------------//	
	bool                       SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                       LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//
	CMotionNode();
	virtual ~CMotionNode();
	//---------------------------------------------------------------------------------//	
	//參數檔案
	bool                       LoadIniFile();
	bool                       SaveIniFile();

	bool                       LoadIniFile(LPCTSTR Section, LPCTSTR Filename);
	bool                       SaveIniFile(LPCTSTR Section, LPCTSTR Filename);
	//---------------------------------------------------------------------------------//		
	LPCTSTR                    GetIniSection() const;
	void                       SetIniSection(LPCTSTR val);

	LPCTSTR                    GetIniFilename() const;
	void                       SetIniFilename(LPCTSTR val);
	//---------------------------------------------------------------------------------//		
	//軸號
	void                       SetAxisID(int val);
	int                        GetAxisID() const;
	bool                       CheckAxisIDValid() const;//確認軸號有效
	//---------------------------------------------------------------------------------//		
	void                       SetAxisName(const char *val);
	const char*                GetAxisName() const;
	//---------------------------------------------------------------------------------//	
	//第幾張卡
	void                       SetCardID(int val);
	int                        GetCardID() const;
	//---------------------------------------------------------------------------------//	
	//通道號或站號
	void                       SetNodeID(int val);
	int                        GetNodeID() const;
	//---------------------------------------------------------------------------------//	
	//子號
	void                       SetSlotID(int val);
	int                        GetSlotID() const;
	//---------------------------------------------------------------------------------//	
	//龍門編號
	void                       SetGantryID(int val);
	int                        GetGantryID() const;
	//---------------------------------------------------------------------------------//	
	//軸樣式
	void                       SetAxisType(MOTION_AXIS_TYPE val);
	MOTION_AXIS_TYPE           GetAxisType() const;
	//---------------------------------------------------------------------------------//		
	//軸跳過
	void                       SetAxisBypass(bool val);
	bool                       GetAxisBypass() const;
	//---------------------------------------------------------------------------------//	
	//驅動器型號
	void                       SetDriverModel(ECAT_DRIVER_MODEL val);
	ECAT_DRIVER_MODEL          GetDriverModel() const;
	//---------------------------------------------------------------------------------//	
	//使用原點感應器
	void                       SetUseHomeSensor(bool val);
	bool                       GetUseHomeSensor() const;
	//---------------------------------------------------------------------------------//	
	//輸入IO的狀態-定位訊號-PCIE_L221_B1D0		
	void                       SetInputIoBit_INP(DWORD val);
	DWORD                      GetInputIoBit_INP() const;	
	bool                       CheckUseInputIoBit_INP() const;
	//---------------------------------------------------------------------------------//
	bool                       UpdateMotionParam(const CMotionNode &NodeRef);
	//---------------------------------------------------------------------------------//	
	//軸方向與Cad的正負方向
	void                       SetSignPositive(int val);
	int                        GetSignPositive() const;
	//---------------------------------------------------------------------------------//	
	//內部方向性轉換
	void                       SetSignPositiveConvert(int val);
	int                        GetSignPositiveConvert() const;
	bool                       CheckConvertSignPositive() const;//確認是否要轉換方向性
	//---------------------------------------------------------------------------------//
	//歸零前移動距離
	void                       SetHomePreMoveDis(double val);
	double                     GetHomePreMoveDis() const;
	//---------------------------------------------------------------------------------//		
	//歸零後的偏移值
	void                       SetHomeOrgOffset(double val);
	double                     GetHomeOrgOffset() const;	
	//---------------------------------------------------------------------------------//	
	//起始速度  
	void                       SetStartVelocity(double val);
	double                     GetStartVelocity() const;	
	//---------------------------------------------------------------------------------//	
	//歸零速度
	void                       SetHomeVelocity(double val);
	double                     GetHomeVelocity() const;	
	//---------------------------------------------------------------------------------//	
	//一般移動最大速度
	void                       SetMaxVelocity(double val);
	double                     GetMaxVelocity() const;	
	//---------------------------------------------------------------------------------//	
	//進板時的速度
	void                       SetBoardInVelocity(double val);
	double                     GetBoardInVelocity() const;	
	//---------------------------------------------------------------------------------//		
	//出板時的速度
	void                       SetBoardOutVelocity(double val);
	double                     GetBoardOutVelocity() const;	
	//---------------------------------------------------------------------------------//	
	//取像時移動最大速度
	void                       SetGrabbingVelocity(double val);
	double                     GetGrabbingVelocity() const;	
	//---------------------------------------------------------------------------------//		
	//取底圖時移動最大速度
	void                       SetGrabMapVelocity(double val);
	double                     GetGrabMapVelocity() const;	
	//---------------------------------------------------------------------------------//		
	//機台起始的位置
	void                       SetStageStartPos(double val);
	double                     GetStageStartPos() const;	
	//---------------------------------------------------------------------------------//		
	//機台離開的位置
	void                       SetStageLeavePos(double val);
	double                     GetStageLeavePos() const;	
	//---------------------------------------------------------------------------------//		
	//進板前座標
	void                       SetBeforePCBInPos(double val);
	double                     GetBeforePCBInPos() const;	
	//---------------------------------------------------------------------------------//		
	//A軌道右停板的位置	
	void                       SetPCBStopRPos_LA(double val);
	double                     GetPCBStopRPos_LA() const;	
	//---------------------------------------------------------------------------------//		
	//B軌道右停板的位置
	void                       SetPCBStopRPos_LB(double val);
	double                     GetPCBStopRPos_LB() const;	
	//---------------------------------------------------------------------------------//		
	//A軌道左停板的位置	
	void                       SetPCBStopLPos_LA(double val);
	double                     GetPCBStopLPos_LA() const;	
	//---------------------------------------------------------------------------------//		
	//B軌道左停板的位置
	void                       SetPCBStopLPos_LB(double val);
	double                     GetPCBStopLPos_LB() const;	
	//---------------------------------------------------------------------------------//		
	//A軌道LED右停板的位置
	void                       SetLaneLedStopRPos_LA(double val);
	double                     GetLaneLedStopRPos_LA() const;	
	//---------------------------------------------------------------------------------//		
	//A軌道LED右減速的位置
	void                       SetLaneLedSlowRPos_LA(double val);
	double                     GetLaneLedSlowRPos_LA() const;	
	//---------------------------------------------------------------------------------//		
	//B軌道LED右停板的位置
	void                       SetLaneLedStopRPos_LB(double val);
	double                     GetLaneLedStopRPos_LB() const;	
	//---------------------------------------------------------------------------------//		
	//B軌道LED右減速的位置
	void                       SetLaneLedSlowRPos_LB(double val);
	double                     GetLaneLedSlowRPos_LB() const;	
	//---------------------------------------------------------------------------------//		
	//A軌道LED左停板的位置
	void                       SetLaneLedStopLPos_LA(double val);
	double                     GetLaneLedStopLPos_LA() const;	
	//---------------------------------------------------------------------------------//		
	//A軌道LED左減速的位置
	void                       SetLaneLedSlowLPos_LA(double val);
	double                     GetLaneLedSlowLPos_LA() const;	
	//---------------------------------------------------------------------------------//		
	//B軌道LED左停板的位置
	void                       SetLaneLedStopLPos_LB(double val);
	double                     GetLaneLedStopLPos_LB() const;	
	//---------------------------------------------------------------------------------//		
	//B軌道LED左減速的位置
	void                       SetLaneLedSlowLPos_LB(double val);
	double                     GetLaneLedSlowLPos_LB() const;	
	//---------------------------------------------------------------------------------//		
	//JOG
	//自由移動時的初始速度
	void                       SetJogStartVelocity(double val);
	double                     GetJogStartVelocity() const;	
	//---------------------------------------------------------------------------------//
	//自由移動時的加速度時間
	void                       SetJogAccelerationTime(double val);
	double                     GetJogAccelerationTime() const;	
	//---------------------------------------------------------------------------------//		
	//自由移動時的減速度時間
	void                       SetJogDecelerationTime(double val);
	double                     GetJogDecelerationTime() const;	
	//---------------------------------------------------------------------------------//		
	//自由移動時的最大速度 mm/s/s
	void                       SetJogMaxVelocity(double val);
	double                     GetJogMaxVelocity() const;	
	//---------------------------------------------------------------------------------//		
	//自由移動時的移動速度 mm/s/s
	void                       SetJogMovingVelocity(double val);
	double                     GetJogMovingVelocity() const;		
	//---------------------------------------------------------------------------------//	
	//加速度, 單位mm/s/s	
	void                       SetAccelerationValue(double val);
	double                     GetAccelerationValue() const;	
	//---------------------------------------------------------------------------------//		
	//加速度時間, sec	
	void                       SetAccelerationTime(double val);
	double                     GetAccelerationTime() const;	
	//---------------------------------------------------------------------------------//		
	//減速度時間, sec	
	void                       SetDecelerationTime(double val);
	double                     GetDecelerationTime() const;	
	//---------------------------------------------------------------------------------//		
	//加速度最小時間, sec	
	void                       SetAccelerationMinTime(double val);
	double                     GetAccelerationMinTime() const;	
	//---------------------------------------------------------------------------------//		
	//調整加速度啟動距離-單位um
	void                       SetAdjustAccclerationDist(double val);
	double                     GetAdjustAccclerationDist() const;	
	//---------------------------------------------------------------------------------//		
	//移動曲線模式
	void                       SetMovingCurveMode(MOVE_CURVE_MODE val);
	MOVE_CURVE_MODE            GetMovingCurveMode() const;	
	//---------------------------------------------------------------------------------//		
	//加減速度時間調整模式	
	void                       SetAccTimeAdjustMode(ACC_TIME_ADJUST_MODE val);
	ACC_TIME_ADJUST_MODE       GetAccTimeAdjustMode() const;	
	//---------------------------------------------------------------------------------//		
	//三角波抑制	
	void                       SetTriangleCorrection(bool val);
	bool                       GetTriangleCorrection() const;	
	//---------------------------------------------------------------------------------//			
	//測驗時間的偏移量, um	
	void                       SetTestTimeDistance(double val);
	double                     GetTestTimeDistance() const;	
	//---------------------------------------------------------------------------------//		
	//最大範圍
	void                       SetLimitMax(double val);
	double                     GetLimitMax() const;	
	//---------------------------------------------------------------------------------//		
	//最小範圍
	void                       SetLimitMin(double val);
	double                     GetLimitMin() const;	
	//---------------------------------------------------------------------------------//		
	//極限範圍內縮值
	void                       SetLimitMargin(double val);
	double                     GetLimitMargin() const;	
	//---------------------------------------------------------------------------------//
	//擷取到位置
	void                       SetLatchPos(double val);
	double                     GetLatchPos() const;	
	//---------------------------------------------------------------------------------//
	//下達位置
	void                       SetCommandPos(double val);
	double                     GetCommandPos() const;	
	//---------------------------------------------------------------------------------//	
	//下達位置-離線模式
	void                       SetCommandOffline(double val);
	double                     GetCommandOffline() const;	
	//---------------------------------------------------------------------------------//	
	//下達最高速度
	void                       SetCommandMaxVelocity(double val);
	double                     GetCommandMaxVelocity() const;	
	//---------------------------------------------------------------------------------//	
	//下達的加速度時間
	void                       SetCommandAccelerationTime(double val);
	double                     GetCommandAccelerationTime() const;	
	//---------------------------------------------------------------------------------//	
	//位置誤差
	double                     GetPosTolerance() const;
	//---------------------------------------------------------------------------------//	
	//搖桿模式的速度
	void                       SetFreeRunVel(double val);
	double                     GetFreeRunVel() const;	
	//---------------------------------------------------------------------------------//
	//龍門偏差值
	void                       SetGantryOffset(double val);
	double                     GetGantryOffset() const;	
	//---------------------------------------------------------------------------------//		
	//龍門校正位置
	void                       SetGantryCalcPos(double val);
	double                     GetGantryCalcPos() const;	
	//---------------------------------------------------------------------------------//		
	//龍門啟用偏差校正
	void                       SetGantryEnableOffset(bool val);
	bool                       GetGantryEnableOffset() const;	
	//---------------------------------------------------------------------------------//
	//龍門標準偏差值
	void                       SetGantryStdOffsetDefault();
	bool                       CheckGantryStdOffsetValid() const;
	void                       SetGantryStdOffset(double val);
	double                     GetGantryStdOffset() const;	
	//---------------------------------------------------------------------------------//	
	//是否已經歸零過
	void                       SetIsHomed(bool val);
	bool                       GetIsHomed() const;	
	//---------------------------------------------------------------------------------//	
	//是否啟用過
	void                       SetIsEnabled(bool val);
	bool                       GetIsEnabled() const;	
	//---------------------------------------------------------------------------------//	
	//搖桿方向
	void                       SetJogDirection(int val);
	int                        GetJogDirection() const;	
	//---------------------------------------------------------------------------------//
	//是否等INP訊號
	void                       SetIsWaitForInp(bool val);
	bool                       GetIsWaitForInp() const;	
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_MOTIONNODE_H__4A031380_6EE2_43D7_B3E4_580907488214__INCLUDED_)
