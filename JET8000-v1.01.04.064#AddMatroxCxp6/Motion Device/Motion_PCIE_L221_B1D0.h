// Motion_PCIE_L221_B1D0.h: interface for the CMotion_PCIE_L221_B1D0 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOTION_PCIE_L221_B1D0_H__AB2427D8_F528_453A_A3C1_F7B371E136D8__INCLUDED_)
#define AFX_MOTION_PCIE_L221_B1D0_H__AB2427D8_F528_453A_A3C1_F7B371E136D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Motion_Basic.h"
//-------------------------------------------------------------------------------------//
#if MOTION_DERIVE_MODE == MOTION_PCIE_L221_B1D0
#ifndef MOTION_OBJ_DISABLE
	#ifdef _X64
		#pragma comment(lib, "..\\JET8000_Library\\PCIE_L221_B1D0\\x64\\Lib\\EtherCAT_DLL_x64.lib")
	#else
		#pragma comment(lib, "..\\JET8000_Library\\PCIE_L221_B1D0\\x32\\Lib\\EtherCAT_DLL.lib")
	#endif//_X64
#endif

#include "..\\JET8000_Library\\PCIE_L221_B1D0\\Include\\EtherCAT_DLL.h"
#include "..\\JET8000_Library\\PCIE_L221_B1D0\\Include\\EtherCat_DLL_Err.h"
//---------------------------------------------------------------------------------------//
class CMotion_ECAT_Node
{
private:
	//---------------------------------------------------------------------------------//	
	U16                        m_CardNo;//卡號
	U16                        m_NodeNo;//站號
	U16                        m_SlotNo;//子號
	ECAT_DRIVER_MODEL          m_DriverModel;//型號
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	CMotion_ECAT_Node(U16 CardNo=-1, U16 NodeNo=-1, U16 SlotNo=0) 
	{
		m_CardNo = CardNo;
		m_NodeNo = NodeNo;
		m_SlotNo = SlotNo;
		m_DriverModel=ECAT_DRIVER_NONE;
	}
	~CMotion_ECAT_Node() {}
	//---------------------------------------------------------------------------------//		
	void                       SetCardNo(U16 val) { m_CardNo = val; }
	U16                        GetCardNo() const { return m_CardNo; }
	//---------------------------------------------------------------------------------//		
	void                       SetNodeNo(U16 val) { m_NodeNo = val; }
	U16                        GetNodeNo() const { return m_NodeNo; }
	//---------------------------------------------------------------------------------//		
	void                       SetSlotNo(U16 val) { m_SlotNo = val; }
	U16                        GetSlotNo() const { return m_SlotNo; }
	//---------------------------------------------------------------------------------//	
	void                       SetDriverModel(ECAT_DRIVER_MODEL val) { m_DriverModel = val; }
	ECAT_DRIVER_MODEL          GetDriverModel() const { return m_DriverModel; }
	//---------------------------------------------------------------------------------//
	bool                       CompareNode(const CMotion_ECAT_Node &Node) const
	{
		if ( GetCardNo() != Node.GetCardNo() ) { return false; }
		if ( GetNodeNo() != Node.GetNodeNo() ) { return false; }
		if ( GetSlotNo() != Node.GetSlotNo() ) { return false; }
		return true;
	}
	//---------------------------------------------------------------------------------//
};
//---------------------------------------------------------------------------------------//
class CMotion_PCIE_L221_B1D0 : public CMotion_Basic  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CMotion_PCIE_L221_B1D0)
	//---------------------------------------------------------------------------------//	
protected:	
	//----------------------------------------------------------------------------------//	
	std::vector<CMotion_ECAT_Node> m_ECatNodeList;
	bool                       BuildCardNoList(std::vector<U16> &CardNoList);
	bool                       AddETherCatNode(const CMotion_ECAT_Node &NodeRef);
	//----------------------------------------------------------------------------------//
	virtual bool               BuildMotionNodeList();//建立運動點列表
	virtual bool               BuildMotionAxisList();//建立運動軸列表		
	virtual bool               CheckMotionNodeValid(const CMotionNode &Ref);//確認軸控節點	
	virtual bool               UpdateMotionAxisDriverModel(CMotionAxis &Ref);//更新運動軸的驅動器型號
	//----------------------------------------------------------------------------------//	
	bool                       CheckReturnOK(U16 ret);
	bool                       ReleaseMotionCard();	
	bool                       OpenMotionCard();
	bool                       OpenMotionCard(U16 CardNo);
	bool                       InitialMotionCard();
	bool                       InitializeForMachine();//特定機台初始化		
	bool                       InitializeForJET8000();//JET8000的初始化
	//----------------------------------------------------------------------------------//
	int                        CheckSupportHomeMoveMode(CMotionAxis *MotionAxisPtr);
	bool                       CheckSupportInPosition(ECAT_DRIVER_MODEL DriverModel) const;	
	//----------------------------------------------------------------------------------//
	bool                       WriteAxisParam_INP(const CMotionNode &MotionNodeRef, bool bINP);
	//----------------------------------------------------------------------------------//		
	bool                       GetAxisStatusWord(int Axis, U16 &Status);
	bool                       ReadNodeStatusWord(CMotionNode *MotionNodePtr, U16 &Status);	
	bool                       ReadAxisStatusWord_Ready(CMotionAxis *MotionAxisPtr, bool &bReady);
	bool                       ReadNodeStatusWord_Ready(CMotionNode *MotionNodePtr, bool &bReady);
	bool                       ReadAxisStatusWord_Enable(CMotionAxis *MotionAxisPtr, bool &bEnable);
	bool                       ReadNodeStatusWord_Enable(CMotionNode *MotionNodePtr, bool &bEnable);
	bool                       ReadAxisStatusWord_Alarm(CMotionAxis *MotionAxisPtr, bool &bAlarm);
	bool                       ReadNodeStatusWord_Alarm(CMotionNode *MotionNodePtr, bool &bAlarm);
	bool                       ReadAxisStatusWord_INP(CMotionAxis *MotionAxisPtr, bool &bINP);
	bool                       ReadNodeStatusWord_INP(CMotionNode *MotionNodePtr, bool &bINP);//In Position
	//----------------------------------------------------------------------------------//	
	bool                       WaitForNodeDone(CMotionNode *MotionNodePtr);
	//----------------------------------------------------------------------------------//	
	bool                       OneAxisMoveTo(U16 Axis, F64 Dist, F64 StrVel, F64 MaxVel,F64 Tacc,F64 Tdec, MOVE_CURVE_MODE VelCurve, MOVE_COORDINATE_MODE CoordMode, F64 SVacc=-1, F64 SVdec=-1);
	//----------------------------------------------------------------------------------//	
	bool                       FreeRun(U16 Axis, F64 StrVel, F64 MaxVel, F64 Tacc);
	//----------------------------------------------------------------------------------//	
	bool                       SetORG(U16 Axis);
	bool                       SetORGAll();	
	bool                       Stop(U16 Axis);
	//----------------------------------------------------------------------------------//
	void                       GetErrorCodeText(const int status, CString &str);//取得錯誤描述	
	LPCTSTR                    GetAxisStatusText(int IO);//取得軸狀態的文字
	void                       GetAxisStatusIDList(std::vector<int> &IDList);
	int                        GetAxisStatus(U16 Axis, CString &str);//回傳該軸狀態, -1取資料出現異常, 要不則是狀態編碼, if pString==NULL, 不取錯誤文字	
	//----------------------------------------------------------------------------------//
	bool                       ForwardTriggerProcess_JET7000S();//正向移動
	bool                       BackwardTriggerProcess_JET7000S();//逆向移動	
	//----------------------------------------------------------------------------------//
	bool                       ResetMotionCardCommandPos(int Axis);//重設軸控卡的命令位置
	//----------------------------------------------------------------------------------//
	bool                       ExecHome(int Axis);
	bool                       ExecEnable(int Axis);
	bool                       ExecDisable(int Axis);
	bool                       ExecSetIsWaitForInPosition(int Axis, bool Iswait);
	bool                       ExecWaitForDone(int Axis, int MaxPreCounts);	
	bool                       ExecMoveTo(int Axis, double TargetPos, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity);
	bool                       ExecTriggerMoveTo(int Axis, double TargetPos, bool IsModifyVelocity);//觸發移動至哪裡
	bool                       ExecXYMoveTo(double PosX, double PosY, bool Offline, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity);//2軸移動，但非同動唷
	bool                       ExecXYZMoveTo(double PosX, double PosY, double PosZ, bool Offline, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity);//3軸移動，但非同動唷
	bool                       ExecDoFaultAck(int Axis);
	//----------------------------------------------------------------------------------//
	bool                       ExecHome_Slave(CMotionAxis *MotionAxisPtr, int SignPositive, double HomeVelocity, double HomeAccTime);
	bool                       ExecHome_CSP(CMotionAxis *MotionAxisPtr, int SignPositive, double HomeVelocity, double HomeAccTime);
	bool                       ExecHome_TouchPro(CMotionAxis *MotionAxisPtr, int SignPositive, double HomeVelocity, double HomeAccTime);
	//----------------------------------------------------------------------------------//
	bool                       ReadMotionAxisLatchPosition(CMotionAxis *MotionAxisPtr);
	bool                       PrepareMotionNodeGantryOffset(CMotionNode *MotionNodePtr);//預備軸控的龍門偏移
	bool                       PrepareMotionAxisGantryOffset(CMotionAxis *MotionAxisPtr);//預備軸控的龍門偏移
	bool                       FetchMotionAxisGantryOffset(CMotionAxis *MotionAxisPtr);//取得軸的龍門偏移
	bool                       CorrectMotionAxisGantryOffset(CMotionAxis *MotionAxisPtr);//校正軸的龍門偏移
	bool                       CalcMotionAxisGantryOffset(CMotionAxis *MotionAxisPtr, bool &bCali);//計算軸的龍門偏移
	bool                       CalibrateMotionAxisGantryOffset(CMotionAxis *MotionAxisPtr);//校正軸的龍門偏移
	bool                       MoveMotionAxisSlavePosition(CMotionAxis *MotionAxisPtr);//移動軸的子軸位置
	bool                       EnableMotionAxisSlaveGantry(CMotionAxis *MotionAxisPtr, bool bEnable);//啟用軸的子軸龍門	
	bool                       Exec_ECAT_Slave_Motion_Set_TouchProbe_Disable(CMotionNode *MotionNodePtr);//關閉『觸發擷取』功能
	bool                       Exec_ECAT_Slave_Motion_Set_TouchProbe_QuickStart(CMotionNode *MotionNodePtr);//快速致能『觸發擷取』功能
	bool                       Exec_ECAT_Slave_Motion_Get_TouchProbe_Status(CMotionNode *MotionNodePtr, U16 *Status);//取得當前『觸發擷取』功能的狀態
	bool                       Exec_ECAT_Slave_Motion_Get_TouchProbe_Position(CMotionNode *MotionNodePtr, I32 *LatchPosition);//取得當前擷取到的位置
	bool                       Exec_ECAT_Slave_Motion_Set_TouchProbe_Config(CMotionNode *MotionNodePtr, U16 TriggerMode, U16 Signal_Source);//設定『觸發擷取』功能的行為模式		
	bool                       Exec_ECAT_Slave_PDO_Set_OD_Data(CMotionNode *MotionNodePtr, U16 ODIndex, U16 ODSubIndex, U8 *Data);
	bool                       Exec_ECAT_Slave_PDO_Get_OD_Data(CMotionNode *MotionNodePtr, U16 IOType, U16 ODIndex, U16 ODSubIndex, U8 *Data);
	//----------------------------------------------------------------------------------//
public:
	//----------------------------------------------------------------------------------//
	CMotion_PCIE_L221_B1D0();
	virtual ~CMotion_PCIE_L221_B1D0();
	//----------------------------------------------------------------------------------//
	virtual  bool              PreInitMotion();//預先初始化
	virtual  bool              InitialMotion();//初始化	
	virtual  bool              ReleaseMotion();//釋放資源

	virtual bool               SaveMotionParamInternal();//儲存運動內部參數	
	virtual bool               GetMotionIsReady();//取得運動系統是否正常
	virtual bool               Home(int Axis);
	virtual bool               Enable(int Axis);
	virtual bool               Disable(int Axis);
	virtual bool               SetIsWaitForInPosition(int Axis, bool Iswait);//設定是否等待定位停止
	virtual bool               WaitForDone(int Axis, int MaxPreCounts=2000);	
	virtual bool               WaitForDone_Backup(int Axis, int MaxPreCounts=2000);	
	virtual bool               MoveTo(int Axis, double TargetPos, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity=true);
	virtual bool               TriggerMoveTo(int Axis, double TargetPos, bool IsModifyVelocity=true);//觸發移動至哪裡
	virtual bool               XYMoveTo(double PosX, double PosY, bool Offline=false, MOTION_MOVING_MODE MovingMode=MOTION_MOVING_NORMAL, bool IsModifyVelocity=true);//2軸移動，但非同動唷
	virtual bool               XYZMoveTo(double PosX, double PosY, double PosZ, bool Offline=false, MOTION_MOVING_MODE MovingMode=MOTION_MOVING_NORMAL, bool IsModifyVelocity=true);//3軸移動，但非同動唷
	virtual bool               GetEncode(int Axis, double &Encode, bool offline=false);//取得該軸的光學尺座標
	virtual bool               GetCurrentPos(double &X, double &Y, bool offline=false);//取得目前機台位置
	virtual bool               GetCurrentPos(double &X, double &Y, double &Z, bool offline=false);//取得目前機台位置	
	virtual bool               GetCurrentRawPos(double &X, double &Y, double &Z, bool offline=false);//取得目前機台原始位置
	virtual bool               StartFreeRun(int Axis, bool Dir);//Dir +為正方向, -為負方向
	virtual bool               StopFreeRun(int Axis);//停止FreeRun	
	virtual bool               ForwardTriggerProcess();//正向移動
	virtual bool               BackwardTriggerProcess();//逆向移動
	virtual bool               EnableCompareTrigger(bool IsEnable);//是否啟動同步比較送外部觸發訊號
	virtual bool               StopCompareTrigger(bool IsStop);//是否停止同步比較送外部觸發訊號
	virtual void               SetTriggerParameter(double SP, double EP, double Start, double End, double Interval, int RepeatCounts, int YMaxCounts, double YOffset, int ORGX, int ORGY, int ORGZ);
	virtual bool               ConfigTriggerTable(bool IsReBuild);
	
	virtual bool               GetIsEnable(int Axis);//該軸是否為Serve ON, 也就是有送電來積磁
	virtual bool               GetIsReady(int Axis);//Driver傳回該軸是否為RDY狀態
	virtual bool               GetIsAlarm(int Axis);//Driver傳回該軸是否為Alarm狀態
	virtual bool               GetIsERCActive(int Axis);//ERC Active
	virtual bool               GetIsInPosition(int Axis);//Driver傳回該軸是否為In Position
	virtual bool               GetIsNLimit(int Axis);//該軸的是否碰觸到副極限
	virtual bool               GetIsPLimit(int Axis);//該軸的是否碰觸到正極限		
	virtual bool               GetIsORG(int Axis);//該軸是否在原點位置
	virtual bool               GetIsEmergencyOn(int Axis);//該軸是否收到急停訊號
	virtual bool               DoFaultAck(int Axis);
	virtual const TCHAR*       GetAxisStatus(int Axis);	
	virtual bool               ResetMotionDriver();//重新復歸運動的Driver
	virtual CString            LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);//載入多國語系
			
	virtual int                GetIOStatus(int Axis, LPTSTR Str);//回傳I/O狀態, -1取資料出現異常, 要不則是狀態編碼, if pString==NULL, 不取錯誤文字	

	virtual bool               FireSingleTrigger();//送出單一觸發訊號(包含燈源與相機)

	virtual bool               EnableSoftwareLimit(int Axis);//啟用軟體極限
	virtual bool               DisableSoftwareLimit(int Axis);//關閉軟體極限
	virtual bool               SetSoftwareLimit(int Axis, double Min, double Max, bool Auto);//設定軟體極限	
	//----------------------------------------------------------------------------------//
	virtual bool               CorrectGantryOffset(int Axis);//修正龍門偏差
	virtual bool               CalibrateGantryOffset(int Axis);//校正龍門偏差
	virtual bool               FetchGantryOffset(int Axis, double &Offset);//取得龍門的偏移值	
	//----------------------------------------------------------------------------------//
	bool                       SetORG_Public(int Axis);
	//----------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
extern CMotion_PCIE_L221_B1D0  Motion_PCIE_L221_B1D0;
//-------------------------------------------------------------------------------------//
#endif//MOTION_DERIVE_MODE
#endif // !defined(AFX_MOTION_PCIE_L221_B1D0_H__AB2427D8_F528_453A_A3C1_F7B371E136D8__INCLUDED_)
