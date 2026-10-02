#ifndef _MES_IPS_DEFINE_H_
#define _MES_IPS_DEFINE_H_
//-------------------------------------------------------------------------------------//
#define IPS_VERSION_V1
//-------------------------------------------------------------------------------------//
enum IPS_STATAUS_ID
{
	IPS_STATAUS_NONE                        =  0,

	IPS_STATAUS_SETUP                       = 100,
	
	IPS_STATAUS_INSPECTION_START            = 200,//檢測開始//Start
	IPS_STATAUS_INSPECTION_END              = 300,//檢測結束//End

	IPS_STATAUS_STATUS_MODE                 = 400,//狀態模式//Status
	IPS_STATAUS_STATUS_READY_TO_LOAD        = 401,//狀態-準備進板//Status_ReadyToLoad
	IPS_STATAUS_STATUS_LOAD_COMPLETE        = 402,//狀態-進板完成//Status_LoadComplete	
	IPS_STATAUS_STATUS_READY_TO_UNLOAD      = 403,//狀態-準備出板//Status_ReadyToUnload	
	IPS_STATAUS_STATUS_UNLOAD_COMPLETE      = 404,//狀態-進板完成//Status_UnloadComplete		
	IPS_STATAUS_STATUS_INSPECTION_STOP      = 405,//狀態-進板完成//Status_InspectionStop

	IPS_STATAUS_STATUS_OFFLINE              = 406,//狀態-進板完成//OffLine
	IPS_STATAUS_STATUS_ONLINE               = 407,//狀態-進板完成//OnLine

	IPS_STATAUS_AOI_CHECK_BARCODE           = 408,//狀態-確認條碼

	IPS_STATAUS_STATUS_ALARM                = 480,//狀態-警報//Alarm

	IPS_STATAUS_REMOTE_CONTROL_START        = 481,//遠端控制-開始檢測
	IPS_STATAUS_REMOTE_CONTROL_STOP         = 482,//遠端控制-停止檢測
	IPS_STATAUS_REMOTE_CONTROL_PROGRAM_LOAD = 483,//遠端控制-載入專案
	IPS_STATAUS_REMOTE_CONTROL_PROGRAM_LIST = 484,//遠端控制-條列專案
	IPS_STATAUS_REMOTE_CONTROL_PAUSE        = 485,//遠端控制-暫停檢測
	IPS_STATAUS_REMOTE_CONTROL_RESUME       = 486,//遠端控制-繼續檢測	
	IPS_STATAUS_REMOTE_CONTROL_ABORT        = 487,//遠端控制-中斷檢測
	IPS_STATAUS_REMOTE_CONTROL_BYPASS       = 489,//遠端控制-開始流片-非標準

	IPS_STATAUS_JET8000_END                 = 500,//軟體結束//ProgramEnd
	IPS_STATAUS_JET8000_START               = 600,//軟體結束//ProgramStart

	IPS_STATAUS_MES_SET_CMD                 = 901,//預留
	IPS_STATAUS_MES_GET_CMD                 = 902,//預留
	IPS_STATAUS_AOI_LOGIN_OUT_CMD           = 903,//預留	

	IPS_STATAUS_RETURN
};
//-------------------------------------------------------------------------------------//
typedef struct tagIPSCommNode//IPS Communication Node
{	
	int          nStatus;      //狀態碼
	int          nPPID;        //資料唯一碼
	int          nAck;         //是否等待回傳
	int          nTimeout;     //逾時時間(ms)
	int          nErrorCode;   //錯誤碼
	std::wstring wsErrorCode;  //錯誤內容
	int          nChkCount;    //AOI確認次數-超過就不處理	
	
	std::wstring wsVersionStr; //"Version_s": "1.0.0",
	std::wstring wsSenderStr;  //"Sender_s": "JET8000",
	int          nStatusCode;  //"StatusCode_i": 100,	
	std::wstring wsFilename;   //檔名

	std::string  sRawData;     //原始資料 

	tagIPSCommNode()
	{	
		nStatus = 0;
		nPPID = 0;
		nAck = 0;
		nTimeout = 0;
		nErrorCode = 0;
		nChkCount = 0;
		wsErrorCode.clear();		

		wsVersionStr.clear();
		wsSenderStr.clear();
		nStatusCode  = 0;
		sRawData.clear();
	}
} TIPSCommNode, *PIPSCommNode;
//-------------------------------------------------------------------------------//
#endif//_MES_IPS_DEFINE_H_