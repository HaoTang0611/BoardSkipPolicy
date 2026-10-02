
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LOCKSCREEN_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LOCKSCREEN_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef LOCKSCREEN_EXPORTS
#define LOCKSCREEN_API __declspec(dllexport)
#else
#define LOCKSCREEN_API __declspec(dllimport)
#endif

LOCKSCREEN_API void SetLockScreenHook( HWND hReportWnd, HWND hLimitWnd, int HOOKMsgID, int UnLockKey, int SysUnLockKey);	// hReportWnd:Message回傳目標	hLimitWnd:主要Handle
LOCKSCREEN_API void UnLockScreenHook();
