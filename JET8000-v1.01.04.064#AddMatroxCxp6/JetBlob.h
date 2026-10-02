// JetBlob.h: interface for the CJetBlob class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JETBLOB_H__5CB99576_4A69_45B0_B226_5EA4362A3CED__INCLUDED_)
#define AFX_JETBLOB_H__5CB99576_4A69_45B0_B226_5EA4362A3CED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include <vector>
//-------------------------------------------------------------------------------------//
const int BLOB_PEN_COLOR_NUMBER = 16;
//-------------------------------------------------------------------------------------//
enum BLOB_CONNECTIVITY_MODE
{
	BLOB_CONNECTIVITY_4 = 4,
	BLOB_CONNECTIVITY_8 = 8
};
//-------------------------------------------------------------------------------------//
enum BLOB_SORT_MODE
{
	BLOB_SORT_ID        = 1,
	BLOB_SORT_PIXELS    = 2,
	BLOB_SORT_RECT_SIZE = 3 //矩形
};
//-------------------------------------------------------------------------------------//
typedef struct _ScanLine
{
	int      m_Yidx;
	int      m_XidxS;
	int      m_XidxE;
	int      m_SLidx;
	int      m_BlobID;

	_ScanLine()
	{
		m_Yidx = -1;
		m_XidxS = -1;
		m_XidxE = -1;
		m_SLidx = -1;
		m_BlobID = -1;		
	}
} TScanLine, *PScanLine;
typedef std::vector<TScanLine> TScanLineVector;
//-------------------------------------------------------------------------------------//
typedef struct _BlobResult
{
	int     m_BlobID;
	int     m_BlobPixels;
	RECT    m_BlobRect;
	RECT    m_BlobRectRaw;
	double  m_BlobGCPosX;
	double  m_BlobGCPosY;

	_BlobResult()
	{
		m_BlobID = -1;;
		m_BlobPixels = 0;
		m_BlobRect.left = m_BlobRect.top = m_BlobRect.bottom = m_BlobRect.right = -1;
		m_BlobRectRaw.left = m_BlobRectRaw.top = m_BlobRectRaw.bottom = m_BlobRectRaw.right = -1;
		m_BlobGCPosX = 0;
		m_BlobGCPosY = 0;
	}

	int   GetBlobRectW() const
	{	return m_BlobRect.right-m_BlobRect.left;	}
	int   GetBlobRectH() const
	{	return m_BlobRect.bottom-m_BlobRect.top;	}
	double  GetBlobRectCpX() const
	{	return (m_BlobRect.right+m_BlobRect.left)*0.5;	}
	double  GetBlobRectCpY() const
	{	return (m_BlobRect.bottom+m_BlobRect.top)*0.5;	}
} TBlobResult, *PBlobResult;
typedef std::vector<TBlobResult> TBlobVector;
//-------------------------------------------------------------------------------------//
class CJetBlob  
{
	static BOOL                m_FirstBlob;
	static COLORREF            m_BlobPenClr[BLOB_PEN_COLOR_NUMBER];//色筆顏色
private:
	//---------------------------------------------------------------------------------//
	int                        m_ImageCpx;
	int                        m_ImageCpy;
	CString                    m_DeumpFolder;	
	BOOL                       m_DumpDebug;
	RECT                       m_RoiRect;//20230628-Blob
	BLOB_SORT_MODE             m_BlobSortMode;//排序方式
	BLOB_CONNECTIVITY_MODE     m_BlobConnectivity;//連接性
	TBlobVector                m_BlobList;
	TScanLineVector            m_ScanLineList;
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	void                       PreInitBlob();
	void                       InitialBlob();
	void                       CloneBlob(const CJetBlob &Blob);
	//---------------------------------------------------------------------------------//	
	bool                       CheckBlobPtr(TBlobResult *BlobPtr);
	//---------------------------------------------------------------------------------//	
	size_t                     CalcBufferSize(IMAGE_SIZE ImageStep, IMAGE_SIZE ImageH) const;//計算記憶體大小
	//---------------------------------------------------------------------------------//	
	bool                       CheckImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE BytePL, const unsigned char *pImage, const RECT &Roi);
	bool                       BuildScanLengthList(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE BytePL, const unsigned char *pImage, const RECT &Roi, int ThL, int ThH);
	bool                       BuildBlobList();
	//---------------------------------------------------------------------------------//	
	bool                       DumpScanLineList(LPCTSTR filename);
	//---------------------------------------------------------------------------------//	
public:
	bool                       SortBlobListByID();
	bool                       SortBlobListByPixels();
	bool                       SortBlobListByRectSize();
	bool                       SortBlobListByToward(int Toward, bool bAsc);
	//---------------------------------------------------------------------------------//	
	bool                       GetScanLinePtrListByID(int BlobID, std::vector<PScanLine> &List);
	//---------------------------------------------------------------------------------//	
	bool                       SortPairList(std::vector<std::pair<int, int>> &List);//排序Pair列表
	bool                       ReplacePairValue(std::vector<std::pair<int, int>> &List);//取代Pair數值	
	bool                       AddKeyRepeatPair(std::vector<std::pair<int, int>> &List);//加入重複Key	
	bool                       RemoveRepeatPair(std::vector<std::pair<int, int>> &List);//移除重複的Pair
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	CJetBlob();
	CJetBlob(const CJetBlob &Blob);
	virtual ~CJetBlob();
	CJetBlob& operator=(const CJetBlob &Blob);
	//---------------------------------------------------------------------------------//	
	bool                       GrayImageBlobDetect(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE BytePL, const unsigned char *pImage, int ThL, int ThH);	
	bool                       GrayImageRoiBlobDetect(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE BytePL, const unsigned char *pImage, const RECT &Roi, int ThL, int ThH);	
	//---------------------------------------------------------------------------------//
	bool                       DrawBlobResult(HDC hDC, int WndCPX, int WndCPY, double Zoom, int OffsetX, int OffsetY);
	//---------------------------------------------------------------------------------//
	void                       Clear();
	//---------------------------------------------------------------------------------//
	size_t                     GetBlobCount() const { return m_BlobList.size(); }
	TBlobResult*               GetBlobPtr(size_t index, bool check);
	void                       CloneBlobList(TBlobVector &BlobList);
	bool                       SortBlobList(BLOB_SORT_MODE SortMode);	
	bool                       GetBlobContour(TBlobResult *BlobPtr, std::vector<POINT> &PtList);	
	bool                       GetBlobContour(TBlobResult *BlobPtr, std::vector<TPOINT2D> &PtList);		
	bool                       GetBlobPixelList(TBlobResult *BlobPtr, std::vector<POINT> &PtList);		
	bool                       GetBlobPixelList(TBlobResult *BlobPtr, std::vector<cv::Point> &PtList);
	bool                       GetBlobPixelList(TBlobResult *BlobPtr, std::vector<cv::Point2f> &PtList);
	bool                       CalcBlobSubRect(TBlobResult *BlobPtr, const RECT &CalcRect, RECT &BlobRect);	//重新計算區塊內特定區域的位置	
	bool                       CheckBlobInDistRange(TBlobResult *BlobPtr, double CX, double CY, double Min, double Max);//確認Blob在這個範圍內
	bool                       GetBlobImage(TBlobResult *BlobPtr, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool clrImg=true);//在原來影像, 只繪製取到的blob資料
	bool                       GetBlobAveGray(TBlobResult *BlobPtr, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, IMAGE_DATA &Gray);//在原來影像, 計算取到的blob平均灰階
	bool                       FillBlobOutside(TBlobResult *BlobPtr, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, IMAGE_DATA FillGray);//在原來影像, 填滿取到的blob外部的圖
	bool                       FillBlobRectOutside(TBlobResult *BlobPtr, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, IMAGE_DATA FillGray);//在原來影像, 填滿取到的blob矩形外部的圖
	bool                       GetBlobSelfImage(TBlobResult *BlobPtr, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr);//取得單獨Blob影像
	//---------------------------------------------------------------------------------//
	size_t                     GetScanLineCount() const { return m_ScanLineList.size(); }
	TScanLine*                 GetScanLinePtr(size_t index, bool check);
	//---------------------------------------------------------------------------------//	
	void                       SetDumpDebug(BOOL value, LPCTSTR Directory);
	BOOL                       GetDumpDebug() const { return m_DumpDebug; }
	//---------------------------------------------------------------------------------//	
	void                       SetBlobSortMode(BLOB_SORT_MODE value) { m_BlobSortMode = value; }
	BLOB_SORT_MODE             GetBlobSortMode() const { return m_BlobSortMode; }
	//---------------------------------------------------------------------------------//
	void                       SetBlobConnectivity(BLOB_CONNECTIVITY_MODE value) { m_BlobConnectivity = value; }
	BLOB_CONNECTIVITY_MODE     GetBlobConnectivity() const { return m_BlobConnectivity; }
	//---------------------------------------------------------------------------------//	
	double                     CalcBlobLongShortRatio(double BlobW, double BlobH) const;
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_JETBLOB_H__5CB99576_4A69_45B0_B226_5EA4362A3CED__INCLUDED_)
