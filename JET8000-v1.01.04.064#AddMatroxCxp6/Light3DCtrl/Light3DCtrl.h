// Light3DCtrl.h: interface for the CLight3DCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHASECTRL_H__214F8B80_14AF_4DC8_9847_034B09CA1A38__INCLUDED_)
#define AFX_PHASECTRL_H__214F8B80_14AF_4DC8_9847_034B09CA1A38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Light3DDef.h"
#include "Light3DTiDLP4500.h"
#include "Light3DTiDLP4710.h"
#include "Light3DTiDLP.h"
//-------------------------------------------------------------------------------------//
#if LIGHT_3D_MODE == LIGHT_3D_TI_DLP	
	#ifdef LIGHT_3D_TI_DLP_4500_USE
		#define LIGHT_3D_CLS CLight3DTiDLP4500
	#endif//LIGHT_3D_TI_DLP_4500_USE	
	#ifdef LIGHT_3D_TI_DLP_4710_USE
		#define LIGHT_3D_CLS CLight3DTiDLP4710
	#endif//LIGHT_3D_TI_DLP_4710_USE
	#ifdef LIGHT_3D_TI_DLP_IMP_USE
		#define LIGHT_3D_CLS CLight3DTiDLP
	#endif//LIGHT_3D_TI_DLP_IMP_USE	
#endif//LIGHT_3D_MODE

#ifndef LIGHT_3D_CLS
	#error Error, No Define Light 3D Class
#endif

#define LIGHT_3D_CLS_PTR LIGHT_3D_CLS*
//-------------------------------------------------------------------------------------//
class CLight3DCtrl  
{
public:
	//---------------------------------------------------------------------------------//
	static bool                CheckBypassLight3D(LIGHT_3D_CAST_ID CastID);//絋粄3Dщ紇絪腹ぃㄏノ
	static LIGHT_3D_CAST_ID    GetLight3DCastIDByIndex(size_t index);//眔3Dщ紇絪腹
	static LIGHT_3D_CAST_ID    GetLight3DCastIDByDLPChannel(size_t Channel);//眔3Dщ紇絪腹
	static unsigned int        GetLight3DIndexByCastID(LIGHT_3D_CAST_ID CastID);//眔3Dщ紇ま计
	static bool                BuildCastIDCombox(CComboBox &Combox, bool bAddAll);//ミщ甮絪腹跌怠			
	//---------------------------------------------------------------------------------//	
	static int                 GetDLPExposureMinTime(bool Internal, bool MultiTable, int NFrames);//眔程胣丁	
	//---------------------------------------------------------------------------------//
private:
	//---------------------------------------------------------------------------------//	
	LIGHT_3D_CLS_PTR           m_Light3DCast01;
	LIGHT_3D_CLS_PTR           m_Light3DCast02;
	LIGHT_3D_CLS_PTR           m_Light3DCast03;
	LIGHT_3D_CLS_PTR           m_Light3DCast04;
	//---------------------------------------------------------------------------------//	
	CString                    m_ErrorString;	
	CString                    m_Version;
	//---------------------------------------------------------------------------------//
	int                        m_PatternBitCount;//妓狾ㄏノじ计
	std::map<int, int>         m_PatternIndexMap_3Bit;
	std::map<int, int>         m_PatternIndexMap_5Bit;
	std::map<int, int>         m_PatternIndexMap_6Bit;
	std::map<int, int>         m_PatternIndexMap_8Bit;//DLP4710
	int                        m_PatternIndexGrayCode_1Bit;//GrayCodeま计秨﹍	
	int                        m_PatternIndexBinaryCode_1Bit;//BinaryCodeま计秨﹍
	int                        m_PatternNumStartGrayCode_1Bit;//GrayCodeま计-材碭ず甧
	int                        m_PatternNumStartBinaryCode_1Bit;//BinaryCodeま计-材碭ず甧
	int                        m_PatternIndexGrayCode_8Bit;//GrayCodeま计秨﹍	
	int                        m_PatternIndexBinaryCode_8Bit;//BinaryCodeま计秨﹍
	int                        m_PatternNumStartGrayCode_8Bit;//GrayCodeま计-材碭ず甧
	int                        m_PatternNumStartBinaryCode_8Bit;//BinaryCodeま计-材碭ず甧
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	CLight3DCtrl(const CLight3DCtrl&ctrl);
	CLight3DCtrl& operator=(const CLight3DCtrl &ctrl);
	//---------------------------------------------------------------------------------//
	void                          PreInitLight3DCtrl();//箇﹍て北
	void                          InitialLight3DCtrl();//﹍て北	
	//---------------------------------------------------------------------------------//	
	void                          ReleaseLight3DCast();//睦北
	//---------------------------------------------------------------------------------//
	void                          InitialErrorString();
	//---------------------------------------------------------------------------------//
	bool                          BuildLight3DCastList(std::vector<LIGHT_3D_CLS_PTR> &List);
	//---------------------------------------------------------------------------------//
	bool                          SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                          LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//
	CString                       GetPatternIndexIniFilename() const;	
	bool                          ReadPatternParameter();
	bool                          ReadPatternIndexIniFile_GC(int BitMode, int &Index, int &StartNum);
	bool                          ReadPatternIndexIniFile_BC(int BitMode, int &Index, int &StartNum);
	bool                          ReadPatternIndexIniFile(int BitMode, std::map<int, int> &Map);
	bool                          WritePatternParameter();
	bool                          WritePatternIndexIniFile_GC(int BitMode, int Index, int StartNum);	
	bool                          WritePatternIndexIniFile_BC(int BitMode, int Index, int StartNum);	
	bool                          WritePatternIndexIniFile(int BitMode, const std::map<int, int> &Map);	
	bool                          FindPatternIndex(const std::map<int, int> &Map, double Period, int &Index);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//	
	CLight3DCtrl();
	virtual ~CLight3DCtrl();
	//---------------------------------------------------------------------------------//	
	LPCTSTR                       GetErrorString();
	//---------------------------------------------------------------------------------//	
	bool                          LoadPatternIndexIniFile();//更DLP妓ま计琈甮郎
	bool                          SavePatternIndexIniFile();//纗DLP妓ま计琈甮郎
	//---------------------------------------------------------------------------------//	
	bool                          ExecLight3DCastConnect();//北硈絬
	bool                          ExecLight3DCastDisconnect();//北耞絬
	//---------------------------------------------------------------------------------//	
	bool                          CheckLight3DCastIsConnected();//北琌硈絬	
	//---------------------------------------------------------------------------------//	
	LIGHT_3D_CLS_PTR              GetLight3DCastPtr(LIGHT_3D_CAST_ID Light3DCastID);
	//---------------------------------------------------------------------------------//	
	LPCTSTR                       GetLight3DCastVersion();
	//---------------------------------------------------------------------------------//
	LIGHT_3D_DEVICE_TYPE          GetLight3DDeviceType(LIGHT_3D_CAST_ID CastID);
	bool                          ChangeLight3DDeviceType(LIGHT_3D_DEVICE_TYPE Type);
	//---------------------------------------------------------------------------------//
	bool                          GetLight3DCastIDList(std::vector<LIGHT_3D_CAST_ID> &CastIDList);
	//---------------------------------------------------------------------------------//
	bool                          SetLight3DLightSetting(LIGHT_3D_CAST_ID Light3DID, int CurrentID);//砞﹚3Dщ縊方砞﹚	
	bool                          SetAllLight3DLEDColor(int LEDColor);//砞﹚┮ΤLED縊肅︹
	bool                          SetAllLight3DLEDCurrentID(int CurrentID);//砞﹚┮Τ縊方眏
	bool                          SetAllLight3DLEDCurrent(int red, int green, int blue, int CurrentID);//砞﹚┮Τ縊方眏	
	bool                          SetLight3DLEDColor(LIGHT_3D_CAST_ID Light3DID, int LEDColor);//砞﹚3DщLED縊肅︹
	bool                          SetLight3DLEDCurrent(LIGHT_3D_CAST_ID Light3DID, int red, int green, int blue, int CurrentID);//砞﹚3Dщ縊方眏	
	bool                          SetLight3DLEDEnabled(LIGHT_3D_CAST_ID Light3DID, bool bAuto, int bRed, int bGreen, int bBlue);//砞﹚3Dщ縊方币ノ	
	//---------------------------------------------------------------------------------//
	bool                          ClearAllLight3DPatternList();//睲埃┮Τ3D妓狾
	bool                          ReleaseAllLight3DBinParam();//睦┮Τ3DBin把计
	//---------------------------------------------------------------------------------//	
	bool                          ExecAllLight3DSequencePlay();//秨﹍┮Τ
	bool                          ExecLight3DSequencePlay(LIGHT_3D_CAST_ID Light3DID);//秨﹍
	//---------------------------------------------------------------------------------//	
	bool                          ExecAllLight3DSequenceStop();//氨ゎ┮Τ	
	bool                          ExecLight3DSequenceStop(LIGHT_3D_CAST_ID Light3DID);//氨ゎ
	//---------------------------------------------------------------------------------//	
	bool                          SetLight3DPhaseMode(LIGHT_3D_CAST_ID Light3DID, int Mode);//砞﹚舱篈家Α
	bool                          GetLight3DPhaseMode(LIGHT_3D_CAST_ID Light3DID, int &Mode);//眔舱篈家Α
	//---------------------------------------------------------------------------------//	
	bool                          SetLight3DImageGamma(LIGHT_3D_CAST_ID Light3DID, double Gamma);//砞﹚紇钩Gamma把计
	bool                          GetLight3DImageGamma(LIGHT_3D_CAST_ID Light3DID, double &Gamma);//眔紇钩Gamma把计
	//---------------------------------------------------------------------------------//	
	bool                          SetLight3DPhaseZero(LIGHT_3D_CAST_ID Light3DID, int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, PHASE_PTR Ptr);//砞﹚キ
	bool                          GetLight3DPhaseZero(LIGHT_3D_CAST_ID Light3DID, int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, PHASE_PTR &Ptr);//眔キ	
	//---------------------------------------------------------------------------------//	
	bool                          SetLight3DPhaseFactor(LIGHT_3D_CAST_ID Light3DID, int PhaseMode, int LEDColor, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageSetp, SPACE_PTR Ptr);//砞﹚キ玒计
	bool                          GetLight3DPhaseFactor(LIGHT_3D_CAST_ID Light3DID, int PhaseMode, int LEDColor, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageSetp, SPACE_PTR &Ptr);//眔キ玒计
	//---------------------------------------------------------------------------------//
	bool                          AdjustLight3DHeightFactorMappingParam();//秸俱蔼ゑㄒ玒计
	bool                          BuildLight3DHeightFactorMappingParam(LIGHT_3D_CAST_ID Light3DID);//ミ蔼ゑㄒ玒计
	bool                          GetLight3DHeightFactor(LIGHT_3D_CAST_ID Light3DID, double T0[], double T1[], double T2[]);//眔蔼ゑㄒ玒计
	//---------------------------------------------------------------------------------//		
	bool                          ClearLight3DHeightFactorTableList(LIGHT_3D_CAST_ID Light3DID);//睲埃蔼玒计翴
	bool                          CloneLight3DHeightFactorTable(LIGHT_3D_CAST_ID Light3DID, int TargetNo, TPhaseFactorTable &GridTable);//狡籹蔼玒计翴	
	bool                          SetLight3DHeightFactorTable(LIGHT_3D_CAST_ID Light3DID, int TargetNo, const TPhaseFactorTable &GridTable);//砞﹚蔼玒计翴	
	//---------------------------------------------------------------------------------//			
	bool                          SaveAllLight3DCastParameter();//纗┮Τ把计
	bool                          SaveAllLight3DCastParameter(bool bParamOnly);//纗┮Τ把计
	bool                          SaveAllLight3DCastParameter_PhaseZero();//纗┮Τ把计-キ
	bool                          LoadAllLight3DCastParameter(bool bLoadParam);//更┮Τ把计
	//---------------------------------------------------------------------------------//		
	bool                          SaveLight3DCastParameter(LIGHT_3D_CAST_ID Light3DID, bool bParamOnly);//纗﹚把计
	bool                          SaveLight3DCastParameter_PhaseZero(LIGHT_3D_CAST_ID Light3DID);//纗﹚把计-キ
	bool                          LoadLight3DCastParameter(LIGHT_3D_CAST_ID Light3DID, bool bLoadParam);//更﹚把计
	//---------------------------------------------------------------------------------//
	int                           GetPatternBitCount() const;//眔妓狾じ计
	int                           GetPatternIndexGrayCode_1Bit() const;//GrayCodeま计秨﹍
	int                           GetPatternIndexBinaryCode_1Bit() const;//BinaryCodeま计秨﹍
	int                           GetPatternNumStartGrayCode_1Bit() const;//GrayCodeま计-材碭ず甧
	int                           GetPatternNumStartBinaryCode_1Bit() const;//BinaryCodeま计-材碭ず甧
	int                           GetPatternIndexGrayCode_8Bit() const;//GrayCodeま计秨﹍
	int                           GetPatternIndexBinaryCode_8Bit() const;//BinaryCodeま计秨﹍
	int                           GetPatternNumStartGrayCode_8Bit() const;//GrayCodeま计-材碭ず甧
	int                           GetPatternNumStartBinaryCode_8Bit() const;//BinaryCodeま计-材碭ず甧
	bool                          FindPatternIndex(int BitMode, double Period, int &Index);
	bool                          SetAllLight3DCastPatternIndexByPeriod(double Period1, double Period2);//穝3Dщ妓-ㄌ沮秅戳
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
extern CLight3DCtrl Light3DCtrl;
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_PHASECTRL_H__214F8B80_14AF_4DC8_9847_034B09CA1A38__INCLUDED_)
