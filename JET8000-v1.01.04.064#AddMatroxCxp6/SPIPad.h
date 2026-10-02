// SPIPad.h: interface for the CSPIPad class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPIPAD_H__0EB44C40_A4A3_434B_B020_F9F492EC8C24__INCLUDED_)
#define AFX_SPIPAD_H__0EB44C40_A4A3_434B_B020_F9F492EC8C24__INCLUDED_
//-------------------------------------------------------------------------------------//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
class CAOIFileIO;
class CSPIPointInfo;
//-------------------------------------------------------------------------------------//
class CSPIPad  
{
private:
	//---------------------------------------------------------------------------------//
	double                     m_PidFileUnitScale;//Pad檔案單位比例
	//---------------------------------------------------------------------------------//
	std::wstring               m_ComponentName;//零件名稱
	std::wstring               m_PinIDText;//Pin編碼
	int                        m_BoardID;//單板編號
	std::wstring               m_PackageName;//封裝名稱
	TPOINT2D                   m_PadPos1;//Pad座標-1
	TPOINT2D                   m_PadPos2;//Pad座標-2
	double                     m_ComponentAngle;//零件角度
	TPOINT2D                   m_SpecialCadPos;//特殊座標 用於HASI
	//---------------------------------------------------------------------------------//
	double                     m_PadArea;//Pad面積
	std::wstring               m_PadType;//Pad樣式
	int                        m_PadSN;//Pad序號
	int                        m_PadGroupID;//Pad群組編號
	int                        m_PadDrawType;//Pad繪圖模式
	BOX_TOWARD                 m_PadToward;//Pad朝向
	double                     m_PadIncludedAngle;//Pad夾角	
	//---------------------------------------------------------------------------------//
	int                        m_TempInt;
	double                     m_TempDlb;
	//---------------------------------------------------------------------------------//	
	std::vector<CSPIPad>       m_SubSpiPadList;
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitSpiPad();
	void                       InitialSpiPad();	
	void                       CloneSpiPad(const CSPIPad &other);
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	CSPIPad();	
	virtual ~CSPIPad();
	CSPIPad(const CSPIPad &other);
	CSPIPad& operator=(const CSPIPad &other);
	//---------------------------------------------------------------------------------//	
	void                       ResetSpiPad();
	//---------------------------------------------------------------------------------//	
	double                     GetPidFileUnitScale() const;
	void                       SetPidFileUnitScale(double val);
	//---------------------------------------------------------------------------------//
	bool                       ReadSpiPadFile(CAOIFileIO &FileIO);
	bool                       ReadSpiPointInfoFile(CAOIFileIO &FileIO, CSPIPointInfo &PointInfo);
	//---------------------------------------------------------------------------------//	
	const wchar_t*             GetComponentName() const;
	void                       SetComponentName(const wchar_t *val);
	//---------------------------------------------------------------------------------//
	const wchar_t*             GetPinIDText() const;
	void                       SetPinIDText(const wchar_t *val);
	//---------------------------------------------------------------------------------//
	int                        GetBoardID() const;
	void                       SetBoardID(int val);
	//---------------------------------------------------------------------------------//
	const wchar_t*             GetPackageName() const;
	void                       SetPackageName(const wchar_t *val);
	//---------------------------------------------------------------------------------//	
	const TPOINT2D&            GetPadPos1() const;
	void                       SetPadPosX1(double val);
	void                       SetPadPosY1(double val);
	void                       SetPadPos1(double x, double y);	
	void                       SetPadPos1(const TPOINT2D &val);	
	//---------------------------------------------------------------------------------//	
	const TPOINT2D&            GetPadPos2() const;
	void                       SetPadPosX2(double val);
	void                       SetPadPosY2(double val);
	void                       SetPadPos2(double x, double y);	
	void                       SetPadPos2(const TPOINT2D &val);	
	//---------------------------------------------------------------------------------//	
	const TPOINT2D&            GetSpecialCadPos() const;
	void                       SetSpecialCadPosX(double val);
	void                       SetSpecialCadPosY(double val);
	//---------------------------------------------------------------------------------//
	void                       CheckPadPos(TPOINT2D &pos) const;	
	void                       SetPadRegion(const TREGION4D &Region);
	void                       CheckPadRegion(TREGION4D &Region) const;	
	void                       GetPadCornerPos(TPOINT2D pos[]) const;	
	//---------------------------------------------------------------------------------//	
	double                     GetComponentAngle() const;
	void                       SetComponentAngle(double val);
	//---------------------------------------------------------------------------------//
	double                     GetPadArea() const;
	void                       SetPadArea(double val);
	//---------------------------------------------------------------------------------//	
	const wchar_t*             GetPadType() const;
	void                       SetPadType(const wchar_t *val);
	//---------------------------------------------------------------------------------//		
	int                        GetPadSN() const;
	void                       SetPadSN(int val);
	//---------------------------------------------------------------------------------//	
	int                        GetPadGroupID() const;
	void                       SetPadGroupID(int val);
	//---------------------------------------------------------------------------------//	
	int                        GetPadDrawType() const;
	void                       SetPadDrawType(int val);
	//---------------------------------------------------------------------------------//	
	BOX_TOWARD                 GetPadToward() const;
	void                       SetPadToward(BOX_TOWARD val);
	//---------------------------------------------------------------------------------//		
	double                     GetPadIncludedAngle() const;
	void                       SetPadIncludedAngle(double val);
	void                       MovePadPos(double x, double y);
	void                       RotatePadAngle(double Angle, const TPOINT2D &cp);	
	//---------------------------------------------------------------------------------//			
	int                        GetTempInt() const;
	void                       SetTempInt(int val);
	//---------------------------------------------------------------------------------//	
	double                     GetTempDlb() const;
	void                       SetTempDlb(double val);
	//---------------------------------------------------------------------------------//
	void                       ClearSubSpiPadList();
	void                       AddSubSpiPad(const CSPIPad &SpiPad);	
	size_t                     GetSubSpiPadCount() const;
	CSPIPad*                   GetSubSpiPadPtr(size_t idx, bool bCheck);	
	bool                       CheckSubSpiPadAccept(const CSPIPad &SpiPad) const;	
	bool                       CalcAverageSpiPad(CSPIPad &SpiPad) const;
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_SPIPAD_H__0EB44C40_A4A3_434B_B020_F9F492EC8C24__INCLUDED_)
