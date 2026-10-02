// JetFieldDivider.h: interface for the CJetFieldDivider class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JETFIELDDIVIDER_H__55E51800_56F7_468C_B5F3_49310A5D59F8__INCLUDED_)
#define AFX_JETFIELDDIVIDER_H__55E51800_56F7_468C_B5F3_49310A5D59F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include <vector>
//-------------------------------------------------------------------------------------//
typedef struct _JetRgn
{
	double minX;
	double minY;
	double maxX;
	double maxY;

	double priority1;
	double priority2;

	bool   used;
	void   *Ptr;
	_JetRgn()
	{
		minX = 0.0;
		minY = 0.0;
		maxX = 0.0;
		maxY = 0.0;
		priority1 = 0.0;
		priority2 = 0.0;
		
		used = false;
		Ptr  = NULL;
	}

} TJetRgn, *PJetRgn;

typedef std::vector<TJetRgn> TJetRgnList;
//-------------------------------------------------------------------------------------//
typedef struct _JetField
{
	size_t  FieldID;//編號	
	size_t  UserIndex;//使用者定義次序
	double  minX;
	double  minY;
	double  maxX;
	double  maxY;
	double  priority1;
	double  priority2;

	bool    selected;
	bool    used;
	bool    visible;

	double  subMinX;
	double  subMinY;
	double  subMaxX;
	double  subMaxY;

	double  PosCadX;
	double  PosCadY;
	double  PosStageZ;
	
	void   *Ptr;
	void   *Ptr2;
	std::vector<size_t>  RgnIdxList;
	_JetField()
	{
		FieldID = -1;//編號	
		UserIndex = -1;
		minX = 0.0;
		minY = 0.0;
		maxX = 0.0;
		maxY = 0.0;
		priority1 = 0.0;
		priority2 = 0.0;

		selected = false;
		used = false;
		visible = false;

		subMinX = 0.0;
		subMinY = 0.0;
		subMaxX = 0.0;
		subMaxY = 0.0;

		PosCadX = 0.0;
		PosCadY = 0.0;
		PosStageZ= 0.0;

		Ptr     = NULL;
		Ptr2    = NULL;
	}
} TJetField, *PJetField;
typedef std::vector<TJetField> TJetFieldList;
//-------------------------------------------------------------------------------------//
class CJetFieldDivider  
{
private:
	//---------------------------------------------------------------------------------//
	double                     m_FieldMaxW;
	double                     m_FieldMaxH;
	TJetRgnList                m_RgnList;                   
	TJetFieldList              m_FieldList;
	FIELD_DIVISION_MODE        m_DivisionMode;
	//---------------------------------------------------------------------------------//
	FIELD_PATH_MODE            m_PathMode;
	TJetFieldList              m_PathList;
	double                     m_SectionFactor;
	double                     m_PathStartPosX;
	double                     m_PathStartPosY;
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitDivider();
	void                       InitialDivider();
	//---------------------------------------------------------------------------------//
	bool                       ExecDivide_MassArea(TJetRgnList &RgnList, TJetFieldList &FieldList);//執行分割-最大面積
	bool                       ExecDivide_DiagonalLine(TJetRgnList &RgnList, TJetFieldList &FieldList);//執行分割-對角線
	bool                       ExecDivide_HorizontalLine(TJetRgnList &RgnList, TJetFieldList &FieldList);//執行分割-水平線
	bool                       ExecDivide_VerticalLine(TJetRgnList &RgnList, TJetFieldList &FieldList);//執行分割-垂直線
	//---------------------------------------------------------------------------------//
	bool                       BuildPriority_MassArea(TJetRgnList &RgnList);//建立區域的優先順序-面積
	bool                       BuildPriority_DiagonalLine(TJetRgnList &RgnList);//建立區域的優先順序-對角線
	bool                       BuildPriority_HorizontalLine(TJetRgnList &RgnList);//建立區域的優先順序-水平線
	bool                       BuildPriority_VerticalLine(TJetRgnList &RgnList);//建立區域的優先順序-垂直線
	//---------------------------------------------------------------------------------//
	bool                       ExecFieldDivision(TJetRgnList &RgnList, TJetFieldList &FieldList);//Field分割
	//---------------------------------------------------------------------------------//
	bool                       AdjustFieldSubRegion(const TJetRgn &Rgn, TJetField &Field);//調整區域內的範圍
	bool                       CheckRgnCombineField(TJetRgn &Rgn, double minX, double maxX, double minY, double maxY, double FieldMaxW, double FieldMaxH);//確認該區域是否可以合併
	//---------------------------------------------------------------------------------//
	bool                       ExecFieldPath_Hor(const TJetFieldList &FieldList, TJetFieldList &PathList);//執行路徑分配-水平為主
	bool                       ExecFieldPath_Ver(const TJetFieldList &FieldList, TJetFieldList &PathList);//執行路徑分配-垂直為主
	bool                       ExecFieldPath_User(const TJetFieldList &FieldList, TJetFieldList &PathList);//執行路徑分配-手動設定
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//
	CJetFieldDivider();
	virtual ~CJetFieldDivider();
	//---------------------------------------------------------------------------------//
	void                       ResetDivider();
	//---------------------------------------------------------------------------------//
	void                       SetFieldMaxW(double value) { m_FieldMaxW = value; }
	double                     GetFieldMaxW() const { return m_FieldMaxW; }
	//---------------------------------------------------------------------------------//
	void                       SetFieldMaxH(double value) { m_FieldMaxH = value; }
	double                     GetFieldMaxH() const { return m_FieldMaxH; }
	//---------------------------------------------------------------------------------//		
	void                       SetDivisionMode(FIELD_DIVISION_MODE value) { m_DivisionMode = value; }
	FIELD_DIVISION_MODE        GetDivisionMode() const { return m_DivisionMode; }
	//---------------------------------------------------------------------------------//	
	void                       SetPathMode(FIELD_PATH_MODE value) { m_PathMode = value; }
	FIELD_PATH_MODE            GetPathMode() const { return m_PathMode; }
	//---------------------------------------------------------------------------------//
	void                       SetSectionFactor(double value) { m_SectionFactor = value; }
	double                     GetSectionFactor() const { return m_SectionFactor; }
	//---------------------------------------------------------------------------------//
	void                       SetPathStartPosX(double value) { m_PathStartPosX = value; }
	double                     GetPathStartPosX() const { return m_PathStartPosX; }
	//---------------------------------------------------------------------------------//
	void                       SetPathStartPosY(double value) { m_PathStartPosY = value; }
	double                     GetPathStartPosY() const { return m_PathStartPosY; }
	//---------------------------------------------------------------------------------//	
	size_t                     GetJetRgnCount() const;
	void                       AddJetRgn(TJetRgn &Rgn);
	TJetRgn*                   GetJetRgnPtr(size_t idx, bool check);
	void                       CloneJetRgnList(TJetRgnList &RgnList);
	//---------------------------------------------------------------------------------//
	size_t                     GetJetFieldCount() const;
	TJetField*                 GetJetFieldPtr(size_t idx, bool check);
	void                       CloneJetFieldList(TJetFieldList &FieldList);
	//---------------------------------------------------------------------------------//
	size_t                     GetJetPathCount() const;
	TJetField*                 GetJetPathPtr(size_t idx, bool check);
	void                       CloneJetPathList(TJetFieldList &PathList);
	//---------------------------------------------------------------------------------//
	bool                       ExecDivide();//執行分割
	bool                       ExecFieldPath(const TJetFieldList &FieldList);//執行路徑分配
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_JETFIELDDIVIDER_H__55E51800_56F7_468C_B5F3_49310A5D59F8__INCLUDED_)
