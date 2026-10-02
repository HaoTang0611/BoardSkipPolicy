// Light3DTiDLP_Imp_4710.h: interface for the CLight3DTiDLP_Imp_4710 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHT3DTIDLP_IMP_4710_H__091F6483_6B4F_4505_A49B_46CC67AA8BE4__INCLUDED_)
#define AFX_LIGHT3DTIDLP_IMP_4710_H__091F6483_6B4F_4505_A49B_46CC67AA8BE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Light3DDef.h"
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_USE_IMP_4710
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
//-------------------------------------------------------------------------------------//
class CLight3DTiDLP_Imp_4710 : public CLight3DTiDLP_Imp
{
public:		
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//				
	int                        m_PatternBitCount;//樣板的位元數
	bool                       m_EnableTemperatureMonitor;	
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
	//---------------------------------------------------------------------------------//	
	DLPC34XX_DUAL_ShortStatus_s  m_ShortStatus;//簡約狀態
	DLPC34XX_DUAL_SystemStatus_s m_SystemStatus;//系統狀態
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
	CLight3DTiDLP_Imp_4710(const CLight3DTiDLP_Imp_4710 &DlpCtrl);
	CLight3DTiDLP_Imp_4710& operator=(const CLight3DTiDLP_Imp_4710 &DlpCtrl);
	//---------------------------------------------------------------------------------//
	void                          PreInitTiDlp(int CtrlID, LIGHT_3D_CAST_ID CastID);
	void                          InitialTiDlp();
	//---------------------------------------------------------------------------------//
	bool                          GetDlpMonoMode() const;//單燈源模式
	//---------------------------------------------------------------------------------//	
	void                          SetLCRErrorFnName(LPCTSTR LCRFnName);	
	//---------------------------------------------------------------------------------//	
	bool                          GetUSB_Number(LIGHT_3D_CAST_ID CastID, char USB_Number[]);
	bool                          GetUSB_Number(LIGHT_3D_CAST_ID CastID, wchar_t USB_Number[]);
	//---------------------------------------------------------------------------------//
	bool                          ReturnNotSupportFunc(const TCHAR *fnName);
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
	int                           GetDLPTrigType(int index, bool IntTrig, bool MultiTable);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CLight3DTiDLP_Imp_4710();
	CLight3DTiDLP_Imp_4710(int CtrlID, LIGHT_3D_CAST_ID CastID);
	virtual ~CLight3DTiDLP_Imp_4710();
	//---------------------------------------------------------------------------------//
	LPCTSTR                       GetDLPProjectName();
	//---------------------------------------------------------------------------------//
	bool                          DLPConnect();
	bool                          DLPDisconnect(int WaitTime_ms=0);
	bool                          GetDLPIsConnected();	
	bool                          CheckDLPFrmForExpLut();//確認DLP韌體支援Exposure Lut
	//---------------------------------------------------------------------------------//	
	bool                          ExecDLPSoftwareReset(DWORD delayTime);
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
	bool                          ReadDLPParameter();//從DLP裝置讀取參數
	//---------------------------------------------------------------------------------//
	bool                          GetDLPReadySignalEnable() const;
	//---------------------------------------------------------------------------------//
	bool                          GetUseExpLut();	
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
	//---------------------------------------------------------------------------------//
	int                           CalcPeriodPaddingTime(int ExpTime) const;//計算週期外加時間-us
	//---------------------------------------------------------------------------------//	
	bool                          ExecDLPPattern_Run();	
	bool                          ExecDLPPattern_Stop();
	bool                          ExecDLPPattern_Pause();
	bool                          LEDSetting(int LEDCurrent=-1, int CurrentID=DLP_LED_CURRENT_ID_01);		
	//---------------------------------------------------------------------------------//
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
	bool                          BuildDLPPatternList_4X43GC_M(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-4X4-3GCM
	bool                          BuildDLPPatternList_4X2_M2(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-42M2
	bool                          BuildDLPPatternList_4X4_M2(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-44M2
	//---------------------------------------------------------------------------------//	
	bool                          ExecDLPPatClear();//清除樣版內容
	bool                          ExecDLPPatRead(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat);//讀取DLP樣版內容	
	bool                          ExecDLPPatSendAll(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新樣板至DLP
	bool                          ExecDLPPatSendOne(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新單一樣板至DLP	

	bool                          ExecDLPValidatePatLutData(unsigned int &Status, bool Sleep);//套用樣板列表資料
	bool                          ExecDLPPatBuildSendValidate(int Mode, bool IntTrig, bool MultiTable, unsigned int Periodus, unsigned int exposure_us, int LEDColor, bool Sleep, bool Force);//建立樣板, 傳送樣板以及驗證

	bool                          ExecDLPLightSetting(int CurrentID);//執行DLP的LED設定-依據目前的設定
	//---------------------------------------------------------------------------------//	
	int                           GetDLPCurrentMax() const;//取得DLP電流上限
	bool                          GetLEDColorUsed_Red() const;//取得LED顏色使用-紅色
	bool                          GetLEDColorUsed_Grn() const;//取得LED顏色使用-綠色
	bool                          GetLEDColorUsed_Blu() const;//取得LED顏色使用-藍色
	//---------------------------------------------------------------------------------//
	bool                          ResetLEDDisable(bool &ResetFinish, bool ShowMsg=true);
	bool                          GetGPIOStatus(UINT PinNum, bool &Status);	//取得GPIO pin 狀態
	bool                          SetGPIOStatus(UINT PinNum, bool Status);	//取得GPIO pin output 狀態
	bool                          GetGPIOTemperatureOver(bool &IsOver, bool ShowMsg);	//偵測溫度狀態。		GPIO11 input狀態。high高溫/low低溫
	bool                          GetGPIOLEDDisable(bool &IsDisable, bool ShowMsg);	//LED是否disable狀態。	GPIO5  input狀態。high disable/low disable已解除
	bool                          SetGPIOLEDEnable();//LED disable狀態解除。	GPIO6  output狀態。 low->hi		
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
#endif//LIGHT_3D_TI_DLP_USE_IMP_4710
#endif // !defined(AFX_LIGHT3DTIDLP_IMP_4710_H__091F6483_6B4F_4505_A49B_46CC67AA8BE4__INCLUDED_)
