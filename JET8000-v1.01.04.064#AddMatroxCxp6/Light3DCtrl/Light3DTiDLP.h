// Light3DTiDLP.h: interface for the CLight3DTiDLP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHT3DTIDLP_H__5B3FDFC8_FE39_47B8_8568_65C7F2A64A2F__INCLUDED_)
#define AFX_LIGHT3DTIDLP_H__5B3FDFC8_FE39_47B8_8568_65C7F2A64A2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Light3DDef.h"
#ifdef LIGHT_3D_TI_DLP_IMP_USE
#include "Light3DTiDLPDef.h"
#include "Light3DTiDLP_Imp.h"
//-------------------------------------------------------------------------------------//
class CLight3DTiDLP  
{
private:	
	//---------------------------------------------------------------------------------//		
	CLight3DTiDLP_Imp         *m_Imp;
	TDLPParam                  m_DLPParamDummy;		
	CString                    m_ErrorString;		
	//---------------------------------------------------------------------------------//		
protected:	
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	CLight3DTiDLP(const CLight3DTiDLP &DlpCtrl);
	CLight3DTiDLP& operator=(const CLight3DTiDLP &DlpCtrl);
	//---------------------------------------------------------------------------------//
	void                       PreInitTiDlp(int CtrlID, LIGHT_3D_CAST_ID CastID);
	void                       InitialTiDlp();
	//---------------------------------------------------------------------------------//
	void                       LockLight3D();
	void                       UnlockLight3D();
	//---------------------------------------------------------------------------------//
	const CLight3DTiDLP_Imp*   GetImp() const;//取得Imp的指標	
	bool                       CheckImp();//確認Imp的指標	
	bool                       CheckImp() const;//確認Imp的指標	
	bool                       CreateImp(int CtrlID, LIGHT_3D_CAST_ID CastID);//建立Imp的指標	
	bool                       CreateImp_DLP4500(int CtrlID, LIGHT_3D_CAST_ID CastID);//建立Imp的指標
	bool                       CreateImp_DLP4710(int CtrlID, LIGHT_3D_CAST_ID CastID);//建立Imp的指標
	bool                       ReleaseImp();//釋放Imp的指標	
	//---------------------------------------------------------------------------------//	
	CString                    GetIniSectionName(LIGHT_3D_CAST_ID CastID);
	LIGHT_3D_DEVICE_TYPE       LoadDeviceType(LIGHT_3D_CAST_ID CastID);//載入裝置型號	
	bool                       GetUSB_Number(LIGHT_3D_CAST_ID CastID, wchar_t USB_Number[]);
	//---------------------------------------------------------------------------------//
	int                        GetDLPSafeCurrent(int value);	
	//---------------------------------------------------------------------------------//
	bool                       ClearPhaseZeroBufferFn();//清除平面相位		
	//---------------------------------------------------------------------------------//
	int                        GetDLPTrigType(int index, bool IntTrig, bool MultiTable);
	//---------------------------------------------------------------------------------//	
	bool                       LoadDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE &PhaseW, IMAGE_SIZE &PhaseH, IMAGE_SIZE &PhaseStep, PHASE_PTR &PhasePtr);//載入DLP平面相位	
	//---------------------------------------------------------------------------------//	
	bool                       LoadDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE &SpaceW, IMAGE_SIZE &SpaceH, IMAGE_SIZE &SpaceStep, SPACE_PTR &SpacePtr);//載入DLP平面係數	
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	CLight3DTiDLP();
	CLight3DTiDLP(int CtrlID, LIGHT_3D_CAST_ID CastID);
	virtual ~CLight3DTiDLP();
	//---------------------------------------------------------------------------------//	
	void                       SetDLPID(int ID);
	int                        GetDLPID();
	//---------------------------------------------------------------------------------//		
	LIGHT_3D_CAST_ID           GetCastID();
	LIGHT_3D_DEVICE_TYPE       GetDeviceType();	
	bool                       ChangeDeviceType(LIGHT_3D_DEVICE_TYPE Type);//變更裝置型號
	//---------------------------------------------------------------------------------//		
	LPCTSTR                    GetDLPProjectName();
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString();
	//---------------------------------------------------------------------------------//
	unsigned int               GetDLPImageW();
	unsigned int               GetDLPImageH();
	//---------------------------------------------------------------------------------//
	bool                       DLPConnect();
	bool                       DLPDisconnect(int WaitTime_ms=0);
	bool                       GetDLPIsConnected();	
	bool                       CheckDLPFrmForExpLut();//確認DLP韌體支援Exposure Lut
	//---------------------------------------------------------------------------------//
	bool                       SaveDLPProcess(LPCTSTR fnName, int Level);
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
	int                        GetDLPOperationMode();	
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
	bool                       InitialDLPParameter(TDLPParam &Param);
	bool                       SaveDLPParameter();//寫參數至檔案
	bool                       LoadDLPParameter();//從檔案讀取參數
	bool                       ReadDLPParameter();//從DLP裝置讀取參數
	TDLPParam&                 GetDLPParam();	
	bool                       SetDLPPhaseMode(int Mode);
	int                        GetDLPPhaseMode();
	int                        GetDLPExposureTime();
	bool                       GetDLPReadySignalEnable();
	//---------------------------------------------------------------------------------//
	bool                       SetDLPParamLEDCurrent(int red, int green, int blue, int CurrentID);
	bool                       GetDLPParamLEDCurrent(int &red, int &green, int &blue, int CurrentID);
	//---------------------------------------------------------------------------------//
	const char*                GetDLPFrmTag();
	const char*                GetTiAPIVersion();	
	const char*                GetDLPFrmVersion();	
	const char*                GetDLPMcuVersion();
	//---------------------------------------------------------------------------------//
	bool                       GetUseExpLut();
	bool                       SetLEDColor(int Type);
	int                        GetLEDColor();	
	//---------------------------------------------------------------------------------//
	double                     GetImageGamma();
	bool                       SetImageGamma(double val);
	//---------------------------------------------------------------------------------//
	double                     GetSecondExpRatio();//取得第2次曝光比例
	bool                       SetSecondExpRatio(double val);
	//---------------------------------------------------------------------------------//
	int                        GetPeriodPaddingTime();//週期外加時間-us
	int                        GetExposurePaddingTime();//曝光外加時間-us
	int                        CalcPeriodPaddingTime(int ExpTime);//計算週期外加時間-us
	//---------------------------------------------------------------------------------//
	bool                       GetUse3BitPattern();//取得使用3Bit樣板圖
	bool                       GetUse5BitPattern();//取得使用5Bit樣板圖
	//---------------------------------------------------------------------------------//	
	int                        GetPatternBitCount();//取得樣板圖位元數
	void                       SetPatternBitCount(int val);//設定樣板圖位元數
	//---------------------------------------------------------------------------------//
	int                        GetPatternIndex1_3Bit();//取得使用3Bit樣板引數-1
	void                       SetPatternIndex1_3Bit(int val);//設定使用3Bit樣板引數-1
	int                        GetPatternIndex2_3Bit();//取得使用3Bit樣板引數-2
	void                       SetPatternIndex2_3Bit(int val);//設定使用3Bit樣板引數-2
	int                        GetPatternIndex1_5Bit();//取得使用5Bit樣板引數-1
	void                       SetPatternIndex1_5Bit(int val);//設定使用5Bit樣板引數-1
	int                        GetPatternIndex1_6Bit();//取得使用6Bit樣板引數-1
	void                       SetPatternIndex1_6Bit(int val);//設定使用6Bit樣板引數-1
	int                        GetPatternIndex2_6Bit();//取得使用6Bit樣板引數-2
	void                       SetPatternIndex2_6Bit(int val);//設定使用6Bit樣板引數-2
	int                        GetPatternIndexGC_1Bit();//取得使用1Bit-GrayCode引數-1
	void                       SetPatternIndexGC_1Bit(int val);//設定使用1Bit-GrayCode引數-1
	int                        GetPatternIndexBC_1Bit();//取得使用1Bit-BinaryCode引數-1
	void                       SetPatternIndexBC_1Bit(int val);//設定使用1Bit-BinaryCode引數-1
	int                        GetPatternStartNumGC_1Bit();//取得使用1Bit-GrayCode起始張數-1
	void                       SetPatternStartNumGC_1Bit(int val);//設定使用1Bit-GrayCode起始張數-1
	int                        GetPatternStartNumBC_1Bit();//取得使用1Bit-BinaryCode起始張數-1
	void                       SetPatternStartNumBC_1Bit(int val);//設定使用1Bit-BinaryCode起始張數-1
	//---------------------------------------------------------------------------------//
	void                       SetPeriod_us(unsigned int val);
	void                       SetExposure_us(unsigned int val);	
	void                       SetPeriod2_us(unsigned int val);
	void                       SetExposure2_us(unsigned int val);
	//---------------------------------------------------------------------------------//
	void                       InitialPatItem(TDLPPatItem &PatItem);		
	//---------------------------------------------------------------------------------//
	bool                       ExecDLPPattern_Run();	
	bool                       ExecDLPPattern_Stop();
	bool                       ExecDLPPattern_Pause();
	bool                       LEDSetting(int LEDCurrent=-1, int CurrentID=DLP_LED_CURRENT_ID_01);	
	size_t                     GetTriggerOutCount();
	//---------------------------------------------------------------------------------//
	size_t                     GetDLPPatCount();
	TDLPPatItem*               GetDLPPatItemPtr(size_t index, bool check);	
	void                       ClearDLPPatternList();//清除m_PatternList	
	bool                       BuildDLPPatternList(int Mode, bool IntTrig, bool MultiTable, int LEDColor);//建立樣板列表 
	bool                       AddDLPPPatItem(TDLPPatItem &PatItem);//增加樣板項目	
	bool                       RemoveDLPPatItem(size_t index);//移除樣板項目
	//---------------------------------------------------------------------------------//
	bool                       ExecDLPPatClear();//清除樣版內容
	bool                       ExecDLPPatRead(bool bExpLut, unsigned int &TrigPeriod_us, unsigned int &Exposure_us, bool &bPatFrmVideo, bool &bTrigIntExt, bool &bRepeat);//讀取DLP樣版內容
	bool                       ExecDLPPatSendAll(bool bExpLut, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新樣板至DLP	
	bool                       ExecDLPPatSendOne(bool bExpLut, int index, unsigned int TrigPeriod_us, unsigned int Exposure_us, bool bPatFrmVideo, bool bTrigIntExt, bool bRepeat);//更新單一樣板至DLP	

	bool                       ExecDLPValidatePatLutData(unsigned int &Status, bool Sleep);//套用樣板列表資料
	bool                       ExecDLPPatBuildSendValidate(int Mode, bool IntTrig, bool MultiTable, unsigned int Periodus, unsigned int exposure_us, int LEDColor, bool Sleep, bool Force);//建立樣板, 傳送樣板以及驗證

	bool                       ExecDLPLightSetting(int CurrentID);//執行DLP的LED設定-依據目前的設定
	//---------------------------------------------------------------------------------//
	bool                       BuildPatternImage(int BitDepth, int NPeriod, int NPixelPeriod, bool bVer, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage);
	//---------------------------------------------------------------------------------//	
	CString                    GetDLPPhaseModeTextForBinFile(int Mode);//取得相位模式的文字
	//---------------------------------------------------------------------------------//
	bool                       CheckPhaseZeroRed();//確認平面相位
	bool                       CheckPhaseZeroGrn();//確認平面相位
	bool                       CheckPhaseZeroBlu();//確認平面相位
	bool                       CheckPhaseZeroWhite();//確認平面相位
	bool                       CheckPhaseZeroDebug();//確認平面相位
	//---------------------------------------------------------------------------------//	
	bool                       LoadPhaseZero();//載入平面相位
	bool                       LoadPhaseZeroRed();//載入平面相位
	bool                       LoadPhaseZeroGrn();//載入平面相位
	bool                       LoadPhaseZeroBlu();//載入平面相位
	bool                       LoadPhaseZeroWhite();//載入平面相位
	bool                       LoadPhaseZeroDebug();//載入平面相位
	//---------------------------------------------------------------------------------//
	bool                       SavePhaseZero();//儲存平面相位
	bool                       SavePhaseZeroRed();//儲存平面相位
	bool                       SavePhaseZeroGrn();//儲存平面相位
	bool                       SavePhaseZeroBlu();//儲存平面相位
	bool                       SavePhaseZeroWhite();//儲存平面相位
	bool                       SavePhaseZeroDebug();//儲存平面相位
	//---------------------------------------------------------------------------------//
	bool                       ClearPhaseZeroBuffer();//清除平面相位	
	bool                       ClearPhaseZeroBufferRed();//清除平面相位	
	bool                       ClearPhaseZeroBufferGrn();//清除平面相位	
	bool                       ClearPhaseZeroBufferBlu();//清除平面相位	
	bool                       ClearPhaseZeroBufferWhite();//清除平面相位	
	bool                       ClearPhaseZeroBufferDebug();//清除平面相位		
	bool                       SetPhaseZero(int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, PHASE_PTR Ptr);//設定平面相位
	bool                       GetPhaseZero(int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr);//取得平面相位	
	bool                       ClonePhaseZero(int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr);//複製平面相位
	bool                       SaveDLPPhaseZeroFile(LPCTSTR pfilename, IMAGE_SIZE PhaseW, IMAGE_SIZE PhaseH, IMAGE_SIZE PhaseStep, const PHASE_PTR PhasePtr);//儲存DLP平面相位
	//---------------------------------------------------------------------------------//	
	bool                       CheckPhaseFactorRed();//確認平面係數
	bool                       CheckPhaseFactorGrn();//確認平面係數
	bool                       CheckPhaseFactorBlu();//確認平面係數
	bool                       CheckPhaseFactorWhite();//確認平面係數
	bool                       CheckPhaseFactorDebug();//確認平面係數
	//---------------------------------------------------------------------------------//	
	bool                       LoadPhaseFactor();//載入平面係數
	bool                       LoadPhaseFactorRed();//載入平面係數
	bool                       LoadPhaseFactorGrn();//載入平面係數
	bool                       LoadPhaseFactorBlu();//載入平面係數
	bool                       LoadPhaseFactorWhite();//載入平面係數
	bool                       LoadPhaseFactorDebug();//載入平面係數
	//---------------------------------------------------------------------------------//	
	bool                       SavePhaseFactor();//儲存平面係數
	bool                       SavePhaseFactorRed();//儲存平面係數
	bool                       SavePhaseFactorGrn();//儲存平面係數
	bool                       SavePhaseFactorBlu();//儲存平面係數
	bool                       SavePhaseFactorWhite();//儲存平面係數
	bool                       SavePhaseFactorDebug();//儲存平面係數
	//---------------------------------------------------------------------------------//	
	bool                       ClearPhaseFactorBuffer();//清除平面係數
	bool                       ClearPhaseFactorBufferRed();//清除平面係數
	bool                       ClearPhaseFactorBufferGrn();//清除平面係數
	bool                       ClearPhaseFactorBufferBlu();//清除平面係數
	bool                       ClearPhaseFactorBufferWhite();//清除平面係數
	bool                       ClearPhaseFactorBufferDebug();//清除平面係數
	//---------------------------------------------------------------------------------//		
	double                     GetPhaseFactorMin();//取得平面係數下限
	void                       SetPhaseFactorMin(double val);//設定平面係數下限
	double                     GetPhaseFactorMax();//取得平面係數上限
	void                       SetPhaseFactorMax(double val);//設定平面係數上限
	//---------------------------------------------------------------------------------//	
	int                        GetLEDCurrentMax();//取得LED電流上限		
	bool                       GetLEDColorUsed_Red();//取得LED顏色使用-紅色
	bool                       GetLEDColorUsed_Grn();//取得LED顏色使用-綠色
	bool                       GetLEDColorUsed_Blu();//取得LED顏色使用-藍色
	//---------------------------------------------------------------------------------//	
	void                       ClearHeightFactor();//清除高度參數T0, T1, T3	
	bool                       SortHeightFactorTableList();//排序高度係數列表
	bool                       SaveHeightFactorTableList();//儲存高度係數列表檔案
	bool                       LoadHeightFactorTableList();//載入高度係數列表檔案
	bool                       RestoreHeightFactorTableList();//復原高度係數列表
	void                       ClearHeightFactorTableList(bool bIncludeFiles);//清除高度係數列表
	bool                       BuildHeightFactorMappingParam(bool bRecv);//建立高度參數T0, T1, T3	
	bool                       CalcHeightFactorMappingParam(std::vector<std::vector<TPhaseFactorGrid>> &GridListArray, std::vector<std::vector<double>> &ParamListArray);//計算高度參數T0, T1, T3
	bool                       VerifyHeightFactorMappingParam(const std::vector<std::vector<double>> &ParamListArray, std::vector<TPhaseFactorGrid> &GridList, double &Error);//驗證高度參數T0, T1, T3	
	bool                       GetHeightFactorMappingParam(double T0[], double T1[], double T2[]);//取得高度參數	
	//---------------------------------------------------------------------------------//		
	bool                       SetPhaseFactor(int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, SPACE_PTR Ptr);//設定平面係數
	bool                       GetPhaseFactor(int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr);//取得平面係數
	bool                       ClonePhaseFactor(int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr);//複製平面係數
	bool                       SaveDLPPhaseFactorFile(LPCTSTR pfilename, IMAGE_SIZE SpaceW, IMAGE_SIZE SpaceH, IMAGE_SIZE SpaceStep, const SPACE_PTR SpacePtr);//儲存DLP平面係數
	//---------------------------------------------------------------------------------//	
	bool                       SetHeightFactorTable(int TargetNo, const TPhaseFactorTable &GridTable);//設定高度係數格點列表		
	bool                       CloneHeightFactorTable(int TargetNo, TPhaseFactorTable &GridTable);//複製高度係數格點列表	
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif//LIGHT_3D_TI_DLP_IMP_USE
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_LIGHT3DTIDLP_H__5B3FDFC8_FE39_47B8_8568_65C7F2A64A2F__INCLUDED_)
