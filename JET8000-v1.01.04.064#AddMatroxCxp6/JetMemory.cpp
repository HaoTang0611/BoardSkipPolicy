// JetMemory.cpp: implementation of the CJetMemory class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "JetMemory.h"
//-------------------------------------------------------------------------------------//
#ifdef OPEN_CV_USING_JET_MEMORY
	#include "OpenCV_Def.h"
#endif//OPEN_CV_USING_JET_MEMORY
//-------------------------------------------------------------------------------------//
#include <assert.h>
#define  MEM_INFO_SIZE     256
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
CJetMemory JetMemory;
//-------------------------------------------------------------------------------------//
CJetMemory::CJetMemory(void)
{	
#ifdef _MT
	::InitializeCriticalSection(&m_cs);	
#endif
	QueryPerformanceFrequency(&m_SystemFreq);

	m_id = 0;
	m_ExecTimeus = 0.0;
	m_memBasicSize = 1024;
	m_memExtraSize = 32;
	m_CheckRepeateRelease = true;
	m_ShowMemoryLeakMessage = true;
	::memset(m_ErrorStringM, 0x00, sizeof(m_ErrorStringM));		
	
#ifndef _X64	
	#ifdef OPEN_CV_USING_JET_MEMORY
		::cvSetMemoryManager(alloc_func, free_func, (void*)this);	
	#endif//OPEN_CV_USING_JET_MEMORY
#endif//_X64
}
//-------------------------------------------------------------------------------------//
CJetMemory::~CJetMemory(void)
{	
#ifndef _X64
	#ifdef OPEN_CV_USING_JET_MEMORY	
		::cvSetMemoryManager(NULL, NULL, NULL); 	
	#endif//OPEN_CV_USING_JET_MEMORY
#endif//_X64

#ifdef ENABLE_JET_MEMORY
	CString filename;	
	CString folder = AOIDataCollect.GetAOILogDirectory();
	filename.Format(_T("%s\\%s"), folder, _T("AppMemory.txt"));
	save_memory_node_list(filename);	
	#ifdef MEMORY_DEBUG_ON
	//	::ShellExecute(NULL, "open", filename, NULL, NULL, SW_SHOW);
	#endif//MEMORY_DEBUG_ON
#endif//ENABLE_JET_MEMORY
	
	bool bException=false;
	release_mem_list(bException);	
	if ( true == bException )
	{
		CString folder2;
		CString strTime;		
		CString filename2;		
		JetAPI::GetTime(strTime, CTime::GetCurrentTime());
		folder2.Format(_T("%s\\%s"), AOIDataCollect.GetAOILogDirectory(), _T("Error"));
		::CreateDirectory(folder2, NULL);
	#ifdef OFFLINE_VERSION
		filename2.Format(_T("%s\\%s_Offline_%s.txt"), folder2, _T("AppMemory"), strTime);
	#else
		filename2.Format(_T("%s\\%s_Online_%s.txt"), folder2, _T("AppMemory"), strTime);
	#endif//OFFLINE_VERSION
		::CopyFile(filename, filename2, FALSE);
	}

#ifdef _MT
	::DeleteCriticalSection(&m_cs);	
#endif

	TRACE0("CJetMemory::~CJetMemory\n");
}
//-------------------------------------------------------------------------------------//
void CJetMemory::lock()
{	
#ifdef _MT
	::EnterCriticalSection(&m_cs);
#endif
}
//------------------------------------------------------------------------------------------//
void CJetMemory::unlock()
{	
#ifdef _MT
	::LeaveCriticalSection(&m_cs);
#endif
}
//-------------------------------------------------------------------------------------//
const char* CJetMemory::GetErrorString()
{
	CString Err(m_ErrorStringM);
	AOIExceptionCodeCtrl.SetAOIExceptionCode_Memory_Others(Err);
	return this->m_ErrorStringM;
}
//-------------------------------------------------------------------------------------//
void CJetMemory::SetMemoryExceptionCode_Alloc(LPCTSTR Err)
{		
	AOIExceptionCodeCtrl.SetAOIExceptionCode_MemoryAlloc(Err);
	//AOIExceptionCodeCtrl.SetAOIExceptionCode_Memory(AOI_EXCEPTION_MEMORY_ALLOC, Err);
}
//-------------------------------------------------------------------------------------//
void CJetMemory::SetMemoryExceptionCode_Free(LPCTSTR Err)
{
	AOIExceptionCodeCtrl.SetAOIExceptionCode_MemoryFree(Err);
	//AOIExceptionCodeCtrl.SetAOIExceptionCode_Memory(AOI_EXCEPTION_MEMORY_FREE, Err);
}
//-------------------------------------------------------------------------------------//
void CJetMemory::SetShowMemoryLeakMessage(bool val)//設定顯示記憶體未釋放訊息	
{
	m_ShowMemoryLeakMessage = val;
}
//-------------------------------------------------------------------------------------//
size_t CJetMemory::getIndexOffset()
{	
	return 64;
}
//-------------------------------------------------------------------------------------//
void CJetMemory::regAlloc(void *ptr, size_t size, size_t idx, short caller, const char *fnName, const char *vaName)
{
#ifdef ENABLE_JET_MEMORY
	Ptr node(ptr, size, MEM_LOCK, fnName, vaName);	
	#ifdef MEMORY_DEBUG_ON
		char Info[MEM_INFO_SIZE]="";
		const size_t n = (get_memory_list_size());
		::sprintf(Info, ("[CJetMemory::regAlloc] Total:%d, ID:%d, Size:%d bytes, fn:%s, va:%s\n"), n, n, node.size, node.fnName.c_str(), node.vaName.c_str());
		//TRACE0(Info);
	#endif//MEMORY_DEBUG_ON
	m_memPtrList.push_back(node);
#endif//ENABLE_JET_MEMORY
}
//-------------------------------------------------------------------------------------//
void CJetMemory::regFree(void *ptr, short caller)
{
	size_t i;
	size_t size;
	Ptr   *pptr = NULL;
	const size_t n = (get_memory_list_size());
	for( i=0; i<n; ++i)
	{      
		pptr = get_memory_node_ptr(i);
		if ( pptr->ptr == ptr )
		{
			size = pptr->size;			
			m_memPtrList[i] = m_memPtrList[n-1];			
			m_memPtrList.pop_back();
			break;
		}
	}
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::save_memory_node_list(LPCTSTR filename)
{
	if ( NULL == filename ) { return false; }	
	TCHAR   TMode[32] = _T("");
	_tcscpy(TMode, _T("w+"));
	//JetAPI::ModifyOpenFileMode_Write(TMode);
	FILE *pfile = ::_tfopen(filename, TMode);
	if ( NULL == pfile )
	{	return false;	}
	
	this->output_memory_node_list(pfile);
	::fclose(pfile); pfile=NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
int CJetMemory::output_memory_node_list(FILE* fp)
{	
	const double ExecTimems = CJetMemory::m_ExecTimeus/1000.0;

#ifndef _X64
	#ifdef MEMORY_DEBUG_ON
		::fprintf(fp , ("%4s: %8s, %8s, %16s, %16s, %16s, %64s, %64s\n"), "No", "Count", "address", "size", "level", "locked", "function name", "variable name");
	#else
		::fprintf(fp , ("%4s: %8s, %8s, %16s, %16s, %16s\n"), "No", "Count", "address", "size", "level", "locked");
	#endif//MEMORY_DEBUG_ON
#else
	#ifdef MEMORY_DEBUG_ON
		::fprintf(fp , ("%4s: %8s, %16s, %16s, %16s, %16s, %64s, %64s\n"), "No", "Count", "address", "size", "level", "locked", "function name", "variable name");
	#else
		::fprintf(fp , ("%4s: %8s, %16s, %16s, %16s, %16s\n"), "No", "Count", "address", "size", "level", "locked");
	#endif//MEMORY_DEBUG_ON
#endif//_X64
//	::sprintf(Level, "Open CV");	
//	this->output(fp, &m_memTrackerOpenCV, Level, size);
	
	size_t i; 	
	Ptr *pptr=NULL;
	size_t size=0;
	const char strLevel[32] = "Level:1";
	const size_t n = (get_memory_list_size());	

	size = 0;
	for( i=0; i<n; ++i)
	{		
		pptr = get_memory_node_ptr(i);

	#ifdef MEMORY_DEBUG_ON
		if ( pptr->states == MEM_LOCK )
		{	::fprintf(fp, ("%4d: %8u, %p, %16u, %16s, %16s, %64s, %64s\n"), i+1, pptr->count, pptr->ptr, pptr->size, strLevel, "Locked", pptr->fnName.c_str(), pptr->vaName.c_str());	}
		else
		{	::fprintf(fp, ("%4d: %8u, %p, %16u, %16s\n"), i+1, pptr->count, pptr->ptr, pptr->size, strLevel);	}
	#else
		if ( pptr->states == MEM_LOCK )
		{	::fprintf(fp, ("%4d: %8u, %p, %16u, %16s, %16s\n"), i+1, pptr->count, pptr->ptr, pptr->size, strLevel, "Locked");	}
		else
		{	::fprintf(fp, ("%4d: %8u, %p, %16u, %16s\n"), i+1, pptr->count, pptr->ptr, pptr->size, strLevel);	}
	#endif//MEMORY_DEBUG_ON
		size += pptr->size;
	}
#ifndef _X64
	::fprintf(fp, "Total size: %u bytes, Exec Time: %.2f ms\n", size, ExecTimems);
#else
	::fprintf(fp, "Total size: %I64u bytes, Exec Time: %.2f ms\n", size, ExecTimems);
#endif//_X64
	return 0;
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::save_memory_size_list(LPCTSTR filename)
{
	if ( NULL == filename ) { return false; }	
	TCHAR   TMode[32] = _T("");
	_tcscpy(TMode, _T("w+"));
	//JetAPI::ModifyOpenFileMode_Write(TMode);
	FILE *pfile = ::_tfopen(filename, TMode);
	if ( NULL == pfile )
	{	return false;	}
	
	this->output_memory_size_list(pfile);
	::fclose(pfile); pfile=NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
int CJetMemory::output_memory_size_list(FILE* fp)//輸出	
{
	size_t i; 		
	size_t size=0;	
	const size_t n = (m_memSizeList.size());

	::fprintf(fp , ("%8s: %16s, \n"), "No", "size");
	for( i=0; i<n; ++i)
	{		
		size = m_memSizeList[i];
		::fprintf(fp, ("%8u: %16u\n"), i+1, size);
	} 
	return 0;
}
//-------------------------------------------------------------------------------------//
void* CJetMemory::alloc_func(size_t size, void *userdata, short caller, const char *fnName, const char *vaName)
{
	CJetMemory *JetMemory = (CJetMemory*)userdata;	
	if ( 0 == size ) 
	{			
		::sprintf(JetMemory->m_ErrorStringM, "Allocation Memory Exception [%s-%s (size=0)]", fnName, vaName);		
		return NULL;	
	}
	double         Timeus;
	LARGE_INTEGER  fnEnd;//起始時間
	LARGE_INTEGER  fnStart;//起始時間
	QueryPerformanceCounter(&fnStart);
	assert(size > 0 && userdata != NULL); 		
	JetMemory->lock();	
	size_t size_alloc=JetMemory->get_align_size(size);	
	Ptr *pptr = JetMemory->get_free_memory(size_alloc, caller);
	if ( NULL != pptr )
	{	
	#ifdef MEMORY_DEBUG_ON
		pptr->fnName = fnName;		
		pptr->vaName = vaName;
	#endif//MEMORY_DEBUG_ON
		pptr->count ++;
		pptr->states = MEM_LOCK;
		//::memset(pptr->ptr, 0xFF, sizeof(unsigned char)*size_alloc);//記憶體填滿-測試用
		JetMemory->unlock();
		QueryPerformanceCounter(&fnEnd);
		Timeus = (fnEnd.QuadPart - fnStart.QuadPart) * 1000000.0/JetMemory->m_SystemFreq.QuadPart;//us
		JetMemory->m_ExecTimeus += Timeus;
		return pptr->ptr;
	}

	try
	{
		TRACE0("[CJetMemory::allocate memory]\n");
	#ifndef ENABLE_JET_MEMORY
		const size_t szIndex = 0;//額外擴增的引數記憶體空間		
	#else
		//const size_t szIndex = sizeof(size_t);//額外擴增的引數記憶體空間
		const size_t szIndex = getIndexOffset();
	#endif//ENABLE_JET_MEMORY
		const size_t szMemory = size_alloc+szIndex;
		const size_t szMemCount = JetMemory->get_memory_list_size();
		unsigned char *uc_ptr = new unsigned char[szMemory];
		//void *ptr = (void*)(new unsigned char[szMemory]);
		if( NULL == uc_ptr ) 
		{		
			JetMemory->unlock();
			return NULL;   
		}
		::memset(uc_ptr, 0x00, sizeof(unsigned char)*szMemory);
		//::memset(uc_ptr, 0xFF, sizeof(unsigned char)*szMemory);//記憶體填滿-測試用
	#ifndef ENABLE_JET_MEMORY
		void *ptr = (void*)(uc_ptr);
	#else
		size_t *sz_ptr = (size_t*)(uc_ptr);
		*sz_ptr = szMemCount;
		void *ptr = (void*)(uc_ptr+szIndex);		
	#endif//ENABLE_JET_MEMORY
		JetMemory->regAlloc(ptr, size_alloc, szMemCount, caller, fnName, vaName);	
		JetMemory->unlock();
		QueryPerformanceCounter(&fnEnd);
		Timeus = (fnEnd.QuadPart - fnStart.QuadPart) * 1000000.0/JetMemory->m_SystemFreq.QuadPart;//us
		JetMemory->m_ExecTimeus += Timeus;
		return ptr;
	}
	catch ( ...  )
	{	
		::sprintf(JetMemory->m_ErrorStringM, "Allocation Memory Exception [%s-%s]", fnName, vaName);		
		
		CString Err = JetMemory->m_ErrorStringM;
		SetMemoryExceptionCode_Alloc(Err);
		::AfxMessageBox(Err);	
		JetMemory->unlock();
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
void* CJetMemory::alloc_func(size_t size, void *userdata)
{
	return alloc_func(size, userdata, MEMORY_OPEN_CV, ("OpenCV"), (""));
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::alloc_func(size_t size, int *&Ptr, const char *fnName, const char *vaName)//allocate memory for normal use
{
	Ptr = (int*)alloc_func(size*sizeof(int), &JetMemory, MEMORY_NORMAL, fnName, vaName);
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::alloc_func(size_t size, short *&Ptr, const char *fnName, const char *vaName)//allocate memory for normal use
{
	Ptr = (short*)alloc_func(size*sizeof(short), &JetMemory, MEMORY_NORMAL, fnName, vaName);
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::alloc_func(size_t size, float *&Ptr, const char *fnName, const char *vaName)//allocate memory for normal use
{
	Ptr = (float*)alloc_func(size*sizeof(float), &JetMemory, MEMORY_NORMAL, fnName, vaName);
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::alloc_func(size_t size, double *&Ptr, const char *fnName, const char *vaName)//allocate memory for normal use
{	
	Ptr = (double*)alloc_func(size*sizeof(double), &JetMemory, MEMORY_NORMAL, fnName, vaName);	
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::alloc_func(size_t size, char *&Ptr, const char *fnName, const char *vaName) //allocate memory for normal use
{
	Ptr = (char*)alloc_func(size*sizeof(char), &JetMemory, MEMORY_NORMAL, fnName, vaName);
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::alloc_func(size_t size, wchar_t *&Ptr, const char *fnName, const char *vaName) //allocate memory for normal use
{
	Ptr = (wchar_t*)alloc_func(size*sizeof(wchar_t), &JetMemory, MEMORY_NORMAL, fnName, vaName);
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::alloc_func(size_t size, unsigned char *&Ptr, const char *fnName, const char *vaName)//allocate memory for normal use
{
	Ptr = (unsigned char*)alloc_func(size*sizeof(unsigned char), &JetMemory, MEMORY_NORMAL, fnName, vaName);
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::alloc_func(size_t size, unsigned short *&Ptr, const char *fnName, const char *vaName)//allocate memory for normal use
{
	Ptr = (unsigned short*)alloc_func(size*sizeof(unsigned short), &JetMemory, MEMORY_NORMAL, fnName, vaName);
	if ( NULL == Ptr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
int CJetMemory::free_func(int *&ptr)              //free  memory for normal use
{
	if ( ptr == NULL ) { return 0; }
	int Res = free_func(ptr, &JetMemory, MEMORY_NORMAL);
	ptr = NULL;
	return Res;
}
//-------------------------------------------------------------------------------------//
int CJetMemory::free_func(short *&ptr)            //free  memory for normal use
{
	if ( ptr == NULL ) { return 0; }
	int Res = free_func(ptr, &JetMemory, MEMORY_NORMAL);
	ptr = NULL;
	return Res;
}
//-------------------------------------------------------------------------------------//
int CJetMemory::free_func(float *&ptr)            //free  memory for normal use
{
	if ( ptr == NULL ) { return 0; }
	int Res = free_func(ptr, &JetMemory, MEMORY_NORMAL);
	ptr = NULL;
	return Res;
}
//-------------------------------------------------------------------------------------//
int CJetMemory::free_func(double *&ptr)           //free  memory for normal use
{
	if ( ptr == NULL ) { return 0; }
	int Res = free_func(ptr, &JetMemory, MEMORY_NORMAL);
	ptr = NULL;
	return Res;
}
//-------------------------------------------------------------------------------------//
int CJetMemory::free_func(char *&ptr)//free  memory for normal use
{
	if ( ptr == NULL ) { return 0; }
	int Res = free_func(ptr, &JetMemory, MEMORY_NORMAL);
	ptr = NULL;
	return Res;
}
//-------------------------------------------------------------------------------------//
int CJetMemory::free_func(wchar_t *&ptr)//free  memory for normal use
{
	if ( ptr == NULL ) { return 0; }
	int Res = free_func(ptr, &JetMemory, MEMORY_NORMAL);
	ptr = NULL;
	return Res;
}
//-------------------------------------------------------------------------------------//
int  CJetMemory::free_func(unsigned char *&ptr)//free  memory for normal use
{
	if ( ptr == NULL ) { return 0; }
	int Res = free_func(ptr, &JetMemory, MEMORY_NORMAL);
	ptr = NULL;
	return Res;
}
//-------------------------------------------------------------------------------------//
int  CJetMemory::free_func(unsigned short *&ptr)//free  memory for normal use
{
	if ( ptr == NULL ) { return 0; }
	int Res = free_func(ptr, &JetMemory, MEMORY_NORMAL);
	ptr = NULL;
	return Res;
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::free_list(std::vector<int*> &list)//free  memory list for normal use
{
	for ( size_t i=0; i<list.size(); i++ )
	{	free_func(list[i]); }
	list.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::free_list(std::vector<short*> &list)//free  memory list for normal use
{
	for ( size_t i=0; i<list.size(); i++ )
	{	free_func(list[i]); }
	list.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::free_list(std::vector<float*> &list)//free  memory list for normal use
{
	for ( size_t i=0; i<list.size(); i++ )
	{	free_func(list[i]); }
	list.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::free_list(std::vector<double*> &list)//free  memory list for normal use
{
	for ( size_t i=0; i<list.size(); i++ )
	{	free_func(list[i]); }
	list.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::free_list(std::vector<char*> &list)//free  memory list for normal use
{
	for ( size_t i=0; i<list.size(); i++ )
	{	free_func(list[i]); }
	list.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::free_list(std::vector<wchar_t*> &list)//free  memory list for normal use
{
	for ( size_t i=0; i<list.size(); i++ )
	{	free_func(list[i]); }
	list.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::free_list(std::vector<unsigned char*> &list)//free  memory list for normal use
{
	for ( size_t i=0; i<list.size(); i++ )
	{	free_func(list[i]); }
	list.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetMemory::free_list(std::vector<unsigned short*> &list)//free  memory list for normal use
{
	for ( size_t i=0; i<list.size(); i++ )
	{	free_func(list[i]); }
	list.clear();
	return true;
}
//-------------------------------------------------------------------------------------//
int CJetMemory::free_func(void *ptr, void *userdata, short caller)//free memory for opencv
{	
	double         Timeus;
	LARGE_INTEGER  fnEnd;//起始時間
	LARGE_INTEGER  fnStart;//起始時間
	QueryPerformanceCounter(&fnStart);

	assert(ptr != NULL && userdata != NULL);	
	CJetMemory *JetMemory = (CJetMemory*)userdata;
	JetMemory->lock();	
	if ( JetMemory->unlock_memory_ptr(ptr, caller) == false )
	{	delete[] ptr;	}
	JetMemory->unlock();

	QueryPerformanceCounter(&fnEnd);
	Timeus = (fnEnd.QuadPart - fnStart.QuadPart) * 1000000.0/JetMemory->m_SystemFreq.QuadPart;//us
	JetMemory->m_ExecTimeus += Timeus;
	return 0;

	free(ptr);	
	JetMemory->regFree(ptr, caller); 
	// CV_OK == 0 
	return 0;
}
//-------------------------------------------------------------------------------------//
int CJetMemory::free_func(void *ptr, void *userdata)
{
	return free_func(ptr, userdata, MEMORY_OPEN_CV);
}
//-------------------------------------------------------------------------------------//
size_t  CJetMemory::get_align_size(size_t size)
{
#ifdef ENABLE_JET_MEMORY
	size_t  i = 0;
	//const size_t szMax = size*2;
	const size_t Count = m_memSizeList.size();
	for ( i=0; i<Count; i++ )
	{
		if ( size <= m_memSizeList[i] )
		{	return m_memSizeList[i];	}
	}
	//將Basic劃分成4等份來疊加
	double f = size;
	f = f/m_memBasicSize;
	int dig = (int)(f);
	//int rdn = (int)((f*100)-(dig*100));
	int rdn = floor((f*100)-(dig*100));
	dig = dig*100;
	if ( rdn == 0 ) 
	{	f = dig;	}
	else if ( rdn < 25 )
	{	f = dig+25;	}
	else if ( rdn < 50 )
	{	f = dig+50;	}
	else if ( rdn < 75 )
	{	f = dig+75;	}
	else
	{	f = dig+100;	}
	f = f/100.0;
	size_t NewSize = (size_t)(f*m_memBasicSize);
	if ( NewSize < size )
	{	
		f += 0.25;
		NewSize = (size_t)(f*m_memBasicSize);
	}
	NewSize += m_memExtraSize;
	add_memory_size(NewSize);
	return NewSize;

#else//ENABLE_JET_MEMORY
	return size;
#endif//ENABLE_JET_MEMORY
}
//-------------------------------------------------------------------------------------//
inline size_t CJetMemory::get_memory_list_size() const
{
	return m_memPtrList.size();
}
//-------------------------------------------------------------------------------------//
inline CJetMemory::Ptr* CJetMemory::get_memory_node_ptr(size_t idx)
{
	return &(m_memPtrList[idx]);
}
//-------------------------------------------------------------------------------------//
CJetMemory::Ptr* CJetMemory::get_free_memory(size_t size, short caller)
{	
	size_t i=0;	
	Ptr *pptr=NULL;
	const size_t size_max=size*2;
	const size_t n = (get_memory_list_size());
	for ( i=0; i<n; ++i)
	{
		pptr = get_memory_node_ptr(i);
		if ( pptr->size != size ) { continue; }
		if ( pptr->states == MEM_LOCK ) { continue; }
		//if ( pptr->states == MEM_NULL ) { continue; }
	#ifdef MEMORY_DEBUG_ON
		char Info[MEM_INFO_SIZE]="";
		::sprintf(Info, "[CJetMemory::get_free_memory] Total:%u, index:%u, count:%u, Size:%u bytes\n", n, i+1, pptr->count, pptr->size);
		//TRACE0(Info);
	#endif//MEMORY_DEBUG_ON
		return pptr;
	} 	
	return NULL;	
}
//-------------------------------------------------------------------------------------//
bool  CJetMemory::unlock_memory_ptr(void *ptr, short caller)
{
#ifdef ENABLE_JET_MEMORY
	size_t i;	
	Ptr *pptr=NULL;			
	//const size_t szIndex = sizeof(size_t);
	const size_t szIndex = getIndexOffset();
	const size_t n = (get_memory_list_size());		
	
	if ( NULL == ptr )
	{	return true; }
	
	CString str;
	unsigned char *uc_ptr = (unsigned char*)(ptr);
	size_t *sz_ptr = (size_t*)(uc_ptr-szIndex);
	const size_t szIdx = *sz_ptr;
#ifdef MEMORY_DEBUG_ON
	char Info[MEM_INFO_SIZE]="";
#endif//MEMORY_DEBUG_ON

	if ( szIdx < n ) 
	{		
		pptr = &(m_memPtrList[szIdx]);
		if ( ptr == pptr->ptr ) 
		{
			if ( true == m_CheckRepeateRelease )
			{
				if ( MEM_UNLOCK == pptr->states )
				{
					str = "Error, [CJetMemory::unlock_memory_ptr] repeate release memory";
					SetMemoryExceptionCode_Free(str);
					::AfxMessageBox(str);
				}
			}

		#ifdef MEMORY_DEBUG_ON			
			::sprintf(Info, "[CJetMemory::unlock_memory_ptr] Total:%u, index:%u, count:%u, size:%u bytes\n", n, szIdx, pptr->count, pptr->size);
			//TRACE0(Info);			
			pptr->fnName = "";
			pptr->vaName = "";			
		#endif//MEMORY_DEBUG_ON
			
			pptr->states = MEM_UNLOCK;			
			return true;
		}
	}
	str = _T("Error, CJetMemory::unlock_memory_ptr Fault");
	SetMemoryExceptionCode_Free(str);
	::AfxMessageBox(str);
	
	for ( i=0; i<n; ++i)
	{
		pptr = get_memory_node_ptr(i);
		if ( pptr->ptr == ptr )
		{
			if ( true == m_CheckRepeateRelease )
			{
				if ( MEM_UNLOCK == pptr->states )
				{	
					str = "Error, [CJetMemory::unlock_memory_ptr] repeate release memory";
					SetMemoryExceptionCode_Free(str);
					::AfxMessageBox(str);
				}
			}

		#ifdef MEMORY_DEBUG_ON			
			::sprintf(Info, "[CJetMemory::unlock_memory_ptr] Total:%u, index:%u, count:%u, size:%u bytes\n", n, i+1, pptr->count, pptr->size);
			//TRACE0(Info);			
			pptr->fnName = "";
			pptr->vaName = "";
		#endif//MEMORY_DEBUG_ON
			pptr->states = MEM_UNLOCK;			
			return true;
		}		
	}
	return false;
#else
	delete[] ptr; 
	return true;
#endif//ENABLE_JET_MEMORY
}
//-------------------------------------------------------------------------------------//
void CJetMemory::release_mem_list(bool &bException)
{	
	TRACE0("[CJetMemory::release_mem_list Begin]\n");
	this->lock();
	size_t         i; 
	size_t         len=0;
	DWORD          Res=IDOK;
	Ptr           *pptr=NULL;
	unsigned char *uc_ptr = NULL;
	char           Info[MEM_INFO_SIZE]="";			
	const size_t   n = (get_memory_list_size());
	//const size_t   szIndex = sizeof(size_t);//額外擴增的引數記憶體空間
	const size_t   szIndex = getIndexOffset();
	bException = false;
	for(i = 0; i < n; ++i)
	{
		pptr = get_memory_node_ptr(i);
		if ( MEM_LOCK == pptr->states )		
		{
			bException = true;
			if ( true == m_ShowMemoryLeakMessage )			
			{
			#ifdef MEMORY_DEBUG_ON
				::sprintf(Info, ("[CJetMemory::MemoryLeak] Total:%d, ID:%d, size:%d bytes\nFnName:%s\nVarName:%s\n"), n, i, pptr->size, pptr->fnName.c_str(), pptr->vaName.c_str());
			#else
				::sprintf(Info, ("[CJetMemory::MemoryLeak] Total:%d, ID:%d, size:%d bytes\n"), n, i, pptr->size);
			#endif//MEMORY_DEBUG_ON
				//TRACE0(Info);
				len = ::strlen(Info);
				if ( IDOK == Res )
				{	Res = ::AfxMessageBox(CString(Info), MB_OKCANCEL); }
			}	
		}

		if ( NULL != pptr->ptr )
		{
			//delete[] pptr->ptr;	
			uc_ptr = (unsigned char*)(pptr->ptr);
			uc_ptr = uc_ptr-szIndex;
			delete[] uc_ptr;

			pptr->ptr=NULL;
			pptr->states = MEM_NULL;				
			pptr->size = 0;
		}
	}
	m_memPtrList.clear();
	this->unlock();
	TRACE0("[CJetMemory::release_mem_list End]\n");
}
//-------------------------------------------------------------------------------------//
void CJetMemory::add_memory_size(size_t size)
{	
	size_t       i=0;
	std::vector<size_t>  memSizeListTmp = m_memSizeList;
	const size_t Count = memSizeListTmp.size();

	m_memSizeList.clear();		
	for ( i=0; i<Count; i++ )
	{
		if ( memSizeListTmp[i] < size )
		{	
			m_memSizeList.push_back(memSizeListTmp[i]);	
			continue;
		}
		else if ( size == memSizeListTmp[i] )
		{	continue; }
		m_memSizeList.push_back(size);	
		break;
	}
	if ( Count == i )
	{	m_memSizeList.push_back(size);		}
	else
	{
		for ( i=i; i<Count; i++ )
		{	m_memSizeList.push_back(memSizeListTmp[i]);	 }
	}
}
//-------------------------------------------------------------------------------------//
void CJetMemory::set_basic_memory_size(size_t size)
{
	m_memBasicSize = size;
}
//-------------------------------------------------------------------------------------//
void CJetMemory::set_extra_memory_size(size_t size)
{
	m_memExtraSize = size;
}
//-------------------------------------------------------------------------------------//
void CJetMemory::recalc_layout_memory_size()
{	
	const size_t size = m_memBasicSize+m_memExtraSize;
	m_memSizeList.clear();
	add_memory_size(size);
}
//-------------------------------------------------------------------------------------//
size_t CJetMemory::get_memory_size_count()
{
	return m_memSizeList.size();
}
//-------------------------------------------------------------------------------------//
size_t CJetMemory::get_memory_size(size_t idx)
{
	const size_t Count=m_memSizeList.size();
	if ( idx>=Count ) { return 0; }
	return m_memSizeList[idx];
}
//-------------------------------------------------------------------------------------//
void CJetMemory::reset_memory_exec_time()//復歸記憶體執行時間
{
	CJetMemory::m_ExecTimeus = 0.0;
}
//-------------------------------------------------------------------------------------//
void  CJetMemory::releaseExtraMemory()//釋放記憶體-非正常使用記憶體
{
#ifdef ENABLE_JET_MEMORY
	//this->lock();	
	//this->unlock();
#else
#endif//ENABLE_JET_MEMORY
}
//-------------------------------------------------------------------------------------//