// Light3DTiDLP4500.h: interface for the CLight3DTiDLP4500 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHT3DTIDLP4500_H__091F6483_6B4F_4505_A49B_46CC67AA8BE4__INCLUDED_)
#define AFX_LIGHT3DTIDLP4500_H__091F6483_6B4F_4505_A49B_46CC67AA8BE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Light3DDef.h"
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_4500_USE
//-------------------------------------------------------------------------------------//
#include "Light3DTiDLPDef.h"
#include "Light3DTiDLP4500Usb.h"
#include "Light3DTiDLP4500Frmw.h"
//-------------------------------------------------------------------------------------//
#define STAT_BIT_FLASH_BUSY     BIT3
#define HID_MESSAGE_MAX_SIZE    512
//-------------------------------------------------------------------------------------//
typedef struct _hidmessageStruct
{
    struct _hidhead
    {
        struct _packetcontrolStruct
        {
            unsigned char dest		:3; /* 0 - ProjCtrl; 1 - RFC; 7 - Debugmsg */
            unsigned char reserved	:2;
            unsigned char nack		:1; /* Command Handler Error */
            unsigned char reply	:1; /* Host wants a reply from device */
            unsigned char rw		:1; /* Write = 0; Read = 1 */
        }flags;
        unsigned char seq;
        unsigned short length;
    }head;
    union
    {
        unsigned short cmd;
        unsigned char data[HID_MESSAGE_MAX_SIZE];
    }text;
}hidMessageStruct;
//-------------------------------------------------------------------------------------//
typedef struct _readCmdData
{
    unsigned char CMD2;
    unsigned char CMD3;
    unsigned short len;
}CmdFormat;
//-------------------------------------------------------------------------------------//
typedef struct _rectangle
{
    unsigned short firstPixel;
    unsigned short firstLine;
    unsigned short pixelsPerLine;
    unsigned short linesPerFrame;
}rectangle2;
//-------------------------------------------------------------------------------------//
typedef struct _vidSigStatus
{
    unsigned char Status;
    unsigned int HRes;
    unsigned int VRes;
    unsigned char RSVD;
    unsigned char HSyncPol;
    unsigned char VSyncPol;
    unsigned long int PixClock;
    unsigned int HFreq;
    unsigned int VFreq;
    unsigned int TotPixPerLine;
    unsigned int TotLinPerFrame;
    unsigned int ActvPixPerLine;
    unsigned int ActvLinePerFrame;
    unsigned int FirstActvPix;
    unsigned int FirstActvLine;
}VideoSigStatus;
//-------------------------------------------------------------------------------------//
typedef enum
{
    VID_SIG_STAT,
    SOURCE_SEL,
    PIXEL_FORMAT,
    CLK_SEL,
    CHANNEL_SWAP,
    FPD_MODE,
    CURTAIN_COLOR,
    POWER_CONTROL,
    FLIP_LONG,
    FLIP_SHORT,
    TPG_SEL,
    PWM_INVERT,
    LED_ENABLE,
    GET_VERSION,
    GET_FIRMWAE_TAG_INFO,
    SW_RESET,
    DMD_PARK,
    BUFFER_FREEZE,
    STATUS_HW,
    STATUS_SYS,
    STATUS_MAIN,
    CSC_DATA,
    GAMMA_CTL,
    BC_CTL,
    PWM_ENABLE,
    PWM_SETUP,
    PWM_CAPTURE_CONFIG,
    GPIO_CONFIG,
    LED_CURRENT,
    DISP_CONFIG,
    TEMP_CONFIG,
    TEMP_READ,
    MEM_CONTROL,
    I2C_CONTROL,
    LUT_VALID,
    DISP_MODE,
    TRIG_OUT1_CTL,
    TRIG_OUT2_CTL,
    RED_STROBE_DLY,
    GRN_STROBE_DLY,
    BLU_STROBE_DLY,
    PAT_DISP_MODE,
    PAT_TRIG_MODE,
    PAT_START_STOP,
    BUFFER_SWAP,
    BUFFER_WR_DISABLE,
    CURRENT_RD_BUFFER,
    PAT_EXPO_PRD,
    INVERT_DATA,
    PAT_CONFIG,
    MBOX_ADDRESS,
    MBOX_CONTROL,
    MBOX_DATA,
    TRIG_IN1_DELAY,
    TRIG_IN2_CONTROL,
    IMAGE_LOAD,
    IMAGE_LOAD_TIMING,
    I2C0_CTRL,
    MBOX_EXP_DATA,
    MBOX_EXP_ADDRESS,
    EXP_PAT_CONFIG,
    NUM_IMAGE_IN_FLASH,
    I2C0_STAT,
    GPCLK_CONFIG,
    PULSE_GPIO_23,
    ENABLE_DLPC350_DEBUG,
    TPG_COLOR,
    PWM_CAPTURE_READ,
    PROG_MODE,
    BL_STATUS,
    BL_SPL_MODE,
    BL_GET_MANID,
    BL_GET_DEVID,
    BL_GET_CHKSUM,
    BL_SET_SECTADDR,
    BL_SECT_ERASE,
    BL_SET_DNLDSIZE,
    BL_DNLD_DATA,
    BL_FLASH_TYPE,
    BL_CALC_CHKSUM,
    BL_PROG_MODE
}DLPC350_CMD;
//-------------------------------------------------------------------------------------//
#ifndef VERSION_H
#define VERSION_H
//-------------------------------------------------------------------------------------//
#define GUI_VERSION_MAJOR 3
#define GUI_VERSION_MINOR 1
#define GUI_VERSION_BUILD 0
//-------------------------------------------------------------------------------------//
/* Version history
* 3.1.0 -
*         . Bug-fix:
*           . Play Once functionality not working correctly in Pattern display
*             mode
*           . Firmware download fails when attempting to download large binary
*             files
*           . GUI doesn't accept updated details from the FlashDeviceParameters.txt
*             file
*         . New feature: Fast flash update feature added in this new feature
*           GUI slectively erase and program the sectors which are changed between
*           previously programmed flash binary to the new flash binary file.
*           For this GUI stores copy of the last successful programmed binary as
*           cache file on the PC.
*
* 3.0.1 -
*         . Bug-fix: If the firmware file > 8MB the fimrware upload fails
*
* 3.0.0 -
*         . Variable Exposure Pattern Configuration support added
*         . Firmware Tagging feature added under Build Frimware option
*         . Firmware Tag Info displayed in the GUI
*         . Added option to select RGB Color or Single illumination system
*         . Fixed Issue in Create Images option for 5-bit and 6-bit pattern
*           stiching
*         . Fixed issue in the GUI related to storing images when RLE Compression
*           applied
*         . InvertPattern Option added while pattern selection
*         . Option added to read incoming Video singal info
*         . TabWidgets arranged in more meaningful way
*         . Source code is formatted with more meaninful variables and
*           functions name

* 2.0.0 - Added support for DLPR350PROM v2.0.0

* 1.2.0 - Added Mac and Linux support; GUI Layout improvements

* 1.1.0 - Added support for internal pattern storage & 2nd flash; Check for v1.0.1;
Added Peripherals tab; Layout and nomenclature improvements

* 1.0.1 - Added capability to download firmware

* 1.0.0 - Initial release
*
*/
#endif //VERSION_H
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
#define DLP_BIN_VERSION_2_0_0                       0x00200000
#define DLP_BIN_VERSION_3_0_0                       0x00300000
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
class CLight3DTiDLP4500  
{
public:		
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//
	CLight3DTiDLP4500Usb       m_TiUSB;//Ti-USB連線
	CLight3DTiDLP4500Frmw      m_TiFrmw;//Ti-韌體
	CRITICAL_SECTION           m_csLight3D;//同步化	
	int                        m_CtrlBoardID;
	LIGHT_3D_CAST_ID           m_Light3DCastID;//裝置投射編號
	LIGHT_3D_DEVICE_TYPE       m_Light3DDevice;//裝置控制型號

	CString                    m_CastName;
	CString                    m_ErrorString;	
	CString                    m_ErrorStringOut;
	size_t                     m_TrigOutCount;
	int                        m_OperationMode;			
	//---------------------------------------------------------------------------------//		
	unsigned char              m_seqNum;
	unsigned int               m_numImgInFlash;

	unsigned int               m_PatLutIndex;	
	unsigned long int          m_PatLut[MAX_PAT_LUT_ENTRIES];

	unsigned int               m_ExpLutIndex;
	unsigned long int          m_ExpLut[MAX_VAR_EXP_PAT_LUT_ENTRIES*3];
	//---------------------------------------------------------------------------------//		
	int                        m_numExtraSplashLutEntries;
	int                        m_ExtraSplashLutEntries[64];
	//---------------------------------------------------------------------------------//	
	char                       m_DLPFrmTag[64];
	char                       m_TiAPIversion[256];
	char                       m_DLPFrmversion[256];
	char                       m_DLPMcuversion[256];
	DWORD                      m_dwFrmVersion;
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
	int                        m_PatternBitCount;//樣板的位元數
	unsigned int               m_Exposure_us;
	unsigned int               m_TrigPeriod_us;	
	unsigned int               m_Exposure2_us;
	unsigned int               m_TrigPeriod2_us;	
	DWORD                      m_DLPDelayTime;	
	std::vector<TDLPPatItem>   m_PatternList;//DLP樣版列表
	bool                       m_EnableTemperatureMonitor;
	//---------------------------------------------------------------------------------//
	int                        m_PatternIndex1_3Bit;
	int                        m_PatternIndex2_3Bit;
	int                        m_PatternIndex1_5Bit;
	int                        m_PatternIndex1_6Bit;
	int                        m_PatternIndex2_6Bit;
	int                        m_PatternIndex1_8Bit;
	int                        m_PatternIndex2_8Bit;
	int                        m_PatternIndexGC_1Bit;//GrayCode	
	int                        m_PatternIndexBC_1Bit;//BinaryCode	
	int                        m_PatternStartNumGC_1Bit;//GrayCode
	int                        m_PatternStartNumBC_1Bit;//BinaryCode
	int                        m_PatternIndexGC_8Bit;//GrayCode	
	int                        m_PatternIndexBC_8Bit;//BinaryCode	
	int                        m_PatternStartNumGC_8Bit;//GrayCode
	int                        m_PatternStartNumBC_8Bit;//BinaryCode
	//---------------------------------------------------------------------------------//
	unsigned char              m_HWStatus;///硬體狀態
	unsigned char              m_SysStatus;//系統狀態
	unsigned char              m_MainStatus;//主要狀態
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
protected:
	//---------------------------------------------------------------------------------//	
	CLight3DTiDLP4500(const CLight3DTiDLP4500 &DlpCtrl);
	CLight3DTiDLP4500& operator=(const CLight3DTiDLP4500 &DlpCtrl);
	//---------------------------------------------------------------------------------//
	void                          PreInitTiDlp(int CtrlID, LIGHT_3D_CAST_ID Light3DID);
	void                          InitialTiDlp();
	//---------------------------------------------------------------------------------//	
	void                          LockLight3D();
	void                          UnlockLight3D();
	//---------------------------------------------------------------------------------//
	CString                       GetDLPErrorKeyName() const;
	//---------------------------------------------------------------------------------//
	void                          SetLCRErrorFnName(LPCTSTR LCRFnName);	
	//---------------------------------------------------------------------------------//	
	bool                          GetUSB_Number(LIGHT_3D_CAST_ID CastID, wchar_t USB_Number[]);
	//---------------------------------------------------------------------------------//
	unsigned char                 GetDLPSafeCurrent(int value);	
	//---------------------------------------------------------------------------------//	
	bool                          SaveDLPCurrentProcess(LPCTSTR pContext, bool bShowMsg);//儲存現在狀態
	//---------------------------------------------------------------------------------//
	int                           GetDLPTrigType(int index, bool IntTrig, bool MultiTable);
	//---------------------------------------------------------------------------------//	
	bool                          SaveDeviceType();	
	//---------------------------------------------------------------------------------//
	bool                          SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                          LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//
	bool                          LoadDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE &PhaseW, IMAGE_SIZE &PhaseH, IMAGE_SIZE &PhaseStep, PHASE_PTR &PhasePtr);//載入DLP平面相位	
	bool                          LoadDLPPhaseZeroFile2(int LedClr, LPCTSTR pfilename, IMAGE_SIZE &PhaseW, IMAGE_SIZE &PhaseH, IMAGE_SIZE &PhaseStep, PHASE_PTR &PhasePtr);//載入DLP平面相位
	//---------------------------------------------------------------------------------//	
	bool                          LoadDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE &SpaceW, IMAGE_SIZE &SpaceH, IMAGE_SIZE &SpaceStep, SPACE_PTR &SpacePtr);//載入DLP平面係數	
	bool                          LoadDLPPhaseFactorFile2(int LedClr, LPCTSTR pfilename, IMAGE_SIZE &SpaceW, IMAGE_SIZE &SpaceH, IMAGE_SIZE &SpaceStep, SPACE_PTR &SpacePtr);//載入DLP平面係數
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//
	CLight3DTiDLP4500();
	CLight3DTiDLP4500(int CtrlID, LIGHT_3D_CAST_ID Light3DID);
	virtual ~CLight3DTiDLP4500();
	//---------------------------------------------------------------------------------//
	void                          SetDLPID(int ID);
	int                           GetDLPID() const;
	//---------------------------------------------------------------------------------//
	LIGHT_3D_CAST_ID              GetCastID() const;
	LIGHT_3D_DEVICE_TYPE          GetDeviceType() const;
	bool                          ChangeDeviceType(LIGHT_3D_DEVICE_TYPE Type);//變更裝置型號
	//---------------------------------------------------------------------------------//
	LPCTSTR                       GetDLPProjectName() const;
	//---------------------------------------------------------------------------------//
	LPCTSTR                       GetErrorString();
	void                          SetDLPExceptionCode(DWORD Code, LPCTSTR Err=NULL);
	//---------------------------------------------------------------------------------//
	unsigned int                  GetDLPImageW() const;
	unsigned int                  GetDLPImageH() const;
	//---------------------------------------------------------------------------------//
	bool                          DLPConnect();
	bool                          DLPConnectFn();
	bool                          DLPDisconnect(int WaitTime_ms=0);
	bool                          DLPDisconnectFn(int WaitTime_ms=0);
	bool                          GetDLPIsConnected();	
	bool                          CheckDLPIsConnected();
	bool                          CheckDLPFrmForExpLut();//確認DLP韌體支援Exposure Lut
	//---------------------------------------------------------------------------------//
	bool                          SaveDLPProcess(LPCTSTR fnName, int Level);
	//---------------------------------------------------------------------------------//
	bool                          ExecDLPSoftwareReset(DWORD delayTime);
	bool                          ExecDLPSoftwareResetFn(DWORD delayTime);
	//---------------------------------------------------------------------------------//
	bool                          SetDLPLongAxisImageFlip(bool Flip);
	bool                          GetDLPLongAxisImageFlip();
	//---------------------------------------------------------------------------------//
	bool                          SetDLPShortAxisImageFlip(bool Flip);
	bool                          GetDLPShortAxisImageFlip();
	//---------------------------------------------------------------------------------//
	bool                          SetDLPOperationMode(int Mode);	
	int                           GetDLPOperationMode() const;	
	//---------------------------------------------------------------------------------//
	bool                          ReleaseDLP_I2C();
	//---------------------------------------------------------------------------------//
	bool                          SetDLPLEDEnable(bool bSeqCtrl, bool bRed, bool bGreen , bool bBlue);
	bool                          GetDLPLEDEnable(bool &bSeqCtrl, bool &bRed, bool &bGreen , bool &bBlue);
	//---------------------------------------------------------------------------------//	
	bool                          SetDLPLEDCurrent(int red, int green, int blue, bool Update, int CurrentID);
	bool                          GetDLPLEDCurrent(int &red, int &green, int &blue);	
	//---------------------------------------------------------------------------------//	
	bool                          SetDLPLEDPWMInvert(bool bInvert);
	bool                          GetDLPLEDPWMInvert(bool &bInvert);
	//---------------------------------------------------------------------------------//
	bool                          CheckDLPStatus();
	bool                          GetDLPStatus_InitDone();
	bool                          GetDLPStatus_ForcedSwap();
	bool                          GetDLPStatus_BufferFreeze();
	bool                          GetDLPStatus_SeqRunning();
	bool                          GetDLPStatus_SeqError();
	bool                          GetDLPStatus_SeqAbort();
	bool                          GetDLPStatus_DRCError();
	bool                          GetDLPStatus_DMDParked();	
	//---------------------------------------------------------------------------------//
	bool                          InitialDLPParameter(TDLPParam &Param);
	bool                          SaveDLPParameter();//寫參數至檔案
	bool                          LoadDLPParameter();//從檔案讀取參數
	bool                          ReadDLPParameter();//從DLP裝置讀取參數
	TDLPParam&                    GetDLPParam();
	const TDLPParam&              GetDLPParam() const;
	bool                          SetDLPPhaseMode(int Mode);
	int                           GetDLPPhaseMode() const { return m_DLPParam.m_PhaseMode; }
	int                           GetDLPPeriodTime() const { return m_DLPParam.m_PeriodTime_us; }
	int                           GetDLPExposureTime() const { return m_DLPParam.m_ExposureTime_us; }
	bool                          GetDLPReadySignalEnable() const { return false; }
	//---------------------------------------------------------------------------------//
	bool                          SetDLPParamLEDCurrent(int red, int green, int blue, int CurrentID);
	bool                          GetDLPParamLEDCurrent(int &red, int &green, int &blue, int CurrentID);
	//---------------------------------------------------------------------------------//
	const char*                   GetDLPFrmTag();	
	const char*                   GetTiAPIVersion();	
	const char*                   GetDLPFrmVersion();	
	const char*                   GetDLPMcuVersion();
	//---------------------------------------------------------------------------------//
	bool                          GetUseExpLut();
	bool                          SetLEDColor(int Type);
	int                           GetLEDColor() const;	
	//---------------------------------------------------------------------------------//
	double                        GetImageGamma() const;
	bool                          SetImageGamma(double val);
	//---------------------------------------------------------------------------------//
	double                        GetSecondExpRatio() const;//取得第2次曝光比例
	void                          SetSecondExpRatio(double val);
	//---------------------------------------------------------------------------------//
	int                           GetPeriodPaddingTime() const;//週期外加時間-us
	int                           GetExposurePaddingTime() const;//曝光外加時間-us
	int                           CalcPeriodPaddingTime(int ExpTime) const;//計算週期外加時間-us
	//---------------------------------------------------------------------------------//
	bool                          GetUse3BitPattern() const;//取得使用3Bit樣板圖
	bool                          GetUse5BitPattern() const;//取得使用5Bit樣板圖
	//---------------------------------------------------------------------------------//	
	int                           GetPatternBitCount() const;//取得樣板圖位元數
	void                          SetPatternBitCount(int val);//設定樣板圖位元數
	//---------------------------------------------------------------------------------//
	int                           GetPatternIndex1_3Bit() const;//取得使用3Bit樣板引數-1
	void                          SetPatternIndex1_3Bit(int val);//設定使用3Bit樣板引數-1	
	int                           GetPatternIndex2_3Bit() const;//取得使用3Bit樣板引數-2
	void                          SetPatternIndex2_3Bit(int val);//設定使用3Bit樣板引數-2	
	int                           GetPatternIndex1_5Bit() const;//取得使用5Bit樣板引數-1
	void                          SetPatternIndex1_5Bit(int val);//設定使用5Bit樣板引數-1
	int                           GetPatternIndex1_6Bit() const;//取得使用6Bit樣板引數-1
	void                          SetPatternIndex1_6Bit(int val);//設定使用6Bit樣板引數-1	
	int                           GetPatternIndex2_6Bit() const;//取得使用6Bit樣板引數-2
	void                          SetPatternIndex2_6Bit(int val);//設定使用6Bit樣板引數-2
	int                           GetPatternIndex1_8Bit() const;//取得使用8Bit樣板引數-1
	void                          SetPatternIndex1_8Bit(int val);//設定使用8Bit樣板引數-1	
	int                           GetPatternIndex2_8Bit() const;//取得使用8Bit樣板引數-2
	void                          SetPatternIndex2_8Bit(int val);//設定使用8Bit樣板引數-2
	int                           GetPatternIndexGC_1Bit() const;//取得使用1Bit-GrayCode引數-1
	void                          SetPatternIndexGC_1Bit(int val);//設定使用1Bit-GrayCode引數-1
	int                           GetPatternIndexBC_1Bit() const;//取得使用1Bit-BinaryCode引數-1
	void                          SetPatternIndexBC_1Bit(int val);//設定使用1Bit-BinaryCode引數-1
	int                           GetPatternStartNumGC_1Bit() const;//取得使用1Bit-GrayCode起始張數-1
	void                          SetPatternStartNumGC_1Bit(int val);//設定使用1Bit-GrayCode起始張數-1
	int                           GetPatternStartNumBC_1Bit() const;//取得使用1Bit-BinaryCode起始張數-1
	void                          SetPatternStartNumBC_1Bit(int val);//設定使用1Bit-BinaryCode起始張數-1
	int                           GetPatternIndexGC_8Bit() const;//取得使用8Bit-GrayCode引數-1
	void                          SetPatternIndexGC_8Bit(int val);//設定使用8Bit-GrayCode引數-1
	int                           GetPatternIndexBC_8Bit() const;//取得使用8Bit-BinaryCode引數-1
	void                          SetPatternIndexBC_8Bit(int val);//設定使用8Bit-BinaryCode引數-1
	int                           GetPatternStartNumGC_8Bit() const;//取得使用8Bit-GrayCode起始張數-1
	void                          SetPatternStartNumGC_8Bit(int val);//設定使用8Bit-GrayCode起始張數-1
	int                           GetPatternStartNumBC_8Bit() const;//取得使用8Bit-BinaryCode起始張數-1
	void                          SetPatternStartNumBC_8Bit(int val);//設定使用8Bit-BinaryCode起始張數-1
	//---------------------------------------------------------------------------------//
	void                          SetPeriod_us(unsigned int val);
	void                          SetExposure_us(unsigned int val);	
	void                          SetPeriod2_us(unsigned int val);
	void                          SetExposure2_us(unsigned int val);
	//---------------------------------------------------------------------------------//
	void                          InitialPatItem(TDLPPatItem &PatItem);		
	//---------------------------------------------------------------------------------//
	bool                          ExecDLPPattern_Run();	
	bool                          ExecDLPPattern_RunFn();	
	bool                          ExecDLPPattern_Stop();
	bool                          ExecDLPPattern_StopFn();
	bool                          ExecDLPPattern_Pause();
	bool                          ExecDLPPattern_PauseFn();
	bool                          LEDSetting(int LEDCurrent=-1, int CurrentID=DLP_LED_CURRENT_ID_01);	
	size_t                        GetTriggerOutCount();
	//---------------------------------------------------------------------------------//
	size_t                        GetDLPPatCount();
	TDLPPatItem*                  GetDLPPatItemPtr(size_t index, bool check);	
	void                          ClearDLPPatternList();//清除m_PatternList
	bool                          BuildDLPPatternList(int Mode, bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表 
	bool                          BuildDLPPatternListFn(int Mode, bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表 
	void                          AddDLPPPatItem(TDLPPatItem &PatItem);//增加樣板項目	
	void                          RemoveDLPPatItem(size_t index);//移除樣板項目
	//---------------------------------------------------------------------------------//
	bool                          ExecDLPPatClear();//清除樣版內容
	bool                          ExecDLPPatRead(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat);//讀取DLP樣版內容
	bool                          ExecDLPPatReadFn(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat);//讀取DLP樣版內容
	bool                          ExecDLPPatRead_Lut(unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat);//讀取DLP樣版內容
	bool                          ExecDLPPatRead_ExpLut(unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat);//讀取DLP樣版內容

	bool                          ExecDLPPatSendAll(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新樣板至DLP
	bool                          ExecDLPPatSendAllFn(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新樣板至DLP
	bool                          ExecDLPPatSendAll_Lut(unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新樣板至DLP
	bool                          ExecDLPPatSendAll_ExpLut(unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新樣板至DLP

	bool                          ExecDLPPatSendOne(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新單一樣板至DLP
	bool                          ExecDLPPatSendOneFn(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新單一樣板至DLP
	bool                          ExecDLPPatSendOne_Lut(int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新單一樣板至DLP
	bool                          ExecDLPPatSendOne_ExpLut(int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新單一樣板至DLP

	bool                          ExecDLPValidatePatLutData(unsigned int &Status, bool Sleep);//套用樣板列表資料
	bool                          ExecDLPPatBuildSendValidate(int Mode, bool IntTrig, bool MultiTable, unsigned int Periodus, unsigned int exposure_us, int LEDColor, bool Sleep, bool Force);//建立樣板, 傳送樣板以及驗證

	bool                          ExecDLPLightSetting(int CurrentID);//執行DLP的LED設定-依據目前的設定
	//---------------------------------------------------------------------------------//
	bool                          BuildPatternImage(int BitDepth, int NPeriod, int NPixelPeriod, bool bVer, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage);
	//---------------------------------------------------------------------------------//	
	CString                       GetDLPPhaseZeroBinShortName(int LedClr);//取得相平面的校正短名
	CString                       GetDLPPhaseFactorBinShortName(int LedClr);//取相位比例的校正短名
	//---------------------------------------------------------------------------------//	
	CString                       GetDLPPhaseZeroBinFilename(int LedClr);//取得相平面的校正檔名		
	CString                       GetDLPLedColorTextForBinFile(int Color);//取得燈源模式的文字	
	CString                       GetDLPPhaseModeTextForBinFile(int Mode);//取得相位模式的文字
	CString                       GetDLPPhaseFactorBinFilename(int LedClr);//取相位比例的校正檔名	
	//---------------------------------------------------------------------------------//
	bool                          CheckPhaseZeroRed();//確認平面相位
	bool                          CheckPhaseZeroGrn();//確認平面相位
	bool                          CheckPhaseZeroBlu();//確認平面相位
	bool                          CheckPhaseZeroWhite();//確認平面相位
	bool                          CheckPhaseZeroDebug();//確認平面相位
	//---------------------------------------------------------------------------------//	
	bool                          LoadPhaseZero();//載入平面相位
	bool                          LoadPhaseZeroRed();//載入平面相位
	bool                          LoadPhaseZeroGrn();//載入平面相位
	bool                          LoadPhaseZeroBlu();//載入平面相位
	bool                          LoadPhaseZeroWhite();//載入平面相位
	bool                          LoadPhaseZeroDebug();//載入平面相位
	//---------------------------------------------------------------------------------//
	bool                          SavePhaseZero();//儲存平面相位
	bool                          SavePhaseZeroRed();//儲存平面相位
	bool                          SavePhaseZeroGrn();//儲存平面相位
	bool                          SavePhaseZeroBlu();//儲存平面相位
	bool                          SavePhaseZeroWhite();//儲存平面相位
	bool                          SavePhaseZeroDebug();//儲存平面相位
	//---------------------------------------------------------------------------------//
	bool                          ClearPhaseZeroBuffer();//清除平面相位	
	bool                          ClearPhaseZeroBufferRed();//清除平面相位	
	bool                          ClearPhaseZeroBufferGrn();//清除平面相位	
	bool                          ClearPhaseZeroBufferBlu();//清除平面相位	
	bool                          ClearPhaseZeroBufferWhite();//清除平面相位	
	bool                          ClearPhaseZeroBufferDebug();//清除平面相位		
	bool                          CheckDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep);//確認DLP平面相位檔案
	bool                          SetPhaseZero(int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, PHASE_PTR Ptr);//設定平面相位
	bool                          GetPhaseZero(int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr);//取得平面相位	
	bool                          ClonePhaseZero(int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr);//複製平面相位
	bool                          CheckDLPPhaseZeroData(IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr);//確認DLP平面相位資料
	bool                          SaveDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr);//儲存DLP平面相位
	//---------------------------------------------------------------------------------//	
	bool                          CheckPhaseFactorRed();//確認平面係數
	bool                          CheckPhaseFactorGrn();//確認平面係數
	bool                          CheckPhaseFactorBlu();//確認平面係數
	bool                          CheckPhaseFactorWhite();//確認平面係數
	bool                          CheckPhaseFactorDebug();//確認平面係數
	//---------------------------------------------------------------------------------//	
	bool                          LoadPhaseFactor();//載入平面係數
	bool                          LoadPhaseFactorRed();//載入平面係數
	bool                          LoadPhaseFactorGrn();//載入平面係數
	bool                          LoadPhaseFactorBlu();//載入平面係數
	bool                          LoadPhaseFactorWhite();//載入平面係數
	bool                          LoadPhaseFactorDebug();//載入平面係數
	//---------------------------------------------------------------------------------//	
	bool                          SavePhaseFactor();//儲存平面係數
	bool                          SavePhaseFactorRed();//儲存平面係數
	bool                          SavePhaseFactorGrn();//儲存平面係數
	bool                          SavePhaseFactorBlu();//儲存平面係數
	bool                          SavePhaseFactorWhite();//儲存平面係數
	bool                          SavePhaseFactorDebug();//儲存平面係數
	//---------------------------------------------------------------------------------//	
	bool                          ClearPhaseFactorBuffer();//清除平面係數
	bool                          ClearPhaseFactorBufferRed();//清除平面係數
	bool                          ClearPhaseFactorBufferGrn();//清除平面係數
	bool                          ClearPhaseFactorBufferBlu();//清除平面係數
	bool                          ClearPhaseFactorBufferWhite();//清除平面係數
	bool                          ClearPhaseFactorBufferDebug();//清除平面係數
	bool                          CheckDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep);//確認DLP平面係數檔案
	//---------------------------------------------------------------------------------//		
	double                        GetPhaseFactorMin() const;//取得平面係數下限
	void                          SetPhaseFactorMin(double val);//設定平面係數下限
	double                        GetPhaseFactorMax() const;//取得平面係數上限
	void                          SetPhaseFactorMax(double val);//設定平面係數上限
	//---------------------------------------------------------------------------------//	
	int                           GetLEDCurrentMax() const;//取得LED電流上限	
	bool                          GetLEDColorUsed_Red() const;//取得LED顏色使用-紅色
	bool                          GetLEDColorUsed_Grn() const;//取得LED顏色使用-綠色
	bool                          GetLEDColorUsed_Blu() const;//取得LED顏色使用-藍色
	//---------------------------------------------------------------------------------//	
	void                          ClearHeightFactor();//清除高度參數T0, T1, T3	
	bool                          SortHeightFactorTableList();//排序高度係數列表
	bool                          SaveHeightFactorTableList();//儲存高度係數列表檔案
	bool                          LoadHeightFactorTableList();//載入高度係數列表檔案
	bool                          RestoreHeightFactorTableList();//復原高度係數列表
	void                          ClearHeightFactorTableList(bool bIncludeFiles);//清除高度係數列表
	bool                          BuildHeightFactorMappingParam(bool bRecv);//建立高度參數T0, T1, T3	
	bool                          CalcHeightFactorMappingParam(std::vector<std::vector<TPhaseFactorGrid>> &GridListArray, std::vector<std::vector<double>> &ParamListArray);//計算高度參數T0, T1, T3
	bool                          VerifyHeightFactorMappingParam(const std::vector<std::vector<double>> &ParamListArray, std::vector<TPhaseFactorGrid> &GridList, double &Error);//驗證高度參數T0, T1, T3	
	bool                          GetHeightFactorMappingParam(double T0[], double T1[], double T2[]);//取得高度參數	
	//---------------------------------------------------------------------------------//		
	bool                          SetPhaseFactor(int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, SPACE_PTR Ptr);//設定平面係數
	bool                          GetPhaseFactor(int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr);//取得平面係數
	bool                          ClonePhaseFactor(int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr);//複製平面係數
	bool                          CheckDLPPhaseFactorData(IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr);//確認DLP平面係數
	bool                          SaveDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr);//儲存DLP平面係數
	//---------------------------------------------------------------------------------//	
	bool                          SetHeightFactorTable(int TargetNo, const TPhaseFactorTable &GridTable);//設定高度係數格點列表		
	bool                          CloneHeightFactorTable(int TargetNo, TPhaseFactorTable &GridTable) const;//複製高度係數格點列表	
	//---------------------------------------------------------------------------------//
	bool                          ResetLEDDisable(bool &ResetFinish, bool ShowMsg=true);
	bool                          GetGPIOStatus(UINT PinNum, bool &Status);	//取得GPIO pin 狀態
	bool                          SetGPIOStatus(UINT PinNum, bool Status);	//取得GPIO pin output 狀態
	bool                          GetGPIOTemperatureOver(bool &IsOver, bool ShowMsg);	//偵測溫度狀態。		GPIO11 input狀態。high高溫/low低溫
	bool                          GetGPIOLEDDisable(bool &IsDisable, bool ShowMsg);	//LED是否disable狀態。	GPIO5  input狀態。high disable/low disable已解除
	bool                          SetGPIOLEDEnable();//LED disable狀態解除。	GPIO6  output狀態。 low->hi	
	void                          DLPSettingDelay();//DLP設定時要先延遲一段時間
	//---------------------------------------------------------------------------------//	
protected:
	//API from Ti source code
	int  DLPC350_Write(bool ackRequired);
	int  DLPC350_Read();
	int  DLPC350_ContinueRead();
	int  DLPC350_SendMsg(hidMessageStruct *pMsg, bool ackRequired);
	int  DLPC350_PrepReadCmd(DLPC350_CMD cmd);
	int  DLPC350_PrepReadCmdWithParam(DLPC350_CMD cmd, unsigned char param);
	int  DLPC350_PrepMemReadCmd(unsigned int addr);
	int  DLPC350_PrepWriteCmd(hidMessageStruct *pMsg, DLPC350_CMD cmd);
	//---------------------------------------------------------------------------------//
	int  DLPC350_GetVideoSignalStatus(VideoSigStatus *vidSigStat);
	int  DLPC350_SetInputSource(unsigned int source, unsigned int portWidth);
	int  DLPC350_GetInputSource(unsigned int *pSource, unsigned int *portWidth);
	int  DLPC350_SetPixelFormat(unsigned int format);
	int  DLPC350_GetPixelFormat(unsigned int *pFormat);
	int  DLPC350_SetPortClock(unsigned int clock);
	int  DLPC350_GetPortClock(unsigned int *pClock);
	int  DLPC350_SetDataChannelSwap(unsigned int port, unsigned int swap);
	int  DLPC350_GetDataChannelSwap(unsigned int *pPort, unsigned int *pSwap);
	int  DLPC350_SetFPD_Mode_Field(unsigned int PixelMappingMode, bool SwapPolarity, unsigned int FieldSignalSelect);
	int  DLPC350_GetFPD_Mode_Field(unsigned int *pPixelMappingMode, bool *pSwapPolarity, unsigned int *pFieldSignalSelect);
	int  DLPC350_SetPowerMode(bool);
	int  DLPC350_GetPowerMode(bool *Standby);
	int  DLPC350_SetLongAxisImageFlip(bool);
	bool DLPC350_GetLongAxisImageFlip();
	int  DLPC350_SetShortAxisImageFlip(bool);
	bool DLPC350_GetShortAxisImageFlip();
	int  DLPC350_SetTPGSelect(unsigned int pattern);
	int  DLPC350_GetTPGSelect(unsigned int *pPattern);
	int  DLPC350_SetLEDPWMInvert(bool invert);
	int  DLPC350_GetLEDPWMInvert(bool *inverted);
	int  DLPC350_SetLedEnables(bool SeqCtrl, bool Red, bool Green, bool Blue);
	int  DLPC350_GetLedEnables(bool *pSeqCtrl, bool *pRed, bool *pGreen, bool *pBlue);
	int  DLPC350_GetVersion(unsigned int *pApp_ver, unsigned int *pAPI_ver, unsigned int *pSWConfig_ver, unsigned int *pSeqConfig_ver);
	int  DLPC350_GetFirmwareVersion(unsigned int *pFW_ver);
	int  DLPC350_SoftwareReset(void);
	int  DLPC350_GetStatus(unsigned char *pHWStatus, unsigned char *pSysStatus, unsigned char *pMainStatus);
	int  DLPC350_SetPWMEnable(unsigned int channel, bool Enable);
	int  DLPC350_GetPWMEnable(unsigned int channel, bool *pEnable);
	int  DLPC350_SetPWMConfig(unsigned int channel, unsigned int pulsePeriod, unsigned int dutyCycle);
	int  DLPC350_GetPWMConfig(unsigned int channel, unsigned int *pPulsePeriod, unsigned int *pDutyCycle);
	int  DLPC350_SetPWMCaptureConfig(unsigned int channel, bool enable, unsigned int sampleRate);
	int  DLPC350_GetPWMCaptureConfig(unsigned int channel, bool *pEnabled, unsigned int *pSampleRate);
	int  DLPC350_SetGPIOConfig(unsigned int pinNum, bool enAltFunc, bool altFunc1, bool dirOutput, bool outTypeOpenDrain, bool pinState);
	int  DLPC350_GetGPIOConfig(unsigned int pinNum, bool *pEnAltFunc, bool *pAltFunc1, bool *pDirOutput, bool *pOutTypeOpenDrain, bool *pState);
	int  DLPC350_GetLedCurrents(unsigned char *pRed, unsigned char *pGreen, unsigned char *pBlue);
	int  DLPC350_SetLedCurrents(unsigned char RedCurrent, unsigned char GreenCurrent, unsigned char BlueCurrent);
	int  DLPC350_SetDisplay(rectangle2 croppedArea, rectangle2 displayArea);
	int  DLPC350_GetDisplay(rectangle2 *pCroppedArea, rectangle2 *pDisplayArea);
	int  DLPC350_MemRead(unsigned int addr, unsigned int *readWord);
	int  DLPC350_MemWrite(unsigned int addr, unsigned int data);
	int  DLPC350_ValidatePatLutData(unsigned int *pStatus);
	int  DLPC350_StartPatLutValidate();
	int  DLPC350_CheckPatLutValidate(bool *ready, unsigned int *pStatus);
	int  DLPC350_SetPatternDisplayMode(bool external);
	int  DLPC350_GetPatternDisplayMode(bool *external);
	int  DLPC350_SetTrigOutConfig(unsigned int trigOutNum, bool invert, unsigned int rising, unsigned int falling);
	int  DLPC350_GetTrigOutConfig(unsigned int trigOutNum, bool *pInvert,unsigned int *pRising, unsigned int *pFalling);
	int  DLPC350_SetRedLEDStrobeDelay(unsigned char rising, unsigned char falling);
	int  DLPC350_SetGreenLEDStrobeDelay(unsigned char rising, unsigned char falling);
	int  DLPC350_SetBlueLEDStrobeDelay(unsigned char rising, unsigned char falling);
	int  DLPC350_GetRedLEDStrobeDelay(unsigned char *, unsigned char *);
	int  DLPC350_GetGreenLEDStrobeDelay(unsigned char *, unsigned char *);
	int  DLPC350_GetBlueLEDStrobeDelay(unsigned char *, unsigned char *);
	int  DLPC350_EnterProgrammingMode(void);
	int  DLPC350_ExitProgrammingMode(void);
	int  DLPC350_GetProgrammingMode(bool *ProgMode);
	int  DLPC350_GetFlashManID(unsigned short *manID);
	int  DLPC350_GetFlashDevID(unsigned long long *devID);
	int  DLPC350_GetBLStatus(unsigned char *BL_Status);
	int  DLPC350_SetFlashAddr(unsigned int Addr);
	int  DLPC350_FlashSectorErase(void);
	int  DLPC350_SetUploadSize(unsigned long int dataLen);
	int  DLPC350_UploadData(unsigned char *pByteArray, unsigned int dataLen);
	void DLPC350_WaitForFlashReady(void);
	int  DLPC350_SetFlashType(unsigned char Type);
	int  DLPC350_CalculateFlashChecksum(void);
	int  DLPC350_GetFlashChecksum(unsigned int*checksum);
	int  DLPC350_SetMode(bool SLmode);
	int  DLPC350_GetMode(bool *pMode);
	int  DLPC350_LoadImageIndex(unsigned int index);
	int  DLPC350_GetImageIndex(unsigned int *pIndex);
	int  DLPC350_GetNumImagesInFlash(unsigned int *pNumImgInFlash);
	int  DLPC350_SetTPGColor(unsigned short redFG, unsigned short greenFG, unsigned short blueFG, unsigned short redBG, unsigned short greenBG, unsigned short blueBG);
	int  DLPC350_GetTPGColor(unsigned short *pRedFG, unsigned short *pGreenFG, unsigned short *pBlueFG, unsigned short *pRedBG, unsigned short *pGreenBG, unsigned short *pBlueBG);
	int  DLPC350_ClearPatLut(void);
	int  DLPC350_ClearExpLut(void);
	int  DLPC350_AddToPatLut(int TrigType, int PatNum,int BitDepth,int LEDSelect,bool InvertPat, bool InsertBlack,bool BufSwap, bool trigOutPrev);
	int  DLPC350_AddToExpLut(int TrigType, int PatNum,int BitDepth,int LEDSelect,bool InvertPat, bool InsertBlack,bool BufSwap, bool trigOutPrev, unsigned int exp_time_us, unsigned int ptn_frame_period_us);
	int  DLPC350_GetPatLutItem(int index, int *pTrigType, int *pPatNum,int *pBitDepth,int *pLEDSelect,bool *pInvertPat, bool *pInsertBlack,bool *pBufSwap, bool *pTrigOutPrev);
	int  DLPC350_GetVarExpPatLutItem(int index, int *pTrigType, int *pPatNum,int *pBitDepth,int *pLEDSelect,bool *pInvertPat, bool *pInsertBlack,bool *pBufSwap, bool *pTrigOutPrev, int *pPatExp, int *pPatPeriod);	
	int  DLPC350_SendPatLut(void);
	int  DLPC350_SendVarExpPatLut(void);
	int  DLPC350_SendImageLut(unsigned char *lutEntries, unsigned int numEntries);
	int  DLPC350_SendVarExpImageLut(unsigned char *lutEntries, unsigned int numEntries);
	int  DLPC350_GetPatLut(int numEntries);
	int  DLPC350_GetVarExpPatLut(int numEntries);
	int  DLPC350_GetImageLut(unsigned char *pLut, int numEntries);
	int  DLPC350_GetvarExpImageLut(unsigned char *pLut, int numEntries);
	int  DLPC350_SetPatternTriggerMode(int);
	int  DLPC350_GetPatternTriggerMode(int *);
	int  DLPC350_PatternDisplay(unsigned int Action);
	int  DLPC350_GetPatternDisplay(unsigned int *pAction);
	int  DLPC350_SetVarExpPatternConfig(unsigned int numLutEntries, unsigned int numPatsForTrigOut2, unsigned int numImages, bool repeat);
	int  DLPC350_GetVarExpPatternConfig(unsigned int *pNumLutEntries, unsigned int *pNumPatsForTrigOut2, unsigned int *pNumImages,  bool *pRepeat);
	int  DLPC350_SetPatternConfig(unsigned int numLutEntries, bool repeat, unsigned int numPatsForTrigOut2, unsigned int numImages);
	int  DLPC350_GetPatternConfig(unsigned int *pNumLutEntries, bool *pRepeat, unsigned int *pNumPatsForTrigOut2, unsigned int *pNumImages);
	int  DLPC350_SetExposure_FramePeriod(unsigned int exposurePeriod, unsigned int framePeriod);
	int  DLPC350_GetExposure_FramePeriod(unsigned int *pExposure, unsigned int *pFramePeriod);
	int  DLPC350_SetTrigIn1Delay(unsigned int Delay);
	int  DLPC350_GetTrigIn1Delay(unsigned int *pDelay);
	int  DLPC350_SetTrigIn2Pol(bool isFallingEdge);
	int  DLPC350_GetTrigIn2Pol(bool *pIsFallingEdge);
	int  DLPC350_SetInvertData(bool invert);
	int  DLPC350_PWMCaptureRead(unsigned int channel, unsigned int *pLowPeriod, unsigned int *pHighPeriod);
	int  DLPC350_SetGeneralPurposeClockOutFreq(unsigned int clkId, bool enable, unsigned int clkDivider);
	int  DLPC350_GetGeneralPurposeClockOutFreq(unsigned int clkId, bool *pEnabled, unsigned int *pClkDivider);
	int  DLPC350_MeasureImageLoadTiming(unsigned int startIndex, unsigned int numFlash);
	int  DLPC350_ReadImageLoadTiming(unsigned int *pTimingData);
	int  DLPC350_SetFreeze(bool Freeze);
	int  DLPC350_GetFirmwareTagInfo(unsigned char *pFwTagInfo);
	int  DLPC350_I2C0WriteData(bool is7Bit,unsigned int sclClk, unsigned int devAddr, unsigned int numWriteBytes, unsigned char *pWdata);
	int  DLPC350_I2C0ReadData(bool is7Bit, unsigned int sclClk, unsigned int devAddr, unsigned int numWriteBytes, unsigned int numReadBytes, unsigned char *pWData, unsigned char *pRdata);
	int  DLPC350_I2C0TranStat(unsigned char *pStat);
	//---------------------------------------------------------------------------------//
	int  DLPC350_OpenMailbox(int MboxNum);
	int  DLPC350_CloseMailbox(void);
	int  DLPC350_MailboxSetAddr(int Addr);
	int  DLPC350_SetVarExpMboxAddr(int Addr);
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif//LIGHT_3D_TI_DLP_4500_USE
#endif // !defined(AFX_LIGHT3DTIDLP4500_H__091F6483_6B4F_4505_A49B_46CC67AA8BE4__INCLUDED_)
