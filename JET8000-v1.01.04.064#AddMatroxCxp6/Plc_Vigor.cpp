// Plc_Vigor.cpp: implementation of the CPLC_Vigor class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Plc_Vigor.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//#define LANE_B_DISABLE_USE
//-------------------------------------------------------------------------------------//
#define PLC_X_BASE   0x0000
#define PLC_X_MAX    0x3f

#define PLC_Y_BASE   0x0040
#define PLC_Y_MAX    0x7f

#define PLC_M_BASE   0x0080
#define PLC_M_MAX    0x02ff

#define PLC_S_BASE   0x0300
#define PLC_S_MAX    0x037c

#define PLC_D_BASE   0x1C00//D0000
#define PLC_D_MAX    0x5BFF//D8191

#define STX          0x02
#define ACK          0x06
#define ETX          0x03
//---------------------------------------------------------------------------------//	
//雙軌的PLC錯誤代碼
/*
#define   JET_PLC_READY                        0//正常
#define   JET_PLC_EMS                        100//急停
#define   JET_PLC_CAP_OPEN                   101//上罩門
#define   JET_PLC_REAR_DOOR_OPEN             102//後門
#define   JET_PLC_AIR_EXCEPTION              103//氣壓不足
#define   JET_PLC_KEY_TURN_OFF               104//鑰匙關閉
#define   JET_PLC_PCB_IN_TIME_OUT            200//進板愈時
#define   JET_PLC_PCB_OUT_TIME_OUT           201//出板愈時
#define   JET_PLC_PCB_BACK_TIME_OUT          202//回板愈時
#define   JET_PLC_PCB_REIN_TIME_OUT          203//慢速進板愈時
#define   JET_PLC_PCB_OUT_WITH_IN_TIME_OUT   204//同出同進愈時
#define   JET_PLC_PCB_OTHER_IN_TIME_OUT      205//反向進板愈時
#define   JET_PLC_BARCODE_EXCEPTION          206//條碼異常
#define   JET_PLC_AUTO_BYPASS_NO_BOARDS      207//單軌Bypass的最大未動作時間
*/
//---------------------------------------------------------------------------------//	
//A軌異常動作監控時間	          D7100
//B軌異常動作監控時間	          D7101
//氣壓壓力低下監控時間	          D7102				
//輸送帶進板停止延遲時間	      D7103				
//夾板停止延遲時間	              D7104				
//A軌同進同出延遲減速有效	      D7106
//B軌同進同出延遲減速有效	      D7206
//A軌出板延遲出料有效	          D7107
//B軌出板延遲出料有效	          D7207
//A軌清軌道時間	                  D7112
//B軌清軌道時間	                  D7212
//間距馬達回原點過久監控	      D7020
//間距馬達運轉過久監控	          D7021
//---------------------------------------------------------------------------------//	

//雙軌的愈時時間設定
#define   JET_PLC_PCB_IN_OUT_TIMEOUT_LA           7100//PCB進出板逾時-10s-A軌//D7100
#define   JET_PLC_PCB_IN_OUT_TIMEOUT_LB           7101//PCB進出板愈時-10s-B軌//D7101
#define   JET_PLC_AIR_LOST_CHECK_TIME             7102//氣壓不足監控時間-0.5s//D7102
#define   JET_PLC_PCB_STOP_DELAY_TIME             7103//定位Sensor感應後持續運轉延遲時間-0.1sec//D7103
#define   JET_PLC_CLAMP_ON_OFF_DELAY_TIME         7104//維修模式時-汽缸夾鬆板愈時-1s//D7104
#define   JET_PLC_PCB_OUT_WITH_IN_DELAY_TIME_LA   7106//PCB出板帶進板延遲時間-0.1s-A軌//D7106
#define   JET_PLC_PCB_OUT_WITH_IN_DELAY_TIME_LB   7206//PCB出板帶進板延遲時間-0.1s-B軌//D7206
#define   JET_PLC_PCB_OUT_DELAY_TIME_LA           7107//PCB出板後延遲時間-0.1s-A軌//D7107
#define   JET_PLC_PCB_OUT_DELAY_TIME_LB           7207//PCB出板後延遲時間-0.1s-B軌//D7207
#define   JET_PLC_PCB_CLEAR_TIMEOUT_LA            7112//PCB板清除時間-時間單位:1=100ms//D7112
#define   JET_PLC_PCB_CLEAR_TIMEOUT_LB            7212//PCB板清除時間-時間單位:1=100ms//D7212
#define   JET_PLC_TURN_ON_LAST_SIGNAL_DELAY_TIME  7113//機台向上一站要板持續時間:1=100ms
#define   JET_PLC_CHECK_PCB_DOUBLE_IN_PITCH_TIME  7114//入料檢知頻寬設定(2個板間隔時間):1=100ms
#define   JET_PLC_CHECK_PCB_DOUBLE_IN_BOARD_TIME  7115//入料檢知時間設定(單板最長時間):1=100ms
//---------------------------------------------------------------------------------//	
#define   JET_PLC_ENABLE_FAN_ALARM                7050//PLC啟用風扇警報
#define   JET_PLC_ENABLE_OPEN_DOOR_STOP_POWER     7051//PLC啟用開門斷電
//---------------------------------------------------------------------------------//	
#define   JET_PLC_ENABLE_STOPPER_SENSOR_LA        7053//啟用擋板塊感應器-A軌
#define   JET_PLC_ENABLE_STOPPER_SENSOR_LB        7153//啟用擋板塊感應器-B軌
//---------------------------------------------------------------------------------//	
#define   JET_PLC_TOWER_LIGHT_MODE                7030//PLC針對特殊客戶模式

#define   JET_PLC_TOWER_LIGHT_NORMAL_LA      4101//PLC塔燈-常態A軌
#define   JET_PLC_TOWER_LIGHT_NORMAL_LB      4121//PLC塔燈-常態B軌
#define   JET_PLC_TOWER_LIGHT_PCB_IN         4102//PLC塔燈-進板中
#define   JET_PLC_TOWER_LIGHT_PCB_OUT        4103//PLC塔燈-出板中
#define   JET_PLC_TOWER_LIGHT_PCB_BACK       4104//PLC塔燈-回板中
#define   JET_PLC_TOWER_LIGHT_WAIT_FOR_LAST  4105//PLC塔燈-等待上一站
#define   JET_PLC_TOWER_LIGHT_WAIT_FOR_NEXT  4106//PLC塔燈-等待下一站
#define   JET_PLC_TOWER_LIGHT_BYPASS         4107//PLC塔燈-輸送帶模式
#define   JET_PLC_TOWER_LIGHT_INSPECTION     4108//PLC塔燈-檢測中
//---------------------------------------------------------------------------------//	
#define   JET_PLC_LANE_ADJUST_HOME_TIMEOUT    7020//自動調整軌道歸零逾時//D7020
#define   JET_PLC_LANE_ADJUST_MOVE_TIMEOUT    7021//自動調整軌道移動逾時//D7021

#define   JET_PLC_LANE_ADJUST_SKEW_PITCH_LA   7308//自動調整軌道A螺紋間距
#define   JET_PLC_LANE_ADJUST_SKEW_PITCH_LB   7328//自動調整軌道B螺紋間距

#define   JET_PLC_LANE_ADJUST_CURRENT_POS_LA  7300//自動調整軌道A目前位置
#define   JET_PLC_LANE_ADJUST_CURRENT_POS_LB  7320//自動調整軌道A目前位置

#define   JET_PLC_LANE_ADJUST_MOVE_TO_POS_LA  3112//自動調整軌道A移動位置
#define   JET_PLC_LANE_ADJUST_MOVE_TO_POS_LB  3212//自動調整軌道B移動位置

#define   JET_PLC_LANE_ADJUST_LIMIT_MAX_LA    7310//自動調整軌道A最大範圍
#define   JET_PLC_LANE_ADJUST_LIMIT_MAX_LB    7330//自動調整軌道B最大範圍

#define   JET_PLC_LANE_ADJUST_LIMIT_MIN_LA    7312//自動調整軌道A最大範圍
#define   JET_PLC_LANE_ADJUST_LIMIT_MIN_LB    7332//自動調整軌道B最大範圍

#define   JET_PLC_LANE_ADJUST_MOVE_SPEED_LA   7304//自動調整軌道A速度
#define   JET_PLC_LANE_ADJUST_JOG_SPEED_LA    7314//自動調整軌道A速度

#define   JET_PLC_LANE_ADJUST_MOVE_SPEED_LB   7324//自動調整軌道B速度
#define   JET_PLC_LANE_ADJUST_JOG_SPEED_LB    7334//自動調整軌道B速度

#define   JET_PLC_LANE_ADJUST_HOME_POS_LA     7306//自動調整軌道A速度
#define   JET_PLC_LANE_ADJUST_HOME_POS_LB     7326//自動調整軌道B速度
//---------------------------------------------------------------------------------//
#if PLC_OBJ_MODE == PLC_OBJ_VIGOR
CPLC_Vigor  PLC_Vigor;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CPLC_Vigor, CPLC_Basic)
//-------------------------------------------------------------------------------------//
CPLC_Vigor::CPLC_Vigor()
{
	CPLC_Vigor::InitialPLC();
}
//-------------------------------------------------------------------------------------//
CPLC_Vigor::~CPLC_Vigor()
{	
	this->PLC_Disconnect();
}
//-------------------------------------------------------------------------------------//
bool CPLC_Vigor::InitialPLC()
{
	this->m_StageNumber = 0;
	this->m_CommCheckCount = 1;

	this->PLC_BuildPlcNodeList();
	this->LoadExtraPlcNodeList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Vigor::PLC_ConnectTo(LPCTSTR Param)
{
#ifndef PLC_OBJ_DISABLE	
	if ( GetPLCIsConnected() == true )
	{	this->PLC_Disconnect(); }
	
	CString str;
	const int nPort = ::_ttoi(Param);
	const int nBaud = 19200;
	const int nByteSize = 7;
	const int nParity = 2;
	const int nStopBits = 0;	
	if ( m_RS232.Open(nPort, nBaud, nByteSize, nParity, nStopBits ) ==  FALSE )
	{
		str = _T("Error, PLC Open COM Port Faild!");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return m_PLCIsConnected;
	}

	const char EndOfChar = ETX;
	m_RS232.SetEventChar(EndOfChar);

	CPLC_Basic::SetPLCConnectParam(Param);
	m_PLCIsConnected = true;

	bool IsOK = true;
	char PLCNode[16]="";

	::strcpy(PLCNode, "M0");
	if ( ReadSingle(PLCNode,IsOK) == false ) 
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}	

	if ( this->ReadPLCVersion() == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}

	//復歸PLC完成訊號
	if ( this->ResetPLCFinishState() == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}

	//清除前後站訊號
	if ( this->WriteSignalToLast_LA(false) == false ||
		 this->WriteSignalToNext_LA(false) == false ||
		 this->WriteSignalToLast_LB(false) == false ||
		 this->WriteSignalToNext_LB(false) == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}
	
	//清除OK/NG訊號
	if ( this->WriteOKSignalToNext_LA(false) == false ||
		 this->WriteNGSignalToNext_LA(false) == false ||
		 this->WriteOKSignalToNext_LB(false) == false ||
		 this->WriteNGSignalToNext_LB(false) == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}

	if ( this->ReadPLCSaftyBypass() == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}
	
	if ( this->ReadPLCDualLaneMode() == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}

	this->WriteDualTowerLight(GetDualTowerLight());
	if ( this->ReadPLCDualTowerLight() == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}

	if ( this->ReadPLCStageAlarm() == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}

	if ( this->ReadPLCInspectionAlarm() == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}

	if ( this->PLC_WriteINIParameter() == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}
	
	const int TowerLightMode = GetTowerLightMode();
	if ( this->WriteTowerLightMode(TowerLightMode) == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}	

	if ( PLC_TOWER_LIGHT_USER_DEFINE == TowerLightMode )
	{
		int TowerLightState = 0;
		TowerLightState = GetTowerLightState_Stop();
		if ( this->WriteTowerLightState_Stop(TowerLightState) == false )
		{
			m_RS232.Close();
			m_PLCIsConnected = false;
			return false;
		}

		TowerLightState = GetTowerLightState_Inspection();
		if ( this->WriteTowerLightState_Inspection(TowerLightState) == false )
		{
			m_RS232.Close();
			m_PLCIsConnected = false;
			return false;
		}

		TowerLightState = GetTowerLightState_Bypass();
		if ( this->WriteTowerLightState_Bypass(TowerLightState) == false )
		{
			m_RS232.Close();
			m_PLCIsConnected = false;
			return false;
		}

		TowerLightState = GetTowerLightState_WaitLast();
		if ( this->WriteTowerLightState_WaitLast(TowerLightState) == false )
		{
			m_RS232.Close();
			m_PLCIsConnected = false;
			return false;
		}

		TowerLightState = GetTowerLightState_WaitNext();
		if ( this->WriteTowerLightState_WaitNext(TowerLightState) == false )
		{
			m_RS232.Close();
			m_PLCIsConnected = false;
			return false;
		}

		TowerLightState = GetTowerLightState_PCBIn();
		if ( this->WriteTowerLightState_PCBIn(TowerLightState) == false )
		{
			m_RS232.Close();
			m_PLCIsConnected = false;
			return false;
		}

		TowerLightState = GetTowerLightState_PCBOut();
		if ( this->WriteTowerLightState_PCBOut(TowerLightState) == false )
		{
			m_RS232.Close();
			m_PLCIsConnected = false;
			return false;
		}

		TowerLightState = GetTowerLightState_PCBBack();
		if ( this->WriteTowerLightState_PCBBack(TowerLightState) == false )
		{
			m_RS232.Close();
			m_PLCIsConnected = false;
			return false;
		}
	}		
	
	const int HomeTimeout = GetLaneAdjustHomeTimeout();//間距馬達-歸零逾時
	if ( this->WriteLaneAdjustHomeTimeout(HomeTimeout) == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}
	
	const int MoveTimeout = GetLaneAdjustMoveTimeout();//間距馬達-移動逾時
	if ( this->WriteLaneAdjustMoveTimeout(MoveTimeout) == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}

	const double SkewPitch_LA = GetLaneAdjustSkewPitch_LA();
	if ( this->WriteLaneAdjustSkewPitch_LA(SkewPitch_LA) == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}
	const double SkewPitch_LB = GetLaneAdjustSkewPitch_LB();
	if ( this->WriteLaneAdjustSkewPitch_LB(SkewPitch_LB) == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}
	if ( this->InitialLaneAdjust_LA() == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}	
	if ( this->InitialLaneAdjust_LB() == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}			
	if ( this->CreatePLCPollingThread() == false )
	{
		m_RS232.Close();
		m_PLCIsConnected = false;
		return false;
	}

	WritePCBBackOut_LA(false);
	WritePCBBackOut_LB(false);

	bool ConveyorMotorForward=true;
	bool ConveyorMotorSlowDown=false;
	m_ConveyorMotorForward_LA=ConveyorMotorForward;
	m_ConveyorMotorSlowDown_LA=ConveyorMotorSlowDown;
	m_ConveyorMotorForward_LB=ConveyorMotorForward;
	m_ConveyorMotorSlowDown_LB=ConveyorMotorSlowDown;;

	str = _T("PLC Connected");
	m_ErrorString = LoadMultiLanguageString(str, str);
	StartPLCPollingThread();
	return m_PLCIsConnected;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Vigor::PLC_Disconnect()
{
#ifndef PLC_OBJ_DISABLE
	this->DeletePLCPollingThread();
	this->LockPLC();	
	if ( this->m_RS232.IsOpened() == true )
	{	this->m_RS232.Close(); }
	
	m_LaneAdjust_Homed_LA = false;
	m_LaneAdjust_Homed_LB = false;
	m_LaneAdjust_CurrentPos_LA = 0.0;
	m_LaneAdjust_CurrentPos_LB = 0.0;
	this->m_PLCIsConnected = false;
	this->UnlockPLC();
#endif
	return true;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Vigor::ReadSingle(const char* unit, bool &stats)
{	
#ifndef PLC_OBJ_DISABLE	
	int counts = 8, point=0;
	bool Stats[8]={0};	
	if ( ReadData(unit, Stats, counts, point) == false )
	{
		CString str;
		CString strUnit = unit;
		str = _T("Error, PLC Read Node Fault");
		str = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		m_ErrorString.Format(_T("%s [%s]"), str, strUnit);
		return false;	
	}	
	stats = Stats[point];	
	
#endif	
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteSingle(const char *unit, bool stats)
{	
#ifndef PLC_OBJ_DISABLE
	if ( CheckPLCIsConnected() == false ) 
	{	return false;	}
	
	int i;
	CString str;
	char Command = 0x00;	
	int DataAdress=0, point=0;
	int temp=0;
	int ErrorNumber = 0;
	char lowbyte = 0, hightbyte = 0;		
	const int ResponseSize = 32;
	char SendBuffer[JET_PLC_BUFFER_SIZE]={0};
	char ReceiveBuffer[JET_PLC_BUFFER_SIZE]={0};
	const TPLCParameter &Param = GetPLCParameter();

	if ( GetUnitsInformation(unit, DataAdress, point) == false )
	{	return false;	}
	
	this->LockPLC();

	//STX
	m_SendBuffer[0] = STX;
	//Stage number
	m_SendBuffer[1] = (m_StageNumber&0Xf0)>>4;
	GetBytes(m_SendBuffer[1], hightbyte, lowbyte);
	m_SendBuffer[1] = hightbyte + lowbyte + 0x30;
		
	m_SendBuffer[2] = m_StageNumber&0X0f;
	GetBytes(m_SendBuffer[2], hightbyte, lowbyte);
	m_SendBuffer[2] = hightbyte + lowbyte + 0x30;
	//Command
	if ( stats )
	{	Command = 0x70; }
	else
	{	Command = 0x71; }

	m_SendBuffer[3] = (Command&0xf0)>>4;
	GetBytes(m_SendBuffer[3], hightbyte, lowbyte);
	m_SendBuffer[3] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[4] = (Command&0x0f);
	GetBytes(m_SendBuffer[4], hightbyte, lowbyte);
	m_SendBuffer[4] = hightbyte + lowbyte + 0x30;

	//Point Adress
	m_SendBuffer[5] = ((DataAdress>>9) & 0x000f );
	GetBytes(m_SendBuffer[5], hightbyte, lowbyte);
	m_SendBuffer[5] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[6] = ((DataAdress>>5) & 0x000f );
	GetBytes(m_SendBuffer[6], hightbyte, lowbyte);
	m_SendBuffer[6] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[7] = ((DataAdress>>1) & 0x000f );
	GetBytes(m_SendBuffer[7], hightbyte, lowbyte);
	m_SendBuffer[7] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[8] = (point&0x0007)+ ((DataAdress&0x0001) <<3 );
	GetBytes(m_SendBuffer[8], hightbyte, lowbyte);
	m_SendBuffer[8] = hightbyte + lowbyte + 0x30;

	//ETX
	m_SendBuffer[9] = ETX;
	temp = 0;
	for ( i=1; i<10; i++ )
	{	temp += m_SendBuffer[i]; }

	//SUM
	m_SendBuffer[10] = (temp&0x00f0)>>4;
	GetBytes(m_SendBuffer[10], hightbyte, lowbyte);
	m_SendBuffer[10] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[11] = temp&0x000f;
	GetBytes(m_SendBuffer[11], hightbyte, lowbyte);
	m_SendBuffer[11] = hightbyte + lowbyte + 0x30;
		
	m_SendBuffer[12] = '\0';
	//==========================================================================//		
	//Send Msg to COM	
	if ( m_RS232.SendData(m_SendBuffer, 12) != 12 )
	{
		str = _T("Error, PLC Send Datas to COM port Fault!");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		this->UnlockPLC();
		return false;
	}
		
	if ( Param.m_PLCCommDelayTime > 0 ) 
	{	::Sleep(Param.m_PLCCommDelayTime); }

	int GetResponseLength = 0;
	int TotalResponseLength = 0;
	const int RealFullDataLength = 10;
	::memset(m_ReceiveBuffer, 0x00, sizeof(char)*ResponseSize);	
	i = 0;
	const int MaxI = m_CommCheckCount;
	do 
	{
		GetResponseLength = m_RS232.ReadDataByNumber(&m_ReceiveBuffer[TotalResponseLength],ResponseSize-TotalResponseLength, RealFullDataLength);
		TotalResponseLength = TotalResponseLength+GetResponseLength;
		if ( TotalResponseLength == RealFullDataLength ) { break; }
		i ++ ;
		if ( i > MaxI )
		{
			str = _T("Error, PLC Get Response from COM Timeout!");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			this->UnlockPLC();
			return false;
		}
	} while ( true );

	::strcpy(SendBuffer, m_SendBuffer);
	::strcpy(ReceiveBuffer, m_ReceiveBuffer);

	this->UnlockPLC();
	//==========================================================================//
	if ( strlen(ReceiveBuffer) == 0 )
	{
		str = _T("Error, PLC No Response From COM");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;
	}
		
	//check content 
	if(ReceiveBuffer[0]!=ACK) 
	{	return false; }

	for(i=1;i<=4;i++)
	{
		if(ReceiveBuffer[i]!=SendBuffer[i]) 
		{	return false; }
	}

	ErrorNumber = (ReceiveBuffer[5]-0x30)*10 + (ReceiveBuffer[6]-0x30);
	if ( !CheckErrorNumber(ErrorNumber) ) 
	{	return false; }	
#endif
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadData(const char* StartUnit, bool Stats[], int counts, int &point)
{
#ifndef PLC_OBJ_DISABLE	
	if ( CheckPLCIsConnected() == false ) 
	{	return false; 	}

	if ( StartUnit == NULL ) 
	{	return false; }
	
	CString str;
	int i, j, index;
	char Command = 0x51;
	int DataAdress=0;
	int temp=0;
	int ErrorNumber = 0;
	int DataLength = (counts+7)/8;
	int NDataSends = 0;
	char lowbyte = 0, hightbyte = 0;
	char SendBuffer[JET_PLC_BUFFER_SIZE];
	char ReceiveBuffer[JET_PLC_BUFFER_SIZE];
	const TPLCParameter &Param = GetPLCParameter();

	if ( !GetUnitsInformation(StartUnit, DataAdress, point) )
	{	return false; }

	this->LockPLC();
	//STX
	m_SendBuffer[0] = STX;
	//Stage Number
	m_SendBuffer[1] = (m_StageNumber&0Xf0)>>4;
	GetBytes(m_SendBuffer[1], hightbyte, lowbyte);
	m_SendBuffer[1] = hightbyte + lowbyte + 0x30;
		
	m_SendBuffer[2] = m_StageNumber&0X0f;
	GetBytes(m_SendBuffer[2], hightbyte, lowbyte);
	m_SendBuffer[2] = hightbyte + lowbyte + 0x30;
	//Command
	m_SendBuffer[3] = (Command&0xf0)>>4;
	GetBytes(m_SendBuffer[3], hightbyte, lowbyte);
	m_SendBuffer[3] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[4] = (Command&0x0f);
	GetBytes(m_SendBuffer[4], hightbyte, lowbyte);
	m_SendBuffer[4] = hightbyte + lowbyte + 0x30;

	//Start Data Adress
	m_SendBuffer[5] = ((DataAdress>>12) & 0x0f);
	GetBytes(m_SendBuffer[5], hightbyte, lowbyte);
	m_SendBuffer[5] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[6] = ((DataAdress>>8) & 0x0f);
	GetBytes(m_SendBuffer[6], hightbyte, lowbyte);
	m_SendBuffer[6] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[7] = ((DataAdress>>4) & 0x0f);
	GetBytes(m_SendBuffer[7], hightbyte, lowbyte);
	m_SendBuffer[7] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[8] = ( DataAdress & 0x0f);
	GetBytes(m_SendBuffer[8], hightbyte, lowbyte);
	m_SendBuffer[8] = hightbyte + lowbyte + 0x30;	
		
	//Data Length
	m_SendBuffer[9] = ((DataLength&0x00f0)>>4);
	GetBytes(m_SendBuffer[9], hightbyte, lowbyte);
	m_SendBuffer[9] = hightbyte + lowbyte + 0x30;
		
	m_SendBuffer[10] = (DataLength & 0x0000f);
	GetBytes(m_SendBuffer[10], hightbyte, lowbyte);
	m_SendBuffer[10] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[11] = ETX;

	temp = 0;
	for ( i=1; i<12; i++ )
	{	temp += m_SendBuffer[i]; }
	//SUM
	m_SendBuffer[12] = (temp&0x00f0)>>4;
	GetBytes(m_SendBuffer[12], hightbyte, lowbyte);
	m_SendBuffer[12] = hightbyte + lowbyte + 0x30;		

	m_SendBuffer[13] = (temp&0x000f);
	GetBytes(m_SendBuffer[13], hightbyte, lowbyte);
	m_SendBuffer[13] = hightbyte + lowbyte + 0x30;
	m_SendBuffer[14] ='\0';
	//==========================================================================//		
	//Send Msg to COM
	NDataSends = m_RS232.SendData(m_SendBuffer, 14);
	if ( NDataSends != 14 )
	{
		str = _T("Error, PLC Send Datas to COM port Fault!");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		this->UnlockPLC();
		return false;
	}

	if ( Param.m_PLCCommDelayTime > 0 ) 
	{	::Sleep(Param.m_PLCCommDelayTime); }

	const int ResponseSize = 10 + DataLength*2;
	const int RealFullDataLength = 10 + DataLength*2;
	//Get Response
	//JET_PLC_BUFFER_SIZE
	if ( (ResponseSize+10) >=JET_PLC_BUFFER_SIZE )
	{
		str = _T("Error, PLC Receive Buffer too small !");		
		str = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		m_ErrorString.Format(_T("%s (%d/%d)"), str, ResponseSize, JET_PLC_BUFFER_SIZE);
		this->UnlockPLC();
		return false;
	}
	::memset(m_ReceiveBuffer, 0x00, sizeof(char)*(ResponseSize+10));
	int GetResponseLength = 0;	
	const int MaxI = m_CommCheckCount;
	int TotalGetResponseLength = 0;
	i = 0;
	do 
	{		
		GetResponseLength = m_RS232.ReadDataByNumber(&(m_ReceiveBuffer[TotalGetResponseLength]), ResponseSize-TotalGetResponseLength, RealFullDataLength);
		TotalGetResponseLength = TotalGetResponseLength+GetResponseLength;
		if ( TotalGetResponseLength >= RealFullDataLength ) { break; }
		i ++ ;
		if ( i > MaxI ) 
		{
			str = _T("Error, PLC Get Response from COM port wait for Timeout!");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			this->UnlockPLC();
			return false;
		}
		::Sleep(0);		
	} while ( true );		
		
	::strcpy(SendBuffer, m_SendBuffer);
	::strcpy(ReceiveBuffer, m_ReceiveBuffer);

	this->UnlockPLC();
	//==========================================================================//		
	if ( strlen(ReceiveBuffer) <= 6 )
	{
		str = _T("Error, PLC No Response From COM port");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;
	}

	if(ReceiveBuffer[0]!=ACK) 
	{ 
		str = _T("Error, PLC Response From COM port Data Exception");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;
	}

	for(i=1;i<=4;i++)
	{
		if(ReceiveBuffer[i]!=SendBuffer[i]) 
		{   
			str = _T("Error, PLC Response From COM port Data Exception");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			return false;
		}
	}

	ErrorNumber = (ReceiveBuffer[5]-0x30)*10 + (ReceiveBuffer[6]-0x30);
	if ( !CheckErrorNumber(ErrorNumber) )
	{	return false;	}
		
	char Data=0x00;
	index = 0;
	for ( i=7; i<7+(DataLength*2); i+=2 )
	{
		hightbyte = ReceiveBuffer[i];
		lowbyte = ReceiveBuffer[i+1];
		this->MakeWord(hightbyte, lowbyte, Data);
		for ( j=0; j<8; j++)
		{
			if ( index >= counts )
			{	break; }
				
			Stats[index] = (Data>>j)&0x01;
			index ++;
		}
	}
#endif
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteData(const char* StartUnit, bool Stats[], int counts)
{
#ifndef PLC_OBJ_DISABLE	
	if ( CheckPLCIsConnected() == false ) 
	{	return false;	}
	
	CString str;
	int i, j, index;
	char Command = 0x61;
	int DataAdress=0, point=0;
	int temp=0;
	int ErrorNumber = 0;
	int DataLength = (counts+7)/8;
	char lowbyte = 0, hightbyte = 0;		
	char SendBuffer[JET_PLC_BUFFER_SIZE];
	char ReceiveBuffer[JET_PLC_BUFFER_SIZE];
	const TPLCParameter &Param = GetPLCParameter();

	if ( !GetUnitsInformation(StartUnit, DataAdress, point) )
	{	return false; }

	const int SendLen = 14 + DataLength*2;
	if ( SendLen >= JET_PLC_BUFFER_SIZE )
	{
		str = _T("Error, PLC Send Buffer too small !");		
		str = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		m_ErrorString.Format(_T("%s (%d/%d)"), str, SendLen, JET_PLC_BUFFER_SIZE);
		return false;
	}

	this->LockPLC();

	memset(m_SendBuffer, ' ', 14 + DataLength*2);
	//STX
	m_SendBuffer[0] = STX;
	//Stage Number
	m_SendBuffer[1] = (m_StageNumber&0Xf0)>>4;
	GetBytes(m_SendBuffer[1], hightbyte, lowbyte);
	m_SendBuffer[1] = hightbyte + lowbyte + 0x30;
		
	m_SendBuffer[2] = m_StageNumber&0X0f;
	GetBytes(m_SendBuffer[2], hightbyte, lowbyte);
	m_SendBuffer[2] = hightbyte + lowbyte + 0x30;

	//Command
	m_SendBuffer[3] = (Command&0xf0)>>4;
	GetBytes(m_SendBuffer[3], hightbyte, lowbyte);
	m_SendBuffer[3] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[4] = (Command&0x0f);
	GetBytes(m_SendBuffer[4], hightbyte, lowbyte);
	m_SendBuffer[4] = hightbyte + lowbyte + 0x30;

	//Start Data Adress
	m_SendBuffer[5] = ((DataAdress>>12) & 0x0f);
	GetBytes(m_SendBuffer[5], hightbyte, lowbyte);
	m_SendBuffer[5] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[6] = ((DataAdress>>8) & 0x0f);
	GetBytes(m_SendBuffer[6], hightbyte, lowbyte);
	m_SendBuffer[6] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[7] = ((DataAdress>>4) & 0x0f);
	GetBytes(m_SendBuffer[7], hightbyte, lowbyte);
	m_SendBuffer[7] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[8] = ( DataAdress & 0x0f);
	GetBytes(m_SendBuffer[8], hightbyte, lowbyte);
	m_SendBuffer[8] = hightbyte + lowbyte + 0x30;
		
	//Data Length
	m_SendBuffer[9] = ((DataLength&0x00f0)>>4);
	GetBytes(m_SendBuffer[9], hightbyte, lowbyte);
	m_SendBuffer[9] = hightbyte + lowbyte + 0x30;
		
	m_SendBuffer[10] = (DataLength & 0x0000f);
	GetBytes(m_SendBuffer[10], hightbyte, lowbyte);
	m_SendBuffer[10] = hightbyte + lowbyte + 0x30;

	j=0; 
	for ( index=11; index<11+DataLength*2; index+=2 )
	{
		m_SendBuffer[index] = m_SendBuffer[index+1] = 0x00;
		for ( i=0; i<8; i++ )
		{
			if ( j>=counts )
			{	break; }

			if ( i<4 )
			{	m_SendBuffer[index+1] += Stats[i]<<i; }
			else
			{	m_SendBuffer[index] += Stats[i]<<(i-4); }

			j++;
		}

		GetBytes(m_SendBuffer[index], hightbyte, lowbyte);
		m_SendBuffer[index] = hightbyte + lowbyte + 0x30;
			
		GetBytes(m_SendBuffer[index+1], hightbyte, lowbyte);
		m_SendBuffer[index+1] = hightbyte + lowbyte + 0x30;
	}

	m_SendBuffer[index] = ETX;

	temp = 0;
	for ( i=1; i<index+1; i++ )
	{	temp += m_SendBuffer[i]; }

	//SUM
	m_SendBuffer[index+1] = (temp&0x00f0)>>4;
	GetBytes(m_SendBuffer[index+1], hightbyte, lowbyte);
	m_SendBuffer[index+1] = hightbyte + lowbyte + 0x30;		

	m_SendBuffer[index+2] = (temp&0x000f);
	GetBytes(m_SendBuffer[index+2], hightbyte, lowbyte);
	m_SendBuffer[index+2] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[index+3] = '\0';
	//=====================================================================//		
	//Send Msg to COM	
	if ( m_RS232.SendData(m_SendBuffer, 14) != 14 )
	{
		str = _T("PLC Send Datas to COM port Fault!");			
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		this->UnlockPLC();
		return false;
	}

	if ( Param.m_PLCCommDelayTime > 0 ) 
	{	::Sleep(Param.m_PLCCommDelayTime); }

	//Get Response
	::memset(m_ReceiveBuffer, 0x00, sizeof(m_ReceiveBuffer[0])*24);
		
	int GetResponseLength = 0;	
	const int MaxI = m_CommCheckCount;
	const int ResponseSize = 10;
	const int RealFullDataLength = 10;
	int TotalGetResponseLength = 0;
	i = 0;
	do 
	{		
		GetResponseLength = m_RS232.ReadDataByNumber(&(m_ReceiveBuffer[TotalGetResponseLength]), ResponseSize-TotalGetResponseLength, RealFullDataLength);
		TotalGetResponseLength = TotalGetResponseLength+GetResponseLength;
		if ( TotalGetResponseLength >= RealFullDataLength ) { break; }
		i ++ ;
		if ( i > MaxI ) 
		{
			str = _T("PLC Get Response from COM port wait for Timeout!");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			this->UnlockPLC();
			return false;
		}
		::Sleep(0);			
	} while ( true );				


	::strcpy(SendBuffer, m_SendBuffer);
	::strcpy(ReceiveBuffer, m_ReceiveBuffer);

	this->UnlockPLC();
	//=====================================================================//		
	if ( strlen(ReceiveBuffer) == 0 )
	{
		str = _T("Error, PLC No Response From COM");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;
	}

	if (ReceiveBuffer[0]!=ACK)  
	{ return false; }
	
	for(i=1;i<=4;i++)
	{
		if (ReceiveBuffer[i]!=SendBuffer[i])	
		{ return false; }
	}

	//Get Response
	ErrorNumber = (ReceiveBuffer[5]-0x30)*10 + (ReceiveBuffer[6]-0x30);
	if ( !CheckErrorNumber(ErrorNumber) )
	{ return false; }		
#endif
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::GetConveyorMotorForward_LA() const
{
	return m_ConveyorMotorForward_LA;
}
//-----------------------------------------------------------------------//
void CPLC_Vigor::SetConveyorMotorForward_LA(bool val)
{
	m_ConveyorMotorForward_LA = val;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::GetConveyorMotorSlowDown_LA() const
{
	return m_ConveyorMotorSlowDown_LA;
}
//-----------------------------------------------------------------------//
void CPLC_Vigor::SetConveyorMotorSlowDown_LA(bool val)
{
	m_ConveyorMotorSlowDown_LA = val;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::GetConveyorMotorForward_LB() const
{
	return m_ConveyorMotorForward_LB;
}
//-----------------------------------------------------------------------//
void CPLC_Vigor::SetConveyorMotorForward_LB(bool val)
{
	m_ConveyorMotorForward_LB = val;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::GetConveyorMotorSlowDown_LB() const
{
	return m_ConveyorMotorSlowDown_LB;
}
//-----------------------------------------------------------------------//
void CPLC_Vigor::SetConveyorMotorSlowDown_LB(bool val)
{
	m_ConveyorMotorSlowDown_LB = val;
}
//-----------------------------------------------------------------------//
void CPLC_Vigor::MakeWord(char hightbyte, char lowbyte, char &word)
{
	char tempbyte[4] = {0x00, 0x00, 0x00, 0x00};
	tempbyte[3] = ((hightbyte-0x30)&0xf0)>>4;
	tempbyte[2] = ((hightbyte-0x30)&0x0f);
	if ( tempbyte[3] )
	{	tempbyte[2] -= 0x01; }
		
	tempbyte[1] = ((lowbyte-0x30)&0xf0)>>4;
	tempbyte[0] = ((lowbyte-0x30)&0x0f);
	if ( tempbyte[1] )
	{	tempbyte[0] -= 0x01; }

	word = (tempbyte[3]*10 + tempbyte[2]) << 4;
	word += (tempbyte[1]*10 + tempbyte[0]);
}
//--------------------------------------------------------------------//
void CPLC_Vigor::GetBytes(char Value, char &HightByte, char &LowByte)
{
	HightByte = ((Value&0x0f)/10)<<4;
	LowByte = (Value&0x0f)%10;
	if ( HightByte )
	{	LowByte += 0x01; }
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::GetUnitsInformation(const char* unit, int &DataAdress, int &point)
{
	if ( unit == NULL ) { return false; }
	CString str;
	int i, j, temp;
	char UnitType=NULL;
	char ch;
	char strNumber[32]="";
	int UnitNumber = 0;
	int UnitBase=0, AddressMax=0;
	int length = (int)(strlen(unit));

	UnitType = unit[0];
	for ( i=1; i<length; i++ )
	{
		if ( !isdigit(unit[i]) )
		{
			str = _T("Error, PLC Input wrong unit number");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);				
			return false;
		}
		strNumber[i-1] = unit[i];
	}
	strNumber[i-1] = '\0';
	UnitNumber = atoi(strNumber);
	switch( UnitType )
	{
	case 'x':
	case 'X':
		UnitBase = PLC_X_BASE;
		AddressMax = PLC_X_MAX;
		for ( i=0; i<length-2; i++ )
		{
			ch = unit[i];
			if ( ch > '7' )
			{
				str = _T("Error, PLC Input wrong unit number, need less than 8");
				m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
				return false;
			}
			temp = 1;
			for ( j=i+1; j<(length-2); j++)
				temp *= 8;
			DataAdress += atoi(&ch)*temp;
		}
		ch = unit[length-2];
		if ( ch > '7' )
		{
			str = _T("Error, PLC Input wrong unit number, need less than 8");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			return false;
		}
		DataAdress += UnitBase;
		point = atoi(&ch);
		break;
	case 'y':
	case 'Y':
		UnitBase = PLC_Y_BASE;
		AddressMax = PLC_Y_MAX;
		for ( i=0; i<length-2; i++ )
		{
			ch = unit[i];
			if ( ch > '7' )
			{
				str = _T("Error, PLC Input wrong unit number, need less than 8");
				m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
				return false;
			}
			temp = 1;
			for ( j=i+1; j<(length-2); j++)
				temp *= 8;
			DataAdress += atoi(&ch)*temp;
		}
		ch = unit[length-2];
		if ( ch > '7' )
		{
			str = _T("Error, PLC Input wrong unit number, need less than 8");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			return false;
		}
		DataAdress += UnitBase;
		point = atoi(&ch);
		break;
	case 'm':
	case 'M':
		UnitBase = PLC_M_BASE;
		AddressMax = PLC_M_MAX;
		DataAdress = (UnitNumber+7)/8;
		DataAdress = (UnitNumber&0xfff8)>>3;
		DataAdress += UnitBase;
		point = UnitNumber&0x0007;
		break;
	case 's':
	case 'S':
		UnitBase = PLC_S_BASE;
		AddressMax = PLC_S_MAX;
		DataAdress = (UnitNumber&0xfff8)>>3;
		DataAdress += UnitBase;
		point = UnitNumber&0x0007;
		break;	
	default:
		str = _T("Error, PLC Input wrong unit");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;
		break;
	}
	if ( DataAdress > AddressMax )
	{	
		str = _T("Error, PLC Unit Address out of Max Address");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;
	}
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckErrorNumber(int ErrorNumber)
{
	CString str;
	switch ( ErrorNumber )
	{
	case 0:
		break;
	case 10:		
		str = _T("Error, PLC ASCII transform Error!");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;
		break;
	case 11:		
		str = _T("Error, PLC SUM Check Error!");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;
		break;
	case 12:		
		str = _T("Error, PLC NO Command you sent!");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;
		break;
	case 14:
		str = _T("Error, PLC Communication Error!");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;
		break;
	case 28:
		str = _T("Error, PLC Data Adress out of range!");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;
		break;
	default:
		str = _T("Error, PLC Response unidentified Error Number!");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;
		break;
	}
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadPLCVersion()
{
#ifdef PLC_OBJ_DISABLE
	m_PLCVersion = _T("DISABLE");
#else
	char VersionS[5]="";	
	const int VersionDCode = 0;
	if ( this->ReadDCode(VersionDCode,VersionS) == false )
	{	return false;	}	
	
	int Value = this->TransferDcodeToINT(VersionS);	//16進位碼

	m_PLCVersion = VersionS;	
	m_PLCVersionI = Value;
	m_PLCVersion.Format(_T("PLC-v%d"), Value);

	//this->AdjustConveyer_Manual();
	//this->AdjustConveyer_Initial2();
	//this->AdjustConveyer_Initial4();
#endif
	return true;
}
//----------------------------------------------------------------------------//
bool CPLC_Vigor::ReadPLCSaftyBypass()
{	
#ifndef PLC_OBJ_DISABLE
	int States=FN_DISABLE;
	if( this->PLC_ReadNode("M300", States) == false ) 
	{	return false;	}
	if ( FN_DISABLE == States )
	{	m_SaftyBypass = false; }
	else
	{	m_SaftyBypass = true; }
#endif//PLC_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------//
bool CPLC_Vigor::ReadPLCDualLaneMode()
{
#ifndef PLC_OBJ_DISABLE
	int States=FN_DISABLE;
	if( this->PLC_ReadNode("M322", States) == false ) 
	{	return false;	}
	if ( FN_DISABLE == States )
	{	m_DualLaneMode = false; }
	else
	{	m_DualLaneMode = true; }
#endif//PLC_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------//
bool CPLC_Vigor::ReadPLCDualTowerLight()
{
#ifndef PLC_OBJ_DISABLE	
	int States=FN_DISABLE;
	if( this->PLC_ReadNode("M325", States) == false ) 
	{	return false;	}
	bool DualTowerLight=false;
	if ( FN_DISABLE == States )
	{	DualTowerLight = false; }
	else
	{	DualTowerLight = true; }
	SetDualTowerLight(DualTowerLight);
#endif//PLC_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------//
bool CPLC_Vigor::ReadPLCStageAlarm()
{
#ifndef PLC_OBJ_DISABLE	
	int States=FN_DISABLE;

	//Lane A
	States=FN_DISABLE;
	if( this->PLC_ReadNode("M344", States) == false ) 
	{	return false;	}
	if ( FN_DISABLE == States )
	{	m_StageAlarm_LA = false; }
	else
	{	m_StageAlarm_LA = true; }

	//Lane B
	States=FN_DISABLE;
	if( this->PLC_ReadNode("M444", States) == false ) 
	{	return false;	}
	if ( FN_DISABLE == States )
	{	m_StageAlarm_LB = false; }
	else
	{	m_StageAlarm_LB = true; }
#endif//PLC_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------//
bool CPLC_Vigor::ReadPLCInspectionAlarm()
{	
#ifndef PLC_OBJ_DISABLE	
	int States=FN_DISABLE;
	//Lane A
	States=FN_DISABLE;
	if( this->PLC_ReadNode("M342", States) == false ) 
	{	return false;	}
	if ( FN_DISABLE == States )
	{	m_InspectionAlarm_LA = false; }
	else
	{	m_InspectionAlarm_LA = true; }

	//Lane B
	States=FN_DISABLE;
	if( this->PLC_ReadNode("M442", States) == false ) 
	{	return false;	}
	if ( FN_DISABLE == States )
	{	m_InspectionAlarm_LB = false; }
	else
	{	m_InspectionAlarm_LB = true; }
#endif//PLC_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------//
bool CPLC_Vigor::ResetPLCFinishState()//復歸PLC完成訊號
{
#ifndef PLC_OBJ_DISABLE	
	int States=FN_DISABLE;
	//A軌道
	//進板完成
	if( this->PLC_WriteNode("M105", States) == false ) 
	{	return false;	}	
	//出板完成
	if( this->PLC_WriteNode("M106", States) == false ) 
	{	return false;	}	
	//回板完成
	if( this->PLC_WriteNode("M107", States) == false ) 
	{	return false;	}	
	//慢速回進板完成
	if( this->PLC_WriteNode("M108", States) == false ) 
	{	return false;	}	
	//出板帶進板完成
	if( this->PLC_WriteNode("M109", States) == false ) 
	{	return false;	}	
	//機台內出板完成
	if( this->PLC_WriteNode("M110", States) == false ) 
	{	return false;	}	
	//自動進出板完成
	if( this->PLC_WriteNode("M111", States) == false ) 
	{	return false;	}		
	//自動退進板完成
	if( this->PLC_WriteNode("M113", States) == false ) 
	{	return false;	}		
	//第2段進板完成
	if( this->PLC_WriteNode("M112", States) == false ) 
	{	return false;	}	
	//第3段進板完成
	if( this->PLC_WriteNode("M150", States) == false ) 
	{	return false;	}	
	//自動進出板-出板完成-1
	if( this->PLC_WriteNode("M151", States) == false ) 
	{	return false;	}	
	//自動進出板-出板完成-2
	if( this->PLC_WriteNode("M152", States) == false ) 
	{	return false;	}	
	//清板完成
	if( this->PLC_WriteNode("M153", States) == false ) 
	{	return false;	}	

	//B軌道
	//進板完成
	if( this->PLC_WriteNode("M205", States) == false ) 
	{	return false;	}	
	//出板完成
	if( this->PLC_WriteNode("M206", States) == false ) 
	{	return false;	}	
	//回板完成
	if( this->PLC_WriteNode("M207", States) == false ) 
	{	return false;	}	
	//慢速回進板完成
	if( this->PLC_WriteNode("M208", States) == false ) 
	{	return false;	}	
	//出板帶進板完成
	if( this->PLC_WriteNode("M209", States) == false ) 
	{	return false;	}	
	//機台內出板完成
	if( this->PLC_WriteNode("M210", States) == false ) 
	{	return false;	}	
	//自動進出板完成
	if( this->PLC_WriteNode("M211", States) == false ) 
	{	return false;	}	
	//自動退進板完成
	if( this->PLC_WriteNode("M213", States) == false ) 
	{	return false;	}
	//第2段進板完成
	if( this->PLC_WriteNode("M212", States) == false ) 
	{	return false;	}	
	//第3段進板完成
	if( this->PLC_WriteNode("M250", States) == false ) 
	{	return false;	}	
	//自動進出板-出板完成-1
	if( this->PLC_WriteNode("M251", States) == false ) 
	{	return false;	}	
	//自動進出板-出板完成-2
	if( this->PLC_WriteNode("M252", States) == false ) 
	{	return false;	}	
	//清板完成
	if( this->PLC_WriteNode("M253", States) == false ) 
	{	return false;	}	
#endif//PLC_OBJ_DISABLE	
	return true;
}
//----------------------------------------------------------------------------//
bool CPLC_Vigor::ReadDCode(int DN, char data[])
{
#ifndef PLC_OBJ_DISABLE	
	if ( CPLC_Vigor::DoReadDCode(DN, data) == false )
	{
		CString str;		
		str = _T("Error, Read DCode Fault");
		str = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		m_ErrorString.Format(_T("%s [D:%d]"), str, DN);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_READ_NODE);
		return false;
	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------//
bool CPLC_Vigor::DoReadDCode(int DN, char data[])//Read D Conde, in 16-bit mode	
{
#ifndef PLC_OBJ_DISABLE	
	if ( CheckPLCIsConnected() == false ) 
	{	return false;	}

	CString str;
	if ( DN < 0 || DN >8191 ) 
	{	
		str = _T("Error, PLC D code number out of size ( 0 ~ 8191 )"); 
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;  
	}
		
	char Command = 0x51;
	int i=0, j=0, index=0;
	int DataAdress=(int)(DN*2+PLC_D_BASE);
	int temp=0;
	int ErrorNumber = 0;
	int DataLength = 2;
	char lowbyte = 0, hightbyte = 0;
	char SendBuffer[JET_PLC_BUFFER_SIZE];
	char ReceiveBuffer[JET_PLC_BUFFER_SIZE];
	const TPLCParameter &Param = GetPLCParameter();

	this->LockPLC();
		
	//STX
	m_SendBuffer[0] = STX;
	//Stage Number
	m_SendBuffer[1] = (m_StageNumber&0Xf0)>>4;
	GetBytes(m_SendBuffer[1], hightbyte, lowbyte);
	m_SendBuffer[1] = hightbyte + lowbyte + 0x30;
		
	m_SendBuffer[2] = m_StageNumber&0X0f;
	GetBytes(m_SendBuffer[2], hightbyte, lowbyte);
	m_SendBuffer[2] = hightbyte + lowbyte + 0x30;
	//Command
	m_SendBuffer[3] = (Command&0xf0)>>4;
	GetBytes(m_SendBuffer[3], hightbyte, lowbyte);
	m_SendBuffer[3] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[4] = (Command&0x0f);
	GetBytes(m_SendBuffer[4], hightbyte, lowbyte);
	m_SendBuffer[4] = hightbyte + lowbyte + 0x30;

	//Start Data Adress
	m_SendBuffer[5] = ((DataAdress>>12) & 0x0f);
	GetBytes(m_SendBuffer[5], hightbyte, lowbyte);
	m_SendBuffer[5] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[6] = ((DataAdress>>8) & 0x0f);
	GetBytes(m_SendBuffer[6], hightbyte, lowbyte);
	m_SendBuffer[6] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[7] = ((DataAdress>>4) & 0x0f);
	GetBytes(m_SendBuffer[7], hightbyte, lowbyte);
	m_SendBuffer[7] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[8] = ( DataAdress & 0x0f);
	GetBytes(m_SendBuffer[8], hightbyte, lowbyte);
	m_SendBuffer[8] = hightbyte + lowbyte + 0x30;	
		
	//Data Length
	m_SendBuffer[9] = ((DataLength&0x00f0)>>4);
	GetBytes(m_SendBuffer[9], hightbyte, lowbyte);
	m_SendBuffer[9] = hightbyte + lowbyte + 0x30;
		
	m_SendBuffer[10] = (DataLength & 0x0000f);
	GetBytes(m_SendBuffer[10], hightbyte, lowbyte);
	m_SendBuffer[10] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[11] = ETX;

	temp = 0;
	for ( i=1; i<12; i++ )
	{	temp += m_SendBuffer[i];	}

	//SUM
	m_SendBuffer[12] = (temp&0x00f0)>>4;
	GetBytes(m_SendBuffer[12], hightbyte, lowbyte);
	m_SendBuffer[12] = hightbyte + lowbyte + 0x30;		

	m_SendBuffer[13] = (temp&0x000f);
	GetBytes(m_SendBuffer[13], hightbyte, lowbyte);
	m_SendBuffer[13] = hightbyte + lowbyte + 0x30;
	m_SendBuffer[14] ='\0';
	//==================================================================================//		

	//Send Msg to COM
	if ( m_RS232.SendData(m_SendBuffer, 14) != 14 )
	{
		str = _T("Error, PLC Send Datas to COM port Fault!");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		this->UnlockPLC();
		return false;
	}

	if ( Param.m_PLCCommDelayTime > 0 ) 
	{	::Sleep(Param.m_PLCCommDelayTime); }

	//Get Response		
	const int ResponseSize = 10 + DataLength*2;
	const int RealFullDataLength = 10 + DataLength*2;		
	if ( (ResponseSize+10) >= JET_PLC_BUFFER_SIZE )
	{	
		str = _T("Error, PLC Receive Buffer too small !");
		str = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		m_ErrorString.Format(_T("%s (%d/%d)"), str, ResponseSize, JET_PLC_BUFFER_SIZE);
		this->UnlockPLC();
		return false;
	}		
	memset(m_ReceiveBuffer, 0x00, ResponseSize+10);		

	int GetResponseLength = 0;	
	const int MaxI = m_CommCheckCount;
	int TotalGetResponseLength = 0;
	i = 0;
	do 
	{		
		GetResponseLength = m_RS232.ReadDataByNumber(&(m_ReceiveBuffer[TotalGetResponseLength]), ResponseSize-TotalGetResponseLength, RealFullDataLength);
		TotalGetResponseLength = TotalGetResponseLength+GetResponseLength;
		if ( TotalGetResponseLength >= RealFullDataLength ) { break; }
		i ++ ;
		if ( i > MaxI ) 
		{
			str = _T("Error, PLC Get Response from COM port wait for Timeout!");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			this->UnlockPLC();
			return false;
		}			
		::Sleep(0);
	} while ( true );		
		
	::strcpy(SendBuffer, m_SendBuffer);
	::strcpy(ReceiveBuffer, m_ReceiveBuffer);

	this->UnlockPLC();
	//==================================================================================//		
	if ( strlen(ReceiveBuffer) <= 6 )
	{
		str = _T("Error, PLC No Response From COM");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;
	}

	if(ReceiveBuffer[0]!=ACK) 
	{	return false;	}

	for(i=1;i<=4;i++)
	{
		if(ReceiveBuffer[i]!=SendBuffer[i]) 
		{	return false;	}
	}

	ErrorNumber = (ReceiveBuffer[5]-0x30)*10 + (ReceiveBuffer[6]-0x30);
	if ( !CheckErrorNumber(ErrorNumber) )
	{	return false;	}

	data[0] = ReceiveBuffer[9];
	data[1] = ReceiveBuffer[10];
	data[2] = ReceiveBuffer[7];
	data[3] = ReceiveBuffer[8];
	data[4] = '\0';				
#endif
	return true;	
}
//-------------------------------------------------------//
bool CPLC_Vigor::WriteDCode(int DN, char data[])
{
	if ( CPLC_Vigor::DoWriteDCode(DN, data) == false )
	{	
		CString str;
		str = _T("Error, Write DCode Fault");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		m_ErrorString.Format(_T("%s [D%d]"), str, DN);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_WRITE_NODE);
		return false;
	}
	return true;
}
//-------------------------------------------------------//
bool CPLC_Vigor::DoWriteDCode(int DN, char data[])//Write D Conde, in 16-bit mode
{
#ifndef PLC_OBJ_DISABLE	
	if ( CheckPLCIsConnected() == false ) 
	{	return false;	}

	CString str;
	const size_t szData=::strlen(data);
	if ( DN < 0 || DN >8191 ) 
	{	
		str = _T("Error, PLC D code number out of size ( 0 ~ 8191 )"); 
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;  
	}

	if ( 4 != szData )
	{	
		str = _T("Error, PLC data lenght dones not equal to  4" ); 
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;  
	}
		
	char Command = 0x61;
	int i=0, index=0;
	int DataAdress=(int)(2*DN+PLC_D_BASE), point=0;
	int temp=0;
	int ErrorNumber = 0;
	int DataLength = 2;
	char lowbyte = 0, hightbyte = 0;
	const int Len = 18;
	char SendBuffer[JET_PLC_BUFFER_SIZE];
	char ReceiveBuffer[JET_PLC_BUFFER_SIZE];
	const TPLCParameter &Param = GetPLCParameter();

	this->LockPLC();	

	memset(m_SendBuffer, 0x00, Len+1 );
	//STX
	m_SendBuffer[0] = STX;
	//Stage Number
	m_SendBuffer[1] = (m_StageNumber&0Xf0)>>4;
	GetBytes(m_SendBuffer[1], hightbyte, lowbyte);
	m_SendBuffer[1] = hightbyte + lowbyte + 0x30;
	
	m_SendBuffer[2] = m_StageNumber&0X0f;
	GetBytes(m_SendBuffer[2], hightbyte, lowbyte);
	m_SendBuffer[2] = hightbyte + lowbyte + 0x30;

	//Command
	m_SendBuffer[3] = (Command&0xf0)>>4;
	GetBytes(m_SendBuffer[3], hightbyte, lowbyte);
	m_SendBuffer[3] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[4] = (Command&0x0f);
	GetBytes(m_SendBuffer[4], hightbyte, lowbyte);
	m_SendBuffer[4] = hightbyte + lowbyte + 0x30;

	//Start Data Adress
	m_SendBuffer[5] = ((DataAdress>>12) & 0x0f);
	GetBytes(m_SendBuffer[5], hightbyte, lowbyte);
	m_SendBuffer[5] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[6] = ((DataAdress>>8) & 0x0f);
	GetBytes(m_SendBuffer[6], hightbyte, lowbyte);
	m_SendBuffer[6] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[7] = ((DataAdress>>4) & 0x0f);
	GetBytes(m_SendBuffer[7], hightbyte, lowbyte);
	m_SendBuffer[7] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[8] = ( DataAdress & 0x0f);
	GetBytes(m_SendBuffer[8], hightbyte, lowbyte);
	m_SendBuffer[8] = hightbyte + lowbyte + 0x30;
		
	//Data Length
	m_SendBuffer[9] = ((DataLength&0x00f0)>>4);
	GetBytes(m_SendBuffer[9], hightbyte, lowbyte);
	m_SendBuffer[9] = hightbyte + lowbyte + 0x30;
		
	m_SendBuffer[10] = (DataLength & 0x0000f);
	GetBytes(m_SendBuffer[10], hightbyte, lowbyte);
	m_SendBuffer[10] = hightbyte + lowbyte + 0x30;
		
	//data	
	m_SendBuffer[11] = data[2];
	m_SendBuffer[12] = data[3];
	m_SendBuffer[13] = data[0];
	m_SendBuffer[14] = data[1];
	//End
	m_SendBuffer[15] = ETX;
	index = 15;

	temp = 0;
	for ( i=1; i<index+1; i++ )
	{	temp += m_SendBuffer[i];	}

	//SUM
	m_SendBuffer[index+1] = (temp&0x00f0)>>4;
	GetBytes(m_SendBuffer[index+1], hightbyte, lowbyte);
	m_SendBuffer[index+1] = hightbyte + lowbyte + 0x30;		

	m_SendBuffer[index+2] = (temp&0x000f);
	GetBytes(m_SendBuffer[index+2], hightbyte, lowbyte);
	m_SendBuffer[index+2] = hightbyte + lowbyte + 0x30;

	m_SendBuffer[index+3] = '\0';
	//==========================================================================//	
	const int TotLen=index+3;
	//Send Msg to COM
	if ( m_RS232.SendData(m_SendBuffer, TotLen) != TotLen )
	{
		str = _T("Error, PLC Send Datas to COM port Fault!");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		this->UnlockPLC();
		return false;
	}
		
	if ( Param.m_PLCCommDelayTime > 0 ) 
	{	::Sleep(Param.m_PLCCommDelayTime); }

	//Get Response		
	m_ReceiveBuffer[0]='\0';
	::memset(SendBuffer, 0x00, sizeof(SendBuffer[0])*24);
		
	int GetResponseLength = 0;	
	const int MaxI = m_CommCheckCount;
	const int ResponseSize = 10;
	const int RealFullDataLength = 10;
	int TotalGetResponseLength = 0;
	i = 0;
	do 
	{		
		GetResponseLength = m_RS232.ReadDataByNumber(&(m_ReceiveBuffer[TotalGetResponseLength]), ResponseSize-TotalGetResponseLength, RealFullDataLength);
		TotalGetResponseLength = TotalGetResponseLength+GetResponseLength;
		if ( TotalGetResponseLength >= RealFullDataLength ) { break; }
		i ++ ;
		if ( i > MaxI ) 
		{
			str = _T("Error, PLC Get Response from COM port wait for Timeout!");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			this->UnlockPLC();
			return false;
		}
		::Sleep(0);			
	} while ( true );				
		
	::strcpy(SendBuffer, m_SendBuffer);
	::strcpy(ReceiveBuffer, m_ReceiveBuffer);

	this->UnlockPLC();
	//==========================================================================//		
	if ( strlen(ReceiveBuffer) == 0 )
	{
		str = _T("Error, PLC No Response From COM");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		return false;
	}

	if(ReceiveBuffer[0]!=ACK)
	{
		return false;
	}
		
	for(i=1;i<=4;i++)
	{	
		if(ReceiveBuffer[i]!=SendBuffer[i])	
		{
			return false; 
		}
	}

	//Get Response
	ErrorNumber = (ReceiveBuffer[5]-0x30)*10 + (ReceiveBuffer[6]-0x30);
	if ( !CheckErrorNumber(ErrorNumber) ) 
	{ 
		return false; 
	}		
#endif//PLC_OBJ_DISABLE
	return true; 
}
//-----------------------------------------------------------------------//
int  CPLC_Vigor::TransferDcodeToINT(const char *pDCode)
{
	int Code = -1;
	if ( pDCode == NULL ) { return Code; }

	//16進位碼
	int i=0;
	int Value = 0, Value2 = 0;
	//個位數
	if ( pDCode[3] > 57 ) 
	{	Value2 = pDCode[3] - 55; }
	else 
	{	Value2 = pDCode[3] - 48; }
	Value = Value+Value2;

	//十位數
	if ( pDCode[2] > 57 ) 
	{	Value2 = pDCode[2] - 55; }
	else 
	{	Value2 = pDCode[2] - 48; }
	Value2 = Value2*16;
	Value = Value+Value2;
	
	//百位數
	if ( pDCode[1] > 57 ) 
	{	Value2 = pDCode[1] - 55; }
	else 
	{	Value2 = pDCode[1] - 48; }
	Value2 = Value2*16*16;
	Value = Value+Value2;	

	//千位數
	if ( pDCode[0] > 57 ) 
	{	Value2 = pDCode[0] - 55; }
	else 
	{	Value2 = pDCode[0] - 48; }
	Value2 = Value2*16*16*16;
	Value = Value+Value2;
	
	Code = Value;
	return Code;
}
//----------------------------------------------------------------------------//
void CPLC_Vigor::TransferINTToDcodeData(int value, char Data[], int DataCnt)
{
	int a=0, b=0, c=value, idx=3;
	char ch=0;	
	switch ( DataCnt )
	{
	case 4://16bits
		idx=3;
		Data[0] = Data[1] = Data[2] = Data[3] = '0';
		Data[4] = '\0';
		break;
	case 8://32bits
		idx=7;
		Data[0] = Data[1] = Data[2] = Data[3] = '0';
		Data[4] = Data[5] = Data[6] = Data[7] = '0';
		Data[8] = '\0';
		break;
	}
	while ( true )
	{
		a = c/16;
		b = c%16;
		if ( a > 0 ) 
		{
			if ( b<10 )
			{	ch = (char)('0'+b);	}
			else
			{	ch = (char)('A'+b-10);	}			
			Data[idx] = ch;

			idx --;
			if ( idx < 0 ) { break; }
			c = a;
		}
		else
		{
			if ( b<10 )
			{	ch = (char)('0'+b);	}
			else
			{	ch = (char)('A'+b-10);	}			
			Data[idx] = ch;
			break;
		}
	}
	return ;	
}
//----------------------------------------------------------------------------//
int CPLC_Vigor::GetCodeNumber(const char *CodeName)
{
	int number = -1;
	if ( NULL == CodeName ) { return number; }

	const size_t size = ::strlen(CodeName);
	if ( size >=32 ) { return number; }

	size_t i=0;
	char Buffer[32]="";
	
	for ( i=0; i<size; i++ )
	{	Buffer[i] = CodeName[i+1];	}
	Buffer[i] = '\0';
	number = ::atoi(Buffer);
	return number;
}
//----------------------------------------------------------------------------//
bool CPLC_Vigor::PLC_BuildPlcNodeList()
{
	this->m_PLCNodeList.clear();

	TPlcNode PlcNode;

	PlcNode.m_Value = 0;
	PlcNode.m_Seleted = false;
	PlcNode.m_Deleted = false;
	PlcNode.m_ToRead = true;

	//LANE_A_DCODE D1000
	::strcpy(PlcNode.m_Address, "D1000");
	PlcNode.m_Name = _T("Lane A DCode");
	PlcNode.m_FnCode = PLC_VIGOR_NODE_FNC_LANE_A_DCODE;
	this->m_PLCNodeList.push_back(PlcNode);

	//LANE_B_DCODE D2000
	::strcpy(PlcNode.m_Address, "D2000");
	PlcNode.m_Name = _T("Lane B DCode");
	PlcNode.m_FnCode = PLC_VIGOR_NODE_FNC_LANE_B_DCODE;
	this->m_PLCNodeList.push_back(PlcNode);

	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::PLC_ReadNode(const char *Node, int &value)
{
	bool IsOK = true;
#ifndef PLC_OBJ_DISABLE
	CString str;
	if ( NULL == Node )
	{
		str = _T("Error, Read Node Fault (Node==NULL)"); 
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_READ_NODE);
		return false;
	}
	int i = 0;	
	bool bState = false;	
	int  DNumber = 0;
	char Type = Node[0];	
	char DCodeResult[32]="";
	switch ( Type )
	{
	case 'm':
	case 'M':
	case 'x':
	case 'X':
	case 'y':
	case 'Y':		
		IsOK = this->ReadSingle(Node, bState);		
		if ( false == bState ) { value = 0; }
		else { value = 1; }
		break;
	case 'd':
	case 'D':
		DNumber = this->GetCodeNumber(Node);		
		IsOK = this->ReadDCode(DNumber, DCodeResult);
		value = this->TransferDcodeToINT(DCodeResult);
		break;
	default:
		IsOK = false;
		str = _T("Error, PLC Node Exception");
		str = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		m_ErrorString.Format(_T("%s (%s)"), str, CString(Node));
		break;
	}
	if ( false == IsOK )
	{	SetPLCExceptionCode(AOI_EXCEPTION_PLC_READ_NODE);	}
#endif//PLC_OBJ_DISABLE
	return IsOK;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::PLC_WriteNode(const char *Node, int value)
{
	bool IsOK = true;
#ifndef PLC_OBJ_DISABLE
	CString str;
	if ( NULL == Node )
	{
		str = _T("Error, Write Node Fault (Node==NULL)"); 
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_WRITE_NODE);
		return false;
	}
	char Type = Node[0];
	int i = 0;	
	bool bState = false;	
	int  DNumber = 0;	
	char DCodeResult[32]="";

	switch ( Type )
	{
	case 'm':
	case 'M':
	case 'x':
	case 'X':
	case 'y':
	case 'Y':		
		if ( value == 0 ) { bState = false; }
		else { bState = true; }
		IsOK = this->WriteSingle(Node, bState);				
		break;
	case 'd':
	case 'D':
		DNumber = this->GetCodeNumber(Node);
		this->TransferINTToDcodeData(value, DCodeResult);
		IsOK = this->WriteDCode(DNumber, DCodeResult);		
		break;
	default:
		IsOK = false;
		str = _T("Error, PLC Node Exception");
		str = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		m_ErrorString.Format(_T("%s (%s)"), str, CString(Node));
		break;
	}
	if ( false == IsOK )
	{	SetPLCExceptionCode(AOI_EXCEPTION_PLC_WRITE_NODE);	}
#endif//PLC_OBJ_DISABLE
	return IsOK;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::PLC_WriteINIParameter()
{
	if ( PLC_WriteINIParameterFn() == false )
	{
		SetPLCExceptionCode_FileWrite();	
		return false; 
	}
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::PLC_WriteINIParameterFn()
{
#ifndef PLC_OBJ_DISABLE

	char TimeS[32]="";
	int DCode = 0;
	int ValueI = 0;
	bool bEnable = true;
	const TPLCParameter &Param = GetPLCParameter();

	
	//Test 32Bit-DCode
	//DCode = 7400;
	//ValueI = 70000;
	//char ReadS[32]="";
	//this->TransferINTToDcodeData(ValueI, TimeS, 8);
	//this->WriteDCode(DCode, TimeS);
	//ReadDCode(DCode, ReadS);
	//*/

	DCode = JET_PLC_PCB_IN_OUT_TIMEOUT_LA;
	ValueI = Param.m_PCBInOutTimeout_LA;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	

	DCode = JET_PLC_PCB_IN_OUT_TIMEOUT_LB;
	ValueI = Param.m_PCBInOutTimeout_LB;	
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	

	DCode = JET_PLC_AIR_LOST_CHECK_TIME;
	ValueI = Param.m_AirLostCheckTime;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }

	DCode = JET_PLC_PCB_STOP_DELAY_TIME;	
	ValueI = Param.m_PCBStopDelayTime;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }		

	DCode = JET_PLC_CLAMP_ON_OFF_DELAY_TIME;
	ValueI = Param.m_ClampOnOffDelayTime;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }

	DCode = JET_PLC_PCB_OUT_WITH_IN_DELAY_TIME_LA;
	ValueI = Param.m_PCBOutWithInDelayTime_LA;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	

	DCode = JET_PLC_PCB_OUT_WITH_IN_DELAY_TIME_LB;
	ValueI = Param.m_PCBOutWithInDelayTime_LB;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	
	
	DCode = JET_PLC_PCB_OUT_DELAY_TIME_LA;
	ValueI = Param.m_PCBOutDelayTime_LA;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	

	DCode = JET_PLC_PCB_OUT_DELAY_TIME_LB;
	ValueI = Param.m_PCBOutDelayTime_LB;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }

	DCode = JET_PLC_PCB_CLEAR_TIMEOUT_LA;
	ValueI = Param.m_PCBClearTimeout_LA;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }

	DCode = JET_PLC_PCB_CLEAR_TIMEOUT_LB;
	ValueI = Param.m_PCBClearTimeout_LB;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }
	
	DCode = JET_PLC_TURN_ON_LAST_SIGNAL_DELAY_TIME;
	ValueI = Param.m_TurnOnLastSignalDelayTime;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }

	DCode = JET_PLC_CHECK_PCB_DOUBLE_IN_PITCH_TIME;
	ValueI = Param.m_CheckPCBDoubleInPitchTime;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }

	DCode = JET_PLC_CHECK_PCB_DOUBLE_IN_BOARD_TIME;
	ValueI = Param.m_CheckPCBDoubleInBoardTime;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }

	if ( FN_ENABLE == Param.m_PCBInAutoOffStopBar_LA ) { bEnable = true; }
	else { bEnable = false; }
	if ( this->PLC_WriteNode("M333", bEnable) == false ) { return false; }
	
	if ( FN_ENABLE == Param.m_PCBInAutoOffStopBar_LB ) { bEnable = true; }
	else { bEnable = false; }
	if ( this->PLC_WriteNode("M433", bEnable) == false ) { return false; }

	if ( FN_ENABLE == Param.m_PCBAutoRunWithSideStop_LA ) { bEnable = true; }
	else { bEnable = false; }
	if ( this->PLC_WriteNode("M335", bEnable) == false ) { return false; }
	
	if ( FN_ENABLE == Param.m_PCBAutoRunWithSideStop_LB ) { bEnable = true; }
	else { bEnable = false; }
	if ( this->PLC_WriteNode("M435", bEnable) == false ) { return false; }

	if ( FN_ENABLE == Param.m_PCBOutWithClearOKNGSignal ) { bEnable = true; }
	else { bEnable = false; }
	if ( this->PLC_WriteNode("M346", bEnable) == false ) { return false; }
	
	if ( FN_ENABLE == Param.m_EnableCheckPCBDoubleIn ) { bEnable = true; }
	else { bEnable = false; }
	if ( this->PLC_WriteNode("M338", bEnable) == false ) { return false; }

	if ( FN_ENABLE == Param.m_EnableCheckPCBBargeIn ) { bEnable = true; }
	else { bEnable = false; }
	if ( this->PLC_WriteNode("M2999", bEnable) == false ) { return false; }

	//啟用風扇警報
	DCode = JET_PLC_ENABLE_FAN_ALARM;
	ValueI = (int)(Param.m_EnabledFanAlarm);
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }

	//PLC啟用開門斷電
	DCode = JET_PLC_ENABLE_OPEN_DOOR_STOP_POWER;
	ValueI = (int)(Param.m_EnabledOpenDoorStopPower);
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }

	//啟用擋板塊感應器-A軌
	DCode = JET_PLC_ENABLE_STOPPER_SENSOR_LA;
	ValueI = (int)(Param.m_EnabledStopperSensor_LA);
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }

	//啟用擋板塊感應器-B軌
	DCode = JET_PLC_ENABLE_STOPPER_SENSOR_LB;
	ValueI = (int)(Param.m_EnabledStopperSensor_LB);
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }

	//開啟軌道感測器電源	
	if ( WriteConveyerSensorPower_LA(true, true) == false ) { return false; }
	if ( WriteConveyerSensorPower_LB(true, true) == false ) { return false; }	

	DCode = JET_PLC_TOWER_LIGHT_MODE;
	ValueI = (int)(this->m_TowerLightMode);
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }

	//軌道自動調整參數
	//m_LaneAdjust_LimitMax_LA
	DCode = JET_PLC_LANE_ADJUST_LIMIT_MAX_LA;
	ValueI = (int)(this->m_LaneAdjust_LimitMax_LA*10);
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }		

	//m_LaneAdjust_LimitMax_LB
	DCode = JET_PLC_LANE_ADJUST_LIMIT_MAX_LB;
	ValueI = (int)(this->m_LaneAdjust_LimitMax_LB*10);
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	
	
	//m_LaneAdjust_LimitMin_LA
	DCode = JET_PLC_LANE_ADJUST_LIMIT_MIN_LA;
	ValueI = (int)(this->m_LaneAdjust_LimitMin_LA*10);
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	

	//m_LaneAdjust_LimitMin_LB
	DCode = JET_PLC_LANE_ADJUST_LIMIT_MIN_LB;
	ValueI = (int)(this->m_LaneAdjust_LimitMin_LB*10);
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	

	//m_LaneAdjust_MoveSpeed_LA
	DCode = JET_PLC_LANE_ADJUST_MOVE_SPEED_LA;
	ValueI = (int)(this->m_LaneAdjust_MoveSpeed_LA*10);
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	

	//m_LaneAdjust_JogSlowSpeed_LA
	DCode = JET_PLC_LANE_ADJUST_JOG_SPEED_LA;
	ValueI = (int)(this->m_LaneAdjust_JogSlowSpeed_LA*10);
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	

	//m_LaneAdjust_MoveSpeed_LB
	DCode = JET_PLC_LANE_ADJUST_MOVE_SPEED_LB;
	ValueI = (int)(this->m_LaneAdjust_MoveSpeed_LB*10);
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }

	//m_LaneAdjust_JogSlowSpeed_LB
	DCode = JET_PLC_LANE_ADJUST_JOG_SPEED_LB;
	ValueI = (int)(this->m_LaneAdjust_JogSlowSpeed_LB*10);
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }
	
	//m_LaneAdjust_HomePos_LA
	DCode = JET_PLC_LANE_ADJUST_HOME_POS_LA;
	ValueI = (int)(this->m_LaneAdjust_HomePos_LA*10);
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	

	//m_LaneAdjust_HomePos_LB
	DCode = JET_PLC_LANE_ADJUST_HOME_POS_LB;
	ValueI = (int)(this->m_LaneAdjust_HomePos_LB*10);
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	
	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
CString CPLC_Vigor::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)//載入多國語系
{
	CString NewLabelText;
	LPCTSTR Section=_T("PLC_VIGOR");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
CString CPLC_Vigor::GetPLCErrorCodeText(int ErrorCode)//取得PLC錯誤帶碼文字
{
	CString str;
	CString str2;
	switch ( ErrorCode )
	{	
	case 1://氣壓低下	
		str = _T("Error, Current Air is too low"); 
		break;
	case 2://前門未關	
		str = _T("Error, Front Cap is opoened"); 
		break;
	case 3://後門未關	
		str = _T("Error, Rear Door is opoened"); 
		break;
	case 5://過熱警報
		str = _T("Error, Over Heat"); 
		break;
	case 6://風扇異常//m_EnabledFanAlarm
		str = _T("Error, Fan Alarm"); 
		break;

	case 10://A 進板監控
		str = _T("Error, PCB-In Fault With PLC Checked (LaneA)");	
		break;
	case 11://A 出板監控
		str = _T("Error, PCB-Out Fault With PLC Checked (LaneA)");
		break;
	case 12://A 回板監控
		str = _T("Error, PCB-Back Fault With PLC Checked (LaneA)");
		break;
	case 13://A 慢速回進板監控
		str = _T("Error, PCB Re-In Fault With PLC Checked (LaneA)");
		break;
	case 14://A 同進同出監控
		str = _T("Error, PCB-Out-In Fault With PLC Checked (LaneA)");
		break;
	case 15://A 機台內出板監控
		str = _T("Error, PCB-Out-Stop Fault With PLC Checked (LaneA)");
		break;
	case 16://A 自動進板監控
		str = _T("Error, PCB Auto Out-In Fault With PLC Checked (LaneA)");
		break;
	case 17://A BYPASS監控 
		str = _T("Error, PCB Bypass Fault With PLC Checked (LaneA)");
		break;
	case 18://A PCB重複進板(連進多板)
		str = _T("Error, PCB Double In With PLC Checked (LaneA)");
		break;
	case 19://保留
		break;

	case 20://B 進板監控
		str = _T("Error, PCB-In Fault With PLC Checked (LaneB)");	
		break;
	case 21://B 出板監控
		str = _T("Error, PCB-Out Fault With PLC Checked (LaneB)");
		break;
	case 22://B 回板監控
		str = _T("Error, PCB-Back Fault With PLC Checked (LaneB)");
		break;
	case 23://B 慢速回進板監控
		str = _T("Error, PCB Re-In Fault With PLC Checked (LaneB)");
		break;
	case 24://B 同進同出監控
		str = _T("Error, PCB-Out-In Fault With PLC Checked (LaneB)");
		break;
	case 25://B 機台內出板監控
		str = _T("Error, PCB-Out-Stop Fault With PLC Checked (LaneB)");
		break;
	case 26://B 自動進板監控
		str = _T("Error, PCB Auto Out-In Fault With PLC Checked (LaneB)");
		break;
	case 27://B BYPASS監控 
		str = _T("Error, PCB Bypass Fault With PLC Checked (LaneB)");
		break;	
	case 28://B PCB重複進板(連進多板)
		str = _T("Error, PCB Double In With PLC Checked (LaneB)");
		break;
	case 29://保留		
		break;

	case 30://軌道2 馬達軸控異常
		str = _T("Error, Lane Adjust 2 Motion Controller Exception With PLC Checked");
		break;
	case 31://軌道2 回原點異常	
		str = _T("Error, Lane Adjust 2 Home Move Fault With PLC Checked");
		break;
	case 32://軌道2 運轉到極限點異常
		str = _T("Error, Lane Adjust 2 Move To Limit Fault With PLC Checked");
		break;
	case 33://軌道4 馬達軸控異常	
		str = _T("Error, Lane Adjust 4 Motion Controller Exception With PLC Checked");
		break;
	case 34://軌道4 回原點異常	
		str = _T("Error, Lane Adjust 4 Home Move Fault With PLC Checked");
		break;
	case 35://軌道4 運轉到極限點異常
		str = _T("Error, Lane Adjust 4 Move To Limit Fault With PLC Checked");
		break;
	case 36://安全開關檢知異常
		str = _T("Error, Lane Adjust Safty Exception With PLC Checked");
		break;
	case 37://軌道2 回原點過久異常
		str = _T("Error, Lane Adjust 2 Home Move Timeout With PLC Checked");
		break;
	case 38://軌道2 運轉過久異常
		str = _T("Error, Lane Adjust 2 Moving Timeout With PLC Checked");
		break;
	case 39://軌道4 回原點過久異常
		str = _T("Error, Lane Adjust 4 Home Move Timeout With PLC Checked");
		break;
	case 40://軌道4 運轉過久異常
		str = _T("Error, Lane Adjust 4 Moving Timeout With PLC Checked");
		break;

	case 48://2軌道異常集合	
		str = _T("Error, Lane Adjust 2 Exception With PLC Checked");
		break;
	case 49://4軌道異常集合	
		str = _T("Error, Lane Adjust 4 Exception With PLC Checked");
		break;

	case 50://A軌PCB闖入
		str = _T("Error, PCB Barge In LA");
		break;
	case 51://B軌PCB闖入
		str = _T("Error, PCB Barge In LB");
		break;

	case 99://緊急停止壓下
		str = _T("Error, EMS button push down");
		break;

	case 106://軌道2 未回原點警示
		str = _T("Warning, Lane Adjust 2 Not Homed");
		break;
	case 107://軌道4 未回原點警示
		str = _T("Warning, Lane Adjust 4 Not Homed");
		break;
	case 108://軌道2 運轉中警示
		str = _T("Warning, Lane Adjust 2 Is Moving");
		break;
	case 109://軌道4 運轉中警示
		str = _T("Warning, Lane Adjust 4 Is Moving");
		break;

	default:
		break;
	}
	
	if ( str.GetLength() != 0 ) 
	{	
		str2 = CPLC_Vigor::LoadMultiLanguageString(str, str); 
		str.Format(_T("%s [%d]"), str2, ErrorCode);
	}
	else
	{	str.Format(_T("Error, PLC ErrorCode [%d]"), ErrorCode); }
	return str;
}
//-------------------------------------------------------------------------------------//
bool CPLC_Vigor::PLC_ReadAllStats(bool bCheckThread)
{
#ifndef PLC_OBJ_DISABLE	
	//----------------------------------------------------------------------------//	
	THREAD_COMMAND_MODE ToRunID = THREAD_COMMAND_TO_RUN;	
	//----------------------------------------------------------------------------//	
	if ( CPLC_Vigor::PLC_ReadMachineSensor(bCheckThread) == false ) 
	{	return false; }
	ToRunID = GetPLCPollingThreadCmd();
	if ( THREAD_COMMAND_TO_RUN != ToRunID ) {	return true; }

	if ( CPLC_Vigor::PLC_ReadExecAlarm() == false )
	{	return false; }
	ToRunID = GetPLCPollingThreadCmd();
	if ( THREAD_COMMAND_TO_RUN != ToRunID ) {	return true; }

	if ( CPLC_Vigor::PLC_ReadConveryerSensor(bCheckThread) == false ) 
	{	return false; }
	ToRunID = GetPLCPollingThreadCmd();
	if ( THREAD_COMMAND_TO_RUN != ToRunID ) {	return true; }

	if ( CPLC_Vigor::PLC_ReadTowerLight(bCheckThread) == false ) 
	{	return false; }
	ToRunID = GetPLCPollingThreadCmd();
	if ( THREAD_COMMAND_TO_RUN != ToRunID ) {	return true; }

	if ( CPLC_Vigor::PLC_ReadConveryerSignalSend(bCheckThread) == false ) 
	{	return false; }	
	ToRunID = GetPLCPollingThreadCmd();
	if ( THREAD_COMMAND_TO_RUN != ToRunID ) {	return true; }

	if ( CPLC_Vigor::PLC_ReadConveryerRunStatus(bCheckThread) == false ) 
	{	return false; }	
	ToRunID = GetPLCPollingThreadCmd();
	if ( THREAD_COMMAND_TO_RUN != ToRunID ) {	return true; }

	bool PollingLaneAdjustSensor = GetPLCPollingLaneAdjustSensor();	
	ReadLaneAdjustDisable_LA();
	ReadLaneAdjustDisable_LB();
	if ( true == PollingLaneAdjustSensor )
	{
		if ( CPLC_Vigor::PLC_ReadLaneAdjustStatus(bCheckThread) == false ) 
		{	return false; }
	}	
	//m_PLCVersion	
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::PLC_ReadExecAlarm()
{	
#ifndef PLC_OBJ_DISABLE	
	int States=FN_DISABLE;
	if( this->PLC_ReadNode("M104", States) == false ) 
	{	return false;	}	
	if ( FN_DISABLE == States )
	{	m_PLCExecAlarm = false; }
	else
	{	m_PLCExecAlarm = true;	}	
#endif//PLC_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------//
bool CPLC_Vigor::PLC_ReadTowerLight(bool bCheckThread)//讀取塔燈
{
#ifndef PLC_OBJ_DISABLE
	bool  States=false;	
	int   Res  = 0;	
	int   Mask = 0;	
	int   Value = 0;
	int   DCodeID = 1040;
	char  MCodeS[32]="";
	char  DCodeS[32]="";		

	//電源與塔燈
	DCodeID = 1050;
	if( this->ReadDCode(DCodeID, DCodeS) == false ) { return false; }	
	Value = this->TransferDcodeToINT(DCodeS);
	if ( 0 == (Value&0x0001) )	{	m_PLC_O_PowerACMotor = false; }
	else	{	m_PLC_O_PowerACMotor = true; }
	if ( 0 == (Value&0x0002) )	{	m_PLC_O_LightDay = false; }
	else	{	m_PLC_O_LightDay = true; }
	if ( 0 == (Value&0x0004) )	{	m_PLC_O_LightStart = false; }
	else	{	m_PLC_O_LightStart = true; }
	if ( 0 == (Value&0x0008) )	{	m_PLC_O_LightStop = false; }
	else	{	m_PLC_O_LightStop = true; }

	//蜂鳴器與塔燈	
	if ( 0 == (Value&0x0010) )	{	m_PLC_O_LightTowerRed_LA = false; }
	else	{	m_PLC_O_LightTowerRed_LA = true; }
	if ( 0 == (Value&0x0020) )	{	m_PLC_O_LightTowerYel_LA = false; }
	else	{	m_PLC_O_LightTowerYel_LA = true; }
	if ( 0 == (Value&0x0040) )	{	m_PLC_O_LightTowerGrn_LA = false; }
	else	{	m_PLC_O_LightTowerGrn_LA = true; }
	if ( 0 == (Value&0x0080) )	{	m_PLC_O_Buzzer_LA = false; }
	else	{	m_PLC_O_Buzzer_LA = true; }	 
	m_PLC_O_Buzzer_LB = m_PLC_O_Buzzer_LA;
	m_PLC_O_LightTowerRed_LB = m_PLC_O_LightTowerRed_LA;
	m_PLC_O_LightTowerYel_LB = m_PLC_O_LightTowerYel_LA;
	m_PLC_O_LightTowerGrn_LB = m_PLC_O_LightTowerGrn_LA;

	//if ( 0 == (Value&0x0002) )	{	m_PLC_O_LightDay = false; }
	//else	{	m_PLC_O_LightDay = true; }
	//if ( 0 == (Value&0x0004) )	{	m_StartLight = false; }
	//else	{	m_StartLight = true; }
	//if ( 0 == (Value&0x0008) )	{	m_StopLight = false; }
	//else	{	m_StopLight = true; }	
	//if ( 0 == (Value&0x0010) )	{	m_TowerLightRed = false; }
	//else	{	m_TowerLightRed = true; }
	//if ( 0 == (Value&0x0020) )	{	m_TowerLightYellow = false; }
	//else	{	m_TowerLightYellow = true; }
	//if ( 0 == (Value&0x0040) )	{	m_TowerLightGreen = false; }
	//else	{	m_TowerLightGreen = true; }
	//if ( 0 == (Value&0x0080) )	{	m_Buzzer = false; }
	//else	{	m_Buzzer = true; }
	/*
	Status = (Value&0x0001)?true:false;	m_PLC_O_MotionPower = Status;
	Status = (Value&0x0002)?true:false;	m_PLC_O_Light = Status;
	Status = (Value&0x0004)?true:false;	m_PLC_O_StartLight = Status;
	Status = (Value&0x0008)?true:false;	m_PLC_O_StopLight = Status;
	Status = (Value&0x0010)?true:false;	m_PLC_O_TowerR = Status;
	Status = (Value&0x0020)?true:false;	m_PLC_O_TowerY = Status;
	Status = (Value&0x0040)?true:false;	m_PLC_O_TowerG = Status;
	Status = (Value&0x0080)?true:false;	m_PLC_O_Buzzer = Status;
	*/
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::PLC_ReadMachineSensor(bool bCheckThread)//讀取機台感測器
{
#ifndef PLC_OBJ_DISABLE
	int SleepTime = this->GetPLCPollingSleepTime();//不能拿掉，否則會卡住
	//----------------------------------------------------------------------------//
	const THREAD_COMMAND_MODE ToRunID = THREAD_COMMAND_TO_RUN;	
	bool  States=false;	
	int   Res  = 0;	
	int   Mask = 0;	
	int   Value = 0;
	int   DCodeID = 1040;
	char  MCodeS[32]="";
	char  DCodeS[32]="";	
	const TPLCParameter &PlcParam = GetPLCParameter();
	const int LastStationLineMode = GetLastStationLineMode();

	DCodeID = 1020;
	if( this->ReadDCode(DCodeID, DCodeS) == false ) { return false; }	
	Value = this->TransferDcodeToINT(DCodeS);		

	//左右進料方向	
	if ( 0 == (Value&0x0002) )	{	m_PLC_RightIn = false; }
	else	{	m_PLC_RightIn = true; }
	if ( 0 == (Value&0x0004) )	{	m_PLC_SafetyAlarm = false; }
	else	{	m_PLC_SafetyAlarm = true; }
	if ( 0 == (Value&0x0008) )	{	m_PLC_SafetyAlarmEMS = false; }
	else	{	m_PLC_SafetyAlarmEMS = true; }

	if ( 0 == (Value&0x0020) )	{	m_HardwareByPass_LA = false; }
	else	{	m_HardwareByPass_LA = true; }
	if ( 0 == (Value&0x0040) )	{	m_HardwareByPass_LB = false; }
	else	{	m_HardwareByPass_LB = true; }
	
	
	//Status = (Value&0x0004)?true:false;	m_PLC_SafetyAlarm = Status;
	//Status = (Value&0x0008)?true:false;	m_PLC_SafetyAlarmWithEMS = Status;
	//Status = (Value&0x0010)?true:false;	m_PLC_SingleWayMode = Status;
	//Status = (Value&0x0020)?true:false;	m_HardwareByPass_LA = Status;   //m_HardwareByPass
	//Status = (Value&0x0040)?true:false;	m_HardwareByPass_LB = Status;	//m_HardwareByPass

	//機台狀態
	DCodeID = 1030;
	if( this->ReadDCode(DCodeID, DCodeS) == false ) { return false; }	
	Value = this->TransferDcodeToINT(DCodeS);
	if ( 0 == (Value&0x0001) )	{	m_PLC_I_EMSOn = false; }
	else	{	m_PLC_I_EMSOn = true; }
	if ( 0 == (Value&0x0002) )	{	m_PLC_I_BtnStart = false; }
	else	{	m_PLC_I_BtnStart = true; }
	if ( 0 == (Value&0x0004) )	{	m_PLC_I_BtnStop = false; }
	else	{	m_PLC_I_BtnStop = true; }
	if ( 0 == (Value&0x0008) )	{	m_PLC_I_AirLost = false; }
	else	{	m_PLC_I_AirLost = true; }
	if ( 0 == (Value&0x0010) )	{	m_PLC_I_BtnReset = false; }
	else	{	m_PLC_I_BtnReset = true; }
	if ( 0 == (Value&0x0020) )	{	m_PLC_I_FrontCapOpened = false; }
	else	{	m_PLC_I_FrontCapOpened = true; }
	if ( 0 == (Value&0x0040) )	{	m_PLC_I_RearCapOpened = false; }
	else	{	m_PLC_I_RearCapOpened = true; }
	if ( 0 == (Value&0x0080) )	{	m_PLC_I_KeySwitchOff = false; }
	else	{	m_PLC_I_KeySwitchOff = true; }

	//機台狀態#2
	DCodeID = 1041;
	if( this->ReadDCode(DCodeID, DCodeS) == false ) { return false; }	
	Value = this->TransferDcodeToINT(DCodeS);
	//Status = (Value&0x0001)?true:false;	//左右進料旋鈕;
	//Status = (Value&0x0002)?true:false;	//蜂鳴器取消鈕;
	//Status = (Value&0x0004)?true:false;	//日光燈鈕;
	//Status = (Value&0x0008)?true:false;	//手動夾板鈕;
	//Status = (Value&0x0010)?true:false;	//手動輸送帶鈕;
	if ( 0 == (Value&0x0020) )	{	m_PLC_I_OverHeat = false; }
	else	{	m_PLC_I_OverHeat = true; }	

	/*			
	//電源開關
	bool                       m_PLC_O_PowerACMotor;
	*/
	/*	
	Status = (Value&0x0001)?true:false;	m_PLC_I_EMS = Status;
	Status = (Value&0x0002)?true:false;	m_PLC_I_GreenBtn = Status;
	Status = (Value&0x0004)?true:false;	m_PLC_I_RedBtn = Status;
	Status = (Value&0x0008)?true:false;	m_PLC_I_Air = Status;
	Status = (Value&0x0010)?true:false;	m_PLC_I_ResetBtn = Status;
	Status = (Value&0x0020)?true:false;	m_PLC_I_FrontCap = Status;
	Status = (Value&0x0040)?true:false;	m_PLC_I_BackCap = Status;
	Status = (Value&0x0080)?true:false;	m_PLC_I_KeySwitch = Status;	//鑰匙開關	//chia 1030319
	*/	

	if ( FN_DISABLE == PlcParam.m_EnabledFanAlarm )
	{	m_PLC_FanAlarm = m_PLC_I_FanAlarm = false;	}
	else
	{
		if ( ReadSingle("M513", States) == true )//M339
		{	
			if ( false == States )
			{	m_PLC_I_FanAlarm = true; }
			else
			{	m_PLC_I_FanAlarm = false;	}
		}
		else
		{	m_PLC_I_FanAlarm = false;	}		
		if ( ReadSingle("M4006", States) == true )
		{	m_PLC_FanAlarm = States;	}
		else
		{	m_PLC_FanAlarm = false;	}		
	}	
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::PLC_ReadConveryerSensor(bool bCheckThread)
{
#ifndef PLC_OBJ_DISABLE
	int SleepTime = this->GetPLCPollingSleepTime();//不能拿掉，否則會卡住
	//----------------------------------------------------------------------------//
	const THREAD_COMMAND_MODE ToRunID = THREAD_COMMAND_TO_RUN;	
	bool  States=false;	
	int   Res  = 0;	
	int   Mask = 0;	
	int   Value = 0;
	int   DCodeID = 1040;
	char  MCodeS[32]="";
	char  DCodeS[32]="";		
	const int LastStationLineMode = GetLastStationLineMode();
	
	//軌道的Dcode
	DCodeID = 1040;		
	if ( CPLC_Vigor::ReadDCode(DCodeID, DCodeS) == false )
	{	return false; }
	Value = this->TransferDcodeToINT(DCodeS);	
	
	if ( false == m_PLC_RightIn )
	{
		//Lane A
		if ( 0 == (Value&0x0001) )	{	m_PLC_I_SensorPCBIn_LA = false; }
		else	{	m_PLC_I_SensorPCBIn_LA = true; }
		if ( 0 == (Value&0x0002) )	{	m_PLC_I_SensorPCBSlow_LA = false; }
		else	{	m_PLC_I_SensorPCBSlow_LA = true; }
		if ( 0 == (Value&0x0004) )	{	m_PLC_I_SensorPCBStop_LA = false; }
		else	{	m_PLC_I_SensorPCBStop_LA = true; }
		if ( 0 == (Value&0x0008) )	{	m_PLC_I_SensorPCBOut_LA = false; }
		else	{	m_PLC_I_SensorPCBOut_LA = true; }
		if ( 0 == (Value&0x0010) )	{	m_PLC_I_SignalFromLast_LA = false; }
		else	{	m_PLC_I_SignalFromLast_LA = true; }
		if ( 0 == (Value&0x0020) )	{	m_PLC_I_SignalFromNext_LA = false; }
		else	{	m_PLC_I_SignalFromNext_LA = true; }

		if ( 0 == (Value&0x0080) )	{	m_PLC_I_SensorPCBStop2_LA = false; }
		else	{	m_PLC_I_SensorPCBStop2_LA = true; }

		//Lane B
		if ( 0 == (Value&0x0100) )	{	m_PLC_I_SensorPCBIn_LB = false; }
		else	{	m_PLC_I_SensorPCBIn_LB = true; }
		if ( 0 == (Value&0x0200) )	{	m_PLC_I_SensorPCBSlow_LB = false; }
		else	{	m_PLC_I_SensorPCBSlow_LB = true; }
		if ( 0 == (Value&0x0400) )	{	m_PLC_I_SensorPCBStop_LB = false; }
		else	{	m_PLC_I_SensorPCBStop_LB = true; }
		if ( 0 == (Value&0x0800) )	{	m_PLC_I_SensorPCBOut_LB = false; }
		else	{	m_PLC_I_SensorPCBOut_LB = true; }

		if ( 0 == (Value&0x1000) )	{	m_PLC_I_SignalFromLast_LB = false; }
		else	{	m_PLC_I_SignalFromLast_LB = true; }
		if ( 0 == (Value&0x2000) )	{	m_PLC_I_SignalFromNext_LB = false; }
		else	{	m_PLC_I_SignalFromNext_LB = true; }

		if ( 0 == (Value&0x8000) )	{	m_PLC_I_SensorPCBStop2_LB = false; }
		else	{	m_PLC_I_SensorPCBStop2_LB = true; }		
	}
	else
	{
		//Lane A
		if ( 0 == (Value&0x0001) )	{	m_PLC_I_SensorPCBOut_LA = false; }
		else	{	m_PLC_I_SensorPCBOut_LA = true; }
		if ( 0 == (Value&0x0002) )	{	m_PLC_I_SensorPCBStop_LA = false; }
		else	{	m_PLC_I_SensorPCBStop_LA = true; }
		if ( 0 == (Value&0x0004) )	{	m_PLC_I_SensorPCBSlow_LA = false; }
		else	{	m_PLC_I_SensorPCBSlow_LA = true; }
		if ( 0 == (Value&0x0008) )	{	m_PLC_I_SensorPCBIn_LA = false; }
		else	{	m_PLC_I_SensorPCBIn_LA = true; }

		if ( 0 == (Value&0x0010) )	{	m_PLC_I_SignalFromLast_LA = false; }
		else	{	m_PLC_I_SignalFromLast_LA = true; }
		if ( 0 == (Value&0x0020) )	{	m_PLC_I_SignalFromNext_LA = false; }
		else	{	m_PLC_I_SignalFromNext_LA = true; }

		if ( 0 == (Value&0x0080) )	{	m_PLC_I_SensorPCBStop2_LA = false; }
		else	{	m_PLC_I_SensorPCBStop2_LA = true; }

		//Lane B
		if ( 0 == (Value&0x0100) )	{	m_PLC_I_SensorPCBOut_LB = false; }
		else	{	m_PLC_I_SensorPCBOut_LB = true; }
		if ( 0 == (Value&0x0200) )	{	m_PLC_I_SensorPCBStop_LB = false; }
		else	{	m_PLC_I_SensorPCBStop_LB = true; }
		if ( 0 == (Value&0x0400) )	{	m_PLC_I_SensorPCBSlow_LB = false; }
		else	{	m_PLC_I_SensorPCBSlow_LB = true; }
		if ( 0 == (Value&0x0800) )	{	m_PLC_I_SensorPCBIn_LB = false; }
		else	{	m_PLC_I_SensorPCBIn_LB = true; }

		if ( 0 == (Value&0x1000) )	{	m_PLC_I_SignalFromLast_LB = false; }
		else	{	m_PLC_I_SignalFromLast_LB = true; }
		if ( 0 == (Value&0x2000) )	{	m_PLC_I_SignalFromNext_LB = false; }
		else	{	m_PLC_I_SignalFromNext_LB = true; }

		if ( 0 == (Value&0x8000) )	{	m_PLC_I_SensorPCBStop2_LB = false; }
		else	{	m_PLC_I_SensorPCBStop2_LB = true; }		
	}

	//非四線式模式時
	switch ( LastStationLineMode )
	{
	case LAST_STATION_LINE_MODE_2:
		m_PLC_I_SignalFromLast_LA = m_PLC_I_SensorPCBIn_LA;
		m_PLC_I_SignalFromLast_LB = m_PLC_I_SensorPCBIn_LB;
		break;
	case LAST_STATION_LINE_MODE_2_4:
		m_PLC_I_SignalFromLast_LA = m_PLC_I_SensorPCBIn_LA|m_PLC_I_SignalFromLast_LA;
		m_PLC_I_SignalFromLast_LB = m_PLC_I_SensorPCBIn_LB|m_PLC_I_SignalFromLast_LB;
		break;
	}	
	//if ( true == m_PLC_I_SensorPCBStop2_LA ) { Inside = true; }
	/*
	Status = (Value&0x0001)?true:false;	m_PLC_I_SensorL_LA = Status;
	Status = (Value&0x0002)?true:false;	m_PLC_I_SensorCL_LA = Status;
	Status = (Value&0x0004)?true:false;	m_PLC_I_SensorCR_LA = Status;
	Status = (Value&0x0008)?true:false;	m_PLC_I_SensorR_LA = Status;

	Status = (Value&0x0010)?true:false;	m_PLC_I_SensorLL_LA = Status;	//前站給板
	Status = (Value&0x0020)?true:false;	m_PLC_I_SensorRR_LA = Status;	//後站要板
	Status = (Value&0x0040)?true:false;	m_PLC_I_BarcodeTrigger_LA = Status;
	//Status = (Value&0x0080)?true:false;		
	Status = (Value&0x0100)?true:false;	m_PLC_I_SensorL_LB = Status;
	Status = (Value&0x0200)?true:false;	m_PLC_I_SensorCL_LB = Status;
	Status = (Value&0x0400)?true:false;	m_PLC_I_SensorCR_LB = Status;
	Status = (Value&0x0800)?true:false;	m_PLC_I_SensorR_LB = Status;	
	Status = (Value&0x1000)?true:false;	m_PLC_I_SensorLL_LB = Status;	//前站給板
	Status = (Value&0x2000)?true:false;	m_PLC_I_SensorRR_LB = Status;	//後站要板
	Status = (Value&0x4000)?true:false;	m_PLC_I_BarcodeTrigger_LB = Status;
	*/		
#endif//PLC_OBJ_DISABLE	
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::PLC_ReadLaneAdjustStatus(bool bCheckThread)//讀取軌道間距狀態
{	
	if ( GetConveryerEnabled_LA() )
	{
		ReadLaneAdjustSensorORG_LA();
		ReadLaneAdjustSensorLimit_LA();
		ReadLaneAdjustCurrentPos_LA();
	}		
	if ( GetConveryerEnabled_LB() )
	{
		ReadLaneAdjustSensorORG_LB();
		ReadLaneAdjustSensorLimit_LB();
		ReadLaneAdjustCurrentPos_LB();
	}
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::PLC_ReadConveryerRunStatus(bool bCheckThread)//讀取軌道運轉狀態
{
#ifndef PLC_OBJ_DISABLE
	bool  States=false;	
	int   Res  = 0;	
	int   Mask = 0;	
	int   Value = 0;
	int   DCodeID = 1040;
	char  MCodeS[32]="";
	char  DCodeS[32]="";	
	//軌道狀態-A軌道
	if ( GetConveryerEnabled_LA() )
	{
		DCodeID = 7108;
		if( this->ReadDCode(DCodeID, DCodeS) == false ) { return false; }	
		m_PLC_O_ConveryerStatus_LA = this->TransferDcodeToINT(DCodeS);
	}	
	//軌道狀態-B軌道
	if ( GetConveryerEnabled_LB() )
	{
		DCodeID = 7208;
		if( this->ReadDCode(DCodeID, DCodeS) == false ) { return false; }	
		m_PLC_O_ConveryerStatus_LB = this->TransferDcodeToINT(DCodeS);	
	}
#endif//PLC_OBJ_DISABLE
	return true;		
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::PLC_ReadConveryerSignalSend(bool bCheckThread)//讀取軌道訊號
{
#ifndef PLC_OBJ_DISABLE
	int SleepTime = this->GetPLCPollingSleepTime();//不能拿掉，否則會卡住
	//----------------------------------------------------------------------------//
	const THREAD_COMMAND_MODE ToRunID = THREAD_COMMAND_TO_RUN;	
	bool  States=false;	
	int   Res  = 0;	
	int   Mask = 0;	
	int   Value = 0;
	int   DCodeID = 1040;
	char  MCodeS[32]="";
	char  DCodeS[32]="";		
	
	//軌道控制
	DCodeID = 1060;
	if( this->ReadDCode(DCodeID, DCodeS) == false ) { return false; }	
	Value = this->TransferDcodeToINT(DCodeS);
	if ( 0 == (Value&0x0001) )	{	m_PLC_O_SignalToLast_LA = false; }
	else	{	m_PLC_O_SignalToLast_LA = true; }
	if ( 0 == (Value&0x0002) )	{	m_PLC_O_SignalToNext_LA = false; }
	else	{	m_PLC_O_SignalToNext_LA = true; }
	if ( 0 == (Value&0x0004) )	{	m_PLC_O_SignalToNextNG_LA = false; }//PCB NG
	else	{	m_PLC_O_SignalToNextNG_LA = true; }
	if ( 0 == (Value&0x0008) )	{	m_ConveryerClamp_LA = false; }
	else	{	m_ConveryerClamp_LA = true; }
	//
	if ( 0 == (Value&0x0080) )	{	m_ConveryerStopBar_LA = false; }
	else	{	m_ConveryerStopBar_LA = true; }	

	if ( 0 == (Value&0x0100) )	{	m_PLC_O_SignalToLast_LB = false; }
	else	{	m_PLC_O_SignalToLast_LB = true; }
	if ( 0 == (Value&0x0200) )	{	m_PLC_O_SignalToNext_LB = false; }
	else	{	m_PLC_O_SignalToNext_LB = true; }
	if ( 0 == (Value&0x0400) )	{	m_PLC_O_SignalToNextNG_LB = false; }//PCB NG
	else	{	m_PLC_O_SignalToNextNG_LB = true; }
	if ( 0 == (Value&0x0800) )	{	m_ConveryerClamp_LB = false; }
	else	{	m_ConveryerClamp_LB = true; }
	//
	if ( 0 == (Value&0x8000) )	{	m_ConveryerStopBar_LB = false; }
	else	{	m_ConveryerStopBar_LB = true; }	

	/*
	Status = (Value&0x0001)?true:false;	m_PLC_O_PCBNeed_LA = Status;
	Status = (Value&0x0002)?true:false;	m_PLC_O_PCBGive_LA = Status;
	Status = (Value&0x0004)?true:false;	m_PLC_O_PCBRepair_LA = Status;
	Status = (Value&0x0008)?true:false;	m_PLC_O_Clamp_LA = Status;
	Status = (Value&0x0010)?true:false;	m_PLC_O_ConvCW_LA = Status;
	Status = (Value&0x0020)?true:false;	m_PLC_O_ConvCCW_LA = Status;
	Status = (Value&0x0040)?true:false;	m_PLC_O_ConvSlow_LA = Status;
	Status = (Value&0x0080)?true:false;	m_PLC_O_Stopper_LA = Status;
	Status = (Value&0x0100)?true:false;	m_PLC_O_PCBNeed_LB = Status;
	Status = (Value&0x0200)?true:false;	m_PLC_O_PCBGive_LB = Status;
	Status = (Value&0x0400)?true:false;	m_PLC_O_PCBRepair_LB = Status;
	Status = (Value&0x0800)?true:false;	m_PLC_O_Clamp_LB = Status;	//fang 1021122 怪怪的
	Status = (Value&0x1000)?true:false;	m_PLC_O_ConvCW_LB = Status;
	Status = (Value&0x2000)?true:false;	m_PLC_O_ConvCCW_LB = Status;
	Status = (Value&0x4000)?true:false;	m_PLC_O_ConvSlow_LB = Status;
	Status = (Value&0x8000)?true:false;	m_PLC_O_Stopper_LB = Status;
	*/	
	//軌道狀態-A軌道
	if ( GetConveryerEnabled_LA() )
	{
		::strcpy(MCodeS, "M330");
		if ( ReadSingle(MCodeS, States) == true )
		{	m_PLC_O_SignalToNextOK_LA = States;	}

		if ( FN_ENABLE == GetPLCParameter().m_EnableCheckPCBBargeIn )
		{
			if ( ReadSingle("M114", States) == true )
			{	m_PLC_O_PCBBargeIn_LA = States;	}
		}
	}
	
	//軌道狀態-B軌道
	if ( GetConveryerEnabled_LB() )
	{
		::strcpy(MCodeS, "M430");
		if ( ReadSingle(MCodeS, States) == true )
		{	m_PLC_O_SignalToNextOK_LB = States;	}	

		if ( FN_ENABLE == GetPLCParameter().m_EnableCheckPCBBargeIn )
		{			
			if ( ReadSingle("M214", States) == true )
			{	m_PLC_O_PCBBargeIn_LB = States;	}
		}
	}
#endif//PLC_OBJ_DISABLE
	return true;		
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::PLC_ReadPlcNodeList(bool bCheckThread)
{
#ifndef PLC_OBJ_DISABLE
	int    i = 0;	
	int    Res  = 0;
	int    Mask = 0;
	int    Value_LA = 0;
	int    Value_LB = 0;
	bool   IsLeftIn = false;
	TPlcNode *PLCNodePtr = NULL;
	const int NodeCount = (int)(this->m_PLCNodeList.size());
	const int SleepTime = this->GetPLCPollingSleepTime();//不能拿掉，否則會卡住
	const THREAD_COMMAND_MODE ToRunID = THREAD_COMMAND_TO_RUN;

	m_PLCNodeCurrentIndex ++;
	if ( (m_PLCNodeCurrentIndex<0) || (m_PLCNodeCurrentIndex>NodeCount) )
	{	m_PLCNodeCurrentIndex = 0; }	

	for ( i=m_PLCNodeCurrentIndex; i<NodeCount; i++ )
	{
		m_PLCNodeCurrentIndex = i;
		PLCNodePtr = &(m_PLCNodeList[i]);
		if ( PLCNodePtr->m_ToRead == false ) 
		{	continue; }

		if ( this->PLC_ReadNode(PLCNodePtr->m_Address, PLCNodePtr->m_Value) == false )
		{	return false; }

		IsLeftIn = !m_PLC_RightIn;

		switch ( PLCNodePtr->m_FnCode )
		{
		case PLC_VIGOR_NODE_FNC_LANE_A_DCODE://Lane A D Code
			//A軌道----------------------------------------------------------			
			break;
		case PLC_VIGOR_NODE_FNC_LANE_B_DCODE://Lane B D Code			
			//B軌道--------------------------------------------------------------				
			break;		
		}

		if ( bCheckThread == true )
		{
			if ( this->GetPLCPollingThreadCmd() != ToRunID ) { return true; }
			::Sleep(SleepTime);//不能拿掉，否則會卡住
		}
	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReturnPLCNotSupportFunc(LPCTSTR fnName)//回傳PLC未支援函式
{
	m_ErrorString.Format(_T("Error, PLC Not Support Func [%s]"), fnName);
	return false;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadLaneFault_LA()//PLC-確認軌道異常-LA
{
#ifndef PLC_OBJ_DISABLE
	int States=FN_DISABLE;
	if ( this->PLC_ReadNode("M102", States) == false ) 
	{	return false;	}

	if ( States == FN_DISABLE ) 
	{	return false;	}	
	//SetPLCExceptionCode(AOI_EXCEPTION_PLC_OTHERS);
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadLaneFault_LB()//PLC-確認軌道異常-LB
{
#ifndef PLC_OBJ_DISABLE
	int States=FN_DISABLE;
	if ( this->PLC_ReadNode("M202", States) == false ) 
	{	return false;	}

	if ( States == FN_DISABLE ) 
	{	return false;	}	
	//SetPLCExceptionCode(AOI_EXCEPTION_PLC_OTHERS);
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WritePCBBackOut_LA(bool On)//PLC-寫入PCB退出板-LA
{
#ifndef PLC_OBJ_DISABLE	
	if ( this->PLC_WriteNode("M334", On) == false ) 
	{	return false;	}
	m_PCBBackOut_LA = On;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WritePCBBackOut_LB(bool On)//PLC-寫入PCB退出板-LB
{
#ifndef PLC_OBJ_DISABLE	
	if ( this->PLC_WriteNode("M434", On) == false ) 
	{	return false;	}
	m_PCBBackOut_LB = On;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WritePCBAutoOutIn_LA(bool On)//PLC-寫入PCB自動出進-LA
{
#ifndef PLC_OBJ_DISABLE	
	if ( this->PLC_WriteNode("M307", On) == false ) 
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WritePCBAutoOutInFinish_LA(bool On)//PLC-寫入PCB自動出進完成-LA
{
#ifndef PLC_OBJ_DISABLE	
	if ( this->PLC_WriteNode("M111", On) == false ) 
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WritePCBAutoOutIn_LB(bool On)//PLC-寫入PCB自動出進-LB
{
#ifndef PLC_OBJ_DISABLE
	if ( this->PLC_WriteNode("M407", On) == false ) 
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WritePCBAutoOutInFinish_LB(bool On)//PLC-寫入PCB自動出進完成-LB
{
#ifndef PLC_OBJ_DISABLE	
	if ( this->PLC_WriteNode("M211", On) == false ) 
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WritePCBAutoBackIn_LA(bool On)//PLC-寫入PCB自動退進-LA
{
#ifndef PLC_OBJ_DISABLE
	if ( this->PLC_WriteNode("M308", On) == false ) 
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WritePCBAutoBackInFinish_LA(bool On)//PLC-寫入PCB自動退進完成-LA
{
#ifndef PLC_OBJ_DISABLE
	if ( this->PLC_WriteNode("M113", On) == false ) 
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckConveryerStopRunning_LA()//PLC-確認軌道停止運轉-LA
{
#ifndef PLC_OBJ_DISABLE	
	int OnOff=FN_DISABLE;
	if ( this->PLC_ReadNode("M724", OnOff) == false ) 
	{	return false;	}
	if ( FN_DISABLE != OnOff  ) { return false; }
	if ( this->PLC_ReadNode("M725", OnOff) == false ) 
	{	return false;	}
	if ( FN_DISABLE != OnOff  ) { return false; }
	if ( this->PLC_ReadNode("M726", OnOff) == false ) 
	{	return false;	}
	if ( FN_DISABLE != OnOff  ) { return false; }
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForConveryerStopRunning_LA()//PLC-等待軌道停止運轉-LA
{
#ifndef PLC_OBJ_DISABLE	
	int i=0;
	const int SleepTime=10;
	const int Timeout=GetPLCParameter().m_PCBInOutTimeout_LA*10;//Convert to ms
	const int MaxCnt=(2*Timeout)/SleepTime;
	for ( i=0; i<MaxCnt; i++ )
	{
		if ( this->ReadLaneFault_LA() == true )
		{
			CString str=_T("Error, Wait for Conveyer Stop Running Fault With PLC Checked (LaneA)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);		
			SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_OUT_LA);
			return false;
		}
		if ( CheckConveryerStopRunning_LA() == true )
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForConveryerStopRunning_LA"));
			return true; 
		}
		::Sleep(SleepTime);
	}
	m_ErrorString=_T("Error, WaitForConveryerStopRunning_LA Fault");
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_OUT_LA);
	return false;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecConveyorMotorRunning_LA(bool On, bool bPositive, bool bSlow)//PLC-執行軌道運轉-LA
{
	//M370正轉, M371反轉, M372慢速	
#ifndef PLC_OBJ_DISABLE	
	const char NodeStop[16]="M373";	//停止
	const char NodeForward[16]="M370";//正轉
	const char NodeBackward[16]="M371";//反轉
	const char NodeSlowDown[16]="M372";	//慢速		
	bool ConveyorMotorForward=GetConveyorMotorForward_LA();
	bool ConveyorMotorSlowDown=GetConveyorMotorSlowDown_LA();
	if ( false == On )
	{		
		const bool bUseStopNode=true;
		if ( true == bUseStopNode )
		{
			if ( this->PLC_WriteNode(NodeStop, true) == false ) 
			{	return false;	}	
			ConveyorMotorSlowDown = On;
		}
		else
		{
			if ( true == ConveyorMotorForward )
			{
				if ( this->PLC_WriteNode(NodeForward, On) == false ) 
				{	return false;	}	
				//if ( this->PLC_WriteNode(NodeBackward, On) == false ) 
				//{	return false;	}	
			}
			else
			{			
				if ( this->PLC_WriteNode(NodeBackward, On) == false ) 
				{	return false;	}	
				//if ( this->PLC_WriteNode(NodeForward, On) == false ) 
				//{	return false;	}	
			}
			if ( ConveyorMotorSlowDown != On )
			{
				if ( this->PLC_WriteNode(NodeSlowDown, On) == false ) 
				{	return false;	}	
			}
			ConveyorMotorSlowDown = On;
		}
	}
	else
	{
		const bool Off=false;		
		if ( true == bPositive )
		{
			if ( this->PLC_WriteNode(NodeBackward, Off) == false ) 
			{	return false;	}	
			if ( this->PLC_WriteNode(NodeForward, On) == false ) 
			{	return false;	}	
			ConveyorMotorForward = true;	
		}
		else
		{
			if ( this->PLC_WriteNode(NodeForward, Off) == false ) 
			{	return false;	}	
			if ( this->PLC_WriteNode(NodeBackward, On) == false ) 
			{	return false;	}	
			ConveyorMotorForward = false;
		}
		if ( bSlow != ConveyorMotorSlowDown )
		{
			if ( this->PLC_WriteNode(NodeSlowDown, bSlow) == false ) 
			{	return false;	}	
		}
		ConveyorMotorSlowDown = bSlow;		
	}
	SetConveyorMotorForward_LA(ConveyorMotorForward);
	SetConveyorMotorSlowDown_LA(ConveyorMotorSlowDown);
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WritePCBAutoBackIn_LB(bool On)//PLC-寫入PCB自動退進-LB
{
#ifndef PLC_OBJ_DISABLE
	if ( this->PLC_WriteNode("M408", On) == false ) 
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WritePCBAutoBackInFinish_LB(bool On)//PLC-寫入PCB自動退進完成-LB
{
#ifndef PLC_OBJ_DISABLE
	if ( this->PLC_WriteNode("M213", On) == false ) 
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckConveryerStopRunning_LB()//PLC-確認軌道停止運轉-LB
{
#ifndef PLC_OBJ_DISABLE		
	int OnOff=FN_DISABLE;
	if ( this->PLC_ReadNode("M732", OnOff) == false ) 
	{	return false;	}
	if ( FN_DISABLE != OnOff  ) { return false; }
	if ( this->PLC_ReadNode("M733", OnOff) == false ) 
	{	return false;	}
	if ( FN_DISABLE != OnOff  ) { return false; }
	if ( this->PLC_ReadNode("M734", OnOff) == false ) 
	{	return false;	}
	if ( FN_DISABLE != OnOff  ) { return false; }
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForConveryerStopRunning_LB()//PLC-等待軌道停止運轉-LB
{
#ifndef PLC_OBJ_DISABLE
	int i=0;
	const int SleepTime=10;
	const int Timeout=GetPLCParameter().m_PCBInOutTimeout_LB*10;//Convert to ms
	const int MaxCnt=(2*Timeout)/SleepTime;
	for ( i=0; i<MaxCnt; i++ )
	{
		if ( this->ReadLaneFault_LB() == true )
		{
			CString str=_T("Error, Wait for Conveyer Stop Running Fault With PLC Checked (LaneB)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);		
			SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_OUT_LB);
			return false;
		}
		if ( CheckConveryerStopRunning_LB() == true )
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForConveryerStopRunning_LB"));
			return true; 
		}
		::Sleep(SleepTime);
	}
	m_ErrorString=_T("Error, WaitForConveryerStopRunning_LB Fault");
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_OUT_LB);
	return false;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecConveyorMotorRunning_LB(bool On, bool bPositive, bool bSlow)//PLC-執行軌道運轉-LB
{
	//M470正轉, M471反轉, M472慢速	
#ifndef PLC_OBJ_DISABLE	
	const char NodeStop[16]="M473";	//停止
	const char NodeForward[16]="M470";//正轉
	const char NodeBackward[16]="M471";//反轉
	const char NodeSlowDown[16]="M472";	//慢速
	bool ConveyorMotorForward=GetConveyorMotorForward_LB();
	bool ConveyorMotorSlowDown=GetConveyorMotorSlowDown_LB();
	if ( false == On )
	{		
		const bool bUseStopNode=true;
		if ( true == bUseStopNode )
		{
			if ( this->PLC_WriteNode(NodeStop, true) == false ) 
			{	return false;	}	
			ConveyorMotorSlowDown = On;
		}
		else
		{
			if ( true == ConveyorMotorForward )
			{
				if ( this->PLC_WriteNode(NodeForward, On) == false ) 
				{	return false;	}	
				//if ( this->PLC_WriteNode(NodeBackward, On) == false ) 
				//{	return false;	}	
			}
			else
			{			
				if ( this->PLC_WriteNode(NodeBackward, On) == false ) 
				{	return false;	}	
				//if ( this->PLC_WriteNode(NodeForward, On) == false ) 
				//{	return false;	}	
			}
			if ( ConveyorMotorSlowDown != On )
			{
				if ( this->PLC_WriteNode(NodeSlowDown, On) == false ) 
				{	return false;	}	
			}
			ConveyorMotorSlowDown = On;
		}
	}
	else
	{
		const bool Off=false;		
		if ( true == bPositive )
		{
			if ( this->PLC_WriteNode(NodeBackward, Off) == false ) 
			{	return false;	}	
			if ( this->PLC_WriteNode(NodeForward, On) == false ) 
			{	return false;	}	
			ConveyorMotorForward = true;	
		}
		else
		{
			if ( this->PLC_WriteNode(NodeForward, Off) == false ) 
			{	return false;	}	
			if ( this->PLC_WriteNode(NodeBackward, On) == false ) 
			{	return false;	}	
			ConveyorMotorForward = false;
		}
		if ( bSlow != ConveyorMotorSlowDown )
		{
			if ( this->PLC_WriteNode(NodeSlowDown, bSlow) == false ) 
			{	return false;	}	
		}
		ConveyorMotorSlowDown = bSlow;		
	}
	SetConveyorMotorForward_LB(ConveyorMotorForward);
	SetConveyorMotorSlowDown_LB(ConveyorMotorSlowDown);
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::TurnOnOffTargetCap(bool On)//PLC-開關塊規上蓋
{
#ifndef PLC_OBJ_DISABLE	
	if ( this->PLC_WriteNode("M337", On) == false ) 
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WritePLCReset()
{	
#ifndef PLC_OBJ_DISABLE
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteBypassSafty(bool On)//關閉安全檢測
{
#ifndef PLC_OBJ_DISABLE
	if( this->PLC_WriteNode("M300", On) == false ) 
	{	return false;	}
	if( this->PLC_WriteNode("M400", On) == false ) 
	{	return false;	}
	m_SaftyBypass = On;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteDualLaneMode(bool On)//設定是否為雙軌道模式
{
#ifndef PLC_OBJ_DISABLE
	if( this->PLC_WriteNode("M322", On) == false ) 
	{	return false;	}
	m_DualLaneMode = On;//雙軌道模式	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteDualTowerLight(bool On)//設定是否為雙塔燈模式
{
#ifndef PLC_OBJ_DISABLE
	if( this->PLC_WriteNode("M325", On) == false ) 
	{	return false;	}
	SetDualTowerLight(On);//雙塔燈模式
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteTowerLightMode(int Mode)//設定是否為塔燈模式
{
#ifndef PLC_OBJ_DISABLE
	char TimeS[32]="";
	int DCode = JET_PLC_TOWER_LIGHT_MODE;
	int ValueI = Mode;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	
	m_TowerLightMode = Mode;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteTowerLightState_Stop(int Value)//設定塔燈狀態-停止
{
#ifndef PLC_OBJ_DISABLE
	if ( WriteTowerLightState_Stop(LANE_ID_A, Value) == false )
	{	return false; }
	if ( WriteTowerLightState_Stop(LANE_ID_B, Value) == false )
	{	return false; }	
	m_TowerLightState_Stop = Value;
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteTowerLightState_Stop(LANE_ID LaneID, int Value)//設定塔燈狀態-停止
{
#ifndef PLC_OBJ_DISABLE	
	char TimeS[32]="";
	int DCode = 0;
	int ValueI = Value;
	const int ValueBefore=GetTowerLightState_Stop(LaneID);
	if ( ValueBefore == Value )
	{	return true; }
	switch ( LaneID )
	{
	case LANE_ID_B:	DCode = JET_PLC_TOWER_LIGHT_NORMAL_LB;	break;
	default:
	case LANE_ID_A:	DCode = JET_PLC_TOWER_LIGHT_NORMAL_LA;	break;
	}
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }			
	SetTowerLightState_Stop(LaneID, Value);	
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteTowerLightState_Inspection(int Value)//設定塔燈狀態-檢測中
{
#ifndef PLC_OBJ_DISABLE
	char TimeS[32]="";
	int DCode = JET_PLC_TOWER_LIGHT_INSPECTION;
	int ValueI = Value;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	
	m_TowerLightState_Inspection = Value;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteTowerLightState_Bypass(int Value)//設定塔燈狀態-直通
{
#ifndef PLC_OBJ_DISABLE
	char TimeS[32]="";
	int DCode = JET_PLC_TOWER_LIGHT_BYPASS;
	int ValueI = Value;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	
	m_TowerLightState_Bypass = Value;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteTowerLightState_WaitLast(int Value)//設定塔燈狀態-等上站
{
#ifndef PLC_OBJ_DISABLE
	char TimeS[32]="";
	int DCode = JET_PLC_TOWER_LIGHT_WAIT_FOR_LAST;
	int ValueI = Value;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	
	m_TowerLightState_WaitLast = Value;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteTowerLightState_WaitNext(int Value)//設定塔燈狀態-等下站
{
#ifndef PLC_OBJ_DISABLE
	char TimeS[32]="";
	int DCode = JET_PLC_TOWER_LIGHT_WAIT_FOR_NEXT;
	int ValueI = Value;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	
	m_TowerLightState_WaitNext = Value;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteTowerLightState_PCBIn(int Value)//設定塔燈狀態-進板
{
#ifndef PLC_OBJ_DISABLE
	char TimeS[32]="";
	int DCode = JET_PLC_TOWER_LIGHT_PCB_IN;
	int ValueI = Value;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	
	m_TowerLightState_PCBIn = Value;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteTowerLightState_PCBOut(int Value)//設定塔燈狀態-出板
{
#ifndef PLC_OBJ_DISABLE
	char TimeS[32]="";
	int DCode = JET_PLC_TOWER_LIGHT_PCB_OUT;
	int ValueI = Value;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	
	m_TowerLightState_PCBOut = Value;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteTowerLightState_PCBBack(int Value)//設定塔燈狀態-退板
{
#ifndef PLC_OBJ_DISABLE
	char TimeS[32]="";
	int DCode = JET_PLC_TOWER_LIGHT_PCB_BACK;
	int ValueI = Value;
	this->TransferINTToDcodeData(ValueI, TimeS);
	if ( this->WriteDCode(DCode, TimeS) == false ) { return false; }	
	m_TowerLightState_PCBBack = Value;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteOnInspection_LA(bool On)//設定檢測中-LA
{
#ifndef PLC_OBJ_DISABLE
	if( this->PLC_WriteNode("M341", On) == false ) 
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::TurnOnConveryerLED_LA(bool On)//開啟軌道LED-LA
{
#ifndef PLC_OBJ_DISABLE
	if ( this->PLC_WriteNode("M347", On) == false ) 
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::TurnOnStageAlarm_LA()//機台異常-LA
{
#ifndef PLC_OBJ_DISABLE
	const bool On=true;
	//if ( this->PLC_WriteNode("M340", true) == false ) //會鬆板
	if ( this->PLC_WriteNode("M344", On) == false ) //不鬆板
	{	return false;	}
	m_StageAlarm_LA = On;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::TurnOffStageAlarm_LA()//機台異常-LA
{
#ifndef PLC_OBJ_DISABLE
	const bool On=false;
	//if ( this->PLC_WriteNode("M340", false) == false ) //會鬆板
	if ( this->PLC_WriteNode("M344", On) == false ) //不鬆板
	{	return false;	}
	m_StageAlarm_LA = On;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::TurnOnInspectAlarm_LA()
{
#ifndef PLC_OBJ_DISABLE
	const bool On=true;
	if( this->PLC_WriteNode("M342", On) == false ) 
	{	return false;	}
	m_InspectionAlarm_LA = On;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::TurnOffInspectAlarm_LA()
{
#ifndef PLC_OBJ_DISABLE
	const bool On=false;
	if( this->PLC_WriteNode("M342", On) == false ) 
	{	return false;	}
	m_InspectionAlarm_LA = On;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteConveryerClamp_LA(bool On)//PLC-是否夾板-LA
{
#ifndef PLC_OBJ_DISABLE		
	if ( true == On )
	{
		SavePLCMovingTimeMsg(_T("CPLC_Vigor::WriteConveryerClamp_LA-On"));
		if( this->PLC_WriteNode("M313", true) == false ) 
		{	return false;	}
	}
	else
	{
		SavePLCMovingTimeMsg(_T("CPLC_Vigor::WriteConveryerClamp_LA-Off"));
		if( this->PLC_WriteNode("M380", true) == false ) 
		{	return false;	}
	}
	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_A);
	m_ConveryerClamp_LA = On;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteConveryerStopBar_LA(bool On)//PLC-停板器-LA
{
#ifndef PLC_OBJ_DISABLE
	if( this->PLC_WriteNode("M314", On) == false ) 
	{	return false;	}
	m_ConveryerStopBar_LA = On;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
int  CPLC_Vigor::ReadLanePLCErrorCode_LA()//讀取PLC的軌道錯誤代碼-LA
{
#ifndef PLC_OBJ_DISABLE
	char Data[32]="";		
	const int DCodeNo = 4000;//D4000
	if ( this->ReadDCode(DCodeNo, Data) == false )
	{	return 0;	}
	int ValueI = this->TransferDcodeToINT(Data);	
	return ValueI;	
#endif//PLC_OBJ_DISABLE
	return 0;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBIn_LA(bool bWait)//PLC-進板-LA
{
#ifndef PLC_OBJ_DISABLE
	CString str;
	m_PCBInCheckCount_LA = 0;
	m_PCBInTimeStart_LA = ::GetTickCount();//進板時間結束	
	m_PCBInTimeEnd_LA = m_PCBInTimeStart_LA;//進板時間結束	
	const TPLCParameter &Param = GetPLCParameter();

	if ( PLC_ReadConveryerSensor(false) == false )
	{	return false; }
	if ( FN_ENABLE == Param.m_CheckPCBInsideBeforePCBIn )
	{	
		if ( GetConveryerSensorPCBSlow_LA() == true ||
			 GetConveryerSensorPCBStop_LA() == true ||
			 GetConveryerSensorPCBOut_LA() == true )
		{
			str = _T("Error, there is one board on the conveyer LA");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			return false;
		}
	}
	else
	{
		if ( GetConveryerSensorPCBSlow_LA() == true )
		{
			str = _T("Error, there is one board on the conveyer LA");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			return false;
		}
	}

	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBIn_LA"));
	if( this->PLC_WriteNode("M301", true) == false ) 
	{ 
		m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束		
		return false; 
	}	
	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_A);
	if ( bWait == false ) { return true; }
	return this->WaitForPCBInFinish_LA();
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBInFinish_LA()//PLC-等進板完成-LA
{
#ifndef PLC_OBJ_DISABLE	
	int i=0;
	CString str;	
	bool States = true;
	const int MaxCounts = this->GetPCBInOutTimeout_LA()+100;	
	for ( i=0; i<MaxCounts; i++ )
	{		
		this->m_PCBInCheckCount_LA = i;
		//讀取是否進板完成
		if ( this->CheckPCBInFinish_LA() == true )
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBInFinish_LA"));
			return true;	
		}

		if ( this->CheckPCBInFault_LA() == true )
		{	
			str = _T("Error, PCB-In Fault With PLC Checked (LaneA)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LA();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}		
		::Sleep(10);
	}
	str = _T("Error, PCB-In Fault With Timeout (LaneA)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_LA);
	m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束	
	return false;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBInFinish_LA()//PLC-進板完成-LA
{
#ifndef PLC_OBJ_DISABLE
	//讀取是否到位
	int States=FN_DISABLE;	
	const char PlcNode[32] ="M105";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}

	if ( States == FN_DISABLE ) 
	{	return false; }

	m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束	
	this->PLC_WriteNode(PlcNode, FN_DISABLE);
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBInFault_LA()//PLC-進板失敗-LA
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LA();
	if ( true == bLaneFault )
	{
		m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束
		CString str = _T("Error, PCB In Fault (LaneA)");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_LA);
	}
	return bLaneFault;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBIn2nd_LA(bool bWait)//PLC-第2段進板-LA
{
#ifndef PLC_OBJ_DISABLE
	m_PCBInCheckCount_LA = 0;
	m_PCBInTimeStart_LA = ::GetTickCount();//進板時間結束	
	m_PCBInTimeEnd_LA = m_PCBInTimeStart_LA;//進板時間結束	

	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBIn2nd_LA"));
	if( this->PLC_WriteNode("M309", true) == false ) 
	{ 
		m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束		
		return false; 
	}	
	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_A);
	if ( bWait == false ) { return true; }
	return this->WaitForPCBIn2ndFinish_LA();
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBIn2ndFinish_LA()//PLC-第2段等進板完成-LA
{
#ifndef PLC_OBJ_DISABLE	
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = this->GetPCBInOutTimeout_LA()+100;	
	for ( i=0; i<MaxCounts; i++ )
	{		
		this->m_PCBInCheckCount_LA = i;
		//讀取是否進板完成
		if ( this->CheckPCBIn2ndFinish_LA() == true )
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBIn2ndFinish_LA"));
			return true;	
		}

		if ( this->CheckPCBIn2ndFault_LA() == true )
		{
			str = _T("Error, PCB-2nd-In Fault With PLC Checked (LaneA)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);		
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LA();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}		
		::Sleep(10);
	}
	str = _T("Error, PCB-2nd-In Fault With Timeout (LaneA)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_LA);
	m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束	
	return false;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBIn2ndFinish_LA()//PLC-第2段進板完成-LA
{
#ifndef PLC_OBJ_DISABLE
	//讀取是否到位
	int States=FN_DISABLE;	
	const char PlcNode[32] ="M112";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}

	if ( States == FN_DISABLE ) 
	{	return false; }

	m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束			
	this->PLC_WriteNode(PlcNode, FN_DISABLE);
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBIn2ndFault_LA()//PLC-第2段進板失敗-LA	
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LA();
	if ( true == bLaneFault )
	{
		m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束			
		CString str = _T("Error, PCB-2nd-In Fault (LaneA)");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_LA);
	}
	return bLaneFault;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBIn3rd_LA(bool bWait)//PLC-第3段進板-LA
{
#ifndef PLC_OBJ_DISABLE
	m_PCBInCheckCount_LA = 0;
	m_PCBInTimeStart_LA = ::GetTickCount();//進板時間結束	
	m_PCBInTimeEnd_LA = m_PCBInTimeStart_LA;//進板時間結束	

	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBIn3rd_LA"));
	if( this->PLC_WriteNode("M350", true) == false ) 
	{ 
		m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束		
		return false; 
	}	
	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_A);
	if ( bWait == false ) { return true; }
	return this->WaitForPCBIn3rdFinish_LA();
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBIn3rdFinish_LA()//PLC-第3段等進板完成-LA
{
#ifndef PLC_OBJ_DISABLE	
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = this->GetPCBInOutTimeout_LA()+100;	
	for ( i=0; i<MaxCounts; i++ )
	{		
		this->m_PCBInCheckCount_LA = i;
		//讀取是否進板完成
		if ( this->CheckPCBIn3rdFinish_LA() == true )
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBIn3rdFinish_LA"));
			return true;	
		}

		if ( this->CheckPCBIn3rdFault_LA() == true )
		{
			str = _T("Error, PCB-3rd-In Fault With PLC Checked (LaneA)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);				
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LA();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}		
		::Sleep(10);
	}
	str = _T("Error, PCB-3rd-In Fault With Timeout (LaneA)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_LA);
	m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束	
	return false;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBIn3rdFinish_LA()//PLC-第3段進板完成-LA
{
#ifndef PLC_OBJ_DISABLE
	//讀取是否到位
	int States=FN_DISABLE;	
	const char PlcNode[32] ="M150";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}

	if ( States == FN_DISABLE ) 
	{	return false; }

	m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束			
	this->PLC_WriteNode(PlcNode, FN_DISABLE);
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBIn3rdFault_LA()//PLC-第3段進板失敗-LA
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LA();
	if ( true == bLaneFault )
	{
		m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束			
		CString str = _T("Error, PCB-3rd-In Fault (LaneA)");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_LA);
	}
	return bLaneFault;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBBack_LA(bool bWait)////PLC-退板-LA
{	
#ifndef PLC_OBJ_DISABLE
	const bool PCBBackOunt=m_PCBBackOut_LA;
	this->m_PCBBackCheckCount_LA = 0;		
	this->m_PCBBackTimeStart_LA = ::GetTickCount();//進板時間結束	
	this->m_PCBBackTimeEnd_LA = this->m_PCBBackTimeStart_LA;
	if ( true == PCBBackOunt )
	{	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBBackOut_LA"));	}
	else
	{	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBBack_LA"));	}
	if ( this->PLC_WriteNode("M303", true) == false ) 	
	{		
		this->m_PCBBackTimeEnd_LA = ::GetTickCount();//進板時間結束
		return false;
	}
	if ( bWait == false ) { return true; }
	return this->WaitForPCBBackFinish_LA();
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBBackFinish_LA()//PLC-等退板完成-LA
{
#ifndef PLC_OBJ_DISABLE	
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = 1000;
	const bool PCBBackOunt=m_PCBBackOut_LA;
	for ( i=0; i<MaxCounts; i++ )
	{
		m_PCBBackCheckCount_LA = i;		
		//讀取是否到位
		if ( this->CheckPCBBackFinish_LA() == true )
		{
			if ( true == PCBBackOunt )
			{	SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBBackOutFinish_LA"));		}
			else
			{	SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBBackFinish_LA"));	}
			return true;	
		}

		if ( this->CheckPCBBackFault_LA() == true )
		{
			if ( true == PCBBackOunt )
			{	str = _T("Error, PCB-Back-Out Fault With PLC Checked (LaneA)");	}
			else
			{	str = _T("Error, PCB-Back Fault With PLC Checked (LaneA)");	}
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);		
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LA();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}
		::Sleep(10);		
	}
	if ( true == PCBBackOunt )
	{	
		WritePCBBackOut_LA(false);
		str = _T("Error, PCB-Back-Out Fault With Timeout (LaneA)");	
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_BACK_OUT_LA);				
	}
	else
	{
		str = _T("Error, PCB-Back Fault With Timeout (LaneA)");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_BACK_LA);
	}
	this->m_PCBBackTimeEnd_LA = ::GetTickCount();//進板時間結束		
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBBackFinish_LA()//PLC-退板完成-LA
{
#ifndef PLC_OBJ_DISABLE
	//讀取是否到位
	int States=FN_DISABLE;
	const char PlcNode[32] = "M107";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}

	if ( States == FN_DISABLE ) 
	{	return false; }	

	if ( true == m_PCBBackOut_LA )
	{	WritePCBBackOut_LA(false);	}
	m_PCBBackTimeEnd_LA = ::GetTickCount();
	this->PLC_WriteNode(PlcNode, FN_DISABLE);
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBBackFault_LA()//PLC-退板失敗-LA
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LA();
	if ( true == bLaneFault )
	{
		if ( true == m_PCBBackOut_LA )
		{
			WritePCBBackOut_LA(false);
			m_PCBBackTimeEnd_LA = ::GetTickCount();
			CString str = _T("Error, PCB Back-Out Fault (LaneA)");		
			m_ErrorString = LoadMultiLanguageString(str, str);
			SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_BACK_OUT_LA);			
		}
		else
		{
			m_PCBBackTimeEnd_LA = ::GetTickCount();
			CString str = _T("Error, PCB Back Fault (LaneA)");
			m_ErrorString = LoadMultiLanguageString(str, str);
			SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_BACK_LA);
		}
	}
	return bLaneFault;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBBackOut_LA(bool bWait)//PLC-退出板-LA
{
#ifndef PLC_OBJ_DISABLE	
	if ( WritePCBBackOut_LA(true) == false )	
	{	return false;	}
	return ExecPCBBack_LA(bWait);		
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBBackOutFinish_LA()//PLC-等退出板完成-LA
{
	return WaitForPCBBackFinish_LA();	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBBackOutFinish_LA()//PLC-退出板完成-LA
{
	return CheckPCBBackFinish_LA();	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBBackOutFault_LA()//PLC-退出板失敗-LA	
{
	return CheckPCBBackFault_LA();	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBOut_LA(bool bWait)//PLC-出板-LA
{
#ifndef PLC_OBJ_DISABLE
	this->m_PCBOutCheckCount_LA = 0;
	this->m_PCBOutTimeStart_LA = ::GetTickCount();//進板時間結束	
	this->m_PCBOutTimeEnd_LA = this->m_PCBOutTimeStart_LA;

	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBOut_LA"));
	if ( this->PLC_WriteNode("M302", true) == false ) 
	{
		this->m_PCBOutTimeEnd_LA = ::GetTickCount();//進板時間結束
		return false;
	}
	if ( bWait == false ) { return true; }
	return this->WaitForPCBOutFinish_LA();	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBOutFinish_LA()//PLC-等出板完成-LA
{	
#ifndef PLC_OBJ_DISABLE
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = this->GetPCBInOutTimeout_LA()+100;
	for ( i=0; i<MaxCounts; i++ )
	{
		//讀取是否到位		
		if ( this->CheckPCBOutFault_LA() == true )
		{
			str = _T("Error, PCB-Out Fault With PLC Checked (LaneA)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);			
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LA();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}

		if ( this->CheckPCBOutFinish_LA() == true ) 
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBOutFinish_LA"));
			return true;	
		}
		::Sleep(10);		
	}
	str = _T("Error, PCB-Out Fault With Timeout (LaneA)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_OUT_LA);
	this->m_PCBOutTimeEnd_LA = ::GetTickCount();//進板時間結束		
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBOutFinish_LA()//PLC-出板完成-LA
{
#ifndef PLC_OBJ_DISABLE	
	int States=FN_DISABLE;
	const char PlcNode[32] = "M106";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}

	if ( States == FN_DISABLE ) 
	{	return false; }	

	m_PCBOutTimeEnd_LA = ::GetTickCount();
	this->PLC_WriteNode(PlcNode, FN_DISABLE);	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBOutFault_LA()//PLC-出板失敗-LA
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LA();
	if ( true == bLaneFault )
	{	
		m_PCBOutTimeEnd_LA = ::GetTickCount();
		CString str = _T("Error, PCB Out Fault (LaneA)");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_OUT_LA);
	}
	return bLaneFault;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBOutInside_LA(bool bWait)//PLC-出板至機台側邊-LA
{
#ifndef PLC_OBJ_DISABLE
	this->m_PCBOutCheckCount_LA = 0;	
	this->m_PCBOutTimeStart_LA = ::GetTickCount();//進板時間結束	
	this->m_PCBOutTimeEnd_LA = this->m_PCBOutTimeStart_LA;//進板時間結束		

	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBOutInside_LA"));
	if ( this->PLC_WriteNode("M306", true) == false ) 
	{		
		this->m_PCBOutTimeEnd_LA = ::GetTickCount();//進板時間結束				
		return false;
	}
	if ( bWait == false ) { return true; }
	return this->WaitForPCBOutInsideFinish_LA();	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBOutInsideFinish_LA()//PLC-等待出板至機台側邊-LA
{		
#ifndef PLC_OBJ_DISABLE
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = this->GetPCBInOutTimeout_LA()+100;
	for ( i=0; i<MaxCounts; i++ )
	{
		this->m_PCBOutCheckCount_LA = i;		
		if ( this->CheckPCBOutInsideFault_LA() == true )
		{
			str = _T("Error, PCB-Out-Stop Fault With PLC Checked (LaneA)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LA();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			this->m_PCBOutTimeEnd_LA = ::GetTickCount();//進板時間結束			
			return false;
		}
		//讀取是否到位		
		if ( this->CheckPCBOutInsideFinish_LA() == true ) 
		{			
			this->m_PCBOutTimeEnd_LA = ::GetTickCount();//進板時間結束		
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBOutInsideFinish_LA"));
			return true;	
		}
		::Sleep(10);		
	}
	str = _T("Error, PCB-Out-Stop Fault With Timeout (LaneA)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_OUT_LA);
	this->m_PCBOutTimeEnd_LA = ::GetTickCount();//進板時間結束	
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBOutInsideFinish_LA()//PLC-出板至機台側邊完成-LA
{
#ifndef PLC_OBJ_DISABLE
	int States=FN_DISABLE;
	const char PlcNode[32] = "M110";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}
	if ( States == FN_DISABLE ) 
	{	return false;	}
	this->PLC_WriteNode(PlcNode, FN_DISABLE);	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBOutInsideFault_LA()//PLC-出板至機台側邊失敗-LA
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LA();
	if ( true == bLaneFault )
	{		
		CString str = _T("Error, PCB Out-Inside Fault (LaneA)");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_OUT_LA);
	}
	return bLaneFault;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBOutIn_LA(bool bWait)//PLC-出板+進板-LA
{
#ifndef PLC_OBJ_DISABLE
	m_PCBOutCheckCount_LA = 0;	
	m_PCBInTimeStart_LA = ::GetTickCount();//進板時間結束	
	m_PCBOutTimeStart_LA = m_PCBInTimeStart_LA;//進板時間結束	
	m_PCBInTimeEnd_LA = m_PCBInTimeStart_LA;//進板時間結束		
	m_PCBOutTimeEnd_LA = m_PCBOutTimeStart_LA;//進板時間結束

	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBOutIn_LA"));
	if ( this->PLC_WriteNode("M305", true) == false ) 
	{		
		m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束				
		m_PCBOutTimeEnd_LA = m_PCBInTimeEnd_LA;//進板時間結束				
		return false;
	}
	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_A);
	if ( bWait == false ) { return true; }
	return this->WaitForPCBOutInFinish_LA();	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBOutInFinish_LA()//PLC-等待出板+進板-LA
{
#ifndef PLC_OBJ_DISABLE
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = this->GetPCBInOutTimeout_LA()+100;
	for ( i=0; i<MaxCounts; i++ )
	{
		this->m_PCBInCheckCount_LA = i;
		this->m_PCBOutCheckCount_LA = i;

		//讀取是否到位		
		if ( this->CheckPCBOutInFault_LA() == true )
		{
			str = _T("Error, PCB-Out-In Fault With PLC Checked (LaneA)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);				
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LA();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}

		if ( this->CheckPCBOutInFinish_LA() == true ) 
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBOutInFinish_LA"));
			return true;	
		}
		::Sleep(10);		
	}
	str = _T("Error, PCB-Out-In Fault With Timeout (LaneA)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_OUT_IN_LA);
	m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束	
	m_PCBOutTimeEnd_LA = m_PCBInTimeEnd_LA;//進板時間結束	
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBOutInFinish_LA()//PLC-出板+進板完成-LA
{
#ifndef PLC_OBJ_DISABLE
	int States=FN_DISABLE;
	const char PlcNode[32] = "M109";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}
	if ( States == FN_DISABLE ) 
	{	return false;	}

	m_PCBInTimeEnd_LA = ::GetTickCount();
	m_PCBOutTimeEnd_LA = m_PCBInTimeEnd_LA;

	//做個虛擬中間時間
	DWORD HalfTime = ((m_PCBInTimeEnd_LA-m_PCBOutTimeStart_LA)/2)+m_PCBOutTimeStart_LA;
	m_PCBInTimeStart_LA = m_PCBOutTimeEnd_LA = HalfTime;

	this->PLC_WriteNode(PlcNode, FN_DISABLE);	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBOutInFault_LA()//PLC-出板+進板失敗-LA
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LA();
	if ( true == bLaneFault )
	{		
		m_PCBInTimeEnd_LA = ::GetTickCount();
		m_PCBOutTimeEnd_LA = m_PCBInTimeEnd_LA;
		CString str = _T("Error, PCB Out-In Fault (LaneA)");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_OUT_IN_LA);
	}
	return bLaneFault;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBReIn_LA(bool bWait)//PLC-重新進板-LA
{
#ifndef PLC_OBJ_DISABLE
	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBReIn_LA"));
	if ( this->PLC_WriteNode("M304", true) == false ) 
	{	return false;	}
	
	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_A);
	if ( bWait == false ) { return true; }
	return this->WaitForPCBReInFinish_LA();
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBReInFinish_LA()//PLC-等待重新進板完成-LA
{	
#ifndef PLC_OBJ_DISABLE
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = 300;
	for ( i=0; i<MaxCounts; i++ )
	{		
		//讀取是否進板完成
		if ( this->CheckPCBReInFinish_LA() == true )
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBReInFinish_LA"));
			return true;	
		}

		if ( this->CheckPCBInFault_LA() == true )
		{
			str = _T("Error, PCB Re-In Fault With PLC Checked (LaneA)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LA();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}		
		::Sleep(10);		
	}
	str = _T("Error, PCB Re-In Fault With Timeout (LaneA)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_LA);
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBReInFinish_LA()//PLC-重新進板完成-LA
{
#ifndef PLC_OBJ_DISABLE
	int States=FN_DISABLE;
	const char PlcNode[32] = "M108";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}
	if ( States == FN_DISABLE ) 
	{	return false;	}
	this->PLC_WriteNode(PlcNode, FN_DISABLE);	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBClear_LA(bool bWait)//PLC-清板板-LA
{
#ifndef PLC_OBJ_DISABLE
	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBClear_LA"));
	if ( this->PLC_WriteNode("M353", true) == false ) 
	{	return false;	}
	
	if ( bWait == false ) { return true; }
	return this->WaitForPCBClearFinish_LA();
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBClearFinish_LA()//PLC-確認清板完成-LA
{
#ifndef PLC_OBJ_DISABLE
	//讀取清板完成
	int States=FN_DISABLE;	
	const char PlcNode[32] ="M153";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}

	if ( States == FN_DISABLE ) 
	{	return false; }
	this->PLC_WriteNode(PlcNode, FN_DISABLE);
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBClearFinish_LA()//PLC-等待清板完成-LA
{
#ifndef PLC_OBJ_DISABLE
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = 3000;
	for ( i=0; i<MaxCounts; i++ )
	{		
		//讀取是否清板完成
		if ( this->CheckPCBClearFinish_LA() == true )
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBClearFinish_LA"));
			return true;	
		}
		::Sleep(10);		
	}
	str = _T("Error, PCB Clear Fault With Timeout (LaneA)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_CLEAR_LA);
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::StopPCBAutoOutIn_LA(bool bWait)//PLC-停止自動進出板-LA
{
	if ( WritePCBAutoOutIn_LA(false) == false )
	{	return false; }
	return true;

	if ( true == bWait )
	{	return WaitForPCBAutoOutInFinish_LA();	}
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBAutoOutIn_LA(bool On, bool bWait)//PLC-自動出進板-LA
{	
#ifndef PLC_OBJ_DISABLE	
	m_PCBOutCheckCount_LA = 0;	
	m_PCBInTimeStart_LA = ::GetTickCount();	
	m_PCBOutTimeStart_LA = m_PCBInTimeStart_LA;	
	m_PCBInTimeEnd_LA = m_PCBInTimeStart_LA;		
	m_PCBOutTimeEnd_LA = m_PCBOutTimeStart_LA;		
	
	SetPCBAutoRunPCBChaned_LA(false);
	if ( WritePCBAutoOutInFinish_LA(false) == false )
	{	return false; }

	SetPCBAutoRunMode_LA(PLC_PCB_AUTO_RUN_OUT_IN);	
	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBAutoOutIn_LA"));
	if ( WritePCBAutoOutIn_LA(On) == false )
	{	return false; }

	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_A);
	if ( true == bWait )
	{	return WaitForPCBAutoOutInFinish_LA();	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBAutoOutInFinish_LA()//PLC-等待自動出進板-LA
{
#ifndef PLC_OBJ_DISABLE
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = this->GetPCBInOutTimeout_LA()+100;	
	for ( i=0; i<MaxCounts; i++ )
	{		
		//讀取是否進板完成
		if ( this->CheckPCBAutoOutInFinish_LA() == true )
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBAutoOutInFinish_LA"));
			return true;	
		}

		if ( this->CheckPCBAutoOutInFault_LA() == true )
		{
			str = _T("Error, PCB Auto Out-In Fault With PLC Checked (LaneA)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LA();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}		
		::Sleep(10);		
	}
	str = _T("Error, PCB Auto Out-In Fault With Timeout (LaneA)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_OUT_LA);
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadPCBAutoOutInFinish_LA()//PLC-自動出進板完成-LA
{
#ifndef PLC_OBJ_DISABLE
	//讀取是否到位
	int States=FN_DISABLE;	
	const char PlcNode[32] ="M111";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}
	if ( States == FN_DISABLE ) 
	{	return false; }	
	SetPCBAutoRunPCBChaned_LA(true);
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBAutoOutInFinish_LA()//PLC-自動出進板完成-LA
{
#ifndef PLC_OBJ_DISABLE
	if ( ReadPCBAutoOutInFinish_LA() == false )
	{	return false; }

	m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束
	m_PCBOutTimeEnd_LA = m_PCBInTimeEnd_LA;//進板時間結束	

	//做個虛擬中間時間
	DWORD HalfTime = ((m_PCBInTimeEnd_LA-m_PCBOutTimeStart_LA)/2)+m_PCBOutTimeStart_LA;
	m_PCBInTimeStart_LA = m_PCBOutTimeEnd_LA = HalfTime;

	//this->PLC_WriteNode(PlcNode, FN_DISABLE);
	WritePCBAutoOutIn_LA(false);		
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBAutoOutInFault_LA()//PLC-自動出進板失敗-LA
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LA();
	if ( true == bLaneFault )
	{
		WritePCBAutoOutIn_LA(false);
		CString str = _T("Error, PCB Auto Out-In Fault (LaneA)");
		m_ErrorString = LoadMultiLanguageString(str, str);		
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_OUT_LA);
	}
	return bLaneFault;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::StopPCBAutoBackIn_LA(bool bWait)//PLC-停止自動退進板-LA
{
	if ( this->WritePCBAutoBackIn_LA(false) == false ) 
	{	return false;	}
	return true;

	if ( true == bWait )
	{	return WaitForPCBAutoBackInFinish_LA();	}
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBAutoBackIn_LA(bool On, bool bWait)//PLC-自動退進板-LA
{
#ifndef PLC_OBJ_DISABLE
	m_PCBOutCheckCount_LA = 0;	
	m_PCBInTimeStart_LA = ::GetTickCount();	
	m_PCBBackTimeStart_LA = m_PCBInTimeStart_LA;
	m_PCBInTimeEnd_LA = m_PCBInTimeStart_LA;	
	m_PCBBackTimeEnd_LA = m_PCBBackTimeStart_LA;

	SetPCBAutoRunPCBChaned_LA(false);
	if ( WritePCBAutoBackInFinish_LA(false) == false )
	{	return false; }

	SetPCBAutoRunMode_LA(PLC_PCB_AUTO_RUN_BACK_IN);	
	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBAutoBackIn_LA"));
	if ( this->WritePCBAutoBackIn_LA(On) == false ) 
	{	return false;	}

	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_A);
	if ( true == bWait )
	{	return WaitForPCBAutoBackInFinish_LA();	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBAutoBackInFinish_LA()//PLC-等待自動退進板-LA
{
#ifndef PLC_OBJ_DISABLE
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = this->GetPCBInOutTimeout_LA()+100;	
	for ( i=0; i<MaxCounts; i++ )
	{		
		//讀取是否進板完成
		if ( this->CheckPCBAutoBackInFinish_LA() == true )
		{	
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBAutoBackInFinish_LA"));
			return true;	
		}

		if ( this->CheckPCBAutoBackInFault_LA() == true )
		{
			str = _T("Error, PCB Auto Back-In Fault With PLC Checked (LaneA)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LA();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}		
		::Sleep(10);		
	}
	str = _T("Error, PCB Auto Back-In Fault With Timeout (LaneA)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_BACK_LA);
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadPCBAutoBackInFinish_LA()//PLC-自動退進板完成-LA
{
#ifndef PLC_OBJ_DISABLE
	//讀取是否到位
	int States=FN_DISABLE;	
	const char PlcNode[32] ="M113";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}
	if ( States == FN_DISABLE ) 
	{	return false; }
	SetPCBAutoRunPCBChaned_LA(true);	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBAutoBackInFinish_LA()//PLC-自動退進板完成-LA
{
#ifndef PLC_OBJ_DISABLE
	if ( ReadPCBAutoBackInFinish_LA() == false ) { return false; }

	m_PCBInTimeEnd_LA = ::GetTickCount();//進板時間結束
	m_PCBBackTimeEnd_LA = m_PCBInTimeEnd_LA;//進板時間結束	

	//做個虛擬中間時間
	DWORD HalfTime = ((m_PCBInTimeEnd_LA-m_PCBBackTimeStart_LA)/2)+m_PCBBackTimeStart_LA;
	m_PCBInTimeStart_LA = m_PCBBackTimeEnd_LA = HalfTime;

	//this->PLC_WriteNode(PlcNode, FN_DISABLE);
	WritePCBAutoBackIn_LA(false);	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBAutoBackInFault_LA()//PLC-自動退進板失敗-LA
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LA();
	if ( true == bLaneFault )
	{
		WritePCBAutoBackIn_LA(false);
		CString str = _T("Error, PCB Auto Back-In Fault (LaneA)");
		m_ErrorString = LoadMultiLanguageString(str, str);		
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_BACK_LA);
	}
	return bLaneFault;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteSignalToLast_LA(bool On)//PLC-送訊號給上一站-LA
{	
#ifndef PLC_OBJ_DISABLE
	if ( this->PLC_WriteNode("M310", On) == false ) 
	{	return false;	}		
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteSignalToNext_LA(bool On)//PLC-送訊號給下一站-LA
{
#ifndef PLC_OBJ_DISABLE
	if ( this->PLC_WriteNode("M311", On) == false ) 
	{	return false;	}		
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteOKSignalToNext_LA(bool On)//PLC-送OK訊號給下一站-LA
{
#ifndef PLC_OBJ_DISABLE
	if ( this->PLC_WriteNode("M330", On) == false ) 
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteNGSignalToNext_LA(bool On)//PLC-送NG訊號給下一站-LA
{
#ifndef PLC_OBJ_DISABLE
	if ( this->PLC_WriteNode("M312", On) == false ) 
	{	return false;	}	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLockSignal_LA(bool On)//PLC-送鎖住訊號-LA
{
#ifndef PLC_OBJ_DISABLE
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteConveyerSensorPower_LA(bool On, bool bForce)//PLC-關閉軌道感應器電源-LA
{
#ifndef PLC_OBJ_DISABLE	
	bool Off=!On;//注意是反向的邏輯
	if ( false == bForce )
	{
		if ( On == GetConveryerSensorPower_LA() )
		{	return true; }
	}
	if ( true == On )
	{	SavePLCMovingTimeMsg(_T("CPLC_Vigor::WriteConveyerSensorPower_LA-On"));	}
	else
	{	SavePLCMovingTimeMsg(_T("CPLC_Vigor::WriteConveyerSensorPower_LA-Off"));	}
	if ( this->PLC_WriteNode("M345", Off) == false ) 
	{	return false;	}	
	SetConveryerSensorPower_LA(On);
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::TurnOffConveyerSensorPower_LA(bool On)//PLC-關閉軌道感應器電源-LA
{	
#ifndef PLC_OBJ_DISABLE
	if ( FN_DISABLE == GetPLCParameter().m_TurnOffConveyerSensorPower_LA ) { return true; }
	const bool Off=!On;
	if ( WriteConveyerSensorPower_LA(Off, false) == false )
	{	return false; }
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteOnInspection_LB(bool On)//設定檢測中-LB
{
#ifndef PLC_OBJ_DISABLE
	if ( this->PLC_WriteNode("M441", On) == false ) 
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::TurnOnConveryerLED_LB(bool On)//開啟軌道LED-LB
{	
#ifndef PLC_OBJ_DISABLE	
	return ReturnPLCNotSupportFunc(_T("TurnOnConveryerLED_LB"));
	//if ( this->PLC_WriteNode("M347", On) == false ) 
	//{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::TurnOnStageAlarm_LB()//機台異常-LB
{
#ifndef PLC_OBJ_DISABLE	
	const bool On=true;
	//if ( this->PLC_WriteNode("M440", true) == false ) //會鬆板
	if ( this->PLC_WriteNode("M444", On) == false ) //不鬆板
	{	return false;	}
	m_StageAlarm_LB = On;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::TurnOffStageAlarm_LB()//機台異常-LB
{
#ifndef PLC_OBJ_DISABLE
	const bool On=false;
	//if ( this->PLC_WriteNode("M440", false) == false ) //會鬆板
	if ( this->PLC_WriteNode("M444", On) == false ) //不鬆板
	{	return false;	}
	m_StageAlarm_LB = On;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::TurnOnInspectAlarm_LB()
{
#ifndef PLC_OBJ_DISABLE
	const bool On=true;
	if( this->PLC_WriteNode("M442", On) == false ) 
	{	return false;	}
	m_InspectionAlarm_LB = On;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::TurnOffInspectAlarm_LB()
{
#ifndef PLC_OBJ_DISABLE
	const bool On=false;
	if( this->PLC_WriteNode("M442", On) == false ) 
	{	return false;	}
	m_InspectionAlarm_LB = On;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteConveryerClamp_LB(bool On)//PLC-是否夾板-LB
{
#ifndef PLC_OBJ_DISABLE	
	if ( true == On ) 
	{
		SavePLCMovingTimeMsg(_T("CPLC_Vigor::WriteConveryerClamp_LB-On"));
		if( this->PLC_WriteNode("M413", true) == false ) 
		{	return false;	}	
	}
	else
	{
		SavePLCMovingTimeMsg(_T("CPLC_Vigor::WriteConveryerClamp_LB-Off"));
		if( this->PLC_WriteNode("M480", true) == false ) 
		{	return false;	}
	}
	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_B);
	m_ConveryerClamp_LB = On;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteConveryerStopBar_LB(bool On)//PLC-停板器-LB
{
#ifndef PLC_OBJ_DISABLE
	if( this->PLC_WriteNode("M414", On) == false ) 
	{	return false;	}
	m_ConveryerStopBar_LB = On;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
int  CPLC_Vigor::ReadLanePLCErrorCode_LB()//讀取PLC的軌道錯誤代碼-LB
{
#ifndef PLC_OBJ_DISABLE
	char Data[32]="";		
	const int DCodeNo = 4000;//D4000
	if ( this->ReadDCode(DCodeNo, Data) == false )
	{	return 0;	}
	int ValueI = this->TransferDcodeToINT(Data);	
	return ValueI;	
#endif//PLC_OBJ_DISABLE
	return 0;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBIn_LB(bool bWait)//PLC-進板-LB
{
#ifndef PLC_OBJ_DISABLE
	CString str;
	m_PCBInCheckCount_LB = 0;
	m_PCBInTimeStart_LB = ::GetTickCount();//進板時間結束
	m_PCBInTimeEnd_LB = m_PCBInTimeStart_LB;//進板時間結束
	const TPLCParameter &Param = GetPLCParameter();

	if ( PLC_ReadConveryerSensor(false) == false )
	{	return false; }
	if ( FN_ENABLE == Param.m_CheckPCBInsideBeforePCBIn )
	{	
		if ( GetConveryerSensorPCBSlow_LB() == true ||
			 GetConveryerSensorPCBStop_LB() == true ||
			 GetConveryerSensorPCBOut_LB() == true )
		{
			str = _T("Error, there is one board on the conveyer LB");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			return false;
		}
	}
	else
	{
		if ( GetConveryerSensorPCBSlow_LB() == true )
		{
			str = _T("Error, there is one board on the conveyer LB");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			return false;
		}
	}

	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBIn_LB"));
	if ( this->PLC_WriteNode("M401", true) == false ) 
	{
		m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束
		return false;
	}
	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_B);
	if ( bWait == false ) { return true; }
	return this->WaitForPCBInFinish_LB();	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBInFinish_LB()//PLC-等進板完成-LB
{
#ifndef PLC_OBJ_DISABLE
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = this->GetPCBInOutTimeout_LB()+100;	
	for ( i=0; i<MaxCounts; i++ )
	{	
		this->m_PCBInCheckCount_LB = i;
		//讀取是否進板完成
		if ( this->CheckPCBInFinish_LB() == true )
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBInFinish_LB"));
			return true;	
		}

		if ( this->CheckPCBInFault_LB() == true )
		{
			str = _T("Error, PCB-In Fault With PLC Checked (LaneB)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LB();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}		
		::Sleep(10);		
	}
	str = _T("Error, PCB-In Fault With Timeout (LaneB)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_LB);
	m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束	
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBInFinish_LB()//PLC-進板完成-LB
{
#ifndef PLC_OBJ_DISABLE
	//讀取是否到位
	int States=FN_DISABLE;
	const char PlcNode[32]="M205";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}
	if ( States == FN_DISABLE ) 
	{	return false; }

	m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束	
	this->PLC_WriteNode(PlcNode, FN_DISABLE);
#endif//PLC_OBJ_DISABLE	
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBInFault_LB()//PLC-進板失敗-LB
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LB();
	if ( true == bLaneFault )
	{
		m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束
		CString str = _T("Error, PCB In Fault (LaneB)");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_LB);
	}
	return bLaneFault;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBIn2nd_LB(bool bWait)//PLC-第2段進板-LB
{
#ifndef PLC_OBJ_DISABLE
	m_PCBInCheckCount_LB = 0;
	m_PCBInTimeStart_LB = ::GetTickCount();//進板時間結束
	m_PCBInTimeEnd_LB = m_PCBInTimeStart_LB;//進板時間結束

	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBIn2nd_LB"));
	if ( this->PLC_WriteNode("M409", true) == false ) 
	{
		m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束		
		return false;
	}
	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_B);
	if ( bWait == false ) { return true; }
	return this->WaitForPCBIn2ndFinish_LB();	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBIn2ndFinish_LB()//PLC-第2段等進板完成-LB
{
#ifndef PLC_OBJ_DISABLE
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = this->GetPCBInOutTimeout_LB()+100;	
	for ( i=0; i<MaxCounts; i++ )
	{	
		this->m_PCBInCheckCount_LB = i;
		//讀取是否進板完成
		if ( this->CheckPCBIn2ndFinish_LB() == true )
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBIn2ndFinish_LB"));
			return true;	
		}

		if ( this->CheckPCBIn2ndFault_LB() == true )
		{
			str = _T("Error, PCB-2nd-In Fault With PLC Checked (LaneB)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);		
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LB();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}		
		::Sleep(10);		
	}
	str = _T("Error, PCB-2nd-In Fault With Timeout (LaneB)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_LB);
	m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束	
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBIn2ndFinish_LB()//PLC-第2段進板完成-LB
{
#ifndef PLC_OBJ_DISABLE
	//讀取是否到位
	int States=FN_DISABLE;
	const char PlcNode[32]="M212";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}
	if ( States == FN_DISABLE ) 
	{	return false; }
	m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束			
	this->PLC_WriteNode(PlcNode, FN_DISABLE);
#endif//PLC_OBJ_DISABLE	
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBIn2ndFault_LB()//PLC-第2段進板失敗-LB
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LB();
	if ( true == bLaneFault )
	{
		m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束			
		CString str = _T("Error, PCB-2nd-In Fault (LaneB)");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_LB);
	}
	return bLaneFault;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBIn3rd_LB(bool bWait)//PLC-第3段進板-LB
{
#ifndef PLC_OBJ_DISABLE
	m_PCBInCheckCount_LB = 0;
	m_PCBInTimeStart_LB = ::GetTickCount();//進板時間結束
	m_PCBInTimeEnd_LB = m_PCBInTimeStart_LB;//進板時間結束

	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBIn3rd_LB"));
	if ( this->PLC_WriteNode("M450", true) == false ) 
	{
		m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束		
		return false;
	}
	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_B);
	if ( bWait == false ) { return true; }
	return this->WaitForPCBIn3rdFinish_LB();	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBIn3rdFinish_LB()//PLC-第3段等進板完成-LB
{
#ifndef PLC_OBJ_DISABLE
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = this->GetPCBInOutTimeout_LB()+100;	
	for ( i=0; i<MaxCounts; i++ )
	{	
		this->m_PCBInCheckCount_LB = i;
		//讀取是否進板完成
		if ( this->CheckPCBIn3rdFinish_LB() == true )
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBIn3rdFinish_LB"));
			return true;	
		}

		if ( this->CheckPCBIn3rdFault_LB() == true )
		{
			str = _T("Error, PCB-3rd-In Fault With PLC Checked (LaneB)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);		
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LB();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}		
		::Sleep(10);		
	}
	str = _T("Error, PCB-3rd-In Fault With Timeout (LaneB)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_LB);
	m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束	
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBIn3rdFinish_LB()//PLC-第3段進板完成-LB
{
#ifndef PLC_OBJ_DISABLE
	//讀取是否到位
	int States=FN_DISABLE;
	const char PlcNode[32]="M250";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}
	if ( States == FN_DISABLE ) 
	{	return false; }
	m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束			
	this->PLC_WriteNode(PlcNode, FN_DISABLE);
#endif//PLC_OBJ_DISABLE	
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBIn3rdFault_LB()//PLC-第3段進板失敗-LB
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LB();
	if ( true == bLaneFault )
	{
		m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束			
		CString str = _T("Error, PCB-3rd-In Fault (LaneB)");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_LB);
	}
	return bLaneFault;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBBack_LB(bool bWait)//PLC-退板-LB
{
#ifndef PLC_OBJ_DISABLE
	const bool PCBBackOunt=m_PCBBackOut_LB;
	this->m_PCBBackCheckCount_LB = 0;
	this->m_PCBBackTimeStart_LB = ::GetTickCount();//進板時間結束
	this->m_PCBBackTimeEnd_LB = this->m_PCBBackTimeStart_LB;	
	if ( true == PCBBackOunt )
	{	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBBackOut_LB"));	}
	else
	{	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBBack_LB"));	}
	if ( this->PLC_WriteNode("M403", true) == false ) 	
	{
		this->m_PCBBackTimeEnd_LB = ::GetTickCount();//進板時間結束		
		return false;
	}
	if ( bWait == false ) { return true; }	
	return this->WaitForPCBBackFinish_LB();	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBBackFinish_LB()//PLC-等退板完成-LB
{	
#ifndef PLC_OBJ_DISABLE
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = 1000;
	const bool PCBBackOunt=m_PCBBackOut_LB;
	for ( i=0; i<MaxCounts; i++ )
	{	
		m_PCBBackCheckCount_LB = i;
		//讀取是否到位
		if ( this->CheckPCBBackFinish_LB() == true )
		{
			if ( true == PCBBackOunt )
			{	SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBBackOutFinish_LB"));	}
			else
			{	SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBBackFinish_LB"));	}
			return true;	
		}

		if ( this->CheckPCBBackFault_LB() == true )
		{
			if ( true == PCBBackOunt )
			{	str = _T("Error, PCB-Back-Out Fault With PLC Checked (LaneB)");	}
			else
			{	str = _T("Error, PCB-Back Fault With PLC Checked (LaneB)");	}
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);			
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LB();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}
		::Sleep(10);		
	}
	if ( true == PCBBackOunt )
	{
		WritePCBBackOut_LB(false);
		str = _T("Error, PCB-Back-Out Fault With Timeout (LaneB)");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_BACK_OUT_LB);		
	}
	else
	{
		str = _T("Error, PCB-Back Fault With Timeout (LaneB)");
		m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_BACK_LB);
	}
	this->m_PCBBackTimeEnd_LB = ::GetTickCount();//進板時間結束	
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBBackFinish_LB()//PLC-退板完成-LB
{
#ifndef PLC_OBJ_DISABLE
	//讀取是否到位
	int States=FN_DISABLE;
	const char PlcNode[32] = "M207";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}
	if ( States == FN_DISABLE ) 
	{	return false; }

	if ( true == m_PCBBackOut_LB )
	{	WritePCBBackOut_LB(false);	}
	m_PCBBackTimeEnd_LB = ::GetTickCount();
	this->PLC_WriteNode(PlcNode, FN_DISABLE);	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBBackFault_LB()//PLC-退板失敗-LB
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LB();
	if ( true == bLaneFault )
	{
		if ( true == m_PCBBackOut_LB )
		{
			WritePCBBackOut_LB(false);
			m_PCBBackTimeEnd_LB = ::GetTickCount();
			CString str = _T("Error, PCB Back-Out Fault (LaneB)");		
			m_ErrorString = LoadMultiLanguageString(str, str);
			SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_BACK_OUT_LB);
		}
		else
		{
			m_PCBBackTimeEnd_LB = ::GetTickCount();
			CString str = _T("Error, PCB Back Fault (LaneB)");
			m_ErrorString = LoadMultiLanguageString(str, str);
			SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_BACK_LB);
		}
	}
	return bLaneFault;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBBackOut_LB(bool bWait)//PLC-退出板-LB
{
#ifndef PLC_OBJ_DISABLE	
	if ( WritePCBBackOut_LB(true) == false )	
	{	return false;	}
	return ExecPCBBack_LB(bWait);	
#endif//PLC_OBJ_DISABLE
	return true;		
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBBackOutFinish_LB()//PLC-等退出板完成-LB
{
	return WaitForPCBBackFinish_LB();	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBBackOutFinish_LB()//PLC-退出板完成-LB
{
	return CheckPCBBackFinish_LB();	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBBackOutFault_LB()//PLC-退出板失敗-LB	
{
	return CheckPCBBackFault_LB();	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBOut_LB(bool bWait)//PLC-出板-LB
{
#ifndef PLC_OBJ_DISABLE
	this->m_PCBOutCheckCount_LB = 0;
	this->m_PCBOutTimeStart_LB = ::GetTickCount();//進板時間結束	
	this->m_PCBOutTimeEnd_LB = this->m_PCBOutTimeStart_LB;//進板時間結束	

	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBOut_LB"));
	if ( this->PLC_WriteNode("M402", true) == false ) 
	{
		this->m_PCBOutTimeEnd_LB = ::GetTickCount();//進板時間結束
		return false;
	}
	if ( bWait == false ) { return true; }
	return this->WaitForPCBOutFinish_LB();	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBOutFinish_LB()//PLC-等出板完成-LB
{	
#ifndef PLC_OBJ_DISABLE	
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = this->GetPCBInOutTimeout_LB()+100;;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( this->CheckPCBOutFinish_LB() == true ) 
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBOutFinish_LB"));
			return true;	
		}
		//讀取是否到位		
		if ( this->CheckPCBOutFault_LB() == true )
		{
			str = _T("Error, PCB-Out Fault With PLC Checked (LaneB)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);			
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LB();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}		
		::Sleep(10);		
	}
	str = _T("Error, PCB-Out Fault With Timeout (LaneB)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_OUT_LB);
	this->m_PCBOutTimeEnd_LB = ::GetTickCount();//進板時間結束	
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBOutFinish_LB()//PLC-出板完成-LB
{
#ifndef PLC_OBJ_DISABLE	
	int States=FN_DISABLE;
	const char PlcNode[32] = "M206";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}

	if ( States == FN_DISABLE ) 
	{	return false; }	

	m_PCBOutTimeEnd_LB = ::GetTickCount();
	this->PLC_WriteNode(PlcNode, FN_DISABLE);	
#endif//PLC_OBJ_DISABLE
	return true;		
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBOutFault_LB()//PLC-出板失敗-LB
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LB();
	if ( true == bLaneFault )
	{		
		m_PCBOutTimeEnd_LB = ::GetTickCount();
		CString str = _T("Error, PCB Out Fault (LaneB)");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_OUT_LB);
	}
	return bLaneFault;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBOutInside_LB(bool bWait)//PLC-出板至機台側邊-LB
{
#ifndef PLC_OBJ_DISABLE
	this->m_PCBOutCheckCount_LB = 0;
	this->m_PCBOutTimeStart_LB = ::GetTickCount();//進板時間結束	
	this->m_PCBOutTimeEnd_LB = this->m_PCBOutTimeStart_LB;//進板時間結束	
	
	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBOutInside_LB"));
	if ( this->PLC_WriteNode("M406", true) == false ) 
	{
		this->m_PCBOutTimeEnd_LB = ::GetTickCount();//進板時間結束		
		return false;
	}
	if ( bWait == false ) { return true; }
	return this->WaitForPCBOutInsideFinish_LB();	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBOutInsideFinish_LB()//PLC-等待出板至機台側邊-LB
{		
#ifndef PLC_OBJ_DISABLE
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = this->GetPCBInOutTimeout_LB()+100;;
	for ( i=0; i<MaxCounts; i++ )
	{	
		this->m_PCBOutCheckCount_LB = i;		
		if ( this->CheckPCBOutInsideFault_LB() == true )
		{
			str = _T("Error, PCB-Out-Stop Fault With PLC Checked (LaneB)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LB();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			this->m_PCBOutTimeEnd_LB = ::GetTickCount();//進板時間結束			
			return false;
		}

		//讀取是否到位
		if ( this->CheckPCBOutInsideFinish_LB() == true ) 
		{			
			this->m_PCBOutTimeEnd_LB = ::GetTickCount();//進板時間結束			
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBOutInsideFinish_LB"));
			return true;	
		}
				
		::Sleep(10);		
	}
	str = _T("Error, PCB-Out-Stop Fault With Timeout (LaneB)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_OUT_LB);
	this->m_PCBOutTimeEnd_LB = ::GetTickCount();//進板時間結束	
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBOutInsideFinish_LB()//PLC-出板至機台側邊完成-LB
{
#ifndef PLC_OBJ_DISABLE
	int States=FN_DISABLE;
	const char PlcNode[32] = "M210";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}
	if ( States == FN_DISABLE ) 
	{	return false;	}	
	this->PLC_WriteNode(PlcNode, FN_DISABLE);	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBOutInsideFault_LB()//PLC-出板至機台側邊失敗-LB
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LB();
	if ( true == bLaneFault )
	{
		CString str = _T("Error, PCB Out-Inside Fault (LaneB)");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_OUT_LB);
	}
	return bLaneFault;
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBOutIn_LB(bool bWait)//PLC-出板+進板-LB
{
#ifndef PLC_OBJ_DISABLE
	m_PCBOutCheckCount_LB = 0;	
	m_PCBInTimeStart_LB = ::GetTickCount();//進板時間結束	
	m_PCBOutTimeStart_LB = m_PCBInTimeStart_LB;//進板時間結束	
	m_PCBInTimeEnd_LB = m_PCBInTimeStart_LB;//進板時間結束		
	m_PCBOutTimeEnd_LB = m_PCBOutTimeStart_LB;//進板時間結束		

	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBOutIn_LB"));
	if ( this->PLC_WriteNode("M405", true) == false ) 
	{		
		m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束				
		m_PCBOutTimeEnd_LB = m_PCBInTimeEnd_LB;//進板時間結束				
		return false;
	}
	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_B);
	if ( bWait == false ) { return true; }
	return this->WaitForPCBOutInFinish_LB();	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBOutInFinish_LB()//PLC-等待出板+進板-LB
{
#ifndef PLC_OBJ_DISABLE
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = this->GetPCBInOutTimeout_LB()+100;
	for ( i=0; i<MaxCounts; i++ )
	{
		this->m_PCBInCheckCount_LB = i;
		this->m_PCBOutCheckCount_LB = i;

		//讀取是否到位		
		if ( this->CheckPCBOutInFault_LB() == true )
		{
			str = _T("Error, PCB-Out-In Fault With PLC Checked (LaneB)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);		
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LB();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}

		if ( this->CheckPCBOutInFinish_LB() == true ) 
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBOutInFinish_LB"));
			return true;	
		}
		::Sleep(10);		
	}
	str = _T("Error, PCB-Out-In Fault With Timeout (LaneB)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_OUT_IN_LB);
	m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束	
	m_PCBOutTimeEnd_LB = m_PCBInTimeEnd_LB;//進板時間結束	
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBOutInFinish_LB()//PLC-出板+進板完成-LB
{
#ifndef PLC_OBJ_DISABLE
	int States=FN_DISABLE;
	const char PlcNode[32] = "M209";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}
	if ( States == FN_DISABLE ) 
	{	return false;	}

	m_PCBInTimeEnd_LB = ::GetTickCount();
	m_PCBOutTimeEnd_LB = m_PCBInTimeEnd_LB;

	//做個虛擬中間時間
	DWORD HalfTime = ((m_PCBInTimeEnd_LB-m_PCBOutTimeStart_LB)/2)+m_PCBOutTimeStart_LB;
	m_PCBInTimeStart_LB = m_PCBOutTimeEnd_LB = HalfTime;

	this->PLC_WriteNode(PlcNode, FN_DISABLE);	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBOutInFault_LB()//PLC-出板+進板失敗-LB
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LB();
	if ( true == bLaneFault )
	{
		m_PCBInTimeEnd_LB = ::GetTickCount();
		m_PCBOutTimeEnd_LB = m_PCBInTimeEnd_LB;
		CString str = _T("Error, PCB Out-In Fault (LaneB)");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_OUT_IN_LB);
	}
	return bLaneFault;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBReIn_LB(bool bWait)//PLC-重新進板-LB
{
#ifndef PLC_OBJ_DISABLE
	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBReIn_LB"));
	if ( this->PLC_WriteNode("M404", true) == false ) 
	{	return false;	}
	
	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_B);
	if ( bWait == false ) { return true; }
	return this->WaitForPCBReInFinish_LB();		
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBReInFinish_LB()//PLC-等待重新進板完成-LB
{
#ifndef PLC_OBJ_DISABLE	
	int i=0;
	CString str;
	bool States = true;	
	const int MaxCounts = 300;
	for ( i=0; i<MaxCounts; i++ )
	{		
		//讀取是否進板完成
		if ( this->CheckPCBReInFinish_LB() == true )
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBReInFinish_LB"));
			return true;	
		}

		if ( this->CheckPCBInFault_LB() == true )
		{
			str = _T("Error, PCB Re-In Fault With PLC Checked (LaneB)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LB();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}		
		::Sleep(10);		
	}
	str = _T("Error, PCB Re-In Fault With Timeout (LaneB)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_LB);
	return false;
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBReInFinish_LB()//PLC-重新進板完成-LB
{
#ifndef PLC_OBJ_DISABLE
	int States=FN_DISABLE;
	const char PlcNode[32] = "M208";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}
	if ( States == FN_DISABLE ) 
	{	return false;	}
	this->PLC_WriteNode(PlcNode, FN_DISABLE);	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBClear_LB(bool bWait)//PLC-清板板-LB
{
#ifndef PLC_OBJ_DISABLE
	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBClear_LB"));
	if ( this->PLC_WriteNode("M453", true) == false ) 
	{	return false;	}
	
	if ( bWait == false ) { return true; }
	return this->WaitForPCBClearFinish_LB();
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBClearFinish_LB()//PLC-確認清板完成-LB
{
#ifndef PLC_OBJ_DISABLE
	//讀取清板完成
	int States=FN_DISABLE;	
	const char PlcNode[32] ="M253";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}

	if ( States == FN_DISABLE ) 
	{	return false; }
	this->PLC_WriteNode(PlcNode, FN_DISABLE);
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBClearFinish_LB()//PLC-等待清板完成-LB
{
#ifndef PLC_OBJ_DISABLE
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = 3000;
	for ( i=0; i<MaxCounts; i++ )
	{		
		//讀取是否清板完成
		if ( this->CheckPCBClearFinish_LB() == true )
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBClearFinish_LB"));
			return true;	
		}
		::Sleep(10);		
	}
	str = _T("Error, PCB Clear Fault With Timeout (LaneB)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_CLEAR_LB);
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::StopPCBAutoOutIn_LB(bool bWait)//PLC-停止自動進出板-LB
{
	if ( WritePCBAutoOutIn_LB(false) == false ) 
	{	return false;	}
	return true;

	if ( true == bWait )
	{	return WaitForPCBAutoOutInFinish_LB();	}
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBAutoOutIn_LB(bool On, bool bWait)//PLC-自動出進板-LB
{
#ifndef PLC_OBJ_DISABLE
	m_PCBOutCheckCount_LB = 0;	
	m_PCBInTimeStart_LB = ::GetTickCount();	
	m_PCBOutTimeStart_LB = m_PCBInTimeStart_LB;	
	m_PCBInTimeEnd_LB = m_PCBInTimeStart_LB;		
	m_PCBOutTimeEnd_LB = m_PCBOutTimeStart_LB;		

	SetPCBAutoRunPCBChaned_LB(false);
	if ( WritePCBAutoOutInFinish_LB(false) == false )
	{	return false; }

	SetPCBAutoRunMode_LB(PLC_PCB_AUTO_RUN_OUT_IN);		
	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBAutoOutIn_LB"));
	if ( WritePCBAutoOutIn_LB(On) == false ) 
	{	return false;	}

	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_B);
	if ( true == bWait )
	{	return WaitForPCBAutoOutInFinish_LB();	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBAutoOutInFinish_LB()//PLC-等待自動出進板-LB
{
#ifndef PLC_OBJ_DISABLE
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = this->GetPCBInOutTimeout_LB()+100;	
	for ( i=0; i<MaxCounts; i++ )
	{		
		//讀取是否進板完成
		if ( this->CheckPCBAutoOutInFinish_LB() == true )
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBAutoOutInFinish_LB"));
			return true;	
		}

		if ( this->CheckPCBAutoOutInFault_LB() == true )
		{
			str = _T("Error, PCB Auto Out-In Fault With PLC Checked (LaneB)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LB();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}		
		::Sleep(10);		
	}
	str = _T("Error, PCB Auto Out-In Fault With Timeout (LaneB)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_OUT_LB);
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadPCBAutoOutInFinish_LB()//PLC-自動出進板完成-LB
{
#ifndef PLC_OBJ_DISABLE
	//讀取是否到位
	int States=FN_DISABLE;	
	const char PlcNode[32] ="M211";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}
	if ( States == FN_DISABLE ) 
	{	return false; }
	SetPCBAutoRunPCBChaned_LB(true);
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBAutoOutInFinish_LB()//PLC-自動出進板完成-LB
{
#ifndef PLC_OBJ_DISABLE
	if ( ReadPCBAutoOutInFinish_LB() == false )
	{	return false; }

	m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束
	m_PCBOutTimeEnd_LB = m_PCBInTimeEnd_LB;//進板時間結束	

	//做個虛擬中間時間
	DWORD HalfTime = ((m_PCBInTimeEnd_LB-m_PCBOutTimeStart_LB)/2)+m_PCBOutTimeStart_LB;
	m_PCBInTimeStart_LB = m_PCBOutTimeEnd_LB = HalfTime;

	//this->PLC_WriteNode(PlcNode, FN_DISABLE);
	WritePCBAutoOutIn_LB(false);	
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBAutoOutInFault_LB()//PLC-自動出進板失敗-LB
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LB();
	if ( true == bLaneFault )
	{
		WritePCBAutoOutIn_LB(false);
		CString str = _T("Error, PCB Auto Out-In Fault (LaneB)");
		m_ErrorString = LoadMultiLanguageString(str, str);		
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_OUT_LB);
	}
	return bLaneFault;	
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::StopPCBAutoBackIn_LB(bool bWait)//PLC-停止自動退進板-LB
{
	if ( this->WritePCBAutoBackIn_LB(false) == false ) 
	{	return false;	}
	return true;

	if ( true == bWait )
	{	return WaitForPCBAutoBackInFinish_LB();	}
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ExecPCBAutoBackIn_LB(bool On, bool bWait)//PLC-自動退進板-LB
{
#ifdef LANE_B_DISABLE_USE
	return true;
#endif//LANE_B_DISABLE_USE

#ifndef PLC_OBJ_DISABLE
	m_PCBOutCheckCount_LB = 0;	
	m_PCBInTimeStart_LB = ::GetTickCount();
	m_PCBBackTimeStart_LB = m_PCBInTimeStart_LB;
	m_PCBInTimeEnd_LB = m_PCBInTimeStart_LB;		
	m_PCBBackTimeEnd_LB = m_PCBBackTimeStart_LB;		

	SetPCBAutoRunPCBChaned_LB(false);
	if ( WritePCBAutoBackInFinish_LB(false) == false )
	{	return false; }

	SetPCBAutoRunMode_LB(PLC_PCB_AUTO_RUN_BACK_IN);		
	SavePLCMovingTimeMsg(_T("CPLC_Vigor::ExecPCBAutoBackIn_LB"));
	if ( this->WritePCBAutoBackIn_LB(On) == false ) 
	{	return false;	}

	AOIDataCollect.SetIsNeedGrabFiducial(true, LANE_ID_B);
	if ( true == bWait )
	{	return WaitForPCBAutoBackInFinish_LB();	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WaitForPCBAutoBackInFinish_LB()//PLC-等待自動退進板-LB
{
#ifndef PLC_OBJ_DISABLE
	int i=0;
	CString str;
	bool States = true;
	const int MaxCounts = this->GetPCBInOutTimeout_LB()+100;	
	for ( i=0; i<MaxCounts; i++ )
	{		
		//讀取是否進板完成
		if ( this->CheckPCBAutoBackInFinish_LB() == true )
		{
			SavePLCMovingTimeMsg(_T("CPLC_Vigor::WaitForPCBAutoBackInFinish_LB"));
			return true;	
		}

		if ( this->CheckPCBAutoBackInFault_LB() == true )
		{
			str = _T("Error, PCB Auto Back-In Fault With PLC Checked (LaneB)");
			m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
			return false;

			int ErrorCode = ReadLanePLCErrorCode_LB();
			m_ErrorString = GetPLCErrorCodeText(ErrorCode);
			return false;
		}		
		::Sleep(10);		
	}
	str = _T("Error, PCB Auto Back-In Fault With Timeout (LaneB)");
	m_ErrorString = CPLC_Vigor::LoadMultiLanguageString(str, str);	
	SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_BACK_LB);
	return false;	
#endif//PLC_OBJ_DISABLE
	return true;	
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadPCBAutoBackInFinish_LB()//PLC-自動退進板完成-LB
{
#ifndef PLC_OBJ_DISABLE
	//讀取是否到位
	int States=FN_DISABLE;	
	const char PlcNode[32] ="M213";
	if ( this->PLC_ReadNode(PlcNode, States) == false ) 
	{	return false;	}
	if ( States == FN_DISABLE ) 
	{	return false; }
	SetPCBAutoRunPCBChaned_LB(true);	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBAutoBackInFinish_LB()//PLC-自動退進板完成-LB
{
#ifndef PLC_OBJ_DISABLE
	if ( ReadPCBAutoBackInFinish_LB() == false ) { return false; }
	
	m_PCBInTimeEnd_LB = ::GetTickCount();//進板時間結束
	m_PCBBackTimeEnd_LB = m_PCBInTimeEnd_LB;//進板時間結束	

	//做個虛擬中間時間
	DWORD HalfTime = ((m_PCBInTimeEnd_LB-m_PCBBackTimeStart_LB)/2)+m_PCBBackTimeStart_LB;
	m_PCBInTimeStart_LB = m_PCBBackTimeEnd_LB = HalfTime;

	//this->PLC_WriteNode(PlcNode, FN_DISABLE);
	WritePCBAutoBackIn_LB(false);	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::CheckPCBAutoBackInFault_LB()//PLC-自動退進板失敗-LB
{
#ifndef PLC_OBJ_DISABLE
	const bool bLaneFault = ReadLaneFault_LB();
	if ( true == bLaneFault )
	{
		WritePCBAutoBackIn_LB(false);
		CString str = _T("Error, PCB Auto Back-In Fault (LaneB)");
		m_ErrorString = LoadMultiLanguageString(str, str);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_PCB_IN_BACK_LB);
	}
	return bLaneFault;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteSignalToLast_LB(bool On)//PLC-送訊號給上一站-LB
{
#ifndef PLC_OBJ_DISABLE
	if ( this->PLC_WriteNode("M410", On) == false ) 
	{	return false;	}		
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteSignalToNext_LB(bool On)//PLC-送訊號給下一站-LB
{
#ifndef PLC_OBJ_DISABLE
	if ( this->PLC_WriteNode("M411", On) == false ) 
	{	return false;	}		
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteOKSignalToNext_LB(bool On)//PLC-送OK訊號給下一站-LB
{
#ifndef PLC_OBJ_DISABLE
	if ( this->PLC_WriteNode("M430", On) == false ) 
	{	return false;	}		
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteNGSignalToNext_LB(bool On)//PLC-送NG訊號給下一站-LB
{
#ifndef PLC_OBJ_DISABLE
	if ( this->PLC_WriteNode("M412", On) == false ) 
	{	return false;	}	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLockSignal_LB(bool On)//PLC-送鎖住訊號-LB
{
#ifndef PLC_OBJ_DISABLE
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteConveyerSensorPower_LB(bool On, bool bForce)//PLC-關閉軌道感應器電源-LB
{
#ifndef PLC_OBJ_DISABLE	
	bool Off=!On;//注意是反向的邏輯
	if ( false == bForce )
	{
		if ( On == GetConveryerSensorPower_LB() )
		{	return true; }
	}
	if ( true == On )
	{	SavePLCMovingTimeMsg(_T("CPLC_Vigor::WriteConveyerSensorPower_LB-On"));	}
	else
	{	SavePLCMovingTimeMsg(_T("CPLC_Vigor::WriteConveyerSensorPower_LB-Off"));	}
	if ( this->PLC_WriteNode("M445", Off) == false ) 
	{	return false;	}
	//注意是反向的邏輯
	SetConveryerSensorPower_LB(On);
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::TurnOffConveyerSensorPower_LB(bool On)//PLC-關閉軌道感應器電源-LB
{
#ifndef PLC_OBJ_DISABLE
	if ( FN_DISABLE == GetPLCParameter().m_TurnOffConveyerSensorPower_LB ) { return true; }
	const bool Off=!On;
	if ( WriteConveyerSensorPower_LB(Off, false) == false ) 
	{	return false;	}	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::PushDownStartBtn()//按下啟動燈
{
#ifndef PLC_OBJ_DISABLE
	const bool bOn = true;
	if ( this->PLC_WriteNode("M501", bOn) == false ) 
	{	return false;	}	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::PushDownResetBtn()//按下復歸燈
{
#ifndef PLC_OBJ_DISABLE
	const bool bOn = true;
	//if ( this->PLC_WriteNode("M504", bOn) == false ) 
	if ( this->PLC_WriteNode("M800", bOn) == false ) 
	{	return false;	}	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::PushDownStopBtn()	//按下停止燈
{
#ifndef PLC_OBJ_DISABLE
	const bool bOn = true;
	if ( this->PLC_WriteNode("M502", bOn) == false ) 
	{	return false;	}	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustManual(bool On)//間距馬達-手動
{
#ifndef PLC_OBJ_DISABLE
	char PLCNode[32] = "M940";		
	if ( this->WriteSingle(PLCNode, On) == false )
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustFixed14Lane(bool On)//間距馬達-固定軌道模式
{
#ifndef PLC_OBJ_DISABLE
	char PLCNode[32] = "M324";	
	bool bNodeOn = false==On ? true:false;
	//改成對調CW/CCW實體線路
	//if ( this->WriteSingle(PLCNode, bNodeOn) == false )
	//{	return false;	}
	m_LaneAdjust_Fixed14Lane = On;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustHomeTimeout(int time)//間距馬達-歸零逾時
{
#ifndef PLC_OBJ_DISABLE
	char Data[32]="";	
	const int DCodeNo = JET_PLC_LANE_ADJUST_HOME_TIMEOUT;	
	const int ValueI = (int)(time);
	this->TransferINTToDcodeData(ValueI, Data);	
	if ( this->WriteDCode(DCodeNo, Data) == false )
	{	return false;	}
	m_LaneAdjust_HomeTimeout = time;		
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustMoveTimeout(int time)//間距馬達-移動逾時
{
#ifndef PLC_OBJ_DISABLE
	char Data[32]="";	
	const int DCodeNo = JET_PLC_LANE_ADJUST_MOVE_TIMEOUT;	
	const int ValueI = (int)(time);
	this->TransferINTToDcodeData(ValueI, Data);	
	if ( this->WriteDCode(DCodeNo, Data) == false )
	{	return false;	}
	m_LaneAdjust_MoveTimeout = time;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::InitialLaneAdjust_LA()//間距馬達-初始化
{
#ifndef PLC_OBJ_DISABLE
	char PLCNode[32] = "";

	//Disable Moving Done
	::sprintf(PLCNode, "%s", "M3165");
	if ( this->PLC_WriteNode(PLCNode, false) == false )
	{	return false;	}

	//Turn off home
	if ( this->WriteLaneAdjustToHome_LA(false) == false )
	{	return false;	}

	//Turn off Move To
	if ( this->WriteLaneAdjustToMove_LA(false) == false )
	{	return false;	}

	//Stop Jog CW
	if ( this->WriteLaneAdjustJogMove_LA(true, true) == false )
	{	return false;	}

	//Stop Jog CCW
	if ( this->WriteLaneAdjustJogMove_LA(false, true) == false )
	{	return false;	}

	if ( this->ResetLaneAdjustException_LA() == false )
	{	return false;	}

	ReadLaneAdjustCurrentPos_LA();
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustCmdPos_LA(double Pos)//間距馬達-設定命令位置	
{	
#ifndef PLC_OBJ_DISABLE
	CString str;
	char Data[32]="";
	const int DCodeNo = JET_PLC_LANE_ADJUST_MOVE_TO_POS_LA;
	if ( Pos < JET_PLC_LANE_ADJUST_LIMIT_MIN )
	{		
		str = _T("Error, Command Position too small");
		str = CPLC_Basic::LoadMultiLanguageString(str, str);
		m_ErrorString.Format(_T("%s (%.2f/%d)"), str, Pos, JET_PLC_LANE_ADJUST_LIMIT_MIN);
		return false;
	}
	if ( Pos<m_LaneAdjust_LimitMin_LA || Pos>m_LaneAdjust_LimitMax_LA )
	{
		str = _T("Error, Command Position is out of range");
		str = CPLC_Basic::LoadMultiLanguageString(str, str);
		m_ErrorString.Format(_T("%s [%.2f (%.2f ~ %.2f)]"), str, Pos, m_LaneAdjust_LimitMin_LA, m_LaneAdjust_LimitMax_LA);
		return false;
	}
	const int ValueI = (int)(Pos*10);
	this->TransferINTToDcodeData(ValueI, Data);	
	if ( this->WriteDCode(DCodeNo, Data) == false )
	{	return false;	}
#else
	SetLaneAdjustCurrentPos_LA(Pos);
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustToHome_LA(bool On)//間距馬達-歸零搜尋
{
#ifndef PLC_OBJ_DISABLE
	char PLCNode[32] = "M3010";
	if ( On == true ) 
	{	
		::sprintf(PLCNode, "%s", "M3155");//Disable Home Done
		if ( this->WriteSingle(PLCNode, false) == false )
		{	return false;	}
		m_LaneAdjust_Homed_LA = false;
	}
	::sprintf(PLCNode, "%s", "M3010");
	if ( this->WriteSingle(PLCNode, On) == false )
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustToMove_LA(bool On)//間距馬達-開始移動
{
#ifndef PLC_OBJ_DISABLE
	char PLCNode[32] = "M3011";
	if ( this->WriteSingle(PLCNode, On) == false )
	{	return false;	}	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustJogMove_LA(bool Dir, bool Stop)//間距馬達-搖桿移動
{
#ifndef PLC_OBJ_DISABLE
	bool stats = false;
	char Data[32]="";
	char PLCNode[32]="";
	if ( Stop == true ) { stats = false; }
	else { stats = true; }	
	if ( true == Dir ) 
	{	::sprintf(PLCNode, "%s", "M3012");	}
	else
	{	::sprintf(PLCNode, "%s", "M3013");	}	
	if ( this->WriteSingle(PLCNode, stats) == false )
	{	return false;	}
	m_LaneAdjust_JogMoving_LA = stats;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustLimitMaxPos_LA(double Pos)//間距馬達-設定最大位置
{
#ifndef PLC_OBJ_DISABLE
	char Data[32]="";	
	const int DCodeNo = JET_PLC_LANE_ADJUST_LIMIT_MAX_LA;
	if ( Pos < JET_PLC_LANE_ADJUST_LIMIT_MIN )
	{
		CString str;
		str = _T("Error, Command Position too small");
		str = CPLC_Basic::LoadMultiLanguageString(str, str);
		m_ErrorString.Format(_T("%s (%.2f/%d)"), str, Pos, JET_PLC_LANE_ADJUST_LIMIT_MIN);
		return false;
	}
	const int ValueI = (int)(Pos*10);
	this->TransferINTToDcodeData(ValueI, Data);	
	if ( this->WriteDCode(DCodeNo, Data) == false )
	{	return false;	}
	m_LaneAdjust_LimitMax_LA = Pos;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustJogSpeed_LA(double Speed)//間距馬達-設定Jog速度
{
#ifndef PLC_OBJ_DISABLE
	char Data[32]="";
	const int DCodeNo = JET_PLC_LANE_ADJUST_JOG_SPEED_LA;
	const int ValueI = (int)(Speed*10);
	this->TransferINTToDcodeData(ValueI, Data);	
	if ( this->WriteDCode(DCodeNo, Data) == false )
	{	return false;	}	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustSkewPitch_LA(double Pitch)//間距馬達-設定移動距離
{
#ifndef PLC_OBJ_DISABLE
	char Data[32]="";	
	const int DCodeNo = JET_PLC_LANE_ADJUST_SKEW_PITCH_LA;	
	const int ValueI = (int)(Pitch);
	this->TransferINTToDcodeData(ValueI, Data);	
	if ( this->WriteDCode(DCodeNo, Data) == false )
	{	return false;	}
	m_LaneAdjust_SkewPitch_LA = Pitch;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ResetLaneAdjustException_LA()//間距馬達-復歸異常
{
#ifndef PLC_OBJ_DISABLE
	char PLCNode[32] = "M800";
	if ( this->WriteSingle(PLCNode, false) == false )
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadLaneAdjustDisable_LA()//間距馬達-是否關閉
{
#ifndef PLC_OBJ_DISABLE
	bool States = false;
	char PLCNode[32] = "M547";
	if ( this->ReadSingle(PLCNode, States) == false )
	{	return true;	}	
	m_LaneAdjust_Disable_LA = States;
	return States;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//

bool CPLC_Vigor::ReadLaneAdjustSensorORG_LA()//間距馬達-確認是否在原點
{
#ifndef PLC_OBJ_DISABLE
	bool States = false;
	char PLCNode[32] = "M508";
	if ( this->ReadSingle(PLCNode, States) == false )
	{	return false;	}
	m_LaneAdjust_SensorORG_LA = States;
	if ( States == true ) 
	{	return true;	}
	return false;	
#endif//PLC_OBJ_DISABLE
	return false;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadLaneAdjustSensorLimit_LA()//間距馬達-確認是否在極限
{
#ifndef PLC_OBJ_DISABLE
	bool States = false;
	char PLCNode[32] = "M510";
	if ( this->ReadSingle(PLCNode, States) == false )
	{	return true;	}
	m_LaneAdjust_SensorLimit_LA = States;
	if ( States == true ) 
	{	return false;	}
	return true;
#endif//PLC_OBJ_DISABLE
	return false;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadLaneAdjustExecption_LA()//間距馬達-確認是否異常
{
#ifndef PLC_OBJ_DISABLE
	bool States = false;	
	char PLCNode[32] = "M4090";
	if ( this->ReadSingle(PLCNode, States) == false )
	{	return true;	}
	if ( true == States ) 
	{
		char ErrorCode[32]="";
		const int DCode = 4000;//D4000
		if ( this->ReadDCode(DCode, ErrorCode) == false ) 
		{	return true; }
		int ValueI = this->TransferDcodeToINT(ErrorCode);	
		this->m_ErrorString.Format(_T("Error, Lane Adjust Exception LA (%d)"), ValueI);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_LANE_ADJUST_MOVE_LA);
		return true;	
	}
	return false;
#endif//PLC_OBJ_DISABLE
	return false;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadLaneAdjustHomeDone_LA()//間距馬達-確認是否歸零完畢
{	
#ifndef PLC_OBJ_DISABLE
	bool States = false;
	char PLCNode[32] = "M3155";
	if ( this->ReadSingle(PLCNode, States) == false )
	{	return false;	}
	m_LaneAdjust_Homed_LA = States;
	return States;	
#endif//PLC_OBJ_DISABLE
	return false;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadLaneAdjustMoveDone_LA()//間距馬達-確認是否移動完畢
{
#ifndef PLC_OBJ_DISABLE
	bool States = false;
	char PLCNode[32] = "M3165";
	if ( this->ReadSingle(PLCNode, States) == false )
	{	return false;	}
	if ( States == true ) 
	{	return true;	}
	return false;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
double CPLC_Vigor::ReadLaneAdjustCurrentPos_LA()//間距馬達-取回目前位置	
{
#ifndef PLC_OBJ_DISABLE
	char Data[32]="";		
	const int DCodeNo = JET_PLC_LANE_ADJUST_CURRENT_POS_LA;
	if ( this->ReadDCode(DCodeNo, Data) == false )
	{	return 0;	}
	int ValueI = this->TransferDcodeToINT(Data);
	double ValueD = ValueI;
	ValueD = ValueD/10.0;	
	if ( GetLaneAdjustDisableBtn_LA() == false )
	{	SetLaneAdjustCurrentPos_LA(ValueD); }
	return ValueD;
#endif//PLC_OBJ_DISABLE
	return 0.0;
}
//-----------------------------------------------------------------------//
int CPLC_Vigor::ReadLaneAdjustPLCErrorCode_LA()//間距馬達-取回PLC的錯誤代碼
{
#ifndef PLC_OBJ_DISABLE
	char Data[32]="";		
	const int DCodeNo = 4000;//D4000
	if ( this->ReadDCode(DCodeNo, Data) == false )
	{	return 0;	}
	int ValueI = this->TransferDcodeToINT(Data);	
	return ValueI;	
#endif//PLC_OBJ_DISABLE
	return 0;
}
//-----------------------------------------------------------------------//
int CPLC_Vigor::ReadLaneAdjustMotorErrorCode_LA()//間距馬達-取回馬達的錯誤代碼		
{
#ifndef PLC_OBJ_DISABLE
	char Data[32]="";		
	const int DCodeNo = 3400;	
	if ( this->ReadDCode(DCodeNo, Data) == false )
	{	return 0;	}
	int ValueI = this->TransferDcodeToINT(Data);	
	return ValueI;	
#endif//PLC_OBJ_DISABLE
	return 0;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::InitialLaneAdjust_LB()//間距馬達-初始化
{
#ifndef PLC_OBJ_DISABLE
	char PLCNode[32] = "";

	//Disable Moving Done
	::sprintf(PLCNode, "%s", "M3265");
	if ( this->PLC_WriteNode(PLCNode, false) == false )
	{	return false;	}

	//Turn off home
	if ( this->WriteLaneAdjustToHome_LB(false) == false )
	{	return false;	}

	//Turn off Move To
	if ( this->WriteLaneAdjustToMove_LB(false) == false )
	{	return false;	}

	//Stop Jog CW
	if ( this->WriteLaneAdjustJogMove_LB(true, true) == false )
	{	return false;	}

	//Stop Jog CCW
	if ( this->WriteLaneAdjustJogMove_LB(false, true) == false )
	{	return false;	}

	if ( this->ResetLaneAdjustException_LB() == false )
	{	return false;	}

	ReadLaneAdjustCurrentPos_LB();
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustCmdPos_LB(double Pos)//間距馬達-設定命令位置	
{	
#ifndef PLC_OBJ_DISABLE
	CString str;
	char Data[32]="";
	const int DCodeNo = JET_PLC_LANE_ADJUST_MOVE_TO_POS_LB;
	if ( Pos < JET_PLC_LANE_ADJUST_LIMIT_MIN )
	{	
		str = _T("Error, Command Position too small");
		str = CPLC_Basic::LoadMultiLanguageString(str, str);
		m_ErrorString.Format(_T("%s (%.2f/%d)"), str, Pos, JET_PLC_LANE_ADJUST_LIMIT_MIN);
		return false;
	}
	if ( Pos<m_LaneAdjust_LimitMin_LB || Pos>m_LaneAdjust_LimitMax_LB )
	{
		str = _T("Error, Command Position is out of range");
		str = CPLC_Basic::LoadMultiLanguageString(str, str);
		m_ErrorString.Format(_T("%s [%.2f (%.2f ~ %.2f)]"), str, Pos, m_LaneAdjust_LimitMin_LB, m_LaneAdjust_LimitMax_LB);
		return false;
	}
	const int ValueI = (int)(Pos*10);
	this->TransferINTToDcodeData(ValueI, Data);	
	if ( this->WriteDCode(DCodeNo, Data) == false )
	{	return false;	}
#else
	SetLaneAdjustCurrentPos_LB(Pos);
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustToHome_LB(bool On)//間距馬達-歸零搜尋
{
#ifndef PLC_OBJ_DISABLE
	char PLCNode[32] = "M3020";
	if ( On == true ) 
	{	
		::sprintf(PLCNode, "%s", "M3255");//Disable Home Done
		if ( this->WriteSingle(PLCNode, false) == false )
		{	return false;	}
		m_LaneAdjust_Homed_LB = false;
	}
	::sprintf(PLCNode, "%s", "M3020");
	if ( this->WriteSingle(PLCNode, On) == false )
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustToMove_LB(bool On)//間距馬達-開始移動
{
#ifndef PLC_OBJ_DISABLE
	char PLCNode[32] = "M3021";
	if ( this->WriteSingle(PLCNode, On) == false )
	{	return false;	}	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustJogMove_LB(bool Dir, bool Stop)//間距馬達-搖桿移動
{
#ifndef PLC_OBJ_DISABLE
	bool stats = false;
	char Data[32]="";
	char PLCNode[32]="";
	if ( Stop == true ) { stats = false; }
	else { stats = true; }	
	if ( true == Dir ) 
	{	::sprintf(PLCNode, "%s", "M3022");	}
	else
	{	::sprintf(PLCNode, "%s", "M3023");	}	
	if ( this->WriteSingle(PLCNode, stats) == false )
	{	return false;	}	
	m_LaneAdjust_JogMoving_LB = stats;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustLimitMaxPos_LB(double Pos)//間距馬達-設定最大位置
{
#ifndef PLC_OBJ_DISABLE
	char Data[32]="";	
	const int DCodeNo = JET_PLC_LANE_ADJUST_LIMIT_MAX_LB;
	if ( Pos < JET_PLC_LANE_ADJUST_LIMIT_MIN )
	{
		CString str;
		str = _T("Error, Command Position too small");
		str = CPLC_Basic::LoadMultiLanguageString(str, str);
		m_ErrorString.Format(_T("%s (%.2f/%d)"), str, Pos, JET_PLC_LANE_ADJUST_LIMIT_MIN);
		return false;
	}
	const int ValueI = (int)(Pos*10);
	this->TransferINTToDcodeData(ValueI, Data);	
	if ( this->WriteDCode(DCodeNo, Data) == false )
	{	return false;	}
	m_LaneAdjust_LimitMax_LB = Pos;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustJogSpeed_LB(double Speed)//間距馬達-設定Jog速度
{
#ifndef PLC_OBJ_DISABLE
	char Data[32]="";
	const int DCodeNo = JET_PLC_LANE_ADJUST_JOG_SPEED_LB;
	const int ValueI = (int)(Speed*10);
	this->TransferINTToDcodeData(ValueI, Data);	
	if ( this->WriteDCode(DCodeNo, Data) == false )
	{	return false;	}	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::WriteLaneAdjustSkewPitch_LB(double Pitch)//間距馬達-設定移動距離
{
#ifndef PLC_OBJ_DISABLE
	char Data[32]="";	
	const int DCodeNo = JET_PLC_LANE_ADJUST_SKEW_PITCH_LB;	
	const int ValueI = (int)(Pitch);
	this->TransferINTToDcodeData(ValueI, Data);	
	if ( this->WriteDCode(DCodeNo, Data) == false )
	{	return false;	}
	m_LaneAdjust_SkewPitch_LB = Pitch;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ResetLaneAdjustException_LB()//間距馬達-復歸異常
{
#ifndef PLC_OBJ_DISABLE
	char PLCNode[32] = "M820";
	if ( this->WriteSingle(PLCNode, false) == false )
	{	return false;	}
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadLaneAdjustDisable_LB()//間距馬達-是否關閉
{
#ifndef PLC_OBJ_DISABLE
	bool States = false;
	char PLCNode[32] = "M515";
	if ( this->ReadSingle(PLCNode, States) == false )
	{	return true;	}	
	m_LaneAdjust_Disable_LB = States;
	return States;	
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadLaneAdjustSensorORG_LB()//間距馬達-確認是否在原點
{
#ifndef PLC_OBJ_DISABLE
	bool States = false;
	char PLCNode[32] = "M509";
	if ( this->ReadSingle(PLCNode, States) == false )
	{	return false;	}
	m_LaneAdjust_SensorORG_LB = States;
	if ( States == true ) 
	{	return true;	}
	return false;	
#endif//PLC_OBJ_DISABLE
	return false;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadLaneAdjustSensorLimit_LB()//間距馬達-確認是否在極限
{
#ifndef PLC_OBJ_DISABLE
	bool States = false;
	char PLCNode[32] = "M511";
	if ( this->ReadSingle(PLCNode, States) == false )
	{	return true;	}
	m_LaneAdjust_SensorLimit_LB = States;
	if ( States == true ) 
	{	return false;	}
	return true;
#endif//PLC_OBJ_DISABLE
	return false;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadLaneAdjustExecption_LB()//間距馬達-確認是否異常
{
#ifndef PLC_OBJ_DISABLE
	bool States = false;	
	char PLCNode[32] = "M4091";
	if ( this->ReadSingle(PLCNode, States) == false )
	{	return true;	}
	if ( true == States ) 
	{
		char ErrorCode[32]="";
		const int DCode = 4000;//D4000
		if ( this->ReadDCode(DCode, ErrorCode) == false ) 
		{	return true; }
		int ValueI = this->TransferDcodeToINT(ErrorCode);	
		this->m_ErrorString.Format(_T("Error, Lane Adjust Exception LB (%d)"), ValueI);
		SetPLCExceptionCode(AOI_EXCEPTION_PLC_LANE_ADJUST_MOVE_LB);
		return true;	
	}
	return false;
#endif//PLC_OBJ_DISABLE
	return false;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadLaneAdjustHomeDone_LB()//間距馬達-確認是否歸零完畢
{	
#ifndef PLC_OBJ_DISABLE
	bool States = false;
	char PLCNode[32] = "M3255";
	if ( this->ReadSingle(PLCNode, States) == false )
	{	return false;	}
	m_LaneAdjust_Homed_LB = States;
	return States;
#endif//PLC_OBJ_DISABLE
	return false;
}
//-----------------------------------------------------------------------//
bool CPLC_Vigor::ReadLaneAdjustMoveDone_LB()//間距馬達-確認是否移動完畢
{
#ifndef PLC_OBJ_DISABLE
	bool States = false;
	char PLCNode[32] = "M3265";
	if ( this->ReadSingle(PLCNode, States) == false )
	{	return false;	}
	if ( States == true ) 
	{	return true;	}
	return false;
#endif//PLC_OBJ_DISABLE
	return true;
}
//-----------------------------------------------------------------------//
double CPLC_Vigor::ReadLaneAdjustCurrentPos_LB()//間距馬達-取回目前位置	
{
#ifndef PLC_OBJ_DISABLE
	char Data[32]="";		
	const int DCodeNo = JET_PLC_LANE_ADJUST_CURRENT_POS_LB;
	if ( this->ReadDCode(DCodeNo, Data) == false )
	{	return 0;	}
	int ValueI = this->TransferDcodeToINT(Data);
	double ValueD = ValueI;
	ValueD = ValueD/10.0;		
	if ( GetLaneAdjustDisableBtn_LB() == false )
	{	SetLaneAdjustCurrentPos_LB(ValueD); }
	return ValueD;
#endif//PLC_OBJ_DISABLE
	return 0.0;
}
//-----------------------------------------------------------------------//
int CPLC_Vigor::ReadLaneAdjustPLCErrorCode_LB()//間距馬達-取回PLC的錯誤代碼
{
#ifndef PLC_OBJ_DISABLE
	char Data[32]="";		
	const int DCodeNo = 4000;//D4000
	if ( this->ReadDCode(DCodeNo, Data) == false )
	{	return 0;	}
	int ValueI = this->TransferDcodeToINT(Data);	
	return ValueI;	
#endif//PLC_OBJ_DISABLE
	return 0;
}
//-----------------------------------------------------------------------//
int CPLC_Vigor::ReadLaneAdjustMotorErrorCode_LB()//間距馬達-取回馬達的錯誤代碼		
{
#ifndef PLC_OBJ_DISABLE
	char Data[32]="";		
	const int DCodeNo = 3401;	
	if ( this->ReadDCode(DCodeNo, Data) == false )
	{	return 0;	}
	int ValueI = this->TransferDcodeToINT(Data);	
	return ValueI;	
#endif//PLC_OBJ_DISABLE
	return 0;
}
//-----------------------------------------------------------------------//
#endif//PLC_OBJ_MODE