// EvsBarcodeDataMatrix.h: interface for the CEvsBarcodeDataMatrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVSBARCODEDATAMATRIX_H__B1E3036C_8C81_4D2B_B9C2_A014988FB03C__INCLUDED_)
#define AFX_EVSBARCODEDATAMATRIX_H__B1E3036C_8C81_4D2B_B9C2_A014988FB03C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "EvsRoiBW8.h"
//-------------------------------------------------------------------------------------//
class CEvsBarcodeDataMatrix  
{
private:
	//---------------------------------------------------------------------------------//
	int                        m_ErrorCode;
	size_t                     m_ErrorStringLen;
	char                       m_ErrorString[128];	
	int                        m_EvsDecodeReader;
	int                        m_EvsDecodeTimeout;
	int                        m_EvsDecodeTimeoutDefault;
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitDataMatrix();
	void                       InitialDataMatrix();
	void                       CloneDataMatrix(const CEvsBarcodeDataMatrix &DataMatrix);
	//---------------------------------------------------------------------------------//	
	CEvsBarcodeDataMatrix(const CEvsBarcodeDataMatrix &DataMatrix);
	CEvsBarcodeDataMatrix& operator=(const CEvsBarcodeDataMatrix &DataMatrix);	
	//---------------------------------------------------------------------------------//
	bool                       SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                       LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//
	bool                       SaveDataMatrixIniFile();//纗DataMatrix把计郎
	bool                       LoadDataMatrixIniFile();//更DataMatrix把计郎	
	//---------------------------------------------------------------------------------//
	bool                       GetIsEVisionError();		
	int                        GetDecodeReader() const;
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CEvsBarcodeDataMatrix();	
	virtual ~CEvsBarcodeDataMatrix();
	//---------------------------------------------------------------------------------//		
	const char*                GetErrorString() const;	
	//---------------------------------------------------------------------------------//	
	void                       SetEvsDecodeTimeout(int Timeoutms) { m_EvsDecodeTimeout = Timeoutms; }
	//---------------------------------------------------------------------------------//	
	bool                       Reader1(CEvsRoiBW8 &Roi, char* BarcodeText, size_t BarcodeTextLen);
	bool                       Reader2(CEvsRoiBW8 &Roi, char* BarcodeText, size_t BarcodeTextLen);
	//---------------------------------------------------------------------------------//	
	bool                       Read(int ImageW, int ImageH, int ImageStep, unsigned char* pImage, char* BarcodeText, size_t BarcodeTextLen);
	//---------------------------------------------------------------------------------//		
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_EVSBARCODEDATAMATRIX_H__B1E3036C_8C81_4D2B_B9C2_A014988FB03C__INCLUDED_)
