#include "stdafx.h"
#include "jet8000.h"
#include "ProjectMapMaskWnd.h"
#include "ProjectMapSpecRegionWnd.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
//-------------------------------------------------------------------------------------//

CProjectMapSpecRegionWnd::CProjectMapSpecRegionWnd(CWnd * pParent)
	:CProjectMapMaskWnd(CProjectMapSpecRegionWnd::IDD,pParent)
{
	m_EditGridW = 8;
	m_EditGridH = 8;
	m_SpecRegionList.clear();
	m_SpecTotalRegion = TREGION4D();
	m_EditingRect = false;
	m_RegionActiveIndex = 0;
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CProjectMapSpecRegionWnd, CProjectMapMaskWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(PMM_REGION_ADD_BTN, &CProjectMapSpecRegionWnd::OnRegionAddBtn)
	ON_BN_CLICKED(PMM_REGION_ERASE_BTN, &CProjectMapSpecRegionWnd::OnRegionEraseBtn)
	ON_BN_CLICKED(PMM_REGION_CLEAR_BTN, &CProjectMapSpecRegionWnd::OnRegionClearBtn)
	ON_BN_CLICKED(PMM_REGION_COLOR_BTN, &CProjectMapSpecRegionWnd::OnRegionColorBtn)
	ON_CBN_SELCHANGE(PMM_MASK_INDEX_COMBO, &CProjectMapSpecRegionWnd::OnSelchangeMaskIndexCombo)
	ON_CBN_SELCHANGE(PMM_REGION_INDEX_COMBO, &CProjectMapSpecRegionWnd::OnSelchangeRegionIndexCombo)
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
CProjectMapSpecRegionWnd::~CProjectMapSpecRegionWnd()
{
	m_SpecRegionList.clear();
	if (NULL != m_SpecRegionMaskImagePtr)
	{
		JetMemory.free_func(m_SpecRegionMaskImagePtr);
	}
}
//-------------------------------------------------------------------------------------//
BOOL CProjectMapSpecRegionWnd::OnInitDialog()
{
	CProjectMapMaskWnd::OnInitDialog();
	SwitchMultiLanguage();
	m_SpecRegionMaskImagePtr = NULL;
	LoadProjectSpecComponent();
	BuildMaskIndexCombox();
	const int nAlign = 4;
	const IMAGE_SIZE ImageW = m_ProjectMapW;
	const IMAGE_SIZE ImageH = m_ProjectMapH;
	const IMAGE_SIZE BitCount = 8;
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, nAlign);
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if (JetMemory.alloc_func(BufferSize, m_SpecRegionMaskImagePtr, "CProjectMapMaskWnd::CreateMaskImageBuffer()", "MapMaskPtr") == false) {
		return false;
	}
	::memset(m_SpecRegionMaskImagePtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);

	CComboBox &Combox = m_MaskIndexComobx;
	CString ItemName = _T("Mask Region");
	Combox.InsertString(5, ItemName);
	Combox.SetItemData(5, 5);
	
	BuildShowImage();
	DrawShowImage();
	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::RedrewWnd()
{
	CClientDC dc(&m_ImageWnd);
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ImageWndMemDC.GetSafeHdc();
	HDC hMemDC2 = m_ImageWndMemDC2.GetSafeHdc();
	if (NULL == hDC || NULL == hMemDC || NULL == hMemDC2) { return; }

	RECT WndRect = m_ImageWndRect;
	::BitBlt(hMemDC2, 0, 0, m_ImageWndRect.right, m_ImageWndRect.bottom, hMemDC, 0, 0, SRCCOPY);
	::IntersectClipRect(hMemDC2, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);

	//Draw Line List
	HDC hDCUsed = hMemDC2;//hDCUsed = hDC;		
	DrawComponent(hDCUsed);
	DrawSpecRng(hDCUsed);
	DrawEditGrid(hDCUsed);
	DrawCursorLine(hDCUsed);
	if (false == m_EditingRect) {
		DrawImageRgn(hDCUsed);
		DrawImageRoi(hDCUsed);
	}
	::BitBlt(hDC, 0, 0, m_ImageWndRect.right, m_ImageWndRect.bottom, hDCUsed, 0, 0, SRCCOPY);
	return;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapSpecRegionWnd::BuildShowImage()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	if ( NULL == m_MapMaskImagePtr ) { return false; }
	if ( NULL == m_ShowImagePtr ) { return false; }
	if ( m_ShowImageW != m_MapMaskImageW ) { return false; }
	if ( m_ShowImageH != m_MapMaskImageH ) { return false; }
	IMAGE_PTR ImagePtr = NULL;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE BitCount = 0;
	IMAGE_SIZE ImageStep = 0;
	const int MapIndex = m_MapIndex;
	if ( ProjectPtr->GetProjectMapPtr(MapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{	return false; }
	if ( ProjectPtr->CreateProjectMapShowPtr(MapIndex, ImagePtr, false) == false )
	{	return false; }	
	if ( m_ShowImageW != ImageW ) { return false; }
	if ( m_ShowImageH != ImageH ) { return false; }
	if ( NULL == ImagePtr ) { return false; }
	unsigned int FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(MapIndex, true);
	
	size_t i = 0, j = 0, k = 0;
	size_t maskIdx=0, imgIdx=0, showIdx=0;
	unsigned char maskR=0, maskG=0, maskB=0, maskV=0, Alpha=0;
	AOIDataCollect.GetMaskImageColor(FrameUniqueID, maskR, maskG, maskB, maskV, Alpha);//取得遮罩影像顏色

	maskR = 255-maskR;
	maskG = 255-maskG;
	maskB = 255-maskB;

	bool bColor = false;
	double GrayScale = 0;
	POINT pt = POINT();
	TREGION4D SpecRgn = TREGION4D();
	const size_t SpecRegionCount = m_SpecRegionList.size();
	const float wR = 0.299f, wG = 0.587f, wB = 0.114f;
	
	if ( PMM_SHOW_MASK_RADIO == m_ShowMode )
	{
		for ( i=0; i<ImageH; i++ )
		{
			for ( j=0; j<ImageW; j++ )
			{				
				maskIdx = (i*m_MapMaskImageStep)+j;
				showIdx = (i*m_ShowImageStep)+(j*3);
				if ( 0 == m_MapMaskImagePtr[maskIdx] )
				{
					m_ShowImagePtr[showIdx] = maskB;
					m_ShowImagePtr[showIdx+1] = maskG;
					m_ShowImagePtr[showIdx+2] = maskR;
				}
				else
				{	
					m_ShowImagePtr[showIdx] = 255;
					m_ShowImagePtr[showIdx+1] = 255;
					m_ShowImagePtr[showIdx+2] = 255;
				}
		
			}
		}
	}
	else if ( PMM_SHOW_IMAGE_RADIO == m_ShowMode )
	{
		if ( 24 == BitCount )
		{
			for ( i=0; i<ImageH; i++ )
			{
				for ( j=0; j<ImageW; j++ )
				{	
					imgIdx = (i*ImageStep)+(j*3);
					showIdx = (i*m_ShowImageStep)+(j*3);
					m_ShowImagePtr[showIdx] = ImagePtr[imgIdx];
					m_ShowImagePtr[showIdx+1] = ImagePtr[imgIdx+1];
					m_ShowImagePtr[showIdx+2] = ImagePtr[imgIdx+2];					
			
				}
			}
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				for ( j=0; j<ImageW; j++ )
				{	
					imgIdx = (i*ImageStep)+j;
					showIdx = (i*m_ShowImageStep)+(j*3);
					m_ShowImagePtr[showIdx] = ImagePtr[imgIdx];
					m_ShowImagePtr[showIdx+1] = ImagePtr[imgIdx];
					m_ShowImagePtr[showIdx+2] = ImagePtr[imgIdx];					
				}
			}
		}
	}
	else if ( PMM_SHOW_COMBINED_RADIO == m_ShowMode )
	{
		if ( 24 == BitCount )
		{
			for ( i=0; i<ImageH; i++ )
			{
				for ( j=0; j<ImageW; j++ )
				{				
					maskIdx = (i*m_MapMaskImageStep)+j;
					showIdx = (i*m_ShowImageStep)+(j*3);
					imgIdx = (i*ImageStep) + (j * 3);
					pt.x = j; pt.y = i;
					for (k = 0; k < SpecRegionCount; k++) {
						SpecRgn = m_SpecRegionList[k];
						bColor = false;
						if (true == SpecRgn.CheckPtInside(pt)) {
							bColor = true;
							break;
						}
					}
					if (true == bColor || SpecRegionCount == 0) {
						if (0 == m_MapMaskImagePtr[maskIdx])
						{
							m_ShowImagePtr[showIdx] = maskB;
							m_ShowImagePtr[showIdx + 1] = maskG;
							m_ShowImagePtr[showIdx + 2] = maskR;
							continue;
						}
						m_ShowImagePtr[showIdx] = ImagePtr[imgIdx];
						m_ShowImagePtr[showIdx + 1] = ImagePtr[imgIdx + 1];
						m_ShowImagePtr[showIdx + 2] = ImagePtr[imgIdx + 2];
					}
					else {
						GrayScale = ImagePtr[imgIdx] * wR + ImagePtr[imgIdx + 1] * wG + ImagePtr[imgIdx + 2] * wB;
						m_ShowImagePtr[showIdx] = m_ShowImagePtr[showIdx + 1] = m_ShowImagePtr[showIdx + 2] = GrayScale;
					}	
				}
			}
		}
		else
		{
			for ( i=0; i<ImageH; i++ )
			{
				for ( j=0; j<ImageW; j++ )
				{				
					maskIdx = (i*m_MapMaskImageStep)+j;
					showIdx = (i*m_ShowImageStep)+(j*3);
					if ( 0 == m_MapMaskImagePtr[maskIdx] )
					{
						m_ShowImagePtr[showIdx] = maskB;
						m_ShowImagePtr[showIdx+1] = maskG;
						m_ShowImagePtr[showIdx+2] = maskR;
					}
					else
					{
						imgIdx = (i*ImageStep)+j;
						m_ShowImagePtr[showIdx] = ImagePtr[imgIdx];
						m_ShowImagePtr[showIdx+1] = ImagePtr[imgIdx];
						m_ShowImagePtr[showIdx+2] = ImagePtr[imgIdx];
					}
				}
			}
		}
	}
	else 
	{	::memset(m_ShowImagePtr, 0x00, sizeof(IMAGE_DATA)*m_ShowImageStep*m_ShowImageH);	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::BuildColorFilterImage(CColorRGBV * rgbvPtr)
{
	if (m_SpecRegionList.size() == 0) {
		CProjectMapMaskWnd::BuildColorFilterImage(rgbvPtr);
		return;
	}
	if ( CheckUseColorFilterMode() == false )
	{	return; }
	if (true == m_EditingRect) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	CColorGroup TempColorGroup;
	if ( NULL == rgbvPtr )
	{	
		m_ColorGroup.UpdateColorGroupUsed();	
		m_ColorGroup.UpdateColorGroupShowColor();
		TempColorGroup = m_ColorGroup;
		return;
	}
	else
	{	
		rgbvPtr->CheckUsed();	
		rgbvPtr->CalcShowColor();
		
		if ( false == m_ShowColorGroup )
		{
			CColorRGBV  rgbv = *rgbvPtr;
			rgbv.SetLogicMode(COLOR_LOGIC_INCLUDE);
			TempColorGroup.AddColorGroupColor(rgbv);
		}
		else
		{	TempColorGroup = m_ColorGroup; }
	}	 
	IMAGE_PTR  ShowPtr = NULL;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;
	const int MapIndex = m_MapIndex;
	if ( ProjectPtr->GetProjectMapPtr(MapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false )
	{	return ; }
	if ( ProjectPtr->CreateProjectMapShowPtr(MapIndex, ShowPtr, false) == false )
	{	return ; }
	const size_t ImageSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if ( NULL==ImagePtr || NULL==m_ShowImagePtr || NULL==ShowPtr ) { return; }	
	unsigned int FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(MapIndex, true);

	CString    str;	
	TREGION4D  WndRgn;
	TREGION4D  ImageRgn;
	RECT       RoiRect={0};	
	MASK_PTR   MaskPtr=NULL;
	IMAGE_SIZE MaskBitCount=8;
	const bool bOpenMP = true;
	IMAGE_SIZE MaskStep=JetAPI::GetBMPImagePixelsPerLine(m_ShowImageW, MaskBitCount, 4);	
	
	double          fnTime=0;
	LARGE_INTEGER   fnEnd;
	LARGE_INTEGER   fnStart;

	JetAPI::SetFuncTimeStart(fnStart);
	ImageRgn = m_SpecRegionList[m_RegionActiveIndex];
	JetAPI::Region4DToRect(ImageRgn, RoiRect, true);	
	JetAPI::BoundaryRect(ImageW, ImageH, RoiRect);
	if ( 24 == BitCount )
	{
		if ( ImageAPI.ColorImageColorFilter(ImageW, ImageH, ImageStep, ImagePtr, TempColorGroup, RoiRect, MaskStep, MaskPtr, true, bOpenMP) == false )
		{	return ; }
	}
	else
	{
		if ( ImageAPI.RGBImageColorFilter(ImageW, ImageH, ImageStep, ImagePtr, ImagePtr, ImagePtr, TempColorGroup, RoiRect, MaskStep, MaskPtr, true) == false )
		{	return ; }
	}
#ifdef _DEBUG
	//str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("ColorMaskWnd.PNG"));
	//ImageAPI.SaveImage(str, ImageW, ImageH, MaskStep, MaskBitCount, MaskPtr, true);
#endif//_DEBUG	
	JetAPI::SetFuncTimeStart(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);//計算函式經過時間
	str.Format(_T("ImageAPI::ColorImageColorFilter Time=%.3f ms"), fnTime);
	AOIDataCollect.SaveUIDrawFuncLog(str);

	MASK_DATA  mask = 0xff;
	IMAGE_DATA mskR=0xFF, mskG=0xFF, mskB=0x00, mskV=0x00, Alpha=0;	
	AOIDataCollect.GetMaskImageColor(FrameUniqueID, mskR, mskG, mskB, mskV, Alpha);

	JetAPI::SetFuncTimeStart(fnStart);	
	if ( ImageStep == m_ShowImageStep )
	{	::memcpy(m_ShowImagePtr, ShowPtr, sizeof(IMAGE_DATA)*ImageSize);	}
	else
	{	ImageAPI.RGBImageToColorImage3(ImageW, ImageH, ImageStep, ShowPtr, ShowPtr, ShowPtr, m_ShowImageStep, m_ShowImagePtr, false);	}
	ImageAPI.ColorImageApplyMask(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowImagePtr, RoiRect, MaskStep, MaskPtr, mask, mskR, mskG, mskB, Alpha);
	
	JetAPI::SetFuncTimeStart(fnEnd);
	fnTime = JetAPI::CalcFuncTimeSpent(fnStart, fnEnd);//計算函式經過時間
	str.Format(_T("ImageAPI::ColorImageApplyMask Time=%.3f ms"), fnTime);
	AOIDataCollect.SaveUIDrawFuncLog(str);

	JetMemory.free_func(MaskPtr);

	DrawShowImage();	
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::SwitchMultiLanguage()
{
	int     WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section = _T("IDD_PROJECT_MAP_SPEC_REGION_WND");
	//---------------------------------------------------------------------------------//
	WndID = IDD_PROJECT_MAP_SPEC_REGION_WND;
	WndKey = _T("IDD_PROJECT_MAP_SPEC_REGION_WND");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = PMM_REGION_ADD_BTN;
	WndKey = _T("PMM_REGION_ADD_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = PMM_REGION_ERASE_BTN;
	WndKey = _T("PMM_REGION_ERASE_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = PMM_REGION_CLEAR_BTN;
	WndKey = _T("PMM_REGION_CLEAR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = PMM_REGION_COLOR_BTN;
	WndKey = _T("PMM_REGION_COLOR_BTN");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = PMM_REGION_INDEX_COMBO;
	WndKey = _T("PMM_REGION_INDEX_COMBO");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = PMM_REGION_CTRL_GROUP;
	WndKey = _T("PMM_REGION_CTRL_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);
	this->SetDlgItemText(WndID, NewLabelText);
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::UpdateCursor(const POINT & pt)
{
	LPTSTR m_CursorID = NULL;
	m_EditRectMode = EDIT_GRID_NULL;
	//if (IMAGE_LBTN_CLICK_EDIT_BOX != m_LBtnClickMode) { return; }
	
	size_t       i = 0;
	BOOL         bPtInRect = FALSE;
	TEditGrid   *EditGridPtr = NULL;
	const size_t EditGridCount = this->m_EditGridList.size();

	for (i = 0; i<EditGridCount; i++)
	{
		EditGridPtr = &(m_EditGridList[i]);
		bPtInRect = ::PtInRect(&(EditGridPtr->sEditGrid), pt);
		if (EDIT_GRID_OUTSIDE == EditGridPtr->sEditMode && FALSE == bPtInRect) { return; }
		if (FALSE == bPtInRect) { continue; }
		switch (EditGridPtr->sEditMode)
		{
		case EDIT_GRID_LEFT:
		case EDIT_GRID_RIGHT:
		case EDIT_GRID_TOP:
		case EDIT_GRID_BOTTOM:
			m_CursorID = IDC_SIZEALL;
			break;
		case EDIT_GRID_CENTER_LEFT:
		case EDIT_GRID_CENTER_RIGH:
			m_CursorID = IDC_SIZEWE;
			break;
		case EDIT_GRID_CENTER_TOP:
		case EDIT_GRID_CENTER_BOTTOM:
			m_CursorID = IDC_SIZENS;
			break;
		case EDIT_GRID_LEFT_TOP:
		case EDIT_GRID_RIGHT_BOTTOM:
			m_CursorID = IDC_SIZENWSE;
			break;
		case EDIT_GRID_RIGHT_TOP:
		case EDIT_GRID_LEFT_BOTTOM:
			m_CursorID = IDC_SIZENESW;
			break;
		}
		m_EditRectMode = EditGridPtr->sEditMode;
		if (NULL != m_CursorID)
		{
			::SetCursor(AfxGetApp()->LoadStandardCursor(m_CursorID));
			return;
		}
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::SetComboMaskIndex(int Index)
{
	CComboBox &Combox = m_MaskIndexComobx;
	JetAPI::SetComboxCurSel(Combox, Index);
	m_MaskIndex = Index;
	CProjectMapMaskWnd::OnSelchangeMaskIndexCombo();
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::DrawPie(HDC hDC, const RECT & Rect)
{
	POINT Pt;
	int HalfSize = (Rect.right - Rect.left) / 2;
	Pt.x = (Rect.left + Rect.right) / 2;
	Pt.y = (Rect.top + Rect.bottom) / 2;
	::Pie(hDC, Pt.x - HalfSize, Pt.y - HalfSize, Pt.x + HalfSize, Pt.y + HalfSize, Pt.x - HalfSize, Pt.y - HalfSize, Pt.x - HalfSize, Pt.y - HalfSize);
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::DrawSpecRng(HDC hDC)
{
	if (NULL == hDC) { return; }
	if (PMM_SHOW_IMAGE_RADIO == m_ShowMode) { return; }
	if (false == RefreshEditGrid()) { return; }

	RECT Rect;
	TREGION4D WndRgn;
	HPEN hPen = ::CreatePen(PS_SOLID, 2, 0x0000FF);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
	TREGION4D SpecRgn = TREGION4D();
	const size_t SpecRegionCount = m_SpecRegionList.size();
	if (0 == SpecRegionCount) { return; }
	size_t i;
	m_SpecTotalRegion = m_SpecRegionList[0];
	for (i = 0; i < SpecRegionCount; i++) {
		SpecRgn = m_SpecRegionList[i];
		ImageAPI.MapImageRgnToWndRgn_DBL(m_ShowImageW, m_ShowImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, SpecRgn, WndRgn);
		JetAPI::Region4DToRect(WndRgn, Rect, true);
		DrawRect(hDC, Rect);
		m_SpecTotalRegion.maxX = MAX(m_SpecTotalRegion.maxX, SpecRgn.maxX);
		m_SpecTotalRegion.maxY = MAX(m_SpecTotalRegion.maxY, SpecRgn.maxY);
		m_SpecTotalRegion.minX = MIN(m_SpecTotalRegion.minX, SpecRgn.minX);
		m_SpecTotalRegion.minY = MIN(m_SpecTotalRegion.minY, SpecRgn.minY);
	}
	::DeleteObject(hPen); hPen = NULL;
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::DrawEditGrid(HDC hDC)
{
	if (PMM_SHOW_IMAGE_RADIO == m_ShowMode) { return; }
	RECT rcGrid = { 0 };
	size_t i = 0;
	const size_t rcCount = m_EditGridList.size();
	HPEN hPen = ::CreatePen(PS_SOLID, 2, 0x0000FF);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));
	HBRUSH hBrush = ::CreateSolidBrush(0x0000FF);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);
	for (i = 0; i<rcCount; i++)
	{
		switch (m_EditGridList[i].sEditMode)
		{
		case EDIT_GRID_CENTER_LEFT:
		case EDIT_GRID_CENTER_RIGH:
		case EDIT_GRID_CENTER_TOP:
		case EDIT_GRID_CENTER_BOTTOM:
		case EDIT_GRID_LEFT_TOP:
		case EDIT_GRID_RIGHT_TOP:
		case EDIT_GRID_LEFT_BOTTOM:
		case EDIT_GRID_RIGHT_BOTTOM:
			//DrawRect(hDC, m_EditGridList[i].sEditGrid);
			DrawPie(hDC, m_EditGridList[i].sEditGrid);
			break;
			//case EDIT_GRID_LEFT:
			//case EDIT_GRID_RIGHT:
			//case EDIT_GRID_TOP:
			//case EDIT_GRID_BOTTOM:
			//	this->DrawRect(hBKDC2, m_EditGridList[i].sEditGrid);
			//	break;
		}
	}
	::SelectObject(hDC, hOldBrush);
	::DeleteObject(hBrush); hBrush = NULL;
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapSpecRegionWnd::CalcSpecStageRegion()
{
	TPOINT2D StageRegionCp;
	double height, width;
	IMAGE_SIZE   ImageW = m_ShowImageW;
	IMAGE_SIZE   ImageH = m_ShowImageH;
	TPOINT2D     ImageRes = m_FrameResolution;
	TREGION4D    ImageStageRgn = m_FrameStageRgn;
	TPOINT2D     ImageStagePos;
	ImageStagePos.x = ImageStageRgn.GetCpX();
	ImageStagePos.y = ImageStageRgn.GetCpY();
	AOIDataCollect.MapCameraRegionToStage(ImageW, ImageH, ImageRes, m_SpecTotalRegion, ImageStagePos, m_StageTotalRegion);
	StageRegionCp.x = m_StageTotalRegion.GetCpX();
	StageRegionCp.y = m_StageTotalRegion.GetCpY();
	height = fabs(m_StageTotalRegion.GetHeight());
	width = fabs(m_StageTotalRegion.GetWidth());
	m_StageTotalRegion.SetRgn(StageRegionCp.x, StageRegionCp.y, width, height);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapSpecRegionWnd::RebuildProjectSpecComponent()
{

	CAOIProject* ProjectPtr = m_ProjectPtr;
	if (NULL == ProjectPtr) { return false; }
	CAOIPanel *PanelPtr = ProjectPtr->GetProjectPanelPtr(0, true);
	if (NULL == PanelPtr) { return false; }
	DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
	CMapCoordinate *MapSTCPtr = PanelPtr->GetPanelMapSTCPtr(DistrictID);
	if (NULL == MapSTCPtr) { return false; }
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent* ComponentPtr = NULL;

	if (ProjectPtr->DeleteProjectComponentType(COMPONENT_TYPE_SPECIAL) == false)
	{	return false; }

	size_t i;
	TPOINT2D StageRegionCp, ComponentStagePos, ComponentCadPos;
	double height, width;
	double ComponentSizeW, ComponentSizeH;
	IMAGE_SIZE   ImageW = m_ShowImageW;
	IMAGE_SIZE   ImageH = m_ShowImageH;
	TPOINT2D     ImageRes = m_FrameResolution;
	TREGION4D    ImageStageRgn = m_FrameStageRgn;
	TREGION4D    SpecRgn, ComponentCadRgn, ComponentStageRgn;
	TPOINT2D     ImageStagePos;
	ImageStagePos.x = ImageStageRgn.GetCpX();
	ImageStagePos.y = ImageStageRgn.GetCpY();

	wchar_t ComponentName[128] = L"";
	wchar_t PartNumber[128] = L"SpecialInspected";
	wchar_t ModelName[128] = L"SpecialInspected";
	_wcsupr(ModelName);

	const size_t SpecRgnCount = m_SpecRegionList.size();
	for (i = 0; i < SpecRgnCount; i++) {
		SpecRgn = m_SpecRegionList[i];
		AOIDataCollect.MapCameraRegionToStage(ImageW, ImageH, ImageRes, SpecRgn, ImageStagePos, ComponentStageRgn);

		ComponentStagePos.x = ComponentStageRgn.GetCpX();
		ComponentStagePos.y = ComponentStageRgn.GetCpY();
		MapSTCPtr->Map2D(ComponentStagePos.x, ComponentStagePos.y, ComponentCadPos.x, ComponentCadPos.y);
		ComponentSizeW = ComponentStageRgn.GetWidth();
		ComponentSizeH = ComponentStageRgn.GetHeight();
		::swprintf(ComponentName, L"%s_%04d", PartNumber, i);

		ComponentPtr = AOIObjManager.CreateComponentObj();
		if (NULL == ComponentPtr) { return false; }

		ComponentPtr->SetComponentName(ComponentName);
		ComponentPtr->SetComponentModelName(ModelName);
		ComponentPtr->SetComponentPartNumber(PartNumber);
		ComponentPtr->SetComponentType(COMPONENT_TYPE_SPECIAL);
		ComponentPtr->SetComponentBodySizeW(ComponentSizeW);
		ComponentPtr->SetComponentBodySizeH(ComponentSizeH);
		ComponentPtr->SetComponentRoiSizeW(ComponentSizeW);
		ComponentPtr->SetComponentRoiSizeH(ComponentSizeH);
		ComponentPtr->SetComponentCadPos(ComponentCadPos);
		ComponentPtr->SetComponentStagePos(ComponentStagePos);
		ComponentPtr->CalcComponentCadCornerPos();
		ComponentPtr->LayoutComponentStageCornerPos();
		ModelPtr = ComponentPtr->GetComponentModelPtr();
		ModelPtr->SetModelType(MODEL_TYPE_OTHERS);
		ComponentPtr->UpdateComponentParamToModel(false);

		ProjectPtr->AddProjectComponentPtr(ComponentPtr, false);
		PanelPtr->AddPanelComponentPtr(ComponentPtr);
		//m_SpecComponentList.push_back(ComponentPtr);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapSpecRegionWnd::LoadProjectSpecComponent()
{

	size_t         i = 0;
	CAOIProject* ProjectPtr = m_ProjectPtr;
	if (NULL == ProjectPtr) { return false; }
	TPOINT2D     StagePos, ImageStagePos;
	TREGION4D SpecRgn = TREGION4D();
	IMAGE_SIZE   ImageW = m_ShowImageW;
	IMAGE_SIZE   ImageH = m_ShowImageH;
	TPOINT2D     ImageRes = m_FrameResolution;
	TREGION4D    ImageStageRgn = m_FrameStageRgn;
	ImageStagePos.x = ImageStageRgn.GetCpX();
	ImageStagePos.y = ImageStageRgn.GetCpY();
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];

	CAOIComponent *ComponentPtr = NULL;
	COMPONENT_TYPE ComponentType = COMPONENT_TYPE_SPECIAL;
	const size_t   ComponentCount = ProjectPtr->GetProjectComponentCount();
	for (i = 0; i<ComponentCount; i++)
	{
		ComponentPtr = ProjectPtr->GetProjectComponentPtr(i,false);
		if (NULL == ComponentPtr) { continue; }
		if (ComponentPtr->GetComponentDeleted() == true) { continue; }
		if (ComponentPtr->GetComponentType() != ComponentType) { continue; }
		StagePos.x = ComponentPtr->GetComponentStagePosX();
		StagePos.y = ComponentPtr->GetComponentStagePosY();
		if (StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX) { continue; }
		if (StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY) { continue; }
		ComponentPtr->GetComponentBodyStageCornerPos(StgCornerPos);
		//AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);
		JetAPI::CornerPtToRegion(ImgCornerPos, SpecRgn);
		//m_SpecRegionList.push_back(SpecRgn);
		ExecAddRegion(SpecRgn);
	}
}
//-------------------------------------------------------------------------------------//
bool CProjectMapSpecRegionWnd::RefreshEditGrid()
{
	const size_t SpecRegionCount = m_SpecRegionList.size();
	if (SpecRegionCount == 0) { return true; }
	size_t i;
	const int GridW = m_EditGridW;
	const int GridH = m_EditGridH;

	RECT WndRect;
	TEditGrid EditGrid;
	TREGION4D SpecRgn = TREGION4D();
	TREGION4D WndRgn = TREGION4D();
	m_EditGridList.clear();
	
	//for (i = 0; i < SpecRegionCount; i++) {
		//if (m_RegionActiveIndex) = 
	SpecRgn = m_SpecRegionList[m_RegionActiveIndex];
	ImageAPI.MapImageRgnToWndRgn_DBL(m_ShowImageW, m_ShowImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, SpecRgn, WndRgn);
	JetAPI::Region4DToRect(WndRgn, WndRect, true);

	POINT CP = { 0 };
	RECT  Rect = { 0 };
	RECT  RectMax = WndRect;
	RECT  RectMin = WndRect;
	CP.x = (WndRect.left + WndRect.right) / 2;
	CP.y = (WndRect.top + WndRect.bottom) / 2;

	::InflateRect(&RectMax, GridW, GridH);
	::InflateRect(&RectMin, -GridW, -GridH);
	EditGrid.sEditMode = EDIT_GRID_OUTSIDE;
	EditGrid.sEditGrid = RectMax;
	m_EditGridList.push_back(EditGrid);

	EditGrid.sEditMode = EDIT_GRID_INSIDE;
	EditGrid.sEditGrid = RectMin;
	m_EditGridList.push_back(EditGrid);

	//C-Left
	Rect.left = RectMax.left;
	Rect.right = RectMin.left;
	Rect.top = CP.y - GridH;
	Rect.bottom = CP.y + GridH;
	EditGrid.sEditMode = EDIT_GRID_CENTER_LEFT;
	EditGrid.sEditGrid = Rect;
	m_EditGridList.push_back(EditGrid);

	//C-Right
	Rect.left = RectMin.right;
	Rect.right = RectMax.right;
	Rect.top = CP.y - GridH;
	Rect.bottom = CP.y + GridH;
	EditGrid.sEditMode = EDIT_GRID_CENTER_RIGH;
	EditGrid.sEditGrid = Rect;
	m_EditGridList.push_back(EditGrid);

	//C-Top
	Rect.top = RectMax.top;
	Rect.bottom = RectMin.top;
	Rect.left = CP.x - GridW;
	Rect.right = CP.x + GridW;
	EditGrid.sEditMode = EDIT_GRID_CENTER_TOP;
	EditGrid.sEditGrid = Rect;
	m_EditGridList.push_back(EditGrid);

	//Bottom
	Rect.top = RectMin.bottom;
	Rect.bottom = RectMax.bottom;
	Rect.left = CP.x - GridW;
	Rect.right = CP.x + GridW;
	EditGrid.sEditMode = EDIT_GRID_CENTER_BOTTOM;
	EditGrid.sEditGrid = Rect;
	m_EditGridList.push_back(EditGrid);


	//left
	Rect.left = RectMax.left;
	Rect.right = RectMin.left;
	Rect.top = RectMin.top;
	Rect.bottom = RectMin.bottom;
	EditGrid.sEditMode = EDIT_GRID_LEFT;
	EditGrid.sEditGrid = Rect;
	m_EditGridList.push_back(EditGrid);

	//right
	Rect.left = RectMin.right;
	Rect.right = RectMax.right;
	Rect.top = RectMin.top;
	Rect.bottom = RectMin.bottom;
	EditGrid.sEditMode = EDIT_GRID_RIGHT;
	EditGrid.sEditGrid = Rect;
	m_EditGridList.push_back(EditGrid);

	//Top
	Rect.top = RectMax.top;
	Rect.bottom = RectMin.top;
	Rect.left = RectMin.left;
	Rect.right = RectMin.right;
	EditGrid.sEditMode = EDIT_GRID_TOP;
	EditGrid.sEditGrid = Rect;
	m_EditGridList.push_back(EditGrid);

	//Bottom
	Rect.top = RectMin.bottom;
	Rect.bottom = RectMax.bottom;
	Rect.left = RectMin.left;
	Rect.right = RectMin.right;
	EditGrid.sEditMode = EDIT_GRID_BOTTOM;
	EditGrid.sEditGrid = Rect;
	m_EditGridList.push_back(EditGrid);

	//left-Top
	Rect.left = RectMax.left;
	Rect.right = RectMin.left;
	Rect.top = RectMax.top;
	Rect.bottom = RectMin.top;
	EditGrid.sEditMode = EDIT_GRID_LEFT_TOP;
	EditGrid.sEditGrid = Rect;
	m_EditGridList.push_back(EditGrid);

	//right-Top
	Rect.left = RectMin.right;
	Rect.right = RectMax.right;
	Rect.top = RectMax.top;
	Rect.bottom = RectMin.top;
	EditGrid.sEditMode = EDIT_GRID_RIGHT_TOP;
	EditGrid.sEditGrid = Rect;
	m_EditGridList.push_back(EditGrid);

	//left-Bottom
	Rect.left = RectMax.left;
	Rect.right = RectMin.left;
	Rect.top = RectMin.bottom;
	Rect.bottom = RectMax.bottom;
	EditGrid.sEditMode = EDIT_GRID_LEFT_BOTTOM;
	EditGrid.sEditGrid = Rect;
	m_EditGridList.push_back(EditGrid);

	//right-Bottom
	Rect.left = RectMin.right;
	Rect.right = RectMax.right;
	Rect.top = RectMin.bottom;
	Rect.bottom = RectMax.bottom;
	EditGrid.sEditMode = EDIT_GRID_RIGHT_BOTTOM;
	EditGrid.sEditGrid = Rect;
	m_EditGridList.push_back(EditGrid);
	//}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapSpecRegionWnd::ExecEditRect(const POINT pt)
{
	//SpecRegionCount
	TPOINT2D ImagePt, ImageLastPt;
	TREGION4D &EditRegion = m_SpecRegionList[m_RegionActiveIndex];
	ImageAPI.MapWndPtToImagePt_DBL(m_ShowImageW, m_ShowImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, pt, ImagePt);
	ImageAPI.MapWndPtToImagePt_DBL(m_ShowImageW, m_ShowImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_LastPt, ImageLastPt);
	TPOINT2D dp;
	dp.x = ImagePt.x - ImageLastPt.x;
	dp.y = ImagePt.y - ImageLastPt.y;
	switch (m_EditRectMode)
	{
	case EDIT_GRID_INSIDE:
		break;
	case EDIT_GRID_LEFT:
	case EDIT_GRID_RIGHT:
	case EDIT_GRID_TOP:
	case EDIT_GRID_BOTTOM:
		EditRegion.Move(dp.x, dp.y);
		break;
	case EDIT_GRID_CENTER_LEFT:
		EditRegion.minX += dp.x;
		break;
	case EDIT_GRID_CENTER_RIGH:
		EditRegion.maxX += dp.x;
		break;
	case EDIT_GRID_CENTER_TOP:
		EditRegion.minY += dp.y;
		break;
	case EDIT_GRID_CENTER_BOTTOM:
		EditRegion.maxY += dp.y;
		break;
	case EDIT_GRID_LEFT_TOP:
		EditRegion.minX += dp.x;
		EditRegion.minY += dp.y;
		break;
	case EDIT_GRID_RIGHT_TOP:
		EditRegion.maxX += dp.x;
		EditRegion.minY += dp.y;
		break;
	case EDIT_GRID_LEFT_BOTTOM:
		EditRegion.minX += dp.x;
		EditRegion.maxY += dp.y;
		break;
	case EDIT_GRID_RIGHT_BOTTOM:
		EditRegion.maxX += dp.x;
		EditRegion.maxY += dp.y;
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapSpecRegionWnd::ModifyMaskEditImageRect()
{
	return true;
	//if ( NULL == m_MapMaskImagePtr ) { return false; }
	if ( NULL == m_MapMaskImagePtr1) { return false; }
	const int nAlign = 4;
	const IMAGE_SIZE ImageW = m_ProjectMapW;
	const IMAGE_SIZE ImageH = m_ProjectMapH;
	const IMAGE_SIZE BitCount = 8;
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, nAlign);
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	::memset(m_MapMaskImagePtr1, 0x00, sizeof(IMAGE_DATA)*BufferSize);

	IMAGE_DATA mask = 0xFF;
	RECT Rect={0};
	size_t  i = 0, j = 0, k = 0, idx = 0;
	TREGION4D SpecRgn = TREGION4D();
	for (i = 0; i < m_SpecRegionList.size(); i++) {
		SpecRgn = m_SpecRegionList[i];
		JetAPI::Region4DToRect(SpecRgn, Rect, true);
		JetAPI::BoundaryRect(m_MapMaskImageW, m_MapMaskImageH, Rect);
		for (j = Rect.top; j<Rect.bottom; j++)
		{
			idx = j*m_MapMaskImageStep + Rect.left;
			for (k = Rect.left; k<Rect.right; k++)
			{
				m_MapMaskImagePtr1[idx++] = mask;
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapSpecRegionWnd::ExecAddRegion(TREGION4D NewRegion)
{
	m_SpecRegionList.push_back(NewRegion);

	const size_t cnt = m_SpecRegionList.size() - 1;
	CComboBox &Combox = m_RegionIndexComobx;
	CString ItemName;
	ItemName.Format(L"Region: %d", m_SpecRegionList.size());
	Combox.InsertString(cnt, ItemName);
	Combox.SetItemData(cnt, cnt);
	JetAPI::SetComboxCurSel(Combox, cnt);
	m_RegionActiveIndex = cnt;

	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapSpecRegionWnd::ModifyColorMaskSpecRegion()
{
	//const bool bUseColorFilter = CheckUseColorFilterMode();
	//if (false == bUseColorFilter) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if (NULL == ProjectPtr) { return false; }
	m_MapMaskImagePtr = m_SpecRegionMaskImagePtr;
	JetAPI::SetComboxCurSel(m_MaskIndexComobx,5);
	if (NULL == m_MapMaskImagePtr) { return false; }

	IMAGE_DATA mask = 0xff;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;
	const unsigned int MapIndex = m_ColorGroup.GetColorGroupFrameIndex();
	if (ProjectPtr->GetProjectMapPtr(MapIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr) == false)
	{	return false;	}
	const size_t ImageSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	if (NULL == ImagePtr || NULL == m_ShowImagePtr) { return false; }

	CString    str;
	TREGION4D  WndRgn;
	TREGION4D  ImageRgn;
	RECT       RoiRect = { 0 };
	MASK_PTR   MaskPtr = NULL;
	IMAGE_SIZE MaskBitCount = 8;
	const bool bOpenMP = true;
	const bool bUseRoiRect = GetUseRoiRectChk();
	IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(m_ShowImageW, MaskBitCount, 4);
	TREGION4D SpecRegion = m_SpecRegionList[m_RegionActiveIndex];
	RECT SpecRect = RECT();
	JetAPI::Region4DToRect(SpecRegion, SpecRect, false);

	if (true == bUseRoiRect)
	{
		JetAPI::Region4DToRect(m_ImageRgnRoi, RoiRect, true);
		JetAPI::BoundaryRect(ImageW, ImageH, RoiRect);
	}
	else
	{
		JetAPI::SizeToRect(ImageW, ImageH, RoiRect);
	}
	if (24 == BitCount)
	{
		if (ImageAPI.ColorImageColorFilter(ImageW, ImageH, ImageStep, ImagePtr, m_ColorGroup, RoiRect, MaskStep, MaskPtr, true, bOpenMP) == false)
		{	return false;	}
	}
	else
	{
		if (ImageAPI.RGBImageColorFilter(ImageW, ImageH, ImageStep, ImagePtr, ImagePtr, ImagePtr, m_ColorGroup, RoiRect, MaskStep, MaskPtr, true) == false)
		{	return false;	}
	}
#ifdef _DEBUG
	//str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("ColorMaskFull.PNG"));
	//ImageAPI.SaveImage(str, ImageW, ImageH, MaskStep, MaskBitCount, MaskPtr, true);
#endif//_DEBUG

	if (ImageH != m_MapMaskImageH || MaskStep != m_MapMaskImageStep)
	{
		JetMemory.free_func(MaskPtr);
		return false;
	}

	size_t i = 0, j = 0, k = 0;
	for (i = RoiRect.top; i<RoiRect.bottom; i++)
	{
		for (j = RoiRect.left; j<RoiRect.right; j++)
		{
			TPOINT2D pt = TPOINT2D(j, i);
			if (false == JetAPI::PtInRect(pt, SpecRect)) { 
				m_MapMaskImagePtr[k] = 0xff - mask;
				continue; 
			}

			k = (i*MaskStep) + j;
			if (0 == MaskPtr[k]) { 
				m_MapMaskImagePtr[k] = 0xff - mask;
				continue; 
			}
			m_MapMaskImagePtr[k] = mask;
		}
	}
	JetMemory.free_func(MaskPtr);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CProjectMapSpecRegionWnd::GetSpecTotalStageRegion(TREGION4D &StageRegion)
{
	
	StageRegion = m_StageTotalRegion;
	return true;
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::DoDataExchange(CDataExchange * pDX)
{
	CProjectMapMaskWnd::DoDataExchange(pDX);
	//
	DDX_Control(pDX, PMM_REGION_INDEX_COMBO, m_RegionIndexComobx);
	// 2. 處理新增加的控制項
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	POINT pt = point;
	CWnd::MapWindowPoints(&m_ImageWnd, &pt, 1);
	if (this != GetCapture())
	{
		UpdateCursor(pt);
	}
	if (nFlags&MK_LBUTTON)
	{
		if (::PtInRect(&m_ImageWndRect, pt) == TRUE)
		{
			if (EDIT_GRID_NULL != m_EditRectMode)
			{
				ExecEditRect(pt);
				//ModifyMaskEditImageRect();
			}
		}
	}
	CProjectMapMaskWnd::OnMouseMove(nFlags, point);
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CProjectMapMaskWnd::OnLButtonUp(nFlags, point);

	const bool bUseColorFilter = CheckUseColorFilterMode();
	if (m_EditingRect == false) { return; }
	if (PMM_SHOW_COMBINED_RADIO == m_ShowMode) {
		BuildShowImage();
		DrawShowImage();
	}
	if (PMM_SHOW_MASK_RADIO == m_ShowMode) {
		//BuildShowImage();
		DrawShowImage();
	}
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	POINT pt = point;
	CProjectMapMaskWnd::OnLButtonDown(nFlags, point);
	if (EDIT_GRID_NULL != m_EditRectMode && EDIT_GRID_INSIDE != m_EditRectMode) { m_EditingRect = true; }
	else { m_EditingRect = false; }
	return;
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::OnRegionAddBtn()
{
	ExecAddRegion(m_ImageRgnOuter);
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::OnRegionEraseBtn()
{
	const size_t SpecRegionCount = m_SpecRegionList.size();
	if (SpecRegionCount == 0) { return; }
	if (m_RegionActiveIndex > SpecRegionCount) { return; }
	//SetComboMaskIndex(1);
	m_SpecRegionList.erase(m_SpecRegionList.begin() + m_RegionActiveIndex);

	CComboBox &Combox = m_RegionIndexComobx;
	JetAPI::ClearCombox(Combox);
	CString ItemName;
	for (size_t i = 0; i < SpecRegionCount; i++) {
		ItemName.Format(L"Region: %d", i+1);
		Combox.InsertString(i, ItemName);
		Combox.SetItemData(i, i);
		JetAPI::SetComboxCurSel(Combox, i);
	}
	m_RegionActiveIndex = 0;
	JetAPI::SetComboxCurSel(Combox, m_RegionActiveIndex);

	if (m_SpecRegionList.size() == 0) {
		m_MapMaskImagePtr = m_MapMaskImagePtr0;
		JetAPI::SetComboxCurSel(m_MaskIndexComobx, 0);
	}

	const IMAGE_SIZE ImageW = m_ProjectMapW;
	const IMAGE_SIZE ImageH = m_ProjectMapH;
	const IMAGE_SIZE BitCount = 8;
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	::memset(m_SpecRegionMaskImagePtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);

	m_EditGridList.clear();
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::OnRegionClearBtn()
{
	const size_t SpecRegionCount = m_SpecRegionList.size();
	if (SpecRegionCount == 0) { return; }
	const IMAGE_SIZE ImageW = m_ProjectMapW;
	const IMAGE_SIZE ImageH = m_ProjectMapH;
	const IMAGE_SIZE BitCount = 8;
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	::memset(m_SpecRegionMaskImagePtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);

	m_MapMaskImagePtr = m_MapMaskImagePtr0;
	JetAPI::SetComboxCurSel(m_MaskIndexComobx, 0);

	m_RegionActiveIndex = 0;
	JetAPI::ClearCombox(m_RegionIndexComobx);
	m_SpecRegionList.clear();
	m_EditGridList.clear();
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::OnRegionColorBtn()
{
	const size_t SpecRegionCount = m_SpecRegionList.size();
	if (SpecRegionCount == 0) { 
		JetAPI::ShowMessageBox(_T("Please add region first."));
		return;
	}
	const IMAGE_SIZE ImageW = m_ProjectMapW;
	const IMAGE_SIZE ImageH = m_ProjectMapH;
	const IMAGE_SIZE BitCount = 8;
	const IMAGE_SIZE ImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, BitCount, 4);
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	::memset(m_SpecRegionMaskImagePtr, 0x00, sizeof(IMAGE_DATA)*BufferSize);

	const size_t ColorIndex = PROJECT_COLOR_ID_BOARD_BEGIN;
	CColorGroup *ColorGroupPtr = m_ProjectPtr->GetProjectColorGroupPtr(ColorIndex, true);
	if (NULL == ColorGroupPtr) { return ; }
	m_ColorGroup = *ColorGroupPtr;
	ModifyColorMaskSpecRegion();
	BuildShowImage();
	DrawShowImage();
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::OnSelchangeRegionIndexCombo()
{
	
	m_RegionActiveIndex = JetAPI::GetComboxCurSelData(m_RegionIndexComobx);
	RefreshEditGrid();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::OnSelchangeMaskIndexCombo()
{
	const int maskIndex = JetAPI::GetComboxCurSelData(m_MaskIndexComobx);
	switch (maskIndex)
	{
	case 1:
		m_MapMaskImagePtr = m_MapMaskImagePtr1;
		break;
	case 2:
		m_MapMaskImagePtr = m_MapMaskImagePtr2;
		break;
	case 3:
		m_MapMaskImagePtr = m_MapMaskImagePtr3;
		break;
	case 4:
		m_MapMaskImagePtr = m_MapMaskImagePtr4;
		break;
	case 5:
		m_MapMaskImagePtr = m_SpecRegionMaskImagePtr;
		break;
	default:
		m_MapMaskImagePtr = m_MapMaskImagePtr0;
		break;
	}
	BuildShowImage();
	DrawShowImage();
	RedrewWnd();
}
//-------------------------------------------------------------------------------------//
void CProjectMapSpecRegionWnd::OnOK()
{
	CalcSpecStageRegion();
	RebuildProjectSpecComponent();
	CProjectMapMaskWnd::OnOK();
}
//-------------------------------------------------------------------------------------//