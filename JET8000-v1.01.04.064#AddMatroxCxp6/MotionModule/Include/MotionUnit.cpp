/*****************************************************************************/
//
//  MotionController API
//	Wrapper for dynamic loadable MotionController DLLs
//
//	Copyright (c) 2018, JET Tech.  All rights reserved.
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

#include "stdafx.h"
#include <tchar.h>
#include <windows.h>
#include "MotionUnit.h"


#define MOU_DYN_DLL_PROC(proc_name, proto_args, args) \
	typedef MOU_ERROR (*T_##proc_name) proto_args; \
	T_##proc_name p_##proc_name = NULL; \
	MOU_ERROR proc_name proto_args \
	{ if(p_##proc_name) return (*p_##proc_name) args; else return MOU_ERR_NOT_IMPLEMENTED;}

#define MOU_DYN_DLL_GET_PROC_ADDR(hinst, proc_name) \
	p_##proc_name = (T_##proc_name) GetProcAddress((hinst), #proc_name ); \
	if(p_##proc_name==NULL) MessageBox(NULL, _T( #proc_name ), _T("DLL procedure not found"), MB_OK)

#define MOU_DYN_DLL_INIT_PROC_ADDR(proc_name) p_##proc_name = NULL

MOU_DYN_DLL_PROC(mouGetDllVersion, (unsigned long* major, unsigned long* minjor, unsigned long* build), (major, minjor, build))
MOU_DYN_DLL_PROC(mouOpen, (const wchar_t * InistString,MOU_HANDLE *handle), (InistString,handle))

MOU_DYN_DLL_PROC(mouInitialize, (MOU_HANDLE handle), (handle))
MOU_DYN_DLL_PROC(mouUninitialize, (MOU_HANDLE handle), (handle));
MOU_DYN_DLL_PROC(mouGetModuleName, (MOU_HANDLE Handle, char pStr[g_knMouModuleNameLen]), (Handle, pStr) );

MOU_DYN_DLL_PROC(mouGetStatusIO, (MOU_HANDLE Handle,MOU_AXIS Axis,MOU_AXIS_STATUS_IO &Status),(Handle,Axis,Status))
MOU_DYN_DLL_PROC(mouGetStatusMT, (MOU_HANDLE Handle,MOU_AXIS Axis,MOU_AXIS_STATUS_MT &Status),(Handle,Axis,Status))

MOU_DYN_DLL_PROC(mouIsBusy, (MOU_HANDLE Handle,MOU_AXIS Axis,bool &Result),(Handle,Axis,Result))

MOU_DYN_DLL_PROC(mouExecHome, (MOU_HANDLE Handle,MOU_AXIS Axis),(Handle,Axis))
MOU_DYN_DLL_PROC(mouExecHomeEx, (MOU_HANDLE Handle,MOU_AXIS Axis, MOU_AXIS_DIRECTION SearchDir), (Handle,Axis, SearchDir) )
MOU_DYN_DLL_PROC(mouEnable, (MOU_HANDLE Handle, MOU_AXIS Axis),(Handle, Axis));
MOU_DYN_DLL_PROC(mouDisable, (MOU_HANDLE Handle, MOU_AXIS Axis), (Handle, Axis));

MOU_DYN_DLL_PROC(mouSetCurveMode, (MOU_HANDLE Handle, MOU_AXIS Axis, MOU_MOVING_CURVE Mode), (Handle, Axis, Mode) );
MOU_DYN_DLL_PROC(mouGetCurveMode, (MOU_HANDLE Handle, MOU_AXIS Axis, MOU_MOVING_CURVE &Mode), (Handle, Axis, Mode) );

MOU_DYN_DLL_PROC(mouResetCommandPosition, (MOU_HANDLE Handle, MOU_AXIS Axis), (Handle, Axis) );
MOU_DYN_DLL_PROC(mouSetORG, (MOU_HANDLE Handle,MOU_AXIS Axis), (Handle, Axis));
MOU_DYN_DLL_PROC(mouResetAlarm, (MOU_HANDLE Handle,MOU_AXIS Axis), (Handle, Axis));

MOU_DYN_DLL_PROC(mouSetSWLimit, (MOU_HANDLE Handle,MOU_AXIS Axis, double MaxPos, double MinPos), (Handle,Axis,MaxPos,MinPos) );
MOU_DYN_DLL_PROC(mouGetSWLimit, (MOU_HANDLE Handle,MOU_AXIS Axis, double &MaxPos, double &MinPos), (Handle,Axis,MaxPos,MinPos) );

MOU_DYN_DLL_PROC(mouEnableSWLimit, (MOU_HANDLE Handle,MOU_AXIS Axis), (Handle,Axis) );
MOU_DYN_DLL_PROC(mouDisableSWLimit, (MOU_HANDLE Handle,MOU_AXIS Axis), (Handle,Axis) );

MOU_DYN_DLL_PROC(mouEnableINP, (MOU_HANDLE Handle,MOU_AXIS Axis), (Handle,Axis) );
MOU_DYN_DLL_PROC(mouDisableINP, (MOU_HANDLE Handle,MOU_AXIS Axis), (Handle,Axis) );

MOU_DYN_DLL_PROC(mouWaitForEndOfMove, (MOU_HANDLE Handle,MOU_AXIS Axis),(Handle,Axis))
MOU_DYN_DLL_PROC(mouStartJog, (MOU_HANDLE Handle,MOU_AXIS Axis,MOU_AXIS_DIRECTION Direction, double Speed),(Handle,Axis,Direction, Speed))
MOU_DYN_DLL_PROC(mouStopJog, (MOU_HANDLE Handle,MOU_AXIS Axis),(Handle,Axis))

MOU_DYN_DLL_PROC(mouMoveRelative, (MOU_HANDLE Handle,MOU_AXIS Axis,double Speed, double Distance),(Handle,Axis,Speed,Distance))
MOU_DYN_DLL_PROC(mouMoveAbsolute, (MOU_HANDLE Handle,MOU_AXIS Axis,double Speed, double Position),(Handle,Axis,Speed,Position))
MOU_DYN_DLL_PROC(mouStopMotion, (MOU_HANDLE Handle,MOU_AXIS Axis), (Handle,Axis) );

MOU_DYN_DLL_PROC(mouSetAccDecTime, (MOU_HANDLE Handle,MOU_AXIS Axis,double Acceleration, double Deceleration),(Handle,Axis,Acceleration,Deceleration))
MOU_DYN_DLL_PROC(mouGetAccDecTime, (MOU_HANDLE Handle,MOU_AXIS Axis,double &Acceleration, double &Deceleration),(Handle,Axis,Acceleration,Deceleration))

//for S-Curve parameters
MOU_DYN_DLL_PROC(mouSetSVaccVdec,(MOU_HANDLE Handle,MOU_AXIS Axis, double SVacc, double SVdec), (Handle, Axis, SVacc, SVdec) );
MOU_DYN_DLL_PROC(mouGetSVaccVdec,(MOU_HANDLE Handle,MOU_AXIS Axis, double &SVacc, double &SVdec), (Handle, Axis, SVacc, SVdec) );

MOU_DYN_DLL_PROC(mouGetPosition, (MOU_HANDLE Handle,MOU_AXIS Axis,double &Position),(Handle,Axis,Position))

MOU_DYN_DLL_PROC(mouGetPEG, (MOU_HANDLE Handle,MOU_AXIS Axis,PEG_Data &Data),(Handle,Axis,Data))
MOU_DYN_DLL_PROC(mouSetPEG, (MOU_HANDLE Handle,MOU_AXIS Axis,const PEG_Data &Data),(Handle,Axis,Data))
MOU_DYN_DLL_PROC(mouEnablePEG, (MOU_HANDLE Handle,MOU_AXIS Axis),(Handle,Axis))
MOU_DYN_DLL_PROC(mouDisablePEG, (MOU_HANDLE Handle,MOU_AXIS Axis),(Handle,Axis))

MOU_DYN_DLL_PROC(mouSetHomeData, (MOU_HANDLE Handle,MOU_AXIS Axis, const HOME_Data &Data), (Handle,Axis, Data) )
MOU_DYN_DLL_PROC(mouGetHomeData, (MOU_HANDLE Handle,MOU_AXIS Axis,HOME_Data &Data), (Handle, Axis, Data) )

MOU_DYN_DLL_PROC(mouSetJogData, (MOU_HANDLE Handle,MOU_AXIS Axis, const JOG_Data &Data), (Handle,Axis, Data) )
MOU_DYN_DLL_PROC(mouGetJogData, (MOU_HANDLE Handle,MOU_AXIS Axis,JOG_Data &Data), (Handle,Axis, Data) )

MOU_DYN_DLL_PROC(mouSendCommand, (MOU_HANDLE Handle, char *pCmd, int CmdLen, char *pResponse, int *ResLen), (Handle, pCmd, CmdLen, pResponse, ResLen))


static HINSTANCE mou_hInstLib = NULL;

static std::string GetLastErrorStdStr()
{
  DWORD error = GetLastError();
  if (error)
  {
    LPVOID lpMsgBuf;
    DWORD bufLen = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR) &lpMsgBuf,
        0, NULL );
    if (bufLen)
    {
      LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
      std::string result(lpMsgStr, lpMsgStr+bufLen);
      
      LocalFree(lpMsgBuf);

      return result;
    }
  }
  return std::string();
}

//boolmouLoadDLL(std::wstring libname)
bool mouLoadDLL(const wchar_t *libname)
{
	if (mou_hInstLib)
		mouFreeDLL();

	 // Get a handle to the DLL module.
    mou_hInstLib = LoadLibraryW(libname); 
 
    // If the handle is valid, try to get the function address.
    if (mou_hInstLib == NULL){
		//UINT err = GetLastError();
		std::string strErr(GetLastErrorStdStr() );
		return false;
	}
    
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouGetDllVersion);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouGetModuleName);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouOpen);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouInitialize);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouUninitialize);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouGetStatusIO);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouGetStatusMT);	
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouIsBusy);

	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouExecHome);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouExecHomeEx);

	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouEnable);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouDisable);

	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouSetCurveMode);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouGetCurveMode);

	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouResetCommandPosition);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouSetORG);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouResetAlarm);
	//
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouSetSWLimit);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouGetSWLimit);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouEnableSWLimit);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouDisableSWLimit);

	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouEnableINP);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouDisableINP);

	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouWaitForEndOfMove);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouStartJog);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouStopJog);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouMoveRelative);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouMoveAbsolute);

	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouStopMotion);
	//
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouSetAccDecTime);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouGetAccDecTime);

	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouSetSVaccVdec);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouGetSVaccVdec);

	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouGetPosition);

	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouGetPEG);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouSetPEG);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouEnablePEG);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouDisablePEG);	

	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouSetHomeData);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouGetHomeData);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouSetJogData);
	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouGetJogData);

	MOU_DYN_DLL_GET_PROC_ADDR (mou_hInstLib,mouSendCommand);
	  
	return true;
}

void mouFreeDLL()
{
	if(mou_hInstLib==NULL)
		return;	

	MOU_DYN_DLL_INIT_PROC_ADDR(mouGetDllVersion);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouGetModuleName);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouOpen);

	MOU_DYN_DLL_INIT_PROC_ADDR(mouInitialize);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouUninitialize);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouGetStatusIO);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouGetStatusMT);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouIsBusy);

	MOU_DYN_DLL_INIT_PROC_ADDR(mouExecHome);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouExecHomeEx);

	MOU_DYN_DLL_INIT_PROC_ADDR(mouEnable);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouDisable);

	MOU_DYN_DLL_INIT_PROC_ADDR(mouSetCurveMode);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouGetCurveMode);

	MOU_DYN_DLL_INIT_PROC_ADDR(mouResetCommandPosition);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouSetORG);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouResetAlarm);

	MOU_DYN_DLL_INIT_PROC_ADDR(mouSetSWLimit);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouGetSWLimit);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouEnableSWLimit);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouDisableSWLimit);
	
	MOU_DYN_DLL_INIT_PROC_ADDR(mouEnableINP);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouDisableINP);
	
	MOU_DYN_DLL_INIT_PROC_ADDR(mouWaitForEndOfMove);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouStartJog);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouStopJog);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouMoveRelative);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouMoveAbsolute);
	
	MOU_DYN_DLL_INIT_PROC_ADDR(mouStopMotion);
	
	MOU_DYN_DLL_INIT_PROC_ADDR(mouSetAccDecTime);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouGetAccDecTime);

	MOU_DYN_DLL_INIT_PROC_ADDR(mouSetSVaccVdec);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouGetSVaccVdec);

	MOU_DYN_DLL_INIT_PROC_ADDR(mouGetPosition);
	
	MOU_DYN_DLL_INIT_PROC_ADDR(mouGetPEG);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouSetPEG);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouEnablePEG);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouDisablePEG);

	MOU_DYN_DLL_INIT_PROC_ADDR(mouSetHomeData);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouGetHomeData);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouSetJogData);
	MOU_DYN_DLL_INIT_PROC_ADDR(mouGetJogData);
	
	MOU_DYN_DLL_INIT_PROC_ADDR(mouSendCommand);	

	FreeLibrary(mou_hInstLib);
	mou_hInstLib = NULL;
}

