// MapCoordinate.cpp: implementation of the CMapCoordinate class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "MapCoordinate.h"
//-------------------------------------------------------------------------------------//
#include "JetMatrix.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CMapCoordinate::CMapCoordinate()
{
	CMapCoordinate::PreInitMap();
	CMapCoordinate::InitialMap();
}
//-------------------------------------------------------------------------------------//
CMapCoordinate::CMapCoordinate(const CMapCoordinate &map)
{
	CMapCoordinate::PreInitMap();
	CMapCoordinate::CloneMap(map);
}
//-------------------------------------------------------------------------------------//
CMapCoordinate::~CMapCoordinate()
{

}
//-------------------------------------------------------------------------------------//
inline void CMapCoordinate::PreInitMap()
{
	return;
}
//-------------------------------------------------------------------------------------//
inline void CMapCoordinate::InitialMap()
{
	m_M11=1;	m_M12=0;	m_M13=0;	m_M14=0;
	m_M21=0;	m_M22=1;	m_M23=0;	m_M24=0;
	m_M31=0;	m_M32=0;	m_M33=1;	m_M34=0;
	m_M41=0;	m_M42=0;	m_M43=0;	m_M44=1;
	return;
}
//-------------------------------------------------------------------------------------//	
CMapCoordinate& CMapCoordinate::operator=(const CMapCoordinate &map)
{
	if ( this == &map ) { return *this; }
	CMapCoordinate::CloneMap(map);
	return *this;
}
//-------------------------------------------------------------------------------------//	
inline void CMapCoordinate::CloneMap(const CMapCoordinate &map)
{
	m_M11=map.m_M11;	m_M12=map.m_M12;	m_M13=map.m_M13;	m_M14=map.m_M14;
	m_M21=map.m_M21;	m_M22=map.m_M22;	m_M23=map.m_M23;	m_M24=map.m_M24;
	m_M31=map.m_M31;	m_M32=map.m_M32;	m_M33=map.m_M33;	m_M34=map.m_M34;
	m_M41=map.m_M41;	m_M42=map.m_M42;	m_M43=map.m_M43;	m_M44=map.m_M44;	
	return;
}
//-------------------------------------------------------------------------------------//	
inline void CMapCoordinate::ClearMap()
{
	m_M11=0;	m_M12=0;	m_M13=0;	m_M14=0;
	m_M21=0;	m_M22=0;	m_M23=0;	m_M24=0;
	m_M31=0;	m_M32=0;	m_M33=0;	m_M34=0;
	m_M41=0;	m_M42=0;	m_M43=0;	m_M44=0;
	return;
}
//-------------------------------------------------------------------------------------//	
inline void CMapCoordinate::SetMap2D(double M11, double M12, double M13, double M21, double M22, double M23, double M31, double M32, double M33)//將3x3-2D矩陣擴展成4x4-3D矩陣
{	
	m_M11=M11;	m_M12=M12;	m_M13=0;	m_M14=M13;
	m_M21=M21;	m_M22=M22;	m_M23=0;	m_M24=M23;
	m_M31=  0;	m_M32=  0;	m_M33=1;	m_M34=  0;
	m_M41=M31;	m_M42=M32;	m_M43=0;	m_M44=M33;
}
//-------------------------------------------------------------------------------------//	
inline void CMapCoordinate::SetMap3D(double M11, double M12, double M13, double M14, double M21, double M22, double M23, double M24, double M31, double M32, double M33, double M34, double M41, double M42, double M43, double M44)
{
	m_M11=M11;	m_M12=M12;	m_M13=M13;	m_M14=M14;
	m_M21=M21;	m_M22=M22;	m_M23=M23;	m_M24=M24;
	m_M31=M31;	m_M32=M32;	m_M33=M33;	m_M34=M34;
	m_M41=M41;	m_M42=M42;	m_M43=M43;	m_M44=M44;
}
//-------------------------------------------------------------------------------------//	
inline void CMapCoordinate::SignMatrix2D(bool SignX, bool SignY)
{
	if ( false == SignX )
	{
		m_M11 = -m_M11;
		m_M21 = -m_M21;
		m_M31 = -m_M31;
		m_M41 = -m_M41;
	}
	if ( false == SignY )
	{
		m_M12 = -m_M12;
		m_M22 = -m_M22;
		m_M32 = -m_M32;
		m_M42 = -m_M42;
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CMapCoordinate::Identity()//單位矩陣, 原進源出
{
	CMapCoordinate::InitialMap();
}
//-------------------------------------------------------------------------------------//	
bool CMapCoordinate::Map2D(double SrcX, double SrcY, double &DstX, double &DstY) const
{
	double x = (SrcX*m_M11) + (SrcY*m_M12) + m_M14;
	double y = (SrcX*m_M21) + (SrcY*m_M22) + m_M24;
	double w = (SrcX*m_M41) + (SrcY*m_M42) + m_M44;
	
	DstX = x/w;
	DstY = y/w;
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CMapCoordinate::MapPt2D(const TPOINT2D &SrcPt, TPOINT2D &DstPt) const
{
	double x = (SrcPt.x*m_M11) + (SrcPt.y*m_M12) + m_M14;
	double y = (SrcPt.x*m_M21) + (SrcPt.y*m_M22) + m_M24;
	double w = (SrcPt.x*m_M41) + (SrcPt.y*m_M42) + m_M44;
	
	DstPt.x = x/w;
	DstPt.y = y/w;
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CMapCoordinate::MapRect2D(const TRECT4D &SrcRect, TRECT4D &DstRect) const
{
	double LTx = (SrcRect.left*m_M11) + (SrcRect.top*m_M12) + m_M14;
	double LTy = (SrcRect.left*m_M21) + (SrcRect.top*m_M22) + m_M24;
	double LTw = (SrcRect.left*m_M41) + (SrcRect.top*m_M42) + m_M44;

	double RBx = (SrcRect.right*m_M11) + (SrcRect.bottom*m_M12) + m_M14;
	double RBy = (SrcRect.right*m_M21) + (SrcRect.bottom*m_M22) + m_M24;
	double RBw = (SrcRect.right*m_M41) + (SrcRect.bottom*m_M42) + m_M44;
	
	const double X1 = LTx/LTw;
	const double Y1 = LTy/LTw;
	const double X2 = RBx/RBw;
	const double Y2 = RBy/RBw;

	DstRect.left   = MIN(X1, X2);//LTx/LTw;
	DstRect.top    = MIN(Y1, Y2);//LTy/LTw;
	DstRect.right  = MAX(X1, X2);//RBx/RBw;
	DstRect.bottom = MAX(Y1, Y2);//RBy/RBw;
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CMapCoordinate::MapRegion2D(const TREGION4D &SrcRgn, TREGION4D &DstRgn) const
{
	double minX = (SrcRgn.minX*m_M11) + (SrcRgn.minY*m_M12) + m_M14;
	double minY = (SrcRgn.minX*m_M21) + (SrcRgn.minY*m_M22) + m_M24;
	double minW = (SrcRgn.minX*m_M41) + (SrcRgn.minY*m_M42) + m_M44;

	double maxX = (SrcRgn.maxX*m_M11) + (SrcRgn.maxY*m_M12) + m_M14;
	double maxY = (SrcRgn.maxX*m_M21) + (SrcRgn.maxY*m_M22) + m_M24;
	double maxW = (SrcRgn.maxX*m_M41) + (SrcRgn.maxY*m_M42) + m_M44;
	
	const double X1 = minX/minW;
	const double Y1 = minY/minW;
	const double X2 = maxX/maxW;
	const double Y2 = maxY/maxW;
	DstRgn.minX = MIN(X1, X2);//minX/minW;
	DstRgn.minY = MIN(Y1, Y2);//minY/minW;
	DstRgn.maxX = MAX(X1, X2);//maxX/maxW;
	DstRgn.maxY = MAX(Y1, Y2);//maxY/maxW;
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CMapCoordinate::Map3D(double SrcX, double SrcY, double SrcZ, double &DstX, double &DstY, double &DstZ) const
{
	double x = (SrcX*m_M11) + (SrcY*m_M12) + (SrcZ*m_M13) + m_M14;
	double y = (SrcX*m_M21) + (SrcY*m_M22) + (SrcZ*m_M23) + m_M24;
	double z = (SrcX*m_M31) + (SrcY*m_M32) + (SrcZ*m_M33) + m_M34;
	double w = (SrcX*m_M41) + (SrcY*m_M42) + (SrcZ*m_M43) + m_M44;
	
	DstX = x/w;
	DstY = y/w;
	DstZ = z/w;
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CMapCoordinate::CalcMatrix2D(double dX, double dY, double AngleDeg, double sX, double sY)//由偏移量與旋轉角度來計算轉換公式
{
	double M11=0, M12=0, M13=0;
	double M21=0, M22=0, M23=0;
	double M31=0, M32=0, M33=0;
	const double AngleRad = AngleDeg*3.1415926535897932384626433832795/180.0;
	const double dCOS = ::cos(AngleRad); 
	const double dSIN = ::sin(AngleRad); 
	CMapCoordinate::InitialMap();
	M11 = dCOS*sX; 
	M12 = dSIN*sY; 
	M21 =-dSIN*sX;	
	M22 = dCOS*sY;
	M13 = dX;
	M23 = dY;
	M33 = 1;
	CMapCoordinate::SetMap2D(M11, M12, M13, M21, M22, M23, M31, M32, M33);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMapCoordinate::CalcMatrix2D(double SrcX, double SrcY, double DstX, double DstY, double AngleDeg, double sX, double sY, bool SignX, bool SignY)
{
	double SrcX2=SrcX, SrcY2=SrcY;
	double DstX2=DstX, DstY2=DstY;	
	double M11=0, M12=0, M13=0;
	double M21=0, M22=0, M23=0;
	double M31=0, M32=0, M33=0;
	const double AngleRad = AngleDeg*3.1415926535897932384626433832795/180.0;
	const double dCOS = ::cos(AngleRad); 
	const double dSIN = ::sin(AngleRad); 	
	CMapCoordinate::InitialMap();
	
	M11 = dCOS*sX; 
	M12 = dSIN*sY; 
	M21 =-dSIN*sX;	
	M22 = dCOS*sY;

	if ( false == SignX )
	{	
		M11 = -M11;
		M21 = -M21;
		SrcX2 = -SrcX;
	}
	if ( false == SignY )
	{
		M12 = -M12;
		M22 = -M22;
		SrcY2 = -SrcY;
	}

	M13 = -(SrcX2*dCOS)-(SrcY2*dSIN)+DstX2;
	M23 =  (SrcX2*dSIN)-(SrcY2*dCOS)+DstY2;
	M33 = 1;
	CMapCoordinate::SetMap2D(M11, M12, M13, M21, M22, M23, M31, M32, M33);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMapCoordinate::CalcMatrix2D(double SrcX[], double SrcY[], double DstX[], double DstY[], size_t num)
{
	const bool SignX = AOIDataCollect.GetStageSignPositiveX();
	const bool SignY = AOIDataCollect.GetStageSignPositiveY();
	return CalcMatrix2D(SrcX, SrcY, DstX, DstY, num, SignX, SignY);
}
//-------------------------------------------------------------------------------------//
bool CMapCoordinate::CalcMatrix2D(double SrcX[], double SrcY[], double DstX[], double DstY[], size_t num, bool SignX, bool SignY)
{
	bool IsOK = false;	
	switch ( num )
	{
	case 4:	IsOK = CMapCoordinate::CalcMatrix4(SrcX, SrcY, DstX, DstY, SignX, SignY);	break;
	case 3:	IsOK = CMapCoordinate::CalcMatrix3(SrcX, SrcY, DstX, DstY, SignX, SignY);	break;
	case 2:	IsOK = CMapCoordinate::CalcMatrix2(SrcX, SrcY, DstX, DstY, SignX, SignY);	break;
	case 1:	IsOK = CMapCoordinate::CalcMatrix1(SrcX, SrcY, DstX, DstY, SignX, SignY);	break;
	case 0:	IsOK = CMapCoordinate::CalcMatrix0(SrcX, SrcY, DstX, DstY, SignX, SignY);	break;
	default:
		IsOK = false;
		CMapCoordinate::InitialMap();
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CMapCoordinate::CalcMatrix0(double SrcX[], double SrcY[], double DstX[], double DstY[], bool SignX, bool SignY)
{
	double M11=0, M12=0, M13=0;
	double M21=0, M22=0, M23=0;
	double M31=0, M32=0, M33=0;

	M11=1; M12=0; M13=0;
	M21=0; M22=1; M23=0;
	M31=0; M32=0; M33=1;
	CMapCoordinate::SetMap2D(M11, M12, M13, M21, M22, M23, M31, M32, M33);	
	CMapCoordinate::SignMatrix2D(SignX, SignY);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMapCoordinate::CalcMatrix1(double SrcX[], double SrcY[], double DstX[], double DstY[], bool SignX, bool SignY)
{
	double M11=0, M12=0, M13=0;
	double M21=0, M22=0, M23=0;
	double M31=0, M32=0, M33=0;
	double SrcX2[4]={0};
	double SrcY2[4]={0};
	double DstX2[4]={0};
	double DstY2[4]={0};	
	if ( true == SignX )
	{	
		SrcX2[0] = SrcX[0];
		DstX2[0] = DstX[0];
	}
	else
	{	
		SrcX2[0] = -SrcX[0];		
		DstX2[0] =  DstX[0];		
	}
	if ( true == SignY )
	{	
		SrcY2[0] = SrcY[0];		
		DstY2[0] = DstY[0];		
	}
	else
	{	
		SrcY2[0] = -SrcY[0];		
		DstY2[0] =  DstY[0];		
	}
	M11=1; M12=0; M13=DstX2[0]-SrcX2[0];
	M21=0; M22=1; M23=DstY2[0]-SrcY2[0];
	M31=0; M32=0; M33=1;

	CMapCoordinate::SetMap2D(M11, M12, M13, M21, M22, M23, M31, M32, M33);	
	CMapCoordinate::SignMatrix2D(SignX, SignY);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMapCoordinate::CalcMatrix2(double SrcX[], double SrcY[], double DstX[], double DstY[], bool SignX, bool SignY)
{
	int   i=0, idx=0;
	const int num = 2;
	const int num2 = num*2;
	double B[8]={0};	
	double SrcX2[4]={0};
	double SrcY2[4]={0};
	double DstX2[4]={0};
	double DstY2[4]={0};
	double Datas[64]={0};		
	double u=0, v=0, x=0, y=0;
	double M11=0, M12=0, M13=0;
	double M21=0, M22=0, M23=0;
	double M31=0, M32=0, M33=0;		
	CJetMatrix mat1, mat2;
	
	if ( true == SignX )
	{	
		SrcX2[0] = SrcX[0];		
		DstX2[0] = DstX[0];
		SrcX2[1] = SrcX[1];
		DstX2[1] = DstX[1];
	}
	else
	{	
		SrcX2[0] = -SrcX[0];		
		DstX2[0] =  DstX[0];
		SrcX2[1] = -SrcX[1];
		DstX2[1] =  DstX[1];
	}
	if ( true == SignY )
	{	
		SrcY2[0] = SrcY[0];		
		DstY2[0] = DstY[0];
		SrcY2[1] = SrcY[1];
		DstY2[1] = DstY[1];		
	}
	else
	{	
		SrcY2[0] = -SrcY[0];		
		DstY2[0] =  DstY[0];
		SrcY2[1] = -SrcY[1];
		DstY2[1] =  DstY[1];		
	}

	idx = 0;
	CMapCoordinate::Identity();
	for ( i=0; i<num; i++ )
	{
		x = DstX2[i];
		y = DstY2[i];
		u = SrcX2[i];
		v = SrcY2[i];

		Datas[idx++] =  u;
		Datas[idx++] =  v;
		Datas[idx++] =  1;
		Datas[idx++] =  0;
			
		Datas[idx++] =  v;
		Datas[idx++] = -u;
		Datas[idx++] =  0;
		Datas[idx++] =  1;
		
	}
	B[0] = DstX2[0];
	B[1] = DstY2[0];
	B[2] = DstX2[1];
	B[3] = DstY2[1];
	
	mat1.SetMatrixSize(num2, num2, Datas);
	mat2 = mat1.Inversion();	
	for ( i=0; i<num2; i++ )
	{
		M11 = M11 + mat2.GetElement(i, 0)*B[i];//X1
		M12 = M12 + mat2.GetElement(i, 1)*B[i];//Y1
		M13 = M13 + mat2.GetElement(i, 2)*B[i];//X2
		M23 = M23 + mat2.GetElement(i, 3)*B[i];//Y2
	}	
	M33 =  1;
	M31 =  M32 = 0;
	M21 = -M12;
	M22 =  M11;	
	
	CMapCoordinate::SetMap2D(M11, M12, M13, M21, M22, M23, M31, M32, M33);
	CMapCoordinate::SignMatrix2D(SignX, SignY);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMapCoordinate::CalcMatrix3(double SrcX[], double SrcY[], double DstX[], double DstY[], bool SignX, bool SignY)
{
	int   i=0, idx=0;
	const int num = 3;
	const int num2 = num*2;
	double B[8]={0};	
	double SrcX2[4]={0};
	double SrcY2[4]={0};
	double DstX2[4]={0};
	double DstY2[4]={0};
	double Datas[64]={0};	
	double u=0, v=0, x=0, y=0;
	double M11=0, M12=0, M13=0;
	double M21=0, M22=0, M23=0;
	double M31=0, M32=0, M33=0;	
	CJetMatrix mat1, mat2;
	
	if ( true == SignX )
	{	
		SrcX2[0] = SrcX[0];		
		DstX2[0] = DstX[0];
		SrcX2[1] = SrcX[1];
		DstX2[1] = DstX[1];
		SrcX2[2] = SrcX[2];
		DstX2[2] = DstX[2];
	}
	else
	{	
		SrcX2[0] = -SrcX[0];		
		DstX2[0] =  DstX[0];
		SrcX2[1] = -SrcX[1];
		DstX2[1] =  DstX[1];
		SrcX2[2] = -SrcX[2];
		DstX2[2] =  DstX[2];
	}
	if ( true == SignY )
	{	
		SrcY2[0] = SrcY[0];		
		DstY2[0] = DstY[0];
		SrcY2[1] = SrcY[1];
		DstY2[1] = DstY[1];		
		SrcY2[2] = SrcY[2];
		DstY2[2] = DstY[2];		
	}
	else
	{	
		SrcY2[0] = -SrcY[0];		
		DstY2[0] =  DstY[0];
		SrcY2[1] = -SrcY[1];
		DstY2[1] =  DstY[1];
		SrcY2[2] = -SrcY[2];
		DstY2[2] =  DstY[2];
	}

	idx=0;
	CMapCoordinate::Identity();
	for ( i=0; i<num; i++ )
	{
		x = DstX2[i];
		y = DstY2[i];
		u = SrcX2[i];
		v = SrcY2[i];	
			
		Datas[idx++] = u;
		Datas[idx++] = v;
		Datas[idx++] = 1;
		Datas[idx++] = 0;
		Datas[idx++] = 0;
		Datas[idx++] = 0;

		Datas[idx++] = 0;
		Datas[idx++] = 0;
		Datas[idx++] = 0;
		Datas[idx++] = u;
		Datas[idx++] = v;
		Datas[idx++] = 1;
	}
	B[0] = DstX2[0];
	B[1] = DstY2[0];
	B[2] = DstX2[1];
	B[3] = DstY2[1];
	B[4] = DstX2[2];
	B[5] = DstY2[2];

	mat1.SetMatrixSize(num2, num2, Datas);
	mat2 = mat1.Inversion();	
	for ( i=0; i<num2; i++ )
	{
		M11 = M11 + mat2.GetElement(i, 0)*B[i];//X1
		M12 = M12 + mat2.GetElement(i, 1)*B[i];//Y1
		M13 = M13 + mat2.GetElement(i, 2)*B[i];//X2
		M21 = M21 + mat2.GetElement(i, 3)*B[i];//Y2
		M22 = M22 + mat2.GetElement(i, 4)*B[i];//X3
		M23 = M23 + mat2.GetElement(i, 5)*B[i];//Y3
	}	
	M33 = 1;
	M31 = 0;
	M32 = 0;	
	
	CMapCoordinate::SetMap2D(M11, M12, M13, M21, M22, M23, M31, M32, M33);
	CMapCoordinate::SignMatrix2D(SignX, SignY);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMapCoordinate::CalcMatrix4(double SrcX[], double SrcY[], double DstX[], double DstY[], bool SignX, bool SignY)
{
	int   i=0, idx=0;
	const int num = 4;
	const int num2 = num*2;
	double B[8]={0};	
	double SrcX2[4]={0};
	double SrcY2[4]={0};
	double DstX2[4]={0};
	double DstY2[4]={0};
	double Datas[64]={0};	
	double u=0, v=0, x=0, y=0;
	double M11=0, M12=0, M13=0;
	double M21=0, M22=0, M23=0;
	double M31=0, M32=0, M33=0;	
	CJetMatrix mat1, mat2;
	
	if ( true == SignX )
	{	
		SrcX2[0] = SrcX[0];		
		DstX2[0] = DstX[0];
		SrcX2[1] = SrcX[1];
		DstX2[1] = DstX[1];
		SrcX2[2] = SrcX[2];
		DstX2[2] = DstX[2];
		SrcX2[3] = SrcX[3];
		DstX2[3] = DstX[3];
	}
	else
	{	
		SrcX2[0] = -SrcX[0];		
		DstX2[0] =  DstX[0];
		SrcX2[1] = -SrcX[1];
		DstX2[1] =  DstX[1];
		SrcX2[2] = -SrcX[2];
		DstX2[2] =  DstX[2];
		SrcX2[3] = -SrcX[3];
		DstX2[3] =  DstX[3];
	}
	if ( true == SignY )
	{	
		SrcY2[0] = SrcY[0];		
		DstY2[0] = DstY[0];
		SrcY2[1] = SrcY[1];
		DstY2[1] = DstY[1];		
		SrcY2[2] = SrcY[2];
		DstY2[2] = DstY[2];		
		SrcY2[3] = SrcY[3];
		DstY2[3] = DstY[3];		
	}
	else
	{	
		SrcY2[0] = -SrcY[0];		
		DstY2[0] =  DstY[0];
		SrcY2[1] = -SrcY[1];
		DstY2[1] =  DstY[1];
		SrcY2[2] = -SrcY[2];
		DstY2[2] =  DstY[2];
		SrcY2[3] = -SrcY[3];
		DstY2[3] =  DstY[3];
	}

	idx=0;
	CMapCoordinate::Identity();
	for ( i=0; i<num; i++ )
	{
		x = DstX2[i];
		y = DstY2[i];
		u = SrcX2[i];
		v = SrcY2[i];
			
		Datas[idx++] = u;
		Datas[idx++] = v;
		Datas[idx++] = 1;
		Datas[idx++] = 0;
		Datas[idx++] = 0;
		Datas[idx++] = 0;
		Datas[idx++] = -x*u;
		Datas[idx++] = -x*v;

		Datas[idx++] = 0;
		Datas[idx++] = 0;
		Datas[idx++] = 0;
		Datas[idx++] = u;
		Datas[idx++] = v;
		Datas[idx++] = 1;
		Datas[idx++] = -y*u;
		Datas[idx++] = -y*v;
	}
	B[0] = DstX2[0];
	B[1] = DstY2[0];
	B[2] = DstX2[1];
	B[3] = DstY2[1];
	B[4] = DstX2[2];
	B[5] = DstY2[2];
	B[6] = DstX2[3];
	B[7] = DstY2[3];

	mat1.SetMatrixSize(num2, num2, Datas);
	mat2 = mat1.Inversion();	
	for ( i=0; i<num2; i++ )
	{
		M11 = M11 + mat2.GetElement(i, 0)*B[i];//X1
		M12 = M12 + mat2.GetElement(i, 1)*B[i];//Y1
		M13 = M13 + mat2.GetElement(i, 2)*B[i];//X2
		M21 = M21 + mat2.GetElement(i, 3)*B[i];//Y2
		M22 = M22 + mat2.GetElement(i, 4)*B[i];//X3
		M23 = M23 + mat2.GetElement(i, 5)*B[i];//Y3
		M31 = M31 + mat2.GetElement(i, 6)*B[i];//X4
		M32 = M32 + mat2.GetElement(i, 7)*B[i];//Y4
	}	
	M33 = 1;
	
	CMapCoordinate::SetMap2D(M11, M12, M13, M21, M22, M23, M31, M32, M33);
	CMapCoordinate::SignMatrix2D(SignX, SignY);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMapCoordinate::GetMatrix2D(double &M11, double &M12, double &M13, double &M21, double &M22, double &M23, double &M31, double &M32, double &M33) const
{
	M11 = m_M11;	M12 = m_M12;	M13 = m_M14;
	M21 = m_M21;	M22 = m_M22;	M23 = m_M24;
	M31 = m_M41;	M32 = m_M42;	M33 = m_M44;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMapCoordinate::GetMatrix3D(double &M11, double &M12, double &M13, double &M14, double &M21, double &M22, double &M23, double &M24, double &M31, double &M32, double &M33, double &M34, double &M41, double &M42, double &M43, double &M44) const
{
	M11 = m_M11;	M12 = m_M12;	M13 = m_M13;	M14 = m_M14;
	M21 = m_M21;	M22 = m_M22;	M23 = m_M23;	M24 = m_M24;
	M31 = m_M31;	M32 = m_M32;	M33 = m_M33;	M34 = m_M34;
	M41 = m_M41;	M42 = m_M42;	M43 = m_M43;	M44 = m_M44;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMapCoordinate::MoveMatrix2D(double dx, double dy)//移動矩陣
{
	m_M14 += dx;
	m_M24 += dy;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMapCoordinate::MoveMatrix3D(double dx, double dy, double dz)//移動矩陣
{
	m_M14 += dx;
	m_M24 += dy;
	m_M34 += dz;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CMapCoordinate::CopyMatrix(const CMapCoordinate &Map, int CopyMode)//複製矩陣	
{	
	DWORD Res=0;

	Res = CopyMode&COORDINATE_MATRIX_TRANSLATION;
	if ( 0 != Res )
	{	
		double dM14 = Map.m_M14-m_M14;
		double dM24 = Map.m_M24-m_M24;
		double dM34 = Map.m_M34-m_M34;

		m_M14 = Map.m_M14;	
		m_M24 = Map.m_M24;	
		m_M34 = Map.m_M34;	
		dM34 = dM34;
	}

	Res = CopyMode&COORDINATE_MATRIX_ROTATION;
	if ( 0 != Res )
	{
		m_M11 = Map.m_M11;	m_M12 = Map.m_M12;	m_M13 = Map.m_M13;
		m_M21 = Map.m_M21;	m_M22 = Map.m_M22;	m_M23 = Map.m_M23;
		m_M31 = Map.m_M31;	m_M32 = Map.m_M32;	m_M33 = Map.m_M33;
	}
	
	Res = CopyMode&COORDINATE_MATRIX_SCALE;
	if ( 0 != Res )
	{	m_M44 = Map.m_M44;	}

	Res = CopyMode&COORDINATE_MATRIX_SKEW;
	if ( 0 != Res )
	{	m_M41 = Map.m_M41;	m_M42 = Map.m_M42;	m_M43 = Map.m_M43;	}
	return true;
}
//-------------------------------------------------------------------------------------//