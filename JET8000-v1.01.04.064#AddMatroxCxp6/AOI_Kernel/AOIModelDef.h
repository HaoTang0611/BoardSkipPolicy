#ifndef _AOIMODEL_DEF_H_
#define _AOIMODEL_DEF_H_
//-------------------------------------------------------------------------------------//
#define PART_ALIGN_MODE_BY_MODEL_MATCH_2D         1//模板匹配2D
#define PART_ALIGN_MODE_BY_MODEL_MATCH_3D         2//模板匹配3D
#define PART_ALIGN_MODE_BY_OBJECT_MEASURE         3//物件量測3D
//-------------------------------------------------------------------------------------//
#define BODY_MISSING_MODE_BY_HEIGHT         1
#define BODY_MISSING_MODE_BY_VOLUME         2
//-------------------------------------------------------------------------------------//
#define IPC_A_610E_LEVEL_1                  1
#define IPC_A_610E_LEVEL_2                  2
#define IPC_A_610E_LEVEL_3                  3
//-------------------------------------------------------------------------------------//
enum CHIP_SIZE_MODE
{
	CHIP_SIZE_NONE=0,	
	CHIP_SIZE_250_120,// 2512//6400x3200
	CHIP_SIZE_200_100,// 2010//5000x2500
	CHIP_SIZE_180_120,// 1812//4500x3200	
	CHIP_SIZE_120_100,// 1210//3200x2500
	CHIP_SIZE_120_060,// 1206//3200x1600
	CHIP_SIZE_080_050,// 0805//2000x1250
	CHIP_SIZE_060_030,// 0603//1600x800
	CHIP_SIZE_040_020,// 0402//1000x500
	CHIP_SIZE_020_010,// 0201//600x300
	CHIP_SIZE_010_005,//01005//400x200
	CHIP_SIZE_030_015_METRIC,//01005//300x150	
	CHIP_SIZE_008_004,//008004//250x125
	CHIP_SIZE_OTHERS
};
//-------------------------------------------------------------------------------------//
enum MODEL_TYPE
{
	MODEL_TYPE_NULL                    =   0,

	MODEL_TYPE_CHIP                    = 100,//被動元件-總類
	MODEL_TYPE_CHIP_C                  = 101,//被動元件-電容
	MODEL_TYPE_CHIP_R                  = 102,//被動元件-電阻
	MODEL_TYPE_CHIP_L                  = 103,//被動元件-電感	
	MODEL_TYPE_CHIP_LED                = 105,//LED雙腳
	MODEL_TYPE_MELF                    = 106,//被動元件

	MODEL_TYPE_ELECTRODE               = 200,//電極元件
	MODEL_TYPE_TANTALUM_CONDENSER      = 201,//鉭質電容
	MODEL_TYPE_CAPACITY_ARRAY          = 202,//排容
	MODEL_TYPE_RESISTOR_ARRAY          = 203,//排阻
	MODEL_TYPE_TRANSISTOR              = 204,//三腳晶體
	MODEL_TYPE_ELECTROLYTIC_CAPACITOR  = 205,//電解電容
	MODEL_TYPE_LED_ARRAY               = 206,//LED多腳	

	MODEL_TYPE_NO_LEAD_COMPONENT       = 300,//無腳元件
	MODEL_TYPE_NO_LEAD_DFN             = 301,//無腳元件-DFN
	MODEL_TYPE_NO_LEAD_QFN             = 302,//無腳元件-QFN
	MODEL_TYPE_NO_LEAD_OSC             = 303,//無腳元件-振盪器
	
	MODEL_TYPE_LEAD_COMPONENT          = 400,//Lead元件
	MODEL_TYPE_LEAD_SOP                = 401,//Lead元件-SOP
	MODEL_TYPE_LEAD_QFP                = 402,//Lead元件-QFP	
	MODEL_TYPE_LEAD_TRANSISTOR         = 403,//Lead-三腳晶體

	MODEL_TYPE_JLEAD_COMPONENT         = 450,//J-Lead元件
	MODEL_TYPE_JLEAD_SOJ               = 451,//J-Lead元件-SOJ
	MODEL_TYPE_JLEAD_PLCC              = 452,//J-Lead元件-QFJ	

	MODEL_TYPE_COMPOSITE_COMPONENT     = 500,//複合元件
	MODEL_TYPE_POWER_TRANSISTOR        = 501,//複合元件-功率電晶體	
	MODEL_TYPE_CONNECTOR               = 511,//複合元件-連接器	

	MODEL_TYPE_BGA                     = 600,//BGA元件
	MODEL_TYPE_FD                      = 601,//Fd元件
	MODEL_TYPE_BARCODE                 = 602,//Barcode元件

	MODEL_TYPE_PAD_COMPONENT           = 700,//焊盤元件	
	MODEL_TYPE_GOLD_FINGER             = 701,//焊盤元件-金手指	

	MODEL_TYPE_DIP_LEAD                = 800,//標準DIP引腳	

	MODEL_TYPE_OTHERS                  = 900, //其餘的

	MODEL_TYPE_ALL                     = 999 //全部的
};
//-------------------------------------------------------------------------------------//
enum LAND_TYPE//特徵框樣式
{
	LAND_TYPE_NULL      = 0,//未定義
	LAND_TYPE_PAD       = 1,//特徵框-焊盤
	LAND_TYPE_ELECTRODE = 2,//特徵框-電極
	LAND_TYPE_IC_LEAD   = 3,//特徵框-引腳-IC
	LAND_TYPE_CON_LEAD  = 4,//特徵框-引腳-連接器
	LAND_TYPE_DIP_LEAD  = 5,//特徵框-引腳-插件DIP
	LAND_TYPE_RETURN
};
//-------------------------------------------------------------------------------------//
enum WND_SYNC_MOVE_MODE//檢測框同動模式
{	
	WND_SYNC_MOVE_NONE       = 0,//未定義
	WND_SYNC_MOVE_ROTATE     = 1,//旋轉
	WND_SYNC_MOVE_MIRROR     = 2,//鏡射	
	WND_SYNC_MOVE_SYMMETRY   = 3,//對稱
	WND_SYNC_MOVE_RETURN
};
//-------------------------------------------------------------------------------------//
enum WND_RGN_LINK_MODE//檢測框尺寸連動模式
{
	WND_RGN_LINK_NONE              = 0,
	WND_RGN_LINK_PAD               = 1,	
	WND_RGN_LINK_BODY              = 2,
	WND_RGN_LINK_LEAD              = 3,	
	WND_RGN_LINK_LEAD_TIP          = 4,	
	WND_RGN_LINK_LEAD_SHOULDER     = 5,
	WND_RGN_LINK_LEAD_TIP_SHOULDER = 6,
	WND_RGN_LINK_PAD_TIP           = 7,
	WND_RGN_LINK_PAD_RGN           = 8,
	WND_RGN_LINK_PAD_BODY_RGN      = 9,//焊盤+本體區域
	WND_RGN_LINK_PAD_RGN_INNER     =10,//焊盤內圍
	WND_RGN_LINK_RETURN
};
//-------------------------------------------------------------------------------------//
enum WND_FOLLOW_MODE//檢測框跟隨移動模式
{
	WND_FOLLOW_NONE              = 0,//不跟著移動
	WND_FOLLOW_PAD               = 1,//跟焊盤移動(任意焊盤)
	WND_FOLLOW_PART              = 2,//跟零件移動(本體或引腳)

	WND_FOLLOW_PAD_BODY          = 11,//跟焊盤移動-本體
	WND_FOLLOW_PAD_LEAD          = 12,//跟焊盤移動-引腳

	WND_FOLLOW_PART_BODY         = 21,//跟零件移動-本體
	WND_FOLLOW_PART_LEAD         = 22,//跟零件移動-引腳

	WND_FOLLOW_RETURN
};
//-------------------------------------------------------------------------------------//
enum MODEL_LAND_DIRECTION//模組焊盤方向
{
	MODEL_LAND_CLOCKWISE = 1,
	MODEL_LAND_COUNTER_CLOCKWISE = 2,
	MODEL_LAND_RETURN
};
//-------------------------------------------------------------------------------------//
enum MODEL_LAND_ADJUST_MODE////模組焊盤調整模式
{
	MODEL_LAND_ADJUST_AVERAGE    = 1,
	MODEL_LAND_ADJUST_INDIVIDUAL = 2,
	MODEL_LAND_ADJUST_RETURN
};
//-------------------------------------------------------------------------------------//
enum ALG_TYPE
{
	ALG_EMPTY                  = 0,//空的
	ALG_BRIGHT_RATIO           = 1,//亮度比例	
	ALG_OUTER_SHORT            = 2,//外接短路
	ALG_BLOB_COUNT             = 3,//區塊數量
	ALG_BODY_TILT              = 4,//本體傾斜
	ALG_BARCODE_RECOGNIZE      = 5,//條碼辨識
	ALG_OBJECT_MEASURE         = 6,//物件量測
	ALG_COLOR_CODE             = 7,//色碼檢測
	ALG_SHAPE_VERIFY           = 9,//外形驗證
	ALG_ANGLE_MEASURE          = 10,//角度量測
	ALG_WIDTH_RATIO	           = 11,//寬度比例	
	ALG_SOLDER_WETTING         = 12,//焊接檢測

	ALG_MODEL_MATCH            = 100,//模板匹配
	ALG_IMAGE_MATCH            = 101,//影像匹配
	ALG_CHAR_VERIFY            = 102,//文字驗證
	ALG_FD_MATCH               = 103,//定位點匹配
	ALG_EDGE_SEARCH            = 104,//邊緣搜尋
	ALG_PIXEL_COMPARE          = 105,//像素比較

	ALG_HEIGHT				   = 200,//高度比例
	ALG_WIRE_WIDTH             = 201,//金線寬度

	ALG_MEASURE_BLACK_GLUE     = 301,//量測黑膠-軍達
	ALG_MEASURE_FLUX_AREA      = 302,//量測Flux面積-軍達
	ALG_MEASURE_CPU_PIN        = 303,//量測CPU接腳-軍達

	ALG_MEASURE_SIP_DISTANCE   = 401,//量測SIP 距離-Alan
	ALG_MEASURE_CONNECTOR      = 402,//量測Connector-本體-Alan
	ALG_MEASURE_CONNECTOR_PIN  = 403,//量測Connector-PIN-Alan

	ALG_TYPE_RETURN
};
//-------------------------------------------------------------------------------------//
enum DRAW_MODEL_MODE//顯示模組模式
{
	DRAW_MODEL_EDIT    = 1,
	DRAW_MODEL_RESULT  = 2,
	DRAW_MODEL_TEMP    = 3,
	DRAW_MODEL_RETURN
};
//-------------------------------------------------------------------------------------//
enum MANIPULATE_MODEL_MODE
{
	MANIPULATE_MODEL_ADD          = 1,//增加框
	MANIPULATE_MODEL_EDIT         = 2,//編輯
	MANIPULATE_MODEL_SELECT       = 3,//選取
	MANIPULATE_MODEL_GATHER_COLOR = 4,//抽色
	MANIPULATE_MODEL_CALIBRATION  = 5,//校正
	MANIPULATE_MODEL_AUTO_ADD     = 6,//自動
	MANIPULATE_MODEL_RETURN
};
//-------------------------------------------------------------------------------------//
enum MODEL_ATTACHED_OBJ//模組掛載物件
{
	MODEL_ATTACHED_NONE=0,
	MODEL_ATTACHED_FD,
	MODEL_ATTACHED_MARK,
	MODEL_ATTACHED_BARCODE,
	MODEL_ATTACHED_COMPONENT,
	MODEL_ATTACHED_RETURN
};
//-------------------------------------------------------------------------------------//
typedef struct _MODEL_DRAW_PARAM//模組繪圖參數
{
	POINT                      ViewCP;
	RECT                       WndRect;	
	bool                       ShowWndBox;
	bool                       ShowLandBox;
	bool                       ShowEditLine;	
	double                     ViewOffsetX;
	double                     ViewOffsetY;
	double                     ResolutionX;
	double                     ResolutionY;
	double                     Scale;
	_MODEL_DRAW_PARAM()
	{
		ViewCP.x = ViewCP.y = 0;
		WndRect.left = WndRect.right = WndRect.top = WndRect.bottom = 0;
		ShowWndBox = true;
		ShowLandBox = true;
		ShowEditLine = true;		
		ViewOffsetX = ViewOffsetY = 0;
		ResolutionX = 10.0;
		ResolutionY = 10.0;
		Scale = 1.0;
	}
} TMODEL_DRAW_PARAM, *PMODEL_DRAW_PARAM;
//-------------------------------------------------------------------------------------//
typedef struct _ActiveObj
{
protected:
	bool        Focused;	
	bool        Selected;
	bool        Editabled;
public:
	double      ComponentAngle;
	bool        IsExceptionAngle;	
	TRECT4D     Rect;
	TPOINT2D    CornerPts[4];	
	
	bool        PassObj;	

	CObject    *BoxPtr;
	CObject    *WndPtr;		
	CObject    *LandPtr;
	CObject    *ModelPtr;	
	CObject    *WndRoiPtr;
	CObject    *WndMaskPtr;
	_ActiveObj()
	{
		ComponentAngle = 0;
		IsExceptionAngle = false;		
		Rect = TRECT4D();
		CornerPts[0] = TPOINT2D();
		CornerPts[1] = TPOINT2D();
		CornerPts[2] = TPOINT2D();
		CornerPts[3] = TPOINT2D();
		Focused = false;
		Selected = false;
		PassObj = false;
		Editabled = false;

		BoxPtr = NULL;
		WndPtr = NULL;
		LandPtr = NULL;
		ModelPtr = NULL;
		WndRoiPtr = NULL;
		WndMaskPtr = NULL;
	}

	bool  GetFocused() const { return Focused; }
	void  SetFocused(bool val) { Focused = val; }

	bool  GetSelected() const { return Selected; }
	void  SetSelected(bool val) { Selected = val; }	
	
	bool  GetEditabled() const { return Editabled; }
	void  SetEditabled(bool val) { Editabled = val; }

	bool CheckModelBodyBox() const
	{
		if ( NULL == BoxPtr ) { return false; }
		if ( NULL == ModelPtr ) { return false; }

		if ( NULL != WndPtr ) { return false; }
		if ( NULL != LandPtr ) { return false; }
		if ( NULL != WndRoiPtr ) { return false; }
		if ( NULL != WndMaskPtr ) { return false; }
		if ( NULL != WndPtr ) { return false; }
		if ( NULL != WndPtr ) { return false; }
		return true;
	}
} TActiveObj, *PActiveObj;
//-------------------------------------------------------------------------------------//
enum MDW_VERSION//模組預設檢測框版本(Model Default Wnd Version)
{
	MDW_VERSION_1     = 1,//原有參數
	MDW_VERSION_2     = 2 //華南版本
};
//-------------------------------------------------------------------------------------//
typedef struct tagMODEL_DEFAULT_WND_PARAM//模組預設檢測框參數
{
	void          *pModel;
	MODEL_TYPE     eModelType;
	MDW_VERSION    eVersion;//版本-1:原有參數, 2:華南版本
	CString        sGroupName;
	CHIP_SIZE_MODE eChipSizeMode;
	bool           bUseDefaultModel;//是否載入預設模組

	bool           bPadAlign;
	bool           bPartAlign;
	bool           bPadAdjust;
	bool           bLeadAdjust;
	bool           bBodyMissing;
	bool           bBodyTilt;
	bool           bBodyMount;
	bool           bPolarity;
	bool           bTextWrong;
	bool           bBodyDamaged;
	bool           bLeadLifted;
	bool           bLeadBended;
	bool           bSolderOpen;
	bool           bSolderPoor;
	bool           bSolderPadExposed;
	bool           bBridge;
	bool           bPadScratch;	
	bool           bForeignBody;
	bool           bDummy;

	bool           bUseLogic;
	bool           bDefaultModelAllDefect;
	
	bool           bUse3DLight;//使用3D光源
	unsigned int   nFrameIndex_3D;//3D光源
	unsigned int   nFrameUniqueID_3D;//3D光源
	unsigned int   nFrameIndex_Low;//Low光源
	unsigned int   nFrameUniqueID_Low;//Low光源
	unsigned int   nFrameIndex_High;//High光源
	unsigned int   nFrameUniqueID_High;//High光源
	unsigned int   nFrameIndex_Text;//文字光源
	unsigned int   nFrameUniqueID_Text;//文字光源
	unsigned int   nFrameIndex_Align;//對位光源
	unsigned int   nFrameUniqueID_Align;//對位光源
	unsigned int   nFrameIndex_Solder;//焊錫光源
	unsigned int   nFrameUniqueID_Solder;	//焊錫光源	

	unsigned int   nDefaultFrameIndex;//預設光源
	unsigned int   nDefaultFrameUniqueID;//預設光源	
	std::vector<unsigned int>   FrameIndexMapList;//專案燈源映射列表	

	double         dPadAlignExtendRange;
	double         dPadAdjustExtendRange;	

	WND_RGN_LINK_MODE ePartAlignLinkMode;
	double         dPartAlignSkewLimit;	
	double         dPartAlignOffsetLimit;	
	double         dPartAlignExtendRange;	
	double         dPartAlignHeightTolerance;

	double         dLeadAdjustExtendRange;	

	int            nPartAlignMode;	

	int            nBodyTiltNum;	
	double         dBodyTiltHeightTolerance;

	int            nBodyMountNum;
	int            nBodyMissingMode;
	double         dBodyMissingHeightTolerance;

	double         dPolarityExtendRange;		
	
	bool           bSolderOpenUseSideWnd;
	double         dSolderOpenBrRatioUSL;	
	
	bool           bSolderPoorUseSideWnd;
	double         dSolderPoorBrRatioUSL;	

	bool           bBridgeUse2D;
	bool           bBridgeUse3D;	
	bool           bBridgeTwoSide;//雙側檢測框
	double         dBridgeExtendRange;
	int            nBridgeFixThresholdL2D;
	int            nBridgeFixThresholdL3D;

	tagMODEL_DEFAULT_WND_PARAM()
	{
		pModel = NULL;
		eModelType = MODEL_TYPE_NULL;
		eVersion = MDW_VERSION_1;
		sGroupName = _T("GroupName");
		eChipSizeMode = CHIP_SIZE_OTHERS;
		bUseDefaultModel = false;

		bPadAlign = true;
		bPartAlign = true;
		bPadAdjust = false;
		bLeadAdjust = false;
		bBodyMissing = true;
		bBodyTilt = true;
		bBodyMount = true;
		bPolarity = true;
		bTextWrong = true;
		bBodyDamaged = false;
		bLeadLifted = true;
		bLeadBended = true;
		bSolderOpen = true;
		bSolderPoor = true;
		bSolderPadExposed = false;
		bBridge = true;
		bPadScratch = false;
		bForeignBody = false;
		bDummy = false;
		bUseLogic = false;
		bDefaultModelAllDefect = false;

		bUse3DLight = true;
		nFrameIndex_3D = 0;
		nFrameIndex_Low = 0;
		nFrameIndex_High = 0;
		nFrameIndex_Text = 0;
		nFrameIndex_Align = 0;
		nFrameIndex_Solder = 0;		
		nFrameUniqueID_3D = FRAME_UNIQUE_ID_DLP;
		nFrameUniqueID_Low = FRAME_UNIQUE_ID_LOW;
		nFrameUniqueID_High = FRAME_UNIQUE_ID_TOP;
		nFrameUniqueID_Text = FRAME_UNIQUE_ID_LOW;
		nFrameUniqueID_Align = FRAME_UNIQUE_ID_TOP;
		nFrameUniqueID_Solder = FRAME_UNIQUE_ID_RGB;		
		nDefaultFrameUniqueID = FRAME_UNIQUE_ID_RGB;//預設光源	

		dPadAlignExtendRange = 400;		

		dPadAdjustExtendRange = 200;		

		ePartAlignLinkMode = WND_RGN_LINK_BODY;		
		dPartAlignSkewLimit = 5.0;
		dPartAlignOffsetLimit = 250;		
		dPartAlignExtendRange = 200;		
		dPartAlignHeightTolerance = 80;

		dLeadAdjustExtendRange = 200;		

		nPartAlignMode = PART_ALIGN_MODE_BY_MODEL_MATCH_2D;		

		nBodyTiltNum = 2;
		dBodyTiltHeightTolerance = 80;

		nBodyMountNum = 1;
		nBodyMissingMode = BODY_MISSING_MODE_BY_HEIGHT;
		dBodyMissingHeightTolerance = 80;

		dPolarityExtendRange = 200;		
		
		bSolderOpenUseSideWnd = false;
		dSolderOpenBrRatioUSL = 35;		
		
		bSolderPoorUseSideWnd = false;
		dSolderPoorBrRatioUSL = 20;		

		bBridgeUse2D = true;
		bBridgeUse3D = true;	
		bBridgeTwoSide = false;
		dBridgeExtendRange = 400;
		nBridgeFixThresholdL2D = 60;
		nBridgeFixThresholdL3D = 150;
	}
	void SetAll(bool bEnable)
	{
		bPadAlign = bEnable;
		bPartAlign = bEnable;
		bPadAdjust = bEnable;
		bLeadAdjust = bEnable;
		bBodyMissing = bEnable;
		bBodyTilt = bEnable;
		bBodyMount = bEnable;
		bPolarity = bEnable;
		bTextWrong = bEnable;
		bBodyDamaged = bEnable;
		bLeadLifted = bEnable;
		bLeadBended = bEnable;
		bSolderOpen = bEnable;
		bSolderPoor = bEnable;
		bSolderPadExposed = bEnable;
		bBridge = bEnable;
		bPadScratch = bEnable;
		bForeignBody = bEnable;

		ePartAlignLinkMode = WND_RGN_LINK_BODY;
		nBodyTiltNum = 2;
		nBodyMountNum = 1;
		bBridgeTwoSide = bEnable;
	}

	void SetEnable(WND_DEFECT_ID DefectID, bool Enable)
	{
		switch ( DefectID )
		{
		case WND_DEFECT_PAD_ALIGN:	bPadAlign=Enable; break;
		case WND_DEFECT_PART_ALIGN:	bPartAlign=Enable; break;
		case WND_DEFECT_PAD_ADJUST:	bPadAdjust=Enable; break;
		case WND_DEFECT_LEAD_ADJUST:bLeadAdjust=Enable; break;

		case WND_DEFECT_BODY_MISSING:bBodyMissing=Enable; break;
		//case WND_DEFECT_BODY_OFFSET:
		case WND_DEFECT_BODY_TILT:	bBodyTilt=Enable;	break;
		case WND_DEFECT_BODY_POLARITY:bPolarity=Enable; break;
		//case WND_DEFECT_BODY_TURNOVER:
		case WND_DEFECT_BODY_MOUNT:	bBodyMount=Enable; break;
		//case WND_DEFECT_BODY_WRONG_CODE:
		case WND_DEFECT_BODY_WRONG_TEXT:bTextWrong=Enable; break;
		//case WND_DEFECT_BODY_TOMBSTONE:	break;
		//case WND_DEFECT_BODY_BILLBOARD:	break;
		case WND_DEFECT_BODY_DAMAGED:	bBodyDamaged=Enable; break;

		case WND_DEFECT_SOLDER_POOR:	bSolderPoor=Enable; break;
		case WND_DEFECT_SOLDER_OPEN:	bSolderOpen=Enable; break;
		case WND_DEFECT_SOLDER_PAD_EXPOSED:bSolderPadExposed=Enable; break;
		case WND_DEFECT_SOLDER_BRIDGE:	bBridge=Enable; break;
		//case WND_DEFECT_SOLDER_BEAD:
		//caes WND_DEFECT_SOLDER_EXCESS:	break;

		case WND_DEFECT_LEAD_LIFTED:	bLeadLifted=Enable; break;
		case WND_DEFECT_LEAD_BENDED:	bLeadBended=Enable; break;
		//case WND_DEFECT_LEAD_PROTRUDED:

		case WND_DEFECT_PAD_SCRATCH:	bPadScratch=Enable; break;
		case WND_DEFECT_FOREIGN_BODY:   bForeignBody=Enable; break;

		//case WND_DEFECT_CLASS_CHECK:
		//case WND_DEFECT_BASE_VALUE:

		//case WND_DEFECT_USER_DEFINE_01:
		//case WND_DEFECT_USER_DEFINE_02:
		//case WND_DEFECT_USER_DEFINE_03:
		//case WND_DEFECT_USER_DEFINE_04:
		//case WND_DEFECT_USER_DEFINE_05:
		//case WND_DEFECT_USER_DEFINE_06:
		//case WND_DEFECT_USER_DEFINE_07:
		//case WND_DEFECT_USER_DEFINE_08:
		//case WND_DEFECT_USER_DEFINE_09:
		//case WND_DEFECT_USER_DEFINE_10:	
			break;
		}		
	}

	bool GetEnable(WND_DEFECT_ID DefectID) const
	{
		bool Enable=false;
		switch ( DefectID )
		{
		case WND_DEFECT_PAD_ALIGN:	Enable=bPadAlign; break;
		case WND_DEFECT_PART_ALIGN:	Enable=bPartAlign; break;
		case WND_DEFECT_PAD_ADJUST:	Enable=bPadAdjust; break;
		case WND_DEFECT_LEAD_ADJUST:Enable=bLeadAdjust; break;

		case WND_DEFECT_BODY_MISSING:Enable=bBodyMissing; break;
		//case WND_DEFECT_BODY_OFFSET:
		case WND_DEFECT_BODY_TILT:	Enable=bBodyTilt;	break;
		case WND_DEFECT_BODY_POLARITY:Enable=bPolarity; break;
		//case WND_DEFECT_BODY_TURNOVER:
		case WND_DEFECT_BODY_MOUNT:	Enable=bBodyMount; break;
		//case WND_DEFECT_BODY_WRONG_CODE:
		case WND_DEFECT_BODY_WRONG_TEXT:Enable=bTextWrong; break;
		//case WND_DEFECT_BODY_TOMBSTONE:	break;
		//case WND_DEFECT_BODY_BILLBOARD:	break;
		case WND_DEFECT_BODY_DAMAGED: Enable=bBodyDamaged; break;

		case WND_DEFECT_SOLDER_POOR:	Enable=bSolderPoor; break;
		case WND_DEFECT_SOLDER_OPEN:	Enable=bSolderOpen; break;
		case WND_DEFECT_SOLDER_PAD_EXPOSED:Enable=bSolderPadExposed; break;
		case WND_DEFECT_SOLDER_BRIDGE:	Enable=bBridge; break;
		//case WND_DEFECT_SOLDER_BEAD:
		//caes WND_DEFECT_SOLDER_EXCESS:	break;

		case WND_DEFECT_LEAD_LIFTED:	Enable=bLeadLifted; break;
		case WND_DEFECT_LEAD_BENDED:	Enable=bLeadBended; break;
		//case WND_DEFECT_LEAD_PROTRUDED:		

		case WND_DEFECT_PAD_SCRATCH:	Enable=bPadScratch; break;
		case WND_DEFECT_FOREIGN_BODY:   Enable=bForeignBody; break;

		//case WND_DEFECT_CLASS_CHECK:
		//case WND_DEFECT_BASE_VALUE:

		//case WND_DEFECT_USER_DEFINE_01:
		//case WND_DEFECT_USER_DEFINE_02:
		//case WND_DEFECT_USER_DEFINE_03:
		//case WND_DEFECT_USER_DEFINE_04:
		//case WND_DEFECT_USER_DEFINE_05:
		//case WND_DEFECT_USER_DEFINE_06:
		//case WND_DEFECT_USER_DEFINE_07:
		//case WND_DEFECT_USER_DEFINE_08:
		//case WND_DEFECT_USER_DEFINE_09:
		//case WND_DEFECT_USER_DEFINE_10:	
			break;
		}
		return Enable;
	}
} TMODEL_DEFAULT_WND_PARAM, *PMODEL_DEFAULT_WND_PARAM;
//-------------------------------------------------------------------------------------//

#endif//_AOIMODEL_DEF_H_