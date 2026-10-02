// SPIComponent.cpp: implementation of the CSPIComponent class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "SPIComponent.h"
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
CSPIComponent::CSPIComponent()
{
	PreInitSpiComponent();
	InitialSpiComponent();
}
//-------------------------------------------------------------------------------------//
CSPIComponent::~CSPIComponent()
{

}
//-------------------------------------------------------------------------------------//
CSPIComponent::CSPIComponent(const CSPIComponent &other)
{
	PreInitSpiComponent();
	CloneSpiComponent(other);
}
//-------------------------------------------------------------------------------------//
CSPIComponent& CSPIComponent::operator=(const CSPIComponent &other)
{
	if ( this == &other ) { return *this; }
	CloneSpiComponent(other);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::PreInitSpiComponent()
{
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::InitialSpiComponent()
{
	m_BoardID = -1;
	m_ComponentName=L"";
	m_PackageName=L"";
	m_ComponentAngle=0;
	m_ComponentPos=TPOINT2D();
	m_ComponentSpecialPos = TPOINT2D();
	m_ComponentSize=TSIZE2D();
	m_ComponentToward=BOX_TOWARD_NULL;
	m_SpiPadList.clear();
	m_TempInt = 0;
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::CloneSpiComponent(const CSPIComponent &other)
{
	m_BoardID = other.m_BoardID;
	m_ComponentName = other.m_ComponentName;
	m_PackageName = other.m_PackageName;
	m_ComponentAngle = other.m_ComponentAngle;
	m_ComponentPos = other.m_ComponentPos;
	m_ComponentSpecialPos = other.m_ComponentSpecialPos;
	m_ComponentSize = other.m_ComponentSize;	
	m_ComponentToward = other.m_ComponentToward;		
	m_SpiPadList = other.m_SpiPadList;
	m_TempInt = other.m_TempInt;	
}
//-------------------------------------------------------------------------------------//
int CSPIComponent::GetBoardID() const
{
	return m_BoardID;
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::SetBoardID(int val)
{
	m_BoardID = val;
}
//-------------------------------------------------------------------------------------//
const wchar_t* CSPIComponent::GetComponentName() const
{
	return m_ComponentName.c_str();
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::SetComponentName(const wchar_t *val)
{
	m_ComponentName = val;
}
//-------------------------------------------------------------------------------------//
const wchar_t* CSPIComponent::GetPackageName() const
{
	return m_PackageName.c_str();
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::SetPackageName(const wchar_t *val)
{
	m_PackageName=val;
}
//-------------------------------------------------------------------------------------//
const TPOINT2D& CSPIComponent::GetComponentPos() const
{
	return m_ComponentPos;
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::SetComponentPos(double x, double y)
{
	m_ComponentPos.x = x;
	m_ComponentPos.y = y;
}
//-------------------------------------------------------------------------------------//
const TPOINT2D & CSPIComponent::GetComponentSpecialPos() const
{
	return m_ComponentSpecialPos;
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::SetComponentSpecialPos(double x, double y)
{
	m_ComponentSpecialPos.x = x;
	m_ComponentSpecialPos.y = y;
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::SetComponentSpecialPos(const TPOINT2D & val)
{
	m_ComponentSpecialPos = val;
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::SetComponentPos(const TPOINT2D &val)
{
	m_ComponentPos = val;
}
//-------------------------------------------------------------------------------------//
const TSIZE2D& CSPIComponent::GetComponentSize() const
{
	return m_ComponentSize;
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::SetComponentSize(double cx, double cy)
{
	m_ComponentSize.cx = cx;
	m_ComponentSize.cy = cy;
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::SetComponentSize(const TSIZE2D &val)
{
	m_ComponentSize = val;
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::SetComponentRegion(const TREGION4D &Region)
{
	SetComponentPos(Region.GetCpX(), Region.GetCpY());
	SetComponentSize(Region.GetSizeX(), Region.GetSizeY());
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::CheckComponentRegion(TREGION4D &Region) const
{
	const double SizeX2=GetComponentSize().cx/2;
	const double SizeY2=GetComponentSize().cy/2;
	Region.minX=GetComponentPos().x-SizeX2;
	Region.minY=GetComponentPos().y-SizeY2;
	Region.maxX=GetComponentPos().x+SizeX2;
	Region.maxY=GetComponentPos().y+SizeY2;
	return;
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::GetComponentCornerPos(TPOINT2D Pos[]) const
{
	TREGION4D Region;
	CheckComponentRegion(Region);	
	Pos[0].x=Region.minX;	Pos[0].y=Region.maxY;
	Pos[1].x=Region.maxX;	Pos[1].y=Region.maxY;	
	Pos[2].x=Region.maxX;	Pos[2].y=Region.minY;
	Pos[3].x=Region.minX;	Pos[3].y=Region.minY;
	return;
}
//-------------------------------------------------------------------------------------//
BOX_TOWARD CSPIComponent::GetComponentToward() const
{
	return m_ComponentToward;
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::SetComponentToward(BOX_TOWARD val)
{
	m_ComponentToward = val;
}
//-------------------------------------------------------------------------------------//
double CSPIComponent::GetComponentAngle() const
{
	return m_ComponentAngle;
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::SetComponentAngle(double val)
{
	m_ComponentAngle = val;
}
//-------------------------------------------------------------------------------------//
bool CSPIComponent::RotateComponent(double Angle)
{	
	TREGION4D Region;
	TPOINT2D CornerPos[4];
	double ComAngle=GetComponentAngle();
	const TPOINT2D Cp=GetComponentPos();
	BOX_TOWARD Toward=GetComponentToward();

	Angle=JetAPI::AdjustRotationAngle(Angle);
	GetComponentCornerPos(CornerPos);
	Toward=JetAPI::RotateToward(Angle, Toward);
	ComAngle=JetAPI::RotateAngle(ComAngle, Angle);
	JetAPI::RotateCornerPos(Angle, Cp.x, Cp.y, CornerPos);
	JetAPI::CornerPtToRegion(CornerPos, Region);

	size_t i=0;
	CSPIPad *SpiPadPtr=NULL;
	const size_t SpiPadCount=GetSpiPadCount();
	for ( i=0; i<SpiPadCount; i++ )
	{
		SpiPadPtr=GetSpiPadPtr(i, false);
		if ( NULL == SpiPadPtr ) { continue; }
		SpiPadPtr->RotatePadAngle(Angle, Cp);
	}
	
	SetComponentToward(Toward);
	SetComponentRegion(Region);	
	SetComponentAngle(ComAngle);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSPIComponent::MoveComponentPos(double x, double y)
{
	size_t i=0;
	CSPIPad *SpiPadPtr=NULL;
	const size_t SpiPadCount=GetSpiPadCount();
	for ( i=0; i<SpiPadCount; i++ )
	{
		SpiPadPtr=GetSpiPadPtr(i, false);
		if ( NULL == SpiPadPtr ) { continue; }
		SpiPadPtr->MovePadPos(x, y);
	}
	m_ComponentPos.x += x;
	m_ComponentPos.y += y;	
	return true;
}
//-------------------------------------------------------------------------------------//
int CSPIComponent::GetTempInt() const
{
	return m_TempInt;
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::SetTempInt(int val)
{
	m_TempInt = val;
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::ClearSpiPadList()
{
	m_SpiPadList.clear();
}
//-------------------------------------------------------------------------------------//
size_t CSPIComponent::GetSpiPadCount() const
{
	return m_SpiPadList.size();
}
//-------------------------------------------------------------------------------------//
void CSPIComponent::AddSpiPad(const CSPIPad &SpiPad)
{
	m_SpiPadList.push_back(SpiPad);
}
//-------------------------------------------------------------------------------------//
CSPIPad* CSPIComponent::GetSpiPadPtr(size_t idx, bool bCheck)
{
	if ( true == bCheck )
	{
		size_t Cnt=m_SpiPadList.size();
		if ( idx >= Cnt )
		{	return NULL; }
	}
	return &(m_SpiPadList[idx]);
}
//-------------------------------------------------------------------------------------//
bool CSPIComponent::AddSpiPadList(const std::vector<CSPIPad> &List)
{
	size_t i=0;
	const size_t Count=List.size();
	for ( i=0; i<Count; i++ )
	{	AddSpiPad(List[i]);	}

	CSPIPad *SpiPadPtr=GetSpiPadPtr(0, true);
	if ( NULL != SpiPadPtr )
	{
		SetPackageName(SpiPadPtr->GetPackageName());
		SetComponentName(SpiPadPtr->GetComponentName());
		SetComponentAngle(SpiPadPtr->GetComponentAngle());		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSPIComponent::SortSpiPadList(bool SortByPosX, std::vector<CSPIPad> &SpiPadList)
{
	size_t i=0;
	TPOINT2D PadPos;
	CSortObj SortObj;
	std::vector<CSortObj> SortList;	
	const size_t SpiPadCount=SpiPadList.size();
	if ( 0 == SpiPadCount ) { return true; }

	SortObj.SetSortMode(SORT_BY_DBL);
	for ( i=0; i<SpiPadCount; i++ )
	{
		const CSPIPad &SpiPad=SpiPadList[i];
		SpiPad.CheckPadPos(PadPos);
		if ( true == SortByPosX ) 
		{	SortObj.SetValueDbl(PadPos.x);	}
		else
		{	SortObj.SetValueDbl(PadPos.y);	}
		SortObj.SetID(i);
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());

	size_t SpiPadIdx=0;
	const size_t SortCount=SortList.size();
	std::vector<CSPIPad> SpiPadList2(SpiPadCount);
	SpiPadList2.clear();
	for ( i=0; i<SortCount; i++ )
	{
		const CSortObj &SortRef=SortList[i];
		SpiPadIdx=SortRef.GetID();
		if ( SpiPadIdx>=SpiPadCount ) { continue; }
		SpiPadList2.push_back(SpiPadList[SpiPadIdx]);
	}
	SpiPadList=SpiPadList2;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSPIComponent::SortSpiPadList(BOX_TOWARD Toward, bool bCWMode, std::vector<CSPIPad> &SpiPadList)
{
	size_t i=0;
	TPOINT2D PadPos;
	CSortObj SortObj;	
	bool DecMode=false;
	bool SortByPosX=true;
	std::vector<CSortObj> SortList;	
	const size_t SpiPadCount=SpiPadList.size();
	if ( 0 == SpiPadCount ) { return true; }
	switch ( Toward )
	{
	case BOX_TOWARD_UP:		
	case BOX_TOWARD_DOWN:		
		SortByPosX = true;
		break;
	case BOX_TOWARD_LEFT:		
	case BOX_TOWARD_RIGHT:
		SortByPosX = false;
		break;
	}
	switch ( Toward )
	{
	case BOX_TOWARD_UP:
	case BOX_TOWARD_LEFT:
		if ( true == bCWMode )
		{	DecMode = false; }
		else
		{	DecMode = true; }
		
		break;
	case BOX_TOWARD_DOWN:
	case BOX_TOWARD_RIGHT:
		if ( true == bCWMode )
		{	DecMode = true; }
		else
		{	DecMode = false; }
		SortByPosX = true;
		break;	
	}
	SortObj.SetSortMode(SORT_BY_DBL);
	for ( i=0; i<SpiPadCount; i++ )
	{
		const CSPIPad &SpiPad=SpiPadList[i];
		SpiPad.CheckPadPos(PadPos);		
		if ( true == SortByPosX ) 
		{	SortObj.SetValueDbl(PadPos.x);	}
		else
		{	SortObj.SetValueDbl(PadPos.y);	}
		SortObj.SetID(i);
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());
	if ( true == DecMode )
	{	std::reverse(SortList.begin(), SortList.end()); }

	size_t SpiPadIdx=0;
	const size_t SortCount=SortList.size();
	std::vector<CSPIPad> SpiPadList2(SpiPadCount);
	SpiPadList2.clear();
	for ( i=0; i<SortCount; i++ )
	{
		const CSortObj &SortRef=SortList[i];
		SpiPadIdx=SortRef.GetID();
		if ( SpiPadIdx>=SpiPadCount ) { continue; }
		SpiPadList2.push_back(SpiPadList[SpiPadIdx]);
	}
	SpiPadList=SpiPadList2;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSPIComponent::GetSpiPadListByToward(BOX_TOWARD Toward, std::vector<CSPIPad> &SpiPadList) const
{
	size_t i=0;		
	const size_t SpiPadCount=m_SpiPadList.size();
	SpiPadList.clear();
	for ( i=0; i<SpiPadCount; i++ )
	{
		const CSPIPad &SpiPadRef=m_SpiPadList[i];		
		if ( Toward != SpiPadRef.GetPadToward() ) { continue; }
		SpiPadList.push_back(SpiPadRef);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSPIComponent::AddSpiPadListToAll(const std::vector<CSPIPad> &SpiPadList, std::vector<CSPIPad> &SpiPadListAll) const
{
	const size_t SpiPadListCntAll=SpiPadListAll.size();
	if ( 0 == SpiPadListCntAll )
	{
		SpiPadListAll=SpiPadList;
		return true;
	}

	size_t i=0;
	bool bDebug = false;
	const size_t SpiPadListCnt=SpiPadList.size();
	if ( SpiPadListCnt != SpiPadListCntAll )
	{	
		bDebug = true; 
		return true;
	}

	for ( i=0; i<SpiPadListCnt; i++ )
	{
		if ( SpiPadListAll[i].CheckSubSpiPadAccept(SpiPadList[i]) == true )
		{	SpiPadListAll[i].AddSubSpiPad(SpiPadList[i]);	}
		else 
		{	bDebug = true; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSPIComponent::AddSpiPadAverageToList(const std::vector<CSPIPad> &SpiPadListAll, std::vector<CSPIPad> &SpiPadList) const
{
	size_t i=0;
	CSPIPad AveSpiPad;
	const size_t SpiPadListAllCnt=SpiPadListAll.size();	
	SpiPadList.clear();
	for ( i=0; i<SpiPadListAllCnt; i++ )
	{
		SpiPadListAll[i].CalcAverageSpiPad(AveSpiPad);
		SpiPadList.push_back(AveSpiPad);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSPIComponent::AnalysisSpiPadConfig()
{
	size_t i=0;
	bool   bFirst=true;	
	CSPIPad *SpiPadPtr=NULL;
	TPOINT2D  PadPos;
	TSIZE2D   PadSize;
	TREGION4D PadRegion;	
	TREGION4D ComponentRegion;	
	const size_t SpiPadCount=GetSpiPadCount();

	if ( m_ComponentName == L"U0300" )//&& 1==m_BoardID 
	{	i = 0;	}
	if ( m_ComponentName == L"SPKR_NEG" )//&& 1==m_BoardID 
	{	i = 0;	}
	if ( m_ComponentName == L"SPKR_POS" )//&& 1==m_BoardID 
	{	i = 0;	}	

	AnalysisComponentRegion(ComponentRegion);	
	const double   ComponetnAngle=GetComponentAngle();
	const TPOINT2D ComponentPos=TPOINT2D(ComponentRegion.GetCpX(), ComponentRegion.GetCpY());
	const TSIZE2D  ComponentSize=TSIZE2D(ComponentRegion.GetSizeX(), ComponentRegion.GetSizeY());	

	bFirst = true;	
	TPOINT2D CornerPtTL, CornerPtTR;//上方左側, //上方右側
	TPOINT2D CornerPtLT, CornerPtLB;//左側上方, //左側下方
	TPOINT2D CornerPtBL, CornerPtBR;//下方左側, //下方右側
	TPOINT2D CornerPtRT, CornerPtRB;//右側上方, //右側下方
	const double Precision=0.001;
	const double CheckComponentPosX1=ComponentPos.x;
	const double CheckComponentPosX2=ComponentPos.x;
	const double CheckComponentPosY1=ComponentPos.y;
	const double CheckComponentPosY2=ComponentPos.y;	
	CornerPtTL.x = ComponentRegion.minX;	CornerPtTL.y = ComponentRegion.maxY;//上方左側
	CornerPtTR.x = ComponentRegion.maxX;	CornerPtTR.y = ComponentRegion.maxY;//上方右側
	CornerPtLT.x = ComponentRegion.minX;	CornerPtLT.y = ComponentRegion.maxY;//左側上方
	CornerPtLB.x = ComponentRegion.minX;	CornerPtLB.y = ComponentRegion.minY;//左側下方
	CornerPtBL.x = ComponentRegion.minX;	CornerPtBL.y = ComponentRegion.minY;//下方左側
	CornerPtBR.x = ComponentRegion.maxX;	CornerPtBR.y = ComponentRegion.minY;//下方右側
	CornerPtRT.x = ComponentRegion.maxX;	CornerPtRT.y = ComponentRegion.maxY;//右側上方
	CornerPtRB.x = ComponentRegion.maxX;	CornerPtRB.y = ComponentRegion.minY;//右側下方

	for ( i=0; i<SpiPadCount; i++ )
	{
		SpiPadPtr = GetSpiPadPtr(i, false);
		if ( NULL == SpiPadPtr ) { continue; }
		SpiPadPtr->CheckPadPos(PadPos);
		SpiPadPtr->CheckPadRegion(PadRegion);		
		if ( PadPos.x < (CheckComponentPosX1) )//Left
		{	
			CornerPtTL.x = MIN(PadRegion.minX, CornerPtTL.x);//上方左側
			CornerPtBL.x = MIN(PadRegion.minX, CornerPtBL.x);//下方左側
			CornerPtLT.x = MIN(PadRegion.maxX, CornerPtLT.x);//左側上方
			CornerPtLB.x = MIN(PadRegion.maxX, CornerPtLB.x);//左側下方
		}
		else if ( PadPos.x > (CheckComponentPosX2) )//Right
		{	
			CornerPtTR.x = MAX(PadRegion.maxX, CornerPtTR.x);//上方右側
			CornerPtBR.x = MAX(PadRegion.maxX, CornerPtBR.x);//下方右側
			CornerPtRT.x = MAX(PadRegion.minX, CornerPtRT.x);//右側上方
			CornerPtRB.x = MAX(PadRegion.minX, CornerPtRB.x);//右側下方
		}

		if ( PadPos.y < (CheckComponentPosY1) )//Bottom
		{	
			CornerPtLB.y = MIN(PadRegion.minY, CornerPtLB.y);//左側下方
			CornerPtRB.y = MIN(PadRegion.minY, CornerPtRB.y);//右側下方
			CornerPtBL.y = MIN(PadRegion.maxY, CornerPtBL.y);//下方左側
			CornerPtBR.y = MIN(PadRegion.maxY, CornerPtBR.y);//下方右側

		}
		else if ( PadPos.y > (CheckComponentPosY2) )//Top
		{	
			CornerPtLT.y = MAX(PadRegion.maxY, CornerPtLT.y);//左側上方
			CornerPtRT.y = MAX(PadRegion.maxY, CornerPtRT.y);//右側上方
			CornerPtTL.y = MAX(PadRegion.minY, CornerPtTL.y);//上方左側
			CornerPtTR.y = MAX(PadRegion.minY, CornerPtTR.y);//上方右側
		}
	}		
	const double AngleLT=Calc2PointsAngle(CornerPtLT, ComponentPos);
	const double AngleLB=Calc2PointsAngle(CornerPtLB, ComponentPos);
	const double AngleRT=Calc2PointsAngle(CornerPtRT, ComponentPos);
	const double AngleRB=Calc2PointsAngle(CornerPtRB, ComponentPos);
	const double AngleTL=Calc2PointsAngle(CornerPtTL, ComponentPos);
	const double AngleTR=Calc2PointsAngle(CornerPtTR, ComponentPos);
	const double AngleBL=Calc2PointsAngle(CornerPtBL, ComponentPos);
	const double AngleBR=Calc2PointsAngle(CornerPtBR, ComponentPos);

	BOX_TOWARD Toward;
	double PadArea=0.0;
	double PadAngle=0.0;		
	double PadDistance=0.0;		
	const int ComponetnAngleLabel=JetAPI::GetAngleLabel(ComponetnAngle);
	switch ( ComponetnAngleLabel )
	{
	case  90: Toward=BOX_TOWARD_UP; break;
	case 180: Toward=BOX_TOWARD_LEFT; break;
	case 270: Toward=BOX_TOWARD_DOWN; break;
	default: Toward=BOX_TOWARD_RIGHT; break;
	}
	const BOX_TOWARD ComponentToward=Toward;
	//計算每個Pad和零件的夾角
	for ( i=0; i<SpiPadCount; i++ )
	{
		SpiPadPtr = GetSpiPadPtr(i, false);
		if ( NULL == SpiPadPtr ) { continue; }
		PadAngle=0.0;
		PadDistance=0.0;
		PadArea=SpiPadPtr->GetPadArea();
		Toward=BOX_TOWARD_NULL;
		SpiPadPtr->CheckPadPos(PadPos);		
		PadAngle=Calc2PointsAngle(PadPos, ComponentPos);
		PadDistance=Calc2PointsDistance(PadPos, ComponentPos);//計算誤差
		if ( 1==SpiPadCount || PadDistance<10.0 )
		{	Toward = ComponentToward;	}
		else
		{			
			if ( PadAngle>AngleTR && PadAngle<AngleTL )
			{	Toward = BOX_TOWARD_UP; }			
			else if ( PadAngle>AngleBL && PadAngle<AngleBR )
			{	Toward = BOX_TOWARD_DOWN; }
			else if ( PadAngle>AngleLT && PadAngle<AngleLB )
			{	Toward = BOX_TOWARD_LEFT; }
			else
			{	Toward = BOX_TOWARD_RIGHT; }			
		}
		SpiPadPtr->SetPadToward(Toward);
		SpiPadPtr->SetPadIncludedAngle(PadAngle);
	}

	std::vector<CAOIBox> PadBoxList(SpiPadCount);
	for ( i=0; i<SpiPadCount; i++ )
	{
		CAOIBox &BoxRef=PadBoxList[i];
		SpiPadPtr = GetSpiPadPtr(i, false);
		BoxRef.SetBoxIndex(i);
		if ( NULL == SpiPadPtr ) { continue; }		

		double RotateAngle=0.0;
		Toward = SpiPadPtr->GetPadToward();
		SpiPadPtr->CheckPadRegion(PadRegion);
		const double CpX=PadRegion.GetCpX();
		const double CpY=PadRegion.GetCpY();

		BoxRef.SetBoxToward(Toward);
		BoxRef.SetBoxRegion(PadRegion);		
		BoxRef.SetBoxResultID(RESULT_ID_NONE);
		switch ( Toward )
		{
		case BOX_TOWARD_LEFT:	RotateAngle=180;	break;
		case BOX_TOWARD_UP:		RotateAngle=270;	break;			
		case BOX_TOWARD_DOWN:	RotateAngle= 90;	break;			
		default:
		case BOX_TOWARD_RIGHT:	RotateAngle=0.0;	break;
		}		
		BoxRef.RotateBox(RotateAngle, CpX, CpY);
		BoxRef.SetBoxRegionRes(PadRegion);//避免朝向分類錯誤
	}

	//比較尺寸來分群
	int PadGroupID = 0;
	double SizeGapW= 0;
	double SizeGapH= 0;
	const double SizeRatio=0.1;
	const double MinRatio=0.90;
	const double MaxRatio=1.10;
	for ( i=0; i<SpiPadCount; i++ )
	{
		CAOIBox &BoxRef=PadBoxList[i];
		if ( RESULT_ID_NONE != BoxRef.GetBoxResultID() ) 
		{	continue; }
		BoxRef.GetBoxRegion(PadRegion);		
		const double BoxW=PadRegion.GetSizeX();
		const double BoxH=PadRegion.GetSizeY();
		SizeGapW=BoxW*SizeRatio;
		SizeGapH=BoxH*SizeRatio;
		SizeGapW = MAX(10, SizeGapW);
		SizeGapW = MIN(100, SizeGapW);
		SizeGapH = MAX(10, SizeGapH);
		SizeGapH = MIN(100, SizeGapH);

		const double BoxW_Min=BoxW-SizeGapW;
		const double BoxW_Max=BoxW+SizeGapW;
		const double BoxH_Min=BoxH-SizeGapH;
		const double BoxH_Max=BoxH+SizeGapH;
		for ( size_t j=0; j<SpiPadCount; j++ )
		{
			CAOIBox &BoxRef2=PadBoxList[j];
			if ( RESULT_ID_NONE != BoxRef2.GetBoxResultID() ) 
			{	continue; }
			bool bMatch = true;
			BoxRef2.GetBoxRegion(PadRegion);
			const double BoxW2=PadRegion.GetSizeX();
			const double BoxH2=PadRegion.GetSizeY();
			if ( BoxW2<BoxW_Min || BoxW2>BoxW_Max || BoxH2<BoxH_Min || BoxH2>BoxH_Max )
			{
				BoxRef2.GetBoxRegionRes(PadRegion);//避免朝向分類錯誤
				const double BoxW3=PadRegion.GetSizeX();
				const double BoxH3=PadRegion.GetSizeY();
				if ( BoxW3<BoxW_Min || BoxW3>BoxW_Max || BoxH3<BoxH_Min || BoxH3>BoxH_Max )
				{	continue;	}
			}
			BoxRef2.SetBoxResultID(RESULT_ID_OK);
			BoxRef2.SetBoxResultValue(PadGroupID);	
		}
		PadGroupID += 1;
	}

	for ( i=0; i<SpiPadCount; i++ )
	{	
		SpiPadPtr = GetSpiPadPtr(i, false);		
		if ( NULL == SpiPadPtr ) { continue; }		
		const CAOIBox &BoxRef=PadBoxList[i];
		PadGroupID = static_cast<int>(BoxRef.GetBoxResultValue());
		SpiPadPtr->SetPadGroupID(PadGroupID);
	}
	
	SetComponentPos(ComponentPos);
	SetComponentSize(ComponentSize);	
	SetComponentToward(ComponentToward);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CSPIComponent::AnalysisComponentRegion(TREGION4D &Region)
{
	size_t i=0;
	bool   bFirst=true;	
	CSPIPad *SpiPadPtr=NULL;
	TPOINT2D  PadPos;
	TSIZE2D   PadSize;
	TREGION4D PadRegion;	
	TREGION4D ComponentRegion;	
	const size_t SpiPadCount=GetSpiPadCount();

	bFirst = true;
	for ( i=0; i<SpiPadCount; i++ )
	{
		SpiPadPtr = GetSpiPadPtr(i, false);
		if ( NULL == SpiPadPtr ) { continue; }
		SpiPadPtr->CheckPadRegion(PadRegion);		
		if ( true == bFirst )
		{
			bFirst = false;
			ComponentRegion=PadRegion;			
			continue;
		}
		if ( ComponentRegion.minX > PadRegion.minX )
		{	ComponentRegion.minX = PadRegion.minX;	}
		if ( ComponentRegion.minY > PadRegion.minY )
		{	ComponentRegion.minY = PadRegion.minY;	}

		if ( ComponentRegion.maxX < PadRegion.maxX )		
		{	ComponentRegion.maxX = PadRegion.maxX;	}
		if ( ComponentRegion.maxY < PadRegion.maxY )
		{	ComponentRegion.maxY = PadRegion.maxY;	}		
	}
	const double PosX=ComponentRegion.GetCpX();
	const double PosY=ComponentRegion.GetCpY();
	const double SizeX=ComponentRegion.GetWidth();
	const double SizeY=ComponentRegion.GetHeight();
	const double SizeX_new=SizeX*0.75;
	const double SizeY_new=SizeY*0.75;
	ComponentRegion.SetRgn(PosX, PosY, SizeX_new, SizeY_new);

	Region=ComponentRegion;	
	SetComponentRegion(ComponentRegion);	
	return true;
}
//-------------------------------------------------------------------------------------//
double CSPIComponent::Calc2PointsAngle(const TPOINT2D &pt, const TPOINT2D &cp)
{
	double Angle=::atan2(pt.y-cp.y, pt.x-cp.x)*RAD_TO_DEG_DBL;	
	if ( Angle < 0 ) { Angle += 360.0; }
	return Angle;
}
//-------------------------------------------------------------------------------------//
double CSPIComponent::Calc2PointsDistance(const TPOINT2D &pt, const TPOINT2D &cp)
{
	const double DistX=pt.x-cp.x;
	const double DistY=pt.y-cp.y;
	return ::sqrt((DistX*DistX)+(DistY*DistY));
}
//-------------------------------------------------------------------------------------//