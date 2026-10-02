#ifndef _AOI_FILE_PID_DEF_H_
#define _AOI_FILE_PID_DEF_H_
//-------------------------------------------------------------------------------------//
#define PID_ID_COMPONENT_NAME          1//零件名稱:str
#define PID_ID_PIN_ID                  2//PIN編號:str
#define PID_ID_BOARD_ID                3//單板編號:int
#define PID_ID_PACKAGE_NAME            4//封裝名稱:str
#define PID_ID_PAD_POS_X_1             5//Pad座標-X1:double
#define PID_ID_PAD_POS_Y_1             6//Pad座標-Y1:double
#define PID_ID_PAD_POS_X_2             7//Pad座標-X2:double
#define PID_ID_PAD_POS_Y_2             8//Pad座標-Y2:double
#define PID_ID_COMPONENT_ANGLE         9//零件角度:double

#define PID_ID_PAD_AREA               27//Pad面積:double
#define PID_ID_PAD_UNKNOW_1           30//未知:double
#define PID_ID_PAD_UNKNOW_2           31//未知:double

#define PID_ID_HASI_CAD_POS_X         34//韓華Cad座標-X
#define PID_ID_HASI_CAD_POS_Y         35//韓華Cad座標-Y
#define PID_ID_HASI_FID_POS_X         36//韓華定位點Cad座標-X
#define PID_ID_HASI_FID_POS_Y         37//韓華定位點Cad座標-Y

#define PID_ID_PAD_TYPE               64//Pad樣式:str
#define PID_ID_PAD_SN                 66//Pad序號:int

#define PID_ID_UNIT                  101//單位:1-mm, 2-inch
#define PID_ID_PAD_UNKNOW_3          102//未知:double

#define PID_ID_PAD_DRAW_TYPE         601//Pad繪圖樣式:int

#define PID_ID_POLYLINE_START        602//多邊形開始
#define PID_ID_POLYLINE_END          603//多邊形結束

#define PID_ID_POINT_INFO_START      610//點資訊開始
#define PID_ID_POINT_INFO_END        611//點資訊結束
#define PID_ID_POINT_POS_X           612//點資訊-座標X:double
#define PID_ID_POINT_POS_Y           613//點資訊-座標Y:double
#define PID_ID_POINT_ARC_X           614//點資訊-圓弧X:double
#define PID_ID_POINT_ARC_Y           615//點資訊-圓弧Y:double
#define PID_ID_POINT_PATH_TYPE       616//點資訊-路徑樣式:int

#define PID_ID_PAD_END               999//Pad結束
#define PID_ID_FILE_END              998//檔案結束
//-------------------------------------------------------------------------------------//

#endif//_AOI_FILE_PID_DEF_H_