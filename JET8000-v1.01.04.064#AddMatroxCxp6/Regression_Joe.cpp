//----------------------------------------------------------------------------------------------//
//---------------- Version 1.0.0.4 ---- Author : Joe ----------------------- Date : 20220808 ---//
//----------------------------------------------------------------------------------------------//
/*
Visual studio 2015
Based on openCV(2.4.13.6)
*/

#include "stdafx.h"
#include "Regression_Joe.h"
#include <omp.h>

#pragma region Environment Setting(OpenCV 2.4.13.6)
//Please set the correct include and lib for OpenCV by your environment.

//Please set the "Preprocessor Directive(前置處理器定義)" for OpenCV by your environment. default : _ALREADYLOAD_OPENCV_LIB_2_4_13_6
#define _ALREADYLOAD_OPENCV_LIB_2_4_13_6

#ifndef _ALREADYLOAD_OPENCV_LIB_2_4_13_6
#include <opencv.hpp>
#ifdef _DEBUG
#pragma comment(lib,"opencv_core2413d.lib")
#pragma comment(lib,"opencv_highgui2413d.lib")
#pragma comment(lib,"opencv_imgproc2413d.lib")
#pragma comment(lib,"opencv_ml2413d.lib")
#pragma comment(lib,"opencv_photo2413d.lib")
#pragma comment(lib,"opencv_calib3d2413d.lib")
#pragma comment(lib,"opencv_objdetect2413d.lib")
#else
#pragma comment(lib,"opencv_core2413.lib")
#pragma comment(lib,"opencv_highgui2413.lib")
#pragma comment(lib,"opencv_imgproc2413.lib")		
#pragma comment(lib,"opencv_objdetect2413.lib")
#pragma comment(lib,"opencv_ml2413.lib")
#pragma comment(lib,"opencv_photo2413.lib")
#pragma comment(lib,"opencv_calib3d2413.lib")
#endif
#else
#include "OpenCV_Def.h"
#endif
#pragma endregion

#pragma region Dynamic Func Get/Set
namespace DynamicFunc_Regression
{
	double(*GetVal)(const void*, size_t shift);
	void(*SetVal)(void*, size_t shift, double val);

	inline double getPointer_double(const void* p, size_t shift) { return *((double*)p + shift); }
	inline double getPointer_float(const void* p, size_t shift) { return *((float*)p + shift); }
	inline double getPointer_int(const void* p, size_t shift) { return *((int*)p + shift); }
	inline double getPointer_short(const void* p, size_t shift) { return *((short*)p + shift); }
	inline double getPointer_ushort(const void* p, size_t shift) { return *((unsigned short*)p + shift); }
	inline double getPointer_char(const void* p, size_t shift) { return *((char*)p + shift); }
	inline double getPointer_uchar(const void* p, size_t shift) { return *((unsigned char*)p + shift); }

	inline void setPointer_double(void* p, size_t shift, double val) { *(((double*)p) + shift) = val; }
	inline void setPointer_float(void* p, size_t shift, double val) { *((float*)p + shift) = (float)val; }
	inline void setPointer_int(void* p, size_t shift, double val) { *((short*)p + shift) = (int)(val + 0.5); }
	inline void setPointer_short(void* p, size_t shift, double val) { *((short*)p + shift) = (short)(val + 0.5); }
	inline void setPointer_ushort(void* p, size_t shift, double val) { *((unsigned short*)p + shift) = (unsigned short)(val + 0.5); }
	inline void setPointer_char(void* p, size_t shift, double val) { *((char*)p + shift) = (char)(val + 0.5); }
	inline void setPointer_uchar(void* p, size_t shift, double val) { *((unsigned char*)p + shift) = (unsigned char)(val + 0.5); }

	//Return sizeof(DataType)
	int InitialFunc_GetSet(int type)
	{
		int dataSize = -1;
		switch (type % 8)
		{
		default:
		case CV_8U:
			GetVal = getPointer_uchar;
			SetVal = setPointer_uchar;
			dataSize = sizeof(unsigned char);
			break;
		case CV_8S:
			GetVal = getPointer_char;
			SetVal = setPointer_char;
			dataSize = sizeof(unsigned char);
			break;
		case CV_16U:
			GetVal = getPointer_ushort;
			SetVal = setPointer_ushort;
			dataSize = sizeof(short);
			break;
		case CV_16S:
			GetVal = getPointer_short;
			SetVal = setPointer_short;
			dataSize = sizeof(short);
			break;
		case CV_32S:
			GetVal = getPointer_int;
			SetVal = setPointer_int;
			dataSize = sizeof(int);
			break;
		case CV_32F:
			GetVal = getPointer_float;
			SetVal = setPointer_float;
			dataSize = sizeof(float);
			break;
		case CV_64F:
			GetVal = getPointer_double;
			SetVal = setPointer_double;
			dataSize = sizeof(double);
			break;
		}
		return dataSize;
	}
	auto InitialFunc_Get(int type) -> double(*)(const void*, size_t shift)
	{
		double(*func_get)(const void*, size_t shift);
		switch (type % 8)
		{
		default:
		case CV_8U:
			func_get = getPointer_uchar;
			break;
		case CV_8S:
			func_get = getPointer_char;
			break;
		case CV_16U:
			func_get = getPointer_ushort;
			break;
		case CV_16S:
			func_get = getPointer_short;
			break;
		case CV_32S:
			func_get = getPointer_int;
			break;
		case CV_32F:
			func_get = getPointer_float;
			break;
		case CV_64F:
			func_get = getPointer_double;
			break;
		}
		return func_get;
	}
	auto InitialFunc_Set(int type) -> void(*)(void*, size_t shift, double val)
	{
		void(*func_set)(void*, size_t shift, double val);
		switch (type % 8)
		{
		default:
		case CV_8U:
			func_set = setPointer_uchar;
			break;
		case CV_8S:
			func_set = setPointer_char;
			break;
		case CV_16U:
			func_set = setPointer_ushort;
			break;
		case CV_16S:
			func_set = setPointer_short;
			break;
		case CV_32S:
			func_set = setPointer_int;
			break;
		case CV_32F:
			func_set = setPointer_float;
			break;
		case CV_64F:
			func_set = setPointer_double;
			break;
		}
		return func_set;
	}
};
#pragma endregion

#pragma region SPI(From Chain)
namespace SPI_UnWrapping
{
	enum EUnWrapStaus
	{
		EPOS_RES = 0x01, /* 1st bit */
		ENEG_RES = 0x02, /* 2nd bit */
		EVISITED = 0x04, /* 3rd bit */
		EACTIVE = 0x08, /* 4th bit */
		EBRANCH_CUT = 0x10, /* 5th bit */
		EBORDER = 0x20, /* 6th bit */	//相位無效
		EUNWRAPPED = 0x40, /* 7th bit */
		EPOSTPONED = 0x80, /* 8th bit */
		ERESIDUE = (EPOS_RES | ENEG_RES),
		EAVOID = (EBRANCH_CUT | EBORDER),
	};
	enum EUnWrapMark
	{
		EUNWRAP_MARK = 0x10,   /* 5th bit */
		EUNWRAP_P_MARK = 0x11,
		EUNWRAP_N_MARK = 0x12,
		EUNWRAP_DIPO_MARK = 0x14,
		EUNWRAP_CUT_MARK = 0x18,
	};
	enum EPlaceCut
	{
		EPOS_MARK = 0x01,   /* 1st bit */
		ENEG_MARK = 0x02,   /* 2nd bit */
		EDIPOLE_MARK = 0x04,   /* 3rd bit */
		ECUT_MARK = 0x08,   /* 4th bit */
	};
	enum EPhaseStatus
	{
		EBLOCK_MASK_LOW_CONTRAST = 0x01, //低對比
		EBLOCK_MASK_OVER_SATURATE = 0x02,    //過飽合
		EBLOCK_MASK_EXTEND_AVOID = 0x04, //外擴無效點
		EBLOCK_MASK_PHASE3D_ERR = 0x08,  //Phase3D超出範圍
		EBLOCK_MASK_DARK = 0x10, //暗點
		EBLOCK_MASK_BOARD = 0x20, //Board	
		EBLOCK_MASK_LEVELING = 0x40, //Leveling	
		EBLOCK_MASK_VALID = 0x80,	//有效點
	};
	enum EUnWrapNoise
	{
		EUNWRAP_DATA_AVOID_PHASE_LOW_CONTRAST,
		EUNWRAP_DATA_AVOID_PHASE_OVER_SATURATION,
		EUNWRAP_DATA_AVOID_PHASE_EXTEND_AVOID,
		EUNWRAP_DATA_AVOID_PHASE_OTHER_AVOID,
		EUNWRAP_DATA_AVOID_PHASE,
		EUNWRAP_DATA_AVOID_BRANCH_CUT,
	};

	class CUnWrapping_SPI
	{
	public:
		std::string m_ErrorString;

		//From Chia(CUnWrapping_CS)
		bool DataUnWrapping_GOLDSTEIN(float *pPhase, unsigned char *pPhaseMask, const int ImageW, const int ImageH, float *&pUnWrappingData, const int ThreadNO, int SubImageW, int SubImageH)
		{
			/*
			in,
			pPhase: 0~1 的正規化數據
			pPhaseMask: NULL
			out,
			pUnWrappingData: Unwrap Result Data.
			*/
			int ImageW_buffer = ImageW;
			int ImageH_buffer = ImageH;
			if (SubImageW > 0 || SubImageH > 0)
			{
				ImageW_buffer = ImageW_buffer;
				ImageH_buffer = ImageH_buffer;
			}
			//-------Joe st-------
			cv::Mat BitFlag = cv::Mat(ImageH, ImageW, CV_8UC1);
			cv::Mat MarkMap = cv::Mat(ImageH, ImageW, CV_8UC1);
			//-------Joe ed-------
			int OldImageW = 0, OldImageH = 0;
			int NumRes = 0, MaxCutLen = 0;
			float *pPhaseData = pPhase;
			unsigned char *pPhaseMaskMap = pPhaseMask;
			unsigned char *pBitFlag = (unsigned char *)BitFlag.datastart;//NULL
			unsigned char *pMarkMap = (unsigned char *)MarkMap.datastart;//NULL
																		 //pBitFlag = GetFlagMap(OldImageW, OldImageH, ThreadNO);
																		 //pMarkMap = GetMarkMap(OldImageW, OldImageH, ThreadNO);
			const int Size_buffer = ImageW_buffer*ImageH_buffer;
			const int Size = ImageW*ImageH;

			::memset(pMarkMap, 0x00, sizeof(unsigned char)*Size_buffer);
			::memset(pBitFlag, 0x00, sizeof(unsigned char)*Size_buffer);
			//UnwrapMap Size 保留
			::memset(pUnWrappingData, 0, sizeof(float)*Size);

			//標記無效點<EBORDER>(計算相位時得到的無效點)
			if (MaskRegion(pPhaseData, pBitFlag, EBORDER, ImageW_buffer, ImageH_buffer) == false) { return false; }

			//標記不連續點(極性點+、-)<EPOS_RES, ENEG_RES>
			if (Residues(pPhaseData, pBitFlag, EPOS_RES, ENEG_RES, EBORDER, ImageW_buffer, ImageH_buffer, NumRes) == false) { return false; }

			//Generate Branch Cuts//
			//相鄰極性點插入BranchCuts平衡極性<EBRANCH_CUT>
			if (Dipole(pBitFlag, pMarkMap, ImageW_buffer, ImageH_buffer, EBRANCH_CUT) == false) { return false; }

			if (MaxCutLen == 0) { MaxCutLen = (int)((ImageW_buffer + ImageH_buffer)*0.5); }

			//所有極性點插入BranchCuts平衡極性<EBRANCH_CUT>
			if (GoldsteinBranchCuts(pBitFlag, pMarkMap, MaxCutLen, NumRes, ImageW_buffer, ImageH_buffer, EBRANCH_CUT, ThreadNO) == false) { return false; }

			if (UnwrapAroundCuts(pPhaseData, pBitFlag, pMarkMap, pUnWrappingData, ImageW_buffer, ImageH_buffer, EAVOID, 0, NULL, ThreadNO) == false) { return false; }

			//unsigned char *pFlag = GetFlagMap(ThreadNO);
			//unsigned char *pMark = GetMarkMap(ThreadNO);
			float *pUnwrap = pUnWrappingData;
			pUnwrap = pUnWrappingData;
			unsigned char PhaseMaskData = 0;
			int index_phasemask = 0;
			int index_unwrap = 0;
			for (int i = 0; i < ImageH_buffer; i++)
			{
				for (int j = 0; j < ImageW_buffer; j++)
				{
					if (pPhaseMaskMap != NULL)
					{
						PhaseMaskData = pPhaseMaskMap[index_phasemask + j];
						if (!(PhaseMaskData&EBLOCK_MASK_VALID))		//無效點
						{
							if (PhaseMaskData&EBLOCK_MASK_LOW_CONTRAST)
							{
								pUnwrap[index_unwrap + j] = EUNWRAP_DATA_AVOID_PHASE_LOW_CONTRAST;
								continue;
							}
							else if (PhaseMaskData&EBLOCK_MASK_OVER_SATURATE)
							{
								pUnwrap[index_unwrap + j] = EUNWRAP_DATA_AVOID_PHASE_OVER_SATURATION;
								continue;
							}
							else if (PhaseMaskData&EBLOCK_MASK_EXTEND_AVOID)
							{
								pUnwrap[index_unwrap + j] = EUNWRAP_DATA_AVOID_PHASE_EXTEND_AVOID;
								continue;
							}
							pUnwrap[index_unwrap + j] = EUNWRAP_DATA_AVOID_PHASE_OTHER_AVOID;
							continue;
						}
					}
					//if ((pFlag[index_unwrap + j] & EBORDER))	//標記無效點
					if ((pBitFlag[index_unwrap + j] & EBORDER))
					{
						pUnwrap[index_unwrap + j] = EUNWRAP_DATA_AVOID_PHASE;
						continue;
					}
					//if ((pMark[index_unwrap + j] & EUNWRAP_MARK) == 0)	//標記無效點
					if ((pMarkMap[index_unwrap + j] & EUNWRAP_MARK) == 0)
					{
						pUnwrap[index_unwrap + j] = EUNWRAP_DATA_AVOID_BRANCH_CUT;
						continue;
					}
				}
				index_phasemask += ImageW;
				index_unwrap += ImageW_buffer;
			}
			return true;
		}

	private:
		bool MaskRegion(const float *pPhase, unsigned char *pBitFlag, int avoid_code, int xsize, int ysize)
		{
			if (pPhase == NULL) { return false; }
			int i = 0;
			int size = xsize*ysize;
			const float *pPhaseData = pPhase;
			unsigned char *pTempBitFlag = pBitFlag;
			for (i = 0; i < size; i++)
			{
				if (*pPhaseData < 0)
				{
					pTempBitFlag[i] = pTempBitFlag[i] | avoid_code;
				}
				pPhaseData++;
			}
			return true;
		}
		//--------------------------------------------------------------------//
		bool Residues(const float *pPhase, unsigned char *pBitFlag, int posres_code,
			int negres_code, int avoid_code, int xsize, int ysize, int &NumRes)
		{
			NumRes = 0;
			if (pPhase == NULL)
			{
				m_ErrorString = "Phase Data Pointer NULL!";
				return false;
			}
			if (pBitFlag == NULL)
			{
				m_ErrorString = "Bit Flag Pointer NULL!";
				return false;
			}

			unsigned char *pTempBitFlag = pBitFlag;

			const int Size = xsize*ysize;
			int Index = -xsize;
			int Y = ysize - 1;
			int X = xsize - 1;
			int i = 0, j = 0, k = 0;
			float  r = 0.0f;
			float r1 = 0.0f, r2 = 0.0f, r3 = 0.0f, r4 = 0.0f;
			float *TempRList = new float[xsize];
			float TempR = 0.0f;
			for (i = 0; i < Y; i++)
			{
				Index += xsize;
				for (j = 0; j < X; j++)
				{
					k = Index + j;
					if ((pTempBitFlag[k] & avoid_code) || (pTempBitFlag[k + 1] & avoid_code)
						|| (pTempBitFlag[k + 1 + xsize] & avoid_code) || (pTempBitFlag[k + xsize] & avoid_code))
					{
						continue; // masked region: don't unwrap
					}

					if (i > 0)
					{
						if (j > 0)
						{
							r1 = -TempRList[j];
							r4 = -TempR;
						}
						else
						{
							r1 = -TempRList[j];
							r4 = Gradient(pPhase[k], pPhase[k + xsize]);
						}
					}
					else
					{
						if (j > 0)
						{
							r4 = -TempR;
						}
						else
						{
							r4 = Gradient(pPhase[k], pPhase[k + xsize]);
						}
						r1 = Gradient(pPhase[k + 1], pPhase[k]);
					}
					r2 = Gradient(pPhase[k + 1 + xsize], pPhase[k + 1]);
					r3 = Gradient(pPhase[k + xsize], pPhase[k + 1 + xsize]);
					TempRList[j] = r3;
					TempR = r2;

					r = r1 + r2 + r3 + r4;
					if (r > 0.01f)
					{
						//m_MarkMap[k] = EPOS_MARK; 
						pTempBitFlag[k] = pTempBitFlag[k] | posres_code;
						if (r > 0.1f)
						{
							NumRes++;
						}
					}
					else if (r < -0.01f)
					{
						//m_MarkMap[k] = ENEG_MARK; 
						pTempBitFlag[k] = pTempBitFlag[k] | negres_code;
						if (r < -0.1f)
						{
							NumRes++;
						}
					}
				}
			}
			delete[]TempRList; TempRList = NULL;
			return true;
		}
		//--------------------------------------------------------------------//
		float Gradient(float p1, float p2)
		{
			float  r;
			r = p1 - p2;
			if (r > 0.5f)
			{
				r -= 1.0f;
			}
			if (r < -0.5f)
			{
				r += 1.0f;
			}
			return r;
		}
		//--------------------------------------------------------------------//
		bool Dipole(unsigned char *pBitFlag, unsigned char *pMarkMap, int xsize, int ysize, int branchcut_code)
		{
			if (pBitFlag == NULL)
			{
				m_ErrorString = "Bit Flag Pointer NULL!";
				return false;
			}
			const int Size = xsize*ysize;
			int i = 0, j = 0;
			int index = -1;
			int indexEnd = 0;
			int xx = 0, yy = 0;

			unsigned char *pTempBitFlag = pBitFlag;
			unsigned char TempFlag = 0;
			for (j = 0; j < ysize; j++)
			{
				for (i = 0; i < xsize; i++)
				{
					index++;
					indexEnd = 0;
					TempFlag = pTempBitFlag[index];
					xx = i;
					yy = j;
					if (TempFlag & EPOS_RES)
					{
						if (i < xsize - 1 && (pTempBitFlag[index + 1] & ENEG_RES))
						{
							indexEnd = index + 1;
							xx = i + 1;
						}
						else if (j < ysize - 1)
						{
							if ((pTempBitFlag[index + xsize] & ENEG_RES))
							{
								indexEnd = index + xsize;
								yy = j + 1;
							}
						}
					}
					else if (TempFlag & ENEG_RES)
					{
						if (i < xsize - 1 && (pTempBitFlag[index + 1] & EPOS_RES))
						{
							indexEnd = index + 1;
							xx = i + 1;
						}
						else if (j < ysize - 1)
						{
							if ((pTempBitFlag[index + xsize] & EPOS_RES))
							{
								indexEnd = index + xsize;
								yy = j + 1;
							}
						}
					}

					if (indexEnd != 0)
					{
						PlaceCut(pTempBitFlag, pMarkMap, i, j, xx, yy, xsize, ysize, branchcut_code);
						pTempBitFlag[index] &= (~(ERESIDUE));     //取消正、負極狀態 
						pTempBitFlag[indexEnd] &= (~(ERESIDUE));    //取消正、負極狀態   
					}
				}
			}
			return true;
		}
		//--------------------------------------------------------------------//
		void PlaceCut(unsigned char *pBitFlag, unsigned char *pMarkMap, int a, int b, int c, int d, int xsize, int ysize, int code)
		{
			/* residue location is upper-left corner of 4-square */

			//設定相鄰的BlachCut
			if (c > a && a > 0)
			{
				a++;
			}
			else if (c < a && c > 0)
			{
				c++;
			}

			if (d > b && b > 0)
			{
				b++;
			}
			else if (d < b && d > 0)
			{
				d++;
			}

			if (a == c && b == d)
			{
				pBitFlag[b*xsize + a] |= code;     //設為BranchCut狀態 
				pMarkMap[b*xsize + a] = ECUT_MARK;
				return;
			}

			//設定不相鄰的BlachCut
			int  i, j, m, n, istep, jstep;
			double  r;
			m = (a < c) ? c - a : a - c;
			n = (b < d) ? d - b : b - d;
			if (m > n)
			{
				istep = (a < c) ? +1 : -1;
				r = ((double)(d - b)) / ((double)(c - a));
				for (i = a; i != c + istep; i += istep)
				{
					j = (int)(b + (i - a)*r + 0.5);
					pBitFlag[j*xsize + i] |= code;     //設為BranchCut狀態
					pMarkMap[j*xsize + i] = ECUT_MARK;
				}
			}
			else   /* n < m */
			{
				jstep = (b < d) ? +1 : -1;
				r = ((double)(c - a)) / ((double)(d - b));
				for (j = b; j != d + jstep; j += jstep)
				{
					i = (int)(a + (j - b)*r + 0.5);
					pBitFlag[j*xsize + i] |= code;     //設為BranchCut狀態
					pMarkMap[j*xsize + i] = ECUT_MARK;
				}
			}
			return;
		}
		//--------------------------------------------------------------------//
		int DistToBorder(unsigned char *pBitFlag, int border_code,
			int a, int b, int *ra, int *rb, int xsize, int ysize)
		{
			int  besta, bestb, found, dist2, best_dist2;
			int  i, j, k, bs;
			int Box_StartX = 0, Box_StartY = 0, Box_EndX = 0, Box_EndY = 0;
			int TempIndex = 0;
			unsigned char TempFlag = 0;
			*ra = *rb = 0;
			for (bs = 0; bs < xsize + ysize; bs++)
			{
				found = 0;
				best_dist2 = 1000000;  /* initialize to large value */
									   /* search boxes of increasing size until border pixel found */

				Box_StartX = a - bs;
				Box_EndX = a + bs;
				Box_StartY = b - bs;
				Box_EndY = b + bs;
				if (Box_StartX < 0) { Box_StartX = 0; }
				if (Box_StartY < 0) { Box_StartY = 0; }
				if (Box_EndX >= xsize) { Box_EndX = xsize; }
				if (Box_EndY >= ysize) { Box_EndY = ysize; }
				TempIndex = Box_StartY*xsize - xsize;
				for (j = Box_StartY; j <= Box_EndY; j++)
				{
					TempIndex += xsize;
					for (i = Box_StartX; i <= Box_EndX; i++)
					{
						k = TempIndex + i;
						TempFlag = pBitFlag[k];
						if (i <= 0 || i >= xsize - 1 || j <= 0 || j >= ysize - 1 || (TempFlag & border_code))
						{
							found = 1;
							dist2 = (j - b)*(j - b) + (i - a)*(i - a);
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
					*ra = besta;
					*rb = bestb;
					break;
				}
			}
			return best_dist2;
		}
		//--------------------------------------------------------------------//
		bool GoldsteinBranchCuts(unsigned char *pBitFlag, unsigned char *pMarkMap, int MaxCutLen,
			int NumRes, int xsize, int ysize, int branchcut_code, const int ThreadNO)
		{	//BranchCut 所有的極性點，達到完全Balance
			//NumRes = 極性點數目 
			if (pBitFlag == NULL)
			{
				m_ErrorString = "Bit Flag Pointer NULL!";
				return false;
			}
			unsigned char *pTempBitFlag = pBitFlag;
			int *active_list = NULL;
			int Size = xsize*ysize;

			int i = 0, k = 0;
			int ii = 0, jj = 0;
			int HalfBox = 0;
			int Box_X = 0, Box_Y = 0;
			int Box_StartX = 0, Box_EndX = 0;
			int Box_StartY = 0, Box_EndY = 0;
			int xx = 0, yy = 0;
			int charge = 0;
			int TempD = 0;
			int index = -1;
			int TempIndex = 0;
			int ActiveIndex = 0;
			int min_dist = 0, dist = 0, rim_i = 0, rim_j = 0, near_i = 0, near_j = 0;
			int num_active = 0, max_active = 0;
			unsigned char TempFlag = 0;
			if (MaxCutLen < 2) { MaxCutLen = 2; }
			max_active = NumRes + 10;
			active_list = new int[max_active];

			/* branch cuts */
			for (i = 0; i < Size; i++)
			{
				index++;
				TempFlag = pTempBitFlag[index];
				if ((TempFlag & ERESIDUE) && !(TempFlag & EVISITED))
				{ //是極性點，且不是EVISITED狀態 
					pTempBitFlag[index] = TempFlag | EVISITED | EACTIVE;		//開啟EVISITED狀態
																				//開啟EACTIVE狀態
					charge = (TempFlag & EPOS_RES) ? 1 : -1;
					num_active = 0;
					active_list[num_active++] = index;

					if (num_active > max_active)
					{
						num_active = max_active;
					}

					for (HalfBox = 1; HalfBox <= MaxCutLen; HalfBox++)
					{

						for (k = 0; k < num_active; k++)
						{
							TempD = active_list[k];
							Box_X = TempD%xsize;
							Box_Y = TempD / xsize;
							Box_StartX = Box_X - HalfBox;
							Box_EndX = Box_X + HalfBox;
							Box_StartY = Box_Y - HalfBox;
							Box_EndY = Box_Y + HalfBox;

							if (Box_StartX < 0) { Box_StartX = 0; }
							if (Box_StartY < 0) { Box_StartY = 0; }
							if (Box_EndX >= xsize) { Box_EndX = xsize - 1; }
							if (Box_EndY >= ysize) { Box_EndY = ysize - 1; }

							TempIndex = Box_StartY*xsize - xsize;
							for (ii = Box_StartY; ii <= Box_EndY; ii++)
							{
								TempIndex += xsize;
								for (jj = Box_StartX; jj <= Box_EndX; jj++)
								{
									ActiveIndex = TempIndex + jj;
									TempFlag = pTempBitFlag[ActiveIndex];
									if (jj == 0 || jj == xsize - 1 || ii == 0 || ii == ysize - 1 || (TempFlag & EBORDER))
									{//如果是邊緣點
										charge = 0;
										DistToBorder(pTempBitFlag, EBORDER, Box_X, Box_Y, &xx, &yy, xsize, ysize);
										PlaceCut(pTempBitFlag, pMarkMap, xx, yy, Box_X, Box_Y, xsize, ysize, branchcut_code);
									}
									else if ((TempFlag & ERESIDUE) && !(TempFlag & EACTIVE))
									{//極性點，且不是EACTIVE狀態
										if (!(TempFlag & EVISITED))
										{//若不是EVISITED狀態，則可加入計算
											charge += (TempFlag & EPOS_RES) ? 1 : -1;
											pTempBitFlag[ActiveIndex] = TempFlag | EVISITED;	//開啟EVISITED狀態
											TempFlag = pTempBitFlag[ActiveIndex];
										}
										active_list[num_active++] = ActiveIndex;	//加入ActiveList

										if (num_active > max_active)
										{
											num_active = max_active;
										}

										pTempBitFlag[ActiveIndex] = TempFlag | EACTIVE;			//開啟EACTIVE狀態
										PlaceCut(pTempBitFlag, pMarkMap, jj, ii, Box_X, Box_Y, xsize, ysize, branchcut_code);
									}
									if (charge == 0)
									{
										goto continue_scan;
									}
								}//for jj
							}//for ii
						}//for k
					}//for box

					if (charge != 0)    /* connect branch cuts to rim */
					{
						min_dist = xsize + ysize;  /* large value */
						for (k = 0; k < num_active; k++)
						{
							ii = active_list[k] % xsize;
							jj = active_list[k] / xsize;
							if ((dist = DistToBorder(pTempBitFlag, EBORDER, ii, jj, &xx, &yy, xsize, ysize)) < min_dist)
							{
								min_dist = dist;
								near_i = ii;
								near_j = jj;
								rim_i = xx;
								rim_j = yy;
							}
						}
						PlaceCut(pTempBitFlag, pMarkMap, near_i, near_j, rim_i, rim_j, xsize, ysize, branchcut_code);
					}

				continue_scan:
					/* mark all active pixels inactive */
					for (k = 0; k < num_active; k++)
					{
						ActiveIndex = active_list[k];
						TempFlag = pTempBitFlag[ActiveIndex];
						pTempBitFlag[ActiveIndex] = TempFlag&~EACTIVE;  //關閉EACTIVE狀態
					}
				}//if 	
			}//for i

			delete active_list;
			return true;
		}
		//--------------------------------------------------------------------//
		bool UnwrapAroundCuts(const float *phase, unsigned char *pBitFlag, unsigned char *pMarkMap,
			float *soln, int xsize, int ysize, int cut_code, int debug_flag, char *infile, const int ThreadNO)
		{
			int  i, j, k, a, b, c, num_pieces = 0;
			//	float  value;
			//	float  min_qual, small_val = -1.0E+10;
			int    num_index, max_list_size;//, bench, benchout;
			int    unwrapped_code = EUNWRAPPED, postponed_code = EPOSTPONED;
			int    avoid_code;
			unsigned char *pTempBitFlag = pBitFlag;
			unsigned char TempFlag = 0;
			int    *index_list = NULL;

			//	min_qual = small_val;
			max_list_size = xsize*ysize; /* this size may be reduced */
			index_list = new int[max_list_size];

			avoid_code = cut_code | unwrapped_code | EBORDER;
			num_index = 0;
			int index = -xsize;
			for (j = 0; j < ysize; j++)
			{
				index += xsize;
				for (i = 0; i < xsize; i++)
				{
					k = index + i;
					TempFlag = pTempBitFlag[k];

					if (!(TempFlag & avoid_code))
					{
						pTempBitFlag[k] = TempFlag | unwrapped_code;
						TempFlag = pTempBitFlag[k];
						//if (TempFlag & postponed_code)
						/* soln[k] already stores the unwrapped value */
						//	{ value = soln[k]; }
						//	else
						//	{
						//	++num_pieces;
						soln[k] = phase[k];
						pMarkMap[k] = EUNWRAP_MARK;
						//	}

						//四相鄰，非cut_code | unwrapped_code | EBORDER 的pixel位置 插入index_list
						UpdateList(i, j, k, soln[k], phase, soln, pTempBitFlag, pMarkMap, xsize, ysize,
							index_list, num_index, avoid_code, unwrapped_code, postponed_code,
							max_list_size);

						while (num_index > 0)
						{
							//取出index_list的最後一筆，並將num_index 減 1，移至下一筆
							//	if (!GetNextOneToUnwrap(a, b, index_list, num_index, xsize, ysize))
							//	{ break;  }   /// no more to unwrap 

							if (num_index < 1)
							{
								break;
							}
							c = index_list[--num_index];
							a = c%xsize;
							b = c / xsize;
							pTempBitFlag[c] |= unwrapped_code;   //設為已經 unwrapping 的狀態					

																 //四相鄰，非cut_code | unwrapped_code | EBORDER 的pixel位置 插入index_list
							UpdateList(a, b, c, soln[c], phase, soln, pTempBitFlag, pMarkMap, xsize, ysize,
								index_list, num_index, avoid_code, unwrapped_code, postponed_code,
								max_list_size);
						}
					}
				}
			}

			index = 0;
			/* unwrap branch cut pixels */
			for (j = 1; j < ysize; j++)
			{
				index += xsize;
				for (i = 1; i < xsize; i++)
				{
					k = index + i;
					if (pTempBitFlag[k] & cut_code)
					{
						if (!(pTempBitFlag[k - 1] & cut_code))
						{
							soln[k] = soln[k - 1] + Gradient(phase[k], phase[k - 1]);
							pMarkMap[k] = EUNWRAP_CUT_MARK;
						}
						else if (!(pTempBitFlag[k - xsize] & cut_code))
						{
							soln[k] = soln[k - xsize] + Gradient(phase[k], phase[k - xsize]);
							pMarkMap[k] = EUNWRAP_CUT_MARK;
						}
					}
				}
			}
			delete index_list;
			return true;
		}
		//--------------------------------------------------------------------//
		bool UpdateList(const int x, const int y, const int Index, float val,
			const float *phase, float *soln, unsigned char *bitflags, unsigned char *pMarkMap,
			int xsize, int ysize, int *index_list, int &num_index,
			int ignore_code, int processed_code, int postponed_code,
			int max_list_size)
		{
			// InserList 會 加入Processed狀態，刪除postponed 的狀態
			unsigned char *pBitFlags = &bitflags[Index];
			const float *pPhase = &phase[Index];
			float  grad;
			int index = 0;
			unsigned char TempFlag = ignore_code | processed_code;

			if (x > 0 && !(*(pBitFlags - 1) & TempFlag))
			{
				if (max_list_size <= num_index)
				{
					m_ErrorString = "out of the list buffer!(" + std::to_string(num_index) + ")";
					return false;
				}
				grad = Gradient(*(pPhase - 1), *(pPhase));
				index = Index - 1;
				soln[index] = val + grad;
				bitflags[index] |= processed_code;
				index_list[num_index++] = index;
				//		if( m_MarkMap[index] & ECUT_MARK )
				//		{ m_MarkMap[index] = EUNWRAP_CUT_MARK; }
				pMarkMap[index] = EUNWRAP_MARK;
				//	soln[index] = phase[index];
			}

			if (x < xsize - 1 && !(*(pBitFlags + 1) & TempFlag))
			{
				if (max_list_size <= num_index)
				{
					m_ErrorString = "out of the list buffer!(" + std::to_string(num_index) + ")";
					return false;
				}
				grad = Gradient(*(pPhase), *(pPhase + 1));
				index = Index + 1;
				soln[index] = val - grad;
				bitflags[index] |= processed_code;
				index_list[num_index++] = index;
				//		if( m_MarkMap[index] & ECUT_MARK )
				//		{ m_MarkMap[index] = EUNWRAP_CUT_MARK; }
				pMarkMap[index] = EUNWRAP_MARK;
				//	soln[index] = phase[index];
			}

			if (y > 0 && !(*(pBitFlags - xsize) & TempFlag))
			{
				if (max_list_size <= num_index)
				{
					m_ErrorString = "out of the list buffer!(" + std::to_string(num_index) + ")";
					return false;
				}
				grad = Gradient(*(pPhase - xsize), *(pPhase));
				index = Index - xsize;
				soln[index] = val + grad;
				bitflags[index] |= processed_code;
				index_list[num_index++] = index;
				//		if( m_MarkMap[index] & ECUT_MARK )
				//		{ m_MarkMap[index] = EUNWRAP_CUT_MARK; }
				pMarkMap[index] = EUNWRAP_MARK;
				//	soln[index] = phase[index];

			}

			if (y < ysize - 1 && !(*(pBitFlags + xsize) & TempFlag))
			{
				if (max_list_size <= num_index)
				{
					m_ErrorString = "out of the list buffer!(" + std::to_string(num_index) + ")";
					return false;
				}
				grad = Gradient(*(pPhase), *(pPhase + xsize));
				index = Index + xsize;
				soln[index] = val - grad;
				bitflags[index] |= processed_code;
				index_list[num_index++] = index;
				//		if( m_MarkMap[index] & ECUT_MARK )
				//		{ m_MarkMap[index] = EUNWRAP_CUT_MARK; }
				pMarkMap[index] = EUNWRAP_MARK;
				//	soln[index] = phase[index];
			}
			return true;
		}
	};
}
#pragma endregion

void Regression_Joe::Set_MultiThreadNum(int num)
{
	this->ThreadNum_Omp = num;
}

void ProcessX(int y, void* pdst, const void* pCof, int width, int height, int numPoly, cv::Mat term1, cv::Mat mV)
{
	cv::Mat R = term1 * mV;
	for (int x = 0; x < width; x++)
	{
		double val = DynamicFunc_Regression::getPointer_double(R.datastart, 0);
		for (int ip = 1; ip <= numPoly; ip++)
		{
			val += DynamicFunc_Regression::getPointer_double(R.datastart, ip) * DynamicFunc_Regression::getPointer_double(pCof, x * (numPoly + 1) + ip);
		}
		DynamicFunc_Regression::SetVal(pdst, y * width + x, val);
	}
}
void ProcessY(int x, void* pdst, const void* pCof, int width, int height, int numPoly, cv::Mat term1, cv::Mat mV) 
{
	cv::Mat R = term1 * mV;
	for (int y = 0; y < height; y++)
	{
		double val = DynamicFunc_Regression::getPointer_double(R.datastart, 0);
		for (int ip = 1; ip <= numPoly; ip++) {
			val += DynamicFunc_Regression::getPointer_double(R.datastart, ip) * DynamicFunc_Regression::getPointer_double(pCof, y * (numPoly + 1) + ip);
		}
		DynamicFunc_Regression::SetVal(pdst, y * width + x, val);
	}
}
void Regression_Joe::Regression_Polynomial_X(void* dataStart, int width, int height, int type, int times)
{
	auto dSize = DynamicFunc_Regression::InitialFunc_GetSet(type);
	size_t length = width * height * dSize;
	int Num_Polynomial = times;
	void* cloneData = malloc(length);
	void* vecVar = malloc((times + 1) * sizeof(double));
	{
		memcpy(cloneData, dataStart, length);

		//initial	
		cv::Mat src = cv::Mat(height, width, type, cloneData).t();
		src.convertTo(src, CV_64F);

		cv::Mat mCof = cv::Mat(width, Num_Polynomial + 1, CV_64F);
		double* pCof = (double*)mCof.datastart;
		for (int x = 0; x < width; x++)
		{
			*pCof++ = 1;
			for (int ip = 0; ip < Num_Polynomial; ip++)
			{
				double val = x;
				*pCof++ = std::pow(x, ip + 1);
			}
		}
		pCof = (double*)mCof.datastart;
		cv::Mat mCofT = mCof.t();
		cv::Mat term1 = (mCofT * mCof).inv(TypeInv) * mCofT;
		void* pdst = dataStart;
		#pragma omp parallel for num_threads(this->ThreadNum_Omp)
		for (int y = 0; y < height; y++) {
			ProcessX(y, pdst, pCof, width, height, Num_Polynomial, term1, src.col(y));
		}
	}
	free(vecVar);
	free(cloneData);
}
void Regression_Joe::Regression_Polynomial_Y(void* dataStart, int width, int height, int type, int times)
{
	auto dSize = DynamicFunc_Regression::InitialFunc_GetSet(type);
	size_t length = width * height * dSize;
	int Num_Polynomial = times;
	void* cloneData = malloc(length);
	void* vecVar = malloc((times + 1) * sizeof(double));
	{
		memcpy(cloneData, dataStart, length);

		//initial
		cv::Mat src = cv::Mat(height, width, type, cloneData);
		src.convertTo(src, CV_64F);

		cv::Mat mCof = cv::Mat(height, Num_Polynomial + 1, CV_64F);
		double* pCof = (double*)mCof.datastart;
		for (int y = 0; y < height; y++)
		{
			*pCof++ = 1;
			for (int ip = 0; ip < Num_Polynomial; ip++)
			{
				*pCof++ = std::pow(y, ip + 1);
			}
		}
		pCof = (double*)mCof.datastart;
		cv::Mat mCofT = mCof.t();
		cv::Mat term1 = (mCofT * mCof).inv(TypeInv) * mCofT;
		void* pdst = dataStart;
		#pragma omp parallel for num_threads(this->ThreadNum_Omp)
		for (int x = 0; x < width; x++) {
			ProcessY(x, pdst, pCof, width, height, Num_Polynomial, term1, src.col(x));
		}
	}
	free(vecVar);
	free(cloneData);
}

void Regression_Joe::Regression_Polynomial_Block_X(void* dataStart, int width, int height, int type, int times, int size)
{
	//initial	
	cv::Mat src = cv::Mat(height, width, type, dataStart);

	//split
	auto blockSize = size;
	std::vector<cv::Mat> vecBlock, vecBlock_Deep;
	for (int i = 0; i < width; i += blockSize) {
		if (i == width) { break; }
		if ((i + blockSize) > width) {
			vecBlock.push_back(src(cv::Range(0, height), cv::Range(i, width)));
			vecBlock_Deep.push_back(src(cv::Range(0, height), cv::Range(i, width)).clone());
		}
		else {
			vecBlock.push_back(src(cv::Range(0, height), cv::Range(i, i + blockSize)));
			vecBlock_Deep.push_back(src(cv::Range(0, height), cv::Range(i, i + blockSize)).clone());
		}
	}

	//process
	for (int i = 0; i < vecBlock_Deep.size(); i++) {
		Regression_Polynomial_X(vecBlock_Deep[i].data, vecBlock_Deep[i].cols, vecBlock_Deep[i].rows, vecBlock_Deep[i].type(), times);
		vecBlock_Deep[i].copyTo(vecBlock[i]);
	}

	//release
	vecBlock.clear();
	vecBlock_Deep.clear();
}
void Regression_Joe::Regression_Polynomial_Block_Y(void* dataStart, int width, int height, int type, int times, int size)
{
	//initial	
	cv::Mat src = cv::Mat(height, width, type, dataStart);

	//split
	auto blockSize = size;
	std::vector<cv::Mat> vecBlock, vecBlock_Deep;
	for (int i = 0; i < height; i += blockSize) {
		if (i == height) { break; }
		if ((i + blockSize) > height) {
			vecBlock.push_back(src(cv::Range(i, height), cv::Range(0, width)));
			vecBlock_Deep.push_back(src(cv::Range(i, height), cv::Range(0, width)).clone());
		}
		else {
			vecBlock.push_back(src(cv::Range(i, i + blockSize), cv::Range(0, width)));
			vecBlock_Deep.push_back(src(cv::Range(i, i + blockSize), cv::Range(0, width)).clone());
		}
	}

	//process
	for (int i = 0; i < vecBlock.size(); i++) {
		Regression_Polynomial_Y(vecBlock[i].data, vecBlock[i].cols, vecBlock[i].rows, vecBlock[i].type(), times);
		vecBlock_Deep[i].copyTo(vecBlock[i]);
	}
}

void Regression_Joe::Calibrate_BasePhase(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, double maxPhase, int times)
{
	Calibrate_BasePhase(psrc, pdst, width, height, typeSrc, typeDst, maxPhase, times, times);
}
void Regression_Joe::Calibrate_BasePhase(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, double maxPhase, int timesX, int timesY)
{
	auto dSize_Src = DynamicFunc_Regression::InitialFunc_GetSet(typeSrc);
	size_t length = width * height * dSize_Src;
	void* cloneSrc = malloc(length);
	{
		//Initial
		memcpy(cloneSrc, psrc, length);
		cv::Mat tmpSrc = cv::Mat(height, width, typeSrc, cloneSrc);
		cv::Mat normalizeSrc;
		cv::Mat unwrapping = cv::Mat(height, width, CV_32F);

		//Step-1 : normalize 0-1
		cv::normalize(tmpSrc, normalizeSrc, 0, 1, CV_MINMAX, CV_32F);

		//Step-2 : get unwrapping
		SPI_UnWrapping::CUnWrapping_SPI Obj_UnWrappingSPI;
		float* psrcFloat = (float*)normalizeSrc.datastart;
		float* punwrapping = (float*)unwrapping.datastart;
		bool isSuccess = Obj_UnWrappingSPI.DataUnWrapping_GOLDSTEIN(psrcFloat, NULL, width, height, punwrapping, 0, -1, -1);

		//step-3 : get ideal unwrapping phase
		Regression_Polynomial_X(punwrapping, width, height, unwrapping.type(), timesX);
		Regression_Polynomial_Y(punwrapping, width, height, unwrapping.type(), timesY);

		//step-4 : wrapping phase & set result
		auto funcGet_unWrap = DynamicFunc_Regression::InitialFunc_Get(unwrapping.type());
		auto funcSet_dst = DynamicFunc_Regression::InitialFunc_Set(typeDst);
		#pragma omp parallel for num_threads(this->ThreadNum_Omp)
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				int i = y * width + x;
				double val = funcGet_unWrap(punwrapping, i);
				double period = std::floor(val);
				double phase = (val - period) * maxPhase;
				funcSet_dst(pdst, i, phase);
			}
		}
	}
	free(cloneSrc);
}

void Regression_Joe::Calibrate_BasePhase_Block(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, double maxPhase, int times, int blocksize)
{
	Calibrate_BasePhase_Block(psrc, pdst, width, height, typeSrc, typeDst, maxPhase, times, times, blocksize, blocksize);
}
void Regression_Joe::Calibrate_BasePhase_Block(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, double maxPhase, int timesX, int timesY, int blocksizeX, int blocksizeY)
{
	auto dSize_Src = DynamicFunc_Regression::InitialFunc_GetSet(typeSrc);
	size_t length = width * height * dSize_Src;
	void* cloneSrc = malloc(length);
	{
		//Initial
		memcpy(cloneSrc, psrc, length);
		cv::Mat tmpSrc = cv::Mat(height, width, typeSrc, cloneSrc);
		cv::Mat normalizeSrc;
		cv::Mat unwrapping = cv::Mat(height, width, CV_32F);

		//Step-1 : normalize 0-1
		cv::normalize(tmpSrc, normalizeSrc, 0, 1, CV_MINMAX, CV_32F);

		//Step-2 : get unwrapping
		SPI_UnWrapping::CUnWrapping_SPI Obj_UnWrappingSPI;
		float* psrcFloat = (float*)normalizeSrc.datastart;
		float* punwrapping = (float*)unwrapping.datastart;
		bool isSuccess = Obj_UnWrappingSPI.DataUnWrapping_GOLDSTEIN(psrcFloat, NULL, width, height, punwrapping, 0, -1, -1);

		//step-3 : get ideal unwrapping phase
		Regression_Polynomial_Block_X(punwrapping, width, height, unwrapping.type(), timesX, blocksizeX);
		Regression_Polynomial_Block_Y(punwrapping, width, height, unwrapping.type(), timesY, blocksizeY);

		//step-4 : wrapping phase & set result
		auto funcGet_unWrap = DynamicFunc_Regression::InitialFunc_Get(unwrapping.type());
		auto funcSet_dst = DynamicFunc_Regression::InitialFunc_Set(typeDst);
#pragma omp parallel for
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				int i = y * width + x;
				double val = funcGet_unWrap(punwrapping, i);
				double period = std::floor(val);
				double phase = (val - period) * maxPhase;
				funcSet_dst(pdst, i, phase);
			}
		}
	}
	free(cloneSrc);
}

void Regression_Joe::Calibrate_HeightFactor(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, int times)
{
	Calibrate_HeightFactor(psrc, pdst, width, height, typeSrc, typeDst, times, times);
}
void Regression_Joe::Calibrate_HeightFactor(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, int timesX, int timesY)
{
	auto dSize_Src = DynamicFunc_Regression::InitialFunc_GetSet(typeSrc);
	size_t length = width * height * dSize_Src;
	void* cloneSrc = malloc(length);
	{
		//Initial
		memcpy(cloneSrc, psrc, length);
		cv::Mat tmpSrc = cv::Mat(height, width, typeSrc, cloneSrc);
		void* ptmpsrc = tmpSrc.data;

		//step-1 : cal regression
		Regression_Polynomial_X(ptmpsrc, width, height, tmpSrc.type(), timesX);
		Regression_Polynomial_Y(ptmpsrc, width, height, tmpSrc.type(), timesY);

		//step-2 : set result
		auto funcGet_tmpsrc = DynamicFunc_Regression::InitialFunc_Get(tmpSrc.type());
		auto funcSet_dst = DynamicFunc_Regression::InitialFunc_Set(typeDst);
		#pragma omp parallel for num_threads(this->ThreadNum_Omp)
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				int i = y * width + x;
				double val = funcGet_tmpsrc(ptmpsrc, i);
				funcSet_dst(pdst, i, val);
			}
		}
	}
	free(cloneSrc);
}

//model : v = ax + by + c
void Regression_Joe::Regression_Linear_Plane(void* psrc, int width, int height, int typeSrc, void* pmask, int typeMask)
{
	//initial
	auto dSize = DynamicFunc_Regression::InitialFunc_GetSet(typeSrc);
	size_t length = width * height * dSize;
	int NumCof = 3;
	int Cal_dataSize = sizeof(double);
	int Cal_valType = CV_64F;
	auto GetVal_Mask = DynamicFunc_Regression::InitialFunc_Get(typeMask);
	auto GetVal_Cal = DynamicFunc_Regression::InitialFunc_Get(Cal_valType);
	void* cloneSrc = malloc(length);
	double* aCof = (double*)calloc(NumCof * NumCof,  Cal_dataSize);
	double* aV = (double*)calloc(NumCof,  Cal_dataSize);
	{
		memcpy(cloneSrc, psrc, length);
		cv::Mat src = cv::Mat(height, width, typeSrc, cloneSrc);
		src.convertTo(src, Cal_valType);
		cv::Mat mCof = cv::Mat(NumCof, NumCof, Cal_valType, aCof);
		cv::Mat mV = cv::Mat(NumCof, 1, Cal_valType, aV);
		if (pmask != NULL)
		{
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					int idCof = 0, idV = 0, idSrc = y * width + x;
					if (GetVal_Mask(pmask, idSrc) > 0)
					{
						*(aCof + idCof++) += x;
						*(aCof + idCof++) += y;
						*(aCof + idCof++) += 1;

						*(aCof + idCof++) += x * x;
						*(aCof + idCof++) += x * y;
						*(aCof + idCof++) += x;

						*(aCof + idCof++) += x * y;
						*(aCof + idCof++) += y * y;
						*(aCof + idCof++) += y;

						*(aV + idV++) += DynamicFunc_Regression::GetVal(psrc, idSrc);
						*(aV + idV++) += DynamicFunc_Regression::GetVal(psrc, idSrc) * x;
						*(aV + idV++) += DynamicFunc_Regression::GetVal(psrc, idSrc) * y;
					}
				}
			}
		}
		else
		{
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					int idCof = 0, idV = 0, idSrc = y * width + x;
					*(aCof + idCof++) += x;
					*(aCof + idCof++) += y;
					*(aCof + idCof++) += 1;

					*(aCof + idCof++) += x * x;
					*(aCof + idCof++) += x * y;
					*(aCof + idCof++) += x;

					*(aCof + idCof++) += x * y;
					*(aCof + idCof++) += y * y;
					*(aCof + idCof++) += y;

					*(aV + idV++) += DynamicFunc_Regression::GetVal(psrc, idSrc);
					*(aV + idV++) += DynamicFunc_Regression::GetVal(psrc, idSrc) * x;
					*(aV + idV++) += DynamicFunc_Regression::GetVal(psrc, idSrc) * y;
				}
			}
		}

		cv::Mat R = mCof.inv(Default_TypeInv) * mV;
		double a = this->Param_Plane.A = GetVal_Cal(R.datastart, 0);
		double b = this->Param_Plane.B = GetVal_Cal(R.datastart, 1);
		double c = this->Param_Plane.C = GetVal_Cal(R.datastart, 2);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				DynamicFunc_Regression::SetVal(psrc, y * width + x, a * x + b * y + c);
			}
		}
		R.release();
	}
	free(aCof);
	free(aV);
}

void Regression_Joe::Regression_Linear_DeSkew(void* psrc, int width, int height, int typeSrc, void* pmask, int typeMask)
{
	//initial
	auto dSize = DynamicFunc_Regression::InitialFunc_GetSet(typeSrc);
	size_t length = width * height * dSize;
	int NumCof = 3;
	int Cal_dataSize = sizeof(double);
	int Cal_valType = CV_64F;
	auto GetVal_Mask = DynamicFunc_Regression::InitialFunc_Get(typeMask);
	auto GetVal_Cal = DynamicFunc_Regression::InitialFunc_Get(Cal_valType);
	void* cloneSrc = malloc(length);
	double* aCof = (double*)calloc(NumCof * NumCof, Cal_dataSize);
	double* aV = (double*)calloc(NumCof, Cal_dataSize);
	{
		memcpy(cloneSrc, psrc, length);
		cv::Mat src = cv::Mat(height, width, typeSrc, cloneSrc);
		src.convertTo(src, Cal_valType);
		cv::Mat mCof = cv::Mat(NumCof, NumCof, Cal_valType, aCof);
		cv::Mat mV = cv::Mat(NumCof, 1, Cal_valType, aV);
		if (pmask != NULL)
		{
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					int idCof = 0, idV = 0, idSrc = y * width + x;
					if (GetVal_Mask(pmask, idSrc) > 0)
					{
						*(aCof + idCof++) += x;
						*(aCof + idCof++) += y;
						*(aCof + idCof++) += 1;

						*(aCof + idCof++) += x * x;
						*(aCof + idCof++) += x * y;
						*(aCof + idCof++) += x;

						*(aCof + idCof++) += x * y;
						*(aCof + idCof++) += y * y;
						*(aCof + idCof++) += y;

						*(aV + idV++) += DynamicFunc_Regression::GetVal(psrc, idSrc);
						*(aV + idV++) += DynamicFunc_Regression::GetVal(psrc, idSrc) * x;
						*(aV + idV++) += DynamicFunc_Regression::GetVal(psrc, idSrc) * y;
					}
				}
			}
		}
		else
		{
			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					int idCof = 0, idV = 0, idSrc = y * width + x;
					*(aCof + idCof++) += x;
					*(aCof + idCof++) += y;
					*(aCof + idCof++) += 1;

					*(aCof + idCof++) += x * x;
					*(aCof + idCof++) += x * y;
					*(aCof + idCof++) += x;

					*(aCof + idCof++) += x * y;
					*(aCof + idCof++) += y * y;
					*(aCof + idCof++) += y;

					*(aV + idV++) += DynamicFunc_Regression::GetVal(psrc, idSrc);
					*(aV + idV++) += DynamicFunc_Regression::GetVal(psrc, idSrc) * x;
					*(aV + idV++) += DynamicFunc_Regression::GetVal(psrc, idSrc) * y;
				}
			}
		}

		cv::Mat R = mCof.inv(Default_TypeInv) * mV;
		double a = this->Param_DeSkew.A = GetVal_Cal(R.datastart, 0);
		double b = this->Param_DeSkew.B = GetVal_Cal(R.datastart, 1);
		double c = this->Param_DeSkew.C = GetVal_Cal(R.datastart, 2);
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				auto idx = y * width + x;
				DynamicFunc_Regression::SetVal(psrc, idx, DynamicFunc_Regression::GetVal(psrc, idx) - (a * x + b * y + c));
			}
		}
		R.release();
	}
	free(aCof);
	free(aV);
}