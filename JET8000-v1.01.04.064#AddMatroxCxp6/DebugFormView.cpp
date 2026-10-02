// DebugFormView.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "DebugFormView.h"
//-------------------------------------------------------------------------------------//
#include "AOIFileIO.h"
#include "JetBarcode.h"
#include "DebugWnd.h"
#include "Draw3DWnd.h"
#include "ImageMaskWnd.h"
#include "MapCoordinate.h"
#include "ImagePhaseWnd.h"
#include "ImageDebugWnd.h"
#include "Light3DTiDLPWnd.h"
#include "ImagePhaseAllWnd.h"
#include "NewProjectWizardWnd.h"
#include "ModelWnd.h"
#include "ModelPropertyWnd.h"
#include "BarcodeConfirmWnd.h"
#include "BarcodeInputWnd.h"
#include "BarcodeDeviceWnd.h"
#include "ProjectColorWnd.h"
#include "ProjectMapMaskWnd.h"
#include "NewPanelWizardWnd.h"
#include "ComponentListWnd.h"
#include "LoadCadxyWnd.h"
#include "ProjectCompareWnd.h"
#include "AlgImageCompareWnd.h"
#include "AlgBarcodeRecognizeWnd.h"
#include "ProjectFieldConfigWnd.h"
#include "ITSCommWnd.h"
#include "ImageCombineWnd.h"
#include "ProjectDivideDistrictWnd.h"
#include "BoardConfigWnd.h"
#include "ComponentConfigWnd.h"
#include "KNNClassify.h"
#include "RemoteParamWnd.h"
#include "FdSortWnd.h"
#include "ProjectCodeListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CDebugFormView
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNCREATE(CDebugFormView, CFormView)
//-------------------------------------------------------------------------------------//
CDebugFormView::CDebugFormView()
	: CFormView(CDebugFormView::IDD)
{
	//{{AFX_DATA_INIT(CDebugFormView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
CDebugFormView::~CDebugFormView()
{
	//std::string tmpStr;	
	//tmpStr.data()
}
//-------------------------------------------------------------------------------------//
void CDebugFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDebugFormView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CDebugFormView, CFormView)
	//{{AFX_MSG_MAP(CDebugFormView)
	ON_WM_DESTROY()
	ON_BN_CLICKED(DEBUG_DEBUG_BTN, OnDebugBtn)
	ON_BN_CLICKED(DEBUG_NEW_PROJECT_BTN, OnNewProjectBtn)
	ON_BN_CLICKED(DEBUG_IMAGE_DEBUG_BTN, OnImageDebugBtn)
	ON_BN_CLICKED(DEBUG_VIEW3D_DEBUG_BTN, OnView3dDebugBtn)
	ON_BN_CLICKED(DEBUG_TIDLP_DEBUG_BTN, OnTiDlpDebugBtn)
	ON_BN_CLICKED(DEBUG_SAVE_BTN, OnSaveBtn)
	ON_BN_CLICKED(DEBUG_COORDINATE_BTN, OnCoordinateBtn)
	ON_BN_CLICKED(DEBUG_IMAGE_MASK_BTN, OnImageMaskBtn)
	ON_BN_CLICKED(DEBUG_ALL_PHASE_BTN, OnAllPhaseBtn)
	ON_BN_CLICKED(DEBUG_MODEL_BTN, OnModelBtn)
	ON_BN_CLICKED(DEBUG_BARCODE_DEVICE_BTN, OnBarcodeDeviceBtn)
	ON_BN_CLICKED(DEBUG_BARCODE_HANDHELD_BTN, OnBarcodeHandheldBtn)
	ON_BN_CLICKED(DEBUG_PROJECT_COLOR_BTN, OnProjectColorBtn)
	ON_BN_CLICKED(DEBUG_PROJECT_MAP_MASK_BTN, OnProjectMapMaskBtn)
	ON_BN_CLICKED(DEBUG_NEW_PANEL_BTN, OnNewPanelBtn)
	ON_BN_CLICKED(DEBUG_FULL_MAP_COMPONENT_BTN, OnFullMapComponentBtn)
	ON_BN_CLICKED(DEBUG_SAVE_SPC_FILE_BTN, OnSaveSpcFileBtn)
	ON_BN_CLICKED(DEBUG_COMPONENT_LIST_WND, OnComponentListWnd)
	ON_BN_CLICKED(DEBUG_LOAD_CADXY_BTN, OnLoadCadxyBtn)
	ON_BN_CLICKED(DEBUG_PROJECT_COMPARE_BTN, OnProjectCompareBtn)
	ON_BN_CLICKED(DEBUG_ALG_IMAGE_COMPARE_BTN, OnAlgImageCompareBtn)
	ON_BN_CLICKED(DEBUG_SAVE_MEMORY_BTN, OnSaveMemoryBtn)
	ON_BN_CLICKED(DEBUG_BUILD_XY_CALI_BTN, OnBuildXYCaliBtn)
	ON_BN_CLICKED(DEBUG_BARCODE_RECOGNIZE_BTN, OnBarcodeRecognizeBtn)
	ON_BN_CLICKED(DEBUG_FIELD_CONFIG_BTN, OnFieldConfigBtn)
	ON_BN_CLICKED(DEBUG_SOCKET_CLIENT_BTN, OnSocketClientBtn)
	ON_BN_CLICKED(DEBUG_LOAD_REPAIR_FILE_BTN, OnLoadRepairFileBtn)
	ON_BN_CLICKED(DEBUG_VERIFY_BIN_FILE_BTN, OnVerifyBinFileBtn)
	ON_BN_CLICKED(DEBUG_SAVE_LOCATION_FILE_BTN, OnSaveLocationFileBtn)
	ON_BN_CLICKED(DEBUG_COMBINE_IMAGE_BTN, OnCombineImageBtn)
	ON_BN_CLICKED(DEBUG_DIVIDE_DISTRICT_BTN, OnDivideDistrictBtn)
	ON_BN_CLICKED(DEBUG_BOARD_CONFIG_BTN, OnBoardConfigBtn)
	ON_BN_CLICKED(DEBUG_COMPONENT_CONFIG_BTN, OnComponentConfigBtn)
	ON_BN_CLICKED(DEBUG_PROJECT_CODE_LIST_BTN, OnProjectCodeListBtn)
	ON_BN_CLICKED(DEBUG_OTHER_DEBUG_BTN, OnOtherDebugBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CDebugFormView diagnostics
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CDebugFormView::AssertValid() const
{
	CFormView::AssertValid();
}
//-------------------------------------------------------------------------------------//
void CDebugFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CDebugFormView message handlers
//-------------------------------------------------------------------------------------//
void CDebugFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
//	ResizeParentToFit();//會讓主視窗調整成FormView的尺寸	
	// TODO: Add your specialized code here and/or call the base class	
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
}
//-------------------------------------------------------------------------------------//
void CDebugFormView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}
//-------------------------------------------------------------------------------------//
void CDebugFormView::OnDebugBtn() 
{
	// TODO: Add your control notification handler code here
	CDebugWnd wnd;
	wnd.DoModal();
}
//-------------------------------------------------------------------------------------//
void CDebugFormView::OnNewProjectBtn() 
{
	// TODO: Add your control notification handler code here
	CNewProjectWizardWnd Wnd;//NewProjectWizardWnd
	Wnd.SetNewProjectMode(NEW_PROJECT_ONLINE);
	if ( Wnd.DoModal() == IDCANCEL ) { return; }	
	CAOIProject *ProjectPtr = Wnd.GetProjectPtr();
	if ( NULL == ProjectPtr ) { return; }
	
	AOIDataCollect.AddProjectPtr(ProjectPtr, false);
	AOIDataCollect.SetActiveProjectIndex(0);
}
//-------------------------------------------------------------------------------------//
void CDebugFormView::OnImageDebugBtn() 
{
	// TODO: Add your control notification handler code here
	/*
	int        i=0;	
	const int  nAlign  = 4;
	const int  nImages = 3;	
	
	CString    str[nImages];
	IMAGE_PTR  ImagePtr[nImages]={NULL};
	IMAGE_SIZE ImageW[nImages]={0};
	IMAGE_SIZE ImageH[nImages]={0};
	IMAGE_SIZE ImageStep[nImages]={0};
	IMAGE_SIZE BitCount[nImages]={0};
	
	for ( i=0; i<nImages; i++ )
	{
		switch ( i )
		{
		//case 0:	str[i] = _T("F:\\TestBMP\\OCV\\Rect-1.PNG");	break;
		//case 1:	str[i] = _T("F:\\TestBMP\\OCV\\Rect-2.PNG");	break;
		//case 2:	str[i] = _T("F:\\TestBMP\\OCV\\Rect-3.PNG");	break;
		case 0:	str[i] = _T("F:\\TestBMP\\OCV\\AB8.PNG");	break;
		case 1:	str[i] = _T("F:\\TestBMP\\OCV\\ABB.PNG");	break;
		case 2:	str[i] = _T("F:\\TestBMP\\OCV\\ABC.PNG");	break;
		case 3:	str[i] = _T("F:\\TestBMP\\OCV\\ABD.PNG");	break;
		case 4:	str[i] = _T("F:\\TestBMP\\OCV\\ABE.PNG");	break;
		case 5:	str[i] = _T("F:\\TestBMP\\OCV\\ABF.PNG");	break;
		case 6:	str[i] = _T("F:\\TestBMP\\OCV\\ABP.PNG");	break;
		case 7:	str[i] = _T("F:\\TestBMP\\OCV\\AB8-2.PNG");	break;
		
		}
		ImageAPI.LoadImage(str[i], ImageW[i], ImageH[i], ImageStep[i], BitCount[i], ImagePtr[i], nAlign, true);
	}


	CString    sstr;
	IMAGE_PTR  ResultPtr=NULL;
	IMAGE_SIZE ResultW=ImageW[0];
	IMAGE_SIZE ResultH=ImageH[0];
	IMAGE_SIZE ResultBitCount=8;
	IMAGE_SIZE ResultStep=JetAPI::GetBMPImagePixelsPerLine(ResultW, ResultBitCount, nAlign);;	
	const size_t ResultSize = ImageAPI.CalcBufferSize(ResultStep, ResultH);
	const int nRoiSizeW = 8;
	const int nRoiSizeH = 8;
	JetMemory.alloc_func(ResultSize, ResultPtr, "CDebugFormView::OnImageDebugBtn", "ResultPtr");
	for ( i=1; i<nImages; i++ )
	{
		ImageAPI.Compare2ColorImageRoi3(ImageW[i], ImageH[i], ImageStep[i], ImagePtr[0], ImagePtr[i], nRoiSizeW, nRoiSizeH, ResultStep, ResultPtr);
		sstr.Format(_T("%s\\Result%d.PNG"), _T("F:\\TestBMP\\OCV"), i);
		ImageAPI.SaveImage(sstr, ResultW, ResultH, ResultStep, ResultBitCount, ResultPtr, true);
	}

	JetMemory.free_func(ResultPtr);
	for ( i=0; i<nImages; i++ )
	{	JetMemory.free_func(ImagePtr[i]);	}
	*/
	CImageDebugWnd  Wnd;
	//CImagePhaseWnd Wnd;
	Wnd.DoModal();
}
//-------------------------------------------------------------------------------------//
void CDebugFormView::OnView3dDebugBtn() 
{
	// TODO: Add your control notification handler code here
	CDraw3DWnd Wnd;
	Wnd.DoModal();
}
//-------------------------------------------------------------------------------------//
void CDebugFormView::OnTiDlpDebugBtn() 
{
	const int RepeatCount = 100;
	const DWORD ResetDelayTime = 3000;
	CString str;
	std::vector<LIGHT_3D_CAST_ID> CastIDList;
	std::vector<LIGHT_3D_CAST_ID> TestCastIDList;
	std::vector<LIGHT_3D_CLS_PTR> CastPtrList;

	if (Light3DCtrl.GetLight3DCastIDList(CastIDList) == false)
	{
		JetAPI::ShowMessageBox(Light3DCtrl.GetErrorString());
		return;
	}

	for (size_t i = 0; i<CastIDList.size(); i++)
	{
		LIGHT_3D_CLS_PTR pCastPtr = Light3DCtrl.GetLight3DCastPtr(CastIDList[i]);
		if (NULL == pCastPtr) { continue; }
		TestCastIDList.push_back(CastIDList[i]);
		CastPtrList.push_back(pCastPtr);
	}

	const int DLPCount = (int)CastPtrList.size();
	if (DLPCount <= 0)
	{
		JetAPI::ShowMessageBox(_T("Error, No DLP device is available"));
		return;
	}

	str.Format(_T("All %d DLP devices will run software reset and reconnect in parallel.\n")
		_T("Each DLP repeats the operation %d times.\n")
		_T("The test stops after the current cycle when an error occurs.\n")
		_T("Press Esc between reset cycles to stop the test.\n\n")
		_T("Start the test?"), DLPCount, RepeatCount);
	if (AfxMessageBox(str, MB_YESNO | MB_ICONWARNING) != IDYES)
	{
		return;
	}

	const int nOpenMP = DLPCount;
	volatile LONG StopRequested = 0;
	std::vector<int> ResultList(DLPCount, 0);
	std::vector<int> CompleteCountList(DLPCount, 0);
	std::vector<CString> ErrorList(DLPCount);

#pragma omp parallel for num_threads(nOpenMP)
	for (int DLPIndex = 0; DLPIndex<DLPCount; DLPIndex++)
	{
		DWORD StartTick = 0;
		DWORD ElapsedTime = 0;
		CString strThread;
		CString strError;
		const LIGHT_3D_CAST_ID CastID = TestCastIDList[DLPIndex];
		LIGHT_3D_CLS_PTR CastPtr = CastPtrList[DLPIndex];

		if (CastPtr->GetDLPIsConnected() == false)
		{
			if (CastPtr->DLPConnect() == false)
			{
				strError = CastPtr->GetErrorString();
				ErrorList[DLPIndex].Format(
					_T("DLP[%d] initial connection failed. %s"),
					CastID, (LPCTSTR)strError);
				ResultList[DLPIndex] = -1;
				::InterlockedExchange(&StopRequested, 1);
				//AOIDataCollect.SaveMovingTimeMsg(ErrorList[DLPIndex]);
				JetAPI::SendDebugString(ErrorList[DLPIndex]);
				continue;
			}
		}

		for (int RepeatIndex = 1; RepeatIndex <= RepeatCount; RepeatIndex++)
		{
			if ((::GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0)
			{
				::InterlockedExchange(&StopRequested, 1);
			}

			if (::InterlockedCompareExchange(&StopRequested, 0, 0) != 0)
			{
				ResultList[DLPIndex] = 2;
				break;
			}

			strThread.Format(_T("DLP[%d] reset/reconnect cycle %d/%d start"),
				CastID, RepeatIndex, RepeatCount);
			//AOIDataCollect.SaveMovingTimeMsg(strThread);
			JetAPI::SendDebugString(strThread);

			StartTick = ::GetTickCount();
			if (CastPtr->ExecDLPSoftwareReset(ResetDelayTime) == false)
			{
				ElapsedTime = ::GetTickCount() - StartTick;
				strError = CastPtr->GetErrorString();
				ErrorList[DLPIndex].Format(
					_T("DLP[%d] reset/reconnect cycle %d/%d failed, elapsed=%lu ms. %s"),
					CastID, RepeatIndex, RepeatCount, ElapsedTime, (LPCTSTR)strError);
				ResultList[DLPIndex] = -1;
				::InterlockedExchange(&StopRequested, 1);
				AOIDataCollect.SaveMovingTimeMsg(ErrorList[DLPIndex]);
				JetAPI::SendDebugString(ErrorList[DLPIndex]);
				break;
			}

			ElapsedTime = ::GetTickCount() - StartTick;
			CompleteCountList[DLPIndex] = RepeatIndex;
			strThread.Format(_T("DLP[%d] reset/reconnect cycle %d/%d success, elapsed=%lu ms"),
				CastID, RepeatIndex, RepeatCount, ElapsedTime);
			AOIDataCollect.SaveMovingTimeMsg(strThread);
			JetAPI::SendDebugString(strThread);
		}

		if (CompleteCountList[DLPIndex] == RepeatCount)
		{
			ResultList[DLPIndex] = 1;
		}
		else if (ResultList[DLPIndex] == 0)
		{
			ResultList[DLPIndex] = 2;
		}
	}

	str = _T("DLP parallel reset/reconnect test result:");
	for (int DLPIndex = 0; DLPIndex<DLPCount; DLPIndex++)
	{
		CString strResult;
		if (ResultList[DLPIndex] == 1)
		{
			strResult.Format(_T("\nDLP[%d]: completed %d/%d cycles"),
				TestCastIDList[DLPIndex], CompleteCountList[DLPIndex], RepeatCount);
		}
		else if (ResultList[DLPIndex] == -1)
		{
			strResult.Format(_T("\nDLP[%d]: failed after %d/%d cycles. %s"),
				TestCastIDList[DLPIndex], CompleteCountList[DLPIndex], RepeatCount,
				(LPCTSTR)ErrorList[DLPIndex]);
		}
		else
		{
			strResult.Format(_T("\nDLP[%d]: stopped after %d/%d cycles"),
				TestCastIDList[DLPIndex], CompleteCountList[DLPIndex], RepeatCount);
		}
		str += strResult;
	}

	//AOIDataCollect.SaveMovingTimeMsg(str);
	JetAPI::SendDebugString(str);
	//JetAPI::ShowMessageBox(str);
#ifdef _DEBUG
	CLight3DTiDLPWnd Wnd;
	Wnd.DoModal();
#endif//_DEBUG
}
//-------------------------------------------------------------------------------------//
void CDebugFormView::OnSaveBtn() 
{
	// TODO: Add your control notification handler code here	
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	TCHAR szFilters[]=_T("PRG Files (*.PRG)|*.PRG|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("PRG"), _T("*.PRG"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }	
	
	CString filename = dialog.GetPathName();	
	DWORD dwCopyFlag = COPY_PROJECT_FOLDER_NO_OFFLINE;
	const bool bPartialCopy = AOIDataCollect.GetPartialCopyProjectLibrary();
	if ( ProjectPtr->SaveProject(filename, filename, dwCopyFlag, bPartialCopy) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return;
	}		
}
//-------------------------------------------------------------------------------------//
void CDebugFormView::OnCoordinateBtn() 
{
	// TODO: Add your control notification handler code here
	int i=0;
	CMapCoordinate MapCor;
	double w=200, h=100;
	double sx=1.2, sy=1.5;
	double dSIN=0, dCOS=0;
	double dx=0, dy=0, angle=0, angleR=0;
	double CadX=0, CadY=0, StageX=0, StageY=0;
	double FdCadX[4]={0}, FdCadY[4]={0};
	double FdCadX2[4]={0}, FdCadY2[4]={0};
	double FdStageX[4]={0}, FdStageY[4]={0};
	double FdStageX2[4]={0}, FdStageY2[4]={0};

	dx = 50; dy = 30; angle = 10;
	angleR = angle*3.14159/180.0;
	dSIN = ::sin(angleR);
	dCOS = ::cos(angleR);

	FdCadX[0] = 0; FdCadY[0]=0;
	FdCadX[1] = w; FdCadY[1]=0;
	FdCadX[2] = w; FdCadY[2]=h;
	FdCadX[3] = 0; FdCadY[3]=h;

	FdCadX2[0] =    0; FdCadY2[0]=   0;
	FdCadX2[1] = w*sx; FdCadY2[1]=   0;
	FdCadX2[2] = w*sx; FdCadY2[2]=h*sy;
	FdCadX2[3] =    0; FdCadY2[3]=h*sy;	

	for ( i=0; i<4; i++ )
	{
		FdStageX[i] = (FdCadX2[i]*dCOS)+(FdCadY2[i]*dSIN)+dx;
		FdStageY[i] = (-FdCadX2[i]*dSIN)+(FdCadY2[i]*dCOS)+dy;
	}	

	MapCor.CalcMatrix2D(dx, dy, angle, sx, sy);
//	MapCor.CalcMatrix2D(FdCadX, FdCadY, FdStageX, FdStageY, 3);
	for ( i=0; i<4; i++ )
	{	MapCor.Map2D(FdCadX[i], FdCadY[i], FdStageX2[i], FdStageY2[i]);	}	

	CadX = CadX;
}

void CDebugFormView::OnImageMaskBtn() 
{
	// TODO: Add your control notification handler code here
	CImageMaskWnd Wnd;
	Wnd.DoModal();
}

void CDebugFormView::OnAllPhaseBtn() 
{
	// TODO: Add your control notification handler code here
	CImagePhaseAllWnd Wnd;
	Wnd.DoModal();
}

void CDebugFormView::OnModelBtn() 
{
	// TODO: Add your control notification handler code here	
	const char fnName[] = "CDebugFormView::OnModelBtn";
	MODEL_ATTACHED_OBJ ModelAttachedObj = AOIDataCollect.GetModelAttachedObj();	
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }	
	CAOIFd        *FdPtr = NULL;
	CAOIMark      *MarkPtr = NULL;
	CAOIModel     *ModelPtr = NULL;
	CAOIBarcode   *BarcodePtr = NULL;
	CAOIComponent *ComponentPtr = NULL;
	switch ( ModelAttachedObj )
	{
	case MODEL_ATTACHED_FD:
		FdPtr = ProjectPtr->GetProjectActiveFd();
		if ( NULL == FdPtr ) { return; }
		ModelPtr = FdPtr->GetFdModelPtr();
		break;
	case MODEL_ATTACHED_MARK:
		MarkPtr = ProjectPtr->GetProjectActiveMark();
		if ( NULL == MarkPtr ) { return; }
		ModelPtr = MarkPtr->GetMarkModelPtr();
		break;
	case MODEL_ATTACHED_BARCODE:
		BarcodePtr = ProjectPtr->GetProjectActiveBarcode();
		if ( NULL == BarcodePtr ) { return; }
		ModelPtr = BarcodePtr->GetBarcodeModelPtr();
		break;
	case MODEL_ATTACHED_COMPONENT:
		ComponentPtr = ProjectPtr->GetProjectActiveComponent();
		if ( NULL == ComponentPtr ) { return; }
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		break;
	default:		
		break;
	}
	if ( NULL == ModelPtr ) { return; }
	ModelPtr = ModelPtr->CloneModelObj();
	if ( NULL == ModelPtr ) { return; }

	const bool bCloned = true;	
	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);	
	std::vector<TUNI_FRAME> UniFrameList;	
	AOIDataCollect.CopyModelUniFrameList(UniFrameList, bCloned);
	AOIDataCollect.SetDrawImageMode(DRAW_IMAGE_NORMAL);
	
	if ( true == IsExceptionAngle ) 
	{
		size_t      i=0;
		TUNI_FRAME  UniFrameTmp;	
		std::vector<TUNI_FRAME> UniFrameListTmp;	
		const size_t FrameCount = UniFrameList.size();
		for ( i=0; i<FrameCount; i++ )
		{
			if ( ImageAPI.RotateUniImage(-AttachedAngle, UniFrameList[i], 4, fnName, UniFrameTmp) == false ) 
			{
				JetAPI::ClearUniFrameList(UniFrameList);
				JetAPI::ClearUniFrameList(UniFrameListTmp);
				return;
			}
			UniFrameListTmp.push_back(UniFrameTmp);
		}
		JetAPI::ClearUniFrameList(UniFrameList);
		UniFrameList = UniFrameListTmp;
		ModelPtr->RotateModel(-AttachedAngle, 0, 0);
	}

	CModelPropertyWnd ModelProptyWnd;
	ModelProptyWnd.SetModelPtr(ModelPtr);
	ModelProptyWnd.SetUniFrameList(UniFrameList);
	ModelProptyWnd.DoModal();
	AOIObjManager.DestroyModelObj(ModelPtr);	
	JetAPI::ClearUniFrameList(UniFrameList);	
	return ;
	
	/*
	CModelWnd Wnd;
	Wnd.SetModelPtr(ModelPtr);
	Wnd.SetActiveProject(ProjectPtr);
	Wnd.SetUniFrameList(UniFrameList);
	Wnd.DoModal();
	AOIObjManager.DestroyModelObj(ModelPtr);		
	JetAPI::ClearUniFrameList(UniFrameList);	
	//*/
	return;
}

void CDebugFormView::OnBarcodeDeviceBtn() 
{
	// TODO: Add your control notification handler code here
	CBarcodeDeviceWnd  Wnd;
#ifndef BARCODE_DEVICE_DISABLE
	Wnd.DoModal();
#endif//BARCODE_DEVICE_DISABLE
}

void CDebugFormView::OnBarcodeHandheldBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	CBarcodeInputWnd Wnd;	
	BARCODE_HANDHELD_READ_MODE BarcodeReadMode = ProjectPtr->GetProjectParameter().m_BarcodeHandHeldReadMode;

	Wnd.SetProjectPtr(ProjectPtr);
	Wnd.SetBarcodeReadMode(BarcodeReadMode);
	Wnd.DoModal();
}

void CDebugFormView::OnProjectColorBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CProjectColorWnd Wnd;
	Wnd.SetProjectPtr(ProjectPtr);
	Wnd.DoModal();
}

void CDebugFormView::OnProjectMapMaskBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CProjectMapMaskWnd Wnd;
	Wnd.SetProjectPtr(ProjectPtr);
	if ( Wnd.DoModal() == IDCANCEL ) 
	{	return; }

	IMAGE_PTR  MaskPtr=NULL;
	IMAGE_SIZE MaskW=0;
	IMAGE_SIZE MaskH=0;
	IMAGE_SIZE MaskStep=0;
	IMAGE_SIZE MaskBitCount=0;	
	MaskPtr = Wnd.GetMaskImage(MaskW, MaskH, MaskStep, MaskBitCount);
	ProjectPtr->SetProjectMapMaskImage(MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);
}

void CDebugFormView::OnNewPanelBtn() 
{
	// TODO: Add your control notification handler code here		
	CNewPanelWizardWnd  WizardWnd;
	CAOIProject        *ProjectPtr=AOIDataCollect.GetActiveProject();;
	WizardWnd.SetProjectPtr(ProjectPtr);
	WizardWnd.DoModal();
}

void CDebugFormView::OnFullMapComponentBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject        *ProjectPtr=AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	CString str;
	str = _T("Do you want to create the components for full project map?");
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return; }

	if ( ProjectPtr->CreateProjectComponentForFullProjectMap() == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return;
	}
	
}

void CDebugFormView::OnSaveSpcFileBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject        *ProjectPtr=AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	bool bSaveTest = true;
	TCHAR szFilters[]=_T("DAT Files (*.DAT)|*.DAT|XML Files (*.XML)|*.XML|JSON Files (*.JSON)|*.JSON|TXT Files (*.TXT)|*.TXT|All Files (*.*)|*.*||");

	if ( true == bSaveTest )
	{
		CFileDialog dialog (FALSE, _T("DAT;XML;JSON;TXT"), _T("*.DAT"), OFN_FILEMUSTEXIST, szFilters);
		if ( dialog.DoModal() == IDCANCEL )
		{	return ; }	
	
		CString ExtName;
		CString filename = dialog.GetPathName();
		JetAPI::ExtractExtendFileName(filename, ExtName);
		ExtName.MakeUpper();

		CString str;
		double fnTime=0;
		LARGE_INTEGER fnStart, fnEnd;
		JetAPI::SetFuncTimeStart(fnStart);
		if ( ExtName == _T("XML") )
		{	ProjectPtr->SaveProjectSpcFile_XML(filename); }
		else if ( ExtName == _T("JSON") )
		{	ProjectPtr->SaveProjectSpcFile_JSON(filename); }
		else if ( ExtName == _T("TXT") )
		{	ProjectPtr->SaveProjectSpcFile_TXT2(filename); }	
		else 
		{	ProjectPtr->SaveProjectSpcFile_Binary(filename);	}
		JetAPI::SetFuncTimeEnd(fnEnd);
		fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
		str.Format(_T("Saving Time=%.3f ms"), fnTime);
		JetAPI::ShowMessageBox(str);
	}
	else
	{
		CFileDialog dialog (TRUE, _T("DAT;XML;JSON;TXT"), _T("*.DAT"), OFN_FILEMUSTEXIST, szFilters);
		if ( dialog.DoModal() == IDCANCEL )
		{	return ; }	
	
		CString ExtName;
		CString filename = dialog.GetPathName();
		JetAPI::ExtractExtendFileName(filename, ExtName);
		ExtName.MakeUpper();

		CString str;
		double fnTime=0;
		LARGE_INTEGER fnStart, fnEnd;
		JetAPI::SetFuncTimeStart(fnStart);
		if ( ExtName == _T("XML") )
		{
			str = _T("Can not load XML file");
			JetAPI::ShowMessageBox(str);
			return;
		}
		else if ( ExtName == _T("JSON") )
		{
			str = _T("Can not load JSON file");
			JetAPI::ShowMessageBox(str);
			return;
		}
		else if ( ExtName == _T("TXT") )
		{	ProjectPtr->LoadProjectSpcFile_TXT(filename);	}	
		else 
		{	ProjectPtr->LoadProjectSpcFile_Binary(filename);	}
		JetAPI::SetFuncTimeEnd(fnEnd);
		fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
		str.Format(_T("Loading Time=%.3f ms"), fnTime);
		JetAPI::ShowMessageBox(str);
	}
}

void CDebugFormView::OnComponentListWnd() 
{
	// TODO: Add your control notification handler code here
	CAOIProject        *ProjectPtr=AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	CComponentListWnd Wnd;
	Wnd.SetProjectPtr(ProjectPtr);
	Wnd.DoModal();	
}

void CDebugFormView::OnLoadCadxyBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject  *ProjectPtr=NULL;
	CLoadCadxyWnd LoadCadxyWnd;	

	ProjectPtr = AOIObjManager.CreateProjectObj();
	if ( NULL == ProjectPtr ) { return; }
	CAOIPanel *PanelPtr = AOIObjManager.CreatePanelObj();
	if ( NULL == PanelPtr )
	{
		AOIObjManager.DestroyProjectObj(ProjectPtr);
		return; 
	}
	PanelPtr->SetPanelSelected(true);	
	ProjectPtr->AddProjectPanelPtr(PanelPtr, false);
	ProjectPtr->SetProjectActivePanel(PanelPtr);

	if ( LoadCadxyWnd.SetProjectPtr(ProjectPtr) == false )
	{
		AOIObjManager.DestroyProjectObj(ProjectPtr);	
		return;
	}

	LoadCadxyWnd.DoModal();
	AOIObjManager.DestroyProjectObj(ProjectPtr);	
	
}

void CDebugFormView::OnProjectCompareBtn() 
{
	// TODO: Add your control notification handler code here
	TestSinePattern();

	CAOIProject        *ProjectPtr=AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	CProjectCompareWnd Wnd;
	Wnd.SetHostProjectPtr(ProjectPtr);
	Wnd.DoModal();

}

void CDebugFormView::OnAlgImageCompareBtn() 
{
	// TODO: Add your control notification handler code here
	CAlgImageCompareWnd Wnd;
	Wnd.DoModal();
}

void CDebugFormView::OnSaveMemoryBtn() 
{
	// TODO: Add your control notification handler code here
	CString filename;	
	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOILogDirectory(), _T("JetMemoryTmp.txt"));
	JetMemory.save_memory_node_list(filename);
	::ShellExecute(NULL, _T("open"), filename, NULL, NULL, SW_SHOW);
}

void CDebugFormView::OnBuildXYCaliBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	//ProjectPtr->SaveProjectReport_Text();
	//return;

	CString  str;
	DWORD    Res=0;
	bool     Rebuild = true;
	const TMotionParameter &MotionParam = MotionCtrlPtr->GetMotionParameter();
	if ( FN_ENABLE == MotionParam.m_XYCaliEnable )
	{
		str = "Please disable XY-Calibration, and Calibrate again.";
		JetAPI::ShowMessageBox(str);		
		return ;
	}

	str = "Do you want to rebuild XY calibration table?";
	Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
	if ( Res == IDCANCEL ) { return; }

	if ( Res == IDNO ) { Rebuild = false; }
	else { Rebuild = true; }
	
	AOIDataCollect.BuildMotionXYCaliList(Rebuild);	

	str = "Do you want to enable XY calibration?";
	Res = JetAPI::ShowMessageBox(str, MB_YESNO);
	if ( Res == IDYES )
	{ 
		MotionCtrlPtr->GetMotionParameter().m_XYCaliEnable = FN_ENABLE;
		return; 
	}
	return;	
}

void CDebugFormView::OnBarcodeRecognizeBtn() 
{
	// TODO: Add your control notification handler code here
	INT_PTR Ret=0;
	const bool bExtend = false;
	CAlgBarcodeRecognizeWnd BarcodeWnd;	
	Ret = BarcodeWnd.DoModal();		
	return ;
}

void CDebugFormView::OnFieldConfigBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	CProjectFieldConfigWnd Wnd;
	Wnd.SetProjectPtr(ProjectPtr);
	Wnd.DoModal();
}

void CDebugFormView::OnSocketClientBtn() 
{
	// TODO: Add your control notification handler code here
	if ( ITSCommWnd.IsWindowVisible() == false )
	{	ITSCommWnd.ShowWindow(SW_SHOW);	}	
}

void CDebugFormView::OnLoadRepairFileBtn() 
{
	// TODO: Add your control notification handler code here
	AOIDataCollect.StartLoadRepairFileThread(true);
}

void CDebugFormView::OnVerifyBinFileBtn() 
{
	// TODO: Add your control notification handler code here
	DWORD   Ret;	
	CString str;
	int     ErrCnt=0;
	CString Filename1;	
	CString Filename2;	
	FILE   *pFile = NULL;
	str = _T("Verify Bin File (Yes-Zero Phase, No-Height Factor(k-value))");
	Ret = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
	if ( IDCANCEL == Ret ) { return ; }
	
	TCHAR szFilters[]=_T("Bin Files (*.Bin)|*.Bin|All Files (*.*)|*.*||");
	CFileDialog dialog (TRUE, _T("Bin"), _T("*.Bin"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }	
	Filename1 = dialog.GetPathName();
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }	
	Filename2 = dialog.GetPathName();

	if ( IDYES == Ret ) 
	{
		IMAGE_SIZE  PhaseZeroW[2];//平面相位寬度
		IMAGE_SIZE  PhaseZeroH[2];//平面相位高度
		IMAGE_SIZE  PhaseZeroStep[2];//平面相位步長
		PHASE_PTR   PhaseZeroPtr[2];//平面相位指標
		if ( ImageAPI.LoadPhaseBinFile(Filename1, PhaseZeroW[0], PhaseZeroH[0], PhaseZeroStep[0], PhaseZeroPtr[0]) == false ) 
		{
			str = ImageAPI.GetImageApiErrorString();
			JetAPI::ShowMessageBox(str);
			JetMemory.free_func(PhaseZeroPtr[0]);
			JetMemory.free_func(PhaseZeroPtr[1]);
			return;
		}
		if ( ImageAPI.LoadPhaseBinFile(Filename2, PhaseZeroW[1], PhaseZeroH[1], PhaseZeroStep[1], PhaseZeroPtr[1]) == false ) 
		{
			str = ImageAPI.GetImageApiErrorString();
			JetAPI::ShowMessageBox(str);
			JetMemory.free_func(PhaseZeroPtr[0]);
			JetMemory.free_func(PhaseZeroPtr[1]);
			return;
		}

		if ( PhaseZeroW[0]!=PhaseZeroW[1] || PhaseZeroH[0]!=PhaseZeroH[1] || PhaseZeroStep[0]!=PhaseZeroStep[1] )
		{
		}
		else
		{
			ImageAPI.Compare2PhaseBinFile(PhaseZeroW[0], PhaseZeroH[0], PhaseZeroStep[0], PhaseZeroPtr[0], PhaseZeroPtr[1], ErrCnt);
		}

		JetMemory.free_func(PhaseZeroPtr[0]);
		JetMemory.free_func(PhaseZeroPtr[1]);		
	}

	if ( IDNO == Ret ) 
	{
		IMAGE_SIZE   PhaseFactorW[2];//平面係數寬度
		IMAGE_SIZE   PhaseFactorH[2];//平面係數高度
		IMAGE_SIZE   PhaseFactorStep[2];//平面係數步長
		SPACE_PTR    PhaseFactorPtr[2];//平面係數指標

		if ( ImageAPI.LoadSpaceBinFile(Filename1, PhaseFactorW[0], PhaseFactorH[0], PhaseFactorStep[0], PhaseFactorPtr[0]) == false ) 
		{
			str = ImageAPI.GetImageApiErrorString();
			JetAPI::ShowMessageBox(str);
			JetMemory.free_func(PhaseFactorPtr[0]);
			JetMemory.free_func(PhaseFactorPtr[1]);
			return;
		}
		if ( ImageAPI.LoadSpaceBinFile(Filename2, PhaseFactorW[1], PhaseFactorH[1], PhaseFactorStep[1], PhaseFactorPtr[1]) == false ) 
		{
			str = ImageAPI.GetImageApiErrorString();
			JetAPI::ShowMessageBox(str);
			JetMemory.free_func(PhaseFactorPtr[0]);
			JetMemory.free_func(PhaseFactorPtr[1]);
			return;
		}

		if ( PhaseFactorW[0]!=PhaseFactorW[1] || PhaseFactorH[0]!=PhaseFactorH[1] || PhaseFactorStep[0]!=PhaseFactorStep[1] )
		{
		}
		else
		{
			ImageAPI.Compare2SpaceBinFile(PhaseFactorW[0], PhaseFactorH[0], PhaseFactorStep[0], PhaseFactorPtr[0], PhaseFactorPtr[1], ErrCnt);
		}
		JetMemory.free_func(PhaseFactorPtr[0]);
		JetMemory.free_func(PhaseFactorPtr[1]);
	}

	str.Format(_T("Error Count = %d"), ErrCnt);
	JetAPI::ShowMessageBox(str);
}

void CDebugFormView::OnSaveLocationFileBtn() 
{
	// TODO: Add your control notification handler code here
	DWORD   Ret;	
	CString str;	
	CString Filename;
	
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	TCHAR szFilters[]=_T("JSON Files (*.JSON)|*.JSON|All Files (*.*)|*.*||");
	CFileDialog dialog (FALSE, _T("JSON"), _T("*.JSON"), OFN_FILEMUSTEXIST, szFilters);
	if ( dialog.DoModal() == IDCANCEL )
	{	return ; }	
	Filename = dialog.GetPathName();
	if ( ProjectPtr->SaveProjectLocationFile(Filename) == false ) 
	{	return ; }


	bool bRet=false;
	int  nVal=1;
	double dVal=1.0;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;

	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	if ( JSonCtrl.OpenFile(Filename, Doc) == false ) 
	{	return; }

	std::wstring wstValue;
	rapidjson::CGMItr itr;
	bRet=JSonCtrl.FindMember(L"Machine", itr);
	if ( false == bRet ) 
	{	return; }

	if ( itr->value.IsString() )
	{	wstValue = itr->value.GetString();	}
	
	return ;
}

void CDebugFormView::OnCombineImageBtn() 
{
	// TODO: Add your control notification handler code here
	CImageCombineWnd Wnd;
	//Wnd.SetCombineMapMode(COMBINE_MAP_BY_RIGHT);
	Wnd.SetCombineMapMode(COMBINE_MAP_BY_LEFT);
	Wnd.DoModal();
	Wnd.ClearMapBuffer();
}

void CDebugFormView::OnDivideDistrictBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	CProjectDivideDistrictWnd Wnd;	
	Wnd.SetProjectPtr(ProjectPtr);
	Wnd.DoModal();
	
}

void CDebugFormView::OnBoardConfigBtn() 
{
	// TODO: Add your control notification handler code here
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }

	CBoardConfigWnd Wnd;	
	Wnd.SetProjectPtr(ProjectPtr);
	Wnd.SetBoardConfigMode(BOARD_CONFIG_ORDER);
	Wnd.DoModal();
}

bool  CDebugFormView::TestImageScale()//影像縮放
{
	return true;

	CString     str;
	CString     filename;
	double      Scale=0;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE BitCount = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_PTR  pImage = NULL;

	IMAGE_SIZE DestW = 0;
	IMAGE_SIZE DestH = 0;	
	IMAGE_SIZE DestStep = 0;
	IMAGE_PTR  pDest = NULL;
	
	filename = _T("C:\\Frame_00002.PNG");
	if ( ImageAPI.LoadImage(filename, ImageW, ImageH, ImageStep, BitCount, pImage, 4, true ) == false )
	{	return false; }

	int i=0;
	Scale=0.1;
	for ( i=0; i<9; i++ )
	{
		if ( ImageAPI.ScaleImage(ImageW, ImageH, ImageStep, BitCount, pImage, Scale, DestW, DestH, DestStep, pDest) == false )
		{
			JetMemory.free_func(pImage);
			return false;
		}
		str.Format(_T("%s\\%s_0%d.PNG"), AOIDataCollect.GetAOITempDirectory(), _T("ScaleImage"), i+1);
		ImageAPI.SaveImage(str, DestW, DestH, DestStep, BitCount, pDest, true);

		JetMemory.free_func(pDest);
		DestW = DestH = DestStep = 0;
		Scale += 0.1;
	}

	JetMemory.free_func(pDest);
	JetMemory.free_func(pImage);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CDebugFormView::TestSTLVector()//標準樣板類別的Vector
{
	//return true;
	size_t      i=0;
	CString     str;
	int         nVar=0;
	float       fVar=0.0f;
	unsigned short shVar=0;
	unsigned char  ucVar=0;
	double      fnTime=0.0;
	LARGE_INTEGER  fnStart, fnEnd;
	const size_t MaxCnt=50;
	const size_t ChkIdx=MaxCnt/2;
	std::vector<int>   IntList;
	std::vector<int>   IntList_nTh;
	std::vector<int>   IntList_Partial;	
	std::vector<float> FltList;
	std::vector<float> FltList_nTh;
	std::vector<float> FltList_Partial;
	std::vector<unsigned short> ShortList;	
	std::vector<unsigned char> UcList;	
	
	fVar=0;
	JetAPI::SetFuncTimeStart(fnStart);
	for ( i=0; i<MaxCnt; i++ )
	{	FltList.push_back(fVar++);	}
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Test float Vector[%d] push_back Time=%.6f ms"), MaxCnt, fnTime);
	AOIDataCollect.SaveLogMessage(str);
	fVar=0;
	for ( i=0; i<MaxCnt; i++ )
	{	FltList_nTh.push_back(fVar++);	}
	fVar=0;
	for ( i=0; i<MaxCnt; i++ )
	{	FltList_Partial.push_back(fVar++);	}

	
	nVar=0;
	JetAPI::SetFuncTimeStart(fnStart);
	for ( i=0; i<MaxCnt; i++ )
	{	IntList.push_back(nVar++);	}
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Test Int Vector[%d] push_back Time=%.6f ms"), MaxCnt, fnTime);
	AOIDataCollect.SaveLogMessage(str);
	nVar=0;
	for ( i=0; i<MaxCnt; i++ )
	{	IntList_Partial.push_back(nVar++);	}
	nVar=0;
	for ( i=0; i<MaxCnt; i++ )
	{	IntList_nTh.push_back(nVar++);	}
	
	
	JetAPI::SetFuncTimeStart(fnStart);
	for ( i=0; i<MaxCnt; i++ )
	{
		shVar=i%65536;
		ShortList.push_back(shVar);	
	}
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Test Short Vector[%d] push_back Time=%.6f ms"), MaxCnt, fnTime);
	AOIDataCollect.SaveLogMessage(str);

	JetAPI::SetFuncTimeStart(fnStart);
	for ( i=0; i<MaxCnt; i++ )
	{	
		ucVar=i%256;
		UcList.push_back(ucVar);	
	}
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Test char Vector[%d] push_back Time=%.6f ms"), MaxCnt, fnTime);
	AOIDataCollect.SaveLogMessage(str);

	JetAPI::SetFuncTimeStart(fnStart);
	std::sort(FltList.begin(), FltList.end());
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Test Float Vector[%d] Sort Time=%.6f ms, [%d]=[%.0f]"), MaxCnt, fnTime, ChkIdx, FltList[ChkIdx]);
	AOIDataCollect.SaveLogMessage(str);
	
	JetAPI::SetFuncTimeStart(fnStart);
	std::nth_element(FltList_nTh.begin(), FltList_nTh.begin()+ChkIdx, FltList_nTh.end());
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Test Float Vector[%d] nTH-Element Time=%.6f ms, [%d]=[%.0f]"), MaxCnt, fnTime, ChkIdx, FltList_nTh[ChkIdx]);
	AOIDataCollect.SaveLogMessage(str);	

	JetAPI::SetFuncTimeStart(fnStart);
	std::partial_sort(FltList_Partial.begin(), FltList_Partial.begin()+ChkIdx, FltList_Partial.end());
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Test Float Vector[%d] Partial-Sort Time=%.6f ms, [%d]=[%.0f]"), MaxCnt, fnTime, ChkIdx, FltList_Partial[ChkIdx]);
	AOIDataCollect.SaveLogMessage(str);	

	JetAPI::SetFuncTimeStart(fnStart);
	std::sort(IntList.begin(), IntList.end());
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Test Int Vector[%d] Sort Time=%.6f ms, [%d]=[%d]"), MaxCnt, fnTime, ChkIdx, IntList[ChkIdx]);
	AOIDataCollect.SaveLogMessage(str);

	JetAPI::SetFuncTimeStart(fnStart);
	std::nth_element(IntList_nTh.begin(), IntList_nTh.begin()+ChkIdx, IntList_nTh.end());
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Test Int Vector[%d] nTH-Element Time=%.6f ms, [%d]=[%d]"), MaxCnt, fnTime, ChkIdx, IntList_nTh[ChkIdx]);
	AOIDataCollect.SaveLogMessage(str);

	JetAPI::SetFuncTimeStart(fnStart);
	std::partial_sort(IntList_Partial.begin(), IntList_Partial.begin()+ChkIdx, IntList_Partial.end());
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Test Int Vector[%d] Partial-Sort Time=%.6f ms, [%d]=[%d]"), MaxCnt, fnTime, ChkIdx, IntList_Partial[ChkIdx]);
	AOIDataCollect.SaveLogMessage(str);	

	JetAPI::SetFuncTimeStart(fnStart);
	std::sort(ShortList.begin(), ShortList.end());
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Test Short Vector[%d] Sort Time=%.6f ms"), MaxCnt, fnTime);
	AOIDataCollect.SaveLogMessage(str);

	JetAPI::SetFuncTimeStart(fnStart);
	std::sort(UcList.begin(), UcList.end());
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	str.Format(_T("Test char Vector[%d] Sort Time=%.6f ms"), MaxCnt, fnTime);
	AOIDataCollect.SaveLogMessage(str);
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CDebugFormView::TestSinePattern()
{
	return true;
	CString     str;
	IMAGE_SIZE ImageW = 912;
	IMAGE_SIZE ImageH = 1140;
	IMAGE_SIZE BitCount = 8;
	IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);

	const int StartPhase = 0;
	const int NPixelsPeriod = 16;
	const int NPeriod = 0;//JetAPI::GetComboxCurSelData(m_PatternBuildPhaseShiftCountCombo);
	const int BitDepth = 6;
	const bool bVer = true; 
	BOOL      bExportSinPat = FALSE;

	unsigned char *pImage = NULL;		
	int      P1 = NPixelsPeriod;

	//000
	P1 = 16;
	ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+0, bVer);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SinePat_016_000_K.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, pImage, false);
	JetMemory.free_func(pImage);
	
	ImageAPI.GrayImageCreateSinPatternII(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+0, bVer);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SinePat_016_000_C.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, pImage, false);
	JetMemory.free_func(pImage);

	//090
	ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+90, bVer);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SinePat_016_090_K.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, pImage, false);
	JetMemory.free_func(pImage);
	
	ImageAPI.GrayImageCreateSinPatternII(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+90, bVer);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SinePat_016_090_C.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, pImage, false);
	JetMemory.free_func(pImage);

	//180
	ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+180, bVer);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SinePat_016_180_K.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, pImage, false);
	JetMemory.free_func(pImage);
	
	ImageAPI.GrayImageCreateSinPatternII(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+180, bVer);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SinePat_016_180_C.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, pImage, false);
	JetMemory.free_func(pImage);

	//270
	ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+270, bVer);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SinePat_016_270_K.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, pImage, false);
	JetMemory.free_func(pImage);
	
	ImageAPI.GrayImageCreateSinPatternII(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+270, bVer);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SinePat_016_270_C.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, pImage, false);
	JetMemory.free_func(pImage);


	//000
	P1 = 224;
	ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+0, bVer);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SinePat_224_000_K.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, pImage, false);
	JetMemory.free_func(pImage);
	
	ImageAPI.GrayImageCreateSinPatternII(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+0, bVer);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SinePat_224_000_C.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, pImage, false);
	JetMemory.free_func(pImage);

	//090	
	ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+90, bVer);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SinePat_224_090_K.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, pImage, false);
	JetMemory.free_func(pImage);
	
	ImageAPI.GrayImageCreateSinPatternII(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+90, bVer);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SinePat_224_090_C.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, pImage, false);
	JetMemory.free_func(pImage);

	//180	
	ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+180, bVer);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SinePat_224_180_K.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, pImage, false);
	JetMemory.free_func(pImage);
	
	ImageAPI.GrayImageCreateSinPatternII(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+180, bVer);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SinePat_224_180_C.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, pImage, false);
	JetMemory.free_func(pImage);


	//270	
	ImageAPI.GrayImageCreateCosPattern(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+270, bVer);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SinePat_224_270_K.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, pImage, false);
	JetMemory.free_func(pImage);
	
	ImageAPI.GrayImageCreateSinPatternII(ImageW, ImageH, ImageStep, pImage, P1, StartPhase+270, bVer);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SinePat_224_270_C.BMP"));
	ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, pImage, false);
	JetMemory.free_func(pImage);
	return true;
}

bool  CDebugFormView::TestJSON()//測試JSON
{
	bool bRet=false;
	int  nVal=1;
	double dVal=1.0;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	//bRet=JSonCtrl.AddMember(L"ABC_I", nVal);
	//std::string  str = "ABCDEF";
	//std::string  str = "ABCDEFGHIJKLMNOPQRST123456789";
	//std::wstring  str = L"ABCDEFGHIJKLMNOPQRST123456789";
	//rapidjson::Value tString;
	//tString.SetString(str.c_str(), Doc.GetAllocator());
	//return true;

	//Add
	//Add value into doc
	bRet=JSonCtrl.AddMember(L"ABC_I", nVal);

	bRet=JSonCtrl.AddMember(L"ABC_F", dVal);

	bRet=JSonCtrl.AddMember(L"ABC_S", L"abc");

	//add object into doc
	rapidjson::WValue object(rapidjson::kObjectType);	

	auto &alc = Doc.GetAllocator();

	rapidjson::WValue objectarray(rapidjson::kArrayType);
	objectarray.PushBack(1, alc);
	objectarray.PushBack(2, alc);
	bRet=JSonCtrl.AddMember(L"Array_Test", objectarray);	
	

	//add value into the object
	bRet=JSonCtrl.AddObject(&object, L"CycleTime", 31.5);

	bRet=JSonCtrl.AddObject(&object, L"Project", L"Jet-123");//bug

	bRet=JSonCtrl.AddMember(L"Data_Info", object);
	
	JSonCtrl.SaveFileIncSpace(L"C:\\BuildTest.JSON", Doc);	

	//Delete	
	/*
	//delete value in the doc
	bRet=JSonCtrl.EraseMember(L"ABC_I");

	//delete value in the object
	bRet=JSonCtrl.EraseObjectMember({L"Data_Info"}, L"Project");

	//delete object
	bRet=JSonCtrl.EraseMember({L"Data_Info"});
	//*/

	//Modify
	//modify value in the doc
	bRet=JSonCtrl.ModifyMember(L"ABC_I", 2);


	//modify value in the object
	bRet=JSonCtrl.ModifyObjectMember({L"Data_Info"}, L"Project", L"Jet-1234");


	JSonCtrl.SaveFileIncSpace(L"C:\\ModifyTest.JSON", Doc);	

	//Query
	//find value in the doc

	rapidjson::CGMItr itr;
	bRet=JSonCtrl.FindMember(L"Array_Test", itr);
	if ( true == bRet )
	{		
		int tmp=0;
		auto pName = itr->name.GetString();
		auto pType = itr->value.GetType();
		auto arrayV = itr->value.GetArray();
		for (auto m = arrayV.Begin(); arrayV.End() != m; ++m)
		{	
			if ( m->IsInt() )
			{
				tmp = m->GetInt();
			}			

			
			tmp ++;
		}
	}

	bRet=JSonCtrl.FindMember(L"Data_Info", itr);
	if ( true == bRet )
	{		
		int tmp=0;
		auto pName = itr->name.GetString();
		auto pType = itr->value.GetType();		
		for (auto m = itr->value.MemberBegin(); itr->value.MemberEnd() != m; ++m)
		{	
			auto pName2 = m->name.GetString();
			auto pType2 = m->value.GetType();

			
			tmp ++;
		}
	}


	
	//find value in the object (nested find)
	//Layer1 Name->Layer2 Name
	//L"Data_Info"->L"Project"
	std::vector<std::wstring> vList = {L"Data_Info", L"Project"};
	bRet=JSonCtrl.FindMember(vList, itr);
	//*/	

	bRet=bRet;
	return true;
}

bool CDebugFormView::TestJSON_02()//測試JSON
{	
	std::wstring wstring;
	wchar_t buffer[256]=L"ABCDEFG";
	buffer[1]=0x16;
	wstring = buffer;
	JetAPI::FilterJSONStringW(buffer);
	JetAPI::FilterJSONStringW(wstring);
	buffer[255]=0x00;
	/*
	bool bRet=false;
	int  nVal=1;
	double dVal=1.0;
	std::wstring filename;
	rapidjson::WDocument Doc;
	rapidjson::CJsonCtrl JSonCtrl;
	
	filename = L"R:\\20200903133017.JSON";
	//initial
	Doc.SetObject();
	JSonCtrl.Set(&Doc);
	
	JSonCtrl.OpenFile(filename.c_str(), Doc);
	
	bRet=bRet;
	*/
	return true;
}

bool CDebugFormView::TestMatch()//影像匹配
{
	return true;
#ifdef _DEBUG
	CString    str;
	CJetMatch  Match;
	CString    ErrorString;
	const int  nAlign = 4;
	const int  nMinReduceArea = 128;
	const int  nFinalReduction = 0;
	IMAGE_PTR  ImagePtr=NULL;	
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();

	str = _T("C:\\JETAOI3D\\Pattern.BMP");
	if ( ImageAPI.LoadImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, nAlign, true) == false )
	{
		JetMemory.free_func(ImagePtr);
		return false;
	}

	const int nImageW = (int)(ImageW);
	const int nImageH = (int)(ImageH);
	const int nImageW2 = (int)(nImageW/2);
	const int nImageH2 = (int)(nImageH/2);

	if ( Match.SetMatchLibType(MatchLibType)==false )
	{	
		ErrorString = Match.GetErrorString();
		return false;	
	}
	//initial eMatch
	Match.SetMatchDefaultParam();
	Match.SetMinReducedArea(nMinReduceArea);
	Match.SetFinalReduction(nFinalReduction);
	/*
	if ( Match.LearnPattern((int)ImageW, (int)ImageH, (int)ImageStep, (int)BitCount, ImagePtr, true) == false )
	{	
		JetMemory.free_func(ImagePtr);
		ErrorString = Match.GetErrorString();
		return false;
	}
	*/

	RECT PatRoi={0,0,0,0};	
	PatRoi.left = nImageW2-(nImageW2/2);
	PatRoi.right = nImageW2+(nImageW2/2);
	PatRoi.top = nImageH2-(nImageH2/2);
	PatRoi.bottom = nImageH2+(nImageH2/2);
	if ( Match.LearnPatternRoi((int)ImageW, (int)ImageH, (int)ImageStep, (int)BitCount, ImagePtr, true, PatRoi) == false )
	{	
		JetMemory.free_func(ImagePtr);
		ErrorString = Match.GetErrorString();
		return false;
	}

	/*
	Match.SetInterpolate(false);
	//Match.SetInterpolate(false);
	Match.SetMinScore(-1);//fMinScore
	if ( Match.Match((int)RoiW, (int)RoiH, (int)RoiStep, (int)MarkBitCount, RoiPtr, true) == false )
	{
		JetMemory.free_func(ImagePtr);
		ErrorString = Match.GetErrorString();
		return false;
	}
	*/
	JetMemory.free_func(ImagePtr);
#endif//_DEBUG
	return true;
}

bool CDebugFormView::TestBarcode()//條碼測試
{
	return true;
#ifdef _DEBUG
	size_t        i=0;
	CString       filename;	
	CJetBarcode   JetBarcode;
	IMAGE_PTR     ImagePtr=NULL;
	IMAGE_SIZE    ImageW=0;
	IMAGE_SIZE    ImageH=0;
	IMAGE_SIZE    ImageStep=0;
	IMAGE_SIZE    BitCount=0;
	char          BarcodeContent[BARCODE_CONTENT_SIZE];  

	filename = _T("C:\\DataMatrixTest.BMP");
	if ( ImageAPI.LoadImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == true ) 
	{		
		if ( 8 == BitCount )
		{
			for ( i=0; i<30; i++ )
			{	JetBarcode.DecodeBarcodeDataMatrix(ImageW, ImageH, ImageStep, ImagePtr, BarcodeContent, BARCODE_CONTENT_SIZE);	}		
		}
		JetMemory.free_func(ImagePtr);
	}
	
#endif//_DEBUG
	return true;
}

bool CDebugFormView::TestMoveFolder()//檔案移動		
{
	return true;
	CString Folder1;
	CString Folder2;
	CString FolderTmp = AOIDataCollect.GetAOITempDirectory();

	Folder1.Format(_T("%s\\%s"), FolderTmp, _T("TempFolder1"));
	Folder2.Format(_T("%s\\%s"), FolderTmp, _T("TempFolder2"));

	::CreateDirectory(Folder1, NULL);
	::Sleep(10);
	::MoveFile(Folder1, Folder2);
	return true;
}
void CDebugFormView::TestSaveLoadUnicodeTextFile()//萬國語言存檔讀檔	
{
	return;
	size_t  i=0;
	CString filename;
	FILE *pfile = NULL;
	TCHAR TMode[32] = _T("");

	//寫檔測試
	_tcscpy(TMode, _T("w+"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SaveLoadText.TXT"));
	pfile = _tfopen(filename, TMode);
	if (NULL == pfile)
	{	return;	}
	
	::_ftprintf(pfile, _T("%d, %s\n"), 100, _T("第1個"));
	::_ftprintf(pfile, _T("%d, %s\n"), 101, _T("第2個"));
	::_ftprintf(pfile, _T("%d, %s\n"), 102, _T("第3個"));
	::_ftprintf(pfile, _T("%d, %s\n"), 103, _T("第4個"));
	::_ftprintf(pfile, _T("%d, %s\n"), 104, _T("第5個"));
	::fclose(pfile); pfile = NULL;

	//讀檔測試	
	_tcscpy(TMode, _T("r"));
	JetAPI::ModifyOpenFileMode_Read(TMode);	
	pfile = _tfopen(filename, TMode);
	if (NULL == pfile)
	{	return;	}

	int Count = 0;
	std::vector<CString> strList;
	const size_t TxtSize = 256;
	TCHAR TextLine[TxtSize] = _T("");
	TCHAR TextIndex[TxtSize] = _T("");
	TCHAR TextData[TxtSize] = _T("");
	while (::_fgetts(TextLine, TxtSize, pfile) != NULL)
	{
		//strList.push_back(TextLine);
		JetAPI::DecoderTextLine(TextLine, TextIndex, TextData);
		strList.push_back(TextData);
		Count++;
	};
	::fclose(pfile); pfile = NULL;	

	//寫檔測試
	_tcscpy(TMode, _T("w+"));
	JetAPI::ModifyOpenFileMode_Write(TMode);
	filename.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("SaveLoadText-2.TXT"));
	pfile = _tfopen(filename, TMode);
	if (NULL == pfile)
	{	return;	}
	
	const size_t strCount = strList.size();
	for ( i=0; i<strCount; i++ )
	{	
		//::_ftprintf(pfile, _T("%s\n"), strList[i]);	
		::fwprintf(pfile, L"%s\n", strList[i]);
	}	
	::fclose(pfile); pfile = NULL;
	return;	
}

bool CDebugFormView::TestColorRatio()
{
	return true;

	CString  filename=_T("C:\\JETAOI3D\\Frame_00068.PNG");
	size_t     i=0, j=0, idx=0;
	double     R2=0, G2=0, B2=0;
	double     R=0, G=0, B=0, L=0;
	IMAGE_PTR  Ptr=NULL;	
	IMAGE_SIZE W=0, H=0, Step=0, Bit=0;
	if ( ImageAPI.LoadImage(filename, W, H, Step, Bit, Ptr, 4, true) == false ) 
	{	return false; }

	for ( i=0; i<H; i++ )
	{
		for ( j=0; j<W; j++ )
		{
			idx = (i*Step)+(j*3);
			B = Ptr[idx];
			G = Ptr[idx+1];
			R = Ptr[idx+2];
			L = sqrt((R*R)+(G*G)+(B*B));
			R2 = R*255/L;
			G2 = G*255/L;
			B2 = B*255/L;
			Ptr[idx]=(unsigned char)(B2);
			Ptr[idx+1]=(unsigned char)(G2);
			Ptr[idx+2]=(unsigned char)(R2);
		}
	}

	filename=_T("C:\\JETAOI3D\\Frame_00068-2.PNG");
	if ( ImageAPI.SaveImage(filename, W, H, Step, Bit, Ptr, true) == false ) 
	{
		JetMemory.free_func(Ptr);
		return false; 
	}
	JetMemory.free_func(Ptr);
	return true;
}

void CDebugFormView::TestMoments()
{
	TPOINT2D Pt;
	TPOINT2D Pt2;	
	int    i=0, j=0;
	double Err1, Err2;
	double MaxErr1=0, MaxErr2=0;
	double COS=0, SIN=0;
	double COS2=0, SIN2=0;
	double Target=0.0;
	int    MinAngle=0, MaxAngle=180;
	double W=0, H=0, Angle=0;
	double CpX=0.0, CpY=0.0;		
	double W2=0, H2=0, Angle2=0;
	double CpX2=0.0, CpY2=0.0;	
	double ResW=0, ResH=0;
	RECT      Rect;
	TREGION4D Region;
	const bool bContour=true;
	const int OrgX=200;
	const int OrgY=150;
	const int OrgW=100;
	const int OrgH=300;
	std::vector<TPOINT2D> PtList;
	std::vector<TPOINT2D> TmpList;
	std::vector<TPOINT2D> OrgList;
	
	if ( false == bContour )
	{
		Rect.left = OrgX-(OrgW/2);
		Rect.top  = OrgY-(OrgH/2);
		Rect.right= Rect.left+OrgW;
		Rect.bottom= Rect.top+OrgH;

		for ( i=Rect.top; i<=Rect.bottom; i++ )
		{
			for ( j=Rect.left; j<=Rect.right; j++ )
			{
				Pt.x = j;
				Pt.y = i;
				OrgList.push_back(Pt);
			}
		}
	}
	else
	{
		Pt.x = OrgX-(OrgW/2);	Pt.y = OrgY-(OrgH/2);	OrgList.push_back(Pt);
		Pt.x = OrgX+(OrgW/2);	Pt.y = OrgY-(OrgH/2);	OrgList.push_back(Pt);
		Pt.x = OrgX+(OrgW/2);	Pt.y = OrgY+(OrgH/2);	OrgList.push_back(Pt);
		Pt.x = OrgX-(OrgW/2);	Pt.y = OrgY+(OrgH/2);	OrgList.push_back(Pt);	
	}	
	MaxErr1=MaxErr2=0;
	TmpList = PtList = OrgList;
	const int PtCount = (int)(OrgList.size());
	for ( i=5; i<90; i++ )
	{		
		Target = i;
		MinAngle = Target-10;
		MaxAngle = Target+10;
		Angle = i*DEG2RAD;
		COS = ::cos(Angle);
		SIN = ::sin(Angle);
		COS2 = ::cos(-Angle);
		SIN2 = ::sin(-Angle);		
		Angle = 0;
		for ( j=0; j<PtCount; j++ )
		{
			Pt2 = OrgList[j];
			Pt2.x -= OrgX;
			Pt2.y -= OrgY;
			Pt.x = COS*Pt2.x-SIN*Pt2.y;
			Pt.y = SIN*Pt2.x+COS*Pt2.y;			
			Pt.x += OrgX;
			Pt.y += OrgY;
			PtList[j] = Pt;
		}		
		ImageAPI.CalcPointListMomentAngle(PtList, CpX2, CpY2, W2, H2, Angle2);
		ImageAPI.CalcPointListMinAreaRect(PtList, MinAngle, MaxAngle, 1, CpX, CpY, W, H, Angle);	
		Err1 = fabs(Angle-Target);
		Err2 = fabs(Angle2-Target);
		if ( MaxErr1 < Err1 ) { MaxErr1=Err1; }
		if ( MaxErr2 < Err2 ) { MaxErr2=Err2; }
		i = i;
	}
	return;
	switch ( 2 )
	{
	case 0://for degree 10->9.9, 9.9
		Target = 10.0;
		Pt.x = 60.96124593; Pt.y = 74.7123857; PtList.push_back(Pt);
		Pt.x = 356.4035718; Pt.y = 126.806839; PtList.push_back(Pt);
		Pt.x = 339.0387541; Pt.y = 225.2876143; PtList.push_back(Pt);
		Pt.x = 43.59642816; Pt.y = 173.193161; PtList.push_back(Pt);
		break;
	case 1://for degree 47->46.9, 47.0
		Target = 47.0;
		Pt.x = 134.2679311; Pt.y = 6.197026754; PtList.push_back(Pt);
		Pt.x = 338.8674391; Pt.y = 225.6031372; PtList.push_back(Pt);
		Pt.x = 265.7320689; Pt.y = 293.8029732; PtList.push_back(Pt);
		Pt.x = 61.13256091; Pt.y = 74.39686276; PtList.push_back(Pt);
		break;
	case 2://for degree 58.4->57.9, 58.2
		Target = 58.4;
		Pt.x = 163.9884608; Pt.y = -3.95833542; PtList.push_back(Pt);
		Pt.x = 321.1842326; Pt.y = 251.5597448; PtList.push_back(Pt);
		Pt.x = 236.0115392; Pt.y = 303.9583354; PtList.push_back(Pt);
		Pt.x = 78.8157674; Pt.y = 48.44025518; PtList.push_back(Pt);
		break;
	case 3://for degree 68.7->68.6,68.6
		Target = 68.7;
		Pt.x = 192.0968768; Pt.y = -7.916245661; PtList.push_back(Pt);
		Pt.x = 301.072246; Pt.y = 271.5911226; PtList.push_back(Pt);
		Pt.x = 207.9031232; Pt.y = 307.9162457; PtList.push_back(Pt);
		Pt.x = 98.92775405; Pt.y = 28.40887739; PtList.push_back(Pt);
		break;
	}

	ImageAPI.CalcPointListMomentAngle(PtList, CpX2, CpY2, W2, H2, Angle2);
	ImageAPI.CalcPointListMinAreaRect(PtList, MinAngle, MaxAngle, 1, CpX, CpY, W, H, Angle);	
	return ;
}

void CDebugFormView::TestMinRectArea()
{
	POINT Pt;
	int    MinAngle=0, MaxAngle=180;
	double CpX=0.0, CpY=0.0;		
	double W=0, H=0, Angle=0;
	double CpX2=0.0, CpY2=0.0;		
	double W2=0, H2=0, Angle2=0;
	
	std::vector<POINT> PtList;
	switch ( 3 )
	{
	case 0://for degree 10->9.9, 9.9
		Pt.x = 60.96124593; Pt.y = 74.7123857; PtList.push_back(Pt);
		Pt.x = 356.4035718; Pt.y = 126.806839; PtList.push_back(Pt);
		Pt.x = 339.0387541; Pt.y = 225.2876143; PtList.push_back(Pt);
		Pt.x = 43.59642816; Pt.y = 173.193161; PtList.push_back(Pt);
		break;
	case 1://for degree 47->46.9, 47.0
		Pt.x = 134.2679311; Pt.y = 6.197026754; PtList.push_back(Pt);
		Pt.x = 338.8674391; Pt.y = 225.6031372; PtList.push_back(Pt);
		Pt.x = 265.7320689; Pt.y = 293.8029732; PtList.push_back(Pt);
		Pt.x = 61.13256091; Pt.y = 74.39686276; PtList.push_back(Pt);
		break;
	case 2://for degree 58.4->57.9, 58.2
		Pt.x = 163.9884608; Pt.y = -3.95833542; PtList.push_back(Pt);
		Pt.x = 321.1842326; Pt.y = 251.5597448; PtList.push_back(Pt);
		Pt.x = 236.0115392; Pt.y = 303.9583354; PtList.push_back(Pt);
		Pt.x = 78.8157674; Pt.y = 48.44025518; PtList.push_back(Pt);
		break;
	case 3://for degree 68.7->68.6,68.6
		Pt.x = 192.0968768; Pt.y = -7.916245661; PtList.push_back(Pt);
		Pt.x = 301.072246; Pt.y = 271.5911226; PtList.push_back(Pt);
		Pt.x = 207.9031232; Pt.y = 307.9162457; PtList.push_back(Pt);
		Pt.x = 98.92775405; Pt.y = 28.40887739; PtList.push_back(Pt);
		break;
	}
	ImageAPI.CalcPointListMomentAngle(PtList, CpX2, CpY2, W2, H2, Angle2);
	ImageAPI.CalcPointListMinAreaRect(PtList, MinAngle, MaxAngle, 1, CpX, CpY, W, H, Angle);
	return;
}

void CDebugFormView::TestBlobContour()
{
	CString filename=_T("C:\\BaiduYunDownload\\Test.BMP");
	IMAGE_PTR  Ptr=NULL;
	IMAGE_SIZE W=0, H=0, Step=0, Bit=0;
	if ( ImageAPI.LoadImage(filename, W, H, Step, Bit, Ptr, 4, true) == true )
	{

		CJetBlob BlobDetector;		
		RECT TempRect={0,0,0,0};
		int  Threshold=128;	

		BlobDetector.InitialBlob();
		BlobDetector.SetBlobSortMode(BLOB_SORT_PIXELS);
		BlobDetector.SetBlobConnectivity(BLOB_CONNECTIVITY_4);
		JetAPI::SizeToRect(W, H, TempRect);
		if ( BlobDetector.GrayImageRoiBlobDetect(W, H, Step, Ptr, TempRect, Threshold, 255) == true )
		{
			CString      str;
			size_t       i=0;
			TBlobResult *BlobPtr=NULL;
			std::vector<POINT> PtList;
			std::vector<POINT> Contour;			
			double TmpCpX=0, TmpCpY=0, TmpW=0, TmpH=0, TmpAngle=0;		
			const size_t LocBlobResCount = BlobDetector.GetBlobCount();
			for ( i=0; i<LocBlobResCount; i++ )
			{
				BlobPtr=BlobDetector.GetBlobPtr(i, false);
				if ( NULL == BlobPtr ) { continue; }
				BlobDetector.GetBlobPixelList(BlobPtr, PtList);
				BlobDetector.GetBlobContour(BlobPtr, Contour);				
				
				IMAGE_PTR  BP=NULL;
				IMAGE_SIZE BW=0, BH=0, BC=0, BS=0;
				str.Format(_T("%s\\BlobPtList#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), i+1);							
				ImageAPI.CreatePointListImage(PtList, BW, BH, BS, BC, BP);
				ImageAPI.SaveImage(str, BW, BH, BS, BC, BP, true);
				JetMemory.free_func(BP);

				str.Format(_T("%s\\BlobContour#%d.BMP"), AOIDataCollect.GetAOITempDirectory(), i+1);							
				ImageAPI.CreatePointListImage(Contour, BW, BH, BS, BC, BP);
				ImageAPI.SaveImage(str, BW, BH, BS, BC, BP, true);
				JetMemory.free_func(BP);				
			}			
		}
		JetMemory.free_func(Ptr);
	}
	return ;
}

void CDebugFormView::TestKnnClassify()
{
	size_t       i=0;
	TKnnNode     Node;	
	CKNNClassify KNNClassify;
	std::vector<TKnnNode> NodeList;

	Node.Name = _T("C63發表會");
	Node.Label = 'P';
	Node.FeatureList.clear();
	Node.FeatureList.push_back(15);
	Node.FeatureList.push_back(25);
	Node.FeatureList.push_back(0);
	Node.FeatureList.push_back(5);
	Node.FeatureList.push_back(8);
	Node.FeatureList.push_back(3);
	NodeList.push_back(Node);

	Node.Name = _T("BMW i8");
	Node.Label = 'P';
	Node.FeatureList.clear();
	Node.FeatureList.push_back(35);
	Node.FeatureList.push_back(40);
	Node.FeatureList.push_back(1);
	Node.FeatureList.push_back(3);
	Node.FeatureList.push_back(3);
	Node.FeatureList.push_back(2);
	NodeList.push_back(Node);

	Node.Name = _T("林書豪");
	Node.Label = 'S';
	Node.FeatureList.clear();
	Node.FeatureList.push_back(5);
	Node.FeatureList.push_back(0);
	Node.FeatureList.push_back(35);
	Node.FeatureList.push_back(50);
	Node.FeatureList.push_back(0);
	Node.FeatureList.push_back(0);
	NodeList.push_back(Node);

	Node.Name = _T("湖人隊");
	Node.Label = 'S';
	Node.FeatureList.clear();
	Node.FeatureList.push_back(1);
	Node.FeatureList.push_back(5);
	Node.FeatureList.push_back(32);
	Node.FeatureList.push_back(15);
	Node.FeatureList.push_back(0);
	Node.FeatureList.push_back(0);
	NodeList.push_back(Node);

	Node.Name = _T("Android");
	Node.Label = 'T';
	Node.FeatureList.clear();
	Node.FeatureList.push_back(10);
	Node.FeatureList.push_back(5);
	Node.FeatureList.push_back(7);
	Node.FeatureList.push_back(0);
	Node.FeatureList.push_back(2);
	Node.FeatureList.push_back(30);
	NodeList.push_back(Node);

	Node.Name = _T("iPhone6");
	Node.Label = 'T';
	Node.FeatureList.clear();
	Node.FeatureList.push_back(5);
	Node.FeatureList.push_back(5);
	Node.FeatureList.push_back(5);
	Node.FeatureList.push_back(15);
	Node.FeatureList.push_back(8);
	Node.FeatureList.push_back(32);
	NodeList.push_back(Node);
	const size_t NodeCount = NodeList.size();

	//	?	10	2	50	56	8	5
	Node.Name = _T("騎士隊");
	Node.Label = -1;
	Node.FeatureList.clear();
	Node.FeatureList.push_back(10);
	Node.FeatureList.push_back(2);
	Node.FeatureList.push_back(50);
	Node.FeatureList.push_back(56);
	Node.FeatureList.push_back(8);
	Node.FeatureList.push_back(5);
	
	int Label=-1;
	KNNClassify.ExecKNNClassify(Node, NodeList, 3, Label);
	Node.Label = Label;

	for ( i=0; i<NodeCount; i++ )
	{
		if ( NodeList[i].Label != Label ) { continue; }
		Node.Label = Label;
	}
}

void CDebugFormView::TestKnnClassify_02()
{
	size_t       i=0;
	TKnnNode     Node;	
	CKNNClassify KNNClassify;
	std::vector<TKnnNode> NodeList;

	Node.Name = _T("OK-01");
	Node.Label = 'P';
	Node.FeatureList.clear();
	Node.FeatureList.push_back(20);
	Node.FeatureList.push_back(50);
	NodeList.push_back(Node);

	Node.Name = _T("OK-02");
	Node.Label = 'P';
	Node.FeatureList.clear();
	Node.FeatureList.push_back(30);
	Node.FeatureList.push_back(40);
	NodeList.push_back(Node);

	Node.Name = _T("OK-03");
	Node.Label = 'P';
	Node.FeatureList.clear();
	Node.FeatureList.push_back(15);
	Node.FeatureList.push_back(55);
	NodeList.push_back(Node);

	Node.Name = _T("NG-01");
	Node.Label = 'N';
	Node.FeatureList.clear();
	Node.FeatureList.push_back(20);
	Node.FeatureList.push_back(200);
	NodeList.push_back(Node);

	Node.Name = _T("NG-02");
	Node.Label = 'N';
	Node.FeatureList.clear();
	Node.FeatureList.push_back(70);
	Node.FeatureList.push_back(40);
	NodeList.push_back(Node);
	

	//	?	20 40
	Node.Name = _T("Test");
	Node.Label = -1;
	Node.FeatureList.clear();
	Node.FeatureList.push_back(20);
	Node.FeatureList.push_back(40);	
	
	int Label=-1;
	KNNClassify.ExecKNNClassify(Node, NodeList, 3, Label);
	Node.Label = Label;
	const size_t NodeCount = NodeList.size();
	for ( i=0; i<NodeCount; i++ )
	{
		if ( NodeList[i].Label != Label ) { continue; }
		Node.Label = Label;
	}
}

void CDebugFormView::TestOpenCV_SVM()
{
#ifndef OPENCV_ML_DISABLE
	int   i=0, j=0;
	const int nFeatures=6;
	const int nTrainCount=6;
    float trainingData[nTrainCount][nFeatures];
	for ( i=0; i<nTrainCount; i++ )
	{
		j=0;
		switch ( i )
		{
		case 0://_T("C63發表會");
			trainingData[i][j++]=15;
			trainingData[i][j++]=25;
			trainingData[i][j++]=0;
			trainingData[i][j++]=5;
			trainingData[i][j++]=8;
			trainingData[i][j++]=3;
			break;
		case 1://BMW i8
			trainingData[i][j++]=35;
			trainingData[i][j++]=40;
			trainingData[i][j++]=1;
			trainingData[i][j++]=3;
			trainingData[i][j++]=3;
			trainingData[i][j++]=2;
			break;
		case 2://_T("林書豪");
			trainingData[i][j++]=5;
			trainingData[i][j++]=0;
			trainingData[i][j++]=35;
			trainingData[i][j++]=50;
			trainingData[i][j++]=0;
			trainingData[i][j++]=0;
			break;
		case 3://_T("湖人隊");
			trainingData[i][j++]=1;
			trainingData[i][j++]=5;
			trainingData[i][j++]=32;
			trainingData[i][j++]=15;
			trainingData[i][j++]=0;
			trainingData[i][j++]=0;
			break;
		case 4:_T("Android");
			trainingData[i][j++]=10;
			trainingData[i][j++]=5;
			trainingData[i][j++]=7;
			trainingData[i][j++]=0;
			trainingData[i][j++]=2;
			trainingData[i][j++]=30;
			break;
		case 5://_T("iPhone6");
			trainingData[i][j++]=5;
			trainingData[i][j++]=5;
			trainingData[i][j++]=5;
			trainingData[i][j++]=15;
			trainingData[i][j++]=8;
			trainingData[i][j++]=32;
			break;
		}
	}
    cv::Mat trainingDataMat(nTrainCount, nFeatures, CV_32FC1, trainingData);

    float labels[nTrainCount] = {1, 1, 2, 2, 3, 3}; 
    cv::Mat labelsMat(nTrainCount, 1, CV_32FC1, labels);

    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

    CvSVM SVM;
    SVM.train(trainingDataMat, labelsMat, cv::Mat(), cv::Mat(), params);
    
	char  filename[256]="";
	const char *TempFolder = AOIDataCollect.GetAOITempDirectoryA();
	::sprintf(filename, "%s\\SVM_Model.XML", TempFolder);
	SVM.save(filename);

	float sample[nFeatures]={10, 2, 50, 56, 8, 5};
	cv::Mat sampleMat(1, nFeatures, CV_32FC1, sample);
	float response = SVM.predict(sampleMat);

	CvSVM SVM2;
    SVM2.load(filename);
	float response2 = SVM2.predict(sampleMat);
#endif//OPENCV_ML_DISABLE
	return;
}

void CDebugFormView::TestOpenCV_SVM_02()
{
#ifndef OPENCV_ML_DISABLE
	int   i=0, j=0;
	const int nFeatures=2;
	const int nTrainCount=5;
    float trainingData[nTrainCount][nFeatures];
	for ( i=0; i<nTrainCount; i++ )
	{
		j=0;
		switch ( i )
		{
		case 0://Pass-1
			trainingData[i][j++]=20;
			trainingData[i][j++]=50;			
			break;
		case 1://Pass-2
			trainingData[i][j++]=30;
			trainingData[i][j++]=40;			
			break;
		case 2://Pass-3
			trainingData[i][j++]=15;
			trainingData[i][j++]=55;			
			break;
		case 3://NG-1
			trainingData[i][j++]=20;
			trainingData[i][j++]=200;			
			break;
		case 4://NG-2
			trainingData[i][j++]=70;
			trainingData[i][j++]=40;			
			break;		
		}
	}
    cv::Mat trainingDataMat(nTrainCount, nFeatures, CV_32FC1, trainingData);

	
    float labels[nTrainCount] = {1, 1, 1, 2, 2}; 
    cv::Mat labelsMat(nTrainCount, 1, CV_32FC1, labels);

    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

    CvSVM SVM;
    SVM.train(trainingDataMat, labelsMat, cv::Mat(), cv::Mat(), params);

	char  filename[256]="";
	const char *TempFolder = AOIDataCollect.GetAOITempDirectoryA();
	::sprintf(filename, "%s\\SVM_Model.XML", TempFolder);
	SVM.save(filename);
    
	float sample[nFeatures]={20, 40};
	cv::Mat sampleMat(1, nFeatures, CV_32FC1, sample);
	float response = SVM.predict(sampleMat);

	CvSVM SVM2;
    SVM2.load(filename);
	float response2 = SVM2.predict(sampleMat);
#endif//OPENCV_ML_DISABLE
	return;
}

void CDebugFormView::TestDeSkew()//影像反轉
{	
	try 
	{		
		cv::Mat img;
		cv::Mat img2;//DeSkew;    
		char filename[64] = "C:\\BaiduYunDownload\\Text-0-01.bmp";
		char filename2[64] = "C:\\BaiduYunDownload\\Text-0-01-Out.bmp";
		img = cv::imread(filename, cv::IMREAD_GRAYSCALE);//預設載入為3通道喔
		cv::imwrite(filename2,img);

		cv::Moments m;	
		m = cv::moments(img, false);//需要單通道, 否則會異常
		if(abs(m.mu02) < 1e-2)
		{
			img2 = img.clone();        
		}
		else
		{
			int SZ = 20;
			float skew = m.mu11/m.mu02;
			//int   affineFlags = WARP_INVERSE_MAP|INTER_LINEAR;
			int   affineFlags = CV_WARP_INVERSE_MAP|CV_INTER_LINEAR;
		
			cv::Mat warpMat = (cv::Mat_<float>(2,3) << 1, skew, -0.5*SZ*skew, 0, 1, 0);

			float val=0.0f;
			std::vector<float> List;						
			if(warpMat.channels()==1)
			{
				cv::Mat_<float>::iterator it = warpMat.begin<float>();
				cv::Mat_<float>::iterator itend = warpMat.end<float>();
				for(;it!=itend;it++)
				{
					val = (*it);
					List.push_back(val);
				}
			}
			/*
			if(warpMat.channels()==3)
			{
				cv::Mat_<float>::iterator it = warpMat.begin<float>();
				cv::Mat_<float>::iterator itend = warpMat.end<float>();
				for(;it!=itend;it++)
				{	
					val = (*it)[0];
					List.push_back(val);
					val = (*it)[1];
					List.push_back(val);
					val = (*it)[2];
					List.push_back(val);
				}
			}*/
			img2 = cv::Mat::zeros(img.rows, img.cols, img.type());
			warpAffine(img, img2, warpMat, img2.size(),affineFlags);
		}
		cv::imwrite(filename2,img2);
	}
	catch ( cv::Exception& e )
	{
		const char* err_msg = e.what();
		JetAPI::ShowMessageBox(err_msg);
	}
	return;
} 

void CDebugFormView::TestAdaBoost()//測試AdaBoost-聯集分類器
{
	size_t i=0,j=0;
	size_t OkCnt=0;
	size_t NgCnt=0;
	int    Label=0;
	double Ai=0.0;
	double Wi=0.0;
	double Ei=0.0;
	POINT  Pt;//2維參數(X,Y)=(X1, X2);
	const size_t ValueCount=10;
	const size_t ClassifyCount=3;
	std::vector<bool>   OkList(ValueCount);		
	std::vector<bool>   TargetOkList(ValueCount);
	std::vector<POINT>  vaList(ValueCount);//value list	
	std::vector<int>    LabList(ValueCount);	
	std::vector<double> eList(ClassifyCount);//Error Ratio List
	std::vector<double> WList(ClassifyCount);//Alpha Ratio List
	std::vector<double> DList(ValueCount);//weighting list	

	InitAdaBoostDataSet(vaList, LabList);
	

	i=0;
	WList[i++]=1.0;
	WList[i++]=1.0;
	WList[i++]=1.0;

	//1. Inital Weight List
	Wi = (double)(ValueCount);
	Wi = 1.0/Wi;
	for ( i=0; i<ValueCount; i++ )
	{	DList[i] = (Wi);	}
	//D1[1/10,1/10,1/10,1/10,1/10,1/10,1/10,1/10,1/10,1/10]

	//2. Choose Min Error Ratio Classify
	const int TargetClassifyID_0 = 0;
	for ( i=0; i<ClassifyCount; i++ )
	{
		OkCnt = 0;
		NgCnt = 0;
		eList[i]=0;
		for ( j=0; j<ValueCount; j++ )
		{
			switch ( i )
			{
			case 0:
				Label = WeakClassify1(vaList[j]);
				break;
			case 1:
				Label = WeakClassify2(vaList[j]);
				break;
			case 2:
				Label = WeakClassify3(vaList[j]);
				break;
			}
			if ( LabList[j] == Label ) 
			{	
				OkCnt ++; 
				OkList[j] = true;
			}
			else
			{	
				NgCnt ++; 
				OkList[j] = false;
			}
		}
		eList[i] = NgCnt*100.0/ValueCount;
		eList[i] = eList[i]/100.0;

		Ei = 0;//誤差率為該錯誤的分類的權重和
		for ( j=0; j<ValueCount; j++ )
		{
			if ( true == OkList[j] ) { continue; }
			Ei += DList[j];
		}
		eList[i] = Ei;
		Ai = 0.5*log((1-Ei)/(Ei));

		if ( TargetClassifyID_0 == i ) 
		{	
			WList[i] = Ai; 
			TargetOkList = OkList;
		}
	}
	//計算新的權重
	//D2[1/14,1/14,1/14,1/14,1/6,1/14,1/6,1/6,1/14,1/14]
	Ei = eList[TargetClassifyID_0];
	for ( j=0; j<ValueCount; j++ )
	{
		if ( TargetOkList[j] == true )
		{
			//For OK::D(k) = D(k-1)/(2(1-e));
			DList[j] = DList[j]/(2*(1-Ei));
		}
		else
		{
			//For NG::D(k) = D(k-1)/(2e);
			DList[j] = DList[j]/(2*Ei);
		}
	}		
	
	//第3次疊代
	for ( i=0; i<ClassifyCount; i++ )
	{
		OkCnt = 0;
		NgCnt = 0;
		eList[i]=0;
		for ( j=0; j<ValueCount; j++ )
		{
			switch ( i )
			{
			case 0:
				Label = WeakClassify1(vaList[j]);
				break;
			case 1:
				Label = WeakClassify2(vaList[j]);
				break;
			case 2:
				Label = WeakClassify3(vaList[j]);
				break;
			}
			if ( LabList[j] == Label ) 
			{	
				OkCnt ++; 
				OkList[j] = true;
			}
			else
			{	
				NgCnt ++; 
				OkList[j] = false;
			}
		}
		Ei = 0;//誤差率為該錯誤的分類的權重和
		for ( j=0; j<ValueCount; j++ )
		{
			if ( true == OkList[j] ) { continue; }
			Ei += DList[j];
		}
		eList[i] = Ei;
		Ai = 0.5*log((1-Ei)/(Ei));

		if ( 1 == i ) 
		{	
			WList[i] = Ai; 
			TargetOkList = OkList;
		}
	}
	//取錯誤率最小的為主Min(eList)
	const int TargetClassifyID_1 = 1;
	//計算新的權重
	//D3=[1/22,1/22,1/6,1/6,7/66,1/6,7/66,7/66,1/22,1/22]
	Ei = eList[TargetClassifyID_1];
	for ( j=0; j<ValueCount; j++ )
	{
		if ( TargetOkList[j] == true )
		{
			//For OK::D(k) = D(k-1)/(2(1-e));
			DList[j] = DList[j]/(2*(1-Ei));
		}
		else
		{
			//For NG::D(k) = D(k-1)/(2e);
			DList[j] = DList[j]/(2*Ei);
		}
	}	

	//第4次疊代
	for ( i=0; i<ClassifyCount; i++ )
	{
		OkCnt = 0;
		NgCnt = 0;
		eList[i]=0;
		for ( j=0; j<ValueCount; j++ )
		{
			switch ( i )
			{
			case 0:
				Label = WeakClassify1(vaList[j]);
				break;
			case 1:
				Label = WeakClassify2(vaList[j]);
				break;
			case 2:
				Label = WeakClassify3(vaList[j]);
				break;
			}
			if ( LabList[j] == Label ) 
			{	
				OkCnt ++; 
				OkList[j] = true;
			}
			else
			{	
				NgCnt ++; 
				OkList[j] = false;
			}
		}
		Ei = 0;//誤差率為該錯誤的分類的權重和
		for ( j=0; j<ValueCount; j++ )
		{
			if ( true == OkList[j] ) { continue; }
			Ei += DList[j];
		}
		eList[i] = Ei;
		Ai = 0.5*log((1-Ei)/(Ei));

		if ( 2 == i ) 
		{	
			WList[i] = Ai; 
			TargetOkList = OkList;
		}
	}
	//取錯誤率最小的為主Min(eList)
	const int TargetClassifyID_2 = 2;
	//計算新的權重
	//D4=[1/6,1/6,11/114,11/114,7/114,11/114,7/114,7/114,1/6,1/38]
	Ei = eList[TargetClassifyID_2];
	for ( j=0; j<ValueCount; j++ )
	{
		if ( TargetOkList[j] == true )
		{
			//For OK::D(k) = D(k-1)/(2(1-e));
			DList[j] = DList[j]/(2*(1-Ei));
		}
		else
		{
			//For NG::D(k) = D(k-1)/(2e);
			DList[j] = DList[j]/(2*Ei);
		}
	}	

	
	//
	//f3(x)=0.4236H1(x) + 0.6496H2(x)+0.9229H3(x)。
	//最末測試
	OkCnt = 0;
	NgCnt = 0;
	double H1=0, H2=0, H3=0, Hx=0;
	for ( j=0; j<ValueCount; j++ )
	{		
		
		for ( i=0; i<ClassifyCount; i++ )
		{
			switch ( i )
			{
			case 0:
				H1 = WeakClassify1(vaList[j]);
				break;
			case 1:
				H2 = WeakClassify2(vaList[j]);
				break;
			case 2:
				H3 = WeakClassify3(vaList[j]);
				break;
			}			
		}
		Hx = WList[0]*H1+WList[1]*H2+WList[2]*H3;
		if ( Hx < 0 ) { Label = -1; }
		else { Label = 1; }
		
		if ( LabList[j] == Label ) 
		{	
			OkCnt ++; 
			OkList[j] = true;
		}
		else
		{	
			NgCnt ++; 
			OkList[j] = false;
		}
	}
	return;
}

void CDebugFormView::TestAdaBoost_02()//測試AdaBoost-聯集分類器	
{
	size_t i=0,j=0;
	size_t OkCnt=0;
	size_t NgCnt=0;
	int    Label=0;
	double Ai=0.0;
	double Wi=0.0;
	double Ei=0.0;
	POINT  Pt;//2維參數(X,Y)=(X1, X2);
	const size_t ValueCount=10;
	const size_t ClassifyCount=3;	
	std::vector<bool>   OkList(ValueCount);		
	std::vector<bool>   TargetOkList(ValueCount);
	std::vector<POINT>  vaList(ValueCount);//value list	
	std::vector<int>    LabList(ValueCount);	
	std::vector<double> eList(ClassifyCount);//Error Ratio List
	std::vector<double> WList(ClassifyCount);//Alpha Ratio List
	std::vector<double> DList(ValueCount);//weighting list	
	std::vector<std::vector<bool>> ClassifyOkList;

	InitAdaBoostDataSet(vaList, LabList);		

	i=0;
	WList[i++]=1.0;
	WList[i++]=1.0;
	WList[i++]=1.0;

	//1. Inital Weight List
	Wi = (double)(ValueCount);
	Wi = 1.0/Wi;
	for ( i=0; i<ValueCount; i++ )
	{	DList[i] = (Wi);	}
	//D1[1/10,1/10,1/10,1/10,1/10,1/10,1/10,1/10,1/10,1/10]

	for ( i=0; i<ClassifyCount; i++ )
	{		
		for ( j=0; j<ValueCount; j++ )
		{
			switch ( i )
			{
			case 0:
				Label = WeakClassify1(vaList[j]);
				break;
			case 1:
				Label = WeakClassify2(vaList[j]);
				break;
			case 2:
				Label = WeakClassify3(vaList[j]);
				break;
			}
			if ( LabList[j] == Label ) 
			{	OkList[j] = true;	}
			else
			{	OkList[j] = false;	}
		}
		ClassifyOkList.push_back(OkList);
	}

	//2. Choose Min Error Ratio Classify
	//===================================================
	int TargetClassifyID_0 = -1;
	if ( GetMinErrorClassifyID(ClassifyOkList, DList, TargetClassifyID_0, Ei) == false )
	{	return; }	
	eList[TargetClassifyID_0] = Ei;
	Ai = 0.5*log((1-Ei)/(Ei));	
	WList[TargetClassifyID_0] = Ai; 
	
	//計算新的權重
	//D2[1/14,1/14,1/14,1/14,1/6,1/14,1/6,1/6,1/14,1/14]
	if ( CalcNewDList(TargetClassifyID_0, ClassifyOkList, Ei, DList) == false )
	{	return ; }

	//===================================================
	int TargetClassifyID_1 = -1;
	if ( GetMinErrorClassifyID(ClassifyOkList, DList, TargetClassifyID_1, Ei) == false )
	{	return; }
	eList[TargetClassifyID_1] = Ei;
	Ai = 0.5*log((1-Ei)/(Ei));	
	WList[TargetClassifyID_1] = Ai; 
	//計算新的權重
	//D3=[1/22,1/22,1/6,1/6,7/66,1/6,7/66,7/66,1/22,1/22]
	if ( CalcNewDList(TargetClassifyID_1, ClassifyOkList, Ei, DList) == false )
	{	return ; }

	//===================================================
	int TargetClassifyID_2 = -1;
	if ( GetMinErrorClassifyID(ClassifyOkList, DList, TargetClassifyID_2, Ei) == false )
	{	return; }
	eList[TargetClassifyID_2] = Ei;
	Ai = 0.5*log((1-Ei)/(Ei));	
	WList[TargetClassifyID_2] = Ai; 	
	//計算新的權重
	//D4=[1/6,1/6,11/114,11/114,7/114,11/114,7/114,7/114,1/6,1/38]
	if ( CalcNewDList(TargetClassifyID_2, ClassifyOkList, Ei, DList) == false )
	{	return ; }

	//
	//f3(x)=0.4236H1(x) + 0.6496H2(x)+0.9229H3(x)。
	//最末測試
	OkCnt = 0;
	NgCnt = 0;
	double H1=0, H2=0, H3=0, Hx=0;
	for ( j=0; j<ValueCount; j++ )
	{				
		for ( i=0; i<ClassifyCount; i++ )
		{
			switch ( i )
			{
			case 0:
				H1 = WeakClassify1(vaList[j]);
				break;
			case 1:
				H2 = WeakClassify2(vaList[j]);
				break;
			case 2:
				H3 = WeakClassify3(vaList[j]);
				break;
			}			
		}
		Hx = WList[0]*H1+WList[1]*H2+WList[2]*H3;
		if ( Hx < 0 ) { Label = -1; }
		else { Label = 1; }
		
		if ( LabList[j] == Label ) 
		{	
			OkCnt ++; 
			OkList[j] = true;
		}
		else
		{	
			NgCnt ++; 
			OkList[j] = false;
		}
	}
	return;
}

void CDebugFormView::TestAdaBoost_03()//測試AdaBoost-聯集分類器	
{
	size_t i=0,j=0;
	size_t OkCnt=0;
	size_t NgCnt=0;
	int    Label=0;	
	double Wi=0.0;
	double Ei=0.0;
	double Hx=0.0;
	POINT  Pt;//2維參數(X,Y)=(X1, X2);
	int ActClassifyID = -1;
	const size_t ValueCount=10;
	const size_t ClassifyCount=3;	
	std::vector<bool>   OkList(ValueCount);		
	std::vector<bool>   TargetOkList(ValueCount);
	std::vector<POINT>  vaList(ValueCount);//value list	
	std::vector<int>    LabList(ValueCount);	
	std::vector<double> eList(ClassifyCount);//Error Ratio List
	std::vector<double> WList(ClassifyCount);//Classify Weighting number//分類器的權重
	std::vector<double> DList(ValueCount);//變數的權重
	std::vector<std::vector<bool>> ClassifyOkList;

	InitAdaBoostWList(WList);
	InitAdaBoostDataSet(vaList, LabList);

	//0. Test every classify result
	if ( CalcClassifyTestList(ClassifyCount, vaList, LabList, ClassifyOkList) == false )
	{	return ; }	

	//1. Inital Weight List
	InitAdaBoostDList(DList);	
	//D1[1/10,1/10,1/10,1/10,1/10,1/10,1/10,1/10,1/10,1/10]
	CalcTotalClassifyError(WList, vaList, LabList, Hx);

	//2. Choose Min Error Ratio Classify
	//===================================================	
	if ( GetMinErrorClassifyID(ClassifyOkList, DList, ActClassifyID, Ei) == false )
	{	return; }	
	eList[ActClassifyID] = Ei;
	Wi = 0.5*log((1-Ei)/(Ei));	
	WList[ActClassifyID] = Wi; 
	
	//計算新的權重
	//D2[1/14,1/14,1/14,1/14,1/6,1/14,1/6,1/6,1/14,1/14]
	if ( CalcNewDList(ActClassifyID, ClassifyOkList, Ei, DList) == false )
	{	return ; }
	CalcTotalClassifyError(WList, vaList, LabList, Hx);
	//===================================================	
	if ( GetMinErrorClassifyID(ClassifyOkList, DList, ActClassifyID, Ei) == false )
	{	return; }
	eList[ActClassifyID] = Ei;
	Wi = 0.5*log((1-Ei)/(Ei));	
	WList[ActClassifyID] = Wi; 	
	//計算新的權重
	//D3=[1/22,1/22,1/6,1/6,7/66,1/6,7/66,7/66,1/22,1/22]
	if ( CalcNewDList(ActClassifyID, ClassifyOkList, Ei, DList) == false )
	{	return ; }
	CalcTotalClassifyError(WList, vaList, LabList, Hx);
	//===================================================	
	if ( GetMinErrorClassifyID(ClassifyOkList, DList, ActClassifyID, Ei) == false )
	{	return; }
	eList[ActClassifyID] = Ei;
	Wi = 0.5*log((1-Ei)/(Ei));	
	WList[ActClassifyID] = Wi; 	
	//計算新的權重
	//D4=[1/6,1/6,11/114,11/114,7/114,11/114,7/114,7/114,1/6,1/38]
	if ( CalcNewDList(ActClassifyID, ClassifyOkList, Ei, DList) == false )
	{	return ; }

	//
	//f3(x)=0.4236H1(x) + 0.6496H2(x)+0.9229H3(x)。
	//最末測試	
	CalcTotalClassifyError(WList, vaList, LabList, Hx);	
	return;
}

void CDebugFormView::TestCopyFolder()
{
	CString       str;
	double        fnTime[10]={0.0};
	LARGE_INTEGER fnStart, fnEnd;
	CString SrcFolder=_T("H:\\BaiduYunDownload\\Offline_Src");
	CString DstFolder=_T("H:\\BaiduYunDownload\\Offline_Dst");
	
	JetAPI::ClearFolder(DstFolder);
	::Sleep(0);
	JetAPI::SetFuncTimeStart(fnStart);
	JetAPI::CopyFolderAToFolderB(SrcFolder, DstFolder, false, false, _T(""), -1, -1);
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime[0] = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	
	JetAPI::ClearFolder(DstFolder);
	::Sleep(0);
	JetAPI::SetFuncTimeStart(fnStart);
	JetAPI::XCopyFolderAToFolderB(SrcFolder, DstFolder, false, false, _T(""), -1, -1);
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime[1] = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	
	JetAPI::ClearFolder(DstFolder);
	::Sleep(0);
	JetAPI::SetFuncTimeStart(fnStart);
	JetAPI::RoboCopyFolderAToFolderB(SrcFolder, DstFolder, false, false, _T(""), -1, -1);
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime[2] = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);

	str.Format(_T("Copy=%.3fms, XCopy=%.3fms, RoboCopy=%.3fms"), fnTime[0], fnTime[1], fnTime[2]);
	AOIDataCollect.SaveMovingTimeMsg(str);
	JetAPI::ShowMessageBox(str);
	return ;
}

void CDebugFormView::TestDeleteFolder()
{
	CString       str;
	double        fnTime[10]={0.0};
	LARGE_INTEGER fnStart, fnEnd;
	//CString SrcFolder=_T("H:\\BaiduYunDownload\\Library_Src");
	//CString DstFolder=_T("H:\\BaiduYunDownload\\Library_Dst");
	CString SrcFolder=_T("H:\\BaiduYunDownload\\Offline_Src");
	CString DstFolder=_T("H:\\BaiduYunDownload\\Offline_Dst");
	
	JetAPI::CopyFolderAToFolderB(SrcFolder, DstFolder, false, false, _T(""), -1, -1);
	::Sleep(0);
	JetAPI::SetFuncTimeStart(fnStart);
	JetAPI::ClearFolder(DstFolder);
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime[0] = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	

	JetAPI::CopyFolderAToFolderB(SrcFolder, DstFolder, false, false, _T(""), -1, -1);
	::Sleep(0);
	JetAPI::SetFuncTimeStart(fnStart);
	JetAPI::ClearFolder_rmdir(DstFolder);
	JetAPI::SetFuncTimeEnd(fnEnd);
	fnTime[1] = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);	

	str.Format(_T("Delet=%.3fms, Rmdir=%.3fms"), fnTime[0], fnTime[1]);
	AOIDataCollect.SaveMovingTimeMsg(str);
	JetAPI::ShowMessageBox(str);
	return ;
}

int  CDebugFormView::WeakClassify1(const POINT &Pt)
{
	if ( Pt.x < 2.5 ) { return 1; }
	else { return -1; }	
}

int  CDebugFormView::WeakClassify2(const POINT &Pt)
{
	if ( Pt.x < 8.5 ) { return 1; }
	else { return -1; }	
}

int  CDebugFormView::WeakClassify3(const POINT &Pt)
{
	if ( Pt.y > 6.5 ) { return 1; }
	else { return -1; }
}

void CDebugFormView::InitAdaBoostDList(std::vector<double> &DList)
{
	size_t i=0;
	double D=0.0;
	const size_t DCount = DList.size();
	D = (double)(DCount);
	D = 1.0/D;
	for ( i=0; i<DCount; i++ )
	{	DList[i] = D;	}
	return;
}

void CDebugFormView::InitAdaBoostWList(std::vector<double> &WList)
{
	size_t i=0;	
	const size_t WCount = WList.size();	
	for ( i=0; i<WCount; i++ )
	{	WList[i] = 0;	}
	return;
}

void CDebugFormView::InitAdaBoostDataSet(std::vector<POINT> &vaList, std::vector<int> &LabList)
{
	size_t i=0;
	POINT        Pt;
	const size_t Cnt1 = vaList.size();
	const size_t Cnt2 = LabList.size();
	if ( Cnt1!=Cnt2 || Cnt1!=10 )
	{	return; }

	i=0;	
	Pt.x= 1;	Pt.y=5; vaList[i++]=Pt;
	Pt.x= 2;	Pt.y=2; vaList[i++]=Pt;
	Pt.x= 3;	Pt.y=1; vaList[i++]=Pt;
	Pt.x= 4;	Pt.y=6; vaList[i++]=Pt;
	Pt.x= 6;	Pt.y=8; vaList[i++]=Pt;
	Pt.x= 6;	Pt.y=5; vaList[i++]=Pt;
	Pt.x= 7;	Pt.y=9; vaList[i++]=Pt;
	Pt.x= 8;	Pt.y=7; vaList[i++]=Pt;
	Pt.x= 9;	Pt.y=8; vaList[i++]=Pt;
	Pt.x=10;	Pt.y=2; vaList[i++]=Pt;	

	i=0;	
	LabList[i++] =  1;
	LabList[i++] =  1;
	LabList[i++] = -1;
	LabList[i++] = -1;
	LabList[i++] =  1;
	LabList[i++] = -1;
	LabList[i++] =  1;
	LabList[i++] =  1;
	LabList[i++] = -1;
	LabList[i++] = -1;
	return;
}

bool CDebugFormView::GetMinErrorClassifyID(const std::vector<std::vector<bool>> &TestList, const std::vector<double> &DList, int &Index, double &Error)
{
	int          idx=-1;
	double       MinErr=DBL_MAX;
	size_t       i=0, j=0;
	const size_t DCount = DList.size();
	const size_t ClassifyCnt = TestList.size();
	std::vector<double> eList(ClassifyCnt);
	Error=0;
	Index=-1;
	for ( i=0; i<ClassifyCnt; i++ )
	{
		eList[i] = 0.0;
		const size_t valCount = TestList[i].size();
		if ( valCount != DCount )
		{	return false; }

		for ( j=0; j<valCount; j++ )
		{
			if ( true == TestList[i][j] ) { continue; }
			eList[i] += DList[j];
		}

		if ( -1==idx || MinErr>eList[i] )
		{
			idx = (int)(i);
			MinErr = eList[i];
		}
	}
	Index = idx;
	Error = MinErr;
	return true;
	
}

bool CDebugFormView::CalcNewDList(int ClasID, const std::vector<std::vector<bool>> &TestList, double Err, std::vector<double> &DList)
{
	const size_t ClassifyCnt = TestList.size();
	if ( ClasID<0 || ClasID>=ClassifyCnt )
	{	return false; }
	
	size_t j=0;
	const size_t DCount=DList.size();
	const size_t valCount=TestList[ClasID].size();
	if ( valCount != DCount )
	{	return false; }

	for ( j=0; j<valCount; j++ )
	{
		if ( true == TestList[ClasID][j] )
		{
			//For OK::D(k) = D(k-1)/(2(1-e));
			DList[j] = DList[j]/(2*(1-Err));
		}
		else
		{
			//For NG::D(k) = D(k-1)/(2e);
			DList[j] = DList[j]/(2*Err);
		}
	}		
	return true;
}

bool CDebugFormView::CalcClassifyTestList(size_t ClassifyCount, const std::vector<POINT> &vaList, const std::vector<int> &LabList, std::vector<std::vector<bool>> &TestList)
{	
	int          Label=0;
	size_t       i=0, j=0;
	const size_t LabCount = LabList.size();
	const size_t ValueCount = vaList.size();	
	if ( ValueCount==0 || ValueCount!=LabCount )
	{	return false; }
	
	std::vector<bool> OkList(ValueCount);
	TestList.clear();
	for ( i=0; i<ClassifyCount; i++ )
	{		
		for ( j=0; j<ValueCount; j++ )
		{
			switch ( i )
			{
			case 0:	Label = WeakClassify1(vaList[j]);	break;
			case 1:	Label = WeakClassify2(vaList[j]);	break;
			case 2:	Label = WeakClassify3(vaList[j]);	break;
			}
			if ( LabList[j] == Label ) 
			{	OkList[j] = true;	}
			else
			{	OkList[j] = false;	}
		}
		TestList.push_back(OkList);
	}
	return true;
}

bool  CDebugFormView::CalcTotalClassifyError(const std::vector<double> &ClasWList, const std::vector<POINT> &vaList, const std::vector<int> &LabList, double &Error)//計算整體的錯誤率
{
	double Hx=0.0;
	int    Label=0;
	size_t i=0, j=0, k=0;
	size_t OkCnt=0, NgCnt=0;
	const size_t valCount = vaList.size();
	const size_t LabCount = LabList.size();
	const size_t ClassifyCount = ClasWList.size();
	std::vector<int> HList(ClassifyCount);

	if ( valCount!=LabCount )
	{	return false; }	
	for ( j=0; j<valCount; j++ )
	{	
		Hx = 0;
		for ( i=0; i<ClassifyCount; i++ )
		{
			switch ( i )
			{
			case 0:	HList[i] = WeakClassify1(vaList[j]);	break;
			case 1:	HList[i] = WeakClassify2(vaList[j]);	break;
			case 2:	HList[i] = WeakClassify3(vaList[j]);	break;
			}
			Hx += ClasWList[i]*HList[i];
		}		
		if ( Hx < 0 ) { Label = -1; }
		else { Label = 1; }
		
		if ( LabList[j] == Label ) 
		{	OkCnt ++;	}
		else
		{	NgCnt ++;	}
	}
	Error = (double)(NgCnt);
	Error = Error/(double)(valCount);
	return true;
}

bool CDebugFormView::TestSortList()
{	
	CString str;
	float val=0.0f;
	const int size=1000;//4
	float val_last=0.0f;	
	double time_stl, time_sel, time_ins, time_quick, time_merge;
	LARGE_INTEGER fnEnd;
	LARGE_INTEGER fnStart;
	std::vector<float> List;
	std::vector<float> ListSort;
	std::vector<float> ListTmp;

	srand((unsigned)time( NULL ));
	for ( int i=0; i<size; i++ )
	{
		while ( true )
		{
			val = JetAPI::GetRandomValue();
			if ( val != val_last )
			{	break; }
		}; 
		List.push_back(val);
		//ListTmp.push_back(val);
		val_last = val;
	}
	//for ( int i=0; i<ListTmp.size(); i++ )
	//{	List.push_back(ListTmp[i]); }
	//ListTmp.clear();
	for ( int i=0; i<size; i++ )
	{	ListTmp.push_back(List[size-i-1]); }
	List = ListTmp;
	ListSort = List;

	JetAPI::SetFuncTimeStart(fnStart);
	std::sort(ListSort.begin(), ListSort.end());
	JetAPI::SetFuncTimeStart(fnEnd);
	time_stl = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	if ( JetAPI::CheckListSorted(ListSort) == false )
	{
		str = _T("Error, STL-Sort Fault");
		JetAPI::ShowMessageBox(str);
		return false; 
	}

	ListSort = List;
	JetAPI::SetFuncTimeStart(fnStart);
	JetAPI::SortList_Selection(ListSort);
	JetAPI::SetFuncTimeStart(fnEnd);
	time_sel = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	if ( JetAPI::CheckListSorted(ListSort) == false )
	{
		str = _T("Error, Selection-Sort Fault");
		JetAPI::ShowMessageBox(str);
		return false; 
	}

	ListSort = List;
	JetAPI::SetFuncTimeStart(fnStart);
	JetAPI::SortList_Insertion(ListSort);
	JetAPI::SetFuncTimeStart(fnEnd);
	time_ins = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	if ( JetAPI::CheckListSorted(ListSort) == false )
	{
		str = _T("Error, Insertion-Sort Fault");
		JetAPI::ShowMessageBox(str);
		return false; 
	}

	ListSort = List;
	JetAPI::SetFuncTimeStart(fnStart);
	JetAPI::SortList_QuickSort(ListSort);
	JetAPI::SetFuncTimeStart(fnEnd);
	time_quick = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	if ( JetAPI::CheckListSorted(ListSort) == false )
	{
		str = _T("Error, Quick-Sort Fault");
		JetAPI::ShowMessageBox(str);
		return false; 
	}

	ListSort = List;
	JetAPI::SetFuncTimeStart(fnStart);
	JetAPI::SortList_MergeSort(ListSort);
	JetAPI::SetFuncTimeStart(fnEnd);
	time_merge = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);
	if ( JetAPI::CheckListSorted(ListSort) == false )
	{
		str = _T("Error, Merge-Sort Fault");
		JetAPI::ShowMessageBox(str);
		return false; 
	}
	
	str.Format(_T("STL-Sort = %.3f ms\nSelection Sort = %.3f ms\nInsertion Sort = %.3f ms\nQuick Sort = %.3f ms\nMerge Sort = %.3f ms\n"), time_stl, time_sel, time_ins, time_quick, time_merge);
	JetAPI::ShowMessageBox(str);
	return true;
}

bool CDebugFormView::TestRGBConvert()
{
	bool bError=false;
	int i=0, j=0, k=0;
	float iX=0, iY=0, iZ=0;
	float oX=0, oY=0, oZ=0;
	float tL=0, ta=0, tb=0;
	unsigned char iR=0, iG=0, iB=0;
	unsigned char oR=0, oG=0, oB=0;

	bError=false;
	for ( i=0; i<256; i++ )
	{
		iR = (unsigned char)(i);
		for ( j=0; j<256; j++ )
		{
			iG = (unsigned char)(j);
			for ( k=0; k<256; k++ )
			{
				iB = (unsigned char)(k);
				ImageAPI.RGBConvertXYZ(iR, iG, iB, iX, iY, iZ);
				ImageAPI.XYZConvertLab(iX,iY, iZ, tL, ta, tb);
				ImageAPI.LabConvertXYZ(tL, ta, tb, oX, oY, oZ);
				ImageAPI.XYZConvertRGB(oX, oY, oZ, oR, oG, oB);

				if ( oR!=iR || oG!=iG || oB!=iB ) 
				{	bError = true;	}
			}
		}
	}
	return true;
}

bool CDebugFormView::TestSleep()
{
	CString str;
	DWORD TickCntD=0;
	DWORD TickCnt1=0;
	DWORD TickCnt2=0;
	DWORD SleepCnt=0;
	const int SleepTime=1;

	TickCnt1 = ::GetTickCount();
	while ( true )
	{
		TickCnt2 = ::GetTickCount();
		TickCntD = TickCnt2-TickCnt1;
		if ( TickCntD > 10000 )
		{	break; }
		SleepCnt ++;
		::Sleep(SleepTime);
	};

	str.Format(_T("Count=%d"), SleepCnt);
	JetAPI::ShowMessageBox(str);	
	return true;
}

bool CDebugFormView::TestFloatImage()
{
	const char fnName[] = "CDebugFormView::TestFloatImage";
	size_t i=0;
	CString str;
	float *DestPtr=NULL;
	float *ImagePtr=NULL;
	unsigned char *ucharPtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;	
	IMAGE_SIZE BitCount=0;
	str = _T("D:\\未命名.PNG");
	if ( ImageAPI.LoadImage(str, ImageW, ImageH, ImageStep, BitCount, ucharPtr, 4, true) == false ) 
	{	return false;	}
	const size_t BufferSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, DestPtr, fnName, "DestPtr") == false ||
		 JetMemory.alloc_func(BufferSize, ImagePtr, fnName, "ImagePtr") == false )
	{	
		JetMemory.free_func(DestPtr);
		JetMemory.free_func(ImagePtr);		
		return false; 
	}
	
	for ( i=0; i<BufferSize; i++ )
	{	ImagePtr[i] = ucharPtr[i];	}
	if ( ImageAPI.MedianSpaceGrayImage3(ImageW, ImageH, ImageStep, ImagePtr, 7, DestPtr) == false )
	{
		JetMemory.free_func(DestPtr);
		JetMemory.free_func(ImagePtr);		
		return false; 
	}

	for ( i=0; i<BufferSize; i++ )
	{
		if ( ImagePtr[i] > 255 ) { ucharPtr[i] = 255; }
		else if ( ImagePtr[i] < 0 ) { ucharPtr[i] = 0; }
		else
		{	ucharPtr[i] = (unsigned char)(ImagePtr[i]); }
	}
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FloatSrc.PNG"));
	ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, ucharPtr, true);

	for ( i=0; i<BufferSize; i++ )
	{
		if ( DestPtr[i] > 255 ) { ucharPtr[i] = 255; }
		else if ( DestPtr[i] < 0 ) { ucharPtr[i] = 0; }
		else
		{	ucharPtr[i] = (unsigned char)(DestPtr[i]); }
	}
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FloatDst.PNG"));
	ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, 8, ucharPtr, true);

	JetMemory.free_func(DestPtr);
	JetMemory.free_func(ImagePtr);
	JetMemory.free_func(ucharPtr);	
	return true;
}

bool CDebugFormView::TestBayerImage()
{
	const char fnName[] = "CDebugFormView::TestBayerImage";
	size_t     i=0;
	CString    str;	
	DWORD      TicCnt1=0;
	DWORD      TicCnt2=0;
	DWORD      TicCntD=0;
	IMAGE_PTR  DstPtr=NULL;
	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE DstStep=0;
	IMAGE_SIZE ImageStep=0;	
	IMAGE_SIZE BitCount=0;
	BAYER_PATTERN_MODE BayerMode;
	str = _T("D:\\Bayer-01.PNG");
	//str = _T("D:\\Bayer-02.BMP");
	if ( ImageAPI.LoadImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false ) 
	{	return false;	}
	if ( 8 != BitCount )
	{
		JetMemory.free_func(ImagePtr);
		return false; 
	}
	int Mode=2;
	bool IsOK = true;
	bool bSaveFile=true;
	TicCnt1 = ::GetTickCount();
	const int BitCount8 = 8;
	const int BitCount24 = 24;
	BayerMode = BAYER_PATTERN_RGGB;	
	switch ( Mode )
	{
	case 0:	IsOK = ImageAPI.DebayerGrayImage(ImageW, ImageH, ImageStep, ImagePtr, BayerMode, DstPtr);	break;
	case 1:	IsOK = ImageAPI.DebayerColorImage(ImageW, ImageH, ImageStep, ImagePtr, BayerMode, DstStep, DstPtr);	break;
	case 2:	IsOK = ImageAPI.DebayerRawColorImage(ImageW, ImageH, ImageStep, ImagePtr, BayerMode, DstStep, DstPtr);	break;
	default:	
		IsOK = false;
		break;
	}
	if ( false == IsOK )	
	{
		JetMemory.free_func(ImagePtr);
		return false; 
	}
	if ( true == bSaveFile )
	{
		str = _T("D:\\Debayer_RGGB.PNG");	
		if ( 0 != Mode ) 
		{	ImageAPI.SaveImage(str, ImageW, ImageH, DstStep, BitCount24, DstPtr, true); }
		else
		{	ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount8, DstPtr, true); }
	}

	BayerMode = BAYER_PATTERN_GRBG;	
	switch ( Mode )
	{
	case 0:	IsOK = ImageAPI.DebayerGrayImage(ImageW, ImageH, ImageStep, ImagePtr, BayerMode, DstPtr);	break;
	case 1:	IsOK = ImageAPI.DebayerColorImage(ImageW, ImageH, ImageStep, ImagePtr, BayerMode, DstStep, DstPtr);	break;
	case 2:	IsOK = ImageAPI.DebayerRawColorImage(ImageW, ImageH, ImageStep, ImagePtr, BayerMode, DstStep, DstPtr);	break;
	default:	
		IsOK = false;
		break;
	}
	if ( false == IsOK )
	{
		JetMemory.free_func(DstPtr);
		JetMemory.free_func(ImagePtr);
		return false; 
	}
	if ( true == bSaveFile )
	{
		str = _T("D:\\Debayer_GRBG.PNG");
		if ( 0 != Mode ) 
		{	ImageAPI.SaveImage(str, ImageW, ImageH, DstStep, BitCount24, DstPtr, true); }
		else
		{	ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount8, DstPtr, true); }
	}

	BayerMode = BAYER_PATTERN_GBRG;	
	switch ( Mode )
	{
	case 0:	IsOK = ImageAPI.DebayerGrayImage(ImageW, ImageH, ImageStep, ImagePtr, BayerMode, DstPtr);	break;
	case 1:	IsOK = ImageAPI.DebayerColorImage(ImageW, ImageH, ImageStep, ImagePtr, BayerMode, DstStep, DstPtr);	break;
	case 2:	IsOK = ImageAPI.DebayerRawColorImage(ImageW, ImageH, ImageStep, ImagePtr, BayerMode, DstStep, DstPtr);	break;
	default:	
		IsOK = false;
		break;
	}
	if ( false == IsOK )
	{
		JetMemory.free_func(DstPtr);
		JetMemory.free_func(ImagePtr);
		return false; 
	}
	if ( true == bSaveFile )
	{
		str = _T("D:\\Debayer_GBRG.PNG");
		if ( 0 != Mode ) 
		{	ImageAPI.SaveImage(str, ImageW, ImageH, DstStep, BitCount24, DstPtr, true); }
		else
		{	ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount8, DstPtr, true); }
	}

	BayerMode = BAYER_PATTERN_BGGR;	
	switch ( Mode )
	{
	case 0:	IsOK = ImageAPI.DebayerGrayImage(ImageW, ImageH, ImageStep, ImagePtr, BayerMode, DstPtr);	break;
	case 1:	IsOK = ImageAPI.DebayerColorImage(ImageW, ImageH, ImageStep, ImagePtr, BayerMode, DstStep, DstPtr);	break;
	case 2:	IsOK = ImageAPI.DebayerRawColorImage(ImageW, ImageH, ImageStep, ImagePtr, BayerMode, DstStep, DstPtr);	break;
	default:	
		IsOK = false;
		break;
	}
	if ( false == IsOK )
	{
		JetMemory.free_func(DstPtr);
		JetMemory.free_func(ImagePtr);
		return false; 
	}
	if ( true == bSaveFile )
	{
		str = _T("D:\\Debayer_BGGR.PNG");
		if ( 0 != Mode ) 
		{	ImageAPI.SaveImage(str, ImageW, ImageH, DstStep, BitCount24, DstPtr, true); }
		else
		{	ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount8, DstPtr, true); }
	}

	TicCnt2 = ::GetTickCount();
	TicCntD = TicCnt2-TicCnt1;
	str.Format(_T("Time Spent:%d ms"), TicCntD);
	JetAPI::ShowMessageBox(str);
	
	JetMemory.free_func(DstPtr);
	JetMemory.free_func(ImagePtr);
	return true;
}

bool CDebugFormView::TestSmoothImage()
{
	const char fnName[] = "CDebugFormView::TestSmoothImage";	
	CString    str;	
	DWORD      TicCnt1=0;
	DWORD      TicCnt2=0;
	DWORD      TicCntD=0;
	IMAGE_PTR  DstPtr=NULL;
	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE DstStep=0;
	IMAGE_SIZE ImageStep=0;	
	IMAGE_SIZE BitCount=0;
	BAYER_PATTERN_MODE BayerMode;
	str = _T("D:\\Bayer-01.PNG");
	//str = _T("D:\\Bayer-02.BMP");
	str = _T("D:\\Frame_00040.PNG");
	if ( ImageAPI.LoadImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false ) 
	{
		str = ImageAPI.GetImageApiErrorString();
		JetAPI::ShowMessageBox(str);
		return false;	
	}
	
	int i=0;
	const int MaxCount=10;
	int MorphMode = MORPH_OPEN;
	int ShpaeMode = MORPH_SHAPE_RECT;
	int NumThreads = cv::getNumThreads();
	int ThreadNum = cv::getThreadNum();
	int NumberOfCPUs = cv::getNumberOfCPUs();
	bool bOptimized = cv::useOptimized();
	cv :: setNumThreads(8);
	int NumThreads2 = cv::getNumThreads();
	TicCnt1 = ::GetTickCount();
	for ( i=0; i<MaxCount; i++ )
	{
		//if ( ImageAPI.SmoothImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, 7, DstPtr) == false )
		if ( ImageAPI.MedianImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, 7, DstPtr) == false )
		//if ( ImageAPI.MorphImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, MorphMode, ShpaeMode, 7, 1, DstPtr) == false )
		{
			JetMemory.free_func(ImagePtr);
			return false;
		}
	}

	TicCnt2 = ::GetTickCount();
	TicCntD = TicCnt2-TicCnt1;
	str.Format(_T("Time Spent:%d ms"), TicCntD/MaxCount);
	JetAPI::ShowMessageBox(str);

	JetMemory.free_func(DstPtr);
	JetMemory.free_func(ImagePtr);
	return true;
}

bool CDebugFormView::TestPyramidImage()//圖像金字塔
{
	const char fnName[] = "CDebugFormView::TestPyramidImage";
	size_t     i=0, idx=0;
	CString    str;	
	DWORD      TicCnt1=0;
	DWORD      TicCnt2=0;
	DWORD      TicCntD=0;
	const size_t Count = 4;
	const size_t Count2 = Count*2;
	IMAGE_PTR  DstPtr[Count2]={NULL};
	IMAGE_SIZE DstW[Count2]={0};
	IMAGE_SIZE DstH[Count2]={0};
	IMAGE_SIZE DstStep[Count2]={0};

	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;	
	IMAGE_SIZE ImageStep=0;	
	IMAGE_SIZE BitCount=0;

	IMAGE_PTR  TmpPtr=NULL;
	IMAGE_SIZE TmpW=0;
	IMAGE_SIZE TmpH=0;	
	IMAGE_SIZE TmpStep=0;		
	CString    Folder = AOIDataCollect.GetAOITempDirectory();
	str = _T("D:\\Pyramid.PNG");	
	//str = _T("D:\\Pyramid-2.PNG");	
	//str = _T("D:\\AppleGray.PNG");	
	if ( ImageAPI.LoadImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false ) 
	{	return false;	}
	TicCnt1 = ::GetTickCount();
		
	idx=0;
	TmpW = ImageW;
	TmpH = ImageH;
	TmpStep = ImageStep;
	TmpPtr = ImagePtr;
	for ( i=0; i<Count; i++ )
	{		
		if ( ImageAPI.PyramidDownImage(TmpW, TmpH, TmpStep, BitCount, TmpPtr, DstW[idx], DstH[idx], DstStep[idx], DstPtr[idx]) == false )
		{
			JetMemory.free_func(ImagePtr);
			for ( i=0; i<idx; i++ )
			{	JetMemory.free_func(DstPtr[i]); }
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}
		str.Format(_T("%s\\%s#%d.PNG"), Folder, _T("PyramidDown"), i+1);
		ImageAPI.SaveImage(str, DstW[idx], DstH[idx], DstStep[idx], BitCount, DstPtr[idx], true);		
		TmpW = DstW[idx];
		TmpH = DstH[idx];
		TmpStep = DstStep[idx];
		TmpPtr = DstPtr[idx];
		idx ++;
	}
	for ( i=0; i<Count; i++ )
	{		
		if ( ImageAPI.PyramidUpImage(TmpW, TmpH, TmpStep, BitCount, TmpPtr, DstW[idx], DstH[idx], DstStep[idx], DstPtr[idx]) == false )
		{
			JetMemory.free_func(ImagePtr);
			for ( i=0; i<idx; i++ )
			{	JetMemory.free_func(DstPtr[i]); }
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}
		str.Format(_T("%s\\%s#%d.PNG"), Folder, _T("PyramidUp"), i+1);
		ImageAPI.SaveImage(str, DstW[idx], DstH[idx], DstStep[idx], BitCount, DstPtr[idx], true);		
		TmpW = DstW[idx];
		TmpH = DstH[idx];
		TmpStep = DstStep[idx];
		TmpPtr = DstPtr[idx];
		idx ++;
	}
	

	TicCnt2 = ::GetTickCount();
	TicCntD = TicCnt2-TicCnt1;
	str.Format(_T("Time Spent:%d ms"), TicCntD);
	JetAPI::ShowMessageBox(str);

	for ( i=0; i<Count2; i++ )
	{	JetMemory.free_func(DstPtr[i]); }
	JetMemory.free_func(ImagePtr);
	return true;
}

bool CDebugFormView::TestPyramidImageMerge()//圖像金字塔
{	
	const char fnName[] = "CDebugFormView::TestPyramidImageMerge";
	size_t     i=0, j=0, k=0;
	CString    str;	
	CString    strL;	
	CString    strR;
	DWORD      TicCnt1=0;
	DWORD      TicCnt2=0;
	DWORD      TicCntD=0;
	const size_t Count = 6;	
	IMAGE_PTR  DstPtr=NULL;
	IMAGE_SIZE DstW=0;
	IMAGE_SIZE DstH=0;
	IMAGE_SIZE DstStep=0;

	//利用遮罩圖來標示哪些是左圖, 哪些是右圖在經過金字塔降階與昇階就可以混和出權重比例進行混色
	IMAGE_PTR  MaskPtr=NULL;
	IMAGE_SIZE MaskW=0;
	IMAGE_SIZE MaskH=0;
	IMAGE_SIZE MaskStep=0;
	IMAGE_SIZE MaskBitCount=8;

	IMAGE_PTR  ImagePtrL=NULL;
	IMAGE_SIZE ImageWL=0;
	IMAGE_SIZE ImageHL=0;	
	IMAGE_SIZE ImageStepL=0;	
	IMAGE_SIZE BitCountL=0;

	IMAGE_PTR  ImagePtrR=NULL;
	IMAGE_SIZE ImageWR=0;
	IMAGE_SIZE ImageHR=0;	
	IMAGE_SIZE ImageStepR=0;	
	IMAGE_SIZE BitCountR=0;

	IMAGE_PTR  TmpPtr=NULL;
	IMAGE_SIZE TmpW=0;
	IMAGE_SIZE TmpH=0;	
	IMAGE_SIZE TmpStep=0;		
	CString    Folder = AOIDataCollect.GetAOITempDirectory();
	//strL = _T("D:\\Pyramid.PNG");	
	strL = _T("D:\\Apple.PNG");	
	strR = _T("D:\\Orange.PNG");	
	//strL = _T("D:\\AppleGray.PNG");	
	//strR = _T("D:\\OrangeGray.PNG");	
	if ( ImageAPI.LoadImage(strL, ImageWL, ImageHL, ImageStepL, BitCountL, ImagePtrL, 4, true) == false ||
		 ImageAPI.LoadImage(strR, ImageWR, ImageHR, ImageStepR, BitCountR, ImagePtrR, 4, true) == false ) 
	{	
		JetMemory.free_func(ImagePtrL);	
		JetMemory.free_func(ImagePtrR);	
		return false;	
	}
	if ( BitCountL!=BitCountR ) 
	{
		JetMemory.free_func(ImagePtrL);	
		JetMemory.free_func(ImagePtrR);	
		return false;	
	}
	IMAGE_SIZE ImageW = MIN(ImageWL, ImageWR);
	IMAGE_SIZE ImageH = MIN(ImageHL, ImageHR);
	IMAGE_SIZE BitCount = MIN(BitCountL, BitCountR);
	TicCnt1 = ::GetTickCount();		

	MaskW = ImageW;
	MaskH = ImageH;
	MaskStep = JetAPI::GetBMPImagePixelsPerLine(MaskW, MaskBitCount, 4);
	const size_t MaskSize=MaskStep*MaskH;
	if ( JetMemory.alloc_func(MaskSize, MaskPtr, fnName, "MaskPtr") == false )
	{
		JetMemory.free_func(ImagePtrL);	
		JetMemory.free_func(ImagePtrR);	
		return false;
	}
	//左邊為100比例, 右邊為0比例, 相對於左側的圖片
	for ( i=0; i<MaskH; i++ )
	{
		for ( j=0; j<MaskW; j++ )
		{
			k = (i*MaskStep)+j;
			if ( j<(MaskW/2) )
			{	MaskPtr[k] = 0xFF; }
			else
			{	MaskPtr[k] = 0x00; }			
		}
	}
	TmpW = MaskW;
	TmpH = MaskH;
	TmpStep = MaskStep;
	TmpPtr = MaskPtr;
	MaskPtr = NULL;
	for ( i=0; i<Count; i++ )
	{		
		if ( ImageAPI.PyramidDownImage(TmpW, TmpH, TmpStep, MaskBitCount, TmpPtr, MaskW, MaskH, MaskStep, MaskPtr) == false )
		{
			JetMemory.free_func(TmpPtr);	
			JetMemory.free_func(ImagePtrL);	
			JetMemory.free_func(ImagePtrR);			
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}
		JetMemory.free_func(TmpPtr);
		str.Format(_T("%s\\%s#%d.PNG"), Folder, _T("MaskDown"), i+1);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);		
		TmpW = MaskW;
		TmpH = MaskH;
		TmpStep = MaskStep;
		TmpPtr = MaskPtr;	
		MaskPtr = NULL;
	}
	for ( i=0; i<Count; i++ )
	{	
		if ( ImageAPI.PyramidUpImage(TmpW, TmpH, TmpStep, MaskBitCount, TmpPtr, MaskW, MaskH, MaskStep, MaskPtr) == false )
		{
			JetMemory.free_func(TmpPtr);
			JetMemory.free_func(ImagePtrL);	
			JetMemory.free_func(ImagePtrR);			
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}
		JetMemory.free_func(TmpPtr);
		str.Format(_T("%s\\%s#%d.PNG"), Folder, _T("MaskUp"), i+1);
		ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);		
		TmpW = MaskW;
		TmpH = MaskH;
		TmpStep = MaskStep;
		TmpPtr = MaskPtr;	
		MaskPtr = NULL;	
	}
	MaskW = TmpW;
	MaskH = TmpH;
	MaskStep = TmpStep;
	MaskPtr = TmpPtr;	
	TmpPtr = NULL;
	
	RECT Roi;
	Roi.right = ImageWL;
	Roi.bottom = ImageHL;
	Roi.right = MIN(Roi.right, ImageWR);
	Roi.bottom = MIN(Roi.bottom, ImageHR);
	Roi.right = MIN(Roi.right, MaskW);
	Roi.bottom = MIN(Roi.bottom, MaskH);
	
	ImageAPI.BlendImage(ImageWL, ImageHL, ImageStepL, BitCountL, ImagePtrL, ImageWR, ImageHR, ImageStepR, BitCountR, ImagePtrR, MaskW, MaskH, MaskStep, MaskPtr, Roi, DstW, DstH, DstStep, DstPtr);
	str.Format(_T("%s\\%s.PNG"), Folder, _T("Blending"));
	ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	

	TicCnt2 = ::GetTickCount();
	TicCntD = TicCnt2-TicCnt1;
	str.Format(_T("Time Spent:%d ms"), TicCntD);
	JetAPI::ShowMessageBox(str);

	JetMemory.free_func(DstPtr);	
	JetMemory.free_func(MaskPtr);	
	JetMemory.free_func(ImagePtrL);	
	JetMemory.free_func(ImagePtrR);		
	return true;
}

bool CDebugFormView::TestBlendImageMerge()//混色影像合併
{
	const char fnName[] = "CDebugFormView::TestBlendImageMerge";
	size_t     i=0, j=0, k=0;
	CString    str;	
	CString    strL;	
	CString    strR;
	DWORD      TicCnt1=0;
	DWORD      TicCnt2=0;
	DWORD      TicCntD=0;
	const size_t Count = 6;	
	IMAGE_PTR  DstPtr=NULL;
	IMAGE_SIZE DstW=0;
	IMAGE_SIZE DstH=0;
	IMAGE_SIZE DstStep=0;

	//利用遮罩圖來標示哪些是左圖, 哪些是右圖在經過金字塔降階與昇階就可以混和出權重比例進行混色
	IMAGE_PTR  MaskPtr=NULL;
	IMAGE_SIZE MaskW=0;
	IMAGE_SIZE MaskH=0;
	IMAGE_SIZE MaskStep=0;
	IMAGE_SIZE MaskBitCount=8;

	IMAGE_PTR  ImagePtrL=NULL;
	IMAGE_SIZE ImageWL=0;
	IMAGE_SIZE ImageHL=0;	
	IMAGE_SIZE ImageStepL=0;	
	IMAGE_SIZE BitCountL=0;

	IMAGE_PTR  ImagePtrR=NULL;
	IMAGE_SIZE ImageWR=0;
	IMAGE_SIZE ImageHR=0;	
	IMAGE_SIZE ImageStepR=0;	
	IMAGE_SIZE BitCountR=0;

	IMAGE_PTR  TmpPtr=NULL;
	IMAGE_SIZE TmpW=0;
	IMAGE_SIZE TmpH=0;	
	IMAGE_SIZE TmpStep=0;		
	CString    Folder = AOIDataCollect.GetAOITempDirectory();
	//strL = _T("D:\\Pyramid.PNG");	
	strL = _T("D:\\Apple.PNG");	
	strR = _T("D:\\Orange.PNG");	
	//strL = _T("D:\\AppleGray.PNG");	
	//strR = _T("D:\\OrangeGray.PNG");	
	if ( ImageAPI.LoadImage(strL, ImageWL, ImageHL, ImageStepL, BitCountL, ImagePtrL, 4, true) == false ||
		 ImageAPI.LoadImage(strR, ImageWR, ImageHR, ImageStepR, BitCountR, ImagePtrR, 4, true) == false ) 
	{	
		JetMemory.free_func(ImagePtrL);	
		JetMemory.free_func(ImagePtrR);	
		return false;	
	}
	if ( BitCountL!=BitCountR ) 
	{
		JetMemory.free_func(ImagePtrL);	
		JetMemory.free_func(ImagePtrR);	
		return false;	
	}
	IMAGE_SIZE ImageW = MIN(ImageWL, ImageWR);
	IMAGE_SIZE ImageH = MIN(ImageHL, ImageHR);
	IMAGE_SIZE BitCount = MIN(BitCountL, BitCountR);
	TicCnt1 = ::GetTickCount();		

	MaskW = ImageW;
	MaskH = ImageH;
	MaskStep = JetAPI::GetBMPImagePixelsPerLine(MaskW, MaskBitCount, 4);
	const size_t MaskSize=MaskStep*MaskH;
	if ( JetMemory.alloc_func(MaskSize, MaskPtr, fnName, "MaskPtr") == false )
	{
		JetMemory.free_func(ImagePtrL);	
		JetMemory.free_func(ImagePtrR);	
		return false;
	}
	//左邊為100比例, 右邊為0比例, 相對於左側的圖片
	for ( i=0; i<MaskH; i++ )
	{
		for ( j=0; j<MaskW; j++ )
		{
			k = (i*MaskStep)+j;
			if ( j<(MaskW/2) )
			{	MaskPtr[k] = 0xFF; }
			else
			{	MaskPtr[k] = 0x00; }			
		}
	}
	TmpW = MaskW;
	TmpH = MaskH;
	TmpStep = MaskStep;
	TmpPtr = MaskPtr;
	MaskPtr = NULL;
	const int KerSize=(TmpW/2)+1;
	if ( ImageAPI.SmoothImage(TmpW, TmpH, TmpStep, MaskBitCount, TmpPtr, KerSize, MaskPtr) == false )
	//if ( ImageAPI.GaussianImage(TmpW, TmpH, TmpStep, MaskBitCount, TmpPtr, KerSize, MaskPtr) == false )
	{
		JetMemory.free_func(TmpPtr);	
		JetMemory.free_func(ImagePtrL);	
		JetMemory.free_func(ImagePtrR);			
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("MaskBlur"));
	ImageAPI.SaveImage(str, MaskW, MaskH, MaskStep, MaskBitCount, MaskPtr, true);		
	JetMemory.free_func(TmpPtr);	

	
	RECT Roi;
	Roi.right = ImageWL;
	Roi.bottom = ImageHL;
	Roi.right = MIN(Roi.right, ImageWR);
	Roi.bottom = MIN(Roi.bottom, ImageHR);
	Roi.right = MIN(Roi.right, MaskW);
	Roi.bottom = MIN(Roi.bottom, MaskH);
	
	ImageAPI.BlendImage(ImageWL, ImageHL, ImageStepL, BitCountL, ImagePtrL, ImageWR, ImageHR, ImageStepR, BitCountR, ImagePtrR, MaskW, MaskH, MaskStep, MaskPtr, Roi, DstW, DstH, DstStep, DstPtr);
	str.Format(_T("%s\\%s.PNG"), Folder, _T("Blending2"));
	ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	

	TicCnt2 = ::GetTickCount();
	TicCntD = TicCnt2-TicCnt1;
	str.Format(_T("Time Spent:%d ms"), TicCntD);
	JetAPI::ShowMessageBox(str);

	JetMemory.free_func(DstPtr);	
	JetMemory.free_func(MaskPtr);	
	JetMemory.free_func(ImagePtrL);	
	JetMemory.free_func(ImagePtrR);		
	return true;
}

bool CDebugFormView::TestOpenCVMat()//cv::Mat不是CvMat
{
	const char fnName[]="CDebugFormView::TestOpenCVMat";
#ifndef OPENCV_DISABLE
	
	int End=0;
	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;

	IMAGE_SIZE RoiW=0;
	IMAGE_SIZE RoiH=0;
	IMAGE_SIZE RoiStep=0;
	IMAGE_PTR  RoiPtr=NULL;
	
	
	SPACE_PTR  TmpPtr=NULL;	

	CString str;
	CString ErrorString;	
	char filename[128]="";
	const int nAlign = 4;
	//CV_8UC1:8位元1通道灰階
	//CV_8UC3:8位元3通道彩色

	int i=0, j=0;
	int nType = 0;
	int nElemSize=0;
	char TypeText[64]="";
	for ( i=CV_8U; i<CV_USRTYPE1; i++ )
	{
		for ( j=1; j<4; j++ )
		{
			nType = CV_MAKETYPE(i, j);
			ImageAPI.GetOpenCVDataTypeText(nType, TypeText);
			nElemSize=CV_ELEM_SIZE(nType);
			j = j;
		}
		i = i;
	}
	
	

	End=0;
	try
	{
		str = _T("D:\\girl_color-255.png");
		if ( ImageAPI.LoadImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, nAlign, true) == false )
		{	return false; }
		RoiW=ImageW/2;
		RoiH=ImageH/2;
		RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, nAlign);
		const size_t RoiSize=ImageAPI.CalcBufferSize(RoiStep, RoiH);
		const size_t BufferSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);	
		if ( JetMemory.alloc_func(BufferSize, TmpPtr, fnName, "TmpPtr") == false || JetMemory.alloc_func(RoiSize, RoiPtr, fnName, "RoiPtr") == false )
		{
			JetMemory.free_func(RoiPtr);
			JetMemory.free_func(TmpPtr);
			JetMemory.free_func(ImagePtr);
			return false;
		}

		cv::Mat MatRoi = ImageAPI.CreateMat(RoiW, RoiH, RoiStep, BitCount, RoiPtr);		
		cv::Mat MatFlt = ImageAPI.CreateMat(ImageW, ImageH, ImageStep, BitCount, TmpPtr);
		cv::Mat MatSrc = ImageAPI.CreateMat(ImageW, ImageH, ImageStep, BitCount, ImagePtr);				
		cv::Mat MatDst=MatSrc;
		cv::Mat MatDst2;

		MatSrc.convertTo(MatFlt, CV_32F);
		MatFlt.convertTo(MatDst2, CV_8U);

		//getRectSubPix
		//cv::blur(MatSrc, MatDst, cv::Size(7, 7));

		const float CpX=ImageW*0.5f;
		const float CpY=ImageH*0.5f;
		cv::getRectSubPix(MatSrc, cv::Size(RoiW,RoiH), cv::Point2f(CpX, CpY), MatRoi);
		void *roi_ptr=MatRoi.data;

		::sprintf(filename, "%s", "d:\\girl_gray-org.png");
		cv::imwrite(filename, MatSrc);
		::sprintf(filename, "%s", "d:\\girl_gray-blur.png");
		cv::imwrite(filename, MatDst);
		::sprintf(filename, "%s", "d:\\girl_gray-cvrt.png");
		cv::imwrite(filename, MatDst2);
		::sprintf(filename, "%s", "d:\\girl_roi.png");
		cv::imwrite(filename, MatRoi);
		JetMemory.free_func(RoiPtr);
		JetMemory.free_func(TmpPtr);
		JetMemory.free_func(ImagePtr);
	}
	catch ( cv::Exception& e )
	{	
		const char* msg_e = e.what();  	
		ErrorString = msg_e;
		JetMemory.free_func(RoiPtr);
		JetMemory.free_func(TmpPtr);
		JetMemory.free_func(ImagePtr);		
		return false;
	}
	
	 End = 3;	 
#endif//OPENCV_DISABLE
	return true;
}

bool  CDebugFormView::TestOpenCVMatList()//cv::Mat不是CvMat
{
	const char fnName[]="CDebugFormView::TestOpenCVMatList";
#ifndef OPENCV_DISABLE
	
	int End=0;
	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	
	
	SPACE_PTR  TmpPtr=NULL;	

	CString str;
	CString ErrorString;	
	char filename[128]="";
	
	//CV_8UC1:8位元1通道灰階
	//CV_8UC3:8位元3通道彩色
	End=0;
	try
	{		
		size_t i=0;
		cv::Mat MatTmp;
		const size_t Count=5;
		std::vector<cv::Mat> MatList;
		for ( i=0; i<Count; i++ )
		{
			MatTmp.create(480, 640, CV_8UC3);			
			MatList.push_back(MatTmp);
			MatTmp.release();
		}
		for ( i=0; i<Count; i++ )
		{
			MatList[i].release();
		}
		
	}
	catch ( cv::Exception& e )
	{	
		const char* msg_e = e.what();  	
		ErrorString = msg_e;		
		return false;
	}
	
	 End = 3;	 
#endif//OPENCV_DISABLE
	return true;
}

bool CDebugFormView::TestRoateImage()
{	
	const char fnName[]="CDebugFormView::TestRoateImage";
#ifndef OPENCV_DISABLE
	
	int End=0;
	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	
	
	SPACE_PTR  TmpPtr=NULL;	

	CString str;
	CString ErrorString;	
	char DataType[32]="";
	char filename[128]="";
	
	//CV_8UC1:8位元1通道灰階
	//CV_8UC3:8位元3通道彩色
	End=0;
	try
	{	
		int i=0, j=0, k=0;
		double  angle=0;
		double  scale=1.0;
		const int nRows=2;
		const int nCols=3;
		double elem[nRows][nCols]={0};
		cv::Mat MatSrc;
		cv::Mat MatDst;
		cv::Mat MatWarp;
		cv::Point2f center;
		CvMat* warp_mat = ::cvCreateMat( nRows, nCols, CV_32FC1 );

		::strcpy(filename, "D:\\girl_color-255.png");
		MatSrc = cv::imread(filename);		
		int w = MatSrc.cols;
		int h = MatSrc.rows;
		int d = (int)(sqrt((w*w)+(h*h))+0.99);
		int ofx=(d-w)/2;
		int ofy=(d-h)/2;
		//::cvWarpAffine(MatSrc, MatDst, warp_mat);
		center.x = (w*0.5f);
		center.y = (h*0.5f);
		angle = 30;
		MatWarp = cv::getRotationMatrix2D(center, angle, scale);		
		
		const int MatWarpType = MatWarp.type();
		ImageAPI.GetOpenCVDataTypeText(MatWarpType, DataType);
		
		 ///*
		double &ox=MatWarp.at<double>(0, 2, 0);
		double &oy=MatWarp.at<double>(1, 2, 0);
		double &ox2=MatWarp.at<double>(0, 2, 0);
		double &oy2=MatWarp.at<double>(1, 2, 0);
		ox += ofx;//移至目標影像中央-X
		oy += ofy;//移至目標影像中央-Y
		
		k=0;
		double *Ptr=(double*)(MatWarp.data);
		for ( i=0; i<nRows; i++ )
		{
			const double* Mi = MatWarp.ptr<double>(i);
			for ( j=0; j<nCols; j++ )
			{
				elem[i][j] = Ptr[k++];//注意影像行列的步長Step
				//elem[i][j] = Mi[j];
			}
		}
		
		cv::warpAffine(MatSrc, MatDst, MatWarp, cv::Size(d,d));

		::strcpy(filename, "D:\\girl_color-Rot.png");
		cv::imwrite(filename, MatDst);

		cvReleaseMat(&warp_mat);

	}
	catch ( cv::Exception& e )
	{	
		const char* msg_e = e.what();  	
		ErrorString = msg_e;		
		return false;
	}
	
	 End = 3;	 
#endif//OPENCV_DISABLE
	return true;
}

bool CDebugFormView::TestRoateImage2()
{
	const char fnName[]="CDebugFormView::TestRoateImage2";
#ifndef OPENCV_DISABLE
	
	int End=0;
	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	
	
	IMAGE_PTR  DstPtr=NULL;
	IMAGE_SIZE DstW=0;
	IMAGE_SIZE DstH=0;
	IMAGE_SIZE DstStep=0;

	CString str;
	CString ErrorString;	
	char DataType[32]="";
	char filename[128]="";
	
	End=0;
	try
	{	
		::strcpy(filename, "D:\\Frame_00040.png");
		str = filename;
		if ( ImageAPI.LoadImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false )
		{	return false; }		
		const IMAGE_SIZE ImageD = ImageAPI.CalcRotateImageSize(ImageW, ImageH);
		DstW = DstH = ImageD;
		DstStep = JetAPI::GetBMPImagePixelsPerLine(DstW, BitCount, 4);
		const size_t DstBufferSize=ImageAPI.CalcBufferSize(DstStep, DstH);
		if ( JetMemory.alloc_func(DstBufferSize, DstPtr, fnName, "DstPtr") == false )
		{
			JetMemory.free_func(ImagePtr);
			return false;
		}


		int i=0, j=0, k=0;
		double  angle=0;
		double  scale=1.0;
		DWORD   TickCnt1=0;
		DWORD   TickCnt2=0;
		DWORD   TickCntD1=0;
		DWORD   TickCntD2=0;
		const int nRows=2;
		const int nCols=3;
		double elem[nRows][nCols]={0};
		cv::Mat MatWarp;
		cv::Point2f center;		
		cv::Mat MatDst = ImageAPI.CreateMat(DstW, DstH, DstStep, BitCount, DstPtr);
		cv::Mat MatSrc = ImageAPI.CreateMat(ImageW, ImageH, ImageStep, BitCount, ImagePtr);

		int w = (int)(ImageW);
		int h = (int)(ImageH);
		int d = (int)(ImageD);
		int ofx=(d-w)/2;
		int ofy=(d-h)/2;
		//::cvWarpAffine(MatSrc, MatDst, warp_mat);
		center.x = (w*0.5f);
		center.y = (h*0.5f);
		angle = 90;
		MatWarp = cv::getRotationMatrix2D(center, angle, scale);		
		
		const int MatWarpType = MatWarp.type();
		ImageAPI.GetOpenCVDataTypeText(MatWarpType, DataType);
		double &ox=MatWarp.at<double>(0, 2, 0);
		double &oy=MatWarp.at<double>(1, 2, 0);		
		ox += ofx;//移至目標影像中央-X
		oy += ofy;//移至目標影像中央-Y
		
		k=0;
		double *Ptr=(double*)(MatWarp.data);
		for ( i=0; i<nRows; i++ )
		{
			const double* Mi = MatWarp.ptr<double>(i);
			for ( j=0; j<nCols; j++ )
			{
				elem[i][j] = Ptr[k++];//注意影像行列的步長Step
				//elem[i][j] = Mi[j];
			}
		}		
		
		::memset(DstPtr, 0x00, sizeof(IMAGE_DATA)*DstBufferSize);
		TickCnt1 = ::GetTickCount();
		cv::warpAffine(MatSrc, MatDst, MatWarp, cv::Size(d,d));
		TickCnt2 = ::GetTickCount();
		TickCntD1 = TickCnt2-TickCnt1;

		::strcpy(filename, "D:\\girl_color-Rot.png");
		cv::imwrite(filename, MatDst);

		::memset(DstPtr, 0x00, sizeof(IMAGE_DATA)*DstBufferSize);
		TickCnt1 = ::GetTickCount();
		ImageAPI.RotateImage(angle, ImageW, ImageH, ImageStep, BitCount, ImagePtr, DstW, DstH, DstStep, DstPtr);
		TickCnt2 = ::GetTickCount();
		TickCntD2 = TickCnt2-TickCnt1;

		::strcpy(filename, "D:\\girl_color-Rot2.png");
		//cv::imwrite(filename, MatDst);
		str = filename;
		ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);
		
		str.Format(_T("ImageAPI Time=%d ms, OpenCV Time=%d ms"), TickCntD2, TickCntD1);
		JetAPI::ShowMessageBox(str);

		JetMemory.free_func(DstPtr);
		JetMemory.free_func(ImagePtr);

	}
	catch ( cv::Exception& e )
	{	
		const char* msg_e = e.what();  	
		ErrorString = msg_e;		
		JetMemory.free_func(DstPtr);
		JetMemory.free_func(ImagePtr);
		return false;
	}
	
	 End = 3;	 
#endif//OPENCV_DISABLE
	return true;
}

bool CDebugFormView::TestExtractRoi()
{
	const char fnName[]="CDebugFormView::TestExtractRoi";
#ifndef OPENCV_DISABLE
	
	int End=0;
	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	
	RECT RoiRect={0};
	IMAGE_PTR  DstPtr=NULL;
	IMAGE_SIZE DstW=0;
	IMAGE_SIZE DstH=0;
	IMAGE_SIZE DstStep=0;

	CString str;
	CString ErrorString;	
	char DataType[32]="";
	char filename[128]="";
	
	End=0;
	try
	{	
		::strcpy(filename, "D:\\Frame_00040.png");
		str = filename;
		if ( ImageAPI.LoadImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false )
		{	return false; }		
		const IMAGE_SIZE ImageD = ImageAPI.CalcRotateImageSize(ImageW, ImageH);
		DstW = ImageW/2;
		DstH = ImageH/2;
		RoiRect.left = (ImageW-DstW)/2;
		RoiRect.top  = (ImageH-DstH)/2;
		RoiRect.right = RoiRect.left+DstW;
		RoiRect.bottom = RoiRect.top+DstH;

		DstStep = JetAPI::GetBMPImagePixelsPerLine(DstW, BitCount, 4);
		const size_t DstBufferSize=ImageAPI.CalcBufferSize(DstStep, DstH);
		if ( JetMemory.alloc_func(DstBufferSize, DstPtr, fnName, "DstPtr") == false )
		{
			JetMemory.free_func(ImagePtr);
			return false;
		}


		int i=0, j=0, k=0;
		double  angle=0;
		double  scale=1.0;
		DWORD   TickCnt1=0;
		DWORD   TickCnt2=0;
		DWORD   TickCntD1=0;
		DWORD   TickCntD2=0;
		cv::Rect cvRect;
		cv::Mat MatDst = ImageAPI.CreateMat(DstW, DstH, DstStep, BitCount, DstPtr);
		cv::Mat MatSrc = ImageAPI.CreateMat(ImageW, ImageH, ImageStep, BitCount, ImagePtr);

		cvRect.x = RoiRect.left;
		cvRect.y = RoiRect.top;
		cvRect.width = RoiRect.right-RoiRect.left;
		cvRect.height = RoiRect.bottom-RoiRect.top;		

		::memset(DstPtr, 0x00, sizeof(IMAGE_DATA)*DstBufferSize);		
		TickCnt1 = ::GetTickCount();
		MatSrc(cvRect).copyTo(MatDst);		
		TickCnt2 = ::GetTickCount();
		TickCntD1 = TickCnt2-TickCnt1;

		::strcpy(filename, "D:\\girl_color-Roi.png");
		cv::imwrite(filename, MatDst);

		::memset(DstPtr, 0x00, sizeof(IMAGE_DATA)*DstBufferSize);
		::strcpy(filename, "D:\\girl_color-Roi_empty.png");
		cv::imwrite(filename, MatDst);

		TickCnt1 = ::GetTickCount();
		ImageAPI.ExtractRoiImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, DstStep, DstPtr, false);
		TickCnt2 = ::GetTickCount();
		TickCntD2 = TickCnt2-TickCnt1;

		::strcpy(filename, "D:\\girl_color-Roi2.png");
		cv::imwrite(filename, MatDst);
		
		str.Format(_T("ImageAPI Time=%d ms, OpenCV Time=%d ms"), TickCntD2, TickCntD1);
		JetAPI::ShowMessageBox(str);

		JetMemory.free_func(DstPtr);
		JetMemory.free_func(ImagePtr);

	}
	catch ( cv::Exception& e )
	{	
		const char* msg_e = e.what();  	
		ErrorString = msg_e;		
		JetMemory.free_func(DstPtr);
		JetMemory.free_func(ImagePtr);
		return false;
	}
	
	 End = 3;	 
#endif//OPENCV_DISABLE
	return true;
}

bool CDebugFormView::TestMorphImage()
{
	const char fnName[] = "CDebugFormView::TestMorphImage";
	size_t     i=0, idx=0;
	CString    str;	
	DWORD      TicCnt1=0;
	DWORD      TicCnt2=0;
	DWORD      TicCntD=0;
	IMAGE_PTR  DstPtr=NULL;
	IMAGE_SIZE DstW=0;
	IMAGE_SIZE DstH=0;
	IMAGE_SIZE DstStep=0;

	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;	
	IMAGE_SIZE ImageStep=0;	
	IMAGE_SIZE BitCount=0;

	CString    Folder = AOIDataCollect.GetAOITempDirectory();
	str = _T("D:\\Pyramid.PNG");	
	//str = _T("D:\\Pyramid-2.PNG");	
	str = _T("D:\\AppleGray.PNG");	
	if ( ImageAPI.LoadImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false ) 
	{	return false;	}
	TicCnt1 = ::GetTickCount();
		
	idx=0;	

	int KenSize = 5;
	int IterCount = 1;
	int MorphMode = 0;
	int ShapeMode = MORPH_SHAPE_RECT;
	DstW = ImageW;
	DstH = ImageH;
	DstStep = ImageStep;

	TicCnt1 = ::GetTickCount();
	MorphMode = MORPH_OPEN;
	if ( ImageAPI.MorphImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, MorphMode, ShapeMode, KenSize, IterCount, DstPtr)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("Morph_Open"));
	ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	

	MorphMode = MORPH_CLOSE;
	if ( ImageAPI.MorphImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, MorphMode, ShapeMode, KenSize, IterCount, DstPtr)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("Morph_Close"));
	ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	

	MorphMode = MORPH_GRADIENT;
	if ( ImageAPI.MorphImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, MorphMode, ShapeMode, KenSize, IterCount, DstPtr)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("Morph_Gradient"));
	ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	

	MorphMode = MORPH_TOPHAT;
	if ( ImageAPI.MorphImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, MorphMode, ShapeMode, KenSize, IterCount, DstPtr)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("Morph_TopHat"));
	ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	

	MorphMode = MORPH_BLACKHAT;
	if ( ImageAPI.MorphImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, MorphMode, ShapeMode, KenSize, IterCount, DstPtr)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("Morph_BlackHat"));
	ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	

	

	TicCnt2 = ::GetTickCount();
	TicCntD = TicCnt2-TicCnt1;
	str.Format(_T("Time Spent:%d ms"), TicCntD);
	JetAPI::ShowMessageBox(str);

	JetMemory.free_func(DstPtr);
	JetMemory.free_func(ImagePtr);
	return true;
}

bool CDebugFormView::TestErodeDilateImage()
{
	const char fnName[] = "CDebugFormView::TestErodeDilateImage";
	size_t     i=0, idx=0;
	CString    str;	
	DWORD      TicCnt1=0;
	DWORD      TicCnt2=0;
	DWORD      TicCntD=0;
	IMAGE_PTR  DstPtr=NULL;
	IMAGE_SIZE DstW=0;
	IMAGE_SIZE DstH=0;
	IMAGE_SIZE DstStep=0;

	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;	
	IMAGE_SIZE ImageStep=0;	
	IMAGE_SIZE BitCount=0;

	CString    Folder = AOIDataCollect.GetAOITempDirectory();
	str = _T("D:\\Pyramid.PNG");	
	//str = _T("D:\\Pyramid-2.PNG");	
	str = _T("D:\\AppleGray.PNG");	
	if ( ImageAPI.LoadImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false ) 
	{	return false;	}
	TicCnt1 = ::GetTickCount();
		
	idx=0;	

	int KenSize = 5;
	int IterCount = 1;	
	int ShapeMode = MORPH_SHAPE_RECT;
	DstW = ImageW;
	DstH = ImageH;
	DstStep = ImageStep;

	TicCnt1 = ::GetTickCount();
	if ( ImageAPI.DilateImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, KenSize, IterCount, DstPtr)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("Dilate"));
	ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	
	
	if ( ImageAPI.ErodeImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, KenSize, IterCount, DstPtr)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("Erode"));
	ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	

	TicCnt2 = ::GetTickCount();
	TicCntD = TicCnt2-TicCnt1;
	str.Format(_T("Time Spent:%d ms"), TicCntD);
	JetAPI::ShowMessageBox(str);

	JetMemory.free_func(DstPtr);
	JetMemory.free_func(ImagePtr);
	return true;
}

bool CDebugFormView::TestGaussianImage()
{
	const char fnName[] = "CDebugFormView::TestGaussianImage";
	size_t     i=0, idx=0;
	CString    str;	
	DWORD      TicCnt1=0;
	DWORD      TicCnt2=0;
	DWORD      TicCntD=0;
	IMAGE_PTR  DstPtr=NULL;
	IMAGE_SIZE DstW=0;
	IMAGE_SIZE DstH=0;
	IMAGE_SIZE DstStep=0;

	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;	
	IMAGE_SIZE ImageStep=0;	
	IMAGE_SIZE BitCount=0;

	IMAGE_SIZE RoiW = 0;
	IMAGE_SIZE RoiH = 0;
	IMAGE_SIZE RoiStep=0;

	const int  nAlign = 4;
	CString    Folder = AOIDataCollect.GetAOITempDirectory();
	str = _T("D:\\Pyramid.PNG");	
	//str = _T("D:\\Pyramid-2.PNG");	
	//str = _T("D:\\AppleGray.PNG");	
	//str = _T("D:\\girl_color-255.PNG");		
	//str = _T("D:\\girl_gray-255.PNG");
	//str = _T("R:\\girl_color-255.PNG");
	str = _T("D:\\Frame_00040.PNG");	
	if ( ImageAPI.LoadImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false ) 
	{	return false;	}
	TicCnt1 = ::GetTickCount();
		
	idx=0;		
	int KenSize = 11;	
	DstW = ImageW;
	DstH = ImageH;
	DstStep = ImageStep;

	TicCnt1 = ::GetTickCount();
	str.Format(_T("%s\\%s.PNG"), Folder, _T("Org"));
	ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);	
	str.Format(_T("%s\\%s.PNG"), Folder, _T("Org_Inv"));
	ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, false);	

	if ( ImageAPI.SmoothImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, KenSize, DstPtr)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("Smooth"));
	ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	

	if ( ImageAPI.GaussianImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, KenSize, DstPtr)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("Gaussian"));
	ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	

	if ( ImageAPI.MedianImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, KenSize, DstPtr)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("Median"));
	ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	
	
	if ( ImageAPI.SobelImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, DstPtr)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("Sobel"));
	ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	

	if ( 24 == BitCount )
	{
		if ( ImageAPI.SobelColorImage(ImageW, ImageH, ImageStep, ImagePtr, DstPtr)==false )
		{
			JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
			return false;
		}
		str.Format(_T("%s\\%s.PNG"), Folder, _T("Sobel-24Bit"));
		ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	
	}

	if ( ImageAPI.ScharrImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, DstPtr)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("Scharr"));
	ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	

	int Radius = 5;
	int ThL = 5;
	int Amount = 150;
	if ( ImageAPI.SharpnessGausImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, Radius, ThL, Amount, DstPtr)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("SharpnessGaus"));
	ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	
	
	if ( ImageAPI.SharpnessLaps2Image(ImageW, ImageH, ImageStep, BitCount, ImagePtr, DstPtr, 3)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("SharpnessLaps2"));
	ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	

	TRECT4D RoiRect4D;
	const double CpX=ImageW*0.5;
	const double CpY=ImageH*0.5;
	RoiRect4D.left = CpX*0.5;
	RoiRect4D.top  = CpY*0.5;
	RoiRect4D.right = RoiRect4D.left+CpX;
	RoiRect4D.bottom = RoiRect4D.top+CpY;
	RoiW = JetAPI::Floor(RoiRect4D.right-RoiRect4D.left);
	RoiH = JetAPI::Floor(RoiRect4D.bottom-RoiRect4D.top);
	RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, nAlign);
	if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect4D, RoiStep, DstPtr, false)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("RoiSubPix"));
	ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, BitCount, DstPtr, true);	

	if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect4D, RoiStep, DstPtr, true)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("RoiSubPix-Rev"));
	ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, BitCount, DstPtr, true);	

	if ( 24 == BitCount )
	{
		unsigned char *pR=NULL;
		unsigned char *pG=NULL;
		unsigned char *pB=NULL;

		unsigned char *pRoiR=NULL;
		unsigned char *pRoiG=NULL;
		unsigned char *pRoiB=NULL;
		unsigned char *pRoiC=NULL;
		IMAGE_SIZE GrayBitCnt=8;
		IMAGE_SIZE GrayStep=JetAPI::GetBMPImagePixelsPerLine(ImageW, GrayBitCnt, nAlign);
		if ( ImageAPI.ColorImageToRGBImage(ImageW, ImageH, ImageStep, ImagePtr, pR, pG, pB, GrayStep, false) == false )
		{
			str = ImageAPI.GetImageApiErrorString();
			JetAPI::ShowMessageBox(str);
			JetMemory.free_func(DstPtr);
			JetMemory.free_func(ImagePtr);
			return false;
		}		
		str.Format(_T("%s\\%s.PNG"), Folder, _T("RGBImage"));
		ImageAPI.SaveRGBImage(str, ImageW, ImageH, GrayStep, pR, pG, pB, true);	
		str.Format(_T("%s\\%s.JPG"), Folder, _T("RGBImage"));
		ImageAPI.SaveRGBImage(str, ImageW, ImageH, GrayStep, pR, pG, pB, true);	
		str.Format(_T("%s\\%s.BMP"), Folder, _T("RGBImage"));
		ImageAPI.SaveRGBImage(str, ImageW, ImageH, GrayStep, pR, pG, pB, true);	
		str.Format(_T("%s\\%s.PNG"), Folder, _T("ColorImage"));
		ImageAPI.SaveImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);	

		RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, nAlign);
		if ( ImageAPI.ExtractRGBRoiImage(ImageW, ImageH, GrayStep, pR, pG, pB, RoiRect4D, RoiStep, pRoiC, false ) == false )
		{
			str = ImageAPI.GetImageApiErrorString();
			JetAPI::ShowMessageBox(str);			
			JetMemory.free_func(pR);
			JetMemory.free_func(pG);
			JetMemory.free_func(pB);
			JetMemory.free_func(DstPtr);
			JetMemory.free_func(ImagePtr);
			return false;
		}
		str.Format(_T("%s\\%s.PNG"), Folder, _T("RGBRoiSubPix_Clr"));
		ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, BitCount, pRoiC, true);	

		if ( ImageAPI.ExtractRGBRoiImage(ImageW, ImageH, GrayStep, pR, pG, pB, RoiRect4D, RoiStep, pRoiC, true ) == false )
		{
			str = ImageAPI.GetImageApiErrorString();
			JetAPI::ShowMessageBox(str);			
			JetMemory.free_func(pR);
			JetMemory.free_func(pG);
			JetMemory.free_func(pB);
			JetMemory.free_func(DstPtr);
			JetMemory.free_func(ImagePtr);
			return false;
		}
		str.Format(_T("%s\\%s.PNG"), Folder, _T("RGBRoiSubPix_Clr-Rev"));
		ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, BitCount, pRoiC, true);

		RoiStep=JetAPI::GetBMPImagePixelsPerLine(RoiW, GrayBitCnt, nAlign);
		if ( ImageAPI.ExtractRGBRoiImage(ImageW, ImageH, GrayStep, pR, pG, pB, RoiRect4D, RoiStep, pRoiR, pRoiG, pRoiB, false ) == false )
		{
			str = ImageAPI.GetImageApiErrorString();
			JetAPI::ShowMessageBox(str);			
			JetMemory.free_func(pR);
			JetMemory.free_func(pG);
			JetMemory.free_func(pB);
			JetMemory.free_func(DstPtr);
			JetMemory.free_func(ImagePtr);
			return false;
		}
		str.Format(_T("%s\\%s.PNG"), Folder, _T("RGBRoiSubPix"));
		ImageAPI.SaveRGBImage(str, RoiW, RoiH, RoiStep, pRoiR, pRoiG, pRoiB, true);	

		if ( ImageAPI.ExtractRGBRoiImage(ImageW, ImageH, GrayStep, pR, pG, pB, RoiRect4D, RoiStep, pRoiR, pRoiG, pRoiB, true ) == false )
		{
			str = ImageAPI.GetImageApiErrorString();
			JetAPI::ShowMessageBox(str);			
			JetMemory.free_func(pR);
			JetMemory.free_func(pG);
			JetMemory.free_func(pB);
			JetMemory.free_func(DstPtr);
			JetMemory.free_func(ImagePtr);
			return false;
		}
		str.Format(_T("%s\\%s.PNG"), Folder, _T("RGBRoiSubPix-Rev"));
		ImageAPI.SaveRGBImage(str, RoiW, RoiH, RoiStep, pRoiR, pRoiG, pRoiB, true);	

		JetMemory.free_func(pR);
		JetMemory.free_func(pG);
		JetMemory.free_func(pB);
		JetMemory.free_func(pRoiR);
		JetMemory.free_func(pRoiG);
		JetMemory.free_func(pRoiB);
		JetMemory.free_func(pRoiC);		
	}	
	if ( ImageAPI.ScaleImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, 0.5, RoiW, RoiH, RoiStep, DstPtr)==false )
	//if ( ImageAPI.FastScaleImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, 2, RoiW, RoiH, RoiStep, DstPtr)==false )
	{
		JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());
		return false;
	}
	str.Format(_T("%s\\%s.PNG"), Folder, _T("ScaleImage"));
	ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, BitCount, DstPtr, true);	

	TicCnt2 = ::GetTickCount();
	TicCntD = TicCnt2-TicCnt1;
	str.Format(_T("Time Spent:%d ms"), TicCntD);
	JetAPI::ShowMessageBox(str);

	JetMemory.free_func(DstPtr);
	JetMemory.free_func(ImagePtr);
	return true;
}

bool CDebugFormView::TestFlipImage()
{
	const char fnName[]="CDebugFormView::TestFlipImage";
#ifndef OPENCV_DISABLE
	
	int End=0;
	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	
	
	IMAGE_PTR  DstPtr=NULL;
	IMAGE_SIZE DstW=0;
	IMAGE_SIZE DstH=0;
	IMAGE_SIZE DstStep=0;

	CString str;
	CString ErrorString;	
	char DataType[32]="";
	char filename[128]="";
	
	End=0;
	try
	{	
		::strcpy(filename, "D:\\Frame_00040.png");
		str = filename;
		if ( ImageAPI.LoadImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false )
		{	return false; }		
		
		DstW = ImageW;
		DstH = ImageH;
		DstStep = JetAPI::GetBMPImagePixelsPerLine(DstW, BitCount, 4);
		const size_t DstBufferSize=ImageAPI.CalcBufferSize(DstStep, DstH);
		if ( JetMemory.alloc_func(DstBufferSize, DstPtr, fnName, "DstPtr") == false )
		{
			JetMemory.free_func(ImagePtr);
			return false;
		}
		
		double angle = 180;
		DWORD   TickCnt1=0;
		DWORD   TickCnt2=0;
		DWORD   TickCntD1=0;
		DWORD   TickCntD2=0;		
		cv::Mat MatDst = ImageAPI.CreateMat(DstW, DstH, DstStep, BitCount, DstPtr);
		cv::Mat MatSrc = ImageAPI.CreateMat(ImageW, ImageH, ImageStep, BitCount, ImagePtr);
		void   *SrcPtr=MatSrc.data;
		void   *TmpPtr=MatDst.data;
		const int flipCode_Y=0;
		const int flipCode_X=1;
		const int flipCode_XY=-1;
		int flipCode=1;
		::memset(DstPtr, 0x00, sizeof(IMAGE_DATA)*DstBufferSize);
		TickCnt1 = ::GetTickCount();
		cv::flip(MatSrc, MatDst, flipCode);
		TickCnt2 = ::GetTickCount();
		TickCntD1 = TickCnt2-TickCnt1;
		void   *SrcPtr2=MatSrc.data;
		void   *TmpPtr2=MatDst.data;

		::strcpy(filename, "D:\\girl_color-flipSrc.png");
		cv::imwrite(filename, MatSrc);
		::strcpy(filename, "D:\\girl_color-flip.png");
		cv::imwrite(filename, MatDst);
		
		::memset(DstPtr, 0x00, sizeof(IMAGE_DATA)*DstBufferSize);
		TickCnt1 = ::GetTickCount();
		//ImageAPI.RotateImage(angle, ImageW, ImageH, ImageStep, BitCount, ImagePtr, DstW, DstH, DstStep, DstPtr);
		ImageAPI.FlipImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, FLIP_Y_AXIS, DstPtr);
		TickCnt2 = ::GetTickCount();
		TickCntD2 = TickCnt2-TickCnt1;

		::strcpy(filename, "D:\\girl_color-flip2.png");
		//cv::imwrite(filename, MatDst);
		str = filename;
		ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);
		
		str.Format(_T("ImageAPI Time=%d ms, OpenCV Time=%d ms"), TickCntD2, TickCntD1);
		JetAPI::ShowMessageBox(str);

		JetMemory.free_func(DstPtr);
		JetMemory.free_func(ImagePtr);

	}
	catch ( cv::Exception& e )
	{	
		const char* msg_e = e.what();  	
		ErrorString = msg_e;		
		JetMemory.free_func(DstPtr);
		JetMemory.free_func(ImagePtr);
		return false;
	}
	
	 End = 3;	 
#endif//OPENCV_DISABLE	
	return true;
}

bool CDebugFormView::TestTransposeImage()
{
	const char fnName[]="CDebugFormView::TestTransposeImage";
#ifndef OPENCV_DISABLE
	
	int End=0;
	IMAGE_PTR  ImagePtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	
	
	IMAGE_PTR  DstPtr=NULL;
	IMAGE_SIZE DstW=0;
	IMAGE_SIZE DstH=0;
	IMAGE_SIZE DstStep=0;

	CString str;
	CString ErrorString;	
	char DataType[32]="";
	char filename[128]="";
	
	End=0;
	try
	{	
		::strcpy(filename, "D:\\Frame_00040.png");
		//::strcpy(filename, "D:\\girl_color-255.png");
		str = filename;
		if ( ImageAPI.LoadImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false )
		{	return false; }		
		
		DstW = ImageH;
		DstH = ImageW;
		DstStep = JetAPI::GetBMPImagePixelsPerLine(DstW, BitCount, 4);
		const size_t DstBufferSize=ImageAPI.CalcBufferSize(DstStep, DstH);
		if ( JetMemory.alloc_func(DstBufferSize, DstPtr, fnName, "DstPtr") == false )
		{
			JetMemory.free_func(ImagePtr);
			return false;
		}
		
		double angle = 270;
		DWORD   TickCnt1=0;
		DWORD   TickCnt2=0;
		DWORD   TickCntD1=0;
		DWORD   TickCntD2=0;		
		cv::Mat MatDst = ImageAPI.CreateMat(DstW, DstH, DstStep, BitCount, DstPtr);
		cv::Mat MatSrc = ImageAPI.CreateMat(ImageW, ImageH, ImageStep, BitCount, ImagePtr);
		void   *SrcPtr=MatSrc.data;
		void   *TmpPtr=MatDst.data;
		const int flipCode_Y=0;
		const int flipCode_X=1;
		const int flipCode_XY=-1;
		int flipCode=1;
		::memset(DstPtr, 0x00, sizeof(IMAGE_DATA)*DstBufferSize);
		TickCnt1 = ::GetTickCount();
		cv::transpose(MatSrc, MatDst);
		cv::flip(MatDst, MatDst, flipCode);
		TickCnt2 = ::GetTickCount();
		TickCntD1 = TickCnt2-TickCnt1;
		void   *SrcPtr2=MatSrc.data;
		void   *TmpPtr2=MatDst.data;

		::strcpy(filename, "D:\\girl_color-transposeSrc.png");
		cv::imwrite(filename, MatSrc);
		::strcpy(filename, "D:\\girl_color-transpose.png");
		cv::imwrite(filename, MatDst);
		
		::memset(DstPtr, 0x00, sizeof(IMAGE_DATA)*DstBufferSize);
		TickCnt1 = ::GetTickCount();
		ImageAPI.RotateImage(angle, ImageW, ImageH, ImageStep, BitCount, ImagePtr, DstW, DstH, DstStep, DstPtr);
		TickCnt2 = ::GetTickCount();
		TickCntD2 = TickCnt2-TickCnt1;

		::strcpy(filename, "D:\\girl_color-transpose2.png");
		//cv::imwrite(filename, MatDst);
		str = filename;
		ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);
		
		str.Format(_T("ImageAPI Time=%d ms, OpenCV Time=%d ms"), TickCntD2, TickCntD1);
		JetAPI::ShowMessageBox(str);

		JetMemory.free_func(DstPtr);
		JetMemory.free_func(ImagePtr);

	}
	catch ( cv::Exception& e )
	{	
		const char* msg_e = e.what();  	
		ErrorString = msg_e;		
		JetMemory.free_func(DstPtr);
		JetMemory.free_func(ImagePtr);
		return false;
	}
	
	 End = 3;	 
#endif//OPENCV_DISABLE	
	return true;
}

bool CDebugFormView::TestSmartPointer()
{
	int   i=0;
	const int len=1024;
	std::shared_ptr<unsigned char> Ptr1(new unsigned char[len], std::default_delete<unsigned char[]>());
	TestSmartPointer2(Ptr1);
	
	unsigned char Bit=0;
	unsigned char *Raw_Ptr=Ptr1.get();
	for ( i=0; i<len; i++ )
	{
		Bit = Raw_Ptr[i];
		if ( 0x00 != Bit )
		{	i = i; }
	}
	return true;
}

bool CDebugFormView::TestSmartPointer2(std::shared_ptr<unsigned char> Ptr)
{	
	if ( Ptr.get() == NULL ) { return false;}
	*Ptr = 3;

	//std::shared_ptr<int[]> Up(new int[8]);
	//Ptr[0] = 1;
	//Ptr.swap
	::memset(Ptr.get(), 0x00, sizeof(unsigned char)*1024);
	return true;
}

bool CDebugFormView::TestTimeStruct()
{
	time_t Time;
	tm *newtime=NULL;
	char buff[32]="";
	char buff2[32]="";

	Time = 0;
	//localtime(&Time);
	newtime = _localtime64(&Time); 
	asctime_s(buff2, sizeof(buff2), newtime );	

	_time64(&Time);
	newtime = _localtime64(&Time); 
	asctime_s(buff, sizeof(buff), newtime );

	FILE *pfile=NULL;
	pfile = ::_tfopen(_T("D:\\TimeTest.TXT"), _T("w+"));
	if ( NULL != pfile )
	{
		Time = 0xFFFFFFFF;
		::_ftprintf(pfile, _T("%I64d"), Time);
		::fclose(pfile);	pfile = NULL;
	}

	pfile = ::_tfopen(_T("D:\\TimeTest.TXT"), _T("r"));
	if ( NULL != pfile )
	{
		Time = 0;
		::_ftscanf(pfile, _T("%I64d"), &Time);		
		::fclose(pfile);	pfile = NULL;
	}
	
	FILETIME ft, ft2;	
	time_t Time2, Time3;
	SYSTEMTIME st,st2;
	_time64(&Time);
	//Time = 0;
	JetAPI::TimetToFileTime(Time, ft);
	JetAPI::FileTimeToTimet(ft, Time3);	
	FileTimeToSystemTime(&ft,&st);

	newtime = _localtime64(&Time); 
	JetAPI::TmToSystemTime(*newtime, st2);	//注意Tm取得可能是LocalTime這樣轉成世界時間會有時間差距
	SystemTimeToFileTime(&st2,&ft2);
	FileTimeToSystemTime(&ft2,&st2);
	//FileTimeToLocalFileTime
	JetAPI::FileTimeToTimet(ft2, Time2);	
	

	newtime = _localtime64(&Time); 
	Time += 1;
	newtime = _localtime64(&Time); 
	return true;
}

bool CDebugFormView::TestProjectSaveServerLibrary()
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return  false; }

	CString  ServerFolder;  
	CString  ServerFolderProject;  
	CString  ServerFolderLibrary;  
	CString  TempFolder = AOIDataCollect.GetAOITempDirectory();	
	ServerFolder.Format(_T("%s\\%s"), TempFolder, _T("Server"));
	if ( JetAPI::CreateFolder(ServerFolder) == false ) 
	{	return false; }
	ServerFolderProject.Format(_T("%s\\%s"), ServerFolder, _T("Project"));
	if ( JetAPI::CreateFolder(ServerFolderProject) == false ) 
	{	return false; }
	ServerFolderLibrary.Format(_T("%s\\%s"), ServerFolder, _T("Library"));
	if ( JetAPI::CreateFolder(ServerFolderLibrary) == false ) 
	{	return false; }

	CString       str;
	double        fnTime1=0;
	double        fnTime2=0;
	LARGE_INTEGER fnEnd;
	LARGE_INTEGER fnStart;
	const bool    bTestProject=true;
	const bool    bTestLibrary=false;

	if ( true == bTestProject )
	{
		JetAPI::SetFuncTimeStart(fnStart);
		if ( ProjectPtr->SaveProjectFileToServerProject(ServerFolderProject, ServerFolderLibrary) == false ) 
		{	
			str = ProjectPtr->GetErrorString();
			JetAPI::ShowMessageBox(str);
			return false;	
		}
		JetAPI::SetFuncTimeEnd(fnEnd);
		fnTime1 = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);

		JetAPI::SetFuncTimeStart(fnStart);
		if ( ProjectPtr->LoadProjectFileFromServerProject(ServerFolderProject, ServerFolderLibrary) == false )
		{
			str = ProjectPtr->GetErrorString();
			JetAPI::ShowMessageBox(str);
			return false; 
		}
		JetAPI::SetFuncTimeEnd(fnEnd);
		fnTime2 = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);

		str.Format(_T("Save Server Library Time=%.3f ms\nLoad Server Project Time=%.3f ms"), fnTime1, fnTime2);
		JetAPI::ShowMessageBox(str);
	}

	if ( true == bTestLibrary )
	{
		const bool bLoadAll = true;
		const bool bIncNewModel = true;	
		JetAPI::SetFuncTimeStart(fnStart);
		if ( ProjectPtr->SaveProjectLibryToServerProjectLibrary(ServerFolderLibrary) == false ) 
		{
			str = ProjectPtr->GetErrorString();
			JetAPI::ShowMessageBox(str);
			return false;	
		}
		JetAPI::SetFuncTimeEnd(fnEnd);
		fnTime1 = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);

		JetAPI::SetFuncTimeStart(fnStart);
		if ( ProjectPtr->LoadProjectLibryFromServerLibrary(ServerFolderLibrary, bIncNewModel, bLoadAll) == false )
		{	
			str = ProjectPtr->GetErrorString();
			JetAPI::ShowMessageBox(str);
			return false; 
		}
		JetAPI::SetFuncTimeEnd(fnEnd);
		fnTime2 = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);

		str.Format(_T("Save Server Library Time=%.3f ms\nLoad Server Library Time=%.3f ms"), fnTime1, fnTime2);
		JetAPI::ShowMessageBox(str);
	}
	return true;
}

bool CDebugFormView::TestRemoteParamWnd()
{
	CRemoteParamWnd Wnd;
	Wnd.DoModal();
	return true;
}

bool CDebugFormView::TestSoftwareNoResponse()
{
	int       i=0;
	CString   str;
	double Time = 0.0;
	LARGE_INTEGER fnEnd;
	LARGE_INTEGER fnStart;
	HWND  hWnd = GetSafeHwnd();
	const int Count = 200;
	const DWORD SleepTime = 50;
	const bool  bRemoveMsg = true;	
	HANDLE hEventTest = NULL;//::CreateEvent(NULL, TRUE, FALSE, NULL);
	JetAPI::SetFuncTimeStart(fnStart);
	for ( i=0; i<Count; i++ )
	{
		if ( SleepTime > 0 )
		{
			if ( NULL != hEventTest )
			{	::WaitForSingleObject(hEventTest, SleepTime); }
			else
			{	::Sleep(SleepTime);  }
		}
		if ( true == bRemoveMsg )
		{	
			JetAPI::RemoveWndBusy(NULL);
			//JetAPI::RemoveMessage(hWnd, NULL , NULL );	
			//JetAPI::RemoveMessage(hWnd, WM_KEYFIRST , WM_KEYLAST );			
			//JetAPI::RemoveMessage(NULL, WM_MOUSEFIRST , WM_MOUSELAST);	
		}
	}
	JetAPI::SetFuncTimeEnd(fnEnd);
	Time = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);

	if ( NULL != hEventTest  )
	{	
		::CloseHandle(hEventTest); 
		hEventTest = NULL;
	}

	if ( false == bRemoveMsg )
	{	str.Format(_T("Finish-%.3f ms"), Time); }
	else
	{	str.Format(_T("Finish-Remove-MSG-%.3f ms"), Time); }
	JetAPI::ShowMessageBox(str);
	return true;
}

bool CDebugFormView::TestDiskDrive()
{
	bool bIs = true;
	CString Folder=_T("F:");
	bIs = JetAPI::IsDiskDrive(Folder);
	bIs = bIs;

	return true;
}

bool CDebugFormView::TestSSEInstructionSet()//SSE 指令集
{//#include <immintrin.h>
	__m128 vector1 = _mm_set_ps(4.0, 3.0, 2.0, 1.0); // high element first, opposite of C array order.  Use _mm_setr_ps if you want "little endian" element order in the source.
    __m128 vector2 = _mm_set_ps(7.0, 8.0, 9.0, 0.0);

    __m128 sum = _mm_add_ps(vector1, vector2); // result = vector1 + vector 2

    vector1 = _mm_shuffle_ps(vector1, vector1, _MM_SHUFFLE(0,1,2,3));
    // vector1 is now (1, 2, 3, 4) (above shuffle reversed it)
    return 0;
	return true;
}

bool CDebugFormView::TestPhaseGamma()//測試相位Gamma
{
	CString str;
	CString filename;		
	CString Folder=_T("R:");
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	IMAGE_PTR  ImagePtr=NULL;	
	filename = _T("R:\\Cast01Pattern4S2P_B1.PNG");
	if ( ImageAPI.LoadImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false )
	{	return false; }

	RECT  Roi;
	int   Gray=0;
	int   Count=0;	
	FILE  *pfile=NULL;
	int   idx=0, idx2=0;	
	int   i=0, j=0, k=0;
	double Sum=0, Ave=0;
	double Min=0, Max=0;
	double Range=0, Step=0;
	const int BandSizeH = 200;
	const int nImageW = (int)(ImageW);
	const int nImageH = (int)(ImageH);
	const int ImageCPX = (int)(nImageW/2);
	const int ImageCPY = (int)(nImageH/2);
	std::vector<unsigned char> Profile;

	Roi.left   = 0;
	Roi.right  = nImageW;
	Roi.top    = ImageCPY-(BandSizeH/2);
	Roi.bottom = ImageCPY+(BandSizeH/2);
	
	//計算投影曲線
	Max = 0;
	Min = 1024;
	for ( i=Roi.left; i<Roi.right; i++ )
	{
		Count = 0;
		Sum = Ave = 0.0;
		for ( j=Roi.top; j<Roi.bottom; j++ )
		{
			idx = (j*ImageStep)+(i);
			Gray = ImagePtr[idx];
			Sum += Gray;
			Count += 1;
		}
		if ( Count > 0 ) 
		{	Ave = Sum/Count; }
		else
		{	Ave = 0.0; }
		if ( Min > Ave ) 
		{	Min = Ave; }
		if ( Max < Ave )
		{	Max = Ave; }
		Profile.push_back((unsigned char)(Ave));
	}
	const size_t ProfileSize=Profile.size();

	str.Format(_T("%s\\%s"), Folder, _T("Profile.TXT"));	
	pfile = ::_tfopen(str, _T("w+"));
	if ( NULL != pfile )
	{
		for ( i=0; i<ProfileSize; i++ )
		{	::_ftprintf(pfile, _T("%d, %d\n"), i, Profile[i]);	}
		::fclose(pfile); pfile=NULL;
	}

	//尋找黑點位置
	Range = Max-Min;
	std::vector<int> DarkList;
	const int ThreadLow = Min+(Range*0.05);
	for ( i=0; i<ProfileSize; i++ )
	{
		Gray = Profile[i];
		if ( Gray > ThreadLow ) { continue; }
		DarkList.push_back(i);
	}
	const size_t DarkCount=DarkList.size();
	str.Format(_T("%s\\%s"), Folder, _T("DarkList.TXT"));	
	pfile = ::_tfopen(str, _T("w+"));
	if ( NULL != pfile )
	{
		for ( i=0; i<DarkCount; i++ )
		{	::_ftprintf(pfile, _T("%d, %d\n"), i, DarkList[i]);	}
		::fclose(pfile); pfile=NULL;
	}
	if ( 0 == DarkCount )
	{
		JetMemory.free_func(ImagePtr);
		return true; 
	}

	//尋找分割位置
	int First=0, EndPos=0;	
	const int IdxGap = 40;	
	std::vector<int> SegmentList;
	First = EndPos = -1;
	for ( i=0; i<DarkCount-1; i++ )
	{
		idx = DarkList[i];
		idx2 = DarkList[i+1];
		if ( -1 == First )
		{	
			First = EndPos = idx;	
			continue;
		}
		if ( (idx2-idx) < IdxGap ) { continue; }
		EndPos = idx;		
		SegmentList.push_back((First+EndPos)/2);
		First = idx2;
	}
	EndPos = idx2;
	SegmentList.push_back((First+EndPos)/2);
	const size_t SegmentCount=SegmentList.size();
	str.Format(_T("%s\\%s"), Folder, _T("SegmentList.TXT"));	
	pfile = ::_tfopen(str, _T("w+"));
	if ( NULL != pfile )
	{
		for ( i=0; i<SegmentCount; i++ )
		{	::_ftprintf(pfile, _T("%d, %d\n"), i, SegmentList[i]);	}
		::fclose(pfile); pfile=NULL;
	}
	
	double A=0;//振幅
	double V=0;//中心位置
	double Gamma=0.0;	
	double Error=0.0;
	double Theata=0.0;
	double Theory=0.0;
	double GammaGray=0.0;
	double MinError=0.0;
	double SumError=0.0;
	double MinErrorGamma=0.0;	
	double MinGray=0, MaxGray=0;	
	const double fPI=3.14159;
	std::vector<double> GrayList;
	std::vector<double> SinePtList;
	std::vector<double> GammaList;
	const size_t SineCount=SegmentCount-1;
	for ( i=0; i<SineCount; i++ )
	{
		MinGray=-1;
		MaxGray=-1;
		First  = SegmentList[i];
		EndPos = SegmentList[i+1];
		Range = EndPos-First;
		Step = fPI*2/Range;
		for ( j=First; j<EndPos; j++ )
		{
			Gray = Profile[j];
			if ( -1 == MaxGray ) 
			{	
				MaxGray = MinGray = Gray; 
				continue;
			}
			if ( MinGray > Gray ) { MinGray = Gray; }
			if ( MaxGray < Gray ) { MaxGray = Gray; }			
		}		
		GrayList.clear();
		SinePtList.clear();
		A = (MaxGray-MinGray)/2;
		V = (MaxGray+MinGray)/2;		
		for ( j=First; j<EndPos; j++ )
		{
			if ( j == (First+EndPos)/2 )
			{	j = j; }
			Gray = Profile[j];

			Theata = ((j-First)*Step)-(fPI*0.5);
			Theory = sin(Theata);
			Theory = Theory*A;
			Theory = Theory+V;
			Error = Gray-Theory;
			Error = Error;
			GrayList.push_back(Gray);
			SinePtList.push_back(Theory);
		}

		//計算Gamma
		MinError = -1;
		MinErrorGamma = 1.0;
		const size_t GrayCount=GrayList.size();
		for ( k=0; k<41; k++ )
		{
			SumError = 0.0;
			Gamma = 0.80+(k*0.01);
			for ( j=0; j<GrayCount; j++ )
			{
				Theory = SinePtList[j];
				GammaGray = GrayList[j];
				GammaGray = GammaGray/255.0;
				GammaGray = ::pow(GammaGray, Gamma);
				GammaGray = GammaGray*255.0;
				Error = GammaGray-Theory;
				if ( Error < 0 ) { Error = -Error; }
				//if ( SumError < Error ) { SumError = Error; }
				SumError += Error;
			}
			if ( -1==MinError || MinError>SumError ) 
			{	
				MinError = SumError; 
				MinErrorGamma = Gamma;
			}
		}
		GammaList.push_back(MinErrorGamma);
	}
	
	Count = 0;
	Gamma = 0;
	const size_t GammaCount = GammaList.size();
	for ( i=0; i<GammaCount; i++ )
	{	Gamma += GammaList[i];	}
	if ( GammaCount > 0 ) 
	{	Gamma = Gamma/GammaCount;  }
	JetMemory.free_func(ImagePtr);

	str.Format(_T("Sine Count:%d, Gamma=%.4f"), GammaCount, Gamma);
	JetAPI::ShowMessageBox(str);
	return true;
}

bool CDebugFormView::TestPhaseGamma2()//測試相位Gamma
{
	CString str;
	CString filename;		
	CString Folder=_T("R:");
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	IMAGE_PTR  ImagePtr=NULL;	
	filename = _T("R:\\Cast04Pattern4S2P_B4.PNG");
	if ( ImageAPI.LoadImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false )
	{	return false; }

	if ( 8 != BitCount )
	{
		JetMemory.free_func(ImagePtr);
		return true;
	}

	bool  bVer = false;
	int   nBand = 100;
	float LowRatio = 0.05;
	int   PeriodGap = 50;
	int   SineCount = 0;
	double Gamma = 1.00; 
	if ( ImageAPI.CalcGrayImageGammaBySine(ImageW, ImageH, ImageStep, ImagePtr, bVer, nBand, LowRatio, PeriodGap, SineCount, Gamma) == false )
	{
		JetMemory.free_func(ImagePtr);
		return false;
	}
	JetMemory.free_func(ImagePtr);
	str.Format(_T("Sine Count:%d, Gamma=%.4f"), SineCount, Gamma);
	JetAPI::ShowMessageBox(str);
	return true;
}

bool CDebugFormView::TestNonLocalMean()//測試Non-Local Mean
{
#ifndef OPENCV_PHOTO_DISABLE
	CString str;
	CString filename;	
	CString filename2;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	IMAGE_PTR  DstPtr=NULL;	
	IMAGE_PTR  ImagePtr=NULL;	
	filename = _T("F:\\girl_gray2.PNG");
	//filename = _T("F:\\2018-Image.PNG");	
	if ( ImageAPI.LoadImage(filename, ImageW, ImageH, ImageStep, BitCount, ImagePtr, 4, true) == false )
	{	return false; }

	if ( 8 != BitCount )
	{
		JetMemory.free_func(ImagePtr);
		return true;
	}

	float h = 5;
	int   szPatch = 7;
	int   szSearch = 21;
	szPatch = 9;
	szSearch = 21;
	if ( ImageAPI.NonLocalMeanGrayImage(ImageW, ImageH, ImageStep, ImagePtr, h, szPatch, szSearch, DstPtr) == false )	
	{
		JetMemory.free_func(DstPtr);
		JetMemory.free_func(ImagePtr);
		return false;
	}

	filename2 = _T("F:\\2018-Image-Dst.PNG");
	ImageAPI.SaveImage(filename2, ImageW, ImageH, ImageStep, BitCount, DstPtr, true);

	JetMemory.free_func(DstPtr);
	JetMemory.free_func(ImagePtr);	
#endif//OPENCV_PHOTO_DISABLE
	return true;
}

void CDebugFormView::TestArray(cv::InputArrayOfArrays objectPoints, cv::InputArrayOfArrays imagePoints)
{
	int nimages = (int)objectPoints.total();
	int nimages2 = (int)imagePoints.total();
    int i, j = 0;
	int ni = 0, total = 0;
	int ni2 = 0, total2 = 0;
    //assert(nimages > 0 && nimages == (int)imagePoints1.total() && (!imgPtMat2 || nimages == (int)imagePoints2.total()));

    for( i = 0; i < nimages; i++ )
    {
        ni = objectPoints.getMat(i).checkVector(3, CV_32F);
		ni2 = imagePoints.getMat(i).checkVector(2, CV_32F);

        assert( ni >= 0 );
		assert( ni2 >= 0 );

        total += ni;
		total2 += ni2;
    }

	return;
}

bool CDebugFormView::TestCorrectTShape()//測試校正-T形
{
	CString strErr;
	try
	{	
		cv::Mat srcImg;
		cv::Mat dstImg;
		cv::Mat warp_mat;
		cv::Point2f srcTri[4];
		cv::Point2f dstTri[4];
		std::string filename;

		filename = "D:\\CameraCalibration\\TShape.bmp";
		srcImg = cv::imread(filename, cv::IMREAD_ANYCOLOR);

		const int width=srcImg.cols;
		const int height=srcImg.rows;

		srcTri[0].x = 0;
		srcTri[0].y = 0;
		srcTri[1].x = width-1;
		srcTri[1].y = 0;
		srcTri[2].x = 0;
		srcTri[2].y = height-1;
		srcTri[3].x = width-1;
		srcTri[3].y = height-1;

		dstTri[0].x = width*0.265;
		dstTri[0].y = height*0;
		dstTri[1].x = width*0.735;
		dstTri[1].y = height*0;
		dstTri[2].x = width*0.055;
		dstTri[2].y = height-1;
		dstTri[3].x = width*0.945;
		dstTri[3].y = height-1;

		//warp_mat = cv::getPerspectiveTransform(srcTri, dstTri);
		warp_mat = cv::getPerspectiveTransform(dstTri, srcTri);
		cv::warpPerspective(srcImg, dstImg, warp_mat, cv::Size(width, height));

		filename = "R:\\TShape.bmp";
		cv::imwrite(filename, dstImg);
	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		strErr = msg_e;
		return false;
	}
	return true;
}

bool CDebugFormView::TestCameraCalibration()//測試相機校正
{	//需要使用OPEN_CV_2_4_13_06_V14版本, 否則有蟲
#ifndef OPENCV_CALIB_3D_DISABLE
	CString strErr;
	try
	{		
		int nSuccess=0;
		//int nBoards = 1;
		int nCornersHor=7;//7
		int nCornersVer=6;//6
		int nCorners=nCornersHor*nCornersVer;
		int nImages = 14;
		char strBuf[256]="";
		char strBuf2[256]="";
		cv::Size board_sz = cv::Size(nCornersHor, nCornersVer);

		bool found=0;
		cv::Mat image;				
		//cv::Mat color_image;
		std::string filename;
		std::string filename2;		
		cv::Point3f obj_point;
		std::vector<cv::Point3f> obj;
		std::vector<cv::Point2f> corners;
		std::vector<cv::Point2f> corners2;
		std::vector<std::vector<cv::Point2f>> image_points;
		std::vector<std::vector<cv::Point3f>> object_points;

		for ( int j=0; j<nCorners; j++ )
		{	
			obj_point = cv::Point3f(j/nCornersHor, j%nCornersHor, 0.0f);
			obj.push_back(obj_point);
		}
		
		corners.resize(nCorners*2);//先建立記憶體空間
		corners.clear();//引數歸零
		//image_points.clear();
		//object_points.clear();
		for ( int i=0; i<nImages; i++ )
		{
			if ( i == 9 ) { continue; }
			::sprintf(strBuf, "D:\\CameraCalibration\\left%02d.jpg", i+1);
			::sprintf(strBuf2, "R:\\left%02d.jpg", i+1);

			filename = strBuf;
			filename2 = strBuf2;			
			
			image = cv::imread(filename, cv::IMREAD_ANYCOLOR);
			//cv::cvtColor(image, gray_image, CV_BGR2GRAY);
			//cv::cvtColor(image, color_image, CV_GRAY2BGR);			
			found = cv::findChessboardCorners(image, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);//記得corners的陣列記憶體要先開起來
			corners2 = corners;
			//continue;
			if ( true == found )
			{	
				cv::cornerSubPix(image, corners2, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));	
				cv::drawChessboardCorners(image, board_sz, corners2, found);
			}			

			cv::imwrite(filename2, image);
			//cv::imshow("win1", image);
			//cv::imshow("win2", gray_image);

			if ( true == found )
			{	
				image_points.push_back(corners);
				object_points.push_back(obj);
				nSuccess ++;				
			}
			//break;
		}
		//return true;
		if ( 0 == nSuccess ) 
		{	return false; }	

		double  Int_Buf[9];
		double  Dis_Buf[9];
		cv::Mat distCoeffs;		
		cv::Mat intrinsic = cv::Mat(3, 3, CV_64FC1);//使用double-64bit
		std::vector<cv::Mat> rvecs;//Rotation-Vector
		std::vector<cv::Mat> tvecs;//Translation-Vector
		::memset(Int_Buf, 0x00, sizeof(Int_Buf));
		::memset(Dis_Buf, 0x00, sizeof(Dis_Buf));

		intrinsic.ptr<double>(0)[0] = 1;
		intrinsic.ptr<double>(1)[1] = 1;
		cv::calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);
		Int_Buf[0] = intrinsic.ptr<double>(0)[0];	Int_Buf[1] = intrinsic.ptr<double>(0)[1];	Int_Buf[2] = intrinsic.ptr<double>(0)[2];
		Int_Buf[3] = intrinsic.ptr<double>(1)[0];	Int_Buf[4] = intrinsic.ptr<double>(1)[1];	Int_Buf[5] = intrinsic.ptr<double>(1)[2];
		Int_Buf[6] = intrinsic.ptr<double>(2)[0];	Int_Buf[7] = intrinsic.ptr<double>(2)[1];	Int_Buf[8] = intrinsic.ptr<double>(2)[2];

		double fx = intrinsic.ptr<double>(0)[0];
		double fy = intrinsic.ptr<double>(1)[1];
		double cx = intrinsic.ptr<double>(0)[2];
		double cy = intrinsic.ptr<double>(1)[2];
		double zz = intrinsic.ptr<double>(2)[2];

		int kk=0;
		for ( int i=0; i<distCoeffs.rows; i++ )
		{
			for ( int j=0; j<distCoeffs.cols; j++ )
			{
				Dis_Buf[kk++]=distCoeffs.ptr<double>(i)[j];
			}
		}
		cv::Mat imageUndistorted;
		for ( int i=0; i<nImages; i++ )
		{
			if ( i == 9 ) { continue; }
			::sprintf(strBuf, "D:\\CameraCalibration\\left%02d.jpg", i+1);
			::sprintf(strBuf2, "R:\\Cali%02d.jpg", i+1);
			filename = strBuf;
			filename2 = strBuf2;

			image = cv::imread(filename, cv::IMREAD_ANYCOLOR);
			cv::undistort(image, imageUndistorted, intrinsic, distCoeffs);
			cv::imwrite(filename2, imageUndistorted);
		}

	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		strErr = msg_e;
		return false;
	}
#endif//OPENCV_CALIB_3D_DISABLE
	return true;
}

bool CDebugFormView::TestJetCameraCalibration()//測試相機校正
{	//需要使用OPEN_CV_2_4_13_06_V14版本, 否則有蟲
#ifndef OPENCV_CALIB_3D_DISABLE
	CString strErr;
	try
	{		
		int nSuccess=0;
		//int nBoards = 1;
		int nCornersHor=17;//18;//7
		int nCornersVer=12;//12;//6
		int nCorners=nCornersHor*nCornersVer;
		int nImages = 19;//19
		char strBuf[256]="";
		char strBuf2[256]="";
		cv::Size board_sz = cv::Size(nCornersHor, nCornersVer);

		bool found=0;
		cv::Mat image;				
		//cv::Mat color_image;
		std::string filename;
		std::string filename2;		
		cv::Point3f obj_point;
		std::vector<cv::Point3f> obj;
		std::vector<cv::Point2f> corners;
		std::vector<cv::Point2f> corners2;
		std::vector<std::vector<cv::Point2f>> image_points;
		std::vector<std::vector<cv::Point3f>> object_points;

		for ( int j=0; j<nCorners; j++ )
		{	
			obj_point = cv::Point3f(j/nCornersHor, j%nCornersHor, 0.0f);
			obj.push_back(obj_point);
		}
		
		corners.resize(nCorners*2);//先建立記憶體空間
		corners.clear();//引數歸零
		//image_points.clear();
		//object_points.clear();
		for ( int i=0; i<nImages; i++ )
		{
			//if ( i == 9 ) { continue; }
			::sprintf(strBuf, "D:\\CameraCalibration\\cal\\grid_%02d.bmp", i);
			::sprintf(strBuf2, "R:\\grid_%02d.bmp", i);

			filename = strBuf;
			filename2 = strBuf2;			
			
			image = cv::imread(filename, cv::IMREAD_ANYCOLOR);
			//cv::cvtColor(image, gray_image, CV_BGR2GRAY);
			//cv::cvtColor(image, color_image, CV_GRAY2BGR);			
			found = cv::findChessboardCorners(image, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);//記得corners的陣列記憶體要先開起來
			corners2 = corners;
			//continue;
			if ( true == found )
			{	
				cv::cornerSubPix(image, corners2, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));	
				cv::drawChessboardCorners(image, board_sz, corners2, found);
			}
			else
			{
				if ( corners2.size() > 0 ) 
				{	cv::drawChessboardCorners(image, board_sz, corners2, found);	}
			}

			cv::imwrite(filename2, image);
			//cv::imshow("win1", image);
			//cv::imshow("win2", gray_image);

			if ( true == found )
			{	
				image_points.push_back(corners);
				object_points.push_back(obj);
				nSuccess ++;				
			}
			//break;
		}
		//return true;
		if ( 0 == nSuccess ) 
		{	return false; }	

		double  Int_Buf[9];
		double  Dis_Buf[9];
		cv::Mat distCoeffs;		
		cv::Mat intrinsic = cv::Mat(3, 3, CV_64FC1);//使用double-64bit
		std::vector<cv::Mat> rvecs;//Rotation-Vector
		std::vector<cv::Mat> tvecs;//Translation-Vector
		::memset(Int_Buf, 0x00, sizeof(Int_Buf));
		::memset(Dis_Buf, 0x00, sizeof(Dis_Buf));

		intrinsic.ptr<double>(0)[0] = 1;
		intrinsic.ptr<double>(1)[1] = 1;
		cv::calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);
		Int_Buf[0] = intrinsic.ptr<double>(0)[0];	Int_Buf[1] = intrinsic.ptr<double>(0)[1];	Int_Buf[2] = intrinsic.ptr<double>(0)[2];
		Int_Buf[3] = intrinsic.ptr<double>(1)[0];	Int_Buf[4] = intrinsic.ptr<double>(1)[1];	Int_Buf[5] = intrinsic.ptr<double>(1)[2];
		Int_Buf[6] = intrinsic.ptr<double>(2)[0];	Int_Buf[7] = intrinsic.ptr<double>(2)[1];	Int_Buf[8] = intrinsic.ptr<double>(2)[2];

		double fx = intrinsic.ptr<double>(0)[0];
		double fy = intrinsic.ptr<double>(1)[1];
		double cx = intrinsic.ptr<double>(0)[2];
		double cy = intrinsic.ptr<double>(1)[2];
		double zz = intrinsic.ptr<double>(2)[2];

		int kk=0;
		for ( int i=0; i<distCoeffs.rows; i++ )
		{
			for ( int j=0; j<distCoeffs.cols; j++ )
			{
				Dis_Buf[kk++]=distCoeffs.ptr<double>(i)[j];
			}
		}
		cv::Mat imageUndistorted;
		for ( int i=0; i<nImages; i++ )
		{
			//if ( i == 9 ) { continue; }
			::sprintf(strBuf, "D:\\CameraCalibration\\cal\\grid_%02d.bmp", i);
			::sprintf(strBuf2, "R:\\Cali%02d.bmp", i);
			filename = strBuf;
			filename2 = strBuf2;

			image = cv::imread(filename, cv::IMREAD_ANYCOLOR);
			cv::undistort(image, imageUndistorted, intrinsic, distCoeffs);
			cv::imwrite(filename2, imageUndistorted);
		}

	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		strErr = msg_e;
		return false;
	}
#endif//OPENCV_CALIB_3D_DISABLE
	return true;
}


bool CDebugFormView::TestStereoCameraCalibration()//測試雙相機校正
{
	//需要使用OPEN_CV_2_4_13_06_V14版本, 否則有蟲
#ifndef OPENCV_CALIB_3D_DISABLE
	CString strErr;
	try
	{		
		int nSuccess=0;
		//int nBoards = 1;
		int nCornersHor=7;//7
		int nCornersVer=6;//6
		int nCorners=nCornersHor*nCornersVer;
		int nImages = 14;
		char strBufL[256]="";
		char strBufR[256]="";
		char strBufL2[256]="";
		char strBufR2[256]="";
		cv::Size board_sz = cv::Size(nCornersHor, nCornersVer);

		bool foundL=0;
		bool foundR=0;
		int  object_pointsBoth_Cnt=0;
		cv::Mat imageL;
		cv::Mat imageR;
		//cv::Mat color_image;
		std::string filenameL;
		std::string filenameR;
		std::string filenameL2;
		std::string filenameR2;
		cv::Point3f obj_point;
		std::vector<cv::Point3f> obj;
		std::vector<cv::Point2f> cornersL;
		std::vector<cv::Point2f> cornersR;
		std::vector<cv::Point2f> cornersL2;
		std::vector<cv::Point2f> cornersR2;
		std::vector<std::vector<cv::Point2f>> image_pointsL;
		std::vector<std::vector<cv::Point2f>> image_pointsR;		
		std::vector<std::vector<cv::Point3f>> object_pointsL;
		std::vector<std::vector<cv::Point3f>> object_pointsR;
		std::vector<std::vector<cv::Point2f>> image_pointsBothL;
		std::vector<std::vector<cv::Point2f>> image_pointsBothR;
		std::vector<std::vector<cv::Point3f>> object_pointsBoth;

		for ( int j=0; j<nCorners; j++ )
		{	
			obj_point = cv::Point3f(j/nCornersHor, j%nCornersHor, 0.0f);
			obj.push_back(obj_point);
		}
		const int objPtCnt=(int)(obj.size());
		cornersL.resize(nCorners*2);//先建立記憶體空間
		cornersR.resize(nCorners*2);//先建立記憶體空間
		cornersL.clear();//引數歸零
		cornersR.clear();//引數歸零
		//image_points.clear();
		//object_points.clear();
		object_pointsBoth_Cnt=0;
		const bool useStereoCamera=true;
		for ( int i=0; i<nImages; i++ )
		{
			if ( i == 9 ) { continue; }
			::sprintf(strBufL, "D:\\CameraCalibration\\left%02d.jpg", i+1);
			::sprintf(strBufR, "D:\\CameraCalibration\\right%02d.jpg", i+1);
			::sprintf(strBufL2, "R:\\left%02d.jpg", i+1);
			::sprintf(strBufR2, "R:\\right%02d.jpg", i+1);

			filenameL = strBufL;
			filenameR = strBufR;
			filenameL2 = strBufL2;						
			filenameR2 = strBufR2;
			
			imageL = cv::imread(filenameL, cv::IMREAD_ANYCOLOR);
			imageR = cv::imread(filenameR, cv::IMREAD_ANYCOLOR);
			//cv::cvtColor(imageL, gray_image, CV_BGR2GRAY);
			//cv::cvtColor(imageL, color_image, CV_GRAY2BGR);			
			foundL = cv::findChessboardCorners(imageL, board_sz, cornersL, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);//記得corners的陣列記憶體要先開起來
			foundR = cv::findChessboardCorners(imageR, board_sz, cornersR, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);//記得corners的陣列記憶體要先開起來
			cornersL2 = cornersL;
			cornersR2 = cornersR;
			//continue;
			if ( true == foundL )
			{	
				cv::cornerSubPix(imageL, cornersL2, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));	
				cv::drawChessboardCorners(imageL, board_sz, cornersL2, foundL);
			}			
			if ( true == foundR )
			{	
				cv::cornerSubPix(imageR, cornersR2, cv::Size(11, 11), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));	
				cv::drawChessboardCorners(imageR, board_sz, cornersR2, foundR);
			}

			cv::imwrite(filenameL2, imageL);
			cv::imwrite(filenameR2, imageR);
			//cv::imshow("win1", image);
			//cv::imshow("win2", gray_image);				
			
			if ( true == foundL )
			{	
				image_pointsL.push_back(cornersL2);				
				object_pointsL.push_back(obj);
			}
			if ( true == foundR )
			{					
				image_pointsR.push_back(cornersR2);
				object_pointsR.push_back(obj);
			}
			
			if ( true==foundL && true==foundR )
			{	
				image_pointsBothL.push_back(cornersL2);
				image_pointsBothR.push_back(cornersR2);
				object_pointsBoth.push_back(obj);
				object_pointsBoth_Cnt += (int)(obj.size());
			}			

			if ( true==foundL || true==foundR )
			{	nSuccess ++; }
			//break;
		}
		//return true;
		if ( 0 == nSuccess ) 
		{	return false; }			
		
		int     kk=0;
		cv::Mat distCoeffsL;
		cv::Mat distCoeffsR;		
		cv::Mat intrinsicL = cv::Mat(3, 3, CV_64FC1);
		cv::Mat intrinsicR = cv::Mat(3, 3, CV_64FC1);
		cv::Mat Rvecs;//Rotation-Vector
		cv::Mat Tvecs;//Translation-Vector
		cv::Mat Evecs;//Essential-Vector
		cv::Mat Fvecs;//Fundamental-Vector
		cv::Mat RotMatL;//Rotation-Matrix
		cv::Mat RotMatR;//Rotation-Matrix
		cv::Mat ProjMatL;//Projection-Matrix
		cv::Mat ProjMatR;//Projection-Matrix
		cv::Mat DepthMat;//disparity-to-depth mapping matrix				
		cv::Mat Point4D;// = cv::Mat(4, object_pointsBoth_Cnt, CV_64FC1);
		cv::Mat ImagePt2dL = cv::Mat(2, object_pointsBoth_Cnt, CV_32FC1);
		cv::Mat ImagePt2dR = cv::Mat(2, object_pointsBoth_Cnt, CV_32FC1);
		std::vector<cv::Mat> rvecsL;//Rotation-Vector
		std::vector<cv::Mat> rvecsR;//Rotation-Vector
		std::vector<cv::Mat> tvecsL;//Translation-Vector
		std::vector<cv::Mat> tvecsR;//Translation-Vector	

		std::vector<double> Int_BufL;
		std::vector<double> Int_BufR;
		std::vector<double> Dis_BufL;
		std::vector<double> Dis_BufR;
		std::vector<double> SteoRot_Buf;
		std::vector<double> SteoTra_Buf;
		std::vector<double> SteoEss_Buf;
		std::vector<double> SteoFnd_Buf;
		std::vector<double> SteoRot_BufL;
		std::vector<double> SteoRot_BufR;
		std::vector<double> SteoProj_BufL;
		std::vector<double> SteoProj_BufR;
		std::vector<double> SteoDepth_Buf;
		std::vector<cv::Point3f> Points3D;
		std::vector<cv::Point3d> SteoPointOut_Buf;		

		for ( int i=0; i<3; i++ )
		{
			for ( int j=0; j<3; j++ )
			{
				intrinsicL.ptr<double>(i)[j] = 0;
				intrinsicR.ptr<double>(i)[j] = 0;
			}
		}


		kk = 0;
		const int ImagePt2dTypeL = ImagePt2dL.type();
		for ( int i=0; i<image_pointsBothL.size(); i++ )
		{	
			for ( int j=0; j<image_pointsBothL[i].size(); j++ )
			{
				if ( kk >= ImagePt2dL.cols )
				{	break; }
				switch ( ImagePt2dTypeL )
				{
				case 5:
					ImagePt2dL.ptr<float>(0)[kk] = image_pointsBothL[i][j].x;
					ImagePt2dL.ptr<float>(1)[kk] = image_pointsBothL[i][j].y;
					break;
				case 6:
					ImagePt2dL.ptr<double>(0)[kk] = image_pointsBothL[i][j].x;
					ImagePt2dL.ptr<double>(1)[kk] = image_pointsBothL[i][j].y;
					break;
				}				
				kk ++;				
			}
		}
		kk = 0;
		const int ImagePt2dTypeR = ImagePt2dR.type();
		for ( int i=0; i<image_pointsBothR.size(); i++ )
		{
			for ( int j=0; j<image_pointsBothR[i].size(); j++ )
			{
				if ( kk >= ImagePt2dR.cols )
				{	break; }
				switch ( ImagePt2dTypeR )
				{
				case 5:
					ImagePt2dR.ptr<float>(0)[kk] = image_pointsBothR[i][j].x;
					ImagePt2dR.ptr<float>(1)[kk] = image_pointsBothR[i][j].y;
					break;
				case 6:
					ImagePt2dR.ptr<double>(0)[kk] = image_pointsBothR[i][j].x;
					ImagePt2dR.ptr<double>(1)[kk] = image_pointsBothR[i][j].y;
					break;				
				}				
				kk ++;				
			}
		}

		intrinsicL.ptr<double>(0)[0] = 1;
		intrinsicL.ptr<double>(1)[1] = 1;
		intrinsicL.ptr<double>(2)[2] = 1.0;
		intrinsicR.ptr<double>(0)[0] = 1;
		intrinsicR.ptr<double>(1)[1] = 1;
		intrinsicR.ptr<double>(2)[2] = 1.0;

		//計算出兩個相機的內部與畸變參數
		cv::calibrateCamera(object_pointsL, image_pointsL, imageL.size(), intrinsicL, distCoeffsL, rvecsL, tvecsL);
		cv::calibrateCamera(object_pointsR, image_pointsR, imageR.size(), intrinsicR, distCoeffsR, rvecsR, tvecsR);		
		if ( true == useStereoCamera )
		{
			//計算雙相機相對座標轉換公式
			//cv::stereoCalibrate(object_pointsL, image_pointsL, image_pointsR, intrinsicL, distCoeffsL, intrinsicR, distCoeffsR, imageL.size(), Rvecs, Tvecs, Evecs, Fvecs);
			cv::stereoCalibrate(object_pointsBoth, image_pointsBothL, image_pointsBothR, intrinsicL, distCoeffsL, intrinsicR, distCoeffsR, imageL.size(), Rvecs, Tvecs, Evecs, Fvecs);

			//計算個別相機的旋轉與投影矩陣
			cv::stereoRectify(intrinsicL, distCoeffsL, intrinsicR, distCoeffsR, imageL.size(), Rvecs, Tvecs, RotMatL, RotMatR, ProjMatL, ProjMatR, DepthMat);

			//計算出空間座標
			//cv::triangulatePoints(ProjMatL, ProjMatR, image_pointsBothL, image_pointsBothR, Point4D);
			cv::triangulatePoints(ProjMatL, ProjMatR, ImagePt2dL, ImagePt2dR, Point4D);
			//將齊次座標轉成非齊次座標系統(4D->3D)//輸入源需要是Col=1, Channel=4的矩陣, 使用Type=cv::Vec4f, 1次4個float
			cv::Mat Point4D_Tran;
			cv::transpose(Point4D, Point4D_Tran);//行列轉置

			cv::Mat Point4D_TranReshape=Point4D_Tran.reshape(4);//將每列4行-通道1調整成每列1行-通道4
			cv::convertPointsFromHomogeneous(Point4D_TranReshape, Points3D);
		}
		
		float TestBuf[64]={0};
		const int TestCnt=8;		
		cv::Vec4f TestBuf2[64]={0};
		cv::Vec4f TestBuf3[64]={0};
		std::vector<cv::Point3f> Test3D;
		std::vector<cv::Point3f> Test3D2;
		cv::Mat Test4D=cv::Mat(4, TestCnt, CV_32FC1);
		cv::Mat Test4D2=cv::Mat(TestCnt, 1, CV_32FC4);
		for ( int i=0; i<TestCnt; i++ )
		{
			float val = (i+1)*100;
			float val1 = (i+1)*10;
			float val2 = (i+1)*0.1;
			cv::Vec4f TestVec4f;
			cv::Vec4d TestVec4d;

			Test4D.ptr<float>(0)[i] = val;
			Test4D.ptr<float>(1)[i] = val+val1;
			Test4D.ptr<float>(2)[i] = val+val1+val1;
			Test4D.ptr<float>(3)[i] = val2;

			TestVec4f[0] = val;
			TestVec4f[1] = val+val1;
			TestVec4f[2] = val+val1+val1;
			TestVec4f[3] = val2;

			TestVec4d[0] = val;
			TestVec4d[1] = val+val1;
			TestVec4d[2] = val+val1+val1;
			TestVec4d[3] = val2;			
			Test4D2.ptr<cv::Vec4f>(i)[0] = TestVec4f;			
		}		

		cv::Mat Test4D3;
		cv::transpose(Test4D, Test4D3);
		cv::Mat Test4DReshape=Test4D3.reshape(4);
		const int Test4DReshapeType = Test4DReshape.type();
		const int Test4DReshapeChannel = Test4DReshape.channels();				
		kk = 0;
		for ( int i=0; i<Test4DReshape.rows; i++ )
		{
			for ( int j=0; j<Test4DReshape.cols; j++ )
			{	
				TestBuf[kk] = (Test4DReshape.ptr<float>(i)[j]);	
				TestBuf2[kk] = (Test4DReshape.ptr<cv::Vec4f>(i)[j]);	
				kk++;
			}
		}		
		kk = 0;
		for ( int i=0; i<Test4D2.rows; i++ )
		{
			for ( int j=0; j<Test4D2.cols; j++ )
			{	
				TestBuf3[kk] = (Test4D2.ptr<cv::Vec4f>(i)[j]);	
				kk++;
			}
		}
		cv::convertPointsFromHomogeneous(Test4DReshape, Test3D);		
		cv::convertPointsFromHomogeneous(Test4D2, Test3D2);		
		//*/
		const int intrinsicType = intrinsicL.type();		
		const int distCoeffsType = distCoeffsL.type();		
		const int RotMatType = RotMatL.type();
		const int ProjMatType = ProjMatL.type();
		const int DepthMatType = DepthMat.type();
		const int Point4DType = Point4D.type();

		Int_BufL.push_back(intrinsicL.ptr<double>(0)[0]);	Int_BufL.push_back(intrinsicL.ptr<double>(0)[1]);	Int_BufL.push_back(intrinsicL.ptr<double>(0)[2]);
		Int_BufL.push_back(intrinsicL.ptr<double>(1)[0]);	Int_BufL.push_back(intrinsicL.ptr<double>(1)[1]);	Int_BufL.push_back(intrinsicL.ptr<double>(1)[2]);
		Int_BufL.push_back(intrinsicL.ptr<double>(2)[0]);	Int_BufL.push_back(intrinsicL.ptr<double>(2)[1]);	Int_BufL.push_back(intrinsicL.ptr<double>(2)[2]);

		Int_BufR.push_back(intrinsicR.ptr<double>(0)[0]);	Int_BufR.push_back(intrinsicR.ptr<double>(0)[1]);	Int_BufR.push_back(intrinsicR.ptr<double>(0)[2]);
		Int_BufR.push_back(intrinsicR.ptr<double>(1)[0]);	Int_BufR.push_back(intrinsicR.ptr<double>(1)[1]);	Int_BufR.push_back(intrinsicR.ptr<double>(1)[2]);
		Int_BufR.push_back(intrinsicR.ptr<double>(2)[0]);	Int_BufR.push_back(intrinsicR.ptr<double>(2)[1]);	Int_BufR.push_back(intrinsicR.ptr<double>(2)[2]);

		double fxL = intrinsicL.ptr<double>(0)[0];
		double fyL = intrinsicL.ptr<double>(1)[1];
		double cxL = intrinsicL.ptr<double>(0)[2];
		double cyL = intrinsicL.ptr<double>(1)[2];
		double zzL = intrinsicL.ptr<double>(2)[2];

		double fxR = intrinsicR.ptr<double>(0)[0];
		double fyR = intrinsicR.ptr<double>(1)[1];
		double cxR = intrinsicR.ptr<double>(0)[2];
		double cyR = intrinsicR.ptr<double>(1)[2];
		double zzR = intrinsicR.ptr<double>(2)[2];
		
		for ( int i=0; i<distCoeffsL.rows; i++ )
		{
			for ( int j=0; j<distCoeffsL.cols; j++ )
			{	Dis_BufL.push_back(distCoeffsL.ptr<double>(i)[j]);	}
		}		
		for ( int i=0; i<distCoeffsR.rows; i++ )
		{
			for ( int j=0; j<distCoeffsR.cols; j++ )
			{	Dis_BufR.push_back(distCoeffsR.ptr<double>(i)[j]);	}
		}
		
		for ( int i=0; i<Rvecs.rows; i++ )
		{
			for ( int j=0; j<Rvecs.cols; j++ )
			{	SteoRot_Buf.push_back(Rvecs.ptr<double>(i)[j]);	}
		}

		for ( int i=0; i<Tvecs.rows; i++ )
		{
			for ( int j=0; j<Tvecs.cols; j++ )
			{	SteoTra_Buf.push_back(Tvecs.ptr<double>(i)[j]);	}
		}
		
		for ( int i=0; i<Evecs.rows; i++ )
		{
			for ( int j=0; j<Evecs.cols; j++ )
			{	SteoEss_Buf.push_back(Evecs.ptr<double>(i)[j]);	}
		}

		for ( int i=0; i<Fvecs.rows; i++ )
		{
			for ( int j=0; j<Fvecs.cols; j++ )
			{	SteoFnd_Buf.push_back(Fvecs.ptr<double>(i)[j]);	}
		}
		
		for ( int i=0; i<RotMatL.rows; i++ )
		{
			for ( int j=0; j<RotMatL.cols; j++ )
			{	SteoRot_BufL.push_back(RotMatL.ptr<double>(i)[j]);	}
		}

		for ( int i=0; i<RotMatR.rows; i++ )
		{
			for ( int j=0; j<RotMatR.cols; j++ )
			{	SteoRot_BufR.push_back(RotMatR.ptr<double>(i)[j]);	}
		}

		for ( int i=0; i<ProjMatL.rows; i++ )
		{
			for ( int j=0; j<ProjMatL.cols; j++ )
			{	SteoProj_BufL.push_back(ProjMatL.ptr<double>(i)[j]);	}
		}

		for ( int i=0; i<ProjMatR.rows; i++ )
		{
			for ( int j=0; j<ProjMatR.cols; j++ )
			{	SteoProj_BufR.push_back(ProjMatR.ptr<double>(i)[j]);	}
		}

		for ( int i=0; i<DepthMat.rows; i++ )
		{
			for ( int j=0; j<DepthMat.cols; j++ )
			{	SteoDepth_Buf.push_back(DepthMat.ptr<double>(i)[j]);	}
		}
		

		for ( int j=0; j<Point4D.cols; j++ )
		{	
			double w=1;
			cv::Point3d point3d;
			switch ( Point4DType )
			{
			case 5:
				w = Point4D.ptr<float>(3)[j];
				point3d.x = Point4D.ptr<float>(0)[j];
				point3d.y = Point4D.ptr<float>(1)[j];
				point3d.z = Point4D.ptr<float>(2)[j];
				break;
			case 6:
				w = Point4D.ptr<double>(3)[j];
				point3d.x = Point4D.ptr<double>(0)[j];
				point3d.y = Point4D.ptr<double>(1)[j];
				point3d.z = Point4D.ptr<double>(2)[j];
				break;
			}			
			point3d.x /= w;
			point3d.y /= w;
			point3d.z /= w;			
			SteoPointOut_Buf.push_back(point3d);
		}		



		
		cv::Mat imageUndistortedL;
		cv::Mat imageUndistortedR;
		for ( int i=0; i<nImages; i++ )
		{
			if ( i == 9 ) { continue; }
			::sprintf(strBufL, "D:\\CameraCalibration\\left%02d.jpg", i+1);
			::sprintf(strBufR, "D:\\CameraCalibration\\right%02d.jpg", i+1);
			::sprintf(strBufL2, "R:\\CaliL%02d.jpg", i+1);			
			::sprintf(strBufR2, "R:\\CaliR%02d.jpg", i+1);
			filenameL = strBufL;
			filenameR = strBufR;
			filenameL2 = strBufL2;
			filenameR2 = strBufR2;

			imageL = cv::imread(filenameL, cv::IMREAD_ANYCOLOR);
			imageR = cv::imread(filenameR, cv::IMREAD_ANYCOLOR);

			cv::undistort(imageL, imageUndistortedL, intrinsicL, distCoeffsL);
			cv::undistort(imageR, imageUndistortedR, intrinsicR, distCoeffsR);

			cv::imwrite(filenameL2, imageUndistortedL);
			cv::imwrite(filenameR2, imageUndistortedR);
		}

	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		strErr = msg_e;
		return false;
	}
#endif//OPENCV_CALIB_3D_DISABLE
	return true;
}

bool CDebugFormView::TestHistogram()//測試直方圖
{
#ifndef OPENCV_DISABLE
	CString strErr;
	try
	{
		cv::Mat src, dst;
		std::string filename;

		filename="D:\\CameraCalibration\\baboon.jpg";
		filename="D:\\CameraCalibration\\hand_sample3.jpg";
		filename="D:\\CameraCalibration\\Megamind.png";
		filename="D:\\CameraCalibration\\Frame_00032.png";		
		// Load image
		src = cv::imread(filename, cv::IMREAD_COLOR);
		if( !src.data )
		{ return false; }

		cv::GaussianBlur(src, src, cv::Size(3, 3), 2, 2 );
		filename="R:\\src.png"; cv::imwrite(filename, src);

		const int nSrcType = src.type();		
		const int nSrcDepth = src.depth();
		const int nSrcChannels = src.channels();
		const int nSplitType=CV_MAKETYPE(nSrcDepth, 1);
		// Separate the image in 3 places ( B, G and R )
		std::vector<cv::Mat> bgr_planes;		
		bgr_planes.push_back(cv::Mat(src.size(), nSplitType));
		bgr_planes.push_back(cv::Mat(src.size(), nSplitType));
		bgr_planes.push_back(cv::Mat(src.size(), nSplitType));
		cv::split(src, bgr_planes);//需要提前建立記憶體區塊, 否則在記憶體釋放時會有蟲蟲//OPEN_CV_2_4_11_00_V10		
		filename="R:\\srcB.png"; cv::imwrite(filename, bgr_planes[0]);
		filename="R:\\srcG.png"; cv::imwrite(filename, bgr_planes[1]);
		filename="R:\\srcR.png"; cv::imwrite(filename, bgr_planes[2]);

		//RGB->HSV
		cv::Mat srcHSV;
		std::vector<cv::Mat> hsv_planes;
		cv::cvtColor(src, srcHSV, cv::COLOR_RGB2HSV);
		hsv_planes.push_back(cv::Mat(src.size(), nSplitType));
		hsv_planes.push_back(cv::Mat(src.size(), nSplitType));
		hsv_planes.push_back(cv::Mat(src.size(), nSplitType));
		cv::split(srcHSV, hsv_planes);//需要提前建立記憶體區塊, 否則在記憶體釋放時會有蟲蟲//OPEN_CV_2_4_11_00_V10		
		filename="R:\\srcH.png"; cv::imwrite(filename, hsv_planes[0]);
		filename="R:\\srcS.png"; cv::imwrite(filename, hsv_planes[1]);
		filename="R:\\srcV.png"; cv::imwrite(filename, hsv_planes[2]);
		cv::equalizeHist(hsv_planes[2], hsv_planes[2]);//均值化亮度的長條圖
		filename="R:\\srcV2.png"; cv::imwrite(filename, hsv_planes[2]);

		cv::Mat dstHSV;
		cv::merge(hsv_planes, dstHSV);
		cv::cvtColor(dstHSV, dst, cv::COLOR_HSV2RGB);
		filename="R:\\Dst.png"; cv::imwrite(filename, dst);

		std::vector<cv::Mat> bgr_planes2;
		bgr_planes2.push_back(cv::Mat(src.size(), nSplitType));
		bgr_planes2.push_back(cv::Mat(src.size(), nSplitType));
		bgr_planes2.push_back(cv::Mat(src.size(), nSplitType));
		cv::split(dst, bgr_planes2);//需要提前建立記憶體區塊, 否則在記憶體釋放時會有蟲蟲//OPEN_CV_2_4_11_00_V10		

		//Establish the number of bins
		int histSize = 256;

		//Set the ranges ( for B,G,R) )
		float range[] = { 0, 256};
		const float* histRange = {range};
		const bool uniform = true; 
		const bool accumulate = false;
		cv::Mat b_hist, g_hist, r_hist;
		cv::Mat b_hist2, g_hist2, r_hist2;
		
		//Compute the histograms:
		cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
		cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
		cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
		
		cv::calcHist(&bgr_planes2[0], 1, 0, cv::Mat(), b_hist2, 1, &histSize, &histRange, uniform, accumulate );
		cv::calcHist(&bgr_planes2[1], 1, 0, cv::Mat(), g_hist2, 1, &histSize, &histRange, uniform, accumulate );
		cv::calcHist(&bgr_planes2[2], 1, 0, cv::Mat(), r_hist2, 1, &histSize, &histRange, uniform, accumulate );
		
		int Max = 0;
		for( int i=1; i<histSize; i++ )
		{				
			float bVal=b_hist.at<float>(i-1);
			float gVal=g_hist.at<float>(i-1);
			float rVal=r_hist.at<float>(i-1);

			float bVal2=b_hist2.at<float>(i-1);
			float gVal2=g_hist2.at<float>(i-1);
			float rVal2=r_hist2.at<float>(i-1);

			if ( Max < bVal ) { Max = bVal; }
			i = i;
		}

		// Draw the histograms for B, G and R
		int hist_w = 512; int hist_h = 400;
		int bin_w = cvRound( (double) hist_w/histSize );

		cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0,0,0) );
		cv::Mat histImage2(hist_h, hist_w, CV_8UC3, cv::Scalar(0,0,0) );
		//Normalize the result to [ 0, histImage.rows ], 將數量縮放置影像高度400pixel		
		cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
		cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
		cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );

		cv::normalize(b_hist2, b_hist2, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
		cv::normalize(g_hist2, g_hist2, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );
		cv::normalize(r_hist2, r_hist2, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );

		//Draw for each channel
		Max = 0;
		for( int i=1; i<histSize; i++ )
		{	
			int x1 = (int)(bin_w*(i-1));
			int x2 = (int)(bin_w*(i));
			float bVal=b_hist.at<float>(i-1);
			float gVal=g_hist.at<float>(i-1);
			float rVal=r_hist.at<float>(i-1);
			
			if ( Max < bVal ) { Max = bVal; }

			cv::line(histImage, cv::Point(bin_w*(i-1), hist_h-std::round(b_hist.at<float>(i-1)) ) ,
			cv::Point(bin_w*(i), hist_h-std::round(b_hist.at<float>(i)) ), cv::Scalar( 255, 0, 0), 2, 8, 0 );

			cv::line( histImage, cv::Point(bin_w*(i-1), hist_h-std::round(g_hist.at<float>(i-1)) ) ,
			cv::Point( bin_w*(i), hist_h-std::round(g_hist.at<float>(i)) ), cv::Scalar( 0, 255, 0), 2, 8, 0 );

			cv::line( histImage, cv::Point( bin_w*(i-1), hist_h-std::round(r_hist.at<float>(i-1)) ) ,
			cv::Point( bin_w*(i), hist_h-std::round(r_hist.at<float>(i)) ), cv::Scalar( 0, 0, 255), 2, 8, 0 );

			cv::line(histImage2, cv::Point(bin_w*(i-1), hist_h-std::round(b_hist2.at<float>(i-1)) ) ,
			cv::Point(bin_w*(i), hist_h-std::round(b_hist2.at<float>(i)) ), cv::Scalar( 255, 0, 0), 2, 8, 0 );

			cv::line( histImage2, cv::Point(bin_w*(i-1), hist_h-std::round(g_hist2.at<float>(i-1)) ) ,
			cv::Point( bin_w*(i), hist_h-std::round(g_hist2.at<float>(i)) ), cv::Scalar( 0, 255, 0), 2, 8, 0 );

			cv::line( histImage2, cv::Point( bin_w*(i-1), hist_h-std::round(r_hist2.at<float>(i-1)) ) ,
			cv::Point( bin_w*(i), hist_h-std::round(r_hist2.at<float>(i)) ), cv::Scalar( 0, 0, 255), 2, 8, 0 );
		}

		filename = "R:\\Histogram.PNG";
		cv::imwrite(filename, histImage);		

		filename = "R:\\Histogram2.PNG";
		cv::imwrite(filename, histImage2);		


		//也可以使用Histogram來進行比較[cv::compareHist], 來進行圖像匹配
	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		strErr = msg_e;
		return false;
	}
#endif//OPENCV_DISABLE
	return true;
}

bool CDebugFormView::TestBackProject()//測試反投影
{
	//將值方圖的的數值取代影像中同灰階範圍
	//000   001   002   003//灰階影像, 數值為灰階值
	//004   005   006   007
	//008   009   010   011
	//008   009   014   015
	//使用Bin為4為範圍, [0,3], [4,7], [8,11], [12, 15], 則Histogram為下
	//[ 0, 3]=000, 001, 002, 003=>4個像素
	//[ 4, 7]=004, 005, 006, 007=>4個像素
	//[ 8,11]=008, 009, 010, 011, 008, 009=>6個像素
	//[12,15]=014, 015=>2個像素
	//在將Histogram取代源來像素所屬Bin內的值
	//004   004   004   004 -Bin1[ 0, 3]=4
	//004   004   004   004 -Bin2[ 4, 7]=4
	//006   006   006   006 -Bin3[ 8,11]=6
	//006   006             -Bin3[ 8,11]=6
	//            002   002 -Bin4[12,15]=2
#ifndef OPENCV_DISABLE
	CString strErr;
	try
	{
		cv::Mat src, dst;
		std::string filename;
		filename="D:\\CameraCalibration\\hand_sample2.jpg";		
		// Load image
		src = cv::imread(filename, cv::IMREAD_COLOR);
		if( !src.data )
		{ return false; }

		//cv::GaussianBlur(src, src, cv::Size(3, 3), 2, 2 );
		filename="R:\\src.png"; cv::imwrite(filename, src);

		const int nSrcType = src.type();		
		const int nSrcDepth = src.depth();
		const int nSrcChannels = src.channels();
		const int nSplitType=CV_MAKETYPE(nSrcDepth, 1);
		// Separate the image in 3 places ( B, G and R )
		//RGB->HSV
		cv::Mat srcHSV;		
		cv::cvtColor(src, srcHSV, cv::COLOR_RGB2HSV);

		cv::Mat srcHug;
		int ch[] = {0, 0};
		const int nChCnt = 1;
		const int nSrcChCpy = 1;
		const int nDstChCpy = 1;		
		srcHug.create(srcHSV.size(), srcHSV.depth());
		cv::mixChannels(&srcHSV, nSrcChCpy, &srcHug, nDstChCpy, ch, nChCnt);		
		filename="R:\\srcH.png"; cv::imwrite(filename, srcHug);
		
		//Establish the number of bins
		int histSize = MAX(25, 2);

		//Set the ranges ( for B,G,R) )
		float range[] = { 0, 180};
		const float* histRange = {range};
		const bool uniform = true; 
		const bool accumulate = false;
		cv::Mat h_hist;
		
		//Compute the histograms:
		cv::calcHist(&srcHug, 1, 0, cv::Mat(), h_hist, 1, &histSize, &histRange, uniform, accumulate );		
		
		// Draw the histograms for B, G and R
		int hist_w = 512; int hist_h = 256;
		int bin_w = cvRound( (double) hist_w/histSize );

		cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0,0,0) );		
		//Normalize the result to [ 0, histImage.rows ], 將數量縮放置影像高度256pixel		
		cv::normalize(h_hist, h_hist, 0, histImage.rows-1, cv::NORM_MINMAX, -1, cv::Mat() );
		
		//Get Back Projection
		cv::Mat backProj;
		cv::calcBackProject(&srcHug, 1, 0, h_hist, backProj, &histRange, 1, true);

		//Draw for each channel		
		for( int i=0; i<histSize; i++ )
		{		
			int x1 = bin_w*(i+0);
			int x2 = bin_w*(i+1);
			float y1 = h_hist.at<float>(i);
			float y2 = h_hist.at<float>(i);
			int ny1 = hist_h;//-std::round(y1);
			int ny2 = hist_h-std::round(y2);
			//cv::line(histImage, cv::Point(bin_w*(i-1), hist_h-std::round(h_hist.at<float>(i-1)) ) ,
			//cv::Point(bin_w*(i), hist_h-std::round(h_hist.at<float>(i)) ), cv::Scalar( 255, 255, 255), 2, 8, 0 );
			cv::rectangle(histImage, cv::Point(x1, ny1), cv::Point(x2, ny2), cv::Scalar(0, 0, 255), -1);
		}

		filename = "R:\\Histogram.PNG";
		cv::imwrite(filename, histImage);		

		filename = "R:\\BackProject.PNG";
		cv::imwrite(filename, backProj);		
	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		strErr = msg_e;
		return false;
	}
#endif//OPENCV_DISABLE
	return true;
}

bool CDebugFormView::TestOpenCVContour()//測試OpenCV輪廓
{	//OPEN_CV_2_4_11_00_V10有蟲
#ifndef OPENCV_DISABLE
	CString strErr;
	try
	{
		int thresh = 100;
		int max_thresh = 255;
		cv::Mat src, src_gray, dst;
		std::string filename;
		filename="D:\\CameraCalibration\\HappyFish.jpg";		
		// Load image
		src = cv::imread(filename, cv::IMREAD_COLOR);
		if( !src.data )
		{ return false; }

		//cv::GaussianBlur(src, src, cv::Size(3, 3), 2, 2 );
		filename="R:\\src.png"; cv::imwrite(filename, src);
		cv::cvtColor(src, src_gray, CV_BGR2GRAY);
		cv::blur(src_gray, src_gray, cv::Size(3,3));
		filename="R:\\src_gray.png"; cv::imwrite(filename, src_gray);

		const int nSrcType = src.type();		
		const int nSrcDepth = src.depth();
		const int nSrcChannels = src.channels();
		const int nSplitType=CV_MAKETYPE(nSrcDepth, 1);

		cv::Mat canny_output;
		const int tempCnt=100;
		std::vector<cv::Vec4i> hierarchy;
		std::vector<std::vector<cv::Point>> contours;
		/*
		std::vector<cv::Point> ptList;
		for ( int i=0; i<tempCnt; i++ )
		{	ptList.push_back(cv::Point());	}		
		for ( int i=0; i<tempCnt; i++ )
		{
			hierarchy.push_back(cv::Vec4i());
			contours.push_back(ptList);
		}
		hierarchy.clear();
		contours.clear();
		*/
		//Detect edges using canny
		cv::Canny(src_gray, canny_output, thresh, thresh*2, 3);
		filename = "R:\\canny.PNG";
		cv::imwrite(filename, canny_output);		

		//Find contours		
		cv::findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

		cv::RNG rng(12345);
		cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);
		for ( int i=0; i<contours.size(); i++ )
		{
			//if ( i > 50 ) { break; }

			cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			cv::drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
		}
		
		filename = "R:\\Contours.PNG";
		cv::imwrite(filename, drawing);		
	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		strErr = msg_e;
		return false;
	}
#endif//OPENCV_DISABLE
	return true;
}

bool CDebugFormView::ShowOpenCVMatrix(cv::Mat &Mat)
{	
#ifndef OPENCV_DISABLE
	CString strErr;
	try
	{	
		double val=0;
		std::vector<double> List;		
		const int nMatRows=Mat.rows;
		const int nMatCols=Mat.cols;

		for ( int i=0; i<Mat.rows; i++ )
		{
			for ( int j=0; j<Mat.cols; j++ )
			{
				val = Mat.ptr<double>(i)[j];
				List.push_back(val);
			}
		}
		val = 0;
	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		strErr = msg_e;
		return false;
	}
#endif//OPENCV_DISABLE
	return true;
}

bool CDebugFormView::SetOpenCVMatrix(int nRows, int nCols, double *Ptr)
{
	if ( NULL == Ptr ) { return false; }
#ifndef OPENCV_DISABLE
	CString strErr;
	try
	{	
		double val=0;
		std::vector<double> List;
		cv::Mat mat(nRows, nCols, CV_64FC1, Ptr);		
		const int nMatRows=mat.rows;
		const int nMatCols=mat.cols;

		for ( int i=0; i<mat.rows; i++ )
		{
			for ( int j=0; j<mat.cols; j++ )
			{
				val = mat.ptr<double>(i)[j];
				List.push_back(val);
			}
		}
		
	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		strErr = msg_e;
		return false;
	}
#endif//OPENCV_DISABLE
	return true;
}

bool CDebugFormView::TestHeightFactorMappingFunc()//測試高度比例的映射函式
{	
#ifndef OPENCV_DISABLE
	CString strErr;
	try
	{
		CString filename;		
		TPhaseFactorGrid Grid;		
		TPhaseFactorTable GridTable;
		std::vector<TPhaseFactorGrid> GridList0;
		std::vector<TPhaseFactorGrid> GridList1;
		std::vector<TPhaseFactorGrid> GridList2;		
		std::vector<TPhaseFactorGrid> GridList3;
		std::vector<TPhaseFactorGrid> GridList4;		
		std::vector<TPhaseFactorGrid> GridList5;
		std::vector<TPhaseFactorGrid> GridList6;		
		std::vector<TPhaseFactorGrid> GridErrList;		
		std::vector<std::vector<TPhaseFactorGrid>> GridListArray;//驗證用

		double TargetH=0;
		int CountX=0, CountY=0;
		const bool bTemp = false;
		LIGHT_3D_CAST_ID CastID=LIGHT_3D_CAST_01;
		filename = AOIDataCollect.GetPhaseFactorFilename(CastID, 500, bTemp);
		AOIDataCollect.LoadPhaseFactorTableFile(filename, GridTable); GridList1=GridTable.GridList; GridTable.GridList.clear();
		filename = AOIDataCollect.GetPhaseFactorFilename(CastID, 1000, bTemp);
		AOIDataCollect.LoadPhaseFactorTableFile(filename, GridTable); GridList2=GridTable.GridList; GridTable.GridList.clear();
		filename = AOIDataCollect.GetPhaseFactorFilename(CastID, 2000, bTemp);
		AOIDataCollect.LoadPhaseFactorTableFile(filename, GridTable); GridList3=GridTable.GridList; GridTable.GridList.clear();
		filename = AOIDataCollect.GetPhaseFactorFilename(CastID, 3000, bTemp);
		AOIDataCollect.LoadPhaseFactorTableFile(filename, GridTable); GridList4=GridTable.GridList; GridTable.GridList.clear();
		filename = AOIDataCollect.GetPhaseFactorFilename(CastID, 5000, bTemp);
		AOIDataCollect.LoadPhaseFactorTableFile(filename, GridTable); GridList5=GridTable.GridList; GridTable.GridList.clear();
		filename = AOIDataCollect.GetPhaseFactorFilename(CastID, 10000, bTemp);
		AOIDataCollect.LoadPhaseFactorTableFile(filename, GridTable); GridList6=GridTable.GridList; GridTable.GridList.clear();

		GridList0 = GridList4;		
		for ( size_t i=0; i<GridList0.size(); i++ )
		{
			GridList0[i].m_Phase=GridList0[i].m_PhaseBase=GridList0[i].m_PhaseTarget=GridList0[i].m_PhaseOffset=0;
			GridList0[i].m_Height=GridList0[i].m_HeightBase=GridList0[i].m_HeightTarget=GridList0[i].m_HeightOffset=0;
		}
		
		//校正用
		if ( GridList0.size() > 0 )	{	GridListArray.push_back(GridList0); }//0um
		//if ( GridList1.size() > 0 )	{	GridListArray.push_back(GridList1); }//500um
		//if ( GridList2.size() > 0 )	{	GridListArray.push_back(GridList2); }//1000um
		//if ( GridList3.size() > 0 )	{	GridListArray.push_back(GridList3); }//2000um
		if ( GridList4.size() > 0 )	{	GridListArray.push_back(GridList4); }//3000um
		if ( GridList5.size() > 0 )	{	GridListArray.push_back(GridList5); }//5000um
		if ( GridList6.size() > 0 )	{	GridListArray.push_back(GridList6); }//10000um

		if ( GridList0.size() > 0 )	{	GridErrList.push_back(GridList0[0]); }//0um
		if ( GridList1.size() > 0 )	{	GridErrList.push_back(GridList1[0]); }//500um
		if ( GridList2.size() > 0 )	{	GridErrList.push_back(GridList2[0]); }//1000um
		if ( GridList3.size() > 0 )	{	GridErrList.push_back(GridList3[0]); }//2000um
		if ( GridList4.size() > 0 )	{	GridErrList.push_back(GridList4[0]); }//3000um
		if ( GridList5.size() > 0 )	{	GridErrList.push_back(GridList5[0]); }//5000um
		if ( GridList6.size() > 0 )	{	GridErrList.push_back(GridList6[0]); }//10000um

		const size_t GridErrCount=GridErrList.size();
		const size_t GridListCount = GridListArray.size();		

		std::vector<double> KList;
		std::vector<double> KList2;		
		std::vector<TPhaseFactorGrid> GridListFunc;
		//0um
		//if ( GridList0.size() > 0 )	{	GridListFunc.insert(GridListFunc.end(), GridList0.begin(), GridList0.end()); }
		//500um
		//if ( GridList1.size() > 0 )	{	GridListFunc.insert(GridListFunc.end(), GridList1.begin(), GridList1.end()); }
		//1000um
		//if ( GridList2.size() > 0 )	{	GridListFunc.insert(GridListFunc.end(), GridList2.begin(), GridList2.end()); }
		//2000um
		//if ( GridList3.size() > 0 )	{	GridListFunc.insert(GridListFunc.end(), GridList3.begin(), GridList3.end()); }
		//3000um
		if ( GridList4.size() > 0 )	{	GridListFunc.insert(GridListFunc.end(), GridList4.begin(), GridList4.end()); }
		//5000um
		//if ( GridList5.size() > 0 )	{	GridListFunc.insert(GridListFunc.end(), GridList5.begin(), GridList5.end()); }
		//10000um
		//if ( GridList6.size() > 0 )	{	GridListFunc.insert(GridListFunc.end(), GridList6.begin(), GridList6.end()); }		
		AOIDataCollect.CalcPhaseFactorXYKMappingFunc(7, GridListFunc, KList);
		AOIDataCollect.CalcPhaseFactorXYPhaseMappingFunc(7, GridListFunc, KList2);
		

		double AveErr=0, MaxErr=0, RatioErr=0, MaxRatioErr=0, MaxErrHeight=0;		
		std::vector<TPhaseFactorGrid> GridListVer;
		
		//0 um
		//if ( GridList0.size() > 0 )	{	GridListVer.insert(GridListVer.end(), GridList0.begin(), GridList0.end()); }
		//500 um
		if ( GridList1.size() > 0 )	{	GridListVer.insert(GridListVer.end(), GridList1.begin(), GridList1.end()); }
		//1000 umm
		if ( GridList2.size() > 0 )	{	GridListVer.insert(GridListVer.end(), GridList2.begin(), GridList2.end()); }
		//2000 um
		if ( GridList3.size() > 0 )	{	GridListVer.insert(GridListVer.end(), GridList3.begin(), GridList3.end()); }
		//3000 um
		if ( GridList4.size() > 0 )	{	GridListVer.insert(GridListVer.end(), GridList4.begin(), GridList4.end()); }
		//5000 um
		if ( GridList5.size() > 0 )	{	GridListVer.insert(GridListVer.end(), GridList5.begin(), GridList5.end()); }		
		//10000 um
		if ( GridList6.size() > 0 )	{	GridListVer.insert(GridListVer.end(), GridList6.begin(), GridList6.end()); }

		const size_t KCount=KList.size();
		const size_t KCount2=KList2.size();
		std::vector<double> T(KCount);		
		const size_t VerCount=GridListVer.size();
		std::vector<double> ErrorList(VerCount);
		std::vector<double> HeightList(VerCount);

		AveErr = MaxRatioErr = MaxErr = 0;
		for ( int i=0; i<GridErrCount; i++ )
		{
			GridErrList[i].m_ImgX = GridErrList[i].m_ImgY = 0;
			GridErrList[i].m_Phase = 0;			
			GridErrList[i].m_Factor = 0;
		}
		for ( int i=0; i<VerCount; i++ )
		{
			double F=1.0;
			double XXX=0, YYY=0, PPP=0;
			double XXY=0, XXP=0, YYX=0, YYP=0, PPX=0, PPY=0;
			double XX=0, YY=0, PP=0, XY=0, XP=0, YP=0;
			double X=GridListVer[i].m_ImgX;
			double Y=GridListVer[i].m_ImgY;
			double P=GridListVer[i].m_Phase;
			double Height=GridListVer[i].m_Height;
			double C=1;

			switch ( KCount )
			{
			case 3:
				if ( P < 0.1 ) { continue; }
				T[0]  = C;//
				T[1]  = X;//u-x
				T[2]  = Y;//v-y

				F = P;
				break;
			case 5:
				T[0]  = C;//c				
				T[1]  = P;//p-phase				
				T[2]  = X*P;//up
				T[3]  = Y*P;//vp				
				T[4]  = P*P;//pp				
				break;
			case 6:
				if ( P < 0.1 ) { continue; }

				T[0]  = C;//
				T[1]  = X;//u-x
				T[2]  = Y;//v-y
				T[3]  = X*Y;//uv				
				T[4]  = X*X;//uu
				T[5]  = Y*Y;//vv

				F = P;
				break;
			case 7://2nd				
				if ( P < 0.1 ) { continue; }

				T[0]  = C;//
				T[1]  = X;//u-x
				T[2]  = Y;//v-y
				T[3]  = X*X;//uu
				T[4]  = Y*Y;//vv
				T[5]  = X*X*X;//uuu
				T[6]  = Y*Y*Y;//vvv

				F = P;
				break;
			case 10://2nd
				T[0]  = C;//
				T[1]  = X;//u-x
				T[2]  = Y;//v-y
				T[3]  = P;;//p-hase
				T[4]  = X*Y;//uv
				T[5]  = X*P;//up
				T[6]  = Y*P;//vp
				T[7]  = X*X;//uu
				T[8]  = Y*Y;//vv
				T[9]  = P*P;//pp

				if ( P < 0.1 ) { continue; }

				T[0]  = C;//
				T[1]  = X;//u-x
				T[2]  = Y;//v-y
				T[3]  = X*Y;//uv				
				T[4]  = X*X;//uu
				T[5]  = Y*Y;//vv

				T[6]  = X*X*Y;//uuv				
				T[7]  = X*Y*Y;//uvv
				T[8]  = X*X*X;//uuu
				T[9]  = Y*Y*Y;//vvv

				F = P;
				break;
			case 11:
				T[0]  = C;//				
				T[1]  = P;//p-hase
				
				T[2]  = X*P;//up
				T[3]  = Y*P;//vp				
				T[4]  = P*P;//pp

				T[5] = X*Y*P;//uvp
				T[6] = X*P*X;//uup
				T[7] = Y*P*Y;//vvp
				T[8] = X*P*P;//upp
				T[9] = Y*P*P;//vpp				
				T[10] = P*P*P;//ppp
				break;
			case 20://3rd
				T[0]  = C;//
				T[1]  = X;//u-x
				T[2]  = Y;//v-y
				T[3]  = P;;//p-hase

				T[4]  = X*Y;//uv
				T[5]  = X*P;//up
				T[6]  = Y*P;//vp
				T[7]  = X*X;//uu
				T[8]  = Y*Y;//vv
				T[9]  = P*P;//pp

				T[10] = X*Y*X;//uuv
				T[11] = X*Y*Y;//uvv
				T[12] = X*Y*P;//uvp
				T[13] = X*P*X;//uup
				T[14] = Y*P*Y;//vvp
				T[15] = X*P*P;//upp
				T[16] = Y*P*P;//vpp
				T[17] = X*X*X;//uuu
				T[18] = Y*Y*Y;//vvv
				T[19] = P*P*P;//ppp
				break;
			}

			double tempD = 0;			
			HeightList[i] = 0;
			ErrorList[i] = 0;			
			for ( int j=0; j<KCount; j++ )
			{
				tempD = KList[j]*T[j];
				HeightList[i] += tempD;
			}
			HeightList[i] *= F;
			ErrorList[i] = fabs(HeightList[i]-Height);
			if ( Height < 0.1 ) { RatioErr = 0; }
			else { RatioErr = 100*ErrorList[i]/Height; }				
			AveErr += ErrorList[i];
			if ( MaxErr < ErrorList[i] )
			{	
				MaxErr = ErrorList[i]; 
				MaxErrHeight = Height;
			}
			if ( MaxRatioErr < RatioErr )
			{	MaxRatioErr = RatioErr; }


			for ( int j=0; j<GridErrCount; j++ )
			{
				if ( fabs(GridErrList[j].m_Height-Height) > 0.1 ) { continue; }
				if ( GridErrList[j].m_Factor < ErrorList[i] )
				{	GridErrList[j].m_Factor = ErrorList[i];	}
				break;
			}
		}
		AveErr /= VerCount;
		AveErr = AveErr;


		//測試同XY下不同相位的高度值
		std::vector<std::vector<double>> KListArray_Z;		
		const size_t PArrayCount=GridListArray[0].size();
		std::vector<std::vector<TPhaseFactorGrid>> GridListPArray(PArrayCount);//For MultiLayer Phase
		for ( int i=0; i<GridListCount; i++ )
		{
			int Mode = 7;
			int idx = 0;
			std::vector<double> KList_Phase;	
			const std::vector<TPhaseFactorGrid> &GridList = GridListArray[i];			
			AOIDataCollect.CalcPhaseFactorXYPhaseMappingFunc(Mode, GridList, KList_Phase);		
			const int KCount_Phase=KList_Phase.size();			
			const size_t ListCnt=GridList.size();

			T.resize(KCount_Phase);
			for ( int j=0; j<ListCnt; j++ )
			{				
				idx = j;
				if ( idx >= PArrayCount ) { continue; }
				double X=GridList[idx].m_ImgX;
				double Y=GridList[idx].m_ImgY;
				double P=GridList[idx].m_Phase;
				double Height=GridList[idx].m_Height;
				double C=1;

				//使用第1組的XY影像座標
				if ( GridListPArray[idx].size() > 0 ) 
				{
					X = GridListPArray[idx][0].m_ImgX;
					Y = GridListPArray[idx][0].m_ImgY;
				}
			
				switch ( KCount_Phase )
				{
				case 3:				
					T[0]  = C;//
					T[1]  = X;//u-x
					T[2]  = Y;//v-y
					break;			
				case 6:
					T[0]  = C;//
					T[1]  = X;//u-x
					T[2]  = Y;//v-y
					T[3]  = X*Y;//uv				
					T[4]  = X*X;//uu
					T[5]  = Y*Y;//vv
					break;
				case 7://2nd
					T[0]  = C;//
					T[1]  = X;//u-x
					T[2]  = Y;//v-y
					T[3]  = X*X;//uu
					T[4]  = Y*Y;//vv
					T[5]  = X*X*X;//uuu
					T[6]  = Y*Y*Y;//vvv
					break;
				case 10://2nd				
					T[0]  = C;//
					T[1]  = X;//u-x
					T[2]  = Y;//v-y
					T[3]  = X*Y;//uv				
					T[4]  = X*X;//uu
					T[5]  = Y*Y;//vv

					T[6]  = X*X*Y;//uuv				
					T[7]  = X*Y*Y;//uvv
					T[8]  = X*X*X;//uuu
					T[9]  = Y*Y*Y;//vvv
					break;			
				}

				double tempD = 0;
				double tempSum = 0;
				for ( int j=0; j<KCount_Phase; j++ )
				{
					tempD = KList_Phase[j]*T[j];
					tempSum += tempD;
				}			
			
				TPhaseFactorGrid PhaseFactorGrid;
				PhaseFactorGrid.m_ImgX = X;
				PhaseFactorGrid.m_ImgY = Y;
				PhaseFactorGrid.m_Height = Height;
				PhaseFactorGrid.m_Phase = tempSum;
				if ( fabs(tempSum) > 0.1 )
				{	PhaseFactorGrid.m_Factor = Height/tempSum; }
				GridListPArray[idx].push_back(PhaseFactorGrid);
			}
		}


		std::vector<std::vector<double>> ParamListArray_Z;		
		if ( GridListPArray.size() > 0 ) 
		{			
			for ( int i=0; i<GridListPArray.size(); i++ )
			{
				if ( GridListPArray[i].size() == 0 ) { continue; }

				const int ZMode = 3;
				std::vector<double> KList_Z;	
				AOIDataCollect.CalcPhaseFactorZMappingFunc(ZMode, GridListPArray[i], KList_Z);		
				KListArray_Z.push_back(KList_Z);
			}

			if ( KListArray_Z.size() > 0 ) 
			{
				int   idx=0;
				FILE *pfile=NULL;
				CString Kfilename;
				const size_t KListCnt_Z = KListArray_Z[0].size();
				const int nRows=sqrt(PArrayCount);
				const int nCols=sqrt(PArrayCount);
				for ( int i=0; i<KListCnt_Z; i++ )
				{
					Kfilename.Format(_T("R:\\KFParam_a%0d.TXT"), i);
					pfile = ::_tfopen(Kfilename, _T("w+"));
					if ( NULL == pfile ) { continue; }
					idx = 0;
					for ( int j=0; j<nRows; j++ )
					{
						for ( int k=0; k<nCols; k++ )
						{
							if ( idx >= PArrayCount ) { continue; }							
							if ( k > 0 ) 
							{	::fprintf(pfile, ", "); }
							::fprintf(pfile, "%.15f", KListArray_Z[idx][i]);							
							idx ++;
						}
						::fprintf(pfile, "\n");
					}
					::fclose(pfile); pfile=NULL;
					
				}

				for ( int i=0; i<KListCnt_Z; i++ )
				{	
					std::vector<TPhaseFactorGrid> ParamList;
					for ( int j=0; j<PArrayCount; j++ )
					{
						idx = j;						
						TPhaseFactorGrid PhaseFactorGrid=GridListPArray[idx][0];						
						PhaseFactorGrid.m_Height = KListArray_Z[idx][i];
						PhaseFactorGrid.m_Phase = KListArray_Z[idx][i];		
						ParamList.push_back(PhaseFactorGrid);
					}
					int Mode = 7;//3-1階, 4,5,6-2階, 7,10-3階					
					std::vector<double> KList_Param;				
					AOIDataCollect.CalcPhaseFactorXYPhaseMappingFunc(Mode, ParamList, KList_Param);	
					ParamListArray_Z.push_back(KList_Param);
				}
			}
			
			//測試整個高度
			for ( int i=0; i<GridErrCount; i++ )
			{
				GridErrList[i].m_ImgX = GridErrList[i].m_ImgY = 0;
				GridErrList[i].m_Phase = 0;			
				GridErrList[i].m_Factor = 0;
			}
			const int ParamListCount=ParamListArray_Z.size();
			if ( ParamListCount > 0 ) 
			{
				AveErr = MaxRatioErr = MaxErr = 0;
				for ( int i=0; i<VerCount; i++ )
				{	
					double tempD = 0;
					double tempSum = 0;
					double X=GridListVer[i].m_ImgX;
					double Y=GridListVer[i].m_ImgY;
					double P=GridListVer[i].m_Phase;
					double Height=GridListVer[i].m_Height;
					double C=1;
					std::vector<double> ParamList;

					//由XY求得a0, a1, a2的參數值
					for ( int j=0; j<ParamListCount; j++ )
					{
						std::vector<double> &rfParamList=ParamListArray_Z[j];
						int rfParamCount=rfParamList.size();
						T.resize(rfParamCount);
						switch ( rfParamCount )
						{
						case 3:
							T[0]  = C;//
							T[1]  = X;//u-x
							T[2]  = Y;//v-y
							break;
						case 4:
							T[0]  = C;//
							T[1]  = X;//u-x
							T[2]  = Y;//v-y
							T[3]  = X*Y;//uv
							break;
						case 5:
							T[0]  = C;//
							T[1]  = X;//u-x
							T[2]  = Y;//v-y
							T[3]  = X*X;//uu
							T[4]  = Y*Y;//vv							
							break;
						case 6:
							T[0]  = C;//
							T[1]  = X;//u-x
							T[2]  = Y;//v-y
							T[3]  = X*Y;//uv
							T[4]  = X*X;//uu
							T[5]  = Y*Y;//vv							
							break;
						case 7:
							T[0]  = C;//
							T[1]  = X;//u-x
							T[2]  = Y;//v-y
							T[3]  = X*X;//uu
							T[4]  = Y*Y;//vv
							T[5]  = X*X*X;//uuu
							T[6]  = Y*Y*Y;//vvv
							break;
						}
						tempSum = 0;
						for ( int k=0; k<rfParamCount; k++ )
						{
							tempD = rfParamList[k]*T[k];
							tempSum += tempD;
						}	

						//tempSum = KListArray_Z[i][j];
						ParamList.push_back(tempSum);
					}

					const int ParamCount=ParamList.size();
					switch ( ParamCount )
					{
					case 2:
						T[0] = C;
						T[1] = P;						
						break;
					case 3:
						T[0] = C;
						T[1] = P;
						T[2] = P*P;
						break;
					case 4:
						T[0] = C;
						T[1] = P;
						T[2] = P*P;
						T[3] = P*P*P;
						break;
					case 5:
						T[0] = C;
						T[1] = P;
						T[2] = P*P;
						T[3] = P*P*P;
						T[4] = P*P*P*P;
						break;
					}

					HeightList[i] = 0;
					ErrorList[i] = 0;					
					for ( int j=0; j<ParamCount; j++ )
					{
						tempD = ParamList[j]*T[j];
						HeightList[i] += tempD;
					}					
					ErrorList[i] = fabs(HeightList[i]-Height);
					if ( Height < 0.1 ) { RatioErr = 0; }
					else { RatioErr = 100*ErrorList[i]/Height; }	
					AveErr += ErrorList[i];
					if ( MaxErr < ErrorList[i] )
					{	
						MaxErr = ErrorList[i]; 
						MaxErrHeight = Height;
					}		
					if ( MaxRatioErr < RatioErr )
					{	MaxRatioErr = RatioErr; }
					for ( int j=0; j<GridErrCount; j++ )
					{
						if ( fabs(GridErrList[j].m_Height-Height) > 0.1 ) { continue; }
						if ( GridErrList[j].m_Factor < ErrorList[i] )
						{	GridErrList[j].m_Factor = ErrorList[i];	}
						break;
					}
				}
				AveErr /= VerCount;
				AveErr = AveErr;
			}			
		}		
		AveErr = AveErr;

		//驗證Light3D的建立參數
		double T0[HEIGHT_FACTOR_PARAM_COUNT];
		double T1[HEIGHT_FACTOR_PARAM_COUNT];
		double T2[HEIGHT_FACTOR_PARAM_COUNT];
		LIGHT_3D_CLS_PTR CastPtr = Light3DCtrl.GetLight3DCastPtr(CastID);
		if ( NULL != CastPtr )
		{
			bool bRecv = false;
			if ( CastPtr->BuildHeightFactorMappingParam(bRecv) == true ) 
			{	CastPtr->GetHeightFactorMappingParam(T0, T1, T2);	}
		}

		AveErr = AveErr;
	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		strErr = msg_e;
		return false;
	}
#endif//OPENCV_DISABLE
	return true;
}

bool CDebugFormView::TestOpenCVImageMoment()//測試OpenCV力矩
{
#ifndef OPENCV_DISABLE
	CString strErr;
	try
	{
		int thresh = 100;
		int max_thresh = 255;
		cv::Mat src, src_gray, dst;
		std::string filename;
		filename="D:\\CameraCalibration\\HappyFish.jpg";		
		// Load image
		src = cv::imread(filename, cv::IMREAD_COLOR);
		if( !src.data )
		{ return false; }

		//cv::GaussianBlur(src, src, cv::Size(3, 3), 2, 2 );
		filename="R:\\src.png"; cv::imwrite(filename, src);
		cv::cvtColor(src, src_gray, CV_BGR2GRAY);
		cv::blur(src_gray, src_gray, cv::Size(3,3));
		filename="R:\\src_gray.png"; cv::imwrite(filename, src_gray);

		const int nSrcType = src.type();		
		const int nSrcDepth = src.depth();
		const int nSrcChannels = src.channels();
		const int nSplitType=CV_MAKETYPE(nSrcDepth, 1);

		cv::Mat canny_output;
		const int tempCnt=100;
		std::vector<cv::Vec4i> hierarchy;
		std::vector<std::vector<cv::Point>> contours;
		/*
		std::vector<cv::Point> ptList;
		for ( int i=0; i<tempCnt; i++ )
		{	ptList.push_back(cv::Point());	}		
		for ( int i=0; i<tempCnt; i++ )
		{
			hierarchy.push_back(cv::Vec4i());
			contours.push_back(ptList);
		}
		hierarchy.clear();
		contours.clear();
		*/
		//Detect edges using canny
		cv::Canny(src_gray, canny_output, thresh, thresh*2, 3);
		filename = "R:\\canny.PNG";
		cv::imwrite(filename, canny_output);		

		//Find contours		
		cv::findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
		const size_t contours_size=contours.size();

		//Get the moments
		const bool bBinay=false;
		std::vector<cv::Moments> mu(contours_size);
		for ( int i=0; i<contours_size; i++ )
		{	mu[i] = cv::moments(contours[i], bBinay);	}

		//Get the mass centers;
		std::vector<cv::Point2f> mc(contours_size);
		for ( int i=0; i<contours_size; i++ )
		{	
			cv::Point2f pt;
			if ( 0.00 == mu[i].m00 ) { pt.x = pt.y = 0.0f; }
			else 
			{ 
				pt.x = mu[i].m10/mu[i].m00;
				pt.y = mu[i].m01/mu[i].m00;
			}
			mc[i] = pt;	
		}

		cv::RNG rng(12345);
		cv::Mat drawing = cv::Mat::zeros(canny_output.size(), CV_8UC3);

		filename = "R:\\Contours.PNG";
		for ( int i=0; i<contours_size; i++ )
		{
			//if ( i > 50 ) { break; }
			cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			cv::drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point());
			cv::circle(drawing, mc[i], 4, color, -1, 8, 0);			
			cv::imwrite(filename, drawing);		
		}
		//CV_CAP_PROP_FRAME_WIDTH
	}
	catch ( cv::Exception& e )
	{			
		const char* msg_e = e.what();  			
		strErr = msg_e;
		return false;
	}
#endif//OPENCV_DISABLE
	return true;
}

bool CDebugFormView::TestRawImagePixelFocus()//測試原圖的Modulation
{
	const char fnName[] = "CDebugFormView::TestRawImagePixelFocus";
	int   Cnt=0;
	int   u=0, v=0, w=0;
	int   i=0, j=0, k=0;
	CString    Folder;
	CString    filename;	
	const int  MaxLayer=2;
	const int  MaxImageCount=4;
	IMAGE_PTR  RestPtr[MaxLayer]={NULL};
	IMAGE_SIZE ImageW[MaxImageCount]={0};
	IMAGE_SIZE ImageH[MaxImageCount]={0};
	IMAGE_SIZE ImageStep[MaxImageCount]={0};
	IMAGE_SIZE BitCount[MaxImageCount]={0};
	IMAGE_PTR  ImagePtr[MaxImageCount]={0};

	double MinC=0, MaxC=0;
	double MinP=0, MaxP=0;
	double Sum=0;
	double Average=0.0;
	double Contrast=0;
	double Potential=0;
	double Im=0, Re=0, Ave=0;
	double P1=0, P2=0, P3=0, P4=0;
	size_t BufferSize = 0;
	
	for ( i=0; i<MaxImageCount; i++ )
	{
		ImagePtr[i] = NULL;
		ImageW[i] = ImageH[i] = ImageStep[i] = BitCount[i] = 0;
	}
	for ( k=0; k<MaxLayer; k++ )
	{
		
		switch ( k )
		{
		case 0:	Folder = _T("R:\\RawImage-7300");	break;
		case 1:	Folder = _T("R:\\RawImage-11300");	break;
		case 2:	Folder = _T("R:\\RawImage-11300");	break;
		default:
			Folder = _T("R:\\RawImage-7300");
			break;
		}
		
		for ( i=0; i<MaxImageCount; i++ )
		{
			//filename.Format(_T("%s\\%s%d.BMP"), _T("R:\\RawImage-7400"), _T("Cast01Pattern4S2P_A"), i+1);
			//filename.Format(_T("%s\\%s%d.BMP"), Folder, _T("Cast01Pattern4S2P_A"), i+1);
			//filename.Format(_T("%s\\%s%d.BMP"), Folder, _T("Cast02Pattern4S2P_A"), i+1);
			//filename.Format(_T("%s\\%s%d.BMP"), Folder, _T("Cast03Pattern4S2P_A"), i+1);
			filename.Format(_T("%s\\%s%d.BMP"), Folder, _T("Cast04Pattern4S2P_A"), i+1);
			if ( ImageAPI.LoadImage(filename, ImageW[i], ImageH[i], ImageStep[i], BitCount[i], ImagePtr[i], 4, true) == false )
			{
				for ( j=0; j<k; j++ )
				{	JetMemory.free_func(RestPtr[j]); }
				for ( j=0; j<i; j++ )
				{	JetMemory.free_func(ImagePtr[j]);	}
				return false;
			}
		}

		for ( i=1; i<MaxImageCount; i++ )
		{
			if ( ImageW[i]!=ImageW[0] || ImageH[i]!=ImageH[0] || ImageStep[i]!=ImageStep[0] || BitCount[i]!=BitCount[0] )
			{
				for ( j=0; j<k; j++ )
				{	JetMemory.free_func(RestPtr[j]); }
				for ( j=0; j<MaxImageCount; j++ )
				{	JetMemory.free_func(ImagePtr[j]);	}
				return false;
			}
		}

		BufferSize=ImageStep[0]*ImageH[0];
		if ( JetMemory.alloc_func(BufferSize, RestPtr[k], fnName, "RestPtr") == false )
		{
			for ( j=0; j<k; j++ )
			{	JetMemory.free_func(RestPtr[j]); }
			for ( j=0; j<MaxImageCount; j++ )
			{	JetMemory.free_func(ImagePtr[j]);	}
			return false;
		}

		MaxP=MaxC=-1.0;
		MinP=MinC=DBL_MAX;	
		for ( i=0; i<BufferSize; i++ )
		{
			P1 = ImagePtr[0][i];
			P2 = ImagePtr[1][i];
			P3 = ImagePtr[2][i];
			P4 = ImagePtr[3][i];

			Ave=(P1+P2+P3+P4)/2.0f;
			Average = (P1+P2+P3+P4)*0.25;
			Im=P4-P2;
			Re=P1-P3;
			Contrast=0.0f;
			if ( Ave != 0.0f )
			{	Contrast = (double)(100.0*(fabs(Im)+fabs(Re))/Ave);	}
			else 
			{	Contrast = 0.0; }

			Potential = (abs(P3-P2)+abs(P1-P2));	
			Potential = Potential/2.0;
			if ( Contrast > MaxC ) { MaxC = Contrast; }
			if ( Contrast < MinC ) { MinC = Contrast; }
			if ( Potential > MaxP ) { MaxP = Potential; }
			if ( Potential < MinP ) { MinP = Potential; }

			if ( Average > 255.0 ) { Average = 255.0; }
			if ( Contrast > 255.0 ) { Contrast = 255.0; }
			if ( Potential > 255.0 ) { Potential = 255.0; }
			
			//RestPtr[k][i] = static_cast<IMAGE_DATA>(Contrast);
			//RestPtr[k][i] = static_cast<IMAGE_DATA>(Potential);
			RestPtr[k][i] = static_cast<IMAGE_DATA>(Average);
		}
		filename.Format(_T("%s\\%s.PNG"), Folder, _T("Result"));
		ImageAPI.SaveImage(filename, ImageW[0], ImageH[0], ImageStep[0], BitCount[0], RestPtr[k], true);		
		
		/*
		Folder = _T("R:");
		switch ( k )
		{
		case 0:	filename.Format(_T("%s\\7300-G.PNG"), Folder);	break;
		case 1:	filename.Format(_T("%s\\11300-G.PNG"), Folder);	break;
		case 2:	filename.Format(_T("%s\\11300-G.PNG"), Folder);	break;
		default:
			filename.Format(_T("%s\\7300-G.PNG"), Folder);	break;
			break;
		}
		ImageAPI.LoadImage(filename, ImageW[0], ImageH[0], ImageStep[0], BitCount[0], RestPtr[k], 4, true);
		BufferSize=ImageStep[0]*ImageH[0];		
		*/
		const int BlurSize = 41;
		const int RoiSize  = 21;
		if ( ImageAPI.BuildFocusPixelImage3(ImageW[0], ImageH[0], ImageStep[0], BitCount[0], RestPtr[k], BlurSize, RoiSize, RestPtr[k]) == false )
		{
			for ( j=0; j<k; j++ )
			{	JetMemory.free_func(RestPtr[j]); }
			for ( j=0; j<MaxImageCount; j++ )
			{	JetMemory.free_func(ImagePtr[j]);	}
			return false;
		}		
		filename.Format(_T("%s\\%s.PNG"), Folder, _T("Result5"));
		ImageAPI.SaveImage(filename, ImageW[0], ImageH[0], ImageStep[0], BitCount[0], RestPtr[k], true);

		for ( j=0; j<MaxImageCount; j++ )
		{	JetMemory.free_func(ImagePtr[j]);	}
	}	
	
	IMAGE_PTR CombinPtr = NULL;
	if ( JetMemory.alloc_func(BufferSize, CombinPtr, fnName, "CombinPtr") == false )
	{
		for ( j=0; j<MaxLayer; j++ )
		{	JetMemory.free_func(RestPtr[j]); }
		for ( j=0; j<MaxImageCount; j++ )
		{	JetMemory.free_func(ImagePtr[j]);	}
		return false;
	}

	IMAGE_DATA clr0 = 64;
	IMAGE_DATA clr1 = 255;
	IMAGE_DATA clr2 = 255;
	if ( 2 == MaxLayer )
	{
		IMAGE_PTR Ptr1 = RestPtr[0];
		IMAGE_PTR Ptr2 = RestPtr[1];
		ImageAPI.Compare2FocusPixelImage3(ImageW[0], ImageH[0], ImageStep[0], BitCount[0], Ptr1, Ptr2, clr0, clr1, CombinPtr);		
	}
	else
	{
		for ( i=0; i<BufferSize; i++ )
		{
			CombinPtr[i] = 0;
			if ( RestPtr[0][i] > RestPtr[1][i] )
			{
				if ( RestPtr[0][i] > RestPtr[2][i] )
				{	CombinPtr[i] = clr0; }
				else
				{	CombinPtr[i] = clr2;	}
			}
			else
			{
				if ( RestPtr[1][i] > RestPtr[2][i] )
				{	CombinPtr[i] = clr1; }
				else
				{	CombinPtr[i] = clr2;	}
			}
		}
	}
	Folder = AOIDataCollect.GetAOITempDirectory();
	Folder = _T("R:");
	filename.Format(_T("%s\\%s.PNG"), Folder, _T("Result"));
	ImageAPI.SaveImage(filename, ImageW[0], ImageH[0], ImageStep[0], BitCount[0], CombinPtr, true);	

	JetMemory.free_func(CombinPtr);
	for ( k=0; k<MaxLayer; k++ )
	{	JetMemory.free_func(RestPtr[k]); }	
	return true;
}

bool CDebugFormView::TestCombine2PixelFocus()//測試合併2個對焦影像
{
	const char fnName[] = "CDebugFormView::TestCombine2PixelFocus";

	int   i=0;
	CString   filename;
	const int  MaxLayer = 2;
	IMAGE_PTR  CombinPtr=NULL;
	IMAGE_SIZE ImageW=0;
	IMAGE_SIZE ImageH=0;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount = 0;
	IMAGE_SIZE SpaceW[MaxLayer]={0};
	IMAGE_SIZE SpaceH[MaxLayer]={0};
	IMAGE_SIZE SpaceStep[MaxLayer]={0};
	MASK_PTR   MaskPtr[MaxLayer]={NULL};
	SPACE_PTR  SpacePtr[MaxLayer]={NULL};	
	for ( i=0; i<MaxLayer; i++ )
	{
		MaskPtr[i] = NULL;
		SpacePtr[i] = NULL;
	}

	for ( i=0; i<MaxLayer; i++ )
	{
		switch ( i )
		{
		case 0:	filename = "R:\\MergeSpace-7300.Z3D";	break;
		case 1:	filename = "R:\\MergeSpace-11300.Z3D";	break; 
		default:	filename = "";	break;
		}
		if ( filename.GetLength() == 0 )
		{	continue; }

		if ( ImageAPI.LoadSpaceGrayImage(filename, SpaceW[i], SpaceH[i], SpaceStep[i], SpacePtr[i], true) == false )
		{
			for ( i=0; i<MaxLayer; i++ )
			{
				JetMemory.free_func(MaskPtr[i]);
				JetMemory.free_func(SpacePtr[i]);		
			}
			return false;
		}
	}

	filename = _T("R:\\ResultMask.PNG";)
	if ( ImageAPI.LoadImage(filename, ImageW, ImageH, ImageStep, BitCount, CombinPtr, 4, true) == false )
	{
		for ( i=0; i<MaxLayer; i++ )
		{
			JetMemory.free_func(MaskPtr[i]);
			JetMemory.free_func(SpacePtr[i]);		
		}
		return false;
	}

	for ( i=0; i<MaxLayer; i++ )
	{
		if ( SpaceW[i]!=ImageW || SpaceH[i]!=ImageH || SpaceStep[i]!=ImageStep )
		{
			JetMemory.free_func(CombinPtr);
			for ( i=0; i<MaxLayer; i++ )
			{
				JetMemory.free_func(MaskPtr[i]);
				JetMemory.free_func(SpacePtr[i]);		
			}
			return false;
		}
	}

	SPACE_PTR  MergePtr=NULL;
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( JetMemory.alloc_func(BufferSize, MergePtr, fnName, "MergePtr") == false )
	{
		JetMemory.free_func(CombinPtr);
		for ( i=0; i<MaxLayer; i++ )
		{
			JetMemory.free_func(MaskPtr[i]);
			JetMemory.free_func(SpacePtr[i]);		
		}
		return false;
	}

	const float ZOffset=4000;
	for ( i=0; i<BufferSize; i++ )
	{
		if ( CombinPtr[i] > 128 ) { MergePtr[i] = SpacePtr[1][i]+ZOffset; }
		else { MergePtr[i] = SpacePtr[0][i]; }
		//MergePtr[i] = SpacePtr[0][i];
	}

	filename = _T("R:\\MergerSpace.Z3D");	
	ImageAPI.SaveSpaceGrayImage(filename, ImageW, ImageH, ImageStep, MergePtr, true);

	JetMemory.free_func(MergePtr);
	JetMemory.free_func(CombinPtr);
	for ( i=0; i<MaxLayer; i++ )
	{
		JetMemory.free_func(MaskPtr[i]);
		JetMemory.free_func(SpacePtr[i]);		
	}
	return true;
}

bool CDebugFormView::TestFloatImageForJoe()
{	
	const char fnName[]="CDebugFormView::TestFloatImageForJoe";
	size_t     i=0;
	CString    str;	
	IMAGE_SIZE W=4000;	
	IMAGE_SIZE H=3000;	
	IMAGE_SIZE Step=W;
	float     *FloatPtr=NULL;	
	PHASE_PTR  PhasePtr=NULL;
	const size_t BufferSize=ImageAPI.CalcBufferSize(Step, H);
	CString    Folder=AOIDataCollect.GetAOITempDirectory();
	if ( JetMemory.alloc_func(BufferSize, PhasePtr, fnName, "PhasePtr") == false ||
		 JetMemory.alloc_func(BufferSize, FloatPtr, fnName, "FloatPtr") == false )
	{	
		JetMemory.free_func(PhasePtr);
		JetMemory.free_func(FloatPtr);		
		return false; 
	}
	
	::memset(PhasePtr, 0x00, sizeof(PhasePtr[0])*BufferSize);
	::memset(FloatPtr, 0x00, sizeof(FloatPtr[0])*BufferSize);	

	for ( i=0; i<BufferSize; i++ )
	{	PhasePtr[i] = i;	}

	if ( ImageAPI.GrayPhaseConvertToFloat3(W, H, Step, PhasePtr, FloatPtr, false) == false )
	{
		JetMemory.free_func(PhasePtr);
		JetMemory.free_func(FloatPtr);		
		return false;
	}

	str.Format(_T("%s\\%s"), Folder, _T("FloatImage.Fimg"));
	if ( ImageAPI.SaveGrayPhaseFloatFile(str, W, H, Step, PhasePtr, false) == false )
	{	JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());	}
	else
	{
		IMAGE_SIZE W2=0;
		IMAGE_SIZE H2=0;
		IMAGE_SIZE Step2=0;
		IMAGE_SIZE BitCount2=0;
		float     *FloatPtr2=NULL;
		if (ImageAPI.LoadGrayFloatFile(str, W2, H2, Step2, BitCount2, FloatPtr2, false, 4) == false )
		{	JetAPI::ShowMessageBox(ImageAPI.GetImageApiErrorString());	}
		else
		{
			size_t ErrorCnt=0;
			const size_t BufferSize2=Step2*H2;
			if ( BufferSize2 != BufferSize )
			{	JetAPI::ShowMessageBox(_T("Error, Size Exception"));	}
			else
			{
				for ( i=0; i<BufferSize; i++ )
				{
					if ( FloatPtr2[i] == FloatPtr[i] ) { continue; }
					ErrorCnt ++;
				}
				if ( ErrorCnt > 0 ) 
				{	JetAPI::ShowMessageBox(_T("Error"));	}
			}
		}		
		JetMemory.free_func(FloatPtr2);
	}

	JetMemory.free_func(PhasePtr);
	JetMemory.free_func(FloatPtr);	
	return true;
}

bool CDebugFormView::TestCreateFolder()
{
	bool IsOK = true;
	CString Folder = _T("");

	IsOK = JetAPI::CreateFolder(Folder);

	Folder = _T("R:");
	IsOK = JetAPI::CreateFolder(Folder);

	Folder = _T("R:\\ABCD");
	IsOK = JetAPI::CreateFolder(Folder);

	Folder = _T("R:\\ABCD\\1234");
	IsOK = JetAPI::CreateFolder(Folder);

	Folder = _T("R:\\A\\B\\C\\D\\E\\F\\G");
	IsOK = JetAPI::CreateFolder(Folder);

	return true;
}

bool CDebugFormView::TestFilterPhase_Joe()
{
	double R=0;
	double H=100;
	double L=JetAPI::CalcDistance(R, H);
	double Angle=JetAPI::CalcCos(R, L, H)-90;
	double Angle2=JetAPI::CalcCos(H, L, R);

	return true;
	const char fnName[]="CDebugFormView::TestFilterPhase_Joe";
	IMAGE_SIZE ImageW=5120;
	IMAGE_SIZE ImageH=5120;
	IMAGE_SIZE ImageStep=5120;
	PHASE_PTR  DstPtr=NULL;
	PHASE_PTR  PhasePtr=NULL;
	const size_t BufferSize=ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if (JetMemory.alloc_func(BufferSize, DstPtr, fnName, "DstPtr") == false ||
		JetMemory.alloc_func(BufferSize, PhasePtr, fnName, "PhasePtr") == false)
	{
		JetMemory.free_func(DstPtr);
		JetMemory.free_func(PhasePtr);
		return false;
	}

	for (IMAGE_SIZE i = 0; i < ImageW; i++)
	{
		for (IMAGE_SIZE j = 0; j < ImageH; j++)
		{
			int idx=(j*ImageStep)+i;
			DstPtr[idx] = 0;
			PhasePtr[idx] = i;

		}
	}
	ImageAPI.CorrectPhaseZero_Joe3(ImageW, ImageH, ImageStep, PhasePtr, DstPtr, 2);

	JetMemory.free_func(DstPtr);
	JetMemory.free_func(PhasePtr);
	JetAPI::ShowMessageBox(AOIDataDefine.GetFinishText());
	return true;
}

void CDebugFormView::OnComponentConfigBtn() 
{
	// TODO: Add your control notification handler code here	
	CString str;
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }	
	CComponentConfigWnd Wnd;	
	Wnd.SetProjectPtr(ProjectPtr);
	Wnd.SetComponentConfigMode(COMPONENT_CONFIG_BOARD_ASSIGN);
	Wnd.DoModal();
}
void CDebugFormView::OnProjectCodeListBtn() 
{
	// TODO: Add your control notification handler code here	
	CProjectCodeListWnd Wnd;
	std::vector<TProjectOpenCode> OpenCodeList;
	CString      TxtFilename;	
	TxtFilename = AOIDataCollect.GetProjectOpenCodeFilename();
	AOIDataCollect.LoadProjectOpenCodeFile(TxtFilename, OpenCodeList);
	Wnd.SetOpenCodeList(OpenCodeList);
	if ( Wnd.DoModal() != IDOK )
	{	return; }	

	Wnd.CloneOpenCodeList(OpenCodeList);
	AOIDataCollect.SaveProjectOpenCodeFile(TxtFilename, OpenCodeList);
}

void CDebugFormView::OnOtherDebugBtn() 
{
	// TODO: Add your control notification handler code here
	//TestMoments();
	//TestMinRectArea();
	//TestColorRatio();
	//TestBlobContour();
	//TestKnnClassify();
	//TestKnnClassify_02();
	//TestOpenCV_SVM();
	//TestOpenCV_SVM_02();
	//TestDeSkew();
	//TestAdaBoost();
	//TestAdaBoost_02();
	//TestAdaBoost_03();
	//TestCopyFolder();
	//TestDeleteFolder();	
	//TestSortList();
	//TestRGBConvert();
	//TestSleep();
	//TestFloatImage();
	//TestBayerImage();
	//TestSmoothImage();
	//TestPyramidImage();
	//TestPyramidImageMerge();
	//TestBlendImageMerge();
	//TestOpenCVMat();
	//TestOpenCVMatList();
	//TestRoateImage();
	//TestRoateImage2();
	//TestExtractRoi();
	//TestMorphImage();
	//TestErodeDilateImage();
	//TestGaussianImage();	
	//TestFlipImage();
	//TestTransposeImage();
	//TestSmartPointer();
	//TestTimeStruct();
	//TestProjectSaveServerLibrary();
	//TestRemoteParamWnd();
	//TestSoftwareNoResponse();
	//TestDiskDrive();
	//TestSSEInstructionSet();
	//TestPhaseGamma();
	//TestPhaseGamma2();
	//TestNonLocalMean();
	//TestCorrectTShape();
	//TestCameraCalibration();
	//TestJetCameraCalibration();
	//TestStereoCameraCalibration();
	//TestHistogram();
	//TestBackProject();//測試反投影
	//TestOpenCVContour();
	//TestHeightFactorMappingFunc();
	//TestOpenCVImageMoment();
	//TestRawImagePixelFocus();
	//TestCombine2PixelFocus();
	//TestJSON_02();
	//TestFloatImageForJoe();
	//TestCreateFolder();
	TestFilterPhase_Joe();
	return;
}
