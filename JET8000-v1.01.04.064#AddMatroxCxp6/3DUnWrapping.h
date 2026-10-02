// 3DUnWrapping.h: interface for the C3dUnWrapping class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_3DUNWRAPPING_H__41240C77_BCDC_4BFE_A835_B2C5373B19CB__INCLUDED_)
#define AFX_3DUNWRAPPING_H__41240C77_BCDC_4BFE_A835_B2C5373B19CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "Light3DDef.h"
//-------------------------------------------------------------------------------------//
#define POS_RES     0x01   /* 1st bit */
#define NEG_RES     0x02   /* 2nd bit */
#define VISITED     0x04   /* 3rd bit */
#define ACTIVE      0x08   /* 4th bit */
#define BRANCH_CUT  0x10   /* 5th bit */
#define BORDER      0x20   /* 6th bit */	//相位無效
#define UNWRAPPED   0x40   /* 7th bit */
#define POSTPONED   0x80   /* 8th bit */
#define RESIDUE      (POS_RES | NEG_RES)
#define AVOID        (BRANCH_CUT | BORDER)

#define POS_MARK	0x01   /* 1st bit */
#define NEG_MARK	0x02   /* 2nd bit */
#define DIPOLE_MARK	0x04   /* 3rd bit */
#define CUT_MARK	0x08   /* 4th bit */
/*
#define CUT_CUT_MARK	CUT_MARK|NEG_MARK  
#define CUT_DIP_MARK	CUT_MARK|DIPOLE_MARK 
#define CUT_PN_MARK		CUT_MARK|POS_MARK 
*/
#define UNWRAP_MARK			0x10   /* 5th bit */
#define UNWRAP_P_MARK		0x11
#define UNWRAP_N_MARK		0x12
#define UNWRAP_DIPO_MARK	0x14
#define UNWRAP_CUT_MARK		0x18
//-------------------------------------------------------------------------------------//
class C3dUnWrapping  
{
private:
	//---------------------------------------------------------------------------------//
	CString                    m_ErrorString;
	//---------------------------------------------------------------------------------//
	IMAGE_SIZE                 m_ImageW[MAX_THREAD_COUNT];
	IMAGE_SIZE                 m_ImageH[MAX_THREAD_COUNT];
	IMAGE_SIZE                 m_ImageStep[MAX_THREAD_COUNT];
	IMAGE_SIZE                 m_BitCount[MAX_THREAD_COUNT];
	
	MASK_PTR                   m_BitPtr[MAX_THREAD_COUNT];
	MASK_PTR                   m_MarkPtr[MAX_THREAD_COUNT];	
	SPACE_PTR                  m_SpacePtr[MAX_THREAD_COUNT];
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitUnWrapping();
	void                       InitialUnWrapping();
	//---------------------------------------------------------------------------------//
	C3dUnWrapping(const C3dUnWrapping &unwrapping);
	C3dUnWrapping& operator=(const C3dUnWrapping &unwrapping);
	//---------------------------------------------------------------------------------//
	bool                       CheckThreadID(THREAD_ID ThreadID);//確認執行緒編號
	//---------------------------------------------------------------------------------//	
	bool                       GetBitMarkBuffer(unsigned int &ImageW, unsigned int &ImageH, unsigned int &ImageStep, MASK_PTR &BitPtr, MASK_PTR &MarkPtr, THREAD_ID ThreadID);
	//---------------------------------------------------------------------------------//
	bool                       ExecUnWrapping_Phase(const SPACE_PTR PhasePtr, const MASK_PTR PhaseMaskPtr, unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, MASK_PTR BitPtr, MASK_PTR MarkPtr, SPACE_PTR DstPtr, THREAD_ID ThreadID);
	bool                       ExecUnWrapping_Goldstein(const SPACE_PTR PhasePtr, const MASK_PTR PhaseMaskPtr, unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, MASK_PTR BitPtr, MASK_PTR MaskPtr, SPACE_PTR DstPtr, THREAD_ID ThreadID);	                                          
	//---------------------------------------------------------------------------------//
	SPACE_DATA                 Gradient(SPACE_DATA p1, SPACE_DATA p2);
	bool                       MaskRegion(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStpe, const SPACE_PTR PhasePtr, MASK_PTR BitPtr, int avoid_code);
	bool                       Residues(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const SPACE_PTR PhasePtr, MASK_PTR BitPtr, int posres_code, int negres_code, int avoid_code, int &NumRes);
	bool                       Dipole(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, MASK_PTR BitPtr, MASK_PTR MaskPtr, int branchcut_code);
	bool                       PlaceCut(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, MASK_PTR BitPtr, MASK_PTR MaskPtr, int a, int b, int c, int d, int code);
	unsigned int               DistToBorder(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, MASK_PTR BitPtr, int border_code, unsigned int a, unsigned int b, unsigned int &ra, unsigned int &rb);
	bool                       GoldsteinBranchCuts(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, MASK_PTR BitPtr, MASK_PTR MaskPtr, int MaxCutLen, int NumRes, int branchcut_code, THREAD_ID ThreadID);	
	bool                       UnwrapAroundCuts(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const SPACE_PTR PhasePtr, MASK_PTR BitPtr, MASK_PTR MaskPtr, SPACE_PTR DstPtr, int cut_code, int debug_flag, char *infile, THREAD_ID ThreadID);
	bool                       UpdateList(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, const SPACE_PTR PhasePtr, MASK_PTR BitPtr, MASK_PTR MaskPtr, unsigned int x, unsigned int y, unsigned int Index, SPACE_DATA val, SPACE_PTR DstPtr, int *index_list, unsigned int &num_index, int ignore_code, int processed_code, int postponed_code, unsigned int max_list_size);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	C3dUnWrapping();
	virtual ~C3dUnWrapping();
	//---------------------------------------------------------------------------------//
	LPCTSTR                    GetErrorString() { return m_ErrorString; }
	//---------------------------------------------------------------------------------//
	bool                       CreateBuffer(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, THREAD_ID ThreadID=THREAD_00);//建立unwrapping所需暫存器
	bool                       ReleaseBuffer(THREAD_ID ThreadID=THREAD_00);//釋放unwrapping所需暫存器
	bool                       ReleaseAllBuffer();//釋放unwrapping所有暫存器
	//---------------------------------------------------------------------------------//
	bool                       ExecPhaseUnWrapping(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, PHASE_PTR PhasePtr, MASK_PTR MaskPtr, SPACE_PTR &DstPtr, THREAD_ID ThreadID=THREAD_00);//計算相位還原
	bool                       ExecPhaseUnWrapping3(unsigned int ImageW, unsigned int ImageH, unsigned int ImageStep, PHASE_PTR PhasePtr, MASK_PTR MaskPtr, SPACE_PTR DstPtr, THREAD_ID ThreadID=THREAD_00);//計算相位還原
	//---------------------------------------------------------------------------------//

};
//-------------------------------------------------------------------------------------//
extern C3dUnWrapping    Calc3DUnWarpping;
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_3DUNWRAPPING_H__41240C77_BCDC_4BFE_A835_B2C5373B19CB__INCLUDED_)
