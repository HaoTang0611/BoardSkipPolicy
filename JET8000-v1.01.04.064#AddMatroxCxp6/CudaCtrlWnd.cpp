// CudaCtrlWnd.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "CudaCtrlWnd.h"
//-------------------------------------------------------------------------------------//
#include "CudaFunc.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCudaCtrlWnd dialog
//-------------------------------------------------------------------------------------//
CCudaCtrlWnd::CCudaCtrlWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CCudaCtrlWnd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCudaCtrlWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CCudaCtrlWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCudaCtrlWnd)
	DDX_Control(pDX, CUDACTRL_PROGRESS_WND, m_ProgressWnd);
	DDX_Control(pDX, CUDACTRL_FUNCTION_COMBO, m_CudaFuncComboxWnd);
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CCudaCtrlWnd, CBaseDialog)
	//{{AFX_MSG_MAP(CCudaCtrlWnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(CUDACTRL_CALCULATE_BTN, OnCalculateBtn)
	ON_BN_CLICKED(CUDACTRL_RESET_CUDA_BTN, OnResetCudaBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCudaCtrlWnd message handlers
//-------------------------------------------------------------------------------------//
BOOL CCudaCtrlWnd::OnInitDialog() 
{
	CBaseDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CCudaCtrlWnd::InitialParameters();
	CCudaCtrlWnd::SwitchMultiLanguage();
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CCudaCtrlWnd::OnDestroy() 
{
	CBaseDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CCudaCtrlWnd::InitialParameters()
{
	int     idx = 0;
	CString str;
	const bool bShowGCMode=false;
	CAMERA_ID CameraID = PRIMARY_CAMERA_ID;	
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(CameraID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(CameraID);

	CWnd::SetDlgItemInt(CUDACTRL_IMAGE_WIDTH_EDIT, ImageW);
	CWnd::SetDlgItemInt(CUDACTRL_IMAGE_HEIGHT_EDIT, ImageH);

	//Cuda Function	
	idx = 0;
	JetAPI::ClearCombox(m_CudaFuncComboxWnd);
	
	str = _T("Solve Multi Phase 4+2 Cast x1");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_2_CAST1);
	idx ++;

	str = _T("Solve Multi Phase 4+2 Cast x2");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_2_CAST2);
	idx ++;

	str = _T("Solve Multi Phase 4+2 Cast x3");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_2_CAST3);
	idx ++;

	str = _T("Solve Multi Phase 4+2 Cast x4");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_2_CAST4);
	idx ++;
	
	str = _T("Solve Multi Phase 4+4 Cast x1");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_4_CAST1);
	idx ++;

	str = _T("Solve Multi Phase 4+4 Cast x2");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_4_CAST2);
	idx ++;

	str = _T("Solve Multi Phase 4+4 Cast x3");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_4_CAST3);
	idx ++;

	str = _T("Solve Multi Phase 4+4 Cast x4");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_4_CAST4);
	idx ++;	
	
if ( true == bShowGCMode )
{
	str = _T("Solve Multi Phase 4+4GC Cast x4");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_4GC_CAST4);
	idx ++;

	str = _T("Solve Multi Phase 4+5GC Cast x4");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_5GC_CAST4);
	idx ++;

	str = _T("Solve Multi Phase 4+6GC Cast x4");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_6GC_CAST4);
	idx ++;	
}

	str = _T("Solve Multi Phase 4+2 Cast x1 Exp2");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_2_CAST1_EXP2);
	idx ++;

	str = _T("Solve Multi Phase 4+2 Cast x2 Exp2");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_2_CAST2_EXP2);
	idx ++;

	str = _T("Solve Multi Phase 4+2 Cast x3 Exp2");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_2_CAST3_EXP2);
	idx ++;

	str = _T("Solve Multi Phase 4+2 Cast x4 Exp2");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_2_CAST4_EXP2);
	idx ++;	
	
	str = _T("Solve Multi Phase 4+4 Cast x1 Exp2");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_4_CAST1_EXP2);
	idx ++;

	str = _T("Solve Multi Phase 4+4 Cast x2 Exp2");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_4_CAST2_EXP2);
	idx ++;

	str = _T("Solve Multi Phase 4+4 Cast x3 Exp2");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_4_CAST3_EXP2);
	idx ++;

	str = _T("Solve Multi Phase 4+4 Cast x4 Exp2");
	m_CudaFuncComboxWnd.InsertString(-1, str);
	m_CudaFuncComboxWnd.SetItemData(idx, CUDA_FUNCTION_MULTI_PHASE_4_4_CAST4_EXP2);
	idx ++;	

	JetAPI::SetComboxCurSel(m_CudaFuncComboxWnd, CUDA_FUNCTION_MULTI_PHASE_4_4_CAST4);

	//Cuda Thread
	CWnd::SetDlgItemInt(CUDACTRL_THREAD_MIN_EDIT, 384);
	CWnd::SetDlgItemInt(CUDACTRL_THREAD_MAX_EDIT, 512);//GTX1060使用768會有資源不足的問題
	CWnd::SetDlgItemInt(CUDACTRL_THREAD_STEP_EDIT, 32);

	//Cuda Block
	CWnd::SetDlgItemInt(CUDACTRL_BLOCK_MIN_EDIT, 384);
	CWnd::SetDlgItemInt(CUDACTRL_BLOCK_MAX_EDIT, 1024);
	CWnd::SetDlgItemInt(CUDACTRL_BLOCK_STEP_EDIT, 24);

	CWnd::SetDlgItemInt(CUDACTRL_REPEAT_COUNT_EDIT, 10);

	m_ProgressWnd.SetPos(0);
	m_ProgressWnd.SetRange((short)0, (short)100);
}
//-------------------------------------------------------------------------------------//
void CCudaCtrlWnd::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_CUDA_CTRL_WND");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_CUDA_CTRL_WND;
	WndKey = _T("IDD_CUDA_CTRL_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = IDOK;
	NewLabelText = AOIDataDefine.GetWndOKText();
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = IDCANCEL;
	NewLabelText = AOIDataDefine.GetWndCancelText();
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//	
	WndID = CUDACTRL_IMAGE_WIDTH_LABEL;
	WndKey = _T("CUDACTRL_IMAGE_WIDTH_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CUDACTRL_IMAGE_HEIGHT_LABEL;
	WndKey = _T("CUDACTRL_IMAGE_HEIGHT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CUDACTRL_IMAGE_HEIGHT_LABEL;
	WndKey = _T("CUDACTRL_IMAGE_HEIGHT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CUDACTRL_FUNCTION_LABEL;
	WndKey = _T("CUDACTRL_FUNCTION_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CUDACTRL_THREAD_RANGE_LABEL;
	WndKey = _T("CUDACTRL_THREAD_RANGE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CUDACTRL_BLOCK_RANGE_LABEL;
	WndKey = _T("CUDACTRL_BLOCK_RANGE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	
	WndID = CUDACTRL_REPEAT_COUNT_LABEL;
	WndKey = _T("CUDACTRL_REPEAT_COUNT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CUDACTRL_CALCULATE_BTN;
	WndKey = _T("CUDACTRL_CALCULATE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	

	WndID = CUDACTRL_RESET_CUDA_BTN;
	WndKey = _T("CUDACTRL_RESET_CUDA_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);	
	//---------------------------------------------------------------------------------//	
}
//-------------------------------------------------------------------------------------//
void CCudaCtrlWnd::OnCalculateBtn() 
{
	// TODO: Add your control notification handler code here
#ifdef CUDA_USE
	const char fnName[] = "CCudaCtrlWnd::OnCalculateBtn";

	CString str, str2;	
	int j=0, k=0;
	int ThreadNumber=192;
	int BlockNumber=961;	
	size_t i=0, CalcCount=0, CudaCount=0;
	int ThreadBest=0, BlockBest=0;
	int ThreadWorst=0, BlockWorst=0;
	bool   bException = false;
	double MinTime=0, MaxTime=0, TotalTime=0, AverageTme=0, Time = 0;
	TCastParam CastParam;			
	MASK_PTR  MaskPtr=NULL;
	SPACE_PTR SpacePtr=NULL;
	PHASE_PTR ZeroPhasePtr=NULL;
	SPACE_PTR HeightFactorPtr=NULL;
	IMAGE_PTR PtrA[4]={NULL};
	IMAGE_PTR PtrB[4]={NULL};		
	LARGE_INTEGER   nEndTime;
	LARGE_INTEGER   nStartTime;
	TPhaseNoiseParam NoiseParam;	
	std::vector<CString> ResultList;
	const IMAGE_SIZE ImageW = CWnd::GetDlgItemInt(CUDACTRL_IMAGE_WIDTH_EDIT);
	const IMAGE_SIZE ImageH = CWnd::GetDlgItemInt(CUDACTRL_IMAGE_HEIGHT_EDIT);
	const IMAGE_SIZE BitCount = 8;
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	
	const int ThreadMin = CWnd::GetDlgItemInt(CUDACTRL_THREAD_MIN_EDIT);
	const int ThreadMax = CWnd::GetDlgItemInt(CUDACTRL_THREAD_MAX_EDIT);
	const int ThreadStep = CWnd::GetDlgItemInt(CUDACTRL_THREAD_STEP_EDIT);

	const int BlockMin = CWnd::GetDlgItemInt(CUDACTRL_BLOCK_MIN_EDIT);
	const int BlockMax = CWnd::GetDlgItemInt(CUDACTRL_BLOCK_MAX_EDIT);
	const int BlockStep = CWnd::GetDlgItemInt(CUDACTRL_BLOCK_STEP_EDIT);

	const size_t RepeatCount = CWnd::GetDlgItemInt(CUDACTRL_REPEAT_COUNT_EDIT);
	const int FunctionID = JetAPI::GetComboxCurSelData(m_CudaFuncComboxWnd);

	if ( JetMemory.alloc_func(BufferSize, PtrA[0], fnName, "PtrA[0]") == false ||
		 JetMemory.alloc_func(BufferSize, PtrA[1], fnName, "PtrA[1]") == false ||
		 JetMemory.alloc_func(BufferSize, PtrA[2], fnName, "PtrA[2]") == false ||
		 JetMemory.alloc_func(BufferSize, PtrA[3], fnName, "PtrA[3]") == false ||
		 JetMemory.alloc_func(BufferSize, PtrB[0], fnName, "PtrB[0]") == false ||
		 JetMemory.alloc_func(BufferSize, PtrB[1], fnName, "PtrB[1]") == false ||
		 JetMemory.alloc_func(BufferSize, PtrB[2], fnName, "PtrB[2]") == false ||
		 JetMemory.alloc_func(BufferSize, PtrB[3], fnName, "PtrB[3]") == false ||		 
		 JetMemory.alloc_func(BufferSize, MaskPtr, fnName, "MaskPtr") == false ||		 
		 JetMemory.alloc_func(BufferSize, SpacePtr, fnName, "SpacePtr") == false ||
		 JetMemory.alloc_func(BufferSize, ZeroPhasePtr, fnName, "ZeroPhasePtr") == false ||
		 JetMemory.alloc_func(BufferSize, HeightFactorPtr, fnName, "HeightFactorPtr") == false )
	{
		JetAPI::ShowMessageBox(JetMemory.GetErrorString());

		JetMemory.free_func(PtrA[0]);	JetMemory.free_func(PtrA[1]);	JetMemory.free_func(PtrA[2]);	JetMemory.free_func(PtrA[3]);
		JetMemory.free_func(PtrB[0]);	JetMemory.free_func(PtrB[1]);	JetMemory.free_func(PtrB[2]);	JetMemory.free_func(PtrB[3]);		
		JetMemory.free_func(MaskPtr);
		JetMemory.free_func(SpacePtr);
		JetMemory.free_func(ZeroPhasePtr);
		JetMemory.free_func(HeightFactorPtr);
		return;
	}
	
	for ( i=0; i<BufferSize; i++ )
	{
		ZeroPhasePtr[i] = 0;
		HeightFactorPtr[i] = 1.0f;
	}

	//建立樣板
	const bool   bVer = false;
	const double StartPhase = 0;
	const TSystemParameter &SysParam = AOIDataCollect.GetSystemParameter();
	const double P1 = SysParam.m_PhasePeriod1;
	const double P2 = SysParam.m_PhasePeriod2;
	ImageAPI.GrayImageCreateCosPattern3(ImageW, ImageH, ImageStep, PtrA[0], P1, StartPhase+0, bVer);//0		
	ImageAPI.GrayImageCreateCosPattern3(ImageW, ImageH, ImageStep, PtrA[1], P1, StartPhase+90, bVer);//90
	ImageAPI.GrayImageCreateCosPattern3(ImageW, ImageH, ImageStep, PtrA[2], P1, StartPhase+180, bVer);//180
	ImageAPI.GrayImageCreateCosPattern3(ImageW, ImageH, ImageStep, PtrA[3], P1, StartPhase+270, bVer);//270
	ImageAPI.GrayImageCreateCosPattern3(ImageW, ImageH, ImageStep, PtrB[0], P2, StartPhase+0, bVer);//0		
	ImageAPI.GrayImageCreateCosPattern3(ImageW, ImageH, ImageStep, PtrB[1], P2, StartPhase+90, bVer);//90
	ImageAPI.GrayImageCreateCosPattern3(ImageW, ImageH, ImageStep, PtrB[2], P2, StartPhase+180, bVer);//180
	ImageAPI.GrayImageCreateCosPattern3(ImageW, ImageH, ImageStep, PtrB[3], P2, StartPhase+270, bVer);//270	

	CastParam.PerA = P1;
	CastParam.PerB = P2;
	CastParam.ExpTimeA = 5000;
	CastParam.ExpTimeB = 4000;
	CastParam.ExpTimeC = 5000;
	CastParam.ExpTimeD = 4000;
	CastParam.ImageW = ImageW;
	CastParam.ImageH = ImageH;
	CastParam.ImageStep = ImageStep;
	CastParam.ImageCount = 8;
	CastParam.ZeroPhasePtr = ZeroPhasePtr;
	CastParam.HeightFactorPtr = HeightFactorPtr;	
	CastParam.PtrA1 = PtrA[0];
	CastParam.PtrA2 = PtrA[1];
	CastParam.PtrA3 = PtrA[2];
	CastParam.PtrA4 = PtrA[3];
	CastParam.PtrB1 = PtrB[0];
	CastParam.PtrB2 = PtrB[1];
	CastParam.PtrB3 = PtrB[2];
	CastParam.PtrB4 = PtrB[3];
	CastParam.PtrC1 = PtrA[0];
	CastParam.PtrC2 = PtrA[1];
	CastParam.PtrC3 = PtrA[2];
	CastParam.PtrC4 = PtrA[3];
	CastParam.PtrD1 = PtrB[0];
	CastParam.PtrD2 = PtrB[1];
	CastParam.PtrD3 = PtrB[2];
	CastParam.PtrD4 = PtrB[3];
	CastParam.HeightBuildMode = SysParam.m_PhaseConvertHeightMode;
	AOIDataCollect.GetPhaseNoiseDefineParam(NoiseParam);
	
	MaxTime = 0;
	MinTime = FLT_MAX;
	TotalTime = 0;
	CalcCount = 0;
	CudaCount = 0;
	bException = false;
	CWnd *pWnd = CWnd::GetDlgItem(CUDACTRL_INFO_EDIT);

	m_ProgressWnd.SetPos(0);
	int ProgressIdx = 0;
	double ProgressDIdx = 0;
	double ProgressStep = 0;
	const int MaxBlockCount = ((BlockMax-BlockMin)/BlockStep)+1;
	const int MaxThreadCount = ((ThreadMax-ThreadMin)/ThreadStep)+1;
	const int MaxTotalCount = MaxBlockCount*MaxThreadCount;
	if ( MaxTotalCount > 0 )
	{
		ProgressStep = 100.0;
		ProgressStep = ProgressStep/MaxTotalCount;	
	}
	for ( j=BlockMin; j<=BlockMax; j+=BlockStep )
	{
		BlockNumber = j;
		for ( k=ThreadMin; k<=ThreadMax; k+=ThreadStep )
		{
			ThreadNumber = k;
			QueryPerformanceCounter(&nStartTime);
			for ( i=0; i<RepeatCount; i++ )
			{
				CalcCount ++;
				switch ( FunctionID )
				{				
				case CUDA_FUNCTION_MULTI_PHASE_4_2_CAST1:
					CastParam.ExpCount = 1;
					CastParam.ImageCount = 6;
					CastParam.DecodeMode = DECODE_PHASE_4_2STEP_2;
					CastParam.PtrB3 = CastParam.PtrB4 = CastParam.PtrB5 = NULL;
					CastParam.PtrC1 = CastParam.PtrC2 = CastParam.PtrC3 = CastParam.PtrC4 = CastParam.PtrC5 = NULL;
					CastParam.PtrD1 = CastParam.PtrD2 = CastParam.PtrD3 = CastParam.PtrD4 = CastParam.PtrD5 = NULL;
					if ( CudaFunc.CudaSolveSpace4Frames2Period1Cast(ImageW, ImageH, ImageStep, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }
					break;
				case CUDA_FUNCTION_MULTI_PHASE_4_2_CAST2:
					CastParam.ExpCount = 1;
					CastParam.ImageCount = 6;
					CastParam.DecodeMode = DECODE_PHASE_4_2STEP_2;
					CastParam.PtrB3 = CastParam.PtrB4 = CastParam.PtrB5 = NULL;
					CastParam.PtrC1 = CastParam.PtrC2 = CastParam.PtrC3 = CastParam.PtrC4 = CastParam.PtrC5 = NULL;
					CastParam.PtrD1 = CastParam.PtrD2 = CastParam.PtrD3 = CastParam.PtrD4 = CastParam.PtrD5 = NULL;
					if ( CudaFunc.CudaSolveSpace4Frames2Period2Cast(ImageW, ImageH, ImageStep, CastParam, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }
					break;
				case CUDA_FUNCTION_MULTI_PHASE_4_2_CAST3:
					CastParam.ExpCount = 1;
					CastParam.ImageCount = 6;
					CastParam.DecodeMode = DECODE_PHASE_4_2STEP_2;
					CastParam.PtrB3 = CastParam.PtrB4 = CastParam.PtrB5 = NULL;
					CastParam.PtrC1 = CastParam.PtrC2 = CastParam.PtrC3 = CastParam.PtrC4 = CastParam.PtrC5 = NULL;
					CastParam.PtrD1 = CastParam.PtrD2 = CastParam.PtrD3 = CastParam.PtrD4 = CastParam.PtrD5 = NULL;
					if ( CudaFunc.CudaSolveSpace4Frames2Period3Cast(ImageW, ImageH, ImageStep, CastParam, CastParam, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }
					break;
				case CUDA_FUNCTION_MULTI_PHASE_4_2_CAST4:
					CastParam.ExpCount = 1;
					CastParam.ImageCount = 6;
					CastParam.DecodeMode = DECODE_PHASE_4_2STEP_2;
					CastParam.PtrB3 = CastParam.PtrB4 = CastParam.PtrB5 = NULL;
					CastParam.PtrC1 = CastParam.PtrC2 = CastParam.PtrC3 = CastParam.PtrC4 = CastParam.PtrC5 = NULL;
					CastParam.PtrD1 = CastParam.PtrD2 = CastParam.PtrD3 = CastParam.PtrD4 = CastParam.PtrD5 = NULL;
					if ( CudaFunc.CudaSolveSpace4Frames2Period4Cast(ImageW, ImageH, ImageStep, CastParam, CastParam, CastParam, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }
					break;				
				case CUDA_FUNCTION_MULTI_PHASE_4_4_CAST1:
					CastParam.ExpCount = 1;
					CastParam.ImageCount = 8;
					CastParam.DecodeMode = DECODE_PHASE_4_4STEP_2;					
					CastParam.PtrC1 = CastParam.PtrC2 = CastParam.PtrC3 = CastParam.PtrC4 = CastParam.PtrC5 = NULL;
					CastParam.PtrD1 = CastParam.PtrD2 = CastParam.PtrD3 = CastParam.PtrD4 = CastParam.PtrD5 = NULL;
					if ( CudaFunc.CudaSolveSpace4Frames2Period1Cast(ImageW, ImageH, ImageStep, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }
					break;
				case CUDA_FUNCTION_MULTI_PHASE_4_4_CAST2:
					CastParam.ExpCount = 1;
					CastParam.ImageCount = 8;
					CastParam.DecodeMode = DECODE_PHASE_4_4STEP_2;
					CastParam.PtrC1 = CastParam.PtrC2 = CastParam.PtrC3 = CastParam.PtrC4 = CastParam.PtrC5 = NULL;
					CastParam.PtrD1 = CastParam.PtrD2 = CastParam.PtrD3 = CastParam.PtrD4 = CastParam.PtrD5 = NULL;
					if ( CudaFunc.CudaSolveSpace4Frames2Period2Cast(ImageW, ImageH, ImageStep, CastParam, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }
					break;
				case CUDA_FUNCTION_MULTI_PHASE_4_4_CAST3:
					CastParam.ExpCount = 1;
					CastParam.ImageCount = 8;
					CastParam.DecodeMode = DECODE_PHASE_4_4STEP_2;
					CastParam.PtrC1 = CastParam.PtrC2 = CastParam.PtrC3 = CastParam.PtrC4 = CastParam.PtrC5 = NULL;
					CastParam.PtrD1 = CastParam.PtrD2 = CastParam.PtrD3 = CastParam.PtrD4 = CastParam.PtrD5 = NULL;
					if ( CudaFunc.CudaSolveSpace4Frames2Period3Cast(ImageW, ImageH, ImageStep, CastParam, CastParam, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }
					break;
				case CUDA_FUNCTION_MULTI_PHASE_4_4_CAST4:
					CastParam.ExpCount = 1;
					CastParam.ImageCount = 8;
					CastParam.DecodeMode = DECODE_PHASE_4_4STEP_2;
					CastParam.PtrC1 = CastParam.PtrC2 = CastParam.PtrC3 = CastParam.PtrC4 = CastParam.PtrC5 = NULL;
					CastParam.PtrD1 = CastParam.PtrD2 = CastParam.PtrD3 = CastParam.PtrD4 = CastParam.PtrD5 = NULL;
					if ( CudaFunc.CudaSolveSpace4Frames2Period4Cast(ImageW, ImageH, ImageStep, CastParam, CastParam, CastParam, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }
					break;	

				case CUDA_FUNCTION_MULTI_PHASE_4_4GC_CAST4:
					CastParam.ExpCount = 1;
					CastParam.ImageCount = 8;
					CastParam.DecodeMode = DECODE_PHASE_4STEP_4GC_2;					
					CastParam.PtrC1 = CastParam.PtrC2 = CastParam.PtrC3 = CastParam.PtrC4 = CastParam.PtrC5 = NULL;
					CastParam.PtrD1 = CastParam.PtrD2 = CastParam.PtrD3 = CastParam.PtrD4 = CastParam.PtrD5 = NULL;
					if ( CudaFunc.CudaSolveSpace4Frames2Period4Cast(ImageW, ImageH, ImageStep, CastParam, CastParam, CastParam, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }
					break;
				case CUDA_FUNCTION_MULTI_PHASE_4_5GC_CAST4:
					CastParam.ExpCount = 1;
					CastParam.ImageCount = 9;
					CastParam.DecodeMode = DECODE_PHASE_4STEP_5GC_2;
					CastParam.PtrB5 = PtrB[3];
					CastParam.PtrC1 = CastParam.PtrC2 = CastParam.PtrC3 = CastParam.PtrC4 = CastParam.PtrC5 = NULL;
					CastParam.PtrD1 = CastParam.PtrD2 = CastParam.PtrD3 = CastParam.PtrD4 = CastParam.PtrD5 = NULL;
					if ( CudaFunc.CudaSolveSpace4Frames2Period4Cast(ImageW, ImageH, ImageStep, CastParam, CastParam, CastParam, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }
					break;				
				case CUDA_FUNCTION_MULTI_PHASE_4_6GC_CAST4:
					CastParam.ExpCount = 1;
					CastParam.ImageCount = 10;
					CastParam.DecodeMode = DECODE_PHASE_4STEP_6GC_2;
					CastParam.PtrB5 = PtrB[3];
					CastParam.PtrB6 = PtrB[3];
					CastParam.PtrC1 = CastParam.PtrC2 = CastParam.PtrC3 = CastParam.PtrC4 = CastParam.PtrC5 = NULL;
					CastParam.PtrD1 = CastParam.PtrD2 = CastParam.PtrD3 = CastParam.PtrD4 = CastParam.PtrD5 = NULL;
					if ( CudaFunc.CudaSolveSpace4Frames2Period4Cast(ImageW, ImageH, ImageStep, CastParam, CastParam, CastParam, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }
					break;

				case CUDA_FUNCTION_MULTI_PHASE_4_2_CAST1_EXP2:
					CastParam.ExpCount = 2;
					CastParam.ImageCount = 12;
					CastParam.DecodeMode = DECODE_PHASE_4_2STEP_2;
					CastParam.PtrB3 = CastParam.PtrB4 = CastParam.PtrB5 = NULL;
					CastParam.PtrD3 = CastParam.PtrD4 = CastParam.PtrD5 = NULL;
					if ( CudaFunc.CudaSolveSpace4Frames2Period1Cast2Exp(ImageW, ImageH, ImageStep, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }					
					break;
				case CUDA_FUNCTION_MULTI_PHASE_4_2_CAST2_EXP2:
					CastParam.ExpCount = 2;
					CastParam.ImageCount = 12;
					CastParam.DecodeMode = DECODE_PHASE_4_2STEP_2;
					CastParam.PtrB3 = CastParam.PtrB4 = CastParam.PtrB5 = NULL;
					CastParam.PtrD3 = CastParam.PtrD4 = CastParam.PtrD5 = NULL;
					if ( CudaFunc.CudaSolveSpace4Frames2Period2Cast2Exp(ImageW, ImageH, ImageStep, CastParam, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }					
					break;
				case CUDA_FUNCTION_MULTI_PHASE_4_2_CAST3_EXP2:
					CastParam.ExpCount = 2;
					CastParam.ImageCount = 12;
					CastParam.DecodeMode = DECODE_PHASE_4_2STEP_2;
					CastParam.PtrB3 = CastParam.PtrB4 = CastParam.PtrB5 = NULL;
					CastParam.PtrD3 = CastParam.PtrD4 = CastParam.PtrD5 = NULL;
					if ( CudaFunc.CudaSolveSpace4Frames2Period3Cast2Exp(ImageW, ImageH, ImageStep, CastParam, CastParam, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }					
					break;
				case CUDA_FUNCTION_MULTI_PHASE_4_2_CAST4_EXP2:
					CastParam.ExpCount = 2;
					CastParam.ImageCount = 12;
					CastParam.DecodeMode = DECODE_PHASE_4_2STEP_2;
					CastParam.PtrB3 = CastParam.PtrB4 = CastParam.PtrB5 = NULL;
					CastParam.PtrD3 = CastParam.PtrD4 = CastParam.PtrD5 = NULL;
					if ( CudaFunc.CudaSolveSpace4Frames2Period4Cast2Exp(ImageW, ImageH, ImageStep, CastParam, CastParam, CastParam, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }					
					break;
				case CUDA_FUNCTION_MULTI_PHASE_4_4_CAST1_EXP2:
					CastParam.ExpCount = 2;
					CastParam.ImageCount = 16;
					CastParam.DecodeMode = DECODE_PHASE_4_4STEP_2;
					if ( CudaFunc.CudaSolveSpace4Frames2Period1Cast2Exp(ImageW, ImageH, ImageStep, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }					
					break;
				case CUDA_FUNCTION_MULTI_PHASE_4_4_CAST2_EXP2:
					CastParam.ExpCount = 2;
					CastParam.ImageCount = 16;
					CastParam.DecodeMode = DECODE_PHASE_4_4STEP_2;
					if ( CudaFunc.CudaSolveSpace4Frames2Period2Cast2Exp(ImageW, ImageH, ImageStep, CastParam, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }					
					break;
				case CUDA_FUNCTION_MULTI_PHASE_4_4_CAST3_EXP2:
					CastParam.ExpCount = 2;
					CastParam.ImageCount = 16;
					CastParam.DecodeMode = DECODE_PHASE_4_4STEP_2;
					if ( CudaFunc.CudaSolveSpace4Frames2Period3Cast2Exp(ImageW, ImageH, ImageStep, CastParam, CastParam, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }					
					break;
				case CUDA_FUNCTION_MULTI_PHASE_4_4_CAST4_EXP2:
					CastParam.ExpCount = 2;
					CastParam.ImageCount = 16;
					CastParam.DecodeMode = DECODE_PHASE_4_4STEP_2;
					if ( CudaFunc.CudaSolveSpace4Frames2Period4Cast2Exp(ImageW, ImageH, ImageStep, CastParam, CastParam, CastParam, CastParam, NoiseParam, MaskPtr, SpacePtr, ThreadNumber, BlockNumber) == false )
					{	bException = true; }					
					break;					
				}	
				if ( true == bException )
				{	break; }
			}
			if ( true == bException )
			{	break; }
			QueryPerformanceCounter(&nEndTime);
			Time = (nEndTime.QuadPart - nStartTime.QuadPart) * 1000.0/AOIDataCollect.m_SystemFreq.QuadPart;
			TotalTime += Time;

			Time = Time/RepeatCount;
			if ( Time < MinTime )
			{
				MinTime = Time;
				BlockBest=BlockNumber;
				ThreadBest=ThreadNumber;
			}
			if ( Time > MaxTime )
			{	
				MaxTime = Time; 
				BlockWorst=BlockNumber;
				ThreadWorst=ThreadNumber;
			}
			str.Format(_T("Block:%d, Thread:%d, Time:%.2fms, Best Time:%.2fms, Worst Time:%.2f"), BlockNumber, ThreadNumber, Time, MinTime, MaxTime);
			CWnd::SetDlgItemText(CUDACTRL_INFO_EDIT, str);
			if ( NULL!=pWnd && pWnd->GetSafeHwnd()!=NULL )
			{	pWnd->RedrawWindow(); }

			//str.Format(_T("Index:%d, Block:%d, Thread:%d, Time:%.2fms\n"), CudaCount+1, BlockNumber, ThreadNumber, Time);
			str.Format(_T("%d, %d, %d, %.2f\n"), CudaCount+1, BlockNumber, ThreadNumber, Time);
			ResultList.push_back(str);
			CudaCount ++;
			
			ProgressDIdx += ProgressStep;
			ProgressIdx = (int)(ProgressDIdx);
			if ( ProgressIdx > 100 ) { ProgressIdx = 100; }
			if ( m_ProgressWnd.GetPos() != ProgressIdx )
			{
				DWORD dwTimeMs = 10;
				BOOL  SkipMsg = TRUE;
				HWND  hWnd = GetSafeHwnd();
				JetAPI::SleepMessage(dwTimeMs, SkipMsg, hWnd);
				m_ProgressWnd.SetPos(ProgressIdx);	
			}

		}
		if ( true == bException )
		{	break;	}
	}

	if ( true == bException )
	{
		str2 = CudaFunc.GetErrorString();
		str.Format(_T("Block:%d, Thread:%d, %s"), BlockNumber, ThreadNumber, str2);
		CWnd::SetDlgItemText(CUDACTRL_INFO_EDIT, str);
		str.Format(_T("%d, %d, %d, %.2f\n"), -1, BlockNumber, ThreadNumber, Time);
		//str.Format(_T("Error, Block:%d, Thread:%d, Time:%.2fms\n"), BlockNumber, ThreadNumber, Time);
		ResultList.push_back(str);
	}
	else
	{
		m_ProgressWnd.SetPos(100);
		if ( CalcCount > 0 ) 
		{	AverageTme = TotalTime/CalcCount; }
		else 
		{	AverageTme = 0; }
		str.Format(_T("Total Count:%d, Worst(T:%.2f, TH:%d, BK:%d), Best(T:%.2fms, TH:%d, BK:%d)"), CalcCount, MaxTime, ThreadWorst, BlockWorst, MinTime, ThreadBest, BlockBest);
		CWnd::SetDlgItemText(CUDACTRL_INFO_EDIT, str);
		str.Format(_T("%d, %d, %d, %.2f\n"), 0, BlockBest, ThreadBest, MinTime);
		//str.Format(_T("Result, Block:%d, Thread:%d, Time:%.2fms\n"), BlockBest, ThreadBest, MinTime);
		ResultList.push_back(str);
	}

	JetMemory.free_func(PtrA[0]);
	JetMemory.free_func(PtrA[1]);
	JetMemory.free_func(PtrA[2]);
	JetMemory.free_func(PtrA[3]);
	JetMemory.free_func(PtrB[0]);
	JetMemory.free_func(PtrB[1]);
	JetMemory.free_func(PtrB[2]);
	JetMemory.free_func(PtrB[3]);
	JetMemory.free_func(MaskPtr);
	JetMemory.free_func(SpacePtr);
	JetMemory.free_func(ZeroPhasePtr);
	JetMemory.free_func(HeightFactorPtr);

	//匯出檔案
	//std::vector<CString> ResultList;
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("CudaList.TXT"));
	FILE *pfile = ::_tfopen(str, _T("w+"));
	::_ftprintf(pfile, _T("%s, %s, %s, %s\n"), _T("Index"), _T("Block"), _T("Thread"), _T("Time(ms)"));
	if ( NULL != pfile )
	{
		const size_t Len = ResultList.size();
		for ( i=0; i<Len; i++ )
		{	::_ftprintf(pfile, _T("%s"), ResultList[i]);	}
		::fclose(pfile);
		pfile = NULL;

		::ShellExecute(NULL, _T("open"), str, NULL, NULL, SW_SHOW);
	}
#endif//CUDA_USE
}
//-------------------------------------------------------------------------------------//
void CCudaCtrlWnd::OnResetCudaBtn()
{	
#ifdef CUDA_USE
	CString str;
	str = _T("Do you want to Reset Cuda Func?");
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return; }
	CudaFunc.ReleaseAll();
	CudaFunc.Initial();
	str = AOIDataDefine.GetFinishText();
	JetAPI::ShowMessageBox(str);
#endif//CUDA_USE
}
//-------------------------------------------------------------------------------------//