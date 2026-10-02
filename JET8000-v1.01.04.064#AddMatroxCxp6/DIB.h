// DIB.h
//------------------------------------------------------------------------------//
#ifndef _DIB_H_
#define _DIB_H_
//------------------------------------------------------------------------------//
class CDib
{
//------------------------------------------------------------------------------//
public:
	//--------------------------------------------------------------------------//
	CDib();
	CDib(const CDib &dib);
	~CDib();
	//--------------------------------------------------------------------------//
	CDib &operator=(const CDib &dib);
	//--------------------------------------------------------------------------//
	unsigned char*             GetDIB();    // BMP data's headle
	unsigned char*             GetDIBBits();// BMP Bits data
	BITMAPINFO*                GetDIBInfo(); //取得BMP Infor
	//--------------------------------------------------------------------------//
	unsigned int               GetImageW();//取得影像寬度
	unsigned int               GetImageH();//取得影像長度
	unsigned int               GetImageBytePerLine();//取得影像每條長度
	unsigned int               GetImageBitCount();//取得影像位元數
	//--------------------------------------------------------------------------//
	CPalette*                  GetPalette();//取得調色盤	//成, 20080623
	BOOL                       Load(LPCTSTR);   // Load BMP file
	BOOL                       Save(LPCTSTR);   // save BMP file
	BOOL                       Draw( HDC &pDC, int nX = 0, int nY = 0, int nWidth = -1, int nHeight = -1 ); // Draw BMP image
	BOOL                       DrawPartion( HDC &pDC, int SrcnX, int SrcnY, int SrcnWidth, int SrcnHeight, int DestnX, int DestnY, int DestnWidth, int DestnHeight); // Draw BMP image
	BOOL                       SetPalette(CDC *);// set BMP palette
	void                       ReleaseBuffer();
	//------------------------------------------------------------------------------//
	bool                       SetImage(const unsigned char *pImageBits, unsigned int ImageW, unsigned int ImageH, unsigned int BytePerLine, unsigned int BitCounts, bool IsInverse);
	//------------------------------------------------------------------------------//
	bool                       DoFlip();
	bool                       DoRotate_180();
	//------------------------------------------------------------------------------//
protected:
	//------------------------------------------------------------------------------//
	void  CloneDib(const CDib &dib);
	void  BuildPalette();
	unsigned int  GetNBytesPerLine(const int ImageW);
	//------------------------------------------------------------------------------//
private:
	//--------------------------------------------------------------------------//
	CPalette m_Palette;        // BMP palette
	unsigned char *m_pDib, *m_pDibBits; // BMP fubber
	DWORD m_dwDibSize;         // Size of data
	BITMAPINFOHEADER *m_pBIH;  // Head of BMP
	RGBQUAD *m_pPalette;      
	int m_nPaletteEntries;
	//--------------------------------------------------------------------------//
};
//------------------------------------------------------------------------------//
#endif//_DIB_H_
