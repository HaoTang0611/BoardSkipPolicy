#include "StdAfx.h"
#include "TreeCtrlDef.h"

LPARAM  MakeItemlParam(size_t ItemType, size_t ItemIndex)
{	
	ItemType = (ItemType&0xFF)<<24;	
	return (ItemType)+(ItemIndex&0x00FFFFFF);	
}
//ItemType::0 ~ 255(8bit), ItemIndex::0 ~ 16777216(24bit)
void DecodeItemlParam(LPARAM lParam, unsigned int &ItemType, unsigned int &ItemIndex)
{
	ItemType  = (lParam&0xFF000000)>>24;	
	ItemIndex = lParam&0x00FFFFFF;
}