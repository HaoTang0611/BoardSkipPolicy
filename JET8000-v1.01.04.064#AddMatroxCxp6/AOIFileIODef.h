#ifndef _AOIFileIODef_H_
#define _AOIFileIODef_H_
//-------------------------------------------------------------------------------------//
enum FILE_MODE
{
	FILE_MODE_TXT       =1,
	FILE_MODE_BINARY    =2,
	FILE_MODE_UNICODE   =3,
	FILE_MODE_DUMMY
};
//-------------------------------------------------------------------------------------//
enum FILE_IO_MODE
{
	FILE_IO_MODE_NONE = 0,
	FILE_IO_MODE_SAVE = 1,
	FILE_IO_MODE_LOAD = 2
};
//-------------------------------------------------------------------------------------//
enum FILE_TARGET
{
	FILE_TARGET_PROJECT        = 1,
	FILE_TARGET_LIBRARAY       = 2,
	FILE_TARGET_OFFLINE        = 3,	
	FILE_TARGET_DEFAULT_MODEL  = 4,
	FILE_TARGET_OTHERS         = 9,
	FILE_TARGET_RETURN
};
//-------------------------------------------------------------------------------------//
enum FILE_READ_MODE//郎弄家Α
{
	FILE_READ_HARD_DISK   = 1,//祑盒弄
	FILE_READ_MEMORY      = 2,//癘拘砰弄
	FILE_READ_RETURN
};
//-------------------------------------------------------------------------------------//
enum FILE_WRITE_MODE//郎糶家Α
{
	FILE_WRITE_HARD_DISK   = 1,//祑盒糶(╰参IO)
	FILE_WRITE_MEMORY      = 2,//癘拘砰弄
	FILE_WRITE_RETURN
};
//-------------------------------------------------------------------------------------//
enum CHUNK_TYPE
{
	CHUNK_TYPE_SHORT    =   1,//32bit, 2byte
	CHUNK_TYPE_INT      =   2,//32bit, 4byte	
	CHUNK_TYPE_INT_64   =   4,//64bit, 8byte	

	CHUNK_TYPE_FLT      =   5,//32bit, 4byt
	CHUNK_TYPE_DBL      =   6,//32bit, 8byt

	CHUNK_TYPE_STR_016  =  16,
	CHUNK_TYPE_WSTR_016 =  17,

	CHUNK_TYPE_STR_032  =  32,
	CHUNK_TYPE_WSTR_032  = 33,

	CHUNK_TYPE_STR_064  =  64,
	CHUNK_TYPE_WSTR_064 =  65,

	CHUNK_TYPE_STR_128  = 128,
	CHUNK_TYPE_WSTR_128 = 129,

	CHUNK_TYPE_STR_256  = 256,
	CHUNK_TYPE_WSTR_256 = 257,

	CHUNK_TYPE_STR_512  = 512,
	CHUNK_TYPE_WSTR_512 = 513,

	CHUNK_DUMMY
};
typedef struct _CHUNK_INT
{
	int   index;//戈ま腹
	int   value;
} TCHUNK_INT, *PCHUNK_INT;

typedef struct _CHUNK_INT_64
{
	int     index;//戈ま腹
	__int64 value;
} TCHUNK_INT_64, *PCHUNK_INT_64;

typedef struct _CHUNK_DBL
{
	int     index;//戈ま腹
	double  value;
} TCHUNK_DBL, *PCHUNK_DBL;

typedef struct _CHUNK_STR_016
{
	int     index;//戈ま腹
	char    value[16];
} TCHUNK_STR_016, *PCHUNK_STR_016;

typedef struct _CHUNK_STR_032
{
	int     index;//戈ま腹
	char    value[32];
} TCHUNK_STR_032, *PCHUNK_STR_032;

typedef struct _CHUNK_STR_064
{
	int     index;//戈ま腹
	char    value[64];
} TCHUNK_STR_064, *PCHUNK_STR_064;

typedef struct _CHUNK_STR_128
{
	int     index;//戈ま腹
	char    value[128];
} TCHUNK_STR_128, *PCHUNK_STR_128;

typedef struct _CHUNK_STR_256
{
	int     index;//戈ま腹
	char    value[256];
} TCHUNK_STR_256, *PCHUNK_STR_256;

typedef struct _CHUNK_STR_512
{
	int     index;//戈ま腹
	char    value[512];
} TCHUNK_STR_512, *PCHUNK_STR_512;

typedef struct _CHUNK_WSTR_016
{
	int     index;//戈ま腹
	wchar_t value[16];
} TCHUNK_WSTR_016, *PCHUNK_WSTR_016;

typedef struct _CHUNK_WSTR_032
{
	int     index;//戈ま腹
	wchar_t value[32];
} TCHUNK_WSTR_032, *PCHUNK_WSTR_032;

typedef struct _CHUNK_WSTR_064
{
	int     index;//戈ま腹
	wchar_t value[64];
} TCHUNK_WSTR_064, *PCHUNK_WSTR_064;

typedef struct _CHUNK_WSTR_128
{
	int     index;//戈ま腹
	wchar_t value[128];
} TCHUNK_WSTR_128, *PCHUNK_WSTR_128;

typedef struct _CHUNK_WSTR_256
{
	int     index;//戈ま腹
	wchar_t value[256];
} TCHUNK_WSTR_256, *PCHUNK_WSTR_256;

typedef struct _CHUNK_WSTR_512
{
	int     index;//戈ま腹
	wchar_t value[512];
} TCHUNK_WSTR_512, *PCHUNK_WSTR_512;
//-------------------------------------------------------------------------------------//
typedef int FILE_IO_ID;
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_START                                =         0;//0
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_SYSTEM_SECTION_START                 =         1;//╰参把计跋丁-癬翴

const FILE_IO_ID FILE_IO_SYSTEM_MACHINE_LOCATION              =         2;//╰参把计跋丁-セ诀紅跋
const FILE_IO_ID FILE_IO_SYSTEM_MACHINE_BUILDING              =         3;//╰参把计跋丁-セ诀瓷
const FILE_IO_ID FILE_IO_SYSTEM_MACHINE_FLOOR                 =         4;//╰参把计跋丁-セ诀加糷
const FILE_IO_ID FILE_IO_SYSTEM_MACHINE_ROOM                  =         5;//╰参把计跋丁-セ诀ó丁
const FILE_IO_ID FILE_IO_SYSTEM_MACHINE_LINE                  =         6;//╰参把计跋丁-セ诀絬
const FILE_IO_ID FILE_IO_SYSTEM_MACHINE_STATION               =         7;//╰参把计跋丁-セ诀絬
const FILE_IO_ID FILE_IO_SYSTEM_MACHINE_SERIES_NUMBER         =         8;//╰参把计跋丁-セ诀腹
const FILE_IO_ID FILE_IO_SYSTEM_MACHINE_NAME                  =         9;//╰参把计跋丁-诀腹
const FILE_IO_ID FILE_IO_SYSTEM_MACHINE_VENDOR                =        10;//╰参把计跋丁-诀紅坝
const FILE_IO_ID FILE_IO_SYSTEM_MACHINE_ALIAS                 =        11;//╰参把计跋丁-诀

const FILE_IO_ID FILE_IO_SYSTEM_COLOR_GROUP_SET_NODE          =       801;//╰参把计跋丁-┾︹竤钉
const FILE_IO_ID FILE_IO_SYSTEM_SECTION_END                   =       999;//╰参把计跋丁-沧翴
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_PROJECT_SECTION_START                =      1001;//盡把计跋丁-癬翴
const FILE_IO_ID FILE_IO_PROJECT_SECTION_END                  =      1999;//盡把计跋丁-沧翴
const FILE_IO_ID FILE_IO_PROJECT_OBJ_UUID                     =      1002;//盡把计-Obj-UUID
const FILE_IO_ID FILE_IO_PROJECT_MULTI_DISTRICT_MODE          =      1003;//盡把计-跋琿家Α
const FILE_IO_ID FILE_IO_PROJECT_MULTI_SYSTEM_ID              =      1004;//盡把计-╰参絪腹

const FILE_IO_ID FILE_IO_PROJECT_FRAME_UNIQUE_ID_BEGIN        =      1091;//盡把计-紇钩斑絏-秨﹍
const FILE_IO_ID FILE_IO_PROJECT_FRAME_UNIQUE_ID_VALUE        =      1092;//盡把计-紇钩斑絏-计
const FILE_IO_ID FILE_IO_PROJECT_FRAME_UNIQUE_ID_END          =      1093;//盡把计-紇钩斑絏-挡

//Project Basic Data 1002 ~ 1099
const FILE_IO_ID FILE_IO_PROJECT_BASIC_BARCODE_DEVICE_INDEX   =      1010;//盡把计-兵絏诀腹
const FILE_IO_ID FILE_IO_PROJECT_BASIC_BARCODE_CODE_INDEX     =      1011;//盡把计-兵絏材碭絏
const FILE_IO_ID FILE_IO_PROJECT_BASIC_BARCODE_RESULT         =      1012;//盡把计-兵絏挡狦

//Project Param Info 1100 ~ 1299
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SECTION_START          =      1101;//盡把计-盡把计癬翴
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SECTION_END            =      1102;//盡把计-盡把计沧翴
const FILE_IO_ID FILE_IO_PROJECT_PARAM_PROJECT_MAP_GAIN       =      1103;//盡把计-盡┏瓜糤痲
const FILE_IO_ID FILE_IO_PROJECT_PARAM_FOCUS_OFFSET           =      1104;//盡把计-盡礘翴熬畉
const FILE_IO_ID FILE_IO_PROJECT_PARAM_PCB_OUT_MODE           =      1105;//盡把计-盡狾家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_FD_NG_HANDLE_MODE      =      1106;//盡把计-﹚翴钵盽矪瞶家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DEFECT_HANDLE_MODE     =      1107;//盡把计-浪钵盽矪瞶家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_ENABLE_CONVEYER_PRE_RUN=      1108;//盡把计-瓂笵矗玡笲锣
const FILE_IO_ID FILE_IO_PROJECT_PARAM_FD_GRAB_MODE           =      1109;//盡把计-﹚翴钩家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_INSPECTION_FIELD_BUILD_MODE=  1110;//盡把计-浪代跋办皌竚家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_LANE_WIDTH             =      1111;//盡把计-瓂笵糴
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SAVE_TEST_MAP          =      1112;//盡把计-纗浪代┏瓜
const FILE_IO_ID FILE_IO_PROJECT_PARAM_TEST_MAP_SCALE_MODE    =      1113;//盡把计-浪代┏瓜罽ゑㄒ
const FILE_IO_ID FILE_IO_PROJECT_PARAM_XBOARD_CHECK_RATIO     =      1114;//盡把计-盡X狾絋粄ゑㄒ

//const FILE_IO_ID FILE_IO_PROJECT_PARAM_DEFECT_TEST_MODE       =      1115;//盡把计-峰搏浪代家Α
//const FILE_IO_ID FILE_IO_PROJECT_PARAM_DEFECT_ALARM_MODE      =      1116;//盡把计-峰搏牡厨家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DLP_LED_COLOR          =      1117;//盡把计-DLP LED肅︹家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_FIELD_WIDTH_SIZE_MODE  =      1118;//盡把计-跋办糴へ家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_FIELD_HEIGHT_SIZE_MODE =      1119;//盡把计-跋办へ家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_ONLINE_TUNING_MODE     =      1120;//盡把计-絬秸诀家Α

const FILE_IO_ID FILE_IO_PROJECT_PARAM_MODULE_NAME            =      1121;//盡把计-盡诀贺嘿
const FILE_IO_ID FILE_IO_PROJECT_PARAM_VERSION                =      1122;//盡把计-盡セ
const FILE_IO_ID FILE_IO_PROJECT_PARAM_PANEL_SIDE             =      1123;//盡把计-盡玻珇タ璉
//const FILE_IO_ID FILE_IO_PROJECT_PARAM_WORK_ORDER           =      1124;//盡把计-盡虫絪腹
const FILE_IO_ID FILE_IO_PROJECT_PARAM_WORK_NUMBER            =      1125;//盡把计-盡虫腹絏
const FILE_IO_ID FILE_IO_PROJECT_PARAM_OPEN_CODE              =      1126;//盡把计-盡虫腹絏
const FILE_IO_ID FILE_IO_PROJECT_PARAM_TEST_SIZE_WIDTH        =      1127;//盡把计-盡へ糴
const FILE_IO_ID FILE_IO_PROJECT_PARAM_TEST_SIZE_HEIGHT       =      1128;//盡把计-盡へ蔼

const FILE_IO_ID FILE_IO_PROJECT_PARAM_SAVE_TEST_MAP_TO_REPAIR=      1129;//盡把计-纗浪代┏瓜蝴
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SAVE_TEST_MAP_01       =      1130;//盡把计-纗浪代┏瓜-01
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SAVE_TEST_MAP_02       =      1131;//盡把计-纗浪代┏瓜-02
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SAVE_TEST_MAP_03       =      1132;//盡把计-纗浪代┏瓜-03
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SAVE_TEST_MAP_04       =      1133;//盡把计-纗浪代┏瓜-04
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SAVE_TEST_MAP_05       =      1134;//盡把计-纗浪代┏瓜-05
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SAVE_TEST_MAP_06       =      1135;//盡把计-纗浪代┏瓜-06
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SAVE_TEST_MAP_07       =      1136;//盡把计-纗浪代┏瓜-07
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SAVE_TEST_MAP_08       =      1137;//盡把计-纗浪代┏瓜-08

const FILE_IO_ID FILE_IO_PROJECT_PARAM_STATISTIC_BY_MODE      =      1140;//盡把计-参璸ㄌ沮家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_STATISTIC_BY_MODE_VALUE=      1141;//盡把计-参璸ㄌ沮丁计秖
const FILE_IO_ID FILE_IO_PROJECT_PARAM_STATISTIC_BY_COUNT_VALUE=     1142;//盡把计-参璸ㄌ沮Ω计计秖
const FILE_IO_ID FILE_IO_PROJECT_PARAM_STATISTIC_DEFECT_FROM_MODE=   1143;//盡把计-参璸峰搏ㄓ方

const FILE_IO_ID FILE_IO_PROJECT_PARAM_ALARM_TEST_YIELD_MIN   =      1150;//盡把计-浪代▆瞯-%
const FILE_IO_ID FILE_IO_PROJECT_PARAM_ALARM_PANEL_YIELD_MIN  =      1151;//盡把计-俱狾▆瞯-%
const FILE_IO_ID FILE_IO_PROJECT_PARAM_ALARM_BOARD_YIELD_MIN  =      1152;//盡把计-虫狾▆瞯-%
const FILE_IO_ID FILE_IO_PROJECT_PARAM_ALARM_PART_YIELD_MIN   =      1153;//盡把计-箂ン▆瞯-%
const FILE_IO_ID FILE_IO_PROJECT_PARAM_ALARM_PART_NG_RATE_MAX =      1154;//盡把计-箂ン虫Ω峰搏瞯-%
const FILE_IO_ID FILE_IO_PROJECT_PARAM_ALARM_EACH_PART_NG_COUNT=     1155;//盡把计-–箂ン峰搏计
const FILE_IO_ID FILE_IO_PROJECT_PARAM_ALARM_EACH_PART_CONTINUE_NG_COUNT=1156;//盡把计-–箂ン硈尿峰搏Ω计
const FILE_IO_ID FILE_IO_PROJECT_PARAM_ALARM_LOCK_MODE        =      1157;//盡把计-牡厨玛家Α

const FILE_IO_ID FILE_IO_PROJECT_PARAM_FIELD_PATH_MODE        =      1160;//盡把计-跋办隔畖家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_FIELD_SECTION_FACTOR   =      1161;//盡把计-跋办だ跋玒计
const FILE_IO_ID FILE_IO_PROJECT_PARAM_FIELD_DIVISION_MODE    =      1162;//盡把计-跋办だ澄家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_FIELD_DIVISION_BOARD_FD_FIRST=1163;//盡把计-跋办だ澄虫狾﹚翴纔
const FILE_IO_ID FILE_IO_PROJECT_PARAM_INSPECTION_FIELD_AREA_MODE=   1169;//盡把计-浪代跋办皌竚縩家Α

const FILE_IO_ID FILE_IO_PROJECT_PARAM_LINK_SERVER_MODE       =      1170;//盡把计-盡硈絬狝竟家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SERVER_LIBRARY_GROUP   =      1171;//盡把计-狝竟戈畐竤舱

const FILE_IO_ID FILE_IO_PROJECT_PARAM_XBOARD_MAPPING_FILE_MODE=     1181;//盡把计-厨紀狾琈甮郎家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_XBOARD_MAPPING_FILE_FLOW=     1182;//盡把计-厨紀狾琈甮郎诀
const FILE_IO_ID FILE_IO_PROJECT_PARAM_XBOARD_MAPPING_FILE_MES_CHECK=1183;//盡把计-厨紀狾MES Comm兵ン浪代


const FILE_IO_ID FILE_IO_PROJECT_PARAM_DEFECT_ITEM_ENABLE     =      1197;//盡把计-盡峰搏兜ヘ币ノ
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DEFECT_ITEM_TEST       =      1198;//盡把计-盡峰搏兜ヘ浪代
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DEFECT_ITEM_ALARM      =      1199;//盡把计-盡峰搏兜ヘ牡厨

const FILE_IO_ID FILE_IO_PROJECT_PARAM_BARCODE_INPUT_TYPE     =      1201;//盡把计-兵絏块妓Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BARCODE_NG_HANDLE_MODE =      1202;//盡把计-兵絏ア毖矪瞶家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BARCODE_CAMERA_GRAB_MODE=     1203;//盡把计-诀兵絏弄家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BARCODE_DEVICE_GRAB_MODE=     1204;//盡把计-兵絏诀弄家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BARCODE_INPUT_CAMERA_ENABLE=  1205;//盡把计-诀块家Α币ノ
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BARCODE_HANDHELD_READ_MODE =  1206;//盡把计-も兵絏诀弄家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BARCODE_CAMERA_SAVE_IMAGE=    1207;//盡把计-诀纗瓜郎币ノ
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BARCODE_INPUT_FILE_ENABLE=    1208;//盡把计-郎块家Α币ノ
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BARCODE_INPUT_FILE_DELAY_TIME=1209;//盡把计-郎块┑筐丁
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BARCODE_END_REMOVE_CHAR_CNT  =1210;//盡把计-兵絏狠簿埃じ计
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BARCODE_BEGIN_REMOVE_CHAR_CNT=1211;//盡把计-兵絏玡狠簿埃じ计
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BARCODE_AUTO_EXPAND_MODE_PANEL=1212;//盡把计-兵絏笆耎甶家Α-俱狾
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BARCODE_AUTO_EXPAND_MODE_BOARD=1213;//盡把计-兵絏笆耎甶家Α-虫狾
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BARCODE_VERIFY_MODE           =1215;//盡把计-兵絏喷靡家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BARCODE_RETRIEVE_MODE         =1216;//盡把计-兵絏家Α

const FILE_IO_ID FILE_IO_PROJECT_PARAM_VERSION_CODE_ACT_INDEX =      1220;//盡把计-セ腹币ノま计

const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_ENABLE       =      1221;//盡把计-┻ン浪代币ノ
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_DARK_LEVEL   =      1222;//盡把计-┻ン浪代穞场﹚竡
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_TOLERANCE    =      1223;//盡把计-┻ン浪代そ畉
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_FRAME_UNIQUE_ID=    1224;//盡把计-┻ン浪代紇钩斑絏
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_FILTER_OPEN_SIZE=   1225;//盡把计-┻ン浪代秨笲衡
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_CALC_SIZE_W  =      1226;//盡把计-┻ン浪代璸衡糴
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_CALC_SIZE_H  =      1227;//盡把计-┻ン浪代璸衡
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_GAUSSIAN_SIZE=      1228;//盡把计-┻ン浪代蔼吹筁耾
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_FILTER_CLOSE_SIZE=  1229;//盡把计-┻ン浪代超笲衡
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_MIN_SIZE_W   =      1230;//盡把计-┻ン浪代程糴
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_MIN_SIZE_H   =      1231;//盡把计-┻ン浪代程
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_LIGHT_LEVEL  =      1232;//盡把计-┻ン浪代筁獹﹚竡
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_MAX_SIZE_R   =      1233;//盡把计-┻ン浪代程ゑㄒ
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_EDGE_REMOVE  =      1234;//盡把计-┻ン浪代娩絬簿埃
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_DILATE_SIZE  =      1235;//盡把计-┻ン浪代勘等へ
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_SAVE_IMAGE   =      1236;//盡把计-┻ン浪代纗瓜
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_SMOOTH_SIZE  =      1237;//盡把计-┻ン浪代キ菲筁耾
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_MATCH_USE_SCALE =   1238;//盡把计-┻ン浪代ㄏノ罽で皌
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_XBOARD_EXCLUDED =   1239;//盡把计-┻ン浪代厨紀狾逼埃
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_3D_OVER_LOW  =      1241;//盡把计-┻ン浪代3D筁玞埃
const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_3D_OVER_HIGH =      1242;//盡把计-┻ン浪代3D筁蔼玞埃

const FILE_IO_ID FILE_IO_PROJECT_PARAM_DROP_PART_3D_NOISE_FILTER=    1249;//盡把计-┻ン浪代3D馒癟筁耾-膀非

const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_PART_ENABLE    =      1251;//盡把计-端浪代币ノ
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_PART_SAVE_IMAGE=      1252;//盡把计-端浪代瓜
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_PART_FRAME_UNIQUE_ID= 1253;//盡把计-端浪代紇钩斑絏
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_PART_MATCH_USE_SCALE= 1254;//盡把计-端浪代ㄏノ罽で皌
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_PART_CALC_SIZE_W=     1255;//盡把计-端浪代璸衡糴
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_PART_CALC_SIZE_H=     1256;//盡把计-端浪代璸衡
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_PART_COLOR_EXPAND=    1257;//盡把计-端浪代肅︹耎
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_FILTER_OPEN_SIZE=     1258;//盡把计-端浪代秨笲衡
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_FILTER_CLOSE_SIZE=    1259;//盡把计-端浪代超笲衡
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_PART_MIN_SIZE_W=      1260;//盡把计-端浪代程糴
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_PART_MIN_SIZE_H=      1261;//盡把计-端浪代程
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_PART_SMOOTH_SIZE=     1262;//盡把计-端浪代蔼吹キ菲筁耾
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_PART_EDGE_THRESHOLD=  1263;//盡把计-端浪代ヘ夹娩絫霩
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_PART_MIN_PIXELS=      1264;//盡把计-端浪代端縩
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_PART_MIN_SIZE_D=      1265;//盡把计-端浪代端程癸à禯瞒
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_PART_MIN_GRAYSCALE=   1266;//盡把计-端浪代η顶
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SCRATCH_PART_MAX_GRAYSCALE=   1267;//盡把计-端浪代η顶

const FILE_IO_ID FILE_IO_PROJECT_PARAM_STATISTIC_DEFECT_FROM_MODE_ARS=1300;//盡把计-参璸峰搏ㄓ方家Α_ARS
const FILE_IO_ID FILE_IO_PROJECT_PARAM_STATISTIC_BY_MODE_ARS      =  1301;//盡把计-参璸ㄌ沮家Α_ARS
const FILE_IO_ID FILE_IO_PROJECT_PARAM_STATISTIC_BY_MODE_VALUE_ARS=  1302;//盡把计-参璸ㄌ沮丁计秖_ARS
const FILE_IO_ID FILE_IO_PROJECT_PARAM_STATISTIC_BY_COUNT_VALUE_ARS= 1303;//盡把计-参璸ㄌ沮Ω计计秖_ARS

const FILE_IO_ID FILE_IO_PROJECT_PARAM_ALARM_TEST_YIELD_MIN_ARS   =  1310;//盡把计-浪代▆瞯-%_ARS
const FILE_IO_ID FILE_IO_PROJECT_PARAM_ALARM_PANEL_YIELD_MIN_ARS  =  1311;//盡把计-俱狾▆瞯-%_ARS
const FILE_IO_ID FILE_IO_PROJECT_PARAM_ALARM_BOARD_YIELD_MIN_ARS  =  1312;//盡把计-虫狾▆瞯-%_ARS
const FILE_IO_ID FILE_IO_PROJECT_PARAM_ALARM_PART_YIELD_MIN_ARS   =  1313;//盡把计-箂ン▆瞯-%_ARS
const FILE_IO_ID FILE_IO_PROJECT_PARAM_ALARM_PART_NG_RATE_MAX_ARS =  1314;//盡把计-箂ン虫Ω峰搏瞯-%_ARS
const FILE_IO_ID FILE_IO_PROJECT_PARAM_ALARM_EACH_PART_NG_COUNT_ARS= 1315;//盡把计-–箂ン峰搏计_ARS
const FILE_IO_ID FILE_IO_PROJECT_PARAM_ALARM_EACH_PART_CONTINUE_NG_COUNT_ARS=1316;//盡把计-–箂ン硈尿峰搏Ω计_ARS

const FILE_IO_ID FILE_IO_PROJECT_PARAM_SAVE_OFFLINE_IMAGE_SCOPE=     1390;//盡把计-纗瞒絬紇钩絛氓
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SAVE_STATIC_DATA        =     1391;//盡把计-纗参璸戈
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SAVE_COMPONENT_WND_LIST_MODE= 1392;//盡把计-纗箂ン浪代家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_SAVE_SPC_HEADER_JSON_BEFORE_INSPECTION = 1393;//盡把计-浪代玡纗JSON SPC繷郎

const FILE_IO_ID FILE_IO_PROJECT_PARAM_AI_SAVE_MODEL_IMAGE_MODE=     1400;//盡把计-AI-纗家舱紇钩家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_AI_SAVE_MODEL_IMAGE_ONOFF=    1401;//盡把计-AI-纗家舱紇钩秨闽
const FILE_IO_ID FILE_IO_PROJECT_PARAM_AI_SAVE_MODEL_IMAGE_3D_FILE=  1402;//盡把计-AI-纗家舱紇钩3D郎

const FILE_IO_ID FILE_IO_PROJECT_PARAM_PANEL_FD_NG_SKIP_COUNT  =     1410;//盡把计-俱狾﹚翴钵盽铬筁浪代计秖
const FILE_IO_ID FILE_IO_PROJECT_PARAM_PANEL_FD_NG_HANDLE_MODE =     1411;//盡把计-俱狾﹚翴钵盽矪瞶家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BOARD_FD_NG_SKIP_COUNT  =     1415;//盡把计-虫狾﹚翴钵盽铬筁浪代计秖
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BOARD_FD_NG_HANDLE_MODE =     1416;//盡把计-虫狾﹚翴钵盽矪瞶家Α
const FILE_IO_ID FILE_IO_PROJECT_PARAM_BOARD_FD_GRAB_MODE      =     1417;//盡把计-虫狾﹚翴钩家Α

const FILE_IO_ID FILE_IO_PROJECT_PARAM_GENERAL_STR_01         =      1501;//盡把计-硄ノ把计﹃-01
const FILE_IO_ID FILE_IO_PROJECT_PARAM_GENERAL_STR_02         =      1502;//盡把计-硄ノ把计﹃-02
const FILE_IO_ID FILE_IO_PROJECT_PARAM_GENERAL_STR_03         =      1503;//盡把计-硄ノ把计﹃-03
const FILE_IO_ID FILE_IO_PROJECT_PARAM_GENERAL_STR_04         =      1504;//盡把计-硄ノ把计﹃-04

const FILE_IO_ID FILE_IO_PROJECT_COLOR_GROUP_NODE             =      1801;//盡把计-盡︹眒竤舱
const FILE_IO_ID FILE_IO_PROJECT_VERSION_CODE_LIST_NODE       =      1802;//盡把计-セ腹
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_FOV_SECTION_START                    =      2001;//跌偿把计跋丁-癬翴
const FILE_IO_ID FILE_IO_FOV_SECTION_END                      =      2999;//跌偿把计跋丁-沧翴

const FILE_IO_ID FILE_IO_FOV_START                            =      2002;//跌偿把计-癬翴
const FILE_IO_ID FILE_IO_FOV_END                              =      2003;//跌偿把计-沧翴
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_PROGRAM_FIELD_SECTION_START          =      3001;//跋办把计跋丁-癬翴
const FILE_IO_ID FILE_IO_PROGRAM_FIELD_SECTION_END            =      3999;//跋办把计跋丁-沧翴

const FILE_IO_ID FILE_IO_INSPECTION_FIELD_SECTION_START       =      3002;//跋办把计跋丁-癬翴
const FILE_IO_ID FILE_IO_INSPECTION_FIELD_SECTION_END         =      3998;//跋办把计跋丁-沧翴

const FILE_IO_ID FILE_IO_FIELD_START                          =      3011;//跋办把计-癬翴
const FILE_IO_ID FILE_IO_FIELD_END                            =      3012;//跋办把计-沧翴
const FILE_IO_ID FILE_IO_FIELD_INDEX                          =      3013;//跋办把计-盡ま计-Debug
const FILE_IO_ID FILE_IO_FIELD_LIST_MODE                      =      3014;//跋办把计-盡家Α
const FILE_IO_ID FILE_IO_FIELD_PANEL_INDEX                    =      3015;//跋办把计-俱狾ま计
const FILE_IO_ID FILE_IO_FIELD_CAD_POS_X                      =      3016;//跋办把计-Cad畒夹-X-um
const FILE_IO_ID FILE_IO_FIELD_CAD_POS_Y                      =      3017;//跋办把计-Cad畒夹-Y-um
const FILE_IO_ID FILE_IO_FIELD_STAGE_POS_X                    =      3018;//跋办把计-诀畒夹-X-um
const FILE_IO_ID FILE_IO_FIELD_STAGE_POS_Y                    =      3019;//跋办把计-诀畒夹-Y-um
const FILE_IO_ID FILE_IO_FIELD_STAGE_POS_Z                    =      3020;//跋办把计-诀畒夹-Z-um
const FILE_IO_ID FILE_IO_FIELD_SIZE_CX_INNER                  =      3021;//跋办把计-ず场へ-糴-um
const FILE_IO_ID FILE_IO_FIELD_SIZE_CY_INNER                  =      3022;//跋办把计-ず场へ-蔼-um
const FILE_IO_ID FILE_IO_FIELD_SIZE_CX_OUTER                  =      3023;//跋办把计-场へ-糴-um
const FILE_IO_ID FILE_IO_FIELD_SIZE_CY_OUTER                  =      3024;//跋办把计-场へ-蔼-um
const FILE_IO_ID FILE_IO_FIELD_OBJ_UUID                       =      3025;//跋办把计-OBJ-UUID
const FILE_IO_ID FILE_IO_FIELD_BOARD_INDEX                    =      3031;//跋办把计-虫狾ま计
const FILE_IO_ID FILE_IO_FIELD_COMPONENT_INDEX                =      3032;//跋办把计-箂ンま计
const FILE_IO_ID FILE_IO_FIELD_DISTRICT_ID                    =      3033;//跋办把计-琿絪腹
const FILE_IO_ID FILE_IO_FIELD_GRAB_INDEX_BY_USER             =      3034;//跋办把计-﹚竡钩Ω
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_FRAME_SECTION_START                  =      4001;//跋办紇钩把计跋丁-癬翴
const FILE_IO_ID FILE_IO_FRAME_SECTION_END                    =      4999;//跋办紇钩把计跋丁-沧翴

const FILE_IO_ID FILE_IO_FRAME_START                          =      4011;//跋办紇钩把计-癬翴
const FILE_IO_ID FILE_IO_FRAME_END                            =      4012;//跋办紇钩把计-沧翴
const FILE_IO_ID FILE_IO_FRAME_OBJ_UUID                       =      4013;//跋办紇钩把计-OBJ-UUID
const FILE_IO_ID FILE_IO_FRAME_INDEX                          =      4014;//跋办紇钩把计-絪腹
const FILE_IO_ID FILE_IO_FRAME_FIELD_INDEX                    =      4015;//跋办紇钩把计-跋办絪腹
const FILE_IO_ID FILE_IO_FRAME_TYPE                           =      4016;//跋办紇钩把计-紇钩妓Α
const FILE_IO_ID FILE_IO_FRAME_CAMERA_ID                      =      4017;//跋办紇钩把计-诀絪腹
const FILE_IO_ID FILE_IO_FRAME_LIGHT_MODE                     =      4018;//跋办紇钩把计-縊方家Α
const FILE_IO_ID FILE_IO_FRAME_FILE_NAME                      =      4019;//跋办紇钩把计-郎嘿
const FILE_IO_ID FILE_IO_FRAME_DISTRICT_ID                    =      4020;//跋办紇钩把计-だ琿絪腹
const FILE_IO_ID FILE_IO_FRAME_SATURATION_RED                 =      4021;//跋办紇钩把计-埂㎝秸俱-︹
const FILE_IO_ID FILE_IO_FRAME_SATURATION_GREEN               =      4022;//跋办紇钩把计-埂㎝秸俱-厚︹
const FILE_IO_ID FILE_IO_FRAME_SATURATION_BLUE                =      4023;//跋办紇钩把计-埂㎝秸俱-屡︹
const FILE_IO_ID FILE_IO_FRAME_BAYER_PATTERN                  =      4024;//跋办紇钩把计-Bayer妓狾
const FILE_IO_ID FILE_IO_FRAME_UNIQUE_ID                      =      4025;//跋办紇钩把计-斑絏
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_PANEL_SECTION_START                  =      5001;//俱狾把计跋丁-癬翴
const FILE_IO_ID FILE_IO_PANEL_SECTION_END                    =      5999;//俱狾把计跋丁-沧翴

const FILE_IO_ID FILE_IO_PANEL_START                          =      5002;//俱狾把计-癬翴
const FILE_IO_ID FILE_IO_PANEL_END                            =      5003;//俱狾把计-沧翴
const FILE_IO_ID FILE_IO_PANEL_INDEX_PROJECT                  =      5004;//俱狾把计-盡ま计-Debug
const FILE_IO_ID FILE_IO_PANEL_BYPASS                         =      5005;//俱狾把计-ぃ浪代
const FILE_IO_ID FILE_IO_PANEL_OBJ_UUID                       =      5006;//俱狾把计-OBJ-UUID
const FILE_IO_ID FILE_IO_PANEL_BARCODE_DEVICE_INDEX           =      5007;//俱狾把计-兵絏诀ま计
const FILE_IO_ID FILE_IO_PANEL_BARCODE_DEVICE_CODE_INDEX      =      5008;//俱狾把计-兵絏诀兵絏ま计
const FILE_IO_ID FILE_IO_PANEL_TYPE                           =      5014;//俱狾把计-妓Α
const FILE_IO_ID FILE_IO_PANEL_BARCODE_RESULT                 =      5015;//俱狾把计-兵絏挡狦
const FILE_IO_ID FILE_IO_PANEL_BARCODE_ENABLED                =      5016;//俱狾把计-兵絏币ノ
const FILE_IO_ID FILE_IO_PANEL_BOARD_ROW_COUNT                =      5017;//俱狾把计-虫狾计
const FILE_IO_ID FILE_IO_PANEL_BOARD_COL_COUNT                =      5018;//俱狾把计-虫狾逆计
const FILE_IO_ID FILE_IO_PANEL_BOARD_COL_BLOCK_COUNT          =      5019;//俱狾把计-虫狾逆跋遏计
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_BOARD_SECTION_START                  =      6001;//虫狾把计跋丁-癬翴
const FILE_IO_ID FILE_IO_BOARD_SECTION_END                    =      6999;//虫狾把计跋丁-沧翴

const FILE_IO_ID FILE_IO_BOARD_START                          =      6002;//虫狾把计-癬翴
const FILE_IO_ID FILE_IO_BOARD_END                            =      6003;//虫狾把计-沧翴
const FILE_IO_ID FILE_IO_BOARD_INDEX_PROJECT                  =      6004;//虫狾把计-盡ま计-Debug
const FILE_IO_ID FILE_IO_BOARD_PANEL_INDEX                    =      6005;//虫狾把计-俱狾ま计
const FILE_IO_ID FILE_IO_BOARD_BYPASS                         =      6006;//虫狾把计-ぃ浪代
const FILE_IO_ID FILE_IO_BOARD_MAP_ENABLE                     =      6007;//虫狾把计-﹚翴币ノ
const FILE_IO_ID FILE_IO_BOARD_OBJ_UUID                       =      6008;//虫狾把计-OBJ-UUID
const FILE_IO_ID FILE_IO_BOARD_ROTATED_ANGLE                  =      6009;//虫狾把计-臂锣à
const FILE_IO_ID FILE_IO_BOARD_ORIENTATION                    =      6010;//虫狾把计-よà-兵絏诀ま计
const FILE_IO_ID FILE_IO_BOARD_BARCODE_DEVICDE_INDEX          =      6011;//虫狾把计-兵絏诀兵絏ま计
const FILE_IO_ID FILE_IO_BOARD_BARCODE_DEVICDE_CODE_INDEX     =      6012;//虫狾把计-兵絏诀兵絏ま计
const FILE_IO_ID FILE_IO_BOARD_SIDE_MODE                      =      6013;//虫狾把计-タ璉
const FILE_IO_ID FILE_IO_BOARD_TYPE                           =      6014;//虫狾把计-妓Α
const FILE_IO_ID FILE_IO_BOARD_BARCODE_RESULT                 =      6015;//虫狾把计-兵絏ず甧
const FILE_IO_ID FILE_IO_BOARD_BARCODE_ENABLED                =      6016;//虫狾把计-兵絏币ノ
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_FD_SECTION_START                     =      7001;//﹚翴把计跋丁-癬翴
const FILE_IO_ID FILE_IO_FD_SECTION_END                       =      7999;//﹚翴把计跋丁-沧翴

const FILE_IO_ID FILE_IO_FD_START                             =      7002;//﹚翴把计-癬翴
const FILE_IO_ID FILE_IO_FD_END                               =      7003;//﹚翴把计-沧翴
const FILE_IO_ID FILE_IO_FD_UNIQUE_ID                         =      7004;//﹚翴把计-斑絏
const FILE_IO_ID FILE_IO_FD_INDEX_PROJECT                     =      7005;//﹚翴把计-盡ま计-Debug
const FILE_IO_ID FILE_IO_FD_PANEL_INDEX                       =      7006;//﹚翴把计-俱狾ま计
const FILE_IO_ID FILE_IO_FD_BOARD_INDEX                       =      7007;//﹚翴把计-虫狾ま计
const FILE_IO_ID FILE_IO_FD_FRAME_UNIQUE_ID                   =      7008;//﹚翴把计-紇钩ま计
const FILE_IO_ID FILE_IO_FD_FIELD_INDEX_RANDOM                =      7009;//﹚翴把计-跋办ま计
const FILE_IO_ID FILE_IO_FD_ANGLE                             =      7010;//﹚翴把计-à
const FILE_IO_ID FILE_IO_FD_BODY_SIZE_CX                      =      7011;//﹚翴把计-セ砰へ-糴-um
const FILE_IO_ID FILE_IO_FD_BODY_SIZE_CY                      =      7012;//﹚翴把计-セ砰へ-蔼-um
const FILE_IO_ID FILE_IO_FD_ROI_SIZE_CX                       =      7013;//﹚翴把计-穓碝へ-蔼-um
const FILE_IO_ID FILE_IO_FD_ROI_SIZE_CY                       =      7014;//﹚翴把计-穓碝へ-蔼-um
const FILE_IO_ID FILE_IO_FD_CAD_POS_X                         =      7015;//﹚翴把计-Cad畒夹-X-um
const FILE_IO_ID FILE_IO_FD_CAD_POS_Y                         =      7016;//﹚翴把计-Cad畒夹-Y-um
const FILE_IO_ID FILE_IO_FD_STAGE_TEACH_POS_X                 =      7017;//﹚翴把计-诀毙旧畒夹-X-um
const FILE_IO_ID FILE_IO_FD_STAGE_TEACH_POS_Y                 =      7018;//﹚翴把计-诀毙旧畒夹-Y-um
const FILE_IO_ID FILE_IO_FD_STAGE_TEACH_POS_Z                 =      7019;//﹚翴把计-诀毙旧畒夹-Z-um
const FILE_IO_ID FILE_IO_FD_STAGE_RESULT_POS_X                =      7020;//﹚翴把计-诀挡狦畒夹-X-um
const FILE_IO_ID FILE_IO_FD_STAGE_RESULT_POS_Y                =      7021;//﹚翴把计-诀挡狦畒夹-Y-um
const FILE_IO_ID FILE_IO_FD_STAGE_RESULT_POS_Z                =      7022;//﹚翴把计-诀挡狦畒夹-Z-um
const FILE_IO_ID FILE_IO_FD_PATTERN_EXTEND_ROI_CX             =      7023;//﹚翴把计-妓狾耎へ-糴-um
const FILE_IO_ID FILE_IO_FD_PATTERN_EXTEND_ROI_CY             =      7024;//﹚翴把计-妓狾耎へ-蔼-um
const FILE_IO_ID FILE_IO_FD_ROI_EXTEND_ROI_CX                 =      7025;//﹚翴把计-穓碝耎へ-糴-um
const FILE_IO_ID FILE_IO_FD_ROI_EXTEND_ROI_CY                 =      7026;//﹚翴把计-穓碝耎へ-蔼-um
const FILE_IO_ID FILE_IO_FD_OBJ_UUID                          =      7027;//﹚翴把计-OBJ-UUID
//const FILE_IO_ID FILE_IO_FD_STAGE_TEACH_POS_X_LB              =      7028;//﹚翴把计-诀毙旧畒夹-X-um//remove-20190308
//const FILE_IO_ID FILE_IO_FD_STAGE_TEACH_POS_Y_LB              =      7029;//﹚翴把计-诀毙旧畒夹-Y-um//remove-20190308
//const FILE_IO_ID FILE_IO_FD_STAGE_TEACH_POS_Z_LB              =      7030;//﹚翴把计-诀毙旧畒夹-Z-um//remove-20190308
const FILE_IO_ID FILE_IO_FD_SORT_ID                           =      7031;//﹚翴把计-逼絪腹
const FILE_IO_ID FILE_IO_FD_DISTRICT_ID                       =      7032;//﹚翴把计-だ琿絪腹
const FILE_IO_ID FILE_IO_FD_GROUP_ID                          =      7033;//﹚翴把计-竤舱絪腹
const FILE_IO_ID FILE_IO_FD_CAD_SPECIAL_POS_X                 =      7051;//﹚翴把计-﹚翴竚PCBオ翴CADГ夹╰い-X
const FILE_IO_ID FILE_IO_FD_CAD_SPECIAL_POS_Y                 =      7052;//﹚翴把计-﹚翴竚PCBオ翴CADГ夹╰い-Y
const FILE_IO_ID FILE_IO_FD_MODEL_NODE                        =      7801;//﹚翴把计-家舱竊翴
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_BARCODE_SECTION_START                =      8001;//硁砰兵絏把计跋丁-癬翴
const FILE_IO_ID FILE_IO_BARCODE_SECTION_END                  =      8999;//硁砰兵絏把计跋丁-沧翴

const FILE_IO_ID FILE_IO_BARCODE_START                        =      8002;//硁砰兵絏把计-癬翴
const FILE_IO_ID FILE_IO_BARCODE_END                          =      8003;//硁砰兵絏把计-沧翴
const FILE_IO_ID FILE_IO_BARCODE_UNIQUE_ID                    =      8004;//硁砰兵絏把计-斑絏
const FILE_IO_ID FILE_IO_BARCODE_INDEX_PROJECT                =      8005;//硁砰兵絏把计-盡ま计-Debug
const FILE_IO_ID FILE_IO_BARCODE_PANEL_INDEX                  =      8006;//硁砰兵絏把计-俱狾ま计
const FILE_IO_ID FILE_IO_BARCODE_BOARD_INDEX                  =      8007;//硁砰兵絏把计-虫狾ま计
const FILE_IO_ID FILE_IO_BARCODE_ANGLE                        =      8008;//硁砰兵絏把计-à
const FILE_IO_ID FILE_IO_BARCODE_BODY_SIZE_CX                 =      8009;//硁砰兵絏把计-セ砰へ-糴-um
const FILE_IO_ID FILE_IO_BARCODE_BODY_SIZE_CY                 =      8010;//硁砰兵絏把计-セ砰へ-蔼-um
const FILE_IO_ID FILE_IO_BARCODE_ROI_SIZE_CX                  =      8011;//硁砰兵絏把计-穓碝へ-糴-um
const FILE_IO_ID FILE_IO_BARCODE_ROI_SIZE_CY                  =      8012;//硁砰兵絏把计-穓碝へ-蔼-um
const FILE_IO_ID FILE_IO_BARCODE_CAD_POS_X                    =      8013;//硁砰兵絏把计-Cad畒夹-X-um
const FILE_IO_ID FILE_IO_BARCODE_CAD_POS_Y                    =      8014;//硁砰兵絏把计-Cad畒夹-Y-um
const FILE_IO_ID FILE_IO_BARCODE_STAGE_POS_X                  =      8015;//硁砰兵絏把计-诀畒夹-X-um
const FILE_IO_ID FILE_IO_BARCODE_STAGE_POS_Y                  =      8016;//硁砰兵絏把计-诀畒夹-Y-um
const FILE_IO_ID FILE_IO_BARCODE_STAGE_POS_Z                  =      8017;//硁砰兵絏把计-诀畒夹-Z-um
const FILE_IO_ID FILE_IO_BARCODE_OBJ_UUID                     =      8018;//硁砰兵絏把计-OBJ-UUID
const FILE_IO_ID FILE_IO_BARCODE_SAVE_TEST_IMAGE_MODE         =      8019;//硁砰兵絏把计-纗浪代瓜郎家Α
const FILE_IO_ID FILE_IO_BARCODE_DISTRICT_ID                  =      8021;//硁砰兵絏把计-だ琿絪腹
const FILE_IO_ID FILE_IO_BARCODE_GROUP_ID                     =      8022;//硁砰兵絏把计-竤舱絪腹
const FILE_IO_ID FILE_IO_BARCODE_LOCAL_BASE_PLANE_ID          =      8023;//硁砰兵絏把计-Ы场膀非絪腹
const FILE_IO_ID FILE_IO_BARCODE_BELONG_MODE                  =      8024;//硁砰兵絏把计-妮家Α
const FILE_IO_ID FILE_IO_BARCODE_SPREAD_OUT                   =      8025;//硁砰兵絏把计-耎甶家Α
const FILE_IO_ID FILE_IO_BARCODE_MODEL_NODE                   =      8801;//硁砰兵絏把计-家舱把计
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_COMPONENT_SECTION_START              =      9001;//箂ン把计跋丁-癬翴
const FILE_IO_ID FILE_IO_COMPONENT_SECTION_END                =      9999;//箂ン把计跋丁-沧翴

const FILE_IO_ID FILE_IO_COMPONENT_START                      =      9002;//箂ン把计-癬翴
const FILE_IO_ID FILE_IO_COMPONENT_END                        =      9003;//箂ン把计-沧翴
const FILE_IO_ID FILE_IO_COMPONENT_UNIQUE_ID                  =      9004;//箂ン把计-斑絏
const FILE_IO_ID FILE_IO_COMPONENT_PANEL_INDEX                =      9005;//箂ン把计-俱狾ま计
const FILE_IO_ID FILE_IO_COMPONENT_BOARD_INDEX                =      9006;//箂ン把计-虫狾ま计
const FILE_IO_ID FILE_IO_COMPONENT_NAME                       =      9007;//箂ン把计-嘿
const FILE_IO_ID FILE_IO_COMPONENT_MODEL_NAME                 =      9008;//箂ン把计-家舱嘿
const FILE_IO_ID FILE_IO_COMPONENT_PART_NUMBER                =      9009;//箂ン把计-腹
const FILE_IO_ID FILE_IO_COMPONENT_NOZZLE_NAME                =      9010;//箂ン把计-糒嘿
const FILE_IO_ID FILE_IO_COMPONENT_FRAME_INDEX                =      9011;//箂ン把计-紇钩ま计
const FILE_IO_ID FILE_IO_COMPONENT_FIELD_INDEX_RANDOM         =      9012;//箂ン把计-跋办ま计
const FILE_IO_ID FILE_IO_COMPONENT_ANGLE                      =      9013;//箂ン把计-à
const FILE_IO_ID FILE_IO_COMPONENT_BODY_SIZE_CX               =      9014;//箂ン把计-セ砰へ-糴-um
const FILE_IO_ID FILE_IO_COMPONENT_BODY_SIZE_CY               =      9015;//箂ン把计-セ砰へ-蔼-um
const FILE_IO_ID FILE_IO_COMPONENT_ROI_SIZE_CX                =      9016;//箂ン把计-穓碝へ-糴-um
const FILE_IO_ID FILE_IO_COMPONENT_ROI_SIZE_CY                =      9017;//箂ン把计-穓碝へ-蔼-um
const FILE_IO_ID FILE_IO_COMPONENT_CAD_POS_X                  =      9018;//箂ン把计-Cad畒夹-X-um
const FILE_IO_ID FILE_IO_COMPONENT_CAD_POS_Y                  =      9019;//箂ン把计-Cad畒夹-Y-um
const FILE_IO_ID FILE_IO_COMPONENT_ORG_CAD_POS_X              =      9020;//箂ン把计-﹍Cad畒夹-X-um
const FILE_IO_ID FILE_IO_COMPONENT_ORG_CAD_POS_Y              =      9021;//箂ン把计-﹍Cad畒夹-Y-um
const FILE_IO_ID FILE_IO_COMPONENT_STAGE_POS_X                =      9022;//箂ン把计-诀畒夹-X-um
const FILE_IO_ID FILE_IO_COMPONENT_STAGE_POS_Y                =      9023;//箂ン把计-诀畒夹-Y-um
const FILE_IO_ID FILE_IO_COMPONENT_STAGE_POS_Z                =      9024;//箂ン把计-诀畒夹-Z-um
const FILE_IO_ID FILE_IO_COMPONENT_BYPASS                     =      9025;//箂ン把计-ぃ浪代
const FILE_IO_ID FILE_IO_COMPONENT_XBOARD_UNIT                =      9026;//箂ン把计-X狾虫
const FILE_IO_ID FILE_IO_COMPONENT_MODEL_INDEX                =      9027;//箂ン把计-家舱ま计
const FILE_IO_ID FILE_IO_COMPONENT_MODEL_ISOLATED             =      9028;//箂ン把计-琌家舱筳瞒
const FILE_IO_ID FILE_IO_COMPONENT_OBJ_UUID                   =      9029;//箂ン把计-UUID
const FILE_IO_ID FILE_IO_COMPONENT_BYPASS_3D                  =      9030;//箂ン把计-琌3Dぃ浪代

const FILE_IO_ID FILE_IO_COMPONENT_2DMASK_BASE_ENABLE         =      9031;//箂ン把计-膀非2D綛竛-币ノ//v1.01.04.038セ硋亥簿埃
const FILE_IO_ID FILE_IO_COMPONENT_2DMASK_FRAME_UNIQUE_ID     =      9032;//箂ン把计-膀非2D綛竛-斑絏//v1.01.04.038セ硋亥簿埃
const FILE_IO_ID FILE_IO_COMPONENT_2DMASK_COLOR_GROUP_INDEX   =      9033;//箂ン把计-膀非2D綛竛-盡肅︹腹//v1.01.04.038セ硋亥簿埃

const FILE_IO_ID FILE_IO_COMPONENT_COMPONENT_TYPE             =      9040;//箂ン把计-箂ン妓Α
const FILE_IO_ID FILE_IO_COMPONENT_MASK_EXTEND_W_BODY         =      9041;//箂ン把计-綛竛耎糴-セ砰
const FILE_IO_ID FILE_IO_COMPONENT_MASK_EXTEND_H_BODY         =      9042;//箂ン把计-綛竛耎-セ砰
const FILE_IO_ID FILE_IO_COMPONENT_MASK_EXTEND_W_LAND         =      9043;//箂ン把计-綛竛耎糴-瞜絃
const FILE_IO_ID FILE_IO_COMPONENT_MASK_EXTEND_H_LAND         =      9044;//箂ン把计-綛竛耎-瞜絃

const FILE_IO_ID FILE_IO_COMPONENT_DISTRICT_ID                =      9048;//箂ン把计-だ琿絪腹
const FILE_IO_ID FILE_IO_COMPONENT_SELF_FIELD_ENABLED         =      9049;//箂ン把计-盡妮跋办币ノ

const FILE_IO_ID FILE_IO_COMPONENT_ENABLE_ALARM               =      9050;//箂ン把计-币ノ牡厨
const FILE_IO_ID FILE_IO_COMPONENT_GROUP_ID                   =      9051;//箂ン把计-竤舱絪腹
const FILE_IO_ID FILE_IO_COMPONENT_GROUP_ORG                  =      9052;//箂ン把计-竤舱翴
const FILE_IO_ID FILE_IO_COMPONENT_LOCAL_BASE_PLANE_ID        =      9053;//箂ン把计-Ы场膀非絪腹
const FILE_IO_ID FILE_IO_COMPONENT_MODEL_CLASS_ID             =      9054;//箂ン把计-家舱摸絪腹(﹚)
const FILE_IO_ID FILE_IO_COMPONENT_DEFECT_ALARM_FROM_MODE_AOI =      9055;//箂ン把计-峰搏牡厨ㄓ方家Α-AOI
const FILE_IO_ID FILE_IO_COMPONENT_DEFECT_ALARM_FROM_MODE_ARS =      9056;//箂ン把计-峰搏牡厨ㄓ方家Α-ARS

const FILE_IO_ID FILE_IO_COMPONENT_DATA_MODEL_ENABLED         =      9060;//箂ン把计-戈家舱币ノ
const FILE_IO_ID FILE_IO_COMPONENT_DATA_MODEL_LEVEL_ID        =      9061;//箂ン把计-戈家舱单
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_COMPONENT_SAVE_WND_LIST              =      9070;//箂ン把计-纗浪代
const FILE_IO_ID FILE_IO_COMPONENT_SAVE_REPORT_ARS            =      9071;//箂ン把计-箂ン纗厨-蝴
const FILE_IO_ID FILE_IO_COMPONENT_GRR_SIGMA_ITEM_INDEX       =      9072;//箂ン把计-GRR夹非畉ま计
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_COMPONENT_DEFECT_ALARM_ENABLE_ON_AOI =      9101;//箂ン把计-峰搏牡厨砞称牡厨
const FILE_IO_ID FILE_IO_COMPONENT_DEFECT_ALARM_ENABLE_ON_ARS =      9102;//箂ン把计-峰搏牡厨蝴陪ボ
const FILE_IO_ID FILE_IO_COMPONENT_DEFECT_COUNT_ENABLE_ON_ARS =      9104;//箂ン把计-峰搏璸计蝴陪ボ
const FILE_IO_ID FILE_IO_COMPONENT_TOTAL_NG_COUNT_ENABLE      =      9105;//箂ン把计-仓璸峰搏币ノ-﹚竡
const FILE_IO_ID FILE_IO_COMPONENT_CONTINUE_NG_COUNT_ENABLE   =      9106;//箂ン把计-硈尿峰搏币ノ-﹚竡
const FILE_IO_ID FILE_IO_COMPONENT_TOTAL_NG_COUNT_LIMIT       =      9107;//箂ン把计-仓璸峰搏-﹚竡
const FILE_IO_ID FILE_IO_COMPONENT_CONTINUE_NG_COUNT_LIMIT    =      9108;//箂ン把计-硈尿峰搏-﹚竡
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_COMPONENT_MASTER_INDEX               =      9150;//箂ン把计-セ碙ま计
const FILE_IO_ID FILE_IO_COMPONENT_COL_INDEX                  =      9151;//箂ン把计-逆ま计-X
const FILE_IO_ID FILE_IO_COMPONENT_ROW_INDEX                  =      9152;//箂ン把计-ま计-Y
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_COMPONENT_HASI_SCO_ENABLE            =      9200;//箂ン把计-HAS I(Hanwha AOI Solution MAOI_SAOI)-甅ノSPIタ
const FILE_IO_ID FILE_IO_COMPONENT_M2M_SAVE_IMAGE             =      9201;//箂ン把计-HAS I-玂瓜M2M戈Ж
const FILE_IO_ID FILE_IO_COMPONENT_SPECIAL_CAD_POS_X          =      9202;//箂ン把计-オO翴Cad畒夹-X
const FILE_IO_ID FILE_IO_COMPONENT_SPECIAL_CAD_POS_Y          =      9203;//箂ン把计-オO翴Cad畒夹-Y
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_COMPONENT_3D_NOISE_FILTER_NODE       =      9800;//箂ン把计-3D馒癟筁耾把计
const FILE_IO_ID FILE_IO_COMPONENT_MODEL_NODE                 =      9801;//箂ン把计-家舱把计
const FILE_IO_ID FILE_IO_COMPONENT_VERSION_PARAM_NODE         =      9802;//箂ン把计-セ把计
const FILE_IO_ID FILE_IO_COMPONENT_DEFECT_ITEM_ALARM_AOI      =      9803;//箂ン把计-峰搏兜ヘ牡厨-AOI
const FILE_IO_ID FILE_IO_COMPONENT_DEFECT_ITEM_ALARM_ARS      =      9804;//箂ン把计-峰搏兜ヘ牡厨-ARS
const FILE_IO_ID FILE_IO_COMPONENT_GRR_SIGMA_ITEM_LIST_NODE   =      9809;//箂ン把计-GRR夹非畉
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_WINDOW_SECTION_START                 =     10001;//浪代把计跋丁-癬翴
const FILE_IO_ID FILE_IO_WINDOW_SECTION_END                   =     10999;//浪代把计跋丁-沧翴

const FILE_IO_ID FILE_IO_WINDOW_START                         =     10002;//浪代把计-癬翴
const FILE_IO_ID FILE_IO_WINDOW_END                           =     10003;//浪代把计-沧翴
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_MODEL_SECTION_START                  =     11001;//家舱把计跋丁-癬翴
const FILE_IO_ID FILE_IO_MODEL_SECTION_END                    =     11999;//家舱把计跋丁-沧翴

const FILE_IO_ID FILE_IO_MODEL_START                          =     11002;//家舱把计-癬翴
const FILE_IO_ID FILE_IO_MODEL_END                            =     11003;//家舱把计-沧翴
const FILE_IO_ID FILE_IO_MODEL_TYPE                           =     11004;//家舱把计-家舱妓Α
const FILE_IO_ID FILE_IO_MODEL_NAME                           =     11005;//家舱把计-家舱嘿
const FILE_IO_ID FILE_IO_MODEL_GROUP_NAME                     =     11006;//家舱把计-家舱竤舱嘿
const FILE_IO_ID FILE_IO_MODEL_LAND_DIRECTION                 =     11007;//家舱把计-家舱竲よ
const FILE_IO_ID FILE_IO_MODEL_OBJ_UUID                       =     11008;//家舱把计-OBJ-UUID
const FILE_IO_ID FILE_IO_MODEL_BK_IMAGE_INDEX                 =     11009;//家舱把计-家舱┏瓜紇钩絪腹
const FILE_IO_ID FILE_IO_MODEL_CHIP_SIZE_MODE                 =     11010;//家舱把计-家舱砆笆じンへ家Α

const FILE_IO_ID FILE_IO_MODEL_BODY_SIZE_X                    =     11011;//家舱把计-家舱セ砰へX
const FILE_IO_ID FILE_IO_MODEL_BODY_SIZE_Y                    =     11012;//家舱把计-家舱セ砰へY
const FILE_IO_ID FILE_IO_MODEL_BODY_HEIGHT                    =     11013;//家舱把计-家舱セ砰蔼

const FILE_IO_ID FILE_IO_MODEL_MODIFIED_DATE_TIME             =     11021;//家舱把计-家舱эら戳
const FILE_IO_ID FILE_IO_MODEL_EXTEND_RANGE_X                 =     11022;//家舱把计-家舱耎絛瞅X
const FILE_IO_ID FILE_IO_MODEL_EXTEND_RANGE_Y                 =     11023;//家舱把计-家舱耎絛瞅Y
const FILE_IO_ID FILE_IO_MODEL_EXTEND_AUTO_ADJUST             =     11024;//家舱把计-家舱耎笆秸俱
const FILE_IO_ID FILE_IO_MODEL_SAVE_LEAD_REPORT               =     11025;//家舱把计-家舱纗ま竲厨
const FILE_IO_ID FILE_IO_MODEL_BODY_LINK_CHIP_LEAD            =     11026;//家舱把计-家舱セ砰硈笆砆笆じンま竲
const FILE_IO_ID FILE_IO_MODEL_WND_SYNC_MOVE_MODE             =     11027;//家舱把计-浪代˙簿笆家Α-箇砞

const FILE_IO_ID FILE_IO_MODEL_DEFECT_ITEM_ESSENTIAL          =     11101;//家舱把计-家舱峰搏ゲ璶絋粄兜ヘ
//(11501~11599-ARS)
const FILE_IO_ID FILE_IO_MODEL_DEFECT_ITEM_RECHECK_ARS        =     11581;//家舱把计-家舱峰搏狡絋粄兜ヘ-ARS(11501~11599-ARS)

const FILE_IO_ID FILE_IO_MODEL_BODY_BOX                       =     11801;//家舱把计-家舱セ砰
const FILE_IO_ID FILE_IO_MODEL_LAND_NODE                      =     11802;//家舱把计-家舱疭紉
const FILE_IO_ID FILE_IO_MODEL_WND_NODE                       =     11803;//家舱把计-家舱疭紉
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_COLOR_RGBV_SECTION_START             =     12001;//RGBV肅︹把计跋丁-癬翴
const FILE_IO_ID FILE_IO_COLOR_RGBV_SECTION_END               =     12099;//RGBV肅︹把计跋丁-沧翴

const FILE_IO_ID FILE_IO_COLOR_RGBV_START                     =     12002;//RGBV肅︹把计-癬翴
const FILE_IO_ID FILE_IO_COLOR_RGBV_END                       =     12003;//RGBV肅︹把计-沧翴
const FILE_IO_ID FILE_IO_COLOR_RGBV_COLOR_MODE                =     12004;//RGBV肅︹把计-︹眒家Α
const FILE_IO_ID FILE_IO_COLOR_RGBV_LOGIC_MODE                =     12005;//RGBV肅︹把计-︹眒呸胯
const FILE_IO_ID FILE_IO_COLOR_RGBV_RED_MAX                   =     12060;//RGBV肅︹把计-︹-60~69
const FILE_IO_ID FILE_IO_COLOR_RGBV_RED_MIN                   =     12061;//RGBV肅︹把计-︹
const FILE_IO_ID FILE_IO_COLOR_RGBV_RED_ENABLED               =     12062;//RGBV肅︹把计-︹币ノ
const FILE_IO_ID FILE_IO_COLOR_RGBV_GREEN_MAX                 =     12070;//RGBV肅︹把计-厚︹-70~79
const FILE_IO_ID FILE_IO_COLOR_RGBV_GREEN_MIN                 =     12071;//RGBV肅︹把计-厚︹
const FILE_IO_ID FILE_IO_COLOR_RGBV_GREEN_ENABLED             =     12072;//RGBV肅︹把计-厚︹币ノ
const FILE_IO_ID FILE_IO_COLOR_RGBV_BLUE_MAX                  =     12080;//RGBV肅︹把计-屡︹-80~89
const FILE_IO_ID FILE_IO_COLOR_RGBV_BLUE_MIN                  =     12081;//RGBV肅︹把计-屡︹
const FILE_IO_ID FILE_IO_COLOR_RGBV_BLUE_ENABLED              =     12082;//RGBV肅︹把计-屡︹币ノ
const FILE_IO_ID FILE_IO_COLOR_RGBV_VALUE_MAX                 =     12090;//RGBV肅︹把计-η︹-90~98
const FILE_IO_ID FILE_IO_COLOR_RGBV_VALUE_MIN                 =     12091;//RGBV肅︹把计-η︹
const FILE_IO_ID FILE_IO_COLOR_RGBV_VALUE_ENABLED             =     12092;//RGBV肅︹把计-η︹币ノ
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_COLOR_GROUP_SECTION_START            =     12101;//┾︹竤舱把计跋丁-癬翴
const FILE_IO_ID FILE_IO_COLOR_GROUP_SECTION_END              =     12199;//┾︹竤舱把计跋丁-沧翴

const FILE_IO_ID FILE_IO_COLOR_GROUP_START                    =     12102;//┾︹竤舱把计-癬翴
const FILE_IO_ID FILE_IO_COLOR_GROUP_END                      =     12103;//┾︹竤舱把计-沧翴

const FILE_IO_ID FILE_IO_COLOR_GROUP_NAME                     =     12104;//┾︹竤舱把计-㏑
const FILE_IO_ID FILE_IO_COLOR_GROUP_FRAME_UNIQUE_ID          =     12105;//┾︹竤舱把计-礶斑絏
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_BARCODE_STEP_SECTION_START           =     12201;//秆絏˙艼把计跋丁-癬翴
const FILE_IO_ID FILE_IO_BARCODE_STEP_SECTION_END             =     12219;//秆絏˙艼把计跋丁-沧翴

const FILE_IO_ID FILE_IO_BARCODE_STEP_START                   =     12202;//秆絏˙艼把计-癬翴
const FILE_IO_ID FILE_IO_BARCODE_STEP_END                     =     12203;//秆絏˙艼把计-沧翴

const FILE_IO_ID FILE_IO_BARCODE_STEP_STEP_INDEX              =     12204;//秆絏˙艼把计-ま计
const FILE_IO_ID FILE_IO_BARCODE_STEP_STEP_MODE               =     12205;//秆絏˙艼把计-˙艼
const FILE_IO_ID FILE_IO_BARCODE_STEP_PARAM_1                 =     12206;//秆絏˙艼把计-把计1
const FILE_IO_ID FILE_IO_BARCODE_STEP_PARAM_2                 =     12207;//秆絏˙艼把计-把计2
const FILE_IO_ID FILE_IO_BARCODE_STEP_ENABLED                 =     12208;//秆絏˙艼把计-币ノ
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_CAMERA_SECTION_START                 =     12221;//诀把计跋丁-癬翴
const FILE_IO_ID FILE_IO_CAMERA_SECTION_END                   =     12239;//诀把计跋丁-沧翴

const FILE_IO_ID FILE_IO_CAMERA_START                         =     12222;//诀把计-沧翴
const FILE_IO_ID FILE_IO_CAMERA_END                           =     12223;//诀把计-沧翴
const FILE_IO_ID FILE_IO_CAMERA_IMAGE_SIZE_W                  =     12224;//诀把计-紇钩糴
const FILE_IO_ID FILE_IO_CAMERA_IMAGE_SIZE_H                  =     12225;//诀把计-紇钩
const FILE_IO_ID FILE_IO_CAMERA_RESOLUTION_X                  =     12226;//诀把计-秆猂-X
const FILE_IO_ID FILE_IO_CAMERA_RESOLUTION_Y                  =     12227;//诀把计-秆猂-Y
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_OFFLINE_SECTION_START                =     12301;//瞒絬把计跋丁-癬翴
const FILE_IO_ID FILE_IO_OFFLINE_SECTION_END                  =     12399;//瞒絬把计跋丁-沧翴

const FILE_IO_ID FILE_IO_OFFLINE_START                        =     12302;//瞒絬把计跋丁-癬翴
const FILE_IO_ID FILE_IO_OFFLINE_END                          =     12303;//瞒絬把计跋丁-沧翴
const FILE_IO_ID FILE_IO_OFFLINE_FILE_MODE                    =     12304;//瞒絬把计跋丁-瞒絬郎家Α
const FILE_IO_ID FILE_IO_OFFLINE_FIELD_BUILD_MODE             =     12305;//瞒絬把计跋丁-瞒絬郎家Α
const FILE_IO_ID FILE_IO_OFFLINE_TEST_START_X                 =     12306;//瞒絬把计跋丁-浪代癬翴-X
const FILE_IO_ID FILE_IO_OFFLINE_TEST_START_Y                 =     12307;//瞒絬把计跋丁-浪代癬翴-Y
const FILE_IO_ID FILE_IO_OFFLINE_FOCUS_OFFSET_Z               =     12308;//瞒絬把计跋丁-礘禯熬畉-Z
const FILE_IO_ID FILE_IO_OFFLINE_IMAGE_SCOPE                  =     12309;//瞒絬把计跋丁-紇钩絛氓

const FILE_IO_ID FILE_IO_OFFLINE_RESOLUTION_MODE              =     12310;//瞒絬把计跋丁-秆猂家Α
const FILE_IO_ID FILE_IO_OFFLINE_FOV_SIZE_FULL_W              =     12311;//瞒絬把计跋丁-跌偿-糴-场
const FILE_IO_ID FILE_IO_OFFLINE_FOV_SIZE_FULL_H              =     12312;//瞒絬把计跋丁-跌偿--场
const FILE_IO_ID FILE_IO_OFFLINE_FOV_SIZE_OUTTER_W            =     12313;//瞒絬把计跋丁-跌偿-糴-场
const FILE_IO_ID FILE_IO_OFFLINE_FOV_SIZE_OUTTER_H            =     12314;//瞒絬把计跋丁-跌偿--场
const FILE_IO_ID FILE_IO_OFFLINE_FOV_SIZE_INNER_W             =     12315;//瞒絬把计跋丁-跌偿-糴-ず场
const FILE_IO_ID FILE_IO_OFFLINE_FOV_SIZE_INNER_H             =     12316;//瞒絬把计跋丁-跌偿--ず场
const FILE_IO_ID FILE_IO_OFFLINE_FOV_SIZE_MODE_W              =     12317;//瞒絬把计跋丁-跌偿-糴-家Α
const FILE_IO_ID FILE_IO_OFFLINE_FOV_SIZE_MODE_H              =     12318;//瞒絬把计跋丁-跌偿--家Α

const FILE_IO_ID FILE_IO_OFFLINE_FIELD_COUNT                  =     12321;//瞒絬把计跋丁-跋办计秖
const FILE_IO_ID FILE_IO_OFFLINE_FRAME_COUNT                  =     12322;//瞒絬把计跋丁-跋办紇钩计秖
const FILE_IO_ID FILE_IO_OFFLINE_FIELD_BUILD_AREA_MODE        =     12323;//瞒絬把计跋丁-瞒絬ミ縩家Α
const FILE_IO_ID FILE_IO_OFFLINE_LANE_ID                      =     12324;//瞒絬把计跋丁-瓂笵絪腹

const FILE_IO_ID FILE_IO_OFFLINE_CAMERA_INFO_1                =     12331;//瞒絬把计跋丁-诀紇钩糴

const FILE_IO_ID FILE_IO_OFFLINE_FRAME_UNIQUE_ID_COUNT        =     12340;//瞒絬把计跋丁-紇钩斑絏计秖
const FILE_IO_ID FILE_IO_OFFLINE_FRAME_UNIQUE_ID_01           =     12341;//瞒絬把计跋丁-紇钩斑絏-1
const FILE_IO_ID FILE_IO_OFFLINE_FRAME_UNIQUE_ID_02           =     12342;//瞒絬把计跋丁-紇钩斑絏-2
const FILE_IO_ID FILE_IO_OFFLINE_FRAME_UNIQUE_ID_03           =     12343;//瞒絬把计跋丁-紇钩斑絏-3
const FILE_IO_ID FILE_IO_OFFLINE_FRAME_UNIQUE_ID_04           =     12344;//瞒絬把计跋丁-紇钩斑絏-4
const FILE_IO_ID FILE_IO_OFFLINE_FRAME_UNIQUE_ID_05           =     12345;//瞒絬把计跋丁-紇钩斑絏-5
const FILE_IO_ID FILE_IO_OFFLINE_FRAME_UNIQUE_ID_06           =     12346;//瞒絬把计跋丁-紇钩斑絏-6
const FILE_IO_ID FILE_IO_OFFLINE_FRAME_UNIQUE_ID_07           =     12347;//瞒絬把计跋丁-紇钩斑絏-7
const FILE_IO_ID FILE_IO_OFFLINE_FRAME_UNIQUE_ID_08           =     12348;//瞒絬把计跋丁-紇钩斑絏-8
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_NOISE_FILTER_SECTION_START           =     12401;//馒癟筁耾把计跋丁-癬翴
const FILE_IO_ID FILE_IO_NOISE_FILTER_SECTION_END             =     12499;//馒癟筁耾把计跋丁-沧翴

const FILE_IO_ID FILE_IO_NOISE_FILTER_START                   =     12402;//馒癟筁耾把计-癬翴
const FILE_IO_ID FILE_IO_NOISE_FILTER_END                     =     12403;//馒癟筁耾把计-沧翴
const FILE_IO_ID FILE_IO_NOISE_FILTER_MODE                    =     12404;//馒癟筁耾把计-家Α
const FILE_IO_ID FILE_IO_NOISE_FILTER_PARAM_1                 =     12405;//馒癟筁耾把计-把计1
const FILE_IO_ID FILE_IO_NOISE_FILTER_PARAM_2                 =     12406;//馒癟筁耾把计-把计2
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_POINT_SECTION_START                  =     12501;//翴把计跋丁-癬翴
const FILE_IO_ID FILE_IO_POINT_SECTION_END                    =     12509;//翴把计跋丁-沧翴

const FILE_IO_ID FILE_IO_POINT_START                          =     12502;//翴把计-癬翴
const FILE_IO_ID FILE_IO_POINT_END                            =     12503;//翴把计-沧翴
const FILE_IO_ID FILE_IO_POINT_X                              =     12504;//翴把计-X
const FILE_IO_ID FILE_IO_POINT_Y                              =     12505;//翴把计-Y
const FILE_IO_ID FILE_IO_POINT_Z                              =     12506;//翴把计-Z
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_RECT_SECTION_START                   =     12511;//痻把计跋丁-癬翴
const FILE_IO_ID FILE_IO_RECT_SECTION_END                     =     12519;//痻把计跋丁-沧翴

const FILE_IO_ID FILE_IO_RECT_START                           =     12512;//痻把计-癬翴
const FILE_IO_ID FILE_IO_RECT_END                             =     12513;//痻把计-沧翴
const FILE_IO_ID FILE_IO_RECT_LEFT                            =     12514;//痻把计-オ
const FILE_IO_ID FILE_IO_RECT_TOP                             =     12515;//痻把计-
const FILE_IO_ID FILE_IO_RECT_RIGHT                           =     12516;//痻把计-
const FILE_IO_ID FILE_IO_RECT_BOTTOM                          =     12517;//痻把计-
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_BOL_LIST_START                       =     12521;//ガ狶把计-癬翴
const FILE_IO_ID FILE_IO_BOL_LIST_END                         =     12522;//ガ狶把计-沧翴
const FILE_IO_ID FILE_IO_BOL_LIST_VALUE                       =     12523;//ガ狶把计-计
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_INT_LIST_START                       =     12525;//俱计把计-癬翴
const FILE_IO_ID FILE_IO_INT_LIST_END                         =     12526;//俱计把计-沧翴
const FILE_IO_ID FILE_IO_INT_LIST_VALUE                       =     12527;//俱计把计-计
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_DBL_LIST_START                       =     12531;//疊翴计把计-癬翴
const FILE_IO_ID FILE_IO_DBL_LIST_END                         =     12532;//疊翴把计-沧翴
const FILE_IO_ID FILE_IO_DBL_LIST_VALUE                       =     12533;//疊翴把计-计
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_STR_LIST_START                       =     12535;//﹃把计-癬翴
const FILE_IO_ID FILE_IO_STR_LIST_END                         =     12536;//﹃把计-沧翴
const FILE_IO_ID FILE_IO_STR_LIST_VALUE                       =     12537;//﹃把计-计
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_VERSION_CODE_SECTION_START           =     12551;//セ腹把计跋丁-癬翴
const FILE_IO_ID FILE_IO_VERSION_CODE_SECTION_END             =     12559;//セ腹把计跋丁-沧翴

const FILE_IO_ID FILE_IO_VERSION_CODE_START                   =     12552;//セ腹把计-癬翴
const FILE_IO_ID FILE_IO_VERSION_CODE_END                     =     12553;//セ腹把计-沧翴
const FILE_IO_ID FILE_IO_VERSION_CODE_NAME                    =     12554;//セ腹把计-嘿
const FILE_IO_ID FILE_IO_VERSION_CODE_ENABLED                 =     12555;//セ腹把计-币ノ
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_VERSION_PARAM_SECTION_START          =     12561;//セ把计跋丁-癬翴
const FILE_IO_ID FILE_IO_VERSION_PARAM_SECTION_END            =     12599;//セ把计跋丁-沧翴

const FILE_IO_ID FILE_IO_VERSION_PARAM_START                  =     12562;//セ把计-癬翴
const FILE_IO_ID FILE_IO_VERSION_PARAM_END                    =     12563;//セ把计-沧翴
const FILE_IO_ID FILE_IO_VERSION_PARAM_BYPASSED               =     12564;//セ把计-ぃ浪代
const FILE_IO_ID FILE_IO_VERSION_PARAM_BYPASSED_3D            =     12565;//セ把计-ぃ浪代3D
const FILE_IO_ID FILE_IO_VERSION_PARAM_X_BOARD_UNIT           =     12566;//セ把计-厨紀ン
const FILE_IO_ID FILE_IO_VERSION_PARAM_MODEL_NAME             =     12567;//セ把计-家舱嘿
const FILE_IO_ID FILE_IO_VERSION_PARAM_PART_NUMBER            =     12568;//セ把计-腹嘿
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_SECTION_START        =     12611;//3D筁耾把计跋丁-癬翴
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_SECTION_END          =     12699;//3D筁耾把计跋丁-沧翴

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_START                =     12612;//3D筁耾把计-癬翴
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_END                  =     12613;//3D筁耾把计-沧翴

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_BASE_PROC_TYPE  =     12620;//3D筁耾把计-膀非-膀非祘妓Α
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_CALC_BASE_MODE  =     12621;//3D筁耾把计-膀非-璸衡膀非家Α
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_MAX_TILT_ANGLE  =     12622;//3D筁耾把计-膀非-程渡弊à
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_SYS_NOISE_RANGE =     12623;//3D筁耾把计-膀非-╰参馒癟絛瞅
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_UPPER_RATIO     =     12624;//3D筁耾把计-膀非-絛瞅ゑㄒ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_LOWER_RATIO     =     12625;//3D筁耾把计-膀非-絛瞅ゑㄒ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_OFFSET_Z        =     12626;//3D筁耾把计-膀非-程Z禸熬畉秖
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_PLANE_RATIO_LSL =     12627;//3D筁耾把计-膀非-キ骸ì程ゑㄒ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_PLANE_RATIO_USL =     12628;//3D筁耾把计-膀非-キぃ禬筁ゑㄒ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_RANGE_RATIO_MIN =     12629;//3D筁耾把计-膀非-絛瞅ゑㄒ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_RANGE_RATIO_MAX =     12630;//3D筁耾把计-膀非-絛瞅ゑㄒ

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_FILTER_MODE     =     12631;//3D筁耾把计-膀非-耾猧家Α
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_FILTER_SIZE     =     12632;//3D筁耾把计-膀非-耾猧へ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_FILTER_INTER_CNT=     12633;//3D筁耾把计-膀非-耾猧舼Ω计
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_FILTER_PITCH    =     12634;//3D筁耾把计-膀非-耾猧丁禯
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_FILTER_USE_SIZE =     12635;//3D筁耾把计-膀非-耾猧ㄏノへ

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_OVER_HIGH_FILTER=     12636;//3D筁耾把计-膀非-筁蔼埃
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_OVER_LOW_FILTER =     12637;//3D筁耾把计-膀非-筁埃

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_BASE_USE_SIDE_MODE   =     12638;//3D筁耾把计-膀非-娩縵匡

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_LINK_INDEX        =   12640;//3D筁耾把计-戈-硈钡ま计
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_VOID_EXPAND_ENABLE=   12641;//3D筁耾把计-戈-礚翴耎币ノ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_VOID_EXPAND_SIZE  =   12642;//3D筁耾把计-戈-礚翴耎へ

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_INFO_TEXT         =   12645;//3D筁耾把计-戈-戈癟ゅ

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_FIRST_FILTER_MODE =   12646;//3D筁耾把计-戈-Ω耾猧家Α
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_FIRST_KERNEL_SIZE =   12647;//3D筁耾把计-戈-Ω耾猧へ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_FIRST_USE_SIZE    =   12648;//3D筁耾把计-戈-Ωㄏノへ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_FIRST_FILTER_PITCH=   12649;//3D筁耾把计-戈-Ω耾猧˙

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_HEIGHT_VAR_MODE  =    12651;//3D筁耾把计-戈-蔼跑钵筁耾家Α
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_HEIGHT_VAR_RANGE =    12652;//3D筁耾把计-戈-蔼跑钵筁耾絛瞅
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_HEIGHT_VAR_CHK_SIZE=  12663;//3D筁耾把计-戈-蔼跑钵絋粄へ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_HEIGHT_VAR_USE_SIZE = 12665;//3D筁耾把计-戈-蔼跑钵ㄏノへ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_HEIGHT_VAR_KER_SIZE = 12667;//3D筁耾把计-戈-蔼跑钵筁耾へ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_HEIGHT_VAR_REPEAT   = 12668;//3D筁耾把计-戈-蔼跑钵筁耾Ω计
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_HEIGHT_VAR_PITCH    = 12669;//3D筁耾把计-戈-蔼跑钵筁耾˙

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_OVER_LOW_MODE   =    12671;//3D筁耾把计-戈-筁筁耾家Α
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_OVER_LOW_RANGE  =    12672;//3D筁耾把计-戈-筁筁耾絛瞅
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_OVER_LOW_LIMIT  =    12673;//3D筁耾把计-戈-筁伐絛瞅
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_OVER_LOW_KER_SIZE=   12674;//3D筁耾把计-戈-筁耾猧へ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_OVER_LOW_USE_SIZE=   12675;//3D筁耾把计-戈-筁ㄏノへ

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_VOID_RECONTRUCT_ENABLE   =    12681;//3D筁耾把计-戈-礚翴币ノ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_VOID_RECONTRUCT_EXT_RANGE=    12682;//3D筁耾把计-戈-礚翴へ

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_FINAL_FILTER_MODE =    12691;//3D筁耾把计-戈-籹耾猧家Α
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_FINAL_KERNEL_SIZE =    12692;//3D筁耾把计-戈-籹耾猧へ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_FINAL_USE_SIZE    =    12693;//3D筁耾把计-戈-籹ㄏノへ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_FINAL_PITCH       =    12694;//3D筁耾把计-戈-籹耾猧˙

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_FINAL_FILTER_MODE2=    12696;//3D筁耾把计-戈-籹耾猧家Α-2
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_FINAL_KERNEL_SIZE2=    12697;//3D筁耾把计-戈-籹耾猧へ-2
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_FINAL_USE_SIZE2   =    12698;//3D筁耾把计-戈-籹ㄏノへ-2
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_DATA_FINAL_PITCH2      =    12699;//3D筁耾把计-戈-籹耾猧˙-2
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_COLOR_GROUP_SET_SECTION_START        =     12701;//┾︹竤钉把计跋丁-癬翴
const FILE_IO_ID FILE_IO_COLOR_GROUP_SET_SECTION_END          =     12719;//┾︹竤钉把计跋丁-沧翴

const FILE_IO_ID FILE_IO_COLOR_GROUP_SET_START                =     12702;//┾︹竤钉把计-癬翴
const FILE_IO_ID FILE_IO_COLOR_GROUP_SET_END                  =     12703;//┾︹竤钉把计-沧翴

const FILE_IO_ID FILE_IO_COLOR_GROUP_SET_NAME                 =     12704;//┾︹竤钉把计-竤钉嘿
const FILE_IO_ID FILE_IO_COLOR_GROUP_SET_COLOR_GROUP          =     12705;//┾︹竤钉把计-┾︹竤舱
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_DEFECT_ITEM_SECTION_START            =     12801;//峰搏兜ヘ把计跋丁-癬翴
const FILE_IO_ID FILE_IO_DEFECT_ITEM_SECTION_END              =     12899;//峰搏兜ヘ把计跋丁-沧翴

const FILE_IO_ID FILE_IO_DEFECT_ITEM_START                    =     12802;//峰搏兜ヘ把计-癬翴
const FILE_IO_ID FILE_IO_DEFECT_ITEM_END                      =     12803;//峰搏兜ヘ把计-沧翴

const FILE_IO_ID FILE_IO_DEFECT_ITEM_NONE                     =     12810;//峰搏兜ヘ把计-礚﹚竡
const FILE_IO_ID FILE_IO_DEFECT_ITEM_PAD_ALIGN                =     12811;//峰搏兜ヘ把计-瞜絃﹚
const FILE_IO_ID FILE_IO_DEFECT_ITEM_PART_ALIGN               =     12812;//峰搏兜ヘ把计-セ砰﹚
const FILE_IO_ID FILE_IO_DEFECT_ITEM_PAD_ADJUST               =     12813;//峰搏兜ヘ把计-瞜絃秸俱
const FILE_IO_ID FILE_IO_DEFECT_ITEM_LEAD_ADJUST              =     12814;//峰搏兜ヘ把计-恨竲秸俱

const FILE_IO_ID FILE_IO_DEFECT_ITEM_CLASS_CHECK              =     12818;//峰搏兜ヘ把计-摸絋粄
const FILE_IO_ID FILE_IO_DEFECT_ITEM_BASE_VALUE               =     12819;//峰搏兜ヘ把计-膀セ计

const FILE_IO_ID FILE_IO_DEFECT_ITEM_BODY_MISSING             =     12820;//峰搏兜ヘ把计-ン
const FILE_IO_ID FILE_IO_DEFECT_ITEM_BODY_OFFSET              =     12821;//峰搏兜ヘ把计-熬簿
const FILE_IO_ID FILE_IO_DEFECT_ITEM_BODY_TILT                =     12822;//峰搏兜ヘ把计-セ砰渡弊
const FILE_IO_ID FILE_IO_DEFECT_ITEM_BODY_POLARITY            =     12823;//峰搏兜ヘ把计-伐は
const FILE_IO_ID FILE_IO_DEFECT_ITEM_BODY_TURN_OVER           =     12824;//峰搏兜ヘ把计-はン
const FILE_IO_ID FILE_IO_DEFECT_ITEM_BODY_MOUNT               =     12825;//峰搏兜ヘ把计-杆禟
const FILE_IO_ID FILE_IO_DEFECT_ITEM_BODY_WRONG_CODE          =     12826;//峰搏兜ヘ把计-兵絏
const FILE_IO_ID FILE_IO_DEFECT_ITEM_BODY_WTRONG_TEXT         =     12827;//峰搏兜ヘ把计-ゅ
const FILE_IO_ID FILE_IO_DEFECT_ITEM_BODY_TOMBSTONE           =     12828;//峰搏兜ヘ把计-ミ窸
const FILE_IO_ID FILE_IO_DEFECT_ITEM_BODY_BILLBOARD           =     12829;//峰搏兜ヘ把计-凹ミ
const FILE_IO_ID FILE_IO_DEFECT_ITEM_BODY_DAMAGED             =     12830;//峰搏兜ヘ把计-瘆穕

const FILE_IO_ID FILE_IO_DEFECT_ITEM_SOLDER_POOR              =     12840;//峰搏兜ヘ把计-瞜奎ぃì
const FILE_IO_ID FILE_IO_DEFECT_ITEM_SOLDER_OPEN              =     12841;//峰搏兜ヘ把计-瞜奎瞜
const FILE_IO_ID FILE_IO_DEFECT_ITEM_SOLDER_PAD_EXPOSED       =     12842;//峰搏兜ヘ把计-簗簧
const FILE_IO_ID FILE_IO_DEFECT_ITEM_SOLDER_BRIDGE            =     12843;//峰搏兜ヘ把计-瞜奎祏隔
const FILE_IO_ID FILE_IO_DEFECT_ITEM_SOLDER_BEAD              =     12844;//峰搏兜ヘ把计-瞜奎奎痌
const FILE_IO_ID FILE_IO_DEFECT_ITEM_SOLDER_EXCESS            =     12845;//峰搏兜ヘ把计-瞜奎筁秖

const FILE_IO_ID FILE_IO_DEFECT_ITEM_LEAD_LIFTED              =     12850;//峰搏兜ヘ把计-ま竲录癬
const FILE_IO_ID FILE_IO_DEFECT_ITEM_LEAD_BENDED              =     12851;//峰搏兜ヘ把计-ま竲舠Ρ
const FILE_IO_ID FILE_IO_DEFECT_ITEM_LEAD_PROTRUDED           =     12853;//峰搏兜ヘ把计-ま竲

const FILE_IO_ID FILE_IO_DEFECT_ITEM_PAD_SCRATCH              =     12860;//峰搏兜ヘ把计-瞜絃端
const FILE_IO_ID FILE_IO_DEFECT_ITEM_FOREIGN_BODY             =     12861;//峰搏兜ヘ把计-钵

const FILE_IO_ID FILE_IO_DEFECT_ITEM_USER_DEFINE_01           =     12880;//峰搏兜ヘ把计-ㄏノ﹚竡-01
const FILE_IO_ID FILE_IO_DEFECT_ITEM_USER_DEFINE_02           =     12881;//峰搏兜ヘ把计-ㄏノ﹚竡-02
const FILE_IO_ID FILE_IO_DEFECT_ITEM_USER_DEFINE_03           =     12882;//峰搏兜ヘ把计-ㄏノ﹚竡-03
const FILE_IO_ID FILE_IO_DEFECT_ITEM_USER_DEFINE_04           =     12883;//峰搏兜ヘ把计-ㄏノ﹚竡-04
const FILE_IO_ID FILE_IO_DEFECT_ITEM_USER_DEFINE_05           =     12884;//峰搏兜ヘ把计-ㄏノ﹚竡-05
const FILE_IO_ID FILE_IO_DEFECT_ITEM_USER_DEFINE_06           =     12885;//峰搏兜ヘ把计-ㄏノ﹚竡-06
const FILE_IO_ID FILE_IO_DEFECT_ITEM_USER_DEFINE_07           =     12886;//峰搏兜ヘ把计-ㄏノ﹚竡-07
const FILE_IO_ID FILE_IO_DEFECT_ITEM_USER_DEFINE_08           =     12887;//峰搏兜ヘ把计-ㄏノ﹚竡-08
const FILE_IO_ID FILE_IO_DEFECT_ITEM_USER_DEFINE_09           =     12888;//峰搏兜ヘ把计-ㄏノ﹚竡-09
const FILE_IO_ID FILE_IO_DEFECT_ITEM_USER_DEFINE_10           =     12889;//峰搏兜ヘ把计-ㄏノ﹚竡-10
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_SIGMA_TEMP_SECTION_START             =     12901;//夹非畉把计跋丁-癬翴
const FILE_IO_ID FILE_IO_SIGMA_TEMP_SECTION_END               =     12919;//夹非畉把计跋丁-沧翴

const FILE_IO_ID FILE_IO_SIGMA_TEMP_START                     =     12902;//夹非畉把计-癬翴
const FILE_IO_ID FILE_IO_SIGMA_TEMP_END                       =     12903;//夹非畉把计-沧翴
const FILE_IO_ID FILE_IO_SIGMA_TEMP_ID                        =     12904;//夹非畉把计-絪腹
const FILE_IO_ID FILE_IO_SIGMA_TEMP_MAX                       =     12905;//夹非畉把计-程
const FILE_IO_ID FILE_IO_SIGMA_TEMP_MIN                       =     12906;//夹非畉把计-程
const FILE_IO_ID FILE_IO_SIGMA_TEMP_SUM                       =     12907;//夹非畉把计-羆㎝
const FILE_IO_ID FILE_IO_SIGMA_TEMP_SUM_SQRD                  =     12908;//夹非畉把计-キよ羆㎝
const FILE_IO_ID FILE_IO_SIGMA_TEMP_COUNT                     =     12909;//夹非畉把计-计秖
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_SIGMA_ITEM_SECTION_START             =     12921;//夹非畉兜ヘ跋丁-癬翴
const FILE_IO_ID FILE_IO_SIGMA_ITEM_SECTION_END               =     12939;//夹非畉兜ヘ跋丁-沧翴

const FILE_IO_ID FILE_IO_SIGMA_ITEM_START                     =     12922;//夹非畉兜ヘ-癬翴
const FILE_IO_ID FILE_IO_SIGMA_ITEM_END                       =     12923;//夹非畉兜ヘ-沧翴
const FILE_IO_ID FILE_IO_SIGMA_ITEM_NODE_OFFSET_X             =     12924;//夹非畉兜ヘ-竊翴-熬簿X
const FILE_IO_ID FILE_IO_SIGMA_ITEM_NODE_OFFSET_Y             =     12925;//夹非畉兜ヘ-竊翴-熬簿Y
const FILE_IO_ID FILE_IO_SIGMA_ITEM_NODE_SKEW_ANGLE           =     12926;//夹非畉兜ヘ-竊翴-熬簿à
const FILE_IO_ID FILE_IO_SIGMA_ITEM_NODE_BODY_HEIGHT          =     12927;//夹非畉兜ヘ-竊翴-セ砰蔼
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_GRR_SIGMA_ITEM_SECTION_START         =     12941;//Grr夹非畉兜ヘ跋丁-癬翴
const FILE_IO_ID FILE_IO_GRR_SIGMA_ITEM_SECTION_END           =     12959;//Grr夹非畉兜ヘ跋丁-沧翴

const FILE_IO_ID FILE_IO_GRR_SIGMA_ITEM_START                 =     12942;//Grr夹非畉兜ヘ-癬翴
const FILE_IO_ID FILE_IO_GRR_SIGMA_ITEM_END                   =     12943;//Grr夹非畉兜ヘ-沧翴
const FILE_IO_ID FILE_IO_GRR_SIGMA_ITEM_COUNT                 =     12944;//Grr夹非畉兜ヘ-计秖
const FILE_IO_ID FILE_IO_GRR_SIGMA_ITEM_BARCODE               =     12945;//Grr夹非畉兜ヘ-兵絏
const FILE_IO_ID FILE_IO_GRR_SIGMA_ITEM_SIGMA_ITEM_NODE       =     12949;//Grr夹非畉兜ヘ-夹非畉兜ヘ
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_SECTION_START_II     =     13001;//3D筁耾把计跋丁-癬翴
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_SECTION_END_II       =     13999;//3D筁耾把计跋丁-沧翴

const FILE_IO_ID FILE_IO_3D_BASE_PLANE_START_II               =     13002;//3D膀把计-癬翴//13002~13099
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_END_II                 =     13003;//3D膀把计-沧翴

const FILE_IO_ID FILE_IO_3D_BASE_PLANE_BASE_PROC_TYPE_II      =     13010;//3D膀把计-膀非祘妓Α
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_CALC_BASE_MODE_II      =     13011;//3D膀把计-璸衡膀非家Α
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_MAX_TILT_ANGLE_II      =     13012;//3D膀把计-程渡弊à
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_SYS_NOISE_RANGE_II     =     13013;//3D膀把计-╰参馒癟絛瞅
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_UPPER_RATIO_II         =     13014;//3D膀把计-絛瞅ゑㄒ
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_LOWER_RATIO_II         =     13015;//3D膀把计-絛瞅ゑㄒ
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_OFFSET_Z_II            =     13016;//3D膀把计-程Z禸熬畉秖
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_PLANE_RATIO_LSL_II     =     13017;//3D膀把计-キ骸ì程ゑㄒ
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_PLANE_RATIO_USL_II     =     13018;//3D膀把计-キぃ禬筁ゑㄒ
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_RANGE_RATIO_MIN_II     =     13019;//3D膀把计-絛瞅ゑㄒ
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_RANGE_RATIO_MAX_II     =     13020;//3D膀把计-絛瞅ゑㄒ
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_OVER_HIGH_FILTER_II    =     13021;//3D膀把计-筁蔼埃
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_OVER_LOW_FILTER_II     =     13022;//3D膀把计-筁埃
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_USE_SIDE_MODE_II       =     13023;//3D膀把计-娩縵匡
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_CLIP_ROTATED_OUTER     =     13024;//3D膀把计-ち埃弊à瞅
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_TOWARD_MODE            =     13025;//3D膀把计-绰家Α
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_USE_INNER_MODE         =     13026;//3D膀把计-ㄏノず场
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_AUTO_REGION_MODE       =     13027;//3D膀把计-笆跋办家Α

const FILE_IO_ID FILE_IO_3D_BASE_PLANE_2DMASK_BASE_ENABLE     =     13030;//3D膀把计-膀非2D綛竛-币ノ
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_2DMASK_FRAME_UNIQUE_ID =     13031;//3D膀把计-膀非2D綛竛-斑絏
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_2DMASK_COLOR_GRAOUP_INDEX=   13032;//3D膀把计-膀非2D綛竛-盡肅︹腹

const FILE_IO_ID FILE_IO_3D_BASE_PLANE_AUTO_REGION_MAX_GAP    =     13041;//3D膀把计-笆匡跋程畉禯

const FILE_IO_ID FILE_IO_3D_BASE_PLANE_BODY_OUTSIDE_WIDTH     =     13045;//3D膀把计-セ砰瞅糴-um
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_BODY_OUTSIDE_HEIGHT    =     13046;//3D膀把计-セ砰瞅蔼-um
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_BODY_OUTSIDE_ENABLED   =     13047;//3D膀把计-セ砰瞅币ノ

const FILE_IO_ID FILE_IO_3D_BASE_PLANE_FILTER_MODE_II         =     13071;//3D膀把计-耾猧家Α
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_FILTER_SIZE_II         =     13072;//3D膀把计-耾猧へ
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_FILTER_INTER_CNT_II    =     13073;//3D膀把计-耾猧舼Ω计
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_FILTER_PITCH_II        =     13074;//3D膀把计-耾猧丁禯
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_FILTER_USE_SIZE_II     =     13075;//3D膀把计-耾猧ㄏノへ

const FILE_IO_ID FILE_IO_3D_BASE_PLANE_2D_FILTER_MODE_II      =     13081;//3D膀把计-2D耾猧家Α
const FILE_IO_ID FILE_IO_3D_BASE_PLANE_2D_FILTER_SIZE_II      =     13082;//3D膀把计-2D耾猧へ

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_START_II             =     13101;//3D筁耾把计-癬翴//13101~13999
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_END_II               =     13102;//3D筁耾把计-沧翴

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_LINK_INDEX_II        =     13103;//3D筁耾把计-硈钡ま计
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_INFO_TEXT_II         =     13104;//3D筁耾把计-戈癟ゅ

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_HEIGHT_CORRECT_II    =     13110;//3D筁耾把计-蔼タ

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_VOID_EXPAND_ENABLE_II=     13121;//3D筁耾把计-礚翴耎币ノ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_VOID_EXPAND_SIZE_II  =     13122;//3D筁耾把计-礚翴耎へ

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FIRST_FILTER_MODE_II =     13151;//3D筁耾把计-Ω耾猧家Α
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FIRST_KERNEL_SIZE_II =     13152;//3D筁耾把计-Ω耾猧へ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FIRST_USE_SIZE_II    =     13153;//3D筁耾把计-Ωㄏノへ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FIRST_FILTER_PITCH_II=     13154;//3D筁耾把计-Ω耾猧˙
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FIRST_SEARCHON_II    =	    13155;//3D筁耾把计-戈-穓碝蔼币ノ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FIRST_ALPHAF_II	  =	    13156;//3D筁耾把计-戈-穓碝蔼
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FIRST_ALPHAS_II	  =	    13157;//3D筁耾把计-戈-ㄏノ蔼
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FIRST_ALPHAM_II	  =	    13158;//3D筁耾把计-戈-ㄏノō蔼
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FIRST_ALPHAI_II	  =	    13159;//3D筁耾把计-戈-ㄏノōη顶
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FIRST_OUTLIER_II	  =	    13160;//3D筁耾把计-戈-馒癟

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_MODE_II    =    13201;//3D筁耾把计-蔼跑钵筁耾家Α//13201~13249
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_RANGE_II   =    13202;//3D筁耾把计-蔼跑钵筁耾絛瞅
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_CHK_SIZE_II=    13203;//3D筁耾把计-蔼跑钵絋粄へ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_USE_SIZE_II=    13204;//3D筁耾把计-蔼跑钵ㄏノへ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_KER_SIZE_II=    13205;//3D筁耾把计-蔼跑钵筁耾へ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_REPEAT_II  =    13206;//3D筁耾把计-蔼跑钵筁耾Ω计
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_HEIGHT_VAR_PITCH_II   =    13207;//3D筁耾把计-蔼跑钵筁耾˙

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_OVER_LOW_MODE_II      =    13251;//3D筁耾把计-筁筁耾家Α//13251~13279
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_OVER_LOW_RANGE_II     =    13252;//3D筁耾把计-筁筁耾絛瞅
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_OVER_LOW_LIMIT_II     =    13253;//3D筁耾把计-筁伐絛瞅
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_OVER_LOW_KER_SIZE_II  =    13254;//3D筁耾把计-筁耾猧へ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_OVER_LOW_USE_SIZE_II  =    13255;//3D筁耾把计-筁ㄏノへ

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_VOID_RECONTRUCT_ENABLE_II=   13281;//3D筁耾把计-礚翴币ノ//13281~13299
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_VOID_RECONTRUCT_EXT_RANGE_II=13282;//3D筁耾把计-礚翴へ

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_FILTER_MODE_II =     13301;//3D筁耾把计-籹耾猧家Α//13301~13349
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_KERNEL_SIZE_II =     13302;//3D筁耾把计-籹耾猧へ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_USE_SIZE_II    =     13303;//3D筁耾把计-籹ㄏノへ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_PITCH_II       =     13304;//3D筁耾把计-籹耾猧˙
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_SEARCHON_II    =	    13305;//3D筁耾把计-穓碝蔼币ノ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAF_II	  =	    13306;//3D筁耾把计-穓碝蔼
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAS_II	  =	    13307;//3D筁耾把计-ㄏノ蔼
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAM_II	  =	    13308;//3D筁耾把计-ㄏノō蔼
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAI_II	  =	    13309;//3D筁耾把计-ㄏノōη顶
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_OUTLIER_II	  =	    13310;//3D筁耾把计-馒癟

const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_FILTER_MODE2_II=     13351;//3D筁耾把计-籹耾猧家Α-2//13351~13399
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_KERNEL_SIZE2_II=     13352;//3D筁耾把计-籹耾猧へ-2
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_USE_SIZE2_II   =     13353;//3D筁耾把计-籹ㄏノへ-2
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_PITCH2_II      =     13354;//3D筁耾把计-籹耾猧˙-2
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_SEARCHON2_II   =     13355;//3D筁耾把计-穓碝蔼币ノ
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAF2_II	  =     13356;//3D筁耾把计-穓碝蔼
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAS2_II	  =     13357;//3D筁耾把计-ㄏノ蔼
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAM2_II	  =     13358;//3D筁耾把计-ㄏノō蔼
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_ALPHAI2_II	  =     13359;//3D筁耾把计-ㄏノōη顶
const FILE_IO_ID FILE_IO_3D_NOISE_FILTER_FINAL_OUTLIER2_II    =     13360;//3D筁耾把计-馒癟
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_DOT_NODE_SECTION_BEGIN               =     14001;//Dot计秖-X
const FILE_IO_ID FILE_IO_DOT_NODE_SECTION_END                 =     14002;//Dot计秖-Y
const FILE_IO_ID FILE_IO_DOT_NODE_COUNT_X                     =     14003;//Dot计秖-X
const FILE_IO_ID FILE_IO_DOT_NODE_COUNT_Y                     =     14004;//Dot计秖-Y
const FILE_IO_ID FILE_IO_DOT_NODE_BEGIN                       =     14011;//Dot秨﹍
const FILE_IO_ID FILE_IO_DOT_NODE_END                         =     14012;//Dot挡
const FILE_IO_ID FILE_IO_DOT_NODE_INDEX_X                     =     14013;//Dotま计-X
const FILE_IO_ID FILE_IO_DOT_NODE_INDEX_Y                     =     14014;//Dotま计-Y
const FILE_IO_ID FILE_IO_DOT_NODE_CAD_POS_X                   =     14021;//Dot瞶阶畒夹-X
const FILE_IO_ID FILE_IO_DOT_NODE_CAD_POS_Y                   =     14022;//Dot瞶阶畒夹-Y
const FILE_IO_ID FILE_IO_DOT_NODE_STAGE_POS_X                 =     14031;//Dot诀畒夹-X
const FILE_IO_ID FILE_IO_DOT_NODE_STAGE_POS_Y                 =     14032;//Dot诀畒夹-Y
const FILE_IO_ID FILE_IO_DOT_NODE_OFFSET_X                    =     14041;//Dot熬畉-X
const FILE_IO_ID FILE_IO_DOT_NODE_OFFSET_Y                    =     14042;//Dot熬畉-Y

const FILE_IO_ID FILE_IO_DOT_NODE_RECT_LEFT                   =     14051;//Dot跋办-オ凹
const FILE_IO_ID FILE_IO_DOT_NODE_RECT_TOP                    =     14052;//Dot跋办-凹
const FILE_IO_ID FILE_IO_DOT_NODE_RECT_RIGHT                  =     14053;//Dot跋办-凹
const FILE_IO_ID FILE_IO_DOT_NODE_RECT_BOTTOM                 =     14054;//Dot跋办-凹

const FILE_IO_ID FILE_IO_DOT_NODE_RESULT_ID                   =     14081;//Dot挡狦絪腹
const FILE_IO_ID FILE_IO_DOT_NODE_SHAPE_MODE                  =     14082;//Dot家Α
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_PHASE_FACTOR_SECTION_BEGIN           =     14101;//蔼ゑㄒ计秖-X
const FILE_IO_ID FILE_IO_PHASE_FACTOR_SECTION_END             =     14102;//蔼ゑㄒ计秖-Y
const FILE_IO_ID FILE_IO_PHASE_FACTOR_COUNT_X                 =     14103;//蔼ゑㄒ计秖-X
const FILE_IO_ID FILE_IO_PHASE_FACTOR_COUNT_Y                 =     14104;//蔼ゑㄒ计秖-Y
const FILE_IO_ID FILE_IO_PHASE_FACTOR_TARGET_HEIGHT           =     14105;//蔼ゑㄒ遏砏蔼
const FILE_IO_ID FILE_IO_PHASE_FACTOR_TARGET_NO               =     14106;//蔼ゑㄒ遏砏蔼
const FILE_IO_ID FILE_IO_PHASE_FACTOR_BEGIN                   =     14111;//蔼ゑㄒ秨﹍
const FILE_IO_ID FILE_IO_PHASE_FACTOR_END                     =     14112;//蔼ゑㄒ挡
const FILE_IO_ID FILE_IO_PHASE_FACTOR_INDEX_X                 =     14113;//蔼ゑㄒま计-X
const FILE_IO_ID FILE_IO_PHASE_FACTOR_INDEX_Y                 =     14114;//蔼ゑㄒま计-Y
const FILE_IO_ID FILE_IO_PHASE_FACTOR_TARGET_NO_TEMP          =     14115;//蔼ゑㄒ遏砏絪腹
const FILE_IO_ID FILE_IO_PHASE_FACTOR_STAGE_POS_X             =     14121;//蔼ゑㄒ诀畒夹-X
const FILE_IO_ID FILE_IO_PHASE_FACTOR_STAGE_POS_Y             =     14122;//蔼ゑㄒ诀畒夹-Y
const FILE_IO_ID FILE_IO_PHASE_FACTOR_STAGE_POS_Z             =     14123;//蔼ゑㄒ诀畒夹-Z
const FILE_IO_ID FILE_IO_PHASE_FACTOR_IMAGE_POS_X             =     14131;//蔼ゑㄒ紇钩畒夹-X
const FILE_IO_ID FILE_IO_PHASE_FACTOR_IMAGE_POS_Y             =     14132;//蔼ゑㄒ紇钩畒夹-Y
const FILE_IO_ID FILE_IO_PHASE_FACTOR_IMAGE_POS_Z             =     14133;//蔼ゑㄒ紇钩畒夹-Z
const FILE_IO_ID FILE_IO_PHASE_FACTOR_PHASE_BASE              =     14141;//蔼ゑㄒ-膀非
const FILE_IO_ID FILE_IO_PHASE_FACTOR_HEIGHT_BASE             =     14142;//蔼ゑㄒ蔼-膀非
const FILE_IO_ID FILE_IO_PHASE_FACTOR_PHASE_TARGET            =     14143;//蔼ゑㄒ-顶蔼遏
const FILE_IO_ID FILE_IO_PHASE_FACTOR_HEIGHT_TARGET           =     14144;//蔼ゑㄒ蔼-顶蔼遏
const FILE_IO_ID FILE_IO_PHASE_FACTOR_PHASE_OFFSET            =     14145;//蔼ゑㄒ-熬畉
const FILE_IO_ID FILE_IO_PHASE_FACTOR_HEIGHT_OFFSET           =     14146;//蔼ゑㄒ蔼-熬畉

const FILE_IO_ID FILE_IO_PHASE_FACTOR_ROI_RECT_LEFT           =     14151;//蔼ゑㄒ穓碝跋办-オ凹
const FILE_IO_ID FILE_IO_PHASE_FACTOR_ROI_RECT_TOP            =     14152;//蔼ゑㄒ穓碝跋办-凹
const FILE_IO_ID FILE_IO_PHASE_FACTOR_ROI_RECT_RIGHT          =     14153;//蔼ゑㄒ穓碝跋办-凹
const FILE_IO_ID FILE_IO_PHASE_FACTOR_ROI_RECT_BOTTOM         =     14154;//蔼ゑㄒ穓碝跋办-凹

const FILE_IO_ID FILE_IO_PHASE_FACTOR_OBJ_RECT_LEFT           =     14161;//蔼ゑㄒ遏砏跋办-オ凹
const FILE_IO_ID FILE_IO_PHASE_FACTOR_OBJ_RECT_TOP            =     14162;//蔼ゑㄒ遏砏跋办-凹
const FILE_IO_ID FILE_IO_PHASE_FACTOR_OBJ_RECT_RIGHT          =     14163;//蔼ゑㄒ遏砏跋办-凹
const FILE_IO_ID FILE_IO_PHASE_FACTOR_OBJ_RECT_BOTTOM         =     14164;//蔼ゑㄒ遏砏跋办-凹
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_GROUND_EQUATION_SECTION_START        =     14901;//膀非よ祘Α把计跋丁-癬翴
const FILE_IO_ID FILE_IO_GROUND_EQUATION_SECTION_END          =     14999;//膀非よ祘Α把计跋丁-沧翴

const FILE_IO_ID FILE_IO_GROUND_EQUATION_START                =     14902;//膀非よ祘Α把计-癬翴
const FILE_IO_ID FILE_IO_GROUND_EQUATION_END                  =     14903;//膀非よ祘Α把计-沧翴
const FILE_IO_ID FILE_IO_GROUND_EQUATION_MODE                 =     14904;//膀非よ祘Α把计-家Α
const FILE_IO_ID FILE_IO_GROUND_EQUATION_PARAM                =     14980;//膀非よ祘Α把计-把计
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_MARK_SECTION_START                   =     15001;//疭紉翴把计跋丁-癬翴
const FILE_IO_ID FILE_IO_MARK_SECTION_END                     =     15999;//疭紉翴把计跋丁-沧翴

const FILE_IO_ID FILE_IO_MARK_START                           =     15002;//疭紉翴把计-癬翴
const FILE_IO_ID FILE_IO_MARK_END                             =     15003;//疭紉翴把计-沧翴
const FILE_IO_ID FILE_IO_MARK_UNIQUE_ID                       =     15004;//疭紉翴把计-斑絏
const FILE_IO_ID FILE_IO_MARK_INDEX_PROJECT                   =     15005;//疭紉翴把计-盡ま计-Debug
const FILE_IO_ID FILE_IO_MARK_PANEL_INDEX                     =     15006;//疭紉翴把计-俱狾ま计
const FILE_IO_ID FILE_IO_MARK_BOARD_INDEX                     =     15007;//疭紉翴把计-虫狾ま计
const FILE_IO_ID FILE_IO_MARK_ANGLE                           =     15008;//疭紉翴把计-à
const FILE_IO_ID FILE_IO_MARK_BODY_SIZE_CX                    =     15009;//疭紉翴把计-セ砰へ-糴-um
const FILE_IO_ID FILE_IO_MARK_BODY_SIZE_CY                    =     15010;//疭紉翴把计-セ砰へ-蔼-um
const FILE_IO_ID FILE_IO_MARK_ROI_SIZE_CX                     =     15011;//疭紉翴把计-穓碝へ-糴-um
const FILE_IO_ID FILE_IO_MARK_ROI_SIZE_CY                     =     15012;//疭紉翴把计-穓碝へ-蔼-um
const FILE_IO_ID FILE_IO_MARK_CAD_POS_X                       =     15013;//疭紉翴把计-Cad畒夹-X-um
const FILE_IO_ID FILE_IO_MARK_CAD_POS_Y                       =     15014;//疭紉翴把计-Cad畒夹-Y-um
const FILE_IO_ID FILE_IO_MARK_STAGE_POS_X                     =     15015;//疭紉翴把计-诀畒夹-X-um
const FILE_IO_ID FILE_IO_MARK_STAGE_POS_Y                     =     15016;//疭紉翴把计-诀畒夹-Y-um
const FILE_IO_ID FILE_IO_MARK_STAGE_POS_Z                     =     15017;//疭紉翴把计-诀畒夹-Z-um
const FILE_IO_ID FILE_IO_MARK_OBJ_UUID                        =     15018;//疭紉翴把计-OBJ-UUID
const FILE_IO_ID FILE_IO_MARK_BYPASSED                        =     15019;//疭紉翴把计-ぃ浪代
const FILE_IO_ID FILE_IO_MARK_DISTRICT_ID                     =     15021;//疭紉翴把计-だ琿絪腹
const FILE_IO_ID FILE_IO_MARK_GROUP_ID                        =     15022;//疭紉翴把计-竤舱絪腹
const FILE_IO_ID FILE_IO_MARK_LOCAL_BASE_PLANE_ID             =     15023;//疭紉翴把计-Ы场膀非絪腹
const FILE_IO_ID FILE_IO_MARK_TYPE_MODE                       =     15025;//疭紉翴把计-妓Α家Α
const FILE_IO_ID FILE_IO_MARK_LOCAL_BASE_RESULT_NX            =     15031;//疭紉翴把计-Ы场膀非挡狦-nX//20240327奔
const FILE_IO_ID FILE_IO_MARK_LOCAL_BASE_RESULT_NY            =     15032;//疭紉翴把计-Ы场膀非挡狦-nY//20240327奔
const FILE_IO_ID FILE_IO_MARK_LOCAL_BASE_RESULT_NZ            =     15033;//疭紉翴把计-Ы场膀非挡狦-nZ//20240327奔
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_MARK_3D_NOISE_FILTER_NODE            =     15800;//疭紉翴把计-3D馒癟筁耾把计
const FILE_IO_ID FILE_IO_MARK_MODEL_NODE                      =     15801;//疭紉翴把计-家舱把计
const FILE_IO_ID FILE_IO_MARK_LOCAL_GROUND_RESULT             =     15802;//疭紉翴把计-Ы场膀非挡狦
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_PART_GROUP_SECTION_START             =     16001;//竤舱把计跋丁-癬翴
const FILE_IO_ID FILE_IO_PART_GROUP_SECTION_END               =     16899;//竤舱把计跋丁-沧翴

const FILE_IO_ID FILE_IO_PART_GROUP_START                     =     16002;//竤舱把计-癬翴
const FILE_IO_ID FILE_IO_PART_GROUP_END                       =     16003;//竤舱把计-沧翴
const FILE_IO_ID FILE_IO_PART_GROUP_UNIQUE_ID                 =     16004;//竤舱把计-斑絏
const FILE_IO_ID FILE_IO_PART_GROUP_INDEX                     =     16005;//竤舱把计-盡ま计-Debug

const FILE_IO_ID FILE_IO_PART_GROUP_OBJ_UUID                  =     16008;//竤舱把计-OBJ-UUID
const FILE_IO_ID FILE_IO_PART_GROUP_DISTRICT_ID               =     16009;//竤舱把计-だ琿絪腹
const FILE_IO_ID FILE_IO_PART_GROUP_GROUP_ID                  =     16010;//竤舱把计-竤舱絪腹
const FILE_IO_ID FILE_IO_PART_GROUP_GROUP_NAME                =     16011;//竤舱把计-竤舱嘿
const FILE_IO_ID FILE_IO_PART_GROUP_GROUP_MODE                =     16012;//竤舱把计-竤舱家Α
const FILE_IO_ID FILE_IO_PART_GROUP_IN_ONE_BOARD              =     16013;//竤舱把计-虫狾ず

const FILE_IO_ID FILE_IO_PART_GROUP_COLINEARITY_MODE          =     16101;//竤舱把计-絬┦家Α
const FILE_IO_ID FILE_IO_PART_GROUP_COLINEARITY_GAP_USL       =     16102;//竤舱把计-絬┦熬畉
const FILE_IO_ID FILE_IO_PART_GROUP_COLINEARITY_GAP_LSL       =     16103;//竤舱把计-絬┦熬畉
const FILE_IO_ID FILE_IO_PART_GROUP_COLINEARITY_GAP_STD       =     16104;//竤舱把计-絬┦熬畉夹非
const FILE_IO_ID FILE_IO_PART_GROUP_COLINEARITY_TARGET_MODE   =     16105;//竤舱把计-絬┦ヘ夹家Α

const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_USL_X        =     16151;//竤舱把计-癸畒夹熬畉-X
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_LSL_X        =     16152;//竤舱把计-癸畒夹熬畉-X
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_USL_Y        =     16153;//竤舱把计-癸畒夹熬畉-Y
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_LSL_Y        =     16154;//竤舱把计-癸畒夹熬畉-Y
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_USL_L        =     16155;//竤舱把计-癸畒夹熬畉-L
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_LSL_L        =     16156;//竤舱把计-癸畒夹熬畉-L
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_ENB_X        =     16157;//竤舱把计-癸畒夹熬畉币ノ-X
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_ENB_Y        =     16158;//竤舱把计-癸畒夹熬畉币ノ-Y
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_ENB_L        =     16159;//竤舱把计-癸畒夹熬畉币ノ-L
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_STD_ENB      =     16160;//竤舱把计-癸畒夹夹非-币ノ
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_STD_X        =     16161;//竤舱把计-癸畒夹夹非-X
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_STD_Y        =     16162;//竤舱把计-癸畒夹夹非-Y
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_STD_L        =     16163;//竤舱把计-癸畒夹夹非-L
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_ADD_X        =     16166;//竤舱把计-癸畒夹熬畉糤-X
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_ADD_Y        =     16167;//竤舱把计-癸畒夹熬畉糤-Y
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_ENB_ABS      =     16170;//竤舱把计-癸畒夹荡癸币ノ
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_SCALE_X      =     16171;//竤舱把计-癸畒夹熬畉瞯-X
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_SCALE_Y      =     16172;//竤舱把计-癸畒夹熬畉瞯-Y
const FILE_IO_ID FILE_IO_PART_GROUP_DISTANCE_GAP_SCALE_L      =     16173;//竤舱把计-癸畒夹熬畉瞯-L

const FILE_IO_ID FILE_IO_PART_GROUP_NODE_PARAM_1              =     16501;//竤舱把计-竊翴把计1
const FILE_IO_ID FILE_IO_PART_GROUP_NODE_PARAM_2              =     16502;//竤舱把计-竊翴把计2
const FILE_IO_ID FILE_IO_PART_GROUP_NODE_PARAM_LIST           =     16503;//竤舱把计-竊翴把计
const FILE_IO_ID FILE_IO_PART_GROUP_NODE_PARAM_3              =     16504;//竤舱把计-竊翴把计3
const FILE_IO_ID FILE_IO_PART_GROUP_NODE_PARAM_4              =     16505;//竤舱把计-竊翴把计4
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_PART_GROUP_NODE_SECTION_START        =     16901;//竤舱竊翴把计跋丁-癬翴
const FILE_IO_ID FILE_IO_PART_GROUP_NODE_SECTION_END          =     16999;//竤舱竊翴把计跋丁-沧翴

const FILE_IO_ID FILE_IO_PART_GROUP_NODE_START                =     16902;//竤竊翴舱把计-癬翴
const FILE_IO_ID FILE_IO_PART_GROUP_NODE_END                  =     16903;//竤舱竊翴把计-沧翴
const FILE_IO_ID FILE_IO_PART_GROUP_NODE_COMPONENT_INDEX      =     16904;//竤竊翴舱把计-箂ンま计
const FILE_IO_ID FILE_IO_PART_GROUP_NODE_MODEL_WND_INDEX      =     16905;//竤舱竊翴把计-浪代ま计
const FILE_IO_ID FILE_IO_PART_GROUP_NODE_MAP_DIR_MODE         =     16906;//竤竊翴舱把计-畒夹锣传よ

const FILE_IO_ID FILE_IO_PART_GROUP_NODE_USER_MAP_CAD_ENABLE  =     16921;//竤舱竊翴把计-璹琈甮畒夹币ノ
const FILE_IO_ID FILE_IO_PART_GROUP_NODE_USER_MAP_CAD_POS_X   =     16922;//竤舱竊翴把计-璹琈甮畒夹-X
const FILE_IO_ID FILE_IO_PART_GROUP_NODE_USER_MAP_CAD_POS_Y   =     16923;//竤舱竊翴把计-璹琈甮畒夹-Y
const FILE_IO_ID FILE_IO_PART_GROUP_NODE_USER_MAP_CAD_DIS_L   =     16924;//竤舱竊翴把计-璹琈甮禯瞒-L
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_BOX_SECTION_START                    =     20001;//膀セ把计跋丁-癬翴
const FILE_IO_ID FILE_IO_BOX_SECTION_END                      =     20999;//膀セ把计跋丁-沧翴

const FILE_IO_ID FILE_IO_BOX_START                            =     20002;//膀セ把计-癬翴
const FILE_IO_ID FILE_IO_BOX_END                              =     20003;//膀セ把计-沧翴
const FILE_IO_ID FILE_IO_BOX_TOWARD                           =     20004;//膀セ把计-绰
const FILE_IO_ID FILE_IO_BOX_SHAPE_MODE                       =     20005;//膀セ把计-
const FILE_IO_ID FILE_IO_BOX_POS_X                            =     20006;//膀セ把计-竚-X	
const FILE_IO_ID FILE_IO_BOX_POS_Y                            =     20007;//膀セ把计-竚-Y
const FILE_IO_ID FILE_IO_BOX_SIZE_X                           =     20008;//膀セ把计-へ-X	
const FILE_IO_ID FILE_IO_BOX_SIZE_Y                           =     20009;//膀セ把计-へ-Y
const FILE_IO_ID FILE_IO_BOX_ANGLE                            =     20010;//膀セ把计-à	
const FILE_IO_ID FILE_IO_BOX_OBJ_UUID                         =     20011;//膀セ把计-UUID
const FILE_IO_ID FILE_IO_BOX_SHAPE_PARAM_1                    =     20012;//膀セ把计-把计-1
const FILE_IO_ID FILE_IO_BOX_SHAPE_PARAM_2                    =     20013;//膀セ把计-把计-2
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_WND_SECTION_START                    =     21001;//家舱把计跋丁-癬翴
const FILE_IO_ID FILE_IO_WND_SECTION_END                      =     21999;//家舱把计跋丁-沧翴

const FILE_IO_ID FILE_IO_WND_START                            =     21002;//家舱把计-癬翴
const FILE_IO_ID FILE_IO_WND_END                              =     21003;//家舱把计-沧翴
const FILE_IO_ID FILE_IO_WND_GROUP_ID                         =     21004;//家舱把计-竤舱絪腹
const FILE_IO_ID FILE_IO_WND_BAND_ID                          =     21005;//家舱把计-Ω竤舱絪腹
const FILE_IO_ID FILE_IO_WND_ISOLATED                         =     21006;//家舱把计-琌筳瞒
const FILE_IO_ID FILE_IO_WND_DEFECT_ID_OLD                    =     21007;//家舱把计-峰搏絏-侣絏
const FILE_IO_ID FILE_IO_WND_LAND_INDEX                       =     21008;//家舱把计-疭紉ま计
const FILE_IO_ID FILE_IO_WND_FOLLOW_MODE                      =     21009;//家舱把计-蛤繦簿笆家Α
const FILE_IO_ID FILE_IO_WND_ENABLED                          =     21010;//家舱把计-币笆

const FILE_IO_ID FILE_IO_WND_RGN_LINK_AUTO                    =     21011;//家舱把计-絛瞅笆硈笆
const FILE_IO_ID FILE_IO_WND_RGN_LINK_MODE                    =     21012;//家舱把计-絛瞅笆家Α
const FILE_IO_ID FILE_IO_WND_RGN_LINK_RATIO_X                 =     21013;//家舱把计-絛瞅Uよゑㄒ-%
const FILE_IO_ID FILE_IO_WND_RGN_LINK_RATIO_Y                 =     21014;//家舱把计-絛瞅Vよゑㄒ-%
const FILE_IO_ID FILE_IO_WND_RGN_LINK_GROUP_ID                =     21015;//家舱把计-竕﹚疭紉竤舱絪腹

const FILE_IO_ID FILE_IO_WND_EXTEND_RANGE_X                   =     21021;//家舱把计-耎絛瞅-X-um
const FILE_IO_ID FILE_IO_WND_EXTEND_RANGE_Y                   =     21022;//家舱把计-耎絛瞅-Y-um
const FILE_IO_ID FILE_IO_WND_EXTEND_ENABLED                   =     21023;//家舱把计-耎絛瞅-琌ㄏノ

const FILE_IO_ID FILE_IO_WND_LOGIC_TYPE                       =     21030;//家舱把计-呸胯妓Α
const FILE_IO_ID FILE_IO_WND_LOGIC_GROUP_ID                   =     21031;//家舱把计-呸胯竤舱絪腹

const FILE_IO_ID FILE_IO_WND_OBJ_UUID                         =     21040;//家舱把计-OBJ-UUID
const FILE_IO_ID FILE_IO_WND_CONSTRAIN_MODE                   =     21041;//家舱把计-玗家Α
const FILE_IO_ID FILE_IO_WND_MODEL_MASK                       =     21042;//家舱把计-家舱綛竛
const FILE_IO_ID FILE_IO_WND_DEFECT_ID                        =     21043;//家舱把计-峰搏絏
const FILE_IO_ID FILE_IO_WND_SYNC_MOVE_MODE                   =     21044;//家舱把计-ぃ疭紉竲硈笆家Α
const FILE_IO_ID FILE_IO_WND_DEFECT_GROUP_ID                  =     21045;//家舱把计-峰搏竤舱絪腹
const FILE_IO_ID FILE_IO_WND_INDEX                            =     21046;//家舱把计-ま计
const FILE_IO_ID FILE_IO_WND_CLASS_ID                         =     21047;//家舱把计-摸絪腹

const FILE_IO_ID FILE_IO_WND_BOX_NODE                         =     21801;//家舱把计-膀セ把计
//const FILE_IO_ID FILE_IO_WND_EXTEND_BOX_NODE                =     21802;//家舱把计-耎把计
const FILE_IO_ID FILE_IO_WND_ROI_NODE                         =     21805;//家舱把计-把计
const FILE_IO_ID FILE_IO_WND_MASK_NODE_BOX                    =     21806;//家舱把计-綛竛把计-侣蹿Α
const FILE_IO_ID FILE_IO_WND_MASK_NODE_WND                    =     21807;//家舱把计-綛竛把计-穝蹿Α//20190426
const FILE_IO_ID FILE_IO_WND_ALG_PARAM                        =     21811;//家舱把计-簍衡猭把计
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_LAND_SECTION_START                   =     22001;//疭紉把计跋丁-癬翴
const FILE_IO_ID FILE_IO_LAND_SECTION_END                     =     22999;//疭紉把计跋丁-沧翴

const FILE_IO_ID FILE_IO_LAND_START                           =     22002;//疭紉把计-癬翴
const FILE_IO_ID FILE_IO_LAND_END                             =     22003;//疭紉把计-沧翴
const FILE_IO_ID FILE_IO_LAND_TYPE                            =     22004;//疭紉把计-妓Α
const FILE_IO_ID FILE_IO_LAND_GROUP_ID                        =     22005;//疭紉把计-竤舱絪腹
const FILE_IO_ID FILE_IO_LAND_ALIGN_ID                        =     22006;//疭紉把计-癸霍絪腹
const FILE_IO_ID FILE_IO_LAND_FIRST_ONE                       =     22007;//疭紉把计-虫凹材竚
const FILE_IO_ID FILE_IO_LAND_LAST_ONE                        =     22008;//疭紉把计-虫凹材ソ竚
const FILE_IO_ID FILE_IO_LAND_OBJ_UUID                        =     22009;//疭紉把计-OBJ-UUID
const FILE_IO_ID FILE_IO_LAND_INCLUDE_PART_ALIGN              =     22010;//疭紉把计-セ砰﹚
const FILE_IO_ID FILE_IO_LAND_INCLUDE_PAD_ALIGN               =     22011;//疭紉把计-瞜絃﹚

const FILE_IO_ID FILE_IO_LAND_LEAD_SIZE_X                     =     22021;//疭紉把计-ま竲へ-X
const FILE_IO_ID FILE_IO_LAND_LEAD_SIZE_Y                     =     22022;//疭紉把计-ま竲へ-Y
const FILE_IO_ID FILE_IO_LAND_LEAD_HEIGHT                     =     22023;//疭紉把计-ま竲蔼

const FILE_IO_ID FILE_IO_LAND_LEAD_TIP_SIZE_X                 =     22031;//疭紉把计-ま竲玡狠へ-X
const FILE_IO_ID FILE_IO_LAND_LEAD_TIP_SIZE_Y                 =     22032;//疭紉把计-ま竲玡狠へ-Y
const FILE_IO_ID FILE_IO_LAND_LEAD_TIP_HEIGHT                 =     22033;//疭紉把计-ま竲玡狠蔼

const FILE_IO_ID FILE_IO_LAND_LEAD_SHOULDER_SIZE_X            =     22041;//疭紉把计-ま竲场へ-X
const FILE_IO_ID FILE_IO_LAND_LEAD_SHOULDER_SIZE_Y            =     22042;//疭紉把计-ま竲场へ-Y
const FILE_IO_ID FILE_IO_LAND_LEAD_SHOULDER_HEIGHT            =     22043;//疭紉把计-ま竲场蔼

const FILE_IO_ID FILE_IO_PAD_BOX                              =     22801;//疭紉把计-瞜絃
const FILE_IO_ID FILE_IO_LEAD_BOX                             =     22802;//疭紉把计-筿伐
const FILE_IO_ID FILE_IO_LEAD_TIP_BOX                         =     22803;//疭紉把计-ま竲玡狠
const FILE_IO_ID FILE_IO_LEAD_SHOULDER_BOX                    =     22804;//疭紉把计-ま竲场
const FILE_IO_ID FILE_IO_BODY_EDGE_BOX                        =     22805;//疭紉把计-セ砰娩絫
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_WND_ROI_SECTION_START                =     23001;//把计跋丁-癬翴
const FILE_IO_ID FILE_IO_WND_ROI_SECTION_END                  =     23999;//把计跋丁-沧翴

const FILE_IO_ID FILE_IO_WND_ROI_START                        =     23002;//把计-癬翴
const FILE_IO_ID FILE_IO_WND_ROI_END                          =     23003;//把计-沧翴
const FILE_IO_ID FILE_IO_WND_ROI_ENABLED                      =     23004;//把计-币笆
const FILE_IO_ID FILE_IO_WND_ROI_BOX_NODE                     =     23801;//把计-膀セ把计
const FILE_IO_ID FILE_IO_WND_ROI_BINARY_PARAM                 =     23811;//把计-2て把计
const FILE_IO_ID FILE_IO_WND_ROI_SELF_FRAME_ENABLED           =     23812;//把计-Τ礶币ノ
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_WND_MASK_SECTION_START               =     24001;//綛竛把计跋丁-癬翴
const FILE_IO_ID FILE_IO_WND_MASK_SECTION_END                 =     24999;//綛竛把计跋丁-沧翴

const FILE_IO_ID FILE_IO_WND_MASK_START                       =     23002;//綛竛把计-癬翴
const FILE_IO_ID FILE_IO_WND_MASK_END                         =     23003;//綛竛把计-沧翴
const FILE_IO_ID FILE_IO_WND_MASK_ERASE_MODE                  =     23004;//綛竛把计-睲埃家Α
const FILE_IO_ID FILE_IO_WND_MASK_BOX_NODE                    =     23801;//綛竛把计-膀セ把计
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_BINARY_PARAM_SECTION_START           =     29001;//2て把计跋丁-癬翴-29001
const FILE_IO_ID FILE_IO_BINARY_PARAM_SECTION_END             =     29099;//2て把计跋丁-沧翴-29099

const FILE_IO_ID FILE_IO_BINARY_PARAM_START                   =     29002;//2て把计-癬翴
const FILE_IO_ID FILE_IO_BINARY_PARAM_END                     =     29003;//2て把计-沧翴

const FILE_IO_ID FILE_IO_BINARY_PARAM_FRAME_UNIQUE_ID         =     29004;//2て把计-Frame斑絏
const FILE_IO_ID FILE_IO_BINARY_PARAM_IMAGE_SOURCE_MODE       =     29005;//2て把计-紇钩ㄓ方
const FILE_IO_ID FILE_IO_BINARY_PARAM_BINARY_MODE             =     29006;//2て把计-2て家Α
const FILE_IO_ID FILE_IO_BINARY_PARAM_BINARY_INVERT           =     29007;//2て把计-2ては
const FILE_IO_ID FILE_IO_BINARY_PARAM_MASK_FUNC_MODE          =     29008;//2て把计-綛竛家Α

const FILE_IO_ID FILE_IO_BINARY_PARAM_SYNTHESIS_WR            =     29011;//2て把计-Θ紇钩-︹
const FILE_IO_ID FILE_IO_BINARY_PARAM_SYNTHESIS_WG            =     29012;//2て把计-Θ紇钩-厚︹
const FILE_IO_ID FILE_IO_BINARY_PARAM_SYNTHESIS_WB            =     29013;//2て把计-Θ紇钩-屡︹

const FILE_IO_ID FILE_IO_BINARY_PARAM_FIXED_THRESHOLD_HIGH    =     29014;//2て把计-㏕﹚恢-
const FILE_IO_ID FILE_IO_BINARY_PARAM_FIXED_THRESHOLD_LOW     =     29015;//2て把计-㏕﹚恢-

const FILE_IO_ID FILE_IO_BINARY_PARAM_DYNAMIC_THRESHOLD_RATIO =     29016;//2て把计-笆篈ゑㄒ
const FILE_IO_ID FILE_IO_BINARY_PARAM_RELATIVE_AVE_THRESHOLD_ABOVE= 29017;//2て把计-癸キА恢-
const FILE_IO_ID FILE_IO_BINARY_PARAM_RELATIVE_AVE_THRESHOLD_BELOW= 29018;//2て把计-癸キА恢-
const FILE_IO_ID FILE_IO_BINARY_PARAM_COLOR_FILTER_LINK_INDEX =     29019;//2て把计-眒︹竤舱硈笆腹
const FILE_IO_ID FILE_IO_BINARY_PARAM_GRAY_GAIN_ENB           =     29020;//2て把计-η顶糤痲币ノ
const FILE_IO_ID FILE_IO_BINARY_PARAM_GRAY_GAIN_VALUE         =     29021;//2て把计-η顶糤痲计

const FILE_IO_ID FILE_IO_BINARY_PARAM_ADAPTIVE_THRESHOLD_GAP  =     29022;//2て把计-続莱Α恢丁禯
const FILE_IO_ID FILE_IO_BINARY_PARAM_ADAPTIVE_THRESHOLD_CALC_SIZE= 29023;//2て把计-続莱Α恢璸衡へ
const FILE_IO_ID FILE_IO_BINARY_PARAM_GRAY_INVERT             =     29024;//2て把计-η顶は-璽

const FILE_IO_ID FILE_IO_BINARY_PARAM_RATIO_THRESHOLD_TARGET  =     29025;//2て把计-ゑㄒ恢-ヘ夹
const FILE_IO_ID FILE_IO_BINARY_PARAM_RATIO_THRESHOLD_HIGH    =     29026;//2て把计-ゑㄒ恢-ゑㄒ
const FILE_IO_ID FILE_IO_BINARY_PARAM_RATIO_THRESHOLD_LOW     =     29027;//2て把计-ゑㄒ恢-ゑㄒ

const FILE_IO_ID FILE_IO_BINARY_PARAM_RELATIVE_AVE_THRESHOLD_BIAS=  29028;//2て把计-癸キА恢-干纕

const FILE_IO_ID FILE_IO_BINARY_PARAM_EDGE_ENHANCE_MODE       =     29050;//2て把计-娩絫眏て家Α
const FILE_IO_ID FILE_IO_BINARY_PARAM_EDGE_ENHANCE_FILTER_1   =     29051;//2て把计-娩絫筁耾竤舱-1
const FILE_IO_ID FILE_IO_BINARY_PARAM_EDGE_ENHANCE_FILTER_2   =     29052;//2て把计-娩絫筁耾竤舱-2

const FILE_IO_ID FILE_IO_BINARY_PARAM_GRAY_FILTER_1           =     29061;//2て把计-η顶筁耾竤舱-1
const FILE_IO_ID FILE_IO_BINARY_PARAM_GRAY_FILTER_2           =     29062;//2て把计-η顶筁耾竤舱-2

const FILE_IO_ID FILE_IO_BINARY_PARAM_BINARY_FILTER_1         =     29071;//2て把计-馒癟筁耾竤舱-1
const FILE_IO_ID FILE_IO_BINARY_PARAM_BINARY_FILTER_2         =     29072;//2て把计-馒癟筁耾竤舱-2

const FILE_IO_ID FILE_IO_BINARY_PARAM_COLOR_FILTER_GROUP      =     29081;//2て把计-眒︹筁耾竤舱

/*
	bool                       FrameSpaceEnabled;//3Dㄏノ	
	NOISE_FILTER_MODE          NosieFilterMode;
*/
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_PATTERN_PARAM_SECTION_START          =     29101;//妓狾把计跋丁-癬翴-29101
const FILE_IO_ID FILE_IO_PATTERN_PARAM_SECTION_END            =     29199;//妓狾把计跋丁-沧翴-29199

const FILE_IO_ID FILE_IO_PATTERN_PARAM_START                  =     29102;//妓狾把计-癬翴
const FILE_IO_ID FILE_IO_PATTERN_PARAM_END                    =     29103;//妓狾把计-沧翴

const FILE_IO_ID FILE_IO_PATTERN_PARAM_PAT_TEXT               =     29104;//妓狾把计-妓狾ゅ
const FILE_IO_ID FILE_IO_PATTERN_PARAM_PAT_SIMILAR_TEXT       =     29105;//妓狾把计-妓狾ゅ

const FILE_IO_ID FILE_IO_PATTERN_PARAM_ROI_LIST_L             =     29171;//妓狾把计-竚把计-绰オ
const FILE_IO_ID FILE_IO_PATTERN_PARAM_ROI_LIST_T             =     29172;//妓狾把计-竚把计-绰
const FILE_IO_ID FILE_IO_PATTERN_PARAM_ROI_LIST_R             =     29173;//妓狾把计-竚把计-绰
const FILE_IO_ID FILE_IO_PATTERN_PARAM_ROI_LIST_B             =     29174;//妓狾把计-竚把计-绰

const FILE_IO_ID FILE_IO_PATTERN_PARAM_BINARY_PARAM           =     29181;//妓狾把计-2て把计
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_PATTERN_ROI_SECTION_START            =     29201;//妓狾把计跋丁-癬翴-29201
const FILE_IO_ID FILE_IO_PATTERN_ROI_SECTION_END              =     29299;//妓狾把计跋丁-沧翴-29299

const FILE_IO_ID FILE_IO_PATTERN_ROI_START                    =     29202;//妓狾把计-癬翴
const FILE_IO_ID FILE_IO_PATTERN_ROI_END                      =     29203;//妓狾把计-沧翴

const FILE_IO_ID FILE_IO_PATTERN_ROI_LEFT                     =     29211;//妓狾把计-オ
const FILE_IO_ID FILE_IO_PATTERN_ROI_TOP                      =     29212;//妓狾把计-
const FILE_IO_ID FILE_IO_PATTERN_ROI_RIGHT                    =     29213;//妓狾把计-
const FILE_IO_ID FILE_IO_PATTERN_ROI_BOTTOM                   =     29214;//妓狾把计-
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_ALG_PARAM_SECTION_START              =     30001;//簍衡猭把计跋丁-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_SECTION_END                =     39999;//簍衡猭把计跋丁-沧翴

const FILE_IO_ID FILE_IO_ALG_PARAM_START                      =     30002;//簍衡猭把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_END                        =     30003;//簍衡猭把计-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_ALG_TYPE                   =     30004;//簍衡猭把计-簍衡猭妓Α
const FILE_IO_ID FILE_IO_ALG_PARAM_GROUP_ID                   =     30005;//簍衡猭把计-竤舱絪腹
const FILE_IO_ID FILE_IO_ALG_PARAM_SAVE_DEFECT_IMAGE_ENABLED  =     30006;//簍衡猭把计-峰搏瓜币ノ

const FILE_IO_ID FILE_IO_ALG_PARAM_BASE_VALUE_ENABLED         =     30010;//簍衡猭把计-膀非币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BASE_VALUE_GROUP_ID        =     30011;//簍衡猭把计-膀非竤舱絪腹

const FILE_IO_ID FILE_IO_ALG_PARAM_OFFSET_X_USL               =     30101;//簍衡猭把计-熬簿-X
const FILE_IO_ID FILE_IO_ALG_PARAM_OFFSET_X_LSL               =     30102;//簍衡猭把计-熬簿-X
const FILE_IO_ID FILE_IO_ALG_PARAM_OFFSET_X_ENABLED           =     30103;//簍衡猭把计-熬簿币ノ-X

const FILE_IO_ID FILE_IO_ALG_PARAM_OFFSET_Y_USL               =     30111;//簍衡猭把计-熬簿-Y
const FILE_IO_ID FILE_IO_ALG_PARAM_OFFSET_Y_LSL               =     30112;//簍衡猭把计-熬簿-Y
const FILE_IO_ID FILE_IO_ALG_PARAM_OFFSET_Y_ENABLED           =     30113;//簍衡猭把计-熬簿币ノ-Y

const FILE_IO_ID FILE_IO_ALG_PARAM_SKEW_ANGLE_USL             =     30121;//簍衡猭把计-熬簿-à
const FILE_IO_ID FILE_IO_ALG_PARAM_SKEW_ANGLE_LSL             =     30122;//簍衡猭把计-熬簿-à
const FILE_IO_ID FILE_IO_ALG_PARAM_SKEW_ANGLE_ENABLED         =     30123;//簍衡猭把计-熬簿币ノ-à

const FILE_IO_ID FILE_IO_ALG_PARAM_SCALE_USL                  =     30131;//簍衡猭把计-熬簿-罽
const FILE_IO_ID FILE_IO_ALG_PARAM_SCALE_LSL                  =     30132;//簍衡猭把计-熬簿-罽
const FILE_IO_ID FILE_IO_ALG_PARAM_SCALE_ENABLED              =     30133;//簍衡猭把计-熬簿币ノ-罽

const FILE_IO_ID FILE_IO_ALG_PARAM_OFFSET_A_USL               =     30141;//簍衡猭把计-熬簿-XYà
const FILE_IO_ID FILE_IO_ALG_PARAM_OFFSET_A_LSL               =     30142;//簍衡猭把计-熬簿-XYà
const FILE_IO_ID FILE_IO_ALG_PARAM_OFFSET_A_ENABLED           =     30143;//簍衡猭把计-熬簿币ノ-XYà

const FILE_IO_ID FILE_IO_ALG_PARAM_OFFSET_L_USL               =     30151;//簍衡猭把计-熬簿-L
const FILE_IO_ID FILE_IO_ALG_PARAM_OFFSET_L_LSL               =     30152;//簍衡猭把计-熬簿-L
const FILE_IO_ID FILE_IO_ALG_PARAM_OFFSET_L_ENABLED           =     30153;//簍衡猭把计-熬簿币ノ-L

const FILE_IO_ID FILE_IO_ALG_PARAM_MASK_BINARY_PARAM          =     30480;//簍衡猭把计-て把计
const FILE_IO_ID FILE_IO_ALG_PARAM_IMAGE_BINARY_PARAM         =     30481;//簍衡猭把计-紇钩把计
//30501 ~ 30099
const FILE_IO_ID FILE_IO_ALG_PARAM_PATTERN_COUNT              =     30501;//簍衡猭把计-妓狾瓜计秖
const FILE_IO_ID FILE_IO_ALG_PARAM_PATTERN_ELABLED            =     30502;//簍衡猭把计-琌ㄏノ妓狾瓜郎	
const FILE_IO_ID FILE_IO_ALG_PARAM_PATTERN_SIMILARITY_USL     =     30503;//簍衡猭把计-妓狾
const FILE_IO_ID FILE_IO_ALG_PARAM_PATTERN_SIMILARITY_LSL     =     30504;//簍衡猭把计-妓狾
const FILE_IO_ID FILE_IO_ALG_PARAM_PATTERN_MIN_AREA           =     30505;//簍衡猭把计-妓狾程玂痙縩
const FILE_IO_ID FILE_IO_ALG_PARAM_PATTERN_POLARITY           =     30506;//簍衡猭把计-妓狾伐┦
const FILE_IO_ID FILE_IO_ALG_PARAM_PATTERN_FINAL_REDUCTION    =     30507;//簍衡猭把计-妓狾程ソ摧緇糷计
const FILE_IO_ID FILE_IO_ALG_PARAM_PATTERN_ANGLE_EXPAND       =     30508;//簍衡猭把计-妓狾à耎
const FILE_IO_ID FILE_IO_ALG_PARAM_PATTERN_SCALE_EXPAND       =     30509;//簍衡猭把计-妓狾罽耎
const FILE_IO_ID FILE_IO_ALG_PARAM_PATTERN_SCALE_ISOTROPIC    =     30510;//簍衡猭把计-妓狾罽单よ┦
const FILE_IO_ID FILE_IO_ALG_PARAM_PATTERN_ADVANCED_LEARNING  =     30511;//簍衡猭把计-妓狾秈顶厩策
const FILE_IO_ID FILE_IO_ALG_PARAM_PATTERN_NODE               =     30580;//簍衡猭把计-妓狾瓜て
//31001 ~ 31099
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_START         =     31001;//簍衡猭把计-獹ゑㄒ把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_END           =     31002;//簍衡猭把计-獹ゑㄒ把计-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_TARGET        =     31003;//簍衡猭把计-獹ゑㄒ把计-ヘ夹蔼
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_RATIO_USL     =     31004;//簍衡猭把计-獹ゑㄒ把计-ゑㄒ
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_RATIO_LSL     =     31005;//簍衡猭把计-獹ゑㄒ把计-ゑㄒ

const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_AVERAGE_SCALE  =    31006;//簍衡猭把计-獹ゑㄒ把计-キАゑㄒ
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_AVERAGE_SCALE_ENB=  31007;//簍衡猭把计-獹ゑㄒ把计-キАゑㄒ币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_AVERAGE_MODE  =     31008;//簍衡猭把计-獹ゑㄒ把计-キА家Α
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_AVE_PARTIAL_H =     31009;//簍衡猭把计-獹ゑㄒ把计-キА絛瞅
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_AVE_PARTIAL_L =     31010;//簍衡猭把计-獹ゑㄒ把计-キА絛瞅

const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_RANGE_ENABLED =     31011;//簍衡猭把计-獹ゑㄒ把计-蔼畉币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_RANGE_USL     =     31012;//簍衡猭把计-獹ゑㄒ把计-蔼畉
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_RANGE_LSL     =     31013;//簍衡猭把计-獹ゑㄒ把计-蔼畉

const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_ROI_BOX_ENB   =     31018;//簍衡猭把计-獹ゑㄒ把计-ㄏノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_RATIO_ENABLED =     31019;//簍衡猭把计-獹ゑㄒ把计-ゑㄒ币ノ

const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_CONTRAST_ENABLED=   31021;//簍衡猭把计-獹ゑㄒ把计-癸ゑ币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_CONTRAST_USL  =     31022;//簍衡猭把计-獹ゑㄒ把计-癸ゑ
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_CONTRAST_LSL  =     31023;//簍衡猭把计-獹ゑㄒ把计-癸ゑ

const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_RANGE   =     31031;//簍衡猭把计-獹ゑㄒ把计-X禸砮羇絛瞅
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_ENABLED =     31032;//簍衡猭把计-獹ゑㄒ把计-X禸砮羇絛瞅
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_USL     =     31033;//簍衡猭把计-獹ゑㄒ把计-X禸砮
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_LSL     =     31034;//簍衡猭把计-獹ゑㄒ把计-X禸砮
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_MODE    =     31035;//簍衡猭把计-獹ゑㄒ把计-X禸砮家Α
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_XLINE_UNIT_MODE=    31036;//簍衡猭把计-獹ゑㄒ把计-X禸砮虫家Α

const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_RANGE   =     31041;//簍衡猭把计-獹ゑㄒ把计-Y禸砮羇絛瞅
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_ENABLED =     31042;//簍衡猭把计-獹ゑㄒ把计-Y禸砮羇絛瞅
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_USL     =     31043;//簍衡猭把计-獹ゑㄒ把计-Y禸砮
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_LSL     =     31044;//簍衡猭把计-獹ゑㄒ把计-Y禸砮
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_MODE    =     31045;//簍衡猭把计-獹ゑㄒ把计-Y禸砮家Α
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_YLINE_UNIT_MODE=    31046;//簍衡猭把计-獹ゑㄒ把计-Y禸砮虫家Α

const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_TOLERANCE_ENABLED=  31051;//簍衡猭把计-獹ゑㄒ把计-そ畉币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_TOLERANCE_USL  =    31052;//簍衡猭把计-獹ゑㄒ把计-そ畉
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_TOLERANCE_LSL  =    31053;//簍衡猭把计-獹ゑㄒ把计-そ畉

const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MAX_ENB  =    31061;//簍衡猭把计-獹ゑㄒ把计-程币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MIN_ENB  =    31062;//簍衡猭把计-獹ゑㄒ把计-程币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_TOL_USL  =    31063;//簍衡猭把计-獹ゑㄒ把计-伐そ畉
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_TOL_LSL  =    31064;//簍衡猭把计-獹ゑㄒ把计-伐そ畉
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MAX_SCALE=    31065;//簍衡猭把计-獹ゑㄒ把计-程ゑㄒ
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MIN_SCALE=    31066;//簍衡猭把计-獹ゑㄒ把计-程ゑㄒ
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MAX_SCALE_ENB=31067;//簍衡猭把计-獹ゑㄒ把计-程ゑㄒ币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BRIGHT_RATIO_LIMIT_MIN_SCALE_ENB=31068;//簍衡猭把计-獹ゑㄒ把计-程ゑㄒ币ノ

//31101 ~ 31199
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_START          =     31101;//簍衡猭把计-钡祏隔把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_END            =     31102;//簍衡猭把计-钡祏隔把计-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_ENABLED_R      =     31111;//簍衡猭把计-钡祏隔把计-币ノ-R
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_RANGE_R        =     31112;//簍衡猭把计-钡祏隔把计-絛瞅-R
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_MODE_R         =     31113;//簍衡猭把计-钡祏隔把计-家Α-R
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_USL_R          =     31114;//簍衡猭把计-钡祏隔把计--R
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_LSL_R          =     31115;//簍衡猭把计-钡祏隔把计--R
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_EXT_MODE_R     =     31116;//簍衡猭把计-钡祏隔把计-┑-R
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_ENABLED_T      =     31121;//簍衡猭把计-钡祏隔把计-币ノ-T
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_RANGE_T        =     31122;//簍衡猭把计-钡祏隔把计-絛瞅-T
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_MODE_T         =     31123;//簍衡猭把计-钡祏隔把计-家Α-T
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_USL_T          =     31124;//簍衡猭把计-钡祏隔把计--T
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_LSL_T          =     31125;//簍衡猭把计-钡祏隔把计--T
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_EXT_MODE_T     =     31126;//簍衡猭把计-钡祏隔把计-┑-T
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_ENABLED_L      =     31131;//簍衡猭把计-钡祏隔把计-币ノ-L
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_RANGE_L        =     31132;//簍衡猭把计-钡祏隔把计-絛瞅-L
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_MODE_L         =     31133;//簍衡猭把计-钡祏隔把计-家Α-L
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_USL_L          =     31134;//簍衡猭把计-钡祏隔把计--L
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_LSL_L          =     31135;//簍衡猭把计-钡祏隔把计--L
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_EXT_MODE_L     =     31136;//簍衡猭把计-钡祏隔把计-┑-L
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_ENABLED_B      =     31141;//簍衡猭把计-钡祏隔把计-币ノ-B
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_RANGE_B        =     31142;//簍衡猭把计-钡祏隔把计-絛瞅-B
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_MODE_B         =     31143;//簍衡猭把计-钡祏隔把计-家Α-B
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_USL_B          =     31144;//簍衡猭把计-钡祏隔把计--B
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_LSL_B          =     31145;//簍衡猭把计-钡祏隔把计--B
const FILE_IO_ID FILE_IO_ALG_PARAM_OUTER_SHORT_EXT_MODE_B     =     31146;//簍衡猭把计-钡祏隔把计-┑-B
//31201 ~ 31399
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_START           =     31201;//簍衡猭把计-跋遏计秖把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_END             =     31202;//簍衡猭把计-跋遏计秖把计-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_COUNT_USL       =     31203;//簍衡猭把计-跋遏计秖把计-跋遏计秖程
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_COUNT_LSL       =     31204;//簍衡猭把计-跋遏计秖把计-跋遏计秖程
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_CONNECTIVITY    =     31205;//簍衡猭把计-跋遏计秖把计-跋遏綟家Α

const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_X_MAX           =     31211;//簍衡猭把计-跋遏计秖把计-跋遏Xへ程
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_X_MAX_ENABLED   =     31212;//簍衡猭把计-跋遏计秖把计-跋遏Xへ程币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_X_MIN           =     31213;//簍衡猭把计-跋遏计秖把计-跋遏Xへ程
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_X_MIN_ENABLED   =     31214;//簍衡猭把计-跋遏计秖把计-跋遏Xへ程币ノ

const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_Y_MAX           =     31221;//簍衡猭把计-跋遏计秖把计-跋遏Yへ程
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_Y_MAX_ENABLED   =     31222;//簍衡猭把计-跋遏计秖把计-跋遏Yへ程币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_Y_MIN           =     31223;//簍衡猭把计-跋遏计秖把计-跋遏Yへ程
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_Y_MIN_ENABLED   =     31224;//簍衡猭把计-跋遏计秖把计-跋遏Yへ程币ノ

const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_AREA_MAX        =     31231;//簍衡猭把计-跋遏计秖把计-跋遏縩へ程
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_AREA_MAX_ENABLED=     31232;//簍衡猭把计-跋遏计秖把计-跋遏縩へ程币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_AREA_MIN        =     31233;//簍衡猭把计-跋遏计秖把计-跋遏縩へ程
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_AREA_MIN_ENABLED=     31234;//簍衡猭把计-跋遏计秖把计-跋遏縩へ程币ノ

const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_ASPECT_RATIO_MAX=    31241;//簍衡猭把计-跋遏计秖把计-跋遏糴ゑ程
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_ASPECT_RATIO_MAX_ENABLED=31242;//簍衡猭把计-跋遏计秖把计-跋遏糴ゑ程币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_ASPECT_RATIO_MIN=    31243;//簍衡猭把计-跋遏计秖把计-跋遏糴ゑ程
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_ASPECT_RATIO_MIN_ENABLED=31244;//簍衡猭把计-跋遏计秖把计-跋遏糴ゑ程币ノ

const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_LONG_SHORT_RATIO_MAX=31246;//簍衡猭把计-跋遏计秖把计-跋遏祏ゑ程
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_LONG_SHORT_RATIO_MAX_ENB=31247;//簍衡猭把计-跋遏计秖把计-跋遏祏ゑ程币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_LONG_SHORT_RATIO_MIN=31248;//簍衡猭把计-跋遏计秖把计-跋遏祏ゑ程
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_LONG_SHORT_RATIO_MIN_ENB=31249;//簍衡猭把计-跋遏计秖把计-跋遏祏ゑ程币ノ

const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_FILL_RATIO_MAX=      31251;//簍衡猭把计-跋遏计秖把计-跋遏恶骸瞯程
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_FILL_RATIO_MAX_ENABLED=31252;//簍衡猭把计-跋遏计秖把计-跋遏恶骸瞯程币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_FILL_RATIO_MIN=      31253;//簍衡猭把计-跋遏计秖把计-跋遏恶骸瞯程
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_FILL_RATIO_MIN_ENABLED=31254;//簍衡猭把计-跋遏计秖把计-跋遏恶骸瞯程币ノ

const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_L_MAX           =     31261;//簍衡猭把计-跋遏计秖把计-跋遏Lへ程
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_L_MAX_ENABLED   =     31262;//簍衡猭把计-跋遏计秖把计-跋遏Lへ程币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_L_MIN           =     31263;//簍衡猭把计-跋遏计秖把计-跋遏Lへ程
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_L_MIN_ENABLED   =     31264;//簍衡猭把计-跋遏计秖把计-跋遏Lへ程币ノ

const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_ROI_BOX_EXTEND_X=     31271;//簍衡猭把计-跋遏计秖把计-跋遏┑へX-um
const FILE_IO_ID FILE_IO_ALG_PARAM_BLOB_COUNT_ROI_BOX_EXTEND_Y=     31272;//簍衡猭把计-跋遏计秖把计-跋遏┑へY-um

//31401 ~ 31599
const FILE_IO_ID FILE_IO_ALG_PARAM_MODEL_MATCH_START          =     31401;//簍衡猭把计-家狾で皌把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_MODEL_MATCH_END            =     31402;//簍衡猭把计-家狾で皌把计-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_MODEL_MATCH_DOCK_MODE      =     31403;//簍衡猭把计-家狾で皌把计-綼娩家Α
const FILE_IO_ID FILE_IO_ALG_PARAM_MODEL_MATCH_RECHECK_BOX    =     31404;//簍衡猭把计-家狾で皌把计-穝絋粄疭紉
//31601 ~ 31799
const FILE_IO_ID FILE_IO_ALG_PARAM_IMAGE_MATCH_START          =     31601;//簍衡猭把计-紇钩で皌把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_IMAGE_MATCH_END            =     31602;//簍衡猭把计-紇钩で皌把计-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_ENABLED    = 31651;//簍衡猭把计-紇钩で皌-钩ゑ耕把计-穞场-31651-31699
const FILE_IO_ID FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_DARK_LEVEL = 31652;//簍衡猭把计-紇钩で皌-钩ゑ耕把计-穞场-31651-31699
const FILE_IO_ID FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_TOLERANCE  = 31653;//簍衡猭把计-紇钩で皌-钩ゑ耕把计-粇畉
const FILE_IO_ID FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_OPEN_SIZE  = 31654;//簍衡猭把计-紇钩で皌-钩ゑ耕把计-Open
const FILE_IO_ID FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_CLOSE_SIZE = 31655;//簍衡猭把计-紇钩で皌-钩ゑ耕把计-Close
const FILE_IO_ID FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_GAUSSIAN_SIZE= 31656;//簍衡猭把计-紇钩で皌-钩ゑ耕把计-Smooth-Gaussian
const FILE_IO_ID FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_XSIZE_MIN  = 31661;//簍衡猭把计-紇钩で皌-钩ゑ耕把计-程へ
const FILE_IO_ID FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_YSIZE_MIN  = 31671;//簍衡猭把计-紇钩で皌-钩ゑ耕把计-程へ
const FILE_IO_ID FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_AREA_MIN   = 31681;//簍衡猭把计-紇钩で皌-钩ゑ耕把计-程縩
const FILE_IO_ID FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_COUNT_LSL  = 31691;//簍衡猭把计-紇钩で皌-钩ゑ耕把计-计秖
const FILE_IO_ID FILE_IO_ALG_PARAM_IMAGE_MATCH_PXL_CMP_COUNT_USL  = 31692;//簍衡猭把计-紇钩で皌-钩ゑ耕把计-计秖
//31801 ~ 31999
const FILE_IO_ID FILE_IO_ALG_PARAM_CHAR_VERIFY_START          =     31801;//簍衡猭把计-ゅ喷靡把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_CHAR_VERIFY_END            =     31802;//簍衡猭把计-ゅ喷靡把计-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_CHAR_VERIFY_CELL_SCORE_MAX =     31803;//簍衡猭把计-ゅ喷靡把计-虫じ
const FILE_IO_ID FILE_IO_ALG_PARAM_CHAR_VERIFY_CELL_SCORE_MIN =     31804;//簍衡猭把计-ゅ喷靡把计-虫じ
const FILE_IO_ID FILE_IO_ALG_PARAM_CHAR_VERIFY_PASS_RATIO_USL =     31805;//簍衡猭把计-ゅ喷靡把计-硄筁ゑㄒ
const FILE_IO_ID FILE_IO_ALG_PARAM_CHAR_VERIFY_PASS_RATIO_LSL =     31806;//簍衡猭把计-ゅ喷靡把计-硄筁ゑㄒ
const FILE_IO_ID FILE_IO_ALG_PARAM_CHAR_VERIFY_CELL_GRID_CNT  =     31807;//簍衡猭把计-ゅ喷靡把计-虫じ灿だ计秖
const FILE_IO_ID FILE_IO_ALG_PARAM_CHAR_VERIFY_CELL_EXT_SIZE  =     31808;//簍衡猭把计-ゅ喷靡把计-虫じ耎へ
//32001 ~ 32199
const FILE_IO_ID FILE_IO_ALG_PARAM_GROUP_COMPARE_START        =     32001;//簍衡猭把计-竤舱ゑ耕把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_GROUP_COMPARE_END          =     32002;//簍衡猭把计-竤舱ゑ耕把计-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_GROUP_COMPARE_DIRECTION_MODE  =  32003;//簍衡猭把计-竤舱ゑ耕把计-よ家Α

const FILE_IO_ID FILE_IO_ALG_PARAM_GROUP_COMPARE_2D_GRAY_ENABLED =  32011;//簍衡猭把计-竤舱ゑ耕把计-2D紇钩币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_GROUP_COMPARE_2D_GRAY_USL     =  32012;//簍衡猭把计-竤舱ゑ耕把计-2D紇钩
const FILE_IO_ID FILE_IO_ALG_PARAM_GROUP_COMPARE_2D_GRAY_LSL     =  32013;//簍衡猭把计-竤舱ゑ耕把计-2D紇钩

const FILE_IO_ID FILE_IO_ALG_PARAM_GROUP_COMPARE_3D_HEIGHT_ENABLED= 32021;//簍衡猭把计-竤舱ゑ耕把计-3D蔼币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_GROUP_COMPARE_3D_HEIGHT_USL    = 32022;//簍衡猭把计-竤舱ゑ耕把计-3D蔼
const FILE_IO_ID FILE_IO_ALG_PARAM_GROUP_COMPARE_3D_HEIGHT_LSL    = 32023;//簍衡猭把计-竤舱ゑ耕把计-3D蔼
const FILE_IO_ID FILE_IO_ALG_PARAM_GROUP_COMPARE_3D_HEIGHT_BASE_MODE= 32024;//簍衡猭把计-竤舱ゑ耕把计-3D蔼膀非蔼

const FILE_IO_ID FILE_IO_ALG_PARAM_GROUP_COMPARE_TILT_ANGLE_ENB   = 32031;//簍衡猭把计-竤舱ゑ耕把计-渡弊à币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_GROUP_COMPARE_TILT_ANGLE_USL   = 32032;//簍衡猭把计-竤舱ゑ耕把计-渡弊à
const FILE_IO_ID FILE_IO_ALG_PARAM_GROUP_COMPARE_TILT_ANGLE_LSL   = 32033;//簍衡猭把计-竤舱ゑ耕把计-渡弊à
//-------------------------------------------------------------------------------------//
//32201 ~ 32299
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_START        =     32201;//簍衡猭把计-兵絏侩醚把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_END          =     32202;//簍衡猭把计-兵絏侩醚把计-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CODE_COUNT   =  32203;//簍衡猭把计-兵絏侩醚把计-计
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CODE_COUNT_USED=  32204;//簍衡猭把计-兵絏侩醚把计-计币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CODE_CONTENT =  32205;//簍衡猭把计-兵絏侩醚把计-ず甧
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CODE_CONTENT_USED=  32206;//簍衡猭把计-兵絏侩醚把计-ず甧币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_VERIFY_USL   =  32207;//簍衡猭把计-兵絏侩醚把计-喷靡
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_VERIFY_LSL   =  32208;//簍衡猭把计-兵絏侩醚把计-喷靡
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_DECODER_TIMOUT=  32209;//簍衡猭把计-兵絏侩醚把计-秆絏筄
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_DIRECTION_MODE=  32210;//簍衡猭把计-兵絏侩醚把计-よ家Α
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_DECODE_START =   32211;//簍衡猭把计-兵絏侩醚把计-秆絏秨﹍˙艼
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CODE_JSON_CHK_USED=32212;//簍衡猭把计-兵絏侩醚把计-ず甧JSON絋粄币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_CHECK_SUM_USED=    32213;//簍衡猭把计-兵絏侩醚把计-兵絏CheckSum币ノ

const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_1D_USED      =     32241;//簍衡猭把计-兵絏侩醚把计-1蝴兵絏
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_QRCODE_USED  =     32242;//簍衡猭把计-兵絏侩醚把计-QR-Code
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_DATA_MATRIX_USED=  32243;//簍衡猭把计-兵絏侩醚把计-Data Matrix

const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_1D_DECODER   =     32251;//簍衡猭把计-兵絏侩醚把计-1蝴兵絏-秆絏竟
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_QRCODE_DECODER=    32252;//簍衡猭把计-兵絏侩醚把计-QR-Code-秆絏竟
const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_DATA_MATRIX_DECODER=32253;//簍衡猭把计-兵絏侩醚把计-Data Matrix-秆絏竟

const FILE_IO_ID FILE_IO_ALG_PARAM_BARCODE_RECOGNIZE_STEP_NODE    =  32281;//簍衡猭把计-兵絏侩醚把计-秆絏˙艼
//-------------------------------------------------------------------------------------//
//32301 ~ 32399
const FILE_IO_ID FILE_IO_ALG_PARAM_COLOR_CODE_START            =     32301;//簍衡猭把计-︹絏浪代把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_COLOR_CODE_END              =     32302;//簍衡猭把计-︹絏浪代把计-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_COLOR_CODE_POLARITY         =     32303;//簍衡猭把计-︹絏浪代把计-伐┦よ
const FILE_IO_ID FILE_IO_ALG_PARAM_COLOR_CODE_CELL_SCORE_MAX   =     32304;//簍衡猭把计-︹絏浪代把计-虫じ
const FILE_IO_ID FILE_IO_ALG_PARAM_COLOR_CODE_CELL_SCORE_MIN   =     32305;//簍衡猭把计-︹絏浪代把计-虫じ
const FILE_IO_ID FILE_IO_ALG_PARAM_COLOR_CODE_PASS_RATIO_USL   =     32306;//簍衡猭把计-︹絏浪代把计-硄筁ゑㄒ
const FILE_IO_ID FILE_IO_ALG_PARAM_COLOR_CODE_PASS_RATIO_LSL   =     32307;//簍衡猭把计-︹絏浪代把计-硄筁ゑㄒ
//-------------------------------------------------------------------------------------//
//32401 ~ 32499
const FILE_IO_ID FILE_IO_ALG_PARAM_FD_MATCH_START              =     32401;//簍衡猭把计-﹚翴で皌把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_FD_MATCH_END                =     32402;//簍衡猭把计-﹚翴で皌把计-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_FD_MATCH_MODE               =     32403;//簍衡猭把计-﹚翴で皌把计-家Α
const FILE_IO_ID FILE_IO_ALG_PARAM_FD_FILL_SIZE                =     32404;//簍衡猭把计-﹚翴で皌把计-恶骸へ
//-------------------------------------------------------------------------------------//
//32501 ~ 32699
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_START        =     32501;//簍衡猭把计-ン秖代把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_END          =     32502;//簍衡猭把计-ン秖代把计-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_CALC_MODE=    32503;//簍衡猭把计-ン秖代把计-へ璸衡家Α
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_CALC_UNIT_MODE=32504;//簍衡猭把计-ン秖代把计-へ璸衡虫家Α
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_BLUR_SIZE=    32505;//簍衡猭把计-ン秖代把计-へキ菲
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_MULTI_BLOB   =     32506;//簍衡猭把计-ン秖代把计-跋遏

const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_SPEC  =     32511;//簍衡猭把计-ン秖代把计-へX砏
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_ENB   =     32512;//簍衡猭把计-ン秖代把计-へX币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_USL_DIFF=   32513;//簍衡猭把计-ン秖代把计-へX-um
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_LSL_DIFF=   32514;//簍衡猭把计-ン秖代把计-へX-um
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_USL_RATIO=  32515;//簍衡猭把计-ン秖代把计-へX-%
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_X_LSL_RATIO=  32516;//簍衡猭把计-ン秖代把计-へX-%

const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_SPEC  =     32531;//簍衡猭把计-ン秖代把计-へY砏
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_ENB   =     32532;//簍衡猭把计-ン秖代把计-へY币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_USL_DIFF=   32533;//簍衡猭把计-ン秖代把计-へY-um
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_LSL_DIFF=   32534;//簍衡猭把计-ン秖代把计-へY-um
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_USL_RATIO=  32535;//簍衡猭把计-ン秖代把计-へY-%
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_SIZE_Y_LSL_RATIO=  32536;//簍衡猭把计-ン秖代把计-へY-%

const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_SPEC  =     32551;//簍衡猭把计-ン秖代把计-蔼砏
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_ENB   =     32552;//簍衡猭把计-ン秖代把计-蔼币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_AVE_MODE=   32553;//簍衡猭把计-ン秖代把计-蔼キА家Α
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_AVE_PART_H= 32554;//簍衡猭把计-ン秖代把计-蔼キА絛瞅
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_AVE_PART_L= 32555;//簍衡猭把计-ン秖代把计-蔼キА絛瞅
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_CALC_UNIT_MODE=32556;//簍衡猭把计-ン秖代把计-蔼璸衡虫家Α
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_USL_DIFF=   32557;//簍衡猭把计-ン秖代把计-蔼-um
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_LSL_DIFF=   32558;//簍衡猭把计-ン秖代把计-蔼-um
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_USL_RATIO=  32559;//簍衡猭把计-ン秖代把计-蔼-%
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_HEIGHT_LSL_RATIO=  32560;//簍衡猭把计-ン秖代把计-蔼-%

const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_AREA_SPEC    =     32571;//簍衡猭把计-ン秖代把计-縩砏
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_AREA_ENB     =     32572;//簍衡猭把计-ン秖代把计-縩币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_AREA_USL     =     32575;//簍衡猭把计-ン秖代把计-縩-%
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_AREA_LSL     =     32576;//簍衡猭把计-ン秖代把计-縩-%

const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_VOLUME_SPEC  =     32591;//簍衡猭把计-ン秖代把计-砰縩砏
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_VOLUME_ENB   =     32592;//簍衡猭把计-ン秖代把计-砰縩币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_VOLUME_USL   =     32595;//簍衡猭把计-ン秖代把计-砰縩-%
const FILE_IO_ID FILE_IO_ALG_PARAM_OBJECT_MEASURE_VOLUME_LSL   =     32596;//簍衡猭把计-ン秖代把计-砰縩-%
//-------------------------------------------------------------------------------------//
//32701 ~ 32799
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_START           =     32701;//簍衡猭把计-娩絫穓碝把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_END             =     32702;//簍衡猭把计-娩絫穓碝把计-沧翴

const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_CUT_LINE_ENB    =     32708;//簍衡猭把计-娩絫穓碝把计-币ノ-ち耞絬-いァ
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_INVERT_ALIGN    =     32709;//簍衡猭把计-娩絫穓碝把计-币ノ-は癸霍

const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_ENABLE_F        =     32711;//簍衡猭把计-娩絫穓碝把计-币ノ-玡狠
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_MIN_RATIO_U_F   =     32712;//簍衡猭把计-娩絫穓碝把计-程糴ゑㄒ-玡狠
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_MAX_RATIO_U_F   =     32713;//簍衡猭把计-娩絫穓碝把计-程糴ゑㄒ-玡狠
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_MIN_RANGE_V_F   =     32715;//簍衡猭把计-娩絫穓碝把计-程絛瞅-玡狠
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_MAX_RANGE_V_F   =     32716;//簍衡猭把计-娩絫穓碝把计-程絛瞅-玡狠
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_SIZE_RATIO_V_F  =     32717;//簍衡猭把计-娩絫穓碝把计-へゑㄒ-玡狠
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_SIZE_CALC_MODE_F=     32718;//簍衡猭把计-娩絫穓碝把计-へ璸衡家Α-玡狠
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_FIND_DIRECTION_F=     32719;//簍衡猭把计-娩絫穓碝把计-穓碝よ-玡狠
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_FIND_RATIO_F    =     32720;//簍衡猭把计-娩絫穓碝把计-穓碝ゑㄒ-玡狠

const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_ENABLE_B        =     32731;//簍衡猭把计-娩絫穓碝把计-币ノ-狠
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_MIN_RATIO_U_B   =     32732;//簍衡猭把计-娩絫穓碝把计-程糴ゑㄒ-狠
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_MAX_RATIO_U_B   =     32733;//簍衡猭把计-娩絫穓碝把计-程糴ゑㄒ-狠
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_MIN_RANGE_V_B   =     32735;//簍衡猭把计-娩絫穓碝把计-程絛瞅-狠
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_MAX_RANGE_V_B   =     32736;//簍衡猭把计-娩絫穓碝把计-程絛瞅-狠
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_SIZE_RATIO_V_B  =     32737;//簍衡猭把计-娩絫穓碝把计-へゑㄒ-狠
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_SIZE_CALC_MODE_B=     32738;//簍衡猭把计-娩絫穓碝把计-へ璸衡家Α-狠
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_FIND_DIRECTION_B=     32739;//簍衡猭把计-娩絫穓碝把计-穓碝よ-狠
const FILE_IO_ID FILE_IO_ALG_PARAM_EDGE_SEARCH_FIND_RATIO_B    =     32740;//簍衡猭把计-娩絫穓碝把计-穓碝ゑㄒ-狠
//-------------------------------------------------------------------------------------//
//32801 ~ 32899
const FILE_IO_ID FILE_IO_ALG_PARAM_SHAPE_VERIFY_START          =     32801;//簍衡猭把计-喷靡把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_SHAPE_VERIFY_END            =     32802;//簍衡猭把计-喷靡把计-沧翴

const FILE_IO_ID FILE_IO_ALG_PARAM_SHAPE_VERIFY_SKIP_RATIO_INN =     32803;//簍衡猭把计-喷靡把计-铬筁ゑㄒ-ず场
const FILE_IO_ID FILE_IO_ALG_PARAM_SHAPE_VERIFY_SKIP_RATIO_OUT =     32804;//簍衡猭把计-喷靡把计-铬筁ゑㄒ-场

const FILE_IO_ID FILE_IO_ALG_PARAM_SHAPE_VERIFY_CIRCLE_OUTER_TOL=    32811;//簍衡猭把计-喷靡把计-蛾畖そ畉
const FILE_IO_ID FILE_IO_ALG_PARAM_SHAPE_VERIFY_CIRCLE_INNER_TOL=    32812;//簍衡猭把计-喷靡把计-ず蛾畖そ畉
const FILE_IO_ID FILE_IO_ALG_PARAM_SHAPE_VERIFY_CIRCLE_RANGE_TOL=    32813;//簍衡猭把计-喷靡把计-ず粇畉そ畉
const FILE_IO_ID FILE_IO_ALG_PARAM_SHAPE_VERIFY_CIRCLE_ERROR_TOL=    32814;//簍衡猭把计-喷靡把计-キА粇畉そ畉
//-------------------------------------------------------------------------------------//
//32901 ~ 32999
const FILE_IO_ID FILE_IO_ALG_PARAM_ANGLE_MEASURE_START         =     32901;//簍衡猭把计-à秖代把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_ANGLE_MEASURE_END           =     32902;//簍衡猭把计-à秖代把计-沧翴

const FILE_IO_ID FILE_IO_ALG_PARAM_ANGLE_MEASURE_ANGLE_SPEC    =     32903;//簍衡猭把计-à秖代把计-à砏
const FILE_IO_ID FILE_IO_ALG_PARAM_ANGLE_MEASURE_ANGLE_TOL_USL =     32904;//簍衡猭把计-à秖代把计-そ畉
const FILE_IO_ID FILE_IO_ALG_PARAM_ANGLE_MEASURE_ANGLE_TOL_LSL =     32905;//簍衡猭把计-à秖代把计-そ畉
const FILE_IO_ID FILE_IO_ALG_PARAM_ANGLE_MEASURE_ANGLE_MODE    =     32906;//簍衡猭把计-à秖代把计-à家Α
const FILE_IO_ID FILE_IO_ALG_PARAM_ANGLE_MEASURE_BASE_LINE_MODE=     32907;//簍衡猭把计-à秖代把计-膀非絬家Α
//-------------------------------------------------------------------------------------//
//33001 ~ 33099
const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_START        =     33001;//簍衡猭把计-钩ゑ耕把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_END          =     33002;//簍衡猭把计-钩ゑ耕把计-沧翴

const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CELL_EXT_SIZE=     33011;//簍衡猭把计-钩ゑ耕把计-虫じ耎へ-pxl
const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_IMG_BLUR_SIZE=     33012;//簍衡猭把计-钩ゑ耕把计-紇钩家絢へ-pxl

const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_PAT_DARK_LEVEL=    33021;//簍衡猭把计-钩ゑ耕把计-妓狾筁穞ぃ矪瞶-η顶
const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_PAT_LIGHT_LEVEL=   33022;//簍衡猭把计-钩ゑ耕把计-妓狾筁獹ぃ矪瞶-η顶
const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_PAT_ERODE_SIZE=    33023;//簍衡猭把计-钩ゑ耕把计-妓狾獻籯へ-pxl
const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_PAT_PURE_COLOR=    33024;//簍衡猭把计-钩ゑ耕把计-妓狾︹家Α

const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_DARK_GAIN=     33041;//簍衡猭把计-钩ゑ耕把计-ゑ耕穞场糤痲-ゑㄒ
const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_LIGHT_GAIN=    33042;//簍衡猭把计-钩ゑ耕把计-ゑ耕獹场糤痲-ゑㄒ
const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_TOLERANCE=     33043;//簍衡猭把计-钩ゑ耕把计-ゑ耕η顶そ畉-η顶
const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_GAUSSIAN_SIZE= 33044;//簍衡猭把计-钩ゑ耕把计-ゑ耕蔼吹耾猧へ-pxl
const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_OPEN_SIZE=     33045;//簍衡猭把计-钩ゑ耕把计-ゑ耕秨笲衡へ-pxl
const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_CMP_CLOSE_SIZE=    33046;//簍衡猭把计-钩ゑ耕把计-ゑ耕超笲衡へ-pxl

const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_BLOB_MIN_SIZE_D=   33061;//簍衡猭把计-钩ゑ耕把计-跋遏程-um
const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_BLOB_COUNT_USL=    33062;//簍衡猭把计-钩ゑ耕把计-跋遏计秖
const FILE_IO_ID FILE_IO_ALG_PARAM_PIEXEL_COMPARE_BLOB_COUNT_LSL=    33063;//簍衡猭把计-钩ゑ耕把计-跋遏计秖
//-------------------------------------------------------------------------------------//
//34001 ~ 34099
const FILE_IO_ID FILE_IO_ALG_PARAM_IPC_START					   = 34001;//簍衡猭把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_IPC_END					       = 34002;//簍衡猭把计-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_IPC_ENABLE                      = 34010;//IPC竚ゑ癸
const FILE_IO_ID FILE_IO_ALG_PARAM_IPC_X				           = 34011;//IPC竚ゑ癸
const FILE_IO_ID FILE_IO_ALG_PARAM_IPC_Y				           = 34012;//IPC竚ゑ癸
const FILE_IO_ID FILE_IO_ALG_PARAM_IPC_CONTINUOUS_SET              = 34021;//硈尿钩
const FILE_IO_ID FILE_IO_ALG_PARAM_IPC_WIDTH_GAP                   = 34022;//丁禯把计
const FILE_IO_ID FILE_IO_ALG_PARAM_IPC_WIDTH_RATIO		           = 34023;//
//-------------------------------------------------------------------------------------//
//34101 ~ 34199
const FILE_IO_ID FILE_IO_ALG_PARAM_HEIGHT_START = 34101;//簍衡猭把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_HEIGHT_END = 34102;//簍衡猭把计-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_HEIGHT_ENABLED = 34103;
const FILE_IO_ID FILE_IO_ALG_PARAM_HEIGHT_NG_CHECK = 34114;
const FILE_IO_ID FILE_IO_ALG_PARAM_HEIGHT_RESIN_TIN_TYPE = 34104;
const FILE_IO_ID FILE_IO_ALG_PARAM_HEIGHT_DIRECTION = 34105;
const FILE_IO_ID FILE_IO_ALG_PARAM_HEIGHT_RANGE = 34106;
const FILE_IO_ID FILE_IO_ALG_PARAM_HEIGHT_MEASURE_MODE = 34107;
const FILE_IO_ID FILE_IO_ALG_PARAM_HEIGHT_OUTPUT_TYPE = 34108;//nOutputType
const FILE_IO_ID FILE_IO_ALG_PARAM_HEIGHT_VALUE = 34109;//result
const FILE_IO_ID FILE_IO_ALG_PARAM_HEIGHT_STEPZ = 34110;
const FILE_IO_ID FILE_IO_ALG_PARAM_HEIGHT_POSITION_SHIFT = 34111;//nShift_Tin
const FILE_IO_ID FILE_IO_ALG_PARAM_HEIGHT_THRESHOLDZ_WIDTH = 34112;
const FILE_IO_ID FILE_IO_ALG_PARAM_HEIGHT_THRESHOLDZ_HEIGHT = 34113;
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_ALG_PARAM_WIRE_START = 34201;//簍衡猭把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_WIRE_END = 34202;//簍衡猭把计-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_WIRE_WIDTH_FILTER_ENABLED = 34203;
const FILE_IO_ID FILE_IO_ALG_PARAM_WIRE_WIDTH_FILTER_SIZE = 34204;
const FILE_IO_ID FILE_IO_ALG_PARAM_WIRE_WIDTH_EDGE_LOW_THRESHOLD = 34205;
const FILE_IO_ID FILE_IO_ALG_PARAM_WIRE_WIDTH_EDGE_HIGHT_THRESHOLD = 34206;
const FILE_IO_ID FILE_IO_ALG_PARAM_WIRE_WIDTH_VALUE_USL = 34207;
const FILE_IO_ID FILE_IO_ALG_PARAM_WIRE_WIDTH_VALUE_LSL = 34208;
//-------------------------------------------------------------------------------------//
//35001 ~ 35099
const FILE_IO_ID FILE_IO_ALG_PARAM_AI_MODEL_START				   = 35001;//AI家把计-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_AI_MODEL_END					   = 35002;//AI家把计-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_AI_MODEL_AI_MODEL_ID			   = 35003;//AI家把计-AI家絪腹
const FILE_IO_ID FILE_IO_ALG_PARAM_AI_MODEL_CONFIDENCE_THRESHOLD   = 35004;//AI家把计-獺み恢
const FILE_IO_ID FILE_IO_ALG_PARAM_AI_MODEL_PATTERN_ANGLE          = 35005;//AI家把计-妓狾à
const FILE_IO_ID FILE_IO_ALG_PARAM_AI_MODEL_CHAR_MATCH_NUM_THRESHOLD= 35006;//AI家把计-じ才计秖恢
const FILE_IO_ID FILE_IO_ALG_PARAM_AI_MODEL_CHAR_NUM_UPPER_THRESHOLD= 35007;//AI家把计-じ计秖恢
//-------------------------------------------------------------------------------------//
//35101 ~ 35199
const FILE_IO_ID FILE_IO_ALG_PARAM_SOLDER_WETTING_START			   = 35101;//瞜钡浪代-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_SOLDER_WETTING_END			   = 35102;//瞜钡浪代-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_SOLDER_WETTING_CIRCLE_ANGLE_ENB = 35111;//瞜钡浪代-吏露à币ノ
const FILE_IO_ID FILE_IO_ALG_PARAM_SOLDER_WETTING_CIRCLE_ANGLE_USL = 35112;//瞜钡浪代-吏露à
const FILE_IO_ID FILE_IO_ALG_PARAM_SOLDER_WETTING_CIRCLE_ANGLE_LSL = 35113;//瞜钡浪代-吏露à
const FILE_IO_ID FILE_IO_ALG_PARAM_SOLDER_WETTING_CIRCLE_ANGLE_LINE_RATIO= 35114;//瞜钡浪代-吏露à畖ゑㄒ
const FILE_IO_ID FILE_IO_ALG_PARAM_SOLDER_WETTING_CIRCLE_ANGLE_LINE_START= 35115;//瞜钡浪代-吏露à畖秨﹍ゑㄒ
//-------------------------------------------------------------------------------------//
//36001 ~ 36099
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_START		    = 36001;//秖代堵溅-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_END		    = 36002;//秖代堵溅-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_SAVE_KEY      = 36011;//秖代堵溅-
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_FRAME_ID_01   = 36021;//秖代堵溅-堵溅紇钩
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_FRAME_ID_02   = 36022;//秖代堵溅-狾娩紇钩
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_FRAME_ID_03   = 36023;//秖代堵溅-荐翰溅紇钩
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_BLACK_GLUE_FRAME_ID_04   = 36024;//秖代堵溅-Coating紇钩
//-------------------------------------------------------------------------------------//
//36101 ~ 36199
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_FLUX_AREA_START		    = 36101;//秖代Flux縩-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_FLUX_AREA_END		    = 36102;//秖代Flux縩-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_FLUX_AREA_SAVE_KEY       = 36111;//秖代Flux縩-
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_FLUX_AREA_FRAME_ID_01    = 36121;//秖代Flux縩-堵溅紇钩
//-------------------------------------------------------------------------------------//
//36201 ~ 36299
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CPU_PIN_START		    = 36201;//秖代CPU钡竲-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CPU_PIN_END		        = 36202;//秖代CPU钡竲-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CPU_PIN_SAVE_KEY         = 36211;//秖代CPU钡竲-
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CPU_PIN_FRAME_ID_01      = 36221;//秖代CPU钡竲-堵溅紇钩
//-------------------------------------------------------------------------------------//
//36301 ~ 36399
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_SIP_START				= 36301;//秖代SIP-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_SIP_END					= 36302;//秖代SIP-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_SIP_DIRECTION			= 36303;//秖代SIP-璸衡よ
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_SIP_ANGLE_SPEC			= 36304;//秖代SIP-à夹非
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_SIP_ANGLE_TOLERANCE_USL	= 36305;//秖代SIP-à
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_SIP_ANGLE_TOLERANCE_LSL	= 36306;//秖代SIP-à
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_A_SPEC	= 36307;//秖代SIP-A翴禯瞒
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_A_TOLERANCE_USL	= 36308;//秖代SIP-A翴禯瞒
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_A_TOLERANCE_LSL	= 36309;//秖代SIP-A翴禯瞒
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_B_SPEC	= 36310;//秖代SIP-B翴禯瞒
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_B_TOLERANCE_USL	= 36311;//秖代SIP-B翴禯瞒
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_SIP_DISTANCE_TO_B_TOLERANCE_LSL	= 36312;//秖代SIP-B翴禯瞒
const FILE_IO_ID FILE_IO_ALG_MEASURE_SIP_INSPEC_EDGE_COUNT			= 36313;//秖代SIP-浪代娩计秖
const FILE_IO_ID FILE_IO_ALG_MEASURE_SIP_REF_EDGE_COUNT				= 36314;//秖代SIP-把σ娩计秖
//-------------------------------------------------------------------------------------//
//36401 ~ 36499
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_START			= 36401;//秖代Connector-癬翴
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_END			= 36402;//秖代Connector-沧翴
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_START	= 36403;//秖代Connector-Pin Tables
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_END      = 36404;//秖代Connector-Pin Tables
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_ROW_COUNT= 36405;//秖代Connector-Pin Row count
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_COL_COUNT= 36406;//秖代Connector-Pin Column count
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_ROW_INDEX= 36407;//秖代Connector-Pin Row Index
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_COL_INDEX= 36408;//秖代Connector-Pin Column Index
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_PIN_X	= 36409;//秖代Connector-Pin PIN X
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_TABLE_PIN_Y    = 36410;//秖代Connector-Pin PIN Y
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_PIN_WIDTH		= 36411;//秖代Connector-Pin PIN Widht
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_PIN_HEIGHT		= 36412;//秖代Connector-Pin PIN Height
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_BLOB_W_MIN		= 36413;//秖代Connector-Pin BLOB Width MIN
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_BLOB_H_MIN		= 36414;//秖代Connector-Pin BLOB Height MIN
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_BLOB_W_MAX		= 36415;//秖代Connector-Pin BLOB Width MAX
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_BLOB_H_MAX		= 36416;//秖代Connector-Pin BLOB Height MAX
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_PIN_D_USL	    = 36417;//秖代Connector-Pin 禯瞒
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_USE_EDGE       = 36418;//秖代Connector-Pin ㄏノ娩絫
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_AUTO_EDGE      = 36419;//秖代Connector-Pin 笆穓碝娩絫
const FILE_IO_ID FILE_IO_ALG_PARAM_MEASURE_CONNECTOR_CHILD          = 36420;//秖代Connector-Pin 琌琌Pinà
//-------------------------------------------------------------------------------------//
const FILE_IO_ID FILE_IO_END                                   =     INT_MAX;//99999,2147483647
//-------------------------------------------------------------------------------------//
#endif//AOIFileIODef