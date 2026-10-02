#ifndef _MES_IPS_DEFINE_H_
#define _MES_IPS_DEFINE_H_
//-------------------------------------------------------------------------------------//
//#define IPS_DEBUG
#define IPS_VERSION_V2
//-------------------------------------------------------------------------------------//
enum IPS_COMM_ID//與IPS通訊編號
{
	IPS_COMM_NONE                            =   0,

	IPS_COMM_GEN                             = 100,//一般使用
	IPS_COMM_GEN_SYSTEM_SETUP                = 101,//參數設定-開啟專案, 切換專案, 內容變更
	IPS_COMM_GEN_INSPECTION_START            = 102,//開始檢測, 設定目前專案
	IPS_COMM_GEN_CUSTOMER_SETTING            = 104,//客戶特殊參數設定
	IPS_COMM_GEN_CHECK_BARCODE               = 105,//確認條碼, 在取得條碼後送出
	IPS_COMM_GEN_PROJECT_UPLOAD              = 106,//告知IPS專案上傳
	IPS_COMM_GEN_PROJECT_DOWNLOAD            = 107,//告知IPS專案下載
	IPS_COMM_GEN_USER_DATA                   = 108,//使用者登入登出
	IPS_COMM_GEN_GET_STATUS                  = 109,//取得狀態

	IPS_COMM_ASK                             = 200,//詢問使用
	IPS_COMM_ASK_CUSTOMER_ASK                = 204,
	IPS_COMM_ASK_BARCODE                     = 205,//詢問條碼
	IPS_COMM_ASK_CHECK_CONTINUE              = 206,
	IPS_COMM_ASK_XBOARD_MAPPING              = 207,//報廢板映射
	IPS_COMM_ASK_UNCHECK_TEST_FILE           = 208,//未判定檢測檔案數

	IPS_COMM_STATUS                          = 400,//狀態使用
	IPS_COMM_STATUS_READY_TO_LOAD            = 401,
	IPS_COMM_STATUS_LOAD_COMPLETE            = 402,
	IPS_COMM_STATUS_READY_TO_UNLOAD          = 403,
	IPS_COMM_STATUS_UNLOAD_COMPLETE          = 404,
	IPS_COMM_STATUS_INSPECTION_STOP          = 405,//檢測停止
	IPS_COMM_STATUS_OFFLINE                  = 406,
	IPS_COMM_STATUS_ONLINE                   = 407,
	IPS_COMM_STATUS_PARAM_CHANGE             = 408,
	IPS_COMM_STATUS_PROJECT_LOAD_FINISH      = 409,
	IPS_COMM_STATUS_ALARM_CLEAR              = 410,
	IPS_COMM_STATUS_INSPECTION_END           = 411,//檢測結束
	IPS_COMM_STATUS_IDLE			         = 412,//機台待機中-20240528
	IPS_COMM_STATUS_LOCAL_ONLINE             = 413,//本地上線
	IPS_COMM_STATUS_STOP                     = 414,//停止-暫定?IPS_COMM_STATUS_INSPECTION_STOP
	IPS_COMM_STATUS_EDIT                     = 415,//編程模式-暫定
	IPS_COMM_STATUS_OPEN_PROJECT             = 416,//開啟專案	
	IPS_COMM_STATUS_TOWER_LIGHT              = 417,//塔燈切換-JET7000
	IPS_COMM_STATUS_ONLINE_TEST              = 418,//線上檢測模式

	IPS_COMM_ALARM                           = 500,//警報使用

	IPS_COMM_REMOTE                          = 600,//遠端控制
	IPS_COMM_REMOTE_ON                       = 601,
	IPS_COMM_REMOTE_OFF                      = 602,
	IPS_COMM_REMOTE_START                    = 603,
	IPS_COMM_REMOTE_STOP                     = 604,
	IPS_COMM_REMOTE_PAUSE                    = 605,
	IPS_COMM_REMOTE_RESUME                   = 606,
	IPS_COMM_REMOTE_ABORT                    = 607,

	IPS_COMM_REMOTE_PROJECT_LOAD             = 610,
	IPS_COMM_REMOTE_PROJECT_LIST             = 611,
	IPS_COMM_REMOTE_PROJECT_UPLOAD           = 612,
	IPS_COMM_REMOTE_UPLOAD_FINISHED          = 613,
	IPS_COMM_REMOTE_PROJECT_DOWNLOAD         = 614,
	IPS_COMM_REMOTE_DOWNLOAD_FINISHED        = 615,
	IPS_COMM_REMOTE_PROJECT_DELETE           = 616,
	IPS_COMM_REMOTE_DELETE_FINISHED          = 617,

	IPS_COMM_REMOTE_PARAM_UPLOAD             = 618,
	IPS_COMM_REMOTE_PARAM_UPLOAD_FINISHED    = 619,
	IPS_COMM_REMOTE_PARAM_DOWNLOAD           = 620,
	IPS_COMM_REMOTE_PARAM_DOWNLOAD_FINISHED  = 621,

	IPS_COMM_REMOTE_SET_LOT_NUMBER           = 623,

	IPS_COMM_REMOTE_BYPASS                   = 699,

	IPS_COMM_PCS_CONTROL                     = 700,//PCS發送
	IPS_COMM_PCS_SHOW_MESSAGE                = 701,//顯示訊息	
	IPS_COMM_PCS_UPLOAD_FINISHED             = 702,//上傳結束
	IPS_COMM_PCS_DOWNLOAD_FINISHED           = 703,//下載結束
	
	IPS_COMM_JET8000_END                     = 9900,//軟體結束//ProgramEnd
	IPS_COMM_JET8000_START                   = 9990,//軟體開始//ProgramStart
	IPS_COMM_RETURN
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
	
	std::wstring wsVersionStr; //"Version": "2.0.0",
	std::wstring wsSenderStr;  //"Sender": "JET8000",
	int          nStatusCode;  //"StatusCode": 100,	
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