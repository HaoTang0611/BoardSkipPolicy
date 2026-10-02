#pragma once

// CConnectorPinSettingsWnd 對話方塊
//-------------------------------------------------------------------------------------//
#include "DialogBase.h"
//-------------------------------------------------------------------------------------//
#define CThisListCtrl_73      CJETListCtrl//目前使用的列表控制類別
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CConnectorPinSettingsWnd dialog
//-------------------------------------------------------------------------------------//
class CConnectorPinSettingsWnd : public CBaseDialog
{
	DECLARE_DYNAMIC(CConnectorPinSettingsWnd)

public:
	CConnectorPinSettingsWnd(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CConnectorPinSettingsWnd();

	// 對話方塊資料
	//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONNECTOR_PIN_TABLE_WND };
	//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援
	DECLARE_MESSAGE_MAP()

public:
	bool                       SetModelInfo(CAOIModel *ModelPtr, CAOIWnd *WndPtr, bool bShowResult = false);
	//---------------------------------------------------------------------------------//
protected:
	CThisListCtrl_73	       m_ListCtrl;
	//---------------------------------------------------------------------------------//
	virtual BOOL               OnInitDialog();
	afx_msg void               OnOK();
	afx_msg void               OnTableSetBtn();
	afx_msg void               OnPinSetBtn();
	afx_msg void               OnImportBtn();
	afx_msg void               OnSaveAsBtn();
	afx_msg void               OnLvnItemchangedPinList(NMHDR* pNMHDR, LRESULT* pResult);
	//---------------------------------------------------------------------------------//
private:
	CAOIModel                 *m_ModelPtr;
	CAOILand                  *m_LandPtr;
	CAOIWnd                   *m_WndPtr;
	//---------------------------------------------------------------------------------//
	int                        m_nRow;
	int                        m_nColumn;
	//---------------------------------------------------------------------------------//
	int                        m_nSelectedRow;
	int                        m_nSelectedCol;
	//---------------------------------------------------------------------------------//
	bool                       m_bShowResult;
	//---------------------------------------------------------------------------------//
	std::vector<std::vector<TPOINT2D> >  m_PinTable;       // 標準值
	std::vector<std::vector<TPOINT2D> >  m_PinTableRes;    // 結果值
														   //---------------------------------------------------------------------------------//
	CString                    LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default);
	//---------------------------------------------------------------------------------//
	bool                       ApplyShowResultMode();
	bool                       CheckPinTableSize(const std::vector<std::vector<TPOINT2D> >& PinTable, int nRow, int nCol);
	bool                       InitEmptyPinTableByRef(std::vector<std::vector<TPOINT2D> >& TargetTable, const std::vector<std::vector<TPOINT2D> >& RefTable);
	//---------------------------------------------------------------------------------//
	bool                       InitListCtrl();
	bool                       InitResultListCtrl();
	bool                       InitPinTable(bool bForce = false);
	bool                       RefreshListCell();
	bool                       RefreshListCell(int row, int col);
	bool                       RefreshResultListCell();
	bool                       RefreshResultSelectedEdit(int nItem);
	//---------------------------------------------------------------------------------//
	bool                       ImportPinLocationFile(const CString& filePath);
	bool                       SplitPinCsvLine(const CString& line, CString& pinLocation, CString& strX, CString& strY);
	bool                       ParsePinLocation(const CString& pinLocation, int& row, int& col);
	//---------------------------------------------------------------------------------//
	bool                       ExportPinTableCsv(const CString& filePath);
	//---------------------------------------------------------------------------------//
	bool                       ExecCreateAOIPin();
	bool                       ExecCreateAOIPin_kn(TPOINT2D PinCadPos, const int nRow, const int nCol);
	bool                       ExecAddDefaultWnd();
	bool                       AddModelWnd_MeasureConnector(const TMODEL_DEFAULT_WND_PARAM & Param, int RefLandGroupID);
	bool                       UpdateToMainFrame();
	//---------------------------------------------------------------------------------//
};
