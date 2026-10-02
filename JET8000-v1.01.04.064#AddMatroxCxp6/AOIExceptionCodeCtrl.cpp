// AOIExceptionCodeCtrl.cpp: implementation of the CAOIExceptionCodeCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AOIExceptionCodeCtrl.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#define MAX_AOI_EXCEPTION_CODE_COUNT     16
//-------------------------------------------------------------------------------------//
CAOIExceptionCodeCtrl AOIExceptionCodeCtrl;
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CAOIExceptionCodeCtrl::CAOIExceptionCodeCtrl()
{
	PreInitExceptionCode();
	InitialExceptionCode();
}
//-------------------------------------------------------------------------------------//
CAOIExceptionCodeCtrl::CAOIExceptionCodeCtrl(const CAOIExceptionCodeCtrl &other)
{
	PreInitExceptionCode();
	CloneExceptionCode(other);
}
//-------------------------------------------------------------------------------------//
CAOIExceptionCodeCtrl::~CAOIExceptionCodeCtrl()
{
	::DeleteCriticalSection(&m_csExceptionCode);
}
//-------------------------------------------------------------------------------------//
CAOIExceptionCodeCtrl& CAOIExceptionCodeCtrl::operator=(const CAOIExceptionCodeCtrl &other)
{
	if ( this == &other ) { return *this; }
	CloneExceptionCode(other);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CAOIExceptionCodeCtrl::PreInitExceptionCode()
{
	m_LogIdx = -1;
	::InitializeCriticalSection(&m_csExceptionCode);
}
//-------------------------------------------------------------------------------------//
void CAOIExceptionCodeCtrl::InitialExceptionCode()
{
	m_ExceptionCodeRunning = false;
}
//-------------------------------------------------------------------------------------//
void CAOIExceptionCodeCtrl::CloneExceptionCode(const CAOIExceptionCodeCtrl &other)
{
	m_ErrorString = other.m_ErrorString;
	m_LastExceptionTxt = other.m_LastExceptionTxt;
	m_LastExceptionCode = other.m_LastExceptionCode;	
	m_ExceptionCodeRunning = other.m_ExceptionCodeRunning;
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::ReturnNoExceptionCode(DWORD Code)
{
	AOI_EXCEPTION_CODE Code2=(AOI_EXCEPTION_CODE)(Code);
	CString Str = GetExceptionCodeText(Code2);
	m_ErrorString.Format(_T("Error, No Define Exception Code[%d]"), Code);
	SaveAOIExceptionCodeLogFile(Code2, Str, m_ErrorString);
	return false;
}
//-------------------------------------------------------------------------------------//
void CAOIExceptionCodeCtrl::LockExceptionCode()
{
	::EnterCriticalSection(&m_csExceptionCode);
}
//-------------------------------------------------------------------------------------//
void CAOIExceptionCodeCtrl::UnlockExceptionCode()
{
	::LeaveCriticalSection(&m_csExceptionCode);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::RegisterAOIExceptionLogFile()
{
	CString LogFolder;
	CString LogBackup;	
	const bool bSaveHeader=true;
	CString LogExtName = _T("TXT");		
	CString LogFilename = _T("ExceptionCode");
#ifndef OFFLINE_VERSION
	LogFilename = _T("ExceptionCode_Online");
#else
	LogFilename = _T("ExceptionCode_Offline");
#endif//OFFLINE_VERSION
	LogFolder.Format(_T("%s\\%s"), AOIDataCollect.GetAOILogDirectory(), _T("ExceptionCode"));	
	::CreateDirectory(LogFolder, NULL);
	//LogBackup.Format(_T("[%s]-[%s]"), HostIP, Vendor);
	m_LogIdx = LogManager.AddLogFile(LogFolder, LogFilename, LogExtName, LogBackup, CLogNode::LOG_FILENAME_BY_DATE, bSaveHeader);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SaveAOIExceptionCodeLogFile(LPCTSTR Message)
{
	if ( -1 == m_LogIdx )
	{	return false; }
	if ( LogManager.AddLogMessage(m_LogIdx, Message) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SaveAOIExceptionCodeLogFile(AOI_EXCEPTION_CODE Code, LPCTSTR Str, LPCTSTR Err)
{
	CString Message;
	if ( NULL == Err )
	{	Message.Format(_T("[%06d]#%s Exception"), Code, Str);	}
	else
	{	Message.Format(_T("[%06d]#%s Exception [%s]"), Code, Str, Err);	}
	if ( SaveAOIExceptionCodeLogFile(Message) == false )	
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
//void CAOIExceptionCodeCtrl::SetErrorString(LPCTSTR Error)
//{
//	m_ErrorString = Error;
//}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetExceptionCode(DWORD Code, LPCTSTR Err)
{
	AOI_EXCEPTION_CODE Code2=(AOI_EXCEPTION_CODE)(Code);
	AddAOIExceptionCode(Code2, Err, true);
	return true;
}
//-------------------------------------------------------------------------------------//
CString CAOIExceptionCodeCtrl::GetExceptionCodeText(AOI_EXCEPTION_CODE Code) const
{
	CString str;
	switch ( Code )
	{
	case AOI_EXCEPTION_NONE: str=_T("OK"); break;
	case AOI_EXCEPTION_OTHERS: str=_T("Others"); break;

	case AOI_EXCEPTION_MEMORY_BEGIN: str=_T("Memory Others"); break;
	case AOI_EXCEPTION_MEMORY_ALLOC: str=_T("Memory Alloc"); break;
	case AOI_EXCEPTION_MEMORY_FREE: str=_T("Memory Free"); break;
	case AOI_EXCEPTION_MEMORY_OTHERS: str=_T("Memory Others"); break;

	case AOI_EXCEPTION_OBJECT_BEGIN: str=_T("Object Others"); break;
	case AOI_EXCEPTION_OBJECT_CREATE: str=_T("Object Create"); break;
	case AOI_EXCEPTION_OBJECT_DELETE: str=_T("Object Delete"); break;
	case AOI_EXCEPTION_OBJECT_OTHERS: str=_T("Object Others"); break;

	case AOI_EXCEPTION_CAMERA_BEGIN: str=_T("Camera Others"); break;	
	case AOI_EXCEPTION_CAMERA_PARAM: str=_T("Camera Parameter"); break;
	case AOI_EXCEPTION_CAMERA_SET_FUNC: str=_T("Camera Set Func"); break;
	case AOI_EXCEPTION_CAMERA_GET_FUNC: str=_T("Camera Get Func"); break;
	case AOI_EXCEPTION_CAMERA_EXEC_FUNC: str=_T("Camera Exec Func"); break;	
	//case AOI_EXCEPTION_CAMERA_FILE_READ: str=_T("Camera File Read"); break;
	//case AOI_EXCEPTION_CAMERA_FILE_WRITE: str=_T("Camera File Write"); break;
	case AOI_EXCEPTION_CAMERA_CONNECT: str=_T("Camera Connect"); break;
	case AOI_EXCEPTION_CAMERA_RELEASE: str=_T("Camera Release"); break;
	case AOI_EXCEPTION_CAMERA_RESET: str=_T("Camera Reset"); break;
	case AOI_EXCEPTION_CAMERA_GRAB_STOP: str=_T("Camera Stop Grabbing"); break;
	case AOI_EXCEPTION_CAMERA_GRAB_START: str=_T("Camera Start to Grab"); break;
	case AOI_EXCEPTION_CAMERA_GRAB_MODE: str=_T("Camera Set Grab Mode"); break;	         
	case AOI_EXCEPTION_CAMERA_EXP_TIME_FUNC: str=_T("Camera Exposure Time Func"); break;
	case AOI_EXCEPTION_CAMERA_SOFT_TRIG_FUNC: str=_T("Camera Software Trigger Func"); break;	
	case AOI_EXCEPTION_CAMERA_READY_TRIG_FUNC: str=_T("Camera Ready to Trigger Func"); break;
	case AOI_EXCEPTION_CAMERA_WHITE_BALANCE_FUNC: str=_T("Camera White Balance Func"); break;
	case AOI_EXCEPTION_CAMERA_RING_BUFFER: str=_T("Camera Ring Buffer Func"); break;	
	case AOI_EXCEPTION_CAMERA_EXP_DONE_FUNC: str=_T("Camera Exposure Finish Func"); break;
	case AOI_EXCEPTION_CAMERA_GRAB_DONE_FUNC: str=_T("Camera Grab Finish Func"); break;
	case AOI_EXCEPTION_CAMERA_EXEC_CALI_FUNC: str=_T("Camera Call Calibration Func"); break;	
	case AOI_EXCEPTION_CAMERA_OTHERS: str=_T("Camera Others"); break;

	case AOI_EXCEPTION_MOTION_BEGIN: str=_T("Motion Others"); break;	
	case AOI_EXCEPTION_MOTION_PARAM: str=_T("Motion Parameter"); break;
	case AOI_EXCEPTION_MOTION_SET_FUNC: str=_T("Motion Set Func"); break;
	case AOI_EXCEPTION_MOTION_GET_FUNC: str=_T("Motion Get Func"); break;
	case AOI_EXCEPTION_MOTION_EXEC_FUNC: str=_T("Motion Exec Func"); break;	
	//case AOI_EXCEPTION_MOTION_FILE_READ: str=_T("Motion File Read"); break;
	//case AOI_EXCEPTION_MOTION_FILE_WRITE: str=_T("Motion File Write"); break;
	case AOI_EXCEPTION_MOTION_CONNECT: str=_T("Motion Connect"); break;
	case AOI_EXCEPTION_MOTION_RELEASE: str=_T("Motion Release"); break;	
	case AOI_EXCEPTION_MOTION_HOME: str=_T("Motion Home"); break;
	case AOI_EXCEPTION_MOTION_MOVE: str=_T("Motion Move"); break;
	case AOI_EXCEPTION_MOTION_MOVE_DONE: str=_T("Motion Move Done"); break;
	case AOI_EXCEPTION_MOTION_FAULT_ACK: str=_T("Motion Fault Ack"); break;
	case AOI_EXCEPTION_MOTION_ENABLE: str=_T("Motion Enable"); break;
	case AOI_EXCEPTION_MOTION_DISABLE: str=_T("Motion Disable"); break;
	case AOI_EXCEPTION_MOTION_INP_FUNC: str=_T("Motion In-Position Func"); break;	
	case AOI_EXCEPTION_MOTION_READ_ENCODE: str=_T("Motion Read Encode"); break;
	case AOI_EXCEPTION_MOTION_JOG_FUNC: str=_T("Motion Jog Func"); break;	
	case AOI_EXCEPTION_MOTION_GANTRY_FUNC: str=_T("Motion Gantry Func"); break;	
	case AOI_EXCEPTION_MOTION_LIMIT_CALI: str=_T("Motion Calibrate Limit"); break;	
	case AOI_EXCEPTION_MOTION_LIMIT_RANGE: str=_T("Motion Limit Range"); break;	
	case AOI_EXCEPTION_MOTION_NOT_READY: str=_T("Motion Not Ready"); break;	
	case AOI_EXCEPTION_MOTION_PARAM_XY_CALI: str=_T("Motion XY Calibration Param"); break;
	case AOI_EXCEPTION_MOTION_OTHERS: str=_T("Motion Others"); break;

	case AOI_EXCEPTION_PLC_BEGIN: str=_T("PLC Others"); break;	
	case AOI_EXCEPTION_PLC_PARAM: str=_T("PLC Parameter"); break;
	case AOI_EXCEPTION_PLC_SET_FUNC: str=_T("PLC Set Func"); break;
	case AOI_EXCEPTION_PLC_GET_FUNC: str=_T("PLC Get Func"); break;
	case AOI_EXCEPTION_PLC_EXEC_FUNC: str=_T("PLC Exec Func"); break;
	case AOI_EXCEPTION_PLC_WRITE_NODE: str=_T("PLC Write Node"); break;
	case AOI_EXCEPTION_PLC_READ_NODE: str=_T("PLC Read Node"); break;
	//case AOI_EXCEPTION_PLC_FILE_READ: str=_T("PLC File Read"); break;
	//case AOI_EXCEPTION_PLC_FILE_WRITE: str=_T("PLC File Write"); break;
	case AOI_EXCEPTION_PLC_CONNECT: str=_T("PLC Connect"); break;
	case AOI_EXCEPTION_PLC_EMERGENCY: str=_T("PLC Emergency"); break;
	case AOI_EXCEPTION_PLC_KEY_SWITCH: str=_T("PLC Key Switch"); break;
	case AOI_EXCEPTION_PLC_SAFTY_ALARM: str=_T("PLC Safty Alarm"); break;
	case AOI_EXCEPTION_PLC_OVER_HEAT: str=_T("PLC Over Heat"); break;
	case AOI_EXCEPTION_PLC_FAN_ALARM: str=_T("PLC Fan Alarm"); break;
	case AOI_EXCEPTION_PLC_DOOR_OPEN: str=_T("PLC Door Open"); break;
	case AOI_EXCEPTION_PLC_AIR_LOST: str=_T("PLC Air Lost"); break;
	case AOI_EXCEPTION_PLC_PCB_IN_LA: str=_T("PLC PCB-In Lane A"); break;
	case AOI_EXCEPTION_PLC_PCB_OUT_LA: str=_T("PLC PCB-Out Lane A"); break;
	case AOI_EXCEPTION_PLC_PCB_BACK_LA: str=_T("PLC PCB-Back Lane A"); break;
	case AOI_EXCEPTION_PLC_PCB_OUT_IN_LA: str=_T("PLC PCB-Out-In Lane A"); break;
	case AOI_EXCEPTION_PLC_PCB_IN_OUT_LA: str=_T("PLC PCB Auto In-Out Lane A"); break;
	case AOI_EXCEPTION_PLC_PCB_IN_BACK_LA: str=_T("PLC PCB Auto In-Back Lane A"); break;
	case AOI_EXCEPTION_PLC_PCB_CLEAR_LA: str=_T("PLC PCB-Clear Lane A"); break;
	case AOI_EXCEPTION_PLC_PCB_BARGE_IN_LA: str=_T("PLC PCB-Barge-In Lane A"); break;
	case AOI_EXCEPTION_PLC_PCB_BACK_OUT_LA: str=_T("PLC PCB-Back-Out Lane A"); break;
	case AOI_EXCEPTION_PLC_LANE_ADJUST_HOME_LA: str=_T("PLC Lane Adjust Home Lane A"); break;
	case AOI_EXCEPTION_PLC_LANE_ADJUST_MOVE_LA: str=_T("PLC Lane Adjust Move Lane A"); break;
	case AOI_EXCEPTION_PLC_PCB_IN_LB: str=_T("PLC PCB-In Lane B"); break;
	case AOI_EXCEPTION_PLC_PCB_OUT_LB: str=_T("PLC PCB-Out Lane B"); break;
	case AOI_EXCEPTION_PLC_PCB_BACK_LB: str=_T("PLC PCB-Back Lane B"); break;
	case AOI_EXCEPTION_PLC_PCB_OUT_IN_LB: str=_T("PLC PCB-Out-In Lane B"); break;
	case AOI_EXCEPTION_PLC_PCB_IN_OUT_LB: str=_T("PLC PCB Auto In-Out Lane B"); break;
	case AOI_EXCEPTION_PLC_PCB_IN_BACK_LB: str=_T("PLC PCB Auto In-Back Lane B"); break;
	case AOI_EXCEPTION_PLC_PCB_CLEAR_LB: str=_T("PLC PCB-Clear Lane B"); break;
	case AOI_EXCEPTION_PLC_PCB_BARGE_IN_LB: str=_T("PLC PCB-Barge-In Lane B"); break;
	case AOI_EXCEPTION_PLC_PCB_BACK_OUT_LB: str=_T("PLC PCB-Back-Out Lane B"); break;
	case AOI_EXCEPTION_PLC_LANE_ADJUST_HOME_LB: str=_T("PLC Lane Adjust Home Lane B"); break;
	case AOI_EXCEPTION_PLC_LANE_ADJUST_MOVE_LB: str=_T("PLC Lane Adjust Move Lane B"); break;
	case AOI_EXCEPTION_PLC_OTHERS: str=_T("PLC Others"); break;	

	case AOI_EXCEPTION_LIGHT_CTRL_BEGIN: str=_T("Light Ctrl Board Others"); break;	
	case AOI_EXCEPTION_LIGHT_CTRL_PARAM: str=_T("Light Ctrl Board Parameter"); break;
	case AOI_EXCEPTION_LIGHT_CTRL_SET_FUNC: str=_T("Light Ctrl Board Set Func"); break;
	case AOI_EXCEPTION_LIGHT_CTRL_GET_FUNC: str=_T("Light Ctrl Board GetFunc"); break;
	case AOI_EXCEPTION_LIGHT_CTRL_EXEC_FUNC: str=_T("Light Ctrl Board Exec Func"); break;
	case AOI_EXCEPTION_LIGHT_CTRL_READ: str=_T("Light Ctrl Board Read Data"); break;
	case AOI_EXCEPTION_LIGHT_CTRL_WRITE: str=_T("Light Ctrl Board Write Data"); break;
	//case AOI_EXCEPTION_LIGHT_CTRL_FILE_READ: str=_T("Light Ctrl Board File Read"); break;
	//case AOI_EXCEPTION_LIGHT_CTRL_FILE_WRITE: str=_T("Light Ctrl Board File Write"); break;	
	case AOI_EXCEPTION_LIGHT_CTRL_CONNECT: str=_T("Light Ctrl Board Connect"); break;
	case AOI_EXCEPTION_LIGHT_CTRL_RELEASE: str=_T("Light Ctrl Board Release"); break;
	case AOI_EXCEPTION_LIGHT_CTRL_CLEAR: str=_T("Light Ctrl Board Clear"); break;
	case AOI_EXCEPTION_LIGHT_CTRL_TRIGGER: str=_T("Light Ctrl Board Trigger"); break;
	case AOI_EXCEPTION_LIGHT_CTRL_LOOP_BACK: str=_T("Light Ctrl Board Loop Back"); break;
	case AOI_EXCEPTION_LIGHT_CTRL_TABLE_READ: str=_T("Light Ctrl Board Table Read"); break;
	case AOI_EXCEPTION_LIGHT_CTRL_TABLE_WRITE: str=_T("Light Ctrl Board Table Write"); break;
	case AOI_EXCEPTION_LIGHT_CTRL_TABLE_CONVERT: str=_T("Light Ctrl Board Table Convert"); break;
	case AOI_EXCEPTION_LIGHT_CTRL_TABLE_COMPARE: str=_T("Light Ctrl Board Table Compare"); break;	          
	case AOI_EXCEPTION_LIGHT_CTRL_CONVEYOR_FUNC: str=_T("Light Ctrl Board Conveyor Func"); break;
	case AOI_EXCEPTION_LIGHT_CTRL_OTHERS: str=_T("Light Ctrl Board Others"); break;	

	case AOI_EXCEPTION_DLP_CTRL_BEGIN: str=_T("DLP Others"); break;	
	case AOI_EXCEPTION_DLP_CTRL_PARAM: str=_T("DLP Param"); break;
	case AOI_EXCEPTION_DLP_CTRL_SET_FUNC: str=_T("DLP Set Func"); break;
	case AOI_EXCEPTION_DLP_CTRL_GET_FUNC: str=_T("DLP Get Func"); break;
	case AOI_EXCEPTION_DLP_CTRL_EXEC_FUNC: str=_T("DLP Exec Func"); break;
	case AOI_EXCEPTION_DLP_CTRL_READ: str=_T("DLP Read Data"); break;
	case AOI_EXCEPTION_DLP_CTRL_WRITE: str=_T("DLP Write Data"); break;
	//case AOI_EXCEPTION_DLP_CTRL_FILE_READ: str=_T("DLP File Read"); break;
	//case AOI_EXCEPTION_DLP_CTRL_FILE_WRITE: str=_T("DLP File Write"); break;
	case AOI_EXCEPTION_DLP_CTRL_CONNECT: str=_T("DLP Connect"); break;
	case AOI_EXCEPTION_DLP_CTRL_DISCONNECT: str=_T("DLP Disconnect"); break;
	case AOI_EXCEPTION_DLP_CTRL_RESET: str=_T("DLP Reset"); break;
	case AOI_EXCEPTION_DLP_CTRL_LIGHT_SETTING: str=_T("DLP Light Setting"); break;
	case AOI_EXCEPTION_DLP_CTRL_PATTERN_PLAY: str=_T("DLP Pattern Play"); break;
	case AOI_EXCEPTION_DLP_CTRL_PATTERN_STOP: str=_T("DLP Pattern Stop"); break;
	case AOI_EXCEPTION_DLP_CTRL_PATTERN_READ: str=_T("DLP Pattern Read"); break;
	case AOI_EXCEPTION_DLP_CTRL_PATTERN_SEND_ONE: str=_T("DLP Pattern Send One"); break;
	case AOI_EXCEPTION_DLP_CTRL_PATTERN_SEND_ALL: str=_T("DLP Pattern Send All"); break;
	case AOI_EXCEPTION_DLP_CTRL_PATTERN_VALIDATE: str=_T("DLP Pattern Validate"); break;		
	case AOI_EXCEPTION_DLP_CTRL_OTHERS: str=_T("DLP Others"); break;	

	case AOI_EXCEPTION_BARCODE_DEVICE_BEGIN: str=_T("Barcode Device Others"); break;	
	case AOI_EXCEPTION_BARCODE_DEVICE_PARAM: str=_T("Barcode Device Param"); break;	
	case AOI_EXCEPTION_BARCODE_DEVICE_SET_FUNC: str=_T("Barcode Device Set Func"); break;
	case AOI_EXCEPTION_BARCODE_DEVICE_GET_FUNC: str=_T("Barcode Device Get Func"); break;
	case AOI_EXCEPTION_BARCODE_DEVICE_EXEC_FUNC: str=_T("Barcode Device Exec Func"); break;
	//case AOI_EXCEPTION_BARCODE_DEVICE_FILE_READ: str=_T("Barcode Device File Read"); break;
	//case AOI_EXCEPTION_BARCODE_DEVICE_FILE_WRITE: str=_T("Barcode Device File Write"); break;
	case AOI_EXCEPTION_BARCODE_DEVICE_CREATE: str=_T("Barcode Device Create"); break;
	case AOI_EXCEPTION_BARCODE_DEVICE_CONNECT: str=_T("Barcode Device Connect"); break;	
	case AOI_EXCEPTION_BARCODE_DEVICE_START: str=_T("Barcode Device Start"); break;
	case AOI_EXCEPTION_BARCODE_DEVICE_DECODE: str=_T("Barcode Device Decode"); break;	
	case AOI_EXCEPTION_BARCODE_DEVICE_OTHERS: str=_T("Barcode Device Others"); break;

	case AOI_EXCEPTION_MES_BEGIN: str=_T("MES Others"); break;	
	case AOI_EXCEPTION_MES_PARAM: str=_T("MES Param"); break;
	case AOI_EXCEPTION_MES_SET_FUNC: str=_T("MES Set Func"); break;
	case AOI_EXCEPTION_MES_GET_FUNC: str=_T("MES Get Func"); break;
	case AOI_EXCEPTION_MES_ASK_FUNC: str=_T("MES Ask Func"); break;
	case AOI_EXCEPTION_MES_EXEC_FUNC: str=_T("MES Exec Func"); break;
	case AOI_EXCEPTION_MES_UPLOAD_FUNC: str=_T("MES Upload Func"); break;
	case AOI_EXCEPTION_MES_DOWNLOAD_FUNC: str=_T("MES Download Func"); break;
	//case AOI_EXCEPTION_MES_FILE_READ: str=_T("MES File Read"); break;
	//case AOI_EXCEPTION_MES_FILE_WITE: str=_T("MES File Write"); break;
	case AOI_EXCEPTION_MES_CREATE: str=_T("MES Create"); break;
	case AOI_EXCEPTION_MES_CONNECT: str=_T("MES Connect"); break;
	case AOI_EXCEPTION_MES_DISCONNECT: str=_T("MES Disonnect"); break;
	case AOI_EXCEPTION_MES_SEND: str=_T("MES Send"); break;
	case AOI_EXCEPTION_MES_RECV: str=_T("MES Recv"); break;
	case AOI_EXCEPTION_MES_GET_MES_READY: str=_T("MES Get Host Ready"); break;
	case AOI_EXCEPTION_MES_SET_SYSTEM_PARAM: str=_T("MES Set System Param"); break;		
	case AOI_EXCEPTION_MES_SET_PROJECT_PARAM: str=_T("MES Set Project Param"); break;		
	case AOI_EXCEPTION_MES_SET_MACHINE_STATUS: str=_T("MES Set Machine Status"); break;		
	case AOI_EXCEPTION_MES_SET_PROCESS_ID: str=_T("MES Set Process ID"); break;		
	case AOI_EXCEPTION_MES_SET_CONTROL_STATE: str=_T("MES Set Ctrl State"); break;
	case AOI_EXCEPTION_MES_BARCODE_CHECK: str=_T("MES Barcode Check"); break;		
	case AOI_EXCEPTION_MES_BARCODE_ASK: str=_T("MES Barcode Ask"); break;		
	case AOI_EXCEPTION_MES_BOARD_MAPPING: str=_T("MES Board Mapping"); break;
	case AOI_EXCEPTION_MES_LOGOUT_LOGIN: str=_T("MES Logout/login"); break;	
	case AOI_EXCEPTION_MES_UNCHECK_TEST_FILE: str=_T("MES UnCheckTestFile"); break;	
	case AOI_EXCEPTION_MES_OTHERS: str=_T("MES Others"); break;

	case AOI_EXCEPTION_CUDA_BEGIN: str=_T("Cuda Others"); break;	
	case AOI_EXCEPTION_CUDA_PARAM: str=_T("Cuda Param"); break;
	case AOI_EXCEPTION_CUDA_SET_FUNC: str=_T("Cuda Set Func"); break;
	case AOI_EXCEPTION_CUDA_GET_FUNC: str=_T("Cuda Get Func"); break;
	case AOI_EXCEPTION_CUDA_EXEC_FUNC: str=_T("Cuda Exec Func"); break;
	//case AOI_EXCEPTION_CUDA_FILE_READ: str=_T("Cuda File Read"); break;
	//case AOI_EXCEPTION_CUDA_FILE_WRITE: str=_T("Cuda File Write"); break;
	case AOI_EXCEPTION_CUDA_INIT: str=_T("Cuda Init"); break;	
	case AOI_EXCEPTION_CUDA_MEM_ALLOC: str=_T("Cuda Memory Alloc"); break;
	case AOI_EXCEPTION_CUDA_MEM_FREE: str=_T("Cuda Memory Free"); break;
	case AOI_EXCEPTION_CUDA_MEM_COPY: str=_T("Cuda Memory Copy"); break;	             
	case AOI_EXCEPTION_CUDA_DISABLED: str=_T("Cuda Disabled"); break;
	case AOI_EXCEPTION_CUDA_OTHERS: str=_T("Cuda Others"); break;

	case AOI_EXCEPTION_FILE_BEGIN: str=_T("File Others"); break;
	case AOI_EXCEPTION_FILE_READ: str=_T("File Read"); break;
	case AOI_EXCEPTION_FILE_WRITE: str=_T("File Write"); break;
	case AOI_EXCEPTION_FILE_OTHERS: str=_T("File Others"); break;	             

	case AOI_EXCEPTION_THREAD_CREATE:	str=_T("Thread Create"); break;
	case AOI_EXCEPTION_THREAD_DELETE:	str=_T("Thread Delete"); break;
	case AOI_EXCEPTION_THREAD_START:		str=_T("Thread Start"); break;
	case AOI_EXCEPTION_THREAD_IDLE:		str=_T("Thread Idle"); break;
	case AOI_EXCEPTION_THREAD_EXECUTION:	str=_T("Thread Execution"); break;
	case AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE:	str=_T("Thread Wait For Idle"); break;
	case AOI_EXCEPTION_THREAD_WAIT_FOR_STOP:	str=_T("Thread Wait For Stop"); break;
	case AOI_EXCEPTION_THREAD_WAIT_FOR_START:str=_T("Thread Wait For Start"); break;
	case AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH:str=_T("Thread Wait For Finish"); break;
	case AOI_EXCEPTION_THREAD_EXEC_SLICE_FILL:str=_T("Thread Exec Slice-Fill"); break;
	case AOI_EXCEPTION_THREAD_EXEC_FRAME_MERGE:str=_T("Thread Exec Frame-Merge"); break;
	case AOI_EXCEPTION_THREAD_EXEC_FIELD_MERGE:str=_T("Thread Exec Field-Merge"); break;
	case AOI_EXCEPTION_THREAD_EXEC_REGION_CALC:str=_T("Thread Exec Region-Calc"); break;
	case AOI_EXCEPTION_THREAD_EXEC_FIELD_CALC:str=_T("Thread Exec Field-Calc"); break;
	case AOI_EXCEPTION_THREAD_EXEC_FRAME_LOAD:str=_T("Thread Exec Frame-Load"); break;
	case AOI_EXCEPTION_THREAD_EXEC_FRAME_FIELD_FREE:str=_T("Thread Exec Frame/Field-Release"); break;
	case AOI_EXCEPTION_THREAD_EXEC_SEQUENCE:str=_T("Thread Exec Sequence"); break;
	case AOI_EXCEPTION_THREAD_EXEC_ONLINE_INSPECTION:str=_T("Thread Exec Online-Inspection"); break;
	case AOI_EXCEPTION_THREAD_EXEC_REMOVE_FOLDER:str=_T("Thread Exec Remove-Folder"); break;	
	case AOI_EXCEPTION_THREAD_EXEC_CONVEYER_AUTO_RUN:str=_T("Thread Exec Conveyer-Auto-Run"); break;	
	case AOI_EXCEPTION_THREAD_EXEC_LOAD_REPAIR_FILE:str=_T("Thread Exec Load-Repair-File"); break;
	case AOI_EXCEPTION_THREAD_EXEC_REPAIR_RESULT_SIGNAL:str=_T("Thread Exec Repair-Result-Signal"); break;
	case AOI_EXCEPTION_THREAD_EXEC_SIMPLE_JOB:str=_T("Thread Exec Simple-Job"); break;
	case AOI_EXCEPTION_THREAD_EXEC_RELOAD_PROJECT:str=_T("Thread Exec Reload-Project"); break;
	
	case AOI_EXCEPTION_PROJECT_START:	str=_T("Project Others"); break;

	case AOI_EXCEPTION_PROJECT_PARAM:	str=_T("Project Param"); break;
	case AOI_EXCEPTION_PROJECT_SET_FUNC:	str=_T("Project Set Func"); break;
	case AOI_EXCEPTION_PROJECT_GET_FUNC:	str=_T("Project Get Func"); break;
	case AOI_EXCEPTION_PROJECT_EXEC_FUNC:	str=_T("Project Exec Func"); break;
	//AOI_EXCEPTION_PROJECT_FILE_READ         = 2008,//專案-檔案讀取
	//AOI_EXCEPTION_PROJECT_FILE_WRITE        = 2009,//專案-檔案寫入		
	//case AOI_EXCEPTION_PROJECT_MAP_START:	str=_T("Project Map Others"); break;
	case AOI_EXCEPTION_PROJECT_MAP_CREATE:	str=_T("Project Map Create"); break;
	case AOI_EXCEPTION_PROJECT_MAP_VALID:	str=_T("Project Map Valid"); break;	        	
	//case AOI_EXCEPTION_PROJECT_MAP_MASK_START:	str=_T("Project Map Mask Others"); break;
	case AOI_EXCEPTION_PROJECT_MAP_MASK_CREATE:	str=_T("Project Map Mask Create"); break;
	case AOI_EXCEPTION_PROJECT_MAP_MASK_VALID:	str=_T("Project Map Mask Valid"); break; 
	//case AOI_EXCEPTION_PROJECT_MARK_IMAGE_START:	str=_T("Project Mark Image Others"); break;
	case AOI_EXCEPTION_PROJECT_MARK_IMAGE_TEST:	str=_T("Project Mark Image Test"); break; 	
	//case AOI_EXCEPTION_PROJECT_TEST_MAP_START: str=_T("Project Test Map Image Others"); break; 	
	case AOI_EXCEPTION_PROJECT_TEST_MAP_CREATE: str=_T("Project Test Map Image Create"); break; 	
	case AOI_EXCEPTION_PROJECT_TEST_MAP_VALID: str=_T("Project Test Map Image Valid"); break; 		   
	//case AOI_EXCEPTION_PROJECT_FIELD_START:	str=_T("Project Field Others"); break;
	case AOI_EXCEPTION_PROJECT_FIELD_CREATE:	str=_T("Project Field Create"); break;
	case AOI_EXCEPTION_PROJECT_FIELD_ADJUST:	str=_T("Project Field Adjust"); break;
	case AOI_EXCEPTION_PROJECT_FIELD_FILL:	str=_T("Project Field Fill"); break;
	case AOI_EXCEPTION_PROJECT_FIELD_PRE_LOAD:	str=_T("Project Field Pre-Load"); break;	      
	//case AOI_EXCEPTION_PROJECT_PANEL_START:	str=_T("Project Panel Others"); break;
	//case AOI_EXCEPTION_PROJECT_BOARD_START:	str=_T("Project Board Others"); break;
	//case AOI_EXCEPTION_PROJECT_FD_START:	str=_T("Project Fd Others"); break;
	case AOI_EXCEPTION_PROJECT_FD_MATCH:	str=_T("Project Fd Match"); break;
	//case AOI_EXCEPTION_PROJECT_MARK_START:	str=_T("Project Mark Others"); break;		
	//case AOI_EXCEPTION_PROJECT_BARCODE_START:	str=_T("Project Barcode Others"); break;	
	case AOI_EXCEPTION_PROJECT_BARCODE_DECODE:	str=_T("Project Barcode Decode"); break;	    
	case AOI_EXCEPTION_PROJECT_BARCODE_INPUT:	str=_T("Project Barcode Input"); break;	     	
	//case AOI_EXCEPTION_PROJECT_COMPONENT_START:	str=_T("Project Component Others"); break;		
	case AOI_EXCEPTION_PROJECT_COMPONENT_AGENT:	str=_T("Project Component Agent"); break;	     	
	//case AOI_EXCEPTION_PROJECT_MODEL_START:	str=_T("Project Model Others"); break;		

	case AOI_EXCEPTION_PROJECT_TEST_DIMENSION:	str=_T("Project Special Test Dimension"); break; 
	case AOI_EXCEPTION_PROJECT_TEST_SCRATCH:	str=_T("Project Special Test Scratch"); break; 
	case AOI_EXCEPTION_PROJECT_TEST_DROPOUT:	str=_T("Project Special Test Dropout"); break; 

	case AOI_EXCEPTION_PROJECT_CALC_STAGE_POS:	str=_T("Project Calc Stage Pos"); break; 
	case AOI_EXCEPTION_PROJECT_CALC_CAD_RESULT:	str=_T("Project Calc CAD Result"); break; 

	case AOI_EXCEPTION_PROJECT_IMAGE_CONFIG_VALID:	str=_T("Project Image Config Valid"); break; 
	case AOI_EXCEPTION_PROJECT_CREATE_GRAB_MAP_OBJ:	str=_T("Project Create Grab Map Obj"); break; 
	case AOI_EXCEPTION_PROJECT_CREATE_PANEL_ALIGN_OBJ:	str=_T("Project Create Panel Align Obj"); break; 
	case AOI_EXCEPTION_PROJECT_CREATE_BOARD_ALIGN_OBJ:	str=_T("Project Create Board Align Obj"); break; 
	case AOI_EXCEPTION_PROJECT_CREATE_INSPECTION_OBJ:	str=_T("Project Create Inspection Obj"); break; 
	case AOI_EXCEPTION_PROJECT_OTHERS:	str=_T("Project Others"); break;

	case AOI_EXCEPTION_SYSTEM_START:	str=_T("System Others"); break;
	case AOI_EXCEPTION_SYSTEM_PARAM:	str=_T("System Param"); break;
	//AOI_EXCEPTION_SYSTEM_SET_FUNC           =10002,//系統-設定函式
	//AOI_EXCEPTION_SYSTEM_GET_FUNC           =10003,//系統-取回函式
	//AOI_EXCEPTION_SYSTEM_EXEC_FUNC          =10004,//系統-執行函式
	//AOI_EXCEPTION_SYSTEM_FILE_READ          =10011,//系統-檔案-讀取
	//AOI_EXCEPTION_SYSTEM_FILE_WRITE         =10012,//系統-檔案-寫入
	//AOI_EXCEPTION_SYSTEM_MEMORY_ALLOC       =10021,//系統-記憶體-建立失敗
	//AOI_EXCEPTION_SYSTEM_MEMORY_FREE        =10022,//系統-記憶體-釋放失敗	
	//AOI_EXCEPTION_SYSTEM_OBJECT_CREATE      =10031,//系統-AOI物件-建立失敗
	//AOI_EXCEPTION_SYSTEM_OBJECT_DELETE      =10032,//系統-AOI物件-釋放失敗
	case AOI_EXCEPTION_SYSTEM_EXEC_PROJECT_MAP:	str=_T("System Others"); break;
	case AOI_EXCEPTION_SYSTEM_EXEC_PROJECT_ALIGN:	str=_T("System Exec Project Tuning"); break;
	case AOI_EXCEPTION_SYSTEM_EXEC_OFFLINE_ALIGN:	str=_T("System Exec Offline Align"); break;
	case AOI_EXCEPTION_SYSTEM_EXEC_OFFLINE_EXPORT:	str=_T("System Exec Offline Export"); break;
	case AOI_EXCEPTION_SYSTEM_EXEC_PROJECT_TUNING:	str=_T("System Exec Project Tuning"); break;
	case AOI_EXCEPTION_SYSTEM_EXEC_OFFLINE_TUNING:	str=_T("System Exec Offline Tuning"); break;
	case AOI_EXCEPTION_SYSTEM_EXEC_PROJECT_INSPECT:	str=_T("System Exec Project Inspection"); break;
	case AOI_EXCEPTION_SYSTEM_GRAB_IMAGE_CNT:	str=_T("System Grab Image Process"); break;
	case AOI_EXCEPTION_SYSTEM_CHEK_PROJECT:	str=_T("System Check Project"); break;
	case AOI_EXCEPTION_SYSTEM_OTHERS:	str=_T("System Others"); break;

	case AOI_EXCEPTION_CALCULATION_START:	str=_T("Calculation Others"); break;	
	case AOI_EXCEPTION_CALCULATION_PARAM:	str=_T("Calculation Param"); break;
	case AOI_EXCEPTION_CALCULATION_SET_FUNC:	str=_T("Calculation Set Func"); break;
	case AOI_EXCEPTION_CALCULATION_GET_FUNC:	str=_T("Calculation Get Func"); break;
	case AOI_EXCEPTION_CALCULATION_EXEC_FUNC:	str=_T("Calculation Exec Func"); break;
	//case AOI_EXCEPTION_CALCULATION_MEM_ALLOC:	str=_T("Calculation Memory Alloc"); break;
	//case AOI_EXCEPTION_CALCULATION_MEM_FREE:	str=_T("Calculation Memory Free"); break;
	//case AOI_EXCEPTION_CALCULATION_FILE_READ:	str=_T("Calculation File Read"); break;
	//case AOI_EXCEPTION_CALCULATION_FILE_WRITE:	str=_T("Calculation File Write"); break;	
	case AOI_EXCEPTION_CALCULATION_OPENCV_API:	str=_T("Calculation OpenCV API"); break;
	case AOI_EXCEPTION_CALCULATION_JETALG_DISABLED:	str=_T("Calculation JetAlg Disabled"); break;
	case AOI_EXCEPTION_CALCULATION_OPENCV_DISABLED:	str=_T("Calculation OpenCV Disabled"); break;
	case AOI_EXCEPTION_CALCULATION_OTHERS:	str=_T("Calculation Others"); break;

	default:
		str.Format(_T("Undefined [%d]"), Code);
		break;
	}	
	return str;
}
//-------------------------------------------------------------------------------------//
void CAOIExceptionCodeCtrl::AddAOIExceptionCode(AOI_EXCEPTION_CODE Code, LPCTSTR Err, bool Block)
{
#ifdef AOI_EXCEPTION_CODE_USE
	if ( GetExceptionCodeRunning() == false )
	{	return; }

	//若數量過多就不再寫入	
	if ( CheckAOIExceptionCodeCount() == true )
	{	return; }

	if ( true == Block )
	{	LockExceptionCode();	}	
	TExceptionNode Node(Code, Err);
	CString Str = GetExceptionCodeText(Code);	
	std::vector<TExceptionNode> &List=m_ExceptionList;
	const size_t CountLast=List.size();
	if ( 0 == CountLast )
	{	List.push_back(Node);	}
	else
	{
		const TExceptionNode &NodeLast=List[CountLast-1];
		if ( NodeLast.eCode!=Node.eCode || NodeLast.sErr.CompareNoCase(Err)!=0 )
		{	List.push_back(Node);	}
	}
	const size_t CountNow=List.size();
	m_LastExceptionTxt = Str;
	m_LastExceptionCode = Code;	
	if ( CountNow != CountLast )
	{	SaveAOIExceptionCodeLogFile(Code,  Str, Err);	}	
	if ( true == Block )
	{	UnlockExceptionCode();	}
#endif//AOI_EXCEPTION_CODE_USE
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIExceptionCodeCtrl::GetAOILastExceptionText() const
{
	return m_LastExceptionTxt;
}
//-------------------------------------------------------------------------------------//
AOI_EXCEPTION_CODE CAOIExceptionCodeCtrl::GetAOILastExceptionCode() const
{
	return m_LastExceptionCode;
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::CheckLastExceptionCodeOK() const//確認異常碼
{
	if ( AOI_EXCEPTION_NONE == GetAOILastExceptionCode() )
	{	return true;	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::CheckAOIExceptionCodeCount() const//確認異常碼
{
	if ( m_ExceptionList.size() < MAX_AOI_EXCEPTION_CODE_COUNT )
	{	return false; }
	return true;		 
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::ResetAOIExceptionCode()//復歸異常碼
{
	if ( CheckLastExceptionCodeOK() == false )
	{
		CString Message;
		Message.Format(_T("[%06d]#Reset Exception Code\n"), AOI_EXCEPTION_NONE);
		if ( SaveAOIExceptionCodeLogFile(Message) == false )
		{	return false; }
	}
	m_ExceptionList.clear();
	m_LastExceptionTxt = _T("OK");
	m_LastExceptionCode = AOI_EXCEPTION_NONE; 
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_OK()//OK異常碼
{
	return SetExceptionCode(AOI_EXCEPTION_NONE, NULL);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Others(LPCTSTR Err)
{
	if ( CheckLastExceptionCodeOK() == false )
	{	return true; }
	return SetExceptionCode(AOI_EXCEPTION_OTHERS, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Param(LPCTSTR Err)//參數異常
{
	return SetAOIExceptionCode_System(AOI_EXCEPTION_SYSTEM_PARAM, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_FileRead(LPCTSTR Err)//檔案讀取
{
	return SetAOIExceptionCode_File(AOI_EXCEPTION_FILE_READ, Err);
	//return SetAOIExceptionCode_System(AOI_EXCEPTION_SYSTEM_FILE_READ, Err);	          
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_FileWrite(LPCTSTR Err)//檔案寫入
{
	return SetAOIExceptionCode_File(AOI_EXCEPTION_FILE_WRITE, Err);
	//return SetAOIExceptionCode_System(AOI_EXCEPTION_SYSTEM_FILE_WRITE, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_MemoryAlloc(LPCTSTR Err)//記憶體配置
{
	return SetAOIExceptionCode_Memory(AOI_EXCEPTION_MEMORY_ALLOC, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_MemoryFree(LPCTSTR Err)//記憶體釋放
{
	return SetAOIExceptionCode_Memory(AOI_EXCEPTION_MEMORY_FREE, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_PLC(DWORD Code, LPCTSTR Err)
{
	if ( Code > AOI_EXCEPTION_PLC_OTHERS )
	{	return ReturnNoExceptionCode(Code); }
	if ( Code < AOI_EXCEPTION_PLC_BEGIN )
	{	return ReturnNoExceptionCode(Code); }		
	return SetExceptionCode(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_PLC_Others(LPCTSTR Err)//PLC異常碼
{
	if ( CheckLastExceptionCodeOK() == false )
	{	return true; }
	return SetAOIExceptionCode_PLC(AOI_EXCEPTION_PLC_OTHERS, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_DLP(DWORD Code, LPCTSTR Err)
{
	if ( Code > AOI_EXCEPTION_DLP_CTRL_OTHERS )
	{	return ReturnNoExceptionCode(Code); }
	if ( Code < AOI_EXCEPTION_DLP_CTRL_BEGIN )
	{	return ReturnNoExceptionCode(Code); }	
	return SetExceptionCode(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_DLP_Others(LPCTSTR Err)//DLP異常碼
{
	if ( CheckLastExceptionCodeOK() == false )
	{	return true; }
	return SetAOIExceptionCode_DLP(AOI_EXCEPTION_DLP_CTRL_OTHERS, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_MES(DWORD Code, LPCTSTR Err)
{
	if ( Code > AOI_EXCEPTION_MES_OTHERS )
	{	return ReturnNoExceptionCode(Code); }
	if ( Code < AOI_EXCEPTION_MES_BEGIN )
	{	return ReturnNoExceptionCode(Code); }	
	return SetExceptionCode(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_MES_Others(LPCTSTR Err)//MES異常碼
{
	if ( CheckLastExceptionCodeOK() == false )
	{	return true; }
	return SetAOIExceptionCode_MES(AOI_EXCEPTION_MES_OTHERS, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Cuda(DWORD Code, LPCTSTR Err)//Cuda異常碼
{
	if ( Code > AOI_EXCEPTION_CUDA_OTHERS )
	{	return ReturnNoExceptionCode(Code); }
	if ( Code < AOI_EXCEPTION_CUDA_BEGIN )
	{	return ReturnNoExceptionCode(Code); }	
	return SetExceptionCode(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Cuda_Others(LPCTSTR Err)//Cuda異常碼
{
	if ( CheckLastExceptionCodeOK() == false )
	{	return true; }
	return SetAOIExceptionCode_Cuda(AOI_EXCEPTION_CUDA_OTHERS, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_File(DWORD Code, LPCTSTR Err)//檔案異常碼
{
	if ( Code > AOI_EXCEPTION_FILE_OTHERS )
	{	return ReturnNoExceptionCode(Code); }
	if ( Code < AOI_EXCEPTION_FILE_BEGIN )
	{	return ReturnNoExceptionCode(Code); }	
	return SetExceptionCode(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_File_Others(LPCTSTR Err)//檔案異常碼	
{
	if ( CheckLastExceptionCodeOK() == false )
	{	return true; }
	return SetAOIExceptionCode_Object(AOI_EXCEPTION_FILE_OTHERS, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Object(DWORD Code, LPCTSTR Err)
{
	if ( Code > AOI_EXCEPTION_OBJECT_OTHERS )
	{	return ReturnNoExceptionCode(Code); }
	if ( Code < AOI_EXCEPTION_OBJECT_BEGIN )
	{	return ReturnNoExceptionCode(Code); }	
	return SetExceptionCode(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Object_Others(LPCTSTR Err)//物件異常碼
{
	if ( CheckLastExceptionCodeOK() == false )
	{	return true; }
	return SetAOIExceptionCode_Object(AOI_EXCEPTION_OBJECT_OTHERS, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Memory(DWORD Code, LPCTSTR Err)
{
	if ( Code > AOI_EXCEPTION_MEMORY_OTHERS )
	{	return ReturnNoExceptionCode(Code); }
	if ( Code < AOI_EXCEPTION_MEMORY_BEGIN )
	{	return ReturnNoExceptionCode(Code); }	
	return SetExceptionCode(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Memory_Others(LPCTSTR Err)//記憶體異常碼
{
	if ( CheckLastExceptionCodeOK() == false )
	{	return true; }
	return SetAOIExceptionCode_Memory(AOI_EXCEPTION_MEMORY_OTHERS, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Camera(DWORD Code, LPCTSTR Err)
{
	if ( Code > AOI_EXCEPTION_CAMERA_OTHERS )
	{	return ReturnNoExceptionCode(Code); }
	if ( Code < AOI_EXCEPTION_CAMERA_BEGIN )
	{	return ReturnNoExceptionCode(Code); }	
	return SetExceptionCode(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Camera_Others(LPCTSTR Err)//相機異常碼
{
	if ( CheckLastExceptionCodeOK() == false )
	{	return true; }
	return SetAOIExceptionCode_Camera(AOI_EXCEPTION_CAMERA_OTHERS, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Motion(DWORD Code, LPCTSTR Err)
{
	if ( Code > AOI_EXCEPTION_MOTION_OTHERS )
	{	return ReturnNoExceptionCode(Code); }
	if ( Code < AOI_EXCEPTION_MOTION_BEGIN )
	{	return ReturnNoExceptionCode(Code); }	
	return SetExceptionCode(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Motion_Others(LPCTSTR Err)//軸控異常碼
{
	if ( CheckLastExceptionCodeOK() == false )
	{	return true; }
	return SetAOIExceptionCode_Motion(AOI_EXCEPTION_MOTION_OTHERS, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Thread(DWORD Code, LPCTSTR Err)//執行緒異常碼
{
	if ( Code > AOI_EXCEPTION_THREAD_OTHERS )
	{	return ReturnNoExceptionCode(Code); }
	if ( Code < AOI_EXCEPTION_THREAD_BEGIN )
	{	return ReturnNoExceptionCode(Code); }		
	return SetExceptionCode(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Thread_Others(LPCTSTR Err)//執行緒異常碼
{
	return SetAOIExceptionCode_Thread_Chk(AOI_EXCEPTION_THREAD_OTHERS, true, Err);	
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Thread_Chk(DWORD Code, bool bChk, LPCTSTR Err)//執行緒異常碼
{
	if ( bChk )
	{
		if ( CheckLastExceptionCodeOK() == false )
		{	return true; }
	}
	return SetAOIExceptionCode_Thread(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_System(DWORD Code, LPCTSTR Err)//系統異常碼
{
	if ( Code > AOI_EXCEPTION_SYSTEM_OTHERS )
	{	return ReturnNoExceptionCode(Code); }
	if ( Code < AOI_EXCEPTION_SYSTEM_START )
	{	return ReturnNoExceptionCode(Code); }	
	return SetExceptionCode(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_System_Others(LPCTSTR Err)//系統異常碼		
{	
	return SetAOIExceptionCode_System_Chk(AOI_EXCEPTION_SYSTEM_OTHERS, true, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_System_Chk(DWORD Code, bool bChk, LPCTSTR Err)//系統異常碼
{
	if ( bChk )
	{
		if ( CheckLastExceptionCodeOK() == false )
		{	return true; }
	}
	return SetAOIExceptionCode_System(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_BarcodeDevice(DWORD Code, LPCTSTR Err)
{
	if ( Code > AOI_EXCEPTION_BARCODE_DEVICE_OTHERS )
	{	return ReturnNoExceptionCode(Code); }
	if ( Code < AOI_EXCEPTION_BARCODE_DEVICE_BEGIN )
	{	return ReturnNoExceptionCode(Code); }	
	return SetExceptionCode(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_BarcodeDevice_Others(LPCTSTR Err)//條碼機異常碼
{
	if ( CheckLastExceptionCodeOK() == false )
	{	return true; }
	return SetAOIExceptionCode_BarcodeDevice(AOI_EXCEPTION_BARCODE_DEVICE_OTHERS, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_LightCtrlBoard(DWORD Code, LPCTSTR Err)
{
	if ( Code > AOI_EXCEPTION_LIGHT_CTRL_OTHERS )
	{	return ReturnNoExceptionCode(Code); }
	if ( Code < AOI_EXCEPTION_LIGHT_CTRL_BEGIN )
	{	return ReturnNoExceptionCode(Code); }	
	return SetExceptionCode(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_LightCtrlBoard_Others(LPCTSTR Err)//燈控異常碼
{
	if ( CheckLastExceptionCodeOK() == false )
	{	return true; }
	return SetAOIExceptionCode_LightCtrlBoard(AOI_EXCEPTION_LIGHT_CTRL_OTHERS, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Project(DWORD Code, LPCTSTR Err)
{
	if ( Code > AOI_EXCEPTION_PROJECT_OTHERS )
	{	return ReturnNoExceptionCode(Code); }
	if ( Code < AOI_EXCEPTION_PROJECT_START )
	{	return ReturnNoExceptionCode(Code); }	
	return SetExceptionCode(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Project_Others(LPCTSTR Err)//專案異常碼
{
	if ( CheckLastExceptionCodeOK() == false )
	{	return true; }
	return SetAOIExceptionCode_Project(AOI_EXCEPTION_PROJECT_OTHERS, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Calculation(DWORD Code, LPCTSTR Err)//資料計算異常碼	
{
	if ( Code > AOI_EXCEPTION_CALCULATION_OTHERS )
	{	return ReturnNoExceptionCode(Code); }
	if ( Code < AOI_EXCEPTION_CALCULATION_START )
	{	return ReturnNoExceptionCode(Code); }	
	return SetExceptionCode(Code, Err);
}
//-------------------------------------------------------------------------------------//
bool CAOIExceptionCodeCtrl::SetAOIExceptionCode_Calculation_Others(LPCTSTR Err)//資料計算異常碼	
{
	if ( CheckLastExceptionCodeOK() == false )
	{	return true; }
	return SetAOIExceptionCode_Calculation(AOI_EXCEPTION_CALCULATION_OTHERS, Err);
}
//-------------------------------------------------------------------------------------//
