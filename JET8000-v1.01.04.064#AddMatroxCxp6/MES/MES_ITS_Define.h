#ifndef _MES_ITS_DEFINE_H_
#define _MES_ITS_DEFINE_H_
//-------------------------------------------------------------------------------------//
enum ITS_STATAUS_ID
{
	ITS_STATAUS_NONE                        =  0,

	ITS_STATAUS_MES_SET_CMD                 =   1,//MES Set Parameters
	ITS_STATAUS_MES_SET_RES                 =   2,//MES Set Parameters Response
	ITS_STATAUS_MES_GET_CMD                 =   3,//MES Set GetParameters
	ITS_STATAUS_MES_GET_RES                 =   4,//MES GetParameters Response

	ITS_STATAUS_AOI_SET_CMD                 =  33,//AOI Set Parameters
	ITS_STATAUS_AOI_SET_RES                 =  34,//AOI Set Parameters Response
	ITS_STATAUS_AOI_GET_CMD                 =  35,//AOI Get Parameters
	ITS_STATAUS_AOI_GET_RES                 =  36,//AOI Get Parameters Response

	ITS_STATAUS_VRS_SET_CMD                 =  37,//VRS Set Parameters
	ITS_STATAUS_VRS_SET_RES                 =  38,//VRS Set Parameters Response
	ITS_STATAUS_VRS_GET_CMD                 =  39,//VRS Get Parameters
	ITS_STATAUS_VRS_GET_RES                 =  40,//VRS Get Parameters Response

	ITS_STATAUS_AOI_READY_TO_LOAD_CMD       =  65,//AOI ReadyToLoad
	ITS_STATAUS_AOI_READY_TO_LOAD_RES       =  66,//AOI ReadyToLoad Response

	ITS_STATAUS_AOI_LOAd_COMPLETE_CMD       =  67,//AOI Load Complete 
	ITS_STATAUS_AOI_LOAd_COMPLETE_RES       =  68,//AOI Load Complete  Response

	ITS_STATAUS_AOI_START_INSPECTION_CMD    =  69,//AOI Start Inspection
	ITS_STATAUS_AOI_START_INSPECTION_RES    =  70,//AOI Start Inspection Response

	ITS_STATAUS_AOI_INSPECTION_COMPLETE_CMD =  71,//AOI Inspection Complete
	ITS_STATAUS_AOI_INSPECTION_COMPLETE_RES =  72,//AOI Inspection Complete Response

	ITS_STATAUS_AOI_READY_TO_UNLOAD_CMD     =  73,//AOI ReadyToUnload
	ITS_STATAUS_AOI_READY_TO_UNLOAD_RES     =  74,//AOI ReadyToUnload Response

	ITS_STATAUS_AOI_UNLOAd_COMPLETE_CMD     =  75,//AOI Unload Complete 
	ITS_STATAUS_AOI_UNLOAd_COMPLETE_RES     =  76,//AOI Unload Complete  Response	

	ITS_STATAUS_AOI_LOGIN_OUT_CMD           =  81,//AOI Login/out Parameters
	ITS_STATAUS_AOI_LOGIN_OUT_RES           =  82,//AOI Login/out Parameters Response

	ITS_STATAUS_AOI_CHECK_BARCODE_CMD       =  83,//AOI Check Barcode
	ITS_STATAUS_AOI_CHECK_BARCODE_RES       =  84,//AOI Check Barcode Response

	ITS_STATAUS_AOI_INSPECTION_STOP_CMD     =  85,//AOI Inspection Stop
	ITS_STATAUS_AOI_INSPECTION_STOP_RES     =  86,//AOI Inspection Stop Response

	ITS_STATAUS_VRS_UPLOAD_SFC_CMD          =  160,//VRS Upload SFC
	ITS_STATAUS_VRS_UPLOAD_SFC_RES          =  161,//VRS Upload SFC Response

	ITS_STATAUS_RETURN
};
//-------------------------------------------------------------------------------------//
#endif//_MES_ITS_DEFINE_H_