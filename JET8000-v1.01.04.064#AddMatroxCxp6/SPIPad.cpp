// SPIPad.cpp: implementation of the CSPIPad class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SPIPad.h"
//-------------------------------------------------------------------------------------//
#include "AOIFilePidDef.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
class CSPIPointInfo
{
public:
	CSPIPointInfo()	{}
	~CSPIPointInfo() {}
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CSPIPad::CSPIPad()
{
	PreInitSpiPad();
	InitialSpiPad();
}
//-------------------------------------------------------------------------------------//
CSPIPad::~CSPIPad()
{

}
//-------------------------------------------------------------------------------------//
CSPIPad::CSPIPad(const CSPIPad &other)
{
	PreInitSpiPad();
	CloneSpiPad(other);
}
//-------------------------------------------------------------------------------------//
CSPIPad& CSPIPad::operator=(const CSPIPad &other)
{
	if ( this == &other ) { return *this; }
	CloneSpiPad(other);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::PreInitSpiPad()
{
}
//-------------------------------------------------------------------------------------//
void CSPIPad::InitialSpiPad()
{
	//---------------------------------------------------------------------------------//
	m_PidFileUnitScale = 1000.0;
	//---------------------------------------------------------------------------------//
	m_ComponentName=L"";//零件名稱
	m_PinIDText=L"";//Pin編碼
	m_BoardID=-1;//單板編號
	m_PackageName=L"";//封裝名稱
	m_PadPos1=TPOINT2D();//Pad座標-1
	m_PadPos2=TPOINT2D();//Pad座標-2
	m_ComponentAngle=0;//零件角度
	m_SpecialCadPos = TPOINT2D();//HASI Cad座標
	//---------------------------------------------------------------------------------//
	m_PadArea=0;//Pad面積
	m_PadType=L"";//Pad樣式
	m_PadSN=0;//Pad序號
	m_PadGroupID=0;
	m_PadDrawType=0;//Pad繪圖模式
	m_PadToward=BOX_TOWARD_NULL;
	m_PadIncludedAngle=0.0;	
	//---------------------------------------------------------------------------------//
	m_TempInt=0;
	m_TempDlb=0;
	//---------------------------------------------------------------------------------//
	m_SubSpiPadList.clear();
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
void CSPIPad::CloneSpiPad(const CSPIPad &other)
{
	//---------------------------------------------------------------------------------//
	m_PidFileUnitScale=other.m_PidFileUnitScale;
	//---------------------------------------------------------------------------------//
	m_ComponentName=other.m_ComponentName;
	m_PinIDText=other.m_PinIDText;
	m_BoardID=other.m_BoardID;
	m_PackageName=other.m_PackageName;
	m_PadPos1=other.m_PadPos1;
	m_PadPos2=other.m_PadPos2;
	m_ComponentAngle=other.m_ComponentAngle;
	m_SpecialCadPos = other.m_SpecialCadPos;
	//---------------------------------------------------------------------------------//
	m_PadArea=other.m_PadArea;
	m_PadType=other.m_PadType;
	m_PadSN=other.m_PadSN;
	m_PadGroupID=other.m_PadGroupID;
	m_PadDrawType=other.m_PadDrawType;
	m_PadToward=other.m_PadToward;
	m_PadIncludedAngle=other.m_PadIncludedAngle;	
	//---------------------------------------------------------------------------------//
	m_TempInt=other.m_TempInt;	
	m_TempDlb=other.m_TempDlb;
	//---------------------------------------------------------------------------------//
	m_SubSpiPadList=other.m_SubSpiPadList;	
	//m_ComponentName=other.m_ComponentName;		
}
//-------------------------------------------------------------------------------------//
void CSPIPad::ResetSpiPad()
{
	InitialSpiPad();
	return;
}
//-------------------------------------------------------------------------------------//
double CSPIPad::GetPidFileUnitScale() const
{
	return m_PidFileUnitScale;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPidFileUnitScale(double val)
{	
	m_PidFileUnitScale = val;
}
//-------------------------------------------------------------------------------------//
bool CSPIPad::ReadSpiPadFile(CAOIFileIO &FileIO)
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == FALSE ) { return false; }
	if ( FileIO.CheckFileOpened() == FALSE ) { return false; }
#endif//_DEBUG	
	
	std::string    str;
	std::wstring   wstr;
	int            index=0;
	int            nValue=0;		
	UINT ConvertCode = CP_UTF8;
	CSPIPointInfo  PointInfo;
	const double   UnitScale=GetPidFileUnitScale();
	CString        ComponentName = GetComponentName();
	//----------------------------------------------------------------------------------------//		
	FileIO.SetFnName(_T("CSPIPad::ReadSpiPadFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 	
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }		
		
		switch ( index )
		{			
		case PID_ID_COMPONENT_NAME://零件名稱:str
			if ( FileIO.GetLoadWStr()==true )
			{	SetComponentName(FileIO.GetData_WSTR());	}
			else
			{
				str=FileIO.GetData_STR();
				JetAPI::char2wstring(str.c_str(), wstr, ConvertCode);
				SetComponentName(wstr.c_str());
			}
			break;	
		case PID_ID_PIN_ID://PIN編號:str
			if ( FileIO.GetLoadWStr()==true )
			{	SetPinIDText(FileIO.GetData_WSTR());	}
			else
			{
				str=FileIO.GetData_STR();
				JetAPI::char2wstring(str.c_str(), wstr, ConvertCode);
				SetPinIDText(wstr.c_str());
			}
			break;
		case PID_ID_BOARD_ID://單板編號:int
			SetBoardID(FileIO.GetData_INT());
			break;
		case PID_ID_PACKAGE_NAME://封裝名稱:str
			if ( FileIO.GetLoadWStr()==true )
			{	SetPackageName(FileIO.GetData_WSTR());	}
			else
			{
				str=FileIO.GetData_STR();
				JetAPI::char2wstring(str.c_str(), wstr, ConvertCode);
				SetPackageName(wstr.c_str());
			}
			break;
		case PID_ID_PAD_POS_X_1://Pad座標-X1:double
			SetPadPosX1(FileIO.GetData_DBL()*UnitScale);
			break;
		case PID_ID_PAD_POS_Y_1://Pad座標-Y1:double
			SetPadPosY1(FileIO.GetData_DBL()*UnitScale);
			break;
		case PID_ID_PAD_POS_X_2://Pad座標-X2:double
			SetPadPosX2(FileIO.GetData_DBL()*UnitScale);
			break;
		case PID_ID_PAD_POS_Y_2://Pad座標-Y2:double
			SetPadPosY2(FileIO.GetData_DBL()*UnitScale);
			break;
		case PID_ID_COMPONENT_ANGLE://零件角度:double			
			SetComponentAngle(FileIO.GetData_DBL());
			break;
		case PID_ID_PAD_AREA://Pad面積:double
			SetPadArea(FileIO.GetData_DBL()*UnitScale*UnitScale);
			break;
		case PID_ID_PAD_UNKNOW_1://未知:double
			break;
		case PID_ID_PAD_UNKNOW_2://未知:double
			break;
		case PID_ID_HASI_CAD_POS_X:
			SetSpecialCadPosX(FileIO.GetData_DBL()*UnitScale);
			break;
		case PID_ID_HASI_CAD_POS_Y:
			SetSpecialCadPosY(FileIO.GetData_DBL()*UnitScale);
			break;
		case PID_ID_HASI_FID_POS_X:
			if (ComponentName.Find(L"JET_Fid") == -1) { break; }
			SetSpecialCadPosX(FileIO.GetData_DBL()*UnitScale);
			break;
		case PID_ID_HASI_FID_POS_Y:
			if (ComponentName.Find(L"JET_Fid") == -1) { break; }
			SetSpecialCadPosY(FileIO.GetData_DBL()*UnitScale);
			break;
		case PID_ID_PAD_TYPE://Pad樣式:str
			if ( FileIO.GetLoadWStr()==true )
			{	SetPadType(FileIO.GetData_WSTR());	}
			else
			{
				str=FileIO.GetData_STR();
				JetAPI::char2wstring(str.c_str(), wstr, ConvertCode);
				SetPadType(wstr.c_str());
			}
			break;
		case PID_ID_PAD_SN://Pad序號:int
			SetPadSN(FileIO.GetData_INT());
			break;

		case PID_ID_PAD_DRAW_TYPE://Pad繪圖樣式:int
			SetPadDrawType(FileIO.GetData_INT());
			break;
		case PID_ID_POLYLINE_START://多邊形開始
			break;
		case PID_ID_POLYLINE_END://多邊形結束
			break;
		case PID_ID_POINT_INFO_START://點資訊開始
			if ( ReadSpiPointInfoFile(FileIO, PointInfo) == false )
			{	return false; }
			break;
	
		case PID_ID_PAD_END://Pad結束						
			return true;
			break;
		default:
			nValue=0;
			break;
		}
	};
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSPIPad::ReadSpiPointInfoFile(CAOIFileIO &FileIO, CSPIPointInfo &PointInfo)
{
#ifdef _DEBUG
	if ( FileIO.CheckFileMode() == FALSE ) { return false; }
	if ( FileIO.CheckFileOpened() == FALSE ) { return false; }
#endif//_DEBUG
	
	int            index=0;
	int            nValue=0;	
	const double   UnitScale=GetPidFileUnitScale();
	//----------------------------------------------------------------------------------------//		
	FileIO.SetFnName(_T("CSPIPad::ReadSpiPointInfoFile"));
	//----------------------------------------------------------------------------------------//
	while ( FileIO.CheckFileEnd()==false )
	{ 	
		if ( FileIO.LoadChunk(index) == false )		
		{	continue; }		
		
		switch ( index )
		{			
	
		case PID_ID_POINT_INFO_END://點資訊結束
			return true;
			break;
		case PID_ID_POINT_POS_X://點資訊-座標X:double
			break;
		case PID_ID_POINT_POS_Y://點資訊-座標Y:double
			break;
		case PID_ID_POINT_ARC_X://點資訊-圓弧X:double
			break;
		case PID_ID_POINT_ARC_Y://點資訊-圓弧Y:double
			break;
		case PID_ID_POINT_PATH_TYPE://點資訊-路徑樣式:int
			break;
		default:
			nValue=0;
			break;
		}
	};
	return true;
}
//-------------------------------------------------------------------------------------//
const wchar_t* CSPIPad::GetComponentName() const
{
	return m_ComponentName.c_str();
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetComponentName(const wchar_t *val)
{
	m_ComponentName = val;
}
//-------------------------------------------------------------------------------------//
const wchar_t* CSPIPad::GetPinIDText() const
{
	return m_PinIDText.c_str();
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPinIDText(const wchar_t *val)
{
	m_PinIDText=val;
}
//-------------------------------------------------------------------------------------//
int CSPIPad::GetBoardID() const
{
	return m_BoardID;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetBoardID(int val)
{
	m_BoardID = val;
}
//-------------------------------------------------------------------------------------//
const wchar_t* CSPIPad::GetPackageName() const
{
	return m_PackageName.c_str();
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPackageName(const wchar_t *val)
{
	m_PackageName=val;
}
//-------------------------------------------------------------------------------------//
const TPOINT2D& CSPIPad::GetPadPos1() const
{
	return m_PadPos1;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPadPosX1(double val)
{
	m_PadPos1.x = val;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPadPosY1(double val)
{
	m_PadPos1.y = val;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPadPos1(double x, double y)
{
	m_PadPos1.x = x;
	m_PadPos1.y = y;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPadPos1(const TPOINT2D &val)
{
	m_PadPos1 = val;
}
//-------------------------------------------------------------------------------------//
const TPOINT2D& CSPIPad::GetPadPos2() const
{
	return m_PadPos2;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPadPosX2(double val)
{
	m_PadPos2.x = val;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPadPosY2(double val)
{
	m_PadPos2.y = val;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPadPos2(double x, double y)
{
	m_PadPos2.x = x;
	m_PadPos2.y = y;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPadPos2(const TPOINT2D &val)
{
	m_PadPos2 = val;
}
//-------------------------------------------------------------------------------------//
const TPOINT2D & CSPIPad::GetSpecialCadPos() const
{
	return m_SpecialCadPos;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetSpecialCadPosX(double val)
{
	m_SpecialCadPos.x = val;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetSpecialCadPosY(double val)
{
	m_SpecialCadPos.y = val;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::CheckPadPos(TPOINT2D &pos) const
{
	pos.x = (m_PadPos1.x+m_PadPos2.x)/2.0;
	pos.y = (m_PadPos1.y+m_PadPos2.y)/2.0;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPadRegion(const TREGION4D &Region)
{
	SetPadPos1(Region.minX, Region.minY);
	SetPadPos2(Region.maxX, Region.maxY);	
	return ;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::CheckPadRegion(TREGION4D &Region) const
{
	Region.minX=MIN(m_PadPos1.x, m_PadPos2.x);
	Region.minY=MIN(m_PadPos1.y, m_PadPos2.y);
	Region.maxX=MAX(m_PadPos1.x, m_PadPos2.x);
	Region.maxY=MAX(m_PadPos1.y, m_PadPos2.y);
	return;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::GetPadCornerPos(TPOINT2D pos[]) const
{
	TREGION4D Region;
	CheckPadRegion(Region);
	pos[0].x = Region.minX;	pos[0].y = Region.maxY;
	pos[1].x = Region.maxX;	pos[1].y = Region.maxY;
	pos[2].x = Region.maxX;	pos[2].y = Region.minY;
	pos[3].x = Region.minX;	pos[3].y = Region.minY;
	return;
}
//-------------------------------------------------------------------------------------//
double CSPIPad::GetComponentAngle() const
{
	return m_ComponentAngle;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetComponentAngle(double val)
{
	m_ComponentAngle = val;
}
//-------------------------------------------------------------------------------------//
double CSPIPad::GetPadArea() const
{
	return m_PadArea;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPadArea(double val)
{
	m_PadArea = val;
}
//-------------------------------------------------------------------------------------//
const wchar_t* CSPIPad::GetPadType() const
{
	return m_PadType.c_str();
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPadType(const wchar_t *val)
{
	m_PadType = val;
}
//-------------------------------------------------------------------------------------//
int CSPIPad::GetPadSN() const
{
	return m_PadSN;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPadSN(int val)
{
	m_PadSN = val;
}
//-------------------------------------------------------------------------------------//
int CSPIPad::GetPadGroupID() const
{
	return m_PadGroupID;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPadGroupID(int val)
{
	m_PadGroupID = val;
}
//-------------------------------------------------------------------------------------//
int CSPIPad::GetPadDrawType() const
{
	return m_PadDrawType;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPadDrawType(int val)
{
	m_PadDrawType = val;
}
//-------------------------------------------------------------------------------------//
BOX_TOWARD CSPIPad::GetPadToward() const
{
	return m_PadToward;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPadToward(BOX_TOWARD val)
{
	m_PadToward = val;
}
//-------------------------------------------------------------------------------------//
double CSPIPad::GetPadIncludedAngle() const
{
	return m_PadIncludedAngle;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetPadIncludedAngle(double val)
{
	m_PadIncludedAngle = val;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::MovePadPos(double x, double y)
{
	m_PadPos1.x += x;
	m_PadPos1.y += y;
	m_PadPos2.x += x;
	m_PadPos2.y += y;
	return;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::RotatePadAngle(double Angle, const TPOINT2D &cp)
{
	TREGION4D Region;
	TPOINT2D CornerPos[4];	
	BOX_TOWARD Toward=GetPadToward();
	double ComAngle=GetComponentAngle();
	double IncAngle=GetPadIncludedAngle();

	GetPadCornerPos(CornerPos);
	Toward=JetAPI::RotateToward(Angle, Toward);	
	ComAngle=JetAPI::RotateAngle(ComAngle, Angle);
	IncAngle=JetAPI::RotateAngle(IncAngle, Angle);
	JetAPI::RotateCornerPos(Angle, cp.x, cp.y, CornerPos);
	JetAPI::CornerPtToRegion(CornerPos, Region);	
	
	SetPadToward(Toward);
	SetPadRegion(Region);
	SetComponentAngle(ComAngle);
	SetPadIncludedAngle(IncAngle);
	return ;
}
//-------------------------------------------------------------------------------------//
int CSPIPad::GetTempInt() const
{
	return m_TempInt;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetTempInt(int val)
{
	m_TempInt = val;
}
//-------------------------------------------------------------------------------------//
double CSPIPad::GetTempDlb() const
{
	return m_TempDlb;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::SetTempDlb(double val)
{
	m_TempDlb = val;
}
//-------------------------------------------------------------------------------------//
void CSPIPad::ClearSubSpiPadList()
{
	m_SubSpiPadList.clear();
}
//-------------------------------------------------------------------------------------//
void CSPIPad::AddSubSpiPad(const CSPIPad &SpiPad)
{
	m_SubSpiPadList.push_back(SpiPad);
}
//-------------------------------------------------------------------------------------//
size_t CSPIPad::GetSubSpiPadCount() const
{
	return m_SubSpiPadList.size();
}
//-------------------------------------------------------------------------------------//
CSPIPad* CSPIPad::GetSubSpiPadPtr(size_t idx, bool bCheck)
{
	if ( true == bCheck )
	{
		const size_t Cnt=m_SubSpiPadList.size();
		if ( idx >= Cnt )
		{	return NULL; }
	}
	return &(m_SubSpiPadList[idx]);
}
//-------------------------------------------------------------------------------------//
bool CSPIPad::CheckSubSpiPadAccept(const CSPIPad &SpiPad) const
{	
	if ( GetPadToward() != SpiPad.GetPadToward() ) { return false; }

	TREGION4D Region;
	TREGION4D SubRegion;
	CheckPadRegion(Region);
	SpiPad.CheckPadRegion(SubRegion);	
	if ( SubRegion.minX > Region.maxX ) { return false; }
	if ( SubRegion.minY > Region.maxY ) { return false; }
	if ( SubRegion.maxX < Region.minX ) { return false; }
	if ( SubRegion.maxY < Region.minY ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSPIPad::CalcAverageSpiPad(CSPIPad &SpiPad) const
{
	size_t i=0;
	bool   bFirst=true;
	TREGION4D  SubRegion;	
	TPOINT2D Pos, SumPos;
	TSIZE2D  Size, SumSize;
	const std::vector<CSPIPad> &SubSpiPadList=m_SubSpiPadList;	
	const size_t SubSpiPadCount=SubSpiPadList.size();

	bFirst = true;
	for ( i=0; i<SubSpiPadCount; i++ )
	{
		const CSPIPad &SpiPadRef=SubSpiPadList[i];		
		SpiPadRef.CheckPadRegion(SubRegion);
		Pos.x = SubRegion.GetCpX();
		Pos.y = SubRegion.GetCpY();
		Size.cx = SubRegion.GetSizeX();
		Size.cy = SubRegion.GetSizeY();
		if ( true == bFirst )
		{
			SumPos = Pos;
			SumSize = Size;
			bFirst = false;
		}
		else
		{
			SumPos.x += Pos.x;
			SumPos.y += Pos.y;
			SumSize.cx += Size.cx;
			SumSize.cy += Size.cy;
		}
	}
	if ( 0 == SubSpiPadCount )
	{
		SpiPad=(*this);
		return true;
	}

	CheckPadRegion(SubRegion);
	Pos.x = SubRegion.GetCpX();
	Pos.y = SubRegion.GetCpY();
	Size.cx = SubRegion.GetSizeX();
	Size.cy = SubRegion.GetSizeY();
	SumPos.x += Pos.x;
	SumPos.y += Pos.y;
	SumSize.cx += Size.cx;
	SumSize.cy += Size.cy;
	const int Count=(int)(SubSpiPadCount+1);

	Pos.x = SumPos.x/Count;
	Pos.y = SumPos.y/Count;
	Size.cx = SumSize.cx/Count;
	Size.cy = SumSize.cy/Count;
	SubRegion.SetRgn(Pos.x, Pos.y, Size.cx, Size.cy);

	SpiPad=(*this);
	SpiPad.ClearSubSpiPadList();
	SpiPad.SetPadRegion(SubRegion);
	return true;
}
//-------------------------------------------------------------------------------------//