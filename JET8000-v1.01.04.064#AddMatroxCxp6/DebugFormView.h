#if !defined(AFX_DEBUGFORMVIEW_H__78C68EF6_3C80_44F9_8866_9F35C04A7D20__INCLUDED_)
#define AFX_DEBUGFORMVIEW_H__78C68EF6_3C80_44F9_8866_9F35C04A7D20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DebugFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDebugFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDebugFormView : public CFormView
{
protected:
	CDebugFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDebugFormView)

// Form Data
public:
	//{{AFX_DATA(CDebugFormView)
	enum { IDD = IDD_DEBUG_FORMVIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDebugFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	bool                       TestImageScale();//影像縮放
	bool                       TestSTLVector();//標準樣板類別的Vector	
	bool                       TestSinePattern();
	bool                       TestJSON();//測試JSON
	bool                       TestJSON_02();//測試JSON
	bool                       TestMatch();//影像匹配
	bool                       TestBarcode();//條碼測試
	bool                       TestMoveFolder();//檔案移動		
	void                       TestSaveLoadUnicodeTextFile();//萬國語言存檔讀檔	
	bool                       TestColorRatio();

	void                       TestMoments();
	void                       TestMinRectArea();	
	void                       TestBlobContour();
	void                       TestKnnClassify();
	void                       TestKnnClassify_02();
	void                       TestOpenCV_SVM();
	void                       TestOpenCV_SVM_02();
	void                       TestDeSkew();//影像反轉	
	void                       TestAdaBoost();//測試AdaBoost-聯集分類器	
	void                       TestAdaBoost_02();//測試AdaBoost-聯集分類器	
	void                       TestAdaBoost_03();//測試AdaBoost-聯集分類器	
	void                       TestCopyFolder();
	void                       TestDeleteFolder();
	int                        WeakClassify1(const POINT &Pt);
	int                        WeakClassify2(const POINT &Pt);
	int                        WeakClassify3(const POINT &Pt);		
	void                       InitAdaBoostDList(std::vector<double> &DList);
	void                       InitAdaBoostWList(std::vector<double> &WList);
	void                       InitAdaBoostDataSet(std::vector<POINT> &vaList, std::vector<int> &LabList);
	bool                       GetMinErrorClassifyID(const std::vector<std::vector<bool>> &TestList, const std::vector<double> &DList, int &Index, double &Error);	//取得最小誤差的分類器編號
	bool                       CalcNewDList(int ClasID, const std::vector<std::vector<bool>> &TestList, double Err, std::vector<double> &DList);//計算新的D值
	bool                       CalcClassifyTestList(size_t ClassifyCount, const std::vector<POINT> &vaList, const std::vector<int> &LabList, std::vector<std::vector<bool>> &TestList);//計算每個分類器的結果
	bool                       CalcTotalClassifyError(const std::vector<double> &ClasWList, const std::vector<POINT> &vaList, const std::vector<int> &LabList, double &Error);//計算整體的錯誤率
	bool                       TestSortList();
	bool                       TestRGBConvert();
	bool                       TestSleep();
	bool                       TestFloatImage();
	bool                       TestBayerImage();
	bool                       TestSmoothImage();
	bool                       TestPyramidImage();//圖像金字塔
	bool                       TestPyramidImageMerge();//圖像金字塔
	bool                       TestBlendImageMerge();//混色影像合併
	bool                       TestOpenCVMat();//cv::Mat不是CvMat
	bool                       TestOpenCVMatList();//cv::Mat不是CvMat	
	bool                       TestRoateImage();
	bool                       TestRoateImage2();
	bool                       TestExtractRoi();
	bool                       TestMorphImage();
	bool                       TestErodeDilateImage();
	bool                       TestGaussianImage();
	bool                       TestFlipImage();
	bool                       TestTransposeImage();
	bool                       TestSmartPointer();
	bool                       TestSmartPointer2(std::shared_ptr<unsigned char> Ptr);
	bool                       TestTimeStruct();
	bool                       TestProjectSaveServerLibrary();
	bool                       TestRemoteParamWnd();
	bool                       TestSoftwareNoResponse();
	bool                       TestDiskDrive();
	bool                       TestSSEInstructionSet();//SSE 指令集
	bool                       TestPhaseGamma();//測試相位Gamma
	bool                       TestPhaseGamma2();//測試相位Gamma
	bool                       TestNonLocalMean();//測試Non-Local Mean
	bool                       TestCorrectTShape();//測試校正-T形
	bool                       TestCameraCalibration();//測試相機校正
	bool                       TestJetCameraCalibration();//測試相機校正
	bool                       TestStereoCameraCalibration();//測試雙相機校正	
	void                       TestArray(cv::InputArrayOfArrays objectPoints, cv::InputArrayOfArrays imagePoints);
	bool                       TestHistogram();//測試直方圖
	bool                       TestBackProject();//測試反投影
	bool                       TestOpenCVContour();//測試OpenCV輪廓
	bool                       ShowOpenCVMatrix(cv::Mat &Mat);
	bool                       SetOpenCVMatrix(int nRows, int nCols, double *Ptr);  	
	bool                       TestHeightFactorMappingFunc();//測試高度比例的映射函式	
	bool                       TestOpenCVImageMoment();//測試OpenCV力矩	
	bool                       TestRawImagePixelFocus();//測試原圖的對焦影像
	bool                       TestCombine2PixelFocus();//測試合併2個對焦影像
	bool                       TestFloatImageForJoe();
	bool                       TestCreateFolder();
	bool                       TestFilterPhase_Joe();
// Implementation
protected:
	virtual ~CDebugFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDebugFormView)
	afx_msg void OnDestroy();
	afx_msg void OnDebugBtn();
	afx_msg void OnNewProjectBtn();
	afx_msg void OnImageDebugBtn();
	afx_msg void OnView3dDebugBtn();
	afx_msg void OnTiDlpDebugBtn();
	afx_msg void OnSaveBtn();
	afx_msg void OnCoordinateBtn();
	afx_msg void OnImageMaskBtn();
	afx_msg void OnAllPhaseBtn();
	afx_msg void OnModelBtn();
	afx_msg void OnBarcodeDeviceBtn();
	afx_msg void OnBarcodeHandheldBtn();
	afx_msg void OnProjectColorBtn();
	afx_msg void OnProjectMapMaskBtn();
	afx_msg void OnNewPanelBtn();
	afx_msg void OnFullMapComponentBtn();
	afx_msg void OnSaveSpcFileBtn();
	afx_msg void OnComponentListWnd();
	afx_msg void OnLoadCadxyBtn();
	afx_msg void OnProjectCompareBtn();
	afx_msg void OnAlgImageCompareBtn();
	afx_msg void OnSaveMemoryBtn();
	afx_msg void OnBuildXYCaliBtn();
	afx_msg void OnBarcodeRecognizeBtn();
	afx_msg void OnFieldConfigBtn();
	afx_msg void OnSocketClientBtn();
	afx_msg void OnLoadRepairFileBtn();
	afx_msg void OnVerifyBinFileBtn();
	afx_msg void OnSaveLocationFileBtn();
	afx_msg void OnCombineImageBtn();
	afx_msg void OnDivideDistrictBtn();
	afx_msg void OnBoardConfigBtn();
	afx_msg void OnComponentConfigBtn();
	afx_msg void OnProjectCodeListBtn();
	afx_msg void OnOtherDebugBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEBUGFORMVIEW_H__78C68EF6_3C80_44F9_8866_9F35C04A7D20__INCLUDED_)
