// OpenCV_MatAllocator.cpp: implementation of the CMatAllocator class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "OpenCV_MatAllocator.h"
//-------------------------------------------------------------------------------------//
#include "JetMemory.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#ifdef ENABLE_JET_MEMORY
	#define MAT_ALLOCATE_JET_MEMORY
#endif//ENABLE_JET_MEMORY
//-------------------------------------------------------------------------------------//
#ifndef OPENCV_DISABLE
//-------------------------------------------------------------------------------------//
#if OPEN_CV_VERSION == OPEN_CV_3_4_16_00_V14
CMatAllocator g_MatAllocator;
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CMatAllocator::CMatAllocator()
{	
}
//-------------------------------------------------------------------------------------//
CMatAllocator::~CMatAllocator()
{
   
}
//-------------------------------------------------------------------------------------//
cv::UMatData* CMatAllocator::allocate(int dims, const int* sizes, int type,
                               void* data0, size_t* step, int flags, cv::UMatUsageFlags usageFlags) const
{
	size_t total = CV_ELEM_SIZE(type);
	for( int i = dims-1; i >= 0; i-- )
	{
		if( step )
		{
			if( data0 && step[i] != CV_AUTOSTEP )
			{
				CV_Assert(total <= step[i]);
				total = step[i];
			}
			else
				step[i] = total;
		}
		total *= sizes[i];
	}
	
	//uchar* data = data0 ? (uchar*)data0 : (uchar*)cv::fastMalloc(total);
	uchar* data = (uchar*)data0;
	if ( nullptr == data )
	{
	#ifdef MAT_ALLOCATE_JET_MEMORY
		if ( JetMemory.alloc_func(total, data, "CMatAllocator::allocate", "data") == false )
		{	return nullptr; }		
	#else
		data = (uchar*)cv::fastMalloc(total);
	#endif//ENABLE_JET_MEMORY
	}
	cv::UMatData* u = new cv::UMatData(this);
	u->data = u->origdata = data;
	u->size = total;
	if(data0)
		u->flags |= cv::UMatData::USER_ALLOCATED;
	return u;
}
//-------------------------------------------------------------------------------------//
bool CMatAllocator::allocate(cv::UMatData* u, int accessflags, cv::UMatUsageFlags usageFlags) const
{
	if(!u) return false;
	return true;
}
//-------------------------------------------------------------------------------------//
void CMatAllocator::deallocate(cv::UMatData* u) const
{
	if(!u)
		return;

	CV_Assert(u->urefcount == 0);
	CV_Assert(u->refcount == 0);
	if( !(u->flags & cv::UMatData::USER_ALLOCATED) )
	{
	#ifdef MAT_ALLOCATE_JET_MEMORY
		uchar *p=(uchar*)u->origdata;
		JetMemory.free_func(p);
	#else
		cv::fastFree(u->origdata);
	#endif//ENABLE_JET_MEMORY		
		u->origdata = 0;
	}
	delete u;
}
//-------------------------------------------------------------------------------------//
#endif//OPEN_CV_VERSION
//-------------------------------------------------------------------------------------//
#endif//OPENCV_DISABLE
//-------------------------------------------------------------------------------------//