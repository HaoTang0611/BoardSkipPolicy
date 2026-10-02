// ColorGroupSet.h: interface for the CColorGroupSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORGROUPSET_H__628EE332_DD33_437D_9185_08C630A3E473__INCLUDED_)
#define AFX_COLORGROUPSET_H__628EE332_DD33_437D_9185_08C630A3E473__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "ColorGroup.h"
//-------------------------------------------------------------------------------------//
class CColorGroupSet  
{
private:
	//---------------------------------------------------------------------------------//	
	std::vector<CColorGroup>   m_ColorGroupList;
	unsigned int               m_ColorGroupSetIndex;
	std::wstring               m_ColorGroupSetName;//彩色群隊的名稱
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitColorGroupSet();
	void                       InitialColorGroupSet();
	void                       CloneColorGroupSet(const CColorGroupSet &GroupSet);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CColorGroupSet();
	CColorGroupSet(const CColorGroupSet &GroupSet);
	virtual ~CColorGroupSet();
	CColorGroupSet&            operator=(const CColorGroupSet &GroupSet);
	//---------------------------------------------------------------------------------//
	bool                       WriteColorGroupSetFile(CAOIFileIO &FileIO);//儲存抽色群隊檔案
	bool                       ReadColorGroupSetFile(CAOIFileIO &FileIO);//載入抽色群隊檔案	
	//---------------------------------------------------------------------------------//
	void                       SetColorGroupSetIndex(unsigned int val) { m_ColorGroupSetIndex = val; }
	unsigned int               GetColorGroupSetIndex() const { return m_ColorGroupSetIndex; }
	//---------------------------------------------------------------------------------//
	void                       SetColorGroupSetName(const char *Name);
	void                       SetColorGroupSetName(const wchar_t *Name);
	const wchar_t*             GetColorGroupSetName() const { return m_ColorGroupSetName.c_str(); }
	//---------------------------------------------------------------------------------//
	bool                       ResetColorGroupSetColorGroupInfo();//重置抽色群隊的資訊
	size_t                     GetColorGroupSetColorGroupCount() const;
	CColorGroup*               GetColorGroupSetColorGroupPtr(size_t index, bool bCheck);
	bool                       CloneColorGroupSetColorGroupList(std::vector<CColorGroup> &ColorGroupList);
	bool                       SetColorGroupSetColorGroupList(const std::vector<CColorGroup> &ColorGroupList);	
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_COLORGROUPSET_H__628EE332_DD33_437D_9185_08C630A3E473__INCLUDED_)
