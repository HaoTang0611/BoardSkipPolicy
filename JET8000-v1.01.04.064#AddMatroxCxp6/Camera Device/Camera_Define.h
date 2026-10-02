#ifndef _CAMERA_DEFINE_H_
#define _CAMERA_DEFINE_H_

//CAMERA_OBJ_DISABLE
//-------------------------------------------------------------------------------------//
#define CAMERA_OBJ_GRABLINKFULL_CSC6M100BMP11_P100               1
#define CAMERA_OBJ_GRABLINKFULL_Q12A65FM                         2
#define CAMERA_OBJ_COAXLINK_Q_12A180_FM                        101
#define CAMERA_OBJ_COAXLINK_VC_12MX_M180                       102
#define CAMERA_OBJ_COAXLINK_QUAD_G3_CAMERA                     103
#define CAMERA_OBJ_COAXLINK_QUAD_CXP12_CAMERA                  105
#define CAMERA_OBJ_TELI_BU1203MC                               201
#define CAMERA_OBJ_TELI_BU1207MCF                              202
#define CAMERA_OBJ_MATROX_RAPIXO_CXP12_CAMERA                  301
#define CAMERA_OBJ_MATROX_RAPIXO_CXP6_CAMERA                   302
#define CAMERA_OBJ_DYNAMIC_MODULE                              999

//#define CAMERA_OBJ_MODE     CAMERA_OBJ_GRABLINKFULL_CSC6M100BMP11_P100
//#define CAMERA_OBJ_MODE     CAMERA_OBJ_GRABLINKFULL_Q12A65FM
//#define CAMERA_OBJ_MODE     CAMERA_OBJ_COAXLINK_Q_12A180_FM
//#define CAMERA_OBJ_MODE     CAMERA_OBJ_COAXLINK_VC_12MX_M180
//#define CAMERA_OBJ_MODE     CAMERA_OBJ_COAXLINK_QUAD_G3_CAMERA
//#define CAMERA_OBJ_MODE     CAMERA_OBJ_COAXLINK_QUAD_CXP12_CAMERA
//#define CAMERA_OBJ_MODE     CAMERA_OBJ_TELI_BU1203MC
//#define CAMERA_OBJ_MODE     CAMERA_OBJ_TELI_BU1207MCF
//#define CAMERA_OBJ_MODE     CAMERA_OBJ_MATROX_RAPIXO_CXP12_CAMERA
#define CAMERA_OBJ_MODE     CAMERA_OBJ_MATROX_RAPIXO_CXP6_CAMERA
//#define CAMERA_OBJ_MODE     CAMERA_OBJ_DYNAMIC_MODULE
//-------------------------------------------------------------------------------------//
#define CAMERA_IMAGE_BUFFER_COUNT_3D                                    96//84//64
#define CAMERA_IMAGE_BUFFER_COUNT_2D                                    24//24
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE == CAMERA_OBJ_GRABLINKFULL_CSC6M100BMP11_P100
	#define GRABLINKFULL_CSC6M100BMP11_P100_USE	
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE == CAMERA_OBJ_GRABLINKFULL_Q12A65FM
	#define GRABLINKFULL_Q12A65FM_USE	
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE == CAMERA_OBJ_COAXLINK_Q_12A180_FM
	#define COAXLINK_Q_12A180F_USE	
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE == CAMERA_OBJ_COAXLINK_VC_12MX_M180
	#define COAXLINK_VC_12MX_M180_USE
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE == CAMERA_OBJ_COAXLINK_QUAD_G3_CAMERA
	#define COAXLINK_QUAD_G3_CAMERA_USE
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE == CAMERA_OBJ_COAXLINK_QUAD_CXP12_CAMERA
	#define COAXLINK_QUAD_CXP12_CAMERA_USE
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE == CAMERA_OBJ_TELI_BU1203MC
	#define TELI_BU1203MC_USE	
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE == CAMERA_OBJ_TELI_BU1207MCF
	#define TELI_BU1207MCF_USE	
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE == CAMERA_OBJ_MATROX_RAPIXO_CXP12_CAMERA
	#define MATROX_RAPIXO_CXP12_CAMERA_USE	
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE == CAMERA_OBJ_MATROX_RAPIXO_CXP6_CAMERA
	#define MATROX_RAPIXO_CXP6_CAMERA_USE	
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
#if CAMERA_OBJ_MODE == CAMERA_OBJ_DYNAMIC_MODULE	
	#define CAMERA_DYNAMIC_MODULE_USE
#endif//CAMERA_OBJ_MODE
//-------------------------------------------------------------------------------------//
enum CAMERA_IMAGE_MODE//相機影像模式
{
	CAMERA_IMAGE_GRAY     = 1,//黑白相機
	CAMERA_IMAGE_BAYER    = 2,//Bayer彩色相機
	CAMERA_IMAGE_COLOR    = 3,//RGB-3個感應器
	CAMERA_IMAGE_RETURN
};
//-------------------------------------------------------------------------------------//
enum CAMERA_GRAB_MODE
{
	CAMERA_GRAB_UNDEFINED =0,
	CAMERA_GRAB_FREE_RUN = 1,
	CAMERA_GRAB_EXTERNAL_TRIGGER = 2,
	CAMERA_GRAB_SOFTWARE_TRIGGER = 3
};
//-------------------------------------------------------------------------------------//
enum CAMERA_SHUTTER_MODE//相機電子快門模式
{
	CAMERA_SHUTTER_GLOBAL, //Global Shutter
	CAMERA_SHUTTER_ROLLING //Rolling Shutter
};
//-------------------------------------------------------------------------------------//
enum CAMERA_EXPOSURE_MODE//相機曝光模式
{
	CAMERA_EXPOSURE_UNDEFINED      = 0,//未定義
	CAMERA_EXPOSURE_TIMED          = 1,//時間-時間固定
	CAMERA_EXPOSURE_TRIGGER_WIDTH  = 2,//觸發寬度-時間可變
};
//-------------------------------------------------------------------------------------//
enum CAMERA_RING_BUFFER_STATES
{
	CAMERA_RING_BUFFER_STATES_NONE    = 0,//未定義
	CAMERA_RING_BUFFER_STATES_NEW     = 1,//新影像
	CAMERA_RING_BUFFER_STATES_DONE    = 2,//已完成(複製走)
};
//-------------------------------------------------------------------------------------//
enum BAYER_PATTERN_MODE
{
	BAYER_PATTERN_NONE = 0,
	BAYER_PATTERN_RGGB = 1,
	BAYER_PATTERN_GRBG = 2,
	BAYER_PATTERN_GBRG = 3,
	BAYER_PATTERN_BGGR = 4,
	BAYER_PATTERN_RETURN
};
//-------------------------------------------------------------------------------------//
enum CAMERA_ROTATION_MODE
{
	CAMERA_ROTATION_000 = 0,  //沒有轉向的問題
	CAMERA_ROTATION_090 = 1,  //順時針旋轉90的相機
	CAMERA_ROTATION_180 = 2,  //順時針旋轉180的相機
	CAMERA_ROTATION_270 = 3,  //順時針旋轉90的相機
	CAMERA_ROTATION_000_YMIRROR = 4,  //沒有轉向的問題+Y鏡射
	CAMERA_ROTATION_090_YMIRROR = 5,  //順時針旋轉90的相機+Y鏡射
	CAMERA_ROTATION_180_YMIRROR = 6,  //順時針旋轉180的相機+Y鏡射
	CAMERA_ROTATION_270_YMIRROR = 7  //順時針旋轉90的相機+Y鏡射
};
//-------------------------------------------------------------------------------------//
enum CAMERA_CALLBACK_TIMMING
{
	CAMERA_CALLBACK_EACH_FRAME       = 1,//每張影像回傳
	CAMERA_CALLBACK_FREE_FRAME       = 2,//每張可回傳影像時, 
	CAMERA_CALLBACK_BATCH_GRAB_DONE  = 3 //每次批次取像後回傳
};
//-------------------------------------------------------------------------------------//
enum COAXPRESS_CAMERA_DEVICE
{	
	COAXPRESS_CAMERA_Q_12A180F        = 1,//CXP6
	COAXPRESS_CAMERA_VC_12MX_M180     = 2,//CXP6-預設值
	COAXPRESS_CAMERA_VC_12MX_M180_HOR = 3,//CXP6
	COAXPRESS_CAMERA_VCC_25CXPHSM     = 4,//CXP12-預設值
	COAXPRESS_CAMERA_VC_25MX2_M150I   = 5,//CXP12
	COAXPRESS_CAMERA_STC_CMB120ACXP   = 6,//CXP6
	COAXPRESS_CAMERA_STC_LBGP251BCXP124 = 7,//CXP12-Alan
	//COAXPRESS_CAMERA_VC_25MX2_M150I   = 7,//CXP12
	COAXPRESS_CAMERA_RETURN
};
//-------------------------------------------------------------------------------------//
#endif//_CAMERA_DEFINE_H_