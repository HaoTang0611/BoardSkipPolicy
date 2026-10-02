// Motion_Basic.cpp: implementation of the CMotion_Basic class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Motion_Basic.h"
//-------------------------------------------------------------------------------------//
#include "Motion_PCI_M114GL.h"
#include "Motion_PCE_M114GL.h"
#include "Motion_Lib_Module.h"
#include "Motion_PCIE_L221_B1D0.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#if MOTION_DERIVE_MODE == MOTION_PCI_M114GL
	CMotion_Basic *MotionCtrlPtr = &Motion_PCI_M114GL;
#elif MOTION_DERIVE_MODE == MOTION_PCE_M114GL
	CMotion_Basic *MotionCtrlPtr = &Motion_PCE_M114GL;
#elif MOTION_DERIVE_MODE == MOTION_LIB_MODULE
	CMotion_Basic *MotionCtrlPtr = &Motion_Lib_Module;
#elif MOTION_DERIVE_MODE == MOTION_PCIE_L221_B1D0
	CMotion_Basic *MotionCtrlPtr = &Motion_PCIE_L221_B1D0;
#else
	CMotion_Basic *MotionCtrlPtr = NULL;
	#error Jet Motion Mode is not specified.
#endif//MOTION_DERIVE_MODE
//-------------------------------------------------------------------------------------//
//XY校正檔案參數編號
#define   XYCALI_START           0

#define   XYCALI_BEGIN          10
#define   XYCALI_INDEX          11

#define   XYCALI_POS_X1        101
#define   XYCALI_POS_Y1        102
#define   XYCALI_CALI_X1       103
#define   XYCALI_CALI_Y1       104

#define   XYCALI_POS_X2        201
#define   XYCALI_POS_Y2        202
#define   XYCALI_CALI_X2       203
#define   XYCALI_CALI_Y2       204

#define   XYCALI_POS_X3        301
#define   XYCALI_POS_Y3        302
#define   XYCALI_CALI_X3       303
#define   XYCALI_CALI_Y3       304

#define   XYCALI_POS_X4        401
#define   XYCALI_POS_Y4        402
#define   XYCALI_CALI_X4       403
#define   XYCALI_CALI_Y4       404

#define   XYCALI_END           910
#define   XYCALI_FINISH        999
//-----------------------------------------------------------------------------//
unsigned int MotionCtrlThreadID_GoStop = 0;//走停取像執行緒編號
HANDLE MotionCtrlThreadHandle_GoStop = NULL;//走停取像執行緒
HANDLE MotionCtrlThreadEvent_GoStop = NULL;//走停取像執行緒事件
unsigned int __stdcall MotionCtrlThreadFn_GoStop(void *pParam);//走停取像執行緒
//-------------------------------------------------------------------------------------//
unsigned int __stdcall MotionCtrlThreadFn_GoStop(void *pParam)//走停取像執行緒
{
#ifndef OFFLINE_VERSION
	TCHAR strBuffer[256]=_T("");
	const int SleepTime = 20;	
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;
	bool IsOK = true;
	bool IsExit = false;	
	DWORD MSG = NULL;
	bool IsShowEmergeMsg = false;
	bool SystemGrabMode = false;
	double                 Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;

	while ( true )
	{	
		IsExit = MotionCtrlPtr->GetMotionCtrlThreadDeleted();
		if ( true == IsExit ) 
		{	break;	}

		ThreadCmd = MotionCtrlPtr->GetMotionThreadCmd_GoStop();
		ThreadState = MotionCtrlPtr->GetMotionThreadState_GoStop();
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{	break; }

		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd )
		{
			MotionCtrlPtr->SetMotionThreadState_GoStop(THREAD_STATE_IDLE);
			::Sleep(SleepTime);
			continue;
		}

		if ( THREAD_COMMAND_TO_RUN == ThreadCmd ) 
		{
			if ( THREAD_STATE_FINISH==ThreadState || THREAD_STATE_EXCEPTION==ThreadState)
			{
				::Sleep(SleepTime);
				continue;
			}
			::SetThreadPriority(MotionCtrlThreadHandle_GoStop, THREAD_PRIORITY_ABOVE_NORMAL);
			MotionCtrlPtr->SetMotionThreadState_GoStop(THREAD_STATE_RUNNING);
			QueryPerformanceCounter(&nStartTime);
			IsOK = MotionCtrlPtr->ExecMotionThreadFn_GoStop();
			QueryPerformanceCounter(&nEndTime);
			CameraCtrl.ResetBatchGrabbing();
			MotionCtrlPtr->SetIsWaitForInPosition(AXIS_X, true);
			MotionCtrlPtr->SetIsWaitForInPosition(AXIS_Y, true);
			if ( false == IsOK )
			{
				CString ErrorMSG = MotionCtrlPtr->GetErrorString();
				AOIDataCollect.SetIsSystemException(true);
				MotionCtrlPtr->SetMotionThreadState_GoStop(THREAD_STATE_EXCEPTION);
				AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread_Chk(AOI_EXCEPTION_THREAD_EXECUTION, true, ErrorMSG);
				MotionCtrlPtr->PostMotionCallbackWndMessage(MSG_SYSTEM_EXCEPTION_CALLBACK, NULL, LPARAM_SYSTEM_EXCEPTION_MOTION);				
			}
			else
			{	
				MotionCtrlPtr->SetMotionThreadState_GoStop(THREAD_STATE_FINISH);
				MotionCtrlPtr->PostMotionCallbackWndMessage(MSG_MOTION_CALLBACK, WPARAM_MOTION_PROCESS_FINISH_CALLBACK, NULL);				
			}
			//MotionCtrlPtr->SetMotionThreadCmd_GoStop(THREAD_COMMAND_TO_IDLE);

			if ( NULL != MotionCtrlThreadEvent_GoStop )
			{	::SetEvent(MotionCtrlThreadEvent_GoStop); }

			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;			
			::_stprintf(strBuffer, _T("MotionCtrlThreadFn_GoStop[%d]: %.2f ms"), 1, Time); 
			AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);	

			::SetThreadPriority(MotionCtrlThreadHandle_GoStop, THREAD_PRIORITY_BELOW_NORMAL);
			continue;
		}
		::Sleep(SleepTime);
	}
	MotionCtrlPtr->SetMotionThreadState_GoStop(THREAD_STATE_NONE);
	::_endthreadex(0);	
#endif	
	return 0;
}
//----------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//	
IMPLEMENT_DYNAMIC(CMotion_Basic, CObject)
//-------------------------------------------------------------------------------------//
bool CMotion_Basic::SetMotionParameterStringByID(MOTION_PARAM_ID ParamID, TMotionParameter &MotionParam, LPCTSTR String)//設定運動參數
{
	bool   IsOK=true;
	int    tempI=0;
	double tempD=0.0;
	switch ( ParamID )
	{	
	case MOTION_PARAM_SAVE_MOTION_CARD_PARAM://是否儲存運動卡參數
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			MotionParam.m_SaveMotionCardParam = tempI;
			break;
		default:	IsOK = false; break;
		}
		break;
	case MOTION_PARAM_SAVE_CURRENT_PROCESS://是否儲存運動訊息
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			MotionParam.m_SaveCurrentMotionProcess = tempI;
			break;
		default:	IsOK = false; break;
		}		
		break;
	case MOTION_PARAM_ACCELERATION_ADJUST://加速度調整
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			MotionParam.m_AccelerationAdjust = tempI;
			break;
		default:	IsOK = false; break;
		}		
		break;
	case MOTION_PARAM_XY_CALI_ENABLE://是否XY座標補正		
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			MotionParam.m_XYCaliEnable = tempI;
			break;
		default:	IsOK = false; break;
		}
		break;
	case MOTION_PARAM_XY_CALI_EXTEND_RANGE://XY座標補正外擴範圍	
		MotionParam.m_XYCaliExtendRange = ::_ttof(String);
		if ( MotionParam.m_XYCaliExtendRange < 0 )
		{	MotionParam.m_XYCaliExtendRange = 0.0; }
		break;
	case MOTION_PARAM_S_CURVE_VELOCITY_RATIO://S-Curve速度比例 	
		MotionParam.m_SCurveVelRatio = ::_ttof(String);
		if ( MotionParam.m_SCurveVelRatio < 0.01 ) 
		{	MotionParam.m_SCurveVelRatio = 0.01; }
		else if ( MotionParam.m_SCurveVelRatio > 100.0 ) 
		{	MotionParam.m_SCurveVelRatio = 100.0; }
		break;	
	case MOTION_PARAM_SIGN_POSITIVE_CONVERT://內部方向性轉換
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			MotionParam.m_SignPositiveConvert = tempI;
			break;
		default:	IsOK = false; break;
		}
		break;
	case MOTION_PARAM_USING_ECAT_NODE_ALIAS_NAME://是否啟用節點別名模式
		tempI = ::_ttoi(String);
		switch (tempI)
		{
		case FN_ENABLE:
		case FN_DISABLE:
			MotionParam.m_UsingEcatNodeAliasName = tempI;
			break;
		default:	IsOK = false; break;
		}
		break;
	case MOTION_PARAM_SIGN_POSITIVE_X://X軸與Cad的正負方向
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			MotionParam.m_SignPositiveX = tempI;
			break;
		default:	IsOK = false; break;
		}
		break;
	case MOTION_PARAM_SIGN_POSITIVE_Y://Y軸與Cad的正負方向
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			MotionParam.m_SignPositiveY = tempI;
			break;
		default:	IsOK = false; break;
		}
		break;
	case MOTION_PARAM_SIGN_POSITIVE_Z://Z軸與Cad的正負方向
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			MotionParam.m_SignPositiveZ = tempI;
			break;
		default:	IsOK = false; break;
		}
		break;
	case MOTION_PARAM_HOME_PRE_DIST_X://X軸歸零前移動距離		
		MotionParam.m_HomePreMoveDisX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_HOME_PRE_DIST_Y://Y軸歸零前移動距離		
		MotionParam.m_HomePreMoveDisY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_HOME_PRE_DIST_Z://Z軸歸零前移動距離		
		MotionParam.m_HomePreMoveDisZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_HOME_ORG_OFFSET_X://X軸歸零後的偏移值		
		MotionParam.m_HomeOrgOffsetX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_HOME_ORG_OFFSET_Y://Y軸歸零後的偏移值		
		MotionParam.m_HomeOrgOffsetY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_HOME_ORG_OFFSET_Z://Z軸歸零後的偏移值		
		MotionParam.m_HomeOrgOffsetZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_START_VELOCITY://起始速度  		
		MotionParam.m_StartVelocity = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_HOME_VELOCITY_X://X軸的歸零速度		
		MotionParam.m_HomeVelocityX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_HOME_VELOCITY_Y://Y軸的歸零速度		
		MotionParam.m_HomeVelocityY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_HOME_VELOCITY_Z://Z軸的歸零速度		
		MotionParam.m_HomeVelocityZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_MAX_VELOCITY_X://X軸的一般移動最大速度:		
		MotionParam.m_MaxVelocityX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_MAX_VELOCITY_Y://Y軸的一般移動最大速度:		
		MotionParam.m_MaxVelocityY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_MAX_VELOCITY_Z://Z軸的一般移動最大速度:		
		MotionParam.m_MaxVelocityZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_BOARD_IN_VELOCITY_X://X軸的移動至進板速度		
		MotionParam.m_BoardInVelocityX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_BOARD_IN_VELOCITY_Y://Y軸的移動至進板速度		
		MotionParam.m_BoardInVelocityY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_BOARD_IN_VELOCITY_Z://Z軸的移動至進板速度		
		MotionParam.m_BoardInVelocityZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_BOARD_OUT_VELOCITY_X://X軸的移動至出板速度		
		MotionParam.m_BoardOutVelocityX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_BOARD_OUT_VELOCITY_Y://Y軸的移動至出板速度		
		MotionParam.m_BoardOutVelocityY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_BOARD_OUT_VELOCITY_Z://Z軸的移動至出板速度		
		MotionParam.m_BoardOutVelocityZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_GRABBING_VELOCITY_X://X軸的取像移動最大速度, 		
		MotionParam.m_GrabbingVelocityX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_GRABBING_VELOCITY_Y://Y軸的取像移動最大速度, 		
		MotionParam.m_GrabbingVelocityY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_GRABBING_VELOCITY_Z://Z軸的取像移動最大速度, 		
		MotionParam.m_GrabbingVelocityZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_GRAB_MAP_VELOCITY_X://X軸的取底圖移動速度, 
		MotionParam.m_GrabMapVelocityX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_GRAB_MAP_VELOCITY_Y://Y軸的取底圖移動速度, 
		MotionParam.m_GrabMapVelocityY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_GRAB_MAP_VELOCITY_Z://Z軸的取底圖移動速度,
		MotionParam.m_GrabMapVelocityZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_STAGE_START_POS_X://機台起始的位置X		
		MotionParam.m_StageStartPosX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_STAGE_START_POS_Y://機台起始的位置Y		
		MotionParam.m_StageStartPosY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_STAGE_START_POS_Z://機台起始的位置Z		
		MotionParam.m_StageStartPosZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_STAGE_LEAVE_POS_X://機台離開的位置X
		MotionParam.m_StageLeavePosX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_STAGE_LEAVE_POS_Y://機台離開的位置Y
		MotionParam.m_StageLeavePosY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_STAGE_LEAVE_POS_Z://機台離開的位置Z
		MotionParam.m_StageLeavePosZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_BEFORE_PCB_IN_POS_X://進板前座標-X		
		MotionParam.m_BeforePCBInPosX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_BEFORE_PCB_IN_POS_Y://進板前座標-Y		
		MotionParam.m_BeforePCBInPosY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_BEFORE_PCB_IN_POS_Z://進板前座標-Z		
		MotionParam.m_BeforePCBInPosZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_PCB_STOP_POS_X_LA://A軌道右停板的位置X		
		MotionParam.m_PCBStopRPosX_LA = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_PCB_STOP_POS_Y_LA://A軌道右停板的位置Y		
		MotionParam.m_PCBStopRPosY_LA = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_PCB_STOP_POS_Z_LA://A軌道右停板的位置Z		
		MotionParam.m_PCBStopRPosZ_LA = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_PCB_STOP_RIGHT_POS_X_LB://B軌道右停板的位置X		
		MotionParam.m_PCBStopRPosX_LB = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_PCB_STOP_RIGHT_POS_Y_LB://B軌道右停板的位置Y		
		MotionParam.m_PCBStopRPosY_LB = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_PCB_STOP_RIGHT_POS_Z_LB://B軌道右停板的位置Z		
		MotionParam.m_PCBStopRPosZ_LB = ::_tcstod(String, NULL);
		break;		
	case MOTION_PARAM_PCB_STOP_LEFT_POS_X_LA://A軌道左停板的位置X
		MotionParam.m_PCBStopLPosX_LA = ::_ttof(String);
		break;
	case MOTION_PARAM_PCB_STOP_LEFT_POS_Y_LA://A軌道左停板的位置Y
		MotionParam.m_PCBStopLPosY_LA = ::_ttof(String);
		break;
	case MOTION_PARAM_PCB_STOP_LEFT_POS_Z_LA://A軌道左停板的位置Z
		MotionParam.m_PCBStopLPosZ_LA = ::_ttof(String);
		break;
	case MOTION_PARAM_PCB_STOP_LEFT_POS_X_LB://B軌道左停板的位置X
		MotionParam.m_PCBStopLPosX_LB = ::_ttof(String);
		break;
	case MOTION_PARAM_PCB_STOP_LEFT_POS_Y_LB://B軌道左停板的位置Y
		MotionParam.m_PCBStopLPosY_LB = ::_ttof(String);
		break;
	case MOTION_PARAM_PCB_STOP_LEFT_POS_Z_LB://B軌道左停板的位置Z
		MotionParam.m_PCBStopLPosZ_LB = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_X_LA://A軌道LED右停板的位置X
		MotionParam.m_LaneLedStopRPosX_LA = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_Y_LA://A軌道LED右停板的位置Y
		MotionParam.m_LaneLedStopRPosY_LA = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_Z_LA://A軌道LED右停板的位置Z
		MotionParam.m_LaneLedStopRPosZ_LA = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_X_LA://A軌道LED右減速的位置X
		MotionParam.m_LaneLedSlowRPosX_LA = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_Y_LA://A軌道LED右減速的位置Y
		MotionParam.m_LaneLedSlowRPosY_LA = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_Z_LA://A軌道LED右減速的位置Z
		MotionParam.m_LaneLedSlowRPosZ_LA = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_X_LB://B軌道LED右停板的位置X
		MotionParam.m_LaneLedStopRPosX_LB = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_Y_LB://B軌道LED右停板的位置Y
		MotionParam.m_LaneLedStopRPosY_LB = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_Z_LB://B軌道LED右停板的位置Z
		MotionParam.m_LaneLedStopRPosZ_LB = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_X_LB://B軌道LED右減速的位置X
		MotionParam.m_LaneLedSlowRPosX_LB = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_Y_LB://B軌道LED右減速的位置Y
		MotionParam.m_LaneLedSlowRPosY_LB = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_Z_LB://B軌道LED右減速的位置Z
		MotionParam.m_LaneLedSlowRPosZ_LB = ::_ttof(String);
		break;	
	case MOTION_PARAM_LANE_LED_LEFT_STOP_POS_X_LA://A軌道LED左停板的位置X	
		MotionParam.m_LaneLedStopLPosX_LA = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_STOP_POS_Y_LA://A軌道LED左停板的位置Y
		MotionParam.m_LaneLedStopLPosY_LA = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_STOP_POS_Z_LA://A軌道LED左停板的位置Z
		MotionParam.m_LaneLedStopLPosZ_LA = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_X_LA://A軌道LED左減速的位置X
		MotionParam.m_LaneLedSlowLPosX_LA = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_Y_LA://A軌道LED左減速的位置Y
		MotionParam.m_LaneLedSlowLPosY_LA = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_Z_LA://A軌道LED左減速的位置Z
		MotionParam.m_LaneLedSlowLPosZ_LA = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_STOP_POS_X_LB://B軌道LED左停板的位置X
		MotionParam.m_LaneLedStopLPosX_LB = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_STOP_POS_Y_LB://B軌道LED左停板的位置Y
		MotionParam.m_LaneLedStopLPosY_LB = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_STOP_POS_Z_LB://B軌道LED左停板的位置Z
		MotionParam.m_LaneLedStopLPosZ_LB = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_X_LB://B軌道LED左減速的位置X
		MotionParam.m_LaneLedSlowLPosX_LB = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_Y_LB://B軌道LED左減速的位置Y
		MotionParam.m_LaneLedSlowLPosY_LB = ::_ttof(String);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_Z_LB://B軌道LED左減速的位置Z
		MotionParam.m_LaneLedSlowLPosZ_LB = ::_ttof(String);
		break;
	
	case MOTION_PARAM_IN_POSITION_DELAY_TIME://機台走定位後, 延遲時間	
		tempI = ::_ttoi(String);
		if ( tempI >= 0 )
		{	
			MotionParam.m_InPositionDelayTime = tempI;	
			MotionCtrlPtr->SetWaitForDoneDelayTime(tempI);
		}
		else 
		{	IsOK = false; }
		break;
	case MOTION_PARAM_WAIT_FOR_DONW_DWELL_TIME://等待移動停止的函式暫停時間 ms		
		tempI = ::_ttoi(String);
		if ( tempI >= 0 )
		{	MotionParam.m_WaitForDoneDwellTime = tempI;	}
		else 
		{	IsOK = false; }
		break;
	case MOTION_PARAM_PRE_TRIGGER_OFFSET_DIST://等速前的偏移位置		
		MotionParam.m_PreTriggerOffsetDis = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_POST_TRIGGER_OFFSET_DIST://停止前的偏移位置		
		MotionParam.m_PostTriggerOffsetDis = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_TRIGGER_START_VELOCITY:		
		MotionParam.m_TriggerStartVelocity = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_TRIGGER_MAX_VELOCITY://取回張數可以一致, 但是影像偶爾有問題		
		MotionParam.m_TriggerMaxVelocity = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_TRIGGER_ACCEL_TIME://Trigger軸加速度時間			
		MotionParam.m_TriggerAccelerationTime = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_TRIGGER_DECEL_TIME://Trigger軸減速度時間			
		MotionParam.m_TriggerDecelerationTime = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_TRIGGER_FORWARD_OFFSET://Trigger向前掃時, 和直接拍攝的Trigger差值, 用校正求得		
		MotionParam.m_TriggerForwardOffset = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_TRIGGER_BACKWARD_OFFSET://Trigger向前掃時, 和直接拍攝的Trigger差值, 用校正求得		
		MotionParam.m_TriggerBackwardOffset = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_TRIGGER_SUB_AXIS_OFFSET://Y軸移動的間距			
		MotionParam.m_TriggerSubAxisOffset = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_JOG_START_VELOCITY://自由移動時的初始速度		
		MotionParam.m_JogStartVelocity = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_JOG_MOVING_VELOCITY://自由移動時的移動速度 			
		MotionParam.m_JogMovingVelocity = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_JOG_MAX_VELOCITY://自由移動時的最大速度 			
		MotionParam.m_JogMaxVelocity = ::_tcstod(String, NULL);
		break;
	case MOTION_PARAM_JOG_ACCEL_TIME://自由移動時的加速度時間		
		MotionParam.m_JogAccelerationTime = ::_tcstod(String, NULL);
		break;
	case MOTION_PARAM_JOG_DECEL_TIME://自由移動時的減速度時間		
		MotionParam.m_JogDecelerationTime = ::_tcstod(String, NULL);
		break;
	case MOTION_PARAM_JOG_MAX_VELOCITY_X://X軸自由移動時的最大速度 	
		MotionParam.m_JogMaxVelocityX = ::_tcstod(String, NULL);
		break;
	case MOTION_PARAM_JOG_MAX_VELOCITY_Y://Y軸自由移動時的最大速度 	
		MotionParam.m_JogMaxVelocityY = ::_tcstod(String, NULL);
		break;
	case MOTION_PARAM_JOG_MAX_VELOCITY_Z://Z軸自由移動時的最大速度 	
		MotionParam.m_JogMaxVelocityZ = ::_tcstod(String, NULL);
		break;
	case MOTION_PARAM_JOG_MOVING_VELOCITY_X://X軸自由移動時的移動速度	
		MotionParam.m_JogMovingVelocityX = ::_tcstod(String, NULL);
		break;
	case MOTION_PARAM_JOG_MOVING_VELOCITY_Y://Y軸自由移動時的移動速度	
		MotionParam.m_JogMovingVelocityY = ::_tcstod(String, NULL);
		break;
	case MOTION_PARAM_JOG_MOVING_VELOCITY_Z://Z軸自由移動時的移動速度			
		MotionParam.m_JogMovingVelocityZ = ::_tcstod(String, NULL);
		break;
	case MOTION_PARAM_JOG_DIRECTION_X://X軸自由移動的方向
		MotionParam.m_JogDirectionX = ::_ttoi(String);
		break;
	case MOTION_PARAM_JOG_DIRECTION_Y://Y軸自由移動的方向
		MotionParam.m_JogDirectionY = ::_ttoi(String);
		break;
	case MOTION_PARAM_JOG_DIRECTION_Z://Z軸自由移動的方向
		MotionParam.m_JogDirectionZ = ::_ttoi(String);
		break;
	case MOTION_PARAM_ACCEL_VALUE_X://X軸加速度, 單位mm/s/s		
		MotionParam.m_AccelerationValueX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_ACCEL_VALUE_Y://Y軸加速度, 單位mm/s/s		
		MotionParam.m_AccelerationValueY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_ACCEL_VALUE_Z://Z軸加速度, 單位mm/s/s		
		MotionParam.m_AccelerationValueZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_ACCEL_TIME_X://X軸加速度時間, sec		
		MotionParam.m_AccelerationTimeX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_ACCEL_TIME_Y://Y軸加速度時間, sec		
		MotionParam.m_AccelerationTimeY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_ACCEL_TIME_Z://Z軸加速度時間, sec		
		MotionParam.m_AccelerationTimeZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_DECEL_TIME_X://X軸減速度時間, sec		
		MotionParam.m_DecelerationTimeX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_DECEL_TIME_Y://Y軸減速度時間, sec		
		MotionParam.m_DecelerationTimeY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_DECEL_TIME_Z://Z軸減速度時間, sec		
		MotionParam.m_DecelerationTimeZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_ACCEL_MIN_TIME_X://X軸加速度最小時間, sec		
		MotionParam.m_AccelerationMinTimeX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_ACCEL_MIN_TIME_Y://Y軸加速度最小時間, sec		
		MotionParam.m_AccelerationMinTimeY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_ACCEL_MIN_TIME_Z://Z軸加速度最小時間, sec		
		MotionParam.m_AccelerationMinTimeZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_ADJUST_ACCEL_DIST_X://X軸調整加速度啟動距離-單位um
		MotionParam.m_AdjustAccclerationDistX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_ADJUST_ACCEL_DIST_Y://Y軸調整加速度啟動距離-單位um
		MotionParam.m_AdjustAccclerationDistY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_ADJUST_ACCEL_DIST_Z://Z軸調整加速度啟動距離-單位um		
		MotionParam.m_AdjustAccclerationDistZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_MOVING_CURVE_MODE_X://X軸移動曲線
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case MOVE_CURVE_T:
		case MOVE_CURVE_S:
			MotionParam.m_MovingCurveModeX = (MOVE_CURVE_MODE)tempI;
			break;
		default:	IsOK = false; break;
		}		
		break;
	case MOTION_PARAM_MOVING_CURVE_MODE_Y://Y軸移動曲線
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case MOVE_CURVE_T:
		case MOVE_CURVE_S:
			MotionParam.m_MovingCurveModeY = (MOVE_CURVE_MODE)tempI;
			break;
		default:	IsOK = false; break;
		}
		break;
	case MOTION_PARAM_MOVING_CURVE_MODE_Z://Z軸移動曲線
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case MOVE_CURVE_T:
		case MOVE_CURVE_S:
			MotionParam.m_MovingCurveModeZ = (MOVE_CURVE_MODE)tempI;
			break;
		default:	IsOK = false; break;
		}
		break;	
	case MOTION_PARAM_ACC_TIME_ADJUST_MODE_X://X軸加減速度時間調整模式
		tempI = ::_ttoi(String);
		switch ( tempI )
		{	
		case ACC_TIME_ADJUST_FIX_T:
		case ACC_TIME_ADJUST_MIN_T:
		case ACC_TIME_ADJUST_GAMMA:
			MotionParam.m_AccTimeAdjustModeX = (ACC_TIME_ADJUST_MODE)tempI;
			break;
		default:
		case ACC_TIME_ADJUST_OFF:
			MotionParam.m_AccTimeAdjustModeX = ACC_TIME_ADJUST_OFF;
			break;
		}
		break;
	case MOTION_PARAM_ACC_TIME_ADJUST_MODE_Y://Y軸加減速度時間調整模式
		tempI = ::_ttoi(String);
		switch ( tempI )
		{	
		case ACC_TIME_ADJUST_FIX_T:
		case ACC_TIME_ADJUST_MIN_T:
		case ACC_TIME_ADJUST_GAMMA:
			MotionParam.m_AccTimeAdjustModeY = (ACC_TIME_ADJUST_MODE)tempI;
			break;
		default:
		case ACC_TIME_ADJUST_OFF:
			MotionParam.m_AccTimeAdjustModeY = ACC_TIME_ADJUST_OFF;
			break;
		}
		break;
	case MOTION_PARAM_ACC_TIME_ADJUST_MODE_Z://Z軸加減速度時間調整模式
		tempI = ::_ttoi(String);
		switch ( tempI )
		{	
		case ACC_TIME_ADJUST_FIX_T:
		case ACC_TIME_ADJUST_MIN_T:
		case ACC_TIME_ADJUST_GAMMA:
			MotionParam.m_AccTimeAdjustModeZ = (ACC_TIME_ADJUST_MODE)tempI;
			break;
		default:
		case ACC_TIME_ADJUST_OFF:
			MotionParam.m_AccTimeAdjustModeZ = ACC_TIME_ADJUST_OFF;
			break;
		}
		break;
	case MOTION_PARAM_TRIANGLE_CORRECTION_X://X軸三角波抑制
		MotionParam.m_TriangleCorrectionX = (bool)(::_ttoi(String));
		break;
	case MOTION_PARAM_TRIANGLE_CORRECTION_Y://Y軸三角波抑制
		MotionParam.m_TriangleCorrectionY = (bool)(::_ttoi(String));
		break;
	case MOTION_PARAM_TRIANGLE_CORRECTION_Z://Z軸三角波抑制	
		MotionParam.m_TriangleCorrectionZ = (bool)(::_ttoi(String));
		break;
	case MOTION_PARAM_TEST_TIME_DISTANCE_X://X軸測驗時間的偏移量, um
		MotionParam.m_TestTimeDistanceX = ::_ttof(String);
		break;
	case MOTION_PARAM_TEST_TIME_DISTANCE_Y://Y軸測驗時間的偏移量, um
		MotionParam.m_TestTimeDistanceY = ::_ttof(String);
		break;
	case MOTION_PARAM_TEST_TIME_DISTANCE_Z://Z軸測驗時間的偏移量, um	
		MotionParam.m_TestTimeDistanceZ = ::_ttof(String);
		break;	
	case MOTION_PARAM_LIMIT_MAX_X://X軸的最大範圍		
		MotionParam.m_LimitMaxX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_LIMIT_MAX_Y://Y軸的最大範圍		
		MotionParam.m_LimitMaxY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_LIMIT_MAX_Z://Z軸的最大範圍		
		MotionParam.m_LimitMaxZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_LIMIT_MIN_X://X軸的最小範圍		
		MotionParam.m_LimitMinX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_LIMIT_MIN_Y://Y軸的最小範圍		
		MotionParam.m_LimitMinY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_LIMIT_MIN_Z://Z軸的最小範圍		
		MotionParam.m_LimitMinZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_LIMIT_MARGIN_X://X軸的範圍內縮值		
		MotionParam.m_LimitMarginX = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_LIMIT_MARGIN_Y://Y軸的範圍內縮值		
		MotionParam.m_LimitMarginY = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_LIMIT_MARGIN_Z://Z軸的範圍內縮值		
		MotionParam.m_LimitMarginZ = ::_tcstod(String, NULL);;
		break;
	case MOTION_PARAM_ENABLE_SOFTWARE_LIMIT://是否啟用運動系統內部的軟體極限
		tempI = ::_ttoi(String);
		switch ( tempI )
		{
		case FN_ENABLE:
		case FN_DISABLE:
			MotionParam.m_UsingMotionSoftwareLimit = tempI;
			break;
		default:	IsOK = false; break;
		}	
		break;
	case MOTION_PARAM_CONTROLLER_PORT_X://X 控制器 Port
		tempI = ::_ttoi(String);
		MotionParam.m_ControllerPortX = tempI;
		break;
	case MOTION_PARAM_CONTROLLER_PORT_Y://Y 控制器 Port
		tempI = ::_ttoi(String);
		MotionParam.m_ControllerPortY = tempI;
		break;
	case MOTION_PARAM_CONTROLLER_PORT_Z://Z 控制器 Port
		tempI = ::_ttoi(String);
		MotionParam.m_ControllerPortZ = tempI;
		break;
	case MOTION_PARAM_CONTROLLER_IP_X://X 控制器 IP
		MotionParam.m_ControllerIPX = String;
		break;
	case MOTION_PARAM_CONTROLLER_IP_Y://Y 控制器 IP
		MotionParam.m_ControllerIPY = String;
		break;
	case MOTION_PARAM_CONTROLLER_IP_Z://Z 控制器 IP
		MotionParam.m_ControllerIPZ = String;
		break;
	default:
		IsOK = false;
		JetAPI::ShowMessageBox(_T("Error, Motion Param Not Defined"));
		break;
	}	
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CMotion_Basic::GetMotionParameterStringByID(MOTION_PARAM_ID ParamID, const TMotionParameter &MotionParam, CString &String)//取得運動參數	
{
	bool   IsOK=true;	
	switch ( ParamID )
	{	
	case MOTION_PARAM_SAVE_MOTION_CARD_PARAM://是否儲存運動卡參數
		String.Format(_T("%d"), MotionParam.m_SaveMotionCardParam);
		break;
	case MOTION_PARAM_SAVE_CURRENT_PROCESS://是否儲存運動訊息
		String.Format(_T("%d"), MotionParam.m_SaveCurrentMotionProcess);
		break;	
	case MOTION_PARAM_ACCELERATION_ADJUST://加速度調整
		String.Format(_T("%d"), MotionParam.m_AccelerationAdjust);		
		break;
	case MOTION_PARAM_XY_CALI_ENABLE://是否XY座標補正
		String.Format(_T("%d"), MotionParam.m_XYCaliEnable);
		break;
	case MOTION_PARAM_XY_CALI_EXTEND_RANGE://XY座標補正外擴範圍	
		String.Format(_T("%.16f"), MotionParam.m_XYCaliExtendRange);		
		break;
	case MOTION_PARAM_S_CURVE_VELOCITY_RATIO://S-Curve速度比例 	
		String.Format(_T("%.16f"), MotionParam.m_SCurveVelRatio);		
		break;	
	case MOTION_PARAM_SIGN_POSITIVE_CONVERT://內部方向性轉換
		String.Format(_T("%d"), MotionParam.m_SignPositiveConvert);
		break;
	case MOTION_PARAM_USING_ECAT_NODE_ALIAS_NAME://是否啟用節點別名模式
		String.Format(_T("%d"), MotionParam.m_UsingEcatNodeAliasName);
		break;
	case MOTION_PARAM_SIGN_POSITIVE_X://X軸與Cad的正負方向
		String.Format(_T("%d"), MotionParam.m_SignPositiveX);		
		break;
	case MOTION_PARAM_SIGN_POSITIVE_Y://Y軸與Cad的正負方向
		String.Format(_T("%d"), MotionParam.m_SignPositiveY);		
		break;
	case MOTION_PARAM_SIGN_POSITIVE_Z://Z軸與Cad的正負方向
		String.Format(_T("%d"), MotionParam.m_SignPositiveZ);		
		break;
	case MOTION_PARAM_HOME_PRE_DIST_X://X軸歸零前移動距離
		String.Format(_T("%.16f"), MotionParam.m_HomePreMoveDisX);		
		break;
	case MOTION_PARAM_HOME_PRE_DIST_Y://Y軸歸零前移動距離
		String.Format(_T("%.16f"), MotionParam.m_HomePreMoveDisY);
		break;
	case MOTION_PARAM_HOME_PRE_DIST_Z://Z軸歸零前移動距離
		String.Format(_T("%.16f"), MotionParam.m_HomePreMoveDisZ);
		break;
	case MOTION_PARAM_HOME_ORG_OFFSET_X://X軸歸零後的偏移值
		String.Format(_T("%.16f"), MotionParam.m_HomeOrgOffsetX);		
		break;
	case MOTION_PARAM_HOME_ORG_OFFSET_Y://Y軸歸零後的偏移值
		String.Format(_T("%.16f"), MotionParam.m_HomeOrgOffsetY);
		break;
	case MOTION_PARAM_HOME_ORG_OFFSET_Z://Z軸歸零後的偏移值
		String.Format(_T("%.16f"), MotionParam.m_HomeOrgOffsetZ);
		break;
	case MOTION_PARAM_START_VELOCITY://起始速度  
		String.Format(_T("%.16f"), MotionParam.m_StartVelocity);		
		break;
	case MOTION_PARAM_HOME_VELOCITY_X://X軸的歸零速度
		String.Format(_T("%.16f"), MotionParam.m_HomeVelocityX);		
		break;
	case MOTION_PARAM_HOME_VELOCITY_Y://Y軸的歸零速度
		String.Format(_T("%.16f"), MotionParam.m_HomeVelocityY);		
		break;
	case MOTION_PARAM_HOME_VELOCITY_Z://Z軸的歸零速度
		String.Format(_T("%.16f"), MotionParam.m_HomeVelocityZ);		
		break;
	case MOTION_PARAM_MAX_VELOCITY_X://X軸的一般移動最大速度:
		String.Format(_T("%.16f"), MotionParam.m_MaxVelocityX);		
		break;
	case MOTION_PARAM_MAX_VELOCITY_Y://Y軸的一般移動最大速度:
		String.Format(_T("%.16f"), MotionParam.m_MaxVelocityY);		
		break;
	case MOTION_PARAM_MAX_VELOCITY_Z://Z軸的一般移動最大速度:
		String.Format(_T("%.16f"), MotionParam.m_MaxVelocityZ);		
		break;
	case MOTION_PARAM_BOARD_IN_VELOCITY_X://X軸的移動至進板速度
		String.Format(_T("%.16f"), MotionParam.m_BoardInVelocityX);		
		break;
	case MOTION_PARAM_BOARD_IN_VELOCITY_Y://Y軸的移動至進板速度
		String.Format(_T("%.16f"), MotionParam.m_BoardInVelocityY);		
		break;
	case MOTION_PARAM_BOARD_IN_VELOCITY_Z://Z軸的移動至進板速度
		String.Format(_T("%.16f"), MotionParam.m_BoardInVelocityZ);		
		break;
	case MOTION_PARAM_BOARD_OUT_VELOCITY_X://X軸的移動至出板速度
		String.Format(_T("%.16f"), MotionParam.m_BoardOutVelocityX);		
		break;
	case MOTION_PARAM_BOARD_OUT_VELOCITY_Y://Y軸的移動至出板速度
		String.Format(_T("%.16f"), MotionParam.m_BoardOutVelocityY);		
		break;
	case MOTION_PARAM_BOARD_OUT_VELOCITY_Z://Z軸的移動至出板速度
		String.Format(_T("%.16f"), MotionParam.m_BoardOutVelocityZ);		
		break;			
	case MOTION_PARAM_GRABBING_VELOCITY_X://X軸的取像移動最大速度, 
		String.Format(_T("%.16f"), MotionParam.m_GrabbingVelocityX);
		break;
	case MOTION_PARAM_GRABBING_VELOCITY_Y://Y軸的取像移動最大速度, 
		String.Format(_T("%.16f"), MotionParam.m_GrabbingVelocityY);
		break;
	case MOTION_PARAM_GRABBING_VELOCITY_Z://Z軸的取像移動最大速度, 
		String.Format(_T("%.16f"), MotionParam.m_GrabbingVelocityZ);
		break;
	case MOTION_PARAM_GRAB_MAP_VELOCITY_X://X軸的取底圖移動速度, 
		String.Format(_T("%.16f"), MotionParam.m_GrabMapVelocityX);
		break;
	case MOTION_PARAM_GRAB_MAP_VELOCITY_Y://Y軸的取底圖移動速度, 
		String.Format(_T("%.16f"), MotionParam.m_GrabMapVelocityY);
		break;
	case MOTION_PARAM_GRAB_MAP_VELOCITY_Z://Z軸的取底圖移動速度, 
		String.Format(_T("%.16f"), MotionParam.m_GrabMapVelocityZ);
		break;
	case MOTION_PARAM_STAGE_START_POS_X://機台起始的位置X
		String.Format(_T("%.16f"), MotionParam.m_StageStartPosX);		
		break;
	case MOTION_PARAM_STAGE_START_POS_Y://機台起始的位置Y
		String.Format(_T("%.16f"), MotionParam.m_StageStartPosY);
		break;
	case MOTION_PARAM_STAGE_START_POS_Z://機台起始的位置Z
		String.Format(_T("%.16f"), MotionParam.m_StageStartPosZ);		
		break;
	case MOTION_PARAM_STAGE_LEAVE_POS_X://機台離開的位置X
		String.Format(_T("%.16f"), MotionParam.m_StageLeavePosX);		
		break;		
	case MOTION_PARAM_STAGE_LEAVE_POS_Y://機台離開的位置Y
		String.Format(_T("%.16f"), MotionParam.m_StageLeavePosY);		
		break;		
	case MOTION_PARAM_STAGE_LEAVE_POS_Z://機台離開的位置Z
		String.Format(_T("%.16f"), MotionParam.m_StageLeavePosZ);
		break;		
	case MOTION_PARAM_BEFORE_PCB_IN_POS_X://進板前座標-X
		String.Format(_T("%.16f"), MotionParam.m_BeforePCBInPosX);		
		break;
	case MOTION_PARAM_BEFORE_PCB_IN_POS_Y://進板前座標-Y
		String.Format(_T("%.16f"), MotionParam.m_BeforePCBInPosY);		
		break;
	case MOTION_PARAM_BEFORE_PCB_IN_POS_Z://進板前座標-Z
		String.Format(_T("%.16f"), MotionParam.m_BeforePCBInPosZ);		
		break;
	case MOTION_PARAM_PCB_STOP_POS_X_LA://A軌道右停板的位置X
		String.Format(_T("%.16f"), MotionParam.m_PCBStopRPosX_LA);		
		break;
	case MOTION_PARAM_PCB_STOP_POS_Y_LA://A軌道右停板的位置Y
		String.Format(_T("%.16f"), MotionParam.m_PCBStopRPosY_LA);
		break;
	case MOTION_PARAM_PCB_STOP_POS_Z_LA://A軌道右停板的位置Z
		String.Format(_T("%.16f"), MotionParam.m_PCBStopRPosZ_LA);
		break;
	case MOTION_PARAM_PCB_STOP_RIGHT_POS_X_LB://B軌道右停板的位置X
		String.Format(_T("%.16f"), MotionParam.m_PCBStopRPosX_LB);		
		break;
	case MOTION_PARAM_PCB_STOP_RIGHT_POS_Y_LB://B軌道右停板的位置Y
		String.Format(_T("%.16f"), MotionParam.m_PCBStopRPosY_LB);
		break;
	case MOTION_PARAM_PCB_STOP_RIGHT_POS_Z_LB://B軌道右停板的位置Z
		String.Format(_T("%.16f"), MotionParam.m_PCBStopRPosZ_LB);
		break;
	case MOTION_PARAM_PCB_STOP_LEFT_POS_X_LA://A軌道左停板的位置X
		String.Format(_T("%.16f"), MotionParam.m_PCBStopLPosX_LA);
		break;
	case MOTION_PARAM_PCB_STOP_LEFT_POS_Y_LA://A軌道左停板的位置Y
		String.Format(_T("%.16f"), MotionParam.m_PCBStopLPosY_LA);
		break;
	case MOTION_PARAM_PCB_STOP_LEFT_POS_Z_LA://A軌道左停板的位置Z
		String.Format(_T("%.16f"), MotionParam.m_PCBStopLPosZ_LA);
		break;
	case MOTION_PARAM_PCB_STOP_LEFT_POS_X_LB://B軌道左停板的位置X
		String.Format(_T("%.16f"), MotionParam.m_PCBStopLPosX_LB);
		break;
	case MOTION_PARAM_PCB_STOP_LEFT_POS_Y_LB://B軌道左停板的位置Y
		String.Format(_T("%.16f"), MotionParam.m_PCBStopLPosY_LB);
		break;
	case MOTION_PARAM_PCB_STOP_LEFT_POS_Z_LB://B軌道左停板的位置Z
		String.Format(_T("%.16f"), MotionParam.m_PCBStopLPosZ_LB);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_X_LA://A軌道LED停板的位置X
		String.Format(_T("%.16f"), MotionParam.m_LaneLedStopRPosX_LA);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_Y_LA://A軌道LED停板的位置Y
		String.Format(_T("%.16f"), MotionParam.m_LaneLedStopRPosY_LA);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_Z_LA://A軌道LED停板的位置Z
		String.Format(_T("%.16f"), MotionParam.m_LaneLedStopRPosZ_LA);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_X_LA://A軌道LED減速的位置X
		String.Format(_T("%.16f"), MotionParam.m_LaneLedSlowRPosX_LA);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_Y_LA://A軌道LED減速的位置Y
		String.Format(_T("%.16f"), MotionParam.m_LaneLedSlowRPosY_LA);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_Z_LA://A軌道LED減速的位置Z
		String.Format(_T("%.16f"), MotionParam.m_LaneLedSlowRPosZ_LA);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_X_LB://B軌道LED停板的位置X
		String.Format(_T("%.16f"), MotionParam.m_LaneLedStopRPosX_LB);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_Y_LB://B軌道LED停板的位置Y
		String.Format(_T("%.16f"), MotionParam.m_LaneLedStopRPosY_LB);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_STOP_POS_Z_LB://B軌道LED停板的位置Z
		String.Format(_T("%.16f"), MotionParam.m_LaneLedStopRPosZ_LB);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_X_LB://B軌道LED減速的位置X
		String.Format(_T("%.16f"), MotionParam.m_LaneLedSlowRPosX_LB);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_Y_LB://B軌道LED減速的位置Y
		String.Format(_T("%.16f"), MotionParam.m_LaneLedSlowRPosY_LB);
		break;
	case MOTION_PARAM_LANE_LED_RIGHT_SLOW_POS_Z_LB://B軌道LED減速的位置Z
		String.Format(_T("%.16f"), MotionParam.m_LaneLedSlowRPosZ_LB);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_STOP_POS_X_LA://A軌道LED左停板的位置X	
		String.Format(_T("%.16f"), MotionParam.m_LaneLedStopLPosX_LA);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_STOP_POS_Y_LA://A軌道LED左停板的位置Y
		String.Format(_T("%.16f"), MotionParam.m_LaneLedStopLPosY_LA);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_STOP_POS_Z_LA://A軌道LED左停板的位置Z
		String.Format(_T("%.16f"), MotionParam.m_LaneLedStopLPosZ_LA);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_X_LA://A軌道LED左減速的位置X
		String.Format(_T("%.16f"), MotionParam.m_LaneLedSlowLPosX_LA);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_Y_LA://A軌道LED左減速的位置Y
		String.Format(_T("%.16f"), MotionParam.m_LaneLedSlowLPosY_LA);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_Z_LA://A軌道LED左減速的位置Z
		String.Format(_T("%.16f"), MotionParam.m_LaneLedSlowLPosZ_LA);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_STOP_POS_X_LB://B軌道LED左停板的位置X
		String.Format(_T("%.16f"), MotionParam.m_LaneLedStopLPosX_LB);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_STOP_POS_Y_LB://B軌道LED左停板的位置Y
		String.Format(_T("%.16f"), MotionParam.m_LaneLedStopLPosY_LB);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_STOP_POS_Z_LB://B軌道LED左停板的位置Z
		String.Format(_T("%.16f"), MotionParam.m_LaneLedStopLPosZ_LB);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_X_LB://B軌道LED左減速的位置X
		String.Format(_T("%.16f"), MotionParam.m_LaneLedSlowLPosX_LB);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_Y_LB://B軌道LED左減速的位置Y
		String.Format(_T("%.16f"), MotionParam.m_LaneLedSlowLPosY_LB);
		break;
	case MOTION_PARAM_LANE_LED_LEFT_SLOW_POS_Z_LB://B軌道LED左減速的位置Z
		String.Format(_T("%.16f"), MotionParam.m_LaneLedSlowLPosZ_LB);
		break;
	
	case MOTION_PARAM_IN_POSITION_DELAY_TIME://機台走定位後, 延遲時間	
		String.Format(_T("%d"), MotionParam.m_InPositionDelayTime);		
		break;
	case MOTION_PARAM_WAIT_FOR_DONW_DWELL_TIME://等待移動停止的函式暫停時間 ms		
		String.Format(_T("%d"), MotionParam.m_WaitForDoneDwellTime);
		break;
	case MOTION_PARAM_PRE_TRIGGER_OFFSET_DIST://等速前的偏移位置
		String.Format(_T("%.16f"), MotionParam.m_PreTriggerOffsetDis);		
		break;
	case MOTION_PARAM_POST_TRIGGER_OFFSET_DIST://停止前的偏移位置
		String.Format(_T("%.16f"), MotionParam.m_PostTriggerOffsetDis);		
		break;
	case MOTION_PARAM_TRIGGER_START_VELOCITY:
		String.Format(_T("%.16f"), MotionParam.m_TriggerStartVelocity);		
		break;
	case MOTION_PARAM_TRIGGER_MAX_VELOCITY://取回張數可以一致, 但是影像偶爾有問題
		String.Format(_T("%.16f"), MotionParam.m_TriggerMaxVelocity);		
		break;
	case MOTION_PARAM_TRIGGER_ACCEL_TIME://Trigger軸加速度時間	
		String.Format(_T("%.16f"), MotionParam.m_TriggerAccelerationTime);		
		break;
	case MOTION_PARAM_TRIGGER_DECEL_TIME://Trigger軸減速度時間	
		String.Format(_T("%.16f"), MotionParam.m_TriggerDecelerationTime);		
		break;
	case MOTION_PARAM_TRIGGER_FORWARD_OFFSET://Trigger向前掃時, 和直接拍攝的Trigger差值, 用校正求得
		String.Format(_T("%.16f"), MotionParam.m_TriggerForwardOffset);		
		break;
	case MOTION_PARAM_TRIGGER_BACKWARD_OFFSET://Trigger向前掃時, 和直接拍攝的Trigger差值, 用校正求得
		String.Format(_T("%.16f"), MotionParam.m_TriggerBackwardOffset);		
		break;
	case MOTION_PARAM_TRIGGER_SUB_AXIS_OFFSET://Y軸移動的間距	
		String.Format(_T("%.16f"), MotionParam.m_TriggerSubAxisOffset);		
		break;
	case MOTION_PARAM_JOG_START_VELOCITY://自由移動時的初始速度
		String.Format(_T("%.16f"), MotionParam.m_JogStartVelocity);		
		break;
	case MOTION_PARAM_JOG_MOVING_VELOCITY://自由移動時的移動速度 	
		String.Format(_T("%.16f"), MotionParam.m_JogMovingVelocity);		
		break;
	case MOTION_PARAM_JOG_MAX_VELOCITY://自由移動時的最大速度 	
		String.Format(_T("%.16f"), MotionParam.m_JogMaxVelocity);		
		break;
	case MOTION_PARAM_JOG_ACCEL_TIME://自由移動時的加速度時間
		String.Format(_T("%.16f"), MotionParam.m_JogAccelerationTime);		
		break;
	case MOTION_PARAM_JOG_DECEL_TIME://自由移動時的減速度時間
		String.Format(_T("%.16f"), MotionParam.m_JogDecelerationTime);		
		break;
	case MOTION_PARAM_JOG_MAX_VELOCITY_X://X軸自由移動時的最大速度 	
		String.Format(_T("%.16f"), MotionParam.m_JogMaxVelocityX);
		break;
	case MOTION_PARAM_JOG_MAX_VELOCITY_Y://Y軸自由移動時的最大速度 	
		String.Format(_T("%.16f"), MotionParam.m_JogMaxVelocityY);
		break;
	case MOTION_PARAM_JOG_MAX_VELOCITY_Z://Z軸自由移動時的最大速度 	
		String.Format(_T("%.16f"), MotionParam.m_JogMaxVelocityZ);
		break;
	case MOTION_PARAM_JOG_MOVING_VELOCITY_X://X軸自由移動時的移動速度	
		String.Format(_T("%.16f"), MotionParam.m_JogMovingVelocityX);
		break;
	case MOTION_PARAM_JOG_MOVING_VELOCITY_Y://Y軸自由移動時的移動速度	
		String.Format(_T("%.16f"), MotionParam.m_JogMovingVelocityY);
		break;
	case MOTION_PARAM_JOG_MOVING_VELOCITY_Z://Z軸自由移動時的移動速度		
		String.Format(_T("%.16f"), MotionParam.m_JogMovingVelocityZ);
		break;
	case MOTION_PARAM_JOG_DIRECTION_X://X軸自由移動的方向
		String.Format(_T("%d"), MotionParam.m_JogDirectionX);
		break;
	case MOTION_PARAM_JOG_DIRECTION_Y://Y軸自由移動的方向
		String.Format(_T("%d"), MotionParam.m_JogDirectionY);
		break;
	case MOTION_PARAM_JOG_DIRECTION_Z://Z軸自由移動的方向
		String.Format(_T("%d"), MotionParam.m_JogDirectionZ);
		break;
	case MOTION_PARAM_ACCEL_VALUE_X://X軸加速度, 單位mm/s/s
		String.Format(_T("%.16f"), MotionParam.m_AccelerationValueX);		
		break;
	case MOTION_PARAM_ACCEL_VALUE_Y://Y軸加速度, 單位mm/s/s
		String.Format(_T("%.16f"), MotionParam.m_AccelerationValueY);
		break;
	case MOTION_PARAM_ACCEL_VALUE_Z://Z軸加速度, 單位mm/s/s
		String.Format(_T("%.16f"), MotionParam.m_AccelerationValueZ);
		break;
	case MOTION_PARAM_ACCEL_TIME_X://X軸加速度時間, sec
		String.Format(_T("%.16f"), MotionParam.m_AccelerationTimeX);		
		break;
	case MOTION_PARAM_ACCEL_TIME_Y://Y軸加速度時間, sec
		String.Format(_T("%.16f"), MotionParam.m_AccelerationTimeY);
		break;
	case MOTION_PARAM_ACCEL_TIME_Z://Z軸加速度時間, sec
		String.Format(_T("%.16f"), MotionParam.m_AccelerationTimeZ);
		break;
	case MOTION_PARAM_DECEL_TIME_X://X軸減速度時間, sec
		String.Format(_T("%.16f"), MotionParam.m_DecelerationTimeX);		
		break;
	case MOTION_PARAM_DECEL_TIME_Y://Y軸減速度時間, sec
		String.Format(_T("%.16f"), MotionParam.m_DecelerationTimeY);
		break;
	case MOTION_PARAM_DECEL_TIME_Z://Z軸減速度時間, sec
		String.Format(_T("%.16f"), MotionParam.m_DecelerationTimeZ);
		break;
	case MOTION_PARAM_ACCEL_MIN_TIME_X://X軸加速度最小時間, sec
		String.Format(_T("%.16f"), MotionParam.m_AccelerationMinTimeX);		
		break;
	case MOTION_PARAM_ACCEL_MIN_TIME_Y://Y軸加速度最小時間, sec
		String.Format(_T("%.16f"), MotionParam.m_AccelerationMinTimeY);
		break;
	case MOTION_PARAM_ACCEL_MIN_TIME_Z://Z軸加速度最小時間, sec
		String.Format(_T("%.16f"), MotionParam.m_AccelerationMinTimeZ);
		break;
	case MOTION_PARAM_ADJUST_ACCEL_DIST_X://X軸調整加速度啟動距離-單位um
		String.Format(_T("%.16f"), MotionParam.m_AdjustAccclerationDistX);		
		break;
	case MOTION_PARAM_ADJUST_ACCEL_DIST_Y://Y軸調整加速度啟動距離-單位um
		String.Format(_T("%.16f"), MotionParam.m_AdjustAccclerationDistY);
		break;
	case MOTION_PARAM_ADJUST_ACCEL_DIST_Z://Z軸調整加速度啟動距離-單位um	
		String.Format(_T("%.16f"), MotionParam.m_AdjustAccclerationDistZ);
		break;
	case MOTION_PARAM_MOVING_CURVE_MODE_X://X軸移動曲線
		String.Format(_T("%d"), MotionParam.m_MovingCurveModeX);
		break;
	case MOTION_PARAM_MOVING_CURVE_MODE_Y://Y軸移動曲線
		String.Format(_T("%d"), MotionParam.m_MovingCurveModeY);
		break;
	case MOTION_PARAM_MOVING_CURVE_MODE_Z://Z軸移動曲線
		String.Format(_T("%d"), MotionParam.m_MovingCurveModeZ);
		break;
	case MOTION_PARAM_ACC_TIME_ADJUST_MODE_X://X軸加減速度時間調整模式
		String.Format(_T("%d"), MotionParam.m_AccTimeAdjustModeX);
		break;
	case MOTION_PARAM_ACC_TIME_ADJUST_MODE_Y://Y軸加減速度時間調整模式
		String.Format(_T("%d"), MotionParam.m_AccTimeAdjustModeY);
		break;
	case MOTION_PARAM_ACC_TIME_ADJUST_MODE_Z://Z軸加減速度時間調整模式
		String.Format(_T("%d"), MotionParam.m_AccTimeAdjustModeZ);
		break;	
	case MOTION_PARAM_TRIANGLE_CORRECTION_X://X軸三角波抑制
		String.Format(_T("%d"), MotionParam.m_TriangleCorrectionX);
		break;
	case MOTION_PARAM_TRIANGLE_CORRECTION_Y://Y軸三角波抑制
		String.Format(_T("%d"), MotionParam.m_TriangleCorrectionY);
		break;
	case MOTION_PARAM_TRIANGLE_CORRECTION_Z://Z軸三角波抑制	
		String.Format(_T("%d"), MotionParam.m_TriangleCorrectionZ);
		break;
	case MOTION_PARAM_TEST_TIME_DISTANCE_X://X軸測驗時間的偏移量, um
		String.Format(_T("%.16f"), MotionParam.m_TestTimeDistanceX);
		break;
	case MOTION_PARAM_TEST_TIME_DISTANCE_Y://Y軸測驗時間的偏移量, um
		String.Format(_T("%.16f"), MotionParam.m_TestTimeDistanceY);
		break;
	case MOTION_PARAM_TEST_TIME_DISTANCE_Z://Z軸測驗時間的偏移量, um
		String.Format(_T("%.16f"), MotionParam.m_TestTimeDistanceZ);
		break;	
	case MOTION_PARAM_LIMIT_MAX_X://X軸的最大範圍
		String.Format(_T("%.16f"), MotionParam.m_LimitMaxX);		
		break;
	case MOTION_PARAM_LIMIT_MAX_Y://Y軸的最大範圍
		String.Format(_T("%.16f"), MotionParam.m_LimitMaxY);
		break;
	case MOTION_PARAM_LIMIT_MAX_Z://Z軸的最大範圍
		String.Format(_T("%.16f"), MotionParam.m_LimitMaxZ);
		break;
	case MOTION_PARAM_LIMIT_MIN_X://X軸的最小範圍
		String.Format(_T("%.16f"), MotionParam.m_LimitMinX);		
		break;
	case MOTION_PARAM_LIMIT_MIN_Y://Y軸的最小範圍
		String.Format(_T("%.16f"), MotionParam.m_LimitMinY);
		break;
	case MOTION_PARAM_LIMIT_MIN_Z://Z軸的最小範圍
		String.Format(_T("%.16f"), MotionParam.m_LimitMinZ);
		break;
	case MOTION_PARAM_LIMIT_MARGIN_X://X軸的範圍內縮值
		String.Format(_T("%.16f"), MotionParam.m_LimitMarginX);		
		break;
	case MOTION_PARAM_LIMIT_MARGIN_Y://Y軸的範圍內縮值
		String.Format(_T("%.16f"), MotionParam.m_LimitMarginY);		
		break;
	case MOTION_PARAM_LIMIT_MARGIN_Z://Z軸的範圍內縮值
		String.Format(_T("%.16f"), MotionParam.m_LimitMarginZ);
		break;
	case MOTION_PARAM_ENABLE_SOFTWARE_LIMIT://是否啟用運動系統內部的軟體極限
		String.Format(_T("%d"), MotionParam.m_UsingMotionSoftwareLimit);		
		break;
	case MOTION_PARAM_CONTROLLER_PORT_X://X 控制器 Port
		String.Format(_T("%d"), MotionParam.m_ControllerPortX);		
		break;
	case MOTION_PARAM_CONTROLLER_PORT_Y://Y 控制器 Port
		String.Format(_T("%d"), MotionParam.m_ControllerPortY);		
		break;
	case MOTION_PARAM_CONTROLLER_PORT_Z://Z 控制器 Port
		String.Format(_T("%d"), MotionParam.m_ControllerPortZ);
		break;
	case MOTION_PARAM_CONTROLLER_IP_X://X 控制器 IP
		String = MotionParam.m_ControllerIPX;		
		break;
	case MOTION_PARAM_CONTROLLER_IP_Y://Y 控制器 IP
		String = MotionParam.m_ControllerIPY;
		break;
	case MOTION_PARAM_CONTROLLER_IP_Z://Z 控制器 IP
		String = MotionParam.m_ControllerIPZ;
		break;
	}
	return IsOK;
	return true;
}
//-------------------------------------------------------------------------------------//
CMotion_Basic::CMotion_Basic()
{
	::InitializeCriticalSection(&m_csMotion);
	::InitializeCriticalSection(&m_csMotion_X);
	::InitializeCriticalSection(&m_csMotion_Y);
	::InitializeCriticalSection(&m_csMotion_Z);
	this->PreInitMotion();
	this->LoadMotionParameter();
	this->SaveMotionParameter();
}
//-------------------------------------------------------------------------------------//
CMotion_Basic::~CMotion_Basic()
{
	::DeleteCriticalSection(&m_csMotion_Z);
	::DeleteCriticalSection(&m_csMotion_Y);
	::DeleteCriticalSection(&m_csMotion_X);
	::DeleteCriticalSection(&m_csMotion);
}
//-------------------------------------------------------------------------------------//
bool CMotion_Basic::LoadMotionNodeList()
{
	size_t  i=0;	
	CString Section;	
	CString Filename=GetMotionNodeListFilename();
	std::vector<CMotionNode> &MotionNodeList=GetMotionNodeList();
	MotionNodeList.clear();

	size_t Count=0;//XYZ*2=6;
	const size_t textlen = 128;	
	CString KeyName = _T("");
	CString Default = _T("");	
	TCHAR   String[textlen]=_T("");	

	Section = _T("Motion Node Info");
	KeyName.Format(_T("Motion Node Count"));
	Default.Format(_T("%d"), Count);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	Count = (::_ttoi(String)); }	

	for ( i=0; i<Count; i++ )
	{	
		CMotionNode MotionNode;
		Section.Format(_T("Motion Node %02d"), i+1);		
		if ( MotionNode.LoadIniFile(Section, Filename) == false ) { continue; }
		if ( MotionNode.CheckAxisIDValid()  == false ) { continue; }
		MotionNodeList.push_back(MotionNode);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Basic::SaveMotionNodeList()
{
	size_t  i=0;	
	CString Section;
	CString Filename=GetMotionNodeListFilename();
	std::vector<CMotionNode> &MotionNodeList=GetMotionNodeList();
	const size_t Count=MotionNodeList.size();
	
	CString KeyName = _T("");
	CString String = _T("");
	Section = _T("Motion Node Info");
	KeyName.Format(_T("Motion Node Count"));
	String.Format(_T("%d"), Count);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	for ( i=0; i<Count; i++ )
	{	
		Section.Format(_T("Motion Node %02d"), i+1);
		CMotionNode &MotionNodeRef=MotionNodeList[i];		
		MotionNodeRef.SaveIniFile(Section, Filename);			
	}
	return true;
}
//-------------------------------------------------------------------------------------//
CString CMotion_Basic::GetMotionNodeListFilename() const
{	
	CString Filename;
	CString ShortName;	
#ifdef TB_SYSTEM_ONLY_BOT
	ShortName = _T("MotionNode_Bot.INI");	
#else
	ShortName = _T("MotionNode.INI");		
#endif//TB_SYSTEM_ONLY_BOT
	Filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOIDirectory(), ShortName);	
	return Filename;
}
//-------------------------------------------------------------------------------------//
std::vector<CMotionNode>&  CMotion_Basic::GetMotionNodeList()
{	
	return m_MotionNodeList;	
}
//-------------------------------------------------------------------------------------//
std::vector<CMotionAxis>& CMotion_Basic::GetMotionAxisList()
{	
	return m_MotionAxisList;	
}
//-------------------------------------------------------------------------------------//
void CMotion_Basic::ResetMotionAxisPtr()
{
	CMotionAxis *Ptr=NULL;	
	m_MotionAxisPtrX = Ptr;
	m_MotionAxisPtrY = Ptr;
	m_MotionAxisPtrZ = Ptr;
	return;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Basic::CheckMotionNodePtr(const CMotionNode *Ptr)
{
	if ( NULL == Ptr )
	{
		m_ErrorString = _T("Error, Check Motion Node Ptr Fault");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Basic::CheckMotionAxisPtr(CMotionAxis *Ptr)
{
	if ( CheckMotionAxisPtr((const CMotionAxis*)(Ptr)) == false )
	{
		m_ErrorString = _T("Error, Check Motion Axis Ptr Fault");
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Basic::CheckMotionAxisPtr(const CMotionAxis *Ptr) const
{
	if ( NULL == Ptr )
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
CMotionAxis* CMotion_Basic::GetMotionAxisPtr(int Axis)
{
	CMotionAxis *Ptr=NULL;	
	switch ( Axis )
	{
	case AXIS_X: Ptr=m_MotionAxisPtrX;	break; 
	case AXIS_Y: Ptr=m_MotionAxisPtrY;	break; 
	case AXIS_Z: Ptr=m_MotionAxisPtrZ;	break; 
	}
	return Ptr;
}
//-------------------------------------------------------------------------------------//
const CMotionAxis* CMotion_Basic::GetMotionAxisPtr(int Axis) const
{
	CMotionAxis *Ptr=NULL;
	switch ( Axis )
	{
	case AXIS_X: Ptr=m_MotionAxisPtrX;	break; 
	case AXIS_Y: Ptr=m_MotionAxisPtrY;	break; 
	case AXIS_Z: Ptr=m_MotionAxisPtrZ;	break; 
	}
	return Ptr;
}
//-------------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionAxisPtr(int Axis, CMotionAxis *Ptr)
{	
	switch ( Axis )
	{
	case AXIS_X: m_MotionAxisPtrX=Ptr;	break; 
	case AXIS_Y: m_MotionAxisPtrY=Ptr;	break; 
	case AXIS_Z: m_MotionAxisPtrZ=Ptr;	break; 
	}
}
//-------------------------------------------------------------------------------------//
int CMotion_Basic::GetGantryAxis()//取得龍門軸
{
	const std::vector<CMotionAxis> &AxisList=GetMotionAxisList();	
	const size_t AxisCount=AxisList.size();
	for ( size_t i=0; i<AxisCount; i++ )
	{
		const CMotionAxis &MotionAxisRef=AxisList[i];
		if ( MotionAxisRef.CheckGantryAxis() == false ) { continue; }
		return MotionAxisRef.GetAxisID();
	}
	return -1;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Basic::PreInitMotion()//預先初始化
{
	SetMotionCardType(MOTION_CARD_NONE);
	m_IsMotionRelease = false;
	m_MotionCallbackHWnd = NULL;	
	m_MotionCtrlThreadDeleted = true;
	SetInitialize(false);	
	m_IsJogMode = false;
	m_MachineType = MACHINE_MODEL_8000;
	m_MaxAxisCount = 0;//最多軸數
	m_UsedAxisCount = 3;//使用軸數
	m_MotionAxisPtrX = NULL;
	m_MotionAxisPtrY = NULL;
	m_MotionAxisPtrZ = NULL;
	m_ErrorString = _T(""); //錯誤字串
	m_MotionStatus = _T("");//運動系統的狀況
	m_MotionLogIndex = -1;	

	//觸發時的參數，除錯用
	m_TriggerStatus = MOTION_TRIGGER_NULL;	   //觸發行程的狀態
	m_TriggerMaxRepeatCounts = 0;	
	m_TriggerStartPos = 0; //移動的起點
	m_TriggerEndPos = 0;   //移動的終點
	m_TriggerPosY = 0;     //移動時的Y軸
	m_TriggerFirstOnePos = 0;    //第一個觸發點
	m_TriggerLastOnePos = 0;      //最後的觸發點
	m_TriggerInterval = 0; //觸發的間距
	m_TriggerNTriggers = 0; //多少個處發點
	m_IsMotionStop= false;     //設定是否要機台停止
	m_WaitForDoneDelayTime = 0;//等待移動完成的延遲時間
	m_ORGX = 0;
	m_ORGY = 0;//進行移動後的回歸位置
	m_ORGZ = 0;

	m_CommandOffline = false;

	m_FreeRunVel_X= 0;
	m_FreeRunVel_Y= 0;
	m_FreeRunVel_Z= 0;
	
	m_XYCaliLaneID = LANE_ID_NULL;
	m_StopXYCalibration = false;
	return true;
}
//-------------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionName(LPCTSTR val)//設定運動系統名稱
{
	m_MotionName = val;
}
//----------------------------------------------------------------------------------//
LPCTSTR CMotion_Basic::GetMotionName() const//取得運動系統名稱
{
	return this->m_MotionName;
}
//-------------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionExceptionCode_Param(LPCTSTR Err)
{	
	CString str = (NULL!=Err) ? Err:m_ErrorString;		
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Param(str);
	//SetMotionExceptionCode(AOI_EXCEPTION_MOTION_PARAM, Err);	return;
	return;
}
//-------------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionExceptionCode_FileRead(LPCTSTR Err)
{
	CString str = (NULL!=Err) ? Err:m_ErrorString;	
	AOIExceptionCodeCtrl.SetAOIExceptionCode_FileRead(str);	
	//SetMotionExceptionCode(AOI_EXCEPTION_MOTION_FILE_READ, Err);	return;	
	return;
}
//-------------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionExceptionCode_FileWrite(LPCTSTR Err)
{
	CString str = (NULL!=Err) ? Err:m_ErrorString;		
	AOIExceptionCodeCtrl.SetAOIExceptionCode_FileWrite(str);
	//SetMotionExceptionCode(AOI_EXCEPTION_MOTION_FILE_WRITE, Err);	return;	
	return;
}
//-------------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionExceptionCode(DWORD Code, LPCTSTR Err)
{
	CString str = (NULL!=Err) ? Err:m_ErrorString;	
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Motion(Code, str);
	return;
}
//-------------------------------------------------------------------------------------//
void CMotion_Basic::SetErrorString(LPCTSTR str)//設定錯誤字串
{
	m_ErrorString = str;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CMotion_Basic::GetErrorString()
{	
	m_ErrorStringOut=JetAPI::AddKeyToErrorString(m_ErrorString, _T("[Motion]"));
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Motion_Others(m_ErrorStringOut);	
	return m_ErrorStringOut;
}
//-----------------------------------------------------------------------------//
LPCTSTR CMotion_Basic::GetMotionStatus() const//取得運動系統狀況
{
	return this->m_MotionStatus;
}
//-----------------------------------------------------------------------------//
CString CMotion_Basic::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)//載入多國語系
{
	CString NewLabelText;
	LPCTSTR Section=_T("MOTION_BASIC_OBJECT");		
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-----------------------------------------------------------------------------//
bool CMotion_Basic::CheckAxisBypass(int Axis)//確認軸跳過
{
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	return CheckAxisBypass(MotionAxisPtr);
}
//-----------------------------------------------------------------------------//
bool CMotion_Basic::CheckAxisBypass(const CMotionAxis *MotionAxisPtr)//確認軸跳過
{
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return true; }
	if ( MotionAxisPtr->GetAxisBypass() == false ) { return false; }		
	return true;
}
//-----------------------------------------------------------------------------//
bool CMotion_Basic::CheckNodeBypass(const CMotionNode *MotionNodePtr)//確認軸跳過
{
	if ( CheckMotionNodePtr(MotionNodePtr) == false ) { return true; }
	if ( MotionNodePtr->GetAxisBypass() == false ) { return false; }	
	return true;
}
//-----------------------------------------------------------------------------//
void CMotion_Basic::SetIsJogMode(const bool Is)//設定是否可以為Jog模式
{
	this->m_IsJogMode = Is;
}
//-----------------------------------------------------------------------------//
bool CMotion_Basic::GetIsJogMode() const
{
	return this->m_IsJogMode;
}
//-----------------------------------------------------------------------------//
double CMotion_Basic::GetTriggerStartPos() const //取得觸發時的起點
{
	return this->m_TriggerStartPos;
}
//-----------------------------------------------------------------------------//
double CMotion_Basic::GetTriggerEndPos() const   //取得觸發時的終點
{
	return this->m_TriggerEndPos;
}
//-----------------------------------------------------------------------------//
double CMotion_Basic::GetTriggerFirstOnePos() const    //取得第一個觸發時的位置
{
	return this->m_TriggerFirstOnePos;
}
//-----------------------------------------------------------------------------//
double CMotion_Basic::GetTriggerLastOnePos() const      //取得最後一個觸發時的位置
{
	return this->m_TriggerLastOnePos;
}
//-----------------------------------------------------------------------------//
int CMotion_Basic::GetNTriggers() const       //取得有多少個觸發點
{
	return this->m_TriggerNTriggers;
}
//-----------------------------------------------------------------------------//
double CMotion_Basic::GetTriggerInterval() const //取得觸發的間隔
{
	return this->m_TriggerInterval;
}
//-----------------------------------------------------------------------------//
void CMotion_Basic::SetMotionIsStop(const bool IsStop)//設定是否運動停止
{
	this->m_IsMotionStop = IsStop;
}
//-----------------------------------------------------------------------------//
bool CMotion_Basic::GetMotionIsStop() const//取得是否運動停止
{
	return this->m_IsMotionStop;
}
//-----------------------------------------------------------------------------//
bool CMotion_Basic::GetIsLimit(const int AxisNo)
{	
	if ( this->GetIsNLimit(AxisNo) == true ) 
	{ 
		switch ( AxisNo )
		{
		case AXIS_X:	this->m_ErrorString.Format(_T("Error, X Axis Stage at Limit Position (N)"));	break;
		case AXIS_Y:	this->m_ErrorString.Format(_T("Error, Y Axis Stage at Limit Position (N)"));	break;
		case AXIS_Z:	this->m_ErrorString.Format(_T("Error, Z Axis Stage at Limit Position (N)"));	break;
		default:
			this->m_ErrorString.Format(_T("Error, Stage at Limit Position (N)"));
			break;
		}		
		return true; 
	}
	if ( this->GetIsPLimit(AxisNo) == true ) 
	{ 
		switch ( AxisNo )
		{
		case AXIS_X:	this->m_ErrorString.Format(_T("Error, X Axis Stage at Limit Position (P)"));	break;
		case AXIS_Y:	this->m_ErrorString.Format(_T("Error, Y Axis Stage at Limit Position (P)"));	break;
		case AXIS_Z:	this->m_ErrorString.Format(_T("Error, Z Axis Stage at Limit Position (P)"));	break;
		default:
			this->m_ErrorString.Format(_T("Error, Stage at Limit Position (P)"));
			break;
		}
		
		return true; 
	}
	return false;
}
//-----------------------------------------------------------------------------//
bool CMotion_Basic::GetInint() const
{
	return m_IsInitialize;
}
//-----------------------------------------------------------------------------//
bool CMotion_Basic::CheckInit()
{
	if ( GetInint() == false )
	{
		this->m_ErrorString.Format(_T("Error, Motion Not Initialized, or Initialized Fault."));
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_CONNECT);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetInitialize(bool bInit)
{
	m_IsInitialize = bInit;
}
//----------------------------------------------------------------------------------//
int CMotion_Basic::GetMaxAxisCount() const
{
	return m_MaxAxisCount;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetMaxAxisCount(int val)
{
	m_MaxAxisCount = val;
}
//----------------------------------------------------------------------------------//
int CMotion_Basic::GetUsedAxisCount() const
{
	return m_UsedAxisCount;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetUsedAxisCount(int val)
{
	m_UsedAxisCount = val;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::BuildMotionNodeList()//建立運動點列表
{
	return ReturnNoSupportFunc(_T("CMotion_Basic::BuildMotionNodeList"));
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::BuildMotionAxisList()//建立運動軸列表
{
	return ReturnNoSupportFunc(_T("CMotion_Basic::BuildMotionAxisList"));
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CheckMotionAxisValid(CMotionAxis &Ref)//確認運動軸	
{
	if ( CheckMotionNodeValid(Ref.GetMotionNodeRef()) == false ) 
	{
		Ref.SetAxisBypass(true);
		return false; 
	}
	
	size_t i=0;	
	bool SlaveNodeErr=false;
	const size_t SlaveNodeCount=Ref.GetSlaveNodeCount();
	for ( i=0; i<SlaveNodeCount; i++ )
	{
		CMotionNode *Ptr=Ref.GetSlaveNodePtr(i, false);
		if ( NULL == Ptr ) { continue; }
		if ( CheckMotionNodeValid(*Ptr) == false )
		{				
			SlaveNodeErr = true;
			Ptr->SetAxisBypass(true);
			continue;
		}
	}
	if ( true == SlaveNodeErr )
	{	Ref.ClearSlaveNodeList();	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CheckMotionNodeValid(const CMotionNode &Ref)//確認軸控節點	
{
	const int MaxAxisCount=GetMaxAxisCount();
	const int CardAxis = Ref.GetNodeID();
	if ( CardAxis<0 || CardAxis>=MaxAxisCount )
	{	return false;	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::UpdateMotionAxisDriverModel(CMotionAxis &Ref)//更新運動軸的驅動器型號
{
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::BuildMotionAxisListFn(const std::vector<CMotionNode> &NodeList, std::vector<CMotionAxis> &AxisList)//建立運動軸列表	
{
	int Axis=0;	
	size_t i=0, j=0;		
	std::vector<CMotionNode> AxisNodeList;
	const size_t MaxAxis=GetUsedAxisCount();	
	const size_t MotionNodeCount=NodeList.size();

	AxisList.clear();
	for ( i=0; i<MaxAxis; i++ )
	{
		Axis = i;
		AxisNodeList.clear();
		for ( j=0; j<MotionNodeCount; j++ )
		{
			const CMotionNode &MotionNodeRef=NodeList[j];
			if ( MotionNodeRef.GetAxisID() != Axis ) { continue; }			
			AxisNodeList.push_back(MotionNodeRef);
		}
		const size_t AxisNodeCount=AxisNodeList.size();
		if ( 0 == AxisNodeCount ) 
		{	continue;	}
		if ( 1 == AxisNodeCount ) 
		{	
			AxisList.push_back(AxisNodeList[0]);
			continue; 
		}

		//Search Master Node
		for ( j=0; j<AxisNodeCount; j++ )
		{
			const CMotionNode &MotionNodeRef=AxisNodeList[j];
			if ( MotionNodeRef.GetAxisType() == MOTION_AXIS_MASTER )
			{	break; }
		}

		size_t MasterIndex=j;
		if ( MasterIndex == AxisNodeCount )
		{	MasterIndex = 0;	}
		CMotionAxis MotionAxis=AxisNodeList[MasterIndex];
		for ( j=0; j<AxisNodeCount; j++ )
		{
			if ( j == MasterIndex ) { continue; }			
			MotionAxis.AddSlaveNode(AxisNodeList[j]);
		}
		AxisList.push_back(MotionAxis);
	}

	int GantryID = 0;
	const size_t MotionAxisCount=AxisList.size();
	for ( i=0; i<MotionAxisCount; i++ )
	{		
		CMotionAxis &MotionAxisRef=AxisList[i];
		Axis = MotionAxisRef.GetAxisID();
		switch ( Axis )
		{
		case AXIS_X: MotionAxisRef.SetAxisName("X Axis"); break;
		case AXIS_Y: MotionAxisRef.SetAxisName("Y Axis"); break;
		case AXIS_Z: MotionAxisRef.SetAxisName("Z Axis"); break;
		}
		if ( MotionAxisRef.CheckGantryAxis() == true )
		{	
			MotionAxisRef.SetGantryID(GantryID);
			const size_t SlaveCount=MotionAxisRef.GetSlaveNodeCount();
			for ( j=0; j<SlaveCount; j++ )
			{
				CMotionNode *SlaveNodePtr=MotionAxisRef.GetSlaveNodePtr(j, false);
				if ( NULL == SlaveNodePtr ) { continue; }
				SlaveNodePtr->SetGantryID(GantryID);
				switch ( Axis )
				{
				case AXIS_X: SlaveNodePtr->SetAxisName("X-Slave Axis"); break;
				case AXIS_Y: SlaveNodePtr->SetAxisName("Y-Slave Axis"); break;
				case AXIS_Z: SlaveNodePtr->SetAxisName("Z-Slave Axis"); break;
				}
			}
			GantryID ++;
		}		
		CheckMotionAxisValid(MotionAxisRef);
		UpdateMotionAxisDriverModel(MotionAxisRef);
		SetMotionAxisPtr(Axis, &MotionAxisRef);
	}	
	UpdateMotionParameterToMotionAxis();	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::GetIsSupportGantry()
{
	return m_IsSupportGantry;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetIsSupportGantry(bool bVal)
{
	m_IsSupportGantry = bVal;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CheckNeedMoveXY(double PosX, double PosY, bool OfflineMode)
{
	if ( m_CommandOffline != OfflineMode ) { return true; }
	const double dX = ::fabs(PosX-GetCommandPosX());
	const double dY = ::fabs(PosY-GetCommandPosY());
	if ( dX>1.0 || dY>1.0 )
	{	return true; }
	return false;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CheckNeedMoveXYZ(double PosX, double PosY, double PosZ, bool OfflineMode)
{
	if ( m_CommandOffline != OfflineMode ) { return true; }
	const double dX = ::fabs(PosX-GetCommandPosX());
	const double dY = ::fabs(PosY-GetCommandPosY());
	const double dZ = ::fabs(PosZ-GetCommandPosZ());
	if ( dX>1.0 || dY>1.0 || dZ>1.0 )
	{	return true; }
	return false;
}
//----------------------------------------------------------------------------------//
int  CMotion_Basic::GetTriggerProcess() const//取得目前是向前走還是向後走
{
	return m_TriggerStatus;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::GetIsTriggerRepeatFinish() const
{
	if ( this->m_TriggerRepeatCounts >= this->m_TriggerMaxRepeatCounts )
	{	return true; }
	else { return false; }
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionCallbackHWnd(const HWND hWnd)
{
	this->m_MotionCallbackHWnd = hWnd;	
}
//----------------------------------------------------------------------------------//
HWND CMotion_Basic::GetMotionCallbackHWnd() const
{
	return m_MotionCallbackHWnd;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::PostMotionCallbackWndMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = GetMotionCallbackHWnd();
	if ( NULL == hWnd ) { return false; }
	//if ( MSG_SYSTEM_EXCEPTION_CALLBACK == message )
	//{	message = MSG_SYSTEM_EXCEPTION_CALLBACK; }
	::PostMessage(hWnd, message, wParam, lParam);
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::SendMotionCallbackWndMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = GetMotionCallbackHWnd();
	if ( NULL == hWnd ) { return false; }
	::SendMessage(hWnd, message, wParam, lParam);
	return true;
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetTriggerPosY()//取得觸發的起點的Y值
{
	return this->m_TriggerPosY;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetTriggerFirstPosY(const double SPY)
{
	m_TriggerPosY = SPY;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CorrectGantryOffset(int Axis)//修正龍門偏差
{
	return ReturnNoSupportFunc(_T("CorrectGantryOffset"));
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CalibrateGantryOffset(int Axis)//校正龍門偏差
{
	return ReturnNoSupportFunc(_T("CalibrateGantryOffset"));
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::SetGantryStdOffset(int Axis, double Offset)//設定龍門的標準偏移值
{
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }	
	if ( MotionAxisPtr->CheckGantryAxis() == false ) { return true; }
	CMotionNode *MotionNodePtr=MotionAxisPtr->GetSlaveNodePtr(0, true);
	if ( NULL == MotionNodePtr ) { return false; }
	MotionNodePtr->SetGantryStdOffset(Offset); 
	MotionNodePtr->SaveIniFile();	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::GetGantryStdOffset(int Axis, double &Offset)//設定龍門的標準偏移值
{
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }	
	if ( MotionAxisPtr->CheckGantryAxis() == false ) { return true; }
	CMotionNode *MotionNodePtr=MotionAxisPtr->GetSlaveNodePtr(0, true);
	if ( NULL == MotionNodePtr ) { return false; }
	Offset = MotionNodePtr->GetGantryStdOffset();
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::FetchGantryOffset(int Axis, double &Offset)//取得龍門的偏移值	
{
	return ReturnNoSupportFunc(_T("FetchGantryOffset"));
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::RegisterMotionCurrentProcessFile()//註冊運動的訊息檔案
{	
#ifndef MOTION_OBJ_DISABLE	
	const bool bSaveHeader = true;
	CString LogFolder, LogFilename, LogExtName, LogBackup;	
	LogFolder = AOIDataCollect.GetAOILogDirectory();
	LogFilename = _T("MotionLog");
	LogExtName = _T("TXT");
	LogBackup = _T("");
	m_MotionLogIndex = LogManager.AddLogFile(LogFolder, LogFilename, LogExtName, LogBackup, CLogNode::LOG_FILENAME_BY_DATE, bSaveHeader);
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::SaveMotionCurrentProcess(const wchar_t *String)
{
	const TMotionParameter &MotionParam=GetMotionParameter();
	if ( MotionParam.m_SaveCurrentMotionProcess == FN_DISABLE ) { return true; }
	if ( LogManager.AddLogMessage(m_MotionLogIndex, String) == true )
	{	return true; }
	AOIDataCollect.SaveCurrentProcess(String);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Basic::SaveMotionCurrentProcess(const char *String)
{
	const TMotionParameter &MotionParam=GetMotionParameter();
	if ( MotionParam.m_SaveCurrentMotionProcess == FN_DISABLE ) { return true; }
	if ( LogManager.AddLogMessage(m_MotionLogIndex, String) == true )
	{	return true; }
	AOIDataCollect.SaveCurrentProcess(String);
	return true;
}
//-------------------------------------------------------------------------------------//
void CMotion_Basic::LockMotion(int Axis)
{
	switch ( Axis )
	{
	case AXIS_X:	::EnterCriticalSection(&m_csMotion_X);	break;
	case AXIS_Y:	::EnterCriticalSection(&m_csMotion_Y);	break;
	case AXIS_Z:	::EnterCriticalSection(&m_csMotion_Z);	break;
	default:		::EnterCriticalSection(&m_csMotion);	break;
	}
}
//-------------------------------------------------------------------------------------//
void CMotion_Basic::UnlockMotion(int Axis)
{
	switch ( Axis )
	{
	case AXIS_X:	::LeaveCriticalSection(&m_csMotion_X);	break;
	case AXIS_Y:	::LeaveCriticalSection(&m_csMotion_Y);	break;
	case AXIS_Z:	::LeaveCriticalSection(&m_csMotion_Z);	break;
	default:		::LeaveCriticalSection(&m_csMotion);	break;
	}
}
//-------------------------------------------------------------------------------------//
bool CMotion_Basic::CheckIsHomed(int Axis)//確認是否歸零過
{
	const CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	return CheckIsHomed(MotionAxisPtr);	
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CheckIsHomed(const CMotionAxis *MotionAxisPtr)//確認是否歸零過
{
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	const int Axis = MotionAxisPtr->GetAxisID();
	const bool IsHomed = MotionAxisPtr->GetIsHomed();
	if ( false == IsHomed )
	{
		CString str;
		switch ( Axis )
		{
		case AXIS_X:	str=_T("Error, X axis is not homed");	break;
		case AXIS_Y:	str=_T("Error, Y axis is not homed");	break;
		case AXIS_Z:	str=_T("Error, Z axis is not homed");	break;
		default:		str=_T("Error, U axis is not homed");	break;
		}
		m_ErrorString = CMotion_Basic::LoadMultiLanguageString(str, str);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CheckIsEnabled(int Axis)//確認是否啟用過
{
	const CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	return CheckIsEnabled(MotionAxisPtr);	
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CheckIsEnabled(const CMotionAxis *MotionAxisPtr)//確認是否啟用過	
{
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	const int Axis = MotionAxisPtr->GetAxisID();
	const bool IsEnabled = MotionAxisPtr->GetIsEnabled();
	if ( false == IsEnabled )
	{
		CString str;
		switch ( Axis )
		{
		case AXIS_X:	str=_T("Error, X axis is not enabled");	break;
		case AXIS_Y:	str=_T("Error, Y axis is not enabled");	break;
		case AXIS_Z:	str=_T("Error, Z axis is not enabled");	break;
		default:		str=_T("Error, U axis is not enabled");	break;
		}
		m_ErrorString = CMotion_Basic::LoadMultiLanguageString(str, str);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::GetIsConvertSignPositive()//取得是否內部方向性轉換
{
	if ( FN_ENABLE == GetMotionParameter().m_SignPositiveConvert ) { return true; }
	return false;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CheckFreeRunMoving(CMotionAxis *MotionAxisPtr)//確認FreeRun移動中
{
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	int Vel=MotionAxisPtr->GetFreeRunVel();
	Vel=abs(Vel);
	if ( 0 == Vel  ) { return false; }	
	return true;
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::ConvertAxisPos(int Axis, double Pos) const//轉換軸位置	
{
	const CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	return ConvertAxisPos(MotionAxisPtr, Pos);	
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::ConvertAxisPos(const CMotionAxis *MotionAxisPtr, double Pos) const//轉換軸位置	
{
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return Pos; }
	if ( MotionAxisPtr->CheckConvertSignPositive() == FN_DISABLE ) { return Pos; }	
	return -1*Pos;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::GetCommandOffline()
{
	return m_CommandOffline;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetCommandOffline(bool val)
{
	m_CommandOffline = val;
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetCommandPosX()
{
	return GetCommandPos(AXIS_X);
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetCommandPosX(double val)
{
	SetCommandPos(AXIS_X, val);
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetCommandPosY()
{
	return GetCommandPos(AXIS_Y);
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetCommandPosY(double val)
{
	SetCommandPos(AXIS_Y, val);
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetCommandPosZ()
{
	return GetCommandPos(AXIS_Z);
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetCommandPosZ(double val)
{
	SetCommandPos(AXIS_Z, val);
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetCommandPos(int Axis)
{
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return 0.0; }
	return MotionAxisPtr->GetCommandPos();
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetCommandPos(int Axis, double val)
{	
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return ; }
	MotionAxisPtr->SetCommandPos(val);
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetCommandPos(double X, double Y, double Z)//設定機台命令位置
{
	SetCommandPosX(X);
	SetCommandPosY(Y);
	SetCommandPosZ(Z);	
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::GetCommandPos(double &X, double &Y, double &Z)//取得機台命令位置
{
	X = GetCommandPosX();
	Y = GetCommandPosY();
	Z = GetCommandPosZ();	
	return true;
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetPosTolerance(int Axis)
{
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return 0.001; }
	return MotionAxisPtr->GetPosTolerance();
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetPosToleranceX()
{
	return GetPosTolerance(AXIS_X);	
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetPosToleranceY()
{
	return GetPosTolerance(AXIS_Y);
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetPosToleranceZ()
{
	return GetPosTolerance(AXIS_Z);
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetCommandMaxVelocity_X()
{
	return GetCommandMaxVelocity(AXIS_X);
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetCommandMaxVelocity_X(double val)
{
	SetCommandMaxVelocity(AXIS_X, val);
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetCommandMaxVelocity_Y()
{
	return GetCommandMaxVelocity(AXIS_Y);
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetCommandMaxVelocity_Y(double val)
{
	SetCommandMaxVelocity(AXIS_Y, val);
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetCommandMaxVelocity_Z()
{
	return GetCommandMaxVelocity(AXIS_Z);
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetCommandMaxVelocity_Z(double val)
{
	SetCommandMaxVelocity(AXIS_Z, val);
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetCommandMaxVelocity(int Axis)
{
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return 0.0; }
	return MotionAxisPtr->GetCommandMaxVelocity();
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetCommandMaxVelocity(int Axis, double val)
{
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return ; }
	MotionAxisPtr->SetCommandMaxVelocity(val);
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetMotionAxisVelocity(CMotionAxis *MotionAxisPtr, MOTION_MOVING_MODE Mode)
{
	double MaxVel = 0.0;
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return MaxVel; }
	switch ( Mode )
	{
	case MOTION_MOVING_HOME:	MaxVel = MotionAxisPtr->GetHomeVelocity();	break;
	//case MOTION_MOVING_SCAN:	MaxVel = MotionParam.m_TriggerMaxVelocity;	break;
	case MOTION_MOVING_GO_STOP:	MaxVel = MotionAxisPtr->GetGrabbingVelocity();	break;
	case MOTION_MOVING_PCB_IN:	MaxVel = MotionAxisPtr->GetBoardInVelocity();		break;
	case MOTION_MOVING_PCB_OUT:	MaxVel = MotionAxisPtr->GetBoardOutVelocity();		break;
	case MOTION_MOVING_GRAB_MAP:MaxVel = MotionAxisPtr->GetGrabMapVelocity();		break;
	default:					MaxVel = MotionAxisPtr->GetMaxVelocity();	break;
	}	
	return MaxVel;
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetCommandAccelerationTim_X()
{
	return GetCommandAccelerationTime(AXIS_X);
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetCommandAccelerationTim_X(double val)
{
	SetCommandAccelerationTime(AXIS_X, val);
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetCommandAccelerationTim_Y()
{
	return GetCommandAccelerationTime(AXIS_Y);
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetCommandAccelerationTim_Y(double val)
{	
	SetCommandAccelerationTime(AXIS_Y, val);
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetCommandAccelerationTim_Z()
{
	return GetCommandAccelerationTime(AXIS_Z);
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetCommandAccelerationTim_Z(double val)
{
	SetCommandAccelerationTime(AXIS_Z, val);
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetCommandAccelerationTime(int Axis)
{
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return 0.0; }
	return MotionAxisPtr->GetCommandAccelerationTime();
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetCommandAccelerationTime(int Axis, double val)
{
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(Axis);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return ; }
	MotionAxisPtr->SetCommandAccelerationTime(val);
}
//----------------------------------------------------------------------------------//
CString CMotion_Basic::GetMotionParamSectionName() const//取得運動參數的區間名稱
{
	CString Section;
#ifdef TB_SYSTEM_ONLY_BOT
	Section = _T("Motion Parameter Bot");
#else
	Section = _T("Motion Parameter");
#endif//TB_SYSTEM_ONLY_BOT
	return Section;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::ExecLoadMotionParameter(LPCTSTR filename, TMotionParameter &MotionParam)//載入運動參數
{
	const size_t textlen = 128;
	CString Filename;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString Default = _T("");
	TCHAR   String[textlen]=_T("");		
	
	Filename = filename;
	Section = GetMotionParamSectionName();	
	//MotionParam
	//載入 Axis Start Velocity
	KeyName.Format(_T("Axis Start Velocity"));
	Default.Format(_T("%f"), MotionParam.m_StartVelocity);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_StartVelocity = ::_tcstod(String, NULL); }
	
	//載入 X Axis Board In Velocity
	KeyName.Format(_T("X Axis Board In Velocity"));
	Default.Format(_T("%f"), MotionParam.m_BoardInVelocityX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_BoardInVelocityX = ::_tcstod(String, NULL); }	

	//載入 Y Axis Board In Velocity
	KeyName.Format(_T("Y Axis Board In Velocity"));
	Default.Format(_T("%f"), MotionParam.m_BoardInVelocityY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_BoardInVelocityY = ::_tcstod(String, NULL); }

	//載入 Z Axis Board In Velocity
	KeyName.Format(_T("Z Axis Board In Velocity"));
	Default.Format(_T("%f"), MotionParam.m_BoardInVelocityZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_BoardInVelocityZ = ::_tcstod(String, NULL); }
	
	//載入 X Axis Board Out Velocity
	KeyName.Format(_T("X Axis Board Out Velocity"));
	Default.Format(_T("%f"), MotionParam.m_BoardOutVelocityX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_BoardOutVelocityX = ::_tcstod(String, NULL); }	

	//載入 Y Axis Board Out Velocity
	KeyName.Format(_T("Y Axis Board Out Velocity"));
	Default.Format(_T("%f"), MotionParam.m_BoardOutVelocityY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_BoardOutVelocityY = ::_tcstod(String, NULL); }

	//載入 Z Axis Board Out Velocity
	KeyName.Format(_T("Z Axis Board Out Velocity"));
	Default.Format(_T("%f"), MotionParam.m_BoardOutVelocityZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_BoardOutVelocityZ = ::_tcstod(String, NULL); }

	//載入 X Axis Max Velocity
	KeyName.Format(_T("X Axis Max Velocity"));
	Default.Format(_T("%f"), MotionParam.m_MaxVelocityX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_MaxVelocityX = ::_tcstod(String, NULL); }		

	//載入 Y Axis Max Velocity
	KeyName.Format(_T("Y Axis Max Velocity"));
	Default.Format(_T("%f"), MotionParam.m_MaxVelocityY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_MaxVelocityY = ::_tcstod(String, NULL); }	

	//載入 Z Axis Max Velocity
	KeyName.Format(_T("Z Axis Max Velocity"));
	Default.Format(_T("%f"), MotionParam.m_MaxVelocityZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_MaxVelocityZ = ::_tcstod(String, NULL); }	

	//載入 X Axis Grabbing Velocity
	KeyName.Format(_T("X Axis Grabbing Velocity"));
	Default.Format(_T("%.0f"), MotionParam.m_GrabbingVelocityX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_GrabbingVelocityX = ::_tcstod(String, NULL); }	

	//載入 Y Axis Grabbing Velocity
	KeyName.Format(_T("Y Axis Grabbing Velocity"));
	Default.Format(_T("%.0f"), MotionParam.m_GrabbingVelocityY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_GrabbingVelocityY = ::_tcstod(String, NULL); }	

	//載入 Z Axis Grabbing Velocity
	KeyName.Format(_T("Z Axis Grabbing Velocity"));
	Default.Format(_T("%.0f"), MotionParam.m_GrabbingVelocityZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_GrabbingVelocityZ = ::_tcstod(String, NULL); }	
	
	MotionParam.m_GrabMapVelocityX=MotionParam.m_GrabbingVelocityX;
	MotionParam.m_GrabMapVelocityY=MotionParam.m_GrabbingVelocityY;
	MotionParam.m_GrabMapVelocityZ=MotionParam.m_GrabbingVelocityZ;

	//X軸的取底圖移動速度, 
	KeyName.Format(_T("X Axis Grab Map Velocity"));
	Default.Format(_T("%.0f"), MotionParam.m_GrabMapVelocityX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_GrabMapVelocityX = ::_tcstod(String, NULL); }	

	//Y軸的取底圖移動速度, 
	KeyName.Format(_T("Y Axis Grab Map Velocity"));
	Default.Format(_T("%.0f"), MotionParam.m_GrabMapVelocityY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_GrabMapVelocityY = ::_tcstod(String, NULL); }	

	//Z軸的取底圖移動速度, 
	KeyName.Format(_T("Z Axis Grab Map Velocity"));
	Default.Format(_T("%.0f"), MotionParam.m_GrabMapVelocityZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_GrabMapVelocityZ = ::_tcstod(String, NULL); }	

	//載入In Position Delay Time
	KeyName.Format(_T("In Position Delay Time"));
	Default.Format(_T("%d"), MotionParam.m_InPositionDelayTime);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_InPositionDelayTime = (int)(::_tcstod(String, NULL)); }
	m_WaitForDoneDelayTime = MotionParam.m_InPositionDelayTime;

	//載入等待移動停止的函式暫停時間 ms
	KeyName.Format(_T("Wait For Done Dwell Time"));
	Default.Format(_T("%d"), MotionParam.m_WaitForDoneDwellTime);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_WaitForDoneDwellTime = ::_ttoi(String); }

	//載入 Trigger 等速的提前距離
	KeyName.Format(_T("Trigger Pre-Offset Distance"));
	Default.Format(_T("%f"), MotionParam.m_PreTriggerOffsetDis);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_PreTriggerOffsetDis = ::_tcstod(String, NULL); }

	//載入 Trigger 停止的提前距離
	KeyName.Format(_T("Trigger Post-Offset Distance"));	
	Default.Format(_T("%f"), MotionParam.m_PostTriggerOffsetDis);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_PostTriggerOffsetDis = ::_tcstod(String, NULL); }


	//載入 Trigger Start Velocity
	KeyName.Format(_T("Trigger Start Velocity"));
	Default.Format(_T("%f"), MotionParam.m_TriggerStartVelocity);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_TriggerStartVelocity = ::_tcstod(String, NULL); }

	//載入 Trigger Max Velocity
	KeyName.Format(_T("Trigger Max Velocity"));
	Default.Format(_T("%f"), MotionParam.m_TriggerMaxVelocity);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_TriggerMaxVelocity = ::_tcstod(String, NULL); }
	
	//載入 X Axis Acceleration Time
	KeyName.Format(_T("X Axis Acceleration Time"));
	Default.Format(_T("%f"), MotionParam.m_AccelerationTimeX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_AccelerationTimeX = ::_tcstod(String, NULL); }

	//載入 X Axis Deceleration Time
	KeyName.Format(_T("X Axis Deceleration Time"));
	Default.Format(_T("%f"), MotionParam.m_DecelerationTimeX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_DecelerationTimeX = ::_tcstod(String, NULL); }

	//載入 Y Axis Acceleration Time
	KeyName.Format(_T("Y Axis Acceleration Time"));
	Default.Format(_T("%f"), MotionParam.m_AccelerationTimeY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_AccelerationTimeY = ::_tcstod(String, NULL); }

	//載入 Y Axis Deceleration Time
	KeyName.Format(_T("Y Axis Deceleration Time"));
	Default.Format(_T("%f"), MotionParam.m_DecelerationTimeY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_DecelerationTimeY = ::_tcstod(String, NULL); }	

	//載入 Z Axis Acceleration Time
	KeyName.Format(_T("Z Axis Acceleration Time"));
	Default.Format(_T("%f"), MotionParam.m_AccelerationTimeZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_AccelerationTimeZ = ::_tcstod(String, NULL); }

	//載入 Z Axis Deceleration Time
	KeyName.Format(_T("Z Axis Deceleration Time"));
	Default.Format(_T("%f"), MotionParam.m_DecelerationTimeZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_DecelerationTimeZ = ::_tcstod(String, NULL); }	

	//載入 Stage Start Pos X
	KeyName.Format(_T("Stage Start Pos X"));
	Default.Format(_T("%f"), MotionParam.m_StageStartPosX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_StageStartPosX = ::_tcstod(String, NULL); }

	//載入 Stage Start Pos Y
	KeyName.Format(_T("Stage Start Pos Y"));
	Default.Format(_T("%f"), MotionParam.m_StageStartPosY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_StageStartPosY = ::_tcstod(String, NULL); }

	//載入 Stage Start Pos Z
	KeyName.Format(_T("Stage Start Pos Z"));
	Default.Format(_T("%f"), MotionParam.m_StageStartPosZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_StageStartPosZ = ::_tcstod(String, NULL); }	
	
	//載入 Stage Leave Pos X
	KeyName.Format(_T("Stage Leave Pos X"));
	Default.Format(_T("%f"), MotionParam.m_StageLeavePosX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_StageLeavePosX = ::_tcstod(String, NULL); }
	
	//載入 Stage Leave Pos Y
	KeyName.Format(_T("Stage Leave Pos Y"));
	Default.Format(_T("%f"), MotionParam.m_StageLeavePosY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_StageLeavePosY = ::_tcstod(String, NULL); }

	//載入 Stage Leave Pos Z
	KeyName.Format(_T("Stage Leave Pos Z"));
	Default.Format(_T("%f"), MotionParam.m_StageLeavePosZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_StageLeavePosZ = ::_tcstod(String, NULL); }

	//載入 Before PCB-In Position X
	KeyName.Format(_T("Before PCB-In Pos X"));
	Default.Format(_T("%f"), MotionParam.m_BeforePCBInPosX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_BeforePCBInPosX = ::_tcstod(String, NULL); }
	
	//載入 Before PCB-In Position Y - 右邊停板
	KeyName.Format(_T("Before PCB-In Pos Y"));
	Default.Format(_T("%f"), MotionParam.m_BeforePCBInPosY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_BeforePCBInPosY = ::_tcstod(String, NULL); }

	//載入 Before PCB-In Position Z - 右邊停板
	KeyName.Format(_T("Before PCB-In Pos Z"));
	Default.Format(_T("%f"), MotionParam.m_BeforePCBInPosZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_BeforePCBInPosZ = ::_tcstod(String, NULL); }		

	//載入 PCB-Stop Position X Lane A - 右邊停板
	KeyName.Format(_T("PCB-Stop Pos X Lane A"));
	Default.Format(_T("%f"), MotionParam.m_PCBStopRPosX_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_PCBStopRPosX_LA = ::_tcstod(String, NULL); }

	//載入 PCB-Stop Position Y Lane A - 右邊停板
	KeyName.Format(_T("PCB-Stop Pos Y Lane A"));
	Default.Format(_T("%f"), MotionParam.m_PCBStopRPosY_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_PCBStopRPosY_LA = ::_tcstod(String, NULL); }

	//載入 PCB-Stop Position Z Lane A - 右邊停板
	KeyName.Format(_T("PCB-Stop Pos Z Lane A"));
	Default.Format(_T("%f"), MotionParam.m_PCBStopRPosZ_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_PCBStopRPosZ_LA = ::_tcstod(String, NULL); }	

	//載入 PCB-Stop Position X Lane B - 右邊停板
	KeyName.Format(_T("PCB-Stop Pos X Lane B"));
	Default.Format(_T("%f"), MotionParam.m_PCBStopRPosX_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_PCBStopRPosX_LB = ::_tcstod(String, NULL); }

	//載入 PCB-Stop Position Y Lane B - 右邊停板
	KeyName.Format(_T("PCB-Stop Pos Y Lane B"));
	Default.Format(_T("%f"), MotionParam.m_PCBStopRPosY_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_PCBStopRPosY_LB = ::_tcstod(String, NULL); }

	//載入 PCB-Stop Position Z Lane A - 右邊停板
	KeyName.Format(_T("PCB-Stop Pos Z Lane B"));
	Default.Format(_T("%f"), MotionParam.m_PCBStopRPosZ_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_PCBStopRPosZ_LB = ::_tcstod(String, NULL); }		
	
	//初始化 左邊停板
	MotionParam.m_PCBStopLPosX_LA = MotionParam.m_PCBStopRPosX_LA;
	MotionParam.m_PCBStopLPosY_LA = MotionParam.m_PCBStopRPosY_LA;
	MotionParam.m_PCBStopLPosZ_LA = MotionParam.m_PCBStopRPosZ_LA;
	MotionParam.m_PCBStopLPosX_LB = MotionParam.m_PCBStopRPosX_LB;
	MotionParam.m_PCBStopLPosY_LB = MotionParam.m_PCBStopRPosY_LB;
	MotionParam.m_PCBStopLPosZ_LB = MotionParam.m_PCBStopRPosZ_LB;	
	//載入 PCB-Stop2 Left Position  X Lane A - 左邊停板
	KeyName.Format(_T("PCB-Stop Left Pos X Lane A"));
	Default.Format(_T("%f"), MotionParam.m_PCBStopLPosX_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_PCBStopLPosX_LA = ::_tcstod(String, NULL); }

	//載入 PCB-Stop Left Position Y Lane A - 左邊停板
	KeyName.Format(_T("PCB-Stop Left Pos Y Lane A"));
	Default.Format(_T("%f"), MotionParam.m_PCBStopLPosY_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_PCBStopLPosY_LA = ::_tcstod(String, NULL); }

	//載入 PCB-Stop Left Position Z Lane A - 左邊停板
	KeyName.Format(_T("PCB-Stop Left Pos Z Lane A"));
	Default.Format(_T("%f"), MotionParam.m_PCBStopLPosZ_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_PCBStopLPosZ_LA = ::_tcstod(String, NULL); }	

	//載入 PCB-Stop Left Position X Lane B - 左邊停板
	KeyName.Format(_T("PCB-Stop Left Pos X Lane B"));
	Default.Format(_T("%f"), MotionParam.m_PCBStopLPosX_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_PCBStopLPosX_LB = ::_tcstod(String, NULL); }

	//載入 PCB-Stop Left Position Y Lane B - 左邊停板
	KeyName.Format(_T("PCB-Stop Left Pos Y Lane B"));
	Default.Format(_T("%f"), MotionParam.m_PCBStopLPosY_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_PCBStopLPosY_LB = ::_tcstod(String, NULL); }

	//載入 PCB-Stop Left Position Z Lane B - 左邊停板
	KeyName.Format(_T("PCB-Stop Left Pos Z Lane B"));
	Default.Format(_T("%f"), MotionParam.m_PCBStopLPosZ_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_PCBStopLPosZ_LB = ::_tcstod(String, NULL); }	
	
	//A軌道LED右停板的位置X	
	KeyName.Format(_T("Lane-LED Right Stop Pos X Lane A"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedStopRPosX_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedStopRPosX_LA = ::_tcstod(String, NULL); }	

	//A軌道LED右停板的位置Y
	KeyName.Format(_T("Lane-LED Right Stop Pos Y Lane A"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedStopRPosY_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedStopRPosY_LA = ::_tcstod(String, NULL); }	

	//A軌道LED右停板的位置Z
	KeyName.Format(_T("Lane-LED Right Stop Pos Z Lane A"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedStopRPosZ_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedStopRPosZ_LA = ::_tcstod(String, NULL); }	

	//A軌道LED右減速的位置X
	KeyName.Format(_T("Lane-LED Right Slow Pos X Lane A"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedSlowRPosX_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedSlowRPosX_LA = ::_tcstod(String, NULL); }	

	//A軌道LED右減速的位置Y
	KeyName.Format(_T("Lane-LED Right Slow Pos Y Lane A"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedSlowRPosY_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedSlowRPosY_LA = ::_tcstod(String, NULL); }	

	//A軌道LED右減速的位置Z
	KeyName.Format(_T("Lane-LED Right Slow Pos Z Lane A"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedSlowRPosZ_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedSlowRPosZ_LA = ::_tcstod(String, NULL); }	

	//B軌道LED右停板的位置X	
	KeyName.Format(_T("Lane-LED Right Stop Pos X Lane B"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedStopRPosX_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedStopRPosX_LB = ::_tcstod(String, NULL); }	

	//B軌道LED右停板的位置Y
	KeyName.Format(_T("Lane-LED Right Stop Pos Y Lane B"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedStopRPosY_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedStopRPosY_LB = ::_tcstod(String, NULL); }	

	//B軌道LED右停板的位置Z
	KeyName.Format(_T("Lane-LED Right Stop Pos Z Lane B"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedStopRPosZ_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedStopRPosZ_LB = ::_tcstod(String, NULL); }	

	//B軌道LED右減速的位置X
	KeyName.Format(_T("Lane-LED Right Slow Pos X Lane B"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedSlowRPosX_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedSlowRPosX_LB = ::_tcstod(String, NULL); }	

	//B軌道LED右減速的位置Y
	KeyName.Format(_T("Lane-LED Right Slow Pos Y Lane B"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedSlowRPosY_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedSlowRPosY_LB = ::_tcstod(String, NULL); }	

	//B軌道LED右減速的位置Z
	KeyName.Format(_T("Lane-LED Right Slow Pos Z Lane B"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedSlowRPosZ_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedSlowRPosZ_LB = ::_tcstod(String, NULL); }	
	
	//A軌道LED左停板的位置X	
	KeyName.Format(_T("Lane-LED Left Stop Pos X Lane A"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedStopLPosX_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedStopLPosX_LA = ::_tcstod(String, NULL); }	

	//A軌道LED左停板的位置Y
	KeyName.Format(_T("Lane-LED Left Stop Pos Y Lane A"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedStopLPosY_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedStopLPosY_LA = ::_tcstod(String, NULL); }	

	//A軌道LED左停板的位置Z
	KeyName.Format(_T("Lane-LED Left Stop Pos Z Lane A"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedStopLPosZ_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedStopLPosZ_LA = ::_tcstod(String, NULL); }	

	//A軌道LED左減速的位置X
	KeyName.Format(_T("Lane-LED Left Slow Pos X Lane A"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedSlowLPosX_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedSlowLPosX_LA = ::_tcstod(String, NULL); }	

	//A軌道LED左減速的位置Y
	KeyName.Format(_T("Lane-LED Left Slow Pos Y Lane A"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedSlowLPosY_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedSlowLPosY_LA = ::_tcstod(String, NULL); }	

	//A軌道LED左減速的位置Z
	KeyName.Format(_T("Lane-LED Left Slow Pos Z Lane A"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedSlowLPosZ_LA);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedSlowLPosZ_LA = ::_tcstod(String, NULL); }	

	//B軌道LED左停板的位置X	
	KeyName.Format(_T("Lane-LED Left Stop Pos X Lane B"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedStopLPosX_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedStopLPosX_LB = ::_tcstod(String, NULL); }	

	//B軌道LED左停板的位置Y
	KeyName.Format(_T("Lane-LED Left Stop Pos Y Lane B"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedStopLPosY_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedStopLPosY_LB = ::_tcstod(String, NULL); }	

	//B軌道LED左停板的位置Z
	KeyName.Format(_T("Lane-LED Left Stop Pos Z Lane B"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedStopLPosZ_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedStopLPosZ_LB = ::_tcstod(String, NULL); }	

	//B軌道LED左減速的位置X
	KeyName.Format(_T("Lane-LED Left Slow Pos X Lane B"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedSlowLPosX_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedSlowLPosX_LB = ::_tcstod(String, NULL); }	

	//B軌道LED左減速的位置Y
	KeyName.Format(_T("Lane-LED Left Slow Pos Y Lane B"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedSlowLPosY_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedSlowLPosY_LB = ::_tcstod(String, NULL); }	

	//B軌道LED左減速的位置Z
	KeyName.Format(_T("Lane-LED Left Slow Pos Z Lane B"));
	Default.Format(_T("%f"), MotionParam.m_LaneLedSlowLPosZ_LB);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LaneLedSlowLPosZ_LB = ::_tcstod(String, NULL); }		

	//載入 JOG Start Velocity
	KeyName.Format(_T("JOG Start Velocity"));
	Default.Format(_T("%f"), MotionParam.m_JogStartVelocity);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_JogStartVelocity = ::_tcstod(String, NULL); }

	//載入 JOG Moving Velocity
	KeyName.Format(_T("JOG Moving Velocity"));
	Default.Format(_T("%f"), MotionParam.m_JogMovingVelocity);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_JogMovingVelocity = ::_tcstod(String, NULL); }

	//載入 JOG Max Velocity
	KeyName.Format(_T("JOG Max Velocity"));
	Default.Format(_T("%f"), MotionParam.m_JogMaxVelocity);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_JogMaxVelocity = ::_tcstod(String, NULL); }

	//載入 JOG Acceleration Time
	KeyName.Format(_T("JOG Acceleration Time"));
	Default.Format(_T("%f"), MotionParam.m_JogAccelerationTime);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_JogAccelerationTime = ::_tcstod(String, NULL); }

	//載入 JOG Deceleration Time
	KeyName.Format(_T("JOG Deceleration Time"));
	Default.Format(_T("%f"), MotionParam.m_JogDecelerationTime);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_JogDecelerationTime = ::_tcstod(String, NULL); }
	
	//X軸自由移動時的最大速度 mm/s/s
	KeyName.Format(_T("X Axis JOG Max Velocity"));
	Default.Format(_T("%f"), MotionParam.m_JogMaxVelocityX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_JogMaxVelocityX = ::_tcstod(String, NULL); }

	//Y軸自由移動時的最大速度 mm/s/s
	KeyName.Format(_T("Y Axis JOG Max Velocity"));
	Default.Format(_T("%f"), MotionParam.m_JogMaxVelocityY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_JogMaxVelocityY = ::_tcstod(String, NULL); }

	//Z軸自由移動時的最大速度 mm/s/s
	KeyName.Format(_T("Z Axis JOG Max Velocity"));
	Default.Format(_T("%f"), MotionParam.m_JogMaxVelocityZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_JogMaxVelocityZ = ::_tcstod(String, NULL); }

	//X軸自由移動時的移動速度 mm/s/s
	KeyName.Format(_T("X Axis JOG Moving Velocity"));
	Default.Format(_T("%f"), MotionParam.m_JogMovingVelocityX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_JogMovingVelocityX = ::_tcstod(String, NULL); }

	//Y軸自由移動時的移動速度 mm/s/s
	KeyName.Format(_T("Y Axis JOG Moving Velocity"));
	Default.Format(_T("%f"), MotionParam.m_JogMovingVelocityY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_JogMovingVelocityY = ::_tcstod(String, NULL); }

	//Z軸自由移動時的移動速度 mm/s/s
	KeyName.Format(_T("Z Axis JOG Moving Velocity"));
	Default.Format(_T("%f"), MotionParam.m_JogMovingVelocityZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_JogMovingVelocityZ = ::_tcstod(String, NULL); }	

	//X軸自由移動的方向
	KeyName.Format(_T("X Axis JOG Direction"));
	Default.Format(_T("%d"), MotionParam.m_JogDirectionX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_JogDirectionX = ::_ttoi(String); }	

	//Y軸自由移動的方向
	KeyName.Format(_T("Y Axis JOG Direction"));
	Default.Format(_T("%d"), MotionParam.m_JogDirectionY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_JogDirectionY = ::_ttoi(String); }	

	//Z軸自由移動的方向
	KeyName.Format(_T("Z Axis JOG Direction"));
	Default.Format(_T("%d"), MotionParam.m_JogDirectionZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_JogDirectionZ = ::_ttoi(String); }	

	//載入 Trigger Forward Offset
	KeyName.Format(_T("Forward Trigger Offset"));
	Default.Format(_T("%d"), MotionParam.m_TriggerForwardOffset);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_TriggerForwardOffset = ::_tcstod(String, NULL); }

	//載入 Trigger Backward Offset
	KeyName.Format(_T("Backward Trigger Offset"));
	Default.Format(_T("%d"), MotionParam.m_TriggerBackwardOffset);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_TriggerBackwardOffset = ::_tcstod(String, NULL); }

	//載入 X Axis Acceleration //X軸加速度, 單位mm/s/s
	KeyName.Format(_T("X Axis Acceleration Value"));
	Default.Format(_T("%f"), MotionParam.m_AccelerationValueX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_AccelerationValueX = ::_tcstod(String, NULL); }	
	
	//載入 Y Axis Acceleration //Y軸加速度, 單位mm/s/s
	KeyName.Format(_T("Y Axis Acceleration Value"));
	Default.Format(_T("%f"), MotionParam.m_AccelerationValueY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_AccelerationValueY = ::_tcstod(String, NULL); }

	//載入 Z Axis Acceleration //Y軸加速度, 單位mm/s/s
	KeyName.Format(_T("Z Axis Acceleration Value"));
	Default.Format(_T("%f"), MotionParam.m_AccelerationValueZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_AccelerationValueZ = ::_tcstod(String, NULL); }

	//載入X軸加速度最小時間, sec
	KeyName.Format(_T("X Axis Acceleration Min Time"));
	Default.Format(_T("%.4f"), MotionParam.m_AccelerationMinTimeX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_AccelerationMinTimeX = ::_tcstod(String, NULL); }	

	//載入Y軸加速度最小時間, sec
	KeyName.Format(_T("Y Axis Acceleration Min Time"));
	Default.Format(_T("%.4f"), MotionParam.m_AccelerationMinTimeY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_AccelerationMinTimeY = ::_tcstod(String, NULL); }		

	//載入Z軸加速度最小時間, sec
	KeyName.Format(_T("Z Axis Acceleration Min Time"));
	Default.Format(_T("%.4f"), MotionParam.m_AccelerationMinTimeZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_AccelerationMinTimeZ = ::_tcstod(String, NULL); }		
	
	//X軸調整加速度啟動距離-單位um
	KeyName.Format(_T("X Axis Adjust Acceleration Distance"));
	Default.Format(_T("%.4f"), MotionParam.m_AdjustAccclerationDistX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_AdjustAccclerationDistX = ::_tcstod(String, NULL); }	

	//Y軸調整加速度啟動距離-單位um
	KeyName.Format(_T("Y Axis Adjust Acceleration Distance"));
	Default.Format(_T("%.4f"), MotionParam.m_AdjustAccclerationDistY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_AdjustAccclerationDistY = ::_tcstod(String, NULL); }	

	//Z軸調整加速度啟動距離-單位um
	KeyName.Format(_T("Z Axis Adjust Acceleration Distance"));
	Default.Format(_T("%.4f"), MotionParam.m_AdjustAccclerationDistZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_AdjustAccclerationDistZ = ::_tcstod(String, NULL); }	


	//X軸移動曲線
	KeyName.Format(_T("X Axis Moving Curve Mode"));
	Default.Format(_T("%d"), MotionParam.m_MovingCurveModeX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_MovingCurveModeX = (MOVE_CURVE_MODE)(::_ttoi(String)); }		

	//Y軸移動曲線
	KeyName.Format(_T("Y Axis Moving Curve Mode"));
	Default.Format(_T("%d"), MotionParam.m_MovingCurveModeY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_MovingCurveModeY = (MOVE_CURVE_MODE)(::_ttoi(String)); }		

	//Z軸移動曲線
	KeyName.Format(_T("Z Axis Moving Curve Mode"));
	Default.Format(_T("%d"), MotionParam.m_MovingCurveModeZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_MovingCurveModeZ = (MOVE_CURVE_MODE)(::_ttoi(String)); }		

	//X軸加減速度時間調整模式
	KeyName.Format(_T("X Axis Acc Time Adjust Mode"));
	Default.Format(_T("%d"), MotionParam.m_AccTimeAdjustModeX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_AccTimeAdjustModeX = (ACC_TIME_ADJUST_MODE)(::_ttoi(String)); }	

	//Y軸加減速度時間調整模式
	KeyName.Format(_T("Y Axis Acc Time Adjust Mode"));
	Default.Format(_T("%d"), MotionParam.m_AccTimeAdjustModeY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_AccTimeAdjustModeY = (ACC_TIME_ADJUST_MODE)(::_ttoi(String)); }	

	//Z軸加減速度時間調整模式	
	KeyName.Format(_T("Z Axis Acc Time Adjust Mode"));
	Default.Format(_T("%d"), MotionParam.m_AccTimeAdjustModeZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_AccTimeAdjustModeZ = (ACC_TIME_ADJUST_MODE)(::_ttoi(String)); }	

	//X軸三角波抑制
	KeyName.Format(_T("X Axis Triangle Correction"));
	Default.Format(_T("%d"), MotionParam.m_TriangleCorrectionX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_TriangleCorrectionX = (bool)(::_ttoi(String)); }		

	//Y軸三角波抑制
	KeyName.Format(_T("Y Axis Triangle Correction"));
	Default.Format(_T("%d"), MotionParam.m_TriangleCorrectionY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_TriangleCorrectionY = (bool)(::_ttoi(String)); }		
		
	//Z軸三角波抑制
	KeyName.Format(_T("Z Axis Triangle Correction"));
	Default.Format(_T("%d"), MotionParam.m_TriangleCorrectionZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_TriangleCorrectionZ = (bool)(::_ttoi(String)); }		

	//X軸測驗時間的偏移量, um
	KeyName.Format(_T("X Axis Test Time Distance"));
	Default.Format(_T("%.16f"), MotionParam.m_TestTimeDistanceX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_TestTimeDistanceX = (::_ttof(String)); }		

	//Y軸測驗時間的偏移量, um
	KeyName.Format(_T("Y Axis Test Time Distance"));
	Default.Format(_T("%.16f"), MotionParam.m_TestTimeDistanceY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_TestTimeDistanceY = (::_ttof(String)); }		

	//Z軸測驗時間的偏移量, um
	KeyName.Format(_T("Z Axis Test Time Distance"));
	Default.Format(_T("%.16f"), MotionParam.m_TestTimeDistanceZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_TestTimeDistanceZ = (::_ttof(String)); }			

	//載入 X 軸的最大範圍
	KeyName.Format(_T("X Axis Limit Max"));
	Default.Format(_T("%f"), MotionParam.m_LimitMaxX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LimitMaxX = ::_tcstod(String, NULL); }

	//載入 Y 軸的最大範圍
	KeyName.Format(_T("Y Axis Limit Max"));
	Default.Format(_T("%f"), MotionParam.m_LimitMaxY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LimitMaxY = ::_tcstod(String, NULL); }

	//載入 Z 軸的最大範圍
	KeyName.Format(_T("Z Axis Limit Max"));
	Default.Format(_T("%f"), MotionParam.m_LimitMaxZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LimitMaxZ = ::_tcstod(String, NULL); }

	//載入 X 軸的最小範圍
	KeyName.Format(_T("X Axis Limit Min"));
	Default.Format(_T("%f"), MotionParam.m_LimitMinX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LimitMinX = ::_tcstod(String, NULL); }

	//載入 Y 軸的最小範圍
	KeyName.Format(_T("Y Axis Limit Min"));
	Default.Format(_T("%f"), MotionParam.m_LimitMinY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LimitMinY = ::_tcstod(String, NULL); }

	//載入 Z 軸的最小範圍
	KeyName.Format(_T("Z Axis Limit Min"));
	Default.Format(_T("%f"), MotionParam.m_LimitMinZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LimitMinZ = ::_tcstod(String, NULL); }
	
	//載入 X 軸的範圍內縮值
	KeyName.Format(_T("X Axis Limit Margin"));
	Default.Format(_T("%f"), MotionParam.m_LimitMarginX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LimitMarginX = ::_tcstod(String, NULL); }

	//載入 Y 軸的範圍內縮值
	KeyName.Format(_T("Y Axis Limit Margin"));
	Default.Format(_T("%f"), MotionParam.m_LimitMarginY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LimitMarginY = ::_tcstod(String, NULL); }

	//載入 Z 軸的範圍內縮值
	KeyName.Format(_T("Z Axis Limit Margin"));
	Default.Format(_T("%f"), MotionParam.m_LimitMarginZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_LimitMarginZ = ::_tcstod(String, NULL); }

	//是否啟用運動系統內部的軟體極限
	KeyName.Format(_T("Using Motion Software Limit"));
	Default.Format(_T("%d"), MotionParam.m_UsingMotionSoftwareLimit);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	
		MotionParam.m_UsingMotionSoftwareLimit = ::_ttoi(String); 
		switch ( MotionParam.m_UsingMotionSoftwareLimit )
		{
		case FN_DISABLE:
		case FN_ENABLE:			
			break;
		default:
			MotionParam.m_UsingMotionSoftwareLimit = FN_DISABLE;
			break;
		}
	}

	//載入 Controller X軸 IP
	KeyName.Format(_T("Controller IP X"));
	Default.Format(_T("%s"), MotionParam.m_ControllerIPX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_ControllerIPX = String; }

	//載入 Controller Y軸 IP
	KeyName.Format(_T("Controller IP Y"));
	Default.Format(_T("%s"), MotionParam.m_ControllerIPY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_ControllerIPY = String; }

	//載入 Controller Z軸 IP
	KeyName.Format(_T("Controller IP Z"));
	Default.Format(_T("%s"), MotionParam.m_ControllerIPZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_ControllerIPZ = String; }

	//載入 Controller X軸 Port
	KeyName.Format(_T("Controller Port X"));
	Default.Format(_T("%d"), MotionParam.m_ControllerPortX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_ControllerPortX = ::_ttoi(String); }

	//載入 Controller Y軸 Port
	KeyName.Format(_T("Controller Port Y"));
	Default.Format(_T("%d"), MotionParam.m_ControllerPortY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_ControllerPortY = ::_ttoi(String); }

	//載入 Controller Z軸 Port
	KeyName.Format(_T("Controller Port Z"));
	Default.Format(_T("%d"), MotionParam.m_ControllerPortZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_ControllerPortZ = ::_ttoi(String); }

	
	//X軸與Cad的正負向
	KeyName.Format(_T("X Stage Sign Positive"));
	Default.Format(_T("%d"), MotionParam.m_SignPositiveX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_SignPositiveX = ::_ttoi(String); }

	//Y軸與Cad的正負向
	KeyName.Format(_T("Y Stage Sign Positive"));
	Default.Format(_T("%d"), MotionParam.m_SignPositiveY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_SignPositiveY = ::_ttoi(String); }

	//Z軸與Cad的正負向
	KeyName.Format(_T("Z Stage Sign Positive"));
	Default.Format(_T("%d"), MotionParam.m_SignPositiveZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_SignPositiveZ = ::_ttoi(String); }
	
	//X軸歸零前移動距離
	KeyName.Format(_T("X Axis Home Pre Move Dis"));
	Default.Format(_T("%.4f"), MotionParam.m_HomePreMoveDisX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_HomePreMoveDisX = ::_tcstod(String, NULL); }

	//Y軸歸零前移動距離
	KeyName.Format(_T("Y Axis Home Pre Move Dis"));
	Default.Format(_T("%.4f"), MotionParam.m_HomePreMoveDisY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_HomePreMoveDisY = ::_tcstod(String, NULL); }

	//Z軸歸零前移動距離
	KeyName.Format(_T("Z Axis Home Pre Move Dis"));
	Default.Format(_T("%.4f"), MotionParam.m_HomePreMoveDisZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_HomePreMoveDisZ = ::_tcstod(String, NULL); }

	//載入X軸原點偏差值
	KeyName.Format(_T("X Axis Home Offset"));
	Default.Format(_T("%.4f"), MotionParam.m_HomeOrgOffsetX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_HomeOrgOffsetX = ::_tcstod(String, NULL); }

	//載入Y軸原點偏差值
	KeyName.Format(_T("Y Axis Home Offset"));
	Default.Format(_T("%.4f"), MotionParam.m_HomeOrgOffsetY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_HomeOrgOffsetY = ::_tcstod(String, NULL); }

	//載入Z軸原點偏差值
	KeyName.Format(_T("Z Axis Home Offset"));
	Default.Format(_T("%.4f"), MotionParam.m_HomeOrgOffsetZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_HomeOrgOffsetZ = ::_tcstod(String, NULL); }

	//載入X軸原點速度值
	KeyName.Format(_T("X Axis Home Velocity"));
	Default.Format(_T("%.4f"), MotionParam.m_HomeVelocityX);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_HomeVelocityX = ::_tcstod(String, NULL); }	
	//if ( MotionParam.m_HomeVelocityX < 1000 ) { MotionParam.m_HomeVelocityX = 1000; }	

	//載入Y軸原點速度值
	KeyName.Format(_T("Y Axis Home Velocity"));
	Default.Format(_T("%.4f"), MotionParam.m_HomeVelocityY);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_HomeVelocityY = ::_tcstod(String, NULL); }	
	//if ( MotionParam.m_HomeVelocityY < 1000 ) { MotionParam.m_HomeVelocityY = 1000; }

	//載入Z軸原點速度值
	KeyName.Format(_T("Z Axis Home Velocity"));
	Default.Format(_T("%.4f"), MotionParam.m_HomeVelocityZ);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_HomeVelocityZ = ::_tcstod(String, NULL); }	
	//if ( MotionParam.m_HomeVelocityZ < 100 ) { MotionParam.m_HomeVelocityZ = 100; }	
	
	//是否儲存運動卡參數	
	KeyName.Format(_T("Save Motion Card Parameters"));
	Default.Format(_T("%d"), MotionParam.m_SaveMotionCardParam);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_SaveMotionCardParam = ::_ttoi(String); }	

	//是否載入運動訊息
	KeyName.Format(_T("Save Current Motion Process"));
	Default.Format(_T("%d"), MotionParam.m_SaveCurrentMotionProcess);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_SaveCurrentMotionProcess = ::_ttoi(String); }	

	//加速度調整
	KeyName.Format(_T("Enable Acceleration Adjust"));
	Default.Format(_T("%d"), MotionParam.m_AccelerationAdjust);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_AccelerationAdjust = ::_ttoi(String); }		

	//是否XY座標補正
	KeyName.Format(_T("Enable XY Calibration"));
	Default.Format(_T("%d"), MotionParam.m_XYCaliEnable);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_XYCaliEnable = ::_ttoi(String); }	
	
	//XY座標補正外擴範圍
	KeyName.Format(_T("XY Calibration Extend Range"));
	Default.Format(_T("%.16f"), MotionParam.m_XYCaliExtendRange);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_XYCaliExtendRange = ::_ttof(String); }		

	//S-Curve速度比例 
	KeyName.Format(_T("S-Curve Velocity Ratio"));
	Default.Format(_T("%.16f"), MotionParam.m_SCurveVelRatio);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_SCurveVelRatio = ::_ttof(String); }		

	//內部方向性轉換
	KeyName.Format(_T("Sign Positive Convert"));
	Default.Format(_T("%d"), MotionParam.m_SignPositiveConvert);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_SignPositiveConvert = ::_ttoi(String); }

	//是否啟用節點別名模式
	KeyName.Format(_T("Using ECAT Node Alias Name"));	
	Default.Format(_T("%d"), MotionParam.m_UsingEcatNodeAliasName);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	MotionParam.m_UsingEcatNodeAliasName = ::_ttoi(String);		}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::ExecSaveMotionParameter(LPCTSTR filename, const TMotionParameter &Param)//讀取運動參數
{
	const size_t textlen = 128;
	CString Filename;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString String = _T("");	
	
	Filename = filename;
	Section = GetMotionParamSectionName();	

	//儲存 Axis Start Velocity
	KeyName.Format(_T("Start Velocity"));
	String.Format(_T("%f"), Param.m_StartVelocity);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}		

	//儲存 X Axis Board Out Velocity
	KeyName.Format(_T("X Axis Board Out Velocity"));
	String.Format(_T("%f"), Param.m_BoardOutVelocityX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Y Axis Board Out Velocity
	KeyName.Format(_T("Y Axis Board Out Velocity"));
	String.Format(_T("%f"), Param.m_BoardOutVelocityY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Z Axis Board Out Velocity
	KeyName.Format(_T("Z Axis Board Out Velocity"));
	String.Format(_T("%f"), Param.m_BoardOutVelocityZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	
	//儲存 X Axis Board In Velocity
	KeyName.Format(_T("X Axis Board In Velocity"));
	String.Format(_T("%f"), Param.m_BoardInVelocityX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Y Axis Board In Velocity
	KeyName.Format(_T("Y Axis Board In Velocity"));
	String.Format(_T("%f"), Param.m_BoardInVelocityY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Z Axis Board In Velocity
	KeyName.Format(_T("Z Axis Board In Velocity"));
	String.Format(_T("%f"), Param.m_BoardInVelocityZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 X Axis Max Velocity
	KeyName.Format(_T("X Axis Max Velocity"));
	String.Format(_T("%f"), Param.m_MaxVelocityX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Y Axis Max Velocity
	KeyName.Format(_T("Y Axis Max Velocity"));
	String.Format(_T("%f"), Param.m_MaxVelocityY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Z Axis Max Velocity
	KeyName.Format(_T("Z Axis Max Velocity"));
	String.Format(_T("%f"), Param.m_MaxVelocityZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 X Axis Grabbing Velocity
	KeyName.Format(_T("X Axis Grabbing Velocity"));
	String.Format(_T("%f"), Param.m_GrabbingVelocityX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Y Axis Grabbing Velocity
	KeyName.Format(_T("Y Axis Grabbing Velocity"));
	String.Format(_T("%f"), Param.m_GrabbingVelocityY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Z Axis Grabbing Velocity
	KeyName.Format(_T("Z Axis Grabbing Velocity"));
	String.Format(_T("%f"), Param.m_GrabbingVelocityZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//X軸的取底圖移動速度, 
	KeyName.Format(_T("X Axis Grab Map Velocity"));
	String.Format(_T("%f"), Param.m_GrabMapVelocityX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//Y軸的取底圖移動速度, 
	KeyName.Format(_T("Y Axis Grab Map Velocity"));
	String.Format(_T("%f"), Param.m_GrabMapVelocityY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//Z軸的取底圖移動速度, 
	KeyName.Format(_T("Z Axis Grab Map Velocity"));
	String.Format(_T("%f"), Param.m_GrabMapVelocityZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存In Position Delay Time
	KeyName.Format(_T("In Position Delay Time"));
	String.Format(_T("%d"), Param.m_InPositionDelayTime);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	
	//儲存In Position Delay Time
	KeyName.Format(_T("In Position Delay Time"));
	String.Format(_T("%d"), Param.m_InPositionDelayTime);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Trigger 等速的提前距離
	KeyName.Format(_T("Trigger Pre-Offset Distance"));
	String.Format(_T("%f"), Param.m_PreTriggerOffsetDis);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Trigger 停止的提前距離
	KeyName.Format(_T("Trigger Post-Offset Distance"));
	String.Format(_T("%f"), Param.m_PostTriggerOffsetDis);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Trigger Start Velocity
	KeyName.Format(_T("Trigger Start Velocity"));
	String.Format(_T("%f"), Param.m_TriggerStartVelocity);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Trigger Max Velocity
	KeyName.Format(_T("Trigger Max Velocity"));
	String.Format(_T("%f"), Param.m_TriggerMaxVelocity);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 X Axis Acceleration Time
	KeyName.Format(_T("X Axis Acceleration Time"));
	String.Format(_T("%f"), Param.m_AccelerationTimeX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//儲存 Y Axis Acceleration Time
	KeyName.Format(_T("Y Axis Acceleration Time"));
	String.Format(_T("%f"), Param.m_AccelerationTimeY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Z Axis Acceleration Time
	KeyName.Format(_T("Z Axis Acceleration Time"));
	String.Format(_T("%f"), Param.m_AccelerationTimeZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 X Axis Deceleration Time
	KeyName.Format(_T("X Axis Deceleration Time"));
	String.Format(_T("%f"), Param.m_DecelerationTimeX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Y Axis Deceleration Time
	KeyName.Format(_T("Y Axis Deceleration Time"));
	String.Format(_T("%f"), Param.m_DecelerationTimeY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//儲存 Z Axis Deceleration Time
	KeyName.Format(_T("Z Axis Deceleration Time"));
	String.Format(_T("%f"), Param.m_DecelerationTimeZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//儲存 Stage Start Pos X
	KeyName.Format(_T("Stage Start Pos X"));
	String.Format(_T("%f"), Param.m_StageStartPosX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Stage Start Pos Y
	KeyName.Format(_T("Stage Start Pos Y"));
	String.Format(_T("%f"), Param.m_StageStartPosY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Stage Start Pos Z
	KeyName.Format(_T("Stage Start Pos Z"));
	String.Format(_T("%f"), Param.m_StageStartPosZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Stage Leave Pos X
	KeyName.Format(_T("Stage Leave Pos X"));
	String.Format(_T("%f"), Param.m_StageLeavePosX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Stage Leave Pos Y
	KeyName.Format(_T("Stage Leave Pos Y"));
	String.Format(_T("%f"), Param.m_StageLeavePosY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	
	//儲存 Stage Leave Pos Z
	KeyName.Format(_T("Stage Leave Pos Z"));
	String.Format(_T("%f"), Param.m_StageLeavePosZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	//儲存 Before PCB-In Position X
	KeyName.Format(_T("Before PCB-In Pos X"));
	String.Format(_T("%f"), Param.m_BeforePCBInPosX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Before PCB-In Position Y
	KeyName.Format(_T("Before PCB-In Pos Y"));
	String.Format(_T("%f"), Param.m_BeforePCBInPosY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Before PCB-In Position Z
	KeyName.Format(_T("Before PCB-In Pos Z"));
	String.Format(_T("%f"), Param.m_BeforePCBInPosZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	
	
	//載入 PCB-Stop Position X Lane A - 右邊停板
	KeyName.Format(_T("PCB-Stop Pos X Lane A"));
	String.Format(_T("%f"), Param.m_PCBStopRPosX_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//載入 PCB-Stop Position Y Lane A - 右邊停板
	KeyName.Format(_T("PCB-Stop Pos Y Lane A"));
	String.Format(_T("%f"), Param.m_PCBStopRPosY_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//載入 PCB-Stop Position Z Lane A - 右邊停板
	KeyName.Format(_T("PCB-Stop Pos Z Lane A"));
	String.Format(_T("%f"), Param.m_PCBStopRPosZ_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//載入 PCB-Stop Position X Lane B - 右邊停板
	KeyName.Format(_T("PCB-Stop Pos X Lane B"));
	String.Format(_T("%f"), Param.m_PCBStopRPosX_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//載入 PCB-Stop Position Y Lane B - 右邊停板
	KeyName.Format(_T("PCB-Stop Pos Y Lane B"));
	String.Format(_T("%f"), Param.m_PCBStopRPosY_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//載入 PCB-Stop Position Z Lane B - 右邊停板
	KeyName.Format(_T("PCB-Stop Pos Z Lane B"));
	String.Format(_T("%f"), Param.m_PCBStopRPosZ_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	
	
	//載入 PCB-Stop Left Position X Lane A - 左邊停板
	KeyName.Format(_T("PCB-Stop Left Pos X Lane A"));
	String.Format(_T("%f"), Param.m_PCBStopLPosX_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//載入 PCB-Stop Left Position Y Lane A - 左邊停板
	KeyName.Format(_T("PCB-Stop Left Pos Y Lane A"));
	String.Format(_T("%f"), Param.m_PCBStopLPosY_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	
	//載入 PCB-Stop Left Position Z Lane A - 左邊停板
	KeyName.Format(_T("PCB-Stop Left Pos Z Lane A"));
	String.Format(_T("%f"), Param.m_PCBStopLPosZ_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//載入 PCB-Stop Left Position X Lane B - 左邊停板
	KeyName.Format(_T("PCB-Stop Left Pos X Lane B"));
	String.Format(_T("%f"), Param.m_PCBStopLPosX_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//載入 PCB-Stop Left Position Y Lane B - 左邊停板
	KeyName.Format(_T("PCB-Stop Left Pos Y Lane B"));
	String.Format(_T("%f"), Param.m_PCBStopLPosY_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//載入 PCB-Stop Left Pos Z Lane B - 左邊停板
	KeyName.Format(_T("PCB-Stop Left Pos Z Lane B"));
	String.Format(_T("%f"), Param.m_PCBStopLPosZ_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//A軌道LED右停板的位置X	
	KeyName.Format(_T("Lane-LED Right Stop Pos X Lane A"));
	String.Format(_T("%f"), Param.m_LaneLedStopRPosX_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//A軌道LED右停板的位置Y
	KeyName.Format(_T("Lane-LED Right Stop Pos Y Lane A"));
	String.Format(_T("%f"), Param.m_LaneLedStopRPosY_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//A軌道LED右停板的位置Z
	KeyName.Format(_T("Lane-LED Right Stop Pos Z Lane A"));
	String.Format(_T("%f"), Param.m_LaneLedStopRPosZ_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//A軌道LED右減速的位置X
	KeyName.Format(_T("Lane-LED Right Slow Pos X Lane A"));
	String.Format(_T("%f"), Param.m_LaneLedSlowRPosX_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//A軌道LED右減速的位置Y
	KeyName.Format(_T("Lane-LED Right Slow Pos Y Lane A"));
	String.Format(_T("%f"), Param.m_LaneLedSlowRPosY_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//A軌道LED右減速的位置Z
	KeyName.Format(_T("Lane-LED Right Slow Pos Z Lane A"));
	String.Format(_T("%f"), Param.m_LaneLedSlowRPosZ_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	
	//B軌道LED右停板的位置X	
	KeyName.Format(_T("Lane-LED Right Stop Pos X Lane B"));
	String.Format(_T("%f"), Param.m_LaneLedStopRPosX_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//B軌道LED右停板的位置Y
	KeyName.Format(_T("Lane-LED Right Stop Pos Y Lane B"));
	String.Format(_T("%f"), Param.m_LaneLedStopRPosY_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//B軌道LED右停板的位置Z
	KeyName.Format(_T("Lane-LED Right Stop Pos Z Lane B"));
	String.Format(_T("%f"), Param.m_LaneLedStopRPosZ_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//B軌道LED右減速的位置X
	KeyName.Format(_T("Lane-LED Right Slow Pos X Lane B"));
	String.Format(_T("%f"), Param.m_LaneLedSlowRPosX_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//B軌道LED右減速的位置Y
	KeyName.Format(_T("Lane-LED Right Slow Pos Y Lane B"));
	String.Format(_T("%f"), Param.m_LaneLedSlowRPosY_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//B軌道LED右減速的位置Z
	KeyName.Format(_T("Lane-LED Right Slow Pos Z Lane B"));
	String.Format(_T("%f"), Param.m_LaneLedSlowRPosZ_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//A軌道LED左停板的位置X	
	KeyName.Format(_T("Lane-LED Left Stop Pos X Lane A"));
	String.Format(_T("%f"), Param.m_LaneLedStopLPosX_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//A軌道LED左停板的位置Y
	KeyName.Format(_T("Lane-LED Left Stop Pos Y Lane A"));
	String.Format(_T("%f"), Param.m_LaneLedStopLPosY_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//A軌道LED左停板的位置Z
	KeyName.Format(_T("Lane-LED Left Stop Pos Z Lane A"));
	String.Format(_T("%f"), Param.m_LaneLedStopLPosZ_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//A軌道LED左減速的位置X
	KeyName.Format(_T("Lane-LED Left Slow Pos X Lane A"));
	String.Format(_T("%f"), Param.m_LaneLedSlowLPosX_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//A軌道LED左減速的位置Y
	KeyName.Format(_T("Lane-LED Left Slow Pos Y Lane A"));
	String.Format(_T("%f"), Param.m_LaneLedSlowLPosY_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//A軌道LED左減速的位置Z
	KeyName.Format(_T("Lane-LED Left Slow Pos Z Lane A"));
	String.Format(_T("%f"), Param.m_LaneLedSlowLPosZ_LA);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	
	//B軌道LED左停板的位置X	
	KeyName.Format(_T("Lane-LED Left Stop Pos X Lane B"));
	String.Format(_T("%f"), Param.m_LaneLedStopLPosX_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//B軌道LED左停板的位置Y
	KeyName.Format(_T("Lane-LED Left Stop Pos Y Lane B"));
	String.Format(_T("%f"), Param.m_LaneLedStopLPosY_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//B軌道LED左停板的位置Z
	KeyName.Format(_T("Lane-LED Left Stop Pos Z Lane B"));
	String.Format(_T("%f"), Param.m_LaneLedStopLPosZ_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//B軌道LED左減速的位置X
	KeyName.Format(_T("Lane-LED Left Slow Pos X Lane B"));
	String.Format(_T("%f"), Param.m_LaneLedSlowLPosX_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//B軌道LED左減速的位置Y
	KeyName.Format(_T("Lane-LED Left Slow Pos Y Lane B"));
	String.Format(_T("%f"), Param.m_LaneLedSlowLPosY_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//B軌道LED左減速的位置Z
	KeyName.Format(_T("Lane-LED Left Slow Pos Z Lane B"));
	String.Format(_T("%f"), Param.m_LaneLedSlowLPosZ_LB);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//儲存 JOG Start Velocity
	KeyName.Format(_T("JOG Start Velocity"));
	String.Format(_T("%f"), Param.m_JogStartVelocity);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 JOG Moving Velocity
	KeyName.Format(_T("JOG Moving Velocity"));
	String.Format(_T("%f"), Param.m_JogMovingVelocity);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 JOG Max Velocity
	KeyName.Format(_T("JOG Max Velocity"));
	String.Format(_T("%f"), Param.m_JogMaxVelocity);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 JOG Acceleration Time
	KeyName.Format(_T("JOG Acceleration Time"));
	String.Format(_T("%.16f"), Param.m_JogAccelerationTime);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 JOG Deceleration Time
	KeyName.Format(_T("JOG Deceleration Time"));
	String.Format(_T("%.16f"), Param.m_JogDecelerationTime);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	
	//X軸自由移動時的最大速度 mm/s/s
	KeyName.Format(_T("X Axis JOG Max Velocity"));
	String.Format(_T("%.16f"), Param.m_JogMaxVelocityX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}		

	//Y軸自由移動時的最大速度 mm/s/s
	KeyName.Format(_T("Y Axis JOG Max Velocity"));
	String.Format(_T("%.16f"), Param.m_JogMaxVelocityY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//Z軸自由移動時的最大速度 mm/s/s
	KeyName.Format(_T("Z Axis JOG Max Velocity"));
	String.Format(_T("%.16f"), Param.m_JogMaxVelocityZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//X軸自由移動時的移動速度 mm/s/s
	KeyName.Format(_T("X Axis JOG Moving Velocity"));
	String.Format(_T("%.16f"), Param.m_JogMovingVelocityX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//Y軸自由移動時的移動速度 mm/s/s
	KeyName.Format(_T("Y Axis JOG Moving Velocity"));
	String.Format(_T("%.16f"), Param.m_JogMovingVelocityY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//Z軸自由移動時的移動速度 mm/s/s
	KeyName.Format(_T("Z Axis JOG Moving Velocity"));
	String.Format(_T("%.16f"), Param.m_JogMovingVelocityZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//X軸自由移動的方向
	KeyName.Format(_T("X Axis JOG Direction"));
	String.Format(_T("%d"), Param.m_JogDirectionX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//Y軸自由移動的方向
	KeyName.Format(_T("Y Axis JOG Direction"));
	String.Format(_T("%d"), Param.m_JogDirectionY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//Z軸自由移動的方向
	KeyName.Format(_T("Z Axis JOG Direction"));
	String.Format(_T("%d"), Param.m_JogDirectionZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 X Axis Acceleration //X軸加速度, 單位mm/s/s
	KeyName.Format(_T("X Axis Acceleration Value"));
	String.Format(_T("%.16f"), Param.m_AccelerationValueX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Y Axis Acceleration //Y軸加速度, 單位mm/s/s
	KeyName.Format(_T("Y Axis Acceleration Value"));
	String.Format(_T("%.16f"), Param.m_AccelerationValueY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Z Axis Acceleration //Y軸加速度, 單位mm/s/s
	KeyName.Format(_T("Z Axis Acceleration Value"));
	String.Format(_T("%.16f"), Param.m_AccelerationValueZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	
	//儲存X軸加速度最小時間, sec
	KeyName.Format(_T("X Axis Acceleration Min Time"));
	String.Format(_T("%.16f"), Param.m_AccelerationMinTimeX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存Y軸加速度最小時間, sec
	KeyName.Format(_T("Y Axis Acceleration Min Time"));
	String.Format(_T("%.16f"), Param.m_AccelerationMinTimeY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存Z軸加速度最小時間, sec
	KeyName.Format(_T("Z Axis Acceleration Min Time"));
	String.Format(_T("%.16f"), Param.m_AccelerationMinTimeZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存X軸調整加速度啟動距離-單位um
	KeyName.Format(_T("X Axis Adjust Acceleration Distance"));	
	String.Format(_T("%.4f"), Param.m_AdjustAccclerationDistX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//儲存Y軸調整加速度啟動距離-單位um
	KeyName.Format(_T("Y Axis Adjust Acceleration Distance"));	
	String.Format(_T("%.4f"), Param.m_AdjustAccclerationDistY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//儲存Z軸調整加速度啟動距離-單位um
	KeyName.Format(_T("Z Axis Adjust Acceleration Distance"));	
	String.Format(_T("%.4f"), Param.m_AdjustAccclerationDistZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//X軸移動曲線
	KeyName.Format(_T("X Axis Moving Curve Mode"));
	String.Format(_T("%d"), Param.m_MovingCurveModeX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//Y軸移動曲線
	KeyName.Format(_T("Y Axis Moving Curve Mode"));
	String.Format(_T("%d"), Param.m_MovingCurveModeY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//Z軸移動曲線
	KeyName.Format(_T("Z Axis Moving Curve Mode"));
	String.Format(_T("%d"), Param.m_MovingCurveModeZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//X軸加減速度時間調整模式
	KeyName.Format(_T("X Axis Acc Time Adjust Mode"));
	String.Format(_T("%d"), Param.m_AccTimeAdjustModeX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//Y軸加減速度時間調整模式
	KeyName.Format(_T("Y Axis Acc Time Adjust Mode"));
	String.Format(_T("%d"), Param.m_AccTimeAdjustModeY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	
	
	//Z軸加減速度時間調整模式	
	KeyName.Format(_T("Z Axis Acc Time Adjust Mode"));
	String.Format(_T("%d"), Param.m_AccTimeAdjustModeZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//X軸三角波抑制
	KeyName.Format(_T("X Axis Triangle Correction"));
	String.Format(_T("%d"), Param.m_TriangleCorrectionX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//Y軸三角波抑制
	KeyName.Format(_T("Y Axis Triangle Correction"));
	String.Format(_T("%d"), Param.m_TriangleCorrectionY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//Z軸三角波抑制
	KeyName.Format(_T("Z Axis Triangle Correction"));
	String.Format(_T("%d"), Param.m_TriangleCorrectionZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//X軸測驗時間的偏移量, um
	KeyName.Format(_T("X Axis Test Time Distance"));
	String.Format(_T("%.16f"), Param.m_TestTimeDistanceX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//Y軸測驗時間的偏移量, um
	KeyName.Format(_T("Y Axis Test Time Distance"));
	String.Format(_T("%.16f"), Param.m_TestTimeDistanceY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//Z軸測驗時間的偏移量, um
	KeyName.Format(_T("Z Axis Test Time Distance"));
	String.Format(_T("%.16f"), Param.m_TestTimeDistanceZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 X 軸的最大範圍
	KeyName.Format(_T("X Axis Limit Max"));
	String.Format(_T("%f"), Param.m_LimitMaxX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Y 軸的最大範圍
	KeyName.Format(_T("Y Axis Limit Max"));
	String.Format(_T("%f"), Param.m_LimitMaxY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Z 軸的最大範圍
	KeyName.Format(_T("Z Axis Limit Max"));
	String.Format(_T("%f"), Param.m_LimitMaxZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 X 軸的最小範圍
	KeyName.Format(_T("X Axis Limit Min"));
	String.Format(_T("%f"), Param.m_LimitMinX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Y 軸的最小範圍
	KeyName.Format(_T("Y Axis Limit Min"));
	String.Format(_T("%f"), Param.m_LimitMinY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Z 軸的最小範圍
	KeyName.Format(_T("Z Axis Limit Min"));
	String.Format(_T("%f"), Param.m_LimitMinZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	
	//儲存 X 軸的範圍內縮值
	KeyName.Format(_T("X Axis Limit Margin"));
	String.Format(_T("%f"), Param.m_LimitMarginX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Y 軸的範圍內縮值
	KeyName.Format(_T("Y Axis Limit Margin"));
	String.Format(_T("%f"), Param.m_LimitMarginY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Z 軸的範圍內縮值
	KeyName.Format(_T("Z Axis Limit Margin"));
	String.Format(_T("%f"), Param.m_LimitMarginZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	
	//是否啟用運動系統內部的軟體極限
	KeyName.Format(_T("Using Motion Software Limit"));
	String.Format(_T("%d"), Param.m_UsingMotionSoftwareLimit);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Controller X軸 IP
	KeyName.Format(_T("Controller IP X"));
	String.Format(_T("%s"), Param.m_ControllerIPX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Controller Y軸 IP
	KeyName.Format(_T("Controller IP Y"));
	String.Format(_T("%s"), Param.m_ControllerIPY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Controller Z軸 IP
	KeyName.Format(_T("Controller IP Z"));
	String.Format(_T("%s"), Param.m_ControllerIPZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	
	//儲存 Controller X軸 Port
	KeyName.Format(_T("Controller Port X"));
	String.Format(_T("%d"), Param.m_ControllerPortX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Controller Y軸 Port
	KeyName.Format(_T("Controller Port Y"));
	String.Format(_T("%d"), Param.m_ControllerPortY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存 Controller Z軸 Port
	KeyName.Format(_T("Controller Port Z"));
	String.Format(_T("%d"), Param.m_ControllerPortZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	
	//X軸與Cad的正負向
	KeyName.Format(_T("X Stage Sign Positive"));
	String.Format(_T("%d"), Param.m_SignPositiveX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//Y軸與Cad的正負向
	KeyName.Format(_T("Y Stage Sign Positive"));
	String.Format(_T("%d"), Param.m_SignPositiveY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//Z軸與Cad的正負向
	KeyName.Format(_T("Z Stage Sign Positive"));
	String.Format(_T("%d"), Param.m_SignPositiveZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//X軸歸零前移動距離
	KeyName.Format(_T("X Axis Home Pre Move Dis"));
	String.Format(_T("%.4f"), Param.m_HomePreMoveDisX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//Y軸歸零前移動距離
	KeyName.Format(_T("Y Axis Home Pre Move Dis"));
	String.Format(_T("%.4f"), Param.m_HomePreMoveDisY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//Z軸歸零前移動距離
	KeyName.Format(_T("Z Axis Home Pre Move Dis"));
	String.Format(_T("%.4f"), Param.m_HomePreMoveDisZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	
	//儲存X軸原點偏差值
	KeyName.Format(_T("X Axis Home Offset"));
	String.Format(_T("%.4f"), Param.m_HomeOrgOffsetX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存Y軸原點偏差值
	KeyName.Format(_T("Y Axis Home Offset"));
	String.Format(_T("%.4f"), Param.m_HomeOrgOffsetY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存Z軸原點偏差值
	KeyName.Format(_T("Z Axis Home Offset"));
	String.Format(_T("%.4f"), Param.m_HomeOrgOffsetZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	
	//儲存X軸原點速度值
	KeyName.Format(_T("X Axis Home Velocity"));
	String.Format(_T("%.4f"), Param.m_HomeVelocityX);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//儲存Y軸原點速度值
	KeyName.Format(_T("Y Axis Home Velocity"));
	String.Format(_T("%.4f"), Param.m_HomeVelocityY);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//儲存Y軸原點速度值
	KeyName.Format(_T("Z Axis Home Velocity"));
	String.Format(_T("%.4f"), Param.m_HomeVelocityZ);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//是否儲存運動卡參數
	//是否儲存運動卡參數	
	KeyName.Format(_T("Save Motion Card Parameters"));
	String.Format(_T("%d"), Param.m_SaveMotionCardParam);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//是否儲存運動訊息
	KeyName.Format(_T("Save Current Motion Process"));
	String.Format(_T("%d"), Param.m_SaveCurrentMotionProcess);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}
	
	//加速度調整
	KeyName.Format(_T("Enable Acceleration Adjust"));
	String.Format(_T("%d"), Param.m_AccelerationAdjust);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//是否XY座標補正
	KeyName.Format(_T("Enable XY Calibration"));
	String.Format(_T("%d"), Param.m_XYCaliEnable);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}

	//XY座標補正外擴範圍
	KeyName.Format(_T("XY Calibration Extend Range"));
	String.Format(_T("%.16f"), Param.m_XYCaliExtendRange);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	
	
	//S-Curve速度比例 
	KeyName.Format(_T("S-Curve Velocity Ratio"));
	String.Format(_T("%.16f"), Param.m_SCurveVelRatio);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//內部方向性轉換
	KeyName.Format(_T("Sign Positive Convert"));
	String.Format(_T("%d"), Param.m_SignPositiveConvert);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	

	//是否啟用節點別名模式
	KeyName.Format(_T("Using ECAT Node Alias Name"));
	String.Format(_T("%d"), Param.m_UsingEcatNodeAliasName);
	if (SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false)
	{	return false;	}
	return true;
}
//----------------------------------------------------------------------------------//
MOTION_CARD_TYPE CMotion_Basic::GetMotionCardType() const//取得軸控卡樣式
{
	return m_CardType;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionCardType(MOTION_CARD_TYPE Type)//設定軸控卡樣式
{
	m_CardType = Type;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::SaveMotionCardType()
{
	bool bSucc = SaveMotionCardType(GetMotionCardType());
	return bSucc;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::LoadMotionCardType(MOTION_CARD_TYPE &val)
{
	const size_t textlen = 128;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString Default = _T("");
	TCHAR   String[textlen]=_T("");		
	CString Filename=AOIDataCollect.GetSystemParamFilename();

	Section = GetMotionParamSectionName();
	//Motion Card Type
	KeyName.Format(_T("Motion Card Type"));
	Default.Format(_T("%d"), val);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, false, m_ErrorString) == true ) 	
	{	val = (MOTION_CARD_TYPE)::_ttoi(String); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Basic::SaveMotionCardType(MOTION_CARD_TYPE val)
{
	const size_t textlen = 128;
	CString Section = _T("");	
	CString KeyName = _T("");
	CString String = _T("");	
	CString Filename=AOIDataCollect.GetSystemParamFilename();

	Section = GetMotionParamSectionName();
	//Motion Card Type
	KeyName.Format(_T("Motion Card Type"));
	String.Format(_T("%d"), val);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false ) 
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
MACHINE_MODEL_TYPE CMotion_Basic::GetMotionMachineType() const//運動系統的機台樣式
{
	return m_MachineType;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionMachineType(MACHINE_MODEL_TYPE Type)//運動系統的機台樣式
{
	m_MachineType = Type;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::LoadMotionParameter()//載入運動參數
{
	CString Filename;
	Filename = AOIDataCollect.GetSystemParamFilename();
	bool IsOK = this->ExecLoadMotionParameter(Filename, m_MotionParameter);
	if ( false == IsOK )
	{
		SetMotionExceptionCode_FileRead();
		return false; 
	}
	UpdateMotionParamter();
	LoadMotionXYCali();
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::SaveMotionParameter()//讀取運動參數
{
	bool bIsOK = true;
	CString Filename;
	Filename = AOIDataCollect.GetSystemParamFilename();
	bIsOK = this->ExecSaveMotionParameter(Filename, m_MotionParameter);	
	if ( false == bIsOK )
	{
		SetMotionExceptionCode_FileWrite();
		return false;
	}
	return bIsOK;	 
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::UpdateMotionParamter()//更新運動參數
{
	const TMotionParameter &MotionParam=GetMotionParameter();	
	m_WaitForDoneDelayTime = MotionParam.m_InPositionDelayTime;
	UpdateMotionParameterToMotionAxis();
	//SetMotionExceptionCode_Param();
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::UpdateMotionParameterToMotionAxis()//更新運動參數至軸內
{
	size_t i=0;	
	int AxisID=0;
	int nVal=0;
	bool bVal=false;
	double fVal=0.0f;	
	const TMotionParameter &MotionParam=GetMotionParameter();	
	std::vector<CMotionAxis> &MotionAxisList=GetMotionAxisList();
	const size_t MotionAxisCount=MotionAxisList.size();
	for ( i=0; i<MotionAxisCount; i++ )
	{
		CMotionAxis &MotionAxisRef=MotionAxisList[i];
		AxisID = MotionAxisRef.GetAxisID();
		switch ( AxisID )
		{
		default:  nVal=FN_ENABLE;	break;
		case AXIS_X: nVal=MotionParam.m_SignPositiveX;	break;
		case AXIS_Y: nVal=MotionParam.m_SignPositiveY;	break;			
		case AXIS_Z: nVal=MotionParam.m_SignPositiveZ;	break;
		}
		MotionAxisRef.SetSignPositive(nVal);
		MotionAxisRef.SetSignPositiveConvert(MotionParam.m_SignPositiveConvert);

		switch ( AxisID )
		{
		default:  fVal=5000;	break;
		case AXIS_X: fVal=MotionParam.m_HomePreMoveDisX;	break;
		case AXIS_Y: fVal=MotionParam.m_HomePreMoveDisY;	break;			
		case AXIS_Z: fVal=MotionParam.m_HomePreMoveDisZ;	break;
		}
		MotionAxisRef.SetHomePreMoveDis(fVal);

		switch ( AxisID )
		{
		default:  fVal=50000;	break;
		case AXIS_X: fVal=MotionParam.m_HomeOrgOffsetX;	break;
		case AXIS_Y: fVal=MotionParam.m_HomeOrgOffsetY;	break;			
		case AXIS_Z: fVal=MotionParam.m_HomeOrgOffsetZ;	break;
		}
		MotionAxisRef.SetHomeOrgOffset(fVal);

		switch ( AxisID )
		{
		default:  fVal=10000;	break;
		case AXIS_X: fVal=MotionParam.m_HomeVelocityX;	break;
		case AXIS_Y: fVal=MotionParam.m_HomeVelocityY;	break;			
		case AXIS_Z: fVal=MotionParam.m_HomeVelocityZ;	break;
		}
		MotionAxisRef.SetHomeVelocity(fVal);
		MotionAxisRef.SetStartVelocity(MotionParam.m_StartVelocity);

		switch ( AxisID )
		{
		default:  fVal=100000;	break;
		case AXIS_X: fVal=MotionParam.m_MaxVelocityX;	break;
		case AXIS_Y: fVal=MotionParam.m_MaxVelocityY;	break;			
		case AXIS_Z: fVal=MotionParam.m_MaxVelocityZ;	break;
		}
		MotionAxisRef.SetMaxVelocity(fVal);

		switch ( AxisID )
		{
		default:  fVal=100000;	break;
		case AXIS_X: fVal=MotionParam.m_BoardInVelocityX;	break;
		case AXIS_Y: fVal=MotionParam.m_BoardInVelocityY;	break;			
		case AXIS_Z: fVal=MotionParam.m_BoardInVelocityZ;	break;
		}
		MotionAxisRef.SetBoardInVelocity(fVal);


		switch ( AxisID )
		{
		default:  fVal=100000;	break;
		case AXIS_X: fVal=MotionParam.m_BoardOutVelocityX;	break;
		case AXIS_Y: fVal=MotionParam.m_BoardOutVelocityY;	break;			
		case AXIS_Z: fVal=MotionParam.m_BoardOutVelocityZ;	break;
		}
		MotionAxisRef.SetBoardOutVelocity(fVal);

		switch ( AxisID )
		{
		default:  fVal=100000;	break;
		case AXIS_X: fVal=MotionParam.m_GrabbingVelocityX;	break;
		case AXIS_Y: fVal=MotionParam.m_GrabbingVelocityY;	break;			
		case AXIS_Z: fVal=MotionParam.m_GrabbingVelocityZ;	break;
		}
		MotionAxisRef.SetGrabbingVelocity(fVal);

		switch ( AxisID )
		{
		default:  fVal=100000;	break;
		case AXIS_X: fVal=MotionParam.m_GrabMapVelocityX;	break;
		case AXIS_Y: fVal=MotionParam.m_GrabMapVelocityY;	break;			
		case AXIS_Z: fVal=MotionParam.m_GrabMapVelocityZ;	break;
		}
		MotionAxisRef.SetGrabMapVelocity(fVal);

		switch ( AxisID )
		{
		default:  fVal=0;	break;
		case AXIS_X: fVal=MotionParam.m_StageStartPosX;	break;
		case AXIS_Y: fVal=MotionParam.m_StageStartPosY;	break;			
		case AXIS_Z: fVal=MotionParam.m_StageStartPosZ;	break;
		}
		MotionAxisRef.SetStageStartPos(fVal);

		switch ( AxisID )
		{
		default:  fVal=0;	break;
		case AXIS_X: fVal=MotionParam.m_StageLeavePosX;	break;
		case AXIS_Y: fVal=MotionParam.m_StageLeavePosY;	break;			
		case AXIS_Z: fVal=MotionParam.m_StageLeavePosZ;	break;
		}
		MotionAxisRef.SetStageLeavePos(fVal);

		switch ( AxisID )
		{
		default:  fVal=0;	break;
		case AXIS_X: fVal=MotionParam.m_BeforePCBInPosX;	break;
		case AXIS_Y: fVal=MotionParam.m_BeforePCBInPosY;	break;			
		case AXIS_Z: fVal=MotionParam.m_BeforePCBInPosZ;	break;
		}
		MotionAxisRef.SetBeforePCBInPos(fVal);
		
		switch ( AxisID )
		{
		default:  fVal=0;	break;
		case AXIS_X: fVal=MotionParam.m_PCBStopRPosX_LA;	break;
		case AXIS_Y: fVal=MotionParam.m_PCBStopRPosY_LA;	break;			
		case AXIS_Z: fVal=MotionParam.m_PCBStopRPosZ_LA;	break;
		}
		MotionAxisRef.SetPCBStopRPos_LA(fVal);

		switch ( AxisID )
		{
		default:  fVal=0;	break;
		case AXIS_X: fVal=MotionParam.m_PCBStopRPosX_LB;	break;
		case AXIS_Y: fVal=MotionParam.m_PCBStopRPosY_LB;	break;			
		case AXIS_Z: fVal=MotionParam.m_PCBStopRPosZ_LB;	break;
		}
		MotionAxisRef.SetPCBStopRPos_LB(fVal);

		switch ( AxisID )
		{
		default:  fVal=0;	break;
		case AXIS_X: fVal=MotionParam.m_PCBStopLPosX_LA;	break;
		case AXIS_Y: fVal=MotionParam.m_PCBStopLPosY_LA;	break;			
		case AXIS_Z: fVal=MotionParam.m_PCBStopLPosZ_LA;	break;
		}
		MotionAxisRef.SetPCBStopLPos_LA(fVal);

		switch ( AxisID )
		{
		default:  fVal=0;	break;
		case AXIS_X: fVal=MotionParam.m_PCBStopLPosX_LB;	break;
		case AXIS_Y: fVal=MotionParam.m_PCBStopLPosY_LB;	break;			
		case AXIS_Z: fVal=MotionParam.m_PCBStopLPosZ_LB;	break;
		}
		MotionAxisRef.SetPCBStopLPos_LB(fVal);

		switch ( AxisID )
		{
		default:  fVal=0;	break;
		case AXIS_X: fVal=MotionParam.m_LaneLedStopRPosX_LA;	break;
		case AXIS_Y: fVal=MotionParam.m_LaneLedStopRPosY_LA;	break;			
		case AXIS_Z: fVal=MotionParam.m_LaneLedStopRPosZ_LA;	break;
		}
		MotionAxisRef.SetLaneLedStopRPos_LA(fVal);

		switch ( AxisID )
		{
		default:  fVal=0;	break;
		case AXIS_X: fVal=MotionParam.m_LaneLedSlowRPosX_LA;	break;
		case AXIS_Y: fVal=MotionParam.m_LaneLedSlowRPosY_LA;	break;			
		case AXIS_Z: fVal=MotionParam.m_LaneLedSlowRPosZ_LA;	break;
		}
		MotionAxisRef.SetLaneLedSlowRPos_LA(fVal);

		switch ( AxisID )
		{
		default:  fVal=0;	break;
		case AXIS_X: fVal=MotionParam.m_LaneLedStopRPosX_LB;	break;
		case AXIS_Y: fVal=MotionParam.m_LaneLedStopRPosY_LB;	break;			
		case AXIS_Z: fVal=MotionParam.m_LaneLedStopRPosZ_LB;	break;
		}
		MotionAxisRef.SetLaneLedStopRPos_LB(fVal);

		switch ( AxisID )
		{
		default:  fVal=0;	break;
		case AXIS_X: fVal=MotionParam.m_LaneLedSlowRPosX_LB;	break;
		case AXIS_Y: fVal=MotionParam.m_LaneLedSlowRPosY_LB;	break;			
		case AXIS_Z: fVal=MotionParam.m_LaneLedSlowRPosZ_LB;	break;
		}
		MotionAxisRef.SetLaneLedSlowRPos_LB(fVal);

		switch ( AxisID )
		{
		default:  fVal=0;	break;
		case AXIS_X: fVal=MotionParam.m_LaneLedStopLPosX_LA;	break;
		case AXIS_Y: fVal=MotionParam.m_LaneLedStopLPosY_LA;	break;			
		case AXIS_Z: fVal=MotionParam.m_LaneLedStopLPosZ_LA;	break;
		}
		MotionAxisRef.SetLaneLedStopLPos_LA(fVal);

		switch ( AxisID )
		{
		default:  fVal=0;	break;
		case AXIS_X: fVal=MotionParam.m_LaneLedSlowLPosX_LA;	break;
		case AXIS_Y: fVal=MotionParam.m_LaneLedSlowLPosY_LA;	break;			
		case AXIS_Z: fVal=MotionParam.m_LaneLedSlowLPosZ_LA;	break;
		}
		MotionAxisRef.SetLaneLedSlowLPos_LA(fVal);

		switch ( AxisID )
		{
		default:  fVal=0;	break;
		case AXIS_X: fVal=MotionParam.m_LaneLedStopLPosX_LB;	break;
		case AXIS_Y: fVal=MotionParam.m_LaneLedStopLPosY_LB;	break;			
		case AXIS_Z: fVal=MotionParam.m_LaneLedStopLPosZ_LB;	break;
		}
		MotionAxisRef.SetLaneLedStopLPos_LB(fVal);

		switch ( AxisID )
		{
		default:  fVal=0;	break;
		case AXIS_X: fVal=MotionParam.m_LaneLedSlowLPosX_LB;	break;
		case AXIS_Y: fVal=MotionParam.m_LaneLedSlowLPosY_LB;	break;			
		case AXIS_Z: fVal=MotionParam.m_LaneLedSlowLPosZ_LB;	break;
		}
		MotionAxisRef.SetLaneLedSlowLPos_LB(fVal);

		MotionAxisRef.SetJogStartVelocity(MotionParam.m_JogStartVelocity);
		MotionAxisRef.SetJogAccelerationTime(MotionParam.m_JogAccelerationTime);
		MotionAxisRef.SetJogDecelerationTime(MotionParam.m_JogDecelerationTime);
		//MotionAxisRef.SetSCurveVelRatio(MotionParam.m_SCurveVelRatio);

		switch (AxisID)//v1.01.03.234
		{
		default:  fVal = MotionParam.m_JogMaxVelocity;	break;
		case AXIS_X: fVal = MotionParam.m_JogMaxVelocityX;	break;
		case AXIS_Y: fVal = MotionParam.m_JogMaxVelocityY;	break;
		case AXIS_Z: fVal = MotionParam.m_JogMaxVelocityZ;	break;
		}
		MotionAxisRef.SetJogMaxVelocity(fVal);

		switch (AxisID)//v1.01.03.234
		{
		default:  fVal = MotionParam.m_JogMovingVelocity;	break;
		case AXIS_X: fVal = MotionParam.m_JogMovingVelocityX;	break;
		case AXIS_Y: fVal = MotionParam.m_JogMovingVelocityY;	break;
		case AXIS_Z: fVal = MotionParam.m_JogMovingVelocityZ;	break;
		}
		MotionAxisRef.SetJogMovingVelocity(fVal);


		switch ( AxisID )
		{
		default:  fVal=2000000;	break;
		case AXIS_X: fVal=MotionParam.m_AccelerationValueX;	break;
		case AXIS_Y: fVal=MotionParam.m_AccelerationValueY;	break;			
		case AXIS_Z: fVal=MotionParam.m_AccelerationValueZ;	break;
		}
		MotionAxisRef.SetAccelerationValue(fVal);

		switch ( AxisID )
		{
		default:  fVal=0.1;	break;
		case AXIS_X: fVal=MotionParam.m_AccelerationTimeX;	break;
		case AXIS_Y: fVal=MotionParam.m_AccelerationTimeY;	break;			
		case AXIS_Z: fVal=MotionParam.m_AccelerationTimeZ;	break;
		}
		MotionAxisRef.SetAccelerationTime(fVal);
		
		switch ( AxisID )
		{
		default:  fVal=0.1;	break;
		case AXIS_X: fVal=MotionParam.m_DecelerationTimeX;	break;
		case AXIS_Y: fVal=MotionParam.m_DecelerationTimeY;	break;			
		case AXIS_Z: fVal=MotionParam.m_DecelerationTimeZ;	break;
		}
		MotionAxisRef.SetDecelerationTime(fVal);
		
		switch ( AxisID )
		{
		default:  fVal=0.05;	break;
		case AXIS_X: fVal=MotionParam.m_AccelerationMinTimeX;	break;
		case AXIS_Y: fVal=MotionParam.m_AccelerationMinTimeY;	break;			
		case AXIS_Z: fVal=MotionParam.m_AccelerationMinTimeZ;	break;
		}
		MotionAxisRef.SetAccelerationMinTime(fVal);

		switch ( AxisID )
		{
		default:  fVal=0.0;	break;
		case AXIS_X: fVal=MotionParam.m_AdjustAccclerationDistX;	break;
		case AXIS_Y: fVal=MotionParam.m_AdjustAccclerationDistY;	break;			
		case AXIS_Z: fVal=MotionParam.m_AdjustAccclerationDistZ;	break;
		}
		MotionAxisRef.SetAdjustAccclerationDist(fVal);	

		switch ( AxisID )
		{
		default:  nVal=MOVE_CURVE_T;	break;
		case AXIS_X: nVal=MotionParam.m_MovingCurveModeX;	break;
		case AXIS_Y: nVal=MotionParam.m_MovingCurveModeY;	break;			
		case AXIS_Z: nVal=MotionParam.m_MovingCurveModeZ;	break;
		}
		MotionAxisRef.SetMovingCurveMode((MOVE_CURVE_MODE)nVal);

		switch ( AxisID )
		{
		default:  nVal=ACC_TIME_ADJUST_OFF;	break;
		case AXIS_X: nVal=MotionParam.m_AccTimeAdjustModeX;	break;
		case AXIS_Y: nVal=MotionParam.m_AccTimeAdjustModeY;	break;			
		case AXIS_Z: nVal=MotionParam.m_AccTimeAdjustModeZ;	break;
		}
		MotionAxisRef.SetAccTimeAdjustMode((ACC_TIME_ADJUST_MODE)nVal);

		switch ( AxisID )
		{
		default:  bVal=false;	break;
		case AXIS_X: bVal=MotionParam.m_TriangleCorrectionX;	break;
		case AXIS_Y: bVal=MotionParam.m_TriangleCorrectionY;	break;			
		case AXIS_Z: bVal=MotionParam.m_TriangleCorrectionZ;	break;
		}
		MotionAxisRef.SetTriangleCorrection(bVal);

		switch ( AxisID )
		{
		default:  fVal=0;	break;
		case AXIS_X: fVal=MotionParam.m_TestTimeDistanceX;	break;
		case AXIS_Y: fVal=MotionParam.m_TestTimeDistanceY;	break;			
		case AXIS_Z: fVal=MotionParam.m_TestTimeDistanceZ;	break;
		}
		MotionAxisRef.SetTestTimeDistance(fVal);

		switch ( AxisID )
		{
		default:  fVal=10000000;	break;
		case AXIS_X: fVal=MotionParam.m_LimitMaxX;	break;
		case AXIS_Y: fVal=MotionParam.m_LimitMaxY;	break;			
		case AXIS_Z: fVal=MotionParam.m_LimitMaxZ;	break;
		}
		MotionAxisRef.SetLimitMax(fVal);

		switch ( AxisID )
		{
		default:  fVal=-10000000;	break;
		case AXIS_X: fVal=MotionParam.m_LimitMinX;	break;
		case AXIS_Y: fVal=MotionParam.m_LimitMinY;	break;			
		case AXIS_Z: fVal=MotionParam.m_LimitMinZ;	break;
		}
		MotionAxisRef.SetLimitMin(fVal);

		switch ( AxisID )
		{
		default:  fVal=5000;	break;
		case AXIS_X: fVal=MotionParam.m_LimitMarginX;	break;
		case AXIS_Y: fVal=MotionParam.m_LimitMarginY;	break;			
		case AXIS_Z: fVal=MotionParam.m_LimitMarginZ;	break;
		}
		MotionAxisRef.SetLimitMargin(fVal);		

		switch (AxisID)
		{
		default:  nVal = FN_ENABLE;	break;
		case AXIS_X: nVal = MotionParam.m_JogDirectionX;	break;
		case AXIS_Y: nVal = MotionParam.m_JogDirectionY;	break;
		case AXIS_Z: nVal = MotionParam.m_JogDirectionZ;	break;
		}
		if ( FN_ENABLE == nVal ) { nVal = 1; }
		else { nVal = -1; }
		MotionAxisRef.SetJogDirection(nVal);		
	}

	//Update to Slave Node List
	std::vector<CMotionNode> &MotionNodeList=GetMotionNodeList();
	for ( i=0; i<MotionAxisCount; i++ )
	{
		CMotionAxis &MotionAxisRef=MotionAxisList[i];
		MotionAxisRef.UpdateToSlaveNodeList();
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::LoadMotionParameter(LPCTSTR filename, TMotionParameter &Param)//載入運動參數
{
	bool IsOK = this->ExecLoadMotionParameter(filename, Param);
	if ( false == IsOK ) 
	{	return false; }	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::SaveMotionParameter(LPCTSTR filename, const TMotionParameter &Param)//讀取運動參數
{
	if ( ExecSaveMotionParameter(filename, Param) == false )
	{	return false; }
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::SaveMotionProcess(int Axis, LPCTSTR fnName, int Level)
{
	CString str;
	switch ( Axis )
	{
	case AXIS_X: str.Format(_T("Motion[X]::%s"), fnName);	break;
	case AXIS_Y: str.Format(_T("Motion[Y]::%s"), fnName);	break;
	case AXIS_Z: str.Format(_T("Motion[Z]::%s"), fnName);	break;
	default:     str.Format(_T("Motion[All]::%s"), fnName);	break;
	}
	if ( AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_MOTION, Level, str) == false )
	{	return false; }
	return true;
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetAxisFreeRunVelocity(int Axis)//取得軸自由移動的速度
{
	CMotionAxis *MotionAxisPtr = GetMotionAxisPtr(Axis);
	return GetAxisFreeRunVelocity(MotionAxisPtr);	
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetAxisFreeRunVelocity(CMotionAxis *MotionAxisPtr)//取得軸自由移動的速度
{
	double Velocity = 0;
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return Velocity; }	
	const bool bMaxVelocity = JetAPI::CheckIsPressVRKey(VK_SHIFT);
	if ( true == bMaxVelocity )
	{	Velocity = MotionAxisPtr->GetJogMaxVelocity();	}
	else
	{	Velocity = MotionAxisPtr->GetJogMovingVelocity();	}
	return Velocity;	
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CheckBitMask(int val, int mask)//確認位元比較
{
	return (mask & val) ? true : false;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::ReturnNoSupportFunc(LPCTSTR fnName)
{
	m_ErrorString.Format(_T("Error, Not Support Func[%s]"), fnName);
	return false;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Basic::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMotion_Basic::WaitForMotionStop(bool bDelay)//等運動系統停下來
{
#ifndef MOTION_OBJ_DISABLE
	if ( this->WaitForDone(AXIS_X) == false )
	{	return false;	}

	if ( this->WaitForDone(AXIS_Y) == false )
	{	return false;	}

	if ( this->WaitForDone(AXIS_Z) == false )
	{	return false;	}	

	if ( true==bDelay && m_WaitForDoneDelayTime>0 )
	{	
		DWORD TickCntd = 0;
		DWORD TickCnt2 = 0;
		DWORD TickCnt1 = ::GetTickCount();
		while ( true )
		{
			TickCnt2 = ::GetTickCount();
			TickCntd = TickCnt2-TickCnt1;
			if ( TickCntd < m_WaitForDoneDelayTime )
			{	continue; }
			break;
		};
		//::Sleep(m_WaitForDoneDelayTime);  
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::ExecLimit(int AxisNo)//確認極限範圍並且設定成軟體極限
{
#ifndef MOTION_OBJ_DISABLE		
	if ( ExecLimitFn(AxisNo) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_LIMIT_CALI);
		return false;
	}
#endif//MOTION_OBJ_DISABLE		
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::ExecLimitFn(int AxisNo)//確認極限範圍並且設定成軟體極限
{
#ifndef MOTION_OBJ_DISABLE
	CString str;
	if ( this->CheckInit() == false ) { return false; }		
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(AxisNo);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }

	//0. 關閉軟體極限, 以免走不到
	//1. 確認啟用沒	
	//2. 往最大座標移動
	//3. 往最小座標移動

	if ( this->GetIsEnable(AxisNo) == false )
	{	return false; }
	double MaxPosLimit = MotionAxisPtr->GetLimitMax();
	double MinPosLimit = MotionAxisPtr->GetLimitMin();	
	TMotionParameter &MotionParam=GetMotionParameter();
	switch ( AxisNo )
	{
	case AXIS_X:
		MaxPosLimit = MotionParam.m_LimitMaxX;
		MinPosLimit = MotionParam.m_LimitMinX;
		break;
	case AXIS_Y:
		MaxPosLimit = MotionParam.m_LimitMaxY;
		MinPosLimit = MotionParam.m_LimitMinY;
		break;
	case AXIS_Z:
		MaxPosLimit = MotionParam.m_LimitMaxZ;
		MinPosLimit = MotionParam.m_LimitMinZ;
		break;
	}
	DisableSoftwareLimit(AxisNo);	
	if ( this->ExecLimitSearch(AxisNo, MinPosLimit, MaxPosLimit) == false )
	{
		if ( MotionParam.m_UsingMotionSoftwareLimit == FN_ENABLE )
		{	
			MotionAxisPtr->SetLimitMax(MaxPosLimit);
			MotionAxisPtr->SetLimitMin(MinPosLimit);
			switch ( AxisNo )
			{
			case AXIS_X:
				MotionParam.m_LimitMaxX = MaxPosLimit;
				MotionParam.m_LimitMinX = MinPosLimit;
				break;
			case AXIS_Y:
				MotionParam.m_LimitMaxY = MaxPosLimit;
				MotionParam.m_LimitMinY = MinPosLimit;
				break;
			case AXIS_Z:
				MotionParam.m_LimitMaxZ = MaxPosLimit;
				MotionParam.m_LimitMinZ = MinPosLimit;
				break;
			}	
			this->EnableSoftwareLimit(AxisNo); 
		}
		return false;
	}
	if ( this->MoveTo(AxisNo, 0, MOTION_MOVING_NORMAL, false) == false )
	{	return false;  }
	if ( this->WaitForDone(AxisNo) == false )
	{	return false;  }

	MotionAxisPtr->SetLimitMax(MaxPosLimit);
	MotionAxisPtr->SetLimitMin(MinPosLimit);
	switch ( AxisNo )
	{
	case AXIS_X:
		MotionParam.m_LimitMaxX = MaxPosLimit;
		MotionParam.m_LimitMinX = MinPosLimit;
		break;
	case AXIS_Y:
		MotionParam.m_LimitMaxY = MaxPosLimit;
		MotionParam.m_LimitMinY = MinPosLimit;
		break;
	case AXIS_Z:
		MotionParam.m_LimitMaxZ = MaxPosLimit;
		MotionParam.m_LimitMinZ = MinPosLimit;
		break;
	}	

	if ( MotionParam.m_UsingMotionSoftwareLimit == FN_ENABLE )
	{	
		this->SetSoftwareLimit(AxisNo, MinPosLimit, MaxPosLimit, false);
		this->EnableSoftwareLimit(AxisNo); 
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::ExecLimitSearch(int AxisNo, double &MinLimit, double &MaxLimit)//確認極限範圍並且設定成軟體極限
{
#ifndef MOTION_OBJ_DISABLE
	CString str;	
	//2. 往最大座標移動
	//3. 往最小座標移動	
	double MaxPosLimit = 0;
	double MinPosLimit = 0;	
	double PosLimitOffset = 0;
	const double MaxPos =  10000000;//10000 mm
	const double MinPos = -10000000;//10000 mm
	const double StrVel = 0;
	const double MaxVel = 100000;
	const double Tacc = 0.5;
	const double Tdec = 0.5;
	const DWORD SleepTime = 500;//500 ms
	TMotionParameter &MotionParam=GetMotionParameter();
	CMotionAxis *MotionAxisPtr=GetMotionAxisPtr(AxisNo);
	if ( CheckMotionAxisPtr(MotionAxisPtr) == false ) { return false; }
	MotionAxisPtr->SetLimitMin(MinPos-PosLimitOffset);
	MotionAxisPtr->SetLimitMax(MaxPos+PosLimitOffset);
	switch ( AxisNo )
	{
	case AXIS_X:
		MotionParam.m_LimitMaxX = MaxPos + PosLimitOffset;
		MotionParam.m_LimitMinX = MinPos - PosLimitOffset ;
		PosLimitOffset = MotionParam.m_LimitMarginX;
		break;
	case AXIS_Y:
		MotionParam.m_LimitMaxY = MaxPos + PosLimitOffset;
		MotionParam.m_LimitMinY = MinPos - PosLimitOffset ;
		PosLimitOffset = MotionParam.m_LimitMarginY;
		break;
	case AXIS_Z:
		MotionParam.m_LimitMaxZ = MaxPos + PosLimitOffset;
		MotionParam.m_LimitMinZ = MinPos - PosLimitOffset ;
		PosLimitOffset = MotionParam.m_LimitMarginZ;
		break;
	}	
	if ( SetSoftwareLimit(AxisNo, MinPos-1000, MaxPos+1000, false) == false )
	{	return false; }
	if ( this->MoveTo(AxisNo, MaxPos, MOTION_MOVING_HOME, false) == false )
	{	return false;	}
	if ( this->WaitForDone(AxisNo) == false )
	{	return false;	}
	if ( SleepTime > 0 ) 
	{	::Sleep(SleepTime); }
	if ( this->GetEncode(AxisNo, MaxPosLimit) == false )
	{	return false;  	}
	MaxLimit = MaxPosLimit-PosLimitOffset;	
	if ( this->MoveTo(AxisNo, MinPos, MOTION_MOVING_HOME, false) == false )
	{	return false;	}
	if ( this->WaitForDone(AxisNo) == false )
	{	return false;  	}
	if ( SleepTime > 0 ) 
	{	::Sleep(SleepTime); }
	if ( this->GetEncode(AxisNo, MinPosLimit) == false )
	{	return false; 	}
	MinLimit = MinPosLimit+PosLimitOffset;		
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::ExecJogMSG(UINT message, WPARAM wParam, LPARAM lParam)//執行Jog訊息
{
#ifndef MOTION_OBJ_DISABLE
	if ( CMotion_Basic::GetIsJogMode() == false ) { return false; }
	if ( AOIDataCollect.GetIsLockUIWnd() == true ) { return false; }
	double PosX=0, PosY=0, PosZ=0;
	switch ( message )
	{
	case WM_KEYDOWN:
		switch ( wParam )
		{
		case VK_UP:		
			this->StartFreeRun(AXIS_Y, true);	
			return true;
			break;
		case VK_DOWN:	
			this->StartFreeRun(AXIS_Y, false);	
			return true;
			break;
		case VK_LEFT:	
			this->StartFreeRun(AXIS_X, false);	
			return true;
			break;
		case VK_RIGHT:	
			this->StartFreeRun(AXIS_X, true);	
			return true;
			break;
		case VK_ADD:
			this->StartFreeRun(AXIS_Z, true);	
			return true;
			break;
		case VK_SUBTRACT:
			this->StartFreeRun(AXIS_Z, false);	
			return true;
			break;
		}
		break;
	case WM_KEYUP:
		switch ( wParam )
		{
		case VK_UP:
		case VK_DOWN:			
		case VK_LEFT:			
		case VK_RIGHT:
		case VK_ADD:
		case VK_SUBTRACT:
			this->StopFreeRun(AXIS_X);	
			this->StopFreeRun(AXIS_Y);			
			this->StopFreeRun(AXIS_Z);
			this->m_CommandOffline = false;			
			this->GetCurrentPos(PosX, PosY, PosZ);
			this->SetCommandPos(PosX, PosY, PosZ);			
			AOIDataCollect.PostCallbackWndMessage(MSG_CAMERA_REGRAB_IMAGE, TRUE, NULL);			
			return true;
		}
		break;
	}
#endif//MOTION_OBJ_DISABLE
	return false;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::ExecHomeAll(bool MoveToStartPos)//執行全部歸零
{
#ifndef MOTION_OBJ_DISABLE
	std::vector<int> AxisList;
	const bool bHomeZFirst=true;	
	double PosX = MotionCtrlPtr->GetMotionParameter().m_StageStartPosX;
	double PosY = MotionCtrlPtr->GetMotionParameter().m_StageStartPosY;
	double PosZ = MotionCtrlPtr->GetMotionParameter().m_StageStartPosZ;
	if ( true == bHomeZFirst )
	{	
		AxisList.push_back(AXIS_Z);
		AxisList.push_back(AXIS_Y);
		AxisList.push_back(AXIS_X);
	}
	else
	{
		AxisList.push_back(AXIS_Y);
		AxisList.push_back(AXIS_X);
		AxisList.push_back(AXIS_Z);
	}
	const size_t AxisCount=AxisList.size();
	for ( size_t i=0; i<AxisCount; i++ )
	{	DoFaultAck(AxisList[i]);	}	
	//::Sleep(1000);//2000
	for ( size_t i=0; i<AxisCount; i++ )
	{	
		if ( Enable(AxisList[i]) == false )
		{	return false; }
	}	
	::Sleep(1000);//2000
	for ( size_t i=0; i<AxisCount; i++ )
	{	
		const int Axis=AxisList[i];
		if ( Home(Axis) == false )
		{	return false; }
		if ( true == MoveToStartPos )
		{			
			double Pos=0;
			switch ( Axis )
			{
			case AXIS_X: Pos=PosX;	break;
			case AXIS_Y: Pos=PosY;	break;
			case AXIS_Z: Pos=PosZ;	break;
			}			
			if ( MoveTo(Axis, Pos, MOTION_MOVING_NORMAL) == false )
			{	return false;	}
			if ( WaitForDone(Axis) == false )
			{	return false;	}
		}
	}	
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CheckMotionReady(bool bChkEnb)//確認運動系統準備好
{
#ifndef MOTION_OBJ_DISABLE		
	if ( ExecCheckMotionReady(bChkEnb) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_NOT_READY);
		return false;
	}
#endif//MOTION_OBJ_DISABLE		
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::ExecCheckMotionReady(bool bChkEnb)//確認運動系統準備好	
{
#ifndef MOTION_OBJ_DISABLE
	CString str;
	if ( MotionCtrlPtr->CheckInit() == false ) 
	{	return false; }

	if ( true == bChkEnb )
	{
		if ( MotionCtrlPtr->CheckIsEnabled(AXIS_X) == false )
		{	return false;	}
		if ( MotionCtrlPtr->CheckIsEnabled(AXIS_Y) == false )
		{	return false;	}
		if ( MotionCtrlPtr->CheckIsEnabled(AXIS_Z) == false )
		{	return false;	}

		if ( MotionCtrlPtr->CheckIsHomed(AXIS_X) == false )
		{	return false;	}
		if ( MotionCtrlPtr->CheckIsHomed(AXIS_Y) == false )
		{	return false;	}
		if ( MotionCtrlPtr->CheckIsHomed(AXIS_Z) == false )
		{	return false;	}
	}

	if ( MotionCtrlPtr->GetIsAlarm(AXIS_X) == true )
	{	
		str = _T("Error, X axis is alarm"); 
		m_ErrorString = CMotion_Basic::LoadMultiLanguageString(str, str);
		return false;
	}
	if ( MotionCtrlPtr->GetIsAlarm(AXIS_Y) == true )
	{	
		str = _T("Error, Y axis is alarm"); 
		m_ErrorString = CMotion_Basic::LoadMultiLanguageString(str, str);
		return false;
	}
	if ( MotionCtrlPtr->GetIsAlarm(AXIS_Z) == true )
	{	
		str = _T("Error, Z axis is alarm"); 
		m_ErrorString = CMotion_Basic::LoadMultiLanguageString(str, str);
		return false;
	}
#endif//MOTION_OBJ_DISABLE
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CreateMotionCtrlThread()
{
#ifndef OFFLINE_VERSION
	AOIDataCollect.SaveCurrentProcess(_T("bool CMotion_Basic::CreateMotionCtrlThread-Start"));	
	this->m_MotionCtrlThreadDeleted = false;	
	//-----------------------------------走停取像執行緒------------------------------------------------//	
	CMotion_Basic::m_MotionThreadCmd_GoStop = THREAD_COMMAND_TO_IDLE;
	MotionCtrlThreadHandle_GoStop = (HANDLE)::_beginthreadex(NULL, NULL, &MotionCtrlThreadFn_GoStop, NULL, NULL, &MotionCtrlThreadID_GoStop);	
	if ( NULL == MotionCtrlThreadHandle_GoStop )
	{
		CMotion_Basic::m_MotionCtrlThreadDeleted = true;		
		CMotion_Basic::DeleteMotionCtrlThread();
		CMotion_Basic::m_MotionThreadState_GoStop = THREAD_STATE_NONE;
		this->m_ErrorString.Format(_T("Error, Create Thread Fault (MotionCtrlThreadHandle_GoStop == NULL)"));
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_CREATE, m_ErrorString);
		return false;
	}	
	MotionCtrlThreadEvent_GoStop = JetAPI::CreateEvent(NULL, TRUE, TRUE, _T("Motion Thread - GoStop"));	
	::SetThreadPriority(MotionCtrlThreadHandle_GoStop, THREAD_PRIORITY_NORMAL);//THREAD_PRIORITY_BELOW_NORMAL
	//::SetThreadAffinityMask(MotionCtrlThreadHandle_GoStop, 0x0F);//1111
	//-----------------------------------走停取像執行緒------------------------------------------------//
	AOIDataCollect.SaveCurrentProcess(_T("bool CMotion_Basic::CreateMotionCtrlThread-End"));
#endif//OFFLINE_VERSION
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::DeleteMotionCtrlThread()
{
	DWORD Res = 0;
	this->m_MotionCtrlThreadDeleted = true;		
#ifndef OFFLINE_VERSION
	CMotion_Basic::SetMotionThreadCmd_GoStop(THREAD_COMMAND_TO_EXIT);
	//-----------------------------------走停取像執行緒------------------------------------------------//
	if ( NULL != MotionCtrlThreadHandle_GoStop )
	{
		Res = ::WaitForSingleObject(MotionCtrlThreadHandle_GoStop, INFINITE);
		::CloseHandle(MotionCtrlThreadHandle_GoStop); MotionCtrlThreadHandle_GoStop = NULL;
	}

	if ( NULL != MotionCtrlThreadEvent_GoStop )
	{	::CloseHandle(MotionCtrlThreadEvent_GoStop); MotionCtrlThreadEvent_GoStop=NULL; }	
	//-----------------------------------走停取像執行緒------------------------------------------------//
#endif//OFFLINE_VERSION
	TRACE(_T("CMotion_Basic::~DeleteMotionCtrlThread\n"));	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::GetMotionCtrlThreadDeleted() const
{
	return m_MotionCtrlThreadDeleted;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::StartMotionThreadStats_GoStop(bool WaitOn)//開始走停取像執行緒
{
	AOIDataCollect.SetIsEnableAutoRetry(false);
	m_IsMotionStop = false;
	if ( NULL != MotionCtrlThreadEvent_GoStop )
	{	::ResetEvent(MotionCtrlThreadEvent_GoStop); }
	if ( THREAD_STATE_FINISH == m_MotionThreadState_GoStop )
	{	SetMotionThreadState_GoStop(THREAD_STATE_IDLE); }	
	SetMotionThreadCmd_GoStop(THREAD_COMMAND_TO_RUN);

	if ( WaitOn == true ) 
	{
		int i=0;
		const int MaxCount = 200;
		const int SleepTime = 10;
		for ( i=0; i<MaxCount; i++ )
		{
			if ( THREAD_COMMAND_TO_RUN != m_MotionThreadCmd_GoStop )//切入下一個階段
			{	break; }
			if ( THREAD_STATE_IDLE != m_MotionThreadState_GoStop )
			{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			this->m_ErrorString.Format(_T("Error, wait for StartMotionGoStopPathThreadStats_Grab too long"));		
			AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_START, m_ErrorString);
			return false;
		}
	}
	return true;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionThreadState_GoStop(THREAD_STATE_MODE State)//設定走停取像執行緒狀態
{
	if ( CMotion_Basic::m_MotionThreadState_GoStop == State ) { return; }	
	LockMotion();
	CMotion_Basic::m_MotionThreadState_GoStop = State;
	UnlockMotion();
}
//----------------------------------------------------------------------------------//
THREAD_STATE_MODE CMotion_Basic::GetMotionThreadState_GoStop() const//取得走停取像執行緒狀態
{
	return CMotion_Basic::m_MotionThreadState_GoStop;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionThreadCmd_GoStop(THREAD_COMMAND_MODE Cmd)//設定走停取像執行緒命令
{
	if ( CMotion_Basic::m_MotionThreadCmd_GoStop == Cmd ) { return; }
	LockMotion();
	CMotion_Basic::m_MotionThreadCmd_GoStop = Cmd;
	UnlockMotion();
}
//----------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CMotion_Basic::GetMotionThreadCmd_GoStop() const//取得走停取像執行緒命令
{
	return CMotion_Basic::m_MotionThreadCmd_GoStop;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::WaitMotionThreadDone_GoStop(size_t MoveCount)//等待走取像停執行緒
{		
	size_t i=0;	
	const size_t MaxI = 100*MoveCount+100;
	const DWORD SleepTime = 100;	
	DWORD Res = 0;
	if ( NULL == MotionCtrlThreadEvent_GoStop )
	{ 
		this->m_ErrorString.Format(_T("Error, NULL == MotionCtrlThreadEvent_GoStop"));
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_EXEC_FUNC);
		return false; 
	}
	for ( i=0; i<MaxI; i++ )
	{
		if ( AOIDataCollect.GetIsSystemReleased() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == CMotion_Basic::m_MotionThreadCmd_GoStop ) { return true; }		
		if ( THREAD_COMMAND_TO_IDLE == CMotion_Basic::m_MotionThreadCmd_GoStop )
		{	break; }
		Res = ::WaitForSingleObject(MotionCtrlThreadEvent_GoStop, SleepTime);
		if ( Res == WAIT_OBJECT_0 ) { break; }
	}
	if ( i == MaxI ) 
	{
		this->m_ErrorString.Format(_T("Error, WaitMotionThreadDone_GoStop too long"));
		AOIExceptionCodeCtrl.SetAOIExceptionCode_Thread(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH, m_ErrorString);			
		return false; 
	}
	const THREAD_STATE_MODE ThreadStats = CMotion_Basic::GetMotionThreadState_GoStop();
	if ( ThreadStats == THREAD_STATE_EXCEPTION )
	{	return false; }	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::ExecMotionThreadFn_GoStop()//執行走停取像執行緒
{
#ifndef MOTION_OBJ_DISABLE		
	if ( ExecMotionThreadFn_GoStopFn() == false )
	{
		if ( AOIExceptionCodeCtrl.CheckLastExceptionCodeOK() == true )
		{	SetMotionExceptionCode(AOI_EXCEPTION_MOTION_EXEC_FUNC);	}
		return false;
	}
#endif//MOTION_OBJ_DISABLE		
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::ExecMotionThreadFn_GoStopFn()//執行走停取像執行緒
{
	CString         str, str2;
	CString         MovingTimeS;
	CString         strErrString;
	LARGE_INTEGER   nEndTime;
	LARGE_INTEGER   nStartTime;	

//	LARGE_INTEGER   fnEndTime;
//	LARGE_INTEGER   fnStartTime;


//	LARGE_INTEGER   nCameraEndTime;	
//	LARGE_INTEGER   nCameraStartTime;

	double          dMotionTime=0;	
	LARGE_INTEGER   nMotionEndTime;	
	LARGE_INTEGER   nMotionStartTime;
	QueryPerformanceCounter(&nStartTime);	
	
	size_t       i = 0;			
	IMAGE_PTR    ImagePtr = NULL;	
	IMAGE_SIZE   ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	
	int          PeriodTime_us = 0;
	int          ExposureTime_us = 0;
	TPOINT3D     StagePos;
	CAMERA_ID    CameraID;
	CAOIFov     *FovPtr = NULL;		
	bool         bCameraFinish = false;		
	bool         bCameraFinish2 = false;		
	double       Temperature=0.0;
	long         cntBatchGrab = 0;
	long         cntCameraBack=0, cntExtBak=0, cntImageBak=0, cntImageCpy=0;
	unsigned int CastIndex=0;	
	const bool bAutoReset = false;	
	const bool bChkStartLight = false;	
	LIGHT_3D_CLS_PTR   Light3DPtr = NULL;
	LIGHT_3D_CAST_ID   Light3DID = LIGHT_3D_CAST_00;		
	THREAD_COMMAND_MODE  ThreadCmd = THREAD_COMMAND_TO_NONE;
	
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const size_t FovCount = AOIDataCollect.GetFovPtrCount();	
	int          ExpTimeus = AOIDataCollect.GetCalibrationParameter().m_ExposureTime_us;		

	std::vector<TSliceParam>  SliceParamList, SliceParamListBefore;
	MOTION_MOVING_MODE MotionMovingMode=MOTION_MOVING_GO_STOP;
	IMAGE_DISPLAY_MODE ImageMode = SystemParam.m_ImageDisplayMode;
	THREAD_GRAB_MODE   ThreadGrabMode = AOIDataCollect.GetThreadGrabMode();	

	switch ( ThreadGrabMode )
	{
	case THREAD_GRAB_PROJECT_MAP: 
		str.Format(_T("[ProjectMap]"));
		MotionMovingMode=MOTION_MOVING_GRAB_MAP;
		break;
	case THREAD_GRAB_PROJECT_MARK:		str.Format(_T("[Project Mark]"));	break;
	case THREAD_GRAB_PANEL_FD: 			str.Format(_T("[Panel Fd]"));		break;
	case THREAD_GRAB_BOARD_FD: 			str.Format(_T("[Board Fd]"));		break;
	case THREAD_GRAB_BARCODE:			str.Format(_T("[Barcode]"));		break;
	case THREAD_GRAB_PROJECT_TEST: 		str.Format(_T("[Project Test]"));	break;
	case THREAD_GRAB_PROJECT_OPEN_CODE:	str.Format(_T("[Project Open Barcode]")); break;
	}	
	AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

	CString strDebug;
	CString strFolder;
	bool bFirstFov=true;
	const bool bAutoRetry=false;
	const bool bAutoRetry2=false;
	const bool bSaveCameraImage=false;
	strFolder = AOIDataCollect.GetAOITempDirectory();
	::CreateDirectory(strFolder, NULL);
	::Sleep(0);
	strFolder.Format(_T("%s\\Dump"), AOIDataCollect.GetAOITempDirectory());
	::CreateDirectory(strFolder, NULL);
	::Sleep(0);
	JetAPI::ClearFolder(strFolder);

	if ( FovCount > 0 ) 
	{
		if ( CMotion_Basic::WaitForMotionStop(false) == false ) 
		{	return false; }
	}
	
	for ( i=0; i<FovCount; i++ )
	{
		ThreadCmd = GetMotionThreadCmd_GoStop();
		if ( AOIDataCollect.GetIsSystemReleased() == true ) { return true; }
		if ( AOIDataCollect.GetIsSystemException() == true ) { return true; }		
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }

		if ( true == bAutoRetry )
		{
			if ( FovCount > 2 )
			{
				if ( i == (FovCount-2) )
				{
					SetErrorString(_T("Test-AutoRetry"));
					AOIDataCollect.SetIsEnableAutoRetry(true);
					return false;
				}
			}
		}

		if ( AOIDataCollect.CheckSystemReady(bChkStartLight, bAutoReset) == false )		
		{
			SetErrorString(AOIDataCollect.GetErrorString());
			return false; 
		}

		FovPtr = AOIDataCollect.GetFovPtr(i, false);
		if ( NULL == FovPtr ) { continue; }
		StagePos.x = FovPtr->GetFovStagePosX();
		StagePos.y = FovPtr->GetFovStagePosY();
		StagePos.z = FovPtr->GetFovStagePosZ();
		QueryPerformanceCounter(&nMotionStartTime);				
		if ( XYZMoveTo(StagePos.x, StagePos.y, StagePos.z, false, MotionMovingMode) == false )
		{	return false; }
		if ( WaitForMotionStop() == false )
		{	return false; }		
		QueryPerformanceCounter(&nMotionEndTime);			
		dMotionTime = (nMotionEndTime.QuadPart - nMotionStartTime.QuadPart)*1000.0/AOIDataCollect.m_SystemFreq.QuadPart;

		str.Format(_T("Move to FOV #%d (%.0f, %.0f, %.0f), Time=%.3f ms"), i+1, StagePos.x, StagePos.y, StagePos.z, dMotionTime);
		AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
		PostMotionCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_PROJECT_MAP, TRUE);

		CameraID       = PRIMARY_CAMERA_ID;		
		FovPtr->CloneFovSliceParamList(SliceParamList);
		if ( true == bFirstFov )
		{	
			bFirstFov = false;	
			SliceParamListBefore = CameraCtrl.GetBatchParamList();			
		}
		if ( AOIDataCollect.CompareSliceParamList(SliceParamList, SliceParamListBefore) == false )
		{	
			if ( CameraCtrl.BatchGrabPrepare2(SliceParamList, false) == false )
			{
				this->m_ErrorString = CameraCtrl.GetErrorString();
				return false;
			}
		}		
		SliceParamListBefore = SliceParamList;

		TRACE(_T("Grab Next Fov\n"));
		
	#ifndef LIGHT_CTRL_DISABLE
		if ( CameraCtrl.ClearCameraCount(CameraID) == false )
		{
			this->m_ErrorString = CameraCtrl.GetErrorString();			
			return false;
		}
		if ( CameraCtrl.BatchIndexReset() == false )
		{
			this->m_ErrorString = CameraCtrl.GetErrorString();			
			return false;
		}		
	#else
		if ( CameraCtrl.BatchGrabPrepare2(SliceParamList, true) == false )
		{
			this->m_ErrorString = CameraCtrl.GetErrorString();			
			return false;
		}
	#endif//LIGHT_CTRL_DISABLE

		CastIndex=0;
		bCameraFinish = false;
		int WaitRingBufferStateCount = 0;
		const int MaxWaitRingBufferStateCount = 200;
		while ( true ) 
		{
			if ( CameraCtrl.CheckCameraRingBufferStateDone(CameraID) == true )
			{	break;	}
			WaitRingBufferStateCount ++;
			if ( WaitRingBufferStateCount > MaxWaitRingBufferStateCount )//多等一些時間
			{	
				str.Format(_T("%s"), _T("Wait For Camera Ring Buffer Done Fault"));
				SetErrorString(str);
				return false; 
			}
			::Sleep(10);
		};

		while ( true )
		{
			ThreadCmd = GetMotionThreadCmd_GoStop();
			if ( AOIDataCollect.GetIsSystemException() == true ) { return true; }		
			if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
			if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }

			//後續需要加上對RingBuffer的確認, 比較有防呆的機置			
			if ( CameraCtrl.BatchGrabStart2(bCameraFinish) == false )
			{
				//this->m_ErrorString = CameraCtrl.GetErrorString();
				str = CameraCtrl.GetErrorString();
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				Temperature = CameraCtrl.ReadCameraTemperature(CameraID);
				cntBatchGrab = CameraCtrl.GetCameraBatchGrabCount(CameraID);
				CameraCtrl.GetCameraCount(CameraID, cntCameraBack, cntExtBak, cntImageBak, cntImageCpy);				
				strErrString.Format(_T("%s \nCount--Need:%d, Cast:%d, Camera:%d, ExpTime:%d, Image:%d, Copy:%d, Temperature=%.2f"), str, cntBatchGrab, CastIndex+1, cntCameraBack, cntExtBak, cntImageBak, cntImageCpy, Temperature);
				SetErrorString(strErrString);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strErrString);

				long  ii=0;				
				long  RingStart = 0;
				long  RingEnd = 0;				
				const long RingIndex = (long)(CameraCtrl.GetCameraRingBufferCurrentIndex(CameraID));
				const long RingListSize  = (long)(CameraCtrl.GetCameraRingBufferListSize(CameraID));				
				BitCount = 8;
				RingStart = RingIndex-cntCameraBack;
				if ( RingStart < 0 ) { RingStart += RingListSize; }
				RingEnd = RingStart+cntCameraBack;
				str = _T("Save Ring Buffer Images");
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				for ( ii=RingStart; ii<RingEnd; ii++ )
				{					
					if ( false == bSaveCameraImage ) { continue; }
					str.Format(_T("Start Retrive Ring Buffer Image #%d"), ii+1);
					AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
					if ( CameraCtrl.GetCameraRingBufferImage(CameraID, ii, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
					{
						str.Format(_T("Fail To Retrive Ring Buffer Image #%d"), ii+1);
						AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
						continue; 
					}
					str.Format(_T("End Retrive Ring Buffer Image #%d"), ii+1);
					AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

					strDebug.Format(_T("%s\\DropImage#%d.PNG"), strFolder, ii+1);
					str.Format(_T("Save Ring Buffer Image #%d [%s]"), ii+1, strDebug);
					AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

					ImageAPI.SaveImage(strDebug, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
					
					ImagePtr = NULL;
					ImageW = ImageH = ImageStep = 0;
				}
				str = _T("End Motion Thread Fn");
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				return false;
			}	
			bCameraFinish2 = CameraCtrl.WaitForCameraGrabFinish(CameraID);			
			if ( true == bAutoRetry2 )
			{
				if ( FovCount > 2 )
				{
					if ( i == (FovCount-2) )
					{
						SetErrorString(_T("Test-AutoRetry"));
						bCameraFinish2 = false;
					}
				}
			}
		#ifdef AOI_EXCEPTION_CODE_USE
			//if ( THREAD_GRAB_PROJECT_TEST == ThreadGrabMode )
			//{
			//	if ( i == (FovCount/2) )
			//	{	bCameraFinish2 = false; }
			//}
		#endif//AOI_EXCEPTION_CODE_USE
			
			if ( false == bCameraFinish2 ) 
			{	
				//this->m_ErrorString = CameraCtrl.GetErrorString();			
				str = CameraCtrl.GetErrorString();
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				Temperature = CameraCtrl.ReadCameraTemperature(CameraID);
				cntBatchGrab = CameraCtrl.GetCameraBatchGrabCount(CameraID);
				CameraCtrl.GetCameraCount(CameraID, cntCameraBack, cntExtBak, cntImageBak, cntImageCpy);
				//this->m_ErrorString.Format(_T("%s \nCount:Cast:%d, Camera:%d, ExpTime:%d, Image:%d, Copy:%d"), str, CastIndex+1, cntCameraBack, cntExtBak, cntImageBak, cntImageCpy);
				strErrString.Format(_T("%s \nCount--Need:%d, Cast:%d, Camera:%d, ExpTime:%d, Image:%d, Copy:%d, Temperature=%.2f"), str, cntBatchGrab, CastIndex+1, cntCameraBack, cntExtBak, cntImageBak, cntImageCpy, Temperature);
				SetErrorString(strErrString);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strErrString);
				AOIDataCollect.SetSystemExceptionCode(AOI_EXCEPTION_SYSTEM_GRAB_IMAGE_CNT, strErrString); 

			#ifndef LIGHT_CTRL_DISABLE
				bool bLightCtrlBoardError=false;
				str = _T("Retrieve Light Control Board Message");
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				LightCtrlBoard.CheckLightCtrlBoardError(bLightCtrlBoardError, str2);
				str = strErrString;
				strErrString.Format(_T("%s\n%s"), str, str2);				
				SetErrorString(strErrString);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str2);
				if ( false==bLightCtrlBoardError && cntBatchGrab!=cntCameraBack )
				{
					LightCtrlBoard.GetTriggerCountText(str2);
					str = strErrString;
					strErrString.Format(_T("%s\n%s"), str, str2);
					SetErrorString(strErrString);
					AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str2);
				}
				str = _T("Retrieve Light Control Board Message End");
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
			#endif//LIGHT_CTRL_DISABLE

				long  ii=0;				
				long  RingStart = 0;
				long  RingEnd = 0;				
				const long RingIndex = (long)(CameraCtrl.GetCameraRingBufferCurrentIndex(CameraID));
				const long RingListSize  = (long)(CameraCtrl.GetCameraRingBufferListSize(CameraID));				
				BitCount = 8;
				RingStart = RingIndex-cntCameraBack;
				if ( RingStart < 0 ) { RingStart += RingListSize; }
				RingEnd = RingStart+cntCameraBack;
				str = _T("Save Ring Buffer Images");
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				for ( ii=RingStart; ii<RingEnd; ii++ )
				{					
					if ( false == bSaveCameraImage ) { continue; }
					str.Format(_T("Start Retrive Ring Buffer Image #%d"), ii+1);
					AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
					if ( CameraCtrl.GetCameraRingBufferImage(CameraID, ii, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
					{	
						str.Format(_T("Fail To Retrive Ring Buffer Image #%d"), ii+1);
						AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
						continue; 
					}
					str.Format(_T("End Retrive Ring Buffer Image #%d"), ii+1);
					AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

					strDebug.Format(_T("%s\\DropImage#%d.PNG"), strFolder, ii+1);
					str.Format(_T("Save Ring Buffer Image #%d [%s]"), ii+1, strDebug);
					AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

					ImageAPI.SaveImage(strDebug, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
					
					ImagePtr = NULL;
					ImageW = ImageH = ImageStep = 0;
				}
				str = _T("End Motion Thread Fn");
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				AOIDataCollect.SetIsEnableAutoRetry(true);
				return false;	
			}
			else
			{
				/*
				Temperature = CameraCtrl.ReadCameraTemperature(CameraID);
				CameraCtrl.GetCameraCount(CameraID, cntCameraBack, cntExtBak, cntImageBak, cntImageCpy);
				long  ii=0;				
				long  RingStart = 0;
				long  RingEnd = 0;				
				const long RingIndex = (long)(CameraCtrl.GetCameraRingBufferCurrentIndex(CameraID));
				const long RingListSize  = (long)(CameraCtrl.GetCameraRingBufferListSize(CameraID));				
				BitCount = 8;
				RingStart = RingIndex-cntCameraBack;
				if ( RingStart < 0 ) { RingStart += RingListSize; }
				RingEnd = RingStart+cntCameraBack;
				str = _T("Save Ring Buffer Images");
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				for ( ii=RingStart; ii<RingEnd; ii++ )
				{	
					if ( false == bSaveCameraImage ) { continue; }
					if ( CameraCtrl.GetCameraRingBufferImage(CameraID, ii, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
					{	
						str.Format(_T("Fail To Retrive Ring Buffer Image #%d"), ii+1);
						AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
						continue; 
					}					
					strDebug.Format(_T("%s\\DropImage#%d.PNG"), strFolder, ii+1);
					str.Format(_T("Save Ring Buffer Image #%d [%s]"), ii+1, strDebug);
					AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
					if ( ImageAPI.SaveImage(strDebug, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true) == false )
					{
						str.Format(_T("Fail to Save Ring Buffer Image #%d [%s]"), ii+1, strDebug);
						AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);						
					}					
					ImagePtr = NULL;
					ImageW = ImageH = ImageStep = 0;
				}
				*/
			}
			
			//第2次打光
			if ( CameraCtrl.BatchGrabNext3DImage(bCameraFinish) == false )
			{
				//this->m_ErrorString = CameraCtrl.GetErrorString();
				str = CameraCtrl.GetErrorString();
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				Temperature = CameraCtrl.ReadCameraTemperature(CameraID);
				cntBatchGrab = CameraCtrl.GetCameraBatchGrabCount(CameraID);
				CameraCtrl.GetCameraCount(CameraID, cntCameraBack, cntExtBak, cntImageBak, cntImageCpy);				
				strErrString.Format(_T("%s \nCount--Need:%d, Cast:%d, Camera:%d, ExpTime:%d, Image:%d, Copy:%d, Temperature=%.2f"), str, cntBatchGrab, CastIndex+1, cntCameraBack, cntExtBak, cntImageBak, cntImageCpy, Temperature);
				SetErrorString(strErrString);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strErrString);

				long  ii=0;				
				long  RingStart = 0;
				long  RingEnd = 0;				
				const long RingIndex = (long)(CameraCtrl.GetCameraRingBufferCurrentIndex(CameraID));
				const long RingListSize  = (long)(CameraCtrl.GetCameraRingBufferListSize(CameraID));				
				BitCount = 8;
				RingStart = RingIndex-cntCameraBack;
				if ( RingStart < 0 ) { RingStart += RingListSize; }
				RingEnd = RingStart+cntCameraBack;
				str = _T("Save Ring Buffer Images");
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				for ( ii=RingStart; ii<RingEnd; ii++ )
				{					
					if ( false == bSaveCameraImage ) { continue; }
					str.Format(_T("Start Retrive Ring Buffer Image #%d"), ii+1);
					AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
					if ( CameraCtrl.GetCameraRingBufferImage(CameraID, ii, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
					{
						str.Format(_T("Fail To Retrive Ring Buffer Image #%d"), ii+1);
						AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
						continue; 
					}
					str.Format(_T("End Retrive Ring Buffer Image #%d"), ii+1);
					AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

					strDebug.Format(_T("%s\\DropImage#%d.PNG"), strFolder, ii+1);
					str.Format(_T("Save Ring Buffer Image #%d [%s]"), ii+1, strDebug);
					AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

					ImageAPI.SaveImage(strDebug, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
					
					ImagePtr = NULL;
					ImageW = ImageH = ImageStep = 0;
				}
				str = _T("End Motion Thread Fn");
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				return false;
			}	
			bCameraFinish2 = CameraCtrl.WaitForCameraGrabFinish(CameraID);			
			if ( true == bAutoRetry2 )
			{
				if ( FovCount > 2 )
				{
					if ( i == (FovCount-2) )
					{
						SetErrorString(_T("Test-AutoRetry"));
						bCameraFinish2 = false;
					}
				}
			}
			if ( false == bCameraFinish2 ) 
			{	
				//this->m_ErrorString = CameraCtrl.GetErrorString();			
				str = CameraCtrl.GetErrorString();
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				Temperature = CameraCtrl.ReadCameraTemperature(CameraID);
				cntBatchGrab = CameraCtrl.GetCameraBatchGrabCount(CameraID);				
				CameraCtrl.GetCameraCount(CameraID, cntCameraBack, cntExtBak, cntImageBak, cntImageCpy);
				//this->m_ErrorString.Format(_T("%s \nCount:Cast:%d, Camera:%d, ExpTime:%d, Image:%d, Copy:%d"), str, CastIndex+1, cntCameraBack, cntExtBak, cntImageBak, cntImageCpy);
				strErrString.Format(_T("%s \nCount--Need:%d, Cast:%d, Camera:%d, ExpTime:%d, Image:%d, Copy:%d, Temperature=%.2f"), str, cntBatchGrab, CastIndex+1, cntCameraBack, cntExtBak, cntImageBak, cntImageCpy, Temperature);
				SetErrorString(strErrString);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strErrString);
				AOIDataCollect.SetSystemExceptionCode(AOI_EXCEPTION_SYSTEM_GRAB_IMAGE_CNT, strErrString); 

			#ifndef LIGHT_CTRL_DISABLE
				bool bLightCtrlBoardError=false;
				str = _T("Retrieve Light Control Board Message");
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				LightCtrlBoard.CheckLightCtrlBoardError(bLightCtrlBoardError, str2);
				str = strErrString;
				strErrString.Format(_T("%s\n%s"), str, str2);
				SetErrorString(strErrString);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str2);
				if ( false==bLightCtrlBoardError && cntBatchGrab!=cntCameraBack )
				{
					LightCtrlBoard.GetTriggerCountText(str2);
					str = strErrString;
					strErrString.Format(_T("%s\n%s"), str, str2);
					SetErrorString(strErrString);
					AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str2);
				}
				str = _T("Retrieve Light Control Board Message End");
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
			#endif//LIGHT_CTRL_DISABLE

				long  ii=0;				
				long  RingStart = 0;
				long  RingEnd = 0;				
				const long RingIndex = (long)(CameraCtrl.GetCameraRingBufferCurrentIndex(CameraID));
				const long RingListSize  = (long)(CameraCtrl.GetCameraRingBufferListSize(CameraID));				
				BitCount = 8;
				RingStart = RingIndex-cntCameraBack;
				if ( RingStart < 0 ) { RingStart += RingListSize; }
				RingEnd = RingStart+cntCameraBack;
				str = _T("Save Ring Buffer Images");
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				for ( ii=RingStart; ii<RingEnd; ii++ )
				{					
					if ( false == bSaveCameraImage ) { continue; }
					str.Format(_T("Start Retrive Ring Buffer Image #%d"), ii+1);
					AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
					if ( CameraCtrl.GetCameraRingBufferImage(CameraID, ii, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
					{	
						str.Format(_T("Fail To Retrive Ring Buffer Image #%d"), ii+1);
						AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
						continue; 
					}
					str.Format(_T("End Retrive Ring Buffer Image #%d"), ii+1);
					AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

					strDebug.Format(_T("%s\\DropImage#%d.PNG"), strFolder, ii+1);
					str.Format(_T("Save Ring Buffer Image #%d [%s]"), ii+1, strDebug);
					AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

					ImageAPI.SaveImage(strDebug, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);
					
					ImagePtr = NULL;
					ImageW = ImageH = ImageStep = 0;
				}
				str = _T("End Motion Thread Fn");
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				AOIDataCollect.SetIsEnableAutoRetry(true);
				return false;	
			}
			else
			{
				/*
				Temperature = CameraCtrl.ReadCameraTemperature(CameraID);
				CameraCtrl.GetCameraCount(CameraID, cntCameraBack, cntExtBak, cntImageBak, cntImageCpy);
				long  ii=0;				
				long  RingStart = 0;
				long  RingEnd = 0;				
				const long RingIndex = (long)(CameraCtrl.GetCameraRingBufferCurrentIndex(CameraID));
				const long RingListSize  = (long)(CameraCtrl.GetCameraRingBufferListSize(CameraID));				
				BitCount = 8;
				RingStart = RingIndex-cntCameraBack;
				if ( RingStart < 0 ) { RingStart += RingListSize; }
				RingEnd = RingStart+cntCameraBack;
				str = _T("Save Ring Buffer Images");
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
				for ( ii=RingStart; ii<RingEnd; ii++ )
				{	
					if ( false == bSaveCameraImage ) { continue; }
					if ( CameraCtrl.GetCameraRingBufferImage(CameraID, ii, ImageW, ImageH, ImageStep, ImagePtr) == false ) 
					{	
						str.Format(_T("Fail To Retrive Ring Buffer Image #%d"), ii+1);
						AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
						continue; 
					}					
					strDebug.Format(_T("%s\\DropImage#%d.PNG"), strFolder, ii+1);
					str.Format(_T("Save Ring Buffer Image #%d [%s]"), ii+1, strDebug);
					AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
					if ( ImageAPI.SaveImage(strDebug, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true) == false )
					{
						str.Format(_T("Fail to Save Ring Buffer Image #%d [%s]"), ii+1, strDebug);
						AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);						
					}					
					ImagePtr = NULL;
					ImageW = ImageH = ImageStep = 0;
				}
				*/
			}			
			CastIndex ++;
			if ( true == bCameraFinish ) 
			{	break; }
			//else
			//{	::Sleep(100); }
			
		};
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::MoveToBeforePCBInPos()//移動至進板前位置
{
	TMotionParameter &Param = GetMotionParameter();
	const double StagePosX = Param.m_BeforePCBInPosX;
	const double StagePosY = Param.m_BeforePCBInPosY;
	//const double StagePosZ = ParamPtr->m_BeforePCBInPosZ;
	//if ( this->XYZMoveTo(StagePosX, StagePosY, StagePosZ) == false )
	if ( this->XYMoveTo(StagePosX, StagePosY) == false )
	{
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_MOVE);
		return false; 
	}	
	return true;;//OFFLINE_VERSION
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetMotionPCBStopPosX(LANE_ID LaneID, bool RightSide) const//PCB停板位置
{
	const TMotionParameter &MotionParam=GetMotionParameter();
	if ( LANE_ID_B == LaneID )
	{
		if ( false == RightSide )
		{	return MotionParam.m_PCBStopLPosX_LB;  }		
		return MotionParam.m_PCBStopRPosX_LB;
	}
	if ( false == RightSide )
	{	return MotionParam.m_PCBStopLPosX_LA;  }
	return MotionParam.m_PCBStopRPosX_LA;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionPCBStopPosX(LANE_ID LaneID, bool RightSide, double val)//PCB停板位置
{
	TMotionParameter &MotionParam=GetMotionParameter();
	switch ( LaneID )
	{
	case LANE_ID_B:	
		if ( false == RightSide )
		{	MotionParam.m_PCBStopLPosX_LB = val;	}
		else
		{	MotionParam.m_PCBStopRPosX_LB = val;	}
		break;
	case LANE_ID_A:	
		if ( false == RightSide )
		{	MotionParam.m_PCBStopLPosX_LA = val;	}
		else
		{	MotionParam.m_PCBStopRPosX_LA = val;	}
		break;
	}	
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetMotionPCBStopPosY(LANE_ID LaneID, bool RightSide) const//PCB停板位置
{
	const TMotionParameter &MotionParam=GetMotionParameter();
	if ( LANE_ID_B == LaneID )
	{	
		if ( false == RightSide )
		{	return MotionParam.m_PCBStopLPosY_LB;  }
		return MotionParam.m_PCBStopRPosY_LB; 
	}
	if ( false == RightSide )
	{	return MotionParam.m_PCBStopLPosY_LA;  }
	return MotionParam.m_PCBStopRPosY_LA;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionPCBStopPosY(LANE_ID LaneID, bool RightSide, double val)//PCB停板位置
{
	TMotionParameter &MotionParam=GetMotionParameter();
	//Y值一律相同
	switch ( LaneID )
	{
	case LANE_ID_B:	
		if ( false == RightSide )
		{	MotionParam.m_PCBStopLPosY_LB = val;	}
		else
		{	MotionParam.m_PCBStopRPosY_LB = val;	}
		MotionParam.m_PCBStopRPosY_LB  = val;
		MotionParam.m_PCBStopLPosY_LB  = val;
		break;
	case LANE_ID_A:	
		if ( false == RightSide )
		{	MotionParam.m_PCBStopLPosY_LA = val; }
		else
		{	MotionParam.m_PCBStopRPosY_LA = val;	}
		MotionParam.m_PCBStopRPosY_LA  = val;
		MotionParam.m_PCBStopLPosY_LA  = val;
		break;
	}	
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetMotionPCBStopPosZ(LANE_ID LaneID, bool RightSide) const//PCB停板位置
{
	const TMotionParameter &MotionParam=GetMotionParameter();
	if ( LANE_ID_B == LaneID )
	{	
		if ( false == RightSide )
		{	return MotionParam.m_PCBStopLPosZ_LB;  }
		return MotionParam.m_PCBStopRPosZ_LB; 
	}
	if ( false == RightSide )
	{	return MotionParam.m_PCBStopLPosZ_LA;  }
	return MotionParam.m_PCBStopRPosZ_LA;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionPCBStopPosZ(LANE_ID LaneID, bool RightSide, double val)//PCB停板位置
{
	TMotionParameter &MotionParam=GetMotionParameter();
	//相同高度
	switch ( LaneID )
	{
	case LANE_ID_B:	
		//if ( false == RightSide )
		//{	MotionParam.m_PCBStopLPosZ_LB = val;	}
		//else
		//{	MotionParam.m_PCBStopRPosZ_LB = val;	}
		MotionParam.m_PCBStopRPosZ_LB = val;
		MotionParam.m_PCBStopLPosZ_LB = val;
		break;
	case LANE_ID_A:	
		//if ( false == RightSide )
		//{	MotionParam.m_PCBStopLPosZ_LA = val;	}
		//else
		//{	MotionParam.m_PCBStopRPosZ_LA = val;	}
		MotionParam.m_PCBStopRPosZ_LA = val;
		MotionParam.m_PCBStopLPosZ_LA = val;
		break;
	}		
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetMotionLaneLedStopPosX(LANE_ID LaneID, bool RightSide)//軌道LED停板位置
{
	const TMotionParameter &MotionParam=GetMotionParameter();
	if ( LANE_ID_B == LaneID )
	{	
		if ( false == RightSide )
		{	return MotionParam.m_LaneLedStopLPosX_LB;  }
		return MotionParam.m_LaneLedStopRPosX_LB; 
	}
	if ( false == RightSide )
	{	return MotionParam.m_LaneLedStopLPosX_LA;  }
	return MotionParam.m_LaneLedStopRPosX_LA;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionLaneLedStopPosX(LANE_ID LaneID, bool RightSide, double val)//軌道LED停板位置
{
	TMotionParameter &MotionParam=GetMotionParameter();
	switch ( LaneID )
	{
	case LANE_ID_B:	
		if ( false == RightSide )
		{	MotionParam.m_LaneLedStopLPosX_LB = val;	}
		else
		{	MotionParam.m_LaneLedStopRPosX_LB = val;	}		
		break;
	case LANE_ID_A:	
		if ( false == RightSide )
		{	MotionParam.m_LaneLedStopLPosX_LA = val; }
		else
		{	MotionParam.m_LaneLedStopRPosX_LA = val;	}		
		break;
	}	
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetMotionLaneLedStopPosY(LANE_ID LaneID, bool RightSide)//軌道LED停板位置
{
	const TMotionParameter &MotionParam=GetMotionParameter();
	if ( LANE_ID_B == LaneID )
	{	
		if ( false == RightSide )
		{	return MotionParam.m_LaneLedStopLPosY_LB;  }
		return MotionParam.m_LaneLedStopRPosY_LB; 
	}
	if ( false == RightSide )
	{	return MotionParam.m_LaneLedStopLPosY_LA;  }
	return MotionParam.m_LaneLedStopRPosY_LA;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionLaneLedStopPosY(LANE_ID LaneID, bool RightSide, double val)//軌道LED停板位置
{
	TMotionParameter &MotionParam=GetMotionParameter();
	switch ( LaneID )
	{
	case LANE_ID_B:	
		if ( false == RightSide )
		{	MotionParam.m_LaneLedStopLPosY_LB = val;	}
		else
		{	MotionParam.m_LaneLedStopRPosY_LB = val;	}		
		break;
	case LANE_ID_A:	
		if ( false == RightSide )
		{	MotionParam.m_LaneLedStopLPosY_LA = val; }
		else
		{	MotionParam.m_LaneLedStopRPosY_LA = val;	}		
		break;
	}	
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetMotionLaneLedStopPosZ(LANE_ID LaneID, bool RightSide)//軌道LED停板位置
{
	const TMotionParameter &MotionParam=GetMotionParameter();
	if ( LANE_ID_B == LaneID )
	{	
		if ( false == RightSide )
		{	return MotionParam.m_LaneLedStopLPosZ_LB;  }
		return MotionParam.m_LaneLedStopRPosZ_LB; 
	}
	if ( false == RightSide )
	{	return MotionParam.m_LaneLedStopLPosZ_LA;  }
	return MotionParam.m_LaneLedStopRPosZ_LA;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionLaneLedStopPosZ(LANE_ID LaneID, bool RightSide, double val)//軌道LED停板位置
{
	TMotionParameter &MotionParam=GetMotionParameter();
	switch ( LaneID )
	{
	case LANE_ID_B:	
		if ( false == RightSide )
		{	MotionParam.m_LaneLedStopLPosZ_LB = val;	}
		else
		{	MotionParam.m_LaneLedStopRPosZ_LB = val;	}		
		break;
	case LANE_ID_A:	
		if ( false == RightSide )
		{	MotionParam.m_LaneLedStopLPosZ_LA = val; }
		else
		{	MotionParam.m_LaneLedStopRPosZ_LA = val;	}		
		break;
	}
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetMotionLaneLedSlowPosX(LANE_ID LaneID, bool RightSide)//軌道LED減速位置
{
	const TMotionParameter &MotionParam=GetMotionParameter();
	if ( LANE_ID_B == LaneID )
	{	
		if ( false == RightSide )
		{	return MotionParam.m_LaneLedSlowLPosX_LB;  }
		return MotionParam.m_LaneLedSlowRPosX_LB; 
	}
	if ( false == RightSide )
	{	return MotionParam.m_LaneLedSlowLPosX_LA;  }
	return MotionParam.m_LaneLedSlowRPosX_LA;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionLaneLedSlowPosX(LANE_ID LaneID, bool RightSide, double val)//軌道LED減速位置
{
	TMotionParameter &MotionParam=GetMotionParameter();
	switch ( LaneID )
	{
	case LANE_ID_B:	
		if ( false == RightSide )
		{	MotionParam.m_LaneLedSlowLPosX_LB = val;	}
		else
		{	MotionParam.m_LaneLedSlowRPosX_LB = val;	}		
		break;
	case LANE_ID_A:	
		if ( false == RightSide )
		{	MotionParam.m_LaneLedSlowLPosX_LA = val; }
		else
		{	MotionParam.m_LaneLedSlowRPosX_LA = val;	}		
		break;
	}
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetMotionLaneLedSlowPosY(LANE_ID LaneID, bool RightSide)//軌道LED減速位置
{
	const TMotionParameter &MotionParam=GetMotionParameter();
	if ( LANE_ID_B == LaneID )
	{	
		if ( false == RightSide )
		{	return MotionParam.m_LaneLedSlowLPosY_LB;  }
		return MotionParam.m_LaneLedSlowRPosY_LB; 
	}
	if ( false == RightSide )
	{	return MotionParam.m_LaneLedSlowLPosY_LA;  }
	return MotionParam.m_LaneLedSlowRPosY_LA;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionLaneLedSlowPosY(LANE_ID LaneID, bool RightSide, double val)//軌道LED減速位置
{
	TMotionParameter &MotionParam=GetMotionParameter();
	switch ( LaneID )
	{
	case LANE_ID_B:	
		if ( false == RightSide )
		{	MotionParam.m_LaneLedSlowLPosY_LB = val;	}
		else
		{	MotionParam.m_LaneLedSlowRPosY_LB = val;	}		
		break;
	case LANE_ID_A:	
		if ( false == RightSide )
		{	MotionParam.m_LaneLedSlowLPosY_LA = val; }
		else
		{	MotionParam.m_LaneLedSlowRPosY_LA = val;	}		
		break;
	}
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::GetMotionLaneLedSlowPosZ(LANE_ID LaneID, bool RightSide)//軌道LED減速位置
{
	const TMotionParameter &MotionParam=GetMotionParameter();
	if ( LANE_ID_B == LaneID )
	{	
		if ( false == RightSide )
		{	return MotionParam.m_LaneLedSlowLPosZ_LB;  }
		return MotionParam.m_LaneLedSlowRPosZ_LB; 
	}
	if ( false == RightSide )
	{	return MotionParam.m_LaneLedSlowLPosZ_LA;  }
	return MotionParam.m_LaneLedSlowRPosZ_LA;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionLaneLedSlowPosZ(LANE_ID LaneID, bool RightSide, double val)//軌道LED減速位置
{
	TMotionParameter &MotionParam=GetMotionParameter();
	switch ( LaneID )
	{
	case LANE_ID_B:	
		if ( false == RightSide )
		{	MotionParam.m_LaneLedSlowLPosZ_LB = val;	}
		else
		{	MotionParam.m_LaneLedSlowRPosZ_LB = val;	}		
		break;
	case LANE_ID_A:	
		if ( false == RightSide )
		{	MotionParam.m_LaneLedSlowLPosZ_LA = val; }
		else
		{	MotionParam.m_LaneLedSlowRPosZ_LA = val;	}		
		break;
	}
}
//----------------------------------------------------------------------------------//
LANE_ID CMotion_Basic::GetMotionXYCaliLaneID() const//取得XY校正表軌道編號
{
	return m_XYCaliLaneID;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetMotionXYCaliLaneID(LANE_ID val)//設XY校正表軌道編號
{
	m_XYCaliLaneID = val;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::GetStopXYCalibration() const//取得停止XY校正
{
	return m_StopXYCalibration;
}
//----------------------------------------------------------------------------------//
void CMotion_Basic::SetStopXYCalibration(bool val)//設定停止XY校正
{
	m_StopXYCalibration = val;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::SaveMotionXYCali()//儲存運動系統的XY校正表
{
	CString   Folder;
	CString   Filename;
	Folder = AOIDataCollect.GetAOIDirectory();
	Filename.Format(_T("%s\\%s"), Folder, MOTION_XY_CALI_FILE);
	if ( SaveMotionXYCali(Filename, m_XYCaliList) == false )
	{
		SetMotionExceptionCode_FileWrite();
		return false; 
	}
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::LoadMotionXYCali()//載入運動系統的XY校正表
{
	CString   Folder;
	CString   Filename;
	Folder = AOIDataCollect.GetAOIDirectory();
	Filename.Format(_T("%s\\%s"), Folder, MOTION_XY_CALI_FILE);	
	if ( LoadMotionXYCali(Filename, m_XYCaliList) == false )
	{
		SetMotionExceptionCode_FileRead();
		return false; 
	}
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::SaveMotionXYCali(LPCTSTR filename, std::vector<TXYCali> &XYCaliList)//儲存運動系統的XY校正表
{		
	FILE     *pfile = NULL;		
	CString   Filename=filename;	
	pfile = ::_tfopen(Filename, _T("w+"));
	if ( pfile == NULL )
	{
		m_ErrorString.Format(_T("Error, open file fault (%s)"), Filename);		
		return false;
	}

	size_t      i=0;
	TXYCali    *XYCaliPtr = NULL;
	const size_t Count = (XYCaliList.size());

	::fprintf(pfile, "%d, %d\n", XYCALI_START, 0);
	for ( i=0; i<Count; i++ )
	{
		XYCaliPtr = &(XYCaliList[i]);

		::fprintf(pfile, "%d, %d\n", XYCALI_BEGIN, 0);
		::fprintf(pfile, "%d, %d\n", XYCALI_INDEX, i+1);		

		::fprintf(pfile, "%d, %.16f\n", XYCALI_POS_X1, XYCaliPtr->PosX1);
		::fprintf(pfile, "%d, %.16f\n", XYCALI_POS_Y1, XYCaliPtr->PosY1);
		::fprintf(pfile, "%d, %.16f\n", XYCALI_CALI_X1, XYCaliPtr->CaliX1);
		::fprintf(pfile, "%d, %.16f\n", XYCALI_CALI_Y1, XYCaliPtr->CaliY1);

		::fprintf(pfile, "%d, %.16f\n", XYCALI_POS_X2, XYCaliPtr->PosX2);
		::fprintf(pfile, "%d, %.16f\n", XYCALI_POS_Y2, XYCaliPtr->PosY2);
		::fprintf(pfile, "%d, %.16f\n", XYCALI_CALI_X2, XYCaliPtr->CaliX2);
		::fprintf(pfile, "%d, %.16f\n", XYCALI_CALI_Y2, XYCaliPtr->CaliY2);

		::fprintf(pfile, "%d, %.16f\n", XYCALI_POS_X3, XYCaliPtr->PosX3);
		::fprintf(pfile, "%d, %.16f\n", XYCALI_POS_Y3, XYCaliPtr->PosY3);
		::fprintf(pfile, "%d, %.16f\n", XYCALI_CALI_X3, XYCaliPtr->CaliX3);
		::fprintf(pfile, "%d, %.16f\n", XYCALI_CALI_Y3, XYCaliPtr->CaliY3);

		::fprintf(pfile, "%d, %.16f\n", XYCALI_POS_X4, XYCaliPtr->PosX4);
		::fprintf(pfile, "%d, %.16f\n", XYCALI_POS_Y4, XYCaliPtr->PosY4);
		::fprintf(pfile, "%d, %.16f\n", XYCALI_CALI_X4, XYCaliPtr->CaliX4);
		::fprintf(pfile, "%d, %.16f\n", XYCALI_CALI_Y4, XYCaliPtr->CaliY4);

		::fprintf(pfile, "%d, %d\n", XYCALI_END, 0);
		
	}
	::fprintf(pfile, "%d, %d\n", XYCALI_FINISH, 0);	
	::fclose(pfile); pfile = NULL;
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::LoadMotionXYCali(LPCTSTR filename, std::vector<TXYCali> &XYCaliList)//儲存運動系統的XY校正表
{		
	FILE     *pfile = NULL;
	CString   Filename = filename;	
	pfile = ::_tfopen(Filename, _T("r"));
	if ( pfile == NULL )
	{
		m_ErrorString.Format(_T("Error, open file fault (%s)"), Filename);		
		return false;
	}
	
	int         i=0, j=0;
	int         idx=0;
	int         len=0;
	int         ReadIndex=0;
	bool        GetFinish=false;
	TXYCali     XYCali;	
	const char  ch = ',';
	const int   textlinesize = 256;
	char        data[textlinesize]="";
	char        index[textlinesize]="";
	char        textline[textlinesize]="";	
	double      OffsetX1=0, OffsetX2=0, OffsetX3=0, OffsetX4=0;
	double      OffsetY1=0, OffsetY2=0, OffsetY3=0, OffsetY4=0;

	while(!feof(pfile) )
	{ 			
		if( fgets(textline,textlinesize,pfile) ==NULL ) 
		{	continue; }
		if ( JetAPI::DecoderTextLineA(textline, index, data) == false ) 
		{	continue; }
		idx = ::atoi(index);

		switch ( idx )
		{
		case XYCALI_START:      XYCaliList.clear();       break;
		case XYCALI_BEGIN:		InitMotionXYCali(XYCali);	break;
		case XYCALI_INDEX:		ReadIndex = ::atoi(data);	break;

		case XYCALI_POS_X1:		XYCali.PosX1 = ::atof(data);	break;
		case XYCALI_POS_Y1:		XYCali.PosY1 = ::atof(data);	break;
		case XYCALI_CALI_X1:	XYCali.CaliX1 = ::atof(data);	break;
		case XYCALI_CALI_Y1:	XYCali.CaliY1 = ::atof(data);	break;

		case XYCALI_POS_X2:		XYCali.PosX2 = ::atof(data);	break;
		case XYCALI_POS_Y2:		XYCali.PosY2 = ::atof(data);	break;
		case XYCALI_CALI_X2:	XYCali.CaliX2 = ::atof(data);	break;
		case XYCALI_CALI_Y2:	XYCali.CaliY2 = ::atof(data);	break;

		case XYCALI_POS_X3:		XYCali.PosX3 = ::atof(data);	break;
		case XYCALI_POS_Y3:		XYCali.PosY3 = ::atof(data);	break;
		case XYCALI_CALI_X3:	XYCali.CaliX3 = ::atof(data);	break;
		case XYCALI_CALI_Y3:	XYCali.CaliY3 = ::atof(data);	break;

		case XYCALI_POS_X4:		XYCali.PosX4 = ::atof(data);	break;
		case XYCALI_POS_Y4:		XYCali.PosY4 = ::atof(data);	break;
		case XYCALI_CALI_X4:	XYCali.CaliX4 = ::atof(data);	break;
		case XYCALI_CALI_Y4:	XYCali.CaliY4 = ::atof(data);	break;

		case XYCALI_END:
			CalcMotionXYCaliLimit(XYCali);
			CalcMotionXYCaliTransform(XYCali);
			XYCaliList.push_back(XYCali);
			break;
		case XYCALI_FINISH:
			GetFinish = true;
			break;
		}
	};
	::fclose(pfile); pfile = NULL;

	SetMotionXYCaliLaneID(LANE_ID_NULL);
	if ( GetFinish == false )
	{
		XYCaliList.clear(); 
		return false;
	}	
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::ChangeMotionXYCali()//變更運動系統的XY校正表
{	
	CString XYNodeCaliName_LA;
	CString XYNodeCaliName_LB;
	int CountX_LA=0, CountY_LA=0;
	int CountX_LB=0, CountY_LB=0;
	LANE_ID XYCaliLaneID;
	LANE_ID UnSetLaneID=LANE_ID_NULL;
	std::vector<TDotNode> DotNodeList_LA;
	std::vector<TDotNode> DotNodeList_LB;	
	LANE_ID CurXYCaliLaneID = GetMotionXYCaliLaneID();
	LANE_WORK_MODE LaneWordkMode_LA = AOIDataCollect.GetLaneWorkMode_LA();
	LANE_WORK_MODE LaneWordkMode_LB = AOIDataCollect.GetLaneWorkMode_LB();

	XYCaliLaneID = UnSetLaneID;
	if ( LANE_WORK_DISABLE != LaneWordkMode_LA )
	{
		if (UnSetLaneID == XYCaliLaneID ) { XYCaliLaneID = LANE_ID_A; }
		else { XYCaliLaneID = LANE_ID_BOTH; }
		XYNodeCaliName_LA = AOIDataCollect.GetDotNodeCaliFilename(LANE_ID_A);
		AOIDataCollect.LoadXYDotNodeCaliFile(XYNodeCaliName_LA, CountX_LA, CountY_LA, DotNodeList_LA);
	}
	if ( LANE_WORK_DISABLE != LaneWordkMode_LB )
	{
		if (UnSetLaneID == XYCaliLaneID ) { XYCaliLaneID = LANE_ID_B; }
		else { XYCaliLaneID = LANE_ID_BOTH; }
		XYNodeCaliName_LB = AOIDataCollect.GetDotNodeCaliFilename(LANE_ID_B);
		AOIDataCollect.LoadXYDotNodeCaliFile(XYNodeCaliName_LB, CountX_LB, CountY_LB, DotNodeList_LB);
	}
	if (UnSetLaneID != CurXYCaliLaneID)
	{
		if (CurXYCaliLaneID == XYCaliLaneID )
		{	return true;	}
	}
	const size_t DotNodeCount_LA = DotNodeList_LA.size();
	const size_t DotNodeCount_LB = DotNodeList_LB.size();
	if ( 0==DotNodeCount_LA && 0==DotNodeCount_LB )
	{	return true; }

	size_t      i=0;
	std::vector<TXYCali> XYCaliList;
	std::vector<TXYCali> XYCaliList_LA;
	std::vector<TXYCali> XYCaliList_LB;
	std::vector<TDotNode> DotNodeList_LA2;
	std::vector<TDotNode> DotNodeList_LB2;
	AOIDataCollect.RefineXYDotNodeListByLane(DotNodeList_LA, DotNodeList_LB, DotNodeList_LA2, DotNodeList_LB2);

	//因為只有縮短Y的資料, 所以不用管CountX, CountY
	AOIDataCollect.ConvertXYDotNodeToXYCali(CountX_LA, CountY_LA, DotNodeList_LA2, XYCaliList_LA);
	AOIDataCollect.ConvertXYDotNodeToXYCali(CountX_LB, CountY_LB, DotNodeList_LB2, XYCaliList_LB);

	const size_t XYCaliCont_LA = XYCaliList_LA.size();
	const size_t XYCaliCont_LB = XYCaliList_LB.size();

	XYCaliList.clear();
	for ( i=0; i<XYCaliCont_LA; i++ )
	{	XYCaliList.push_back(XYCaliList_LA[i]);	}
	for ( i=0; i<XYCaliCont_LB; i++ )
	{	XYCaliList.push_back(XYCaliList_LB[i]);	}

	const bool Rebuild = true;
	SetMotionXYCaliLaneID(XYCaliLaneID);
	SetMotionXYYCaliList(XYCaliList, Rebuild);
	SaveMotionXYCali();
	//SetMotionExceptionCode(AOI_EXCEPTION_MOTION_PARAM_XY_CALI);
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::InitMotionXYCali(TXYCali &XYCali)//初始化XY參數
{
	XYCali.PosX1=XYCali.PosX2=XYCali.PosX3=XYCali.PosX4=0;
	XYCali.PosY1=XYCali.PosY2=XYCali.PosY3=XYCali.PosY4=0;
	XYCali.CaliX1=XYCali.CaliX2=XYCali.CaliX3=XYCali.CaliX4=0;
	XYCali.CaliY1=XYCali.CaliY2=XYCali.CaliY3=XYCali.CaliY4=0;
	XYCali.PosXMin=XYCali.PosXMax=XYCali.PosYMin=XYCali.PosYMax=0;
	XYCali.CaliXMin=XYCali.CaliXMax=XYCali.CaliYMin=XYCali.CaliYMax=0;

	//跟機台方向有關嗎?
	XYCali.P2C_M11=1; XYCali.P2C_M12=0;	XYCali.P2C_M13=0;
	XYCali.P2C_M21=0; XYCali.P2C_M22=1; XYCali.P2C_M23=0;
	XYCali.P2C_M31=0; XYCali.P2C_M32=0; XYCali.P2C_M33=1;

	XYCali.C2P_M11=1; XYCali.C2P_M12=0; XYCali.C2P_M13=0;
	XYCali.C2P_M21=0; XYCali.C2P_M22=1; XYCali.C2P_M23=0;
	XYCali.C2P_M31=0; XYCali.C2P_M32=0; XYCali.C2P_M33=1;
	//SetMotionExceptionCode(AOI_EXCEPTION_MOTION_PARAM_XY_CALI);
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::RearrangeMotionXYCali(TXYCali &XYCali)//排序XY內的四個端點
{
	TXYCali TmpXYCali = XYCali;
	double L1=0, L2=0, L3=0, L4=0;
	bool   Used1=false, Used2=false, Used3=false, Used4=false;

	InitMotionXYCali(XYCali);
	//排序
	//P4, P3
	//P1, P2

	//Find P1
	L1 = (TmpXYCali.PosX1*TmpXYCali.PosX1)+(TmpXYCali.PosY1*TmpXYCali.PosY1);
	L2 = (TmpXYCali.PosX2*TmpXYCali.PosX2)+(TmpXYCali.PosY2*TmpXYCali.PosY2);
	L3 = (TmpXYCali.PosX3*TmpXYCali.PosX3)+(TmpXYCali.PosY3*TmpXYCali.PosY3);
	L4 = (TmpXYCali.PosX4*TmpXYCali.PosX4)+(TmpXYCali.PosY4*TmpXYCali.PosY4);
	if ( L1<L2 && L1<L3 && L1<L4 ) 
	{
		Used1 = true;
		XYCali.PosX1 = TmpXYCali.PosX1;
		XYCali.PosY1 = TmpXYCali.PosY1;
		XYCali.CaliX1 = TmpXYCali.CaliX1;
		XYCali.CaliY1 = TmpXYCali.CaliY1;
	}
	else if ( L2<L1 && L2<L3 && L2<L4 ) 
	{
		Used2 = true;
		XYCali.PosX1 = TmpXYCali.PosX2;
		XYCali.PosY1 = TmpXYCali.PosY2;
		XYCali.CaliX1 = TmpXYCali.CaliX2;
		XYCali.CaliY1 = TmpXYCali.CaliY2;
	}
	else if ( L3<L1 && L3<L2 && L3<L4 ) 
	{
		Used3 = true;
		XYCali.PosX1 = TmpXYCali.PosX3;
		XYCali.PosY1 = TmpXYCali.PosY3;
		XYCali.CaliX1 = TmpXYCali.CaliX3;
		XYCali.CaliY1 = TmpXYCali.CaliY3;
	}
	else
	{
		Used4 = true;
		XYCali.PosX1 = TmpXYCali.PosX4;
		XYCali.PosY1 = TmpXYCali.PosY4;
		XYCali.CaliX1 = TmpXYCali.CaliX4;
		XYCali.CaliY1 = TmpXYCali.CaliY4;
	}

	//Find P2 - 最小的Y
	if ( Used1 == true ) 
	{	L1 = DBL_MAX; }
	else
	{	L1 = fabs(TmpXYCali.PosY1-XYCali.PosY1); }
	if ( Used2 == true ) 
	{	L2 = DBL_MAX; }
	else
	{	L2 = fabs(TmpXYCali.PosY2-XYCali.PosY1); }
	if ( Used3 == true ) 
	{	L3 = DBL_MAX; }
	else
	{	L3 = fabs(TmpXYCali.PosY3-XYCali.PosY1); }
	if ( Used4 == true ) 
	{	L4 = DBL_MAX; }
	else
	{	L4 = fabs(TmpXYCali.PosY4-XYCali.PosY1); }

	if ( L1<L2 && L1<L3 && L1<L4 ) 
	{
		Used1 = true;
		XYCali.PosX2 = TmpXYCali.PosX1;
		XYCali.PosY2 = TmpXYCali.PosY1;
		XYCali.CaliX2 = TmpXYCali.CaliX1;
		XYCali.CaliY2 = TmpXYCali.CaliY1;
	}
	else if ( L2<L1 && L2<L3 && L2<L4 ) 
	{
		Used2 = true;
		XYCali.PosX2 = TmpXYCali.PosX2;
		XYCali.PosY2 = TmpXYCali.PosY2;
		XYCali.CaliX2 = TmpXYCali.CaliX2;
		XYCali.CaliY2 = TmpXYCali.CaliY2;
	}
	else if ( L3<L1 && L3<L2 && L3<L4 ) 
	{
		Used3 = true;
		XYCali.PosX2 = TmpXYCali.PosX3;
		XYCali.PosY2 = TmpXYCali.PosY3;
		XYCali.CaliX2 = TmpXYCali.CaliX3;
		XYCali.CaliY2 = TmpXYCali.CaliY3;
	}
	else
	{
		Used4 = true;
		XYCali.PosX2 = TmpXYCali.PosX4;
		XYCali.PosY2 = TmpXYCali.PosY4;
		XYCali.CaliX2 = TmpXYCali.CaliX4;
		XYCali.CaliY2 = TmpXYCali.CaliY4;
	}

	//Find P4 - 最小的X
	if ( Used1 == true ) 
	{	L1 = DBL_MAX; }
	else
	{	L1 = fabs(TmpXYCali.PosX1-XYCali.PosX1); }
	if ( Used2 == true ) 
	{	L2 = DBL_MAX; }
	else
	{	L2 = fabs(TmpXYCali.PosX2-XYCali.PosX1); }
	if ( Used3 == true ) 
	{	L3 = DBL_MAX; }
	else
	{	L3 = fabs(TmpXYCali.PosX3-XYCali.PosX1); }
	if ( Used4 == true ) 
	{	L4 = DBL_MAX; }
	else
	{	L4 = fabs(TmpXYCali.PosX4-XYCali.PosX1); }
	if ( L1<L2 && L1<L3 && L1<L4 ) 
	{
		Used1 = true;
		XYCali.PosX4 = TmpXYCali.PosX1;
		XYCali.PosY4 = TmpXYCali.PosY1;
		XYCali.CaliX4 = TmpXYCali.CaliX1;
		XYCali.CaliY4 = TmpXYCali.CaliY1;
	}
	else if ( L2<L1 && L2<L3 && L2<L4 ) 
	{
		Used2 = true;
		XYCali.PosX4 = TmpXYCali.PosX2;
		XYCali.PosY4 = TmpXYCali.PosY2;
		XYCali.CaliX4 = TmpXYCali.CaliX2;
		XYCali.CaliY4 = TmpXYCali.CaliY2;
	}
	else if ( L3<L1 && L3<L2 && L3<L4 ) 
	{
		Used3 = true;
		XYCali.PosX4 = TmpXYCali.PosX3;
		XYCali.PosY4 = TmpXYCali.PosY3;
		XYCali.CaliX4 = TmpXYCali.CaliX3;
		XYCali.CaliY4 = TmpXYCali.CaliY3;
	}
	else
	{
		Used4 = true;
		XYCali.PosX4 = TmpXYCali.PosX4;
		XYCali.PosY4 = TmpXYCali.PosY4;
		XYCali.CaliX4 = TmpXYCali.CaliX4;
		XYCali.CaliY4 = TmpXYCali.CaliY4;
	}

	//剩餘的就是P3
	if ( Used1 == false ) 
	{
		Used1 = true;
		XYCali.PosX3 = TmpXYCali.PosX1;
		XYCali.PosY3 = TmpXYCali.PosY1;
		XYCali.CaliX3 = TmpXYCali.CaliX1;
		XYCali.CaliY3 = TmpXYCali.CaliY1;
	}
	else if ( Used2 == false ) 
	{
		Used2 = true;
		XYCali.PosX3 = TmpXYCali.PosX2;
		XYCali.PosY3 = TmpXYCali.PosY2;
		XYCali.CaliX3 = TmpXYCali.CaliX2;
		XYCali.CaliY3 = TmpXYCali.CaliY2;
	}
	else if ( Used3 == false ) 
	{
		Used3 = true;
		XYCali.PosX3 = TmpXYCali.PosX3;
		XYCali.PosY3 = TmpXYCali.PosY3;
		XYCali.CaliX3 = TmpXYCali.CaliX3;
		XYCali.CaliY3 = TmpXYCali.CaliY3;
	}
	else
	{
		Used4 = true;
		XYCali.PosX3 = TmpXYCali.PosX4;
		XYCali.PosY3 = TmpXYCali.PosY4;
		XYCali.CaliX3 = TmpXYCali.CaliX4;
		XYCali.CaliY3 = TmpXYCali.CaliY4;
	}

	//確認是否有異常
	if ( Used1==false || Used2==false || Used3==false || Used4==false )
	{
		XYCali = TmpXYCali;		
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_PARAM_XY_CALI);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CalcMotionXYCaliLimit(TXYCali &XYCali)//計算XY內的極限範圍
{
	XYCali.PosXMin = XYCali.PosXMax = XYCali.PosX1;
	XYCali.PosYMin = XYCali.PosYMax = XYCali.PosY1;
	XYCali.CaliXMin = XYCali.CaliXMax = XYCali.CaliX1;
	XYCali.CaliYMin = XYCali.CaliYMax = XYCali.CaliY1;

	if ( XYCali.PosXMin > XYCali.PosX2 ) { XYCali.PosXMin = XYCali.PosX2; }
	if ( XYCali.PosYMin > XYCali.PosY2 ) { XYCali.PosYMin = XYCali.PosY2; }
	if ( XYCali.PosXMax < XYCali.PosX2 ) { XYCali.PosXMax = XYCali.PosX2; }
	if ( XYCali.PosYMax < XYCali.PosY2 ) { XYCali.PosYMax = XYCali.PosY2; }

	if ( XYCali.CaliXMin > XYCali.CaliX2 ) { XYCali.CaliXMin = XYCali.CaliX2; }
	if ( XYCali.CaliYMin > XYCali.CaliY2 ) { XYCali.CaliYMin = XYCali.CaliY2; }
	if ( XYCali.CaliXMax < XYCali.CaliX2 ) { XYCali.CaliXMax = XYCali.CaliX2; }
	if ( XYCali.CaliYMax < XYCali.CaliY2 ) { XYCali.CaliYMax = XYCali.CaliY2; }

	if ( XYCali.PosXMin > XYCali.PosX3 ) { XYCali.PosXMin = XYCali.PosX3; }
	if ( XYCali.PosYMin > XYCali.PosY3 ) { XYCali.PosYMin = XYCali.PosY3; }
	if ( XYCali.PosXMax < XYCali.PosX3 ) { XYCali.PosXMax = XYCali.PosX3; }
	if ( XYCali.PosYMax < XYCali.PosY3 ) { XYCali.PosYMax = XYCali.PosY3; }

	if ( XYCali.CaliXMin > XYCali.CaliX3 ) { XYCali.CaliXMin = XYCali.CaliX3; }
	if ( XYCali.CaliYMin > XYCali.CaliY3 ) { XYCali.CaliYMin = XYCali.CaliY3; }
	if ( XYCali.CaliXMax < XYCali.CaliX3 ) { XYCali.CaliXMax = XYCali.CaliX3; }
	if ( XYCali.CaliYMax < XYCali.CaliY3 ) { XYCali.CaliYMax = XYCali.CaliY3; }

	if ( XYCali.PosXMin > XYCali.PosX4 ) { XYCali.PosXMin = XYCali.PosX4; }
	if ( XYCali.PosYMin > XYCali.PosY4 ) { XYCali.PosYMin = XYCali.PosY4; }
	if ( XYCali.PosXMax < XYCali.PosX4 ) { XYCali.PosXMax = XYCali.PosX4; }
	if ( XYCali.PosYMax < XYCali.PosY4 ) { XYCali.PosYMax = XYCali.PosY4; }

	if ( XYCali.CaliXMin > XYCali.CaliX4 ) { XYCali.CaliXMin = XYCali.CaliX4; }
	if ( XYCali.CaliYMin > XYCali.CaliY4 ) { XYCali.CaliYMin = XYCali.CaliY4; }
	if ( XYCali.CaliXMax < XYCali.CaliX4 ) { XYCali.CaliXMax = XYCali.CaliX4; }
	if ( XYCali.CaliYMax < XYCali.CaliY4 ) { XYCali.CaliYMax = XYCali.CaliY4; }	

	const TMotionParameter &MotionParam=GetMotionParameter();
	const double ExtendRange = MotionParam.m_XYCaliExtendRange;
	XYCali.PosXMin = XYCali.PosXMin - ExtendRange;
	XYCali.PosYMin = XYCali.PosYMin - ExtendRange;
	XYCali.PosXMax = XYCali.PosXMax + ExtendRange;
	XYCali.PosYMax = XYCali.PosYMax + ExtendRange;

	XYCali.CaliXMin = XYCali.CaliXMin - ExtendRange;
	XYCali.CaliYMin = XYCali.CaliYMin - ExtendRange;
	XYCali.CaliXMax = XYCali.CaliXMax + ExtendRange;
	XYCali.CaliYMax = XYCali.CaliYMax + ExtendRange;
	//SetMotionExceptionCode(AOI_EXCEPTION_MOTION_PARAM_XY_CALI);
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CalcMotionXYCaliTransform(TXYCali &XYCali)//計算XY內的轉換公式
{
#ifndef OFFLINE_VERSION
	cv::Point2f src[4];
	cv::Point2f dst[4];	
	double *fPtr = NULL;
	
	//Stage to Cali
	src[0].x = (float)(XYCali.PosX1);	src[0].y = (float)(XYCali.PosY1);
	src[1].x = (float)(XYCali.PosX2);	src[1].y = (float)(XYCali.PosY2);
	src[2].x = (float)(XYCali.PosX3);	src[2].y = (float)(XYCali.PosY3);
	src[3].x = (float)(XYCali.PosX4);	src[3].y = (float)(XYCali.PosY4);		

	dst[0].x = (float)(XYCali.CaliX1); dst[0].y = (float)(XYCali.CaliY1);
	dst[1].x = (float)(XYCali.CaliX2); dst[1].y = (float)(XYCali.CaliY2);
	dst[2].x = (float)(XYCali.CaliX3); dst[2].y = (float)(XYCali.CaliY3);
	dst[3].x = (float)(XYCali.CaliX4); dst[3].y = (float)(XYCali.CaliY4);

	cv::Mat P2Cmat = cv::getPerspectiveTransform(src, dst);
	fPtr = (double*)(P2Cmat.data);
	XYCali.P2C_M11 = fPtr[0];	XYCali.P2C_M12 = fPtr[1];	XYCali.P2C_M13 = fPtr[2];
	XYCali.P2C_M21 = fPtr[3];	XYCali.P2C_M22 = fPtr[4];	XYCali.P2C_M23 = fPtr[5];
	XYCali.P2C_M31 = fPtr[6];	XYCali.P2C_M32 = fPtr[7];	XYCali.P2C_M33 = fPtr[8];

	
	//Cali to Stage
	src[0].x = (float)(XYCali.CaliX1); src[0].y = (float)(XYCali.CaliY1);
	src[1].x = (float)(XYCali.CaliX2); src[1].y = (float)(XYCali.CaliY2);
	src[2].x = (float)(XYCali.CaliX3); src[2].y = (float)(XYCali.CaliY3);
	src[3].x = (float)(XYCali.CaliX4); src[3].y = (float)(XYCali.CaliY4);

	dst[0].x = (float)(XYCali.PosX1);	dst[0].y = (float)(XYCali.PosY1);
	dst[1].x = (float)(XYCali.PosX2);	dst[1].y = (float)(XYCali.PosY2);
	dst[2].x = (float)(XYCali.PosX3);	dst[2].y = (float)(XYCali.PosY3);
	dst[3].x = (float)(XYCali.PosX4);	dst[3].y = (float)(XYCali.PosY4);		
	cv::Mat C2Pmat = cv::getPerspectiveTransform(src, dst);
	fPtr = (double*)(C2Pmat.data);
	XYCali.C2P_M11 = fPtr[0];	XYCali.C2P_M12 = fPtr[1];	XYCali.C2P_M13 = fPtr[2];
	XYCali.C2P_M21 = fPtr[3];	XYCali.C2P_M22 = fPtr[4];	XYCali.C2P_M23 = fPtr[5];
	XYCali.C2P_M31 = fPtr[6];	XYCali.C2P_M32 = fPtr[7];	XYCali.C2P_M33 = fPtr[8];	
	//SetMotionExceptionCode(AOI_EXCEPTION_MOTION_PARAM_XY_CALI);
#endif//OFFLINE_VERSION
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::SetMotionXYYCaliList(const std::vector<TXYCali> &XYCaliList, bool Rebuild)
{
	if ( Rebuild == true ) 
	{	
		m_XYCaliList = XYCaliList;
		return true;
	}

	size_t   i=0;
	const size_t Count = (XYCaliList.size());
	for ( i=0; i<Count; i++ )
	{	m_XYCaliList.push_back(XYCaliList[i]);	}
	//SetMotionExceptionCode(AOI_EXCEPTION_MOTION_PARAM_XY_CALI);
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::StageToCali(double PosX, double PosY, double &CaliX, double &CaliY)//理論座標轉成校正後的座標
{
#ifdef OFFLINE_VERSION
	CaliX = PosX;
	CaliY = PosY;
#else
	const bool StopXYCalibration = GetStopXYCalibration();
	const TMotionParameter &MotionParam=GetMotionParameter();
	if ( FN_DISABLE == MotionParam.m_XYCaliEnable || true == StopXYCalibration )	
	{
		CaliX = PosX;
		CaliY = PosY;
		return true;
	}
	
	size_t       i=0;
	size_t       CaliIndex=-1;	
	double       CpX=0, CpY=0, MinL=0;
	double       OffsetX=0, OffsetY=0, Length=0;
	TXYCali     *XYCaliPtr = NULL;
	const size_t  Count = (m_XYCaliList.size());	
	for ( i=0; i<Count; i++ )
	{
		XYCaliPtr = &(m_XYCaliList[i]);
		if ( PosX < XYCaliPtr->PosXMin || 
			 PosY < XYCaliPtr->PosYMin || 
			 PosX > XYCaliPtr->PosXMax || 
			 PosY > XYCaliPtr->PosYMax )
		{	continue;	}

		
		CpX = (XYCaliPtr->PosX1+XYCaliPtr->PosX2+XYCaliPtr->PosX3+XYCaliPtr->PosX4)/4;
		CpY = (XYCaliPtr->PosY1+XYCaliPtr->PosY2+XYCaliPtr->PosY3+XYCaliPtr->PosY4)/4;
		OffsetX = PosX-CpX;
		OffsetY = PosY-CpY;
		Length = sqrt((OffsetX*OffsetX)+(OffsetY*OffsetY));
		if ( -1==CaliIndex || Length<MinL  ) 
		{
			MinL = Length;
			CaliIndex = i;			
		}
	}
	if ( -1 == CaliIndex ) 
	{
		CaliX = PosX;
		CaliY = PosY;
		return true;
	}

	TXYCali Cali = m_XYCaliList[CaliIndex];
	CaliValue(Cali, true, PosX, PosY, CaliX, CaliY);//座標轉換
#endif//OFFLINE_VERSION	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CaliToStage(double CaliX, double CaliY, double &PosX, double &PosY)//校正後座標轉成理論的座標
{
#ifdef OFFLINE_VERSION
	PosX = CaliX;
	PosY = CaliY;
#else
	const bool StopXYCalibration = GetStopXYCalibration();
	const TMotionParameter &MotionParam=GetMotionParameter();
	if ( FN_DISABLE == MotionParam.m_XYCaliEnable || true == StopXYCalibration )	
	{
		PosX = CaliX;
		PosY = CaliY;
		return true;
	}

	size_t       i=0;
	size_t       CaliIndex=-1;	
	double       CpX=0, CpY=0, MinL=0;
	double       OffsetX=0, OffsetY=0, Length=0;
	TXYCali     *XYCaliPtr = NULL;
	const size_t Count = (m_XYCaliList.size());	
	for ( i=0; i<Count; i++ )
	{
		XYCaliPtr = &(m_XYCaliList[i]);
		if ( CaliX < XYCaliPtr->CaliXMin || 
			 CaliY < XYCaliPtr->CaliYMin || 
			 CaliX > XYCaliPtr->CaliXMax || 
			 CaliY > XYCaliPtr->CaliYMax )
		{	continue;	}

		
		CpX = (XYCaliPtr->CaliX1+XYCaliPtr->CaliX2+XYCaliPtr->CaliX3+XYCaliPtr->CaliX4)/4;
		CpY = (XYCaliPtr->CaliY1+XYCaliPtr->CaliY2+XYCaliPtr->CaliY3+XYCaliPtr->CaliY4)/4;
		OffsetX = CaliX-CpX;
		OffsetY = CaliY-CpY;
		Length = sqrt((OffsetX*OffsetX)+(OffsetY*OffsetY));
		if ( -1==CaliIndex || Length<MinL  ) 
		{
			MinL = Length;
			CaliIndex = i;			
		}
	}
	if ( -1 == CaliIndex ) 
	{
		PosX = CaliX;
		PosY = CaliY;
		return true;
	}

	TXYCali Cali = m_XYCaliList[CaliIndex];
	CaliValue(Cali, false, CaliX, CaliY, PosX, PosY);//座標轉換
#endif//OFFLINE_VERSION	
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::MoveMotionXYCali(double OffsetX, double OffsetY, std::vector<TXYCali> &XYCaliList)//移動運動系統的XY校正表
{
	size_t   i=0;
	const size_t Count = (XYCaliList.size());
	for ( i=0; i<Count; i++ )
	{
		TXYCali &XYCali=XYCaliList[i];
		XYCali.PosX1 += OffsetX;
		XYCali.PosY1 += OffsetY;
		XYCali.CaliX1 += OffsetX;
		XYCali.CaliY1 += OffsetY;

		XYCali.PosX2 += OffsetX;
		XYCali.PosY2 += OffsetY;
		XYCali.CaliX2 += OffsetX;
		XYCali.CaliY2 += OffsetY;
	
		XYCali.PosX3 += OffsetX;
		XYCali.PosY3 += OffsetY;
		XYCali.CaliX3 += OffsetX;
		XYCali.CaliY3 += OffsetY;

		XYCali.PosX4 += OffsetX;
		XYCali.PosY4 += OffsetY;
		XYCali.CaliX4 += OffsetX;
		XYCali.CaliY4 += OffsetY;
	}
	//SetMotionExceptionCode(AOI_EXCEPTION_MOTION_PARAM_XY_CALI);
	return true;	
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CaliValue(const TXYCali &Cali, bool forward, double PosX, double PosY, double &ValX, double &ValY)//座標轉換
{
	return CaliValue_Matrix(Cali, forward, PosX, PosY, ValX, ValY);//直接座標轉換
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CaliValue_Matrix(const TXYCali &Cali, bool forward, double PosX, double PosY, double &ValX, double &ValY)//投影矩陣
{
	double ValW=1;
	double m11=0, m12=0, m13=0;
	double m21=0, m22=0, m23=0;
	double m31=0, m32=0, m33=0;
	if ( forward==true ) 
	{	
		m11 = Cali.P2C_M11;	m12 = Cali.P2C_M12;	m13 = Cali.P2C_M13;
		m21 = Cali.P2C_M21;	m22 = Cali.P2C_M22;	m23 = Cali.P2C_M23;
		m31 = Cali.P2C_M31;	m32 = Cali.P2C_M32;	m33 = Cali.P2C_M33;		
	}
	else
	{
		m11 = Cali.C2P_M11;	m12 = Cali.C2P_M12;	m13 = Cali.C2P_M13;
		m21 = Cali.C2P_M21;	m22 = Cali.C2P_M22;	m23 = Cali.C2P_M23;
		m31 = Cali.C2P_M31;	m32 = Cali.C2P_M32;	m33 = Cali.C2P_M33;
	}

	ValX = (PosX*m11+PosY*m12+m13);
	ValY = (PosX*m21+PosY*m22+m23);
	ValW = (PosX*m31+PosY*m32+m33);
	if ( fabs(ValW) > 0.000001 )
	{
		ValX = ValX/ValW;
		ValY = ValY/ValW;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CheckStagePosValidX(double Pos)//確認機台座標有效
{
	const TMotionParameter &MotionParam=GetMotionParameter();
	const double MinPos = MotionParam.m_LimitMinX;
	const double MaxPos = MotionParam.m_LimitMaxX;
	if ( Pos < MinPos ) 
	{
		CString str;
		str = _T("Error, stage is out of X-axis Min Limit");
		str = CMotion_Basic::LoadMultiLanguageString(str, str);
		m_ErrorString.Format(_T("%s (%.0f/%.0f)"), str, Pos, MinPos);
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_LIMIT_RANGE);
		return false;
	}
	if ( Pos > MaxPos ) 
	{
		CString str;
		str = _T("Error, stage is out of X-axis Max Limit");
		str = CMotion_Basic::LoadMultiLanguageString(str, str);
		m_ErrorString.Format(_T("%s (%.0f/%.0f)"), str, Pos, MaxPos);
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_LIMIT_RANGE);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CheckStagePosValidY(double Pos)//確認機台座標有效
{
	const TMotionParameter &MotionParam=GetMotionParameter();
	const double MinPos = MotionParam.m_LimitMinY;
	const double MaxPos = MotionParam.m_LimitMaxY;
	if ( Pos < MinPos ) 
	{
		CString str;
		str = _T("Error, stage is out of Y-axis Min Limit");
		str = CMotion_Basic::LoadMultiLanguageString(str, str);
		m_ErrorString.Format(_T("%s (%.0f/%.0f)"), str, Pos, MinPos);
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_LIMIT_RANGE);
		return false;
	}
	if ( Pos > MaxPos ) 
	{
		CString str;
		str = _T("Error, stage is out of Y-axis Max Limit");
		str = CMotion_Basic::LoadMultiLanguageString(str, str);
		m_ErrorString.Format(_T("%s (%.0f/%.0f)"), str, Pos, MaxPos);
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_LIMIT_RANGE);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::CheckStagePosValidZ(double Pos)//確認機台座標有效	
{
	const TMotionParameter &MotionParam=GetMotionParameter();
	const double MinPos = MotionParam.m_LimitMinZ;
	const double MaxPos = MotionParam.m_LimitMaxZ;
	if ( Pos < MinPos ) 
	{
		CString str;
		str = _T("Error, stage is out of Z-axis Min Limit");
		str = CMotion_Basic::LoadMultiLanguageString(str, str);
		m_ErrorString.Format(_T("%s (%.0f/%.0f)"), str, Pos, MinPos);
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_LIMIT_RANGE);
		return false;
	}
	if ( Pos > MaxPos ) 
	{
		CString str;
		str = _T("Error, stage is out of Z-axis Max Limit");
		str = CMotion_Basic::LoadMultiLanguageString(str, str);
		m_ErrorString.Format(_T("%s (%.0f/%.0f)"), str, Pos, MaxPos);
		SetMotionExceptionCode(AOI_EXCEPTION_MOTION_LIMIT_RANGE);
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::SetFocusPosZ(double PosZ, LANE_ID LaneID)//設定焦距位置
{
	TMotionParameter &MotionParam = GetMotionParameter();		
	switch ( LaneID )
	{
	case LANE_ID_B:	
		MotionParam.m_PCBStopRPosZ_LB = PosZ;	
		MotionParam.m_PCBStopLPosZ_LB = PosZ;	
		MotionParam.m_LaneLedStopRPosZ_LB = PosZ;
		MotionParam.m_LaneLedSlowRPosZ_LB = PosZ;
		MotionParam.m_LaneLedStopLPosZ_LB = PosZ;
		MotionParam.m_LaneLedSlowLPosZ_LB = PosZ;
		break;
	default:		
		MotionParam.m_PCBStopRPosZ_LA = PosZ;	
		MotionParam.m_PCBStopLPosZ_LA = PosZ;	
		MotionParam.m_LaneLedStopRPosZ_LA = PosZ;
		MotionParam.m_LaneLedSlowRPosZ_LA = PosZ;
		MotionParam.m_LaneLedStopLPosZ_LA = PosZ;
		MotionParam.m_LaneLedSlowLPosZ_LA = PosZ;
		break;
	}	
	MotionParam.m_StageStartPosZ = PosZ;
	MotionParam.m_StageLeavePosZ = PosZ;
	MotionParam.m_BeforePCBInPosZ = PosZ;
	//SetMotionExceptionCode(AOI_EXCEPTION_MOTION_SET_FUNC);
	return true;
}
//----------------------------------------------------------------------------------//
bool CMotion_Basic::ModifyORGPosition(double PosX, double PosY, double PosZ)//修改原點位置
{
	TMotionParameter &MotionParam = GetMotionParameter();
	//公式
	//絕對位置=目前位置+目前原點=新的位置+新的原點

	const double NewORGX = PosX;
	const double NewORGY = PosY;
	const double NewORGZ = PosZ;
	const double OldORGX = MotionParam.m_HomeOrgOffsetX;
	const double OldORGY = MotionParam.m_HomeOrgOffsetY;
	const double OldORGZ = MotionParam.m_HomeOrgOffsetZ;
	const double OffsetX = NewORGX-OldORGX;
	const double OffsetY = NewORGY-OldORGY;
	const double OffsetZ = NewORGZ-OldORGZ;

	MotionParam.m_HomeOrgOffsetX = NewORGX;
	MotionParam.m_HomeOrgOffsetY = NewORGY;
	MotionParam.m_HomeOrgOffsetZ = NewORGZ;

	MotionParam.m_StageStartPosX -= OffsetX;
	MotionParam.m_StageStartPosY -= OffsetY;
	MotionParam.m_StageStartPosZ -= OffsetZ;

	MotionParam.m_StageLeavePosX -= OffsetX;
	MotionParam.m_StageLeavePosY -= OffsetY;
	MotionParam.m_StageLeavePosZ -= OffsetZ;

	MotionParam.m_BeforePCBInPosX -= OffsetX;
	MotionParam.m_BeforePCBInPosY -= OffsetY;
	MotionParam.m_BeforePCBInPosZ -= OffsetZ;

	MotionParam.m_PCBStopRPosX_LA -= OffsetX;
	MotionParam.m_PCBStopRPosY_LA -= OffsetY;
	MotionParam.m_PCBStopRPosZ_LA -= OffsetZ;

	MotionParam.m_PCBStopRPosX_LB -= OffsetX;
	MotionParam.m_PCBStopRPosY_LB -= OffsetY;
	MotionParam.m_PCBStopRPosZ_LB -= OffsetZ;

	MotionParam.m_PCBStopLPosX_LA -= OffsetX;
	MotionParam.m_PCBStopLPosY_LA -= OffsetY;
	MotionParam.m_PCBStopLPosZ_LA -= OffsetZ;

	MotionParam.m_PCBStopLPosX_LB -= OffsetX;
	MotionParam.m_PCBStopLPosY_LB -= OffsetY;
	MotionParam.m_PCBStopLPosZ_LB -= OffsetZ;

	MotionParam.m_LimitMaxX -= OffsetX;
	MotionParam.m_LimitMaxY -= OffsetY;
	MotionParam.m_LimitMaxZ -= OffsetZ;

	MotionParam.m_LimitMinX -= OffsetX;
	MotionParam.m_LimitMinY -= OffsetY;
	MotionParam.m_LimitMinZ -= OffsetZ;

	UpdateMotionParameterToMotionAxis();
	//SetMotionExceptionCode(AOI_EXCEPTION_MOTION_SET_FUNC);
	return true;
}
//----------------------------------------------------------------------------------//
int CMotion_Basic::GetAxisSignPositive(int Axis)//取得軸控的軸方向	
{
	int nSign = FN_ENABLE;
	const TMotionParameter &MotionParam = GetMotionParameter();
	switch ( Axis )
	{
	case AXIS_X:	nSign = MotionParam.m_SignPositiveX;	break;
	case AXIS_Y:	nSign = MotionParam.m_SignPositiveY;	break;
	case AXIS_Z:	nSign = MotionParam.m_SignPositiveZ;	break;
	}
	return nSign;
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::AdjustAccValue(double AccVal, double Dist, double RefDist)//調整加減速數值
{
	double NewAccVal=AccVal;
	if ( fabs(RefDist) < 1.0 )
	{	return NewAccVal; }

	if ( Dist >= RefDist )
	{	return NewAccVal; }
	
	double Ratio = Dist/RefDist;
	double RatioUsed = Ratio;
	//double RatioUsed = sqrt(Ratio);
	double AccVal2 = AccVal*RatioUsed;
	NewAccVal = MAX(0.01*AccVal, AccVal2);		
	return NewAccVal;
}
//----------------------------------------------------------------------------------//
double CMotion_Basic::AdjustAccTime(double AccTime, double AccMinTime, ACC_TIME_ADJUST_MODE Mode)//調整加減速時間
{	
	//關閉
	if ( ACC_TIME_ADJUST_OFF == Mode )
	{	return AccTime; }
	
	//固定時間(一律以使用者設定時間為主)
	if ( ACC_TIME_ADJUST_FIX_T == Mode )
	{	return AccMinTime; }

	//最小時間(低於最小時間就以最小時間為主)
	if ( ACC_TIME_ADJUST_MIN_T == Mode )
	{	
		if ( AccTime < AccMinTime )
		{	return AccMinTime; }
		return AccTime; 
	}

	//Gamma修正(低於最小時間就以Gamma時間修正)
	if ( ACC_TIME_ADJUST_GAMMA == Mode )
	{
		const double Gamma=0.25;
		const double AccMaxTime=AccMinTime*1.0;//sec
		if ( AccTime < AccMaxTime )
		{	
			const double dRatio=AccTime/AccMaxTime;
			const double dPower=pow(dRatio, Gamma);
			const double dTacc2=dPower*AccMaxTime;			
			return dTacc2;
		}
		return AccTime;
	}
	return AccTime;
}
//----------------------------------------------------------------------------------//