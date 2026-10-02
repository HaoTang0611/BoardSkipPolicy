#pragma once
//-------------------------------------------------------------------------------------//
#if FRAME_STYLE_TYPE  != FRAME_STYLE_MFC
// CPropertiesToolBar
class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CJETPropertyGridCtrl;
class CJETPropertyGridProperty : public CMFCPropertyGridProperty
{
	friend CJETPropertyGridCtrl;
	DECLARE_DYNAMIC(CJETPropertyGridProperty)
private:
	//---------------------------------------------------------------------------------//
	UINT                       m_uID;	
	//For Reading
	BOOL                       m_bReadingMode;
	BOOL                       m_bReadingRight;
	COleVariant                m_varReading;	
	CRect                      m_rectValue;	
	CRect                      m_rectReading;	
	COLORREF                   m_clrValueText;
	COLORREF                   m_clrCaptionText;
	COLORREF                   m_clrReadingText;

	//For Button             
	BOOL                       m_bHasUserBtn;
	BOOL                       m_bClickUserBtn;
	CImageList                 m_BtnImages;

	//For Check Box
	bool                       m_bCheckValue;
	bool                       m_bCheckValueOrig;
	CRect                      m_rectCheckBox;
	BOOL                       m_bCheckBoxEnabled;
	//---------------------------------------------------------------------------------//	
	CString                    m_strTextFormatShort;
	CString                    m_strTextFormatLong;
	CString                    m_strTextFormatChar;
	CString                    m_strTextFormatUShort;
	CString                    m_strTextFormatULong;
	CString                    m_strTextFormatFloat;
	CString                    m_strTextFormatDouble;
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	void                       InitTextFormat();
	CString                    FormatReading();
	void                       ClickCheckBox();
	void                       CalcRect(const CRect &rect, int &nMid, CRect &rcValue, CRect &rcReading, BOOL ReadingOnRight);
	virtual void               OnDrawReading(CDC* pDC, CRect rect);	
	virtual void               OnDrawUserBtn(CDC* pDC, CRect rectButton);
	virtual void               OnClickUserBtn(CPoint point);
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	// Group constructor
	CJETPropertyGridProperty(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);
	//---------------------------------------------------------------------------------//	
	// Simple property
	CJETPropertyGridProperty(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0,
		LPCTSTR lpszEditMask = NULL, LPCTSTR lpszEditTemplate = NULL, LPCTSTR lpszValidChars = NULL);
	//---------------------------------------------------------------------------------//	
	~CJETPropertyGridProperty(void);
	//---------------------------------------------------------------------------------//
	virtual CString FormatProperty();
	//---------------------------------------------------------------------------------//
	void         SetID(UINT ID) { m_uID = ID; }
	UINT         GetID() const { return m_uID; }
	//---------------------------------------------------------------------------------//
	CJETPropertyGridProperty* FindSubItemByID(UINT ID) const;
	//---------------------------------------------------------------------------------//
	void         SetValueTextColor(COLORREF var) { m_clrValueText = var; }
	void         SetCaptionTextColor(COLORREF var) { m_clrCaptionText = var; }
	//---------------------------------------------------------------------------------//
	void         SetHasUserBtn(BOOL Has=TRUE, UINT BMPID=0);	
	BOOL         GetClickUserBtn() const { return m_bClickUserBtn; }	
	//---------------------------------------------------------------------------------//
	void         SetReading(const COleVariant &var, BOOL Mode=TRUE) { m_varReading = var; m_bReadingMode=Mode; }
	void         SetReadingMode(BOOL var) { m_bReadingMode = var; }
	void         SetReadingOnRight(BOOL var) { m_bReadingRight = var; }
	void         SetReadingTextColor(COLORREF var) { m_clrReadingText = var; }
	//---------------------------------------------------------------------------------//
	void         SetCheckValue(BOOL bCheck, BOOL bCheckBoxEnabled=TRUE);
	virtual bool GetCheckValue();
	virtual bool GetOriginalCheckValue();	
	//---------------------------------------------------------------------------------//
	virtual BOOL HasButton() const;	
	virtual void OnClickButton(CPoint point);
	//---------------------------------------------------------------------------------//
	virtual void OnDrawName(CDC* pDC, CRect rect);	
	virtual void OnDrawValue(CDC* pDC, CRect rect);	
	virtual void OnDrawButton(CDC* pDC, CRect rectButton);
	virtual void OnDrawCheckBox(CDC * pDC, CRect rectCheck, BOOL bChecked);
	//---------------------------------------------------------------------------------//
	virtual void OnClickName(CPoint point);
	virtual BOOL OnDblClk(CPoint point);
	//---------------------------------------------------------------------------------//
	virtual void AdjustButtonRect();
	virtual void AdjustInPlaceEditRect(CRect& rectEdit, CRect& rectSpin);
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
// CJETPropertyGridCtrl
class CJETPropertyGridCtrl : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(CJETPropertyGridCtrl)

public:
	CJETPropertyGridCtrl();
	virtual ~CJETPropertyGridCtrl();
	
protected:
	BOOL         m_EnableLBtnDbClick;//¤¹³\¥ªÁäÂùÀ»
	BOOL         m_EnableRBtnDbClick;//¤¹³\¥kÁäÂùÀ»
	float        m_fLeftColumnWidthRatio;//¥ª°¼¼e«×¤ñ¨Ò	

protected:
	// Generated message map functions
	//{{AFX_MSG(CMFCPropertyGridCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
	virtual void OnChangeSelection(CMFCPropertyGridProperty* /*pNewSel*/, CMFCPropertyGridProperty* /*pOldSel*/);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:	
	CJETPropertyGridProperty*  FindItemByID(UINT ID, BOOL bSearchSubItems=TRUE) const;

	void     SetEnableLBtnDbClick(BOOL val) { m_EnableLBtnDbClick=val; }//¤¹³\¥ªÁäÂùÀ»
	void     SetEnableRBtnDbClick(BOOL val) { m_EnableRBtnDbClick=val; }//¤¹³\¥kÁäÂùÀ»
	void     SetLeftColumnWidthRatio(float val) { m_fLeftColumnWidthRatio=val; }//³]©w¥ª°¼¼e«×¤ñ¨Ò	

	COLORREF SetBackgroundColor(COLORREF clr);            // Control background color
	COLORREF SetTextColor(COLORREF clr);                  // Control foreground color
	COLORREF SetGroupBackgroundColor(COLORREF clr);       // Group background text
	COLORREF SetGroupTextColor(COLORREF clr);             // Group foreground text
	COLORREF SetDescriptionBackgroundColor(COLORREF clr); // Description background text
	COLORREF SetDescriptionTextColor(COLORREF clr);       // Description foreground text
	COLORREF SetLineColor(COLORREF clr);                  // Color of the grid lines
};
//-------------------------------------------------------------------------------------//
CJETPropertyGridProperty* CreateGridPropertyBarcodeDecoderList(LPCTSTR Caption, BARCODE_DECODER_TYPE Decoder, void *Ptr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyBarcodeSpreadModeList(LPCTSTR Caption, BARCODE_SPREAD_MODE Mode, void *Ptr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyBarcodeBelongModeList(LPCTSTR Caption, BARCODE_BELONG_MODE Mode, void *Ptr, LPCTSTR Descr, BARCODE_BELONG_MODE Score);
CJETPropertyGridProperty* CreateGridPropertySaveTestImageModeList(LPCTSTR Caption, SAVE_TEST_IMAGE_MODE Mode, void *Ptr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyBarcodeDeviceIndexList(LPCTSTR Caption, unsigned int BarcodeDeviceIndex, void *Ptr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyBarcodeDeviceCodeIndexList(LPCTSTR Caption, unsigned int CodeIndex, void *Ptr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyAIModelIDList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyWndDefectList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyRotateAngleList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyWndLogicTypeList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyWndLogicGroupIDList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyWndFollowModeList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyWndRgnLinkModeList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyWndBoxShapeModeList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyWndSyncMoveModeList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyWndConstrainModeList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyAlgBaseValueGroupIDList(LPCTSTR Caption, CAOIWnd *WndPtr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyAlgEdgeFeatureModeList(LPCTSTR Caption, ALG_EDGE_FEATURE_MODE EdgeMode, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyAlgSearchDirectionList(LPCTSTR Caption, ALG_SEARCH_DIRECTION SearchDir, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyWndFrameList(LPCTSTR Caption, CAOIProject *pProject, CAOIWnd *WndPtr, unsigned int FrameUniqueID, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyAlgorithmList(AOI_OBJ_TYPE AttachedType, int WndGroupID, WND_DEFECT_ID WndDefectID, ALG_TYPE AlgType, CAOILand *LandPtr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyAlgDirectionList(LPCTSTR Caption, ALG_DIRECTION Direction, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyAlgMatchDockModeList(LPCTSTR Caption, ALG_MATCH_DOCK_MODE DockMode, void *Ptr, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyTiltCellModeList(LPCTSTR Caption, ALG_TILE_CELL_MODE CellMode, LPCTSTR Descr, CAOIWnd *WndPtr);
CJETPropertyGridProperty* CreateGridPropertyBarcodeDirModeList(LPCTSTR Caption, ALG_BARCODE_DIR_MODE DirMode, LPCTSTR Descr, CAOIWnd *WndPtr);
CJETPropertyGridProperty* CreateGridPropertyBarcodeStepParam(LPCTSTR Caption, const TALG_BARCODE_STEP_PARAM &StepParam, UINT ParamID, LPCTSTR Descr, CAOIWnd *WndPtr);
CJETPropertyGridProperty* CreateGridPropertyMatchMinReduceAreaList(LPCTSTR Caption, int MinReduceArea, LPCTSTR Descr);
CJETPropertyGridProperty* CreateGridPropertyMatchFinalReductionList(LPCTSTR Caption, int FinalReduction, LPCTSTR Descr);
//-------------------------------------------------------------------------------------//
#endif//FRAME_STYLE_TYPE

