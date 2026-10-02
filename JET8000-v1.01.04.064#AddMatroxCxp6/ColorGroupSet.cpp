// ColorGroupSet.cpp: implementation of the CColorGroupSet class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "ColorGroupSet.h"
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
CColorGroupSet::CColorGroupSet()
{
	PreInitColorGroupSet();
	InitialColorGroupSet();
}
//-------------------------------------------------------------------------------------//
CColorGroupSet::CColorGroupSet(const CColorGroupSet &GroupSet)
{
	PreInitColorGroupSet();
	CloneColorGroupSet(GroupSet);
}
//-------------------------------------------------------------------------------------//
CColorGroupSet::~CColorGroupSet()
{

}
//-------------------------------------------------------------------------------------//
CColorGroupSet& CColorGroupSet::operator=(const CColorGroupSet &GroupSet)
{
	if ( this == &GroupSet ) { return *this; }
	CloneColorGroupSet(GroupSet);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CColorGroupSet::PreInitColorGroupSet()
{
}
//-------------------------------------------------------------------------------------//
void CColorGroupSet::InitialColorGroupSet()
{
	//m_ColorGroup.;	
	int          i=0;
	CString      ColorName;
	CColorGroup  ColorGroup;	
	ColorGroup.BuildColorGroup_Test();

	m_ColorGroupList.clear();
	for ( i=0; i<MAX_PROJECT_COLOR_COUNT; i++ )
	{	
		ColorName = AOIDataDefine.GetProjectColorGroupText(i);
		
		ColorGroup.SetColorGroupIndex(i);		
		ColorGroup.SetColorGroupName(ColorName);
		m_ColorGroupList.push_back(ColorGroup);			
	}	

	m_ColorGroupSetIndex = -1;
	m_ColorGroupSetName = L"";//彩色群隊的名稱
}
//-------------------------------------------------------------------------------------//
void CColorGroupSet::CloneColorGroupSet(const CColorGroupSet &GroupSet)
{
	m_ColorGroupList = GroupSet.m_ColorGroupList;
	m_ColorGroupSetIndex = GroupSet.m_ColorGroupSetIndex;
	m_ColorGroupSetName = GroupSet.m_ColorGroupSetName;
}
//-------------------------------------------------------------------------------------//
bool CColorGroupSet::WriteColorGroupSetFile(CAOIFileIO &FileIO)//儲存抽色群隊檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG

	int     index = 0;
	CColorGroupSet *ColorGroupPtr = this;
	FileIO.SetFnName(_T("CColorGroupSet::WriteColorGroupSetFile"));
	//----------------------------------------------------------------------------------------//
	size_t           i=0;

	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_GROUP_SET_START, 0) == false ) { return false; }	
	if ( FileIO.SaveChunk_STR(FILE_IO_COLOR_GROUP_SET_NAME, m_ColorGroupSetName.c_str()) == false ) { return false; }		

	const size_t ColorGroupCount = m_ColorGroupList.size();
	for ( i=0; i<ColorGroupCount; i++ )
	{
		if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_GROUP_SET_COLOR_GROUP, i) == false ) { return false; }
		if ( m_ColorGroupList[i].WriteColorGroupFile(FileIO) == false ) { return false; }
	}	

	if ( FileIO.SaveChunk_INT(FILE_IO_COLOR_GROUP_SET_END, 0) == false ) { return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CColorGroupSet::ReadColorGroupSetFile(CAOIFileIO &FileIO)//載入抽色群隊檔案	
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	
	int             nValue = 0;
	int             index = 0;	
	CString         ColorName;
	CColorGroup     ColorGroup;
	CColorGroupSet *GroupSetPtr = this;
	FileIO.SetFnName(_T("CColorGroupSet::ReadColorGroupSetFile"));
	//----------------------------------------------------------------------------------------//	
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }
		
		switch ( index )
		{
		case FILE_IO_COLOR_GROUP_SET_START://抽色群隊參數-起點	
			GroupSetPtr->m_ColorGroupList.clear();
			break;
		case FILE_IO_COLOR_GROUP_SET_END://抽色群隊參數-終點			
			return true;
			break;
		case FILE_IO_COLOR_GROUP_SET_NAME://抽色群隊參數-群隊名稱
			if ( FileIO.GetLoadWStr()==true )
			{	GroupSetPtr->SetColorGroupSetName(FileIO.GetData_WSTR());	}
			else
			{	GroupSetPtr->SetColorGroupSetName(FileIO.GetData_STR());	}			
			break;		
		case FILE_IO_COLOR_GROUP_SET_COLOR_GROUP:

			nValue = FileIO.GetData_INT();
			if ( ColorGroup.ReadColorGroupFile(FileIO) == false )
			{	return false; }

			index = (int)(GroupSetPtr->m_ColorGroupList.size());
			ColorName = AOIDataDefine.GetProjectColorGroupText(index);
			ColorGroup.SetColorGroupIndex(index);
			ColorGroup.SetColorGroupName(ColorName);
			GroupSetPtr->m_ColorGroupList.push_back(ColorGroup);			
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
void CColorGroupSet::SetColorGroupSetName(const char *Name)
{
	if ( NULL == Name ) { return; }	
	JetAPI::char2wstring(Name, m_ColorGroupSetName);	
}
//-------------------------------------------------------------------------------------//
void CColorGroupSet::SetColorGroupSetName(const wchar_t *Name)
{
	if ( NULL == Name ) { return; }
	m_ColorGroupSetName = Name;
}
//-------------------------------------------------------------------------------------//
bool CColorGroupSet::ResetColorGroupSetColorGroupInfo()//重置抽色群隊的資訊
{
	size_t       i=0;
	CString      ColorGroupName;
	const size_t ColorGroupCount = m_ColorGroupList.size();
	for ( i=0; i<ColorGroupCount; i++ )
	{
		ColorGroupName = AOIDataDefine.GetProjectColorGroupText(i);
		m_ColorGroupList[i].SetColorGroupIndex(i);
		m_ColorGroupList[i].SetColorGroupName(ColorGroupName);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CColorGroupSet::GetColorGroupSetColorGroupCount() const
{
	return m_ColorGroupList.size();
}
//-------------------------------------------------------------------------------------//
CColorGroup* CColorGroupSet::GetColorGroupSetColorGroupPtr(size_t index, bool bCheck)
{
	if ( true == bCheck )
	{
		const size_t Count = GetColorGroupSetColorGroupCount();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return &(m_ColorGroupList[index]);
}
//-------------------------------------------------------------------------------------//
bool CColorGroupSet::CloneColorGroupSetColorGroupList(std::vector<CColorGroup> &ColorGroupList)
{
	ColorGroupList = m_ColorGroupList;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CColorGroupSet::SetColorGroupSetColorGroupList(const std::vector<CColorGroup> &ColorGroupList)
{
	m_ColorGroupList = ColorGroupList;
	return true;	
}
//-------------------------------------------------------------------------------------//
