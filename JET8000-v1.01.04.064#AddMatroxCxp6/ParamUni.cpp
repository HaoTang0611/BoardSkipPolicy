// ParamUni.cpp: implementation of the CParamUni class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "ParamUni.h"
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
CParamUni::CParamUni()
{
	CParamUni::PreInitParam();
	CParamUni::InitialParam();
}
//-------------------------------------------------------------------------------------//
CParamUni::CParamUni(const CParamUni &Param)
{
	CParamUni::PreInitParam();
	CParamUni::CloneParam(Param);
}
//-------------------------------------------------------------------------------------//
CParamUni::~CParamUni()
{

}
//-------------------------------------------------------------------------------------//
CParamUni& CParamUni::operator=(const CParamUni &Param)
{
	if ( this == &Param ) { return *this; }
	CParamUni::CloneParam(Param);
	return *this;
}
//-------------------------------------------------------------------------------------//
inline void CParamUni::PreInitParam()
{
}
//-------------------------------------------------------------------------------------//
inline void CParamUni::InitialParam()
{
	m_Delimiter = _T('$');
	//---------------------------------------------------------------------------------//
	m_sTempText = _T("");
	//---------------------------------------------------------------------------------//
	m_nItem = -1;
	m_nSubItem = -1;	
	m_WndCtrlID = 0;
	m_BtnWndPtr = NULL;
	m_BtnWndPtr2 = NULL;
	m_ListCtrlPtr = NULL;//北跌怠	
	//---------------------------------------------------------------------------------//
	m_ParamID = -1;
	m_ParamIndex = -1;
	m_bReadOnly = false;
	m_Precision = 2;
	m_bChanged = false;
	m_bReStart = false;
	m_DataType = PARAM_DATA_STR;	
	m_TextColor = CLR_DEFAULT;
	//---------------------------------------------------------------------------------//		
	m_Value_INT = 0;
	m_Default_INT = 0;	
	//---------------------------------------------------------------------------------//		
	m_Value_DBL = 0;
	m_Default_DBL = 0;	
	//---------------------------------------------------------------------------------//	
	m_Value_CLR = 0;
	m_Default_CLR = 0;
	//---------------------------------------------------------------------------------//	
	m_Max.DBL = DBL_MAX;
	m_Min.DBL =-DBL_MAX;
	//---------------------------------------------------------------------------------//	
	this->m_SelParam = -1;
	this->m_SelDefault = -1;
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
inline void CParamUni::CloneParam(const CParamUni &Param)
{
	m_Delimiter = Param.m_Delimiter;
	//---------------------------------------------------------------------------------//
	m_sSection = Param.m_sSection;
	m_sKeyName = Param.m_sKeyName;	
	m_sCaption = Param.m_sCaption;
	m_sTempText = Param.m_sTempText;	
	m_Description = Param.m_Description;	
	//---------------------------------------------------------------------------------//
	m_nItem = Param.m_nItem;
	m_nSubItem = Param.m_nSubItem;	
	m_BtnWndPtr = Param.m_BtnWndPtr;
	m_BtnWndPtr2 = Param.m_BtnWndPtr2;
	m_WndCtrlID = Param.m_WndCtrlID;
	m_ListCtrlPtr = Param.m_ListCtrlPtr;//北跌怠
	m_ParamText = Param.m_ParamText;
	m_TextColor = Param.m_TextColor;
	//---------------------------------------------------------------------------------//
	m_ParamID = Param.m_ParamID;
	m_ParamIndex = Param.m_ParamIndex;
	m_bReadOnly = Param.m_bReadOnly;
	m_Precision = Param.m_Precision;
	m_bChanged = Param.m_bChanged;
	m_bReStart = Param.m_bReStart;		
	m_DataType = Param.m_DataType;		
	//---------------------------------------------------------------------------------//
	m_Value_STR = Param.m_Value_STR;
	m_Default_STR = Param.m_Default_STR;
	//---------------------------------------------------------------------------------//
	m_Max = Param.m_Max;
	m_Min = Param.m_Min;
	//---------------------------------------------------------------------------------//	
	m_Value_INT = Param.m_Value_INT;
	m_Default_INT = Param.m_Default_INT;
	//---------------------------------------------------------------------------------//		
	m_Value_DBL = Param.m_Value_DBL;
	m_Default_DBL = Param.m_Default_DBL;
	//---------------------------------------------------------------------------------//
	m_Value_CLR = Param.m_Value_CLR;
	m_Default_CLR = Param.m_Default_CLR;
	//---------------------------------------------------------------------------------//
	m_SelParam = Param.m_SelParam;
	m_SelDefault = Param.m_SelDefault;	
	m_SelTextList = Param.m_SelTextList;
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
void  CParamUni::SetSection(LPCTSTR str)
{
	this->m_sSection = str;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CParamUni::GetSection() const
{
	return this->m_sSection;
}
//-------------------------------------------------------------------------------------//
void CParamUni::SetKeyName(LPCTSTR str)
{
	this->m_sKeyName = str;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CParamUni::GetKeyName() const
{
	return this->m_sKeyName;
}
//-------------------------------------------------------------------------------------//
void CParamUni::SetCaption(LPCTSTR str)
{
	m_sCaption = str;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CParamUni::GetCaption() const
{
	return m_sCaption;
}
//-------------------------------------------------------------------------------------//
void CParamUni::SetDesction(LPCTSTR str)
{
	m_Description = str;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CParamUni::GetDesction() const
{
	return m_Description;
}
//-------------------------------------------------------------------------------------//
void CParamUni::SetTempText(LPCTSTR str)
{
	m_sTempText = str;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CParamUni::GetTempText() const
{
	return m_sTempText;
}
//-------------------------------------------------------------------------------------//
bool CParamUni::GetChanged()
{
	return m_bChanged;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CParamUni::GetParamText()
{
	return m_ParamText;
}
//-------------------------------------------------------------------------------------//
bool CParamUni::SetNewValue(LPCTSTR Value)//砞﹚穝把计
{
	bool     IsOK=true;
	int      iTemp=0;
	double   fTemp=0.0;
	COLORREF cTemp=0;
	switch ( m_DataType )
	{
	case PARAM_DATA_STR:
		SetNewValue_STR(Value);		
		break;
	case PARAM_DATA_INT:
		iTemp = ::_ttoi(Value);
		if ( SetNewValue_INT(iTemp) == false )
		{	return false; }				
		break;
	case PARAM_DATA_DBL:
		fTemp = ::_tcstod(Value, NULL);
		if ( SetNewValue_DBL(fTemp) == false )
		{	return false; }		
		break;
	case PARAM_DATA_CLR:
		cTemp = (COLORREF)(::_ttoi(Value));
		if ( SetNewValue_CLR(cTemp) == false )
		{	return false; }				
		break;		
	case PARAM_DATA_SEL:
		if ( SetNewValue_SEL(Value) == false )
		{	return false; }	
		
		break;
	default:	IsOK=false;	break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
void CParamUni::SetNewValue_STR(LPCTSTR Param)
{
	this->m_Value_STR = Param;	
	this->m_ParamText = Param;
	if ( m_Default_STR != Param )
	{	m_bChanged = true; }
	else
	{	m_bChanged = false; }
}
//-------------------------------------------------------------------------------------//
bool CParamUni::GetReStart() const
{
	return m_bReStart;
}
//-------------------------------------------------------------------------------------//
void CParamUni::SetReStart(bool Value)
{
	m_bReStart = Value;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CParamUni::GetValue_STR() const
{
	return this->m_Value_STR;
}
//-------------------------------------------------------------------------------------//
void CParamUni::SetValue_STR(LPCSTR Value)
{
	this->m_bChanged = false;
	this->m_Value_STR = Value;
	this->m_Default_STR = Value;
	this->m_ParamText = Value;
	this->m_DataType = PARAM_DATA_STR;
}
//-------------------------------------------------------------------------------------//
void CParamUni::SetValue_STR(LPCWSTR Value)
{
	this->m_bChanged = false;
	this->m_Value_STR = Value;
	this->m_Default_STR = Value;
	this->m_ParamText = Value;
	this->m_DataType = PARAM_DATA_STR;
}
//-------------------------------------------------------------------------------------//
bool CParamUni::SetNewValue_INT(int Value)
{
	if ( Value>m_Max.INT || Value<m_Min.INT ) { return false;}
	this->m_Value_INT = Value;
	this->m_Value_STR.Format(_T("%d"), Value);
	this->m_ParamText = m_Value_STR;
	if ( Value != m_Default_INT )
	{	m_bChanged = true; }
	else
	{	m_bChanged = false; }
	return true;
}
//-------------------------------------------------------------------------------------//
int CParamUni::GetValue_INT() const
{
	return this->m_Value_INT;
}
//-------------------------------------------------------------------------------------//
void CParamUni::SetValue_INT(int Value, int Min, int Max)
{
	this->m_bChanged = false;
	this->m_Min.INT = Min;
	this->m_Max.INT = Max;
	this->m_Value_INT = Value;
	this->m_Default_INT = Value;	
	this->m_DataType = PARAM_DATA_INT;
	this->m_Value_STR.Format(_T("%d"), Value);
	this->m_Default_STR = this->m_Value_STR;
	this->m_ParamText = this->m_Value_STR;
}	
//-------------------------------------------------------------------------------------//
bool  CParamUni::SetNewValue_DBL(double Value)
{
	if ( Value>m_Max.DBL || Value<m_Min.DBL ) { return false;}	
	switch ( m_Precision )
	{
	case 0:	this->m_Value_STR.Format(_T("%.0f"), Value);	break;
	case 1:	this->m_Value_STR.Format(_T("%.1f"), Value);	break;
	case 2:	this->m_Value_STR.Format(_T("%.2f"), Value);	break;
	case 3:	this->m_Value_STR.Format(_T("%.3f"), Value);	break;
	case 4:	this->m_Value_STR.Format(_T("%.4f"), Value);	break;
	case 5:	this->m_Value_STR.Format(_T("%.5f"), Value);	break;
	case 6:	this->m_Value_STR.Format(_T("%.6f"), Value);	break;
	case 7:	this->m_Value_STR.Format(_T("%.7f"), Value);	break;
	case 8:	this->m_Value_STR.Format(_T("%.8f"), Value);	break;
	case 9:	this->m_Value_STR.Format(_T("%.9f"), Value);	break;
	default:
		this->m_Value_STR.Format(_T("l%f"), Value);
		break;
	}
	this->m_Value_DBL = Value;
	if ( Value != m_Default_DBL )
	{	m_bChanged = true; }
	else
	{	m_bChanged = false; }
	this->m_ParamText = this->m_Value_STR;
	return true;
}
//-------------------------------------------------------------------------------------//
double CParamUni::GetValue_DBL() const
{
	return this->m_Value_DBL;
}
//-------------------------------------------------------------------------------------//
void CParamUni::SetValue_DBL(double Value, int Precision, double Min, double Max)
{
	this->m_bChanged = false;
	this->m_Min.DBL = Min;
	this->m_Max.DBL = Max;
	this->m_Value_DBL = Value;
	this->m_Default_DBL = Value;	
	this->m_Precision = Precision;
	this->m_DataType = PARAM_DATA_DBL;
	switch ( m_Precision )
	{
	case 0:		this->m_Value_STR.Format(_T("%.0f"), Value);	break;
	case 1:		this->m_Value_STR.Format(_T("%.1f"), Value);	break;
	case 2:		this->m_Value_STR.Format(_T("%.2f"), Value);	break;
	case 3:		this->m_Value_STR.Format(_T("%.3f"), Value);	break;
	case 4:		this->m_Value_STR.Format(_T("%.4f"), Value);	break;
	case 5:		this->m_Value_STR.Format(_T("%.5f"), Value);	break;
	case 6:		this->m_Value_STR.Format(_T("%.6f"), Value);	break;
	case 7:		this->m_Value_STR.Format(_T("%.7f"), Value);	break;
	case 8:		this->m_Value_STR.Format(_T("%.8f"), Value);	break;
	case 9:		this->m_Value_STR.Format(_T("%.9f"), Value);	break;
	case 10:	this->m_Value_STR.Format(_T("%.10f"), Value);	break;
	case 11:	this->m_Value_STR.Format(_T("%.11f"), Value);	break;
	case 12:	this->m_Value_STR.Format(_T("%.12f"), Value);	break;
	case 13:	this->m_Value_STR.Format(_T("%.13f"), Value);	break;
	case 14:	this->m_Value_STR.Format(_T("%.14f"), Value);	break;
	case 15:	this->m_Value_STR.Format(_T("%.15f"), Value);	break;
	default:
		this->m_Value_STR.Format(_T("%.16f"), Value);
		break;
	}
	this->m_Default_STR = this->m_Value_STR;	
	this->m_ParamText = this->m_Value_STR;
}
//-------------------------------------------------------------------------------------//
COLORREF CParamUni::GetValue_CLR() const
{
	return m_Value_CLR;
}
//-------------------------------------------------------------------------------------//
void CParamUni::SetValue_CLR(COLORREF Value)
{
	this->m_bChanged = false;	
	this->m_Value_CLR = Value;
	this->m_Default_CLR = Value;	
	this->m_DataType = PARAM_DATA_CLR;
	
	int Red = GetRValue(Value);
	int Grn = GetGValue(Value);
	int Blu = GetBValue(Value);	
	this->m_Value_STR.Format(_T("R:%d, G:%d, B:%d"), Red, Grn, Blu);
	this->m_Default_STR = this->m_Value_STR;	
	this->m_ParamText = this->m_Value_STR;
}
//-------------------------------------------------------------------------------------//
bool CParamUni::SetNewValue_CLR(COLORREF Value)
{		
	int Red = GetRValue(Value);
	int Grn = GetGValue(Value);
	int Blu = GetBValue(Value);	
	this->m_Value_STR.Format(_T("R:%d, G:%d, B:%d"), Red, Grn, Blu);

	this->m_Value_CLR = Value;
	if ( Value != m_Default_CLR )
	{	m_bChanged = true; }
	else
	{	m_bChanged = false; }
	this->m_ParamText = this->m_Value_STR;
	return true;
}
//-------------------------------------------------------------------------------------//
void CParamUni::ClearSelList()
{	
	this->m_SelTextList.clear();	
}
//-------------------------------------------------------------------------------------//
void CParamUni::AddSelItem(DWORD Param)
{
	CString Text;
	Text.Format(_T("%d"), Param);
	AddSelItem(Param, Text);
	return;
}
//-------------------------------------------------------------------------------------//
void CParamUni::AddSelItem(DWORD Param, LPCTSTR Text)
{
	CString str;	
	str.Format(_T("%d%c%s"), Param, m_Delimiter, Text);
	this->m_SelTextList.push_back(str);		
	return;
}
//-------------------------------------------------------------------------------------//
size_t CParamUni::GetSelItemCount()
{
	return this->m_SelTextList.size();
}
//-------------------------------------------------------------------------------------//
bool CParamUni::GetSelItem(size_t idx, bool check, int &Param, CString &Text)
{
	if ( true == check )
	{
		const size_t count = this->m_SelTextList.size();
		if ( idx >= count ) { return false; }
	}

	CString str;
	CString strFull = m_SelTextList[idx];
	const int len = strFull.GetLength();
	if ( len == 0 ) { return false; }
	const int index = strFull.Find(m_Delimiter, 0);
	if ( index < 0 ) 
	{ return false; }
	
	str = strFull.Left(index);
	Param = ::_ttoi(str);	
	Text = strFull.Right(len-index-1);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CParamUni::SetValue_SEL(DWORD Param)
{		
	size_t i=0;
	CString str;
	int     value=0;	
	const size_t count = m_SelTextList.size();
	for ( i=0; i<count; i++ )
	{
		if ( GetSelItem(i, false, value, str) == false )
		{	return false; }
		if ( value != Param ) { continue; }
		this->m_ParamText = str;
		break;
	}
	if ( i == count )
	{ 
		str.Format(_T("Unknow[%d]"), Param);
		AddSelItem(Param, str);
		m_ParamText = str;
	}

	this->m_SelParam = Param;
	this->m_SelDefault = Param;
	this->m_Value_STR.Format(_T("%d"), Param);	
	this->m_DataType = PARAM_DATA_SEL;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CParamUni::SetValue_SEL(LPCTSTR Text)
{	
	size_t i=0;
	CString str;
	int     value=0;	
	const size_t count = m_SelTextList.size();
	for ( i=0; i<count; i++ )
	{
		if ( GetSelItem(i, false, value, str) == false )
		{	return false; }
		if ( str != Text ) { continue; }
		this->m_ParamText = str;
		break;
	}
	if ( i == count ) { return false; }

	this->m_SelParam = value;
	this->m_SelDefault = value;
	this->m_Value_STR.Format(_T("%d"), value);	
	this->m_DataType = PARAM_DATA_SEL;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CParamUni::SetNewValue_SEL(DWORD Param)
{	
	size_t i=0;
	CString str;
	int     value=0;	
	const size_t count = m_SelTextList.size();	
	for ( i=0; i<count; i++ )
	{
		if ( GetSelItem(i, false, value, str) == false )
		{	return false; }
		if ( value != Param ) { continue; }
		this->m_ParamText = str;
		break;
	}
	if ( i == count ) { return false; }

	this->m_Value_STR.Format(_T("%d"), Param);	
	this->m_SelParam = Param;
	if ( Param != m_SelDefault )
	{	m_bChanged = true; }
	else
	{	m_bChanged = false; }
	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CParamUni::SetNewValue_SEL(LPCTSTR Text)
{
	size_t i=0;
	CString str;
	int     value=0;	
	const size_t count = m_SelTextList.size();	
	for ( i=0; i<count; i++ )
	{
		if ( GetSelItem(i, false, value, str) == false )
		{	return false; }
		if ( str != Text ) { continue; }
		this->m_ParamText = str;
		break;
	}
	if ( i == count ) { return false; }

	this->m_Value_STR.Format(_T("%d"), value);	
	this->m_SelParam = value;
	if ( value != m_SelDefault )
	{	m_bChanged = true; }
	else
	{	m_bChanged = false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
DWORD CParamUni::GetSelParam()
{
	return this->m_SelParam;
}
//-------------------------------------------------------------------------------------//