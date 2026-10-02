// ColorGroup.cpp: implementation of the CColorGroup class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "ColorGroup.h"
//-------------------------------------------------------------------------------------//
#include "AOIFileIO.h"
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
CColorGroup::CColorGroup()
{
	PreInitColorGroup();
	InitialColorGroup();
}
//-------------------------------------------------------------------------------------//
CColorGroup::CColorGroup(const CColorGroup &Group)
{
	PreInitColorGroup();
	CloneColorGroup(Group);
}
//-------------------------------------------------------------------------------------//
CColorGroup::~CColorGroup()
{

}
//-------------------------------------------------------------------------------------//
CColorGroup& CColorGroup::operator=(const CColorGroup &Group)
{
	if ( &Group == this ) { return *this; }
	CloneColorGroup(Group);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CColorGroup::PreInitColorGroup()
{
	
}
//-------------------------------------------------------------------------------------//
inline void CColorGroup::InitialColorGroup()
{
	m_ColorGroupIndex = -1;
	m_ColorGroupName = _T("");	
	m_ColorList.clear();
	m_ColorGroupFrameIndex = 0;		
	m_ColorGroupFrameUniqueID = FRAME_UNIQUE_ID_DEFAULT;
}
//-------------------------------------------------------------------------------------//
inline void CColorGroup::CloneColorGroup(const CColorGroup &Group)
{
	m_ColorGroupIndex = Group.m_ColorGroupIndex;
	m_ColorGroupName = Group.m_ColorGroupName;	
	m_ColorList = Group.m_ColorList;
	m_ColorGroupFrameIndex = Group.m_ColorGroupFrameIndex;
	m_ColorGroupFrameUniqueID = Group.m_ColorGroupFrameUniqueID;
}
//-------------------------------------------------------------------------------------//
bool CColorGroup::WriteColorGroupFile(CAOIFileIO &FileIO)//儲存抽色群組檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	int     index = 0;
	CColorGroup *ColorGroupPtr = this;
	FileIO.SetFnName(_T("CColorGroup::WriteColorGroupFile"));
	//----------------------------------------------------------------------------------------//
	size_t           i=0;
	CColorRGBV      *rgbvPtr =NULL;
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_GROUP_START, 0) == false ) { return false; }	

	//FILE_IO_COLOR_GROUP_NAME                     =     12104;//抽色群組參數-命名
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_GROUP_FRAME_UNIQUE_ID, GetColorGroupFrameUniqueID()) == false ) { return false; }	

	//儲存rgbv顏色
	const size_t ColorCount = ColorGroupPtr->GetColorGroupColorCount();	
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_SECTION_START, 0) == false ) { return false; }
	for ( i=0; i<ColorCount; i++ )
	{
		rgbvPtr = ColorGroupPtr->GetColorGroupColorPtr(i, false);
		if ( NULL == rgbvPtr ) { continue; }		
		if ( rgbvPtr->WriteColorRGBVFile(FileIO) == false ) { return false; }		
	}
	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_RGBV_SECTION_END, 0) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_GROUP_END, 0) == false ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CColorGroup::ReadColorGroupFile(CAOIFileIO &FileIO)//載入抽色群組檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	int        index = 0;
	CColorRGBV rgbv;
	CColorGroup *ColorGroupPtr = this;
	FileIO.SetFnName(_T("CColorGroup::ReadColorGroupFile"));
	//----------------------------------------------------------------------------------------//	
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		
		switch ( index )
		{
		case FILE_IO_COLOR_GROUP_START://抽色群組參數-起點
			ColorGroupPtr->ClearColorGroupColorList();
			break;
		case FILE_IO_COLOR_GROUP_END://抽色群組參數-終點			
			return true;
			break;
		case FILE_IO_COLOR_GROUP_FRAME_UNIQUE_ID://抽色群組參數-畫面唯一碼
			ColorGroupPtr->SetColorGroupFrameUniqueID(FileIO.GetData_INT());
			break;
		case FILE_IO_COLOR_RGBV_SECTION_START:
			break;
		case FILE_IO_COLOR_RGBV_START:
			rgbv.ResetColor();
			if ( rgbv.ReadColorRGBVFile(FileIO) == false )			
			{	return false;	}
			ColorGroupPtr->AddColorGroupColor(rgbv);
			break;
		case FILE_IO_COLOR_RGBV_SECTION_END:
			break;
		default:
		#ifdef _DEBUG
			index = index;
		#endif//_DEBUG
			break;
		}
	};		
	return true;
}
//-------------------------------------------------------------------------------------//
void CColorGroup::BuildColorGroup_Test()//建立檢測用的抽色群組
{	
	CColorRGBV   rgbv;

	ClearColorGroupColorList();

	rgbv.SetLogicMode(COLOR_LOGIC_INCLUDE);
	AddColorGroupColor(rgbv);
	AddColorGroupColor(rgbv);
	AddColorGroupColor(rgbv);
	AddColorGroupColor(rgbv);
	AddColorGroupColor(rgbv);
	AddColorGroupColor(rgbv);
	AddColorGroupColor(rgbv);
	AddColorGroupColor(rgbv);

	rgbv.SetLogicMode(COLOR_LOGIC_EXCLUDE);
	AddColorGroupColor(rgbv);
	AddColorGroupColor(rgbv);
	AddColorGroupColor(rgbv);
	AddColorGroupColor(rgbv);

	AddColorGroupColor(rgbv);//擴增至8組減色
	AddColorGroupColor(rgbv);
	AddColorGroupColor(rgbv);
	AddColorGroupColor(rgbv);
	return;
}
//-------------------------------------------------------------------------------------//
size_t CColorGroup::GetColorGroupColorCount() const
{
	return m_ColorList.size();
}
//-------------------------------------------------------------------------------------//
void CColorGroup::AddColorGroupColor(const CColorRGBV &rgbv)
{
	m_ColorList.push_back(rgbv);
}
//-------------------------------------------------------------------------------------//
bool CColorGroup::SetColorGroupColor(size_t idx, const CColorRGBV &rgbv)//設定抽色群組的顏色
{
	const size_t ColorCount = CColorGroup::GetColorGroupColorCount();
	if ( idx >= ColorCount ) { return false; }
	m_ColorList[idx] = rgbv;
	return true;
}
//-------------------------------------------------------------------------------------//
CColorRGBV* CColorGroup::GetColorGroupColorPtr(size_t index, bool Check)
{
	if ( Check )
	{
		const size_t size = m_ColorList.size();
		if ( index >= size )
		{	return NULL; }
	}
	return &(m_ColorList[index]);
}
//-------------------------------------------------------------------------------------//
const CColorRGBV* CColorGroup::GetColorGroupColorPtr(size_t index, bool Check) const
{
	if ( Check )
	{
		const size_t size = m_ColorList.size();
		if ( index >= size )
		{	return NULL; }
	}
	return &(m_ColorList[index]);
}
//-------------------------------------------------------------------------------------//
CColorRGBV* CColorGroup::GetColorGroupActiveColorPtr()//取得主要操作顏色
{
	size_t       i=0;
	CColorRGBV  *rgbvPtr = NULL;
	const size_t Count = GetColorGroupColorCount();
	for ( i=0; i<Count; i++ )
	{
		rgbvPtr = GetColorGroupColorPtr(i, false);		
		if ( NULL == rgbvPtr ) { continue; }
		if ( rgbvPtr->GetActived() == false ) { continue; }
		return rgbvPtr;		
	}
	return NULL;
}
//-------------------------------------------------------------------------------------//
void CColorGroup::SetColorGroupActiveColorPtr(CColorRGBV* Ptr)//設定主要操作顏色	
{
	size_t       i=0;
	CColorRGBV  *rgbvPtr = NULL;
	const size_t Count = GetColorGroupColorCount();
	for ( i=0; i<Count; i++ )
	{
		rgbvPtr = GetColorGroupColorPtr(i, false);		
		if ( NULL == rgbvPtr ) { continue; }		
		rgbvPtr->SetActived(false);
	}
	if ( NULL != Ptr )
	{	Ptr->SetActived(true); }
	return;
}
//-------------------------------------------------------------------------------------//
size_t CColorGroup::GetColorGroupActiveColorIndex() const//取得主要操作顏色	
{
	size_t       i=0;
	//CColorRGBV  *rgbvPtr = NULL;
	const size_t Count = GetColorGroupColorCount();
	for ( i=0; i<Count; i++ )
	{
		const CColorRGBV* rgbvPtr = GetColorGroupColorPtr(i, false);
		if ( NULL == rgbvPtr ) { continue; }
		if ( rgbvPtr->GetActived() == false ) { continue; }
		return i;		
	}
	return -1;
}
//-------------------------------------------------------------------------------------//
void CColorGroup::SetColorGroupActiveColorIndex(size_t index)//設定主要操作顏色	
{
	size_t       i=0;
	CColorRGBV  *rgbvPtr = NULL;
	const size_t Count = GetColorGroupColorCount();
	for ( i=0; i<Count; i++ )
	{
		rgbvPtr = GetColorGroupColorPtr(i, false);
		if ( NULL == rgbvPtr ) { continue; }		
		if ( index == i )
		{	rgbvPtr->SetActived(true);	}
		else
		{	rgbvPtr->SetActived(false); }		
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CColorGroup::ClearColorGroupColorList()//清除抽色群組的顏色列表
{
	m_ColorList.clear();
}
//-------------------------------------------------------------------------------------//
void CColorGroup::ResetColorGroupColorList()//復歸抽色群組的顏色列表
{
	size_t       i=0;
	CColorRGBV  *rgbvPtr = NULL;
	const size_t Count = GetColorGroupColorCount();
	for ( i=0; i<Count; i++ )
	{
		rgbvPtr = GetColorGroupColorPtr(i, false);
		if ( NULL == rgbvPtr ) { continue; }
		rgbvPtr->ResetColor();
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CColorGroup::UpdateColorGroupUsed()//更新抽色群組的顏色列表是否使用
{
	size_t       i=0;
	CColorRGBV  *rgbvPtr = NULL;
	const size_t Count = CColorGroup::GetColorGroupColorCount();
	for ( i=0; i<Count; i++ )
	{
		rgbvPtr = CColorGroup::GetColorGroupColorPtr(i, false);
		if ( NULL == rgbvPtr ) { continue; }
		rgbvPtr->CheckUsed();
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CColorGroup::UpdateColorGroupShowColor()//更新抽色群組的顏色列表顯示顏色
{
	size_t       i=0;
	CColorRGBV  *rgbvPtr = NULL;
	const size_t Count = CColorGroup::GetColorGroupColorCount();
	for ( i=0; i<Count; i++ )
	{
		rgbvPtr = CColorGroup::GetColorGroupColorPtr(i, false);
		if ( NULL == rgbvPtr ) { continue; }
		rgbvPtr->CalcShowColor();
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CColorGroup::CloneColorGroupColorList(CColorGroup &ColorGroup)//複製抽色群組的顏色列表
{
	size_t       i=0;
	CColorRGBV  *rgbvPtrSrc = NULL;
	CColorRGBV  *rgbvPtrDst = NULL;
	const size_t Count = GetColorGroupColorCount();
	for ( i=0; i<Count; i++ )
	{
		rgbvPtrSrc = GetColorGroupColorPtr(i, false);
		if ( NULL == rgbvPtrSrc ) { continue; }
		rgbvPtrDst = ColorGroup.GetColorGroupColorPtr(i, true);
		if ( NULL == rgbvPtrDst ) { continue; }
		rgbvPtrSrc->CopyColorRGBV(*rgbvPtrDst);
	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CColorGroup::MergeColorGroupColor(bool IncClr, bool ExcClr)//合併抽色群組內的色
{
	size_t              i=0, j=0;	
	CColorRGBV         *rgbvPtr = NULL;
	CColorRGBV         *rgbvPtr2 = NULL;
	COLOR_LOGIC_MODE    LogicMode;		
	const size_t ColorCount = GetColorGroupColorCount();
	
	if ( true == IncClr )
	{
		LogicMode = COLOR_LOGIC_INCLUDE;
		for ( i=0; i<ColorCount; i++ )
		{
			rgbvPtr = GetColorGroupColorPtr(i, false);
			if ( NULL == rgbvPtr ) { continue; }			
			if ( rgbvPtr->GetLogicMode() != LogicMode ) { continue; }
			if ( rgbvPtr->CheckUsed() == false ) { continue; }
			if ( rgbvPtr->CheckMerge() == false ) { continue; }			

			for ( j=i+1; j<ColorCount; j++ )
			{
				rgbvPtr2 = GetColorGroupColorPtr(j, false);
				if ( NULL == rgbvPtr2 ) { continue; }
				if ( rgbvPtr2->GetLogicMode() != LogicMode ) { continue; }
				if ( rgbvPtr2->CheckUsed() == false ) { continue; }
				if ( rgbvPtr2->CheckMerge() == false ) { continue; }				
				if ( rgbvPtr->MergeColor(true, rgbvPtr2) == false ) { continue; }

				rgbvPtr2->ResetColor();
			}
		}

		//Rearrange List
		for ( i=0; i<ColorCount; i++ )
		{
			rgbvPtr = GetColorGroupColorPtr(i, false);
			if ( NULL == rgbvPtr ) { continue; }			
			if ( rgbvPtr->GetLogicMode() != LogicMode ) { continue; }
			if ( rgbvPtr->CheckUsed() == true ) { continue; }

			for ( j=i+1; j<ColorCount; j++ )
			{
				rgbvPtr2 = GetColorGroupColorPtr(j, false);
				if ( NULL == rgbvPtr2 ) { continue; }
				if ( rgbvPtr2->GetLogicMode() != LogicMode ) { continue; }
				if ( rgbvPtr2->CheckUsed() == false ) { continue; }
				
				*rgbvPtr = *rgbvPtr2;
				rgbvPtr2->ResetColor();
			}
		}
	}
	if ( true == ExcClr )
	{
		LogicMode = COLOR_LOGIC_EXCLUDE;
		for ( i=0; i<ColorCount; i++ )
		{
			rgbvPtr = GetColorGroupColorPtr(i, false);
			if ( NULL == rgbvPtr ) { continue; }			
			if ( rgbvPtr->GetLogicMode() != LogicMode ) { continue; }
			if ( rgbvPtr->CheckUsed() == false ) { continue; }

			for ( j=i+1; j<ColorCount; j++ )
			{
				rgbvPtr2 = GetColorGroupColorPtr(j, false);
				if ( NULL == rgbvPtr2 ) { continue; }
				if ( rgbvPtr2->GetLogicMode() != LogicMode ) { continue; }
				if ( rgbvPtr2->CheckUsed() == false ) { continue; }
				if ( rgbvPtr->MergeColor(true, rgbvPtr2) == false ) { continue; }

				rgbvPtr2->ResetColor();
			}
		}

		//Rearrange List
		for ( i=0; i<ColorCount; i++ )
		{
			rgbvPtr = GetColorGroupColorPtr(i, false);
			if ( NULL == rgbvPtr ) { continue; }			
			if ( rgbvPtr->GetLogicMode() != LogicMode ) { continue; }
			if ( rgbvPtr->CheckUsed() == true ) { continue; }

			for ( j=i+1; j<ColorCount; j++ )
			{
				rgbvPtr2 = GetColorGroupColorPtr(j, false);
				if ( NULL == rgbvPtr2 ) { continue; }
				if ( rgbvPtr2->GetLogicMode() != LogicMode ) { continue; }
				if ( rgbvPtr2->CheckUsed() == false ) { continue; }
				
				*rgbvPtr = *rgbvPtr2;
				rgbvPtr2->ResetColor();
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CColorGroup::SetColorGroupName(LPCTSTR Name)
{	
	m_ColorGroupName = Name;	
}
//-------------------------------------------------------------------------------------//
LPCTSTR  CColorGroup::GetColorGroupName()
{
	return m_ColorGroupName;	
}
//-------------------------------------------------------------------------------------//
bool CColorGroup::CheckColorGroupUsed()//確認顏色群組是否已經使用
{
	size_t              i=0, j=0;	
	CColorRGBV         *rgbvPtr = NULL;		
	const size_t ColorCount = GetColorGroupColorCount();

	for ( i=0; i<ColorCount; i++ )
	{
		rgbvPtr = GetColorGroupColorPtr(i, false);
		if ( NULL == rgbvPtr ) { continue; }
		if ( rgbvPtr->CheckUsed() == true )
		{	return true; }
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CColorGroup::UpdateColorGroupFrameUniqueID(unsigned int DefaultIndex, unsigned int DefaultUniqueID, const std::vector<unsigned int> &FrameIndexMapList)
{
	unsigned int FrameIndex = GetColorGroupFrameIndex();
	unsigned int FrameUniqueID = GetColorGroupFrameUniqueID();		
	const size_t FrameIndexMapSize = FrameIndexMapList.size();
	if ( FrameUniqueID<0 || FrameUniqueID>=FrameIndexMapSize )
	{	
		FrameIndex = DefaultIndex;	
		FrameUniqueID = DefaultUniqueID;
	}
	else
	{	
		FrameIndex = (FrameIndexMapList[FrameUniqueID]);
		if ( -1 == FrameIndex )
		{ 
			FrameIndex = DefaultIndex;	
			FrameUniqueID = DefaultUniqueID;
		}
	}	
	SetColorGroupFrameIndex(FrameIndex);
	SetColorGroupFrameUniqueID(FrameUniqueID);	
	return true;
}
//-------------------------------------------------------------------------------------//