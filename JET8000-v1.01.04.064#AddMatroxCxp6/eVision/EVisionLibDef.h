#ifndef _EVisionLibDef_H_
#define _EVisionLibDef_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#pragma component(browser, off, references)
#ifdef EVISION_USE	

	#define	EMATCH_REDUCE_AREA_AUTO					0
	#define	EMATCH_REDUCE_AREA_64					64
	#define	EMATCH_REDUCE_AREA_128					128
	#define	EMATCH_REDUCE_AREA_256					256
	#define	EMATCH_REDUCE_AREA_512					512
	#define	EMATCH_REDUCE_AREA_MIN					EMATCH_REDUCE_AREA_64
	#define	EMATCH_REDUCE_ARED_MAX					EMATCH_REDUCE_AREA_512
	#define EMATCH_BINARY_LOW                       64
	#define EMATCH_BINARY_HIGH                      255

	#if EVISION_MODE == EVISION_MODE_EVISION
		#include "..\\JET8000_Library\\eVision\\6_7_1_440\\Include\\EImage.h"
		//using namespace Euresys::eVision;

		#define EVS_ROI_BW8   EROIBW8
		#define EVS_ROI_C24   EROIC24
		#define EVS_IMAGE_BW8 EImageBW8
		#define EVS_IMAGE_C24 EImageC24		

		#ifdef EVISION_MATCH_USE	
			#include "..\\JET8000_Library\\eVision\\6_7_1_440\\Include\\EMatch.h"
			#define EVS_MATCH_CLS   EMatch
			#define EVS_MATCH_POS   EMatchPosition	
		#endif//EVISION_MATCH_USE

		#ifdef EVISION_1D_BARCODE_USE
			#include "..\\JET8000_Library\\eVision\\6_7_1_440\\Include\\EBarCode.h"			
			#define EVS_BARCODE_1D  EBarCode
		#endif//EVISION_1D_BARCODE_USE

		#ifdef EVISION_DATA_MATRIX_USE	
			#include "..\\JET8000_Library\\eVision\\6_7_1_440\\Include\\EMatrixCode.h"
		#endif //EVISION_DATA_MATRIX_USE	

	#elif EVISION_MODE == EVISION_MODE_OPEN_EVISION
		//1_2_0_7496
		//1_2_0_7524
		//1_2_7_8735
        //2_5_0_1106
		#if EVISION_MODE_OPEN_EVISION_VERSION == EVISION_MODE_OPEN_EVISION_1_2_0_7496
			#include "..\\JET8000_Library\\Open eVision\\1_2_0_7496\\Include\\Open_eVision_1_2.h"	
		#elif EVISION_MODE_OPEN_EVISION_VERSION == EVISION_MODE_OPEN_EVISION_1_2_0_7524
			#include "..\\JET8000_Library\\Open eVision\\1_2_0_7524\\Include\\Open_eVision_1_2.h"	
		#elif EVISION_MODE_OPEN_EVISION_VERSION == EVISION_MODE_OPEN_EVISION_1_2_7_8735
			#include "..\\JET8000_Library\\Open eVision\\1_2_7_8735\\Include\\Open_eVision_1_2.h"	
		#elif EVISION_MODE_OPEN_EVISION_VERSION == EVISION_MODE_OPEN_EVISION_1_3_2_0000
			#include "..\\JET8000_Library\\Open eVision\\1_3_2_0000\\Include\\Open_eVision_1_3.h"
		#elif EVISION_MODE_OPEN_EVISION_VERSION == EVISION_MODE_OPEN_EVISION_2_5_0_1106
			#include "..\\JET8000_Library\\Open eVision\\2_5_0_1106\\Include\\Open_eVision_2_5.h"	
		#elif EVISION_MODE_OPEN_EVISION_VERSION == EVISION_MODE_OPEN_EVISION_22_12_2_15123
			#include "..\\JET8000_Library\\Open eVision\\22_12_2_15123\\Include\\Open_eVision_22_12.h"
		#elif EVISION_MODE_OPEN_EVISION_VERSION == EVISION_MODE_OPEN_EVISION_23_12_0_18439
			#include "..\\JET8000_Library\\Open eVision\\23_12_0_18439\\Include\\Open_eVision_23_12.h"
		#else
			#error Open eVision Version not Defined.
		#endif//EVISION_MODE_OPEN_EVISION_VERSION
		
		#if EVISION_MODE_OPEN_EVISION_VERSION >= EVISION_MODE_OPEN_EVISION_23_12_0_18439
			using namespace Euresys::Open_eVision_23_12;
		#elif EVISION_MODE_OPEN_EVISION_VERSION >= EVISION_MODE_OPEN_EVISION_22_12_2_15123
			using namespace Euresys::Open_eVision_22_12;
		#elif EVISION_MODE_OPEN_EVISION_VERSION >= EVISION_MODE_OPEN_EVISION_2_5_0_1106
			using namespace Euresys::Open_eVision_2_5;			
		#elif EVISION_MODE_OPEN_EVISION_VERSION >= EVISION_MODE_OPEN_EVISION_1_3_2_0000
			using namespace Euresys::Open_eVision_1_3;
		#else
			using namespace Euresys::Open_eVision_1_2;
		#endif//EVISION_MODE_OPEN_EVISION_VERSION
			#define EVS_ROI_BW8                 EROIBW8
			#define EVS_ROI_C24                 EROIC24	

			#define EVS_IMAGE_BW8               EImageBW8
			#define EVS_IMAGE_C24               EImageC24	
		
		#ifdef EVISION_MATCH_USE			
			#define E_MATCH_STANDARD			ECorrelationMode_Standard
			#define E_MATCH_OFFSET_NORMALIZED	ECorrelationMode_OffsetNormalized
			#define E_MATCH_GAIN_NORMALIZED		ECorrelationMode_GainNormalized
			#define E_MATCH_NORMALIZED			ECorrelationMode_Normalized

			#define MCH_CONTRAST_NORMAL			EMatchContrastMode_Normal
			#define MCH_CONTRAST_INVERSE		EMatchContrastMode_Inverse
			#define MCH_CONTRAST_ANY			EMatchContrastMode_Any

			#define MCH_UNIFORM					EFilteringMode_Uniform
			#define MCH_LOWPASS					EFilteringMode_LowPass	
			
			#define EVS_MATCH_CLS               EMatcher
			#define EVS_MATCH_POS               EMatchPosition					
		#endif//EVISION_MATCH_USE

		#ifdef EVISION_1D_BARCODE_USE			
			#define EVS_BARCODE_1D              EBarCode				
		#endif //EVISION_1D_BARCODE_USE
		
	#endif//EVISION_MODE

#endif //endif EVISION_USE
//#pragma component(browser, on, references)
#endif//EVisionLibDef