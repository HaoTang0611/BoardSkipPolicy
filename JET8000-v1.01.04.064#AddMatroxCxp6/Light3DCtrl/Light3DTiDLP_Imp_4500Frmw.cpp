// Light3DTiDLP_Imp_4500Frmw.cpp: implementation of the CLight3DTiDLP_Imp_4500Frmw class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Light3DTiDLP_Imp_4500Frmw.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#ifdef LIGHT_3D_TI_DLP_USE_IMP_4500
//-------------------------------------------------------------------------------------//
#include "DLPC350_3_1_0\\dlpc350_firmware.cpp"
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CLight3DTiDLP_Imp_4500Frmw::CLight3DTiDLP_Imp_4500Frmw()
{	
	m_FLASH_TABLE_ADDRESS = 0x00020000;
	m_pFrmwImageArray = NULL;
	m_splBuffer = NULL;

	m_ChipSelectSize[0] = 0x01000000;
	m_ChipSelectSize[1] = 0x01000000;
	m_ChipSelectSize[2] = 0x01000000;

	// LightCrafter 4500 does not have third chip
	m_ChipSelectSize[0] = 0x00000000;
	m_ChipSelectSize[1] = 0x01000000;
	m_ChipSelectSize[2] = 0x01000000;

	m_ChipSelectEnd[0]  = 0xFC000000;
	m_ChipSelectEnd[1]  = 0xFA000000;
	m_ChipSelectEnd[2]  = 0xFB000000;

	m_ChipSelectBase[0] = 0xFB000000;
	m_ChipSelectBase[1] = 0xF9000000;
	m_ChipSelectBase[2] = 0xFA000000;
	::memset(m_firstIniToken, 0x00, sizeof(m_firstIniToken));
}
//-------------------------------------------------------------------------------------//
CLight3DTiDLP_Imp_4500Frmw::~CLight3DTiDLP_Imp_4500Frmw()
{

}
//-------------------------------------------------------------------------------------//
unsigned int CLight3DTiDLP_Imp_4500Frmw::StringToUInt(LPCTSTR Str, int Base)
{
	size_t i=0, j=0;
	unsigned int v1 = 0;
	unsigned int v2 = 0;
	unsigned int v3 = 0;
	const size_t len = ::_tcslen(Str);
	switch ( Base )
	{
	case 2:
		for ( i=0; i<len; i++)
		{
			switch ( Str[i] )
			{
			case _T('0'):	v1 = 0;	break;
			case _T('1'):	v1 = 1;	break;
			default:		v1 = 0;	break;
			}

			if ( 0 == v1 ) { continue; }
			
			v2 = 1;
			for ( j=0; j<len-i-1; j++ )
			{	v2 *=2; }			
			v3 += (v1*v2);
		}
		break;
	case 4:
		for ( i=0; i<len; i++)
		{
			switch ( Str[i] )
			{
			case _T('0'):	v1 = 0;	break;
			case _T('1'):	v1 = 1;	break;
			case _T('2'):	v1 = 2;	break;
			case _T('3'):	v1 = 3;	break;			
			default:		v1 = 0;	break;
			}

			if ( 0 == v1 ) { continue; }
			
			v2 = 1;
			for ( j=0; j<len-i-1; j++ )
			{	v2 *=4; }			
			v3 += (v1*v2);
		}
		break;
	case 8:
		for ( i=0; i<len; i++)
		{
			switch ( Str[i] )
			{
			case _T('0'):	v1 = 0;	break;
			case _T('1'):	v1 = 1;	break;
			case _T('2'):	v1 = 2;	break;
			case _T('3'):	v1 = 3;	break;
			case _T('4'):	v1 = 4;	break;
			case _T('5'):	v1 = 5;	break;
			case _T('6'):	v1 = 6;	break;
			case _T('7'):	v1 = 7;	break;
			default:		v1 = 0;	break;
			}

			if ( 0 == v1 ) { continue; }
			
			v2 = 1;
			for ( j=0; j<len-i-1; j++ )
			{	v2 *=8; }			
			v3 += (v1*v2);
		}
		break;
	case 10:
		for ( i=0; i<len; i++)
		{
			switch ( Str[i] )
			{
			case _T('0'):	v1 = 0;	break;
			case _T('1'):	v1 = 1;	break;
			case _T('2'):	v1 = 2;	break;
			case _T('3'):	v1 = 3;	break;
			case _T('4'):	v1 = 4;	break;
			case _T('5'):	v1 = 5;	break;
			case _T('6'):	v1 = 6;	break;
			case _T('7'):	v1 = 7;	break;
			case _T('8'):	v1 = 8;	break;
			case _T('9'):	v1 = 9;	break;			
			default:	v1 = 0;	break;
			}
			if ( 0 == v1 ) { continue; }

			v2 = 1;
			for ( j=0; j<len-i-1; j++ )
			{	v2 *=10; }			
			v3 += (v1*v2);
		}
		break;
	case 16:
		for ( i=0; i<len; i++)
		{
			switch ( Str[i] )
			{
			case _T('0'):	v1 = 0;	break;
			case _T('1'):	v1 = 1;	break;
			case _T('2'):	v1 = 2;	break;
			case _T('3'):	v1 = 3;	break;
			case _T('4'):	v1 = 4;	break;
			case _T('5'):	v1 = 5;	break;
			case _T('6'):	v1 = 6;	break;
			case _T('7'):	v1 = 7;	break;
			case _T('8'):	v1 = 8;	break;
			case _T('9'):	v1 = 9;	break;
			case _T('a'):	v1 =10;	break;
			case _T('A'):	v1 =10;	break;
			case _T('b'):	v1 =11;	break;
			case _T('B'):	v1 =11;	break;
			case _T('c'):	v1 =12;	break;
			case _T('C'):	v1 =12;	break;
			case _T('d'):	v1 =13;	break;
			case _T('D'):	v1 =13;	break;
			case _T('e'):	v1 =14;	break;
			case _T('E'):	v1 =14;	break;
			case _T('f'):	v1 =15;	break;
			case _T('F'):	v1 =15;	break;
			default:	v1 = 0;	break;
			}
			if ( 0 == v1 ) { continue; }
			v2 = 1;
			for ( j=0; j<len-i-1; j++ )
			{	v2 *=16; }			
			v3 += (v1*v2);
		}
		break;
	}
	return v3;
}
//-------------------------------------------------------------------------------------//
static int SPLASH_PerformLineCompression(unsigned char *SourceAddr, int ImageWidth, int ImageHeight, uint32 *compressed_size, uint8 numLines)
{
	uint16 Row, Col;
    uint32 lineLength, bytesPerPixel = 3, imageHeight, pixelIndex;
    unsigned char *line1Data, *line2Data;

    lineLength =  ImageWidth * bytesPerPixel;

    if(lineLength % 4 != 0)
    {
        lineLength = (lineLength / 4 + 1) * 4;
    }

    if(numLines > 8)
    {
        *compressed_size = 0xFFFFFFFF;
        return 1;
    }

    imageHeight = (ImageHeight / numLines) * numLines;

    for (Row = 0; Row < (imageHeight - numLines); Row++)
    {
        line1Data = (unsigned char *)(SourceAddr + (lineLength *  Row));
        line2Data = (unsigned char *)(SourceAddr + (lineLength * (Row + numLines)));

        for (Col = 0; Col < ImageWidth; Col++)
        {
            pixelIndex = Col * bytesPerPixel;

            if( (line1Data[pixelIndex  ] != line2Data[pixelIndex  ]) ||
                    (line1Data[pixelIndex+1] != line2Data[pixelIndex+1]) ||
                    (line1Data[pixelIndex+2] != line2Data[pixelIndex+2]))
            {
                *compressed_size = 0xFFFFFFFF;
                return 1;
            }
        }
    }

    *compressed_size = numLines * lineLength;
    return 0;
}
//-------------------------------------------------------------------------------------//
static int SPLASH_PerformRLECompression(unsigned char *SourceAddr, unsigned char *DestinationAddr, int ImageWidth, int ImageHeight, uint32 *compressed_size)
{
    uint16 Row, Col;
    BOOL   FirstPixel = TRUE;
    uint8 Repeat = 1;
    uint32 Pixel = 0, S, D;
    uint32 LastColor = 0, pad;
    uint8 count = 0;

    //    uint8 *SourceAddr		= (uint08 *)(SplashRLECfg->SourceAddr);
    //    uint8 *DestinationAddr = (uint08 *)(SplashRLECfg->DestinationAddr);
    uint32 PixelSize		= 3;
    
    S = 0;
    D = 0;

    /* RLE Encode the Splash Image*/
    for (Row = 0; Row < ImageHeight; Row++)
    {
        for (Col = 0; Col < ImageWidth; Col++)
        {

            memcpy(&Pixel, SourceAddr + S, PixelSize);

            S += PixelSize;

            /* if this is the first Pixel, remember it and move on... */
            if (FirstPixel)
            {
                LastColor  = Pixel;
                Repeat  = 1;
                FirstPixel = FALSE;
                count = 0;
            }
            else
            {
                if (Pixel == LastColor)
                {
                    if (count)
                    {
                        if(count > 1) DestinationAddr[D++] = 0;
                        DestinationAddr[D++] = count;
                        memcpy(DestinationAddr + D, SourceAddr + (S - ((count + 2) * PixelSize)) , count * PixelSize);
                        D += (count * PixelSize);
                        count = 0;
                    }

                    Repeat++;

                    if (Repeat == 255)
                    {
                        DestinationAddr[D++] = Repeat;
                        memcpy(DestinationAddr + D, &LastColor, PixelSize);
                        D += PixelSize;
                        FirstPixel = TRUE;
                    }
                }
                else
                {
                    if (Repeat == 1)
                    {
                        count++;
                        if (count == 255)
                        {
                            if(count > 1) DestinationAddr[D++] = 0;
                            DestinationAddr[D++] = count;
                            memcpy(DestinationAddr + D, SourceAddr + (S - ((count + 1) * PixelSize)), count * PixelSize);
                            D += (count * PixelSize);
                            count = 0;
                        }
                    }
                    else
                    {
                        DestinationAddr[D++] = Repeat;
                        memcpy(DestinationAddr + D, &LastColor, PixelSize);
                        D += PixelSize;
                        Repeat = 1;
                    }
                    LastColor = Pixel;
                }
            }

            /* Last Pixel of the line*/
            if (Col == (ImageWidth-1) && Repeat != 255 && count != 255)
            {
                if (count)
                {
                    DestinationAddr[D++] = 0;
                    DestinationAddr[D++] = count + 1;
                    memcpy(DestinationAddr + D, SourceAddr + (S - ((count + 1) * PixelSize)), (count + 1) * PixelSize);
                    D += ((count + 1) * PixelSize);
                    count = 0;
                }
                else
                {
                    DestinationAddr[D++] = Repeat;
                    memcpy(DestinationAddr + D, &LastColor, PixelSize);
                    D += PixelSize;
                }
                FirstPixel = TRUE;
            }
        }
        // END OF LINE
        DestinationAddr[D++] = 0;
        DestinationAddr[D++] = 0;

        /* Scan lines are always padded out to next 32-bit boundary */
        if(D % 4 != 0)
        {
            pad = 4 - (D % 4);
            memset(DestinationAddr + D, 0, pad);
            D += pad;
        }
    }

    /* End of file: Control Byte = 0 & Color Byte = 1 */
    DestinationAddr[D++] = 0;
    DestinationAddr[D++] = 1;

    /* End of file should be padded out till 128-bit boundary */
    if(D % 16 != 0)
    {
        pad = 16 - (D % 16);
        memset(DestinationAddr + D, 0, pad);
        D += pad;
    }

    // update flash size
    *compressed_size = D;

    return 0;
}
//-------------------------------------------------------------------------------------//
static int SPLASH_PerformRLEUnCompression(unsigned char *SourceAddr, unsigned char *DestinationAddr, uint32 *size)
{
	uint32 PixelSize= 3, S = 0, D = 0;
    int i;

    while (S < *size)
    {
        uint32 ctrl_byte, color_byte;

        ctrl_byte = SourceAddr[S];
        color_byte = SourceAddr[S + 1];
        if (ctrl_byte == 0)
        {
            if (color_byte == 1)	// End of image
                break;
            else if (color_byte == 0)	// End of Line.
            {
                i = 0;
                S +=2;
                if (S % 4 != 0)
                {
                    int pad = 4 - (S % 4);
                    S += pad;
                }
            }
            else if (color_byte >= 2)
            {
                S +=2;
                memcpy(DestinationAddr + D, SourceAddr + S, color_byte * PixelSize);
                D += color_byte * PixelSize;
                S += color_byte * PixelSize;
            }
            else
                return -1;
        }
        else if (ctrl_byte > 0)
        {
            S++;
            for (i = 0; (unsigned int)i < ctrl_byte; i++)
            {
                memcpy(DestinationAddr + D, SourceAddr + S, PixelSize);
                D += PixelSize;
            }
            S += PixelSize;
        }
        else
            return -1;
    }
    *size = D;
    return 0;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500Frmw::Frmw_CopyAndVerifyImage(const unsigned char *pByteArray, int size)
{
	FLASH_TABLE *flash_table;

    if (m_pFrmwImageArray != NULL)
    {
        free(m_pFrmwImageArray);
        m_pFrmwImageArray = NULL;
        m_splash_data_start_flash_address = 0;
        m_appl_config_data_flash_address = 0;
    }


    m_pFrmwImageArray = (unsigned char *)malloc(size);
    if (m_pFrmwImageArray == NULL)
        return ERROR_NO_MEM_FOR_MALLOC;

    memcpy(m_pFrmwImageArray, pByteArray, size);

    flash_table = (FLASH_TABLE *)(m_pFrmwImageArray + m_FLASH_TABLE_ADDRESS);

    if(flash_table->Signature != FLASHTABLE_APP_SIGNATURE)
    {
        m_FLASH_TABLE_ADDRESS = 0x00008000;

        flash_table = (FLASH_TABLE *)(m_pFrmwImageArray + m_FLASH_TABLE_ADDRESS);

        if(flash_table->Signature != FLASHTABLE_APP_SIGNATURE)
        {
            m_FLASH_TABLE_ADDRESS = 0x00020000;

            flash_table = (FLASH_TABLE *)(m_pFrmwImageArray + m_FLASH_TABLE_ADDRESS);

            if(flash_table->Signature != FLASHTABLE_APP_SIGNATURE)
                return ERROR_FRMW_FLASH_TABLE_SIGN_MISMATCH;
        }
    }
    m_splash_data_start_flash_address = flash_table->Splash_Data[FLASH_TABLE_SPLASH_INDEX].Address;
    m_appl_config_data_flash_address = flash_table->APPL_Config_Data[0].Address;

    return 0;
}
//-------------------------------------------------------------------------------------//
uint32 CLight3DTiDLP_Imp_4500Frmw::Frmw_GetVersionNumber()
{
	uint32 version_number;

    if (!m_appl_config_data_flash_address)
        return ERROR_INIT_NOT_DONE_PROPERLY;
    memcpy(&version_number, m_pFrmwImageArray + (m_appl_config_data_flash_address - FLASH_BASE_ADDRESS), sizeof(version_number));

    return version_number;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500Frmw::Frmw_WriteApplConfigData(char *token, uint32 *params, int numParams)
{
	int i;
    int j;
    int index = -1;
    int offset;
    char toUpperToken[128];
    static int trigMode = -1;

    uint32 appl_config_data_start_address = m_appl_config_data_flash_address - FLASH_BASE_ADDRESS;
    uint8 *app_data = (uint8 *)(m_pFrmwImageArray + appl_config_data_start_address);

    if (!m_appl_config_data_flash_address)
        return ERROR_INIT_NOT_DONE_PROPERLY;

    //Convert everything to uppercase for string comparison
    i = 0;
    while(token[i] != '\0')
    {
        toUpperToken[i] = toupper(token[i]);
        i++;
    }
    toUpperToken[i] = '\0';

    for (index = 0; index < NR_INI_TOKENS; index++)
        if(strcmp(&g_iniParam_Info[index].token[0],&toUpperToken[0]) == 0)
            break;

    if (index == NR_INI_TOKENS) //if no match found in the above search
        return ERROR_WRONG_PARAMS;

    offset = g_iniParam_Info[index].frmw_offset;

    if(strcmp("DEFAULT.PORTCONFIG.CSC",&toUpperToken[0]) == 0)
    {
        if(numParams != 9)
            return ERROR_WRONG_PARAMS;

        for(i = 0; i < 9; i++)
        {
            *((uint16*) (app_data + offset)) = (uint16)params[i];
            offset += 2;
        }
    }
    else if (strcmp("DEFAULT.SPLASHLUT",&toUpperToken[0]) == 0)
    {
        //legacy trigger mode
        if(trigMode >= 0 && trigMode <= 2)
        {
            //Legacy pattern sequence maxsize = 64
            if((numParams < 1) || (numParams > MAX_IMAGE_LUT_ENTRIES))
                return ERROR_WRONG_PARAMS;
        }
        else
        {
            //Variable exposure trigger mode
            if(trigMode > 2 && trigMode <= 4)
            {
                //Variable exposure pattern sequence maxsize = 256
                if((numParams < 1) || (numParams > MAX_VAR_EXP_IMAGE_LUT_ENTRIES))
                    return ERROR_WRONG_PARAMS;
            }
            else
                return ERROR_WRONG_PARAMS;
        }

        //Special case for two entries
        if(numParams == 2)
        {
            *((uint8*) (app_data + offset)) = (uint8)params[1];
            offset ++;

            *((uint8*) (app_data + offset)) = (uint8)params[0];
            offset ++;
        }
        else
        {
            for (i = 0; i < numParams; i++)
            {
                *((uint8*) (app_data + offset)) = (uint8)params[i];
                offset ++;
            }
        }
    }
    else if (strcmp("DEFAULT.SEQPATLUT",&toUpperToken[0]) == 0)
    {

        //legacy trigger mode
        if(trigMode >= 0 && trigMode <= 2)
        {
            //Legacy pattern sequence maxsize = 128
            if((numParams < 1) || (numParams > MAX_PAT_LUT_ENTRIES))
                return ERROR_WRONG_PARAMS;

            //16-BYTES space available per entry
            for (i = 0; i < numParams; i++)
            {
                //BYTE - 3 Rsvd
                *((uint8*) (app_data + offset)) = (uint8)((params[i] >> 24) & 0xFF);
                offset ++;

                //BYTE - 0
                *((uint8*) (app_data + offset)) = (uint8)((params[i] >> 0) & 0xFF);
                offset ++;

                //BYTE - 1
                *((uint8*) (app_data + offset)) = (uint8)((params[i] >> 8) & 0xFF);
                offset ++;

                //BYTE - 2
                *((uint8*) (app_data + offset)) = (uint8)((params[i] >> 16) & 0xFF);
                offset ++;

                //fill remaining 12 - bytes with zeros
                for(j = 0; j < 12; j++)
                {
                    *((uint8*) (app_data + offset)) = (uint8) 0x00;
                    offset ++;
                }
            }
        }
        else
        {
            //Variable exposure trigger mode
            if(trigMode > 2 && trigMode <= 4)
            {
                //Variable exposure pattern sequence maxsize = 1824
                if((numParams < 1) || (numParams > (MAX_VAR_EXP_PAT_LUT_ENTRIES*3)))
                    return ERROR_WRONG_PARAMS;
            }
            else
                return ERROR_WRONG_PARAMS;

            //16-BYTES space available per entry - maximum can be 1824x3 32bit params
            for (i = 0; i < (numParams/3); i++)
            {
                //BYTE - 3 Rsvd
                *((uint8*) (app_data + offset)) = (uint8)((params[i*3] >> 24) & 0xFF);
                offset ++;

                //BYTE - 0
                *((uint8*) (app_data + offset)) = (uint8)((params[i*3] >> 0) & 0xFF);
                offset ++;

                //BYTE - 1
                *((uint8*) (app_data + offset)) = (uint8)((params[i*3] >> 8) & 0xFF);
                offset ++;

                //BYTE - 2
                *((uint8*) (app_data + offset)) = (uint8)((params[i*3] >> 16) & 0xFF);
                offset ++;

                //Fill Pattern exposure 32-bit information
                //LSB
                *((uint8*) (app_data + offset)) = (uint8)((params[i*3+1] >> 0) & 0xFF);
                offset ++;

                //LSB+1
                *((uint8*) (app_data + offset)) = (uint8)((params[i*3+1] >> 8) & 0xFF);
                offset ++;

                //LSB+2
                *((uint8*) (app_data + offset)) = (uint8)((params[i*3+1] >> 16) & 0xFF);
                offset ++;

                //LSB+3
                *((uint8*) (app_data + offset)) = (uint8)((params[i*3+1] >> 24) & 0xFF);
                offset ++;

                //Fill total Pattern period 32-bit information
                //LSB
                *((uint8*) (app_data + offset)) = (uint8)((params[i*3+2] >> 0) & 0xFF);
                offset ++;

                //LSB+1
                *((uint8*) (app_data + offset)) = (uint8)((params[i*3+2] >> 8) & 0xFF);
                offset ++;

                //LSB+2
                *((uint8*) (app_data + offset)) = (uint8)((params[i*3+2] >> 16) & 0xFF);
                offset ++;

                //LSB+3
                *((uint8*) (app_data + offset)) = (uint8)((params[i*3+2] >> 24) & 0xFF);
                offset ++;

                //fill remaining 4 - bytes with zeros
                for(j = 0; j < 4; j++)
                {
                    *((uint8*) (app_data + offset)) = (uint8) 0x00;
                    offset ++;
                }
           }
        }
    }
    else if (strcmp("PERIPHERALS.USB_SRL",&toUpperToken[0]) == 0)
    {
        if(numParams != 4)
            return ERROR_WRONG_PARAMS;

        for(int i = 0; i < 4; i++)
        {
            *((uint16*) (app_data + offset)) = (uint16)params[i];
            offset += 2;
        }
    }
    else if (strcmp("DEFAULT.FIRMWARE_TAG", &toUpperToken[0]) == 0)
    {
        if((numParams > 32))
            return ERROR_WRONG_PARAMS;

        for (i=0; i < numParams; i++)
        {
            *((uint8*) (app_data + offset)) = (uint8)(params[i] & 0xFF);
            offset ++;
        }

        //Append '\0' character if string length less than 32
        if(i < 32)
            *((uint8*) (app_data + offset)) = '\0';
    }
    else
    {
        //Store the trigger Mode
        if (strcmp("DEFAULT.PATTERNCONFIG.TRIG_MODE", &toUpperToken[0]) == 0)
        {
            trigMode = (uint8) params[0];
        }

        if (numParams > 1)
            return ERROR_WRONG_PARAMS;

        switch(g_iniParam_Info[index].frmw_size)
        {
        case 1:
            *((uint8 *)(app_data + offset)) = (uint8) params[0];
            break;
        case 2:
            *((uint16 *)(app_data + offset)) = (uint16) params[0];
            break;
        case 3:
            *((uint8 *)(app_data + offset + 0)) = (uint8) ((params[0] >> 0) & 0xFF);
            *((uint8 *)(app_data + offset + 1)) = (uint8) ((params[0] >> 8) & 0xFF);
            *((uint8 *)(app_data + offset + 2)) = (uint8) ((params[0] >> 16) & 0xFF);
            break;
        case 4:
            *((uint32 *)(app_data + offset)) = (uint32) params[0];
            break;
        default:
            return ERROR_WRONG_PARAMS;
        }
    }
    return 0;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500Frmw::Frmw_GetSplashCount()
{
	uint32 splash_data_start_address = m_splash_data_start_flash_address - FLASH_BASE_ADDRESS;
    SPLASH_SUPER_BINARY_INFO binary_info;

    memcpy(&binary_info, m_pFrmwImageArray + splash_data_start_address, sizeof(binary_info));

    /* The GUI supports pattern image display for firmware images built with DLPC350_CONFIG.exe alone */
    if ((binary_info.Sig1 == 0x12345678) && (binary_info.Sig2 == 0x87654321))
        return binary_info.BlobCount;
    else
        return -1;
}
//-------------------------------------------------------------------------------------//
uint32 CLight3DTiDLP_Imp_4500Frmw::Frmw_GetSPlashFlashStartAddress()
{
	return m_splash_data_start_flash_address;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500Frmw::Frmw_GetSpashImage(unsigned char *pImageBuffer, int index)
{
	SPLASH_BLOB_INFO blob_info;
    uint32 blob_address, splash_image_address, splash_image_size;
    SPLASH_HEADER splash_header;
    unsigned char *image_buffer, *lineData;
    int i, j, lineLength;
    uint32 splash_data_start_address = m_splash_data_start_flash_address - FLASH_BASE_ADDRESS;

    blob_address = splash_data_start_address + sizeof(SPLASH_SUPER_BINARY_INFO) + index * sizeof(blob_info);
    memcpy(&blob_info, m_pFrmwImageArray + blob_address, sizeof(blob_info));

    if (blob_info.BlobOffset == 0xffffffff)
        return ERROR_NO_SPLASH_IMAGE;

    if(blob_info.BlobOffset < FLASH_BASE_ADDRESS)
    {
        blob_info.BlobOffset += 0x03000000;
    }

    splash_image_address = blob_info.BlobOffset - FLASH_BASE_ADDRESS;
    memcpy(&splash_header, m_pFrmwImageArray + splash_image_address, sizeof(splash_header));
    splash_image_address += sizeof(splash_header);

    splash_image_size = blob_info.BlobSize - sizeof(splash_header);

    if (splash_header.Compression == SPLASH_4LINE_COMPRESSION)
    {
        unsigned char *fourLine_buffer;

        fourLine_buffer = (unsigned char *)malloc(splash_image_size);
        if(fourLine_buffer == NULL)
            return ERROR_NO_MEM_FOR_MALLOC;

        memcpy(fourLine_buffer, m_pFrmwImageArray + splash_image_address, splash_image_size);

        image_buffer =  (unsigned char *)malloc(splash_header.Image_width * splash_header.Image_height * 3); // We only support 24 bit format.
        if(image_buffer == NULL)
            return ERROR_NO_MEM_FOR_MALLOC;

        for (i = 0; i < splash_header.Image_height; i+=4)
            memcpy((image_buffer + (splash_header.Image_width * 3 * i)), fourLine_buffer, splash_image_size);

        splash_image_size = splash_header.Image_width * splash_header.Image_height * 3;
    }
    else if (splash_header.Compression == SPLASH_RLE_COMPRESSION)
    {
        unsigned char* rle_buffer;

        rle_buffer = (unsigned char *)malloc(splash_image_size);
        if(rle_buffer == NULL)
            return ERROR_NO_MEM_FOR_MALLOC;

        image_buffer =  (unsigned char *)malloc(splash_header.Image_width * splash_header.Image_height * 3); // We only support 24 bit format.
        if(image_buffer == NULL)
            return ERROR_NO_MEM_FOR_MALLOC;

        memcpy(rle_buffer, m_pFrmwImageArray + splash_image_address, splash_image_size);

        SPLASH_PerformRLEUnCompression(rle_buffer, image_buffer, &splash_image_size);
        free(rle_buffer);
    }
    else if (splash_header.Compression == SPLASH_UNCOMPRESSED)
    {
        image_buffer = (unsigned char *)malloc(splash_image_size);
        if(image_buffer == NULL)
            return ERROR_NO_MEM_FOR_MALLOC;

        memcpy(image_buffer, m_pFrmwImageArray + splash_image_address, splash_image_size);
    }

    lineLength = splash_header.Image_width * 3;

    lineData = (unsigned char *) malloc(lineLength);
    for (i = 0; i < splash_header.Image_height; i++)
    {
        memcpy(lineData, image_buffer + (lineLength * i), lineLength);
        for (j = 0;j < splash_header.Image_width; j++)
        {
            unsigned char tempByte;

            tempByte = lineData[j * 3 + 2];
            lineData[j * 3 + 2] = lineData[j * 3 + 1];
            lineData[j * 3 + 1] = tempByte;
        }
        memcpy(image_buffer + (lineLength * i), lineData, lineLength);
    }

    free(lineData);
    memcpy(pImageBuffer, image_buffer, splash_image_size);
    free(image_buffer);

    return 0;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500Frmw::Frmw_SPLASH_InitBuffer(int numSplash)
{
	SPLASH_SUPER_BINARY_INFO	binary_info;
    SPLASH_BLOB_INFO		blob_info;
    uint32 i;

    if(numSplash > MAX_SPLASH_IMAGES)
        return -1;

    if(m_splBuffer != NULL)
    {
        free(m_splBuffer);
        m_splBuffer = NULL;
    }
    m_splash_index = 0;
    m_splash_count = 0;

    binary_info.Sig1 = 0x12345678;
    binary_info.Sig2 = 0x87654321;
    binary_info.BlobCount = numSplash;

    blob_info.BlobOffset = 0xFFFFFFFF;
    blob_info.BlobSize   = 0xFFFFFFFF;

    //Allocate memory for splash images
    m_splBuffer = (unsigned char *) malloc(sizeof(binary_info) + (sizeof(blob_info) * MAX_SPLASH_IMAGES));

    memset(m_splBuffer,0x00,(sizeof(binary_info) + (sizeof(blob_info) * MAX_SPLASH_IMAGES)));

    memcpy(m_splBuffer + m_splash_index, &binary_info, sizeof(binary_info));
    m_splash_index += sizeof(binary_info);

    for(i = 0; i < (unsigned int)MAX_SPLASH_IMAGES; i++)
    {
        memcpy(m_splBuffer + m_splash_index, &blob_info, sizeof(blob_info));
        m_splash_index += sizeof(blob_info);
    }

    return 0;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500Frmw::Frmw_SPLASH_AddSplash(unsigned char *pImageBuffer, uint8 *compression, uint32 *compSize)
{
	uint32 lineCompSize, rleCompSize;

    BITMAPINFOHEADER headerInfo;
    unsigned char *bitmapImage, *line1Data, *line2Data, *splashImage;
    int lineLength, bytesPerPixel, i, j;
    uint32 splashSize;
    SPLASH_HEADER splash_header;
    SPLASH_BLOB_INFO *blob_info;
    unsigned short bfType;
    unsigned int bfSize, bfOffBits;

    if((!m_splBuffer || !m_splash_data_start_flash_address))
        return ERROR_INIT_NOT_DONE_PROPERLY;

    memcpy(&bfType, pImageBuffer, sizeof(bfType));
    memcpy(&bfSize, pImageBuffer + sizeof(bfType), sizeof(bfSize));
    memcpy(&bfOffBits, pImageBuffer + 3*sizeof(bfType) + sizeof(bfSize), sizeof(bfOffBits));
    memcpy(&headerInfo, pImageBuffer + BMP_FILE_HEADER_SIZE, sizeof(headerInfo));

    if (bfType != 0x4D42)
        return ERROR_NOT_BMP_FILE;
    if(headerInfo.biBitCount != 24)// && headerInfo.biBitCount != 32)
    {
        return ERROR_NOT_24bit_BMP_FILE;
    }

    bitmapImage = (unsigned char *)malloc(bfSize - bfOffBits);
    if (!bitmapImage)
        return ERROR_NO_MEM_FOR_MALLOC;

    memcpy(bitmapImage, pImageBuffer + bfOffBits, bfSize - bfOffBits);

    bytesPerPixel = headerInfo.biBitCount / 8;

    lineLength    =  headerInfo.biWidth * bytesPerPixel;

    if(lineLength % 4 != 0)
    {
        lineLength = (lineLength / 4 + 1) * 4;
    }
    line1Data = (unsigned char *)malloc(lineLength);

    if(line1Data == NULL)
    {
        free(bitmapImage);
        return ERROR_NO_MEM_FOR_MALLOC;
    }

    line2Data = (unsigned char *)malloc(lineLength);

    if(line2Data == NULL)
    {
        free(line1Data);
        free(bitmapImage);
        return ERROR_NO_MEM_FOR_MALLOC;
    }

    // vertically flip the bitmap image
    for(i = 0; i < (headerInfo.biHeight / 2); i++)
    {
        memcpy(line1Data, bitmapImage + (lineLength * i), lineLength);
        memcpy(line2Data, bitmapImage + (lineLength * (headerInfo.biHeight - i - 1)), lineLength);

        unsigned char tempbyte;

        for(j = 0; j < headerInfo.biWidth; j++)
        {
            
            tempbyte = line1Data[j * 3 + 2];
            line1Data[j * 3 + 2] = line1Data[j * 3 + 1];
            line1Data[j * 3 + 1] = tempbyte;

            tempbyte = line2Data[j * 3 + 2];
            line2Data[j * 3 + 2] = line2Data[j * 3 + 1];
            line2Data[j * 3 + 1] = tempbyte;
        }

        memcpy(bitmapImage + (lineLength * (headerInfo.biHeight - i - 1)), line1Data, lineLength);
        memcpy(bitmapImage + (lineLength * i), line2Data, lineLength);
    }

    free(line1Data);
    free(line2Data);

    unsigned char *rleBuffer = (unsigned char *)malloc((((headerInfo.biHeight * headerInfo.biWidth * bytesPerPixel) +
                                                         ((headerInfo.biHeight * headerInfo.biWidth * 4) / 255) + (headerInfo.biWidth * 2) + 15) - 1));

    if (rleBuffer == NULL)
    {
        free(bitmapImage);
        return ERROR_NO_MEM_FOR_MALLOC;
    }

    switch(*compression)
    {
    case 0: // force uncompress
        splashSize  = headerInfo.biHeight * lineLength;
        splashImage = bitmapImage;
        break;

    case 1: // force rle compress
        SPLASH_PerformRLECompression(bitmapImage, rleBuffer, headerInfo.biWidth, headerInfo.biHeight, &splashSize);
        splashImage = rleBuffer;
        break;

    case 4: // force 4 line compress
        splashSize  = 4 * lineLength;
        splashImage = bitmapImage;
        break;

    default: // auto compression

        SPLASH_PerformLineCompression(bitmapImage, headerInfo.biWidth, headerInfo.biHeight, &lineCompSize, 4);
        SPLASH_PerformRLECompression(bitmapImage, rleBuffer, headerInfo.biWidth, headerInfo.biHeight, &rleCompSize);

        splashSize  = headerInfo.biHeight * lineLength;

        if(lineCompSize < splashSize)
        {
            splashSize  = 4 * lineLength;
            splashImage = bitmapImage;
            *compression = 4;
        }
        else if(rleCompSize < splashSize)
        {
            splashSize  = rleCompSize;
            splashImage = rleBuffer;
            *compression    = 1;
        }
        else
        {
            splashSize  = headerInfo.biHeight * lineLength;
            splashImage = bitmapImage;
            *compression    = 0;
        }

        break;
    }

    splash_header.Signature		= 0x636C7053;
    splash_header.Image_width	= (uint16)headerInfo.biWidth;
    splash_header.Image_height	= (uint16)headerInfo.biHeight;
    splash_header.Pixel_format	= 1; // 24-bit packed
    splash_header.Subimg_offset = -1;
    splash_header.Subimg_end	= -1;
    splash_header.Bg_color		= 0;
    splash_header.ByteOrder		= 1;
    splash_header.ChromaOrder	= 0;
    splash_header.Byte_count	= splashSize;
    splash_header.Compression	= *compression;

    blob_info = (SPLASH_BLOB_INFO *)(m_splBuffer + sizeof(SPLASH_SUPER_BINARY_INFO) + (m_splash_count * sizeof(SPLASH_BLOB_INFO)));

    uint32 FlashEnd, currChipSelect;
    int nextChipSelect;

    if(m_ChipSelectSize[0] != 0)
        FlashEnd = FLASH_THREE_ADDRESS + m_ChipSelectSize[0];
    else if(m_ChipSelectSize[2] != 0)
        FlashEnd = FLASH_TWO_ADDRESS   + m_ChipSelectSize[2];
    else if(m_ChipSelectSize[1] != 0)
        FlashEnd = FLASH_BASE_ADDRESS  + m_ChipSelectSize[1];

    m_ChipSelectEnd[0] = FLASH_THREE_ADDRESS + m_ChipSelectSize[0];
    m_ChipSelectEnd[1] = FLASH_BASE_ADDRESS  + m_ChipSelectSize[1];
    m_ChipSelectEnd[2] = FLASH_TWO_ADDRESS   + m_ChipSelectSize[2];

    if((m_splash_index + m_splash_data_start_flash_address + sizeof(splash_header) + splashSize) < FlashEnd)
    {
        nextChipSelect = -1;
        if((m_splash_index + m_splash_data_start_flash_address) < m_ChipSelectEnd[1] &&
                (m_splash_index + m_splash_data_start_flash_address + sizeof(splash_header) + splashSize) > m_ChipSelectEnd[1] &&
                (m_ChipSelectSize[1] != 0x01000000))
        {
            currChipSelect = 1;

            if(m_ChipSelectSize[2] != 0)
                nextChipSelect = 2;
            else if(m_ChipSelectSize[0] != 0)
                nextChipSelect = 0;
        }

        if((m_splash_index + m_splash_data_start_flash_address) < m_ChipSelectEnd[2] &&
                (m_splash_index + m_splash_data_start_flash_address + sizeof(splash_header) + splashSize) > m_ChipSelectEnd[2])
        {
            currChipSelect = 2;
            nextChipSelect = 0;
        }

        if(nextChipSelect != -1)
        {
            //printf("OVERFLOW FLASH_CS%d, MOVING SPLASH [%d] DATA TO FLASH_CS%d \n", currChipSelect, m_splash_count, nextChipSelect);
            //printf("BYTES UNUSED IN FLASH_CS%d = 0x%08X <%d> bytes\n", currChipSelect, m_ChipSelectEnd[currChipSelect] -
            //       m_splash_data_start_flash_address - m_splash_index, m_ChipSelectEnd[currChipSelect] - m_splash_data_start_flash_address - m_splash_index);

            m_splBuffer = (unsigned char *)realloc(m_splBuffer, m_ChipSelectBase[nextChipSelect] - m_splash_data_start_flash_address);

            memset(m_splBuffer + m_splash_index, 0xFF, m_ChipSelectBase[nextChipSelect] - m_splash_data_start_flash_address - m_splash_index);

            m_splash_index = m_ChipSelectBase[nextChipSelect] - m_splash_data_start_flash_address;

            blob_info = (SPLASH_BLOB_INFO *)(m_splBuffer + sizeof(SPLASH_SUPER_BINARY_INFO) + (m_splash_count * sizeof(SPLASH_BLOB_INFO)));
        }
        blob_info->BlobOffset = m_splash_index + m_splash_data_start_flash_address;
        blob_info->BlobSize   = sizeof(splash_header) + splashSize;

        /* check if it is crossing the 3rd chipselect, if yes remap it to 0th chip select */
        if(blob_info->BlobOffset >= FLASH_THREE_ADDRESS)
        {
            blob_info->BlobOffset -= 0x03000000;
        }

        m_splBuffer = (unsigned char *)realloc(m_splBuffer, m_splash_index + sizeof(splash_header) + splashSize);

        memcpy(m_splBuffer + m_splash_index, &splash_header, sizeof(splash_header));

        m_splash_index += sizeof(splash_header);

        memcpy(m_splBuffer + m_splash_index, splashImage, splashSize);

        m_splash_index += splashSize;

        m_splash_count++;
    }
    else
    {
        m_splash_count++;
        // printf("NO SPACE LEFT IN THE FLASH CAN'T WRITE SPLASH [%d]\n", m_splash_count);
        return ERROR_NO_SPACE_IN_FRMW;
    }

    free(rleBuffer);
    free(bitmapImage);
    *compSize = splashSize;
    return 0;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500Frmw::Frmw_Get_NewFlashImage(unsigned char **newFrmwbuffer, uint32 *newFrmwsize)
{
	uint32 newfrmFileInLen = (m_splash_data_start_flash_address - FLASH_BASE_ADDRESS) + m_splash_index;

    m_pFrmwImageArray	= (unsigned char *)realloc(m_pFrmwImageArray, newfrmFileInLen);
    memcpy(m_pFrmwImageArray + (m_splash_data_start_flash_address - FLASH_BASE_ADDRESS), m_splBuffer, m_splash_index);

    *newFrmwbuffer = m_pFrmwImageArray;
    *newFrmwsize = newfrmFileInLen;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500Frmw::Frmw_Get_NewSplashBuffer(unsigned char **newSplashBuffer, uint32 *newSplashSize)
{
	*newSplashBuffer = m_splBuffer;
    *newSplashSize = m_splash_index;
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500Frmw::Frmw_UpdateFlashTableSplashAddress(unsigned char *flashTableSectorBuffer, uint32 address_offset)
{
	FLASH_TABLE *flash_table;
    unsigned char *temp_flashTableSector = (unsigned char*) malloc(128 * 1024);

    m_splash_data_start_flash_address = address_offset + FLASH_BASE_ADDRESS;
    memcpy(temp_flashTableSector, m_pFrmwImageArray + m_FLASH_TABLE_ADDRESS, 128 * 1024);
    flash_table = (FLASH_TABLE *)(temp_flashTableSector);

    flash_table->Splash_Data[FLASH_TABLE_SPLASH_INDEX].Address = address_offset + FLASH_BASE_ADDRESS;
    memcpy(flashTableSectorBuffer, temp_flashTableSector, 128 * 1024);

    free(temp_flashTableSector);
}
//-------------------------------------------------------------------------------------//
void CLight3DTiDLP_Imp_4500Frmw::Frmw_GetCurrentIniLineParam(char *token, uint32 *params, int *numParams)
{
	unsigned int i = 0;

    //Point to the m_firstIniToken
    strcpy(token,&m_firstIniToken[0]);

    for (i = 0; i < m_numIniParams; i++)
        params[i] = m_iniParams[i];

    *numParams = m_numIniParams;

    return;
}
//-------------------------------------------------------------------------------------//
int CLight3DTiDLP_Imp_4500Frmw::Frmw_ParseIniLines(char *line)
{
	const char space[2] = " ";
    char *token;
    unsigned int tmpIntVar;
    bool isParamNameRcvd = false;

    m_numIniParams = 0;

    /* get the first token */
    token = strtok(&line[0], &space[0]);

    /* walk through other tokens */
    while( token != NULL )
    {

        //if end of data to be interpreted
        if(token[0] == ';')
            break;

        //check if it token is string
        if(sscanf(token,"%i",&tmpIntVar))
        {
            if(isParamNameRcvd)
            {
                m_iniParams[m_numIniParams] = tmpIntVar;
                m_numIniParams++;
            }
        }
        else
        {
            //Interpret the token only if it is a valid string
            if((*token == '\0') || (*token == '\t') || (*token == '\n') || (*token == ' '))
            {
               continue;
            }
            else
            {
                //Copy the param name
                strcpy(m_firstIniToken,token);

                isParamNameRcvd = true;

                //Remove spaces or tabs in the m_firstIniToken
                char *a = m_firstIniToken;

                while(1)
                {
                 if((*a == '\0') || (*a == '\t') || (*a == '\n') || (*a == ' '))
                     break;
                 else
                     a++;
                }

                *a = '\0';
            }
        }

        token = strtok(NULL, space);
    }

    //If there is no valid token in the line
    if(strcmp(m_firstIniToken," ") == 0)
        return -1;

#if 0
    unsigned int i;
    printf("Token = %s\n",str);
    printf("Number of params = %d\n",m_numIniParams);
    for(i=0;i<m_numIniParams;i++)
        printf("0x%06X\t",m_iniParams[i]);
#endif

    return 0;
}
//-------------------------------------------------------------------------------------//
#endif//LIGHT_3D_TI_DLP_USE_IMP_4500
//-------------------------------------------------------------------------------------//
