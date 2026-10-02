#pragma once
//----------------------------------------------------------------------------//
#ifndef _VERSION_DEFINE_H_
#define _VERSION_DEFINE_H_
//----------------------------------------------------------------------------//
#if _MSC_VER == VC_6
	#define OFFLINE_VERSION   //定義是否為OFF LINE版本
#endif
	//#define OFFLINE_VERSION   //定義是否為OFF LINE版本
//----------------------------------------------------------------------------//
//JET8000-v1.01.04.063-Alan
//----------------------------------------------------------------------------//
//#define NO_3D_VERSION//沒有3D的版本
//#define LABORATORY_VERSION//實驗室版本
//#define TB_SYSTEM_ONLY_BOT//僅有下系統版本//尚未除錯, 切勿啟用
//----------------------------------------------------------------------------//
//#define ODM_BRAND_VERSION_JET         0//客戶特製版本-捷智科技
#define ODM_BRAND_VERSION_ASUS        1//客戶特製版本-華碩科技
#define ODM_BRAND_VERSION_XMOR        2//客戶特製版本-訊茂科技
//#define ODM_BRAND_VERSION             ODM_BRAND_VERSION_ASUS//客戶特製版本
//#define ODM_BRAND_VERSION             ODM_BRAND_VERSION_XMOR//客戶特製版本
//----------------------------------------------------------------------------//
#if defined(LABORATORY_VERSION) || defined(ODM_BRAND_VERSION) || defined(OFFLINE_VERSION)
	#ifdef TB_SYSTEM_ONLY_BOT
		#error TB_SYSTEM_ONLY_BOT Exception
	#endif//TB_SYSTEM_ONLY_BOT
#endif//LABORATORY_VERSION
//----------------------------------------------------------------------------//
//巨集
#define MACRO_STRING(X)        _T(#X)
#define MACRO_STRING_A(X)       #X
#define MACRO_STRING_W(X)      L#X

#define MACRO_EXPAND(X)        MACRO_STRING(X)
#define MACRO_EXPAND_A(X)      MACRO_STRING_A(X)
#define MACRO_EXPAND_W(X)      MACRO_STRING_W(X)

#define MACRO_CAT(X, Y)        X##Y
#define MACRO_GLUE(X, Y)       MACRO_CAT(X,Y)
//----------------------------------------------------------------------------//
#if ODM_BRAND_VERSION == ODM_BRAND_VERSION_ASUS
	#define AOI3D_VENDOR_        ASUS        //貼牌廠商
	#define AOI3D_APP_NAME_      AIDIS800SJ //貼牌軟體名稱		
	#define AOI3D_MAIN_FOLDER_   C:\\AIDISAOI3D//軟體主要資料夾		
#ifndef OFFLINE_VERSION	
	#define AOI3D_BMP_FILE       "res\\AIDIS800SJ_Online.BMP"
	#define AOI3D_ICON_FILE      "res\\AIDIS800SJ_Online.ico"
#else	
	#define AOI3D_BMP_FILE       "res\\AIDIS800SJ_Offline.BMP"	
	#define AOI3D_ICON_FILE      "res\\AIDIS800SJ_Offline.ico"
#endif//OFFLINE_VERSION

#elif ODM_BRAND_VERSION == ODM_BRAND_VERSION_XMOR
	#define AOI3D_VENDOR_        XMOR        //貼牌廠商
	#define AOI3D_APP_NAME_      XM8000 //貼牌軟體名稱		
	#define AOI3D_MAIN_FOLDER_   C:\\XMORAOI3D//軟體主要資料夾		
#ifndef OFFLINE_VERSION	
	#define AOI3D_BMP_FILE       "res\\MFC_Online.BMP"
	#define AOI3D_ICON_FILE      "res\\MFC_Online.ico"
#else	
	#define AOI3D_BMP_FILE       "res\\MFC_Offline.BMP"	
	#define AOI3D_ICON_FILE      "res\\MFC_Offline.ico"
#endif//OFFLINE_VERSION

#else
	#define AOI3D_VENDOR_        JET        //貼牌廠商
	#define AOI3D_APP_NAME_      JET8000//貼牌軟體名稱	
	#define AOI3D_MAIN_FOLDER_   C:\\JETAOI3D//軟體主要資料夾	
#ifndef OFFLINE_VERSION
	#define AOI3D_BMP_FILE       "res\\JET8000_Online.BMP"
	#define AOI3D_ICON_FILE      "res\\JET8000_Online.ico"
#else
	#define AOI3D_BMP_FILE       "res\\JET8000_Offline.BMP"
	#define AOI3D_ICON_FILE      "res\\JET8000_Offline.ico"
#endif//OFFLINE_VERSION
//----------------------------------------------------------------------------//
#endif//ODM_BRAND_VERSION
#define AOI3D_COPYRIGHTS_A      "Copyright (C) 2018. All rights reserved."//版權聲明

#define AOI3D_APP_VERSION        1,1,4,63//軟體版本
#define AOI3D_APP_VERSION_T      MACRO_EXPAND(AOI3D_APP_VERSION)//軟體版本 
#define AOI3D_APP_VERSION_A      MACRO_EXPAND_A(AOI3D_APP_VERSION)//軟體版本 
#define AOI3D_APP_VERSION_W      MACRO_EXPAND_W(AOI3D_APP_VERSION)//軟體版本 

#define AOI3D_FILE_VERSION       1,1,4,63//檔案版本
#define AOI3D_FILE_VERSION_T     MACRO_EXPAND(AOI3D_FILE_VERSION)//檔案版本 
#define AOI3D_FILE_VERSION_A     MACRO_EXPAND_A(AOI3D_FILE_VERSION)//檔案版本 
#define AOI3D_FILE_VERSION_W     MACRO_EXPAND_W(AOI3D_FILE_VERSION)//檔案版本         

#define AOI3D_VENDOR             MACRO_EXPAND(AOI3D_VENDOR_)    //貼牌廠商
#define AOI3D_VENDOR_A           MACRO_EXPAND_A(AOI3D_VENDOR_)  //貼牌廠商
#define AOI3D_VENDOR_W           MACRO_EXPAND_W(AOI3D_VENDOR_)  //貼牌廠商

#define AOI3D_APP_NAME           MACRO_EXPAND(AOI3D_APP_NAME_)//貼牌軟體名稱
#define AOI3D_APP_NAME_A         MACRO_EXPAND_A(AOI3D_APP_NAME_)//貼牌軟體名稱
#define AOI3D_APP_NAME_W         MACRO_EXPAND_W(AOI3D_APP_NAME_)//貼牌軟體名稱

#define AOI3D_MAIN_FOLDER        MACRO_EXPAND(AOI3D_MAIN_FOLDER_)//軟體主要資料夾	
#define AOI3D_MAIN_FOLDER_A      MACRO_EXPAND_A(AOI3D_MAIN_FOLDER_)//軟體主要資料夾	
#define AOI3D_MAIN_FOLDER_W      MACRO_EXPAND_W(AOI3D_MAIN_FOLDER_)//軟體主要資料夾	

#define AOI3D_APP_CAT(name)      MACRO_CAT(MACRO_EXPAND(AOI3D_APP_NAME_), name)//貼牌軟體+(name)
#define AOI3D_APP_CAT_A(name)    MACRO_CAT(MACRO_EXPAND_A(AOI3D_APP_NAME_), name)//貼牌軟體+(name)
#define AOI3D_APP_CAT_W(name)    MACRO_CAT(MACRO_EXPAND_W(AOI3D_APP_NAME_), name)//貼牌軟體+(name)

#define AOI3D_VENDOR_CAT(name)   MACRO_CAT(MACRO_EXPAND(AOI3D_VENDOR_), name)//貼牌廠商+(name)
#define AOI3D_VENDOR_CAT_A(name) MACRO_CAT(MACRO_EXPAND_A(AOI3D_VENDOR_), name)//貼牌廠商+(name)
#define AOI3D_VENDOR_CAT_W(name) MACRO_CAT(MACRO_EXPAND_W(AOI3D_VENDOR_), name)//貼牌廠商+(name)

#define AOI3D_FOLDER(name)       MACRO_CAT(MACRO_EXPAND(AOI3D_MAIN_FOLDER_), name)//軟體主要資料夾下路徑
#define AOI3D_FOLDER_A(name)     MACRO_CAT(MACRO_EXPAND_A(AOI3D_MAIN_FOLDER_), name)//軟體主要資料夾下路徑
#define AOI3D_FOLDER_W(name)     MACRO_CAT(MACRO_EXPAND_W(AOI3D_MAIN_FOLDER_), name)//軟體主要資料夾下路徑
//----------------------------------------------------------------------------//
#endif//_VERSION_DEFINE_H_
