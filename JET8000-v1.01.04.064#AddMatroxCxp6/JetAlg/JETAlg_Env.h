#pragma once


#ifdef JETALG_EXPORTS
	//OpenCV
	#include "C:/opencv/build/include/opencv2/opencv.hpp"
	
	#ifdef _DEBUG
	#pragma comment(lib, "opencv_world3416d.lib")
	#else
	#pragma comment(lib, "opencv_world3416.lib")
	#endif // _DEBUG

#endif // JETALG_EXPORTS

#ifndef JETALG_EXPORTS
	#ifndef _ALGDEV_PATH
		#define _ALGDEV_PATH "JETAlg_Dev.h"
	#endif // _ALGDEV_PATH

	#include _ALGDEV_PATH
#endif // !JETALG_EXPORTS