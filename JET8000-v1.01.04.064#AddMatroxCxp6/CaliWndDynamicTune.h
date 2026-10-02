#if !defined(AFX_CALIWNDDYNAMICTUNE_H__EB094488_6776_46D3_9668_D9669EA7080D__INCLUDED_)
#define AFX_CALIWNDDYNAMICTUNE_H__EB094488_6776_46D3_9668_D9669EA7080D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CaliWndDynamicTune.h : header file
//
//-------------------------------------------------------------------------------------//
typedef struct tagDynamicTuneParam
{
	int        nAxis;//軸號
	int        nRepeat;//重複數
	int        nTolerance;//公差量//um

	int        nGroupID;//群組編號
	double     OffsetX;//偏差X-um
	double     OffsetY;//偏差Y-um
	double     TimeUse;//耗時-ms

	int        DistanceUse;//偏移使用
	int        DistanceMin;//偏移下限//um
	int        DistanceMax;//偏移上限
	int        DistanceStep;//偏移步長

	int        VelocityUse;//速度使用
	int        VelocityMin;//速度下限
	int        VelocityMax;//速度上限
	int        VelocityStep;//速度步長
	
	int        AccelerationUse;//加速度使用
	int        AccelerationMin;//加速度下限
	int        AccelerationMax;//加速度上限
	int        AccelerationStep;//加速度步長	

	double     AccelerationMinTime;//加速度最短時間
	double     DecelerationMinTime;//減速度最短時間
	tagDynamicTuneParam()
	{
		nAxis = AXIS_X;
		nRepeat = 1;
		nTolerance = 3;

		nGroupID = 0;//群組編號
		OffsetX  = 0.0;//偏差X-um
		OffsetY  = 0.0;//偏差Y-um
		TimeUse = 0.0;//耗時-ms

		DistanceUse = 10000;
		DistanceMin = 10000;//偏移下限
		DistanceMax = 60000;//偏移上限
		DistanceStep= 10000;//偏移步長

		VelocityUse = 100000;//速度使用
		VelocityMin = 100000;//速度下限
		VelocityMax = 600000;//速度上限
		VelocityStep= 100000;//速度步長
	
		AccelerationUse = 1000000;//加速度使用
		AccelerationMin = 1000000;//加速度下限
		AccelerationMax = 6000000;//加速度上限
		AccelerationStep= 1000000;//加速度步長

		AccelerationMinTime = 0.1;//加速度最短時間
		DecelerationMinTime = 0.1;//減速度最短時間
	}
} TDynamicTuneParam, *PDynamicTuneParam;
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCaliWndDynamicTune dialog
//-------------------------------------------------------------------------------------//
class CCaliWndDynamicTune : public CDialog
{
// Construction
public:
	CCaliWndDynamicTune(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCaliWndDynamicTune)
	enum { IDD = IDD_CALIBRATION_WND_DYNAMIC_TUNE };
	CComboBox	m_AxisCombox;
	CComboBox	m_RepeatCombox;
	CComboBox	m_DisMinCombox;
	CComboBox	m_DisMaxCombox;
	CComboBox	m_DisStepCombox;
	CComboBox	m_VelMinCombox;
	CComboBox	m_VelMaxCombox;
	CComboBox	m_VelStepCombox;
	CComboBox	m_AccMinCombox;
	CComboBox	m_AccMaxCombox;
	CComboBox	m_AccStepCombox;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaliWndDynamicTune)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//---------------------------------------------------------------------------------//
	void                       GetTDynamicTuneParam(TDynamicTuneParam &Param);
	void                       SetTDynamicTuneParam(const TDynamicTuneParam &Param);	
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	TDynamicTuneParam          m_DynamicTuneParam;
	//---------------------------------------------------------------------------------//
	void                       SwitchMultiLanguage();
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//	
	void                       InitialUI();
	bool                       UpdateToParam(TDynamicTuneParam &Param);
	void                       UpdateToUI(const TDynamicTuneParam &Param);	
	//---------------------------------------------------------------------------------//	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCaliWndDynamicTune)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//-------------------------------------------------------------------------------------//
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIWNDDYNAMICTUNE_H__EB094488_6776_46D3_9668_D9669EA7080D__INCLUDED_)
