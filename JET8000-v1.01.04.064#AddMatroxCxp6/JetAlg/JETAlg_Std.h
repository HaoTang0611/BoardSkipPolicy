#pragma once
#ifdef _JETALG_USE_H_CPP
	#define JETALG_API
#else
	#ifdef JETALG_EXPORTS
		#ifndef JETALG_API
			#define JETALG_API _declspec(dllexport)
		#endif // !JETALG_API
	#else
		#ifndef JETALG_API
			#define JETALG_API _declspec(dllimport)
		#endif // !JETALG_API
	#endif // JETALG_EXPORTS
#endif // _JETALG_USE_DLL

//修改版本請從這裡改即可! by Joe
//---------- Version ----------//
#ifndef ToString2
#define ToString2(arg) #arg
#endif // ToString2
#ifndef ToString
#define ToString(arg) ToString2(arg)
#endif // ToString
#define JETALG_VER_MAN 1
#define JETALG_VER_LIB 5
#define JETALG_VER_DLL 0
#define JETALG_VER_BUG 2
#define JETALG_VER ToString(JETALG_VER_MAN.JETALG_VER_LIB.JETALG_VER_DLL.JETALG_VER_BUG)
//---------- Version ----------//

namespace JET {
#pragma region Public Func
	JETALG_API const char* Get_DllVersion_JETAlg_MAN();
	JETALG_API const char* Get_DllVersion_JETAlg_LIB();
	JETALG_API const char* Get_DllVersion_JETAlg_DLL();
	JETALG_API const char* Get_DllVersion_JETAlg_BUG();
	JETALG_API const char* Get_DllVersion_JETAlg();
#pragma endregion
}