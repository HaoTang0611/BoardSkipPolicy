#ifndef _LOG_OPER_CTRL_H_
#define _LOG_OPER_CTRL_H_
//-------------------------------------------------------------------------------------//
#include "AOIProject.h"
//-------------------------------------------------------------------------------------//
typedef struct tagLogOperModify
{
	CString    m_sKey;
	CString    m_sOld;
	CString    m_sNew;	
	DWORD_PTR  m_dwData;	
	tagLogOperModify()
	{	m_dwData = 0;	}	
	
	LPCTSTR GetKey() const { return m_sKey; }
	void SetKey(LPCTSTR val) { m_sKey = val; }

	LPCTSTR GetOld() const { return m_sOld; }
	void SetOld(LPCTSTR val) { m_sOld = val; }

	LPCTSTR GetNew() const { return m_sNew; }
	void SetNew(LPCTSTR val) { m_sNew = val; }

	DWORD_PTR GetData() const { return m_dwData; }
	void SetData(DWORD_PTR val) { m_dwData = val; }

	bool ChkBol(bool Old, bool New, DWORD_PTR dwData)
	{	
		m_dwData = dwData;
		if ( Old == New ) { return true; }
		m_sOld = AOIDataDefine.GetEnableDisableText(Old);
		m_sNew = AOIDataDefine.GetEnableDisableText(New);
		return false;
	}
	bool ChkInt(int Old, int New, DWORD_PTR dwData)
	{	
		m_dwData = dwData;
		if ( Old == New ) { return true; }
		m_sOld.Format(_T("%d"), Old);
		m_sNew.Format(_T("%d"), New);
		return false;
	}
	bool ChkDbl(double Old, double New, DWORD_PTR dwData)
	{
		m_dwData = dwData;		
		if ( Old == New ) { return true; }	
		//避免極小數點不同
		const int nOld=(int)(Old*1000.0+0.5);
		const int nNew=(int)(New*1000.0+0.5);
		if ( nOld == nNew ) { return true; }
		m_sOld.Format(_T("%.2f"), Old);
		m_sNew.Format(_T("%.2f"), New);		
		return false;
	}
	bool ChkStr(LPCTSTR Old, LPCTSTR New, DWORD_PTR dwData)
	{
		m_dwData = dwData;
		if ( _tcscmp(Old, New) == 0 ) { return true; }
		m_sOld = Old;
		m_sNew = New;		
		return false;
	}
	bool SetBol(LPCTSTR Key, bool Old, bool New, DWORD_PTR dwData)
	{
		m_sKey = Key;
		m_dwData = dwData;
		m_sOld = AOIDataDefine.GetEnableDisableText(Old);
		m_sNew = AOIDataDefine.GetEnableDisableText(New);
		return true;
	}
	bool SetInt(LPCTSTR Key, int Old, int New, DWORD_PTR dwData)
	{
		m_sKey = Key;
		m_dwData = dwData;
		m_sOld.Format(_T("%d"), Old);
		m_sNew.Format(_T("%d"), New);
		return true;
	}
	bool SetDbl(LPCTSTR Key, double Old, double New, DWORD_PTR dwData)
	{
		m_sKey = Key;
		m_dwData = dwData;
		m_sOld.Format(_T("%.2f"), Old);
		m_sNew.Format(_T("%.2f"), New);		
		return true;
	}
	bool SetStr(LPCTSTR Key, LPCTSTR Old, LPCTSTR New, DWORD_PTR dwData)
	{
		m_sKey = Key;		
		m_sOld = Old;
		m_sNew = New;
		m_dwData = dwData;
		return true;
	}
} TLogOperModify, *PLogOperModify;
//-------------------------------------------------------------------------------------//
class CLogOperCtrl
{
private:
	//---------------------------------------------------------------------------------//	
	char                       m_ReadBufferA[2048];
	wchar_t                    m_ReadBufferW[2048];
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//	
	bool                       CheckStrValid(LPCTSTR str);
	//---------------------------------------------------------------------------------//	
	bool                       CheckFdPtr(const CAOIFd *Ptr);
	bool                       CheckBoxPtr(const CAOIBox *Ptr);
	bool                       CheckWndPtr(const CAOIWnd *Ptr);
	bool                       CheckLandPtr(const CAOILand *Ptr);
	bool                       CheckMarkPtr(const CAOIMark *Ptr);
	bool                       CheckRGBVPtr(const CColorRGBV *Ptr);
	bool                       CheckModelPtr(const CAOIModel *Ptr);
	bool                       CheckPanelPtr(const CAOIPanel *Ptr);	
	bool                       CheckBoardPtr(const CAOIBoard *Ptr);			
	bool                       CheckBarcodePtr(const CAOIBarcode *Ptr);	
	bool                       CheckProjectPtr(const CAOIProject *Ptr);	
	bool                       CheckComponentPtr(const CAOIComponent *Ptr);
	//---------------------------------------------------------------------------------//
	bool                       FormatContent(LPCTSTR sOper, LPCTSTR sKey, int nVal, CString &str);
	bool                       FormatContent(LPCTSTR sOper, LPCTSTR sKey, double fVal, CString &str);
	bool                       FormatContent(LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal, CString &str);
	bool                       FormatContent(LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew, CString &str);
	bool                       FormatContent(LPCTSTR sOper, LPCTSTR sAlg, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew, CString &str);
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	CLogOperCtrl();
	~CLogOperCtrl();
	//---------------------------------------------------------------------------------//
	bool                       CloseLogOperFile(FILE *&pfile);
	FILE*                      OpenLogOperFile(LPCTSTR filename);
	bool                       CheckLogOperFileEnd(FILE *pfile);
	bool                       ReadLogOperText(FILE *pfile, CString &Str);
	bool                       ReadLogOperText(FILE *pfile, LPWSTR Buffer, int szBuffer);
	int                        CheckLogOperContentCount(LPCTSTR Content);
	int                        CheckLogOperContentCountA(LPCSTR Content);
	int                        CheckLogOperContentCountW(LPCWSTR Content);
	bool                       DecodeLogOperContent(LPCTSTR Content, std::vector<CString> &List);
	bool                       DecodeLogOperContentA(LPCSTR Content, std::vector<CString> &List);
	bool                       DecodeLogOperContentW(LPCWSTR Content, std::vector<CString> &List);
	//---------------------------------------------------------------------------------//
	bool                       AddLogOperModified_Key(LPCTSTR Key, TLogOperModify &Obj, std::vector<TLogOperModify> &List);
	bool                       CmpAndAddLogOperModif_OBJ(TLogOperModify &Obj, std::vector<TLogOperModify> &List);
	bool                       CmpAndAddLogOperModif_BOL(LPCTSTR Key, int Old, int New, std::vector<TLogOperModify> &List, DWORD_PTR dwData=0);
	bool                       CmpAndAddLogOperModif_INT(LPCTSTR Key, int Old, int New, std::vector<TLogOperModify> &List, DWORD_PTR dwData=0);	
	bool                       CmpAndAddLogOperModif_CLR(LPCTSTR Key, int Old, int New, std::vector<TLogOperModify> &List, DWORD_PTR dwData=0);
	bool                       CmpAndAddLogOperModif_DBL(LPCTSTR Key, double Old, double New, std::vector<TLogOperModify> &List, DWORD_PTR dwData=0);	
	bool                       CmpAndAddLogOperModif_STR(LPCTSTR Key, LPCTSTR Old, LPCTSTR New, std::vector<TLogOperModify> &List, DWORD_PTR dwData=0);
	bool                       CmpAndAddLogOperModif_STR(LPCTSTR Key, const std::string &Old, const std::string &New, std::vector<TLogOperModify> &List, DWORD_PTR dwData=0);
	bool                       CmpAndAddLogOperModif_STR(LPCTSTR Key, const std::wstring &Old, const std::wstring &New, std::vector<TLogOperModify> &List, DWORD_PTR dwData=0);
	//---------------------------------------------------------------------------------//
	bool                       SaveLogOper_SystemContent(LPCTSTR Content);//儲存操作訊息-系統函式
	//---------------------------------------------------------------------------------//	
	bool                       SaveLogUserLogDelAdd(LPCTSTR Oper, LPCTSTR Val = NULL);//儲存操作訊息使用者
	bool                       SaveLogUserLogInOut(LPCTSTR Oper, LPCTSTR Val=NULL);//儲存操作訊息使用者
	bool                       SaveLogUserLogIn(const TUserNode &User);//儲存操作訊息使用者-登入
	bool                       SaveLogUserLogOut(const TUserNode &User);//儲存操作訊息使用者-登出
	//---------------------------------------------------------------------------------//
	bool                       SaveLogSystemComparParam(const TSystemParameter &OldParam, const TSystemParameter &NewParam);//儲存操作訊息-系統參數修改
	//---------------------------------------------------------------------------------//
	bool                       SaveLogProject(CAOIProject *ProjectPtr, LPCTSTR Oper, LPCTSTR Val=NULL);//儲存操作訊息專案
	bool                       SaveLogProjectContent(CAOIProject *ProjectPtr, LPCTSTR Content);//儲存操作訊息-專案內容	
	bool                       SaveLogProjectOperate(CAOIProject *ProjectPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);//儲存操作訊息-專案修改		

	bool                       SaveLogProjectNew(CAOIProject *ProjectPtr, LPCTSTR filename);//儲存操作訊息專案-新建
	bool                       SaveLogProjectOpen(CAOIProject *ProjectPtr, LPCTSTR filename);//儲存操作訊息專案-開檔
	bool                       SaveLogProjectSave(CAOIProject *ProjectPtr, LPCTSTR filename);//儲存操作訊息專案-存檔
	bool                       SaveLogProjectClose(CAOIProject *ProjectPtr, LPCTSTR filename);//儲存操作訊息專案-關檔
	bool                       SaveLogProjectCompare(CAOIProject *OldPtr, CAOIProject *NewPtr);//儲存操作訊息-專案比較
	bool                       SaveLogProjectComparParam(CAOIProject *ProjectPtr, const TProjectParameter &Param);//儲存操作訊息-專案修改
	//---------------------------------------------------------------------------------//
	bool                       SaveLogPanelContent(CAOIPanel *PanelPtr, LPCTSTR Content);//儲存操作訊息-整板內容
	bool                       SaveLogPanelCompare(CAOIPanel *OldPtr, CAOIPanel *NewPtr);//儲存操作訊息-整板比較
	bool                       SaveLogPanelOperate(CAOIPanel *PanelPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);//儲存操作訊息-整板修改		
	//---------------------------------------------------------------------------------//
	bool                       SaveLogBoardContent(CAOIBoard *BoardPtr, LPCTSTR Content);//儲存操作訊息-單板內容
	bool                       SaveLogBoardCompare(CAOIBoard *OldPtr, CAOIBoard *NewPtr);//儲存操作訊息-單板比較
	bool                       SaveLogBoardOperate(CAOIBoard *BoardPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);//儲存操作訊息-單板修改
	//---------------------------------------------------------------------------------//
	bool                       SaveLogFdContent(CAOIFd *FdPtr, LPCTSTR Content);//儲存操作訊息-定位點內容
	bool                       SaveLogFdCompare(CAOIFd *OldPtr, CAOIFd *NewPtr);//儲存操作訊息-定位點比較
	bool                       SaveLogFdOperate(CAOIFd *FdPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);//儲存操作訊息-定位點修改
	//---------------------------------------------------------------------------------//
	bool                       SaveLogMarkContent(CAOIMark *MarkPtr, LPCTSTR Content);//儲存操作訊息-特徵內容
	bool                       SaveLogMarkCompare(CAOIMark *OldPtr, CAOIMark *NewPtr);//儲存操作訊息-特徵比較
	bool                       SaveLogMarkOperate(CAOIMark *MarkPtr, LPCTSTR sOper, LPCTSTR sKey, int nVal);//儲存操作訊息-特徵操作	
	bool                       SaveLogMarkOperate(CAOIMark *MarkPtr, LPCTSTR sOper, LPCTSTR sKey, double fVal);//儲存操作訊息-特徵操作	
	bool                       SaveLogMarkOperate(CAOIMark *MarkPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal);//儲存操作訊息-特徵操作	
	bool                       SaveLogMarkOperate(CAOIMark *MarkPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);//儲存操作訊息-特徵操作	
	//---------------------------------------------------------------------------------//
	bool                       SaveLogBarcodeContent(CAOIBarcode *BarcodePtr, LPCTSTR Content);//儲存操作訊息-條碼內容
	bool                       SaveLogBarcodeCompare(CAOIBarcode *OldPtr, CAOIBarcode *NewPtr);//儲存操作訊息-條碼比較
	bool                       SaveLogBarcodeOperate(CAOIBarcode *BarcodePtr, LPCTSTR sOper, LPCTSTR sKey, int nVal);//儲存操作訊息-條碼操作	
	bool                       SaveLogBarcodeOperate(CAOIBarcode *BarcodePtr, LPCTSTR sOper, LPCTSTR sKey, double fVal);//儲存操作訊息-條碼操作	
	bool                       SaveLogBarcodeOperate(CAOIBarcode *BarcodePtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal);//儲存操作訊息-條碼操作	
	bool                       SaveLogBarcodeOperate(CAOIBarcode *BarcodePtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);//儲存操作訊息-條碼操作	
	//---------------------------------------------------------------------------------//
	bool                       SaveLogComponentContent(CAOIComponent *ComponentPtr, LPCTSTR Content);//儲存操作訊息-零件內容
	bool                       SaveLogComponentCompare(CAOIComponent *OldPtr, CAOIComponent *NewPtr);//儲存操作訊息-零件比較
	bool                       SaveLogComponentOperate(CAOIComponent *ComponentPtr, LPCTSTR sOper, LPCTSTR sKey, int nVal);//儲存操作訊息-零件操作
	bool                       SaveLogComponentOperate(CAOIComponent *ComponentPtr, LPCTSTR sOper, LPCTSTR sKey, double fVal);//儲存操作訊息-零件操作
	bool                       SaveLogComponentOperate(CAOIComponent *ComponentPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal);//儲存操作訊息-零件操作		
	bool                       SaveLogComponentOperate(CAOIComponent *ComponentPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);//儲存操作訊息-零件修改		
	//---------------------------------------------------------------------------------//		
	bool                       SaveLogPartGroupContent(CAOIPartGroup *PartGroupPtr, LPCTSTR Content);//儲存操作訊息-元件群組內容
	bool                       SaveLogPartGroupCompare(CAOIPartGroup *OldPtr, CAOIPartGroup *NewPtr);//儲存操作訊息-元件群組比較
	bool                       SaveLogPartGroupOperate(CAOIPartGroup *PartGroupPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);//儲存操作訊息-元件群組修改
	bool                       SaveLogPartGroupNodeCompare(TPartGroupNode *OldPtr, TPartGroupNode *NewPtr, std::vector<TLogOperModify> &ChangeList);//儲存操作訊息-元件群組比較
	//---------------------------------------------------------------------------------//
	bool                       SaveLogBoxCompare(CAOIBox *OldPtr, CAOIBox *NewPtr, std::vector<TLogOperModify> &ChangeList);//儲存操作訊息模組-框比較	
	//---------------------------------------------------------------------------------//
	bool                       SaveLogModelContent(CAOIModel *ModelPtr, LPCTSTR Content);//儲存操作訊息模組-模組內容	
	bool                       SaveLogModelCompare(CAOIModel *OldPtr, CAOIModel *NewPtr);//儲存操作訊息模組-模組比較
	bool                       SaveLogModelOperate(CAOIModel *ModelPtr, LPCTSTR sOper, LPCTSTR sKey, int nVal);//儲存操作訊息-模組操作
	bool                       SaveLogModelOperate(CAOIModel *ModelPtr, LPCTSTR sOper, LPCTSTR sKey, double fVal);//儲存操作訊息-模組操作
	bool                       SaveLogModelOperate(CAOIModel *ModelPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal);//儲存操作訊息-模組操作		
	bool                       SaveLogModelOperate(CAOIModel *ModelPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);//儲存操作訊息-模組修改		
	bool                       SaveLogModelWndDefectItemCompare(CAOIModel *ModelPtr, LPCTSTR Name, const CWndDefectItem &Old, const CWndDefectItem &New);//儲存操作訊息模組-檢測框瑕疵數比較		
	//---------------------------------------------------------------------------------//
	bool                       SaveLogModelLandContent(CAOILand *LandPtr, LPCTSTR Content);//儲存操作訊息模組-特徵框內容
	bool                       SaveLogModelLandCompare(CAOILand *OldPtr, CAOILand *NewPtr);//儲存操作訊息模組-特徵框-比較
	bool                       SaveLogModelLandOperate(CAOILand *LandPtr, LPCTSTR sOper, LPCTSTR sKey, int nVal);//儲存操作訊息-特徵框操作
	bool                       SaveLogModelLandOperate(CAOILand *LandPtr, LPCTSTR sOper, LPCTSTR sKey, double fVal);//儲存操作訊息-特徵框操作
	bool                       SaveLogModelLandOperate(CAOILand *LandPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal);//儲存操作訊息-特徵框操作		
	bool                       SaveLogModelLandOperate(CAOILand *LandPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);//儲存操作訊息-特徵框修改		
	//---------------------------------------------------------------------------------//	
	bool                       SaveLogModelWndContent(CAOIWnd *WndPtr, LPCTSTR Content);//儲存操作訊息模組-檢測框內容
	bool                       SaveLogModelWndCompare(CAOIWnd *OldPtr, CAOIWnd *NewPtr);//儲存操作訊息模組-檢測框比較
	bool                       SaveLogModelWndOperate(CAOIWnd *WndPtr, LPCTSTR sOper, LPCTSTR sKey, int nVal);//儲存操作訊息-檢測框操作
	bool                       SaveLogModelWndOperate(CAOIWnd *WndPtr, LPCTSTR sOper, LPCTSTR sKey, double fVal);//儲存操作訊息-檢測框操作
	bool                       SaveLogModelWndOperate(CAOIWnd *WndPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal);//儲存操作訊息-檢測框操作		
	bool                       SaveLogModelWndOperate(CAOIWnd *WndPtr, LPCTSTR sOper, LPCTSTR sAlg, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);//儲存操作訊息-檢測框修改		
	//---------------------------------------------------------------------------------//	
	bool                       SaveLogModelWndRoiContent(CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, LPCTSTR Content);//儲存操作訊息-檢測子框內容
	bool                       SaveLogModelWndRoiOperate(CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, LPCTSTR sOper, LPCTSTR sKey, int nVal);//儲存操作訊息-檢測子框操作
	bool                       SaveLogModelWndRoiOperate(CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, LPCTSTR sOper, LPCTSTR sKey, double fVal);//儲存操作訊息-檢測子框操作
	bool                       SaveLogModelWndRoiOperate(CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal);//儲存操作訊息-檢測子框操作		
	bool                       SaveLogModelWndRoiOperate(CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, LPCTSTR sOper, LPCTSTR sAlg, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);//儲存操作訊息-檢測子框修改		
	//---------------------------------------------------------------------------------//	
	bool                       SaveLogModelWndMaskContent(CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr, LPCTSTR Content);//儲存操作訊息-遮罩框內容
	bool                       SaveLogModelWndMaskOperate(CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr, LPCTSTR sOper, LPCTSTR sKey, int nVal);//儲存操作訊息-遮罩框操作
	bool                       SaveLogModelWndMaskOperate(CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr, LPCTSTR sOper, LPCTSTR sKey, double fVal);//儲存操作訊息-遮罩框操作
	bool                       SaveLogModelWndMaskOperate(CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sVal);//儲存操作訊息-遮罩框操作		
	bool                       SaveLogModelWndMaskOperate(CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr, LPCTSTR sOper, LPCTSTR sAlg, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);//儲存操作訊息-遮罩框修改		
	//---------------------------------------------------------------------------------//
	bool                       SaveLogModelWndAlgCompare(CAOIWnd *WndPtr, CAlgParam *OldPtr, CAlgParam *NewPtr);//儲存操作訊息模組-演算法比較
	bool                       SaveLogModelWndAlgBinaryCompare(CAOIWnd *WndPtr, CAlgBinaryParam *OldPtr, CAlgBinaryParam *NewPtr);//儲存操作訊息模組-檢測框-演算法-2值化	
	//---------------------------------------------------------------------------------//
	bool                       SaveLogModelWndAlgPatternContent(CAOIWnd *WndPtr, LPCTSTR sOper);//儲存操作訊息模組-檢測框-演算法-樣板內容
	bool                       SaveLogModelWndAlgPatternContentAdd(CAOIWnd *WndPtr);//儲存操作訊息模組-檢測框-演算法-樣板-新增
	bool                       SaveLogModelWndAlgPatternContentText(CAOIWnd *WndPtr);//儲存操作訊息模組-檢測框-演算法-樣板-文字
	bool                       SaveLogModelWndAlgPatternContentDelete(CAOIWnd *WndPtr);//儲存操作訊息模組-檢測框-演算法-樣板-刪除
	bool                       SaveLogModelWndAlgPatternContentModify(CAOIWnd *WndPtr);//儲存操作訊息模組-檢測框-演算法-樣板-修改
	bool                       SaveLogModelWndAlgPatternContentClearAll(CAOIWnd *WndPtr);//儲存操作訊息模組-檢測框-演算法-樣板-清除全部
	//---------------------------------------------------------------------------------//	
	bool                       SaveLogModelWndAlgColorFilterContent(CAOIWnd *WndPtr, LPCTSTR sOper);//儲存操作訊息模組-檢測框-演算法-抽色內容	
	bool                       SaveLogModelWndAlgColorFilterOperate(CAOIWnd *WndPtr, LPCTSTR sOper);//儲存操作訊息-檢測框-演算法-抽色修改		
	bool                       SaveLogModelWndAlgColorFilterOperate(CAOIWnd *WndPtr, LPCTSTR sOper, LPCTSTR sKey, LPCTSTR sOld, LPCTSTR sNew);//儲存操作訊息-檢測框-演算法-抽色修改		
	bool                       SaveLogModelWndAlgColorFilterOperateExtractWndColor(CAOIWnd *WndPtr);//儲存操作訊息-檢測框-演算法-抽色-檢測框顏色
	bool                       SaveLogModelWndAlgColorFilterCompare(CAOIWnd *WndPtr, int Index, CColorRGBV *OldPtr, CColorRGBV *NewPtr);//儲存操作訊息模組-檢測框-演算法-抽色比較
	//---------------------------------------------------------------------------------//
	bool                       SaveLogProjectPanelSelected(CAOIProject *ProjrectPtr, LPCTSTR Oper, LPCTSTR Val=NULL);//儲存操作訊息專案整板選取
	bool                       SaveLogProjectPanelSelectedCreate(CAOIProject *ProjrectPtr);//儲存操作訊息專案整板選取-建立
	bool                       SaveLogProjectPanelSelectedDelete(CAOIProject *ProjrectPtr);//儲存操作訊息專案整板選取-刪除
	bool                       SaveLogProjectPanelSelectedBypassed(CAOIProject *ProjrectPtr);//儲存操作訊息專案整板選取-不檢測
	bool                       SaveLogProjectPanelSelectedMirrorX(CAOIProject *ProjrectPtr);//儲存操作訊息專案整板選取-鏡射X
	bool                       SaveLogProjectPanelSelectedMirrorY(CAOIProject *ProjrectPtr);//儲存操作訊息專案整板選取-鏡射Y	
	bool                       SaveLogProjectPanelSelectedRotate(CAOIProject *ProjrectPtr, double Angle);//儲存操作訊息專案整板選取-旋轉
	bool                       SaveLogProjectPanelSelectedMove(CAOIProject *ProjrectPtr, double X, double Y);//儲存操作訊息專案整板選取-移動
	bool                       SaveLogProjectPanelSelectedBarcodeCodeIndex(CAOIProject *ProjrectPtr, unsigned int index);//儲存操作訊息專案整板選取-條碼序號
	bool                       SaveLogProjectPanelSelectedBarcodeDeviceIndex(CAOIProject *ProjrectPtr, unsigned int index);//儲存操作訊息專案整板選取-條碼機序號	
	//---------------------------------------------------------------------------------//
	bool                       SaveLogProjectBoardSelected(CAOIProject *ProjrectPtr, LPCTSTR Oper, LPCTSTR val=NULL);//儲存操作訊息專案單板選取
	bool                       SaveLogProjectBoardSelectedCreate(CAOIProject *ProjrectPtr);//儲存操作訊息專案單板選取-建立
	bool                       SaveLogProjectBoardSelectedDelete(CAOIProject *ProjrectPtr);//儲存操作訊息專案單板選取-刪除	
	bool                       SaveLogProjectBoardSelectedBypassed(CAOIProject *ProjrectPtr);//儲存操作訊息專案單板選取-不檢測	
	bool                       SaveLogProjectBoardSelectedMirrorX(CAOIProject *ProjrectPtr);//儲存操作訊息專案單板選取-鏡射X
	bool                       SaveLogProjectBoardSelectedMirrorY(CAOIProject *ProjrectPtr);//儲存操作訊息專案單板選取-鏡射Y
	bool                       SaveLogProjectBoardSelectedRotate(CAOIProject *ProjrectPtr, double val);//儲存操作訊息專案單板選取-旋轉
	bool                       SaveLogProjectBoardSelectedMove(CAOIProject *ProjrectPtr, double X, double Y);//儲存操作訊息專案單板選取-移動
	bool                       SaveLogProjectBoardSelectedBarcodeCodeIndex(CAOIProject *ProjrectPtr, unsigned int index);//儲存操作訊息專案單板選取-條碼序號
	bool                       SaveLogProjectBoardSelectedBarcodeDeviceIndex(CAOIProject *ProjrectPtr, unsigned int index);//儲存操作訊息專案單板選取-條碼機序號	
	//---------------------------------------------------------------------------------//	
	bool                       SaveLogProjectFd(CAOIFd *FdPtr, LPCTSTR Oper, LPCTSTR val=NULL);//儲存操作訊息專案定位點
	bool                       SaveLogProjectFdAdd(CAOIFd *FdPtr);//儲存操作訊息專案定位點新增
	bool                       SaveLogProjectFdSelected(const std::vector<CAOIFd*> &FdList, LPCTSTR Oper, LPCTSTR val=NULL);//儲存操作訊息專案定位點選取		
	bool                       SaveLogProjectFdSelectedAdd(const std::vector<CAOIFd*> &FdList);//儲存操作訊息專案定位點選取-複製
	bool                       SaveLogProjectFdSelectedClone(const std::vector<CAOIFd*> &FdList);//儲存操作訊息專案定位點選取-複製
	bool                       SaveLogProjectFdSelectedDelete(CAOIProject *ProjrectPtr);//儲存操作訊息專案定位點選取-刪除
	bool                       SaveLogProjectFdSelectedDelete(const std::vector<CAOIFd*> &FdList);//儲存操作訊息專案定位點選取-刪除	
	//---------------------------------------------------------------------------------//	
	bool                       SaveLogProjectBarcodeSelected(CAOIProject *ProjrectPtr, LPCTSTR Oper, LPCTSTR val=NULL);//儲存操作訊息專案條碼選取
	bool                       SaveLogProjectBarcodeSelectedDelete(CAOIProject *ProjrectPtr);//儲存操作訊息專案條碼選取-刪除
	bool                       SaveLogProjectBarcodeSelectedRotate(CAOIProject *ProjrectPtr, double Angle);//儲存操作訊息專案條碼選取-旋轉
	bool                       SaveLogProjectBarcodeSelectedLocalBasePlaneID(CAOIProject *ProjrectPtr, int val);//儲存操作訊息專案條碼選取-局部基準面
	//---------------------------------------------------------------------------------//	
	bool                       SaveLogProjectComponentSelected(CAOIProject *ProjrectPtr, LPCTSTR Oper, LPCTSTR val=NULL);//儲存操作訊息專案零件選取
	bool                       SaveLogProjectComponentSelectedCreate(CAOIProject *ProjrectPtr);//儲存操作訊息專案零件選取-建立
	bool                       SaveLogProjectComponentSelectedDelete(CAOIProject *ProjrectPtr);//儲存操作訊息專案零件選取-刪除
	bool                       SaveLogProjectComponentSelectedMirrorX(CAOIProject *ProjrectPtr);//儲存操作訊息專案零件選取-鏡射X
	bool                       SaveLogProjectComponentSelectedMirrorY(CAOIProject *ProjrectPtr);//儲存操作訊息專案零件選取-鏡射Y
	bool                       SaveLogProjectComponentSelectedBypassed(CAOIProject *ProjrectPtr);//儲存操作訊息專案零件選取-不檢測
	bool                       SaveLogProjectComponentSelectedBypass3D(CAOIProject *ProjrectPtr);//儲存操作訊息專案零件選取-不檢測3D
	bool                       SaveLogProjectComponentSelectedXBoardUnit(CAOIProject *ProjrectPtr);//儲存操作訊息專案零件選取-報廢件
	bool                       SaveLogProjectComponentSelectedModelIsolated(CAOIProject *ProjrectPtr);//儲存操作訊息專案零件選取-模組隔離
	bool                       SaveLogProjectComponentSelectedRotate(CAOIProject *ProjrectPtr, double Angle);//儲存操作訊息專案零件選取-旋轉
	bool                       SaveLogProjectComponentSelectedMove(CAOIProject *ProjrectPtr, double X, double Y);//儲存操作訊息專案零件選取-移動
	bool                       SaveLogProjectComponentSelectedPartNumber(CAOIProject *ProjrectPtr, LPCTSTR PartNumber);//儲存操作訊息專案零件選取-料號
	bool                       SaveLogProjectComponentSelectedNozzlName(CAOIProject *ProjrectPtr, LPCTSTR NozzlName);//儲存操作訊息專案零件選取-吸嘴
	bool                       SaveLogProjectComponentSelectedComponentName(CAOIProject *ProjrectPtr, LPCTSTR RefName, LPCTSTR NewName);//儲存操作訊息專案零件選取-名稱
	bool                       SaveLogProjectComponentSelectedMaskExtendSize_Body(CAOIProject *ProjrectPtr, double ExtW, double ExtH);//儲存操作訊息專案零件選取-遮罩外擴尺寸-本體
	bool                       SaveLogProjectComponentSelectedAlarm(CAOIProject *ProjrectPtr, bool bEnable);//儲存操作訊息專案零件選取-停機警報
	bool                       SaveLogProjectComponentSelectedAlarmOnAOI(CAOIProject *ProjrectPtr, bool bEnable);//儲存操作訊息專案零件選取-停機警報
	bool                       SaveLogProjectComponentSelectedAlarmOnARS(CAOIProject *ProjrectPtr, bool bEnable);//儲存操作訊息專案零件選取-維修站顯示
	bool                       SaveLogProjectComponentSelectedDefectCountOnARS(CAOIProject *ProjrectPtr, bool bEnable);//儲存操作訊息專案零件選取-維修站顯示
	bool                       SaveLogProjectComponentSelectedSaveReportARS(CAOIProject *ProjrectPtr, bool bSave);//儲存操作訊息專案零件選取-是否儲存報告-維修站
	bool                       SaveLogProjectComponentSelectedSelfField(CAOIProject *ProjrectPtr, bool bEnable);//儲存操作訊息專案零件選取-專屬區域	
	bool                       SaveLogProjectComponentSelectedSaveWndList(CAOIProject *ProjrectPtr, bool bEnable);//儲存操作訊息專案零件選取-儲存檢測框列表
	bool                       SaveLogProjectComponentSelectedChangeBoard(CAOIProject *ProjrectPtr, CAOIBoard *BoardPtr);//儲存操作訊息專案零件選取-單板編號
	bool                       SaveLogProjectComponentSelectedLocalBasePlaneID(CAOIProject *ProjrectPtr, int BasePlaneID);//儲存操作訊息專案零件選取-局部基準面編號		
	bool                       SaveLogProjectComponentSelectedDataModelParam(CAOIProject *ProjrectPtr, int Param);//儲存操作訊息專案零件選取-資料物件參數
	bool                       SaveLogProjectComponentSelectedDefectAlarmAOI(CAOIProject *ProjrectPtr, DEFECT_PARAM_FROM_MODE FromMode, const CWndDefectItem &DefectItem);//儲存操作訊息專案零件選取-停機警報
	bool                       SaveLogProjectComponentSelectedDefectAlarmARS(CAOIProject *ProjrectPtr, DEFECT_PARAM_FROM_MODE FromMode, const CWndDefectItem &DefectItem);//儲存操作訊息專案零件選取-停機警報
	//---------------------------------------------------------------------------------//		
	bool                       SaveLogModelModifyPos(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOILand *LandPtr, CAOIWndRoi *WndRoiPtr, CAOIWndMask *MaskWndPtr, double dPx, double dPy);//儲存操作訊息模組-框座標
	bool                       SaveLogModelBodyPos(CAOIModel *ModelPtr, CAOIBox *BoxPtr, double dPx, double dPy);//儲存操作訊息模組-框座標
	bool                       SaveLogModelWndPos(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOIWnd *WndPtr, double dPx, double dPy);//儲存操作訊息模組-框座標
	bool                       SaveLogModelLandPos(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOILand *LandPtr, double dPx, double dPy);//儲存操作訊息模組-框座標
	bool                       SaveLogModelWndRoiPos(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, double dPx, double dPy);//儲存操作訊息模組-框座標
	bool                       SaveLogModelWndMaskPos(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOIWndMask *WndMaskPtr, double dPx, double dPy);//儲存操作訊息模組-框座標
	//---------------------------------------------------------------------------------//	
	bool                       SaveLogModelModifySize(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOILand *LandPtr, CAOIWndRoi *WndRoiPtr, CAOIWndMask *MaskWndPtr, const TREGION4D &dPos);//儲存操作訊息模組-框尺寸	
	bool                       SaveLogModelBodySize(CAOIModel *ModelPtr, CAOIBox *BoxPtr, const TREGION4D &dPos);//儲存操作訊息模組-框尺寸
	bool                       SaveLogModelWndSize(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOIWnd *WndPtr, const TREGION4D &dPos);//儲存操作訊息模組-框尺寸
	bool                       SaveLogModelLandSize(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOILand *LandPtr, const TREGION4D &dPos);//儲存操作訊息模組-框尺寸
	bool                       SaveLogModelWndRoiSize(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOIWndRoi *WndRoiPtr, const TREGION4D &dPos);//儲存操作訊息模組-框尺寸
	bool                       SaveLogModelWndMaskSize(CAOIModel *ModelPtr, CAOIBox *BoxPtr, CAOIWnd *WndPtr, CAOIWndMask *MaskWndPtr, const TREGION4D &dPos);//儲存操作訊息模組-框尺寸
	//---------------------------------------------------------------------------------//	
	bool                       SaveLogModelLandFunc(CAOIModel *ModelPtr, CAOILand *LandPtr, LPCTSTR Content);//儲存操作訊息模組-特徵框函式
	bool                       SaveLogModelLandCount(CAOIModel *ModelPtr, CAOILand *LandPtr, int Count);//儲存操作訊息模組-特徵框-數量	
	bool                       SaveLogModelLandPitch(CAOIModel *ModelPtr, CAOILand *LandPtr, double Pitch);//儲存操作訊息模組-特徵框-間距
	bool                       SaveLogModelLandGroupID(CAOIModel *ModelPtr, const std::vector<size_t> &IndexList, int GroupID);//儲存操作訊息模組-特徵框-群組編號
	bool                       SaveLogModelLandAlignID(CAOIModel *ModelPtr, const std::vector<size_t> &IndexList, int AlignID);//儲存操作訊息模組-特徵框-對齊編號	
	bool                       SaveLogModelLandAlignID(CAOIModel *ModelPtr, const std::vector<size_t> &IndexList, const std::vector<int> &IDList);//儲存操作訊息模組-特徵框-對齊編號	
	//---------------------------------------------------------------------------------//	
	bool                       SaveLogModelLandSelected(CAOIModel *ModelPtr, LPCTSTR Oper, LPCTSTR val=NULL);//儲存操作訊息模組-特徵框選取	
	bool                       SaveLogModelLandSelectedAlign(CAOIModel *ModelPtr);//儲存操作訊息模組-特徵框選取-至中
	bool                       SaveLogModelLandSelectedClone(CAOIModel *ModelPtr);//儲存操作訊息模組-特徵框選取-複製
	bool                       SaveLogModelLandSelectedCreate(CAOIModel *ModelPtr);//儲存操作訊息模組-特徵框選取-建立
	bool                       SaveLogModelLandSelectedDelete(CAOIModel *ModelPtr);//儲存操作訊息模組-特徵框選取-刪除
	bool                       SaveLogModelLandSelectedMirrorX(CAOIModel *ModelPtr);//儲存操作訊息模組-特徵框選取-鏡射X
	bool                       SaveLogModelLandSelectedMirrorY(CAOIModel *ModelPtr);//儲存操作訊息模組-特徵框選取-鏡射Y
	bool                       SaveLogModelLandSelectedAlignCenterU(CAOIModel *ModelPtr);//儲存操作訊息模組-特徵框選取-至中U
	bool                       SaveLogModelLandSelectedRotate(CAOIModel *ModelPtr, double Angle);//儲存操作訊息模組-特徵框選取-旋轉
	bool                       SaveLogModelLandSelectedMove(CAOIModel *ModelPtr, double X, double Y);//儲存操作訊息模組-特徵框選取-移動
	//---------------------------------------------------------------------------------//	
	bool                       SaveLogModelWndSelected(CAOIModel *ModelPtr, LPCTSTR Oper, LPCTSTR val=NULL);//儲存操作訊息模組-檢測框選取	
	bool                       SaveLogModelWndSelectedClone(CAOIModel *ModelPtr);//儲存操作訊息模組-檢測框選取-複製
	bool                       SaveLogModelWndSelectedCreate(CAOIModel *ModelPtr);//儲存操作訊息模組-檢測框選取-建立
	bool                       SaveLogModelWndSelectedDelete(CAOIModel *ModelPtr);//儲存操作訊息模組-檢測框選取-刪除		
	bool                       SaveLogModelWndSelectedMirrorX(CAOIModel *ModelPtr);//儲存操作訊息模組-檢測框選取-鏡射X
	bool                       SaveLogModelWndSelectedMirrorY(CAOIModel *ModelPtr);//儲存操作訊息模組-檢測框選取-鏡射Y	
	bool                       SaveLogModelWndSelectedAlignCenter(CAOIModel *ModelPtr);//儲存操作訊息模組-檢測框選取-至中
	bool                       SaveLogModelWndSelectedAlignCenterU(CAOIModel *ModelPtr);//儲存操作訊息模組-檢測框選取-至中U
	bool                       SaveLogModelWndSelectedAlignCenterV(CAOIModel *ModelPtr);//儲存操作訊息模組-檢測框選取-至中V
	bool                       SaveLogModelWndSelectedRotate(CAOIModel *ModelPtr, double Angle);//儲存操作訊息模組-檢測框選取-旋轉 
	bool                       SaveLogModelWndSelectedMove(CAOIModel *ModelPtr, double X, double Y);//儲存操作訊息模組-檢測框選取-移動	
	//---------------------------------------------------------------------------------//		
	bool                       SaveLogWndRoiSelected(CAOIWnd *WndPtr, LPCTSTR Oper, LPCTSTR val=NULL);//儲存操作訊息模組-檢測子框選取	
	bool                       SaveLogWndRoiSelectedClone(CAOIWnd *WndPtr);//儲存操作訊息模組-檢測子框選取-複製
	bool                       SaveLogWndRoiSelectedCreate(CAOIWnd *WndPtr);//儲存操作訊息模組-檢測子框選取-建立
	bool                       SaveLogWndRoiSelectedDelete(CAOIWnd *WndPtr);//儲存操作訊息模組-檢測子框選取-刪除
	bool                       SaveLogWndRoiSelectedClearAll(CAOIWnd *WndPtr);//儲存操作訊息模組-檢測子框選取-清除全部
	//---------------------------------------------------------------------------------//
	bool                       SaveLogWndMaskSelected(CAOIWnd *WndPtr, LPCTSTR Oper, LPCTSTR val=NULL);//儲存操作訊息模組-遮罩框選取	
	bool                       SaveLogWndMaskSelectedClone(CAOIWnd *WndPtr);//儲存操作訊息模組-遮罩框選取-複製
	bool                       SaveLogWndMaskSelectedCreate(CAOIWnd *WndPtr);//儲存操作訊息模組-遮罩框選取-建立
	bool                       SaveLogWndMaskSelectedDelete(CAOIWnd *WndPtr);//儲存操作訊息模組-遮罩框選取-刪除
	bool                       SaveLogWndMaskSelectedClearAll(CAOIWnd *WndPtr);//儲存操作訊息模組-遮罩框選取-清除全部
	bool                       SaveLogWndMaskSelectedRotate(CAOIWnd *WndPtr, double Angle);//儲存操作訊息模組-遮罩框選取-旋轉
	bool                       SaveLogWndMaskSelectedEraseMode(CAOIWnd *WndPtr, bool bErase);//儲存操作訊息模組-遮罩框選取-框外型-清除
	bool                       SaveLogWndMaskSelectedShapeMode(CAOIWnd *WndPtr, BOX_SHAPE_MODE Mode);//儲存操作訊息模組-遮罩框選取-框外型
	bool                       SaveLogWndMaskSelectedShapeParam(CAOIWnd *WndPtr, double Param);//儲存操作訊息模組-遮罩框選取-框外型
	bool                       SaveLogWndMaskSelectedShapeParam2(CAOIWnd *WndPtr, double Param);//儲存操作訊息模組-遮罩框選取-框外型
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
extern CLogOperCtrl LogOperCtrl;
//-------------------------------------------------------------------------------------//
#endif//_LOG_OPER_CTRL_H_
