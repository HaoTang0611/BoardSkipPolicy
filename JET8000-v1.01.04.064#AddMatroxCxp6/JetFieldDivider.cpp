// JetFieldDivider.cpp: implementation of the CJetFieldDivider class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "JetFieldDivider.h"
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
bool sortJetRgn_fn(const TJetRgn & m1, const TJetRgn & m2) 
{
	if ( m1.priority1 == m2.priority1 )
	{
		return m1.priority2 > m2.priority2; 		
	}
	return m1.priority1 > m2.priority1;
}
//-------------------------------------------------------------------------------------//
bool sortMaxJetField_fn(const TJetField & m1, const TJetField & m2) 
{
	if ( m1.priority1 == m2.priority1 )
	{
		return m1.priority2 > m2.priority2; 
	}	
	return m1.priority1 > m2.priority1;
}
//-------------------------------------------------------------------------------------//
bool sortMinJetField_fn(const TJetField & m1, const TJetField & m2) 
{
	if ( m1.priority1 == m2.priority1 )
	{
		return m1.priority2 < m2.priority2; 
	}	
	return m1.priority1 < m2.priority1;
}
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CJetFieldDivider::CJetFieldDivider()
{
	CJetFieldDivider::PreInitDivider();
	CJetFieldDivider::InitialDivider();
}
//-------------------------------------------------------------------------------------//
CJetFieldDivider::~CJetFieldDivider()
{

}
//-------------------------------------------------------------------------------------//
inline void CJetFieldDivider::PreInitDivider()
{
}
//-------------------------------------------------------------------------------------//
inline void CJetFieldDivider::InitialDivider()
{
	m_FieldMaxW = 1000;
	m_FieldMaxH = 1000;
	m_DivisionMode = FIELD_DIVISION_DIAGONAL_LINE;
	m_PathMode = FIELD_PATH_SPATH_HOR;
	m_SectionFactor = 0.5;
	m_PathStartPosX = 0;
	m_PathStartPosY = 0;
}
//-------------------------------------------------------------------------------------//
void CJetFieldDivider::ResetDivider()
{
	CJetFieldDivider::m_RgnList.clear();
	CJetFieldDivider::m_FieldList.clear();
}
//-------------------------------------------------------------------------------------//
size_t CJetFieldDivider::GetJetRgnCount() const
{
	return CJetFieldDivider::m_RgnList.size();
}
//-------------------------------------------------------------------------------------//
void CJetFieldDivider::AddJetRgn(TJetRgn &Rgn)
{	
	CJetFieldDivider::m_RgnList.push_back(Rgn);
}
//-------------------------------------------------------------------------------------//
TJetRgn* CJetFieldDivider::GetJetRgnPtr(size_t idx, bool check)
{
	if ( true == check )
	{
		const size_t size = CJetFieldDivider::m_RgnList.size();
		if ( idx >= size ) { return NULL; }
	}
	return &(m_RgnList[idx]);
}
//-------------------------------------------------------------------------------------//
void  CJetFieldDivider::CloneJetRgnList(TJetRgnList &RgnList)
{
	RgnList = CJetFieldDivider::m_RgnList;
}
//-------------------------------------------------------------------------------------//
size_t CJetFieldDivider::GetJetFieldCount() const
{
	return CJetFieldDivider::m_FieldList.size();
}
//-------------------------------------------------------------------------------------//
TJetField* CJetFieldDivider::GetJetFieldPtr(size_t idx, bool check)
{
	if ( true == check )
	{
		const size_t Count = CJetFieldDivider::m_FieldList.size();
		if ( idx >= Count ) 
		{	return NULL; }
	}
	return &(m_FieldList[idx]);
}
//-------------------------------------------------------------------------------------//
void CJetFieldDivider::CloneJetFieldList(TJetFieldList &FieldList)
{
	FieldList = CJetFieldDivider::m_FieldList;
}
//-------------------------------------------------------------------------------------//
size_t CJetFieldDivider::GetJetPathCount() const
{
	return CJetFieldDivider::m_PathList.size();
}
//-------------------------------------------------------------------------------------//
TJetField* CJetFieldDivider::GetJetPathPtr(size_t idx, bool check)
{
	if ( true == check )
	{
		const size_t Count = CJetFieldDivider::m_PathList.size();
		if ( idx >= Count ) 
		{	return NULL; }
	}
	return &(m_PathList[idx]);
}
//-------------------------------------------------------------------------------------//
void CJetFieldDivider::CloneJetPathList(TJetFieldList &PathList)
{
	PathList = CJetFieldDivider::m_PathList;
}
//-------------------------------------------------------------------------------------//
bool CJetFieldDivider::ExecDivide()//執行分割
{
	bool IsOK = true;
	switch ( m_DivisionMode )
	{
	case FIELD_DIVISION_MASS_AREA:
		IsOK = ExecDivide_MassArea(m_RgnList, m_FieldList);
		break;
	case FIELD_DIVISION_DIAGONAL_LINE:
		IsOK = ExecDivide_DiagonalLine(m_RgnList, m_FieldList);
		break;
	case FIELD_DIVISION_HORIZONTAL_LINE:
		IsOK = ExecDivide_HorizontalLine(m_RgnList, m_FieldList);
		break;
	case FIELD_DIVISION_VERTICAL_LINE:
		IsOK = ExecDivide_VerticalLine(m_RgnList, m_FieldList);
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetFieldDivider::BuildPriority_MassArea(TJetRgnList &RgnList)//建立區域的優先順序-面積
{	
	size_t   i=0;
	double   CPX=0, CPY=0;
	TJetRgn *RgnPtr = NULL;
	const    size_t RgnCount = RgnList.size();	

	//將全部先復歸
	for ( i=0; i<RgnCount; i++ )
	{
		RgnPtr = &(RgnList[i]);		

		RgnPtr->used = false;				
		
		RgnPtr->priority1 = (RgnPtr->maxX-RgnPtr->minX)*(RgnPtr->maxY-RgnPtr->minY);
		CPX = (RgnPtr->maxX+RgnPtr->minX)/2;
		CPY = (RgnPtr->maxY+RgnPtr->minY)/2;
		RgnPtr->priority2 = (CPX*CPX)+(CPY*CPY);			
	}		
	
	//尋找面積最大
	std::sort(RgnList.begin(), RgnList.end(), sortJetRgn_fn);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CJetFieldDivider::BuildPriority_DiagonalLine(TJetRgnList &RgnList)//建立區域的優先順序-對角線
{
	size_t   i=0;
	double   CPX=0, CPY=0;
	TJetRgn *RgnPtr = NULL;
	const    size_t RgnCount = RgnList.size();	

	//將全部先復歸
	for ( i=0; i<RgnCount; i++ )
	{
		RgnPtr = &(RgnList[i]);		

		RgnPtr->used = false;				
		
		CPX = (RgnPtr->maxX+RgnPtr->minX)/2;
		CPY = (RgnPtr->maxY+RgnPtr->minY)/2;
		//CPX = (RgnPtr->maxX-RgnPtr->minX);//可能有蟲
		//CPY = (RgnPtr->maxY-RgnPtr->minY);//可能有蟲
		RgnPtr->priority1 = (CPX*CPX)+(CPY*CPY);
		RgnPtr->priority2 = (RgnPtr->maxX-RgnPtr->minX)*(RgnPtr->maxY-RgnPtr->minY);
	}		
	
	//尋找面積最大
	std::sort(RgnList.begin(), RgnList.end(), sortJetRgn_fn);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CJetFieldDivider::BuildPriority_HorizontalLine(TJetRgnList &RgnList)//建立區域的優先順序-水平線
{
	size_t   i=0;
	double   CPX=0, CPY=0;
	TJetRgn *RgnPtr = NULL;
	const    size_t RgnCount = RgnList.size();	

	//將全部先復歸
	for ( i=0; i<RgnCount; i++ )
	{
		RgnPtr = &(RgnList[i]);		

		RgnPtr->used = false;				
		
		CPX = (RgnPtr->maxX+RgnPtr->minX)/2;
		CPY = (RgnPtr->maxY+RgnPtr->minY)/2;		
		RgnPtr->priority1 = RgnPtr->maxX;
		RgnPtr->priority2 = RgnPtr->maxY;
	}		
	
	//尋找面積最大
	std::sort(RgnList.begin(), RgnList.end(), sortJetRgn_fn);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CJetFieldDivider::BuildPriority_VerticalLine(TJetRgnList &RgnList)//建立區域的優先順序-垂直線
{
	size_t   i=0;
	double   CPX=0, CPY=0;
	TJetRgn *RgnPtr = NULL;
	const    size_t RgnCount = RgnList.size();	

	//將全部先復歸
	for ( i=0; i<RgnCount; i++ )
	{
		RgnPtr = &(RgnList[i]);		

		RgnPtr->used = false;				
		
		CPX = (RgnPtr->maxX+RgnPtr->minX)/2;
		CPY = (RgnPtr->maxY+RgnPtr->minY)/2;		
		RgnPtr->priority1 = RgnPtr->maxY;
		RgnPtr->priority2 = RgnPtr->maxX;
	}		
	
	//尋找面積最大
	std::sort(RgnList.begin(), RgnList.end(), sortJetRgn_fn);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetFieldDivider::AdjustFieldSubRegion(const TJetRgn &Rgn, TJetField &Field)
{
	if ( Field.subMaxX < Rgn.maxX ) { Field.subMaxX = Rgn.maxX; }
	if ( Field.subMaxY < Rgn.maxY ) { Field.subMaxY = Rgn.maxY; }
	if ( Field.subMinX > Rgn.minX ) { Field.subMinX = Rgn.minX; }
	if ( Field.subMinY > Rgn.minY ) { Field.subMinY = Rgn.minY; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetFieldDivider::CheckRgnCombineField(TJetRgn &Rgn, double minX, double maxX, double minY, double maxY, double MaxW, double MaxH)
{
	double MinX = Rgn.minX;
	double MinY = Rgn.minY;
	double MaxX = Rgn.maxX;
	double MaxY = Rgn.maxY;

	if ( MinX > minX ) { MinX = minX; }
	if ( MaxX < maxX ) { MaxX = maxX; }

	if ( MinY > minY ) { MinY = minY; }
	if ( MaxY < maxY ) { MaxY = maxY; }

	double width  = MaxX-MinX;
	double height = MaxY-MinY;
	if ( width > MaxW ) { return false; }
	if ( height > MaxH ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetFieldDivider::ExecFieldDivision(TJetRgnList &RgnList, TJetFieldList &FieldList)//Field分割
{
	FieldList.clear();
	const size_t RgnCount = (RgnList.size());
	const double FieldMaxW   = this->m_FieldMaxW;
	const double FieldMaxH   = this->m_FieldMaxH;
	const double FieldMaxW2  = FieldMaxW/2;
	const double FieldMaxH2  = FieldMaxH/2;
	const double FieldCheckW = FieldMaxW+0.5;//因為浮點數問題所以額外加0.5
	const double FieldCheckH = FieldMaxH+0.5;//因為浮點數問題所以額外加0.5
	size_t i=0, j=0, k=0;
	double    width = 0;
	double    height = 0;
	bool      ReSearch = true;
	double    CPX=0, CPY=0;
	double    SubW=0, SubH=0;
	TJetRgn   *RgnPtr = NULL;
	TJetRgn   *RgnPtrOther = NULL;	
	TJetField  Field;
	
	for ( i=0; i<RgnCount; i++ )
	{
		RgnPtr = &(RgnList[i]);		
		if ( RgnPtr->used == true ) { continue; }
		
		Field.FieldID   = 0;
		//Field.SectionID = 0;
		Field.minX      = ((RgnPtr->minX+RgnPtr->maxX)/2)-FieldMaxW2;		
		Field.minY      = ((RgnPtr->minY+RgnPtr->maxY)/2)-FieldMaxH2;				
		Field.maxX      = Field.minX+FieldMaxW;
		Field.maxY      = Field.minY+FieldMaxH;
		Field.priority1 = 0;			
		Field.selected  = false;
		Field.visible   = false;
		//Field.FOVIndexX = -1;
		//Field.FOVIndexY = -1;
		
		Field.priority2 = 0; 		
		//Field.LightMode = pScanBlock->sLightMode; 	//Light Mode - Vic 20140405	
		Field.RgnIdxList.clear();
		
		Field.subMaxX  = RgnPtr->maxX;
		Field.subMinX  = RgnPtr->minX;
		Field.subMinY  = RgnPtr->minY;
		Field.subMaxY  = RgnPtr->maxY;

		RgnPtr->used = true;
		//Field.SectionID ++;
		Field.RgnIdxList.push_back(i);
		do 
		{
			ReSearch = false;
			for ( j=0; j<RgnCount; j++ )
			{
				RgnPtrOther = &(RgnList[j]);			
				if ( RgnPtrOther->used == true ) { continue; }
				//if ( RgnPtrOther->sLightMode != RgnPtr->sLightMode ) { continue; }	//Light Mode - Vic 20140405			
				//if (this->CheckRgnCombineField(*RgnPtrOther, Field.minX, Field.maxX, Field.minY, Field.maxY, FieldCheckW, FieldCheckH) == false ) { continue; }
				if (this->CheckRgnCombineField(*RgnPtrOther, Field.subMinX, Field.subMaxX, Field.subMinY, Field.subMaxY, FieldCheckW, FieldCheckH) == false ) { continue; }

				RgnPtrOther->used  = true;
				ReSearch  = true;				
				this->AdjustFieldSubRegion(*RgnPtrOther, Field);				
				//Field.SectionID ++;
				Field.RgnIdxList.push_back(j);
			}
			if ( ReSearch == false ) { break; }

			CPX = (Field.subMinX+Field.subMaxX)/2;
			CPY = (Field.subMinY+Field.subMaxY)/2;

			Field.minX   = CPX-FieldMaxW2;
			Field.minY   = CPY-FieldMaxH2;
			Field.maxX   = Field.minX+FieldMaxW;
			Field.maxY   = Field.minY+FieldMaxH;

		} while (true);
		Field.visible = true;		
		FieldList.push_back(Field);		
	}	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CJetFieldDivider::ExecDivide_MassArea(TJetRgnList &RgnList, TJetFieldList &FieldList)//執行分割-最大面積
{
	if ( BuildPriority_MassArea(RgnList) == false ) { return false; }
	if ( ExecFieldDivision(RgnList, FieldList) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetFieldDivider::ExecDivide_DiagonalLine(TJetRgnList &RgnList, TJetFieldList &FieldList)//執行分割-對角線
{
	if ( BuildPriority_DiagonalLine(RgnList) == false ) { return false; }
	if ( ExecFieldDivision(RgnList, FieldList) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetFieldDivider::ExecDivide_HorizontalLine(TJetRgnList &RgnList, TJetFieldList &FieldList)//執行分割-水平線
{
	if ( BuildPriority_HorizontalLine(RgnList) == false ) { return false; }
	if ( ExecFieldDivision(RgnList, FieldList) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetFieldDivider::ExecDivide_VerticalLine(TJetRgnList &RgnList, TJetFieldList &FieldList)//執行分割-垂直線
{
	if ( BuildPriority_VerticalLine(RgnList) == false ) { return false; }
	if ( ExecFieldDivision(RgnList, FieldList) == false ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetFieldDivider::ExecFieldPath(const TJetFieldList &FieldList)//執行路徑分配
{	
	bool IsOK = true;
	switch ( m_PathMode )
	{
	case FIELD_PATH_SPATH_HOR:
		IsOK = ExecFieldPath_Hor(FieldList, m_PathList);
		break;
	case FIELD_PATH_SPATH_VER:
		IsOK = ExecFieldPath_Ver(FieldList, m_PathList);
		break;	
	case FIELD_PATH_SPATH_USER:
		IsOK = ExecFieldPath_User(FieldList, m_PathList);
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CJetFieldDivider::ExecFieldPath_Hor(const TJetFieldList &FieldList, TJetFieldList &PathList)//執行路徑分配-水平為主
{
	PathList.clear();	
	const double MarginW=0;
	const double MarginH=0;	
	const double FieldMaxW = this->m_FieldMaxW-MarginW-MarginW;
	const double FieldMaxH = this->m_FieldMaxH-MarginH-MarginH;
	const double SectionMaxH = this->m_FieldMaxH*m_SectionFactor;
	double SectionMaxX=0, SectionMinX=0;
	double SectionMaxY=0, SectionMinY=0;
	const size_t FieldCount = FieldList.size();
	if ( FieldCount == 0 ) { return true; }

	bool First = true;	
	bool VerToMax=true;
	size_t i=0, j=0;
	size_t NoUseCount = 0;	
	double RegionMaxX=0, RegionMinX=0;
	double RegionMaxY=0, RegionMinY=0;

	TJetFieldList  tempFieldList = FieldList;
	TJetField     *FieldPtr = NULL;
	TJetField      NewPath;
	TJetRgn        Section;

	double LastPosX=0;
	double LastPosY=0;
	double PosX=0;
	double PosY=0;
	double PosYMax=0, PosYMin=0;	
	double PosXMax=0, PosXMin=0;	

	First = true;
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = &(tempFieldList[i]);

		FieldPtr->FieldID = i;
		//FieldPtr->SectionID = -1;
		FieldPtr->used = false;		
		FieldPtr->priority1 = FieldPtr->maxX;
		FieldPtr->priority2 = FieldPtr->minX;

		PosX = (FieldPtr->minX+FieldPtr->maxX)*0.5;
		PosY = (FieldPtr->minY+FieldPtr->maxY)*0.5;

		if ( First == true )
		{
			First = false;
			RegionMaxX = RegionMinX = PosX;
			RegionMaxY = RegionMinY = PosY;
		}
		else
		{
			if ( RegionMinX > PosX ) { RegionMinX = PosX; }
			if ( RegionMaxX < PosX ) { RegionMaxX = PosX; }
			if ( RegionMinY > PosY ) { RegionMinY = PosY; }
			if ( RegionMaxY < PosY ) { RegionMaxY = PosY; }
		}
	}
	const double RegionRangeW = RegionMaxX-RegionMinX;
	const double RegionRangeH = RegionMaxY-RegionMinY;
	const size_t MaxSectionCount = (size_t)(RegionRangeH/SectionMaxH)+1;
	const double DistMinX = fabs(m_PathStartPosX-RegionMinX);
	const double DistMinY = fabs(m_PathStartPosY-RegionMinY);
	const double DistMaxX = fabs(m_PathStartPosX-RegionMaxX);
	const double DistMaxY = fabs(m_PathStartPosY-RegionMaxY);
	TJetFieldList tempPathList2;	
	size_t SubFieldCount = 0;
	
	First = true;
	//Section.visible = true;	
	LastPosX = this->m_PathStartPosX;
	LastPosY = this->m_PathStartPosY;
	if ( DistMinX < DistMaxX ) 
	{	LastPosX = RegionMinX; }
	else
	{	LastPosX = RegionMaxX; }
	if ( DistMinY < DistMaxY ) 
	{
		VerToMax=true;
		LastPosY = RegionMinY; 
	}
	else
	{
		VerToMax=false;
		LastPosY = RegionMaxY; 
	}
	
	if ( true == VerToMax )
	{
		SectionMinY    = RegionMinY;
		SectionMaxY    = SectionMinY+SectionMaxH;
	}
	else
	{
		SectionMaxY    = RegionMaxY;
		SectionMinY    = SectionMaxY-SectionMaxH;
	}
	for ( j=0; j<MaxSectionCount; j++ )
	{		
		//尋找最小的Y當作區間起點
		First = true;
		NoUseCount = 0;
		for ( i=0; i<FieldCount; i++ )
		{
			FieldPtr = &(tempFieldList[i]);
			if ( FieldPtr->used == true ) { continue; }
			NoUseCount ++;

			PosY = (FieldPtr->minY+FieldPtr->maxY)*0.5;
			if ( First == true )
			{
				First = false;
				PosYMax = PosYMin = PosY;
			}
			else
			{
				if ( PosYMin > PosY ) { PosYMin = PosY; }
				if ( PosYMax < PosY ) { PosYMax = PosY; }
			}
		}
		if ( NoUseCount == 0 )//不用找了
		{	break; }

		if ( First == true ) //此區間沒有, 移動至下一個區間
		{	
			if ( true == VerToMax )
			{
				SectionMinY    = SectionMinY+SectionMaxH;
				SectionMaxY    = SectionMinY+SectionMaxH;
			}
			else
			{
				SectionMaxY    = SectionMaxY-SectionMaxH;
				SectionMinY    = SectionMaxY-SectionMaxH;
			}
			continue; 
		}		

		if ( true == VerToMax )
		{
			SectionMinY    = PosYMin;
			SectionMaxY    = SectionMinY+SectionMaxH;
		}
		else
		{
			SectionMaxY    = PosYMax;
			SectionMinY    = SectionMaxY-SectionMaxH;
		}
		Section.minY   = SectionMinY;
		Section.maxY   = SectionMaxY;

		First = true;
		for ( i=0; i<FieldCount; i++ )
		{
			FieldPtr = &(tempFieldList[i]);
			if ( FieldPtr->used == true ) { continue; }

			PosY = (FieldPtr->minY+FieldPtr->maxY)*0.5;

			if ( true == VerToMax )
			{
				if ( PosY > SectionMaxY )	{ continue; }
			}
			else
			{
				if ( PosY < SectionMinY )	{ continue; }
			}

			PosX = (FieldPtr->minX+FieldPtr->maxX)*0.5;
			if ( First == true )
			{
				First = false;
				Section.minX = FieldPtr->minX;
				Section.maxX = FieldPtr->maxX;
				SectionMaxX = SectionMinX = PosX;
			}
			else
			{
				if ( Section.minX > FieldPtr->minX ) { Section.minX = FieldPtr->minX; }
				if ( Section.maxX < FieldPtr->maxX ) { Section.maxX = FieldPtr->maxX; }
				if ( SectionMinX > PosX ) { SectionMinX = PosX; }
				if ( SectionMaxX < PosX ) { SectionMaxX = PosX; }
			}
					
			FieldPtr->used = true;
			NewPath = *FieldPtr;
			//NewPath.SectionID = (int)(SectionList.size());			
			
			NewPath.priority1 = PosX;
			NewPath.priority1 = PosX-LastPosX;//相對於上一個X
			NewPath.priority2 = 0;			
			tempPathList2.push_back(NewPath);
		}
		SubFieldCount = tempPathList2.size();
		if ( SubFieldCount == 0 ) 
		{
			if ( true == VerToMax )
			{
				SectionMinY    = SectionMinY+SectionMaxH;
				SectionMaxY    = SectionMinY+SectionMaxH;
			}
			else
			{
				SectionMaxY    = SectionMaxY-SectionMaxH;
				SectionMinY    = SectionMaxY-SectionMaxH;
			}
			continue; 
		}

		//Section.ID = (int)(SectionList.size());
		//SectionList.push_back(Section);
		
		PosXMax = (SectionMaxX-LastPosX)*(SectionMaxX-LastPosX);			
		PosXMin = (SectionMinX-LastPosX)*(SectionMinX-LastPosX);
		if ( PosXMax < PosXMin )//比較靠近X的最大值, 從X的最大值開始
		{	std::sort(tempPathList2.begin(), tempPathList2.end(), sortMaxJetField_fn);	 }
		else
		{	std::sort(tempPathList2.begin(), tempPathList2.end(), sortMinJetField_fn);	 }

		for ( i=0; i<SubFieldCount; i++ )
		{	PathList.push_back(tempPathList2[i]);	}

		//重新設定原點座標
		SubFieldCount = PathList.size();
		FieldPtr = &(PathList[SubFieldCount-1]);
		LastPosX = (FieldPtr->minX+FieldPtr->maxX)*0.5;
		LastPosY = (FieldPtr->minY+FieldPtr->maxY)*0.5;
		tempPathList2.clear();
		if ( true == VerToMax )
		{
			SectionMinY    = SectionMinY+SectionMaxH;
			SectionMaxY    = SectionMinY+SectionMaxH;
		}
		else
		{
			SectionMaxY    = SectionMaxY-SectionMaxH;
			SectionMinY    = SectionMaxY-SectionMaxH;
		}
	}	
	return true;		
}
//-------------------------------------------------------------------------------------//
bool CJetFieldDivider::ExecFieldPath_Ver(const TJetFieldList &FieldList, TJetFieldList &PathList)//執行路徑分配-垂直為主
{
	PathList.clear();
	const double MarginW=0;
	const double MarginH=0;	
	const double FieldMaxW = this->m_FieldMaxW-MarginW-MarginW;
	const double FieldMaxH = this->m_FieldMaxH-MarginH-MarginH;
	const double SectionMaxW = this->m_FieldMaxW*m_SectionFactor;
	double SectionMaxX=0, SectionMinX=0;
	double SectionMaxY=0, SectionMinY=0;
	const size_t FieldCount = FieldList.size();
	if ( FieldCount == 0 ) { return true; }
	
	bool First = true;	
	bool HorToMax=true;
	size_t i=0, j=0;
	size_t NoUseCount=0;	
	double RegionMaxX=0, RegionMinX=0;
	double RegionMaxY=0, RegionMinY=0;

	TJetFieldList  tempPathList = FieldList;
	TJetField     *FieldPtr = NULL;
	TJetField      NewPath;
	TJetRgn        Section;

	double LastPosX=0;
	double LastPosY=0;
	double PosX=0;
	double PosY=0;
	double PosXMax=0, PosXMin=0;	
	double PosYMax=0, PosYMin=0;	

	First = true;
	for ( i=0; i<FieldCount; i++ )
	{
		FieldPtr = &(tempPathList[i]);		

		FieldPtr->FieldID = i;
		//FieldPtr->SectionID = -1;
		FieldPtr->used = false;		
		FieldPtr->priority1 = FieldPtr->maxX;
		FieldPtr->priority2 = FieldPtr->minX;

		PosX = (FieldPtr->minX+FieldPtr->maxX)/2.0;
		PosY = (FieldPtr->minY+FieldPtr->maxY)/2.0;

		if ( First == true )
		{
			First = false;
			RegionMaxX = RegionMinX = PosX;
			RegionMaxY = RegionMinY = PosY;
		}
		else
		{
			if ( RegionMinX > PosX ) { RegionMinX = PosX; }
			if ( RegionMaxX < PosX ) { RegionMaxX = PosX; }
			if ( RegionMinY > PosY ) { RegionMinY = PosY; }
			if ( RegionMaxY < PosY ) { RegionMaxY = PosY; }
		}
	}
	const double RegionRangeW = RegionMaxX-RegionMinX;
	const double RegionRangeH = RegionMaxY-RegionMinY;
	const int MaxSectionCount = (int)(RegionRangeW/SectionMaxW)+1;
	const double DistMinX = fabs(m_PathStartPosX-RegionMinX);
	const double DistMinY = fabs(m_PathStartPosY-RegionMinY);
	const double DistMaxX = fabs(m_PathStartPosX-RegionMaxX);
	const double DistMaxY = fabs(m_PathStartPosY-RegionMaxY);
	TJetFieldList tempPathList2;	
	size_t SubFieldCount = 0;
	
	First = true;
	//Section.visible = true;	
	LastPosX = this->m_PathStartPosX;
	LastPosY = this->m_PathStartPosY;
	if ( DistMinX < DistMaxX ) 
	{
		HorToMax=true;
		LastPosX = RegionMinX; 
	}
	else
	{
		HorToMax=false;
		LastPosX = RegionMaxX; 
	}
	if ( DistMinY < DistMaxY ) 
	{	LastPosY = RegionMinY; }
	else
	{	LastPosY = RegionMaxY; }

	if ( true == HorToMax )
	{
		SectionMinX    = RegionMinX;
		SectionMaxX    = SectionMinX+SectionMaxW;
	}
	else
	{
		SectionMaxX    = RegionMaxX;
		SectionMinX    = SectionMaxX-SectionMaxW;
	}
	for ( j=0; j<MaxSectionCount; j++ )
	{	
		//尋找最小的X當作區間起點
		First = true;
		NoUseCount = 0;
		for ( i=0; i<FieldCount; i++ )
		{
			FieldPtr = &(tempPathList[i]);
			if ( FieldPtr->used == true ) { continue; }
			NoUseCount ++;

			PosX = (FieldPtr->minX+FieldPtr->maxX)*0.5;
			if ( First == true )
			{
				First = false;
				PosXMax = PosXMin = PosX;
			}
			else
			{
				if ( PosXMin > PosX ) { PosXMin = PosX; }
				if ( PosXMax < PosX ) { PosXMax = PosX; }
			}
		}
		if ( NoUseCount == 0 )//不用找了
		{	break; }

		if ( First == true ) //此區間沒有, 移動至下一個區間
		{	
			if ( true == HorToMax )
			{
				SectionMinX    = SectionMinX+SectionMaxW;
				SectionMaxX    = SectionMinX+SectionMaxW;
			}
			else
			{
				SectionMaxX    = SectionMaxX-SectionMaxW;
				SectionMinX    = SectionMaxX-SectionMaxW;
			}
			continue; 
		}		

		if ( true == HorToMax )
		{
			SectionMinX    = PosXMin;
			SectionMaxX    = SectionMinX+SectionMaxW;
		}
		else
		{
			SectionMaxX    = PosXMax;
			SectionMinX    = SectionMaxX-SectionMaxW;
		}
		Section.minX   = SectionMinX;
		Section.maxX   = SectionMaxX;
		First = true;
		for ( i=0; i<FieldCount; i++ )
		{
			FieldPtr = &(tempPathList[i]);
			if ( FieldPtr->used == true ) { continue; }

			PosX = (FieldPtr->minX+FieldPtr->maxX)*0.5;

			if ( true == HorToMax )
			{
				if ( PosX > SectionMaxX )	{ continue; }
			}
			else
			{	
				if ( PosX < SectionMinX )	{ continue; }
			}

			PosY = (FieldPtr->minY+FieldPtr->maxY)*0.5;
			if ( First == true )
			{
				First = false;
				Section.minY = FieldPtr->minY;
				Section.maxY = FieldPtr->maxY;
				SectionMaxY = SectionMinY = PosY;
			}
			else
			{
				if ( Section.minY > FieldPtr->minY ) { Section.minY = FieldPtr->minY; }
				if ( Section.maxY < FieldPtr->maxY ) { Section.maxY = FieldPtr->maxY; }
				if ( SectionMinY > PosY ) { SectionMinY = PosY; }
				if ( SectionMaxY < PosY ) { SectionMaxY = PosY; }
			}
					
			FieldPtr->used = true;
			NewPath = *FieldPtr;
			//NewPath.SectionID = (int)(SectionList.size());			
			
			NewPath.priority1 = PosY;
			NewPath.priority1 = PosY-LastPosY;//相對於上一個Y
			NewPath.priority2 = 0;			
			tempPathList2.push_back(NewPath);
		}
		SubFieldCount = tempPathList2.size();
		if ( SubFieldCount == 0 ) 
		{ 
			if ( true == HorToMax )
			{
				SectionMinX    = SectionMinX+SectionMaxW;
				SectionMaxX    = SectionMinX+SectionMaxW;
			}
			else
			{
				SectionMaxX    = SectionMaxX-SectionMaxW;
				SectionMinX    = SectionMaxX-SectionMaxW;
			}
			continue; 
		}

		//Section.ID = (int)(SectionList.size());
		//SectionList.push_back(Section);
		
		PosYMax = (SectionMaxY-LastPosY)*(SectionMaxY-LastPosY);			
		PosYMin = (SectionMinY-LastPosY)*(SectionMinY-LastPosY);
		if ( PosYMax < PosYMin )//比較靠近Y的最大值, 從Y的最大值開始
		{	std::sort(tempPathList2.begin(), tempPathList2.end(), sortMaxJetField_fn);	 }
		else
		{	std::sort(tempPathList2.begin(), tempPathList2.end(), sortMinJetField_fn);	 }

		for ( i=0; i<SubFieldCount; i++ )
		{	PathList.push_back(tempPathList2[i]);	}

		//重新設定原點座標
		SubFieldCount = PathList.size();
		FieldPtr = &(PathList[SubFieldCount-1]);
		LastPosX = (FieldPtr->minX+FieldPtr->maxX)*0.5;
		LastPosY = (FieldPtr->minY+FieldPtr->maxY)*0.5;
		tempPathList2.clear();
		if ( true == HorToMax )
		{
			SectionMinX    = SectionMinX+SectionMaxW;
			SectionMaxX    = SectionMinX+SectionMaxW;
		}
		else
		{
			SectionMaxX    = SectionMaxX-SectionMaxW;
			SectionMinX    = SectionMaxX-SectionMaxW;
		}
	}		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetFieldDivider::ExecFieldPath_User(const TJetFieldList &FieldList, TJetFieldList &PathList)//執行路徑分配-手動設定
{
	PathList.clear();
	const size_t FieldCount = FieldList.size();
	if ( FieldCount == 0 ) { return true; }

	size_t     i=0;
	size_t     ID=0;	
	CSortObj   SortObj;
	TJetField  NewPath;	
	std::vector<CSortObj> SortList(FieldCount);

	SortObj.SetSortMode(SORT_BY_CNT);
	for ( i=0; i<FieldCount; i++ )
	{
		CSortObj &refSortObj = SortList[i];
		const TJetField *FieldPtr = &(FieldList[i]);		
		refSortObj.SetID(i);
		refSortObj.SetSortMode(SORT_BY_CNT);
		refSortObj.SetValueCnt(FieldPtr->UserIndex);		
	}
	
	std::sort(SortList.begin(), SortList.end());
	const size_t SortCount=SortList.size();
	for ( i=0; i<SortCount; i++ )
	{
		CSortObj &refSortObj = SortList[i];
		ID = refSortObj.GetID();
		if ( ID >= FieldCount ) { continue; }
		NewPath = FieldList[ID];
		NewPath.FieldID = i;
		PathList.push_back(NewPath);
	}	
	return true;
}
//-------------------------------------------------------------------------------------//