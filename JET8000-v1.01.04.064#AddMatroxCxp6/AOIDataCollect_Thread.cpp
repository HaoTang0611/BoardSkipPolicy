//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "AOIDataCollect.h"
//-------------------------------------------------------------------------------------//
#include "JetLoadDll.h"
#include "MES\\MES_Class.h"
//-------------------------------------------------------------------------------------//
#define THREAD_CHECK_NO_WORKING_MAX_COUNT    200//執行緒確認空轉的最大次數
//-------------------------------------------------------------------------------------//
unsigned int  SystemRunThreadID = 0; //系統運作的執行緒編號
HANDLE SystemRunThreadHandle = NULL; //系統運作的執行緒處理碼 
HANDLE SystemRunThreadEvent = NULL;  //系統運作的執行緒事件
unsigned int __stdcall SystemRunThreadFn(void *pParam);//系統運作的執行緒
//-------------------------------------------------------------------------------------//
//相機圖填滿的執行緒
unsigned int  SliceFillThreadID[MAX_THREAD_COUNT_SLICE_FILL] = {0}; //相機圖影像的執行緒編號
HANDLE SliceFillThreadHandle[MAX_THREAD_COUNT_SLICE_FILL] = {NULL}; //相機圖影像的執行緒處理碼 
HANDLE SliceFillThreadEvent[MAX_THREAD_COUNT_SLICE_FILL] = {NULL};  //相機圖影像的執行緒事件
unsigned int __stdcall SliceFillThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
//影像合併的執行緒
unsigned int  FrameMergeThreadID[MAX_THREAD_COUNT_FRAME_MERGE] = {0}; //影像合併的執行緒編號
HANDLE FrameMergeThreadHandle[MAX_THREAD_COUNT_FRAME_MERGE] = {NULL}; //影像合併的執行緒處理碼 
HANDLE FrameMergeThreadEvent[MAX_THREAD_COUNT_FRAME_MERGE] = {NULL};  //影像合併的執行緒事件
unsigned int __stdcall FrameMergeThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
//區域合併的執行緒
unsigned int  FieldMergeThreadID[MAX_THREAD_COUNT_FIELD_MERGE] = {0}; //區域合併的執行緒編號
HANDLE FieldMergeThreadHandle[MAX_THREAD_COUNT_FIELD_MERGE] = {NULL}; //區域合併的執行緒處理碼 
HANDLE FieldMergeThreadEvent[MAX_THREAD_COUNT_FIELD_MERGE] = {NULL};  //區域合併的執行緒事件
unsigned int __stdcall FieldMergeThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
//影像載入的執行緒
unsigned int  FrameLoadThreadID[MAX_THREAD_COUNT_FRAME_LOAD] = {0}; //影像載入的執行緒編號
HANDLE FrameLoadThreadHandle[MAX_THREAD_COUNT_FRAME_LOAD] = {NULL}; //影像載入的執行緒處理碼 
HANDLE FrameLoadThreadEvent[MAX_THREAD_COUNT_FRAME_LOAD] = {NULL};  //影像載入的執行緒事件
unsigned int __stdcall FrameLoadThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
//視野計算的執行緒
unsigned int  FieldCalcThreadID[MAX_THREAD_COUNT_FIELD_CALC] = {0}; //視野計算的執行緒編號
HANDLE FieldCalcThreadHandle[MAX_THREAD_COUNT_FIELD_CALC] = {NULL}; //視野計算的執行緒處理碼 
HANDLE FieldCalcThreadEvent[MAX_THREAD_COUNT_FIELD_CALC] = {NULL};  //視野計算的執行緒事件
unsigned int __stdcall FieldCalcThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
//區域計算的執行緒
unsigned int RegionCalcThreadID[MAX_THREAD_COUNT_REGION_CALC] = {0}; //影像合併的執行緒編號
HANDLE RegionCalcThreadHandle[MAX_THREAD_COUNT_REGION_CALC] = {NULL}; //影像合併的執行緒處理碼 
HANDLE RegionCalcThreadEvent[MAX_THREAD_COUNT_REGION_CALC] = {NULL};  //影像合併的執行緒事件
unsigned int __stdcall RegionCalcThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
//檢測程序執行緒
unsigned int ThreadSequenceThreadID = 0; //執行緒程序的執行緒編號
HANDLE ThreadSequenceThreadHandle = NULL; //執行緒程序的執行緒處理碼 
HANDLE ThreadSequenceThreadEvent = NULL;  //執行緒程序的執行緒事件
unsigned int __stdcall ThreadSequenceThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
//區域影像釋放執行緒
unsigned int FieldFrameReleaseThreadID = 0; //區域影像釋放的執行緒編號
HANDLE FieldFrameReleaseHandle = NULL; //區域影像釋放的執行緒處理碼 
HANDLE FieldFrameReleaseEvent = NULL;  //區域影像釋放的執行緒事件
unsigned int __stdcall FieldFrameReleaseThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
//線上檢測執行緒
unsigned int OnlineInspectionThreadID = 0;//線上檢測的執行緒編號
HANDLE OnlineInspectionHandle = NULL; //線上檢測的執行緒處理碼 
HANDLE OnlineInspectionEvent = NULL;  //線上檢測的執行緒事件
unsigned int __stdcall OnlineInspectionThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
//移除資料夾執行緒
unsigned int RemoveFolderThreadID = 0;//移除資料夾執行緒編號
HANDLE RemoveFolderThreadHandle = NULL; //移除資料夾執行緒處理碼 
HANDLE RemoveFolderThreadEvent = NULL;  //移除資料夾執行緒事件
unsigned int __stdcall RemoveFolderThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
//軌道自動運轉
unsigned int ConveyerAutoRunThreadID[MAX_THREAD_COUNT_LANE_AUTO_RUN] = {0};//軌道自動運轉的執行緒編號
HANDLE ConveyerAutoRunThreadHandle[MAX_THREAD_COUNT_LANE_AUTO_RUN] = {NULL};//軌道自動運轉的執行緒處理碼
HANDLE ConveyerAutoRunThreadEvent[MAX_THREAD_COUNT_LANE_AUTO_RUN] = {NULL};//軌道自動運轉的執行緒事件
unsigned int __stdcall ConveyerAutoRunThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
//載入維修站檔案執行緒
unsigned int LoadRepairFileThreadID = 0;//載入維修站檔案執行緒編號
HANDLE LoadRepairFileThreadHandle = NULL;//載入維修站檔案執行緒處理碼
HANDLE LoadRepairFileThreadEvent = NULL;//載入維修站檔案執行緒事件
unsigned int __stdcall LoadRepairFileThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
//維修站結果訊號執行
unsigned int RepairResultSignalThreadID = 0;//維修站結果訊號執行緒編號
HANDLE RepairResultSignalThreadHandle = NULL;//維修站結果訊號執行緒處理碼
HANDLE RepairResultSignalThreadEvent = NULL;//維修站結果訊號執行緒事件
unsigned int __stdcall RepairResultSignalThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
//簡易工作執行緒
unsigned int SimpleJobThreadID = 0;//簡易工作執行緒編號
HANDLE SimpleJobThreadHandle = NULL;//簡易工作執行緒處理碼
HANDLE SimpleJobThreadEvent = NULL;//簡易工作執行緒事件
unsigned int __stdcall SimpleJobThreadFn(void *pParam);
//-------------------------------------------------------------------------------------//
unsigned int __stdcall SystemRunThreadFn(void *pParam)//系統運作的執行緒
{
#ifndef OFFLINE_VERSION
	bool IsOK = true;	
	DWORD  SleepTime = 10;//60 sec exec	
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	while ( true )
	{	
		ThreadCmd = AOIDataCollect.GetSystemRunThreadCmd();
		ThreadState = AOIDataCollect.GetSystemRunThreadState();
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{	break;	}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			AOIDataCollect.SetSystemRunThreadState(THREAD_STATE_IDLE);		
			::Sleep(SleepTime);	
			continue; 
		}
		if ( THREAD_COMMAND_TO_RUN == ThreadCmd ) 
		{	
			AOIDataCollect.SetSystemRunThreadState(THREAD_STATE_RUNNING);					
			IsOK = AOIDataCollect.ExecSystemRunFn();
		}
		::Sleep(SleepTime);
	};
	AOIDataCollect.SetSystemRunThreadState(THREAD_STATE_NONE);
	if ( NULL != SystemRunThreadEvent )
	{	::SetEvent(SystemRunThreadEvent); }
#endif//OFFLINE_VERSION
	return 0;
}
//-------------------------------------------------------------------------------------//
unsigned int __stdcall SliceFillThreadFn(void *pParam)
{
#ifndef OFFLINE_VERSION
	bool IsOK = true;
	DWORD  SleepTime = 10;
	const size_t ThreadIdx = (size_t)pParam;
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	
	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");
	while ( true )
	{			
		ThreadCmd = AOIDataCollect.GetSliceFillThreadCmd(ThreadIdx);
		ThreadState = AOIDataCollect.GetSliceFillThreadState(ThreadIdx);
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{
			break;
		}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			AOIDataCollect.SetSliceFillThreadState(ThreadIdx, THREAD_STATE_IDLE);		
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
			::SetThreadPriority(SliceFillThreadHandle[ThreadIdx], THREAD_PRIORITY_ABOVE_NORMAL);
			AOIDataCollect.SetSliceFillThreadState(ThreadIdx, THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = AOIDataCollect.ExecSliceFillFn(ThreadIdx);
			QueryPerformanceCounter(&nEndTime);
			if ( false == IsOK )
			{	
				CString ErrorMSG = AOIDataCollect.GetErrorStringRaw();
				AOIDataCollect.SetIsSystemException(true);
				AOIDataCollect.SetSliceFillThreadState(ThreadIdx, THREAD_STATE_EXCEPTION);
				AOIDataCollect.SetThreadExceptionCode(AOI_EXCEPTION_THREAD_EXEC_SLICE_FILL, true, ErrorMSG);
				AOIDataCollect.PostCallbackWndMessage(MSG_SYSTEM_EXCEPTION_CALLBACK, NULL, LPARAM_SYSTEM_EXCEPTION_INSPECTION);				
			}
			else
			{	AOIDataCollect.SetSliceFillThreadState(ThreadIdx, THREAD_STATE_FINISH);	}
			//AOIDataCollect.SetSliceFillThreadCmd(ThreadIdx, THREAD_COMMAND_TO_IDLE);
			if ( NULL  != SliceFillThreadEvent[ThreadIdx] )
			{	::SetEvent(SliceFillThreadEvent[ThreadIdx]); }		

			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;

			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveSliceFillThreadLog ) 
			{	
				_stprintf(strBuffer, _T("SliceFillThreadFn[%d]: %.2f ms"), ThreadIdx+1, Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);
			}
			::SetThreadPriority(SliceFillThreadHandle[ThreadIdx], THREAD_PRIORITY_BELOW_NORMAL);
			continue;
		}
		::Sleep(SleepTime);
	};
	AOIDataCollect.SetSliceFillThreadState(ThreadIdx, THREAD_STATE_NONE);
	if ( NULL != SliceFillThreadEvent[ThreadIdx] )
	{	::SetEvent(SliceFillThreadEvent[ThreadIdx]); }
//	::_endthreadex(0);
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
unsigned int __stdcall FrameMergeThreadFn(void *pParam)
{
#ifndef OFFLINE_VERSION
	bool IsOK = true;
	DWORD  SleepTime = 10;	
	const size_t ThreadIdx = (size_t)pParam;
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	
	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");

	while ( true )
	{	
		ThreadCmd = AOIDataCollect.GetFrameMergeThreadCmd(ThreadIdx);
		ThreadState = AOIDataCollect.GetFrameMergeThreadState(ThreadIdx);
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{
			break;
		}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			AOIDataCollect.SetFrameMergeThreadState(ThreadIdx, THREAD_STATE_IDLE);		
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
			::SetThreadPriority(FrameMergeThreadHandle[ThreadIdx], THREAD_PRIORITY_ABOVE_NORMAL);
			AOIDataCollect.SetFrameMergeThreadState(ThreadIdx, THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = AOIDataCollect.ExecFrameMergeFn(ThreadIdx);
			QueryPerformanceCounter(&nEndTime);
			if ( false == IsOK )
			{	
				CString ErrorMSG = AOIDataCollect.GetErrorStringRaw();
				AOIDataCollect.SetIsSystemException(true);
				AOIDataCollect.SetFrameMergeThreadState(ThreadIdx, THREAD_STATE_EXCEPTION);
				AOIDataCollect.SetThreadExceptionCode(AOI_EXCEPTION_THREAD_EXEC_FRAME_MERGE, true, ErrorMSG);
				AOIDataCollect.PostCallbackWndMessage(MSG_SYSTEM_EXCEPTION_CALLBACK, NULL, LPARAM_SYSTEM_EXCEPTION_INSPECTION);				
			}
			else
			{	AOIDataCollect.SetFrameMergeThreadState(ThreadIdx, THREAD_STATE_FINISH);	}
			//AOIDataCollect.SetFrameMergeThreadCmd(ThreadIdx, THREAD_COMMAND_TO_IDLE);
			if ( NULL  != FrameMergeThreadEvent[ThreadIdx] )
			{	::SetEvent(FrameMergeThreadEvent[ThreadIdx] ); }		

			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;	

			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveFrameMergeThreadLog ) 
			{	
				_stprintf(strBuffer, _T("FrameMergeThreadFn[%d]: %.2f ms"), ThreadIdx+1, Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);	
			}
			::SetThreadPriority(FrameMergeThreadHandle[ThreadIdx], THREAD_PRIORITY_BELOW_NORMAL);
			continue;
		}
		::Sleep(SleepTime);
	};
	AOIDataCollect.SetFrameMergeThreadState(ThreadIdx, THREAD_STATE_NONE);
	if ( NULL != FrameMergeThreadEvent[ThreadIdx] )
	{	::SetEvent(FrameMergeThreadEvent[ThreadIdx]); }
//	::_endthreadex(0);
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
unsigned int __stdcall FieldMergeThreadFn(void *pParam)
{
#ifndef OFFLINE_VERSION
	bool IsOK = true;
	DWORD  SleepTime = 10;	
	const size_t ThreadIdx = (size_t)pParam;
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	
	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");

	while ( true )
	{	
		ThreadCmd = AOIDataCollect.GetFieldMergeThreadCmd(ThreadIdx);
		ThreadState = AOIDataCollect.GetFieldMergeThreadState(ThreadIdx);
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{
			break;
		}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			AOIDataCollect.SetFieldMergeThreadState(ThreadIdx, THREAD_STATE_IDLE);		
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
			::SetThreadPriority(FieldMergeThreadHandle[ThreadIdx], THREAD_PRIORITY_ABOVE_NORMAL);
			AOIDataCollect.SetFieldMergeThreadState(ThreadIdx, THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = AOIDataCollect.ExecFieldMergeFn(ThreadIdx);
			QueryPerformanceCounter(&nEndTime);
			if ( false == IsOK )
			{	
				CString ErrorMSG = AOIDataCollect.GetErrorStringRaw();
				AOIDataCollect.SetIsSystemException(true);
				AOIDataCollect.SetFieldMergeThreadState(ThreadIdx, THREAD_STATE_EXCEPTION);
				AOIDataCollect.SetThreadExceptionCode(AOI_EXCEPTION_THREAD_EXEC_FIELD_MERGE, true, ErrorMSG);
				AOIDataCollect.PostCallbackWndMessage(MSG_SYSTEM_EXCEPTION_CALLBACK, NULL, LPARAM_SYSTEM_EXCEPTION_INSPECTION);				
			}
			else
			{	AOIDataCollect.SetFieldMergeThreadState(ThreadIdx, THREAD_STATE_FINISH);	}
			//AOIDataCollect.SetFieldMergeThreadCmd(ThreadIdx, THREAD_COMMAND_TO_IDLE);
			if ( NULL  != FieldMergeThreadEvent[ThreadIdx] )
			{	::SetEvent(FieldMergeThreadEvent[ThreadIdx] ); }		

			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;	

			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveFieldMergeThreadLog ) 			
			{	
				_stprintf(strBuffer, _T("FieldMergeThreadFn[%d]: %.2f ms"), ThreadIdx+1, Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);	
			}
			::SetThreadPriority(FieldMergeThreadHandle[ThreadIdx], THREAD_PRIORITY_BELOW_NORMAL);
			continue;
		}
		::Sleep(SleepTime);
	};
	AOIDataCollect.SetFieldMergeThreadState(ThreadIdx, THREAD_STATE_NONE);
	if ( NULL != FieldMergeThreadEvent[ThreadIdx] )
	{	::SetEvent(FieldMergeThreadEvent[ThreadIdx]); }
//	::_endthreadex(0);
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
unsigned int __stdcall FrameLoadThreadFn(void *pParam)
{
//#ifndef OFFLINE_VERSION
	bool IsOK = true;
	DWORD  SleepTime = 10;	
	const size_t ThreadIdx = (size_t)pParam;
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	
	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");

	while ( true )
	{	
		ThreadCmd = AOIDataCollect.GetFrameLoadThreadCmd(ThreadIdx);
		ThreadState = AOIDataCollect.GetFrameLoadThreadState(ThreadIdx);
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{
			break;
		}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			AOIDataCollect.SetFrameLoadThreadState(ThreadIdx, THREAD_STATE_IDLE);		
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
			::SetThreadPriority(FrameLoadThreadHandle[ThreadIdx], THREAD_PRIORITY_ABOVE_NORMAL);
			AOIDataCollect.SetFrameLoadThreadState(ThreadIdx, THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = AOIDataCollect.ExecFrameLoadFn(ThreadIdx);
			QueryPerformanceCounter(&nEndTime);
			if ( false == IsOK )
			{	
				CString ErrorMSG = AOIDataCollect.GetErrorStringRaw();
				AOIDataCollect.SetIsSystemException(true);
				AOIDataCollect.SetFrameLoadThreadState(ThreadIdx, THREAD_STATE_EXCEPTION);
				AOIDataCollect.SetThreadExceptionCode(AOI_EXCEPTION_THREAD_EXEC_FRAME_LOAD, true, ErrorMSG);
				AOIDataCollect.PostCallbackWndMessage(MSG_SYSTEM_EXCEPTION_CALLBACK, NULL, LPARAM_SYSTEM_EXCEPTION_INSPECTION);				
			}
			else
			{	AOIDataCollect.SetFrameLoadThreadState(ThreadIdx, THREAD_STATE_FINISH);	}
			//AOIDataCollect.SetFrameLoadThreadCmd(ThreadIdx, THREAD_COMMAND_TO_IDLE);
			if ( NULL  != FrameLoadThreadEvent[ThreadIdx] )
			{	::SetEvent(FrameLoadThreadEvent[ThreadIdx] ); }		

			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;		
			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveFrameLoadThreadLog ) 
			{
				_stprintf(strBuffer, _T("FrameLoadThreadFn[%d]: %.2f ms"), ThreadIdx+1, Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);	
			}
			::SetThreadPriority(FrameLoadThreadHandle[ThreadIdx], THREAD_PRIORITY_BELOW_NORMAL);
			continue;
		}
		::Sleep(SleepTime);
	};
	AOIDataCollect.SetFrameLoadThreadState(ThreadIdx, THREAD_STATE_NONE);
	if ( NULL != FrameLoadThreadEvent[ThreadIdx] )
	{	::SetEvent(FrameLoadThreadEvent[ThreadIdx]); }
//	::_endthreadex(0);
//#endif//OFFLINE_VERSION
	return 0;
}
//-------------------------------------------------------------------------------------//
unsigned int __stdcall FieldCalcThreadFn(void *pParam)
{
//#ifndef OFFLINE_VERSION
	bool IsOK = true;
	DWORD  SleepTime = 10;	
	const size_t ThreadIdx = (size_t)pParam;
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	
	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");

	while ( true )
	{	
		ThreadCmd = AOIDataCollect.GetFieldCalcThreadCmd(ThreadIdx);
		ThreadState = AOIDataCollect.GetFieldCalcThreadState(ThreadIdx);
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{
			break;
		}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			AOIDataCollect.SetFieldCalcThreadState(ThreadIdx, THREAD_STATE_IDLE);		
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
			AOIDataCollect.SetFieldCalcThreadState(ThreadIdx, THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = AOIDataCollect.ExecFieldCalcFn(ThreadIdx);
			QueryPerformanceCounter(&nEndTime);
			if ( false == IsOK )
			{	
				CString ErrorMSG = AOIDataCollect.GetErrorStringRaw();
				AOIDataCollect.SetIsSystemException(true);
				AOIDataCollect.SetFieldCalcThreadState(ThreadIdx, THREAD_STATE_EXCEPTION);
				AOIDataCollect.SetThreadExceptionCode(AOI_EXCEPTION_THREAD_EXEC_FIELD_CALC, true, ErrorMSG);
				AOIDataCollect.PostCallbackWndMessage(MSG_SYSTEM_EXCEPTION_CALLBACK, NULL, LPARAM_SYSTEM_EXCEPTION_INSPECTION);				
			}
			else
			{	AOIDataCollect.SetFieldCalcThreadState(ThreadIdx, THREAD_STATE_FINISH);	}
			//AOIDataCollect.SetFieldCalcThreadCmd(ThreadIdx, THREAD_COMMAND_TO_IDLE);
			if ( NULL  != FieldCalcThreadEvent[ThreadIdx] )
			{	::SetEvent(FieldCalcThreadEvent[ThreadIdx] ); }		

			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;		
			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveFieldMergeThreadLog ) 
			{
				_stprintf(strBuffer, _T("FieldCalcThreadFn[%d]: %.2f ms"), ThreadIdx+1, Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);	
			}			
			continue;
		}
		::Sleep(SleepTime);
	};
	AOIDataCollect.SetFieldCalcThreadState(ThreadIdx, THREAD_STATE_NONE);
	if ( NULL != FieldCalcThreadEvent[ThreadIdx] )
	{	::SetEvent(FieldCalcThreadEvent[ThreadIdx]); }
//	::_endthreadex(0);
//#endif//OFFLINE_VERSION
	return 0;
}
//-------------------------------------------------------------------------------------//
unsigned int __stdcall RegionCalcThreadFn(void *pParam)
{
//#ifndef OFFLINE_VERSION//Kai-20170410
	bool IsOK = true;	
	DWORD  SleepTime = 10;
	const size_t ThreadIdx = (size_t)pParam;
	const DWORD  CpuCoreNumber =  AOIDataCollect.GetComputerCPUCoreNumber();
	const DWORD  CpuCoreKeep = 4;//保留4個核心
	const DWORD  CpuCoreUsingNumber = ThreadIdx+CpuCoreKeep;//
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	
	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");

	while ( true )
	{	
		ThreadCmd = AOIDataCollect.GetRegionCalcThreadCmd(ThreadIdx);
		ThreadState = AOIDataCollect.GetRegionCalcThreadState(ThreadIdx);
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{
			break;
		}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			AOIDataCollect.SetRegionCalcThreadState(ThreadIdx, THREAD_STATE_IDLE);		
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

			//if ( CpuCoreUsingNumber < CpuCoreNumber )
			{	::SetThreadPriority(RegionCalcThreadHandle[ThreadIdx], THREAD_PRIORITY_ABOVE_NORMAL); }

			AOIDataCollect.SetRegionCalcThreadState(ThreadIdx, THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = AOIDataCollect.ExecRegionCalcFn(ThreadIdx);			
			QueryPerformanceCounter(&nEndTime);
			if ( false == IsOK )
			{	
				CString ErrorMSG = AOIDataCollect.GetErrorStringRaw();
				AOIDataCollect.SetIsSystemException(true);
				AOIDataCollect.SetRegionCalcThreadState(ThreadIdx, THREAD_STATE_EXCEPTION);
				AOIDataCollect.SetThreadExceptionCode(AOI_EXCEPTION_THREAD_EXEC_REGION_CALC, true, ErrorMSG);
				AOIDataCollect.PostCallbackWndMessage(MSG_SYSTEM_EXCEPTION_CALLBACK, NULL, LPARAM_SYSTEM_EXCEPTION_INSPECTION);				
			}
			else
			{	AOIDataCollect.SetRegionCalcThreadState(ThreadIdx, THREAD_STATE_FINISH);	}
			//AOIDataCollect.SetRegionCalcThreadCmd(ThreadIdx, THREAD_COMMAND_TO_IDLE);
			if ( NULL  != RegionCalcThreadEvent[ThreadIdx] )
			{	::SetEvent(RegionCalcThreadEvent[ThreadIdx] ); }		

			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;

			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveRegionCalcThreadLog ) 
			{
				_stprintf(strBuffer, _T("RegionCalcThreadFn[%d]: %.2f ms"), ThreadIdx+1, Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);
			}

			//if ( CpuCoreUsingNumber < CpuCoreNumber )
			{	::SetThreadPriority(RegionCalcThreadHandle[ThreadIdx], THREAD_PRIORITY_BELOW_NORMAL); }
			continue;
		}
		::Sleep(SleepTime);
	};
	AOIDataCollect.SetRegionCalcThreadState(ThreadIdx, THREAD_STATE_NONE);
	if ( NULL != RegionCalcThreadEvent[ThreadIdx] )
	{	::SetEvent(RegionCalcThreadEvent[ThreadIdx] ); }
//	::_endthreadex(0);
//#endif//Kai-20170410
	return 0;
}
//-------------------------------------------------------------------------------------//
unsigned int __stdcall ThreadSequenceThreadFn(void *pParam)
{
	bool IsOK = true;
	DWORD  SleepTime = 10;
	const size_t ThreadIdx = (size_t)pParam;
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	
	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");

	while ( true )
	{	
		ThreadCmd = AOIDataCollect.GetThreadSequenceThreadCmd();
		ThreadState = AOIDataCollect.GetThreadSequenceThreadState();
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{
			break;
		}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			AOIDataCollect.SetThreadSequenceThreadState(THREAD_STATE_IDLE);		
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

			::SetThreadPriority(ThreadSequenceThreadHandle, THREAD_PRIORITY_ABOVE_NORMAL);					
			AOIDataCollect.SetThreadSequenceThreadState(THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = AOIDataCollect.ExecThreadSequenceFn();
			QueryPerformanceCounter(&nEndTime);
			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;
			AOIDataCollect.SetThreadSequenceThreadElapseTimems(Time);			
			if ( false == IsOK )
			{	
				CString ErrorMSG = AOIDataCollect.GetErrorStringRaw();
				AOIDataCollect.SetIsSystemException(true);
				AOIDataCollect.SetThreadSequenceThreadState(THREAD_STATE_EXCEPTION);
				AOIDataCollect.SetThreadExceptionCode(AOI_EXCEPTION_THREAD_EXEC_SEQUENCE, true, ErrorMSG);
				AOIDataCollect.PostCallbackWndMessage(MSG_SYSTEM_EXCEPTION_CALLBACK, NULL, LPARAM_SYSTEM_EXCEPTION_INSPECTION);				
			}
			else
			{	AOIDataCollect.SetThreadSequenceThreadState(THREAD_STATE_FINISH);	}			
			//AOIDataCollect.SetThreadSequenceThreadCmd(THREAD_COMMAND_TO_IDLE);
			if ( NULL  != ThreadSequenceThreadEvent )
			{	::SetEvent(ThreadSequenceThreadEvent); }
			
			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveSequenceThreadLog ) 
			{
				_stprintf(strBuffer, _T("ThreadSequenceThreadFn: %.2f ms"), Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);		
			}
			::SetThreadPriority(ThreadSequenceThreadHandle, THREAD_PRIORITY_BELOW_NORMAL);
			continue;
		}
		::Sleep(SleepTime);
	};
	AOIDataCollect.SetThreadSequenceThreadState(THREAD_STATE_NONE);
	if ( NULL != ThreadSequenceThreadEvent )
	{	::SetEvent(ThreadSequenceThreadEvent); }
//	::_endthreadex(0);
	return 0;
}
//-------------------------------------------------------------------------------------//
unsigned int __stdcall FieldFrameReleaseThreadFn(void *pParam)
{
//#ifndef OFFLINE_VERSION
	bool IsOK = true;	
	DWORD  SleepTime = 10;
	const size_t ThreadIdx = (size_t)pParam;
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	
	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");

	while ( true )
	{	
		ThreadCmd = AOIDataCollect.GetFieldFrameReleaseThreadCmd();
		ThreadState = AOIDataCollect.GetFieldFrameReleaseThreadState();
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{	break;	}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			AOIDataCollect.SetFieldFrameReleaseThreadState(THREAD_STATE_IDLE);		
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

			::SetThreadPriority(FieldFrameReleaseHandle, THREAD_PRIORITY_ABOVE_NORMAL);
			AOIDataCollect.SetFieldFrameReleaseThreadState(THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = AOIDataCollect.ExecFieldFrameReleaseFn();
			QueryPerformanceCounter(&nEndTime);
			if ( false == IsOK )
			{	
				CString ErrorMSG = AOIDataCollect.GetErrorStringRaw();
				AOIDataCollect.SetIsSystemException(true);
				AOIDataCollect.SetFieldFrameReleaseThreadState(THREAD_STATE_EXCEPTION);
				AOIDataCollect.SetThreadExceptionCode(AOI_EXCEPTION_THREAD_EXEC_FRAME_FIELD_FREE, true, ErrorMSG);
				AOIDataCollect.PostCallbackWndMessage(MSG_SYSTEM_EXCEPTION_CALLBACK, NULL, LPARAM_SYSTEM_EXCEPTION_INSPECTION);				
			}
			else
			{	AOIDataCollect.SetFieldFrameReleaseThreadState(THREAD_STATE_FINISH);	}
			//AOIDataCollect.SetFieldFrameReleaseThreadCmd(THREAD_COMMAND_TO_IDLE);
			if ( NULL  != FieldFrameReleaseEvent )
			{	::SetEvent(FieldFrameReleaseEvent ); }		

			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;

			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveFieldFrameReleaseThreadLog ) 
			{
				_stprintf(strBuffer, _T("FieldFrameReleaseThreadFn[%d]: %.2f ms"), ThreadIdx+1, Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);	
			}			
			::SetThreadPriority(FieldFrameReleaseHandle, THREAD_PRIORITY_BELOW_NORMAL);
			continue;
		}
		::Sleep(SleepTime);
	};
	AOIDataCollect.SetFieldFrameReleaseThreadState(THREAD_STATE_NONE);
	if ( NULL != FieldFrameReleaseEvent )
	{	::SetEvent(FieldFrameReleaseEvent); }
//	::_endthreadex(0);
//#endif//OFFLINE_VERSION
	return 0;
}
//-------------------------------------------------------------------------------------//
unsigned int __stdcall OnlineInspectionThreadFn(void *pParam)
{
#ifndef OFFLINE_VERSION
	bool IsOK = true;	
	DWORD  SleepTime = 10;
	const size_t ThreadIdx = (size_t)pParam;
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;			
	
	double Time = 0;	
	ONLINE_STATE_MODE      OnlineStateMode;
	CAOIProject           *ProjectPtr=NULL;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	bool                   IsSystemException=false;
	TCHAR strBuffer[256]=_T("");

	while ( true )
	{	
		ThreadCmd = AOIDataCollect.GetOnlineInspectionThreadCmd();
		ThreadState = AOIDataCollect.GetOnlineInspectionThreadState();
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{	break;	}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			AOIDataCollect.SetOnlineInspectionThreadState(THREAD_STATE_IDLE);		
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

			::SetThreadPriority(OnlineInspectionHandle, THREAD_PRIORITY_ABOVE_NORMAL);
			AOIDataCollect.SetOnlineInspectionThreadState(THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = AOIDataCollect.ExecOnlineInspectionFn();
			QueryPerformanceCounter(&nEndTime);
			if ( false == IsOK )
			{	
				CString ErrorMSG = AOIDataCollect.GetErrorStringRaw();				
				AOIDataCollect.SetIsSystemException(true);				
				AOIDataCollect.SetOnlineInspectionThreadState(THREAD_STATE_EXCEPTION);
				AOIDataCollect.SetThreadExceptionCode(AOI_EXCEPTION_THREAD_EXEC_ONLINE_INSPECTION, true, ErrorMSG);
				AOIDataCollect.PostCallbackWndMessage(MSG_SYSTEM_EXCEPTION_CALLBACK, NULL, LPARAM_SYSTEM_EXCEPTION_INSPECTION);
			}
			else
			{	AOIDataCollect.SetOnlineInspectionThreadState(THREAD_STATE_FINISH);	}
			IsSystemException = AOIDataCollect.GetIsSystemException();

			PlcCtrlPtr->WriteSignalToLast_LA(false);
			PlcCtrlPtr->WriteSignalToLast_LB(false);
			PlcCtrlPtr->WriteSignalToNext_LA(false);
			PlcCtrlPtr->WriteSignalToNext_LB(false);

			AOIDataCollect.LockThread();
			OnlineStateMode = AOIDataCollect.GetOnlineStateMode();
			ProjectPtr = AOIDataCollect.GetActiveProject();
			if ( NULL != ProjectPtr )
			{	ProjectPtr->SetProjectOnlineStateMode(OnlineStateMode); }
			if ( AOIDataCollect.GetIsSystemException() == false )
			{	
				AOIDataCollect.SetTaskMode(TASK_NONE); 
				AOIDataCollect.SetOnlineTaskState(TASK_STATE_IDLE);
				AOIDataCollect.SetOnlineStateMode(ONLINE_STATE_INSPECTION_STOP);
			}			
			AOIDataCollect.UnlockThread();
			AOIDataCollect.StopLoadRepairFileThread(true);			
			AOIDataCollect.SetIsLockUIWnd(false);
			//AOIDataCollect.SetOnlineInspectionThreadCmd(THREAD_COMMAND_TO_IDLE);			
			if ( false == IsSystemException ) 
			{	AOIDataCollect.SendCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SET_DRAW_PROJECT_MODE, LPARAM_DRAW_PROJECT_MODE_NORMAL); }			
			if ( NULL  != OnlineInspectionEvent )
			{	::SetEvent(OnlineInspectionEvent ); }		

			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;
			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveOnlineInspectionThreadLog ) 
			{
				_stprintf(strBuffer, _T("OnlineInspectionThreadFn[%d]: %.2f ms"), ThreadIdx+1, Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);	
			}
			::SetThreadPriority(OnlineInspectionHandle, THREAD_PRIORITY_BELOW_NORMAL);
			if ( true == IsOK )
			{	AOIDataCollect.PostCallbackWndMessage(MSG_INSPECTION_CALLBACK, WPARAM_INSPECTION_ONLINE_FINISH, NULL); }
			continue;
		}
		::Sleep(SleepTime);
	};
	AOIDataCollect.SetOnlineInspectionThreadState(THREAD_STATE_NONE);
	if ( NULL != OnlineInspectionEvent )
	{	::SetEvent(OnlineInspectionEvent); }
//	::_endthreadex(0);
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
unsigned int __stdcall RemoveFolderThreadFn(void *pParam)
{
#ifndef OFFLINE_VERSION
	bool IsOK = true;	
	DWORD  SleepTime = 10000;//60 sec exec
	const size_t ThreadIdx = (size_t)pParam;
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	
	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");

	while ( true )
	{	
		ThreadCmd = AOIDataCollect.GetRemoveFolderThreadCmd();
		ThreadState = AOIDataCollect.GetRemoveFolderThreadState();
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{	break;	}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			AOIDataCollect.SetRemoveFolderThreadState(THREAD_STATE_IDLE);		
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
			AOIDataCollect.SetRemoveFolderThreadState(THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = AOIDataCollect.ExecRemoveFolderFn();
			QueryPerformanceCounter(&nEndTime);			
			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;
			if ( false == IsOK )
			{
				CString ErrorMSG = AOIDataCollect.GetErrorStringRaw();
				AOIDataCollect.SetThreadExceptionCode(AOI_EXCEPTION_THREAD_EXEC_REMOVE_FOLDER, true, ErrorMSG);	
			}
			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveRemoveFolderThreadLog ) 
			{
				_stprintf(strBuffer, _T("RemoveFolderThreadFn[%d]: %.2f ms"), ThreadIdx+1, Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);			
			}
		}
		::Sleep(SleepTime);
	};
	AOIDataCollect.SetRemoveFolderThreadState(THREAD_STATE_NONE);
	if ( NULL != RemoveFolderThreadEvent )
	{	::SetEvent(RemoveFolderThreadEvent); }
//	::_endthreadex(0);
#endif//OFFLINE_VERSION
	return 0;
}
//-------------------------------------------------------------------------------------//
unsigned int __stdcall ConveyerAutoRunThreadFn(void *pParam)
{
#ifndef OFFLINE_VERSION
	bool IsOK = true;
	DWORD  SleepTime = 10;
	const size_t LaneIdx = (size_t)pParam;	
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	LANE_ID LaneID = AOIDataCollect.GetConveyerAutoRunLaneID(LaneIdx);

	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");
	while ( true )
	{	
		ThreadCmd = AOIDataCollect.GetConveyerAutoRunThreadCmd(LaneIdx);
		ThreadState = AOIDataCollect.GetConveyerAutoRunThreadState(LaneIdx);
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{
			break;
		}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			AOIDataCollect.SetConveyerAutoRunThreadState(LaneIdx, THREAD_STATE_IDLE);		
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

			::SetThreadPriority(ConveyerAutoRunThreadHandle[LaneIdx], THREAD_PRIORITY_ABOVE_NORMAL);
			AOIDataCollect.SetConveyerAutoRunThreadState(LaneIdx, THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = AOIDataCollect.ExecConveyerAutoRunFn(LaneIdx);
			QueryPerformanceCounter(&nEndTime);
			if ( false == IsOK )
			{	
				CString ErrorMSG = AOIDataCollect.GetErrorStringRaw();
				AOIDataCollect.SetExceptionLaneID(LaneID);
				//AOIDataCollect.SetIsSystemException(true);//控制權由ThreadSequenceThreadFn為主
				AOIDataCollect.SetConveyerAutoRunThreadState(LaneIdx, THREAD_STATE_EXCEPTION);
				AOIDataCollect.SetThreadExceptionCode(AOI_EXCEPTION_THREAD_EXEC_CONVEYER_AUTO_RUN, true, ErrorMSG);
				//AOIDataCollect.PostCallbackWndMessage(MSG_SYSTEM_EXCEPTION_CALLBACK, NULL, LPARAM_SYSTEM_EXCEPTION_INSPECTION);				
			}
			else
			{	AOIDataCollect.SetConveyerAutoRunThreadState(LaneIdx, THREAD_STATE_FINISH);	}
			//AOIDataCollect.SetConveyerAutoRunThreadCmd(LaneIdx, THREAD_COMMAND_TO_IDLE);
			if ( NULL  != ConveyerAutoRunThreadEvent[LaneIdx] )
			{	::SetEvent(ConveyerAutoRunThreadEvent[LaneIdx]); }		

			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;
			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveConveyerPreRunThreadLog ) 
			{
				if ( true == IsOK )
				{	_stprintf(strBuffer, _T("ConveyerAutoRunThreadFn OK[%d]: %.2f ms"), LaneIdx+1, Time); }
				else
				{	_stprintf(strBuffer, _T("ConveyerAutoRunThreadFn Exception[%d]: %.2f ms"), LaneIdx+1, Time); }
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);
			}
			::SetThreadPriority(ConveyerAutoRunThreadHandle[LaneIdx], THREAD_PRIORITY_BELOW_NORMAL);
			continue;
		}
		::Sleep(SleepTime);
	};
	AOIDataCollect.SetConveyerAutoRunThreadState(LaneIdx, THREAD_STATE_NONE);
	if ( NULL != ConveyerAutoRunThreadEvent[LaneIdx] )
	{	::SetEvent(ConveyerAutoRunThreadEvent[LaneIdx]); }
//	::_endthreadex(0);
#endif
	return 0;
}
//-------------------------------------------------------------------------------------//
unsigned int __stdcall LoadRepairFileThreadFn(void *pParam)
{
#ifndef OFFLINE_VERSION
	bool IsOK = true;	
	DWORD  SleepTime = 100;//	
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	
	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");

	while ( true )
	{	
		ThreadCmd = AOIDataCollect.GetLoadRepairFileThreadCmd();
		ThreadState = AOIDataCollect.GetLoadRepairFileThreadState();
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{	break;	}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			AOIDataCollect.SetLoadRepairFileThreadState(THREAD_STATE_IDLE);		
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
			AOIDataCollect.SetLoadRepairFileThreadState(THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = AOIDataCollect.ExecLoadRepairFileFn();
			if ( false == IsOK )
			{	
				CString ErrorMSG = AOIDataCollect.GetErrorStringRaw();
				AOIDataCollect.SetIsSystemException(true);
				AOIDataCollect.SetLoadRepairFileThreadState(THREAD_STATE_EXCEPTION);
				AOIDataCollect.SetThreadExceptionCode(AOI_EXCEPTION_THREAD_EXEC_LOAD_REPAIR_FILE, true, ErrorMSG);
				AOIDataCollect.PostCallbackWndMessage(MSG_SYSTEM_EXCEPTION_CALLBACK, NULL, LPARAM_SYSTEM_EXCEPTION_INSPECTION);				
			}			
			QueryPerformanceCounter(&nEndTime);			
			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;
			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveLoadRepairFileThreadLog ) 
			{
				_stprintf(strBuffer, _T("ExecLoadRepairFileFn: %.2f ms"), Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);			
			}
		}
		::Sleep(SleepTime);
	};
	AOIDataCollect.SetLoadRepairFileThreadState(THREAD_STATE_NONE);
	if ( NULL != LoadRepairFileThreadEvent )
	{	::SetEvent(LoadRepairFileThreadEvent); }
#endif//OFFLINE_VERSION	
	return 0;
}
//-------------------------------------------------------------------------------------//
unsigned int __stdcall RepairResultSignalThreadFn(void *pParam)
{
#ifndef OFFLINE_VERSION
	bool IsOK = true;	
	DWORD  SleepTime = 10;//60 sec exec
	const size_t ThreadIdx = (size_t)pParam;
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	
	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");

	while ( true )
	{	
		ThreadCmd = AOIDataCollect.GetRepairResultSignalThreadCmd();
		ThreadState = AOIDataCollect.GetRepairResultSignalThreadState();
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{	break;	}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			AOIDataCollect.SetRepairResultSignalThreadState(THREAD_STATE_IDLE);		
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
			AOIDataCollect.SetRepairResultSignalThreadState(THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = AOIDataCollect.ExecRepairResultSignalFn();
			QueryPerformanceCounter(&nEndTime);			
			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;
			if ( false == IsOK )
			{
				CString ErrorMSG = AOIDataCollect.GetErrorStringRaw();
				AOIDataCollect.SetThreadExceptionCode(AOI_EXCEPTION_THREAD_EXEC_REPAIR_RESULT_SIGNAL, true, ErrorMSG); 
			}
			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveRepairResultSignalThreadLog ) 
			{
				_stprintf(strBuffer, _T("ExecRepairResultSignalFn[%d]: %.2f ms"), ThreadIdx+1, Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);			
			}
		}
		::Sleep(SleepTime);
	};
	AOIDataCollect.SetRepairResultSignalThreadState(THREAD_STATE_NONE);
	if ( NULL != RepairResultSignalThreadEvent )
	{	::SetEvent(RepairResultSignalThreadEvent); }
#endif//OFFLINE_VERSION
	return 0;
}
//-------------------------------------------------------------------------------------//
unsigned int __stdcall SimpleJobThreadFn(void *pParam)
{
#ifndef OFFLINE_VERSION
	bool IsOK = true;	
	DWORD  SleepTime = 10;//60 sec exec	
	THREAD_STATE_MODE   ThreadState = THREAD_STATE_NONE;
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_NONE;		
	
	double Time = 0;
	LARGE_INTEGER          nStartTime;
	LARGE_INTEGER          nEndTime;
	TCHAR strBuffer[256]=_T("");

	while ( true )
	{	
		ThreadCmd = AOIDataCollect.GetSimpleJobThreadCmd();
		ThreadState = AOIDataCollect.GetSimpleJobThreadState();
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd )
		{	break;	}
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			AOIDataCollect.SetSimpleJobThreadState(THREAD_STATE_IDLE);		
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

			AOIDataCollect.SetSimpleJobThreadState(THREAD_STATE_RUNNING);		
			QueryPerformanceCounter(&nStartTime);
			IsOK = AOIDataCollect.ExecSimpleJobFn();
			QueryPerformanceCounter(&nEndTime);
			
			if ( false == IsOK )
			{	
				CString ErrorMSG = AOIDataCollect.GetErrorStringRaw();
				AOIDataCollect.SetIsSystemException(true);
				AOIDataCollect.SetSimpleJobThreadState(THREAD_STATE_EXCEPTION);
				AOIDataCollect.SetThreadExceptionCode(AOI_EXCEPTION_THREAD_EXEC_SIMPLE_JOB, true, ErrorMSG);
				AOIDataCollect.PostCallbackWndMessage(MSG_SYSTEM_EXCEPTION_CALLBACK, NULL, LPARAM_SYSTEM_EXCEPTION_INSPECTION);				
			}
			else
			{	AOIDataCollect.SetSimpleJobThreadState(THREAD_STATE_FINISH);	}
			//AOIDataCollect.SetSimpleJobThreadCmd(THREAD_COMMAND_TO_IDLE);
			if ( NULL != SimpleJobThreadEvent )
			{	::SetEvent(SimpleJobThreadEvent); }			

			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;
			const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
			if ( FN_ENABLE == SystemParam.m_SaveSimpleJobThreadLog ) 
			{
				_stprintf(strBuffer, _T("SimpleJobThreadFn: %.2f ms"), Time);
				AOIDataCollect.SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, strBuffer);
			}
			continue;
		}
		::Sleep(SleepTime);
	};
	AOIDataCollect.SetSimpleJobThreadState(THREAD_STATE_NONE);
	if ( NULL != SimpleJobThreadEvent )
	{	::SetEvent(SimpleJobThreadEvent); }
#endif//OFFLINE_VERSION
	return 0;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::IdleAllThread(bool bWait)//停止所有執行緒至Idle狀態
{
	size_t i = 0;	
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_IDLE;

	SetInspectionDrawing(false);	
	//設定取像完畢, 避免其他執行緒卡住	
	CameraCtrl.SetAllCameraExposureFinishEvent();
	CameraCtrl.SetAllCameraGrabFinishEvent();
	
	for ( i=0; i<MAX_THREAD_COUNT_SLICE_FILL; i++ )
	{	SetSliceFillThreadCmd(i, ThreadCmd); }	

	for ( i=0; i<MAX_THREAD_COUNT_FRAME_MERGE; i++ )
	{	SetFrameMergeThreadCmd(i, ThreadCmd); }		

	for ( i=0; i<MAX_THREAD_COUNT_FIELD_MERGE; i++ )
	{	SetFieldMergeThreadCmd(i, ThreadCmd); }		

	for ( i=0; i<MAX_THREAD_COUNT_FRAME_LOAD; i++ )
	{	SetFrameLoadThreadCmd(i, ThreadCmd); }		

	for ( i=0; i<MAX_THREAD_COUNT_FIELD_CALC; i++ )
	{	SetFieldCalcThreadCmd(i, ThreadCmd); }		

	for ( i=0; i<MAX_THREAD_COUNT_REGION_CALC; i++ )
	{	SetRegionCalcThreadCmd(i, ThreadCmd); }

	for ( i=0; i<MAX_THREAD_COUNT_LANE_AUTO_RUN; i++ )
	{	SetConveyerAutoRunThreadCmd(i, ThreadCmd); }

	SetSimpleJobThreadCmd(ThreadCmd);
	SetThreadSequenceThreadCmd(ThreadCmd);
	SetFieldFrameReleaseThreadCmd(ThreadCmd);
	SetOnlineInspectionThreadCmd(ThreadCmd);	
	//SetITSProcThreadCmd(ThreadCmd);
	//SetRepairResultSignalThreadCmd(ThreadCmd);
	MotionCtrlPtr->SetMotionThreadCmd_GoStop(ThreadCmd);
	if ( true == bWait )
	{	WaitForAllThreadIdle();		}

	//SetTaskMode(TASK_NONE);
	//SetThreadGrabMode(THREAD_GRAB_NONE);
	//SetOnlineStateMode(ONLINE_STATE_INSPECTION_STOP);
	//SetOnlineStateMode_Lane(ONLINE_STATE_INSPECTION_STOP, LaneID);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::IdleAllCalcThread(bool bWait)//停止所有計算執行緒至Idle狀態
{
	size_t i = 0;	
	THREAD_COMMAND_MODE ThreadCmd = THREAD_COMMAND_TO_IDLE;	
	for ( i=0; i<MAX_THREAD_COUNT_SLICE_FILL; i++ )
	{	SetSliceFillThreadCmd(i, ThreadCmd); }
	for ( i=0; i<MAX_THREAD_COUNT_FRAME_MERGE; i++ )
	{	SetFrameMergeThreadCmd(i, ThreadCmd); }
	for ( i=0; i<MAX_THREAD_COUNT_FIELD_MERGE; i++ )
	{	SetFieldMergeThreadCmd(i, ThreadCmd); }
	for ( i=0; i<MAX_THREAD_COUNT_FRAME_LOAD; i++ )
	{	SetFrameLoadThreadCmd(i, ThreadCmd); }
	for ( i=0; i<MAX_THREAD_COUNT_FIELD_CALC; i++ )
	{	SetFieldCalcThreadCmd(i, ThreadCmd); }		
	for ( i=0; i<MAX_THREAD_COUNT_REGION_CALC; i++ )
	{	SetRegionCalcThreadCmd(i, ThreadCmd); }
	if ( true == bWait )
	{	WaitForAllCalcThreadIdle();	}
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataCollect::GetThreadStateModeText(THREAD_STATE_MODE State)
{
	switch ( State )
	{
	case THREAD_STATE_NONE:			::_tcscpy(m_ThreadStateModeText, _T("None"));	break;
	case THREAD_STATE_IDLE:			::_tcscpy(m_ThreadStateModeText, _T("Idle"));	break;
	case THREAD_STATE_RUNNING:		::_tcscpy(m_ThreadStateModeText, _T("Running"));	break;
	case THREAD_STATE_FINISH:		::_tcscpy(m_ThreadStateModeText, _T("Finish"));	break;
	case THREAD_STATE_EXCEPTION:	::_tcscpy(m_ThreadStateModeText, _T("Exeption"));	break;
	default:	::_tcscpy(m_ThreadStateModeText, _T("Not-Defined"));	break;
	}
	return m_ThreadStateModeText;		
}
//-------------------------------------------------------------------------------------//
LPCTSTR CAOIDataCollect::GetThreadCommandModeText(THREAD_COMMAND_MODE Cmd)
{	
	switch ( Cmd )
	{
	case THREAD_COMMAND_TO_NONE:		::_tcscpy(m_ThreadCommandModeText, _T("None"));	break;
	case THREAD_COMMAND_TO_EXIT:		::_tcscpy(m_ThreadCommandModeText, _T("To Exit"));	break;
	case THREAD_COMMAND_TO_IDLE:		::_tcscpy(m_ThreadCommandModeText, _T("To Idle"));	break;
	case THREAD_COMMAND_TO_RUN:			::_tcscpy(m_ThreadCommandModeText, _T("To Run"));	break;	
	case THREAD_COMMAND_WAIT_EXCEPTION:	::_tcscpy(m_ThreadCommandModeText, _T("Wait for Exception"));	break;
	default:	::_tcscpy(m_ThreadCommandModeText, _T("Not-Defined"));	break;
	}
	return m_ThreadCommandModeText;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetIsAllCalcThreadStop() const//取得是否所有計算執行緒停止
{
	return m_IsAllCalcThreadStop;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetIsAllCalcThreadStop(bool Value)//設定是否所有計算執行緒停止
{
	if ( m_IsAllCalcThreadStop == Value ) { return; }
	LockThread();
	m_IsAllCalcThreadStop = Value;
	UnlockThread();	 
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::ResetIsAnyCalcThreadWorking()//復歸計算的執行緒工作中
{
	SetIsAnyCalcThreadWorking(false);
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetIsAnyCalcThreadWorking() const//取得計算的執行緒工作中
{
	return m_IsAnyCalcThreadWorking;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetIsAnyCalcThreadWorking(bool Value)//設定計算的執行緒工作中
{
	if ( m_IsAnyCalcThreadWorking == Value ) { return; }
	LockThread();
	m_IsAnyCalcThreadWorking = Value;
	UnlockThread();
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateAllThread()//建立所有執行緒
{
	bool IsOK = true;	
	SetIsAllCalcThreadStop(false);
	if ( CreateSystemRunThread() == false )
	{
		IsOK = false;
		JetAPI::ShowMessageBox(GetErrorString());	
	}

	if ( CreateSliceFillThread() == false )
	{
		IsOK = false;
		JetAPI::ShowMessageBox(GetErrorString());	
	}	

	if ( CreateFrameMergeThread() == false )
	{
		IsOK = false;
		JetAPI::ShowMessageBox(GetErrorString());	
	}

	if ( CreateFieldMergeThread() == false )
	{
		IsOK = false;
		JetAPI::ShowMessageBox(GetErrorString());	
	}

	if ( CreateFrameLoadThread() == false )
	{
		IsOK = false;
		JetAPI::ShowMessageBox(GetErrorString());	
	}

	if ( CreateFieldCalcThread() == false )
	{
		IsOK = false;
		JetAPI::ShowMessageBox(GetErrorString());	
	}

	if ( CreateRegionCalcThread() == false )
	{
		IsOK = false;
		JetAPI::ShowMessageBox(GetErrorString());	
	}

	if ( CreateThreadSequenceThread() == false )
	{
		IsOK = false;
		JetAPI::ShowMessageBox(GetErrorString());
	}
	
	if ( CreateOnlineInspectionThread() == false )
	{
		IsOK = false;
		JetAPI::ShowMessageBox(GetErrorString());
	}
	
	if ( CreateFieldFrameReleaseThread() == false )
	{
		IsOK = false;
		JetAPI::ShowMessageBox(GetErrorString());
	}

	if ( CreateRemoveFolderThread() == false )
	{
		IsOK = false;
		JetAPI::ShowMessageBox(GetErrorString());
	}
	
	if ( CreateConveyerAutoRunThread() == false )
	{
		IsOK = false;
		JetAPI::ShowMessageBox(GetErrorString());
	}	

	if ( CreateLoadRepairFileThread() == false )
	{
		IsOK = false;
		JetAPI::ShowMessageBox(GetErrorString());
	}

	if ( CreateRepairResultSignalThread() == false )
	{
		IsOK = false;
		JetAPI::ShowMessageBox(GetErrorString());
	}	
	/*
	//由於MES執行緒是在MES_IMP內執行, 切換MES_IMP時需要重建執行緒, 這裡就不建立
	if ( CreateMESProcThread() == false ) 
	{
		IsOK = false;
		JetAPI::ShowMessageBox(GetErrorString());
	}
	*/
	if ( CreateSimpleJobThread() == false )
	{
		IsOK = false;
		JetAPI::ShowMessageBox(GetErrorString());
	}

	if ( MotionCtrlPtr->CreateMotionCtrlThread() == false )
	{
		IsOK = false;
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());	
	}
#ifndef HASI_DISABLE
	if (true == GetHASI_Enable()) {
		if (CreateHASI_MoniterThread() == false) {
			IsOK = false;
			JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		}
	}
#endif // !HASI_DISABLE
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::UpdateThreadAffinityMask()//更新執行緒的CPU親和性
{
	int   i=0;
	int   ThreadIdx=0;
	DWORD_PTR AffinityMask = GetThreadAffinityMask();
	for ( i=0; i<MAX_THREAD_COUNT_FRAME_MERGE; i++ )
	{
		ThreadIdx = i;
		if ( NULL != FrameMergeThreadHandle[ThreadIdx] )
		{	::SetThreadAffinityMask(FrameMergeThreadHandle[ThreadIdx], AffinityMask);	}
	}	
	for ( i=0; i<MAX_THREAD_COUNT_FIELD_MERGE; i++ )
	{
		ThreadIdx = i;
		if ( NULL != FieldMergeThreadHandle[ThreadIdx] )
		{	::SetThreadAffinityMask(FieldMergeThreadHandle[ThreadIdx], AffinityMask);	}
	}
	for ( i=0; i<MAX_THREAD_COUNT_FRAME_LOAD; i++ )
	{
		ThreadIdx = i;
		if ( NULL != FrameLoadThreadHandle[ThreadIdx] )
		{	::SetThreadAffinityMask(FrameLoadThreadHandle[ThreadIdx], AffinityMask);	}
	}	
	for ( i=0; i<MAX_THREAD_COUNT_FIELD_CALC; i++ )
	{
		ThreadIdx = i;
		if ( NULL != FieldCalcThreadHandle[ThreadIdx] )
		{	::SetThreadAffinityMask(FieldCalcThreadHandle[ThreadIdx], AffinityMask);	}
	}
	for ( i=0; i<MAX_THREAD_COUNT_REGION_CALC; i++ )
	{
		ThreadIdx = i;
		if ( NULL != RegionCalcThreadHandle[ThreadIdx] )
		{	::SetThreadAffinityMask(RegionCalcThreadHandle[ThreadIdx], AffinityMask);	}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteAllThread()//刪除所有執行緒
{
	bool IsOK = true;

	SetInspectionDrawing(false);
	SetThreadGrabMode(THREAD_GRAB_NONE);

	CameraCtrl.SetAllCameraExposureFinishEvent();
	CameraCtrl.SetAllCameraGrabFinishEvent();

	DeleteSliceFillThread();	
	DeleteFrameMergeThread();	
	DeleteFieldMergeThread();
	DeleteFrameLoadThread();
	DeleteFieldCalcThread();
	DeleteRegionCalcThread();
	DeleteThreadSequenceThread();
	DeleteFieldFrameReleaseThread();
	DeleteOnlineInspectionThread();
	DeleteRemoveFolderThread();
	DeleteConveyerAutoRunThread();
	DeleteRepairResultSignalThread();
	DeleteLoadRepairFileThread();
	DeleteMESProcThread();	
	DeleteSimpleJobThread();
	DeleteSystemRunThread();

	MotionCtrlPtr->DeleteMotionCtrlThread();
	//PlcCtrlPtr->DeletePLCPollingThread();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckAllCalcThreadStateFinish()//確認所有執行緒狀態
{
	THREAD_STATE_MODE State;
	if ( CheckSliceFillThreadStateFinish() == false )
	{	return false; }	
	if ( CheckFrameMergeThreadStateFinish() == false )
	{	return false; }
	if ( CheckFieldMergeThreadStateFinish() == false )
	{	return false; }
	if ( CheckFrameLoadThreadStateFinish() == false )
	{	return false; }
	if ( CheckFieldCalcThreadStateFinish() == false )
	{	return false; }
	if ( CheckRegionCalcThreadStateFinish() == false )
	{	return false; }
	if ( GetFieldFrameReleaseThreadNeedCheck() == true )
	{
		if ( CheckFieldFrameReleaseThreadStateFinish() == false )
		{	return false; }
	}
//	if ( CheckThreadSequenceThreadStateFinish() == false )
//	{	return false; }
//	if ( CheckOnlineInspectionThreadStateFinish() == false )
//	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForAllThreadFinish()//等待所有執行緒結束
{
	if ( WaitForSliceFillThreadFinish() == false )
	{	return false; }	
	if ( WaitForFrameMergeThreadFinish() == false )
	{	return false; }
	if ( WaitForFieldMergeThreadFinish() == false )
	{	return false; }	
	if ( WaitForFieldCalcThreadFinish() == false )
	{	return false; }
	if ( WaitForRegionCalcThreadFinish() == false )
	{	return false; }
	if ( WaitForFieldFrameReleaseThreadFinish() == false )
	{	return false; }
//	if ( WaitForThreadSequenceThreadFinish() == false )
//	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForAllThreadIdle()//等待所有執行緒停止
{
	bool bIsOK = true;
	if ( WaitForSliceFillThreadIdle() == false ) 
	{	bIsOK = false; }
	if ( WaitForFrameMergeThreadIdle() == false )
	{	bIsOK = false; }
	if ( WaitForFieldMergeThreadIdle() == false )
	{	bIsOK = false; }
	if ( WaitForFrameLoadThreadIdle() == false ) 
	{	bIsOK = false; }
	if ( WaitForFieldCalcThreadIdle() == false ) 
	{	bIsOK = false; }	
	if ( WaitForRegionCalcThreadIdle() == false ) 
	{	bIsOK = false; }
	if ( WaitForSimpleJobThreadIdle() == false ) 
	{	bIsOK = false; }	
	if ( WaitForConveyerAutoRunThreadIdle() == false ) 
	{	bIsOK = false; }
	if ( WaitForThreadSequenceThreadIdle() == false ) 
	{	bIsOK = false; }
	if ( WaitForFieldFrameReleaseThreadIdle() == false ) 
	{	bIsOK = false; }
	//WaitForOnlineInspectionThreadIdle();
	//WaitForITSProcThreadIdle();
	//WaitForRepairResultSignalThreadIdle();		
	//MotionCtrlPtr->SetMotionThreadCmd_GoStop(ThreadCmd);
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForAllThreadStop()//等待所有執行緒停止
{
	bool bIsOK = true;
	if ( WaitForSliceFillThreadStop() == false ) 
	{	bIsOK = false; }
	if ( WaitForFrameMergeThreadStop() == false )
	{	bIsOK = false; }
	if ( WaitForFieldMergeThreadStop() == false )
	{	bIsOK = false; }
	if ( WaitForFrameLoadThreadStop() == false ) 
	{	bIsOK = false; }
	if ( WaitForFieldCalcThreadStop() == false ) 
	{	bIsOK = false; }
	if ( WaitForRegionCalcThreadStop() == false ) 
	{	bIsOK = false; }
	if ( WaitForConveyerAutoRunThreadStop() == false ) 
	{	bIsOK = false; }
	if ( WaitForThreadSequenceThreadStop() == false ) 
	{	bIsOK = false; }
	if ( WaitForFieldFrameReleaseThreadStop() == false ) 
	{	bIsOK = false; }
	//WaitForOnlineInspectionThreadStop();
	//WaitForITSProcThreadStop();
	//WaitForRepairResultSignalThreadStop();		
	//MotionCtrlPtr->SetMotionThreadCmd_GoStop(ThreadCmd);
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForAllCalcThreadIdle()//等待所有計算執行緒停止
{
	bool bIsOK = true;
	if ( WaitForSliceFillThreadIdle() == false ) 
	{	bIsOK = false; }
	if ( WaitForFrameMergeThreadIdle() == false )
	{	bIsOK = false; }
	if ( WaitForFieldMergeThreadIdle() == false )
	{	bIsOK = false; }
	if ( WaitForFrameLoadThreadIdle() == false ) 
	{	bIsOK = false; }
	if ( WaitForFieldCalcThreadIdle() == false ) 
	{	bIsOK = false; }	
	if ( WaitForRegionCalcThreadIdle() == false ) 
	{	bIsOK = false; }	
	if ( WaitForFieldFrameReleaseThreadIdle() == false ) 
	{	bIsOK = false; }

	//WaitForConveyerAutoRunThreadIdle();
	//WaitForThreadSequenceThreadIdle();
	//WaitForOnlineInspectionThreadIdle();
	//WaitForITSProcThreadIdle();
	//WaitForRepairResultSignalThreadIdle();		
	//MotionCtrlPtr->SetMotionThreadCmd_GoStop(ThreadCmd);
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForAllCalcThreadStop()//等待所有計算執行緒停止
{
	bool bIsOK = true;
	SetIsAllCalcThreadStop(true);
	if ( WaitForSliceFillThreadStop() == false ) 
	{	bIsOK = false; }
	if ( WaitForFrameMergeThreadStop() == false )
	{	bIsOK = false; }
	if ( WaitForFieldMergeThreadStop() == false )
	{	bIsOK = false; }
	if ( WaitForFrameLoadThreadStop() == false ) 
	{	bIsOK = false; }
	if ( WaitForFieldCalcThreadStop() == false ) 
	{	bIsOK = false; }
	if ( WaitForRegionCalcThreadStop() == false ) 
	{	bIsOK = false; }	
	if ( WaitForFieldFrameReleaseThreadStop() == false ) 
	{	bIsOK = false; }

	//WaitForConveyerAutoRunThreadStop();
	//WaitForThreadSequenceThreadStop();
	//WaitForOnlineInspectionThreadStop();
	//WaitForITSProcThreadStop();
	//WaitForRepairResultSignalThreadStop();		
	//MotionCtrlPtr->SetMotionThreadCmd_GoStop(ThreadCmd);
	return bIsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::IdleAllProjectThread(bool bWait)//停止所有專案執行緒至Idle狀態
{
	CAOIProject *ProjectPtr = NULL;
	const size_t ProjectCount=GetProjectPtrCount();
	for ( size_t i=0; i<ProjectCount; i++ )
	{
		ProjectPtr = GetProjectPtr(i, false);
		if ( NULL == ProjectPtr ) { continue; }
		ProjectPtr->IdleProjectSaveFieldThread(bWait);
		ProjectPtr->IdleProjectCopyFileThread(bWait);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveAllThreadState(LPCTSTR filename)//儲存所有執行緒狀態
{
	if ( SaveAllThreadStateFn(filename) == false )
	{
		SetSystemExceptionCode_FileWrite();;
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveAllThreadStateFn(LPCTSTR filename)//儲存所有執行緒狀態
{	
	TCHAR   TMode[32] = _T("");
	_tcscpy(TMode, _T("w+"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	FILE *pfile = ::_tfopen(filename, TMode);
	if ( NULL == pfile )
	{
		this->m_ErrorString.Format(_T("Error Save All Thread State Fault(%s)"), filename);
		return false; 
	}	

	int i = 0;
	CString StateText, CmdText;

	//系統運作執行緒	
	::_ftprintf(pfile, _T("Sysrem Run Thread\n"));	
	StateText = GetThreadStateModeText(m_SystemRunThreadState);
	CmdText = GetThreadCommandModeText(m_SystemRunThreadCmd);
	::_ftprintf(pfile, _T("TH:%d, State::%s, Cmd::%s\n"), 1, (LPCTSTR)StateText, (LPCTSTR)CmdText);	
	::_ftprintf(pfile, _T("\n"));	

	//相機圖填滿執行緒
	::_ftprintf(pfile, _T("Slice Fill Thread\n"));
	for ( i=0; i<MAX_THREAD_COUNT_SLICE_FILL; i++ )
	{
		StateText = GetThreadStateModeText(m_SliceFillThreadState[i]);
		CmdText = GetThreadCommandModeText(m_SliceFillThreadCmd[i]);
		::_ftprintf(pfile, _T("TH:%d, State::%s, Cmd::%s\n"), i+1, (LPCTSTR)StateText, (LPCTSTR)CmdText);
	}
	::_ftprintf(pfile, _T("\n"));	

	//影像合併執行緒
	::_ftprintf(pfile, _T("Frame Merge Thread\n"));
	for ( i=0; i<MAX_THREAD_COUNT_FRAME_MERGE; i++ )
	{
		StateText = GetThreadStateModeText(m_FrameMergeThreadState[i]);
		CmdText = GetThreadCommandModeText(m_FrameMergeThreadCmd[i]);
		::_ftprintf(pfile, _T("TH:%d, State::%s, Cmd::%s\n"), i+1, (LPCTSTR)StateText, (LPCTSTR)CmdText);
	}
	::_ftprintf(pfile, _T("\n"));

	//區域合併執行緒
	::_ftprintf(pfile, _T("Field Merge Thread\n"));
	for ( i=0; i<MAX_THREAD_COUNT_FIELD_MERGE; i++ )
	{
		StateText = GetThreadStateModeText(m_FieldMergeThreadState[i]);
		CmdText = GetThreadCommandModeText(m_FieldMergeThreadCmd[i]);
		::_ftprintf(pfile, _T("TH:%d, State::%s, Cmd::%s\n"), i+1, (LPCTSTR)StateText, (LPCTSTR)CmdText);
	}
	::_ftprintf(pfile, _T("\n"));	

	//影像載入執行緒
	::_ftprintf(pfile, _T("Frame Load Thread\n"));
	for ( i=0; i<MAX_THREAD_COUNT_FRAME_LOAD; i++ )
	{
		StateText = GetThreadStateModeText(m_FrameLoadThreadState[i]);
		CmdText = GetThreadCommandModeText(m_FrameLoadThreadCmd[i]);
		::_ftprintf(pfile, _T("TH:%d, State::%s, Cmd::%s\n"), i+1, (LPCTSTR)StateText, (LPCTSTR)CmdText);
	}
	::_ftprintf(pfile, _T("\n"));

	//視野計算執行緒
	::_ftprintf(pfile, _T("Field Calc Thread\n"));
	for ( i=0; i<MAX_THREAD_COUNT_FIELD_CALC; i++ )
	{
		StateText = GetThreadStateModeText(m_FieldCalcThreadState[i]);
		CmdText = GetThreadCommandModeText(m_FieldCalcThreadCmd[i]);
		::_ftprintf(pfile, _T("TH:%d, State::%s, Cmd::%s\n"), i+1, (LPCTSTR)StateText, (LPCTSTR)CmdText);
	}
	::_ftprintf(pfile, _T("\n"));

	//區域計算執行緒
	::_ftprintf(pfile, _T("Region Calc Thread\n"));
	for ( i=0; i<MAX_THREAD_COUNT_REGION_CALC; i++ )
	{
		StateText = GetThreadStateModeText(m_RegionCalcThreadState[i]);
		CmdText = GetThreadCommandModeText(m_RegionCalcThreadCmd[i]);
		::_ftprintf(pfile, _T("TH:%d, State::%s, Cmd::%s\n"), i+1, (LPCTSTR)StateText, (LPCTSTR)CmdText);
	}
	::_ftprintf(pfile, _T("\n"));

	//執行緒程序執行緒
	::_ftprintf(pfile, _T("Thread Sequence Thread\n"));	
	StateText = GetThreadStateModeText(m_ThreadSequenceThreadState);
	CmdText = GetThreadCommandModeText(m_ThreadSequenceThreadCmd);
	::_ftprintf(pfile, _T("TH:%d, State::%s, Cmd::%s\n"), 1, (LPCTSTR)StateText, (LPCTSTR)CmdText);
	::_ftprintf(pfile, _T("\n"));

	//區域影像釋放執行緒
	::_ftprintf(pfile, _T("Field Freame Release Thread\n"));	
	StateText = GetThreadStateModeText(m_FieldFrameReleaseThreadState);
	CmdText = GetThreadCommandModeText(m_FieldFrameReleaseThreadCmd);
	::_ftprintf(pfile, _T("TH:%d, State::%s, Cmd::%s\n"), 1, (LPCTSTR)StateText, (LPCTSTR)CmdText);
	::_ftprintf(pfile, _T("\n"));

	//線上檢測
	const ONLINE_STATE_MODE OnlineState = this->GetOnlineStateMode();
	CString OnlineStateText = AOIDataDefine.GetOnlineStateText(OnlineState);
	::_ftprintf(pfile, _T("Thread Online Inspection\n"));	
	StateText = GetThreadStateModeText(m_OnlineInspectionThreadState);
	CmdText = GetThreadCommandModeText(m_OnlineInspectionThreadCmd);
	::_ftprintf(pfile, _T("TH:%d, State::%s, Cmd::%s, Step:%d\n"), 1, (LPCTSTR)StateText, (LPCTSTR)CmdText, OnlineState);
	::_ftprintf(pfile, _T("\n"));	

	//線上調機移除
	::_ftprintf(pfile, _T("Thread Online Tuning Remove\n"));	
	StateText = GetThreadStateModeText(m_RemoveFolderThreadState);
	CmdText = GetThreadCommandModeText(m_RemoveFolderThreadCmd);
	::_ftprintf(pfile, _T("TH:%d, State::%s, Cmd::%s\n"), 1, (LPCTSTR)StateText, (LPCTSTR)CmdText);
	::_ftprintf(pfile, _T("\n"));
	
	//軌道自動運轉
	::_ftprintf(pfile, _T("Conveyer Auto Run Thread\n"));
	for ( i=0; i<MAX_THREAD_COUNT_LANE_AUTO_RUN; i++ )
	{
		StateText = GetThreadStateModeText(m_ConveyerAutoRunThreadState[i]);
		CmdText = GetThreadCommandModeText(m_ConveyerAutoRunThreadCmd[i]);
		::_ftprintf(pfile, _T("TH:%d, State::%s, Cmd::%s\n"), i+1, (LPCTSTR)StateText, (LPCTSTR)CmdText);
	}
	::_ftprintf(pfile, _T("\n"));

	//系統異常
	const bool bSystemException = GetIsSystemException();	
	if ( true == bSystemException )
	{	::_ftprintf(pfile, _T("System Exception:Yes\n"));	 }
	else
	{	::_ftprintf(pfile, _T("System Exception:No\n"));	 }	
	::_ftprintf(pfile, _T("\n"));	
	
	::fclose(pfile); pfile = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecSystemRunFn()//執行系統運作執行緒-常駐運作	
{
#ifndef OFFLINE_VERSION
	if ( GetIsSystemReleased() == true ) 
	{	return true; }
	ExecSystemRun_OnlineRunLock();		
	ExecSystemRun_AutoSwitchToOnlineView();
	ExecSystemRun_AutoSwitchToOnlineRemoteCtrlMode();
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateSystemRunThread()//建立系統運作執行緒
{
	if ( DeleteSystemRunThread() == false ) { return false; }	
	m_SystemRunThreadCmd = THREAD_COMMAND_TO_IDLE;	

	SystemRunThreadHandle = (HANDLE)::_beginthreadex(NULL, NULL, &SystemRunThreadFn, (void*)0, NULL, &SystemRunThreadID);	
	if ( NULL == SystemRunThreadHandle )
	{	
		this->m_SystemRunThreadState = THREAD_STATE_NONE;
		this->m_ErrorString = _T("Eorror, Create Thread Fault (SystemRunThreadHandle == NULL)");	
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_CREATE);
		return false;
	}			
	SystemRunThreadEvent = JetAPI::CreateEvent(NULL, TRUE, TRUE, _T("Thread System Run"));		
	::SetThreadPriority(SystemRunThreadHandle, THREAD_PRIORITY_BELOW_NORMAL); 	

	SetSystemRunThreadCmd(THREAD_COMMAND_TO_RUN);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteSystemRunThread()//刪除系統運作執行緒
{
	DWORD WaitTime = 1000;//1 sec	
	if ( NULL == SystemRunThreadHandle ) { return true; }		
	this->SetSystemRunThreadCmd(THREAD_COMMAND_TO_EXIT);	
	DWORD Res = ::WaitForSingleObject(SystemRunThreadHandle, WaitTime);
	::CloseHandle(SystemRunThreadHandle); 
	SystemRunThreadHandle = NULL;
	if ( NULL != SystemRunThreadEvent )
	{	
		::CloseHandle(SystemRunThreadEvent); 
		SystemRunThreadEvent=NULL; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetSystemRunThreadState(THREAD_STATE_MODE State)//設定系統運作執行緒狀態
{	
	if ( m_SystemRunThreadState == State ) { return; }
	LockThreadSystemRun();
	m_SystemRunThreadState = State;
	UnlockThreadSystemRun();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE CAOIDataCollect::GetSystemRunThreadState()//取得系統運作執行緒狀態
{
	return m_SystemRunThreadState;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetSystemRunThreadCmd(THREAD_COMMAND_MODE Cmd)//設定系統運作執行緒命令
{	
	if ( m_SystemRunThreadCmd == Cmd ) { return; }
	LockThreadSystemRun();
	m_SystemRunThreadCmd = Cmd;
	UnlockThreadSystemRun();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CAOIDataCollect::GetSystemRunThreadCmd()//取得系統運作執行緒命令
{
	return m_SystemRunThreadCmd;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecSliceFillFn(size_t ThreadIdx)//執行相機圖填滿執行緒
{
#ifndef OFFLINE_VERSION
	CString          str;
	size_t           i=0;	
	size_t           LastIdx=0;
	bool             Reset = false;
	DWORD            SleepTime = 10;//ms	
	CAOISlice       *SlicePtr = NULL;
	SLICE_FILL_STATE SliceState = SLICE_FILL_NONE;	
	const size_t     SliceCount = this->GetSlicePtrCount();
	THREAD_COMMAND_MODE  ThreadCmd = THREAD_COMMAND_TO_NONE;

	Reset = false;
	for ( i=0; i<SliceCount+1; i++ )//避免最後一筆無法進來迴圈, 因此手動+1
	{
		ThreadCmd = GetSliceFillThreadCmd(ThreadIdx);
		if ( GetIsSystemReleased() == true ) { return true; }
		if ( GetIsSystemException() == true ) { return true; }		
		if ( GetIsAllCalcThreadStop() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }

		if ( true == Reset ) //由於上一次計算尚未有圖片, 退回引數號碼1步
		{			
			i = LastIdx;
			Reset = false;
		}
		if ( i >= SliceCount )
		{	break; }

		SlicePtr = this->GetSlicePtr(i, false);
		if ( NULL == SlicePtr ) { continue; }
		SliceState = SlicePtr->GetSliceFillState();
		if ( SLICE_FILL_DONE == SliceState ) { continue; }
		if ( SLICE_FILL_DOING == SliceState ) { continue; }
		if ( SLICE_FILL_CLEAR == SliceState ) { continue; }		

		//同步化
		CAOISlice::LockSlice();
		SliceState = SlicePtr->GetSliceFillState();//重新取得狀態, 因為在鎖住前可能別的執行緒又進去算了
		if ( SLICE_FILL_NONE != SliceState )
		{
			CAOISlice::UnlockSlice();
			continue;
		}
		SlicePtr->SetSliceFillState(SLICE_FILL_DOING);
		CAOISlice::UnlockSlice();
		
		LastIdx = i;		
		if ( SlicePtr->CheckSliceCameaGrabbed() == false )
		{
			Reset = true;			
			CAOISlice::LockSlice();
			SlicePtr->SetSliceFillState(SLICE_FILL_NONE);
			CAOISlice::UnlockSlice();
			if ( SleepTime > 0 ) //等一下讓相機取影像
			{	::Sleep(SleepTime); }
			continue;
		}
		if ( SlicePtr->ExecSliceFill() == false )
		{
			LockThread();
			this->m_ErrorString.Format(_T("Error, Fill Slice%d Fault"), i+1);
			UnlockThread();
			return false;
		}		
#ifdef _DEBUG
		str.Format(_T("Fill Slice[TH:%d]#%d/%d Finished\n"), ThreadIdx+1, i+1, SliceCount);
		TRACE(str);
#endif//_DEBUG
		SlicePtr->SetSliceFillState(SLICE_FILL_DONE);
		InterlockedIncrement(&m_SliceFinishedCnt);		
		SetIsAnyCalcThreadWorking(true);
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateSliceFillThread()//建立相機圖填滿執行緒
{
	size_t  i=0;
	const size_t MaxThreadCount=MAX_THREAD_COUNT_SLICE_FILL;
#ifndef OFFLINE_VERSION		
	CString str;
	size_t ThreadIdx = 0;	
	if ( this->DeleteSliceFillThread() == false ) { return false; }	

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		this->m_SliceFillThreadCmd[ThreadIdx] = THREAD_COMMAND_TO_IDLE;	
	}

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		SliceFillThreadHandle[ThreadIdx] = (HANDLE)::_beginthreadex(NULL, NULL, &SliceFillThreadFn, (void*)ThreadIdx, NULL, &SliceFillThreadID[ThreadIdx]);
		if ( NULL == SliceFillThreadHandle[ThreadIdx] )
		{	
			this->m_SliceFillThreadState[ThreadIdx] = THREAD_STATE_NONE;
			this->m_ErrorString = _T("Eorror, Create Thread Fault (SliceFillThreadHandle == NULL)");		
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_CREATE);
			return false;
		}	
	}

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;	
		str.Format(_T("Slice Fill Event[%d]"), ThreadIdx+1);	
		SliceFillThreadEvent[ThreadIdx] = JetAPI::CreateEvent(NULL, TRUE, TRUE, str);
	}
	
	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		if ( NULL != SliceFillThreadHandle[ThreadIdx] )
		{	
			::SetThreadPriority(SliceFillThreadHandle[ThreadIdx], THREAD_PRIORITY_BELOW_NORMAL); 
			//::SetThreadAffinityMask(SliceFillThreadHandle[ThreadIdx], 0x3C);//111100
		}
	}
#else
	for ( i=0; i<MaxThreadCount; i++ )
	{	
		SliceFillThreadEvent[i] = NULL; 
		SliceFillThreadHandle[i] = NULL; 
	}		
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteSliceFillThread()//刪除相機圖填滿執行緒
{
#ifndef OFFLINE_VERSION	
	size_t i = 0;
	size_t ThreadIdx = 0;
	DWORD WaitTime = 1000;//1 sec
	for ( i=0; i<MAX_THREAD_COUNT_SLICE_FILL; i++ )
	{
		ThreadIdx = i;
		if ( NULL == SliceFillThreadHandle[ThreadIdx] ) { continue; }		
		this->SetSliceFillThreadCmd(ThreadIdx, THREAD_COMMAND_TO_EXIT);	
		DWORD Res = ::WaitForSingleObject(SliceFillThreadHandle[ThreadIdx], WaitTime);
		::CloseHandle(SliceFillThreadHandle[ThreadIdx]); 
		SliceFillThreadHandle[ThreadIdx] = NULL;
		if ( NULL != SliceFillThreadEvent[ThreadIdx] )
		{	
			::CloseHandle(SliceFillThreadEvent[ThreadIdx]); 
			SliceFillThreadEvent[ThreadIdx]=NULL; 
		}	
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartSliceFillThread(bool WaitOn)//開始相機圖填滿執行緒
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	const size_t ThreadCount   = GetSliceFillThreadUsingCount();
	for ( i=0; i<ThreadCount; i++ )
	{		
		ThreadIdx = i;
		if ( NULL == SliceFillThreadHandle[ThreadIdx] ) { return true; }
		if ( NULL != SliceFillThreadEvent[ThreadIdx] )
		{	::ResetEvent(SliceFillThreadEvent[ThreadIdx]); }
		if ( THREAD_STATE_FINISH == m_SliceFillThreadState[ThreadIdx] )
		{  SetSliceFillThreadState(ThreadIdx, THREAD_STATE_IDLE); }
		SetSliceFillThreadCmd(ThreadIdx, THREAD_COMMAND_TO_RUN);
	}
	if ( false == WaitOn ) 
	{	return true; }		
	if ( WaitForSliceFillThreadStart() == false )
	{	return false; }
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForSliceFillThreadIdle()//等待相機圖填滿執行緒停止
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t SliceCount = GetSlicePtrCount();
	const size_t MaxCounts = SliceCount*100+100;	
	const size_t ThreadCount   = GetSliceFillThreadUsingCount();
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			//if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_STATE_NONE == m_SliceFillThreadState[ThreadIdx] )
			{	break; }
			if ( THREAD_STATE_IDLE == m_SliceFillThreadState[ThreadIdx] )
			{	break; }
			if ( SleepTime > 0 )
			{	::Sleep(SleepTime); }
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_SliceFillThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_SliceFillThreadState[ThreadIdx]);
			m_ErrorString.Format(_T("Error, WaitForSliceFillThreadIdle[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE);
			return false;
		}	
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForSliceFillThreadStop()//等待相機圖填滿執行緒停止
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t SliceCount = GetSlicePtrCount();
	const size_t MaxCounts = SliceCount*100+100;	
	const size_t ThreadCount   = GetSliceFillThreadUsingCount();
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			//if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_STATE_RUNNING != m_SliceFillThreadState[ThreadIdx] )
			{	break; }			
			if ( SleepTime > 0 )
			{	::Sleep(SleepTime); }
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_SliceFillThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_SliceFillThreadState[ThreadIdx]);
			m_ErrorString.Format(_T("Error, WaitForSliceFillThreadStop[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText); 
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP);
			return false;
		}	
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForSliceFillThreadStart()//等待相機圖填滿執行緒開始
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	const size_t MaxCount = 100;
	const size_t SleepTime = 10;
	const size_t ThreadCount   = GetSliceFillThreadUsingCount();
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		for ( i=0; i<MaxCount; i++ )
		{
			if ( THREAD_COMMAND_TO_RUN != m_SliceFillThreadCmd[ThreadIdx] )//切入下一個階段
			{	break; }
			if ( THREAD_STATE_IDLE != m_SliceFillThreadState[ThreadIdx] )
			{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			CString CmdText = GetThreadCommandModeText(m_SliceFillThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_SliceFillThreadState[ThreadIdx]);
			m_ErrorString.Format(_T("Error, WaitForSliceFillThreadStart[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);	
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_START);	
			return false;
		}
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForSliceFillThreadFinish()//等待相機圖填滿執行緒結束
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t SliceCount = GetSlicePtrCount();
	const size_t MaxCounts = SliceCount*100+100;	
	const size_t ThreadCount   = GetSliceFillThreadUsingCount();
	SaveDebugMessage(_T("WaitForSliceFillThreadFinish-Start"));
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		if ( SliceFillThreadEvent[ThreadIdx] == NULL ) { continue; }
		size_t CheckWorkingCount=0;
		ResetIsAnyCalcThreadWorking();
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_COMMAND_TO_EXIT == m_SliceFillThreadCmd[ThreadIdx] ) { return true; }		
			if ( THREAD_COMMAND_TO_IDLE == m_SliceFillThreadCmd[ThreadIdx] )
			{	break; }

			Res = ::WaitForSingleObject(SliceFillThreadEvent[ThreadIdx], SleepTime);
			if ( Res != WAIT_TIMEOUT ) 
			{
				if ( THREAD_STATE_RUNNING == m_SliceFillThreadState[ThreadIdx] )
				{
					if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
					continue; 
				}				
				break; 
			}

			if ( GetIsAnyCalcThreadWorking() == true )
			{	
				CheckWorkingCount = 0; 
				ResetIsAnyCalcThreadWorking();
			}
			else
			{
				CheckWorkingCount ++;
				if ( CheckWorkingCount > THREAD_CHECK_NO_WORKING_MAX_COUNT )
				{
					CString CmdText = GetThreadCommandModeText(m_SliceFillThreadCmd[ThreadIdx]);
					CString StateText = GetThreadStateModeText(m_SliceFillThreadState[ThreadIdx]);
					SaveDebugMessage(_T("WaitForSliceFillThreadFinish-Fault (No-Working)"));
					m_ErrorString.Format(_T("Error, WaitForSliceFillThreadFinish[%d, Cmd=%s, State=%s] too long (No-Working)"), ThreadIdx+1, CmdText, StateText);
					SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
					return false;
				}
			}
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_SliceFillThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_SliceFillThreadState[ThreadIdx]);
			SaveDebugMessage(_T("WaitForSliceFillThreadFinish-Fault"));
			m_ErrorString.Format(_T("Error, WaitForSliceFillThreadFinish[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
			return false;
		}	
	}	
	SaveDebugMessage(_T("WaitForSliceFillThreadFinish-End"));
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckSliceFillThreadStateFinish()//確認相機圖填滿執行緒狀態
{
	size_t i=0;	
	for ( i=0; i<MAX_THREAD_COUNT_SLICE_FILL; i++ )
	{	
		if ( NULL == SliceFillThreadHandle[i] ) { continue; }
		if ( THREAD_COMMAND_TO_NONE == m_SliceFillThreadCmd[i] ) { continue; }
		if ( THREAD_COMMAND_TO_EXIT == m_SliceFillThreadCmd[i] ) { continue; }
		if ( THREAD_COMMAND_TO_IDLE == m_SliceFillThreadCmd[i] ) { continue; }		
		if ( m_SliceFillThreadState[i] != THREAD_STATE_FINISH )		
		{
			this->m_ErrorString.Format(_T("Error, m_SliceFillThreadState#%d is Exception"), i+1);
			return false;
		}	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetSliceFillThreadState(size_t idx, THREAD_STATE_MODE State)//設定相機圖填滿執行緒狀態
{
	if ( idx >= MAX_THREAD_COUNT_SLICE_FILL ) { return; } 
	if ( m_SliceFillThreadState[idx] == State ) { return; }
	LockThreadSliceFill();	
	m_SliceFillThreadState[idx] = State;
	UnlockThreadSliceFill();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE CAOIDataCollect::GetSliceFillThreadState(size_t idx)//取得相機圖填滿執行緒狀態
{
	if ( idx >= MAX_THREAD_COUNT_SLICE_FILL ) { return THREAD_STATE_NONE; } 
	return m_SliceFillThreadState[idx];
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetSliceFillThreadCmd(size_t idx, THREAD_COMMAND_MODE Cmd)//設定相機圖填滿執行緒命令
{
	if ( idx >= MAX_THREAD_COUNT_SLICE_FILL ) { return; } 
	if ( m_SliceFillThreadCmd[idx] == Cmd ) { return; }
	LockThreadSliceFill();	
	m_SliceFillThreadCmd[idx] = Cmd;
	UnlockThreadSliceFill();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CAOIDataCollect::GetSliceFillThreadCmd(size_t idx) //取得相機圖填滿執行緒命令
{
	if ( idx >= MAX_THREAD_COUNT_SLICE_FILL ) { return THREAD_COMMAND_TO_NONE; } 
	return m_SliceFillThreadCmd[idx];
}
//-------------------------------------------------------------------------------------//
size_t CAOIDataCollect::GetSliceFillThreadUsingCount() const   //取得相機圖填滿實際使用數量 
{
	return m_SystemParameter.m_MTCount_SliceFill;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecFrameMergeFn(size_t ThreadIdx)//執行影像合併執行緒
{
#ifndef OFFLINE_VERSION
	CString           str;
	size_t            i=0;	
	size_t            LastIdx=0;	
	bool              Reset = false;
	double            Time = 0;
	DWORD             SleepTime = 10;//ms			
	CAOIFrame        *FramePtr = NULL;		
	LARGE_INTEGER     nStartTime;
	LARGE_INTEGER     nEndTime;
	bool              AdjustCudaThread=false;//調整Cuad執行緒
	TASK_MODE         TaskMode = GetTaskMode();
	FRAME_MERGE_STATE FrameState = FRAME_MERGE_NONE;
	const size_t      FrameCount = GetFramePtrCount();
	THREAD_COMMAND_MODE  ThreadCmd = THREAD_COMMAND_TO_NONE;
	
#ifdef CUDA_USE		
	AdjustCudaThread = true;
	if ( FN_DISABLE == GetSystemParameter().m_CudaFnEnabled )
	{	AdjustCudaThread = false;	}
	//if ( CudaFunc.GetIsCudaDeviceUse()==false )
	//{	AdjustCudaThread = false;	}
#else
	AdjustCudaThread = false;
#endif//CUDA_USE
	//AdjustCudaThread = false;

	Reset = false;	
	int  nDebug=0;
	bool bDebug=false;	
	if ( 0 != ThreadIdx )
	{	bDebug = true; }
	for ( i=0; i<FrameCount+1; i++ )//避免最後一筆無法進來迴圈, 因此手動+1
	{	
		ThreadCmd = GetFrameMergeThreadCmd(ThreadIdx);
		if ( GetIsSystemReleased() == true ) { return true; }
		if ( GetIsSystemException() == true ) { return true; }
		if ( GetIsAllCalcThreadStop() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }

		if ( true == Reset ) //由於上一次計算尚未有圖片, 退回引數號碼1步
		{			
			i = LastIdx;
			Reset = false;
		}

		if ( i>=FrameCount )
		{	break; }
		FramePtr = this->GetFramePtr(i, false);
		if ( NULL == FramePtr ) { continue; }
		FrameState = FramePtr->GetFrameMergeState();
		if ( FRAME_MERGE_DONE == FrameState ) { continue; }
		if ( FRAME_MERGE_DOING == FrameState ) { continue; }		
		if ( FRAME_MERGE_CLEAR == FrameState ) { continue; }
	
		//讓Cuda專注於第1個執行緒上作業
		if ( true == AdjustCudaThread )
		{
			if ( FRAME_SPACE == FramePtr->GetFrameType())			
			{
				if ( 0 != ThreadIdx )
				{	continue; }
			}
		}		


		//同步化
		CAOIFrame::LockFrame();
		FrameState = FramePtr->GetFrameMergeState();//重新取得狀態, 因為在鎖住前可能別的執行緒又進去算了
		if ( FRAME_MERGE_NONE != FrameState )
		{
			CAOIFrame::UnlockFrame();
			continue;
		}
		FramePtr->SetFrameMergeState(FRAME_MERGE_DOING);
		CAOIFrame::UnlockFrame();	
		LastIdx = i;		
		if ( FramePtr->CheckFrameFieldFilled() == false )
		{
			Reset = true;			
			CAOIFrame::LockFrame();
			FramePtr->SetFrameMergeState(FRAME_MERGE_NONE);
			CAOIFrame::UnlockFrame();
			if ( SleepTime > 0 ) //等一下讓相機取影像
			{	::Sleep(SleepTime); }
			continue;
		}		
		QueryPerformanceCounter(&nStartTime);
		if ( true == bDebug )
		{	nDebug ++;	}
		if ( FramePtr->ExecFrameMerge() == false )
		{	
			LockThread();
			this->m_ErrorString.Format(_T("Error, Merge Frame#%d Fault"), i+1);
			UnlockThread();
			return false;
		}
	#ifdef _DEBUG
		str.Format(_T("Merge Frame[TH:%d]#%d/%d Finished\n"), ThreadIdx+1, i+1, FrameCount);
		TRACE(str);
	#endif//_DEBUG		
		FramePtr->SetFrameMergeState(FRAME_MERGE_DONE);		
		QueryPerformanceCounter(&nEndTime);
		Time = (nEndTime.QuadPart - nStartTime.QuadPart)*1000.0/m_SystemFreq.QuadPart;
		FramePtr->SetFrameMergeTime(Time);
		InterlockedIncrement(&m_FrameFinishedCnt);
		SetIsAnyCalcThreadWorking(true);
		/*
		CAOIField *FieldPtr = FramePtr->GetFrameFieldPtr();		
		if ( NULL != FieldPtr ) 
		{			
			if ( FieldPtr->CheckFieldFramesMergeFinish() == true )
			{
				CAOIField::LockField();
				FieldPtr->SetFieldMergeState(FIELD_MERGE_DONE);	
				CAOIField::UnlockField();
			}
		}*/
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIDataCollect::CreateFrameMergeThread()//建立影像合併執行緒
{
	size_t  i=0;
	const size_t MaxThreadCount=MAX_THREAD_COUNT_FRAME_MERGE;
#ifndef OFFLINE_VERSION	
	CString str;
	size_t ThreadIdx = 0;	
	if ( this->DeleteFrameMergeThread() == false ) { return false; }	

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		this->m_FrameMergeThreadCmd[ThreadIdx] = THREAD_COMMAND_TO_IDLE;	
	}

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		FrameMergeThreadHandle[ThreadIdx] = (HANDLE)::_beginthreadex(NULL, NULL, &FrameMergeThreadFn, (void*)ThreadIdx, NULL, &FrameMergeThreadID[ThreadIdx]);
		if ( NULL == FrameMergeThreadHandle[ThreadIdx] )
		{	
			this->m_FrameMergeThreadState[ThreadIdx] = THREAD_STATE_NONE;
			this->m_ErrorString = _T("Eorror, Create Thread Fault (FrameMergeThreadHandle == NULL)");		
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_CREATE);
			return false;
		}	
	}

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;	
		str.Format(_T("Frame Merge Event[%d]"), ThreadIdx+1);	
		FrameMergeThreadEvent[ThreadIdx] = JetAPI::CreateEvent(NULL, TRUE, TRUE, str);
	}

	DWORD_PTR AffinityMask = GetThreadAffinityMask();
	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		if ( NULL != FrameMergeThreadHandle[ThreadIdx] )
		{	
			::SetThreadPriority(FrameMergeThreadHandle[ThreadIdx], THREAD_PRIORITY_BELOW_NORMAL); 
			::SetThreadAffinityMask(FrameMergeThreadHandle[ThreadIdx], AffinityMask);//111100
		}
	}	
#else
	for ( i=0; i<MaxThreadCount; i++ )
	{
		FrameMergeThreadEvent[i] = NULL;
		FrameMergeThreadHandle[i] = NULL;		
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIDataCollect::DeleteFrameMergeThread()//刪除影像合併執行緒
{
#ifndef OFFLINE_VERSION	
	size_t i = 0;
	size_t ThreadIdx = 0;
	DWORD WaitTime = 1000;//1 sec
	for ( i=0; i<MAX_THREAD_COUNT_FRAME_MERGE; i++ )
	{
		ThreadIdx = i;
		if ( NULL == FrameMergeThreadHandle[ThreadIdx] ) { continue; }		
		this->SetFrameMergeThreadCmd(ThreadIdx, THREAD_COMMAND_TO_EXIT);	
		DWORD Res = ::WaitForSingleObject(FrameMergeThreadHandle[ThreadIdx], WaitTime);
		::CloseHandle(FrameMergeThreadHandle[ThreadIdx]); 
		FrameMergeThreadHandle[ThreadIdx] = NULL;
		if ( NULL != FrameMergeThreadEvent[ThreadIdx] )
		{	
			::CloseHandle(FrameMergeThreadEvent[ThreadIdx]); 
			FrameMergeThreadEvent[ThreadIdx]=NULL; 
		}
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIDataCollect::StartFrameMergeThread(bool WaitOn) //開始影像合併執行緒
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	const size_t ThreadCount = GetFrameMergeThreadUsingCount();	
	for ( i=0; i<ThreadCount; i++ )
	{		
		ThreadIdx = i;
		if ( NULL == FrameMergeThreadHandle[ThreadIdx] ) { return true; }
		if ( NULL != FrameMergeThreadEvent[ThreadIdx] )
		{	::ResetEvent(FrameMergeThreadEvent[ThreadIdx]); }
		if ( THREAD_STATE_FINISH == m_FrameMergeThreadState[ThreadIdx] )
		{  SetFrameMergeThreadState(ThreadIdx, THREAD_STATE_IDLE); }
		this->SetFrameMergeThreadCmd(ThreadIdx, THREAD_COMMAND_TO_RUN);
	}
	if ( false == WaitOn ) 
	{	return true; }
	if ( WaitForFrameMergeThreadStart() == false ) 
	{	return false; }				
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIDataCollect::WaitForFrameMergeThreadIdle()//等待影像合併執行緒停止
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t FrameCount = GetFramePtrCount();
	const size_t MaxCounts = FrameCount*100+100;	
	const size_t ThreadCount = GetFrameMergeThreadUsingCount();	
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;		
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			//if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_STATE_NONE == m_FrameMergeThreadState[ThreadIdx] )
			{	break; }
			if ( THREAD_STATE_IDLE == m_FrameMergeThreadState[ThreadIdx] )
			{	break; }
			if ( SleepTime > 0 )
			{	::Sleep(SleepTime); }					
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_FrameMergeThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_FrameMergeThreadState[ThreadIdx]);			
			m_ErrorString.Format(_T("Error, WaitForFrameMergeThreadIdle[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE);
			return false;
		}	
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIDataCollect::WaitForFrameMergeThreadStop()//等待影像合併執行緒停止
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t FrameCount = GetFramePtrCount();
	const size_t MaxCounts = FrameCount*100+100;	
	const size_t ThreadCount = GetFrameMergeThreadUsingCount();	
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;		
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			//if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_STATE_RUNNING != m_FrameMergeThreadState[ThreadIdx] )
			{	break; }			
			if ( SleepTime > 0 )
			{	::Sleep(SleepTime); }					
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_FrameMergeThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_FrameMergeThreadState[ThreadIdx]);			
			m_ErrorString.Format(_T("Error, WaitForFrameMergeThreadStop[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP);
			return false;
		}	
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIDataCollect::WaitForFrameMergeThreadStart()//等待影像合併執行緒開始
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	const size_t MaxCount = 100;
	const size_t SleepTime = 10;
	const size_t ThreadCount = GetFrameMergeThreadUsingCount();		

	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		for ( i=0; i<MaxCount; i++ )
		{
			if ( THREAD_COMMAND_TO_RUN != m_FrameMergeThreadCmd[ThreadIdx] )//切入下一個階段
			{	break; }
			if ( THREAD_STATE_IDLE != m_FrameMergeThreadState[ThreadIdx] )
			{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			CString CmdText = GetThreadCommandModeText(m_FrameMergeThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_FrameMergeThreadState[ThreadIdx]);			
			m_ErrorString.Format(_T("Error, WaitForFrameMergeThreadStart[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);			
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_START);	
			return false;
		}
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIDataCollect::WaitForFrameMergeThreadFinish()//等待影像合併執行緒結束
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t FrameCount = CAOIDataCollect::GetFramePtrCount();
	const size_t MaxCounts = FrameCount*100+100;	
	const size_t ThreadCount = GetFrameMergeThreadUsingCount();	
	SaveDebugMessage(_T("WaitForFrameMergeThreadFinish-Start"));
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		if ( FrameMergeThreadEvent[ThreadIdx] == NULL ) { continue; }
		size_t CheckWorkingCount=0;
		ResetIsAnyCalcThreadWorking();
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_COMMAND_TO_EXIT == m_FrameMergeThreadCmd[ThreadIdx] ) { return true; }		
			if ( THREAD_COMMAND_TO_IDLE == m_FrameMergeThreadCmd[ThreadIdx] )
			{	break; }

			Res = ::WaitForSingleObject(FrameMergeThreadEvent[ThreadIdx], SleepTime);
			if ( Res != WAIT_TIMEOUT ) 
			{
				if ( THREAD_STATE_RUNNING == m_FrameMergeThreadState[ThreadIdx] )
				{
					if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
					continue; 
				}				
				break; 
			}

			if ( GetIsAnyCalcThreadWorking() == true )
			{	
				CheckWorkingCount = 0; 
				ResetIsAnyCalcThreadWorking();
			}
			else
			{
				CheckWorkingCount ++;
				if ( CheckWorkingCount > THREAD_CHECK_NO_WORKING_MAX_COUNT )
				{
					CString CmdText = GetThreadCommandModeText(m_FrameMergeThreadCmd[ThreadIdx]);
					CString StateText = GetThreadStateModeText(m_FrameMergeThreadState[ThreadIdx]);			
					SaveDebugMessage(_T("WaitForFrameMergeThreadFinish-Fault (No-Working)"));
					m_ErrorString.Format(_T("Error, WaitForFrameMergeThreadFinish[%d, Cmd=%s, State=%s] too long (No-Working)"), ThreadIdx+1, CmdText, StateText);
					SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
					return false;
				}
			}	
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_FrameMergeThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_FrameMergeThreadState[ThreadIdx]);			
			SaveDebugMessage(_T("WaitForFrameMergeThreadFinish-Fault"));
			m_ErrorString.Format(_T("Error, WaitForFrameMergeThreadFinish[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
			return false;
		}	
	}	
	SaveDebugMessage(_T("WaitForFrameMergeThreadFinish-End"));
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CAOIDataCollect::CheckFrameMergeThreadStateFinish()//確認影像合併執行緒狀態
{
	size_t i=0;
	for ( i=0; i<MAX_THREAD_COUNT_FRAME_MERGE; i++ )
	{
		if ( NULL == FrameMergeThreadHandle[i] ) { continue; }
		if ( THREAD_COMMAND_TO_NONE == m_FrameMergeThreadCmd[i] ) { continue; }
		if ( THREAD_COMMAND_TO_EXIT == m_FrameMergeThreadCmd[i] ) { continue; }
		if ( THREAD_COMMAND_TO_IDLE == m_FrameMergeThreadCmd[i] ) { continue; }
		if ( m_FrameMergeThreadState[i] != THREAD_STATE_FINISH )
		{
			this->m_ErrorString.Format(_T("Error, m_FrameMergeThreadState#%d is Exception"), i+1);
			return false;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//	
void CAOIDataCollect::SetFrameMergeThreadState(size_t idx, THREAD_STATE_MODE State)//設定影像合併執行緒狀態
{
	if ( idx >= MAX_THREAD_COUNT_FRAME_MERGE ) { return; }
	if ( m_FrameMergeThreadState[idx] == State ) { return; }
	LockThreadFrameMerge();
	m_FrameMergeThreadState[idx] = State;
	UnlockThreadFrameMerge();
}
//-------------------------------------------------------------------------------------//	
THREAD_STATE_MODE CAOIDataCollect::GetFrameMergeThreadState(size_t idx) //取得影像合併執行緒狀態
{
	if ( idx >= MAX_THREAD_COUNT_FRAME_MERGE ) { return THREAD_STATE_NONE; }
	return m_FrameMergeThreadState[idx];
}
//-------------------------------------------------------------------------------------//	
void CAOIDataCollect::SetFrameMergeThreadCmd(size_t idx, THREAD_COMMAND_MODE Cmd)//設定影像合併執行緒命令
{
	if ( idx >= MAX_THREAD_COUNT_FRAME_MERGE ) { return; }
	if ( m_FrameMergeThreadCmd[idx] == Cmd ) { return; }
	LockThreadFrameMerge();
	m_FrameMergeThreadCmd[idx] = Cmd;
	UnlockThreadFrameMerge();
}
//-------------------------------------------------------------------------------------//	
THREAD_COMMAND_MODE CAOIDataCollect::GetFrameMergeThreadCmd(size_t idx) //取得影像合併執行緒命令
{
	if ( idx >= MAX_THREAD_COUNT_FRAME_MERGE ) { return THREAD_COMMAND_TO_NONE; }
	return m_FrameMergeThreadCmd[idx];
}
//-------------------------------------------------------------------------------------//
size_t CAOIDataCollect::GetFrameMergeThreadUsingCount() const//取得影像合併實際使用數量 
{
	size_t ThreadCount = 0;
	THREAD_GRAB_MODE GrabMode = GetThreadGrabMode();	
	if ( false==m_SystemParameter.m_CudaFnEnabled || THREAD_GRAB_PROJECT_MAP==GrabMode )
	{	ThreadCount   = m_SystemParameter.m_MTCount_FrameMerge; }
	else
	{	ThreadCount   = 1; }


	ThreadCount   = m_SystemParameter.m_MTCount_FrameMerge;
	return ThreadCount;
}
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecFieldMergeFn(size_t ThreadIdx)//執行區域合併執行緒
{
#ifndef OFFLINE_VERSION
	CString           str;
	size_t            i=0;	
	size_t            LastIdx=0;	
	bool              Reset = false;
	double            Time = 0;
	DWORD             SleepTime = 10;//ms			
	CAOIField        *FieldPtr = NULL;		
	LARGE_INTEGER     nStartTime;
	LARGE_INTEGER     nEndTime;
	TASK_MODE         TaskMode = GetTaskMode();
	FIELD_MERGE_STATE FieldState = FIELD_MERGE_NONE;
	const size_t      FieldCount = GetFieldPtrCount();
	THREAD_COMMAND_MODE  ThreadCmd = THREAD_COMMAND_TO_NONE;
	
	Reset = false;	
	for ( i=0; i<FieldCount+1; i++ )//避免最後一筆無法進來迴圈, 因此手動+1
	{	
		ThreadCmd = GetFieldMergeThreadCmd(ThreadIdx);
		if ( GetIsSystemReleased() == true ) { return true; }
		if ( GetIsSystemException() == true ) { return true; }
		if ( GetIsAllCalcThreadStop() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }

		if ( true == Reset ) //由於上一次計算尚未有圖片, 退回引數號碼1步
		{			
			i = LastIdx;
			Reset = false;
		}

		if ( i>=FieldCount )
		{	break; }
		FieldPtr = this->GetFieldPtr(i, false);
		if ( NULL == FieldPtr ) { continue; }
		FieldState = FieldPtr->GetFieldMergeState();
		if ( FIELD_MERGE_DONE == FieldState ) { continue; }
		if ( FIELD_MERGE_DOING == FieldState ) { continue; }		
		if ( FIELD_MERGE_CLEAR == FieldState ) { continue; }
	
		//同步化
		CAOIField::LockField();
		FieldState = FieldPtr->GetFieldMergeState();//重新取得狀態, 因為在鎖住前可能別的執行緒又進去算了
		if ( FIELD_MERGE_NONE != FieldState )
		{
			CAOIField::UnlockField();
			continue;
		}
		FieldPtr->SetFieldMergeState(FIELD_MERGE_DOING);
		CAOIField::UnlockField();	
		LastIdx = i;		
		if ( FieldPtr->CheckFieldFramesMergeFinish() == false )
		{
			Reset = true;			
			CAOIField::LockField();
			FieldPtr->SetFieldMergeState(FIELD_MERGE_NONE);
			CAOIField::UnlockField();
			if ( SleepTime > 0 ) //等一下讓相機取影像
			{	::Sleep(SleepTime); }
			continue;
		}		
		QueryPerformanceCounter(&nStartTime);
		if ( FieldPtr->MergeFieldFrames() == false )
		{	
			LockThread();
			this->m_ErrorString.Format(_T("Error, Merge Field#%d Frames Fault"), i+1);
			UnlockThread();
			return false;
		}
	#ifdef _DEBUG
		str.Format(_T("Merge Field[TH:%d]#%d/%d Finished\n"), ThreadIdx+1, i+1, FieldCount);
		TRACE(str);
	#endif//_DEBUG		
		FieldPtr->SetFieldMergeState(FIELD_MERGE_DONE);		
		QueryPerformanceCounter(&nEndTime);
		Time = (nEndTime.QuadPart - nStartTime.QuadPart)*1000.0/m_SystemFreq.QuadPart;
		FieldPtr->SetFieldMergeTime(Time);
		InterlockedIncrement(&m_FieldMergeFinishedCnt);	
		SetIsAnyCalcThreadWorking(true);
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateFieldMergeThread()//建立區域合併執行緒
{
	size_t  i=0;
	const size_t MaxThreadCount=MAX_THREAD_COUNT_FIELD_MERGE;
#ifndef OFFLINE_VERSION	
	CString str;
	size_t ThreadIdx = 0;	
	if ( this->DeleteFieldMergeThread() == false ) { return false; }	

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		this->m_FieldMergeThreadCmd[ThreadIdx] = THREAD_COMMAND_TO_IDLE;	
	}

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		FieldMergeThreadHandle[ThreadIdx] = (HANDLE)::_beginthreadex(NULL, NULL, &FieldMergeThreadFn, (void*)ThreadIdx, NULL, &FieldMergeThreadID[ThreadIdx]);
		if ( NULL == FieldMergeThreadHandle[ThreadIdx] )
		{	
			this->m_FieldMergeThreadState[ThreadIdx] = THREAD_STATE_NONE;
			this->m_ErrorString = _T("Eorror, Create Thread Fault (FieldMergeThreadHandle == NULL)");		
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_CREATE);
			return false;
		}	
	}

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;	
		str.Format(_T("Field Merge Event[%d]"), ThreadIdx+1);	
		FieldMergeThreadEvent[ThreadIdx] = JetAPI::CreateEvent(NULL, TRUE, TRUE, str);
	}

	DWORD_PTR AffinityMask = GetThreadAffinityMask();
	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		if ( NULL != FieldMergeThreadHandle[ThreadIdx] )
		{	
			::SetThreadPriority(FieldMergeThreadHandle[ThreadIdx], THREAD_PRIORITY_BELOW_NORMAL); 
			::SetThreadAffinityMask(FieldMergeThreadHandle[ThreadIdx], AffinityMask);//111100
		}
	}	
#else
	for ( i=0; i<MaxThreadCount; i++ )
	{
		FieldMergeThreadEvent[i] = NULL;
		FieldMergeThreadHandle[i] = NULL;		
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteFieldMergeThread()//刪除區域合併執行緒
{
#ifndef OFFLINE_VERSION	
	size_t i = 0;
	size_t ThreadIdx = 0;
	DWORD WaitTime = 1000;//1 sec
	for ( i=0; i<MAX_THREAD_COUNT_FIELD_MERGE; i++ )
	{
		ThreadIdx = i;
		if ( NULL == FieldMergeThreadHandle[ThreadIdx] ) { continue; }		
		this->SetFieldMergeThreadCmd(ThreadIdx, THREAD_COMMAND_TO_EXIT);	
		DWORD Res = ::WaitForSingleObject(FieldMergeThreadHandle[ThreadIdx], WaitTime);
		::CloseHandle(FieldMergeThreadHandle[ThreadIdx]); 
		FieldMergeThreadHandle[ThreadIdx] = NULL;
		if ( NULL != FieldMergeThreadEvent[ThreadIdx] )
		{	
			::CloseHandle(FieldMergeThreadEvent[ThreadIdx]); 
			FieldMergeThreadEvent[ThreadIdx]=NULL; 
		}
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartFieldMergeThread(bool WaitOn)//開始區域合併執行緒	
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	const size_t ThreadCount = GetFieldMergeThreadUsingCount();	
	for ( i=0; i<ThreadCount; i++ )
	{		
		ThreadIdx = i;
		if ( NULL == FieldMergeThreadHandle[ThreadIdx] ) { return true; }
		if ( NULL != FieldMergeThreadEvent[ThreadIdx] )
		{	::ResetEvent(FieldMergeThreadEvent[ThreadIdx]); }
		if ( THREAD_STATE_FINISH == m_FieldMergeThreadState[ThreadIdx] )
		{  SetFieldMergeThreadState(ThreadIdx, THREAD_STATE_IDLE); }
		this->SetFieldMergeThreadCmd(ThreadIdx, THREAD_COMMAND_TO_RUN);
	}
	if ( false == WaitOn ) 
	{	return true; }
	if ( WaitForFieldMergeThreadStart() == false ) 
	{	return false; }				
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForFieldMergeThreadIdle()//等待區域合併執行緒閒置
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t FieldCount = GetFieldPtrCount();
	const size_t MaxCounts = FieldCount*100+100;	
	const size_t ThreadCount = GetFieldMergeThreadUsingCount();	
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;		
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			//if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_STATE_NONE == m_FieldMergeThreadState[ThreadIdx] )
			{	break; }
			if ( THREAD_STATE_IDLE == m_FieldMergeThreadState[ThreadIdx] )
			{	break; }
			if ( SleepTime > 0 )
			{	::Sleep(SleepTime); }					
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_FieldMergeThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_FieldMergeThreadState[ThreadIdx]);	
			m_ErrorString.Format(_T("Error, WaitForFieldMergeThreadIdle[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE);
			return false;
		}	
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForFieldMergeThreadStop()//等待區域合併執行緒停止
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t FieldCount = GetFieldPtrCount();
	const size_t MaxCounts = FieldCount*100+100;	
	const size_t ThreadCount = GetFieldMergeThreadUsingCount();	
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;		
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			//if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_STATE_RUNNING != m_FieldMergeThreadState[ThreadIdx] )
			{	break; }			
			if ( SleepTime > 0 )
			{	::Sleep(SleepTime); }					
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_FieldMergeThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_FieldMergeThreadState[ThreadIdx]);	
			m_ErrorString.Format(_T("Error, WaitForFieldMergeThreadStop[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);	
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP);
			return false;
		}	
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForFieldMergeThreadStart()//等待區域合併執行緒開始
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	const size_t MaxCount = 100;
	const size_t SleepTime = 10;
	const size_t ThreadCount = GetFieldMergeThreadUsingCount();		

	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		for ( i=0; i<MaxCount; i++ )
		{
			if ( THREAD_COMMAND_TO_RUN != m_FieldMergeThreadCmd[ThreadIdx] )//切入下一個階段
			{	break; }
			if ( THREAD_STATE_IDLE != m_FieldMergeThreadState[ThreadIdx] )
			{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			CString CmdText = GetThreadCommandModeText(m_FieldMergeThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_FieldMergeThreadState[ThreadIdx]);	
			m_ErrorString.Format(_T("Error, WaitForFieldMergeThreadStart[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText); 
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_START);	
			return false;
		}
	}
#endif//OFFLINE_VERSION
	return true;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForFieldMergeThreadFinish()//等待區域合併執行緒結束
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t FieldCount = CAOIDataCollect::GetFieldPtrCount();
	const size_t MaxCounts = FieldCount*100+100;	
	const size_t ThreadCount = GetFieldMergeThreadUsingCount();	
	SaveDebugMessage(_T("WaitForFieldMergeThreadFinish-Start"));
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		if ( FieldMergeThreadEvent[ThreadIdx] == NULL ) { continue; }
		size_t CheckWorkingCount=0;
		ResetIsAnyCalcThreadWorking();
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_COMMAND_TO_EXIT == m_FieldMergeThreadCmd[ThreadIdx] ) { return true; }		
			if ( THREAD_COMMAND_TO_IDLE == m_FieldMergeThreadCmd[ThreadIdx] )
			{	break; }

			Res = ::WaitForSingleObject(FieldMergeThreadEvent[ThreadIdx], SleepTime);
			if ( Res != WAIT_TIMEOUT ) 
			{
				if ( THREAD_STATE_RUNNING == m_FieldMergeThreadState[ThreadIdx] )
				{
					if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
					continue; 
				}				
				break; 
			}

			if ( GetIsAnyCalcThreadWorking() == true )
			{	
				CheckWorkingCount = 0; 
				ResetIsAnyCalcThreadWorking();
			}
			else
			{
				CheckWorkingCount ++;
				if ( CheckWorkingCount > THREAD_CHECK_NO_WORKING_MAX_COUNT )
				{
					CString CmdText = GetThreadCommandModeText(m_FieldMergeThreadCmd[ThreadIdx]);
					CString StateText = GetThreadStateModeText(m_FieldMergeThreadState[ThreadIdx]);	
					SaveDebugMessage(_T("WaitForFieldMergeThreadFinish-Fault (No-Working)"));
					m_ErrorString.Format(_T("Error, WaitForFieldMergeThreadFinish[%d, Cmd=%s, State=%s] too long (No-Working)"), ThreadIdx+1, CmdText, StateText);
					SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
					return false;
				}
			}
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_FieldMergeThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_FieldMergeThreadState[ThreadIdx]);	
			SaveDebugMessage(_T("WaitForFieldMergeThreadFinish-Fault"));
			m_ErrorString.Format(_T("Error, WaitForFieldMergeThreadFinish[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
			return false;
		}	
	}	
	SaveDebugMessage(_T("WaitForFieldMergeThreadFinish-End"));
#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckFieldMergeThreadStateFinish()//確認區域合併執行緒狀態
{
	size_t i=0;
	for ( i=0; i<MAX_THREAD_COUNT_FIELD_MERGE; i++ )
	{
		if ( NULL == FieldMergeThreadHandle[i] ) { continue; }
		if ( THREAD_COMMAND_TO_NONE == m_FieldMergeThreadCmd[i] ) { continue; }
		if ( THREAD_COMMAND_TO_EXIT == m_FieldMergeThreadCmd[i] ) { continue; }
		if ( THREAD_COMMAND_TO_IDLE == m_FieldMergeThreadCmd[i] ) { continue; }
		if ( m_FieldMergeThreadState[i] != THREAD_STATE_FINISH )
		{
			this->m_ErrorString.Format(_T("Error, m_FieldMergeThreadState#%d is Exception"), i+1);
			return false;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetFieldMergeThreadState(size_t idx, THREAD_STATE_MODE State)//設定區域合併執行緒狀態
{
	if ( idx >= MAX_THREAD_COUNT_FIELD_MERGE ) { return; }
	if ( m_FieldMergeThreadState[idx] == State ) { return; }
	LockThreadFieldMerge();
	m_FieldMergeThreadState[idx] = State;
	UnlockThreadFieldMerge();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE CAOIDataCollect::GetFieldMergeThreadState(size_t idx)//取得區域合併執行緒狀態
{
	if ( idx >= MAX_THREAD_COUNT_FIELD_MERGE ) { return THREAD_STATE_NONE; }
	return m_FieldMergeThreadState[idx];
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetFieldMergeThreadCmd(size_t idx, THREAD_COMMAND_MODE Cmd)//設定區域合併執行緒命令
{
	if ( idx >= MAX_THREAD_COUNT_FIELD_MERGE ) { return; }
	if ( m_FieldMergeThreadCmd[idx] == Cmd ) { return; }
	LockThreadFieldMerge();
	m_FieldMergeThreadCmd[idx] = Cmd;
	UnlockThreadFieldMerge();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CAOIDataCollect::GetFieldMergeThreadCmd(size_t idx)//取得區域合併執行緒命令
{
	if ( idx >= MAX_THREAD_COUNT_FIELD_MERGE ) { return THREAD_COMMAND_TO_NONE; }
	return m_FieldMergeThreadCmd[idx];
}
//-------------------------------------------------------------------------------------//
size_t CAOIDataCollect::GetFieldMergeThreadUsingCount() const//取得區域合併實際使用數量 
{
	size_t ThreadCount = 0;
	THREAD_GRAB_MODE GrabMode = GetThreadGrabMode();	
	if ( false==m_SystemParameter.m_CudaFnEnabled || THREAD_GRAB_PROJECT_MAP==GrabMode )
	{	ThreadCount   = m_SystemParameter.m_MTCount_FieldMerge; }
	else
	{	ThreadCount   = 1; }
	ThreadCount   = m_SystemParameter.m_MTCount_FieldMerge;
	return ThreadCount;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecFrameLoadFn(size_t ThreadIdx)//執行影像載入執行緒
{
//#ifndef OFFLINE_VERSION
	CString           str;
	size_t            i=0, j=0;	
	size_t            LastIdx=0;		
	size_t            RgnUnCalcCount=0;
	size_t            FieldRgnUnCalcCount=0;
	bool              Reset = false;	
	bool             bLoadFrame=false;
	bool             bCheckFrameUsedCount=false;
	double            Time = 0;
	TPOINT3D          StagePos;
	DWORD             SleepTime = 10;//ms
	CAOIRgn          *RgnPtr = NULL;
	CAOIField        *FieldPtr = NULL;
	CAOIFrame        *FramePtr = NULL;	
	int               FrameLoadCount=0;	
	LARGE_INTEGER     nStartTime;
	LARGE_INTEGER     nEndTime;	
	FRAME_MERGE_STATE FrameState = FRAME_MERGE_NONE;
	const size_t      FrameCount = GetFramePtrCount();
	THREAD_COMMAND_MODE  ThreadCmd = THREAD_COMMAND_TO_NONE;
	const bool           AutoReleaseFieldFrame = GetAutoReleaseOfflineFieldFrame();
	
	if ( false == AutoReleaseFieldFrame ) 
	{	bCheckFrameUsedCount = false;	}
	else
	{	bCheckFrameUsedCount = true;	}	
	Reset = false;
	bCheckFrameUsedCount = false;//v1.01.04.034
	for ( i=0; i<FrameCount+1; i++ )//避免最後一筆無法進來迴圈, 因此手動+1
	{	
		ThreadCmd = GetFrameLoadThreadCmd(ThreadIdx);
		if ( GetIsSystemReleased() == true ) { return true; }				
		if ( GetIsSystemException() == true ) { return true; }
		if ( GetIsAllCalcThreadStop() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }

		if ( true==bCheckFrameUsedCount )
		{
			RgnUnCalcCount=0;			
			CAOIFrame::LockFrame();
			if ( true == m_FrameLoadThreadLocked ) 
			{	
				i = LastIdx;
				::Sleep(0);
				CAOIFrame::UnlockFrame();
				continue; 
			}
			m_FrameLoadThreadLocked = true;			
			for ( j=0; j<FrameCount; j++ )			
			{
				FramePtr = GetFramePtr(j, false);
				if ( NULL == FramePtr ) { continue; }				
				FrameState = FramePtr->GetFrameMergeState();
				if ( FRAME_MERGE_DONE != FrameState ) { continue; }
				FieldPtr = FramePtr->GetFrameFieldPtr();
				if ( NULL != FieldPtr ) 
				{
					if ( FieldPtr->CheckFieldFramesMergeFinish() == true ) 
					{
						FieldRgnUnCalcCount = FieldPtr->CalcFieldRgnUnCalculatedCount();
						RgnUnCalcCount += FieldRgnUnCalcCount;						
					}
				}
			}			
			m_FrameLoadThreadLocked = false;	
			CAOIFrame::UnlockFrame();
			if ( RgnUnCalcCount > 0 ) //還有尚未計算的區域
			{
				i = LastIdx;
				::Sleep(0);
				continue; 
			}			
		}		
		if ( true == Reset ) //由於上一次計算尚未有圖片, 退回引數號碼1步
		{			
			i = LastIdx;
			Reset = false;
		}

		if ( i>=FrameCount )
		{	break; }
		FramePtr = this->GetFramePtr(i, false);
		if ( NULL == FramePtr ) { continue; }
		FrameState = FramePtr->GetFrameMergeState();
		if ( FRAME_MERGE_DONE == FrameState ) { continue; }
		if ( FRAME_MERGE_DOING == FrameState ) { continue; }		
		if ( FRAME_MERGE_CLEAR == FrameState ) { continue; }
	
		//同步化
		CAOIFrame::LockFrame();
		FrameState = FramePtr->GetFrameMergeState();//重新取得狀態, 因為在鎖住前可能別的執行緒又進去算了
		if ( FRAME_MERGE_NONE != FrameState )
		{
			CAOIFrame::UnlockFrame();
			continue;
		}
		FramePtr->SetFrameMergeState(FRAME_MERGE_DOING);
		CAOIFrame::UnlockFrame();	
		LastIdx = i;
		QueryPerformanceCounter(&nStartTime);
		bLoadFrame = FramePtr->ExecFrameLoad();
		bLoadFrame = true;//可能沒圖, 但還是要過
		if ( false == bLoadFrame )
		{	
			LockThread();
			this->m_ErrorString.Format(_T("Error, Load Frame#%d Fault"), i+1);
			UnlockThread();
			return false;
		}
		
	#ifdef _DEBUG		
		str.Format(_T("Load Frame[TH:%d]#%d/%d Finished\n"), ThreadIdx+1, i+1, FrameCount);
		TRACE(str);
	#endif//_DEBUG		

		FieldPtr = FramePtr->GetFrameFieldPtr();
		if ( NULL != FieldPtr ) 
		{
			StagePos.x = FieldPtr->GetFieldStagePosX();
			StagePos.y = FieldPtr->GetFieldStagePosY();
			MotionCtrlPtr->XYMoveTo(StagePos.x, StagePos.y, true);
		}

		FramePtr->SetFrameMergeState(FRAME_MERGE_DONE);		
		QueryPerformanceCounter(&nEndTime);
		Time = (nEndTime.QuadPart - nStartTime.QuadPart)*1000.0/m_SystemFreq.QuadPart;
		FramePtr->SetFrameMergeTime(Time);
		InterlockedIncrement(&m_FrameFinishedCnt);		
		SetIsAnyCalcThreadWorking(true);
		
		if ( NULL != FieldPtr ) 
		{			
			if ( FieldPtr->CheckFieldFramesMergeFinish() == true )
			{
				CAOIField::LockField();
				FieldPtr->SetFieldMergeState(FIELD_MERGE_DONE);	
				CAOIField::UnlockField();
			}
		}	
	}	
//#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateFrameLoadThread()//建立影像載入執行緒
{
//#ifndef OFFLINE_VERSION
	size_t  i=0;
	CString str;
	size_t ThreadIdx = 0;
	const size_t MaxThreadCount=MAX_THREAD_COUNT_FRAME_LOAD;
	if ( this->DeleteFrameLoadThread() == false ) { return false; }	

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		this->m_FrameLoadThreadCmd[ThreadIdx] = THREAD_COMMAND_TO_IDLE;	
	}

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		FrameLoadThreadHandle[ThreadIdx] = (HANDLE)::_beginthreadex(NULL, NULL, &FrameLoadThreadFn, (void*)ThreadIdx, NULL, &FrameLoadThreadID[ThreadIdx]);
		if ( NULL == FrameLoadThreadHandle[ThreadIdx] )
		{	
			this->m_FrameLoadThreadState[ThreadIdx] = THREAD_STATE_NONE;
			this->m_ErrorString = _T("Eorror, Create Thread Fault (FrameLoadThreadHandle == NULL)");		
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_CREATE);
			return false;
		}	
	}

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;	
		str.Format(_T("Frame Load Event[%d]"), ThreadIdx+1);	
		FrameLoadThreadEvent[ThreadIdx] = JetAPI::CreateEvent(NULL, TRUE, TRUE, str);
	}

	DWORD_PTR AffinityMask = GetThreadAffinityMask();
	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		if ( NULL != FrameLoadThreadHandle[ThreadIdx] )
		{	
			::SetThreadPriority(FrameLoadThreadHandle[ThreadIdx], THREAD_PRIORITY_BELOW_NORMAL); 
			::SetThreadAffinityMask(FrameLoadThreadHandle[ThreadIdx], AffinityMask);//111100
		}
	}	
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteFrameLoadThread()//刪除影像載入執行緒
{
//#ifndef OFFLINE_VERSION	
	size_t i = 0;
	size_t ThreadIdx = 0;
	DWORD WaitTime = 1000;//1 sec
	for ( i=0; i<MAX_THREAD_COUNT_FRAME_LOAD; i++ )
	{
		ThreadIdx = i;
		if ( NULL == FrameLoadThreadHandle[ThreadIdx] ) { continue; }		
		this->SetFrameLoadThreadCmd(ThreadIdx, THREAD_COMMAND_TO_EXIT);	
		DWORD Res = ::WaitForSingleObject(FrameLoadThreadHandle[ThreadIdx], WaitTime);
		::CloseHandle(FrameLoadThreadHandle[ThreadIdx]); 
		FrameLoadThreadHandle[ThreadIdx] = NULL;
		if ( NULL != FrameLoadThreadEvent[ThreadIdx] )
		{	
			::CloseHandle(FrameLoadThreadEvent[ThreadIdx]); 
			FrameLoadThreadEvent[ThreadIdx]=NULL; 
		}
	}
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartFrameLoadThread(bool WaitOn)//開始影像載入執行緒	
{
//#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	const size_t ThreadCount   = GetFrameLoadThreadUsingCount();
	m_FrameLoadThreadLocked = false;
	for ( i=0; i<ThreadCount; i++ )
	{		
		ThreadIdx = i;
		if ( NULL == FrameLoadThreadHandle[ThreadIdx] ) { return true; }
		if ( NULL != FrameLoadThreadEvent[ThreadIdx] )
		{	::ResetEvent(FrameLoadThreadEvent[ThreadIdx]); }
		if ( THREAD_STATE_FINISH == m_FrameLoadThreadState[ThreadIdx] )
		{  SetFrameLoadThreadState(ThreadIdx, THREAD_STATE_IDLE); }
		SetFrameLoadThreadCmd(ThreadIdx, THREAD_COMMAND_TO_RUN);
	}

	if ( false == WaitOn )
	{	return true; }
	if ( WaitForFrameLoadThreadStart() == false )
	{	return false; }	
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForFrameLoadThreadIdle()//等待影像載入執行緒停止
{
//#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t FrameCount = GetFramePtrCount();
	const size_t MaxCounts = FrameCount*100+100;	
	const size_t ThreadCount   = GetFrameLoadThreadUsingCount();
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;		
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			//if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_STATE_NONE == m_FrameLoadThreadState[ThreadIdx] )
			{	break; }
			if ( THREAD_STATE_IDLE == m_FrameLoadThreadState[ThreadIdx] )
			{	break; }
			if ( SleepTime > 0 )
			{	::Sleep(SleepTime); }			
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_FrameLoadThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_FrameLoadThreadState[ThreadIdx]);			
			m_ErrorString.Format(_T("Error, WaitForFrameLoadThreadIdle[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE);
			return false;
		}	
	}	
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForFrameLoadThreadStop()//等待影像載入執行緒停止
{
//#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t FrameCount = GetFramePtrCount();
	const size_t MaxCounts = FrameCount*100+100;	
	const size_t ThreadCount   = GetFrameLoadThreadUsingCount();
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;		
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			//if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_STATE_RUNNING != m_FrameLoadThreadState[ThreadIdx] )
			{	break; }			
			if ( SleepTime > 0 )
			{	::Sleep(SleepTime); }			
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_FrameLoadThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_FrameLoadThreadState[ThreadIdx]);			
			m_ErrorString.Format(_T("Error, WaitForFrameLoadThreadStop[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP);
			return false;
		}	
	}	
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForFrameLoadThreadStart()//等待影像載入執行緒開始
{
//#ifndef OFFLINE_VERSION		
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	const size_t MaxCount = 100;
	const size_t SleepTime = 10;
	const size_t ThreadCount   = GetFrameLoadThreadUsingCount();
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		for ( i=0; i<MaxCount; i++ )
		{
			if ( THREAD_COMMAND_TO_RUN != m_FrameLoadThreadCmd[ThreadIdx] )//切入下一個階段
			{	break; }
			if ( THREAD_STATE_IDLE != m_FrameLoadThreadState[ThreadIdx] )
			{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			CString CmdText = GetThreadCommandModeText(m_FrameLoadThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_FrameLoadThreadState[ThreadIdx]);
			m_ErrorString.Format(_T("Error, WaitForFrameLoadThreadStart[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);			
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_START);	
			return false;
		}
	}		
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForFrameLoadThreadFinish()//等待影像載入執行緒結束
{
//#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t FrameCount = GetFramePtrCount();
	const size_t MaxCounts = FrameCount*100+100;	
	const size_t ThreadCount   = GetFrameLoadThreadUsingCount();
	SaveDebugMessage(_T("WaitForFrameLoadThreadFinish-Start"));
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		if ( FrameLoadThreadEvent[ThreadIdx] == NULL ) { continue; }
		size_t CheckWorkingCount=0;
		ResetIsAnyCalcThreadWorking();
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_COMMAND_TO_EXIT == m_FrameLoadThreadCmd[ThreadIdx] ) { return true; }		
			if ( THREAD_COMMAND_TO_IDLE == m_FrameLoadThreadCmd[ThreadIdx] )
			{	break; }

			Res = ::WaitForSingleObject(FrameLoadThreadEvent[ThreadIdx], SleepTime);
			if ( Res != WAIT_TIMEOUT ) 
			{
				if ( THREAD_STATE_RUNNING == m_FrameLoadThreadState[ThreadIdx] )
				{
					if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
					continue; 
				}
				break; 
			}

			if ( GetIsAnyCalcThreadWorking() == true )
			{	
				CheckWorkingCount = 0; 
				ResetIsAnyCalcThreadWorking();
			}
			else
			{
				CheckWorkingCount ++;
				if ( CheckWorkingCount > THREAD_CHECK_NO_WORKING_MAX_COUNT )
				{
					CString CmdText = GetThreadCommandModeText(m_FrameLoadThreadCmd[ThreadIdx]);
					CString StateText = GetThreadStateModeText(m_FrameLoadThreadState[ThreadIdx]);
					SaveDebugMessage(_T("WaitForFrameLoadThreadFinish-Fault (No-Working)"));
					m_ErrorString.Format(_T("Error, WaitForFrameLoadThreadFinish[%d, Cmd=%s, State=%s] too long (No-Working)"), ThreadIdx+1, CmdText, StateText);
					SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
					return false;
				}
			}
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_FrameLoadThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_FrameLoadThreadState[ThreadIdx]);
			SaveDebugMessage(_T("WaitForFrameLoadThreadFinish-Fault"));
			m_ErrorString.Format(_T("Error, WaitForFrameLoadThreadFinish[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
			return false;
		}	
	}	
	SaveDebugMessage(_T("WaitForFrameLoadThreadFinish-End"));
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckFrameLoadThreadStateFinish()//確認影像載入執行緒狀態
{
	size_t i=0;
	for ( i=0; i<MAX_THREAD_COUNT_FRAME_LOAD; i++ )
	{
		if ( NULL == FrameLoadThreadHandle[i] ) { continue; }
		if ( THREAD_COMMAND_TO_NONE == m_FrameLoadThreadCmd[i] ) { continue; }
		if ( THREAD_COMMAND_TO_EXIT == m_FrameLoadThreadCmd[i] ) { continue; }
		if ( THREAD_COMMAND_TO_IDLE == m_FrameLoadThreadCmd[i] ) { continue; }
		if ( m_FrameLoadThreadState[i] != THREAD_STATE_FINISH )
		{
			this->m_ErrorString.Format(_T("Error, m_FrameLoadThreadState#%d is Exception"), i+1);
			return false;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetFrameLoadThreadState(size_t idx, THREAD_STATE_MODE State)//設定影像載入執行緒狀態
{
	if ( idx >= MAX_THREAD_COUNT_FRAME_LOAD ) { return; }
	if ( m_FrameLoadThreadState[idx] == State ) { return; }
	LockThreadFrameLoad();
	m_FrameLoadThreadState[idx] = State;
	UnlockThreadFrameLoad();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE CAOIDataCollect::GetFrameLoadThreadState(size_t idx)//取得影像載入執行緒狀態
{
	if ( idx >= MAX_THREAD_COUNT_FRAME_LOAD ) { return THREAD_STATE_NONE; }
	return m_FrameLoadThreadState[idx];
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetFrameLoadThreadCmd(size_t idx, THREAD_COMMAND_MODE Cmd)//設定影像載入執行緒命令
{
	if ( idx >= MAX_THREAD_COUNT_FRAME_LOAD ) { return; }
	if ( m_FrameLoadThreadCmd[idx] == Cmd ) { return; }
	LockThreadFrameLoad();
	m_FrameLoadThreadCmd[idx] = Cmd;
	UnlockThreadFrameLoad();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CAOIDataCollect::GetFrameLoadThreadCmd(size_t idx)//取得影像載入執行緒命令
{
	if ( idx >= MAX_THREAD_COUNT_FRAME_LOAD ) { return THREAD_COMMAND_TO_NONE; }
	return m_FrameLoadThreadCmd[idx];
}
//-------------------------------------------------------------------------------------//
size_t CAOIDataCollect::GetFrameLoadThreadUsingCount() const   //取得影像載入實際使用數量 
{
	return m_SystemParameter.m_MTCount_FrameLoad;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecFieldCalcFn(size_t ThreadIdx)      //執行視野計算執行緒
{
//#ifndef OFFLINE_VERSION
	CString           str;
	size_t            i=0;		
	double            Time = 0;
	size_t            FieldCount=0;
	size_t            UnDoneCount=0;
	DWORD             SleepTime = 10;//ms			
	CAOIField        *FieldPtr = NULL;		
	LARGE_INTEGER     nStartTime;
	LARGE_INTEGER     nEndTime;
	TASK_MODE         TaskMode = GetTaskMode();
	FIELD_CALC_STATE  FieldState = FIELD_CALC_NONE;		
	THREAD_COMMAND_MODE  ThreadCmd = THREAD_COMMAND_TO_NONE;	
	
	while ( true ) //避免多次取像時會等待區域完成才往下確認, 因此改成重複迴圈確認
	{	
		ThreadCmd = GetFieldCalcThreadCmd(ThreadIdx);
		if ( GetIsSystemReleased() == true ) { return true; }
		if ( GetIsSystemException() == true ) { return true; }
		if ( GetIsAllCalcThreadStop() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }

		UnDoneCount=0;				
		FieldCount = GetFieldPtrCount();
		for ( i=0; i<FieldCount; i++ )//避免最後一筆無法進來迴圈, 因此手動+1
		{	
			ThreadCmd = GetFieldCalcThreadCmd(ThreadIdx);
			if ( GetIsSystemReleased() == true ) { return true; }
			if ( GetIsSystemException() == true ) { return true; }
			if ( GetIsAllCalcThreadStop() == true ) { return true; }
			if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
			if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }
		
			FieldPtr = this->GetFieldPtr(i, false);
			if ( NULL == FieldPtr ) { continue; }
			FieldState = FieldPtr->GetFieldCalcState();
			if ( FIELD_CALC_DONE == FieldState ) { continue; }
			if ( FIELD_CALC_DOING == FieldState ) { continue; }		
			//if ( FIELD_CALC_CLEAR == FieldState ) { continue; }
	
			//同步化
			CAOIField::LockField();
			FieldState = FieldPtr->GetFieldCalcState();//重新取得狀態, 因為在鎖住前可能別的執行緒又進去算了
			if ( FIELD_CALC_NONE != FieldState )
			{
				CAOIField::UnlockField();
				continue;
			}
			UnDoneCount ++;
			FieldPtr->SetFieldCalcState(FIELD_CALC_DOING);
			CAOIField::UnlockField();					
			if ( FieldPtr->GetFieldMergeState() != FIELD_MERGE_DONE )
			{				
				CAOIField::LockField();
				FieldPtr->SetFieldCalcState(FIELD_CALC_NONE);
				CAOIField::UnlockField();				
				continue;
			}	
			QueryPerformanceCounter(&nStartTime);
			CAOIProject *ProjectPtr = FieldPtr->GetFieldProjectPtr();
			const size_t FieldFrameCount=FieldPtr->GetFieldFramePtrCount();
			if ( NULL != ProjectPtr )
			{	ProjectPtr->TestProjectSpecialInspection(FieldPtr);	}
			else
			{	FieldPtr->SetFieldFrameCalcState(FRAME_CALC_DONE);	}
			FieldPtr->SetFieldCalcState(FIELD_CALC_DONE);		
			QueryPerformanceCounter(&nEndTime);
			Time = (nEndTime.QuadPart - nStartTime.QuadPart)*1000.0/m_SystemFreq.QuadPart;
			//FieldPtr->SetFieldCalcTime(Time);
			InterlockedIncrement(&m_FieldCalcFinishedCnt);	
			SetIsAnyCalcThreadWorking(true);
		#ifdef _DEBUG
			str.Format(_T("Calc Field[TH:%d]#%d/%d Finished\n"), ThreadIdx+1, i+1, FieldCount);
			TRACE(str);
		#endif//_DEBUG		
		}	

		if ( 0 == UnDoneCount )
		{	break; }
		if ( SleepTime > 0 ) //等一下讓相機取影像
		{	::Sleep(SleepTime); }
	};
//#endif//OFFLINE_VERSION
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateFieldCalcThread()                //建立視野計算執行緒
{
//#ifndef OFFLINE_VERSION
#ifdef FIELD_CALC_THREAD_USE
	size_t  i=0;
	CString str;
	size_t ThreadIdx = 0;
	const size_t MaxThreadCount=MAX_THREAD_COUNT_FIELD_CALC;
	if ( this->DeleteFieldCalcThread() == false ) { return false; }	

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		this->m_FieldCalcThreadCmd[ThreadIdx] = THREAD_COMMAND_TO_IDLE;	
	}

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		FieldCalcThreadHandle[ThreadIdx] = (HANDLE)::_beginthreadex(NULL, NULL, &FieldCalcThreadFn, (void*)ThreadIdx, NULL, &FieldCalcThreadID[ThreadIdx]);
		if ( NULL == FieldCalcThreadHandle[ThreadIdx] )
		{	
			this->m_FieldCalcThreadState[ThreadIdx] = THREAD_STATE_NONE;
			this->m_ErrorString = _T("Eorror, Create Thread Fault (FieldCalcThreadHandle == NULL)");		
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_CREATE);
			return false;
		}	
	}

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;	
		str.Format(_T("Field Calc Event[%d]"), ThreadIdx+1);	
		FieldCalcThreadEvent[ThreadIdx] = JetAPI::CreateEvent(NULL, TRUE, TRUE, str);
	}

	DWORD_PTR AffinityMask = GetThreadAffinityMask();
	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		if ( NULL != FieldCalcThreadHandle[ThreadIdx] )
		{	
			::SetThreadPriority(FieldCalcThreadHandle[ThreadIdx], THREAD_PRIORITY_BELOW_NORMAL); 
			::SetThreadAffinityMask(FieldCalcThreadHandle[ThreadIdx], AffinityMask);//111100
		}
	}	
#endif//FIELD_CALC_THREAD_USE
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteFieldCalcThread()                //刪除視野計算執行緒
{
//#ifndef OFFLINE_VERSION	
#ifdef FIELD_CALC_THREAD_USE
	size_t i = 0;
	size_t ThreadIdx = 0;
	DWORD WaitTime = 1000;//1 sec
	for ( i=0; i<MAX_THREAD_COUNT_FIELD_CALC; i++ )
	{
		ThreadIdx = i;
		if ( NULL == FieldCalcThreadHandle[ThreadIdx] ) { continue; }		
		this->SetFieldCalcThreadCmd(ThreadIdx, THREAD_COMMAND_TO_EXIT);	
		DWORD Res = ::WaitForSingleObject(FieldCalcThreadHandle[ThreadIdx], WaitTime);
		::CloseHandle(FieldCalcThreadHandle[ThreadIdx]); 
		FieldCalcThreadHandle[ThreadIdx] = NULL;
		if ( NULL != FieldCalcThreadEvent[ThreadIdx] )
		{	
			::CloseHandle(FieldCalcThreadEvent[ThreadIdx]); 
			FieldCalcThreadEvent[ThreadIdx]=NULL; 
		}
	}
#endif//FIELD_CALC_THREAD_USE
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartFieldCalcThread(bool WaitOn)      //開始視野計算執行緒	
{
//#ifndef OFFLINE_VERSION	
#ifdef FIELD_CALC_THREAD_USE
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	const size_t ThreadCount   = GetFieldCalcThreadUsingCount();
	m_FrameLoadThreadLocked = false;
	for ( i=0; i<ThreadCount; i++ )
	{		
		ThreadIdx = i;
		if ( NULL == FieldCalcThreadHandle[ThreadIdx] ) { return true; }
		if ( NULL != FieldCalcThreadEvent[ThreadIdx] )
		{	::ResetEvent(FieldCalcThreadEvent[ThreadIdx]); }
		if ( THREAD_STATE_FINISH == m_FieldCalcThreadState[ThreadIdx] )
		{  SetFieldCalcThreadState(ThreadIdx, THREAD_STATE_IDLE); }
		SetFieldCalcThreadCmd(ThreadIdx, THREAD_COMMAND_TO_RUN);
	}

	if ( false == WaitOn )
	{	return true; }
	if ( WaitForFieldCalcThreadStart() == false )
	{	return false; }	
#endif//FIELD_CALC_THREAD_USE
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForFieldCalcThreadIdle()           //等待視野計算執行緒閒置
{
//#ifndef OFFLINE_VERSION	
#ifdef FIELD_CALC_THREAD_USE
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t FieldCount = GetFieldPtrCount();
	const size_t MaxCounts = FieldCount*100+100;	
	const size_t ThreadCount   = GetFieldCalcThreadUsingCount();
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;		
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			//if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_STATE_NONE == m_FieldCalcThreadState[ThreadIdx] )
			{	break; }
			if ( THREAD_STATE_IDLE == m_FieldCalcThreadState[ThreadIdx] )
			{	break; }
			if ( SleepTime > 0 )
			{	::Sleep(SleepTime); }			
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_FieldCalcThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_FieldCalcThreadState[ThreadIdx]);					
			m_ErrorString.Format(_T("Error, WaitForFieldCalcThreadIdle[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE);
			return false;
		}	
	}	
#endif//FIELD_CALC_THREAD_USE
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForFieldCalcThreadStop()           //等待視野計算執行緒停止
{
//#ifndef OFFLINE_VERSION	
#ifdef FIELD_CALC_THREAD_USE
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t FieldCount = GetFieldPtrCount();
	const size_t MaxCounts = FieldCount*100+100;	
	const size_t ThreadCount   = GetFieldCalcThreadUsingCount();
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;		
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			//if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_STATE_RUNNING != m_FieldCalcThreadState[ThreadIdx] )
			{	break; }			
			if ( SleepTime > 0 )
			{	::Sleep(SleepTime); }			
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_FieldCalcThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_FieldCalcThreadState[ThreadIdx]);					
			m_ErrorString.Format(_T("Error, WaitForFieldCalcThreadStop[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP);
			return false;
		}	
	}	
#endif//FIELD_CALC_THREAD_USE
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForFieldCalcThreadStart()          //等待視野計算執行緒開始
{
//#ifndef OFFLINE_VERSION		
#ifdef FIELD_CALC_THREAD_USE
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	const size_t MaxCount = 100;
	const size_t SleepTime = 10;
	const size_t ThreadCount   = GetFieldCalcThreadUsingCount();
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		for ( i=0; i<MaxCount; i++ )
		{
			if ( THREAD_COMMAND_TO_RUN != m_FieldCalcThreadCmd[ThreadIdx] )//切入下一個階段
			{	break; }
			if ( THREAD_STATE_IDLE != m_FieldCalcThreadState[ThreadIdx] )
			{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			CString CmdText = GetThreadCommandModeText(m_FieldCalcThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_FieldCalcThreadState[ThreadIdx]);					
			m_ErrorString.Format(_T("Error, WaitForFieldCalcThreadStart[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_START);	
			return false;
		}
	}		
#endif//FIELD_CALC_THREAD_USE
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForFieldCalcThreadFinish()         //等待視野計算執行緒結束
{
//#ifndef OFFLINE_VERSION	
#ifdef FIELD_CALC_THREAD_USE
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t FieldCount = GetFieldPtrCount();
	const size_t MaxCounts = FieldCount*100+100;	
	const size_t ThreadCount   = GetFieldCalcThreadUsingCount();
	SaveDebugMessage(_T("WaitForFieldCalcThreadFinish-Start"));
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		if ( FieldCalcThreadEvent[ThreadIdx] == NULL ) { continue; }
		size_t CheckWorkingCount=0;
		ResetIsAnyCalcThreadWorking();
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_COMMAND_TO_EXIT == m_FieldCalcThreadCmd[ThreadIdx] ) { return true; }		
			if ( THREAD_COMMAND_TO_IDLE == m_FieldCalcThreadCmd[ThreadIdx] )
			{	break; }

			Res = ::WaitForSingleObject(FieldCalcThreadEvent[ThreadIdx], SleepTime);
			if ( Res != WAIT_TIMEOUT ) 
			{
				if ( THREAD_STATE_RUNNING == m_FieldCalcThreadState[ThreadIdx] )
				{
					if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
					continue; 
				}
				break; 
			}

			if ( GetIsAnyCalcThreadWorking() == true )
			{	
				CheckWorkingCount = 0; 
				ResetIsAnyCalcThreadWorking();
			}
			else
			{
				CheckWorkingCount ++;
				if ( CheckWorkingCount > THREAD_CHECK_NO_WORKING_MAX_COUNT )
				{
					CString CmdText = GetThreadCommandModeText(m_FieldCalcThreadCmd[ThreadIdx]);
					CString StateText = GetThreadStateModeText(m_FieldCalcThreadState[ThreadIdx]); 
					SaveDebugMessage(_T("WaitForFieldCalcThreadFinish-Fault (No-Working)"));
					m_ErrorString.Format(_T("Error, WaitForFieldCalcThreadFinish[%d, Cmd=%s, State=%s] too long (No-Working)"), ThreadIdx+1, CmdText, StateText);
					SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
					return false;
				}
			}	
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_FieldCalcThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_FieldCalcThreadState[ThreadIdx]); 
			SaveDebugMessage(_T("WaitForFieldCalcThreadFinish-Fault"));
			m_ErrorString.Format(_T("Error, WaitForFieldCalcThreadFinish[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
			return false;
		}	
	}	
	SaveDebugMessage(_T("WaitForFieldCalcThreadFinish-End"));
#endif//FIELD_CALC_THREAD_USE
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckFieldCalcThreadStateFinish()      //確認視野計算執行緒狀態
{
#ifdef FIELD_CALC_THREAD_USE
	size_t i=0;
	for ( i=0; i<MAX_THREAD_COUNT_FIELD_CALC; i++ )
	{
		if ( NULL == FieldCalcThreadHandle[i] ) { continue; }
		if ( THREAD_COMMAND_TO_NONE == m_FieldCalcThreadCmd[i] ) { continue; }
		if ( THREAD_COMMAND_TO_EXIT == m_FieldCalcThreadCmd[i] ) { continue; }
		if ( THREAD_COMMAND_TO_IDLE == m_FieldCalcThreadCmd[i] ) { continue; }
		if ( m_FieldCalcThreadState[i] != THREAD_STATE_FINISH )
		{
			this->m_ErrorString.Format(_T("Error, m_FieldCalcThreadState#%d is Exception"), i+1);
			return false;
		}
	}
#endif//FIELD_CALC_THREAD_USE
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetFieldCalcThreadState(size_t idx, THREAD_STATE_MODE State)//設定視野計算執行緒狀態
{
	if ( idx >= MAX_THREAD_COUNT_FIELD_CALC ) { return; }
	if ( m_FieldCalcThreadState[idx] == State ) { return; }
	LockThreadFieldCalc();
	m_FieldCalcThreadState[idx] = State;
	UnlockThreadFieldCalc();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE CAOIDataCollect::GetFieldCalcThreadState(size_t idx)              //取得視野計算執行緒狀態
{
	if ( idx >= MAX_THREAD_COUNT_FIELD_CALC ) { return THREAD_STATE_NONE; }
	return m_FieldCalcThreadState[idx];
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetFieldCalcThreadCmd(size_t idx, THREAD_COMMAND_MODE Cmd) //設定視野計算執行緒命令
{
	if ( idx >= MAX_THREAD_COUNT_FIELD_CALC ) { return; }
	if ( m_FieldCalcThreadCmd[idx] == Cmd ) { return; }
	LockThreadFieldCalc();
	m_FieldCalcThreadCmd[idx] = Cmd;
	UnlockThreadFieldCalc();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CAOIDataCollect::GetFieldCalcThreadCmd(size_t idx)	               //取得視野計算執行緒命令
{
	if ( idx >= MAX_THREAD_COUNT_FIELD_CALC ) { return THREAD_COMMAND_TO_NONE; }
	return m_FieldCalcThreadCmd[idx];
}
//-------------------------------------------------------------------------------------//
size_t CAOIDataCollect::GetFieldCalcThreadUsingCount() const   //取得視野計算實際使用數量 
{
	const size_t Count=MIN(MAX_THREAD_COUNT_FIELD_CALC, m_SystemParameter.m_MTCount_RegionCalc);
	return Count;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecRegionCalcFn(size_t ThreadIdx)//執行區域計算執行緒
{
	bool IsOK = true;
	THREAD_GRAB_MODE  ThreadGrabMode = GetThreadGrabMode();
	switch ( ThreadGrabMode )
	{
	case THREAD_GRAB_PROJECT_MAP://專案底圖
		IsOK = ExecRegionCalcFn_ProjectMap(ThreadIdx);
		break;
	case THREAD_GRAB_PROJECT_MARK://專案特徵
		IsOK = ExecRegionCalcFn_ProjectMark(ThreadIdx);
		break;
	case THREAD_GRAB_PANEL_FD://整板定位點
		IsOK = ExecRegionCalcFn_PanelFd(ThreadIdx);
		break;
	case THREAD_GRAB_BOARD_FD://單板定位點
		IsOK = ExecRegionCalcFn_BoardFd(ThreadIdx);
		break;
	case THREAD_GRAB_BARCODE://條碼檢測
	case THREAD_GRAB_PROJECT_TEST://專案檢測
		IsOK = ExecRegionCalcFn_Inspection(ThreadIdx);
		break;
	case THREAD_GRAB_PROJECT_OPEN_CODE://專案開檔條碼
		IsOK = ExecRegionCalcFn_ProjectOpenCode(ThreadIdx);		
		break;
	}
	return IsOK;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecRegionCalcFn_ProjectMap(size_t ThreadIdx)//執行區域計算執行緒-專案底圖
{
#ifndef OFFLINE_VERSION//Kai-20170410
	CString            str;
	size_t             i=0;	
	size_t             LastIdx=0;
	bool               Reset = false;
	DWORD              SleepTime = 10;//ms
	CAOIRgn           *RgnPtr = NULL;
	REGION_CALC_STATE  RegionState = REGION_CALC_NONE;
	const size_t       RegionCount = this->GetRgnPtrCount();
	THREAD_COMMAND_MODE  ThreadCmd = THREAD_COMMAND_TO_NONE;
	THREAD_STATE_MODE    MotionThreadStats = THREAD_STATE_NONE;
	Reset = false;
	for ( i=0; i<RegionCount+1; i++ )//避免最後一筆無法進來迴圈, 因此手動+1
	{	
		ThreadCmd = GetRegionCalcThreadCmd(ThreadIdx);
		if ( GetIsSystemReleased() == true ) { return true; }				
		if ( GetIsSystemException() == true ) { return true; }
		if ( GetIsAllCalcThreadStop() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }

		if ( true == Reset ) //由於上一次計算尚未有圖片, 退回引數號碼1步
		{			
			i = LastIdx;
			Reset = false;
		}		
		if ( i >= RegionCount )
		{	break; }
		RgnPtr = this->GetRgnPtr(i, false);
		if ( NULL == RgnPtr ) { continue; }
		RegionState = RgnPtr->GetRgnCalcState();
		if ( REGION_CALC_DONE == RegionState ) { continue; }
		if ( REGION_CALC_DOING == RegionState ) { continue; }
		if ( REGION_CALC_CLEAR == RegionState ) { continue; }		
	
		//同步化
		CAOIRgn::LockRgn();
		RegionState = RgnPtr->GetRgnCalcState();//重新取得狀態, 因為在鎖住前可能別的執行緒又進去算了
		if ( REGION_CALC_NONE != RegionState )
		{
			CAOIRgn::UnlockRgn();
			continue;
		}
		RgnPtr->SetRgnCalcState(REGION_CALC_DOING);
		CAOIRgn::UnlockRgn();
		LastIdx = i;		
		if ( RgnPtr->CheckRgnFieldAllFrameMergeFinish() == false )
		{
			Reset = true;			
			CAOIRgn::LockRgn();
			RgnPtr->SetRgnCalcState(REGION_CALC_NONE);
			CAOIRgn::UnlockRgn();
			if ( SleepTime > 0 ) //等一下讓相機取影像
			{	::Sleep(SleepTime); }
			continue;
		}
		if ( RgnPtr->ExecRgnCalc_ProjectMap() == false )
		{
			LockThread();
			this->m_ErrorString.Format(_T("Error, Calc Region[%d] #%d Fault"), ThreadIdx+1, i+1);
			UnlockThread();
			return false;
		}		
	#ifdef _DEBUG
		str.Format(_T("Calc Region[%d]#%d/%d Finished\n"), ThreadIdx+1, i+1, RegionCount);
		TRACE(str);
	#endif//_DEBUG
		RgnPtr->SetRgnCalculated(true);
		RgnPtr->SetRgnCalcState(REGION_CALC_DONE);		
		RgnPtr->UpdateRgnParentCalcStateDone();
		InterlockedIncrement(&m_RgnFinishedCnt);	
		SetIsAnyCalcThreadWorking(true);

		MotionThreadStats = MotionCtrlPtr->GetMotionThreadState_GoStop();
		if ( THREAD_STATE_FINISH==MotionThreadStats || THREAD_STATE_IDLE==MotionThreadStats )
		{	PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_PROJECT_MAP, TRUE); }
	}	
#endif//OFFLINE_VERSION//Kai-20170410
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecRegionCalcFn_PanelFd(size_t ThreadIdx)//執行區域計算執行緒-整板定位點
{
#ifndef OFFLINE_VERSION//Kai-20170410
	CString            str;
	size_t             i=0;	
	size_t             LastIdx=0;
	bool               Reset = false;
	DWORD              SleepTime = 10;//ms
	CAOIFd            *FdPtr = NULL;
	CAOIRgn           *RgnPtr = NULL;
	const bool         MultiFdLight = GetSystemMultiFdLight();
	AOI_OBJ_TYPE       AOIObjType=AOI_OBJ_BASIC;
	REGION_CALC_STATE  RegionState = REGION_CALC_NONE;	
	const size_t       RegionCount = GetRgnPtrCount();
	THREAD_COMMAND_MODE  ThreadCmd = THREAD_COMMAND_TO_NONE;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{	return false; }

	Reset = false;
	for ( i=0; i<RegionCount+1; i++ )//避免最後一筆無法進來迴圈, 因此手動+1
	{	
		ThreadCmd = GetRegionCalcThreadCmd(ThreadIdx);
		if ( GetIsSystemReleased() == true ) { return true; }				
		if ( GetIsSystemException() == true ) { return true; }
		if ( GetIsAllCalcThreadStop() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }

		if ( true == Reset ) //由於上一次計算尚未有圖片, 退回引數號碼1步
		{			
			i = LastIdx;
			Reset = false;
		}		
		if ( i >= RegionCount )
		{	break; }
		RgnPtr = this->GetRgnPtr(i, false);
		if ( NULL == RgnPtr ) { continue; }
		AOIObjType = RgnPtr->GetObjType();
		if ( AOI_OBJ_FD != AOIObjType ) { continue; }
		RegionState = RgnPtr->GetRgnCalcState();
		if ( REGION_CALC_DONE == RegionState ) { continue; }
		if ( REGION_CALC_DOING == RegionState ) { continue; }
		if ( REGION_CALC_CLEAR == RegionState ) { continue; }
	
		//同步化
		CAOIRgn::LockRgn();
		RegionState = RgnPtr->GetRgnCalcState();//重新取得狀態, 因為在鎖住前可能別的執行緒又進去算了
		if ( REGION_CALC_NONE != RegionState )
		{
			CAOIRgn::UnlockRgn();
			continue;
		}
		RgnPtr->SetRgnCalcState(REGION_CALC_DOING);
		CAOIRgn::UnlockRgn();
		LastIdx = i;		
		if ( RgnPtr->CheckRgnFieldAllFrameMergeFinish() == false )
		{
			Reset = true;			
			CAOIRgn::LockRgn();
			RgnPtr->SetRgnCalcState(REGION_CALC_NONE);
			CAOIRgn::UnlockRgn();
			if ( SleepTime > 0 ) //等一下讓相機取影像
			{	::Sleep(SleepTime); }
			continue;
		}
		
		FdPtr = dynamic_cast<CAOIFd*>(RgnPtr);
		if ( true == MultiFdLight )
		{
			if ( FdPtr->ExtractFdFrame() == false ) 
			{
				LockThread();
				this->m_ErrorString.Format(_T("Error, Extract Fd[%d] Frame #%d Fault"), ThreadIdx+1, i+1);
				UnlockThread();
				return false;
			}
			if ( FdPtr->ExecFdInspection() == false )
			{
				LockThread();
				this->m_ErrorString.Format(_T("Error, Inspect Fd[%d] #%d Fault"), ThreadIdx+1, i+1);
				UnlockThread();
				return false;
			}
			if ( FdPtr->GetFdKeepImage() == false )
			{	FdPtr->ClearRgnImageBuffer(); }
		}
		else
		{
			if ( FdPtr->FindFiducial() == false )
			{
				LockThread();
				this->m_ErrorString.Format(_T("Error, Find Fd[%d] #%d Fault"), ThreadIdx+1, i+1);
				UnlockThread();
				return false;
			}	
		}				
	#ifdef _DEBUG
		str.Format(_T("Find Fd[%d]#%d/%d Finished\n"), ThreadIdx+1, i+1, RegionCount);
		TRACE(str);
	#endif//_DEBUG
		RgnPtr->SetRgnCalculated(true);
		RgnPtr->SetRgnCalcState(REGION_CALC_DONE);		
		RgnPtr->UpdateRgnParentCalcStateDone();
		InterlockedIncrement(&m_RgnFinishedCnt);
		SetIsAnyCalcThreadWorking(true);

		PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	}		
#endif//OFFLINE_VERSION//Kai-20170410
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecRegionCalcFn_BoardFd(size_t ThreadIdx)//執行區域計算執行緒-單板定位點
{
#ifndef OFFLINE_VERSION//Kai-20170410
	CString            str;
	size_t             i=0;	
	size_t             LastIdx=0;
	bool               Reset = false;
	DWORD              SleepTime = 10;//ms
	CAOIFd            *FdPtr = NULL;
	CAOIRgn           *RgnPtr = NULL;
	const bool         MultiFdLight = GetSystemMultiFdLight();
	AOI_OBJ_TYPE       AOIObjType=AOI_OBJ_BASIC;
	REGION_CALC_STATE  RegionState = REGION_CALC_NONE;
	const size_t       RegionCount = this->GetRgnPtrCount();
	THREAD_COMMAND_MODE  ThreadCmd = THREAD_COMMAND_TO_NONE;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{	return false; }

	Reset = false;
	for ( i=0; i<RegionCount+1; i++ )//避免最後一筆無法進來迴圈, 因此手動+1
	{	
		ThreadCmd = GetRegionCalcThreadCmd(ThreadIdx);
		if ( GetIsSystemReleased() == true ) { return true; }				
		if ( GetIsSystemException() == true ) { return true; }
		if ( GetIsAllCalcThreadStop() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }

		if ( true == Reset ) //由於上一次計算尚未有圖片, 退回引數號碼1步
		{			
			i = LastIdx;
			Reset = false;
		}		
		if ( i >= RegionCount )
		{	break; }
		RgnPtr = this->GetRgnPtr(i, false);
		if ( NULL == RgnPtr ) { continue; }
		AOIObjType = RgnPtr->GetObjType();
		if ( AOI_OBJ_FD != AOIObjType ) { continue; }
		RegionState = RgnPtr->GetRgnCalcState();
		if ( REGION_CALC_DONE == RegionState ) { continue; }
		if ( REGION_CALC_DOING == RegionState ) { continue; }
		if ( REGION_CALC_CLEAR == RegionState ) { continue; }

		//同步化
		CAOIRgn::LockRgn();
		RegionState = RgnPtr->GetRgnCalcState();//重新取得狀態, 因為在鎖住前可能別的執行緒又進去算了
		if ( REGION_CALC_NONE != RegionState )
		{
			CAOIRgn::UnlockRgn();
			continue;
		}
		RgnPtr->SetRgnCalcState(REGION_CALC_DOING);
		CAOIRgn::UnlockRgn();
		LastIdx = i;		
		if ( RgnPtr->CheckRgnFieldAllFrameMergeFinish() == false )
		{
			Reset = true;			
			CAOIRgn::LockRgn();
			RgnPtr->SetRgnCalcState(REGION_CALC_NONE);
			CAOIRgn::UnlockRgn();
			if ( SleepTime > 0 ) //等一下讓相機取影像
			{	::Sleep(SleepTime); }
			continue;
		}
		FdPtr = dynamic_cast<CAOIFd*>(RgnPtr);

		if ( true == MultiFdLight )
		{
			if ( FdPtr->ExtractFdFrame() == false ) 
			{
				LockThread();
				this->m_ErrorString.Format(_T("Error, Extract Fd[%d] Frame #%d Fault"), ThreadIdx+1, i+1);
				UnlockThread();
				return false;
			}
			if ( FdPtr->ExecFdInspection() == false )
			{
				LockThread();
				this->m_ErrorString.Format(_T("Error, Inspect Fd[%d] #%d Fault"), ThreadIdx+1, i+1);
				UnlockThread();
				return false;
			}
			if ( FdPtr->GetFdKeepImage() == false )
			{	FdPtr->ClearRgnImageBuffer(); }
		}
		else
		{
			if ( FdPtr->FindFiducial() == false )
			{
				LockThread();
				this->m_ErrorString.Format(_T("Error, Find Fd[%d] #%d Fault"), ThreadIdx+1, i+1);
				UnlockThread();
				return false;
			}
		}
	#ifdef _DEBUG
		str.Format(_T("Find Fd[%d]#%d/%d Finished\n"), ThreadIdx+1, i+1, RegionCount);
		TRACE(str);
	#endif//_DEBUG
		RgnPtr->SetRgnCalculated(true);
		RgnPtr->SetRgnCalcState(REGION_CALC_DONE);		
		RgnPtr->UpdateRgnParentCalcStateDone();
		InterlockedIncrement(&m_RgnFinishedCnt);
		SetIsAnyCalcThreadWorking(true);

		PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	}		
#endif//OFFLINE_VERSION//Kai-20170410
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecRegionCalcFn_Inspection(size_t ThreadIdx)//執行區域計算執行緒-專案檢測
{
//#ifndef OFFLINE_VERSION//Kai-20170410
	CString            str;
	CString            RgnName;
	size_t             i=0;		
	size_t             LastIdx=0;
	size_t             RgnIndex=0;
	size_t             FinishCnt=0;	
	size_t             UnDoneCount=0;
	size_t             RegionCount=0;
	bool               Reset = false;
	bool               Finished = false;
	DWORD              SleepTime = 10;//ms	
	CAOIFd            *FdPtr = NULL;	
	CAOIRgn           *RgnPtr = NULL;		
	CAOIRgn           *RgnPtr2 = NULL;
	CAOIMark          *MarkPtr = NULL;
	CAOIWindow        *WindowPtr = NULL;
	CAOIBarcode       *BarcodePtr = NULL;
	CAOIComponent     *ComponentPtr = NULL;
	AOI_OBJ_TYPE       AOIObjType=AOI_OBJ_BASIC;
	REGION_CALC_STATE  RegionState = REGION_CALC_NONE;	
	THREAD_COMMAND_MODE  ThreadCmd = THREAD_COMMAND_TO_NONE;
	TASK_MODE            TaskMode = GetTaskMode();

	while ( true )//避免多次取像時會等待區域完成才往下確認, 因此改成重複迴圈確認
	{	
		ThreadCmd = GetRegionCalcThreadCmd(ThreadIdx);
		if ( GetIsSystemReleased() == true ) { return true; }				
		if ( GetIsSystemException() == true ) { return true; }
		if ( GetIsAllCalcThreadStop() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }		
		
		UnDoneCount=0;
		RegionCount=GetRgnPtrCount();		
		for ( i=0; i<RegionCount; i++ )
		{	
			ThreadCmd = GetRegionCalcThreadCmd(ThreadIdx);
			if ( GetIsSystemReleased() == true ) { return true; }				
			if ( GetIsSystemException() == true ) { return true; }
			if ( GetIsAllCalcThreadStop() == true ) { return true; }
			if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
			if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }

			RgnPtr = this->GetRgnPtr(i, false);
			if ( NULL == RgnPtr ) { continue; }		
			RegionState = RgnPtr->GetRgnCalcState();
			if ( REGION_CALC_DONE == RegionState ) { continue; }
			if ( REGION_CALC_DOING == RegionState ) { continue; }
			if ( REGION_CALC_CLEAR == RegionState ) { continue; }

			//同步化
			CAOIRgn::LockRgn();
			RegionState = RgnPtr->GetRgnCalcState();//重新取得狀態, 因為在鎖住前可能別的執行緒又進去算了
			if ( REGION_CALC_NONE != RegionState )
			{
				CAOIRgn::UnlockRgn();
				continue;
			}
			UnDoneCount ++;
			RgnPtr->SetRgnCalcState(REGION_CALC_DOING);
			CAOIRgn::UnlockRgn();

			if ( RgnPtr->CheckRgnNoNeedToCalculate() == false )
			{
				RgnPtr->SetRgnCalculated(true);
				RgnPtr->SetRgnCalcState(REGION_CALC_DONE);
				RgnPtr->UpdateRgnParentCalcStateDone();
				InterlockedIncrement(&m_RgnFinishedCnt);
				SetIsAnyCalcThreadWorking(true);
				continue;
			}

			if ( RgnPtr->CheckRgnFieldAllFrameMergeFinish() == false )
			{
				Reset = true;			
				CAOIRgn::LockRgn();
				RgnPtr->SetRgnCalcState(REGION_CALC_NONE);
				CAOIRgn::UnlockRgn();
				//if ( SleepTime > 0 ) //等一下讓相機取影像
				//{	::Sleep(SleepTime); }
				continue;
			}
			if ( RgnPtr->CheckRgnReadyToCalc() == false )
			{
				Reset = true;			
				CAOIRgn::LockRgn();
				RgnPtr->SetRgnCalcState(REGION_CALC_NONE);
				CAOIRgn::UnlockRgn();
				//if ( SleepTime > 0 ) //等一下讓物件可以計算
				//{	::Sleep(SleepTime); }
				continue;
			}

			MarkPtr = NULL;
			WindowPtr = NULL;
			BarcodePtr = NULL;
			ComponentPtr = NULL;
			RgnIndex = RgnPtr->GetRgnIndex();
			AOIObjType = RgnPtr->GetObjType();
			switch ( AOIObjType )
			{
			case AOI_OBJ_FD:        FdPtr = dynamic_cast<CAOIFd*>(RgnPtr);	break;		
			case AOI_OBJ_RGN:		RgnPtr = dynamic_cast<CAOIRgn*>(RgnPtr);	break;		
			case AOI_OBJ_MARK:		MarkPtr = dynamic_cast<CAOIMark*>(RgnPtr);	break;
			case AOI_OBJ_WINDOW:	WindowPtr = dynamic_cast<CAOIWindow*>(RgnPtr);	break;
			case AOI_OBJ_BARCODE:   BarcodePtr = dynamic_cast<CAOIBarcode*>(RgnPtr);	break;
			case AOI_OBJ_COMPONENT:	ComponentPtr = dynamic_cast<CAOIComponent*>(RgnPtr); break;
			default:	AOIObjType = AOI_OBJ_BASIC;	break;
			}
			if ( TASK_EXPORT_OFFLINE == TaskMode )
			{	Finished = true;	}
			else
			{
				if ( AOI_OBJ_BASIC == AOIObjType )
				{	
					RgnPtr->SetRgnCalculated(true);
					RgnPtr->SetRgnCalcState(REGION_CALC_DONE);
					RgnPtr->UpdateRgnParentCalcStateDone();
					InterlockedIncrement(&m_RgnFinishedCnt);
					SetIsAnyCalcThreadWorking(true);
					continue;
				}	
				if ( RgnPtr->ExtractRgnDerivedFrame(Finished) == false )
				{				
					LockThread();
					RgnPtr2 = RgnPtr->GetRgnParent();
					if ( NULL == RgnPtr2 )
					{	RgnName = RgnPtr->GetRgnDerivedName();	}
					else
					{	RgnName = RgnPtr2->GetRgnDerivedName();	}
					this->m_ErrorString.Format(_T("Error, Extract Rgn[%d]-[%s] #%d Fault"), ThreadIdx+1, RgnName, i+1);
					UnlockThread();
					return false;
				}
			}
		#ifdef _DEBUG
			str.Format(_T("Region Calc[TH:%d]#%d/%d Finished\n"), ThreadIdx+1, i+1, RegionCount);
			TRACE(str);
		#endif//_DEBUG
			RgnPtr->SetRgnCalculated(true);
			if ( true == Finished )
			{	
				RgnPtr->SetRgnCalcState(REGION_CALC_DONE);	
				RgnPtr->UpdateRgnParentCalcStateDone();
			}
			InterlockedIncrement(&m_RgnFinishedCnt);
			SetIsAnyCalcThreadWorking(true);

			FinishCnt = m_RgnFinishedCnt;
			//if ( TASK_TUNING_PROJECT==TaskMode || TASK_TUNING_OFFLINE==TaskMode )
			{
				//if ( (FinishCnt%4) == 0 )
				{	PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);	}
			}
		}	
		if ( 0 == UnDoneCount )
		{	break; }
		if ( SleepTime > 0 ) //等一下讓物件可以計算
		{	::Sleep(SleepTime); }
	};
//#endif//OFFLINE_VERSION//Kai-20170410
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecRegionCalcFn_ProjectMark(size_t ThreadIdx)//執行區域計算執行緒-專案標記
{
#ifndef OFFLINE_VERSION
	CString            str;
	size_t             i=0;	
	size_t             LastIdx=0;
	bool               Reset = false;
	DWORD              SleepTime = 10;//ms
	CAOIRgn           *RgnPtr = NULL;
	REGION_CALC_STATE  RegionState = REGION_CALC_NONE;
	const size_t       RegionCount = this->GetRgnPtrCount();
	THREAD_COMMAND_MODE  ThreadCmd = THREAD_COMMAND_TO_NONE;
	THREAD_STATE_MODE    MotionThreadStats = THREAD_STATE_NONE;
	Reset = false;
	for ( i=0; i<RegionCount+1; i++ )//避免最後一筆無法進來迴圈, 因此手動+1
	{	
		ThreadCmd = GetRegionCalcThreadCmd(ThreadIdx);
		if ( GetIsSystemReleased() == true ) { return true; }				
		if ( GetIsSystemException() == true ) { return true; }
		if ( GetIsAllCalcThreadStop() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }

		if ( true == Reset ) //由於上一次計算尚未有圖片, 退回引數號碼1步
		{			
			i = LastIdx;
			Reset = false;
		}		
		if ( i >= RegionCount )
		{	break; }
		RgnPtr = this->GetRgnPtr(i, false);
		if ( NULL == RgnPtr ) { continue; }
		RegionState = RgnPtr->GetRgnCalcState();
		if ( REGION_CALC_DONE == RegionState ) { continue; }
		if ( REGION_CALC_DOING == RegionState ) { continue; }
		if ( REGION_CALC_CLEAR == RegionState ) { continue; }		

		//同步化
		CAOIRgn::LockRgn();
		RegionState = RgnPtr->GetRgnCalcState();//重新取得狀態, 因為在鎖住前可能別的執行緒又進去算了
		if ( REGION_CALC_NONE != RegionState )
		{
			CAOIRgn::UnlockRgn();
			continue;
		}
		RgnPtr->SetRgnCalcState(REGION_CALC_DOING);
		CAOIRgn::UnlockRgn();
		LastIdx = i;		
		if ( RgnPtr->CheckRgnFieldAllFrameMergeFinish() == false )
		{
			Reset = true;			
			CAOIRgn::LockRgn();
			RgnPtr->SetRgnCalcState(REGION_CALC_NONE);
			CAOIRgn::UnlockRgn();
			if ( SleepTime > 0 ) //等一下讓相機取影像
			{	::Sleep(SleepTime); }
			continue;
		}
		if ( RgnPtr->ExecRgnCalc_ProjectMark() == false )
		{
			LockThread();
			this->m_ErrorString.Format(_T("Error, Calc Region[%d] #%d Fault"), ThreadIdx+1, i+1);
			UnlockThread();
			return false;
		}		
	#ifdef _DEBUG
		str.Format(_T("Calc Region[%d]#%d/%d Finished\n"), ThreadIdx+1, i+1, RegionCount);
		TRACE(str);
	#endif//_DEBUG
		RgnPtr->SetRgnCalculated(true);
		RgnPtr->SetRgnCalcState(REGION_CALC_DONE);		
		RgnPtr->UpdateRgnParentCalcStateDone();
		InterlockedIncrement(&m_RgnFinishedCnt);	
		SetIsAnyCalcThreadWorking(true);

		MotionThreadStats = MotionCtrlPtr->GetMotionThreadState_GoStop();
		if ( THREAD_STATE_FINISH==MotionThreadStats || THREAD_STATE_IDLE==MotionThreadStats )
		{	PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_PROJECT_MAP, TRUE); }
	}	
#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecRegionCalcFn_ProjectOpenCode(size_t ThreadIdx)//執行區域計算執行緒-專案開檔條碼
{
#ifndef OFFLINE_VERSION
	CString            str;
	CString            RgnName;
	size_t             i=0;	
	size_t             LastIdx=0;
	unsigned int       RgnIndex=0;	
	bool               Reset = false;	
	bool               Finished = false;
	DWORD              SleepTime = 10;//ms
	CAOIRgn           *RgnPtr = NULL;
	CAOIRgn           *RgnPtr2 = NULL;
	CAOIBarcode       *BarcodePtr = NULL;
	AOI_OBJ_TYPE       AOIObjType=AOI_OBJ_BASIC;
	REGION_CALC_STATE  RegionState = REGION_CALC_NONE;
	const size_t       RegionCount = this->GetRgnPtrCount();
	THREAD_COMMAND_MODE  ThreadCmd = THREAD_COMMAND_TO_NONE;
	THREAD_STATE_MODE    MotionThreadStats = THREAD_STATE_NONE;
	Reset = false;
	for ( i=0; i<RegionCount+1; i++ )//避免最後一筆無法進來迴圈, 因此手動+1
	{	
		ThreadCmd = GetRegionCalcThreadCmd(ThreadIdx);
		if ( GetIsSystemReleased() == true ) { return true; }				
		if ( GetIsSystemException() == true ) { return true; }
		if ( GetIsAllCalcThreadStop() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }

		if ( true == Reset ) //由於上一次計算尚未有圖片, 退回引數號碼1步
		{			
			i = LastIdx;
			Reset = false;
		}		
		if ( i >= RegionCount )
		{	break; }
		RgnPtr = this->GetRgnPtr(i, false);
		if ( NULL == RgnPtr ) { continue; }
		RegionState = RgnPtr->GetRgnCalcState();
		if ( REGION_CALC_DONE == RegionState ) { continue; }
		if ( REGION_CALC_DOING == RegionState ) { continue; }
		if ( REGION_CALC_CLEAR == RegionState ) { continue; }		

		//同步化
		CAOIRgn::LockRgn();
		RegionState = RgnPtr->GetRgnCalcState();//重新取得狀態, 因為在鎖住前可能別的執行緒又進去算了
		if ( REGION_CALC_NONE != RegionState )
		{
			CAOIRgn::UnlockRgn();
			continue;
		}
		RgnPtr->SetRgnCalcState(REGION_CALC_DOING);
		CAOIRgn::UnlockRgn();
		LastIdx = i;		
		if ( RgnPtr->CheckRgnFieldAllFrameMergeFinish() == false )
		{
			Reset = true;			
			CAOIRgn::LockRgn();
			RgnPtr->SetRgnCalcState(REGION_CALC_NONE);
			CAOIRgn::UnlockRgn();
			if ( SleepTime > 0 ) //等一下讓相機取影像
			{	::Sleep(SleepTime); }
			continue;
		}
		/*
		if ( RgnPtr->ExecRgnCalc_ProjectMark() == false )
		{
			LockThread();
			this->m_ErrorString.Format(_T("Error, Calc Region[%d] #%d Fault"), ThreadIdx+1, i+1);
			UnlockThread();
			return false;
		}*/
		if ( RgnPtr->CheckRgnReadyToCalc() == false )
		{
			Reset = true;			
			CAOIRgn::LockRgn();
			RgnPtr->SetRgnCalcState(REGION_CALC_NONE);
			CAOIRgn::UnlockRgn();
			if ( SleepTime > 0 ) //等一下讓物件可以計算
			{	::Sleep(SleepTime); }
			continue;
		}
		
		BarcodePtr = NULL;		
		RgnIndex = RgnPtr->GetRgnIndex();
		AOIObjType = RgnPtr->GetObjType();
		switch ( AOIObjType )
		{		
		case AOI_OBJ_RGN:		RgnPtr = dynamic_cast<CAOIRgn*>(RgnPtr);	break;				
		case AOI_OBJ_BARCODE:   BarcodePtr = dynamic_cast<CAOIBarcode*>(RgnPtr);	break;		
		default:	AOIObjType = AOI_OBJ_BASIC;	break;
		}		
		if ( AOI_OBJ_BASIC == AOIObjType )
		{	
			RgnPtr->SetRgnCalculated(true);
			RgnPtr->SetRgnCalcState(REGION_CALC_DONE);
			RgnPtr->UpdateRgnParentCalcStateDone();
			InterlockedIncrement(&m_RgnFinishedCnt);
			SetIsAnyCalcThreadWorking(true);
			continue;
		}	
		if ( RgnPtr->ExtractRgnDerivedFrame(Finished) == false )
		{				
			LockThread();
			RgnPtr2 = RgnPtr->GetRgnParent();
			if ( NULL == RgnPtr2 )
			{	RgnName = RgnPtr->GetRgnDerivedName();	}
			else
			{	RgnName = RgnPtr2->GetRgnDerivedName();	}
			this->m_ErrorString.Format(_T("Error, Extract Rgn[%d]-[%s] #%d Fault"), ThreadIdx+1, RgnName, i+1);
			UnlockThread();
			return false;
		}		
	#ifdef _DEBUG
		str.Format(_T("Calc Region[%d]#%d/%d Finished\n"), ThreadIdx+1, i+1, RegionCount);
		TRACE(str);
	#endif//_DEBUG
		RgnPtr->SetRgnCalculated(true);
		RgnPtr->SetRgnCalcState(REGION_CALC_DONE);		
		RgnPtr->UpdateRgnParentCalcStateDone();
		InterlockedIncrement(&m_RgnFinishedCnt);	
		SetIsAnyCalcThreadWorking(true);

		MotionThreadStats = MotionCtrlPtr->GetMotionThreadState_GoStop();
		if ( THREAD_STATE_FINISH==MotionThreadStats || THREAD_STATE_IDLE==MotionThreadStats )
		{	PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_PROJECT_MAP, TRUE); }
	}	
#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateRegionCalcThread()//建立區域計算執行緒
{
//#ifndef OFFLINE_VERSION//Kai-20170410
	size_t  i=0;
	CString str;
	size_t  ThreadIdx = 0;	
	const size_t MaxThreadCount=MAX_THREAD_COUNT_REGION_CALC;
	if ( this->DeleteRegionCalcThread() == false ) { return false; }	

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		this->m_RegionCalcThreadCmd[ThreadIdx] = THREAD_COMMAND_TO_IDLE;	
	}

	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		RegionCalcThreadHandle[ThreadIdx] = (HANDLE)::_beginthreadex(NULL, NULL, &RegionCalcThreadFn, (void*)ThreadIdx, NULL, &RegionCalcThreadID[ThreadIdx]);	
		if ( NULL == RegionCalcThreadHandle[ThreadIdx] )
		{	
			this->m_RegionCalcThreadState[ThreadIdx] = THREAD_STATE_NONE;
			this->m_ErrorString = _T("Eorror, Create Thread Fault (RegionCalcThreadHandle == NULL)");		
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_CREATE);
			return false;
		}	
	}
	
	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;	
		str.Format(_T("Region Fill Event[%d]"), ThreadIdx+1);	
		RegionCalcThreadEvent[ThreadIdx] = JetAPI::CreateEvent(NULL, TRUE, TRUE, str);
	}

	DWORD_PTR AffinityMask = 0xFFFFFFFF;
	const DWORD CPUCoreNumber = GetComputerCPUCoreNumber();
	if ( CPUCoreNumber > 6 )
	{	AffinityMask ^= 0x0F;	}//保留6個
	else
	{	AffinityMask ^= 0x03;	}//保留2個
	AffinityMask = GetThreadAffinityMask();
	for ( i=0; i<MaxThreadCount; i++ )
	{
		ThreadIdx = i;
		if ( NULL != RegionCalcThreadHandle[ThreadIdx] )
		{	
			::SetThreadPriority(RegionCalcThreadHandle[ThreadIdx], THREAD_PRIORITY_LOWEST);
			::SetThreadAffinityMask(RegionCalcThreadHandle[ThreadIdx], AffinityMask);			
		}
	}
//#endif//OFFLINE_VERSION//Kai-20170410
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteRegionCalcThread()//刪除區域計算執行緒
{
//#ifndef OFFLINE_VERSION//Kai-20170410
	size_t i = 0;
	size_t ThreadIdx = 0;
	DWORD WaitTime = 1000;//1 sec
	for ( i=0; i<MAX_THREAD_COUNT_REGION_CALC; i++ )
	{
		ThreadIdx = i;
		if ( NULL == RegionCalcThreadHandle[ThreadIdx] ) { continue; }		
		this->SetRegionCalcThreadCmd(ThreadIdx, THREAD_COMMAND_TO_EXIT);	
		DWORD Res = ::WaitForSingleObject(RegionCalcThreadHandle[ThreadIdx], WaitTime);
		::CloseHandle(RegionCalcThreadHandle[ThreadIdx]); 
		RegionCalcThreadHandle[ThreadIdx] = NULL;
		if ( NULL != RegionCalcThreadEvent[ThreadIdx] )
		{	
			::CloseHandle(RegionCalcThreadEvent[ThreadIdx]); 
			RegionCalcThreadEvent[ThreadIdx]=NULL; 
		}	
	}
//#endif//OFFLINE_VERSION//Kai-20170410
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartRegionCalcThread(bool WaitOn)//開始區域計算執行緒
{
//#ifndef OFFLINE_VERSION//Kai-20170410
	size_t i=0, j=0;
	size_t ThreadIdx=0;		
	for ( i=0; i<MAX_THREAD_COUNT_REGION_CALC; i++ )
	{		
		ThreadIdx = i;
		if ( NULL == RegionCalcThreadHandle[ThreadIdx] ) { return true; }
		if ( NULL != RegionCalcThreadEvent[ThreadIdx] )
		{	::ResetEvent(RegionCalcThreadEvent[ThreadIdx]); }
		if ( THREAD_STATE_FINISH == m_RegionCalcThreadState[ThreadIdx] )
		{  SetRegionCalcThreadState(ThreadIdx, THREAD_STATE_IDLE); }
		SetRegionCalcThreadCmd(ThreadIdx, THREAD_COMMAND_TO_RUN);
	}
	if ( false == WaitOn ) 
	{	return true; }
	if ( WaitForRegionCalcThreadStart() == false )
	{	return false; }	
//#endif//OFFLINE_VERSION//Kai-20170410
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartRegionCalcThread_Full(bool WaitOn)//開始區域計算執行緒-全部執行緒
{
//#ifndef OFFLINE_VERSION//Kai-20170410
	size_t i=0, j=0;
	size_t ThreadIdx=0;	
	bool   IsSystemException = false;
	const size_t ThreadStart = CalcRegionCalcPartialThreadCount();	
	const size_t ThreadEnd   = GetRegionCalcThreadUsingCount();
	if ( ThreadEnd <= ThreadStart ) { return true; }

	DWORD_PTR AffinityMask = m_ThreadAffinityMask;
	if ( 0 == GetSystemParameter().m_CpuMaxCountUsed )	
	{
		AffinityMask = (DWORD_PTR)(-1);
		if ( (DWORD_PTR)(-1) != m_ThreadAffinityMask)
		{	AffinityMask ^= 0x01;	}//需要保留1個核心處理其餘事件
	}
	//m_ThreadAffinityMask = AffinityMask;
	for ( i=ThreadStart; i<ThreadEnd; i++ )
	{		
		ThreadIdx = i;
		if ( NULL == RegionCalcThreadHandle[ThreadIdx] ) { return true; }		
		if ( NULL != RegionCalcThreadEvent[ThreadIdx] )
		{	::ResetEvent(RegionCalcThreadEvent[ThreadIdx]); }
		SetThreadAffinityMask(RegionCalcThreadHandle[ThreadIdx], AffinityMask);
		IsSystemException = GetIsSystemException();
		if ( true == IsSystemException ) 
		{	return true;	}
		if ( THREAD_STATE_FINISH == m_RegionCalcThreadState[ThreadIdx] )
		{  SetRegionCalcThreadState(ThreadIdx, THREAD_STATE_IDLE); }
		SetRegionCalcThreadCmd(ThreadIdx, THREAD_COMMAND_TO_RUN);
	}
	if ( false == WaitOn )
	{	return true;	}
	if ( WaitForRegionCalcThreadStart_Full() == false )
	{	return false; }	
//#endif//OFFLINE_VERSION//Kai-20170410
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartRegionCalcThread_Partial(bool WaitOn)//開始區域計算執行緒-部分執行緒
{
//#ifndef OFFLINE_VERSION//Kai-20170410
	DWORD Res = 0;
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	const size_t ThreadCount = CalcRegionCalcPartialThreadCount();	
	for ( i=0; i<ThreadCount; i++ )
	{		
		ThreadIdx = i;
		if ( NULL == RegionCalcThreadHandle[ThreadIdx] ) { return true; }
		if ( NULL != RegionCalcThreadEvent[ThreadIdx] )
		{	::ResetEvent(RegionCalcThreadEvent[ThreadIdx]); }
		if ( THREAD_STATE_FINISH == m_RegionCalcThreadState[ThreadIdx] )
		{  SetRegionCalcThreadState(ThreadIdx, THREAD_STATE_IDLE); }
		SetRegionCalcThreadCmd(ThreadIdx, THREAD_COMMAND_TO_RUN);
	}
	if ( false == WaitOn )
	{	return true; }
	if ( WaitForRegionCalcThreadStart_Partial() == false )
	{	return false; }	
//#endif//OFFLINE_VERSION//Kai-20170410
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CAOIDataCollect::CalcRegionCalcPartialThreadCount() const//計算區域計算部分執行緒數量
{
	size_t ThreadCount = GetRegionCalcThreadMaxCount();
	const size_t CPUCoreNumber = GetComputerCPUCoreNumber();	
	const size_t GrabIdleCount = GetSystemParameter().m_MTCount_GrabIdle;	
	const size_t LeastCPUCoreNumberFree = MIN(CPUCoreNumber, GrabIdleCount);//至少空出N個自由的CPU	
	size_t CPUCoreNumberForUsing = CPUCoreNumber-LeastCPUCoreNumberFree;	
	if ( ThreadCount > CPUCoreNumberForUsing ) { ThreadCount = CPUCoreNumberForUsing; }
	if ( ThreadCount == 0 ) { ThreadCount = 1; }
	return ThreadCount;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForRegionCalcThreadIdle()//等待區域計算執行緒完成
{
//#ifndef OFFLINE_VERSION//Kai-20170410
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;		
	const size_t RegCount = GetRgnPtrCount();
	const size_t MaxCounts = (RegCount*200)+100;	
	const size_t ThreadCount   = GetRegionCalcThreadUsingCount();

	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;		
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			//if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_STATE_NONE == m_RegionCalcThreadState[ThreadIdx] )
			{	break; }		
			if ( THREAD_STATE_IDLE == m_RegionCalcThreadState[ThreadIdx] )
			{	break; }
			if ( SleepTime > 0 )
			{	::Sleep(SleepTime); }
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_RegionCalcThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_RegionCalcThreadState[ThreadIdx]);
			m_ErrorString.Format(_T("Error, WaitForRegionCalcThreadIdle[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE);
			return false;
		}	
		ThreadIdx = j;
	}
//#endif//OFFLINE_VERSION//Kai-20170410
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForRegionCalcThreadStop()//等待區域計算執行緒停止
{
//#ifndef OFFLINE_VERSION//Kai-20170410
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;
	const size_t RegCount = GetRgnPtrCount();
	const size_t MaxCounts = (RegCount*400)+100;	
	const size_t ThreadCount   = GetRegionCalcThreadUsingCount();

	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;		
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			//if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_STATE_RUNNING != m_RegionCalcThreadState[ThreadIdx] )
			{	break; }
			if ( SleepTime > 0 )
			{	::Sleep(SleepTime); }
		}
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_RegionCalcThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_RegionCalcThreadState[ThreadIdx]);
			m_ErrorString.Format(_T("Error, WaitForRegionCalcThreadStop[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP);
			return false;
		}	
		ThreadIdx = j;
	}
//#endif//OFFLINE_VERSION//Kai-20170410
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForRegionCalcThreadStart()//等待區域計算執行緒開始
{
//#ifndef OFFLINE_VERSION//Kai-20170410		
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	const size_t MaxCount = 500;
	const size_t SleepTime = 10;	
	for ( j=0; j<MAX_THREAD_COUNT_REGION_CALC; j++ )
	{
		ThreadIdx = j;
		for ( i=0; i<MaxCount; i++ )
		{
			if ( THREAD_COMMAND_TO_RUN != m_RegionCalcThreadCmd[ThreadIdx] )//切入下一個階段
			{	break; }
			if ( THREAD_STATE_IDLE != m_RegionCalcThreadState[ThreadIdx] )
			{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			CString CmdText = GetThreadCommandModeText(m_RegionCalcThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_RegionCalcThreadState[ThreadIdx]);
			m_ErrorString.Format(_T("Error, WaitForRegionCalcThreadStart[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_START);	
			return false;
		}
	}	
//#endif//OFFLINE_VERSION//Kai-20170410
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForRegionCalcThreadStart_Full()//等待區域計算執行緒開始-全部執行緒
{
//#ifndef OFFLINE_VERSION//Kai-20170410	
	size_t i=0, j=0;
	size_t ThreadIdx=0;	
	const size_t MaxCount = 500;
	const size_t SleepTime = 10;		
	const size_t ThreadStart = CalcRegionCalcPartialThreadCount();	
	const size_t ThreadEnd   = GetRegionCalcThreadUsingCount();
	if ( ThreadEnd <= ThreadStart ) { return true; }
	for ( j=ThreadStart; j<ThreadEnd; j++ )
	{
		ThreadIdx = j;
		for ( i=0; i<MaxCount; i++ )
		{
			if ( THREAD_COMMAND_TO_RUN != m_RegionCalcThreadCmd[ThreadIdx] )//切入下一個階段
			{	break; }
			if ( THREAD_STATE_IDLE != m_RegionCalcThreadState[ThreadIdx] )
			{	break; }
			::Sleep(SleepTime);
		}		
		if ( i == MaxCount )
		{	
			CString CmdText = GetThreadCommandModeText(m_RegionCalcThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_RegionCalcThreadState[ThreadIdx]);
			m_ErrorString.Format(_T("Error, WaitForRegionCalcThreadStart_Full[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			return false;
		}
	}		
//#endif//OFFLINE_VERSION//Kai-20170410
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForRegionCalcThreadStart_Partial()//等待區域計算執行緒開始-部分執行緒
{
//#ifndef OFFLINE_VERSION//Kai-20170410		
	DWORD Res = 0;
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	const size_t MaxCount = 500;
	const size_t SleepTime = 10;
	const size_t ThreadCount = CalcRegionCalcPartialThreadCount();	
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		for ( i=0; i<MaxCount; i++ )
		{
			if ( THREAD_COMMAND_TO_RUN != m_RegionCalcThreadCmd[ThreadIdx] )//切入下一個階段
			{	break; }
			if ( THREAD_STATE_IDLE != m_RegionCalcThreadState[ThreadIdx] )
			{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			CString CmdText = GetThreadCommandModeText(m_RegionCalcThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_RegionCalcThreadState[ThreadIdx]);
			m_ErrorString.Format(_T("Error, WaitForRegionCalcThreadStart_Partial[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			return false;
		}
	}	
//#endif//OFFLINE_VERSION//Kai-20170410
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForRegionCalcThreadFinish()//等待區域計算執行緒結束
{
//#ifndef OFFLINE_VERSION//Kai-20170410
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;		
	const size_t RegCount = GetRgnPtrCount();
	const size_t MaxCounts = (RegCount*200)+100;	
	const size_t ThreadCount   = GetRegionCalcThreadUsingCount();
	DWORD_PTR AffinityMask = m_ThreadAffinityMask_Backup;	
	SaveDebugMessage(_T("WaitForRegionCalcThreadFinish-Start"));
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		if ( RegionCalcThreadEvent[ThreadIdx] == NULL ) { continue; }
		size_t CheckWorkingCount=0;		
		ResetIsAnyCalcThreadWorking();
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_COMMAND_TO_EXIT == m_RegionCalcThreadCmd[ThreadIdx] ) { return true; }		
			if ( THREAD_COMMAND_TO_IDLE == m_RegionCalcThreadCmd[ThreadIdx] )
			{	break; }

			Res = ::WaitForSingleObject(RegionCalcThreadEvent[ThreadIdx], SleepTime);
			if ( Res != WAIT_TIMEOUT )
			{
				if ( THREAD_STATE_RUNNING == m_RegionCalcThreadState[ThreadIdx] )
				{
					if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
					continue; 
				}
				break; 
			}

			if ( GetIsAnyCalcThreadWorking() == true )
			{	
				CheckWorkingCount = 0; 
				ResetIsAnyCalcThreadWorking();
			}
			else
			{
				CheckWorkingCount ++;
				if ( CheckWorkingCount > THREAD_CHECK_NO_WORKING_MAX_COUNT )
				{	
					CString CmdText = GetThreadCommandModeText(m_RegionCalcThreadCmd[ThreadIdx]);
					CString StateText = GetThreadStateModeText(m_RegionCalcThreadState[ThreadIdx]);
					SaveDebugMessage(_T("WaitForRegionCalcThreadFinish-Fault (No-Working)"));
					m_ThreadAffinityMask = AffinityMask;
					m_ErrorString.Format(_T("Error, WaitForRegionCalcThreadFinish[%d, Cmd=%s, State=%s] too long (No-Working)"), ThreadIdx+1, CmdText, StateText);
					SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
					return false;
				}
			}	
		}
		SetThreadAffinityMask(RegionCalcThreadHandle[ThreadIdx], AffinityMask);
		if ( i == MaxCounts )
		{
			CString CmdText = GetThreadCommandModeText(m_RegionCalcThreadCmd[ThreadIdx]);
			CString StateText = GetThreadStateModeText(m_RegionCalcThreadState[ThreadIdx]);
			SaveDebugMessage(_T("WaitForRegionCalcThreadFinish-Fault"));
			m_ThreadAffinityMask = AffinityMask;
			m_ErrorString.Format(_T("Error, WaitForRegionCalcThreadFinish[%d, Cmd=%s, State=%s] too long"), ThreadIdx+1, CmdText, StateText);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
			return false;
		}	
		ThreadIdx = j;
	}
	m_ThreadAffinityMask = AffinityMask;	
	SaveDebugMessage(_T("WaitForRegionCalcThreadFinish-End"));
//#endif//OFFLINE_VERSION//Kai-20170410
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckRegionCalcThreadStateFinish()//確認區域計算執行緒狀態
{
	size_t i=0;
	for ( i=0; i<MAX_THREAD_COUNT_REGION_CALC; i++ )
	{
		if ( NULL == RegionCalcThreadHandle[i] ) { continue; }
		if ( THREAD_COMMAND_TO_NONE == m_RegionCalcThreadCmd[i] ) { continue; }
		if ( THREAD_COMMAND_TO_EXIT == m_RegionCalcThreadCmd[i] ) { continue; }
		if ( THREAD_COMMAND_TO_IDLE == m_RegionCalcThreadCmd[i] ) { continue; }
		if ( m_RegionCalcThreadState[i] != THREAD_STATE_FINISH )
		{
			this->m_ErrorString.Format(_T("Error, m_RegionCalcThreadState#%d is Exception"), i+1);
			return false;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetRegionCalcThreadState(size_t idx, THREAD_STATE_MODE State)//設定區域計算執行緒狀態
{
	if ( idx >= MAX_THREAD_COUNT_REGION_CALC ) { return ; }
	if ( m_RegionCalcThreadState[idx] == State ) { return; }
	LockThreadRegionCalc();
	m_RegionCalcThreadState[idx] = State;
	UnlockThreadRegionCalc();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE CAOIDataCollect::GetRegionCalcThreadState(size_t idx)//取得區域計算執行緒狀態
{
	if ( idx >= MAX_THREAD_COUNT_REGION_CALC ) { return THREAD_STATE_NONE; }
	return m_RegionCalcThreadState[idx];
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetRegionCalcThreadCmd(size_t idx, THREAD_COMMAND_MODE Cmd)//設定區域計算執行緒命令
{
	if ( idx >= MAX_THREAD_COUNT_REGION_CALC ) { return ; }
	if ( m_RegionCalcThreadCmd[idx] == Cmd ) { return; }
	LockThreadRegionCalc();
	m_RegionCalcThreadCmd[idx] = Cmd;
	UnlockThreadRegionCalc();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CAOIDataCollect::GetRegionCalcThreadCmd(size_t idx)//取得區域計算執行緒命令
{
	if ( idx >= MAX_THREAD_COUNT_REGION_CALC ) { return THREAD_COMMAND_TO_NONE; }
	return m_RegionCalcThreadCmd[idx];	
}
//-------------------------------------------------------------------------------------//
size_t CAOIDataCollect::GetRegionCalcThreadMaxCount() const//取得區域計算設定數量 
{	
	return MIN(MAX_THREAD_COUNT_REGION_CALC, m_SystemParameter.m_MTCount_RegionCalc);	
}
//-------------------------------------------------------------------------------------//
size_t CAOIDataCollect::GetRegionCalcThreadUsingCount() const//取得區域計算實際使用數量 
{
	//return m_SystemParameter.m_MTCount_RegionCalc;
	size_t MaxUseCount=0;
	size_t CPUCoreNumber = GetComputerCPUCoreNumber();
	size_t MaxRegionCalc = GetRegionCalcThreadMaxCount();
	MaxUseCount=MIN(CPUCoreNumber, MaxRegionCalc);//小於系統核心可以讓UI操作更新起來
	MaxUseCount=MAX(MaxUseCount, 1);
	return MaxUseCount;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecTaskLaneBypass()//執行軌道流片
{
	bool bAutoReset = true;
	bool bChkStartLight = true;		
	PCB_OUT_MODE PcbOutMode=GetSystemParameter().m_DefaultPCBOutMode;	

	ResetSystemException();
	if ( TurnOnConveyerSensorPower() == false )
	{	return false; }
	if ( CheckSystemReady(bChkStartLight, bAutoReset) == false )
	{	return false; }

	if ( MoveCameraToBeforePCBInPosition(true) == false )
	{	return false; }	

	SetOnlineTaskCancel(false);
	SetTaskMode(TASK_LANE_BYPASS);
	SetIsOnlineCheckSystemReady(true);
	ResetSwitchMultiLine_NextLaneID();
	UpdateLaneWorkMode(TASK_LANE_BYPASS);
	SetOnlineTaskState(TASK_STATE_RUNNING);
	SetLanePCBOutMode(LANE_ID_A, PcbOutMode);
	SetLanePCBOutMode(LANE_ID_B, PcbOutMode);
	AOIExceptionCodeCtrl.ResetAOIExceptionCode();
	if ( StartOnlineInspectionThread(true) == false )
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecInspectionProject()//執行檢測專案-調機測試
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{	return false; }

	bool bAutoReset = false;
	bool bChkStartLight = true;
	ResetSystemException();
	SetOnlineOpenProjectFinish(false);	
	if ( CheckSystemReady(bChkStartLight, bAutoReset) == false )
	{	return false; }

	//Project Setting	
	PlcCtrlPtr->SetPLCPollingLaneAdjustSensor(false);	
	ProjectPtr->SetProjectAllObjToNeedToCalculateRgn(true);

	SaveCameraTemperatureLog(PRIMARY_CAMERA_ID);
	SetTaskMode(TASK_INSPECT_PROJECT);

	if ( WaitForThreadSequenceThreadStop() == false )
	{	return false;	}

	if ( StartThreadSequenceThread(true) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecInspectionProjectMark()//執行檢測專案特徵
{
	CString      str;	
	bool bAutoReset = false;
	bool bChkStartLight = true;
	//SetIsIgnoreAutoRetry(false);
	//SetIsSystemException(false);
	//SetIsGetSystemExceptionMsg(false);
	if ( TurnOnConveyerSensorPower() == false )
	{	return false; }
	if ( CheckSystemReady(bChkStartLight, bAutoReset) == false )
	{	return false; }	
	
	str = _T("Inspect Project Mark Start");	
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	str += _T("\n");	
	TRACE(str);

	if ( CreateProjectMarkObj() == false )
	{
		ClearMarkObjList();
		SetIsIgnoreAutoRetry(true);
		return false; 
	}

	m_RgnPtrList = m_MarkRgnPtrList;
	m_FovPtrList = m_MarkFovPtrList;
	m_SlicePtrList = m_MarkSlicePtrList;
	m_FieldPtrList = m_MarkFieldPtrList;
	m_FramePtrList = m_MarkFramePtrList;
	
	const size_t FieldCount = m_FieldPtrList.size();
	if ( FieldCount > 0 ) 
	{
		CAOIField *FieldPtr = m_FieldPtrList[0];
		double StagePosX = FieldPtr->GetFieldStagePosX();
		double StagePosY = FieldPtr->GetFieldStagePosY();
		MotionCtrlPtr->XYMoveTo(StagePosX, StagePosY);
	}

	m_RgnFinishedCnt = 0;
	m_SliceFinishedCnt = 0;	
	m_FrameFinishedCnt = 0;
	m_FieldCalcFinishedCnt = 0;
	m_FieldMergeFinishedCnt = 0;

	//確認列表狀態	
	if ( CheckObjListInitialized() == false )
	{	return false;	}

	//是否儲存Offline
	SetSaveOfflineFiles(false);
	SetSaveOfflineImageFiles(false);	

	//先重設定所有燈原狀態
	//if ( this->SetupAllLightSetting() == false )
	//{	return false;	}

#ifndef LIGHT_CTRL_DISABLE
	std::vector<TSliceParam> SliceParamList = m_MarkSliceParamList;	
	if ( CameraCtrl.BatchGrabPrepare2(SliceParamList, true) == false )
	{
		this->m_ErrorString = CameraCtrl.GetErrorString();			
		return false;
	}
#endif//LIGHT_CTRL_DISABLE

	SetInspectionDrawing(true);
	SetThreadGrabMode(THREAD_GRAB_PROJECT_MARK);	

	//覆歸循環Buffer的資料
	//CameraCtrl.ClearAllCameraCount();
	CameraCtrl.ResetAllCameraRingBuffer();

	//啟動計算執行緒
	const bool bWaitThread=false;
	StartSliceFillThread(bWaitThread);//啟動填圖的執行緒	
	StartFrameMergeThread(bWaitThread);//啟動影像的執行緒
	StartFieldMergeThread(bWaitThread);//啟動區域的執行緒
	StartFieldCalcThread(bWaitThread);//啟動區域的執行緒
	//StartRegionCalcThread(bWaitThread);
	StartRegionCalcThread_Partial(bWaitThread);
	if ( false == bWaitThread )	
	{	
		WaitForSliceFillThreadStart();
		WaitForFrameMergeThreadStart();
		WaitForFieldMergeThreadStart();
		WaitForFieldCalcThreadStart();
		//WaitForRegionCalcThreadStart();
		WaitForRegionCalcThreadStart_Partial();
	}

	const bool AutoReleaseFieldFrame = GetAutoReleaseFieldFrame();
	if ( true == AutoReleaseFieldFrame )
	{	this->StartFieldFrameReleaseThread(true);	}
	//StopFieldFrameReleaseThread(true);

	//啟動運動系統的執行緒
	if ( MotionCtrlPtr->StartMotionThreadStats_GoStop(true) == false )
	{
		this->m_ErrorString = MotionCtrlPtr->GetErrorString();		
		return false;
	}

	const size_t GrabFovCount = CAOIDataCollect::GetFovPtrCount();
	if ( MotionCtrlPtr->WaitMotionThreadDone_GoStop(GrabFovCount) == false )
	{
		m_ErrorString = MotionCtrlPtr->GetErrorString();
		return false;//20190506
	}
	if ( WaitForRegionCalcThreadFinish() == false )
	{	return false; }			
	if ( WaitForFieldCalcThreadFinish() == false )
	{	return false; }

	SetSaveOfflineFiles(false);
	SetSaveOfflineImageFiles(false);	
	SetInspectionDrawing(false);
	ClearMarkObjList();

	if ( SelectProjectMarkBestOne() == false )
	{
		SetIsIgnoreAutoRetry(true);
		return false; 
	}

	str = _T("Inspect Project Mark End");
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	str += _T("\n");	
	TRACE(str);		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecInspectionProjectByTurn(LANE_ID eLaneID)//執行檢測專案輪流切換
{
	size_t  i=0;
	double  TestCount=0;
	double  TestCountLess=DBL_MAX;
	CString strLaneID;
	const LANE_ID LaneID = eLaneID;	
	CAOIProject *ProjectPtr = NULL;
	CAOIProject *ProjectPtrInTurn = NULL;
	CAOIProject *ProjectPtrBefore = NULL;	
	std::vector<CAOIProject*> LaneProjectList;

	ProjectPtrBefore = GetActiveProject();	
	SetMultiProjectTestResetDone(true, LaneID);
	strLaneID = AOIDataDefine.GetLaneIDText(LaneID);
	if ( BuildLaneProjectList(LaneID, LaneProjectList) == false )
	{	return false; }	
	const size_t LaneProjectCount = LaneProjectList.size();
	const size_t LaneProjectTestCount = GetLaneProjectTestCountForTurn(LaneID);
	const size_t LaneProjectMod = LaneProjectTestCount%LaneProjectCount;//使用餘數
	ProjectPtrInTurn = LaneProjectList[LaneProjectMod];
	if ( NULL == ProjectPtrInTurn ) 
	{
		m_ErrorString.Format(_T("Error, No In Turn Project in %s"), strLaneID);		
		return false;
	}
	if ( ProjectPtrInTurn == ProjectPtrBefore )
	{	
		SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH_MARK, NULL);		
		return true; 
	}

	//切換專案
	const unsigned int ProjectIndex = ProjectPtrInTurn->GetProjectIndex();
	SetActiveProjectIndex(ProjectIndex);
	SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);
	return true;
	
	//切換手持條碼機內容
	/*
	TB_SYSTEM_ID SysIDInTurn=ProjectPtrInTurn->GetProjectSystemID();
	TB_SYSTEM_ID SysIDBefore=ProjectPtrInTurn->GetProjectSystemID();
	if ( NULL != ProjectPtrBefore )
	{	SysIDBefore=ProjectPtrBefore->GetProjectSystemID();	}
	if ( SysIDBefore != SysIDInTurn )
	{
		CBarcode_Handheld* BarcodeHandHeldPtr = GetBarcodeHandHeldPtr(LaneID, SysIDBefore);
		if ( NULL != BarcodeHandHeldPtr )
		{
			CBarcode_Handheld BarcodeHandHeld = *BarcodeHandHeldPtr;
			if ( ProjectPtrInTurn->ModifyProjectBarcodeHandHeldCode(BarcodeHandHeld) == false )
			{
				m_ErrorString = ProjectPtrInTurn->GetErrorString();
				return false;			
			}			
			SetBarcodeHandHeld(LaneID, SysIDInTurn, BarcodeHandHeld);
		}
	}
	*/
	return true;	
}
//-------------------------------------------------------------------------------------//
CAOIProject* CAOIDataCollect::GetNextProjectByTurnOneCycle(LANE_ID eLaneID)//執行檢測專案輪流切換,一次檢測內-取得下一個檢測專案
{
	size_t  i=0, idx=0;
	const LANE_ID LaneID = eLaneID;	
	CAOIProject *ProjectPtr = NULL;	
	std::vector<CAOIProject*> LaneProjectList;
	if ( BuildLaneProjectList(LaneID, LaneProjectList) == false )
	{	return NULL; }	
	const size_t LaneProjectCount = LaneProjectList.size();		
	MULTI_PROJECT_TEST_ORDER_MODE MultiProjectTestOrderMode=GetMultiProjectTestOrderMode();
	for ( i=0; i<LaneProjectCount; i++ )
	{
		if ( MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_A_B == MultiProjectTestOrderMode )
		{	idx = i;	}
		else if ( MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_B_A == MultiProjectTestOrderMode )
		{	idx = LaneProjectCount-i-1;	}

		ProjectPtr = LaneProjectList[idx];
		if ( NULL == ProjectPtr ) { continue; }		
		if ( ProjectPtr->GetProjectHasTestInOneCycle(LaneID) == true ) { continue; }
		return ProjectPtr;		
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecInspectionProjectByTurnOneCycleReset(LANE_ID eLaneID)//執行檢測專案輪流切換, 一次檢測內
{
	size_t  i=0;	
	CString strLaneID;
	const LANE_ID LaneID = eLaneID;		
	CAOIProject *ProjectPtr = NULL;	
	CAOIProject *ProjectPtrBefore = NULL;	
	CAOIProject *ProjectPtrInTurn = NULL;	
	std::vector<CAOIProject*> LaneProjectList;

	ProjectPtrBefore = GetActiveProject();		
	SetMultiProjectTestResetDone(true, LaneID);
	if ( BuildLaneProjectList(LaneID, LaneProjectList) == false )
	{	return false; }	
	const size_t LaneProjectCount = LaneProjectList.size();
	if ( 0 == LaneProjectCount ) { return true; }
	for ( i=0; i<LaneProjectCount; i++ )
	{
		ProjectPtr = LaneProjectList[i];
		if ( NULL == ProjectPtr ) { continue; }
		ProjectPtr->SetProjectHasTestInOneCycle(LaneID, false);
	}

	size_t idx=0;
	MULTI_PROJECT_TEST_ORDER_MODE MultiProjectTestOrderMode=GetMultiProjectTestOrderMode();
	if ( MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_A_B == MultiProjectTestOrderMode )
	{	idx = 0;	}
	else if ( MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_B_A == MultiProjectTestOrderMode )
	{	idx = LaneProjectCount-1;	}
	ProjectPtrInTurn = LaneProjectList[idx];
	if ( NULL == ProjectPtrInTurn ) 
	{	return true; }
	if ( ProjectPtrBefore == ProjectPtrInTurn )
	{	return true; }

	const unsigned int ProjectIndex = ProjectPtrInTurn->GetProjectIndex();
	SetActiveProjectIndex(ProjectIndex);
	SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecInspectionProjectByTurnOneCycleNext(LANE_ID eLaneID, bool &bNext)//執行檢測專案輪流切換, 一次檢測內
{
	size_t  i=0;		
	const LANE_ID LaneID = eLaneID;		
	CAOIProject *ProjectPtrInTurn = NULL;
	CAOIProject *ProjectPtrBefore = NULL;		

	bNext = false;
	ProjectPtrBefore = GetActiveProject();	
	ProjectPtrBefore->SetProjectHasTestInOneCycle(LaneID, true);	
	ProjectPtrInTurn = GetNextProjectByTurnOneCycle(LaneID);
	if ( NULL == ProjectPtrInTurn )
	{	return true;	}

	//切換專案	
	const unsigned int ProjectIndex = ProjectPtrInTurn->GetProjectIndex();
	SetActiveProjectIndex(ProjectIndex);
	MoveCameraToProjectFirstPos(ProjectPtrInTurn, LaneID, DISTRICT_ID_A, false);
	SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH, NULL);

	bNext = true;
	ONLINE_STATE_MODE OnlineStateMode=ONLINE_STATE_INSPECTION_START;
	ONLINE_STATE_MODE OnlineStateModeNext=OnlineStateMode;
	SetIsNeedGrabFiducial(true, LaneID);
	SetOnlineStateMode(OnlineStateMode);//由於其他執行緒會執行, 因此需要最後更新
	SetOnlineStateMode_GUI(OnlineStateMode);
	SetOnlineStateMode_Next(OnlineStateModeNext);//由於其他執行緒會執行, 因此需要最後更新		
	SetOnlineStateMode_GUI_Lane(OnlineStateMode, LaneID);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecInspectionProjectTuning()//執行檢測專案
{
	if ( HideGlobalWndForInspection() == false )
	{	return false; }
	if ( CheckAIServerIsReady() == false )
	{	return false;	}
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{	return false; }
	if ( ProjectPtr->CheckProjectComponentBarcodeReadyToInspection() == false )
	{
		m_ErrorString = ProjectPtr->GetErrorString();
		return false;
	}

	bool bAutoReset = true;
	bool bChkStartLight = true;
	const LANE_ID  LaneID = GetActiveLaneID();
	const bool OfflineMode = GetOfflineMode();
	ResetSystemException();
	if ( false == OfflineMode )
	{
		if ( TurnOnConveyerSensorPower(100) == false )
		{	return false; }

		if ( CheckSystemReady(bChkStartLight, bAutoReset) == false )
		{	return false; }

		if ( CheckPCBInside(LaneID) == false )
		{	return false; }
	}
	
	//Project Setting		
	ProjectPtr->SetProjectTickCountStamp(0);
	ProjectPtr->RestoreProjectAllObjNeedToCalculate();
	if ( ProjectPtr->GetProjectNeedGrabFdForTuning() == true )
	{	SetIsNeedGrabFiducial(true); }
	PlcCtrlPtr->SetPLCPollingLaneAdjustSensor(false);
	if ( StartThreadSequenceThread(true) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecInspectionProjectOnline()//執行檢測專案-線上檢測
{
	if ( CheckAIServerIsReady() == false )
	{	return false;	}
	CAOIProject *ProjectPtr = GetActiveProject();
	const bool IsOnAutoRetry = GetIsOnAutoRetry();
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }

	if ( false == IsOnAutoRetry )
	{	
		if ( ExecInspectionProjectOnline_Prep() == false )
		{	return false; }
	}

	if ( StartLoadRepairFileThread(true) == false ) 
	{	return false; }
	
	LANE_ID LaneID = GetActiveLaneID();		
	const TASK_MODE TaskMode = TASK_INSPECT_PROJECT;
	const int LastStationLineMode = GetLastStationLineMode();	
	ONLINE_STATE_MODE OnlineStateMode = GetOnlineStateMode_Lane(LaneID);
	//ONLINE_STATE_MODE OnlineStateMode = ProjectPtr->GetProjectOnlineStateMode();
	ONLINE_STATE_MODE OnlineStateModeNew = OnlineStateMode;//避免手動進出板造成狀態不符合
	if ( false == IsOnAutoRetry )//避免手動進出板造成狀態不符合
	{	OnlineStateModeNew = ModifyFirstOnlineStateMode(LaneID, OnlineStateMode); }
	ProjectPtr->SetProjectOnlineStateMode(OnlineStateModeNew);
	switch ( OnlineStateModeNew )
	{
	case ONLINE_STATE_WAIT_FOR_LAST_STATION:
		if ( LAST_STATION_LINE_MODE_2 == LastStationLineMode )//2線式
		{	PlcCtrlPtr->WriteSignalToLast(LaneID, true); }		
		break;
	case ONLINE_STATE_WAIT_FOR_NEXT_STATION:
		PlcCtrlPtr->WriteSignalToNext(LaneID, true);
		break;
	case ONLINE_STATE_WAIT_FOR_REPAIR_VERIFY:
		if ( PlcCtrlPtr->GetConveryerSensorPCBStop(LaneID) == false ) 
		{
			ClearRepairDateTimeToCheck(LaneID);
			if ( LAST_STATION_LINE_MODE_2 == LastStationLineMode )//2線式
			{	PlcCtrlPtr->WriteSignalToLast(LaneID, true); }					
			OnlineStateModeNew = ONLINE_STATE_WAIT_FOR_LAST_STATION;			
			SetOnlineStateMode_Lane(OnlineStateModeNew, LaneID);
			ProjectPtr->SetProjectOnlineStateMode(OnlineStateModeNew);	
			PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
		}
		break;
	}
	SetTaskMode(TaskMode);	
	SetOnlineTaskCancel(false);
	SetOnlineTaskState(TASK_STATE_RUNNING);	
	LANE_STATE_MODE TaskLaneStateMode_LA = GetOnlineLaneTaskState(LANE_ID_A);
	LANE_STATE_MODE TaskLaneStateMode_LB = GetOnlineLaneTaskState(LANE_ID_B);
	if (TaskLaneStateMode_LA == LANE_STATE_NONE && TaskLaneStateMode_LB == LANE_STATE_NONE) {
		LANE_WORK_MODE LaneWorkMode_LA = GetLaneWorkMode_LA();
		LANE_WORK_MODE LaneWorkMode_LB = GetLaneWorkMode_LB();
		TaskLaneStateMode_LA = (LANE_WORK_BYPASS == LaneWorkMode_LA) ? LANE_STATE_BYPASS : LANE_STATE_RUNNING;
		TaskLaneStateMode_LB = (LANE_WORK_BYPASS == LaneWorkMode_LB) ? LANE_STATE_BYPASS : LANE_STATE_RUNNING;
		SetOnlineLaneTaskState(LANE_ID_A, TaskLaneStateMode_LA);
		SetOnlineLaneTaskState(LANE_ID_B, TaskLaneStateMode_LB);
	}
	SetOnlineStateMode(OnlineStateModeNew);
	AOIExceptionCodeCtrl.ResetAOIExceptionCode();
	//SetOnlineStateMode_Lane(OnlineStateModeNew, LaneID);	
	if ( StartOnlineInspectionThread(true) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecInspectionProjectOnline_Prep()//執行檢測專案-線上檢測-預備
{
	if ( HideGlobalWndForInspection() == false )
	{	return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( CheckProjectPtr(ProjectPtr) == false ) { return false; }

	size_t  i=0;
	CString str;
	bool bAutoReset = true;
	bool bChkStartLight = true;
	const size_t ProjectCount=GetProjectPtrCount();
	const TSystemParameter &SysParam = GetSystemParameter();
	MULTI_LANE_MODE MultiLane = CheckMultiLaneMode();
	const TASK_MODE TaskMode = TASK_INSPECT_PROJECT;
	ResetSystemException();
	ResetSwitchMultiLine_NextLaneID();
	if ( TurnOnConveyerSensorPower() == false )
	{	return false; }
	if ( CheckSystemReady(bChkStartLight, bAutoReset) == false )
	{	return false; }		
	//Project Setting	

	if ( MoveCameraToBeforePCBInPosition(true) == false )
	{	return false; }	


	CheckNPM_FuncBypass();
	SetOfflineMode(false);		
	SetRepeatedTestCount(0);
	SetRepeatedTestMaxCount(0);
	SetIsRepeatTest(false);
	SetIsRepeatTestUI(false);
	UpdateLaneWorkMode(TaskMode);
	SetIsNeedResetOKNGSignal(true);
	SetIsNeedGrabFiducial(true, LANE_ID_A);
	SetIsNeedGrabFiducial(true, LANE_ID_B);
	SetIsNeedResetLightCtrlDLP(true);	
	SetIsOnlineCheckSystemReady(true);
	SwitchProjectTaskMode(PROJECT_TASK_NORMAL);
	if ( LayoutLaneProjectList() == false )
	{	return false; }	

	if ( FN_ENABLE == SysParam.m_OnlineOpenProjectCameraBarcode )
	{
		CAOIProject *OpenProjectProjectPtr = GetOpenProjectProjectPtr();
		if ( NULL != OpenProjectProjectPtr )
		{
			//ExecInspectProjectBarcode				
			FIELD_BUILD_MODE FieldBuildMode = FIELD_BUILD_RANDOM_PROJECT;
			FIELD_BUILD_AREA_MODE FieldAreaMode = FIELD_BUILD_AREA_COMPONENT;
			OpenProjectProjectPtr->InitProjectInspection(LANE_ID_A);
			OpenProjectProjectPtr->ClearProjectAllInspectionField();				
			OpenProjectProjectPtr->SetProjectAllObjToNeedToCalculateRgn(true);
			OpenProjectProjectPtr->SetProjectInspectionFieldBuildMode(FieldBuildMode);
			OpenProjectProjectPtr->SetProjectInspectionFieldBuildAreaMode(FieldAreaMode);
			if ( OpenProjectProjectPtr->CreateProjectInspectionObject(FieldBuildMode, FieldAreaMode) == false )
			{
				str = OpenProjectProjectPtr->GetErrorString();
				m_ErrorString = str;
				return false;
			}		
		}
	}

	int LastStationLineMode = GetLastStationLineMode();	
	LANE_WORK_MODE LaneWorkMode_LA = GetLaneWorkMode_LA();
	LANE_WORK_MODE LaneWorkMode_LB = GetLaneWorkMode_LB();
	ONLINE_STATE_MODE OnlineState_LA = GetOnlineStateMode_LA();
	ONLINE_STATE_MODE OnlineState_LB = GetOnlineStateMode_LB();
	PCB_OUT_DIRECTION PcbOutDirection = GetPCBOutDirection();
	//雙軌模式, 啟動前確認機制
	if ( MULTI_LANE_2 == MultiLane )
	{	
		LANE_ID LaneID = LANE_ID_A;
		//注意以下確認順序
		if ( LANE_WORK_BYPASS == LaneWorkMode_LB )
		{	LaneID = LANE_ID_B;	}
		if ( LANE_WORK_BYPASS == LaneWorkMode_LA )
		{	LaneID = LANE_ID_A;	}
		if ( LANE_WORK_RUN == LaneWorkMode_LB )
		{	LaneID = LANE_ID_B;	}
		if ( LANE_WORK_RUN == LaneWorkMode_LA )
		{	LaneID = LANE_ID_A;	}

		if ( LANE_WORK_DISABLE != LaneWorkMode_LB )
		{
			const bool bPCBIn=PlcCtrlPtr->GetConveryerSensorPCBIn_LB();
			const bool bPCBOut=PlcCtrlPtr->GetConveryerSensorPCBOut_LB();
			const bool bPCBStop=PlcCtrlPtr->GetConveryerSensorPCBStop_LB();
			if ( false == bPCBStop ) 
			{	
				OnlineState_LB = ONLINE_STATE_INSPECTION_STOP; 
				SetOnlineStateMode_LB(OnlineState_LB);
			}
			if ( PCB_OUT_DIR_FORWARD==PcbOutDirection || PCB_OUT_DIR_BACKWARD_OUT==PcbOutDirection )
			{
				if ( bPCBIn == true || bPCBOut == true )
				{
					str = _T("Error, the board is on the converyer (PCB-In or PCB-Out) (Lane B)");
					m_ErrorString = PlcCtrlPtr->LoadMultiLanguageString(str, str);
					return false;
				}
			}
			PlcCtrlPtr->ReadLaneAdjustCurrentPos_LB();
			if ( bPCBStop==true && LANE_WORK_RUN==LaneWorkMode_LB && ONLINE_STATE_INSPECTION_FINISH!=OnlineState_LB )
			{	LaneID = LANE_ID_B;	}
		}
		if ( LANE_WORK_DISABLE != LaneWorkMode_LA )
		{
			const bool bPCBIn=PlcCtrlPtr->GetConveryerSensorPCBIn_LA();
			const bool bPCBOut=PlcCtrlPtr->GetConveryerSensorPCBOut_LA();
			const bool bPCBStop=PlcCtrlPtr->GetConveryerSensorPCBStop_LA();
			if ( false == bPCBStop ) 
			{	
				OnlineState_LA = ONLINE_STATE_INSPECTION_STOP; 
				SetOnlineStateMode_LA(OnlineState_LA);
			}
			if ( PCB_OUT_DIR_FORWARD==PcbOutDirection || PCB_OUT_DIR_BACKWARD_OUT==PcbOutDirection )
			{
				if ( bPCBIn == true || bPCBOut == true )
				{
					str = _T("Error, the board is on the converyer (PCB-In or PCB-Out) (Lane A)");
					m_ErrorString = PlcCtrlPtr->LoadMultiLanguageString(str, str);
					return false;
				}
			}
			PlcCtrlPtr->ReadLaneAdjustCurrentPos_LA();
			if ( bPCBStop==true && LANE_WORK_RUN==LaneWorkMode_LA && ONLINE_STATE_INSPECTION_FINISH!=OnlineState_LA )
			{	LaneID = LANE_ID_A;	}
		}

		LANE_ID LaneNextID;
		LANE_ID LaneTempID;
		LANE_WORK_MODE LaneWorkMode;
		ONLINE_STATE_MODE LaneOnlineState;
		LANE_ID LaneOrder[LANE_ID_RETURN];
		LANE_ID LaneActID = GetActiveLaneID();
		//先跳開目前檢測的軌道		
		LaneNextID = LANE_ID_NULL;
		switch ( LaneActID )
		{
		case LANE_ID_B:	//LaneOrder[0] = LANE_ID_A;	LaneOrder[1] = LANE_ID_B;
			if ( ONLINE_STATE_INSPECTION_FINISH == OnlineState_LB )
			{	LaneOrder[0] = LANE_ID_A;	LaneOrder[1] = LANE_ID_B;	}
			else
			{	LaneOrder[0] = LANE_ID_B;	LaneOrder[1] = LANE_ID_A;	}
			break;
		default:
		case LANE_ID_A:	//LaneOrder[0] = LANE_ID_B;	LaneOrder[1] = LANE_ID_A;
			if ( ONLINE_STATE_INSPECTION_FINISH == OnlineState_LA )
			{	LaneOrder[0] = LANE_ID_B;	LaneOrder[1] = LANE_ID_A;	}
			else
			{	LaneOrder[0] = LANE_ID_A;	LaneOrder[1] = LANE_ID_B;	}
			break;
		}
		//確認軌道-1
		if ( LANE_ID_NULL == LaneNextID )
		{
			LaneTempID = LaneOrder[0];
			LaneWorkMode = GetLaneWorkMode(LaneTempID);	
			LaneOnlineState = GetOnlineStateMode_Lane(LaneTempID);
			if ( PlcCtrlPtr->GetConveryerSensorPCBStop(LaneTempID)==true && LANE_WORK_RUN==LaneWorkMode && ONLINE_STATE_INSPECTION_FINISH!=LaneOnlineState )
			{	LaneNextID = LaneTempID;	}			
		}
		//確認軌道-2
		if ( LANE_ID_NULL == LaneNextID )
		{
			LaneTempID = LaneOrder[1];
			LaneWorkMode = GetLaneWorkMode(LaneTempID);
			LaneOnlineState = GetOnlineStateMode_Lane(LaneTempID);
			if ( PlcCtrlPtr->GetConveryerSensorPCBStop(LaneTempID)==true && LANE_WORK_RUN==LaneWorkMode && ONLINE_STATE_INSPECTION_FINISH!=LaneOnlineState )
			{	LaneNextID = LaneTempID;	}			
		}
		if ( LANE_ID_NULL != LaneNextID )
		{	LaneID = LaneNextID;	}
		SetActiveLaneID(LaneID);
		SetOnlineStateMode(ONLINE_STATE_INSPECTION_STOP);			 
		//SetOnlineStateMode_Lane(ONLINE_STATE_INSPECTION_STOP, LaneID);
	}
	else
	{
		LANE_ID LaneID = LANE_ID_A;	
		if ( LANE_WORK_RUN == LaneWorkMode_LB )
		{	
			LaneID = LANE_ID_B;	 
			PlcCtrlPtr->ReadLaneAdjustCurrentPos(LaneID);
		}
		if ( LANE_WORK_RUN == LaneWorkMode_LA )
		{	
			LaneID = LANE_ID_A;	 
			PlcCtrlPtr->ReadLaneAdjustCurrentPos(LaneID);
		}
		SetActiveLaneID(LaneID);	
	}

	//Project Setting		
	CAOIProject *ProjectPtr_Other=NULL;

	ProjectPtr->SetProjectTickCountStamp(0);
	ProjectPtr->SetProjectSpcFileSaveEnabled(true);	
	for ( i=0; i<ProjectCount; i++ )
	{
		ProjectPtr_Other = GetProjectPtr(i, false);
		if ( NULL == ProjectPtr_Other ) { continue; }
		ProjectPtr_Other->SetProjectFdException(false);		
		ProjectPtr_Other->SetProjectIsException(false);
		ProjectPtr_Other->ApplySystemParameter(LANE_ID_A);
		ProjectPtr_Other->SetProjectAllObjToNeedToCalculateRgn(true);		
		ProjectPtr_Other->ReleaseProjectProgramFieldFrameImageBuffer();//釋放專案編程區域記憶體
		if ( ProjectPtr_Other->CheckProjectBarcodeDeviceIDList() == false )
		{
			m_ErrorString = ProjectPtr_Other->GetErrorString();
			return false;
		}		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecInspectionProjectOpenCode()//執行檢測專案開啟條碼
{	
	return ExecInspectionProjectOpenCode_2();
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecInspectionProjectOpenCode_2()//執行檢測專案開啟條碼
{
	CAOIProject *ProjectPtr = GetOpenProjectProjectPtr();
	if ( NULL == ProjectPtr )
	{	return false; }

	CString        str;	
	LANE_ID        LaneID = LANE_ID_A;	
	OFFLINE_FILE_MODE OfflineFileMode = OFFLINE_FILE_INSPECTION;
	FIELD_BUILD_MODE  FieldBuildMode=ProjectPtr->GetProjectInspectionFieldBuildMode();
	if ( GetOnlineOpenProjectCameraBarcodeTestState() == BARCODE_CAMERA_TEST_STATE_ALL )
	{	ProjectPtr->SetProjectAllObjToNeedToCalculateRgn(true);	}
	if ( ProjectPtr->InitProjectInspection(LaneID) == false )
	{	
		SetIsIgnoreAutoRetry(true);
		m_ErrorString = ProjectPtr->GetErrorString();
		return false;	
	}
	if ( ExecInspectProjectBarcode(ProjectPtr, OfflineFileMode) == false )
	{	return false; }	
	
	if ( OpenProjectByBarcodeCamera() == false )
	{
		SetIsIgnoreAutoRetry(true);
		return false;
	}

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecThreadSequenceFn()//執行執行緒程序執行緒
{
	CString str;
	bool IsOK = false;	
	const LANE_ID LaneID = GetActiveLaneID();	
	TASK_MODE TaskMode = GetTaskMode();
	TASK_STATE_MODE TaskStateMode = GetOnlineTaskState();	
	DeleteMovingTimeMsg();	
	DeleteCurrentProcess();
	ResetSystemException();
	SetSaveDebugMessage_RGN(false);

	//先重設定所有燈原狀態-拉在外面, 減少設定次數
	if ( GetIsNeedResetLightCtrlDLP() == true )
	{	
		if ( GetOfflineMode() == false )
		{
			//if ( SetupAllLightSetting() == false )
			//{	return false;	}
			SetIsNeedResetLightCtrlDLP(false);
		}		
	}
	SetIsOnInspection(true);
	SetIsNeedReCheckPCBInside(false);	
	PlcCtrlPtr->WriteOnInspection(LaneID, true);
	PlcCtrlPtr->TurnOffConveyerSensorPower(LaneID, true);
	const bool bCheckOK=true;
	switch ( TaskMode )
	{
	case TASK_PROJECT_MAP://專案底圖
		IsOK = ExecThreadSequenceFn_ProjectMap();
		if ( false == IsOK )
		{	SetSystemExceptionCode(AOI_EXCEPTION_SYSTEM_EXEC_PROJECT_MAP, bCheckOK); }
		break;
	case TASK_ALIGN_PROJECT://對齊專案
		IsOK = ExecThreadSequenceFn_AlignProject();
		if ( false == IsOK )
		{	SetSystemExceptionCode(AOI_EXCEPTION_SYSTEM_EXEC_PROJECT_ALIGN, bCheckOK); }
		break;
	case TASK_EXPORT_OFFLINE://匯出離線專案
		IsOK = ExecThreadSequenceFn_ExportOffline();
		if ( false == IsOK )
		{	SetSystemExceptionCode(AOI_EXCEPTION_SYSTEM_EXEC_OFFLINE_EXPORT, bCheckOK); }
		break;
	case TASK_TUNING_PROJECT://調適專案
		IsOK = ExecThreadSequenceFn_InspectProject();
		if ( false == IsOK )
		{	SetSystemExceptionCode(AOI_EXCEPTION_SYSTEM_EXEC_PROJECT_TUNING, bCheckOK); }
		break;
	case TASK_TUNING_OFFLINE://調適離線專案
		IsOK = ExecThreadSequenceFn_InspectOffline();
		if ( false == IsOK )
		{	SetSystemExceptionCode(AOI_EXCEPTION_SYSTEM_EXEC_OFFLINE_TUNING, bCheckOK); }
		break;
	case TASK_INSPECT_PROJECT://檢測專案
		IsOK = ExecThreadSequenceFn_InspectProject();
		if ( false == IsOK )
		{	SetSystemExceptionCode(AOI_EXCEPTION_SYSTEM_EXEC_PROJECT_INSPECT, bCheckOK); }
		break;	
	case TASK_LANE_BYPASS://軌道流片
		IsOK = true;
		break;
	default:
		this->m_ErrorString.Format(_T("Error, Task Mode Exception"));
		SetSystemExceptionCode_Param(m_ErrorString);
		break;
	}
	SetIsOnInspection(false);
	SetInspectionDrawing(false);
	SetThreadGrabMode(THREAD_GRAB_NONE);
	PlcCtrlPtr->TurnOffConveyerSensorPower(LaneID, false);
	if ( false == IsOK ) 
	{
		if ( GetIsSystemException() == true )
		{	return true; }		
		return false;	
	}	

	//要讓執行緒跑到下一個迴圈, 所以要等一等
	size_t i=0;
	DWORD  SleepTime = 10;
	const size_t MaxI = 1000;
	if ( GetSaveOfflineImageFiles() == true )//如果有儲存離線編程的話, 時間多100倍
	{	SleepTime = 1000; }
	for ( i=0; i<MaxI; i++ )
	{
		if ( GetIsSystemReleased() == true ) { break; }
		if ( GetIsSystemException() == true ) { break; }		
		if ( CheckAllCalcThreadStateFinish() == true )
		{	break;	}
		::Sleep(SleepTime);
	}	
	PlcCtrlPtr->WriteOnInspection(LaneID, false);	
	str.Format(_T("Wait for Calc Thread to Finish State Count=%d"), i+1);
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	if ( MaxI == i )
	{	return false; }

	SetIsNeedResetLightCtrlDLP(true);
	LightCtrlBoard.ClearLCBTableList();
	if ( GetIsSystemException() == true )
	{	return true; }
	
	CAOIProject  *ProjectPtr = GetActiveProject();		
	if ( NULL != ProjectPtr )
	{
		ProjectPtr->RemoveProjectTestTrackFolder();	
		if ( ProjectPtr->GetProjectConveyerPreRunRunning() == true )
		{
			ProjectPtr->SetProjectTickCountCalcFinish();
			ProjectPtr->SetProjectTickCountTestFinish();
		}
	}

	if ( TASK_INSPECT_PROJECT == TaskMode )
	{
		MULTI_PROJECT_TEST_ORDER_MODE MultiProjectTestOrderMode=GetMultiProjectTestOrderMode();
		if ( MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_A_B==MultiProjectTestOrderMode || MULTI_PROJECT_TEST_ORDER_ONE_CYCLE_B_A==MultiProjectTestOrderMode )
		{
			bool bNext = false;
			if ( ExecInspectionProjectByTurnOneCycleNext(LaneID, bNext) == false )
			{	return false; }
			if ( true == bNext )
			{	return true; }
		}		
	}

	ProjectPtr = GetActiveProject();		
	MULTI_LANE_MODE   MultiLaneMode = CheckMultiLaneMode();
	PCB_OUT_DIRECTION PCBOutDirection = GetPCBOutDirection();
	ONLINE_STATE_MODE OnlineStateMode = GetOnlineStateMode();
	ONLINE_STATE_MODE OnlineStateModeNext = GetOnlineStateMode();
	ONLINE_STATE_MODE OnlineStateModePreRun = GetOnlineStateMode();
	if ( NULL != ProjectPtr )
	{
		if ( TASK_ALIGN_PROJECT == TaskMode )
		{	ProjectPtr->RemoveProjectAllInspectionFolder();	}
		else if ( TASK_TUNING_PROJECT==TaskMode || TASK_TUNING_OFFLINE==TaskMode )
		{	OnlineStateMode = ONLINE_STATE_INSPECTION_STOP;	}		
		else if ( TASK_INSPECT_PROJECT==TaskMode )
		{
			//size_t LaneIndex = GetActiveLaneID();
			LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
			size_t LaneIndex = GetConveyerAutoRunLaneIdx(LaneID);//注意系統的軌道可能已經切換
			const bool  ConveyerPreRunRunning = ProjectPtr->GetProjectConveyerPreRunRunning();			
			if ( true == ConveyerPreRunRunning )
			{
				bool bStopOK = true;
				THREAD_STATE_MODE ConveyerPerThreadState = GetConveyerAutoRunThreadState(LaneIndex);
				bStopOK = StopConveyerAutoRunThread(LaneIndex, true);
				if ( THREAD_STATE_EXCEPTION == ConveyerPerThreadState )
				{
					SetIsOnlineCheckSystemReady(true);
					ProjectPtr->SetProjectConveyerPreRunRunning(false);//後面會用到, 所以不使用
					return false;
				}
				if ( true == bStopOK )
				{
					OnlineStateModePreRun = OnlineStateMode = GetConveyerAutoRunState(LaneIndex);					
					OnlineStateModeNext = OnlineStateMode;
					SetConveyerAutoRunProjectPtr(LaneIndex, NULL);
					SetConveyerAutoRunState(LaneIndex, ONLINE_STATE_INSPECTION_STOP);					
					ProjectPtr->SetProjectOnlineStateMode(OnlineStateMode);		

					CString str;
					CString strState = AOIDataDefine.GetOnlineStateText(OnlineStateModePreRun);
					str.Format(_T("Conveyer Pre Run Close [%s]"), strState);
					SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);	
					SetOnlineStateMode_Lane(OnlineStateMode, (LANE_ID)(LaneIndex));

					const bool bDualRunMode = CheckDualRunMode();//20230425		
					if ( true == bDualRunMode )
					{	
						if ( StartConveyerAutoRunThread(LaneIndex, OnlineStateModePreRun, ProjectPtr, false, false) == false )
						{	return false; }
						OnlineStateModeNext = OnlineStateMode = ONLINE_STATE_PCB_DUAL_RUN_CHECKING;	
					}
				}
				SetIsOnlineCheckSystemReady(true);
				ProjectPtr->SetProjectConveyerPreRunRunning(false);//後面會用到, 所以不使用				
			}
			else
			{
				OnlineStateModeNext = OnlineStateMode = ONLINE_STATE_INSPECTION_FINISH;				
				ProjectPtr->SetProjectOnlineStateMode(OnlineStateMode); 
			}
			if ( ProjectPtr->GetProjectIsException() == true ) 
			{
				OnlineStateMode = OnlineStateModeNext = ONLINE_STATE_INSPECTION_STOP;
				SetOnlineTaskState(TASK_STATE_TO_STOP);	
			}
			else
			{	
				if ( ProjectPtr->GetProjectAlarmDefect() == true ) 
				{	
					CString str;
					TTestResult TestResult = ProjectPtr->GetProjectResultAlarm();
					SetOnlineTaskState(TASK_STATE_TO_STOP);						
					if ( TEST_RESULT_NG == TestResult.sResultID )
					{							
						str.Format(_T("%s\\%s"), GetAOITempDirectory(), _T("ResultAlarmComponentList.TXT"));
						if ( ProjectPtr->SaveProjectResultAlarmComponentList(str) == true )
						{	::ShellExecute(NULL, _T("open"), str, NULL, NULL, SW_SHOW); }
					}
					else
					{
						CString strAlarm = ProjectPtr->GetProjectAlarmString();
						if ( strAlarm.GetLength() > 0 ) 
						{							
							str.Format(_T("%s\\%s"), GetAOITempDirectory(), _T("ResultAlarmString.TXT"));
							if ( ProjectPtr->SaveProjectResultAlarmString(str) == true )
							{	::ShellExecute(NULL, _T("open"), str, NULL, NULL, SW_SHOW); }
						}
					}
				}
				//else
				{
					bool bTestNG = false;
					CString strDateTime;
					char    DateTimeBuffer[32]="";
					JetAPI::GetTime(strDateTime, ProjectPtr->GetProjectResultCurrent().sDateTimeS);
					TEST_RESULT_ID TestResultID = ProjectPtr->GetProjectResultCurrent().sResultID;
					DEFECT_HANDLE_MODE DefectHandelMode = ProjectPtr->GetProjectParameter().m_DefectHandleMode;

					JetAPI::TCHAR2char(strDateTime, DateTimeBuffer, 32);
					switch ( TestResultID )
					{
					case TEST_RESULT_OK:
						bTestNG = false;
						break;
					case TEST_RESULT_NG:
					case TEST_RESULT_FD:
						bTestNG = true;
						break;
					}
					switch ( DefectHandelMode )
					{
					case DEFECT_HANDLE_NEXT_STOP://停於下一站						
						SendPCBOKNGSignal(LaneID, bTestNG);
						break;
					case DEFECT_HANDLE_PASS://通過, 燒機模式
						break;
					case DEFECT_HANDLE_STOP_ALARM://停機警報
						SendPCBOKNGSignal(LaneID, bTestNG);
						if ( true == bTestNG )
						{	SetOnlineTaskState(TASK_STATE_TO_STOP);	}						
						break;
					case DEFECT_HANDLE_WAIT_FOR_REPAIR://等人員判定						
						if ( MULTI_LANE_2 == MultiLaneMode )//雙軌道模式
						{	
							const bool bDualRunMode = CheckDualRunMode();//20230425	
							if ( true == bDualRunMode )
							{								
								StopConveyerAutoRunThread(LaneIndex, true);								
								if ( StartConveyerAutoRunThread(LaneIndex, ONLINE_STATE_WAIT_FOR_REPAIR_VERIFY, ProjectPtr, false, false) == false )
								{	return false; }
								ResetSwitchMultiLine_NextLaneID();
								OnlineStateMode = OnlineStateModeNext = ONLINE_STATE_PCB_DUAL_RUN_CHECKING;
							}
							else
							{
								OnlineStateModeNext = ONLINE_STATE_PCB_AUTO_RUN_CHECKING;	
								if ( PCB_OUT_DIR_BACKWARD==PCBOutDirection || PCB_OUT_DIR_BACKWARD_OUT==PCBOutDirection)
								{	PlcCtrlPtr->WritePCBAutoBackInFinish(LaneID, false);	}
								else
								{	PlcCtrlPtr->WritePCBAutoOutInFinish(LaneID, false);	}
								PlcCtrlPtr->SetPCBAutoRunMode(LaneID, PLC_PCB_AUTO_RUN_STOP);
							}
						}
						else
						{	OnlineStateMode = OnlineStateModeNext = ONLINE_STATE_WAIT_FOR_REPAIR_VERIFY;	}

						if ( ONLINE_STATE_WAIT_FOR_REPAIR_VERIFY == OnlineStateModeNext )
						{
							if ( true == bTestNG )
							{	PlcCtrlPtr->TurnOnInspectAlarm(LaneID);	}
						}

						MoveCameraToBeforePCBInPosition(false);
						SetRepairDateTimeToCheck(LaneID, DateTimeBuffer);
						PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);						
						break;
					case DEFECT_HANDLE_CONTROL_CENTER://中控中心	
						MoveCameraToBeforePCBInPosition(false);
						SetCCSDateTimeFinish(LaneID, DateTimeBuffer);
						//SaveControlCenterLog(_T("ExecThreadSequenceFn(Finish)"));
						//ExecCCSDataChange(LaneID, DateTimeBuffer);//20221031						
						break;
					}					
				}
			}
		}
	}


	/*
	switch ( TaskMode )
	{
	case TASK_PROJECT_MAP:
	case TASK_ALIGN_PROJECT:
	case TASK_EXPORT_OFFLINE:
	case TASK_TUNING_PROJECT:
	case TASK_TUNING_OFFLINE:
	case TASK_EXPORT_COMPONENT:
		SetTaskMode(TASK_NONE);
		break;
	}*/		

	if ( TASK_INSPECT_PROJECT == TaskMode )
	{
		if ( ONLINE_STATE_INSPECTION_FINISH != OnlineStateMode )//軌道提前運轉會跳過檢測結束, 要補檢測結束訊息給MES
		{
			if ( ExecMESComm_ProcessID(MES_STATAUS_AOI_INSPECTION_COMPLETE_CMD) == false ) 
			{	return false; }
		}
	}

	SetOnlineStateMode(OnlineStateMode);//由於其他執行緒會執行, 因此需要最後更新
	SetOnlineStateMode_GUI(OnlineStateMode);
	SetOnlineStateMode_Next(OnlineStateModeNext);//由於其他執行緒會執行, 因此需要最後更新		
	SetOnlineStateMode_GUI_Lane(OnlineStateMode, LaneID);	
	PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);//20190819
	//SendCallbackWndMessage(MSG_INSPECTION_CALLBACK, WPARAM_INSPECTION_FINISH, NULL);
	PostCallbackWndMessage(MSG_INSPECTION_CALLBACK, WPARAM_INSPECTION_FINISH, NULL);

	if ( TASK_INSPECT_PROJECT == TaskMode )
	{
		if ( CheckMultiLaneMode_Off() == true )
		{
			SetOnlineTaskCancel(true);
			SetOnlineTaskState(TASK_STATE_TO_STOP);	
			MoveCameraToBeforePCBInPosition(false);
		}
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecThreadSequenceFn_ProjectMap()//執行執行緒程序執行緒-專案底圖
{	
	CString str;
	size_t  FovCount = 0;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	
	const UINT MsgInspection = MSG_INSPECTION_CALLBACK;	
	JetMemory.reset_memory_exec_time();

	str = _T("Exec Project Map Thread Start");	
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

	//WPARAM_INSPECTION_PROJECT_MARK
	//WPARAM_INSPECTION_PROJECT_INITIAL		
	if ( ExecCaptureProjectMap(ProjectPtr) == false )//取專案底圖
	{
		SendCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_SET_DRAW_PROJECT_MODE, LPARAM_DRAW_PROJECT_MODE_NORMAL);
		//WaitForAllCalcThreadStop();
		//ProjectPtr->ClearProjectAllTempObjects();
		return false; 
	}	
	
	str = _T("Exec Project Map Thread End");	
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

	SetProjectLightSetting(ProjectPtr);
	SetIsNeedResetLightCtrlDLP(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecThreadSequenceFn_AlignProject()//執行執行緒程序執行緒-專案對齊
{
	CString str;
	size_t  FovCount = 0;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }		
	CString OfflineFolder;
	CString OfflineFdName;
	LANE_ID LaneID = GetActiveLaneID();		
	TASK_MODE TaskMode = GetTaskMode();
	const UINT MsgInspection = MSG_INSPECTION_CALLBACK;		
	const TSystemParameter &SysParam = GetSystemParameter();
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
	const TProjectParameter &ProjectParam = ProjectPtr->GetProjectParameter();
	JetMemory.reset_memory_exec_time();

	//0. 資料清除
	ClearRepairDateTimeToCheck(LaneID);
	SetLanePCBOutMode(LaneID, ProjectParam.m_PCBOutMode);
	SetLaneDefectHandleMode(LaneID, ProjectParam.m_DefectHandleMode);		
	if ( ProjectPtr->InitProjectInspection(LaneID) == false )
	{
		this->m_ErrorString = ProjectPtr->GetErrorString();
		return false;
	}
	if ( TASK_INSPECT_PROJECT == TaskMode )
	{
		if ( ProjectPtr->AssignProjectBarcodeHandHeldCode(LaneID) == false )
		{
			this->m_ErrorString = ProjectPtr->GetErrorString();
			return false;
		}
		if ( ProjectPtr->AssignProjectBarcodeDeviceCode(LaneID) == false )
		{
			this->m_ErrorString = ProjectPtr->GetErrorString();
			return false;
		}
	}

	//WPARAM_INSPECTION_PROJECT_MARK
	//WPARAM_INSPECTION_PROJECT_INITIAL	
	str = _T("Exec Align Project Thread Start");	
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);	
	bool  FdException = false;   
	const bool MultiFdLight = GetSystemMultiFdLight();
	const bool IsNeedGrabFiducial = GetIsNeedGrabFiducial();	
	const DWORD GrabFiducialDelayTime = SysParam.m_GrabFiducialDelayTime_ms;
	const DWORD InPositionDelayTime = MotionCtrlPtr->GetMotionParameter().m_InPositionDelayTime;
	const DWORD TotalGrabFidDelayTime = GrabFiducialDelayTime+InPositionDelayTime;
	BOARD_FD_GRAB_MODE BoardFdGrabMode = ProjectPtr->GetProjectBoardFdGrabMode();
	if ( true == IsNeedGrabFiducial )
	{
		//1. 整板定位點			
		SetOnlineStateMode(ONLINE_STATE_INSPECT_FD_PANEL);
		SetOnlineStateMode_Lane(ONLINE_STATE_INSPECT_FD_PANEL, LaneID);
		PostCallbackWndMessage(MsgInspection, WPARAM_INSPECTION_PANEL_FD, NULL);
		PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
		MotionCtrlPtr->SetWaitForDoneDelayTime(TotalGrabFidDelayTime);
		ProjectPtr->ReleaseProjectInspectionFieldFrameImageBuffer();//重掃定位點, 因此需要將圖像清除
		if ( ExecAlignProjectPanelFd(ProjectPtr) == false )//對齊專案整板定位點
		{	
			WaitForAllCalcThreadStop();
			ProjectPtr->SetProjectIsException(true);
			ProjectPtr->ClearProjectAllTempObjects();
			MotionCtrlPtr->SetWaitForDoneDelayTime(InPositionDelayTime);
			return false; 
		}	
		
		//2. 單板定位點
		FdException = ProjectPtr->GetProjectFdException();
		//if ( true == FdException ) 
		//{	return true; }
		if ( BOARD_FD_GRAB_AFTER_PANEL == BoardFdGrabMode )
		{
			SetOnlineStateMode(ONLINE_STATE_INSPECT_FD_BOARD);
			SetOnlineStateMode_Lane(ONLINE_STATE_INSPECT_FD_BOARD, LaneID);
			PostCallbackWndMessage(MsgInspection, WPARAM_INSPECTION_BOARD_FD, NULL);
			PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
			if ( ExecAlignProjectBoardFd(ProjectPtr) == false )//對齊專案單板定位點
			{
				WaitForAllCalcThreadStop();
				ProjectPtr->SetProjectIsException(true);
				ProjectPtr->ClearProjectAllTempObjects();
				MotionCtrlPtr->SetWaitForDoneDelayTime(InPositionDelayTime);
				return false; 
			}		
		}
		MotionCtrlPtr->SetWaitForDoneDelayTime(InPositionDelayTime);
		FdException = ProjectPtr->GetProjectFdException();
		//if ( true == FdException ) 
		//{	return true; }
	}
	OfflineFolder = ProjectPtr->GetProjectInspectionOfflineFolder();//m_OfflineFolder
	::CreateDirectory(OfflineFolder, NULL);
	OfflineFdName = AOIDataDefine.GetProjectOfflineFdName(OfflineFolder, DistrictID);
	ProjectPtr->SaveProjectOfflineFdFile(OfflineFdName);
	ProjectPtr->SetProjectOfflineFileMode(OFFLINE_FILE_INSPECTION);
	
	//3. FOV/Field Assign
	FIELD_BUILD_MODE BuildMode = ProjectPtr->GetProjectInspectionFieldBuildMode();
	FIELD_BUILD_AREA_MODE AreaMode = ProjectPtr->GetProjectInspectionFieldBuildAreaMode();	
	OFFLINE_FILE_MODE OfflineFileMode = ProjectPtr->GetProjectOfflineFileMode();
	PostCallbackWndMessage(MsgInspection, WPARAM_INSPECTION_PROJECT_RESET, NULL);	
	
	if ( ProjectPtr->AdjustProjectField(OfflineFileMode, BuildMode, AreaMode) == false )
	{
		ProjectPtr->SetProjectIsException(true);
		this->m_ErrorString = ProjectPtr->GetErrorString();
		return false;
	}	
	if ( ProjectPtr->AssignProjectField(OfflineFileMode) == false )
	{
		ProjectPtr->SetProjectIsException(true);
		this->m_ErrorString = ProjectPtr->GetErrorString();
		return false;
	}		
	ProjectPtr->SetProjectTickCountGrabFinish();
	str = _T("Exec Align Project Thread End");	
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	
	SetIsNeedGrabFiducial(false);	
	SetIsNeedResetLightCtrlDLP(true);
	SetOnlineStateMode(ONLINE_STATE_INSPECTION_STOP);	
	SetOnlineStateMode_Lane(ONLINE_STATE_INSPECTION_STOP, LaneID);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecThreadSequenceFn_ExportOffline()//執行執行緒程序執行緒-離線程式
{
	CString str;
	size_t  FovCount = 0;	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	
	CString BaseFolder;
	CString ProjectFolder;
	CString OfflineFolder;
	CString OfflineFdName;	
	CString FileMainName = ProjectPtr->GetProjectFileMainName();
	LANE_ID     LaneID = ProjectPtr->GetProjectActLaneID();
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
	TProjectParameter &ProjectParam = ProjectPtr->GetProjectParameter();
	const UINT MsgInspection = MSG_INSPECTION_CALLBACK;	
	const TSystemParameter &SysParam = GetSystemParameter();
	JetMemory.reset_memory_exec_time();	

	//WPARAM_INSPECTION_PROJECT_MARK
	//WPARAM_INSPECTION_PROJECT_INITIAL	

	str = _T("Exec Export Offline Thread Start");	
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

	FIELD_BUILD_MODE FieldBuildMode = FIELD_BUILD_MATRIX;//ProjectPtr->GetProjectFieldBuildMode();
	FIELD_BUILD_AREA_MODE FieldAreaMode = FIELD_BUILD_AREA_COMPONENT;//ProjectPtr->GetProjectFieldBuildAreaMode()
	OFFLINE_FILE_MODE OfflineFileMode = OFFLINE_FILE_INSPECTION;//ProjectPtr->GetProjectOfflineFileMode();
	BaseFolder = CAOIDataCollect::GetAOITempDirectory();
	ProjectFolder.Format(_T("%s\\%s"), BaseFolder, FileMainName);
	::CreateDirectory(ProjectFolder, NULL);
	::Sleep(0);
	m_OfflineFolder = AOIDataDefine.GetProjectOfflineFolderName(ProjectFolder);		
	::CreateDirectory(m_OfflineFolder, NULL);
	::Sleep(0);
	
	bool  FdException = false;
	const bool IsNeedGrabFiducial = GetIsNeedGrabFiducial();	
	const DWORD GrabFiducialDelayTime = SysParam.m_GrabFiducialDelayTime_ms;
	const DWORD InPositionDelayTime = MotionCtrlPtr->GetMotionParameter().m_InPositionDelayTime;
	const DWORD TotalGrabFidDelayTime = GrabFiducialDelayTime+InPositionDelayTime;
	BOARD_FD_GRAB_MODE BoardFdGrabMode = ProjectPtr->GetProjectBoardFdGrabMode();
	if ( true == IsNeedGrabFiducial )
	{
		//1. 整板定位點
		SetOnlineStateMode(ONLINE_STATE_INSPECT_FD_PANEL);
		SetOnlineStateMode_Lane(ONLINE_STATE_INSPECT_FD_PANEL, LaneID);
		PostCallbackWndMessage(MsgInspection, WPARAM_INSPECTION_PANEL_FD, NULL);
		PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
		MotionCtrlPtr->SetWaitForDoneDelayTime(TotalGrabFidDelayTime);
		ProjectPtr->ReleaseProjectInspectionFieldFrameImageBuffer();//重掃定位點, 因此需要將圖像清除
		if ( ExecAlignProjectPanelFd(ProjectPtr) == false )//對齊專案整板定位點
		{	
			WaitForAllCalcThreadStop();
			ProjectPtr->SetProjectIsException(true);
			ProjectPtr->ClearProjectAllTempObjects();
			MotionCtrlPtr->SetWaitForDoneDelayTime(InPositionDelayTime);
			return false; 
		}			
	
		//2. 單板定位點		
		FdException = ProjectPtr->GetProjectFdException();
		if ( true == FdException ) 
		{	return true; }
		if ( BOARD_FD_GRAB_AFTER_PANEL == BoardFdGrabMode )
		{
			SetOnlineStateMode(ONLINE_STATE_INSPECT_FD_BOARD);
			SetOnlineStateMode_Lane(ONLINE_STATE_INSPECT_FD_BOARD, LaneID);
			PostCallbackWndMessage(MsgInspection, WPARAM_INSPECTION_BOARD_FD, NULL);	
			PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
			if ( ExecAlignProjectBoardFd(ProjectPtr) == false )//對齊專案單板定位點
			{
				WaitForAllCalcThreadStop();
				ProjectPtr->SetProjectIsException(true);
				ProjectPtr->ClearProjectAllTempObjects();
				MotionCtrlPtr->SetWaitForDoneDelayTime(InPositionDelayTime);
				return false; 
			}		
		}
		MotionCtrlPtr->SetWaitForDoneDelayTime(InPositionDelayTime);
		FdException = ProjectPtr->GetProjectFdException();
		if ( true == FdException ) 
		{	return true; }
	}
	SetIsNeedGrabFiducial(false);		
	OfflineFolder = ProjectPtr->GetProjectInspectionOfflineFolder();//m_OfflineFolder
	OfflineFdName = AOIDataDefine.GetProjectOfflineFdName(OfflineFolder, DistrictID);
	ProjectPtr->SaveProjectOfflineFdFile(OfflineFdName);
	ProjectPtr->SetProjectOfflineFileMode(OFFLINE_FILE_INSPECTION);		
	
	//3. FOV/Field Assign	
	PostCallbackWndMessage(MsgInspection, WPARAM_INSPECTION_PROJECT_RESET, NULL);	
	
	if ( ProjectPtr->AdjustProjectField(OfflineFileMode, FieldBuildMode, FieldAreaMode) == false )
	{
		ProjectPtr->SetProjectIsException(true);
		this->m_ErrorString = ProjectPtr->GetErrorString();
		return false;
	}	
	
	if ( ProjectPtr->CheckProjectBarcodeCameraGrabAfterFd() == true )
	{
		//4.1 條碼檢測		
		SetOnlineStateMode(ONLINE_STATE_INSPECT_BARCODE);
		SetOnlineStateMode_Lane(ONLINE_STATE_INSPECT_BARCODE, LaneID);		
		PostCallbackWndMessage(MsgInspection, WPARAM_INSPECTION_BARCODE, NULL);
		PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
		if ( ExecInspectProjectBarcode(ProjectPtr, OfflineFileMode) == false )//對齊專案單板定位點
		{
			WaitForAllCalcThreadStop();
			ProjectPtr->SetProjectIsException(true);
			ProjectPtr->ClearProjectAllTempObjects();
			return false; 
		}		
	}
	//4.2 零件檢測	
	SetOnlineStateMode(ONLINE_STATE_INSPECT_PROJECT);	
	SetOnlineStateMode_Lane(ONLINE_STATE_INSPECT_PROJECT, LaneID);
	PostCallbackWndMessage(MsgInspection, WPARAM_INSPECTION_PROJECT_TEST, NULL);	
	PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	if ( ExecInspectProjectComponent(ProjectPtr, OfflineFileMode) == false )//對齊專案單板定位點
	{
		WaitForAllCalcThreadStop();
		ProjectPtr->SetProjectIsException(true);
		ProjectPtr->ClearProjectAllTempObjects();
		return false; 
	}
	if (BOARD_FD_GRAB_INSPECTING == BoardFdGrabMode)
	{ ProjectPtr->SaveProjectOfflineFdFile(OfflineFdName);}

	SetIsNeedResetLightCtrlDLP(true);
	//5. 檢測統計等等	
	//WPARAM_INSPECTION_PROJECT_ANALYSIS
	ExecAnalysisProjectInspection(ProjectPtr);
	
	str = _T("Exec Export Offline Thread End");	
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecThreadSequenceFn_InspectProject()//執行執行緒程序執行緒-專案檢測	
{
	CString str;
	size_t  FovCount = 0;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	
	LANE_ID          LaneID = GetActiveLaneID();
	TASK_MODE        TaskMode = GetTaskMode();	
	const bool       OfflineMode = GetOfflineMode();
	TProjectParameter &ProjectParam = ProjectPtr->GetProjectParameter();	
	const bool bMulitDistrictMode = ProjectPtr->GetProjectMultiDistrictMode();
	FIELD_BUILD_MODE BuildMode = ProjectPtr->GetProjectInspectionFieldBuildMode();	
	OFFLINE_FILE_MODE OfflineFileMode = OFFLINE_FILE_INSPECTION;//ProjectPtr->GetProjectOfflineFileMode();	
	const UINT MsgInspection = MSG_INSPECTION_CALLBACK;	
	const TSystemParameter &SysParam = GetSystemParameter();
	JetMemory.reset_memory_exec_time();
	
	//WPARAM_INSPECTION_PROJECT_MARK
	//WPARAM_INSPECTION_PROJECT_INITIAL
	str = _T("Exec Inspect Project Thread Start");	
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

	ClearRepairDateTimeToCheck(LaneID);
	SetLanePCBOutMode(LaneID, ProjectParam.m_PCBOutMode);
	const bool bNeedResetOKNGSignal = GetIsNeedResetOKNGSignal();
	if ( true == bNeedResetOKNGSignal )
	{
		SaveControlCenterLog(_T("ExecThreadSequenceFn_InspectProject"));
		ResetPCBOKNGSignal(LaneID, ProjectParam.m_DefectHandleMode);	
	}	
	SetLaneDefectHandleMode(LaneID, ProjectParam.m_DefectHandleMode);
	SetIsNeedResetOKNGSignal(true);
	str = _T("Reset PCB OK-NG Signal");	
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

	//0. 資料清除
	DISTRICT_ID DistrictID = DISTRICT_ID_A;
	if ( true == bMulitDistrictMode )
	{
		if ( ProjectPtr->GetProjectNeedCheckDistrictPos() == true ) 
		{
			DISTRICT_ID LastDistrictID = ProjectPtr->GetProjectActDistrictID();
			if ( DistrictID != LastDistrictID )
			{	
				if ( MovePCBToDistrictID(LaneID, DistrictID) == false )
				{
					SetIsIgnoreAutoRetry(true);
					return false;
				}
			}
		}
	}
	SetActiveDistrictID(DistrictID);
	ProjectPtr->SetProjectNeedCheckDistrictPos(true);
	ProjectPtr->SetProjectActDistrictID(DistrictID, true);			
	if ( true == bMulitDistrictMode )
	{	PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH_MAP, NULL); }
	if ( ProjectPtr->InitProjectInspection(LaneID) == false )
	{
		SetIsIgnoreAutoRetry(true);
		this->m_ErrorString = ProjectPtr->GetErrorString();
		return false;
	}

	CString TestDateTime;
	JetAPI::GetTime(TestDateTime, ProjectPtr->GetProjectResultCurrent().sDateTimeS);
	str.Format(_T("InitProjectInspection[%s]"), TestDateTime);
	AOIDataCollect.SaveMovingTimeMsg(str);

	//if ( CheckStopOnlineTask() == true )
	//{			
	//	SetIsIgnoreAutoRetry(true);
	//	m_ErrorString=_T("Error, User Stop Inspection");
	//	return false; 
	//}	
	if ( TASK_INSPECT_PROJECT == TaskMode )
	{
		if ( ProjectPtr->AssignProjectBarcodeHandHeldCode(LaneID) == false )
		{
			SetIsIgnoreAutoRetry(true);
			this->m_ErrorString = ProjectPtr->GetErrorString();
			return false;
		}
		if ( ProjectPtr->AssignProjectBarcodeDeviceCode(LaneID) == false )
		{
			SetIsIgnoreAutoRetry(true);
			this->m_ErrorString = ProjectPtr->GetErrorString();
			return false;
		}	
	}
	m_OfflineFolder = ProjectPtr->GetProjectInspectionOfflineFolder();
	::CreateDirectory(m_OfflineFolder, NULL);
	::Sleep(0);	

	str = _T("Initial Inspection Finish");		
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);

	if ( ExecMESComm_SetProjectParam() == false )
	{	
		SetIsIgnoreAutoRetry(true);
		JetAPI::ShowMessageBox(GetErrorString());	
		ProjectPtr->SetProjectIsException(true);
		ProjectPtr->ClearProjectAllTempObjects();
		return false; 
	}

	if ( TASK_INSPECT_PROJECT == TaskMode )
	{
		if ( ProjectPtr->CreateProjectTestMapBuffer() == false )
		{
			//4.2 儲存專案檢測底圖
			SetIsIgnoreAutoRetry(true);
			m_ErrorString = ProjectPtr->GetErrorString();			
			ProjectPtr->SetProjectIsException(true);
			ProjectPtr->ClearProjectAllTempObjects();
			return false; 
		}
	}	
#ifndef HASI_disable
	if (true == GetHASI_Enable()) {
		const int HASIQueueSize = GetHASI_SerialFileQueueSize();
		DWORD HASIWaitedTime = 0, HASIDwellTime = 50;
		DWORD HASITimeout = GetHASI_SerialFileDwellTime();
		bool HASIsOK = false;
		while (HASIWaitedTime < HASITimeout && false == HASIsOK) {
			bool bReadSerial = false, bReadSPI = false;
			if (HASIQueueSize == 0 && false == bReadSerial) {
				bReadSerial = ExecHASI_LoadPCBSerialFile(LaneID);
			}
			else if (HASIQueueSize>0) { bReadSerial = true; }
			if (false == bReadSPI) { bReadSPI = ExecHASI_LoadSPIOffsetFile(ProjectPtr); }
			HASIsOK = bReadSPI && bReadSerial;
			Sleep(HASIDwellTime); HASIWaitedTime += HASIDwellTime;
		}
		if (false == HASIsOK) {
			m_ErrorString = GetErrorString();
			return false;
		}
	}
#endif // !HASI_disable

	bool bNewTest=true;
	if ( ExecThreadSequenceFn_InspectProjectKernel(bNewTest) == false ) 
	{	return false;	}	
	
	if ( GetIsSystemException() == true )
	{	return true; }

	if ( true == bMulitDistrictMode )
	{		
		bNewTest = false;
		OFFLINE_IMAGE_SCOPE SaveOfflineImageScope=ProjectPtr->GetProjectSaveOfflineImageScope();
		if ( OFFLINE_IMAGE_PART == SaveOfflineImageScope )
		{	
			CString OfflineFolder = ProjectPtr->GetProjectInspectionOfflineFolder();
			CString OfflineFileName = AOIDataDefine.GetProjectOfflineFileName(OfflineFolder, DistrictID);
			ProjectPtr->SaveProjectInspectionPartOfflineFile(OfflineFileName);
		}

		DISTRICT_ID DistrictID2=DISTRICT_ID_B;
		//if ( MovePCBToDistrictID(LaneID, DistrictID2) == false )		
		if ( WaitForSimpleJobFn_MoveToDistrict() == false )
		{
			WaitForAllCalcThreadStop();
			ProjectPtr->SetProjectIsException(true);
			ProjectPtr->ClearProjectAllTempObjects();
			return false;
		}
		SetIsNeedGrabFiducial(true);
		ProjectPtr->SetProjectTickCountTestFirst_DB();
		ProjectPtr->SetProjectActDistrictID(DistrictID2, true);		
		SetActiveDistrictID(DistrictID2);
		PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH_MAP, NULL);

		if ( ExecThreadSequenceFn_InspectProjectKernel(bNewTest) == false ) 
		{
			WaitForAllCalcThreadStop();
			ProjectPtr->SetProjectIsException(true);
			ProjectPtr->ClearProjectAllTempObjects();
			return false;	
		}	

		/*
		if ( TASK_INSPECT_PROJECT!=TaskMode )
		{
			DISTRICT_ID DistrictID3=DISTRICT_ID_A;
			if ( MovePCBToDistrictID(LaneID, DistrictID3) == false )
			{
				WaitForAllCalcThreadStop();
				ProjectPtr->SetProjectIsException(true);
				ProjectPtr->ClearProjectAllTempObjects();
				return false;
			}
			ProjectPtr->SetProjectActDistrictID(DistrictID3, true);			
			SetActiveDistrictID(DistrictID3);
			PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_SWITCH_MAP, NULL);
			SetIsNeedGrabFiducial(true);
		}
		*/
	}	
	SetIsNeedResetLightCtrlDLP(true);
	if ( GetIsSystemException() == true ) 
	{	return true; }

	//5. 檢測統計等等	
	//WPARAM_INSPECTION_PROJECT_ANALYSIS	
	const bool  ConveyerPreRunRunning = ProjectPtr->GetProjectConveyerPreRunRunning();	
	if ( TASK_INSPECT_PROJECT == TaskMode )
	{
		if ( false == ConveyerPreRunRunning )
		{
			SetOnlineStateMode(ONLINE_STATE_STATICS_PROJECT);
			SetOnlineStateMode_Lane(ONLINE_STATE_STATICS_PROJECT, LaneID);
			ProjectPtr->SetProjectOnlineStateMode(ONLINE_STATE_STATICS_PROJECT);	
		}
	}
	if ( ExecAnalysisProjectInspection(ProjectPtr) == false )
	{	
		ProjectPtr->SetProjectIsException(true);
		ProjectPtr->ClearProjectAllTempObjects();
		return false;	
	}
	ExecRemoveProjectRawImage(ProjectPtr);

	SetAutoRetryCount(0);
	str = _T("Exec Inspect Project Thread End");	
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecThreadSequenceFn_InspectProjectKernel(bool bNewTest) //執行執行緒程序執行緒-專案檢測核心
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	LANE_ID          LaneID = GetActiveLaneID();
	TASK_MODE        TaskMode = GetTaskMode();	
	const UINT MsgInspection = MSG_INSPECTION_CALLBACK;		
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
	TProjectParameter &ProjectParam = ProjectPtr->GetProjectParameter();	

	bool  FdException = false;
	const bool IsNeedGrabFiducial = GetIsNeedGrabFiducial();
	BOARD_FD_GRAB_MODE BoardFdGrabMode = ProjectPtr->GetProjectBoardFdGrabMode();
	FIELD_BUILD_MODE BuildMode = ProjectPtr->GetProjectInspectionFieldBuildMode();	
	FIELD_BUILD_AREA_MODE AreaMode = ProjectPtr->GetProjectInspectionFieldBuildAreaMode();	

	PlcCtrlPtr->TurnOffConveyerSensorPower(LaneID, true);
	if ( true == IsNeedGrabFiducial )
	{	
		//1. 整板定位點
		SetMotionInPositionDelayTime_Fd();
		SetOnlineStateMode(ONLINE_STATE_INSPECT_FD_PANEL);
		SetOnlineStateMode_Lane(ONLINE_STATE_INSPECT_FD_PANEL, LaneID);
		if ( TASK_INSPECT_PROJECT == TaskMode )
		{	ProjectPtr->SetProjectOnlineStateMode(ONLINE_STATE_INSPECT_FD_PANEL);	}
		PostCallbackWndMessage(MsgInspection, WPARAM_INSPECTION_PANEL_FD, NULL);	
		PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);		
		if ( true == bNewTest )//重掃定位點, 因此需要將圖像清除
		{	ProjectPtr->ReleaseProjectInspectionFieldFrameImageBuffer();	} 
		if ( ExecAlignProjectPanelFd(ProjectPtr) == false )//對齊專案整板定位點
		{	
			WaitForAllCalcThreadStop();
			ProjectPtr->SetProjectIsException(true);
			ProjectPtr->ClearProjectAllTempObjects();
			SetMotionInPositionDelayTime_Normal();
			return false; 
		}			
		if ( GetIsSystemException() == true ) 
		{	
			SetMotionInPositionDelayTime_Normal();
			return true; 
		}
	
		//2. 單板定位點		
		FdException = ProjectPtr->GetProjectFdException();
		if ( true == FdException ) 
		{
			SetMotionInPositionDelayTime_Normal();
			return true; 
		}
		if ( BOARD_FD_GRAB_AFTER_PANEL == BoardFdGrabMode )
		{
			SetOnlineStateMode(ONLINE_STATE_INSPECT_FD_BOARD);
			SetOnlineStateMode_Lane(ONLINE_STATE_INSPECT_FD_BOARD, LaneID);
			if ( TASK_INSPECT_PROJECT == TaskMode )
			{	ProjectPtr->SetProjectOnlineStateMode(ONLINE_STATE_INSPECT_FD_BOARD);	}
			PostCallbackWndMessage(MsgInspection, WPARAM_INSPECTION_BOARD_FD, NULL);	
			PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
			if ( ExecAlignProjectBoardFd(ProjectPtr) == false )//對齊專案單板定位點
			{
				WaitForAllCalcThreadStop();
				ProjectPtr->SetProjectIsException(true);
				ProjectPtr->ClearProjectAllTempObjects();
				SetMotionInPositionDelayTime_Normal();
				return false; 
			}		
		}
		SetMotionInPositionDelayTime_Normal();
		if ( GetIsSystemException() == true ) 
		{	return true; }
		FdException = ProjectPtr->GetProjectFdException();
		if ( true == FdException ) 
		{	return true; }
	}
	CString OfflineFolder;
	CString OfflineFdName;
	OFFLINE_FILE_MODE OfflineFileMode = OFFLINE_FILE_INSPECTION;//ProjectPtr->GetProjectOfflineFileMode();	
	SetIsNeedGrabFiducial(false);

	OfflineFolder = ProjectPtr->GetProjectInspectionOfflineFolder();//m_OfflineFolder
	OfflineFdName = AOIDataDefine.GetProjectOfflineFdName(OfflineFolder, DistrictID);
	ProjectPtr->SaveProjectOfflineFdFile(OfflineFdName);
	ProjectPtr->SetProjectOfflineFileMode(OFFLINE_FILE_INSPECTION);	

	//3. FOV/Field Assign	
	PostCallbackWndMessage(MsgInspection, WPARAM_INSPECTION_PROJECT_RESET, NULL);	
	if ( ProjectPtr->AdjustProjectField(OfflineFileMode, BuildMode, AreaMode) == false )
	{	
		SetIsIgnoreAutoRetry(true);
		ProjectPtr->SetProjectIsException(true);
		ProjectPtr->ClearProjectAllTempObjects();
		m_ErrorString = ProjectPtr->GetErrorString();
		return false;
	}	

	if ( ProjectPtr->CheckProjectBarcodeCameraGrabAfterFd() == true )
	{
		//4.1 條碼檢測
		SetOnlineStateMode(ONLINE_STATE_INSPECT_BARCODE);
		SetOnlineStateMode_Lane(ONLINE_STATE_INSPECT_BARCODE, LaneID);
		if ( TASK_INSPECT_PROJECT == TaskMode )
		{	ProjectPtr->SetProjectOnlineStateMode(ONLINE_STATE_INSPECT_BARCODE);	}
		PostCallbackWndMessage(MsgInspection, WPARAM_INSPECTION_BARCODE, NULL);
		PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
		if ( ExecInspectProjectBarcode(ProjectPtr, OfflineFileMode) == false )//對齊專案單板定位點
		{
			WaitForAllCalcThreadStop();
			ProjectPtr->SetProjectIsException(true);
			ProjectPtr->ClearProjectAllTempObjects();
			return false; 
		}		
	}

	//4.2 Xboard 映射檔案。
	XBOARD_MAPPING_FILE_FLOW XBoardMappingFileFlow = ProjectPtr->GetProjectParameter().m_XBoardMappingFileFlow;
	if ( XBOARD_MAPPING_FILE_FLOW_AFTER_BARCODE == XBoardMappingFileFlow ) 
	{
		if ( ExecMESComm_BoardMapping(ProjectPtr) == false )
		{	return false; }
	}

	//4.3 零件檢測
	SetOnlineStateMode(ONLINE_STATE_INSPECT_PROJECT);
	SetOnlineStateMode_Lane(ONLINE_STATE_INSPECT_PROJECT, LaneID);
	if ( TASK_INSPECT_PROJECT == TaskMode )
	{	ProjectPtr->SetProjectOnlineStateMode(ONLINE_STATE_INSPECT_PROJECT);	}	
	PostCallbackWndMessage(MsgInspection, WPARAM_INSPECTION_PROJECT_TEST, NULL);
	PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	if ( ExecInspectProjectComponent(ProjectPtr, OfflineFileMode) == false )//對齊專案單板定位點
	{
		WaitForAllCalcThreadStop();
		ProjectPtr->SetProjectIsException(true);		
		ProjectPtr->ClearProjectAllTempObjects();
		return false; 
	}
	if ( BOARD_FD_GRAB_INSPECTING == ProjectParam.m_BoardFdGrabMode )
	{	ProjectPtr->SaveProjectOfflineFdFile(OfflineFdName); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecThreadSequenceFn_InspectOffline()//執行執行緒程序執行緒-離線檢測	
{
	CString str;
	size_t  FovCount = 0;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }		
	bool bDistrictChange = false;
	LANE_ID LaneID = GetActiveLaneID();
	FIELD_BUILD_MODE BuildMode = ProjectPtr->GetProjectFieldBuildMode();	
	const bool bMulitDistrictMode = ProjectPtr->GetProjectMultiDistrictMode();
	OFFLINE_FILE_MODE OfflineFileMode = ProjectPtr->GetProjectOfflineFileMode();
	const TProjectParameter &ProjectParam = ProjectPtr->GetProjectParameter();
	const UINT MsgInspection = MSG_INSPECTION_CALLBACK;		
	JetMemory.reset_memory_exec_time();	
	
	ClearRepairDateTimeToCheck(LaneID);
	SetLanePCBOutMode(LaneID, ProjectParam.m_PCBOutMode);
	SetLaneDefectHandleMode(LaneID, ProjectParam.m_DefectHandleMode);

	DISTRICT_ID DistrictID = DISTRICT_ID_A;
	if ( true == bMulitDistrictMode )
	{
		DISTRICT_ID LastDistrictID = ProjectPtr->GetProjectActDistrictID();
		if ( DistrictID != LastDistrictID )
		{	bDistrictChange = true;	}		
	}
	SetActiveDistrictID(DistrictID);	
	ProjectPtr->SetProjectActDistrictID(DistrictID, true);	
	if ( ProjectPtr->SwtichProjectOfflineFileMode(OfflineFileMode, bDistrictChange, true) == false )
	{
		this->m_ErrorString = ProjectPtr->GetErrorString();
		return false;
	}

	if ( ProjectPtr->InitProjectInspection(LaneID) == false )
	{
		this->m_ErrorString = ProjectPtr->GetErrorString();
		return false;
	}

	CString TestDateTime;
	JetAPI::GetTime(TestDateTime, ProjectPtr->GetProjectResultCurrent().sDateTimeS);
	str.Format(_T("InitProjectInspection[%s]"), TestDateTime);
	AOIDataCollect.SaveMovingTimeMsg(str);

	CString OfflineFdName = ProjectPtr->GetProjectOfflineFdFilename();
	if ( ProjectPtr->LoadProjectOfflineFdFile(OfflineFdName) == false )
	{
		this->m_ErrorString = ProjectPtr->GetErrorString();
		return false;
	}

#ifdef HAS_DEBUG
	if (true == GetHASI_Enable()) {
		//讀取SerialFile
		if (false == ExecHASI_SaveStateFile()) {
			m_ErrorString = GetErrorString();
			return false;
		}
		if (ExecHASI_SaveAOIJobFile(LaneID) == false)
		{
			m_ErrorString = GetErrorString();
			return false;
		}
		if (ExecHASI_LoadPCBSerialFile(LaneID) == false)
		{
			m_ErrorString = GetErrorString();
			return false;
		}
		if (ExecHASI_LoadSPIOffsetFile(ProjectPtr) == false)
		{
			m_ErrorString = GetErrorString();
			return false;
		}
	}
#endif

	if ( ExecThreadSequenceFn_InspectOfflineKernel() == false ) 
	{	return false;	}
	
	if ( true == bMulitDistrictMode )
	{
		bDistrictChange = true;
		DistrictID = DISTRICT_ID_B;
		SetActiveDistrictID(DistrictID);	
		ProjectPtr->SetProjectActDistrictID(DistrictID, true);	
		if ( ProjectPtr->SwtichProjectOfflineFileMode(OfflineFileMode, bDistrictChange, true) == false )
		{
			this->m_ErrorString = ProjectPtr->GetErrorString();
			return false;
		}
		if ( ExecThreadSequenceFn_InspectOfflineKernel() == false ) 
		{	return false;	}
	}

	//WPARAM_INSPECTION_PROJECT_ANALYSIS	
	if ( ExecAnalysisProjectInspection(ProjectPtr) == false ) 
	{	
		ProjectPtr->ClearProjectAllTempObjects();
		return false;
	}
	SetAutoRetryCount(0);
	//ProjectPtr->ReleaseProjectProgramFieldFrameImageBuffer();//Need To Debug
	//ProjectPtr->ReleaseProjectInspectionFieldFrameImageBuffer();//Need To Debug
	ReleaseModelUniFrameList();

	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecThreadSequenceFn_InspectOfflineKernel()//執行執行緒程序執行緒-離線檢測核心
{
	CString str;
	size_t  FovCount = 0;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }		
	LANE_ID LaneID = GetActiveLaneID();
	FIELD_BUILD_MODE BuildMode = ProjectPtr->GetProjectFieldBuildMode();		
	FIELD_BUILD_AREA_MODE AreaMode = ProjectPtr->GetProjectFieldBuildAreaMode();	
	OFFLINE_FILE_MODE OfflineFileMode = ProjectPtr->GetProjectOfflineFileMode();
	const TProjectParameter &ProjectParam = ProjectPtr->GetProjectParameter();
	const UINT MsgInspection = MSG_INSPECTION_CALLBACK;		

	str = _T("Exec Inspect Offline Thread Start");	
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);	

	//3. FOV/Field Assign	
	PostCallbackWndMessage(MsgInspection, WPARAM_INSPECTION_PROJECT_RESET, NULL);
	if ( ProjectPtr->AdjustProjectField(OfflineFileMode, BuildMode, AreaMode) == false )
	{
		this->m_ErrorString = ProjectPtr->GetErrorString();
		return false;
	}	
	if ( ProjectPtr->AssignProjectField(OfflineFileMode) == false )
	{
		this->m_ErrorString = ProjectPtr->GetErrorString();
		return false;
	}		
	
	//4. 零件檢測
	SetOnlineStateMode(ONLINE_STATE_INSPECT_PROJECT);	
	SetOnlineStateMode_Lane(ONLINE_STATE_INSPECT_PROJECT, LaneID);
	PostCallbackWndMessage(MsgInspection, WPARAM_INSPECTION_PROJECT_TEST, NULL);	
	PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
	if ( ExecInspectProjectOffline(ProjectPtr) == false )//對齊專案單板定位點
	{
		WaitForAllCalcThreadStop();
		ProjectPtr->ClearProjectAllTempObjects();
		return false; 
	}

	//清除暫存的指標列表
	ProjectPtr->ClearProjectAllTempObjects();
	RemoveAllPtrList();		
	
	str = _T("Inspect Project Component End");
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	str += _T("\n");	
	
	str = _T("Exec Inspect Project Thread End");	
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateThreadSequenceThread()//建立執行緒程序執行緒
{
	if ( this->DeleteThreadSequenceThread() == false ) { return false; }	
	this->m_ThreadSequenceThreadCmd = THREAD_COMMAND_TO_IDLE;	

	ThreadSequenceThreadHandle = (HANDLE)::_beginthreadex(NULL, NULL, &ThreadSequenceThreadFn, (void*)0, NULL, &ThreadSequenceThreadID);	
	if ( NULL == ThreadSequenceThreadHandle )
	{	
		this->m_ThreadSequenceThreadState = THREAD_STATE_NONE;
		this->m_ErrorString = _T("Eorror, Create Thread Fault (ThreadSequenceThreadHandle == NULL)");		
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_CREATE);
		return false;
	}			
	ThreadSequenceThreadEvent = JetAPI::CreateEvent(NULL, TRUE, TRUE, _T("Thread Sequence Event"));		
	::SetThreadPriority(ThreadSequenceThreadHandle, THREAD_PRIORITY_BELOW_NORMAL); 
	//::SetThreadAffinityMask(ThreadSequenceThreadHandle, 0x03);//保留2個
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteThreadSequenceThread()//刪除執行緒程序執行緒
{
	DWORD WaitTime = 1000;//1 sec	
	if ( NULL == ThreadSequenceThreadHandle ) { return true; }		
	this->SetThreadSequenceThreadCmd(THREAD_COMMAND_TO_EXIT);	
	DWORD Res = ::WaitForSingleObject(ThreadSequenceThreadHandle, WaitTime);
	::CloseHandle(ThreadSequenceThreadHandle); 
	ThreadSequenceThreadHandle = NULL;
	if ( NULL != ThreadSequenceThreadEvent )
	{	
		::CloseHandle(ThreadSequenceThreadEvent); 
		ThreadSequenceThreadEvent=NULL; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartThreadSequenceThread(bool WaitOn)//開始執行緒程序執行緒
{	
	size_t i=0;	
	if ( NULL == ThreadSequenceThreadHandle ) { return true; }
	if ( WaitForThreadSequenceThreadStop() == false )
	{	return true; }
	if ( NULL != ThreadSequenceThreadEvent )
	{	::ResetEvent(ThreadSequenceThreadEvent); }

	if ( THREAD_STATE_FINISH == m_ThreadSequenceThreadState )
	{	SetThreadSequenceThreadState(THREAD_STATE_IDLE);	}
	
	SetIsOnlineCheckSystemReady(false);
	SetThreadSequenceThreadCmd(THREAD_COMMAND_TO_RUN);	
	if ( THREAD_STATE_FINISH == m_ThreadSequenceThreadState )
	{	i = 2;	}

	if ( false == WaitOn )
	{	return true; }
	if ( WaitForThreadSequenceThreadStart() == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForThreadSequenceThreadIdle()//等待執行緒程序執行緒完成
{
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		//if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_NONE == m_ThreadSequenceThreadState ) { return true; }
		if ( THREAD_STATE_IDLE == m_ThreadSequenceThreadState ) { return true; }
		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }		
	}
	if ( i == MaxCounts )
	{
		this->m_ErrorString.Format(_T("Error, Wait for WaitForThreadSequenceThreadIdle too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE);
		return false;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForThreadSequenceThreadStop()//等待執行緒程序執行緒停止
{
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		//if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_RUNNING != m_ThreadSequenceThreadState ) { return true; }		
		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }		
	}
	if ( i == MaxCounts )
	{
		this->m_ErrorString.Format(_T("Error, Wait for WaitForThreadSequenceThreadStop too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP);
		return false;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForThreadSequenceThreadStart()//等待執行緒程序執行緒開始
{
	size_t i=0;	
	const size_t MaxCount = 100;
	const size_t SleepTime = 10;
	for ( i=0; i<MaxCount; i++ )
	{
		if ( THREAD_COMMAND_TO_RUN != m_ThreadSequenceThreadCmd )//切入下一個階段
		{	break; }
		if ( THREAD_STATE_IDLE != m_ThreadSequenceThreadState )
		{	break; }
		::Sleep(SleepTime);
	}
	if ( i == MaxCount )
	{
		this->m_ErrorString.Format(_T("Error, wait for StartThreadSequenceThread too long"));	
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_START);	
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForThreadSequenceThreadFinish()//等待執行緒程序執行緒結束
{
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;

	if ( ThreadSequenceThreadEvent == NULL ) { return true; }
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == m_ThreadSequenceThreadCmd ) { return true; }		
		if ( THREAD_COMMAND_TO_IDLE == m_ThreadSequenceThreadCmd )
		{	break; }

		Res = ::WaitForSingleObject(ThreadSequenceThreadEvent, SleepTime);
		if ( Res != WAIT_TIMEOUT ) 
		{
			if ( THREAD_STATE_RUNNING == m_ThreadSequenceThreadState )
			{
				if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
				continue; 
			}
			break; 
		}
	}
	if ( i == MaxCounts )
	{
		this->m_ErrorString.Format(_T("Error, Wait for WaitForThreadSequenceThreadFinish too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
		return false;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckThreadSequenceThreadStateFinish()//確認執行緒程序執行緒狀態
{
	if ( m_ThreadSequenceThreadState != THREAD_STATE_FINISH )
	{
		this->m_ErrorString.Format(_T("Error, m_ThreadSequenceThreadState is Exception"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetThreadSequenceThreadState(THREAD_STATE_MODE State)//設定執行緒程序執行緒狀態
{
	if ( m_ThreadSequenceThreadState == State ) { return; }
	LockThreadSequenceThread();
	this->m_ThreadSequenceThreadState = State;
	UnlockThreadSequenceThread();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE  CAOIDataCollect::GetThreadSequenceThreadState()//取得執行緒程序執行緒狀態
{
	return m_ThreadSequenceThreadState;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetThreadSequenceThreadCmd(THREAD_COMMAND_MODE Cmd)//設定執行緒程序執行緒命令
{	
	if ( m_ThreadSequenceThreadCmd == Cmd ) { return; }
	LockThreadSequenceThread();
	m_ThreadSequenceThreadCmd = Cmd;
	UnlockThreadSequenceThread();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CAOIDataCollect::GetThreadSequenceThreadCmd()//取得執行緒程序執行緒命令
{
	return m_ThreadSequenceThreadCmd;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetThreadSequenceThreadElapseTimems(double value)//設定執行緒程序執行緒經過時間
{
	m_ThreadSequenceThreadElapseTimems = value;
}
//-------------------------------------------------------------------------------------//
double CAOIDataCollect::GetThreadSequenceThreadElapseTimems()//取得執行緒程序執行緒經過時間	
{
	return m_ThreadSequenceThreadElapseTimems;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecReleaseFieldFrame()//執行釋放區域影像
{
	size_t       i =0;
	CAOIFrame   *FramePtr = NULL;
	const size_t FrameCount = GetFramePtrCount();	
	for ( i=0; i<FrameCount; i++ )//避免最後一筆無法進來迴圈, 因此手動+1
	{	
		FramePtr = GetFramePtr(i, false);
		if ( NULL == FramePtr ) { continue; }
		FramePtr->ClearFrameBuffer();
		FramePtr->SetFrameCalcState(FRAME_CALC_NONE);
		FramePtr->SetFrameMergeState(FRAME_MERGE_NONE);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecFieldFrameReleaseFn()//執行區域影像釋放執行緒
{
//#ifndef OFFLINE_VERSION
	CString           str;
	size_t            i=0, j=0;	
	size_t            LastIdx=0;
	size_t            FieldIndex = 0;
	size_t            FieldCount = 0;
	size_t            FieldRgnCount = 0;
	size_t            FieldFrameCount = 0;	
	size_t            FieldReleaseCount = 0;
	bool              ContinueCheck = false;
	DWORD             SleepTime = 10;//ms
	const bool        bTickCheck=false;
	CAOIRgn          *RgnPtr = NULL;
	CAOIFrame        *FramePtr = NULL;
	CAOIField        *FieldPtr = NULL;	
	
	//REGION_CALC_STATE RgnCalcState = REGION_CALC_NONE;		
	FIELD_MERGE_STATE FieldMergeState=FIELD_MERGE_NONE;
	FIELD_MERGE_STATE LastFieldMergeState=FIELD_MERGE_NONE;
	THREAD_COMMAND_MODE  ThreadCmd = THREAD_COMMAND_TO_NONE;

	while ( true )
	{		
		ContinueCheck = false;
		ThreadCmd = GetFieldFrameReleaseThreadCmd();
		if ( GetIsSystemReleased() == true ) { return true; }
		if ( GetIsSystemException() == true ) { return true; }
		if ( GetIsAllCalcThreadStop() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }		

		FieldCount = GetFieldPtrCount();
		for ( i=0; i<FieldCount; i++ )
		{
			ThreadCmd = GetFieldFrameReleaseThreadCmd();
			if ( GetIsSystemReleased() == true ) { return true; }				
			if ( GetIsSystemException() == true ) { return true; }
			if ( GetIsAllCalcThreadStop() == true ) { return true; }
			if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
			if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }
			
			FieldPtr = GetFieldPtr(i, false);
			if ( NULL == FieldPtr ) { continue; }
			FieldMergeState = FieldPtr->GetFieldMergeState();
			if ( FIELD_MERGE_CLEAR == FieldMergeState )	{	continue; }			
			if ( FIELD_MERGE_NONE == FieldMergeState ||
				 FIELD_MERGE_DOING == FieldMergeState )
			{	
				ContinueCheck = true;
				JetAPI::SleepTime(SleepTime, bTickCheck);				
				continue; 
			}	

			FieldIndex = FieldPtr->GetFieldIndex();
	
			//同步化
			CAOIField::LockField();
			FieldMergeState = FieldPtr->GetFieldMergeState();//重新取得狀態, 因為在鎖住前可能別的執行緒又進去算了
			if ( FIELD_MERGE_CLEAR == FieldMergeState )
			{
				CAOIField::UnlockField();
				JetAPI::SleepTime(SleepTime, bTickCheck);
				continue;
			}
			LastFieldMergeState = FieldMergeState;
			FieldPtr->SetFieldMergeState(FIELD_MERGE_CLEAR);
			CAOIField::UnlockField();
			
			//Field視野圖有另外儲存執行緒
			if ( FieldPtr->GetFieldLockRelease() == true )
			{
				//ContinueCheck = true;	
				FieldPtr->SetFieldMergeState(LastFieldMergeState);
				//JetAPI::SleepTime(SleepTime, bTickCheck);
				continue;
			}

			//由於有些Field會沒有Rgn所以也要確認Field內的Frame的狀態
			if ( FieldPtr->CheckFieldCalcDone() == false )
			{
				ContinueCheck = true;	
				FieldPtr->SetFieldMergeState(LastFieldMergeState);
				JetAPI::SleepTime(SleepTime, bTickCheck);
				continue;
			}

			//if ( FieldPtr->CheckFieldMergeFinish() == false )			
			if ( FieldPtr->CheckFieldFramesCalcFinish() == false )
			{
				ContinueCheck = true;	
				FieldPtr->SetFieldMergeState(LastFieldMergeState);
				JetAPI::SleepTime(SleepTime, bTickCheck);
				continue;
			}			
			if ( FieldPtr->CheckFieldRgnFinish() == false )
			{
				ContinueCheck = true;	
				FieldPtr->SetFieldMergeState(LastFieldMergeState);
				JetAPI::SleepTime(SleepTime, bTickCheck);
				continue;
			}

			FieldFrameCount = FieldPtr->GetFieldFramePtrCount();
			for ( j=0; j<FieldFrameCount; j++ )
			{
				FramePtr = FieldPtr->GetFieldFramePtr(j, false);
				if ( NULL == FramePtr ) { continue; }
				FramePtr->ClearFrameBuffer();
				CAOIFrame::LockFrame();
				FramePtr->SetFrameMergeState(FRAME_MERGE_CLEAR);
				CAOIFrame::UnlockFrame();
			}
			FieldReleaseCount ++;
			FieldPtr->SetFieldMergeState(FIELD_MERGE_CLEAR);
			JetAPI::SleepTime(SleepTime, bTickCheck);

		#ifdef _DEBUG
			str.Format(_T("Field Frame#%d/%d Released\n"), i+1, FieldCount);
			TRACE(str);
		#endif//_DEBUG
		}
		if ( false == ContinueCheck )
		{	break; }
		JetAPI::SleepTime(SleepTime, bTickCheck);
	};	
	ContinueCheck = ContinueCheck;
//#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateFieldFrameReleaseThread()//建立區域影像釋放執行緒
{
//#ifndef OFFLINE_VERSION
	if ( this->DeleteFieldFrameReleaseThread() == false ) { return false; }	
	this->m_FieldFrameReleaseThreadCmd = THREAD_COMMAND_TO_IDLE;	

	FieldFrameReleaseHandle = (HANDLE)::_beginthreadex(NULL, NULL, &FieldFrameReleaseThreadFn, (void*)0, NULL, &FieldFrameReleaseThreadID);	
	if ( NULL == FieldFrameReleaseHandle )
	{	
		this->m_FieldFrameReleaseThreadState = THREAD_STATE_NONE;
		this->m_ErrorString = _T("Eorror, Create Thread Fault (FieldFrameReleaseHandle == NULL)");		
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_CREATE);
		return false;
	}
	FieldFrameReleaseEvent = JetAPI::CreateEvent(NULL, TRUE, TRUE, _T("Field Frame Release Event"));		
	::SetThreadPriority(FieldFrameReleaseHandle, THREAD_PRIORITY_BELOW_NORMAL); 
	//::SetThreadAffinityMask(FieldFrameReleaseHandle, 0x03);//保留2個		
//#else
//	FieldFrameReleaseEvent = NULL;
//	FieldFrameReleaseHandle = NULL;
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteFieldFrameReleaseThread()//刪除區域影像釋放執行緒
{
//#ifndef OFFLINE_VERSION
	DWORD WaitTime = 1000;//1 sec	
	if ( NULL == FieldFrameReleaseHandle ) { return true; }		
	this->SetFieldFrameReleaseThreadCmd(THREAD_COMMAND_TO_EXIT);	
	DWORD Res = ::WaitForSingleObject(FieldFrameReleaseHandle, WaitTime);
	::CloseHandle(FieldFrameReleaseHandle); 
	FieldFrameReleaseHandle = NULL;
	if ( NULL != FieldFrameReleaseEvent )
	{	
		::CloseHandle(FieldFrameReleaseEvent); 
		FieldFrameReleaseEvent=NULL; 
	}		
//#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StopFieldFrameReleaseThread(bool WaitOn)//停止區域影像釋放執行緒
{
//#ifndef OFFLINE_VERSION
	size_t i=0;
	if ( NULL == FieldFrameReleaseHandle ) { return true; }
	if ( NULL != FieldFrameReleaseEvent )
	{	::ResetEvent(FieldFrameReleaseEvent); }
	SetFieldFrameReleaseThreadCmd(THREAD_COMMAND_TO_IDLE);	

	if ( WaitOn == true ) 
	{	
		const size_t MaxCount = 100;
		const size_t SleepTime = 10;
		for ( i=0; i<MaxCount; i++ )
		{
			//if ( THREAD_COMMAND_TO_IDLE != this->m_FieldFrameReleaseThreadCmd )//切入下一個階段
			//{	break; }
			if ( THREAD_STATE_IDLE == m_FieldFrameReleaseThreadState )
			{	break; }
			if ( THREAD_STATE_NONE == m_FieldFrameReleaseThreadState )
			{	break; }
			//if ( THREAD_STATE_FINISH == m_FieldFrameReleaseThreadState )//有可能後面才完成, 所以要加上完成確認
			//{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			this->m_ErrorString.Format(_T("Error, wait for StopFieldFrameReleaseThread too long"));			
			return false;
		}		
	}
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartFieldFrameReleaseThread(bool WaitOn)//開始區域影像釋放執行緒
{
//#ifndef OFFLINE_VERSION
	size_t i=0;
	if ( NULL == FieldFrameReleaseHandle ) { return true; }
	if ( NULL != FieldFrameReleaseEvent )
	{	::ResetEvent(FieldFrameReleaseEvent); }
	if ( THREAD_STATE_FINISH == m_FieldFrameReleaseThreadState )
	{  SetFieldFrameReleaseThreadState(THREAD_STATE_IDLE); }
	SetFieldFrameReleaseThreadCmd(THREAD_COMMAND_TO_RUN);	

	if ( false == WaitOn )
	{	return true; }
	if ( WaitForFieldFrameReleaseThreadStart() == false )
	{	return false; }	
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForFieldFrameReleaseThreadIdle()//等待區域影像釋放執行緒停止
{
//#ifndef OFFLINE_VERSION		
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;

	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		//if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_NONE == m_FieldFrameReleaseThreadState ) { return true; }
		if ( THREAD_STATE_IDLE == m_FieldFrameReleaseThreadState ) { return true; }
		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }
	}
	if ( i == MaxCounts )
	{
		CString CmdText = GetThreadCommandModeText(m_FieldFrameReleaseThreadCmd);
		CString StateText = GetThreadStateModeText(m_FieldFrameReleaseThreadState);
		m_ErrorString.Format(_T("Error, WaitForFieldFrameReleaseThreadIdle[Cmd=%s, State=%s] too long"), CmdText, StateText);
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE);
		return false;
	}		
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForFieldFrameReleaseThreadStop()//等待區域影像釋放執行緒停止
{
//#ifndef OFFLINE_VERSION		
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;

	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		//if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_RUNNING != m_FieldFrameReleaseThreadState ) { return true; }		
		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }
	}
	if ( i == MaxCounts )
	{
		CString CmdText = GetThreadCommandModeText(m_FieldFrameReleaseThreadCmd);
		CString StateText = GetThreadStateModeText(m_FieldFrameReleaseThreadState);
		m_ErrorString.Format(_T("Error, WaitForFieldFrameReleaseThreadStop[Cmd=%s, State=%s] too long"), CmdText, StateText);
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP);
		return false;
	}		
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForFieldFrameReleaseThreadStart()//等待區域影像釋放執行緒開始
{
//#ifndef OFFLINE_VERSION
	size_t i=0;
	const size_t MaxCount = 100;
	const size_t SleepTime = 10;
	for ( i=0; i<MaxCount; i++ )
	{
		if ( THREAD_COMMAND_TO_RUN != m_FieldFrameReleaseThreadCmd )//切入下一個階段
		{	break; }
		if ( THREAD_STATE_IDLE != m_FieldFrameReleaseThreadState )
		{	break; }
		::Sleep(SleepTime);
	}
	if ( i == MaxCount )
	{
		CString CmdText = GetThreadCommandModeText(m_FieldFrameReleaseThreadCmd);
		CString StateText = GetThreadStateModeText(m_FieldFrameReleaseThreadState);
		m_ErrorString.Format(_T("Error, WaitForFieldFrameReleaseThreadStart[Cmd=%s, State=%s] too long"), CmdText, StateText);
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_START);	
		return false;
	}
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForFieldFrameReleaseThreadFinish()//等待區域影像釋放執行緒結束
{
//#ifndef OFFLINE_VERSION		
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;

	if ( FieldFrameReleaseEvent == NULL ) { return true; }
	SaveDebugMessage(_T("WaitForFieldFrameReleaseThreadFinish-Start"));
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == m_FieldFrameReleaseThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == m_FieldFrameReleaseThreadCmd )
		{	break; }

		Res = ::WaitForSingleObject(FieldFrameReleaseEvent, SleepTime);
		if ( Res != WAIT_TIMEOUT ) 
		{
			if ( THREAD_STATE_RUNNING == m_FieldFrameReleaseThreadState )
			{
				if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
				continue; 
			}
			break; 
		}
	}
	if ( i == MaxCounts )
	{
		CString CmdText = GetThreadCommandModeText(m_FieldFrameReleaseThreadCmd);
		CString StateText = GetThreadStateModeText(m_FieldFrameReleaseThreadState);
		SaveDebugMessage(_T("WaitForFieldFrameReleaseThreadFinish-Fault"));
		m_ErrorString.Format(_T("Error, WaitForFieldFrameReleaseThreadFinish[Cmd=%s, State=%s] too long"), CmdText, StateText);
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
		return false;
	}		
	SaveDebugMessage(_T("WaitForFieldFrameReleaseThreadFinish-End"));
//#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckFieldFrameReleaseThreadStateFinish()//確認區域影像釋放執行緒狀態
{
	if ( NULL == FieldFrameReleaseHandle ) { return true; }
	if ( THREAD_COMMAND_TO_NONE == m_FieldFrameReleaseThreadCmd ) { return true; }
	if ( THREAD_COMMAND_TO_EXIT == m_FieldFrameReleaseThreadCmd ) { return true; }
	if ( THREAD_COMMAND_TO_IDLE == m_FieldFrameReleaseThreadCmd ) { return true; }
	if ( m_FieldFrameReleaseThreadState != THREAD_STATE_FINISH )
	{
		this->m_ErrorString.Format(_T("Error, m_FieldFrameReleaseThreadState# is Exception"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetFieldFrameReleaseThreadState(THREAD_STATE_MODE State)//設定區域影像釋放執行緒狀態
{
	if ( m_FieldFrameReleaseThreadState == State ) { return; }
	LockThreadFrameRelease();
	this->m_FieldFrameReleaseThreadState = State;
	UnlockThreadFrameRelease();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE  CAOIDataCollect::GetFieldFrameReleaseThreadState()//取得區域影像釋放執行緒狀態
{
	return m_FieldFrameReleaseThreadState;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetFieldFrameReleaseThreadCmd(THREAD_COMMAND_MODE Cmd)//設定區域影像釋放執行緒命令
{
	if ( m_FieldFrameReleaseThreadCmd == Cmd ) { return; }
	LockThreadFrameRelease();
	this->m_FieldFrameReleaseThreadCmd = Cmd;
	UnlockThreadFrameRelease();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CAOIDataCollect::GetFieldFrameReleaseThreadCmd()//取得區域影像釋放執行緒命令
{
	return m_FieldFrameReleaseThreadCmd;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetFieldFrameReleaseThreadNeedCheck(bool Val)//設定區域影像釋放執行緒需要確認
{
	m_FieldFrameReleaseThreadNeedCheck = Val;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetFieldFrameReleaseThreadNeedCheck() const//取得區域影像釋放執行緒需要確認
{
	return m_FieldFrameReleaseThreadNeedCheck;
}
//-------------------------------------------------------------------------------------//	
bool CAOIDataCollect::ExecOnlineInspectionFn()//線上檢測執行緒
{
#ifndef OFFLINE_VERSION
	CString              str;
	CString              strLaneID;
	CString              filename;
	size_t               i=0, j=0;
	POINT                MousePosLast;
	bool                 bFirstUI=true;
	DWORD                SleepTime = 10;//ms	
	DWORD                SleepTime00 = 0;//ms	
	DWORD                SleepTime01 = 10;//ms	
	DWORD                SleepTime05 = 50;//ms	
	DWORD                SleepTime10 =100;//ms		
	DWORD                TickCountRepeat=0;
	size_t               ProjectCount=0;	
	TOnlineProcParam     OnlineProcParam;
	int                  PLCPCBAutoRunMode=0;
	LANE_ID              LaneID = LANE_ID_A;	
	LANE_ID              NextLaneID = LANE_ID_A;
	bool                 IsNeedReCheckPCBInside = false;
	PCB_OUT_MODE         PCBOutMode=PCB_OUT_NORMAL;
	double               StagePosX=0, StagePosY=0, StagePosZ=0;	
	bool                 bOnlineProcRet=false;
	bool                 AutoRunLane_LA=false;
	bool                 AutoRunLane_LB=false;
	bool                 BypassLastSignal=false;
	bool                 BypassNextSignal=false;
	bool                 IsOnlineCheckSystemReady=true;
	THREAD_COMMAND_MODE  ThreadCmd = THREAD_COMMAND_TO_NONE;
	TASK_MODE            TaskMode = TASK_NONE;	
	TASK_STATE_MODE      TaskStateMode;	
	WND_MESSAGE_MODE     WndMessageMode=WND_MESSAGE_POST;
	PCB_OUT_DIRECTION    PCBOutDirection;	
	MULTI_LANE_MODE      MultiLaneMode = MULTI_LANE_1;
	LANE_WORK_MODE       LaneWorkMode_LA = LANE_WORK_RETURN;
	LANE_WORK_MODE       LaneWorkMode_LB = LANE_WORK_RETURN;	
	ONLINE_STATE_MODE    OldOnlineState = ONLINE_STATE_RETURN;
	ONLINE_STATE_MODE    NewOnlineState = ONLINE_STATE_RETURN;	
	LANE_STATE_MODE      TaskLaneStateMode = LANE_STATE_NONE;
	bool                 SensorPcbStop=false;	
	bool                 SensorPcbSlow=false;
	bool                 SensorPcbOut=false;
	bool                 SensorPcbIn=false;
	bool                 SensorPcbStop_LA=false;
	bool                 SensorPcbStop_LB=false;
	const bool           bAutoReset = false;
	const bool           bChkStartLight = true;
	//CAOIProject         *ProjectPtr = NULL;

	SleepTime = SleepTime01;
	while ( true )
	{	
		ThreadCmd = GetOnlineInspectionThreadCmd();
		if ( GetIsSystemReleased() == true ) { return true; }
		if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }		
		if ( SleepTime > 0 ) 
		{	::Sleep(SleepTime); }
		//ProjectPtr = GetActiveProject();		
		TaskMode = GetTaskMode();
		LaneID = GetActiveLaneID();		
		PCBOutMode = GetLanePCBOutMode(LaneID);
		MultiLaneMode = CheckMultiLaneMode();
		LaneWorkMode_LA = GetLaneWorkMode_LA();
		LaneWorkMode_LB = GetLaneWorkMode_LB();		
		TaskStateMode = GetOnlineTaskState();		
		NewOnlineState = OldOnlineState = GetOnlineStateMode();
		PCBOutDirection = GetPCBOutDirection();	
		BypassLastSignal = GetBypassLastSignal();
		BypassNextSignal = GetBypassNextSignal();
		TaskLaneStateMode = GetOnlineLaneTaskState(LaneID);
		LANE_STATE_MODE TaskLaneStateMode_LA = GetOnlineLaneTaskState(LANE_ID_A);
		LANE_STATE_MODE TaskLaneStateMode_LB = GetOnlineLaneTaskState(LANE_ID_B);

		OnlineProcParam.eTaskMode = GetTaskMode();
		OnlineProcParam.eLaneID = GetActiveLaneID();
		OnlineProcParam.eLaneIDNext = OnlineProcParam.eLaneID;
		OnlineProcParam.ePCBOutMode = GetLanePCBOutMode(OnlineProcParam.eLaneID);
		OnlineProcParam.eFromMode = ONLINE_FROM_ONLINE_THREAD;
		OnlineProcParam.eMultiLaneMode = CheckMultiLaneMode();
		OnlineProcParam.eLaneWorkModeLA = GetLaneWorkMode_LA();
		OnlineProcParam.eLaneWorkModeLB = GetLaneWorkMode_LB();
		OnlineProcParam.bBypassLastSignal = GetBypassLastSignal();
		OnlineProcParam.bBypassNextSignal = GetBypassNextSignal();
		OnlineProcParam.ePCBOutDirection = GetPCBOutDirection();
		OnlineProcParam.eOnlineStateOld = GetOnlineStateMode();
		OnlineProcParam.nLastStationLineMode = GetLastStationLineMode();
		OnlineProcParam.eOnlineStateNew = OnlineProcParam.eOnlineStateOld;
		OnlineProcParam.dwSleepTime = SleepTime;
		OnlineProcParam.eWndMessageMode = WndMessageMode;
		OnlineProcParam.bConveyerPreRunMode = false;

		IsOnlineCheckSystemReady=GetIsOnlineCheckSystemReady();		
		if ( true == IsOnlineCheckSystemReady )//不是檢測過程中才去確認系統狀態
		{
			if ( CheckSystemReady(bChkStartLight, bAutoReset) == false )
			{	return false; }
		}

		if ( TASK_STATE_TO_ABORT == TaskStateMode )
		{
			//SetIsSystemException(true);
			SetErrorString(_T("Error, Inspection Abort"));
			return false;
		}

		//str.Format(_T("debug:: OnlineState: %d,Lane: %d, Lane1: %d, Lane2: %d"),
		//	OldOnlineState, GetActiveLaneID(), TaskLaneStateMode_LA, TaskLaneStateMode_LB);
		//SaveMovingTimeMsg(str);
		switch ( OldOnlineState )
		{
		case ONLINE_STATE_INSPECTION_STOP://停止檢測			
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_INSPECTION_STOP"));
			if ( ExecOnlineProcInspectionStop(OnlineProcParam) == false ) 
			{	return false;	}
			SleepTime = OnlineProcParam.dwSleepTime;
			NextLaneID = OnlineProcParam.eLaneIDNext;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;			
			break;
		case ONLINE_STATE_PCB_READY://PCB就緒
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_READY"));	
			if (LANE_STATE_PAUSE == TaskLaneStateMode) {
				ExecOnlineProcPCBDualRunPauseOne(OnlineProcParam);
				NewOnlineState = OnlineProcParam.eOnlineStateNew;
				break;
			}
			if ( ExecOnlineProcPCBReady(OnlineProcParam) == false ) 
			{	return false;	}
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;	
			break;
		case ONLINE_STATE_INPUT_BARCODE://輸入條碼			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_INPUT_BARCODE"));	
			if ( ExecOnlineProcInputBarcode(OnlineProcParam) == false ) 
			{	return false;	}
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PROJECT_MARK://專案標記			
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PROJECT_MARK"));	
			if ( ExecOnlineProcInspectProjectMark(OnlineProcParam) == false ) 
			{	return false;	}
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PROJECT_OPEN_CODE://開啟專案條碼
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PROJECT_OPEN_CODE"));	
			if ( ExecOnlineProcInspectProjectOpenCode(OnlineProcParam) == false ) 
			{	return false;	}
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;			
			break;
		case ONLINE_STATE_INSPECTION_START://開始檢測			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}			
			if (LANE_STATE_PAUSE == TaskLaneStateMode) {
				ExecOnlineProcPCBDualRunPauseOne(OnlineProcParam);
				NewOnlineState = OnlineProcParam.eOnlineStateNew;
				break;
			}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_INSPECTION_START"));	
			if ( ExecMESComm_ProcessID(MES_STATAUS_AOI_START_INSPECTION_CMD) == false ) 
			{	return false; }	

			if ( ExecOnlineProcInspectionStart(OnlineProcParam) == false ) 
			{
				SetIsIgnoreAutoRetry(true);
				return false;	
			}
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_INSPECTION_WAITING:			
			break;
		case ONLINE_STATE_INSPECT_FD_PANEL://定位整板
			break;
		case ONLINE_STATE_INSPECT_FD_BOARD://定位單板
			break;
		case ONLINE_STATE_INSPECT_BARCODE://檢測條碼
			break;
		case ONLINE_STATE_INSPECT_PROJECT://檢測專案
			if ( ExecOnlineProcInspectProject(OnlineProcParam) == false)
			{	return false;	}
			break;
		case ONLINE_STATE_STATICS_PROJECT://統計專案
			break;
		case ONLINE_STATE_INSPECTION_FINISH://檢測結束
			if ( ExecMESComm_ProcessID(MES_STATAUS_AOI_INSPECTION_COMPLETE_CMD) == false ) 
			{	return false; }
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{	
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			if (LANE_STATE_PAUSE == TaskLaneStateMode) {
				ExecOnlineProcPCBDualRunPauseOne(OnlineProcParam);
				NewOnlineState = OnlineProcParam.eOnlineStateNew;
				break;
			}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_INSPECTION_FINISH"));	
			OnlineProcParam.eOnlineStateNew = GetOnlineStateMode_Next();
			if ( ExecOnlineProcInspectionFinish(OnlineProcParam) == false ) 
			{
				if ( MULTI_LANE_2 == MultiLaneMode )//雙軌道模式
				{					
					NewOnlineState = ONLINE_STATE_INSPECTION_STOP;
					SetOnlineStateMode(NewOnlineState);
					//SetOnlineStateMode_Lane(NewOnlineState, LaneID);
				}	
				return false;	
			}
			SleepTime = OnlineProcParam.dwSleepTime;
			NextLaneID = OnlineProcParam.eLaneIDNext;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PROJECT_SWITCH_BY_TURN://專案切換-輪流
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			if ( ExecOnlineProcProjectSwitchByTurn(OnlineProcParam) == false )
			{	return false;	}					
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PROJECT_SWITCH_BY_TURN_ONE_CYCLE_RESET://專案切換-重設輪流一次檢測
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }				
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			if ( ExecOnlineProcProjectSwitchByTurnOneCycleReset(OnlineProcParam) == false )
			{	return false;	}					
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_WAIT_FOR_LAST_STATION://等待上一站訊號
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			if (LANE_STATE_PAUSE == TaskLaneStateMode) {
				ExecOnlineProcPCBDualRunPauseOne(OnlineProcParam);
				NewOnlineState = OnlineProcParam.eOnlineStateNew;
				break;
			}
			if ( ExecOnlineProcWaitForLastStation(OnlineProcParam) == false )
			{	return false;	}					
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_WAIT_FOR_NEXT_STATION://等待下一站訊號
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			if (LANE_STATE_PAUSE == TaskLaneStateMode) {
				ExecOnlineProcPCBDualRunPauseOne(OnlineProcParam);
				NewOnlineState = OnlineProcParam.eOnlineStateNew;
				break;
			}
			if ( ExecOnlineProcWaitForNextStation(OnlineProcParam) == false ) 
			{	return false; }			
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_IN_START://進板開始			
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_IN_START"));	
			if ( ExecMESComm_ProcessID(MES_STATAUS_AOI_READY_TO_LOAD_CMD) == false ) 
			{	return false; }
			if ( ExecOnlineProcPCBInStart(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_IN_CHECKING://進板確認
			if ( ExecOnlineProcPCBInChecking(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;			
			break;
		case ONLINE_STATE_PCB_IN_FINISH://進板結束
			if ( ExecMESComm_ProcessID(MES_STATAUS_AOI_LOAd_COMPLETE_CMD) == false ) 
			{	return false; }
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{	
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_IN_FINISH"));	
			if ( ExecOnlineProcPCBInFinish(OnlineProcParam) == false ) 
			{	return false; }			
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_OUT_START://出板開始
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_OUT_START"));	
			if ( ExecMESComm_ProcessID(MES_STATAUS_AOI_READY_TO_UNLOAD_CMD) == false ) 
			{	return false; }
			if ( ExecOnlineProcPCBOutStart(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_OUT_CHECKING://出板確認
			if ( ExecOnlineProcPCBOutChecking(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;			
			break;
		case ONLINE_STATE_PCB_OUT_FINISH://出板結束
			if ( ExecMESComm_ProcessID(MES_STATAUS_AOI_UNLOAd_COMPLETE_CMD) == false ) 
			{	return false; }
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_OUT_FINISH"));	
			if ( ExecOnlineProcPCBOutFinish(OnlineProcParam) == false ) 
			{	return false; }			
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;		
		case ONLINE_STATE_PCB_OUT_INSIDE_START://停側邊開始
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_OUT_INSIDE_START"));	
			if ( ExecOnlineProcPCBOutInsideStart(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_OUT_INSIDE_CHECKING://停側邊確認
			if ( ExecOnlineProcPCBOutInsideChecking(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_OUT_INSIDE_FINISH://停側邊結束
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_OUT_INSIDE_FINISH"));	
			if ( ExecOnlineProcPCBOutInsideFinish(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;			
		case ONLINE_STATE_PCB_BACK_START://退板開始
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_BACK_START"));	
			if ( ExecMESComm_ProcessID(MES_STATAUS_AOI_READY_TO_UNLOAD_CMD) == false ) 
			{	return false; }
			if ( ExecOnlineProcPCBBackStart(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_BACK_CHECKING://退板確認
			if ( ExecOnlineProcPCBBackChecking(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_BACK_FINISH://退板結束
			if ( ExecMESComm_ProcessID(MES_STATAUS_AOI_UNLOAd_COMPLETE_CMD) == false ) 
			{	return false; }
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_BACK_FINISH"));			
			if ( ExecOnlineProcPCBBackFinish(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_BACK_OUT_START://退出板開始
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_BACK_OUT_START"));	
			if ( ExecMESComm_ProcessID(MES_STATAUS_AOI_READY_TO_UNLOAD_CMD) == false ) 
			{	return false; }
			if ( ExecOnlineProcPCBBackOutStart(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_BACK_OUT_CHECKING://退出板確認
			if ( ExecOnlineProcPCBBackOutChecking(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_BACK_OUT_FINISH://退出板結束
			if ( ExecMESComm_ProcessID(MES_STATAUS_AOI_UNLOAd_COMPLETE_CMD) == false ) 
			{	return false; }
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_BACK_OUT_FINISH"));			
			if ( ExecOnlineProcPCBBackOutFinish(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_WAIT_FOR_PCB_REMOVED://等待板子移走
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			if ( ExecOnlineProcWaitForPCBRemoved(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_WAIT_FOR_REPAIR_VERIFY://等待維修站確認
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }				
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}			
			if (LANE_STATE_PAUSE == TaskLaneStateMode) {
				ExecOnlineProcPCBDualRunPauseOne(OnlineProcParam);
				NewOnlineState = OnlineProcParam.eOnlineStateNew;
				break;
			}
			if ( ExecOnlineProcWaitForRepairVerify(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_OUT_IN_START://出板帶進板開始
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_OUT_IN_START"));	
			if ( ExecOnlineProcPCBOutInStart(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;			
			break;
		case ONLINE_STATE_PCB_OUT_IN_CHECKING://出板帶進板確認	
			if ( ExecOnlineProcPCBOutInChecking(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;			
			break;
		case ONLINE_STATE_PCB_OUT_IN_FINISH://出板帶進板結束
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_OUT_IN_FINISH"));	
			if ( ExecOnlineProcPCBOutInFinish(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;			
			break;		
		case ONLINE_STATE_PCB_AUTO_RUN_START:
			if ( PCB_OUT_DIR_BACKWARD == PCBOutDirection )
			{	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_AUTO_RUN_START-BACK"));	}
			else if ( PCB_OUT_DIR_BACKWARD_OUT == PCBOutDirection )
			{	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_AUTO_RUN_START-BACK-OUT"));	}
			else
			{	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_AUTO_RUN_START"));	}			
			if ( ExecOnlineProcPCBAutoRunStart(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;			
			break;
		case ONLINE_STATE_PCB_AUTO_RUN_CHECKING:
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{	
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;					
			}
			if ( ExecOnlineProcPCBAutoRunChecking(OnlineProcParam) == false ) 
			{
				NewOnlineState = ONLINE_STATE_INSPECTION_STOP;
				SetOnlineStateMode(NewOnlineState);
				//SetOnlineStateMode_Lane(NewOnlineState, LaneID);
				return false; 
			}
			LaneID = OnlineProcParam.eLaneID;
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_AUTO_RUN_FINISH:
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_AUTO_RUN_FINISH"));
			if ( ExecOnlineProcPCBAutoRunFinish(OnlineProcParam) == false ) 
			{	return false;	}
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;			
			break;
		case ONLINE_STATE_PCB_DUAL_RUN_START:			
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_DUAL_RUN_START"));
			if ( ExecOnlineProcPCBDualRunStart(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_DUAL_RUN_CHECKING:
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{	
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;					
			}
			//if (LANE_STATE_PAUSE == TaskLaneStateMode) {
			//	ExecOnlineProcPCBDualRunPauseOne(OnlineProcParam);
			//	NewOnlineState = OnlineProcParam.eOnlineStateNew;
			//	break;
			//}
			if ( ExecOnlineProcPCBDualRunChecking(OnlineProcParam) == false ) 
			{
				NewOnlineState = ONLINE_STATE_INSPECTION_STOP;
				SetOnlineStateMode(NewOnlineState);
				//SetOnlineStateMode_Lane(NewOnlineState, LaneID);
				return false; 
			}
			LaneID = OnlineProcParam.eLaneID;
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_DUAL_RUN_FINISH:
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_DUAL_RUN_FINISH"));
			if ( ExecOnlineProcPCBDualRunFinish(OnlineProcParam) == false ) 
			{	return false;	}
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_INSPECTION_PAUSE:
			if (TASK_STATE_TO_STOP == TaskStateMode)
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;
			}
			if (false == ExecOnlineProcPCBDualRunResume(OnlineProcParam))
			{	return false;	}
			LaneID = OnlineProcParam.eLaneID;
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_AUTO_CALIBRATION_XYZ_HOME://自動校正-XYZ歸零
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_AUTO_CALIBRATION_XYZ_HOME"));
			if ( ExecOnlineProcAutoCalibration_XYZ_Home(OnlineProcParam) == false ) 
			{	return false;	}
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;			
			break;
		case ONLINE_STATE_AUTO_CALIBRATION_2D_CURRENT://自動校正-2D電流
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_AUTO_CALIBRATION_2D_CURRENT"));
			if ( ExecOnlineProcAutoCalibration_2D_Current(OnlineProcParam) == false ) 
			{	return false;	}
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_AUTO_CALIBRATION_3D_CURRENT://自動校正-3D電流
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_AUTO_CALIBRATION_3D_CURRENT"));
			if ( ExecOnlineProcAutoCalibration_3D_Current(OnlineProcParam) == false ) 
			{	return false;	}
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_AUTO_CALIBRATION_3D_ZERO_PLANE://自動校正-3D相平面
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_AUTO_CALIBRATION_3D_ZERO_PLANE"));
			if ( ExecOnlineProcAutoCalibration_3D_ZeroPlane(OnlineProcParam) == false ) 
			{	return false;	}
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_AUTO_CALIBRATION_3D_HEIGHT_FACTOR://自動校正-3D高度比例
			if ( ProcesMESRecvNodeList(false ) == false )
			{	return false; }			
			if ( TASK_STATE_TO_STOP == TaskStateMode )
			{
				ExecOnlineInspectionStopFn(OldOnlineState);
				return true;	
			}
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_AUTO_CALIBRATION_3D_HEIGHT_FACTOR"));
			if ( ExecOnlineProcAutoCalibration_3D_HeightFactor(OnlineProcParam) == false ) 
			{	return false;	}
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		}		
		if ( OldOnlineState!=NewOnlineState || true==bFirstUI )
		{	
			SetOnlineStateMode(NewOnlineState);
			SetOnlineStateMode_Lane(NewOnlineState, LaneID);
			SaveAOIMonitorStatus(NewOnlineState, NULL);
			ExecMESComm_SetMachineStatus(NewOnlineState);
			WndMessageMode = OnlineProcParam.eWndMessageMode;
			if ( WND_MESSAGE_POST == WndMessageMode )
			{	PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL); }
			else
			{	SendCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL); }

			switch ( NewOnlineState )
			{
			case ONLINE_STATE_INSPECTION_WAITING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_INSPECTION_WAITING"));
				break;
			case ONLINE_STATE_WAIT_FOR_PCB_REMOVED:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_WAIT_FOR_PCB_REMOVED"));
				break;
			case ONLINE_STATE_WAIT_FOR_LAST_STATION:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_WAIT_FOR_LAST_STATION"));
				break;
			case ONLINE_STATE_WAIT_FOR_NEXT_STATION:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_WAIT_FOR_NEXT_STATION"));
				break;
			case ONLINE_STATE_PCB_IN_CHECKING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_IN_CHECKING"));
				break;
			case ONLINE_STATE_PCB_OUT_CHECKING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_OUT_CHECKING"));
				break;
			case ONLINE_STATE_PCB_OUT_INSIDE_CHECKING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_OUT_INSIDE_CHECKING"));
				break;
			case ONLINE_STATE_PCB_BACK_CHECKING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_BACK_CHECKING"));
				break;
			case ONLINE_STATE_PCB_BACK_OUT_CHECKING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_BACK_OUT_CHECKING"));
				break;
			case ONLINE_STATE_PCB_OUT_IN_CHECKING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_OUT_IN_CHECKING"));
				break;
			case ONLINE_STATE_PCB_AUTO_RUN_CHECKING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_AUTO_RUN_CHECKING"));
				break;
			case ONLINE_STATE_PCB_DUAL_RUN_CHECKING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, _T("ONLINE_STATE_PCB_DUAL_RUN_CHECKING"));
				break;
			}	
		}
		if ( OldOnlineState != NewOnlineState )
		{	
			TickCountRepeat=0;	
			MousePosLast.x = MousePosLast.y = 0;
		}
		else
		{			
			if ( 0 == TickCountRepeat )
			{
				::GetCursorPos(&MousePosLast);
				TickCountRepeat = ::GetTickCount();	
			}
			else
			{
				DWORD TickCountGap = ::GetTickCount()-TickCountRepeat;
				if ( TickCountGap > 1000 )
				{
					//CString str;
					POINT MousePosNow;			
					::GetCursorPos(&MousePosNow);
					TickCountRepeat = ::GetTickCount();
					if ( MousePosNow.x!=MousePosLast.x || MousePosNow.y!=MousePosLast.y )//20260402
					{	str.Format(_T("ExecOnlineInspectionFn Repeat[%s]"), AOIDataDefine.GetOnlineStateText(NewOnlineState));	}
					else
					{	
						mouse_event(MOUSEEVENTF_MOVE, 0, 0, 0, 0 );	
						str.Format(_T("ExecOnlineInspectionFn Repeat[%s]-AutoMoveMouse"), AOIDataDefine.GetOnlineStateText(NewOnlineState));
					}
					SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
					MousePosLast = MousePosNow;
				}			
			}
		}
		bFirstUI = false;
		ExecOnlineProcAutoStopByIdleTime();
		ExecOnlineProcAutoStopBySpecTime();
	};
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineInspectionStopFn(ONLINE_STATE_MODE State)//執行線上檢測停止函式
{	
	bool            bPCBStop=false;
	bool            bPCBChanged=false;
	bool            bAutoRunFinish=false;	
	LANE_ID         LaneID = LANE_ID_A;
	int             PLCPCBAutoRunMode=0;	
	PCB_OUT_MODE    PCBOutMode;
	std::vector<LANE_ID> LaneIDList;
	ONLINE_STATE_MODE OnlineStageMode;
	MULTI_LANE_MODE MultiLaneMode = CheckMultiLaneMode();
	ONLINE_STATE_MODE OnlineStageMode_LA=GetOnlineStateMode_LA();	
	ONLINE_STATE_MODE OnlineStageMode_LB=GetOnlineStateMode_LB();	
	const int       PLCPCBAutoRunModeStop = PLC_PCB_AUTO_RUN_STOP;

	LaneID = GetActiveLaneID();
	SetOnlineStateMode_Lane(State, LaneID);//20240402
	SaveMovingTimeMsg(_T("CAOIDataCollect::ExecOnlineInspectionStopFn Start"));	
	if ( MULTI_LANE_2 == MultiLaneMode )//雙軌道模式
	{
		LaneID = LANE_ID_A;
		PCBOutMode = GetLanePCBOutMode(LaneID);
		PlcCtrlPtr->TurnOffInspectAlarm(LaneID);
		if ( PCB_OUT_LANE_AUTO == PCBOutMode )
		{	LaneIDList.push_back(LaneID); }

		LaneID = LANE_ID_B;
		PCBOutMode = GetLanePCBOutMode(LaneID);
		PlcCtrlPtr->TurnOffInspectAlarm(LaneID);
		if ( PCB_OUT_LANE_AUTO == PCBOutMode )
		{	LaneIDList.push_back(LaneID); }		
	}
	else
	{	
		LaneID = GetActiveLaneID();
		PCBOutMode = GetLanePCBOutMode(LaneID);
		PlcCtrlPtr->TurnOffInspectAlarm(LaneID);
		if ( ONLINE_STATE_PCB_AUTO_RUN_CHECKING == State )
		{
			if ( PCB_OUT_LANE_AUTO == PCBOutMode )
			{	LaneIDList.push_back(LaneID); }
		}
	}

	size_t i=0;
	const size_t LaneIDCnt=LaneIDList.size();
	for ( i=0; i<LaneIDCnt; i++ )
	{
		bPCBStop=true;
		bPCBChanged=false;		
		bAutoRunFinish=false;
		LaneID = LaneIDList[i];
		PLCPCBAutoRunMode = PlcCtrlPtr->GetPCBAutoRunMode(LaneID);
		if ( PLC_PCB_AUTO_RUN_OUT_IN == PLCPCBAutoRunMode ) 
		{	
			PlcCtrlPtr->StopPCBAutoOutIn(LaneID, true);				
			if ( PlcCtrlPtr->WaitForConveryerStopRunning(LaneID) == true )
			{
				::Sleep(100);
				bAutoRunFinish=PlcCtrlPtr->CheckPCBAutoOutInFinish(LaneID);
			}
		}
		if ( PLC_PCB_AUTO_RUN_BACK_IN == PLCPCBAutoRunMode ) 
		{	
			bAutoRunFinish=true;
			PlcCtrlPtr->WaitForPCBAutoBackInFinish(LaneID);
			//PlcCtrlPtr->StopPCBAutoBackIn(LaneID, true);	
		}
		PlcCtrlPtr->PLC_ReadConveryerSensor(false);
		bPCBStop = PlcCtrlPtr->GetConveryerSensorPCBStop(LaneID);
		bPCBChanged = PlcCtrlPtr->GetPCBAutoRunPCBChaned(LaneID);		
		if ( false == bPCBStop )
		{	OnlineStageMode = ONLINE_STATE_INSPECTION_STOP;	}
		else
		{
			if ( true==bAutoRunFinish || true==bPCBChanged )
			{	OnlineStageMode = ONLINE_STATE_PCB_READY;	}
			else
			{	OnlineStageMode = ONLINE_STATE_INSPECTION_FINISH; }
		}
		SetOnlineStateMode_Lane(OnlineStageMode, LaneID);
		PlcCtrlPtr->SetPCBAutoRunMode(LaneID, PLCPCBAutoRunModeStop);

		SetOnlineStateMode(ONLINE_STATE_INSPECTION_STOP);				
	}
	if ( StopConveyerAutoRunThread(true) == false )
	{ return false; }
	ResetSwitchMultiLine_NextLaneID();	
	PlcCtrlPtr->UpdateTowerLightState_Stop();
	SaveAOIMonitorStatus(ONLINE_STATE_INSPECTION_STOP, NULL);	
	
	//Turn Off Barcode Device
	if ( MULTI_LANE_2 == MultiLaneMode )//雙軌道模式
	{
		ExecBarcodeDeviceEndReading(LANE_ID_A);
		ExecBarcodeDeviceEndReading(LANE_ID_B);		
	}
	else
	{
		LaneID = GetActiveLaneID();
		ExecBarcodeDeviceEndReading(LaneID);		
	}
	if ( ExecMESComm_ProcessID(MES_STATAUS_AOI_INSPECTION_STOP_CMD) == false ) 
	{	return false; }	
	SaveMovingTimeMsg(_T("CAOIDataCollect::ExecOnlineInspectionStopFn End"));
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecOnlineInspectionFinish()//執行線上檢測結束
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if (NULL == ProjectPtr) { return true; }

	CString str;
	CString str2;
	LANE_ID LaneID = GetActiveLaneID();
	const bool AlarmStop = ProjectPtr->GetProjectAlarmStop();
	const bool AlarmDefect = ProjectPtr->GetProjectAlarmDefect();
	if (true == AlarmDefect || true == AlarmStop)
	{
		str.Format(_T("Error, Project Test Result NG"));
		str2 = _T("Do you want to clear the statistic records?");
		//str = LoadMultiLanguageString(str, str);
		//str2 = LoadMultiLanguageString(str2, str2);
		str = AOIDataDefine.GetInspectionResultFaultText();//檢測結果異常	
		str2 = AOIDataDefine.GetDoYouWantToClearTheStatisticRecordsText();//是否清除統計資料
		PlcCtrlPtr->TurnOnInspectAlarm(LaneID);
		while (true)
		{
			JetAPI::ShowMessageBox(str);
			if (false == AlarmDefect)
			{	break;	}
			const bool bForce = false;			
			//if ( UserLogin(USER_LEVEL_ENGINEER, bForce) == true)
			if ( OperateLevelOnlineUnlockAlarm(false) == true )
			{
				if (JetAPI::ShowMessageBox(str2, MB_YESNO) == IDYES)
				{
					ProjectPtr->ResetProjectStatisticRecords();
				}
				UserLogout();
				break;
			}
		};
		PlcCtrlPtr->TurnOffInspectAlarm(LaneID);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateOnlineInspectionThread()//線上檢測執行緒
{
#ifndef OFFLINE_VERSION
	if ( this->DeleteOnlineInspectionThread() == false ) { return false; }	
	this->m_OnlineInspectionThreadCmd = THREAD_COMMAND_TO_IDLE;	

	OnlineInspectionHandle = (HANDLE)::_beginthreadex(NULL, NULL, &OnlineInspectionThreadFn, (void*)0, NULL, &OnlineInspectionThreadID);	
	if ( NULL == OnlineInspectionHandle )
	{	
		this->m_OnlineInspectionThreadState = THREAD_STATE_NONE;
		this->m_ErrorString = _T("Eorror, Create Thread Fault (OnlineInspectionHandle == NULL)");		
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_CREATE);
		return false;
	}
	OnlineInspectionEvent = JetAPI::CreateEvent(NULL, TRUE, TRUE, _T("Online Inspection Event"));		
	::SetThreadPriority(OnlineInspectionHandle, THREAD_PRIORITY_BELOW_NORMAL); 
	//::SetThreadAffinityMask(OnlineInspectionHandle, 0x03);//保留2個		
#else
	OnlineInspectionEvent = NULL;
	OnlineInspectionHandle = NULL;
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteOnlineInspectionThread()//線上檢測執行緒
{
#ifndef OFFLINE_VERSION
	DWORD WaitTime = 1000;//1 sec	
	if ( NULL == OnlineInspectionHandle ) { return true; }		
	this->SetOnlineInspectionThreadCmd(THREAD_COMMAND_TO_EXIT);	
	DWORD Res = ::WaitForSingleObject(OnlineInspectionHandle, WaitTime);
	::CloseHandle(OnlineInspectionHandle); 
	OnlineInspectionHandle = NULL;
	if ( NULL != OnlineInspectionEvent )
	{	
		::CloseHandle(OnlineInspectionEvent); 
		OnlineInspectionEvent=NULL; 
	}		
#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartOnlineInspectionThread(bool WaitOn)//線上檢測執行緒
{
#ifndef OFFLINE_VERSION
	size_t i=0;
	if ( NULL == OnlineInspectionHandle ) { return true; }
	if ( NULL != OnlineInspectionEvent )
	{	::ResetEvent(OnlineInspectionEvent); }
	if ( THREAD_STATE_FINISH == m_OnlineInspectionThreadState )
	{  SetOnlineInspectionThreadState(THREAD_STATE_IDLE); }
	SetOnlineInspectionThreadCmd(THREAD_COMMAND_TO_RUN);	

	if ( false == WaitOn )
	{	return true; }
	if ( WaitForOnlineInspectionThreadStart() == false )
	{	return false; }	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForOnlineInspectionThreadIdle()//等待線上檢測執行緒停止
{
#ifndef OFFLINE_VERSION		
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		//if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_NONE == m_OnlineInspectionThreadState ) { return true; }		
		if ( THREAD_STATE_IDLE == m_OnlineInspectionThreadState ) { return true; }
		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }		
	}
	if ( i == MaxCounts )
	{
		this->m_ErrorString.Format(_T("Error, Wait for WaitForOnlineInspectionThreadIdle too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE);
		return false;
	}		
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForOnlineInspectionThreadStop()//等待線上檢測執行緒停止
{
#ifndef OFFLINE_VERSION		
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		//if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_RUNNING != m_OnlineInspectionThreadState ) { return true; }
		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }		
	}
	if ( i == MaxCounts )
	{
		this->m_ErrorString.Format(_T("Error, Wait for WaitForOnlineInspectionThreadStop too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP);
		return false;
	}		
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForOnlineInspectionThreadStart()//等待線上檢測執行緒開始
{
#ifndef OFFLINE_VERSION
	size_t i=0;
	const size_t MaxCount = 200;
	const size_t SleepTime = 10;
	for ( i=0; i<MaxCount; i++ )
	{
		if ( THREAD_COMMAND_TO_RUN != m_OnlineInspectionThreadCmd )//切入下一個階段
		{	break; }
		if ( THREAD_STATE_IDLE != m_OnlineInspectionThreadState )
		{	break; }
		::Sleep(SleepTime);
	}
	if ( i == MaxCount )
	{
		this->m_ErrorString.Format(_T("Error, wait for StartInlineInspectionThread too long"));		
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_START);	
		return false;
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForOnlineInspectionThreadFinish()//線上檢測執行緒結束
{	
#ifndef OFFLINE_VERSION		
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;

	if ( NULL == OnlineInspectionEvent ) { return true; }
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == m_OnlineInspectionThreadCmd ) { return true; }		
		if ( THREAD_COMMAND_TO_IDLE == m_OnlineInspectionThreadCmd )
		{	break; }

		Res = ::WaitForSingleObject(OnlineInspectionEvent, SleepTime);
		if ( Res != WAIT_TIMEOUT ) 
		{
			if ( THREAD_STATE_RUNNING == m_OnlineInspectionThreadState )
			{
				if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
				continue; 
			}
			break; 
		}
	}
	if ( i == MaxCounts )
	{
		this->m_ErrorString.Format(_T("Error, Wait for WaitForInlineInspectionThreadFinish too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
		return false;
	}		
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckOnlineInspectionThreadStateFinish()//確認線上檢測執行緒狀態
{
	if ( NULL == OnlineInspectionHandle ) { return true; }
	if ( m_OnlineInspectionThreadState != THREAD_STATE_FINISH )
	{
		this->m_ErrorString.Format(_T("Error, m_OnlineInspectionThreadState is Exception"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetOnlineInspectionThreadState(THREAD_STATE_MODE State)//設定線上檢測執行緒狀態
{
	if ( m_OnlineInspectionThreadState == State ) { return; }
	LockThreadOnlineInspection();
	this->m_OnlineInspectionThreadState = State;
	UnlockThreadOnlineInspection();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE CAOIDataCollect::GetOnlineInspectionThreadState()//取得線上檢測執行緒狀態
{
	return m_OnlineInspectionThreadState;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetOnlineInspectionThreadCmd(THREAD_COMMAND_MODE Cmd)//設定線上檢測執行緒命令
{
	if ( m_OnlineInspectionThreadCmd == Cmd ) { return; }
	LockThreadOnlineInspection();
	this->m_OnlineInspectionThreadCmd = Cmd;
	UnlockThreadOnlineInspection();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CAOIDataCollect::GetOnlineInspectionThreadCmd()//取得線上檢測執行緒命令
{
	return m_OnlineInspectionThreadCmd;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecRemoveFolderFn()//執行移除資料夾執行緒
{	
	size_t       i=0;	
#ifndef OFFLINE_VERSION
	LockThreadRemoveFolder();
	const size_t FolderCountNew = m_RemoveFolderListNew.size();
	for ( i=0; i<FolderCountNew; i++ )
	{	m_RemoveFolderListExec.push_back(m_RemoveFolderListNew[i]);	}
	m_RemoveFolderListNew.clear();
	UnlockThreadRemoveFolder();

	CString        Folder;	
	TRemoveFolder  RemoveFolder;
	CTimeSpan      SpentDateTime;
	LONGLONG       SpentMinutes = 0;
	THREAD_COMMAND_MODE    ThreadCmd;	
	CTime        CurrentDateTime = CTime::GetCurrentTime();		
	const LONGLONG MaxKeepMinutes = GetOnlineTuningKeepMaxTime();//十分鐘後刪除
	const size_t FolderCountExec = m_RemoveFolderListExec.size();	
	if ( 0 == FolderCountExec ) { return true; }
	
	for ( i=0; i<FolderCountExec; i++ )
	{
		ThreadCmd = GetRemoveFolderThreadCmd();
		if ( GetIsSystemReleased() == true ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }		
		if ( THREAD_COMMAND_TO_NONE == ThreadCmd ) { return true; }		

		RemoveFolder = m_RemoveFolderListExec[i];
		SpentDateTime = CurrentDateTime-RemoveFolder.DateTime;		
		SpentMinutes = SpentDateTime.GetTotalMinutes();		
		if ( REMOVE_FOLDER_TYPE_ONLINE_TUNING_FOLDER == RemoveFolder.FolderType )
		{
			if ( SpentMinutes < MaxKeepMinutes ) 
			{	continue;	}
		}
		
		m_RemoveFolderListExec[i].Removed = true;
		Folder = RemoveFolder.Folder;
		JetAPI::RemoveFolder(Folder);
	}

	std::vector<TRemoveFolder> TuningFolderListTmp = m_RemoveFolderListExec;
	const size_t FolderCountTmp = TuningFolderListTmp.size();	

	m_RemoveFolderListExec.clear();
	for ( i=0; i<FolderCountTmp; i++ )
	{
		RemoveFolder = TuningFolderListTmp[i];
		if ( true == RemoveFolder.Removed ) { continue; }
		m_RemoveFolderListExec.push_back(RemoveFolder);
	}	
	const size_t FolderCountExec2 = m_RemoveFolderListExec.size();	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateRemoveFolderThread()//建立移除資料夾執行緒
{
#ifndef OFFLINE_VERSION
	if ( DeleteRemoveFolderThread() == false ) { return false; }	
	this->m_RemoveFolderThreadCmd = THREAD_COMMAND_TO_IDLE;	

	RemoveFolderThreadHandle = (HANDLE)::_beginthreadex(NULL, NULL, &RemoveFolderThreadFn, (void*)0, NULL, &RemoveFolderThreadID);	
	if ( NULL == RemoveFolderThreadHandle )
	{	
		this->m_RemoveFolderThreadState = THREAD_STATE_NONE;
		this->m_ErrorString = _T("Eorror, Create Thread Fault (RemoveFolderThreadHandle == NULL)");		
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_CREATE);
		return false;
	}		
	RemoveFolderThreadEvent = JetAPI::CreateEvent(NULL, TRUE, TRUE, _T("Online Tuning Remove Event"));		
	::SetThreadPriority(RemoveFolderThreadHandle, THREAD_PRIORITY_BELOW_NORMAL); 
	//::SetThreadAffinityMask(RemoveFolderThreadHandle, 0x03);//保留2個		
	StartRemoveFolderThread(false);
#else
	RemoveFolderThreadEvent = NULL;
	RemoveFolderThreadHandle = NULL;
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteRemoveFolderThread()//刪除移除資料夾執行緒
{
#ifndef OFFLINE_VERSION
	DWORD WaitTime = 10000;//1 sec	
	if ( NULL == RemoveFolderThreadHandle ) { return true; }		
	this->SetRemoveFolderThreadCmd(THREAD_COMMAND_TO_EXIT);	
	DWORD Res = ::WaitForSingleObject(RemoveFolderThreadHandle, WaitTime);
	::CloseHandle(RemoveFolderThreadHandle); 
	RemoveFolderThreadHandle = NULL;
	if ( NULL != RemoveFolderThreadEvent )
	{	
		::CloseHandle(RemoveFolderThreadEvent); 
		RemoveFolderThreadEvent=NULL; 
	}		
#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartRemoveFolderThread(bool WaitOn)//開始移除資料夾執行緒
{
#ifndef OFFLINE_VERSION
	size_t i=0;
	if ( NULL == RemoveFolderThreadHandle ) { return true; }
	if ( NULL != RemoveFolderThreadEvent )
	{	::ResetEvent(RemoveFolderThreadEvent); }
	if ( THREAD_STATE_FINISH == m_RemoveFolderThreadState )
	{  SetRemoveFolderThreadState(THREAD_STATE_IDLE); }
	SetRemoveFolderThreadCmd(THREAD_COMMAND_TO_RUN);	

	if ( false == WaitOn ) 
	{	return true; }
	if ( WaitForRemoveFolderThreadStart() == false )
	{	return false; }	
#endif//OFFLINE_VERSION
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForRemoveFolderThreadIdle()//等待移除資料夾除執行緒停止
{
#ifndef OFFLINE_VERSION		
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		//if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_NONE == m_RemoveFolderThreadState ) { return true; }		
		if ( THREAD_STATE_IDLE == m_RemoveFolderThreadState ) { return true; }

		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }		
	}
	if ( i == MaxCounts )
	{
		this->m_ErrorString.Format(_T("Error, Wait for WaitForRemoveFolderThreadIdle too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE);
		return false;
	}		
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForRemoveFolderThreadStop()//等待移除資料夾除執行緒停止
{
#ifndef OFFLINE_VERSION		
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		//if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_RUNNING != m_RemoveFolderThreadState ) { return true; }

		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }		
	}
	if ( i == MaxCounts )
	{
		this->m_ErrorString.Format(_T("Error, Wait for WaitForRemoveFolderThreadStop too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP);
		return false;
	}		
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForRemoveFolderThreadStart()//等待移除資料夾除執行緒開始
{
#ifndef OFFLINE_VERSION
	size_t i=0;
	const size_t MaxCount = 100;
	const size_t SleepTime = 20;
	for ( i=0; i<MaxCount; i++ )
	{
		if ( THREAD_COMMAND_TO_RUN != m_RemoveFolderThreadCmd )//切入下一個階段
		{	break; }
		if ( THREAD_STATE_IDLE != m_RemoveFolderThreadState )
		{	break; }
		::Sleep(SleepTime);
	}
	if ( i == MaxCount )
	{
		this->m_ErrorString.Format(_T("Error, wait for StartRemoveFolderThread too long"));	
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_START);	
		return false;
	}
#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForRemoveFolderThreadFinish()//等待移除資料夾除執行緒結束
{
#ifndef OFFLINE_VERSION		
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;

	if ( NULL == OnlineInspectionEvent ) { return true; }
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == m_RemoveFolderThreadCmd ) { return true; }		
		if ( THREAD_COMMAND_TO_IDLE == m_RemoveFolderThreadCmd )
		{	break; }

		Res = ::WaitForSingleObject(RemoveFolderThreadEvent, SleepTime);
		if ( Res != WAIT_TIMEOUT ) 
		{
			if ( THREAD_STATE_RUNNING == m_RemoveFolderThreadState )
			{
				if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
				continue; 
			}
			break; 
		}
	}
	if ( i == MaxCounts )
	{
		this->m_ErrorString.Format(_T("Error, Wait for WaitForRemoveFolderThreadFinish too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
		return false;
	}		
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckRemoveFolderThreadState(THREAD_STATE_MODE State)//確認移除資料夾執行緒狀態
{
	if ( NULL == RemoveFolderThreadHandle ) { return true; }
	if ( m_RemoveFolderThreadState != State )
	{
		this->m_ErrorString.Format(_T("Error, m_RemoveFolderThreadState is Exception"));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetRemoveFolderThreadState(THREAD_STATE_MODE State)//設定移除資料夾執行緒狀態
{
	if ( m_RemoveFolderThreadState == State ) { return; }
	LockThreadRemoveFolder();
	m_RemoveFolderThreadState = State;
	UnlockThreadRemoveFolder();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE CAOIDataCollect::GetRemoveFolderThreadState()//取得移除資料夾執行緒狀態
{
	return m_RemoveFolderThreadState;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetRemoveFolderThreadCmd(THREAD_COMMAND_MODE Cmd)//設定移除資料夾執行緒命令
{
	if ( m_RemoveFolderThreadCmd == Cmd ) { return; }
	LockThreadRemoveFolder();
	m_RemoveFolderThreadCmd = Cmd;
	UnlockThreadRemoveFolder();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CAOIDataCollect::GetRemoveFolderThreadCmd()//取得移除資料夾執行緒命令
{
	return m_RemoveFolderThreadCmd;
}
//-------------------------------------------------------------------------------------//
LANE_ID CAOIDataCollect::GetConveyerAutoRunLaneID(size_t ThreadIdx) const//取得軌道自動運轉軌道號
{
	LANE_ID LaneID=LANE_ID_NULL;
	switch ( ThreadIdx )
	{
	case 1: LaneID = LANE_ID_A; break;
	case 2: LaneID = LANE_ID_B; break;	
	}
	return LaneID;
}
//-------------------------------------------------------------------------------------//
size_t CAOIDataCollect::GetConveyerAutoRunLaneIdx(LANE_ID LaneID) const//取得軌道自動運轉軌道引數
{
	size_t idx=0;
	switch ( LaneID )
	{
	case LANE_ID_B: idx = 2; break;
	default:
	case LANE_ID_A: idx = 1; break;
	}
	return idx;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecConveyerAutoRunFn(size_t idx)//執行軌道自動運轉執行緒
{
#ifndef OFFLINE_VERSION
	LANE_ID NextLaneID;
	const size_t ThreadIdx=idx;
	LANE_ID LaneID=GetConveyerAutoRunLaneID(idx);			
	if ( LANE_ID_NULL == LaneID ) { return true; }	
	LANE_ID LaneID_ORG=GetConveyerAutoRunLaneID(idx);

	TASK_MODE TaskMode = GetTaskMode();
	if ( TASK_PROJECT_MAP == TaskMode ) { return true; }
	if ( TASK_ALIGN_PROJECT == TaskMode ) { return true; }
	if ( TASK_EXPORT_OFFLINE == TaskMode ) { return true; }
	if ( TASK_TUNING_PROJECT == TaskMode ) { return true; }
	if ( TASK_TUNING_OFFLINE == TaskMode ) { return true; }
	//if ( TASK_INSPECT_PROJECT == TaskMode ) { return true; }
	if ( TASK_EXPORT_COMPONENT == TaskMode ) { return true; }
	//if ( TASK_LANE_BYPASS == TaskMode ) { return true; }

	const bool bPreRunMode  = GetConveyerAutoRunPreRunMode(ThreadIdx);
	CAOIProject *ProjectPtr = GetConveyerAutoRunProjectPtr(ThreadIdx);//取得軌道自動運轉專案
	if ( true==bPreRunMode && NULL==ProjectPtr )
	{	return true;	}

	POINT MousePosLast;	
	bool  bExitLoop=false;
	DWORD SleepTime = 50;
	DWORD TickCountRepeat=0;		
	bool  SensorPcbStop = false;
	bool  IsNeedReCheckPCBInside=false;
	const bool bAutoReset = false;
	const bool bChkStartLight = false;	
	const DWORD SleepTime01 = 10;//ms	
	const DWORD SleepTime05 = 50;//ms	
	const DWORD SleepTime10 =100;//ms		
	const bool bUpdateUI=bPreRunMode;

	THREAD_COMMAND_MODE  ThreadCmd;
	WND_MESSAGE_MODE     WndMessageMode = WND_MESSAGE_SEND;
	PCB_OUT_MODE         PCBOutMode = GetLanePCBOutMode(LaneID);
	PCB_OUT_DIRECTION    PCBOutDirection = GetPCBOutDirection();//進出板方向
	ONLINE_STATE_MODE    OnlineStateMode = GetConveyerAutoRunState(ThreadIdx);		

	MULTI_LANE_MODE MultiLaneMode = CheckMultiLaneMode();
	LANE_WORK_MODE LaneWorkMode_LA = GetLaneWorkMode_LA();
	LANE_WORK_MODE LaneWorkMode_LB = GetLaneWorkMode_LB();		
	TASK_STATE_MODE TaskStateMode = GetOnlineTaskState();	
	LANE_STATE_MODE LaneStateMode = GetOnlineLaneTaskState(LaneID);
	ONLINE_STATE_MODE OldOnlineState = OnlineStateMode;
	ONLINE_STATE_MODE NewOnlineState = OnlineStateMode;	
	int BypassLastSignal = GetBypassLastSignal();		
	int BypassNextSignal = GetBypassNextSignal();		
	
	bool                 bOnlineProcRet;
	TOnlineProcParam     OnlineProcParam;	
	OnlineProcParam.pProject = ProjectPtr;
	OnlineProcParam.bExitLoop = false;
	OnlineProcParam.eTaskMode = GetTaskMode();
	OnlineProcParam.eLaneID = LaneID;
	OnlineProcParam.eLaneIDNext = OnlineProcParam.eLaneID;
	OnlineProcParam.ePCBOutMode = GetLanePCBOutMode(OnlineProcParam.eLaneID);
	OnlineProcParam.eFromMode = ONLINE_FROM_CONVEYER_THREAD;
	OnlineProcParam.eMultiLaneMode = CheckMultiLaneMode();
	OnlineProcParam.eLaneWorkModeLA = GetLaneWorkMode_LA();
	OnlineProcParam.eLaneWorkModeLB = GetLaneWorkMode_LB();
	OnlineProcParam.bBypassLastSignal = GetBypassLastSignal();
	OnlineProcParam.bBypassNextSignal = GetBypassNextSignal();
	OnlineProcParam.ePCBOutDirection = GetPCBOutDirection();
	OnlineProcParam.nLastStationLineMode = GetLastStationLineMode();
	OnlineProcParam.eOnlineStateOld = OnlineStateMode;
	OnlineProcParam.eOnlineStateNew = OnlineStateMode;
	OnlineProcParam.dwSleepTime = SleepTime;	
	OnlineProcParam.eWndMessageMode = WndMessageMode;
	OnlineProcParam.bConveyerPreRunMode = bPreRunMode;
	const CString strLaneID = AOIDataDefine.GetLaneIDText(LaneID);
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ExecConveyerAutoRunFn Start"))+strLaneID);
	while ( true )
	{
		bExitLoop=false;
		OldOnlineState = NewOnlineState;
		ThreadCmd = GetConveyerAutoRunThreadCmd(ThreadIdx);
		if ( GetIsSystemReleased() == true ) { return true; }
		if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) 
		{ 			
			bExitLoop = true;
			break;
		}		
		if ( SleepTime > 0 ) 
		{	::Sleep(SleepTime); }

		if ( CheckSystemReady(bChkStartLight, bAutoReset) == false )
		{	return true; }

		TaskStateMode = GetOnlineTaskState();	
		LaneStateMode = GetOnlineLaneTaskState(LaneID);
		if ( OnlineProcParam.pProject != ProjectPtr )
		{	OnlineProcParam.pProject = ProjectPtr;	}
		OnlineProcParam.eLaneID = LaneID;
		OnlineProcParam.eLaneIDNext = LaneID;
		OnlineProcParam.eOnlineStateOld = OldOnlineState;
		OnlineProcParam.eOnlineStateNew = NewOnlineState;
		OnlineProcParam.eWndMessageMode = WndMessageMode;
		OnlineProcParam.eLaneWorkModeLA = GetLaneWorkMode_LA();
		OnlineProcParam.eLaneWorkModeLB = GetLaneWorkMode_LB();

		if ( TASK_STATE_TO_STOP == TaskStateMode )
		{
			if ( CheckOnlineConveyerAutoRunCanStop(OldOnlineState) == true )
			{
				switch ( OldOnlineState )
				{
				case ONLINE_STATE_PCB_DUAL_RUN_CHECKING:
					SetOnlineStateMode_Lane(ONLINE_STATE_INSPECTION_STOP, LaneID);
					break;
				}
				bExitLoop = true;	
				break;
			}
		}	
		
		switch ( LaneStateMode )
		{
		case LANE_STATE_RUNNING:
			if (LANE_ID_B == LaneID) { SetLaneWorkMode_LB(LANE_WORK_RUN); }
			else { SetLaneWorkMode_LA(LANE_WORK_RUN); }
			break;
		case LANE_STATE_PAUSE:
			if (CheckOnlineConveyerAutoRunCanStop(OldOnlineState) == true)
			{
				//SetOnlineStateMode_GUI_Lane(ONLINE_STATE_PCB_INSPECTION_PAUSE, LaneID);
				WndMessageMode = OnlineProcParam.eWndMessageMode;
				if ( WND_MESSAGE_POST == WndMessageMode )
				{	PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL); }
				else
				{	SendCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL); }
				if (false == PlcCtrlPtr->GetConveryerSensorPCBStop(LaneID)) {continue;}
				bExitLoop = true;
			}
			break;
		case LANE_STATE_BYPASS:
			if (LANE_ID_B == LaneID) {	SetLaneWorkMode_LB(LANE_WORK_BYPASS);	}
			else { SetLaneWorkMode_LA(LANE_WORK_BYPASS); }
			break;
		default:
			break;
		}
		if (true == bExitLoop) { break; }

		switch ( OldOnlineState )
		{		
		case ONLINE_STATE_PCB_READY://PCB就緒			
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_READY-(Auto-Run)"))+strLaneID);	
			if ( ExecOnlineProcPCBReady(OnlineProcParam) == false ) 
			{	return false;	}
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;	
			break;
		case ONLINE_STATE_INPUT_BARCODE://輸入條碼
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_INPUT_BARCODE-(Auto-Run)"))+strLaneID);	
			if ( ExecOnlineProcInputBarcode(OnlineProcParam) == false ) 
			{	return false;	}			
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;			
		case ONLINE_STATE_INSPECTION_FINISH://檢測結束
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_INSPECTION_FINISH-(Auto-Run)"))+strLaneID);
			if ( ExecOnlineProcInspectionFinish(OnlineProcParam) == false ) 
			{
				if ( MULTI_LANE_2 == MultiLaneMode )//雙軌道模式
				{					
					NewOnlineState = ONLINE_STATE_INSPECTION_STOP;					
					SetConveyerAutoRunState(ThreadIdx, NewOnlineState);
					if ( true == bUpdateUI )//20230425
					{	SetOnlineStateMode_GUI(NewOnlineState); }
					else
					{	SetOnlineStateMode_Lane(NewOnlineState, LaneID);	}
				}	
				return false;	
			}
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PROJECT_SWITCH_BY_TURN://專案切換-輪流
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PROJECT_SWITCH_BY_TURN-(Auto-Run)"))+strLaneID);
			if ( ExecOnlineProcProjectSwitchByTurn(OnlineProcParam) == false )
			{	return false; }
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PROJECT_SWITCH_BY_TURN_ONE_CYCLE_RESET://專案切換-重設輪流一次檢測
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PROJECT_SWITCH_BY_TURN_ONE_CYCLE_RESET-(Auto-Run)"))+strLaneID);
			if ( ExecOnlineProcProjectSwitchByTurnOneCycleReset(OnlineProcParam) == false )
			{	return false; }
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_WAIT_FOR_LAST_STATION://等待上一站訊號			
			if ( ExecOnlineProcWaitForLastStation(OnlineProcParam) == false )
			{	return false; }
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_WAIT_FOR_NEXT_STATION://等待下一站訊號			
			if ( ExecOnlineProcWaitForNextStation(OnlineProcParam) == false ) 
			{	return false; }
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_IN_START://進板開始
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_IN_START-(Auto-Run)"))+strLaneID);	
			if ( ExecOnlineProcPCBInStart(OnlineProcParam) == false ) 
			{	return false; }
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_IN_CHECKING://進板確認
			if ( ExecOnlineProcPCBInChecking(OnlineProcParam) == false ) 
			{	return false; }
			NewOnlineState = OnlineProcParam.eOnlineStateNew;		
			break;
		case ONLINE_STATE_PCB_IN_FINISH://進板結束			
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_IN_FINISH-(Auto-Run)"))+strLaneID);				
			if ( ExecOnlineProcPCBInFinish(OnlineProcParam) == false ) 
			{	return false; }			
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_OUT_START://出板開始
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_OUT_START-(Auto-Run)"))+strLaneID);	
			if ( ExecOnlineProcPCBOutStart(OnlineProcParam) == false ) 
			{	return false; }
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_OUT_CHECKING://出板確認
			if ( ExecOnlineProcPCBOutChecking(OnlineProcParam) == false ) 
			{	return false; }
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_OUT_FINISH://出板結束			
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_OUT_FINISH-(Auto-Run)"))+strLaneID);	
			if ( ExecOnlineProcPCBOutFinish(OnlineProcParam) == false ) 
			{	return false; }				
			NewOnlineState = OnlineProcParam.eOnlineStateNew;			
			break;		
		case ONLINE_STATE_PCB_OUT_INSIDE_START://停側邊開始
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_OUT_INSIDE_START-(Auto-Run)"))+strLaneID);	
			if ( ExecOnlineProcPCBOutInsideStart(OnlineProcParam) == false ) 
			{	return false; }			
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_OUT_INSIDE_CHECKING://停側邊確認
			if ( ExecOnlineProcPCBOutInsideChecking(OnlineProcParam) == false ) 
			{	return false; }
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_OUT_INSIDE_FINISH://停側邊結束			
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_OUT_INSIDE_FINISH-(Auto-Run)"))+strLaneID);	
			if ( ExecOnlineProcPCBOutInsideFinish(OnlineProcParam) == false ) 
			{	return false; }
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;			
		case ONLINE_STATE_PCB_BACK_START://退板開始
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_BACK_START-(Auto-Run)"))+strLaneID);	
			if ( ExecOnlineProcPCBBackStart(OnlineProcParam) == false ) 
			{	return false; }
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_BACK_CHECKING://退板確認	
			if ( ExecOnlineProcPCBBackChecking(OnlineProcParam) == false ) 
			{	return false; }
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_BACK_FINISH://退板結束
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_BACK_FINISH-(Auto-Run)"))+strLaneID);	
			if ( ExecOnlineProcPCBBackFinish(OnlineProcParam) == false ) 
			{	return false; }			
			NewOnlineState = OnlineProcParam.eOnlineStateNew;			
			break;
		case ONLINE_STATE_PCB_BACK_OUT_START://退出板開始
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_BACK_OUT_START-(Auto-Run)"))+strLaneID);	
			if ( ExecOnlineProcPCBBackOutStart(OnlineProcParam) == false ) 
			{	return false; }
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_BACK_OUT_CHECKING://退出板確認
			if ( ExecOnlineProcPCBBackOutChecking(OnlineProcParam) == false ) 
			{	return false; }
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_BACK_OUT_FINISH://退出板結束
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_BACK_OUT_FINISH-(Auto-Run)"))+strLaneID);	
			if ( ExecOnlineProcPCBBackOutFinish(OnlineProcParam) == false ) 
			{	return false; }			
			NewOnlineState = OnlineProcParam.eOnlineStateNew;			
			break;
		case ONLINE_STATE_WAIT_FOR_PCB_REMOVED://等待板子移走			
			if ( ExecOnlineProcWaitForPCBRemoved(OnlineProcParam) == false ) 
			{	return false; }			
			NewOnlineState = OnlineProcParam.eOnlineStateNew;			
			break;
		case ONLINE_STATE_WAIT_FOR_REPAIR_VERIFY://等待維修站確認			
			if ( ExecOnlineProcWaitForRepairVerify(OnlineProcParam) == false ) 
			{	return false; }
			SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_OUT_IN_START://出板帶進板開始
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_OUT_IN_START-(Auto-Run)"))+strLaneID);
			if ( ExecOnlineProcPCBOutInStart(OnlineProcParam) == false ) 
			{	return false; }			
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_OUT_IN_CHECKING://出板帶進板確認	
			if ( ExecOnlineProcPCBOutInChecking(OnlineProcParam) == false ) 
			{	return false; }			
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_OUT_IN_FINISH://出板帶進板結束
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_OUT_IN_FINISH-(Auto-Run)"))+strLaneID);	
			if ( ExecOnlineProcPCBOutInFinish(OnlineProcParam) == false ) 
			{	return false; }				
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;		
		case ONLINE_STATE_PCB_AUTO_RUN_START:
			if ( PCB_OUT_DIR_BACKWARD == PCBOutDirection )
			{	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_AUTO_RUN_START-BACK-(Auto-Run)"))+strLaneID);		}
			else if ( PCB_OUT_DIR_BACKWARD_OUT == PCBOutDirection )
			{	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_AUTO_RUN_START-BACK-OUT-(Auto-Run)"))+strLaneID);		}
			else
			{	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_AUTO_RUN_START-(Auto-Run)"))+strLaneID);		}
			if ( ExecOnlineProcPCBAutoRunStart(OnlineProcParam) == false ) 
			{	return false; }			
			NewOnlineState = OnlineProcParam.eOnlineStateNew;				
			break;
		case ONLINE_STATE_PCB_AUTO_RUN_CHECKING:			
			if ( ExecOnlineProcPCBAutoRunChecking(OnlineProcParam) == false ) 
			{
				NewOnlineState = ONLINE_STATE_INSPECTION_STOP;				
				SetConveyerAutoRunState(ThreadIdx, NewOnlineState);
				if ( true == bUpdateUI )//20230425
				{	SetOnlineStateMode_GUI(NewOnlineState); }
				else
				{	SetOnlineStateMode_Lane(NewOnlineState, LaneID);	}
				return false; 
			}
			//LaneID = OnlineProcParam.eLaneID;
			//SleepTime = OnlineProcParam.dwSleepTime;
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_AUTO_RUN_FINISH:			
			SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_AUTO_RUN_FINISH-(Auto-Run)"))+strLaneID);	
			if ( ExecOnlineProcPCBAutoRunFinish(OnlineProcParam) == false ) 
			{	return false;	}			
			NewOnlineState = OnlineProcParam.eOnlineStateNew;
			break;
		case ONLINE_STATE_PCB_DUAL_RUN_START:			
		case ONLINE_STATE_PCB_DUAL_RUN_CHECKING:			
		case ONLINE_STATE_PCB_DUAL_RUN_FINISH:
			bExitLoop = true;
			break;
		default:		
			bExitLoop = true;			
			break;
		}
		if ( OldOnlineState != NewOnlineState )
		{
			TickCountRepeat=0;
			MousePosLast.x = MousePosLast.y = 0;
			OldOnlineState = NewOnlineState;
			//SetOnlineStateMode(NewOnlineState);							
			SetConveyerAutoRunState(ThreadIdx, NewOnlineState);
			if ( false == bUpdateUI )//20230425
			{	SetOnlineStateMode_Lane(NewOnlineState, LaneID);	}
			else
			{
				SetOnlineStateMode_GUI(NewOnlineState);
				SaveAOIMonitorStatus(NewOnlineState, NULL);
			}	
			WndMessageMode = OnlineProcParam.eWndMessageMode;
			if ( WND_MESSAGE_POST == WndMessageMode )
			{	PostCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL); }
			else
			{	SendCallbackWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL); }

			switch ( NewOnlineState )
			{
			case ONLINE_STATE_INSPECTION_WAITING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_INSPECTION_WAITING-(Auto-Run)"))+strLaneID);
				break;
			case ONLINE_STATE_WAIT_FOR_PCB_REMOVED:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_WAIT_FOR_PCB_REMOVED-(Auto-Run)"))+strLaneID);
				break;
			case ONLINE_STATE_WAIT_FOR_LAST_STATION:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_WAIT_FOR_LAST_STATION-(Auto-Run)"))+strLaneID);
				break;
			case ONLINE_STATE_WAIT_FOR_NEXT_STATION:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_WAIT_FOR_NEXT_STATION-(Auto-Run)"))+strLaneID);
				break;
			case ONLINE_STATE_PCB_IN_CHECKING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_IN_CHECKING-(Auto-Run)"))+strLaneID);
				break;
			case ONLINE_STATE_PCB_OUT_CHECKING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_OUT_CHECKING-(Auto-Run)"))+strLaneID);
				break;
			case ONLINE_STATE_PCB_OUT_INSIDE_CHECKING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_OUT_INSIDE_CHECKING-(Auto-Run)"))+strLaneID);
				break;
			case ONLINE_STATE_PCB_BACK_CHECKING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_BACK_CHECKING-(Auto-Run)"))+strLaneID);
				break;
			case ONLINE_STATE_PCB_BACK_OUT_CHECKING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_BACK_OUT_CHECKING-(Auto-Run)"))+strLaneID);
				break;
			case ONLINE_STATE_PCB_OUT_IN_CHECKING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_OUT_IN_CHECKING-(Auto-Run)"))+strLaneID);
				break;
			case ONLINE_STATE_PCB_AUTO_RUN_CHECKING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_AUTO_RUN_CHECKING-(Auto-Run)"))+strLaneID);
				break;
			case ONLINE_STATE_PCB_DUAL_RUN_CHECKING:
				SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ONLINE_STATE_PCB_DUAL_RUN_CHECKING-(Auto-Run)"))+strLaneID);
				break;
			}
		}
		else
		{
			if ( 0 == TickCountRepeat )
			{
				::GetCursorPos(&MousePosLast);
				TickCountRepeat = ::GetTickCount();	
			}
			else
			{
				DWORD TickCountGap = ::GetTickCount()-TickCountRepeat;
				if ( TickCountGap > 1000 )
				{
					CString str;
					POINT MousePosNow;
					::GetCursorPos(&MousePosNow);
					TickCountRepeat = ::GetTickCount();
					if ( MousePosNow.x!=MousePosLast.x || MousePosNow.y!=MousePosLast.y )//20260402
					{	str.Format(_T("ExecConveyerAutoRunFn Repeat[%s]%s"), AOIDataDefine.GetOnlineStateText(NewOnlineState), strLaneID);	}
					else
					{	
						mouse_event(MOUSEEVENTF_MOVE, 0, 0, 0, 0 );	
						str.Format(_T("ExecConveyerAutoRunFn Repeat[%s]%s-AutoMoveMouse"), AOIDataDefine.GetOnlineStateText(NewOnlineState), strLaneID);
					}
					SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, str);
					MousePosLast = MousePosNow;
				}			
			}
		}
		if ( true == bExitLoop || true==OnlineProcParam.bExitLoop )
		{	break; }
	}
	SetConveyerAutoRunPreRunMode(ThreadIdx, false);
	SetConveyerAutoRunProjectPtr(ThreadIdx, NULL);	
	SetConveyerAutoRunState(ThreadIdx, NewOnlineState);
	SaveMovingTimeMsg(MSG_FILTER_SYSTEM, MSG_LEVEL_HIGH, CString(_T("ExecConveyerAutoRunFn End"))+strLaneID);
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateConveyerAutoRunThread()//建立軌道自動運轉入執行緒
{
#ifndef OFFLINE_VERSION
	size_t  i=0;	
	CString str;
	size_t  ThreadIdx = 0;		
	if ( this->DeleteConveyerAutoRunThread() == false ) { return false; }	

	for ( i=0; i<MAX_THREAD_COUNT_LANE_AUTO_RUN; i++ )
	{
		ThreadIdx = i;
		this->m_ConveyerAutoRunThreadCmd[ThreadIdx] = THREAD_COMMAND_TO_IDLE;	
	}

	for ( i=0; i<MAX_THREAD_COUNT_LANE_AUTO_RUN; i++ )
	{
		ThreadIdx = i;
		ConveyerAutoRunThreadHandle[ThreadIdx] = (HANDLE)::_beginthreadex(NULL, NULL, &ConveyerAutoRunThreadFn, (void*)ThreadIdx, NULL, &ConveyerAutoRunThreadID[ThreadIdx]);	
		if ( NULL == ConveyerAutoRunThreadHandle[ThreadIdx] )
		{	
			this->m_ConveyerAutoRunThreadState[ThreadIdx] = THREAD_STATE_NONE;
			this->m_ErrorString = _T("Eorror, Create Thread Fault (ConveyerAutoRunThreadHandle == NULL)");		
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_CREATE);
			return false;
		}	
	}
	
	for ( i=0; i<MAX_THREAD_COUNT_LANE_AUTO_RUN; i++ )
	{
		ThreadIdx = i;	
		str.Format(_T("Conveyer Auto Run Event[%d]"), ThreadIdx+1);	
		ConveyerAutoRunThreadEvent[ThreadIdx] = JetAPI::CreateEvent(NULL, TRUE, TRUE, str);
		::SetThreadPriority(ConveyerAutoRunThreadHandle[ThreadIdx], THREAD_PRIORITY_BELOW_NORMAL);
	}
#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteConveyerAutoRunThread()//刪除軌道自動運轉執行緒
{
#ifndef OFFLINE_VERSION
	size_t i = 0;
	size_t ThreadIdx = 0;
	DWORD WaitTime = 1000;//1 sec
	for ( i=0; i<MAX_THREAD_COUNT_LANE_AUTO_RUN; i++ )
	{
		ThreadIdx = i;
		if ( NULL == ConveyerAutoRunThreadHandle[ThreadIdx] ) { continue; }		
		this->SetConveyerAutoRunThreadCmd(ThreadIdx, THREAD_COMMAND_TO_EXIT);	
		DWORD Res = ::WaitForSingleObject(ConveyerAutoRunThreadHandle[ThreadIdx], WaitTime);
		::CloseHandle(ConveyerAutoRunThreadHandle[ThreadIdx]); 
		ConveyerAutoRunThreadHandle[ThreadIdx] = NULL;
		if ( NULL != ConveyerAutoRunThreadEvent[ThreadIdx] )
		{	
			::CloseHandle(ConveyerAutoRunThreadEvent[ThreadIdx]); 
			ConveyerAutoRunThreadEvent[ThreadIdx]=NULL; 
		}	
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StopConveyerAutoRunThread(bool WaitOn)//停止軌道自動運轉執行緒	
{
#ifndef OFFLINE_VERSION	
	bool bSucc = true;
	CString Error;
	const size_t ThreadCount   = MAX_THREAD_COUNT_LANE_AUTO_RUN;
	for ( size_t i=0; i<ThreadCount; i++ )
	{
		if ( StopConveyerAutoRunThread(i, WaitOn) == false )
		{
			bSucc = false;
			Error = GetErrorString();
		}
	}	
	if ( false == bSucc )
	{	return false; }
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StopConveyerAutoRunThread(size_t idx, bool WaitOn)//停止軌道自動運轉執行緒	
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;	
	const size_t ThreadCount   = MAX_THREAD_COUNT_LANE_AUTO_RUN;
	if ( idx >= ThreadCount ) { return true; }
	
	const size_t ThreadIdx=idx;
	if ( NULL == ConveyerAutoRunThreadHandle[ThreadIdx] ) { return true; }
	if ( NULL != ConveyerAutoRunThreadEvent[ThreadIdx] )
	{	::ResetEvent(ConveyerAutoRunThreadEvent[ThreadIdx]); }
	SetConveyerAutoRunThreadCmd(ThreadIdx, THREAD_COMMAND_TO_IDLE);	

	if ( WaitOn == true ) 
	{	
		const size_t MaxCount = 1000;
		const size_t SleepTime = 10;		
		for ( i=0; i<MaxCount; i++ )
		{	
			if ( THREAD_STATE_IDLE == m_ConveyerAutoRunThreadState[ThreadIdx] )
			{	break; }
			if ( THREAD_STATE_NONE == m_ConveyerAutoRunThreadState[ThreadIdx] )
			{	break; }
			//if ( THREAD_STATE_FINISH == m_ConveyerAutoRunThreadState[ThreadIdx] )//有可能後面才完成, 所以要加上完成確認
			//{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			this->m_ErrorString.Format(_T("Error, wait for StopConveyerAutoRunThread[%d] too long"), ThreadIdx+1);			
			return false;
		}		
	}
#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartConveyerAutoRunThreadFn(size_t idx, bool WaitOn)//開始軌道自動運轉執行緒	
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;	
	const size_t ThreadCount   = MAX_THREAD_COUNT_LANE_AUTO_RUN;
	if ( idx >= ThreadCount ) { return true; }
	
	const size_t ThreadIdx=idx;
	if ( NULL == ConveyerAutoRunThreadHandle[ThreadIdx] ) { return true; }
	if ( NULL != ConveyerAutoRunThreadEvent[ThreadIdx] )
	{	::ResetEvent(ConveyerAutoRunThreadEvent[ThreadIdx]); }
	if ( THREAD_STATE_FINISH == m_ConveyerAutoRunThreadState[ThreadIdx] )
	{  SetConveyerAutoRunThreadState(ThreadIdx, THREAD_STATE_IDLE); }
	SetConveyerAutoRunThreadCmd(ThreadIdx, THREAD_COMMAND_TO_RUN);	

	if ( false == WaitOn )
	{	return true; }
	if ( WaitForConveyerAutoRunThreadStart(idx) == false )
	{	return false; }	
#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartConveyerAutoRunThread(size_t idx, ONLINE_STATE_MODE State, CAOIProject *ProjectPtr, bool bPreRun, bool WaitOn)//開始軌道自動運轉執行緒		
{
	SetConveyerAutoRunState(idx, State);
	SetConveyerAutoRunPreRunMode(idx, bPreRun);
	SetConveyerAutoRunProjectPtr(idx, ProjectPtr);		
	if ( StartConveyerAutoRunThreadFn(idx, WaitOn) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForConveyerAutoRunThreadIdle()//等待軌道自動運轉執行緒停止
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;		
	const size_t ThreadCount   = MAX_THREAD_COUNT_LANE_AUTO_RUN;
	const size_t MaxCounts = ThreadCount*100+100;		
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			//if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_STATE_NONE == m_ConveyerAutoRunThreadState[ThreadIdx] )
			{	break; }
			if ( THREAD_STATE_IDLE == m_ConveyerAutoRunThreadState[ThreadIdx] )
			{	break; }		
			if ( SleepTime > 0 )
			{	::Sleep(SleepTime); }
		}
		if ( i == MaxCounts )
		{
			this->m_ErrorString.Format(_T("Error, Wait for WaitForConveyerAutoRunThreadIdle[%d] too long"), ThreadIdx+1);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE);
			return false;
		}	
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForConveyerAutoRunThreadStop()//等待軌道自動運轉執行緒停止
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;		
	const size_t ThreadCount   = MAX_THREAD_COUNT_LANE_AUTO_RUN;
	const size_t MaxCounts = ThreadCount*100+100;		
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			//if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_STATE_RUNNING != m_ConveyerAutoRunThreadState[ThreadIdx] )
			{	break; }			
			if ( SleepTime > 0 )
			{	::Sleep(SleepTime); }
		}
		if ( i == MaxCounts )
		{
			this->m_ErrorString.Format(_T("Error, Wait for WaitForConveyerAutoRunThreadStop[%d] too long"), ThreadIdx+1);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP);
			return false;
		}	
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForConveyerAutoRunThreadStart(size_t idx)//等待軌道自動運轉執行緒開始
{
#ifndef OFFLINE_VERSION
	size_t i=0, j=0;	
	const size_t ThreadIdx=idx;
	const size_t MaxCount = 1000;
	const size_t SleepTime = 10;
	for ( i=0; i<MaxCount; i++ )
	{
		if ( THREAD_COMMAND_TO_RUN != m_ConveyerAutoRunThreadCmd[ThreadIdx] )//切入下一個階段
		{	break; }
		if ( THREAD_STATE_IDLE != m_ConveyerAutoRunThreadState[ThreadIdx] )
		{	break; }
		::Sleep(SleepTime);
	}
	if ( i == MaxCount )
	{
		this->m_ErrorString.Format(_T("Error, wait for StartConveyerAutoRunThread[%d] too long"), ThreadIdx+1);			
		return false;
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForConveyerAutoRunThreadFinish()//等待軌道自動運轉執行緒結束
{
#ifndef OFFLINE_VERSION	
	size_t i=0, j=0;
	size_t ThreadIdx=0;
	DWORD Res = 0;		
	const DWORD SleepTime = 100;		
	const size_t ThreadCount   = MAX_THREAD_COUNT_LANE_AUTO_RUN;
	const size_t MaxCounts = ThreadCount*100+100;		
	for ( j=0; j<ThreadCount; j++ )
	{
		ThreadIdx = j;
		if ( ConveyerAutoRunThreadEvent[ThreadIdx] == NULL ) { continue; }
		for ( i=0; i<MaxCounts; i++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }				
			if ( GetIsSystemException() == true ) { return true; }
			if ( THREAD_COMMAND_TO_EXIT == m_ConveyerAutoRunThreadCmd[ThreadIdx] ) { return true; }
			if ( THREAD_COMMAND_TO_IDLE == m_ConveyerAutoRunThreadCmd[ThreadIdx] )
			{	break; }			

			Res = ::WaitForSingleObject(ConveyerAutoRunThreadEvent[ThreadIdx], SleepTime);
			if ( Res != WAIT_TIMEOUT ) 
			{
				if ( THREAD_STATE_RUNNING == m_ConveyerAutoRunThreadState[ThreadIdx] )
				{
					if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
					continue; 
				}
				break; 
			}
		}
		if ( i == MaxCounts )
		{
			this->m_ErrorString.Format(_T("Error, Wait for WaitForConveyerAutoRunThreadFinish[%d] too long"), ThreadIdx+1);
			SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
			return false;
		}	
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetConveyerAutoRunPreRunMode(size_t idx, bool value)//設定軌道自動運轉-提前運轉
{
	if ( idx >= MAX_THREAD_COUNT_LANE_AUTO_RUN ) { return; }	
	//LockThreadConveyerAutoRun();
	m_ConveyerAutoRunPreRunMode[idx] = value;
	//UnlockThreadConveyerAutoRun();
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::GetConveyerAutoRunPreRunMode(size_t idx) const//取得軌道自動運轉-提前運轉
{
	if ( idx >= MAX_THREAD_COUNT_LANE_AUTO_RUN ) { return m_ConveyerAutoRunPreRunMode[0]; }
	return m_ConveyerAutoRunPreRunMode[idx];
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetConveyerAutoRunState(size_t idx, ONLINE_STATE_MODE State)//設定軌道自動運轉狀態
{
	if ( idx >= MAX_THREAD_COUNT_LANE_AUTO_RUN ) { return; }
	if ( m_ConveyerAutoRunState[idx] == State ) { return; }
	//LockThreadConveyerAutoRun();
	m_ConveyerAutoRunState[idx] = State;
	//UnlockThreadConveyerAutoRun();
}
//-------------------------------------------------------------------------------------//
ONLINE_STATE_MODE CAOIDataCollect::GetConveyerAutoRunState(size_t idx)//取得軌道自動運轉狀態
{
	if ( idx >= MAX_THREAD_COUNT_LANE_AUTO_RUN ) { return m_ConveyerAutoRunState[0]; }
	return m_ConveyerAutoRunState[idx];
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetConveyerAutoRunProjectPtr(size_t idx, CAOIProject *Ptr)//設定軌道自動運轉專案
{
	if ( idx >= MAX_THREAD_COUNT_LANE_AUTO_RUN ) { return; }	
	//LockThreadConveyerAutoRun();
	m_ConveyerAutoRunProjectPtr[idx] = Ptr;
	//UnlockThreadConveyerAutoRun();
}
//-------------------------------------------------------------------------------------//
CAOIProject* CAOIDataCollect::GetConveyerAutoRunProjectPtr(size_t idx)//取得軌道自動運轉專案
{
	if ( idx >= MAX_THREAD_COUNT_LANE_AUTO_RUN ) { return m_ConveyerAutoRunProjectPtr[0]; }
	return m_ConveyerAutoRunProjectPtr[idx];
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckConveyerAutoRunIdle(size_t idx)//確認軌道自動運轉閒置
{
	THREAD_STATE_MODE ThreadState = GetConveyerAutoRunThreadState(idx);
	if (THREAD_STATE_IDLE != ThreadState)
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckConveyerAutoRunFinish(size_t idx)//確認軌道自動運轉完成
{
	THREAD_STATE_MODE ThreadState=GetConveyerAutoRunThreadState(idx);
	if ( THREAD_STATE_FINISH != ThreadState )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckConveyerAutoRunException(size_t idx)//確認軌道自動運轉異常
{
	THREAD_STATE_MODE ThreadState=GetConveyerAutoRunThreadState(idx);
	if ( THREAD_STATE_EXCEPTION != ThreadState )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetConveyerAutoRunThreadState(size_t idx, THREAD_STATE_MODE State)//設定軌道自動運轉執行緒狀態
{
	if ( idx >= MAX_THREAD_COUNT_LANE_AUTO_RUN ) { return; }
	if ( m_ConveyerAutoRunThreadState[idx] == State ) { return; }
	LockThreadConveyerAutoRun();
	m_ConveyerAutoRunThreadState[idx] = State;
	UnlockThreadConveyerAutoRun();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE CAOIDataCollect::GetConveyerAutoRunThreadState(size_t idx)//取得軌道自動運轉執行緒狀態
{
	if ( idx >= MAX_THREAD_COUNT_LANE_AUTO_RUN ) { return m_ConveyerAutoRunThreadState[0]; }
	return m_ConveyerAutoRunThreadState[idx];	
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetConveyerAutoRunThreadCmd(size_t idx, THREAD_COMMAND_MODE Cmd)//設定軌道自動運轉執行緒命令
{
	if ( idx >= MAX_THREAD_COUNT_LANE_AUTO_RUN ) { return; }
	if ( m_ConveyerAutoRunThreadCmd[idx] == Cmd ) { return; }
	LockThreadConveyerAutoRun();
	m_ConveyerAutoRunThreadCmd[idx] = Cmd;
	UnlockThreadConveyerAutoRun();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CAOIDataCollect::GetConveyerAutoRunThreadCmd(size_t idx)//取得軌道自動運轉執行緒命令	
{
	if ( idx >= MAX_THREAD_COUNT_LANE_AUTO_RUN ) { return m_ConveyerAutoRunThreadCmd[0]; }
	return m_ConveyerAutoRunThreadCmd[idx];
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateMESProcThread()//建立MES執行執行緒
{
#ifndef MES_DISABLE
	if ( MES_OBJ.CreateMESProcThread() == false )
	{
		m_ErrorString = MES_OBJ.GetErrorString();
		return false;
	}
#endif//MES_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteMESProcThread()//刪除MES執行執行緒
{
#ifndef MES_DISABLE
	if ( MES_OBJ.DeleteMESProcThread() == false )
	{
		m_ErrorString = MES_OBJ.GetErrorString();
		return false;
	}
#endif//MES_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecLoadRepairFileFn()//執行載入維修站檔案執行緒
{		
#ifndef OFFLINE_VERSION	
	size_t                       i=0, j=0, k=0;
	size_t                       szParts=0;
	size_t                       FileCount=0;
	int                          nValue=0;
	int                          ResultLen=0;
	double                       dValue=0.0;	
	wchar_t                      wcKey[MAX_JET_PATH]=L"";
	RESULT_ID                    eResultID;
	WND_DEFECT_ID                eDefectID;	
	std::vector<WND_DEFECT_ID>   eDefectList;	
	UUID                         uuidComponent;
	unsigned int                 PanelIndex=0, BoardIndex=0;
	char                         DateTime[32]="";
	CTime                        cTestTime;	
	LANE_ID                      eLaneID;	
	std::wstring                 wsComponentName;
	std::wstring                 wsKey=L"";
	std::wstring                 wsKey2=L"";
	std::wstring                 wsObjName=L"";
	std::wstring                 wsValue=L"";
	std::wstring                 wsValue2=L"";
	std::wstring                 wsFileName=L"";
	CString                      str;
	CString                      str2;
	CString                      FileName;	
	CString                      ResultFolder;	
	CString                      strComponentName;	
	TRepairResultNode            RepairResultNode;//維修站結果
	CAOIPanel                   *PanelPtr=NULL;
	CAOIBoard                   *BoardPtr=NULL;
	CAOIComponent               *ComponentPtr=NULL;		
	const DWORD                  dwSleepTime=100;
	WIN32_FIND_DATA              File;
	std::vector<WIN32_FIND_DATA> FileList;
	THREAD_COMMAND_MODE          ThreadCmdMode;
	CAOIProject                 *ProjectPtr = NULL;
	const size_t ProjectCount = GetProjectPtrCount();
	const SAVE_SPC_FILE_MODE    SaveSpcFileMode=GetSaveProjectSpcFileMode();

	for ( i=0; i<ProjectCount; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }
		if ( GetIsSystemException() == true ) { return true; }
		ThreadCmdMode = GetLoadRepairFileThreadCmd();
		if ( THREAD_COMMAND_TO_IDLE==ThreadCmdMode || THREAD_COMMAND_TO_EXIT==ThreadCmdMode )
		{	return true; }

		if ( dwSleepTime > 0 ) 
		{	::Sleep(dwSleepTime); }

		ProjectPtr = GetProjectPtr(i, false);
		if ( NULL == ProjectPtr ) { continue; }
		ResultFolder = ProjectPtr->GetProjectSpcResultFolder();
		ResultLen = ResultFolder.GetLength();
		if ( 0 == ResultLen ) { continue; }
		FileList.clear();
		JetAPI::ListFilesInFolder(ResultFolder, _T("JSON"), FileList);
		FileCount = FileList.size();
		if ( 0 == FileCount ) { continue; }
		for ( j=0; j<FileCount; j++ )
		{
			if ( GetIsSystemReleased() == true ) { return true; }
			if ( GetIsSystemException() == true ) { return true; }
			ThreadCmdMode = GetLoadRepairFileThreadCmd();
			if ( THREAD_COMMAND_TO_IDLE==ThreadCmdMode || THREAD_COMMAND_TO_EXIT==ThreadCmdMode )
			{	return true; }
			
			File = FileList[j];
			str.Format(_T("Check [%s] File"), File.cFileName);
			SaveControlCenterLog(str);

			FileName.Format(_T("%s\\%s"), ResultFolder, File.cFileName);
			JetAPI::TCHAR2wstring(FileName, wsFileName);				
			rapidjson::CGMItr itr;
			rapidjson::CGMItr itrLv2;
			rapidjson::WDocument Doc;
			rapidjson::CJsonCtrl JSonCtrl;

			//initial		
			Doc.SetObject();
			JSonCtrl.Set(&Doc);					
			if ( JSonCtrl.OpenFile(wsFileName.c_str(), Doc) == false ) 
			{
				str.Format(_T("Read [%s] File Fault"), File.cFileName);
				SaveControlCenterLog(str);
				//::DeleteFile(FileName);
				//str.Format(_T("Delete [%s] File"), File.cFileName);
				//SaveControlCenterLog(str);
				continue; 
			}
			str.Format(_T("Read [%s] File Success"), File.cFileName);
			SaveControlCenterLog(str);
			::DeleteFile(FileName);					
			str.Format(_T("Delete [%s] File"), File.cFileName);
			SaveControlCenterLog(str);

			//Test_Time
			if ( JSonCtrl.FindMember(L"Test_Time", itr) == false ) 
			{	continue; }				
			if ( itr->value.IsString() == false ) 
			{	continue; }
			wsValue = itr->value.GetString();
			JetAPI::wchar2char(wsValue.c_str(), DateTime, 32);
			JetAPI::GetTime(DateTime, cTestTime);

			//Lane
			if ( JSonCtrl.FindMember(L"Lane", itr) == false ) 
			{	continue; }				
			if ( itr->value.IsInt() == false ) 
			{	continue; }
			nValue = itr->value.GetInt();
			eLaneID = (LANE_ID)(nValue);		
			ProjectPtr->InitialProjectResultARS(eLaneID);

			//Check_Result, Result		
			if ( JSonCtrl.FindMember(L"Result", itr) == false ) 
			{	continue; }				
			if ( itr->value.IsInt() == false ) 
			{	continue; }
			nValue = itr->value.GetInt();		
			eResultID = (MapSpcResultIDToResultID((SPC_RESULT_ID)(nValue)));

			//N_NgParts
			if ( JSonCtrl.FindMember(L"N_Parts", itr) == false ) 
			{	continue; }				
			if ( itr->value.IsInt() == false ) 
			{	continue; }
			szParts = itr->value.GetInt();
			for ( k=0; k<szParts; k++ )
			{
				::swprintf(wcKey, L"Part_%d", k+1);
				wsObjName = wcKey;
				if ( JSonCtrl.FindMember(wsObjName, itr) == false ) 
				{	continue; }			
			
				PanelPtr=NULL;
				BoardPtr=NULL;
				ComponentPtr=NULL;

				eDefectID=WND_DEFECT_NONE;	
				eDefectList.clear();			
				wsComponentName = L"";
				PanelIndex = BoardIndex = -1;			
				JetAPI::InitialUUID(uuidComponent);

				wsKey = itr->name.GetString();
				itrLv2 = itr->value.FindMember(L"PanelID");
				if ( itrLv2 == itr->value.MemberEnd() )
				{	continue; }			
				if ( itrLv2->value.IsInt() == false ) 
				{	continue; }
				PanelIndex = itrLv2->value.GetInt()-1;

				itrLv2 = itr->value.FindMember(L"BoardID");
				if ( itrLv2 == itr->value.MemberEnd() )
				{	continue; }			
				if ( itrLv2->value.IsInt() == false ) 
				{	continue; }
				BoardIndex = itrLv2->value.GetInt()-1;

				itrLv2 = itr->value.FindMember(L"Name");
				if ( itrLv2 == itr->value.MemberEnd() )
				{	continue; }			
				if ( itrLv2->value.IsString() == false ) 
				{	continue; }
				wsComponentName = itrLv2->value.GetString();
			#ifdef _DEBUG
				strComponentName = wsComponentName.c_str();
			#endif//_DEBUG

				if ( SAVE_SPC_FILE_JSON_VRS == SaveSpcFileMode )
				{
					itrLv2 = itr->value.FindMember(L"UUID");
					if ( itrLv2 == itr->value.MemberEnd() )
					{	continue; }			
					if ( itrLv2->value.IsString() == false ) 
					{	continue; }
					wsValue2 = itrLv2->value.GetString();
					JetAPI::UUIDFromStringW(wsValue2.c_str(), uuidComponent);
				}

				itrLv2 = itr->value.FindMember(L"Main_Defect");
				if ( itrLv2 == itr->value.MemberEnd() )
				{	continue; }			
				if ( itrLv2->value.IsInt() == false ) 
				{	continue; }
				eDefectID = (WND_DEFECT_ID)(itrLv2->value.GetInt());

				itrLv2 = itr->value.FindMember(L"Check_Defect");
				if ( itrLv2 == itr->value.MemberEnd() )
				{	continue; }			
				if ( itrLv2->value.IsArray() == false ) 
				{	continue; }
				for (auto itrLv3 = itrLv2->value.Begin();  itrLv3!= itrLv2->value.End(); ++itrLv3)
				{
					if ( itrLv3->IsInt() == false ) 
					{	continue; }
					eDefectList.push_back((WND_DEFECT_ID)(itrLv3->GetInt()));						
				}
				if ( SAVE_SPC_FILE_JSON_VRS == SaveSpcFileMode )
				{	ComponentPtr = ProjectPtr->GetProjectComponentPtrByComponentUUID(uuidComponent); }
				else
				{	ComponentPtr = ProjectPtr->GetProjectComponentPtrByComponentFullName(PanelIndex, BoardIndex, wsComponentName.c_str());	}
				if ( NULL == ComponentPtr ) { continue; }					
			#ifdef _DEBUG
				if ( ComponentPtr->GetComponentPanelIndex_Project() != PanelIndex || 
					 ComponentPtr->GetComponentBoardIndex_Panel() != BoardIndex )
				{
				}
				if ( strComponentName.CompareNoCase(ComponentPtr->GetComponentName()) != 0 ) 
				{
					str2 = ComponentPtr->GetComponentName();
					str.Format(_T("UUID Name(%s), Load Name(%s)"), strComponentName, str2);
					str2.Format(_T("Error, ExecLoadRepairFileFn Fault (%s)"), str);
					JetAPI::ShowMessageBox(str2);
				}
			#endif//_DEBUG			
				ComponentPtr->UpdateComponentCurrentDefectCountARS(eLaneID, eDefectID, eDefectList);
			}
			//可能零件會對不起來, 使用ARS判定的結果-拋件, 刮傷, 定位點
			ProjectPtr->CalcProjectResultLatest_ARS(eLaneID, eResultID, cTestTime);
			ProjectPtr->AddProjectStatisticRecords_ARS(eLaneID);
			ProjectPtr->AnalyzeProjectStatisticRecords_ARS(eLaneID);

			strcpy(RepairResultNode.sDateTime, DateTime);
			RepairResultNode.eLaneID = eLaneID;
			RepairResultNode.eResultID = eResultID;

			str.Format(_T("Read [%s] File Result [%s:%s]"), File.cFileName, AOIDataDefine.GetLaneIDText(eLaneID), AOIDataDefine.GetResultIDText(eResultID));
			SaveControlCenterLog(str);

			DEFECT_HANDLE_MODE DefectHandleMode = ProjectPtr->GetProjectParameter().m_DefectHandleMode;
			if ( DEFECT_HANDLE_WAIT_FOR_REPAIR==DefectHandleMode || DEFECT_HANDLE_CONTROL_CENTER==DefectHandleMode )
			{	AddRepairResultNode(RepairResultNode); }
		
			if ( dwSleepTime > 0 ) 
			{	::Sleep(dwSleepTime); }
		}	
				
		if (ExecHASI_SaveAOIInspectionResultFile_VPnP(ProjectPtr, false) == false)
		{	return false;	}
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateLoadRepairFileThread()//建立載入維修站檔案執行緒
{
#ifndef OFFLINE_VERSION
	if ( DeleteLoadRepairFileThread() == false ) { return false; }	
	m_LoadRepairFileThreadCmd = THREAD_COMMAND_TO_IDLE;	

	LoadRepairFileThreadHandle = (HANDLE)::_beginthreadex(NULL, NULL, &LoadRepairFileThreadFn, (void*)0, NULL, &LoadRepairFileThreadID);	
	if ( NULL == LoadRepairFileThreadHandle )
	{	
		m_LoadRepairFileThreadState = THREAD_STATE_NONE;
		m_ErrorString = _T("Eorror, Create Thread Fault (LoadRepairFileThreadHandle == NULL)");		
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_CREATE);
		return false;
	}		
	LoadRepairFileThreadEvent = JetAPI::CreateEvent(NULL, TRUE, TRUE, _T("Load Repair File Event"));		
	::SetThreadPriority(LoadRepairFileThreadHandle, THREAD_PRIORITY_BELOW_NORMAL); 
	//::SetThreadAffinityMask(LoadRepairFileThreadHandle, 0x03);//保留2個		
	//StartLoadRepairFileThread(false);	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteLoadRepairFileThread()//刪除載入維修站檔案執行緒
{
#ifndef OFFLINE_VERSION
	DWORD WaitTime = 30000;//30 sec	
	if ( NULL == LoadRepairFileThreadHandle ) { return true; }		
	SetLoadRepairFileThreadCmd(THREAD_COMMAND_TO_EXIT);	
	DWORD Res = ::WaitForSingleObject(LoadRepairFileThreadHandle, WaitTime);
	::CloseHandle(LoadRepairFileThreadHandle); 
	LoadRepairFileThreadHandle = NULL;
	if ( NULL != LoadRepairFileThreadEvent )
	{	
		::CloseHandle(LoadRepairFileThreadEvent); 
		LoadRepairFileThreadEvent=NULL; 
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StopLoadRepairFileThread(bool WaitOn)//停止載入維修站檔案執行緒
{
#ifndef OFFLINE_VERSION
	size_t i=0;
	if ( NULL == LoadRepairFileThreadHandle ) { return true; }
	if ( NULL != LoadRepairFileThreadEvent )
	{	::ResetEvent(LoadRepairFileThreadEvent); }
	SetLoadRepairFileThreadCmd(THREAD_COMMAND_TO_IDLE);	

	if ( WaitOn == true ) 
	{	
		const size_t MaxCount = 1000;
		const size_t SleepTime = 20;
		for ( i=0; i<MaxCount; i++ )
		{	
			if ( THREAD_STATE_NONE == m_LoadRepairFileThreadState )
			{	break; }
			if ( THREAD_STATE_IDLE == m_LoadRepairFileThreadState )
			{	break; }
			//if ( THREAD_STATE_FINISH == m_LoadRepairFileThreadState )//有可能後面才完成, 所以要加上完成確認
			//{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			this->m_ErrorString.Format(_T("Error, wait for StopLoadRepairFileThread too long"));			
			return false;
		}		
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartLoadRepairFileThread(bool WaitOn)//開始載入維修站檔案執行緒	
{
#ifndef OFFLINE_VERSION
	size_t i=0;
	if ( NULL == LoadRepairFileThreadHandle ) { return true; }
	if ( NULL != LoadRepairFileThreadEvent )
	{	::ResetEvent(LoadRepairFileThreadEvent); }
	if ( THREAD_STATE_FINISH == m_LoadRepairFileThreadState )
	{  SetLoadRepairFileThreadState(THREAD_STATE_IDLE); }
	SetLoadRepairFileThreadCmd(THREAD_COMMAND_TO_RUN);	

	if ( false == WaitOn )
	{	return true; }
	if ( WaitForLoadRepairFileThreadStart() == false )
	{	return false; }	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForLoadRepairFileThreadIdle()//等待載入維修站檔案執行緒停止
{
#ifndef OFFLINE_VERSION
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		//if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_NONE == m_LoadRepairFileThreadState ) { return true; }		
		if ( THREAD_STATE_IDLE == m_LoadRepairFileThreadState ) { return true; }

		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }		
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForLoadRepairFileThreadIdle too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE);
		return false;
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForLoadRepairFileThreadStop()//等待載入維修站檔案執行緒停止
{
#ifndef OFFLINE_VERSION
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		//if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_RUNNING != m_LoadRepairFileThreadState ) { return true; }	

		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }		
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForLoadRepairFileThreadStop too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP);
		return false;
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForLoadRepairFileThreadStart()//等待載入維修站檔案執行緒開始
{
#ifndef OFFLINE_VERSION		
	size_t i=0;
	const size_t MaxCount = 1000;
	const size_t SleepTime = 20;
	for ( i=0; i<MaxCount; i++ )
	{
		if ( THREAD_COMMAND_TO_RUN != m_LoadRepairFileThreadCmd )//切入下一個階段
		{	break; }
		if ( THREAD_STATE_IDLE != m_LoadRepairFileThreadState )
		{	break; }
		::Sleep(SleepTime);
	}
	if ( i == MaxCount )
	{
		m_ErrorString.Format(_T("Error, wait for StartLoadRepairFileThread too long"));			
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_START);	
		return false;
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForLoadRepairFileThreadFinish()//等待載入維修站檔案執行緒結束
{
#ifndef OFFLINE_VERSION
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;

	if ( NULL == LoadRepairFileThreadEvent ) { return true; }
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == m_LoadRepairFileThreadCmd ) { return true; }		
		if ( THREAD_COMMAND_TO_IDLE == m_LoadRepairFileThreadCmd )
		{	break; }

		Res = ::WaitForSingleObject(LoadRepairFileThreadEvent, SleepTime);
		if ( Res != WAIT_TIMEOUT ) 
		{
			if ( THREAD_STATE_RUNNING == m_LoadRepairFileThreadState )
			{
				if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
				continue; 
			}
			break; 
		}
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForLoadRepairFileThreadFinish too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
		return false;
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckLoadRepairFileThreadState(THREAD_STATE_MODE State)//確認載入維修站檔案執行緒狀態
{
	if ( NULL == LoadRepairFileThreadHandle ) { return true; }
	if ( m_LoadRepairFileThreadState != State )
	{
		m_ErrorString.Format(_T("Error, m_LoadRepairFileThreadState is Exception"));
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetLoadRepairFileThreadState(THREAD_STATE_MODE State)//設定載入維修站檔案執行緒狀態
{
	if ( m_LoadRepairFileThreadState == State ) { return; }
	LockThreadLoadRepairFile();
	m_LoadRepairFileThreadState = State;
	UnlockThreadLoadRepairFile();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE CAOIDataCollect::GetLoadRepairFileThreadState()//取得載入維修站檔案執行緒狀態
{
	return m_LoadRepairFileThreadState;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetLoadRepairFileThreadCmd(THREAD_COMMAND_MODE Cmd)//設定載入維修站檔案執行緒命令
{
	if ( m_LoadRepairFileThreadCmd == Cmd ) { return; }
	LockThreadLoadRepairFile();
	m_LoadRepairFileThreadCmd = Cmd;
	UnlockThreadLoadRepairFile();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CAOIDataCollect::GetLoadRepairFileThreadCmd()//取得載入維修站檔案執行緒命令
{
	return m_LoadRepairFileThreadCmd;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecRepairResultSignalFn()//執行維修站結果訊號執行緒
{
#ifndef OFFLINE_VERSION
	LANE_ID    eLaneID;	
	const bool bThread = true;
	TASK_MODE           eTaskMode;	
	TASK_STATE_MODE     eTaskStateMode;
	THREAD_COMMAND_MODE    ThreadCmd;	
	while ( true )
	{
		ThreadCmd = GetRepairResultSignalThreadCmd();
		if ( GetIsSystemReleased() == true ) { return true; }
		if ( THREAD_COMMAND_TO_IDLE == ThreadCmd ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == ThreadCmd ) { return true; }		
		if ( THREAD_COMMAND_TO_NONE == ThreadCmd ) { return true; }

		eTaskMode = GetTaskMode();		
		eTaskStateMode = GetOnlineTaskState();				
		if ( TASK_INSPECT_PROJECT != eTaskMode ) 
		{
			::Sleep(100);
			continue;
		}
		if ( TASK_STATE_RUNNING != eTaskStateMode ) 
		{
			::Sleep(100);
			continue;
		}

		//確認A軌道
		eLaneID = LANE_ID_A;		
		if ( ExecRepairResultSignalLaneFn(eLaneID) == false )
		{	return false; }		

		//確認B軌道
		eLaneID = LANE_ID_B;
		if ( ExecRepairResultSignalLaneFn(eLaneID) == false )
		{	return false; }
		::Sleep(100);
	}
#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecRepairResultSignalLaneFn(LANE_ID LaneID) //執行維修站結果訊號執行緒-軌道
{	
	const LANE_ID            eLaneID = LaneID;
	const PCB_OUT_MODE       ePCBOutMode = GetLanePCBOutMode(eLaneID);//20230425
	const LANE_WORK_MODE     eLaneWorkMode = GetLaneWorkMode(eLaneID);			
	const DEFECT_HANDLE_MODE eDefectHandleMode = GetLaneDefectHandleMode(eLaneID);
	if ( LANE_WORK_RUN == eLaneWorkMode )
	{
		TEST_RESULT_ID TestResultID = TEST_RESULT_NONE;
		if ( DEFECT_HANDLE_WAIT_FOR_REPAIR == eDefectHandleMode )
		{	
			const bool bDualRunMode = CheckDualRunMode();//20230425	
			const MULTI_LANE_MODE eMultiLaneMode = CheckMultiLaneMode();
			if ( MULTI_LANE_2==eMultiLaneMode && PCB_OUT_LANE_AUTO==ePCBOutMode )//雙軌道模式使用
			{
				if ( false == bDualRunMode )
				{
					ExecRepairResultSignalLaneRepairFn(LaneID, TestResultID);	
					if ( TEST_RESULT_NONE != TestResultID )
					{
						const PCB_OUT_DIRECTION  ePCBOutDirection = GetPCBOutDirection();	
						ExecPCBAutoRunProc(eLaneID, false, false, ePCBOutDirection);						
					}
				}
			}
		}

		if ( DEFECT_HANDLE_CONTROL_CENTER == eDefectHandleMode )
		{	ExecRepairResultSignalLaneControlCenterFn(LaneID, TestResultID);	}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecRepairResultSignalLaneRepairFn(LANE_ID LaneID, TEST_RESULT_ID &TestResultID) //執行維修站結果訊號執行緒-軌道-維修站
{		
	bool       bTestNG=false;	
	char       DateTime[32]="";
	TRepairResultNode RepairResultNode;		
	const LANE_ID     eLaneID = LaneID;

	TestResultID = TEST_RESULT_NONE;
	const bool bEmpty = GetIsRepairDateTimeToCheckEmpty(eLaneID);//是否有維修站資料
	if ( true == bEmpty ) 
	{	return true; }

	if ( GetRepairDateTimeToCheck(eLaneID, DateTime) == false ) 
	{	return true; }

	if ( GetRepairResultNodeByDateTime(DateTime, RepairResultNode) == false ) 
	{	return true; }

	if ( RepairResultNode.eLaneID != eLaneID )//Double Check Lane ID
	{	return true; }

	RemoveRepairResultNode(eLaneID, DateTime);
	if ( RESULT_ID_NG==RepairResultNode.eResultID || RESULT_ID_EXCEPTION==RepairResultNode.eResultID )
	{	bTestNG = true;	}
	else
	{	bTestNG = false;	}

	if ( true == bTestNG )
	{	TestResultID = TEST_RESULT_NG;	}
	else
	{	TestResultID = TEST_RESULT_OK;	}	
	SendPCBOKNGSignal(eLaneID, bTestNG);
	ClearRepairDateTimeToCheck(eLaneID);
	PlcCtrlPtr->TurnOffInspectAlarm(eLaneID);
	PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecRepairResultSignalLaneControlCenterFn(LANE_ID LaneID, TEST_RESULT_ID &TestResultID) //執行維修站結果訊號執行緒-軌道-中央控制
{
	bool       bLock;
	bool       bEmpty;	
	bool       bTestNG=false;	
	char       DateTime[32]="";	
	TRepairResultNode  RepairResultNode;	
	const LANE_ID      eLaneID = LaneID;	
	const PCB_OUT_MODE ePcbOutMode= GetLanePCBOutModeRunning(eLaneID);//GetLanePCBOutMode
	const int          nLaneStatus=PlcCtrlPtr->GetConveryerStatus(eLaneID);		
	
	TestResultID = TEST_RESULT_NONE;
	if ( PCB_OUT_LANE_AUTO == ePcbOutMode )
	{
		if ( PLC_CONVERYER_STATUS_PCB_OUT_RUNNING == nLaneStatus || 
			 PLC_CONVERYER_STATUS_PCB_BACK_RUNNING == nLaneStatus )
		{
			bLock = GetIsCCSDataTimeLockPCBInOut(eLaneID);
			bEmpty = GetIsCCSDateTimeFinishEmpty(eLaneID);
			if ( false==bEmpty && false==bLock ) 
			{	
				SetCCSDataTimeLockTest(eLaneID, true);
				SaveControlCenterLog(_T("PLC_CONVERYER_STATUS_PCB_OUT_RUNNING"));
				ResetPCBOKNGSignal(eLaneID);
				ShiftCCSDateTime(eLaneID);
				SetCCSDataTimeLockTest(eLaneID, false);
				PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);
			}
		}
	}
	bEmpty = GetIsCCSDateTimeToCheckEmpty(eLaneID);//是否有中控資料
	if ( true == bEmpty )
	{	return true; }
				
	::memset(DateTime, 0x00, sizeof(DateTime));
	if ( GetCCSDateTimeToCheck(eLaneID, DateTime) == false ) 
	{	return true; }

	if ( GetRepairResultNodeByDateTime(DateTime, RepairResultNode) == false ) 
	{	return true; }

	if ( RepairResultNode.eLaneID != eLaneID )//Double Check Lane ID
	{	return true; }

	RemoveRepairResultNode(eLaneID, DateTime);
	if ( RESULT_ID_NG==RepairResultNode.eResultID || RESULT_ID_EXCEPTION==RepairResultNode.eResultID )
	{	bTestNG = true;	}
	else
	{	bTestNG = false;	}

	if ( true == bTestNG )
	{	TestResultID = TEST_RESULT_NG; }
	else
	{	TestResultID = TEST_RESULT_OK; }	
	SendPCBOKNGSignal(eLaneID, bTestNG);
	SetCCSDateTimeToCheck(eLaneID, (""));	
	PostMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_REDRAW_VIEW_WND, NULL);			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateRepairResultSignalThread()//建立維修站結果訊號執行緒
{
#ifndef OFFLINE_VERSION
	if ( DeleteRepairResultSignalThread() == false ) { return false; }	
	m_RepairResultSignalThreadCmd = THREAD_COMMAND_TO_IDLE;	

	RepairResultSignalThreadHandle = (HANDLE)::_beginthreadex(NULL, NULL, &RepairResultSignalThreadFn, (void*)0, NULL, &RepairResultSignalThreadID);	
	if ( NULL == RepairResultSignalThreadHandle )
	{	
		m_RepairResultSignalThreadState = THREAD_STATE_NONE;
		m_ErrorString = _T("Eorror, Create Thread Fault (RepairResultSignalThreadHandle == NULL)");		
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_CREATE);
		return false;
	}		
	RepairResultSignalThreadEvent = JetAPI::CreateEvent(NULL, TRUE, TRUE, _T("Repair Result Signal Event"));		
	::SetThreadPriority(RepairResultSignalThreadHandle, THREAD_PRIORITY_BELOW_NORMAL); 
	//::SetThreadAffinityMask(RepairResultSignalThreadHandle, 0x03);//保留2個		
	StartRepairResultSignalThread(false);	
#else
	RepairResultSignalThreadHandle = NULL;
	RepairResultSignalThreadEvent = NULL;
#endif//OFFLINE_VERSION
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteRepairResultSignalThread()//刪除維修站結果訊號執行緒
{
#ifndef OFFLINE_VERSION
	DWORD WaitTime = 10000;//1 sec	
	if ( NULL == RepairResultSignalThreadHandle ) { return true; }		
	SetRepairResultSignalThreadCmd(THREAD_COMMAND_TO_EXIT);	
	DWORD Res = ::WaitForSingleObject(RepairResultSignalThreadHandle, WaitTime);
	::CloseHandle(RepairResultSignalThreadHandle); 
	RepairResultSignalThreadHandle = NULL;
	if ( NULL != RepairResultSignalThreadEvent )
	{	
		::CloseHandle(RepairResultSignalThreadEvent); 
		RepairResultSignalThreadEvent=NULL; 
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StopRepairResultSignalThread(bool WaitOn)//開始維修站結果訊號執行緒
{
#ifndef OFFLINE_VERSION
	size_t i=0;
	if ( NULL == RepairResultSignalThreadHandle ) { return true; }
	if ( NULL != RepairResultSignalThreadEvent )
	{	::ResetEvent(RepairResultSignalThreadEvent); }
	SetRepairResultSignalThreadCmd(THREAD_COMMAND_TO_IDLE);	

	if ( WaitOn == true ) 
	{	
		const size_t MaxCount = 100;
		const size_t SleepTime = 10;
		for ( i=0; i<MaxCount; i++ )
		{	
			if ( THREAD_STATE_NONE == m_RepairResultSignalThreadState )
			{	break; }
			if ( THREAD_STATE_IDLE == m_RepairResultSignalThreadState )
			{	break; }
			//if ( THREAD_STATE_FINISH == m_RepairResultSignalThreadState )//有可能後面才完成, 所以要加上完成確認
			//{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			this->m_ErrorString.Format(_T("Error, wait for StopRepairResultSignalThread too long"));			
			return false;
		}		
	}
#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartRepairResultSignalThread(bool WaitOn)//開始維修站結果訊號執行緒
{
#ifndef OFFLINE_VERSION
	size_t i=0;
	if ( NULL == RepairResultSignalThreadHandle ) { return true; }
	if ( NULL != RepairResultSignalThreadEvent )
	{	::ResetEvent(RepairResultSignalThreadEvent); }
	if ( THREAD_STATE_FINISH == m_RepairResultSignalThreadState )
	{  SetRepairResultSignalThreadState(THREAD_STATE_IDLE); }
	SetRepairResultSignalThreadCmd(THREAD_COMMAND_TO_RUN);	

	if ( false == WaitOn )
	{	return true; }
	if ( WaitForRepairResultSignalThreadStart() == false )
	{	return false; }	
#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForRepairResultSignalThreadIdle()//等待維修站結果訊號執行緒停止
{
#ifndef OFFLINE_VERSION
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		//if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_NONE == m_RepairResultSignalThreadState ) { return true; }
		if ( THREAD_STATE_IDLE == m_RepairResultSignalThreadState ) { return true; }
		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }		
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForRepairResultSignalThreadIdle too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE);
		return false;
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForRepairResultSignalThreadStop()//等待維修站結果訊號執行緒停止
{
#ifndef OFFLINE_VERSION
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		//if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_RUNNING != m_RepairResultSignalThreadState ) { return true; }		
		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }		
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForRepairResultSignalThreadStop too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP);
		return false;
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForRepairResultSignalThreadStart()//等待維修站結果訊號執行緒開始
{
#ifndef OFFLINE_VERSION	
	size_t i=0;
	const size_t MaxCount = 100;
	const size_t SleepTime = 10;
	for ( i=0; i<MaxCount; i++ )
	{
		if ( THREAD_COMMAND_TO_RUN != m_RepairResultSignalThreadCmd )//切入下一個階段
		{	break; }
		if ( THREAD_STATE_IDLE != m_RepairResultSignalThreadState )
		{	break; }
		::Sleep(SleepTime);
	}
	if ( i == MaxCount )
	{
		m_ErrorString.Format(_T("Error, wait for StartRepairResultSignalThread too long"));		
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_START);	
		return false;
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForRepairResultSignalThreadFinish()//等待維修站結果訊號執行緒結束
{
#ifndef OFFLINE_VERSION
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;

	if ( NULL == RepairResultSignalThreadEvent ) { return true; }
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == m_RepairResultSignalThreadCmd ) { return true; }		
		if ( THREAD_COMMAND_TO_IDLE == m_RepairResultSignalThreadCmd )
		{	break; }

		Res = ::WaitForSingleObject(RepairResultSignalThreadEvent, SleepTime);
		if ( Res != WAIT_TIMEOUT ) 
		{
			if ( THREAD_STATE_RUNNING == m_RepairResultSignalThreadState )
			{
				if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
				continue; 
			}
			break; 
		}
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForRepairResultSignalThreadFinish too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
		return false;
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckRepairResultSignalThreadState(THREAD_STATE_MODE State)//確認維修站結果訊號執行緒狀態
{
	if ( NULL == RepairResultSignalThreadHandle ) { return true; }
	if ( m_RepairResultSignalThreadState != State )
	{
		m_ErrorString.Format(_T("Error, m_RepairResultSignalThreadState is Exception"));
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetRepairResultSignalThreadState(THREAD_STATE_MODE State)//設定維修站結果訊號執行緒狀態
{
	if ( m_RepairResultSignalThreadState == State ) { return; }
	LockThread();
	m_RepairResultSignalThreadState = State;
	UnlockThread();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE CAOIDataCollect::GetRepairResultSignalThreadState()//取得維修站結果訊號執行緒狀態
{
	return m_RepairResultSignalThreadState;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetRepairResultSignalThreadCmd(THREAD_COMMAND_MODE Cmd)//設定維修站結果訊號執行緒命令
{
	if ( m_RepairResultSignalThreadCmd == Cmd ) { return; }
	LockThread();
	m_RepairResultSignalThreadCmd = Cmd;
	UnlockThread();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CAOIDataCollect::GetRepairResultSignalThreadCmd()//取得維修站結果訊號執行緒命令
{
	return m_RepairResultSignalThreadCmd;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecSimpleJobFn()//執行維修站結果訊號執行緒
{
	bool IsOK = true;
#ifndef OFFLINE_VERSION			
	SIMPLE_JOB_MODE SimpleJob = GetSimpleJobMode();	
	switch ( SimpleJob )
	{
	case SIMPLE_JOB_MOVE_TO_DISTRICT_B:
		IsOK = ExecSimpleJobFn_MoveToDistrict();
		break;
	default:
	case SIMPLE_JOB_NULL:
		break;
	}
#endif//OFFLINE_VERSION	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::ExecSimpleJobFn_MoveToDistrict()//執行簡單工作執行緒
{
	LANE_ID LaneID = GetSimpleJob_LaneID();
	DISTRICT_ID DistrictID = GetSimpleJob_DistrictID();		
	if ( MovePCBToDistrictID(LaneID, DistrictID) == false ) 
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartSimpleJobFn_MoveToDistrict(LANE_ID LaneID, DISTRICT_ID DistrictID, bool bWait)//開始簡單工作-移至分段位置
{
#ifndef OFFLINE_VERSION
	if ( NULL == SimpleJobThreadHandle ) { return false; }

	SetSimpleJob_LaneID(LaneID);	
	SetSimpleJob_DistrictID(DistrictID);
	SetSimpleJobMode(SIMPLE_JOB_MOVE_TO_DISTRICT_B);
	if ( StartSimpleJobThread(bWait) == false ) 
	{	return false; }
#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForSimpleJobFn_MoveToDistrict()//等待簡單工作緒結束-移至分段位置
{
#ifndef OFFLINE_VERSION	
	/*
	if ( WaitForSimpleJobThreadFinish() == false ) 
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForSimpleJobFn_MoveToDistrict too long"));
		return false; 
	}	
	*/
	size_t i=0;	
	DWORD Res = 0;			
	bool CheckSensor = true;
	bool SensorPcbStop2On = false;
	bool SensorPcbStop2Off = false;	

	bool SensorPcbStop2 = false;	
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;
	LANE_ID      LaneID = GetSimpleJob_LaneID();
	DISTRICT_ID  DistrictID=GetSimpleJob_DistrictID();
	if ( NULL == SimpleJobThreadEvent ) { return true; }

	CheckSensor = true;
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == m_SimpleJobThreadCmd ) { return true; }		
		if ( THREAD_COMMAND_TO_IDLE == m_SimpleJobThreadCmd )
		{	break; }

		if ( true == CheckSensor )
		{
			//先往後移動關閉, 然後再移動碰到感應器
			//所以感應器是On->Off->On
			SensorPcbStop2 = PlcCtrlPtr->GetConveryerSensorPCBStop2(LaneID);
			if ( false == SensorPcbStop2Off )
			{
				if ( false == SensorPcbStop2 )
				{	SensorPcbStop2Off = true; }
			}
			if ( true==SensorPcbStop2Off && false==SensorPcbStop2On )
			{
				if ( true == SensorPcbStop2 )
				{	SensorPcbStop2On = true; }
			}
			if ( true == SensorPcbStop2On )
			{
				CAOIProject *ProjectPtr = GetActiveProject();
				if ( NULL != ProjectPtr )
				{
					if ( MoveCameraToProjectFirstPos(ProjectPtr, LaneID, DistrictID, false) == false )
					{	return false; }
				}
				CheckSensor = false;
			}
		}

		Res = ::WaitForSingleObject(SimpleJobThreadEvent, SleepTime);
		if ( Res != WAIT_TIMEOUT ) 
		{
			if ( THREAD_STATE_EXCEPTION == m_SimpleJobThreadState )
			{	return false;	}
			if ( THREAD_STATE_RUNNING == m_SimpleJobThreadState )
			{
				if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
				continue; 
			}
			break; 
		}		
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForSimpleJobFn_MoveToDistrict too long"));
		return false;
	}	
#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CreateSimpleJobThread()//建立維修站結果訊號執行緒
{
#ifndef OFFLINE_VERSION
	if ( DeleteSimpleJobThread() == false ) { return false; }		
	m_SimpleJobThreadCmd = THREAD_COMMAND_TO_IDLE;	
	SetSimpleJob_LaneID(LANE_ID_A);
	SetSimpleJobMode(SIMPLE_JOB_NULL);
	SetSimpleJob_DistrictID(DISTRICT_ID_A);
	SimpleJobThreadHandle = (HANDLE)::_beginthreadex(NULL, NULL, &SimpleJobThreadFn, (void*)0, NULL, &SimpleJobThreadID);	
	if ( NULL == SimpleJobThreadHandle )
	{	
		m_SimpleJobThreadState = THREAD_STATE_NONE;
		m_ErrorString = _T("Eorror, Create Thread Fault (SimpleJobThreadHandle == NULL)");		
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_CREATE);
		return false;
	}		
	SimpleJobThreadEvent = JetAPI::CreateEvent(NULL, TRUE, TRUE, _T("Simple Job Event"));		
	::SetThreadPriority(SimpleJobThreadHandle, THREAD_PRIORITY_BELOW_NORMAL); 
	//::SetThreadAffinityMask(SimpleJobThreadHandle, 0x03);//保留2個		
	//StartSimpleJobThread(false);	
#else
	SimpleJobThreadHandle = NULL;
	SimpleJobThreadEvent = NULL;
#endif//OFFLINE_VERSION
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::DeleteSimpleJobThread()//刪除維修站結果訊號執行緒
{
#ifndef OFFLINE_VERSION
	DWORD WaitTime = 10000;//1 sec	
	if ( NULL == SimpleJobThreadHandle ) { return true; }		
	SetSimpleJobThreadCmd(THREAD_COMMAND_TO_EXIT);	
	DWORD Res = ::WaitForSingleObject(SimpleJobThreadHandle, WaitTime);
	::CloseHandle(SimpleJobThreadHandle); 
	SimpleJobThreadHandle = NULL;
	if ( NULL != SimpleJobThreadEvent )
	{	
		::CloseHandle(SimpleJobThreadEvent); 
		SimpleJobThreadEvent=NULL; 
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StopSimpleJobThread(bool WaitOn)//開始維修站結果訊號執行緒
{
#ifndef OFFLINE_VERSION
	size_t i=0;
	if ( NULL == SimpleJobThreadHandle ) { return true; }
	if ( NULL != SimpleJobThreadEvent )
	{	::ResetEvent(SimpleJobThreadEvent); }
	SetSimpleJobThreadCmd(THREAD_COMMAND_TO_IDLE);	

	if ( WaitOn == true ) 
	{	
		const size_t MaxCount = 100;
		const size_t SleepTime = 10;
		for ( i=0; i<MaxCount; i++ )
		{	
			if ( THREAD_STATE_NONE == m_SimpleJobThreadState )
			{	break; }
			if ( THREAD_STATE_IDLE == m_SimpleJobThreadState )
			{	break; }
			//if ( THREAD_STATE_FINISH == m_SimpleJobThreadState )//有可能後面才完成, 所以要加上完成確認
			//{	break; }
			::Sleep(SleepTime);
		}
		if ( i == MaxCount )
		{
			this->m_ErrorString.Format(_T("Error, wait for StopSimpleJobThread too long"));			
			return false;
		}		
	}
#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::StartSimpleJobThread(bool WaitOn)//開始維修站結果訊號執行緒
{
#ifndef OFFLINE_VERSION
	size_t i=0;
	if ( NULL == SimpleJobThreadHandle ) { return true; }
	if ( NULL != SimpleJobThreadEvent )
	{	::ResetEvent(SimpleJobThreadEvent); }
	if ( THREAD_STATE_FINISH == m_SimpleJobThreadState )
	{  SetSimpleJobThreadState(THREAD_STATE_IDLE); }
	SetSimpleJobThreadCmd(THREAD_COMMAND_TO_RUN);	

	if ( false == WaitOn ) 
	{	return true; }
	if ( WaitForSimpleJobThreadStart() == false )
	{	return false; }	
#endif//OFFLINE_VERSION	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForSimpleJobThreadIdle()//等待簡單工作緒停止
{
#ifndef OFFLINE_VERSION
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;

	if ( NULL == SimpleJobThreadEvent ) { return true; }
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		//if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_NONE == m_SimpleJobThreadState ) { return true; }		
		if ( THREAD_STATE_IDLE == m_SimpleJobThreadState ) { return true; }
		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }		
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForSimpleJobThreadIdle too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_IDLE);
		return false;
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForSimpleJobThreadStop()//等待簡單工作緒停止
{
#ifndef OFFLINE_VERSION
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;

	if ( NULL == SimpleJobThreadEvent ) { return true; }
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		//if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_STATE_RUNNING != m_SimpleJobThreadState ) { return true; }

		if ( SleepTime > 0 )
		{	::Sleep(SleepTime); }		
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForSimpleJobThreadStop too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_STOP);
		return false;
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForSimpleJobThreadStart()//等待簡單工作緒開始
{
#ifndef OFFLINE_VERSION	
	size_t i=0;
	const size_t MaxCount = 100;
	const size_t SleepTime = 10;
	for ( i=0; i<MaxCount; i++ )
	{
		if ( THREAD_COMMAND_TO_RUN != m_SimpleJobThreadCmd )//切入下一個階段
		{	break; }
		if ( THREAD_STATE_IDLE != m_SimpleJobThreadState )
		{	break; }
		::Sleep(SleepTime);
	}
	if ( i == MaxCount )
	{
		m_ErrorString.Format(_T("Error, wait for StartSimpleJobThread too long"));		
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_START);	
		return false;
	}
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::WaitForSimpleJobThreadFinish()//等待維修站結果訊號執行緒結束
{
#ifndef OFFLINE_VERSION
	size_t i=0;	
	DWORD Res = 0;		
	const DWORD SleepTime = 100;	
	const size_t MaxCounts = 200000;

	if ( NULL == SimpleJobThreadEvent ) { return true; }
	for ( i=0; i<MaxCounts; i++ )
	{
		if ( GetIsSystemReleased() == true ) { return true; }				
		if ( GetIsSystemException() == true ) { return true; }
		if ( THREAD_COMMAND_TO_EXIT == m_SimpleJobThreadCmd ) { return true; }		
		if ( THREAD_COMMAND_TO_IDLE == m_SimpleJobThreadCmd )
		{	break; }

		Res = ::WaitForSingleObject(SimpleJobThreadEvent, SleepTime);
		if ( Res != WAIT_TIMEOUT ) 
		{
			if ( THREAD_STATE_RUNNING == m_SimpleJobThreadState )
			{
				if ( SleepTime > 0 ) { ::Sleep(SleepTime); }
				continue; 
			}
			break; 
		}
	}
	if ( i == MaxCounts )
	{
		m_ErrorString.Format(_T("Error, Wait for WaitForSimpleJobThreadFinish too long"));
		SetThreadExceptionCode(AOI_EXCEPTION_THREAD_WAIT_FOR_FINISH);
		return false;
	}	
#endif//OFFLINE_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::CheckSimpleJobThreadState(THREAD_STATE_MODE State)//確認維修站結果訊號執行緒狀態
{
	if ( NULL == SimpleJobThreadHandle ) { return true; }
	if ( m_SimpleJobThreadState != State )
	{
		m_ErrorString.Format(_T("Error, m_SimpleJobThreadState is Exception"));
		return false;
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetSimpleJobThreadState(THREAD_STATE_MODE State)//設定維修站結果訊號執行緒狀態
{
	if ( m_SimpleJobThreadState == State ) { return; }
	LockThreadSimpleJob();
	m_SimpleJobThreadState = State;
	UnlockThreadSimpleJob();
}
//-------------------------------------------------------------------------------------//
THREAD_STATE_MODE CAOIDataCollect::GetSimpleJobThreadState()//取得維修站結果訊號執行緒狀態
{
	return m_SimpleJobThreadState;	
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetSimpleJobThreadCmd(THREAD_COMMAND_MODE Cmd)//設定維修站結果訊號執行緒命令
{
	if ( m_SimpleJobThreadCmd == Cmd ) { return; }
	LockThreadSimpleJob();
	m_SimpleJobThreadCmd = Cmd;
	UnlockThreadSimpleJob();
}
//-------------------------------------------------------------------------------------//
THREAD_COMMAND_MODE CAOIDataCollect::GetSimpleJobThreadCmd()//取得維修站結果訊號執行緒命令
{
	return m_SimpleJobThreadCmd;		
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetSimpleJobMode(SIMPLE_JOB_MODE Mode)//設定簡單工作模式
{
	m_SimpleJobMode = Mode;//簡單工作模式
}
//-------------------------------------------------------------------------------------//
SIMPLE_JOB_MODE CAOIDataCollect::GetSimpleJobMode()//取得簡單工作模式
{
	return m_SimpleJobMode;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetSimpleJob_LaneID(LANE_ID ID)//設定簡單工作模式-軌道編號
{
	m_SimpleJob_LaneID = ID;
}
//-------------------------------------------------------------------------------------//
LANE_ID CAOIDataCollect::GetSimpleJob_LaneID()//取得簡單工作模式-軌道編號
{
	return m_SimpleJob_LaneID;
}
//-------------------------------------------------------------------------------------//
void CAOIDataCollect::SetSimpleJob_DistrictID(DISTRICT_ID ID)//設定簡單工作模式-分段編號
{
	m_SimpleJob_DistrictID = ID;
}
//-------------------------------------------------------------------------------------//
DISTRICT_ID CAOIDataCollect::GetSimpleJob_DistrictID()//取得簡單工作模式-分段編號
{
	return m_SimpleJob_DistrictID;
}
//-------------------------------------------------------------------------------------//	