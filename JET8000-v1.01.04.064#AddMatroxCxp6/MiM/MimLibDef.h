#ifndef _MimLibDef_H_
#define _MimLibDef_H_

#ifdef MIM_LIB_USE	
	#ifndef _X64
		#include "..\\JET8000_Library\\MiM\\Include\\x32\\iVisionErrs.h"
		#include "..\\JET8000_Library\\MiM\\Include\\x32\\iVision_Types.h"
		#include "..\\JET8000_Library\\MiM\\Include\\x32\\iVision.h"
		#include "..\\JET8000_Library\\MiM\\Include\\x32\\iImage.h"
		#include "..\\JET8000_Library\\MiM\\Include\\x32\\iROI.h"

		#pragma comment(lib,"..\\JET8000_Library\\MiM\\Lib\\x32\\iImage.lib")		
		#pragma comment(lib,"..\\JET8000_Library\\MiM\\Lib\\x32\\iVision.lib")
		#pragma comment(lib,"..\\JET8000_Library\\MiM\\Lib\\x32\\iROI.lib")

		#ifdef MIM_MATCH_USE	
			#include "..\\JET8000_Library\\MiM\\Include\\x32\\iMatch.h"			
			#pragma comment(lib,"..\\JET8000_Library\\MiM\\Lib\\x32\\iMatch.lib")
		#endif//MIM_MATCH_USE
#else//_X64
		#include "..\\JET8000_Library\\MiM\\Include\\x64\\iVisionErrs.h"
		#include "..\\JET8000_Library\\MiM\\Include\\x64\\iVision_Types.h"
		#include "..\\JET8000_Library\\MiM\\Include\\x64\\iVision_x64.h"
		#include "..\\JET8000_Library\\MiM\\Include\\x64\\iImage_x64.h"
		#include "..\\JET8000_Library\\MiM\\Include\\x64\\iROI_x64.h"

		#pragma comment(lib,"..\\JET8000_Library\\MiM\\Lib\\x64\\iImage_x64.lib")
		#pragma comment(lib,"..\\JET8000_Library\\MiM\\Lib\\x64\\iVision_x64.lib")
		#pragma comment(lib,"..\\JET8000_Library\\MiM\\Lib\\x64\\iROI_x64.lib")

		#ifdef MIM_MATCH_USE	
			#include "..\\JET8000_Library\\MiM\\Include\\x64\\iMatch_x64.h"			
			#pragma comment(lib,"..\\JET8000_Library\\MiM\\Lib\\x64\\iMatch_x64.lib")
		#endif//MIM_MATCH_USE
		
	#endif//_X64
#endif//MIM_LIB_USE

#endif//MimLibDef