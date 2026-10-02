// Motion_PCIE_L221_B1D0.cpp: implementation of the CMotion_PCIE_L221_B1D0 class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Motion_PCIE_L221_B1D0.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#if MOTION_DERIVE_MODE == MOTION_PCIE_L221_B1D0
//-------------------------------------------------------------------------------------//
CMotion_PCIE_L221_B1D0  Motion_PCIE_L221_B1D0;
//-------------------------------------------------------------------------------------//
#define MAX_MOTION_AXIS        4//運動控制卡支援運動軸數
#define MAX_USED_AXIS          3//系統使用的運動軸數
//-------------------------------------------------------------------------------------//
#define HOME_START_VELOCITY    1000
#define HOME_MAX_VELOCITY      50000
#define HOME_ACCELERATE_TIME   0.1
#define HOME_PRE_MOVE_DIST     100000
//-------------------------------------------------------------------------------------//
#define ECAT_MOTION_MOVE_MODE_NONE     0////無運動模式
#define ECAT_MOTION_MOVE_MODE_PP       1//位制運動模式
#define ECAT_MOTION_MOVE_MODE_VL       2//速度控制模式
#define ECAT_MOTION_MOVE_MODE_PV       3//速度運動模式
#define ECAT_MOTION_MOVE_MODE_PT       4//扭力運動模式
#define ECAT_MOTION_MOVE_MODE_HOME     6//原點復歸模式
#define ECAT_MOTION_MOVE_MODE_INT_P    7//位置插值模式
#define ECAT_MOTION_MOVE_MODE_CSP      8//同步位置模式
#define ECAT_MOTION_MOVE_MODE_CSV      9//同步速度模式
#define ECAT_MOTION_MOVE_MODE_CST     10//同步扭力模式
//-------------------------------------------------------------------------------------//
#define PCIE_L221_B1D0_MDONE_STOP     0//Stop
#define PCIE_L221_B1D0_MDONE_ACCE     1//In Acceleration
#define PCIE_L221_B1D0_MDONE_REACH    2//In Reach
#define PCIE_L221_B1D0_MDONE_DECE     3//In Deceleration
#define PCIE_L221_B1D0_MDONE_MAILBOX  4//Mail Box Comm.	
//-------------------------------------------------------------------------------------//
#define PCIE_L221_B1D0_STATUS_WORD_RDYS		0x0001//驅動器允許操作狀態		-0
#define PCIE_L221_B1D0_STATUS_WORD_RDY		0x0002//驅動器允許操作			-1
#define PCIE_L221_B1D0_STATUS_WORD_SVON		0x0004//馬達激磁完成				-2
#define PCIE_L221_B1D0_STATUS_WORD_S_ERR	0x0008//伺服發生錯誤，馬達停止激磁	-3 Servo
#define PCIE_L221_B1D0_STATUS_WORD_SPO		0x0010//驅動器上電狀態SERVO_POWOR_ON-4
#define PCIE_L221_B1D0_STATUS_WORD_EMG		0x0020//立即停止					-5
#define PCIE_L221_B1D0_STATUS_WORD_ALM		0x0040//驅動器不允許操做			-6
#define PCIE_L221_B1D0_STATUS_WORD_S_WARN	0x0080//伺服發生警告，馬達仍為激磁狀態	-7
#define PCIE_L221_B1D0_STATUS_WORD_KEEP1	0x0100//保留						-8
#define PCIE_L221_B1D0_STATUS_WORD_CON		0x0200//連線狀態					-9
#define PCIE_L221_B1D0_STATUS_WORD_INP		0x0400//到位訊號					-10
#define PCIE_L221_B1D0_STATUS_WORD_LIM		0x0800//驅動器軟體極限，不支援		-11
#define PCIE_L221_B1D0_STATUS_WORD_OPM1		0x1000//操作模式					-12
#define PCIE_L221_B1D0_STATUS_WORD_OPM2		0x2000//操作模式					-13
#define PCIE_L221_B1D0_STATUS_WORD_KEEP2	0x4000//保留					-14
#define PCIE_L221_B1D0_STATUS_WORD_KEEP3	0x8000//保留					-15
//-------------------------------------------------------------------------------------//
//Col-1
#define PCIE_L221_B1D0_PDO_NEL_LIMIT        0x0001//負極限
#define PCIE_L221_B1D0_PDO_PEL_LIMIT        0x0002//正極限
#define PCIE_L221_B1D0_PDO_ORG   	        0x0004//原點
#define PCIE_L221_B1D0_PDO_ORG_GANTRY       0x0008//龍門原點
//Col-3
#define PCIE_L221_B1D0_PDO_INP              0x0001//定位
#define PCIE_L221_B1D0_PDO_ALARM            0x0020//警報
#define PCIE_L221_B1D0_PDO_EMG              0x0080//急停
//-------------------------------------------------------------------------------------//
//歸零移動模式
#define PCIE_L221_B1D0_HOME_MOVE_SLAVE           1
#define PCIE_L221_B1D0_HOME_MOVE_CSP             2
#define PCIE_L221_B1D0_HOME_MOVE_TOUCH_PRO       3
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CMotion_PCIE_L221_B1D0, CMotion_Basic)
//-------------------------------------------------------------------------------------//
CMotion_PCIE_L221_B1D0::CMotion_PCIE_L221_B1D0()
{
	CMotion_PCIE_L221_B1D0::PreInitMotion();
	SetInitialize(false);
	m_MotionStatus = _T("");
	m_ErrorString = _T("");		
	SetIsSupportGantry(true);
#ifdef OFFLINE_VERSION
	BuildMotionAxisList();
#endif//OFFLINE_VERSION
}
//-------------------------------------------------------------------------------------//
CMotion_PCIE_L221_B1D0::~CMotion_PCIE_L221_B1D0()
{
	m_MotionCallbackHWnd = NULL;	
	m_IsMotionRelease = true;	
	if ( this->ReleaseMotion() == false )
	{
		JetAPI::ShowMessageBox(this->m_ErrorString);
	}
}
//-------------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::OpenMotionCard()
{
	this->ReleaseMotionCard();
#ifndef MOTION_OBJ_DISABLE
	int i=0;
	U16 TotalCard=0;	
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	m_ECatNodeList.clear();
	ErrorStatus = _ECAT_Master_Open(&TotalCard);
	if ( CheckReturnOK(ErrorStatus) == false )
	{		
		m_ErrorString.Format(_T("Error, _ECAT_Master_Open(&TotalCard)"));
		return false;
	}

	if ( TotalCard == 0 )
	{
		ErrorStatus = _ECAT_Master_Close();
		m_ErrorString.Format(_T("Error, No PCIE_L221_B1D0 Inside IPC"));
		return false; 
	}

	//_ECAT_Master_Initial
	U16 CardNo=0;
	const int MaxCardNo=32;
	for ( i=0; i<MaxCardNo; i++ )
	{
		ErrorStatus = _ECAT_Master_Get_CardSeq(i, &CardNo);
		if ( ERR_ECAT_NO_ERROR != ErrorStatus ) { break; }		
		if ( OpenMotionCard(CardNo) == false )
		{
			std::vector<U16> CardNoList;
			BuildCardNoList(CardNoList);
			const size_t CardNoCount=CardNoList.size();
			for ( i=0; i<CardNoCount; i++ )
			{	_ECAT_Master_Reset(CardNoList[i]);	}
			_ECAT_Master_Close();
			return false;
		}			
	}			
#endif//MOTION_OBJ_DISABLE	
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::OpenMotionCard(U16 CardNo)
{
#ifndef MOTION_OBJ_DISABLE		
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	U16 EnableAlias=1;
	if ( FN_ENABLE == GetMotionParameter().m_UsingEcatNodeAliasName )//是否啟用節點別名模式 //20231205
	{	EnableAlias = 1;	}
	else
	{	EnableAlias = 0;	}
	//_ECAT_Master_NodeID_Alias_Enable(U16 CardNo, U16 Enable);//設定是否使用自定義站號的功能
	ErrorStatus = _ECAT_Master_NodeID_Alias_Enable(CardNo, EnableAlias);
	CheckReturnOK(ErrorStatus);

	//_ECAT_Master_Set_DCSyncShiftTime(U16 CardNo, U16 ShiftTime_Percent); //設定DC SyncShift時間間隔分比(需在Initial前設定)//Kent.chiang 20180706
	ErrorStatus = _ECAT_Master_Initial(CardNo);
	if ( CheckReturnOK(ErrorStatus) == false )
	{	return false;	}		
	
	U16 InitialDone=0;
	DWORD CheckInitCount=0;
	const DWORD MaxCheckInitCount=500;
	while ( true )
	{
		ErrorStatus = _ECAT_Master_Check_Initial_Done(CardNo, &InitialDone);		
		if ( 0 == InitialDone )
		{	break;	}
		CheckInitCount ++;
		if ( CheckInitCount>MaxCheckInitCount || 99==InitialDone )
		{	
			U16 Error = _ECAT_Master_Get_Initial_ErrorCode(CardNo);						
			m_ErrorString.Format(_T("Error, _ECAT_Master_Check_Initial_Done(CardNO)"));
			return false;
		}
		::Sleep(10);
	};

	U16 SlaveNum=0;
	ErrorStatus = _ECAT_Master_Get_SlaveNum(CardNo, &SlaveNum);
	if ( CheckReturnOK(ErrorStatus) == false )
	{	
		m_ErrorString.Format(_T("Error, _ECAT_Master_Get_SlaveNum(CardNo, &SlaveNum)"));
		return false;
	}
	if ( 0 == SlaveNum )
	{	return true;	}
	
	//_ECAT_Master_Set_CycleTime(U16 CardNo, U16 Mode);													//設定循環週期時間(U16 CardNo, 需在Initial前設定)
	//_ECAT_Master_Get_CycleTime(U16 CardNo, U16 *CycleTime);											//讀取當前設定的循環週期時間	
	//_ECAT_Master_Get_DLL_SeqID(U16 CardNo, U16 *SeqID);												//取得當前DLL使用的序列ID
	//_ECAT_Master_Get_SerialNo(U16 CardNo, U32 *SerialNo);												//取得PAC的Serial No
	//_ECAT_Master_NodeID_Alias_Enable(U16 CardNo, U16 Enable);											//設定是否使用自定義站號的功能
	
	U16 SeqID=0, NodeID=0;
	U32 VendorID=0, ProductCode=0, RevisionNo=0, SlaveDCTime=0;	
	for ( U16 i=0; i<SlaveNum; i++ )
	{
		SeqID = (U16)(i);
		//(U16 CardNo, U16 SeqID, U16 *NodeID, U32 *VendorID, U32 *ProductCode, U32 *RevisionNo, U32 *DCTime);	//讀取各模組資訊
		ErrorStatus = _ECAT_Master_Get_Slave_Info(CardNo, SeqID, &NodeID, &VendorID, &ProductCode, &RevisionNo, &SlaveDCTime);
		if ( CheckReturnOK(ErrorStatus) == false )
		{	
			m_ErrorString.Format(_T("Error, _ECAT_Master_Get_Slave_Info(CardNo, i, ReMapNodeID, VendorID, ProductCode, RevisionNo, SlaveDCTime)"));
			return false;
		}		
		ECAT_DRIVER_MODEL DriverModel=ECAT_DRIVER_NONE;
		CMotion_ECAT_Node EtherCatNode(CardNo, NodeID);		
		if ( VendorID == VendorID )
		{
			switch ( ProductCode )
			{
			case 0x613C0007://750W
			case 0x613C0006://400W
			case 0x613C0004://100W
				DriverModel=ECAT_DRIVER_PANASONIC;
				break;			
			}
		}
		if ( ECAT_DRIVER_NONE == DriverModel )
		{	DriverModel = ECAT_DRIVER_YASKAWA;	}
		EtherCatNode.SetDriverModel(DriverModel);
		if ( AddETherCatNode(EtherCatNode) == false )		
		{	return false;	}
	}		
#endif//MOTION_OBJ_DISABLE	
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::InitialMotionCard()
{
	this->ReleaseMotionCard();
	if ( OpenMotionCard() == false )
	{	return false; }	
	if ( SaveMotionCardType() == false )
	{	return false; }
	if ( InitializeForMachine() == false )
	{	return false; }	
	SetInitialize(true);
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::InitializeForMachine()//特定機台初始化	
{
	bool IsOK = true;
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	MACHINE_MODEL_TYPE MachineType = SystemParam.m_MachineModelType;	
	BuildMotionAxisList();
	SetMotionMachineType(MachineType);			
	IsOK = InitializeForJET8000();
	return IsOK;		
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::InitializeForJET8000()//JET8000的初始化
{	
#ifndef MOTION_OBJ_DISABLE
	size_t i=0;	
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;	
	std::vector<CMotionAxis> &MotionAxisList=GetMotionAxisList();
	const size_t MotionAxisCount=MotionAxisList.size();

	for( i=0;i<MotionAxisCount;i++)
	{
		CMotionAxis &MotionAxisRef=MotionAxisList[i];
		if ( CheckAxisBypass(&MotionAxisRef) == true ) { continue; }

		const U16 Axis = MotionAxisRef.GetAxisID();
		const U16 CardNo = MotionAxisRef.GetCardID();
		const U16 NodeID = MotionAxisRef.GetNodeID();
		const U16 SlotID = MotionAxisRef.GetSlotID();
		const bool GantryAxis = MotionAxisRef.CheckGantryAxis();		
		const size_t SlaveNodeCount=MotionAxisRef.GetSlaveNodeCount();
		ECAT_DRIVER_MODEL DriverModel = MotionAxisRef.GetDriverModel();		

		//_ECAT_Slave_Motion_Set_MoveMode(U16 CardNo, U16 NodeID, U16 SlotNo, U16 OpMode);//設定當前MotionSlave的動作模式 0:空模式 1:PP 3:PV 4:PT 6:Home 8:CSP
		ErrorStatus = _ECAT_Slave_Motion_Set_MoveMode(CardNo, NodeID, SlotID, ECAT_MOTION_MOVE_MODE_CSP);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _ECAT_Slave_Motion_Set_MoveMode(CardNo, CardAxis, SlotID, ECAT_MOTION_MOVE_MODE_CSP)"));
			return false;
		}

		//_ECAT_Slave_Motion_Set_Alm_Reaction(U16 CardNo, U16 NodeID, U16 SlotNo, U16 Fault_Type, U16 WR_Type);	//設定Alm時的反應機制 0:不理會, 1:上緣觸發, 2:永久停止
		ErrorStatus = _ECAT_Slave_Motion_Set_Alm_Reaction(CardNo, NodeID, SlotID, 2, 2);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _ECAT_Slave_Motion_Set_Alm_Reaction(CardNo, CardAxis, SlotID, 2, 1)"));
			return false;
		}

		//_ECAT_Slave_Motion_Set_Internal_Limit_Active_Reaction(U16 CardNo, U16 NodeID, U16 SlotID, U16 Internal_Limit_Active_Type);//設定ila時的反應機制 0:不理會, 1:上緣觸發, 2:永久停止
		ErrorStatus = _ECAT_Slave_Motion_Set_Internal_Limit_Active_Reaction(CardNo, NodeID, SlotID, 1);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _ECAT_Slave_Motion_Set_Internal_Limit_Active_Reaction(CardNo, CardAxis, SlotID, 2, 1)"));
			return false;
		}

		//_ECAT_Slave_Motion_Set_Command_Wait_Target_Reach(U16 CardNo, U16 NodeID, U16 SlotNo, U16 Wait);  // 2016-10-27
		ErrorStatus = _ECAT_Slave_Motion_Set_Command_Wait_Target_Reach(CardNo, NodeID, SlotID, 1);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _ECAT_Slave_Motion_Set_Command_Wait_Target_Reach(CardNo, CardAxis, SlotID, 2, 1)"));
			return false;
		}

		//set pulse command output mode		
		//_m114_set_pls_outmode(U16 SwitchCardNo, U16 AxisNo, I16 pls_outmode);//pls_outmode:6,7(A/B Phase)		

		//set encoder input mode

		//set counter input source //注意一定要這樣設定				
				
		//set abs position reference//注意一定要這樣設定		
		
		//Set Home Config					
		I16 home_mode = 0;//0, or 6		

		//set alarm logic = Low						

		//set end limit logic 				

		//set servo on logic = low for J2S 				

		//set in-position logic

		//set move ratio (command/feedback)//No this Function
		//ErrorStatus = _M114GL_set_move_ratio(m_CardNo,CardAxis, 1); 
		//if ( CheckReturnOK(ErrorStatus) == false )
		//{
		//	m_ErrorString.Format(_T("Error, _M114GL_set_move_ratio(m_CardNo,AXIS, 1)"));
		//	return false;
		//}

		//turn off ERC signal
	
		//set ERC turn on mode (0~6=>time, 7=level output)		

		//set SD(slow down) configuration
		//I16 status= _m114_set_sd(U16 SwitchCardNo, U16 AxisNo, I16 enable,I16 sd_logic, I16 sd_latch, I16 sd_mode)		

		//set INT(interrupt) mode//No this Function
		//ErrorStatus = _M114GL_set_int_factor(m_CardNo,CardAxis, 0x0000);//normal stop
		//if ( CheckReturnOK(ErrorStatus) == false )
		//{
		//	m_ErrorString.Format(_T("Error, _M114GL_set_int_factor(m_CardNo,AXIS, 0)"));
		//	return false;
		//}
		//disable software limit		

		
		const U16 Window_Time = 3;
		const U32 Position_Window = 10;
		const U16 Enable = true;
		if ( CheckSupportInPosition(DriverModel) == false )		
		{
			ErrorStatus = _ECAT_Slave_CSP_Set_SoftTargetReach(CardNo, NodeID, SlotID, Window_Time, Position_Window, Enable); //Window_Time:ms, Position_Window:pulse
			if ( CheckReturnOK(ErrorStatus) == false )
			{
				m_ErrorString.Format(_T("Error, _ECAT_Slave_CSP_Set_SoftTargetReach(CardNo, NodeID, SlotID, Window_Time, Position_Window, Enable)"));
				return false;
			}

		}
	}	
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::Stop(U16 Axis)
{	
#ifndef MOTION_OBJ_DISABLE	
	const double Tdec = 0.15f;
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	if ( CheckIsEnabled(MotionAxisPtr) == false ) { return true; }

	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	
	CMotion_Basic::SaveMotionProcess(Axis, _T("Stop"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Stop Start"));	
	ErrorStatus = _ECAT_Slave_Motion_Sd_Stop(CardNo,NodeID,SlotID,Tdec);	
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		m_ErrorString.Format(_T("Error, _ECAT_Slave_Motion_Sd_Stop"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Stop NG End"));
		return false;
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Stop OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
int  CMotion_PCIE_L221_B1D0::GetIOStatus(int Axis, LPTSTR Str)//回傳錯誤狀態
{		
	U16 io_sts = 0;	
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return -1; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIOStatus Start"));	
	if ( GetAxisStatusWord(Axis, io_sts) == false )
	{	
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIOStatus NG-1 End"));
		return -1;
	}	
	
	if ( NULL != Str )
	{
		int IoID = 0;
		CString Status;
		CString stringbuff;			
		std::vector<int> IDList;
		GetAxisStatusIDList(IDList);			
		const size_t IDCount=IDList.size();
		for ( size_t i=0; i<IDCount; i++ )
		{
			IoID = IDList[i];
			if ( (io_sts&IoID)!= 0x00 )
			{	
				stringbuff = GetAxisStatusText(IoID);
				if ( Status.GetLength() == 0 )
				{	Status = stringbuff; }
				else
				{	Status = Status + _T(", ") + stringbuff; }		
			}
		}
		_tcscpy(Str, Status);	
	}
#endif//MOTION_OBJ_DISABLE
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIOStatus OK-2 End"));
	return io_sts;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::FreeRun(U16 Axis, F64 StrVel, F64 MaxVel, F64 Tacc)
{	
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }	
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	if ( CheckIsEnabled(MotionAxisPtr) == false ) { return false; }
	if ( CheckFreeRunMoving(MotionAxisPtr) == true ) { return true; }		

	U16 Dir = 0;
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	
	CMotion_Basic::SaveMotionProcess(Axis, _T("FreeRun"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::FreeRun Start"));	
	if ( MaxVel > 0 ) { Dir = 0; }
	else { Dir = 1; }
	MaxVel = abs(MaxVel);
	//_ECAT_Slave_CSP_Start_V_Move(U16 CardNo, U16 NodeID, U16 SlotNo, U16 Dir, I32 StrVel, I32 ConstVel, F64 Tacc, U16 SCurve);//連續等速運動
	ErrorStatus = _ECAT_Slave_CSP_Start_V_Move(CardNo, NodeID, SlotID, Dir, (I32)StrVel, (I32)(MaxVel), Tacc, 0);	
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("_ECAT_Slave_CSP_Start_V_Move(axis, StrVel, MaxVel, Tacc"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::FreeRun NG End"));
		return false; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::FreeRun OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
int CMotion_PCIE_L221_B1D0::CheckSupportHomeMoveMode(CMotionAxis *MotionAxisPtr)
{
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	ECAT_DRIVER_MODEL DriverModel=MotionAxisPtr->GetDriverModel();
	int HomeMoveMode=PCIE_L221_B1D0_HOME_MOVE_SLAVE;
	if ( MotionAxisPtr->CheckGantryAxis() == true )
	{	HomeMoveMode=PCIE_L221_B1D0_HOME_MOVE_TOUCH_PRO;	}
	else
	{
		switch ( DriverModel )
		{
		case ECAT_DRIVER_PANASONIC:
			HomeMoveMode=PCIE_L221_B1D0_HOME_MOVE_SLAVE;
			//HomeMoveMode=PCIE_L221_B1D0_HOME_MOVE_TOUCH_PRO;
			break;
		default:
			HomeMoveMode=PCIE_L221_B1D0_HOME_MOVE_CSP;
			break;
		}
	}
	return HomeMoveMode;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::CheckSupportInPosition(ECAT_DRIVER_MODEL DriverModel) const
{
	if ( ECAT_DRIVER_PANASONIC == DriverModel ) { return false; }
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::WriteAxisParam_INP(const CMotionNode &MotionNodeRef, bool bINP)
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }		
	
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 Enable = bINP;
	const U16 Window_Time = 3;
	const U32 Position_Window = 10;
	const U16 CardNo = MotionNodeRef.GetCardID();
	const U16 NodeID = MotionNodeRef.GetNodeID();
	const U16 SlotID = MotionNodeRef.GetSlotID();
	ECAT_DRIVER_MODEL DriverModel = MotionNodeRef.GetDriverModel();
	if ( CheckSupportInPosition(DriverModel) == true )
	{	
		//_ECAT_Slave_CSP_Set_SoftTargetReach(U16 CardNo, U16 NodeID, U16 SlotID, U16 Window_Time, U32 Position_Window, U16 Enable); //Window_Time:ms, Position_Window:pulse
		ErrorStatus = _ECAT_Slave_CSP_Set_SoftTargetReach(CardNo, NodeID, SlotID, Window_Time, Position_Window, Enable); //Window_Time:ms, Position_Window:pulse
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			CString Err;
			Err.Format(_T("Error, _ECAT_Slave_CSP_Set_SoftTargetReach(CardNo, NodeID, SlotID, Window_Time, Position_Window, Enable)"));
			SetErrorString(Err);
			return false;
		}		
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::GetAxisStatusWord(int Axis, U16 &Status)
{
#ifndef MOTION_OBJ_DISABLE
	Status = 0;
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetAxisStatusWord Start"));

	ErrorStatus = _ECAT_Slave_Motion_Get_StatusWord(CardNo, NodeID, SlotID, &Status);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CString Err;
		Err.Format(_T("Error, _ECAT_Slave_Motion_Get_StatusWord(CardNo, CardAxis, SlotID, &Status)"));
		SetErrorString(Err);
		return false;
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetAxisStatusWord OK End"));
	return true;
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ReadNodeStatusWord(CMotionNode *MotionNodePtr, U16 &Status)
{
#ifndef MOTION_OBJ_DISABLE
	Status = 0;
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionNodePtr(MotionNodePtr) == false ) { return false; }
	
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionNodePtr->GetCardID();
	const U16 NodeID = MotionNodePtr->GetNodeID();
	const U16 SlotID = MotionNodePtr->GetSlotID();
	ErrorStatus = _ECAT_Slave_Motion_Get_StatusWord(CardNo, NodeID, SlotID, &Status);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CString Err;
		Err.Format(_T("Error, _ECAT_Slave_Motion_Get_StatusWord(CardNo, CardAxis, SlotID, &Status)"));
		SetErrorString(Err);
		return false;
	}	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ReadAxisStatusWord_Ready(CMotionAxis *MotionAxisPtr, bool &bReady)
{
	bReady = false;	
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	
	if ( ReadNodeStatusWord_Ready(MotionAxisPtr->GetMotionNodePtr(), bReady) == false )
	{	return false;	}
	if ( false == bReady )
	{	return true;	}
	
	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	for ( int i=0; i<SlaveNodeCount; i++ )
	{
		CMotionNode *SlaveNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == SlaveNodePtr ) { continue; }
		if ( ReadNodeStatusWord_Ready(SlaveNodePtr, bReady) == false )
		{	return false;	}
		if ( false == bReady )
		{	return true;	}
	}
	bReady = true;
#endif//MOTION_OBJ_DISABLE
	return bReady;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ReadNodeStatusWord_Ready(CMotionNode *MotionNodePtr, bool &bReady)
{
#ifndef MOTION_OBJ_DISABLE
	U16 Status = 0;
	if ( CheckInit() == false ) { return false; }		
	if ( ReadNodeStatusWord(MotionNodePtr, Status) == false ) { return false; }
	if ( (Status & PCIE_L221_B1D0_STATUS_WORD_RDYS) != 0x00 ) 
	{	bReady = true;	}
	else
	{	bReady = false; }
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ReadAxisStatusWord_Enable(CMotionAxis *MotionAxisPtr, bool &bEnable)
{
	bEnable = false;	
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }

	if ( ReadNodeStatusWord_Enable(MotionAxisPtr->GetMotionNodePtr(), bEnable) == false )
	{	return false;	}
	if ( false == bEnable )
	{	return true;	}
	
	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	for ( int i=0; i<SlaveNodeCount; i++ )
	{
		CMotionNode *SlaveNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == SlaveNodePtr ) { continue; }
		if ( ReadNodeStatusWord_Enable(SlaveNodePtr, bEnable) == false )
		{	return false;	}
		if ( false == bEnable )
		{	return true;	}
	}
	bEnable = true;
#endif//MOTION_OBJ_DISABLE
	return bEnable;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ReadNodeStatusWord_Enable(CMotionNode *MotionNodePtr, bool &bEnable)
{
#ifndef MOTION_OBJ_DISABLE
	U16 Status = 0;
	if ( CheckInit() == false ) { return false; }		
	if ( ReadNodeStatusWord(MotionNodePtr, Status) == false ) { return false; }
	if ( (Status & PCIE_L221_B1D0_STATUS_WORD_SVON) != 0x00 ) 
	{	bEnable = true;	}
	else
	{	bEnable = false; }
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ReadAxisStatusWord_Alarm(CMotionAxis *MotionAxisPtr, bool &bAlarm)
{
	bAlarm = false;
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	
	if ( ReadNodeStatusWord_Alarm(MotionAxisPtr->GetMotionNodePtr(), bAlarm) == false )
	{	return false;	}
	if ( true == bAlarm )
	{	return true;	}
	
	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	for ( int i=0; i<SlaveNodeCount; i++ )
	{
		CMotionNode *SlaveNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == SlaveNodePtr ) { continue; }
		if ( ReadNodeStatusWord_Alarm(SlaveNodePtr, bAlarm) == false )
		{	return false;	}
		if ( true == bAlarm )
		{	return true;	}
	}
	bAlarm = false;
#endif//MOTION_OBJ_DISABLE
	return bAlarm;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ReadNodeStatusWord_Alarm(CMotionNode *MotionNodePtr, bool &bAlarm)
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }		

	U8 Data[4];
	const U16 IOType = 0x00;
	const U16 ODIndex = 0x60FD;
	const U16 ODSubIndex = 0x0;
	const U16 ByteSize = sizeof(Data);

	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionNodePtr->GetCardID();
	const U16 NodeID = MotionNodePtr->GetNodeID();
	const U16 SlotID = MotionNodePtr->GetSlotID();	

	::memset(Data, 0x00, sizeof(Data));
	//_ECAT_Slave_PDO_Get_OD_Data(U16 CardNo, U16 NodeID, U16 SlotNo, U16 IOType, U16 ODIndex, U16 ODSubIndex, U16 ByteSize, U8 *Data);	//"Slave通用指令，對該站讀取某一OD碼的資料，該資料需有映射於PDO構成中, IOType 0:Rx 1:Tx"
	ErrorStatus = _ECAT_Slave_PDO_Get_OD_Data(CardNo, NodeID, SlotID, IOType, ODIndex, ODSubIndex, ByteSize, Data);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		m_ErrorString.Format(_T("Error, _ECAT_Slave_PDO_Get_OD_Data(CardNo, NodeID, SlotID, IOType, ODIndex, ODSubIndex, ByteSize, Data)"));		
		return false;
	}

	const int AxisStatus = Data[3];	
	if ( (AxisStatus&PCIE_L221_B1D0_PDO_ALARM)!= 0x00 ) 
	{	bAlarm = true;	}
	else
	{	bAlarm = false; }
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ReadAxisStatusWord_INP(CMotionAxis *MotionAxisPtr, bool &bINP)
{
	bINP = false;	
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }

	if ( ReadNodeStatusWord_INP(MotionAxisPtr->GetMotionNodePtr(), bINP) == false )
	{	return false;	}
	if ( false == bINP )
	{	return true;	}
	
	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	for ( int i=0; i<SlaveNodeCount; i++ )
	{
		CMotionNode *SlaveNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == SlaveNodePtr ) { continue; }
		if ( ReadNodeStatusWord_INP(SlaveNodePtr, bINP) == false )
		{	return false;	}
		if ( false == bINP )
		{	return true;	}
	}
	bINP = true;
#endif//MOTION_OBJ_DISABLE
	return bINP;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ReadNodeStatusWord_INP(CMotionNode *MotionNodePtr, bool &bINP)//In Position
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }		
	
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionNodePtr->GetCardID();
	const U16 NodeID = MotionNodePtr->GetNodeID();
	const U16 SlotID = MotionNodePtr->GetSlotID();
	const bool UseInputIO = MotionNodePtr->CheckUseInputIoBit_INP();
	if ( false == UseInputIO )
	{
		U8 Data[4];
		const U16 IOType = 0x00;
		const U16 ODIndex = 0x60FD;
		const U16 ODSubIndex = 0x0;
		const U16 ByteSize = sizeof(Data);

		::memset(Data, 0x00, sizeof(Data));
		//_ECAT_Slave_PDO_Get_OD_Data(U16 CardNo, U16 NodeID, U16 SlotNo, U16 IOType, U16 ODIndex, U16 ODSubIndex, U16 ByteSize, U8 *Data);	//"Slave通用指令，對該站讀取某一OD碼的資料，該資料需有映射於PDO構成中, IOType 0:Rx 1:Tx"
		ErrorStatus = _ECAT_Slave_PDO_Get_OD_Data(CardNo, NodeID, SlotID, IOType, ODIndex, ODSubIndex, ByteSize, Data);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _ECAT_Slave_PDO_Get_OD_Data(CardNo, NodeID, SlotID, IOType, ODIndex, ODSubIndex, ByteSize, Data)"));		
			return false;
		}

		const int AxisStatus = Data[3];	
		if ( (AxisStatus&PCIE_L221_B1D0_PDO_INP)!= 0x00 ) 
		{	bINP = true;	}
		else
		{	bINP = false; }
	}
	else
	{
		U16 OnOff=0;	
		ErrorStatus = _ECAT_GPIO_Get_Input(CardNo, &OnOff);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			CString Err;
			Err.Format(_T("Error, _ECAT_GPIO_Get_Input(CardNo, &OnOff)"));
			SetErrorString(Err);
			return false;
		}	

		U16 InpBit = static_cast<U16>(MotionNodePtr->GetInputIoBit_INP());
		/*
		switch (NodeID)
		{
		case 1:	InpBit = 2; break;
		case 2:	InpBit = 4; break;
		case 3:	InpBit = 8; break;
		case 4:	InpBit = 16; break;
		default:InpBit = 0;  break;
		}
		*/
		if ( (OnOff & InpBit) != 0x00 ) 
		{	bINP = true;	}
		else
		{	bINP = false; }
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::WaitForNodeDone(CMotionNode *MotionNodePtr)
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckMotionNodePtr(MotionNodePtr) == false  ) { return false; }		
	CString AxisS = MotionNodePtr->GetAxisName();

	CString   str;
	CString   Err;	
	int       IoID = 0;
	bool      bINP=false;
	U16       Mdone = 0;
	U16       io_sts = 0;
	DWORD     TickCnt1=0;
	DWORD     TickCnt2=0;
	DWORD     TickCntD=0;
	//const int SeelpTime = 1;//10ms, 不能太短
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const DWORD MaxTickCount=20000;//20 sec	
	const U16 CardNo = MotionNodePtr->GetCardID();
	const U16 NodeID = MotionNodePtr->GetNodeID();
	const U16 SlotID = MotionNodePtr->GetSlotID();	
	const bool bWaitForInp = MotionNodePtr->GetIsWaitForInp();
	const int SeelpTime = GetMotionParameter().m_WaitForDoneDwellTime;	

	TickCnt1 = ::GetTickCount();	
	while ( true )
	{	
		//_ECAT_Slave_Motion_Get_Mdone(U16 CardNo, U16 NodeID, U16 SlotNo, U16 *Mdone);//0:Strop, 1:Moving
		ErrorStatus = _ECAT_Slave_Motion_Get_Mdone(CardNo, NodeID, SlotID, &Mdone);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			str = _T("Error, wait for done too long");
			str = LoadMultiLanguageString(str, str);
			Err.Format(_T("%s [%s, time:%d ms]"), str, AxisS, TickCntD);
			SetErrorString(Err);
			return false;
		}
		if ( PCIE_L221_B1D0_MDONE_STOP == Mdone )
		{
			if ( false == bWaitForInp )
			{	bINP = true;	}
			else
			{
				if ( ReadNodeStatusWord_INP(MotionNodePtr, bINP) == false )
				{	return false; }
			}
			if ( true == bINP )
			{	break;  }
		}

		ErrorStatus = _ECAT_Slave_Motion_Get_StatusWord(CardNo, NodeID, SlotID, &io_sts);
		if ( CheckReturnOK(ErrorStatus) == true )
		{
			//if ( (io_sts&PCIE_L221_B1D0_STATUS_WORD_EMG) != 0x00 )
			//{	break;	}
			if ( (io_sts&PCIE_L221_B1D0_STATUS_WORD_ALM) != 0x00 )
			{	break;	}
		}

		TickCnt2 = ::GetTickCount();
		TickCntD = TickCnt2-TickCnt1;
		if ( TickCntD > MaxTickCount )
		{
			str = _T("Error, wait for done too long");
			str = LoadMultiLanguageString(str, str);
			Err.Format(_T("%s [%s, time:%d ms]"), str, AxisS, TickCntD);
			SetErrorString(Err);
			return false; 
		}

		if ( SeelpTime > 0 )
		{	::Sleep(SeelpTime); }
	}	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::OneAxisMoveTo(U16 Axis, F64 Dist, F64 StrVel, F64 MaxVel,F64 Tacc,F64 Tdec, MOVE_CURVE_MODE VelCurve, MOVE_COORDINATE_MODE CoordMode, F64 SVacc, F64 SVdec)
{
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	if ( CheckIsEnabled(MotionAxisPtr) == false ) { return false; }
	if ( CheckIsHomed(MotionAxisPtr) == false )	{	return false; }

	CString str;
	int nSign = FN_ENABLE;
	bool bTriangleCorrection = false;
	double Acc = 3000000;//加速度mm/s/s	
	double MinTimeAcc = 0.14;//最慢的時間為0.05 sec
	double MinTimeDec = 0.14;//最慢的時間為0.05 sec	
	double PosTolerance = 1.0;
	ACC_TIME_ADJUST_MODE AccTimeAdjustMode=ACC_TIME_ADJUST_OFF;
	
	if ( CMotion_PCIE_L221_B1D0::WaitForDone(Axis) == false )
	{	return false; }

	str.Format(_T("OneAxisMoveTo[%.0f]"), Dist);
	CMotion_Basic::SaveMotionProcess(Axis, str, MSG_LEVEL_HIGH);
	//CMotion_Basic::SaveMotionProcess(Axis, _T("OneAxisMoveTo"), MSG_LEVEL_HIGH);

	nSign = MotionAxisPtr->GetSignPositive();
	Acc = MotionAxisPtr->GetAccelerationValue();
	MinTimeAcc = MotionAxisPtr->GetAccelerationTime();	
	MinTimeDec = MotionAxisPtr->GetDecelerationTime();
	PosTolerance = MotionAxisPtr->GetPosTolerance();
	AccTimeAdjustMode = MotionAxisPtr->GetAccTimeAdjustMode();
	bTriangleCorrection = MotionAxisPtr->GetTriangleCorrection();		
	MotionAxisPtr->SetCommandMaxVelocity(0);
	MotionAxisPtr->SetCommandAccelerationTime(0);
	const double LimitMin = MotionAxisPtr->GetLimitMin();
	const double LimitMax = MotionAxisPtr->GetLimitMax();
	const TMotionParameter &MotionParam=GetMotionParameter();	
	switch ( Axis )
	{
	default: str=_T(""); break;
	case AXIS_X: str.Format(_T("CMotion_PCIE_L221_B1D0::OneAxisMoveTo(X, Pos:%.0f, Speed:%.0f, Tacc:%.4f, Tdec:%.4f)"), Dist, MaxVel, Tacc, Tdec);	break;
	case AXIS_Y: str.Format(_T("CMotion_PCIE_L221_B1D0::OneAxisMoveTo(Y, Pos:%.0f, Speed:%.0f, Tacc:%.4f, Tdec:%.4f)"), Dist, MaxVel, Tacc, Tdec);	break;
	case AXIS_Z: str.Format(_T("CMotion_PCIE_L221_B1D0::OneAxisMoveTo(Z, Pos:%.0f, Speed:%.0f, Tacc:%.4f, Tdec:%.4f)"), Dist, MaxVel, Tacc, Tdec);	break;
	}
	if ( str.GetLength() > 0 )
	{	CMotion_Basic::SaveMotionCurrentProcess(str); }

	if ( Dist>LimitMax || Dist<LimitMin )
	{
		switch ( Axis )
		{
		default: str=_T("Axis"); break;
		case AXIS_X: str = _T("Error, X Axis Out of Stage Limit");	break;
		case AXIS_Y: str = _T("Error, Y Axis Out of Stage Limit");	break;
		case AXIS_Z: str = _T("Error, Z Axis Out of Stage Limit");	break;
		}		
		str = CMotion_Basic::LoadMultiLanguageString(str, str);
		this->m_ErrorString.Format(_T("%s (%.0f ~ %.0f, Pos=%.0f)"), str, LimitMin, LimitMax, Dist);
		return false;
	}	

	double Dis = Dist;
	double MaxVelocity = MaxVel;
	double Encode=0;
	GetEncode(Axis, Encode);
	if ( CoordMode == MOVE_COORDINATE_ABS)
	{	Dis = Dist - Encode;	}
	
	const bool bConvert=GetIsConvertSignPositive();//注意順序
	if ( true == bConvert )
	{
		if ( FN_DISABLE == nSign )
		{	Dist = -Dist;	}
	}

	if ( Dis < 0 ) { Dis = -Dis; }
	const double AdjustAccDist=MotionAxisPtr->GetAdjustAccclerationDist();
	Acc = AdjustAccValue(Acc, Dis, AdjustAccDist);

	//MaxVelocity = sqrt(Acc*Dis/2.0);//平行四邊形
	MaxVelocity = sqrt(Acc*Dis);//三角形
	//MaxVelocity = sqrt(2.0*Acc*Dis/3.0);//三角形
	//MaxVelocity = sqrt(2.0*Acc*Dis/3.0);//三角形
	if ( MaxVelocity > MaxVel ) { MaxVelocity = MaxVel; }
	Tacc = ::fabs(MaxVelocity/Acc);
	Tdec = ::fabs(MaxVelocity/Acc);
	Tacc = AdjustAccTime(Tacc, MinTimeAcc, AccTimeAdjustMode);
	Tdec = AdjustAccTime(Tdec, MinTimeDec, AccTimeAdjustMode);
	
	double NewAcc = Dis/(Tacc*Tacc);	
	if ( NewAcc > Acc ) { NewAcc = Acc; }
	MaxVelocity = Tacc*NewAcc;
	if ( MaxVelocity > MaxVel ) 
	{
		MaxVelocity = MaxVel;
		Acc = MaxVelocity/Tacc;
	}	
	if ( FN_DISABLE==MotionParam.m_AccelerationAdjust )
	{	
		Tacc = MinTimeAcc;
		Tdec = MinTimeDec;		
	}
	else
	{	MaxVel = MaxVelocity;	}
#ifndef MOTION_OBJ_DISABLE
	if ( Dis < PosTolerance ) { return true; }
	MotionAxisPtr->SetCommandMaxVelocity(MaxVel);
	MotionAxisPtr->SetCommandAccelerationTime(Tacc);

	//以下確認是避免算到負號時異常動作
	U32 uSVacc=0, uSVdec = 0;
	U32 uStrVel=0, uMaxVel=0;	
	if ( StrVel < 0 ) { uStrVel = (U32)(-StrVel); }
	else { uStrVel = (U32)(StrVel); }
	if ( MaxVel < 0 ) { uMaxVel = (U32)(-MaxVel); }
	else { uMaxVel = (U32)(MaxVel); }
	if ( SVacc < 0 ) { uSVacc = (U32)(-SVacc); }
	else { uSVacc = (U32)(SVacc); }
	if ( SVdec < 0 ) { uSVdec = (U32)(-SVdec); }
	else { uSVdec = (U32)(SVdec); }

	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::OneAxisMoveTo Start"));	
	if ( VelCurve ==  MOVE_CURVE_T )
	{	
		CString strInfo;
		strInfo.Format(_T("T-Curve Dist(%.2f), StrVel(%d), MaxVel(%d), Tacc(%.4f), Tdec(%.4f)"), Dist, uStrVel, uMaxVel, Tacc, Tdec);
		if ( FN_ENABLE == MotionParam.m_SaveMotionCardParam  )
		{	SaveMotionProcess(Axis, strInfo, MSG_LEVEL_HIGH); }
		if ( CoordMode == MOVE_COORDINATE_ABS )
		{				
			const U16 IsAbs = 1;
			const U16 SCurve= 1;
			//ErrorStatus = _ECAT_Slave_PP_Start_Move(CardNo, NodeID, SlotID, Dist, uMaxVel, Tacc, Tdec, IsAbs);
			ErrorStatus = _ECAT_Slave_CSP_Start_Move(CardNo, NodeID, SlotID, Dist, uStrVel, uMaxVel, uStrVel, Tacc, Tdec, SCurve, IsAbs);
			if ( CheckReturnOK(ErrorStatus) == false )
			{
				CString str = "";
				GetErrorCodeText(ErrorStatus, str);
				this->m_ErrorString.Format(_T("Error, _ECAT_Slave_CSP_Start_Move Fault.(%s)"), str);

				int axis = Axis;
				double dist = Dist;
				double startvel = StrVel;
				double maxvel = MaxVel;
				double acc = Tacc;
				double dec = Tdec;
				str.Format(_T("Axis: %d, Dist: %.2f,  StartVel: %.2f, MaxVel: %.2f, TAcc: %.2f, TDec: %.2f"), axis, dist, startvel, maxvel, acc, dec);

				CMotion_Basic::SaveMotionCurrentProcess(m_ErrorString);
				CMotion_Basic::SaveMotionCurrentProcess(str);
				CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::OneAxisMoveTo NG-1 End"));
				return false; 
			}
		}
		else if ( CoordMode == MOVE_COORDINATE_INS )
		{			
			const U16 IsAbs = 0;
			const U16 SCurve= 1;
			//ErrorStatus = _ECAT_Slave_PP_Start_Move(CardNo, NodeID, SlotID, Dist, uMaxVel, Tacc, Tdec, IsAbs);
			ErrorStatus = _ECAT_Slave_CSP_Start_Move(CardNo, NodeID, SlotID, Dist, uStrVel, uMaxVel, uStrVel, Tacc, Tdec, SCurve, IsAbs);
			if ( CheckReturnOK(ErrorStatus) == false )
			{
				this->m_ErrorString.Format(_T("Error, _ECAT_Slave_CSP_Start_Move Fault"));
				CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::OneAxisMoveTo NG-2 End"));
				return false; 
			}
		}
		else
		{
			this->m_ErrorString.Format(_T("Error, Coordinate Mode Exception (%d)"), CoordMode);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::OneAxisMoveTo NG-3 End"));
			return false; 
		}		
	}
	else if ( VelCurve == MOVE_CURVE_S )
	{
		uSVacc = (uMaxVel-uStrVel)/3;
		uSVdec = (uMaxVel-uStrVel)/3;
		const double DifVel = (uMaxVel-uStrVel);
		const double Ratio = MotionParam.m_SCurveVelRatio/100.0;
		uSVacc = (U32)(Ratio*0.5*DifVel);
		uSVdec = (U32)(Ratio*0.5*DifVel);
		//uSVacc = (U32)(Ratio*DifVel*Tdec/(Tacc+Tdec));
		//uSVdec = (U32)(Ratio*DifVel*Tacc/(Tacc+Tdec));		
		CString strInfo;
		strInfo.Format(_T("S-Curve Dist(%.2f), StrVel(%d), MaxVel(%d), Tacc(%.4f), Tdec(%.4f), SVacc(%d), SVdec(%d)"), Dist, uStrVel, uMaxVel, Tacc, Tdec, uSVacc, uSVdec);		
		if ( FN_ENABLE == MotionParam.m_SaveMotionCardParam  )
		{	SaveMotionProcess(Axis, strInfo, MSG_LEVEL_HIGH); }
		if ( CoordMode == MOVE_COORDINATE_ABS )
		{	
			const U16 IsAbs = 1;
			const U16 SCurve= 2;
			//ErrorStatus = _ECAT_Slave_PP_Start_Move(CardNo, NodeID, SlotID, Dist, uMaxVel, Tacc, Tdec, IsAbs);
			ErrorStatus = _ECAT_Slave_CSP_Start_Move(CardNo, NodeID, SlotID, Dist, uStrVel, uMaxVel, uStrVel, Tacc, Tdec, SCurve, IsAbs);
			if ( CheckReturnOK(ErrorStatus) == false )
			{
				this->m_ErrorString.Format(_T("Error, _ECAT_Slave_CSP_Start_Move Fault"));
				CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::OneAxisMoveTo NG-4 End"));
				return false; 
			}
		}
		else if ( CoordMode == MOVE_COORDINATE_INS )
		{
			const U16 IsAbs = 0;
			const U16 SCurve= 2;
			//ErrorStatus = _ECAT_Slave_PP_Start_Move(CardNo, NodeID, SlotID, Dist, uMaxVel, Tacc, Tdec, IsAbs);
			ErrorStatus = _ECAT_Slave_CSP_Start_Move(CardNo, NodeID, SlotID, Dist, uStrVel, uMaxVel, uStrVel, Tacc, Tdec, SCurve, IsAbs);
			if ( CheckReturnOK(ErrorStatus) == false )
			{
				this->m_ErrorString.Format(_T("Error, _ECAT_Slave_CSP_Start_Move Fault"));
				CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::OneAxisMoveTo NG-5 End"));
				return false; 
			}
		}
		else
		{
			this->m_ErrorString.Format(_T("Error, Coordinate Mode Exception (%d)"), CoordMode);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::OneAxisMoveTo NG-6 End"));
			return false; 
		}		
	}
	else
	{
		this->m_ErrorString.Format(_T("Error, Velocity Curve Exception (%d)"), CoordMode);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::OneAxisMoveTo NG-7 End"));
		return false; 
	}	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::OneAxisMoveTo OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::SetORGAll()
{
	if ( this->CheckInit() == false ) { return false; }
	if ( this->SetORG(AXIS_X) == false ) 
	{	return false; }	
	if ( this->SetORG(AXIS_Y) == false ) 
	{	return false; }	
	if ( this->SetORG(AXIS_Z) == false ) 
	{	return false; }	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::SetORG(U16 Axis)
{	
#ifndef MOTION_OBJ_DISABLE
	I32 pos = 0;
	I32 cmd = 0;
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }	
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }

	CString   Err;
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();
	const bool GantryAxis = MotionAxisPtr->CheckGantryAxis();
	CMotion_Basic::SaveMotionProcess(Axis, _T("SetORG"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::SetORG Start"));
	
	if ( true == GantryAxis )
	{		
		const U16 GantryNo = MotionAxisPtr->GetGantryID();
		//_ECAT_Slave_CSP_Gantry_Set_Position(U16 CardNo, U16 GantryNo, I32 NewPosition);  // 2016-11-28
		ErrorStatus = _ECAT_Slave_CSP_Gantry_Set_Position(CardNo, GantryNo, pos);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			Err.Format(_T("Error, _ECAT_Slave_Motion_Set_Position Fault"));
			SetErrorString(Err);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::SetORG NG-1 End"));
			return false; 
		}
	}
	else
	{
		ErrorStatus = _ECAT_Slave_Motion_Set_Position(CardNo, NodeID, SlotID, pos);//feedback
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			Err.Format(_T("Error, _ECAT_Slave_Motion_Set_Position Fault"));
			SetErrorString(Err);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::SetORG NG-1 End"));
			return false; 
		}	
	}
	
	ErrorStatus = _ECAT_Slave_Motion_Set_Command(CardNo, NodeID, SlotID, cmd);//command pulse
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		Err.Format(_T("Error, _ECAT_Slave_Motion_Set_Command Fault"));
		SetErrorString(Err);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::SetORG NG-2 End"));
		return false; 
	}
	
	MotionAxisPtr->SetCommandPos(0);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::SetORG OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
LPCTSTR CMotion_PCIE_L221_B1D0::GetAxisStatusText(int IO)//取得軸狀態的文字
{
	switch ( IO )
	{
	case PCIE_L221_B1D0_STATUS_WORD_RDYS:	m_MotionIOStats = _T("RDYS");	break;
	case PCIE_L221_B1D0_STATUS_WORD_RDY:	m_MotionIOStats = _T("RDY");	break;
	case PCIE_L221_B1D0_STATUS_WORD_SVON:	m_MotionIOStats = _T("Servo-ON");	break;
	case PCIE_L221_B1D0_STATUS_WORD_S_ERR:	m_MotionIOStats = _T("Server-Err");	break;
	case PCIE_L221_B1D0_STATUS_WORD_SPO:	m_MotionIOStats = _T("Servo Power On");	break;
	case PCIE_L221_B1D0_STATUS_WORD_EMG:	m_MotionIOStats = _T("Emergency Stop");	break;
	case PCIE_L221_B1D0_STATUS_WORD_ALM:	m_MotionIOStats = _T("Alarm");	break;
	case PCIE_L221_B1D0_STATUS_WORD_S_WARN:	m_MotionIOStats = _T("Server Alarm");	break;
	case PCIE_L221_B1D0_STATUS_WORD_KEEP1:	m_MotionIOStats = _T("Reverse 1");	break;
	case PCIE_L221_B1D0_STATUS_WORD_CON:	m_MotionIOStats = _T("Connected");	break;
	case PCIE_L221_B1D0_STATUS_WORD_INP:	m_MotionIOStats = _T("In-Position");	break;
	case PCIE_L221_B1D0_STATUS_WORD_LIM:	m_MotionIOStats = _T("Software Limit");	break;
	case PCIE_L221_B1D0_STATUS_WORD_OPM1:	m_MotionIOStats = _T("Operation Mode 1");	break;
	case PCIE_L221_B1D0_STATUS_WORD_OPM2:	m_MotionIOStats = _T("Operation Mode 2");	break;
	case PCIE_L221_B1D0_STATUS_WORD_KEEP2:	m_MotionIOStats = _T("Reverse 2");	break;
	case PCIE_L221_B1D0_STATUS_WORD_KEEP3:	m_MotionIOStats = _T("Reverse 3");	break;
	default:
		m_MotionIOStats = _T("No defined");
		break;
	}	
	return m_MotionIOStats;
}
//----------------------------------------------------------------------------------//
void CMotion_PCIE_L221_B1D0::GetAxisStatusIDList(std::vector<int> &IDList)
{
	IDList.clear();
	IDList.push_back(PCIE_L221_B1D0_STATUS_WORD_RDYS);
	IDList.push_back(PCIE_L221_B1D0_STATUS_WORD_RDY);
	IDList.push_back(PCIE_L221_B1D0_STATUS_WORD_SVON);
	IDList.push_back(PCIE_L221_B1D0_STATUS_WORD_S_ERR);
	IDList.push_back(PCIE_L221_B1D0_STATUS_WORD_SPO);
	IDList.push_back(PCIE_L221_B1D0_STATUS_WORD_EMG);
	IDList.push_back(PCIE_L221_B1D0_STATUS_WORD_ALM);
	IDList.push_back(PCIE_L221_B1D0_STATUS_WORD_S_WARN);
	IDList.push_back(PCIE_L221_B1D0_STATUS_WORD_KEEP1);
	IDList.push_back(PCIE_L221_B1D0_STATUS_WORD_CON);
	IDList.push_back(PCIE_L221_B1D0_STATUS_WORD_INP);
	IDList.push_back(PCIE_L221_B1D0_STATUS_WORD_LIM);
	IDList.push_back(PCIE_L221_B1D0_STATUS_WORD_OPM1);
	IDList.push_back(PCIE_L221_B1D0_STATUS_WORD_OPM2);
	IDList.push_back(PCIE_L221_B1D0_STATUS_WORD_KEEP2);
	IDList.push_back(PCIE_L221_B1D0_STATUS_WORD_KEEP3);
	return;
}
//----------------------------------------------------------------------------------//
int CMotion_PCIE_L221_B1D0::GetAxisStatus(U16 Axis, CString &str)//回傳該軸狀態, -1取資料出現異常, 要不則是狀態編碼, if pString==NULL, 不取錯誤文字
{
	str = _T("");
	if ( CheckInit() == false ) { return -1; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return -1; }		
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return 0; }

#ifndef MOTION_OBJ_DISABLE
	U16 Mdone=0;
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	
	//_ECAT_Slave_Motion_Get_Mdone(U16 CardNo, U16 NodeID, U16 SlotNo, U16 *Mdone);//0:Strop, 1:Moving
	ErrorStatus = _ECAT_Slave_Motion_Get_Mdone(CardNo, NodeID, SlotID, &Mdone);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CString Err;
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetAxisStatus NG-1 End"));
		SetErrorString(Err);
		return -1;
	}

	switch ( Mdone )
	{
	case PCIE_L221_B1D0_MDONE_STOP:	str.Format(_T("Stop"));	break;
	case PCIE_L221_B1D0_MDONE_ACCE: str.Format(_T("In acceleration"));	break;
	case PCIE_L221_B1D0_MDONE_REACH: str.Format(_T("In Max velocity motion"));	break;
	case PCIE_L221_B1D0_MDONE_DECE: str.Format(_T("In deceleration"));	break;
	case PCIE_L221_B1D0_MDONE_MAILBOX: str.Format(_T("In MailBox Comm."));	break;
	default:
		str.Format(_T("Others[%d]"), Mdone);
		break;
	}
	return (int)Mdone;
#else
	return 0;
#endif
	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::BuildCardNoList(std::vector<U16> &CardNoList)
{	
	size_t i=0, j=0;	
	const size_t EtherCatNodeCount=m_ECatNodeList.size();

	CardNoList.clear();
	for ( i=0; i<EtherCatNodeCount; i++ )
	{		
		size_t CardNoCount=CardNoList.size();
		CMotion_ECAT_Node &EtherCatNodeRef=m_ECatNodeList[i];
		U16 CardNo=EtherCatNodeRef.GetCardNo();
		for ( j=0; j<CardNoCount; j++ )
		{
			if ( CardNo == CardNoList[j] )
			{	break; }
		}
		if ( j < CardNoCount ) { continue; }
		CardNoList.push_back(CardNo);
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::AddETherCatNode(const CMotion_ECAT_Node &NodeRef)
{
	size_t i=0;
	std::vector<CMotion_ECAT_Node> &ECatNodeList=m_ECatNodeList;
	const size_t ECatNodeCount=ECatNodeList.size();

	for ( i=0; i<ECatNodeCount; i++ )
	{
		const CMotion_ECAT_Node &Ref=ECatNodeList[i];
		if ( Ref.CompareNode(NodeRef) == false ) { continue; }
		m_ErrorString.Format(_T("Error, Add Ether CAT Node Fault [Card:%d, Node:%d, Slot:%d]"), NodeRef.GetCardNo(), NodeRef.GetNodeNo(), NodeRef.GetSlotNo());
		return false;
	}
	ECatNodeList.push_back(NodeRef);
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::BuildMotionNodeList()//建立運動點列表
{	
	if ( LoadMotionNodeList() == false ) { return false; }	
	std::vector<CMotionNode> &MotionNodeListRef=GetMotionNodeList();
	const size_t MotionNodeCount = MotionNodeListRef.size();
	if ( MotionNodeCount > 0 ) { return true; }
	
	const int ECatNodeCount=(int)(m_ECatNodeList.size());
	for ( int i=0; i<ECatNodeCount; i++ )
	{
		const int AxisID=i;
		const CMotion_ECAT_Node &ECatNode=m_ECatNodeList[i];		
		const int CardNo = ECatNode.GetCardNo();
		const int NodeNo = ECatNode.GetNodeNo();
		const int SlotNo = ECatNode.GetSlotNo();
		ECAT_DRIVER_MODEL DriverModel = ECatNode.GetDriverModel();

		CMotionNode MotionNode;
		MotionNode.SetAxisID(AxisID);//軸號-AXIS_X, AXIS_Y, AXIS_Z;
		MotionNode.SetCardID(CardNo);//卡號
		MotionNode.SetNodeID(NodeNo);//通道或站號
		MotionNode.SetSlotID(SlotNo);//子號
		MotionNode.SetDriverModel(DriverModel);//驅動器型號

		MotionNode.SetGantryID(-1);//龍門邊號
		MotionNodeListRef.push_back(MotionNode); 			
	}	
	SaveMotionNodeList();
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::BuildMotionAxisList()//建立運動軸列表
{	
	ResetMotionAxisPtr();
	if ( BuildMotionNodeList() == false ) { return false; }
	std::vector<CMotionAxis> &MotionAxisListRef=GetMotionAxisList();
	const std::vector<CMotionNode> &MotionNodeListRef=GetMotionNodeList();
	if ( BuildMotionAxisListFn(MotionNodeListRef, MotionAxisListRef) == false )
	{	return false; }
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::CheckMotionNodeValid(const CMotionNode &Ref)//更新運動軸的驅動器型號
{
	size_t i=0;
	const U16 CardNo = Ref.GetCardID();
	const U16 NodeID = Ref.GetNodeID();
	const U16 SlotID = Ref.GetSlotID();	
	const size_t ECatNodeCount=m_ECatNodeList.size();
	for ( i=0; i<ECatNodeCount; i++ )
	{
		const CMotion_ECAT_Node &ECatNode=m_ECatNodeList[i];
		if ( CardNo != ECatNode.GetCardNo() ) { continue; }
		if ( NodeID != ECatNode.GetNodeNo() ) { continue; }
		if ( SlotID != ECatNode.GetSlotNo() ) { continue; }		
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::UpdateMotionAxisDriverModel(CMotionAxis &Ref)//更新軸控節點的驅動器型號
{
	size_t i=0, j=0;
	const U16 CardNo = Ref.GetCardID();
	const U16 NodeID = Ref.GetNodeID();
	const U16 SlotID = Ref.GetSlotID();	
	const size_t ECatNodeCount=m_ECatNodeList.size();
	for ( i=0; i<ECatNodeCount; i++ )
	{
		const CMotion_ECAT_Node &ECatNode=m_ECatNodeList[i];
		if ( CardNo != ECatNode.GetCardNo() ) { continue; }
		if ( NodeID != ECatNode.GetNodeNo() ) { continue; }
		if ( SlotID != ECatNode.GetSlotNo() ) { continue; }

		ECAT_DRIVER_MODEL DriverModel=ECatNode.GetDriverModel();
		Ref.SetDriverModel(DriverModel);
		const size_t SlaveNodeCount=Ref.GetSlaveNodeCount();
		for ( j=0; j<SlaveNodeCount; j++ )
		{
			CMotionNode *Ptr=Ref.GetSlaveNodePtr(j, false);
			if ( NULL == Ptr ) { continue; }
			Ptr->SetDriverModel(DriverModel);
		}
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::CheckReturnOK(U16 ret)
{
	if ( ERR_ECAT_NO_ERROR != ret )
	{
		CString Err;
		GetErrorCodeText(ret, Err);
		return false; 
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ReleaseMotionCard()
{	
	if ( GetInint() == false ) { return true; }

	DisableSoftwareLimit(AXIS_X);
	DisableSoftwareLimit(AXIS_Y);
	DisableSoftwareLimit(AXIS_Z);

	size_t i=0, j=0;
	std::vector<CMotionAxis> &MotionAxisList=GetMotionAxisList();
	const size_t MotionAxisCount=MotionAxisList.size();	

#ifndef MOTION_OBJ_DISABLE
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::ReleaseMotionCard Start"));		

	bool ResetFault=false;
	std::vector<U16> CardNoList;
	BuildCardNoList(CardNoList);
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	for( i=0;i<MotionAxisCount;i++)	
	{	
		CMotionAxis &MotionAxisRef=MotionAxisList[i];
		if ( CheckAxisBypass(&MotionAxisRef) == true ) { continue; }
		if ( CheckMotionNodeValid(MotionAxisRef.GetMotionNodeRef()) == false ) { continue; }

		const U16 Off = 0;		
		const U16 CardNo = MotionAxisRef.GetCardID();
		const U16 NodeID = MotionAxisRef.GetNodeID();
		const U16 SlotID = MotionAxisRef.GetSlotID();
		const U16 GantryNo = MotionAxisRef.GetGantryID();		
		const size_t SlaveNodeCount = MotionAxisRef.GetSlaveNodeCount();		
		for ( j=0; j<SlaveNodeCount; j++ )
		{
			CMotionNode *SlaveNodePtr=MotionAxisRef.GetSlaveNodePtr(j, false);
			if ( NULL == SlaveNodePtr ) { continue; }
			const U16 SlaveNodeID = SlaveNodePtr->GetNodeID();
			const U16 SlaveSlotID = SlaveNodePtr->GetSlotID();
			ErrorStatus = _ECAT_Slave_CSP_Follow_Enable(CardNo, GantryNo, NodeID, SlotID, SlaveNodeID, SlaveSlotID, Off);
			CheckReturnOK(ErrorStatus);

			ErrorStatus = _ECAT_Slave_Motion_Set_Svon(CardNo,NodeID,SlotID,Off);	
			CheckReturnOK(ErrorStatus);			
		}

		ErrorStatus = _ECAT_Slave_Motion_Set_Svon(CardNo,NodeID,SlotID,Off);	
		CheckReturnOK(ErrorStatus);			
	}

	const size_t CardNoCount=CardNoList.size();
	for ( size_t i=0; i<CardNoCount; i++ )
	{	
		const U16 CardNo=CardNoList[i];		
		ErrorStatus = _ECAT_Master_Reset(CardNo);	
		if ( CheckReturnOK(ErrorStatus) == false )
		{	ResetFault = true;	}
	}	
	if ( true == ResetFault )
	{
		this->m_ErrorString.Format(_T("Error, _ECAT_Master_Reset() Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::ReleaseMotionCard NG-1 End"));
		return false; 
	}
	ErrorStatus = _ECAT_Master_Close();
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _ECAT_Master_Close Fault"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::ReleaseMotionCard NG-2 End"));
		return false; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::ReleaseMotionCard OK End"));
#endif//MOTION_OBJ_DISABLE
	SetInitialize(false);
	SetIsSupportGantry(false);		

	for( i=0;i<MotionAxisCount;i++)	
	{	
		CMotionAxis &MotionAxisRef=MotionAxisList[i];		
		MotionAxisRef.SetIsHomed(false);
		MotionAxisRef.SetIsEnabled(false);
	}	
	return true;
}
//----------------------------------------------------------------------------------//
void CMotion_PCIE_L221_B1D0::SetTriggerParameter(double SP, double EP, double Start, double End, double Interval, int MaxRepeatCounts, int YMaxCounts, double YOffset, int ORGX, int ORGY, int ORGZ)
{	
//	this->SetSystemDefaultMotionParameter();	
	m_TriggerStartPos = SP;			//移動的起點	(含加速距離)	//chia031
	m_TriggerEndPos = EP;				//移動的終點	(含減速距離)	
	m_TriggerFirstOnePos = Start;		//第一個觸發點
	m_TriggerLastOnePos = End;		//最後的觸發點
	m_TriggerInterval = Interval;		//觸發的間距	
	m_TriggerYMaxCounts = YMaxCounts;
	m_TriggerMaxRepeatCounts = MaxRepeatCounts;
	m_ORGX = ORGX;
	m_ORGY = ORGY;
	m_ORGZ = ORGZ;
	//計算出每掃一條的觸發次數
	m_TriggerNTriggers=(int)((m_TriggerFirstOnePos-m_TriggerLastOnePos)/m_TriggerInterval);
	if ( m_TriggerNTriggers < 0 ) { m_TriggerNTriggers = -m_TriggerNTriggers; }	
	m_TriggerNTriggers = m_TriggerNTriggers+1;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ForwardTriggerProcess_JET7000S()//正向移動
{	
	return ReturnNoSupportFunc(_T("ForwardTriggerProcess_JET7000S"));	
	this->m_TriggerStatus = MOTION_TRIGGER_FORWARD;	
#ifndef MOTION_OBJ_DISABLE	
	const int CurrentFOVY = 0;//AOIDataCollect.GetCurrentMovingScanIndex();
	const size_t MaxScanCount = this->m_ScanPathList.size();
	const TMotionParameter &MotionParam=GetMotionParameter();
	if ( CurrentFOVY<0 || CurrentFOVY>=MaxScanCount ) 
	{	return false; }

	//此範圍沒有檢測框	
	if ( m_ScanPathList[CurrentFOVY].m_FOVStartIDX_C < 0 ) 
	{
		this->m_TriggerRepeatCounts ++;	
		return true; 
	}
	const float EndOffset=0.5f;	
	const int PreTriggerStart = m_ScanPathList[CurrentFOVY].m_TriggerStartPosX;	//第一個觸發點
	const int PreTriggerEnd = m_ScanPathList[CurrentFOVY].m_TriggerEndPosX;		//最後一個觸發點
	const int TriggerStart = (int)(PreTriggerStart-MotionParam.m_TriggerForwardOffset);		//第一個觸發點(含Trigger Offset)
	const int TriggerEnd = (int)(PreTriggerEnd-MotionParam.m_TriggerForwardOffset+EndOffset);	//最後一個觸發點(含Trigger Offset)
	const int StartPosX = m_ScanPathList[CurrentFOVY].m_StartPosX;		//掃描起始點(含加速距離)
	const int EndPosX = m_ScanPathList[CurrentFOVY].m_EndPosX;			//掃描結束點(含減速距離)
	const int CurrentPosY = m_ScanPathList[CurrentFOVY].m_CurrentPosY;	//當前的Y Stage

	this->m_TriggerStartPos = StartPosX;
	this->m_TriggerEndPos = EndPosX;

	this->m_TriggerFirstOnePos = TriggerStart;
	this->m_TriggerLastOnePos = TriggerEnd;	


	//建立比對的陣列
	F64 *pArray=0;
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	I16 ArraySize = (int)((TriggerEnd-TriggerStart)/m_TriggerInterval);
	//避免有正負號的問題
	if ( ArraySize < 0 ) 
	{	ArraySize = -ArraySize+1;	}
	else
	{	ArraySize = ArraySize+1;	}

	int LEDTriggerDis = TRIGGER_CCD_BETWEEN_LED;
	LEDTriggerDis = (int)(TRIGGER_CCD_BETWEEN_LED*0.001*MotionParam.m_TriggerMaxVelocity);	//時間換算成距離
	ArraySize = ArraySize*2;	//兩倍Trigger數量  LED + CCD
	int i=0;
	int LEDindex = 0;

	bool IsTest = false;	
	if( IsTest == true )
	{ pArray = new F64[ArraySize-1]; }
	else
	{ pArray = new F64[ArraySize]; }

	for ( i=0; i<ArraySize; i+=2 )
	{	
		pArray[i] = (int)(TriggerStart + LEDindex*m_TriggerInterval)-LEDTriggerDis;	//LED Trigger
		if( IsTest == true && i == ArraySize-2)
		{ break; }
		pArray[i+1] = pArray[i] + LEDTriggerDis;									//CCD Trigger
		LEDindex++;
	}

	/*
	//_ECAT_Compare_Set_Channel_Position(U16 CardNo, U16 CompareChannel, I32 Position);
	//_ECAT_Compare_Get_Channel_Position(U16 CardNo, U16 CompareChannel, I32 *Position);
	//_ECAT_Compare_Set_Ipulser_Mode(U16 CardNo, U16 Mode);	//0:AB phase, 1:CW/CCW
	//_ECAT_Compare_Set_Channel_Direction(U16 CardNo, U16 CompareChannel, U16 Dir);	//0:Normal, 1:Inverse
	//_ECAT_Compare_Set_Channel_Trigger_Time(U16 CardNo, U16 CompareChannel, U32 TimeUs);
	//_ECAT_Compare_Set_Channel_Trigger_Time_Multiple(U16 CardNo, U16 CompareChannel, U16 Multiple);  // 2016-03-29
	//_ECAT_Compare_Set_Channel_One_Shot(U16 CardNo, U16 CompareChannel);
	//_ECAT_Compare_Set_Channel_Source(U16 CardNo, U16 CompareChannel, U16 Source);
	//_ECAT_Compare_Set_Channel_Enable(U16 CardNo, U16 CompareChannel, U16 Enable);
	//Dir 1: Negativeu, 0:Positive
	//TriggerCount 0:infinity trigger
	//_ECAT_Compare_Channel0_Position(U16 CardNo, I32 Start, U16 Dir, U16 Interval, U32 TriggerCount);
	//_ECAT_Compare_Set_Channel0_Trigger_By_GPIO(U16 CardNo, U16 Dir, U16 Interval, I32 TriggerCount);
	//_ECAT_Compare_Set_Channel1_Output_Enable(U16 CardNo, U16 OnOff);	//0: Off, 1:On
	//_ECAT_Compare_Set_Channel1_Output_Mode(U16 CardNo, U16 Mode);
	//_ECAT_Compare_Get_Channel1_IO_Status(U16 CardNo, U16 *IOStatus);
	//_ECAT_Compare_Set_Channel1_GPIO_Out(U16 CardNo, U16 OnOff);
	//_ECAT_Compare_Set_Channel1_Position_Table(U16 CardNo, I32 *PosTable, U32 TableSize);
	//_ECAT_Compare_Set_Channel1_Position_Table_Level(U16 CardNo, I32 *PosTable, U32 *LevelTable, U32 TableSize);
	//_ECAT_Compare_Get_Channel1_Position_Table_Count(U16 CardNo, U32 *pCount);
	//_ECAT_Compare_Set_Channel_Polarity(U16 CardNo, U16 Inverse);	//0:Normal, 1:Inverse
	//_ECAT_Compare_Reuse_Channel1_Position_Table(U16 CardNo);
	//_ECAT_Compare_Reuse_Channel1_Position_Table_Level(U16 CardNo);
	// +2017-01-09
	//_ECAT_Compare_Channel0_Position_32Bit(U16 CardNo, I32 Start, U16 Dir, U32 Interval, U32 TriggerCount);
	//_ECAT_Compare_Set_Channel0_Trigger_By_GPIO_32Bit(U16 CardNo, U16 Dir, U32 Interval, I32 TriggerCount);
	//_ECAT_Compare_Set_Channel_Position_Table(U16 CardNo, U16 CompareChannel, I32 *PosTable, U32 TableSize);
	//_ECAT_Compare_Set_Channel_Position_Table_Level(U16 CardNo, U16 CompareChannel, I32 *PosTable, U32 *LevelTable, U32 TableSize);
	//_ECAT_Compare_Get_Channel_Position_Table_Count(U16 CardNo, U16 CompareChannel, U32 *Count);
	//_ECAT_Compare_Reuse_Channel_Position_Table(U16 CardNo, U16 CompareChannel);
	//_ECAT_Compare_Reuse_Channel_Position_Table_Level(U16 CardNo, U16 CompareChannel);
	// -2017-01-09
	//_ECAT_Compare_Set_Control_Mode(U16 CardNo, U16 CompareChannel, U16 Mode);  // 2018-11-29
	*/
	ErrorStatus = ERR_ECAT_NO_ERROR;
	const int Axis = TRIGGER_AXIS;	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }

	const int TriggerAxis = Axis;
	const int CompareNo_LED = 0;
	const int CompareNo_CCD = 1;
	const int Dir = 1;//CCW:0, CW:1;
	const int CntMode = 0;//0:A/B phase, 1:CW/CCW
	const int CntDir  = 0;//0:Normal, 1:Inverse
	const int NElems = (int)((TriggerEnd-TriggerStart)/m_TriggerInterval)+1;
	double CurPos = 0;
	this->GetEncode(Axis, CurPos);
	const long AxisCounter = (long)(CurPos);
	const U16 TriggerInterval = (U16)(m_TriggerInterval);
	const I32 Start1 = (I32)(pArray[0]);
	const I32 Start2 = (I32)(pArray[1]);
	
	//LED、相機分別觸發
	//1. Disable Auto Trigger  //_m114_start_auto_compare(U16 SwitchCardNo, U16 AxisNo, U16 OnOff);
	//ErrorStatus = _m114_start_auto_compare(m_CardNo, CompareNo_LED, 0);//LED Trigger
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
		return false;
	}
	//ErrorStatus = _m114_start_auto_compare(m_CardNo, CompareNo_CCD, 0);//Camera Trigger
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
		return false;
	}		
	
	//2. Set Axis Count  
	//ErrorStatus = _M114GL_set_axis_counter(m_CardNo, TriggerAxis, CntMode, CntDir, AxisCounter);//Axis Counter
	//if ( CheckReturnOK(ErrorStatus) == false )
	//{
	//	this->m_ErrorString.Format(_T("Error, _M114GL_start_auto_trigger Fault"));
	//	return false;
	//}

	//設定觸發軸與比較軸, _m114_set_auto_compare_source(U16 SwitchCardNo, U16 AxisNo, U16 SrcAxisNo)	
	//ErrorStatus = _m114_set_auto_compare_source(m_CardNo, CompareNo_LED, TriggerAxis);//Axis Counter
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_source(LED) Fault"));
		return false;
	}
	//ErrorStatus = _m114_set_auto_compare_source(m_CardNo, CompareNo_CCD, TriggerAxis);//Axis Counter
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_source(CCD) Fault"));
		return false;
	}
	//座標同步化// _m114_set_auto_compare_encoder (U16 SwitchCardNo, U16 AxisNo, I32 EncPos)//
	//ErrorStatus = _m114_set_auto_compare_encoder(m_CardNo, TriggerAxis, AxisCounter);//Axis Counter
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_encoder Fault"));
		return false;
	}	
	
	//3. Set Trigger Output pulse width  //_m114_set_auto_compare_trigger(U16 SwitchCardNo, U16 AxisNo, U16 Level, U16 Width);
	U16 Level = 0;//0->Normal Low, 1->Normal Hight
	//ErrorStatus = _m114_set_auto_compare_trigger(m_CardNo, CompareNo_LED, Level, 184);//LED Trigger 
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_trigger Fault"));
		return false;
	}
	//ErrorStatus = _m114_set_auto_compare_trigger(m_CardNo, CompareNo_CCD, Level, 184);//Camera Trigger 
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_trigger Fault"));
		return false;
	}

	//4.Set Auto Trigger parameter //_m114_set_auto_compare_function(U16 SwitchCardNo, U16 AxisNo, U8 Dir, I32 StrPos, I32 Interval, U16 TrgCnt);
	//ErrorStatus = _m114_set_auto_compare_function(m_CardNo, CompareNo_LED, Dir, Start1, TriggerInterval, NElems);//LED Trigger 
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_function Fault"));
		return false;
	}

	//ErrorStatus = _m114_set_auto_compare_function(m_CardNo, CompareNo_CCD, Dir, Start2, TriggerInterval, NElems);//Camera Trigger 
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_function Fault"));
		return false;
	}

	//5.Enable Auto Trigger
	//_m114_start_auto_compare(U16 SwitchCardNo, U16 AxisNo, U16 OnOff);
	//ErrorStatus = _m114_start_auto_compare(m_CardNo, CompareNo_LED, 1);//Camera Trigger
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
		return false;
	}
	//ErrorStatus = _m114_start_auto_compare(m_CardNo, CompareNo_CCD, 1);//Camera Trigger
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
		return false;
	}	
	delete[] pArray; pArray=NULL;
	
	U32 uMaxVel = 0;
	U32 uStrVel = 0;
	F32 fTacc = 0.0f;
	F32 fTdec = 0.0f;
	if ( MotionParam.m_TriggerStartVelocity < 0 ) { uStrVel = (U32)(-MotionParam.m_TriggerStartVelocity); }
	else { uStrVel = (U32)(MotionParam.m_TriggerStartVelocity); }
	if ( MotionParam.m_TriggerMaxVelocity < 0 ) { uMaxVel = (U32)(-MotionParam.m_TriggerMaxVelocity); }
	else { uMaxVel = (U32)(MotionParam.m_TriggerMaxVelocity); }
	fTacc = (F32)(MotionParam.m_TriggerAccelerationTime);
	fTdec = (F32)(MotionParam.m_TriggerDecelerationTime);
	//ErrorStatus = _m114_start_ta_move(m_CardNo, TriggerAxis, EndPosX, uStrVel ,uMaxVel, fTacc, fTdec);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_ta_move Fault in Trigger"));
		return false; 
	}
	
#endif//MOTION_OBJ_DISABLE
	this->m_TriggerRepeatCounts ++;	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::BackwardTriggerProcess_JET7000S()//逆向移動
{	
	return ReturnNoSupportFunc(_T("BackwardTriggerProcess_JET7000S"));	
	this->m_TriggerStatus = MOTION_TRIGGER_BACKWARD;
#ifndef MOTION_OBJ_DISABLE	
	const int CurrentFOVY = 0;//AOIDataCollect.GetCurrentMovingScanIndex();
	const size_t MaxScanCount = this->m_ScanPathList.size();
	const TMotionParameter &MotionParam=GetMotionParameter();
	if ( CurrentFOVY<0 || CurrentFOVY>=MaxScanCount ) 
	{	return false; }

	//此範圍沒有檢測框
	if ( m_ScanPathList[CurrentFOVY].m_FOVStartIDX_C < 0 ) 
	{ 
		this->m_TriggerRepeatCounts ++;	
		return true; 
	}	
	const float EndOffset=-0.5f;
	const int PreTriggerStart = m_ScanPathList[CurrentFOVY].m_TriggerStartPosX;	//第一個觸發點
	const int PreTriggerEnd = m_ScanPathList[CurrentFOVY].m_TriggerEndPosX;		//最後一個觸發點
	const int TriggerStart = (int)(MotionParam.m_TriggerBackwardOffset+PreTriggerEnd);			//第一個觸發點(含Trigger Offset)
	const int TriggerEnd  = (int)(MotionParam.m_TriggerBackwardOffset+PreTriggerStart+EndOffset);	//最後一個觸發點(含Trigger Offset)
	const int StartPosX = m_ScanPathList[CurrentFOVY].m_StartPosX;				//掃描起始點(含加速距離)
	const int EndPosX = m_ScanPathList[CurrentFOVY].m_EndPosX;					//掃描結束點(含減速距離)

	this->m_TriggerStartPos = StartPosX;
	this->m_TriggerEndPos = EndPosX;
	this->m_TriggerFirstOnePos = TriggerStart;
	this->m_TriggerLastOnePos = TriggerEnd;

	//建立比對的陣列
	F64 *pArray=0;
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	I16 ArraySize = (int)((TriggerEnd-TriggerStart)/m_TriggerInterval);	
	
	int LEDTriggerDis = TRIGGER_CCD_BETWEEN_LED;
	LEDTriggerDis = (int)(TRIGGER_CCD_BETWEEN_LED*0.001*MotionParam.m_TriggerMaxVelocity);	//時間換算成距離	

	int i=0;
	int LEDindex = 0;
	
	//避免有正負號的問題
	if ( ArraySize < 0 )
	{
		ArraySize = -ArraySize+1;
		ArraySize = ArraySize*2;	//兩倍Trigger數量  LED + CCD
		pArray = new F64[ArraySize];
		for ( i=0; i<ArraySize; i+=2 )
		{
			pArray[i] = (int)(TriggerStart - LEDindex*m_TriggerInterval) + LEDTriggerDis;	//LED Trigger
			pArray[i+1] = pArray[i] - LEDTriggerDis;										//CCD Trigger
			LEDindex++;
		}
	}
	else
	{
		ArraySize = ArraySize+1;
		ArraySize = ArraySize*2;	//兩倍Trigger數量  LED + CCD
		pArray = new F64[ArraySize];
		for ( i=0; i<ArraySize; i+=2 )
		{	
			pArray[i] = (int)(TriggerStart + LEDindex*m_TriggerInterval) + LEDTriggerDis;	//LED Trigger
			pArray[i+1] = pArray[i] - LEDTriggerDis;										//CCD Trigger
			LEDindex++;	
		}
	}
	
	/*
	//_ECAT_Compare_Set_Channel_Position(U16 CardNo, U16 CompareChannel, I32 Position);
	//_ECAT_Compare_Get_Channel_Position(U16 CardNo, U16 CompareChannel, I32 *Position);
	//_ECAT_Compare_Set_Ipulser_Mode(U16 CardNo, U16 Mode);	//0:AB phase, 1:CW/CCW
	//_ECAT_Compare_Set_Channel_Direction(U16 CardNo, U16 CompareChannel, U16 Dir);	//0:Normal, 1:Inverse
	//_ECAT_Compare_Set_Channel_Trigger_Time(U16 CardNo, U16 CompareChannel, U32 TimeUs);
	//_ECAT_Compare_Set_Channel_Trigger_Time_Multiple(U16 CardNo, U16 CompareChannel, U16 Multiple);  // 2016-03-29
	//_ECAT_Compare_Set_Channel_One_Shot(U16 CardNo, U16 CompareChannel);
	//_ECAT_Compare_Set_Channel_Source(U16 CardNo, U16 CompareChannel, U16 Source);
	//_ECAT_Compare_Set_Channel_Enable(U16 CardNo, U16 CompareChannel, U16 Enable);
	//Dir 1: Negativeu, 0:Positive
	//TriggerCount 0:infinity trigger
	//_ECAT_Compare_Channel0_Position(U16 CardNo, I32 Start, U16 Dir, U16 Interval, U32 TriggerCount);
	//_ECAT_Compare_Set_Channel0_Trigger_By_GPIO(U16 CardNo, U16 Dir, U16 Interval, I32 TriggerCount);
	//_ECAT_Compare_Set_Channel1_Output_Enable(U16 CardNo, U16 OnOff);	//0: Off, 1:On
	//_ECAT_Compare_Set_Channel1_Output_Mode(U16 CardNo, U16 Mode);
	//_ECAT_Compare_Get_Channel1_IO_Status(U16 CardNo, U16 *IOStatus);
	//_ECAT_Compare_Set_Channel1_GPIO_Out(U16 CardNo, U16 OnOff);
	//_ECAT_Compare_Set_Channel1_Position_Table(U16 CardNo, I32 *PosTable, U32 TableSize);
	//_ECAT_Compare_Set_Channel1_Position_Table_Level(U16 CardNo, I32 *PosTable, U32 *LevelTable, U32 TableSize);
	//_ECAT_Compare_Get_Channel1_Position_Table_Count(U16 CardNo, U32 *pCount);
	//_ECAT_Compare_Set_Channel_Polarity(U16 CardNo, U16 Inverse);	//0:Normal, 1:Inverse
	//_ECAT_Compare_Reuse_Channel1_Position_Table(U16 CardNo);
	//_ECAT_Compare_Reuse_Channel1_Position_Table_Level(U16 CardNo);
	// +2017-01-09
	//_ECAT_Compare_Channel0_Position_32Bit(U16 CardNo, I32 Start, U16 Dir, U32 Interval, U32 TriggerCount);
	//_ECAT_Compare_Set_Channel0_Trigger_By_GPIO_32Bit(U16 CardNo, U16 Dir, U32 Interval, I32 TriggerCount);
	//_ECAT_Compare_Set_Channel_Position_Table(U16 CardNo, U16 CompareChannel, I32 *PosTable, U32 TableSize);
	//_ECAT_Compare_Set_Channel_Position_Table_Level(U16 CardNo, U16 CompareChannel, I32 *PosTable, U32 *LevelTable, U32 TableSize);
	//_ECAT_Compare_Get_Channel_Position_Table_Count(U16 CardNo, U16 CompareChannel, U32 *Count);
	//_ECAT_Compare_Reuse_Channel_Position_Table(U16 CardNo, U16 CompareChannel);
	//_ECAT_Compare_Reuse_Channel_Position_Table_Level(U16 CardNo, U16 CompareChannel);
	// -2017-01-09
	//_ECAT_Compare_Set_Control_Mode(U16 CardNo, U16 CompareChannel, U16 Mode);  // 2018-11-29
	*/
	ErrorStatus = ERR_ECAT_NO_ERROR;
	const int Axis = TRIGGER_AXIS;
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }

	const int TriggerAxis = Axis;
	const int CompareNo_LED = 0;
	const int CompareNo_CCD = 1;
	const int Dir = 0;//CCW:0, CW:1;
	const int CntMode = 0;//0:A/B phase, 1:CW/CCW
	const int CntDir  = 0;//0:Normal, 1:Inverse
	const int NElems = (-(int)((TriggerEnd-TriggerStart)/m_TriggerInterval))+1;
	double CurPos = 0;
	this->GetEncode(Axis, CurPos);
	const long AxisCounter = (long)(CurPos);
	const U16 TriggerInterval = (U16)(m_TriggerInterval);
	const I32 Start1 = (I32)(pArray[0]);
	const I32 Start2 = (I32)(pArray[1]);
		
	//LED、相機分別觸發
	//1. Disable Auto Trigger  //_m114_start_auto_compare(U16 SwitchCardNo, U16 AxisNo, U16 OnOff);
	//ErrorStatus = _m114_start_auto_compare(m_CardNo, CompareNo_LED, 0);//LED Trigger
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
		return false;
	}
	//ErrorStatus = _m114_start_auto_compare(m_CardNo, CompareNo_CCD, 0);//Camera Trigger
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
		return false;
	}
	
	//2. Set Axis Count
	//ErrorStatus = _M114GL_set_axis_counter(m_CardNo, TriggerAxis, CntMode, CntDir, AxisCounter);//Axis Counter
	//if ( CheckReturnOK(ErrorStatus) == false )
	//{
	//	this->m_ErrorString.Format(_T("Error, _M114GL_start_auto_trigger Fault"));
	//	return false;
	//}
	//設定觸發軸與比較軸, _m114_set_auto_compare_source(U16 SwitchCardNo, U16 AxisNo, U16 SrcAxisNo)	
	//ErrorStatus = _m114_set_auto_compare_source(m_CardNo, CompareNo_LED, TriggerAxis);//Axis Counter
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_source(LED) Fault"));
		return false;
	}
	//ErrorStatus = _m114_set_auto_compare_source(m_CardNo, CompareNo_CCD, TriggerAxis);//Axis Counter
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_source(CCD) Fault"));
		return false;
	}
	//座標同步化// _m114_set_auto_compare_encoder (U16 SwitchCardNo, U16 AxisNo, I32 EncPos)//
	//ErrorStatus = _m114_set_auto_compare_encoder(m_CardNo, TriggerAxis, AxisCounter);//Axis Counter
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_encoder Fault"));
		return false;
	}	

	//3. Set Trigger Output pulse width
	//_m114_set_auto_compare_trigger(U16 SwitchCardNo, U16 AxisNo, U16 Level, U16 Width);
	U16 Level = 0;//0->Normal Low, 1->Normal Hight
	//ErrorStatus = _m114_set_auto_compare_trigger(m_CardNo, CompareNo_LED, Level, 184);//LED Trigger 
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_trigger Fault"));
		return false;
	}
	//ErrorStatus = _m114_set_auto_compare_trigger(m_CardNo, CompareNo_CCD, Level, 184);//Camera Trigger 
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_trigger Fault"));
		return false;
	}

	//4.Set Auto Trigger parameter  //_m114_set_auto_compare_function(U16 SwitchCardNo, U16 AxisNo, U8 Dir, I32 StrPos, I32 Interval, U16 TrgCnt);
	//ErrorStatus = _m114_set_auto_compare_function(m_CardNo, CompareNo_LED, Dir, Start1, TriggerInterval, NElems);//LED Trigger 
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_function Fault"));
		return false;
	}

	//ErrorStatus = _m114_set_auto_compare_function(m_CardNo, CompareNo_CCD, Dir, Start2, TriggerInterval, NElems);//Camera Trigger 
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_set_auto_compare_function Fault"));
		return false;
	}

	//5.Enable Auto Trigger
	//_m114_start_auto_compare(U16 SwitchCardNo, U16 AxisNo, U16 OnOff);
	//ErrorStatus = _m114_start_auto_compare(m_CardNo, CompareNo_LED, 1);//Camera Trigger
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
		return false;
	}
	//ErrorStatus = _m114_start_auto_compare(m_CardNo, CompareNo_CCD, 1);//Camera Trigger
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_auto_compare Fault"));
		return false;
	}
	delete[] pArray; pArray=NULL;

	U32 uMaxVel = 0;
	U32 uStrVel = 0;
	F32 fTacc = 0.0f;
	F32 fTdec = 0.0f;
	if ( MotionParam.m_TriggerStartVelocity < 0 ) { uStrVel = (U32)(-MotionParam.m_TriggerStartVelocity); }
	else { uStrVel = (U32)(MotionParam.m_TriggerStartVelocity); }
	if ( MotionParam.m_TriggerMaxVelocity < 0 ) { uMaxVel = (U32)(-MotionParam.m_TriggerMaxVelocity); }
	else { uMaxVel = (U32)(MotionParam.m_TriggerMaxVelocity); }
	fTacc = (F32)(MotionParam.m_TriggerAccelerationTime);
	fTdec = (F32)(MotionParam.m_TriggerDecelerationTime);
	//ErrorStatus = _m114_start_ta_move(m_CardNo, TriggerAxis, StartPosX, uStrVel, uMaxVel, fTacc, fTdec);	
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _m114_start_ta_move Fault in Trigger"));
		return false;
	}
	
#endif//MOTION_OBJ_DISABLE
	this->m_TriggerRepeatCounts ++;
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ForwardTriggerProcess()
{
	bool IsOK = false;
	IsOK = ForwardTriggerProcess_JET7000S();	
	if ( false == IsOK )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_MOVE);
		return false;
	}
	return IsOK;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::BackwardTriggerProcess()
{	
	bool IsOK = false;
	IsOK = this->BackwardTriggerProcess_JET7000S();
	if ( false == IsOK )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_MOVE);
		return false;
	}
	return IsOK;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::StopCompareTrigger(bool IsStop)//是否停止同步比較送外部觸發訊號
{	
#ifndef MOTION_OBJ_DISABLE
	const int CompareNo_LED = 0;
	const int CompareNo_CCD = 1;	
	CMotion_Basic::SaveMotionProcess(-1, _T("StopCompareTrigger"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::StopCompareTrigger Start"));
	if ( IsStop == true ) 
	{		
	}
	//SetMotionExceptionCode(AOI_EXCEPTION_MOTION_EXEC_FUNC);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::StopCompareTrigger End"));
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::EnableCompareTrigger(bool IsEnable)
{		
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }	
	CMotion_Basic::SaveMotionProcess(-1, _T("EnableCompareTrigger"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::EnableCompareTrigger Start"));
	if ( IsEnable == true )
	{	
		
	}
	else
	{	
		this->StopCompareTrigger(true);
	}	//取消Trigger送出	
	//SetMotionExceptionCode(AOI_EXCEPTION_MOTION_EXEC_FUNC);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::EnableCompareTrigger OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
void CMotion_PCIE_L221_B1D0::GetErrorCodeText(const int status, CString &str)
{	
#ifndef MOTION_OBJ_DISABLE
	char Buffer[256]="";
	_ECAT_Master_Get_Return_Code_Message((U16)status, Buffer);
	str = CString(Buffer);	
#endif//MOTION_OBJ_DISABLE
	return;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::PreInitMotion()//預先初始化
{
//	CMotion_Basic::PreInitMotion();	
	SetMotionName(_T("PCIE_L221_B1D0"));
	SetMotionCardType(MOTION_CARD_PCIE_L221_B1D0);
	m_MaxAxisCount = MAX_MOTION_AXIS;
	m_UsedAxisCount = MAX_USED_AXIS;
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::InitialMotion()//初始化
{	
	bool IsOK = true;

#ifdef MOTION_OBJ_DISABLE
	IsOK = true;
	SetInitialize(true);
#else
	CMotion_Basic::SaveMotionProcess(-1, _T("InitialMotion"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::InitialMotion Start"));	
	IsOK = InitialMotionCard();
	if ( IsOK == false ) 
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_CONNECT);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::InitialMotion NG End"));	
	}
	else
	{	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::InitialMotion OK End"));	}
#endif//MOTION_OBJ_DISABLE	
	return IsOK;	
	
}
//----------------------------------------------------------------------------------//	
bool CMotion_PCIE_L221_B1D0::ReleaseMotion()
{
	if ( ReleaseMotionCard() == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_RELEASE);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::GetMotionIsReady()//取得運動系統是否正常
{
	if ( CheckInit() == false ) { return false; }
	this->m_ErrorString = _T("");
	
	//先判斷是否Enable	
	U16 AxisNo = AXIS_X;
	CString Text;
	if ( this->GetIsEnable(AXIS_X) == false )
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("X Axis didn't Enabled. (%s)"), Text);
		return false;
	}

	if ( this->GetIsEnable(AXIS_Y) == false )
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("Y Axis didn't Enabled. (%s)"), Text);
		return false;
	}

	if ( this->GetIsEnable(AXIS_Z) == false )
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("Z Axis didn't Enabled. (%s)"), Text);
		return false;
	}
	
	//確認是否為ALARM狀態	
	if ( this->GetIsAlarm(AXIS_X) == true ) 
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("X Axis is Alarm. (%s)"), Text);
		return false;
	}
	if ( this->GetIsAlarm(AXIS_Y) == true ) 
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("Y Axis is Alarm. (%s)"), Text);
		return false;
	}
	if ( this->GetIsAlarm(AXIS_Z) == true ) 
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("Z Axis is Alarm. (%s)"), Text);
		return false;
	}

	//確認是否為Ready狀態	
	if ( this->GetIsReady(AXIS_X) == false ) 
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("X Axis is not Ready. (%s)"), Text);
		return false;
	}
	if ( this->GetIsReady(AXIS_Y) == false ) 
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("Y Axis is not Ready. (%s)"), Text);
		return false;
	}
	if ( this->GetIsReady(AXIS_Z) == false ) 
	{
		Text = this->m_ErrorString;
		this->m_ErrorString.Format(_T("Z Axis is not Ready. (%s)"), Text);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ExecHome_Slave(CMotionAxis *MotionAxisPtr, int SignPositive, double HomeVelocity, double HomeAccTime)
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }		

	CString Err;
	U16 Dir = 1;//0-Pot, 1-Neg
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;		
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	
	const bool UseHomeSensor=MotionAxisPtr->GetUseHomeSensor();

	U16 HomeMode = 5;//0~2	
	const I32 HomeOffset = 0;
	const U32 FirstVel = HomeVelocity;	
	const U32 SecondVel = HomeVelocity/10;
	const F64 Tacc = HomeAccTime;	
	const I32 Shift = 0;
	const U32 Acceleration = 500000;//加速度過小會導致還在加速過程中就碰到極限會造成異常-500000-20000
	if ( FN_ENABLE == SignPositive ) { Dir = 1; }
	else { Dir = 0; }
	if ( false == UseHomeSensor )
	{	HomeMode = 1;	}
	else
	{	HomeMode = 5;	}	
	
	//_ECAT_Slave_Home_Config(U16 CardNo, U16 NodeID, U16 SlotNo, U16 Mode, I32 Offset, U32 FirstVel, U32 SecondVel, U32 Acceleration);	//尋邊運動設定
	ErrorStatus = _ECAT_Slave_Home_Config(CardNo, NodeID, SlotID, HomeMode, HomeOffset, FirstVel, SecondVel, Acceleration);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		Err.Format(_T("Error, _ECAT_Slave_Home_Config(CardNo, CardAxis, SlotID, %d, 1)"), HomeMode);
		SetErrorString(Err);		
		return false;
	}	
	//_ECAT_Slave_Home_Move(U16 CardNo, U16 NodeID, U16 SlotNo);
	ErrorStatus = _ECAT_Slave_Home_Move(CardNo, NodeID, SlotID);
	if ( CheckReturnOK(ErrorStatus) == false )
	{	
		Err.Format(_T("Error, _ECAT_Slave_Home_Move Fault"));
		SetErrorString(Err);		
		return false; 
	}
	//_ECAT_Slave_Home_Status(U16 CardNo, U16 NodeID, U16 SlotNo, U16 *Status);			
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ExecHome_CSP(CMotionAxis *MotionAxisPtr, int SignPositive, double HomeVelocity, double HomeAccTime)
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }		

	CString Err;
	U16 Dir = 1;//0-Pot, 1-Neg
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;		
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	

	const U16 HomeNo = 0;//0~7
	const U16 HomeMode = 1;//0~2
	const I32 HomeOffset = 0;
	const U32 FirstVel = HomeVelocity;
	const U32 SecondVel = HomeVelocity;		
	const F64 Tacc = HomeAccTime;	
	const I32 Shift = 0;			
	if ( FN_ENABLE == SignPositive ) { Dir = 1; }
	else { Dir = 0; }
	//_ECAT_Slave_CSP_Home_Axis_Config(U16 CardNo, U16 HomeNo, U16 NodeID, U16 SlotID, U16 Mode, I32 Offset, U32 FirstVel, U32 SecondVel, F64 Tacc, U16 Dir, I32 Shift);
	ErrorStatus = _ECAT_Slave_CSP_Home_Axis_Config(CardNo, HomeNo, NodeID, SlotID, HomeMode, HomeOffset, FirstVel, SecondVel, Tacc, Dir, Shift);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		Err.Format(_T("Error, _ECAT_Slave_CSP_Home_Axis_Config(CardNo, CardAxis, SlotID, 2, 1)"));
		SetErrorString(Err);		
		return false;
	}		
	const U16 LimitNo = 0x02;//抓第3個Bit		
	//_ECAT_Slave_CSP_Home_Move(U16 CardNo, U16 HomeNo, U16 u16_LimitNo);
	ErrorStatus = _ECAT_Slave_CSP_Home_Move(CardNo, HomeNo, LimitNo);
	if ( CheckReturnOK(ErrorStatus) == false )
	{	
		Err.Format(_T("Error, _ECAT_Slave_CSP_Home_Move Fault"));
		SetErrorString(Err);		
		return false; 
	}	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ExecHome_TouchPro(CMotionAxis *MotionAxisPtr, int SignPositive, double HomeVelocity, double HomeAccTime)
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }		

	CString Err;
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;		
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	

	U16 Direction = 0;//0:Forward, 1:Backward
	const U16 TriggerSource = 0;//0: DI, 1:Z
	const U16 TriggerEdge = 0;//0:Pos, 1:Neg		
	const I32 HomeOffset = 0;
	const I32 StrVel = 0;
	const I32 FirstVel = HomeVelocity;
	const I32 SecondVel = HomeVelocity;		
	const I32 EndVel = 0;
	const F64 Tacc = HomeAccTime;
	const F64 Tdec = HomeAccTime;
	const U16 SCurve = 0;//0:Linear, 1:Curve		

	//SetORG(Axis, SysID) ;
	if ( FN_ENABLE == SignPositive ) { Direction = 0; }
	else { Direction = 1; }
	ErrorStatus = _ECAT_Slave_CSP_Start_TouchProb_Home_Move(CardNo, NodeID, SlotID, TriggerSource, TriggerEdge, Direction, HomeOffset, StrVel, FirstVel, SecondVel, EndVel, Tacc, Tdec, SCurve);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		Err.Format(_T("Error, _ECAT_Slave_CSP_Start_TouchProb_Home_Move(CardNo, NodeID, SlotID, TriggerSource, TriggerEdge, Direction, HomeOffset, StrVel, FirstVel, SecondVel, EndVel, Tacc, Tdec, SCurve)"));
		SetErrorString(Err);
		return false;
	}	
	
	U16 Home_Status = 0;
	DWORD HomeCheckCnt=0;
	I32 LastEncode=INT_MAX, NowEncode=0;	
	while ( true )
	{
		ErrorStatus = _ECAT_Slave_CSP_Start_TouchProb_Home_Status(CardNo, NodeID, SlotID, &Home_Status);
		CheckReturnOK(ErrorStatus);
		if ( 0 == Home_Status ) 
		{	break; }
		ErrorStatus = _ECAT_Slave_Motion_Get_Position(CardNo, NodeID, SlotID, &NowEncode);
		CheckReturnOK(ErrorStatus);	 

		HomeCheckCnt ++;
		if ( HomeCheckCnt>1000 || 99==Home_Status || NowEncode==LastEncode )
		{
			Err.Format(_T("Error, _ECAT_Slave_CSP_Start_TouchProb_Home_Move(CardNo, NodeID, SlotID, TriggerSource, TriggerEdge, Direction, HomeOffset, StrVel, FirstVel, SecondVel, EndVel, Tacc, Tdec, SCurve)"));
			SetErrorString(Err);			
			return false;
		}
		LastEncode = NowEncode;
		::Sleep(250);			
	};

	//_ECAT_Slave_CSP_Gantry_Home_Move(U16 CardNo, U16 GantryNo);	
	//_ECAT_Slave_CSP_Gantry_Disable_Home_Move(U16 CardNo, U16 GantryNo);
	//_ECAT_Slave_CSP_Gantry_Set_Home_Edge_Trigger_Level(U16 CardNo, U16 GantryNo, U16 PositiveLimt, U16 NegativeLimit, U16 HomeSensor);
	//_ECAT_Slave_CSP_Gantry_Set_Home_Config(U16 CardNo, U16 GantryNo, U16 Mode, I32 Offset, U32 FirstVel, U32 SecondVel, F64 Tacc, U16 Dir, I32 Shift);
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ReadMotionAxisLatchPosition(CMotionAxis *MotionAxisPtr)
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	CMotionNode *MotionNodePtr = NULL;	
	std::vector<CMotionNode*> NodeList;	
	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	NodeList.push_back(MotionAxisPtr->GetMotionNodePtr());
	for ( size_t i=0; i<SlaveNodeCount; i++ )
	{
		MotionNodePtr = MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == MotionNodePtr ) { continue; }
		NodeList.push_back(MotionNodePtr);
	}
	bool  bFinish=true;
	U16   Status = 0;
	I32   LatchPosition = 0;
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;	
	const size_t NodeCount=NodeList.size();
	DWORD TickGap=0, TickCount=GetTickCount();
	while ( true )
	{
		bFinish=true;
		for ( size_t i=0; i<NodeCount; i++ )
		{
			MotionNodePtr = NodeList[i];
			if ( DBL_MAX != MotionNodePtr->GetLatchPos() )
			{	continue; }
			bFinish = false;
			const U16 CardNo = MotionNodePtr->GetCardID();
			const U16 NodeID = MotionNodePtr->GetNodeID();
			const U16 SlotID = MotionNodePtr->GetSlotID();	
			ErrorStatus = _ECAT_Slave_Motion_Get_TouchProbe_Status(CardNo, NodeID, SlotID, &Status);
			if ( (0x02&Status) == 0 )
			{	continue; }
			ErrorStatus = _ECAT_Slave_Motion_Get_TouchProbe_Position(CardNo, NodeID, SlotID, &LatchPosition);			
			MotionNodePtr->SetLatchPos(LatchPosition);
		}
		if ( true == bFinish )
		{	break; }
		TickGap = GetTickCount()-TickCount;
		if ( TickGap > 20000 )//More than 10 sec
		{	return false;	}
	};


	for ( size_t i=0; i<NodeCount; i++ )
	{
		MotionNodePtr = NodeList[i];		
		if ( Exec_ECAT_Slave_Motion_Set_TouchProbe_Disable(MotionNodePtr) == false )
		{	return false; }	
		U8 PDO_Data = 0;
		const U16 ODIndex = 0x60B8;
		const U16 ODSubIndex = 0x00;	
		if ( Exec_ECAT_Slave_PDO_Set_OD_Data(MotionNodePtr, ODIndex, ODSubIndex, &PDO_Data) == false )
		{	return false; }
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::PrepareMotionNodeGantryOffset(CMotionNode *MotionNodePtr)
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionNodePtr(MotionNodePtr) == false ) { return false; }
	MotionNodePtr->SetLatchPos(DBL_MAX);
	MotionNodePtr->SetGantryOffset(0);
	MotionNodePtr->SetGantryCalcPos(0);
	if ( Exec_ECAT_Slave_Motion_Set_TouchProbe_Disable(MotionNodePtr) == false )
	{	return false; }

	U8 PDO_Data = 0;
	const U16 ODIndex = 0x60B8;
	const U16 ODSubIndex = 0x00;	
	if ( Exec_ECAT_Slave_PDO_Set_OD_Data(MotionNodePtr, ODIndex, ODSubIndex, &PDO_Data) == false )
	{	return false; }
	U8 PDO_Data2 = 0;
	const U16 IOType = 1;	//0 Rx , 1 Tx
	if ( Exec_ECAT_Slave_PDO_Get_OD_Data(MotionNodePtr, IOType, ODIndex, ODSubIndex, &PDO_Data2) == false )
	{	return false; }	

	U16 TriggerMode = 0;	//第一次被訊號觸發時，紀錄脈波位置 bit1
	U16 Signal_Source = 1;	//由馬達的Z 相訊號作為第一組的觸發擷取訊號	bit2
	if ( Exec_ECAT_Slave_Motion_Set_TouchProbe_Config(MotionNodePtr, TriggerMode, Signal_Source) == false )
	{	return false; }
	if ( Exec_ECAT_Slave_Motion_Set_TouchProbe_QuickStart(MotionNodePtr) == false )
	{	return false; }

	U8 PDO_Data_60B8 = 0;
	if ( TriggerMode == 1) { PDO_Data_60B8 |= 0x02; }	// bit1
	if ( Signal_Source == 1) { PDO_Data_60B8 |= 0x04; }	// bit2
	PDO_Data_60B8 |= 0x01;	// bit0	開啟第一組觸發擷取的功能
	PDO_Data_60B8 |= 0x10;	//bit4 第一組的上緣訊號被觸發時，開始觸發擷取功能
	if ( Exec_ECAT_Slave_PDO_Set_OD_Data(MotionNodePtr, ODIndex, ODSubIndex, &PDO_Data_60B8) == false )
	{	return false; }
	if ( Exec_ECAT_Slave_PDO_Get_OD_Data(MotionNodePtr, IOType, ODIndex, ODSubIndex, &PDO_Data2) == false )
	{	return false; }	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::PrepareMotionAxisGantryOffset(CMotionAxis *MotionAxisPtr)//預備軸控的龍門偏移
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }

	CMotionNode *MotionNodePtr = NULL;	
	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	for ( size_t i=0; i<SlaveNodeCount; i++ )
	{
		MotionNodePtr = MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == MotionNodePtr ) { continue; }
		if ( PrepareMotionNodeGantryOffset(MotionNodePtr) == false )
		{	return false; }
	}	
	if ( PrepareMotionNodeGantryOffset(MotionAxisPtr->GetMotionNodePtr()) == false )
	{	return false; }	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::FetchMotionAxisGantryOffset(CMotionAxis *MotionAxisPtr)//取得軸的龍門偏移
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckIsEnabled(MotionAxisPtr) == false ) { return false; }

	I32   Encode = 0;
	I32   Position=0.0;
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;
	const double FetchMoveGap = 100000;
	const int Axis=MotionAxisPtr->GetAxisID();
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();		
	const int nSign = MotionAxisPtr->GetSignPositive();	
	const bool bConvert=GetIsConvertSignPositive();//注意順序
	double HomePreMove = MotionAxisPtr->GetHomePreMoveDis();

	if ( PrepareMotionAxisGantryOffset(MotionAxisPtr) == false )
	{	return false; }

	ErrorStatus = _ECAT_Slave_Motion_Get_Position(CardNo, NodeID, SlotID, &Encode);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::CalibrateMotionAxisGantryOffset Fault"));
		return false; 
	}	
	if ( true==bConvert && FN_DISABLE==nSign )
	{	HomePreMove = -HomePreMove;	}
	if ( HomePreMove < 0 )
	{	Position = Encode-FetchMoveGap;	}
	else
	{	Position = Encode+FetchMoveGap;	}	
	const I32 StrVel = 0;
	const I32 EndVel = 0;
	const I32 ConstVel = HOME_MAX_VELOCITY/5;
	const F64 Tacc = (F64)(HOME_ACCELERATE_TIME);
	const F64 Tdec = (F64)(HOME_ACCELERATE_TIME);
	const U16 IsAbs = 1;
	const U16 SCurve= 1;

	//Move the Axis
	ErrorStatus = _ECAT_Slave_CSP_Start_Move(CardNo, NodeID, SlotID, Position, StrVel, ConstVel, EndVel, Tacc, Tdec, SCurve, IsAbs);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::CalibrateMotionAxisGantryOffset Fault"));
		return false; 
	}	
	if ( ReadMotionAxisLatchPosition(MotionAxisPtr) == false )
	{	return false; }

	//Stop the Axis
	ErrorStatus = _ECAT_Slave_Motion_Sd_Stop(CardNo, NodeID, SlotID, Tdec);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::CalibrateMotionAxisGantryOffset Fault"));
		return false; 
	}

	//Wait the Axis Done
	if ( WaitForDone(Axis) == false )
	{	return false; }

	CMotionNode *MotionNodePtr = NULL;	
	double LatchOffset=0.0, LatchSlave=0.0;	
	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	double LatchPosMaster=MotionAxisPtr->GetMotionNodePtr()->GetLatchPos();
	for ( size_t i=0; i<SlaveNodeCount; i++ )
	{
		MotionNodePtr = MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == MotionNodePtr ) { continue; }
		LatchSlave = MotionNodePtr->GetLatchPos();
		LatchOffset = LatchSlave-LatchPosMaster;
		MotionNodePtr->SetGantryOffset(LatchOffset);
	}	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::CorrectMotionAxisGantryOffset(CMotionAxis *MotionAxisPtr)//校正軸的龍門偏移
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckIsEnabled(MotionAxisPtr) == false ) { return false; }

	//Check Gantry Offset is Different?
	bool bNeedCalc=false;
	if ( CalcMotionAxisGantryOffset(MotionAxisPtr, bNeedCalc) == false )
	{	return false; }
	if ( false == bNeedCalc ) 
	{	return true; }		
	DWORD DelayTime=250;
	if ( 0 != DelayTime )//需要延遲一下, 否則後面移動沒有甚麼效果
	{	::Sleep(DelayTime);	}	
	//Disable Gantry Mode
	if ( EnableMotionAxisSlaveGantry(MotionAxisPtr, false) == false )
	{	return false; }	
	if ( 0 != DelayTime )//需要延遲一下, 否則後面移動沒有甚麼效果
	{	::Sleep(DelayTime);	}	
	//Move Slave Position
	if ( MoveMotionAxisSlavePosition(MotionAxisPtr) == false )
	{	return false; }	
	if ( 0 != DelayTime )//需要延遲一下, 否則後面啟用龍門容易失敗
	{	::Sleep(DelayTime);	}	
	//Enable Gantry Mode
	if ( EnableMotionAxisSlaveGantry(MotionAxisPtr, true) == false )
	{	return false; }	
	if ( 0 != DelayTime )//需要延遲一下, 否則後面移動沒有甚麼效果
	{	::Sleep(DelayTime);	}		
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::CalcMotionAxisGantryOffset(CMotionAxis *MotionAxisPtr, bool &bCali)//計算軸的龍門偏移
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }

	CMotionNode *MotionNodePtr = NULL;	
	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();

	//Check Gantry Offset is Different?
	bool bNeedCalc=false;
	double LatchOffset=0.0;
	double LatchOffsetGap=0.0;
	double LatchStdOffset=0.0;	
	for ( size_t i=0; i<SlaveNodeCount; i++ )
	{
		MotionNodePtr = MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == MotionNodePtr ) { continue; }		
		LatchOffset = MotionNodePtr->GetGantryOffset();
		LatchStdOffset = MotionNodePtr->GetGantryStdOffset();
		if ( MotionNodePtr->GetGantryEnableOffset() == false )
		{	continue; }
		if ( MotionNodePtr->CheckGantryStdOffsetValid() == false )
		{	continue;	}

		LatchOffsetGap = LatchOffset-LatchStdOffset;
		MotionNodePtr->SetGantryCalcPos(LatchOffsetGap);
		const int nGap = (int)(LatchOffsetGap);
		if ( abs(nGap) > 0 )
		{	bNeedCalc = true;	}
	}
	bCali = bNeedCalc;
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::CalibrateMotionAxisGantryOffset(CMotionAxis *MotionAxisPtr)//校正軸的龍門偏移
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	//Fetch Gantry Offset
	if ( FetchMotionAxisGantryOffset(MotionAxisPtr) == false )
	{	return false; }
	//Correct Gantry Offset
	if ( CorrectMotionAxisGantryOffset(MotionAxisPtr) == false )
	{	return false; }	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::MoveMotionAxisSlavePosition(CMotionAxis *MotionAxisPtr)//移動軸的子軸位置
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	I32   Encode2=0;
	I32   Encode=0, Position=0;
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;		
	DWORD TickGap=0, TickCount=0;
	const I32 StrVel = 0;
	const I32 EndVel = 0;
	const I32 ConstVel = HOME_MAX_VELOCITY/5;
	const F64 Tacc = (F64)(HOME_ACCELERATE_TIME);
	const F64 Tdec = (F64)(HOME_ACCELERATE_TIME);
	const U16 IsAbs = 1;
	const U16 SCurve= 1;
	const size_t SlaveCount = MotionAxisPtr->GetSlaveNodeCount();
	for ( size_t i=0; i<SlaveCount; i++ )
	{
		CMotionNode *SlaveNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == SlaveNodePtr ) { continue; }	
		const U16 CardNo = SlaveNodePtr->GetCardID();	
		const U16 NodeID = SlaveNodePtr->GetNodeID();
		const U16 SlotID = SlaveNodePtr->GetSlotID();
		const int GantryGap= (SlaveNodePtr->GetGantryCalcPos());
		if ( 0 == GantryGap ) { continue; }
		if ( abs(GantryGap) > 1000 ) { continue; }

		ErrorStatus = _ECAT_Slave_Motion_Get_Position(CardNo, NodeID, SlotID, &Encode);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::MoveMotionAxisSlavePosition Fault"));
			return false; 
		}
		Position = Encode+GantryGap;
		//GantryGap
		ErrorStatus = _ECAT_Slave_CSP_Start_Move(CardNo, NodeID, SlotID, Position, StrVel, ConstVel, EndVel, Tacc, Tdec, SCurve, IsAbs);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::MoveMotionAxisSlavePosition Fault"));
			return false; 
		}	

		TickCount=GetTickCount();
		while ( true )
		{			
			ErrorStatus = _ECAT_Slave_Motion_Get_Position(CardNo, NodeID, SlotID, &Encode2);
			if ( fabs(Encode2-Position)<1 )
			{	break; }
			TickGap=GetTickCount()-TickCount;
			if ( TickGap > 2000 )
			{	break;	}
			::Sleep(10);
		};
	}	
	if ( 0 != TickCount )
	{	::Sleep(100);		}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::EnableMotionAxisSlaveGantry(CMotionAxis *MotionAxisPtr, bool bEnable)//啟用軸的子軸龍門
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;		
	const U16 OnOff = bEnable;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 GantryNo = MotionAxisPtr->GetGantryID();
	const U16 MasterNodeID = MotionAxisPtr->GetNodeID();
	const U16 MasterSlotID = MotionAxisPtr->GetSlotID();		
	const size_t SlaveCount = MotionAxisPtr->GetSlaveNodeCount();
	for ( size_t i=0; i<SlaveCount; i++ )
	{
		CMotionNode *SlaveNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == SlaveNodePtr ) { continue; }		
		const U16 SlaveNodeID = SlaveNodePtr->GetNodeID();
		const U16 SlaveSlotID = SlaveNodePtr->GetSlotID();		
		ErrorStatus = _ECAT_Slave_CSP_Follow_Enable(CardNo, GantryNo, MasterNodeID, MasterSlotID, SlaveNodeID, SlaveSlotID, OnOff);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			//CString Err;
			//GetErrorCodeText(ErrorStatus, Err);
			//JetAPI::ShowMessageBox(Err);
			m_ErrorString.Format(_T("Error, _ECAT_Slave_CSP_Follow_Enable(CardNo, GantryNo, MasterCardAxis, MasterSlotID, SlaveCardAxis, SlaveSlotID, OnOff=%d)"), OnOff);
			return false;
		}
	}	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::Exec_ECAT_Slave_Motion_Set_TouchProbe_Disable(CMotionNode *MotionNodePtr)//關閉『觸發擷取』功能
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionNodePtr(MotionNodePtr) == false ) { return false; }
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;		
	const U16 CardNo = MotionNodePtr->GetCardID();
	const U16 NodeID = MotionNodePtr->GetNodeID();
	const U16 SlotID = MotionNodePtr->GetSlotID();	
	//Disable
	ErrorStatus = _ECAT_Slave_Motion_Set_TouchProbe_Disable(CardNo, NodeID, SlotID);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Exec_ECAT_Slave_Motion_Set_TouchProbe_Disable Fault"));
		return false; 
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::Exec_ECAT_Slave_Motion_Set_TouchProbe_QuickStart(CMotionNode *MotionNodePtr)//快速致能『觸發擷取』功能
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionNodePtr(MotionNodePtr) == false ) { return false; }
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;		
	const U16 CardNo = MotionNodePtr->GetCardID();
	const U16 NodeID = MotionNodePtr->GetNodeID();
	const U16 SlotID = MotionNodePtr->GetSlotID();	
	//Disable
	ErrorStatus = _ECAT_Slave_Motion_Set_TouchProbe_QuickStart(CardNo, NodeID, SlotID);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Exec_ECAT_Slave_Motion_Set_TouchProbe_QuickStart Fault"));
		return false; 
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::Exec_ECAT_Slave_Motion_Get_TouchProbe_Status(CMotionNode *MotionNodePtr, U16 *Status)//取得當前『觸發擷取』功能的狀態
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionNodePtr(MotionNodePtr) == false ) { return false; }
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;		
	const U16 CardNo = MotionNodePtr->GetCardID();
	const U16 NodeID = MotionNodePtr->GetNodeID();
	const U16 SlotID = MotionNodePtr->GetSlotID();	
	//Disable
	ErrorStatus = _ECAT_Slave_Motion_Get_TouchProbe_Status(CardNo, NodeID, SlotID, Status);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Exec_ECAT_Slave_Motion_Get_TouchProbe_Status Fault"));
		return false; 
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::Exec_ECAT_Slave_Motion_Get_TouchProbe_Position(CMotionNode *MotionNodePtr, I32 *LatchPosition)//取得當前擷取到的位置
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionNodePtr(MotionNodePtr) == false ) { return false; }
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;		
	const U16 CardNo = MotionNodePtr->GetCardID();
	const U16 NodeID = MotionNodePtr->GetNodeID();
	const U16 SlotID = MotionNodePtr->GetSlotID();	
	//Disable
	ErrorStatus = _ECAT_Slave_Motion_Get_TouchProbe_Position(CardNo, NodeID, SlotID, LatchPosition);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Exec_ECAT_Slave_Motion_Get_TouchProbe_Position Fault"));
		return false; 
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::Exec_ECAT_Slave_Motion_Set_TouchProbe_Config(CMotionNode *MotionNodePtr, U16 TriggerMode, U16 Signal_Source)//設定『觸發擷取』功能的行為模式	
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionNodePtr(MotionNodePtr) == false ) { return false; }
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;		
	const U16 CardNo = MotionNodePtr->GetCardID();
	const U16 NodeID = MotionNodePtr->GetNodeID();
	const U16 SlotID = MotionNodePtr->GetSlotID();	
	//Disable
	ErrorStatus = _ECAT_Slave_Motion_Set_TouchProbe_Config(CardNo, NodeID, SlotID, TriggerMode, Signal_Source);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Exec_ECAT_Slave_Motion_Set_TouchProbe_Config Fault"));
		return false; 
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::Exec_ECAT_Slave_PDO_Set_OD_Data(CMotionNode *MotionNodePtr, U16 ODIndex, U16 ODSubIndex, U8 *Data)
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionNodePtr(MotionNodePtr) == false ) { return false; }
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;		
	const U16 CardNo = MotionNodePtr->GetCardID();
	const U16 NodeID = MotionNodePtr->GetNodeID();
	const U16 SlotID = MotionNodePtr->GetSlotID();
	ErrorStatus = _ECAT_Slave_PDO_Set_OD_Data (CardNo, NodeID, SlotID, ODIndex, ODSubIndex, Data);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Exec_ECAT_Slave_PDO_Set_OD_Data Fault"));
		return false; 
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::Exec_ECAT_Slave_PDO_Get_OD_Data(CMotionNode *MotionNodePtr, U16 IOType, U16 ODIndex, U16 ODSubIndex, U8 *Data)
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }
	if ( CheckMotionNodePtr(MotionNodePtr) == false ) { return false; }
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;		
	const U16 CardNo = MotionNodePtr->GetCardID();
	const U16 NodeID = MotionNodePtr->GetNodeID();
	const U16 SlotID = MotionNodePtr->GetSlotID();
	const U16 ByteSize = (U16)(sizeof(U16));
	ErrorStatus = _ECAT_Slave_PDO_Get_OD_Data(CardNo, NodeID, SlotID, IOType, ODIndex, ODSubIndex, ByteSize, Data);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::_ECAT_Slave_PDO_Get_OD_Data Fault"));
		return false; 
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ExecHome(int Axis)
{		
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }		
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	CString AxisS = MotionAxisPtr->GetAxisName();

#ifndef MOTION_OBJ_DISABLE	
	CString str;
	CString Err;
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;		

	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	
	const bool GantryAxis = MotionAxisPtr->CheckGantryAxis();
	ECAT_DRIVER_MODEL DriverModel = MotionAxisPtr->GetDriverModel();

	str.Format(_T("Home %s"), AxisS);	
	const TMotionParameter &MotionParam=GetMotionParameter();
	CMotion_Basic::SaveMotionProcess(Axis, _T("Home"), MSG_LEVEL_HIGH);	
	AOIDataCollect.SaveLogMessage(str);		

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home Start"));

	//if ( MotionParam.m_UsingMotionSoftwareLimit == FN_ENABLE )
	{	DisableSoftwareLimit(Axis);	}

	if ( MotionAxisPtr->GetIsEnabled() == false )
	{ 
		Err.Format(_T("%s axis not enabled, Please Enable it First!"), AxisS);
		SetErrorString(Err);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home NG-1 End"));
		return false; 
	}
	if ( GetIsAlarm(Axis) == true )
	{
		Err.Format(_T("%s axis was alarm, Please Solve it First!"), AxisS);
		SetErrorString(Err);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home NG-2 End"));
		return false; 
	}	
	MotionAxisPtr->SetIsHomed(false);
	
	int    SignPositive=0;
	double HomeVelocity = 0;
	double ORGOffset = 10000;
	double ORGVelocity  = 50000;
	double HomePreMove = HOME_PRE_MOVE_DIST;	
	const bool bConvert=GetIsConvertSignPositive();	

	ORGOffset = MotionAxisPtr->GetHomeOrgOffset();
	ORGVelocity = MotionAxisPtr->GetHomeVelocity();
	HomePreMove = MotionAxisPtr->GetHomePreMoveDis();
	SignPositive = MotionAxisPtr->GetSignPositive();
	HomeVelocity = ::fabs(ORGVelocity);
	
	if ( true==bConvert && FN_DISABLE==SignPositive )
	{	
		ORGOffset = -ORGOffset;	
		HomePreMove = -HomePreMove;
	}

	//晚後移動, 避免撞機(低速模式)	
	U32 uStrVel = 0;
	U32 uMaxVel = 0;	
	U16 SCurve = 1;
	U16 IsAbs = 0;
	const F32 fTacc = (F32)(HOME_ACCELERATE_TIME); 
	const F32 fTdec = (F32)(HOME_ACCELERATE_TIME); 

	uStrVel = 0;
	if ( HOME_MAX_VELOCITY < 0 ) { uMaxVel =(U32)(-HOME_MAX_VELOCITY); }
	else { uMaxVel = (U32)(HOME_MAX_VELOCITY); }
	uStrVel = uMaxVel/10;
	
	//_ECAT_Slave_CSP_Start_Move(U16 CardNo, U16 NodeID, U16 SlotNo, I32 Dist, I32 StrVel, I32 ConstVel, I32 EndVel, F64 Tacc, F64 Tdec, U16 SCurve, U16 IsAbs);	//單軸點對點運動
	ErrorStatus = _ECAT_Slave_CSP_Start_Move(CardNo, NodeID, SlotID, HomePreMove, uStrVel, uMaxVel, uStrVel, fTacc, fTdec, SCurve, IsAbs);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		Err.Format(_T("Error, _ECAT_Slave_CSP_Start_Move Fault"));
		SetErrorString(Err);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home NG-3 End"));
		return false; 
	}		
	//----------等待歸零結束---------------------------------------
	if ( WaitForDone(Axis) == false ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home NG-4 End"));
		return false; 
	}

	if ( GetIsEmergencyOn(Axis) == true )
	{ 
		Err.Format(_T("%s axis is Emergency On, Please Solve it First!"), AxisS);
		SetErrorString(Err);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home NG-5 End"));
		return false; 
	}
	::Sleep(250);

	//高速模式的歸零, 但是不準	
	bool HomeOk=false;
	double HomeAccTime = 0.25;
	const int UseHomeMoveMode=CheckSupportHomeMoveMode(MotionAxisPtr);		
	switch ( UseHomeMoveMode )
	{
	default: m_ErrorString.Format(_T("Error, Home Move Mode Exception(%d)"), UseHomeMoveMode);
	case PCIE_L221_B1D0_HOME_MOVE_SLAVE:	 HomeOk = ExecHome_Slave(MotionAxisPtr, SignPositive, HomeVelocity, HomeAccTime);	break;
	case PCIE_L221_B1D0_HOME_MOVE_CSP:		 HomeOk = ExecHome_CSP(MotionAxisPtr, SignPositive, HomeVelocity, HomeAccTime);	break;
	case PCIE_L221_B1D0_HOME_MOVE_TOUCH_PRO: HomeOk = ExecHome_TouchPro(MotionAxisPtr, SignPositive, HomeVelocity, HomeAccTime);	break;
	}
	if ( false == HomeOk )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home NG-3 End"));
		return false; 
	}
	//----------等待歸零結束---------------------------------------
	if ( WaitForDone(Axis) == false ) 
	{ 
		//ErrorStatus = _M114GL_set_el(m_CardNo,CardAxis,0);//關閉SlowDown
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home NG-10 End"));
		return false; 
	}

	if ( GetIsEmergencyOn(Axis) == true )
	{ 
		Err.Format(_T("%s axis is Emergency On, Please Solve it First!"), AxisS);
		SetErrorString(Err);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home NG-6 End"));
		return false; 
	}

	//----------重設原點位置----------------------------------------//
	::Sleep(50);

	//ErrorStatus = _M114GL_set_el(m_CardNo,CardAxis,0);//關閉SlowDown
	if ( SetORG(Axis) == false ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home NG-13 End"));
		return false; 
	}
	if ( GetIsEmergencyOn(Axis) == true )
	{ 
		Err.Format(_T("%s axis is Emergency On, Please Solve it First!"), AxisS);
		SetErrorString(Err);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home NG-13 End"));
		return false; 
	}
	const bool bCalibrateGantryOffset=MotionAxisPtr->GetGantryEnableOffset();
	if ( true==bCalibrateGantryOffset && true==MotionAxisPtr->CheckGantryAxis() )
	{
		if ( CalibrateMotionAxisGantryOffset(MotionAxisPtr) == false )
		{
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home NG-13 End"));
			return false; 
		}
	}

	double Vel = HOME_MAX_VELOCITY;
	//double Vel = ORGVelocity;
	if ( Vel < 0 ) { uMaxVel = (U32)(-Vel); }		
	else { uMaxVel = (U32)(Vel); }	
	ErrorStatus = _ECAT_Slave_CSP_Start_Move(CardNo, NodeID, SlotID, ORGOffset, 0, uMaxVel, 0, fTacc, fTdec, SCurve, IsAbs);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home NG-14 End"));
		return false; 
	}

	if ( WaitForDone(Axis) == false ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home NG-15 End"));
		return false; 
	}

	if ( GetIsEmergencyOn(Axis) == true )
	{ 
		Err.Format(_T("%s axis is Emergency On, Please Solve it First!"), AxisS);
		SetErrorString(Err);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home NG-7 End"));
		return false; 
	}

	if ( SetORG(Axis) == false ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home NG-16 End"));
		return false; 
	}

	if ( MotionParam.m_UsingMotionSoftwareLimit == FN_ENABLE )
	{
		const double Max = MotionAxisPtr->GetLimitMax();
		const double Min = MotionAxisPtr->GetLimitMin();
		SetSoftwareLimit(Axis, Min, Max, false);		
		EnableSoftwareLimit(Axis);
	}
	MotionAxisPtr->SetIsHomed(true);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Home OK End"));	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::Home(int Axis)
{
	if ( ExecHome(Axis) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_HOME);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ExecEnable(int Axis)
{	
#ifndef MOTION_OBJ_DISABLE
	CString Err;
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;
	//const I16 axis = Axis;
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	if ( MotionAxisPtr->GetIsEnabled() == true ) { return true; }//後續龍門不能重複啟用否則會報異常
	
	const U16 On = 1;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	
	CMotion_Basic::SaveMotionProcess(Axis, _T("Enable"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Enable Start"));	
	ErrorStatus = _ECAT_Slave_Motion_Set_Svon(CardNo, NodeID, SlotID, On);
	if ( CheckReturnOK(ErrorStatus) == false )//0->turn on
	{
		Err.Format(_T("Error, _ECAT_Slave_Motion_Set_Svon(Enable)"));
		SetErrorString(Err);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Enable NG End"));
		return false;
	}

	const U16 GantryNo = MotionAxisPtr->GetGantryID();
	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	for ( int i=0; i<SlaveNodeCount; i++ )
	{
		CMotionNode *SlaveNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == SlaveNodePtr ) { continue; }
		const U16 MasterNodeID = NodeID;
		const U16 MasterSlotID = SlotID;
		const U16 SlaveNodeID = SlaveNodePtr->GetNodeID();
		const U16 SlaveSlotID = SlaveNodePtr->GetSlotID();
		ErrorStatus = _ECAT_Slave_Motion_Set_Svon(CardNo, SlaveNodeID, SlaveSlotID, On);
		if ( CheckReturnOK(ErrorStatus) == false )//0->turn on
		{
			Err.Format(_T("Error, _ECAT_Slave_Motion_Set_Svon(Enable)"));
			SetErrorString(Err);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Enable NG End"));
			return false;
		}		
		ErrorStatus = _ECAT_Slave_CSP_Follow_Enable(CardNo, GantryNo, MasterNodeID, MasterSlotID, SlaveNodeID, SlaveSlotID, On);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _ECAT_Slave_CSP_Follow_Enable(CardNo, GantryNo, MasterCardAxis, MasterSlotID, SlaveCardAxis, SlaveSlotID, 1)"));
			return false;
		}
		SlaveNodePtr->SetIsEnabled(true);
	}
	MotionAxisPtr->SetIsEnabled(true);

	const bool bHomed = MotionAxisPtr->GetIsHomed();
	if ( true == bHomed )
	{
		if ( ResetMotionCardCommandPos(Axis) == false )
		{	MotionAxisPtr->SetIsHomed(false);	}		 
	}	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Enable OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::Enable(int Axis)
{
	if ( ExecEnable(Axis) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_ENABLE);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ExecDisable(int Axis)
{	
#ifndef MOTION_OBJ_DISABLE
	CString Err;
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;
	//const I16 axis = Axis;
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	
	const U16 Off = 0;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();		
	const U16 GantryNo = MotionAxisPtr->GetGantryID();
	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	CMotion_Basic::SaveMotionProcess(Axis, _T("Disable"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Disable Start"));	
	for ( int i=0; i<SlaveNodeCount; i++ )
	{
		CMotionNode *SlaveNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == SlaveNodePtr ) { continue; }
		const U16 MasterNodeID = NodeID;
		const U16 MasterSlotID = SlotID;
		const U16 SlaveNodeID = SlaveNodePtr->GetNodeID();
		const U16 SlaveSlotID = SlaveNodePtr->GetSlotID();		
		ErrorStatus = _ECAT_Slave_CSP_Follow_Enable(CardNo, GantryNo, MasterNodeID, MasterSlotID, SlaveNodeID, SlaveSlotID, Off);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _ECAT_Slave_CSP_Follow_Enable(CardNo, GantryNo, MasterCardAxis, MasterSlotID, SlaveCardAxis, SlaveSlotID, 0)"));
			return false;
		}
		ErrorStatus = _ECAT_Slave_Motion_Set_Svon(CardNo, SlaveNodeID, SlaveSlotID, Off);
		if ( CheckReturnOK(ErrorStatus) == false )//1->turn off
		{
			Err.Format(_T("Error, _ECAT_Slave_Motion_Set_Svon(Disable)"));
			SetErrorString(Err);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Disable NG End"));
			return false;
		}
		SlaveNodePtr->SetIsEnabled(false);
	}

	ErrorStatus = _ECAT_Slave_Motion_Set_Svon(CardNo, NodeID, SlotID, Off);
	if ( CheckReturnOK(ErrorStatus) == false )//1->turn off
	{
		Err.Format(_T("Error, _ECAT_Slave_Motion_Set_Svon(Disable)"));
		SetErrorString(Err);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Disable NG End"));
		return false;
	}	
	MotionAxisPtr->SetIsEnabled(false);	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::Disable OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::Disable(int Axis)
{
	if ( ExecDisable(Axis) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_DISABLE);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ExecWaitForDone(int Axis, int MaxPreCounts)	
{		
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) {	return false;	}	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }	
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }	
	CString AxisS = MotionAxisPtr->GetAxisName();
	const bool bWaitForInp=MotionAxisPtr->GetIsWaitForInp();
	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();

	CMotion_Basic::SaveMotionProcess(Axis, _T("WaitForDone"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::WaitForDone Start"));
	if ( WaitForNodeDone(MotionAxisPtr->GetMotionNodePtr()) == false )
	{	
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::WaitForDone NG End"));
		return false; 
	}
	for (size_t i=0; i<SlaveNodeCount; i++)
	{
		CMotionNode *MotionNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == MotionNodePtr ) { continue; }
		MotionNodePtr->SetIsWaitForInp(bWaitForInp);
		if ( WaitForNodeDone(MotionNodePtr) == false )
		{
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::WaitForDone NG End"));
			return false;
		}
	}	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::WaitForDone OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::WaitForDone(int Axis, int MaxPreCounts)
{
	if ( ExecWaitForDone(Axis, MaxPreCounts) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_MOVE_DONE);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::WaitForDone_Backup(int Axis, int MaxPreCounts)
{		
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) {	return false;	}	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }	
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	CString AxisS = MotionAxisPtr->GetAxisName();
	
	CString   str;
	CString   Err;
	int       IoID = 0;
	U16       Mdone = 0;
	U16       io_sts = 0;
	DWORD     TickCnt1=0;
	DWORD     TickCnt2=0;
	DWORD     TickCntD=0;
	//const int SeelpTime = 1;//10ms, 不能太短
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const DWORD MaxTickCount=20000;//20 sec	
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	
	const int SeelpTime = GetMotionParameter().m_WaitForDoneDwellTime;	
	CMotion_Basic::SaveMotionProcess(Axis, _T("WaitForDone"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::WaitForDone Start"));	

	TickCnt1 = ::GetTickCount();	
	while ( true )
	{	
		//_ECAT_Slave_Motion_Get_Mdone(U16 CardNo, U16 NodeID, U16 SlotNo, U16 *Mdone);//0:Strop, 1:Moving
		ErrorStatus = _ECAT_Slave_Motion_Get_Mdone(CardNo, NodeID, SlotID, &Mdone);
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			str = _T("Error, wait for done too long");
			str = LoadMultiLanguageString(str, str);
			Err.Format(_T("%s [%s, time:%d ms]"), str, AxisS, TickCntD);
			SetErrorString(Err);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::WaitForDone NG End"));
			return false;
		}
		if ( PCIE_L221_B1D0_MDONE_STOP == Mdone )
		{	break; }

		ErrorStatus = _ECAT_Slave_Motion_Get_StatusWord(CardNo, NodeID, SlotID, &io_sts);
		if ( CheckReturnOK(ErrorStatus) == true )
		{
			//if ( (io_sts&PCIE_L221_B1D0_STATUS_WORD_EMG) != 0x00 )
			//{	break;	}
			if ( (io_sts&PCIE_L221_B1D0_STATUS_WORD_ALM) != 0x00 )
			{	break;	}
		}
		
		TickCnt2 = ::GetTickCount();
		TickCntD = TickCnt2-TickCnt1;
		if ( TickCntD > MaxTickCount )
		{
			str = _T("Error, wait for done too long");
			str = LoadMultiLanguageString(str, str);
			Err.Format(_T("%s [%s, time:%d ms]"), str, AxisS, TickCntD);
			SetErrorString(Err);
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::WaitForDone NG End"));
			return false; 
		}

		if ( SeelpTime > 0 )
		{	::Sleep(SeelpTime); }
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::WaitForDone OK End"));
	return true;	
#endif//MOTION_OBJ_DISABLE
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ExecTriggerMoveTo(int Axis, double TargetPos, bool IsModifyVelocity)//觸發移動至哪裡
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) {	return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }	
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }

	F64 StrVel=0, MaxVel=300000, Tacc=0.1, Tdec=0.1;	
	const TMotionParameter &MotionParam=GetMotionParameter();
	MaxVel = MotionParam.m_TriggerMaxVelocity;
	StrVel = MotionAxisPtr->GetStartVelocity();	
	Tacc = MotionAxisPtr->GetAccelerationTime();
	Tdec = MotionAxisPtr->GetDecelerationTime();
//	return this->OneAxisMoveTo(Axis, Dist, StrVel, MaxVel, Tacc, Tdec, MOVE_CURVE_S, MOVE_COORDINATE_ABS);
	return this->OneAxisMoveTo(Axis, TargetPos, StrVel, MaxVel, Tacc, Tdec, MOVE_CURVE_T, MOVE_COORDINATE_ABS);
#endif//MOTION_OBJ_DISABLE
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::TriggerMoveTo(int Axis, double TargetPos, bool IsModifyVelocity)//觸發移動至哪裡
{
	if ( ExecTriggerMoveTo(Axis, TargetPos, IsModifyVelocity) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_MOVE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ExecMoveTo(int Axis, double TargetPos, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }	
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }

	MOVE_CURVE_MODE MoveCurveMode=MOVE_CURVE_T;
	F64 StrVel=0, MaxVel=300000, Tacc=0.1, Tdec=0.1;
	const TMotionParameter &MotionParam=GetMotionParameter();
	
	Tacc = MotionAxisPtr->GetAccelerationTime();
	Tdec = MotionAxisPtr->GetDecelerationTime();
	MoveCurveMode = MotionAxisPtr->GetMovingCurveMode();
	MaxVel = GetMotionAxisVelocity(MotionAxisPtr, MovingMode);	

	MotionAxisPtr->SetCommandPos(TargetPos);
//	return this->OneAxisMoveTo(Axis, Dist, StrVel, MaxVel, Tacc, Tdec, MOVE_CURVE_S, MOVE_COORDINATE_ABS);
	return this->OneAxisMoveTo(Axis, TargetPos, StrVel, MaxVel, Tacc, Tdec, MoveCurveMode, MOVE_COORDINATE_ABS);
#endif
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::MoveTo(int Axis, double TargetPos, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)
{
	if ( ExecMoveTo(Axis, TargetPos, MovingMode, IsModifyVelocity) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_MOVE);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::XYMoveTo(double PosX, double PosY, bool Offline, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)//2軸移動，但非同動唷
{
	if ( ExecXYMoveTo(PosX, PosY, Offline, MovingMode, IsModifyVelocity) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_MOVE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ExecXYMoveTo(double PosX, double PosY, bool Offline, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)//2軸移動，但非同動唷
{
	if ( CheckNeedMoveXY(PosX, PosY, Offline) == false )//與之前的移動位置相同
	{	return true; }

	SetCommandOffline(Offline);
	CMotionAxis *MotionAxisPtrX=GetMotionAxisPtr(AXIS_X);
	CMotionAxis *MotionAxisPtrY=GetMotionAxisPtr(AXIS_Y);	
	if ( CheckMotionAxisPtr(MotionAxisPtrX) == true )
	{	MotionAxisPtrX->SetCommandPos(PosX); }
	if ( CheckMotionAxisPtr(MotionAxisPtrY) == true )
	{	MotionAxisPtrY->SetCommandPos(PosY);	}	
	if ( true == Offline )
	{	return true; }
#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	const double ToleranceX=GetPosToleranceX();//誤差值
	const double ToleranceY=GetPosToleranceY();//誤差值
	const double ToleranceZ=GetPosToleranceZ();//誤差值
	double EncodeX=0;
	double EncodeY=0;	
	//GetCurrentPos(EncodeX, EncodeY, EncodeZ);//已校正的座標
	GetEncode(AXIS_X, EncodeX, Offline);//要取未校正的座標
	GetEncode(AXIS_Y, EncodeY, Offline);//要取未校正的座標	

	double CaliX=0, CaliY=0;
	StageToCali(PosX, PosY, CaliX, CaliY);
	//XY座標會因為另一軌位置不同而變更, 以最後軸座標來比較
	if ( fabs(CaliX-EncodeX) > ToleranceX ) 
	{	
		if ( MoveTo(AXIS_X, CaliX, MovingMode, IsModifyVelocity) == false )
		{	return false; }		
		if ( CheckMotionAxisPtr(MotionAxisPtrX) == true )
		{	MotionAxisPtrX->SetCommandPos(PosX); }
	}

	if ( fabs(CaliY-EncodeY) > ToleranceY ) 
	{
		if ( MoveTo(AXIS_Y, CaliY, MovingMode, IsModifyVelocity) == false )
		{	return false; }
		if ( CheckMotionAxisPtr(MotionAxisPtrY) == true )
		{	MotionAxisPtrY->SetCommandPos(PosY);	}
	}	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ExecXYZMoveTo(double PosX, double PosY, double PosZ, bool Offline, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)//3軸移動，但非同動唷
{	
	if ( CheckNeedMoveXYZ(PosX, PosY, PosZ, Offline) == false )//與之前的移動位置相同
	{	return true; }

	SetCommandOffline(Offline);
	CMotionAxis *MotionAxisPtrX=GetMotionAxisPtr(AXIS_X);
	CMotionAxis *MotionAxisPtrY=GetMotionAxisPtr(AXIS_Y);
	CMotionAxis *MotionAxisPtrZ=GetMotionAxisPtr(AXIS_Z);
	if ( CheckMotionAxisPtr(MotionAxisPtrX) == true )
	{	MotionAxisPtrX->SetCommandPos(PosX); }
	if ( CheckMotionAxisPtr(MotionAxisPtrY) == true )
	{	MotionAxisPtrY->SetCommandPos(PosY);	}
	if ( CheckMotionAxisPtr(MotionAxisPtrZ) == true )
	{	MotionAxisPtrZ->SetCommandPos(PosZ); }
	if ( true == Offline )
	{	return true; }

#ifndef MOTION_OBJ_DISABLE	
	if ( this->CheckInit() == false ) { return false; }
	const double ToleranceX=GetPosToleranceX();//誤差值
	const double ToleranceY=GetPosToleranceY();//誤差值
	const double ToleranceZ=GetPosToleranceZ();//誤差值
	double EncodeX=0;
	double EncodeY=0;
	double EncodeZ=0;
	//GetCurrentPos(EncodeX, EncodeY, EncodeZ);//已校正的座標
	GetEncode(AXIS_X, EncodeX, Offline);//要取未校正的座標
	GetEncode(AXIS_Y, EncodeY, Offline);//要取未校正的座標
	GetEncode(AXIS_Z, EncodeZ, Offline);//要取未校正的座標

	double CaliX=0, CaliY=0;
	StageToCali(PosX, PosY, CaliX, CaliY);
	//XY座標會因為另一軌位置不同而變更, 以最後軸座標來比較
	if ( fabs(CaliX-EncodeX) > ToleranceX ) 
	{	
		if ( MoveTo(AXIS_X, CaliX, MovingMode, IsModifyVelocity) == false )
		{	return false; }		
		if ( CheckMotionAxisPtr(MotionAxisPtrX) == true )
		{	MotionAxisPtrX->SetCommandPos(PosX); }
	}

	if ( fabs(CaliY-EncodeY) > ToleranceY ) 
	{
		if ( MoveTo(AXIS_Y, CaliY, MovingMode, IsModifyVelocity) == false )
		{	return false; }
		if ( CheckMotionAxisPtr(MotionAxisPtrY) == true )
		{	MotionAxisPtrY->SetCommandPos(PosY);	}
	}
	
	if ( fabs(PosZ-EncodeZ) > ToleranceZ )
	{	
		if ( MoveTo(AXIS_Z, PosZ, MovingMode, IsModifyVelocity) == false )
		{	return false; }
		if ( CheckMotionAxisPtr(MotionAxisPtrZ) == true )
		{	MotionAxisPtrZ->SetCommandPos(PosZ); }
	}
#endif//MOTION_OBJ_DISABLE
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::XYZMoveTo(double PosX, double PosY, double PosZ, bool Offline, MOTION_MOVING_MODE MovingMode, bool IsModifyVelocity)//3軸移動，但非同動唷
{
	if ( ExecXYZMoveTo(PosX, PosY, PosZ, Offline, MovingMode, IsModifyVelocity) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_MOVE);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::GetEncode(int Axis, double &Encode, bool offline)//取得該軸的光學尺座標
{
	Encode = GetCommandPos(Axis);
#ifndef MOTION_OBJ_DISABLE	
	I32 encode = 0;
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	if ( true == offline )	{	return true;	}

	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	
//	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetEncode Start"));
	//_ECAT_Slave_Motion_Get_Position(U16 CardNo, U16 NodeID, U16 SlotNo, I32 *Position);//讀取當前MotionSlave的Position
	ErrorStatus = _ECAT_Slave_Motion_Get_Position(CardNo, NodeID, SlotID, &encode);
	Encode = encode;
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = MotionAxisPtr->GetSignPositive();
		if ( FN_DISABLE == nSign )
		{	Encode = -Encode;	}
	}

	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _ECAT_Slave_Motion_Get_Position Fault"));
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_READ_ENCODE);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetEncode NG End"));
		return false; 
	}
//	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetEncode OK End"));
#else
	Encode = GetCommandPos(Axis);	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::GetCurrentPos(double &X, double &Y, bool offline)//取得目前機台位置
{
	double PosZ=0;
	return GetCurrentPos(X, Y, PosZ, offline);
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::GetCurrentPos(double &X, double &Y, double &Z, bool offline)//取得目前機台位置
{	
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }	
	if ( true == offline )
	{
		GetCommandPos(X, Y, Z);
		return true;
	}
	F64 EncodeX=0;
	F64 EncodeY=0;
	F64 EncodeZ=0;
	if ( this->GetEncode(AXIS_X, EncodeX, offline) == false ) { return false; }
	if ( this->GetEncode(AXIS_Y, EncodeY, offline) == false ) { return false; }
	if ( this->GetEncode(AXIS_Z, EncodeZ, offline) == false ) { return false; }
	X = EncodeX;
	Y = EncodeY;
	Z = EncodeZ;	

	double PosX=0, PosY=0;
	CaliToStage(EncodeX, EncodeY, PosX, PosY);
	X = PosX;
	Y = PosY;	
#else
	GetCommandPos(X, Y, Z);
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::GetCurrentRawPos(double &X, double &Y, double &Z, bool offline)//取得目前機台原始位置
{
#ifndef MOTION_OBJ_DISABLE
	if ( this->CheckInit() == false ) { return false; }	
	if ( true == offline )
	{
		GetCommandPos(X, Y, Z);
		return true;
	}
	F64 EncodeX=0;
	F64 EncodeY=0;
	F64 EncodeZ=0;
	if ( this->GetEncode(AXIS_X, EncodeX, offline) == false ) { return false; }
	if ( this->GetEncode(AXIS_Y, EncodeY, offline) == false ) { return false; }
	if ( this->GetEncode(AXIS_Z, EncodeZ, offline) == false ) { return false; }
	X = EncodeX;
	Y = EncodeY;
	Z = EncodeZ;	
#else
	GetCommandPos(X, Y, Z);
#endif//MOTION_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::GetIsEnable(int Axis)//該軸是否為Serve ON, 也就是有送電來積磁
{	
	bool bEnable = false;
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return false; }
	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsEnable Start"));
	if ( ReadAxisStatusWord_Enable(MotionAxisPtr, bEnable) == false )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsEnable NG End"));
		return false; 
	}	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsEnable OK End"));	
#endif//MOTION_OBJ_DISABLE
	return bEnable;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::GetIsReady(int Axis)//Driver傳回該軸是否為RDY狀態
{	
	bool bReady = false;
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return false; }

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsReady Start"));	
	if ( ReadAxisStatusWord_Ready(MotionAxisPtr, bReady) == false )	
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsReady NG End"));
		return false; 
	}	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsReady OK End"));	
#endif//MOTION_OBJ_DISABLE
	return bReady;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ResetMotionCardCommandPos(int Axis)//重設軸控卡的命令位置
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }

	CString   Err;
	I32 encode = 0;	
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();
	//_ECAT_Slave_Motion_Get_Position(U16 CardNo, U16 NodeID, U16 SlotNo, I32 *Position);//讀取當前MotionSlave的Position
	ErrorStatus = _ECAT_Slave_Motion_Get_Position(CardNo, NodeID, SlotID, &encode);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		Err.Format(_T("Error, _ECAT_Slave_Motion_Get_Position(CardNo, CardAxis, SlotID, &encode)"));
		SetErrorString(Err);
		return false;
	}
	//_ECAT_Slave_Motion_Set_Command(U16 CardNo, U16 NodeID, U16 SlotNo, I32 Cmd);//設定當前MotionSlave的命令位置
	ErrorStatus = _ECAT_Slave_Motion_Set_Command(CardNo, NodeID, SlotID, (int)(encode));
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		Err.Format(_T("Error, _ECAT_Slave_Motion_Set_Command(CardNo, CardAxis, SlotID, (int)(encode))"));
		SetErrorString(Err);
		return false;
	}
	double ComdPos=ConvertAxisPos(MotionAxisPtr, encode);
	MotionAxisPtr->SetCommandPos(ComdPos);	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ExecDoFaultAck(int Axis)
{//Alarm Reset	
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	
	CString   Err;	
	size_t    i=0;
	U16   ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	
	const bool GantryAxis = MotionAxisPtr->CheckGantryAxis();
	CMotion_Basic::SaveMotionProcess(Axis, _T("DoFaultAck"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::DoFaultAck Start"));	
	
	//_ECAT_Slave_Motion_Ralm(U16 CardNo, U16 NodeID, U16 SlotNo);//重置當前MotionSlave的錯誤狀態
	ErrorStatus = _ECAT_Slave_Motion_Ralm(CardNo, NodeID, SlotID);	
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		Err.Format(_T("Error, _ECAT_Slave_Motion_Ralm(CardNo, CardAxis, SlotID)"));
		SetErrorString(Err);
		return false;
	}
	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	for ( i=0; i<SlaveNodeCount; i++ )
	{
		CMotionNode *MotionNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == MotionNodePtr ) { continue; }
		const U16 SlaveNodeID = MotionNodePtr->GetNodeID();
		const U16 SlaveSlotID = MotionNodePtr->GetSlotID();	
		ErrorStatus = _ECAT_Slave_Motion_Ralm(CardNo, SlaveNodeID, SlaveSlotID);	
		CheckReturnOK(ErrorStatus);
	}	
	
	if ( true == GantryAxis )
	{
		U16 GantryStatus=0;		
		U16 GantryPassLevel=0;

		const F64 Tacc = 0.2;
		const F64 Tdec = 0.2;
		const I32 Velocity = 5000;		
		const U16 GantryNo = MotionAxisPtr->GetGantryID();

		ErrorStatus = _ECAT_Slave_CSP_Gantry_Get_Status(CardNo, GantryNo, &GantryStatus, &GantryPassLevel);
		CheckReturnOK(ErrorStatus);

		//_ECAT_Slave_CSP_Gantry_Ralm(U16 CardNo, U16 GantryNo, I32 Velocity, F64 Tacc, F64 Tdec);		
		ErrorStatus = _ECAT_Slave_CSP_Gantry_Ralm(CardNo, GantryNo, Velocity, Tacc, Tdec);		
		CheckReturnOK(ErrorStatus);

		//_ECAT_Slave_CSP_Gantry_Ralm_Status(U16 CardNo, U16 GantryNo, U16 *Status);
		ErrorStatus = _ECAT_Slave_CSP_Gantry_Ralm_Status(CardNo, GantryNo, &GantryStatus);
		CheckReturnOK(ErrorStatus);		
	}

	const bool bHomed = MotionAxisPtr->GetIsHomed();
	if ( true == bHomed )
	{
		if ( ResetMotionCardCommandPos(Axis) == false )
		{	MotionAxisPtr->SetIsHomed(false);	}		 
	}	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::DoFaultAck OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::DoFaultAck(int Axis)
{
	if ( ExecDoFaultAck(Axis) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_FAULT_ACK);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
const TCHAR* CMotion_PCIE_L221_B1D0::GetAxisStatus(int Axis)
{
	CMotion_PCIE_L221_B1D0::GetAxisStatus(Axis, m_MotionStatus);
	return this->m_MotionStatus;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::GetIsERCActive(int Axis)//Driver傳回該軸是否為ERC Active狀態
{	
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsERCActive Start"));
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsERCActive OK-2 End"));
#endif//MOTION_OBJ_DISABLE
	return false; 	
}
//-----------------------------------------------------------------------
bool CMotion_PCIE_L221_B1D0::GetIsEmergencyOn(int Axis)//該軸是否收到急停訊號
{	
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }	
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return false; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsEmergencyOn Start"));
	
	U16 Status = 0;
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	
	ErrorStatus = _ECAT_GPIO_Emg_Get_Status(CardNo, &Status);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsEmergencyOn NG-1 End"));
		return false; 
	}
	//Bit-0:Emg Off/On, Bit-1:Level Low/High, Bit-2:Enable Emg Off/On
	if ( (Status & 0x01) != 0x00 ) 
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsEmergencyOn OK-1 End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsEmergencyOn OK-2 End"));
#endif//MOTION_OBJ_DISABLE
	return false; 	
}
//-----------------------------------------------------------------------
bool CMotion_PCIE_L221_B1D0::GetIsAlarm(int Axis)//Driver傳回該軸是否為Alarm狀態
{	
	bool bAlarm = false;	
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }	
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return false; }
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsAlarm Start"));		
	if ( ReadAxisStatusWord_Alarm(MotionAxisPtr, bAlarm) == false )
	{	
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsAlarm NG End"));
		return false; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsAlarm OK End"));	
#endif//MOTION_OBJ_DISABLE
	return bAlarm; 	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::GetIsInPosition(int Axis)//Driver傳回該軸是否為In Position
{
	bool bINP = false;	
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return false; }

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsInPosition Start"));
	
	if ( ReadAxisStatusWord_INP(MotionAxisPtr, bINP) == false )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsInPosition NG End"));
		return false; 
	}	

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsInPosition OK End"));	

	/*
	U16 Status = 0;
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	
	ECAT_DRIVER_MODEL DriverModel = MotionAxisPtr->GetDriverModel();		
	if ( CheckSupportInPosition(DriverModel) == false )	
	{
		ErrorStatus = _ECAT_Slave_CSP_Get_SoftTargetReach_Status(CardNo, NodeID, SlotID, &Status); //0: Off, 1:On
		if ( CheckReturnOK(ErrorStatus) == false )
		{
			m_ErrorString.Format(_T("Error, _ECAT_Slave_CSP_Get_SoftTargetReach_Status(CardNo, NodeID, SlotID, &Status)"));
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsInPosition NG End"));
			return false;
		}
		if ( 0 != Status ) 
		{ 
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsInPosition OK End"));
			return true; 
		}
	}
	else
	{	
		if ( GetAxisStatusWord(Axis, Status) == false )
		{ 
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsInPosition NG End"));
			return false; 
		}
		if ( (Status & PCIE_L221_B1D0_STATUS_WORD_INP) != 0x00 ) 
		{ 
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsInPosition OK End"));
			return true; 
		}
	}		
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsInPosition OK End"));
	*/
#endif//MOTION_OBJ_DISABLE
	return bINP;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::GetIsNLimit(int Axis)//該軸的是否碰觸到副極限
{
#ifndef MOTION_OBJ_DISABLE	
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return false; }

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsNLimit Start"));	
	
	U8 Data[4];
	const U16 IOType = 0x00;
	const U16 ODIndex = 0x60FD;
	const U16 ODSubIndex = 0x0;
	const U16 ByteSize = sizeof(Data);

	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	

	::memset(Data, 0x00, sizeof(Data));
	//_ECAT_Slave_PDO_Get_OD_Data(U16 CardNo, U16 NodeID, U16 SlotNo, U16 IOType, U16 ODIndex, U16 ODSubIndex, U16 ByteSize, U8 *Data);	//"Slave通用指令，對該站讀取某一OD碼的資料，該資料需有映射於PDO構成中, IOType 0:Rx 1:Tx"
	ErrorStatus = _ECAT_Slave_PDO_Get_OD_Data(CardNo, NodeID, SlotID, IOType, ODIndex, ODSubIndex, ByteSize, Data);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		m_ErrorString.Format(_T("Error, _ECAT_Slave_PDO_Get_OD_Data(CardNo, NodeID, SlotID, IOType, ODIndex, ODSubIndex, ByteSize, Data)"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsNLimit NG End"));
		return false;
	}
	
	const int AxisStatus = Data[0];
	int Filter = PCIE_L221_B1D0_PDO_NEL_LIMIT;	
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = MotionAxisPtr->GetSignPositive();	
		if ( FN_DISABLE == nSign )
		{	Filter = PCIE_L221_B1D0_PDO_PEL_LIMIT;	}
	}

	if ( (AxisStatus & Filter) != 0x00 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsNLimit OK-1 End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsNLimit OK-2 End"));
#endif//MOTION_OBJ_DISABLE
	return false;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::GetIsPLimit(int Axis)//該軸的是否碰觸到正極限	
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return false; }

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsPLimit Start"));
	
	U8 Data[4];
	const U16 IOType = 0x00;
	const U16 ODIndex = 0x60FD;
	const U16 ODSubIndex = 0x0;
	const U16 ByteSize = sizeof(Data);

	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	

	::memset(Data, 0x00, sizeof(Data));
	//_ECAT_Slave_PDO_Get_OD_Data(U16 CardNo, U16 NodeID, U16 SlotNo, U16 IOType, U16 ODIndex, U16 ODSubIndex, U16 ByteSize, U8 *Data);	//"Slave通用指令，對該站讀取某一OD碼的資料，該資料需有映射於PDO構成中, IOType 0:Rx 1:Tx"
	ErrorStatus = _ECAT_Slave_PDO_Get_OD_Data(CardNo, NodeID, SlotID, IOType, ODIndex, ODSubIndex, ByteSize, Data);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		m_ErrorString.Format(_T("Error, _ECAT_Slave_PDO_Get_OD_Data(CardNo, NodeID, SlotID, IOType, ODIndex, ODSubIndex, ByteSize, Data)"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsPLimit NG End"));
		return false;
	}

	const int AxisStatus = Data[0];
	int Filter = PCIE_L221_B1D0_PDO_PEL_LIMIT;	
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = GetAxisSignPositive(Axis);		
		if ( FN_DISABLE == nSign )
		{	Filter = PCIE_L221_B1D0_PDO_NEL_LIMIT;	}
	}

	if ( (AxisStatus & Filter) != 0x00 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsPLimit OK End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsPLimit OK End"));
#endif//MOTION_OBJ_DISABLE
	return false;	
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::GetIsORG(int Axis)//該軸是否在原點位置
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return false; }

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsORG Start"));
	
	U8 Data[4];
	const U16 IOType = 0x00;
	const U16 ODIndex = 0x60FD;
	const U16 ODSubIndex = 0x0;
	const U16 ByteSize = sizeof(Data);

	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	

	::memset(Data, 0x00, sizeof(Data));
	//_ECAT_Slave_PDO_Get_OD_Data(U16 CardNo, U16 NodeID, U16 SlotNo, U16 IOType, U16 ODIndex, U16 ODSubIndex, U16 ByteSize, U8 *Data);	//"Slave通用指令，對該站讀取某一OD碼的資料，該資料需有映射於PDO構成中, IOType 0:Rx 1:Tx"
	ErrorStatus = _ECAT_Slave_PDO_Get_OD_Data(CardNo, NodeID, SlotID, IOType, ODIndex, ODSubIndex, ByteSize, Data);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		m_ErrorString.Format(_T("Error, _ECAT_Slave_PDO_Get_OD_Data(CardNo, NodeID, SlotID, IOType, ODIndex, ODSubIndex, ByteSize, Data)"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsORG NG End"));
		return false;
	}


	int AxisStatus = Data[0];
	int Filter = PCIE_L221_B1D0_PDO_ORG;	
	if ( MotionAxisPtr->CheckGantryAxis() == true )
	{
		AxisStatus = Data[2];
		Filter = PCIE_L221_B1D0_PDO_ORG_GANTRY;	
	}

	if ( (AxisStatus & Filter) != 0x00 ) 
	{ 
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsORG OK End"));
		return true; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::GetIsORG OK End"));
#endif//MOTION_OBJ_DISABLE
	return false;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::StartFreeRun(int Axis, bool Dir)//Dir +為正方向, -為負方向
{
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	if ( CheckIsEnabled(MotionAxisPtr) == false ) { return false; }	
	if ( CheckFreeRunMoving(MotionAxisPtr) == true ) { return true; }

	CMotion_Basic::SaveMotionProcess(Axis, _T("StartFreeRun"), MSG_LEVEL_HIGH);	
	
	int nDir = 0;//0:Pos, 1:Neg
	const int Dir_Pos = 0;
	const int Dir_Neg = 1;	
	const int Direction=MotionAxisPtr->GetJogDirection();
	const int MaxV = (int)(GetAxisFreeRunVelocity(MotionAxisPtr));
	if ( true == Dir ) { nDir = Dir_Neg; }
	else { nDir = Dir_Pos; }
	if ( Direction > 0 ) 
	{	nDir = nDir;	}
	else
	{ 		
		switch ( nDir )
		{
		case Dir_Pos: nDir = Dir_Neg;	break;
		case Dir_Neg: nDir = Dir_Pos;	break;
		}
	}		
	MotionAxisPtr->SetFreeRunVel(MaxV);	
#ifndef MOTION_OBJ_DISABLE
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();
	const I32 StrVel = (U32)(MotionAxisPtr->GetJogStartVelocity());
	const F64 Tacc = (F64)(MotionAxisPtr->GetJogAccelerationTime());
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::StartFreeRun Start"));	
	//_ECAT_Slave_CSP_Start_V_Move(U16 CardNo, U16 NodeID, U16 SlotNo, U16 Dir, I32 StrVel, I32 ConstVel, F64 Tacc, U16 SCurve);//連續等速運動
	ErrorStatus = _ECAT_Slave_CSP_Start_V_Move(CardNo, NodeID, SlotID, nDir, (I32)StrVel, (I32)(MaxV), Tacc, 0);	
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _ECAT_Slave_CSP_Start_V_Move Fault"));
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_JOG_FUNC);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::StartFreeRun NG End"));
		return false; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::StartFreeRun OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::StopFreeRun(int Axis)//停止FreeRun
{	
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }

	CMotion_Basic::SaveMotionProcess(Axis, _T("StopFreeRun"), MSG_LEVEL_HIGH);
#ifndef MOTION_OBJ_DISABLE	
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();
	const F64 Tdec = (F64)(MotionAxisPtr->GetJogDecelerationTime());
	MotionAxisPtr->SetFreeRunVel(0.0);	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::StopFreeRun Start"));	
	
	//_ECAT_Slave_Motion_Sd_Stop(U16 CardNo, U16 NodeID, U16 SlotNo, F64 Tdec);//對當前MotionSlave下達減速停止指令	                
	ErrorStatus = _ECAT_Slave_Motion_Sd_Stop(CardNo, NodeID, SlotID, Tdec);	
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _ECAT_Slave_Motion_Sd_Stop Fault"));
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_JOG_FUNC);
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::StopFreeRun NG End"));
		return false; 
	}
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::StopFreeRun OK End"));	
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::SaveMotionParamInternal()//儲存運動內部參數
{
#ifndef MOTION_OBJ_DISABLE
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ExecSetIsWaitForInPosition(int Axis, bool Iswait)//設定是否等待定位停止
{
	if ( this->CheckInit() == false ) { return false; }
	
#ifndef MOTION_OBJ_DISABLE
	bool Wait = Iswait;
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }	
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }

	CMotion_Basic::SaveMotionProcess(Axis, _T("SetIsWaitForInPosition"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::SetIsWaitForInPosition Start"));	
	if ( WriteAxisParam_INP(MotionAxisPtr->GetMotionNodeRef(), Iswait) == false )
	{
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::SetIsWaitForInPosition NG End"));
		return false; 
	}
	const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
	for ( int i=0; i<SlaveNodeCount; i++ )
	{
		CMotionNode *SlaveNodePtr=MotionAxisPtr->GetSlaveNodePtr(i, false);
		if ( NULL == SlaveNodePtr ) { continue; }
		if ( WriteAxisParam_INP(*SlaveNodePtr, Iswait) == false )
		{
			CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::SetIsWaitForInPosition NG-2 End"));
			return false; 
		}
	}
	MotionAxisPtr->SetIsWaitForInp(Iswait);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::SetIsWaitForInPosition OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::SetIsWaitForInPosition(int Axis, bool Iswait)//設定是否等待定位停止
{
	if ( ExecSetIsWaitForInPosition(Axis, Iswait) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_INP_FUNC);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ConfigTriggerTable(bool IsReBuild)
{
#ifndef MOTION_OBJ_DISABLE
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::ConfigTriggerTable Start"));
	//SetMotionExceptionCode(AOI_EXCEPTION_MOTION_SET_FUNC);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::ConfigTriggerTable OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::ResetMotionDriver()//重新復歸運動的Driver
{	
#ifndef MOTION_OBJ_DISABLE
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::ResetMotionDriver Start"));
	DoFaultAck(AXIS_X);
	DoFaultAck(AXIS_Y);
	DoFaultAck(AXIS_Z);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::ResetMotionDriver OK End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
CString CMotion_PCIE_L221_B1D0::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)//載入多國語系
{
	CString NewLabelText;
	LPCTSTR Section=_T("MOTION_PCIE_L221_B1D0");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::FireSingleTrigger()//送出單一觸發訊號(包含燈源與相機)
{	
	return ReturnNoSupportFunc(_T("CMotion_PCIE_L221_B1D0::FireSingleTrigger"));
#ifndef MOTION_OBJ_DISABLE
	const int Axis = TRIGGER_AXIS;
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }

	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();
	const double TriggerBetweenTime = TRIGGER_CCD_BETWEEN_LED;//ms
	//Total Time, On Time, NTriggers
	//Total Time:166us, On Time:40us, NTriggers=1
	//Total Time: TRIGGER_CCD_BETWEEN_LED*2
	//On Time:40
	//NTrigger = 2

	int TotalTime = (int)(TriggerBetweenTime*1000);
	int OnTime = 40;
	int NTrigger = 1;
	//_ECAT_Compare_Set_Channel_One_Shot(U16 CardNo, U16 CompareChannel);
	//--------------------------------------------------------------------------------------------------------------//	
	this->m_ErrorString.Format(_T("Error, PCE-CMotion_PCIE_L221_B1D0 does not support Fire single trigger mode"));
	return false;
	//--------------------------------------------------------------------------------------------------------------//	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::SetSoftwareLimit(int Axis, double Min, double Max, bool Auto)//設定軟體極限
{	
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }

	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	
	CMotion_Basic::SaveMotionProcess(Axis, _T("SetSoftwareLimit"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::SetSoftwareLimit Start"));

	I32 MinI = (I32)(Min);
	I32 MaxI = (I32)(Max);		
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = GetAxisSignPositive(Axis);
		if ( FN_DISABLE == nSign )
		{
			MinI = (I32)MIN(-Min, -Max);
			MaxI = (I32)MAX(-Min, -Max);
		}
	}
	//設定軟體極限	
	//_ECAT_Slave_CSP_Set_Softlimit(U16 CardNo, U16 NodeID, U16 SlotNo, I32 NegaLimit, I32 PosiLimit, U16 Mode);
	ErrorStatus = _ECAT_Slave_CSP_Set_Softlimit(CardNo, NodeID, SlotID, (I32)MinI, (I32)MaxI, 2);
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _ECAT_Slave_CSP_Set_Softlimit()"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::SetSoftwareLimit NG End"));
		return false;
	}

	const bool GantryAxis = MotionAxisPtr->CheckGantryAxis();
	if ( true == GantryAxis )
	{
		size_t i=0;
		CMotionNode *SlaveNodePtr = NULL;
		const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
		for ( i=0; i<SlaveNodeCount; i++ )
		{
			SlaveNodePtr = MotionAxisPtr->GetSlaveNodePtr(i, false);
			if ( NULL == SlaveNodePtr ) { continue; }
			const U16 SlaveNodeID = SlaveNodePtr->GetNodeID();
			const U16 SlaveSlotID = SlaveNodePtr->GetSlotID();		
			ErrorStatus = _ECAT_Slave_CSP_Set_Softlimit(CardNo, SlaveNodeID, SlaveSlotID, (I32)MinI, (I32)MaxI, 2);
			CheckReturnOK(ErrorStatus);
		}
	}	

	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::SetSoftwareLimit End"));
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::EnableSoftwareLimit(int Axis)//啟用軟體極限
{	
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }

	int MinI=0, MaxI=0;
	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();	
	const double LimitMin = MotionAxisPtr->GetLimitMin();
	const double LimitMax = MotionAxisPtr->GetLimitMax();

	CMotion_Basic::SaveMotionProcess(Axis, _T("EnableSoftwareLimit"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::EnableSoftwareLimit Start"));

	MinI = LimitMin;
	MaxI = LimitMax;
	const bool bConvert=GetIsConvertSignPositive();
	if ( true == bConvert )
	{
		const int nSign = GetAxisSignPositive(Axis);
		if ( FN_DISABLE == nSign )
		{
			MinI = (I32)MIN(-LimitMin, -LimitMax);
			MaxI = (I32)MAX(-LimitMin, -LimitMax);
		}
	}

	//開啟軟體極限, //0->OFF, 1->Immediately stop, 2->Slow Down then stop, 3->Reserved
	//_ECAT_Slave_CSP_Set_Softlimit(U16 CardNo, U16 NodeID, U16 SlotNo, I32 NegaLimit, I32 PosiLimit, U16 Mode);
	ErrorStatus = _ECAT_Slave_CSP_Set_Softlimit(CardNo, NodeID, SlotID, (I32)MinI, (I32)MaxI, 2);//v1.01.03.234
	if ( CheckReturnOK(ErrorStatus) == false )
	{
		this->m_ErrorString.Format(_T("Error, _ECAT_Slave_CSP_Set_Softlimit()"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::EnableSoftwareLimit NG End"));
		return false;
	}

	const bool GantryAxis = MotionAxisPtr->CheckGantryAxis();
	if ( true == GantryAxis )
	{
		size_t i=0;
		CMotionNode *SlaveNodePtr = NULL;
		const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
		for ( i=0; i<SlaveNodeCount; i++ )
		{
			SlaveNodePtr = MotionAxisPtr->GetSlaveNodePtr(i, false);
			if ( NULL == SlaveNodePtr ) { continue; }
			const U16 SlaveNodeID = SlaveNodePtr->GetNodeID();
			const U16 SlaveSlotID = SlaveNodePtr->GetSlotID();		
			ErrorStatus = _ECAT_Slave_CSP_Set_Softlimit(CardNo, SlaveNodeID, SlaveSlotID, (I32)LimitMin, (I32)LimitMax, 2);
			CheckReturnOK(ErrorStatus);
		}
	}	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::EnableSoftwareLimit End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::DisableSoftwareLimit(int Axis)//關閉軟體極限
{	
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }	
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }

	U16 ErrorStatus = ERR_ECAT_NO_ERROR;
	const U16 Mode= 0;
	const int MinI=-100000;
	const int MaxI= 100000;	
	const U16 CardNo = MotionAxisPtr->GetCardID();
	const U16 NodeID = MotionAxisPtr->GetNodeID();
	const U16 SlotID = MotionAxisPtr->GetSlotID();		
	CMotion_Basic::SaveMotionProcess(Axis, _T("DisableSoftwareLimit"), MSG_LEVEL_HIGH);
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::DisableSoftwareLimit Start"));

	//關閉軟體極限
	//_ECAT_Slave_CSP_Set_Softlimit(U16 CardNo, U16 NodeID, U16 SlotNo, I32 NegaLimit, I32 PosiLimit, U16 Mode);
	ErrorStatus = _ECAT_Slave_CSP_Set_Softlimit(CardNo, NodeID, SlotID, MinI, MaxI, Mode);
	if ( CheckReturnOK(ErrorStatus) == false ) //0->Disable, 0->active at low
	{
		this->m_ErrorString.Format(_T("Error, _ECAT_Slave_CSP_Set_Softlimit()"));
		CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::DisableSoftwareLimit NG End"));
		return false;
	}
	
	const bool GantryAxis = MotionAxisPtr->CheckGantryAxis();
	if ( true == GantryAxis )
	{
		size_t i=0;
		CMotionNode *SlaveNodePtr = NULL;
		const size_t SlaveNodeCount=MotionAxisPtr->GetSlaveNodeCount();
		for ( i=0; i<SlaveNodeCount; i++ )
		{
			SlaveNodePtr = MotionAxisPtr->GetSlaveNodePtr(i, false);
			if ( NULL == SlaveNodePtr ) { continue; }
			const U16 SlaveNodeID = SlaveNodePtr->GetNodeID();
			const U16 SlaveSlotID = SlaveNodePtr->GetSlotID();		
			ErrorStatus = _ECAT_Slave_CSP_Set_Softlimit(CardNo, SlaveNodeID, SlaveSlotID, MinI, MaxI, Mode);
			CheckReturnOK(ErrorStatus);
		}
	}	
	CMotion_Basic::SaveMotionCurrentProcess(_T("CMotion_PCIE_L221_B1D0::DisableSoftwareLimit End"));
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::CorrectGantryOffset(int Axis)//修正龍門偏差
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	if ( MotionAxisPtr->CheckGantryAxis() == false ) { return true; }
	if ( CorrectMotionAxisGantryOffset(MotionAxisPtr) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_GANTRY_FUNC);
		return false; 
	}
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::CalibrateGantryOffset(int Axis)//校正龍門偏差
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	if ( MotionAxisPtr->CheckGantryAxis() == false ) { return true; }
	if ( CalibrateMotionAxisGantryOffset(MotionAxisPtr) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_GANTRY_FUNC);
		return false; 
	}
#endif//MOTION_OBJ_DISABLE	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::FetchGantryOffset(int Axis, double &Offset)//取得龍門的偏移值
{
#ifndef MOTION_OBJ_DISABLE
	if ( CheckInit() == false ) { return false; }
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }	
	if ( CheckAxisBypass(MotionAxisPtr) == true ) { return true; }
	if ( MotionAxisPtr->CheckGantryAxis() == false ) { return true; }
	if ( FetchMotionAxisGantryOffset(MotionAxisPtr) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_GANTRY_FUNC);
		return false; 
	}

	CMotionNode *MotionNodePtr=MotionAxisPtr->GetSlaveNodePtr(0, true);
	if ( NULL != MotionNodePtr )
	{	Offset = MotionNodePtr->GetGantryOffset(); }
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_PCIE_L221_B1D0::SetORG_Public(int Axis)
{
	return SetORG(Axis);
}
//----------------------------------------------------------------------------------//
#endif//MOTION_DERIVE_MODE