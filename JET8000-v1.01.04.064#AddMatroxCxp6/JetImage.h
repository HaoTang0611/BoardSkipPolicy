// JetImage.h: interface for the CJetImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JETIMAGE_H__1EE3820A_40F8_48EC_AD54_1AB230498740__INCLUDED_)
#define AFX_JETIMAGE_H__1EE3820A_40F8_48EC_AD54_1AB230498740__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
class CJetImage  
{	
private:
	//--------------------------------------------------------------------------------//
	IMAGE_PTR                  m_Ptr;
	IMAGE_SIZE                 m_Width;
	IMAGE_SIZE                 m_Height;
	IMAGE_SIZE                 m_Step;
	IMAGE_SIZE                 m_BitCount;	
	//--------------------------------------------------------------------------------//
protected:
	//--------------------------------------------------------------------------------//
	void                       PreInitImage();
	void                       ReleaseImage();
	bool                       CheckSelfImage() const;
	bool                       CheckImage(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE BitCount, const IMAGE_PTR Ptr) const;
	bool                       CheckRGBImage(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, const IMAGE_PTR PtrR, const IMAGE_PTR PtrG, const IMAGE_PTR PtrB) const;
	//--------------------------------------------------------------------------------//
	int                        GetImageChannels(IMAGE_SIZE BitCount) const;//取得影像通道數	
	IMAGE_SIZE                 GetImageRealWidth(IMAGE_SIZE ImageW, IMAGE_SIZE BitCount) const;//取得影像實際寬度
	IMAGE_SIZE                 GetImageAlignedWidth(IMAGE_SIZE ImageW, IMAGE_SIZE BitCount, int Align) const;//取得影像對齊寬度
	//--------------------------------------------------------------------------------//
	bool                       SetImage(const CvMat *ImagePtr);
	bool                       SetImage(const IplImage *ImagePtr);
	//--------------------------------------------------------------------------------//
public:
	//--------------------------------------------------------------------------------//
	CJetImage();
	CJetImage(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE Bits, IMAGE_PTR Ptr, bool bClone);
	CJetImage(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_PTR PtrR, IMAGE_PTR PtrG, IMAGE_PTR PtrB);
	CJetImage(const CJetImage &Image);
	CJetImage(const CvMat *ImagePtr);	
	CJetImage(const IplImage *ImagePtr);	
	virtual ~CJetImage();
	//--------------------------------------------------------------------------------//
	CJetImage& operator=(const CJetImage &Image);
	CJetImage& operator=(const CvMat *ImagePtr);
	CJetImage& operator=(const IplImage *ImagePtr);
	//--------------------------------------------------------------------------------//
	void   ClearImage();
	bool   GetImage(IMAGE_SIZE &W, IMAGE_SIZE &H, IMAGE_SIZE &Step, IMAGE_SIZE &Bits, IMAGE_PTR &Ptr);
	bool   SetImage(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, IMAGE_SIZE Bits, const IMAGE_PTR Ptr, bool bClone);	
	bool   SetRGB(IMAGE_SIZE W, IMAGE_SIZE H, IMAGE_SIZE Step, const IMAGE_PTR PtrR, const IMAGE_PTR PtrG, const IMAGE_PTR PtrB);
	//--------------------------------------------------------------------------------//	
	bool   AlignImage(int Align);//重新對齊影像指標
	//--------------------------------------------------------------------------------//	
	IMAGE_PTR                  GetImagePtr() const;
	IMAGE_SIZE                 GetImageW() const;
	IMAGE_SIZE                 GetImageH() const;
	IMAGE_SIZE                 GetImageStep() const;
	IMAGE_SIZE                 GetBitCount() const;	
	bool                       CloneImage(CJetImage &Image) const;
	bool                       CloneImage(CvMat *&ImagePtr) const;
	bool                       CloneImage(IplImage *&ImagePtr) const;
	//--------------------------------------------------------------------------------//
	bool                       SaveImage(LPCTSTR pfilename);
	bool                       LoadImage(LPCTSTR pfilename);
	//--------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_JETIMAGE_H__1EE3820A_40F8_48EC_AD54_1AB230498740__INCLUDED_)
