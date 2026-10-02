// 3DUnWrapping.cpp: implementation of the C3dUnWrapping class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "3DUnWrapping.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
C3dUnWrapping    Calc3DUnWarpping;
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
C3dUnWrapping::C3dUnWrapping()
{
	C3dUnWrapping::PreInitUnWrapping();
	C3dUnWrapping::InitialUnWrapping();
}
//-------------------------------------------------------------------------------------//
C3dUnWrapping::~C3dUnWrapping()
{
	C3dUnWrapping::ReleaseAllBuffer();	
}
//-------------------------------------------------------------------------------------//
void C3dUnWrapping::PreInitUnWrapping()
{
	size_t i=0;
	for ( i=0; i<MAX_THREAD_COUNT; i++ )
	{
		m_ImageW[i] = 0;
		m_ImageH[i] = 0;
		m_ImageStep[i] = 0;
		m_BitCount[i] = 8;
		m_BitPtr[i] = NULL;
		m_MarkPtr[i] = NULL;
		m_SpacePtr[i] = NULL;
	}	
}
//-------------------------------------------------------------------------------------//
void C3dUnWrapping::InitialUnWrapping()
{
}
//-------------------------------------------------------------------------------------//
inline bool C3dUnWrapping::CheckThreadID(THREAD_ID ThreadID)//確認執行緒編號
{
	const int value = ThreadID;
	if ( value<0 || value>=MAX_THREAD_COUNT ) 
	{
		this->m_ErrorString.Format(_T("Error, Thread ID Exception(%d)"), ThreadID);
		return false; 
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
inline bool C3dUnWrapping::GetBitMarkBuffer(unsigned int &ImageW, unsigned int &ImageH, unsigned int &ImageStep, MASK_PTR &BitPtr, MASK_PTR &MarkPtr, THREAD_ID ThreadID)
{
	if ( C3dUnWrapping::CheckThreadID(ThreadID) == false )
	{	return false; }
	ImageW = m_ImageW[ThreadID];
	ImageH = m_ImageH[ThreadID];
	ImageStep = m_ImageStep[ThreadID];
	BitPtr = m_BitPtr[ThreadID];
	MarkPtr = m_MarkPtr[ThreadID];
	return true;
}
//-------------------------------------------------------------------------------------//
bool C3dUnWrapping::CreateBuffer(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, THREAD_ID ThreadID)//建立unwrapping所需暫存器
{
	const char fnName[] = "C3dUnWrapping::CreateBuffer";
	if ( C3dUnWrapping::CheckThreadID(ThreadID) == false )
	{	return false; }
	C3dUnWrapping::ReleaseBuffer(ThreadID);	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, m_BitPtr[ThreadID], fnName, "m_BitPtr") == false ||
		 JetMemory.alloc_func(BufferSize, m_MarkPtr[ThreadID], fnName, "m_MarkPtr") == false ||
		 JetMemory.alloc_func(BufferSize, m_SpacePtr[ThreadID], fnName, "m_SpacePtr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();

		JetMemory.free_func(m_BitPtr[ThreadID]);
		JetMemory.free_func(m_MarkPtr[ThreadID]);
		JetMemory.free_func(m_SpacePtr[ThreadID]);
		return false;
	} 

	m_ImageW[ThreadID] = ImageW;
	m_ImageH[ThreadID] = ImageH;
	m_ImageStep[ThreadID] = ImageStep;
	m_BitCount[ThreadID] = 8;
	return true;
}
//-------------------------------------------------------------------------------------//
bool C3dUnWrapping::ReleaseBuffer(THREAD_ID ThreadID)//釋放unwrapping所需暫存器
{
	if ( C3dUnWrapping::CheckThreadID(ThreadID) == false )
	{	return false; }
	if ( NULL != m_BitPtr[ThreadID] )
	{	JetMemory.free_func(m_BitPtr[ThreadID]);	}
	if ( NULL != m_MarkPtr[ThreadID] )
	{	JetMemory.free_func(m_MarkPtr[ThreadID]);	}
	if ( NULL != m_SpacePtr[ThreadID] )
	{	JetMemory.free_func(m_SpacePtr[ThreadID]);	}	
	m_ImageW[ThreadID] = 0;
	m_ImageH[ThreadID] = 0;
	m_ImageStep[ThreadID] = 0;
	m_BitCount[ThreadID] = 0;
	return true;
}
//-------------------------------------------------------------------------------------//
bool C3dUnWrapping::ReleaseAllBuffer()//釋放unwrapping所有暫存器
{
	size_t i=0;
	for ( i=0; i<MAX_THREAD_COUNT; i++ )
	{
		m_ImageW[i] = 0;
		m_ImageH[i] = 0;
		m_ImageStep[i] = 0;
		m_BitCount[i] = 8;

		if ( NULL != m_BitPtr[i] )
		{	JetMemory.free_func(m_BitPtr[i]); }

		if ( NULL != m_MarkPtr[i] )
		{	JetMemory.free_func(m_MarkPtr[i]);	}

		if ( NULL != m_SpacePtr[i] )
		{	JetMemory.free_func(m_SpacePtr[i]);	}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool C3dUnWrapping::ExecPhaseUnWrapping(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, PHASE_PTR PhasePtr, MASK_PTR MaskPtr, SPACE_PTR &DstPtr, THREAD_ID ThreadID)//計算相位還原
{
	const char fnName[] = "C3dUnWrapping::ExecPhaseUnWrapping";

	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	JetMemory.free_func(DstPtr);
	if ( JetMemory.alloc_func(BufferSize, DstPtr, fnName, "DstPtr") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		return false;
	}
	if ( C3dUnWrapping::ExecPhaseUnWrapping3(ImageW, ImageH, ImageStep, PhasePtr, MaskPtr, DstPtr, ThreadID) == false )
	{
		JetMemory.free_func(DstPtr);
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool C3dUnWrapping::ExecPhaseUnWrapping3(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, PHASE_PTR PhasePtr, MASK_PTR PhaseMaskPtr, SPACE_PTR DstPtr, THREAD_ID ThreadID)//計算相位還原
{
	const char fnName[] = "C3dUnWrapping::ExecPhaseUnWrapping3";
	unsigned int OldImageW=0, OldImageH=0, OldImageStep=0;
	int NumRes=0, MaxCutLen=0;
	const PHASE_PTR pPhaseData = PhasePtr;
	MASK_PTR BitPtr = NULL;
	MASK_PTR MarkPtr = NULL;
	SPACE_PTR PhaseBuffer = NULL;

	if ( NULL == PhasePtr)
	{
		m_ErrorString.Format(_T("Error, Phase Image Pointer Fault!"));
		return false;
	}
/*	if( PhaseMask == NULL)
	{		
		m_ErrorString.Format(_T("Error, Mask Map Pointer Fault!"));
		return false;
	}*/

	if ( this->GetBitMarkBuffer(OldImageW, OldImageH, OldImageStep, BitPtr, MarkPtr, ThreadID) == false )
	{	return false; }
	
	if( DstPtr == NULL)
	{
		m_ErrorString.Format(_T("Error, UnWrapping Pointer Fault!"));
		return false;
	}	
	if( ImageW != OldImageW || ImageH != OldImageH || ImageStep != OldImageStep )
	{
		m_ErrorString.Format(_T("Error, Image size Fault! (%d, %d) (%d, %d)"), ImageW, ImageH, OldImageW, OldImageH);
		return false;
	}
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if( BufferSize == 0 )
	{
		m_ErrorString.Format(_T("Error, Image size Fault! (Size = %s)"), BufferSize);
		return false;
	}

	//將相位資料正規化0~1	
	if ( JetMemory.alloc_func(BufferSize, PhaseBuffer, fnName, "PhaseBuffer") == false )	
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		return false;
	}
	size_t i=0;
	size_t count = 0;
	double dPhase = 0;
	MASK_DATA PhaseMask = 0;
	const double PhaseToOne = PHASE_SHORT2ONE;
	for( i=0 ; i<BufferSize ; i++ )
	{
		if ( NULL != PhaseMaskPtr )
		{
			PhaseMask = PhaseMaskPtr[i];
			if( (PhaseMask&PHASE_MASK_NOISE) != 0 )  
			{ 
				PhaseBuffer[i] = UNWRAP_DATA_AVOID_PHASE; 
				continue; 
			}		
		}
		dPhase =  PhasePtr[i];
		dPhase *= PhaseToOne;
		PhaseBuffer[i] = static_cast<SPACE_DATA>(dPhase);
		if( PhaseBuffer[i] > UNWRAP_PERIOD_FULL ) 
		{ 
			PhaseBuffer[i] = UNWRAP_DATA_AVOID_PHASE; 
			count++; 
			continue; 
		}
	}
	count = 0;

	TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	PHASE_UNWRAP_MODE UnWrappingMode = SysParam.m_UnwrappingMode;
	if( PHASE_UNWRAP_DISABLE == UnWrappingMode )
	{
		::memcpy(DstPtr, PhaseBuffer, sizeof(SPACE_DATA)*BufferSize);
		JetMemory.free_func(PhaseBuffer);
		return true;
	}

	//UnWrappingMode = PHASE_UNWRAP_GOLDSTEIN;
	if( PHASE_UNWRAP_PHASE == UnWrappingMode )
	{
		if( ExecUnWrapping_Phase(PhaseBuffer, PhaseMaskPtr, ImageW, ImageH, ImageStep, BitPtr, MarkPtr, DstPtr, ThreadID) == false )
		{ 
			JetMemory.free_func(PhaseBuffer);
			return false; 
		}
	}
	else //PHASE_UNWRAP_GOLDSTEIN
	{
		if( ExecUnWrapping_Goldstein(PhaseBuffer, PhaseMaskPtr, ImageW, ImageH, ImageStep, BitPtr, MarkPtr, DstPtr, ThreadID) == false )
		{ 
			JetMemory.free_func(PhaseBuffer);
			return false; 
		}
	}
	JetMemory.free_func(PhaseBuffer);
	return true;
}
//-------------------------------------------------------------------------------------//
bool C3dUnWrapping::ExecUnWrapping_Phase(const SPACE_PTR PhasePtr, const MASK_PTR PhaseMaskPtr, unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, MASK_PTR BitPtr, MASK_PTR MarkPtr, SPACE_PTR DstPtr, THREAD_ID ThreadID)
{	
	size_t i=0, j=0;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	::memset(MarkPtr, 0x00, sizeof(MASK_DATA)*BufferSize);
	::memset(BitPtr, 0x00, sizeof(MASK_DATA)*BufferSize);
	::memset(DstPtr, 0x00, sizeof(SPACE_DATA)*BufferSize);

	if( NULL == PhaseMaskPtr )
	{	::memcpy(DstPtr, PhasePtr, sizeof(SPACE_DATA)*BufferSize);	}
	else
	{	
		MASK_DATA PhaseMask = 0;		
		for( i=0 ; i<BufferSize ; i++ )
		{
			PhaseMask = PhaseMaskPtr[i];
			if( (PhaseMask&PHASE_MASK_NOISE) != 0 )		//無效點
			{
				if     ( PhaseMask&PHASE_MASK_LOW_CONTRAST )	{ DstPtr[i] = UNWRAP_DATA_AVOID_PHASE_LOW_CONTRAST;  continue;}
				else if( PhaseMask&PHASE_MASK_LOW_POTENTIAL )	{ DstPtr[i] = UNWRAP_DATA_AVOID_PHASE_LOW_CONTRAST;  continue;}
				else if( PhaseMask&PHASE_MASK_OVER_SATURATED )	{ DstPtr[i] = UNWRAP_DATA_AVOID_PHASE_OVER_SATURATION;  continue;}
				else if( PhaseMask&PHASE_MASK_EXTEND_VOID ) 	{ DstPtr[i] = UNWRAP_DATA_AVOID_PHASE_EXTEND_VOID;  continue;}				
			}
			DstPtr[i] = PhasePtr[i];
		}	
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool C3dUnWrapping::ExecUnWrapping_Goldstein(const SPACE_PTR PhasePtr, const MASK_PTR PhaseMaskPtr, unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, MASK_PTR BitPtr, MASK_PTR MaskPtr, SPACE_PTR DstPtr, THREAD_ID ThreadID)
{
	size_t i=0;	
	int NumRes=0, MaxCutLen=0;
	const SPACE_PTR pPhaseData = PhasePtr;	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);

	//此模式的輸入資料已經做過正規化0~1所以不用再轉了。
	//相位資料轉為0~1 (小於0，則為無效點)
//	::memcpy(pPhaseData, pPhaseImage, sizeof(float)*BufferSize);
	::memset(MaskPtr, 0x00, sizeof(MASK_DATA)*BufferSize);
	::memset(BitPtr, 0x00, sizeof(MASK_DATA)*BufferSize);
	::memset(DstPtr, 0x00, sizeof(SPACE_DATA)*BufferSize);

	//標記無效點<BORDER>(計算相位時得到的無效點)
	if( MaskRegion(ImageW, ImageH, ImageStep, PhasePtr, BitPtr, BORDER) == false ) { return false; }

	//標記不連續點(極性點+、-)<POS_RES, NEG_RES>	
	if( Residues(ImageW, ImageH, ImageStep, PhasePtr, BitPtr, POS_RES, NEG_RES, BORDER, NumRes) == false ) { return false; }

	//Generate Branch Cuts//
	//相鄰極性點插入BranchCuts平衡極性<BRANCH_CUT>
	if( Dipole(ImageW, ImageH, ImageStep, BitPtr, MaskPtr, BRANCH_CUT) == false ) { return false; }

    if (MaxCutLen==0) { MaxCutLen = (int)((ImageW+ImageH)*0.5); }

	//所有極性點插入BranchCuts平衡極性<BRANCH_CUT>
	if( GoldsteinBranchCuts(ImageW, ImageH, ImageStep, BitPtr, MaskPtr, MaxCutLen, NumRes, BRANCH_CUT, ThreadID) == false ) { return false; }

	if( UnwrapAroundCuts(ImageW, ImageH, ImageStep, PhasePtr, BitPtr, MaskPtr, DstPtr, AVOID, 0, NULL, ThreadID) == false ) { return false; }	
		
	for(i=0 ; i<BufferSize ; i++ )
	{		
		if ( (BitPtr[i]&BORDER) )//標記無效點		
		{ 
			DstPtr[i] = UNWRAP_DATA_AVOID_PHASE;			
			continue; 
		}		

		if( (MaskPtr[i]&UNWRAP_MARK)==0 )	//標記無效點
		{ 
			DstPtr[i] = UNWRAP_DATA_AVOID_PHASE;
			continue;  
		}			
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool C3dUnWrapping::MaskRegion(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStpe, const SPACE_PTR PhasePtr, MASK_PTR BitPtr, int avoid_code)
{
	if( NULL==PhasePtr || NULL==BitPtr ) 
	{	return false; }

	size_t i=0;
	const size_t BufferSize = ImageH*ImageStpe;
	for( i=0 ; i<BufferSize ; i++ )
	{
		if( PhasePtr[i] < 0 )
		{	BitPtr[i] = BitPtr[i]|avoid_code;	}		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
SPACE_DATA  C3dUnWrapping::Gradient(SPACE_DATA p1, SPACE_DATA p2)
{
	SPACE_DATA  r = p1 - p2;	
	if ( r > UNWRAP_PERIOD_HALF) 
	{	r -= UNWRAP_PERIOD_FULL;	}

	if ( r < -UNWRAP_PERIOD_HALF) 
	{	r += UNWRAP_PERIOD_FULL;	}
	return r;
}
//-------------------------------------------------------------------------------------//
bool C3dUnWrapping::Residues(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const SPACE_PTR PhasePtr, MASK_PTR BitPtr, int posres_code, int negres_code, int avoid_code, int &NumRes)
{
	if( NULL==PhasePtr || NULL==BitPtr ) 
	{	return false; }
	const char fnName[] = "C3dUnWrapping::Residues";
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);	
	const unsigned int ImageH1 = ImageH-1;
	const unsigned int ImageW1 = ImageW-1;
	size_t Index = 0;
	size_t i=0, j=0 , k=0;
	SPACE_DATA  r=0; 		
	SPACE_DATA  r1=0, r2=0, r3=0, r4=0;
	SPACE_DATA  TempR=0;
	const SPACE_DATA MinValue1 = UNWRAP_PERIOD_FULL/10;
	const SPACE_DATA MinValue2 = UNWRAP_PERIOD_FULL/100;
	SPACE_PTR   TempRList=NULL;

	if ( JetMemory.alloc_func(ImageStep, TempRList, fnName, "TempRList") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		return false;
	}

	for (i=0; i<ImageH1; i++)
	{        
		Index = i*ImageStep;
		for( j=0 ; j<ImageW1 ; j++ )
		{
			k = Index+j;
			if(  (BitPtr[k]&avoid_code) || (BitPtr[k+1]&avoid_code) 
			|| (BitPtr[k+1+ImageStep]&avoid_code) || (BitPtr[k+ImageStep]&avoid_code) )
			{
				continue; // masked region: don't unwrap
			}

			if( i > 0 )
			{
				if( j > 0 )
				{
					r1 = -TempRList[j];
					r4 = -TempR;
				}
				else
				{
					r1 = -TempRList[j];
					r4 = Gradient(PhasePtr[k], PhasePtr[k+ImageStep]);
				}
			}
			else
			{
				if( j > 0 )
				{
					r4 = -TempR;						
				}
				else
				{
					r4 = Gradient(PhasePtr[k], PhasePtr[k+ImageStep]);
				}
				r1 = Gradient(PhasePtr[k+1], PhasePtr[k]);
			}
			r2 = Gradient(PhasePtr[k+1+ImageStep], PhasePtr[k+1]);
			r3 = Gradient(PhasePtr[k+ImageStep], PhasePtr[k+1+ImageStep]);
			TempRList[j] = r3;
			TempR = r2;

			r = r1+r2+r3+r4;
			if (r > MinValue2)
			{ 
				//m_MarkMap[k] = POS_MARK; 
				BitPtr[k] = BitPtr[k]|posres_code; 
				if( r > MinValue1 )
				{ NumRes++; }				
			}
			else if (r < -MinValue2)
			{ 
				//m_MarkMap[k] = NEG_MARK; 
				BitPtr[k] = BitPtr[k]|negres_code; 
				if( r < -MinValue1 )
				{	NumRes++; }
			}					
		}	
	}
	JetMemory.free_func(TempRList);
	return true;
}
//-------------------------------------------------------------------------------------//
bool C3dUnWrapping::Dipole(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, MASK_PTR BitPtr, MASK_PTR MaskPtr, int branchcut_code)
{
	if( NULL==BitPtr || NULL==MaskPtr ) 
	{	return false; }

	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	unsigned int i=0, j=0;
	unsigned int index = 0;
	unsigned int index2 = 0;
	unsigned int indexEnd = 0;
	unsigned int xx=0, yy=0;
	
	MASK_DATA TempFlag = 0;
	for (j=0; j<ImageH; j++)
	{
		index2 = j*ImageStep;
		for (i=0; i<ImageW; i++)
		{	
			index = index2+i;

			indexEnd = 0;
			TempFlag = BitPtr[index];
			xx = i;
			yy = j;
			if (TempFlag & POS_RES)
			{
				if (i<ImageW-1 && (BitPtr[index+1] & NEG_RES))
				{ 
					indexEnd=index+1;
					xx = i+1;
				}
				else if (j<ImageH-1)
				{
					if ((BitPtr[index+ImageStep] & NEG_RES))
					{ 
						indexEnd=index+ImageStep; 
						yy = j+1;
					}
				}
			}
			else if (TempFlag & NEG_RES)
			{
				if (i<ImageW-1 && (BitPtr[index+1] & POS_RES))
				{ 
					indexEnd=index+1; 
					xx = i+1;
				}
				else if (j<ImageH-1)
				{
					if ((BitPtr[index+ImageStep] & POS_RES))
					{ 
						indexEnd=index+ImageStep; 
						yy = j+1;
					}
				}
			}

			if ( indexEnd != 0 )
			{
				//PlaceCut(BitPtr, MaskPtr, i, j, xx, yy, xsize, ysize, branchcut_code);
				PlaceCut(ImageW, ImageH, ImageStep, BitPtr, MaskPtr, i, j, xx, yy, branchcut_code);
				BitPtr[index] &= (~(RESIDUE));     //取消正、負極狀態 
				BitPtr[indexEnd] &= (~(RESIDUE));    //取消正、負極狀態   
			}
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool C3dUnWrapping::PlaceCut(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, MASK_PTR BitPtr, MASK_PTR MaskPtr, int a, int b, int c, int d, int code)
{
	/* residue location is upper-left corner of 4-square */
	//設定相鄰的BlachCut
	if (c > a && a > 0)
	{ a++; }
	else if (c < a && c > 0)
	{ c++; }

	if (d > b && b > 0)
	{ b++; }
	else if (d < b && d > 0)
	{ d++; }

	if (a==c && b==d)
	{
		BitPtr[b*ImageStep + a] |= code;     //設為BranchCut狀態 
		MaskPtr[b*ImageStep + a] = CUT_MARK;
		return true;
	}

	//設定不相鄰的BlachCut
	int     i, j, m, n;
	int     istep=0, jstep=0;	
	double  r;
	m = (a < c) ? c - a : a - c;
	n = (b < d) ? d - b : b - d;
	if (m > n)
	{
		istep = (a < c) ? +1 : -1;
		r = ((double)(d - b))/((double)(c - a));
		for (i=a; i!=c+istep; i+=istep)
		{
			j = (int)(b + (i - a)*r + 0.5);
			BitPtr[j*ImageStep + i] |= code;     //設為BranchCut狀態
			MaskPtr[j*ImageStep + i] = CUT_MARK;
		}
	}
	else   /* n < m */
	{
		jstep = (b < d) ? +1 : -1;
		r = ((double)(c - a))/((double)(d - b));
		for (j=b; j!=d+jstep; j+=jstep)
		{
			i = (int)(a + (j - b)*r + 0.5);
			BitPtr[j*ImageStep + i] |= code;     //設為BranchCut狀態
			MaskPtr[j*ImageStep + i] = CUT_MARK;
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
unsigned int C3dUnWrapping::DistToBorder(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, MASK_PTR BitPtr, int border_code, unsigned int a, unsigned int b, unsigned int &ra, unsigned int &rb)
{	
	unsigned int  i, j, k, bs;	
	unsigned int Box_StartX=0, Box_StartY=0, Box_EndX=0, Box_EndY=0;
	unsigned int TempIndex = 0;
	unsigned int  besta, bestb, found, dist2, best_dist2;
	MASK_DATA TempFlag = 0;
	ra = rb = 0;
	for (bs=0; bs<ImageStep+ImageH; bs++) 
	{
		found = 0;
		best_dist2 = 1000000;  /* initialize to large value */
		/* search boxes of increasing size until border pixel found */		
		Box_EndX = a+bs;		
		Box_EndY = b+bs;
		if ( a < bs ) { Box_StartX = 0; }
		else { Box_StartX = a-bs; }
		if ( b < bs ) { Box_StartY = 0; }
		else { Box_StartY = b-bs; }		
		if( Box_EndX >= ImageW ) { Box_EndX = ImageW-1; }
		if( Box_EndY >= ImageH ) { Box_EndY = ImageH-1; }
		
		for (j=Box_StartY; j<=Box_EndY; j++) 
		{
			TempIndex = j*ImageStep;
			for (i=Box_StartX; i<=Box_EndX; i++) 
			{
				k = TempIndex+i;
				TempFlag = BitPtr[k];
				if ( i<=0 || i>=(ImageW-1) || j<=0 || j>=(ImageH-1) || (TempFlag & border_code)) 
				//if ( (TempFlag & border_code) ) 
				{
					found = 1;

					dist2 = 0;
					if ( j > b ) { dist2 += (j-b)*(j-b); }
					else { dist2 += (b-j)*(b-j); }

					if ( i > a ) { dist2 += (i-a)*(i-a); }
					else {	dist2 += (a-i)*(a-i); }

					if (dist2 < best_dist2) 
					{
						best_dist2 = dist2;
						besta = i;
						bestb = j;
					}
				}
			}
		}
		if (found) 
		{
			ra = besta;
			rb = bestb;
			break;
		}
	}
	return best_dist2;
}
//-------------------------------------------------------------------------------------//
bool C3dUnWrapping::GoldsteinBranchCuts(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, MASK_PTR BitPtr, MASK_PTR MaskPtr, int MaxCutLen, int NumRes, int branchcut_code, THREAD_ID ThreadID)
{
	//BranchCut 所有的極性點，達到完全Balance
	//NumRes = 極性點數目 
	if (NULL==BitPtr || NULL==MaskPtr )
	{	return false;	}  
	
	const char fnName[] = "C3dUnWrapping::GoldsteinBranchCuts";
	int *active_list = NULL;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);

	unsigned int i=0, k=0;
	unsigned int ii=0, jj=0;
	unsigned int HalfBox = 0;
	unsigned int Box_X=0, Box_Y=0;
	unsigned int Box_StartX=0, Box_EndX=0;
	unsigned int Box_StartY=0, Box_EndY=0;
	unsigned int xx=0, yy=0;
	unsigned int charge = 0;
	unsigned int TempD = 0;
	unsigned int index = 0;
	unsigned int TempIndex = 0;
	unsigned int ActiveIndex = 0;
	unsigned int min_dist=0, dist=0, rim_i=0, rim_j=0, near_i=0, near_j=0;
  	unsigned int num_active=0, max_active=0;
	MASK_DATA TempFlag = 0;
	if (MaxCutLen < 2) { MaxCutLen = 2; }
	max_active = NumRes + 32;
	
	if ( JetMemory.alloc_func(max_active, active_list, fnName, "active_list") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		return false;
	}	

	/* branch cuts */
	num_active = 0;
	for( i=0 ; i<BufferSize ; i++ )
	{
		index = i;
		TempFlag = BitPtr[index];
		if ((TempFlag & RESIDUE) && !(TempFlag & VISITED))
		{ //是極性點，且不是VISITED狀態 
			BitPtr[index] = TempFlag|VISITED|ACTIVE;		//開啟VISITED狀態
																//開啟ACTIVE狀態
			charge = (TempFlag & POS_RES) ? 1 : -1;
			num_active = 0;			
			active_list[num_active++] = index;
			
			if (num_active > max_active)
			{ num_active = max_active; }

			for( HalfBox=1 ; HalfBox<=MaxCutLen ; HalfBox++ )		
			{			
				for( k=0 ; k<num_active ; k++ )				
				{
					TempD = active_list[k];
					Box_X = TempD%ImageStep;
					Box_Y = TempD/ImageStep;
					Box_StartX = Box_X-HalfBox;
					Box_EndX = Box_X+HalfBox;
					Box_StartY = Box_Y-HalfBox;
					Box_EndY = Box_Y+HalfBox;

					if( Box_StartX < 0) { Box_StartX = 0; }
					if( Box_StartY < 0) { Box_StartY = 0; }
					if( Box_EndX >= ImageW) { Box_EndX = ImageW-1; }
					if( Box_EndY >= ImageH) { Box_EndY = ImageH-1; }					
					
					for( ii=Box_StartY ; ii<=Box_EndY ; ii++ )
					{
						TempIndex = (ii*ImageStep); 
						for( jj=Box_StartX ; jj<=Box_EndX ; jj++ )
						{
							ActiveIndex = TempIndex+jj; 	
							TempFlag = BitPtr[ActiveIndex];								
							if (jj==0 || jj==(ImageW-1) || ii==0 || ii==(ImageH-1) || (TempFlag & BORDER))
							{//如果是邊緣點
								charge = 0;
								//DistToBorder(pTempBitFlag, BORDER, Box_X, Box_Y, &xx, &yy, xsize, ysize);
								DistToBorder(ImageW, ImageH, ImageStep, BitPtr, BORDER, Box_X, Box_Y, xx, yy);
								//PlaceCut(pTempBitFlag, pMarkMap, xx, yy, Box_X, Box_Y, xsize, ysize, branchcut_code);
								PlaceCut(ImageW, ImageH, ImageStep, BitPtr, MaskPtr, xx, yy, Box_X, Box_Y, branchcut_code);
							}
							else if ((TempFlag & RESIDUE) && !(TempFlag & ACTIVE))
							{//極性點，且不是ACTIVE狀態
								if (!(TempFlag & VISITED))
								{//若不是VISITED狀態，則可加入計算
									charge += (TempFlag & POS_RES) ? 1 : -1;
									BitPtr[ActiveIndex] = TempFlag|VISITED;	//開啟VISITED狀態
									TempFlag = BitPtr[ActiveIndex];
								}
								active_list[num_active++] = ActiveIndex;	//加入ActiveList

								if (num_active > max_active)
								{ num_active = max_active; }

								BitPtr[ActiveIndex] = TempFlag|ACTIVE;			//開啟ACTIVE狀態
								//PlaceCut(pTempBitFlag, pMarkMap, jj, ii, Box_X, Box_Y, xsize, ysize, branchcut_code);
								PlaceCut(ImageW, ImageH, ImageStep, BitPtr, MaskPtr, jj, ii, Box_X, Box_Y, branchcut_code);
							}
							if (charge==0)
							{  goto continue_scan; }
						}//for jj
					}//for ii
				}//for k
			}//for box
				
			if (charge != 0)    /* connect branch cuts to rim */
			{
				min_dist = ImageW + ImageH;  /* large value */
				for (k=0; k<num_active; k++)
				{
					ii = active_list[k]%ImageStep;
					jj = active_list[k]/ImageStep;
					//if ((dist = DistToBorder(pTempBitFlag, BORDER, ii, jj, &xx, &yy, xsize, ysize)) < min_dist)
					dist = DistToBorder(ImageW, ImageH, ImageStep, BitPtr, BORDER, ii, jj, xx, yy);
					if ( dist < min_dist )
					{
						min_dist = dist;
						near_i = ii;
						near_j = jj;
						rim_i = xx;
						rim_j = yy;
					}
				}
				//PlaceCut(pTempBitFlag, pMarkMap, near_i, near_j, rim_i, rim_j, xsize, ysize, branchcut_code);
				PlaceCut(ImageW, ImageH, ImageStep, BitPtr, MaskPtr, near_i, near_j, rim_i, rim_j, branchcut_code);
			}

			continue_scan :
			/* mark all active pixels inactive */
			for (k=0; k<num_active; k++)
			{
				ActiveIndex = active_list[k];
				TempFlag = BitPtr[ActiveIndex];
				BitPtr[ActiveIndex] = TempFlag&~ACTIVE;  //關閉ACTIVE狀態
			}
		}//if 	
	}//for i	

	JetMemory.free_func(active_list);
	return true;
}
//-------------------------------------------------------------------------------------//
bool C3dUnWrapping::UnwrapAroundCuts(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const SPACE_PTR PhasePtr, MASK_PTR BitPtr, MASK_PTR MaskPtr, SPACE_PTR DstPtr, int cut_code, int debug_flag, char *infile, THREAD_ID ThreadID)
{
	if ( NULL==PhasePtr || NULL==BitPtr || NULL==MaskPtr )
	{	return false; }

	const char fnName[] = "C3dUnWrapping::UnwrapAroundCuts";
	unsigned int  i, j, k, a, b, c, num_pieces=0;
	unsigned int  num_index, max_list_size;//, bench, benchout;
	int    unwrapped_code=UNWRAPPED, postponed_code = POSTPONED;
	int    avoid_code;	
	MASK_DATA TempFlag = 0;
	int    *index_list = NULL;
	max_list_size = ImageAPI.CalcBufferSize(ImageStep, ImageH); /* this size may be reduced */
	if ( JetMemory.alloc_func(max_list_size, index_list, fnName, "index_list") == false )
	{
		this->m_ErrorString = JetMemory.GetErrorString();
		return false;
	}

	avoid_code = cut_code | unwrapped_code | BORDER;
	num_index = 0;
	unsigned int index = 0;
	for (j=0; j<ImageH; j++)
	{
		index = j*ImageStep;
		for (i=0; i<ImageW; i++)
		{
			k = index+i;
			TempFlag = BitPtr[k];

			if (!(TempFlag & avoid_code))
			{
				BitPtr[k] = TempFlag | unwrapped_code;
				TempFlag = BitPtr[k];
				//if (TempFlag & postponed_code)
				/* DstPtr[k] already stores the unwrapped value */
			//	{ value = DstPtr[k]; }
			//	else
			//	{
				//	++num_pieces;
					DstPtr[k] = PhasePtr[k];					
					MaskPtr[k] = UNWRAP_MARK;
			//	}

				//四相鄰，非cut_code | unwrapped_code | BORDER 的pixel位置 插入index_list
				//UpdateList(i, j, k, soln[k], phase, soln, pTempBitFlag, pMarkMap, xsize, ysize , 
				//	index_list, num_index, avoid_code, unwrapped_code, postponed_code, 
				//	max_list_size);
				UpdateList(ImageW, ImageH, ImageStep, PhasePtr, BitPtr, MaskPtr, i, j, k, DstPtr[k], DstPtr, index_list, num_index, avoid_code, unwrapped_code, postponed_code, max_list_size);

				while (num_index > 0)
				{
				//取出index_list的最後一筆，並將num_index 減 1，移至下一筆
				//	if (!GetNextOneToUnwrap(a, b, index_list, num_index, xsize, ysize))
				//	{ break;  }   /// no more to unwrap 

					if (num_index < 1)
					{ break; } 
					c = index_list[--num_index];
					a = c%ImageStep;
					b = c/ImageStep;
					BitPtr[c] |= unwrapped_code;   //設為已經 unwrapping 的狀態					

				//四相鄰，非cut_code | unwrapped_code | BORDER 的pixel位置 插入index_list
					//UpdateList(a, b, c, soln[c], phase, soln, pTempBitFlag, pMarkMap, xsize, ysize , 
					//	index_list, num_index, avoid_code, unwrapped_code, postponed_code,
					//	max_list_size);
					UpdateList(ImageW, ImageH, ImageStep, PhasePtr, BitPtr, MaskPtr, a, b, c, DstPtr[c], DstPtr, index_list, num_index, avoid_code, unwrapped_code, postponed_code, max_list_size);
				}
			}
		}
	}

	index = 0;
	/* unwrap branch cut pixels */	
	for (j=1; j<ImageH; j++)
	{
		index += ImageStep;
		for (i=1; i<ImageW; i++)
		{
			k = index+i;
			if (BitPtr[k] & cut_code)
			{
				if (!(BitPtr[k-1] & cut_code))
				{
					DstPtr[k] = DstPtr[k-1] + Gradient(PhasePtr[k], PhasePtr[k-1]);
					MaskPtr[k] = UNWRAP_CUT_MARK;
				}
				else if (!(BitPtr[k-ImageStep] & cut_code))
				{
					DstPtr[k] = DstPtr[k-ImageStep] + Gradient(PhasePtr[k], PhasePtr[k-ImageStep]);
					MaskPtr[k] = UNWRAP_CUT_MARK;
				}
			}
		}
	}

/*
	pTempBitFlag = pBitFlag;
	int size = xsize*ysize;
	for( i=0 ; i<size ; i++ )
	{
		if( (pTempBitFlag[i] & AVOID) ) 
		{
			pTempBitFlag[i] = BORDER;
		}
		else
		{
			pTempBitFlag[i] = UNWRAPPED;
		}	
	}
*/

  //return num_pieces;	
	JetMemory.free_func(index_list);
	return true;
}
//-------------------------------------------------------------------------------------//
bool C3dUnWrapping::UpdateList(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const SPACE_PTR PhasePtr, MASK_PTR BitPtr, MASK_PTR MaskPtr, unsigned int x, unsigned int y, unsigned int Index, SPACE_DATA val, SPACE_PTR DstPtr, int *index_list, unsigned int &num_index, int ignore_code, int processed_code, int postponed_code, unsigned int max_list_size)
{
	// InserList 會 加入Processed狀態，刪除postponed 的狀態	
	unsigned int index = 0;
	SPACE_DATA  grad;	
	MASK_DATA TempFlag = ignore_code | processed_code;
	if ( x>0 && !(BitPtr[Index-1] & TempFlag))
	{	
		if( max_list_size <= num_index )
		{
			this->m_ErrorString.Format(_T("Error, out of the list buffer!(%d)"), num_index);
			return false;
		}
		grad = Gradient(PhasePtr[Index-1], PhasePtr[Index]);
		index = Index-1; 
		DstPtr[index] = val + grad; 
		BitPtr[index] |= processed_code;
		index_list[num_index++] = index;
//		if( MaskPtr[index] & CUT_MARK )
//		{ MaskPtr[index] = UNWRAP_CUT_MARK; }
		MaskPtr[index] = UNWRAP_MARK;
//	DstPtr[index] = phase[index];
	}

	if ( x<(ImageW-1) && !(BitPtr[Index+1] & TempFlag))
	{
		if( max_list_size <= num_index )
		{
			this->m_ErrorString.Format(_T("Error, out of the list buffer!(%d)"), num_index);
			return false;
		}
		grad =  Gradient(PhasePtr[Index], PhasePtr[Index+1]);
		index = Index+1; 
		DstPtr[index] = val - grad; 
		BitPtr[index] |= processed_code;
		index_list[num_index++] = index;
//		if( MaskPtr[index] & CUT_MARK )
//		{ MaskPtr[index] = UNWRAP_CUT_MARK; }
		MaskPtr[index] = UNWRAP_MARK;
//	DstPtr[index] = phase[index];
	}

	if ( y>0 && !(BitPtr[Index-ImageStep] & TempFlag))
	{
		if( max_list_size <= num_index )
		{
			this->m_ErrorString.Format(_T("Error, out of the list buffer!(%d)"), num_index);
			return false;
		}
		grad = Gradient(PhasePtr[Index-ImageStep], PhasePtr[Index]);
		index = Index-ImageStep; 
		DstPtr[index] = val + grad; 
		BitPtr[index] |= processed_code;
		index_list[num_index++] = index;
//		if( MaskPtr[index] & CUT_MARK )
//		{ MaskPtr[index] = UNWRAP_CUT_MARK; }
		MaskPtr[index] = UNWRAP_MARK;
//	DstPtr[index] = phase[index];

	}

	if ( y<ImageH-1 && !(BitPtr[Index+ImageStep] & TempFlag))
	{
		if( max_list_size <= num_index )
		{
			this->m_ErrorString.Format(_T("Error, out of the list buffer!(%d)"), num_index);
			return false;
		}
		grad =  Gradient(PhasePtr[Index], PhasePtr[Index+ImageStep]);
		index = Index+ImageStep; 
		DstPtr[index] = val - grad; 
		BitPtr[index] |= processed_code;
		index_list[num_index++] = index;
//		if( MaskPtr[index] & CUT_MARK )
//		{ MaskPtr[index] = UNWRAP_CUT_MARK; }
		MaskPtr[index] = UNWRAP_MARK;
//	DstPtr[index] = phase[index];
	}
	return true;
}
//-------------------------------------------------------------------------------------//