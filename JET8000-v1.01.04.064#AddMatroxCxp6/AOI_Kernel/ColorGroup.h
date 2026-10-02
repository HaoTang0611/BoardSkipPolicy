// ColorGroup.h: interface for the CColorGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORGROUP_H__8244DEB8_B168_4DDD_94D1_91796F13EA71__INCLUDED_)
#define AFX_COLORGROUP_H__8244DEB8_B168_4DDD_94D1_91796F13EA71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "ColorRGBV.h"
//-------------------------------------------------------------------------------------//
class CAOIFileIO;
//-------------------------------------------------------------------------------------//
class CColorGroup  
{
private:
	//-------------------------------------------------------------------------------------//			
	size_t                     m_ColorGroupIndex;
	CString                    m_ColorGroupName;
	std::vector<CColorRGBV>    m_ColorList;
	unsigned int               m_ColorGroupFrameIndex;//畫面編號
	unsigned int               m_ColorGroupFrameUniqueID;////畫面唯一碼
	//-------------------------------------------------------------------------------------//
protected:
	//-------------------------------------------------------------------------------------//
	void                       PreInitColorGroup();
	void                       InitialColorGroup();
	void                       CloneColorGroup(const CColorGroup &Group);
	//-------------------------------------------------------------------------------------//
public:
	//-------------------------------------------------------------------------------------//
	CColorGroup();
	CColorGroup(const CColorGroup &Group);
	virtual ~CColorGroup();
	CColorGroup&               operator=(const CColorGroup &Group);
	//-------------------------------------------------------------------------------------//
	bool                       WriteColorGroupFile(CAOIFileIO &FileIO);//儲存抽色群組檔案
	bool                       ReadColorGroupFile(CAOIFileIO &FileIO);//載入抽色群組檔案
	//-------------------------------------------------------------------------------------//
	void                       BuildColorGroup_Test();//建立檢測用的抽色群組
	//-------------------------------------------------------------------------------------//
	size_t                     GetColorGroupColorCount() const;
	void                       AddColorGroupColor(const CColorRGBV &rgbv);
	bool                       SetColorGroupColor(size_t idx, const CColorRGBV &rgbv);//設定抽色群組的顏色
	CColorRGBV*                GetColorGroupColorPtr(size_t index, bool Check);
	const CColorRGBV*          GetColorGroupColorPtr(size_t index, bool Check) const;
	CColorRGBV*                GetColorGroupActiveColorPtr();//取得主要操作顏色	
	void                       SetColorGroupActiveColorPtr(CColorRGBV* Ptr);//設定主要操作顏色	
	size_t                     GetColorGroupActiveColorIndex() const;//取得主要操作顏色	
	void                       SetColorGroupActiveColorIndex(size_t index);//設定主要操作顏色	
	void                       ClearColorGroupColorList();//清除抽色群組的顏色列表
	void                       ResetColorGroupColorList();//復歸抽色群組的顏色列表	
	void                       UpdateColorGroupUsed();//更新抽色群組的顏色列表是否使用
	void                       UpdateColorGroupShowColor();//更新抽色群組的顏色列表顯示顏色
	void                       CloneColorGroupColorList(CColorGroup &ColorGroup);//複製抽色群組的顏色列表
	bool                       MergeColorGroupColor(bool IncClr=true, bool ExcClr=true);//合併抽色群組內的色	
	//-------------------------------------------------------------------------------------//
	void                       SetColorGroupName(LPCTSTR Name);
	LPCTSTR                    GetColorGroupName();
	//-------------------------------------------------------------------------------------//		
	void                       SetColorGroupIndex(size_t idx) { m_ColorGroupIndex=idx; }
	size_t                     GetColorGroupIndex() const { return m_ColorGroupIndex; }
	//-------------------------------------------------------------------------------------//
	//畫面編號
	void                       SetColorGroupFrameIndex(unsigned int idx) { m_ColorGroupFrameIndex=idx; }
	unsigned int               GetColorGroupFrameIndex() const { return m_ColorGroupFrameIndex; }
	//-------------------------------------------------------------------------------------//
	//畫面唯一碼
	void                       SetColorGroupFrameUniqueID(unsigned int idx) { m_ColorGroupFrameUniqueID=idx; }
	unsigned int               GetColorGroupFrameUniqueID() const { return m_ColorGroupFrameUniqueID; }
	//-------------------------------------------------------------------------------------//	
	bool                       CheckColorGroupUsed();//確認顏色群組是否已經使用
	//-------------------------------------------------------------------------------------//	
	bool                       UpdateColorGroupFrameUniqueID(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList);
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_COLORGROUP_H__8244DEB8_B168_4DDD_94D1_91796F13EA71__INCLUDED_)
