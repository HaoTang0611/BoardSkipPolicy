// AOIExceptionCodeCtrl.h: interface for the CAOIExceptionCodeCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIEXCEPTIONCODECTRL_H__2447BA75_1722_439F_A460_1CE9EC540916__INCLUDED_)
#define AFX_AOIEXCEPTIONCODECTRL_H__2447BA75_1722_439F_A460_1CE9EC540916__INCLUDED_
//-------------------------------------------------------------------------------------//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "AOIExceptionCodeDef.h"
//-------------------------------------------------------------------------------------//
typedef struct tagExceptionNode
{
	CString               sErr;
	AOI_EXCEPTION_CODE    eCode;
	tagExceptionNode(AOI_EXCEPTION_CODE Code=AOI_EXCEPTION_NONE, LPCTSTR Err=_T(""))
	{
		sErr = Err;
		eCode = Code;
	}
} TExceptionNode, *PExceptionNode;
//-------------------------------------------------------------------------------------//
class CAOIExceptionCodeCtrl  
{
private:
	//---------------------------------------------------------------------------------//
	unsigned int               m_LogIdx;    //訊息檔案編號	
	CString                    m_ErrorString;
	CString                    m_LastExceptionTxt;
	CRITICAL_SECTION           m_csExceptionCode;	
	AOI_EXCEPTION_CODE         m_LastExceptionCode;		
	bool                       m_ExceptionCodeRunning;
	//---------------------------------------------------------------------------------//
	std::vector<TExceptionNode> m_ExceptionList;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	CAOIExceptionCodeCtrl(const CAOIExceptionCodeCtrl &other);
	CAOIExceptionCodeCtrl& operator=(const CAOIExceptionCodeCtrl &other);
	//---------------------------------------------------------------------------------//	
	void                       PreInitExceptionCode();
	void                       InitialExceptionCode();
	void                       CloneExceptionCode(const CAOIExceptionCodeCtrl &other);
	//---------------------------------------------------------------------------------//	
	bool                       ReturnNoExceptionCode(DWORD Code);
	//---------------------------------------------------------------------------------//
	bool                       SetExceptionCode(DWORD Code, LPCTSTR Err);	
	CString                    GetExceptionCodeText(AOI_EXCEPTION_CODE Code) const;
	//---------------------------------------------------------------------------------//
	void                       AddAOIExceptionCode(AOI_EXCEPTION_CODE Code, LPCTSTR Err, bool Block=true);		
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//	
	CAOIExceptionCodeCtrl();
	virtual ~CAOIExceptionCodeCtrl();
	//---------------------------------------------------------------------------------//	
	void                       LockExceptionCode();
	void                       UnlockExceptionCode();
	//---------------------------------------------------------------------------------//	
	bool                       RegisterAOIExceptionLogFile();
	bool                       SaveAOIExceptionCodeLogFile(LPCTSTR Message);
	bool                       SaveAOIExceptionCodeLogFile(AOI_EXCEPTION_CODE Code, LPCTSTR Str, LPCTSTR Err);	
	//---------------------------------------------------------------------------------//		
	LPCTSTR                    GetAOILastExceptionText() const;
	AOI_EXCEPTION_CODE         GetAOILastExceptionCode() const;
	//---------------------------------------------------------------------------------//	
	void                       SetExceptionCodeRunning(bool val) { m_ExceptionCodeRunning=val; }
	bool                       GetExceptionCodeRunning() const { return m_ExceptionCodeRunning; }
	//---------------------------------------------------------------------------------//	
	bool                       CheckLastExceptionCodeOK() const;//確認異常碼
	bool                       CheckAOIExceptionCodeCount() const;//確認異常碼	
	//---------------------------------------------------------------------------------//
	bool                       ResetAOIExceptionCode();//復歸異常碼
	bool                       SetAOIExceptionCode_OK();//OK異常碼
	bool                       SetAOIExceptionCode_Others(LPCTSTR Err);//其餘異常碼
	bool                       SetAOIExceptionCode_Param(LPCTSTR Err);//參數異常	
	bool                       SetAOIExceptionCode_FileRead(LPCTSTR Err);//檔案讀取
	bool                       SetAOIExceptionCode_FileWrite(LPCTSTR Err);//檔案寫入
	bool                       SetAOIExceptionCode_MemoryAlloc(LPCTSTR Err);//記憶體配置
	bool                       SetAOIExceptionCode_MemoryFree(LPCTSTR Err);//記憶體釋放	
	//---------------------------------------------------------------------------------//	
	bool                       SetAOIExceptionCode_PLC(DWORD Code, LPCTSTR Err);//PLC異常碼
	bool                       SetAOIExceptionCode_PLC_Others(LPCTSTR Err);//PLC異常碼
	bool                       SetAOIExceptionCode_DLP(DWORD Code, LPCTSTR Err);//DLP異常碼
	bool                       SetAOIExceptionCode_DLP_Others(LPCTSTR Err);//DLP異常碼
	bool                       SetAOIExceptionCode_MES(DWORD Code, LPCTSTR Err);//MES異常碼
	bool                       SetAOIExceptionCode_MES_Others(LPCTSTR Err);//MES異常碼
	bool                       SetAOIExceptionCode_Cuda(DWORD Code, LPCTSTR Err);//Cuda異常碼
	bool                       SetAOIExceptionCode_Cuda_Others(LPCTSTR Err);//Cuda異常碼	
	bool                       SetAOIExceptionCode_File(DWORD Code, LPCTSTR Err);//檔案異常碼
	bool                       SetAOIExceptionCode_File_Others(LPCTSTR Err);//檔案異常碼	
	bool                       SetAOIExceptionCode_Object(DWORD Code, LPCTSTR Err);//物件異常碼
	bool                       SetAOIExceptionCode_Object_Others(LPCTSTR Err);//物件異常碼
	bool                       SetAOIExceptionCode_Memory(DWORD Code, LPCTSTR Err);//記憶體異常碼
	bool                       SetAOIExceptionCode_Memory_Others(LPCTSTR Err);//記憶體異常碼
	bool                       SetAOIExceptionCode_Camera(DWORD Code, LPCTSTR Err);//相機異常碼
	bool                       SetAOIExceptionCode_Camera_Others(LPCTSTR Err);//相機異常碼
	bool                       SetAOIExceptionCode_Motion(DWORD Code, LPCTSTR Err);//軸控異常碼
	bool                       SetAOIExceptionCode_Motion_Others(LPCTSTR Err);//軸控異常碼
	bool                       SetAOIExceptionCode_Thread(DWORD Code, LPCTSTR Err);//執行緒異常碼	
	bool                       SetAOIExceptionCode_Thread_Others(LPCTSTR Err);//執行緒異常碼
	bool                       SetAOIExceptionCode_Thread_Chk(DWORD Code, bool bChk, LPCTSTR Err);//執行緒異常碼
	bool                       SetAOIExceptionCode_System(DWORD Code, LPCTSTR Err);//系統異常碼
	bool                       SetAOIExceptionCode_System_Others(LPCTSTR Err);//系統異常碼
	bool                       SetAOIExceptionCode_System_Chk(DWORD Code, bool bChk, LPCTSTR Err);//系統異常碼
	bool                       SetAOIExceptionCode_BarcodeDevice(DWORD Code, LPCTSTR Err);//條碼機異常碼
	bool                       SetAOIExceptionCode_BarcodeDevice_Others(LPCTSTR Err);//條碼機異常碼
	bool                       SetAOIExceptionCode_LightCtrlBoard(DWORD Code, LPCTSTR Err);//燈控異常碼
	bool                       SetAOIExceptionCode_LightCtrlBoard_Others(LPCTSTR Err);//燈控異常碼
	bool                       SetAOIExceptionCode_Project(DWORD Code, LPCTSTR Err);//專案異常碼
	bool                       SetAOIExceptionCode_Project_Others(LPCTSTR Err);//專案異常碼
	bool                       SetAOIExceptionCode_Calculation(DWORD Code, LPCTSTR Err);//資料計算異常碼	
	bool                       SetAOIExceptionCode_Calculation_Others(LPCTSTR Err);//資料計算異常碼		
	//---------------------------------------------------------------------------------//		
};
//-------------------------------------------------------------------------------------//
extern CAOIExceptionCodeCtrl AOIExceptionCodeCtrl;
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIEXCEPTIONCODECTRL_H__2447BA75_1722_439F_A460_1CE9EC540916__INCLUDED_)
