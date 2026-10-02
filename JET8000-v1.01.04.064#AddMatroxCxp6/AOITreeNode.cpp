// AOITreeNode.cpp: implementation of the CAOITreeNode class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOITreeNode.h"
//-------------------------------------------------------------------------------------//
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
CAOITreeNode::CAOITreeNode():m_hItem(NULL), m_ObjPtr(NULL)
{
	m_LastChildAdded = -1;
}
//-------------------------------------------------------------------------------------//
CAOITreeNode::CAOITreeNode(HTREEITEM item, CAOIObj *Ptr):m_hItem(item), m_ObjPtr(Ptr)
{
	m_LastChildAdded = -1;
}
//-------------------------------------------------------------------------------------//
CAOITreeNode::~CAOITreeNode()
{
}
//-------------------------------------------------------------------------------------//
HTREEITEM CAOITreeNode::GetHItem() const
{
	return m_hItem;
}
//-------------------------------------------------------------------------------------//
void CAOITreeNode::SetHItem(HTREEITEM val)
{
	m_hItem = val;
}
//-------------------------------------------------------------------------------------//
CAOIObj* CAOITreeNode::GetObjPtr() const
{
	return m_ObjPtr;
}
//-------------------------------------------------------------------------------------//
void CAOITreeNode::SetObjPtr(CAOIObj *Ptr)
{
	m_ObjPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
size_t CAOITreeNode::GetChildNodeCount() const
{
	return m_ChildNodeList.size();
}
//-------------------------------------------------------------------------------------//
CAOITreeNode* CAOITreeNode::GetChildNodePtr(size_t idx, bool bChk)
{
	if ( bChk )
	{
		if ( idx >= m_ChildNodeList.size() )
		{	return NULL; }
	}
	return &(m_ChildNodeList[idx]);
}
//-------------------------------------------------------------------------------------//
void CAOITreeNode::AddChildNode(const CAOITreeNode &Node)
{
	m_ChildNodeList.push_back(Node);
}
//-------------------------------------------------------------------------------------//
void CAOITreeNode::AddChildNode(HTREEITEM item, CAOIObj *Ptr)
{	
	m_ChildNodeList.push_back(CAOITreeNode(item, Ptr));
}
//-------------------------------------------------------------------------------------//
int CAOITreeNode::GetLastChildAdded() const//取得最後子節點已建立完畢
{
	return m_LastChildAdded;
}
//-------------------------------------------------------------------------------------//
void CAOITreeNode::SetLastChildAdded(int idx)//設定最後子節點已建立完畢
{
	m_LastChildAdded = idx;
}
//-------------------------------------------------------------------------------------//
bool CAOITreeNode::GetChildNodeListDone() const//取得子節點已建立完畢	
{
	if ( (m_LastChildAdded+1) != m_ChildNodeList.size() )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//