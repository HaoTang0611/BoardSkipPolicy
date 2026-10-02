#ifndef _AOI_PART_GROUP_DEF_H_
#define _AOI_PART_GROUP_DEF_H_
//-------------------------------------------------------------------------------------//
enum PART_GROUP_MODE
{
	PART_GROUP_COLINEARITY         = 1,//群組模式-共線性	
	PART_GROUP_COLINEARITY_TO_LINE = 2,//群組模式-共線性	-對線

	PART_GROUP_DIST_PART_TO_PART   = 21,//距離量測-點對點
	PART_GROUP_DIST_PART_NEIGHBOR  = 22,//距離量測-點對點-相鄰
	PART_GROUP_DIST_PART_TO_GROUP  = 31,//距離量測-點對群
	PART_GROUP_DIST_GROUP_TO_PART  = 32,//距離量測-群對點

	PART_GROUP_DIST_GROUP_COORD_MAP= 41,//距離量測-群-座標轉換
	PART_GROUP_RETURN
};
//-------------------------------------------------------------------------------------//
enum PART_GROUP_COLINEARITY_MODE
{
	PART_GROUP_COLINEARITY_X      = 1,//水平
	PART_GROUP_COLINEARITY_Y      = 2,//垂直	
	PART_GROUP_COLINEARITY_SKEW   = 3,//角度
	PART_GROUP_COLINEARITY_HEIGHT = 4,//高度(BodyHeight)
	PART_GROUP_COLINEARITY_RETURN
};
//-------------------------------------------------------------------------------------//
enum PART_GROUP_TARGET_MODE//目標值模式
{
	PART_GROUP_TARGET_AVE    = 1,//平均值
	PART_GROUP_TARGET_MIN    = 2,//最小值	
	PART_GROUP_TARGET_RETURN
};
//-------------------------------------------------------------------------------------//
enum PART_GROUP_MAP_DIR_MODE//群組比較座標轉換方向
{
	PART_GROUP_MAP_DIR_NONE    =  0,//無定義-原點
	PART_GROUP_MAP_DIR_POS_X   =  1,//正X
	PART_GROUP_MAP_DIR_POS_Y   =  2,//正Y
	PART_GROUP_MAP_DIR_NEG_X   =  3,//負X
	PART_GROUP_MAP_DIR_NEG_Y   =  4,//負Y
	PART_GROUP_MAP_DIR_RETURN
};
//-------------------------------------------------------------------------------------//
typedef struct tagPartGroupNode
{
	CAOIPanel     *PanelPtr;
	CAOIBoard     *BoardPtr;
	CAOIComponent *ComponentPtr;
	CAOIWnd       *WndPtr;

	unsigned int   PanelIndex;
	unsigned int   BoardIndex;
	unsigned int   ComponentIndex;
	unsigned int   ModelWndIndex;	
	PART_GROUP_MAP_DIR_MODE MapDirMode;
	
	double         UserMapCadPosX;//映射座標-X
	double         UserMapCadPosY;//映射座標-Y
	double         UserMapCadDisL;//映射距離-L	
	bool           UserMapCadEnable;//自訂義映射座標啟用

	CString        ResultText;
	RESULT_ID      ResultID;
	double         ResultStdX;//結果標準-X
	double         ResultStdY;//結果標準-Y	
	double         ResultStdL;//結果標準-L
	double         ResultSkew;//結果位置-Skew
	double         ResultStdHeight;//結果標準-Height

	double         ResultGapX;//結果偏差-X
	double         ResultGapY;//結果偏差-Y
	double         ResultGapL;//結果偏差-L
	double         ResultGapSkew;//結果偏差-Skew	
	double         ResultGapHeight;//結果偏差-Height

	tagPartGroupNode()
	{
		PanelPtr = NULL;
		BoardPtr = NULL;
		ComponentPtr = NULL;
		WndPtr = NULL;

		PanelIndex = -1;
		BoardIndex = -1;
		ComponentIndex = -1;
		ModelWndIndex = -1;
		MapDirMode=PART_GROUP_MAP_DIR_NONE;
				
		UserMapCadPosX = 0.0;
		UserMapCadPosY = 0.0;
		UserMapCadDisL = 0.0;
		UserMapCadEnable = false;

		ResultStdX = 0.0;
		ResultStdY = 0.0;		
		ResultStdL = 0.0;
		ResultSkew = 0.0;
		ResultStdHeight=0.0;

		ResultGapX = 0.0;
		ResultGapY = 0.0;
		ResultGapL = 0.0;
		ResultGapSkew = 0.0;
		ResultGapHeight=0.0;

		ResultText = _T("");
		ResultID = RESULT_ID_NONE;
	}

	void InitPartGroupNodeTest()
	{
		WndPtr = NULL;
		if ( NULL != ComponentPtr )
		{	WndPtr = ComponentPtr->GetComponentModelPtr()->GetModelWndPtr(ModelWndIndex, true);	}

		ResultStdX = 0.0;
		ResultStdY = 0.0;
		ResultStdL = 0.0;
		ResultSkew = 0.0;
		ResultStdHeight=0.0;

		ResultGapX = 0.0;
		ResultGapY = 0.0;
		ResultGapL = 0.0;
		ResultGapSkew = 0.0;
		ResultGapHeight=0.0;
		ResultText = _T("");
		ResultID = RESULT_ID_NONE;
	}

	bool CheckPartGroupNodeValid() const;
	void SetComponentPtr(CAOIComponent *Ptr);	
} TPartGroupNode, *PPartGroupNode;
//-------------------------------------------------------------------------------------//
enum PART_GROUP_PARAM_ID
{
	PART_GROUP_PARAM_GROUP_ID,     //群組編號
	PART_GROUP_PARAM_GROUP_NAME,   //群組名稱
	PART_GROUP_PARAM_IN_ONE_BOARD, //群組同個單板	

	PART_GROUP_PARAM_COLINEARITY_MODE,     //共線性模式
	PART_GROUP_PARAM_COLINEARITY_TARGET_MODE, //共線性偏差標準模式
	PART_GROUP_PARAM_COLINEARITY_GAP_STD,  //共線性偏差標準
	PART_GROUP_PARAM_COLINEARITY_GAP_USL,  //共線性偏差上限
	PART_GROUP_PARAM_COLINEARITY_GAP_LSL,  //共線性偏差下限

	PART_GROUP_PARAM_DISTANCE_GAP_USL_X,  //距離偏差上限-X
	PART_GROUP_PARAM_DISTANCE_GAP_LSL_X,  //距離偏差下限-X
	PART_GROUP_PARAM_DISTANCE_GAP_USL_Y,  //距離偏差上限-Y
	PART_GROUP_PARAM_DISTANCE_GAP_LSL_Y,  //距離偏差下限-Y
	PART_GROUP_PARAM_DISTANCE_GAP_USL_L,  //距離偏差上限-L
	PART_GROUP_PARAM_DISTANCE_GAP_LSL_L,  //距離偏差下限-L
	PART_GROUP_PARAM_DISTANCE_GAP_ENB_X,  //距離偏差啟用-X
	PART_GROUP_PARAM_DISTANCE_GAP_ENB_Y,  //距離偏差啟用-Y
	PART_GROUP_PARAM_DISTANCE_GAP_ENB_L,  //距離偏差啟用-L

	PART_GROUP_PARAM_DISTANCE_GAP_STD_ENB,//距離偏差標準啟用
	PART_GROUP_PARAM_DISTANCE_GAP_STD_X,  //距離偏差標準-X
	PART_GROUP_PARAM_DISTANCE_GAP_STD_Y,  //距離偏差標準-Y
	PART_GROUP_PARAM_DISTANCE_GAP_STD_L,  //距離偏差標準-L

	PART_GROUP_PARAM_DISTANCE_GAP_ADD_X,  //距離偏差加值-X
	PART_GROUP_PARAM_DISTANCE_GAP_ADD_Y,  //距離偏差加值-Y

	PART_GROUP_PARAM_DISTANCE_GAP_ENB_ABS,//距離偏差絕對值啟用	

	PART_GROUP_PARAM_DISTANCE_GAP_SCALE_X,//距離偏差倍率-X
	PART_GROUP_PARAM_DISTANCE_GAP_SCALE_Y,//距離偏差倍率-Y
	PART_GROUP_PARAM_DISTANCE_GAP_SCALE_L,//距離偏差倍率-L

	PART_GROUP_NODE_BEGIN,                //節點參數-開始
	PART_GROUP_NODE_MAP_DIR_MODE,         //節點參數-座標轉換方向
	PART_GROUP_NODE_USER_MAP_CAD_POS_X,   //節點參數-自訂映射座標-X
	PART_GROUP_NODE_USER_MAP_CAD_POS_Y,   //節點參數-自訂映射座標-Y
	PART_GROUP_NODE_USER_MAP_CAD_DIS_L,   //節點參數-自訂映射距離-L
	PART_GROUP_NODE_USER_MAP_CAD_ENABLED, //節點參數-自訂映射座標啟用	
	PART_GROUP_NODE_END,                  //節點參數-結束

	PART_GROUP_PARAM_RETURN
};
//-------------------------------------------------------------------------------------//
#endif//_AOI_PART_GROUP_DEF_H_
//-------------------------------------------------------------------------------------//

