//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "AOIDataCollect.h"
//-------------------------------------------------------------------------------------//
#include "JetLoadDll.h"
//-------------------------------------------------------------------------------------//
const bool bUseProjectOperLogFunc=false;
//-------------------------------------------------------------------------------------//
enum LOG_OPER_MODE
{
	LOG_OPER_SYSTEM_SCORE         =   1,
	LOG_OPER_PROJECT_SCORE        = 100,
	LOG_OPER_PANEL_SCORE          = 110,
	LOG_OPER_BOARD_SCORE          = 120,	
	LOG_OPER_FD_SCORE             = 130,
	LOG_OPER_MARK_SCORE           = 140,	
	LOG_OPER_BARCODE_SCORE        = 150,	
	LOG_OPER_COMPONENT_SCORE      = 160,	
	LOG_OPER_PART_GROUP_SCORE     = 170,	
	LOG_OPER_MODEL_SCORE          = 500,
	LOG_OPER_LAND_SCORE           = 510,
	LOG_OPER_WND_SCORE            = 520,	
	LOG_OPER_WND_ROI_SCORE        = 530,	
	LOG_OPER_WND_MASK_SCORE       = 540,		
	LOG_OPER_RETURN 
};
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_Func(int OperMode, LPCTSTR tag, LPCTSTR Content)//儲存操作訊息-系統啟動
{
	if ( SaveLogOper_FuncFn(OperMode, tag, Content) == false )
	{
		SetSystemExceptionCode_FileWrite();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_FuncFn(int OperMode, LPCTSTR tag, LPCTSTR Content)//儲存操作訊息
{
	const int m_SaveLogOperationMessage = GetSystemParameter().m_SaveUserOperationLog;
	if ( FN_DISABLE == m_SaveLogOperationMessage ) { return true; }
#ifdef _SEND_DEBUG_STRING
	if ( FN_DISABLE != m_SystemParameter.m_SendDebugViewString )
	{
		CString DebugString;		
		DebugString.Format(_T("LogOper::[%d] [%s] %s"), OperMode, tag, Content);
		JetAPI::SendDebugString(DebugString);
	}
#endif//_SEND_DEBUG_STRING

	CString Text;
	CString DateTime;
	unsigned int LogIdx=m_LogIdx_LogOper;
	CString UserName = GetCurrentUserName();	
	if ( true == bUseProjectOperLogFunc )
	{
		CAOIProject *ProjectPtr=GetActiveProject();
		if ( NULL != ProjectPtr )
		{	
			if ( ProjectPtr->CheckProjectOperLogIndexValid() == true )
			{	LogIdx = ProjectPtr->GetProjectOperLogIndex();  }
		}
	}
	JetAPI::GetTime(DateTime,CTime::GetCurrentTime());
	//DateTime@User@OperMode@Tag@Content)
	Text.Format(_T("%s@%s@%d@%s@%s"), DateTime, UserName, OperMode, tag, Content);
	if ( LogManager.AddLogMessage(LogIdx, Text) == false )
	{	
		m_ErrorString = _T("CAOIDataCollect::SaveLogOper_Func Fault");
		return false; 
	}

	CAOIProject* ProjectPtr = GetActiveProject();
	if ( NULL != ProjectPtr )
	{	ProjectPtr->SetProjectHasModified(true);	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_Func(LPCTSTR OperName, LPCTSTR tag, LPCTSTR Content)//儲存操作訊息
{
	if ( SaveLogOper_FuncFn(OperName, tag, Content) == false )
	{
		SetSystemExceptionCode_FileWrite();
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_FuncFn(LPCTSTR OperName, LPCTSTR tag, LPCTSTR Content)//儲存操作訊息	
{
	const int m_SaveLogOperationMessage = GetSystemParameter().m_SaveUserOperationLog;
	if ( FN_DISABLE == m_SaveLogOperationMessage ) { return true; }
#ifdef _SEND_DEBUG_STRING
	if ( FN_DISABLE != m_SystemParameter.m_SendDebugViewString )
	{
		CString DebugString;		
		DebugString.Format(_T("LogOper::[%s] [%s] %s"), OperName, tag, Content);
		JetAPI::SendDebugString(DebugString);
	}
#endif//_SEND_DEBUG_STRING

	CString Text;
	CString DateTime;
	unsigned int LogIdx=m_LogIdx_LogOper;
	CString UserName = GetCurrentUserName();	
	if ( true == bUseProjectOperLogFunc )
	{
		CAOIProject *ProjectPtr=GetActiveProject();
		if ( NULL != ProjectPtr )
		{	
			if ( ProjectPtr->CheckProjectOperLogIndexValid() == true )
			{	LogIdx = ProjectPtr->GetProjectOperLogIndex();  }
		}
	}
	JetAPI::GetTime(DateTime,CTime::GetCurrentTime());
	//DateTime@User@OperMode@Tag@Content)
	Text.Format(_T("%s@%s@%s@%s@%s"), DateTime, UserName, OperName, tag, Content);
	if ( LogManager.AddLogMessage(LogIdx, Text) == false )
	{	
		m_ErrorString = _T("CAOIDataCollect::SaveLogOper_Func Fault");
		return false; 
	}

	if (OperName != _T("Project") && OperName != _T("System")) 
	{
		CAOIProject* ProjectPtr = GetActiveProject();;		
		if (NULL != ProjectPtr)
		{	ProjectPtr->SetProjectHasModified(true);	}
		//else m_ErrorString = _T("CAOIDataCollect::ProjectPtr is NULL");
	}
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_UserFunc(LPCTSTR tag, LPCTSTR Content)//儲存操作訊息-使用者函式	
{	
	if ( SaveLogOper_Func(_T("User"), tag, Content) == false )	
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_SystemFunc(LPCTSTR tag, LPCTSTR Content)//儲存操作訊息-系統函式	
{
	return SaveLogOper_Func(_T("System"), tag, Content);	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_ProjectFunc(LPCTSTR tag, LPCTSTR Content)//儲存操作訊息-專案函式
{
	return SaveLogOper_Func(_T("Project"), tag, Content);	
	//return SaveLogOper_Func(LOG_OPER_PROJECT_SCORE, tag, Content);	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_ProjectFunc(CAOIProject *ProjectPtr, LPCTSTR Content)//儲存操作訊息-專案函式
{
	if ( NULL == ProjectPtr ) { return false; }
	CString ShowName=ProjectPtr->GetProjectFileMainName();
	return SaveLogOper_ProjectFunc(ShowName, Content);
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_PanelFunc(CAOIPanel *PanelPtr, LPCTSTR Content)//儲存操作訊息-整板函式
{
	if ( NULL == PanelPtr ) { return false; }
	CString Name;
	CString strPanel = AOIDataDefine.GetPanelText();	
	Name.Format(_T("%s_%04d"), strPanel, PanelPtr->GetPanelIndex_Project()+1);
	if ( SaveLogOper_Func(_T("Panel"), Name, Content) == false )
	//if ( SaveLogOper_Func(LOG_OPER_PANEL_SCORE, Name, Content) == false )
	{	return false; }			
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_BoardFunc(CAOIBoard *BoardPtr, LPCTSTR Content)//儲存操作訊息-單板函式
{
	if ( NULL == BoardPtr ) { return false; }
	CString Name;
	CString strBoard = AOIDataDefine.GetBoardText();
	CAOIPanel *PanelPtr=BoardPtr->GetBoardPanelPtr();
	if ( NULL == PanelPtr )
	{	Name.Format(_T("%s_%04d"), strBoard, BoardPtr->GetBoardPanelIndex_Project()+1);		}	
	else
	{
		CString strPanel = AOIDataDefine.GetPanelText();
		Name.Format(_T("%s_%04d#%s_%04d"), strPanel, PanelPtr->GetPanelIndex_Project()+1, strBoard, BoardPtr->GetBoardIndex_Panel()+1);		
	}	
	if ( SaveLogOper_Func(_T("Board"), Name, Content) == false )
	//if ( SaveLogOper_Func(LOG_OPER_BOARD_SCORE, Name, Content) == false )
	{	return false; }			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_FdFunc(CAOIFd *FdPtr, LPCTSTR Content)//儲存操作訊息-定位點函式
{
	if ( NULL == FdPtr ) { return false; }	
	CString str;
	CString str2;
	CString sName;
	CString Name=FdPtr->GetRgnDerivedName();
	CAOIPanel *PanelPtr=FdPtr->GetFdPanelPtr();
	CAOIBoard *BoardPtr=FdPtr->GetFdBoardPtr();
	if ( NULL != PanelPtr )
	{	
		str.Format(_T("%s_%04d"), AOIDataDefine.GetPanelText(), PanelPtr->GetPanelIndex_Project()+1);	
		if ( sName.GetLength() == 0 ) { sName = str; }
		else 
		{ 
			str2 = sName;
			sName.Format(_T("%s %s"), str2, str); 
		}
		if ( NULL != BoardPtr )
		{	
			str.Format(_T("%s_%04d"), AOIDataDefine.GetBoardText(), BoardPtr->GetBoardIndex_Panel()+1);	
			if ( sName.GetLength() == 0 ) { sName = str; }
			else 
			{ 
				str2 = sName;
				sName.Format(_T("%s %s"), str2, str); 
			}
			Name.Format(_T("%s %s[%04d]"), sName, AOIDataDefine.GetFdText(), FdPtr->GetRgnIndex_Board()+1);
		}
		else
		{	Name.Format(_T("%s %s[%04d]"), sName, AOIDataDefine.GetFdText(), FdPtr->GetRgnIndex_Panel()+1);	}
		str2 = Name;
		Name.Format(_T("%s[ID:%05d]"), str2, FdPtr->GetFdUniqueID()+1);
	}		
	if ( SaveLogOper_Func(_T("Fd"), Name, Content) == false )
	//if ( SaveLogOper_Func(LOG_OPER_FD_SCORE, Name, Content) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_MarkFunc(CAOIMark *MarkPtr, LPCTSTR Content)//儲存操作訊息-特徵點函式
{
	if ( NULL == MarkPtr ) { return false; }	
	CString str;
	CString str2;
	CString sName;
	CString Name=MarkPtr->GetMarkFullName();
	CAOIPanel *PanelPtr=MarkPtr->GetMarkPanelPtr();
	CAOIBoard *BoardPtr=MarkPtr->GetMarkBoardPtr();
	if ( NULL != PanelPtr )
	{	
		str.Format(_T("%s_%04d"), AOIDataDefine.GetPanelText(), PanelPtr->GetPanelIndex_Project()+1);	
		if ( sName.GetLength() == 0 ) { sName = str; }
		else 
		{ 
			str2 = sName;
			sName.Format(_T("%s %s"), str2, str); 
		}
		if ( NULL != BoardPtr )
		{	
			str.Format(_T("%s_%04d"), AOIDataDefine.GetBoardText(), BoardPtr->GetBoardIndex_Panel()+1);	
			if ( sName.GetLength() == 0 ) { sName = str; }
			else 
			{ 
				str2 = sName;
				sName.Format(_T("%s %s"), str2, str); 
			}
			Name.Format(_T("%s %s[%04d]"), sName, AOIDataDefine.GetMarkText(), MarkPtr->GetRgnIndex_Board()+1);
		}
		else
		{	Name.Format(_T("%s %s[%04d]"), sName, AOIDataDefine.GetMarkText(), MarkPtr->GetRgnIndex_Panel()+1);	}
		str2 = Name;
		Name.Format(_T("%s[ID:%05d]"), str2, MarkPtr->GetMarkUniqueID()+1);
	}	
	if ( SaveLogOper_Func(_T("Mark"), Name, Content) == false )
	//if ( SaveLogOper_Func(LOG_OPER_MARK_SCORE, Name, Content) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_BarcodeFunc(CAOIBarcode *BarcodePtr, LPCTSTR Content)//儲存操作訊息-條碼函式
{
	if ( NULL == BarcodePtr ) { return false; }
	CString str;
	CString str2;
	CString sName;
	CString Name=BarcodePtr->GetBarcodeFullName();
	CAOIPanel *PanelPtr=BarcodePtr->GetBarcodePanelPtr();
	CAOIBoard *BoardPtr=BarcodePtr->GetBarcodeBoardPtr();
	if ( NULL != PanelPtr )
	{	
		str.Format(_T("%s_%04d"), AOIDataDefine.GetPanelText(), PanelPtr->GetPanelIndex_Project()+1);	
		if ( sName.GetLength() == 0 ) { sName = str; }
		else 
		{ 
			str2 = sName;
			sName.Format(_T("%s %s"), str2, str); 
		}
		if ( NULL != BoardPtr )
		{	
			str.Format(_T("%s_%04d"), AOIDataDefine.GetBoardText(), BoardPtr->GetBoardIndex_Panel()+1);	
			if ( sName.GetLength() == 0 ) { sName = str; }
			else 
			{ 
				str2 = sName;
				sName.Format(_T("%s %s"), str2, str); 
			}
			Name.Format(_T("%s %s[%04d]"), sName, AOIDataDefine.GetBarcodeText(), BarcodePtr->GetRgnIndex_Board()+1);
		}
		else
		{	Name.Format(_T("%s %s[%04d]"), sName, AOIDataDefine.GetBarcodeText(), BarcodePtr->GetRgnIndex_Panel()+1);	}
		str2 = Name;
		Name.Format(_T("%s[ID:%05d]"), str2, BarcodePtr->GetBarcodeUniqueID()+1);
	}		
	if ( SaveLogOper_Func(_T("Barcode"), Name, Content) == false )
	//if ( SaveLogOper_Func(LOG_OPER_BARCODE_SCORE, Name, Content) == false )
	{	return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_ComponentFunc(CAOIComponent *ComponentPtr, LPCTSTR Content)//儲存操作訊息-零件函式
{
	if ( NULL == ComponentPtr ) { return false; }
	CString Name=ComponentPtr->GetComponentFullName();
	if ( SaveLogOper_Func(_T("Component"), Name, Content) == false )
	//if ( SaveLogOper_Func(LOG_OPER_COMPONENT_SCORE, Name, Content) == false )
	{	return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_PartGroupFunc(CAOIPartGroup *PartGroupPtr, LPCTSTR Content)//儲存操作訊息-元件群組函式		
{
	if ( NULL == PartGroupPtr ) { return false; }
	CString Name;
	Name.Format(_T("Part_Group_%04d"), PartGroupPtr->GetPartGroupIndex()+1);
	if ( SaveLogOper_Func(_T("Part Group"), Name, Content) == false )
	//if ( SaveLogOper_Func(LOG_OPER_PART_GROUP_SCORE, Name, Content) == false )
	{	return false; }		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_ModelFunc(CAOIModel *ModelPtr, LPCTSTR Content)//儲存操作訊息-模組函式
{
	if ( NULL == ModelPtr ) { return false; }	
	CString ShowName = ModelPtr->GetModelName();	
	return SaveLogOper_Func(_T("Model"), ShowName, Content);		
	//return SaveLogOper_Func(LOG_OPER_MODEL_SCORE, ShowName, Content);		
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_LandFunc(CAOILand *LandPtr, LPCTSTR Content)//儲存操作訊息-特徵框函式	
{
	if ( NULL == LandPtr ) { return false; }
	CString ShowName;
	CAOIModel *ModelPtr = LandPtr->GetLandModelPtr();
	const unsigned int LandIndex = LandPtr->GetLandIndex();	
	if ( NULL == ModelPtr )
	{	ShowName.Format(_T("Land%04d"), LandIndex+1);	}
	else
	{
		CString ModelName = ModelPtr->GetModelName();
		ShowName.Format(_T("%s_Land%04d"), ModelName, LandIndex+1);
	}
	return SaveLogOper_Func(_T("Land"), ShowName, Content);		
	//return SaveLogOper_Func(LOG_OPER_LAND_SCORE, ShowName, Content);		
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_WndFunc(CAOIWnd *WndPtr, LPCTSTR Content)//儲存操作訊息-檢測框函式
{
	if ( NULL == WndPtr ) { return false; }	
	CString ShowName;
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();
	const unsigned int WndIndex = WndPtr->GetWndIndex();
	if ( NULL == ModelPtr )
	{	ShowName.Format(_T("Wnd%04d"), WndIndex+1);	}
	else
	{
		CString ModelName = ModelPtr->GetModelName();
		CAOIRgn *RgnPtr = ModelPtr->GetModelAttachedPtr();		
		if ( NULL != RgnPtr )		
		{
			AOI_OBJ_TYPE AttachedType = ModelPtr->GetModelAttachedType();
			if ( AOI_OBJ_COMPONENT != AttachedType )
			{	ModelName = RgnPtr->GetRgnDerivedName();	}
		}
		ShowName.Format(_T("%s_Wnd%04d"), ModelName, WndIndex+1);
	}
	return SaveLogOper_Func(_T("Wnd"), ShowName, Content);	
	//return SaveLogOper_Func(LOG_OPER_WND_SCORE, ShowName, Content);	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_WndRoiFunc(CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, LPCTSTR Content)//儲存操作訊息-檢測子框函式	
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == WndRoiPtr ) { return false; }

	CString ShowName;
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();	
	const unsigned int WndIndex = WndPtr->GetWndIndex();
	const unsigned int WndRoiIndex = WndRoiPtr->GetWndRoiIndex();
	if ( NULL == ModelPtr )
	{	ShowName.Format(_T("Wnd%04d_Roi%04d"), WndIndex+1, WndRoiIndex+1);	}
	else
	{
		CString ModelName = ModelPtr->GetModelName();
		ShowName.Format(_T("%s_Wnd%04d_Roi%04d"), ModelName, WndIndex+1, WndRoiIndex+1);
	}
	return SaveLogOper_Func(_T("Wnd_Roi"), ShowName, Content);	
	//return SaveLogOper_Func(LOG_OPER_WND_ROI_SCORE, ShowName, Content);	
}
//-------------------------------------------------------------------------------------//
bool CAOIDataCollect::SaveLogOper_WndMaskFunc(CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr, LPCTSTR Content)//儲存操作訊息-遮罩框函式	
{
	if ( NULL == WndPtr ) { return false; }
	if ( NULL == WndMaskPtr ) { return false; }

	CString ShowName;
	CAOIModel *ModelPtr = WndPtr->GetWndModelPtr();	
	const unsigned int WndIndex = WndPtr->GetWndIndex();
	const unsigned int WndMaskIndex = WndMaskPtr->GetWndMaskIndex();
	if ( NULL == ModelPtr )
	{	ShowName.Format(_T("Wnd%04d_Mask%04d"), WndIndex+1, WndMaskIndex+1);	}
	else
	{
		CString ModelName = ModelPtr->GetModelName();
		ShowName.Format(_T("%s_Wnd%04d_Mask%04d"), ModelName, WndIndex+1, WndMaskIndex+1);
	}
	return SaveLogOper_Func(_T("Wnd_Mask"), ShowName, Content);	
	//return SaveLogOper_Func(LOG_OPER_WND_MASK_SCORE, ShowName, Content);	
}
//-------------------------------------------------------------------------------------//