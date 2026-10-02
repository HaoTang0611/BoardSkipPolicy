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
#include "Camera2DUnit.h"


#define C2D_DYN_DLL_PROC(proc_name, proto_args, args) \
	typedef C2D_ERROR (*T_##proc_name) proto_args; \
	T_##proc_name p_##proc_name = NULL; \
	C2D_ERROR proc_name proto_args \
	{ if(p_##proc_name) return (*p_##proc_name) args; else return C2D_ERR_NOT_IMPLEMENTED;}

#define C2D_DYN_DLL_GET_PROC_ADDR(hinst, proc_name) \
	p_##proc_name = (T_##proc_name) GetProcAddress((hinst), #proc_name ); \
	if(p_##proc_name==NULL) MessageBox(NULL, _T( #proc_name ), _T("DLL procedure not found"), MB_OK)

#define C2D_DYN_DLL_INIT_PROC_ADDR(proc_name) p_##proc_name = NULL

C2D_DYN_DLL_PROC(c2dGetDllVersion, (unsigned long* major, unsigned long* minjor, unsigned long* build), (major, minjor, build))
C2D_DYN_DLL_PROC(c2dOpen, (const wchar_t * InistString, Camera2DType Type, C2D_HANDLE *handle), (InistString, Type, handle))
C2D_DYN_DLL_PROC(c2dClose, (C2D_HANDLE handle), (handle) );

C2D_DYN_DLL_PROC(c2dGetModuleName, (C2D_HANDLE handle, char pStr[g_knModuleNameLen]), (handle, pStr) );

C2D_DYN_DLL_PROC(c2dInitialize, (C2D_HANDLE handle), (handle))
C2D_DYN_DLL_PROC(c2dUninitialize, (C2D_HANDLE handle), (handle));


C2D_DYN_DLL_PROC(c2dRegisterFunction, (C2D_HANDLE Handle, FrameCallback CallBack), (Handle, CallBack) );

C2D_DYN_DLL_PROC(c2dSetExposureTime, (C2D_HANDLE Handle, double ETime), (Handle, ETime) );
C2D_DYN_DLL_PROC(c2dGetExposureTime, (C2D_HANDLE Handle, double &ETime), (Handle, ETime) );

C2D_DYN_DLL_PROC(c2dSnapshot, (C2D_HANDLE Handle), (Handle));
C2D_DYN_DLL_PROC(c2dLive, (C2D_HANDLE Handle), (Handle) );
C2D_DYN_DLL_PROC(c2dFreeze, (C2D_HANDLE Handle), (Handle) );
//
C2D_DYN_DLL_PROC(c2dSetGrabMode, (C2D_HANDLE Handle, CameraGrabMode Mode), (Handle, Mode) );
C2D_DYN_DLL_PROC(c2dGetGrabMode, (C2D_HANDLE Handle, CameraGrabMode &Mode), (Handle, Mode) );

C2D_DYN_DLL_PROC(c2dSetTriggerMode, (C2D_HANDLE Handle, CameraTriggerMode Mode), (Handle, Mode) );
C2D_DYN_DLL_PROC(c2dGetTriggerMode, (C2D_HANDLE Handle, CameraTriggerMode &Mode), (Handle, Mode) );

C2D_DYN_DLL_PROC(c2dSetBayPattern, (C2D_HANDLE Handle, CameraBayerPattern BP), (Handle, BP) );
C2D_DYN_DLL_PROC(c2dGetBayPattern, (C2D_HANDLE Handle, CameraBayerPattern &rBP), (Handle, rBP) );

C2D_DYN_DLL_PROC(c2dGetImageWidth, (C2D_HANDLE Handle, unsigned int &rW), (Handle, rW) );
C2D_DYN_DLL_PROC(c2dGetImageHeight, (C2D_HANDLE Handle, unsigned int &rH), (Handle, rH) );

C2D_DYN_DLL_PROC(c2dGetImagePayLoadSize, (C2D_HANDLE Handle, unsigned int &rPayLoadSize), (Handle, rPayLoadSize) );

C2D_DYN_DLL_PROC(c2dGetCameraInfo, (C2D_HANDLE Handle, CCameraInfo &rInfo), (Handle, rInfo) );

C2D_DYN_DLL_PROC(c2dSendCommand, (C2D_HANDLE handle, char *pCmd, int CmdLen, char *pResponse, int *ResLen), (handle, pCmd, CmdLen, pResponse, ResLen))

HINSTANCE c2d_hInstLib = NULL;

std::string GetLastErrorStdStr()
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

bool c2dLoadDLL(const wchar_t *libname)
{
	if (c2d_hInstLib)
		c2dFreeDLL();

	 // Get a handle to the DLL module.
    c2d_hInstLib = LoadLibraryW(libname);
 
    // If the handle is valid, try to get the function address.
    if (c2d_hInstLib == NULL){
		//UINT err = GetLastError();
		std::string strErr(GetLastErrorStdStr() );
		return false;
	}
    
	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dGetDllVersion);	
	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dOpen);
	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dClose);	

	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dInitialize);
	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dUninitialize);
	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dGetModuleName);

	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dRegisterFunction);
	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dSetExposureTime);
	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dGetExposureTime);
	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dSnapshot);
	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dLive);
	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dFreeze);

	//
	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dSetGrabMode);
	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dGetGrabMode);

	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dSetTriggerMode);
	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dGetTriggerMode);

	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dSetBayPattern);
	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dGetBayPattern);

	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dGetImageWidth);
	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dGetImageHeight);

	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dGetImagePayLoadSize);

	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dGetCameraInfo);

	C2D_DYN_DLL_GET_PROC_ADDR (c2d_hInstLib,c2dSendCommand);

	return true;
}

void c2dFreeDLL()
{
	if(c2d_hInstLib==NULL)
		return;	

	C2D_DYN_DLL_INIT_PROC_ADDR (c2dGetDllVersion);
	C2D_DYN_DLL_INIT_PROC_ADDR (c2dOpen);
	C2D_DYN_DLL_INIT_PROC_ADDR (c2dClose);

	C2D_DYN_DLL_INIT_PROC_ADDR (c2dInitialize);
	C2D_DYN_DLL_INIT_PROC_ADDR (c2dUninitialize);
	C2D_DYN_DLL_INIT_PROC_ADDR (c2dGetModuleName);

	C2D_DYN_DLL_INIT_PROC_ADDR (c2dRegisterFunction);
	C2D_DYN_DLL_INIT_PROC_ADDR (c2dSetExposureTime);
	C2D_DYN_DLL_INIT_PROC_ADDR (c2dGetExposureTime);
	C2D_DYN_DLL_INIT_PROC_ADDR (c2dSnapshot);
	C2D_DYN_DLL_INIT_PROC_ADDR (c2dLive);	 
	C2D_DYN_DLL_INIT_PROC_ADDR (c2dFreeze);

	C2D_DYN_DLL_INIT_PROC_ADDR (c2dSetGrabMode);
	C2D_DYN_DLL_INIT_PROC_ADDR (c2dGetGrabMode);

	C2D_DYN_DLL_INIT_PROC_ADDR (c2dSetTriggerMode);
	C2D_DYN_DLL_INIT_PROC_ADDR (c2dGetTriggerMode);

	C2D_DYN_DLL_INIT_PROC_ADDR (c2dSetBayPattern);
	C2D_DYN_DLL_INIT_PROC_ADDR (c2dGetBayPattern);

	C2D_DYN_DLL_INIT_PROC_ADDR (c2dGetImageWidth);
	C2D_DYN_DLL_INIT_PROC_ADDR (c2dGetImageHeight);
	C2D_DYN_DLL_INIT_PROC_ADDR (c2dGetImagePayLoadSize);

	C2D_DYN_DLL_INIT_PROC_ADDR (c2dGetCameraInfo);
	
	C2D_DYN_DLL_INIT_PROC_ADDR (c2dSendCommand);	

	FreeLibrary(c2d_hInstLib);
	c2d_hInstLib = NULL;
}

