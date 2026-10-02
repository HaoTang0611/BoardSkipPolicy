// OpenCV_MatAllocator.h: interface for the CMatAllocator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENCV_MATALLOCATOR_H__16F9FA38_845B_43D6_8CCC_2698C9FB97EA__INCLUDED_)
#define AFX_OPENCV_MATALLOCATOR_H__16F9FA38_845B_43D6_8CCC_2698C9FB97EA__INCLUDED_
//-------------------------------------------------------------------------------------//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "OpenCV_Def.h"
//-------------------------------------------------------------------------------------//
#ifndef OPENCV_DISABLE
//-------------------------------------------------------------------------------------//
#if OPEN_CV_VERSION == OPEN_CV_3_4_16_00_V14
class CMatAllocator : public cv::MatAllocator
{
public:
	CMatAllocator();
	~CMatAllocator();

	virtual cv::UMatData* allocate(int dims, const int* sizes, int type,
								   void* data, size_t* step, int flags, cv::UMatUsageFlags usageFlags) const;
	virtual bool allocate(cv::UMatData* data, int accessflags, cv::UMatUsageFlags usageFlags) const;
	virtual void deallocate(cv::UMatData* data) const;
};
//-------------------------------------------------------------------------------------//
extern CMatAllocator g_MatAllocator;
//-------------------------------------------------------------------------------------//
#endif//OPEN_CV_VERSION
//-------------------------------------------------------------------------------------//
#endif//OPENCV_DISABLE
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_OPENCV_MATALLOCATOR_H__16F9FA38_845B_43D6_8CCC_2698C9FB97EA__INCLUDED_)
