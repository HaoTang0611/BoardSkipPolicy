// BarCode_CS.h: interface for the BarCode_CS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BARCODE_CS_H__D9D68CBF_AE53_4F4C_9FC6_380FD6A87F9C__INCLUDED_)
#define AFX_BARCODE_CS_H__D9D68CBF_AE53_4F4C_9FC6_380FD6A87F9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <string>
#include <vector>
#include "BarcodeTypeDef.h"
#include "Serial.h"

#define SEPARATOR				 "+"

//----------------------------------------------------------------------------//
#define BARCODE_CODE_DISABLE            1//關閉該碼
#define BARCODE_CODE_ENABLE             2//啟用該碼

//----------------------------------------------------------------------------//
#define BARCODE_CODE_SYSTEM                0
//----------------------------------------------------------------------------//
enum BarcodeCodeType1D
{	
	BARCODE_CODE_1D_GROUP = 100,
	BARCODE_CODE_CODE39,
	BARCODE_CODE_CODE128,
	BARCODE_CODE_BC412,
	BARCODE_CODE_INTERLEAVED_2OF5,
	BARCODE_CODE_CODABAR,
	BARCODE_CODE_UPC_EAN,
	BARCODE_CODE_CODE93,
	BARCODE_CODE_PHARMACODE,
};

enum BarcodeCodeType2D
{
	BARCODE_CODE_2D_GROUP = 200,
	BARCODE_CODE_DATAMATRIX,
	BARCODE_CODE_QRCODE,
	BARCODE_CODE_MICRO_QRCODE,
	BARCODE_CODE_AZTEC_CODE,
	BARCODE_CODE_PDF417,
	BARCODE_CODE_MICRO_PDF417,
};
//----------------------------------------------------------------------------//
namespace Barcode_API
{
	const std::string GetBarcodeReaderName(const int BarcodeID);
	const std::string GetCodeName(const int CodeID);
	const std::string wstring2string(const std::wstring &rStr);
	const std::wstring string2wstring(const std::string &rStr);
}
//----------------------------------------------------------------------------//
class BarCode_CS  
{
	//Support BarCode	
	bool   IsSupportDecode(int Type);//支援該碼解碼器
	bool   IsEnableDecode(int Type);
	bool   EnableDecode(int Type);
	void   DisableDecode(int Type);

	const std::string GetCodeName(int SerialId);
	int GetDecodeSerialId(int DecType);
	std::string  m_ErrMsg;
protected:	
	int      m_MinCodes;
	int      m_MaxCodes;
	int      m_NCodes;
	
	int      m_BarcodeID;	
	bool     m_IsExternalTrigger;
	bool     m_IsInitialSuccessed;	
	std::string m_SectionName;
	std::string m_INIPath;
	bool     m_IsSupportCalibration;
	int      m_BarcodeType;
	int		 m_Timeout;

	std::string  m_FullErrMsg;	
	CRITICAL_SECTION  m_BarcodeCriticalSection;

	int		 m_CodeLength;	//chia005
	char     m_ResultBuffer[1024];
	
	//Support BarCode list
	std::vector<bool>    m_SupportDecode;//支援該碼解碼器
	std::vector<int>     m_EnableDecode;
	
	//--------------------------------------------------------------------------//
	void SetErrorMsg(char *format, ...);
	void ClearErrorMsg();

	virtual bool FindStringDrop(std::string &rSrcDec, const std::string &rObject, bool Foreward);
	virtual bool Import()=0;
	virtual bool Export()=0;
	virtual void ClearBuffer()=0;	

public:
	BarCode_CS();
	virtual ~BarCode_CS();

	virtual void Show();
	virtual bool Initialize()=0;
	virtual bool Trigger()=0;
	virtual bool ReadData()=0;
	virtual bool Connected()=0;
	//virtual bool GetCode_N(int ,char*,int)=0;
	virtual bool GetCode_N(int ,char*,int);
	virtual bool GetCode_N(std::vector<std::string> &rDataList);
	virtual bool SetComPort(int nPort)=0;
	virtual int GetComPort() = 0;
	virtual bool SetCodeLength(int len)=0;
	virtual bool StartDevice()=0;
	virtual bool EndRead()=0;
	virtual bool ChangeRecipe()=0;
	virtual bool TrunONCalibrationMode()=0;
	virtual bool TrunOFFCalibrationMode()=0;
	virtual bool DoCalibration()=0;
	virtual bool SetNMultiCodes(int SymbolNum)=0;
	virtual bool UpdateCodeSetting()=0;

	int  GetNMultiCodes();

	bool    DoBarcodeReadFn();	
	void    SetBarcodeID(int BarcodeID);
	int     GetBarcodeID();
	void    SetIsExternalTrigger(bool Is);
	bool    GetIsExternalTrigger();	
	bool    GetIsInitialSuccessed();

	const std::string &GetErrMsg();
	bool    GetIsSupportCalibration() const;
	int     GetBarcodeType() const;	
	//
	bool   GetDecoder(int CodeID);
	bool   SetDecoder(int CodeID, bool TF);
	bool   GetIsEnableCode(int CodeID);
	bool   EnableCode(int CodeID);	
	void   DisableCode(int CodeID);	

	void        ClearResultBuffer();
	void        SetResultBuffer(const char* String);
	const char* GetResultBuffer() const;
	
	int         GetCodeLength() const;

	void SetTimeout(int TimeoutMs);
	int GetTimeout();

	void SetINIFilePath(const std::string &rFilePath);
	const std::string &GetINIFilePath() const;

	void SetInitializeFlag(bool Enable);
};

//----------------------------------------------------------------------------//
#endif // !defined(AFX_BARCODE_CS_H__D9D68CBF_AE53_4F4C_9FC6_380FD6A87F9C__INCLUDED_)
