// ImageAPI_OpenCV.cpp: implementation of the CImageAPI class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "ImageAPI.h"
//-------------------------------------------------------------------------------------//
#include "OpenCV_MatAllocator.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#ifndef OPENCV_DISABLE
//-------------------------------------------------------------------------------------//
bool CImageAPI::InitOpenCV()
{
	int NumThreads = cv::getNumThreads();
	int ThreadNum = cv::getThreadNum();
	int NumberOfCPUs = cv::getNumberOfCPUs();
	bool bOptimized = cv::useOptimized();
	cv :: setNumThreads(0);//0-disable parall loop
	int NumThreads2 = cv::getNumThreads();	
#if OPEN_CV_VERSION == OPEN_CV_3_4_16_00_V14
	cv::Mat Mat;	
	Mat.setDefaultAllocator(&g_MatAllocator);
#endif//OPEN_CV_VERSION
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::GetOpenCVDataTypeText(int nType, char Text[])//取得 OpenCV-資料型態文字
{		
	bool IsOK = true;
	//for(int i = 0; i < M.rows; i++)
	//{
	//	const double* Mi = M.ptr<double>(i);//重點
	//	for(int j = 0; j < M.cols; j++)
	//		sum += std::max(Mi[j], 0.);
	//}
	switch ( nType )
	{
	case CV_8UC1:	::strcpy(Text, "CV_8UC1");	break;
	case CV_8UC2:	::strcpy(Text, "CV_8UC2");	break;
	case CV_8UC3:	::strcpy(Text, "CV_8UC3");	break;
	case CV_8UC4:	::strcpy(Text, "CV_8UC4");	break;

	case CV_8SC1:	::strcpy(Text, "CV_8SC1");	break;
	case CV_8SC2:	::strcpy(Text, "CV_8SC2");	break;
	case CV_8SC3:	::strcpy(Text, "CV_8SC3");	break;
	case CV_8SC4:	::strcpy(Text, "CV_8SC4");	break;

	case CV_16UC1:	::strcpy(Text, "CV_16UC1");	break;
	case CV_16UC2:	::strcpy(Text, "CV_16UC2");	break;
	case CV_16UC3:	::strcpy(Text, "CV_16UC3");	break;
	case CV_16UC4:	::strcpy(Text, "CV_16UC4");	break;

	case CV_16SC1:	::strcpy(Text, "CV_16SC1");	break;
	case CV_16SC2:	::strcpy(Text, "CV_16SC2");	break;
	case CV_16SC3:	::strcpy(Text, "CV_16SC3");	break;
	case CV_16SC4:	::strcpy(Text, "CV_16SC4");	break;

	case CV_32SC1:	::strcpy(Text, "CV_32SC1");	break;
	case CV_32SC2:	::strcpy(Text, "CV_32SC2");	break;
	case CV_32SC3:	::strcpy(Text, "CV_32SC3");	break;
	case CV_32SC4:	::strcpy(Text, "CV_32SC4");	break;

	case CV_32FC1:	::strcpy(Text, "CV_32FC1");	break;
	case CV_32FC2:	::strcpy(Text, "CV_32FC2");	break;
	case CV_32FC3:	::strcpy(Text, "CV_32FC3");	break;
	case CV_32FC4:	::strcpy(Text, "CV_32FC4");	break;

	case CV_64FC1:	::strcpy(Text, "CV_64FC1");	break;
	case CV_64FC2:	::strcpy(Text, "CV_64FC2");	break;
	case CV_64FC3:	::strcpy(Text, "CV_64FC3");	break;
	case CV_64FC4:	::strcpy(Text, "CV_64FC4");	break;
	default:
		IsOK = false;
		::sprintf(Text, "%d", nType);		
		break;
	}
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::AdjustSaveLoadImageReverse(bool Reverse)//調整存檔開檔的影像反向變數, 為了與CDib吻合
{
	if ( true == Reverse ) { return false; }
	return true; 
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::ReturnOpenCVDisableException()//回傳關閉OpenCV的錯誤
{
	LockImageAPI();
	m_ErrorString = _T("Error, OpenCV Disabled");
	UnlockImageAPI();
	return false;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::CheckMatIsValid(const cv::Mat &M)//確認cv::Mat是有效的
{
	if ( NULL == M.data || NULL==M.datastart || NULL==M.dataend ) 
	{
		LockImageAPI();
		m_ErrorString = _T("Error, OpenCV-Mat is Exception");
		UnlockImageAPI();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::CatchOpenCVException(const cv::Exception &e)//取得OpenCV的例外物件
{
	const char* msg_e = e.what();  	
	LockImageAPI();
	m_ErrorString = msg_e;
	UnlockImageAPI();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::CheckMatIdentity(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const void *Ptr)//確認Mat相同
{
	void *MPtr = M.data;	
	if ( Ptr != MPtr )
	{	return false; }

	const int MStep = M.step1();	
	if ( ImageW!=M.cols || ImageH!=M.rows || ImageStep!=MStep )
	{	return false; }

	const int MChannel = M.channels();
	const int nChannels = GetImageChannels(BitCount);
	if ( nChannels != MChannel )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
void  CImageAPI::RectToCVRect(const RECT &Rect, cv::Rect &cvRect)
{
	cvRect.x = Rect.left;
	cvRect.y = Rect.top;
	cvRect.width = Rect.right-Rect.left;
	cvRect.height = Rect.bottom-Rect.top;
}
//-------------------------------------------------------------------------------------//
void CImageAPI::CvPointToCVRect(const CvPoint &P1, const CvPoint &P2, cv::Rect &cvRect)
{
	if ( P1.x > P2.x )
	{
		cvRect.x = P2.x;
		cvRect.width = P1.x-P2.x;
	}
	else
	{
		cvRect.x = P1.x;
		cvRect.width = P2.x-P1.x;
	}
	if ( P1.y > P2.y )
	{
		cvRect.y = P2.y;
		cvRect.height = P1.y-P2.y;
	}
	else
	{
		cvRect.y = P1.y;
		cvRect.height = P2.y-P1.y;
	}
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::WriteMatData(LPCTSTR filename, cv::Mat &M)
{
	FILE *pfile = ::_tfopen(filename, _T("w+"));
	if ( NULL == pfile ) 
	{
		CString str = filename;
		m_ErrorString.Format(_T("Error, Open File Fault [%s]"), str);			
		return false;
	}
	const int Depth=M.depth();
	switch ( Depth )
	{
	case CV_8UC1:
		for ( int i=0; i<M.rows; i++ )
		{
			for ( int j=0; j<M.cols; j++ )
			{
				unsigned int v=M.ptr<unsigned char>(i)[j];
				::fprintf(pfile, "%u, ", v);
			}
			::fprintf(pfile, "\n");
		}
		break;
	case CV_8SC1:
		for ( int i=0; i<M.rows; i++ )
		{
			for ( int j=0; j<M.cols; j++ )
			{
				int v=M.ptr<char>(i)[j];
				::fprintf(pfile, "%d, ", v);
			}
			::fprintf(pfile, "\n");
		}
		break;
	case CV_16UC1:
		for ( int i=0; i<M.rows; i++ )
		{
			for ( int j=0; j<M.cols; j++ )
			{
				unsigned int v=M.ptr<unsigned short>(i)[j];
				::fprintf(pfile, "%u, ", v);
			}
			::fprintf(pfile, "\n");
		}
		break;
	case CV_16SC1:
		for ( int i=0; i<M.rows; i++ )
		{
			for ( int j=0; j<M.cols; j++ )
			{
				int v=M.ptr<short>(i)[j];
				::fprintf(pfile, "%d, ", v);
			}
			::fprintf(pfile, "\n");
		}
		break;
	case CV_32SC1:
		for ( int i=0; i<M.rows; i++ )
		{
			for ( int j=0; j<M.cols; j++ )
			{
				int v=M.ptr<int>(i)[j];
				::fprintf(pfile, "%d, ", v);
			}
			::fprintf(pfile, "\n");
		}
		break;
	case CV_32FC1:
		for ( int i=0; i<M.rows; i++ )
		{
			for ( int j=0; j<M.cols; j++ )
			{
				float v=M.ptr<float>(i)[j];
				::fprintf(pfile, "%f, ", v);
			}
			::fprintf(pfile, "\n");
		}
		break;
	case CV_64FC1:
		for ( int i=0; i<M.rows; i++ )
		{
			for ( int j=0; j<M.cols; j++ )
			{
				double v=M.ptr<double>(i)[j];
				::fprintf(pfile, "%f, ", v);
			}
			::fprintf(pfile, "\n");
		}
		break;
	}	
	::fclose(pfile); pfile = NULL;
	return true;
}
//-------------------------------------------------------------------------------------//
cv::Mat CImageAPI::CreateMatImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, int Type, int Channel, const char *fnName)//創建OpenCV Image指標	
{	
	cv::Mat TmpMat;	
	unsigned char *Ptr = NULL;	
	const int nType = CV_MAKETYPE(Type, Channel);
	const int nElemSize=CV_ELEM_SIZE(nType);
	const int nStep=GetImageAlignedWidth(ImageW*nElemSize, 8, 4);
	const size_t BufferSize=CalcBufferSize(nStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, Ptr, fnName, "cvPtr") == false )
	{	
		SetErrorString(JetMemory.GetErrorString());
		return TmpMat;
	}
	cv::Size sz((int)(ImageW), (int)(ImageH));
	TmpMat = cv::Mat(sz, nType, (void*)Ptr, nStep);	
	return TmpMat;
}
//-------------------------------------------------------------------------------------//
cv::Mat CImageAPI::CreateMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const int *Ptr)
{
	int nType = 0;	
	int nStep=(int)((ImageStep)*sizeof(int));
	cv::Size sz((int)(ImageW), (int)(ImageH));
	if ( 8 == BitCount ) { nType = CV_32SC1;}
	if ( 24 == BitCount ) { nType = CV_32SC3;}
	cv::Mat M(sz, nType, (void*)Ptr, nStep);	
	return M;
}
//-------------------------------------------------------------------------------------//
cv::Mat CImageAPI::CreateMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const char *Ptr)
{
	int nType = 0;	
	int nStep=(int)((ImageStep)*sizeof(char));
	cv::Size sz((int)(ImageW), (int)(ImageH));
	if ( 8 == BitCount ) { nType = CV_8SC1;}
	if ( 24 == BitCount ) { nType = CV_8SC3;}
	cv::Mat M(sz, nType, (void*)Ptr, nStep);		
	return M;
}
//-------------------------------------------------------------------------------------//
cv::Mat CImageAPI::CreateMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const short *Ptr)
{
	int nType = 0;	
	int nStep=(int)((ImageStep)*sizeof(short));
	cv::Size sz((int)(ImageW), (int)(ImageH));
	if ( 8 == BitCount ) { nType = CV_16SC1;}
	if ( 24 == BitCount ) { nType = CV_16SC3;}
	cv::Mat M(sz, nType, (void*)Ptr, nStep);	
	return M;
}
//-------------------------------------------------------------------------------------//
cv::Mat CImageAPI::CreateMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const float *Ptr)
{
	int nType = 0;	
	int nStep=(int)((ImageStep)*sizeof(float));
	cv::Size sz((int)(ImageW), (int)(ImageH));
	if ( 8 == BitCount ) { nType = CV_32FC1;}
	if ( 24 == BitCount ) { nType = CV_32FC3;}
	cv::Mat M(sz, nType, (void*)Ptr, nStep);	
	return M;
}
//-------------------------------------------------------------------------------------//
cv::Mat CImageAPI::CreateMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const double *Ptr)
{
	int nType = 0;	
	int nStep=(int)((ImageStep)*sizeof(double));
	cv::Size sz((int)(ImageW), (int)(ImageH));
	if ( 8 == BitCount ) { nType = CV_64FC1;}
	if ( 24 == BitCount ) { nType = CV_64FC3;}
	cv::Mat M(sz, nType, (void*)Ptr, nStep);	
	return M;
}
//-------------------------------------------------------------------------------------//
cv::Mat CImageAPI::CreateMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *Ptr)
{	
	int nType = 0;	
	int nStep=(int)((ImageStep)*sizeof(unsigned char));
	cv::Size sz((int)(ImageW), (int)(ImageH));
	if ( 8 == BitCount ) { nType = CV_8UC1;}
	if ( 24 == BitCount ) { nType = CV_8UC3;}
	cv::Mat M(sz, nType, (void*)Ptr, nStep);
	return M;
}
//-------------------------------------------------------------------------------------//
cv::Mat CImageAPI::CreateMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned short *Ptr)
{
	int nType = 0;	
	int nStep=(int)((ImageStep)*sizeof(unsigned short));
	cv::Size sz((int)(ImageW), (int)(ImageH));
	if ( 8 == BitCount ) { nType = CV_16UC1;}
	if ( 24 == BitCount ) { nType = CV_16UC3;}
	cv::Mat M(sz, nType, (void*)Ptr, nStep);	
	return M;
}
//-------------------------------------------------------------------------------------//
cv::Mat CImageAPI::CreateMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB)
{
	const int nType = CV_8UC3;		
	cv::Size sz((int)(ImageW), (int)(ImageH));	
	cv::Mat M(sz, nType);

	size_t i=0, j=0;
	size_t midx=0, idx=0;
	unsigned char *Ptr=M.data;
	const int MStep=M.step1();
	for ( i=0; i<ImageH; i++ )
	{
		midx = (i*MStep);
		idx = i*ImageStep;
		for ( j=0; j<ImageW; j++ )
		{
			Ptr[midx++]=pB[idx];
			Ptr[midx++]=pG[idx];
			Ptr[midx++]=pR[idx];
			idx ++;
		}
	}	
	return M;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::CloneMatData(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, int *Ptr, bool Reverse)
{	
	if ( CheckMatIdentity(M, ImageW, ImageH, ImageStep, BitCount, Ptr) == true )
	{
		if ( true == Reverse )
		{
			m_ErrorString = _T("Error, Repeat Image Ptr can not be Reverse");
			return false;
		}
		return true; 
	}
	size_t CopyLen=ImageW;	
	const size_t MStep=M.step1();//已經除以資料結構長度
	if ( 8 == BitCount )
	{	
		if ( CheckGrayImage(ImageW, ImageH, ImageStep, Ptr) == false )
		{	return false; }
	}
	if ( 24 == BitCount )
	{	
		CopyLen = ImageW*3;
		if ( CheckColorImage(ImageW, ImageH, ImageStep, Ptr) == false )
		{	return false; }
	}
	if ( MStep < CopyLen )
	{
		m_ErrorString.Format(_T("Error, Mat Step Exception (W, H, S, Copy)=(%d, %d, %d, %d)"), ImageW, ImageH, MStep, CopyLen);
		return false;
	}

	size_t i=0;	
	size_t srcidx=0, destidx=0;		
	unsigned char *MPtr = M.data;
	const size_t MatByteStep = M.step;			
	const size_t Total = M.total();
	const size_t BuffsetStep= ImageStep;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
	CopyLen = CopyLen*sizeof(int);	
	if ( false == Reverse )
	{
		if ( MStep == ImageStep )
		{	::memcpy(Ptr, MPtr, sizeof(int)*BufferSize);	}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*MatByteStep;
				destidx = i*BuffsetStep;
				::memcpy(&(Ptr[destidx]), &(MPtr[srcidx]), CopyLen);
			}
		}
	}
	else
	{
		for ( i=0; i<ImageH; i++ )
		{
			srcidx = i*MatByteStep;
			destidx = (ImageH-i-1)*BuffsetStep;
			::memcpy(&(Ptr[destidx]), &(MPtr[srcidx]), CopyLen);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::CloneMatData(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, char *Ptr, bool Reverse)
{
	if ( CheckMatIdentity(M, ImageW, ImageH, ImageStep, BitCount, Ptr) == true )
	{
		if ( true == Reverse )
		{
			m_ErrorString = _T("Error, Repeat Image Ptr can not be Reverse");
			return false;
		}
		return true; 
	}
	size_t CopyLen=ImageW;	
	const size_t MStep=M.step1();//已經除以資料結構長度
	if ( 8 == BitCount )
	{	
		if ( CheckGrayImage(ImageW, ImageH, ImageStep, Ptr) == false )
		{	return false; }
	}
	if ( 24 == BitCount )
	{	
		CopyLen = ImageW*3;
		if ( CheckColorImage(ImageW, ImageH, ImageStep, Ptr) == false )
		{	return false; }
	}
	if ( MStep < CopyLen )
	{
		m_ErrorString.Format(_T("Error, Mat Step Exception (W, H, S, Copy)=(%d, %d, %d, %d)"), ImageW, ImageH, MStep, CopyLen);
		return false;
	}

	size_t i=0;	
	size_t srcidx=0, destidx=0;	
	unsigned char *MPtr = M.data;	
	const size_t MatByteStep = M.step;			
	const size_t Total = M.total();
	const size_t BuffsetStep= ImageStep;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
	CopyLen = CopyLen*sizeof(char);	
	if ( false == Reverse )
	{
		if ( MStep == ImageStep )
		{	::memcpy(Ptr, MPtr, sizeof(char)*BufferSize);	}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*MatByteStep;
				destidx = i*BuffsetStep;
				::memcpy(&(Ptr[destidx]), &(MPtr[srcidx]), CopyLen);
			}
		}
	}
	else
	{
		for ( i=0; i<ImageH; i++ )
		{
			srcidx = i*MatByteStep;
			destidx = (ImageH-i-1)*BuffsetStep;
			::memcpy(&(Ptr[destidx]), &(MPtr[srcidx]), CopyLen);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::CloneMatData(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, short *Ptr, bool Reverse)
{
	if ( CheckMatIdentity(M, ImageW, ImageH, ImageStep, BitCount, Ptr) == true )
	{
		if ( true == Reverse )
		{
			m_ErrorString = _T("Error, Repeat Image Ptr can not be Reverse");
			return false;
		}
		return true; 
	}
	size_t CopyLen=ImageW;	
	const size_t MStep=M.step1();//已經除以資料結構長度
	if ( 8 == BitCount )
	{	
		if ( CheckGrayImage(ImageW, ImageH, ImageStep, Ptr) == false )
		{	return false; }
	}
	if ( 24 == BitCount )
	{	
		CopyLen = ImageW*3;
		if ( CheckColorImage(ImageW, ImageH, ImageStep, Ptr) == false )
		{	return false; }
	}
	if ( MStep < CopyLen )
	{
		m_ErrorString.Format(_T("Error, Mat Step Exception (W, H, S, Copy)=(%d, %d, %d, %d)"), ImageW, ImageH, MStep, CopyLen);
		return false;
	}

	size_t i=0;	
	size_t srcidx=0, destidx=0;	
	unsigned char *MPtr = M.data;	
	const size_t MatByteStep = M.step;			
	const size_t Total = M.total();
	const size_t BuffsetStep= ImageStep;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
	CopyLen = CopyLen*sizeof(short);	
	if ( false == Reverse )
	{
		if ( MStep == ImageStep )
		{	::memcpy(Ptr, MPtr, sizeof(short)*BufferSize);	}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*MatByteStep;
				destidx = i*BuffsetStep;
				::memcpy(&(Ptr[destidx]), &(MPtr[srcidx]), CopyLen);
			}
		}
	}
	else
	{
		for ( i=0; i<ImageH; i++ )
		{
			srcidx = i*MatByteStep;
			destidx = (ImageH-i-1)*BuffsetStep;
			::memcpy(&(Ptr[destidx]), &(MPtr[srcidx]), CopyLen);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::CloneMatData(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, float *Ptr, bool Reverse)
{
	if ( CheckMatIdentity(M, ImageW, ImageH, ImageStep, BitCount, Ptr) == true )
	{
		if ( true == Reverse )
		{
			m_ErrorString = _T("Error, Repeat Image Ptr can not be Reverse");
			return false;
		}
		return true; 
	}
	size_t CopyLen=ImageW;	
	const size_t MStep=M.step1();//已經除以資料結構長度
	if ( 8 == BitCount )
	{	
		if ( CheckGrayImage(ImageW, ImageH, ImageStep, Ptr) == false )
		{	return false; }
	}
	if ( 24 == BitCount )
	{	
		CopyLen = ImageW*3;
		if ( CheckColorImage(ImageW, ImageH, ImageStep, Ptr) == false )
		{	return false; }
	}
	if ( MStep < CopyLen )
	{
		m_ErrorString.Format(_T("Error, Mat Step Exception (W, H, S, Copy)=(%d, %d, %d, %d)"), ImageW, ImageH, MStep, CopyLen);
		return false;
	}

	size_t i=0;	
	size_t srcidx=0, destidx=0;	
	unsigned char *MPtr = M.data;	
	const size_t MatByteStep = M.step;			
	const size_t Total = M.total();
	const size_t BuffsetStep= ImageStep;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
	CopyLen = CopyLen*sizeof(float);	
	if ( false == Reverse )
	{
		if ( MStep == ImageStep )
		{	::memcpy(Ptr, MPtr, sizeof(float)*BufferSize);	}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*MatByteStep;
				destidx = i*BuffsetStep;
				::memcpy(&(Ptr[destidx]), &(MPtr[srcidx]), CopyLen);
			}
		}
	}
	else
	{
		for ( i=0; i<ImageH; i++ )
		{
			srcidx = i*MatByteStep;
			destidx = (ImageH-i-1)*BuffsetStep;
			::memcpy(&(Ptr[destidx]), &(MPtr[srcidx]), CopyLen);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::CloneMatData(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, double *Ptr, bool Reverse)
{
	if ( CheckMatIdentity(M, ImageW, ImageH, ImageStep, BitCount, Ptr) == true )
	{
		if ( true == Reverse )
		{
			m_ErrorString = _T("Error, Repeat Image Ptr can not be Reverse");
			return false;
		}
		return true; 
	}
	size_t CopyLen=ImageW;	
	const size_t MStep=M.step1();//已經除以資料結構長度
	if ( 8 == BitCount )
	{	
		if ( CheckGrayImage(ImageW, ImageH, ImageStep, Ptr) == false )
		{	return false; }
	}
	if ( 24 == BitCount )
	{	
		CopyLen = ImageW*3;
		if ( CheckColorImage(ImageW, ImageH, ImageStep, Ptr) == false )
		{	return false; }
	}
	if ( MStep < CopyLen )
	{
		m_ErrorString.Format(_T("Error, Mat Step Exception (W, H, S, Copy)=(%d, %d, %d, %d)"), ImageW, ImageH, MStep, CopyLen);
		return false;
	}

	size_t i=0;	
	size_t srcidx=0, destidx=0;	
	unsigned char *MPtr = M.data;	
	const size_t MatByteStep = M.step;			
	const size_t Total = M.total();
	const size_t BuffsetStep= ImageStep;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
	CopyLen = CopyLen*sizeof(double);	
	if ( false == Reverse )
	{
		if ( MStep == ImageStep )
		{	::memcpy(Ptr, MPtr, sizeof(double)*BufferSize);	}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*MatByteStep;
				destidx = i*BuffsetStep;
				::memcpy(&(Ptr[destidx]), &(MPtr[srcidx]), CopyLen);
			}
		}
	}
	else
	{
		for ( i=0; i<ImageH; i++ )
		{
			srcidx = i*MatByteStep;
			destidx = (ImageH-i-1)*BuffsetStep;
			::memcpy(&(Ptr[destidx]), &(MPtr[srcidx]), CopyLen);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::CloneMatData(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, unsigned char *Ptr, bool Reverse)
{
	if ( CheckMatIdentity(M, ImageW, ImageH, ImageStep, BitCount, Ptr) == true )
	{
		if ( true == Reverse )
		{
			m_ErrorString = _T("Error, Repeat Image Ptr can not be Reverse");
			return false;
		}
		return true; 
	}
	size_t CopyLen=ImageW;	
	const size_t MStep=M.step1();//已經除以資料結構長度
	if ( 8 == BitCount )
	{	
		if ( CheckGrayImage(ImageW, ImageH, ImageStep, Ptr) == false )
		{	return false; }
	}
	if ( 24 == BitCount )
	{	
		CopyLen = ImageW*3;
		if ( CheckColorImage(ImageW, ImageH, ImageStep, Ptr) == false )
		{	return false; }
	}
	if ( MStep < CopyLen )
	{
		m_ErrorString.Format(_T("Error, Mat Step Exception (W, H, S, Copy)=(%d, %d, %d, %d)"), ImageW, ImageH, MStep, CopyLen);
		return false;
	}

	size_t i=0;	
	size_t srcidx=0, destidx=0;	
	unsigned char *MPtr = M.data;	
	const size_t MatByteStep = M.step;			
	const size_t Total = M.total();
	const size_t BuffsetStep= ImageStep;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
	CopyLen = CopyLen*sizeof(unsigned char);	
	if ( false == Reverse )
	{
		if ( MStep == ImageStep )
		{	::memcpy(Ptr, MPtr, sizeof(unsigned char)*BufferSize);	}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*MatByteStep;
				destidx = i*BuffsetStep;
				::memcpy(&(Ptr[destidx]), &(MPtr[srcidx]), CopyLen);
			}
		}
	}
	else
	{
		for ( i=0; i<ImageH; i++ )
		{
			srcidx = i*MatByteStep;
			destidx = (ImageH-i-1)*BuffsetStep;
			::memcpy(&(Ptr[destidx]), &(MPtr[srcidx]), CopyLen);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::CloneMatData(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, unsigned short *Ptr, bool Reverse)
{
	if ( CheckMatIdentity(M, ImageW, ImageH, ImageStep, BitCount, Ptr) == true )
	{
		if ( true == Reverse )
		{
			m_ErrorString = _T("Error, Repeat Image Ptr can not be Reverse");
			return false;
		}
		return true; 
	}
	size_t CopyLen=ImageW;	
	const size_t MStep=M.step1();//已經除以資料結構長度
	if ( 8 == BitCount )
	{	
		if ( CheckGrayImage(ImageW, ImageH, ImageStep, Ptr) == false )
		{	return false; }
	}
	if ( 24 == BitCount )
	{	
		CopyLen = ImageW*3;
		if ( CheckColorImage(ImageW, ImageH, ImageStep, Ptr) == false )
		{	return false; }
	}
	if ( MStep < CopyLen )
	{
		m_ErrorString.Format(_T("Error, Mat Step Exception (W, H, S, Copy)=(%d, %d, %d, %d)"), ImageW, ImageH, MStep, CopyLen);
		return false;
	}

	size_t i=0;	
	size_t srcidx=0, destidx=0;	
	unsigned char *MPtr = M.data;	
	const size_t MatByteStep = M.step;			
	const size_t Total = M.total();
	const size_t BuffsetStep= ImageStep;
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
	CopyLen = CopyLen*sizeof(unsigned short);	
	if ( false == Reverse )
	{
		if ( MStep == ImageStep )
		{	::memcpy(Ptr, MPtr, sizeof(unsigned short)*BufferSize);	}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*MatByteStep;
				destidx = i*BuffsetStep;
				::memcpy(&(Ptr[destidx]), &(MPtr[srcidx]), CopyLen);
			}
		}
	}
	else
	{
		for ( i=0; i<ImageH; i++ )
		{
			srcidx = i*MatByteStep;
			destidx = (ImageH-i-1)*BuffsetStep;
			::memcpy(&(Ptr[destidx]), &(MPtr[srcidx]), CopyLen);
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::CloneMatData(cv::Mat &M, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, unsigned char *pR, unsigned char *pG, unsigned char *pB, bool Reverse)
{		
	const int nChannels=M.channels();
	if ( CheckGrayImage3(ImageW, ImageH, ImageStep, pR, pG, pB) == false )
	{	return false; }
	if ( 3 != nChannels )
	{
		m_ErrorString.Format(_T("Error, Channels not 3-Ch[%d]"), nChannels);
		return false;
	}	

	size_t i=0, j=0;	
	size_t srcidx=0, destidx=0;	
	unsigned char *MPtr = M.data;	
	const size_t MStep=M.step1();//已經除以資料結構長度
	const size_t Total = M.total();
	if ( false == Reverse )
	{
		for ( i=0; i<ImageH; i++ )
		{
			srcidx = i*MStep;
			destidx = i*ImageStep;
			for ( j=0; j<ImageW; j++ )
			{
				pB[destidx] = MPtr[srcidx++];
				pG[destidx] = MPtr[srcidx++];
				pR[destidx] = MPtr[srcidx++];
				destidx ++;
			}			
		}
	}
	else
	{
		for ( i=0; i<ImageH; i++ )
		{
			srcidx = i*MStep;
			destidx = (ImageH-i-1)*ImageStep;
			for ( j=0; j<ImageW; j++ )
			{
				pB[destidx] = MPtr[srcidx++];
				pG[destidx] = MPtr[srcidx++];
				pR[destidx] = MPtr[srcidx++];
				destidx ++;
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DestroyCVMat(int CreateMode, CvMat *&cvMat)//刪除OpenCV Mat指標
{
	if ( NULL == cvMat ) { return true; }
	unsigned char *Ptr = NULL;
	switch ( CreateMode )
	{
	case CREATE_OPENCV_IMAGE_HEADER:
		::cvReleaseMat(&cvMat);
		break;
	case CREATE_OPENCV_IMAGE_BODY:
		Ptr = cvMat->data.ptr;
		JetMemory.free_func(Ptr);
		::cvReleaseMat(&cvMat);
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
CvMat* CImageAPI::CreateCVMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, int Type, const char *fnName)//創建OpenCV Mat指標	
{
	unsigned char *Ptr = NULL;	
	CvMat *cvMat = ::cvCreateMatHeader(ImageW, ImageH, Type);	
	if ( NULL == cvMat ) 
	{ 
		this->m_ErrorString.Format(_T("Error, cvCreateMatHeader Fault"));
		return NULL; 
	}
	size_t BufferSize = CalcBufferSize(cvMat->step, cvMat->height);
	if ( JetMemory.alloc_func(BufferSize, Ptr, fnName, "cvPtr") == false )
	{
		::cvReleaseMat(&cvMat);
		SetErrorString(JetMemory.GetErrorString());
		return NULL;
	}
	cvMat->data.ptr = Ptr;	
	return cvMat;
}
//-------------------------------------------------------------------------------------//
CvMat* CImageAPI::CreateCVMat(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, int Type, int Channel, const char *fnName)//創建OpenCV Mat指標	
{
	unsigned char *Ptr = NULL;	
	CvMat *cvMat = ::cvCreateMatHeader(ImageW, ImageH, CV_MAKETYPE(Type,Channel));	
	if ( NULL == cvMat ) 
	{ 
		this->m_ErrorString.Format(_T("Error, cvCreateMatHeader Fault"));
		return NULL; 
	}
	size_t BufferSize = CalcBufferSize(cvMat->step, cvMat->height);
	if ( JetMemory.alloc_func(BufferSize, Ptr, fnName, "cvPtr") == false )
	{
		::cvReleaseMat(&cvMat);
		SetErrorString(JetMemory.GetErrorString());
		return NULL;
	}
	cvMat->data.ptr = Ptr;	
	return cvMat;
}
//-------------------------------------------------------------------------------------//
CvMat* CImageAPI::CreateCVMatHeader(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, int Type, int Channel)//創建OpenCV Mat標頭指標
{
	CvMat *cvMat = ::cvCreateMatHeader(ImageW, ImageH, CV_MAKETYPE(Type,Channel));	
	if ( NULL == cvMat ) 
	{ 
		this->m_ErrorString.Format(_T("Error, cvCreateMatHeader Fault"));
		return NULL; 
	}
	return cvMat;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DestroyCVImage(int CreateMode, IplImage *&cvImage)//刪除Open CV影像
{
	if ( NULL == cvImage ) { return true; }
	unsigned char *Ptr = NULL;
	switch ( CreateMode )
	{
	case CREATE_OPENCV_IMAGE_HEADER:
		::cvReleaseImageHeader(&cvImage);	
		break;
	case CREATE_OPENCV_IMAGE_BODY:
		Ptr = (unsigned char*)cvImage->imageData;
		JetMemory.free_func(Ptr);
		::cvReleaseImageHeader(&cvImage);
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
IplImage* CImageAPI::CreateCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, int Type, int Channel, const char *fnName)
{
	unsigned char *Ptr = NULL;		
	IplImage *cvImagePtr = ::cvCreateImageHeader(cvSize(ImageW,ImageH), Type, Channel);
	if ( NULL == cvImagePtr ) 
	{
		this->m_ErrorString.Format(_T("Error, cvCreateImageHeader Fault"));
		return NULL; 
	}		
	size_t BufferSize = CalcBufferSize(cvImagePtr->widthStep, cvImagePtr->height);
	if ( JetMemory.alloc_func(BufferSize, Ptr, fnName, "cvPtr") == false )
	{
		::cvReleaseImageHeader(&cvImagePtr);
		SetErrorString(JetMemory.GetErrorString());
		return NULL;
	}
	cvImagePtr->imageData = (char*)(Ptr);
	return cvImagePtr;
}
//-------------------------------------------------------------------------------------//
IplImage* CImageAPI::CreateCVImageHeader(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, int Type, int Channel)//創建OpenCV Image標頭指標	
{
	IplImage *cvImagePtr = ::cvCreateImageHeader(cvSize(ImageW,ImageH), Type, Channel);
	if ( NULL == cvImagePtr ) 
	{
		this->m_ErrorString.Format(_T("Error, cvCreateImageHeader Fault"));
		return NULL; 
	}
	return cvImagePtr;
}
//-------------------------------------------------------------------------------------//
IplImage* CImageAPI::ImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, int &CreateMode)//灰階影像轉成cv影像
{
	if ( this->CheckImage(ImageW, ImageH, ImageStep, BitCount, pImage) == false )
	{	return NULL; }
	
	IplImage *cvImagePtr  = NULL;		
	const int channel = this->GetImageChannels(BitCount);
	const unsigned int cvRowStep = CImageAPI::GetImageAlignedWidth(ImageW, BitCount, 4);

	CreateMode = CREATE_OPENCV_IMAGE_NULL;
	if ( cvRowStep == ImageStep )
	{
		cvImagePtr = CreateCVImageHeader(ImageW, ImageH,IPL_DEPTH_8U, channel);
		if ( NULL == cvImagePtr )
		{	return NULL;	}
		cvImagePtr->imageData = (char*)(pImage);		
		CreateMode =  CREATE_OPENCV_IMAGE_HEADER;
	}
	else
	{					
		cvImagePtr = CreateCVImage(ImageW, ImageH, IPL_DEPTH_8U, channel, "CImageAPI::ImageToCVImage");
		if ( NULL == cvImagePtr ) 
		{	return NULL;	}
		CreateMode =  CREATE_OPENCV_IMAGE_BODY;
		size_t i=0, j=0;
		size_t srcidx=0, destidx=0;
		const size_t LineLen = this->GetImageRealWidth(ImageW, BitCount);
		const size_t LineSize = sizeof(unsigned char)*LineLen;
		for ( i=0; i<ImageH; i++ )
		{
			srcidx = i*ImageStep;
			destidx = i*cvImagePtr->widthStep;			
			::memcpy(&(cvImagePtr->imageData[destidx]), &(pImage[srcidx]), LineSize);
		}
	}
	return cvImagePtr;
}
//-------------------------------------------------------------------------------------//
CvMat* CImageAPI::ImageToCVMat(const TIMAGE &Image, bool Reverse, const char *fnName)
{
	int      cvType=0;
	const int nType      = Image.GetType();
	const int nChannels  = Image.GetChannels();
	if ( IMG_TYPE_000 == nType ) { return NULL; }
	IMAGE_SIZE ImageW    = Image.GetImageW();
	IMAGE_SIZE ImageH    = Image.GetImageH();
	IMAGE_SIZE ImageStep = Image.GetRowStep();
	IMAGE_SIZE BitCount  = Image.GetBitCount();
	switch ( nType ) 
	{
	case IMG_TYPE_08U:	cvType=CV_8U; break;
	case IMG_TYPE_16U:	cvType=CV_16U; break;
	case IMG_TYPE_32S:	cvType=CV_32S; break;
	case IMG_TYPE_32F:	cvType=CV_32F; break;	
	}	
	CvMat *cvMatPtr = CImageAPI::CreateCVMat(ImageW, ImageH, cvType, nChannels, fnName);
	if ( NULL == cvMatPtr ) { return NULL; }

	size_t i=0, j=0;
	size_t CopyLen = 0;
	size_t srcIdx=0, dstIdx=0;
	IMAGE_SIZE cvStep =  cvMatPtr->step;
	if ( IMG_TYPE_08U == nType )
	{
		if ( 1 == nChannels )
		{			
			unsigned char *srcPtr = Image.Get08uPtr();
			unsigned char *dstPtr = cvMatPtr->data.ptr;
			if ( CImageAPI::CheckPtr(srcPtr) == false ) { return false; }
			CopyLen = CImageAPI::GetImageRealWidth(ImageW, BitCount)*sizeof(unsigned char);
			if ( ImageStep!=cvStep || true==Reverse )
			{
				if ( true == Reverse )
				{
					for ( i=0; i<ImageH; i++ )
					{
						srcIdx = i*ImageStep;
						dstIdx = (ImageH-i-1)*cvStep;
						::memcpy(&(dstPtr[dstIdx]), &(srcPtr[srcIdx]), CopyLen);
					}
				}
				else
				{
					for ( i=0; i<ImageH; i++ )
					{
						srcIdx = i*ImageStep;
						dstIdx = i*cvStep;
						::memcpy(&(dstPtr[dstIdx]), &(srcPtr[srcIdx]), CopyLen);
					}
				}
			}
			else
			{	::memcpy(dstPtr, srcPtr, sizeof(unsigned char)*cvStep*ImageH);	}
		}
		if ( 3 == nChannels )
		{
			unsigned char *srcPtrR = Image.Get08uPtrR();
			unsigned char *srcPtrG = Image.Get08uPtrG();
			unsigned char *srcPtrB = Image.Get08uPtrB();
			unsigned char *dstPtr = cvMatPtr->data.ptr;
			if ( CImageAPI::CheckPtr3(srcPtrR, srcPtrG, srcPtrB) == false ) { return false; }
			if ( true == Reverse )
			{
				for ( i=0; i<ImageH; i++ )
				{
					srcIdx = i*ImageStep;
					dstIdx = (ImageH-i-1)*cvStep;
					for ( j=0; j<ImageW; j++ )
					{
						dstPtr[dstIdx] = srcPtrB[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrG[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrR[srcIdx];	dstIdx++;
						srcIdx ++;
					}
				}
			}
			else
			{
				for ( i=0; i<ImageH; i++ )
				{
					srcIdx = i*ImageStep;
					dstIdx = i*cvStep;
					for ( j=0; j<ImageW; j++ )
					{
						dstPtr[dstIdx] = srcPtrB[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrG[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrR[srcIdx];	dstIdx++;
						srcIdx ++;
					}
				}
			}
		}
	}
	if ( IMG_TYPE_16U == nType )
	{
		cvStep =  cvStep/2;
		if ( 1 == nChannels )
		{
			short          *dstPtr = cvMatPtr->data.s;
			unsigned short *srcPtr = Image.Get16uPtr();			
			if ( CImageAPI::CheckPtr(srcPtr) == false ) { return false; }
			CopyLen = CImageAPI::GetImageRealWidth(ImageW, BitCount)*sizeof(unsigned short);
			if ( ImageStep!=cvStep || true==Reverse )
			{
				if ( true == Reverse )
				{
					for ( i=0; i<ImageH; i++ )
					{
						srcIdx = i*ImageStep;
						dstIdx = (ImageH-i-1)*cvStep;
						::memcpy(&(dstPtr[dstIdx]), &(srcPtr[srcIdx]), CopyLen);
					}
				}
				else
				{
					for ( i=0; i<ImageH; i++ )
					{
						srcIdx = i*ImageStep;
						dstIdx = i*cvStep;
						::memcpy(&(dstPtr[dstIdx]), &(srcPtr[srcIdx]), CopyLen);
					}
				}
			}
			else
			{	::memcpy(dstPtr, srcPtr, sizeof(unsigned short)*cvStep*ImageH);	}
		}
		if ( 3 == nChannels )
		{
			short          *dstPtr = cvMatPtr->data.s;
			unsigned short *srcPtrR = Image.Get16uPtrR();
			unsigned short *srcPtrG = Image.Get16uPtrG();
			unsigned short *srcPtrB = Image.Get16uPtrB();			
			if ( CImageAPI::CheckPtr3(srcPtrR, srcPtrG, srcPtrB) == false ) { return false; }
			if ( true == Reverse )
			{
				for ( i=0; i<ImageH; i++ )
				{
					srcIdx = i*ImageStep;
					dstIdx = (ImageH-i-1)*cvStep;
					for ( j=0; j<ImageW; j++ )
					{
						dstPtr[dstIdx] = srcPtrB[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrG[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrR[srcIdx];	dstIdx++;
						srcIdx ++;
					}
				}
			}
			else
			{
				for ( i=0; i<ImageH; i++ )
				{
					srcIdx = i*ImageStep;
					dstIdx = i*cvStep;
					for ( j=0; j<ImageW; j++ )
					{
						dstPtr[dstIdx] = srcPtrB[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrG[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrR[srcIdx];	dstIdx++;
						srcIdx ++;
					}
				}
			}		
		}
	}
	if ( IMG_TYPE_32F == nType )
	{
		cvStep =  cvStep/4;
		if ( 1 == nChannels )
		{
			float  *dstPtr = cvMatPtr->data.fl;
			float  *srcPtr = Image.Get32fPtr();			
			if ( CImageAPI::CheckPtr(srcPtr) == false ) { return false; }
			CopyLen = CImageAPI::GetImageRealWidth(ImageW, BitCount)*sizeof(float);
			if ( ImageStep!=cvStep || true==Reverse )
			{
				if ( true == Reverse )
				{
					for ( i=0; i<ImageH; i++ )
					{
						srcIdx = i*ImageStep;
						dstIdx = (ImageH-i-1)*cvStep;
						::memcpy(&(dstPtr[dstIdx]), &(srcPtr[srcIdx]), CopyLen);
					}
				}
				else
				{
					for ( i=0; i<ImageH; i++ )
					{
						srcIdx = i*ImageStep;
						dstIdx = i*cvStep;
						::memcpy(&(dstPtr[dstIdx]), &(srcPtr[srcIdx]), CopyLen);
					}
				}
			}
			else
			{	::memcpy(dstPtr, srcPtr, sizeof(float)*cvStep*ImageH);	}
		}
		if ( 3 == nChannels )
		{
			float  *dstPtr  = cvMatPtr->data.fl;
			float  *srcPtrR = Image.Get32fPtrR();
			float  *srcPtrG = Image.Get32fPtrG();
			float  *srcPtrB = Image.Get32fPtrB();			
			if ( CImageAPI::CheckPtr3(srcPtrR, srcPtrG, srcPtrB) == false ) { return false; }
			if ( true == Reverse )
			{
				for ( i=0; i<ImageH; i++ )
				{
					srcIdx = i*ImageStep;
					dstIdx = (ImageH-i-1)*cvStep;
					for ( j=0; j<ImageW; j++ )
					{
						dstPtr[dstIdx] = srcPtrB[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrG[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrR[srcIdx];	dstIdx++;
						srcIdx ++;
					}
				}
			}
			else
			{
				for ( i=0; i<ImageH; i++ )
				{
					srcIdx = i*ImageStep;
					dstIdx = i*cvStep;
					for ( j=0; j<ImageW; j++ )
					{
						dstPtr[dstIdx] = srcPtrB[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrG[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrR[srcIdx];	dstIdx++;
						srcIdx ++;
					}
				}
			}		
		}
	}	
	return cvMatPtr;
}
//-------------------------------------------------------------------------------------//
IplImage* CImageAPI::ImageToCVImage(const TIMAGE &Image, bool Reverse, const char *fnName)
{
	int      cvType=0;
	const int nType      = Image.GetType();
	const int nChannels  = Image.GetChannels();
	if ( IMG_TYPE_000 == nType ) { return NULL; }
	IMAGE_SIZE ImageW    = Image.GetImageW();
	IMAGE_SIZE ImageH    = Image.GetImageH();
	IMAGE_SIZE ImageStep = Image.GetRowStep();
	IMAGE_SIZE BitCount  = Image.GetBitCount();
	switch ( nType ) 
	{
	case IMG_TYPE_08U:	cvType=IPL_DEPTH_8U; break;
	case IMG_TYPE_16U:	cvType=IPL_DEPTH_16U; break;
	case IMG_TYPE_32S:	cvType=IPL_DEPTH_32S; break;
	case IMG_TYPE_32F:	cvType=IPL_DEPTH_32F; break;	
	}	
	IplImage *cvImagePtr = CImageAPI::CreateCVImage(ImageW, ImageH, cvType, nChannels, fnName);
	if ( NULL == cvImagePtr ) { return NULL; }

	size_t i=0, j=0;
	size_t CopyLen = 0;
	size_t srcIdx=0, dstIdx=0;
	IMAGE_SIZE cvStep =  cvImagePtr->widthStep;
	if ( IMG_TYPE_08U == nType )
	{
		if ( 1 == nChannels )
		{			
			unsigned char *srcPtr = Image.Get08uPtr();
			unsigned char *dstPtr = (unsigned char*)cvImagePtr->imageData;
			if ( CImageAPI::CheckPtr(srcPtr) == false ) { return false; }
			CopyLen = CImageAPI::GetImageRealWidth(ImageW, BitCount)*sizeof(unsigned char);
			if ( ImageStep!=cvStep || true==Reverse )
			{
				if ( true == Reverse )
				{
					for ( i=0; i<ImageH; i++ )
					{
						srcIdx = i*ImageStep;
						dstIdx = (ImageH-i-1)*cvStep;
						::memcpy(&(dstPtr[dstIdx]), &(srcPtr[srcIdx]), CopyLen);
					}
				}
				else
				{
					for ( i=0; i<ImageH; i++ )
					{
						srcIdx = i*ImageStep;
						dstIdx = i*cvStep;
						::memcpy(&(dstPtr[dstIdx]), &(srcPtr[srcIdx]), CopyLen);
					}
				}
			}
			else
			{	::memcpy(dstPtr, srcPtr, sizeof(unsigned char)*cvStep*ImageH);	}
		}
		if ( 3 == nChannels )
		{
			unsigned char *srcPtrR = Image.Get08uPtrR();
			unsigned char *srcPtrG = Image.Get08uPtrG();
			unsigned char *srcPtrB = Image.Get08uPtrB();
			unsigned char *dstPtr = (unsigned char*)cvImagePtr->imageData;
			if ( CImageAPI::CheckPtr3(srcPtrR, srcPtrG, srcPtrB) == false ) { return false; }
			if ( true == Reverse )
			{
				for ( i=0; i<ImageH; i++ )
				{
					srcIdx = i*ImageStep;
					dstIdx = (ImageH-i-1)*cvStep;
					for ( j=0; j<ImageW; j++ )
					{
						dstPtr[dstIdx] = srcPtrB[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrG[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrR[srcIdx];	dstIdx++;
						srcIdx ++;
					}
				}
			}
			else
			{
				for ( i=0; i<ImageH; i++ )
				{
					srcIdx = i*ImageStep;
					dstIdx = i*cvStep;
					for ( j=0; j<ImageW; j++ )
					{
						dstPtr[dstIdx] = srcPtrB[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrG[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrR[srcIdx];	dstIdx++;
						srcIdx ++;
					}
				}
			}
		}
	}
	if ( IMG_TYPE_16U == nType )
	{
		cvStep =  cvStep/2;
		if ( 1 == nChannels )
		{			
			unsigned short *srcPtr = Image.Get16uPtr();
			unsigned short *dstPtr = (unsigned short*)cvImagePtr->imageData;
			if ( CImageAPI::CheckPtr(srcPtr) == false ) { return false; }
			CopyLen = CImageAPI::GetImageRealWidth(ImageW, BitCount)*sizeof(unsigned short);
			if ( ImageStep!=cvStep || true==Reverse )
			{
				if ( true == Reverse )
				{
					for ( i=0; i<ImageH; i++ )
					{
						srcIdx = i*ImageStep;
						dstIdx = (ImageH-i-1)*cvStep;
						::memcpy(&(dstPtr[dstIdx]), &(srcPtr[srcIdx]), CopyLen);
					}
				}
				else
				{
					for ( i=0; i<ImageH; i++ )
					{
						srcIdx = i*ImageStep;
						dstIdx = i*cvStep;
						::memcpy(&(dstPtr[dstIdx]), &(srcPtr[srcIdx]), CopyLen);
					}
				}
			}
			else
			{	::memcpy(dstPtr, srcPtr, sizeof(unsigned short)*cvStep*ImageH);	}
		}
		if ( 3 == nChannels )
		{			
			unsigned short *srcPtrR = Image.Get16uPtrR();
			unsigned short *srcPtrG = Image.Get16uPtrG();
			unsigned short *srcPtrB = Image.Get16uPtrB();
			unsigned short *dstPtr = (unsigned short*)cvImagePtr->imageData;
			if ( CImageAPI::CheckPtr3(srcPtrR, srcPtrG, srcPtrB) == false ) { return false; }
			if ( true == Reverse )
			{
				for ( i=0; i<ImageH; i++ )
				{
					srcIdx = i*ImageStep;
					dstIdx = (ImageH-i-1)*cvStep;
					for ( j=0; j<ImageW; j++ )
					{
						dstPtr[dstIdx] = srcPtrB[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrG[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrR[srcIdx];	dstIdx++;
						srcIdx ++;
					}
				}
			}
			else
			{
				for ( i=0; i<ImageH; i++ )
				{
					srcIdx = i*ImageStep;
					dstIdx = i*cvStep;
					for ( j=0; j<ImageW; j++ )
					{
						dstPtr[dstIdx] = srcPtrB[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrG[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrR[srcIdx];	dstIdx++;
						srcIdx ++;
					}
				}
			}		
		}
	}
	if ( IMG_TYPE_32F == nType )
	{
		cvStep =  cvStep/4;
		if ( 1 == nChannels )
		{			
			float *srcPtr = Image.Get32fPtr();
			float *dstPtr = (float*)cvImagePtr->imageData;
			if ( CImageAPI::CheckPtr(srcPtr) == false ) { return false; }
			CopyLen = CImageAPI::GetImageRealWidth(ImageW, BitCount)*sizeof(float);
			if ( ImageStep!=cvStep || true==Reverse )
			{
				if ( true == Reverse )
				{
					for ( i=0; i<ImageH; i++ )
					{
						srcIdx = i*ImageStep;
						dstIdx = (ImageH-i-1)*cvStep;
						::memcpy(&(dstPtr[dstIdx]), &(srcPtr[srcIdx]), CopyLen);
					}
				}
				else
				{
					for ( i=0; i<ImageH; i++ )
					{
						srcIdx = i*ImageStep;
						dstIdx = i*cvStep;
						::memcpy(&(dstPtr[dstIdx]), &(srcPtr[srcIdx]), CopyLen);
					}
				}
			}
			else
			{	::memcpy(dstPtr, srcPtr, sizeof(float)*cvStep*ImageH);	}
		}
		if ( 3 == nChannels )
		{			
			float *srcPtrR = Image.Get32fPtrR();
			float *srcPtrG = Image.Get32fPtrG();
			float *srcPtrB = Image.Get32fPtrB();
			float *dstPtr = (float*)cvImagePtr->imageData;
			if ( CImageAPI::CheckPtr3(srcPtrR, srcPtrG, srcPtrB) == false ) { return false; }
			if ( true == Reverse )
			{
				for ( i=0; i<ImageH; i++ )
				{
					srcIdx = i*ImageStep;
					dstIdx = (ImageH-i-1)*cvStep;
					for ( j=0; j<ImageW; j++ )
					{
						dstPtr[dstIdx] = srcPtrB[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrG[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrR[srcIdx];	dstIdx++;
						srcIdx ++;
					}
				}
			}
			else
			{
				for ( i=0; i<ImageH; i++ )
				{
					srcIdx = i*ImageStep;
					dstIdx = i*cvStep;
					for ( j=0; j<ImageW; j++ )
					{
						dstPtr[dstIdx] = srcPtrB[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrG[srcIdx];	dstIdx++;
						dstPtr[dstIdx] = srcPtrR[srcIdx];	dstIdx++;
						srcIdx ++;
					}
				}
			}		
		}
	}		
	return cvImagePtr;
}
//-------------------------------------------------------------------------------------//
IplImage* CImageAPI::GrayImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, bool Reverse, int &CreateMode)//灰階影像轉成cv影像
{
	if ( this->CheckGrayImage(ImageW, ImageH, ImageStep, pImage) == false )
	{	return NULL; }

	IplImage *cvImagePtr = NULL;	
	const unsigned int BitCount = 8;
	const unsigned int channel = GetImageChannels(BitCount);
	const unsigned int cvRowStep = GetImageAlignedWidth(ImageW, BitCount, 4);
	
	if ( cvRowStep!=ImageStep || true==Reverse || CREATE_OPENCV_IMAGE_BODY==CreateMode)
	{		
		cvImagePtr = CreateCVImage(ImageW, ImageH, IPL_DEPTH_8U, channel, "CImageAPI::GrayImageToCVImage");
		if ( NULL == cvImagePtr ) 
		{
			CreateMode = CREATE_OPENCV_IMAGE_NULL;
			return NULL;	
		}

		CreateMode =  CREATE_OPENCV_IMAGE_BODY;
		size_t i=0, j=0;
		size_t srcidx=0, destidx=0;
		const size_t LineLen = this->GetImageRealWidth(ImageW, BitCount);
		const size_t LineSize = sizeof(unsigned char)*LineLen;
		if ( false == Reverse )
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*ImageStep;
				destidx = i*cvImagePtr->widthStep;			
				::memcpy(&(cvImagePtr->imageData[destidx]), &(pImage[srcidx]), LineSize);
			}
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*ImageStep;
				destidx = (ImageH-i-1)*cvImagePtr->widthStep;			
				::memcpy(&(cvImagePtr->imageData[destidx]), &(pImage[srcidx]), LineSize);
			}
		}
	}
	else
	{			
		cvImagePtr = CreateCVImageHeader(ImageW, ImageH, IPL_DEPTH_8U, channel);
		if ( NULL == cvImagePtr )
		{
			CreateMode = CREATE_OPENCV_IMAGE_NULL;
			return NULL;	
		}
		cvImagePtr->imageData = (char*)(pImage);		
		CreateMode =  CREATE_OPENCV_IMAGE_HEADER;		
	}	
	return cvImagePtr;
}
//-------------------------------------------------------------------------------------//
IplImage* CImageAPI::ColorImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, bool Reverse, int &CreateMode)//彩色影像轉成cv影像
{
	if ( this->CheckColorImage(ImageW, ImageH, ImageStep, pImage) == false )
	{	return NULL; }

	IplImage *cvImagePtr = NULL;	
	const unsigned int BitCount = 24;
	const unsigned int channel = this->GetImageChannels(BitCount);
	const unsigned int cvRowStep = CImageAPI::GetImageAlignedWidth(ImageW, BitCount, 4);
	
	if ( cvRowStep!=ImageStep || true==Reverse || CREATE_OPENCV_IMAGE_BODY==CreateMode )
	{		
		cvImagePtr = CreateCVImage(ImageW, ImageH, IPL_DEPTH_8U, channel, "CImageAPI::ColorImageToCVImage");
		if ( NULL == cvImagePtr ) 
		{
			CreateMode = CREATE_OPENCV_IMAGE_NULL;
			return NULL;	
		}

		CreateMode =  CREATE_OPENCV_IMAGE_BODY;
		size_t i=0, j=0;
		size_t srcidx=0, destidx=0;
		const size_t LineLen = this->GetImageRealWidth(ImageW, BitCount);
		const size_t LineSize = sizeof(unsigned char)*LineLen;
		if ( false == Reverse ) 
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*ImageStep;
				destidx = i*cvImagePtr->widthStep;			
				::memcpy(&(cvImagePtr->imageData[destidx]), &(pImage[srcidx]), LineSize);
			}
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*ImageStep;
				destidx = (ImageH-i-1)*cvImagePtr->widthStep;			
				::memcpy(&(cvImagePtr->imageData[destidx]), &(pImage[srcidx]), LineSize);
			}
		}
	}
	else
	{	
		cvImagePtr = CreateCVImageHeader(ImageW, ImageH, IPL_DEPTH_8U, channel);
		if ( NULL == cvImagePtr )
		{
			CreateMode = CREATE_OPENCV_IMAGE_NULL;
			return NULL;	
		}
		cvImagePtr->imageData = (char*)(pImage);		
		CreateMode =  CREATE_OPENCV_IMAGE_HEADER;		
	}	
	return cvImagePtr;
}
//-------------------------------------------------------------------------------------//
IplImage* CImageAPI::RGBImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, bool Reverse)//RGB影像轉成cv影像
{	
	if ( this->CheckGrayImage3(ImageW, ImageH, ImageStep, pR, pG, pB) == false )
	{	return NULL; }		
	
	IplImage  *cvImagePtr = CreateCVImage(ImageW, ImageH, IPL_DEPTH_8U, 3, "CImageAPI::RGBImageToCVImage");
	if ( NULL == cvImagePtr ) 
	{	return NULL;	}

	size_t i=0, j=0;		
	size_t srcidx=0, destidx=0;
	if ( Reverse == false )
	{
		for ( i=0; i<ImageH; i++ )
		{
			srcidx = i*ImageStep;
			destidx = i*cvImagePtr->widthStep;
			for ( j=0; j<ImageW; j++ )
			{	
				cvImagePtr->imageData[destidx++] = pB[srcidx];
				cvImagePtr->imageData[destidx++] = pG[srcidx];
				cvImagePtr->imageData[destidx++] = pR[srcidx];
				srcidx ++;
			}
		}		
	}
	else
	{
		for ( i=0; i<ImageH; i++ )
		{
			srcidx = i*ImageStep;
			destidx = (ImageH-i-1)*cvImagePtr->widthStep;
			for ( j=0; j<ImageW; j++ )
			{	
				cvImagePtr->imageData[destidx++] = pB[srcidx];
				cvImagePtr->imageData[destidx++] = pG[srcidx];
				cvImagePtr->imageData[destidx++] = pR[srcidx];
				srcidx ++;
			}
		}	
	}
	return cvImagePtr;
}
//-------------------------------------------------------------------------------------//
IplImage* CImageAPI::ShortGrayImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const short *pImage, bool Reverse, int &CreateMode)//灰階影像轉成cv影像
{
	if ( this->CheckGrayImage(ImageW, ImageH, ImageStep, pImage) == false )
	{	return NULL; }
		
	IplImage* cvImagePtr = CreateCVImage(ImageW, ImageH, IPL_DEPTH_16S, 1, "CImageAPI::ShortGrayImageToCVImage");
	if ( NULL == cvImagePtr ) 
	{	return NULL;	}

	IMAGE_SIZE cvStep = cvImagePtr->widthStep/2;//1 data => 2 Bytes
	if ( cvStep!=ImageStep || true==Reverse )
	{
		size_t i=0, j=0;
		size_t srcidx=0, destidx=0;
		size_t range = sizeof(short)*ImageW;
		unsigned char *pDest = (unsigned char*)(cvImagePtr->imageData);//注意以unsigned char來移動指標
		if ( false == Reverse )
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*ImageStep;
				destidx = i*cvImagePtr->widthStep;
				::memcpy(&(pDest[destidx]), &(pImage[srcidx]), range);
			}
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*ImageStep;
				destidx = (ImageH-i-1)*cvImagePtr->widthStep;
				::memcpy(&(pDest[destidx]), &(pImage[srcidx]), range);
			}
		}
	}
	else
	{
		const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
		::memcpy(cvImagePtr->imageData, pImage, sizeof(short)*BufferSize);	
	}
	CreateMode =  CREATE_OPENCV_IMAGE_BODY;
	return cvImagePtr;
}
//-------------------------------------------------------------------------------------//
IplImage* CImageAPI::ShortGrayImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, bool Reverse, int &CreateMode)//灰階影像轉成cv影像
{
	if ( this->CheckGrayImage(ImageW, ImageH, ImageStep, pImage) == false )
	{	return NULL; }
		
	IplImage* cvImagePtr = CreateCVImage(ImageW, ImageH, IPL_DEPTH_16U, 1, "CImageAPI::ShortGrayImageToCVImage");
	if ( NULL == cvImagePtr ) 
	{	return NULL;	}

	IMAGE_SIZE cvStep = cvImagePtr->widthStep/2;//1 data => 2 Bytes
	if ( cvStep!=ImageStep || true==Reverse )
	{
		size_t i=0, j=0;
		size_t srcidx=0, destidx=0;
		size_t range = sizeof(unsigned short)*ImageW;
		unsigned char *pDest = (unsigned char*)(cvImagePtr->imageData);//注意以unsigned char來移動指標
		if ( false == Reverse )
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*ImageStep;
				destidx = i*cvImagePtr->widthStep;
				::memcpy(&(pDest[destidx]), &(pImage[srcidx]), range);
			}
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*ImageStep;
				destidx = (ImageH-i-1)*cvImagePtr->widthStep;
				::memcpy(&(pDest[destidx]), &(pImage[srcidx]), range);
			}
		}
	}
	else
	{
		const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
		::memcpy(cvImagePtr->imageData, pImage, sizeof(unsigned short)*BufferSize);	
	}
	CreateMode =  CREATE_OPENCV_IMAGE_BODY;
	return cvImagePtr;
}
//-------------------------------------------------------------------------------------//
IplImage* CImageAPI::ShortColorImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pImage, bool Reverse)//彩色影像轉成cv影像
{
	if ( this->CheckColorImage(ImageW, ImageH, ImageStep, pImage) == false )
	{	return NULL; }
	
	IplImage* cvImagePtr = CreateCVImage(ImageW, ImageH, IPL_DEPTH_16U, 3, "CImageAPI::ShortColorImageToCVImage");
	if ( NULL == cvImagePtr ) 
	{	return NULL;	}

	IMAGE_SIZE cvStep = cvImagePtr->widthStep/2;//1 data => 2 Bytes
	if ( cvStep!=ImageStep || true==Reverse )
	{	
		size_t i=0, j=0;
		size_t srcidx=0, destidx=0;
		size_t range = sizeof(unsigned short)*ImageW*3;
		unsigned char *pDest = (unsigned char*)(cvImagePtr->imageData);//注意以unsigned char來移動指標

		if ( false == Reverse )
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*ImageStep;
				destidx = i*cvImagePtr->widthStep;
				::memcpy(&(pDest[destidx]), &(pImage[srcidx]), range);
			}
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*ImageStep;
				destidx = (ImageH-i-1)*cvImagePtr->widthStep;
				::memcpy(&(pDest[destidx]), &(pImage[srcidx]), range);
			}
		}
	}
	else
	{
		const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
		::memcpy(cvImagePtr->imageData, pImage, sizeof(unsigned short)*BufferSize);	
	}
	return cvImagePtr;
}
//-------------------------------------------------------------------------------------//
IplImage* CImageAPI::ShortRGBImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned short *pR, const unsigned short *pG, const unsigned short *pB, bool Reverse)//RGB影像轉成cv影像
{
	if ( this->CheckGrayImage3(ImageW, ImageH, ImageStep, pR, pG, pB) == false )
	{	return NULL; }	
	
	IplImage* cvImagePtr = CreateCVImage(ImageW, ImageH, IPL_DEPTH_16U, 3, "CImageAPI::ShortRGBImageToCVImage");
	if ( NULL == cvImagePtr ) 
	{	return NULL;	}

	size_t i=0, j=0;		
	size_t srcidx=0, destidx=0;
	size_t cvStep = cvImagePtr->widthStep/2;//1 data => 2 Bytes
	unsigned short *pDest = (unsigned short*)(cvImagePtr->imageData);
	if ( false == Reverse )
	{
		for ( i=0; i<ImageH; i++ )
		{
			srcidx = i*ImageStep;
			destidx = i*cvImagePtr->widthStep;
			for ( j=0; j<ImageW; j++ )
			{	
				pDest[destidx++] = pB[srcidx];
				pDest[destidx++] = pG[srcidx];
				pDest[destidx++] = pR[srcidx];
				srcidx ++;
			}
		}		
	}
	else
	{
		for ( i=0; i<ImageH; i++ )
		{
			srcidx = i*ImageStep;
			destidx = (ImageH-i-1)*cvImagePtr->widthStep;
			for ( j=0; j<ImageW; j++ )
			{	
				pDest[destidx++] = pB[srcidx];
				pDest[destidx++] = pG[srcidx];
				pDest[destidx++] = pR[srcidx];
				srcidx ++;
			}
		}
	}
	return cvImagePtr;
}
//-------------------------------------------------------------------------------------//
IplImage* CImageAPI::FloatGrayImageToCVImage(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const float *pImage, bool Reverse, int &CreateMode)//浮點數影像轉成cv影像
{
	if ( this->CheckGrayImage(ImageW, ImageH, ImageStep, pImage) == false )
	{	return NULL; }
		
	IplImage* cvImagePtr = CreateCVImage(ImageW, ImageH, IPL_DEPTH_32F, 1, "CImageAPI::FloatGrayImageToCVImage");
	if ( NULL == cvImagePtr ) 
	{	return NULL;	}

	IMAGE_SIZE cvStep = cvImagePtr->widthStep/4;//1 data => 4 Bytes
	if ( cvStep!=ImageStep || true==Reverse )
	{
		size_t i=0, j=0;
		size_t srcidx=0, destidx=0;
		size_t range = sizeof(float)*ImageW;
		float *pDest = (float*)(cvImagePtr->imageData);//注意以unsigned char來移動指標
		if ( false == Reverse )
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*ImageStep;
				destidx = i*cvImagePtr->widthStep;
				::memcpy(&(pDest[destidx]), &(pImage[srcidx]), range);
			}
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				srcidx = i*ImageStep;
				destidx = (ImageH-i-1)*cvImagePtr->widthStep;
				::memcpy(&(pDest[destidx]), &(pImage[srcidx]), range);
			}
		}
	}
	else
	{
		const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
		::memcpy(cvImagePtr->imageData, pImage, sizeof(float)*BufferSize);	
	}
	CreateMode =  CREATE_OPENCV_IMAGE_BODY;
	return cvImagePtr;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::LoadCVImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, unsigned char *&pImage, int Align, bool Reverse)//讀取CV支援圖檔
{
	const char fnName[] = "CImageAPI::LoadCVImage";
	const size_t chBufferSize = MAX_JET_PATH;
	char filename[chBufferSize]={0};
	JetAPI::TCHAR2char(pfilename, filename, chBufferSize);
#if _MSC_VER == VC_6
	IplImage * pImg = ::cvLoadImage(filename, 0);//cvSaveImage
#else
	IplImage * pImg = ::cvLoadImage(filename, CV_LOAD_IMAGE_ANYCOLOR);//cvSaveImage
#endif//_MSC_VER == VC_6
	if ( pImg == NULL ) 
	{ 
		LockImageAPI();
		this->m_ErrorString.Format(_T("Error, Load cvImage File Fault. (%s)"), pfilename);
		UnlockImageAPI();
		return false; 
	}
	ImageW = pImg->width;
	ImageH = pImg->height;		
	if ( 3 == pImg->nChannels ) 
	{	BitCount = 24; }
	else
	{	BitCount = 8; }
	ImageStep = CImageAPI::GetImageAlignedWidth(ImageW, BitCount, Align);
	JetMemory.free_func(pImage);
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, pImage, fnName, "pImage") == false )
	{
		::cvReleaseImage(&pImg);
		SetErrorString(JetMemory.GetErrorString());
		return false;
	}
	Reverse = AdjustSaveLoadImageReverse(Reverse);//注意為了與CDib的讀取方式相同
	if ( true==Reverse || ImageStep!=pImg->widthStep )
	{
		size_t i=0;
		size_t SrcIdx=0, DstIdx=0;
		const size_t CopyLen = ImageW*pImg->nChannels;
		if ( true == Reverse )
		{
			for ( i=0; i<ImageH; i++ )
			{
				SrcIdx = i*pImg->widthStep;
				DstIdx = (ImageH-1-i)*ImageStep;
				::memcpy(&(pImage[DstIdx]), &(pImg->imageData[SrcIdx]), CopyLen);				
			}
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				DstIdx = i*ImageStep;
				SrcIdx = i*pImg->widthStep;				
				::memcpy(&(pImage[DstIdx]), &(pImg->imageData[SrcIdx]), CopyLen);				
			}
		}
	}
	else
	{	::memcpy(pImage, pImg->imageData, sizeof(unsigned char)*BufferSize);	}
	::cvReleaseImage(&pImg);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::LoadCVGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage, int Align, bool Reverse)//讀取V支援灰階圖檔
{
	const char fnName[] = "CImageAPI::LoadCVGrayImage";
	const size_t chBufferSize = MAX_JET_PATH;
	char filename[chBufferSize]={0};
	JetAPI::TCHAR2char(pfilename, filename, chBufferSize);
#if _MSC_VER == VC_6
	IplImage * pImg = ::cvLoadImage(filename, -1);//cvSaveImage
#else
	IplImage * pImg = ::cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);//cvSaveImage
#endif//_MSC_VER == VC_6
	if ( pImg == NULL ) 
	{ 
		LockImageAPI();
		this->m_ErrorString.Format(_T("Error, Load cvImage File Fault. (%s)"), pfilename);
		UnlockImageAPI();
		return false; 
	}
	ImageW = pImg->width;
	ImageH = pImg->height;		
	if ( 1 != pImg->nChannels ) 
	{			
		LockImageAPI();
		this->m_ErrorString.Format(_T("Error, Load cvImage File Fault (channel:%d)"), pImg->nChannels);
		UnlockImageAPI();
		::cvReleaseImage(&pImg);
		return false;
	}
	
	const IMAGE_SIZE BitCount = 8;
	ImageStep = CImageAPI::GetImageAlignedWidth(ImageW, BitCount, Align);
	JetMemory.free_func(pImage);
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, pImage, fnName, "pImage") == false )
	{
		::cvReleaseImage(&pImg);
		SetErrorString(JetMemory.GetErrorString());
		return false;
	}
	Reverse = AdjustSaveLoadImageReverse(Reverse);//注意為了與CDib的讀取方式相同
	if ( true==Reverse || ImageStep!=pImg->widthStep )
	{
		size_t i=0;
		size_t SrcIdx=0, DstIdx=0;
		const size_t CopyLen = ImageW*pImg->nChannels;
		if ( true == Reverse )
		{
			for ( i=0; i<ImageH; i++ )
			{
				SrcIdx = i*pImg->widthStep;
				DstIdx = (ImageH-1-i)*ImageStep;
				::memcpy(&(pImage[DstIdx]), &(pImg->imageData[SrcIdx]), CopyLen);				
			}
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				DstIdx = i*ImageStep;
				SrcIdx = i*pImg->widthStep;				
				::memcpy(&(pImage[DstIdx]), &(pImg->imageData[SrcIdx]), CopyLen);				
			}
		}
	}
	else
	{	::memcpy(pImage, pImg->imageData, sizeof(unsigned char)*BufferSize);	}
	::cvReleaseImage(&pImg);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::LoadCVColorImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage, int Align, bool Reverse)//讀取V支援彩色圖檔
{
	const char fnName[] = "CImageAPI::LoadCVColorImage";
	const size_t chBufferSize = MAX_JET_PATH;
	char filename[chBufferSize]={0};
	JetAPI::TCHAR2char(pfilename, filename, chBufferSize);
#if _MSC_VER == VC_6
	IplImage * pImg = ::cvLoadImage(filename, 1);//cvSaveImage
#else
	IplImage * pImg = ::cvLoadImage(filename, CV_LOAD_IMAGE_COLOR);//cvSaveImage
#endif//_MSC_VER == VC_6
	if ( pImg == NULL ) 
	{ 
		LockImageAPI();
		this->m_ErrorString.Format(_T("Error, Load cvImage File Fault. (%s)"), pfilename);
		UnlockImageAPI();
		return false; 
	}
	ImageW = pImg->width;
	ImageH = pImg->height;		
	if ( 3 != pImg->nChannels ) 
	{
		LockImageAPI();
		this->m_ErrorString.Format(_T("Error, Load cvImage File Fault (channel:%d)"), pImg->nChannels);
		UnlockImageAPI();
		::cvReleaseImage(&pImg);
		return false;
	}
	
	const IMAGE_SIZE BitCount = 24;
	ImageStep = CImageAPI::GetImageAlignedWidth(ImageW, BitCount, Align);
	JetMemory.free_func(pImage);
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, pImage, fnName, "pImage") == false )
	{	
		::cvReleaseImage(&pImg);
		SetErrorString(JetMemory.GetErrorString());
		return false;
	}
	Reverse = AdjustSaveLoadImageReverse(Reverse);//注意為了與CDib的讀取方式相同
	if ( true==Reverse || ImageStep!=pImg->widthStep )
	{
		size_t i=0;
		size_t SrcIdx=0, DstIdx=0;
		const size_t CopyLen = ImageW*pImg->nChannels;
		if ( true == Reverse )
		{
			for ( i=0; i<ImageH; i++ )
			{
				SrcIdx = i*pImg->widthStep;
				DstIdx = (ImageH-1-i)*ImageStep;
				::memcpy(&(pImage[DstIdx]), &(pImg->imageData[SrcIdx]), CopyLen);				
			}
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				DstIdx = i*ImageStep;
				SrcIdx = i*pImg->widthStep;				
				::memcpy(&(pImage[DstIdx]), &(pImg->imageData[SrcIdx]), CopyLen);				
			}
		}
	}
	else
	{	::memcpy(pImage, pImg->imageData, sizeof(unsigned char)*BufferSize);	}
	::cvReleaseImage(&pImg);
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::SaveCVImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, bool Reverse, const int* Params)//儲存PNG圖檔
{
	//return SaveMatImage(pfilename, ImageW, ImageH, ImageStep, BitCount, pImage, Reverse);
	if ( CImageAPI::CheckBitCount(BitCount) == false ) { return false; }
	if ( 8 == BitCount ) 
	{	return CImageAPI::SaveCVGrayImage(pfilename, ImageW, ImageH, ImageStep, pImage, Reverse, Params); }
	if ( 24 == BitCount ) 
	{	return CImageAPI::SaveCVColorImage(pfilename, ImageW, ImageH, ImageStep, pImage, Reverse, Params); }
	return false;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::SaveCVGrayImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, bool Reverse, const int* Params)//儲存PNG灰階圖檔
{
	const size_t chBufferSize = MAX_JET_PATH;
	char filename[chBufferSize]="";
	JetAPI::TCHAR2char(pfilename, filename, chBufferSize);
	int       CreateMode = CREATE_OPENCV_IMAGE_NULL;

	Reverse = AdjustSaveLoadImageReverse(Reverse);//注意為了與CDib的讀取方式相同
	IplImage *cvImagePtr = GrayImageToCVImage(ImageW, ImageH, ImageStep, pImage, Reverse, CreateMode);
	if ( NULL == cvImagePtr ) 
	{	return false; }

	int Res = cvSaveImage(filename, cvImagePtr, Params);
	DestroyCVImage(CreateMode, cvImagePtr);		

	if ( 1 != Res )
	{
		LockImageAPI();
		this->m_ErrorString.Format(_T("Error, Save cvImage File Fault(%s)"), pfilename);
		UnlockImageAPI();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::SaveCVColorImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pImage, bool Reverse, const int* Params)//儲存PNG彩色圖檔	
{
	const size_t chBufferSize = MAX_JET_PATH;
	char filename[chBufferSize]="";
	JetAPI::TCHAR2char(pfilename, filename, chBufferSize);

	int       CreateMode = CREATE_OPENCV_IMAGE_NULL;	
	Reverse = AdjustSaveLoadImageReverse(Reverse);//注意為了與CDib的讀取方式相同
	IplImage *cvImagePtr = ColorImageToCVImage(ImageW, ImageH, ImageStep, pImage, Reverse, CreateMode);
	if ( NULL == cvImagePtr ) 
	{	return false; }

	int Res = cvSaveImage(filename, cvImagePtr, Params);	
	DestroyCVImage(CreateMode, cvImagePtr);			

	if ( 1 != Res )
	{
		LockImageAPI();
		this->m_ErrorString.Format(_T("Error, Save cvImage File Fault(%s)"), pfilename);
		UnlockImageAPI();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::SaveCVRGBImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, const unsigned char *pR, const unsigned char *pG, const unsigned char *pB, bool Reverse)//儲存RGB彩色圖檔	
{
	const size_t chBufferSize = MAX_JET_PATH;
	char filename[chBufferSize]="";
	JetAPI::TCHAR2char(pfilename, filename, chBufferSize);

	int       CreateMode = CREATE_OPENCV_IMAGE_BODY;
	Reverse = AdjustSaveLoadImageReverse(Reverse);//注意為了與CDib的讀取方式相同
	IplImage *cvImagePtr = RGBImageToCVImage(ImageW, ImageH, ImageStep, pR, pG, pB, Reverse);
	if ( NULL == cvImagePtr ) 
	{	return false; }

	int Res = cvSaveImage(filename, cvImagePtr);	
	DestroyCVImage(CreateMode, cvImagePtr);			

	if ( 1 != Res )
	{
		LockImageAPI();
		this->m_ErrorString.Format(_T("Error, Save cvImage File Fault(%s)"), pfilename);
		UnlockImageAPI();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::SaveMatImage(LPCTSTR pfilename, IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const unsigned char *pImage, bool Reverse, const std::vector<int> &Params)//儲存Mat圖檔
{	
	const char fnName[] = "CImageAPI::SaveMatImage";
	if ( CheckImage(ImageW, ImageH, ImageStep, BitCount, pImage) == false ) { return false; }
	unsigned char *BufferPtr=NULL;
	const size_t BufferSize=CalcBufferSize(ImageStep, ImageH);
	try 
	{	//cv::Mat的資料不會預設使用4倍數的對齊格式, 因此先不使用
		bool bIsOK=true;
		const size_t chBufferSize = MAX_JET_PATH;
		char filename[chBufferSize]="";
		JetAPI::TCHAR2char(pfilename, filename, chBufferSize);
		Reverse = AdjustSaveLoadImageReverse(Reverse);//注意為了與CDib的讀取方式相同
		if ( true == Reverse )
		{
			if ( JetMemory.alloc_func(BufferSize, BufferPtr, fnName, "BufferPtr") == false )
			{
				SetErrorString(JetMemory.GetErrorString());
				return false;
			}
			if ( ReverseImage3(ImageW, ImageH, ImageStep, BitCount, pImage, BufferPtr) == false )
			{	
				JetMemory.free_func(BufferPtr);
				return false; 
			}			
		}
		else
		{	BufferPtr = (unsigned char*)pImage;	}
		cv::Mat Img = CreateMat(ImageW, ImageH, ImageStep, BitCount, BufferPtr);
		if ( CheckMatIsValid(Img) == false )
		{
			if ( BufferPtr != pImage ) 
			{	JetMemory.free_func(BufferPtr); }
			return false; 
		}
		bIsOK = cv::imwrite(filename, Img, Params);
		if ( BufferPtr != pImage ) 
		{	JetMemory.free_func(BufferPtr); }
		if ( false == bIsOK )
		{
			LockImageAPI();
			m_ErrorString.Format(_T("Error, Save Mat Image Fualt[%s]"), pfilename);	
			UnlockImageAPI();
		}
		return bIsOK;
	}
	catch ( cv::Exception& e )
	{	
		CatchOpenCVException(e);
		if ( BufferPtr != pImage ) 
		{	JetMemory.free_func(BufferPtr); }
		return false;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::LoadMatImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, unsigned char *&pImage, int Align, bool Reverse)//讀取Mat支援圖檔
{
	const char fnName[] = "CImageAPI::LoadMatImage";
	try
	{	//cv::Mat的資料不會預設使用4倍數的對齊格式, 因此先不使用
		const size_t chBufferSize = MAX_JET_PATH;
		char filename[chBufferSize]={0};
		JetAPI::TCHAR2char(pfilename, filename, chBufferSize);
		cv::Mat Img=cv::imread(filename, cv::IMREAD_ANYCOLOR);		
		if ( CheckMatIsValid(Img)==false )
		{	
			LockImageAPI();
			this->m_ErrorString.Format(_T("Error, Load Mat File Fault. (%s)"), pfilename);
			UnlockImageAPI();
			return false; 
		}
		ImageW = Img.cols;
		ImageH = Img.rows;
		unsigned char *MPtr = Img.data;
		const int nChannels = Img.channels();
		const int MatStep = Img.step1();
		if ( 3 == nChannels ) 
		{	BitCount = 24; }
		else
		{	BitCount = 8; }
		ImageStep = GetImageAlignedWidth(ImageW, BitCount, Align);
		JetMemory.free_func(pImage);
		
		const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize, pImage, fnName, "pImage") == false )
		{
			SetErrorString(JetMemory.GetErrorString());
			return false;
		}
		Reverse = AdjustSaveLoadImageReverse(Reverse);//注意為了與CDib的讀取方式相同
		if ( true==Reverse || ImageStep!=MatStep )
		{
			size_t i=0;
			size_t SrcIdx=0, DstIdx=0;
			const size_t CopyLen = ImageW*nChannels;
			if ( true == Reverse )
			{
				for ( i=0; i<ImageH; i++ )
				{
					SrcIdx = i*MatStep;
					DstIdx = (ImageH-1-i)*ImageStep;
					::memcpy(&(pImage[DstIdx]), &(MPtr[SrcIdx]), CopyLen);				
				}
			}
			else
			{
				for ( i=0; i<ImageH; i++ )
				{
					DstIdx = i*ImageStep;
					SrcIdx = i*MatStep;				
					::memcpy(&(pImage[DstIdx]), &(MPtr[SrcIdx]), CopyLen);				
				}
			}
		}
		else
		{	::memcpy(pImage, MPtr, sizeof(unsigned char)*BufferSize);	}		
		return true;
	}
	catch ( cv::Exception& e )
	{	
		CatchOpenCVException(e);	
		JetMemory.free_func(pImage);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::LoadMatGrayImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage, int Align, bool Reverse)//讀取Mat支援灰階圖檔
{
	const char fnName[] = "CImageAPI::LoadMatGrayImage";
	try
	{	//cv::Mat的資料不會預設使用4倍數的對齊格式, 因此先不使用
		const size_t chBufferSize = MAX_JET_PATH;
		char filename[chBufferSize]={0};
		JetAPI::TCHAR2char(pfilename, filename, chBufferSize);
		cv::Mat Img=cv::imread(filename, cv::IMREAD_GRAYSCALE);		
		if ( CheckMatIsValid(Img)==false )
		{	
			LockImageAPI();
			this->m_ErrorString.Format(_T("Error, Load Mat File Fault. (%s)"), pfilename);
			UnlockImageAPI();
			return false; 
		}
		ImageW = Img.cols;
		ImageH = Img.rows;
		unsigned char *MPtr = Img.data;
		const int nChannels = Img.channels();
		const int MatStep = Img.step1();
		if ( 1 != nChannels ) 
		{		
			LockImageAPI();
			this->m_ErrorString.Format(_T("Error, Load Mat File Fault (channel:%d)"), nChannels);			
			UnlockImageAPI();
			return false;
		}
		const IMAGE_SIZE BitCount = 8;
		ImageStep = GetImageAlignedWidth(ImageW, BitCount, Align);
		JetMemory.free_func(pImage);
		const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize, pImage, fnName, "pImage") == false )
		{
			SetErrorString(JetMemory.GetErrorString());
			return false;
		}
		Reverse = AdjustSaveLoadImageReverse(Reverse);//注意為了與CDib的讀取方式相同
		if ( true==Reverse || ImageStep!=MatStep )
		{
			size_t i=0;
			size_t SrcIdx=0, DstIdx=0;
			const size_t CopyLen = ImageW*nChannels;
			if ( true == Reverse )
			{
				for ( i=0; i<ImageH; i++ )
				{
					SrcIdx = i*MatStep;
					DstIdx = (ImageH-1-i)*ImageStep;
					::memcpy(&(pImage[DstIdx]), &(MPtr[SrcIdx]), CopyLen);				
				}
			}
			else
			{
				for ( i=0; i<ImageH; i++ )
				{
					DstIdx = i*ImageStep;
					SrcIdx = i*MatStep;				
					::memcpy(&(pImage[DstIdx]), &(MPtr[SrcIdx]), CopyLen);				
				}
			}
		}
		else
		{	::memcpy(pImage, MPtr, sizeof(unsigned char)*BufferSize);	}		
		return true;
	}
	catch ( cv::Exception& e )
	{	
		CatchOpenCVException(e);	
		JetMemory.free_func(pImage);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::LoadMatColorImage(LPCTSTR pfilename, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, unsigned char *&pImage, int Align, bool Reverse)//讀取Mat支援彩色圖檔
{
	const char fnName[] = "CImageAPI::LoadMatColorImage";
	try
	{	//cv::Mat的資料不會預設使用4倍數的對齊格式, 因此先不使用
		const size_t chBufferSize = MAX_JET_PATH;
		char filename[chBufferSize]={0};
		JetAPI::TCHAR2char(pfilename, filename, chBufferSize);
		cv::Mat Img=cv::imread(filename, cv::IMREAD_COLOR);		
		if ( CheckMatIsValid(Img)==false )
		{	
			LockImageAPI();
			this->m_ErrorString.Format(_T("Error, Load Mat File Fault. (%s)"), pfilename);
			UnlockImageAPI();
			return false; 
		}
		ImageW = Img.cols;
		ImageH = Img.rows;
		unsigned char *MPtr = Img.data;
		const int nChannels = Img.channels();
		const int MatStep = Img.step1();
		if ( 3 != nChannels ) 
		{		
			LockImageAPI();
			this->m_ErrorString.Format(_T("Error, Load Mat File Fault (channel:%d)"), nChannels);
			UnlockImageAPI();
			return false;
		}
		const IMAGE_SIZE BitCount = 24;
		ImageStep = GetImageAlignedWidth(ImageW, BitCount, Align);
		JetMemory.free_func(pImage);
		const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
		if ( JetMemory.alloc_func(BufferSize, pImage, fnName, "pImage") == false )
		{
			SetErrorString(JetMemory.GetErrorString());
			return false;
		}
		Reverse = AdjustSaveLoadImageReverse(Reverse);//注意為了與CDib的讀取方式相同
		if ( true==Reverse || ImageStep!=MatStep )
		{
			size_t i=0;
			size_t SrcIdx=0, DstIdx=0;
			const size_t CopyLen = ImageW*nChannels;
			if ( true == Reverse )
			{
				for ( i=0; i<ImageH; i++ )
				{
					SrcIdx = i*MatStep;
					DstIdx = (ImageH-1-i)*ImageStep;
					::memcpy(&(pImage[DstIdx]), &(MPtr[SrcIdx]), CopyLen);				
				}
			}
			else
			{
				for ( i=0; i<ImageH; i++ )
				{
					DstIdx = i*ImageStep;
					SrcIdx = i*MatStep;				
					::memcpy(&(pImage[DstIdx]), &(MPtr[SrcIdx]), CopyLen);				
				}
			}
		}
		else
		{	::memcpy(pImage, MPtr, sizeof(unsigned char)*BufferSize);	}		
		return true;
	}
	catch ( cv::Exception& e )
	{	
		CatchOpenCVException(e);	
		JetMemory.free_func(pImage);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
int CImageAPI::GetCVMorphMode(int MorphMode)
{
	int cvMorphMode = 0;
	switch ( MorphMode ) 
	{
	case MORPH_OPEN:     cvMorphMode=CV_MOP_OPEN;	break;
	case MORPH_CLOSE:    cvMorphMode=CV_MOP_CLOSE;	break;
	case MORPH_GRADIENT: cvMorphMode=CV_MOP_GRADIENT;break;
	case MORPH_TOPHAT:   cvMorphMode=CV_MOP_TOPHAT;	break;
	case MORPH_BLACKHAT: cvMorphMode=CV_MOP_BLACKHAT;	break;
	default:             this->m_ErrorString.Format(_T("Error, Morph Mode Exception(%d)"), MorphMode);	break;
	}	
	return cvMorphMode;
}
//-------------------------------------------------------------------------------------//
int CImageAPI::GetCVMorphShapeMode(int ShapeMode)
{
	int cvShapeMode = 0;
	switch ( ShapeMode ) 
	{
	case MORPH_SHAPE_RECT:    cvShapeMode=CV_SHAPE_RECT;	break;
	case MORPH_SHAPE_CROSS:   cvShapeMode=CV_SHAPE_CROSS;	break;
	case MORPH_SHAPE_ELLIPSE: cvShapeMode=CV_SHAPE_ELLIPSE;break;		
	default:                  m_ErrorString.Format(_T("Error, Morph Shape Mode Exception(%d)"), ShapeMode);	break;
	}	
	return cvShapeMode;
}
//-------------------------------------------------------------------------------------//
int CImageAPI::GetCVBayerModeGray(BAYER_PATTERN_MODE Bayer)//Bayer to Gray
{
	int cvBayerMode=0;
	switch ( Bayer )
	{
	case BAYER_PATTERN_RGGB:	cvBayerMode=CV_BayerRG2GRAY;	break;
	case BAYER_PATTERN_GRBG:	cvBayerMode=CV_BayerGR2GRAY;	break;
	case BAYER_PATTERN_GBRG:	cvBayerMode=CV_BayerGB2GRAY;	break;
	case BAYER_PATTERN_BGGR:	cvBayerMode=CV_BayerBG2GRAY;	break;
	default:					m_ErrorString.Format(_T("Error, Bayer Pattern Mode Exception(%d)"), Bayer);	break;
	}
	return cvBayerMode;
}
//-------------------------------------------------------------------------------------//
int CImageAPI::GetCVBayerModeBGR(BAYER_PATTERN_MODE Bayer)
{
	int cvBayerMode=0;
	switch ( Bayer )
	{
	case BAYER_PATTERN_RGGB:	cvBayerMode=CV_BayerRG2BGR;	break;
	case BAYER_PATTERN_GRBG:	cvBayerMode=CV_BayerGR2BGR;	break;
	case BAYER_PATTERN_GBRG:	cvBayerMode=CV_BayerGB2BGR;	break;
	case BAYER_PATTERN_BGGR:	cvBayerMode=CV_BayerBG2BGR;	break;
	default:					m_ErrorString.Format(_T("Error, Bayer Pattern Mode Exception(%d)"), Bayer);	break;
	}
	return cvBayerMode;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DrawBoxImage_Rect(cv::Mat &Img, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect)
{
	if ( CheckMatIsValid(Img)==false ) 
	{	return false; }
	CvPoint  PA1, PA2;
	cv::Rect cvRect;
	PA1.x = MaskRect.left;
	PA1.y = MaskRect.top;
	PA2.x = MaskRect.right;
	PA2.y = MaskRect.bottom;
	CvPointToCVRect(PA1, PA2, cvRect);
	cv::rectangle(Img, cvRect, color, CV_FILLED);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DrawBoxImage_Rect(IplImage *pImg, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect)	
{
	if ( NULL == pImg ) { return false; }
	CvPoint  PA1, PA2;
	PA1.x = MaskRect.left;
	PA1.y = MaskRect.top;
	PA2.x = MaskRect.right;
	PA2.y = MaskRect.bottom;
	::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DrawBoxImage_RroundRect(cv::Mat &Img, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect, int Radius)
{
	if ( CheckMatIsValid(Img)==false ) 
	{	return false; }

	RECT     TempRect;
	CvBox2D  cvbox;
	CvPoint  PA1, PA2;
	cv::Rect cvRect;

	//Rect-Center-TB
	TempRect.left = MaskRect.left+Radius;
	TempRect.right = MaskRect.right-Radius;
	TempRect.top = MaskRect.top;
	TempRect.bottom = MaskRect.bottom;		
	PA1.x = TempRect.left;	PA1.y = TempRect.top;
	PA2.x = TempRect.right;	PA2.y = TempRect.bottom;
	CvPointToCVRect(PA1, PA2, cvRect);
	cv::rectangle(Img, cvRect, color, CV_FILLED);	

	//Rect-Left
	TempRect.left = MaskRect.left;
	TempRect.right = MaskRect.left+Radius;
	TempRect.top = MaskRect.top+Radius;
	TempRect.bottom = MaskRect.bottom-Radius;
	PA1.x = TempRect.left;	PA1.y = TempRect.top;
	PA2.x = TempRect.right;	PA2.y = TempRect.bottom;	
	CvPointToCVRect(PA1, PA2, cvRect);
	cv::rectangle(Img, cvRect, color, CV_FILLED);
	
	//Rect-Right
	TempRect.left = MaskRect.right-Radius;
	TempRect.right = MaskRect.right;
	TempRect.top = MaskRect.top+Radius;
	TempRect.bottom = MaskRect.bottom-Radius;
	PA1.x = TempRect.left;	PA1.y = TempRect.top;
	PA2.x = TempRect.right;	PA2.y = TempRect.bottom;	
	CvPointToCVRect(PA1, PA2, cvRect);
	cv::rectangle(Img, cvRect, color, CV_FILLED);	

	//Corner Circle 
	cvbox.angle = 90; 
	cvbox.size.height = (float)(Radius*2.0f);
	cvbox.size.width = (float)(Radius*2.0f);
	//Corner Circle Left-Top
	cvbox.center.x = (float)(MaskRect.left+Radius);
	cvbox.center.y = (float)(MaskRect.top+Radius);	
	cv::ellipse(Img, cvbox, color, CV_FILLED);
		
	//Corner Circle Left-Bottom
	cvbox.center.x = (float)(MaskRect.left+Radius);
	cvbox.center.y = (float)(MaskRect.bottom-Radius);
	cv::ellipse(Img, cvbox, color, CV_FILLED);

	//Corner Circle Right-Bottom
	cvbox.center.x = (float)(MaskRect.right-Radius);
	cvbox.center.y = (float)(MaskRect.bottom-Radius);
	cv::ellipse(Img, cvbox, color, CV_FILLED);
		
	//Corner Circle Right-Top
	cvbox.center.x = (float)(MaskRect.right-Radius);
	cvbox.center.y = (float)(MaskRect.top+Radius);
	cv::ellipse(Img, cvbox, color, CV_FILLED);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DrawBoxImage_RroundRect(IplImage *pImg, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect, int Radius)
{
	if ( NULL == pImg ) { return false; }

	RECT    TempRect;
	CvBox2D cvbox;
	CvPoint PA1, PA2;

	//Rect-Center-TB
	TempRect.left = MaskRect.left+Radius;
	TempRect.right = MaskRect.right-Radius;
	TempRect.top = MaskRect.top;
	TempRect.bottom = MaskRect.bottom;
	PA1.x = TempRect.left;	PA1.y = TempRect.top;
	PA2.x = TempRect.right;	PA2.y = TempRect.bottom;
	::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);

	//Rect-Left
	TempRect.left = MaskRect.left;
	TempRect.right = MaskRect.left+Radius;
	TempRect.top = MaskRect.top+Radius;
	TempRect.bottom = MaskRect.bottom-Radius;
	PA1.x = TempRect.left;	PA1.y = TempRect.top;
	PA2.x = TempRect.right;	PA2.y = TempRect.bottom;
	::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);

	//Rect-Right
	TempRect.left = MaskRect.right-Radius;
	TempRect.right = MaskRect.right;
	TempRect.top = MaskRect.top+Radius;
	TempRect.bottom = MaskRect.bottom-Radius;
	PA1.x = TempRect.left;	PA1.y = TempRect.top;
	PA2.x = TempRect.right;	PA2.y = TempRect.bottom;
	::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);

	//Corner Circle 
	cvbox.angle = 90; 
	cvbox.size.height = (float)(Radius*2.0f);
	cvbox.size.width = (float)(Radius*2.0f);
	//Corner Circle Left-Top
	cvbox.center.x = (float)(MaskRect.left+Radius);
	cvbox.center.y = (float)(MaskRect.top+Radius);
	::cvEllipseBox(pImg, cvbox, color,CV_FILLED);	
	//Corner Circle Left-Bottom
	cvbox.center.x = (float)(MaskRect.left+Radius);
	cvbox.center.y = (float)(MaskRect.bottom-Radius);
	::cvEllipseBox(pImg, cvbox, color,CV_FILLED);	
	//Corner Circle Right-Bottom
	cvbox.center.x = (float)(MaskRect.right-Radius);
	cvbox.center.y = (float)(MaskRect.bottom-Radius);
	::cvEllipseBox(pImg, cvbox, color,CV_FILLED);	
	//Corner Circle Right-Top
	cvbox.center.x = (float)(MaskRect.right-Radius);
	cvbox.center.y = (float)(MaskRect.top+Radius);
	::cvEllipseBox(pImg, cvbox, color,CV_FILLED);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DrawBoxImage_Ellipse(cv::Mat &Img, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect)
{
	if ( CheckMatIsValid(Img)==false ) 
	{	return false; }
	CvBox2D cvbox;
	cvbox.center.x = (MaskRect.left+MaskRect.right)/2.0f;
	cvbox.center.y = (MaskRect.top+MaskRect.bottom)/2.0f;
	cvbox.size.height = (float)(MaskRect.right-MaskRect.left);
	cvbox.size.width = (float)(MaskRect.bottom-MaskRect.top);								
	cvbox.angle = 90; 	
	cv::ellipse(Img, cvbox, color, CV_FILLED);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DrawBoxImage_Ellipse(IplImage *pImg, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect)
{
	if ( NULL == pImg ) { return false; }
	CvBox2D cvbox;
	cvbox.center.x = (MaskRect.left+MaskRect.right)/2.0f;
	cvbox.center.y = (MaskRect.top+MaskRect.bottom)/2.0f;
	cvbox.size.height = (float)(MaskRect.right-MaskRect.left);
	cvbox.size.width = (float)(MaskRect.bottom-MaskRect.top);								
	cvbox.angle = 90; 	
	::cvEllipseBox(pImg, cvbox, color,CV_FILLED);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DrawBoxImage_Capsule(cv::Mat &Img, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect)
{
	if ( CheckMatIsValid(Img)==false ) 
	{	return false; }

	CvBox2D cvbox;
	CvPoint PA1, PA2;
	cv::Rect cvRect;
	RECT CircleRect = MaskRect;
	int RectW = MaskRect.right-MaskRect.left;
	int RectH = MaskRect.bottom-MaskRect.top;
	
	if ( RectW < RectH )//上下鵝卵形
	{
		cvbox.size.width = (float)RectW;
		cvbox.size.height = (float)RectW;
		cvbox.angle = 0;    

		CircleRect.top = MaskRect.top;
		CircleRect.bottom = CircleRect.top+RectW;
		cvbox.center.x = (CircleRect.left+CircleRect.right)/2.0f;
		cvbox.center.y = (CircleRect.top+CircleRect.bottom)/2.0f;
		cv::ellipse(Img, cvbox, color, CV_FILLED);

		CircleRect.bottom = MaskRect.bottom;
		CircleRect.top = CircleRect.bottom-RectW;
		cvbox.center.x = (CircleRect.left+CircleRect.right)/2.0f;
		cvbox.center.y = (CircleRect.top+CircleRect.bottom)/2.0f;		
		cv::ellipse(Img, cvbox, color, CV_FILLED);

		PA1.x = MaskRect.left;
		PA1.y = MaskRect.top+(RectW/2);
		PA2.x = MaskRect.right;
		PA2.y = MaskRect.bottom-(RectW/2);
		CvPointToCVRect(PA1, PA2, cvRect);
		cv::rectangle(Img, cvRect, color, CV_FILLED);		
	}
	else//左右鵝卵形
	{
		cvbox.size.width = (float)RectH;
		cvbox.size.height = (float)RectH;
		cvbox.angle = 0;    

		CircleRect.left = MaskRect.left;
		CircleRect.right = CircleRect.left+RectH;
		cvbox.center.x = (CircleRect.left+CircleRect.right)/2.0f;
		cvbox.center.y = (CircleRect.top+CircleRect.bottom)/2.0f;
		cv::ellipse(Img, cvbox, color, CV_FILLED);

		CircleRect.right = MaskRect.right;
		CircleRect.left = CircleRect.right-RectH;
		cvbox.center.x = (CircleRect.left+CircleRect.right)/2.0f;
		cvbox.center.y = (CircleRect.top+CircleRect.bottom)/2.0f;		
		cv::ellipse(Img, cvbox, color, CV_FILLED);

		PA1.x = MaskRect.left+(RectH/2);
		PA1.y = MaskRect.top;
		PA2.x = MaskRect.right-(RectH/2);;
		PA2.y = MaskRect.bottom;
		CvPointToCVRect(PA1, PA2, cvRect);
		cv::rectangle(Img, cvRect, color, CV_FILLED);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DrawBoxImage_Capsule(IplImage *pImg, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect)
{
	if ( NULL == pImg ) { return false; }

	CvBox2D cvbox;
	CvPoint PA1, PA2;
	RECT CircleRect = MaskRect;
	int RectW = MaskRect.right-MaskRect.left;
	int RectH = MaskRect.bottom-MaskRect.top;
	
	if ( RectW < RectH )//上下鵝卵形
	{
		cvbox.size.width = (float)RectW;
		cvbox.size.height = (float)RectW;
		cvbox.angle = 0;    

		CircleRect.top = MaskRect.top;
		CircleRect.bottom = CircleRect.top+RectW;
		cvbox.center.x = (CircleRect.left+CircleRect.right)/2.0f;
		cvbox.center.y = (CircleRect.top+CircleRect.bottom)/2.0f;
		::cvEllipseBox(pImg, cvbox, color,CV_FILLED);

		CircleRect.bottom = MaskRect.bottom;
		CircleRect.top = CircleRect.bottom-RectW;
		cvbox.center.x = (CircleRect.left+CircleRect.right)/2.0f;
		cvbox.center.y = (CircleRect.top+CircleRect.bottom)/2.0f;
		::cvEllipseBox(pImg, cvbox, color,CV_FILLED);

		PA1.x = MaskRect.left;
		PA1.y = MaskRect.top+(RectW/2);
		PA2.x = MaskRect.right;
		PA2.y = MaskRect.bottom-(RectW/2);
		::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);
	}
	else//左右鵝卵形
	{
		cvbox.size.width = (float)RectH;
		cvbox.size.height = (float)RectH;
		cvbox.angle = 0;    

		CircleRect.left = MaskRect.left;
		CircleRect.right = CircleRect.left+RectH;
		cvbox.center.x = (CircleRect.left+CircleRect.right)/2.0f;
		cvbox.center.y = (CircleRect.top+CircleRect.bottom)/2.0f;
		::cvEllipseBox(pImg, cvbox, color,CV_FILLED);

		CircleRect.right = MaskRect.right;
		CircleRect.left = CircleRect.right-RectH;
		cvbox.center.x = (CircleRect.left+CircleRect.right)/2.0f;
		cvbox.center.y = (CircleRect.top+CircleRect.bottom)/2.0f;
		::cvEllipseBox(pImg, cvbox, color,CV_FILLED);

		PA1.x = MaskRect.left+(RectH/2);
		PA1.y = MaskRect.top;
		PA2.x = MaskRect.right-(RectH/2);;
		PA2.y = MaskRect.bottom;		
		::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DrawBoxImage_Bullet(cv::Mat &Img, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect)
{
	if ( CheckMatIsValid(Img)==false ) 
	{	return false; }

	CvBox2D cvbox;
	CvPoint PA1, PA2;
	cv::Rect cvRect;
	RECT CircleRect = MaskRect;
	int RectW = MaskRect.right-MaskRect.left;
	int RectH = MaskRect.bottom-MaskRect.top;

	switch ( BoxToward )
	{
	case BOX_TOWARD_UP:
		cvbox.size.width = (float)RectW;
		cvbox.size.height = (float)RectW;
		cvbox.angle = 0;

		CircleRect.top = MaskRect.top;
		CircleRect.bottom = CircleRect.top+RectW;
		cvbox.center.x = (CircleRect.left+CircleRect.right)/2.0f;
		cvbox.center.y = (CircleRect.top+CircleRect.bottom)/2.0f;
		cv::ellipse(Img, cvbox, color, CV_FILLED);

		PA1.x = MaskRect.left;
		PA1.y = MaskRect.top+(RectW/2);
		PA2.x = MaskRect.right;
		PA2.y = MaskRect.bottom;		
		
		CvPointToCVRect(PA1, PA2, cvRect);
		cv::rectangle(Img, cvRect, color, CV_FILLED);		

		if ( CircleRect.bottom  > MaskRect.bottom )
		{
			PA1.x = MaskRect.left-1;
			PA1.y = MaskRect.bottom;
			PA2.x = MaskRect.right+1;
			PA2.y = CircleRect.bottom+1;
			CvPointToCVRect(PA1, PA2, cvRect);
			cv::rectangle(Img, cvRect, bkcolor, CV_FILLED);			
		}
		break;
	case BOX_TOWARD_LEFT:
		cvbox.size.width = (float)RectH;
		cvbox.size.height = (float)RectH;
		cvbox.angle = 0;    

		CircleRect.left = MaskRect.left;
		CircleRect.right = CircleRect.left+RectH;
		cvbox.center.x = (CircleRect.left+CircleRect.right)/2.0f;
		cvbox.center.y = (CircleRect.top+CircleRect.bottom)/2.0f;
		cv::ellipse(Img, cvbox, color, CV_FILLED);

		PA1.x = MaskRect.left+(RectH/2);
		PA1.y = MaskRect.top;
		PA2.x = MaskRect.right;
		PA2.y = MaskRect.bottom;
		CvPointToCVRect(PA1, PA2, cvRect);
		cv::rectangle(Img, cvRect, color, CV_FILLED);		
				
		if ( CircleRect.right  > MaskRect.right )
		{
			PA1.x = MaskRect.right;
			PA1.y = MaskRect.top-1;
			PA2.x = CircleRect.right+1;
			PA2.y = MaskRect.bottom+1;			
			CvPointToCVRect(PA1, PA2, cvRect);
			cv::rectangle(Img, cvRect, bkcolor, CV_FILLED);			
		}
		break;
	case BOX_TOWARD_DOWN:
		cvbox.size.width = (float)RectW;
		cvbox.size.height = (float)RectW;
		cvbox.angle = 0;
					
		CircleRect.bottom = MaskRect.bottom;
		CircleRect.top = CircleRect.bottom-RectW;
		cvbox.center.x = (CircleRect.left+CircleRect.right)/2.0f;
		cvbox.center.y = (CircleRect.top+CircleRect.bottom)/2.0f;
		cv::ellipse(Img, cvbox, color, CV_FILLED);

		PA1.x = MaskRect.left;
		PA1.y = MaskRect.top;
		PA2.x = MaskRect.right;
		PA2.y = MaskRect.bottom-(RectW/2);
		CvPointToCVRect(PA1, PA2, cvRect);
		cv::rectangle(Img, cvRect, color, CV_FILLED);		

		if ( CircleRect.top  < MaskRect.top )
		{
			PA1.x = MaskRect.left-1;
			PA1.y = MaskRect.top;
			PA2.x = MaskRect.right+1;
			PA2.y = CircleRect.top-1;
			CvPointToCVRect(PA1, PA2, cvRect);
			cv::rectangle(Img, cvRect, bkcolor, CV_FILLED);			
		}
		break;
	case BOX_TOWARD_RIGHT:
		cvbox.size.width = (float)RectH;
		cvbox.size.height = (float)RectH;
		cvbox.angle = 0;

		CircleRect.right = MaskRect.right;
		CircleRect.left = CircleRect.right-RectH;
		cvbox.center.x = (CircleRect.left+CircleRect.right)/2.0f;
		cvbox.center.y = (CircleRect.top+CircleRect.bottom)/2.0f;
		cv::ellipse(Img, cvbox, color, CV_FILLED);

		PA1.x = MaskRect.left;
		PA1.y = MaskRect.top;
		PA2.x = MaskRect.right-(RectH/2);;
		PA2.y = MaskRect.bottom;
		CvPointToCVRect(PA1, PA2, cvRect);
		cv::rectangle(Img, cvRect, color, CV_FILLED);		

		if ( CircleRect.left  < MaskRect.left )
		{
			PA1.x = MaskRect.left;
			PA1.y = MaskRect.top-1;
			PA2.x = CircleRect.left-1;
			PA2.y = MaskRect.bottom+1;
			CvPointToCVRect(PA1, PA2, cvRect);
			cv::rectangle(Img, cvRect, bkcolor, CV_FILLED);			
		}
		break;
	}
	//cvFillPoly(CvArr* array,CvPoint** pts,int* npts,int contours,CvScalar color,int line_type=8);//多邊形
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DrawBoxImage_Bullet(IplImage *pImg, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect)			
{
	if ( NULL == pImg ) { return false; }

	CvBox2D cvbox;
	CvPoint PA1, PA2;
	RECT CircleRect = MaskRect;
	int RectW = MaskRect.right-MaskRect.left;
	int RectH = MaskRect.bottom-MaskRect.top;

	switch ( BoxToward )
	{
	case BOX_TOWARD_UP:
		cvbox.size.width = (float)RectW;
		cvbox.size.height = (float)RectW;
		cvbox.angle = 0;

		CircleRect.top = MaskRect.top;
		CircleRect.bottom = CircleRect.top+RectW;
		cvbox.center.x = (CircleRect.left+CircleRect.right)/2.0f;
		cvbox.center.y = (CircleRect.top+CircleRect.bottom)/2.0f;
		::cvEllipseBox(pImg, cvbox, color,CV_FILLED);

		PA1.x = MaskRect.left;
		PA1.y = MaskRect.top+(RectW/2);
		PA2.x = MaskRect.right;
		PA2.y = MaskRect.bottom;
		::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);

		if ( CircleRect.bottom  > MaskRect.bottom )
		{
			PA1.x = MaskRect.left-1;
			PA1.y = MaskRect.bottom;
			PA2.x = MaskRect.right+1;
			PA2.y = CircleRect.bottom+1;
			::cvRectangle(pImg, PA1, PA2, bkcolor, CV_FILLED);
		}
		break;
	case BOX_TOWARD_LEFT:
		cvbox.size.width = (float)RectH;
		cvbox.size.height = (float)RectH;
		cvbox.angle = 0;    

		CircleRect.left = MaskRect.left;
		CircleRect.right = CircleRect.left+RectH;
		cvbox.center.x = (CircleRect.left+CircleRect.right)/2.0f;
		cvbox.center.y = (CircleRect.top+CircleRect.bottom)/2.0f;
		::cvEllipseBox(pImg, cvbox, color,CV_FILLED);

		PA1.x = MaskRect.left+(RectH/2);
		PA1.y = MaskRect.top;
		PA2.x = MaskRect.right;
		PA2.y = MaskRect.bottom;
		::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);
				
		if ( CircleRect.right  > MaskRect.right )
		{
			PA1.x = MaskRect.right;
			PA1.y = MaskRect.top-1;
			PA2.x = CircleRect.right+1;
			PA2.y = MaskRect.bottom+1;
			::cvRectangle(pImg, PA1, PA2, bkcolor, CV_FILLED);
		}
		break;
	case BOX_TOWARD_DOWN:
		cvbox.size.width = (float)RectW;
		cvbox.size.height = (float)RectW;
		cvbox.angle = 0;
					
		CircleRect.bottom = MaskRect.bottom;
		CircleRect.top = CircleRect.bottom-RectW;
		cvbox.center.x = (CircleRect.left+CircleRect.right)/2.0f;
		cvbox.center.y = (CircleRect.top+CircleRect.bottom)/2.0f;
		::cvEllipseBox(pImg, cvbox, color,CV_FILLED);

		PA1.x = MaskRect.left;
		PA1.y = MaskRect.top;
		PA2.x = MaskRect.right;
		PA2.y = MaskRect.bottom-(RectW/2);
		::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);

		if ( CircleRect.top  < MaskRect.top )
		{
			PA1.x = MaskRect.left-1;
			PA1.y = MaskRect.top;
			PA2.x = MaskRect.right+1;
			PA2.y = CircleRect.top-1;
			::cvRectangle(pImg, PA1, PA2, bkcolor, CV_FILLED);
		}
		break;
	case BOX_TOWARD_RIGHT:
		cvbox.size.width = (float)RectH;
		cvbox.size.height = (float)RectH;
		cvbox.angle = 0;

		CircleRect.right = MaskRect.right;
		CircleRect.left = CircleRect.right-RectH;
		cvbox.center.x = (CircleRect.left+CircleRect.right)/2.0f;
		cvbox.center.y = (CircleRect.top+CircleRect.bottom)/2.0f;
		::cvEllipseBox(pImg, cvbox, color,CV_FILLED);

		PA1.x = MaskRect.left;
		PA1.y = MaskRect.top;
		PA2.x = MaskRect.right-(RectH/2);;
		PA2.y = MaskRect.bottom;
		::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);

		if ( CircleRect.left  < MaskRect.left )
		{
			PA1.x = MaskRect.left;
			PA1.y = MaskRect.top-1;
			PA2.x = CircleRect.left-1;
			PA2.y = MaskRect.bottom+1;			
			::cvRectangle(pImg, PA1, PA2, bkcolor, CV_FILLED);
		}
		break;
	}
	//cvFillPoly(CvArr* array,CvPoint** pts,int* npts,int contours,CvScalar color,int line_type=8);//多邊形
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DrawBoxImage_HalfRroundRect(cv::Mat &Img, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect, int Radius)
{
	if ( CheckMatIsValid(Img)==false ) 
	{	return false; }

	CvBox2D  cvbox;
	CvSize  size;
	CvPoint center;
	CvPoint  PA1, PA2;
	cv::Rect cvRect;
	RECT     TempRect;
	RECT     CircleRect = MaskRect;
	int RectW = MaskRect.right-MaskRect.left;
	int RectH = MaskRect.bottom-MaskRect.top;
	switch ( BoxToward )
	{
	case BOX_TOWARD_UP:
		//Corner Circle 
		size.width = Radius;
		size.height = Radius;
				
		//Corner Circle Left-Top				
		center.x = (MaskRect.left+Radius);
		center.y = (MaskRect.top+Radius);
		cv::ellipse(Img, center, size, 0, 180, 360, color, CV_FILLED);
		if ( Radius < (RectW/2) )
		{
			//Corner Circle Right-Top
			center.x = (MaskRect.right-Radius);
			center.y = (MaskRect.top+Radius);
			cv::ellipse(Img, center, size, 0, 180, 360, color, CV_FILLED);

			PA1.x = MaskRect.left+Radius;	PA1.y = MaskRect.top;
			PA2.x = MaskRect.right-Radius;	PA2.y = MaskRect.bottom;
			CvPointToCVRect(PA1, PA2, cvRect);
			cv::rectangle(Img, cvRect, color, CV_FILLED);			
		}
		if ( Radius < RectH )
		{
			PA1.x = MaskRect.left-1;	PA1.y = MaskRect.top+Radius-1;
			PA2.x = MaskRect.right+1;	PA2.y = MaskRect.bottom;
			CvPointToCVRect(PA1, PA2, cvRect);
			cv::rectangle(Img, cvRect, color, CV_FILLED);			
		}
		break;
	case BOX_TOWARD_LEFT:
		//Corner Circle 
		size.width = Radius;
		size.height = Radius;
		//Corner Circle Left-Top				
		center.x = (MaskRect.left+Radius);
		center.y = (MaskRect.top+Radius);
		cv::ellipse(Img, center, size, 0, 90, 270, color, CV_FILLED);
		if ( Radius < (RectH/2) )
		{
			//Corner Circle Left-Bottom					
			center.x = (MaskRect.left+Radius);
			center.y = (MaskRect.bottom-Radius);
			cv::ellipse(Img, center, size, 0, 90, 270, color, CV_FILLED);

			PA1.x = MaskRect.left;	PA1.y = MaskRect.top+Radius;
			PA2.x = MaskRect.right;	PA2.y = MaskRect.bottom-Radius;
			CvPointToCVRect(PA1, PA2, cvRect);
			cv::rectangle(Img, cvRect, color, CV_FILLED);			
		}
		if ( Radius < RectW )
		{
			PA1.x = MaskRect.left+Radius-1;	PA1.y = MaskRect.top;
			PA2.x = MaskRect.right;	PA2.y = MaskRect.bottom;
			CvPointToCVRect(PA1, PA2, cvRect);
			cv::rectangle(Img, cvRect, color, CV_FILLED);			
		}
		break;
	case BOX_TOWARD_DOWN:
		//Corner Circle 
		size.width = Radius;
		size.height = Radius;
		//Corner Circle Left-Bottom				
		center.x = (MaskRect.left+Radius);
		center.y = (MaskRect.bottom-Radius);
		cv::ellipse(Img, center, size, 0, 0, 180, color, CV_FILLED);
		if ( Radius < (RectW/2) )
		{
			//Corner Circle Right-Bottom					
			center.x = (MaskRect.right-Radius);
			center.y = (MaskRect.bottom-Radius);
			cv::ellipse(Img, center, size, 0, 0, 180, color, CV_FILLED);

			PA1.x = MaskRect.left+Radius;	PA1.y = MaskRect.top;
			PA2.x = MaskRect.right-Radius;	PA2.y = MaskRect.bottom;
			CvPointToCVRect(PA1, PA2, cvRect);
			cv::rectangle(Img, cvRect, color, CV_FILLED);			
		}
		if ( Radius < RectH )
		{
			PA1.x = MaskRect.left;	PA1.y = MaskRect.top;
			PA2.x = MaskRect.right;	PA2.y = MaskRect.bottom-Radius+1;
			CvPointToCVRect(PA1, PA2, cvRect);
			cv::rectangle(Img, cvRect, color, CV_FILLED);			
		}
		break;
	case BOX_TOWARD_RIGHT:
		//Corner Circle 
		size.width = Radius;
		size.height = Radius;
		//Corner Circle Right-Bottom				
		center.x = (MaskRect.right-Radius);
		center.y = (MaskRect.bottom-Radius);
		cv::ellipse(Img, center, size, 0, 270, 450, color, CV_FILLED);
		if ( Radius < (RectH/2) )
		{
			//Corner Circle Right-Top					
			center.x = (MaskRect.right-Radius);
			center.y = (MaskRect.top+Radius);
			cv::ellipse(Img, center, size, 0, 270, 450, color, CV_FILLED);

			PA1.x = MaskRect.left;	PA1.y = MaskRect.top+Radius;
			PA2.x = MaskRect.right;	PA2.y = MaskRect.bottom-Radius;
			CvPointToCVRect(PA1, PA2, cvRect);
			cv::rectangle(Img, cvRect, color, CV_FILLED);			
		}
		if ( Radius < RectW )
		{
			PA1.x = MaskRect.left;	PA1.y = MaskRect.top;
			PA2.x = MaskRect.right-Radius+1;	PA2.y = MaskRect.bottom;
			CvPointToCVRect(PA1, PA2, cvRect);
			cv::rectangle(Img, cvRect, color, CV_FILLED);			
		}
		break;
	default:
		//Rect-Center-TB
		TempRect.left = MaskRect.left+Radius;
		TempRect.right = MaskRect.right-Radius;
		TempRect.top = MaskRect.top;
		TempRect.bottom = MaskRect.bottom;
		PA1.x = TempRect.left;	PA1.y = TempRect.top;
		PA2.x = TempRect.right;	PA2.y = TempRect.bottom;
		CvPointToCVRect(PA1, PA2, cvRect);
		cv::rectangle(Img, cvRect, color, CV_FILLED);		

		//Rect-Left
		TempRect.left = MaskRect.left;
		TempRect.right = MaskRect.left+Radius;
		TempRect.top = MaskRect.top+Radius;
		TempRect.bottom = MaskRect.bottom-Radius;
		PA1.x = TempRect.left;	PA1.y = TempRect.top;
		PA2.x = TempRect.right;	PA2.y = TempRect.bottom;
		CvPointToCVRect(PA1, PA2, cvRect);
		cv::rectangle(Img, cvRect, color, CV_FILLED);		

		//Rect-Right
		TempRect.left = MaskRect.right-Radius;
		TempRect.right = MaskRect.right;
		TempRect.top = MaskRect.top+Radius;
		TempRect.bottom = MaskRect.bottom-Radius;
		PA1.x = TempRect.left;	PA1.y = TempRect.top;
		PA2.x = TempRect.right;	PA2.y = TempRect.bottom;
		CvPointToCVRect(PA1, PA2, cvRect);
		cv::rectangle(Img, cvRect, color, CV_FILLED);		

		//Corner Circle 
		cvbox.angle = 90; 
		cvbox.size.height = (float)(Radius*2.0f);
		cvbox.size.width = (float)(Radius*2.0f);
		//Corner Circle Left-Top
		cvbox.center.x = (float)(MaskRect.left+Radius);
		cvbox.center.y = (float)(MaskRect.top+Radius);
		cv::ellipse(Img, cvbox, color, CV_FILLED);
		//Corner Circle Left-Bottom
		cvbox.center.x = (float)(MaskRect.left+Radius);
		cvbox.center.y = (float)(MaskRect.bottom-Radius);
		cv::ellipse(Img, cvbox, color, CV_FILLED);
		//Corner Circle Right-Bottom
		cvbox.center.x = (float)(MaskRect.right-Radius);
		cvbox.center.y = (float)(MaskRect.bottom-Radius);
		cv::ellipse(Img, cvbox, color, CV_FILLED);
		//Corner Circle Right-Top
		cvbox.center.x = (float)(MaskRect.right-Radius);
		cvbox.center.y = (float)(MaskRect.top+Radius);
		cv::ellipse(Img, cvbox, color, CV_FILLED);
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DrawBoxImage_HalfRroundRect(IplImage *pImg, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect, int Radius)
{
	if ( NULL == pImg ) { return false; }

	CvBox2D cvbox;
	CvSize  size;
	CvPoint center;
	CvPoint PA1, PA2;
	RECT    TempRect;
	RECT    CircleRect = MaskRect;
	int RectW = MaskRect.right-MaskRect.left;
	int RectH = MaskRect.bottom-MaskRect.top;

	switch ( BoxToward )
	{
	case BOX_TOWARD_UP:
		//Corner Circle 
		size.width = Radius;
		size.height = Radius;
				
		//Corner Circle Left-Top				
		center.x = (MaskRect.left+Radius);
		center.y = (MaskRect.top+Radius);
		::cvEllipse(pImg, center, size, 0, 180, 360, color, CV_FILLED);
		if ( Radius < (RectW/2) )
		{
			//Corner Circle Right-Top
			center.x = (MaskRect.right-Radius);
			center.y = (MaskRect.top+Radius);
			::cvEllipse(pImg, center, size, 0, 180, 360, color, CV_FILLED);

			PA1.x = MaskRect.left+Radius;	PA1.y = MaskRect.top;
			PA2.x = MaskRect.right-Radius;	PA2.y = MaskRect.bottom;
			::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);
		}
		if ( Radius < RectH )
		{
			PA1.x = MaskRect.left-1;	PA1.y = MaskRect.top+Radius-1;
			PA2.x = MaskRect.right+1;	PA2.y = MaskRect.bottom;
			::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);
		}
		break;
	case BOX_TOWARD_LEFT:
		//Corner Circle 
		size.width = Radius;
		size.height = Radius;
		//Corner Circle Left-Top				
		center.x = (MaskRect.left+Radius);
		center.y = (MaskRect.top+Radius);
		::cvEllipse(pImg, center, size, 0, 90, 270, color, CV_FILLED);
		if ( Radius < (RectH/2) )
		{
			//Corner Circle Left-Bottom					
			center.x = (MaskRect.left+Radius);
			center.y = (MaskRect.bottom-Radius);
			::cvEllipse(pImg, center, size, 0, 90, 270, color, CV_FILLED);

			PA1.x = MaskRect.left;	PA1.y = MaskRect.top+Radius;
			PA2.x = MaskRect.right;	PA2.y = MaskRect.bottom-Radius;
			::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);
		}
		if ( Radius < RectW )
		{
			PA1.x = MaskRect.left+Radius-1;	PA1.y = MaskRect.top;
			PA2.x = MaskRect.right;	PA2.y = MaskRect.bottom;
			::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);
		}
		break;
	case BOX_TOWARD_DOWN:
		//Corner Circle 
		size.width = Radius;
		size.height = Radius;
		//Corner Circle Left-Bottom				
		center.x = (MaskRect.left+Radius);
		center.y = (MaskRect.bottom-Radius);
		::cvEllipse(pImg, center, size, 0, 0, 180, color, CV_FILLED);
		if ( Radius < (RectW/2) )
		{
			//Corner Circle Right-Bottom					
			center.x = (MaskRect.right-Radius);
			center.y = (MaskRect.bottom-Radius);
			::cvEllipse(pImg, center, size, 0, 0, 180, color, CV_FILLED);

			PA1.x = MaskRect.left+Radius;	PA1.y = MaskRect.top;
			PA2.x = MaskRect.right-Radius;	PA2.y = MaskRect.bottom;
			::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);
		}
		if ( Radius < RectH )
		{
			PA1.x = MaskRect.left;	PA1.y = MaskRect.top;
			PA2.x = MaskRect.right;	PA2.y = MaskRect.bottom-Radius+1;
			::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);
		}
		break;
	case BOX_TOWARD_RIGHT:
		//Corner Circle 
		size.width = Radius;
		size.height = Radius;
		//Corner Circle Right-Bottom				
		center.x = (MaskRect.right-Radius);
		center.y = (MaskRect.bottom-Radius);
		::cvEllipse(pImg, center, size, 0, 270, 450, color, CV_FILLED);
		if ( Radius < (RectH/2) )
		{
			//Corner Circle Right-Top					
			center.x = (MaskRect.right-Radius);
			center.y = (MaskRect.top+Radius);
			::cvEllipse(pImg, center, size, 0, 270, 450, color, CV_FILLED);

			PA1.x = MaskRect.left;	PA1.y = MaskRect.top+Radius;
			PA2.x = MaskRect.right;	PA2.y = MaskRect.bottom-Radius;
			::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);
		}
		if ( Radius < RectW )
		{
			PA1.x = MaskRect.left;	PA1.y = MaskRect.top;
			PA2.x = MaskRect.right-Radius+1;	PA2.y = MaskRect.bottom;
			::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);
		}
		break;
	default:
		//Rect-Center-TB
		TempRect.left = MaskRect.left+Radius;
		TempRect.right = MaskRect.right-Radius;
		TempRect.top = MaskRect.top;
		TempRect.bottom = MaskRect.bottom;
		PA1.x = TempRect.left;	PA1.y = TempRect.top;
		PA2.x = TempRect.right;	PA2.y = TempRect.bottom;
		::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);

		//Rect-Left
		TempRect.left = MaskRect.left;
		TempRect.right = MaskRect.left+Radius;
		TempRect.top = MaskRect.top+Radius;
		TempRect.bottom = MaskRect.bottom-Radius;
		PA1.x = TempRect.left;	PA1.y = TempRect.top;
		PA2.x = TempRect.right;	PA2.y = TempRect.bottom;
		::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);

		//Rect-Right
		TempRect.left = MaskRect.right-Radius;
		TempRect.right = MaskRect.right;
		TempRect.top = MaskRect.top+Radius;
		TempRect.bottom = MaskRect.bottom-Radius;
		PA1.x = TempRect.left;	PA1.y = TempRect.top;
		PA2.x = TempRect.right;	PA2.y = TempRect.bottom;
		::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);

		//Corner Circle 
		cvbox.angle = 90; 
		cvbox.size.height = (float)(Radius*2.0f);
		cvbox.size.width = (float)(Radius*2.0f);
		//Corner Circle Left-Top
		cvbox.center.x = (float)(MaskRect.left+Radius);
		cvbox.center.y = (float)(MaskRect.top+Radius);
		::cvEllipseBox(pImg, cvbox, color,CV_FILLED);
		//Corner Circle Left-Bottom
		cvbox.center.x = (float)(MaskRect.left+Radius);
		cvbox.center.y = (float)(MaskRect.bottom-Radius);
		::cvEllipseBox(pImg, cvbox, color,CV_FILLED);
		//Corner Circle Right-Bottom
		cvbox.center.x = (float)(MaskRect.right-Radius);
		cvbox.center.y = (float)(MaskRect.bottom-Radius);
		::cvEllipseBox(pImg, cvbox, color,CV_FILLED);
		//Corner Circle Right-Top
		cvbox.center.x = (float)(MaskRect.right-Radius);
		cvbox.center.y = (float)(MaskRect.top+Radius);
		::cvEllipseBox(pImg, cvbox, color,CV_FILLED);
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DrawBoxImage_TShapeRect(cv::Mat &Img, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect, int wRatio, int hRatio)
{
	if ( CheckMatIsValid(Img)==false ) 
	{	return false; }

	int RectW2_S=0;
	int RectH2_S=0;	
	int Param=wRatio;
	int Param2=hRatio;
	RECT RectB=MaskRect;//Bigger
	RECT RectS=MaskRect;//Smaller
	const int RectW_B=RectB.right-RectB.left;
	const int RectH_B=RectB.bottom-RectB.top;
	const int RectW2_B=RectW_B/2;
	const int RectH2_B=RectH_B/2;
	Param = 2*Param;
	if ( Param < 0 ) { Param = 0; }
	else if ( Param > 100 ) { Param = 100; }
	if ( Param2 < 0 ) { Param2 = 0; }
	else if ( Param2 > 100 ) { Param2 = 100; }
	switch ( BoxToward )
	{
	case BOX_TOWARD_UP:
	case BOX_TOWARD_DOWN:
		RectW2_S=(int)(RectW2_B*Param/100.0);
		RectH2_S=(int)(RectH2_B*Param2/100.0);	
		break;
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		RectW2_S=(int)(RectW2_B*Param2/100.0);
		RectH2_S=(int)(RectH2_B*Param/100.0);	
		break;
	default:
		break;
	}
	const int RectW_S=RectW2_S*2;
	const int RectH_S=RectH2_S*2;
	
	switch ( BoxToward )
	{
	case BOX_TOWARD_UP:
		RectS.left  += RectW2_S;
		RectS.right -= RectW2_S;
		RectS.top    = RectB.bottom-RectH_S;
		RectB.bottom = RectS.top;
		break;
	case BOX_TOWARD_LEFT:
		RectS.top    += RectH2_S;
		RectS.bottom -= RectH2_S;
		RectS.left   = RectB.right-RectW_S;
		RectB.right  = RectS.left;
		break;
	case BOX_TOWARD_DOWN:	
		RectS.left  += RectW2_S;
		RectS.right -= RectW2_S;
		RectS.bottom = RectB.top+RectH_S;
		RectB.top    = RectS.bottom;
		break;
	case BOX_TOWARD_RIGHT:
		RectS.top    += RectH2_S;
		RectS.bottom -= RectH2_S;
		RectS.right   = RectB.left+RectW_S;
		RectB.left    = RectS.right;
		break;
	default:
		break;
	}	
	
	cv::Rect cvRect;
	CvPoint  PA1, PA2;	
	
	PA1.x = RectS.left;		PA1.y = RectS.top;
	PA2.x = RectS.right;	PA2.y = RectS.bottom;
	CvPointToCVRect(PA1, PA2, cvRect);
	cv::rectangle(Img, cvRect, color, CV_FILLED);	

	PA1.x = RectB.left;		PA1.y = RectB.top;
	PA2.x = RectB.right;	PA2.y = RectB.bottom;
	CvPointToCVRect(PA1, PA2, cvRect);
	cv::rectangle(Img, cvRect, color, CV_FILLED);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DrawBoxImage_TShapeRect(IplImage *pImg, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, const RECT &MaskRect, int wRatio, int hRatio)
{
	if ( NULL == pImg ) { return false; }

	int RectW2_S=0;
	int RectH2_S=0;	
	int Param=wRatio;
	int Param2=hRatio;
	RECT RectB=MaskRect;//Bigger
	RECT RectS=MaskRect;//Smaller
	const int RectW_B=RectB.right-RectB.left;
	const int RectH_B=RectB.bottom-RectB.top;
	const int RectW2_B=RectW_B/2;
	const int RectH2_B=RectH_B/2;
	Param = 2*Param;
	if ( Param < 0 ) { Param = 0; }
	else if ( Param > 100 ) { Param = 100; }
	if ( Param2 < 0 ) { Param2 = 0; }
	else if ( Param2 > 100 ) { Param2 = 100; }
	switch ( BoxToward )
	{
	case BOX_TOWARD_UP:
	case BOX_TOWARD_DOWN:
		RectW2_S=(int)(RectW2_B*Param/100.0);
		RectH2_S=(int)(RectH2_B*Param2/100.0);	
		break;
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		RectW2_S=(int)(RectW2_B*Param2/100.0);
		RectH2_S=(int)(RectH2_B*Param/100.0);	
		break;
	default:
		break;
	}
	const int RectW_S=RectW2_S*2;
	const int RectH_S=RectH2_S*2;
	
	switch ( BoxToward )
	{
	case BOX_TOWARD_UP:
		RectS.left  += RectW2_S;
		RectS.right -= RectW2_S;
		RectS.top    = RectB.bottom-RectH_S;
		RectB.bottom = RectS.top;
		break;
	case BOX_TOWARD_LEFT:
		RectS.top    += RectH2_S;
		RectS.bottom -= RectH2_S;
		RectS.left   = RectB.right-RectW_S;
		RectB.right  = RectS.left;
		break;
	case BOX_TOWARD_DOWN:	
		RectS.left  += RectW2_S;
		RectS.right -= RectW2_S;
		RectS.bottom = RectB.top+RectH_S;
		RectB.top    = RectS.bottom;
		break;
	case BOX_TOWARD_RIGHT:
		RectS.top    += RectH2_S;
		RectS.bottom -= RectH2_S;
		RectS.right   = RectB.left+RectW_S;
		RectB.left    = RectS.right;
		break;
	default:
		break;
	}	

	CvPoint PA1, PA2;
	PA1.x = RectS.left;		PA1.y = RectS.top;
	PA2.x = RectS.right;	PA2.y = RectS.bottom;
	::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);

	PA1.x = RectB.left;		PA1.y = RectB.top;
	PA2.x = RectB.right;	PA2.y = RectB.bottom;
	::cvRectangle(pImg, PA1, PA2, color, CV_FILLED);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DrawBoxImage_RotatedRect(cv::Mat &Img, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, double Angle, const RECT &MaskRect)
{
	cv::Point vertices[4];    
	cv::Point2f vertices2f[4];
	cv::Point centerPoint(0, 0);				
	cv::Size  rectangleSize(0, 0);	
	centerPoint.x = (MaskRect.left+MaskRect.right)/2;
	centerPoint.y = (MaskRect.top+MaskRect.bottom)/2;
	rectangleSize.width = MaskRect.right-MaskRect.left;
	rectangleSize.height = MaskRect.bottom-MaskRect.top;
	// Create the rotated rectangle
	cv::RotatedRect rotatedRectangle(centerPoint, rectangleSize, Angle);
	// We take the edges that OpenCV calculated for us	
	rotatedRectangle.points(vertices2f);
	// Convert them so we can use them in a fillConvexPoly	
	for(int i = 0; i < 4; ++i)
	{	vertices[i] = vertices2f[i];	}
	// Now we can fill the rotated rectangle with our specified color	
	cv::fillConvexPoly(Img, vertices, 4, color);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::DrawBoxImage_RotatedRect(IplImage *pImg, const CvScalar &color, const CvScalar &bkcolor, BOX_TOWARD BoxToward, double Angle, const RECT &MaskRect)	
{
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::ExecFindAbnormal(IMAGE_SIZE SrcImageW, IMAGE_SIZE SrcImageH, IMAGE_SIZE SrcImageStep, IMAGE_SIZE SrcBitCount, const unsigned char * SrcPtr, IMAGE_SIZE RefImageW, IMAGE_SIZE RefImageH, IMAGE_SIZE RefImageStep, IMAGE_SIZE RefBitCount, const unsigned char * RefPtr, POINT ResStart, const int ColorExpand, size_t WndSizeW, size_t WndSizeH, const int MinEdge, unsigned char * DstPtr)
{
	//   SrcImageW      - Width of the source image in pixels
	//   SrcImageH      - Height of the source image in pixels
	//   SrcImageStep   - Number of bytes per row in the source image (stride)
	//   SrcBitCount    - Bit depth of the source image (e.g., 8, 24, 32)
	//   SrcPtr         - Pointer to the raw data of the source image
	//
	//   RefImageW      - Width of the reference image in pixels
	//   RefImageH      - Height of the reference image in pixels
	//   RefImageStep   - Number of bytes per row in the reference image (stride)
	//   RefBitCount    - Bit depth of the reference image
	//   RefPtr         - Pointer to the raw data of the reference image
	//
	//   ResStart       - Starting point (x, y) for the result region
	//   ColorExpand    - Color expansion factor or tolerance for comparison
	//   WndSizeW       - Width of the detection window
	//   WndSizeH       - Height of the detection window
	cv::Mat Src = CreateMat(SrcImageW, SrcImageH, SrcImageStep, SrcBitCount, SrcPtr);
	cv::Mat Ref = CreateMat(RefImageW, RefImageH, RefImageStep, RefBitCount, RefPtr);

	// 區域設定
	cv::Rect RefRoiRect(ResStart.x, ResStart.y, Src.cols, Src.rows);
	cv::Mat RefRoi = Ref(RefRoiRect);
	cv::Point AnchorPoint = { ResStart.x, ResStart.y };

	const int sampleStep = 5;

	//#define _CV_DEBUG
	//#define TIME_DEBUG

#ifdef TIME_DEBUG
	std::vector<double> Time;
	LARGE_INTEGER nStartTime, nEndTime;
	LONGLONG QuadPart = AOIDataCollect.m_SystemFreq.QuadPart;
	QueryPerformanceCounter(&nStartTime);
#endif

	// ----------- 顏色校正 -----------
	cv::Mat Coeff, SrcCalibration;
	if (false == TrainColorCalibrationCoeff(Src, RefRoi, sampleStep, Coeff)) return false;
	if (false == ApplyColorCalibrationCoeff(Src, Coeff, SrcCalibration)) return false;

#ifdef TIME_DEBUG
	QueryPerformanceCounter(&nEndTime);
	Time.push_back((nEndTime.QuadPart - nStartTime.QuadPart) * 1000 / QuadPart);
	QueryPerformanceCounter(&nStartTime);
#endif

#ifdef _CV_DEBUG
	cv::imshow("Src", Src);
	cv::imshow("SrcCalibration", SrcCalibration);
	cv::waitKey(0);
#endif

	// ----------- 差異計算（正負分離） -----------
	cv::Mat temp1, temp2;
	SrcCalibration.convertTo(temp1, CV_32SC3);
	RefRoi.convertTo(temp2, CV_32SC3);

	cv::Mat diff = temp1 - temp2;
	cv::Mat zeroMat = cv::Mat::zeros(diff.size(), diff.type());
	cv::Mat positiveDiff, negativeDiff;

	cv::max(diff, zeroMat, positiveDiff);   // 正差異
	cv::max(-diff, zeroMat, negativeDiff);  // 負差異

	positiveDiff.convertTo(positiveDiff, CV_8UC3);
	negativeDiff.convertTo(negativeDiff, CV_8UC3);

	// 取 RGB 三通道最大值，轉為灰階
	std::vector<cv::Mat> posSplit, negSplit;
	cv::split(positiveDiff, posSplit);
	cv::split(negativeDiff, negSplit);

	cv::Mat tmpMax, posGray, negGray;
	cv::max(posSplit[0], posSplit[1], tmpMax);
	cv::max(tmpMax, posSplit[2], posGray);

	cv::max(negSplit[0], negSplit[1], tmpMax);
	cv::max(tmpMax, negSplit[2], negGray);

	// 二值化
	cv::Mat posBinary, negBinary;
	cv::threshold(posGray, posBinary, ColorExpand, 255, cv::THRESH_BINARY);
	cv::threshold(negGray, negBinary, ColorExpand, 255, cv::THRESH_BINARY);

	// ----------- 邊緣偵測與反光降噪 -----------
	cv::Mat SrcGray, RefGray, SrcEdge, RefEdge;
	cv::Mat grad_x, grad_y, grad_mag;
	double med, low, high;

	// --- Src ---
	if (false == EdgeWithDirectionConsistency(SrcCalibration, SrcEdge, 3, 30, 30, 3, 0.4, true)) { return false; }
	//cv::cvtColor(SrcCalibration, SrcGray, cv::COLOR_BGR2GRAY);
	//cv::Sobel(SrcGray, grad_x, CV_32F, 1, 0);
	//cv::Sobel(SrcGray, grad_y, CV_32F, 0, 1);
	//cv::magnitude(grad_x, grad_y, grad_mag);
	////med = cv::mean(grad_mag)[0];
	////low = std::max(0.0, 0.66 * med);
	//cv::Canny(SrcGray, SrcEdge, MinEdge, MinEdge);

	// --- Ref ---
	if (false == EdgeWithDirectionConsistency(Ref, RefEdge)) { return false; }
	//cv::cvtColor(Ref, RefGray, cv::COLOR_BGR2GRAY);
	//cv::bilateralFilter(RefGray, RefGray, 9, 75, 75); // 邊緣保留降噪
	//cv::Sobel(RefGray, grad_x, CV_32F, 1, 0);
	//cv::Sobel(RefGray, grad_y, CV_32F, 0, 1);
	//cv::magnitude(grad_x, grad_y, grad_mag);
	//med = cv::mean(grad_mag)[0];
	//high = std::min(255.0, 1.33 * med);
	//cv::Canny(RefGray, RefEdge, med, high);

	// 邊緣膨脹 & 合併二值化
	static const cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, { 3, 3 });
	cv::dilate(SrcEdge, SrcEdge, kernel);
	cv::dilate(RefEdge, RefEdge, kernel);
	cv::bitwise_and(posBinary, SrcEdge, posBinary);
	cv::bitwise_and(negBinary, SrcEdge, negBinary);

#ifdef _CV_DEBUG
	cv::imshow("SrcEdge", SrcEdge);
	cv::imshow("posBinary", posBinary);
	cv::imshow("negBinary", negBinary);
	cv::waitKey(0);
#endif

#ifdef TIME_DEBUG
	QueryPerformanceCounter(&nEndTime);
	Time.push_back((nEndTime.QuadPart - nStartTime.QuadPart) * 1000 / QuadPart);
	QueryPerformanceCounter(&nStartTime);
#endif

	// ----------- 差異區域輪廓比對 -----------
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Rect> boundingRects;
	cv::Mat mask = cv::Mat::zeros(Src.size(), CV_8U);

	cv::Mat SrcLAB, RefLAB;
	cv::cvtColor(Ref, RefLAB, cv::COLOR_BGR2Lab);
	cv::cvtColor(SrcCalibration, SrcLAB, cv::COLOR_BGR2Lab);

	const double ColorExpandLAB = 2.5;

	// ---- 正差異 ----
	if (!ExecSearchContours(posBinary, contours, boundingRects)) return false;
	for (size_t i = 0; i < boundingRects.size(); i++) {
		if (CheckIsShiftError_vEdge(SrcCalibration, RefEdge, AnchorPoint, boundingRects[i], contours[i], WndSizeW, WndSizeH, 0.1)) continue;
		if (CheckIsShiftError(SrcLAB, RefLAB, AnchorPoint, boundingRects[i], contours[i], WndSizeW, WndSizeH, ColorExpandLAB)) continue;
		cv::drawContours(mask, contours, (int)i, cv::Scalar(255), cv::FILLED);
	}

	// ---- 負差異 ----
	if (!ExecSearchContours(negBinary, contours, boundingRects)) return false;
	for (size_t i = 0; i < boundingRects.size(); i++) {
		if (CheckIsShiftError_vEdge(SrcCalibration, RefEdge, AnchorPoint, boundingRects[i], contours[i], WndSizeW, WndSizeH, 0.1)) continue;
		if (CheckIsShiftError(SrcLAB, RefLAB, AnchorPoint, boundingRects[i], contours[i], WndSizeW, WndSizeH, ColorExpandLAB)) continue;
		cv::drawContours(mask, contours, (int)i, cv::Scalar(255), cv::FILLED);
	}

	// ----------- 輸出結果 -----------
	const IMAGE_SIZE MaskBitCount = 8;
	const IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(SrcImageW, MaskBitCount, 4);
	cv::dilate(mask, mask, kernel);
	cv::Mat dst(mask.size(), mask.type(), DstPtr, MaskStep);
	mask.copyTo(dst);

#ifdef TIME_DEBUG
	QueryPerformanceCounter(&nEndTime);
	Time.push_back((nEndTime.QuadPart - nStartTime.QuadPart) * 1000 / QuadPart);
#endif

#ifdef _CV_DEBUG
	cv::destroyAllWindows();
#endif
	return true;

}
//-------------------------------------------------------------------------------------//
bool CImageAPI::TrainColorCalibrationCoeff(const cv::Mat & Src, const cv::Mat & Ref, const int sampleStep, cv::Mat &Coeff)
{
	if (Src.empty() || Ref.empty() ||
		Src.size() != Ref.size() ||
		Src.type() != CV_8UC3 || Ref.type() != CV_8UC3) {
		m_ErrorString = L"Error: invalid training data.";
		return false;
	}
	if (sampleStep < 1) {
		m_ErrorString = L"Error: sampleStep must be positive";
		return false;
	}
	BuildPolynomial13Matrices(Src, Ref, Coeff, sampleStep);
	return true;
}
//-------------------------------------------------------------------------------------//
void CImageAPI::BuildPolynomial13Matrices(const cv::Mat & Src, const cv::Mat & Ref, cv::Mat& Coeff, int sampleStep)
{
	using namespace cv;
	//CV_Assert(!Src.empty() && !Ref.empty());
	//CV_Assert(Src.size() == Ref.size() && Src.type() == CV_8UC3 && Ref.type() == CV_8UC3);

	const int rows = Src.rows;
	const int cols = Src.cols;

	// === 初始化矩陣 ===
	Mat L = Mat::zeros(13, 13, CV_64F);
	Mat R = Mat::zeros(13, 3, CV_64F);

	// 指標快取，加速矩陣操作
	double* pL[13];
	double* pR[13];
	for (int i = 0; i < 13; ++i) {
		pL[i] = L.ptr<double>(i);
		pR[i] = R.ptr<double>(i);
	}

	// === 掃描所有像素並累積特徵 ===
	for (int y = 0; y < rows; y += sampleStep) {
		const cv::Vec3b* pSrcRow = Src.ptr<cv::Vec3b>(y);
		const cv::Vec3b* pRefRow = Ref.ptr<cv::Vec3b>(y);
		for (int x = 0; x < cols; x += sampleStep) {

			Vec3b sPix = pSrcRow[x];
			Vec3b tPix = pRefRow[x];

			double r = sPix[2] / 255.0;
			double g = sPix[1] / 255.0;
			double b = sPix[0] / 255.0;

			// 目標值 (R,G,B)
			double Rt = tPix[2] / 255.0;
			double Gt = tPix[1] / 255.0;
			double Bt = tPix[0] / 255.0;

			// === 13項多項式特徵 ===
			double feat[13] = {
				r, g, b,
				r * g, r * b, g * b,
				r * g * g, r * b * b, g * r * r,
				g * b * b, b * r * r, b * g * g,
				r * g * b
			};

			// === 累積 L = S^T * S ===
			for (int i = 0; i < 13; ++i)
				for (int j = i; j < 13; ++j)
					pL[i][j] += feat[i] * feat[j];

			// === 累積 R = S^T * T ===
			for (int i = 0; i < 13; ++i) {
				pR[i][0] += feat[i] * Rt;
				pR[i][1] += feat[i] * Gt;
				pR[i][2] += feat[i] * Bt;
			}
		}
	}

	// === 對稱填補 L 的下三角 ===
	for (int i = 0; i < 13; ++i)
		for (int j = i + 1; j < 13; ++j)
			pL[j][i] = pL[i][j];

	// === 解 L * Coeff = R ===
	if (!solve(L, R, Coeff, DECOMP_SVD)) {
		Coeff.release();
		m_ErrorString = L"[BuildPolynomial13Matrices] Failed to solve coefficients.";
		return;
	}
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::ApplyColorCalibrationCoeff(const cv::Mat& Src, const cv::Mat& Coeff, cv::Mat& Dst)
{
	using namespace cv;
	// 檢查
	if (Src.empty() || Src.type() != CV_8UC3) {
		return false;
	}
	if (Coeff.empty() || Coeff.rows != 13 || Coeff.cols != 3 || Coeff.type() != CV_64F) {
		return false;
	}

	// 輸出初始化
	Dst = Src.clone();

	// 預取 Coeff 每一列的 pointer，加速內部計算
	const double* pCoefRow[13];
	for (int i = 0; i < 13; ++i) {
		pCoefRow[i] = Coeff.ptr<double>(i); // 每列有 3 個 double (col0,col1,col2)
	}

	const int rows = Dst.rows;
	const int cols = Dst.cols;

	for (int y = 0; y < rows; ++y) {
		Vec3b* pDstRow = Dst.ptr<Vec3b>(y);
		const Vec3b* pSrcRow = Src.ptr<Vec3b>(y);
		for (int x = 0; x < cols; ++x) {
			// 讀取原始 BGR，並正規化到 [0,1]
			const Vec3b s = pSrcRow[x];
			double r = s[2] / 255.0;
			double g = s[1] / 255.0;
			double b = s[0] / 255.0;

			// 計算 13 維特徵（與 BuildPolynomial13Matrices 一致）
			double RG = r * g;
			double RB = r * b;
			double GB = g * b;
			double RG2 = RG * g;   // r * g^2
			double RB2 = RB * b;   // r * b^2
			double GR2 = RG * r;   // g * r^2
			double GB2 = GB * b;   // g * b^2
			double BR2 = RB * r;   // b * r^2
			double BG2 = GB * g;   // b * g^2
			double RGB = r * g * b;

			double feat[13];
			feat[0] = r;
			feat[1] = g;
			feat[2] = b;
			feat[3] = RG;
			feat[4] = RB;
			feat[5] = GB;
			feat[6] = RG2;
			feat[7] = RB2;
			feat[8] = GR2;
			feat[9] = GB2;
			feat[10] = BR2;
			feat[11] = BG2;
			feat[12] = RGB;

			// 計算輸出三通道（在 normalized 空間）
			double outR = 0.0, outG = 0.0, outB = 0.0;
			// Coeff row i: pCoefRow[i][0..2] 對應 R,G,B 的係數
			for (int i = 0; i < 13; ++i) {
				const double f = feat[i];
				const double* pc = pCoefRow[i];
				outR += f * pc[0];
				outG += f * pc[1];
				outB += f * pc[2];
			}

			int rr = cv::saturate_cast<uchar>(outR * 255.0);
			int gg = cv::saturate_cast<uchar>(outG * 255.0);
			int bb = cv::saturate_cast<uchar>(outB * 255.0);

			double diffR = double(rr) - double(s[2]);
			double diffG = double(gg) - double(s[1]);
			double diffB = double(bb) - double(s[0]);
			double colorDiff = std::sqrt(diffB * diffB + diffG * diffG + diffR * diffR);

			// --- 如果差異太大，跳過該像素 ---
			if (colorDiff > 100) {
				pDstRow[x] = s;
			}
			else {
				pDstRow[x] = Vec3b((uchar)bb, (uchar)gg, (uchar)rr);
			}
		}
	}

	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::ExecSearchContours(const cv::Mat & SrcMask, std::vector<std::vector<cv::Point>>& contours, std::vector<cv::Rect>& boundingRects, bool bToEdge, bool bBoundary, bool bRectangle, double nMinPerimeter, double nMaxPerimeter)
{
	//blob功能或許可以取代
	// --- 輸入檢查 ---
	if (SrcMask.empty() || SrcMask.channels() != 1) {
		std::cerr << "Error: Input must be single-channel Mat." << std::endl;
		return false;
	}

	cv::Mat img;
	SrcMask.copyTo(img);

	// --- 若需轉為邊界圖 ---
	if (bToEdge) {
		cv::Canny(img, img, 80, 160);
	}

	// --- 找輪廓 ---
	std::vector<std::vector<cv::Point>> foundContours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(img, foundContours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	contours.clear();
	boundingRects.clear();

	// --- 篩選輪廓 ---
	for (auto& contour : foundContours) {
		double perimeter = cv::arcLength(contour, true);
		if (perimeter < nMinPerimeter || perimeter > nMaxPerimeter)
			continue;

		double area = cv::contourArea(contour, true);

		// 忽略整張圖大小的輪廓（防止整區都被抓進來）
		if (std::fabs(area) > 0.9 * img.cols * img.rows)
			continue;

		contours.push_back(contour);

		if (bRectangle) {
			boundingRects.push_back(cv::boundingRect(contour));
		}
	}

	// --- 若要求邊界也視為一個輪廓 ---
	if (bBoundary) {
		std::vector<cv::Point> frame{
			{ 0, 0 },
			{ img.cols - 1, 0 },
			{ img.cols - 1, img.rows - 1 },
			{ 0, img.rows - 1 }
		};
		contours.push_back(frame);
		boundingRects.push_back(cv::boundingRect(frame));
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::EdgeWithDirectionConsistency(const cv::Mat & Src, cv::Mat & Dst, int bilateral_d, double sigmaColor, double sigmaSpace, int ksize, double consistencyThresh, bool bEnhance)
{
	// --- 檢查輸入 ---
	if (Src.empty()) {
		std::cerr << "[EdgeWithDirectionConsistency] Error: Src is empty." << std::endl;
		return false;
	}
	if (ksize <= 1 || consistencyThresh <= 0.0 || consistencyThresh > 1.0) {
		std::cerr << "[EdgeWithDirectionConsistency] Error: Invalid parameters." << std::endl;
		return false;
	}

	// --- 轉灰階 ---
	cv::Mat gray;
	if (Src.channels() == 3)
		cv::cvtColor(Src, gray, cv::COLOR_BGR2GRAY);
	else
		gray = Src.clone();

	// --- 雙邊濾波 (可選) ---
	cv::Mat filtered;
	if (bilateral_d > 0)
		cv::bilateralFilter(gray, filtered, bilateral_d, sigmaColor, sigmaSpace);
	else
		filtered = gray.clone();

	// --- 計算梯度方向與大小 ---
	cv::Mat grad_x, grad_y, grad_mag, grad_dir;
	cv::Sobel(filtered, grad_x, CV_32F, 1, 0);
	cv::Sobel(filtered, grad_y, CV_32F, 0, 1);
	cv::cartToPolar(grad_x, grad_y, grad_mag, grad_dir, true); // degree: 0–360°

															   // --- 使用查表方式計算 sin, cos ---
	cv::Mat sin_dir = cv::Mat::zeros(grad_dir.size(), CV_32F);
	cv::Mat cos_dir = cv::Mat::zeros(grad_dir.size(), CV_32F);

	const float DEG2RAD = CV_PI / 180.0f;
	for (int y = 0; y < grad_dir.rows; ++y) {
		const float* dirPtr = grad_dir.ptr<float>(y);
		float* sinPtr = sin_dir.ptr<float>(y);
		float* cosPtr = cos_dir.ptr<float>(y);
		for (int x = 0; x < grad_dir.cols; ++x) {
			float rad = dirPtr[x] * DEG2RAD;
			sinPtr[x] = std::sin(rad);
			cosPtr[x] = std::cos(rad);
		}
	}

	// --- 平均方向一致性 ---
	cv::Mat mean_sin, mean_cos;
	cv::blur(sin_dir, mean_sin, cv::Size(ksize, ksize));
	cv::blur(cos_dir, mean_cos, cv::Size(ksize, ksize));

	cv::Mat dir_consistency;
	cv::magnitude(mean_cos, mean_sin, dir_consistency); // 0~1

	cv::Mat mask_consistent;
	cv::threshold(dir_consistency, mask_consistent, consistencyThresh, 1.0, cv::THRESH_BINARY);
	mask_consistent.convertTo(mask_consistent, CV_8U, 255);

	// --- 自適應 Canny ---
	double med, high, low;
	if (false == bEnhance) {
		med = cv::mean(grad_mag)[0];
		high = std::min(255.0, 1.33 * med);
		low = std::max(0.0, 0.66 * med);
	}
	else {
		med = cv::mean(grad_mag)[0];
		high = std::min(255.0, 1.1 * med);
		low = std::max(0.0, 0.5 * med);
	}

	cv::Mat edge;
	cv::Canny(filtered, edge, low, high);

	// --- 保留方向一致邊 ---
	cv::bitwise_and(edge, mask_consistent, Dst);

	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::CheckIsShiftError(const cv::Mat & Src, const cv::Mat & Ref, const cv::Point AnchorPoint, const cv::Rect & rect,
	std::vector<cv::Point> contour, size_t WndSizeW, size_t WndSizeH, double thresholdColor)
{
	using namespace cv;

#ifdef TIME_DEBUG
	std::vector<double> Time;
	LARGE_INTEGER     nStartTime;
	LARGE_INTEGER     nEndTime;
	LONGLONG QuadPart = AOIDataCollect.m_SystemFreq.QuadPart;
	QueryPerformanceCounter(&nStartTime);
#endif // TIME_DEBUG

	// 建立 contour mask (全圖大小)
	std::vector<std::vector<Point>> contours;
	contours.push_back(contour);
	Mat mask = Mat::zeros(Src.size(), CV_8U);
	drawContours(mask, contours, 0, Scalar(255), FILLED);

	// 只把位於 rect 內的 mask 點收集起來，並存成相對於 rect 的座標 (local)
	std::vector<cv::Point> maskLocalPoints;
	std::vector<cv::Vec3b> sampledPixels;
	for (int y = rect.y; y < rect.y + rect.height; ++y) {
		const uchar* rowPtr = mask.ptr<uchar>(y);
		const cv::Vec3b* srcRow = Src.ptr<cv::Vec3b>(y);
		for (int x = rect.x; x < rect.x + rect.width; ++x) {
			if (rowPtr[x] > 0) {
				maskLocalPoints.emplace_back(x - rect.x, y - rect.y); // local point
				sampledPixels.push_back(srcRow[x]);  // 直接取像素，不呼叫 .at()
			}
		}
	}
	//bool bColorthreshold = true;
	//if (!sampledPixels.empty()) {
	//	Mat pixelsMat(sampledPixels.size(), 1, CV_8UC3, sampledPixels.data());
	//	// 計算平均值
	//	Scalar meanLab = mean(pixelsMat);
	//	double L = meanLab[0];
	//	double a = meanLab[1] - 128.0;
	//	double b = meanLab[2] - 128.0;
	//	// 若接近白色 (L>80 且 a,b 在 -10~10)
	//	if (L > 80 && std::abs(a) < 10 && std::abs(b) < 10) {
	//		bColorthreshold = false;
	//	}
	//}
	const double thresholdL = thresholdColor * 5;
	int count = static_cast<int>(maskLocalPoints.size());
	if (count == 0) {
		// 沒有要比對的像素，視為 no error（或根據需求回傳 true/false）
		return true;
	}

	// 取 ROI（target ROI 為 Src 的 rect）
	Mat tgtROI = Src(rect);  // size = rect.size()

	Point bestShift(0, 0);
	bool bfoundAny = false;

#ifdef TIME_DEBUG
	QueryPerformanceCounter(&nEndTime);
	Time.push_back((nEndTime.QuadPart - nStartTime.QuadPart) * 1000 / QuadPart);
	QueryPerformanceCounter(&nStartTime);
#endif // TIME_DEBUG

	double totalL, totalA, totalB;
	Mat tmpROI;
	Rect shiftedRect;
	int WndSizeH2 = WndSizeH >> 1;
	int WndSizeW2 = WndSizeW >> 1;
	// 針對每一個候選位移做搜尋.
	int dx, dy;
	double minErrAB = DBL_MAX, meanErrAB = DBL_MAX;
	double minErrL = DBL_MAX, meanErrL = DBL_MAX;

	for (dy = -WndSizeH2; dy <= WndSizeH2; ++dy) {
		for (dx = -WndSizeW2; dx <= WndSizeW2; ++dx) {
			// 構造移位後的 rect
			shiftedRect = rect + Point(dx, dy) + AnchorPoint;
			// 若移出邊界就跳過
			if (shiftedRect.x < 0 || shiftedRect.y < 0 ||
				shiftedRect.x + shiftedRect.width > Ref.cols ||
				shiftedRect.y + shiftedRect.height > Ref.rows)
				continue;
			tmpROI = Ref(shiftedRect); // tmpROI 與 tgtROI 相同大小
			totalL = totalA = totalB = 0.0;
			const int roiCols = rect.width;
			for (const auto& pLocal : maskLocalPoints) {
				int lx = pLocal.x;
				int ly = pLocal.y;
				const cv::Vec3b tgtPixel = tgtROI.at<cv::Vec3b>(ly, lx);
				const cv::Vec3b refPixel = tmpROI.at<cv::Vec3b>(ly, lx);
				totalL += std::abs((int)tgtPixel[0] - (int)refPixel[0]);
				totalA += std::abs((int)tgtPixel[1] - (int)refPixel[1]);
				totalB += std::abs((int)tgtPixel[2] - (int)refPixel[2]);
			}
			// mean error over channels and pixels
			/*if (false == bColorthreshold) { meanErr = totalL / count; }
			else { meanErr = (totalA + totalB) / (2 * count); }*/
			meanErrL = totalL / count;
			meanErrAB = (totalA + totalB) / (2 * count);
			if (meanErrL < minErrL && meanErrAB < minErrAB) {
				minErrL = meanErrL;
				minErrAB = meanErrAB;
				bestShift = Point(dx, dy) + AnchorPoint;
				if (minErrL < thresholdL&&minErrAB < thresholdColor) {
					bfoundAny = true;
					break;
				}
			}
		}
	}
#ifdef TIME_DEBUG
	QueryPerformanceCounter(&nEndTime);
	Time.push_back((nEndTime.QuadPart - nStartTime.QuadPart) * 1000 / QuadPart);
#endif // TIME_DEBUG

#ifdef _CV_DEBUG

	shiftedRect = rect + bestShift;
	std::vector<std::vector<cv::Point>> contours_shift;
	std::vector<cv::Point> cshift;
	for (cv::Point it : contour) {
		cshift.push_back(it + bestShift);
	}
	contours_shift.push_back(cshift);

	cv::Scalar color = (bfoundAny) ? cv::Scalar(0, 255, 0) : cv::Scalar(0, 0, 255);

	cv::Mat template_Show = Ref.clone();
	cv::Mat target_Show = Src.clone();
	cv::cvtColor(template_Show, template_Show, cv::COLOR_Lab2BGR);
	cv::cvtColor(target_Show, target_Show, cv::COLOR_Lab2BGR);

	//if (rect.area() > 50 && minErr > thresholdColor) {
	if (rect.area() > 50) {
		cv::rectangle(template_Show, shiftedRect, color, 1);
		cv::rectangle(target_Show, rect, color, 1);
		cv::drawContours(template_Show, contours_shift, 0, color, cv::FILLED);
		cv::drawContours(target_Show, contours, 0, color, cv::FILLED);
		cv::imshow("template_Show", template_Show);
		cv::imshow("target_Show", target_Show);
		cv::waitKey(0);
	}

#endif // _CV_DEBUG
	return bfoundAny;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::CheckIsShiftError_vEdge(const cv::Mat & Src, const cv::Mat & RefEdge, const cv::Point AnchorPoint, const cv::Rect & rect, std::vector<cv::Point> contour, size_t WndSizeW, size_t WndSizeH, double ratio) {
#ifdef TIME_DEBUG
	std::vector<double> Time;
	LARGE_INTEGER     nStartTime;
	LARGE_INTEGER     nEndTime;
	LONGLONG QuadPart = AOIDataCollect.m_SystemFreq.QuadPart;
	QueryPerformanceCounter(&nStartTime);
#endif // TIME_DEBUG
	// 建立 contour mask
	std::vector<std::vector<cv::Point>> contours;
	contours.push_back(contour);
	cv::Mat mask = cv::Mat::zeros(Src.size(), CV_8U);
	cv::drawContours(mask, contours, 0, cv::Scalar(255), cv::FILLED);

	// 取 ROI
	cv::Mat tgtMask = mask(rect);
	cv::Point bestShift;

	cv::Mat RefEdgeBin;
	RefEdge.convertTo(RefEdgeBin, CV_8U, 1.0 / 255.0);
	cv::Mat RefEdgeIntegral;
	cv::integral(RefEdgeBin, RefEdgeIntegral, CV_32S);
	double maskSum = cv::sum(tgtMask)[0] / 255.0;
	double invMaskSum = 1.0 / maskSum;

#ifdef TIME_DEBUG
	QueryPerformanceCounter(&nEndTime);
	Time.push_back((nEndTime.QuadPart - nStartTime.QuadPart) * 1000 / QuadPart);
	QueryPerformanceCounter(&nStartTime);
#endif // TIME_DEBUG
	double minErr = DBL_MAX;
	int WndSizeH2 = WndSizeH >> 1;
	int WndSizeW2 = WndSizeW >> 1;
	int rectW = rect.width;
	int rectH = rect.height;
	int rectX = rect.x + AnchorPoint.x;
	int rectY = rect.y + AnchorPoint.y;
	for (int dy = -WndSizeH2; dy <= WndSizeH2; dy++) {
		int y1 = rectY + dy;
		int y2 = y1 + rectH;
		int* row1 = RefEdgeIntegral.ptr<int>(y1);
		int* row2 = RefEdgeIntegral.ptr<int>(y2);
		for (int dx = -WndSizeW2; dx <= WndSizeW2; dx++) {
			int x1 = rectX + dx;
			int x2 = x1 + rectW;

			int refSum = row2[x2] - row2[x1] - row1[x2] + row1[x1];
			double SumErr = (maskSum - refSum) / maskSum;

			if (SumErr < minErr) {
				minErr = SumErr;
				bestShift = cv::Point(dx, dy) + AnchorPoint;
			}
		}
	}
#ifdef TIME_DEBUG
	QueryPerformanceCounter(&nEndTime);
	Time.push_back((nEndTime.QuadPart - nStartTime.QuadPart) * 1000 / QuadPart);
#endif // TIME_DEBUG

#ifdef _CV_DEBUG
	// 顯示結果
	cv::Rect shiftedRect = rect + bestShift;
	std::vector<std::vector<cv::Point>> contours_shift;
	std::vector<cv::Point> cshift;
	for (cv::Point& it : contour) {
		cshift.push_back(it + bestShift);
	}
	contours_shift.push_back(cshift);

	cv::Scalar color;
	double threshold = ratio;
	if (minErr <= threshold) {
		color = cv::Scalar(0, 255, 0);
	}
	else {
		color = cv::Scalar(0, 0, 255);
	}

	cv::Mat template_Show = RefEdge.clone();
	cv::cvtColor(template_Show, template_Show, cv::COLOR_GRAY2BGR);
	cv::Mat target_Show = Src.clone();
	if (rect.area()>50 && minErr > ratio) {
		//if (rect.area() > 50) {
		cv::rectangle(template_Show, shiftedRect, color, 1);
		cv::rectangle(target_Show, rect, color, 1);
		cv::drawContours(template_Show, contours_shift, 0, color, cv::FILLED);
		cv::drawContours(target_Show, contours, 0, color, cv::FILLED);
		cv::imshow("template_Show", template_Show);
		cv::imshow("target_Show", target_Show);
		cv::waitKey(0);
	}
#endif // _CV_DEBUG
	return (minErr <= ratio);
}
//-------------------------------------------------------------------------------------//
/// <summary>
/// Multi-Focus Image Fusion
/// <para>結果影像(dst)記憶體需自行宣告，函示只將結果填入</para>
/// </summary>
/// <param name="srcs">輸入影像(須同樣尺寸大小且連續記憶體)</param>
/// <param name="pdst">融合結果(須與輸入影像尺寸一致)</param>
bool CImageAPI::MultiFocusImageFusionFn(const std::vector<cv::Mat> srcs, cv::Mat& dst)//MFIF//20260304-Joe
{
	//Obj_TS->EntryFunc(__func__);
	//Obj_TS->TimePoint("Start");
	if (srcs.size() <= 0 || dst.empty())
	{	return false;	}

	const bool bSupportAVX2 = cv::checkHardwareSupport(CV_CPU_AVX2);
	const int num = srcs.size();
	const int width = dst.cols;
	const int height = dst.rows;
	const int cvType = dst.type();
	const int ch = CV_MAT_CN(dst.type());
	const int levels = (std::min(width, height) >= 4096) ? 5 : (std::min(width, height) >= 2048 ? 4 : 3);
	const int typeF = CV_MAKETYPE(CV_32F, ch);

	//Obj_TS->TimePoint("prepare pyramid");
	std::vector<std::vector<cv::Mat>> laps(num, std::vector<cv::Mat>(levels - 1));
	std::vector<cv::Mat> bases(num);
	std::vector<std::vector<cv::Mat>> focus(num, std::vector<cv::Mat>(levels - 1));

	//Obj_TS->TimePoint("cal pyramid");
	// 預先建立可重用的快取，避免在迴圈內反覆分配記憶體
	cv::Mat g_curr, g_next, up, absLap;
	for (int i = 0; i < num; ++i) {
		// 1) 封裝輸入（8U，避免一開始 convertTo float）
		const cv::Mat& rawInput = srcs[i];
		CV_Assert(rawInput.depth() == CV_8U);
		// 2) 準備當層的 8U 彩色/灰階
		g_curr = rawInput;
		// 3) 逐層建 pyramid（全部在 8U 上跑 pyrDown/pyrUp）
		for (int l = 0; l < levels - 1; ++l) {
			// --- 彩色/灰階通用：Gaussian down / up（8U） ---
			cv::pyrDown(g_curr, g_next);
			cv::pyrUp(g_next, up, g_curr.size());

			// Laplacian 殘差(作為focus用)
			cv::subtract(g_curr, up, laps[i][l], cv::noArray(), typeF);

			// --- 灰階focus，1ch---
			absLap = cv::abs(laps[i][l]);
			if (ch == 3) {
				cv::cvtColor(absLap, focus[i][l], cv::COLOR_BGR2GRAY);
			}
			else {
				std::swap(focus[i][l], absLap);
			}

			// 準備下一層
			std::swap(g_curr, g_next);
		}
		// 最頂層 base：保持 8U（之後 base 融合再轉）
		bases[i] = g_curr.clone();
	}

	//Obj_TS->TimePoint("pyramid fusion");
	// 預定義索引矩陣：將 8 個單一 Mask 擴展為 24 個連續的 BGR Mask
	// idx0 對應前 8 個 float (Pixel 0, 1, 2-B, 2-G)
	const __m256i idx0 = _mm256_setr_epi32(0, 0, 0, 1, 1, 1, 2, 2);
	// idx1 對應中間 8 個 float (Pixel 2-R, 3, 4, 5-B)
	const __m256i idx1 = _mm256_setr_epi32(2, 3, 3, 3, 4, 4, 4, 5);
	// idx2 對應最後 8 個 float (Pixel 5-G, 5-R, 6, 7)
	const __m256i idx2 = _mm256_setr_epi32(5, 5, 6, 6, 6, 7, 7, 7);
	// 融合階段：使用指標或是更直接的 Mask 處理
	std::vector<cv::Mat> fusedLaps(levels - 1);
	for (int l = 0; l < levels - 1; ++l) {
		int rows = laps[0][l].rows;
		int cols = laps[0][l].cols;
		int channels = laps[0][l].channels();

		// 建立結果矩陣，初始化為第 0 張圖的內容，減少一次比較
		fusedLaps[l] = laps[0][l];
		cv::Mat& maxFocus = focus[0][l];
		for (int i = 1; i < num; ++i) {
			// 256-bit / 32-bit = 8 floats
			for (int y = 0; y < rows; ++y) {
				float* pFocus = focus[i][l].ptr<float>(y);
				float* pLap = laps[i][l].ptr<float>(y);
				float* pMaxF = maxFocus.ptr<float>(y);
				float* pFused = fusedLaps[l].ptr<float>(y);

				int x = 0;
				if (bSupportAVX2) {
					// 每次處理 8 個 float (Focus 是單通道)
					for (; x <= cols - 8; x += 8) {
						// 載入當前與紀錄的最大 Focus
						__m256 vFocus = _mm256_loadu_ps(&pFocus[x]);
						__m256 vMaxF = _mm256_loadu_ps(&pMaxF[x]);

						// 比較：vFocus > vMaxF
						__m256 mask = _mm256_cmp_ps(vFocus, vMaxF, _CMP_GT_OQ);

						// 更新 MaxFocus：如果 mask 為真則選 vFocus，否則選 vMaxF
						vMaxF = _mm256_blendv_ps(vMaxF, vFocus, mask);
						_mm256_storeu_ps(&pMaxF[x], vMaxF);

						// 處理像素融合 (Fused Lap)
						if (ch == 1) {
							// --- 灰階 1-Ch SIMD ---
							__m256 vLap = _mm256_loadu_ps(&pLap[x]);
							__m256 vFused = _mm256_loadu_ps(&pFused[x]);
							vFused = _mm256_blendv_ps(vFused, vLap, mask);
							_mm256_storeu_ps(&pFused[x], vFused);
						}
						else if (ch == 3) {
							// --- 彩色 3-Ch SIMD (Branchless) ---
							// 利用索引矩陣將 8 個 mask 擴展成 24 個 (3 條通道)
							__m256 mask0 = _mm256_permutevar8x32_ps(mask, idx0);
							__m256 mask1 = _mm256_permutevar8x32_ps(mask, idx1);
							__m256 mask2 = _mm256_permutevar8x32_ps(mask, idx2);

							// 一次處理 24 個 float (即 8 個 BGR 像素)
							// 第一組 8 floats
							_mm256_storeu_ps(&pFused[x * 3], _mm256_blendv_ps(_mm256_loadu_ps(&pFused[x * 3]), _mm256_loadu_ps(&pLap[x * 3]), mask0));
							// 第二組 8 floats
							_mm256_storeu_ps(&pFused[x * 3 + 8], _mm256_blendv_ps(_mm256_loadu_ps(&pFused[x * 3 + 8]), _mm256_loadu_ps(&pLap[x * 3 + 8]), mask1));
							// 第三組 8 floats
							_mm256_storeu_ps(&pFused[x * 3 + 16], _mm256_blendv_ps(_mm256_loadu_ps(&pFused[x * 3 + 16]), _mm256_loadu_ps(&pLap[x * 3 + 16]), mask2));
						}
					}
				}
				// 處理剩餘像素 (Remainder)
				for (; x < cols; ++x) {
					if (pFocus[x] > pMaxF[x]) {
						pMaxF[x] = pFocus[x];
						for (int c = 0; c < ch; ++c) pFused[x * ch + c] = pLap[x * ch + c];
					}
				}
			}
		}
	}

	// ... 後續融合與重建 ...
	//Obj_TS->TimePoint("fusedBase");
	cv::Mat fusedBase = cv::Mat::zeros(bases[0].size(), typeF);
	float invNum = 1.0f / (float)num;
	__m256 vInvNum = _mm256_set1_ps(invNum);
	int rows = bases[0].rows;
	int cols = bases[0].cols * bases[0].channels();
	for (int y = 0; y < rows; ++y) {
		float* pFused = fusedBase.ptr<float>(y);
		std::vector<const uchar*> ptrs(num); // 取得所有圖層在這一行的 8U 指標
		for (int i = 0; i < num; ++i) ptrs[i] = bases[i].ptr<uchar>(y);

		int x = 0;
		if (bSupportAVX2) {
			// AVX2 每次處理 8 個像素 (從 8U 載入後轉為 32F)
			for (; x <= cols - 8; x += 8) {
				__m256 vSum = _mm256_setzero_ps();
				for (int i = 0; i < num; ++i) {
					// 1. 載入 8 個 uint8 (64-bit data)
					__m128i v8u = _mm_loadl_epi64((__m128i*)(ptrs[i] + x));
					// 2. 將 uint8 提升至 int32 (256-bit)
					__m256i v32i = _mm256_cvtepu8_epi32(v8u);
					// 3. 將 int32 轉為 float32
					__m256 v32f = _mm256_cvtepi32_ps(v32i);
					// 4. 累加到 vSum
					vSum = _mm256_add_ps(vSum, v32f);
				}
				// 最後乘以平均權重並儲存
				_mm256_storeu_ps(pFused + x, _mm256_mul_ps(vSum, vInvNum));
			}
		}
		// 剩餘像素處理
		for (; x < cols; ++x) {
			float sum = 0;
			for (int i = 0; i < num; ++i) sum += (float)ptrs[i][x];
			pFused[x] = sum * invNum;
		}
	}

	//Obj_TS->TimePoint("rebuild");
	// 預先配置最大尺寸的兩個 Buffer，避免迴圈內重複 create
	cv::Mat buf1(fusedLaps[0].size(), typeF);
	cv::Mat buf2(fusedLaps[0].size(), typeF);
	cv::Mat* pCurrentRes = &buf1;
	cv::Mat* pNextRes = &buf2;
	// 初始頂層資料拷貝
	fusedBase.copyTo((*pCurrentRes)(cv::Rect(0, 0, fusedBase.cols, fusedBase.rows)));
	for (int l = levels - 2; l >= 0; --l) {
		cv::Size nextSize = fusedLaps[l].size();
		cv::Mat currentView = (*pCurrentRes)(cv::Rect(0, 0, fusedBase.cols, fusedBase.rows));
		cv::Mat nextView = (*pNextRes)(cv::Rect(0, 0, nextSize.width, nextSize.height));

		// 1. 執行 pyrUp 
		cv::pyrUp(currentView, nextView, nextSize);

		// 2. 使用 AVX2 直接將殘差加回，不產生臨時矩陣
		int rows = nextView.rows;
		int total_elements = nextView.cols * nextView.channels();
		for (int y = 0; y < rows; ++y) {
			float* pDst = nextView.ptr<float>(y);
			const float* pSrc = fusedLaps[l].ptr<float>(y);
			int x = 0;
			if (bSupportAVX2) {
				for (; x <= total_elements - 8; x += 8) {
					__m256 vDst = _mm256_loadu_ps(pDst + x);
					__m256 vSrc = _mm256_loadu_ps(pSrc + x);
					_mm256_storeu_ps(pDst + x, _mm256_add_ps(vDst, vSrc));
				}
			}
			for (; x < total_elements; ++x) {
				pDst[x] += pSrc[x];
			}
		}
		// 指標切換 (Ping-Pong)
		std::swap(pCurrentRes, pNextRes);
		fusedBase = nextView; // 更新當前尺寸參考
	}

	//Obj_TS->TimePoint("set result");
	fusedBase.convertTo(dst, cvType);
	//Obj_TS->EndTimePoint();
	//Obj_TS->Show_TimeSpanReport("TimeSpen");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::MultiFocusImageFusion(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const std::vector<IMAGE_PTR> &ImagePtrList, IMAGE_PTR &DstPtr)//MFIF//20260304-Joe
{
	const char fnName[] = "CImageAPI::MultiFocusImageFusion";	
	if ( CheckGrayImageList(ImageW, ImageH, ImageStep, ImagePtrList) == false ) { return false; }		
	JetMemory.free_func(DstPtr);		
	const size_t BufferSize = CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, DstPtr, fnName, "DstPtr") == false )
	{
		SetErrorString(JetMemory.GetErrorString());
		return false;
	}
	if ( MultiFocusImageFusion3(ImageW, ImageH, ImageStep, BitCount, ImagePtrList, DstPtr) == false )
	{
		JetMemory.free_func(DstPtr);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::MultiFocusImageFusion3(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, const std::vector<IMAGE_PTR> &ImagePtrList, IMAGE_PTR DstPtr)//MFIF//20260304-Joe
{
	if ( CheckPtr(DstPtr) == false ) { return false; }
	if ( CImageAPI::CheckBitCount(BitCount) == false ) { return false; }
	if ( CheckGrayImageList(ImageW, ImageH, ImageStep, ImagePtrList) == false ) { return false; }				
	
	const size_t ImageCnt=ImagePtrList.size();
	const size_t BufferSize=CalcBufferSize(ImageStep, ImageH);

	std::vector<cv::Mat> MatList;
	for ( size_t i=0; i<ImageCnt; i++ )
	{
		const IMAGE_PTR ImagePtr=ImagePtrList[i];
		if ( NULL == ImagePtr ) { continue; }
		cv::Mat Mat=CreateMat(ImageW, ImageH, ImageStep, BitCount, ImagePtr);	
		MatList.push_back(Mat);
	}	
	const size_t MatCount=MatList.size();
	cv::Mat MatDst=CreateMat(ImageW, ImageH, ImageStep, BitCount, DstPtr);	
	if ( MultiFocusImageFusionFn(MatList, MatDst) == false )
	{
		SetErrorString(_T("Error, Exec MultiFocusImageFusionFn Fault"));				
		return false; 
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::ExecAnglePCA_Binary(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR & pImage, double & dSlope, double & dAngle, TPOINT2D & cvdCenterPt)
{
	const char fnName[] = ("CImageAPI::ExecAnglePCA_Binary");
	if (this->CheckGrayImage(ImageW, ImageH, ImageStep, pImage) == false) { return false; }
	if (BitCount != 8) { return false; }

	dSlope = 0.0;
	dAngle = 0.0;
	cvdCenterPt.x = 0.0;
	cvdCenterPt.y = 0.0;

	const double dEps = 1e-12;
	double dSumX = 0.0;
	double dSumY = 0.0;
	double dSumXX = 0.0;
	double dSumYY = 0.0;
	double dSumXY = 0.0;

	size_t nPointCount = 0;

	const size_t szImageW = (size_t)ImageW;
	const size_t szImageH = (size_t)ImageH;
	const size_t szImageStep = (size_t)ImageStep;
	size_t Idx = 0;
	for (size_t y = 0; y < szImageH; y++)
	{
		for (size_t x = 0; x < szImageW; x++)
		{
			Idx = y*szImageStep + x;
			if (pImage[Idx] == 0) { continue; }
			const double dx = (double)x;
			const double dy = (double)y;
			dSumX += dx;
			dSumY += dy;
			dSumXX += dx * dx;
			dSumYY += dy * dy;
			dSumXY += dx * dy;

			++nPointCount;
		}
	}

	if (nPointCount < 2) { return false; }

	const double dInvN = 1.0 / (double)nPointCount;

	const double dMeanX = dSumX * dInvN;
	const double dMeanY = dSumY * dInvN;

	cvdCenterPt.x = dMeanX;
	cvdCenterPt.y = dMeanY;

	// Covariance matrix:
	//
	// [ covXX  covXY ]
	// [ covXY  covYY ]
	//
	// 是否除以 N 不影響 PCA 主方向，所以這裡不除以 N。
	const double dCovXX = dSumXX - dSumX * dSumX * dInvN;
	const double dCovYY = dSumYY - dSumY * dSumY * dInvN;
	const double dCovXY = dSumXY - dSumX * dSumY * dInvN;

	// 所有點幾乎集中在同一點，無法估計方向。
	if (std::fabs(dCovXX) < dEps &&
		std::fabs(dCovYY) < dEps &&
		std::fabs(dCovXY) < dEps)
	{
		return false;
	}

	// 2D PCA 主軸角度公式：
	//
	// theta = 0.5 * atan2(2 * covXY, covXX - covYY)
	//
	// theta 是最大特徵值對應的方向。
	const double dTheta = 0.5 * std::atan2(2.0 * dCovXY, dCovXX - dCovYY);

	double dAngleDeg = dTheta * 180.0 / CV_PI;
	while (dAngleDeg >= 90.0) { dAngleDeg -= 180.0; }
	while (dAngleDeg < -90.0) { dAngleDeg += 180.0; }

	dAngle = dAngleDeg;
	const double dThetaNorm = dAngle * CV_PI / 180.0;
	const double dVecX = std::cos(dThetaNorm);
	const double dVecY = std::sin(dThetaNorm);

	if (std::fabs(dVecX) < dEps)
	{
		dSlope = (dVecY >= 0.0) ? DBL_MAX : -DBL_MAX;
	}
	else
	{
		dSlope = dVecY / dVecX;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CImageAPI::ExecGetPCAAxisEndPoint_Binary(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, IMAGE_SIZE ImageStep, IMAGE_SIZE BitCount, IMAGE_PTR & pImage, const TPOINT2D & cvdCenterPt, double dAngle, TPOINT2D & cvdAxisPt1, TPOINT2D & cvdAxisPt2)
{
	const char fnName[] = ("CImageAPI::GetPCAAxisEndPoint_Binary");

	cvdAxisPt1.x = 0.0;
	cvdAxisPt1.y = 0.0;
	cvdAxisPt2.x = 0.0;
	cvdAxisPt2.y = 0.0;

	if (BitCount != 8) { return false; }

	if (this->CheckGrayImage(ImageW, ImageH, ImageStep, pImage) == false) { return false; }

	const double dRad = dAngle * CV_PI / 180.0;
	const double dVecX = std::cos(dRad);
	const double dVecY = std::sin(dRad);
	const double dEps = 1e-12;

	if (std::fabs(dVecX) < dEps && std::fabs(dVecY) < dEps) { return false; }

	bool bFindPoint = false;
	double dMinT = 0.0;
	double dMaxT = 0.0;

	const size_t szImageW = (size_t)ImageW;
	const size_t szImageH = (size_t)ImageH;
	const size_t szImageStep = (size_t)ImageStep;

	for (size_t y = 0; y < szImageH; ++y)
	{
		const unsigned char* pRow = pImage + y * szImageStep;
		for (size_t x = 0; x < szImageW; ++x)
		{
			if (pRow[x] == 0)
			{
				continue;
			}

			const double dx = (double)x - cvdCenterPt.x;
			const double dy = (double)y - cvdCenterPt.y;

			// 將前景點投影到 PCA 主軸上
			const double dT = dx * dVecX + dy * dVecY;

			if (bFindPoint == false)
			{
				dMinT = dT;
				dMaxT = dT;
				bFindPoint = true;
			}
			else
			{
				if (dT < dMinT) { dMinT = dT; }
				if (dT > dMaxT) { dMaxT = dT; }
			}
		}
	}

	if (bFindPoint == false){	return false;	}

	//cvdAxisPt1.x = cvdCenterPt.x + dMinT * dVecX;
	//cvdAxisPt1.y = cvdCenterPt.y + dMinT * dVecY;

	//cvdAxisPt2.x = cvdCenterPt.x + dMaxT * dVecX;
	//cvdAxisPt2.y = cvdCenterPt.y + dMaxT * dVecY;

	cvdAxisPt1.x = dMinT * dVecX;
	cvdAxisPt1.y = -dMinT * dVecY;

	cvdAxisPt2.x = dMaxT * dVecX;
	cvdAxisPt2.y = -dMaxT * dVecY;

	return true;
}
//-------------------------------------------------------------------------------------//


#endif//OPENCV_DISABLE
