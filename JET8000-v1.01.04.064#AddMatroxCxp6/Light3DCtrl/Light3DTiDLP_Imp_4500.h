// Light3DTiDLP_Imp_4500.h: interface for the CLight3DTiDLP_Imp_4500 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHT3DTIDLP_IMP_4500_V3_H__54288060_6D69_4E38_A8BE_4C1DF39F9E6F__INCLUDED_)
#define AFX_LIGHT3DTIDLP_IMP_4500_V3_H__54288060_6D69_4E38_A8BE_4C1DF39F9E6F__INCLUDED_
//-------------------------------------------------------------------------------------//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_USE_IMP_4500
//-------------------------------------------------------------------------------------//
#include "DLPC350_3_1_0\\dlpc350_common.h"
#include "DLPC350_3_1_0\\dlpc350_api.h"
//-------------------------------------------------------------------------------------//
#include "Light3DTiDLP_Imp.h"
#include "Light3DTiDLP_Imp_4500Usb.h"
#include "Light3DTiDLP_Imp_4500Frmw.h"
//-------------------------------------------------------------------------------------//
class CLight3DTiDLP_Imp_4500 : public CLight3DTiDLP_Imp
{
private:
	//---------------------------------------------------------------------------------//
	CLight3DTiDLP_Imp_4500Usb  m_TiUSB;//Ti-USB連線
	CLight3DTiDLP_Imp_4500Frmw m_TiFrmw;//Ti-韌體
	//---------------------------------------------------------------------------------//
	unsigned char              m_HWStatus;///硬體狀態
	unsigned char              m_SysStatus;//系統狀態
	unsigned char              m_MainStatus;//主要狀態
	bool                       m_EnableTemperatureMonitor;
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
	int                        m_PatternBitCount;//樣板的位元數
	int                        m_PatternIndex1_3Bit;
	int                        m_PatternIndex2_3Bit;
	int                        m_PatternIndex1_5Bit;
	int                        m_PatternIndex1_6Bit;
	int                        m_PatternIndex2_6Bit;
	int                        m_PatternIndexGC_1Bit;//GrayCode	
	int                        m_PatternIndexBC_1Bit;//BinaryCode	
	int                        m_PatternStartNumGC_1Bit;//GrayCode
	int                        m_PatternStartNumBC_1Bit;//BinaryCode
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CLight3DTiDLP_Imp_4500(const CLight3DTiDLP_Imp_4500 &DlpCtrl);
	CLight3DTiDLP_Imp_4500& operator=(const CLight3DTiDLP_Imp_4500 &DlpCtrl);
	//---------------------------------------------------------------------------------//
	void                       PreInitTiDlp(int CtrlID, LIGHT_3D_CAST_ID CastID);
	void                       InitialTiDlp();
	//---------------------------------------------------------------------------------//
	bool                       GetUSB_Number(LIGHT_3D_CAST_ID CastID, wchar_t USB_Number[]);
	//---------------------------------------------------------------------------------//
	bool                       CheckDLPPatternIndex(int index, unsigned int Count, int Mode);
	bool                       BuildDLPPatternList_TestGC(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-測試GC
	bool                       BuildDLPPatternList_White(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-白燈
	bool                       BuildDLPPatternList_RGB(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-紅綠藍燈
	bool                       BuildDLPPatternList_2X2_M(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-22M
	bool                       BuildDLPPatternList_4X4_1(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-441
	bool                       BuildDLPPatternList_4X4_2(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-442
	bool                       BuildDLPPatternList_4X2_M(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-42M
	bool                       BuildDLPPatternList_4X4_M(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-44M
	bool                       BuildDLPPatternList_4X4GC_M(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-4X4GCM
	bool                       BuildDLPPatternList_4X5GC_M(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-4X5GCM
	bool                       BuildDLPPatternList_4X6GC_M(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-4X6GCM
	bool                       BuildDLPPatternList_4X4GC_M2(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-4X4GCM2
	bool                       BuildDLPPatternList_4X43GC_M(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-4X4-3GCM
	bool                       BuildDLPPatternList_4X2_M2(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-42M2
	bool                       BuildDLPPatternList_4X4_M2(bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表-44M2
	//---------------------------------------------------------------------------------//
	bool                       ResetLEDDisable(bool &ResetFinish, bool ShowMsg=true);
	bool                       GetGPIOStatus(UINT PinNum, bool &Status);	//取得GPIO pin 狀態
	bool                       SetGPIOStatus(UINT PinNum, bool Status);	//取得GPIO pin output 狀態
	bool                       GetGPIOTemperatureOver(bool &IsOver, bool ShowMsg);	//偵測溫度狀態。		GPIO11 input狀態。high高溫/low低溫
	bool                       GetGPIOLEDDisable(bool &IsDisable, bool ShowMsg);	//LED是否disable狀態。	GPIO5  input狀態。high disable/low disable已解除
	bool                       SetGPIOLEDEnable();//LED disable狀態解除。	GPIO6  output狀態。 low->hi		
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CLight3DTiDLP_Imp_4500();
	CLight3DTiDLP_Imp_4500(int CtrlID, LIGHT_3D_CAST_ID CastID);
	virtual ~CLight3DTiDLP_Imp_4500();
	//---------------------------------------------------------------------------------//
	bool                       DLPConnect();
	bool                       DLPDisconnect(int WaitTime_ms=0);
	bool                       GetDLPIsConnected();	
	bool                       CheckDLPFrmForExpLut();//確認DLP韌體支援Exposure Lut
	//---------------------------------------------------------------------------------//
	bool                       ExecDLPSoftwareReset(DWORD delayTime);
	//---------------------------------------------------------------------------------//
	bool                       SetDLPLongAxisImageFlip(bool Flip);
	bool                       GetDLPLongAxisImageFlip();
	//---------------------------------------------------------------------------------//
	bool                       SetDLPShortAxisImageFlip(bool Flip);
	bool                       GetDLPShortAxisImageFlip();
	//---------------------------------------------------------------------------------//
	bool                       SetDLPOperationMode(int Mode);	
	int                        GetDLPOperationMode() const;	
	//---------------------------------------------------------------------------------//
	bool                       SetDLPLEDEnable(bool bSeqCtrl, bool bRed, bool bGreen , bool bBlue);
	bool                       GetDLPLEDEnable(bool &bSeqCtrl, bool &bRed, bool &bGreen , bool &bBlue);
	//---------------------------------------------------------------------------------//	
	bool                       SetDLPLEDCurrent(int red, int green, int blue, bool Update, int CurrentID);
	bool                       GetDLPLEDCurrent(int &red, int &green, int &blue);	
	//---------------------------------------------------------------------------------//	
	bool                       SetDLPLEDPWMInvert(bool bInvert);
	bool                       GetDLPLEDPWMInvert(bool &bInvert);
	//---------------------------------------------------------------------------------//
	bool                       CheckDLPStatus();
	bool                       GetDLPStatus_InitDone();
	bool                       GetDLPStatus_ForcedSwap();
	bool                       GetDLPStatus_BufferFreeze();
	bool                       GetDLPStatus_SeqRunning();
	bool                       GetDLPStatus_SeqError();
	bool                       GetDLPStatus_SeqAbort();
	bool                       GetDLPStatus_DRCError();
	bool                       GetDLPStatus_DMDParked();	
	//---------------------------------------------------------------------------------//
	bool                       ReadDLPParameter();//從DLP裝置讀取參數
	//---------------------------------------------------------------------------------//	
	int                        GetPatternBitCount() const;//取得樣板圖位元數
	void                       SetPatternBitCount(int val);//設定樣板圖位元數
	//---------------------------------------------------------------------------------//
	int                        GetPatternIndex1_3Bit() const;//取得使用3Bit樣板引數-1
	void                       SetPatternIndex1_3Bit(int val);//設定使用3Bit樣板引數-1
	int                        GetPatternIndex2_3Bit() const;//取得使用3Bit樣板引數-2
	void                       SetPatternIndex2_3Bit(int val);//設定使用3Bit樣板引數-2
	int                        GetPatternIndex1_5Bit() const;//取得使用5Bit樣板引數-1
	void                       SetPatternIndex1_5Bit(int val);//設定使用5Bit樣板引數-1
	int                        GetPatternIndex1_6Bit() const;//取得使用6Bit樣板引數-1
	void                       SetPatternIndex1_6Bit(int val);//設定使用6Bit樣板引數-1
	int                        GetPatternIndex2_6Bit() const;//取得使用6Bit樣板引數-2
	void                       SetPatternIndex2_6Bit(int val);//設定使用6Bit樣板引數-2
	int                        GetPatternIndexGC_1Bit() const;//取得使用1Bit-GrayCode引數-1
	void                       SetPatternIndexGC_1Bit(int val);//設定使用1Bit-GrayCode引數-1
	int                        GetPatternIndexBC_1Bit() const;//取得使用1Bit-BinaryCode引數-1
	void                       SetPatternIndexBC_1Bit(int val);//設定使用1Bit-BinaryCode引數-1
	int                        GetPatternStartNumGC_1Bit() const;//取得使用1Bit-GrayCode起始張數-1
	void                       SetPatternStartNumGC_1Bit(int val);//設定使用1Bit-GrayCode起始張數-1
	int                        GetPatternStartNumBC_1Bit() const;//取得使用1Bit-BinaryCode起始張數-1
	void                       SetPatternStartNumBC_1Bit(int val);//設定使用1Bit-BinaryCode起始張數-1
	//---------------------------------------------------------------------------------//
	bool                       ExecDLPPattern_Run();	
	bool                       ExecDLPPattern_Stop();
	bool                       ExecDLPPattern_Pause();
	bool                       LEDSetting(int LEDCurrent=-1, int CurrentID=DLP_LED_CURRENT_ID_01);	
	//---------------------------------------------------------------------------------//
	bool                       BuildDLPPatternList(int Mode, bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表 	
	//---------------------------------------------------------------------------------//	
	bool                       ExecDLPPatRead(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat);//讀取DLP樣版內容
	bool                       ExecDLPPatRead_Lut(unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat);//讀取DLP樣版內容
	bool                       ExecDLPPatRead_ExpLut(unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat);//讀取DLP樣版內容

	bool                       ExecDLPPatSendAll(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新樣板至DLP
	bool                       ExecDLPPatSendAll_Lut(unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新樣板至DLP
	bool                       ExecDLPPatSendAll_ExpLut(unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新樣板至DLP

	bool                       ExecDLPPatSendOne(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新單一樣板至DLP
	bool                       ExecDLPPatSendOne_Lut(int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新單一樣板至DLP
	bool                       ExecDLPPatSendOne_ExpLut(int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新單一樣板至DLP

	bool                       ExecDLPValidatePatLutData(unsigned int &Status, bool Sleep);//套用樣板列表資料
	bool                       ExecDLPPatBuildSendValidate(int Mode, bool IntTrig, bool MultiTable, unsigned int Periodus, unsigned int exposure_us, int LEDColor, bool Sleep, bool Force);//建立樣板, 傳送樣板以及驗證

	bool                       ExecDLPLightSetting(int CurrentID);//執行DLP的LED設定-依據目前的設定
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
	int  DLPC350_SetDisplay(rectangle croppedArea, rectangle displayArea);
	int  DLPC350_GetDisplay(rectangle *pCroppedArea, rectangle *pDisplayArea);
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
#endif//LIGHT_3D_TI_DLP_USE_IMP_4500
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_LIGHT3DTIDLP_IMP_4500_V3_H__54288060_6D69_4E38_A8BE_4C1DF39F9E6F__INCLUDED_)
