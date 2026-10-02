// WndDefectItem.cpp: implementation of the CWndDefectItem class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "WndDefectItem.h"
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
CWndDefectItem::CWndDefectItem()
{	
	PreInitWndDefectItem();
	InitialWndDefectItem();
}
//-------------------------------------------------------------------------------------//
CWndDefectItem::~CWndDefectItem()
{
	
}
//-------------------------------------------------------------------------------------//
void CWndDefectItem::PreInitWndDefectItem()
{
	
}
//-------------------------------------------------------------------------------------//
void CWndDefectItem::InitialWndDefectItem()
{	
	SetAllItemCount(0);
	m_LaneID = LANE_ID_NULL;//軌道編號
	m_ResultID = RESULT_ID_NONE;//結果編號
}
//-------------------------------------------------------------------------------------//
void CWndDefectItem::CloneWndDefectItem(const CWndDefectItem &rhs)
{
	return;
}
//-------------------------------------------------------------------------------------//
bool CWndDefectItem::ReadWndDefectItemFile(CAOIFileIO &FileIO)
{
	#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	int    index = 0;
	FileIO.SetFnName(_T("CWndDefectItem::ReadWndDefectItemFile"));
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false ) 		
		{	continue; }		
		
		switch ( index )
		{
		case FILE_IO_DEFECT_ITEM_START://瑕疵項目參數-起點			
			break;
		case FILE_IO_DEFECT_ITEM_END://瑕疵項目參數-終點			
			return true;
			break;
		case FILE_IO_DEFECT_ITEM_NONE://瑕疵項目參數-無定義
			m_None = FileIO.GetData_INT();
			break;		
		case FILE_IO_DEFECT_ITEM_PAD_ALIGN://瑕疵項目參數-焊盤定位
			m_PadAlign = FileIO.GetData_INT();
			break;		
		case FILE_IO_DEFECT_ITEM_PART_ALIGN://瑕疵項目參數-本體定位
			m_PartAlign = FileIO.GetData_INT();
			break;		
		case FILE_IO_DEFECT_ITEM_PAD_ADJUST://瑕疵項目參數-焊盤調整
			m_PadAdjust = FileIO.GetData_INT();
			break;		
		case FILE_IO_DEFECT_ITEM_LEAD_ADJUST://瑕疵項目參數-管腳調整
			m_LeadAdjust = FileIO.GetData_INT();
			break;

		case FILE_IO_DEFECT_ITEM_CLASS_CHECK://瑕疵項目參數-類別確認
			m_ClassCheck = FileIO.GetData_INT();			
			break;
		case FILE_IO_DEFECT_ITEM_BASE_VALUE://瑕疵項目參數-基本數值
			m_BaseValue = FileIO.GetData_INT();			
			break;

		case FILE_IO_DEFECT_ITEM_BODY_MISSING://瑕疵項目參數-缺件
			m_BodyMissing = FileIO.GetData_INT();
			break;		
		case FILE_IO_DEFECT_ITEM_BODY_OFFSET://瑕疵項目參數-偏移
			m_BodyOffset = FileIO.GetData_INT();
			break;		
		case FILE_IO_DEFECT_ITEM_BODY_TILT://瑕疵項目參數-本體傾斜
			m_BodyTilt = FileIO.GetData_INT();
			break;		
		case FILE_IO_DEFECT_ITEM_BODY_POLARITY://瑕疵項目參數-極反
			m_BodyPolarity = FileIO.GetData_INT();
			break;
		case FILE_IO_DEFECT_ITEM_BODY_TURN_OVER://瑕疵項目參數-反件
			m_BodyTurnOver = FileIO.GetData_INT();
			break;		
		case FILE_IO_DEFECT_ITEM_BODY_MOUNT://瑕疵項目參數-裝貼
			m_BodyMount = FileIO.GetData_INT();
			break;		
		case FILE_IO_DEFECT_ITEM_BODY_WRONG_CODE://瑕疵項目參數-條碼
			m_BodyWrongCode = FileIO.GetData_INT();
			break;		
		case FILE_IO_DEFECT_ITEM_BODY_WTRONG_TEXT://瑕疵項目參數-文字
			m_BodyWrongText = FileIO.GetData_INT();
			break;
		case FILE_IO_DEFECT_ITEM_BODY_TOMBSTONE://瑕疵項目參數-立碑
			m_BodyTombstone = FileIO.GetData_INT();
			break;
		case FILE_IO_DEFECT_ITEM_BODY_BILLBOARD://瑕疵項目參數-側立
			m_BodyBillboard = FileIO.GetData_INT();
			break;
		case FILE_IO_DEFECT_ITEM_BODY_DAMAGED://瑕疵項目參數-破損
			m_BodyDamaged = FileIO.GetData_INT();
			break;

		case FILE_IO_DEFECT_ITEM_SOLDER_POOR://瑕疵項目參數-焊錫不足
			m_SolderPoor = FileIO.GetData_INT();
			break;		
		case FILE_IO_DEFECT_ITEM_SOLDER_OPEN://瑕疵項目參數-焊錫空焊
			m_SolderOpen = FileIO.GetData_INT();
			break;		
		case FILE_IO_DEFECT_ITEM_SOLDER_PAD_EXPOSED://瑕疵項目參數-漏銅
			m_SolderPadExposed = FileIO.GetData_INT();
			break;		
		case FILE_IO_DEFECT_ITEM_SOLDER_BRIDGE://瑕疵項目參數-焊錫短路
			m_SolderBridge = FileIO.GetData_INT();
			break;		
		case FILE_IO_DEFECT_ITEM_SOLDER_BEAD://瑕疵項目參數-焊錫錫珠
			m_SolderBead = FileIO.GetData_INT();
			break;
		case FILE_IO_DEFECT_ITEM_SOLDER_EXCESS://瑕疵項目參數-焊錫過量
			m_SolderExcess = FileIO.GetData_INT();
			break;

		case FILE_IO_DEFECT_ITEM_LEAD_LIFTED://瑕疵項目參數-引腳翹起
			m_LeadLifted = FileIO.GetData_INT();
			break;
		case FILE_IO_DEFECT_ITEM_LEAD_BENDED://瑕疵項目參數-引腳彎曲
			m_LeadBended = FileIO.GetData_INT();
			break;		
		case FILE_IO_DEFECT_ITEM_LEAD_PROTRUDED://瑕疵項目參數-引腳凸出
			m_LeadProtruded = FileIO.GetData_INT();
			break;
	
		case FILE_IO_DEFECT_ITEM_PAD_SCRATCH://瑕疵項目參數-焊盤刮傷
			m_PadScratch = FileIO.GetData_INT();
			break;		
		case FILE_IO_DEFECT_ITEM_FOREIGN_BODY://瑕疵項目參數-異物
			m_ForeignBody = FileIO.GetData_INT();
			break;

		case FILE_IO_DEFECT_ITEM_USER_DEFINE_01://瑕疵項目參數-使用者定義-01
			m_UserDefine_01 = FileIO.GetData_INT();			
			break;
		case FILE_IO_DEFECT_ITEM_USER_DEFINE_02://瑕疵項目參數-使用者定義-02
			m_UserDefine_02 = FileIO.GetData_INT();			
			break;
		case FILE_IO_DEFECT_ITEM_USER_DEFINE_03://瑕疵項目參數-使用者定義-03
			m_UserDefine_03 = FileIO.GetData_INT();			
			break;
		case FILE_IO_DEFECT_ITEM_USER_DEFINE_04://瑕疵項目參數-使用者定義-04
			m_UserDefine_04 = FileIO.GetData_INT();			
			break;
		case FILE_IO_DEFECT_ITEM_USER_DEFINE_05://瑕疵項目參數-使用者定義-05
			m_UserDefine_05 = FileIO.GetData_INT();			
			break;
		case FILE_IO_DEFECT_ITEM_USER_DEFINE_06://瑕疵項目參數-使用者定義-06
			m_UserDefine_06 = FileIO.GetData_INT();			
			break;
		case FILE_IO_DEFECT_ITEM_USER_DEFINE_07://瑕疵項目參數-使用者定義-07
			m_UserDefine_07 = FileIO.GetData_INT();			
			break;
		case FILE_IO_DEFECT_ITEM_USER_DEFINE_08://瑕疵項目參數-使用者定義-08
			m_UserDefine_08 = FileIO.GetData_INT();			
			break;
		case FILE_IO_DEFECT_ITEM_USER_DEFINE_09://瑕疵項目參數-使用者定義-09
			m_UserDefine_09 = FileIO.GetData_INT();			
			break;
		case FILE_IO_DEFECT_ITEM_USER_DEFINE_10://瑕疵項目參數-使用者定義-10
			m_UserDefine_10 = FileIO.GetData_INT();			
			break;
				
		default:
			break;
		}
		
	};		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CWndDefectItem::WriteWndDefectItemFile(CAOIFileIO &FileIO) const
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }	
#endif//_DEBUG

	FileIO.SetFnName(_T("CWndDefectItem::WriteWndDefectItemFile"));
	//----------------------------------------------------------------------------------------//	
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_START, 0) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_NONE, m_None) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_PAD_ALIGN, m_PadAlign) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_PART_ALIGN, m_PartAlign) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_PAD_ADJUST, m_PadAdjust) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_LEAD_ADJUST, m_LeadAdjust) == false ) { return false; }
	
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_CLASS_CHECK, m_ClassCheck) == false ) { return false; }	
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_BASE_VALUE, m_BaseValue) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_BODY_MISSING, m_BodyMissing) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_BODY_OFFSET, m_BodyOffset) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_BODY_TILT, m_BodyTilt) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_BODY_POLARITY, m_BodyPolarity) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_BODY_TURN_OVER, m_BodyTurnOver) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_BODY_MOUNT, m_BodyMount) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_BODY_WRONG_CODE, m_BodyWrongCode) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_BODY_WTRONG_TEXT, m_BodyWrongText) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_BODY_TOMBSTONE, m_BodyTombstone) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_BODY_BILLBOARD, m_BodyBillboard) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_BODY_DAMAGED, m_BodyDamaged) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_SOLDER_POOR, m_SolderPoor) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_SOLDER_OPEN, m_SolderOpen) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_SOLDER_PAD_EXPOSED, m_SolderPadExposed) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_SOLDER_BRIDGE, m_SolderBridge) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_SOLDER_BEAD, m_SolderBead) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_SOLDER_EXCESS, m_SolderExcess) == false ) { return false; }	

	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_LEAD_LIFTED, m_LeadLifted) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_LEAD_BENDED, m_LeadBended) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_LEAD_PROTRUDED, m_LeadProtruded) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_PAD_SCRATCH, m_PadScratch) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_FOREIGN_BODY, m_ForeignBody) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_USER_DEFINE_01, m_UserDefine_01) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_USER_DEFINE_02, m_UserDefine_02) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_USER_DEFINE_03, m_UserDefine_03) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_USER_DEFINE_04, m_UserDefine_04) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_USER_DEFINE_05, m_UserDefine_05) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_USER_DEFINE_06, m_UserDefine_06) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_USER_DEFINE_07, m_UserDefine_07) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_USER_DEFINE_08, m_UserDefine_08) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_USER_DEFINE_09, m_UserDefine_09) == false ) { return false; }
	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_USER_DEFINE_10, m_UserDefine_10) == false ) { return false; }

	if ( FileIO.SaveChunk_INT(FILE_IO_DEFECT_ITEM_END, 0) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
LANE_ID CWndDefectItem::GetLaneID() const
{
	return m_LaneID;
}
//-------------------------------------------------------------------------------------//
void CWndDefectItem::SetLaneID(LANE_ID val)
{
	m_LaneID = val;
}
//-------------------------------------------------------------------------------------//
RESULT_ID CWndDefectItem::GetResultID() const
{
	return m_ResultID;
}
//-------------------------------------------------------------------------------------//
void CWndDefectItem::SetResultID(RESULT_ID val)
{
	m_ResultID = val;
}
//-------------------------------------------------------------------------------------//
void CWndDefectItem::BuildWndDefectItemAlarm()//建成警報項目
{	//警報項目沒有[WND_DEFECT_ITEM_NO_SHOW]
	int  ItemDisable=WND_DEFECT_ITEM_DISABLE;
	if ( WND_DEFECT_ITEM_ENABLE != m_PadAlign )
	{	m_PadAlign = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_PartAlign )
	{	m_PartAlign = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_PadAdjust )
	{	m_PadAdjust = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_LeadAdjust )
	{	m_LeadAdjust = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_ClassCheck )
	{	m_ClassCheck = ItemDisable; }	

	if ( WND_DEFECT_ITEM_ENABLE != m_BaseValue )
	{	m_BaseValue = ItemDisable; }	

	if ( WND_DEFECT_ITEM_ENABLE != m_BodyMissing )
	{	m_BodyMissing = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_BodyOffset )
	{	m_BodyOffset = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_BodyTilt )
	{	m_BodyTilt = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_BodyPolarity )
	{	m_BodyPolarity = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_BodyTurnOver )
	{	m_BodyTurnOver = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_BodyMount )
	{	m_BodyMount = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_BodyWrongCode )
	{	m_BodyWrongCode = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_BodyWrongText )
	{	m_BodyWrongText = ItemDisable; }
	
	if ( WND_DEFECT_ITEM_ENABLE != m_BodyTombstone )
	{	m_BodyTombstone = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_BodyBillboard )
	{	m_BodyBillboard = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_BodyDamaged )
	{	m_BodyDamaged = ItemDisable; }	

	if ( WND_DEFECT_ITEM_ENABLE != m_SolderPoor )
	{	m_SolderPoor = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_SolderOpen )
	{	m_SolderOpen = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_SolderPadExposed )
	{	m_SolderPadExposed = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_SolderBridge )
	{	m_SolderBridge = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_SolderBead )
	{	m_SolderBead = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_SolderExcess )
	{	m_SolderExcess = ItemDisable; }
	

	if ( WND_DEFECT_ITEM_ENABLE != m_LeadLifted )
	{	m_LeadLifted = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_LeadBended )
	{	m_LeadBended = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_LeadProtruded )
	{	m_LeadProtruded = ItemDisable; }


	if ( WND_DEFECT_ITEM_ENABLE != m_PadScratch )
	{	m_PadScratch = ItemDisable; }

	if ( WND_DEFECT_ITEM_ENABLE != m_ForeignBody )
	{	m_ForeignBody = ItemDisable; }	

	if ( WND_DEFECT_ITEM_ENABLE != m_UserDefine_01 )
	{	m_UserDefine_01 = ItemDisable; }	
	if ( WND_DEFECT_ITEM_ENABLE != m_UserDefine_02 )
	{	m_UserDefine_02 = ItemDisable; }
	if ( WND_DEFECT_ITEM_ENABLE != m_UserDefine_03 )
	{	m_UserDefine_03 = ItemDisable; }
	if ( WND_DEFECT_ITEM_ENABLE != m_UserDefine_04 )
	{	m_UserDefine_04 = ItemDisable; }
	if ( WND_DEFECT_ITEM_ENABLE != m_UserDefine_05 )
	{	m_UserDefine_05 = ItemDisable; }
	if ( WND_DEFECT_ITEM_ENABLE != m_UserDefine_06 )
	{	m_UserDefine_06 = ItemDisable; }
	if ( WND_DEFECT_ITEM_ENABLE != m_UserDefine_07 )
	{	m_UserDefine_07 = ItemDisable; }
	if ( WND_DEFECT_ITEM_ENABLE != m_UserDefine_08 )
	{	m_UserDefine_08 = ItemDisable; }	
	if ( WND_DEFECT_ITEM_ENABLE != m_UserDefine_09 )
	{	m_UserDefine_09 = ItemDisable; }	
	if ( WND_DEFECT_ITEM_ENABLE != m_UserDefine_10 )
	{	m_UserDefine_10 = ItemDisable; }	
	return ;
}
//-------------------------------------------------------------------------------------//
void CWndDefectItem::BuildModelBasicTestItem(MODEL_TYPE Type)//建立模組基本檢測項目
{
	const int nEnable=1;
	SetAllItemCount(0);

	//m_None;
	m_PadAlign = nEnable;
	m_PartAlign = nEnable;
	//m_PadAdjust = nEnable;
	//m_LeadAdjust = nEnable;

	//m_ClassCheck = nEnable;
	//m_BaseValue = nEnable;

	m_BodyMissing = nEnable;
	m_BodyOffset = nEnable;
	m_BodyTilt = nEnable;
	m_BodyPolarity = nEnable;
	//m_BodyTurnOver = nEnable;
	m_BodyMount = nEnable;
	//m_BodyWrongCode = nEnable;
	m_BodyWrongText = nEnable;		
	//m_BodyTombstone = nEnable;		
	//m_BodyBillboard = nEnable;		
	//m_BodyDamaged = nEnable;

	m_SolderPoor = nEnable;		
	m_SolderOpen = nEnable;		
	//m_SolderPadExposed = nEnable;		
	m_SolderBridge = nEnable;		
	//m_SolderBead = nEnable;		
	//m_SolderExcess = nEnable;		
	
	m_LeadLifted = nEnable;		
	m_LeadBended = nEnable;		
	m_LeadProtruded = nEnable;		

	m_PadScratch = nEnable;		
	m_ForeignBody = nEnable;

	m_UserDefine_01 = nEnable;
	m_UserDefine_02 = nEnable;
	m_UserDefine_03 = nEnable;
	m_UserDefine_04 = nEnable;
	m_UserDefine_05 = nEnable;
	m_UserDefine_06 = nEnable;
	m_UserDefine_07 = nEnable;
	m_UserDefine_08 = nEnable;
	m_UserDefine_09 = nEnable;
	m_UserDefine_10 = nEnable;
	switch ( Type )
	{
	case MODEL_TYPE_CHIP:
	case MODEL_TYPE_CHIP_C:
	case MODEL_TYPE_CHIP_R:
	case MODEL_TYPE_CHIP_L:
	case MODEL_TYPE_CHIP_LED:
	case MODEL_TYPE_MELF:
		m_SolderBridge = 0;		
		break;
	}		
	return ;
}
//-------------------------------------------------------------------------------------//
void CWndDefectItem::SetAllItemCount(int Count)
{	
	if ( 0 == Count ) 
	{	m_ResultID = RESULT_ID_NONE; }
	else
	{	m_ResultID = RESULT_ID_NG; }

	m_None = Count;//無定義	
	m_PadAlign = Count;//焊盤定位
	m_PartAlign = Count;//本體定位
	m_PadAdjust = Count;//焊盤調整
	m_LeadAdjust = Count;//管腳調整

	m_ClassCheck = Count;//類別確認
	m_BaseValue = Count;//基準數值

	m_BodyMissing = Count;//缺件
	m_BodyOffset = Count;//偏移
	m_BodyTilt = Count;//本體傾斜
	m_BodyPolarity = Count;//極反
	m_BodyTurnOver = Count;//反件
	m_BodyMount = Count;//錯件-裝貼
	m_BodyWrongCode = Count;//錯件-條碼
	m_BodyWrongText = Count;//錯件-文字
	m_BodyTombstone = Count;//立碑
	m_BodyBillboard = Count;//側立
	m_BodyDamaged = Count;//破損

	m_SolderPoor = Count;//焊錫不足
	m_SolderOpen = Count;//焊錫空焊
	m_SolderPadExposed = Count;//焊錫沒有-漏銅
	m_SolderBridge = Count;//焊錫短路
	m_SolderBead = Count;//焊錫錫珠
	m_SolderExcess = Count;//焊錫過量

	m_LeadLifted = Count;//引腳翹起
	m_LeadBended = Count;//引腳彎曲
	m_LeadProtruded = Count;//引腳凸出

	m_PadScratch = Count;//焊盤刮傷
	m_ForeignBody = Count;//異物

	m_UserDefine_01 = Count;//使用者定義-01
	m_UserDefine_02 = Count;//使用者定義-02
	m_UserDefine_03 = Count;//使用者定義-03
	m_UserDefine_04 = Count;//使用者定義-04
	m_UserDefine_05 = Count;//使用者定義-05
	m_UserDefine_06 = Count;//使用者定義-06
	m_UserDefine_07 = Count;//使用者定義-07
	m_UserDefine_08 = Count;//使用者定義-08
	m_UserDefine_09 = Count;//使用者定義-09
	m_UserDefine_10 = Count;//使用者定義-10
}
//-------------------------------------------------------------------------------------//
int CWndDefectItem::CalcAllItemCount() const//計算所有項目數量
{
	int Sum=0;
	//Sum += m_None;
	Sum += m_PadAlign;
	Sum += m_PartAlign;
	Sum += m_PadAdjust;
	Sum += m_LeadAdjust;

	Sum += m_ClassCheck;
	Sum += m_BaseValue;

	Sum += m_BodyMissing;
	Sum += m_BodyOffset;
	Sum += m_BodyTilt;
	Sum += m_BodyPolarity;
	Sum += m_BodyTurnOver;
	Sum += m_BodyMount;
	Sum += m_BodyWrongCode;
	Sum += m_BodyWrongText;
	Sum += m_BodyTombstone;
	Sum += m_BodyBillboard;
	Sum += m_BodyDamaged;

	Sum += m_SolderPoor;
	Sum += m_SolderOpen;
	Sum += m_SolderPadExposed;
	Sum += m_SolderBridge;
	Sum += m_SolderBead;
	Sum += m_SolderExcess;

	Sum += m_LeadLifted;
	Sum += m_LeadBended;
	Sum += m_LeadProtruded;

	Sum += m_PadScratch;
	Sum += m_ForeignBody;

	Sum += m_UserDefine_01;
	Sum += m_UserDefine_02;
	Sum += m_UserDefine_03;
	Sum += m_UserDefine_04;
	Sum += m_UserDefine_05;
	Sum += m_UserDefine_06;
	Sum += m_UserDefine_07;
	Sum += m_UserDefine_08;
	Sum += m_UserDefine_09;
	Sum += m_UserDefine_10;

	return Sum;
}
//-------------------------------------------------------------------------------------//
bool CWndDefectItem::AddItemCount(WND_DEFECT_ID DefectID)//增加特定瑕疵項目數量
{
	bool bSucc=true;
	switch ( DefectID )
	{
	case WND_DEFECT_NONE:	m_None ++;	break;

	case WND_DEFECT_PAD_ALIGN:	m_PadAlign ++;	break;
	case WND_DEFECT_PART_ALIGN:	m_PartAlign ++;	break;
	case WND_DEFECT_PAD_ADJUST:	m_PadAdjust ++;	break;
	case WND_DEFECT_LEAD_ADJUST:	m_LeadAdjust ++;	break;

	case WND_DEFECT_CLASS_CHECK:     m_ClassCheck ++;  break;
	case WND_DEFECT_BASE_VALUE:     m_BaseValue ++;  break;

	case WND_DEFECT_BODY_MISSING:	m_BodyMissing ++;	break;
	case WND_DEFECT_BODY_OFFSET:	m_BodyOffset ++;	break;
	case WND_DEFECT_BODY_TILT:	m_BodyTilt ++;	break;
	case WND_DEFECT_BODY_POLARITY:	m_BodyPolarity ++;	break;
	case WND_DEFECT_BODY_TURNOVER:	m_BodyTurnOver ++;	break;
	case WND_DEFECT_BODY_MOUNT:	m_BodyMount ++;	break;
	case WND_DEFECT_BODY_WRONG_CODE:	m_BodyWrongCode ++;	break;
	case WND_DEFECT_BODY_WRONG_TEXT:	m_BodyWrongText ++;	break;
	case WND_DEFECT_BODY_TOMBSTONE:		m_BodyTombstone ++;	break;
	case WND_DEFECT_BODY_BILLBOARD:		m_BodyBillboard ++;	break;
	case WND_DEFECT_BODY_DAMAGED:		m_BodyDamaged ++; break;

	case WND_DEFECT_SOLDER_POOR:	m_SolderPoor ++;	break;
	case WND_DEFECT_SOLDER_OPEN:	m_SolderOpen ++;	break;
	case WND_DEFECT_SOLDER_PAD_EXPOSED:	m_SolderPadExposed ++;	break;
	case WND_DEFECT_SOLDER_BRIDGE:	m_SolderBridge ++;	break;
	case WND_DEFECT_SOLDER_BEAD:	m_SolderBead ++;	break;
	case WND_DEFECT_SOLDER_EXCESS:	m_SolderExcess ++;	break;

	case WND_DEFECT_LEAD_LIFTED:	m_LeadLifted ++;	break;
	case WND_DEFECT_LEAD_BENDED:	m_LeadBended ++;	break;
	case WND_DEFECT_LEAD_PROTRUDED:	m_LeadProtruded ++;	break;

	case WND_DEFECT_PAD_SCRATCH:	m_PadScratch ++;	break;
	case WND_DEFECT_FOREIGN_BODY:	m_ForeignBody ++;	break;

	case WND_DEFECT_USER_DEFINE_01: m_UserDefine_01 ++;	break;
	case WND_DEFECT_USER_DEFINE_02: m_UserDefine_02 ++;	break;
	case WND_DEFECT_USER_DEFINE_03: m_UserDefine_03 ++;	break;
	case WND_DEFECT_USER_DEFINE_04: m_UserDefine_04 ++;	break;
	case WND_DEFECT_USER_DEFINE_05: m_UserDefine_05 ++;	break;
	case WND_DEFECT_USER_DEFINE_06: m_UserDefine_06 ++;	break;
	case WND_DEFECT_USER_DEFINE_07: m_UserDefine_07 ++;	break;
	case WND_DEFECT_USER_DEFINE_08: m_UserDefine_08 ++;	break;
	case WND_DEFECT_USER_DEFINE_09: m_UserDefine_09 ++;	break;
	case WND_DEFECT_USER_DEFINE_10: m_UserDefine_10 ++;	break;

	default:	
		bSucc = false;
		break;
	}	
	return bSucc;
}
//-------------------------------------------------------------------------------------//
int CWndDefectItem::GetItemCount(WND_DEFECT_ID DefectID) const//取得特定瑕疵項目數量
{
	int Count=0;
	switch ( DefectID ) 
	{
	case WND_DEFECT_NONE: Count = m_None;	break;
	case WND_DEFECT_PAD_ALIGN: Count = m_PadAlign;	break;
	case WND_DEFECT_PART_ALIGN: Count = m_PartAlign;	break;
	case WND_DEFECT_PAD_ADJUST: Count = m_PadAdjust;	break;
	case WND_DEFECT_LEAD_ADJUST: Count = m_LeadAdjust;	break;

	case WND_DEFECT_CLASS_CHECK: Count = m_ClassCheck;	break;
	case WND_DEFECT_BASE_VALUE:  Count = m_BaseValue;    break;

	case WND_DEFECT_BODY_MISSING: Count = m_BodyMissing;	break;
	case WND_DEFECT_BODY_OFFSET: Count = m_BodyOffset;	break;
	case WND_DEFECT_BODY_TILT: Count = m_BodyTilt;	break;
	case WND_DEFECT_BODY_POLARITY: Count = m_BodyPolarity;	break;
	case WND_DEFECT_BODY_TURNOVER: Count = m_BodyTurnOver;	break;
	case WND_DEFECT_BODY_MOUNT: Count = m_BodyMount;	break;
	case WND_DEFECT_BODY_WRONG_CODE: Count = m_BodyWrongCode;	break;
	case WND_DEFECT_BODY_WRONG_TEXT: Count = m_BodyWrongText;	break;
	case WND_DEFECT_BODY_TOMBSTONE: Count = m_BodyTombstone;	break;
	case WND_DEFECT_BODY_BILLBOARD: Count = m_BodyBillboard;	break;
	case WND_DEFECT_BODY_DAMAGED:	Count = m_BodyDamaged; break;

	case WND_DEFECT_SOLDER_POOR: Count = m_SolderPoor;	break;
	case WND_DEFECT_SOLDER_OPEN: Count = m_SolderOpen;	break;
	case WND_DEFECT_SOLDER_PAD_EXPOSED: Count = m_SolderPadExposed;	break;
	case WND_DEFECT_SOLDER_BRIDGE: Count = m_SolderBridge;	break;
	case WND_DEFECT_SOLDER_BEAD: Count = m_SolderBead;	break;
	case WND_DEFECT_SOLDER_EXCESS: Count = m_SolderExcess;	break;
		
	case WND_DEFECT_LEAD_LIFTED: Count = m_LeadLifted;	break;
	case WND_DEFECT_LEAD_BENDED: Count = m_LeadBended;	break;
	case WND_DEFECT_LEAD_PROTRUDED: Count = m_LeadProtruded;	break;
	
	case WND_DEFECT_PAD_SCRATCH: Count = m_PadScratch;	break;
	case WND_DEFECT_FOREIGN_BODY: Count = m_ForeignBody;	break;

	case WND_DEFECT_USER_DEFINE_01: Count = m_UserDefine_01;	break;
	case WND_DEFECT_USER_DEFINE_02: Count = m_UserDefine_02;	break;
	case WND_DEFECT_USER_DEFINE_03: Count = m_UserDefine_03;	break;
	case WND_DEFECT_USER_DEFINE_04: Count = m_UserDefine_04;	break;
	case WND_DEFECT_USER_DEFINE_05: Count = m_UserDefine_05;	break;
	case WND_DEFECT_USER_DEFINE_06: Count = m_UserDefine_06;	break;
	case WND_DEFECT_USER_DEFINE_07: Count = m_UserDefine_07;	break;
	case WND_DEFECT_USER_DEFINE_08: Count = m_UserDefine_08;	break;
	case WND_DEFECT_USER_DEFINE_09: Count = m_UserDefine_09;	break;
	case WND_DEFECT_USER_DEFINE_10: Count = m_UserDefine_10;	break;
	
	default:
		Count = 0;
		break;
	}  
	return Count;
}
//-------------------------------------------------------------------------------------//
bool CWndDefectItem::SetItemCount(WND_DEFECT_ID DefectID, int Count)
{
	bool bSucc=true;
	switch ( DefectID ) 
	{
	case WND_DEFECT_NONE: m_None = Count;	break;
	case WND_DEFECT_PAD_ALIGN: m_PadAlign = Count;	break;
	case WND_DEFECT_PART_ALIGN: m_PartAlign = Count;	break;
	case WND_DEFECT_PAD_ADJUST: m_PadAdjust = Count;	break;
	case WND_DEFECT_LEAD_ADJUST: m_LeadAdjust = Count;	break;
	
	case WND_DEFECT_CLASS_CHECK:  m_ClassCheck = Count;    break;
	case WND_DEFECT_BASE_VALUE:  m_BaseValue = Count;    break;

	case WND_DEFECT_BODY_MISSING: m_BodyMissing = Count;	break;
	case WND_DEFECT_BODY_OFFSET: m_BodyOffset = Count;	break;
	case WND_DEFECT_BODY_TILT: m_BodyTilt = Count;	break;
	case WND_DEFECT_BODY_POLARITY: m_BodyPolarity = Count;	break;
	case WND_DEFECT_BODY_TURNOVER: m_BodyTurnOver = Count;	break;
	case WND_DEFECT_BODY_MOUNT: m_BodyMount = Count;	break;
	case WND_DEFECT_BODY_WRONG_CODE: m_BodyWrongCode = Count;	break;
	case WND_DEFECT_BODY_WRONG_TEXT: m_BodyWrongText = Count;	break;
	case WND_DEFECT_BODY_TOMBSTONE: m_BodyTombstone = Count;	break;
	case WND_DEFECT_BODY_BILLBOARD: m_BodyBillboard = Count;	break;
	case WND_DEFECT_BODY_DAMAGED:	m_BodyDamaged = Count; break;

	case WND_DEFECT_SOLDER_POOR: m_SolderPoor = Count;	break;
	case WND_DEFECT_SOLDER_OPEN: m_SolderOpen = Count;	break;
	case WND_DEFECT_SOLDER_PAD_EXPOSED: m_SolderPadExposed = Count;	break;
	case WND_DEFECT_SOLDER_BRIDGE: m_SolderBridge = Count;	break;
	case WND_DEFECT_SOLDER_BEAD: m_SolderBead = Count;	break;
	case WND_DEFECT_SOLDER_EXCESS: m_SolderExcess = Count;	break;
		
	case WND_DEFECT_LEAD_LIFTED: m_LeadLifted = Count;	break;
	case WND_DEFECT_LEAD_BENDED: m_LeadBended = Count;	break;
	case WND_DEFECT_LEAD_PROTRUDED: m_LeadProtruded = Count;	break;
	
	case WND_DEFECT_PAD_SCRATCH: m_PadScratch = Count;	break;
	case WND_DEFECT_FOREIGN_BODY: m_ForeignBody = Count;	break;

	case WND_DEFECT_USER_DEFINE_01: m_UserDefine_01 = Count;	break;
	case WND_DEFECT_USER_DEFINE_02: m_UserDefine_02 = Count;	break;
	case WND_DEFECT_USER_DEFINE_03: m_UserDefine_03 = Count;	break;
	case WND_DEFECT_USER_DEFINE_04: m_UserDefine_04 = Count;	break;
	case WND_DEFECT_USER_DEFINE_05: m_UserDefine_05 = Count;	break;
	case WND_DEFECT_USER_DEFINE_06: m_UserDefine_06 = Count;	break;
	case WND_DEFECT_USER_DEFINE_07: m_UserDefine_07 = Count;	break;
	case WND_DEFECT_USER_DEFINE_08: m_UserDefine_08 = Count;	break;
	case WND_DEFECT_USER_DEFINE_09: m_UserDefine_09 = Count;	break;
	case WND_DEFECT_USER_DEFINE_10: m_UserDefine_10 = Count;	break;

	default:	
		bSucc = false;
		break;
	}  
	return bSucc;
}
//-------------------------------------------------------------------------------------//
void CWndDefectItem::AddWndDefectItemCount(const CWndDefectItem &rhs)//加入瑕疵項目數量
{
	if ( 0 < rhs.m_None ) { m_None ++; }

	if ( 0 < rhs.m_PadAlign ) { m_PadAlign ++; }
	if ( 0 < rhs.m_PartAlign ) { m_PartAlign ++; }
	if ( 0 < rhs.m_PadAdjust ) { m_PadAdjust ++; }
	if ( 0 < rhs.m_LeadAdjust ) { m_LeadAdjust ++; }

	if ( 0 < rhs.m_ClassCheck ) { m_ClassCheck ++; }	
	if ( 0 < rhs.m_BaseValue ) { m_BaseValue ++; }	

	if ( 0 < rhs.m_BodyMissing ) { m_BodyMissing ++; }
	if ( 0 < rhs.m_BodyOffset ) { m_BodyOffset ++; }
	if ( 0 < rhs.m_BodyTilt ) { m_BodyTilt ++; }
	if ( 0 < rhs.m_BodyPolarity ) { m_BodyPolarity ++; }
	if ( 0 < rhs.m_BodyTurnOver ) { m_BodyTurnOver ++; }
	if ( 0 < rhs.m_BodyMount ) { m_BodyMount ++; }
	if ( 0 < rhs.m_BodyWrongCode ) { m_BodyWrongCode ++; }
	if ( 0 < rhs.m_BodyWrongText ) { m_BodyWrongText ++; }
	if ( 0 < rhs.m_BodyTombstone ) { m_BodyTombstone ++; }
	if ( 0 < rhs.m_BodyBillboard ) { m_BodyBillboard ++; }
	if ( 0 < rhs.m_BodyDamaged ) { m_BodyDamaged ++; }	

	if ( 0 < rhs.m_SolderPoor ) { m_SolderPoor ++; }
	if ( 0 < rhs.m_SolderOpen ) { m_SolderOpen ++; }
	if ( 0 < rhs.m_SolderPadExposed ) { m_SolderPadExposed ++; }
	if ( 0 < rhs.m_SolderBridge ) { m_SolderBridge ++; }
	if ( 0 < rhs.m_SolderBead ) { m_SolderBead ++; }
	if ( 0 < rhs.m_SolderExcess ) { m_SolderExcess ++; }

	if ( 0 < rhs.m_LeadLifted ) { m_LeadLifted ++; }
	if ( 0 < rhs.m_LeadBended ) { m_LeadBended ++; }
	if ( 0 < rhs.m_LeadProtruded ) { m_LeadProtruded ++; }

	if ( 0 < rhs.m_PadScratch ) { m_PadScratch ++; }
	if ( 0 < rhs.m_ForeignBody ) { m_ForeignBody ++; }

	if ( 0 < rhs.m_UserDefine_01 ) { m_UserDefine_01 ++; }
	if ( 0 < rhs.m_UserDefine_02 ) { m_UserDefine_02 ++; }
	if ( 0 < rhs.m_UserDefine_03 ) { m_UserDefine_03 ++; }
	if ( 0 < rhs.m_UserDefine_04 ) { m_UserDefine_04 ++; }
	if ( 0 < rhs.m_UserDefine_05 ) { m_UserDefine_05 ++; }
	if ( 0 < rhs.m_UserDefine_06 ) { m_UserDefine_06 ++; }
	if ( 0 < rhs.m_UserDefine_07 ) { m_UserDefine_07 ++; }
	if ( 0 < rhs.m_UserDefine_08 ) { m_UserDefine_08 ++; }	
	if ( 0 < rhs.m_UserDefine_09 ) { m_UserDefine_09 ++; }	
	if ( 0 < rhs.m_UserDefine_10 ) { m_UserDefine_10 ++; }
	return;
}
//-------------------------------------------------------------------------------------//