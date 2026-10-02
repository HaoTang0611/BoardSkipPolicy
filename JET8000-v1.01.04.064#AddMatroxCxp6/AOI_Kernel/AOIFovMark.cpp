// AOIFovMark.cpp: implementation of the CAOIFovMark class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIFovMark.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CAOIFovMark, CObject)
//-------------------------------------------------------------------------------------//
CAOIFovMark::CAOIFovMark()
{
	PreInitFovMark();
	InitialFovMark();
}
//-------------------------------------------------------------------------------------//
CAOIFovMark::CAOIFovMark(const CAOIFovMark &mark)
{
	PreInitFovMark();
	CloneFovMark(mark);
}
//-------------------------------------------------------------------------------------//
CAOIFovMark::~CAOIFovMark()
{

}
//-------------------------------------------------------------------------------------//
CAOIFovMark& CAOIFovMark::operator=(const CAOIFovMark &mark)
{
	if ( this == &mark ) { return *this; }	
	CloneFovMark(mark);
	return *this; 
}
//-------------------------------------------------------------------------------------//
void CAOIFovMark::PreInitFovMark()//預先初始化FOV定位
{
}
//-------------------------------------------------------------------------------------//
void CAOIFovMark::InitialFovMark()//初始化FOV定位
{
}
//-------------------------------------------------------------------------------------//
void CAOIFovMark::CloneFovMark(const CAOIFovMark &mark)//複製FOV定位
{
}
//-------------------------------------------------------------------------------------//
CAOIFovMark* CAOIFovMark::CloneFovMarkObj() const//建立且複製一個FOV定位
{
	CAOIFovMark *ObjPtr = new CAOIFovMark();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
