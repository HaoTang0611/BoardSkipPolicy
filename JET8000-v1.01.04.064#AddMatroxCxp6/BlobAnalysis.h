// BlobAnalysis.h: interface for the CBlobAnalysis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOBANALYSIS_H__16F9FA38_845B_43D6_8CCC_2698C9FB97EA__INCLUDED_)
#define AFX_BLOBANALYSIS_H__16F9FA38_845B_43D6_8CCC_2698C9FB97EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include<vector>

struct ROIInfo
{
	long ROI[4];      // 0,1 x-axis min./max. 2,3=y-axis min./max.
	long PixelNumber; // count number
	float GCenter[2];  // gravity center
	long Level;       // blob define number
};

struct RunData
{
   long OrgX,OrgY;  // coordinate of X and Y
   long Len;        // Length of the line
   long Level;      // blob define number
};

struct BlobPoint
{
   long X,Y;  // coordinate of X and Y
   
};

//-------------------------------------------------------------------------//

class CBlobAnalysis  
{
	struct LineFit
	{
		long Number;
		long X,Y,XX,XY;
		float a,b,c;
		long PointX,PointY;
	};

private:

    unsigned char ThresholdColorL;    // For threshold value
	unsigned char ThresholdColorH;    // For threshold value
	bool IsBMPFormat;                 // image format is BMP? or row data?
    bool IsFourConnect;               // 4-connect or 8-connect
	long ImageW,ImageH;               // Image height and Width
	long ImageOrgX,ImageOrgY,ImageOrgWidth,ImageOrgHeight,ImageRealEndX,ImageRealEndY; // active image information
	char ImageAddShift;               // For BMP format need shift some pixel
	unsigned char *ImageBuf;          // Image headle
	bool IsWhiteBlobAnalysis;         // White blob or black blob
	bool FindExtendRound(long Number); // record all outline of blob,but not continue connect
	bool IsRoundDataOK;               // Is round-data is got??
	
    bool ExchangeROI(long Number1,long Number2); // exchange ROI information
	bool CalLineFit(LineFit &Data);   // Line fit function

	// threshold method
	unsigned char T_Threshold(unsigned char *Image,long OrgX,long OrgY,long RealEndX,long RealEndY,long Width);
	unsigned char OTSU_Threshold(unsigned char *Image,long OrgX,long OrgY,long RealEndX,long RealEndY,long Width);
	
	// temp buffer
    RunData BlankRun;
	BlobPoint BlankRound;
	BlobPoint BlankRoundFitLine;

public:
	// Calculate Image
	bool CalPadNumber(long Width,long Height,unsigned char *Image,long OrgX=0,long OrgY=0,long OrgWidth=-1,long OrgHeight=-1); 
	bool SetThresholdL(unsigned char Threshold);    // Set Threshold value-L
	bool SetThresholdH(unsigned char Threshold);    // Set Threshold value-H
	bool SortFeature(long Type,bool IsDescending); // sort value by type
	bool ReSet();                                  // Re-set all blob information
	bool SetImageFormat(bool IsBMP);               // set image type, if type=BMP Y-axis data must change
	bool SetConnectType(bool Is4Conect);           // 4-connect or 8-connect
	long PadNumber;                                // blob number (1 ~ N)
	bool FindContinueRound(long Number);           // Find all round data and shape
	bool GetRoundFitLine(int MinDis,float ShiftDis); // Find Fit line of round data
	bool SetClass(bool IsWhiteBlob);               // set analysis blob class
	bool AutoThreshold(long Width,long Height,unsigned char *Image,unsigned char &Value,char Type=0,long OrgX=0,long OrgY=0,long OrgWidth=-1,long OrgHeight=-1);  // Autothreshold by method
	bool FitCircle(long Number,float &X,float &Y,float &STD,float &Radius);  // Fit a circle equation
	bool Erode(long Width,long Height,unsigned char *Image,int ErodeSize,char Type,long OrgX,long OrgY,long OrgWidth,long OrgHeight);  // Erode function
	bool Draw(CDC &pDC,long ShiftX=0,long ShiftY=0);  // Draw Image at DC

    ROIInfo *ROI_Data;                             // blob number data
	std::vector<RunData> LineData;                 // each line data
	std::vector<BlobPoint> ContinueRoundData;      // all round data of blob
	std::vector<BlobPoint> RoundDataFitLine;       // all round data of blob fit to line

	std::vector<BlobPoint> RoundData;              // all round data of blob

	CBlobAnalysis();
	virtual ~CBlobAnalysis();

};

#endif // !defined(AFX_BLOBANALYSIS_H__16F9FA38_845B_43D6_8CCC_2698C9FB97EA__INCLUDED_)
