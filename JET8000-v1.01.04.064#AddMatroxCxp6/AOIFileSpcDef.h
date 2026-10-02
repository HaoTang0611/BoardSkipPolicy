#ifndef _AOI_FILE_SPC_DEF_H_
#define _AOI_FILE_SPC_DEF_H_
//-------------------------------------------------------------------------------------//
#define CHUNK_TYPE_PROJECT_INFO        1
#define CHUNK_TYPE_PANEL_NODE          2
#define CHUNK_TYPE_BOARD_NODE          3
#define CHUNK_TYPE_COMPONENT_NODE      4
#define CHUNK_TYPE_WINDOW_NODE         5
//-------------------------------------------------------------------------------------//
typedef struct tagSpcChunk
{
	int            nType;
	int            nSize;
	int            nCount;
	tagSpcChunk()
	{
		nType = 0;
		nSize = 0;
		nCount = 0;
	};
} TSpcChunk, *PSpcChunk;
//-------------------------------------------------------------------------------------//
typedef struct tagSpcHeader
{
	char       sSymbol[32];//標記
	int        nVersion;//板本編號
	int        nChunkTypeNumber;//Chunk樣式數量

	tagSpcHeader()
	{
		sSymbol[0] = sSymbol[1] = sSymbol[2] = sSymbol[4] = '\0';
		nVersion = 0;
		nChunkTypeNumber = 0;
	};
} TSpcHeader, *PSpcHeader;
//-------------------------------------------------------------------------------------//
//Test->AOI Test,  Check->Operator Confirm
typedef struct tagSpcProjectInfo
{
	UUID           uuidProject;
	wchar_t        sLocation[64];//廠區
	wchar_t        sBuilding[64];//棟別
	wchar_t        sFloor[64];//樓層
	wchar_t        sRoomName[64];//車間
	wchar_t        sLineName[64];//線名
	wchar_t        sStationName[64];//站名
	wchar_t        sMachineSN[64];//機台序號
	int            nLaneID;//軌道編號

	wchar_t        sModuleName[256];//機種名稱
	wchar_t        sSideName[64];//正背面名稱
	wchar_t        sWorkOrder[256];//工單編號
	wchar_t        sWorkNumber[256];//工單號碼
	double         fSpentTime;//檢測花費時間

	char           sTestDateTime[32];//檢測日期
	char           sCheckDateTime[32];//確認日期

	int            nTestResultID;//檢測結果
	int            nCheckResultID;//確認結果

	wchar_t        sTestUserName[64];//檢測人員
	wchar_t        sCheckUserName[64];//確認人員

	tagSpcProjectInfo()
	{
		::memset(&uuidProject, 0x00, sizeof(uuidProject));		
		::memset(sLocation, 0x00, sizeof(sLocation));//廠區
		::memset(sBuilding, 0x00, sizeof(sBuilding));//棟別
		::memset(sFloor, 0x00, sizeof(sFloor));//樓層
		::memset(sRoomName, 0x00, sizeof(sRoomName));//車間
		::memset(sLineName, 0x00, sizeof(sLineName));//線名
		::memset(sStationName, 0x00, sizeof(sStationName));//站名
		::memset(sMachineSN, 0x00, sizeof(sMachineSN));//機台序號
		nLaneID = LANE_ID_A;//軌道編號

		::memset(sModuleName, 0x00, sizeof(sModuleName));//機種名稱
		::memset(sSideName, 0x00, sizeof(sSideName));//正背面名稱
		::memset(sWorkOrder, 0x00, sizeof(sWorkOrder));;//工單編號
		::memset(sWorkNumber, 0x00, sizeof(sWorkNumber));//工單號碼
		fSpentTime = 0.0;//檢測花費時間

		::memset(sTestDateTime, 0x00, sizeof(sTestDateTime));//檢測日期
		::memset(sCheckDateTime, 0x00, sizeof(sCheckDateTime));//確認日期

		nTestResultID = 0;//檢測結果
		nCheckResultID = 0;//確認結果

		::memset(sTestUserName, 0x00, sizeof(sTestUserName));//檢測人員
		::memset(sCheckUserName, 0x00, sizeof(sCheckUserName));//確認人員
	};
} TSpcProjectInfo, *PtagSpcProjectInfo;
//-------------------------------------------------------------------------------------//
typedef struct tagSpcPanel
{
	UUID           uuidPanel;//整板唯一碼
	int            uPanelIndex;//整板編號
	wchar_t        sPanelBarcode[128];//整板條碼
	int            nPanelTestResultID;//整板檢測結果
	int            nPanelCheckResultID;//整板確認結果
	double         fPanelRgnMinX;//整板邊界最小X
	double         fPanelRgnMinY;//整板邊界最小Y
	double         fPanelRgnMaxX;//整板邊界最大X
	double         fPanelRgnMaxY;//整板邊界最大Y	
	tagSpcPanel()
	{
		::memset(&uuidPanel, 0x00, sizeof(uuidPanel));		
		uPanelIndex = 0;
		::memset(sPanelBarcode, 0x00, sizeof(sPanelBarcode));
		nPanelTestResultID = 0;
		nPanelCheckResultID = 0;
		fPanelRgnMinX =-DBL_MAX;
		fPanelRgnMinY =-DBL_MAX;
		fPanelRgnMaxX = DBL_MAX;
		fPanelRgnMaxY = DBL_MAX;
	};
} TSpcPanel, *PSpcPanel;
//-------------------------------------------------------------------------------------//
typedef struct tagSpcBoard
{
	UUID           uuidBoard;//單板唯一碼
	int            uPanelIndex;//整板編號
	int            uBoardIndex;//單板編號
	wchar_t        sBoardBarcode[128];//單板條碼
	int            nBoardTestResultID;//單板檢測結果
	int            nBoardCheckResultID;//單板確認結果
	double         fBoardRgnMinX;//單板邊界最小X
	double         fBoardRgnMinY;//單板邊界最小Y
	double         fBoardRgnMaxX;//單板邊界最大X
	double         fBoardRgnMaxY;//單板邊界最大Y
	tagSpcBoard()
	{
		::memset(&uuidBoard, 0x00, sizeof(uuidBoard));		
		uPanelIndex = 0;
		uBoardIndex = 0;
		::memset(sBoardBarcode, 0x00, sizeof(sBoardBarcode));
		nBoardTestResultID = 0;
		nBoardCheckResultID = 0;
		fBoardRgnMinX =-DBL_MAX;
		fBoardRgnMinY =-DBL_MAX;
		fBoardRgnMaxX = DBL_MAX;
		fBoardRgnMaxY = DBL_MAX;
	};
} TSpcBoard, *PSpcBoard;
//-------------------------------------------------------------------------------------//
typedef struct tagSpcComponent
{
	UUID           uuidComponent;//零件唯一碼
	int            uPanelIndex;//整板編號
	int            uBoardIndex;//單板編號
	int            uComponentIndex;//零件編號
	wchar_t        sComponentName[64];//零件名稱
	wchar_t        sPartNumber[64];//料號名稱
	wchar_t        sModelName[64];//模組名稱
	wchar_t        sNozzleName[64];//吸嘴名稱	
	double         fStageCornerPosX[4];//機台角落座標-X
	double         fStageCornerPosY[4];//機台角落座標-Y
	double         fOffsetX;//偏移X
	double         fOffsetY;//偏移Y
	double         fSkewAngle;//偏移角度

	int            nTestResultID;//檢測結果
    unsigned int   uTestDefectID;//檢測瑕疵
	int            nCheckResultID;//確認結果
    unsigned int   uCheckDefectID;//確認瑕疵
	tagSpcComponent()
	{
		::memset(&uuidComponent, 0x00, sizeof(uuidComponent));		
		uPanelIndex = 0;
		uBoardIndex = 0;
		uComponentIndex = 0;
		::memset(sComponentName, 0x00, sizeof(sComponentName));
		::memset(sPartNumber, 0x00, sizeof(sPartNumber));
		::memset(sModelName, 0x00, sizeof(sModelName));
		::memset(sNozzleName, 0x00, sizeof(sNozzleName));
		fStageCornerPosX[0] = DBL_MAX;
		fStageCornerPosX[1] = DBL_MAX;
		fStageCornerPosX[2] = DBL_MAX;
		fStageCornerPosX[3] = DBL_MAX;
		fStageCornerPosY[0] = DBL_MAX;
		fStageCornerPosY[1] = DBL_MAX;
		fStageCornerPosY[2] = DBL_MAX;
		fStageCornerPosY[3] = DBL_MAX;

		fOffsetX = 0;
		fOffsetY = 0;
		fSkewAngle = 0;
		nTestResultID = 0;
		uTestDefectID = 0;
		nTestResultID = 0;
		nCheckResultID = 0;
		uCheckDefectID = 0;		
	};
} TSpcComponent, *PSpcComponent;
//-------------------------------------------------------------------------------------//
typedef struct tagSpcWindow
{	
	UUID           uuidWindow;//框唯一碼
	UUID           uuidComponent;//框唯一碼
	int            uComponentIndex;//零件編號
	int            uWindowIndex;//檢測框編號
	int            nDefectID;//瑕疵編號
	int            nAlgorithmID;//演算法編號
	RECT           rcImageRect;//視窗影像區域
	double         fStageCornerPosX[4];//機台角落座標-X
	double         fStageCornerPosY[4];//機台角落座標-Y
	tagSpcWindow()
	{
		::memset(&uuidWindow, 0x00, sizeof(uuidWindow));
		::memset(&uuidComponent, 0x00, sizeof(uuidComponent));		
		uComponentIndex = 0;
		uWindowIndex = 0;
		
		nDefectID = 0;//瑕疵編號
		nAlgorithmID = 0;//演算法編號
		rcImageRect.left = rcImageRect.top = 0;//視窗影像區域
		rcImageRect.right = rcImageRect.bottom = 0;

		fStageCornerPosX[0] = DBL_MAX;
		fStageCornerPosX[1] = DBL_MAX;
		fStageCornerPosX[2] = DBL_MAX;
		fStageCornerPosX[3] = DBL_MAX;
		fStageCornerPosY[0] = DBL_MAX;
		fStageCornerPosY[1] = DBL_MAX;
		fStageCornerPosY[2] = DBL_MAX;
		fStageCornerPosY[3] = DBL_MAX;		
	};
} TSpcWindow, *PSpcWindow;
//-------------------------------------------------------------------------------------//
#endif//_AOI_FILE_SPC_DEF_H_