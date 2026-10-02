#include "stdafx.h"
#include "JetGroundEquation.h"
//-------------------------------------------------------------------------------------//
CJetGroundEquation::CJetGroundEquation()
{
	PreInit();
	Initial();
}
//-------------------------------------------------------------------------------------//
CJetGroundEquation::CJetGroundEquation(const CJetGroundEquation &rhs)
{
	PreInit();
	Clone(rhs);
}
//-------------------------------------------------------------------------------------//
CJetGroundEquation::~CJetGroundEquation()
{
}
//-------------------------------------------------------------------------------------//
CJetGroundEquation& CJetGroundEquation::operator=(const CJetGroundEquation &rhs)
{
	if ( this == &rhs ) { return *this; }
	Clone(rhs);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CJetGroundEquation::PreInit()
{		
	m_UseMatrixFunc = false;
	::memset(m_T, 0x00, sizeof(m_T));
	m_GroundEquationMode = GROUND_EQUATION_NONE;
}
//-------------------------------------------------------------------------------------//
void CJetGroundEquation::Initial()
{
}
//-------------------------------------------------------------------------------------//
void CJetGroundEquation::Clone(const CJetGroundEquation &rhs)
{		
	::memcpy(m_T, rhs.m_T, sizeof(m_T));	
	m_UseMatrixFunc = rhs.m_UseMatrixFunc;
	m_GroundEquationMode = rhs.m_GroundEquationMode;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CJetGroundEquation::GetErrorString() const
{
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
double* CJetGroundEquation::GetGroundParamList()
{
	return m_T;
}
//-------------------------------------------------------------------------------------//
const double* CJetGroundEquation::GetGroundParamList() const
{
	return m_T;
}
//-------------------------------------------------------------------------------------//
int CJetGroundEquation::GetGroundParamCount() const
{
	int Count = 0;
	GROUND_EQUATION_MODE Mode=GetGroundEquationMode();
	switch ( Mode )
	{
	case GROUND_EQUATION_PLANE:	Count = 3;	break;
	case GROUND_EQUATION_CURVE:	Count = 6;	break;
	}
	return Count;
}
//-------------------------------------------------------------------------------------//
GROUND_EQUATION_MODE CJetGroundEquation::GetGroundEquationMode() const
{
	return m_GroundEquationMode;
}
//-------------------------------------------------------------------------------------//
void CJetGroundEquation::SetGroundEquationMode(GROUND_EQUATION_MODE val)
{
	m_GroundEquationMode = val;
	switch ( val )
	{
	default:
	case GROUND_EQUATION_NONE:
	case GROUND_EQUATION_PLANE:
		m_UseMatrixFunc = false;
		break;
	case GROUND_EQUATION_CURVE:	
		m_UseMatrixFunc = true;
		break;
	}
}
//-------------------------------------------------------------------------------------//
bool CJetGroundEquation::VerticalPlane()//キ
{
	for ( int i=1; i<GROUND_EQUATION_PARAM_COUNT; i++ )
	{	m_T[i] = 0.0;	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CJetGroundEquation::ClearGroundParam()//睲埃膀非把计
{	
	::memset(m_T, 0x00, sizeof(m_T));
	SetGroundEquationMode(GROUND_EQUATION_NONE);
}
//-------------------------------------------------------------------------------------//
void CJetGroundEquation::InitGroundParam(GROUND_EQUATION_MODE Mode)//﹍て膀非把计
{	
	ClearGroundParam();

	m_ErrorString = _T("");
	SetGroundEquationMode(Mode);	

	if ( false == m_UseMatrixFunc )
	{
		t_X = t_Y = t_Z = 0.0;
		t_XY= t_XZ= t_YZ= 0.0;
		t_XX= t_YY= t_ZZ= 0.0;	
	}
	else
	{
		const int ParamCount=GetGroundParamCount();//3:1顶, 6:2顶, 10::2顶
		const int TotalMatrixElements=ParamCount*ParamCount;		
		t_MatrixElem1.resize(TotalMatrixElements);
		t_MatrixElem2.resize(ParamCount);	
		std::fill(t_MatrixElem1.begin(), t_MatrixElem1.end(), 0);
		std::fill(t_MatrixElem2.begin(), t_MatrixElem2.end(), 0);
	}
}
//-------------------------------------------------------------------------------------//
bool CJetGroundEquation::AddGroundValue(double x, double y, double z)//膀非计
{
	bool bSucc=true;
	if ( false == m_UseMatrixFunc )
	{
		//X += x;		Y += y;		Z += z;
		//XY += x*y;	XZ += x*z;	YZ += y*z;
		//XX += x*x;	YY += y*y;  ZZ += 1;		

		t_X += x;		t_Y += y;		t_Z += z;
		t_XY += x*y;	t_XZ += x*z;	t_YZ += y*z;
		t_XX += x*x;	t_YY += y*y;	t_ZZ += 1;
	}
	else
	{
		double t[GROUND_EQUATION_PARAM_COUNT];
		const double C = 1.0;
		const double Height = z;
		const int ParamCount=GetGroundParamCount();
		switch ( ParamCount )
		{
		default:
		case 3:	
			t[0]=C;		t[1]=x;		t[2]=y;	
			break;
		case 5://t0+t1x+t2y+t3xx+t4yy
			t[0]=C;
			t[1]=x;		t[2]=y;
			t[3]=x*x;	t[4]=y*y;
			break;
		case 6://t0+t1x+t2y+t3xy+t4xx+t5yy
			t[0]=C;		t[1]=x;		t[2]=y;
			t[3]=x*y;	t[4]=x*x;	t[5]=y*y;
			break;
		case 10://t0+t1x+t2y+t3z+t4xy+t5xz+t6yz+t7xx+t8yy+t9zz
			t[0]=C;		
			t[1]=x;		t[2]=y;		t[3]=z;
			t[4]=x*y;	t[5]=x*z;	t[6]=y*z;
			t[7]=x*x;	t[8]=y*y;	t[9]=z*z;
			break;
		}	
		for ( int i=0; i<ParamCount; i++ )
		{	t_MatrixElem2[i] += Height*t[i];	}
	
		int idx=0;
		for ( int i=0; i<ParamCount; i++ )
		{
			for ( int j=0; j<ParamCount; j++ )
			{	
				t_MatrixElem1[idx] += t[i]*t[j];
				idx ++;
			}			
		}		
	}
	return bSucc;
}
//-------------------------------------------------------------------------------------//
bool CJetGroundEquation::CalcGroundParam()//璸衡膀非把计
{
	double nX=0.0, nY=0.0, nZ=0.0;
	if ( false == m_UseMatrixFunc )
	{
		//norm = XX*YY*ZZ + XY*Y*X + X*Y*XY - X*YY*X - XY*XY*ZZ - XX*Y*Y;
		const double norm = (t_XX*t_YY*t_ZZ)+(t_XY*t_Y*t_X)+(t_X*t_Y*t_XY)-(t_X*t_YY*t_X)-(t_XY*t_XY*t_ZZ)-(t_XX*t_Y*t_Y);
		if( 0.0 != norm)
		{
			//nX = (XZ*YY*ZZ + YZ*Y*X + Z*Y*XY - X*YY*Z - XY*YZ*ZZ - XZ*Y*Y)/norm;
			//nY = (XX*YZ*ZZ + XY*Z*X + X*Y*XZ - X*YZ*X - XZ*XY*ZZ - XX*Z*Y)/norm;
			//nZ = (XX*YY*Z + XY*Y*XZ + X*YZ*XY - XZ*YY*X - XY*XY*Z - XX*Y*YZ)/norm;
			nX = ((t_XZ*t_YY*t_ZZ)+(t_YZ*t_Y*t_X) +(t_Z*t_Y*t_XY) -(t_X*t_YY*t_Z) -(t_XY*t_YZ*t_ZZ)-(t_XZ*t_Y*t_Y))/norm;
			nY = ((t_XX*t_YZ*t_ZZ)+(t_XY*t_Z*t_X) +(t_X*t_Y*t_XZ) -(t_X*t_YZ*t_X) -(t_XZ*t_XY*t_ZZ)-(t_XX*t_Z*t_Y))/norm;
			nZ = ((t_XX*t_YY*t_Z) +(t_XY*t_Y*t_XZ)+(t_X*t_YZ*t_XY)-(t_XZ*t_YY*t_X)-(t_XY*t_XY*t_Z) -(t_XX*t_Y*t_YZ))/norm;		
		}
		m_T[0] = nZ;
		m_T[1] = nX;
		m_T[2] = nY;
	}
	else
	{
		try
		{		
			const int ParamCount=GetGroundParamCount();
			const int NRows = ParamCount;
			const int NCols = ParamCount;
			cv::Mat mat1(NRows, NCols, CV_64FC1, t_MatrixElem1.data());
			cv::Mat mat2(NRows, 1, CV_64FC1, t_MatrixElem2.data());	
			cv::Mat mat3=mat1.inv();	
			//代刚は痻皚ノ
			//cv::Mat mat4=mat1*mat3;	
			//―秆
			cv::Mat mat5 = mat3*mat2;
			//盢戈秈挡狦皚い			
			for ( int i=0; i<ParamCount; i++ )
			{	m_T[i] = mat5.ptr<double>(0)[i];	}			
		}
		catch ( cv::Exception& e )
		{	
			const char* msg_e = e.what();  			
			m_ErrorString = CString(msg_e);
			return false;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetGroundEquation::CalcLocalGroundParam(const RECT &Rect, GROUND_EQUATION_MODE LocalMode, CJetGroundEquation &LocalEquationParam) const//璸衡Ы场膀非把计
{
	double x=0, y=0, z=0;	

	LocalEquationParam.InitGroundParam(LocalMode);	

	//Left Top
	x = Rect.left; y = Rect.top; z=CalcGroundValue(x, y);
	x -= Rect.left;	y -= Rect.top;	LocalEquationParam.AddGroundValue(x, y, z);	

	//Right Top
	x = Rect.right; y = Rect.top; z=CalcGroundValue(x, y);
	x -= Rect.left;	y -= Rect.top;	LocalEquationParam.AddGroundValue(x, y, z);	

	//Left Bot
	x = Rect.left; y = Rect.bottom; z=CalcGroundValue(x, y);
	x -= Rect.left;	y -= Rect.top;	LocalEquationParam.AddGroundValue(x, y, z);	

	//Right Bot
	x = Rect.right; y = Rect.bottom; z=CalcGroundValue(x, y);
	x -= Rect.left;	y -= Rect.top;	LocalEquationParam.AddGroundValue(x, y, z);	

	if ( GROUND_EQUATION_PLANE != LocalMode )
	{
		double cx=(Rect.left+Rect.right)*0.5;
		double cy=(Rect.top+Rect.bottom)*0.5;

		//Center Top
		x = cx; y = Rect.top; z=CalcGroundValue(x, y);
		x -= Rect.left;	y -= Rect.top;	LocalEquationParam.AddGroundValue(x, y, z);	

		//Left Center
		x = Rect.left; y = cy; z=CalcGroundValue(x, y);
		x -= Rect.left;	y -= Rect.top;	LocalEquationParam.AddGroundValue(x, y, z);	

		//Center Center
		x = cx; y = cy; z=CalcGroundValue(x, y);
		x -= Rect.left;	y -= Rect.top;	LocalEquationParam.AddGroundValue(x, y, z);	

		//Left Center
		x = Rect.right; y = cy; z=CalcGroundValue(x, y);
		x -= Rect.left;	y -= Rect.top;	LocalEquationParam.AddGroundValue(x, y, z);	

		//Center Bot
		x = cx; y = Rect.bottom; z=CalcGroundValue(x, y);
		x -= Rect.left;	y -= Rect.top;	LocalEquationParam.AddGroundValue(x, y, z);	
	}
	LocalEquationParam.CalcGroundParam();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetGroundEquation::CheckGroundParamValid() const//絋粄膀非把计Τ
{
	//if ( fabs(m_T[0]) < 0.0001 )
	if ( GROUND_EQUATION_NONE == m_GroundEquationMode )
	{	return false; }			
	return true;
}
//-------------------------------------------------------------------------------------//
double CJetGroundEquation::CalcGroundValue(double x, double y) const//璸衡膀非计
{
	double Val=0.0;	
	switch ( m_GroundEquationMode )
	{
	case GROUND_EQUATION_PLANE:
		Val = m_T[0]+(m_T[1]*x)+(m_T[2]*y);
		break;
	case GROUND_EQUATION_CURVE:
		Val = m_T[0]+(m_T[1]*x)+(m_T[2]*y)+(m_T[3]*x*y)+(m_T[4]*x*x)+(m_T[5]*y*y);
		break;
	}		
	return Val;
}
//-------------------------------------------------------------------------------------//
void CJetGroundEquation::SetGroundNormal(double nX, double nY, double nZ)//砞﹚膀非猭秖
{
	ClearGroundParam();
	m_T[0] = nZ;	m_T[1] = nX;	m_T[2] = nY;
	SetGroundEquationMode(GROUND_EQUATION_PLANE);
	return ;
}
//-------------------------------------------------------------------------------------//
void CJetGroundEquation::GetGroundNormal(double &nX, double &nY, double &nZ) const//眔膀非猭秖
{
	nX = m_T[1];
	nY = m_T[2];
	nZ = m_T[0];
}
//-------------------------------------------------------------------------------------//