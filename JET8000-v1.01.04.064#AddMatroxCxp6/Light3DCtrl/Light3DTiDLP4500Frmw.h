// Light3DTiDLP4500Frmw.h: interface for the CLight3DTiDLP4500Frmw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHT3DTIDLP4500_FRMW_H__A0854AA8_43AE_4C77_BFF1_B2105B31A36A__INCLUDED_)
#define AFX_LIGHT3DTIDLP4500_FRMW_H__A0854AA8_43AE_4C77_BFF1_B2105B31A36A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Light3DDef.h"
//-------------------------------------------------------------------------------------//
/*
 * dlpc350_firmware.h
 *
 * This module handles building and parsing of firmware images
 *
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
*/
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_4500_USE
//-------------------------------------------------------------------------------------//
#include "dlpc350_common.h"

#define RELEASE_FW_VERSION  0x030000 // update for new DLPC350 binaries

#define MAX_FIRMWARE_BYTES              31457280 // Only 30MB for safety even though LCr4500 has 32MB onboard
#define MAX_SPLASH_IMAGES		        256
#define FLASH_TABLE_SPLASH_INDEX	    0
#define FLASHTABLE_APP_SIGNATURE	    0x01234567

#define FLASH_NUM_APP_ADDRS             4
#define FLASH_NUM_ASIC_CFG_ADDRS        4
#define FLASH_NUM_SEQ_ADDRS             4
#define FLASH_NUM_APPL_CFG_ADDRS        4
#define FLASH_NUM_OSD_ADDRS             4
#define FLASH_NUM_SPLASH_ADDRS          4
#define FLASH_NUM_OTHER_ADDRS           4
#define FLASH_NUM_SPLASH2_ADDRS         12
#define FLASH_NUM_BATCHFILES            16
#define FLASH_MAX_APPS                  4

#define FLASH_BASE_ADDRESS		0xF9000000

#define ERROR_NO_MEM_FOR_MALLOC			        -1
#define ERROR_FRMW_FLASH_TABLE_SIGN_MISMATCH	-2
#define ERROR_NO_SPLASH_IMAGE                   -3
#define ERROR_NOT_BMP_FILE                      -4
#define ERROR_NOT_24bit_BMP_FILE                -5
#define ERROR_INIT_NOT_DONE_PROPERLY            -6
#define ERROR_WRONG_PARAMS		                -7
#define ERROR_NO_SPACE_IN_FRMW                  -8

#define SPLASH_UNCOMPRESSED         0
#define SPLASH_RLE_COMPRESSION		1
#define SPLASH_4LINE_COMPRESSION	4
#define SPLASH_NOCOMP_SPECIFIED		5

typedef struct
{
    uint32 Address;    /* Address of block */
    uint32 ByteCount;  /* Size of block in bytes */
} FLASH_BLOCK; /* sizeof(FLASH_BLOCK) == 8 */

/** Flash Data Address Types */
typedef struct
{
  uint32          Signature;    /* = 0x1234567 */
  uint32          Boot_Address;     /* Address of Application entry to bootloader */
  uint32          Version;          /* Version == 0x11 */
  uint32          Free_Area_Start; /* Address of first free location in flash */
  FLASH_BLOCK     AppCode[FLASH_NUM_APP_ADDRS];//application binary
  FLASH_BLOCK     ASIC_Config_Data[FLASH_NUM_ASIC_CFG_ADDRS];
  FLASH_BLOCK     Sequence[FLASH_NUM_SEQ_ADDRS];
  FLASH_BLOCK     APPL_Config_Data[FLASH_NUM_APPL_CFG_ADDRS];
  FLASH_BLOCK     OSD_Data[FLASH_NUM_OSD_ADDRS];
  FLASH_BLOCK     Splash_Data[FLASH_NUM_SPLASH_ADDRS];
  FLASH_BLOCK     APPL_OtherBinary[FLASH_NUM_OTHER_ADDRS]; //other binary
  FLASH_BLOCK     Splash_Data2[FLASH_NUM_SPLASH2_ADDRS];
  FLASH_BLOCK     Batch_File[FLASH_NUM_BATCHFILES];
} FLASH_TABLE; /*  */

typedef struct superbinaryinfo
{
    uint32 Sig1;		// '&H52505553 "SUPR"
    uint32 Sig2;		// '&H53544D43 "CMTS" / '&H53514553 "SEQS"
    uint32 BlobCount;
} SPLASH_SUPER_BINARY_INFO;

typedef struct blobinfo
{
    uint32 BlobOffset;
    uint32 BlobSize;
}SPLASH_BLOB_INFO;

typedef struct splashhdr
{
    uint32  Signature;      /**< format 3 == "Splc" */
                            /**< (0x53, 0x70, 0x6c, 0x63) */
    uint16  Image_width;    /**< width of image in pixels */
    uint16  Image_height;   /**< height of image in pixels */
    uint32  Byte_count;     /**< number of bytes starting at "data" */
    uint32  Subimg_offset;  /**< byte-offset from "data" to 1st line */
                            /**< of sub-image, or 0xFFFFFFFF if none. */
    uint32  Subimg_end;     /**< byte-offset from "data" to end of */
                            /**< last line of sub-image, */
                            /**< or 0xFFFFFFFF if none. */
    uint32  Bg_color;       /**< unpacked 24-bit background color */
                            /**< (format: 0x00RRGGBB) */
    uint8  Pixel_format;   /**< format of pixel data */
                            /**< 0 = 24-bit unpacked: 0x00RRGGBB Not supported by DDP2230/DDPDDP243x*/
                            /**< 1 = 24-bit packed:   RGB 8-8-8 */
                            /**< 2 = 16-bit:          RGB 5-6-5   DDP3020 only */
                            /**< 3 = 16-bit:          YCrCb 4:2:2 DDP2230/DDPDDP243x only */
    uint8  Compression;    /**< compression of image */
                            /**< SPLASH_FORCE_UNCOMPRESSED  = uncompressed */
                            /**< SPLASH_FORCE_RLE           = RLE compressed */
                            /**< SPLASH_USER_DEFINED        = User Defined Compression */
                            /**< SPLASH_FORCE_RLE_2PIXEL    = RLE compressed 2Pixel */

    uint8  ByteOrder;      /**< 0 - pixel is 00RRGGBB - DDP3020 */
                            /**< 1 - pixel is 00GGRRBB - DDP2230/DDPDDP243x */
    uint8  ChromaOrder;    /**< Indicates chroma order of pixel data (DDP2230/DDPDDP243x only) */
                            /**< 0 - Cr is first pixel (0xYYRR) */
                            /**< 1 - Cb is first pixel (0xYYBB) */
    uint8  Pad[4];         /**< pad so that data starts at 16-byte boundary */
} SPLASH_HEADER;

typedef struct iniParamInfo
{
	char   token[128];
	uint32 default_param[1824*3];
	uint32 gui_defined_param[1824*3];
	int nr_default_params;
	int nr_user_defined_params;
	bool is_gui_editable;
	int frmw_offset;
	int frmw_size;
} INIPARAM_INFO;

enum iniTokens
{
    //APPCONFIG_VERSION_SUBMINOR,
    APPCONFIG_VERSION_SUBMINOR,
    APPCONFIG_VERSION_MINOR,
    APPCONFIG_VERSION_MAJOR,
    DEFAULT_FIRMWARE_TAG,
    DEFAULT_AUTOSTART,
    DEFAULT_DISPMODE,
    DEFAULT_SHORT_FLIP,
    DEFAULT_LONG_FLIP,
    DEFAULT_TRIG_OUT_1_POL,
    DEFAULT_TRIG_OUT_1_RDELAY,
    DEFAULT_TRIG_OUT_1_FDELAY,
    DEFAULT_TRIG_OUT_2_POL,
    DEFAULT_TRIG_OUT_2_WIDTH,
    DEFAULT_TRIG_IN_1_DELAY,
    //DEFAULT_TRIG_IN_1_POL,
    //DEFAULT_TRIG_IN_2_DELAY,
    DEFAULT_TRIG_IN_2_POL,
    DEFAULT_RED_STROBE_RDELAY,
    DEFAULT_RED_STROBE_FDELAY,
    DEFAULT_GRN_STROBE_RDELAY,
    DEFAULT_GRN_STROBE_FDELAY,
    DEFAULT_BLU_STROBE_RDELAY,
    DEFAULT_BLU_STROBE_FDELAY,
    DEFAULT_INVERTDATA,
    DEFAULT_TESTPATTERN,
    DEFAULT_LEDCURRENT_RED,
    DEFAULT_LEDCURRENT_GRN,
    DEFAULT_LEDCURRENT_BLU,
    DEFAULT_PATTERNCONFIG_PAT_EXPOSURE,
    DEFAULT_PATTERNCONFIG_PAT_PERIOD,
    DEFAULT_PATTERNCONFIG_PAT_MODE,
    DEFAULT_PATTERNCONFIG_TRIG_MODE,
    DEFAULT_PATTERNCONFIG_PAT_REPEAT,
    DEFAULT_PATTERNCONFIG_NUM_LUT_ENTRIES,
    DEFAULT_PATTERNCONFIG_NUM_PATTERNS,
    DEFAULT_PATTERNCONFIG_NUM_SPLASH,
    DEFAULT_SPLASHLUT,
    DEFAULT_SEQPATLUT,
    DEFAULT_LED_ENABLE_MAN_MODE,
    DEFAULT_MAN_ENABLE_RED_LED,
    DEFAULT_MAN_ENABLE_GRN_LED,
    DEFAULT_MAN_ENABLE_BLU_LED,
    DEFAULT_PORTCONFIG_PORT,
    DEFAULT_PORTCONFIG_BPP,
    DEFAULT_PORTCONFIG_PIX_FMT,
    DEFAULT_PORTCONFIG_PORT_CLK,
    //DEFAULT_PORTCONFIG_CSC_0,
    //DEFAULT_PORTCONFIG_CSC_1,
    //DEFAULT_PORTCONFIG_CSC_2,
    DEFAULT_PORTCONFIG_ABC_MUX,
    DEFAULT_PORTCONFIG_PIX_MODE,
    DEFAULT_PORTCONFIG_SWAP_POL,
    DEFAULT_PORTCONFIG_FLD_SEL,
    PERIPHERALS_I2CADDRESS_0,
    PERIPHERALS_I2CADDRESS_1,
    //PERIPHERALS_USB_SRL_0,
    //PERIPHERALS_USB_SRL_1,
    DATAPATH_SPLASHSTARTUPTIMEOUT,
    DATAPATH_SPLASHATSTARTUPENABLE,
    MACHINE_DATA_COLORPROFILE_0_BRILLIANTCOLORLOOK,
    INITOKENS_MAX,
};

#define NR_INI_TOKENS INITOKENS_MAX

#define NR_INI_GUI_TOKENS  42 //Is taken from iniGUITokens list entries
//-------------------------------------------------------------------------------------//
class CLight3DTiDLP4500Frmw  
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
	CLight3DTiDLP4500Frmw(const CLight3DTiDLP4500Frmw &firmware);
	CLight3DTiDLP4500Frmw& operator=(const CLight3DTiDLP4500Frmw &firmware);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CLight3DTiDLP4500Frmw();
	virtual ~CLight3DTiDLP4500Frmw();
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
#endif//LIGHT_3D_TI_DLP_4500_USE
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_LIGHT3DTIDLP4500_FRMW_H__A0854AA8_43AE_4C77_BFF1_B2105B31A36A__INCLUDED_)
