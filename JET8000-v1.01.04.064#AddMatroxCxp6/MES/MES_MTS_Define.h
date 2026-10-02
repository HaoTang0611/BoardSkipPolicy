#ifndef _MES_MTS_DEFINE_H_
#define _MES_MTS_DEFINE_H_
//-------------------------------------------------------------------------------------//
enum ITS_STATAUS_ID
{
	MTS_STATAUS_NONE                        =  0,

	MTS_STATAUS_MES_SET_CMD                 =   1,//MES Set Parameters
	MTS_STATAUS_MES_SET_RES                 =   2,//MES Set Parameters Response
	MTS_STATAUS_MES_GET_CMD                 =   3,//MES Set GetParameters
	MTS_STATAUS_MES_GET_RES                 =   4,//MES GetParameters Response

	MTS_STATAUS_AOI_SET_CMD                 =  33,//AOI Set Parameters
	MTS_STATAUS_AOI_SET_RES                 =  34,//AOI Set Parameters Response
	MTS_STATAUS_AOI_GET_CMD                 =  35,//AOI Get Parameters
	MTS_STATAUS_AOI_GET_RES                 =  36,//AOI Get Parameters Response

	MTS_STATAUS_VRS_SET_CMD                 =  37,//VRS Set Parameters
	MTS_STATAUS_VRS_SET_RES                 =  38,//VRS Set Parameters Response
	MTS_STATAUS_VRS_GET_CMD                 =  39,//VRS Get Parameters
	MTS_STATAUS_VRS_GET_RES                 =  40,//VRS Get Parameters Response

	MTS_STATAUS_AOI_READY_TO_LOAD_CMD       =  65,//AOI ReadyToLoad
	MTS_STATAUS_AOI_READY_TO_LOAD_RES       =  66,//AOI ReadyToLoad Response

	MTS_STATAUS_AOI_LOAd_COMPLETE_CMD       =  67,//AOI Load Complete 
	MTS_STATAUS_AOI_LOAd_COMPLETE_RES       =  68,//AOI Load Complete  Response

	MTS_STATAUS_AOI_START_INSPECTION_CMD    =  69,//AOI Start Inspection
	MTS_STATAUS_AOI_START_INSPECTION_RES    =  70,//AOI Start Inspection Response

	MTS_STATAUS_AOI_INSPECTION_COMPLETE_CMD =  71,//AOI Inspection Complete
	MTS_STATAUS_AOI_INSPECTION_COMPLETE_RES =  72,//AOI Inspection Complete Response

	MTS_STATAUS_AOI_READY_TO_UNLOAD_CMD     =  73,//AOI ReadyToUnload
	MTS_STATAUS_AOI_READY_TO_UNLOAD_RES     =  74,//AOI ReadyToUnload Response

	MTS_STATAUS_AOI_UNLOAd_COMPLETE_CMD     =  75,//AOI Unload Complete 
	MTS_STATAUS_AOI_UNLOAd_COMPLETE_RES     =  76,//AOI Unload Complete  Response	

	MTS_STATAUS_AOI_LOGIN_OUT_CMD           =  81,//AOI Login/out Parameters
	MTS_STATAUS_AOI_LOGIN_OUT_RES           =  82,//AOI Login/out Parameters Response

	MTS_STATAUS_AOI_CHECK_BARCODE_CMD       =  83,//AOI Check Barcode
	MTS_STATAUS_AOI_CHECK_BARCODE_RES       =  84,//AOI Check Barcode Response

	MTS_STATAUS_AOI_INSPECTION_STOP_CMD     =  85,//AOI Inspection Stop
	MTS_STATAUS_AOI_INSPECTION_STOP_RES     =  86,//AOI Inspection Stop Response

	MTS_STATAUS_VRS_UPLOAD_SFC_CMD          =  160,//VRS Upload SFC
	MTS_STATAUS_VRS_UPLOAD_SFC_RES          =  161,//VRS Upload SFC Response

	MTS_STATAUS_RETURN
};
//-------------------------------------------------------------------------------------//
#endif//_MES_MTS_DEFINE_H_