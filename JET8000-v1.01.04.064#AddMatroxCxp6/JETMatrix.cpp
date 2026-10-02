#include "stdafx.h"
#include "JETMatrix.h"
#include <stdio.h>
//----------------------------------------------------------------------------//
CJetMatrix::CJetMatrix()//建構子
{
	this->PreInitialize();
}
//----------------------------------------------------------------------------//
CJetMatrix::~CJetMatrix()//解構子
{
	this->ClearDataBuffer();
}
//----------------------------------------------------------------------------//
CJetMatrix::CJetMatrix( const CJetMatrix &Matrix)//複製建構子
{
	this->PreInitialize();
	this->CloneMatrix(Matrix);
}
//----------------------------------------------------------------------------//
void CJetMatrix::Initialize()//初始化
{
	this->ClearDataBuffer();
}
//----------------------------------------------------------------------------//
bool CJetMatrix::CloneMatrix(const CJetMatrix &Matrix)//複製函式
{
	this->Initialize();
	if ( this->SetMatrixSize(Matrix.m_NRows, Matrix.m_NCols, Matrix.m_pData) == false ) 
	{	return false; }

	return true;
}
//----------------------------------------------------------------------------//
void CJetMatrix::PreInitialize()//預先初始化
{
	this->m_pData = NULL;//資料群
	this->m_NRows = 0;//多少行
	this->m_NCols = 0;//多少列	
}
//----------------------------------------------------------------------------//
void CJetMatrix::ClearDataBuffer()//清除資料群記憶體空間
{
	if ( this->m_pData != NULL )
	{
		delete[] this->m_pData; 
		this->m_pData = NULL;
	}
	this->m_NRows = 0;//多少行
	this->m_NCols = 0;//多少列	
}
//----------------------------------------------------------------------------//
CJetMatrix &CJetMatrix::operator=(const CJetMatrix& Matrix)//等於運算子
{
	if ( this == &Matrix ) { return *this; }
	this->CloneMatrix(Matrix);
	return *this;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator*(const double a)//乘與一個常數運算子
{
	if ( this->m_pData == NULL )
	{	return *this; }

	const int NDatas = this->m_NRows*this->m_NCols;

	CJetMatrix tempMatrix(*this);
	if ( NDatas > 0 )
	{
		int i=0;
		for ( i=0; i<NDatas; i++ )
		{	tempMatrix.m_pData[i] = this->m_pData[i]*a;	}
	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator*(const float a)//乘與一個常數運算子
{
	if ( this->m_pData == NULL )
	{	return *this; }

	const int NDatas = this->m_NRows*this->m_NCols;

	CJetMatrix tempMatrix(*this);
	if ( NDatas > 0 )
	{
		int i=0;
		for ( i=0; i<NDatas; i++ )
		{	tempMatrix.m_pData[i] = this->m_pData[i]*a;	}
	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator*(const int a)//乘與一個常數運算子
{
	if ( this->m_pData == NULL )
	{	return *this; }

	const int NDatas = this->m_NRows*this->m_NCols;

	CJetMatrix tempMatrix(*this);
	if ( NDatas > 0 )
	{
		int i=0;
		for ( i=0; i<NDatas; i++ )
		{	tempMatrix.m_pData[i] = this->m_pData[i]*a;	}
	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator*(const long a)//乘與一個常數運算子
{
	if ( this->m_pData == NULL )
	{	return *this; }

	const int NDatas = this->m_NRows*this->m_NCols;

	CJetMatrix tempMatrix(*this);
	if ( NDatas > 0 )
	{
		int i=0;
		for ( i=0; i<NDatas; i++ )
		{	tempMatrix.m_pData[i] = this->m_pData[i]*a;	}
	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator/(const double a)//乘與一個常數運算子
{
	if ( this->m_pData == NULL )
	{	return *this; }

	if ( a == 0 )
	{	return *this; }
	
	const int NDatas = this->m_NRows*this->m_NCols;

	CJetMatrix tempMatrix(*this);
	if ( NDatas > 0 )
	{
		int i=0;
		for ( i=0; i<NDatas; i++ )
		{	tempMatrix.m_pData[i] = this->m_pData[i]/a;	}
	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator/(const float a)//乘與一個常數運算子
{
	if ( this->m_pData == NULL )
	{	return *this; }

	if ( a == 0 )
	{	return *this; }
	
	const int NDatas = this->m_NRows*this->m_NCols;

	CJetMatrix tempMatrix(*this);
	if ( NDatas > 0 )
	{
		int i=0;
		for ( i=0; i<NDatas; i++ )
		{	tempMatrix.m_pData[i] = this->m_pData[i]/a;	}
	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator/(const int a)//乘與一個常數運算子
{
	if ( this->m_pData == NULL )
	{	return *this; }

	if ( a == 0 )
	{	return *this; }
	
	const int NDatas = this->m_NRows*this->m_NCols;

	CJetMatrix tempMatrix(*this);
	if ( NDatas > 0 )
	{
		int i=0;
		for ( i=0; i<NDatas; i++ )
		{	tempMatrix.m_pData[i] = this->m_pData[i]/a;	}
	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator/(const long a)//乘與一個常數運算子
{
	if ( this->m_pData == NULL )
	{	return *this; }

	if ( a == 0 )
	{	return *this; }
	
	const int NDatas = this->m_NRows*this->m_NCols;

	CJetMatrix tempMatrix(*this);
	if ( NDatas > 0 )
	{
		int i=0;
		for ( i=0; i<NDatas; i++ )
		{	tempMatrix.m_pData[i] = this->m_pData[i]/a;	}
	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator+(const double a)//加上一個常數運算子
{
		if ( this->m_pData == NULL )
	{	return *this; }	
	
	const int NDatas = this->m_NRows*this->m_NCols;

	CJetMatrix tempMatrix(*this);
	if ( NDatas > 0 )
	{
		int i=0;
		for ( i=0; i<NDatas; i++ )
		{	tempMatrix.m_pData[i] = this->m_pData[i]+a;	}
	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator+(const float a)//加上一個常數運算子
{
		if ( this->m_pData == NULL )
	{	return *this; }	
	
	const int NDatas = this->m_NRows*this->m_NCols;

	CJetMatrix tempMatrix(*this);
	if ( NDatas > 0 )
	{
		int i=0;
		for ( i=0; i<NDatas; i++ )
		{	tempMatrix.m_pData[i] = this->m_pData[i]+a;	}
	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator+(const int a)//加上一個常數運算子
{
	if ( this->m_pData == NULL )
	{	return *this; }	
	
	const int NDatas = this->m_NRows*this->m_NCols;

	CJetMatrix tempMatrix(*this);
	if ( NDatas > 0 )
	{
		int i=0;
		for ( i=0; i<NDatas; i++ )
		{	tempMatrix.m_pData[i] = this->m_pData[i]+a;	}
	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator+(const long a)//加上一個常數運算子
{
	if ( this->m_pData == NULL )
	{	return *this; }	
	
	const int NDatas = this->m_NRows*this->m_NCols;

	CJetMatrix tempMatrix(*this);
	if ( NDatas > 0 )
	{
		int i=0;
		for ( i=0; i<NDatas; i++ )
		{	tempMatrix.m_pData[i] = this->m_pData[i]+a;	}
	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator-(const double a)//減掉一個常數運算子
{
	if ( this->m_pData == NULL )
	{	return *this; }	
	
	const int NDatas = this->m_NRows*this->m_NCols;

	CJetMatrix tempMatrix(*this);
	if ( NDatas > 0 )
	{
		int i=0;
		for ( i=0; i<NDatas; i++ )
		{	tempMatrix.m_pData[i] = this->m_pData[i]-a;	}
	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator-(const float a)//減掉一個常數運算子
{
	if ( this->m_pData == NULL )
	{	return *this; }	
	
	const int NDatas = this->m_NRows*this->m_NCols;

	CJetMatrix tempMatrix(*this);
	if ( NDatas > 0 )
	{
		int i=0;
		for ( i=0; i<NDatas; i++ )
		{	tempMatrix.m_pData[i] = this->m_pData[i]-a;	}
	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator-(const int a)//減掉一個常數運算子
{
	if ( this->m_pData == NULL )
	{	return *this; }	
	
	const int NDatas = this->m_NRows*this->m_NCols;

	CJetMatrix tempMatrix(*this);
	if ( NDatas > 0 )
	{
		int i=0;
		for ( i=0; i<NDatas; i++ )
		{	tempMatrix.m_pData[i] = this->m_pData[i]-a;	}
	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator-(const long a)//減掉一個常數運算子
{
	if ( this->m_pData == NULL )
	{	return *this; }	
	
	const int NDatas = this->m_NRows*this->m_NCols;

	CJetMatrix tempMatrix(*this);
	if ( NDatas > 0 )
	{
		int i=0;
		for ( i=0; i<NDatas; i++ )
		{	tempMatrix.m_pData[i] = this->m_pData[i]-a;	}
	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator*(const CJetMatrix &Matrix)//乘與矩陣
{
	if ( this->m_pData == NULL )
	{	return *this; }
	if ( Matrix.m_pData == NULL )
	{	return *this; }	
	if ( this->m_NCols != Matrix.m_NRows )
	{	return *this; }

	int i=0, j=0, k=0;
	int idx1=0, idx2=0, idx3=0;
	double tempD=0;
	CJetMatrix tempMatrix;
	tempMatrix.SetMatrixSize(this->m_NRows, Matrix.m_NCols);//
	for ( i=0; i<this->m_NRows; i++ )
	{
		idx1 = i*this->m_NCols;
		for ( j=0; j<Matrix.m_NCols; j++ )
		{
			tempD=0;
			for ( k=0; k<this->m_NCols; k++ )
			{
				idx2 = k*Matrix.m_NCols;
				tempD += this->m_pData[idx1+k]*Matrix.m_pData[idx2+j];
			}
			idx3 = i*tempMatrix.m_NCols;
			tempMatrix.m_pData[idx3+j] = tempD;
		}
	}	
	return tempMatrix;
}
//----------------------------------------------------------------------------//
bool CJetMatrix::SetMatrixSize(const int NRows, const int NCols, const double *pData)//設定矩陣大小
{
	this->ClearDataBuffer();
	this->m_NCols = NCols;
	this->m_NRows = NRows;
	const int size = this->m_NCols*this->m_NRows;

	if ( size == 0 ) { return true; }

	this->m_pData = new double[size];

	if ( this->m_pData == NULL )
	{	this->ClearDataBuffer();	return false;	}

	if ( pData!=NULL )
	{	::memcpy(this->m_pData, pData, sizeof(double)*size);	}
	else
	{	::memset(this->m_pData, 0x00, sizeof(double)*size);	}
	return true;
}
//----------------------------------------------------------------------------//
bool CJetMatrix::operator==(const CJetMatrix& Matrix)//相等運算子
{
	if ( this->m_NCols != Matrix.m_NCols ) { return false; }
	if ( this->m_NRows != Matrix.m_NRows ) { return false; }
	
	const int size = this->m_NCols*this->m_NRows;
	if ( size > 0 )
	{
		if ( this->m_pData == NULL ) { return false; }
		if ( Matrix.m_pData == NULL ) { return false; }
		int i=0;
		for ( i=0; i<size; i++ )
		{
			if ( this->m_pData[i] != Matrix.m_pData[i] ) { return false; }
		}
	}
	return true;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator+(const CJetMatrix Matrix)//加上一個常數運算子
{
	if ( this->m_pData == NULL )
	{	return *this; }
	if ( Matrix.m_pData == NULL )
	{	return *this; }
	if ( this->m_NRows != Matrix.m_NRows ) 
	{	return *this; }
	if ( this->m_NCols != Matrix.m_NCols )
	{	return *this; }

	const int size = this->m_NCols*this->m_NRows;
	int i=0;	
	CJetMatrix tempMatrix;
	tempMatrix.SetMatrixSize(this->m_NRows, this->m_NCols);		
	for ( i=0; i<size; i++ )
	{	tempMatrix.m_pData[i] = this->m_pData[i]+Matrix.m_pData[i];	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::operator-(const CJetMatrix Matrix)//減掉一個常數運算子
{
	if ( this->m_pData == NULL )
	{	return *this; }
	if ( Matrix.m_pData == NULL )
	{	return *this; }
	if ( this->m_NRows != Matrix.m_NRows ) 
	{	return *this; }
	if ( this->m_NCols != Matrix.m_NCols )
	{	return *this; }

	const int size = this->m_NCols*this->m_NRows;
	int i=0;	
	CJetMatrix tempMatrix;
	tempMatrix.SetMatrixSize(this->m_NRows, this->m_NCols);		
	for ( i=0; i<size; i++ )
	{	tempMatrix.m_pData[i] = this->m_pData[i]-Matrix.m_pData[i];	}
	return tempMatrix;
}
//----------------------------------------------------------------------------//
bool CJetMatrix::GetDeterminant(double &Det)//取得行列式
{
	if ( this->m_NCols != this->m_NRows ) { return false; }
	if ( this->m_NCols == 0 ) { return false; }
	if ( this->m_NCols == 1 ) 
	{	
		Det = this->m_pData[0]; 
		return true;
	}
	if ( this->m_NCols == 2 )
	{
		Det = this->m_pData[0]*this->m_pData[3] - this->m_pData[1]*this->m_pData[2];
		return true;
	}	
	
	CJetMatrix Matrix(*this);
	double detVal = 1;
	double piv = 0;
	int i=0, j=0, k=0;
	int RowIndex=0;
	for (i=0; i<Matrix.m_NRows; i++)
	{
		RowIndex = Matrix.Pivot(i);
		if ( RowIndex < 0  ) { return false; }		
		detVal = - detVal;
		detVal = detVal * Matrix.m_pData[i*Matrix.m_NRows+i];
		for (j=i+1; j<Matrix.m_NRows; j++)
		{
			piv = Matrix.m_pData[j*Matrix.m_NRows+i]/Matrix.m_pData[i*Matrix.m_NRows+i];
			for (k=i+1; k<Matrix.m_NRows; k++)
			{				
				Matrix.m_pData[j*Matrix.m_NRows+k] -= piv * Matrix.m_pData[i*Matrix.m_NRows+k];
			}
		}
	}
   
	Det = detVal;
	return true;
}
//----------------------------------------------------------------------------//
bool CJetMatrix::GetSubMatrix(const int x, const int y, CJetMatrix &Matrix) const//取回子矩陣，扣除(x,y)後的矩陣
{
	if ( this->m_NCols < 1 ) { return false; }
	if ( this->m_NRows < 1 ) { return false; }

	if ( x >= this->m_NCols ) { return false; }
	if ( y >= this->m_NRows ) { return false; }

	const int NewNRows = this->m_NRows-1;
	const int NewNCols = this->m_NCols-1;

	if ( Matrix.SetMatrixSize(NewNRows, NewNCols) == false ) { return false; }
	int i=0, j=0, k=0;
	for ( i=0; i<m_NRows; i++ )
	{
		if ( i == y ) { continue; }
		for ( j=0; j<m_NCols; j++ )
		{
			if ( j == x ) { continue; }
			Matrix.m_pData[k] = this->m_pData[i*m_NCols+j];
		}
	}
	return true;
	
}
//----------------------------------------------------------------------------//
bool CJetMatrix::operator!=(const CJetMatrix& Matrix)//不相等運算子
{
	if ( this->operator ==(Matrix) == true ) { return false; }
	else { return true; }
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::Inversion()//反轉
{
	if ( this->m_NCols != this->m_NRows ) { return *this; }
	CJetMatrix Matrix(*this), ORGMatrix(*this);
	Matrix = Matrix.Unit();
	
	double a1=0, a2=0, *RowPtr=NULL;
	int RowIndex = 0;
	int i=0, j=0, k=0;
	for (k=0; k <ORGMatrix.m_NRows; k++)
	{
		RowIndex = ORGMatrix.Pivot(k);
		if ( RowIndex < 0 ) 
		{ 
			return (*this); 
		}		

		if ( RowIndex != k)
		{
			//切換列
			for ( i=0; i<ORGMatrix.m_NRows; i++ )
			{
				a1 = Matrix.m_pData[k*ORGMatrix.m_NCols+i];
				Matrix.m_pData[k*ORGMatrix.m_NCols+i] = Matrix.m_pData[RowIndex*ORGMatrix.m_NCols+i];
				Matrix.m_pData[RowIndex*ORGMatrix.m_NCols+i] = a1;
			}			
		}
		
		a1 = ORGMatrix.m_pData[k*ORGMatrix.m_NCols+k];
		for (j=0; j <ORGMatrix.m_NRows; j++)
		{			
			ORGMatrix.m_pData[k*ORGMatrix.m_NCols+j] /= a1;
			Matrix.m_pData[k*ORGMatrix.m_NCols+j] /= a1;			
		}
		for (i=0; i <ORGMatrix.m_NRows; i++)
		{
			if (i != k)
			{				
				a2 = ORGMatrix.m_pData[i*ORGMatrix.m_NCols+k];
				for (j=0; j < ORGMatrix.m_NRows; j++)
				{					
					ORGMatrix.m_pData[i*ORGMatrix.m_NCols+j] -= a2*ORGMatrix.m_pData[k*ORGMatrix.m_NCols+j];
					Matrix.m_pData[i*ORGMatrix.m_NCols+j] -= a2*Matrix.m_pData[k*ORGMatrix.m_NCols+j];
				}
			}
		}
	}
	return Matrix;
}
//----------------------------------------------------------------------------//
CJetMatrix  CJetMatrix::Unit()//單位矩陣
{
	CJetMatrix Matrix;
	Matrix.SetMatrixSize(this->m_NRows, this->m_NCols);
	int i=0;
	for ( i=0; i<this->m_NRows; i++ )
	{	Matrix.m_pData[i*this->m_NCols+i] = 1;	}
	return Matrix;
}
//----------------------------------------------------------------------------//
int CJetMatrix::Pivot(const int StartNRow)//軸樞矩陣
{
	if ( StartNRow >= this->m_NRows ) { return -1; }
	CJetMatrix Matrix(*this);
	int i=StartNRow;
	int idx=StartNRow;
	double Max=this->m_pData[i*this->m_NCols+i];	
	if ( Max < 0 ) { Max = -Max; }
	double tempD = 0;
	for ( i=StartNRow+1; i<this->m_NRows; i++ )
	{
		tempD = this->m_pData[i*this->m_NCols+StartNRow];
		if ( tempD < 0 ) { tempD = -tempD; }
		if ( tempD > Max ) 
		{
			Max = tempD; 
			idx = i;
		}
	}
	if( Max == 0 ) { return -1; }
	if ( idx == StartNRow ) { return idx; }//自己那一行已經是最小值
	//更換列	
	int idx1=m_NCols*StartNRow;
	int idx2=m_NCols*idx;
	for ( i=0; i<this->m_NCols; i++ )
	{
		tempD = this->m_pData[idx1+i];
		this->m_pData[idx1+i] = this->m_pData[idx2+i];
		this->m_pData[idx2+i] = tempD;
	}	
	return idx;
}
//----------------------------------------------------------------------------//
double CJetMatrix::GetElement(int x, int y) const//取得數值
{
	const int idx = y*this->m_NCols+x;
	const int size = this->m_NCols*this->m_NRows;
	if ( idx >= size ) { return 0; }
	return this->m_pData[idx];
}
//----------------------------------------------------------------------------//
CJetMatrix CJetMatrix::Transpose()//轉置矩陣
{
	CJetMatrix Matrix;
	Matrix.SetMatrixSize(this->m_NCols, this->m_NRows);
	int i=0, j=0;
	for ( i=0; i<this->m_NRows; i++ )
	{
		for ( j=0; j<this->m_NCols; j++ )
		{
			Matrix.m_pData[j*m_NRows+i] = this->m_pData[i*this->m_NCols+j];
		}
	}
	return Matrix;
}
//----------------------------------------------------------------------------//
int CJetMatrix::GetNRows() const//取得有幾列
{
	return this->m_NRows;
}
//----------------------------------------------------------------------------//
int CJetMatrix::GetNCols() const//取得有幾行
{
	return this->m_NCols;
}
//----------------------------------------------------------------------------//
bool CJetMatrix::SaveMatrix(const char *pfilename)//儲存資料至檔案
{
	if ( m_pData == NULL ) { return false; }
	if ( m_NRows <= 0 ) { return false; }
	if ( m_NCols <= 0 ) { return false; }
	FILE *pfile = ::fopen(pfilename, "w+");
	if ( pfile == NULL ) { return false; }
	int i=0, j=0;
	for ( i=0; i<this->m_NRows; i++ )
	{
		for ( j=0; j<this->m_NCols; j++ )
		{
			::fprintf(pfile, "%.4f, ", this->GetElement(j, i));			
		}
		::fprintf(pfile, "\n");
	}
	::fprintf(pfile, "\n");
	::fclose(pfile);
	return true;
}
//----------------------------------------------------------------------------//