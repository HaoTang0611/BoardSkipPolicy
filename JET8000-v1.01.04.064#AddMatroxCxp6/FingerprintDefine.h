#pragma once

//#define WINBIO_DISABLE

typedef enum {
	FPS_DEVICE_NONE,
	FPS_DEVICE_AS608,
	FPS_DEVICE_PQIFPS_READER,
	FPS_DEVICE_RETURN,
} FPS_DEVICE;
//---------------------------------------------------------------------------------//
//AS608回傳訊息，只有少部分的狀態是封包回傳值，部分內容可以借用
typedef enum
{
	AS608_STATUS_OK = 0x00,						/**< ok */
	AS608_STATUS_FRAME_ERROR = 0x01,			/**< frame error */
	AS608_STATUS_NO_FINGERPRINT = 0x02,			/**< no fingerprint */
	AS608_STATUS_INPUT_ERROR = 0x03,			/**< fingerprint image error */
	AS608_STATUS_IMAGE_TOO_DRY = 0x04,			/**< fingerprint image too dry */
	AS608_STATUS_IMAGE_TOO_WET = 0x05,			/**< fingerprint image too wet */
	AS608_STATUS_IMAGE_TOO_CLUTTER = 0x06,      /**< fingerprint too clutter */
	AS608_STATUS_IMAGE_TOO_FEW_FEATURE = 0x07,  /**< fingerprint feature too few */
	AS608_STATUS_NOT_MATCH = 0x08,				/**< not match */
	AS608_STATUS_NOT_FOUND = 0x09,				/**< not found */
	AS608_STATUS_FEATURE_COMBINE_ERROR = 0x0A,  /**< feature combine error */
	AS608_STATUS_LIB_ADDR_OVER = 0x0B,			/**< fingerprint lib addr is over */
	AS608_STATUS_LIB_READ_ERROR = 0x0C,			/**< fingerprint lib read error */
	AS608_STATUS_UPLOAD_FEATURE_ERROR = 0x0D,   /**< upload feature error */
	AS608_STATUS_NO_FRAME = 0x0E,				/**< no frame */
	AS608_STATUS_UPLOAD_IMAGE_ERROR = 0x0F,     /**< upload image error */
	AS608_STATUS_LIB_DELETE_ERROR = 0x10,       /**< delete lib error */
	AS608_STATUS_LIB_CLEAR_ERROR = 0x11,        /**< clear lib error */
	AS608_STATUS_ENTER_LOW_POWER_ERROR = 0x12,  /**< enter low power error */
	AS608_STATUS_COMMAND_INVALID = 0x13,        /**< command invalid */
	AS608_STATUS_RESET_ERROR = 0x14,			/**< reset error */
	AS608_STATUS_BUFFER_INVALID = 0x15,			/**< buffer invalid */
	AS608_STATUS_UPDATE_ERROR = 0x16,			/**< update error */
	AS608_STATUS_NO_MOVE = 0x17,				/**< no move */
	AS608_STATUS_FLASH_ERROR = 0x18,			/**< flash error */
	AS608_STATUS_F0_RESPONSE = 0xF0,			/**< f0 response */
	AS608_STATUS_F1_RESPONSE = 0xF1,			/**< f1 response */
	AS608_STATUS_FLASH_WRITE_SUM_ERROR = 0xF2,  /**< flash sum error */
	AS608_STATUS_FLASH_WRITE_HEADER_ERROR = 0xF3,	/**< flash header error */
	AS608_STATUS_FLASH_WRITE_LENGTH_ERROR = 0xF4,   /**< flash length error */
	AS608_STATUS_FLASH_WRITE_LENGTH_TOO_LONG = 0xF5,/**< flash length too long */
	AS608_STATUS_FLASH_WRITE_ERROR = 0xF6,			/**< flash write error */
	AS608_STATUS_UNKNOWN = 0x19,				/**< unknown */
	AS608_STATUS_REG_INVALID = 0x1A,			/**< reg invalid */
	AS608_STATUS_DATA_INVALID = 0x1B,			/**< data invalid */
	AS608_STATUS_NOTE_PAGE_INVALID = 0x1C,      /**< note page invalid */
	AS608_STATUS_PORT_INVALID = 0x1D,			/**< port invalid */
	AS608_STATUS_ENROOL_ERROR = 0x1E,			/**< enrool error */
	AS608_STATUS_LIB_FULL = 0x1F,				/**< lib full */
} AS608_STATUS;	
//---------------------------------------------------------------------------------//
//AS608Buffer地址
typedef enum
{
	AS608_BUFFER_NUMBER_1 = 0x01,        /**< buffer 1 */
	AS608_BUFFER_NUMBER_2 = 0x02,        /**< buffer 2 */
} AS608_BUFFER_NUMBER;	
//---------------------------------------------------------------------------------//
//AS608命令
typedef enum {
	AS608_COMMAND_GETIMAGE = 0x01,
	AS608_COMMAND_GENCHAR,
	AS608_COMMAND_MATCH,
	AS608_COMMAND_SEARCH,
	AS608_COMMAND_REGMODEL,
	AS608_COMMAND_STORECHAR,
	AS608_COMMAND_LOADCHAR,
	AS608_COMMAND_UPCHAR,
	AS608_COMMAND_DOWNCHAR,
	AS608_COMMAND_UPIMAGE,
	AS608_COMMAND_DOWNIMAGE,
	AS608_COMMAND_DELETCHAR,
	AS608_COMMAND_EMPTY,
	AS608_COMMAND_WRITEREG,
	AS608_COMMAND_READSYSPARA,
	AS608_COMMAND_ENROLL,
	AS608_COMMAND_IDENTIFY,
	AS608_COMMAND_SETPWD,
	AS608_COMMAND_VFYPWD,
	AS608_COMMAND_GETRANDOMCODE,
	AS608_COMMAND_SETCHIPADDR,
	AS608_COMMAND_READINFPAGE,
	AS608_COMMAND_PORT_CONTROL,
	AS608_COMMAND_WRITENOTEPAD,
	AS608_COMMAND_READNOTEPAD,
	AS608_COMMAND_BURNCODE,
	AS608_COMMAND_HIGHSPEEDSEARCH,
	AS608_COMMAND_GENBINIMAGE,
	AS608_COMMAND_VALIDTEMPLETENUM,
	AS608_COMMAND_RETURN
} AS608_COMMAND;		
//---------------------------------------------------------------------------------//
//AS608封包類型
typedef enum {
	AS608_FLAG_COMMAND = 1,	//輸出封包
	AS608_FLAG_DATA = 2,	//資料封包
	AS608_FLAG_RESPOND = 7,	//接收封包
	AS608_FLAG_DATA_END = 8,//結束封包
} AS608_FLAG;			
//---------------------------------------------------------------------------------//
//以下是自定義類
//---------------------------------------------------------------------------------//
//使用者啟用的指紋模式
typedef enum {
	USER_FINGERPRINT_MODE_UNABLE = 0,
	USER_FINGERPRINT_MODE_MATCH,//指紋驗證
	USER_FINGERPRINT_MODE_ENROLL,//指紋註冊
	USER_FINGERPRINT_MODE_FIND,//指紋尋找使用者
	USER_FINGERPRINT_MODE_RETURN
} USER_FINGERPRINT_MODE;
//---------------------------------------------------------------------------------//
//嚴格註冊用的狀態機
typedef enum {
	FINGERPRINT_RECORD_PROCESS_RECORD,
	FINGERPRINT_RECORD_PROCESS_GENCHAR,
	FINGERPRINT_RECORD_PROCESS_MOVEOUT,
	FINGERPRINT_RECORD_PROCESS_MERGE,
	FINGERPRINT_RECORD_PROCESS_UPCHAR
} FINGERPRINT_RECORD_PROCESS; 
//---------------------------------------------------------------------------------//
