// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__3985CB25_62CD_4AD9_BF49_875ED5C19147__INCLUDED_)
#define AFX_STDAFX_H__3985CB25_62CD_4AD9_BF49_875ED5C19147__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//----------------------------------------------------------------------------//
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif//!VC_EXTRALEAN

#include "targetver.h"

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif//!_AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif //!_AFX_NO_AFXCMN_SUPPORT

#include <process.h>
//How to define unicode at VC6.0
//1. [C/C++] -> [Preprocessor definitions] -> add(_UNICODE), remove(_MBCS)
//2. [Link] -> [Entri-point symbol] -> add(wWinMainCRTStartup)
//----------------------------------------------------------------------------//
#ifdef _WIN64
	#define _X64           1
#endif//_WIN64

#if _MSC_VER >= VS_2010_NET
	//verrsrc.h	
	//afxribbon.rc	
	//#define FRAME_STYLE_TYPE FRAME_STYLE_MFC
	//#define FRAME_STYLE_TYPE FRAME_STYLE_STUDIO
	#define FRAME_STYLE_TYPE FRAME_STYLE_OFFICE
	
	#include <afxcontrolbars.h>     // 功能區和控制列的 MFC 支援
	#include <afxsock.h>            // MFC 通訊端擴充功能	
	#ifdef _UNICODE
		#if defined _M_IX86
			#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
		#elif defined _M_X64
			#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
		#else
			#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
		#endif
	#endif//_UNICODE
	const UINT AFX_WM_PROPERTY_LCLICKED =::RegisterWindowMessage(_T("AFX_WM_PROPERTY_LCLICKED"));
	const UINT AFX_WM_PROPERTY_RCLICKED =::RegisterWindowMessage(_T("AFX_WM_PROPERTY_RCLICKED"));
	const UINT AFX_WM_PROPERTY_LDBCLICK =::RegisterWindowMessage(_T("AFX_WM_PROPERTY_LDBCLICK"));
	const UINT AFX_WM_PROPERTY_RDBCLICK =::RegisterWindowMessage(_T("AFX_WM_PROPERTY_RDBCLICK"));	
	const UINT AFX_WM_PROPERTY_SEL_CHANGED =::RegisterWindowMessage(_T("AFX_WM_PROPERTY_SEL_CHANGED"));
#else
	#define FRAME_STYLE_TYPE FRAME_STYLE_MFC

	#ifdef _X64
		//typedef __int64 INT_PTR, *PINT_PTR;
		//typedef unsigned __int64 UINT_PTR, *PUINT_PTR;
		typedef __int64 LONG_PTR, *PLONG_PTR;
		typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;
		typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;
	#else//_X64
		//typedef int INT_PTR, *PINT_PTR;
		//typedef unsigned int UINT_PTR, *PUINT_PTR;
		typedef long LONG_PTR, *PLONG_PTR;
		typedef unsigned long ULONG_PTR, *PULONG_PTR;
		typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;
	#endif//_X64
		
	//const UINT AFX_WM_ON_CHANGE_RIBBON_CATEGORY =::RegisterWindowMessage(_T("AFX_WM_ON_CHANGE_RIBBON_CATEGORY"));// = ::RegisterWindowMessage(_T("AFX_WM_PROPERTY_LCLICKED"));	
	const UINT AFX_WM_PROPERTY_CHANGED =::RegisterWindowMessage(_T("AFX_WM_PROPERTY_CHANGED"));
	const UINT AFX_WM_PROPERTY_LCLICKED =::RegisterWindowMessage(_T("AFX_WM_PROPERTY_LCLICKED"));
	const UINT AFX_WM_PROPERTY_RCLICKED =::RegisterWindowMessage(_T("AFX_WM_PROPERTY_RCLICKED"));
	const UINT AFX_WM_PROPERTY_LDBCLICK =::RegisterWindowMessage(_T("AFX_WM_PROPERTY_LDBCLICK"));
	const UINT AFX_WM_PROPERTY_RDBCLICK =::RegisterWindowMessage(_T("AFX_WM_PROPERTY_RDBCLICK"));	
	const UINT AFX_WM_PROPERTY_SEL_CHANGED =::RegisterWindowMessage(_T("AFX_WM_PROPERTY_SEL_CHANGED"));
#endif

#ifndef FRAME_STYLE_TYPE
	#error No Define Frame Style Type
#endif//FRAME_STYLE_TYPE

#if FRAME_STYLE_TYPE == FRAME_STYLE_MFC
	#define CBasicFrame        CFrameWnd	
	#define CBasicSplitterWnd  CSplitterWnd
	#define CBasicDialog       CDialog
	#define CBasicListCtrl     CListCtrl//基礎的列表控制類別
	#define CBasicTreeCtrl     CTreeCtrl//基礎的樹狀圖控制類別
	#define CBasicHeaderCtrl   CHeaderCtrl//基礎的列表標頭控制類別
#else
	#define CBasicFrame        CFrameWndEx	
	#define CBasicSplitterWnd  CSplitterWndEx
	#define CBasicDialog       CDialogEx
	#define CBasicListCtrl     CMFCListCtrl//基礎的列表控制類別	
	#define CBasicTreeCtrl     CTreeCtrl//基礎的樹狀圖控制類別
	#define CBasicHeaderCtrl   CMFCHeaderCtrl//基礎的列表標頭控制類別
#endif//FRAME_STYLE_TYPE == FRAME_STYLE_MFC
//----------------------------------------------------------------------------//
#pragma warning (disable:4786)
#pragma warning (disable:4800)//bool/BOOL
#pragma warning (disable:4996)// _CRT_SECURE_NO_WARNINGS
#pragma component(browser, off, references)//手動關閉瀏覽資訊//BK4504
//----------------------------------------------------------------------------//
#include "VersionDefine.h"
//----------------------------------------------------------------------------//
//CThisListCtrl_69

//增加自己所要新增的定義
//----------------------------------------------------------------------------//
#define _SEND_DEBUG_STRING   //送出偵錯文字
//----------------------------------------------------------------------------//
#define ZLIB_USE
//----------------------------------------------------------------------------//
#define DTK_BARCODE_USE
#define HON_BARCODE_USE//Honeywell SwiftDecoder
//----------------------------------------------------------------------------//
#define ONLINE_OPEN_PROJECT_USE//尚未除錯, 切勿啟用
#define MULTI_CLASS_USE//尚未除錯, 切勿啟用
#define PART_GROUP_USE//尚未除錯, 切勿啟用
//#define PATTERN_BINARY_USE//尚未除錯, 切勿啟用
#define ONLINE_AUTO_STOP_USE//尚未除錯, 切勿啟用
#define ONLINE_AUTO_CALIBRATION_USE//尚未除錯, 切勿啟用
#define AI_MODEL_USE//尚未除錯, 切勿啟用
//#define TEST_FOV_SIZE_USE//尚未除錯, 切勿啟用
#define DIALOG_BASE_USE//尚未除錯, 切勿啟用	
#define FIELD_CALC_THREAD_USE//尚未除錯, 切勿啟用
//#define ALG_MEASURE_BLACK_GLUE_USE//尚未除錯, 切勿啟用
//#define ALG_MEASURE_FLUX_AREA_USE//尚未除錯, 切勿啟用	
//#define ALG_MEASURE_CPU_PIN_USE//尚未除錯, 切勿啟用	
//#define MODEL_WND_SKEW_SIZE_USE_ALAN//20250715-Alan
#define AOI_EXCEPTION_CODE_USE//尚未除錯, 切勿啟用
//#define BURNING_TEST_FD_USE//尚未除錯, 切勿啟用
//#define SAVE_LOG_MSG_SYNC_USE//尚未除錯, 切勿啟用
#define MUST_BACKUP_SYSTEM_FILE_USE//尚未除錯, 切勿啟用	
//----------------------------------------------------------------------------//
//#define ROCKEY4ND_USE//硬體鎖
//----------------------------------------------------------------------------//
#define  CAMERA_LIST_USE//使用相機列表
//----------------------------------------------------------------------------//
#ifdef _OPENMP
	#define OPEN_MP_USE	
#endif//_OPENMP
//----------------------------------------------------------------------------//
#if _MSC_VER >= VS_2010_NET	
	#ifndef HON_BARCODE_USE//與Honeywell SwiftDecoder相衝突
		#define MIM_LIB_USE       //啟用Mim Library-不支援VC6.0
	#endif//HON_BARCODE_USE
#endif//_MSC_VER
//----------------------------------------------------------------------------//
//定義OpenCV的版本別
#define OPEN_CV_0_9_07_01_V14                    09//v0.9.7.7-VC6使用
#define OPEN_CV_2_4_11_00_V10                    10//v2.4.11-VS2010
#define OPEN_CV_2_4_13_06_V14                    14//v2.4.13.6-VS2015
#define OPEN_CV_3_4_16_00_V14                   314//v3.4.16  -VS2015

#if _MSC_VER >= VS_2015_NET
	#define OPEN_CV_VERSION  OPEN_CV_3_4_16_00_V14
	//#define OPEN_CV_VERSION  OPEN_CV_2_4_13_06_V14
	//#define OPEN_CV_VERSION  OPEN_CV_2_4_11_00_V10
#elif _MSC_VER >= VS_2008_NET
	#define OPEN_CV_VERSION  OPEN_CV_2_4_11_00_V10
#else
	#define OPEN_CV_VERSION  OPEN_CV_0_9_07_01_V14
#endif
#ifndef OPEN_CV_VERSION
	#error OPEN_CV_VERSION is not defeined.
#endif//OPEN_CV_VERSION
//----------------------------------------------------------------------------//
//定義Open-eVision的版本別
#define EVISION_USE           //啟用Open eVision

#define EVISION_MODE_EVISION				  1
#define EVISION_MODE_OPEN_EVISION             2

#define EVISION_MODE_OPEN_EVISION_NULL        0
#define EVISION_MODE_OPEN_EVISION_1_2_0_7496  1
#define EVISION_MODE_OPEN_EVISION_1_2_0_7524  2
#define EVISION_MODE_OPEN_EVISION_1_2_7_8735  3	 
#define EVISION_MODE_OPEN_EVISION_1_3_2_0000  4
#define EVISION_MODE_OPEN_EVISION_2_5_0_1106  5
#define EVISION_MODE_OPEN_EVISION_22_12_2_15123  6
#define EVISION_MODE_OPEN_EVISION_23_12_0_18439  7

#if _MSC_VER >= VS_2008_NET	
	#define EVISION_MODE	EVISION_MODE_OPEN_EVISION		
	//#define EVISION_MODE_OPEN_EVISION_VERSION  EVISION_MODE_OPEN_EVISION_1_2_0_7496//JET6500
	//#define EVISION_MODE_OPEN_EVISION_VERSION  EVISION_MODE_OPEN_EVISION_1_2_0_7524//JET7300
	//#define EVISION_MODE_OPEN_EVISION_VERSION  EVISION_MODE_OPEN_EVISION_1_2_7_8735
	//#define EVISION_MODE_OPEN_EVISION_VERSION  EVISION_MODE_OPEN_EVISION_1_3_2_0000//JET8000 
	//#define EVISION_MODE_OPEN_EVISION_VERSION  EVISION_MODE_OPEN_EVISION_2_5_0_1106//JET8000	 
	#define EVISION_MODE_OPEN_EVISION_VERSION  EVISION_MODE_OPEN_EVISION_22_12_2_15123//JET8000
	//#define EVISION_MODE_OPEN_EVISION_VERSION  EVISION_MODE_OPEN_EVISION_23_12_0_18439//JET8000
#else
	#define EVISION_MODE	EVISION_MODE_EVISION		
	#define EVISION_MODE_OPEN_EVISION_VERSION  EVISION_MODE_OPEN_EVISION_NULL
	//#define EVISION_MODE	EVISION_MODE_OPEN_EVISION	
#endif//_MSC_VER >= VS_2008_NET
#ifndef EVISION_MODE
	#error EVISION_MODE is not defeined.
#endif//EVISION_MODE
//----------------------------------------------------------------------------//
#ifdef EVISION_USE
	#define EVISION_MATCH_USE       //啟用EMatch
	#define EVISION_1D_BARCODE_USE  //啟用EBarcode
	#define EVISION_DATA_MATRIX_USE //啟用EDataMatrix
	#define EVISION_QRCODE_USE      //啟用EQRCodeReader
#endif //EVISION_USE
//----------------------------------------------------------------------------//
#ifdef MIM_LIB_USE
	#define MIM_MATCH_USE    //啟用iMatch
#endif//MIM_LIB_USE
//----------------------------------------------------------------------------//
#ifdef NO_3D_VERSION
	#define DISABLE_3D
#endif //NO_3D_VERSION
//----------------------------------------------------------------------------//
//#define BYPASS_DLP1_USE
//#define BYPASS_DLP2_USE
//#define BYPASS_DLP3_USE
//#define BYPASS_DLP4_USE
//----------------------------------------------------------------------------//
//定義MES類別模式
//定義MES對接軟體
#define MES_CONTACT_IBS              1
#define MES_CONTACT_IPS              2
#define MES_CONTACT_MTS              3//除錯用, 不開放
#define MES_CONTACT_TYPE            MES_CONTACT_IBS
//#define MES_CONTACT_TYPE            MES_CONTACT_IPS
//#define MES_CONTACT_TYPE            MES_CONTACT_MTS
//#define ITS_DISABLE	
//#define IPS_DISABLE	
#define MTS_DISABLE//除錯用, 不開放
//----------------------------------------------------------------------------//
#ifdef OFFLINE_VERSION
	#define PLC_OBJ_DISABLE
	#define MOTION_OBJ_DISABLE
	#define CAMERA_OBJ_DISABLE
	#define PHASE_CTRL_DISABLE
	#define LIGHT_CTRL_DISABLE
	#define BARCODE_DEVICE_DISABLE
	#define M2M_DISABLE
	#define HASI_DISABLE
	#define MES_DISABLE	
	#if _MSC_VER >= VS_2010_NET 	
		#define CUDA_USE		//Inline 才可使用Cuda
	#endif//_MSC_VER >= VS_2010_NET 
#else
	#ifdef LABORATORY_VERSION
	#define PLC_OBJ_DISABLE
	#endif//LABORATORY_VERSION

	#define M2M_DISABLE
	#define HASI_DISABLE

	#if _MSC_VER >= VS_2010_NET 	
		#define CUDA_USE		//Inline 才可使用Cuda
	#endif//_MSC_VER >= VS_2010_NET
	#ifdef DISABLE_3D		
		#define PHASE_CTRL_DISABLE
	#endif//DISABLE_3D
	//#define PHASE_CTRL_DISABLE
#endif

#ifndef IDC_HAND
	#define IDC_HAND            MAKEINTRESOURCE(32649)
#endif//IDC_HAND

#ifdef OPEN_MP_USE
	#include <omp.h>
#endif//OPEN_MP_USE

#ifndef BARCODE_DEVICE_DISABLE
	//#define BARCODE_DEVICE_MODULE            1
#endif//BARCODE_DEVICE_DISABLE

#ifdef OPENCV_DISABLE
	#define OPENCV_ML_DISABLE
	#define OPENCV_PHOTO_DISABLE
	#define OPENCV_CALIB_3D_DISABLE
	#error OPENCV Can not be Disable
#else
	#define OPENCV_ML_DISABLE
	#define OPENCV_PHOTO_DISABLE	
	#define OPENCV_CALIB_3D_DISABLE
#endif//OPENCV_DISABLE

#define VK_INSPECTION        VK_F5
#define VK_ONLINE_RUN        VK_F6
#define VK_ENHANCE_IMAGE     VK_F9

#define LoadIDName(Name)      _T(#Name)//載入ID的名稱
#define LoadIDAndName(Name)   Name, _T(#Name)//載入ID與名稱
//----------------------------------------------------------------------------//

#include <math.h>
#include <vector>
#include <algorithm>
#include "JetGroundEquation.h"
#include "Barcode_Define.h"
#include "JetAOI3DDefine.h"
#include "Camera_Define.h"
#include "Light3DDef.h"
#include "AlgParamDef.h"
#include "AOIModelDef.h"
#include "CalibrationDef.h"
#include "SystemParameterDef.h"
#include "ProjectParameterDef.h"
#include "MultiLanguageDef.h"
#include "JET8000MSGDef.h"
#include "AOIExceptionCodeCtrl.h"
#include "FingerprintDefine.h"

#include "JsonCtrl.h"
#include "SortObj.h"
#include "LogManager.h"
#include "JetMemory.h"
#include "AOIFileSpcDef.h"
#include "JetAPIUtility.h"

#include "Plc_Basic.h"
#include "ImageAPI.h"
#include "CameraCtrl.h"
#include "Motion_Basic.h"
#include "Light3DCtrl.h"
#include "LightCtrlBoard.h"
#include "AOIObjManager.h"
#include "AOIDataDefine.h"
#include "AOIDataCollect.h"
#include "Barcode_Device.h"
#include "LogOperCtrl.h"
#include "UserHotKeyCtrl.h"
#include "..\\JET8000_Library\\LockScreen\\Include\\LockScreen.h"	// LockScreen
#ifdef BARCODE_DEVICE_MODULE
#include "BarcodeUnit.h"
#endif//BARCODE_DEVICE_MODULE

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__3985CB25_62CD_4AD9_BF49_875ED5C19147__INCLUDED_)
