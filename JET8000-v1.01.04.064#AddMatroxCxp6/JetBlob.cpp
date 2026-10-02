// JetBlob.cpp: implementation of the CJetBlob class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "JetBlob.h"
//-------------------------------------------------------------------------------------//
#include "SortObj.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
const int TOWARD_UP     = 1;//朝上
const int TOWARD_LEFT   = 2;//朝左
const int TOWARD_DOWN   = 4;//朝下
const int TOWARD_RIGHT  = 8;//朝右
//-------------------------------------------------------------------------------------//
BOOL     CJetBlob::m_FirstBlob = TRUE;
COLORREF CJetBlob::m_BlobPenClr[BLOB_PEN_COLOR_NUMBER];
//-------------------------------------------------------------------------------------//
bool SortPairFn(std::pair<int, int> &a, std::pair<int, int> &b)
{
	if ( a.first==b.first )
	{	return (a.second<b.second); }
	return (a.first<b.first);
}
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CJetBlob::CJetBlob()
{
	PreInitBlob();
	InitialBlob();	
}
//-------------------------------------------------------------------------------------//
CJetBlob::CJetBlob(const CJetBlob &Blob)
{
	PreInitBlob();
	CloneBlob(Blob);
}
//-------------------------------------------------------------------------------------//
CJetBlob::~CJetBlob()
{

}
//-------------------------------------------------------------------------------------//
CJetBlob& CJetBlob::operator=(const CJetBlob &Blob)
{
	if ( this == &Blob ) { return *this; }
	CloneBlob(Blob);
	return *this;
}
//-------------------------------------------------------------------------------------//
void CJetBlob::Clear()
{
	m_BlobList.clear();
	m_ScanLineList.clear();
}
//-------------------------------------------------------------------------------------//
TBlobResult* CJetBlob::GetBlobPtr(size_t index, bool check)
{
	if ( true == check ) 
	{
		const size_t size = m_BlobList.size();
		if ( index >= size )
		{	return NULL; }
	}
	return &(m_BlobList[index]);
}
//-------------------------------------------------------------------------------------//
void CJetBlob::CloneBlobList(TBlobVector &BlobList)
{
	BlobList = m_BlobList;
}
//-------------------------------------------------------------------------------------//
void CJetBlob::PreInitBlob()
{
	if ( CJetBlob::m_FirstBlob == TRUE )
	{
		CJetBlob::m_BlobPenClr[ 0] = 0x0000FF;
		CJetBlob::m_BlobPenClr[ 1] = 0x00FF00;
		CJetBlob::m_BlobPenClr[ 2] = 0xFF2F2F;
		CJetBlob::m_BlobPenClr[ 3] = 0x00008F;
		CJetBlob::m_BlobPenClr[ 4] = 0x008F00;
		CJetBlob::m_BlobPenClr[ 5] = 0x8F2F2F;
		CJetBlob::m_BlobPenClr[ 6] = 0x00FFFF;
		CJetBlob::m_BlobPenClr[ 7] = 0xFFFF00;
		CJetBlob::m_BlobPenClr[ 8] = 0xFF00FF;
		CJetBlob::m_BlobPenClr[ 9] = 0x008F8F;
		CJetBlob::m_BlobPenClr[10] = 0x8F8F00;
		CJetBlob::m_BlobPenClr[11] = 0x8F008F;
		CJetBlob::m_BlobPenClr[12] = 0x0000AF;
		CJetBlob::m_BlobPenClr[13] = 0x00AF00;
		CJetBlob::m_BlobPenClr[14] = 0xAF2F2F;
		CJetBlob::m_BlobPenClr[15] = 0xFFFFFF;
		CJetBlob::m_FirstBlob = FALSE;
	}
}
//-------------------------------------------------------------------------------------//
void CJetBlob::InitialBlob()
{
	m_ImageCpx = 0;
	m_ImageCpy = 0;
	m_DeumpFolder = _T("C:");
	m_DumpDebug = FALSE;	
	m_RoiRect.left=m_RoiRect.right=0;//20230628-Blob
	m_RoiRect.top=m_RoiRect.bottom=0;//20230628-Blob
	m_BlobSortMode = BLOB_SORT_ID;
	m_BlobConnectivity = BLOB_CONNECTIVITY_4;
	m_BlobList.clear();
	m_ScanLineList.clear();
}
//-------------------------------------------------------------------------------------//
void CJetBlob::CloneBlob(const CJetBlob &Blob)
{
	m_ImageCpx = Blob.m_ImageCpx;
	m_ImageCpy = Blob.m_ImageCpy;
	m_DeumpFolder = Blob.m_DeumpFolder;
	m_DumpDebug = Blob.m_DumpDebug;	
	m_RoiRect = Blob.m_RoiRect;//20230628-Blob
	m_BlobSortMode = Blob.m_BlobSortMode;
	m_BlobConnectivity = Blob.m_BlobConnectivity;
	m_BlobList     = Blob.m_BlobList;
	m_ScanLineList = Blob.m_ScanLineList;
}
//-------------------------------------------------------------------------------------//
TScanLine* CJetBlob::GetScanLinePtr(size_t index, bool check)
{
	if ( true == check ) 
	{
		const size_t size = m_ScanLineList.size();
		if ( index >= size )
		{	return NULL; }
	}
	return &(m_ScanLineList[index]);
}
//-------------------------------------------------------------------------------------//
void CJetBlob::SetDumpDebug(BOOL value, LPCTSTR Directory)
{	
	m_DumpDebug = value; 
	if ( TRUE == value  )
	{
		if ( NULL == Directory ) { return; }
		this->m_DeumpFolder = Directory;
	}
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::GrayImageBlobDetect(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE BytePL, const unsigned char *pImage, int ThL, int ThH)
{
	bool IsOK = true;
	RECT RoiRect={0,0,0,0};
	RoiRect.left=0; RoiRect.right=(int)(ImageW);
	RoiRect.top=0; RoiRect.bottom=(int)(ImageH);
	IsOK = GrayImageRoiBlobDetect(ImageW, ImageH, BytePL, pImage, RoiRect, ThL, ThH);
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::GrayImageRoiBlobDetect(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE BytePL, const unsigned char *pImage, const RECT &Roi, int ThL, int ThH)
{
	this->m_BlobList.clear();
	this->m_ScanLineList.clear();

	if ( this->CheckImage(ImageW, ImageH, BytePL, pImage, Roi) == false )
	{	return false; }

	if ( this->BuildScanLengthList(ImageW, ImageH, BytePL, pImage, Roi, ThL, ThH) == false )
	{	return false; }	
	
	if ( this->BuildBlobList() == false )
	{	return false; }

	switch ( m_BlobSortMode )
	{
	case BLOB_SORT_PIXELS:
		if ( SortBlobListByPixels() == false )
		{	return false; }
		break;
	case BLOB_SORT_RECT_SIZE:
		if ( SortBlobListByRectSize() == false )
		{	return false; }
		break;
	default:
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::CheckBlobPtr(TBlobResult *BlobPtr)
{
	if ( NULL == BlobPtr )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
size_t CJetBlob::CalcBufferSize(IMAGE_SIZE ImageStep, IMAGE_SIZE ImageH) const//計算記憶體大小
{
	return (size_t)(ImageStep)*(size_t)(ImageH);
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::CheckImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE BytePL, const unsigned char *pImage, const RECT &Roi)
{	
	if ( pImage==NULL || ImageW==0 || ImageH==0 || BytePL==0 || BytePL<ImageW ) { return false; }
	const int nImageW = (int)(ImageW);
	const int nImageH = (int)(ImageH);
	if ( Roi.left<0 || Roi.top<0 || Roi.right>nImageW || Roi.bottom>nImageH ) { return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::BuildScanLengthList(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE BytePL, const unsigned char *pImage, const RECT &Roi, int ThL, int ThH)
{	
	size_t      i=0, j=0, idx=0;
	TScanLine   ScanLine;
	TScanLine  *pScanLine=NULL;
	
	m_RoiRect = Roi;//20230628-Blob
	m_ImageCpx = (ImageW+1)/2;
	m_ImageCpy = (ImageH+1)/2;
	ScanLine.m_Yidx = -1;		
	ScanLine.m_XidxS = ScanLine.m_XidxE = -1;		
	for ( i=Roi.top; i<Roi.bottom; i++ )
	{
		idx = i*BytePL;		
		ScanLine.m_Yidx = i;
		for ( j=Roi.left; j<Roi.right; j++ )
		{
			if ( pImage[idx+j]<ThL ||  pImage[idx+j]>ThH ) 
			{
				if ( ScanLine.m_XidxS >= 0 ) 
				{	
					//ScanLine.m_XidxE = j; 
					ScanLine.m_XidxE = j-1;//20230628-Blob
					ScanLine.m_SLidx = (int)(this->m_ScanLineList.size());					
					this->m_ScanLineList.push_back(ScanLine);
					ScanLine.m_XidxS = ScanLine.m_XidxE = -1;					
				}
				continue; 			
			}
			if ( ScanLine.m_XidxS < 0 )
			{	ScanLine.m_XidxS = j; }
		}
		if ( ScanLine.m_XidxS >= 0 ) 
		{	
			//ScanLine.m_XidxE = j; 
			ScanLine.m_XidxE = j-1;//20230628-Blob
			ScanLine.m_SLidx = (int)(this->m_ScanLineList.size());
			this->m_ScanLineList.push_back(ScanLine);
			ScanLine.m_Yidx = -1;
			ScanLine.m_XidxS = ScanLine.m_XidxE = -1;		
		}		
	}

#ifdef _DEBUG	
	CString filename;
	filename.Format(_T("%s\\%s"), m_DeumpFolder, _T("BlobScanLength.txt"));
	this->DumpScanLineList(filename);
#endif
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::DumpScanLineList(LPCTSTR filename)
{
#ifdef _DEBUG	
	if ( FALSE == this->m_DumpDebug ) 
	{	return true; }

	TCHAR   TMode[32] = _T("");
	_tcscpy(TMode, _T("w+"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	FILE *pfile = ::_tfopen(filename, TMode);
	if ( NULL == pfile )
	{	return false; }

	size_t i=0;
	TScanLine  *pScanLine=NULL;
	const size_t Size = m_ScanLineList.size();
	::_ftprintf(pfile, _T("idx, Yidx, X-Start, X-End, Length, BlobID\n"));
	for ( i=0; i<Size; i++ )
	{
		pScanLine = &(m_ScanLineList[i]);
		::_ftprintf(pfile, _T("%d, %d, %d, %d, %d, %d\n"),
			i+1,
			pScanLine->m_Yidx,
			pScanLine->m_XidxS,
			pScanLine->m_XidxE,
			pScanLine->m_XidxE-pScanLine->m_XidxS,
			pScanLine->m_BlobID );
	}
	::fclose(pfile); pfile=NULL;		
#endif
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::BuildBlobList()
{
	CString filename;	
	int     i=0, j=0, k=0;
	int     LoopCount = 0;
	bool Finish = false;
	size_t  CurYidx=0;	
	size_t  LastYidx=0;	
	size_t  NextYidx=0;	
	int OldBlobID = 0;
	int NewBlobID = 0;
	int XidxS=0;
	int XidxE=0;	
	int XLen = 0;
	int YLen = 0;
	double XCpx = 0;
	TScanLine  *pScanLine=NULL;
	TScanLine  *pOtherScanLine=NULL;
	TScanLine  *pLastScanLine=NULL;
	TScanLine  *pNextScanLine=NULL;
	const int LimitLeft=m_RoiRect.left;//20230628-Blob
	const int LimitRight=m_RoiRect.right-1;//20230628-Blob
	const int ScanLineCount = (int)(this->m_ScanLineList.size());
	if ( ScanLineCount == 0 ) { return true; }

	const size_t PreBlobIDCount=(size_t)(ScanLineCount/2);
	std::vector<std::pair<int, int>> IDPairList(PreBlobIDCount);	
	IDPairList.clear();	

	size_t BlobIDUsed=0;
	std::vector<int> BlobIDList;	

	//由上往下搜尋		
	Finish = true;
	CurYidx = -1;
	for ( i=0; i<ScanLineCount; i++ )
	{
		pScanLine = &(this->m_ScanLineList[i]);			
		CurYidx = pScanLine->m_Yidx;
		NextYidx = CurYidx+1;
		if ( pScanLine->m_BlobID < 0 ) 
		{
			pScanLine->m_BlobID = BlobIDUsed;
			BlobIDUsed ++;	
		}

		if ( BLOB_CONNECTIVITY_8 == m_BlobConnectivity )
		{
			XidxS = pScanLine->m_XidxS;
			XidxE = pScanLine->m_XidxE;
			if ( XidxS > LimitLeft ) { XidxS --; }//20230628-Blob
			if ( XidxE < LimitRight ) { XidxE ++; }//20230628-Blob
		}
		else
		{
			//XidxS = pScanLine->m_XidxS+1;
			//XidxE = pScanLine->m_XidxE-1;
			XidxS = pScanLine->m_XidxS;//20230628-Blob
			XidxE = pScanLine->m_XidxE;//20230628-Blob
		}
		

		for ( j=i+1; j<ScanLineCount; j++ )
		{
			pNextScanLine = &(this->m_ScanLineList[j]);				
			if ( pNextScanLine->m_Yidx >  NextYidx ) { break; }
			
			if ( pNextScanLine->m_Yidx == CurYidx ) { continue; }
			if ( pNextScanLine->m_XidxE < XidxS ) { continue; }
			if ( pNextScanLine->m_XidxS > XidxE ) { continue; }

			if ( pNextScanLine->m_BlobID < 0 ) 
			{	
				pNextScanLine->m_BlobID = pScanLine->m_BlobID; 
				continue;
			}
			if ( pNextScanLine->m_BlobID == pScanLine->m_BlobID ) 
			{	continue; }
				
			if ( pNextScanLine->m_BlobID < pScanLine->m_BlobID )
			{
				OldBlobID = pScanLine->m_BlobID;
				NewBlobID = pNextScanLine->m_BlobID;
			}
			else
			{
				OldBlobID = pNextScanLine->m_BlobID;
				NewBlobID = pScanLine->m_BlobID;			
			}
			IDPairList.push_back(std::make_pair(OldBlobID, NewBlobID));
		}
	}	

	BlobIDList.resize(BlobIDUsed);
	for ( i=0; i<BlobIDUsed; i++ )
	{	BlobIDList[i] = i; }

	while ( true )
	{
		SortPairList(IDPairList);//由小到大排序

		//取代Pair數值-連結BlobID-花最多時間
		ReplacePairValue(IDPairList);

		RemoveRepeatPair(IDPairList);//移除重複的Pair對
		if ( AddKeyRepeatPair(IDPairList) == false )
		{	break; }
	};
	const size_t IDPairCnt=IDPairList.size();		
	for ( i=0; i<IDPairCnt; i++ )
	{	BlobIDList[IDPairList[i].first]=IDPairList[i].second;	}
	for ( i=0; i<ScanLineCount; i++ )
	{
		pScanLine = &(this->m_ScanLineList[i]);			
		pScanLine->m_BlobID=BlobIDList[pScanLine->m_BlobID];
	}

#ifdef _DEBUG
	filename.Format(_T("%s\\%s#%d.txt"), m_DeumpFolder, _T("BuildBlobID"), LoopCount+1);
	this->DumpScanLineList(filename);
#endif
	
	
	TBlobResult  Blob;	
	TBlobResult *pBlob=NULL;
	const size_t BlobIDCount = BlobIDList.size();
	//取得有使用的BlobIndex;
	for ( i=0; i<BlobIDCount; i++ )
	{	BlobIDList[i] = 0;	}
	for ( i=0; i<ScanLineCount; i++ )
	{	BlobIDList[m_ScanLineList[i].m_BlobID] ++;	}

	NewBlobID = 0;
	for ( i=0; i<BlobIDCount; i++ )
	{
		if ( BlobIDList[i] == 0 )//沒有使用的BlobID
		{	
			BlobIDList[i] = -1;//將編號設定為-1
			continue;
		}
		BlobIDList[i] = NewBlobID;
		NewBlobID ++;
	}
	//取得最後的Blob的數量
	const size_t BlobCount = NewBlobID;
	m_BlobList.resize(NewBlobID);//直接給予陣列數量
	m_BlobList.clear();
	Blob = TBlobResult();//建構子來初始化
	for ( i=0; i<BlobCount; i++ )
	{	
		Blob.m_BlobID = i;//初始化		
		m_BlobList.push_back(Blob);
	}		
	for ( i=0; i<ScanLineCount; i++ )
	{
		pScanLine = &(this->m_ScanLineList[i]);
		pScanLine->m_BlobID = BlobIDList[pScanLine->m_BlobID];
	#ifdef _DEBUG
		if ( pScanLine->m_BlobID < 0 ) 
		{	
			::AfxMessageBox(_T("Error, Blob ID Exception")); 
			break;
		}
	#endif

		pBlob = &(m_BlobList[pScanLine->m_BlobID]);	
		
		//面積 - 像素數量
		XCpx = pScanLine->m_XidxE+pScanLine->m_XidxS;		
		//XLen = pScanLine->m_XidxE-pScanLine->m_XidxS;
		XLen = pScanLine->m_XidxE-pScanLine->m_XidxS+1;//20230628-Blob
		pBlob->m_BlobPixels += XLen;

		//邊界範圍
		if ( pBlob->m_BlobRect.left < 0 ) 
		{
			pBlob->m_BlobRect.top = pScanLine->m_Yidx;
			//pBlob->m_BlobRect.bottom = pBlob->m_BlobRect.top+1;
			pBlob->m_BlobRect.bottom = pBlob->m_BlobRect.top;//20230628-Blob
			pBlob->m_BlobRect.left = pScanLine->m_XidxS;
			pBlob->m_BlobRect.right = pScanLine->m_XidxE;
		}
		else
		{
			if ( pBlob->m_BlobRect.top > pScanLine->m_Yidx )
			{	pBlob->m_BlobRect.top = pScanLine->m_Yidx; }
			//if ( pBlob->m_BlobRect.bottom < (pScanLine->m_Yidx+1) )
			//{	pBlob->m_BlobRect.bottom = (pScanLine->m_Yidx+1); }
			if ( pBlob->m_BlobRect.bottom < (pScanLine->m_Yidx) )//20230628-Blob
			{	pBlob->m_BlobRect.bottom = (pScanLine->m_Yidx); }
				
			if ( pBlob->m_BlobRect.left > pScanLine->m_XidxS )
			{	pBlob->m_BlobRect.left = pScanLine->m_XidxS; }
			if ( pBlob->m_BlobRect.right < (pScanLine->m_XidxE) )
			{	pBlob->m_BlobRect.right = (pScanLine->m_XidxE); }
		}

		//計算重心
		pBlob->m_BlobGCPosX += ((XCpx)*XLen);
		pBlob->m_BlobGCPosY += ((pScanLine->m_Yidx)*XLen);	
	}
	
	for ( i=0; i<BlobCount; i++ )
	{	
		pBlob = &(m_BlobList[i]);

		//因為使用上往往不包含本身, 所以外擴1個像素		
		pBlob->m_BlobRectRaw = pBlob->m_BlobRect;
		pBlob->m_BlobRect.right += 1;
		pBlob->m_BlobRect.bottom += 1;
		pBlob->m_BlobGCPosX = pBlob->m_BlobGCPosX/(2*pBlob->m_BlobPixels);
		pBlob->m_BlobGCPosY = pBlob->m_BlobGCPosY/pBlob->m_BlobPixels;
		pBlob->m_BlobGCPosX += 0.5;//20230628-Blob
		pBlob->m_BlobGCPosY += 0.5;//20230628-Blob
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::DrawBlobResult(HDC hDC, int WndCPX, int WndCPY, double Zoom, int OffsetX, int OffsetY)
{
	const size_t BlobCount = this->GetBlobCount();
	if ( 0 == BlobCount ) 
	{	return true; }

	int    ClrID = 0;
	int    BlobID = 0;		
	RECT   BlobRect={0};
	POINT  BlobCP={0};
	size_t i=0, j=0;
	HPEN   hPen = NULL;
	HPEN   hOldPen = NULL;
	TBlobResult *pBlob=NULL;
	for ( i=0; i<BlobCount; i++ )
	{
		pBlob = this->GetBlobPtr(i, false);
		if ( NULL == pBlob ) { continue; }
		BlobID = pBlob->m_BlobID;
		ClrID = BlobID%BLOB_PEN_COLOR_NUMBER;
		
		hPen = ::CreatePen(PS_SOLID, 1, CJetBlob::m_BlobPenClr[ClrID]);
		if ( NULL != hPen )
		{
			BlobRect = pBlob->m_BlobRect;
			
			::OffsetRect(&BlobRect, -m_ImageCpx+OffsetX, -m_ImageCpy+OffsetY);
			BlobRect.left = (long)(BlobRect.left/Zoom);
			BlobRect.top  = (long)(BlobRect.top/Zoom);
			BlobRect.right = (long)(BlobRect.right/Zoom);
			BlobRect.bottom = (long)(BlobRect.bottom/Zoom);
			::OffsetRect(&BlobRect, WndCPX, WndCPY);
			hOldPen = (HPEN)::SelectObject(hDC, hPen);

			::MoveToEx(hDC, BlobRect.left, BlobRect.top, NULL);
			::LineTo(hDC, BlobRect.right, BlobRect.top);
			::LineTo(hDC, BlobRect.right, BlobRect.bottom);
			::LineTo(hDC, BlobRect.left, BlobRect.bottom);
			::LineTo(hDC, BlobRect.left, BlobRect.top);
			
			::SelectObject(hDC, hOldPen);
			::DeleteObject(hPen); hPen = NULL;
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::SortBlobList(BLOB_SORT_MODE SortMode)
{
	bool IsOK = true;
	switch ( SortMode )
	{
	case BLOB_SORT_ID:
		IsOK = SortBlobListByID();
		break;
	case BLOB_SORT_PIXELS:
		IsOK = SortBlobListByPixels();
		break;
	case BLOB_SORT_RECT_SIZE:
		IsOK = SortBlobListByRectSize();
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::GetBlobContour(TBlobResult *BlobPtr, std::vector<POINT> &PtList)
{
	if ( CheckBlobPtr(BlobPtr) == false )
	{	return false; }	

	POINT        Pt;
	int          k=0;
	int          XS=0, XE=0;
	size_t       i=0, j=0;	
	TScanLine   *pScanLine=NULL;
	TScanLine   *pScanLine2=NULL;
	TScanLine   *pScanLineT=NULL;
	TScanLine   *pScanLineB=NULL;
	std::vector<size_t> ScanLineListT;
	std::vector<size_t> ScanLineListB;
	std::vector<PScanLine> ScanLinePtrList;
	GetScanLinePtrListByID(BlobPtr->m_BlobID, ScanLinePtrList);
	const size_t ScanLineCount = ScanLinePtrList.size();

	PtList.clear();
	for ( i=0; i<ScanLineCount; i++ )
	{	
		pScanLine = (ScanLinePtrList[i]);
		if ( NULL == pScanLine ) { continue; }
		if ( pScanLine->m_BlobID != BlobPtr->m_BlobID ) { continue; }						
		ScanLineListT.clear();
		ScanLineListB.clear();
		for ( j=0; j<ScanLineCount; j++ )
		{
			if ( j == i ) { continue; }
			pScanLine2 = (ScanLinePtrList[j]);
			if ( NULL == pScanLine2 ) { continue; }
			if ( pScanLine2->m_BlobID != BlobPtr->m_BlobID ) { continue; }
			if ( pScanLine2->m_Yidx == (pScanLine->m_Yidx-1) )
			{	ScanLineListT.push_back(j);	}
			if ( pScanLine2->m_Yidx == (pScanLine->m_Yidx+1) )
			{	ScanLineListB.push_back(j);	}
		}

		bool  bTop=false;
		bool  bBot=false;
		const size_t ScanLineCountT = ScanLineListT.size();
		const size_t ScanLineCountB = ScanLineListB.size();

		Pt.y = pScanLine->m_Yidx;
		for ( k=pScanLine->m_XidxS; k<=pScanLine->m_XidxE; k++ )
		{	
			if ( k==pScanLine->m_XidxS || k==pScanLine->m_XidxE ) 
			{
				Pt.x = k;
				PtList.push_back(Pt);
				continue;
			}
			bTop = false;
			for ( j=0; j<ScanLineCountT; j++ )
			{
				pScanLine2 = (ScanLinePtrList[ScanLineListT[j]]);
				if ( k < pScanLine2->m_XidxS ) { continue; }
				if ( k > pScanLine2->m_XidxE ) { continue; }
				bTop = true;
				break;
			}
			if ( false == bTop )
			{
				Pt.x = k;
				PtList.push_back(Pt);
				continue;
			}

			bBot = false;
			for ( j=0; j<ScanLineCountB; j++ )
			{
				pScanLine2 = (ScanLinePtrList[ScanLineListB[j]]);
				if ( k < pScanLine2->m_XidxS ) { continue; }
				if ( k > pScanLine2->m_XidxE ) { continue; }
				bBot = true;
				break;
			}
			if ( false == bBot )
			{
				Pt.x = k;
				PtList.push_back(Pt);
				continue;
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::GetBlobContour(TBlobResult *BlobPtr, std::vector<TPOINT2D> &PtList)
{
	if ( CheckBlobPtr(BlobPtr) == false )
	{	return false; }	

	POINT        Pt;
	int          k=0;
	int          XS=0, XE=0;
	size_t       i=0, j=0;	
	TScanLine   *pScanLine=NULL;
	TScanLine   *pScanLine2=NULL;
	TScanLine   *pScanLineT=NULL;
	TScanLine   *pScanLineB=NULL;
	std::vector<size_t> ScanLineListT;
	std::vector<size_t> ScanLineListB;
	std::vector<PScanLine> ScanLinePtrList;
	GetScanLinePtrListByID(BlobPtr->m_BlobID, ScanLinePtrList);
	const size_t ScanLineCount = ScanLinePtrList.size();

	PtList.clear();
	for ( i=0; i<ScanLineCount; i++ )
	{	
		pScanLine = (ScanLinePtrList[i]);
		if ( NULL == pScanLine ) { continue; }
		if ( pScanLine->m_BlobID != BlobPtr->m_BlobID ) { continue; }						
		ScanLineListT.clear();
		ScanLineListB.clear();
		for ( j=0; j<ScanLineCount; j++ )
		{
			if ( j == i ) { continue; }
			pScanLine2 = (ScanLinePtrList[j]);
			if ( NULL == pScanLine2 ) { continue; }
			if ( pScanLine2->m_BlobID != BlobPtr->m_BlobID ) { continue; }
			if ( pScanLine2->m_Yidx == (pScanLine->m_Yidx-1) )
			{	ScanLineListT.push_back(j);	}
			if ( pScanLine2->m_Yidx == (pScanLine->m_Yidx+1) )
			{	ScanLineListB.push_back(j);	}
		}

		bool  bTop=false;
		bool  bBot=false;
		const size_t ScanLineCountT = ScanLineListT.size();
		const size_t ScanLineCountB = ScanLineListB.size();

		Pt.y = pScanLine->m_Yidx;
		for ( k=pScanLine->m_XidxS; k<=pScanLine->m_XidxE; k++ )
		{	
			if ( k==pScanLine->m_XidxS || k==pScanLine->m_XidxE ) 
			{
				Pt.x = k;
				PtList.push_back(Pt);
				continue;
			}
			bTop = false;
			for ( j=0; j<ScanLineCountT; j++ )
			{
				pScanLine2 = (ScanLinePtrList[ScanLineListT[j]]);
				if ( k < pScanLine2->m_XidxS ) { continue; }
				if ( k > pScanLine2->m_XidxE ) { continue; }
				bTop = true;
				break;
			}
			if ( false == bTop )
			{
				Pt.x = k;
				PtList.push_back(Pt);
				continue;
			}

			bBot = false;
			for ( j=0; j<ScanLineCountB; j++ )
			{
				pScanLine2 = (ScanLinePtrList[ScanLineListB[j]]);
				if ( k < pScanLine2->m_XidxS ) { continue; }
				if ( k > pScanLine2->m_XidxE ) { continue; }
				bBot = true;
				break;
			}
			if ( false == bBot )
			{
				Pt.x = k;
				PtList.push_back(Pt);
				continue;
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::GetBlobPixelList(TBlobResult *BlobPtr, std::vector<POINT> &PtList)
{
	if ( CheckBlobPtr(BlobPtr) == false )
	{	return false; }	

	POINT       Pt;
	int         j=0;
	size_t      i=0;	
	TScanLine  *pScanLine=NULL;
	const size_t ScanLineCount = m_ScanLineList.size();

	PtList.clear();
	for ( i=0; i<ScanLineCount; i++ )
	{	
		pScanLine = &(m_ScanLineList[i]);
		if ( NULL == pScanLine ) { continue; }
		if ( pScanLine->m_BlobID != BlobPtr->m_BlobID ) { continue; }
		Pt.y = pScanLine->m_Yidx;
		for ( j=pScanLine->m_XidxS; j<=pScanLine->m_XidxE; j++ )
		{	
			Pt.x = j;
			PtList.push_back(Pt);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::GetBlobPixelList(TBlobResult *BlobPtr, std::vector<cv::Point> &PtList)
{
	if ( CheckBlobPtr(BlobPtr) == false )
	{	return false; }	

	cv::Point   Pt;
	int         j=0;
	size_t      i=0;	
	TScanLine  *pScanLine=NULL;
	const size_t ScanLineCount = m_ScanLineList.size();

	PtList.clear();
	for ( i=0; i<ScanLineCount; i++ )
	{	
		pScanLine = &(m_ScanLineList[i]);
		if ( NULL == pScanLine ) { continue; }
		if ( pScanLine->m_BlobID != BlobPtr->m_BlobID ) { continue; }
		Pt.y = pScanLine->m_Yidx;
		for ( j=pScanLine->m_XidxS; j<=pScanLine->m_XidxE; j++ )
		{	
			Pt.x = j;
			PtList.push_back(Pt);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::GetBlobPixelList(TBlobResult *BlobPtr, std::vector<cv::Point2f> &PtList)
{
	if ( CheckBlobPtr(BlobPtr) == false )
	{	return false; }	

	cv::Point2f Pt;
	int         j=0;
	size_t      i=0;	
	TScanLine  *pScanLine=NULL;
	const size_t ScanLineCount = m_ScanLineList.size();

	PtList.clear();
	for ( i=0; i<ScanLineCount; i++ )
	{	
		pScanLine = &(m_ScanLineList[i]);
		if ( NULL == pScanLine ) { continue; }
		if ( pScanLine->m_BlobID != BlobPtr->m_BlobID ) { continue; }
		Pt.y = pScanLine->m_Yidx;
		for ( j=pScanLine->m_XidxS; j<=pScanLine->m_XidxE; j++ )
		{	
			Pt.x = j;
			PtList.push_back(Pt);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::CalcBlobSubRect(TBlobResult *BlobPtr, const RECT &CalcRect, RECT &BlobRect)//重新計算區塊內特定區域的位置
{
	if ( CheckBlobPtr(BlobPtr) == false )
	{	return false; }	

	size_t      i=0;	
	RECT        Rect;
	int         PosY=0;
	int         PosX_S=0;
	int         PosX_E=0;
	int         Left=0, Right=0;
	TScanLine  *pScanLine=NULL;
	const size_t ScanLineCount = m_ScanLineList.size();

	Rect.left = INT_MAX;
	Rect.top = INT_MAX;
	Rect.right = -1;	
	Rect.bottom = -1;
	BlobRect = BlobPtr->m_BlobRect;
	for ( i=0; i<ScanLineCount; i++ )
	{	
		pScanLine = &(m_ScanLineList[i]);
		if ( NULL == pScanLine ) { continue; }
		if ( pScanLine->m_BlobID != BlobPtr->m_BlobID ) { continue; }
		PosY = pScanLine->m_Yidx;
		if ( PosY < CalcRect.top ) { continue; }
		if ( PosY > CalcRect.bottom ) { continue; }
		
		PosX_S = pScanLine->m_XidxS;
		PosX_E = pScanLine->m_XidxE;
		if ( PosX_S > CalcRect.right ) { continue; }
		if ( PosX_E < CalcRect.left ) { continue; }
		
		Left  = MAX(CalcRect.left, PosX_S);
		Right = MIN(CalcRect.right, PosX_E);
		if ( Rect.top > PosY ) { Rect.top = PosY; }
		if ( Rect.bottom < PosY ) { Rect.bottom = PosY; }
		if ( Rect.left > Left ) { Rect.left = Left; }	
		if ( Rect.right< Right ) { Rect.right = Right; }
	}
	if ( -1 != Rect.right ) 
	{	BlobRect = Rect; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::CheckBlobInDistRange(TBlobResult *BlobPtr, double CX, double CY, double Min, double Max)//確認Blob在這個範圍內
{	
	if ( CheckBlobPtr(BlobPtr) == false )
	{	return false; }	
	
	int        i=0, j=0;	
	double     dY=0, dX=0, dR=0;
	TScanLine *pScanLine=NULL;			
	const int  ScanLineCount = (int)(m_ScanLineList.size());

	for ( i=0; i<ScanLineCount; i++ )
	{	
		pScanLine = &(m_ScanLineList[i]);
		if ( NULL == pScanLine ) { continue; }
		if ( pScanLine->m_BlobID != BlobPtr->m_BlobID ) { continue; }

		dY = pScanLine->m_Yidx-CY;
		for ( j=pScanLine->m_XidxS; j<=pScanLine->m_XidxE; j++ )
		{
			dX = j-CX;
			dR = sqrt((dX*dX)+(dY*dY));
			if ( dR < Min ) { continue; }
			if ( dR > Max ) { continue; }
			return true;
		}		
	}
	return false;

}
//-------------------------------------------------------------------------------------//
bool CJetBlob::GetBlobImage(TBlobResult *BlobPtr, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, bool clrImg)
{	
	if ( NULL == ImagePtr ) { return false; }
	if ( CheckBlobPtr(BlobPtr) == false )
	{	return false; }	
	
	size_t      i=0, j=0;
	size_t      Left=0, Right=0, Range=0;
	TScanLine  *pScanLine=NULL;	
	const size_t MinX=0;
	const size_t MaxX=ImageW-1;
	const size_t BufferSize=CalcBufferSize(ImageStep, ImageH);
	const size_t ScanLineCount = m_ScanLineList.size();
	
	if ( true == clrImg )
	{	::memset(ImagePtr, 0x00, sizeof(IMAGE_DATA)*BufferSize); }
	for ( i=0; i<ScanLineCount; i++ )
	{	
		pScanLine = &(m_ScanLineList[i]);
		if ( NULL == pScanLine ) { continue; }
		if ( pScanLine->m_BlobID != BlobPtr->m_BlobID ) { continue; }
		Left = MAX(0, pScanLine->m_XidxS);
		Right = MIN(MaxX, pScanLine->m_XidxE+1);
		if ( 24 == BitCount )
		{
			Range = (Right-Left)*3;
			j = (pScanLine->m_Yidx*ImageStep)+(Left*3);
		}
		else
		{
			Range = Right-Left;
			j = (pScanLine->m_Yidx*ImageStep)+Left;
		}
		::memset(&(ImagePtr[j]), 0xFF, sizeof(IMAGE_DATA)*Range);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::GetBlobAveGray(TBlobResult *BlobPtr, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, IMAGE_DATA &Gray)//在原來影像, 計算取到的blob平均灰階
{
	if ( NULL == ImagePtr ) { return false; }
	if ( CheckBlobPtr(BlobPtr) == false )
	{	return false; }	
	
	double      Sum=0;
	size_t      Count=0;
	size_t      i=0, j=0, k=0, idx=0;
	size_t      Left=0, Right=0, Range=0;
	TScanLine  *pScanLine=NULL;	
	const size_t MinX=0;
	const size_t MaxX=ImageW-1;
	const size_t BufferSize=CalcBufferSize(ImageStep, ImageH);
	const size_t ScanLineCount = m_ScanLineList.size();
	for ( i=0; i<ScanLineCount; i++ )
	{	
		pScanLine = &(m_ScanLineList[i]);
		if ( NULL == pScanLine ) { continue; }
		if ( pScanLine->m_BlobID != BlobPtr->m_BlobID ) { continue; }
		Left = MAX(0, pScanLine->m_XidxS);
		Right = MIN(MaxX, pScanLine->m_XidxE);		
		if ( 24 == BitCount )
		{
			Left  *= 3;
			Right *= 3;
		}
		k = pScanLine->m_Yidx*ImageStep;
		for ( j=Left; j<=Right; j++ )
		{
			idx = (k)+(j);
			Sum += ImagePtr[idx];
			Count ++;
		}		
	}
	if ( 0 == Count )
	{	Gray = 0;		}
	else
	{
		double Ave=Sum/Count;
		if ( Ave > 255 ) { Gray = 255; }
		else if ( Ave < 0.0 ) { Gray = 0; }
		else
		{	Gray = (unsigned char)(Ave); }
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::FillBlobOutside(TBlobResult *BlobPtr, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, IMAGE_DATA FillGray)//在原來影像, 填滿取到的blob外部的圖
{
	if ( NULL == ImagePtr ) { return false; }
	if ( CheckBlobPtr(BlobPtr) == false )
	{	return false; }	
	
	size_t      i=0, j=0, k=0, idx=0;
	int         left=0, right=0;
	int         Left=0, Right=0, Range=0;
	unsigned char Gray=FillGray;
	TScanLine  *pScanLine=NULL;	
	const size_t MinX=0;
	const size_t MaxX=ImageW-1;
	const size_t MaxY=ImageH-1;
	const size_t BufferSize=CalcBufferSize(ImageStep, ImageH);	
	const size_t ScanLineCount = m_ScanLineList.size();	
	std::vector<TScanLine*> ScanLinePtrList;
	GetScanLinePtrListByID(BlobPtr->m_BlobID, ScanLinePtrList);	
	const size_t ScanLinePtrCount = ScanLinePtrList.size();
	if ( 0 == ScanLinePtrCount ) { return true; }
	
	RECT BlobRect = BlobPtr->m_BlobRect;
	for ( i=0; i<ImageH; i++ )
	{
		idx = i*ImageStep;		
		if ( i<BlobRect.top || i>=BlobRect.bottom ) 
		{
			Range = ImageStep;
			::memset(&(ImagePtr[idx]), Gray, sizeof(IMAGE_DATA)*Range);
		}		
	}
	//return true;

	//由上下, 找出每條水平線的左右邊線
	for ( i=BlobRect.top; i<=BlobRect.bottom; i++ )
	{
		Left = Right = -1;
		for ( j=0; j<ScanLinePtrCount; j++ )
		{	
			pScanLine = ScanLinePtrList[j];
			if ( NULL == pScanLine ) { continue; }
			if ( i != pScanLine->m_Yidx ) { continue; }

			left = MAX(0, pScanLine->m_XidxS);
			right = MIN(MaxX, pScanLine->m_XidxE);			
			if ( -1 == Left )
			{
				Left = left;
				Right = right;
			}
			else
			{
				if ( Left > left ) { Left = left; }
				if ( Right < right ) { Right = right; }
			}
		}

		k = i*ImageStep;
		if ( -1 == Left ) 
		{	
			idx = k;
			Range = ImageStep;
			::memset(&(ImagePtr[idx]), Gray, sizeof(IMAGE_DATA)*Range);
			continue; 
		}
		
		//Fill 0->Left
		if ( Left > 0 )
		{
			idx = k;
			if ( 24 == BitCount )
			{	Range = Left*3;	}
			else
			{	Range = Left;	}			
			::memset(&(ImagePtr[idx]), Gray, sizeof(IMAGE_DATA)*Range);
		}
		//Fill Right->Max
		if ( Right < ImageW )
		{			
			if ( 24 == BitCount )
			{
				idx = k+(Right*3);
				Range = (ImageW-Right)*3;	
			}
			else
			{	
				idx = k+(Right);
				Range = (ImageW-Right);	
			}			
			::memset(&(ImagePtr[idx]), Gray, sizeof(IMAGE_DATA)*Range);
		}		
	}	

	int YIndex=0;
	int Top=0, Bottom=0;
	//由左右, 找出每條垂直線的上下邊線
	for ( i=BlobRect.left; i<=BlobRect.right; i++ )
	{
		Top = Bottom = -1;
		for ( j=0; j<ScanLinePtrCount; j++ )
		{	
			pScanLine = ScanLinePtrList[j];
			if ( NULL == pScanLine ) { continue; }
			if ( pScanLine->m_XidxS>i || pScanLine->m_XidxE<i ) { continue; }

			YIndex = pScanLine->m_Yidx;
			if ( YIndex < 0 ) { YIndex = 0; }
			else if ( YIndex > MaxY ) { YIndex = MaxY; }
			if ( -1 == Top )
			{	Top = Bottom = YIndex;	}
			else
			{
				if ( Top > YIndex ) { Top = YIndex; }
				if ( Bottom < YIndex ) { Bottom = YIndex; }
			}
		}
		if ( -1 == Top ) 
		{	continue;	}
		for ( j=BlobRect.top; j<=BlobRect.bottom; j++ )
		{
			if ( j<Top || j>=Bottom )
			{
				if ( 24 == BitCount )
				{	
					idx = (j*ImageStep)+(i*3);	
					ImagePtr[idx] = ImagePtr[idx+1] = ImagePtr[idx+2] = Gray;
				}
				else
				{	
					idx = (j*ImageStep)+(i);	
					ImagePtr[idx] = Gray;
				}
			}
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::FillBlobRectOutside(TBlobResult *BlobPtr, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR ImagePtr, IMAGE_DATA FillGray)//在原來影像, 填滿取到的blob矩形外部的圖
{
	if ( NULL == ImagePtr ) { return false; }
	if ( CheckBlobPtr(BlobPtr) == false )
	{	return false; }	
	
	size_t      i=0, j=0, k=0, idx=0;
	int         left=0, right=0;
	int         Left=0, Right=0, Range=0;
	int         Channels=1;
	unsigned char Gray=FillGray;
	TScanLine  *pScanLine=NULL;	
	const size_t MinX=0;
	const size_t MaxX=ImageW-1;
	const size_t MaxY=ImageH-1;
	const size_t BufferSize=CalcBufferSize(ImageStep, ImageH);	
	const size_t ScanLineCount = m_ScanLineList.size();	
	std::vector<TScanLine*> ScanLinePtrList;
	if ( 24 == BitCount )
	{	Channels = 3; }
	else
	{	Channels = 1; }
	GetScanLinePtrListByID(BlobPtr->m_BlobID, ScanLinePtrList);
	const size_t ScanLinePtrCount = ScanLinePtrList.size();
	if ( 0 == ScanLinePtrCount ) { return true; }
	
	RECT BlobRect = BlobPtr->m_BlobRect;
	for ( i=0; i<ImageH; i++ )
	{
		idx = i*ImageStep;
		if ( i<BlobRect.top || i>=BlobRect.bottom ) 
		{
			Range = ImageStep;
			::memset(&(ImagePtr[idx]), Gray, sizeof(IMAGE_DATA)*Range);
			continue;
		}		
		if ( BlobRect.left > 0 )
		{
			Range = BlobRect.left*Channels;
			::memset(&(ImagePtr[idx]), Gray, sizeof(IMAGE_DATA)*Range);
			
		}
		if ( BlobRect.right < ImageW )
		{
			idx += (BlobRect.right)*Channels;
			Range = (ImageW-BlobRect.right)*Channels;
			::memset(&(ImagePtr[idx]), Gray, sizeof(IMAGE_DATA)*Range);
		}
	}
	return true;

	//由上下, 找出每條水平線的左右邊線
	int LastLeft=-1, LastRight=-1;
	for ( i=BlobRect.top; i<=BlobRect.bottom; i++ )
	{
		Left = Right = -1;
		for ( j=0; j<ScanLinePtrCount; j++ )
		{	
			pScanLine = ScanLinePtrList[j];
			if ( NULL == pScanLine ) { continue; }
			if ( i != pScanLine->m_Yidx ) { continue; }

			left = MAX(0, pScanLine->m_XidxS);
			right = MIN(MaxX, pScanLine->m_XidxE);			
			if ( -1 == Left )
			{
				Left = left;
				Right = right;
			}
			else
			{
				if ( Left > Left ) { Left = Left; }
				if ( Right < right ) { Right = right; }
			}
		}
		k = i*ImageStep;
		if ( -1 == Left ) 
		{	
			idx = k;
			Range = ImageStep;
			::memset(&(ImagePtr[idx]), Gray, sizeof(IMAGE_DATA)*Range);
			continue; 
		}

		//Fill 0->Left
		if ( Left > 0 )
		{
			idx = k;			
			Range = Left*Channels;			
			::memset(&(ImagePtr[idx]), Gray, sizeof(IMAGE_DATA)*Range);
		}
		//Fill Right->Max
		if ( Right < ImageW )
		{	
			idx = k+(Right*Channels);
			Range = (ImageW-Right)*Channels;
			::memset(&(ImagePtr[idx]), Gray, sizeof(IMAGE_DATA)*Range);
		}		
	}	

	int YIndex=0;
	int Top=0, Bottom=0;
	//由左右, 找出每條垂直線的上下邊線
	for ( i=BlobRect.left; i<=BlobRect.right; i++ )
	{
		Top = Bottom = -1;
		for ( j=0; j<ScanLinePtrCount; j++ )
		{	
			pScanLine = ScanLinePtrList[j];
			if ( NULL == pScanLine ) { continue; }
			if ( pScanLine->m_XidxS>i || pScanLine->m_XidxE<i ) { continue; }

			YIndex = pScanLine->m_Yidx;
			if ( YIndex < 0 ) { YIndex = 0; }
			else if ( YIndex > MaxY ) { YIndex = MaxY; }
			if ( -1 == Top )
			{	Top = Bottom = YIndex;	}
			else
			{
				if ( Top > YIndex ) { Top = YIndex; }
				if ( Bottom < YIndex ) { Bottom = YIndex; }
			}
		}
		if ( -1 == Top ) 
		{	continue;	}
		for ( j=BlobRect.top; j<=BlobRect.bottom; j++ )
		{
			if ( j<Top || j>=Bottom )
			{
				idx = (j*ImageStep)+(i*Channels);	
				if ( 24 == BitCount )
				{	ImagePtr[idx] = ImagePtr[idx+1] = ImagePtr[idx+2] = Gray;	}
				else
				{	ImagePtr[idx] = Gray;	}
			}
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::GetBlobSelfImage(TBlobResult *BlobPtr, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr)
{
	if ( CheckBlobPtr(BlobPtr) == false )
	{	return false; }	

	POINT       Pt;
	int         j=0;
	size_t      i=0;	
	size_t      idx=0;
	size_t      idx2=0;	
	RECT        Rect={0,0,0,0};	
	TScanLine  *pScanLine=NULL;
	std::vector<size_t> IndexList;
	const size_t ScanLineCount = m_ScanLineList.size();

	Rect.left   =  INT_MAX;
	Rect.top    =  INT_MAX;
	Rect.right  = -INT_MAX;
	Rect.bottom = -INT_MAX;
	for ( i=0; i<ScanLineCount; i++ )
	{	
		pScanLine = &(m_ScanLineList[i]);
		if ( NULL == pScanLine ) { continue; }
		if ( pScanLine->m_BlobID != BlobPtr->m_BlobID ) { continue; }
		IndexList.push_back(i);

		if ( Rect.top > pScanLine->m_Yidx ) { Rect.top = pScanLine->m_Yidx; }
		if ( Rect.bottom < pScanLine->m_Yidx ) { Rect.bottom = pScanLine->m_Yidx; }		

		if ( Rect.left > pScanLine->m_XidxS ) { Rect.left = pScanLine->m_XidxS; }
		if ( Rect.right < pScanLine->m_XidxE ) { Rect.left = pScanLine->m_XidxE; }		
	}
	Rect.bottom += 1;
	Rect.right  += 1;

	const int Ox = Rect.left;
	const int Oy = Rect.top;
	const int W = Rect.right-Rect.left;
	const int H = Rect.bottom-Rect.top;
	const int BitCnt = 8;
	const int Step = JetAPI::GetBMPImagePixelsPerLine(W, BitCnt, 4);
	const size_t BufferSize = CalcBufferSize(Step, H);
	if ( JetMemory.alloc_func(BufferSize, ImagePtr, "CJetBlob::GetBlobSelfImage", "ImagePtr") == false ) 
	{	return false; }
	::memset(ImagePtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);

	const size_t IndexCount = IndexList.size();
	for ( i=0; i<IndexCount; i++ )
	{
		idx = IndexList[i];
		if ( idx >= ScanLineCount ) { continue; }
		pScanLine = &(m_ScanLineList[idx]);
		if ( NULL == pScanLine ) { continue; }
		if ( pScanLine->m_BlobID != BlobPtr->m_BlobID ) { continue; }

		idx2 = (pScanLine->m_Yidx-Oy)*Step;
		for ( j=pScanLine->m_XidxS; j<=pScanLine->m_XidxE; j++ )
		{	ImagePtr[idx2+j-Ox]=0xFF;	}
	}
	ImageW = W;
	ImageH = H;
	ImageStep = Step;
	BitCount = BitCnt;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::SortBlobListByID()
{
	size_t       i=0;
	size_t       ID=0;
	TBlobResult *BlobPtr=NULL;	
	TBlobVector  BlobList;
	CSortObj     SortObj;
	CSortObj    *SortObjPtr=NULL;
	std::vector<CSortObj> SortList;
	CloneBlobList(BlobList);
	const size_t BlobCount = BlobList.size();
	if ( 0 == BlobCount ) { return true; }

	SortObj.SetSortMode(SORT_BY_INT);
	for ( i=0; i<BlobCount; i++ )
	{
		BlobPtr = &(BlobList[i]);
		if ( NULL == BlobPtr ) { continue; }		
		SortObj.SetID(i);
		SortObj.SetValueInt(BlobPtr->m_BlobID);
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());
	
	m_BlobList.clear();
	const size_t SortCount = SortList.size();
	for ( i=0; i<SortCount; i++ )
	{
		SortObjPtr = &(SortList[i]);
		if ( NULL == SortObjPtr ) { continue; }
		ID = SortObjPtr->GetID();
		if ( ID > BlobCount ) { continue; }
		m_BlobList.push_back(BlobList[ID]);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::SortBlobListByPixels()
{
	size_t       i=0;
	size_t       ID=0;
	TBlobResult *BlobPtr=NULL;	
	TBlobVector  BlobList;
	CSortObj     SortObj;
	CSortObj    *SortObjPtr=NULL;
	std::vector<CSortObj> SortList;
	CloneBlobList(BlobList);
	const size_t BlobCount = BlobList.size();
	if ( 0 == BlobCount ) { return true; }

	SortObj.SetSortMode(SORT_BY_INT);
	for ( i=0; i<BlobCount; i++ )
	{
		BlobPtr = &(BlobList[i]);
		if ( NULL == BlobPtr ) { continue; }		
		SortObj.SetID(i);
		SortObj.SetValueInt(BlobPtr->m_BlobPixels);
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());
	
	m_BlobList.clear();
	const size_t SortCount = SortList.size();
	for ( i=0; i<SortCount; i++ )
	{
		SortObjPtr = &(SortList[SortCount-i-1]);
		if ( NULL == SortObjPtr ) { continue; }
		ID = SortObjPtr->GetID();
		if ( ID > BlobCount ) { continue; }
		m_BlobList.push_back(BlobList[ID]);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::SortBlobListByRectSize()
{
	size_t       i=0;
	size_t       ID=0;
	TBlobResult *BlobPtr=NULL;	
	TBlobVector  BlobList;
	CSortObj     SortObj;
	CSortObj    *SortObjPtr=NULL;
	std::vector<CSortObj> SortList;
	CloneBlobList(BlobList);
	const size_t BlobCount = BlobList.size();
	if ( 0 == BlobCount ) { return true; }

	SortObj.SetSortMode(SORT_BY_INT);
	for ( i=0; i<BlobCount; i++ )
	{
		BlobPtr = &(BlobList[i]);
		if ( NULL == BlobPtr ) { continue; }		
		SortObj.SetID(i);		
		SortObj.SetValueInt(BlobPtr->GetBlobRectW()*BlobPtr->GetBlobRectH());
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());
	
	m_BlobList.clear();
	const size_t SortCount = SortList.size();
	for ( i=0; i<SortCount; i++ )
	{
		SortObjPtr = &(SortList[SortCount-i-1]);
		if ( NULL == SortObjPtr ) { continue; }
		ID = SortObjPtr->GetID();
		if ( ID > BlobCount ) { continue; }
		m_BlobList.push_back(BlobList[ID]);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::SortBlobListByToward(int Toward, bool bAsc)
{
	size_t       i=0;
	size_t       ID=0;
	TBlobResult *BlobPtr=NULL;	
	TBlobVector  BlobList;
	CSortObj     SortObj;
	CSortObj    *SortObjPtr=NULL;
	std::vector<CSortObj> SortList;
	CloneBlobList(BlobList);
	const size_t BlobCount = BlobList.size();
	if ( 0 == BlobCount ) { return true; }

	SortObj.SetSortMode(SORT_BY_INT);
	for ( i=0; i<BlobCount; i++ )
	{
		BlobPtr = &(BlobList[i]);
		if ( NULL == BlobPtr ) { continue; }		
		SortObj.SetID(i);
		switch ( Toward )
		{
		case TOWARD_UP://由下往上
			SortObj.SetValueInt(BlobPtr->m_BlobRect.bottom);
			break;
		case TOWARD_DOWN://由上往下			
			SortObj.SetValueInt(BlobPtr->m_BlobRect.top);
			break;
		case TOWARD_LEFT://由右往左
			SortObj.SetValueInt(BlobPtr->m_BlobRect.right);
			break;
		case TOWARD_RIGHT://由左往右
			SortObj.SetValueInt(BlobPtr->m_BlobRect.left);
			break;
		}		
		SortList.push_back(SortObj);
	}
	std::sort(SortList.begin(), SortList.end());
	
	m_BlobList.clear();
	const size_t SortCount = SortList.size();

	for ( i=0; i<SortCount; i++ )
	{
		if ( true == bAsc )
		{	SortObjPtr = &(SortList[i]);	}
		else
		{	SortObjPtr = &(SortList[SortCount-i-1]); }		
		if ( NULL == SortObjPtr ) { continue; }
		ID = SortObjPtr->GetID();
		if ( ID > BlobCount ) { continue; }
		m_BlobList.push_back(BlobList[ID]);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::GetScanLinePtrListByID(int BlobID, std::vector<PScanLine> &List)
{
	const size_t ScanLineCount = m_ScanLineList.size();	
	for ( size_t i=0; i<ScanLineCount; i++ )
	{	
		TScanLine *pScanLine = &(m_ScanLineList[i]);
		if ( NULL == pScanLine ) { continue; }
		if ( pScanLine->m_BlobID != BlobID ) { continue; }						
		List.push_back(pScanLine);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::SortPairList(std::vector<std::pair<int, int>> &List)//排序Pair列表
{
	std::sort(List.begin(), List.end(), SortPairFn);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::ReplacePairValue(std::vector<std::pair<int, int>> &List)//取代Pair數值
{
	const size_t Count=List.size();
	for ( size_t i=0; i<Count; i++ )
	{
		//OldBlobID = rPair.first;
		//NewBlobID = rPair.second;		
		const std::pair<int, int> &rPair=List[i];
		for ( size_t j=i+1; j<Count; j++ )
		{
			std::pair<int, int> &rPair2=List[j];
			if ( rPair2.second != rPair.first ) { continue; }
			rPair2.second = rPair.second;
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::AddKeyRepeatPair(std::vector<std::pair<int, int>> &List)//加入重複Key
{		
	const size_t Count=List.size();
	std::vector<std::pair<int, int>> List2;
	for ( size_t i=0; i<Count; i++ )
	{	
		size_t j=0;
		const std::pair<int, int> &rPair=List[i];		
		for ( j=i+1; j<Count; j++ )
		{
			const std::pair<int, int> &rPair2=List[j];
			if ( rPair.first!=rPair2.first )
			{	break; }
			if ( rPair.second == rPair2.second )
			{	continue;	}
			bool bExist=false;
			int Val=MIN(rPair.second, rPair2.second);
			int Key=MAX(rPair.second, rPair2.second);			
			for ( size_t k=0; k<Count; k++ )
			{
				const std::pair<int, int> &rPair3=List[k];
				if ( Key < rPair3.first ) { break; }
				if ( Key!=rPair3.first || Val!=rPair3.second ) { continue; }
				bExist=true;
				break;				
			}
			if ( false == bExist )
			{	List2.push_back(std::make_pair(Key, Val)); }
		}
		i = j-1;
	}	

	if ( 0 == List2.size() )
	{	return false; }
		
	AddKeyRepeatPair(List2);
	List.insert(List.end(), List2.begin(), List2.end());			 
	return true;
}
//-------------------------------------------------------------------------------------//
bool CJetBlob::RemoveRepeatPair(std::vector<std::pair<int, int>> &List)
{
	size_t i=0, j=0;
	std::vector<std::pair<int,int>> TmpList=List;
	const size_t TmpCnt=TmpList.size();

	List.clear();	
	for ( i=0; i<TmpCnt; i++ )
	{			
		const std::pair<int, int> &rPair=TmpList[i];		
		for ( j=i+1; j<TmpCnt; j++ )
		{
			const std::pair<int, int> &rPair2=TmpList[j];
			if ( rPair.first!=rPair2.first || rPair.second!=rPair2.second )
			{	break; }			
		}
		i = j-1;
		List.push_back(rPair);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
double  CJetBlob::CalcBlobLongShortRatio(double BlobW, double BlobH) const
{
	if ( BlobW > BlobH )
	{	return BlobW/BlobH; }
	return BlobH/BlobW;
}
//-------------------------------------------------------------------------------------//