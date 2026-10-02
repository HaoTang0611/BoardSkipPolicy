// Light3DTiDLP_Imp.h: interface for the CLight3DTiDLP_Imp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHT3DTIDLP_IMP_H__5B3FDFC8_FE39_47B8_8568_65C7F2A64A2F__INCLUDED_)
#define AFX_LIGHT3DTIDLP_IMP_H__5B3FDFC8_FE39_47B8_8568_65C7F2A64A2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Light3DDef.h"
#ifdef LIGHT_3D_TI_DLP_IMP_USE
#include "Light3DTiDLPDef.h"
//-------------------------------------------------------------------------------------//
#define	DLP_OPERATION_PATTERN                     0	
#define	DLP_OPERATION_VIDEO                       1
#define	DLP_OPERATION_STANDBY                     2	
#define	DLP_OPERATION_PATTERN_EXP                 3
#define DLP_OPERATION_DEFAULT                     DLP_OPERATION_PATTERN_EXP
//-------------------------------------------------------------------------------------//
#define	DLP_BIT_DEPTH_1                           1
#define	DLP_BIT_DEPTH_2                           2
#define	DLP_BIT_DEPTH_3                           3
#define	DLP_BIT_DEPTH_4                           4
#define	DLP_BIT_DEPTH_5                           5
#define	DLP_BIT_DEPTH_6                           6
#define	DLP_BIT_DEPTH_7                           7
#define	DLP_BIT_DEPTH_8                           8	
//-------------------------------------------------------------------------------------//
#define DLP_PATTERN_SOURCE_FLASH                  0
#define DLP_PATTERN_SOURCE_VIDEO_PORT             1
//-------------------------------------------------------------------------------------//
#define DLP_SEQUENCE_TRIGGER_VSYNC                0
#define DLP_SEQUENCE_TRIGGER_INT_EXT              1
//-------------------------------------------------------------------------------------//
#define PZT_AXIS_A	0
#define PZT_AXIS_B	1
//-------------------------------------------------------------------------------------//
#define TRIG_BOARD_ID_01							1
#define TRIG_BOARD_ID_02							2
//-------------------------------------------------------------------------------------//
typedef struct _DLPPatItem               
{
	int  sTrigType;
	int  sFlashIndex;
	int  sColorIndex;
	int  sBitDepth;	
	int  sBitNum;//PatNum?
	int  sBitStart;
	int  sBitEnd;
	int  nPeriod;//週期
	int  nExposure;//曝光時間
	bool sInvertPattern;
	bool sInsertBlack;
	bool sTrigOutPrev;	
	bool sBufSwap;
	_DLPPatItem()
	{
		sTrigType      = DLP_LED_TRIGGER_INTERNAL;
		sFlashIndex    = 0;
		sColorIndex    = DLP_LED_COLOR_WHITE;
		sBitDepth      = DLP_BIT_DEPTH_1;
		sBitNum        = 0;//PatNum?
		sBitStart      = 0;
		sBitEnd        = 0;
		nPeriod        = 10000;
		nExposure      = 10000;
		sInvertPattern = false;
		sInsertBlack   = true;
		sTrigOutPrev   = false;	
		sBufSwap       = false;
	}
} TDLPPatItem, *PDLPPatItem;
//-------------------------------------------------------------------------------------//
typedef struct _DLPParam
{
	//DLP	TI
	int           m_LEDColor;
	int           m_TrigType;
	int           m_PatternBitDepth;
	int		      m_TrigInterval_us;	
	int		      m_PatternExp_us;	
	int           m_PhaseMode;
	
	int           m_LEDCurrentR_1;
	int           m_LEDCurrentG_1;
	int           m_LEDCurrentB_1;
	int           m_LEDCurrentR_2;
	int           m_LEDCurrentG_2;
	int           m_LEDCurrentB_2;
	bool	      m_InvertPWM;
	bool	      m_LEDEnabled_Auto;
	bool	      m_LEDEnabled_R;
	bool	      m_LEDEnabled_G;
	bool	      m_LEDEnabled_B;
	bool	      m_FlipLong;
	bool	      m_FlipShort;
	double        m_ImageGamma;
	int           m_ValidateDelayTime;//Validate後的延遲時間
	int           m_PeriodTime_us;
	int           m_ExposureTime_us;	
	int           m_PeriodTime2_us;//第2週期的週期
	int           m_ExposureTime2_us;//第2週期的曝光	
	bool          m_UseExpLut;//曝光式樣板
	double        m_PhaseFactorMin;//平面係數下限
	double        m_PhaseFactorMax;//平面係數上限
	int           m_LEDCurrentMax;//燈源電流上限	
	double        m_SecondExpRatio;//第2曝光比例
	int           m_PeriodPaddingTime;//週期外加時間-us
	int           m_ExposurePaddingTime;//曝光外加時間-us
	_DLPParam()
	{
		m_LEDColor		=	DLP_LED_COLOR_WHITE;
		m_TrigType		=	DLP_LED_TRIGGER_INTERNAL;
		m_PatternBitDepth	=	6;
		m_TrigInterval_us	=	10300;
		m_PatternExp_us	=	10000;
		m_PhaseMode  	=	LIGHT3D_PHASE_4_4_M;
	
		m_LEDCurrentR_1	=	100;
		m_LEDCurrentG_1	=	100;
		m_LEDCurrentB_1	=	100;	

		m_LEDCurrentR_2	=	100;
		m_LEDCurrentG_2	=	100;
		m_LEDCurrentB_2	=	100;	

		m_InvertPWM		=	false;
		m_LEDEnabled_Auto	=	true;
		m_LEDEnabled_R	=	true;
		m_LEDEnabled_G	=	true;
		m_LEDEnabled_B	=	true;
		m_FlipLong		=	false;
		m_FlipShort		=	false;
		m_ImageGamma    =   1.0;
		m_ValidateDelayTime = 20;
		m_PeriodTime_us = 4000;//us
		m_ExposureTime_us = 3000;//us
		m_PeriodTime2_us = 4000;//us
		m_ExposureTime2_us = 3000;//us
		m_UseExpLut = true;//曝光式樣板

		m_PhaseFactorMin = 4500;//平面係數下限
		m_PhaseFactorMax = 7500;//平面係數上限
		m_LEDCurrentMax = 180;//燈源電流上限		
		m_SecondExpRatio = 2.5;
		m_PeriodPaddingTime = 250;//週期外加時間-us
		m_ExposurePaddingTime = 0;//曝光外加時間-us
//	bool	InvertPat = false;
//	bool	InsertBlack = true;
//	bool	TrigOutPrev = false;		
	}
} TDLPParam, *PDLPParam;
//-------------------------------------------------------------------------------------//
class CLight3DTiDLP;
//-------------------------------------------------------------------------------------//
class CLight3DTiDLP_Imp  
{
public:
	//---------------------------------------------------------------------------------//	
	friend CLight3DTiDLP;
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//	
	int                        m_PreInitCount;//初始化次數
	//---------------------------------------------------------------------------------//	
	CRITICAL_SECTION           m_csLight3D;//同步化	
	int                        m_CtrlBoardID;
	LIGHT_3D_CAST_ID           m_Light3DCastID;//裝置投射編號		
	//---------------------------------------------------------------------------------//		
protected:
	//---------------------------------------------------------------------------------//
	LIGHT_3D_DEVICE_TYPE       m_Light3DDevice;//裝置控制型號
	//---------------------------------------------------------------------------------//
	unsigned int               m_DLPImageW;//DLP的DMD畫素寬
	unsigned int               m_DLPImageH;//DLP的DMD畫素長
	//---------------------------------------------------------------------------------//	
	CString                    m_CastName;
	CString                    m_ErrorString;	
	CString                    m_ErrorStringOut;	
	int                        m_OperationMode;		
	//---------------------------------------------------------------------------------//	
	TDLPParam                  m_DLPParam;	
	//---------------------------------------------------------------------------------//
	int                        m_LEDColor;	
	int                        m_PatternMode;	
	int                        m_LEDCurrentR;
	int                        m_LEDCurrentG;
	int                        m_LEDCurrentB;	
	int                        m_LEDCurrentMax;//燈源電流上限	
	bool                       m_InvertPWM;
	bool	                   m_LEDEnabled_R;
	bool	                   m_LEDEnabled_G;
	bool	                   m_LEDEnabled_B;
	bool	                   m_LEDEnabled_Auto;
	double                     m_ImageGamma;
	double                     m_SecondExpRatio;
	//---------------------------------------------------------------------------------//
	DWORD                      m_DLPDelayTime;
	size_t                     m_TrigOutCount;
	std::vector<TDLPPatItem>   m_PatternList;//DLP樣版列表
	unsigned int               m_PatternSetCountInDLP;//DLP樣板設定數量
	//---------------------------------------------------------------------------------//
	unsigned int               m_Exposure_us;
	unsigned int               m_TrigPeriod_us;	
	unsigned int               m_Exposure2_us;
	unsigned int               m_TrigPeriod2_us;		
	//---------------------------------------------------------------------------------//
	char                       m_DLPFrmTag[64];	
	char                       m_TiAPIversion[256];
	char                       m_DLPFrmversion[256];
	DWORD                      m_dwFrmVersion;
	//---------------------------------------------------------------------------------//		
	IMAGE_SIZE                 m_PhaseZeroW;//平面相位寬度
	IMAGE_SIZE                 m_PhaseZeroH;//平面相位高度
	IMAGE_SIZE                 m_PhaseZeroStep;//平面相位步長
	PHASE_PTR                  m_PhaseZeroPtr;//平面相位指標	
	int                        m_PhaseZeroLEDColor;
	int                        m_PhaseZeroPhaseMode;//相位模式-基準平面	
	//---------------------------------------------------------------------------------//	
	IMAGE_SIZE                 m_PhaseFactorW;//平面係數寬度
	IMAGE_SIZE                 m_PhaseFactorH;//平面係數高度
	IMAGE_SIZE                 m_PhaseFactorStep;//平面係數步長
	SPACE_PTR                  m_PhaseFactorPtr;//平面係數指標
	int                        m_PhaseFactorLEDColor;
	int                        m_PhaseFactorPhaseMode;//相位模式-高度比值	
	//---------------------------------------------------------------------------------//
	double                     m_HeightFactor0[HEIGHT_FACTOR_PARAM_COUNT];//高度係數T0
	double                     m_HeightFactor1[HEIGHT_FACTOR_PARAM_COUNT];//高度係數T1
	double                     m_HeightFactor2[HEIGHT_FACTOR_PARAM_COUNT];//高度係數T2
	std::vector<TPhaseFactorTable> m_FactorTableList;//相位系數格點列表		
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	CLight3DTiDLP_Imp(const CLight3DTiDLP_Imp &DlpCtrl);
	CLight3DTiDLP_Imp& operator=(const CLight3DTiDLP_Imp &DlpCtrl);
	//---------------------------------------------------------------------------------//
	virtual void               PreInitTiDlp(int CtrlID, LIGHT_3D_CAST_ID CastID);
	virtual void               InitialTiDlp();
	//---------------------------------------------------------------------------------//
	void                       LockLight3D();
	void                       UnlockLight3D();
	//---------------------------------------------------------------------------------//
	void                       SetDLPImageW(unsigned int val);	
	void                       SetDLPImageH(unsigned int val);	
	//---------------------------------------------------------------------------------//
	void                       SetDeviceType(LIGHT_3D_DEVICE_TYPE val);		
	//---------------------------------------------------------------------------------//
	int                        ReturnPatternIndex_Bit() const;//回傳樣板引數-位元
	bool                       ReturnNotImplement(LPCTSTR fnName);//回傳未完成函式	
	//---------------------------------------------------------------------------------//	
	virtual void               SetLCRErrorFnName(LPCTSTR LCRFnName);	
	//---------------------------------------------------------------------------------//		
	virtual bool               GetUSB_Number(LIGHT_3D_CAST_ID CastID, wchar_t USB_Number[]);
	//---------------------------------------------------------------------------------//
	int                        GetDLPSafeCurrent(int value) const;	
	//---------------------------------------------------------------------------------//
	bool                       SaveDLPCurrentProcess(LPCTSTR pContext, bool bShowMsg);//儲存現在狀態
	//---------------------------------------------------------------------------------//
	virtual bool               ClearPhaseZeroBufferFn();//清除平面相位	
	virtual bool               ClearPhaseFactorBufferFn();//清除平面係數
	//---------------------------------------------------------------------------------//
	virtual int                GetDLPTrigType(int index, bool IntTrig, bool MultiTable);
	//---------------------------------------------------------------------------------//
	virtual bool               SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	virtual bool               LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//	
	virtual bool               LoadDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE &PhaseW, IMAGE_SIZE &PhaseH, IMAGE_SIZE &PhaseStep, PHASE_PTR &PhasePtr);//載入DLP平面相位
	virtual bool               SaveDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr);//儲存DLP平面相位
	//---------------------------------------------------------------------------------//	
	virtual bool               LoadDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE &SpaceW, IMAGE_SIZE &SpaceH, IMAGE_SIZE &SpaceStep, SPACE_PTR &SpacePtr);//載入DLP平面係數
	virtual bool               SaveDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr);//儲存DLP平面係數
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	CLight3DTiDLP_Imp();
	CLight3DTiDLP_Imp(int CtrlID, LIGHT_3D_CAST_ID CastID);
	virtual ~CLight3DTiDLP_Imp();
	//---------------------------------------------------------------------------------//	
	void                       SetDLPID(int ID);
	int                        GetDLPID() const;
	//---------------------------------------------------------------------------------//	
	LIGHT_3D_CAST_ID           GetCastID() const;
	LIGHT_3D_DEVICE_TYPE       GetDeviceType() const;
	//---------------------------------------------------------------------------------//		
	LPCTSTR                    GetDLPProjectName() const;
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString();
	void                       SetAOIExceptionCode(DWORD Code);
	//---------------------------------------------------------------------------------//
	unsigned int               GetDLPImageW() const;
	unsigned int               GetDLPImageH() const;
	//---------------------------------------------------------------------------------//
	virtual bool               DLPConnect();
	virtual bool               DLPDisconnect(int WaitTime_ms=0);
	virtual bool               GetDLPIsConnected();	
	virtual bool               CheckDLPFrmForExpLut();//確認DLP韌體支援Exposure Lut
	//---------------------------------------------------------------------------------//
	virtual bool               SaveDLPProcess(LPCTSTR fnName, int Level);
	//---------------------------------------------------------------------------------//
	virtual bool               ExecDLPSoftwareReset(DWORD delayTime);
	//---------------------------------------------------------------------------------//
	virtual bool               SetDLPLongAxisImageFlip(bool Flip);
	virtual bool               GetDLPLongAxisImageFlip();
	//---------------------------------------------------------------------------------//
	virtual bool               SetDLPShortAxisImageFlip(bool Flip);
	virtual bool               GetDLPShortAxisImageFlip();
	//---------------------------------------------------------------------------------//
	virtual bool               SetDLPOperationMode(int Mode);	
	virtual int                GetDLPOperationMode() const;	
	//---------------------------------------------------------------------------------//
	virtual bool               SetDLPLEDEnable(bool bSeqCtrl, bool bRed, bool bGreen , bool bBlue);
	virtual bool               GetDLPLEDEnable(bool &bSeqCtrl, bool &bRed, bool &bGreen , bool &bBlue);
	//---------------------------------------------------------------------------------//	
	virtual bool               SetDLPLEDCurrent(int red, int green, int blue, bool Update, int CurrentID);
	virtual bool               GetDLPLEDCurrent(int &red, int &green, int &blue);	
	//---------------------------------------------------------------------------------//	
	virtual bool               SetDLPLEDPWMInvert(bool bInvert);
	virtual bool               GetDLPLEDPWMInvert(bool &bInvert);
	//---------------------------------------------------------------------------------//
	virtual bool               CheckDLPStatus();
	virtual bool               GetDLPStatus_InitDone();
	virtual bool               GetDLPStatus_ForcedSwap();
	virtual bool               GetDLPStatus_BufferFreeze();
	virtual bool               GetDLPStatus_SeqRunning();
	virtual bool               GetDLPStatus_SeqError();
	virtual bool               GetDLPStatus_SeqAbort();
	virtual bool               GetDLPStatus_DRCError();
	virtual bool               GetDLPStatus_DMDParked();	
	//---------------------------------------------------------------------------------//
	virtual bool               InitialDLPParameter(TDLPParam &Param);
	virtual bool               SaveDLPParameter();//寫參數至檔案
	virtual bool               LoadDLPParameter();//從檔案讀取參數
	virtual bool               ReadDLPParameter();//從DLP裝置讀取參數
	TDLPParam&                 GetDLPParam();
	const TDLPParam&           GetDLPParam() const;
	virtual bool               SetDLPPhaseMode(int Mode);
	virtual int                GetDLPPhaseMode() const;
	virtual int                GetDLPExposureTime() const;
	virtual bool               GetDLPReadySignalEnable() const;
	//---------------------------------------------------------------------------------//
	virtual bool               SetDLPParamLEDCurrent(int red, int green, int blue, int CurrentID);
	virtual bool               GetDLPParamLEDCurrent(int &red, int &green, int &blue, int CurrentID);
	//---------------------------------------------------------------------------------//
	const char*                GetDLPFrmTag();
	const char*                GetTiAPIVersion();	
	const char*                GetDLPFrmVersion();	
	const char*                GetDLPMcuVersion();
	//---------------------------------------------------------------------------------//
	virtual bool               GetUseExpLut();
	virtual bool               SetLEDColor(int Type);
	virtual int                GetLEDColor() const;	
	//---------------------------------------------------------------------------------//
	virtual double             GetImageGamma() const;
	virtual bool               SetImageGamma(double val);
	//---------------------------------------------------------------------------------//
	virtual double             GetSecondExpRatio() const;//取得第2次曝光比例
	virtual bool               SetSecondExpRatio(double val);
	//---------------------------------------------------------------------------------//
	int                        GetPeriodPaddingTime() const;//週期外加時間-us
	int                        GetExposurePaddingTime() const;//曝光外加時間-us
	virtual int                CalcPeriodPaddingTime(int ExpTime) const;//計算週期外加時間-us
	//---------------------------------------------------------------------------------//
	virtual bool               GetUse3BitPattern() const;//取得使用3Bit樣板圖
	virtual bool               GetUse5BitPattern() const;//取得使用5Bit樣板圖
	//---------------------------------------------------------------------------------//	
	virtual int                GetPatternBitCount() const;//取得樣板圖位元數
	virtual void               SetPatternBitCount(int val);//設定樣板圖位元數
	//---------------------------------------------------------------------------------//
	virtual int                GetPatternIndex1_3Bit() const;//取得使用3Bit樣板引數-1
	virtual void               SetPatternIndex1_3Bit(int val);//設定使用3Bit樣板引數-1
	virtual int                GetPatternIndex2_3Bit() const;//取得使用3Bit樣板引數-2
	virtual void               SetPatternIndex2_3Bit(int val);//設定使用3Bit樣板引數-2
	virtual int                GetPatternIndex1_5Bit() const;//取得使用5Bit樣板引數-1
	virtual void               SetPatternIndex1_5Bit(int val);//設定使用5Bit樣板引數-1
	virtual int                GetPatternIndex1_6Bit() const;//取得使用6Bit樣板引數-1
	virtual void               SetPatternIndex1_6Bit(int val);//設定使用6Bit樣板引數-1
	virtual int                GetPatternIndex2_6Bit() const;//取得使用6Bit樣板引數-2
	virtual void               SetPatternIndex2_6Bit(int val);//設定使用6Bit樣板引數-2
	virtual int                GetPatternIndexGC_1Bit() const;//取得使用1Bit-GrayCode引數-1
	virtual void               SetPatternIndexGC_1Bit(int val);//設定使用1Bit-GrayCode引數-1
	virtual int                GetPatternIndexBC_1Bit() const;//取得使用1Bit-BinaryCode引數-1
	virtual void               SetPatternIndexBC_1Bit(int val);//設定使用1Bit-BinaryCode引數-1
	virtual int                GetPatternStartNumGC_1Bit() const;//取得使用1Bit-GrayCode起始張數-1
	virtual void               SetPatternStartNumGC_1Bit(int val);//設定使用1Bit-GrayCode起始張數-1
	virtual int                GetPatternStartNumBC_1Bit() const;//取得使用1Bit-BinaryCode起始張數-1
	virtual void               SetPatternStartNumBC_1Bit(int val);//設定使用1Bit-BinaryCode起始張數-1
	//---------------------------------------------------------------------------------//
	virtual void               SetPeriod_us(unsigned int val);
	virtual void               SetExposure_us(unsigned int val);	
	virtual void               SetPeriod2_us(unsigned int val);
	virtual void               SetExposure2_us(unsigned int val);
	//---------------------------------------------------------------------------------//
	virtual void               InitialPatItem(TDLPPatItem &PatItem);		
	//---------------------------------------------------------------------------------//
	virtual bool               ExecDLPPattern_Run();	
	virtual bool               ExecDLPPattern_Stop();
	virtual bool               ExecDLPPattern_Pause();
	virtual bool               LEDSetting(int LEDCurrent=-1, int CurrentID=DLP_LED_CURRENT_ID_01);	
	size_t                     GetTriggerOutCount();
	//---------------------------------------------------------------------------------//
	size_t                     GetDLPPatCount();
	TDLPPatItem*               GetDLPPatItemPtr(size_t index, bool check);	
	virtual void               ClearDLPPatternList();//清除m_PatternList	
	virtual bool               BuildDLPPatternList(int Mode, bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表 
	virtual bool               AddDLPPPatItem(TDLPPatItem &PatItem);//增加樣板項目	
	virtual bool               RemoveDLPPatItem(size_t index);//移除樣板項目
	//---------------------------------------------------------------------------------//
	virtual bool               ExecDLPPatClear();//清除樣版內容
	virtual bool               ExecDLPPatRead(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat);//讀取DLP樣版內容
	virtual bool               ExecDLPPatSendAll(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新樣板至DLP	
	virtual bool               ExecDLPPatSendOne(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新單一樣板至DLP
	virtual bool               ExecDLPValidatePatLutData(unsigned int &Status, bool Sleep);//套用樣板列表資料
	virtual bool               ExecDLPPatBuildSendValidate(int Mode, bool IntTrig, bool MultiTable, unsigned int Periodus, unsigned int exposure_us, int LEDColor, bool Sleep, bool Force);//建立樣板, 傳送樣板以及驗證

	virtual bool               ExecDLPLightSetting(int CurrentID);//執行DLP的LED設定-依據目前的設定
	//---------------------------------------------------------------------------------//
	virtual bool               BuildPatternImage(int BitDepth, int NPeriod, int NPixelPeriod, bool bVer, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage);
	//---------------------------------------------------------------------------------//	
	virtual CString            GetDLPPhaseModeTextForBinFile(int Mode);//取得相位模式的文字
	//---------------------------------------------------------------------------------//
	virtual bool               CheckPhaseZeroRed();//確認平面相位
	virtual bool               CheckPhaseZeroGrn();//確認平面相位
	virtual bool               CheckPhaseZeroBlu();//確認平面相位
	virtual bool               CheckPhaseZeroWhite();//確認平面相位
	virtual bool               CheckPhaseZeroDebug();//確認平面相位
	//---------------------------------------------------------------------------------//	
	virtual bool               LoadPhaseZero();//載入平面相位
	virtual bool               LoadPhaseZeroRed();//載入平面相位
	virtual bool               LoadPhaseZeroGrn();//載入平面相位
	virtual bool               LoadPhaseZeroBlu();//載入平面相位
	virtual bool               LoadPhaseZeroWhite();//載入平面相位
	virtual bool               LoadPhaseZeroDebug();//載入平面相位
	//---------------------------------------------------------------------------------//
	virtual bool               SavePhaseZero();//儲存平面相位
	virtual bool               SavePhaseZeroRed();//儲存平面相位
	virtual bool               SavePhaseZeroGrn();//儲存平面相位
	virtual bool               SavePhaseZeroBlu();//儲存平面相位
	virtual bool               SavePhaseZeroWhite();//儲存平面相位
	virtual bool               SavePhaseZeroDebug();//儲存平面相位
	//---------------------------------------------------------------------------------//
	virtual bool               ClearPhaseZeroBuffer();//清除平面相位	
	virtual bool               ClearPhaseZeroBufferRed();//清除平面相位	
	virtual bool               ClearPhaseZeroBufferGrn();//清除平面相位	
	virtual bool               ClearPhaseZeroBufferBlu();//清除平面相位	
	virtual bool               ClearPhaseZeroBufferWhite();//清除平面相位	
	virtual bool               ClearPhaseZeroBufferDebug();//清除平面相位		
	virtual bool               SetPhaseZero(int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, PHASE_PTR Ptr);//設定平面相位
	virtual bool               GetPhaseZero(int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr);//取得平面相位	
	virtual bool               ClonePhaseZero(int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr);//複製平面相位
	//---------------------------------------------------------------------------------//	
	virtual bool               CheckPhaseFactorRed();//確認平面係數
	virtual bool               CheckPhaseFactorGrn();//確認平面係數
	virtual bool               CheckPhaseFactorBlu();//確認平面係數
	virtual bool               CheckPhaseFactorWhite();//確認平面係數
	virtual bool               CheckPhaseFactorDebug();//確認平面係數
	//---------------------------------------------------------------------------------//	
	virtual bool               LoadPhaseFactor();//載入平面係數
	virtual bool               LoadPhaseFactorRed();//載入平面係數
	virtual bool               LoadPhaseFactorGrn();//載入平面係數
	virtual bool               LoadPhaseFactorBlu();//載入平面係數
	virtual bool               LoadPhaseFactorWhite();//載入平面係數
	virtual bool               LoadPhaseFactorDebug();//載入平面係數
	//---------------------------------------------------------------------------------//	
	virtual bool               SavePhaseFactor();//儲存平面係數
	virtual bool               SavePhaseFactorRed();//儲存平面係數
	virtual bool               SavePhaseFactorGrn();//儲存平面係數
	virtual bool               SavePhaseFactorBlu();//儲存平面係數
	virtual bool               SavePhaseFactorWhite();//儲存平面係數
	virtual bool               SavePhaseFactorDebug();//儲存平面係數
	//---------------------------------------------------------------------------------//	
	virtual bool               ClearPhaseFactorBuffer();//清除平面係數
	virtual bool               ClearPhaseFactorBufferRed();//清除平面係數
	virtual bool               ClearPhaseFactorBufferGrn();//清除平面係數
	virtual bool               ClearPhaseFactorBufferBlu();//清除平面係數
	virtual bool               ClearPhaseFactorBufferWhite();//清除平面係數
	virtual bool               ClearPhaseFactorBufferDebug();//清除平面係數
	//---------------------------------------------------------------------------------//		
	virtual double             GetPhaseFactorMin() const;//取得平面係數下限
	virtual void               SetPhaseFactorMin(double val);//設定平面係數下限
	virtual double             GetPhaseFactorMax() const;//取得平面係數上限
	virtual void               SetPhaseFactorMax(double val);//設定平面係數上限
	//---------------------------------------------------------------------------------//	
	virtual int                GetLEDCurrentMax() const;//取得LED電流上限
	virtual int                GetDLPCurrentMax() const;//取得DLP電流上限
	virtual bool               GetLEDColorUsed_Red() const;//取得LED顏色使用-紅色
	virtual bool               GetLEDColorUsed_Grn() const;//取得LED顏色使用-綠色
	virtual bool               GetLEDColorUsed_Blu() const;//取得LED顏色使用-藍色
	//---------------------------------------------------------------------------------//	
	virtual void               ClearHeightFactor();//清除高度參數T0, T1, T3	
	virtual bool               SortHeightFactorTableList();//排序高度係數列表
	virtual bool               SaveHeightFactorTableList();//儲存高度係數列表檔案
	virtual bool               LoadHeightFactorTableList();//載入高度係數列表檔案
	virtual bool               RestoreHeightFactorTableList();//復原高度係數列表
	virtual void               ClearHeightFactorTableList(bool bIncludeFiles);//清除高度係數列表
	virtual bool               BuildHeightFactorMappingParam(bool bRecv);//建立高度參數T0, T1, T3	
	virtual bool               CalcHeightFactorMappingParam(std::vector<std::vector<TPhaseFactorGrid>> &GridListArray, std::vector<std::vector<double>> &ParamListArray);//計算高度參數T0, T1, T3
	virtual bool               VerifyHeightFactorMappingParam(const std::vector<std::vector<double>> &ParamListArray, std::vector<TPhaseFactorGrid> &GridList, double &Error);//驗證高度參數T0, T1, T3	
	virtual bool               GetHeightFactorMappingParam(double T0[], double T1[], double T2[]);//取得高度參數	
	//---------------------------------------------------------------------------------//		
	virtual bool               SetPhaseFactor(int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, SPACE_PTR Ptr);//設定平面係數
	virtual bool               GetPhaseFactor(int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr);//取得平面係數
	virtual bool               ClonePhaseFactor(int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr);//複製平面係數
	//---------------------------------------------------------------------------------//	
	virtual bool               SetHeightFactorTable(int TargetNo, const TPhaseFactorTable &GridTable);//設定高度係數格點列表		
	virtual bool               CloneHeightFactorTable(int TargetNo, TPhaseFactorTable &GridTable) const;//複製高度係數格點列表	
	//---------------------------------------------------------------------------------//	
	virtual void               DLPSettingDelay() const;//DLP設定時要先延遲一段時間
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif//LIGHT_3D_TI_DLP_IMP_USE
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_LIGHT3DTIDLP_IMP_H__5B3FDFC8_FE39_47B8_8568_65C7F2A64A2F__INCLUDED_)
