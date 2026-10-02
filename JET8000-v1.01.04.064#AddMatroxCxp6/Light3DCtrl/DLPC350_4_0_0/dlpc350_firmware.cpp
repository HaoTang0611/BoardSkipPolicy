/*
 * dlpc350_firmware.cpp
 *
 * This module handles building and parsing of firmware images
 *
 * Copyright (C) {2015} Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "dlpc350_common.h"
#include "dlpc350_error.h"
#include "dlpc350_firmware.h"



#ifdef _WIN32
#include <windows.h>
#else
typedef unsigned short WORD;
typedef unsigned int LONG;
typedef unsigned int DWORD;
typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
#endif

#define BMP_FILE_HEADER_SIZE        14

/* Do not change the order of entries. It is used in inisavewindow.cpp to populate the save window with default and gui defined enteries */
static INIPARAM_INFO g_iniParam_Info[] =
{
    {"APPCONFIG.VERSION.SUBMINOR", {0x00}, {0x00}, 1, 1, false, 0, 1},//SK: Remove
    {"APPCONFIG.VERSION.MINOR", {0x00}, {0x00}, 1, 1, false, 1, 1}, //SK: Remove
    {"APPCONFIG.VERSION.MAJOR", {0x03}, {0x00}, 1, 1, false, 2, 1}, //SK:Remove
    //{"APPCONFIG.VERSION.RSERVED", {0x00}, {0x00}, 1, 1, true, 3, 1},
    {"DEFAULT.FIRMWARE_TAG", {0x44, 0x4C, 0x50}, {0x00}, 3, 1, true, 4, 32},
    {"DEFAULT.AUTOSTART", {0x00}, {0x00}, 1, 1, false, 36, 1},
    {"DEFAULT.DISPMODE", {0x00}, {0x00}, 1, 1, true, 37, 1},
    {"DEFAULT.SHORT_FLIP", {0x00}, {0x00}, 1, 1, true, 38, 1},
    {"DEFAULT.LONG_FLIP", {0x00}, {0x00}, 1, 1, true, 39, 1},
    {"DEFAULT.TRIG_OUT_1.POL", {0x00}, {0x00}, 1, 1, true, 104, 1},
    {"DEFAULT.TRIG_OUT_1.RDELAY", {0xBB}, {0xBB}, 1, 1, true, 105, 1},
    {"DEFAULT.TRIG_OUT_1.FDELAY", {0xBB}, {0xBB}, 1, 1, true, 106, 1},
    {"DEFAULT.TRIG_OUT_2.POL", {0x00}, {0x00}, 1, 1, true, 108, 1},
    {"DEFAULT.TRIG_OUT_2.WIDTH", {0xBB}, {0xBB}, 1, 1, true, 109, 1},
    {"DEFAULT.TRIG_IN_1.DELAY", {0x00}, {0x00}, 1, 1, true, 112, 4},
    //{"DEFAULT.TRIG_IN_1.POL", {0x00}, {0x00}, 1, 1, false, 116, 1},
    //{"DEFAULT.TRIG_IN_2.DELAY", {0x00}, {0x00}, 1, 1, false, 120, 4},
    {"DEFAULT.TRIG_IN_2.POL", {0x00}, {0x00}, 1, 1, true, 124, 1},
    {"DEFAULT.RED_STROBE.RDELAY", {0xBB}, {0xBB}, 1, 1, true, 128, 1},
    {"DEFAULT.RED_STROBE.FDELAY", {0xBB}, {0xBB}, 1, 1, true, 129, 1},
    {"DEFAULT.GRN_STROBE.RDELAY", {0xBB}, {0xBB}, 1, 1, true, 132, 1},
    {"DEFAULT.GRN_STROBE.FDELAY", {0xBB}, {0xBB}, 1, 1, true, 133, 1},
    {"DEFAULT.BLU_STROBE.RDELAY", {0xBB}, {0xBB}, 1, 1, true, 136, 1},
    {"DEFAULT.BLU_STROBE.FDELAY", {0xBB}, {0xBB}, 1, 1, true, 137, 1},
    {"DEFAULT.INVERTDATA", {0x00}, {0x00}, 1, 1, true, 140, 1},
    {"DEFAULT.TESTPATTERN", {0x08}, {0x08}, 1, 1, true, 141, 1},
    {"DEFAULT.LEDCURRENT_RED", {0x97}, {0x97}, 1, 1, true, 149, 1},
    {"DEFAULT.LEDCURRENT_GRN", {0x78}, {0x78}, 1, 1, true, 150, 1},
    {"DEFAULT.LEDCURRENT_BLU", {0x7D}, {0x7D}, 1, 1, true, 151, 1},
    {"DEFAULT.PATTERNCONFIG.PAT_EXPOSURE", {0x7A120}, {0x7A120}, 1, 1, true, 156, 4},
    {"DEFAULT.PATTERNCONFIG.PAT_PERIOD", {0x7A120}, {0x7A120}, 1, 1, true, 160, 4},
    {"DEFAULT.PATTERNCONFIG.PAT_MODE", {0x03}, {0x03}, 1, 1, true, 164, 1},
    {"DEFAULT.PATTERNCONFIG.TRIG_MODE", {0x1}, {0x1}, 1, 1, true, 165, 1},
    {"DEFAULT.PATTERNCONFIG.PAT_REPEAT", {0x1}, {0x1}, 1, 1, true, 166, 1},
    {"DEFAULT.PATTERNCONFIG.NUM_LUT_ENTRIES", {0x02}, {0x1}, 1, 1, true, 168, 2},
    {"DEFAULT.PATTERNCONFIG.NUM_PATTERNS", {0x02}, {0x1}, 1, 1, true, 170, 2},
    {"DEFAULT.PATTERNCONFIG.NUM_SPLASH", {0x00}, {0x00}, 1, 1, true, 172, 2},
    {"DEFAULT.SPLASHLUT", {0x01}, {0x0}, 1, 1, true, 176, 256},
    {"DEFAULT.SEQPATLUT", {0x00061800, 0x00022804, 0x00024808}, {0x0}, 3, 1, true, 432, 29184},
    {"DEFAULT.LED_ENABLE_MAN_MODE", {0x0}, {0x0}, 1, 1, true, 29616, 1},
    {"DEFAULT.MAN_ENABLE_RED_LED", {0x0}, {0x0}, 1, 1, true, 29617, 1},
    {"DEFAULT.MAN_ENABLE_GRN_LED", {0x0}, {0x0}, 1, 1, true, 29618, 1},
    {"DEFAULT.MAN_ENABLE_BLU_LED", {0x0}, {0x0}, 1, 1, true, 29619, 1},
    {"DEFAULT.PORTCONFIG.PORT",{0x0}, {0x0},1, 1, true, 40, 1},
    {"DEFAULT.PORTCONFIG.BPP", {0x1}, {0x1}, 1, 1, true, 41, 1},
    {"DEFAULT.PORTCONFIG.PIX_FMT", {0x0}, {0x0}, 1, 1, true, 42, 1},
    {"DEFAULT.PORTCONFIG.PORT_CLK", {0x0}, {0x0}, 1, 1, true, 43, 1},
    //{"DEFAULT.PORTCONFIG.CSC[0]", {0x0400, 0x0000, 0x0000, 0x0000, 0x0400, 0x0000, 0x0000, 0x0000, 0x0400}, {0}, 9, 1, false, 44, 18},
    //{"DEFAULT.PORTCONFIG.CSC[1]", {0x04A8, 0xFDC7, 0xFF26, 0x04A8, 0x0715, 0x0000, 0x04A8, 0x0000, 0x0875}, {0}, 9, 1, false, 62, 18},
    //{"DEFAULT.PORTCONFIG.CSC[2]", {0x04A8, 0xFCC0, 0xFE6F, 0x04A8, 0x0662, 0x0000, 0x04A8, 0x0000, 0x0812}, {0}, 9, 1, false, 80, 18},
    {"DEFAULT.PORTCONFIG.ABC_MUX", {0x4}, {0x4}, 1, 1, true, 100, 1},
    {"DEFAULT.PORTCONFIG.PIX_MODE", {0x1}, {0x1}, 1, 1, true, 101, 1},
    {"DEFAULT.PORTCONFIG.SWAP_POL", {0x1}, {0x1}, 1, 1 ,true, 102, 1},
    {"DEFAULT.PORTCONFIG.FLD_SEL", {0x0}, {0x0}, 1, 1, true, 103, 1},
    {"PERIPHERALS.I2CADDRESS[0]", {0x34}, {0x34}, 1, 1, false, 29649, 1},
    {"PERIPHERALS.I2CADDRESS[1]", {0x3A}, {0x3A}, 1, 1, false, 29650, 1},
    {"PERIPHERALS.I2CBUSYGPIO_ENABLE", {0x00}, {0x00}, 1, 1, true, 29651, 1},
    {"PERIPHERALS.I2CBUSYGPIO_SELECT", {0x00}, {0x00}, 1, 1, true, 29652, 1},
    //{"PERIPHERALS.USB_SRL[0]", {0x004C, 0x0043, 0x0052, 0x0032}, {0x0}, 4, 1, false, 29656, 8},
    //{"PERIPHERALS.USB_SRL[1]", {0x004C, 0x0043, 0x0052, 0x0033}, {0x0}, 4, 1, false, 29664, 8},
    {"DATAPATH.SPLASHSTARTUPTIMEOUT", {0x1388}, {0x1388}, 1, 1, false, 29678, 2},
    {"DATAPATH.SPLASHATSTARTUPENABLE", {0x01}, {0x1}, 1, 1, true, 29682, 1},
    {"MACHINE_DATA.COLORPROFILE_0_BRILLIANTCOLORLOOK", {0x0}, {0x0}, 1, 1, true, 29752, 1},
};

#define FLASH_THREE_ADDRESS					0xFB000000	// actually it is re map to 0xF8000000
#define FLASH_TWO_ADDRESS					0xFA000000
#define FLASH_BASE_ADDRESS					0xF9000000