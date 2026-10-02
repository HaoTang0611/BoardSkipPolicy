// SortObj.h: interface for the CSortObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SORTOBJ_H__9F1B5360_ED80_4029_B734_25E5A097B843__INCLUDED_)
#define AFX_SORTOBJ_H__9F1B5360_ED80_4029_B734_25E5A097B843__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
enum SORT_MODE
{
	SORT_BY_ID  = 1,
	SORT_BY_INT = 2,
	SORT_BY_DBL = 3,
	SORT_BY_TXT = 4,
	SORT_BY_CNT = 5
};
//-------------------------------------------------------------------------------------//
class CSortObj  
{
private:
	//---------------------------------------------------------------------------------//	
	SORT_MODE                  m_SortMode;
	//---------------------------------------------------------------------------------//
	size_t                     m_ID;	
	int                        m_ValueInt;
	double                     m_ValueDbl;
	CString                    m_ValueStr;
	size_t                     m_ValueCnt;
	void*                      m_Ptr;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitSort();
	void                       InitialSort();
	void                       CloneSort(const CSortObj &Obj);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//	
	CSortObj();
	CSortObj(const CSortObj &Obj);
	virtual ~CSortObj();
	//---------------------------------------------------------------------------------//
	CSortObj& operator=(const CSortObj &Obj);
	bool operator>(const CSortObj& Obj);
	bool operator<(const CSortObj& Obj);
	//---------------------------------------------------------------------------------//
	void                       SetSortMode(SORT_MODE value) {	m_SortMode = value; }
	SORT_MODE                  GetSortMode() const { return m_SortMode; }
	//---------------------------------------------------------------------------------//
	void                       SetID(size_t value) {	m_ID = value; }
	size_t                     GetID() const { return m_ID; }
	//---------------------------------------------------------------------------------//
	void                       SetValueInt(int value) {	m_ValueInt = value; }
	int                        GetValueInt() const { return m_ValueInt; }
	//---------------------------------------------------------------------------------//
	void                       SetValueDbl(double value) {	m_ValueDbl = value; }
	double                     GetValueDbl() const { return m_ValueDbl; }
	//---------------------------------------------------------------------------------//
	void                       SetValueStr(LPCTSTR    value) {	m_ValueStr = value; }
	LPCTSTR                    GetValueStr() const { return m_ValueStr; }
	//---------------------------------------------------------------------------------//
	void                       SetValueCnt(size_t value) {	m_ValueCnt = value; }
	size_t                     GetValueCnt() const { return m_ValueCnt; }
	//---------------------------------------------------------------------------------//
	void                       SetPtr(void*    value) {	m_Ptr = value; }
	void*                      GetPtr() const { return m_Ptr; }
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_SORTOBJ_H__9F1B5360_ED80_4029_B734_25E5A097B843__INCLUDED_)
