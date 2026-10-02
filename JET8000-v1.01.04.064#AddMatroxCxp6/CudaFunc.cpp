//CudaFunc.cpp
#include "StdAfx.h"
#include "CudaFunc.h"
#include "Light3DDef.h"
#include <algorithm>


#ifdef CUDA_USE		//chia 1050127

#if _MSC_VER >= VS_2015_NET
	#ifdef _X64
		//#pragma comment(lib, "cudart.lib")
		#pragma comment(lib, "..\\JET8000_Library\\CUDA_9_1\\lib\\x64\\cudart.lib")
	#else
		#pragma comment(lib, "..\\JET8000_Library\\CUDA_9_1\\lib\\x32\\cudart.lib")
	#endif//_X64
#elif _MSC_VER >= VS_2008_NET
	#ifdef _X64	
		#pragma comment(lib, "..\\JET8000_Library\\CUDA\\lib\\x64\\cudart.lib")
	#else		
		#pragma comment(lib, "..\\JET8000_Library\\CUDA\\lib\\x32\\cudart.lib")
	#endif//_X64
#else
#endif//_MSC_VER

	// This will output the proper CUDA error strings in the event that a CUDA host call returns an error
	#define checkCudaErrors(err)  __checkCudaErrors (err, __FILE__, __LINE__)
	// This will output the proper error string when calling cudaGetLastError
	//#define getLastCudaError(msg)      __getLastCudaError (msg, __FILE__, __LINE__)


	inline bool __checkCudaErrors(cudaError err, const char *file, const int line )
	{
		if(cudaSuccess != err)
		{
	//		fprintf(stderr, "%s(%i) : CUDA Runtime API error %d: %s.\n",file, line, (int)err, cudaGetErrorString( err ) );
			return false;
		}
		return true;
	}
	 /*
	inline bool __getLastCudaError(const char *errorMessage, const char *file, const int line )
	{
		cudaError_t err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			fprintf(stderr, "%s(%i) : getLastCudaError() CUDA error : %s : (%d) %s.\n",file, line, errorMessage, (int)err, cudaGetErrorString( err ) );
			return false;
		}
	}
	*/
//------------------------------------------------------------------------------//
/*inline void __checkCudaErrors(cudaError err, const char *file, const int line )
{
	if(cudaSuccess != err)
	{
		fprintf(stderr, "%s(%i) : CUDA Runtime API error %d: %s.\n",file, line, (int)err, cudaGetErrorString( err ) );
		exit(-1);        
	}

}
//------------------------------------------------------------------------------//
inline void __getLastCudaError(const char *errorMessage, const char *file, const int line )
{
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		fprintf(stderr, "%s(%i) : getLastCudaError() CUDA error : %s : (%d) %s.\n",
			file, line, errorMessage, (int)err, cudaGetErrorString( err ) );
		exit(-1);
	}
}*/
//////////////////////////////////////////////////////////////////////
CCudaFunc CudaFunc;
CRITICAL_SECTION CCudaFunc::m_csCuda;//同步機制-關鍵區間
CRITICAL_SECTION CCudaFunc::m_csCudaFunc;//同步機制-關鍵區間函式呼叫
//////////////////////////////////////////////////////////////////////
void CCudaFunc::InitCudaLock()//初始化Cuda的關鍵區間
{
	InitializeCriticalSection(&m_csCuda);
}
//-------------------------------------------------------------------------//
void CCudaFunc::DeleteCudaLock() //刪除Cuda的關鍵區間
{
	DeleteCriticalSection(&m_csCuda);
}
//-------------------------------------------------------------------------//
void CCudaFunc::LockCuda()        //進入Cuda的關鍵區間
{
	EnterCriticalSection(&m_csCuda);
}
//-------------------------------------------------------------------------//
void CCudaFunc::UnlockCuda()      //離開Cuda的關鍵區間
{
	LeaveCriticalSection(&m_csCuda);
}
//-------------------------------------------------------------------------//
void CCudaFunc::InitCudaFuncLock()//初始化Cuda函式呼叫的關鍵區間
{
	InitializeCriticalSection(&m_csCudaFunc);
}
//-------------------------------------------------------------------------//
void CCudaFunc::DeleteCudaFuncLock()//刪除Cuda函式呼叫的關鍵區間
{
	DeleteCriticalSection(&m_csCudaFunc);
}
//-------------------------------------------------------------------------//
void CCudaFunc::LockCudaFunc()//進入Cuda函式呼叫的關鍵區間
{
	EnterCriticalSection(&m_csCudaFunc);
}
//-------------------------------------------------------------------------//
void CCudaFunc::UnlockCudaFunc()//離開Cuda函式呼叫的關鍵區間
{
	LeaveCriticalSection(&m_csCudaFunc);
}
//-------------------------------------------------------------------------//
CCudaFunc::CCudaFunc()
{
	InitCudaLock();
	InitCudaFuncLock();
	PreInitial();
	Initial();
}
//-------------------------------------------------------------------------//
CCudaFunc::~CCudaFunc()
{
	ReleaseAll();
	DeleteCudaFuncLock();
	DeleteCudaLock();
}
//-------------------------------------------------------------------------//
bool CCudaFunc::InitCUDA()
{
	if ( InitCUDAFn() == false )
	{
		SetCudaExceptionCode(AOI_EXCEPTION_CUDA_INIT);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::InitCUDAFn()
{
#ifdef  CUDA_USE
	int  count = 0;
	bool bSuccess = false;	
	
	SetCudaDeviceIndex(-1);
	SetIsCudaDeviceUse(false);
	bSuccess = checkCudaErrors(cudaGetDeviceCount(&count));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaGetDeviceCount Error. %s", cudaGetErrorString(err));
		return false;	
	}
    
    if ( 0 == count ) 
	{
        ::sprintf(m_ErrorString, "There is no device.\n");
	    return false;
    }

    int i=0;
	cudaDeviceProp prop;
    for(i = 0; i < count; i++) 
	{
		prop = cudaDeviceProp();
        if ( cudaGetDeviceProperties(&prop, i) == cudaSuccess) 
		{
            if ( prop.major >= 1 ) 
			{	
				SetCudaDeviceIndex(i);
				break;	
			}
        }
    }

    if ( -1 == m_CudaDeviceIndex)
	{
        ::sprintf(m_ErrorString, "There is no device supporting CUDA 1.x.\n");
	    return false;
    }

	if (checkCudaErrors(cudaSetDevice(m_CudaDeviceIndex)) == false)
	{
		::sprintf(m_ErrorString, "cudaSetDevice Fault. %s", cudaGetErrorString(cudaGetLastError()));	
		return false;
	}	
	
	if ( checkCudaErrors(cudaGetDeviceProperties(&m_CudaDevProp, m_CudaDeviceIndex)) == false )
	{
		::sprintf(m_ErrorString, "cudaGetDeviceProperties Fault. %s", cudaGetErrorString(cudaGetLastError()));	
		return false;
	}
	ExecCudaBuildAtanTable();
	return true;
#endif //#ifndef CUDA_USE
	return false;
}
//-------------------------------------------------------------------------//
void CCudaFunc::PreInitial()
{
	SetCudaDeviceIndex(-1);
	SetIsCudaDeviceUse(false);
	::memset(m_ErrorString, 0x00, sizeof(m_ErrorString));

	//解相位
//THREAD_00
	m_PhaseBufferSizeD = 0;
	m_ImgD1 = m_ImgD2 = m_ImgD3 = m_ImgD4 = m_ImgD5 = NULL;
	m_pPhaseD = NULL; m_pMaskD = NULL; m_pMergeD = NULL;
//THREAD_01
	m_PhaseBufferSizeD_01 = 0;
	m_ImgD1_01 = m_ImgD2_01 = m_ImgD3_01 = m_ImgD4_01 = m_ImgD5_01 = NULL;
	m_pPhaseD_01 = NULL; m_pMaskD_01 = NULL; m_pMergeD_01 = NULL;
	m_pPhaseD_F2_01 = NULL; m_pMaskD_F2_01 = NULL; m_pMergeD_F2_01 = NULL;

//THREAD_02
	m_PhaseBufferSizeD_02 = 0;
	m_ImgD1_02 = m_ImgD2_02 = m_ImgD3_02 = m_ImgD4_02 = m_ImgD5_02 = NULL;
	m_pPhaseD_02 = NULL; m_pMaskD_02 = NULL; m_pMergeD_02 = NULL;
	m_pPhaseD_F2_02 = NULL; m_pMaskD_F2_02 = NULL; m_pMergeD_F2_02 = NULL;

//THREAD_03
	m_PhaseBufferSizeD_03 = 0;
	m_ImgD1_03 = m_ImgD2_03 = m_ImgD3_03 = m_ImgD4_03 = m_ImgD5_03 = NULL;
	m_pPhaseD_03 = NULL; m_pMaskD_03 = NULL; m_pMergeD_03 = NULL;
	m_pPhaseD_F2_03 = NULL; m_pMaskD_F2_03 = NULL; m_pMergeD_F2_03 = NULL;

//THREAD_04
	m_PhaseBufferSizeD_04 = 0;
	m_ImgD1_04 = m_ImgD2_04 = m_ImgD3_04 = m_ImgD4_04 = m_ImgD5_04 = NULL;
	m_pPhaseD_04 = NULL; m_pMaskD_04 = NULL; m_pMergeD_04 = NULL;
	m_pPhaseD_F2_04 = NULL; m_pMaskD_F2_04 = NULL; m_pMergeD_F2_04 = NULL;

	m_BasePhaseBufferSizeD_A = 0;
	m_BasePhaseD_A = NULL;	
	m_FFCBaseBufferSizeD_A = 0;
	m_FFCGainD_A = NULL;
	m_FFCBaseD_A = NULL;

	m_BasePhaseBufferSizeD_B = 0;	
	m_BasePhaseD_B = NULL;
	m_FFCBaseBufferSizeD_B = 0;
	m_FFCGainD_B = NULL;
	m_FFCBaseD_B = NULL;

	m_BasePhaseBufferSizeD_C = 0;	
	m_BasePhaseD_C = NULL;
	m_FFCBaseBufferSizeD_C = 0;
	m_FFCGainD_C = NULL;
	m_FFCBaseD_C = NULL;

	m_BasePhaseBufferSizeD_D = 0;	
	m_BasePhaseD_D = NULL;
	m_FFCBaseBufferSizeD_D = 0;
	m_FFCGainD_D = NULL;
	m_FFCBaseD_D = NULL;

	//PhaseSmooth
//THREAD_00
	m_PhaseSmoothDestD = NULL;
	m_PhaseSmoothTempSinBufferD = NULL;
	m_PhaseSmoothTempCosBufferD = NULL;
	m_PhaseSmoothTempMaskBufferD = NULL;
//THREAD_01
	m_PhaseSmoothDestD_01 = NULL;
	m_PhaseSmoothTempSinBufferD_01 = NULL;
	m_PhaseSmoothTempCosBufferD_01 = NULL;
	m_PhaseSmoothTempMaskBufferD_01 = NULL;
//THREAD_02
	m_PhaseSmoothDestD_02 = NULL;
	m_PhaseSmoothTempSinBufferD_02 = NULL;
	m_PhaseSmoothTempCosBufferD_02 = NULL;
	m_PhaseSmoothTempMaskBufferD_02 = NULL;
//THREAD_03
	m_PhaseSmoothDestD_03 = NULL;
	m_PhaseSmoothTempSinBufferD_03 = NULL;
	m_PhaseSmoothTempCosBufferD_03 = NULL;
	m_PhaseSmoothTempMaskBufferD_03 = NULL;
//THREAD_04
	m_PhaseSmoothDestD_04 = NULL;
	m_PhaseSmoothTempSinBufferD_04 = NULL;
	m_PhaseSmoothTempCosBufferD_04 = NULL;
	m_PhaseSmoothTempMaskBufferD_04 = NULL;
	
	//AverageSmooth
	m_AverageSmoothSrcD = NULL;
	m_AverageSmoothDestD = NULL;
	m_AverageSmoothBufferD = NULL;
	this->m_MaxAverageSmoothSize = 0;
	//2D Fileter Data Gaussian Smooth
	m_GaussianSmoothSrcD = NULL;
	m_GaussianSmoothDestD = NULL;
	m_GaussianSmoothBufferD = NULL;
	m_MaxGaussianSmoothSize = 0;
}
//-------------------------------------------------------------------------//
void CCudaFunc::Initial()
{
    if(!InitCUDA())
	{
	#ifdef _DEBUG
		JetAPI::ShowMessageBox(m_ErrorString);
	#endif//_DEBUG
		return ;
    }
	SetIsCudaDeviceUse(true);	
}
//-------------------------------------------------------------------------//
void CCudaFunc::ReleaseAll()
{
	int FrequenceMode = FREQUENCE_MODE_P1;
	ReleaseSolvePhaseDataBuffer();
	ReleaseBasePhasePlaneBuffer(FrequenceMode);
	ReleaseFFCBaseBuffer();
	ReleasePhaseSmoothDataBuffer();

	ReleaseAverageSmoothDataBuffer();
	ReleaseGaussianSmoothDataBuffer();

	CString filename;
	CString folder = AOIDataCollect.GetAOILogDirectory();
	filename.Format(_T("%s\\%s"), folder, _T("CudaMemory.txt"));
	SaveCudaBufferList(filename);
	ReleaseCudaBufferList(true);

#ifdef USE_MULTI_FREQUENCY
	FrequenceMode = FREQUENCE_MODE_P2;
	ReleaseBasePhasePlaneBuffer(FrequenceMode);
#endif

#ifdef  CUDA_USE
	UnLinkTextureSolvePhase();	
	cudaThreadExit();
	//cudaDeviceReset();
#endif //#ifndef CUDA_USE

	SetCudaDeviceIndex(-1);
	SetIsCudaDeviceUse(false);	
}
//-------------------------------------------------------------------------//
void CCudaFunc::SetCudaExceptionCode_Param(LPCTSTR Err)
{
	SetCudaExceptionCode(AOI_EXCEPTION_CUDA_PARAM, Err);	
}
//-------------------------------------------------------------------------//
void CCudaFunc::SetCudaExceptionCode_FileRead(LPCTSTR Err)
{
	CString str = (NULL!=Err) ? Err:CString(m_ErrorString);
	AOIExceptionCodeCtrl.SetAOIExceptionCode_FileRead(str);
	//AOIExceptionCodeCtrl.SetAOIExceptionCode_Cuda(AOI_EXCEPTION_CUDA_FILE_READ, str);
	return;
}
//-------------------------------------------------------------------------//
void CCudaFunc::SetCudaExceptionCode_FileWrite(LPCTSTR Err)
{
	CString str = (NULL!=Err) ? Err:CString(m_ErrorString);
	AOIExceptionCodeCtrl.SetAOIExceptionCode_FileWrite(str);
	//AOIExceptionCodeCtrl.SetAOIExceptionCode_Cuda(AOI_EXCEPTION_CUDA_FILE_WRITE, str);	
	return;
}
//-------------------------------------------------------------------------//
void CCudaFunc::SetCudaExceptionCode_ExeFunc(LPCTSTR Err)
{
	SetCudaExceptionCode(AOI_EXCEPTION_CUDA_EXEC_FUNC, Err);
}
//-------------------------------------------------------------------------//
void CCudaFunc::SetCudaExceptionCode_MemCopy(LPCTSTR Err)
{
	SetCudaExceptionCode(AOI_EXCEPTION_CUDA_MEM_COPY, Err);
}
//-------------------------------------------------------------------------//
void CCudaFunc::SetCudaExceptionCode(DWORD Code, LPCTSTR Err)
{
	CString str = (NULL!=Err) ? Err:CString(m_ErrorString);
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Cuda(Code, str);
}
//-------------------------------------------------------------------------//
inline void CCudaFunc::SetCudaDeviceIndex(int val)
{
	m_CudaDeviceIndex = val;
}
//-------------------------------------------------------------------------//
inline void CCudaFunc::SetIsCudaDeviceUse(bool val)
{
	m_IsCudaDeviceUse = val;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::GetIsCudaDeviceUse() const
{	
	//return true;
	//return false;
	return m_IsCudaDeviceUse;
}
//-------------------------------------------------------------------------//
void CCudaFunc::CudaExceptionClean()
{
#ifdef  CUDA_USE
	ReleaseCudaBufferList(false);

	cudaThreadExit();
	//cudaDeviceReset();

//此時Buffer 指標已經無效  將Buffer指標 清空
	//SlovePhase
//THREAD_00
	m_PhaseBufferSizeD = 0;
	m_ImgD1 = m_ImgD2 = m_ImgD3 = m_ImgD4 = NULL;
	m_pPhaseD = NULL; m_pMaskD = NULL; m_pMergeD = NULL;
//THREAD_01
	m_PhaseBufferSizeD_01 = 0;
	m_ImgD1_01 = m_ImgD2_01 = m_ImgD3_01 = m_ImgD4_01 = NULL;
	m_pPhaseD_01 = NULL; m_pMaskD_01 = NULL; m_pMergeD_01 = NULL;
	m_pPhaseD_F2_01 = NULL; m_pMaskD_F2_01 = NULL; m_pMergeD_F2_01 = NULL;
//THREAD_02
	m_PhaseBufferSizeD_02 = 0;
	m_ImgD1_02 = m_ImgD2_02 = m_ImgD3_02 = m_ImgD4_02 = NULL;
	m_pPhaseD_02 = NULL; m_pMaskD_02 = NULL; m_pMergeD_02 = NULL;
	m_pPhaseD_F2_02 = NULL; m_pMaskD_F2_02 = NULL; m_pMergeD_F2_02 = NULL;
//THREAD_03
	m_PhaseBufferSizeD_03 = 0;
	m_ImgD1_03 = m_ImgD2_03 = m_ImgD3_03 = m_ImgD4_03 = NULL;
	m_pPhaseD_03 = NULL; m_pMaskD_03 = NULL; m_pMergeD_03 = NULL;
	m_pPhaseD_F2_03 = NULL; m_pMaskD_F2_03 = NULL; m_pMergeD_F2_03 = NULL;
//THREAD_04
	m_PhaseBufferSizeD_04 = 0;
	m_ImgD1_04 = m_ImgD2_04 = m_ImgD3_04 = m_ImgD4_04 = NULL;
	m_pPhaseD_04 = NULL; m_pMaskD_04 = NULL; m_pMergeD_04 = NULL;
	m_pPhaseD_F2_04 = NULL; m_pMaskD_F2_04 = NULL; m_pMergeD_F2_04 = NULL;

	//PhaseSmooth
//THREAD_00
	m_PhaseSmoothBufferSizeD = 0;
	m_PhaseSmoothDestD = NULL;
	m_PhaseSmoothTempSinBufferD = NULL;
	m_PhaseSmoothTempCosBufferD = NULL;
	m_PhaseSmoothTempMaskBufferD = NULL;
//THREAD_01
	m_PhaseSmoothBufferSizeD_01 = 0;
	m_PhaseSmoothDestD_01 = NULL;
	m_PhaseSmoothTempSinBufferD_01 = NULL;
	m_PhaseSmoothTempCosBufferD_01 = NULL;
	m_PhaseSmoothTempMaskBufferD_01 = NULL;
//THREAD_02
	m_PhaseSmoothBufferSizeD_02 = 0;
	m_PhaseSmoothDestD_02 = NULL;
	m_PhaseSmoothTempSinBufferD_02 = NULL;
	m_PhaseSmoothTempCosBufferD_02 = NULL;
	m_PhaseSmoothTempMaskBufferD_02 = NULL;
//THREAD_03
	m_PhaseSmoothBufferSizeD_03 = 0;
	m_PhaseSmoothDestD_03 = NULL;
	m_PhaseSmoothTempSinBufferD_03 = NULL;
	m_PhaseSmoothTempCosBufferD_03 = NULL;
	m_PhaseSmoothTempMaskBufferD_03 = NULL;
//THREAD_04
	m_PhaseSmoothBufferSizeD_04 = 0;
	m_PhaseSmoothDestD_04 = NULL;
	m_PhaseSmoothTempSinBufferD_04 = NULL;
	m_PhaseSmoothTempCosBufferD_04 = NULL;
	m_PhaseSmoothTempMaskBufferD_04 = NULL;

//重新連線裝置 	
   if(!InitCUDA())
	{
	#ifdef _DEBUG
	   JetAPI::ShowMessageBox(m_ErrorString);
	#endif//_DEBUG	   
		return;
    }
   SetIsCudaDeviceUse(true);

#endif//CUDA_USE
}
//-------------------------------------------------------------------------//
bool CCudaFunc::ReturnCudaDisabled()
{
	::sprintf(this->m_ErrorString, "Error, No Define Cuda Use");
	SetCudaExceptionCode(AOI_EXCEPTION_CUDA_DISABLED);
	return false;
}
//-------------------------------------------------------------------------//
const char*CCudaFunc::GetErrorString()
{
	CString Err(m_ErrorString);
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Cuda_Others(Err);
	return m_ErrorString;
}
//-------------------------------------------------------------------------//
size_t CCudaFunc::CalcBufferSize(IMAGE_SIZE Step, IMAGE_SIZE H) const//計算記憶體大小
{
	return (size_t)(Step)*(size_t)(H);
}
//-------------------------------------------------------------------------//
inline bool CCudaFunc::CheckPtr(const char fnName[], const void *Ptr)
{
	if ( NULL == Ptr )
	{
		::sprintf(m_ErrorString, "Error, Ptr is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
inline bool CCudaFunc::CheckPtr2(const char fnName[], const void *Ptr1, const void *Ptr2)
{
	if ( NULL==Ptr1 || NULL==Ptr2 )
	{
		::sprintf(m_ErrorString, "Error, Ptr(2) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
inline bool CCudaFunc::CheckPtr3(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 )
	{
		::sprintf(m_ErrorString, "Error, Ptr(3) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
inline bool CCudaFunc::CheckPtr4(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 )
	{
		::sprintf(m_ErrorString, "Error, Ptr(4) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
inline bool CCudaFunc::CheckPtr5(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 )
	{
		::sprintf(m_ErrorString, "Error, Ptr(5) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
inline bool CCudaFunc::CheckPtr6(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 )
	{
		::sprintf(m_ErrorString, "Error, Ptr(6) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckPtr7(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 || NULL==Ptr7 )
	{
		::sprintf(m_ErrorString, "Error, Ptr(7) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckPtr8(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 || NULL==Ptr7 || NULL==Ptr8 )
	{
		::sprintf(m_ErrorString, "Error, Ptr(8) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckPtr9(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 || NULL==Ptr7 || NULL==Ptr8 || NULL==Ptr9 )
	{
		::sprintf(m_ErrorString, "Error, Ptr(9) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckPtr10(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 || NULL==Ptr7 || NULL==Ptr8 || NULL==Ptr9 || NULL==Ptr10 )
	{
		::sprintf(m_ErrorString, "Error, Ptr(10) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckPtr11(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 || NULL==Ptr7 || NULL==Ptr8 || NULL==Ptr9 || NULL==Ptr10 || NULL==Ptr11 )
	{
		::sprintf(m_ErrorString, "Error, Ptr(11) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckPtr12(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 || NULL==Ptr7 || NULL==Ptr8 || NULL==Ptr9 || NULL==Ptr10 || NULL==Ptr11 || NULL==Ptr12)
	{
		::sprintf(m_ErrorString, "Error, Ptr(12) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckPtr13(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 || NULL==Ptr7 || NULL==Ptr8 || NULL==Ptr9 || NULL==Ptr10 || NULL==Ptr11 || NULL==Ptr12 || NULL==Ptr13)
	{
		::sprintf(m_ErrorString, "Error, Ptr(13) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckPtr14(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 || NULL==Ptr7 || NULL==Ptr8 || NULL==Ptr9 || NULL==Ptr10 || NULL==Ptr11 || NULL==Ptr12 || NULL==Ptr13 || NULL==Ptr14)
	{
		::sprintf(m_ErrorString, "Error, Ptr(14) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckPtr15(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 || NULL==Ptr7 || NULL==Ptr8 || NULL==Ptr9 || NULL==Ptr10 || NULL==Ptr11 || NULL==Ptr12 || NULL==Ptr13 || NULL==Ptr14 || NULL==Ptr15)
	{
		::sprintf(m_ErrorString, "Error, Ptr(15) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckPtr16(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 || NULL==Ptr7 || NULL==Ptr8 || NULL==Ptr9 || NULL==Ptr10 || NULL==Ptr11 || NULL==Ptr12 || NULL==Ptr13 || NULL==Ptr14 || NULL==Ptr15 || NULL==Ptr16)
	{
		::sprintf(m_ErrorString, "Error, Ptr(16) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckPtr17(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 || NULL==Ptr7 || NULL==Ptr8 || NULL==Ptr9 || NULL==Ptr10 || NULL==Ptr11 || NULL==Ptr12 || NULL==Ptr13 || NULL==Ptr14 || NULL==Ptr15 || NULL==Ptr16 || NULL==Ptr17)
	{
		::sprintf(m_ErrorString, "Error, Ptr(17) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckPtr18(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 || NULL==Ptr7 || NULL==Ptr8 || NULL==Ptr9 || NULL==Ptr10 || NULL==Ptr11 || NULL==Ptr12 || NULL==Ptr13 || NULL==Ptr14 || NULL==Ptr15 || NULL==Ptr16 || NULL==Ptr17 || NULL==Ptr18)
	{
		::sprintf(m_ErrorString, "Error, Ptr(18) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckPtr19(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18, const void *Ptr19)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 || NULL==Ptr7 || NULL==Ptr8 || NULL==Ptr9 || NULL==Ptr10 || NULL==Ptr11 || NULL==Ptr12 || NULL==Ptr13 || NULL==Ptr14 || NULL==Ptr15 || NULL==Ptr16 || NULL==Ptr17 || NULL==Ptr18 || NULL==Ptr19)
	{
		::sprintf(m_ErrorString, "Error, Ptr(19) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckPtr20(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18, const void *Ptr19, const void *Ptr20)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 || NULL==Ptr7 || NULL==Ptr8 || NULL==Ptr9 || NULL==Ptr10 || NULL==Ptr11 || NULL==Ptr12 || NULL==Ptr13 || NULL==Ptr14 || NULL==Ptr15 || NULL==Ptr16 || NULL==Ptr17 || NULL==Ptr18 || NULL==Ptr19 || NULL==Ptr20)
	{
		::sprintf(m_ErrorString, "Error, Ptr(20) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckPtr21(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18, const void *Ptr19, const void *Ptr20, const void *Ptr21)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 || NULL==Ptr7 || NULL==Ptr8 || NULL==Ptr9 || NULL==Ptr10 || NULL==Ptr11 || NULL==Ptr12 || NULL==Ptr13 || NULL==Ptr14 || NULL==Ptr15 || NULL==Ptr16 || NULL==Ptr17 || NULL==Ptr18 || NULL==Ptr19 || NULL==Ptr20 || NULL==Ptr21)
	{
		::sprintf(m_ErrorString, "Error, Ptr(21) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckPtr22(const char fnName[], const void *Ptr1, const void *Ptr2, const void *Ptr3, const void *Ptr4, const void *Ptr5, const void *Ptr6, const void *Ptr7, const void *Ptr8, const void *Ptr9, const void *Ptr10, const void *Ptr11, const void *Ptr12, const void *Ptr13, const void *Ptr14, const void *Ptr15, const void *Ptr16, const void *Ptr17, const void *Ptr18, const void *Ptr19, const void *Ptr20, const void *Ptr21, const void *Ptr22)
{
	if ( NULL==Ptr1 || NULL==Ptr2 || NULL==Ptr3 || NULL==Ptr4 || NULL==Ptr5 || NULL==Ptr6 || NULL==Ptr7 || NULL==Ptr8 || NULL==Ptr9 || NULL==Ptr10 || NULL==Ptr11 || NULL==Ptr12 || NULL==Ptr13 || NULL==Ptr14 || NULL==Ptr15 || NULL==Ptr16 || NULL==Ptr17 || NULL==Ptr18 || NULL==Ptr19 || NULL==Ptr20 || NULL==Ptr21 || NULL==Ptr22)
	{
		::sprintf(m_ErrorString, "Error, Ptr(22) is NULL (%s)", fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckCastParam2Exp(const char fnName[], const TCastParam &CastParam)
{
	if ( 2 != CastParam.ExpCount )
	{
		::sprintf(m_ErrorString, "Error, Cast Image is Not Exposure2 Param(%d) (%s)", CastParam.ExpCount, fnName);
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckCastParam(const char fnName[], unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const TCastParam &CastParam)
{
	if ( CheckCastParamFn(fnName, ImageW, ImageH, ImageStep, CastParam) == false )
	{
		SetCudaExceptionCode_Param();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CheckCastParamFn(const char fnName[], unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const TCastParam &CastParam)	
{
	if ( CastParam.ImageW!=ImageW || CastParam.ImageH!=ImageH || CastParam.ImageStep!=ImageStep )
	{
		::sprintf(m_ErrorString, "Error, Cast Image Size Exception(%d, %d, %d) (%s)", CastParam.ImageW, CastParam.ImageH, CastParam.ImageStep, fnName);				
		return false;
	}
	DECODE_PHASE_MODE DecodeMode=CastParam.DecodeMode;	
	switch ( DecodeMode )
	{
	case DECODE_PHASE_3STEP_1:
		if ( 1 == CastParam.ExpCount )
		{
			if ( 3 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr5(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }	
		}
		if ( 2 == CastParam.ExpCount )
		{
			if ( 6 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr8(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrC1, CastParam.PtrC2, CastParam.PtrC3, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }	
		}
		break;
	case DECODE_PHASE_4STEP_1:
		if ( 1 == CastParam.ExpCount )
		{
			if ( 4 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr6(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrA4, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }	
		}
		if ( 2 == CastParam.ExpCount )
		{
			if ( 8 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr10(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrA4, CastParam.PtrC1, CastParam.PtrC2, CastParam.PtrC3, CastParam.PtrC4, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }	
		}
		break;
	case DECODE_PHASE_5STEP_1:
		if ( 1 == CastParam.ExpCount )
		{
			if ( 5 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr7(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrA4, CastParam.PtrA5, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }	
		}
		if ( 2 == CastParam.ExpCount )
		{
			if ( 10 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr12(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrA4, CastParam.PtrA5, CastParam.PtrC1, CastParam.PtrC2, CastParam.PtrC3, CastParam.PtrC4, CastParam.PtrC5, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }	
		}
		break;
	case DECODE_PHASE_3_3STEP_2:		
		if ( 1 == CastParam.ExpCount )
		{
			if ( 6 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr8(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrB1, CastParam.PtrB2, CastParam.PtrB3, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }
		}
		if ( 2 == CastParam.ExpCount )
		{
			if ( 12 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr14(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrB1, CastParam.PtrB2, CastParam.PtrB3, CastParam.PtrC1, CastParam.PtrC2, CastParam.PtrC3, CastParam.PtrD1, CastParam.PtrD2, CastParam.PtrD3, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }	
		}
		break;
	case DECODE_PHASE_4_4STEP_2:
		if ( 1 == CastParam.ExpCount )
		{
			if ( 8 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr10(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrA4, CastParam.PtrB1, CastParam.PtrB2, CastParam.PtrB3, CastParam.PtrB4, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }	
		}		
		if ( 2 == CastParam.ExpCount )
		{
			if ( 16 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr18(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrA4, CastParam.PtrB1, CastParam.PtrB2, CastParam.PtrB3, CastParam.PtrB4, CastParam.PtrC1, CastParam.PtrC2, CastParam.PtrC3, CastParam.PtrC4, CastParam.PtrD1, CastParam.PtrD2, CastParam.PtrD3, CastParam.PtrD4, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }				
		}
		break;
	case DECODE_PHASE_5_5STEP_2:
		if ( 1 == CastParam.ExpCount )
		{
			if ( 10 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr12(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrA4, CastParam.PtrA5, CastParam.PtrB1, CastParam.PtrB2, CastParam.PtrB3, CastParam.PtrB4, CastParam.PtrB5, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }	
		}
		if ( 2 == CastParam.ExpCount )
		{
			if ( 20 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr22(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrA4, CastParam.PtrA5, CastParam.PtrB1, CastParam.PtrB2, CastParam.PtrB3, CastParam.PtrB4, CastParam.PtrB5, CastParam.PtrC1, CastParam.PtrC2, CastParam.PtrC3, CastParam.PtrC4, CastParam.PtrC5, CastParam.PtrD1, CastParam.PtrD2, CastParam.PtrD3, CastParam.PtrD4, CastParam.PtrD5, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }				
		}
		break;
	case DECODE_PHASE_2_1STEP_1:
		if ( 1 == CastParam.ExpCount )
		{
			if ( 3 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr5(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }				
		}
		if ( 2 == CastParam.ExpCount )
		{
			if ( 6 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr8(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrC1, CastParam.PtrC2, CastParam.PtrC3, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }	
		}
		break;
	case DECODE_PHASE_2_2STEP_2:
		if (1 == CastParam.ExpCount)
		{
			if (5 != CastParam.ImageCount)
			{	return false;	}
			if (this->CheckPtr7(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrB1, CastParam.PtrB2, CastParam.PtrB3, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false)
			{	return false;	}
		}
		if (2 == CastParam.ExpCount)
		{
			if (10 != CastParam.ImageCount)
			{	return false;	}
			if (this->CheckPtr12(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrC1, CastParam.PtrC2, CastParam.PtrB1, CastParam.PtrB2, CastParam.PtrB3, CastParam.PtrD1, CastParam.PtrD2, CastParam.PtrD3, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false)
			{	return false;	}
		}
		break;
	case DECODE_PHASE_4_2STEP_2:
		if ( 1 == CastParam.ExpCount )
		{
			if ( 6 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr8(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrA4, CastParam.PtrB1, CastParam.PtrB2, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }	
		}
		if ( 2 == CastParam.ExpCount )
		{
			if ( 12 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr14(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrA4, CastParam.PtrB1, CastParam.PtrB2, CastParam.PtrC1, CastParam.PtrC2, CastParam.PtrC3, CastParam.PtrC4, CastParam.PtrD1, CastParam.PtrD2, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }	
		}
		break;
	case DECODE_PHASE_4STEP_4GC_2:
		if ( 1 == CastParam.ExpCount)
		{
			if ( 8 != CastParam.ImageCount)
			{	return false;	}
			if (this->CheckPtr10(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrA4, CastParam.PtrB1, CastParam.PtrB2, CastParam.PtrB3, CastParam.PtrB4, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false)
			{	return false;	}
		}
		if (2 == CastParam.ExpCount)
		{	
			if ( 16 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr18(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrA4, CastParam.PtrB1, CastParam.PtrB2, CastParam.PtrB3, CastParam.PtrB4, CastParam.PtrC1, CastParam.PtrC2, CastParam.PtrC3, CastParam.PtrC4, CastParam.PtrD1, CastParam.PtrD2, CastParam.PtrD3, CastParam.PtrD4, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }
		}
		break;
	case DECODE_PHASE_4STEP_5GC_2:
		if (1 == CastParam.ExpCount)
		{
			if ( 9 != CastParam.ImageCount)
			{	return false;	}
			if (this->CheckPtr11(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrA4, CastParam.PtrB1, CastParam.PtrB2, CastParam.PtrB3, CastParam.PtrB4, CastParam.PtrB5, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false)
			{	return false;	}
		}
		if (2 == CastParam.ExpCount)
		{	
			if ( 18 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr20(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrA4, CastParam.PtrB1, CastParam.PtrB2, CastParam.PtrB3, CastParam.PtrB4, CastParam.PtrB5, CastParam.PtrC1, CastParam.PtrC2, CastParam.PtrC3, CastParam.PtrC4, CastParam.PtrD1, CastParam.PtrD2, CastParam.PtrD3, CastParam.PtrD4, CastParam.PtrD5, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }
		}
		break;
	case DECODE_PHASE_4STEP_6GC_2:
		if (1 == CastParam.ExpCount)
		{
			if ( 10 != CastParam.ImageCount)
			{	return false;	}
			if (this->CheckPtr12(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrA4, CastParam.PtrB1, CastParam.PtrB2, CastParam.PtrB3, CastParam.PtrB4, CastParam.PtrB5, CastParam.PtrB6, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false)
			{	return false;	}
		}
		if (2 == CastParam.ExpCount)
		{	
			if ( 20 != CastParam.ImageCount ) 
			{	return false; }
			if ( this->CheckPtr22(fnName, CastParam.PtrA1, CastParam.PtrA2, CastParam.PtrA3, CastParam.PtrA4, CastParam.PtrB1, CastParam.PtrB2, CastParam.PtrB3, CastParam.PtrB4, CastParam.PtrB5, CastParam.PtrB6, CastParam.PtrC1, CastParam.PtrC2, CastParam.PtrC3, CastParam.PtrC4, CastParam.PtrD1, CastParam.PtrD2, CastParam.PtrD3, CastParam.PtrD4, CastParam.PtrD5, CastParam.PtrD6, CastParam.ZeroPhasePtr, CastParam.HeightFactorPtr) == false ) 
			{	return false; }
		}
		break;
	default:
		::sprintf(m_ErrorString, "Error, Cast Image Count Exception(%d) (%s)", CastParam.ImageCount, fnName);		
		return false;		
	}
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CloneCastParamToDevice(const char fnName[], unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const TCastParam &CastParam, TCastParam &CastParamD)
{
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam) == false )
	{	return false; }

	size_t i=0;
	bool bSuccess = true;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);

	CastParamD.PerA = CastParam.PerA;
	CastParamD.PerB = CastParam.PerB;
	CastParamD.Gamma = CastParam.Gamma;
	CastParamD.ImageW = CastParam.ImageW;
	CastParamD.ImageH = CastParam.ImageH;
	CastParamD.ImageStep = CastParam.ImageStep;
	CastParamD.ImageCount = CastParam.ImageCount;
	
	CastParamD.ExpCount = CastParam.ExpCount;
	CastParamD.ExpTimeA = CastParam.ExpTimeA;
	CastParamD.ExpTimeB = CastParam.ExpTimeB;
	CastParamD.ExpTimeC = CastParam.ExpTimeC;
	CastParamD.ExpTimeD = CastParam.ExpTimeD;	
	CastParamD.DebugIndex = CastParam.DebugIndex;
	CastParamD.DebugExpID = CastParam.DebugExpID;	
	CastParamD.DecodeMode = CastParam.DecodeMode;		
	CastParamD.HeightBuildMode = CastParam.HeightBuildMode;		

	const size_t HeightFacotorCnt0=sizeof(CastParam.HeightFactor0)/sizeof(CastParam.HeightFactor0[0]);
	const size_t HeightFacotorCnt1=sizeof(CastParam.HeightFactor1)/sizeof(CastParam.HeightFactor1[0]);
	const size_t HeightFacotorCnt2=sizeof(CastParam.HeightFactor2)/sizeof(CastParam.HeightFactor2[0]);	
	for ( i=0; i<HeightFacotorCnt0; i++ )
	{	CastParamD.HeightFactor0[i] = CastParam.HeightFactor0[i];	}
	for ( i=0; i<HeightFacotorCnt1; i++ )
	{	CastParamD.HeightFactor1[i] = CastParam.HeightFactor1[i];	}
	for ( i=0; i<HeightFacotorCnt2; i++ )
	{	CastParamD.HeightFactor2[i] = CastParam.HeightFactor2[i];	}

	if ( NULL != CastParam.PtrA1 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrA1, fnName, "CastParamD.PtrA1") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrA1, CastParam.PtrA1, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrA1 To Device.PtrA1. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrA2 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrA2, fnName, "CastParamD.PtrA2") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrA2, CastParam.PtrA2, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrA2 To Device.PtrA2. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrA3 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrA3, fnName, "CastParamD.PtrA3") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrA3, CastParam.PtrA3, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrA3 To Device.PtrA3. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrA4 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrA4, fnName, "CastParamD.PtrA4") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrA4, CastParam.PtrA4, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrA4 To Device.PtrA4. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrA5 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrA5, fnName, "CastParamD.PtrA5") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrA5, CastParam.PtrA5, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrA5 To Device.PtrA5. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	
	if ( NULL != CastParam.PtrB1 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrB1, fnName, "CastParamD.PtrB1") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrB1, CastParam.PtrB1, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrB1 To Device.PtrB1. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrB2 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrB2, fnName, "CastParamD.PtrB2") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrB2, CastParam.PtrB2, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrB2 To Device.PtrB2. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrB3 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrB3, fnName, "CastParamD.PtrB3") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrB3, CastParam.PtrB3, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrB3 To Device.PtrB3. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrB4 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrB4, fnName, "CastParamD.PtrB4") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrB4, CastParam.PtrB4, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrB4 To Device.PtrB4. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrB5 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrB5, fnName, "CastParamD.PtrB5") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrB5, CastParam.PtrB5, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrB5 To Device.PtrB5. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrB6 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrB6, fnName, "CastParamD.PtrB6") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrB6, CastParam.PtrB6, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrB6 To Device.PtrB6. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}

	//---
	if ( NULL != CastParam.PtrC1 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrC1, fnName, "CastParamD.PtrC1") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrC1, CastParam.PtrC1, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrC1 To Device.PtrC1. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrC2 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrC2, fnName, "CastParamD.PtrC2") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrC2, CastParam.PtrC2, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrC2 To Device.PtrC2. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrC3 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrC3, fnName, "CastParamD.PtrC3") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrC3, CastParam.PtrC3, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrC3 To Device.PtrC3. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrC4 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrC4, fnName, "CastParamD.PtrC4") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrC4, CastParam.PtrC4, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrC4 To Device.PtrC4. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrC5 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrC5, fnName, "CastParamD.PtrC5") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrC5, CastParam.PtrC5, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrC5 To Device.PtrC5. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	
	if ( NULL != CastParam.PtrD1 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrD1, fnName, "CastParamD.PtrD1") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrD1, CastParam.PtrD1, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrD1 To Device.PtrD1. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrD2 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrD2, fnName, "CastParamD.PtrD2") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrD2, CastParam.PtrD2, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrD2 To Device.PtrD2. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrD3 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrD3, fnName, "CastParamD.PtrD3") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrD3, CastParam.PtrD3, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrD3 To Device.PtrD3. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrD4 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrD4, fnName, "CastParamD.PtrD4") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrD4, CastParam.PtrD4, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrD4 To Device.PtrD4. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrD5 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrD5, fnName, "CastParamD.PtrD5") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrD5, CastParam.PtrD5, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrD5 To Device.PtrD5. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.PtrD6 )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrD6, fnName, "CastParamD.PtrD6") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.PtrD6, CastParam.PtrD6, sizeof(unsigned char)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.PtrD6 To Device.PtrD6. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL != CastParam.ZeroPhasePtr )
	{
		if ( allocBuffer_Cuda(BufferSize, CastParamD.ZeroPhasePtr, fnName, "CastParamD.ZeroPhasePtr") == false )			
		{	return false;	}
		if ( checkCudaErrors(cudaMemcpy(CastParamD.ZeroPhasePtr, CastParam.ZeroPhasePtr, sizeof(short)*BufferSize , cudaMemcpyHostToDevice)) == false )
		{			
			::sprintf(m_ErrorString, "cudaMemcpy CastParam.ZeroPhasePtr To Device.ZeroPhasePtr. %s", cudaGetErrorString(cudaGetLastError()));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}		
	}

	CastParamD.HeightFactorPtr = NULL;
	if ( PHASE_CONVERT_HEIGHT_SCALE == CastParam.HeightBuildMode )
	{
		if ( NULL != CastParam.HeightFactorPtr )
		{
			if ( allocBuffer_Cuda(BufferSize, CastParamD.HeightFactorPtr, fnName, "CastParamD.HeightFactorPtr") == false )			
			{	return false;	}		
			if ( checkCudaErrors(cudaMemcpy(CastParamD.HeightFactorPtr, CastParam.HeightFactorPtr, sizeof(float)*BufferSize , cudaMemcpyHostToDevice)) == false )
			{			
				::sprintf(m_ErrorString, "cudaMemcpy CastParam.HeightFactorPtr To Device.HeightFactorPtr. %s", cudaGetErrorString(cudaGetLastError()));			
				SetCudaExceptionCode_MemCopy();
				return false;		
			}
		}
	}

	if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrMask, fnName, "CastParamD.PtrMask") == false )			
	{	return false;	}
	if ( allocBuffer_Cuda(BufferSize, CastParamD.PtrSpace, fnName, "CastParamD.PtrSpace") == false )			
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CloneCastParamPtrToDevice(const char fnName[], unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const TCastParam &CastParam, TCastParam &CastParamD, PCastParam &CastParamDPtr)
{
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam) == false )
	{	return false; }

	bool bSuccess = true;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
	const size_t CastParamSize = sizeof(TCastParam);
	//注意不能直接等於, 要透過函式轉貼過去
	if ( CloneCastParamToDevice(fnName, ImageW, ImageH, ImageStep, CastParam, CastParamD) == false )
	{	return false;	}	
	
	if ( allocBuffer_Cuda(1, CastParamDPtr, fnName, "CastParamDPtr") == false )			
	{	
		//freeBuffer_Cuda(CastParamD);
		FreeCastParamBuffer(CastParamD);
		return false;	
	}	

	if ( checkCudaErrors(cudaMemcpy(CastParamDPtr, &(CastParamD), CastParamSize, cudaMemcpyHostToDevice)) == false )
	{			
		::sprintf(m_ErrorString, "cudaMemcpy CastParamD To Device.CastParamD. %s", cudaGetErrorString(cudaGetLastError()));			
		SetCudaExceptionCode_MemCopy();
		freeBuffer_Cuda(CastParamDPtr);
		FreeCastParamBuffer(CastParamD);		
		return false;		
	}

	return true;	
}
//-------------------------------------------------------------------------//
inline bool CCudaFunc::FreeCastParamBuffer(TCastParam &CastParamD)
{
	freeBuffer_Cuda(CastParamD.PtrA1);
	freeBuffer_Cuda(CastParamD.PtrA2);
	freeBuffer_Cuda(CastParamD.PtrA3);
	freeBuffer_Cuda(CastParamD.PtrA4);
	freeBuffer_Cuda(CastParamD.PtrA5);

	freeBuffer_Cuda(CastParamD.PtrB1);
	freeBuffer_Cuda(CastParamD.PtrB2);
	freeBuffer_Cuda(CastParamD.PtrB3);
	freeBuffer_Cuda(CastParamD.PtrB4);
	freeBuffer_Cuda(CastParamD.PtrB5);
	freeBuffer_Cuda(CastParamD.PtrB6);

	freeBuffer_Cuda(CastParamD.PtrC1);
	freeBuffer_Cuda(CastParamD.PtrC2);
	freeBuffer_Cuda(CastParamD.PtrC3);
	freeBuffer_Cuda(CastParamD.PtrC4);
	freeBuffer_Cuda(CastParamD.PtrC5);

	freeBuffer_Cuda(CastParamD.PtrD1);
	freeBuffer_Cuda(CastParamD.PtrD2);
	freeBuffer_Cuda(CastParamD.PtrD3);
	freeBuffer_Cuda(CastParamD.PtrD4);
	freeBuffer_Cuda(CastParamD.PtrD5);
	freeBuffer_Cuda(CastParamD.PtrD6);

	freeBuffer_Cuda(CastParamD.PtrMask);	
	freeBuffer_Cuda(CastParamD.PtrSpace);	

	freeBuffer_Cuda(CastParamD.ZeroPhasePtr);
	freeBuffer_Cuda(CastParamD.HeightFactorPtr);
	return true;
}
//-------------------------------------------------------------------------//
inline bool CCudaFunc::FreeCastParamPtrBuffer(PCastParam &CastParamDPtr)
{
	if ( NULL == CastParamDPtr ) { return true; }
	FreeCastParamBuffer(*CastParamDPtr);
	freeBuffer_Cuda(CastParamDPtr);
	return true;
}
//-------------------------------------------------------------------------//
inline bool CCudaFunc::CloneCastParamResultToHost(const TCastParam &CastParamD, TCastParam &CastParam)
{
	const size_t DeviceDataSize = CastParam.ImageStep*CastParam.ImageH;	
	if ( NULL!=CastParamD.PtrMask && NULL!=CastParam.PtrMask )
	{
		const bool bSuccess = checkCudaErrors(cudaMemcpy(CastParam.PtrMask, CastParamD.PtrMask, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy CastParam[%d] MaskPtrD To MaskPtr. %s", CastParam.CastID, cudaGetErrorString(err));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	if ( NULL!=CastParamD.PtrSpace && NULL!=CastParam.PtrSpace )
	{
		const bool bSuccess = checkCudaErrors(cudaMemcpy(CastParam.PtrSpace, CastParamD.PtrSpace, sizeof(float)*DeviceDataSize, cudaMemcpyDeviceToHost));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy CastParam[%d] SpacePtrD To SpacePtr. %s", CastParam.CastID, cudaGetErrorString(err));			
			SetCudaExceptionCode_MemCopy();
			return false;		
		}
	}
	return true;
}
//-------------------------------------------------------------------------//
void CCudaFunc::GetSolveF1PhaseDataDevicePtr(unsigned char *&pMergeD, short *&pPhaseD, unsigned char *&pMaskD, const int ThreadNO)
{
	switch(ThreadNO)
	{
	case THREAD_01:	
		pMergeD = m_pMergeD_01;
		pPhaseD = m_pPhaseD_01;
		pMaskD = m_pMaskD_01;	
		break;
	case THREAD_02:
		pMergeD = m_pMergeD_02;
		pPhaseD = m_pPhaseD_02;
		pMaskD = m_pMaskD_02;	
		break;
	case THREAD_03:
		pMergeD = m_pMergeD_03;
		pPhaseD = m_pPhaseD_03;
		pMaskD = m_pMaskD_03;	
		break;
	case THREAD_04:
		pMergeD = m_pMergeD_04;
		pPhaseD = m_pPhaseD_04;
		pMaskD = m_pMaskD_04;	
		break;
	default:
		pMergeD = m_pMergeD;
		pPhaseD = m_pPhaseD;
		pMaskD = m_pMaskD;	
		break;
	}
}
//-------------------------------------------------------------------------//
void CCudaFunc::GetSolveF2PhaseDataDevicePtr(unsigned char *&pMergeD, short *&pPhaseD, unsigned char *&pMaskD, const int ThreadNO)
{
	switch(ThreadNO)
	{
	case THREAD_01:	
		pMergeD = m_pMergeD_F2_01;
		pPhaseD = m_pPhaseD_F2_01;
		pMaskD = m_pMaskD_F2_01;	
		break;
	case THREAD_02:
		pMergeD = m_pMergeD_F2_02;
		pPhaseD = m_pPhaseD_F2_02;
		pMaskD = m_pMaskD_F2_02;	
		break;
	case THREAD_03:
		pMergeD = m_pMergeD_F2_03;
		pPhaseD = m_pPhaseD_F2_03;
		pMaskD = m_pMaskD_F2_03;	
		break;
	case THREAD_04:
		pMergeD = m_pMergeD_F2_04;
		pPhaseD = m_pPhaseD_F2_04;
		pMaskD = m_pMaskD_F2_04;	
		break;
	default:
		pMergeD = m_pMergeD_F2;
		pPhaseD = m_pPhaseD_F2;
		pMaskD = m_pMaskD_F2;	
		break;
	}
}
//-------------------------------------------------------------------------//
void CCudaFunc::GetSolvePhaseImageDevicePtr(unsigned char *&pImgD1, unsigned char *&pImgD2, unsigned char *&pImgD3, unsigned char *&pImgD4, unsigned char *&pImgD5, const int ThreadNO)
{
	switch(ThreadNO)
	{
	case THREAD_01:	
		pImgD1 = m_ImgD1_01;
		pImgD2 = m_ImgD2_01;
		pImgD3 = m_ImgD3_01;
		pImgD4 = m_ImgD4_01;
		pImgD5 = m_ImgD5_01;
		break;
	case THREAD_02:
		pImgD1 = m_ImgD1_02;
		pImgD2 = m_ImgD2_02;
		pImgD3 = m_ImgD3_02;
		pImgD4 = m_ImgD4_02;
		pImgD5 = m_ImgD5_02;
		break;
	case THREAD_03:
		pImgD1 = m_ImgD1_03;
		pImgD2 = m_ImgD2_03;
		pImgD3 = m_ImgD3_03;
		pImgD4 = m_ImgD4_03;
		pImgD5 = m_ImgD5_03;
		break;
	case THREAD_04:
		pImgD1 = m_ImgD1_04;
		pImgD2 = m_ImgD2_04;
		pImgD3 = m_ImgD3_04;
		pImgD4 = m_ImgD4_04;
		pImgD5 = m_ImgD5_04;
		break;
	default:
		pImgD1 = m_ImgD1;
		pImgD2 = m_ImgD2;
		pImgD3 = m_ImgD3;
		pImgD4 = m_ImgD4;
		pImgD5 = m_ImgD5;
		break;
	}
}
//-------------------------------------------------------------------------//
void CCudaFunc::GetSolvePhaseDataDevicePtr(unsigned char *&pImgD1, unsigned char *&pImgD2, unsigned char *&pImgD3, unsigned char *&pImgD4, unsigned char *&pImgD5, unsigned char *&pMergeD, short *&pPhaseD, unsigned char *&pMaskD, const int ThreadNO)
{
	switch(ThreadNO)
	{
	case THREAD_01:	
		pImgD1 = m_ImgD1_01;
		pImgD2 = m_ImgD2_01;
		pImgD3 = m_ImgD3_01;
		pImgD4 = m_ImgD4_01;
		pImgD5 = m_ImgD5_01;
		pMergeD = m_pMergeD_01;
		pPhaseD = m_pPhaseD_01;
		pMaskD = m_pMaskD_01;	
		break;
	case THREAD_02:
		pImgD1 = m_ImgD1_02;
		pImgD2 = m_ImgD2_02;
		pImgD3 = m_ImgD3_02;
		pImgD4 = m_ImgD4_02;
		pImgD5 = m_ImgD5_02;
		pMergeD = m_pMergeD_02;
		pPhaseD = m_pPhaseD_02;
		pMaskD = m_pMaskD_02;	
		break;
	case THREAD_03:
		pImgD1 = m_ImgD1_03;
		pImgD2 = m_ImgD2_03;
		pImgD3 = m_ImgD3_03;
		pImgD4 = m_ImgD4_03;
		pImgD5 = m_ImgD5_03;
		pMergeD = m_pMergeD_03;
		pPhaseD = m_pPhaseD_03;
		pMaskD = m_pMaskD_03;	
		break;
	case THREAD_04:
		pImgD1 = m_ImgD1_04;
		pImgD2 = m_ImgD2_04;
		pImgD3 = m_ImgD3_04;
		pImgD4 = m_ImgD4_04;
		pImgD5 = m_ImgD5_04;
		pMergeD = m_pMergeD_04;
		pPhaseD = m_pPhaseD_04;
		pMaskD = m_pMaskD_04;	
		break;
	default:
		pImgD1 = m_ImgD1;
		pImgD2 = m_ImgD2;
		pImgD3 = m_ImgD3;
		pImgD4 = m_ImgD4;
		pImgD5 = m_ImgD5;
		pMergeD = m_pMergeD;
		pPhaseD = m_pPhaseD;
		pMaskD = m_pMaskD;	
		break;
	}
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CreateSolvePhaseDataBuffer(const int nStep, int W, int H, const int ThreadNO)
{
#ifdef  CUDA_USE
	const int DataSize =  W * H;
	bool bSuccess = true;
	switch(ThreadNO)
	{
	case THREAD_01:	
		if( DataSize > m_PhaseBufferSizeD_01 )
		{
			m_PhaseBufferSizeD_01 = DataSize;
			cudaFree(m_ImgD1_01);	m_ImgD1_01 = NULL;
			cudaFree(m_ImgD2_01);	m_ImgD2_01 = NULL;
			cudaFree(m_ImgD3_01);	m_ImgD3_01 = NULL;
			if( nStep >= 4 )
			{ cudaFree(m_ImgD4_01);	m_ImgD4_01 = NULL; }
			if( nStep >= 5 )
			{ cudaFree(m_ImgD5_01);	m_ImgD5_01 = NULL; }

			cudaFree(m_pMergeD_01);	m_pMergeD_01 = NULL;
			cudaFree(m_pPhaseD_01);	m_pPhaseD_01 = NULL;
			cudaFree(m_pMaskD_01);	m_pMaskD_01 = NULL;
			
			cudaFree(m_pMergeD_F2_01);	m_pMergeD_F2_01 = NULL;
			cudaFree(m_pPhaseD_F2_01);	m_pPhaseD_F2_01 = NULL;
			cudaFree(m_pMaskD_F2_01);	m_pMaskD_F2_01 = NULL;

			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD1_01, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_ImgD1_01 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD2_01, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_ImgD2_01 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD3_01, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_ImgD3_01 Error. %s", cudaGetErrorString(err));
				return false;
			}		
			if( nStep >= 4 )
			{
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD4_01, sizeof(unsigned char)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_ImgD4_01 Error. %s", cudaGetErrorString(err));
					return false;
				}
			}
			if( nStep >= 5 )
			{
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD5_01, sizeof(unsigned char)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_ImgD5_01 Error. %s", cudaGetErrorString(err));
					return false;
				}
			}

			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMergeD_01, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMergeD_01 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pPhaseD_01, sizeof(short)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pPhaseD_01 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMaskD_01, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMaskD_01 Error. %s", cudaGetErrorString(err));
				return false;
			}

			
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMergeD_F2_01, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMergeD_F2_01 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pPhaseD_F2_01, sizeof(short)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pPhaseD_F2_01 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMaskD_F2_01, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMaskD_F2_01 Error. %s", cudaGetErrorString(err));
				return false;
			}
		}
		break;
	case THREAD_02:
		if( DataSize > m_PhaseBufferSizeD_02 )
		{
			m_PhaseBufferSizeD_02 = DataSize;
			cudaFree(m_ImgD1_02);	m_ImgD1_02 = NULL;
			cudaFree(m_ImgD2_02);	m_ImgD2_02 = NULL;
			cudaFree(m_ImgD3_02);	m_ImgD3_02 = NULL;
			if( nStep >= 4 )
			{ cudaFree(m_ImgD4_02);	m_ImgD4_02 = NULL; }
			if( nStep >= 5 )
			{ cudaFree(m_ImgD5_02);	m_ImgD5_02 = NULL; }
			cudaFree(m_pMergeD_02);	m_pMergeD_02 = NULL;
			cudaFree(m_pPhaseD_02);	m_pPhaseD_02 = NULL;
			cudaFree(m_pMaskD_02);	m_pMaskD_02 = NULL;
			
			cudaFree(m_pMergeD_F2_02);	m_pMergeD_F2_02 = NULL;
			cudaFree(m_pPhaseD_F2_02);	m_pPhaseD_F2_02 = NULL;
			cudaFree(m_pMaskD_F2_02);	m_pMaskD_F2_02 = NULL;

			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD1_02, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_ImgD1_02 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD2_02, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_ImgD2_02 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD3_02, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_ImgD3_02 Error. %s", cudaGetErrorString(err));
				return false;
			}		
			if( nStep >= 4 )
			{
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD4_02, sizeof(unsigned char)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_ImgD4_02 Error. %s", cudaGetErrorString(err));
					return false;
				}
			}
			if( nStep >= 5 )
			{
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD5_02, sizeof(unsigned char)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_ImgD5_02 Error. %s", cudaGetErrorString(err));
					return false;
				}
			}

			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMergeD_02, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMergeD_02 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pPhaseD_02, sizeof(short)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pPhaseD_02 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMaskD_02, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMaskD_02 Error. %s", cudaGetErrorString(err));
				return false;
			}

			
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMergeD_F2_02, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMergeD_F2_02 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pPhaseD_F2_02, sizeof(short)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pPhaseD_F2_02 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMaskD_F2_02, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMaskD_F2_02 Error. %s", cudaGetErrorString(err));
				return false;
			}
		}
		break;
	case THREAD_03:
		if( DataSize > m_PhaseBufferSizeD_03 )
		{
			m_PhaseBufferSizeD_03 = DataSize;
			cudaFree(m_ImgD1_03);	m_ImgD1_03 = NULL;
			cudaFree(m_ImgD2_03);	m_ImgD2_03 = NULL;
			cudaFree(m_ImgD3_03);	m_ImgD3_03 = NULL;
			if( nStep >= 4 )
			{ cudaFree(m_ImgD4_03);	m_ImgD4_03 = NULL; }
			if( nStep >= 5 )
			{ cudaFree(m_ImgD5_03);	m_ImgD5_03 = NULL; }
			cudaFree(m_pMergeD_03);	m_pMergeD_03 = NULL;
			cudaFree(m_pPhaseD_03);	m_pPhaseD_03 = NULL;
			cudaFree(m_pMaskD_03);	m_pMaskD_03 = NULL;
			
			cudaFree(m_pMergeD_F2_03);	m_pMergeD_F2_03 = NULL;
			cudaFree(m_pPhaseD_F2_03);	m_pPhaseD_F2_03 = NULL;
			cudaFree(m_pMaskD_F2_03);	m_pMaskD_F2_03 = NULL;

			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD1_03, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_ImgD1_03 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD2_03, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_ImgD2_03 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD3_03, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_ImgD3_03 Error. %s", cudaGetErrorString(err));
				return false;
			}		
			if( nStep >= 4 )
			{
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD4_03, sizeof(unsigned char)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_ImgD4_03 Error. %s", cudaGetErrorString(err));
					return false;
				}
			}
			if( nStep >= 5 )
			{
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD5_03, sizeof(unsigned char)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_ImgD5_03 Error. %s", cudaGetErrorString(err));
					return false;
				}
			}

			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMergeD_03, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMergeD_03 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pPhaseD_03, sizeof(short)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pPhaseD_03 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMaskD_03, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMaskD_03 Error. %s", cudaGetErrorString(err));
				return false;
			}

			
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMergeD_F2_03, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMergeD_F2_03 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pPhaseD_F2_03, sizeof(short)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pPhaseD_F2_03 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMaskD_F2_03, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMaskD_F2_03 Error. %s", cudaGetErrorString(err));
				return false;
			}
		}
		break;
	case THREAD_04:
		if( DataSize > m_PhaseBufferSizeD_04 )
		{
			m_PhaseBufferSizeD_04 = DataSize;
			cudaFree(m_ImgD1_04);	m_ImgD1_04 = NULL;
			cudaFree(m_ImgD2_04);	m_ImgD2_04 = NULL;
			cudaFree(m_ImgD3_04);	m_ImgD3_04 = NULL;
			if( nStep >= 4 )
			{ cudaFree(m_ImgD4_04);	m_ImgD4_04 = NULL; }
			if( nStep >= 5 )
			{ cudaFree(m_ImgD5_04);	m_ImgD5_04 = NULL; }
			cudaFree(m_pMergeD_04);	m_pMergeD_04 = NULL;
			cudaFree(m_pPhaseD_04);	m_pPhaseD_04 = NULL;
			cudaFree(m_pMaskD_04);	m_pMaskD_04 = NULL;			
			
			cudaFree(m_pMergeD_F2_04);	m_pMergeD_F2_04 = NULL;
			cudaFree(m_pPhaseD_F2_04);	m_pPhaseD_F2_04 = NULL;
			cudaFree(m_pMaskD_F2_04);	m_pMaskD_F2_04 = NULL;

			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD1_04, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_ImgD1_04 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD2_04, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_ImgD2_04 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD3_04, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_ImgD3_04 Error. %s", cudaGetErrorString(err));
				return false;
			}		
			if( nStep >= 4 )
			{
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD4_04, sizeof(unsigned char)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_ImgD4_04 Error. %s", cudaGetErrorString(err));
					return false;
				}
			}
			if( nStep >= 5 )
			{
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD5_04, sizeof(unsigned char)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_ImgD5_04 Error. %s", cudaGetErrorString(err));
					return false;
				}
			}

			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMergeD_04, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMergeD_04 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pPhaseD_04, sizeof(short)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pPhaseD_04 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMaskD_04, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMaskD_04 Error. %s", cudaGetErrorString(err));
				return false;
			}

			
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMergeD_F2_04, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMergeD_F2_04 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pPhaseD_F2_04, sizeof(short)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pPhaseD_F2_04 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMaskD_F2_04, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMaskD_F2_04 Error. %s", cudaGetErrorString(err));
				return false;
			}
		}
		break;
	default:
		if( DataSize > m_PhaseBufferSizeD )
		{
			m_PhaseBufferSizeD = DataSize;
			cudaFree(m_ImgD1);m_ImgD1 = NULL;
			cudaFree(m_ImgD2);m_ImgD2 = NULL;
			cudaFree(m_ImgD3);m_ImgD3 = NULL;
			if( nStep >= 4 )
			{ cudaFree(m_ImgD4);m_ImgD4 = NULL; }
			if( nStep >= 5 )
			{ cudaFree(m_ImgD5);m_ImgD5 = NULL; }
			cudaFree(m_pMergeD);	m_pMergeD = NULL;
			cudaFree(m_pPhaseD);m_pPhaseD = NULL;
			cudaFree(m_pMaskD);m_pMaskD = NULL;
			
			cudaFree(m_pMergeD_F2);	m_pMergeD_F2 = NULL;
			cudaFree(m_pPhaseD_F2);	m_pPhaseD_F2 = NULL;
			cudaFree(m_pMaskD_F2);	m_pMaskD_F2 = NULL;

			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD1, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_ImgD1 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD2, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_ImgD2 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD3, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_ImgD3 Error. %s", cudaGetErrorString(err));
				return false;
			}		
			if( nStep >= 4 )
			{
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD4, sizeof(unsigned char)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_ImgD4 Error. %s", cudaGetErrorString(err));
					return false;
				}
			}
			if( nStep >= 5 )
			{
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_ImgD5, sizeof(unsigned char)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_ImgD5 Error. %s", cudaGetErrorString(err));
					return false;
				}
			}

			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMergeD, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMergeD Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pPhaseD, sizeof(short)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pPhaseD Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMaskD, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMaskD Error. %s", cudaGetErrorString(err));
				return false;
			}		

			
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMergeD_F2, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMergeD_F2 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pPhaseD_F2, sizeof(short)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pPhaseD_F2 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_pMaskD_F2, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_pMaskD_F2 Error. %s", cudaGetErrorString(err));
				return false;
			}
		}
		break;
	}
#endif //#ifndef CUDA_USE

	return true;
}
//-------------------------------------------------------------------------//
void CCudaFunc::ReleaseSolvePhaseDataBuffer()
{
	#ifdef  CUDA_USE
//THREAD_00
	m_PhaseBufferSizeD = 0;
	cudaFree(m_ImgD1);m_ImgD1 = NULL;
	cudaFree(m_ImgD2);m_ImgD2 = NULL;
	cudaFree(m_ImgD3);m_ImgD3 = NULL;
	cudaFree(m_ImgD4);m_ImgD4 = NULL;
	cudaFree(m_ImgD5);m_ImgD5 = NULL;
	cudaFree(m_pPhaseD);m_pPhaseD = NULL;
	cudaFree(m_pMaskD);m_pMaskD = NULL;
	cudaFree(m_pMergeD);m_pMergeD = NULL;
	
	cudaFree(m_pPhaseD_F2);	m_pPhaseD_F2 = NULL;
	cudaFree(m_pMaskD_F2);	m_pMaskD_F2 = NULL;
	cudaFree(m_pMergeD_F2);	m_pMergeD_F2 = NULL;

//THREAD_01
	m_PhaseBufferSizeD_01 = 0;
	cudaFree(m_ImgD1_01);	m_ImgD1_01 = NULL;
	cudaFree(m_ImgD2_01);	m_ImgD2_01 = NULL;
	cudaFree(m_ImgD3_01);	m_ImgD3_01 = NULL;
	cudaFree(m_ImgD4_01);	m_ImgD4_01 = NULL;
	cudaFree(m_ImgD5_01);	m_ImgD5_01 = NULL;
	cudaFree(m_pPhaseD_01);	m_pPhaseD_01 = NULL;
	cudaFree(m_pMaskD_01);	m_pMaskD_01 = NULL;
	cudaFree(m_pMergeD_01);	m_pMergeD_01 = NULL;
	
	cudaFree(m_pPhaseD_F2_01);	m_pPhaseD_F2_01 = NULL;
	cudaFree(m_pMaskD_F2_01);	m_pMaskD_F2_01 = NULL;
	cudaFree(m_pMergeD_F2_01);	m_pMergeD_F2_01 = NULL;
//THREAD_02
	m_PhaseBufferSizeD_02 = 0;
	cudaFree(m_ImgD1_02);	m_ImgD1_02 = NULL;
	cudaFree(m_ImgD2_02);	m_ImgD2_02 = NULL;
	cudaFree(m_ImgD3_02);	m_ImgD3_02 = NULL;
	cudaFree(m_ImgD4_02);	m_ImgD4_02 = NULL;
	cudaFree(m_ImgD5_02);	m_ImgD5_02 = NULL;
	cudaFree(m_pPhaseD_02);	m_pPhaseD_02 = NULL;
	cudaFree(m_pMaskD_02);	m_pMaskD_02 = NULL;
	cudaFree(m_pMergeD_02);	m_pMergeD_02 = NULL;
	
	cudaFree(m_pPhaseD_F2_02);	m_pPhaseD_F2_02 = NULL;
	cudaFree(m_pMaskD_F2_02);	m_pMaskD_F2_02 = NULL;
	cudaFree(m_pMergeD_F2_02);	m_pMergeD_F2_02 = NULL;
//THREAD_03
	m_PhaseBufferSizeD_03 = 0;
	cudaFree(m_ImgD1_03);	m_ImgD1_03 = NULL;
	cudaFree(m_ImgD2_03);	m_ImgD2_03 = NULL;
	cudaFree(m_ImgD3_03);	m_ImgD3_03 = NULL;
	cudaFree(m_ImgD4_03);	m_ImgD4_03 = NULL;
	cudaFree(m_ImgD5_03);	m_ImgD5_03 = NULL;
	cudaFree(m_pPhaseD_03);	m_pPhaseD_03 = NULL;
	cudaFree(m_pMaskD_03);	m_pMaskD_03 = NULL;
	cudaFree(m_pMergeD_03);	m_pMergeD_03 = NULL;
	
	cudaFree(m_pPhaseD_F2_03);	m_pPhaseD_F2_03 = NULL;
	cudaFree(m_pMaskD_F2_03);	m_pMaskD_F2_03 = NULL;
	cudaFree(m_pMergeD_F2_03);	m_pMergeD_F2_03 = NULL;
//THREAD_04
	m_PhaseBufferSizeD = 0;
	cudaFree(m_ImgD1_04);	m_ImgD1_04 = NULL;
	cudaFree(m_ImgD2_04);	m_ImgD2_04 = NULL;
	cudaFree(m_ImgD3_04);	m_ImgD3_04 = NULL;
	cudaFree(m_ImgD4_04);	m_ImgD4_04 = NULL;
	cudaFree(m_ImgD5_04);	m_ImgD5_04 = NULL;
	cudaFree(m_pPhaseD_04);	m_pPhaseD_04 = NULL;
	cudaFree(m_pMaskD_04);	m_pMaskD_04 = NULL;
	cudaFree(m_pMergeD_04);	m_pMergeD_04 = NULL;
	
	cudaFree(m_pPhaseD_F2_04);	m_pPhaseD_F2_04 = NULL;
	cudaFree(m_pMaskD_F2_04);	m_pMaskD_F2_04 = NULL;
	cudaFree(m_pMergeD_F2_04);	m_pMergeD_F2_04 = NULL;
	#endif //#ifndef CUDA_USE
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CreateBasePhasePlaneBuffer(const int DataSize, const int LightSide, const int FrequenceMode)
{
#ifdef  CUDA_USE
	bool bSuccess = false;
	if( LightSide == LIGHT_MODE_A )
	{
		if( FrequenceMode == FREQUENCE_MODE_P1 )
		{
			if( DataSize > m_BasePhaseBufferSizeD_A )
			{
				cudaFree(m_BasePhaseD_A);	m_BasePhaseD_A = NULL;
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_BasePhaseD_A, sizeof(short)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_BasePhaseD_L Error. %s", cudaGetErrorString(err));
					return false;
				}
				m_BasePhaseBufferSizeD_A = DataSize;
			}
		}
		else
		{
			if( DataSize > m_BasePhaseBufferSizeD_A_F2 )
			{
				cudaFree(m_BasePhaseD_A_F2);	m_BasePhaseD_A_F2 = NULL;
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_BasePhaseD_A_F2, sizeof(short)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_BasePhaseD_L Error. %s", cudaGetErrorString(err));
					return false;
				}
				m_BasePhaseBufferSizeD_A_F2 = DataSize;
			}
		}
	}
	else if( LightSide == LIGHT_MODE_B )
	{
		if( FrequenceMode == FREQUENCE_MODE_P1 )
		{
			if( DataSize > m_BasePhaseBufferSizeD_B )
			{
				cudaFree(m_BasePhaseD_B);	m_BasePhaseD_B = NULL;
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_BasePhaseD_B, sizeof(short)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_BasePhaseD_B Error. %s", cudaGetErrorString(err));
					return false;
				}
				m_BasePhaseBufferSizeD_B = DataSize;
			}
		}
		else
		{
			if( DataSize > m_BasePhaseBufferSizeD_B_F2 )
			{
				cudaFree(m_BasePhaseD_B_F2);	m_BasePhaseD_B_F2 = NULL;
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_BasePhaseD_B_F2, sizeof(short)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_BasePhaseD_L Error. %s", cudaGetErrorString(err));
					return false;
				}
				m_BasePhaseBufferSizeD_B_F2 = DataSize;
			}
		}
	}
	else if( LightSide == LIGHT_MODE_C )
	{
		if( FrequenceMode == FREQUENCE_MODE_P1 )
		{
			if( DataSize > m_BasePhaseBufferSizeD_C )
			{
				cudaFree(m_BasePhaseD_C);	m_BasePhaseD_C = NULL;
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_BasePhaseD_C, sizeof(short)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_BasePhaseD_B Error. %s", cudaGetErrorString(err));
					return false;
				}
				m_BasePhaseBufferSizeD_C = DataSize;
			}
		}
		else
		{
			if( DataSize > m_BasePhaseBufferSizeD_C_F2 )
			{
				cudaFree(m_BasePhaseD_C_F2);	m_BasePhaseD_C_F2 = NULL;
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_BasePhaseD_C_F2, sizeof(short)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_BasePhaseD_L Error. %s", cudaGetErrorString(err));
					return false;
				}
				m_BasePhaseBufferSizeD_C_F2 = DataSize;
			}
		}
	}
	else if( LightSide == LIGHT_MODE_D )
	{
		if( FrequenceMode == FREQUENCE_MODE_P1 )
		{
			if( DataSize > m_BasePhaseBufferSizeD_D )
			{
				cudaFree(m_BasePhaseD_D);	m_BasePhaseD_D = NULL;
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_BasePhaseD_D, sizeof(short)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_BasePhaseD_D Error. %s", cudaGetErrorString(err));
					return false;
				}
				m_BasePhaseBufferSizeD_D = DataSize;
			}
		}
		else
		{
			if( DataSize > m_BasePhaseBufferSizeD_D_F2 )
			{
				cudaFree(m_BasePhaseD_D_F2);	m_BasePhaseD_D_F2 = NULL;
				bSuccess = checkCudaErrors( cudaMalloc((void**) &m_BasePhaseD_D_F2, sizeof(short)*DataSize));	
				if( bSuccess == false )
				{
					cudaError_t err = cudaGetLastError();
					::sprintf(m_ErrorString, "CudaMalloc m_BasePhaseD_L Error. %s", cudaGetErrorString(err));
					return false;
				}
				m_BasePhaseBufferSizeD_D_F2 = DataSize;
			}
		}
	}
	else
	{
		::sprintf(m_ErrorString, "Error LightSide!");
		return false;
	}
#endif
	return true;
}
//-------------------------------------------------------------------------//
void CCudaFunc::ReleaseBasePhasePlaneBuffer(const int FrequenceMode)
{
#ifdef  CUDA_USE
	if( FrequenceMode == FREQUENCE_MODE_P1 )
	{
		m_BasePhaseBufferSizeD_A = 0;
		m_BasePhaseBufferSizeD_B = 0;
		m_BasePhaseBufferSizeD_C = 0;
		m_BasePhaseBufferSizeD_D = 0;
		cudaFree(m_BasePhaseD_A);	m_BasePhaseD_A = NULL;
		cudaFree(m_BasePhaseD_B);	m_BasePhaseD_B = NULL;
		cudaFree(m_BasePhaseD_C);	m_BasePhaseD_C = NULL;
		cudaFree(m_BasePhaseD_D);	m_BasePhaseD_D = NULL;
	}
	else
	{
		m_BasePhaseBufferSizeD_A_F2 = 0;
		m_BasePhaseBufferSizeD_B_F2 = 0;
		m_BasePhaseBufferSizeD_C_F2 = 0;
		m_BasePhaseBufferSizeD_D_F2 = 0;
		cudaFree(m_BasePhaseD_A_F2);	m_BasePhaseD_A_F2 = NULL;
		cudaFree(m_BasePhaseD_B_F2);	m_BasePhaseD_B_F2 = NULL;
		cudaFree(m_BasePhaseD_C_F2);	m_BasePhaseD_C_F2 = NULL;
		cudaFree(m_BasePhaseD_D_F2);	m_BasePhaseD_D_F2 = NULL;	
	}
#endif
}
//-------------------------------------------------------------------------//
void CCudaFunc::GetBasePhasePlaneDevicePtr(short *&pBasePhaseD, const int LightSide, const int FrequenceMode)
{
	switch(LightSide)
	{
	case LIGHT_MODE_A:	
		if( FrequenceMode == FREQUENCE_MODE_P1 ){ pBasePhaseD = m_BasePhaseD_A; }
		else									{ pBasePhaseD = m_BasePhaseD_A_F2; }
		break;
	case LIGHT_MODE_B:
		if( FrequenceMode == FREQUENCE_MODE_P1 ){ pBasePhaseD = m_BasePhaseD_B; }
		else									{ pBasePhaseD = m_BasePhaseD_B_F2; }
		break;
	case LIGHT_MODE_C:
		if( FrequenceMode == FREQUENCE_MODE_P1 ){ pBasePhaseD = m_BasePhaseD_C; }
		else									{ pBasePhaseD = m_BasePhaseD_C_F2; }
		break;
	case LIGHT_MODE_D:
		if( FrequenceMode == FREQUENCE_MODE_P1 ){ pBasePhaseD = m_BasePhaseD_D; }
		else									{ pBasePhaseD = m_BasePhaseD_D_F2; }
		break;
	default:
		pBasePhaseD = NULL;
		break;
	}
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CreateFFCBaseBuffer(const int DataSize, const int LightSide)
{
#ifdef  CUDA_USE
	bool bSuccess = false;
	if( LightSide == LIGHT_MODE_A )
	{
		if( DataSize > m_FFCBaseBufferSizeD_A )
		{
			cudaFree(m_FFCGainD_A);	m_FFCGainD_A = NULL;
			cudaFree(m_FFCBaseD_A);	m_FFCBaseD_A = NULL;
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_FFCGainD_A, sizeof(float)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_FFCGainD_L Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_FFCBaseD_A, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_FFCBaseD_A Error. %s", cudaGetErrorString(err));
				return false;
			}
			m_FFCBaseBufferSizeD_A = DataSize;
		}
	}
	else if( LightSide == LIGHT_MODE_B )
	{
		if( DataSize > m_FFCBaseBufferSizeD_B )
		{
			cudaFree(m_FFCGainD_B);	m_FFCGainD_B = NULL;
			cudaFree(m_FFCBaseD_B);	m_FFCBaseD_B = NULL;
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_FFCGainD_B, sizeof(float)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_FFCGainD_B Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_FFCBaseD_B, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_FFCBaseD_B Error. %s", cudaGetErrorString(err));
				return false;
			}
			m_FFCBaseBufferSizeD_B = DataSize;
		}
	}
	else if( LightSide == LIGHT_MODE_C )
	{
		if( DataSize > m_FFCBaseBufferSizeD_C )
		{
			cudaFree(m_FFCGainD_C);	m_FFCGainD_C = NULL;
			cudaFree(m_FFCBaseD_C);	m_FFCBaseD_C = NULL;
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_FFCGainD_C, sizeof(float)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_FFCGainD_C Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_FFCBaseD_C, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_FFCBaseD_C Error. %s", cudaGetErrorString(err));
				return false;
			}
			m_FFCBaseBufferSizeD_C = DataSize;
		}
	}
	else if( LightSide == LIGHT_MODE_D )
	{
		if( DataSize > m_FFCBaseBufferSizeD_D )
		{
			cudaFree(m_FFCGainD_D);	m_FFCGainD_D = NULL;
			cudaFree(m_FFCBaseD_D);	m_FFCBaseD_D = NULL;
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_FFCGainD_D, sizeof(float)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_FFCGainD_D Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_FFCBaseD_D, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_FFCBaseD_D Error. %s", cudaGetErrorString(err));
				return false;
			}
			m_FFCBaseBufferSizeD_D = DataSize;
		}
	}
	else
	{
		::sprintf(m_ErrorString, "Error LightSide!");
		return false;
	}
	
#endif
	return true;
}
//-------------------------------------------------------------------------//
void CCudaFunc::ReleaseFFCBaseBuffer()
{
#ifdef  CUDA_USE
	m_FFCBaseBufferSizeD_A = 0;
	m_FFCBaseBufferSizeD_B = 0;
	m_FFCBaseBufferSizeD_C = 0;
	m_FFCBaseBufferSizeD_D = 0;

	cudaFree(m_FFCGainD_A);	m_FFCGainD_A = NULL;
	cudaFree(m_FFCGainD_B);	m_FFCGainD_B = NULL;
	cudaFree(m_FFCGainD_C);	m_FFCGainD_C = NULL;
	cudaFree(m_FFCGainD_D);	m_FFCGainD_D = NULL;

	cudaFree(m_FFCBaseD_A);	m_FFCBaseD_A = NULL;
	cudaFree(m_FFCBaseD_B);	m_FFCBaseD_B = NULL;
	cudaFree(m_FFCBaseD_C);	m_FFCBaseD_C = NULL;
	cudaFree(m_FFCBaseD_D);	m_FFCBaseD_D = NULL;
	
#endif
}
//-------------------------------------------------------------------------//
void CCudaFunc::GetFFCBaseBufferDevicePtr(float *&pGainD, unsigned char *&pBaseD, const int LightSide)
{
	switch(LightSide)
	{
	case LIGHT_MODE_A:	
		pGainD = m_FFCGainD_A;
		pBaseD = m_FFCBaseD_A;
		break;
	case LIGHT_MODE_B:
		pGainD = m_FFCGainD_B;
		pBaseD = m_FFCBaseD_B;
		break;
	case LIGHT_MODE_C:
		pGainD = m_FFCGainD_C;
		pBaseD = m_FFCBaseD_C;
		break;
	case LIGHT_MODE_D:
		pGainD = m_FFCGainD_D;
		pBaseD = m_FFCBaseD_D;
		break;
	default:
		pGainD = NULL;
		pBaseD = NULL;
		break;
	}
}
//-------------------------------------------------------------------------//
void CCudaFunc::GetPhaseSmoothDataPtr(short *&pPhaseSmoothDestD, float *&pPhaseSmoothTempSinBuffer, float *&pPhaseSmoothTempCosBuffer, unsigned char *&pPhaseSmoothTempMaskBuffer, const int ThreadNO)
{
	switch(ThreadNO)
	{
	case THREAD_01:	
		pPhaseSmoothDestD = m_PhaseSmoothDestD_01;
		pPhaseSmoothTempSinBuffer = m_PhaseSmoothTempSinBufferD_01;
		pPhaseSmoothTempCosBuffer = m_PhaseSmoothTempCosBufferD_01;
		pPhaseSmoothTempMaskBuffer = m_PhaseSmoothTempMaskBufferD_01;
		break;
	case THREAD_02:
		pPhaseSmoothDestD = m_PhaseSmoothDestD_02;
		pPhaseSmoothTempSinBuffer = m_PhaseSmoothTempSinBufferD_02;
		pPhaseSmoothTempCosBuffer = m_PhaseSmoothTempCosBufferD_02;
		pPhaseSmoothTempMaskBuffer = m_PhaseSmoothTempMaskBufferD_02;
		break;
	case THREAD_03:
		pPhaseSmoothDestD = m_PhaseSmoothDestD_03;
		pPhaseSmoothTempSinBuffer = m_PhaseSmoothTempSinBufferD_03;
		pPhaseSmoothTempCosBuffer = m_PhaseSmoothTempCosBufferD_03;
		pPhaseSmoothTempMaskBuffer = m_PhaseSmoothTempMaskBufferD_03;
		break;
	case THREAD_04:
		pPhaseSmoothDestD = m_PhaseSmoothDestD_04;
		pPhaseSmoothTempSinBuffer = m_PhaseSmoothTempSinBufferD_04;
		pPhaseSmoothTempCosBuffer = m_PhaseSmoothTempCosBufferD_04;
		pPhaseSmoothTempMaskBuffer = m_PhaseSmoothTempMaskBufferD_04;
		break;
	default:
		pPhaseSmoothDestD = m_PhaseSmoothDestD;
		pPhaseSmoothTempSinBuffer = m_PhaseSmoothTempSinBufferD;
		pPhaseSmoothTempCosBuffer = m_PhaseSmoothTempCosBufferD;
		pPhaseSmoothTempMaskBuffer = m_PhaseSmoothTempMaskBufferD;
		break;
	}

}
//-------------------------------------------------------------------------//
bool CCudaFunc::CreatePhaseSmoothDataBuffer(int W, int H, const int ThreadNO)
{
#ifdef  CUDA_USE
	const int DataSize =  W * H ;
	bool bSuccess = true;
	switch(ThreadNO)
	{
	case THREAD_01:
		if( DataSize > m_PhaseSmoothBufferSizeD_01 )
		{
			m_PhaseSmoothBufferSizeD_01 = DataSize;
			cudaFree(m_PhaseSmoothDestD_01);		m_PhaseSmoothDestD_01 = NULL;
			cudaFree(m_PhaseSmoothTempSinBufferD_01);	m_PhaseSmoothTempSinBufferD_01 = NULL;
			cudaFree(m_PhaseSmoothTempCosBufferD_01);	m_PhaseSmoothTempCosBufferD_01 = NULL;
			cudaFree(m_PhaseSmoothTempMaskBufferD_01);	m_PhaseSmoothTempMaskBufferD_01 = NULL;
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothDestD_01, sizeof(short)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothDestD_01 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothTempSinBufferD_01, sizeof(float)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothTempSinBufferD_01 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothTempCosBufferD_01, sizeof(float)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothTempCosBufferD_01 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothTempMaskBufferD_01, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothTempMaskBufferD_01 Error. %s", cudaGetErrorString(err));
				return false;
			}
		}
		break;
	case THREAD_02:
		if( DataSize > m_PhaseSmoothBufferSizeD_02 )
		{
			m_PhaseSmoothBufferSizeD_02 = DataSize;
			cudaFree(m_PhaseSmoothDestD_02);		m_PhaseSmoothDestD_02 = NULL;
			cudaFree(m_PhaseSmoothTempSinBufferD_02);	m_PhaseSmoothTempSinBufferD_02 = NULL;
			cudaFree(m_PhaseSmoothTempCosBufferD_02);	m_PhaseSmoothTempCosBufferD_02 = NULL;
			cudaFree(m_PhaseSmoothTempMaskBufferD_02);	m_PhaseSmoothTempMaskBufferD_02 = NULL;
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothDestD_02, sizeof(short)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothDestD_02 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothTempSinBufferD_02, sizeof(float)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothTempSinBufferD_02 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothTempCosBufferD_02, sizeof(float)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothTempCosBufferD_02 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothTempMaskBufferD_02, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothTempMaskBufferD_02 Error. %s", cudaGetErrorString(err));
				return false;
			}
		}	
		break;
	case THREAD_03:
		if( DataSize > m_PhaseSmoothBufferSizeD_03 )
		{
			m_PhaseSmoothBufferSizeD_03 = DataSize;
			cudaFree(m_PhaseSmoothDestD_03);		m_PhaseSmoothDestD_03 = NULL;
			cudaFree(m_PhaseSmoothTempSinBufferD_03);	m_PhaseSmoothTempSinBufferD_03 = NULL;
			cudaFree(m_PhaseSmoothTempCosBufferD_03);	m_PhaseSmoothTempCosBufferD_03 = NULL;
			cudaFree(m_PhaseSmoothTempMaskBufferD_03);	m_PhaseSmoothTempMaskBufferD_03 = NULL;
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothDestD_03, sizeof(short)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothDestD_03 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothTempSinBufferD_03, sizeof(float)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothTempSinBufferD_03 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothTempCosBufferD_03, sizeof(float)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothTempCosBufferD_03 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothTempMaskBufferD_03, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothTempMaskBufferD_03 Error. %s", cudaGetErrorString(err));
				return false;
			}
		}
		break;
	case THREAD_04:
		if( DataSize > m_PhaseSmoothBufferSizeD_04 )
		{
			m_PhaseSmoothBufferSizeD_04 = DataSize;
			cudaFree(m_PhaseSmoothDestD_04);		m_PhaseSmoothDestD_04 = NULL;
			cudaFree(m_PhaseSmoothTempSinBufferD_04);	m_PhaseSmoothTempSinBufferD_04 = NULL;
			cudaFree(m_PhaseSmoothTempCosBufferD_04);	m_PhaseSmoothTempCosBufferD_04 = NULL;
			cudaFree(m_PhaseSmoothTempMaskBufferD_04);	m_PhaseSmoothTempMaskBufferD_04 = NULL;
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothDestD_04, sizeof(short)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothDestD_04 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothTempSinBufferD_04, sizeof(float)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothTempSinBufferD_04 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothTempCosBufferD_04, sizeof(float)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothTempCosBufferD_04 Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothTempMaskBufferD_04, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothTempMaskBufferD_04 Error. %s", cudaGetErrorString(err));
				return false;
			}
		}
		break;
	default:
		if( DataSize > m_PhaseSmoothBufferSizeD )
		{
			m_PhaseSmoothBufferSizeD = DataSize;
			cudaFree(m_PhaseSmoothDestD);		m_PhaseSmoothDestD = NULL;
			cudaFree(m_PhaseSmoothTempSinBufferD);	m_PhaseSmoothTempSinBufferD = NULL;
			cudaFree(m_PhaseSmoothTempCosBufferD);	m_PhaseSmoothTempCosBufferD = NULL;
			cudaFree(m_PhaseSmoothTempMaskBufferD);	m_PhaseSmoothTempMaskBufferD = NULL;
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothDestD, sizeof(short)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothDestD Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothTempSinBufferD, sizeof(float)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothTempSinBufferD Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothTempCosBufferD, sizeof(float)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothTempCosBufferD Error. %s", cudaGetErrorString(err));
				return false;
			}
			bSuccess = checkCudaErrors( cudaMalloc((void**) &m_PhaseSmoothTempMaskBufferD, sizeof(unsigned char)*DataSize));	
			if( bSuccess == false )
			{
				cudaError_t err = cudaGetLastError();
				::sprintf(m_ErrorString, "CudaMalloc m_PhaseSmoothTempMaskBufferD Error. %s", cudaGetErrorString(err));
				return false;
			}
		}
		break;
	}
#endif
	return true;
}
//-------------------------------------------------------------------------//
void CCudaFunc::ReleasePhaseSmoothDataBuffer()
{
	#ifdef  CUDA_USE
//THREAD_00
	m_PhaseSmoothBufferSizeD = 0;
	cudaFree(m_PhaseSmoothDestD);			m_PhaseSmoothDestD = NULL;
	cudaFree(m_PhaseSmoothTempSinBufferD);	m_PhaseSmoothTempSinBufferD = NULL;
	cudaFree(m_PhaseSmoothTempCosBufferD);	m_PhaseSmoothTempCosBufferD = NULL;
	cudaFree(m_PhaseSmoothTempMaskBufferD);	m_PhaseSmoothTempMaskBufferD = NULL;
//THREAD_01
	m_PhaseSmoothBufferSizeD_01 = 0;
	cudaFree(m_PhaseSmoothDestD_01);			m_PhaseSmoothDestD_01 = NULL;
	cudaFree(m_PhaseSmoothTempSinBufferD_01);	m_PhaseSmoothTempSinBufferD_01 = NULL;
	cudaFree(m_PhaseSmoothTempCosBufferD_01);	m_PhaseSmoothTempCosBufferD_01 = NULL;
	cudaFree(m_PhaseSmoothTempMaskBufferD_01);	m_PhaseSmoothTempMaskBufferD_01 = NULL;
//THREAD_02
	m_PhaseSmoothBufferSizeD_02 = 0;
	cudaFree(m_PhaseSmoothDestD_02);		m_PhaseSmoothDestD_02 = NULL;
	cudaFree(m_PhaseSmoothTempSinBufferD_02);	m_PhaseSmoothTempSinBufferD_02 = NULL;
	cudaFree(m_PhaseSmoothTempCosBufferD_02);	m_PhaseSmoothTempCosBufferD_02 = NULL;
	cudaFree(m_PhaseSmoothTempMaskBufferD_02);	m_PhaseSmoothTempMaskBufferD_02 = NULL;
//THREAD_03
	m_PhaseSmoothBufferSizeD_03 = 0;
	cudaFree(m_PhaseSmoothDestD_03);		m_PhaseSmoothDestD_03 = NULL;
	cudaFree(m_PhaseSmoothTempSinBufferD_03);	m_PhaseSmoothTempSinBufferD_03 = NULL;
	cudaFree(m_PhaseSmoothTempCosBufferD_03);	m_PhaseSmoothTempCosBufferD_03 = NULL;
	cudaFree(m_PhaseSmoothTempMaskBufferD_03);	m_PhaseSmoothTempMaskBufferD_03 = NULL;
//THREAD_04
	m_PhaseSmoothBufferSizeD_04 = 0;
	cudaFree(m_PhaseSmoothDestD_04);		m_PhaseSmoothDestD_04 = NULL;
	cudaFree(m_PhaseSmoothTempSinBufferD_04);	m_PhaseSmoothTempSinBufferD_04 = NULL;
	cudaFree(m_PhaseSmoothTempCosBufferD_04);	m_PhaseSmoothTempCosBufferD_04 = NULL;
	cudaFree(m_PhaseSmoothTempMaskBufferD_04);	m_PhaseSmoothTempMaskBufferD_04 = NULL;
	#endif //#ifndef CUDA_USE
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CreateAverageSmoothDataBuffer(size_t size)
{
#ifdef  CUDA_USE	
	if( size > m_MaxAverageSmoothSize )
	{
		m_MaxAverageSmoothSize = size;
		cudaFree(m_AverageSmoothSrcD);m_AverageSmoothSrcD = NULL;
		cudaFree(m_AverageSmoothDestD);m_AverageSmoothDestD = NULL;
		cudaFree(m_AverageSmoothBufferD);m_AverageSmoothBufferD = NULL;

		cudaMalloc(	&m_AverageSmoothSrcD, sizeof(unsigned char) * size);
		cudaMalloc( &m_AverageSmoothDestD, sizeof(unsigned char) * size);
		cudaMalloc( &m_AverageSmoothBufferD, sizeof(unsigned short) * size);	

		if ( NULL==m_AverageSmoothSrcD || m_AverageSmoothDestD==NULL || m_AverageSmoothBufferD==NULL )
		{
			m_MaxAverageSmoothSize = 0;
			cudaFree(m_AverageSmoothSrcD);m_AverageSmoothSrcD = NULL;
			cudaFree(m_AverageSmoothDestD);m_AverageSmoothDestD = NULL;
			cudaFree(m_AverageSmoothBufferD);m_AverageSmoothBufferD = NULL;

			::sprintf(m_ErrorString, "CreateAverageSmoothDataBuffer Fault");
			return false;
		}
		cudaMemset(m_AverageSmoothBufferD ,0 , sizeof(unsigned short) * size);
	}
	return true;
#endif //#ifndef CUDA_USE
	return false;
}
//-------------------------------------------------------------------------//
void CCudaFunc::ReleaseAverageSmoothDataBuffer()
{
#ifdef  CUDA_USE
	m_MaxAverageSmoothSize = 0;
	cudaFree(m_AverageSmoothSrcD);m_AverageSmoothSrcD = NULL;
	cudaFree(m_AverageSmoothDestD);m_AverageSmoothDestD = NULL;
	cudaFree(m_AverageSmoothBufferD);m_AverageSmoothBufferD = NULL;
#endif //#ifndef CUDA_USE
}
//-------------------------------------------------------------------------//
void CCudaFunc::CreateGaussianSmoothDataBuffer(int W,int H)
{
#ifdef  CUDA_USE
	const int DataSize =  W * H ;
	if( DataSize > m_MaxGaussianSmoothSize )
	{
		m_MaxGaussianSmoothSize = DataSize;
		cudaFree(m_GaussianSmoothSrcD);m_GaussianSmoothSrcD = NULL;
		cudaFree(m_GaussianSmoothDestD);m_GaussianSmoothDestD = NULL;
		cudaFree(m_GaussianSmoothBufferD);m_GaussianSmoothBufferD = NULL;
		cudaMalloc(	&m_GaussianSmoothSrcD, sizeof(float) * DataSize);
		cudaMalloc( &m_GaussianSmoothDestD, sizeof(float) * DataSize);
		cudaMalloc( &m_GaussianSmoothBufferD, sizeof(float) * DataSize);	
		cudaMemset(m_GaussianSmoothBufferD ,0 , sizeof(float) * DataSize);
	}
#endif //#ifndef CUDA_USE
}
//-------------------------------------------------------------------------//
void CCudaFunc::ReleaseGaussianSmoothDataBuffer()
{
#ifdef  CUDA_USE
	m_MaxGaussianSmoothSize = 0;
	cudaFree(m_GaussianSmoothSrcD);m_GaussianSmoothSrcD = NULL;
	cudaFree(m_GaussianSmoothDestD);m_GaussianSmoothDestD = NULL;
	cudaFree(m_GaussianSmoothBufferD);m_GaussianSmoothBufferD = NULL;
#endif //#ifndef CUDA_USE
}
//-------------------------------------------------------------------------//
bool CCudaFunc::CudaRecursiveGaussian(unsigned char *pSrc, unsigned char *pDest, const int W, const int H, float sigma)//fang 101/07/24
{
	#ifdef  CUDA_USE
	if( GetIsCudaDeviceUse() == false )
	{
		::sprintf(m_ErrorString, "No Cuda Device!");
		return false;
	}
	if( pSrc == NULL ) 
	{
		::sprintf(m_ErrorString, "pSrc Error! pSrc == NULL!");
		return false;
	}
	if( pDest == NULL ) 
	{
		::sprintf(m_ErrorString, "pDest Error! pDest == NULL!");
		return false;
	}
	int Size = W*H;
	if( Size <= 0 )
	{
		::sprintf(m_ErrorString, "DoFFCProcess Error! Size <= 0 !");
		return false;	
	}

	//創建GPU Memery
	unsigned char *pSrcD;
	unsigned char *pDestD;   
	unsigned char *pTempD;  
	cudaMalloc(	&pSrcD, sizeof(unsigned char) * Size);
	cudaMalloc( &pDestD, sizeof(unsigned char) * Size);
	cudaMalloc( &pTempD, sizeof(unsigned char) * Size);

	cudaMemset(pDestD ,0 , sizeof(unsigned char) * Size);
	cudaMemcpy(pSrcD  ,pSrc,  sizeof(unsigned char) * Size , cudaMemcpyHostToDevice);

	//Host function
	DoCudaRecursiveGaussian(pSrcD, pDestD,pTempD, W, H, sigma);

	cudaMemcpy(pDest, pDestD, sizeof(unsigned char) * Size, cudaMemcpyDeviceToHost);


	cudaFree(pSrcD);
	cudaFree(pDestD);
	cudaFree(pTempD);
		
		return true;
	#else
		return false;
	#endif //#ifndef CUDA_USE

}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaImageAverageSmooth(unsigned int ImageW, unsigned int ImageH, unsigned int Step, const unsigned char *pSrc,unsigned char *pDest, int KernelSize)
{
#ifdef  CUDA_USE
	if( GetIsCudaDeviceUse() == false )
	{
		::sprintf(m_ErrorString, "No Cuda Device!");
		return false;
	}
	if( pSrc == NULL ) 
	{
		::sprintf(m_ErrorString, "pSrc Error! pSrc == NULL!");
		return false;
	}
	if( pDest == NULL ) 
	{
		::sprintf(m_ErrorString, "pDest Error! pDest == NULL!");
		return false;
	}
	if( (KernelSize % 2) ==0 )
	{
		::sprintf(m_ErrorString, "ImageSmoothMask Mod 2 =0 !");
		return false;
	}
	if( KernelSize > 15 )
	{
		::sprintf(m_ErrorString, "ImageSmoothMask > 15!");
		return false;
	}
	size_t ImageSize = Step*ImageH;
	if( ImageSize <= 0 )
	{
		::sprintf(m_ErrorString, "ImageSize <= 0!");
		return false;
	}

	if ( CreateAverageSmoothDataBuffer(ImageSize) == false )
	{	return false; }

	unsigned char  *pSrcD = this->m_AverageSmoothSrcD;
	unsigned char  *pDestD = this->m_AverageSmoothDestD;
	unsigned short *BufferD = this->m_AverageSmoothBufferD;
    
	cudaMemcpy(pSrcD, pSrc, ImageSize * sizeof(unsigned char), cudaMemcpyHostToDevice);

	cudaThreadSynchronize();
	//1
	cudaMemcpy(pDestD, pSrcD, ImageSize * sizeof(unsigned char), cudaMemcpyDeviceToDevice);
	cudaThreadSynchronize();
	DoCudaAverageSmoothRows(ImageW, ImageH, Step, pSrcD, BufferD, KernelSize);
	cudaThreadSynchronize();
	DoCudaAverageSmoothColumns(ImageW, ImageH, Step, BufferD, pDestD, KernelSize);
	cudaThreadSynchronize();
	cudaMemcpy(pDest, pDestD, ImageSize * sizeof(unsigned char), cudaMemcpyDeviceToHost);
	return true;
#else
	return false;
#endif //#ifndef CUDA_USE
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaDataGaussianSmooth(float *pSrc,float *pDest,int W,int H,int DataSmoothMask)
{
#ifdef  CUDA_USE
	if( GetIsCudaDeviceUse() == false )
	{
		::sprintf(m_ErrorString, "No Cuda Device!");
		return false;
	}
	if( pSrc == NULL ) 
	{
		::sprintf(m_ErrorString, "pSrc Error! pSrc == NULL!");
		return false;
	}
	if( pDest == NULL ) 
	{
		::sprintf(m_ErrorString, "pDest Error! pDest == NULL!");
		return false;
	}

	int DataSize = W * H;
	if( DataSize <= 0 )
	{
		::sprintf(m_ErrorString, "DataSize <= 0!");
		return false;
	}
	//Create 2D kernel Gaussian filter
	float *h_KernelRow;
	float *h_KernelCol;
	h_KernelRow = new float[DataSmoothMask];
	h_KernelCol = new float[DataSmoothMask];

	int i;
	float DivideNumber = 0;
	switch(DataSmoothMask)
	{
	case 1:			DivideNumber = 1;			break;
	case 3:			DivideNumber = 4;			break;
	case 5:			DivideNumber = 9;			break;
	case 7:			DivideNumber = 16;			break;
	case 9:			DivideNumber = 25;			break;
	case 11:		DivideNumber = 36;			break;
	case 13:		DivideNumber = 49;			break;
	case 15:		DivideNumber = 64;			break;
	default:			
	::sprintf(m_ErrorString, "DataSmoothMask error!");
		return false;
	}
	int KernelMaxValue = (DataSmoothMask+1)/2;
	for(i=0;i<KernelMaxValue;i++)
	{
		h_KernelRow[i] = (float)(i+1)/DivideNumber;
		h_KernelCol[i] = h_KernelRow[i];
	}
	for(i;i<DataSmoothMask;i++)
	{
		KernelMaxValue--;
		h_KernelRow[i] = (float)KernelMaxValue/DivideNumber;
		h_KernelCol[i] = h_KernelRow[i];
	}



	SetConvolutionKernel(h_KernelRow,h_KernelCol,DataSmoothMask);

	CreateGaussianSmoothDataBuffer(W,H);

	float *pSrcD = this->m_GaussianSmoothSrcD;
	float *pDestD = this->m_GaussianSmoothDestD;
	float *BufferD = this->m_GaussianSmoothBufferD;

	cudaMemcpy(pSrcD, pSrc, DataSize * sizeof(float), cudaMemcpyHostToDevice);
	cudaThreadSynchronize();
	//1
	cudaMemcpy(pDestD, pSrcD, DataSize * sizeof(float), cudaMemcpyDeviceToDevice);
	cudaThreadSynchronize();
	DoDataConvolutionRows( BufferD,  pSrcD ,W,H , DataSmoothMask);
	cudaThreadSynchronize();
	DoDataConvolutionColumns(pDestD,BufferD,W,H, DataSmoothMask);
	cudaThreadSynchronize();
	cudaMemcpy(pDest, pDestD, DataSize * sizeof(float), cudaMemcpyDeviceToHost);


	delete[] h_KernelRow;
	delete[] h_KernelCol;


	return true;
#else
	return false;
#endif //#ifndef CUDA_USE
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaContentAwareFilter(int SpaceW, int SpaceH, int SpaceStep, const float *pSrc, const unsigned char *pMask, const unsigned char *GuidedImagePtr, int KerSize, int UseSize, bool FilterSearchOn, int FilterAlphaF, int FilterAlphaS, int FilterAlphaM, int FilterAlphaI, int FilterThresdhold_Outlier, float *pDst, int ThreadNumber, int BlockNumber)//影像融合
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaContentAwareFilter";
	if (CheckPtr3(fnName, pSrc, pMask, GuidedImagePtr) == false)
	{
		return false;
	}

	//GPU運算
	int local_ThreadNumber = ThreadNumber;//192, ThreadNumber 128
	int local_BlockNumber = BlockNumber;//961, BlockNumber 512
	bool bSuccess = false;
	float *CpSrc = NULL;
	float *CpDst = NULL;
	unsigned char *CpMask = NULL;
	unsigned char *CpGuidedImage = NULL;
	const size_t BufferSize = CalcBufferSize(SpaceStep, SpaceH);
	const size_t DeviceDataSize = CalcBufferSize(SpaceStep, SpaceH);
	if (allocBuffer_Cuda(BufferSize, CpSrc, fnName, "CpSrc") == false ||
		allocBuffer_Cuda(BufferSize, CpMask, fnName, "CpMask") == false ||
		allocBuffer_Cuda(BufferSize, CpGuidedImage, fnName, "CpGuidedImage") == false ||
		allocBuffer_Cuda(BufferSize, CpDst, fnName, "CpDst") == false)
	{
		CudaExceptionClean();
		return false;
	}

	bSuccess = checkCudaErrors(cudaMemcpy(CpSrc, pSrc, sizeof(float)*DeviceDataSize, cudaMemcpyHostToDevice));
	if (!bSuccess)
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pSrc To CpSrc. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;
	}
	bSuccess = checkCudaErrors(cudaMemcpy(CpMask, pMask, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyHostToDevice));
	if (!bSuccess)
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pMask To CpMask. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;
	}
	bSuccess = checkCudaErrors(cudaMemcpy(CpGuidedImage, GuidedImagePtr, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyHostToDevice));
	if (!bSuccess)
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy GuidedImagePtr To CpGuidedImage. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;
	}

	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaContentAwareFilter(SpaceW, SpaceH, SpaceStep, CpSrc, CpMask, CpGuidedImage, KerSize, UseSize, FilterSearchOn, FilterAlphaF, FilterAlphaS, FilterAlphaM, FilterAlphaI, FilterThresdhold_Outlier, CpDst, IsUseTexture, local_ThreadNumber, local_BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaContentAwareFilter Error. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;
	}

	bSuccess = checkCudaErrors(cudaMemcpy(pDst, CpDst, sizeof(float)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if (!bSuccess)
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy CpDst To pDst. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;
	}
	cudaThreadSynchronize();

	//Release
	freeBuffer_Cuda(CpSrc);
	freeBuffer_Cuda(CpMask);
	freeBuffer_Cuda(CpGuidedImage);
	freeBuffer_Cuda(CpDst);

	//if (IsUseTexture) { UnLinkTextureSolvePhase(); }
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
void   CCudaFunc::ClearCudeBufferSizeList()//清除Cude記憶體尺寸列表
{
	CCudaFunc::m_BufferSizeList.clear();
}
//--------------------------------------------------------------------//
bool   CCudaFunc::LayoutCudeBufferSizeList()//排列Cude記憶體尺寸列表
{
	std::sort(m_BufferSizeList.begin(), m_BufferSizeList.end());
	return true;
}
//--------------------------------------------------------------------//
void   CCudaFunc::AddCudeBufferSize(size_t size)//新增Cude記憶體尺寸
{
	CCudaFunc::m_BufferSizeList.push_back(size);
}
//--------------------------------------------------------------------//
bool CCudaFunc::TestCudaBuffer(size_t size, size_t Count)//測試Cuda記憶體, 同時也是一次初始化
{
#ifdef  CUDA_USE
	if ( 0 == size )
	{
		::sprintf(this->m_ErrorString, "Error, pBufferD size is 0");
		return false;
	}
	const char fnName[] = "CCudaFunc::TestCudaBuffer";
	size_t   i=0, j=0;
	unsigned char **pBufferD = NULL;
	const size_t BufferSize = size;	
	pBufferD = new unsigned char*[Count];
	if ( NULL == pBufferD ) 
	{
		::sprintf(this->m_ErrorString, "Error, pBufferD = new unsigned char*[Count] Fault");
		return false; 
	}
	for ( i=0; i<Count; i++ )
	{
		if ( allocBuffer_Cuda(BufferSize, pBufferD[i], fnName, "pBufferD") == false )
		{
			for ( j=0; j<i; j++ )
			{	freeBuffer_Cuda(pBufferD[j]);	}
			delete [] pBufferD; pBufferD=NULL;
			CudaExceptionClean();
			return false;
		}	
	}
	for ( j=0; j<Count; j++ )
	{	freeBuffer_Cuda(pBufferD[j]);	}
	delete [] pBufferD; pBufferD=NULL;
#endif//CUDA_USE
	return true;
}
//--------------------------------------------------------------------//
size_t CCudaFunc::CalcFitBufferSize(size_t size)//計算適合的記憶體尺寸
{
	size_t i = 0;
	const size_t BufferSizeCount = CCudaFunc::m_BufferSizeList.size();
	for ( i=0; i<BufferSizeCount; i++ )
	{
		if ( size <= CCudaFunc::m_BufferSizeList[i] )
		{	return CCudaFunc::m_BufferSizeList[i]; }
	}
	return size;
}
//--------------------------------------------------------------------//
CCudaFunc::CudaPtr* CCudaFunc::GetFreeCudaBuffer(size_t size_min)//取得可用的Cuda記憶體   
{
	size_t i = 0;
	CudaPtr   *cuda_ptr = NULL;
	const size_t CudaBufferCount = CCudaFunc::m_CudaBufferList.size();
	for ( i=0; i<CudaBufferCount; i++ )
	{
		cuda_ptr = &(m_CudaBufferList[i]);
		if ( CUDA_MEM_UNLOCK != cuda_ptr->states ) { continue; }
		if ( cuda_ptr->size == size_min  )
		{	return cuda_ptr; }
	}
	return NULL;
}
//--------------------------------------------------------------------//
void  CCudaFunc::AddCudaBufferPtr(size_t size, void *Ptr, const char *fnName, const char *vaName)//新增加cuda記憶體
{
	CudaPtr cuda_ptr(Ptr, size, CUDA_MEM_LOCK, fnName, vaName);
	CCudaFunc::m_CudaBufferList.push_back(cuda_ptr);	
}
//--------------------------------------------------------------------//
void*  CCudaFunc::allocBuffer_Cuda(size_t size, const char *fnName, const char *vaName)//建立cuda記憶體
{
#ifdef  CUDA_USE	
	CudaPtr  *cuda_ptr = NULL;
	const size_t size_min  = CalcFitBufferSize(size);	
	LockCuda();
	cuda_ptr = GetFreeCudaBuffer(size_min);
	if ( NULL != cuda_ptr )
	{
	#ifdef _DEBUG
		cuda_ptr->fnName = fnName;		
		cuda_ptr->vaName = vaName;
	#endif
		cuda_ptr->states = CUDA_MEM_LOCK;		
		UnlockCuda();		
		return cuda_ptr->ptr;
	}

	void *Ptr = NULL;
	if( checkCudaErrors(cudaMalloc((void**) &Ptr, sizeof(unsigned char)*size_min)) == false )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "CudaMalloc %s[%s] Error. %s", fnName, vaName, cudaGetErrorString(err));
		SetCudaExceptionCode(AOI_EXCEPTION_CUDA_MEM_ALLOC);
		UnlockCuda();	
		return NULL;
	}
	AddCudaBufferPtr(size_min, Ptr, fnName, vaName);
	UnlockCuda();
	return Ptr;
#endif//CUDA_USE
	ReturnCudaDisabled();	
	return NULL;
}
//--------------------------------------------------------------------//
bool CCudaFunc::UnlockBuffer_Cuda(void *Ptr)//釋放cuda記憶體
{	
	size_t i;	
	CudaPtr  *cuda_ptr = NULL;
	const size_t BufferCount = (m_CudaBufferList.size());	

	for ( i=0; i<BufferCount; ++i)
	{
		cuda_ptr = &(m_CudaBufferList[i]);
		if ( cuda_ptr->ptr == Ptr )
		{
		#ifdef _DEBUG			
			cuda_ptr->fnName = "";
			cuda_ptr->vaName = "";
		#endif
			cuda_ptr->states = CUDA_MEM_UNLOCK;			
			return true;
		}		
	}	
	::sprintf(this->m_ErrorString, "Error, UnlockBuffer_Cuda Fault");
	SetCudaExceptionCode(AOI_EXCEPTION_CUDA_MEM_FREE);
	return false;
}
//--------------------------------------------------------------------//
bool CCudaFunc::freeBufferKernel_Cuda(void *Ptr)//釋放cuda記憶體
{	
	LockCuda();
	if ( UnlockBuffer_Cuda(Ptr) == false )
	{	cudaFree(Ptr);	}
	UnlockCuda();
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::ReleaseCudaBufferList(bool ShowMsg)//歸還整個Cuda記憶體
{
	size_t    i=0;	
	CudaPtr  *cuda_ptr = NULL;
	const size_t BufferCount = (m_CudaBufferList.size());	
	LockCuda();
	for ( i=0; i<BufferCount; ++i)
	{
		cuda_ptr = &(m_CudaBufferList[i]);
		if ( NULL == cuda_ptr->ptr ) { continue; }

		if ( true==ShowMsg && CUDA_MEM_LOCK==cuda_ptr->states )
		{
			::sprintf(m_ErrorString, ("[CCudaFunc::MemoryLeak] ID:%d, size:%d bytes\n"), i+1, cuda_ptr->size);	
			SetCudaExceptionCode(AOI_EXCEPTION_CUDA_MEM_FREE);
			::AfxMessageBox(CString(m_ErrorString));
		}
	#ifdef _DEBUG			
		cuda_ptr->fnName = "";
		cuda_ptr->vaName = "";
	#endif

		cudaFree(cuda_ptr->ptr);
		cuda_ptr->ptr = NULL;
		cuda_ptr->states = CUDA_MEM_NULL;
		cuda_ptr->size = 0;		
	}
	m_CudaBufferList.clear();
	UnlockCuda();
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::SaveCudaBufferList(LPCTSTR filename)//輸出文件
{
	if ( SaveCudaBufferListFn(filename) == false )
	{
		SetCudaExceptionCode_FileWrite();
		return false;
	}
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::SaveCudaBufferListFn(LPCTSTR filename)//輸出文件
{
	FILE *pfile = NULL;
	pfile = ::_tfopen(filename, _T("w+"));
	if ( NULL == pfile )
	{
		::sprintf(this->m_ErrorString, "Error, Save Cuda Buffer List File Fault");
		return false;
	}
	CCudaFunc::output_Cuda(pfile);
	::fclose(pfile); pfile = NULL;
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::output_Cuda(FILE* fp)//輸出文件
{
	size_t    i=0; 	
	size_t    TotalSize=0;
	CudaPtr  *cuda_ptr = NULL;
	const size_t BufferCount = (m_CudaBufferList.size());		

#ifdef _DEBUG
	::fprintf(fp , ("%4s, %8s, %16s, %16s, %64s, %64s\n"), "Idx", "address", "size", "locked", "function name", "variable name");
#else
	::fprintf(fp , ("%4s, %8s, %16s, %16s\n"), "Idx", "address", "size", "locked");
#endif

	for( i=0; i<BufferCount; ++i)
	{		
		cuda_ptr = &(m_CudaBufferList[i]);
	#ifdef _DEBUG
		if ( cuda_ptr->states == CUDA_MEM_LOCK )
		{	::fprintf(fp, ("%4zd: %p, %16zu, %16s, %64s, %64s\n"), i+1, cuda_ptr->ptr, cuda_ptr->size, "Locked", cuda_ptr->fnName.c_str(), cuda_ptr->vaName.c_str());	}
		else
		{	::fprintf(fp, ("%4zd: %p, %16zu\n"), i+1, cuda_ptr->ptr, cuda_ptr->size);	}
	#else
		if ( cuda_ptr->states == CUDA_MEM_LOCK )
		{	::fprintf(fp, ("%4zd: %p, %16zu, %16s\n"), i+1, cuda_ptr->ptr, cuda_ptr->size, "Locked");	}
		else
		{	::fprintf(fp, ("%4zd: %p, %16zu\n"), i+1, cuda_ptr->ptr, cuda_ptr->size);	}
	#endif
		TotalSize += cuda_ptr->size;
	} 	
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::allocBuffer_Cuda(size_t size, float *&Ptr, const char *fnName, const char *vaName)//建立cuda記憶體
{
	Ptr = (float*)allocBuffer_Cuda(size*sizeof(float), fnName, vaName);
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::allocBuffer_Cuda(size_t size, short *&Ptr, const char *fnName, const char *vaName)//建立cuda記憶體
{
	Ptr = (short*)allocBuffer_Cuda(size*sizeof(short), fnName, vaName);
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::allocBuffer_Cuda(size_t size, unsigned char *&Ptr, const char *fnName, const char *vaName)//建立cuda記憶體
{
	Ptr = (unsigned char*)allocBuffer_Cuda(size*sizeof(unsigned char), fnName, vaName);
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::allocBuffer_Cuda(size_t size, TCastParam *&Ptr, const char *fnName, const char *vaName)//建立cuda記憶體
{
	Ptr = (TCastParam*)allocBuffer_Cuda(size*sizeof(TCastParam), fnName, vaName);
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::freeBuffer_Cuda(float *&Ptr)//釋放cuda記憶體
{
	if ( NULL == Ptr ) { return true; }
	CCudaFunc::freeBufferKernel_Cuda(Ptr);
	Ptr = NULL;
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::freeBuffer_Cuda(short *&Ptr)//釋放cuda記憶體
{
	if ( NULL == Ptr ) { return true; }
	freeBufferKernel_Cuda(Ptr);
	Ptr = NULL;
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::freeBuffer_Cuda(unsigned char *&Ptr)//釋放cuda記憶體
{
	if ( NULL == Ptr ) { return true; }
	freeBufferKernel_Cuda(Ptr);
	Ptr = NULL;
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::freeBuffer_Cuda(TCastParam *&Ptr)//釋放cuda記憶體
{
	if ( NULL == Ptr ) { return true; }
	freeBufferKernel_Cuda(Ptr);
	Ptr = NULL;
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaMedianFilter_Internal(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *pMaskD, const TPhaseNoiseParam &NoiseParam, float *pSpaceD, const bool IsUseTexture, const int ThreadNumber, const int BlockNumber)//中值濾波器
{
	const int nCastFilterMode = NoiseParam.SpaceCastFilterMode;
	const int nMedianFilterSize = NoiseParam.SpaceCastMedianFilterSize;
	if (nCastFilterMode == CAST_SPACE_FILTER_MODE_DEFAULT) {
		if (nMedianFilterSize <= 1) { return true; }
		if (nMedianFilterSize > 99) { return true; }
	}

	bool bSuccess = false;
	const char fnName[] = "CCudaFunc::CudaMedianFilter_Internal";

	float *pMedianD = NULL;//中值濾波	
	const int KerSize = ((nMedianFilterSize / 2) * 2) + 1;
	const int UseSize = NoiseParam.SpaceCastMedianFilterUseSize;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);
	if (allocBuffer_Cuda(BufferSize, pMedianD, fnName, "pMedianD") == false)
	{
		return false;
	}

	cudaThreadSynchronize();
	//ExecCudaMedianFilter(ImageW, ImageH, ImageStep, pSpaceD, pMaskD, KerSize, UseSize, pMedianD, IsUseTexture, ThreadNumber, BlockNumber);		
	ExecCudaSpaceFilter_Joe(ImageW, ImageH, ImageStep, pSpaceD, pMaskD, nCastFilterMode, KerSize, UseSize, pMedianD, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		//::sprintf(m_ErrorString, "Error, ExecCudaMedianFilter Error. %s", cudaGetErrorString(err));
		::sprintf(m_ErrorString, "Error, ExecCudaSpaceFilter_Joe Error. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_ExeFunc();
		return false;
	}

	bSuccess = checkCudaErrors(cudaMemcpy(pSpaceD, pMedianD, sizeof(float)*DeviceDataSize, cudaMemcpyDeviceToDevice));
	if (!bSuccess)
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMedianD To pSpaceD. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		return false;
	}
	freeBuffer_Cuda(pMedianD);
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSetBasePhaseData(short *pBasePhasePlane, const int DataSize, const int LightSide, const int FrequenceMode)
{
#ifdef  CUDA_USE
	if( pBasePhasePlane == NULL )
	{
		::sprintf(m_ErrorString, "pBasePhase == NULL");
		return false;
	}
	if( CreateBasePhasePlaneBuffer(DataSize, LightSide, FrequenceMode) == false )	{ return false; }

	short *pBasePhaseD = NULL;
	GetBasePhasePlaneDevicePtr(pBasePhaseD, LightSide, FrequenceMode);

	if( pBasePhaseD == NULL )
	{
		::sprintf(m_ErrorString, "pBasePhaseD = NULL");
		return false;
	}

	bool bSuccess = false;
	bSuccess = checkCudaErrors(cudaMemcpy(pBasePhaseD, pBasePhasePlane, sizeof(short) * DataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pBasePhasePlane To pBasePhaseD. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

#endif
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSetFFCBaseData(float *pGain, unsigned char *pBase, const int DataSize, const int LightSide)
{
#ifdef  CUDA_USE
	if( pGain == NULL )
	{
		::sprintf(m_ErrorString, "pGain == NULL");
		return false;
	}	
	if( pBase == NULL )
	{
		::sprintf(m_ErrorString, "pBase == NULL");
		return false;
	}

	if( CreateFFCBaseBuffer(DataSize, LightSide) == false )	{ return false; }

	float *pGainD = NULL;
	unsigned char *pBaseD = NULL;
	GetFFCBaseBufferDevicePtr( pGainD, pBaseD, LightSide);

	if( pGainD == NULL )
	{
		::sprintf(m_ErrorString, "pGainD = NULL");
		return false;
	}
	if( pBaseD == NULL )
	{
		::sprintf(m_ErrorString, "pBaseD = NULL");
		return false;
	}

	bool bSuccess = false;
	bSuccess = checkCudaErrors(cudaMemcpy(pGainD, pGain, sizeof(float) * DataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pGain To pGainD. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pBaseD, pBase, sizeof(unsigned char) * DataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pBase To pBaseD. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

#endif
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolvePhase_Three(const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pImage3, unsigned char *pMergeImage, short *pPhaseData, unsigned char *pMaskMap, const int W, const int H, int ImageSmoothMaskSize, bool IsUseFFC, bool IsUseBasePhase, const int LightSide, const int FrequenceMode, const int ThreadNO)
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;

	if( pImage1 == NULL )
	{
		::sprintf(m_ErrorString, "pImage1 = NULL");
		return false;
	}
	if( pImage2 == NULL )
	{
		::sprintf(m_ErrorString, "pImage2 = NULL");
		return false;
	}	
	if( pImage3 == NULL )
	{
		::sprintf(m_ErrorString, "pImage3 = NULL");
		return false;
	}
	if( pPhaseData == NULL )
	{
		::sprintf(m_ErrorString, "pPhaseData = NULL");
		return false;
	}
	if( pMaskMap == NULL )
	{
		::sprintf(m_ErrorString, "pMaskMap == NULL");
		return false;
	}
	//Kai-20160727
	/*
	int SaturateValue = SPIDataCollect.m_SystemParameter.m_SystemParam.m_SaturationValue;
	int LowContrastValue = SPIDataCollect.m_SystemParameter.m_SystemParam.m_LowContrastValue;
	int PotentialValue = SPIDataCollect.m_SystemParameter.m_SystemParam.m_PotentialValue;
	bool IsUsePotentialValue = SPIDataCollect.m_SystemParameter.m_SystemParam.m_PotentialValueCheck;
	//GPU運算
	int nStep = 3;
	if( CreateSolvePhaseDataBuffer(nStep, W, H, ThreadNO) == false) { return false; }

	unsigned char *ImgD1 = NULL;
	unsigned char *ImgD2 = NULL;
	unsigned char *ImgD3 = NULL;
	unsigned char *ImgD4 = NULL;
	unsigned char *ImgD5 = NULL;
	unsigned char *pMergeD = NULL;
	short *pPhaseD = NULL;
	unsigned char *pMaskD = NULL;
	GetSolvePhaseDataDevicePtr(ImgD1, ImgD2, ImgD3, ImgD4, ImgD5, pMergeD, pPhaseD, pMaskD, ThreadNO);
	int DeviceDataSize = W * H;

	bool bSuccess = false;
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD1, pImage1, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage1 To ImgD1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD2, pImage2, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage2 To ImgD2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD3, pImage3, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage3 To ImgD3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
		

	int ThreadNumber = 192;
	int BlockNumber = 961;
	cudaThreadSynchronize();
	//Hostfunction
	if( IsUseFFC )
	{
		//short
		float *pFFCGainD = NULL;
		unsigned char *pFFCBaseD = NULL;
		GetFFCBaseBufferDevicePtr( pFFCGainD, pFFCBaseD, LightSide);
		if( pFFCGainD == NULL )
		{
			::sprintf(m_ErrorString, "pFFCGain == NULL");
			return false;
		}
		if( pFFCBaseD == NULL )
		{
			::sprintf(m_ErrorString, "pFFCBaseD == NULL");
			return false;
		}
		DoCudaSolvePhaseAndFFC_Three(ImgD1, ImgD2, ImgD3, pMergeD, pPhaseD, pMaskD, pFFCGainD, pFFCBaseD, DeviceDataSize, SaturateValue, LowContrastValue, PotentialValue, IsUsePotentialValue, IsUseTexture, ThreadNumber, BlockNumber);
	}
	else
	{
		DoCudaSolvePhase_Three(ImgD1, ImgD2, ImgD3, pMergeD, pPhaseD, pMaskD, DeviceDataSize, SaturateValue, LowContrastValue, PotentialValue, IsUsePotentialValue, IsUseTexture, ThreadNumber, BlockNumber);
	}
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "DoCudaSolvePhase Error. %s", cudaGetErrorString(err));			
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}


	//PhaseSmooth
	if( SPIDataCollect.m_SystemParameter.m_SystemParam.m_CudaPhaseSmooth == true )
	{
		int MaskSize = SPIDataCollect.m_SystemParameter.m_SystemParam.m_CudaPhaseSmoothMaskSize;
		if( CreatePhaseSmoothDataBuffer(W, H, ThreadNO) == false) { return false; }

		short *pPhaseSmoothSrcD = NULL;
		short *pPhaseSmoothDestD = NULL;
		float *pPhaseSmoothTempSinBufferD = NULL;
		float *pPhaseSmoothTempCosBufferD = NULL;
		unsigned char *pPhaseSmoothTempMaskBufferD = NULL;
		unsigned char *pPhaseSmoothMaskD = NULL;

		pPhaseSmoothSrcD = pPhaseD;
		pPhaseSmoothMaskD = pMaskD; 
		GetPhaseSmoothDataPtr(pPhaseSmoothDestD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ThreadNO);

		//先將原始資料複製一份到目標位置
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short) * DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}

		//
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, W, H, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, W, H, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		
		pPhaseD = pPhaseSmoothDestD;
	}

	if( IsUseBasePhase )//相位相減
	{
		short *pBasePhaseD = NULL;
		GetBasePhasePlaneDevicePtr( pBasePhaseD, LightSide, FrequenceMode);
		if( pBasePhaseD == NULL )
		{
			::sprintf(m_ErrorString, "pBasePhase == NULL");
			return false;
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaSubtractBasePhasePlane(pPhaseD, pBasePhaseD, DeviceDataSize, IsUseTexture);
		cudaThreadSynchronize();//等到device執行完
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "DoCudaSubtractBasePhasePlane Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
	}

	if( pMergeImage != NULL )
	{
		bSuccess = checkCudaErrors(cudaMemcpy(pMergeImage, pMergeD, sizeof(unsigned char) * DeviceDataSize, cudaMemcpyDeviceToHost));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "cudaMemcpy pMergerD To pMergeImage. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseData, pPhaseD, sizeof(short) * DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskMap, pMaskD, sizeof(unsigned char) * DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();


	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
	*/
#endif
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolvePhase_Four(const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pImage3, const unsigned char *pImage4, unsigned char *pMergeImage, short *pPhaseData, unsigned char *pMaskMap, const int W, const int H, int ImageSmoothMaskSize, bool IsUseFFC, bool IsUseBasePhase, const int LightSide, const int FrequenceMode, const int ThreadNO)
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	if ( CheckPtr6("CCudaFunc::CudaSolvePhase_Four", pImage1, pImage2, pImage3, pImage4, pPhaseData, pMaskMap) == false )
	{	return false; }
	
	//Kai-20160727	
	TPhaseNoiseParam NoiseParam;
	bool IsUsePotentialValue = false;
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	int SaturateValue = SysParam.m_PhaseNoiseOverSaturatedA;
	int LowContrastValue = SysParam.m_PhaseNoiseLowContrastA;
	int PotentialValue = SysParam.m_PhaseNoiseLowPotentialA;	
	AOIDataCollect.GetPhaseNoiseDefineParam(NoiseParam);
	if ( 0 == (NoiseParam.PhaseNoiseDef&PHASE_NOSIE_LOW_POTENTIAL) )
	{	IsUsePotentialValue = false; }
	else
	{	IsUsePotentialValue = true; }
	
	//GPU運算
	const int nStep = 4;
	unsigned char *ImgD1 = NULL;
	unsigned char *ImgD2 = NULL;
	unsigned char *ImgD3 = NULL;
	unsigned char *ImgD4 = NULL;
	unsigned char *ImgD5 = NULL;
	unsigned char *pMergeD = NULL;
	short *pPhaseD = NULL;
	unsigned char *pMaskD = NULL;
	const int ImgWH = W*H;

	//Kai-20160927
	if( CreateSolvePhaseDataBuffer(nStep, W, H, ThreadNO) == false) { return false; }
	GetSolvePhaseDataDevicePtr(ImgD1, ImgD2, ImgD3, ImgD4, ImgD5, pMergeD, pPhaseD, pMaskD, ThreadNO);

//	const char fnName[] = "CCudaFunc::CudaSolvePhase_Four";
//	if ( allocBuffer_Cuda(ImgWH, ImgD1, fnName, "ImgD1") == false ||
//		 allocBuffer_Cuda(ImgWH, ImgD2, fnName, "ImgD2") == false ||
//		 allocBuffer_Cuda(ImgWH, ImgD3, fnName, "ImgD3") == false ||
//		 allocBuffer_Cuda(ImgWH, ImgD4, fnName, "ImgD4") == false ||
//		 allocBuffer_Cuda(ImgWH, pMergeD, fnName, "pMergeD") == false ||
//		 allocBuffer_Cuda(ImgWH, pMaskD, fnName, "pMaskD") == false ||
//		 allocBuffer_Cuda(ImgWH, pPhaseD, fnName, "pPhaseD") == false )
//	{
//		freeBuffer_Cuda(ImgD1);
//		freeBuffer_Cuda(ImgD2);
//		freeBuffer_Cuda(ImgD3);
//		freeBuffer_Cuda(ImgD4);
//		freeBuffer_Cuda(pMergeD);
//		freeBuffer_Cuda(pMaskD);
//		freeBuffer_Cuda(pPhaseD);
//		return false;
//	}	 

	int DeviceDataSize = W * H;
	bool bSuccess = false;
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD1, pImage1, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage1 To ImgD1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD2, pImage2, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage2 To ImgD2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD3, pImage3, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage3 To ImgD3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD4, pImage4, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage4 To ImgD4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	
	int ThreadNumber = 192;
	int BlockNumber = 961;	

	ThreadNumber = 192;
	BlockNumber = 961;
	cudaThreadSynchronize();
	//Hostfunction
	if( IsUseFFC )
	{
		//short
		float *pFFCGainD = NULL;
		unsigned char *pFFCBaseD = NULL;
		GetFFCBaseBufferDevicePtr( pFFCGainD, pFFCBaseD, LightSide);
		if( pFFCGainD == NULL )
		{
			::sprintf(m_ErrorString, "pFFCGain == NULL");
			return false;
		}
		if( pFFCBaseD == NULL )
		{
			::sprintf(m_ErrorString, "pFFCBaseD == NULL");
			return false;
		}
		DoCudaSolvePhaseAndFFC_Four(ImgD1, ImgD2, ImgD3, ImgD4, pMergeD, pPhaseD, pMaskD, pFFCGainD, pFFCBaseD, DeviceDataSize, SaturateValue, LowContrastValue, PotentialValue, IsUsePotentialValue, IsUseTexture, ThreadNumber, BlockNumber);
	}
	else
	{
		DoCudaSolvePhase_Four(ImgD1, ImgD2, ImgD3, ImgD4, pMergeD, pPhaseD, pMaskD, DeviceDataSize, SaturateValue, LowContrastValue, PotentialValue, IsUsePotentialValue, IsUseTexture, ThreadNumber, BlockNumber);		
	}
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "DoCudaSolvePhase Error. %s", cudaGetErrorString(err));			
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}
	
	//PhaseSmooth
	if( SysParam.m_PhaseSmoothCudaMode == FN_ENABLE )
	{
		int MaskSize = SysParam.m_PhaseSmoothCudaMaskSize;
		if( CreatePhaseSmoothDataBuffer(W, H, ThreadNO) == false) { return false; }

		short *pPhaseSmoothSrcD = NULL;
		short *pPhaseSmoothDestD = NULL;
		float *pPhaseSmoothTempSinBufferD = NULL;
		float *pPhaseSmoothTempCosBufferD = NULL;
		unsigned char *pPhaseSmoothTempMaskBufferD = NULL;
		unsigned char *pPhaseSmoothMaskD = NULL;

		pPhaseSmoothSrcD = pPhaseD;
		pPhaseSmoothMaskD = pMaskD; 
		GetPhaseSmoothDataPtr(pPhaseSmoothDestD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ThreadNO);

		//先將原始資料複製一份到目標位置
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short) * DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}

		//
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, W, H, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, W, H, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		
		pPhaseD = pPhaseSmoothDestD;
	}

	if( IsUseBasePhase )//相位相減
	{
		short *pBasePhaseD = NULL;
		GetBasePhasePlaneDevicePtr( pBasePhaseD, LightSide, FrequenceMode);
		if( pBasePhaseD == NULL )
		{
			::sprintf(m_ErrorString, "pBasePhase == NULL");
			return false;
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaSubtractBasePhasePlane(pPhaseD, pBasePhaseD, DeviceDataSize, IsUseTexture);
		cudaThreadSynchronize();//等到device執行完
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "DoCudaSubtractBasePhasePlane Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
	}

	if( pMergeImage != NULL )
	{
		bSuccess = checkCudaErrors(cudaMemcpy(pMergeImage, pMergeD, sizeof(unsigned char) * DeviceDataSize, cudaMemcpyDeviceToHost));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "cudaMemcpy pMergerD To pMergeImage. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseData, pPhaseD, sizeof(short) * DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskMap, pMaskD, sizeof(unsigned char) * DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();

//	freeBuffer_Cuda(ImgD1);
//	freeBuffer_Cuda(ImgD2);
//	freeBuffer_Cuda(ImgD3);
//	freeBuffer_Cuda(ImgD4);
//	freeBuffer_Cuda(pMergeD);
//	freeBuffer_Cuda(pMaskD);
//	freeBuffer_Cuda(pPhaseD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
#endif
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolvePhase_Five(const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pImage3, const unsigned char *pImage4, const unsigned char *pImage5, unsigned char *pMergeImage, short *pPhaseData, unsigned char *pMaskMap, const int W, const int H, int ImageSmoothMaskSize, bool IsUseFFC, bool IsUseBasePhase, const int LightSide, const int FrequenceMode, const int ThreadNO)
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;

	if( pImage1 == NULL )
	{
		::sprintf(m_ErrorString, "pImage1 = NULL");
		return false;
	}
	if( pImage2 == NULL )
	{
		::sprintf(m_ErrorString, "pImage2 = NULL");
		return false;
	}	
	if( pImage3 == NULL )
	{
		::sprintf(m_ErrorString, "pImage3 = NULL");
		return false;
	}
	if( pImage4 == NULL )
	{
		::sprintf(m_ErrorString, "pImage4 = NULL");
		return false;
	}
	if( pImage5 == NULL )
	{
		::sprintf(m_ErrorString, "pImage5 = NULL");
		return false;
	}
	if( pPhaseData == NULL )
	{
		::sprintf(m_ErrorString, "pPhaseData = NULL");
		return false;
	}
	if( pMaskMap == NULL )
	{
		::sprintf(m_ErrorString, "pMaskMap == NULL");
		return false;
	}

	//Kai-20160727
	/*
	int SaturateValue = SPIDataCollect.m_SystemParameter.m_SystemParam.m_SaturationValue;
	int LowContrastValue = SPIDataCollect.m_SystemParameter.m_SystemParam.m_LowContrastValue;
	int PotentialValue = SPIDataCollect.m_SystemParameter.m_SystemParam.m_PotentialValue;
	bool IsUsePotentialValue = SPIDataCollect.m_SystemParameter.m_SystemParam.m_PotentialValueCheck;
	//GPU運算
	int nStep = 5;
	if( CreateSolvePhaseDataBuffer(nStep, W, H, ThreadNO) == false) { return false; }

	unsigned char *ImgD1 = NULL;
	unsigned char *ImgD2 = NULL;
	unsigned char *ImgD3 = NULL;
	unsigned char *ImgD4 = NULL;
	unsigned char *ImgD5 = NULL;
	unsigned char *pMergeD = NULL;
	short *pPhaseD = NULL;
	unsigned char *pMaskD = NULL;
	GetSolvePhaseDataDevicePtr(ImgD1, ImgD2, ImgD3, ImgD4, ImgD5, pMergeD, pPhaseD, pMaskD, ThreadNO);
	int DeviceDataSize = W * H;

	bool bSuccess = false;
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD1, pImage1, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage1 To ImgD1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD2, pImage2, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage2 To ImgD2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD3, pImage3, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage3 To ImgD3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD4, pImage4, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage4 To ImgD4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD5, pImage5, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage5 To ImgD5. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}


	int ThreadNumber = 192;
	int BlockNumber = 961;
	cudaThreadSynchronize();
	//Hostfunction
	if( IsUseFFC )
	{
		//short
		float *pFFCGainD = NULL;
		unsigned char *pFFCBaseD = NULL;
		GetFFCBaseBufferDevicePtr( pFFCGainD, pFFCBaseD, LightSide);
		if( pFFCGainD == NULL )
		{
			::sprintf(m_ErrorString, "pFFCGain == NULL");
			return false;
		}
		if( pFFCBaseD == NULL )
		{
			::sprintf(m_ErrorString, "pFFCBaseD == NULL");
			return false;
		}
		DoCudaSolvePhaseAndFFC_Five(ImgD1, ImgD2, ImgD3, ImgD4, ImgD5, pMergeD, pPhaseD, pMaskD, pFFCGainD, pFFCBaseD, DeviceDataSize, SaturateValue, LowContrastValue, PotentialValue, IsUsePotentialValue, IsUseTexture, ThreadNumber, BlockNumber);
	}
	else
	{
		DoCudaSolvePhase_Five(ImgD1, ImgD2, ImgD3, ImgD4, ImgD5, pMergeD, pPhaseD, pMaskD, DeviceDataSize, SaturateValue, LowContrastValue, PotentialValue, IsUsePotentialValue, IsUseTexture, ThreadNumber, BlockNumber);
	}
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "DoCudaSolvePhase Error. %s", cudaGetErrorString(err));		
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}


	//PhaseSmooth
	if( SPIDataCollect.m_SystemParameter.m_SystemParam.m_CudaPhaseSmooth == true )
	{
		int MaskSize = SPIDataCollect.m_SystemParameter.m_SystemParam.m_CudaPhaseSmoothMaskSize;
		if( CreatePhaseSmoothDataBuffer(W, H, ThreadNO) == false) { return false; }

		short *pPhaseSmoothSrcD = NULL;
		short *pPhaseSmoothDestD = NULL;
		float *pPhaseSmoothTempSinBufferD = NULL;
		float *pPhaseSmoothTempCosBufferD = NULL;
		unsigned char *pPhaseSmoothTempMaskBufferD = NULL;
		unsigned char *pPhaseSmoothMaskD = NULL;

		pPhaseSmoothSrcD = pPhaseD;
		pPhaseSmoothMaskD = pMaskD; 
		GetPhaseSmoothDataPtr(pPhaseSmoothDestD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ThreadNO);

		//先將原始資料複製一份到目標位置
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short) * DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}

		//
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, W, H, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, W, H, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		
		pPhaseD = pPhaseSmoothDestD;
	}

	if( IsUseBasePhase )//相位相減
	{
		short *pBasePhaseD = NULL;
		GetBasePhasePlaneDevicePtr( pBasePhaseD, LightSide, FrequenceMode);
		if( pBasePhaseD == NULL )
		{
			::sprintf(m_ErrorString, "pBasePhase == NULL");
			return false;
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaSubtractBasePhasePlane(pPhaseD, pBasePhaseD, DeviceDataSize, IsUseTexture);
		cudaThreadSynchronize();//等到device執行完
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "DoCudaSubtractBasePhasePlane Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
	}

	if( pMergeImage != NULL )
	{
		bSuccess = checkCudaErrors(cudaMemcpy(pMergeImage, pMergeD, sizeof(unsigned char) * DeviceDataSize, cudaMemcpyDeviceToHost));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "cudaMemcpy pMergerD To pMergeImage. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseData, pPhaseD, sizeof(short) * DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskMap, pMaskD, sizeof(unsigned char) * DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
	*/
#endif
	return true;
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaPhaseSmooth(short *pSrcPhase, short *pDestPhase, unsigned char *pMaskMap, const int W, const int H, const int MaskSize, const int ThreadNO)
{
#ifdef  CUDA_USE
	if( pSrcPhase == NULL )
	{
		::sprintf(m_ErrorString, "pSrcPhase = NULL");
		return false;
	}
	if( pDestPhase == NULL )
	{
		::sprintf(m_ErrorString, "pDestPhase = NULL");
		return false;
	}
	if( pMaskMap == NULL )
	{
		::sprintf(m_ErrorString, "pMaskMap = NULL");
		return false;
	}

	//Kai-20160727
	/*
	int nStep = 4;
	nStep = SPIDataCollect.GetNGrattingShiftStep();	//chia 1040422
	if( CreateSolvePhaseDataBuffer(nStep, W, H, ThreadNO) == false) { return false; }

	unsigned char *ImgD1 = NULL;
	unsigned char *ImgD2 = NULL;
	unsigned char *ImgD3 = NULL;
	unsigned char *ImgD4 = NULL;
	unsigned char *ImgD5 = NULL;
	unsigned char *pMergeD = NULL;
	short *pPhaseD = NULL;
	unsigned char *pMaskD = NULL;
	GetSolvePhaseDataDevicePtr(ImgD1, ImgD2, ImgD3, ImgD4, ImgD5, pMergeD, pPhaseD, pMaskD, ThreadNO);
	int DeviceDataSize = W * H;

	bool bSuccess = false;
	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseD, pSrcPhase, sizeof(short) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pSrcPhase To pPhaseD. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskD, pMaskMap, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pSrcPhase To pPhaseD. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();

	if( CreatePhaseSmoothDataBuffer(W, H, ThreadNO) == false) { return false; }

	short *pPhaseSmoothSrcD = NULL;
	short *pPhaseSmoothDestD = NULL;
	float *pPhaseSmoothTempSinBufferD = NULL;
	float *pPhaseSmoothTempCosBufferD = NULL;
	unsigned char *pPhaseSmoothTempMaskBufferD = NULL;
	unsigned char *pPhaseSmoothMaskD = NULL;

	pPhaseSmoothSrcD = pPhaseD;
	pPhaseSmoothMaskD = pMaskD; 
	GetPhaseSmoothDataPtr(pPhaseSmoothDestD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ThreadNO);

	//先將原始資料複製一份到目標位置
	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short) * DeviceDataSize, cudaMemcpyDeviceToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();

	DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, W, H, MaskSize, false);		//ROW
	cudaThreadSynchronize();
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));			
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}
	DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, W, H, MaskSize, false);	//COL
	cudaThreadSynchronize();
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));	
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();


	bSuccess = checkCudaErrors(cudaMemcpy(pDestPhase, pPhaseSmoothDestD, sizeof(short) * DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskMap, pPhaseSmoothMaskD, sizeof(unsigned char) * DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

	cudaThreadSynchronize();
	*/
#endif
	return true;
}
//--------------------------------------------------------------------//
//Kai-20160727
/*
bool CCudaFunc::CudaSolveMultiPhase_Four(PBlockInfoST pBlockInfo, const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pImage3, const unsigned char *pImage4
		, const unsigned char *pImage1_F2, const unsigned char *pImage2_F2, const unsigned char *pImage3_F2, const unsigned char *pImage4_F2
		, int ImageW, int ImageH, bool IsColor, unsigned char *&p2DImage, short *&pPhaseData, unsigned char *&pMask, int ThreadNO)
{//chia 1041130

#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;

	if( pImage1 == NULL ) { ::sprintf(m_ErrorString, "pImage1 = NULL"); return false; }
	if( pImage2 == NULL ) { ::sprintf(m_ErrorString, "pImage2 = NULL"); return false; }
	if( pImage3 == NULL ) { ::sprintf(m_ErrorString, "pImage3 = NULL"); return false; }
	if( pImage4 == NULL ) { ::sprintf(m_ErrorString, "pImage4 = NULL"); return false; }
	if( pImage1_F2 == NULL ) { ::sprintf(m_ErrorString, "pImage1_F2 = NULL"); return false; }
	if( pImage2_F2 == NULL ) { ::sprintf(m_ErrorString, "pImage2_F2 = NULL"); return false; }
	if( pImage3_F2 == NULL ) { ::sprintf(m_ErrorString, "pImage3_F2 = NULL"); return false; }
	if( pImage4_F2 == NULL ) { ::sprintf(m_ErrorString, "pImage4_F2 = NULL"); return false; }
	if( pPhaseData == NULL ) { ::sprintf(m_ErrorString, "pPhaseData = NULL"); return false; }
	if( p2DImage == NULL ) { ::sprintf(m_ErrorString, "p2DImage = NULL"); return false; }
	if( pMask == NULL ) { ::sprintf(m_ErrorString, "pMaskMap = NULL"); return false; }

	
	int W=ImageW, H=ImageH;
	int SaturateValue = SPIDataCollect.m_SystemParameter.m_SystemParam.m_SaturationValue;
	int LowContrastValue = SPIDataCollect.m_SystemParameter.m_SystemParam.m_LowContrastValue;
	int PotentialValue = SPIDataCollect.m_SystemParameter.m_SystemParam.m_PotentialValue;
	bool IsUsePotentialValue = SPIDataCollect.m_SystemParameter.m_SystemParam.m_PotentialValueCheck;
	bool IsUseBasePhase = SPIDataCollect.m_SystemParameter.m_SystemParam.m_SubstraceBasePhase;

	//GPU運算
	int nStep = 4;
	if( CreateSolvePhaseDataBuffer(nStep, W, H, ThreadNO) == false) { return false; }

	unsigned char *ImgD1 = NULL, *ImgD2 = NULL, *ImgD3 = NULL, *ImgD4 = NULL, *ImgD5 = NULL;
	unsigned char *pMergeD=NULL, *pMergeD_F2=NULL;
	short *pPhaseD=NULL, *pPhaseD_F2=NULL;
	unsigned char *pMaskD=NULL, *pMaskD_F2=NULL;
	short *pBasePhaseD = NULL, *pBasePhaseD_F2;
	GetSolvePhaseImageDevicePtr(ImgD1, ImgD2, ImgD3, ImgD4, ImgD5, ThreadNO);
	GetSolveF1PhaseDataDevicePtr(pMergeD, pPhaseD, pMaskD, ThreadNO);
	GetSolveF1PhaseDataDevicePtr(pMergeD, pPhaseD, pMaskD, ThreadNO);
	int DeviceDataSize = W * H;

	bool bSuccess = false;
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD1, pImage1, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage1 To ImgD1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD2, pImage2, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage2 To ImgD2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD3, pImage3, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage3 To ImgD3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD4, pImage4, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage4 To ImgD4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

	
	int FrequenceMode = pBlockInfo->sMF_FrequencyMode;
	int FrequenceMode1=FrequenceMode;
	int FrequenceMode2=FrequenceMode;
	int LightSide = SPIDataCollect.GetBlockLightMode(pBlockInfo->sBlockType);
	if( FrequenceMode == FREQUENCE_MODE_MF )
	{
		FrequenceMode1 = FREQUENCE_MODE_P1;
		FrequenceMode2 = FREQUENCE_MODE_P2;
	}
	else
	{
		FrequenceMode1 = FrequenceMode;
		FrequenceMode2 = FrequenceMode1;
	}
	
//先計算第一相位
	
	if( IsUseBasePhase )//基本相位
	{
		GetBasePhasePlaneDevicePtr( pBasePhaseD, LightSide, FrequenceMode1);
		if( pBasePhaseD == NULL )
		{
			::sprintf(m_ErrorString, "pBasePhase == NULL");
			return false;
		}
	}
	
	int ThreadNumber = GPU_BLOCK_SIZE;
	int BlockNumber = (DeviceDataSize/GPU_BLOCK_SIZE)+1;	
	cudaThreadSynchronize();
	//if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
	DoCudaSolvePhaseAndBasePhase_Four(ImgD1, ImgD2, ImgD3, ImgD4, pMergeD, pPhaseD, pMaskD, pBasePhaseD, DeviceDataSize, SaturateValue, LowContrastValue, PotentialValue, IsUsePotentialValue, IsUseTexture, ThreadNumber, BlockNumber);		
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "DoCudaSolvePhase Error. %s", cudaGetErrorString(err));			
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}

//算第二相位	bSuccess = false;
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD1, pImage1_F2, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage1 To ImgD1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD2, pImage2_F2, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage2 To ImgD2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD3, pImage3_F2, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage3 To ImgD3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD4, pImage4_F2, sizeof(unsigned char) * DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage4 To ImgD4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	if( IsUseBasePhase )//基本相位
	{
		GetBasePhasePlaneDevicePtr( pBasePhaseD_F2, LightSide, FrequenceMode2);
		if( pBasePhaseD_F2 == NULL )
		{
			::sprintf(m_ErrorString, "pBasePhase == NULL");
			return false;
		}
	}	
	//if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
	DoCudaSolvePhaseAndBasePhase_Four(ImgD1, ImgD2, ImgD3, ImgD4, pMergeD_F2, pPhaseD_F2, pMaskD_F2, pBasePhaseD_F2, DeviceDataSize, SaturateValue, LowContrastValue, PotentialValue, IsUsePotentialValue, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "DoCudaSolvePhase Error. %s", cudaGetErrorString(err));			
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}


//雙週期合併計算
	int P1=1, P2=1;
	int TempP1 = P1;
	int TempP2 = P2;
	short *pTempPhase1 = pPhaseD;
	short *pTempPhase2 = pPhaseD_F2;
	unsigned char *pTempMask1 = pMaskD;
	unsigned char *pTempMask2 = pMaskD_F2;
	if( P1 > P2 )
	{
		TempP1 = P2;
		TempP2 = P1;
		pTempPhase1 = pPhaseD_F2;
		pTempPhase2 = pPhaseD;
		pTempMask1 = pMaskD_F2;
		pTempMask2 = pMaskD;
	}
	DoCudaSolveMultiPhase(TempP1, TempP2, pPhaseD, pMaskD, pPhaseD_F2, pMaskD_F2, W, H, IsUseTexture, ThreadNumber, BlockNumber);
	//...
	//...
	//...

//計算完成，資料Copy

	if( p2DImage != NULL )
	{
		bSuccess = checkCudaErrors(cudaMemcpy(p2DImage, pMergeD, sizeof(unsigned char) * DeviceDataSize, cudaMemcpyDeviceToHost));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "cudaMemcpy pMergerD To pMergeImage. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseData, pPhaseD, sizeof(short) * DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMask, pMaskD, sizeof(unsigned char) * DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }

#endif
	return true;
}
//--------------------------------------------------------------------//
*/
bool CCudaFunc::CudaSolvePhase221Frames2Period(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, double PA, double PB, int ExpTimeA, int ExpTimeB, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap)//2+2步雙相位
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolvePhase221Frames2Period";
	if ( CheckPtr7(fnName, PtrA1, PtrA2, PtrB1, PtrB2, PtrB3, pPhaseData, pMaskMap) == false )
	{	return false; }
	
	//GPU運算
	int ThreadNumber = 192;
	int BlockNumber = 961;	
	bool bSuccess = false;
	const int nStep = 2;
	short *pPhaseD = NULL;	
	short *pBasePhaseD = NULL;
	unsigned char *ImgDA1 = NULL;
	unsigned char *ImgDA2 = NULL;
	unsigned char *ImgDB1 = NULL;
	unsigned char *ImgDB2 = NULL;	
	unsigned char *ImgDB3 = NULL;	
	unsigned char *pMaskD = NULL;	
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);	
	if( NULL != BasePhasePtr )//相位相減
	{	
		if ( allocBuffer_Cuda(BufferSize, pBasePhaseD, fnName, "pBasePhaseD") == false )
		{
			CudaExceptionClean();
			return false;
		}
		bSuccess = checkCudaErrors(cudaMemcpy(pBasePhaseD, BasePhasePtr, sizeof(short)*DeviceDataSize, cudaMemcpyHostToDevice));		
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy BasePhasePtr To pBasePhaseD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
	}
	if ( allocBuffer_Cuda(BufferSize, ImgDA1, fnName, "ImgDA1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA2, fnName, "ImgDA2") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB1, fnName, "ImgDB1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB2, fnName, "ImgDB2") == false || 
		 allocBuffer_Cuda(BufferSize, ImgDB3, fnName, "ImgDB3") == false ||
		 allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseD, fnName, "pPhaseD") == false )
	{
		CudaExceptionClean();		
		return false;
	}	 	

	//Phase A
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA1, PtrA1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA1 To ImgDA1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA2, PtrA2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA2 To ImgDA2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	//Phase B
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB1, PtrB1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB1 To ImgDB1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB2, PtrB2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB2 To ImgDB2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB3, PtrB3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB3 To ImgDB3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	//Solve Phase A
	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaSolvePhase221Frames2Phase(ImgDA1, ImgDA2, ImgDB1, ImgDB2, ImgDB3, pBasePhaseD, PA, PB, ExpTimeA, ExpTimeB, DeviceDataSize, Gamma, NoiseParam, pMaskD, pPhaseD, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolvePhase221Frames2Phase Error. %s", cudaGetErrorString(err));	
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	

	//Release ImgD1, ImgD2, ImgD3, ImgD4
	freeBuffer_Cuda(ImgDA1);
	freeBuffer_Cuda(ImgDA2);
	freeBuffer_Cuda(ImgDB1);
	freeBuffer_Cuda(ImgDB2);	
	freeBuffer_Cuda(ImgDB3);
	freeBuffer_Cuda(pBasePhaseD);			

	//PhaseSmooth
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	if( SysParam.m_PhaseSmoothCudaMode == FN_ENABLE )
	{
		short *pPhaseSmoothSrcD = NULL;
		short *pPhaseSmoothDestD = NULL;
		float *pPhaseSmoothTempSinBufferD = NULL;
		float *pPhaseSmoothTempCosBufferD = NULL;
		unsigned char *pPhaseSmoothTempMaskBufferD = NULL;
		unsigned char *pPhaseSmoothMaskD = NULL;
		const int MaskSize = SysParam.m_PhaseSmoothCudaMaskSize;

		if ( allocBuffer_Cuda(BufferSize, pPhaseSmoothDestD, fnName, "pPhaseSmoothDestD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempSinBufferD, fnName, "pPhaseSmoothTempSinBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempCosBufferD, fnName, "pPhaseSmoothTempCosBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempMaskBufferD, fnName, "pPhaseSmoothTempMaskBufferD") == false )
		{	
			CudaExceptionClean();
			return false;
		}

		//先將原始資料複製一份到目標位置-Phase M
		pPhaseSmoothSrcD = pPhaseD;
		pPhaseSmoothMaskD = pMaskD;
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
		
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}

		freeBuffer_Cuda(pPhaseD);//清除原始相位指標		
		pPhaseD = pPhaseSmoothDestD;//重新導向相位指標

		freeBuffer_Cuda(pPhaseSmoothTempSinBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempCosBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempMaskBufferD);		
	}
	
	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseData, pPhaseD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskMap, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pPhaseD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolvePhase221Frames2Period2Exp(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, double PA, double PB, int ExpTimeA, int ExpTimeB, int ExpTimeC, int ExpTimeD, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap)//2+2步雙相位2曝光
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolvePhase221Frames2Period2Exp";
	if ( CheckPtr12(fnName, PtrA1, PtrA2, PtrB1, PtrB2, PtrB3, pPhaseData, pMaskMap, PtrC1, PtrC2, PtrD1, PtrD2, PtrD3) == false )
	{	return false; }
	
	//GPU運算
	int ThreadNumber = 192;
	int BlockNumber = 961;	
	bool bSuccess = false;
	const int nStep = 4;
	short *pPhaseD = NULL;	
	short *pPhaseDAB = NULL;	
	short *pPhaseDCD = NULL;	
	short *pBasePhaseD = NULL;
	unsigned char *ImgDA1 = NULL;
	unsigned char *ImgDA2 = NULL;
	unsigned char *ImgDB1 = NULL;
	unsigned char *ImgDB2 = NULL;	
	unsigned char *ImgDB3 = NULL;	
	unsigned char *pMaskD = NULL;	
	unsigned char *pMaskDAB = NULL;	
	unsigned char *pMaskDCD = NULL;	
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);	
	if( NULL != BasePhasePtr )//相位相減
	{	
		if ( allocBuffer_Cuda(BufferSize, pBasePhaseD, fnName, "pBasePhaseD") == false )
		{
			CudaExceptionClean();
			return false;
		}
		bSuccess = checkCudaErrors(cudaMemcpy(pBasePhaseD, BasePhasePtr, sizeof(short)*DeviceDataSize, cudaMemcpyHostToDevice));		
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy BasePhasePtr To pBasePhaseD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
	}
	if ( allocBuffer_Cuda(BufferSize, ImgDA1, fnName, "ImgDA1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA2, fnName, "ImgDA2") == false ||		 
		 allocBuffer_Cuda(BufferSize, ImgDB1, fnName, "ImgDB1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB2, fnName, "ImgDB2") == false ||		 
		 allocBuffer_Cuda(BufferSize, ImgDB3, fnName, "ImgDB3") == false ||		
		 allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pMaskDAB, fnName, "pMaskDAB") == false ||
		 allocBuffer_Cuda(BufferSize, pMaskDCD, fnName, "pMaskDCD") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseD, fnName, "pPhaseD") == false || 
		 allocBuffer_Cuda(BufferSize, pPhaseDAB, fnName, "pPhaseDAB") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseDCD, fnName, "pPhaseDCD") == false )
	{
		CudaExceptionClean();		
		return false;
	}	 	

	//Phase A
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA1, PtrA1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA1 To ImgDA1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA2, PtrA2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA2 To ImgDA2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

	//Phase B
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB1, PtrB1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB1 To ImgDB1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB2, PtrB2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB2 To ImgDB2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB3, PtrB3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB3 To ImgDB3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	
	//Solve Phase A
	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaSolvePhase221Frames2Phase(ImgDA1, ImgDA2, ImgDB1, ImgDB2, ImgDB3, pBasePhaseD, PA, PB, ExpTimeA, ExpTimeB, DeviceDataSize, Gamma, NoiseParam, pMaskDAB, pPhaseDAB, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolvePhase221Frames2Phase Error. %s", cudaGetErrorString(err));			
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	
	
	//Phase A - Exposure 2
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA1, PtrC1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrC1 To ImgDA1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA2, PtrC2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrC2 To ImgDA2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

	//Phase B - Exposure 2
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB1, PtrD1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrD1 To ImgDB1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB2, PtrD2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrD2 To ImgDB2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB3, PtrD3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrD3 To ImgDB3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	
	//Solve Phase A
	cudaThreadSynchronize();
	//Hostfunction
	ExecCudaSolvePhase221Frames2Phase(ImgDA1, ImgDA2, ImgDB1, ImgDB2, ImgDB3, pBasePhaseD, PA, PB, ExpTimeC, ExpTimeD, DeviceDataSize, Gamma, NoiseParam, pMaskDCD, pPhaseDCD, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolvePhase221Frames2Phase Error. %s", cudaGetErrorString(err));	
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	
	
	//Release ImgD1, ImgD2, ImgD3, ImgD4
	freeBuffer_Cuda(ImgDA1);
	freeBuffer_Cuda(ImgDA2);
	freeBuffer_Cuda(ImgDB1);
	freeBuffer_Cuda(ImgDB2);
	freeBuffer_Cuda(ImgDB3);
	freeBuffer_Cuda(pBasePhaseD);			

	//PhaseSmooth
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	if( SysParam.m_PhaseSmoothCudaMode == FN_ENABLE )
	{
		short *pPhaseSmoothSrcD = NULL;
		short *pPhaseSmoothDestD = NULL;
		float *pPhaseSmoothTempSinBufferD = NULL;
		float *pPhaseSmoothTempCosBufferD = NULL;
		unsigned char *pPhaseSmoothTempMaskBufferD = NULL;
		unsigned char *pPhaseSmoothMaskD = NULL;
		const int MaskSize = SysParam.m_PhaseSmoothCudaMaskSize;

		if ( allocBuffer_Cuda(BufferSize, pPhaseSmoothDestD, fnName, "pPhaseSmoothDestD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempSinBufferD, fnName, "pPhaseSmoothTempSinBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempCosBufferD, fnName, "pPhaseSmoothTempCosBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempMaskBufferD, fnName, "pPhaseSmoothTempMaskBufferD") == false )
		{	
			CudaExceptionClean();
			return false;
		}

		//先將原始資料複製一份到目標位置-Phase M
		pPhaseSmoothSrcD = pPhaseDAB;
		pPhaseSmoothMaskD = pMaskDAB;
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
		
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		cudaMemcpy(pPhaseSmoothSrcD, pPhaseSmoothDestD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice);
		//SetCudaExceptionCode_MemCopy();
		
		//先將原始資料複製一份到目標位置-Phase M-Exposure 2
		pPhaseSmoothSrcD = pPhaseDCD;
		pPhaseSmoothMaskD = pMaskDCD;
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
		
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));			
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		cudaMemcpy(pPhaseSmoothSrcD, pPhaseSmoothDestD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice);
		//SetCudaExceptionCode_MemCopy();

		freeBuffer_Cuda(pPhaseSmoothDestD);
		freeBuffer_Cuda(pPhaseSmoothTempSinBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempCosBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempMaskBufferD);		
	}
	
	//Merge 2 Exposure 2Phase	
	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaMerge2Exp2Phase(pMaskDAB, pPhaseDAB, pMaskDCD, pPhaseDCD, pMaskD, pPhaseD, DeviceDataSize, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaMerge2Exp2Phase Error. %s", cudaGetErrorString(err));			
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	
	freeBuffer_Cuda(pMaskDAB);
	freeBuffer_Cuda(pPhaseDAB);
	freeBuffer_Cuda(pMaskDCD);
	freeBuffer_Cuda(pPhaseDCD);

	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseData, pPhaseD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskMap, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pPhaseD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolvePhase42Frames2Period(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrB1, const unsigned char *PtrB2, double PA, double PB, int ExpTimeA, int ExpTimeB, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap)//4+2步雙相位
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolvePhase42Frames2Period";
	if ( CheckPtr8(fnName, PtrA1, PtrA2, PtrA3, PtrA4, PtrB1, PtrB2, pPhaseData, pMaskMap) == false )
	{	return false; }
	
	//GPU運算
	int ThreadNumber = 192;
	int BlockNumber = 961;	
	bool bSuccess = false;
	const int nStep = 4;
	short *pPhaseD = NULL;	
	short *pBasePhaseD = NULL;
	unsigned char *ImgDA1 = NULL;
	unsigned char *ImgDA2 = NULL;
	unsigned char *ImgDA3 = NULL;
	unsigned char *ImgDA4 = NULL;	
	unsigned char *ImgDB1 = NULL;
	unsigned char *ImgDB2 = NULL;	
	unsigned char *pMaskD = NULL;	
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);	
	if( NULL != BasePhasePtr )//相位相減
	{	
		if ( allocBuffer_Cuda(BufferSize, pBasePhaseD, fnName, "pBasePhaseD") == false )
		{
			CudaExceptionClean();
			return false;
		}
		bSuccess = checkCudaErrors(cudaMemcpy(pBasePhaseD, BasePhasePtr, sizeof(short)*DeviceDataSize, cudaMemcpyHostToDevice));		
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy BasePhasePtr To pBasePhaseD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
	}
	if ( allocBuffer_Cuda(BufferSize, ImgDA1, fnName, "ImgDA1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA2, fnName, "ImgDA2") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA3, fnName, "ImgDA3") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA4, fnName, "ImgDA4") == false ||		 		 
		 allocBuffer_Cuda(BufferSize, ImgDB1, fnName, "ImgDB1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB2, fnName, "ImgDB2") == false ||		 
		 allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseD, fnName, "pPhaseD") == false )
	{
		CudaExceptionClean();		
		return false;
	}	 	

	//Phase A
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA1, PtrA1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA1 To ImgDA1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA2, PtrA2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA2 To ImgDA2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA3, PtrA3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA3 To ImgDA3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA4, PtrA4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA4 To ImgDA4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

	//Phase B
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB1, PtrB1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB1 To ImgDB1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB2, PtrB2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB2 To ImgDB2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	//Solve Phase A
	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaSolvePhase42Frames2Phase(ImgDA1, ImgDA2, ImgDA3, ImgDA4, ImgDB1, ImgDB2, pBasePhaseD, PA, PB, ExpTimeA, ExpTimeB, DeviceDataSize, Gamma, NoiseParam, pMaskD, pPhaseD, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolvePhase42Frames2Phase Error. %s", cudaGetErrorString(err));		
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	

	//Release ImgD1, ImgD2, ImgD3, ImgD4
	freeBuffer_Cuda(ImgDA1);
	freeBuffer_Cuda(ImgDA2);
	freeBuffer_Cuda(ImgDA3);
	freeBuffer_Cuda(ImgDA4);
	freeBuffer_Cuda(ImgDB1);
	freeBuffer_Cuda(ImgDB2);	
	freeBuffer_Cuda(pBasePhaseD);			

	//PhaseSmooth
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	if( SysParam.m_PhaseSmoothCudaMode == FN_ENABLE )
	{
		short *pPhaseSmoothSrcD = NULL;
		short *pPhaseSmoothDestD = NULL;
		float *pPhaseSmoothTempSinBufferD = NULL;
		float *pPhaseSmoothTempCosBufferD = NULL;
		unsigned char *pPhaseSmoothTempMaskBufferD = NULL;
		unsigned char *pPhaseSmoothMaskD = NULL;
		const int MaskSize = SysParam.m_PhaseSmoothCudaMaskSize;

		if ( allocBuffer_Cuda(BufferSize, pPhaseSmoothDestD, fnName, "pPhaseSmoothDestD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempSinBufferD, fnName, "pPhaseSmoothTempSinBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempCosBufferD, fnName, "pPhaseSmoothTempCosBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempMaskBufferD, fnName, "pPhaseSmoothTempMaskBufferD") == false )
		{	
			CudaExceptionClean();
			return false;
		}

		//先將原始資料複製一份到目標位置-Phase M
		pPhaseSmoothSrcD = pPhaseD;
		pPhaseSmoothMaskD = pMaskD;
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
		
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));			
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}

		freeBuffer_Cuda(pPhaseD);//清除原始相位指標		
		pPhaseD = pPhaseSmoothDestD;//重新導向相位指標

		freeBuffer_Cuda(pPhaseSmoothTempSinBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempCosBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempMaskBufferD);		
	}
	
	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseData, pPhaseD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskMap, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pPhaseD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolvePhase42Frames2Period2Exp(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrD1, const unsigned char *PtrD2, double PA, double PB, int ExpTimeA, int ExpTimeB, int ExpTimeC, int ExpTimeD, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap)//4+2步雙相位2曝光
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolvePhase42Frames2Period2Exp";
	if ( CheckPtr14(fnName, PtrA1, PtrA2, PtrA3, PtrA4, PtrB1, PtrB2, pPhaseData, pMaskMap, PtrC1, PtrC2, PtrC3, PtrC4, PtrD1, PtrD2) == false )
	{	return false; }
	
	//GPU運算
	int ThreadNumber = 192;
	int BlockNumber = 961;	
	bool bSuccess = false;
	const int nStep = 4;
	short *pPhaseD = NULL;	
	short *pPhaseDAB = NULL;	
	short *pPhaseDCD = NULL;	
	short *pBasePhaseD = NULL;
	unsigned char *ImgDA1 = NULL;
	unsigned char *ImgDA2 = NULL;
	unsigned char *ImgDA3 = NULL;
	unsigned char *ImgDA4 = NULL;	
	unsigned char *ImgDB1 = NULL;
	unsigned char *ImgDB2 = NULL;	
	unsigned char *pMaskD = NULL;	
	unsigned char *pMaskDAB = NULL;	
	unsigned char *pMaskDCD = NULL;	
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);	
	if( NULL != BasePhasePtr )//相位相減
	{	
		if ( allocBuffer_Cuda(BufferSize, pBasePhaseD, fnName, "pBasePhaseD") == false )
		{
			CudaExceptionClean();
			return false;
		}
		bSuccess = checkCudaErrors(cudaMemcpy(pBasePhaseD, BasePhasePtr, sizeof(short)*DeviceDataSize, cudaMemcpyHostToDevice));		
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy BasePhasePtr To pBasePhaseD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
	}
	if ( allocBuffer_Cuda(BufferSize, ImgDA1, fnName, "ImgDA1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA2, fnName, "ImgDA2") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA3, fnName, "ImgDA3") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA4, fnName, "ImgDA4") == false ||		 		 
		 allocBuffer_Cuda(BufferSize, ImgDB1, fnName, "ImgDB1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB2, fnName, "ImgDB2") == false ||		 
		 allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pMaskDAB, fnName, "pMaskDAB") == false ||
		 allocBuffer_Cuda(BufferSize, pMaskDCD, fnName, "pMaskDCD") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseD, fnName, "pPhaseD") == false || 
		 allocBuffer_Cuda(BufferSize, pPhaseDAB, fnName, "pPhaseDAB") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseDCD, fnName, "pPhaseDCD") == false )
	{
		CudaExceptionClean();		
		return false;
	}	 	

	//Phase A
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA1, PtrA1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA1 To ImgDA1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA2, PtrA2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA2 To ImgDA2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA3, PtrA3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA3 To ImgDA3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA4, PtrA4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA4 To ImgDA4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

	//Phase B
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB1, PtrB1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB1 To ImgDB1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB2, PtrB2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB2 To ImgDB2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	
	//Solve Phase A
	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaSolvePhase42Frames2Phase(ImgDA1, ImgDA2, ImgDA3, ImgDA4, ImgDB1, ImgDB2, pBasePhaseD, PA, PB, ExpTimeA, ExpTimeB, DeviceDataSize, Gamma, NoiseParam, pMaskDAB, pPhaseDAB, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolvePhase42Frames2Phase Error. %s", cudaGetErrorString(err));		
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	
	
	//Phase A - Exposure 2
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA1, PtrC1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrC1 To ImgDA1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA2, PtrC2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrC2 To ImgDA2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA3, PtrC3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrC3 To ImgDA3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA4, PtrC4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrC4 To ImgDA4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

	//Phase B - Exposure 2
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB1, PtrD1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrD1 To ImgDB1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB2, PtrD2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrD2 To ImgDB2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	
	//Solve Phase A
	cudaThreadSynchronize();
	//Hostfunction
	ExecCudaSolvePhase42Frames2Phase(ImgDA1, ImgDA2, ImgDA3, ImgDA4, ImgDB1, ImgDB2, pBasePhaseD, PA, PB, ExpTimeC, ExpTimeD, DeviceDataSize, Gamma, NoiseParam, pMaskDCD, pPhaseDCD, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolvePhase42Frames2Phase Error. %s", cudaGetErrorString(err));	
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	
	
	//Release ImgD1, ImgD2, ImgD3, ImgD4
	freeBuffer_Cuda(ImgDA1);
	freeBuffer_Cuda(ImgDA2);
	freeBuffer_Cuda(ImgDA3);
	freeBuffer_Cuda(ImgDA4);
	freeBuffer_Cuda(ImgDB1);
	freeBuffer_Cuda(ImgDB2);	
	freeBuffer_Cuda(pBasePhaseD);			

	//PhaseSmooth
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	if( SysParam.m_PhaseSmoothCudaMode == FN_ENABLE )
	{
		short *pPhaseSmoothSrcD = NULL;
		short *pPhaseSmoothDestD = NULL;
		float *pPhaseSmoothTempSinBufferD = NULL;
		float *pPhaseSmoothTempCosBufferD = NULL;
		unsigned char *pPhaseSmoothTempMaskBufferD = NULL;
		unsigned char *pPhaseSmoothMaskD = NULL;
		const int MaskSize = SysParam.m_PhaseSmoothCudaMaskSize;

		if ( allocBuffer_Cuda(BufferSize, pPhaseSmoothDestD, fnName, "pPhaseSmoothDestD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempSinBufferD, fnName, "pPhaseSmoothTempSinBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempCosBufferD, fnName, "pPhaseSmoothTempCosBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempMaskBufferD, fnName, "pPhaseSmoothTempMaskBufferD") == false )
		{	
			CudaExceptionClean();
			return false;
		}

		//先將原始資料複製一份到目標位置-Phase M
		pPhaseSmoothSrcD = pPhaseDAB;
		pPhaseSmoothMaskD = pMaskDAB;
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
		
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		cudaMemcpy(pPhaseSmoothSrcD, pPhaseSmoothDestD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice);
		//SetCudaExceptionCode_MemCopy();

		//先將原始資料複製一份到目標位置-Phase M-Exposure 2
		pPhaseSmoothSrcD = pPhaseDCD;
		pPhaseSmoothMaskD = pMaskDCD;
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
		
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		cudaMemcpy(pPhaseSmoothSrcD, pPhaseSmoothDestD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice);
		//SetCudaExceptionCode_MemCopy();

		freeBuffer_Cuda(pPhaseSmoothDestD);
		freeBuffer_Cuda(pPhaseSmoothTempSinBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempCosBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempMaskBufferD);		
	}
	
	//Merge 2 Exposure 2Phase	
	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaMerge2Exp2Phase(pMaskDAB, pPhaseDAB, pMaskDCD, pPhaseDCD, pMaskD, pPhaseD, DeviceDataSize, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaMerge2Exp2Phase Error. %s", cudaGetErrorString(err));		
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	
	freeBuffer_Cuda(pMaskDAB);
	freeBuffer_Cuda(pPhaseDAB);
	freeBuffer_Cuda(pMaskDCD);
	freeBuffer_Cuda(pPhaseDCD);

	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseData, pPhaseD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskMap, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pPhaseD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolvePhase44GCFrames2Period(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PerB, int ExpTimeB, float Gamma, const short *BasePhasePtr, TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap)//4+4GC張灰階影像解相位
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolvePhase44GCFrames2Period";
	if ( CheckPtr10(fnName, PtrA1, PtrA2, PtrA3, PtrA4, PtrB1, PtrB2, PtrB3, PtrB4, pPhaseData, pMaskMap) == false )
	{	return false; }
	
	//GPU運算
	int ThreadNumber = 192;
	int BlockNumber = 961;	
	bool bSuccess = false;
	const int nStep = 4;
	short *pPhaseD = NULL;	
	short *pBasePhaseD = NULL;
	unsigned char *ImgDA1 = NULL;
	unsigned char *ImgDA2 = NULL;
	unsigned char *ImgDA3 = NULL;
	unsigned char *ImgDA4 = NULL;	
	unsigned char *ImgDB1 = NULL;
	unsigned char *ImgDB2 = NULL;	
	unsigned char *ImgDB3 = NULL;
	unsigned char *ImgDB4 = NULL;
	unsigned char *pMaskD = NULL;	
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);	
	if( NULL != BasePhasePtr )//相位相減
	{	
		if ( allocBuffer_Cuda(BufferSize, pBasePhaseD, fnName, "pBasePhaseD") == false )
		{
			CudaExceptionClean();
			return false;
		}
		bSuccess = checkCudaErrors(cudaMemcpy(pBasePhaseD, BasePhasePtr, sizeof(short)*DeviceDataSize, cudaMemcpyHostToDevice));		
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy BasePhasePtr To pBasePhaseD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
	}
	if ( allocBuffer_Cuda(BufferSize, ImgDA1, fnName, "ImgDA1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA2, fnName, "ImgDA2") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA3, fnName, "ImgDA3") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA4, fnName, "ImgDA4") == false ||		 		 
		 allocBuffer_Cuda(BufferSize, ImgDB1, fnName, "ImgDB1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB2, fnName, "ImgDB2") == false ||		 
		 allocBuffer_Cuda(BufferSize, ImgDB3, fnName, "ImgDB3") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB4, fnName, "ImgDB4") == false ||		 
		 allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseD, fnName, "pPhaseD") == false )
	{
		CudaExceptionClean();		
		return false;
	}	 	

	//Phase A
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA1, PtrA1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA1 To ImgDA1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA2, PtrA2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA2 To ImgDA2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA3, PtrA3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA3 To ImgDA3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA4, PtrA4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA4 To ImgDA4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

	//Phase B
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB1, PtrB1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB1 To ImgDB1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB2, PtrB2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB2 To ImgDB2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB3, PtrB3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB3 To ImgDB3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB4, PtrB4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB4 To ImgDB4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	
	//Solve Phase A
	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaSolvePhase44GCFrames2Phase(ImgDA1, ImgDA2, ImgDA3, ImgDA4, ImgDB1, ImgDB2, ImgDB3, ImgDB4, pBasePhaseD, PerA, PerB, ExpTimeA, ExpTimeB, DeviceDataSize, Gamma, NoiseParam, pMaskD, pPhaseD, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolvePhase44GCFrames2Phase Error. %s", cudaGetErrorString(err));			
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	

	//Release ImgD1, ImgD2, ImgD3, ImgD4
	freeBuffer_Cuda(ImgDA1);
	freeBuffer_Cuda(ImgDA2);
	freeBuffer_Cuda(ImgDA3);
	freeBuffer_Cuda(ImgDA4);
	freeBuffer_Cuda(ImgDB1);
	freeBuffer_Cuda(ImgDB2);	
	freeBuffer_Cuda(ImgDB3);
	freeBuffer_Cuda(ImgDB4);
	freeBuffer_Cuda(pBasePhaseD);			

	//PhaseSmooth
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	if( SysParam.m_PhaseSmoothCudaMode == FN_ENABLE )
	{
		short *pPhaseSmoothSrcD = NULL;
		short *pPhaseSmoothDestD = NULL;
		float *pPhaseSmoothTempSinBufferD = NULL;
		float *pPhaseSmoothTempCosBufferD = NULL;
		unsigned char *pPhaseSmoothTempMaskBufferD = NULL;
		unsigned char *pPhaseSmoothMaskD = NULL;
		const int MaskSize = SysParam.m_PhaseSmoothCudaMaskSize;

		if ( allocBuffer_Cuda(BufferSize, pPhaseSmoothDestD, fnName, "pPhaseSmoothDestD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempSinBufferD, fnName, "pPhaseSmoothTempSinBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempCosBufferD, fnName, "pPhaseSmoothTempCosBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempMaskBufferD, fnName, "pPhaseSmoothTempMaskBufferD") == false )
		{	
			CudaExceptionClean();
			return false;
		}

		//先將原始資料複製一份到目標位置-Phase M
		pPhaseSmoothSrcD = pPhaseD;
		pPhaseSmoothMaskD = pMaskD;
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
		
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}

		freeBuffer_Cuda(pPhaseD);//清除原始相位指標		
		pPhaseD = pPhaseSmoothDestD;//重新導向相位指標

		freeBuffer_Cuda(pPhaseSmoothTempSinBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempCosBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempMaskBufferD);		
	}
	
	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseData, pPhaseD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskMap, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pPhaseD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolvePhase45GCFrames2Period(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, double PerB, int ExpTimeB, float Gamma, const short *BasePhasePtr, TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap)//4+5GC張灰階影像解相位
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolvePhase45GCFrames2Period";
	if ( CheckPtr11(fnName, PtrA1, PtrA2, PtrA3, PtrA4, PtrB1, PtrB2, PtrB3, PtrB4, PtrB5, pPhaseData, pMaskMap) == false )
	{	return false; }
	
	//GPU運算
	int ThreadNumber = 192;
	int BlockNumber = 961;	
	bool bSuccess = false;
	const int nStep = 4;
	short *pPhaseD = NULL;	
	short *pBasePhaseD = NULL;
	unsigned char *ImgDA1 = NULL;
	unsigned char *ImgDA2 = NULL;
	unsigned char *ImgDA3 = NULL;
	unsigned char *ImgDA4 = NULL;	
	unsigned char *ImgDB1 = NULL;
	unsigned char *ImgDB2 = NULL;	
	unsigned char *ImgDB3 = NULL;
	unsigned char *ImgDB4 = NULL;	
	unsigned char *ImgDB5 = NULL;	
	unsigned char *pMaskD = NULL;	
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);	
	if( NULL != BasePhasePtr )//相位相減
	{	
		if ( allocBuffer_Cuda(BufferSize, pBasePhaseD, fnName, "pBasePhaseD") == false )
		{
			CudaExceptionClean();
			return false;
		}
		bSuccess = checkCudaErrors(cudaMemcpy(pBasePhaseD, BasePhasePtr, sizeof(short)*DeviceDataSize, cudaMemcpyHostToDevice));		
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy BasePhasePtr To pBasePhaseD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
	}
	if ( allocBuffer_Cuda(BufferSize, ImgDA1, fnName, "ImgDA1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA2, fnName, "ImgDA2") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA3, fnName, "ImgDA3") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA4, fnName, "ImgDA4") == false ||		 		 
		 allocBuffer_Cuda(BufferSize, ImgDB1, fnName, "ImgDB1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB2, fnName, "ImgDB2") == false ||		 
		 allocBuffer_Cuda(BufferSize, ImgDB3, fnName, "ImgDB3") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB4, fnName, "ImgDB4") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB5, fnName, "ImgDB5") == false ||
		 allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseD, fnName, "pPhaseD") == false )
	{
		CudaExceptionClean();		
		return false;
	}	 	

	//Phase A
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA1, PtrA1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA1 To ImgDA1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA2, PtrA2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA2 To ImgDA2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA3, PtrA3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA3 To ImgDA3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA4, PtrA4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA4 To ImgDA4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

	//Phase B
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB1, PtrB1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB1 To ImgDB1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB2, PtrB2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB2 To ImgDB2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB3, PtrB3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB3 To ImgDB3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB4, PtrB4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB4 To ImgDB4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB5, PtrB5, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB5 To ImgDB5. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	
	//Solve Phase A
	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaSolvePhase45GCFrames2Phase(ImgDA1, ImgDA2, ImgDA3, ImgDA4, ImgDB1, ImgDB2, ImgDB3, ImgDB4, ImgDB5, pBasePhaseD, PerA, PerB, ExpTimeA, ExpTimeB, DeviceDataSize, Gamma, NoiseParam, pMaskD, pPhaseD, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolvePhase45GCFrames2Phase Error. %s", cudaGetErrorString(err));	
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	

	//Release ImgD1, ImgD2, ImgD3, ImgD4
	freeBuffer_Cuda(ImgDA1);
	freeBuffer_Cuda(ImgDA2);
	freeBuffer_Cuda(ImgDA3);
	freeBuffer_Cuda(ImgDA4);
	freeBuffer_Cuda(ImgDB1);
	freeBuffer_Cuda(ImgDB2);	
	freeBuffer_Cuda(ImgDB3);
	freeBuffer_Cuda(ImgDB4);	
	freeBuffer_Cuda(ImgDB5);	
	freeBuffer_Cuda(pBasePhaseD);			

	//PhaseSmooth
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	if( SysParam.m_PhaseSmoothCudaMode == FN_ENABLE )
	{
		short *pPhaseSmoothSrcD = NULL;
		short *pPhaseSmoothDestD = NULL;
		float *pPhaseSmoothTempSinBufferD = NULL;
		float *pPhaseSmoothTempCosBufferD = NULL;
		unsigned char *pPhaseSmoothTempMaskBufferD = NULL;
		unsigned char *pPhaseSmoothMaskD = NULL;
		const int MaskSize = SysParam.m_PhaseSmoothCudaMaskSize;

		if ( allocBuffer_Cuda(BufferSize, pPhaseSmoothDestD, fnName, "pPhaseSmoothDestD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempSinBufferD, fnName, "pPhaseSmoothTempSinBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempCosBufferD, fnName, "pPhaseSmoothTempCosBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempMaskBufferD, fnName, "pPhaseSmoothTempMaskBufferD") == false )
		{	
			CudaExceptionClean();
			return false;
		}

		//先將原始資料複製一份到目標位置-Phase M
		pPhaseSmoothSrcD = pPhaseD;
		pPhaseSmoothMaskD = pMaskD;
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
		
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}

		freeBuffer_Cuda(pPhaseD);//清除原始相位指標		
		pPhaseD = pPhaseSmoothDestD;//重新導向相位指標

		freeBuffer_Cuda(pPhaseSmoothTempSinBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempCosBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempMaskBufferD);		
	}
	
	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseData, pPhaseD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskMap, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pPhaseD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolvePhase46GCFrames2Period(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, double PerA, int ExpTimeA, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrB5, const unsigned char *PtrB6, double PerB, int ExpTimeB, float Gamma, const short *BasePhasePtr, TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap)//4+6GC張灰階影像解相位
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolvePhase46GCFrames2Period";
	if ( CheckPtr12(fnName, PtrA1, PtrA2, PtrA3, PtrA4, PtrB1, PtrB2, PtrB3, PtrB4, PtrB5, PtrB6, pPhaseData, pMaskMap) == false )
	{	return false; }
	
	//GPU運算
	int ThreadNumber = 192;
	int BlockNumber = 961;	
	bool bSuccess = false;
	const int nStep = 4;
	short *pPhaseD = NULL;	
	short *pBasePhaseD = NULL;
	unsigned char *ImgDA1 = NULL;
	unsigned char *ImgDA2 = NULL;
	unsigned char *ImgDA3 = NULL;
	unsigned char *ImgDA4 = NULL;	
	unsigned char *ImgDB1 = NULL;
	unsigned char *ImgDB2 = NULL;	
	unsigned char *ImgDB3 = NULL;
	unsigned char *ImgDB4 = NULL;	
	unsigned char *ImgDB5 = NULL;	
	unsigned char *ImgDB6 = NULL;	
	unsigned char *pMaskD = NULL;	
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);	
	if( NULL != BasePhasePtr )//相位相減
	{	
		if ( allocBuffer_Cuda(BufferSize, pBasePhaseD, fnName, "pBasePhaseD") == false )
		{
			CudaExceptionClean();
			return false;
		}
		bSuccess = checkCudaErrors(cudaMemcpy(pBasePhaseD, BasePhasePtr, sizeof(short)*DeviceDataSize, cudaMemcpyHostToDevice));		
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy BasePhasePtr To pBasePhaseD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
	}
	if ( allocBuffer_Cuda(BufferSize, ImgDA1, fnName, "ImgDA1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA2, fnName, "ImgDA2") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA3, fnName, "ImgDA3") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA4, fnName, "ImgDA4") == false ||		 		 
		 allocBuffer_Cuda(BufferSize, ImgDB1, fnName, "ImgDB1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB2, fnName, "ImgDB2") == false ||		 
		 allocBuffer_Cuda(BufferSize, ImgDB3, fnName, "ImgDB3") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB4, fnName, "ImgDB4") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB5, fnName, "ImgDB5") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB6, fnName, "ImgDB6") == false ||
		 allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseD, fnName, "pPhaseD") == false )
	{
		CudaExceptionClean();		
		return false;
	}	 	

	//Phase A
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA1, PtrA1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA1 To ImgDA1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA2, PtrA2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA2 To ImgDA2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA3, PtrA3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA3 To ImgDA3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA4, PtrA4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA4 To ImgDA4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

	//Phase B
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB1, PtrB1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB1 To ImgDB1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB2, PtrB2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB2 To ImgDB2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB3, PtrB3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB3 To ImgDB3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB4, PtrB4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB4 To ImgDB4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB5, PtrB5, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB5 To ImgDB5. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB6, PtrB6, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB6 To ImgDB6. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	
	//Solve Phase A
	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaSolvePhase46GCFrames2Phase(ImgDA1, ImgDA2, ImgDA3, ImgDA4, ImgDB1, ImgDB2, ImgDB3, ImgDB4, ImgDB5, ImgDB6, pBasePhaseD, PerA, PerB, ExpTimeA, ExpTimeB, DeviceDataSize, Gamma, NoiseParam, pMaskD, pPhaseD, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolvePhase46GCFrames2Phase Error. %s", cudaGetErrorString(err));		
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	

	//Release ImgD1, ImgD2, ImgD3, ImgD4
	freeBuffer_Cuda(ImgDA1);
	freeBuffer_Cuda(ImgDA2);
	freeBuffer_Cuda(ImgDA3);
	freeBuffer_Cuda(ImgDA4);
	freeBuffer_Cuda(ImgDB1);
	freeBuffer_Cuda(ImgDB2);	
	freeBuffer_Cuda(ImgDB3);
	freeBuffer_Cuda(ImgDB4);	
	freeBuffer_Cuda(ImgDB5);
	freeBuffer_Cuda(ImgDB6);
	freeBuffer_Cuda(pBasePhaseD);			

	//PhaseSmooth
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	if( SysParam.m_PhaseSmoothCudaMode == FN_ENABLE )
	{
		short *pPhaseSmoothSrcD = NULL;
		short *pPhaseSmoothDestD = NULL;
		float *pPhaseSmoothTempSinBufferD = NULL;
		float *pPhaseSmoothTempCosBufferD = NULL;
		unsigned char *pPhaseSmoothTempMaskBufferD = NULL;
		unsigned char *pPhaseSmoothMaskD = NULL;
		const int MaskSize = SysParam.m_PhaseSmoothCudaMaskSize;

		if ( allocBuffer_Cuda(BufferSize, pPhaseSmoothDestD, fnName, "pPhaseSmoothDestD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempSinBufferD, fnName, "pPhaseSmoothTempSinBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempCosBufferD, fnName, "pPhaseSmoothTempCosBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempMaskBufferD, fnName, "pPhaseSmoothTempMaskBufferD") == false )
		{	
			CudaExceptionClean();
			return false;
		}

		//先將原始資料複製一份到目標位置-Phase M
		pPhaseSmoothSrcD = pPhaseD;
		pPhaseSmoothMaskD = pMaskD;
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
		
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}

		freeBuffer_Cuda(pPhaseD);//清除原始相位指標		
		pPhaseD = pPhaseSmoothDestD;//重新導向相位指標

		freeBuffer_Cuda(pPhaseSmoothTempSinBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempCosBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempMaskBufferD);		
	}
	
	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseData, pPhaseD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskMap, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pPhaseD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolvePhase4Frames(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *pImage1, const unsigned char *pImage2, const unsigned char *pImage3, const unsigned char *pImage4, int PatternID, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap)//4步相位
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolvePhase4Frames";
	if ( CheckPtr6(fnName, pImage1, pImage2, pImage3, pImage4, pPhaseData, pMaskMap) == false )
	{	return false; }
	
	//GPU運算
	const int nStep = 4;
	short *pPhaseD = NULL;
	unsigned char *ImgD1 = NULL;
	unsigned char *ImgD2 = NULL;
	unsigned char *ImgD3 = NULL;
	unsigned char *ImgD4 = NULL;
	unsigned char *ImgD5 = NULL;		
	unsigned char *pMaskD = NULL;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	if ( allocBuffer_Cuda(BufferSize, ImgD1, fnName, "ImgD1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgD2, fnName, "ImgD2") == false ||
		 allocBuffer_Cuda(BufferSize, ImgD3, fnName, "ImgD3") == false ||
		 allocBuffer_Cuda(BufferSize, ImgD4, fnName, "ImgD4") == false ||		 
		 allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseD, fnName, "pPhaseD") == false )
	{
		CudaExceptionClean();
		return false;
	}	 

	bool bSuccess = false;	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);	
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD1, pImage1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage1 To ImgD1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD2, pImage2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage2 To ImgD2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD3, pImage3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage3 To ImgD3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD4, pImage4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pImage4 To ImgD4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	
	int ThreadNumber = 192;
	int BlockNumber = 961;	

	ThreadNumber = 192;
	BlockNumber = 961;
	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaSolvePhase4Frames(ImgD1, ImgD2, ImgD3, ImgD4, pPhaseD, pMaskD, DeviceDataSize, PatternID, Gamma, NoiseParam, IsUseTexture, ThreadNumber, BlockNumber);

	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "DoCudaSolvePhase Error. %s", cudaGetErrorString(err));		
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}
	freeBuffer_Cuda(ImgD1);
	freeBuffer_Cuda(ImgD2);
	freeBuffer_Cuda(ImgD3);
	freeBuffer_Cuda(ImgD4);		

	//PhaseSmooth
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	if( SysParam.m_PhaseSmoothCudaMode == FN_ENABLE )
	{
		short *pPhaseSmoothSrcD = NULL;
		short *pPhaseSmoothDestD = NULL;
		float *pPhaseSmoothTempSinBufferD = NULL;
		float *pPhaseSmoothTempCosBufferD = NULL;
		unsigned char *pPhaseSmoothTempMaskBufferD = NULL;
		unsigned char *pPhaseSmoothMaskD = NULL;
		const int MaskSize = SysParam.m_PhaseSmoothCudaMaskSize;

		pPhaseSmoothSrcD = pPhaseD;
		pPhaseSmoothMaskD = pMaskD;

		if ( allocBuffer_Cuda(BufferSize, pPhaseSmoothDestD, fnName, "pPhaseSmoothDestD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempSinBufferD, fnName, "pPhaseSmoothTempSinBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempCosBufferD, fnName, "pPhaseSmoothTempCosBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempMaskBufferD, fnName, "pPhaseSmoothTempMaskBufferD") == false )
		{	
			CudaExceptionClean();
			return false;
		}

		//先將原始資料複製一份到目標位置
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}

		//
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}

		freeBuffer_Cuda(pPhaseD);//清除原始相位指標
		freeBuffer_Cuda(pPhaseSmoothTempSinBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempCosBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempMaskBufferD);		
		pPhaseD = pPhaseSmoothDestD;//重新導向相位指標
	}	
	if( NULL != BasePhasePtr )//相位相減
	{
		short *pBasePhaseD = NULL;
		if ( allocBuffer_Cuda(BufferSize, pBasePhaseD, fnName, "pBasePhaseD") == false )
		{
			CudaExceptionClean();
			return false;
		}
		bSuccess = checkCudaErrors(cudaMemcpy(pBasePhaseD, BasePhasePtr, sizeof(short)*DeviceDataSize, cudaMemcpyHostToDevice));		
		//SetCudaExceptionCode_MemCopy();
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaSubtractBasePhasePlane(pPhaseD, pBasePhaseD, DeviceDataSize, IsUseTexture);
		cudaThreadSynchronize();//等到device執行完
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "DoCudaSubtractBasePhasePlane Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		freeBuffer_Cuda(pBasePhaseD);
	}
	
	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseData, pPhaseD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskMap, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pPhaseD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolvePhase4Frames2Period(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PA, double PB, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap)//4步雙相位
{
	//return CudaSolvePhase4Frames2Period_Separate(ImageW, ImageH, ImageStep, PtrA1, PtrA2, PtrA3, PtrA4, PtrB1, PtrB2, PtrB3, PtrB4, PA, PB, Gamma, BasePhasePtr, NoiseParam, pPhaseData, pMaskMap);
	return CudaSolvePhase4Frames2Period_Combine(ImageW, ImageH, ImageStep, PtrA1, PtrA2, PtrA3, PtrA4, PtrB1, PtrB2, PtrB3, PtrB4, PA, PB, Gamma, BasePhasePtr, NoiseParam, pPhaseData, pMaskMap);
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolvePhase4Frames2Period_Combine(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PA, double PB, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap)//4步雙相位
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolvePhase4Frames2Period_Combine";
	if ( CheckPtr10(fnName, PtrA1, PtrA2, PtrA3, PtrA4, PtrB1, PtrB2, PtrB3, PtrB4, pPhaseData, pMaskMap) == false )
	{	return false; }
	
	//GPU運算
	int ThreadNumber = 192;
	int BlockNumber = 961;	
	bool bSuccess = false;
	const int nStep = 4;
	short *pPhaseD = NULL;	
	short *pBasePhaseD = NULL;
	unsigned char *ImgDA1 = NULL;
	unsigned char *ImgDA2 = NULL;
	unsigned char *ImgDA3 = NULL;
	unsigned char *ImgDA4 = NULL;	
	unsigned char *ImgDB1 = NULL;
	unsigned char *ImgDB2 = NULL;
	unsigned char *ImgDB3 = NULL;
	unsigned char *ImgDB4 = NULL;	
	unsigned char *pMaskD = NULL;	
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);	
	if( NULL != BasePhasePtr )//相位相減
	{	
		if ( allocBuffer_Cuda(BufferSize, pBasePhaseD, fnName, "pBasePhaseD") == false )
		{
			CudaExceptionClean();
			return false;
		}
		bSuccess = checkCudaErrors(cudaMemcpy(pBasePhaseD, BasePhasePtr, sizeof(short)*DeviceDataSize, cudaMemcpyHostToDevice));		
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy BasePhasePtr To pBasePhaseD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
	}
	if ( allocBuffer_Cuda(BufferSize, ImgDA1, fnName, "ImgDA1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA2, fnName, "ImgDA2") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA3, fnName, "ImgDA3") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA4, fnName, "ImgDA4") == false ||		 		 
		 allocBuffer_Cuda(BufferSize, ImgDB1, fnName, "ImgDB1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB2, fnName, "ImgDB2") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB3, fnName, "ImgDB3") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB4, fnName, "ImgDB4") == false ||		 
		 allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseD, fnName, "pPhaseD") == false )
	{
		CudaExceptionClean();		
		return false;
	}	 	

	//Phase A
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA1, PtrA1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA1 To ImgDA1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA2, PtrA2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA2 To ImgDA2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA3, PtrA3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA3 To ImgDA3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA4, PtrA4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA4 To ImgDA4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

	//Phase B
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB1, PtrB1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB1 To ImgDB1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB2, PtrB2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB2 To ImgDB2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB3, PtrB3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB3 To ImgDB3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB4, PtrB4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB4 To ImgDB4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	
	//Solve Phase A
	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaSolvePhase4Frames2Phase(ImgDA1, ImgDA2, ImgDA3, ImgDA4, ImgDB1, ImgDB2, ImgDB3, ImgDB4, pBasePhaseD, PA, PB, DeviceDataSize, Gamma, NoiseParam, pMaskD, pPhaseD, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolvePhase4Frames2Phase Error. %s", cudaGetErrorString(err));	
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	

	//Release ImgD1, ImgD2, ImgD3, ImgD4
	freeBuffer_Cuda(ImgDA1);
	freeBuffer_Cuda(ImgDA2);
	freeBuffer_Cuda(ImgDA3);
	freeBuffer_Cuda(ImgDA4);
	freeBuffer_Cuda(ImgDB1);
	freeBuffer_Cuda(ImgDB2);
	freeBuffer_Cuda(ImgDB3);
	freeBuffer_Cuda(ImgDB4);		
	freeBuffer_Cuda(pBasePhaseD);			

	//PhaseSmooth
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	if( SysParam.m_PhaseSmoothCudaMode == FN_ENABLE )
	{
		short *pPhaseSmoothSrcD = NULL;
		short *pPhaseSmoothDestD = NULL;
		float *pPhaseSmoothTempSinBufferD = NULL;
		float *pPhaseSmoothTempCosBufferD = NULL;
		unsigned char *pPhaseSmoothTempMaskBufferD = NULL;
		unsigned char *pPhaseSmoothMaskD = NULL;
		const int MaskSize = SysParam.m_PhaseSmoothCudaMaskSize;

		if ( allocBuffer_Cuda(BufferSize, pPhaseSmoothDestD, fnName, "pPhaseSmoothDestD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempSinBufferD, fnName, "pPhaseSmoothTempSinBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempCosBufferD, fnName, "pPhaseSmoothTempCosBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempMaskBufferD, fnName, "pPhaseSmoothTempMaskBufferD") == false )
		{	
			CudaExceptionClean();
			return false;
		}

		//先將原始資料複製一份到目標位置-Phase M
		pPhaseSmoothSrcD = pPhaseD;
		pPhaseSmoothMaskD = pMaskD;
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
		
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}

		freeBuffer_Cuda(pPhaseD);//清除原始相位指標		
		pPhaseD = pPhaseSmoothDestD;//重新導向相位指標

		freeBuffer_Cuda(pPhaseSmoothTempSinBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempCosBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempMaskBufferD);		
	}
	
	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseData, pPhaseD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskMap, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pPhaseD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolvePhase4Frames2Period_Separate(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, double PA, double PB, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap)//4步雙相位
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolvePhase4Frames2Period_Separate";
	if ( CheckPtr10(fnName, PtrA1, PtrA2, PtrA3, PtrA4, PtrB1, PtrB2, PtrB3, PtrB4, pPhaseData, pMaskMap) == false )
	{	return false; }
	
	//GPU運算
	int ThreadNumber = 192;
	int BlockNumber = 961;	
	const int nStep = 4;
	int    PatternID= 0;
	short *pPhaseDA = NULL;
	short *pPhaseDB = NULL;
	unsigned char *ImgD1 = NULL;
	unsigned char *ImgD2 = NULL;
	unsigned char *ImgD3 = NULL;
	unsigned char *ImgD4 = NULL;	
	unsigned char *pMaskDA = NULL;
	unsigned char *pMaskDB = NULL;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	if ( allocBuffer_Cuda(BufferSize, ImgD1, fnName, "ImgD1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgD2, fnName, "ImgD2") == false ||
		 allocBuffer_Cuda(BufferSize, ImgD3, fnName, "ImgD3") == false ||
		 allocBuffer_Cuda(BufferSize, ImgD4, fnName, "ImgD4") == false ||		 
		 allocBuffer_Cuda(BufferSize, pMaskDA, fnName, "pMaskDA") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseDA, fnName, "pPhaseDA") == false ||
		 allocBuffer_Cuda(BufferSize, pMaskDB, fnName, "pMaskDB") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseDB, fnName, "pPhaseDB") == false )
	{
		CudaExceptionClean();
		return false;
	}	 

	bool bSuccess = false;
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);	

	//Phase A
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD1, PtrA1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA1 To ImgD1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD2, PtrA2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA2 To ImgD2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD3, PtrA3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA3 To ImgD3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD4, PtrA4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA4 To ImgD4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	
	
	//Solve Phase A
	ThreadNumber = 192;
	BlockNumber = 961;

	cudaThreadSynchronize();
	//Hostfunction	
	PatternID = PHASE_PATTERN_A;
	ExecCudaSolvePhase4Frames(ImgD1, ImgD2, ImgD3, ImgD4, pPhaseDA, pMaskDA, DeviceDataSize, PatternID, Gamma, NoiseParam, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, DoCudaSolvePhase Error. %s", cudaGetErrorString(err));			
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}
	
	//Phase B
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD1, PtrB1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy PtrB1 To ImgD1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD2, PtrB2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy PtrB2 To ImgD2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD3, PtrB3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy PtrB3 To ImgD3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD4, PtrB4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy PtrB4 To ImgD4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	
	//Solve Phase B
	ThreadNumber = 192;
	BlockNumber = 961;

	cudaThreadSynchronize();
	//Hostfunction	
	PatternID = PHASE_PATTERN_B;
	ExecCudaSolvePhase4Frames(ImgD1, ImgD2, ImgD3, ImgD4, pPhaseDB, pMaskDB, DeviceDataSize, PatternID, Gamma, NoiseParam, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolvePhase4Frames Error. %s", cudaGetErrorString(err));	
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	

	//Release ImgD1, ImgD2, ImgD3, ImgD4
	freeBuffer_Cuda(ImgD1);
	freeBuffer_Cuda(ImgD2);
	freeBuffer_Cuda(ImgD3);
	freeBuffer_Cuda(ImgD4);		

	//PhaseSmooth
	/*
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	if( SysParam.m_PhaseSmoothCudaMode == FN_ENABLE )
	{
		short *pPhaseSmoothSrcD = NULL;
		short *pPhaseSmoothDestD = NULL;
		float *pPhaseSmoothTempSinBufferD = NULL;
		float *pPhaseSmoothTempCosBufferD = NULL;
		unsigned char *pPhaseSmoothTempMaskBufferD = NULL;
		unsigned char *pPhaseSmoothMaskD = NULL;
		const int MaskSize = SysParam.m_PhaseSmoothCudaMaskSize;

		if ( allocBuffer_Cuda(BufferSize, pPhaseSmoothDestD, fnName, "pPhaseSmoothDestD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempSinBufferD, fnName, "pPhaseSmoothTempSinBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempCosBufferD, fnName, "pPhaseSmoothTempCosBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempMaskBufferD, fnName, "pPhaseSmoothTempMaskBufferD") == false )
		{	
			CudaExceptionClean();	
			return false;
		}

		//先將原始資料複製一份到目標位置-Phase A
		pPhaseSmoothSrcD = pPhaseDA;
		pPhaseSmoothMaskD = pMaskDA;
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
		
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));			
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}

		freeBuffer_Cuda(pPhaseDA);//清除原始相位指標		
		pPhaseDA = pPhaseSmoothDestD;//重新導向相位指標
		
		//先將原始資料複製一份到目標位置-Phase B
		pPhaseSmoothSrcD = pPhaseDB;
		pPhaseSmoothMaskD = pMaskDB;
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
		
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));			
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}

		freeBuffer_Cuda(pPhaseDB);//清除原始相位指標		
		pPhaseDB = pPhaseSmoothDestD;//重新導向相位指標

		freeBuffer_Cuda(pPhaseSmoothTempSinBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempCosBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempMaskBufferD);		
	}	
	*/

	//解雙相位, 合併單一相位
	short *pPhaseD = NULL;	
	short *pBasePhaseD = NULL;
	unsigned char *pMaskD = NULL;	
	if( NULL != BasePhasePtr )//相位相減
	{	
		if ( allocBuffer_Cuda(BufferSize, pBasePhaseD, fnName, "pBasePhaseD") == false )
		{
			CudaExceptionClean();
			return false;
		}
		bSuccess = checkCudaErrors(cudaMemcpy(pBasePhaseD, BasePhasePtr, sizeof(short)*DeviceDataSize, cudaMemcpyHostToDevice));		
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy BasePhasePtr To pBasePhaseD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
	}
	if ( allocBuffer_Cuda(BufferSize, pPhaseD, fnName, "pPhaseD") == false ||
		 allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false )
	{
		CudaExceptionClean();	
		return false;
	}

	cudaThreadSynchronize();
	//Host Function
	ExecCudaSolve2PhasePeriod(PA, pPhaseDA, pMaskDA, PB, pPhaseDB, pMaskDB, pBasePhaseD, pMaskD, pPhaseD, DeviceDataSize, NoiseParam.PhaseCombinePeriodMode, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, DoCudaSolve2Phase Error. %s", cudaGetErrorString(err));		
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}
	//Release PhaseA, PhaseB, MaskA, MaskB Buffer
	freeBuffer_Cuda(pMaskDA);
	freeBuffer_Cuda(pPhaseDA);
	freeBuffer_Cuda(pMaskDB);
	freeBuffer_Cuda(pPhaseDB);
	freeBuffer_Cuda(pBasePhaseD);		
	
	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseData, pPhaseD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskMap, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pPhaseD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaPhaseToSpace(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const short *PhasePtr, const float* KPtr, float *DstPtr)//相位乘上K值變成空間數值	
{
#ifdef  CUDA_USE
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaPhaseToSpace";
	if ( CheckPtr3(fnName, PhasePtr, KPtr, DstPtr) == false )
	{	return false; }
	
	//GPU運算
	int ThreadNumber = 192;
	int BlockNumber = 961;		
	short *pPhaseD = NULL;	
	float *pKvalueD = NULL;
	float *pDstD = NULL;
	cudaError_t err;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	if ( allocBuffer_Cuda(BufferSize, pPhaseD, fnName, "pPhaseD") == false ||
		 allocBuffer_Cuda(BufferSize, pKvalueD, fnName, "pKvalueD") == false ||
		 allocBuffer_Cuda(BufferSize, pDstD, fnName, "pDstD") == false )
	{
		CudaExceptionClean();
		return false;
	}	 

	bool bSuccess = false;
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);	
	
	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseD, PhasePtr, sizeof(short)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PhasePtr To pPhaseD. %s", cudaGetErrorString(err));	
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

	bSuccess = checkCudaErrors(cudaMemcpy(pKvalueD, KPtr, sizeof(float)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy KPtr To pKvalueD. %s", cudaGetErrorString(err));	
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaPhaseToSpace(DeviceDataSize, pPhaseD, pKvalueD, pDstD, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{			
		::sprintf(m_ErrorString, "Error, ExecCudaPhaseToSpace Error. %s", cudaGetErrorString(err));			
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	

	bSuccess = checkCudaErrors(cudaMemcpy(DstPtr, pDstD, sizeof(float)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{	
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pDstD To DstPtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	freeBuffer_Cuda(pDstD);
	freeBuffer_Cuda(pPhaseD);
	freeBuffer_Cuda(pKvalueD);
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolvePhase4Frames2Period2Exp(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, double PA, double PB, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap)//4步雙相位2曝光
{
	//return CudaSolvePhase4Frames2Period2Exp_Separate(ImageW, ImageH, ImageStep, PtrA1, PtrA2, PtrA3, PtrA4, PtrB1, PtrB2, PtrB3, PtrB4, PtrC1, PtrC2, PtrC3, PtrC4, PtrD1, PtrD2, PtrD3, PtrD4, PA, PB, Gamma, BasePhasePtr, NoiseParam, pPhaseData, pMaskMap);
	return CudaSolvePhase4Frames2Period2Exp_Combine(ImageW, ImageH, ImageStep, PtrA1, PtrA2, PtrA3, PtrA4, PtrB1, PtrB2, PtrB3, PtrB4, PtrC1, PtrC2, PtrC3, PtrC4, PtrD1, PtrD2, PtrD3, PtrD4, PA, PB, Gamma, BasePhasePtr, NoiseParam, pPhaseData, pMaskMap);	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolvePhase4Frames2Period2Exp_Separate(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, double PA, double PB, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap)//4步雙相位2曝光
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolvePhase4Frames2Period2Exp_Separate";
	if ( CheckPtr18(fnName, PtrA1, PtrA2, PtrA3, PtrA4, PtrB1, PtrB2, PtrB3, PtrB4, pPhaseData, pMaskMap, PtrC1, PtrC2, PtrC3, PtrC4, PtrD1, PtrD2, PtrD3, PtrD4) == false )
	{	return false; }
	
	//GPU運算
	int ThreadNumber = 192;
	int BlockNumber = 961;	
	const int nStep = 4;
	int    PatternID= 0;
	short *pPhaseDA = NULL;
	short *pPhaseDB = NULL;
	short *pPhaseDC = NULL;
	short *pPhaseDD = NULL;
	unsigned char *ImgD1 = NULL;
	unsigned char *ImgD2 = NULL;
	unsigned char *ImgD3 = NULL;
	unsigned char *ImgD4 = NULL;	
	unsigned char *pMaskDA = NULL;
	unsigned char *pMaskDB = NULL;
	unsigned char *pMaskDC = NULL;
	unsigned char *pMaskDD = NULL;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	if ( allocBuffer_Cuda(BufferSize, ImgD1, fnName, "ImgD1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgD2, fnName, "ImgD2") == false ||
		 allocBuffer_Cuda(BufferSize, ImgD3, fnName, "ImgD3") == false ||
		 allocBuffer_Cuda(BufferSize, ImgD4, fnName, "ImgD4") == false ||		 
		 allocBuffer_Cuda(BufferSize, pMaskDA, fnName, "pMaskDA") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseDA, fnName, "pPhaseDA") == false ||
		 allocBuffer_Cuda(BufferSize, pMaskDB, fnName, "pMaskDB") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseDB, fnName, "pPhaseDB") == false || 
		 allocBuffer_Cuda(BufferSize, pMaskDC, fnName, "pMaskDC") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseDC, fnName, "pPhaseDC") == false ||
		 allocBuffer_Cuda(BufferSize, pMaskDD, fnName, "pMaskDD") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseDD, fnName, "pPhaseDD") == false  )
	{
		CudaExceptionClean();
		return false;
	}	 

	bool bSuccess = false;
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);	
	//ThreadNumber = 192;
	//BlockNumber = 961;
	//Phase A
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD1, PtrA1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA1 To ImgD1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD2, PtrA2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA2 To ImgD2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD3, PtrA3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA3 To ImgD3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD4, PtrA4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA4 To ImgD4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	
	//Solve Phase A
	cudaThreadSynchronize();
	//Hostfunction	
	PatternID = PHASE_PATTERN_A;
	ExecCudaSolvePhase4Frames(ImgD1, ImgD2, ImgD3, ImgD4, pPhaseDA, pMaskDA, DeviceDataSize, PatternID, Gamma, NoiseParam, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, DoCudaSolvePhase Error. %s", cudaGetErrorString(err));			
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}
	
	//Phase B
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD1, PtrB1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy PtrB1 To ImgD1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD2, PtrB2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy PtrB2 To ImgD2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD3, PtrB3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy PtrB3 To ImgD3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD4, PtrB4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy PtrB4 To ImgD4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	
	//Solve Phase B
	cudaThreadSynchronize();
	//Hostfunction	
	PatternID = PHASE_PATTERN_B;
	ExecCudaSolvePhase4Frames(ImgD1, ImgD2, ImgD3, ImgD4, pPhaseDB, pMaskDB, DeviceDataSize, PatternID, Gamma, NoiseParam, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolvePhase4Frames Error. %s", cudaGetErrorString(err));	
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	

	//Phase A - Exposure-2
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD1, PtrC1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrC1 To ImgD1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD2, PtrC2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrC2 To ImgD2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD3, PtrC3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrC3 To ImgD3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD4, PtrC4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrC4 To ImgD4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	
	//Solve Phase A
	cudaThreadSynchronize();
	//Hostfunction	
	PatternID = PHASE_PATTERN_A;
	ExecCudaSolvePhase4Frames(ImgD1, ImgD2, ImgD3, ImgD4, pPhaseDC, pMaskDC, DeviceDataSize, PatternID, Gamma, NoiseParam, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, DoCudaSolvePhase Error. %s", cudaGetErrorString(err));			
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}
	
	//Phase B - Exposure-2
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD1, PtrD1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy PtrD1 To ImgD1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD2, PtrD2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy PtrD2 To ImgD2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD3, PtrD3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy PtrD3 To ImgD3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgD4, PtrD4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy PtrD4 To ImgD4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	
	//Solve Phase B
	cudaThreadSynchronize();
	//Hostfunction	
	PatternID = PHASE_PATTERN_B;
	ExecCudaSolvePhase4Frames(ImgD1, ImgD2, ImgD3, ImgD4, pPhaseDD, pMaskDD, DeviceDataSize, PatternID, Gamma, NoiseParam, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolvePhase4Frames Error. %s", cudaGetErrorString(err));	
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}

	//Release ImgD1, ImgD2, ImgD3, ImgD4
	freeBuffer_Cuda(ImgD1);
	freeBuffer_Cuda(ImgD2);
	freeBuffer_Cuda(ImgD3);
	freeBuffer_Cuda(ImgD4);		

	//PhaseSmooth
	/*
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	if( SysParam.m_PhaseSmoothCudaMode == FN_ENABLE )
	{
		short *pPhaseSmoothSrcD = NULL;
		short *pPhaseSmoothDestD = NULL;
		float *pPhaseSmoothTempSinBufferD = NULL;
		float *pPhaseSmoothTempCosBufferD = NULL;
		unsigned char *pPhaseSmoothTempMaskBufferD = NULL;
		unsigned char *pPhaseSmoothMaskD = NULL;
		const int MaskSize = SysParam.m_PhaseSmoothCudaMaskSize;

		if ( allocBuffer_Cuda(BufferSize, pPhaseSmoothDestD, fnName, "pPhaseSmoothDestD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempSinBufferD, fnName, "pPhaseSmoothTempSinBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempCosBufferD, fnName, "pPhaseSmoothTempCosBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempMaskBufferD, fnName, "pPhaseSmoothTempMaskBufferD") == false )
		{	
			CudaExceptionClean();	
			return false;
		}

		//先將原始資料複製一份到目標位置-Phase A
		pPhaseSmoothSrcD = pPhaseDA;
		pPhaseSmoothMaskD = pMaskDA;
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
		
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}

		freeBuffer_Cuda(pPhaseDA);//清除原始相位指標		
		pPhaseDA = pPhaseSmoothDestD;//重新導向相位指標
		
		//先將原始資料複製一份到目標位置-Phase B
		pPhaseSmoothSrcD = pPhaseDB;
		pPhaseSmoothMaskD = pMaskDB;
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
		
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}

		freeBuffer_Cuda(pPhaseDB);//清除原始相位指標		
		pPhaseDB = pPhaseSmoothDestD;//重新導向相位指標

		freeBuffer_Cuda(pPhaseSmoothTempSinBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempCosBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempMaskBufferD);		
	}	
	*/

	//解雙相位, 合併單一相位
	short *pPhaseD = NULL;	
	short *pPhaseDAB = NULL;	
	short *pPhaseDCD = NULL;	
	short *pBasePhaseD = NULL;
	unsigned char *pMaskD = NULL;	
	unsigned char *pMaskDAB = NULL;	
	unsigned char *pMaskDCD = NULL;	
	if( NULL != BasePhasePtr )//相位相減
	{	
		if ( allocBuffer_Cuda(BufferSize, pBasePhaseD, fnName, "pBasePhaseD") == false )
		{
			CudaExceptionClean();
			return false;
		}
		bSuccess = checkCudaErrors(cudaMemcpy(pBasePhaseD, BasePhasePtr, sizeof(short)*DeviceDataSize, cudaMemcpyHostToDevice));		
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy BasePhasePtr To pBasePhaseD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
	}
	if ( allocBuffer_Cuda(BufferSize, pPhaseD, fnName, "pPhaseD") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseDAB, fnName, "pPhaseDAB") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseDCD, fnName, "pPhaseDCD") == false ||
		 allocBuffer_Cuda(BufferSize, pMaskDAB, fnName, "pMaskDAB") == false || 
		 allocBuffer_Cuda(BufferSize, pMaskDCD, fnName, "pMaskDCD") == false || 
		 allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false )
	{
		CudaExceptionClean();	
		return false;
	}

	cudaThreadSynchronize();
	//Host Function
	ExecCudaSolve2PhasePeriod(PA, pPhaseDA, pMaskDA, PB, pPhaseDB, pMaskDB, pBasePhaseD, pMaskDAB, pPhaseDAB, DeviceDataSize, NoiseParam.PhaseCombinePeriodMode, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, DoCudaSolve2Phase Error. %s", cudaGetErrorString(err));	
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}

	cudaThreadSynchronize();
	//Host Function
	ExecCudaSolve2PhasePeriod(PA, pPhaseDC, pMaskDC, PB, pPhaseDD, pMaskDD, pBasePhaseD, pMaskDCD, pPhaseDCD, DeviceDataSize, NoiseParam.PhaseCombinePeriodMode, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, DoCudaSolve2Phase Error. %s", cudaGetErrorString(err));	
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}
	//Release PhaseA, PhaseB, MaskA, MaskB Buffer
	freeBuffer_Cuda(pMaskDA);
	freeBuffer_Cuda(pPhaseDA);
	freeBuffer_Cuda(pMaskDB);
	freeBuffer_Cuda(pPhaseDB);
	freeBuffer_Cuda(pMaskDC);
	freeBuffer_Cuda(pPhaseDC);
	freeBuffer_Cuda(pMaskDD);
	freeBuffer_Cuda(pPhaseDD);
	freeBuffer_Cuda(pBasePhaseD);		

	//Merage 2Exp Phase 
	cudaThreadSynchronize();
	//Host Function
	ExecCudaMerge2Exp2Phase(pMaskDAB, pPhaseDAB, pMaskDCD, pPhaseDCD, pMaskD, pPhaseD, DeviceDataSize, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaMerge2Exp2Phase Error. %s", cudaGetErrorString(err));			
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}
	freeBuffer_Cuda(pMaskDAB);
	freeBuffer_Cuda(pPhaseDAB);
	freeBuffer_Cuda(pMaskDCD);
	freeBuffer_Cuda(pPhaseDCD);
	
	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseData, pPhaseD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskMap, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pPhaseD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolvePhase4Frames2Period2Exp_Combine(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const unsigned char *PtrA1, const unsigned char *PtrA2, const unsigned char *PtrA3, const unsigned char *PtrA4, const unsigned char *PtrB1, const unsigned char *PtrB2, const unsigned char *PtrB3, const unsigned char *PtrB4, const unsigned char *PtrC1, const unsigned char *PtrC2, const unsigned char *PtrC3, const unsigned char *PtrC4, const unsigned char *PtrD1, const unsigned char *PtrD2, const unsigned char *PtrD3, const unsigned char *PtrD4, double PA, double PB, float Gamma, const short * BasePhasePtr, const TPhaseNoiseParam &NoiseParam, short *pPhaseData, unsigned char *pMaskMap)//4步雙相位2曝光
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolvePhase4Frames2Period2Exp_Combine";
	if ( CheckPtr18(fnName, PtrA1, PtrA2, PtrA3, PtrA4, PtrB1, PtrB2, PtrB3, PtrB4, pPhaseData, pMaskMap, PtrC1, PtrC2, PtrC3, PtrC4, PtrD1, PtrD2, PtrD3, PtrD4) == false )
	{	return false; }
	
	//GPU運算
	int ThreadNumber = 192;
	int BlockNumber = 961;	
	bool bSuccess = false;
	const int nStep = 4;
	short *pPhaseD = NULL;	
	short *pPhaseDAB = NULL;	
	short *pPhaseDCD = NULL;	
	short *pBasePhaseD = NULL;
	unsigned char *ImgDA1 = NULL;
	unsigned char *ImgDA2 = NULL;
	unsigned char *ImgDA3 = NULL;
	unsigned char *ImgDA4 = NULL;	
	unsigned char *ImgDB1 = NULL;
	unsigned char *ImgDB2 = NULL;
	unsigned char *ImgDB3 = NULL;
	unsigned char *ImgDB4 = NULL;	
	unsigned char *pMaskD = NULL;	
	unsigned char *pMaskDAB = NULL;	
	unsigned char *pMaskDCD = NULL;	
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);	
	if( NULL != BasePhasePtr )//相位相減
	{	
		if ( allocBuffer_Cuda(BufferSize, pBasePhaseD, fnName, "pBasePhaseD") == false )
		{
			CudaExceptionClean();
			return false;
		}
		bSuccess = checkCudaErrors(cudaMemcpy(pBasePhaseD, BasePhasePtr, sizeof(short)*DeviceDataSize, cudaMemcpyHostToDevice));		
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy BasePhasePtr To pBasePhaseD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
	}
	if ( allocBuffer_Cuda(BufferSize, ImgDA1, fnName, "ImgDA1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA2, fnName, "ImgDA2") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA3, fnName, "ImgDA3") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDA4, fnName, "ImgDA4") == false ||		 		 
		 allocBuffer_Cuda(BufferSize, ImgDB1, fnName, "ImgDB1") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB2, fnName, "ImgDB2") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB3, fnName, "ImgDB3") == false ||
		 allocBuffer_Cuda(BufferSize, ImgDB4, fnName, "ImgDB4") == false ||		 
		 allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pMaskDAB, fnName, "pMaskDAB") == false ||
		 allocBuffer_Cuda(BufferSize, pMaskDCD, fnName, "pMaskDCD") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseD, fnName, "pPhaseD") == false || 
		 allocBuffer_Cuda(BufferSize, pPhaseDAB, fnName, "pPhaseDAB") == false ||
		 allocBuffer_Cuda(BufferSize, pPhaseDCD, fnName, "pPhaseDCD") == false )
	{
		CudaExceptionClean();		
		return false;
	}	 	

	//Phase A
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA1, PtrA1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA1 To ImgDA1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA2, PtrA2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA2 To ImgDA2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA3, PtrA3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA3 To ImgDA3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA4, PtrA4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrA4 To ImgDA4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

	//Phase B
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB1, PtrB1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB1 To ImgDB1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB2, PtrB2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB2 To ImgDB2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB3, PtrB3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB3 To ImgDB3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB4, PtrB4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrB4 To ImgDB4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	
	//Solve Phase A
	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaSolvePhase4Frames2Phase(ImgDA1, ImgDA2, ImgDA3, ImgDA4, ImgDB1, ImgDB2, ImgDB3, ImgDB4, pBasePhaseD, PA, PB, DeviceDataSize, Gamma, NoiseParam, pMaskDAB, pPhaseDAB, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolvePhase4Frames2Phase Error. %s", cudaGetErrorString(err));		
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	
	
	//Phase A - Exposure 2
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA1, PtrC1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrC1 To ImgDA1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA2, PtrC2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrC2 To ImgDA2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA3, PtrC3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrC3 To ImgDA3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDA4, PtrC4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrC4 To ImgDA4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}

	//Phase B - Exposure 2
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB1, PtrD1, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrD1 To ImgDB1. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB2, PtrD2, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrD2 To ImgDB2. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB3, PtrD3, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrD3 To ImgDB3. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(ImgDB4, PtrD4, sizeof(unsigned char)*DeviceDataSize , cudaMemcpyHostToDevice));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "cudaMemcpy PtrD4 To ImgDB4. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}	
	
	//Solve Phase A
	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaSolvePhase4Frames2Phase(ImgDA1, ImgDA2, ImgDA3, ImgDA4, ImgDB1, ImgDB2, ImgDB3, ImgDB4, pBasePhaseD, PA, PB, DeviceDataSize, Gamma, NoiseParam, pMaskDCD, pPhaseDCD, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolvePhase4Frames2Phase Error. %s", cudaGetErrorString(err));		
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	
	
	//Release ImgD1, ImgD2, ImgD3, ImgD4
	freeBuffer_Cuda(ImgDA1);
	freeBuffer_Cuda(ImgDA2);
	freeBuffer_Cuda(ImgDA3);
	freeBuffer_Cuda(ImgDA4);
	freeBuffer_Cuda(ImgDB1);
	freeBuffer_Cuda(ImgDB2);
	freeBuffer_Cuda(ImgDB3);
	freeBuffer_Cuda(ImgDB4);		
	freeBuffer_Cuda(pBasePhaseD);			

	//PhaseSmooth
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	if( SysParam.m_PhaseSmoothCudaMode == FN_ENABLE )
	{
		short *pPhaseSmoothSrcD = NULL;
		short *pPhaseSmoothDestD = NULL;
		float *pPhaseSmoothTempSinBufferD = NULL;
		float *pPhaseSmoothTempCosBufferD = NULL;
		unsigned char *pPhaseSmoothTempMaskBufferD = NULL;
		unsigned char *pPhaseSmoothMaskD = NULL;
		const int MaskSize = SysParam.m_PhaseSmoothCudaMaskSize;

		if ( allocBuffer_Cuda(BufferSize, pPhaseSmoothDestD, fnName, "pPhaseSmoothDestD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempSinBufferD, fnName, "pPhaseSmoothTempSinBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempCosBufferD, fnName, "pPhaseSmoothTempCosBufferD") == false ||
			 allocBuffer_Cuda(BufferSize, pPhaseSmoothTempMaskBufferD, fnName, "pPhaseSmoothTempMaskBufferD") == false )
		{	
			CudaExceptionClean();
			return false;
		}

		//先將原始資料複製一份到目標位置-Phase M
		pPhaseSmoothSrcD = pPhaseDAB;
		pPhaseSmoothMaskD = pMaskDAB;
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
		
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		cudaMemcpy(pPhaseSmoothSrcD, pPhaseSmoothDestD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice);
		//SetCudaExceptionCode_MemCopy();
		
		//先將原始資料複製一份到目標位置-Phase M-Exposure 2
		pPhaseSmoothSrcD = pPhaseDCD;
		pPhaseSmoothMaskD = pMaskDCD;
		bSuccess = checkCudaErrors(cudaMemcpy(pPhaseSmoothDestD, pPhaseSmoothSrcD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice));
		if( !bSuccess )
		{
			cudaError_t err = cudaGetLastError();
			::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseSmoothSrcD To pPhaseSmoothDestD. %s", cudaGetErrorString(err));
			SetCudaExceptionCode_MemCopy();
			CudaExceptionClean();
			return false;		
		}
		
		cudaThreadSynchronize();
		//PhaseSmooth
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothRows(pPhaseSmoothSrcD, pPhaseSmoothMaskD, pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, ImageStep, ImageH, MaskSize, IsUseTexture);		//ROW
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothRows Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }
		DoCudaPhaseSmoothColumns(pPhaseSmoothTempSinBufferD, pPhaseSmoothTempCosBufferD, pPhaseSmoothTempMaskBufferD, pPhaseSmoothDestD, pPhaseSmoothMaskD, ImageStep, ImageH, MaskSize, IsUseTexture);	//COL
		cudaThreadSynchronize();
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, DoCudaPhaseSmoothColumns Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			CudaExceptionClean();
			return false;		
		}
		cudaMemcpy(pPhaseSmoothSrcD, pPhaseSmoothDestD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToDevice);
		//SetCudaExceptionCode_MemCopy();

		freeBuffer_Cuda(pPhaseSmoothDestD);
		freeBuffer_Cuda(pPhaseSmoothTempSinBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempCosBufferD);
		freeBuffer_Cuda(pPhaseSmoothTempMaskBufferD);		
	}
	
	//Merge 2 Exposure 2Phase	
	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaMerge2Exp2Phase(pMaskDAB, pPhaseDAB, pMaskDCD, pPhaseDCD, pMaskD, pPhaseD, DeviceDataSize, IsUseTexture, ThreadNumber, BlockNumber);
	cudaThreadSynchronize();//等到device執行完
	err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaMerge2Exp2Phase Error. %s", cudaGetErrorString(err));		
		SetCudaExceptionCode_ExeFunc();
		CudaExceptionClean();
		return false;		
	}	
	freeBuffer_Cuda(pMaskDAB);
	freeBuffer_Cuda(pPhaseDAB);
	freeBuffer_Cuda(pMaskDCD);
	freeBuffer_Cuda(pPhaseDCD);

	bSuccess = checkCudaErrors(cudaMemcpy(pPhaseData, pPhaseD, sizeof(short)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pPhaseD To pPhaseData. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(pMaskMap, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To pMaskMap. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pPhaseD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolveSpace4Frames2Period1Cast(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber, int BlockNumber)//1投光計算出高度值
{
	LockCudaFunc();
	const bool IsOK = CudaSolveSpace4Frames2Period1CastFn(ImageW, ImageH, ImageStep, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber);
	UnlockCudaFunc();
	return IsOK;
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolveSpace4Frames2Period1CastFn(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber, int BlockNumber)//1投光計算出高度值
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolveSpace4Frames2Period1Cast";	
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam) == false )
	{	return false; }		
	
	//GPU運算	
	bool bSuccess = false;
	const int nStep = 4;	
	float *pSpaceD = NULL;		
	TCastParam CastParamD;
	PCastParam CastParamPtrD = NULL;
	unsigned char *pMaskD = NULL;	
	const int DecodeMode = CastParam.DecodeMode;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);

	if ( allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pSpaceD, fnName, "pSpaceD") == false ||		 		 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam, CastParamD, CastParamPtrD) == false )
	{
		freeBuffer_Cuda(pMaskD);
		freeBuffer_Cuda(pSpaceD);
		freeBuffer_Cuda(CastParamPtrD);
		FreeCastParamBuffer(CastParamD);
		return false;
	}

	//Solve Phase A
	//ThreadNumber = 192;
	//BlockNumber = 961;

	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaSolveSpace4Frames2Period1Cast(CastParamPtrD, DeviceDataSize, NoiseParam, pMaskD, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber, DecodeMode);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolveSpace4Frames2Period1Cast Error. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_ExeFunc();
		freeBuffer_Cuda(pMaskD);
		freeBuffer_Cuda(pSpaceD);
		freeBuffer_Cuda(CastParamPtrD);
		FreeCastParamBuffer(CastParamD);
		return false;		
	}
	if ( NoiseParam.SpaceMultiCastPatchSize > 0 ) 
	{	
	}
	//Copy Back To Cast Param
	if ( CloneCastParamResultToHost(CastParamD, CastParam) == false )
	{
		CudaExceptionClean();
		return false;
	}
	freeBuffer_Cuda(CastParamPtrD);		
	FreeCastParamBuffer(CastParamD);		
	
	if ( CudaMedianFilter_Internal(ImageW, ImageH, ImageStep, pMaskD, NoiseParam, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber) == false )
	{
		CudaExceptionClean();
		return false;
	}

	bSuccess = checkCudaErrors(cudaMemcpy(SpacePtr, pSpaceD, sizeof(float)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pSpaceD To SpacePtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(MaskPtr, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To MaskPtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();

	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pSpaceD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolveSpace4Frames2Period2Cast(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber, int BlockNumber)//2投光計算出高度值
{
	LockCudaFunc();
	const bool IsOK = CudaSolveSpace4Frames2Period2CastFn(ImageW, ImageH, ImageStep, CastParam1, CastParam2, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber);
	UnlockCudaFunc();
	return IsOK;
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolveSpace4Frames2Period2CastFn(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber, int BlockNumber)//2投光計算出高度值
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolveSpace4Frames2Period2Cast";
	if ( CastParam1.ImageCount != CastParam2.ImageCount ) 
	{
		::sprintf(this->m_ErrorString, "Error, Cuda CastParam[1, 2] Image Count Difference");
		SetCudaExceptionCode_Param();
		return false;
	}
	if ( NULL==MaskPtr || NULL==SpacePtr )
	{
		::sprintf(this->m_ErrorString, "Error, Cuda Solve 3D MaskPtr or SpacePtr is NULL");
		SetCudaExceptionCode_Param();
		return false;
	}
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam1) == false )
	{	return false; }		
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam2) == false )
	{	return false; }		
	
	//GPU運算	
	bool bSuccess = false;	
	float *pSpaceD = NULL;		
	TCastParam CastParamD1;
	TCastParam CastParamD2;
	PCastParam CastParamPtrD1 = NULL;
	PCastParam CastParamPtrD2 = NULL;	
	unsigned char *pMaskD = NULL;	
	const int DecodeMode = CastParam1.DecodeMode;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);	

	if ( allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pSpaceD, fnName, "pSpaceD") == false ||		 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam1, CastParamD1, CastParamPtrD1) == false || 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam2, CastParamD2, CastParamPtrD2) == false )
	{
		freeBuffer_Cuda(pMaskD);
		freeBuffer_Cuda(pSpaceD);
		freeBuffer_Cuda(CastParamPtrD1);
		freeBuffer_Cuda(CastParamPtrD2);
		FreeCastParamBuffer(CastParamD1);
		FreeCastParamBuffer(CastParamD2);
		return false;
	}

	//Solve Phase A
//	ThreadNumber = 192;
//	BlockNumber = 961;

	cudaThreadSynchronize();
	//Hostfunction		
	ExecCudaSolveSpace4Frames2Period2Cast(CastParamPtrD1, CastParamPtrD2, DeviceDataSize, NoiseParam, pMaskD, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber, DecodeMode);	
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolveSpace4Frames2Period2Cast Error. %s", cudaGetErrorString(err));		
		SetCudaExceptionCode_ExeFunc();
		freeBuffer_Cuda(pMaskD);
		freeBuffer_Cuda(pSpaceD);
		freeBuffer_Cuda(CastParamPtrD1);
		freeBuffer_Cuda(CastParamPtrD2);
		FreeCastParamBuffer(CastParamD1);
		FreeCastParamBuffer(CastParamD2);
		return false;		
	}

	if ( NoiseParam.SpaceMultiCastPatchSize > 0 ) 
	{
		cudaThreadSynchronize();
		//Hostfunction	
		ExecCudaMergeSpace4Frames2Period2Cast(CastParamPtrD1, CastParamPtrD2, DeviceDataSize, NoiseParam, pMaskD, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber);
		cudaThreadSynchronize();//等到device執行完
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, ExecCudaMergeSpace4Frames2Period2Cast Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			freeBuffer_Cuda(pMaskD);
			freeBuffer_Cuda(pSpaceD);
			freeBuffer_Cuda(CastParamPtrD1);
			freeBuffer_Cuda(CastParamPtrD2);
			FreeCastParamBuffer(CastParamD1);
			FreeCastParamBuffer(CastParamD2);				
			return false;		
		}		
	}	
	//Copy Back To Cast Param
	if ( CloneCastParamResultToHost(CastParamD1, CastParam1) == false ||	
	     CloneCastParamResultToHost(CastParamD2, CastParam2) == false )
	{
		CudaExceptionClean();
		return false;
	}
	freeBuffer_Cuda(CastParamPtrD1);
	freeBuffer_Cuda(CastParamPtrD2);
	FreeCastParamBuffer(CastParamD1);
	FreeCastParamBuffer(CastParamD2);	
	
	if ( CudaMedianFilter_Internal(ImageW, ImageH, ImageStep, pMaskD, NoiseParam, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber) == false )
	{
		CudaExceptionClean();
		return false;
	}

	bSuccess = checkCudaErrors(cudaMemcpy(SpacePtr, pSpaceD, sizeof(float)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pSpaceD To SpacePtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(MaskPtr, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To MaskPtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pSpaceD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolveSpace4Frames2Period3Cast(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber, int BlockNumber)//3投光計算出高度值
{
	LockCudaFunc();
	const bool IsOK = CudaSolveSpace4Frames2Period3CastFn(ImageW, ImageH, ImageStep, CastParam1, CastParam2, CastParam3, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber);
	UnlockCudaFunc();
	return IsOK;
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolveSpace4Frames2Period3CastFn(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber, int BlockNumber)//3投光計算出高度值	
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolveSpace4Frames2Period3Cast";
	if ( CastParam1.ImageCount!=CastParam2.ImageCount || CastParam1.ImageCount!=CastParam3.ImageCount ) 
	{
		::sprintf(this->m_ErrorString, "Error, Cuda CastParam[1, 2, 3] Image Count Difference");
		SetCudaExceptionCode_Param();
		return false;
	}
	if ( NULL==MaskPtr || NULL==SpacePtr )
	{
		::sprintf(this->m_ErrorString, "Error, Cuda Solve 3D MaskPtr or SpacePtr is NULL");
		SetCudaExceptionCode_Param();
		return false;
	}

	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam1) == false )
	{	return false; }		
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam2) == false )
	{	return false; }		
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam3) == false )
	{	return false; }		
	
	//GPU運算	
	bool bSuccess = false;	
	float *pSpaceD = NULL;		
	TCastParam CastParamD1;
	TCastParam CastParamD2;
	TCastParam CastParamD3;
	PCastParam CastParamPtrD1 = NULL;
	PCastParam CastParamPtrD2 = NULL;	
	PCastParam CastParamPtrD3 = NULL;
	unsigned char *pMaskD = NULL;	
	const int DecodeMode = CastParam1.DecodeMode;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);

	if ( allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pSpaceD, fnName, "pSpaceD") == false ||		 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam1, CastParamD1, CastParamPtrD1) == false || 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam2, CastParamD2, CastParamPtrD2) == false || 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam3, CastParamD3, CastParamPtrD3) == false )		
	{
		freeBuffer_Cuda(pMaskD);
		freeBuffer_Cuda(pSpaceD);
		freeBuffer_Cuda(CastParamPtrD1);
		freeBuffer_Cuda(CastParamPtrD2);
		freeBuffer_Cuda(CastParamPtrD3);
		FreeCastParamBuffer(CastParamD1);
		FreeCastParamBuffer(CastParamD2);
		FreeCastParamBuffer(CastParamD3);		
		return false;
	} 	
	//
	//Solve Phase A
	//ThreadNumber = 192;
	//BlockNumber = 961;
	
	//CastParamx3.Cast1 = 
	cudaThreadSynchronize();
	//Hostfunction		
	ExecCudaSolveSpace4Frames2Period3Cast(CastParamPtrD1, CastParamPtrD2, CastParamPtrD3, DeviceDataSize, NoiseParam, pMaskD, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber, DecodeMode);	
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolveSpace4Frames2Period3Cast Error. %s", cudaGetErrorString(err));	
		SetCudaExceptionCode_ExeFunc();
		freeBuffer_Cuda(pMaskD);
		freeBuffer_Cuda(pSpaceD);
		freeBuffer_Cuda(CastParamPtrD1);
		freeBuffer_Cuda(CastParamPtrD2);
		freeBuffer_Cuda(CastParamPtrD3);
		FreeCastParamBuffer(CastParamD1);
		FreeCastParamBuffer(CastParamD2);
		FreeCastParamBuffer(CastParamD3);		
		return false;		
	}

	if ( NoiseParam.SpaceMultiCastPatchSize > 0 ) 
	{
		cudaThreadSynchronize();
		//Hostfunction	
		ExecCudaMergeSpace4Frames2Period3Cast(CastParamPtrD1, CastParamPtrD2, CastParamPtrD3, DeviceDataSize, NoiseParam, pMaskD, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber);
		cudaThreadSynchronize();//等到device執行完
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, ExecCudaMergeSpace4Frames2Period3Cast Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			freeBuffer_Cuda(pMaskD);
			freeBuffer_Cuda(pSpaceD);
			freeBuffer_Cuda(CastParamPtrD1);
			freeBuffer_Cuda(CastParamPtrD2);
			freeBuffer_Cuda(CastParamPtrD3);
			FreeCastParamBuffer(CastParamD1);
			FreeCastParamBuffer(CastParamD2);
			FreeCastParamBuffer(CastParamD3);			
			return false;		
		}		
	}
	//Copy Back To Cast Param
	if ( CloneCastParamResultToHost(CastParamD1, CastParam1) == false ||	
	     CloneCastParamResultToHost(CastParamD2, CastParam2) == false ||
		 CloneCastParamResultToHost(CastParamD3, CastParam3) == false )
	{
		CudaExceptionClean();
		return false;
	}
	freeBuffer_Cuda(CastParamPtrD1);
	freeBuffer_Cuda(CastParamPtrD2);
	freeBuffer_Cuda(CastParamPtrD3);	
	FreeCastParamBuffer(CastParamD1);
	FreeCastParamBuffer(CastParamD2);
	FreeCastParamBuffer(CastParamD3);

	if ( CudaMedianFilter_Internal(ImageW, ImageH, ImageStep, pMaskD, NoiseParam, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber) == false )
	{
		CudaExceptionClean();
		return false;
	}

	bSuccess = checkCudaErrors(cudaMemcpy(SpacePtr, pSpaceD, sizeof(float)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pSpaceD To SpacePtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(MaskPtr, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To MaskPtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pSpaceD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolveSpace4Frames2Period4Cast(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber, int BlockNumber)//4投光計算出高度值
{
	LockCudaFunc();
	const bool IsOK = CudaSolveSpace4Frames2Period4CastFn(ImageW, ImageH, ImageStep, CastParam1, CastParam2, CastParam3, CastParam4, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber);
	UnlockCudaFunc();
	return IsOK;
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolveSpace4Frames2Period4CastFn(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber, int BlockNumber)//4投光計算出高度值
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolveSpace4Frames2Period4Cast";
	if ( CastParam1.ImageCount!=CastParam2.ImageCount || CastParam1.ImageCount!=CastParam3.ImageCount || CastParam1.ImageCount!=CastParam4.ImageCount ) 
	{
		::sprintf(this->m_ErrorString, "Error, Cuda CastParam[1, 2, 3, 4] Image Count Difference");
		SetCudaExceptionCode_Param();
		return false;
	}
	if ( NULL==MaskPtr || NULL==SpacePtr )
	{
		::sprintf(this->m_ErrorString, "Error, Cuda Solve 3D MaskPtr or SpacePtr is NULL");
		SetCudaExceptionCode_Param();
		return false;
	}
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam1) == false )
	{	return false; }		
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam2) == false )
	{	return false; }		
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam3) == false )
	{	return false; }		
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam4) == false )
	{	return false; }		
	
	//GPU運算	
	bool bSuccess = false;	
	float *pSpaceD = NULL;	
	TCastParam CastParamD1;
	TCastParam CastParamD2;
	TCastParam CastParamD3;
	TCastParam CastParamD4;
	PCastParam CastParamPtrD1 = NULL;
	PCastParam CastParamPtrD2 = NULL;	
	PCastParam CastParamPtrD3 = NULL;	
	PCastParam CastParamPtrD4 = NULL;	
	unsigned char *pMaskD = NULL;	
	const int DecodeMode = CastParam1.DecodeMode;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);	

	if ( allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pSpaceD, fnName, "pSpaceD") == false ||		 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam1, CastParamD1, CastParamPtrD1) == false || 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam2, CastParamD2, CastParamPtrD2) == false || 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam3, CastParamD3, CastParamPtrD3) == false || 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam4, CastParamD4, CastParamPtrD4) == false )
	{
		freeBuffer_Cuda(pMaskD);
		freeBuffer_Cuda(pSpaceD);		
		freeBuffer_Cuda(CastParamPtrD1);
		freeBuffer_Cuda(CastParamPtrD2);
		freeBuffer_Cuda(CastParamPtrD3);
		freeBuffer_Cuda(CastParamPtrD4);
		FreeCastParamBuffer(CastParamD1);
		FreeCastParamBuffer(CastParamD2);
		FreeCastParamBuffer(CastParamD3);
		FreeCastParamBuffer(CastParamD4);
		return false;
	} 		

	//Solve Phase A
	//ThreadNumber = 192;
	//BlockNumber = 961;
	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaSolveSpace4Frames2Period4Cast(CastParamPtrD1, CastParamPtrD2, CastParamPtrD3, CastParamPtrD4, DeviceDataSize, NoiseParam, pMaskD, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber, DecodeMode);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolveSpace4Frames2Period4Cast Error. %s", cudaGetErrorString(err));	
		SetCudaExceptionCode_ExeFunc();
		freeBuffer_Cuda(pMaskD);
		freeBuffer_Cuda(pSpaceD);		
		freeBuffer_Cuda(CastParamPtrD1);
		freeBuffer_Cuda(CastParamPtrD2);
		freeBuffer_Cuda(CastParamPtrD3);
		freeBuffer_Cuda(CastParamPtrD4);
		FreeCastParamBuffer(CastParamD1);
		FreeCastParamBuffer(CastParamD2);
		FreeCastParamBuffer(CastParamD3);
		FreeCastParamBuffer(CastParamD4);		
		return false;		
	}

	if ( NoiseParam.SpaceMultiCastPatchSize > 0 ) 
	{
		cudaThreadSynchronize();
		//Hostfunction	
		ExecCudaMergeSpace4Frames2Period4Cast(CastParamPtrD1, CastParamPtrD2, CastParamPtrD3, CastParamPtrD4, DeviceDataSize, NoiseParam, pMaskD, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber);
		cudaThreadSynchronize();//等到device執行完
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, ExecCudaMergeSpace4Frames2Period4Cast Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			freeBuffer_Cuda(pMaskD);
			freeBuffer_Cuda(pSpaceD);		
			freeBuffer_Cuda(CastParamPtrD1);
			freeBuffer_Cuda(CastParamPtrD2);
			freeBuffer_Cuda(CastParamPtrD3);
			freeBuffer_Cuda(CastParamPtrD4);
			FreeCastParamBuffer(CastParamD1);
			FreeCastParamBuffer(CastParamD2);
			FreeCastParamBuffer(CastParamD3);
			FreeCastParamBuffer(CastParamD4);			
			return false;		
		}

	#ifdef _CUDA_USE_DEBUG_PARAM
		cudaThreadSynchronize();
		::cudaMemset(pSpaceD, 0x00, sizeof(float)*1000);
		::cudaMemset(pMaskD, 0x00, sizeof(unsigned char)*1000);					
		ExecCudaCopyDebugParam(pMaskD, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber);
		cudaThreadSynchronize();//等到device執行完
		err = cudaGetLastError();
		//SetCudaExceptionCode_ExeFunc();
	#endif//_CUDA_USE_DEBUG_PARAM
	}	
	//Copy Back To Cast Param
	if ( CloneCastParamResultToHost(CastParamD1, CastParam1) == false ||	
	     CloneCastParamResultToHost(CastParamD2, CastParam2) == false ||
		 CloneCastParamResultToHost(CastParamD3, CastParam3) == false ||
		 CloneCastParamResultToHost(CastParamD4, CastParam4) == false )
	{
		CudaExceptionClean();
		return false;
	}
	freeBuffer_Cuda(CastParamPtrD1);
	freeBuffer_Cuda(CastParamPtrD2);
	freeBuffer_Cuda(CastParamPtrD3);
	freeBuffer_Cuda(CastParamPtrD4);
	FreeCastParamBuffer(CastParamD1);
	FreeCastParamBuffer(CastParamD2);
	FreeCastParamBuffer(CastParamD3);
	FreeCastParamBuffer(CastParamD4);
	
	if ( CudaMedianFilter_Internal(ImageW, ImageH, ImageStep, pMaskD, NoiseParam, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber) == false )
	{
		CudaExceptionClean();
		return false;
	}
	
	bSuccess = checkCudaErrors(cudaMemcpy(SpacePtr, pSpaceD, sizeof(float)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pSpaceD To SpacePtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(MaskPtr, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To MaskPtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pSpaceD);	
	
	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolveSpace4Frames2Period1Cast2Exp(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber, int BlockNumber)//1投光2曝光計算出高度值
{
	LockCudaFunc();
	const bool IsOK = CudaSolveSpace4Frames2Period1Cast2ExpFn(ImageW, ImageH, ImageStep, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber);
	UnlockCudaFunc();
	return IsOK;
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolveSpace4Frames2Period1Cast2ExpFn(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber, int BlockNumber)//1投光2曝光計算出高度值
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolveSpace4Frames2Period1Cast2Exp";	
	if ( CheckCastParam2Exp(fnName, CastParam) == false )
	{	return false; }
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam) == false )
	{	return false; }		
	
	//GPU運算	
	bool bSuccess = false;
	const int nStep = 4;	
	float *pSpaceD = NULL;		
	TCastParam CastParamD;
	PCastParam CastParamPtrD = NULL;
	unsigned char *pMaskD = NULL;	
	const int DecodeMode = CastParam.DecodeMode;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);

	if ( allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pSpaceD, fnName, "pSpaceD") == false ||		 		 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam, CastParamD, CastParamPtrD) == false )
	{
		freeBuffer_Cuda(pMaskD);
		freeBuffer_Cuda(pSpaceD);
		freeBuffer_Cuda(CastParamPtrD);		
		FreeCastParamBuffer(CastParamD);		
		return false;
	}

	//Solve Phase A
	//ThreadNumber = 192;
	//BlockNumber = 961;

	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaSolveSpace4Frames2Period1Cast2Exp(CastParamPtrD, DeviceDataSize, NoiseParam, pMaskD, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber, DecodeMode);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolveSpace4Frames2Period1Cast2Exp Error. %s", cudaGetErrorString(err));			
		SetCudaExceptionCode_ExeFunc();
		freeBuffer_Cuda(pMaskD);
		freeBuffer_Cuda(pSpaceD);
		freeBuffer_Cuda(CastParamPtrD);		
		FreeCastParamBuffer(CastParamD);
		return false;		
	}
	if ( NoiseParam.SpaceMultiCastPatchSize > 0 ) 
	{	
	}
	//Copy Back To Cast Param
	if ( CloneCastParamResultToHost(CastParamD, CastParam) == false )
	{
		CudaExceptionClean();
		return false;
	}
	freeBuffer_Cuda(CastParamPtrD);		
	FreeCastParamBuffer(CastParamD);		
	
	if ( CudaMedianFilter_Internal(ImageW, ImageH, ImageStep, pMaskD, NoiseParam, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber) == false )
	{
		CudaExceptionClean();
		return false;
	}

	bSuccess = checkCudaErrors(cudaMemcpy(SpacePtr, pSpaceD, sizeof(float)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pSpaceD To SpacePtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(MaskPtr, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To MaskPtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pSpaceD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolveSpace4Frames2Period2Cast2Exp(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber, int BlockNumber)//2投光2曝光計算出高度值
{
	LockCudaFunc();
	const bool IsOK = CudaSolveSpace4Frames2Period2Cast2ExpFn(ImageW, ImageH, ImageStep, CastParam1, CastParam2, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber);
	UnlockCudaFunc();
	return IsOK;
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolveSpace4Frames2Period2Cast2ExpFn(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber, int BlockNumber)//2投光2曝光計算出高度值
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolveSpace4Frames2Period2Cast2Exp";
	if ( CastParam1.ImageCount!=CastParam2.ImageCount ) 
	{
		::sprintf(this->m_ErrorString, "Error, Cuda CastParam[1, 2] Image Count Difference");
		SetCudaExceptionCode_Param();
		return false;
	}
	if ( NULL==MaskPtr || NULL==SpacePtr )
	{
		::sprintf(this->m_ErrorString, "Error, Cuda Solve 3D MaskPtr or SpacePtr is NULL");
		SetCudaExceptionCode_Param();
		return false;
	}
	if ( CheckCastParam2Exp(fnName, CastParam1) == false )
	{	return false; }
	if ( CheckCastParam2Exp(fnName, CastParam2) == false )
	{	return false; }
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam1) == false )
	{	return false; }		
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam2) == false )
	{	return false; }		
	
	//GPU運算	
	bool bSuccess = false;	
	float *pSpaceD = NULL;		
	TCastParam CastParamD1;
	TCastParam CastParamD2;
	PCastParam CastParamPtrD1 = NULL;
	PCastParam CastParamPtrD2 = NULL;	
	unsigned char *pMaskD = NULL;	
	const int DecodeMode = CastParam1.DecodeMode;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);	

	if ( allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pSpaceD, fnName, "pSpaceD") == false ||		 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam1, CastParamD1, CastParamPtrD1) == false || 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam2, CastParamD2, CastParamPtrD2) == false )
	{
		freeBuffer_Cuda(pMaskD);
		freeBuffer_Cuda(pSpaceD);
		freeBuffer_Cuda(CastParamPtrD1);
		freeBuffer_Cuda(CastParamPtrD2);
		FreeCastParamBuffer(CastParamD1);
		FreeCastParamBuffer(CastParamD2);		
		return false;
	}

	//Solve Phase A
//	ThreadNumber = 192;
//	BlockNumber = 961;

	cudaThreadSynchronize();
	//Hostfunction		
	ExecCudaSolveSpace4Frames2Period2Cast2Exp(CastParamPtrD1, CastParamPtrD2, DeviceDataSize, NoiseParam, pMaskD, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber, DecodeMode);	
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolveSpace4Frames2Period2Cast2Exp Error. %s", cudaGetErrorString(err));		
		SetCudaExceptionCode_ExeFunc();
		freeBuffer_Cuda(pMaskD);
		freeBuffer_Cuda(pSpaceD);
		freeBuffer_Cuda(CastParamPtrD1);
		freeBuffer_Cuda(CastParamPtrD2);
		FreeCastParamBuffer(CastParamD1);
		FreeCastParamBuffer(CastParamD2);		
		return false;		
	}
	if ( NoiseParam.SpaceMultiCastPatchSize > 0 ) 
	{
		cudaThreadSynchronize();
		//Hostfunction	
		ExecCudaMergeSpace4Frames2Period2Cast(CastParamPtrD1, CastParamPtrD2, DeviceDataSize, NoiseParam, pMaskD, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber);
		cudaThreadSynchronize();//等到device執行完
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, ExecCudaMergeSpace4Frames2Period2Cast Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			freeBuffer_Cuda(pMaskD);
			freeBuffer_Cuda(pSpaceD);
			freeBuffer_Cuda(CastParamPtrD1);
			freeBuffer_Cuda(CastParamPtrD2);
			FreeCastParamBuffer(CastParamD1);
			FreeCastParamBuffer(CastParamD2);				
			return false;		
		}		
	}	
	//Copy Back To Cast Param
	if ( CloneCastParamResultToHost(CastParamD1, CastParam1) == false ||	
	     CloneCastParamResultToHost(CastParamD2, CastParam2) == false )
	{
		CudaExceptionClean();
		return false;
	}
	freeBuffer_Cuda(CastParamPtrD1);
	freeBuffer_Cuda(CastParamPtrD2);
	FreeCastParamBuffer(CastParamD1);
	FreeCastParamBuffer(CastParamD2);	
	
	if ( CudaMedianFilter_Internal(ImageW, ImageH, ImageStep, pMaskD, NoiseParam, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber) == false )
	{
		CudaExceptionClean();
		return false;
	}

	bSuccess = checkCudaErrors(cudaMemcpy(SpacePtr, pSpaceD, sizeof(float)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pSpaceD To SpacePtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(MaskPtr, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To MaskPtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pSpaceD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolveSpace4Frames2Period3Cast2Exp(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber, int BlockNumber)//3投光2曝光計算出高度值	
{
	LockCudaFunc();
	const bool IsOK = CudaSolveSpace4Frames2Period3Cast2ExpFn(ImageW, ImageH, ImageStep, CastParam1, CastParam2, CastParam3, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber);
	UnlockCudaFunc();
	return IsOK;
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolveSpace4Frames2Period3Cast2ExpFn(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber, int BlockNumber)//3投光2曝光計算出高度值	
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolveSpace4Frames2Period3Cast2Exp";
	if ( CastParam1.ImageCount!=CastParam2.ImageCount || CastParam1.ImageCount!=CastParam3.ImageCount ) 
	{
		::sprintf(this->m_ErrorString, "Error, Cuda CastParam[1, 2, 3] Image Count Difference");
		SetCudaExceptionCode_Param();
		return false;
	}
	if ( NULL==MaskPtr || NULL==SpacePtr )
	{
		::sprintf(this->m_ErrorString, "Error, Cuda Solve 3D MaskPtr or SpacePtr is NULL");
		SetCudaExceptionCode_Param();
		return false;
	}
	if ( CheckCastParam2Exp(fnName, CastParam1) == false )
	{	return false; }
	if ( CheckCastParam2Exp(fnName, CastParam2) == false )
	{	return false; }
	if ( CheckCastParam2Exp(fnName, CastParam3) == false )
	{	return false; }
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam1) == false )
	{	return false; }		
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam2) == false )
	{	return false; }		
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam3) == false )
	{	return false; }		
	
	//GPU運算	
	bool bSuccess = false;	
	float *pSpaceD = NULL;		
	TCastParam CastParamD1;
	TCastParam CastParamD2;
	TCastParam CastParamD3;
	PCastParam CastParamPtrD1 = NULL;
	PCastParam CastParamPtrD2 = NULL;	
	PCastParam CastParamPtrD3 = NULL;
	unsigned char *pMaskD = NULL;	
	const int DecodeMode = CastParam1.DecodeMode;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);

	if ( allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pSpaceD, fnName, "pSpaceD") == false ||		 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam1, CastParamD1, CastParamPtrD1) == false || 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam2, CastParamD2, CastParamPtrD2) == false || 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam3, CastParamD3, CastParamPtrD3) == false )		
	{
		freeBuffer_Cuda(pMaskD);
		freeBuffer_Cuda(pSpaceD);
		freeBuffer_Cuda(CastParamPtrD1);
		freeBuffer_Cuda(CastParamPtrD2);
		freeBuffer_Cuda(CastParamPtrD3);		
		FreeCastParamBuffer(CastParamD1);
		FreeCastParamBuffer(CastParamD2);
		FreeCastParamBuffer(CastParamD3);
		return false;
	} 	
	//
	//Solve Phase A
	//ThreadNumber = 192;
	//BlockNumber = 961;
	
	//CastParamx3.Cast1 = 
	cudaThreadSynchronize();
	//Hostfunction		
	ExecCudaSolveSpace4Frames2Period3Cast2Exp(CastParamPtrD1, CastParamPtrD2, CastParamPtrD3, DeviceDataSize, NoiseParam, pMaskD, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber, DecodeMode);	
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolveSpace4Frames2Period3Cast2Exp Error. %s", cudaGetErrorString(err));			
		SetCudaExceptionCode_ExeFunc();
		freeBuffer_Cuda(pMaskD);
		freeBuffer_Cuda(pSpaceD);
		freeBuffer_Cuda(CastParamPtrD1);
		freeBuffer_Cuda(CastParamPtrD2);
		freeBuffer_Cuda(CastParamPtrD3);
		FreeCastParamBuffer(CastParamD1);
		FreeCastParamBuffer(CastParamD2);
		FreeCastParamBuffer(CastParamD3);	
		return false;		
	}
	if ( NoiseParam.SpaceMultiCastPatchSize > 0 ) 
	{
		cudaThreadSynchronize();
		//Hostfunction	
		ExecCudaMergeSpace4Frames2Period3Cast(CastParamPtrD1, CastParamPtrD2, CastParamPtrD3, DeviceDataSize, NoiseParam, pMaskD, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber);
		cudaThreadSynchronize();//等到device執行完
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, ExecCudaMergeSpace4Frames2Period3Cast Error. %s", cudaGetErrorString(err));		
			SetCudaExceptionCode_ExeFunc();
			freeBuffer_Cuda(pMaskD);
			freeBuffer_Cuda(pSpaceD);
			freeBuffer_Cuda(CastParamPtrD1);
			freeBuffer_Cuda(CastParamPtrD2);
			freeBuffer_Cuda(CastParamPtrD3);
			FreeCastParamBuffer(CastParamD1);
			FreeCastParamBuffer(CastParamD2);
			FreeCastParamBuffer(CastParamD3);			
			return false;		
		}		
	}	
	//Copy Back To Cast Param
	if ( CloneCastParamResultToHost(CastParamD1, CastParam1) == false ||	
	     CloneCastParamResultToHost(CastParamD2, CastParam2) == false ||
		 CloneCastParamResultToHost(CastParamD3, CastParam3) == false )
	{
		CudaExceptionClean();
		return false;
	}
	freeBuffer_Cuda(CastParamPtrD1);
	freeBuffer_Cuda(CastParamPtrD2);
	freeBuffer_Cuda(CastParamPtrD3);	
	FreeCastParamBuffer(CastParamD1);
	FreeCastParamBuffer(CastParamD2);
	FreeCastParamBuffer(CastParamD3);

	if ( CudaMedianFilter_Internal(ImageW, ImageH, ImageStep, pMaskD, NoiseParam, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber) == false )
	{
		CudaExceptionClean();
		return false;
	}

	bSuccess = checkCudaErrors(cudaMemcpy(SpacePtr, pSpaceD, sizeof(float)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pSpaceD To SpacePtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(MaskPtr, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To MaskPtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pSpaceD);

	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolveSpace4Frames2Period4Cast2Exp(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber, int BlockNumber)//4投光2曝光計算出高度值
{
	LockCudaFunc();
	const bool IsOK = CudaSolveSpace4Frames2Period4Cast2ExpFn(ImageW, ImageH, ImageStep, CastParam1, CastParam2, CastParam3, CastParam4, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber);
	UnlockCudaFunc();
	return IsOK;
}
//--------------------------------------------------------------------//
bool CCudaFunc::CudaSolveSpace4Frames2Period4Cast2ExpFn(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, TCastParam &CastParam1, TCastParam &CastParam2, TCastParam &CastParam3, TCastParam &CastParam4, const TPhaseNoiseParam &NoiseParam, MASK_PTR MaskPtr, SPACE_PTR SpacePtr, int ThreadNumber, int BlockNumber)//4投光2曝光計算出高度值
{
#ifdef  CUDA_USE
	//Texture
	bool IsUseTexture = false;
	const char fnName[] = "CCudaFunc::CudaSolveSpace4Frames2Period4Cast2Exp";
	if ( CastParam1.ImageCount!=CastParam2.ImageCount || CastParam1.ImageCount!=CastParam3.ImageCount || CastParam1.ImageCount!=CastParam4.ImageCount ) 
	{
		::sprintf(this->m_ErrorString, "Error, Cuda CastParam[1, 2, 3, 4] Image Count Difference");
		SetCudaExceptionCode_Param();
		return false;
	}
	if ( NULL==MaskPtr || NULL==SpacePtr )
	{
		::sprintf(this->m_ErrorString, "Error, Cuda Solve 3D MaskPtr or SpacePtr is NULL");
		SetCudaExceptionCode_Param();
		return false;
	}
	if ( CheckCastParam2Exp(fnName, CastParam1) == false )
	{	return false; }
	if ( CheckCastParam2Exp(fnName, CastParam2) == false )
	{	return false; }
	if ( CheckCastParam2Exp(fnName, CastParam3) == false )
	{	return false; }
	if ( CheckCastParam2Exp(fnName, CastParam4) == false )
	{	return false; }
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam1) == false )
	{	return false; }		
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam2) == false )
	{	return false; }		
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam3) == false )
	{	return false; }		
	if ( CheckCastParam(fnName, ImageW, ImageH, ImageStep, CastParam4) == false )
	{	return false; }		
	
	//GPU運算	
	bool bSuccess = false;	
	float *pSpaceD = NULL;	
	TCastParam CastParamD1;
	TCastParam CastParamD2;
	TCastParam CastParamD3;
	TCastParam CastParamD4;
	PCastParam CastParamPtrD1 = NULL;
	PCastParam CastParamPtrD2 = NULL;	
	PCastParam CastParamPtrD3 = NULL;	
	PCastParam CastParamPtrD4 = NULL;	
	unsigned char *pMaskD = NULL;	
	const int DecodeMode = CastParam1.DecodeMode;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);	
	const size_t DeviceDataSize = CalcBufferSize(ImageStep, ImageH);

	if ( allocBuffer_Cuda(BufferSize, pMaskD, fnName, "pMaskD") == false ||
		 allocBuffer_Cuda(BufferSize, pSpaceD, fnName, "pSpaceD") == false ||		 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam1, CastParamD1, CastParamPtrD1) == false || 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam2, CastParamD2, CastParamPtrD2) == false || 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam3, CastParamD3, CastParamPtrD3) == false || 
		 CloneCastParamPtrToDevice(fnName, ImageW, ImageH, ImageStep, CastParam4, CastParamD4, CastParamPtrD4) == false )
	{
		freeBuffer_Cuda(pMaskD);
		freeBuffer_Cuda(pSpaceD);		
		freeBuffer_Cuda(CastParamPtrD1);
		freeBuffer_Cuda(CastParamPtrD2);
		freeBuffer_Cuda(CastParamPtrD3);
		freeBuffer_Cuda(CastParamPtrD4);
		FreeCastParamBuffer(CastParamD1);
		FreeCastParamBuffer(CastParamD2);
		FreeCastParamBuffer(CastParamD3);
		FreeCastParamBuffer(CastParamD4);
		return false;
	} 		

	//Solve Phase A
	//ThreadNumber = 192;
	//BlockNumber = 961;

	cudaThreadSynchronize();
	//Hostfunction	
	ExecCudaSolveSpace4Frames2Period4Cast2Exp(CastParamPtrD1, CastParamPtrD2, CastParamPtrD3, CastParamPtrD4, DeviceDataSize, NoiseParam, pMaskD, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber, DecodeMode);
	cudaThreadSynchronize();//等到device執行完
	cudaError_t err = cudaGetLastError();
	if (cudaSuccess != err)
	{
		::sprintf(m_ErrorString, "Error, ExecCudaSolveSpace4Frames2Period4Cast2Exp Error. %s", cudaGetErrorString(err));	
		SetCudaExceptionCode_ExeFunc();
		freeBuffer_Cuda(pMaskD);
		freeBuffer_Cuda(pSpaceD);		
		freeBuffer_Cuda(CastParamPtrD1);
		freeBuffer_Cuda(CastParamPtrD2);
		freeBuffer_Cuda(CastParamPtrD3);
		freeBuffer_Cuda(CastParamPtrD4);
		FreeCastParamBuffer(CastParamD1);
		FreeCastParamBuffer(CastParamD2);
		FreeCastParamBuffer(CastParamD3);
		FreeCastParamBuffer(CastParamD4);
		return false;		
	}	
	if ( NoiseParam.SpaceMultiCastPatchSize > 0 ) 
	{
		cudaThreadSynchronize();
		//Hostfunction	
		ExecCudaMergeSpace4Frames2Period4Cast(CastParamPtrD1, CastParamPtrD2, CastParamPtrD3, CastParamPtrD4, DeviceDataSize, NoiseParam, pMaskD, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber);
		cudaThreadSynchronize();//等到device執行完
		err = cudaGetLastError();
		if (cudaSuccess != err)
		{
			::sprintf(m_ErrorString, "Error, ExecCudaMergeSpace4Frames2Period4Cast Error. %s", cudaGetErrorString(err));	
			SetCudaExceptionCode_ExeFunc();
			freeBuffer_Cuda(pMaskD);
			freeBuffer_Cuda(pSpaceD);		
			freeBuffer_Cuda(CastParamPtrD1);
			freeBuffer_Cuda(CastParamPtrD2);
			freeBuffer_Cuda(CastParamPtrD3);
			freeBuffer_Cuda(CastParamPtrD4);
			FreeCastParamBuffer(CastParamD1);
			FreeCastParamBuffer(CastParamD2);
			FreeCastParamBuffer(CastParamD3);
			FreeCastParamBuffer(CastParamD4);
			return false;		
		}		
	}
	//Copy Back To Cast Param
	if ( CloneCastParamResultToHost(CastParamD1, CastParam1) == false ||	
	     CloneCastParamResultToHost(CastParamD2, CastParam2) == false ||
		 CloneCastParamResultToHost(CastParamD3, CastParam3) == false ||
		 CloneCastParamResultToHost(CastParamD4, CastParam4) == false )
	{
		CudaExceptionClean();
		return false;
	}
	freeBuffer_Cuda(CastParamPtrD1);
	freeBuffer_Cuda(CastParamPtrD2);
	freeBuffer_Cuda(CastParamPtrD3);
	freeBuffer_Cuda(CastParamPtrD4);
	FreeCastParamBuffer(CastParamD1);
	FreeCastParamBuffer(CastParamD2);
	FreeCastParamBuffer(CastParamD3);
	FreeCastParamBuffer(CastParamD4);
	
	if ( CudaMedianFilter_Internal(ImageW, ImageH, ImageStep, pMaskD, NoiseParam, pSpaceD, IsUseTexture, ThreadNumber, BlockNumber) == false )
	{
		CudaExceptionClean();
		return false;
	}

	bSuccess = checkCudaErrors(cudaMemcpy(SpacePtr, pSpaceD, sizeof(float)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pSpaceD To SpacePtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	bSuccess = checkCudaErrors(cudaMemcpy(MaskPtr, pMaskD, sizeof(unsigned char)*DeviceDataSize, cudaMemcpyDeviceToHost));
	if( !bSuccess )
	{
		cudaError_t err = cudaGetLastError();
		::sprintf(m_ErrorString, "Error, cudaMemcpy pMaskD To MaskPtr. %s", cudaGetErrorString(err));
		SetCudaExceptionCode_MemCopy();
		CudaExceptionClean();
		return false;		
	}
	cudaThreadSynchronize();
	
	freeBuffer_Cuda(pMaskD);
	freeBuffer_Cuda(pSpaceD);	
	
	if( IsUseTexture )	{ UnLinkTextureSolvePhase(); }	
	return true;
#endif//CUDA_USE
	return ReturnCudaDisabled();	
}
//--------------------------------------------------------------------//
#endif