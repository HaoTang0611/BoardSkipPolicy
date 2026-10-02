// BlobAnalysis.cpp: implementation of the CBlobAnalysis class.
//
//////////////////////////////////////////////////////////////////////

//   Author:  Casper      CopyRight 2005.5.3
//   Version 1.00


#include "stdafx.h"
#include "BlobAnalysis.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBlobAnalysis::CBlobAnalysis()
{
    ThresholdColorL=125;
	ThresholdColorH=255;

	ROI_Data=NULL;

	BlankRun.Len=0;
	BlankRun.Level=-1;
	BlankRun.OrgX=0;
	BlankRun.OrgY=0;

	BlankRound.X=0; 
	BlankRound.Y=0;

	BlankRoundFitLine.X=0; 
	BlankRoundFitLine.Y=0; 
	
	IsBMPFormat=false;
	IsFourConnect=false;
	IsWhiteBlobAnalysis=true;

	ImageW=ImageH=0;
	ImageOrgX=ImageOrgY=ImageOrgWidth=ImageOrgHeight=ImageRealEndX=ImageRealEndY=0;
	ImageAddShift=0;

	ReSet();
	
}

CBlobAnalysis::~CBlobAnalysis()
{
   ReSet();
}

bool CBlobAnalysis::SetImageFormat(bool IsBMP)
{
	IsBMPFormat=IsBMP;
	return true;
}

bool CBlobAnalysis::SetConnectType(bool Is4Conect)
{
	IsFourConnect=Is4Conect;
	return true;
}

bool CBlobAnalysis::ReSet()
{
	PadNumber=0;
	ImageBuf=NULL;
	if(LineData.empty() == false) LineData.clear();
	if(ContinueRoundData.empty() == false) ContinueRoundData.clear(); 
	if(RoundDataFitLine.empty() == false) RoundDataFitLine.clear();
	if(RoundData.empty() == false) RoundData.clear();
	
	delete[] ROI_Data; ROI_Data=NULL;

	IsRoundDataOK=false;
	return true;
}

bool CBlobAnalysis::SetThresholdL(unsigned char Threshold)
{
	ThresholdColorL=Threshold;
	return true;
}

bool CBlobAnalysis::SetThresholdH(unsigned char Threshold)
{
	ThresholdColorH=Threshold;
	return true;
}

bool CBlobAnalysis::SetClass(bool IsWhiteBlob)
{
	IsWhiteBlobAnalysis=IsWhiteBlob;
	return true;
}


bool CBlobAnalysis::CalPadNumber(long Width,long Height,unsigned char *Image,long OrgX,long OrgY,long OrgWidth,long OrgHeight)
{
	ReSet();

	if(Image==NULL) return false;

	ImageBuf=Image;
	
	ImageW=Width;
	ImageH=Height;
	
	register long Number=0;  // cal. line number
	register long i,j,k,index;
	register long SetNumber,CheckNumber;
	register unsigned char PadGrayL=ThresholdColorL;
	register unsigned char PadGrayH=ThresholdColorH;
	register bool IsFirst; 
	register char Connect;
	register long RealEndX,RealEndY;	

	char AddShift;  // for BMP rule

	if(IsFourConnect==false) Connect=1;
	else Connect=0;
    
	if(IsBMPFormat==true)
	{ 
	  AddShift=(char) (((int) (Width/4)+1)*4-Width);
	  if(AddShift==4) AddShift=0; 

	  // modify X-axis parameter
      if(OrgWidth==-1) 
	  { RealEndX=Width;
	    OrgX=0;
	  }
	  else
	  {	 if( (OrgX+OrgWidth)>Width) OrgWidth=Width-OrgX;
		 RealEndX=OrgWidth+OrgX; 
	  }

	  // modify Y-axis parameter
      if(OrgHeight==-1)
	  { RealEndY=Height;
	    OrgY=0;
	  }
      else
	  { 
		if( (OrgY+OrgHeight)>Height) OrgHeight=Height-OrgY;

		OrgY=Height-(OrgHeight+OrgY);
	    RealEndY=OrgHeight+OrgY;
	    
	  }
	}
	else  // Pure data
	{ 
	  AddShift=0;
	  // modify X-axis parameter 
	  if(OrgWidth==-1) 
	  { RealEndX=Width;
	    OrgX=0;
	  }
	  else
	  {	 if( (OrgX+OrgWidth)>Width) OrgWidth=Width-OrgX;
		 RealEndX=OrgWidth+OrgX; 
	  }

      // modify Y-axis parameter
	  if(OrgHeight==-1)
	  { RealEndY=Height;
	    OrgY=0;
	  }
      else
	  { if( (OrgY+OrgHeight)>Height) OrgHeight=Height-OrgY;
		RealEndY=OrgHeight+OrgY;
	  }

	}

	// For after image process
    ImageOrgHeight=OrgHeight;
	ImageOrgWidth=OrgWidth;
	ImageOrgX=OrgX;
	ImageOrgY=OrgY;
	ImageRealEndX=RealEndX;
	ImageRealEndY=RealEndY;
	ImageAddShift=AddShift;
	
	IsFirst=true;
	BlankRun.Level=-1; 

	if(IsWhiteBlobAnalysis==true)  
	{
	  for(j=OrgY;j<RealEndY;j++)
	  {
		index=OrgX+j*(Width+AddShift);

		for(i=OrgX;i<RealEndX;i++)
		{
			if( (Image[index]<PadGrayL) || (Image[index]>PadGrayH) )  // threshold value
			{
				if(IsFirst==false) 
				{ IsFirst=true; 
				  BlankRun.Len=i-BlankRun.OrgX;
				  BlankRun.OrgY=j;
				  LineData.push_back(BlankRun);
				  Number++;
				}
			}
			else
			{
				if(IsFirst==true) 
				{   BlankRun.OrgX=i; 
					IsFirst=false; 
				}
			}

			/*
			if(Image[index]>=PadGray)  // threshold value
			{
				if(IsFirst==true) 
				{   BlankRun.OrgX=i; 
					IsFirst=false; 
				}
			}
			else
			{
				if(IsFirst==false) 
				{ IsFirst=true; 
				  BlankRun.Len=i-BlankRun.OrgX;
				  BlankRun.OrgY=j;
				  LineData.push_back(BlankRun);
				  Number++;
				}
			}
			*/

            index++;
		}

        if(IsFirst==false)
		{ IsFirst=true; 
		  BlankRun.Len=i-BlankRun.OrgX;
		  BlankRun.OrgY=j;
		  LineData.push_back(BlankRun);
		  Number++;
		}
	  }
	}
	else // for speed up
	{
	  for(j=OrgY;j<RealEndY;j++)
	  {
		index=OrgX+j*(Width+AddShift);

		for(i=OrgX;i<RealEndX;i++)
		{
			if( (Image[index]<PadGrayL) || (Image[index]>PadGrayH))  // threshold value
			{
				if(IsFirst==true) 
				{   BlankRun.OrgX=i; 
					IsFirst=false; 
				}
			}
			else
			{
				if(IsFirst==false) 
				{ IsFirst=true; 
				  BlankRun.Len=i-BlankRun.OrgX;
				  BlankRun.OrgY=j;
				  LineData.push_back(BlankRun);
				  Number++;
				}
			}

			/*
			if(Image[index]<=PadGray)  // threshold value
			{
				if(IsFirst==true) 
				{   BlankRun.OrgX=i; 
					IsFirst=false; 
				}
			}
			else
			{
				if(IsFirst==false) 
				{ IsFirst=true; 
				  BlankRun.Len=i-BlankRun.OrgX;
				  BlankRun.OrgY=j;
				  LineData.push_back(BlankRun);
				  Number++;
				}
			}
			*/

            index++;
		}

        if(IsFirst==false)
		{ IsFirst=true; 
		  BlankRun.Len=i-BlankRun.OrgX;
		  BlankRun.OrgY=j;
		  LineData.push_back(BlankRun);
		  Number++;
		}
	  }
	}
	
	// analysis each line data
	if(Number==0)
	{
		PadNumber=0;
		return true;
	}	

    LineData[0].Level=0; 
	index=1;
    IsFirst=false;

    for(i=1;i<Number;i++)
	{
		for(j=i-1;j>=0;j--)
		{
			if(LineData[j].OrgY==LineData[i].OrgY) continue;
			if(LineData[j].OrgY<LineData[i].OrgY-1) break;  // if OrgY > 1 pixel

			if( LineData[j].OrgX<LineData[i].OrgX+LineData[i].Len+Connect && 
				LineData[j].OrgX+LineData[j].Len+Connect>LineData[i].OrgX)
			{
				
				if(IsFirst==true) // if it connect more than 2 line data
				{
				  if(LineData[j].Level == LineData[i].Level) continue;

				  if(LineData[j].Level > LineData[i].Level)	
				  {
					CheckNumber=LineData[j].Level;
					SetNumber=LineData[i].Level;
				  }
				  else
				  {
					CheckNumber=LineData[i].Level;
					SetNumber=LineData[j].Level;
				  }

					for(k=i;k>=0;k--)
					{
 					  if(LineData[k].Level==CheckNumber) LineData[k].Level=SetNumber;
					}

					// delete last number 
                    if(index>1)
					{ 
					  index--;

					  if(index!=CheckNumber)
					  {
						for(k=i;k>=0;k--)
						{
						   if(LineData[k].Level==index) LineData[k].Level=CheckNumber;
						}
					  }
					}
				}
				else
				{ IsFirst=true;
				  LineData[i].Level=LineData[j].Level; 

				}
			}

		}

		if(IsFirst==false)
		{
			LineData[i].Level=index;
			index++;
		}
		else IsFirst=false;
	}

	// create result data
	PadNumber=index;
	ROI_Data=new ROIInfo[PadNumber];
	ROIInfo TempROI;

	TempROI.ROI[0]=TempROI.ROI[2]=100000;
	TempROI.ROI[1]=TempROI.ROI[3]=-100000;
	TempROI.PixelNumber=0;
	TempROI.Level=0; 
	TempROI.GCenter[0]=TempROI.GCenter[1]=0.0f;
	j=sizeof(ROIInfo);

	for(i=0;i<index;i++)
	{
		memcpy(&ROI_Data[i],&TempROI,j);
		ROI_Data[i].Level=i; 
	}

	// analysis ROI range
	for(i=0;i<Number;i++)
	{
	  SetNumber=LineData[i].Level;
	  
	  if(LineData[i].OrgX<ROI_Data[SetNumber].ROI[0])  ROI_Data[SetNumber].ROI[0]=LineData[i].OrgX;
	  CheckNumber=LineData[i].OrgX+LineData[i].Len-1;
	  if(CheckNumber>ROI_Data[SetNumber].ROI[1])  ROI_Data[SetNumber].ROI[1]=CheckNumber;

	  if(LineData[i].OrgY<ROI_Data[SetNumber].ROI[2])  ROI_Data[SetNumber].ROI[2]=LineData[i].OrgY;
	  if(LineData[i].OrgY>ROI_Data[SetNumber].ROI[3])  ROI_Data[SetNumber].ROI[3]=LineData[i].OrgY;

	  ROI_Data[SetNumber].PixelNumber+=LineData[i].Len;
	  
	  //calculate center of gravity
      ROI_Data[SetNumber].GCenter[0]=ROI_Data[SetNumber].GCenter[0]+((float) LineData[i].OrgX+(float)LineData[i].Len/2)*LineData[i].Len;   
	  ROI_Data[SetNumber].GCenter[1]=ROI_Data[SetNumber].GCenter[1]+LineData[i].OrgY*LineData[i].Len;

	}

	for(i=0;i<index;i++)
	{
      ROI_Data[i].GCenter[0]=ROI_Data[i].GCenter[0]/ROI_Data[i].PixelNumber+0.50f;  // for first pixel is 0.5
	  ROI_Data[i].GCenter[1]=ROI_Data[i].GCenter[1]/ROI_Data[i].PixelNumber+0.50f;  // for first pixel is 0.5
	}
	

	if(IsBMPFormat==true)
	{
		//For BMP rule, change Y-axis coordiante

		j=Height-1;

		for(i=0;i<index;i++)
		{
		   k=j-ROI_Data[i].ROI[2];
		   ROI_Data[i].ROI[2]=j-ROI_Data[i].ROI[3];
		   ROI_Data[i].ROI[3]=k;

		   ROI_Data[i].GCenter[1]=j-ROI_Data[i].GCenter[1];
		}

        for(i=0;i<Number;i++) LineData[i].OrgY=j-LineData[i].OrgY;
		
	}

	return true;
}

bool CBlobAnalysis::ExchangeROI(long Number1,long Number2)
{
	if(Number1==Number2) return true;
	if(Number1>=PadNumber || Number2>=PadNumber) return false;

	long Size=sizeof(ROIInfo);
	ROIInfo temp;
	
	memcpy(&temp,&ROI_Data[Number2],Size);
	memcpy(&ROI_Data[Number2],&ROI_Data[Number1],Size);
	memcpy(&ROI_Data[Number1],&temp,Size);
	
	return true;
}

// Type = 0, Area
bool CBlobAnalysis::SortFeature(long Type,bool IsDescending)
{
	if(ROI_Data==NULL) return true;

	register long i,j,Number,ChangeNumber,TempNumber;
	register long Temp_long;
	
	Number=PadNumber;

	if(IsDescending==true)
	{
		switch(Type)
		{
		case 0://面積
			TempNumber=Number-1;
			for(i=0;i<TempNumber;i++)
			{
				Temp_long=ROI_Data[i].PixelNumber; 
				ChangeNumber=i;
				for(j=i+1;j<Number;j++)
				{
					if(ROI_Data[j].PixelNumber>Temp_long)
					{
						Temp_long=ROI_Data[j].PixelNumber; 
						ChangeNumber=j;
					}
				}
				ExchangeROI(i,ChangeNumber);
			}
			break;
		case 1://X
			TempNumber=Number-1;
			for(i=0;i<TempNumber;i++)
			{
				Temp_long=(long)(ROI_Data[i].GCenter[0]); 
				ChangeNumber=i;
				for(j=i+1;j<Number;j++)
				{
					if(ROI_Data[j].GCenter[0]>Temp_long)
					{
						Temp_long=(long)(ROI_Data[j].GCenter[0]); 
						ChangeNumber=j;
					}
				}
				ExchangeROI(i,ChangeNumber);
			}
			break;
		case 2://Y
			TempNumber=Number-1;
			for(i=0;i<TempNumber;i++)
			{
				Temp_long=(long)(ROI_Data[i].GCenter[1]); 
				ChangeNumber=i;
				for(j=i+1;j<Number;j++)
				{
					if(ROI_Data[j].GCenter[1]>Temp_long)
					{
						Temp_long=(long)(ROI_Data[j].GCenter[1]); 
						ChangeNumber=j;
					}
				}
				ExchangeROI(i,ChangeNumber);
			}
			break;
		default: return false; break;
		}
	}		
	else
	{
        switch(Type)
		{
		case 0://面積
			TempNumber=Number-1;
			for(i=0;i<TempNumber;i++)
			{
				Temp_long=ROI_Data[i].PixelNumber; 
				ChangeNumber=i;
				for(j=i+1;j<Number;j++)
				{
					if(ROI_Data[j].PixelNumber<Temp_long)
					{
						Temp_long=ROI_Data[j].PixelNumber; 
						ChangeNumber=j;
					}
				}
				ExchangeROI(i,ChangeNumber);
			}
			break;
		case 1://X
			TempNumber=Number-1;
			for(i=0;i<TempNumber;i++)
			{
				Temp_long=(long)(ROI_Data[i].GCenter[0]);
				ChangeNumber=i;
				for(j=i+1;j<Number;j++)
				{
					if(ROI_Data[j].GCenter[0]<Temp_long)
					{
						Temp_long=(long)(ROI_Data[j].GCenter[0]); 
						ChangeNumber=j;
					}
				}
				ExchangeROI(i,ChangeNumber);
			}
			break;
		case 2://X
			TempNumber=Number-1;
			for(i=0;i<TempNumber;i++)
			{
				Temp_long=(long)(ROI_Data[i].GCenter[1]); 
				ChangeNumber=i;
				for(j=i+1;j<Number;j++)
				{
					if(ROI_Data[j].GCenter[1]<Temp_long)
					{
						Temp_long=(long)(ROI_Data[j].GCenter[1]); 
						ChangeNumber=j;
					}
				}
				ExchangeROI(i,ChangeNumber);
			}
			break;
		default: return false; break;
		}
	}		
	return true;
}

bool CBlobAnalysis::FindContinueRound(long Number)
{
	IsRoundDataOK=false; 

	ContinueRoundData.clear();

	if(Number>=PadNumber) return false;

	register long i,j,SetNumber,CheckPx,CheckPy;
	register long X,Y,index,AddShift,MaxNumber;
	register char Dir,PreDir;
	int BackNumber=0;

	for(i=0;i<PadNumber;i++)
	{
      if(ROI_Data[i].Level==Number) 
	  {
		  MaxNumber=((ROI_Data[i].ROI[1]-ROI_Data[i].ROI[0])+(ROI_Data[i].ROI[3]-ROI_Data[i].ROI[2]))*6;
		  break;
	  }
	}
		
	j=(long)LineData.size(); 

	for(i=0;i<j;i++)
	{
      if(LineData[i].Level==Number) break; 
	}

	if(i==j) return false;

	register unsigned char PadGrayL, PadGrayH;
	long ReStartP[2];
	
	PadGrayL=ThresholdColorL;
	PadGrayH=ThresholdColorH;
	AddShift=ImageAddShift;

	
	ContinueRoundData.push_back(BlankRound);

	X=ContinueRoundData[0].X=LineData[i].OrgX; 
    if(IsBMPFormat==true) Y=ContinueRoundData[0].Y=ImageH-LineData[i].OrgY-1; 
	else Y=ContinueRoundData[0].Y=LineData[i].OrgY; 

	AddShift=ImageW+ImageAddShift;
	PreDir=0;
	SetNumber=1;

	long Width,Height;
	Width=ImageRealEndX-1;
	Height=ImageRealEndY-1;

	CheckPx=X; CheckPy=Y;
	ReStartP[0]=X;
	ReStartP[1]=Y;

	for(;;)
	{
		Dir=0;
		index=X+Y*AddShift;

		if(SetNumber>=132)
			SetNumber=SetNumber;

		if( (PreDir!=2) && (X>ImageOrgX) && (ImageBuf[index-1]>=PadGrayL) && (ImageBuf[index-1]<=PadGrayH) ) Dir+=1;
		if( (PreDir!=1) && (X<Width) && (ImageBuf[index+1]>=PadGrayL) && (ImageBuf[index+1]<=PadGrayH) ) Dir+=2;
		if( (PreDir!=8) && (Y>ImageOrgY) && (ImageBuf[index-AddShift]>=PadGrayL) && (ImageBuf[index-AddShift]<=PadGrayH) ) Dir+=4;
		if( (PreDir!=4) && (Y<Height) && (ImageBuf[index+AddShift]>=PadGrayL) && (ImageBuf[index+AddShift]<=PadGrayH) ) Dir+=8;

		switch(Dir)
		{
		   case 1: PreDir=1;  break;
		   case 2: PreDir=2;  break;
           case 4: PreDir=4;  break;
           case 8: PreDir=8;  break;

           case 5: if(PreDir==1) PreDir=1;
			       else PreDir=4;
				   break;
		   case 6: if(PreDir==2) PreDir=2;
			       else PreDir=4;
				   break;
           case 9: if(PreDir==1) PreDir=1;
			       else PreDir=8;
				   break;
           case 10: if(PreDir==2) PreDir=2;
			       else PreDir=8;
				   break;

           case 7: if( (ImageBuf[index+1+AddShift]>=PadGrayL) && (ImageBuf[index+1+AddShift]<=PadGrayH) ) PreDir=1;
				   else PreDir=2;
				   break;
           case 11: if( (ImageBuf[index+1-AddShift]>=PadGrayL) && (ImageBuf[index+1-AddShift]<=PadGrayH) ) PreDir=1;
				   else PreDir=2; 
				   break;
           case 13: if( (ImageBuf[index+1+AddShift]>=PadGrayL) && (ImageBuf[index+1+AddShift]<=PadGrayH) ) PreDir=4;
				   else PreDir=8;
				   break;
           case 14: if( (ImageBuf[index-1+AddShift]>=PadGrayL) && (ImageBuf[index-1+AddShift]<=PadGrayH) ) PreDir=4;
				   else PreDir=8;
				   break;

		   case 3: 
			       if(PreDir==8)
				   {
					   if( (ImageBuf[index-1-AddShift]>=PadGrayL) && (ImageBuf[index-1-AddShift]<=PadGrayH) ) PreDir=1;
					   else PreDir=2;
				   }
			       else
				   {
					   if( (ImageBuf[index-1+AddShift]>=PadGrayL) && (ImageBuf[index-1+AddShift]<=PadGrayH) ) PreDir=1;
					   else PreDir=2;
				   }
 			       break;
		   case 12: 
			       if(PreDir==1)
				   {
                      if( (ImageBuf[index+1-AddShift]>=PadGrayL) && (ImageBuf[index+1-AddShift]<=PadGrayH) ) PreDir=4;
					   else PreDir=8;
					  
				   }
			       else
				   {
					  if( (ImageBuf[index-1-AddShift]>=PadGrayL) && (ImageBuf[index-1-AddShift]<=PadGrayH) ) PreDir=4;
					  else PreDir=8;
				   }
 			       break;

		   case 0: PreDir=0; break;
           case 15: PreDir=-1; break;
		}

		switch(PreDir)
		{
		   case 1:  X--; break;
		   case 2:  X++; break;
		   case 4:  Y--; break;
		   case 8:  Y++; break;
		   
		   case 0: 
			       BackNumber++;
				   if(BackNumber > 20)
				   {
                     ContinueRoundData.clear(); 
			         return false;
				   }

			       if(SetNumber<2) 
				   {
                     ContinueRoundData.clear(); 
			         return false;
				   }

				   if(X+Y*AddShift > ImageH*AddShift) 
				   {
                     ContinueRoundData.clear(); 
			         return false;
				   }

				   ImageBuf[X+Y*AddShift]=0;

				   X=ReStartP[0];
				   Y=ReStartP[1];
				   SetNumber=1;
				   PreDir=0;
				   BlankRound.X=X;
		           BlankRound.Y=Y;
		           ContinueRoundData.clear(); 
		           ContinueRoundData.push_back(BlankRound);
				   continue;

				   /*
				   X=ContinueRoundData[SetNumber-1].X; 
				   Y=ContinueRoundData[SetNumber-1].Y; 

                   ImageBuf[ContinueRoundData[SetNumber].X+ContinueRoundData[SetNumber].Y*AddShift]=0;

				   if(ContinueRoundData[SetNumber-2].X==X)
				   {
					   if(ContinueRoundData[SetNumber-2].Y==Y-1) PreDir=8;
					   else PreDir=4;
				   }
				   else
				   {
					   if(ContinueRoundData[SetNumber-1].X==X-1) PreDir=1;
					   else PreDir=2;
				   }

                   ContinueRoundData.erase(ContinueRoundData.begin()+SetNumber); 
				   continue;
				   */

				   break;

  	       case -1:  
			        return false; 
					break; 

		}

		if( CheckPx==X && CheckPy==Y) break;

		
		BlankRound.X=X;
		BlankRound.Y=Y;
		
		ContinueRoundData.push_back(BlankRound);

	    SetNumber++;

		if(SetNumber> MaxNumber)
		{
			X=ContinueRoundData[1].X;
			Y=ContinueRoundData[1].Y;

			for(i=2;i<SetNumber;i++)
			{
              if(ContinueRoundData[i].X==X && ContinueRoundData[i].Y==Y) 
			  {
				  for(j=i;j<SetNumber;j++) ContinueRoundData.erase(ContinueRoundData.begin()+i); 

				  if(IsBMPFormat==true)
				  {
				    Y=ImageH-1;
	                for(j=0;j<i;j++) ContinueRoundData[j].Y=Y-ContinueRoundData[j].Y;
				  }

				  IsRoundDataOK=true;
				  return true;
			  }

			}

            ContinueRoundData.clear(); 
			return false;
		}
	}

	if(IsBMPFormat==true)
	{

		j=ImageH-1;
		for(i=0;i<SetNumber;i++)
		{
		   ContinueRoundData[i].Y=j-ContinueRoundData[i].Y;
		}
	}

    IsRoundDataOK=true;
    
	return true;
}

bool CBlobAnalysis::GetRoundFitLine(int MinDis,float ShiftDis)
{
    RoundDataFitLine.clear();
	
	if(IsRoundDataOK==false) return false;

	register long Number;
	Number=(long)ContinueRoundData.size();

	register long i,tp;
	register float Dis,C;
	int FitSize;
	float slantDis=ShiftDis*ShiftDis;	
	register LineFit FitData;
	std::vector<LineFit> SaveFitData;

	float yy,xx;
    
	if(Number == 0) return false;

	FitSize=sizeof(LineFit);
	
	BlankRoundFitLine.X=ContinueRoundData[0].X; 
	BlankRoundFitLine.Y=ContinueRoundData[0].Y; 
	RoundDataFitLine.push_back(BlankRoundFitLine); 
	
	i=0;

	for(;;)
	{  
	   memset(&FitData,0x00,FitSize);

	   tp=i+MinDis;
	   if(tp>Number)
	   { tp=Number;
	     FitData.Number=Number-i;
	   }
	   else FitData.Number=MinDis;
		   
       for(;i<tp;i++)
	   {
           FitData.X+=ContinueRoundData[i].X; 
		   FitData.Y+=ContinueRoundData[i].Y; 
           FitData.XX=FitData.XX+ContinueRoundData[i].X*ContinueRoundData[i].X; 
		   FitData.XY=FitData.XY+ContinueRoundData[i].X*ContinueRoundData[i].Y; 
	   }

	   CalLineFit(FitData);

	   for(;i<Number;)
	   {
		   if(FitData.b==0.0f)
		   {
			   yy=(float)(ContinueRoundData[i].Y);
			   xx=(float)(FitData.c);
		   }
		   else if(FitData.a==0.0f)
		   {
			  xx=(float)(ContinueRoundData[i].X);
  		      yy=(float)(FitData.c);
		   }
		   else
		   { 
			 C=-(FitData.b*ContinueRoundData[i].X-FitData.a*ContinueRoundData[i].Y);
			 xx=FitData.a+FitData.b*FitData.b/FitData.a;
		     xx=-(FitData.b/FitData.a*C+FitData.c)/xx;
			 yy=(FitData.b*xx+C)/FitData.a; 
		   }
           
		   Dis=(xx-ContinueRoundData[i].X)*(xx-ContinueRoundData[i].X)+(yy-ContinueRoundData[i].Y)*(yy-ContinueRoundData[i].Y);
		   
		   if(Dis >= slantDis) break;
		   i++;
	   }

	   if(i>= Number)
	   {   FitData.PointX=ContinueRoundData[i-1].X; 
		   FitData.PointY=ContinueRoundData[i-1].Y; 
	   	   SaveFitData.push_back(FitData);
		   break;
	   }
	   else
	   {
          
		  FitData.PointX=ContinueRoundData[i-1].X; 
		  FitData.PointY=ContinueRoundData[i-1].Y; 
	   	  SaveFitData.push_back(FitData);
		 
	   }

	}

	register float Pre[3],ll[3];
	Pre[0]=SaveFitData[0].a;
	Pre[1]=SaveFitData[0].b;
	Pre[2]=SaveFitData[0].c;

	tp=(long)SaveFitData.size();
	for(i=1;i<tp;i++)
	{
		ll[0]=SaveFitData[i].a;
	    ll[1]=SaveFitData[i].b;
	    ll[2]=SaveFitData[i].c;

		if( fabs(Pre[0]-SaveFitData[i].a) >=0.02f || (Pre[1]-SaveFitData[i].b) >=0.020f || fabs(Pre[2]-SaveFitData[i].c) >= 30.0f)
		{   BlankRoundFitLine.X=SaveFitData[i-1].PointX;
		    BlankRoundFitLine.Y=SaveFitData[i-1].PointY;
			RoundDataFitLine.push_back(BlankRoundFitLine); 
			Pre[0]=SaveFitData[i].a;
	        Pre[1]=SaveFitData[i].b;
	        Pre[2]=SaveFitData[i].c;
		}
	}

	BlankRoundFitLine.X=SaveFitData[i-1].PointX;
    BlankRoundFitLine.Y=SaveFitData[i-1].PointY;
	RoundDataFitLine.push_back(BlankRoundFitLine); 


    SaveFitData.clear();

	return true;
}

bool CBlobAnalysis::CalLineFit(LineFit &Data)
{
    register double tp1,tp2,tp3,tp4;

	tp3=(double) Data.X/(double)Data.Number;
	tp1=(double) Data.XX/(double)Data.X-(double)tp3;
	if(tp1==0.0f) 
	{	
		Data.a=1.0f; 
	    Data.b=0.0f; 
		Data.c=(float)(tp3); 
		return true;
	}
    
	tp4=(double) Data.Y/(double)Data.Number;
	tp2=(double) Data.XY/(double)Data.X-(double)tp4;

    Data.a=(float)(tp2/tp1); 
	Data.c=(float)(tp4-tp3*(double)Data.a);
	Data.b=-1.0f;

	return true;
}

bool CBlobAnalysis::AutoThreshold(long Width,long Height,unsigned char *Image,unsigned char &Value,char Type,long OrgX,long OrgY,long OrgWidth,long OrgHeight)
{
    long RealEndX,RealEndY;
	
    if(IsBMPFormat==true)
	{ 
	  // modify X-axis parameter
      if(OrgWidth==-1) 
	  { RealEndX=Width;
	    OrgX=0;
	  }
	  else
	  {	 if( (OrgX+OrgWidth)>Width) OrgWidth=Width-OrgX;
		 RealEndX=OrgWidth+OrgX; 
	  }

	  // modify Y-axis parameter
      if(OrgHeight==-1)
	  { RealEndY=Height;
	    OrgY=0;
	  }
      else
	  { 
		if( (OrgY+OrgHeight)>Height) OrgHeight=Height-OrgY;

		OrgY=Height-(OrgHeight+OrgY);
	    RealEndY=OrgHeight+OrgY;
	    
	  }
	}
	else  // Pure data
	{ 
	  // modify X-axis parameter 
	  if(OrgWidth==-1) 
	  { RealEndX=Width;
	    OrgX=0;
	  }
	  else
	  {	 if( (OrgX+OrgWidth)>Width) OrgWidth=Width-OrgX;
		 RealEndX=OrgWidth+OrgX; 
	  }

      // modify Y-axis parameter
	  if(OrgHeight==-1)
	  { RealEndY=Height;
	    OrgY=0;
	  }
      else
	  { if( (OrgY+OrgHeight)>Height) OrgHeight=Height-OrgY;
		RealEndY=OrgHeight+OrgY;
	  }

	}

	switch(Type)
	{
	  case 0:  // 平均灰階值
              Value=T_Threshold(Image,OrgX,OrgY,RealEndX,RealEndY,Width);
			  break;
	  case 1:  // OTSU二值化
		       Value=OTSU_Threshold(Image,OrgX,OrgY,RealEndX,RealEndY,Width);
		       break;

	}

    
	return true;
}

// 平均灰階值
unsigned char CBlobAnalysis::T_Threshold(unsigned char *Image,long OrgX,long OrgY,long RealEndX,long RealEndY,long Width)
{
   unsigned char Threshold;
   register long i,j,tp,tp2,Number;

   tp=0;  
   Number=(RealEndX-OrgX)*(RealEndY-OrgY);
		  
   for(j=OrgY;j<RealEndY;j++)
   {  tp2=j*Width+OrgX;
      for(i=OrgX;i<RealEndX;i++)
	  {
          tp+=Image[tp2];
		  tp2++;
	  }
  }

   Threshold=(unsigned char)(tp/Number);

   return Threshold;
}


// OTSU二值化
unsigned char CBlobAnalysis::OTSU_Threshold(unsigned char *Image,long OrgX,long OrgY,long RealEndX,long RealEndY,long Width)
{
   unsigned char Threshold;
   register long i,j,e,tp2,Value1=0,Value2=0,EndX,EndY;

   EndX=RealEndX-1;
   if(RealEndX==OrgX) 
   {  Threshold=0;
      return Threshold;
   }

   EndY=RealEndY-1;
   if(RealEndY==OrgY) 
   {  Threshold=0;
      return Threshold;
   }

   for(j=OrgY;j<EndY;j++)
   {  tp2=j*Width+OrgX;
      for(i=OrgX;i<EndX;i++)
	  {
        e=abs(Image[tp2+1]-Image[tp2])+abs(Image[tp2+Width]-Image[tp2]);
        Value1=Value1+e*Image[tp2];
        Value2+=e;
	 	tp2++;
	  }
  }

   Threshold=(unsigned char) (Value1/Value2);

   return Threshold;
}

bool CBlobAnalysis::FindExtendRound(long Number)
{
	register long i,j,k,m,Start,Total,RunNumber;
    BlobPoint *Line=NULL;

	RoundData.clear();
	
    for(i=0;i<PadNumber;i++)
	{
      if(ROI_Data[i].Level==Number) 
	  {   Total=ROI_Data[i].ROI[3];
		  Line=new BlobPoint[Total-ROI_Data[i].ROI[2]+1];
		  break;
	  }
	}

	if(Line==NULL) return false;

	RunNumber=(long)LineData.size();
	
	Start=ROI_Data[i].ROI[2];

	if(IsBMPFormat==true)
	{
		for(k=RunNumber-1;k>=0;k--)
		{
			if(Start==LineData[k].OrgY) break;
		}

		if(k<0) return false;

		m=0;
		for(j=Start;j<=Total;j++)
		{
		   Line[m].X=ROI_Data[i].ROI[1];
		   Line[m].Y=ROI_Data[i].ROI[0];
		   
		   for(;k>=0;k--)
		   {
			   if(LineData[k].OrgY!=j) break;
			   if(LineData[k].Level!=Number) continue;

			   if(LineData[k].OrgX < Line[m].X ) Line[m].X=LineData[k].OrgX;
			   if(LineData[k].OrgX+LineData[k].Len > Line[m].Y ) Line[m].Y=LineData[k].OrgX+LineData[k].Len;

		   }
		   m++;
		}

		BlankRound.Y=Start;

		for(j=Line[0].X;j<Line[0].Y;j++)
		{
			BlankRound.X=j;
			RoundData.push_back(BlankRound);
			
		}

		m=1;
		for(j=Start+1;j<=Total;j++)
		{
			BlankRound.Y=j;

			// add left-side data
			if(Line[m].X < Line[m-1].X)
			{
				for(k=Line[m].X;k<=Line[m-1].X;k++)
				{
					BlankRound.X=k;
					RoundData.push_back(BlankRound);
				}
			}
			else
			{
				for(k=Line[m].X;k>=Line[m-1].X;k--)
				{
					BlankRound.X=k;
					RoundData.push_back(BlankRound);
				}
			}

			// add right-side data
			if(Line[m].Y < Line[m-1].Y)
			{
				for(k=Line[m].Y;k<=Line[m-1].Y;k++)
				{
					BlankRound.X=k;
					RoundData.push_back(BlankRound);
				}
			}
			else
			{
				for(k=Line[m].Y;k>=Line[m-1].Y;k--)
				{
					BlankRound.X=k;
					RoundData.push_back(BlankRound);
				}
			}
			m++;
			
		}

	}
	else
	{
		for(k=0;k<RunNumber;k++)
		{
			if(Start==LineData[k].OrgY) break;
		}

		if(k>=RunNumber) return false;

		m=0;
		for(j=Start;j<=Total;j++)
		{
		   Line[m].X=ROI_Data[i].ROI[1];
		   Line[m].Y=ROI_Data[i].ROI[0];
		   
		   for(;k<RunNumber;k++)
		   {
			   if(LineData[k].OrgY!=j) break;
			   if(LineData[k].Level!=Number) continue;

			   if(LineData[k].OrgX < Line[m].X ) Line[m].X=LineData[k].OrgX;
			   if(LineData[k].OrgX+LineData[k].Len > Line[m].Y ) Line[m].Y=LineData[k].OrgX+LineData[k].Len;

		   }
		   m++;
		}

		BlankRound.Y=Start;

		for(j=Line[0].X;j<Line[0].Y;j++)
		{
			BlankRound.X=j;
			RoundData.push_back(BlankRound);
		}

		m=1;
		for(j=Start+1;j<=Total;j++)
		{
			BlankRound.Y=j;

			// add left-side data
			if(Line[m].X < Line[m-1].X)
			{
				for(k=Line[m].X;k<=Line[m-1].X;k++)
				{
					BlankRound.X=k;
					RoundData.push_back(BlankRound);
				}
			}
			else
			{
				for(k=Line[m].X;k>=Line[m-1].X;k--)
				{
					BlankRound.X=k;
					RoundData.push_back(BlankRound);
				}
			}

			// add right-side data
			if(Line[m].Y < Line[m-1].Y)
			{
				for(k=Line[m].Y;k<=Line[m-1].Y;k++)
				{
					BlankRound.X=k;
					RoundData.push_back(BlankRound);
				}
			}
			else
			{
				for(k=Line[m].Y;k>=Line[m-1].Y;k--)
				{
					BlankRound.X=k;
					RoundData.push_back(BlankRound);
				}
			}
			m++;
			
		}


	}

	delete[] Line;

    return true;
}

bool CBlobAnalysis::FitCircle(long Number,float &X,float &Y,float &STD,float &Radius)
{
	if(FindExtendRound(Number)==false) return false;

	register long i,Total;
	
	Total=(long)RoundData.size();

	if(Total < 10 ) 
	{   RoundData.clear(); 
		return false;
	}

	register _int64 SX,SY,SXX,SYY,SXY,SXXX,SYYY,SXYY,SXXY;
	register _int64 a1,b1,b2,c1,c2,tp;
	
	SX=SXX=SY=SYY=SXY=SXXX=SYYY=SXYY=SXXY=0;

	
	for(i=0;i<Total;i++)
	{
	   SX+=RoundData[i].X; 
	   SY+=RoundData[i].Y;

	   tp=RoundData[i].X*RoundData[i].X;
	   SXX=SXX+tp;
	   SXXX=SXXX+tp*RoundData[i].X;
	   SXXY=SXXY+tp*RoundData[i].Y;

	   tp=RoundData[i].Y*RoundData[i].Y;
	   SYY=SYY+tp;
	   SYYY=SYYY+tp*RoundData[i].Y;
	   SXYY=SXYY+RoundData[i].X*tp;

	   SXY=SXY+RoundData[i].X*RoundData[i].Y;
		
	}

	a1=2*(SX*SX-Total*SXX);
	b1=2*(SX*SY-Total*SXY);
	b2=2*(SY*SY-Total*SYY);
	c1=SXX*SX-Total*SXXX+SX*SYY-Total*SXYY;
	c2=SXX*SY-Total*SYYY+SY*SYY-Total*SXXY;

    tp=a1*b2-b1*b1;

	if(tp==0) 
	{   RoundData.clear(); 
		return false;
	}

    X=((float) c1*b2-c2*b1)/tp;
	Y=((float) c2*a1-c1*b1)/tp;

	register double fX=0.0f,fXX=0.0f,Dis;

	for(i=0;i<Total;i++)
	{
		Dis=((float) RoundData[i].X-X)*(RoundData[i].X-X)+(RoundData[i].Y-Y)*(RoundData[i].Y-Y);
		if(Dis!=0.0f) Dis=sqrt(Dis);
		fX+=Dis;
		fXX=fXX+Dis*Dis;
	}
    

	STD=(float)(((double)Total*fXX-fX*fX)/Total/(Total-1));
	if(STD!=0.0f) STD=sqrt(STD);

	Radius=(float)(fX/Total);   

	return true;
}

bool CBlobAnalysis::Erode(long Width,long Height,unsigned char *Image,int ErodeSize,char Type,long OrgX,long OrgY,long OrgWidth,long OrgHeight)
{
    long RealEndX,RealEndY;
	
    if(IsBMPFormat==true)
	{ 
	  // modify X-axis parameter
      if(OrgWidth==-1) 
	  { RealEndX=Width;
	    OrgX=0;
	  }
	  else
	  {	 if( (OrgX+OrgWidth)>Width) OrgWidth=Width-OrgX;
		 RealEndX=OrgWidth+OrgX; 
	  }

	  // modify Y-axis parameter
      if(OrgHeight==-1)
	  { RealEndY=Height;
	    OrgY=0;
	  }
      else
	  { 
		if( (OrgY+OrgHeight)>Height) OrgHeight=Height-OrgY;

		OrgY=Height-(OrgHeight+OrgY);
	    RealEndY=OrgHeight+OrgY;
	    
	  }
	}
	else  // Pure data
	{ 
	  // modify X-axis parameter 
	  if(OrgWidth==-1) 
	  { RealEndX=Width;
	    OrgX=0;
	  }
	  else
	  {	 if( (OrgX+OrgWidth)>Width) OrgWidth=Width-OrgX;
		 RealEndX=OrgWidth+OrgX; 
	  }

      // modify Y-axis parameter
	  if(OrgHeight==-1)
	  { RealEndY=Height;
	    OrgY=0;
	  }
      else
	  { if( (OrgY+OrgHeight)>Height) OrgHeight=Height-OrgY;
		RealEndY=OrgHeight+OrgY;
	  }
	}

	register long i,j,m,n,index;
	register char AddShift=ImageAddShift;

	for(j=OrgY;j<RealEndY;j++)
	{
		index=OrgX+j*(Width+AddShift);

		for(i=OrgX;i<RealEndX;i++)
		{
			m=i-ErodeSize;
			if( m < OrgX ) m=OrgX;
			n=j-ErodeSize;
			if( n < OrgY ) n=OrgY;

			for(;;m++)
			{
				if(m>=RealEndX || m>i+ErodeSize) break;

			}

			index++;
			
		}
	}
	return true;

}

bool CBlobAnalysis::Draw(CDC &pDC,long ShiftX,long ShiftY)
{
  register long i,j,k;
  
  if(IsBMPFormat==true)
  {
    for(i=0;i<ImageW;i++)
	{
	  for(j=0;j<ImageH;j++)
	  {   k=ImageBuf[i+j*(ImageW+ImageAddShift)];
		  pDC.SetPixel(i+ShiftX,ImageH-j-1+ShiftY,RGB(k,k,k));
	  }
	}
  }
  else
  {
    for(i=0;i<ImageW;i++)
	{
	  for(j=0;j<ImageH;j++)
	  {   k=ImageBuf[i+j*(ImageW+ImageAddShift)];
		  pDC.SetPixel(i+ShiftX,j+ShiftY,RGB(k,k,k));
	  }
	}
  } 
	return true;

}