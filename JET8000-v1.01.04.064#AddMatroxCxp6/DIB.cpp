// DIB.cpp
//------------------------------------------------------------------------------//
#include "stdafx.h"
#include "DIB.h"
//------------------------------------------------------------------------------//
CDib::CDib()
{
	// Set the Dib pointer to
	// NULL so we know if it's
	// been loaded.
	this->m_pDib = NULL;
	this->m_pBIH = NULL;
	this->m_pDibBits = NULL;	
	this->m_dwDibSize = 0;
}
//------------------------------------------------------------------------------//
CDib::CDib(const CDib &dib)
{
	this->m_pDib = NULL;
	this->m_pBIH = NULL;
	this->m_pDibBits = NULL;	
	this->m_dwDibSize = 0;
	this->CloneDib(dib);
}
//------------------------------------------------------------------------------//
CDib::~CDib()
{

	// If a Dib has been loaded,
	// delete the memory.
	this->ReleaseBuffer();
}
//------------------------------------------------------------------------------//
CDib& CDib::operator=(const CDib &dib)
{
	if ( this == &dib ) { return *this; }
	this->CloneDib(dib);
	return *this;
}
//------------------------------------------------------------------------------//
void CDib::CloneDib(const CDib &dib)
{
	const char fnName[] = ("CDib::CloneDib");
	this->ReleaseBuffer();	
	
	this->m_dwDibSize = dib.m_dwDibSize;
	if ( this->m_dwDibSize == 0 ) { return; }

	if ( JetMemory.alloc_func(m_dwDibSize, m_pDib, fnName, "m_pDib") == false )	
	{
		this->m_dwDibSize = 0;
		return ; 
	}	
	::memcpy(this->m_pDib, dib.m_pDib, sizeof(unsigned char)*m_dwDibSize);

	this->m_nPaletteEntries = dib.m_nPaletteEntries;
	m_pBIH = (BITMAPINFOHEADER *) m_pDib;	
	m_pPalette = (RGBQUAD *) &m_pDib[sizeof(BITMAPINFOHEADER)];
	m_pDibBits = &m_pDib[sizeof(BITMAPINFOHEADER)+ m_nPaletteEntries*sizeof(RGBQUAD)];	
	this->BuildPalette();
}
//------------------------------------------------------------------------------//
void CDib::ReleaseBuffer()
{
	if ( m_pDib != NULL )
	{
		JetMemory.free_func(m_pDib);	//chia032
		m_pDib = NULL;
	}	
	this->m_pBIH = NULL;//Kai
	this->m_pDibBits = NULL;
	this->m_pPalette = NULL;
	this->m_dwDibSize = 0;
}
//------------------------------------------------------------------------------//
BOOL CDib::Load(LPCTSTR pszFilename )
{
	this->ReleaseBuffer();
	const char fnName[] = ("CDib::Load");
	CFile cf;

	// Attempt to open the Dib file for reading.
	if( !cf.Open( pszFilename, CFile::modeRead ) )
	{
		return( FALSE );
	}

	// Get the size of the file and store
	// in a local variable. Subtract the
	// size of the BITMAPFILEHEADER structure
	// since we won't keep that in memory.
	DWORD dwDibSize;
	dwDibSize = DWORD( cf.GetLength()-sizeof(BITMAPFILEHEADER) );

	// Attempt to allocate the Dib memory.
	if ( JetMemory.alloc_func(dwDibSize, m_pDib, fnName, "m_pDib") == false )	
	{
		::AfxMessageBox(CString(JetMemory.GetErrorString()));
		return( FALSE );
	}

	BITMAPFILEHEADER BFH;
	// Read in the Dib header and data.
	try 
	{
		// Did we read in the entire BITMAPFILEHEADER?
		if( cf.Read( &BFH, sizeof( BITMAPFILEHEADER ) )
			!= sizeof( BITMAPFILEHEADER ) ||

			// Is the type 'MB'?
			BFH.bfType != 'MB' ||

			// Did we read in the remaining data?
			cf.Read( m_pDib, dwDibSize ) != dwDibSize )
		{

			// Delete the memory if we had any
			// errors and return FALSE.
			JetMemory.free_func(m_pDib);
			cf.Close();
			return( FALSE );
		}
	}

	// If we catch an exception, delete the
	// exception, the temporary Dib memory,
	// and return FALSE.
	catch( CFileException *e )
	{
		e->Delete();		
		JetMemory.free_func(m_pDib);
		cf.Close();
		return( FALSE );
	}
	
	// If we got to this point, the Dib has been
	// loaded. If a Dib was already loaded into
	// this class, we must now delete it.

	// Store the local Dib data pointer and
	// Dib size variables in the class member
	// variables.
	m_dwDibSize = dwDibSize;

	// Pointer our BITMAPINFOHEADER and RGBQUAD
	// variables to the correct place in the Dib data.
	m_pBIH = (BITMAPINFOHEADER *) m_pDib;
	m_pPalette = (RGBQUAD *) &m_pDib[sizeof(BITMAPINFOHEADER)];

	// Calculate the number of palette entries.
	m_nPaletteEntries = 1 << m_pBIH->biBitCount;
	if( m_pBIH->biBitCount > 8 )
	{
		m_nPaletteEntries = 0;
	}
	else if( m_pBIH->biClrUsed != 0 )
	{
		m_nPaletteEntries = m_pBIH->biClrUsed;
	}

	// Point m_pDibBits to the actual Dib bits data.
	m_pDibBits = &m_pDib[sizeof(BITMAPINFOHEADER)+m_nPaletteEntries*sizeof(RGBQUAD)];

	this->BuildPalette();
	
	cf.Close();
	return( TRUE );
}
//------------------------------------------------------------------------------//
void CDib::BuildPalette()
{
	// If we have a valid palette, delete it.
	if( m_Palette.GetSafeHandle() != NULL )
	{
		m_Palette.DeleteObject();
	}

	// If there are palette entries, we'll need
	// to create a LOGPALETTE then create the
	// CPalette palette.
	if( m_nPaletteEntries == 0 ) { return; }
	
	// Allocate the LOGPALETTE structure.
	LOGPALETTE *pLogPal = (LOGPALETTE *) new char [sizeof(LOGPALETTE)+m_nPaletteEntries*sizeof(PALETTEENTRY)];
	if ( pLogPal == NULL ) { return; }

	// Set the LOGPALETTE to version 0x300
	// and store the number of palette
	// entries.
	pLogPal->palVersion = 0x300;
	pLogPal->palNumEntries = m_nPaletteEntries;

	// Store the RGB values into each
	// PALETTEENTRY element.
	for( int i=0; i<m_nPaletteEntries; i++ )
	{
		pLogPal->palPalEntry[i].peRed   = 	m_pPalette[i].rgbRed;
		pLogPal->palPalEntry[i].peGreen =	m_pPalette[i].rgbGreen;
		pLogPal->palPalEntry[i].peBlue  =	m_pPalette[i].rgbBlue;
	}

	// Create the CPalette object and
	// delete the LOGPALETTE memory.
	m_Palette.CreatePalette( pLogPal );
	delete [] pLogPal;
}
//------------------------------------------------------------------------------//
BOOL CDib::Save(LPCTSTR pszFilename )
{
	// If we have no data, we can't save.
	if( m_pDib == NULL )
	{
		return( FALSE );
	}

	CFile cf;

	// Attempt to create the file.
	if( !cf.Open( pszFilename, CFile::modeCreate | CFile::modeWrite ) )
	{
		return( FALSE );
	}
	
	// Write the data.
	try
	{
		// First, create a BITMAPFILEHEADER
		// with the correct data.
		BITMAPFILEHEADER BFH;
		memset( &BFH, 0, sizeof( BITMAPFILEHEADER ) );
		BFH.bfType = 'MB';
		BFH.bfSize = sizeof( BITMAPFILEHEADER ) + m_dwDibSize;
		BFH.bfOffBits = sizeof( BITMAPFILEHEADER ) +
			sizeof( BITMAPINFOHEADER ) +
			m_nPaletteEntries * sizeof( RGBQUAD );

		// Write the BITMAPFILEHEADER and the
		// Dib data.
		cf.Write( &BFH, sizeof( BITMAPFILEHEADER ) );
		cf.Write( m_pDib, m_dwDibSize );
	}

	// If we get an exception, delete the exception and
	// return FALSE.
	catch( CFileException *e )
	{
		e->Delete();
		return( FALSE );
	}
	return( TRUE );
}
//------------------------------------------------------------------------------//
BOOL CDib::DrawPartion( HDC &pDC, int SrcnX, int SrcnY, int SrcnWidth, int SrcnHeight, int DestnX = 0, int DestnY = 0, int DestnWidth = -1, int DestnHeight = -1 ) // Draw BMP image
{
	// If we have not data we can't draw.
	if( m_pDib == NULL )
		return( FALSE );

	// Check for the default values of -1
	// in the width and height arguments. If
	// we find -1 in either, we'll set them
	// to the value that's in the BITMAPINFOHEADER.
	if( SrcnWidth == -1 )	{ return FALSE; }
	if( SrcnHeight == -1 )	{ return FALSE; }
	
	// Use StretchDIBits to draw the Dib.
	StretchDIBits( pDC, DestnX, DestnY,
		DestnWidth, DestnHeight,
		SrcnX, SrcnY,
		SrcnWidth, SrcnHeight,
		m_pDibBits,
		(BITMAPINFO *) m_pBIH,
		BI_RGB, SRCCOPY );
	
	return( TRUE );

}
//------------------------------------------------------------------------------//
BOOL CDib::Draw( HDC &pDC, int nX, int nY, int nWidth, int nHeight )
{

	// If we have not data we can't draw.
	if( m_pDib == NULL )
		return( FALSE );

	// Check for the default values of -1
	// in the width and height arguments. If
	// we find -1 in either, we'll set them
	// to the value that's in the BITMAPINFOHEADER.
	if( nWidth == -1 )
		nWidth = m_pBIH->biWidth;
	if( nHeight == -1 )
		nHeight = m_pBIH->biHeight;
	
	// Use StretchDIBits to draw the Dib.
	StretchDIBits( pDC, nX, nY,
		nWidth, nHeight,
		0, 0,
		m_pBIH->biWidth, m_pBIH->biHeight,
		m_pDibBits,
		(BITMAPINFO *) m_pBIH,
		BI_RGB, SRCCOPY );
	
	return( TRUE );

}
//------------------------------------------------------------------------------//
BOOL CDib::SetPalette( CDC *pDC )
{
	// If we have not data we
	// won't want to set the palette.
	if( m_pDib == NULL )
	{
		return( FALSE );
	}

	// Check to see if we have a palette
	// handle. For Dibs greater than 8 bits,
	// this will be NULL.
	if( m_Palette.GetSafeHandle() == NULL )
	{
		return( TRUE );
	}

	// Select the palette, realize the palette,
	// then finally restore the old palette.
	CPalette *pOldPalette;
	pOldPalette = pDC->SelectPalette( &m_Palette, FALSE );
	pDC->RealizePalette();
	pDC->SelectPalette( pOldPalette, FALSE );

	return( TRUE );
}
//------------------------------------------------------------------------------//
unsigned char * CDib::GetDIB()
{
	return m_pDib;
}
//------------------------------------------------------------------------------//
unsigned char * CDib::GetDIBBits()// BMP Bits data
{
	if ( m_pDib == NULL ) { return NULL; }
	return m_pDibBits;
}
//------------------------------------------------------------------------------//
BITMAPINFO * CDib::GetDIBInfo() //取得BMP Infor
{
	if ( m_pDib == NULL ) { return NULL; }
	return (BITMAPINFO*)this->m_pBIH;
}
//------------------------------------------------------------------------------//
unsigned int CDib::GetImageW()//取得影像寬度
{
	BITMAPINFO *pInfo = this->GetDIBInfo();
	if ( NULL == pInfo ) { return 0; }
	return (unsigned int)(pInfo->bmiHeader.biWidth);
}
//------------------------------------------------------------------------------//
unsigned int CDib::GetImageH()//取得影像長度
{
	BITMAPINFO *pInfo = this->GetDIBInfo();
	if ( NULL == pInfo ) { return 0; }
	return (unsigned int)(pInfo->bmiHeader.biHeight);
}
//------------------------------------------------------------------------------//
unsigned int CDib::GetImageBytePerLine()//取得影像每條長度
{
	BITMAPINFO *pInfo = this->GetDIBInfo();
	if ( NULL == pInfo ) { return 0; }

	unsigned int BytePerLine = 0;
	switch ( pInfo->bmiHeader.biBitCount )
	{
	case 32:
		BytePerLine = pInfo->bmiHeader.biWidth*4;
		break;
	case 24:
		BytePerLine = GetNBytesPerLine(pInfo->bmiHeader.biWidth*3);
		break;
	default:
		BytePerLine = GetNBytesPerLine(pInfo->bmiHeader.biWidth);
		break;
	}
	return BytePerLine;
}
//------------------------------------------------------------------------------//
unsigned int CDib::GetImageBitCount()//取得影像位元數
{
	BITMAPINFO *pInfo = this->GetDIBInfo();
	if ( NULL == pInfo ) { return 0; }
	return (unsigned int)(pInfo->bmiHeader.biBitCount);
}
//------------------------------------------------------------------------------//
CPalette* CDib::GetPalette()		//取得調色盤	//成, 20080623
{
	if ( m_pDib == NULL ) { return NULL; }
	return &this->m_Palette;
}
//------------------------------------------------------------------------------//
bool CDib::SetImage(const unsigned char *pImageBits, unsigned int ImageW, unsigned int ImageH, unsigned int BytePerLine, unsigned int BitCounts, bool IsInverse)
{
	const char fnName[] = ("CDib::SetImage");
	this->ReleaseBuffer();
	IMAGE_SIZE nChannel = 1;
	IMAGE_SIZE Offset = 0;
	switch ( BitCounts )
	{
	case 8:	
		m_nPaletteEntries = 256;
		nChannel = 1;	
		Offset = sizeof(BITMAPINFOHEADER)+256*sizeof(RGBQUAD);
		break;
	case 24:	
		nChannel = 3;	
		m_nPaletteEntries = 0;
		Offset = sizeof(BITMAPINFOHEADER);
		break;
	default:
		return false;
	}

	const unsigned int ImageW2 = this->GetNBytesPerLine(ImageW*nChannel);
	const unsigned int dwSize = (ImageW2*ImageH*nChannel)+Offset;
	if ( JetMemory.alloc_func(dwSize, this->m_pDib, fnName, "m_pDib") == false )
	{	return false; }	
	
	m_dwDibSize   = dwSize;

	m_pBIH = (BITMAPINFOHEADER *) m_pDib;
	m_pBIH->biSize = sizeof(BITMAPINFOHEADER);
	m_pBIH->biWidth = ImageW;
	m_pBIH->biHeight = ImageH;
	m_pBIH->biPlanes = 1;
	m_pBIH->biBitCount = BitCounts;
	m_pBIH->biCompression = 0;
	m_pBIH->biSizeImage = ImageW*ImageH*nChannel;

	m_pBIH->biXPelsPerMeter = 20;
	m_pBIH->biYPelsPerMeter = 20;
	m_pBIH->biClrUsed = 0;
	m_pBIH->biClrImportant = 0;

	m_pPalette = (RGBQUAD *) &m_pDib[sizeof(BITMAPINFOHEADER)];
	m_pDibBits = &m_pDib[sizeof(BITMAPINFOHEADER)+ m_nPaletteEntries*sizeof(RGBQUAD)];


	size_t i=0, j=0;
	size_t s=0, t=0;
	size_t u=0, v=0;
	const size_t CopyLen = sizeof(unsigned char)*ImageW*nChannel;
	for ( j=0; j<ImageH; j++ )
	{
		s = j*BytePerLine;
		if ( IsInverse == false )
		{	u = j*ImageW2; }
		else
		{	u = (ImageH-j-1)*ImageW2; }

		if ( nChannel == 3 )
		{	::memcpy(&(m_pDibBits[u]), &(pImageBits[s]), CopyLen);	}
		else
		{	::memcpy(&(m_pDibBits[u]), &(pImageBits[s]), CopyLen);	}
	}

	for ( i=0; i<m_nPaletteEntries; i++ )
	{
		m_pPalette[i].rgbRed      = (BYTE)i;
		m_pPalette[i].rgbGreen    = (BYTE)i;
		m_pPalette[i].rgbBlue     = (BYTE)i;
		m_pPalette[i].rgbReserved = 0;
	}
	this->BuildPalette();
	return true;
}
//------------------------------------------------------------------------------//
unsigned int  CDib::GetNBytesPerLine(const int ImageW)
{
	unsigned int bytes_per_line = ( ImageW * 8 + 7)/8;
    bytes_per_line = ( bytes_per_line + 3 ) / 4;
    bytes_per_line = bytes_per_line*4;
	return bytes_per_line;
}
//-------------------------------------------------------------------//
bool CDib::DoFlip()
{
	const char fnName[] = ("CDib::DoFlip");
	BITMAPINFO *pInfo = this->GetDIBInfo();
	if ( pInfo == NULL ) { return false; }

	unsigned char *pDibBits = this->GetDIBBits();
	if ( pDibBits == NULL ) { return false; }
	if ( m_dwDibSize <= 0 ) { return false; }

	size_t i=0, idx1=0, idx2=0;
	const unsigned int ImageW = pInfo->bmiHeader.biWidth;
	const unsigned int ImageH = pInfo->bmiHeader.biHeight;
	const unsigned int BitCounts = pInfo->bmiHeader.biBitCount;
	unsigned int Strip = 1;
	if ( BitCounts == 24 )
	{	Strip = 3; }

	const unsigned int BytesPerLine = this->GetNBytesPerLine(ImageW*Strip);
	const unsigned int TotalSize = BytesPerLine*ImageH;
	const unsigned int CopyLen = sizeof(unsigned char)*BytesPerLine;

	unsigned char *pImage = NULL;
	if ( JetMemory.alloc_func(TotalSize, pImage, fnName, "pImage") == false )
	{	return false; }

	for ( i=0; i<ImageH; i++ )
	{
		idx1 = i*BytesPerLine;
		idx2 = (ImageH-i-1)*BytesPerLine;
		::memcpy(&(pImage[idx1]), &(pDibBits[idx2]), CopyLen);
	}
	::memcpy(&(pDibBits[0]), &(pImage[0]), sizeof(unsigned char)*TotalSize);
	JetMemory.free_func(pImage);
	return true;
}
//-------------------------------------------------------------------//
bool CDib::DoRotate_180()
{
	const char fnName[] = ("CDib::DoRotate_180");
	BITMAPINFO *pInfo = this->GetDIBInfo();
	if ( pInfo == NULL ) { return false; }

	unsigned char *pDibBits = this->GetDIBBits();
	if ( pDibBits == NULL ) { return false; }
	if ( m_dwDibSize <= 0 ) { return false; }

	unsigned int i=0, j=0;
	unsigned int idx1=0, idx2=0;
	const unsigned int ImageW = pInfo->bmiHeader.biWidth;
	const unsigned int ImageH = pInfo->bmiHeader.biHeight;
	const unsigned int BitCounts = pInfo->bmiHeader.biBitCount;
	unsigned int Strip = 1;
	if ( BitCounts == 24 )
	{	Strip = 3; }

	const unsigned int BytesPerLine = this->GetNBytesPerLine(ImageW*Strip);
	const unsigned int TotalSize = BytesPerLine*ImageH;
	const unsigned int CopyLen = sizeof(unsigned char)*BytesPerLine;

	unsigned char *pImage = NULL;
	if ( JetMemory.alloc_func(TotalSize, pImage, fnName, "pImage") == false )
	{	return false; }

	if ( 24 == BitCounts )
	{
		for ( i=0; i<ImageH; i++ )
		{
			for ( j=0; j<ImageW; j++ )
			{
				idx1 = i*BytesPerLine+(j*3);
				idx2 = (ImageH-i-1)*BytesPerLine+((ImageW-j-1)*3);
				pImage[idx1] = pDibBits[idx2];
				pImage[idx1+1] = pDibBits[idx2+1];
				pImage[idx1+2] = pDibBits[idx2+2];
			}
		}
	}
	else
	{
		for ( i=0; i<ImageH; i++ )
		{
			for ( j=0; j<ImageW; j++ )
			{
				idx1 = i*BytesPerLine+(j);
				idx2 = (ImageH-i-1)*BytesPerLine+((ImageW-j-1));
				pImage[idx1] = pDibBits[idx2];				
			}
		}
	}
	::memcpy(&(pDibBits[0]), &(pImage[0]), sizeof(unsigned char)*TotalSize);
	JetMemory.free_func(pImage);
	return true;
}
//-------------------------------------------------------------------//