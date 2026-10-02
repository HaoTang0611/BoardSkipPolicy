#ifndef _JetLoadDll_H_
#define _JetLoadDll_H_

#ifdef _MSC_VER	

#pragma once

	//Rpc
	#pragma comment(lib, "Rpcrt4.lib") 

	//Version
	#pragma comment(lib, "Version.lib") 

	//MS Image32
#if _MSC_VER == VC_6
	#pragma comment(lib, "msimg32.lib") 
#endif

#ifndef OPENCV_DISABLE
	#if OPEN_CV_VERSION == OPEN_CV_3_4_16_00_V14
		#ifdef _X64		
			#ifdef _DEBUG				
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_3_4_16\\x64\\Lib\\vc14\\opencv_world3416d.lib")				
			#else
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_3_4_16\\x64\\Lib\\vc14\\opencv_world3416.lib")				
			#endif//_DEBUG
		#else
			#error OPEN_CV_3_4_16_00_V14 is not support x86.
		#endif//_X64
	#elif OPEN_CV_VERSION == OPEN_CV_2_4_13_06_V14
		//----------------------------------------------------------//
		#ifdef _X64		
			#ifdef _DEBUG
				//#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x64\\Lib\\vc14\\opencv_core2413d.lib")
				//#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x64\\Lib\\vc14\\opencv_highgui2413d.lib")
				//#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x64\\Lib\\vc14\\opencv_imgproc2413d.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x64\\Lib\\vc14\\opencv_core2413d.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x64\\Lib\\vc14\\opencv_highgui2413d.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x64\\Lib\\vc14\\opencv_imgproc2413d.lib")
			#ifndef OPENCV_ML_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x64\\Lib\\vc14\\opencv_ml2413d.lib")
			#endif//OPENCV_ML_DISABLE
			#ifndef OPENCV_PHOTO_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x64\\Lib\\vc14\\opencv_photo2413d.lib")
			#endif//OPENCV_PHOTO_DISABLE
			#ifndef OPENCV_CALIB_3D_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x64\\Lib\\vc14\\opencv_calib3d2413d.lib")
			#endif//OPENCV_CALIB_3D_DISABLE
			#else
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x64\\Lib\\vc14\\opencv_core2413.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x64\\Lib\\vc14\\opencv_highgui2413.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x64\\Lib\\vc14\\opencv_imgproc2413.lib")
			#ifndef OPENCV_ML_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x64\\Lib\\vc14\\opencv_ml2413.lib")
			#endif//OPENCV_ML_DISABLE
			#ifndef OPENCV_PHOTO_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x64\\Lib\\vc14\\opencv_photo2413.lib")
			#endif//OPENCV_PHOTO_DISABLE
			#ifndef OPENCV_CALIB_3D_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x64\\Lib\\vc14\\opencv_calib3d2413.lib")
			#endif//OPENCV_CALIB_3D_DISABLE
			#endif//_DEBUG
		#else
			#ifdef _DEBUG
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x32\\Lib\\vc14\\opencv_core2413d.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x32\\Lib\\vc14\\opencv_highgui2413d.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x32\\Lib\\vc14\\opencv_imgproc2413d.lib")
			#ifndef OPENCV_ML_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x32\\Lib\\vc14\\opencv_ml2413d.lib")
			#endif//OPENCV_ML_DISABLE
			#ifndef OPENCV_PHOTO_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x32\\Lib\\vc14\\opencv_photo2413d.lib")
			#endif//OPENCV_PHOTO_DISABLE
			#ifndef OPENCV_CALIB_3D_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x32\\Lib\\vc14\\opencv_calib3d2413d.lib")
			#endif//OPENCV_CALIB_3D_DISABLE
			#else
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x32\\Lib\\vc14\\opencv_core2413.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x32\\Lib\\vc14\\opencv_highgui2413.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x32\\Lib\\vc14\\opencv_imgproc2413.lib")		
			#ifndef OPENCV_ML_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x32\\Lib\\vc14\\opencv_ml2413.lib")
			#endif//OPENCV_ML_DISABLE
			#ifndef OPENCV_PHOTO_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x32\\Lib\\vc14\\opencv_photo2413.lib")
			#endif//OPENCV_PHOTO_DISABLE
			#ifndef OPENCV_CALIB_3D_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\x32\\Lib\\vc14\\opencv_calib3d2413.lib")
			#endif//OPENCV_CALIB_3D_DISABLE
			#endif//_DEBUG			
		#endif//_X64
	#elif OPEN_CV_VERSION == OPEN_CV_2_4_11_00_V10
		//----------------------------------------------------------//
		#ifdef _X64		
			#ifdef _DEBUG
				//#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x64\\Lib\\vc10\\opencv_core2411d.lib")
				//#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x64\\Lib\\vc10\\opencv_highgui2411d.lib")
				//#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x64\\Lib\\vc10\\opencv_imgproc2411d.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x64\\Lib\\vc10\\opencv_core2411.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x64\\Lib\\vc10\\opencv_highgui2411.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x64\\Lib\\vc10\\opencv_imgproc2411.lib")
			#ifndef OPENCV_ML_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x64\\Lib\\vc10\\opencv_ml2411d.lib")
			#endif//OPENCV_ML_DISABLE
			#ifndef OPENCV_PHOTO_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x64\\Lib\\vc10\\opencv_photo2411d.lib")
			#endif//OPENCV_PHOTO_DISABLE
			#ifndef OPENCV_CALIB_3D_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x64\\Lib\\vc10\\opencv_calib3d2411d.lib")				
			#endif//OPENCV_CALIB_3D_DISABLE
			#else
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x64\\Lib\\vc10\\opencv_core2411.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x64\\Lib\\vc10\\opencv_highgui2411.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x64\\Lib\\vc10\\opencv_imgproc2411.lib")
			#ifndef OPENCV_ML_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x64\\Lib\\vc10\\opencv_ml2411.lib")
			#endif//OPENCV_ML_DISABLE
			#ifndef OPENCV_PHOTO_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x64\\Lib\\vc10\\opencv_photo2411.lib")
			#endif//OPENCV_PHOTO_DISABLE
			#ifndef OPENCV_CALIB_3D_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x64\\Lib\\vc10\\opencv_calib3d2411.lib")				
			#endif//OPENCV_CALIB_3D_DISABLE
			#endif//_DEBUG
		#else
			#ifdef _DEBUG
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x32\\Lib\\vc10\\opencv_core2411d.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x32\\Lib\\vc10\\opencv_highgui2411d.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x32\\Lib\\vc10\\opencv_imgproc2411d.lib")
			#ifndef OPENCV_ML_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x32\\Lib\\vc10\\opencv_ml2411d.lib")
			#endif//OPENCV_ML_DISABLE
			#ifndef OPENCV_PHOTO_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x32\\Lib\\vc10\\opencv_photo2411d.lib")
			#endif//OPENCV_PHOTO_DISABLE
			#ifndef OPENCV_CALIB_3D_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x32\\Lib\\vc10\\opencv_calib3d2411d.lib")				
			#endif//OPENCV_CALIB_3D_DISABLE
			#else
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x32\\Lib\\vc10\\opencv_core2411.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x32\\Lib\\vc10\\opencv_highgui2411.lib")
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x32\\Lib\\vc10\\opencv_imgproc2411.lib")		
			#ifndef OPENCV_ML_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x32\\Lib\\vc10\\opencv_ml2411.lib")
			#endif//OPENCV_ML_DISABLE
			#ifndef OPENCV_PHOTO_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x32\\Lib\\vc10\\opencv_photo2411.lib")
			#endif//OPENCV_PHOTO_DISABLE
			#ifndef OPENCV_CALIB_3D_DISABLE
				#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\x32\\Lib\\vc10\\opencv_calib3d2411.lib")				
			#endif//OPENCV_CALIB_3D_DISABLE
			#endif//_DEBUG			
		#endif//_X64
		//----------------------------------------------------------//
	#else
		#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_0_9_7_1\\x32\\Lib\\cv.lib")
		#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_0_9_7_1\\x32\\Lib\\cxcore.lib")
		#pragma comment(lib,"..\\JET8000_Library\\OpenCV\\OpenCV_0_9_7_1\\x32\\Lib\\highgui.lib")
		//----------------------------------------------------------//
	#endif//endif _MSC_VER >= VS_2008_NET
#endif//OPENCV_DISABLE

#ifdef _X64		
	#ifdef _DEBUG
		#pragma comment(lib, "..\\JET8000_Library\\JPEG\\x64\\Lib\\JpegLibD_64.lib")
	#else
		#pragma comment(lib, "..\\JET8000_Library\\JPEG\\x64\\Lib\\JpegLib_64.lib")
	#endif
#else		
	#ifdef _DEBUG
		#pragma comment(lib, "..\\JET8000_Library\\JPEG\\x32\\Lib\\JpegLibD.lib")
	#else
		#pragma comment(lib, "..\\JET8000_Library\\JPEG\\x32\\Lib\\JpegLib.lib")
	#endif
#endif//_X64


#ifndef OFFLINE_VERSION//Lock Screen	
	#ifdef _X64		
		#pragma comment(lib,"..\\JET8000_Library\\LockScreen\\Lib\\x64\\LockScreenV1_2(x64).lib")		
	#else
		#pragma comment(lib,"..\\JET8000_Library\\LockScreen\\Lib\\x86\\LockScreenV1_2(x86).lib")
	#endif
#endif//OFFLINE_VERSION

#ifdef EVISION_USE	
	#if EVISION_MODE == EVISION_MODE_EVISION
		#if _MSC_VER == VC_6
			#ifdef _DEBUG
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EMchMs60d.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EasyMs60d.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EOcrMs60d.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EMxcMs60d.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EBrcMs60d.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EInsMs60d.lib")	
			#else
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EMchMs60.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EasyMs60.lib")	
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EOcrMs60.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EMxcMs60.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EBrcMs60.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EInsMs60.lib")			
			#endif
		#elif _MSC_VER <= VS_2015_NET
			#ifdef _DEBUG			
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EMchMs71d.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EasyMs71d.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EOcrMs71d.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EMxcMs71d.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EBrcMs71d.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EInsMs71d.lib")
			#else			
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EMchMs71.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EasyMs71.lib")	
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EOcrMs71.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EMxcMs71.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EBrcMs71.lib")
				#pragma comment(lib, "..\\JET8000_Library\\eVision\\6_7_1_440\\x32\\Lib\\EInsMs71.lib")
			#endif
		#else
			#error eVision Library Exception
		#endif
	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		//1_2_0_7496
		//1_2_0_7524
		//1_2_7_8735		
		#ifdef _X64		
			#ifdef _DEBUG			
			//	#pragma comment(lib, "..\\JET8000_Library\\Open eVision\\1_2_0_7524\\x64\\Lib\\Open_eVision_1_2_VC6_Debug.lib")
			#else			
			//	#pragma comment(lib, "..\\JET8000_Library\\Open eVision\\1_2_0_7524\\x64\\Lib\\Open_eVision_1_2_VC6_Release.lib")
			#endif//endif _DEBUG
		#else//_X64		
			#ifdef _DEBUG
				#if EVISION_MODE_OPEN_EVISION_VERSION == EVISION_MODE_OPEN_EVISION_1_2_0_7496
					#pragma comment(lib, "..\\JET8000_Library\\Open eVision\\1_2_0_7496\\x32\\Lib\\Open_eVision_1_2_VC6_Debug.lib")
				#elif EVISION_MODE_OPEN_EVISION_VERSION == EVISION_MODE_OPEN_EVISION_1_2_0_7524
					#pragma comment(lib, "..\\JET8000_Library\\Open eVision\\1_2_0_7524\\x32\\Lib\\Open_eVision_1_2_VC6_Debug.lib")
					//#pragma comment(lib, "..\\JET8000_Library\\Open eVision\\1_2_0_7524\\x32\\Lib\\Legacy_Open_eVision_1_2_VC6_Debug.lib")
				#elif EVISION_MODE_OPEN_EVISION_VERSION == EVISION_MODE_OPEN_EVISION_1_2_7_8735
					#pragma comment(lib, "..\\JET8000_Library\\Open eVision\\1_2_7_8735\\x32\\Lib\\Open_eVision_1_2_VC6_Debug.lib")				
				#endif//EVISION_MODE_OPEN_EVISION_VERSION
			#else
				#if EVISION_MODE_OPEN_EVISION_VERSION == EVISION_MODE_OPEN_EVISION_1_2_0_7496
					#pragma comment(lib, "..\\JET8000_Library\\Open eVision\\1_2_0_7496\\x32\\Lib\\Open_eVision_1_2_VC6_Release.lib")
				#elif EVISION_MODE_OPEN_EVISION_VERSION == EVISION_MODE_OPEN_EVISION_1_2_0_7524
					#pragma comment(lib, "..\\JET8000_Library\\Open eVision\\1_2_0_7524\\x32\\Lib\\Open_eVision_1_2_VC6_Release.lib")
					//#pragma comment(lib, "..\\JET8000_Library\\Open eVision\\1_2_0_7524\\x32\\Lib\\Legacy_Open_eVision_1_2_VC6_Release.lib")
				#elif EVISION_MODE_OPEN_EVISION_VERSION == EVISION_MODE_OPEN_EVISION_1_2_7_8735
					#pragma comment(lib, "..\\JET8000_Library\\Open eVision\\1_2_7_8735\\x32\\Lib\\Open_eVision_1_2_VC6_Release.lib")				
				#endif//EVISION_MODE_OPEN_EVISION_VERSION
			#endif//endif _DEBUG
		#endif//_X64		
	#endif//EVISION_MODE
#endif // EVISION_USE

#ifdef ZLIB_USE 
	#ifdef _X64	
		#pragma comment(lib, "..\\JET8000_Library\\Zlib\\x64\\Lib\\zlibwapi.lib")
	#else	
		#pragma comment(lib, "..\\JET8000_Library\\Zlib\\x32\\Lib\\zlibwapi.lib")
	#endif//_X64
#endif//ZLIB_USE

#endif//_MSC_VER	

#endif//JetLoadDll