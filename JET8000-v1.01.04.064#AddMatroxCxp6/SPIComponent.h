// SPIComponent.h: interface for the CSPIComponent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPICOMPONENT_H__2F248601_D2CE_4C0B_A6DB_0E36014B4E46__INCLUDED_)
#define AFX_SPICOMPONENT_H__2F248601_D2CE_4C0B_A6DB_0E36014B4E46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "SPIPad.h"
//-------------------------------------------------------------------------------------//
class CSPIComponent  
{
private:
	//---------------------------------------------------------------------------------//
	int                        m_BoardID;//單板編號
	std::wstring               m_ComponentName;//零件名稱
	std::wstring               m_PackageName;//封裝名稱
	double                     m_ComponentAngle;//零件角度
	//---------------------------------------------------------------------------------//
	TPOINT2D                   m_ComponentPos;
	TPOINT2D                   m_ComponentSpecialPos; // 韓華 HASI
	TSIZE2D                    m_ComponentSize;
	BOX_TOWARD                 m_ComponentToward;
	//---------------------------------------------------------------------------------//
	std::vector<CSPIPad>       m_SpiPadList;
	//---------------------------------------------------------------------------------//	
	int                        m_TempInt;
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitSpiComponent();
	void                       InitialSpiComponent();	
	void                       CloneSpiComponent(const CSPIComponent &other);
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	CSPIComponent();	
	virtual ~CSPIComponent();
	CSPIComponent(const CSPIComponent &other);
	CSPIComponent& operator=(const CSPIComponent &other);
	//---------------------------------------------------------------------------------//		
	int                        GetBoardID() const;
	void                       SetBoardID(int val);
	//---------------------------------------------------------------------------------//
	const wchar_t*             GetComponentName() const;
	void                       SetComponentName(const wchar_t *val);
	//---------------------------------------------------------------------------------//	
	const wchar_t*             GetPackageName() const;
	void                       SetPackageName(const wchar_t *val);
	//---------------------------------------------------------------------------------//
	const TPOINT2D&            GetComponentPos() const;
	void                       SetComponentPos(double x, double y);
	void                       SetComponentPos(const TPOINT2D &val);
	//---------------------------------------------------------------------------------//
	const TPOINT2D&            GetComponentSpecialPos() const;
	void                       SetComponentSpecialPos(double x, double y);
	void                       SetComponentSpecialPos(const TPOINT2D &val);
	//---------------------------------------------------------------------------------//
	const TSIZE2D&             GetComponentSize() const;
	void                       SetComponentSize(double cx, double cy);
	void                       SetComponentSize(const TSIZE2D &val);
	//---------------------------------------------------------------------------------//		
	void                       SetComponentRegion(const TREGION4D &Region);	
	void                       CheckComponentRegion(TREGION4D &Region) const;	
	void                       GetComponentCornerPos(TPOINT2D Pos[]) const;
	//---------------------------------------------------------------------------------//	
	BOX_TOWARD                 GetComponentToward() const;
	void                       SetComponentToward(BOX_TOWARD val);
	//---------------------------------------------------------------------------------//	
	double                     GetComponentAngle() const;
	void                       SetComponentAngle(double val);	
	bool                       RotateComponent(double Angle);
	bool                       MoveComponentPos(double x, double y);
	//---------------------------------------------------------------------------------//
	int                        GetTempInt() const;
	void                       SetTempInt(int val);	
	//---------------------------------------------------------------------------------//	
	void                       ClearSpiPadList();
	size_t                     GetSpiPadCount() const;
	void                       AddSpiPad(const CSPIPad &SpiPad);
	CSPIPad*                   GetSpiPadPtr(size_t idx, bool bCheck);
	bool                       AddSpiPadList(const std::vector<CSPIPad> &List);
	bool                       SortSpiPadList(bool SortByPosX, std::vector<CSPIPad> &SpiPadList);
	bool                       SortSpiPadList(BOX_TOWARD Toward, bool bCWMode, std::vector<CSPIPad> &SpiPadList);
	bool                       GetSpiPadListByToward(BOX_TOWARD Toward, std::vector<CSPIPad> &SpiPadList) const;	
	bool                       AddSpiPadListToAll(const std::vector<CSPIPad> &SpiPadList, std::vector<CSPIPad> &SpiPadListAll) const;
	bool                       AddSpiPadAverageToList(const std::vector<CSPIPad> &SpiPadListAll, std::vector<CSPIPad> &SpiPadList) const;
	//---------------------------------------------------------------------------------//		
	bool                       AnalysisSpiPadConfig();	
	bool                       AnalysisComponentRegion(TREGION4D &Region);
	double                     Calc2PointsAngle(const TPOINT2D &pt, const TPOINT2D &cp);
	double                     Calc2PointsDistance(const TPOINT2D &pt, const TPOINT2D &cp);
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_SPICOMPONENT_H__2F248601_D2CE_4C0B_A6DB_0E36014B4E46__INCLUDED_)
