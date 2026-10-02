/*
 * dlpc350_api.cpp
 *
 * This module provides C callable APIs for each of the command supported
 * by LightCrafter4500 platform and detailed in the programmer's guide.
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
#include <string.h>

//#include "dlpc350_common.h"
//#include "dlpc350_api.h"
//#include "dlpc350_usb.h"

static CmdFormat CmdList[255] =
{
    {   0x07,  0x1C,  0x1C   },      //VID_SIG_STAT,
    {   0x1A,  0x00,  0x01   },      //SOURCE_SEL,
    {   0x1A,  0x02,  0x01   },      //PIXEL_FORMAT,
    {   0x1A,  0x03,  0x01   },      //CLK_SEL,
    {   0x1A,  0x37,  0x01   },      //CHANNEL_SWAP,
    {   0x1A,  0x04,  0x01   },      //FPD_MODE,
    {   0,  0,  0   },      //CURTAIN_COLOR,
    {   0x02,  0x00,  0x01   },      //POWER_CONTROL,
    {   0x10,  0x08,  0x01   },      //FLIP_LONG,
    {   0x10,  0x09,  0x01   },      //FLIP_SHORT,
    {   0x12,  0x03,  0x01   },      //TPG_SEL,
    {   0x1A,  0x05,  0x01   },      //PWM_INVERT,
    {   0x1A,  0x07,  0x01   },      //LED_ENABLE,
    {   0x02,  0x05,  0x00   },      //GET_VERSION,
    {   0x1A,  0xFF,  0x00   },      //GET_FIRMWAE_TAG_INFO
    {   0x08,  0x02,  0x00   },      //SW_RESET,
    {   0,  0,  0   },      //DMD_PARK,
    {   0x10,  0x0A,  0x01   },      //BUFFER_FREEZE,
    {   0x1A,  0x0A,  0x00   },      //STATUS_HW,
    {   0x1A,  0x0B,  0x00   },      //STATUS_SYS,
    {   0x1A,  0x0C,  0x00   },      //STATUS_MAIN,
    {   0,  0,  0   },      //CSC_DATA,
    {   0,  0,  0   },      //GAMMA_CTL,
    {   0,  0,  0   },      //BC_CTL,
    {   0x1A,  0x10,  0x01   },      //PWM_ENABLE,
    {   0x1A,  0x11,  0x06   },      //PWM_SETUP,
    {   0x1A,  0x12,  0x05   },      //PWM_CAPTURE_CONFIG,
    {   0x1A,  0x38,  0x02   },      //GPIO_CONFIG,
    {   0x0B,  0x01,  0x03   },      //LED_CURRENT,
    {   0x10,  0x00,  0x10   },      //DISP_CONFIG,
    {   0,  0,  0   },      //TEMP_CONFIG,
    {   0,  0,  0   },      //TEMP_READ,
    {   0x1A,  0x16,  0x09   },      //MEM_CONTROL,
    {   0,  0,  0   },      //I2C_CONTROL,
    {   0x1A,  0x1A,  0x01   },      //LUT_VALID,
    {   0x1A,  0x1B,  0x01   },      //DISP_MODE,
    {   0x1A,  0x1D,  0x03   },      //TRIG_OUT1_CTL,
    {   0x1A,  0x1E,  0x02   },      //TRIG_OUT2_CTL,
    {   0x1A,  0x1F,  0x02   },      //RED_STROBE_DLY,
    {   0x1A,  0x20,  0x02   },      //GRN_STROBE_DLY,
    {   0x1A,  0x21,  0x02   },      //BLU_STROBE_DLY,
    {   0x1A,  0x22,  0x01   },      //PAT_DISP_MODE,
    {   0x1A,  0x23,  0x01   },      //PAT_TRIG_MODE,
    {   0x1A,  0x24,  0x01   },      //PAT_START_STOP,
    {   0,  0,  0   },      //BUFFER_SWAP,
    {   0,  0,  0   },      //BUFFER_WR_DISABLE,
    {   0,  0,  0   },      //CURRENT_RD_BUFFER,
    {   0x1A,  0x29,  0x08   },      //PAT_EXPO_PRD,
    {   0x1A,  0x30,  0x01   },      //INVERT_DATA,
    {   0x1A,  0x31,  0x04   },      //PAT_CONFIG,
    {   0x1A,  0x32,  0x01   },      //MBOX_ADDRESS,
    {   0x1A,  0x33,  0x01   },      //MBOX_CONTROL,
    {   0x1A,  0x34,  0x00   },      //MBOX_DATA,
    {   0x1A,  0x35,  0x04   },      //TRIG_IN1_DELAY,
    {   0x1A,  0x36,  0x01   },      //TRIG_IN2_CONTROL,
    {   0x1A,  0x39,  0x01   },      //IMAGE_LOAD,
    {   0x1A,  0x3A,  0x02   },      //IMAGE_LOAD_TIMING,
    {   0x1A,  0x3B,  0x00   },      //I2C0_CTRL,
    {   0x1A,  0x3E,  0x0C   },      //MBOX_EXP_DATA,
    {   0x1A,  0x3F,  0x02   },      //MBOX_EXP_ADDRESS,
    {   0x1A,  0x40,  0x06   },      //EXP_PAT_CONFIG
    {   0x1A,  0x42,  0x01   },      //NUM_IMAGE_IN_FLASH,
    {   0x1A,  0x43,  0x01   },      //I2C0_STAT,
    {   0x08,  0x07,  0x03   },      //GPCLK_CONFIG,
    {   0,  0,  0   },      //PULSE_GPIO_23,
    {   0,  0,  0   },      //ENABLE_DLPC350_DEBUG,
    {   0x12,  0x04,  0x0C   },      //TPG_COLOR,
    {   0x1A,  0x13,  0x05   },     //PWM_CAPTURE_READ,
    {   0x30,  0x01,  0x00   },     //PROG_MODE,
    {   0x00,  0x00,  0x00   },     //BL_STATUS
    {   0x00,  0x23,  0x01   },     //BL_SPL_MODE
    {   0x00,  0x15,  0x01   },     //BL_GET_MANID,
    {   0x00,  0x15,  0x01   },     //BL_GET_DEVID,
    {   0x00,  0x15,  0x01   },     //BL_GET_CHKSUM,
    {   0x00,  0x29,  0x04   },     //BL_SETSECTADDR,
    {   0x00,  0x28,  0x00   },     //BL_SECT_ERASE,
    {   0x00,  0x2C,  0x04   },     //BL_SET_DNLDSIZE,
    {   0x00,  0x25,  0x00   },     //BL_DNLD_DATA,
    {   0x00,  0x2F,  0x01   },     //BL_FLASH_TYPE,
    {   0x00,  0x26,  0x00   },     //BL_CALC_CHKSUM,
    {   0x00,  0x30,  0x01   }     //BL_PROG_MODE,
};