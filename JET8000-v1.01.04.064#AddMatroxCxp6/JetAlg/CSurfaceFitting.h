//Author : Jun
#pragma once
#include "JETAlg_Std.h"
#include <windows.h>
#include <vector>

using namespace std;
namespace JET {
	namespace alg {

		// 平面擬合類型
		enum ESurfaceType
		{
			// 線性       Z = a1*x + a2*y + a3
			SURFACE_LINE = 0,

			// 高斯		  Z = a1*x + a2*y + a3*xy + a4*x^2 + a5*y^2 + a6
			SURFACE_GAUSS = 1,

			// 曲面	      Z = a1*x + a2*y + a3*xy + a4*x^2 + a5*y^2 + a6*x^2y + a7*xy^2 + a8*x^3 + a9*y^3 + a10
			SURFACE_CURVE = 2,

			SURFACE_MAX = 3,
		};

		// 資料處理模式 : 對ROI範圍內的Data先做篩選
		enum EDataMode
		{
			// 不做任何處理
			DATA_NONE = 0,

			// 以bDataInside 與 nIndent 決定計算範圍後, 再計算範圍內的高度平均值, 範圍內3D高度大於高度平均值才會進行後續計算    
			DATA_MEAN_UP = 1,

			// 以bDataInside 與 nIndent 決定計算範圍後, 再計算範圍內的高度平均值, 範圍內3D高度小於高度平均值才會進行後續計算
			DATA_MEAN_DOWN = 2,

			DATA_MAX = 3,
		};

		// 損失模式 : 遞迴收斂條件
		enum ELossesMode
		{
			// 最小高度差-- 計算擬合平面4個角點的高度差,取高度差最小值(適用在良品多的時候)           
			LOSSES_HEIGHT_MIN = 0,

			// 最小平均誤差--計算擬合平面與實際高度的平均誤差,取誤差最小值(適用在3D資訊穩定的時候)
			LOSSES_MEANERROR_MIN = 1,

			// 誤差閥值--前後次擬合平面與實際高度的誤差閥值,大於1時將會繼續遞迴(計算速度最快, 有一定準卻度)
			LOSSES_ERRORTHRES = 2,

			LOSSES_MAX = 3,
		};

		// 平面擬合輸入參數
		struct SSurfaceParam
		{
			// 是否建立擬合後的平面
			bool bCreateSurface;

			// 從零件ROI內的Data做完篩選後, 再內縮 nIndent 距離後, 若 bDataInside 為true, => 取內縮邊界內的Data
			//                                               false, => 取ROI邊界到內縮邊界的Data
			bool bDataInside;

			// 平面擬合類型
			int nSurfaceType;

			// 資料處理模式 : 對ROI範圍內的資料先進行篩選
			int nDataProcessMode;

			// 損失模式--遞迴時的收斂條件
			int nLossesMode;

			// 高度閥值--原始3D高度低於此值將不進行平面擬合
			float fHeightThreshold;

			// 內縮距離(Pixels)
			int nIndent;

			// 遞迴的最大次數
			int nSumTimes;

			// 最大誤差百分比
			// 若 fMaxErrorPercent=0.05, 則每次都會剃除誤差最大的前 5% 資料再進行遞迴計算
			float fMaxErrorPercent;

			// 資料縮放倍率
			float fRatio;

			SSurfaceParam()
			{
				// 預設不做擬合平面
				bCreateSurface = true;

				// 預設取內縮範圍內的Data
				bDataInside = false;

				// 預設為性線平面
				nSurfaceType = SURFACE_LINE;

				// 預設對ROI範圍內的資料 不進行任何處理
				nDataProcessMode = DATA_NONE;

				// 預設遞迴時的收斂條件為 -- 最小高度差
				nLossesMode = LOSSES_HEIGHT_MIN;

				// 預設高度低於 50um 將不進行平面擬合
				fHeightThreshold = 50.0;

				// 預設零件ROI的上 下 左 右都往內縮 20 pixels 當作取值範圍
				nIndent = 20;

				// 預設遞迴計算最大次數為10次
				nSumTimes = 10;

				// 預設取高度誤差最大的 5% 當作剔除條件
				fMaxErrorPercent = 0.05;

				// 預設將 X Y座標與高度都縮小1000倍再計算
				fRatio = 1000.0;
			}
		};

		// 平面擬合輸出結果
		struct SSurfaceResult
		{
			// 實際遞迴的計算次數
			int nTimes;

			// 擬合平面的最高點與最低點的高度差
			float fDifference_Height;

			// 擬合平面的最高點與最低點形成的斜率
			float fSlope;

			// 擬合平面的最高點與最低點形成的角度
			float fAngle;

			// 擬合平面最低點的高度
			float fZ_Lowest;

			// 擬合平面最高點的高度
			float fZ_Highest;

			// 擬合平面最低點的座標
			POINT ptLowest;

			// 擬合平面最高點的座標
			POINT ptHighest;

			SSurfaceResult()
			{
				Initial();
			}

			void Initial()
			{
				nTimes = 0;
				fDifference_Height = 0.0;
				fSlope = 0.0;
				fAngle = 0.0;
				fZ_Lowest = 0.0;
				fZ_Highest = 0.0;
				ptLowest.x = -1;
				ptLowest.y = -1;
				ptHighest.x = -1;
				ptHighest.y = -1;
			}
		};

		class JETALG_API CSurfaceFitting
		{
		private:

			// 控制是否要儲存影像
			bool					m_SaveImage;

			// 平面的 X 座標陣列
			float*					m_pfX;

			// 平面的 Y 座標陣列
			float*					m_pfY;

			// 平面的 高度陣列
			float*					m_pfZ;

			// ROI內縮後 原始3D高度的平均值
			float					m_fMeanHeight;

			// 方程式係數的個數
			int						m_nCount_Coefficient;

			// ROI 範圍內的實際會被拿來計算的資料個數 
			int						m_nCount_Data;

			// 紀錄版次
			string					m_strVersion;

			// 儲存影像的路徑與檔名
			string					m_strSavePathName;

			// 平面方程式係數
			vector<double>			m_vtdCoefficient_Finally;

			// 擬合平面的4個角點 0=LT, 1=RT, 2=RB, 3=LB
			vector<POINT>			m_vtptCorner;

			// 輸入參數
			SSurfaceParam			m_sParam;

			// 紀錄遞迴過程的誤差平均
			vector<float>			m_fMeanError;

			// 紀錄遞迴過程的擬合平面高度差
			vector<float>			m_fDiffHeight;

			// 紀錄遞迴過程的 平面方程式係數
			vector<vector<double>>	m_vtdCoefficient_Process;

		public:
			CSurfaceFitting();
			~CSurfaceFitting();

			// 取得版次
			string GetVersion()const { return m_strVersion; }

			// 執行結果判斷
			// nReturnValue = 每個函式的回傳值
			// return : true,代表函式執行成功 ; false,代表執行失敗
			bool CheckIsExecption(const int& nReturnValue) const;

			// 平面擬合  
			// ImageH : pfSrc 的高
			// ImageW : pfSrc 的寬
			// ROI : 平面範圍
			// pfSrc : 原始3D高度
			// pfDst : 擬合後的高度
			// sParam : 輸入參數
			// sResult : 輸出結果
			// return值 : -1代表執行函式失敗 , 1代表成功 
			int SurfaceFitting(const int& ImageH, const int& ImageW, const RECT& ROI, const float* pfSrc, float* pfDst, const SSurfaceParam& sParam, SSurfaceResult& sResult);

			// 20230426 依遮罩位置取點(未完成)
			int SurfaceFitting(const int& ImageH, const int& ImageW, const float* pfSrc, const unsigned char* pucMask, const SSurfaceParam& sParam, SSurfaceResult& sResult);

			// 取得平面方程式係數
			// vtdCoefficient : 方程式係數 ; 
			// 若 SSurfaceParam.nSurfaceType = TYPE_LINE => Z = a1*x + a2*y + a3
			//                                 a1 = vtdCoefficient[0] , a2 = vtdCoefficient[1] , a3=vtdCoefficient[2]
			void GetCoefficient(vector<double>& vtdCoefficient);

			// 取得擬合平面的最大高度差 傾斜角度 傾斜斜率 最高與最低點的座標
			// sResult : 輸出結果
			// return值 : -1代表執行函式失敗 , 1代表成功 
			int GetResult(SSurfaceResult& sResult);

			// 計算擬合面
			// ImageW : 影像寬度
			// ROI : 要進行平面擬合的範圍
			// pfDst : 擬合的結果
			// return值 : -1代表執行函式失敗 , 1代表成功 
			int CalculateSurface(const vector<double>& vtdCoefficient, const int& ImageH, const int& ImageW, const RECT& ROI, float* pfDst);

			// 計算擬合面_高度偵測(原始3D高度低於 m_sParam.fHeightThreshold值, 該點將不計算擬合平面)
			// ImageW : 影像寬度
			// ROI : 要進行平面擬合的範圍
			// pfSrc : 原始3D高度
			// pfDst : 擬合的結果
			// return值 : -1代表執行函式失敗 , 1代表成功 
			int CalculateSurface(const vector<double>& vtdCoefficient, const int& ImageH, const int& ImageW, const RECT& ROI, const float* pfSrc, float* pfDst);

			// 傾斜校正 : 
			// vtdCoefficient : 平面方程式的係數
			// ImageH : pfSrc 的高
			// ImageW : pfSrc 的寬
			// ROI : 平面範圍
			// pfSrc : 原始3D高度
			// pfDst : 校正後的高度
			// return值 : -1代表執行函式失敗 , 1代表成功 
			int TiltCorrection(const vector<double>& vtdCoefficient, const int& ImageH, const int& ImageW, const RECT& ROI, const float* pfSrc, float* pfDst);

			// 將平面傾斜
			// fAngle_X : X方向傾斜角度 ; 在影像坐標系中(原點在左上), 若fAngle_X為正值=>越往右高度會越高
			// fAngle_Y : Y方向傾斜角度 ; 在影像坐標系中(原點在左上), 若fAngle_Y為正值=>越往下高度會越高
			// ImageH : pfSrc 的高
			// ImageW : pfSrc 的寬
			// ROI : 平面範圍
			// pfSrc : 原始3D高度
			// pfDst : 傾斜後的高度
			// return值 : -1代表執行函式失敗 , 1代表成功 
			int TiltSurface(const float& fAngle_X, const float& fAngle_Y, const int& ImageH, const int& ImageW, const RECT& ROI, const float* pfSrc, float* pfDst);

			// 取得遞迴過程的資料
			void GetRecursionData(vector<vector<double>>& vtdCoefficient, vector<float>& vtfMeanError, vector<float>& vtfDiff_Height);

			// 設定存圖的路徑與檔名(不需要副檔名)
			int SetSavePathName(const string& strPathName);

			// 設定是否要輸出計算過程的影像
			void SetSaveImage(const bool& bSave);

		private:
			// 釋放 m_pfX, m_pfY, m_pfZ
			void ReleaseData();

			// 檢查輸入參數
			int CheckParam(const SSurfaceParam& sParam);

			// 設置計算線性平面的Data
			// ImageW : pfSrc的寬度
			// ROI :要計算的範圍
			// pfSrc : 原始3D高度
			// nCount : pfX, pfY, pfZ 的陣列大小
			// pfX : 輸出 ROI範圍的 X座標
			// pfY : 輸出 ROI範圍的 Y座標
			// pfZ : 輸出 ROI範圍的 高度
			int SetData_Line(const int& ImageH, const int& ImageW, const RECT& ROI, const int& nCount, const float* pfSrc, float* pfX, float* pfY, float* pfZ);

			// 線性平面擬合  Z = a1*x + a2*y + a3(遞迴)
			// pfX : X座標
			// pfY : Y座標
			// pfZ : 3D高度
			// nCount : pfCoordinateX, pfCoordinateY, pf3D 的數量
			// nSumTimes : 遞迴次數上限
			// nTimes : 實際遞迴次數
			int SurfaceFitting_Line(float* pfX, const float* pfY, const float* pfZ, const int& nCount, const int& nSumTimes, int& nTimes);

			// 線性平面擬合  Z = a1*x + a2*y + a3(單次)
			// pfCoordinateX : X座標
			// pfCoordinateY : Y座標
			// pf3D : 3D高度
			// nCount = pfCoordinateX, pfCoordinateY, pf3D 的數量
			// vtdCoefficient : 方程式係數
			// vtdCoefficient[0] = a1
			// vtdCoefficient[1] = a2
			// vtdCoefficient[2] = a3
			int SurfaceFitting_Line(const float* pfCoordinateX, const float* pfCoordinateY, const float* pf3D, const int& nCount, vector<double>& vtdCoefficient);

			// 計算擬合平面4個角點的高度差
			// vtdCoefficient : 方程式係數
			// nIndex_Min : 高度最低角點的 index
			// nIndex_Max : 高度最高角點的 index
			// fDiffHeight : 高度差
			int Calculate_DifferenceHeight(const vector<double>& vtdCoefficient, int& nIndex_Min, int& nIndex_Max, float& fDiffHeight);

			// 計算平面傾斜角度
			// fSlope : 傾斜斜率
			// fAngle : 傾斜角度
			int Calculate_3DSlope(const vector<double>& vtdCoefficient, float& fSlope, float& fAngle);
		};

	}
}