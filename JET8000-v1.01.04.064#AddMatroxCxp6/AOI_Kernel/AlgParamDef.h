#ifndef _ALG_PARAM_DEF_H_
#define _ALG_PARAM_DEF_H_
//-------------------------------------------------------------------------------------//
#ifdef ALG_MEASURE_BLACK_GLUE_USE
#include "..\\JetAlg\\JunAlgorithm\\MeasurementBlackGlue.h"
#endif//ALG_MEASURE_BLACK_GLUE_USE
//-------------------------------------------------------------------------------------//
#ifdef ALG_MEASURE_FLUX_AREA_USE
#include "..\\JetAlg\\JunAlgorithm\\MeasurementBlackGlue.h"
#endif//ALG_MEASURE_FLUX_AREA_USE
//-------------------------------------------------------------------------------------//
#ifdef ALG_MEASURE_CPU_PIN_USE
#include "..\\JetAlg\\JunAlgorithm\\CPUAlignment.h"
#endif//ALG_MEASURE_CPU_PIN_USE
//-------------------------------------------------------------------------------------//
#define    LINE_MODE_BRIGHT        1
#define    LINE_MODE_DARK          2
//-------------------------------------------------------------------------------------//
#define    ALG_DOCK_DISABLE        0
#define    ALG_DOCK_ENABLE         1
//-------------------------------------------------------------------------------------//
enum AI_RESULT_ID
{
	AI_RESULT_ID_NONE      = 0,//未檢測
	AI_RESULT_ID_OK        = 1,//良品
	AI_RESULT_ID_NG        = 2,//瑕疵
	AI_RESULT_ID_BYPASS    = 3,//不檢測
	AI_RESULT_ID_EXCEPTION = 4,//異常, 查看ErrorMessage
	AI_RESULT_ID_RETURN
};
//-------------------------------------------------------------------------------------//
enum ALG_AI_MODEL_ID//AI模型編號
{
	ALG_AI_MODEL_NONE       =    0,//關閉AI
	ALG_AI_MODEL_OCR_01     =    1,//AI-OCR-01
	ALG_AI_MODEL_OCR_02     =    2,//AI-OCR-02
	ALG_AI_MODEL_OCR_03     =    3,//AI-OCR-03
	ALG_AI_MODEL_SOLDER_01  = 1001,//AI-Solder-01
	ALG_AI_MODEL_SOLDER_02  = 1002,//AI-Solder-02
	ALG_AI_MODEL_SOLDER_03  = 1003,//AI-Solder-03
	ALG_AI_MODEL_AUTO_LABEL = 3001,//AI-Auto Label Component
	ALG_AI_MODEL_RETURN
};
//-------------------------------------------------------------------------------------//
enum ALG_CALC_UNIT_MODE//計算單位模式
{
	ALG_CALC_UNIT_ABS   = 1,  //絕對數值, 單位um, 
	ALG_CALC_UNIT_DIFF  = 2,  //相對差值, 單位um, 
	ALG_CALC_UNIT_RATIO = 3,  //絕對百分比, 單位%-percentage
	CALC_UNIT_RETURN
};
//-------------------------------------------------------------------------------------//
enum ALG_BRIGHT_AVERAGE_MODE//亮度平均的模式
{
	ALG_BRIGHT_AVERAGE_FULL      = 0,//全平均
	ALG_BRIGHT_AVERAGE_PARTIAL   = 1,//部分平均	
	ALG_BRIGHT_AVERAGE_RETURN
};
//-------------------------------------------------------------------------------------//
enum ALG_MATCH_DOCK_MODE//匹配的靠邊模式
{
	ALG_MATCH_DOCK_DISABLE     = 0,//關閉
	ALG_MATCH_DOCK_TO_TIP      = 1,//靠前端
	ALG_MATCH_DOCK_TO_SHOULDER = 2,//靠根部
	ALG_MATCH_DOCK_RETURN
};
//-------------------------------------------------------------------------------------//
enum ALG_3D_BASE_HEIGHT_MODE//3D基準高度模式
{
	ALG_3D_BASE_HEIGHT_MIN    = 1,//最小值
	ALG_3D_BASE_HEIGHT_MAX    = 2,//最大值
	ALG_3D_BASE_HEIGHT_AVE    = 3,//平均值
	ALG_3D_BASE_HEIGHT_MID    = 4,//中位數	
	ALG_3D_BASE_HEIGHT_SQR    = 5,//Least Square
	ALG_3D_BASE_HEIGHT_RETURN
};
//-------------------------------------------------------------------------------------//
enum ALG_GROUP_CMP_DIR_MODE//群組比較方向模式
{
	ALG_GROUP_CMP_DIR_ANY     = 0,//任方向
	ALG_GROUP_CMP_DIR_ONE     = 1,//同方向	
	ALG_GROUP_CMP_DIR_RETURN
};
//-------------------------------------------------------------------------------------//
enum ALG_SEARCH_DIRECTION
{
	SEARCH_DIRECTION_FORWARD  = 1,//同向
	SEARCH_DIRECTION_BACKWARD = 2,//反向
	SEARCH_DIRECTION_RETURN
};
//-------------------------------------------------------------------------------------//
enum ALG_EDGE_FEATURE_MODE//邊緣特徵模式
{
	ALG_EDGE_FEATURE_W2B  = 1,//白到黑(White to Black)
	ALG_EDGE_FEATURE_B2W  = 2,//黑道白(Black to White)
	ALG_EDGE_FEATURE_RETURN
};
//-------------------------------------------------------------------------------------//
enum ALG_BARCODE_DIR_MODE//條碼方向
{
	ALG_BARCODE_DIR_AUTO  =   0,
	ALG_BARCODE_DIR_HOR   =   1,
	ALG_BARCODE_DIR_VER   =   2,
	ALG_BARCODE_DIR_ALL   = 255,
	ALG_BARCODE_DIR_RETURN
};
//-------------------------------------------------------------------------------------//
enum ALG_OBJECT_SIZE_CALC_MODE//物件尺寸計算模式
{
	ALG_OBJECT_SIZE_CALC_BOUNDARY       =  1,//邊界模式
	ALG_OBJECT_SIZE_CALC_AVERAGE        =  2,//平均模式
	ALG_OBJECT_SIZE_CALC_AVE_RECT       =  3,//平均+邊界模式	
	ALG_OBJECT_SIZE_CALC_BLUR_RECT      =  4,//平滑矩形
	ALG_OBJECT_SIZE_CALC_RETURN
};
//-------------------------------------------------------------------------------------//
enum ALG_OBJECT_HEIGHT_AVERAGE_MODE//物件高度平均的模式
{
	ALG_OBJECT_HEIGHT_AVERAGE_FULL      = 0,//全平均
	ALG_OBJECT_HEIGHT_AVERAGE_PARTIAL   = 1,//部分平均	
	ALG_VOLUME_HEIGHT_AVERAGE_RETURN
};
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_BRIGHT_RATIO//亮度比例, 改成BrightReading,增加平均亮度上下限, 標準差與高度差
{	
	double                     brTargetValue;//目標高度
	bool                       brRoiBoxEnabled;//子框使用

	ALG_BRIGHT_AVERAGE_MODE    brAverageMode;//平均模式
	double                     brAverageScale;//平均比例
	bool                       brAverageScaleEnabled;//平均比例使用
	double                     brAveragePartialH;//平均範圍上限
	double                     brAveragePartialL;//平均範圍下限
	double                     brAverageReading;//平均讀值	
	double                     brAverageReadingMin;//平均-最小值
	double                     brAverageReadingMax;//平均-最大值

	bool                       brToleranceEnabled;//公差使用
	double                     brToleranceUSL;//公差上限
	double                     brToleranceLSL;//公差下限
	double                     brToleranceReading;//公差讀值	

	bool                       brLimitMaxEnabled;//極限最大使用
	bool                       brLimitMinEnabled;//極限最小使用	
	double                     brLimitTolUSL;//極限公差上限
	double                     brLimitTolLSL;//極限公差下限
	double                     brLimitMaxScale;//極限比例-最大
	double                     brLimitMinScale;//極限比例-最小
	bool                       brLimitMaxScaleEnabled;//極限最大值比例使用	
	bool                       brLimitMinScaleEnabled;//極限最大值比例使用	
	double                     brLimitReadingMin;//最小讀值
	double                     brLimitReadingMax;//最大讀值

	bool                       brRatioEnabled;//比例啟用
	double                     brRatioUSL;//比例上限
	double                     brRatioLSL;//比例下限
	double                     brRatioReading;//比例讀值	
	double                     brRatioArea;//比例物理面積

	bool                       brRangeEnabled;////高低差啟用
	double                     brRangeUSL;//高低差上限
	double                     brRangeLSL;//高低差下限
	double                     brRangeReading;//高低差讀值

	bool                       brContrastEnabled;//對比啟用
	double                     brContrastUSL;//對比上限
	double                     brContrastLSL;//對比下限
	double                     brContrastReading;//對比讀值

	double                     brXLineRange;//X軸貫穿縱向範圍-um
	bool                       brXLineEnabled;//X軸貫穿啟用
	int                        brXLineMode;//X軸貫穿模式, 白或黑
	ALG_CALC_UNIT_MODE         brXLineUnitMode;//X軸貫穿單位模式
	double                     brXLineUSL;//X軸貫穿上限
	double                     brXLineLSL;//X軸貫穿下限
	double                     brXLineReading;//X軸貫穿讀值

	double                     brYLineRange;//Y軸貫穿橫向範圍-um
	bool                       brYLineEnabled;//X軸貫穿啟用
	int                        brYLineMode;//Y軸貫穿模式, 白或黑
	ALG_CALC_UNIT_MODE         brYLineUnitMode;//Y軸貫穿單位模式
	double                     brYLineUSL;//X軸貫穿上限
	double                     brYLineLSL;//X軸貫穿下限
	double                     brYLineReading;//X軸貫穿讀值

	tagALG_PARAM_BRIGHT_RATIO()
	{		
		brTargetValue    =  255.0;
		brRoiBoxEnabled  = false;

		brAverageMode    = ALG_BRIGHT_AVERAGE_FULL;
		brAverageScale   = 100.0;
		brAverageScaleEnabled= false;
		brAveragePartialH =   75.0;
		brAveragePartialL =   25.0;
		brAverageReading =    0.0;
		brAverageReadingMin=  0.0;
		brAverageReadingMax=  0.0;

		brToleranceEnabled = true;
		brToleranceUSL   =  100;
		brToleranceLSL   = -100;
		brToleranceReading = 0.0;

		brLimitMaxEnabled=  false;
		brLimitMinEnabled=  false;		
		brLimitTolUSL   =     100;
		brLimitTolLSL   =    -100;
		brLimitMaxScale =     100;
		brLimitMinScale =     100;
		brLimitMaxScaleEnabled=false;
		brLimitMinScaleEnabled=false;
		brLimitReadingMin=    0.0;
		brLimitReadingMax=    0.0;

		brRatioEnabled   =  true;//比例啟用
		brRatioUSL       =  100.0;
		brRatioLSL       =    0.0;
		brRatioReading   =    0.0;
		brRatioArea      =    0.0;
		
		brRangeEnabled   =  false;
		brRangeUSL       = 1024.0;
		brRangeLSL       =    0.0;
		brRangeReading   =    0.0;

		brContrastEnabled=  false;
		brContrastUSL    = 1000.0;
		brContrastLSL    =    0.0;
		brContrastReading=    0.0;

		brXLineRange     =   50.0;
		brXLineEnabled   =   false;
		brXLineMode      =   LINE_MODE_BRIGHT;//X軸貫穿模式, 白或黑
		brXLineUnitMode  =   ALG_CALC_UNIT_RATIO;
		brXLineUSL       =   75.0;
		brXLineLSL       =    0.0;
		brXLineReading   =    0.0;

		brYLineRange     =   50.0;
		brYLineEnabled   =   false;
		brYLineMode      =   LINE_MODE_BRIGHT;//X軸貫穿模式, 白或黑
		brYLineUnitMode  =   ALG_CALC_UNIT_RATIO;
		brYLineUSL       =   75.0;
		brYLineLSL       =    0.0;
		brYLineReading   =    0.0;
	}
} TALG_PARAM_BRIGHT_RATIO, *PALG_PARAM_BRIGHT_RATIO;
//-------------------------------------------------------------------------------------//
enum ALG_DIRECTION//貫穿方向
{
	ALG_DIR_NONE    = 0,
	ALG_HORIZONTAL  = 1,//水平貫穿
	ALG_VERTICAL    = 2 //垂直貫穿
};
enum ALG_OUTER_SHORT_EXT_MODE//線延長模式
{
	ALG_OUTER_SHORT_EXT_NONE   = 0,//關閉
	ALG_OUTER_SHORT_EXT_LEFT   = 1,//左側延長
	ALG_OUTER_SHORT_EXT_RIGHT  = 2,//右側延長
	ALG_OUTER_SHORT_EXT_BOTH   = 3,//雙面延長
	ALG_OUTER_SHORT_EXT_RETURN
};

typedef struct tagALG_PARAM_OUTER_LINE//外部線段
{
	bool                       olEnabled;//啟用-右側
	double                     olRange;//範圍-um-右側
	int                        olMode;//模式-白或黑-右側
	ALG_OUTER_SHORT_EXT_MODE   olExtMode;//延長模式-右側
	double                     olUSL;//比例上限-右側
	double                     olLSL;//比例下限-右側
	double                     olReading;//比例讀值-右側	
	tagALG_PARAM_OUTER_LINE()
	{
		olEnabled  = true;
		olRange    = 50.0;
		olMode     = LINE_MODE_BRIGHT;
		olExtMode  = ALG_OUTER_SHORT_EXT_NONE;
		olUSL      = 75.0;
		olLSL      = 0.0;
		olReading  = 0.0;		
	}
} TALG_PARAM_OUTER_LINE, *PALG_PARAM_OUTER_LINE;

typedef struct tagALG_PARAM_OUTER_SHORT//外部連接
{
	TALG_PARAM_OUTER_LINE      osLine_R;//右側
	TALG_PARAM_OUTER_LINE      osLine_T;//上方
	TALG_PARAM_OUTER_LINE      osLine_L;//左側
	TALG_PARAM_OUTER_LINE      osLine_B;//下方

	tagALG_PARAM_OUTER_SHORT()
	{		
	}

	void SetAll(const TALG_PARAM_OUTER_LINE &osLine)
	{
		osLine_R = osLine;
		osLine_T = osLine;
		osLine_L = osLine;
		osLine_B = osLine;
	}	
} TALG_PARAM_OUTER_SHORT, *PALG_PARAM_OUTER_SHORT;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_BLOB_COUNT//亮度區塊
{	
	double                     bcXSizeMax;//區塊X尺寸最大值
	bool                       bcXSizeMaxEnabled;//區塊X尺寸最大值啟用
	double                     bcXSizeMin;//區塊X尺寸最小值
	bool                       bcXSizeMinEnabled;//區塊X尺寸最小值啟用
	double                     bcYSizeMax;//區塊Y尺寸最大值
	bool                       bcYSizeMaxEnabled;//區塊Y尺寸最大值啟用
	double                     bcYSizeMin;//區塊Y尺寸最小值
	bool                       bcYSizeMinEnabled;//區塊Y尺寸最小值啟用
	double                     bcLSizeMax;//區塊L尺寸最大值
	bool                       bcLSizeMaxEnabled;//區塊L尺寸最大值啟用
	double                     bcLSizeMin;//區塊L尺寸最小值
	bool                       bcLSizeMinEnabled;//區塊L尺寸最小值啟用	
	double                     bcAreaSizeMax;//區塊面積尺寸最大值
	bool                       bcAreaSizeMaxEnabled;//區塊面積尺寸最大值啟用
	double                     bcAreaSizeMin;//區塊面積尺寸最小值
	bool                       bcAreaSizeMinEnabled;//區塊面積尺寸最小值啟用
	double                     bcAspectRatioMax;//區塊長寬比最大值
	bool                       bcAspectRatioMaxEnabled;//區塊長寬比最大值啟用
	double                     bcAspectRatioMin;//區塊長寬比最小值
	bool                       bcAspectRatioMinEnabled;//區塊長寬比最小值啟用
	double                     bcFillRatioMax;//區塊填滿率最大值
	bool                       bcFillRatioMaxEnabled;//區塊填滿率最大值啟用
	double                     bcFillRatioMin;//區塊填滿率最小值
	bool                       bcFillRatioMinEnabled;//區塊填滿率最小值啟用
	double                     bcLongShortRatioMax;//長短比例最大值
	bool                       bcLongShortRatioMaxEnabled;//長短比例最大值啟用
	double                     bcLongShortRatioMin;//長短比例最小值
	bool                       bcLongShortRatioMinEnabled;//長短比例最小值啟用

	int                        bcConnectivity;//4鄰近或8鄰近(BLOB_CONNECTIVITY_4, BLOB_CONNECTIVITY_8)
	float                      bcRoiBoxExtendX;//小框延伸X尺寸-um
	float                      bcRoiBoxExtendY;//小框延伸Y尺寸-um

	int                        bcCountUSL;//區塊數量最大值
	int                        bcCountLSL;//區塊數量最小值
	int                        bcCountNum;//區塊數量
	tagALG_PARAM_BLOB_COUNT()
	{
		bcXSizeMax = 0;
		bcXSizeMaxEnabled = false;
		bcXSizeMin = 0;
		bcXSizeMinEnabled = false;
		bcYSizeMax = 0;
		bcYSizeMaxEnabled = false;
		bcYSizeMin = 0;
		bcYSizeMinEnabled = false;
		bcLSizeMax = 0;
		bcLSizeMaxEnabled = false;
		bcLSizeMin = 0;
		bcLSizeMinEnabled = false;
		bcAreaSizeMax = 0;
		bcAreaSizeMaxEnabled = false;
		bcAreaSizeMin = 0;
		bcAreaSizeMinEnabled = false;
		bcAspectRatioMax = 0;
		bcAspectRatioMaxEnabled = false;
		bcAspectRatioMin = 0;
		bcAspectRatioMinEnabled = false;

		bcFillRatioMax = 100;//區塊填滿率最大值
		bcFillRatioMaxEnabled = false;//區塊填滿率最大值啟用
		bcFillRatioMin = 0;//區塊填滿率最小值
		bcFillRatioMinEnabled = false;//區塊填滿率最小值啟用

		bcLongShortRatioMax = 100;
		bcLongShortRatioMaxEnabled = false;
		bcLongShortRatioMin = 100;
		bcLongShortRatioMinEnabled = false;		

		bcConnectivity = 4;
		bcRoiBoxExtendX = 0.0f;
		bcRoiBoxExtendY = 0.0f;	

		bcCountUSL = 1;
		bcCountLSL = 0;
		bcCountNum = 0;
	}
} TALG_PARAM_BLOB_COUNT, *PALG_PARAM_BLOB_COUNT;
//-------------------------------------------------------------------------------------//
enum ALG_TILE_CELL_MODE//傾斜計算模式
{
	ALG_TILE_CELL_HOR      = 1,//水平
	ALG_TILE_CELL_VER      = 2,//垂直
	ALG_TILE_CELL_CORNER   = 4,//角落
	ALG_TILE_CELL_QUAD     = 5,//四邊
	ALG_TILE_CELL_OCTA     = 8 //八邊
};

typedef struct tagALG_PARAM_BODY_TILT//本體傾斜
{
	double                     btSizeRatioX;//Cell尺寸比例-X
	double                     btSizeRatioY;//Cell尺寸比例-Y
	ALG_TILE_CELL_MODE         btCellMode;//傾斜模式(水平, 垂直, 對腳, 四角, 四邊, 八點)
	double                     btTiltGapUSL;//差距上限
	double                     btTiltGapLSL;//差距下限
	double                     btTiltAngleUSL;//角度上限
	double                     btTiltAngleLSL;//角度下限

	double                     btTiltGapReading;//差距讀值
	double                     btTiltAngleReading;//角度讀值	
	tagALG_PARAM_BODY_TILT()
	{
		btSizeRatioX = 25;
		btSizeRatioY = 25;
		btCellMode = ALG_TILE_CELL_CORNER;
		btTiltGapUSL = 250.0;
		btTiltGapLSL =   0.0;		
		btTiltAngleUSL = 5.0;
		btTiltAngleLSL = 0.0;

		btTiltGapReading = 0; 
		btTiltAngleReading = 0; 
	}
} TALG_PARAM_BODY_TILT, *PALG_PARAM_BODY_TILT;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_MODEL_MATCH//模板匹配
{		
	ALG_MATCH_DOCK_MODE        mmDockMode;//停靠模式	
	int                        mmReCheckBox;//重新確認特徵
	tagALG_PARAM_MODEL_MATCH()
	{			
		mmDockMode = ALG_MATCH_DOCK_DISABLE;		
		mmReCheckBox = FN_DISABLE;
	}
} TALG_PARAM_MODEL_MATCH, *PALG_PARAM_MODEL_MATCH;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_IMAGE_MATCH//影像匹配
{	
	bool                       imPxlCmpEnabled;//像素比較//PxlCmp[Pixel Compare]
	int                        imPxlCmpDarkLevel;//像素比較-過暗定義
	int                        imPxlCmpTolerance;//像素比較-誤差允許	
	int                        imPxlCmpOpenSize;//雜訊過濾-Open
	int                        imPxlCmpCloseSize;//雜訊過濾-Close
	int                        imPxlCmpGaussianSize;//雜訊過濾-高斯
	double                     imPxlCmpXSizeMin;//像素比較X尺寸下限-um
	double                     imPxlCmpYSizeMin;//像素比較Y尺寸下限-um
	double                     imPxlCmpAreaMin;//像素比較面積下限-um
	int                        imPxlCmpCountLSL;//像素比較數量下限
	int                        imPxlCmpCountUSL;//像素比較數量上限

	double                     imPxlCmpCountNum;//像素比較數量	
	tagALG_PARAM_IMAGE_MATCH()
	{	
		imPxlCmpEnabled = false;
		imPxlCmpDarkLevel = 15;//像素比較-過暗定義
		imPxlCmpTolerance = 50;;//像素比較-誤差允許		
		imPxlCmpOpenSize = 5;
		imPxlCmpCloseSize = 3;		
		imPxlCmpGaussianSize = 15;
		imPxlCmpXSizeMin = 50;
		imPxlCmpYSizeMin = 50;
		imPxlCmpAreaMin  = 2500;
		imPxlCmpCountNum = 0;		
		imPxlCmpCountLSL = 0;
		imPxlCmpCountUSL = 0;
	}
} TALG_PARAM_IMAGE_MATCH, *PALG_PARAM_IMAGE_MATCH;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_CHAR_VERIFY//文字驗證
{	
	int                        cvCellGridCnt;//單元細分數量
	int                        cvCellExtSize;//單元外擴尺寸-pxl
	double                     cvCellScoreMax;//單元成績上限
	double                     cvCellScoreMin;//單元成績下限
	double                     cvPassRatioUSL;//通過比例上限
	double                     cvPassRatioLSL;//通過比例下限

	double                     cvPassRatioReading;//通過比例讀值
	
	tagALG_PARAM_CHAR_VERIFY()
	{	
		cvCellGridCnt = 1;
		cvCellExtSize = 0;
		cvCellScoreMax = 100.0;
		cvCellScoreMin =  65.0;
		cvPassRatioUSL = 100.0;
		cvPassRatioLSL = 90.0;

		cvPassRatioReading = 0.0;
	}
} TALG_PARAM_CHAR_VERIFY, *PALG_PARAM_CHAR_VERIFY;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_GROUP_COMPARE//群組比較
{
	ALG_GROUP_CMP_DIR_MODE     gcDirectionMode;    //方向模式 
	bool                       gc2DGrayEnabled;    //2D影像啟用
	double                     gc2DGrayUSL;       //2D上限
	double                     gc2DGrayLSL;       //2D下限	
	ALG_3D_BASE_HEIGHT_MODE    gc3DHeightBaseMode;  //3D高度基準模式
	bool                       gc3DHeightEnabled;   //3D高度啟用
	double                     gc3DHeightUSL;       //3D上限
	double                     gc3DHeightLSL;       //3D下限	
	bool                       gcTiltAngleEnabled; //傾斜角度啟用
	double                     gcTiltAngleUSL;    //傾斜角度上限
	double                     gcTiltAngleLSL;    //傾斜角度下限	
	
	double                     gc2DGrayValue;     //2D讀值
	double                     gc2DGrayReading;   //2D讀值
	double                     gc3DHeightBase;     //3D讀值-基準面
	double                     gc3DHeightBasePlaneA;     //3D讀值-基準面擬合結果
	double                     gc3DHeightBasePlaneB;     //3D讀值-基準面擬合結果
	double                     gc3DHeightBasePlaneC;     //3D讀值-基準面擬合結果
	double                     gc3DHeightValue;     //3D讀值
	double                     gc3DHeightReading;   //3D讀值
	double                     gcTiltAngleReading;//傾斜角度讀值
	RESULT_ID                  gcResultID;    //比較結果
	
	tagALG_PARAM_GROUP_COMPARE()
	{
		gcDirectionMode  = ALG_GROUP_CMP_DIR_ANY;
		gc2DGrayEnabled = false;
		gc2DGrayUSL    = 255.0;
		gc2DGrayLSL    =   0.0;		
		gc3DHeightBaseMode = ALG_3D_BASE_HEIGHT_MIN;//3D高度基準模式
		gc3DHeightEnabled =  false;
		gc3DHeightUSL    =  50.0;
		gc3DHeightLSL    = -50.0;
		gcTiltAngleEnabled = false;
		gcTiltAngleUSL = 10;
		gcTiltAngleLSL =-10;

		gc2DGrayValue  = 0.0;
		gc2DGrayReading  = 0.0;
		gc3DHeightBase  = 0.0;
		gc3DHeightBasePlaneA = 0.0;
		gc3DHeightBasePlaneB = 0.0;
		gc3DHeightBasePlaneC = 0.0;
		gc3DHeightValue  = 0.0;
		gc3DHeightReading  = 0.0;
		gcTiltAngleReading = 0.0;
		gcResultID  = RESULT_ID_NONE;		
	}
} TALG_PARAM_GROUP_COMPARE, *PALG_PARAM_GROUP_COMPARE;
//-------------------------------------------------------------------------------------//
#define BARCODE_CONTENT_SIZE              128
#define MAX_BARCODE_DECODE_COUNT            8

enum ALG_BARCODE_STEP_MODE//條碼步驟模式
{
	ALG_BARCODE_STEP_NONE         =  0,//無意義
	ALG_BARCODE_STEP_SCALE        =  1,//縮放
	ALG_BARCODE_STEP_GAIN_OFFSET  =  2,//增益與偏移
	ALG_BARCODE_STEP_SMOOTH       =  3,//平均濾波
	ALG_BARCODE_STEP_OPEN         =  4,//開運算
	ALG_BARCODE_STEP_CLOSE        =  5,//閉運算
	ALG_BARCODE_STEP_MEDIAN       =  6,//中值濾波
	ALG_BARCODE_STEP_INVERT       =  7,//反相處理
	ALG_BARCODE_STEP_FLIP         =  8,//翻轉或鏡射
	ALG_BARCODE_STEP_FILL         =  9,//填滿外圈
	ALG_BARCODE_STEP_ERODE        = 10,//侵蝕
	ALG_BARCODE_STEP_DILATE       = 11,//膨脹
	ALG_BARCODE_STEP_FILL_2D      = 12,//填滿外圈-2D
	ALG_BARCODE_STEP_SHARP        = 13,//銳利化
	ALG_BARCODE_STEP_GRAY_RANGE   = 14,//灰階範圍
	ALG_BARCODE_STEP_RETURN
};

typedef struct tagALG_BARCODE_STEP_PARAM
{
	ALG_BARCODE_STEP_MODE     BarcodeStep;
	double                    Param1;
	double                    Param2;
	bool                      Enabled;
	bool                      Decoded;
	tagALG_BARCODE_STEP_PARAM()
	{
		BarcodeStep = ALG_BARCODE_STEP_NONE;
		Param1 = 1.0;
		Param2 = 1.0;
		Enabled = true;
		Decoded = false;
	}
} TALG_BARCODE_STEP_PARAM, *PALG_BARCODE_STEP_PARAM;

typedef struct tagALG_PARAM_BARCODE_RECOGNIZE
{
	bool                       br1DCodeEnabled;     //1D條碼啟用
	bool                       brQRCodeEnabled;     //QRCode條碼啟用
	bool                       brDataMatrixEnabled; //DataMatrix條碼啟用
	BARCODE_DECODER_TYPE       br1DCodeDecoderType; //1D條碼解碼器
	BARCODE_DECODER_TYPE       brQRCodeDecoderType; //QRCode條碼解碼器
	BARCODE_DECODER_TYPE       brDataMatrixDecoderType;//DataMatrix條碼解碼器

	int                        brDecodeTimeout;     //解碼逾時
	int                        brDecodeStartStep;   //解碼開始步驟
	bool                       brCheckSumEnabled;//條碼的CheckSum啟用
	TALG_BARCODE_STEP_PARAM    brDecodeStep[MAX_BARCODE_DECODE_COUNT];//解碼步驟
	int                        brCodeCount;         //條碼長度
	bool                       brCodeCountEnabled;  //條碼長度	
	ALG_BARCODE_DIR_MODE       brCodeDirectionMode;
	std::wstring               brBarcodeContent;//條碼內容
	bool                       brCodeContentEnabled;//條碼容啟用
	bool                       brCodeJSONCheckEnabled;//條碼JOSN確認啟用	
	double                     brVerifyUSL;//驗證成績上限
	double                     brVerifyLSL;//驗證成績下限
	int                        brCodeCountReading;//條碼長度讀值
	double                     brVerifyReading;//驗證成績讀值
	std::wstring               brBarcodeResult;//條碼結果
	
	tagALG_PARAM_BARCODE_RECOGNIZE()
	{
		br1DCodeEnabled = false;
		brQRCodeEnabled = false;
		brDataMatrixEnabled = true;
		br1DCodeDecoderType = BARCODE_DECODER_DTK;
		brQRCodeDecoderType = BARCODE_DECODER_DTK;
		brDataMatrixDecoderType = BARCODE_DECODER_DTK;

		brDecodeTimeout = 0;		
		brDecodeStartStep = 0;
		brCheckSumEnabled = true;
		brDecodeStep[0].BarcodeStep = ALG_BARCODE_STEP_SCALE;
		brDecodeStep[0].Param1 = 0.5;
		brDecodeStep[0].Param2 = 1.0;
		brDecodeStep[1].BarcodeStep = ALG_BARCODE_STEP_NONE;
		brDecodeStep[1].Param1 = 1.0;
		brDecodeStep[1].Param2 = 1.0;
		brDecodeStep[2].BarcodeStep = ALG_BARCODE_STEP_NONE;
		brDecodeStep[2].Param1 = 1.0;
		brDecodeStep[2].Param2 = 1.0;
		brDecodeStep[3].BarcodeStep = ALG_BARCODE_STEP_NONE;
		brDecodeStep[3].Param1 = 1.0;
		brDecodeStep[3].Param2 = 1.0;
		brDecodeStep[4].BarcodeStep = ALG_BARCODE_STEP_NONE;
		brDecodeStep[4].Param1 = 1.0;
		brDecodeStep[4].Param2 = 1.0;
		brDecodeStep[5].BarcodeStep = ALG_BARCODE_STEP_NONE;
		brDecodeStep[5].Param1 = 1.0;
		brDecodeStep[5].Param2 = 1.0;
		brDecodeStep[6].BarcodeStep = ALG_BARCODE_STEP_NONE;
		brDecodeStep[6].Param1 = 1.0;
		brDecodeStep[6].Param2 = 1.0;
		brDecodeStep[7].BarcodeStep = ALG_BARCODE_STEP_NONE;
		brDecodeStep[7].Param1 = 1.0;
		brDecodeStep[7].Param2 = 1.0;
		
		brCodeCount = 0;
		brCodeCountEnabled = false;
		brBarcodeContent = L"";
		brCodeContentEnabled = false;
		brCodeJSONCheckEnabled = true;		
		brCodeDirectionMode = ALG_BARCODE_DIR_ALL;
		brVerifyUSL = 100;
		brVerifyLSL = 90;
		brCodeCountReading = 0;
		brVerifyReading = 0;
		brBarcodeResult = L"";		
	}

} TALG_PARAM_BARCODE_RECOGNIZE, *PALG_PARAM_BARCODE_RECOGNIZE;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_OBJECT_MEASURE//物件量測
{		
	bool                       omUseMultiBlob;//多個物件
	int                        omSizeBlurSize;//尺寸平滑大小
	ALG_OBJECT_SIZE_CALC_MODE  omSizeCalcMode;//尺寸計算模式
	ALG_CALC_UNIT_MODE         omSizeCalcUnitMode;//尺寸計算單位模式um,%

	double                     omSizeXSpec;//寬度規格-um
	double                     omSizeXDiffUSL;//寬度上限-um
	double                     omSizeXDiffLSL;//寬度下限-um
	double                     omSizeXRatioUSL;//寬度上限-%
	double                     omSizeXRatioLSL;//寬度下限-%
	bool                       omSizeXEnabled;//寬度啟用

	double                     omSizeYSpec;//長度規格-um
	double                     omSizeYDiffUSL;//長度上限-um
	double                     omSizeYDiffLSL;//長度下限-um
	double                     omSizeYRatioUSL;//長度上限-%
	double                     omSizeYRatioLSL;//長度下限-%
	bool                       omSizeYEnabled;//長度啟用

	double                     omHeightSpec;//高度規格
	double                     omHeightDiffUSL;//高度上限-um
	double                     omHeightDiffLSL;//高度下限-um
	double                     omHeightRatioUSL;//高度上限-%
	double                     omHeightRatioLSL;//高度下限-%
	bool                       omHeightEnabled;//高度啟用	
	double                     omHeightAveragePartialH;//高度平均範圍上限
	double                     omHeightAveragePartialL;//高度平均範圍下限
	ALG_OBJECT_HEIGHT_AVERAGE_MODE  omHeightAverageMode;//高度平均模式
	ALG_CALC_UNIT_MODE         omHeightCalcUnitMode;//高度計算單位模式um,%

	double                     omAreaSpec;//面積規格
	double                     omAreaUSL;//面積上限-%
	double                     omAreaLSL;//面積下限-%
	bool                       omAreaEnabled;//面積啟用

	double                     omVolumeSpec;//體積規格
	double                     omVolumeUSL;//體積上限-%
	double                     omVolumeLSL;//體積下限-%
	bool                       omVolumeEnabled;//體積啟用
	
	double                     omSizeXReading;//寬度讀值
	double                     omSizeYReading;//長度讀值
	double                     omHeightReading;//高度讀值
	double                     omAreaReading;//面積讀值
	double                     omVolumeReading;//體積讀值

	tagALG_PARAM_OBJECT_MEASURE()
	{
		omUseMultiBlob = false;
		omSizeBlurSize = 5;
		omSizeCalcMode = ALG_OBJECT_SIZE_CALC_AVERAGE;
		omSizeCalcUnitMode = ALG_CALC_UNIT_DIFF;

		omSizeXSpec = 1000;
		omSizeXDiffUSL =  150;
		omSizeXDiffLSL = -150;
		omSizeXRatioUSL =  125.0;
		omSizeXRatioLSL =   75.0;
		omSizeXEnabled = false;

		omSizeYSpec = 1000;
		omSizeYDiffUSL  =  150;
		omSizeYDiffLSL  = -150;
		omSizeYRatioUSL  =  125.0;
		omSizeYRatioLSL  =   75.0;
		omSizeYEnabled = false;

		omHeightSpec = 1000.0;//um
		omHeightDiffUSL =  100;
		omHeightDiffLSL = -100;
		omHeightRatioUSL = 125.0;
		omHeightRatioLSL =  75;
		omHeightEnabled = true;
		omHeightAveragePartialH = 75;
		omHeightAveragePartialL = 25;
		omHeightAverageMode=ALG_OBJECT_HEIGHT_AVERAGE_FULL;
		omHeightCalcUnitMode = ALG_CALC_UNIT_DIFF;

		omAreaSpec  =  1000.0;
		omAreaUSL   = 125.0;
		omAreaLSL   =  75.0;
		omAreaEnabled = true;

		omVolumeSpec = omAreaSpec*omHeightSpec;
		omVolumeUSL   = 125.0;
		omVolumeLSL   =  75.0;
		omVolumeEnabled = true;

		omSizeXReading = 0;
		omSizeYReading = 0;
		omHeightReading = 0;
		omAreaReading = 0;
		omVolumeReading = 0;
	}
} TALG_PARAM_OBJECT_MEASURE, *PALG_PARAM_OBJECT_MEASURE;
//-------------------------------------------------------------------------------------//
enum FD_MATCH_MODE
{
	FD_MATCH_MODEL  = 1,
	FD_MATCH_IMAGE  = 2
};
typedef struct tagALG_PARAM_FD_MATCH//定位點匹配
{		
	int                        fmFillSize;//填滿尺寸	
	FD_MATCH_MODE              fmMatchMode;//匹配模式-影像或者區塊
	tagALG_PARAM_FD_MATCH()
	{			
		fmFillSize = 0;		
		fmMatchMode = FD_MATCH_IMAGE;
	}
} TALG_PARAM_FD_MATCH, *PALG_PARAM_FD_MATCH;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_COLOR_CODE//色碼檢測
{		
	int                        ccPolarityNum;//色碼方向1, 2	
	double                     ccCellScoreMax;//單元成績上限
	double                     ccCellScoreMin;//單元成績下限
	double                     ccPassRatioUSL;//通過比例上限
	double                     ccPassRatioLSL;//通過比例下限

	double                     ccPassRatioReading;//通過比例讀值
	tagALG_PARAM_COLOR_CODE()
	{			
		ccPolarityNum = 1;
		ccCellScoreMax = 100;
		ccCellScoreMin = 50;
		ccPassRatioUSL = 100;
		ccPassRatioLSL = 90;
		ccPassRatioReading = 0;
	}
} TALG_PARAM_COLOR_CODE, *PALG_PARAM_COLOR_CODE;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_EDGE_SEARCH//邊緣搜尋
{
	bool                       esCutLineEnabled;//切斷線-中央切開
	bool                       esInvertAlign;//反向對齊

	bool                       esEnabled_F;//啟動-前端		
	double                     esMinRatioU_F;//最小寬度比例-前端	
	double                     esMaxRatioU_F;//最大寬度比例-前端	
	double                     esMinRangeV_F;//最小長度範圍-前端
	double                     esMaxRangeV_F;//最大長度範圍-前端
	double                     esSizeRatioV_F;//尺寸比例長度-前端
	double                     esSearchRatioV_F;//搜尋比例長度-後端
	ALG_SEARCH_DIRECTION       esSearchDir_F;//搜尋方向模式-前端
	ALG_OBJECT_SIZE_CALC_MODE  esSizeCalcMode_F;//尺寸計算模式-前端
	RESULT_ID                  esResultID_F;//搜尋結果-前端
	

	bool                       esEnabled_B;//啟動-後端	
	double                     esMinRatioU_B;//最小寬度比例-後端
	double                     esMaxRatioU_B;//最大寬度比例-後端
	double                     esMinRangeV_B;//最小長度比例-後端
	double                     esMaxRangeV_B;//最大長度比例-後端
	double                     esSizeRatioV_B;//尺寸比例長度-後端
	double                     esSearchRatioV_B;//搜尋比例長度-後端
	ALG_SEARCH_DIRECTION       esSearchDir_B;//搜尋方向模式-後端
	ALG_OBJECT_SIZE_CALC_MODE  esSizeCalcMode_B;//尺寸計算模式-後端
	RESULT_ID                  esResultID_B;//搜尋結果-後端

	tagALG_PARAM_EDGE_SEARCH()
	{
		esCutLineEnabled = false;
		esInvertAlign = false;

		esEnabled_F = true;
		esMinRatioU_F =  70;
		esMaxRatioU_F = 130;
		esMinRangeV_F = 100;
		esMaxRangeV_F = 100000;
		esSizeRatioV_F = 50;
		esSearchRatioV_F = 100;
		esSearchDir_F = SEARCH_DIRECTION_BACKWARD;
		esSizeCalcMode_F = ALG_OBJECT_SIZE_CALC_AVERAGE;
		esResultID_F = RESULT_ID_NONE;

		esEnabled_B = false;
		esMinRatioU_B =  70;
		esMaxRatioU_B = 130;
		esMinRangeV_B = 100;
		esMaxRangeV_B = 100000;
		esSizeRatioV_B = 50;
		esSearchRatioV_B = 100;
		esSearchDir_B = SEARCH_DIRECTION_FORWARD;
		esSizeCalcMode_B = ALG_OBJECT_SIZE_CALC_AVERAGE;
		esResultID_B = RESULT_ID_NONE;
	}
} TALG_PARAM_EDGE_SEARCH, *PALG_PARAM_EDGE_SEARCH;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_SHAPE_VERIFY//形狀驗證-外形檢測
{	
	float                      svSkipRatioInner;//跳過內圓的比例-內部
	float                      svSkipRatioOuter;//跳過外圓的比例-外部

	float                      svCircleR;//圓形-R-半徑

	float                      svOuterTol;//外圓半徑公差
	float                      svInnerTol;//內圓半徑公差
	float                      svRangeTol;//外內範圍公差
	float                      svErrorTol;//平均誤差公差	
	
	float                      svReadingRInner;//長度讀值-半徑-最小	
	float                      svReadingROuter;//長度讀值-半徑-最大
	float                      svReadingRAverage;//長度讀值-半徑-平均	
	tagALG_PARAM_SHAPE_VERIFY()
	{	
		svSkipRatioInner = 75.0f;
		svSkipRatioOuter = 125.0f;

		svCircleR = 1;

		svOuterTol =  50.0;
		svInnerTol =  50.0;
		svRangeTol = 100.0;
		svErrorTol =  50.00;		

		svReadingRInner = 0;		
		svReadingROuter = 0;
		svReadingRAverage = 0;		
	}
} TALG_PARAM_SHAPE_VERIFY, *PALG_PARAM_SHAPE_VERIFY;
//-------------------------------------------------------------------------------------//
enum ANGLE_MEASURE_MODE//角度量測模式
{
	ANGLE_MEASURE_SKEW       = 1,//旋轉角度
	ANGLE_MEASURE_TILT       = 2,//傾斜角度
	ANGLE_MEASURE_RETURN         
};
//-------------------------------------------------------------------------------------//
enum LINE_EQUATION_MODE//線方程式模式
{
	LINE_EQUATION_CALC      = 0,//計算出
	LINE_EQUATION_HOR       = 1,//水平線00
	LINE_EQUATION_VER       = 2,//垂直線90
	LINE_EQUATION_RETURN
};
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_ANGLE_MEASURE//角度量測-二線段夾角
{	
	ANGLE_MEASURE_MODE         amAngleMode;//角度模式
	LINE_EQUATION_MODE         amBaseLineMode;//基準線模式
	double                     amAngleSpec;//角度規格
	double                     amAngleTolUSL;//公差上限
	double                     amAngleTolLSL;//公差下限
	double                     amAngleReading;//角度讀值	

	double                     amHeightA;
	double                     amHeightB;
	TPOINT2D                   amLineResultPtA_1;
	TPOINT2D                   amLineResultPtB_1;
	TPOINT2D                   amLineResultPtA_2;
	TPOINT2D                   amLineResultPtB_2;		

	tagALG_PARAM_ANGLE_MEASURE()
	{		
		amAngleMode = ANGLE_MEASURE_SKEW;
		amBaseLineMode = LINE_EQUATION_CALC;
		amAngleSpec = 0;
		amAngleTolUSL = 10;
		amAngleTolLSL =-10;
		amAngleReading = 0.0;

		amHeightA = 0.0;
		amHeightB = 0.0;
		amLineResultPtA_1 = TPOINT2D();
		amLineResultPtB_1 = TPOINT2D();
		amLineResultPtA_2 = TPOINT2D();
		amLineResultPtB_2 = TPOINT2D();
	}
} TALG_PARAM_ANGLE_MEASURE, *PALG_PARAM_ANGLE_MEASURE;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_PIXEL_COMPARE//像素比較
{		
	int                        pcCellExtSize;//單元外擴尺寸-pxl

	int                        pcImgBlurSize;//影像模糊尺寸	

	int                        pcPatDarkLevel;//樣板過暗不處理
	int                        pcPatLightLevel;//樣板過亮不處理	
	int                        pcPatErodeSize;//樣板侵蝕尺寸
	bool                       pcPatPureColor;//樣板純色模式

	float                      pcCmpDarkGain;//比較圖的暗部增益
	float                      pcCmpLightGain;//比較圖的亮部增益	
	int                        pcCmpTolerance;//比較圖的灰階公差
	int                        pcCmpGaussianSize;//比較圖的高斯濾波
	int                        pcCmpOpenSize;//比較圖的開運算尺寸
	int                        pcCmpCloseSize;//比較圖的閉運算尺寸	
	
	double                     pcBlobMinSizeD;//區塊最小長度-um
	int                        pcBlobCountUSL;//區塊數量上限
	int                        pcBlobCountLSL;//區塊數量下限

	int                        pcBlobCountNum;//區塊數量
	
	tagALG_PARAM_PIXEL_COMPARE()
	{	
		pcCellExtSize = 4;

		pcImgBlurSize = 3;

		pcPatDarkLevel = 80;
		pcPatLightLevel = 240;		
		pcPatErodeSize = 3;
		pcPatPureColor = false;

		pcCmpDarkGain = 1.0f;
		pcCmpLightGain = 1.0f;
		pcCmpTolerance = 25;
		pcCmpGaussianSize = 3;
		pcCmpOpenSize = 3;
		pcCmpCloseSize = 0;		
		
		pcBlobMinSizeD = 30;
		pcBlobCountUSL = 0;
		pcBlobCountLSL = 0;

		pcBlobCountNum = 0;
	}
} TALG_PARAM_PIXEL_COMPARE, *PALG_PARAM_PIXEL_COMPARE;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_IPC_PRODUCT//像素比較
{
	bool				   ipcEnabled;
	int					   ipcX;
	int					   ipcY;
	int					   ipcContinuousPixel;
	int					   ipcGapPixel;
	int                    ipcWidthRatio;
	int					   ipcResult;


	tagALG_PARAM_IPC_PRODUCT()
	{
		ipcEnabled = false;
		ipcX = 0;
		ipcY = 0;
		ipcContinuousPixel = 0;
		ipcGapPixel = 0;
		ipcWidthRatio = 1;
		ipcResult = 0;
	}
} TALG_PARAM_IPC_PRODUCT, *PALG_PARAM_IPC_PRODUCT;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_RESIN_HEIGHT//高度檢測
{
	bool enabled;
	bool enableDoubleCheck;
	// 檢測類型, 1=錫高, 2=膠高
	int nInspectionType;

	// 錫高輸出方式: (預設為 1) // 1 : 平均值// 2 : 最大值
	int nOutputType;

	// 錫(膠)的方向, 1=垂直, 2=水平, 3=垂直水平都有, 4=零件的四個角, 5=垂直水平+四個角
	int nDirection;

	// 計算高度的範圍(pixels)
	int nMeasureRange;

	// 高度偵測的移動距離, 預設為 50(um)
	int nStepZ;

	// 錫高量測位置 = 3D定位後再往外偏移 nShift_Tin (pixels), 預設為 1
	int nShift_Tin;

	// 零件寬度的高度差允收值, 預設為 2
	int nThresholdZ_Width;

	// 零件高度的高度差允收值, 預設為 2
	int nThresholdZ_Height;

	// MeasureMode, 0=量ratio 1=量高度
	int nPartHeightMeasureMode;
	int nPartHeightThreshold;

	std::vector<std::vector<POINT>> partPoint;
	std::vector<std::vector<POINT>> tinPoint;
	int resultID;
	int resultH;

	tagALG_PARAM_RESIN_HEIGHT()
	{
		enabled = false;
		enableDoubleCheck = true;
		nInspectionType = 1;
		nOutputType = 1;
		nDirection = 2;
		nMeasureRange = 3;
		nStepZ = 50;
		nShift_Tin = 5;
		nThresholdZ_Width = 2;
		nThresholdZ_Height = 2;
		nPartHeightMeasureMode = 0;
		nPartHeightThreshold = 25;
		resultID = 0;
		resultH = 0;
	}
} TALG_PARAM_RESIN_HEIGHT, *PALG_PARAM_RESIN_HEIGHT;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_WIRE_WIDTH//金線寬度檢測
{
	// 預設為 false(不開啟)
	// 是否進行影像濾波,背景有嚴重干擾再開啟
	bool bFilter;

	// 預設為 31, "bFilter" 必須為true才會執行
	// 濾波大小
	int nFilterSize;

	// 預設為 1
	// 1 = Canny找邊
	// 2 = 二值化找邊
	int nEdgeMode;

	// 預設為 500
	// Canny低閥值
	int nLowThres;

	// 預設為 1500
	// Canny高閥值
	int nHeightThres;

	// 預設為 0
	// 影像高 = pu8Image的高度
	int nImageH;

	// 預設為 0
	// 影像寬 = pu8Image的寬度
	int nImageW;

	// 預設為 0
	// 零件量測範圍
	RECT rectPartRange;

	// 預設為 nullptr
	// pu8Image = 高角度白光彩色影像(檢測框影像)
	BYTE* pu8Image;

	double widthUSL;
	double widthLSL;

	// 旋轉角度
	float fAngle;

	// 線寬(pixel)
	float fWidth;

	// 計算線寬的兩個端點(顯示用)
	POINT ptLimit1;
	POINT ptLimit2;

	// 物體中心線(顯示用)
	POINT ptCenterLine_Start;
	POINT ptCenterLine_End;

	tagALG_PARAM_WIRE_WIDTH()
	{
		bFilter = false;
		nEdgeMode = 1;
		nFilterSize = 31;
		nImageH = 0;
		nImageW = 0;
		nLowThres = 500;
		nHeightThres = 1500;
		rectPartRange.left = 0;
		rectPartRange.right = 0;
		rectPartRange.top = 0;
		rectPartRange.bottom = 0;
		pu8Image = nullptr;

		widthUSL = 100;
		widthLSL = 0;

		fAngle = 0.0;
		fWidth = 0.0;
		ptLimit1.x = 0;
		ptLimit1.y = 0;
		ptLimit2.x = 0;
		ptLimit2.y = 0;
		ptCenterLine_Start.x = 0;
		ptCenterLine_Start.y = 0;
		ptCenterLine_End.x = 0;
		ptCenterLine_End.y = 0;
	}
} TALG_PARAM_WIRE_WIDTH, *PALG_PARAM_WIRE_WIDTH;
//-------------------------------------------------------------------------------------//
#define AI_MODEL_VERSION         L"1.1.0"
typedef struct tagALG_PARAM_AI_CHAR//AI模型
{
	std::wstring          acChar;
	RECT                  acCharRect;
	float                 acCharConfidence;
	tagALG_PARAM_AI_CHAR()
	{
		acChar = L"";
		acCharConfidence = 0.0f;
		::memset(&acCharRect, 0x00, sizeof(acCharRect));
	}
} TALG_PARAM_AI_CHAR, *PALG_PARAM_AI_CHAR;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_AI_MODEL//AI模型
{
	ALG_AI_MODEL_ID        aiModelID;
	float                  aiPatternAngle;//樣板角度
	float                  aiConfidenceThreshold;//信心度閥值
	int                    aiCharMatchNumThreshold;//字元相符數量閥值
	int                    aiCharNumUpperThreshold;//字元數量上限閥值

	AI_RESULT_ID           aiResultID;
	RECT                   aiResultRect;
	float                  aiConfidence;
	std::wstring           aiResultText;
	std::vector<TALG_PARAM_AI_CHAR> aiOcrCharList;

	tagALG_PARAM_AI_MODEL()
	{
		aiPatternAngle = 0.0f;
		aiConfidenceThreshold = 50.0f;
		aiCharMatchNumThreshold = 65535;
		aiCharNumUpperThreshold = 65535;

		aiResultID = AI_RESULT_ID_NONE;
		aiConfidence = 0.0f;
		aiModelID = ALG_AI_MODEL_NONE;
		aiResultText = L"";
		::memset(&aiResultRect, 0x00, sizeof(aiResultRect));		
	}
} TALG_PARAM_AI_MODEL, *PALG_PARAM_AI_MODEL;
//-------------------------------------------------------------------------------------//
typedef struct tagRESULT_CIRCLE_ANGLE
{
	float caAngle;
	float caRadiusPassResult;	
	tagRESULT_CIRCLE_ANGLE()
	{
		caAngle = 0.0f;
		caRadiusPassResult = 0.0f;
	}
} TRESULT_CIRCLE_ANGLE, *PRESULT_CIRCLE_ANGLE;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_SOLDER_WETTING//焊接檢測
{		
	bool                   swCircleAngleEnabled;//焊接環繞角啟用
	float                  swCircleAngleUSL;//焊接環繞角上限
	float                  swCircleAngleLSL;//焊接環繞角下限
	float                  swCircleAngleLinePassRatio;//焊接環繞角半徑通過比例
	float                  swCircleAngleLineStartRatio;//焊接環繞角半徑始比例
	float                  swReadingCircleAngle;//焊接環繞角讀值

	float                  swCircleAngleCpX;//焊接環繞角中心X-pxl
	float                  swCircleAngleCpY;//焊接環繞角中心Y-pxl
	float                  swCircleAngleRadius;//焊接環繞角半徑-pxl
	float                  swCircleAngleRadiusStart;//焊接環繞角半徑起點-pxl
	std::vector<TRESULT_CIRCLE_ANGLE> swResultCircleAngleRadius;
	tagALG_PARAM_SOLDER_WETTING()
	{
		swCircleAngleEnabled = true;
		swCircleAngleUSL = 360;
		swCircleAngleLSL = 330;
		swCircleAngleLinePassRatio = 75;
		swCircleAngleLineStartRatio = 0;
		swReadingCircleAngle = 0.0f;

		swCircleAngleCpX = 0.0;
		swCircleAngleCpY = 0.0;
		swCircleAngleRadius = 0.0;
		swCircleAngleRadiusStart = 0.0;
		swResultCircleAngleRadius.clear();
	}
} TALG_PARAM_SOLDER_WETTING, *PALG_PARAM_SOLDER_WETTING;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_MEASURE_BLACK_GLUE//量測黑膠-軍達
{	
#ifdef ALG_MEASURE_BLACK_GLUE_USE
	unsigned int               bgFrameIndex1;//畫面編號-Glue
	unsigned int               bgFrameUniqueID1;//畫面唯一碼-Glue
	unsigned int               bgFrameIndex2;//畫面編號-Board
	unsigned int               bgFrameUniqueID2;//畫面唯一碼-Board
	unsigned int               bgFrameIndex3;//畫面編號-Thermal
	unsigned int               bgFrameUniqueID3;//畫面唯一碼-Thermal
	unsigned int               bgFrameIndex4;//畫面編號-Coating
	unsigned int               bgFrameUniqueID4;//畫面唯一碼-Coating
	JET::alg::SMeasurementBlackGlue_Result bgResult;
	JET::alg::SMeasurementBlackGlue_Parameter bgParam;	
#endif//ALG_MEASURE_BLACK_GLUE_USE
	tagALG_PARAM_MEASURE_BLACK_GLUE()
	{
	#ifdef ALG_MEASURE_BLACK_GLUE_USE
		bgFrameIndex1 = 0;
		bgFrameUniqueID1 = FRAME_UNIQUE_ID_DEFAULT;
		bgFrameIndex2 = 0;
		bgFrameUniqueID2 = FRAME_UNIQUE_ID_DEFAULT;
		bgFrameIndex3 = 0;
		bgFrameUniqueID3 = FRAME_UNIQUE_ID_DEFAULT;
		bgFrameIndex4 = 0;
		bgFrameUniqueID4 = FRAME_UNIQUE_ID_DEFAULT;
		bgResult=JET::alg::SMeasurementBlackGlue_Result();
		bgParam=JET::alg::SMeasurementBlackGlue_Parameter();
	#endif//ALG_MEASURE_BLACK_GLUE_USE
	}
} TALG_PARAM_MEASURE_BLACK_GLUE, *PALG_PARAM_MEASURE_BLACK_GLUE;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_MEASURE_FLUX_AREA//量測Flux面積-軍達
{	
#ifdef ALG_MEASURE_FLUX_AREA_USE
	unsigned int               faFrameIndex1;//畫面編號-Glue
	unsigned int               faFrameUniqueID1;//畫面唯一碼-Glue	
	JET::alg::SMeasurementFluxArea_Result faResult;
	JET::alg::SMeasurementFluxArea_Parameter faParam;	
#endif//ALG_MEASURE_FLUX_AREA_USE
	tagALG_PARAM_MEASURE_FLUX_AREA()
	{
	#ifdef ALG_MEASURE_FLUX_AREA_USE
		faFrameIndex1 = 0;
		faFrameUniqueID1 = FRAME_UNIQUE_ID_DEFAULT;		
		faResult=JET::alg::SMeasurementFluxArea_Result();
		faParam=JET::alg::SMeasurementFluxArea_Parameter();
	#endif//ALG_MEASURE_FLUX_AREA_USE
	}
} TALG_PARAM_MEASURE_FLUX_AREA, *PALG_PARAM_MEASURE_FLUX_AREA;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_MEASURE_CPU_PIN//量測CPU接腳-軍達
{
#ifdef ALG_MEASURE_CPU_PIN_USE
	unsigned int               cpFrameIndex1;//畫面編號-CpuPin
	unsigned int               cpFrameUniqueID1;//畫面唯一碼-CpuPin	
	stOutputData               cpResult;
	InputData_8000             cpParam;		
#endif//ALG_MEASURE_CPU_PIN_USE
	tagALG_PARAM_MEASURE_CPU_PIN()
	{
	#ifdef ALG_MEASURE_CPU_PIN_USE
		cpFrameIndex1 = 0;
		cpFrameUniqueID1 = FRAME_UNIQUE_ID_DEFAULT;		
		cpResult= stOutputData();
		cpParam = InputData_8000();
	#endif//ALG_MEASURE_CPU_PIN_USE
	}
} TALG_PARAM_MEASURE_CPU_PIN, *PALG_PARAM_MEASURE_CPU_PIN;
//-------------------------------------------------------------------------------------//
typedef struct tagALG_PARAM_MEASURE_SIP//SIP
{
	tagALG_PARAM_ANGLE_MEASURE msPart_am;	//暫存 目標SIP
	tagALG_PARAM_ANGLE_MEASURE msRef_am;	//暫存 PCB參考線
	ALG_DIRECTION              msDirection;	//
	size_t                     msInspecEdgeCount;	// 檢測邊數量 
	size_t                     msRefEdgeCount;	// 參考邊數量
	double                     msAngleSpec;	//角度規格
	double                     msAngleTolUSL;	//公差上限
	double                     msAngleTolLSL;	//公差下限
	double                     msAngleReading;	//角度讀值
	double                     msPtASpec;	//A點距離規格
	double                     msPtATolUSL;	//A點距離上限
	double                     msPtATolLSL;	//A點距離下限
	double                     msPtAReading;//A點距離讀值
	double                     msPtBSpec;	//B點距離規格
	double                     msPtBTolUSL;	//B點距離上限
	double                     msPtBTolLSL;	//B點距離下限
	double                     msPtBReading;//B點距離讀值

	tagALG_PARAM_MEASURE_SIP()
	{
		msDirection = ALG_HORIZONTAL;
		msAngleSpec = 0;
		msAngleTolUSL = 10;
		msAngleTolLSL = -10;
		msAngleReading = 0.0;
		msPtASpec = 200;
		msPtATolUSL = 200;
		msPtATolLSL = 200;
		msPtAReading = 0;
		msPtBSpec = 200;
		msPtBTolUSL = 200;
		msPtBTolLSL = 200;
		msPtBReading = 0;
		msInspecEdgeCount = 1;
		msRefEdgeCount = 1;
	}
	void ClearReading() {
		msAngleReading = 0;
		msPtAReading = 0;
		msPtBReading = 0;
	}
} TALG_PARAM_MEASURE_SIP, *PALG_PARAM_MEASURE_SIP;
//-------------------------------------------------------------------------------------//
enum LINE_FINDER {
	LINE_FINDER_NORMAL,
	LINE_FINDER_HOUGH
};

typedef struct tagALG_PARAM_MEASURE_CONNECTOR // connector
{
	double mcPtXReading;   // X點距離讀值
	double mcPtYReading;   // Y點距離讀值
	int mc_nBlobHeightMin;	//BLOB預選位置-Height Min
	int mc_nBlobWidthMin;	//BLOB預選位置-widht Min
	int mc_nBlobHeightMax;	//BLOB預選位置-Height Max
	int	mc_nBlobWidthMax;	//BLOB預選位置-width Max
	int mc_nPinPosTableRow; //表格Row總數
	int mc_nPinPosTableCol;	//表格Col總數
	int mc_nPinWidth;		//針腳大小-Width
	int mc_nPinHeight;		//針腳大小-Height
	bool mc_bUseEdge;
	LINE_FINDER mc_EdgeFinder;
	bool mc_bChild;

	std::vector<std::vector<TPOINT2D>> mc_PinPosTable;		//標準
	std::vector<std::vector<TPOINT2D>> mc_PinPosTableDraw;	//繪圖用
	std::vector<std::vector<TPOINT2D>> mc_PinPosTableRes;	//讀值
	double mc_PtDUSL;		//誤差上限(mm)

	tagALG_PARAM_MEASURE_CONNECTOR()
	{
		mcPtXReading = 0.0;
		mcPtYReading = 0.0;
		mc_bUseEdge = false;
		mc_EdgeFinder = LINE_FINDER_HOUGH;
		mc_bChild = false;
		mc_nBlobWidthMin = 500;
		mc_nBlobHeightMin = 500;
		mc_nBlobWidthMax = 2000;
		mc_nBlobHeightMax = 2000;
		mc_nPinPosTableRow = 0;
		mc_nPinPosTableCol = 0;
		mc_nPinWidth = 150;
		mc_nPinHeight = 150;
		mc_PtDUSL = 0.3;
	}
	void InitPinTable() {
		mc_PinPosTable.resize(mc_nPinPosTableRow);
		for (int r = 0; r < mc_nPinPosTableRow; r++)
		{
			mc_PinPosTable[r].resize(mc_nPinPosTableCol);
			for (int c = 0; c < mc_nPinPosTableCol; c++)
			{
				mc_PinPosTable[r][c].x = 0.0;
				mc_PinPosTable[r][c].y = 0.0;
			}
		}
	}
	void InitResultTable()
	{
		mc_PinPosTableRes.clear();
		mc_PinPosTableRes.resize(mc_PinPosTable.size());

		for (size_t i = 0; i < mc_PinPosTable.size(); ++i)
		{
			mc_PinPosTableRes[i].resize(mc_PinPosTable[i].size());

			for (size_t j = 0; j < mc_PinPosTable[i].size(); ++j)
			{
				// 建議用 -1 表示尚未量測，避免 affine 把未量測點當成 (0,0)
				mc_PinPosTableRes[i][j].x = -1.0;
				mc_PinPosTableRes[i][j].y = -1.0;
			}
		}
	}

} TALG_PARAM_MEASURE_CONNECTOR, *PALG_PARAM_MEASURE_CONNECTOR;
#endif//_ALG_PARAM_DEF_H_