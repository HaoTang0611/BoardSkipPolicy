// AOIWndRoi.cpp: implementation of the CAOIWndRoi class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "AOIWndRoi.h"
//-------------------------------------------------------------------------------------//
#include "AOIFileIO.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNAMIC(CAOIWndRoi, CAOIObj)
//-------------------------------------------------------------------------------------//
CAOIWndRoi::CAOIWndRoi():CAOIObj(AOI_OBJ_WND_ROI)
{
	PreInitWndRoi();
	InitialWndRoi();
}
//-------------------------------------------------------------------------------------//
CAOIWndRoi::CAOIWndRoi(const CAOIWndRoi &WndRoi):CAOIObj(WndRoi)
{
	PreInitWndRoi();
	CloneWndRoi(WndRoi);
}
//-------------------------------------------------------------------------------------//
CAOIWndRoi::~CAOIWndRoi()
{

}
//-------------------------------------------------------------------------------------//
CAOIWndRoi& CAOIWndRoi::operator=(const CAOIWndRoi &WndRoi)
{
	if ( &WndRoi == this ) { return *this; }
	CAOIObj::operator=(WndRoi);
	CloneWndRoi(WndRoi);
	return *this;
}
//-------------------------------------------------------------------------------------//
CAOIWndRoi* CAOIWndRoi::CloneWndRoiObj() const//ミ狡籹
{
	CAOIWndRoi *ObjPtr = AOIObjManager.CreateWndRoiObj();
	if ( NULL == ObjPtr ) { return NULL; }
	ObjPtr->operator=(*this);
	return ObjPtr;
}
//-------------------------------------------------------------------------------------//
inline void CAOIWndRoi::PreInitWndRoi()
{	
}
//-------------------------------------------------------------------------------------//
inline void CAOIWndRoi::InitialWndRoi()
{
	m_WndRoiAlgType = ALG_EMPTY;
	m_WndRoiBox = CAOIBox();
	m_WndRoiIndex = -1;
	m_WndRoiWndPtr = NULL;
	m_WndRoiImageRect.left = 0;
	m_WndRoiImageRect.top = 0;
	m_WndRoiImageRect.right = 0;
	m_WndRoiImageRect.bottom = 0;			
	m_WndRoiBinaryParam = CAlgBinaryParam();
	m_WndRoiBinaryParam.SetBinaryBelongToWho(BIN_PARAM_BELONG_TO_ROI_IMAGE);
	m_WndRoiSelfFrameEnabled = false;
	m_WndRoiBinaryParamEnabled = false;		
}
//-------------------------------------------------------------------------------------//
inline void CAOIWndRoi::CloneWndRoi(const CAOIWndRoi &WndRoi)
{	
	m_WndRoiAlgType = WndRoi.m_WndRoiAlgType;
	m_WndRoiBox = WndRoi.m_WndRoiBox;
	m_WndRoiIndex = WndRoi.m_WndRoiIndex;
	m_WndRoiWndPtr = WndRoi.m_WndRoiWndPtr;
	m_WndRoiImageRect = WndRoi.m_WndRoiImageRect;	
	m_WndRoiBinaryParam = WndRoi.m_WndRoiBinaryParam;
	m_WndRoiSelfFrameEnabled = WndRoi.m_WndRoiSelfFrameEnabled;
	m_WndRoiBinaryParamEnabled = WndRoi.m_WndRoiBinaryParamEnabled;	
}
//-------------------------------------------------------------------------------------//
bool CAOIWndRoi::WriteWndRoiFile(CAOIFileIO &FileIO)//纗浪代郎
{

#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG
	
	int       index = 0;
	int       nValue = 0;
	double    dValue = 0.0;
	CAlgParam    AlgParam;
	CAOIWndRoi  *WndRoiPtr = this;
	CAOIBox     *BoxPtr = NULL;
	CAlgBinaryParam *BinaryParamPtr = NULL;
	char         uuidStr[MAX_JET_PATH]="";	
	wchar_t      uuidWStr[MAX_JET_PATH]=L"";	
	UUID         uuid = WndRoiPtr->GetObjUuid();	
	FileIO.SetFnName(_T("CAOIWndRoi::WriteWndRoiFile"));
	JetAPI::UUIDToStringA(uuid, uuidStr);
	JetAPI::UUIDToStringW(uuid, uuidWStr);
	//----------------------------------------------------------------------------------------//
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_ROI_START, 0) == false ) { return false;; }	
	//FILE_IO_WND_ROI_ENABLED
	BoxPtr = WndRoiPtr->GetWndRoiBoxPtr();
	if ( NULL != BoxPtr )
	{		
		if ( FileIO.SaveChunk_INT(FILE_IO_WND_ROI_BOX_NODE, 0) == false ) { return false; }
		if ( BoxPtr->WriteBoxFile(FileIO) == false ) { return false; }		
	}
	
	BinaryParamPtr = WndRoiPtr->GetWndRoiBinaryParamPtr();
	if ( NULL != BinaryParamPtr )
	{
		if ( FileIO.SaveChunk_INT(FILE_IO_WND_ROI_BINARY_PARAM, 0) == false ) { return false; }
		if ( BinaryParamPtr->WriteAlgBinaryParamFile(FileIO) == false ) { return false; }		
	}	
	//v1.01.04.061
	if (FileIO.SaveChunk_BOL(FILE_IO_WND_ROI_SELF_FRAME_ENABLED, GetWndRoiSelfFrameEnabled()) == false) { return false; }
	
	if ( FileIO.SaveChunk_INT(FILE_IO_WND_ROI_END, 0) == false ) { return false; }
	return true;
	
}
//-------------------------------------------------------------------------------------//
bool CAOIWndRoi::ReadWndRoiFile(CAOIFileIO &FileIO)//更浪代郎
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == false ) { return false; }
	if ( FileIO.CheckFileOpened() == false ) { return false; }
#endif//_DEBUG	
	
	int            index = 0;
	int            nValue = 0;
	double         dValue = 0.0;
	CAlgParam      AlgParam;
	CAOIWndRoi    *WndRoiPtr = this;
	CAOIBox       *BoxPtr = NULL;
	CAlgBinaryParam *BinaryParamPtr = NULL;
	FileIO.SetFnName(_T("CAOIWndRoi::ReadWndRoiFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 		
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }

		switch ( index )
		{
		case FILE_IO_WND_ROI_START://把计-癬翴
			break;
		case FILE_IO_WND_ROI_END://把计-沧翴
			//WndPtr->UpdateWndExtendBox();			
			return true;	
		case FILE_IO_WND_ROI_ENABLED://把计-币笆
			break;
		case FILE_IO_WND_ROI_BOX_NODE://把计-膀セ把计
			BoxPtr = WndRoiPtr->GetWndRoiBoxPtr();
			if ( NULL != BoxPtr )
			{
				if ( BoxPtr->ReadBoxFile(FileIO) == false )				
				{	return false; }
			}
			break;
		case FILE_IO_WND_ROI_BINARY_PARAM://把计-2て把计
			BinaryParamPtr = WndRoiPtr->GetWndRoiBinaryParamPtr();
			if ( NULL != BinaryParamPtr )
			{
				if ( BinaryParamPtr->ReadAlgBinaryParamFile(FileIO) == false )				
				{	return false; }
			}
			break;
		case FILE_IO_WND_ROI_SELF_FRAME_ENABLED://v1.01.04.061
			SetWndRoiSelfFrameEnabled(FileIO.GetData_BOL());
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
inline bool CAOIWndRoi::CheckWndRoiBinaryParamEnabled(ALG_TYPE AlgType)//絋粄簍衡猭琌や穿2て
{
	bool Enabled = false;
	switch ( AlgType )
	{
	case ALG_COLOR_CODE:
	case ALG_ANGLE_MEASURE:
	case ALG_MEASURE_SIP_DISTANCE:
	case ALG_MEASURE_CONNECTOR:
		Enabled = true;
		break;
	default:
		Enabled = false;
		break;
	}
	return Enabled;
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::SetWndRoiAlgType(ALG_TYPE AlgType)
{	
	m_WndRoiAlgType = AlgType;
	m_WndRoiBinaryParamEnabled = CheckWndRoiBinaryParamEnabled(AlgType);
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::SetWndRoiIndex(unsigned int value)
{
	m_WndRoiIndex = value;
	m_WndRoiBinaryParam.SetBinaryBelongIndex(value);
}
//-------------------------------------------------------------------------------------//
bool CAOIWndRoi::VisibleWndRoi()//币ノ陪ボ篈
{
	const bool Visibled = true;
	m_WndRoiBox.SetBoxVisibled(Visibled);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWndRoi::UnSelectWndRoi()//匡篈
{
	const bool Selected = false;	
	m_WndRoiBox.SetBoxSelected(Selected);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWndRoi::InvisibleWndRoi()//陪ボ篈
{
	const bool Visibled = false;
	m_WndRoiBox.SetBoxVisibled(Visibled);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::SetWndRoiRegion(const TREGION4D &Region, bool IncludeRes)
{
	m_WndRoiBox.SetBoxRegion(Region, IncludeRes);
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::SetWndRoiRegion(double MinX, double MinY, double MaxX, double MaxY, bool IncludeRes)
{
	m_WndRoiBox.SetBoxRegion(MinX, MinY, MaxX, MaxY, IncludeRes);
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::SetWndRoiRegionRes(const TREGION4D &Region)
{
	m_WndRoiBox.SetBoxRegionRes(Region);
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::SetWndRoiRegionRes(double MinX, double MinY, double MaxX, double MaxY)
{
	m_WndRoiBox.SetBoxRegionRes(MinX, MinY, MaxX, MaxY);
}
//-------------------------------------------------------------------------------------//
CAlgBinaryParam& CAOIWndRoi::GetWndRoiBinaryParam()
{
	return m_WndRoiBinaryParam;
}
//-------------------------------------------------------------------------------------//
CAlgBinaryParam* CAOIWndRoi::GetWndRoiBinaryParamPtr()
{
	return &m_WndRoiBinaryParam;
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::SetWndRoiBinaryParam(const CAlgBinaryParam &BinParam)
{
	m_WndRoiBinaryParam = BinParam;
}
//-------------------------------------------------------------------------------------//
bool CAOIWndRoi::ApplyWndRoi(const CAOIWndRoi *RefWndRoiPtr, int TowardAngle)
{
	m_WndRoiBox.ApplyBox(&(RefWndRoiPtr->m_WndRoiBox));
	m_WndRoiBinaryParam = RefWndRoiPtr->m_WndRoiBinaryParam;
	m_WndRoiBinaryParam.RotateBinaryParam(TowardAngle);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWndRoi::SynchronousWndRoi(const CAOIWndRoi *RefWndRoiPtr)
{
	m_WndRoiBox.SynchronousBox(&(RefWndRoiPtr->m_WndRoiBox));
	m_WndRoiBinaryParam = RefWndRoiPtr->m_WndRoiBinaryParam;
	return true;
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::ScaleWndRoi(double sx, double sy, bool bIncludeRes)//罽浪代
{
	m_WndRoiBox.ScaleBox(sx, sy, bIncludeRes);	
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::MoveWndRoi(double x, double y, bool bIncludeRes)
{
	m_WndRoiBox.MoveBox(x, y, bIncludeRes);	
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::MoveWndRoi(const TPOINT2D &Pos, bool bIncludeRes)
{
	m_WndRoiBox.MoveBox(Pos, bIncludeRes);
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::MoveWndRoiResult(double x, double y)
{
	m_WndRoiBox.MoveBoxRes(x, y);		
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::MoveWndRoiResult(const TPOINT2D &Pos)
{
	m_WndRoiBox.MoveBoxRes(Pos);
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::SpinWndRoi(double Angle)
{
	double PosX=0, PosY=0;
	m_WndRoiBox.GetBoxPos(PosX, PosY);
	m_WndRoiBox.RotateBox(Angle, PosX, PosY);
	const int AngleLabel = JetAPI::GetAngleLabel(Angle);
	m_WndRoiBinaryParam.RotateBinaryParam(AngleLabel);
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::RotateWndRoi(double Angle, double CPX, double CPY)
{
	m_WndRoiBox.RotateBox(Angle, CPX, CPY);
	const int AngleLabel = JetAPI::GetAngleLabel(Angle);
	m_WndRoiBinaryParam.RotateBinaryParam(AngleLabel);
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::MirrorWndRoiXAxis(double CPY)
{
	m_WndRoiBox.MirrorBoxXAxis(CPY);	
	m_WndRoiBinaryParam.MirrorBinaryParamXAxis();
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::MirrorWndRoiYAxis(double CPX)
{
	m_WndRoiBox.MirrorBoxYAxis(CPX);	
	m_WndRoiBinaryParam.MirrorBinaryParamYAxis();
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::DrawWndRoiBoxEdit(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const
{
	m_WndRoiBox.DrawBoxEdit(hDC, DrawParam);	
}
//-------------------------------------------------------------------------------------//
void CAOIWndRoi::DrawWndRoiBoxResult(HDC hDC, const TBOX_DRAW_PARAM &DrawParam) const
{
	m_WndRoiBox.DrawBoxResult(hDC, DrawParam);
}
//-------------------------------------------------------------------------------------//
bool CAOIWndRoi::InitWndRoiInspection(bool bModelInit)//﹍て浪代浪代
{
	m_WndRoiBox.ResetBoxRegionRes();	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWndRoi::SetWndRoiBoxAlgDrawParam(HDC hDC, TBOX_DRAW_PARAM & BoxDrawParam)
{
	CAOIWnd *WndPtr = m_WndRoiWndPtr;
	ALG_TYPE AlgType = WndPtr->GetWndAlgType();
	const int WndRoiIndex = GetWndRoiIndex();
	int TempInt;	char TempChar;
	switch (AlgType)
	{
	case ALG_MEASURE_SIP_DISTANCE:
		TempInt = WndPtr->GetWndAlgParamPtr()->GetAlgParamMeasureSIP().msInspecEdgeCount;
		BoxDrawParam.DrawBoxIndex = false;
		BoxDrawParam.strBoxIndex = _T("");
		BoxDrawParam.Extend.x = BoxDrawParam.Extend.y = 0;
		if (WndRoiIndex == 0) { ::SelectObject(hDC, CAOIModel::hSubBoxPen4); }
		else if (WndRoiIndex < TempInt+1) {
			TempChar = 'A' + (WndRoiIndex - 1);
			BoxDrawParam.DrawBoxIndex = true;
			BoxDrawParam.strBoxIndex.Format(_T("%c"), TempChar);
			::SelectObject(hDC, CAOIModel::hSubBoxPen1);
		}
		else { ::SelectObject(hDC, CAOIModel::hSubBoxPen1); }
		break;
	default:
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIWndRoi::CheckWndRoiBoxAlgUseExtend()
{
	CAOIWnd *WndPtr = m_WndRoiWndPtr;
	ALG_TYPE AlgType = WndPtr->GetWndAlgType();
	const int WndRoiIndex = GetWndRoiIndex();
	switch (AlgType)
	{
	case ALG_MEASURE_SIP_DISTANCE:
		if (WndRoiIndex == 0) { return true; }
		break;
	default:
		break;
	}
	return false;
}
//-------------------------------------------------------------------------------------//