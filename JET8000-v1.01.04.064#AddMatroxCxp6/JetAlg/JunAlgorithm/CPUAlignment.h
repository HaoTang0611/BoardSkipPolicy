#pragma once

// 1. 20250801 Version 5.0.1 修改演算法給 3D AOI合併
// 2. 20250814 Version 5.1.0 提升 標準點位的精度
// 3.                        修改 結果影像交叉黃線的計算方式
// 4.                        修改 Add Pin 的計算方式, 可一次框選多個
// 5.                        修改 Delete Pin 的計算方式, 可一次框選多個
// 6. 20250821 Version 5.1.1 增加黃色交叉線的穩定性

#include "stdafx.h"
#include "JET_ImageFunction.h"

#ifdef FOR_MACHINE
	#if (FOR_MACHINE == JET_MACHINE_8000 || FOR_MACHINE == JET_MEAUSREMENT_UI)

	#include "ImageProcessBaseStruct.h"

	// 線顯示的資料結構
	struct DrawLine
	{
		POINT ptStart;		// 線的起始點
		POINT ptEnd;		// 線的中止點

		DrawLine()
		{
			ptStart.x = -1;
			ptStart.y = -1;

			ptEnd.x = -1;
			ptEnd.y = -1;
		}
	};

	// 紀錄每一根 Pin的位置
	struct PinPositionData
	{
		int nNo;			// 編號

		// 20190712 Jun+
		RGBQUAD rgbBodyColor;	// 紀錄 Pin Body的顏色(彩色)
		CRect crBodyROI;		// Pin Body 在影像上的中心點座標

		POINT ptCenter;		// Pin在影像上的中心點座標
		POINT ptMap;	// 紀錄Map編號

		PinPositionData()
		{
			nNo = -1;

			rgbBodyColor.rgbBlue = 0;
			rgbBodyColor.rgbGreen = 0;
			rgbBodyColor.rgbRed = 0;
			rgbBodyColor.rgbReserved = 0;

			crBodyROI.left = -1;
			crBodyROI.right = -1;
			crBodyROI.top = -1;
			crBodyROI.bottom = -1;

			ptCenter.x = -1;
			ptCenter.y = -1;

			ptMap.x = -1;
			ptMap.y = -1;
		}
	};

	// 每一個 Pin 的檢測結果
	struct DetectData
	{
		int				nNo;				// 編號 (同一根針角需編成2個ID,不可重複)
		POINT			ptCenter;			// Pin在影像上的中心點座標				
		POINT			ptOffset;			// Pin在 X Y 方向的偏移量
		bool			bOkNg;				// 檢測結果

		DetectData()
		{
			nNo = -1;
			ptCenter.x = -1;
			ptCenter.y = -1;

			ptOffset.x = 0;
			ptOffset.y = 0;
			bOkNg = false;
		}
	};

	// 輸入參數的資料結構
	struct  InputData
	{
		// 20190712 Jun+
		Mat				matSrc;						// 要檢測的影像

													//20230331 Jun+
		bool			bUseAI;						// 是否使用AI模式進行檢測

		bool			bInspectionBody;			// 偵測Pin Body是否過亮 
		int				nBodyThres_Gray;			// CPU 本體的灰階閥值
		int				nBodyThres_RGB;				// CPU 本體的RGB閥值
													//20230331 Jun+

		int				nImageWidth;				// pImage 影像的寬
		int				nImageHeight;				// pImage 影像的高
		int				nMode;						// 模式切換, 0=建立CPU點位, 1=檢測.

		int				nPin_Number;					//pin 的數目
		std::vector<PinPositionData> PinPosition;		//記錄pin position 的位置

		int				nInitialThres;				// CPU 針角的初始化閥值



		int				nPinWidth;					// Pin的寬度(pixels)
		int				nPinHeight;					// Pin的高度(pixels)

		int				nPin_Search_X;				// Pin角 X 方向搜尋範圍(Pixels)
		int				nPin_Search_Y;				// Pin角 Y 方向搜尋範圍(Pixels)
		int				nPin_Interval_Slant_X;		// Pin角 X 方向間距(斜)(pixels)
		int				nPin_Interval_Slant_Y;		// Pin角 Y 方向間距(斜)(pixels)
		int				nPin_Interval_Straight_X;	// Pin角 X 方向間距(水平)(pixels)
		int				nPin_Interval_Straight_Y;	// Pin角 Y 方向間距(垂直)(pixels)
		int				nDistThres_X;				// Pin角 X 方向偏移距離的閥值(X方向偏移量超過此值即報錯)(pixels)
		int				nDistThres_Y;				// Pin角 Y 方向偏移距離的閥值(X方向偏移量超過此值即報錯)(pixels)

		int				nFilterSize;				// 二值化完後 型態學 opening的大小
		int				nMinLinePoint;				// 一條線最少要幾個點
		int				nDiffCount;					// 相鄰兩條線的差異顆數
		int				nTimes;						// 搜尋定為起始點的圈數
		float			fScale;						// 影像縮放比例

		InputData()
		{
			// 20230331 Jun+
			bUseAI = false;

			// 20190712 Jun+
			bInspectionBody = false;
			nBodyThres_Gray = -1;
			nBodyThres_RGB = -1;


			nImageWidth = -1;
			nImageHeight = -1;
			nMode = -1;

			nPin_Number = -1;
			nInitialThres = -1;

			nPinWidth = -1;
			nPinHeight = -1;

			nPin_Search_X = -1;
			nPin_Search_Y = -1;
			nPin_Interval_Slant_X = -1;
			nPin_Interval_Slant_Y = -1;
			nPin_Interval_Straight_X = -1;
			nPin_Interval_Straight_Y = -1;
			nDistThres_X = -1;
			nDistThres_Y = -1;

			nFilterSize = -1;
			nMinLinePoint = 10;
			nDiffCount = 3;
			nTimes = 2;
			fScale = 1.0;
		}
	};

	// 20250725 Jun+
	// CPU輸入參數的資料結構 ===8000機台專用===
	struct  InputData_8000
	{
		// 預設 10.0
		// 水平方向 1 pixels 對應的實際距離, 單位:um
		float			fResolutionX;

		// 預設 10.0
		// 垂直方向 1 pixels 對應的實際距離, 單位:um
		float			fResolutionY;

		// 預設值 1
		// Pin 的分佈模式, 1=棋盤狀, 2=蜂巢狀
		int             nLayoutType;

		// 預設值 1
		// Pin的寬度, 單位:um
		int				nPinWidth;	

		// 預設值 1
		// Pin寬度的誤差範圍, 單位:um
		int				nPinWidth_Range;

		// 預設值 1
		// Pin的高度, 單位:um
		int				nPinHeight;	

		// 預設值 1
		// Pin高度的誤差範圍, 單位:um
		int				nPinHeight_Range;

		// 預設值 1
		// Pin 水平 方向間距, 單位:um
		mutable int		nPin_Interval_X;

		// 預設值 1
		// Pin 垂直 方向間距, 單位:um
		mutable int		nPin_Interval_Y;

		// 預設值 50
		// Pin 水平方向位移閥值(X方向偏移量超過此值即報錯), 單位:um
		int				nDistThres_X;	

		// 預設值 50
		// Pin 垂直方向位移閥值(Y方向偏移量超過此值即報錯), 單位:um
		int				nDistThres_Y;				

		// 預設值 5
		// 一條線最少要幾個點
		int				nMinLinePoint;	

		// 預設值 3
		// 相鄰兩條線的差異顆數
		int				nDiffCount;		

		// 預設值 2
		// 搜尋定為起始點的圈數
		int				nTimes;						

		// 預設值 0
		// pin 的數目, nMode為1時 要輸入
		mutable int				nPin_Number;

		//記錄 pin position 的位置, nMode為1時 要輸入 vtsPinPosition.size() 必須等於 nPin_Number
		mutable std::vector<PinPositionData> vtsPinPosition;

		// 找Pin的處理模式
		JET::alg::SFindEdge_Parameter		sFindPin;

		InputData_8000() : fResolutionX(10.0), fResolutionY(10.0), nLayoutType(1), nPinWidth(1), nPinWidth_Range(1), nPinHeight(1), nPinHeight_Range(1),
			nPin_Interval_X(1), nPin_Interval_Y(1), nDistThres_X(50), nDistThres_Y(50), nMinLinePoint(5), nDiffCount(3), nTimes(2), nPin_Number(0), sFindPin() 
		{
			sFindPin.nOpenX = 7;
			sFindPin.nOpenY = 7;
			sFindPin.vtsColorExtraction[0].nRange = 50;
			sFindPin.vtsGrayExtraction[0].nValue_Gray = 205;
			sFindPin.vtsGrayExtraction[0].nRange = 50;
		}

		~InputData_8000() = default;

		bool Check(string& strInfo) const 
		{
			if (fResolutionX <= 0.0) {
				strInfo = "ResolutionX is error";
				return false;
			}

			if (fResolutionY <= 0.0) {
				strInfo = "ResolutionY is error";
				return false;
			}

			if (nLayoutType < 1 || nLayoutType > 2) {
				strInfo = "LayoutType is Error";
				return false;
			}

			if (nDistThres_X <= 0 || nDistThres_Y <= 0) {
				strInfo = "DistThres is Error";
				return false;
			}

			if (nPinWidth <= 0) {
				strInfo = "Pin width is Error";
				return false;
			}

			if (nPinWidth_Range < 0) {
				strInfo = "Pin width Range is Error";
				return false;
			}

			if (nPinHeight <= 0) {
				strInfo = "Pin height is Error";
				return false;
			}

			if (nPinHeight_Range < 0) {
				strInfo = "Pin height Range is Error";
				return false;
			}

			if (nPin_Interval_X <= fResolutionX) {
				strInfo = "Pin horizontal interval is Error";
				return false;
			}

			if (nPin_Interval_Y <= fResolutionY) {
				strInfo = "Pin vertical interval is Error";
				return false;
			}

			if (nMinLinePoint <= 1) {
				strInfo = "Points less than 2";
				return false;
			}

			if (nDiffCount < 1) {
				strInfo = "Points less than 2";
				return false;
			}

			if (nTimes < 1) {
				strInfo = "Search laps less than 1";
				return false;
			}

			if (nPin_Number != vtsPinPosition.size()) {
				strInfo = "Pin Data is Error";
				return false;
			}

			string strData;
			if (!sFindPin.Check(strData)) {
				strInfo = "Find Pin Error-" + strData;
				return false;
			}

			return true;
		}
	};

	// 結果輸出的資料結構
	struct stOutputData
	{
		int nMapShift_X;		// Map X方向的偏移量
		int nMapShift_Y;		// Map Y方向的偏移量

		double dImageShift_X;	// 檢測影像與建立Map_STD影像在X方向的偏移量
		double dImageShift_Y;	// 檢測影像與建立Map_STD影像在Y方向的偏移量

		std::vector<DetectData>	vtResult;	// 紀錄每一個 Pin 的檢測結果
		std::vector<DrawLine>	vtLine;		// 紀錄要顯示的線條

		stOutputData() : nMapShift_X(0), nMapShift_Y(0), dImageShift_X(0.0), dImageShift_Y(0.0) {}
		~stOutputData() = default;

		void Initial()
		{
			nMapShift_X = 0;
			nMapShift_Y = 0;
			dImageShift_X = 0.0;
			dImageShift_Y = 0.0;
			vtResult.clear();
			vtLine.clear();
		}
	};

	#endif // FOR_MACHINE == JET_MACHINE_7000
#endif // FOR_MACHINE

class CPU_Alignment
{

public:
	InputData					m_stInput;
	stOutputData*				m_pstOutput;
	vector<PinPositionData>		m_vtPinPos;


private:
	bool			m_bAI_Ok;				// 判斷 AI 是否就緒
	bool			m_bSaveImage;			// 測試用影像存檔
	bool			m_bSaveRunImage;		// 只儲存原始影像
	string			m_strSavePah;			// 存檔路徑
	string			m_strErrorMessage;		// 錯誤訊息
	string			m_strVersion;			// 紀錄版次

	// 20230331
	string			m_strAI_FileName;			// AI檔案的名稱
	string			m_strAOI_To_AI_FolderPath;	// 傳資料給AI的資料夾路徑
	string			m_strAI_To_AOI_FolderPath;	// AI回傳結果的資料夾路徑
	string			m_strMachine;				// 目前是哪一台機器
	vector<Rect>	m_vtAI_Pin;					// AI 回傳的 Pin範圍
	int				m_nAI_Model_Id;				// 使用的 AI模型代號
	int				m_nCount_AI_Pin;			// AI回傳的Pin數量
	double			m_dTime_ToAI;				// 建立給AI檔案所花費的時間
	double			m_dTime_AI_Result;			// 讀取AI回傳檔案所花費的時間
	double			m_sTime_AI_Wait;			// AI回傳檔案的實際等待時間
	double			m_dSumTime_AI;				// 從建立AI檔案開始計時到接收AI回傳的資料為止的總時間	
	double			m_dSumTime_CPU;				// CPU演算法總執行時間
	int				m_nMaxWaitTimes;			// AI回傳結果的最大等待時間
	int				m_nAI_FileFormat;			// AI的檔案格式
	bool			m_bDeleteFile;				// 是否刪除已讀取的檔案

	Mat				m_matColor;				// 輸入的彩色影像(Mat格式)
	Mat				m_matGray;				// 輸入的灰階影像(Mat格式)
	Mat				m_matThres;				// 
	Mat				m_matCenter;
	Mat				m_matEdge;				// 紀錄Pin的邊界影像
	Mat				m_matPinCenter;			// 紀錄每一根 Pin 中心點位置的影像
	Mat				m_matPinModel;			// 單一個 Pin角 的影像(拿來當作Pattern match的Model影像)
	Mat				m_matNum;				// 紀錄搜尋過程點位的影像
	

	vector<Mat>		m_vtmatLineIndex;		// 紀錄搜尋過程點位的影像
	int				m_nDefaultValue1;		// 過程計算用
	int				m_nDefaultValue2;		// 過程計算用
	int				m_nPinMeanGray_Pattern;
	int				m_nCount_StdPin;
	int				m_nFirstPointRange;		// 搜尋定位起始點的範圍
	POINT			m_ptSearchStartPt;		// 紀錄搜尋的起始點
	float			m_fDist_PinToBody_X;	// Pin中心點與Body中心在X方向的距離
	float			m_fDist_PinToBody_Y;	// Pin中心點與Body中心在Y方向的距離
	float			m_fBody_Width;			// Body的平均寬度
	float			m_fBody_Height;			// Body的平均高度

	// 20250731 Jun+
	float			m_fResolutionX;
	float			m_fResolutionY;

	// 20250725 Jun+
	int						m_nId_Threshold;

	// 20250725 Jun+
	// 找 Pin的處理流程
	SProcessModeParam		m_sPM_FindPin;

	// 20220624 Jun+
	vector<vector<float>>	m_vtfFeature;			// 紀錄特徵值
	vector<vector<float>>	m_vtfFeature_Thres;		// 特徵的閥值												
	vector<vector<int>>		m_vtusFeature_ID;		// 紀錄 初始 Pin的 ID	


	vector<vector<PinPositionData>>	m_vtptCoordinate_STD;	// 標準 Map
	vector<vector<PinPositionData>>	m_vtptCoordinate;		// 當前影像的 Map
	vector<vector<bool>>			m_vtbBodyResult;		// 紀錄Pin Body的檢測結果

	vector<double>					m_vtdAffineCoefficient;	// 彷射轉換方程式係數

	vector<vector<Point2f>>			m_vt2ptfCoordinate_ReSlope;	// 紀錄標準點位用來重算交叉線
public:

	CPU_Alignment();
	~CPU_Alignment();

#pragma region 8000
#ifdef FOR_MACHINE
	#if (FOR_MACHINE == JET_MACHINE_8000 || FOR_MACHINE == JET_MEAUSREMENT_UI)
	// 儲存參數
	// [In]      sParam : 要儲存的參數
	// [In] strPathName : 儲存路徑+檔名(不須要檔名)
	bool Save_Parameter(const InputData_8000& sParam, const string& strPathName);

	// 讀取參數
	// [In] strPathName : 讀取路徑+檔名(不須要檔名)
	// [In]      sParam : 讀取後存放資料的變數
	bool Load_Parameter(const string& strPathName, InputData_8000& sParam);

	// Pin偏移檢測 --- 建立Pin座標
	// [In] matImage : 要建立Pin點位資訊的影像, 必須為彩色影像 
	// [In]   sParam : Pin偏移檢測的相關參數
	// [Out] sResult : Pin偏移檢測的結果
	bool CPUAlignment_CreatePin(const Mat& matImage, const InputData_8000& sParam, stOutputData& sResult);

	// Pin偏移檢測 --- 檢測用 
	// [In] matImage : 要檢測Pin偏移的影像, 必須為彩色影像 
	// [In]   sParam : Pin偏移檢測的相關參數
	// [Out] sResult : Pin偏移檢測的結果
	bool CPUAlignment_Inspection(const Mat& matImage, const InputData_8000& sParam, stOutputData& sResult);

	// 輸出檢測結果
	// [In]        sResult : Pin 偏移檢測的結果
	// [In][Out] matResult : 顯示檢測結果的影像(CPUAlignment_Inspection()的輸入影像),必須為彩色影像
	bool ShowResult(const stOutputData &sResult, Mat matResult);

	// 輸出檢測結果---UI程式用
	// [In]        sResult : Pin 偏移檢測的結果
	// [In][Out] matResult : 顯示檢測結果的影像(CPUAlignment_Inspection()的輸入影像), 必須為彩色影像
	// [Out]     vtstrInfo : 要顯示在UI程式的資訊 
	bool ShowResult(const stOutputData &sResult, Mat matResult, vector<string>& vtstrInfo);

	// 增加 Pin點---UI程式用
	// 輸入一個範圍, 將框選範圍內線的交點都當作要新增的點
	// [In]   crectRange : 要增加Pin的範圍
	// [In][Out] sResult : 輸入原始 Pin Map與所有的線, 輸出增加Pin後的Map 
	bool AddPin(const RECT &rectRange, vector<PinPositionData>& vtsStdPin_New);

	// 調整 Find Pin 參數---UI程式用
	// matInput : 必須是彩色影像
	//    nStep : 值域 [1,6]
	bool AdjustmentFindPinParameter(const Mat& matInput, const int& nStep, const JET::alg::SFindEdge_Parameter& sParam, vector<jet_imagefunction::SProcessModeOutput>& vtsResult, Mat& matOutput);
	#endif // FOR_MACHINE == JET_MACHINE_7000
#endif // FOR_MACHINE
#pragma endregion

	// 取得CPU演算法的版次
	string GetVersion() const { return m_strVersion; }

	// 設定 AI 目前狀態 
	// bOk = true , 代表AI可以執行 
	void SetAIState(const bool& bOk) { m_bAI_Ok = bOk; };

	// 20230325 Jun+
	// strAIFileName : AI檔案的名稱
	// strAIDataFolder : 傳資料給AI的資料夾路徑
	// strAIResultFolder : AI回傳結果的資料夾路徑
	bool SetAI_Info(const string& strAI_FileName, const string& strAOI_To_AI_Folder, const string& strAI_To_AOI_Folder, const string& strMachine, const bool& bDelete);

	// CPU 針角檢測, 檢測結果須保存, 同一根針角需編成2個ID,不可重複
	int CPUAlignment(InputData &stInput, stOutputData &stOutput, vector<PinPositionData> &vtPinPos);

	// 20230331 Jun+
	// 用輸入點位計算 目前 Map Index
	// 再用仿射轉換計算 Map STD 的座標
	bool ReCreateMap(vector<PinPositionData> &vtPinPos, const POINT &ptAddPin);

	// 20250818 Jun+
	bool ReCreateMap(vector<PinPositionData> &vtPinPos, const CRect &crectRange);

	// 刪除一個 Pin 點位, 並重新排序
	int DeletePin(vector<PinPositionData> &vtPinPos, const CRect &crectRange);

	void GetErrorMessage(CString &cstrMessage);

	// 取出搜尋起始點
	POINT GetStartPoint();

	// 設定是否開啟存檔功能
	// bSave 若為 true, 則會將過程影像儲存到 SetSavePath()所設定的路徑下
	void SetSaveImageFlag(bool bSave);

	// 設定存檔路徑
	// strPath = 存放檔案資料夾的路徑
	void SetSavePath(string strPath);

	// 將檢測結果存成 txt檔, 並將線跟框畫在 matImage影像上
	// 檔案會儲存到 SetSavePath()所設定的路徑下
	// strFileName = 存檔檔名
	void OutputResult(Mat matImage, const stOutputData &stOutput, string strFileName);
	void OutputResult(const stOutputData &stOutput, string strFilePathName);

	// 輸出Map檔(.txt)
	// 檔案會儲存到 SetSavePath()所設定的路徑下
	// vtptCoordinate = 要輸出的 Data
	void OutputMap_No(const vector<vector<PinPositionData>> &vtptCoordinate, string strFileName);

	// 輸出 m_vtptCoordinate
	void OutputMap(string strFileName);

	// 輸出 m_vtptCoordinate_STD
	void OutputMap_STD(string strFileName);

	// 輸出 m_vtptCoordinate
	void OutputBadyValue(string strFileName);

	// 讀取 Map檔案
	// strFilePathName : Map的完整路徑, 包含檔名與副檔名
	void Load_Map(const string& strFilePathName, vector<PinPositionData> &vtData, InputData& sPaam = InputData());
	void Load_Map(const string& strFilePathName, InputData& sPaam);

	void SaveRunTimeImage(bool bSave, CString cstrPath, const Mat &matImage);

private:

#pragma region 8000
#ifdef FOR_MACHINE
	#if (FOR_MACHINE == JET_MACHINE_8000 || FOR_MACHINE == JET_MEAUSREMENT_UI)
	// 20250725 Jun+
	// bCreatePin : true代表是 CreatePin模式
	bool DataCheck_8000(const bool& bCreatePin, const Mat& matImage, const InputData_8000 &stInput, stOutputData& stOutput);

	// 20250725 Jun+
	// 設定找 Pin 的流程
	bool SetFindPinProcess(const JET::alg::SFindEdge_Parameter& sParam, SProcessModeParam& sPM);

	// 執行找 Pin 流程
	bool RunProcess_FindPin(const JET::alg::SFindEdge_Parameter& sParam);
	#endif // FOR_MACHINE == JET_MACHINE_7000
#endif // FOR_MACHINE
#pragma endregion

	// 輸入值檢察
	int DataCheck(InputData &stInput);

	// 建立 Data
	int CPUAlignment_Mode0(InputData &stInput, stOutputData &stOutput, vector<PinPositionData> &vtPinPos);

	// 檢測
	int CPUAlignment_Mode1(InputData &stInput, stOutputData &stOutput, vector<PinPositionData> &vtPinPos);

	// 檢測--使用AI找Pin 20230325
	int CPUAlignment_Mode1_AI(InputData &stInput, stOutputData &stOutput, vector<PinPositionData> &vtPinPos);

	// 將 m_stInput.vtpu8SrcImage 轉成 Mat格式(重新建立記憶體位置)
	int InputBufferToMat();

	// 20220624 Jun+ 影像增強
	int ImageEnhance(const Mat& matSrc, Mat& matDst, const int& nMode, const int& nGaussSize, const int& nGain);

	// 20220624 Jun+
	void Morphology_Open(Mat& matImage, const int& nSize);

	// 20220624 Jun+
	// Pin 的特徵值計算
	int CalPinFeature(const vector<vector<POINT>>& vtEdgePoint, vector<Point2f>& vtcvfptPin, vector<RECT>& vtrtPinROI);

	// 特徵比對-長寬比
	int Feature_AspectRatio(Mat& matThres);

	// 特徵比對-面積
	int Feature_Area(Mat& matThres);

	// 特徵計算-重心
	int Feature_CenterOfGravity(const Mat& matThres, vector<POINT>& vtCenter);

	// 20220221 Jun+
	// 20220308 Jun+
	// 計算每一根Pin的中心點座標
	int GetCpuCenterPoint();

	// 20230419 Jun+
	int GetCpuCenterPoint_Line();

	// 20220624 Jun+ 新型CPU用
	// 計算每一根Pin的中心點座標
	int GetCpuCenterPoint_New();

	// 局部影像強化
	void ROIEnhance(const int& nMode, const int& nGaussSize, const int& nGain, const Rect& cvROI, Mat& matEnhance);

	// 計算 Pin 的特徵值--單點
	int Feature_Pin(const POINT& ptPoint, int& nPerimeter);
	int Feature_Pin(const POINT& ptPoint, int& nPerimeter, const Scalar& scColor);

	// 計算 Pin 的特徵值--範圍
	int Feature_Pin(const POINT& ptPoint, const int& nSearchX, const int& nSearchY, int& nPerimeter, Rect& cvROI);
	int Feature_Pin(const POINT& ptPoint, const int& nSearchX, const int& nSearchY, int& nPerimeter, Rect& cvROI, const Scalar& scColor);

	// 決定搜尋起始點
	// 搜尋範圍(pixels)
	// ptStart = 起始點位置
	int DefineStartPoint(POINT &ptStart);

	// 20220624 Jun 修改 搜尋起始點的定義
	// 找出所有符合 BasePin 條件的 Pin 
	// nTimes = 圈數
	// nSearchRange = 搜尋範圍(pixels)
	// vtptBasePoint = 所有符合 BasePin 條件的 Pin
	int SeachBasePin(const int& nTimes, const int& nSearchRangeX, const int& nSearchRangeY, vector<POINT>& vtptBasePoint);

	// 找出疑似Pin的點
	void FindPoint(const POINT& ptStart, const int& nSearchRangeX, const int& nSearchRangeY, vector<POINT>& vtptPin, int& nCount);

	// 輸入多個疑似Pin的點位 用Match比分數最高的
	//void FindPin_Match(const vector<POINT>& vtptPoint, const int& nMatchRangeX, const int& nMatchRangeY, int& nPinIndex);
	void FindPin_Match(const POINT& ptCenter, const vector<POINT>& vtptPoint, const int& nMatchRangeX, const int& nMatchRangeY, int& nPinIndex);
	void FindPin_Match(POINT& ptPoint, const int& nMatchRangeX, const int& nMatchRangeY, POINT& ptPin, int& nScore, const bool& bAbsolute=false);

	// 20220624 Jun+ 參考隔壁的線來判斷目前線上是否還有Pin
	void FindMissPin_Refer(const int& nReferIndexX, const int& nReferIndexY, const int& nCount_Refer, const bool& bSlope1, const int& nSearchY, const int& nIndexX, const int& nIndexY, int& nCount, vector<vector<POINT>>& vtLinePoint);

	// 20250804 Jun+ 參考自己來判斷隔壁的線是否還有 Pin
	void FindMissPin_Refer2(const int& nReferIndexX, const int& nReferIndexY, int& nCount_Refer, const bool& bSlope1, const int& nSearchY, const int& nIndexX, const int& nIndexY, const int& nCount, vector<vector<POINT>>& vtLinePoint);

	// 20220624 Jun+ 找出最上 最下的Pin, 再往上 往下搜尋
	void FindMissPin_EndPoint(const int& nSearchX, const int& nSearchY, const int& nIndexX, int& nIndexY, int& nCount, vector<vector<POINT>>& vtLinePoint);

	// 20220624 Jun+ 從端點再往上或往下搜尋
	void SearchPin_EndPoint(POINT ptStart, const int& nIntervalX, const int& nIntervalY, int& nCount, vector<POINT>& vtptPin);

	// 20220624 Jun+ 搜尋整張影像的 Pin中心點 (斜線 : 左上->右下
	int SearchCpuPin_Slope1(POINT ptStart, vector<int> &vtnCount, vector<POINT> &vtYIndex, vector<double> &vtSlope, vector<vector<POINT>> &vtLinePoint);

	// 搜尋一條線的 Pin中心點 (斜線 : 左上->右下)
	int SearchPoint_Slope1(POINT ptStart, int &nStartIndexY, int &nCount, vector<POINT> &vtLinePoint);

	// 搜尋整張影像的 Pin中心點 (斜線 : 右上->左下)
	int SearchCpuPin_Slope2(POINT ptStart, vector<int> &vtnCount, vector<POINT> &vtYIndex, vector<double> &vtSlope, vector<vector<POINT>> &vtLinePoint);

	// 搜尋一條線的 Pin中心點 (斜線 : 右上->左下)
	int SearchPoint_Slope2(POINT ptStart, int &nStartIndexY, int &nCount, vector<POINT> &vtLinePoint);

	// 偵測 指定點位附近 有沒有白點
	void  DetectionWhitePoint_Old(POINT ptStart, POINT &ptWhite);

	// 20220624 Jun+ 由中心往外搜尋
	void  DetectionWhitePoint(const POINT& ptStart, POINT &ptWhite);

	// 偵測 指定範圍內 有沒有圓球
	// nIntervalX = X 方向的偵測半徑
	// nIntervalY = Y 方向的偵測半徑
	void DetectionPin(POINT ptCenter, POINT &ptPin);

	// 20220308 Jun+ 重新二值化(閥值減少) 找Pin
	void FindPin(const int &nTop, const int &nBottom, const int &nLeft, const int &nRight, POINT& ptPin);

	// 圖形比對 
	int Match(Mat &matInput, int nMethod, float &fScore, POINT &LeftTopPoint);

	// 灰階共生矩陣二值化
	int Threshold_GLCM(const Rect& cvROI, const int& nRange, Mat& matThres);

	// 計算單一搜尋方向的平均斜率與每條線的平均截距
	int CalculateMeanSlope(vector<vector<int>> vtnCount, vector<vector<POINT>> vtYIndex, vector<vector<double>> vtdLineSlope, vector<vector<vector<POINT>>> vtSLine, vector<Mat> &matLint_A, vector<Mat> &matLint_B, vector<double> &vtdMeanSlope, stOutputData &stOutput);

	// 20190610 從Map中找出屬於邊界的Pin
	// pair<POINT, int>    POINT = Map Index
	// pair<POINT, int>    int = Map的區域, 1=上, 2=下, 3=左, 4=右
	int FinPin_Edge(vector<pair<POINT, int>> &vtMapIndex);

	// 判斷是否是在邊界的 Pin
	// nPosition => 1=上, 2=下, 3=左, 4=右
	int DetectionPin_Edge(const int &nMapIndexX, const int &nMapIndexY, const bool &bSquare, int &nPosition);

	// 判斷 Pin主體的位置
	// pair<POINT, int>    POINT = Map Index
	// pair<POINT, int>    int = Map的區域, 1=上, 2=下, 3=左, 4=右
	int DetectionPin_Body(const vector<pair<POINT, int>> &vtMapIndex, const Point &ptStart, Mat &matPinBody, vector<CRect> &vtcrBodyROI, int &nPosition);
	
	// 20220308 Jun+ Pin Body 不明顯時用
	int DetectionPin_Body_New(const int &nMapShiftX, const int &nMapShiftY, const vector<pair<POINT, int>> &vtMapIndex, const Point &ptStart, Mat &matPinBody, vector<CRect> &vtcrBodyROI, int &nPosition);

	// 計算 單一個Pin Body 邊界座標所形成的斜率
	int GetBodySlope(const CRect &vrBodyROI, const int &nPosition, float &fSlope);

	// 計算出所有 Pin Body的位置與顏色
	int GetBodyValue(int nMapShiftX, int nMapShiftY, const vector<pair<POINT, int>> &vtMapIndex, const int &nPosition, const Mat &matPinBody, vector<CRect> &vtcrBodyROI, stOutputData &stOutput);

	// 找出顏色不同的 Pin Body
	int GetDifferBody(stOutputData &stOutput);
	int GetDifferBody();

	// 用顏色來判斷 Pin Body是否歪斜
	int Inspection_PinBody(int nMapShiftX, int nMapShiftY, const POINT &ptStart, stOutputData &stOutput);

	// 計算2個Map的偏移距離
	int GetMapShift(const int &nMapShiftX, const int &nMapShiftY, int &nPinShiftX, int &nPinShiftY);

	// 使用二值化與 pattern match來找 Pin
	int FindPin_Threshold_And_Match(const POINT& ptSearchPoint, POINT& ptPin);

	// 輸入搜尋起始點與搜尋範圍來計算Pin的中心點
	int FindPin_GLCM(const POINT& ptSearchPoint, const int& nSearchRangeX, const int& nSearchRangeY, POINT& ptPin);

	// 計算偏移量
	int CalculateOffset(int nMapShiftX, int nMapShiftY, stOutputData &vtData);
	int CalculateOffset(vector<DetectData> &vtData);

	// 初始化 vtptCoordinate
	void Init_Map(vector<vector<PinPositionData>> &vtptCoordinate);

	// 20220624 Jun+ 輸出建立Map的過程
	void OutputCreateMapProcess(Mat& matImage, const int& nPtX, const int& nPtY, const int& nId, const int& nMapIndexX, const int& nMapIndexY, const Scalar& scColor);

	// 20220624 Jun+ 搜尋 Map線的交點
	void FindCross_Pin(Mat& matCrossLine, const POINT& ptCenter, const int& nSearchX, const int& nSearchY, const uchar& ucColor_Pin, vector<POINT>& vtptSearchPin);
	void FindCross_Line(Mat& matCrossLine, const POINT& ptCenter, const int& nSearchX, const int& nSearchY, const uchar& ucColor_Line, vector<POINT>& vtptSearchPin);

	// 建立 Map矩陣 m_vtptCoordinate
	int CreateMap(const vector<Mat> &matLint_A, const vector<Mat> &matLint_B, const POINT ptSearchStart, stOutputData &stOutput);

	// 用 m_vtptCoordinate_STD 來檢察 m_vtptCoordinate 內容是否正確
	int AlignmentMap(int &nMapShiftX, int &nMapShiftY);

	// 2維Map 轉1維
	int Map2DTo1D(const vector<vector<PinPositionData>> &vtstMap, vector<PinPositionData> &vtPinPosData);

	// 1維Map 轉2維
	int Map1DTo2D(const vector<PinPositionData> &vtPinPosData, vector<vector<PinPositionData>> &vtstMap);

	// 計算線性方程式
	// vPInput = 輸入座標
	// 若 bChange = true, 則 X座標與Y座標互換
	// nStartIndex = 第一個Data的位置
	// nCount = 要被計算的 Data數
	// dSlope = 輸出方程式的斜率
	// dIntercept = 輸出方程式的截距
	int SimpleLinearRegr(std::vector<POINT> &vPInput, bool bChange, int nStartIndex, int nEndIndex, int nCount, double &dSlope, double &dIntercept);

	// BlobAnalysis
	//int CpuBlobAnalysis(const Mat& matEdge, vector<CRect> &vtcretRectangle, vector<Point> &vtcvptCenter, Mat &matCenter);

	// 20190610 找Pin的主體
	int BlobAnalysis_PinBody(Mat &matPinBody, vector<CRect> &vtcrectROI);

	// 搜尋 matPinBody影像中有無 Body中心點, 若有中心點再計算 Body的 RGB Value
	// return : 若有Body則return true; 若無則 return false
	bool SearchBodyAndColor(const CRect &SearchROI, const Mat &matPinBody, const vector<CRect> &vtcrBodyROI, const vector<Mat> &vtmatBGR, PinPositionData &BodyData);

	// 搜尋 m_matGray影像中有無 Body, 若有Body再計算 Body的 RGB Value
	// return : 有Body則return true; 若無則 return false
	bool SearchBodyAndColor(const CRect &SearchROI, const vector<Mat> &vtmatBGR, PinPositionData &BodyData);

	// 對ROI內的影像二值化 找出PinBody
	int BlobAnalysis_ROIToBody(const CRect &crROI, const int &nThres, Mat &matThres, CRect &crtBodyROI);

	// 20220624 Jun+ 輸入中心點與搜尋距離 計算搜尋範圍
	void CalSearchROI(const POINT& ptCenter, const int& SearchX, const int& nSearchY, int& nT, int&nB, int& nL, int& nR, Rect& cvROI);

	// 檢查並修正 ROI 的範圍
	void CheckROI(CRect &ROI);
	void CheckROI(Rect& ROI);
	void FixROI(const int& nImageH, const int& nImageW, Rect& ROI);

	// 計算單一個 Pin Body的 顏色值
	void GetBodyColor(const vector<Mat> &matBody, const Mat &matMask, const CRect &crBodyROI, RGBQUAD &rgbValue);

	// 輸出AI相關資訊
	void OutputParamInfo(string& strData, Mat& matDisplay = Mat());

	// 讀取CPU相關參數
	void LoadParamInfo(const vector<string>& vtstrData, InputData& sPaam);

	// 新增一個 Pin點位, 並重新排序 
	int AddPin(vector<PinPositionData> &vtPinPos, const POINT &ptNewPin);

	// 判斷point是否落在Range中
	bool IsPointInRect(const Point2f& ptfPoint, const RECT& rectRange);

	bool GetAffine(const vector<pair<Point2f, Point2f>> &vtInputPoint, double dRate, const int& nTimes, const float& fErrorPercent, vector<double> &vtdCoefficient);
};




