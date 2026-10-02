// AOIWndMask.cpp: implementation of the CAOIWndMask class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIWndMask.h"
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
IMPLEMENT_DYNAMIC(CAOIWndMask, CAOIObj)
//-------------------------------------------------------------------------------------//
CAOIWndMask::CAOIWndMask():CAOIObj(AOI_OBJ_WND_MASK)
{
	PreInitWndMask();
	InitialWndMask();
}
//-------------------------------------------------------------------------------------//
CAOIWndMask::CAOIWndMask(const CAOIWndMask &WndMask):CAOIObj(WndMask)
{
	PreInitWndMask();
	CloneWndMask(WndMask);
}
//-------------------------------------------------------------------------------------//
CAOIWndMask::~CAOIWndMask()
{

}
//-------------------------------------------------------------------------------------//
CAOIWndMask& CAOIWndMask::operator=(const CAOIWndMask &WndMask)
{
	if ( &WndMask == this ) { return *this; }
	CAOIObj::operator=(WndMask);
	CloneWndMask(WndMask);
	return *this;
}
//-------------------------------------------------------------------------------------//
CAOIWndMask* CAOIWndMask::CloneWndMaskObj() const//建立且複製一個框
{
	CAOIWndMask *ObjPtr = AOIObjManager.CreateWndMaskObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
inline void CAOIWndMask::PreInitWndMask()
{	
}
//-------------------------------------------------------------------------------------//
inline void CAOIWndMask::InitialWndMask()
{
	m_WndMaskBox = CAOIBox();	
	m_WndMaskIndex = -1;
	m_WndMaskBox.SetBoxMaskEraseMode(false);
}
//-------------------------------------------------------------------------------------//
inline void CAOIWndMask::CloneWndMask(const CAOIWndMask &WndMask)
{	
	m_WndMaskBox = WndMask.m_WndMaskBox;
	m_WndMaskIndex = WndMask.m_WndMaskIndex;
	return;
}
//-------------------------------------------------------------------------------------//
bool CAOIWndMask::WriteWndMaskFile(CAOIFileIO &FileIO)//儲存檢測框檔案
{

#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	
	int       index = 0;
	int       nValue = 0;
	double    dValue = 0.0;	
	CAOIWndMask  *WndMaskPtr = this;
	CAOIBox      *BoxPtr = NULL;	
	char         uuidStr[MAX_JET_PATH]="";	
	wchar_t      uuidWStr[MAX_JET_PATH]=L"";	
	UUID         uuid = WndMaskPtr->GetObjUuid();	
	FileIO.SetFnName(_T("CAOIWndMask::WriteWndMaskFile"));
	JetAPI::UUIDToStringA(uuid, uuidStr);
	JetAPI::UUIDToStringW(uuid, uuidWStr);
	//----------------------------------------------------------------------------------------//
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_MASK_START, 0) == false ) { return false; }		
	if ( FileIO.SaveChunk_BOL(FILE_IO_WND_MASK_ERASE_MODE, WndMaskPtr->GetWndMaskEraseMode()) == false ) { return false; }		
	
	BoxPtr = WndMaskPtr->GetWndMaskBoxPtr();
	if ( NULL != BoxPtr )
	{
		if ( FileIO.SaveChunk_INT(FILE_IO_WND_MASK_BOX_NODE, 0) == false ) { return false; }
		if ( BoxPtr->WriteBoxFile(FileIO) == false ) { return false; }
	}
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_MASK_END, 0) == false ) { return false; }
	return true;
	
}
//-------------------------------------------------------------------------------------//
bool CAOIWndMask::ReadWndMaskFile(CAOIFileIO &FileIO)//載入檢測框檔案
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG	
	
	int            index = 0;
	int            nValue = 0;
	double         dValue = 0.0;	
	CAOIWndMask   *WndMaskPtr = this;
	CAOIBox       *BoxPtr = NULL;	
	FileIO.SetFnName(_T("CAOIWndMask::ReadWndMaskFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }

		switch ( index )
		{
		case FILE_IO_WND_MASK_START://遮罩框參數-起點
			break;
		case FILE_IO_WND_MASK_END://遮罩框參數-終點
			//WndPtr->UpdateWndExtendBox();			
			return true;	
			break;
		case FILE_IO_WND_MASK_ERASE_MODE://遮罩框參數-清除模式
			WndMaskPtr->SetWndMaskEraseMode(FileIO.GetData_BOL());
			break;
		case FILE_IO_WND_MASK_BOX_NODE://遮罩框參數-基本框參數
			BoxPtr = WndMaskPtr->GetWndMaskBoxPtr();
			if ( NULL != BoxPtr )
			{
				if ( BoxPtr->ReadBoxFile(FileIO) == false )				
				{	return false; }
			}
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
CAOIBox* CAOIWndMask::GetWndMaskBoxPtr()//取得遮罩框的Box
{
	return &m_WndMaskBox;
}
//-------------------------------------------------------------------------------------//
unsigned int CAOIWndMask::GetWndMaskIndex() const//取得遮罩框引數
{
	return m_WndMaskIndex;
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SetWndMaskIndex(unsigned int val)//設定遮罩框引數
{
	m_WndMaskIndex = val;
}
//-------------------------------------------------------------------------------------//
BOX_TOWARD CAOIWndMask::GetWndMaskToward() const//取得遮罩框朝向
{
	return m_WndMaskBox.GetBoxToward();
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SetWndMaskToward(BOX_TOWARD val)//設定遮罩框朝向
{
	m_WndMaskBox.SetBoxToward(val);
}
//-------------------------------------------------------------------------------------//
BOX_SHAPE_MODE CAOIWndMask::GetWndMaskShapeMode() const//取得遮罩框外型
{
	return m_WndMaskBox.GetBoxShapeMode();
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SetWndMaskShapeMode(BOX_SHAPE_MODE val)//設定遮罩框外型
{
	m_WndMaskBox.SetBoxShapeMode(val);
}
//-------------------------------------------------------------------------------------//
double CAOIWndMask::GetWndMaskShapeParam() const//取得遮罩框外型參數
{
	return m_WndMaskBox.GetBoxShapeParam();
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SetWndMaskShapeParam(double val)//設定遮罩框外型參數
{
	m_WndMaskBox.SetBoxShapeParam(val);
}
//-------------------------------------------------------------------------------------//
double CAOIWndMask::GetWndMaskShapeParam2() const//取得遮罩框外型參數-2
{
	return m_WndMaskBox.GetBoxShapeParam2();
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SetWndMaskShapeParam2(double val)//設定遮罩框外型參數-2
{
	m_WndMaskBox.SetBoxShapeParam2(val);
}
//-------------------------------------------------------------------------------------//
bool CAOIWndMask::GetWndMaskEnabled() const//取得是否啟用
{
	return m_WndMaskBox.GetBoxEnabled();
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SetWndMaskEnabled(bool val)//取得是否啟用
{
	m_WndMaskBox.SetBoxEnabled(val);
}
//-------------------------------------------------------------------------------------//
bool CAOIWndMask::GetWndMaskActived() const//取得是否主要操作
{
	return m_WndMaskBox.GetBoxActived();
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SetWndMaskActived(bool val)//取得是否主要操作
{
	m_WndMaskBox.SetBoxActived(val);
}
//-------------------------------------------------------------------------------------//
bool CAOIWndMask::GetWndMaskVisibled() const//取得是否顯示
{
	return m_WndMaskBox.GetBoxVisibled();
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SetWndMaskVisibled(bool val)//設定是否顯示
{
	m_WndMaskBox.SetBoxVisibled(val);
}
//-------------------------------------------------------------------------------------//
bool CAOIWndMask::GetWndMaskSelected() const//取得是否選取到
{
	return m_WndMaskBox.GetBoxSelected();
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SetWndMaskSelected(bool val)//設定是否選取到 
{
	m_WndMaskBox.SetBoxSelected(val);
}
//-------------------------------------------------------------------------------------//
bool CAOIWndMask::GetWndMaskEditabled() const//取得是否可編輯
{
	return m_WndMaskBox.GetBoxEditabled();
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SetWndMaskEditabled(bool val)//設定是否可編輯
{
	m_WndMaskBox.SetBoxEditabled(val);
}
//-------------------------------------------------------------------------------------//
bool CAOIWndMask::GetWndMaskEraseMode() const//取得是否清除遮罩
{
	return m_WndMaskBox.GetBoxMaskEraseMode();
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SetWndMaskEraseMode(bool val)//設定是否清除遮罩
{
	m_WndMaskBox.SetBoxMaskEraseMode(val);	
}
//-------------------------------------------------------------------------------------//
double CAOIWndMask::GetWndMaskPosX() const//取得遮罩框座標-X
{
	return m_WndMaskBox.GetBoxPosX();
}
//-------------------------------------------------------------------------------------//
double CAOIWndMask::GetWndMaskPosY() const//取得遮罩框座標-Y
{
	return m_WndMaskBox.GetBoxPosY();
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskPos(TPOINT2D &Pos) const//取得遮罩框座標
{
	m_WndMaskBox.GetBoxPos(Pos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskPos(double &Px, double &Py) const//取得遮罩框座標
{
	m_WndMaskBox.GetBoxPos(Px, Py);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskPosRes(TPOINT2D &Pos) const//取得遮罩框座標結果
{
	m_WndMaskBox.GetBoxPosRes(Pos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskPosRes(double &Px, double &Py) const//取得遮罩框座標結果
{
	m_WndMaskBox.GetBoxPosRes(Px, Py);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskPosCad(TPOINT2D &Pos) const//取得遮罩框座標--Cad
{
	m_WndMaskBox.GetBoxPosCad(Pos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskPosCadRes(TPOINT2D &Pos) const//取得遮罩框座標-Cad結果
{
	m_WndMaskBox.GetBoxPosCadRes(Pos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskPosStage(TPOINT2D &Pos) const//取得遮罩框座標-機台結果
{
	m_WndMaskBox.GetBoxPosStage(Pos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskPosStageRes(TPOINT2D &Pos) const//取得遮罩框座標-機台結果
{
	m_WndMaskBox.GetBoxPosStageRes(Pos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskCornerPos(TPOINT2D CornerPos[]) const//取得遮罩框4端點
{
	m_WndMaskBox.GetBoxCornerPos(CornerPos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskCornerPosRes(TPOINT2D CornerPos[]) const//取得遮罩框4端點
{
	m_WndMaskBox.GetBoxCornerPosRes(CornerPos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskCornerPosCad(TPOINT2D CornerPos[]) const//取得遮罩框4端點
{
	m_WndMaskBox.GetBoxCornerPosCad(CornerPos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskCornerPosCadRes(TPOINT2D CornerPos[]) const//取得遮罩框4端點
{
	m_WndMaskBox.GetBoxCornerPosCadRes(CornerPos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskCornerPosStage(TPOINT2D CornerPos[]) const//取得遮罩框4端點
{
	m_WndMaskBox.GetBoxCornerPosStage(CornerPos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskCornerPosStageRes(TPOINT2D CornerPos[]) const//取得遮罩框4端點
{
	m_WndMaskBox.GetBoxCornerPosStageRes(CornerPos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::ResetWndMaskRegionRes()
{
	m_WndMaskBox.ResetBoxRegionRes();
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskRegion(TREGION4D &Region) const
{
	m_WndMaskBox.GetBoxRegion(Region);
}
//-------------------------------------------------------------------------------------//
void  CAOIWndMask::SetWndMaskRegion(const TREGION4D &Region)
{
	m_WndMaskBox.SetBoxRegion(Region);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskRegionRes(TREGION4D &Region) const
{
	m_WndMaskBox.GetBoxRegionRes(Region);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::MoveWndMask(double x, double y, bool bIncludeRes)//移動遮罩框
{
	m_WndMaskBox.MoveBox(x, y, bIncludeRes);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::MoveWndMask(const TPOINT2D &Pos, bool bIncludeRes)//移動遮罩框
{
	m_WndMaskBox.MoveBox(Pos, bIncludeRes);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SkewWndMaskAngle(double Skew)//旋轉遮罩框
{
	m_WndMaskBox.SkewBoxAngle(Skew);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::MoveWndMaskRes(double x, double y)//移動遮罩框結果
{
	m_WndMaskBox.MoveBoxRes(x, y);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::MoveWndMaskRes(const TPOINT2D &Pos)//移動遮罩框結果
{
	m_WndMaskBox.MoveBoxRes(Pos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::ScaleWndMask(double sx, double sy, bool bIncludeRes)
{
	m_WndMaskBox.ScaleBox(sx, sy, bIncludeRes);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SpinWndMask(double Angle, bool bIncludeRes)//自轉遮罩框
{
	double CpX=0, CpY=0;
	GetWndMaskPos(CpX, CpY);
	RotateWndMask(Angle, CpX, CpY, bIncludeRes);
	return;
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::RotateWndMask(double Angle, double CPX, double CPY, bool bIncludeRes)//旋轉遮罩框
{
	m_WndMaskBox.RotateBox(Angle, CPX, CPY, bIncludeRes);
}
//-------------------------------------------------------------------------------------//
bool CAOIWndMask::MirrorWndMaskXAxis(double CPY)//鏡射遮罩框X軸
{
	return m_WndMaskBox.MirrorBoxXAxis(CPY);
}
//-------------------------------------------------------------------------------------//
bool CAOIWndMask::MirrorWndMaskYAxis(double CPX)//鏡射遮罩框Y軸
{
	return m_WndMaskBox.MirrorBoxYAxis(CPX);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::DrawWndMaskEdit(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const
{
	m_WndMaskBox.DrawBoxEdit(hDC,DrawParam);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::DrawWndMaskResult(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const
{
	m_WndMaskBox.DrawBoxResult(hDC,DrawParam);
}
//-------------------------------------------------------------------------------------//
double CAOIWndMask::GetWndMaskAttachedAngle() const
{
	return m_WndMaskBox.GetBoxAttachedAngle();
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SetWndMaskAttachedAngle(double Angle)
{
	m_WndMaskBox.SetBoxAttachedAngle(Angle);
}
//-------------------------------------------------------------------------------------//	
TPOINT2D CAOIWndMask::GetWndMaskAttachedPosCad() const
{
	return m_WndMaskBox.GetBoxAttachedPosCad();
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskAttachedPosCad(TPOINT2D &Pos) const
{
	m_WndMaskBox.GetBoxAttachedPosCad(Pos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SetWndMaskAttachedPosCad(const TPOINT2D &Pos)//設定所屬零件的座標-Cad	
{
	m_WndMaskBox.SetBoxAttachedPosCad(Pos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskAttachedPosCad(double &PosX, double &PosY) const
{
	m_WndMaskBox.GetBoxAttachedPosCad(PosX, PosY);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SetWndMaskAttachedPosCad(double PosX, double PosY)//設定所屬零件的座標-Cad	
{
	m_WndMaskBox.SetBoxAttachedPosCad(PosX, PosY);
}
//-------------------------------------------------------------------------------------//
TPOINT2D CAOIWndMask::GetWndMaskAttachedPosStage() const
{
	return m_WndMaskBox.GetBoxAttachedPosStage();
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskAttachedPosStage(TPOINT2D &Pos) const
{
	m_WndMaskBox.GetBoxAttachedPosStage(Pos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SetWndMaskAttachedPosStage(const TPOINT2D &Pos)//設定所屬零件的座標-Stage			
{
	m_WndMaskBox.SetBoxAttachedPosStage(Pos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::SetWndMaskAttachedPosStage(double PosX, double PosY)//設定所屬零件的座標-Stage
{
	m_WndMaskBox.SetBoxAttachedPosStage(PosX, PosY);
}
//-------------------------------------------------------------------------------------//
void CAOIWndMask::GetWndMaskAttachedPosStage(double &PosX, double &PosY) const
{
	m_WndMaskBox.GetBoxAttachedPosStage(PosX, PosY);
}
//-------------------------------------------------------------------------------------//
bool CAOIWndMask::ApplyWndMask(const CAOIWndMask *RefWndMaskPtr, int TowardAngle)
{
	if ( NULL == RefWndMaskPtr ) { return false; }
	m_WndMaskBox.ApplyBox(&(RefWndMaskPtr->m_WndMaskBox));
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWndMask::SynchronousWndMask(const CAOIWndMask *RefWndMaskPtr)
{
	if ( NULL == RefWndMaskPtr ) { return false; }
	m_WndMaskBox.SynchronousBox(&(RefWndMaskPtr->m_WndMaskBox));	
	return true;
}
//-------------------------------------------------------------------------------------//