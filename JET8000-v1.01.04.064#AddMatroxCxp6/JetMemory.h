// JetMemory.h: interface for the CJetMemory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JETMEMORY_H__F71E0F4E_8F38_44CD_B321_A65D8AEEDACE__INCLUDED_)
#define AFX_JETMEMORY_H__F71E0F4E_8F38_44CD_B321_A65D8AEEDACE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------//
#ifndef OPENCV_DISABLE
	#if _MSC_VER == VC_6
		#define  OPEN_CV_USING_JET_MEMORY
	#endif//_MSC_VER == VC_6
#endif//OPENCV_DISABLE
//-------------------------------------------------------------------//
#define  ENABLE_JET_MEMORY
//-------------------------------------------------------------------//
#include <vector>
#include <string>
#define  MEM_NULL                     0
#define  MEM_UNLOCK                   1
#define  MEM_LOCK                     2
//-------------------------------------------------------------------//
#define  ALIGN_BASE                   128
//-------------------------------------------------------------------//
#define  MEMORY_NORMAL                 0
#define  MEMORY_OPEN_CV                1
//-------------------------------------------------------------------//
//MEMORY_DEBUG_ON            //紀錄函式與變數名稱  
#ifdef _DEBUG
	#define  MEMORY_DEBUG_ON
#else
	#define  MEMORY_DEBUG_ON
#endif//_DEBUG
//-------------------------------------------------------------------//
//__FUNCTION__//VC6.0不支援
class CJetMemory
{
private:
	//------------------------------------------------------------------------------------------//
#ifdef _MT
	CRITICAL_SECTION  m_cs;	//支援多執行緒呼叫
#endif
	//------------------------------------------------------------------------------------------//
	struct Ptr
	{
		void         *ptr;     //記憶體指標
		size_t        size;    //記憶體大小
		size_t        count;   //記憶體呼叫次數
		short         states;  //記憶體狀態		
#ifdef MEMORY_DEBUG_ON
		std::string   fnName;  //呼叫的函式名稱
		std::string   vaName;  //呼叫的變數名稱
#endif //MEMORY_DEBUG_ON
		Ptr(void *_ptr, size_t _size, short _stats, const char*_fnName, const char*_vaName)
		{
			ptr = _ptr;
			size = _size;
			count = 1;
			states = _stats;		
		#ifdef MEMORY_DEBUG_ON
			fnName = _fnName;
			vaName = _vaName;
		#endif //MEMORY_DEBUG_ON
		}
	};   
	//------------------------------------------------------------------------------------------//	
	size_t                     m_memExtraSize;//外加記憶體尺寸
	size_t                     m_memBasicSize;//基本記憶體尺寸	
	std::vector<Ptr>           m_memPtrList;//記憶體節點列表
	std::vector<size_t>        m_memSizeList;//記憶體尺寸列表
	//------------------------------------------------------------------------------------------//	
	int                        m_id;	
	char                       m_ErrorStringM[256];
	double                     m_ExecTimeus;//執行時間
	LARGE_INTEGER              m_SystemFreq;//系統計數頻率	
	bool                       m_CheckRepeateRelease;//確認重複釋放
	bool                       m_ShowMemoryLeakMessage;//顯示記憶體未釋放訊息
	//------------------------------------------------------------------------------------------//	
	void                       lock();//鎖住多執行緒
	void                       unlock();//解鎖多執行緒
	size_t                     get_align_size(size_t size);//取得對齊的大小	
	//------------------------------------------------------------------------------------------//
private:
	//------------------------------------------------------------------------------------------//
	static size_t              getIndexOffset();
	//------------------------------------------------------------------------------------------//
	void                       regAlloc(void *ptr, size_t size, size_t idx, short caller, const char *fnName, const char *vaName);//註冊新的記憶體節點
	void                       regFree(void *ptr, short caller);//釋放記憶體節點	
	//------------------------------------------------------------------------------------------//
	size_t                     get_memory_list_size() const;
	CJetMemory::Ptr*           get_memory_node_ptr(size_t idx);	
	//------------------------------------------------------------------------------------------//
	CJetMemory::Ptr*           get_free_memory(size_t size, short caller);//取得未鎖住的記憶體節點	
	//------------------------------------------------------------------------------------------//
	bool                       unlock_memory_ptr(void *ptr, short caller);//解鎖記憶體節點	
	//------------------------------------------------------------------------------------------//	
	static void*               alloc_func(size_t size, void *userdata);//allocate memory for opencv
	static int                 free_func(void *ptr, void *userdata);//free memory for opencv
	//------------------------------------------------------------------------------------------//
	static void*               alloc_func(size_t size, void *userdata, short caller, const char *fnName, const char *vaName);//allocate memory for opencv
	static int                 free_func(void *ptr, void *userdata, short caller);//free memory for opencv
	//------------------------------------------------------------------------------------------//		
	int                        output_memory_node_list(FILE* fp); //輸出	
	int                        output_memory_size_list(FILE* fp); //輸出	
	//------------------------------------------------------------------------------------------//
protected:
	//------------------------------------------------------------------------------------------//
	CJetMemory(const CJetMemory &mem);
	CJetMemory& operator=(const CJetMemory &mem);
	//------------------------------------------------------------------------------------------//
public:
	//------------------------------------------------------------------------------------------//
	CJetMemory(void);
	~CJetMemory(void);
	//------------------------------------------------------------------------------------------//		
	static void                SetMemoryExceptionCode_Alloc(LPCTSTR Err);
	static void                SetMemoryExceptionCode_Free(LPCTSTR Err);
	//------------------------------------------------------------------------------------------//
	static bool                alloc_func(size_t size, int *&Ptr, const char *fnName, const char *vaName); //allocate memory for normal use
	static bool                alloc_func(size_t size, short *&Ptr, const char *fnName, const char *vaName); //allocate memory for normal use
	static bool                alloc_func(size_t size, float *&Ptr, const char *fnName, const char *vaName); //allocate memory for normal use
	static bool                alloc_func(size_t size, double *&Ptr, const char *fnName, const char *vaName); //allocate memory for normal use
	static bool                alloc_func(size_t size, char *&Ptr, const char *fnName, const char *vaName); //allocate memory for normal use
	static bool                alloc_func(size_t size, wchar_t *&Ptr, const char *fnName, const char *vaName); //allocate memory for normal use
	static bool                alloc_func(size_t size, unsigned char *&Ptr, const char *fnName, const char *vaName); //allocate memory for normal use
	static bool                alloc_func(size_t size, unsigned short *&Ptr, const char *fnName, const char *vaName); //allocate memory for normal use
	//------------------------------------------------------------------------------------------//		
	static int                 free_func(int *&ptr);              //free  memory for normal use
	static int                 free_func(short *&ptr);            //free  memory for normal use
	static int                 free_func(float *&ptr);            //free  memory for normal use
	static int                 free_func(double *&ptr);           //free  memory for normal use
	static int                 free_func(char *&ptr);             //free  memory for normal use
	static int                 free_func(wchar_t *&ptr);          //free  memory for normal use
	static int                 free_func(unsigned char *&ptr);    //free  memory for normal use
	static int                 free_func(unsigned short *&ptr);   //free  memory for normal use
	//------------------------------------------------------------------------------------------//
	static bool                free_list(std::vector<int*> &list);           //free  memory list for normal use
	static bool                free_list(std::vector<short*> &list);         //free  memory list for normal use
	static bool                free_list(std::vector<float*> &list);         //free  memory list for normal use
	static bool                free_list(std::vector<double*> &list);        //free  memory list for normal use
	static bool                free_list(std::vector<char*> &list);          //free  memory list for normal use
	static bool                free_list(std::vector<wchar_t*> &list);       //free  memory list for normal use
	static bool                free_list(std::vector<unsigned char*> &list); //free  memory list for normal use
	static bool                free_list(std::vector<unsigned short*> &list);//free  memory list for normal use
	//------------------------------------------------------------------------------------------//	
	void                       release_mem_list(bool &bException);//釋放記憶體	
	void                       releaseExtraMemory();//釋放記憶體-非正常使用記憶體
	//------------------------------------------------------------------------------------------//
	const char*                GetErrorString();
	//------------------------------------------------------------------------------------------//
	void                       SetShowMemoryLeakMessage(bool val);//設定顯示記憶體未釋放訊息	
	//------------------------------------------------------------------------------------------//
	void                       add_memory_size(size_t size);	
	void                       set_basic_memory_size(size_t size);
	void                       set_extra_memory_size(size_t size);
	void                       recalc_layout_memory_size();
	size_t                     get_memory_size_count();
	size_t                     get_memory_size(size_t idx);
	//------------------------------------------------------------------------------------------//
	void                       reset_memory_exec_time();//復歸記憶體執行時間
	//------------------------------------------------------------------------------------------//
	bool                       save_memory_node_list(LPCTSTR filename);
	bool                       save_memory_size_list(LPCTSTR filename);
	//------------------------------------------------------------------------------------------//	
};
//------------------------------------------------------------------------------------------//
extern CJetMemory JetMemory;
//------------------------------------------------------------------------------------------//

#endif // !defined(AFX_JETMEMORY_H__F71E0F4E_8F38_44CD_B321_A65D8AEEDACE__INCLUDED_)
