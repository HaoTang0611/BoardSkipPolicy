#if !defined(AFX_EDITWNDVIEW_H__DC3D3F6B_2939_4188_BA90_8699846C7395__INCLUDED_)
#define AFX_EDITWNDVIEW_H__DC3D3F6B_2939_4188_BA90_8699846C7395__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditWndView.h : header file
//
//-------------------------------------------------------------------------------------//
#include "AOIModel.h"
#include "PageSplitterWnd.h"
#include "WndAlgPropertyDef.h"
#include "JETPropertyGridCtrl.h"
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditWndView window
//-------------------------------------------------------------------------------------//
class CEditWndView : public CWnd
{
// Construction
public:
	CEditWndView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditWndView)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL     Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
	virtual ~CEditWndView();

public:
	//---------------------------------------------------------------------------------//
	void SetVSDotNetLook(BOOL bSet);
	//---------------------------------------------------------------------------------//
protected:	
	//---------------------------------------------------------------------------------//	
	CAOIModel                 *m_ModelPtr;
	CAOIProject               *m_ProjectPtr;
	CAOIWnd                   *m_ActiveWndPtr;
	int                        m_ActClassID;
	//---------------------------------------------------------------------------------//
	CFont                      m_fntPropList;
	CComboBox                  m_wndClassCombo;
	CButton                    m_wndClassCopyBtn;
	CButton                    m_wndClassClearBtn;
	CPropertiesToolBar         m_wndToolBar;
	CJETPropertyGridCtrl       m_wndGroupList;
	CJETPropertyGridCtrl       m_wndWndList;
	CJETPropertyGridCtrl       m_wndWndParam;
	CPageSplitterWnd           m_wndSplitter;
	COLORREF                   m_clrOK;
	COLORREF                   m_clrNG;
	COLORREF                   m_clrWarnning;
	COLORREF                   m_clrException;
	COLORREF                   m_clrSkip;
	COLORREF                   m_clrBypass;
	COLORREF                   m_clrUnTest;	
	//---------------------------------------------------------------------------------//
	TPropGridParam             m_PropGridParam;
	TPropGridParam&            GetPropGridParam() { return m_PropGridParam; }
	//---------------------------------------------------------------------------------//
	void                       InitPropList();
	void                       AdjustLayout();
	void                       SetPropListFont();
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	void                       PostMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam);
	void                       SendMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam);
	//---------------------------------------------------------------------------------//	
	COLORREF                   GetResultColor(RESULT_ID ResultID, RESULT_ID LogicResultID);
	//---------------------------------------------------------------------------------//				
	CAOIModel*                 GetModelPtr();
	void                       CloseProject();	
	CAOIProject*               GetActiveProject();
	//---------------------------------------------------------------------------------//
	void                       SetActiveWndPtr(CAOIWnd *WndPtr);
	bool                       CheckWndClassIDVisible(CAOIWnd *WndPtr);
	//---------------------------------------------------------------------------------//
	bool                       ResetClassCombox();
	bool                       UpdateClassComboxID();
	//---------------------------------------------------------------------------------//
	bool                       ShowWndGroupItem();
	bool                       BuildWndGroupList();
	bool                       BuildWndGroupListKernel(int ActClassID);	
	bool                       ClearWndGroupList();
	bool                       UpdateWndGrupListSelected();
	bool                       BuildWndGroupList_ModelExtendRange(CJETPropertyGridCtrl *pCtrl, CJETPropertyGridProperty *pGroup, CAOIModel *ModelPtr);

	bool                       ExecWndGroupListLClicked(CJETPropertyGridProperty *pProp);
	bool                       ExecWndGroupListRClicked(CJETPropertyGridProperty *pProp);
	bool                       ExecWndGroupListLDbClick(CJETPropertyGridProperty *pProp);
	bool                       ExecWndGroupListRDbClick(CJETPropertyGridProperty *pProp);
	bool                       ExecWndGroupListChanged(CJETPropertyGridProperty *pProp);
	bool                       ExecWndGroupListSelChanged(CJETPropertyGridProperty *pProp);
	//---------------------------------------------------------------------------------//
	bool                       BuildWndObjList(CAOIModel *ModelPtr, int WndGroupID);
	bool                       ClearWndObjList();
	bool                       UpdateWndObjListSelected();
	bool                       ExecWndObjListLClicked(CJETPropertyGridProperty *pProp);
	bool                       ExecWndObjListRClicked(CJETPropertyGridProperty *pProp);
	bool                       ExecWndObjListLDbClick(CJETPropertyGridProperty *pProp);
	bool                       ExecWndObjListRDbClick(CJETPropertyGridProperty *pProp);
	bool                       ExecWndObjListChanged(CJETPropertyGridProperty *pProp);
	bool                       ExecWndObjListSelChanged(CJETPropertyGridProperty *pProp);
	//---------------------------------------------------------------------------------//
	bool                       RefreshWndPtr();
	bool                       BuildWndParamList();		
	CString                    FormWndParamListCategoryName(CAOIWnd *WndPtr);	
	bool                       BuildWndParamList_AIModel(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_RegionLink(CJETPropertyGridProperty *pGroup, CAOIWnd *WndPtr, bool bXYRatio);
	bool                       BuildWndParamList_WndDefectID(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr);	
	bool                       BuildWndParamList_AlgFrameIndex(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_MaskFrameIndex(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr);	
	bool                       BuildWndParamList_MatchOffset(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr, bool bX, bool bY, bool bA, bool bS);
	bool                       BuildWndParamList_ModelMaskFlag(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr);	
	bool                       BuildWndParamList_ScaleRatio(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_ExtendRange(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_PixelCompare(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr);	
	bool                       BuildWndParamList_LogicParam(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_BaseValue(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_SaveDefectImage(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_FollowMode(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr);	
	bool                       BuildWndParamList_ResultText(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr);	
	bool                       BuildWndParamList_SyncMoveMode(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_ConstrainMode(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr);	
	bool                       BuildWndParamList_ClassID(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_BoxShape(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr);		
	bool                       BuildWndParamList_DefectGroupID(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr);

	bool                       BuildWndParamList_BrightRatio(CAOIProject *Project, CAOIWnd *WndPtr);	
	bool                       BuildWndParamList_OuterShort(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_BlobCount(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_BodyTilt(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_BarcodeRecognize(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_ObjectMeasure(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_ColorCode(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_ModelMatch(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_ImageMatch(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_CharVerify(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_FdMatch(CAOIProject *Project, CAOIWnd *WndPtr);	
	bool                       BuildWndParamList_EdgeSearch(CAOIProject *Project, CAOIWnd *WndPtr);	
	bool                       BuildWndParamList_ShapeVerify(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_AngleMeasure(CAOIProject *Project, CAOIWnd *WndPtr);	
	bool					   BuildWndParamList_ROICompare(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr);
	bool					   BuildWndParamList_WidthRatio(CAOIProject *Project, CAOIWnd *WndPtr);
	bool					   BuildWndParamList_ResinHight(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_WireWidth(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_PixelCompare(CAOIProject *Project, CAOIWnd *WndPtr);	
	bool                       BuildWndParamList_SolderWetting(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_MeasureBlackGlue(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_MeasureFluxArea(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_MeasureCpuPin(CAOIProject *Project, CAOIWnd *WndPtr);	
	bool                       BuildWndParamList_MeasureSIP(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_MeasureConnector(CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_MeasureConnector_Pin(CAOIProject *Project, CAOIWnd *WndPtr);

	bool                       BuildWndParamList_WndRoi(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_MaskBox(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_GroupCompare(CJETPropertyGridCtrl &GridCtrl, CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_AdvanceGeneral(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr);
	bool                       BuildWndParamList_AdvancePatMatch(CJETPropertyGridProperty *pGroup, CAOIProject *Project, CAOIWnd *WndPtr);

	bool                       ClearWndParamList();
	bool                       ExecWndParamListLClicked(CJETPropertyGridProperty *pProp);
	bool                       ExecWndParamListRClicked(CJETPropertyGridProperty *pProp);
	bool                       ExecWndParamListLDbClick(CJETPropertyGridProperty *pProp);
	bool                       ExecWndParamListRDbClick(CJETPropertyGridProperty *pProp);
	bool                       ExecWndParamListChanged(CJETPropertyGridProperty *pProp);	
	bool                       ExecWndParamListSelChanged(CJETPropertyGridProperty *pProp);	
	bool                       ExecWndParamListChanged_General(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);	
	bool                       ExecWndParamListChanged_PatternMatch(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool                       ExecWndParamListChanged_BrightRatio(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);	
	bool                       ExecWndParamListChanged_OuterShort(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool                       ExecWndParamListChanged_BlobCount(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool                       ExecWndParamListChanged_BodyTilt(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool                       ExecWndParamListChanged_BarcodeRecognize(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool                       ExecWndParamListChanged_ObjectMeasure(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool                       ExecWndParamListChanged_ModelMatch(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool                       ExecWndParamListChanged_ImageMatch(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool                       ExecWndParamListChanged_CharVerify(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool                       ExecWndParamListChanged_ColorCode(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool                       ExecWndParamListChanged_FdMatch(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool                       ExecWndParamListChanged_EdgeSearch(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool                       ExecWndParamListChanged_ShapeVerify(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool                       ExecWndParamListChanged_AngleMeasure(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool                       ExecWndParamListChanged_PixelCompare(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool                       ExecWndParamListChanged_GroupCompare(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool					   ExecWndParamListChanged_IPC(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool				   	   ExecWndParamListChanged_ResinHeight(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool                       ExecWndParamListChanged_WireWidth(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool					   ExecWndParamListChanged_AIModel(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);	
	bool					   ExecWndParamListChanged_SolderWetting(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool					   ExecWndParamListChanged_MeasureBlackGlue(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);	
	bool					   ExecWndParamListChanged_MeasureFluxArea(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool					   ExecWndParamListChanged_MeasureCpuPin(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);	
	bool					   ExecWndParamListChanged_MeasureSIP(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	bool					   ExecWndParamListChanged_MeasureConnector(CJETPropertyGridProperty *pProp, TWND_PARAM_CHANGED_RESULT &Changed);
	//---------------------------------------------------------------------------------//
	bool                       ExecWndParamDetailSetting_BlobCount(CAOIWnd *WndPtr);
	bool                       ExecWndParamDetailSetting_CharVerify(CAOIWnd *WndPtr);
	//---------------------------------------------------------------------------------//
	bool                       ExecWndParamListChanged_WndRoi_Add(CAOIWnd *WndPtr);
	bool                       ExecWndParamListChanged_WndRoi_Delete(CAOIWnd *WndPtr);
	bool                       ExecWndParamListChanged_WndRoi_ClearList(CAOIWnd *WndPtr);
	bool                       ExecWndParamListChanged_WndRoi_Add_Auto(CAOIWnd *WndPtr);
	bool                       ExecWndParamListChanged_WndRoi_Add_Auto_Text(CAOIWnd *WndPtr);
	bool                       ExecWndParamListChanged_WndRoi_Add_Auto_Color(CAOIWnd *WndPtr);
	bool                       ExecWndParamListChanged_WndRoi_Modify_Auto(CAOIWnd *WndPtr, WND_ALG_PROPERTY_ID ParamID);
	bool                       ExecWndParamListChanged_WndRoi_Modify_Auto_Delete(CAOIWnd *WndPtr);
	//---------------------------------------------------------------------------------//
	bool                       ExecWndParamListChanged_MaskBox_Add(CAOIWnd *WndPtr);
	bool                       ExecWndParamListChanged_MaskBox_Rotate(CAOIWnd *WndPtr, double Angle);
	bool                       ExecWndParamListChanged_MaskBox_ShapeMode(CAOIWnd *WndPtr, BOX_SHAPE_MODE BoxShapeMode);	
	bool                       ExecWndParamListChanged_MaskBox_ShapeParam(CAOIWnd *WndPtr, double ShapeParam);	
	bool                       ExecWndParamListChanged_MaskBox_ShapeParam2(CAOIWnd *WndPtr, double ShapeParam);	
	bool                       ExecWndParamListChanged_MaskBox_Delete(CAOIWnd *WndPtr);
	bool                       ExecWndParamListChanged_MaskBox_ClearList(CAOIWnd *WndPtr);
	//---------------------------------------------------------------------------------//
	bool                       ExecWndParamList_BarcodeRecognizeWnd(CAOIWnd *WndPtr, bool bUpdateModel);
	bool                       ExecWndParamList_SolderWettingWnd(CAOIWnd *WndPtr);
	bool                       ExecWndParamList_MeasureBlackGlueWnd(CAOIWnd *WndPtr);	
	bool                       ExecWndParamList_MeasureFluxAreaWnd(CAOIWnd *WndPtr);	
	bool                       ExecWndParamList_MeasureCpuPinWnd(CAOIWnd *WndPtr);	
	//---------------------------------------------------------------------------------//	
	bool                       BuildModelWndProp(CWnd *SrcWndPtr);
	bool                       BuildModelWndProp_FD(CWnd *SrcWndPtr);
	bool                       BuildModelWndProp_Mark(CWnd *SrcWndPtr);
	bool                       BuildModelWndProp_Barcode(CWnd *SrcWndPtr);		
	bool                       BuildModelWndProp_Component(CWnd *SrcWndPtr);
	bool                       UpdateModelWndProp();
	bool                       ClearModelWndProp();	
	//---------------------------------------------------------------------------------//
	unsigned int               DecodeFrameUniqueID(LPCTSTR ItemText);//解出畫面的唯一碼
	//---------------------------------------------------------------------------------//
	void                       LockUIWnd(bool bLock);	
	bool                       GetLockUIWnd() const;//取得是否鎖住UIWnd
	bool                       GetEnableUIWnd() const;//取得是否啟用UI視窗	
	bool                       UpdateTextColor();//更新文字顏色	
	//---------------------------------------------------------------------------------//
	bool                       CheckShowAlgAngleMeasureBaseLineMode(ANGLE_MEASURE_MODE Mode) const;
	bool                       ExecWndParamChangedUpdate(CAOIModel *ModelPtr, CAOIWnd *WndPtr, TWND_PARAM_CHANGED_RESULT Changed);
	//---------------------------------------------------------------------------------//
	// Generated message map functions
protected:
	//{{AFX_MSG(CEditWndView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);	
	afx_msg void OnClassCopyBtn();	
	afx_msg void OnClassClearBtn();	
	afx_msg void OnSelchangeClassCombo();	
	afx_msg LRESULT OnPropertyLClicked(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPropertyRClicked(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPropertyLDbClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPropertyRDbClick(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPropertySelChanged(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITWNDVIEW_H__DC3D3F6B_2939_4188_BA90_8699846C7395__INCLUDED_)
