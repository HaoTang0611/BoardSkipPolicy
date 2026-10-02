#ifndef _JET_GROUND_EQUATION_H_
#define _JET_GROUND_EQUATION_H_
//-------------------------------------------------------------------------------------//
#include <vector>
//-------------------------------------------------------------------------------------//
#define GROUND_EQUATION_PARAM_COUNT      16
//-------------------------------------------------------------------------------------//
enum GROUND_EQUATION_MODE
{
	GROUND_EQUATION_NONE    = 0,//膀非よ祘Α-ゼ璸衡
	GROUND_EQUATION_PLANE   = 1,//膀非よ祘Α-キ
	GROUND_EQUATION_CURVE   = 2,//膀非よ祘Α-Ρ
};
//-------------------------------------------------------------------------------------//
//┏(膀非)よ祘Α
class CJetGroundEquation
{
private:
	//---------------------------------------------------------------------------------//	
	CString                    m_ErrorString;
	bool                       m_UseMatrixFunc;//ㄏノ痻皚ㄧΑ
	GROUND_EQUATION_MODE       m_GroundEquationMode;
	double                     m_T[GROUND_EQUATION_PARAM_COUNT];//把计
	//---------------------------------------------------------------------------------//	
	double                     t_X, t_Y, t_Z;
	double                     t_XY, t_XZ, t_YZ;
	double                     t_XX, t_YY, t_ZZ;
	//---------------------------------------------------------------------------------//	
	std::vector<double>        t_MatrixElem1;
	std::vector<double>        t_MatrixElem2;		
	//---------------------------------------------------------------------------------//	

protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInit();
	void                       Initial();
	void                       Clone(const CJetGroundEquation &rhs);
	//---------------------------------------------------------------------------------//	

public:
	//---------------------------------------------------------------------------------//	
	CJetGroundEquation();
	CJetGroundEquation(const CJetGroundEquation &rhs);
	~CJetGroundEquation();
	//---------------------------------------------------------------------------------//	
	CJetGroundEquation& operator=(const CJetGroundEquation &rhs);
	//---------------------------------------------------------------------------------//		
	LPCTSTR                    GetErrorString() const;
	//---------------------------------------------------------------------------------//	
	double*                    GetGroundParamList();
	const double*              GetGroundParamList() const;
	int                        GetGroundParamCount() const;		
	//---------------------------------------------------------------------------------//	
	GROUND_EQUATION_MODE       GetGroundEquationMode() const;
	void                       SetGroundEquationMode(GROUND_EQUATION_MODE val);
	//---------------------------------------------------------------------------------//	
	bool                       VerticalPlane();//キ	
	//---------------------------------------------------------------------------------//
	void                       ClearGroundParam();//睲埃膀非把计
	void                       InitGroundParam(GROUND_EQUATION_MODE Mode);//﹍て膀非把计
	bool                       AddGroundValue(double x, double y, double z);//膀非计
	bool                       CalcGroundParam();//璸衡膀非把计	
	bool                       CalcLocalGroundParam(const RECT &Rect, GROUND_EQUATION_MODE LocalMode, CJetGroundEquation &LocalEquationParam) const;//璸衡Ы场膀非把计
	//---------------------------------------------------------------------------------//
	bool                       CheckGroundParamValid() const;//絋粄膀非把计Τ
	double                     CalcGroundValue(double x, double y) const;//璸衡膀非计	
	void                       SetGroundNormal(double nX, double nY, double nZ);//砞﹚膀非猭秖
	void                       GetGroundNormal(double &nX, double &nY, double &nZ) const;//眔膀非猭秖	
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif _JET_GROUND_EQUATION_H_
