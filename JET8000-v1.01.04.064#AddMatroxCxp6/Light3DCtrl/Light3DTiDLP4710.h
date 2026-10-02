// Light3DTiDLP4710.h: interface for the CLight3DTiDLP4710 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHT3DTIDLP4710_H__091F6483_6B4F_4505_A49B_46CC67AA8BE4__INCLUDED_)
#define AFX_LIGHT3DTIDLP4710_H__091F6483_6B4F_4505_A49B_46CC67AA8BE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Light3DDef.h"
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_4710_USE
//-------------------------------------------------------------------------------------//
#include "Light3DTiDLPDef.h"
//-------------------------------------------------------------------------------------//
#include "stdint.h"
#include "stdio.h"
#include "math.h"
#include "time.h"

#include "dlpc_common.h"
#include "dlpc34xx_dual.h"
#include "dlpc347x_internal_patterns.h"
#include "CypressI2C.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif

#define CYPRESS_I2C_COMMUNICATION         1		/* set to 0 for DeVaSys I2C Communication */

#define MAX_WIDTH                         1920//DLP4710_WIDTH
#define MAX_HEIGHT                        1082//DLP4710_HEIGHT

#define MAX_PATTERN_SETS                  128
#define PATTERN_STEP_DELAY_MS             500

#define NUM_PATTERN_SETS                  4
#define NUM_PATTERN_ORDER_TABLE_ENTRIES   4
#define NUM_ONE_BIT_HORIZONTAL_PATTERNS   4
#define NUM_EIGHT_BIT_HORIZONTAL_PATTERNS 4
#define NUM_ONE_BIT_VERTICAL_PATTERNS     4
#define NUM_EIGHT_BIT_VERTICAL_PATTERNS   4
#define TOTAL_HORIZONTAL_PATTERNS         (NUM_ONE_BIT_HORIZONTAL_PATTERNS + NUM_EIGHT_BIT_HORIZONTAL_PATTERNS)
#define TOTAL_VERTICAL_PATTERNS           (NUM_ONE_BIT_VERTICAL_PATTERNS + NUM_EIGHT_BIT_VERTICAL_PATTERNS)

#define FLASH_WRITE_BLOCK_SIZE            1024
#define FLASH_READ_BLOCK_SIZE             256

#define MAX_WRITE_CMD_PAYLOAD             (FLASH_WRITE_BLOCK_SIZE + 8)
#define MAX_READ_CMD_PAYLOAD              (FLASH_READ_BLOCK_SIZE  + 8)

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
#define DLPC350_BIN_VERSION_2_0_0                   0x00200000
#define DLPC350_BIN_VERSION_3_0_0                   0x00300000
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
		m_PeriodPaddingTime = 1000;//週期外加時間-us
		m_ExposurePaddingTime = 0;//曝光外加時間-us
//	bool	InvertPat = false;
//	bool	InsertBlack = true;
//	bool	TrigOutPrev = false;		
	}
} TDLPParam, *PDLPParam;
//-------------------------------------------------------------------------------------//
class CLight3DTiDLP4710  
{
public:		
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//	
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
	unsigned int               m_PatternSetCountInDLP;//DLP樣板設定數量
	//---------------------------------------------------------------------------------//	
	int                        m_PatternDarkIndex_8Bit;
	int                        m_PatternGrayIndex_8Bit;
	int                        m_PatternWhiteIndex_8Bit;	
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
	int                        m_PatternIndexGC_8Bit;//GrayCode	
	int                        m_PatternIndexBC_8Bit;//BinaryCode	
	int                        m_PatternStartNumGC_1Bit;//GrayCode
	int                        m_PatternStartNumBC_1Bit;//BinaryCode
	int                        m_PatternStartNumGC_8Bit;//GrayCode
	int                        m_PatternStartNumBC_8Bit;//BinaryCode
	//---------------------------------------------------------------------------------//
	unsigned char              m_HWStatus;///硬體狀態
	unsigned char              m_SysStatus;//系統狀態
	unsigned char              m_MainStatus;//主要狀態
	DLPC34XX_DUAL_ShortStatus_s  m_ShortStatus;//簡約狀態
	DLPC34XX_DUAL_SystemStatus_s m_SystemStatus;//系統狀態
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
private:
	//---------------------------------------------------------------------------------//
	uint32_t                                  s_Index;
	uint8_t                                   s_HorizontalPatternData[TOTAL_HORIZONTAL_PATTERNS][MAX_HEIGHT];
	uint8_t                                   s_VerticalPatternData[TOTAL_VERTICAL_PATTERNS][MAX_WIDTH];
	DLPC34XX_INT_PAT_PatternData_s            s_Patterns[TOTAL_HORIZONTAL_PATTERNS + TOTAL_VERTICAL_PATTERNS];
	DLPC34XX_INT_PAT_PatternSet_s             s_PatternSets[NUM_PATTERN_SETS];
	DLPC34XX_INT_PAT_PatternOrderTableEntry_s s_PatternOrderTable[NUM_PATTERN_ORDER_TABLE_ENTRIES];

	uint8_t                                   s_WriteBuffer[MAX_WRITE_CMD_PAYLOAD];
	uint8_t                                   s_ReadBuffer[MAX_READ_CMD_PAYLOAD];

	bool                                      s_StartProgramming;
	uint8_t                                   s_FlashProgramBuffer[FLASH_WRITE_BLOCK_SIZE];
	uint16_t                                  s_FlashProgramBufferPtr;

	FILE*                                     s_FilePointer;

	CCypressI2C                               m_I2C;	
	uint16_t                                  s_WriteBufferSize;
	uint16_t                                  s_WriteBufferIndex;	
	uint16_t                                  s_ReadBufferSize;
	uint16_t                                  s_ReadBufferIndex;	
	DLPC_COMMON_CommandProtocolData_s         s_ProtocolData;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	CLight3DTiDLP4710(const CLight3DTiDLP4710 &DlpCtrl);
	CLight3DTiDLP4710& operator=(const CLight3DTiDLP4710 &DlpCtrl);
	//---------------------------------------------------------------------------------//
	void                          PreInitTiDlp(int CtrlID, LIGHT_3D_CAST_ID Light3DID);
	void                          InitialTiDlp();
	//---------------------------------------------------------------------------------//
	void                          LockLight3D();
	void                          UnlockLight3D();
	//---------------------------------------------------------------------------------//
	bool                          GetDlpMonoMode() const;//單燈源模式
	//---------------------------------------------------------------------------------//
	CString                       GetDLPErrorKeyName() const;
	//---------------------------------------------------------------------------------//
	void                          SetLCRErrorFnName(LPCTSTR LCRFnName);	
	//---------------------------------------------------------------------------------//	
	bool                          GetUSB_Number(LIGHT_3D_CAST_ID CastID, char USB_Number[]);
	bool                          GetUSB_Number(LIGHT_3D_CAST_ID CastID, wchar_t USB_Number[]);
	//---------------------------------------------------------------------------------//
	bool                          ReturnNotSupportFunc(const TCHAR *fnName);
	//---------------------------------------------------------------------------------//
	bool                          ResetDLP4710();
	bool                          CheckDLP4710McuIsConnected();//確認DLP4710的MCU可連線
	//---------------------------------------------------------------------------------//
	bool                          ReadSystemTemperature(double *Temperature);
	bool                          ReadShortStatus(DLPC34XX_DUAL_ShortStatus_s *ShortStatus);
	bool                          ReadSystemStatus(DLPC34XX_DUAL_SystemStatus_s *SystemStatus);
	bool                          ReadSystemSoftwareVersion(uint16_t *PatchVersion, uint8_t *MinorVersion, uint8_t *MajorVersion);
	bool                          ReadFirmwareBuildVersion(uint16_t *PatchVersion, uint8_t *MinorVersion, uint8_t *MajorVersion);
	//---------------------------------------------------------------------------------//	
	DLPC34XX_DUAL_OperatingMode_e MapOperatingModeSelect(int OperatingMode);
	int                           MapOperatingModeSelect(DLPC34XX_DUAL_OperatingMode_e OperatingMode);	
	bool                          ReadOperatingModeSelect(DLPC34XX_DUAL_OperatingMode_e *OperatingMode);
	bool                          WriteOperatingModeSelect(DLPC34XX_DUAL_OperatingMode_e OperatingMode);	
	//---------------------------------------------------------------------------------//
	bool                          ReadDisplayImageOrientation(DLPC34XX_DUAL_ImageFlip_e *LongAxisImageFlip, DLPC34XX_DUAL_ImageFlip_e *ShortAxisImageFlip);
	bool                          WriteDisplayImageOrientation(DLPC34XX_DUAL_ImageFlip_e LongAxisImageFlip, DLPC34XX_DUAL_ImageFlip_e ShortAxisImageFlip);
	//---------------------------------------------------------------------------------//
	bool                          ReadLedOutputControlMethod(DLPC34XX_DUAL_LedControlMethod_e *LedControlMethod);
	bool                          WriteLedOutputControlMethod(DLPC34XX_DUAL_LedControlMethod_e LedControlMethod);
	bool                          ReadRgbLedEnable(bool *RedLedEnable, bool *GreenLedEnable, bool *BlueLedEnable);
	bool                          WriteRgbLedEnable(bool RedLedEnable, bool GreenLedEnable, bool BlueLedEnable);	
	bool                          ReadRgbLedCurrent(uint16_t *RedLedCurrent, uint16_t *GreenLedCurrent, uint16_t *BlueLedCurrent);
	bool                          WriteRgbLedCurrent(uint16_t RedLedCurrent, uint16_t GreenLedCurrent, uint16_t BlueLedCurrent);	
	//---------------------------------------------------------------------------------//	
	bool                          WriteTriggerInConfiguration(DLPC34XX_DUAL_TriggerEnable_e TriggerEnable, DLPC34XX_DUAL_TriggerPolarity_e TriggerPolarity);
	bool                          ReadTriggerInConfiguration(DLPC34XX_DUAL_TriggerEnable_e *TriggerEnable, DLPC34XX_DUAL_TriggerPolarity_e *TriggerPolarity);
	//---------------------------------------------------------------------------------//
	bool                          WritePatternReadyConfiguration(DLPC34XX_DUAL_TriggerEnable_e TriggerEnable, DLPC34XX_DUAL_TriggerPolarity_e TriggerPolarity);
	bool                          ReadPatternReadyConfiguration(DLPC34XX_DUAL_TriggerEnable_e *TriggerEnable, DLPC34XX_DUAL_TriggerPolarity_e *TriggerPolarity);
	//---------------------------------------------------------------------------------//
	bool                          WriteTriggerOutConfiguration(DLPC34XX_DUAL_TriggerType_e TriggerType, DLPC34XX_DUAL_TriggerEnable_e TriggerEnable, DLPC34XX_DUAL_TriggerInversion_e TriggerInversion, int32_t Delay);
	bool                          ReadTriggerOutConfiguration(DLPC34XX_DUAL_TriggerType_e TriggerType, DLPC34XX_DUAL_TriggerEnable_e *TriggerEnable, DLPC34XX_DUAL_TriggerInversion_e *TriggerInversion, int32_t *Delay);
	//---------------------------------------------------------------------------------//
	bool                          WriteInternalPatternControl(DLPC34XX_DUAL_PatternControl_e PatternControl, uint8_t RepeatCount=0);
	//---------------------------------------------------------------------------------//
	unsigned int                  GetPatternSetCountInDLP();	
	bool                          ReadPatternOrderTableEntry(uint8_t PatternOrderTableEntryIndex, DLPC34XX_DUAL_PatternOrderTableEntry_s *PatternOrderTableEntry);
	bool                          WritePatternOrderTableEntry(DLPC34XX_DUAL_WriteControl_e WriteControl, DLPC34XX_DUAL_PatternOrderTableEntry_s *PatternOrderTableEntry);
	bool                          ReadValidateExposureTime(DLPC34XX_DUAL_PatternMode_e PatternMode, DLPC34XX_DUAL_SequenceType_e BitDepth, uint32_t ExposureTime, DLPC34XX_DUAL_ValidateExposureTime_s *ValidateExposureTime);
	//---------------------------------------------------------------------------------//
	int                           GetDLPSafeCurrent(int value);	
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
	CLight3DTiDLP4710();
	CLight3DTiDLP4710(int CtrlID, LIGHT_3D_CAST_ID Light3DID);
	virtual ~CLight3DTiDLP4710();
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
	bool                          GetDLPReadySignalEnable() const { return true; }
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
	int                           GetPatternDarkIndex_8Bit() const;//取得使用8Bit黑畫面引數-1
	void                          SetPatternDarkIndex_8Bit(int val);//設定使用8Bit黑畫面引數-1	
	int                           GetPatternGrayIndex_8Bit() const;//取得使用8Bit灰畫面引數-1
	void                          SetPatternGrayIndex_8Bit(int val);//設定使用8Bit灰畫面引數-1	
	int                           GetPatternWhiteIndex_8Bit() const;//取得使用8Bit白畫面引數-1
	void                          SetPatternWhiteIndex_8Bit(int val);//設定使用8Bit白畫面引數-1		
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
	int                           GetPatternIndexGC_8Bit() const;//取得使用8Bit-GrayCode引數-1
	void                          SetPatternIndexGC_8Bit(int val);//設定使用8Bit-GrayCode引數-1
	int                           GetPatternIndexBC_8Bit() const;//取得使用8Bit-BinaryCode引數-1
	void                          SetPatternIndexBC_8Bit(int val);//設定使用8Bit-BinaryCode引數-1
	int                           GetPatternStartNumGC_1Bit() const;//取得使用1Bit-GrayCode起始張數-1
	void                          SetPatternStartNumGC_1Bit(int val);//設定使用1Bit-GrayCode起始張數-1
	int                           GetPatternStartNumBC_1Bit() const;//取得使用1Bit-BinaryCode起始張數-1
	void                          SetPatternStartNumBC_1Bit(int val);//設定使用1Bit-BinaryCode起始張數-1
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
	bool                          CheckDLPPatternIndex(int index, unsigned int Count, int Mode);
	bool                          BuildDLPPatternList(int Mode, bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表 
	bool                          BuildDLPPatternList_TestGC(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-測試GC	
	bool                          BuildDLPPatternList_White(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-白燈
	bool                          BuildDLPPatternList_RGB(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-紅綠藍燈
	bool                          BuildDLPPatternList_2X2_M(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-22M
	bool                          BuildDLPPatternList_4X4_1(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-441
	bool                          BuildDLPPatternList_4X4_2(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-442
	bool                          BuildDLPPatternList_4X2_M(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-42M
	bool                          BuildDLPPatternList_4X4_M(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-44M
	bool                          BuildDLPPatternList_4X4GC_M(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-4X4GCM
	bool                          BuildDLPPatternList_4X5GC_M(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-4X5GCM
	bool                          BuildDLPPatternList_4X6GC_M(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-4X6GCM
	bool                          BuildDLPPatternList_4X4GC_M2(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-4X4GCM2
	bool                          BuildDLPPatternList_4X5GC_M2(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-4X5GCM2
	bool                          BuildDLPPatternList_4X6GC_M2(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-4X6GCM2
	bool                          BuildDLPPatternList_4X2_M2(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-42M2
	bool                          BuildDLPPatternList_4X4_M2(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-44M2
	void                          AddDLPPPatItem(TDLPPatItem &PatItem);//增加樣板項目	
	void                          RemoveDLPPatItem(size_t index);//移除樣板項目
	//---------------------------------------------------------------------------------//	
	bool                          ExecDLPPatClear();//清除樣版內容
	bool                          ExecDLPPatRead(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat);//讀取DLP樣版內容	
	bool                          ExecDLPPatReadFn(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat);//讀取DLP樣版內容	
	bool                          ExecDLPPatSendAll(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新樣板至DLP
	bool                          ExecDLPPatSendAllFn(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新樣板至DLP
	bool                          ExecDLPPatSendOne(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新單一樣板至DLP	
	bool                          ExecDLPPatSendOneFn(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新單一樣板至DLP	

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
	//---------------------------------------------------------------------------------//	
	bool     CYPRESS_I2C_WriteI2C(uint32_t WriteDataLength, uint8_t* WriteData);
	bool     CYPRESS_I2C_ReadI2C(uint32_t ReadDataLength, uint8_t* ReadData);

	uint32_t WriteI2C(uint16_t WriteDataLength, uint8_t* WriteData,	DLPC_COMMON_CommandProtocolData_s* ProtocolData);
	uint32_t ReadI2C(uint16_t WriteDataLength,	uint8_t* WriteData,	uint16_t ReadDataLength, uint8_t* ReadData,	DLPC_COMMON_CommandProtocolData_s* ProtocolData);

	int64_t  ConvertFloatToFixed(double Value, uint32_t Scale);
	double   ConvertFixedToFloat(int64_t Value, uint32_t Scale);
	uint64_t GetBitMask(uint8_t NumBits);
	bool     IsSignedBitSet(int64_t Value, int32_t NumBits);

	uint32_t DLPC_COMMON_SendWrite();
	uint32_t DLPC_COMMON_SendRead(uint16_t ReadLength);
	void     DLPC_COMMON_ClearWriteBuffer();
	void     DLPC_COMMON_ClearReadBuffer();	
	void     DLPC_COMMON_PackOpcode(int32_t Length, uint16_t Opcode);
	void     DLPC_COMMON_MoveWriteBufferPointer(int32_t Offset);
	void     DLPC_COMMON_PackByte(uint8_t Data);
	void     DLPC_COMMON_PackBytes(uint8_t* Data, int32_t Length);
	void     DLPC_COMMON_PackFloat(double Value, int32_t Length, uint32_t Scale);
	void     DLPC_COMMON_SetBits(int32_t Value, int32_t NumBits, int32_t BitOffset);
	void     DLPC_COMMON_MoveReadBufferPointer(int32_t Length);
	uint8_t* DLPC_COMMON_UnpackBytes(int32_t Length);
	double   DLPC_COMMON_UnpackFloat(int32_t Length, uint32_t Scale, bool Signed);
	uint64_t DLPC_COMMON_GetBits(uint8_t NumBits, uint8_t BitOffset, bool Signed);
	void     DLPC_COMMON_SetCommandDestination(uint16_t CommandDestination);
	uint16_t DLPC_COMMON_GetBytesRead();
	//---------------------------------------------------------------------------------//	
	uint32_t MCU_Write_MCU_I2Ccontrol(uint8_t command, uint8_t Index);//DLP4710_MCU_USE
	uint32_t DLPC34xx_AH_ReadMcuVersion(uint8_t *data, uint8_t *dataLength);//DLP4710_MCU_USE
	uint32_t DLPC34xx_AH_ReadEeprom(uint8_t index, uint8_t *data);//DLP4710_MCU_USE
	uint32_t eeprom_Write_eeprom_I2Ccontrol(uint8_t command, uint8_t Index);//DLP4710_MCU_USE

	uint32_t DLPC34XX_DUAL_WriteOperatingModeSelect(DLPC34XX_DUAL_OperatingMode_e OperatingMode);
	uint32_t DLPC34XX_DUAL_ReadOperatingModeSelect(DLPC34XX_DUAL_OperatingMode_e *OperatingMode);
	uint32_t DLPC34XX_DUAL_WriteSplashScreenSelect(uint8_t SplashScreenIndex);
	uint32_t DLPC34XX_DUAL_ReadSplashScreenSelect(uint8_t *SplashScreenIndex);
	uint32_t DLPC34XX_DUAL_WriteSplashScreenExecute();
	uint32_t DLPC34XX_DUAL_ReadSplashScreenHeader(uint8_t SplashScreenIndex, DLPC34XX_DUAL_SplashScreenHeader_s *SplashScreenHeader);
	uint32_t DLPC34XX_DUAL_WriteExternalVideoSourceFormatSelect(DLPC34XX_DUAL_ExternalVideoFormat_e VideoFormat);
	uint32_t DLPC34XX_DUAL_ReadExternalVideoSourceFormatSelect(DLPC34XX_DUAL_ExternalVideoFormat_e *VideoFormat);
	uint32_t DLPC34XX_DUAL_WriteVideoChromaProcessingSelect(DLPC34XX_DUAL_ChromaInterpolationMethod_e ChromaInterpolationMethod, DLPC34XX_DUAL_ChromaChannelSwap_e ChromaChannelSwap, uint8_t CscCoefficientSet);
	uint32_t DLPC34XX_DUAL_ReadVideoChromaProcessingSelect(DLPC34XX_DUAL_ChromaInterpolationMethod_e *ChromaInterpolationMethod, DLPC34XX_DUAL_ChromaChannelSwap_e *ChromaChannelSwap, uint8_t *CscCoefficientSet);
	uint32_t DLPC34XX_DUAL_Write3DControl(DLPC34XX_DUAL_ThreeDDominance_e ThreeDFrameDominance, DLPC34XX_DUAL_ThreeDReferencePolarity_e ThreeDReferencePolarity);
	uint32_t DLPC34XX_DUAL_Read3DControl(DLPC34XX_DUAL_ThreeDModes_e *ThreeDMode, DLPC34XX_DUAL_ThreeDDominance_e *ThreeDFrameDominance, DLPC34XX_DUAL_ThreeDReferencePolarity_e *ThreeDReferencePolarity);
	uint32_t DLPC34XX_DUAL_WriteInputImageSize(uint16_t PixelsPerLine, uint16_t LinesPerFrame);
	uint32_t DLPC34XX_DUAL_ReadInputImageSize(uint16_t *PixelsPerLine, uint16_t *LinesPerFrame);
	uint32_t DLPC34XX_DUAL_WriteDisplaySize(uint16_t PixelsPerLine, uint16_t LinesPerFrame);
	uint32_t DLPC34XX_DUAL_ReadDisplaySize(uint16_t *PixelsPerLine, uint16_t *LinesPerFrame);
	uint32_t DLPC34XX_DUAL_WriteDisplayImageOrientation(DLPC34XX_DUAL_ImageFlip_e LongAxisImageFlip, DLPC34XX_DUAL_ImageFlip_e ShortAxisImageFlip);
	uint32_t DLPC34XX_DUAL_ReadDisplayImageOrientation(DLPC34XX_DUAL_ImageFlip_e *LongAxisImageFlip, DLPC34XX_DUAL_ImageFlip_e *ShortAxisImageFlip);
	uint32_t DLPC34XX_DUAL_WriteDisplayImageCurtain(DLPC34XX_DUAL_ImageCurtainEnable_e Enable, DLPC34XX_DUAL_Color_e Color);
	uint32_t DLPC34XX_DUAL_ReadDisplayImageCurtain(DLPC34XX_DUAL_ImageCurtainEnable_e *Enable, DLPC34XX_DUAL_Color_e *Color);
	uint32_t DLPC34XX_DUAL_WriteImageFreeze(bool Enable);
	uint32_t DLPC34XX_DUAL_ReadImageFreeze(bool *Enable);
	uint32_t DLPC34XX_DUAL_WriteBorderColor(DLPC34XX_DUAL_Color_e DisplayBorderColor);
	uint32_t DLPC34XX_DUAL_ReadBorderColor(DLPC34XX_DUAL_Color_e *DisplayBorderColor, DLPC34XX_DUAL_BorderColorSource_e *PillarBoxBorderColorSource);
	uint32_t DLPC34XX_DUAL_WriteSolidField(DLPC34XX_DUAL_BorderEnable_e Border, DLPC34XX_DUAL_Color_e ForegroundColor);
	uint32_t DLPC34XX_DUAL_WriteHorizontalRamp(DLPC34XX_DUAL_BorderEnable_e Border, DLPC34XX_DUAL_Color_e ForegroundColor, uint8_t StartValue, uint8_t EndValue);
	uint32_t DLPC34XX_DUAL_WriteVerticalRamp(DLPC34XX_DUAL_BorderEnable_e Border, DLPC34XX_DUAL_Color_e ForegroundColor, uint8_t StartValue, uint8_t EndValue);
	uint32_t DLPC34XX_DUAL_WriteHorizontalLines(DLPC34XX_DUAL_HorizontalLines_s *HorizontalLines);
	uint32_t DLPC34XX_DUAL_WriteDiagonalLines(DLPC34XX_DUAL_DiagonalLines_s *DiagonalLines);
	uint32_t DLPC34XX_DUAL_WriteVerticalLines(DLPC34XX_DUAL_VerticalLines_s *VerticalLines);
	uint32_t DLPC34XX_DUAL_WriteGridLines(DLPC34XX_DUAL_GridLines_s *GridLines);
	uint32_t DLPC34XX_DUAL_WriteCheckerboard(DLPC34XX_DUAL_Checkerboard_s *Checkerboard);
	uint32_t DLPC34XX_DUAL_WriteColorbars(DLPC34XX_DUAL_BorderEnable_e Border);
	uint32_t DLPC34XX_DUAL_ReadTestPatternSelect(DLPC34XX_DUAL_TestPatternSelect_s *TestPatternSelect);
	uint32_t DLPC34XX_DUAL_WriteExecuteFlashBatchFile(uint8_t BatchFileNumber);
	uint32_t DLPC34XX_DUAL_WriteBatchFileDelay(uint16_t DelayInMicroseconds);
	uint32_t DLPC34XX_DUAL_WriteLedOutputControlMethod(DLPC34XX_DUAL_LedControlMethod_e LedControlMethod);
	uint32_t DLPC34XX_DUAL_ReadLedOutputControlMethod(DLPC34XX_DUAL_LedControlMethod_e *LedControlMethod);
	uint32_t DLPC34XX_DUAL_WriteRgbLedEnable(bool RedLedEnable, bool GreenLedEnable, bool BlueLedEnable);
	uint32_t DLPC34XX_DUAL_ReadRgbLedEnable(bool *RedLedEnable, bool *GreenLedEnable, bool *BlueLedEnable);
	uint32_t DLPC34XX_DUAL_WriteRgbLedCurrent(uint16_t RedLedCurrent, uint16_t GreenLedCurrent, uint16_t BlueLedCurrent);
	uint32_t DLPC34XX_DUAL_ReadRgbLedCurrent(uint16_t *RedLedCurrent, uint16_t *GreenLedCurrent, uint16_t *BlueLedCurrent);
	uint32_t DLPC34XX_DUAL_ReadCaicLedMaxAvailablePower(double *MaxLedPower);
	uint32_t DLPC34XX_DUAL_WriteRgbLedMaxCurrent(uint16_t MaxRedLedCurrent, uint16_t MaxGreenLedCurrent, uint16_t MaxBlueLedCurrent);
	uint32_t DLPC34XX_DUAL_ReadRgbLedMaxCurrent(uint16_t *MaxRedLedCurrent, uint16_t *MaxGreenLedCurrent, uint16_t *MaxBlueLedCurrent);
	uint32_t DLPC34XX_DUAL_ReadCaicRgbLedCurrent(uint16_t *RedLedCurrent, uint16_t *GreenLedCurrent, uint16_t *BlueLedCurrent);
	uint32_t DLPC34XX_DUAL_WriteLookSelect(uint8_t LookNumber);
	uint32_t DLPC34XX_DUAL_ReadLookSelect(uint8_t *LookNumber, uint8_t *SequenceIndex, double *SequenceFrameTime);
	uint32_t DLPC34XX_DUAL_ReadSequenceHeaderAttributes(DLPC34XX_DUAL_SequenceHeaderAttributes_s *SequenceHeaderAttributes);
	uint32_t DLPC34XX_DUAL_WriteLocalAreaBrightnessBoostControl(DLPC34XX_DUAL_LabbControl_e LabbControl, uint8_t SharpnessStrength, uint8_t LabbStrengthSetting);
	uint32_t DLPC34XX_DUAL_ReadLocalAreaBrightnessBoostControl(DLPC34XX_DUAL_LabbControl_e *LabbControl, uint8_t *SharpnessStrength, uint8_t *LabbStrengthSetting, uint8_t *LabbGainValue);
	uint32_t DLPC34XX_DUAL_WriteCaicImageProcessingControl(DLPC34XX_DUAL_CaicGainDisplayScale_e CaicGainDisplayScale, bool CaicGainDisplayEnable, double CaicMaxLumensGain, double CaicClippingThreshold);
	uint32_t DLPC34XX_DUAL_ReadCaicImageProcessingControl(DLPC34XX_DUAL_CaicGainDisplayScale_e *CaicGainDisplayScale, bool *CaicGainDisplayEnable, double *CaicMaxLumensGain, double *CaicClippingThreshold);
	uint32_t DLPC34XX_DUAL_WriteColorCoordinateAdjustmentControl(bool CcaEnable);
	uint32_t DLPC34XX_DUAL_ReadColorCoordinateAdjustmentControl(bool *CcaEnable);
	uint32_t DLPC34XX_DUAL_ReadShortStatus(DLPC34XX_DUAL_ShortStatus_s *ShortStatus);
	uint32_t DLPC34XX_DUAL_ReadSystemStatus(DLPC34XX_DUAL_SystemStatus_s *SystemStatus);
	uint32_t DLPC34XX_DUAL_ReadCommunicationStatus(DLPC34XX_DUAL_CommunicationStatus_s *CommunicationStatus);
	uint32_t DLPC34XX_DUAL_ReadSystemSoftwareVersion(uint16_t *PatchVersion, uint8_t *MinorVersion, uint8_t *MajorVersion);
	uint32_t DLPC34XX_DUAL_ReadControllerDeviceId(DLPC34XX_DUAL_ControllerDeviceId_e *DeviceId);
	uint32_t DLPC34XX_DUAL_ReadDmdDeviceId(DLPC34XX_DUAL_DmdDataSelection_e DmdDataSelection, uint32_t *DeviceId);
	uint32_t DLPC34XX_DUAL_ReadFirmwareBuildVersion(uint16_t *PatchVersion, uint8_t *MinorVersion, uint8_t *MajorVersion);
	uint32_t DLPC34XX_DUAL_ReadSystemTemperature(double *Temperature);
	uint32_t DLPC34XX_DUAL_ReadFlashUpdatePrecheck(uint32_t FlashUpdatePackageSize, DLPC34XX_DUAL_Error_e *PackageSizeStatus, DLPC34XX_DUAL_Error_e *PacakgeConfigurationCollapsed, DLPC34XX_DUAL_Error_e *PacakgeConfigurationIdentifier);
	uint32_t DLPC34XX_DUAL_WriteFlashDataTypeSelect(DLPC34XX_DUAL_FlashDataTypeSelect_e FlashSelect);
	uint32_t DLPC34XX_DUAL_WriteFlashDataLength(uint16_t FlashDataLength);
	uint32_t DLPC34XX_DUAL_WriteFlashErase();
	uint32_t DLPC34XX_DUAL_WriteFlashStart(uint16_t DataLength, uint8_t* Data);
	uint32_t DLPC34XX_DUAL_ReadFlashStart(uint16_t Length, uint8_t *Data);
	uint32_t DLPC34XX_DUAL_WriteFlashContinue(uint16_t DataLength, uint8_t* Data);
	uint32_t DLPC34XX_DUAL_ReadFlashContinue(uint16_t Length, uint8_t *Data);
	uint32_t DLPC34XX_DUAL_ReadSequenceBinaryVersion(uint8_t *PatchVersion, uint8_t *MinorVersion, uint8_t *MajorVersion);
	uint32_t DLPC34XX_DUAL_WriteInternalPatternControl(DLPC34XX_DUAL_PatternControl_e PatternControl, uint8_t RepeatCount);
	uint32_t DLPC34XX_DUAL_ReadValidateExposureTime(DLPC34XX_DUAL_PatternMode_e PatternMode, DLPC34XX_DUAL_SequenceType_e BitDepth, uint32_t ExposureTime, DLPC34XX_DUAL_ValidateExposureTime_s *ValidateExposureTime);
	uint32_t DLPC34XX_DUAL_WriteTriggerInConfiguration(DLPC34XX_DUAL_TriggerEnable_e TriggerEnable, DLPC34XX_DUAL_TriggerPolarity_e TriggerPolarity);
	uint32_t DLPC34XX_DUAL_ReadTriggerInConfiguration(DLPC34XX_DUAL_TriggerEnable_e *TriggerEnable, DLPC34XX_DUAL_TriggerPolarity_e *TriggerPolarity);
	uint32_t DLPC34XX_DUAL_WriteTriggerOutConfiguration(DLPC34XX_DUAL_TriggerType_e TriggerType, DLPC34XX_DUAL_TriggerEnable_e TriggerEnable, DLPC34XX_DUAL_TriggerInversion_e TriggerInversion, int32_t Delay);
	uint32_t DLPC34XX_DUAL_ReadTriggerOutConfiguration(DLPC34XX_DUAL_TriggerType_e Trigger, DLPC34XX_DUAL_TriggerEnable_e *TriggerEnable, DLPC34XX_DUAL_TriggerInversion_e *TriggerInversion, int32_t *Delay);
	uint32_t DLPC34XX_DUAL_WritePatternReadyConfiguration(DLPC34XX_DUAL_TriggerEnable_e TriggerEnable, DLPC34XX_DUAL_TriggerPolarity_e TriggerPolarity);
	uint32_t DLPC34XX_DUAL_ReadPatternReadyConfiguration(DLPC34XX_DUAL_TriggerEnable_e *TriggerEnable, DLPC34XX_DUAL_TriggerPolarity_e *TriggerPolarity);
	uint32_t DLPC34XX_DUAL_WritePatternConfiguration(DLPC34XX_DUAL_PatternConfiguration_s *PatternConfiguration);
	uint32_t DLPC34XX_DUAL_ReadPatternConfiguration(DLPC34XX_DUAL_PatternConfiguration_s *PatternConfiguration);
	uint32_t DLPC34XX_DUAL_WritePatternOrderTableEntry(DLPC34XX_DUAL_WriteControl_e WriteControl, DLPC34XX_DUAL_PatternOrderTableEntry_s *PatternOrderTableEntry);
	uint32_t DLPC34XX_DUAL_ReadPatternOrderTableEntry(uint8_t PatternOrderTableEntryIndex, DLPC34XX_DUAL_PatternOrderTableEntry_s *PatternOrderTableEntry);
	uint32_t DLPC34XX_DUAL_ReadInternalPatternStatus(DLPC34XX_DUAL_InternalPatternStatus_s *InternalPatternStatus);

};
//-------------------------------------------------------------------------------------//
#endif//LIGHT_3D_TI_DLP_4710_USE
#endif // !defined(AFX_LIGHT3DTIDLP4710_H__091F6483_6B4F_4505_A49B_46CC67AA8BE4__INCLUDED_)
