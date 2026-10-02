#ifndef _JET_MATRIX_H_
#define _JET_MATRIX_H_
//----------------------------------------------------------------------------//
#include <math.h>
#include <memory.h>
//----------------------------------------------------------------------------//
// n * m Matrix (Col=m, Row=n)
//| 11 12 13 14 1m |
//| 21 22 23 24 2m |
//| 31 32 33 34 3m |
//| n1 n2 n3 n4 nm |
//----------------------------------------------------------------------------//
class CJetMatrix
{
private:
	double *m_pData;//資料群
	int    m_NRows;//多少行
	int    m_NCols;//多少列	
protected:
	bool CloneMatrix(const CJetMatrix &Matrix);//複製函式
	void PreInitialize();//預先初始化
	void ClearDataBuffer();//清除資料群記憶體空間
public:
	//----------------------------------------------------------------------------//
	CJetMatrix();//建構子
	~CJetMatrix();//解構子
	CJetMatrix( const CJetMatrix &Matrix);//複製建構子
	//----------------------------------------------------------------------------//
	bool operator==(const CJetMatrix& Matrix);    //相等運算子
	bool operator!=(const CJetMatrix& Matrix);    //不相等運算子
	//運算子操作
	CJetMatrix &operator=(const CJetMatrix& Matrix);    //等於運算子
	CJetMatrix operator+(const CJetMatrix Matrix);//加上一個常數運算子
	CJetMatrix operator-(const CJetMatrix Matrix);//減掉一個常數運算子
	CJetMatrix operator*(const CJetMatrix &Matrix);//乘與矩陣

	CJetMatrix operator*(const double a);//乘與一個常數運算子
	CJetMatrix operator*(const float a);//乘與一個常數運算子
	CJetMatrix operator*(const int a);//乘與一個常數運算子
	CJetMatrix operator*(const long a);//乘與一個常數運算子

	CJetMatrix operator/(const double a);//除與一個常數運算子
	CJetMatrix operator/(const float a);//除與一個常數運算子
	CJetMatrix operator/(const int a);//除與一個常數運算子
	CJetMatrix operator/(const long a);//除與一個常數運算子

	CJetMatrix operator+(const double a);//加上一個常數運算子
	CJetMatrix operator+(const float a);//加上一個常數運算子
	CJetMatrix operator+(const int a);//加上一個常數運算子
	CJetMatrix operator+(const long a);//加上一個常數運算子

	CJetMatrix operator-(const double a);//減掉一個常數運算子
	CJetMatrix operator-(const float a);//減掉一個常數運算子
	CJetMatrix operator-(const int a);//減掉一個常數運算子
	CJetMatrix operator-(const long a);//減掉一個常數運算子	
	//----------------------------------------------------------------------------//
	void Initialize();//初始化
	bool SetMatrixSize(const int NRows, const int NCols, const double *pData=NULL);//設定矩陣大小
	//----------------------------------------------------------------------------//
	CJetMatrix Inversion();//逆矩陣
	CJetMatrix Unit();//單位矩陣
	CJetMatrix Transpose();//轉置矩陣
	//----------------------------------------------------------------------------//
	int Pivot(const int StartNRow);//軸樞法，輸入第a(i,i)的位置，往後尋找最大的值，並將他做成列轉換，回傳掉換的列數
	bool GetSubMatrix(const int x, const int y, CJetMatrix &Matrix) const;//取回子矩陣，扣除(x,y)後的矩陣
	bool GetDeterminant(double &Det);//取得行列式
	//----------------------------------------------------------------------------//
	int    GetNRows() const;//取得有幾列
	int    GetNCols() const;//取得有幾行
	double GetElement(int x, int y) const;//取得數值
	//----------------------------------------------------------------------------//
	bool   SaveMatrix(const char *pfilename);//儲存資料至檔案
	//----------------------------------------------------------------------------//
};
//----------------------------------------------------------------------------//
#endif