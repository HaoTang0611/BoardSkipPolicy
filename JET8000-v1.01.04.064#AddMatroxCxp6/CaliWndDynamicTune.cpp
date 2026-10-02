// CaliWndDynamicTune.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "CaliWndDynamicTune.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCaliWndDynamicTune dialog
//-------------------------------------------------------------------------------------//
CCaliWndDynamicTune::CCaliWndDynamicTune(CWnd* pParent /*=NULL*/)
	: CDialog(CCaliWndDynamicTune::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCaliWndDynamicTune)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
//-------------------------------------------------------------------------------------//
void CCaliWndDynamicTune::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCaliWndDynamicTune)
	DDX_Control(pDX, DYNTUNE_AXIS_COMBO, m_AxisCombox);
	DDX_Control(pDX, DYNTUNE_REPEAT_COMBO, m_RepeatCombox);
	DDX_Control(pDX, DYNTUNE_DISTANCE_MIN_COMBO, m_DisMinCombox);
	DDX_Control(pDX, DYNTUNE_DISTANCE_MAX_COMBO, m_DisMaxCombox);
	DDX_Control(pDX, DYNTUNE_DISTANCE_STEP_COMBO, m_DisStepCombox);
	DDX_Control(pDX, DYNTUNE_VELOCITY_MIN_COMBO, m_VelMinCombox);
	DDX_Control(pDX, DYNTUNE_VELOCITY_MAX_COMBO, m_VelMaxCombox);
	DDX_Control(pDX, DYNTUNE_VELOCITY_STEP_COMBO, m_VelStepCombox);
	DDX_Control(pDX, DYNTUNE_ACCELERATION_MIN_COMBO, m_AccMinCombox);
	DDX_Control(pDX, DYNTUNE_ACCELERATION_MAX_COMBO, m_AccMaxCombox);
	DDX_Control(pDX, DYNTUNE_ACCELERATION_STEP_COMBO, m_AccStepCombox);	
	//}}AFX_DATA_MAP
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CCaliWndDynamicTune, CDialog)
	//{{AFX_MSG_MAP(CCaliWndDynamicTune)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CCaliWndDynamicTune message handlers

BOOL CCaliWndDynamicTune::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SwitchMultiLanguage();
	InitialUI();
	UpdateToUI(m_DynamicTuneParam);
	AOIDataCollect.UpdateUIWndFont(GetSafeHwnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------------------//
void CCaliWndDynamicTune::GetTDynamicTuneParam(TDynamicTuneParam &Param)
{
	Param = m_DynamicTuneParam;
}
//-------------------------------------------------------------------------------------//
void CCaliWndDynamicTune::SetTDynamicTuneParam(const TDynamicTuneParam &Param)
{
	m_DynamicTuneParam = Param;
}
//-------------------------------------------------------------------------------------//
void  CCaliWndDynamicTune::SwitchMultiLanguage()
{
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_CALIBRATION_WND_DYNAMIC_TUNE");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_CALIBRATION_WND_DYNAMIC_TUNE;
	WndKey = _T("IDD_CALIBRATION_WND_DYNAMIC_TUNE");
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
	WndID = DYNTUNE_AXIS_LABEL;
	WndKey = _T("DYNTUNE_AXIS_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = DYNTUNE_REPEAT_LABEL;
	WndKey = _T("DYNTUNE_REPEAT_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	
	WndID = DYNTUNE_TOLERANCE_LABEL;
	WndKey = _T("DYNTUNE_TOLERANCE_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = DYNTUNE_DISTANCE_GROUP;
	WndKey = _T("DYNTUNE_DISTANCE_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = DYNTUNE_DISTANCE_MIN_LABEL;
	WndKey = _T("DYNTUNE_DISTANCE_MIN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = DYNTUNE_DISTANCE_MAX_LABEL;
	WndKey = _T("DYNTUNE_DISTANCE_MAX_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = DYNTUNE_DISTANCE_STEP_LABEL;
	WndKey = _T("DYNTUNE_DISTANCE_STEP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = DYNTUNE_VELOCITY_GROUP;
	WndKey = _T("DYNTUNE_VELOCITY_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = DYNTUNE_VELOCITY_MIN_LABEL;
	WndKey = _T("DYNTUNE_VELOCITY_MIN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = DYNTUNE_VELOCITY_MAX_LABEL;
	WndKey = _T("DYNTUNE_VELOCITY_MAX_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = DYNTUNE_VELOCITY_STEP_LABEL;
	WndKey = _T("DYNTUNE_VELOCITY_STEP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = DYNTUNE_ACCELERATION_GROUP;
	WndKey = _T("DYNTUNE_ACCELERATION_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = DYNTUNE_ACCELERATION_MIN_LABEL;
	WndKey = _T("DYNTUNE_ACCELERATION_MIN_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = DYNTUNE_ACCELERATION_MAX_LABEL;
	WndKey = _T("DYNTUNE_ACCELERATION_MAX_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = DYNTUNE_ACCELERATION_STEP_LABEL;
	WndKey = _T("DYNTUNE_ACCELERATION_STEP_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
	WndID = DYNTUNE_ACC_DEC_MIN_TIME_GROUP;
	WndKey = _T("DYNTUNE_ACC_DEC_MIN_TIME_GROUP");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = DYNTUNE_ACC_MIN_TIME_LABEL;
	WndKey = _T("DYNTUNE_ACC_MIN_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);

	WndID = DYNTUNE_DEC_MIN_TIME_LABEL;
	WndKey = _T("DYNTUNE_DEC_MIN_TIME_LABEL");
	this->GetDlgItemText(WndID, LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetDlgItemText(WndID, NewLabelText);
	//---------------------------------------------------------------------------------//
}
//-------------------------------------------------------------------------------------//
CString CCaliWndDynamicTune::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_CALIBRATION_WND_DYNAMIC_TUNE");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
void CCaliWndDynamicTune::OnOK() 
{
	// TODO: Add extra validation here
	UpdateToParam(m_DynamicTuneParam);
	CDialog::OnOK();
}
//-------------------------------------------------------------------------------------//
void CCaliWndDynamicTune::InitialUI()
{
	int nItem=0;
	CString str;
	TDynamicTuneParam DefaultParam;
	//Axis
	{		
		CComboBox &Combox=m_AxisCombox;
		nItem=0;
		JetAPI::ClearCombox(Combox);

		str = _T("X"); 
		Combox.AddString(str); 
		Combox.SetItemData(nItem, AXIS_X);	nItem ++;

		str = _T("Y"); 
		Combox.AddString(str); 
		Combox.SetItemData(nItem, AXIS_Y);	nItem ++;
		JetAPI::SetComboxCurSel(Combox, DefaultParam.nAxis);
	}

	//Repeat
	{		
		CComboBox &Combox=m_RepeatCombox;
		nItem=0;
		JetAPI::ClearCombox(Combox);
		for ( int i=0; i<5; i++ )
		{
			str.Format(_T("%d"), i+1);
			Combox.AddString(str); 
			Combox.SetItemData(nItem, i+1);	nItem ++;
		}
		JetAPI::SetComboxCurSel(Combox, DefaultParam.nRepeat);
	}

	CWnd::SetDlgItemInt(DYNTUNE_TOLERANCE_EDIT, DefaultParam.nTolerance);


	//Distance
	{	
		int  Value=0;		
		int  Step=DefaultParam.DistanceStep;
		{
			int  Start=DefaultParam.DistanceMin;
			CComboBox &Combox=m_DisMinCombox;			
			nItem=0;
			JetAPI::ClearCombox(Combox);			
			for ( int i=0; i<5; i++ )
			{
				Value = i*Step+Start;
				str.Format(_T("%d"), Value);
				Combox.AddString(str);			
				Combox.SetItemData(nItem, Value);
				nItem ++;
			}
			Combox.SetCurSel(0);
		}

		{
			int  Start=DefaultParam.DistanceMax;
			CComboBox &Combox=m_DisMaxCombox;
			nItem=0;
			JetAPI::ClearCombox(Combox);			
			for ( int i=0; i<35; i++ )
			{
				Value = i*Step+Start;
				str.Format(_T("%d"), Value);
				Combox.AddString(str);			
				Combox.SetItemData(nItem, Value);
				nItem ++;
			}
			Combox.SetCurSel(0);
		}

		{
			CComboBox &Combox=m_DisStepCombox;
			nItem=0;
			JetAPI::ClearCombox(Combox);			
			for ( int i=0; i<5; i++ )
			{
				Value = (i+1)*Step;
				str.Format(_T("%d"), Value);
				Combox.AddString(str);			
				Combox.SetItemData(nItem, Value);
				nItem ++;
			}
			Combox.SetCurSel(0);
		}
	}

	//Velocity
	{	
		int  Value=0;
		int  Step=DefaultParam.VelocityStep;
		{
			int  Start=DefaultParam.VelocityMin;
			CComboBox &Combox=m_VelMinCombox;
			nItem=0;
			JetAPI::ClearCombox(Combox);			
			for ( int i=0; i<5; i++ )
			{
				Value = i*Step+Start;
				str.Format(_T("%d"), Value);
				Combox.AddString(str);			
				Combox.SetItemData(nItem, Value);
				nItem ++;
			}
			Combox.SetCurSel(0);
		}	
		
		{
			int  Start=DefaultParam.VelocityMax;
			CComboBox &Combox=m_VelMaxCombox;
			nItem=0;
			JetAPI::ClearCombox(Combox);			
			for ( int i=0; i<5; i++ )
			{
				Value = i*Step+Start;
				str.Format(_T("%d"), Value);
				Combox.AddString(str);			
				Combox.SetItemData(nItem, Value);
				nItem ++;
			}
			Combox.SetCurSel(0);
		}
		
		{
			CComboBox &Combox=m_VelStepCombox;
			nItem=0;
			JetAPI::ClearCombox(Combox);			
			for ( int i=0; i<5; i++ )
			{
				Value = (i+1)*Step;
				str.Format(_T("%d"), Value);
				Combox.AddString(str);			
				Combox.SetItemData(nItem, Value);
				nItem ++;
			}
			Combox.SetCurSel(0);
		}
	}

	//Acceleration
	{	
		int  Value=0;
		int  Step=DefaultParam.AccelerationStep;
		{
			int  Start=DefaultParam.AccelerationMin;
			CComboBox &Combox=m_AccMinCombox;
			nItem=0;
			JetAPI::ClearCombox(Combox);			
			for ( int i=0; i<5; i++ )
			{
				Value = i*Step+Start;
				str.Format(_T("%d"), Value);
				Combox.AddString(str);			
				Combox.SetItemData(nItem, Value);
				nItem ++;
			}
			Combox.SetCurSel(0);
		}

		{
			int  Start=DefaultParam.AccelerationMax;
			CComboBox &Combox=m_AccMaxCombox;
			nItem=0;
			JetAPI::ClearCombox(Combox);			
			for ( int i=0; i<10; i++ )
			{
				Value = i*Step+Start;
				str.Format(_T("%d"), Value);
				Combox.AddString(str);			
				Combox.SetItemData(nItem, Value);
				nItem ++;
			}
			Combox.SetCurSel(0);
		}
		
		{
			CComboBox &Combox=m_AccStepCombox;
			nItem=0;
			JetAPI::ClearCombox(Combox);			
			for ( int i=0; i<5; i++ )
			{
				Value = (i+1)*Step;
				str.Format(_T("%d"), Value);
				Combox.AddString(str);			
				Combox.SetItemData(nItem, Value);
				nItem ++;
			}
			Combox.SetCurSel(0);
		}	
	}

	str.Format(_T("%.3f"), DefaultParam.AccelerationMinTime);
	CWnd::SetDlgItemText(DYNTUNE_ACC_MIN_TIME_EDIT, str);
	str.Format(_T("%.3f"), DefaultParam.DecelerationMinTime);
	CWnd::SetDlgItemText(DYNTUNE_DEC_MIN_TIME_EDIT, str);
	return;
}
//-------------------------------------------------------------------------------------//
bool CCaliWndDynamicTune::UpdateToParam(TDynamicTuneParam &Param)
{
	int nItem=0;
	CString str;	
	//Axis
	{		
		CComboBox &Combox=m_AxisCombox;
		Param.nAxis = JetAPI::GetComboxCurSelData(Combox);
	}

	//Repeat
	{		
		CComboBox &Combox=m_RepeatCombox;
		Param.nRepeat = JetAPI::GetComboxCurSelData(Combox);
	}

	//Tolerance
	Param.nTolerance = (int)CWnd::GetDlgItemInt(DYNTUNE_TOLERANCE_EDIT);
	Param.nTolerance = ::abs(Param.nTolerance);

	//Distance
	{	
		int idx=0;
		int Min=0;
		int Max=0;
		int Step=1000;		
		CComboBox &ComboxMin=m_DisMinCombox;
		CComboBox &ComboxMax=m_DisMaxCombox;
		CComboBox &ComboxStep=m_DisStepCombox;
		str = JetAPI::GetComboxCurSelText(ComboxMin);
		if ( str.GetLength() > 0 ) { Min = ::_ttoi(str); }
		str = JetAPI::GetComboxCurSelText(ComboxMax);
		if ( str.GetLength() > 0 ) { Max = ::_ttoi(str); }
		str = JetAPI::GetComboxCurSelText(ComboxStep);
		if ( str.GetLength() > 0 ) { Step = ::_ttoi(str); }
		Param.DistanceStep = Step;
		Param.DistanceMin = MIN(Min, Max);
		Param.DistanceMax = MAX(Min, Max);		
	}

	//Velocity
	{	
		int Min=0;
		int Max=0;
		int Step=100;				
		CComboBox &ComboxMin=m_VelMinCombox;
		CComboBox &ComboxMax=m_VelMaxCombox;
		CComboBox &ComboxStep=m_VelStepCombox;
		str = JetAPI::GetComboxCurSelText(ComboxMin);
		if ( str.GetLength() > 0 ) { Min = ::_ttoi(str); }
		str = JetAPI::GetComboxCurSelText(ComboxMax);
		if ( str.GetLength() > 0 ) { Max = ::_ttoi(str); }
		str = JetAPI::GetComboxCurSelText(ComboxStep);
		if ( str.GetLength() > 0 ) { Step = ::_ttoi(str); }
		Param.VelocityStep = Step;
		Param.VelocityMin = MIN(Min, Max);
		Param.VelocityMax = MAX(Min, Max);
	}

	//Acceleration
	{	
		int Min=0;
		int Max=0;
		int Step=1000;		
		CComboBox &ComboxMin=m_AccMinCombox;
		CComboBox &ComboxMax=m_AccMaxCombox;
		CComboBox &ComboxStep=m_AccStepCombox;
		str = JetAPI::GetComboxCurSelText(ComboxMin);
		if ( str.GetLength() > 0 ) { Min = ::_ttoi(str); }
		str = JetAPI::GetComboxCurSelText(ComboxMax);
		if ( str.GetLength() > 0 ) { Max = ::_ttoi(str); }
		str = JetAPI::GetComboxCurSelText(ComboxStep);
		if ( str.GetLength() > 0 ) { Step = ::_ttoi(str); }
		Param.AccelerationStep = Step;
		Param.AccelerationMin = MIN(Min, Max);
		Param.AccelerationMax = MAX(Min, Max);
	}

	CWnd::GetDlgItemText(DYNTUNE_ACC_MIN_TIME_EDIT, str);
	Param.AccelerationMinTime = ::fabs(::_ttof(str));	
	
	CWnd::GetDlgItemText(DYNTUNE_DEC_MIN_TIME_EDIT, str);
	Param.DecelerationMinTime = ::fabs(::_ttof(str));	
	return true;
}
//-------------------------------------------------------------------------------------//
void CCaliWndDynamicTune::UpdateToUI(const TDynamicTuneParam &Param)
{
	CString  str;
	JetAPI::SetComboxCurSel(m_AxisCombox, Param.nAxis);
	JetAPI::SetComboxCurSel(m_RepeatCombox, Param.nRepeat);
	CWnd::SetDlgItemInt(DYNTUNE_TOLERANCE_EDIT, Param.nTolerance);
	
	JetAPI::SetComboxCurSel(m_DisMinCombox, Param.DistanceMin);
	JetAPI::SetComboxCurSel(m_DisMaxCombox, Param.DistanceMax);
	JetAPI::SetComboxCurSel(m_DisStepCombox, Param.DistanceStep);
	
	JetAPI::SetComboxCurSel(m_VelMinCombox, Param.VelocityMin);
	JetAPI::SetComboxCurSel(m_VelMaxCombox, Param.VelocityMax);
	JetAPI::SetComboxCurSel(m_VelStepCombox, Param.VelocityStep);

	JetAPI::SetComboxCurSel(m_AccMinCombox, Param.AccelerationMin);
	JetAPI::SetComboxCurSel(m_AccMaxCombox, Param.AccelerationMax);
	JetAPI::SetComboxCurSel(m_AccStepCombox, Param.AccelerationStep);	

	str.Format(_T("%.3f"), Param.AccelerationMinTime);
	CWnd::SetDlgItemText(DYNTUNE_ACC_MIN_TIME_EDIT, str);
	str.Format(_T("%.3f"), Param.DecelerationMinTime);
	CWnd::SetDlgItemText(DYNTUNE_DEC_MIN_TIME_EDIT, str);
	return ;
}
//-------------------------------------------------------------------------------------//
