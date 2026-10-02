#ifndef OPENCV_DEF_H_
#define OPENCV_DEF_H_

#ifndef OPENCV_DISABLE
	#if OPEN_CV_VERSION == OPEN_CV_3_4_16_00_V14
		#include "..\\JET8000_Library\\OpenCV\\OpenCV_3_4_16\\Include\\opencv.hpp"
	#elif OPEN_CV_VERSION == OPEN_CV_2_4_13_06_V14
		#include "..\\JET8000_Library\\OpenCV\\OpenCV_2_4_13_6\\Include\\opencv.hpp"
	#elif OPEN_CV_VERSION == OPEN_CV_2_4_11_00_V10
		#include "..\\JET8000_Library\\OpenCV\\OpenCV_2_4_11\\Include\\opencv.hpp"
	#else
		#include "..\\JET8000_Library\\OpenCV\\OpenCV_0_9_7_1\\Include\\cv.h"
		#include "..\\JET8000_Library\\OpenCV\\OpenCV_0_9_7_1\\Include\\highgui.h"
		#include "..\\JET8000_Library\\OpenCV\\OpenCV_0_9_7_1\\Include\\cxcore.h"
	#endif
#endif//OPENCV_DISABLE

#endif//OPENCV_DEF_H_
