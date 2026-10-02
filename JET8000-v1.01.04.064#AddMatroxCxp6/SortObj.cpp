// SortObj.cpp: implementation of the CSortObj class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SortObj.h"
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
CSortObj::CSortObj()
{
	CSortObj::PreInitSort();
	CSortObj::InitialSort();
}
//-------------------------------------------------------------------------------------//
CSortObj::CSortObj(const CSortObj &Obj)
{
	CSortObj::PreInitSort();
	CSortObj::CloneSort(Obj);
}
//-------------------------------------------------------------------------------------//
CSortObj::~CSortObj()
{

}
//-------------------------------------------------------------------------------------//
inline void CSortObj::PreInitSort()
{
}
//-------------------------------------------------------------------------------------//
inline void CSortObj::InitialSort()
{
	m_ID = -1;
	m_SortMode = SORT_BY_ID;
	m_ValueInt = 0;
	m_ValueDbl = 0;
	m_ValueStr = _T("");
	m_ValueCnt = 0;
	m_Ptr = NULL;;
}
//-------------------------------------------------------------------------------------//
inline void CSortObj::CloneSort(const CSortObj &Obj)
{
	m_ID = Obj.m_ID;
	m_SortMode = Obj.m_SortMode;
	m_ValueInt = Obj.m_ValueInt;
	m_ValueDbl = Obj.m_ValueDbl;
	m_ValueStr = Obj.m_ValueStr;
	m_ValueCnt = Obj.m_ValueCnt;
	m_Ptr = Obj.m_Ptr;
}
//-------------------------------------------------------------------------------------//
CSortObj& CSortObj::operator=(const CSortObj &Obj)
{
	if ( this == &Obj ) { return *this; }
	CSortObj::CloneSort(Obj);
	return *this;
}
//-------------------------------------------------------------------------------------//
bool CSortObj::operator>(const CSortObj& Obj)
{
	switch ( m_SortMode )
	{
	case SORT_BY_INT:
		if ( m_ValueInt > Obj.m_ValueInt ) { return true; }
		else { return false; }
		break;
	case SORT_BY_DBL:
		if ( m_ValueDbl > Obj.m_ValueDbl ) { return true; }
		else { return false; }
		break;
	case SORT_BY_TXT:
		if ( m_ValueStr > Obj.m_ValueStr ) { return true; }
		else { return false; }
		break;
	case SORT_BY_CNT:
		if ( m_ValueCnt > Obj.m_ValueCnt ) { return true; }
		else { return false; }
		break;
	default:
		if ( m_ID > Obj.m_ID ) { return true; }
		else { return false; }
		break;
	}
	 return true;
}
//-------------------------------------------------------------------------------------//
bool CSortObj::operator<(const CSortObj& Obj)
{
	switch ( m_SortMode )
	{
	case SORT_BY_INT:
		if ( m_ValueInt < Obj.m_ValueInt ) { return true; }
		else { return false; }
		break;
	case SORT_BY_DBL:
		if ( m_ValueDbl < Obj.m_ValueDbl ) { return true; }
		else { return false; }
		break;
	case SORT_BY_TXT:
		if ( m_ValueStr < Obj.m_ValueStr ) { return true; }
		else { return false; }
		break;
	case SORT_BY_CNT:
		if ( m_ValueCnt < Obj.m_ValueCnt ) { return true; }
		else { return false; }
		break;
	default:
		if ( m_ID < Obj.m_ID ) { return true; }
		else { return false; }
		break;
	}
	 return true;
}
//-------------------------------------------------------------------------------------//