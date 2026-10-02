/*****************************************************************************/
//
// 	Motion Controller API
//
//	Copyright (c) 2018, JET Tech.  All rights reserved.
//
//  Version 1.0
//
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" WITHOUT WARRANTY OF ANY KIND. NO WARRANTIES, EITHER EXPRESS
//  OR IMPLIED, ARE MADE WITH RESPECT TO THE SOFTWARE, INCLUDING, BUT 
//  NOT LIMITED TO, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS 
//  FOR A PARTICULAR PURPOSE, TITLE OR NON-INFRINGEMENT, OR ANY OTHER 
//  WARRANTIES THAT MAY ARISE FROM USAGE OF TRADE OR COURSE OF DEALING. 
//  THE COPYRIGHT HOLDERS AND CONTRIBUTORS DO NOT WARRANT, GUARANTEE, OR 
//  MAKE ANY REPRESENTATIONS REGARDING THE USE OF OR THE RESULTS OF THE 
//  USE OF THE SOFTWARE IN TERMS OF CORRECTNESS, ACCURACY, RELIABILITY, 
//  OR OTHERWISE AND DO NOT WARRANT THAT THE OPERATION OF THE SOFTWARE 
//  WILL BE UNINTERRUPTED OR ERROR FREE.  THE ENTIRE RISK AS TO THE 
//  PERFORMANCE OF THE SOFTWARE IS WITH YOU. IN NO EVENT SHALL THE 
//  COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
//  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
//  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
//  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
//  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
//  THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF ADVISED OF THE 
//  POSSIBILITY OF SUCH DAMAGE.
//
/*****************************************************************************/

#ifndef _MOUFUNCTIONS_H_
#define _MOUFUNCTIONS_H_

#include <string>
#include "MotionDataStructure.h"

/*****************************************************************************
Exports
*****************************************************************************/

#ifdef MOU_EXPORTS
    #define MOU_API __declspec(dllexport)
#else
	#ifdef MOU_EXTERN
	   #define MOU_API extern
	#else
	   #define MOU_API __declspec(dllimport)
	#endif
#endif

#define MOU_MAX_MOTIONCONTROLLERS   2

/*****************************************************************************
Definitions and types 
*****************************************************************************/
typedef enum 
{
	MOU_AXIS_X = 0x01, //0
	MOU_AXIS_Y = 0x02, //1
	MOU_AXIS_Z = 0x04, //2
	MOU_AXIS_T = 0x08, //3
	MOU_AXIS_A = 0x10, //4
	MOU_AXIS_B = 0x20, //5
	MOU_AXIS_C = 0x40, //6
	MOU_AXIS_D = 0x80, //7	
} MOU_AXIS;

const int MOU_NUMBER_OF_AXES = 8;

typedef enum 
{
	MOU_AXIS_FORWARD = 1,
	MOU_AXIS_BACKWARD = -1
} MOU_AXIS_DIRECTION;

typedef enum
{
	MOU_T_CURVE,
	MOU_S_CURVE
}MOU_MOVING_CURVE;

typedef unsigned int MOU_AXIS_STATUS_IO;
//for io
#define MOU_AXIS_READY						(0x00000001)
#define MOU_AXIS_ALARM						(MOU_AXIS_READY<<1)
#define MOU_AXIS_FORWARD_LIMITSWITCH		(MOU_AXIS_ALARM<<1)
#define MOU_AXIS_BACKWARD_LIMITSWITCH		(MOU_AXIS_FORWARD_LIMITSWITCH<<1)
#define MOU_AXIS_IN_ORG						(MOU_AXIS_BACKWARD_LIMITSWITCH<<1)
#define MOU_AXIS_DIR						(MOU_AXIS_IN_ORG<<1)			//-Reserved
#define MOU_AXIS_EMERGENCY_STOP				(MOU_AXIS_DIR<<1)
#define MOU_AXIS_PCS						(MOU_AXIS_EMERGENCY_STOP<<1)	//-Reserved
#define MOU_AXIS_ERC						(MOU_AXIS_PCS<<1)				
#define MOU_AXIS_EZ							(MOU_AXIS_ERC<<1)				//-Reserved
#define MOU_AXIS_CLR						(MOU_AXIS_EZ<<1)				//-Reserved
#define MOU_AXIS_LATCH						(MOU_AXIS_CLR<<1)				//-Reserved
#define MOU_AXIS_SLOW_DOWN					(MOU_AXIS_LATCH<<1)				//-Reserved
#define MOU_AXIS_IN_POSITION				(MOU_AXIS_SLOW_DOWN<<1)
#define MOU_AXIS_SERVO_ON					(MOU_AXIS_IN_POSITION<<1)
#define MOU_AXIS_RALM						(MOU_AXIS_SERVO_ON<<1)

//
typedef unsigned int MOU_AXIS_STATUS_MT;
//for motion
#define MOU_AXIS_STOP					(0x00000001)
#define MOU_AXIS_OVER_CURRENT			(MOU_AXIS_STOP<<1)
#define MOU_AXIS_WAIT_CSTA				(MOU_AXIS_OVER_CURRENT<<1)		//Wait CSTA (Synchronous start signal)
#define MOU_AXIS_WAIT_IN_SYN_SNG		(MOU_AXIS_WAIT_CSTA<<1)			//"Wait Internal sync. signal
#define MOU_AXIS_WAITING_IN_SYN_SNG		(MOU_AXIS_WAIT_IN_SYN_SNG<<1)	//Waiting for internal synchronization signal
#define MOU_AXIS_WAIT_ERC_FNH			(MOU_AXIS_WAITING_IN_SYN_SNG<<1)//Wait ERC finished
#define MOU_AXIS_WAIT_DIR_CHG			(MOU_AXIS_WAIT_ERC_FNH<<1)		//Wait DIR Change
#define MOU_AXIS_BACKlash_COMP			(MOU_AXIS_WAIT_DIR_CHG<<1)		//Backlash compensating
#define MOU_AXIS_WAIT_PA_PB				(MOU_AXIS_BACKlash_COMP<<1)		//Wait PA/PB
#define MOU_AXIS_IN_HOME_SPD_MOTION		(MOU_AXIS_WAIT_PA_PB<<1)		//In home special speed motion
#define MOU_AXIS_IN_START_VEL_MOTION	(MOU_AXIS_IN_HOME_SPD_MOTION<<1)//In start velocity motion
#define MOU_AXIS_IN_ACCLERATION			(MOU_AXIS_IN_START_VEL_MOTION<<1)//In acceleration
#define MOU_AXIS_IN_MAX_VEL_MOTION		(MOU_AXIS_IN_ACCLERATION<<1)	//In Max velocity motion
#define MOU_AXIS_IN_DECELERATION		(MOU_AXIS_IN_MAX_VEL_MOTION<<1)	//In deceleration
#define MOU_AXIS_WAIT_INP				(MOU_AXIS_IN_DECELERATION<<1)	//Wait INP		

/*****************************************************************************
Handle Codes
*****************************************************************************/
typedef long MOU_HANDLE;

#define MOU_HANDLE_MASK				0x0FFFFFFF
#define MOU_HANDLE_ACS				0x10000000
#define MOU_HANDLE_ADLINK           0x20000000
#define MOU_HANDLE_TPM           	0x40000000			//TAIWAN PLUS MOTION
#define MOU_HANDLE_SYN_TEK         	0x80000000			//Syn tek

/*****************************************************************************
Error Codes
*****************************************************************************/
typedef int MOU_ERROR;

enum MouErrorCode
{
	MOU_ERR_SUCCESS =	 0,
	MOU_ERR_FAILED,
	MOU_ERR_NOT_IMPLEMENTED,
	MOU_ERR_DEVICE_NOT_OPEN,
	MOU_ERR_INVALID_HANDLE,
	MOU_ERR_AXIS_INVALID,
	MOU_ERR_INITIALIZE_FAIL,
	MOU_ERR_MODULE_TYPE_FAIL,
	MOU_ERR_COMMAND_INVALID,
	MOU_ERR_COMMAND_PARAMETER_INVALID,
	MOU_ERR_COMMAND_TIME_OUT,
	MOU_ERR_LOAD_CFG_FILE_FAIL,
	MOU_ERR_INIT_FAILED_AXIS_X,
	MOU_ERR_INIT_FAILED_AXIS_Y,
	MOU_ERR_INIT_FAILED_AXIS_Z,
	MOU_ERR_INIT_FAILED_AXIS_T,
	MOU_ERR_INIT_FAILED_AXIS_A,
	MOU_ERR_INIT_FAILED_AXIS_B,
	MOU_ERR_INIT_FAILED_AXIS_C,
	MOU_ERR_INIT_FAILED_AXIS_D,	
	MOU_ERR_ENABEL_FAILED_AXIS_X,
	MOU_ERR_ENABEL_FAILED_AXIS_Y,
	MOU_ERR_ENABEL_FAILED_AXIS_Z,
	MOU_ERR_ENABEL_FAILED_AXIS_T,
	MOU_ERR_ENABEL_FAILED_AXIS_A,
	MOU_ERR_ENABEL_FAILED_AXIS_B,
	MOU_ERR_ENABEL_FAILED_AXIS_C,
	MOU_ERR_ENABEL_FAILED_AXIS_D,	
	MOU_ERR_DRIVER_ALARM_AXIS_X,
	MOU_ERR_DRIVER_ALARM_AXIS_Y,
	MOU_ERR_DRIVER_ALARM_AXIS_Z,
	MOU_ERR_DRIVER_ALARM_AXIS_T,
	MOU_ERR_DRIVER_ALARM_AXIS_A,
	MOU_ERR_DRIVER_ALARM_AXIS_B,
	MOU_ERR_DRIVER_ALARM_AXIS_C,
	MOU_ERR_DRIVER_ALARM_AXIS_D,
	MOU_ERR_ENCODER_FAILED_AXIS_X,
	MOU_ERR_ENCODER_FAILED_AXIS_Y,
	MOU_ERR_ENCODER_FAILED_AXIS_Z,
	MOU_ERR_ENCODER_FAILED_AXIS_T,
	MOU_ERR_ENCODER_FAILED_AXIS_A,
	MOU_ERR_ENCODER_FAILED_AXIS_B,
	MOU_ERR_ENCODER_FAILED_AXIS_C,
	MOU_ERR_ENCODER_FAILED_AXIS_D	
};

const int g_knMouModuleNameLen = 256;

/*****************************************************************************
API
*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*********** General API  Functions  ************/
MOU_API MOU_ERROR mouGetDllVersion(unsigned long* major, unsigned long* minjor, unsigned long* build);
MOU_API MOU_ERROR mouOpen(const wchar_t * InitString, MOU_HANDLE* Handle);
  
/*********** Basic  Functions ************/
MOU_API MOU_ERROR mouInitialize(MOU_HANDLE Handle);
MOU_API MOU_ERROR mouUninitialize(MOU_HANDLE Handle);
MOU_API MOU_ERROR mouGetModuleName(MOU_HANDLE Handle, char pStr[g_knMouModuleNameLen]);
  
//
MOU_API MOU_ERROR mouGetStatusIO(MOU_HANDLE Handle,MOU_AXIS Axis,MOU_AXIS_STATUS_IO &Status);
MOU_API MOU_ERROR mouGetStatusMT(MOU_HANDLE Handle,MOU_AXIS Axis,MOU_AXIS_STATUS_MT &Status);

MOU_API MOU_ERROR mouIsBusy(MOU_HANDLE Handle,MOU_AXIS Axis, bool &Result);
  
MOU_API MOU_ERROR mouExecHome(MOU_HANDLE Handle,MOU_AXIS Axis);
MOU_API MOU_ERROR mouExecHomeEx(MOU_HANDLE Handle,MOU_AXIS Axis, MOU_AXIS_DIRECTION SearchDir);
MOU_API MOU_ERROR mouEnable(MOU_HANDLE Handle, MOU_AXIS Axis);
MOU_API MOU_ERROR mouDisable(MOU_HANDLE Handle, MOU_AXIS Axis);

//
MOU_API MOU_ERROR mouSetCurveMode(MOU_HANDLE Handle, MOU_AXIS Axis, MOU_MOVING_CURVE Mode);
MOU_API MOU_ERROR mouGetCurveMode(MOU_HANDLE Handle, MOU_AXIS Axis, MOU_MOVING_CURVE &Mode);

MOU_API MOU_ERROR mouResetCommandPosition(MOU_HANDLE Handle, MOU_AXIS Axis);
MOU_API MOU_ERROR mouSetORG(MOU_HANDLE Handle,MOU_AXIS Axis);  
MOU_API MOU_ERROR mouResetAlarm(MOU_HANDLE Handle,MOU_AXIS Axis);
  
//
MOU_API MOU_ERROR mouSetSWLimit(MOU_HANDLE Handle,MOU_AXIS Axis, double MaxPos, double MinPos);
MOU_API MOU_ERROR mouGetSWLimit(MOU_HANDLE Handle,MOU_AXIS Axis, double &MaxPos, double &MinPos);
  
MOU_API MOU_ERROR mouEnableSWLimit(MOU_HANDLE Handle,MOU_AXIS Axis);
MOU_API MOU_ERROR mouDisableSWLimit(MOU_HANDLE Handle,MOU_AXIS Axis);
  
MOU_API MOU_ERROR mouEnableINP(MOU_HANDLE Handle,MOU_AXIS Axis);
MOU_API MOU_ERROR mouDisableINP(MOU_HANDLE Handle,MOU_AXIS Axis);
  
MOU_API MOU_ERROR mouWaitForEndOfMove(MOU_HANDLE Handle,MOU_AXIS Axis);
  
MOU_API MOU_ERROR mouStartJog(MOU_HANDLE Handle,MOU_AXIS Axis,MOU_AXIS_DIRECTION Direction, double Speed);
MOU_API MOU_ERROR mouStopJog(MOU_HANDLE Handle,MOU_AXIS Axis);
  
//
MOU_API MOU_ERROR mouMoveRelative(MOU_HANDLE Handle,MOU_AXIS Axis,double Speed, double Distance);
MOU_API MOU_ERROR mouMoveAbsolute(MOU_HANDLE Handle,MOU_AXIS Axis,double Speed, double Position);
MOU_API MOU_ERROR mouStopMotion(MOU_HANDLE Handle,MOU_AXIS Axis);
  
MOU_API MOU_ERROR mouSetAccDecTime(MOU_HANDLE Handle,MOU_AXIS Axis,double Acceleration,double Deceleration);
MOU_API MOU_ERROR mouGetAccDecTime(MOU_HANDLE Handle,MOU_AXIS Axis,double &Acceleration,double &Deceleration);

//for S-Curve parameters
MOU_API MOU_ERROR mouSetSVaccVdec(MOU_HANDLE Handle,MOU_AXIS Axis,double SVacc, double SVdec);
MOU_API MOU_ERROR mouGetSVaccVdec(MOU_HANDLE Handle,MOU_AXIS Axis,double &SVacc, double &SVdec);
  
MOU_API MOU_ERROR mouGetPosition(MOU_HANDLE Handle,MOU_AXIS Axis,double &Position);
  
MOU_API MOU_ERROR mouSetPEG(MOU_HANDLE Handle,MOU_AXIS Axis, const PEG_Data &Data);
MOU_API MOU_ERROR mouGetPEG(MOU_HANDLE Handle,MOU_AXIS Axis,PEG_Data &Data);
MOU_API MOU_ERROR mouEnablePEG(MOU_HANDLE Handle,MOU_AXIS Axis);
MOU_API MOU_ERROR mouDisablePEG(MOU_HANDLE Handle,MOU_AXIS Axis);
  
MOU_API MOU_ERROR mouSetHomeData(MOU_HANDLE Handle,MOU_AXIS Axis, const HOME_Data &Data);
MOU_API MOU_ERROR mouGetHomeData(MOU_HANDLE Handle,MOU_AXIS Axis,HOME_Data &Data);

MOU_API MOU_ERROR mouSetJogData(MOU_HANDLE Handle,MOU_AXIS Axis, const JOG_Data &Data);
MOU_API MOU_ERROR mouGetJogData(MOU_HANDLE Handle,MOU_AXIS Axis,JOG_Data &Data);
  
//For log command (ex. LOG_ON/LOG_OFF) or other command
MOU_API MOU_ERROR mouSendCommand(MOU_HANDLE Handle, char *pCmd, int CmdLen, char *pResponse, int *ResLen);
//Log On, Log Off, LogDir C:\ABC, SaveINI

#ifdef __cplusplus
}
#endif

#endif