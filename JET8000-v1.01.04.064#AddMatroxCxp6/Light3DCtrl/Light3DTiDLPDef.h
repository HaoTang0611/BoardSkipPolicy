#ifndef LIGHT_3D_TI_DLP_DEF_H_
#define LIGHT_3D_TI_DLP_DEF_H_
//-------------------------------------------------------------------------------------//
#pragma once
//-------------------------------------------------------------------------------------//
/* Bit masks. */
#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80
#define BIT8      0x0100
#define BIT9      0x0200
#define BIT10     0x0400
#define BIT11     0x0800
#define BIT12     0x1000
#define BIT13     0x2000
#define BIT14     0x4000
#define BIT15     0x8000
#define BIT16 0x00010000
#define BIT17 0x00020000
#define BIT18 0x00040000
#define BIT19 0x00080000
#define BIT20 0x00100000
#define BIT21 0x00200000
#define BIT22 0x00400000
#define BIT23 0x00800000
#define BIT24 0x01000000
#define BIT25 0x02000000
#define BIT26 0x04000000
#define BIT27 0x08000000
#define BIT28 0x10000000
#define BIT29 0x20000000
#define BIT30 0x40000000
#define BIT31 0x80000000
//-------------------------------------------------------------------------------------//
#define DLP_TRIG_TYPE_INTERNAL	0
#define DLP_TRIG_TYPE_EXT_POS	1
#define DLP_TRIG_TYPE_EXT_NEG	2
#define DLP_TRIG_TYPE_NO_TRIG	3
//-------------------------------------------------------------------------------------//
#define DLP_DISPLAY_MODE_VIDEO		0
#define DLP_DISPLAY_MODE_PATTERN	1
//-------------------------------------------------------------------------------------//
#define DLP_POWER_MODE_NORMAL		0
#define DLP_POWER_MODE_STANDBY		1
//-------------------------------------------------------------------------------------//
#define DLP_LED_TRIGGER_INTERNAL		0 // Internal trigger
#define DLP_LED_TRIGGER_EXTERNAL_POS	1 // External positive
#define DLP_LED_TRIGGER_EXTERNAL_NEG	2 // External negative
#define DLP_LED_TRIGGER_NO_INPUT		3 // No Input Trigger
//-------------------------------------------------------------------------------------//
#define DLP_LED_COLOR_NO		0		// No LED (Pass Through)
#define DLP_LED_COLOR_RED		1		// Red
#define DLP_LED_COLOR_GREEN		2		// Green
#define DLP_LED_COLOR_YELLOW	3		// Yellow (Green + Red)
#define DLP_LED_COLOR_BLUE		4		// Blue
#define DLP_LED_COLOR_MAGENTA	5		// Magenta (Blue + Red)
#define DLP_LED_COLOR_CYAN		6		// Cyan (Blue + Green)
#define DLP_LED_COLOR_WHITE		7		// White (Red + Blue + Green)
#define DLP_LED_COLOR_DEBUG		99		// Debug
//-------------------------------------------------------------------------------------//
#define DLP_LED_CURRENT_ID_01               1//DLP LED電流編號-1
#define DLP_LED_CURRENT_ID_02               2//DLP LED電流編號-2
//-------------------------------------------------------------------------------------//
#define DLP_PATTERN_EXP_NUM_01              1 //DLP Pattern曝光數量-1
#define DLP_PATTERN_EXP_NUM_02              2 //DLP Pattern曝光數量-2
//-------------------------------------------------------------------------------------//
#define DLP_PATTERN_SEQUENCE_NONE           0 //DLP Pattern序列-關閉
#define DLP_PATTERN_SEQUENCE_WHITE          1 //DLP Pattern序列-白燈
#define DLP_PATTERN_SEQUENCE_RGB            2 //DLP Pattern序列-紅綠藍燈
#define DLP_PATTERN_SEQUENCE_2_2_M         29 //DLP Pattern序列-2+2步2週期-合成相位-Joe
#define DLP_PATTERN_SEQUENCE_4_4_1         41 //DLP Pattern序列-4+4步2週期-第1相位
#define DLP_PATTERN_SEQUENCE_4_4_2         42 //DLP Pattern序列-4+4步2週期-第2相位
#define DLP_PATTERN_SEQUENCE_4_5GC_M       45 //DLP Pattern序列-4步+5GC2週期-合成相位
#define DLP_PATTERN_SEQUENCE_4_6GC_M       46 //DLP Pattern序列-4步+6GC2週期-合成相位
#define DLP_PATTERN_SEQUENCE_4_4GC_M       47 //DLP Pattern序列-4步+4GC2週期-合成相位
#define DLP_PATTERN_SEQUENCE_4_2_M         58 //DLP Pattern序列-4+2步2週期-合成相位
#define DLP_PATTERN_SEQUENCE_4_4_M         49 //DLP Pattern序列-4+4步2週期-合成相位
#define DLP_PATTERN_SEQUENCE_4_4_1_2      141 //DLP Pattern序列-4+4步2週期-第1相位-2曝光
#define DLP_PATTERN_SEQUENCE_4_4_2_2      142 //DLP Pattern序列-4+4步2週期-第2相位-2曝光
#define DLP_PATTERN_SEQUENCE_4_5GC_M2     145 //DLP Pattern序列-4步+5GC2週期2曝光-合成相位
#define DLP_PATTERN_SEQUENCE_4_6GC_M2     146 //DLP Pattern序列-4步+6GC2週期2曝光-合成相位
#define DLP_PATTERN_SEQUENCE_4_4GC_M2     147 //DLP Pattern序列-4步+4GC2週期2曝光-合成相位
#define DLP_PATTERN_SEQUENCE_4_2_M_2      158 //DLP Pattern序列-4+2步2週期2曝光-合成相位
#define DLP_PATTERN_SEQUENCE_4_4_M_2      149 //DLP Pattern序列-4+4步2週期2曝光-合成相位
#define DLP_PATTERN_SEQUENCE_DEBUG         99 //DLP Pattern序列-偵錯, 自由指定
//-------------------------------------------------------------------------------------//
#define LIGHT3D_PHASE_2_2_1                221//DLP 相位模式2+2步2周期-第1週期-Joe
#define LIGHT3D_PHASE_2_2_2                222//DLP 相位模式2+2步2周期-第2週期-Joe
#define LIGHT3D_PHASE_2_2_M                229//DLP 相位模式2+2步2周期-第2週期-Joe
#define LIGHT3D_PHASE_2_2_M_2              2292//DLP 相位模式2+2步2周期-第2週期2曝光-Joe

#define LIGHT3D_PHASE_3_3_1                331//DLP 相位模式3步2周期-第1週期
#define LIGHT3D_PHASE_3_3_2                332//DLP 相位模式3步2周期-第2週期
#define LIGHT3D_PHASE_3_3_M                339//DLP 相位模式3步2周期-合成週期
#define LIGHT3D_PHASE_3_3_1_2              3312//DLP 相位模式3步2周期-第1週期2曝光
#define LIGHT3D_PHASE_3_3_2_2              3322//DLP 相位模式3步2周期-第1週期2曝光
#define LIGHT3D_PHASE_3_3_M_2              3392//DLP 相位模式3步2周期-第1週期2曝光

#define LIGHT3D_PHASE_4_4_1                441//DLP 相位模式4+4步2周期-第1週期
#define LIGHT3D_PHASE_4_4_2                442//DLP 相位模式4+4步2周期-第2週期
#define LIGHT3D_PHASE_4_2_M                429//DLP 相位模式4+2步2周期-合成週期
#define LIGHT3D_PHASE_4_4_M                449//DLP 相位模式4+4步2周期-合成週期
#define LIGHT3D_PHASE_4_5GC_M              459//DLP 相位模式4步+5GC2周期-合成週期
#define LIGHT3D_PHASE_4_6GC_M              469//DLP 相位模式4步+6GC2周期-合成週期
#define LIGHT3D_PHASE_4_4GC_M              479//DLP 相位模式4步+4GC2周期-合成週期
#define LIGHT3D_PHASE_4_4_1_2              4412//DLP 相位模式4步2周期-第1週期2曝光
#define LIGHT3D_PHASE_4_4_2_2              4422//DLP 相位模式4步2周期-第2週期2曝光
#define LIGHT3D_PHASE_4_2_M_2              4292//DLP 相位模式4+2步2周期-合成週期2曝光
#define LIGHT3D_PHASE_4_4_M_2              4492//DLP 相位模式4步2周期-合成週期2曝光
#define LIGHT3D_PHASE_4_5GC_M_2            4592//DLP 相位模式4步+5GC2周期-合成週期2曝光
#define LIGHT3D_PHASE_4_6GC_M_2            4692//DLP 相位模式4步+6GC2周期-合成週期2曝光
#define LIGHT3D_PHASE_4_4GC_M_2            4792//DLP 相位模式4步+4GC2周期-合成週期2曝光

#define LIGHT3D_PHASE_5_5_1                551//DLP 相位模式5步2周期-第1週期
#define LIGHT3D_PHASE_5_5_2                552//DLP 相位模式5步2周期-第2週期
#define LIGHT3D_PHASE_5_5_M                559//DLP 相位模式5步2周期-合成週期
#define LIGHT3D_PHASE_5_5_1_2              5512//DLP 相位模式5步2周期-第1週期2曝光
#define LIGHT3D_PHASE_5_5_2_2              5522//DLP 相位模式5步2周期-第2週期2曝光
#define LIGHT3D_PHASE_5_5_M_2              5592//DLP 相位模式5步2周期-合成週期2曝光
//-------------------------------------------------------------------------------------//
#define MIN_PAT_EXP_TIME					3000	//最低Pattern曝光時間	//chia 1050331
#define DLP_SPLASH_LUT_MAX                  64      //最多splash的lut數量
#define DLP_SPLASH_EXP_LUT_MAX             256      //最多splash的exp-lut數量
//-------------------------------------------------------------------------------------//

#endif//LIGHT_3D_TI_DLP_DEF_H_
