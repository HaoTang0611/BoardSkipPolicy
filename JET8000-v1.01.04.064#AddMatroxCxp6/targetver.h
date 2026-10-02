// 這個 MFC 範例原始程式碼會示範如何使用 MFC Microsoft Office Fluent 使用者介面
// ("Fluent UI") 並且僅提供為參考資料，做為
// MFC 參考及 MFC C++ 程式庫軟體
// 隨附相關電子文件的補充。
// 關於 Fluent UI 之複製、使用或散發的授權條款則分別提供。
// 如需 Fluent UI 授權計劃的詳細資訊，請造訪
// http://msdn.microsoft.com/officeui。
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#pragma once

// 加上 SDKDDKVer.h 可定義最高可用的 Windows 平台。

// 如果要針對先前的 Windows 平台建置應用程式，請加上 WinSDKVer.h，
// 並在加上 SDKDDKVer.h 之前將 _WIN32_WINNT 巨集設為要支援的平台。
//----------------------------------------------------------------------------//
#define VC_6               1200//VC6.0
#define VC_2002_NET        1300//VC7.0
#define VS_2003_NET        1310//VC7.1
#define VS_2005_NET        1400//VC8.0
#define VS_2008_NET		   1500//VC9.0
#define VS_2010_NET		   1600//VC10.0
#define VS_2015_NET		   1900//VC14.0
//----------------------------------------------------------------------------//
#define FRAME_STYLE_MFC          1//MFC UI
#define FRAME_STYLE_STUDIO       2//Studio UI  
#define FRAME_STYLE_OFFICE       3//Office UI 
//----------------------------------------------------------------------------//
#if _MSC_VER >= VS_2010_NET
	#include <SDKDDKVer.h>
#endif//_MSC_VER >= VS_2010_NET
//----------------------------------------------------------------------------//

