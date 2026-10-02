// AOIWindow.cpp: implementation of the CAOIWindow class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIWindow.h"
//-------------------------------------------------------------------------------------//
#include "AOIComponent.h"
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
IMPLEMENT_DYNAMIC(CAOIWindow, CAOIRgn)
//-------------------------------------------------------------------------------------//
CAOIWindow::CAOIWindow():CAOIRgn(AOI_OBJ_WINDOW)
{
	PreInitWindow();
	InitialWindow();
}
//-------------------------------------------------------------------------------------//
CAOIWindow::CAOIWindow(const CAOIWindow &window):CAOIRgn(window)
{
	PreInitWindow();
	CloneWindow(window);
}
//-------------------------------------------------------------------------------------//
CAOIWindow::~CAOIWindow()
{

}
//-------------------------------------------------------------------------------------//
CAOIWindow& CAOIWindow::operator=(const CAOIWindow &window)
{
	if ( this == &window ) { return *this; }
	CAOIRgn::operator=(window);
	CloneWindow(window);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CAOIWindow::PreInitWindow()
{	
	//---------------------------------------------------------------------------------//
	m_WindowIndex = -1;
	m_WindowComponentIdx = -1;
	m_WindowComponentPtr = NULL;
}
//-------------------------------------------------------------------------------------//
inline void CAOIWindow::InitialWindow()
{	
	m_WindowSelected = false;
	m_WindowDeleted = false;
}
//-------------------------------------------------------------------------------------//
inline void CAOIWindow::CloneWindow(const CAOIWindow &window)
{	
	m_WindowIndex = window.m_WindowIndex;
	m_WindowComponentIdx = window.m_WindowComponentIdx;
	m_WindowComponentPtr = window.m_WindowComponentPtr;
	m_WindowSelected = window.m_WindowSelected;
	m_WindowDeleted = window.m_WindowDeleted;
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CAOIWindow::GetRgnDerivedName() const//取得區域的名稱
{
	return CString(_T("Window"));
}
//-------------------------------------------------------------------------------------//
CString CAOIWindow::GetRgnDerivedKeyName() const//取得檢測框的名稱	
{
	return CString(_T("Window"));
}
//-------------------------------------------------------------------------------------//
void CAOIWindow::SetWindowComponentPtr(CAOIComponent *Ptr)
{
	if ( NULL == Ptr )
	{	this->m_WindowComponentIdx = -1;	}
	else
	{
		this->m_WindowComponentIdx = Ptr->GetComponentIndex_Project();
	}
}
//-------------------------------------------------------------------------------------//
void CAOIWindow::GetWindowRoiCadRegion(TREGION4D &Region)//取得檢測框在Cad的範圍	
{
	Region.minX = Region.maxX = CAOIRgn::m_RgnRoiCadCornerPos[0].x;
	Region.minY = Region.maxY = CAOIRgn::m_RgnRoiCadCornerPos[0].y;

	if ( Region.minX > CAOIRgn::m_RgnRoiCadCornerPos[1].x ) { Region.minX = CAOIRgn::m_RgnRoiCadCornerPos[1].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiCadCornerPos[1].y ) { Region.minY = CAOIRgn::m_RgnRoiCadCornerPos[1].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiCadCornerPos[1].x ) { Region.maxX = CAOIRgn::m_RgnRoiCadCornerPos[1].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiCadCornerPos[1].y ) { Region.maxY = CAOIRgn::m_RgnRoiCadCornerPos[1].y; }

	if ( Region.minX > CAOIRgn::m_RgnRoiCadCornerPos[2].x ) { Region.minX = CAOIRgn::m_RgnRoiCadCornerPos[2].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiCadCornerPos[2].y ) { Region.minY = CAOIRgn::m_RgnRoiCadCornerPos[2].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiCadCornerPos[2].x ) { Region.maxX = CAOIRgn::m_RgnRoiCadCornerPos[2].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiCadCornerPos[2].y ) { Region.maxY = CAOIRgn::m_RgnRoiCadCornerPos[2].y; }

	if ( Region.minX > CAOIRgn::m_RgnRoiCadCornerPos[3].x ) { Region.minX = CAOIRgn::m_RgnRoiCadCornerPos[3].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiCadCornerPos[3].y ) { Region.minY = CAOIRgn::m_RgnRoiCadCornerPos[3].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiCadCornerPos[3].x ) { Region.maxX = CAOIRgn::m_RgnRoiCadCornerPos[3].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiCadCornerPos[3].y ) { Region.maxY = CAOIRgn::m_RgnRoiCadCornerPos[3].y; }
}
//-------------------------------------------------------------------------------------//
void CAOIWindow::GetWindowRoiStageRegion(TREGION4D &Region)//取得檢測框在Stage的範圍	
{
	Region.minX = Region.maxX = CAOIRgn::m_RgnRoiStageCornerPos[0].x;
	Region.minY = Region.maxY = CAOIRgn::m_RgnRoiStageCornerPos[0].y;

	if ( Region.minX > CAOIRgn::m_RgnRoiStageCornerPos[1].x ) { Region.minX = CAOIRgn::m_RgnRoiStageCornerPos[1].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiStageCornerPos[1].y ) { Region.minY = CAOIRgn::m_RgnRoiStageCornerPos[1].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiStageCornerPos[1].x ) { Region.maxX = CAOIRgn::m_RgnRoiStageCornerPos[1].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiStageCornerPos[1].y ) { Region.maxY = CAOIRgn::m_RgnRoiStageCornerPos[1].y; }

	if ( Region.minX > CAOIRgn::m_RgnRoiStageCornerPos[2].x ) { Region.minX = CAOIRgn::m_RgnRoiStageCornerPos[2].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiStageCornerPos[2].y ) { Region.minY = CAOIRgn::m_RgnRoiStageCornerPos[2].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiStageCornerPos[2].x ) { Region.maxX = CAOIRgn::m_RgnRoiStageCornerPos[2].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiStageCornerPos[2].y ) { Region.maxY = CAOIRgn::m_RgnRoiStageCornerPos[2].y; }

	if ( Region.minX > CAOIRgn::m_RgnRoiStageCornerPos[3].x ) { Region.minX = CAOIRgn::m_RgnRoiStageCornerPos[3].x; }
	if ( Region.minY > CAOIRgn::m_RgnRoiStageCornerPos[3].y ) { Region.minY = CAOIRgn::m_RgnRoiStageCornerPos[3].y; }
	if ( Region.maxX < CAOIRgn::m_RgnRoiStageCornerPos[3].x ) { Region.maxX = CAOIRgn::m_RgnRoiStageCornerPos[3].x; }
	if ( Region.maxY < CAOIRgn::m_RgnRoiStageCornerPos[3].y ) { Region.maxY = CAOIRgn::m_RgnRoiStageCornerPos[3].y; }
}
//-------------------------------------------------------------------------------------//
void CAOIWindow::CalcWindowCadCornerPos()//計算檢測框Cad端點座標	
{
	CAOIRgn::CalcRgnCadCornerPos();
}
//-------------------------------------------------------------------------------------//
void CAOIWindow::LayoutWindowStageCornerPos()//更新檢測框機台端點座標	
{
	CAOIRgn::LayoutRgnStageCornerPos();
}
//-------------------------------------------------------------------------------------//
void  CAOIWindow::MapWindowCadToStagePos(const CMapCoordinate &Map)//將檢測框CAD轉成機台座標
{
	CAOIRgn::MapRgnCadToStagePos(Map);
}
//-------------------------------------------------------------------------------------//
bool CAOIWindow::SpinWindow(double Angle)//檢測框自旋轉
{
	double CadAngle = 0;
	double StageAngle = 0;
	CadAngle = Angle;
	AOIDataCollect.MapCadAngleToStage(CadAngle, StageAngle);
	const double CadCpX = CAOIRgn::m_RgnCadPos.x;
	const double CadCpY = CAOIRgn::m_RgnCadPos.y;
	const double StageCpX = CAOIRgn::m_RgnStagePos.x;
	const double StageCpY = CAOIRgn::m_RgnStagePos.y;
	CAOIWindow::RotateWindowCad(CadAngle, CadCpX, CadCpY);
	CAOIWindow::RotateWindowStage(StageAngle, StageCpX, StageCpY);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWindow::RotateWindowCad(double Angle, double CpX, double CpY)//檢測框旋轉
{
	CAOIRgn::RotateRgnCad(Angle, CpX, CpY);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWindow::RotateWindowStage(double Angle, double CpX, double CpY)//檢測框旋轉
{
	CAOIRgn::RotateRgnStage(Angle, CpX, CpY);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWindow::MirrorXWindowCad(double CpX)//檢測框鏡射-X
{
	CAOIRgn::MirrorXRgnCad(CpX);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWindow::MirrorYWindowCad(double CpY)//檢測框鏡射-Y
{
	CAOIRgn::MirrorYRgnCad(CpY);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWindow::MirrorXWindowStage(double CpX)//檢測框鏡射-X
{
	CAOIRgn::MirrorXRgnStage(CpX);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWindow::MirrorYWindowStage(double CpY)//檢測框鏡射-Y
{
	CAOIRgn::MirrorYRgnStage(CpY);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWindow::ExtractRgnDerivedFrame(bool &Finished)//挖取檢測框圖片
{
	if ( CAOIRgn::ExtractRgnFrame(Finished) == false ) { return false; }

	if ( true == Finished )
	{
		bool IsOK = true;
		IsOK = CAOIWindow::ExecWindowInspection();
		ClearRgnMaskBuffer_Base();
		CAOIRgn::ClearRgnImageBuffer();
		if ( false == IsOK )
		{	return false; }	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWindow::CreateWindowSubRgnList(FIELD_BUILD_MODE BuildMode, bool ByCadRegion)//建立檢測框子檢測區域列表
{
	switch ( BuildMode )
	{	
	case FIELD_BUILD_RANDOM_PANEL:
	case FIELD_BUILD_RANDOM_BOARD:
	case FIELD_BUILD_RANDOM_PROJECT:
		if ( CAOIRgn::CreateRgnSubList_RandomField() == false )
		{	return false; }
		break;
	default:
		if ( CAOIRgn::CreateRgnSubList_MatrixField(ByCadRegion) == false )
		{	return false; }		
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIWindow::ClearWindowSubRgnList()//清除檢測框的子列表
{
	CAOIRgn::ClearRgnSubList();
}
//-------------------------------------------------------------------------------------//
size_t CAOIWindow::GetWindowSubRgnCount() const//取得檢測框的子數量
{
	return CAOIRgn::m_RgnSubList.size();
}
//-------------------------------------------------------------------------------------//
CAOIRgn* CAOIWindow::GetWindowSubRgnPtr(size_t index, bool check) const//取得檢測框的子指標	
{
	if ( true == check )
	{
		const size_t Count = m_RgnSubList.size();
		if ( index >= Count ) 
		{	return NULL; }
	}
	return m_RgnSubList[index];
}
//-------------------------------------------------------------------------------------//
bool CAOIWindow::ExecRgnDerivedInspection()//執行檢測框檢測
{
	return ExecWindowInspection();	
}
//-------------------------------------------------------------------------------------//
void CAOIWindow::InitWindowInspection()//初始化檢測框檢測
{
}
//-------------------------------------------------------------------------------------//
bool CAOIWindow::ExecWindowInspection()//執行檢測框檢測
{
	return true;
}
//-------------------------------------------------------------------------------------//