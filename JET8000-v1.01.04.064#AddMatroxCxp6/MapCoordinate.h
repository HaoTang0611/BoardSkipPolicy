// MapCoordinate.h: interface for the CMapCoordinate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPCOORDINATE_H__13CFDA8F_D7AC_4AF7_9C99_E27AEB4360E6__INCLUDED_)
#define AFX_MAPCOORDINATE_H__13CFDA8F_D7AC_4AF7_9C99_E27AEB4360E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#define COORDINATE_MATRIX_TRANSLATION          0x01//移動矩陣
#define COORDINATE_MATRIX_ROTATION             0x02//旋轉矩陣
#define COORDINATE_MATRIX_SCALE                0x04//縮放矩陣
#define COORDINATE_MATRIX_SKEW                 0x08//偏移矩陣
#define COORDINATE_MATRIX_ALL                  0xFF//全部
//-------------------------------------------------------------------------------------//
class CMapCoordinate  
{
private:
	//---------------------------------------------------------------------------------//	
	double                     m_M11, m_M12, m_M13, m_M14;
	double                     m_M21, m_M22, m_M23, m_M24;
	double                     m_M31, m_M32, m_M33, m_M34;
	double                     m_M41, m_M42, m_M43, m_M44;
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitMap();
	void                       InitialMap();
	void                       CloneMap(const CMapCoordinate &map);
	//---------------------------------------------------------------------------------//	
	void                       ClearMap();
	//將3x3-2D矩陣擴展成4x4-3D矩陣
	void                       SetMap2D(double M11, double M12, double M13, double M21, double M22, double M23, double M31, double M32, double M33);
	void                       SetMap3D(double M11, double M12, double M13, double M14, double M21, double M22, double M23, double M24, double M31, double M32, double M33, double M34, double M41, double M42, double M43, double M44);
	//---------------------------------------------------------------------------------//	
	void                       SignMatrix2D(bool SignX, bool SignY);
	//---------------------------------------------------------------------------------//
	bool                       CalcMatrix0(double SrcX[], double SrcY[], double DstX[], double DstY[], bool SignX, bool SignY);	
	bool                       CalcMatrix1(double SrcX[], double SrcY[], double DstX[], double DstY[], bool SignX, bool SignY);	
	bool                       CalcMatrix2(double SrcX[], double SrcY[], double DstX[], double DstY[], bool SignX, bool SignY);	
	bool                       CalcMatrix3(double SrcX[], double SrcY[], double DstX[], double DstY[], bool SignX, bool SignY);	
	bool                       CalcMatrix4(double SrcX[], double SrcY[], double DstX[], double DstY[], bool SignX, bool SignY);	
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	CMapCoordinate();
	CMapCoordinate(const CMapCoordinate &map);
	virtual ~CMapCoordinate();
	//---------------------------------------------------------------------------------//	
	CMapCoordinate& operator=(const CMapCoordinate &map);
	//---------------------------------------------------------------------------------//	
	void                      Identity();//單位矩陣, 原進原出 
	//---------------------------------------------------------------------------------//		
	bool                      Map2D(double SrcX, double SrcY, double &DstX, double &DstY) const;
	bool                      MapPt2D(const TPOINT2D &SrcPt, TPOINT2D &DstPt) const;
	bool                      MapRect2D(const TRECT4D &SrcRect, TRECT4D &DstRect) const;
	bool                      MapRegion2D(const TREGION4D &SrcRgn, TREGION4D &DstRgn) const;
	bool                      Map3D(double SrcX, double SrcY, double SrcZ, double &DstX, double &DstY, double &DstZ) const;
	//---------------------------------------------------------------------------------//	
	//計算轉換矩陣
	bool                      CalcMatrix2D(double dX, double dY, double AngleDeg, double sX, double sY);//由偏移量與旋轉角度來計算轉換公式
	bool                      CalcMatrix2D(double SrcX[], double SrcY[], double DstX[], double DstY[], size_t num);
	bool                      CalcMatrix2D(double SrcX[], double SrcY[], double DstX[], double DstY[], size_t num, bool SignX, bool SignY);
	bool                      CalcMatrix2D(double SrcX, double SrcY, double DstX, double DstY, double AngleDeg, double sX, double sY, bool SignX, bool SignY);
	//---------------------------------------------------------------------------------//
	bool                      GetMatrix2D(double &M11, double &M12, double &M13, double &M21, double &M22, double &M23, double &M31, double &M32, double &M33) const;
	bool                      GetMatrix3D(double &M11, double &M12, double &M13, double &M14, double &M21, double &M22, double &M23, double &M24, double &M31, double &M32, double &M33, double &M34, double &M41, double &M42, double &M43, double &M44) const;
	//---------------------------------------------------------------------------------//
	bool                      MoveMatrix2D(double dx, double dy);//移動矩陣
	bool                      MoveMatrix3D(double dx, double dy, double dz);//移動矩陣
	//---------------------------------------------------------------------------------//
	bool                      CopyMatrix(const CMapCoordinate &Map, int CopyMode);//複製矩陣	
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_MAPCOORDINATE_H__13CFDA8F_D7AC_4AF7_9C99_E27AEB4360E6__INCLUDED_)
