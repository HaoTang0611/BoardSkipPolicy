/*****************************************************************************/
//
// 	Camera 2D Controller API
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

#ifndef _C2D_FUNCTIONS_FILE_H_
#define _C2D_FUNCTIONS_FILE_H_

#include <string>
#include "Camera2DUnitTypeDef.h"

/*****************************************************************************
Exports
*****************************************************************************/

#ifdef C2D_EXPORTS
    #define C2D_API __declspec(dllexport)
#else
	#ifdef C2D_EXTERN
	   #define C2D_API extern
	#else
	   #define C2D_API __declspec(dllimport)
	#endif
#endif

/*****************************************************************************
API
*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*********** General API  Functions  ************/
C2D_API C2D_ERROR c2dGetDllVersion(unsigned long* major, unsigned long* minjor, unsigned long* build);
C2D_API C2D_ERROR c2dOpen(const wchar_t *InitString, Camera2DType Type, C2D_HANDLE* Handle);
C2D_API C2D_ERROR c2dClose(C2D_HANDLE Handle);
C2D_API C2D_ERROR c2dGetModuleName(C2D_HANDLE Handle, char pStr[g_knModuleNameLen]);

/*********** Basic  Functions ************/
C2D_API C2D_ERROR c2dInitialize(C2D_HANDLE Handle);
C2D_API C2D_ERROR c2dUninitialize(C2D_HANDLE Handle);

C2D_API C2D_ERROR c2dRegisterFunction(C2D_HANDLE Handle, FrameCallback CallBack);

//
C2D_API C2D_ERROR c2dSetExposureTime(C2D_HANDLE Handle, double ETime);
C2D_API C2D_ERROR c2dGetExposureTime(C2D_HANDLE Handle, double &ETime);

//
C2D_API C2D_ERROR c2dSnapshot(C2D_HANDLE Handle);
C2D_API C2D_ERROR c2dLive(C2D_HANDLE Handle);
C2D_API C2D_ERROR c2dFreeze(C2D_HANDLE Handle);
//
C2D_API C2D_ERROR c2dSetGrabMode(C2D_HANDLE Handle, CameraGrabMode Mode);
C2D_API C2D_ERROR c2dGetGrabMode(C2D_HANDLE Handle, CameraGrabMode &Mode);

C2D_API C2D_ERROR c2dSetTriggerMode(C2D_HANDLE Handle, CameraTriggerMode Mode);
C2D_API C2D_ERROR c2dGetTriggerMode(C2D_HANDLE Handle, CameraTriggerMode &Mode);

C2D_API C2D_ERROR c2dSetBayPattern(C2D_HANDLE Handle, CameraBayerPattern BP);
C2D_API C2D_ERROR c2dGetBayPattern(C2D_HANDLE Handle, CameraBayerPattern &rBP);

C2D_API C2D_ERROR c2dGetImageWidth(C2D_HANDLE Handle, unsigned int &rW);
C2D_API C2D_ERROR c2dGetImageHeight(C2D_HANDLE Handle, unsigned int &rH);

C2D_API C2D_ERROR c2dGetImagePayLoadSize(C2D_HANDLE Handle, unsigned int &rPayLoadSize);

C2D_API C2D_ERROR c2dGetCameraInfo(C2D_HANDLE Handle, CCameraInfo &rInfo);

//For log command (ex. LOG_ON/LOG_OFF) or other command
C2D_API C2D_ERROR c2dSendCommand(C2D_HANDLE Handle, char *pCmd, int CmdLen, char *pResponse, int *ResLen);
//Log On, Log Off, LogDir C:\ABC, SaveINI

#ifdef __cplusplus
}
#endif

#endif