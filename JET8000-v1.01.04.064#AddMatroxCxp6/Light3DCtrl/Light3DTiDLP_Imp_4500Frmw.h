// Light3DTiDLP_Imp_4500Frmw.h: interface for the CLight3DTiDLP_Imp_4500Frmw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHT3DTIDLP_IMP_4500FRMW_H__A0854AA8_43AE_4C77_BFF1_B2105B31A36A__INCLUDED_)
#define AFX_LIGHT3DTIDLP_IMP_4500FRMW_H__A0854AA8_43AE_4C77_BFF1_B2105B31A36A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Light3DDef.h"
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_USE_IMP_4500
//-------------------------------------------------------------------------------------//
#include "DLPC350_3_1_0\\dlpc350_common.h"
//-------------------------------------------------------------------------------------//
class CLight3DTiDLP_Imp_4500Frmw  
{
private:
	uint32              m_ChipSelectSize[3];
	uint32              m_ChipSelectEnd[3];
	uint32              m_ChipSelectBase[3];
	uint32              m_FLASH_TABLE_ADDRESS;
	unsigned char      *m_pFrmwImageArray;
	unsigned char      *m_splBuffer;
	uint32              m_splash_index;
	uint32              m_splash_data_start_flash_address;
	uint32              m_appl_config_data_flash_address;	
	int                 m_splash_count;
	char                m_firstIniToken[128];
	uint32              m_iniParams[MAX_VAR_EXP_PAT_LUT_ENTRIES*3];
	int                 m_numIniParams;	
protected:
	//---------------------------------------------------------------------------------//	
	CLight3DTiDLP_Imp_4500Frmw(const CLight3DTiDLP_Imp_4500Frmw &firmware);
	CLight3DTiDLP_Imp_4500Frmw& operator=(const CLight3DTiDLP_Imp_4500Frmw &firmware);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CLight3DTiDLP_Imp_4500Frmw();
	virtual ~CLight3DTiDLP_Imp_4500Frmw();
	unsigned int StringToUInt(LPCTSTR Str, int Base=10);
	//---------------------------------------------------------------------------------//	
	//firmware API
	int Frmw_CopyAndVerifyImage(const unsigned char *pByteArray, int size);//DLPC350_Frmw_CopyAndVerifyImage
	int Frmw_GetSplashCount();//DLPC350_Frmw_GetSplashCount
	uint32 Frmw_GetVersionNumber();//DLPC350_Frmw_GetVersionNumber
	uint32 Frmw_GetSPlashFlashStartAddress();//DLPC350_Frmw_GetSPlashFlashStartAddress
	int Frmw_GetSpashImage(unsigned char *pImageBuffer, int index);//DLPC350_Frmw_GetSpashImage
	int Frmw_SPLASH_InitBuffer(int numSplash);//DLPC350_Frmw_SPLASH_InitBuffer
	int Frmw_SPLASH_AddSplash(unsigned char *pImageBuffer, uint8 *compression, uint32 *compSize);//DLPC350_Frmw_SPLASH_AddSplash
	void Frmw_Get_NewFlashImage(unsigned char **newFrmwbuffer, uint32 *newFrmwsize);//DLPC350_Frmw_Get_NewFlashImage
	void Frmw_Get_NewSplashBuffer(unsigned char **newSplashBuffer, uint32 *newSplashSize);//DLPC350_Frmw_Get_NewSplashBuffer
	void Frmw_UpdateFlashTableSplashAddress(unsigned char *flashTableSectorBuffer, uint32 address_offset);//DLPC350_Frmw_UpdateFlashTableSplashAddress
	int Frmw_ParseIniLines(char *line);//DLPC350_Frmw_ParseIniLines
	void Frmw_GetCurrentIniLineParam(char *token, uint32 *params, int *numParams);//DLPC350_Frmw_GetCurrentIniLineParam
	int Frmw_WriteApplConfigData(char *token, uint32 *params, int numParams);//DLPC350_Frmw_WriteApplConfigData
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif//LIGHT_3D_TI_DLP_USE_IMP_4500
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_LIGHT3DTIDLP_IMP_4500FRMW_H__A0854AA8_43AE_4C77_BFF1_B2105B31A36A__INCLUDED_)
