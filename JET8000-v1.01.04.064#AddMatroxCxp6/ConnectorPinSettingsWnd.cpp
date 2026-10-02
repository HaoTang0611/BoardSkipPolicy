// ConnectorPinSettingsWnd.cpp : 實作檔
//
//---------------------------------------------------------------------------------//
#include "stdafx.h"
#include "Jet8000.h"
#include "ConnectorPinSettingsWnd.h"
#include "afxdialogex.h"

//---------------------------------------------------------------------------------//
// CConnectorPinSettingsWnd 對話方塊

IMPLEMENT_DYNAMIC(CConnectorPinSettingsWnd, CDialogEx)
//---------------------------------------------------------------------------------//
CConnectorPinSettingsWnd::CConnectorPinSettingsWnd(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CConnectorPinSettingsWnd::IDD, pParent)
{
	m_ModelPtr = NULL;
	m_LandPtr = NULL;
	m_WndPtr = NULL;
	m_nColumn = 8;
	m_nRow = 8;
	m_nSelectedRow = -1;
	m_nSelectedCol = -1;
	m_bShowResult = false;
	m_PinTable.clear();
	m_PinTableRes.clear();
}
//---------------------------------------------------------------------------------//
CConnectorPinSettingsWnd::~CConnectorPinSettingsWnd()
{
	m_PinTable.clear();
	m_PinTableRes.clear();
	JetAPI::ClearListCtrl(m_ListCtrl, true);
}
//---------------------------------------------------------------------------------//
void CConnectorPinSettingsWnd::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, CPT_PIN_LIST, m_ListCtrl);
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::SetModelInfo(CAOIModel * ModelPtr, CAOIWnd * WndPtr, bool bShowResult)
{
	if (ModelPtr == NULL || WndPtr == NULL) { return false; }
	m_ModelPtr = ModelPtr;
	m_WndPtr = WndPtr;
	m_bShowResult = bShowResult;
	return true;
}
//---------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CConnectorPinSettingsWnd, CBaseDialog)
	ON_BN_CLICKED(CPT_TABLE_SETTING_BTN, OnTableSetBtn)
	ON_BN_CLICKED(CPT_PIN_SETTING_BTN, OnPinSetBtn)
	ON_BN_CLICKED(CPT_IMPORT_BTN, OnImportBtn)
	ON_BN_CLICKED(CPT_SAVEAS_BTN, OnSaveAsBtn)
	ON_NOTIFY(NM_CLICK, CPT_PIN_LIST, OnLvnItemchangedPinList)
END_MESSAGE_MAP()
//---------------------------------------------------------------------------------//
BOOL CConnectorPinSettingsWnd::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	if (m_WndPtr == NULL) { return 0; }

	CAlgParam &AlgParamPtr = m_WndPtr->GetWndAlgParam();
	TALG_PARAM_MEASURE_CONNECTOR &mcParam = AlgParamPtr.GetAlgParamMeasureConnector();

	m_nRow = mcParam.mc_nPinPosTableRow;
	m_nColumn = mcParam.mc_nPinPosTableCol;
	m_PinTable = mcParam.mc_PinPosTable;
	m_PinTableRes = mcParam.mc_PinPosTableRes;

	if (m_bShowResult == true && CheckPinTableSize(m_PinTableRes, m_nRow, m_nColumn) == false)
	{
		InitEmptyPinTableByRef(m_PinTableRes, m_PinTable);
	}

	SetDlgItemInt(CPT_TABLE_COLUMN_EDIT, m_nColumn);
	SetDlgItemInt(CPT_TABLE_ROW_EDIT, m_nRow);

	JetAPI::InitialListCtrl(m_ListCtrl);
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.ModifyStyle(0, LVS_REPORT);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES);

	if (m_bShowResult == true)
	{
		InitResultListCtrl();
		RefreshResultListCell();
	}
	else
	{
		InitListCtrl();
		InitPinTable();
		RefreshListCell();
	}

	ApplyShowResultMode();
	return 0;
}
//---------------------------------------------------------------------------------//
void CConnectorPinSettingsWnd::OnOK()
{
	if (m_bShowResult == true)
	{
		CBaseDialog::OnOK();
		return;
	}

	CWnd* pFocus = GetFocus();
	if (pFocus != NULL)
	{
		UINT id = pFocus->GetDlgCtrlID();
		switch (id)
		{
		case CPT_PIN_X_EDIT:
		case CPT_PIN_Y_EDIT:
			OnPinSetBtn();
			return;
		}
	}

	if (m_WndPtr == NULL) { CBaseDialog::OnOK(); return; }

	CAlgParam &AlgParamPtr = m_WndPtr->GetWndAlgParam();
	TALG_PARAM_MEASURE_CONNECTOR &mcParam = AlgParamPtr.GetAlgParamMeasureConnector();
	mcParam.mc_PinPosTable = m_PinTable;
	mcParam.mc_nPinPosTableRow = GetDlgItemInt(CPT_TABLE_ROW_EDIT);
	mcParam.mc_nPinPosTableCol = GetDlgItemInt(CPT_TABLE_COLUMN_EDIT);
	BOOL bUsed = FALSE;
	const int width = GetDlgItemInt(CPT_PIN_WIDTH_EDIT, &bUsed);
	if (TRUE == bUsed) { mcParam.mc_nPinWidth = width; }
	const int height = GetDlgItemInt(CPT_PIN_HEIGHT_EDIT, &bUsed);
	if (TRUE == bUsed) { mcParam.mc_nPinHeight = height; }

	ExecCreateAOIPin();
	CBaseDialog::OnOK();
}
//---------------------------------------------------------------------------------//
void CConnectorPinSettingsWnd::OnTableSetBtn()
{
	if (m_bShowResult == true) { return; }
	InitListCtrl();
	InitPinTable(true);
	RefreshListCell();
}
//---------------------------------------------------------------------------------//
void CConnectorPinSettingsWnd::OnPinSetBtn()
{
	if (m_bShowResult == true) { return; }

	CString str;
	if (m_nSelectedRow < 0 || m_nSelectedCol < 0)
	{
		str = _T("Please select a cell first.");
		JetAPI::ShowMessageBox(str);
		return;
	}
	if (m_nSelectedRow >= (int)m_PinTable.size()) { return; }
	if (m_nSelectedCol >= (int)m_PinTable[m_nSelectedRow].size()) { return; }

	CString strX, strY;
	GetDlgItemText(CPT_PIN_X_EDIT, strX);
	GetDlgItemText(CPT_PIN_Y_EDIT, strY);
	strX.Trim();
	strY.Trim();
	if (strX.IsEmpty() || strY.IsEmpty())
	{
		str = _T("X and Y cannot be empty.");
		JetAPI::ShowMessageBox(str);
		return;
	}

	m_PinTable[m_nSelectedRow][m_nSelectedCol].x = _tstof(strX);
	m_PinTable[m_nSelectedRow][m_nSelectedCol].y = _tstof(strY);
	RefreshListCell(m_nSelectedRow, m_nSelectedCol);
}
//---------------------------------------------------------------------------------//
void CConnectorPinSettingsWnd::OnImportBtn()
{
	if (m_bShowResult == true) { return; }

	CString str;
	CFileDialog dlg(TRUE, _T("csv"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("Data Files (*.csv;*.txt;*.xlsx)|*.csv;*.txt;*.xlsx|CSV Files (*.csv)|*.csv|Text Files (*.txt)|*.txt|Excel Files (*.xlsx)|*.xlsx|All Files (*.*)|*.*||"), this);
	if (dlg.DoModal() != IDOK) { return; }

	CString filePath = dlg.GetPathName();
	CString ext = dlg.GetFileExt();
	ext.MakeLower();
	if (ext == _T("xlsx"))
	{
		str = _T("XLSX import is not implemented yet. Please save as CSV first.");
		JetAPI::ShowMessageBox(str);
		return;
	}
	if (false == ImportPinLocationFile(filePath))
	{
		str = _T("Import failed.");
		JetAPI::ShowMessageBox(str);
	}
}
//---------------------------------------------------------------------------------//
void CConnectorPinSettingsWnd::OnSaveAsBtn()
{
	if (m_bShowResult == true) { return; }

	CFileDialog dlg(FALSE, _T("csv"), _T("ConnectorPinSetting.csv"), OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		_T("CSV Files (*.csv)|*.csv|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"), this);
	if (dlg.DoModal() != IDOK) { return; }

	CString str;
	CString filePath = dlg.GetPathName();
	if (!ExportPinTableCsv(filePath))
	{
		str = _T("Export CSV failed.");
		JetAPI::ShowMessageBox(str);
		return;
	}
	str = _T("Export CSV completed.");
	JetAPI::ShowMessageBox(str);
}
//---------------------------------------------------------------------------------//
void CConnectorPinSettingsWnd::OnLvnItemchangedPinList(NMHDR * pNMHDR, LRESULT * pResult)
{
	LPNMITEMACTIVATE pItem = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pItem == NULL) { *pResult = 0; return; }

	if (m_bShowResult == true)
	{
		RefreshResultSelectedEdit(pItem->iItem);
		*pResult = 0;
		return;
	}

	int nItem = pItem->iItem;
	int nSubItem = pItem->iSubItem;
	m_nSelectedRow = nItem;
	m_nSelectedCol = nSubItem - 1;

	if (m_nSelectedRow >= 0 && m_nSelectedCol >= 0)
	{
		if (m_nSelectedRow >= (int)m_PinTable.size()) { *pResult = 0; return; }
		if (m_nSelectedCol >= (int)m_PinTable[m_nSelectedRow].size()) { *pResult = 0; return; }

		CString strName;
		strName.Format(_T("R%dC%d"), nItem + 1, nSubItem);
		SetDlgItemText(CPT_PIN_NAME_EDIT, strName);

		const TPOINT2D& pt = m_PinTable[m_nSelectedRow][m_nSelectedCol];
		CString sx, sy;
		sx.Format(_T("%.3f"), pt.x);
		sy.Format(_T("%.3f"), pt.y);
		SetDlgItemText(CPT_PIN_X_EDIT, sx);
		SetDlgItemText(CPT_PIN_Y_EDIT, sy);
	}
	*pResult = 0;
}
//---------------------------------------------------------------------------------//
CString CConnectorPinSettingsWnd::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section = _T("IDD_CONNECTOR_PIN_TABLE_WND");
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);
	return NewLabelText;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::ApplyShowResultMode()
{
	if (m_bShowResult == false) { return true; }
	const UINT DisableIDList[] = {
		CPT_TABLE_COLUMN_EDIT, CPT_TABLE_ROW_EDIT, CPT_TABLE_SETTING_BTN,
		CPT_PIN_NAME_EDIT, CPT_PIN_X_EDIT, CPT_PIN_Y_EDIT,
		CPT_PIN_WIDTH_EDIT, CPT_PIN_HEIGHT_EDIT, CPT_PIN_ANGLE_EDIT,
		CPT_PIN_SETTING_BTN, CPT_IMPORT_BTN, CPT_SAVEAS_BTN
	};
	for (int i = 0; i < (int)(sizeof(DisableIDList) / sizeof(DisableIDList[0])); ++i)
	{
		CWnd* pWnd = GetDlgItem(DisableIDList[i]);
		if (pWnd != NULL) { pWnd->EnableWindow(FALSE); }
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::CheckPinTableSize(const std::vector<std::vector<TPOINT2D> >& PinTable, int nRow, int nCol)
{
	if (nRow <= 0 || nCol <= 0) { return false; }
	if ((int)PinTable.size() != nRow) { return false; }
	for (int r = 0; r < nRow; ++r)
	{
		if ((int)PinTable[r].size() != nCol) { return false; }
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::InitEmptyPinTableByRef(std::vector<std::vector<TPOINT2D> >& TargetTable, const std::vector<std::vector<TPOINT2D> >& RefTable)
{
	TargetTable.clear();
	TargetTable.resize(RefTable.size());
	for (size_t r = 0; r < RefTable.size(); ++r)
	{
		TargetTable[r].resize(RefTable[r].size());
		for (size_t c = 0; c < RefTable[r].size(); ++c)
		{
			TargetTable[r][c].x = 0.0;
			TargetTable[r][c].y = 0.0;
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::InitListCtrl()
{
	CThisListCtrl_73 &ListCtrl = m_ListCtrl;
	JetAPI::ClearListCtrl(ListCtrl, true);
	RECT Rect;
	CString str;
	ListCtrl.GetClientRect(&Rect);
	const int Align = LVCFMT_LEFT;
	const int ListCtrlWidth = Rect.right - Rect.left;
	const int MinWidth = 32;
	const int n_Column = GetDlgItemInt(CPT_TABLE_COLUMN_EDIT);
	const int n_Row = GetDlgItemInt(CPT_TABLE_ROW_EDIT);
	if (n_Column <= 0 || n_Row <= 0) { return true; }
	m_nColumn = n_Column;
	m_nRow = n_Row;

	int width = MAX(MinWidth, ListCtrlWidth / (n_Column + 1));
	int nCol = 0;
	str = _T("row\\col");
	str = LoadMultiLanguageString(str, str);
	ListCtrl.InsertColumn(nCol++, str, Align, width);
	for (int c = 0; c < n_Column; c++)
	{
		str.Format(_T("C%d"), c + 1);
		ListCtrl.InsertColumn(nCol++, str, Align, width);
	}
	for (int r = 0; r < n_Row; r++)
	{
		str.Format(_T("R%d"), r + 1);
		int nItem = ListCtrl.InsertItem(r, str);
		for (int c = 0; c < n_Column; c++)
		{
			ListCtrl.SetItemText(nItem, c + 1, _T("(0.0,0.0)"));
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::InitResultListCtrl()
{
	CThisListCtrl_73 &ListCtrl = m_ListCtrl;
	JetAPI::ClearListCtrl(ListCtrl, true);
	ListCtrl.ModifyStyle(0, LVS_REPORT);
	ListCtrl.SetExtendedStyle(ListCtrl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	int nCol = 0;
	ListCtrl.InsertColumn(nCol++, _T("Pin"), LVCFMT_LEFT, 70);
	ListCtrl.InsertColumn(nCol++, _T("Std X"), LVCFMT_RIGHT, 90);
	ListCtrl.InsertColumn(nCol++, _T("Std Y"), LVCFMT_RIGHT, 90);
	ListCtrl.InsertColumn(nCol++, _T("Res X"), LVCFMT_RIGHT, 90);
	ListCtrl.InsertColumn(nCol++, _T("Res Y"), LVCFMT_RIGHT, 90);
	ListCtrl.InsertColumn(nCol++, _T("dX"), LVCFMT_RIGHT, 90);
	ListCtrl.InsertColumn(nCol++, _T("dY"), LVCFMT_RIGHT, 90);
	return true;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::InitPinTable(bool bForce)
{
	const int n_Column = GetDlgItemInt(CPT_TABLE_COLUMN_EDIT);
	const int n_Row = GetDlgItemInt(CPT_TABLE_ROW_EDIT);
	if (false == m_PinTable.empty() && false == bForce) { return true; }
	m_PinTable.resize(n_Row);
	for (int r = 0; r < n_Row; r++)
	{
		m_PinTable[r].resize(n_Column);
		for (int c = 0; c < n_Column; c++)
		{
			m_PinTable[r][c].x = 0.0;
			m_PinTable[r][c].y = 0.0;
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::RefreshListCell()
{
	for (int i = 0; i < m_nRow; i++)
	{
		for (int j = 0; j < m_nColumn; j++)
		{
			RefreshListCell(i, j);
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::RefreshListCell(int row, int col)
{
	if (row < 0 || col < 0) { return true; }
	if (row >= (int)m_PinTable.size()) { return false; }
	if (col >= (int)m_PinTable[row].size()) { return false; }
	const TPOINT2D& pt = m_PinTable[row][col];
	CString text;
	text.Format(_T("(%.1f, %.1f)"), pt.x, pt.y);
	m_ListCtrl.SetItemText(row, col + 1, text);
	return true;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::RefreshResultListCell()
{
	m_ListCtrl.DeleteAllItems();
	if (CheckPinTableSize(m_PinTable, m_nRow, m_nColumn) == false) { return false; }
	if (CheckPinTableSize(m_PinTableRes, m_nRow, m_nColumn) == false) { return false; }

	CString str;
	for (int r = 0; r < m_nRow; ++r)
	{
		for (int c = 0; c < m_nColumn; ++c)
		{
			const TPOINT2D& StdPt = m_PinTable[r][c];
			const TPOINT2D& ResPt = m_PinTableRes[r][c];
			const double dX = ResPt.x - StdPt.x;
			const double dY = ResPt.y - StdPt.y;
			CString strPin;
			strPin.Format(_T("R%dC%d"), r + 1, c + 1);
			const int nItem = m_ListCtrl.InsertItem(m_ListCtrl.GetItemCount(), strPin);
			str.Format(_T("%.3f"), StdPt.x); m_ListCtrl.SetItemText(nItem, 1, str);
			str.Format(_T("%.3f"), StdPt.y); m_ListCtrl.SetItemText(nItem, 2, str);
			str.Format(_T("%.3f"), ResPt.x); m_ListCtrl.SetItemText(nItem, 3, str);
			str.Format(_T("%.3f"), ResPt.y); m_ListCtrl.SetItemText(nItem, 4, str);
			str.Format(_T("%+.3f"), dX); m_ListCtrl.SetItemText(nItem, 5, str);
			str.Format(_T("%+.3f"), dY); m_ListCtrl.SetItemText(nItem, 6, str);
		}
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::RefreshResultSelectedEdit(int nItem)
{
	if (nItem < 0 || m_nColumn <= 0) { return false; }
	const int nRow = nItem / m_nColumn;
	const int nCol = nItem % m_nColumn;
	m_nSelectedRow = nRow;
	m_nSelectedCol = nCol;
	if (CheckPinTableSize(m_PinTable, m_nRow, m_nColumn) == false) { return false; }
	if (CheckPinTableSize(m_PinTableRes, m_nRow, m_nColumn) == false) { return false; }
	if (nRow < 0 || nRow >= m_nRow || nCol < 0 || nCol >= m_nColumn) { return false; }

	const TPOINT2D& StdPt = m_PinTable[nRow][nCol];
	const TPOINT2D& ResPt = m_PinTableRes[nRow][nCol];
	const double dX = ResPt.x - StdPt.x;
	const double dY = ResPt.y - StdPt.y;

	CString strName, strX, strY;
	strName.Format(_T("R%dC%d  D=(%+.3f, %+.3f)"), nRow + 1, nCol + 1, dX, dY);
	strX.Format(_T("%.6f"), ResPt.x);
	strY.Format(_T("%.6f"), ResPt.y);
	SetDlgItemText(CPT_PIN_NAME_EDIT, strName);
	SetDlgItemText(CPT_PIN_X_EDIT, strX);
	SetDlgItemText(CPT_PIN_Y_EDIT, strY);
	return true;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::ImportPinLocationFile(const CString & filePath)
{
	CStdioFile file;
	if (false == file.Open(filePath, CFile::modeRead | CFile::typeText)) { return false; }
	CString line;
	if (false == file.ReadString(line)) { file.Close(); return false; }
	int maxRow = 0, maxCol = 0;
	struct SImportPin { int row; int col; TPOINT2D point; };
	std::vector<SImportPin> pins;
	SImportPin pin;
	while (file.ReadString(line))
	{
		line.Trim();
		if (line.IsEmpty()) { continue; }
		CString pinLocation, strX, strY;
		if (false == SplitPinCsvLine(line, pinLocation, strX, strY)) { continue; }
		int row = 0, col = 0;
		if (!ParsePinLocation(pinLocation, row, col)) { continue; }
		pin.row = row;
		pin.col = col;
		pin.point.x = _tstof(strX);
		pin.point.y = _tstof(strY);
		pins.push_back(pin);
		maxRow = MAX(maxRow, row);
		maxCol = MAX(maxCol, col);
	}
	file.Close();
	if (pins.empty()) { return false; }
	SetDlgItemInt(CPT_TABLE_ROW_EDIT, maxRow);
	SetDlgItemInt(CPT_TABLE_COLUMN_EDIT, maxCol);
	InitListCtrl();
	InitPinTable(true);
	for (size_t i = 0; i < pins.size(); i++)
	{
		int Row = pins[i].row - 1;
		int Col = pins[i].col - 1;
		if (Row < 0 || Col < 0) { continue; }
		if (Row >= (int)m_PinTable.size()) { continue; }
		if (Col >= (int)m_PinTable[Row].size()) { continue; }
		m_PinTable[Row][Col] = pins[i].point;
		RefreshListCell(Row, Col);
	}
	return true;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::SplitPinCsvLine(const CString & line, CString & pinLocation, CString & strX, CString & strY)
{
	CString temp = line;
	TCHAR delimiter = _T(',');
	if (temp.Find(_T('\t')) >= 0) { delimiter = _T('\t'); }
	int pos = 0;
	pinLocation = temp.Tokenize(CString(delimiter), pos);
	strX = temp.Tokenize(CString(delimiter), pos);
	strY = temp.Tokenize(CString(delimiter), pos);
	pinLocation.Trim(); strX.Trim(); strY.Trim();
	return !pinLocation.IsEmpty() && !strX.IsEmpty() && !strY.IsEmpty();
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::ParsePinLocation(const CString & pinLocation, int & row, int & col)
{
	row = 0; col = 0;
	CString s = pinLocation;
	s.Trim(); s.MakeUpper();
	int rPos = s.Find(_T('R'));
	int cPos = s.Find(_T('C'));
	if (rPos < 0 || cPos < 0 || cPos <= rPos) { return false; }
	CString strRow = s.Mid(rPos + 1, cPos - rPos - 1);
	CString strCol = s.Mid(cPos + 1);
	row = _ttoi(strRow);
	col = _ttoi(strCol);
	return row > 0 && col > 0;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::ExportPinTableCsv(const CString & filePath)
{
	CStdioFile file;
	if (!file.Open(filePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText)) { return false; }
	file.WriteString(_T("Pin Location,X Nom,Y Nom\n"));
	const int nRow = static_cast<int>(m_PinTable.size());
	CString line;
	for (int r = 0; r < nRow; r++)
	{
		const int nCol = static_cast<int>(m_PinTable[r].size());
		for (int c = 0; c < nCol; c++)
		{
			const TPOINT2D& pt = m_PinTable[r][c];
			line.Format(_T("R%dC%d,%.6f,%.6f\n"), r + 1, c + 1, pt.x, pt.y);
			file.WriteString(line);
		}
	}
	file.Close();
	return true;
}
//---------------------------------------------------------------------------------//
// 以下模型建立相關函式保留原本流程，僅補基本防呆與結果模式不會進入 OnOK 建立流程。
bool CConnectorPinSettingsWnd::ExecCreateAOIPin()
{
	CAOIModel *ModelPtr = m_ModelPtr;
	if (NULL == ModelPtr) { return true; }
	if (ModelPtr->GetModelEditMode() == false) { return true; }
	if (AOIDataCollect.OperateLevelEditFuncAddModelWnd() == false) { return false; }
	TPOINT2D WndCenterPt, PinPt, CadPt;
	ModelPtr->GetModelBodyPos(WndCenterPt);
	for (int i = 0; i < m_nRow; i++)
	{
		for (int j = 0; j < m_nColumn; j++)
		{
			if (i >= (int)m_PinTable.size()) { continue; }
			if (j >= (int)m_PinTable[i].size()) { continue; }
			PinPt = m_PinTable[i][j];
			CadPt.x = WndCenterPt.x + JetAPI::Unit_MMtoUM(PinPt.x);
			CadPt.y = WndCenterPt.y + JetAPI::Unit_MMtoUM(PinPt.y);
			ExecCreateAOIPin_kn(CadPt, i, j);
		}
	}
	ExecAddDefaultWnd();
	ModelPtr->UpdateModelBodyRgnFromChipLead();
	ModelPtr->SetModelModifiedCount(true);
	ModelPtr->SetModelNeedSaveFiles(true);
	ModelPtr->CalcModelTotalRegionAll();
	ModelPtr->UpdateModelRegionToAttached();
	ModelPtr->UpdateModelChipLeadRgnFromBody();
	ModelPtr->UpdateModelBodyToComponent();
	UpdateToMainFrame();
	return true;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::ExecCreateAOIPin_kn(TPOINT2D PinCadPos, const int nRow, const int nCol)
{
	CAOIModel *ModelPtr = m_ModelPtr;
	if (NULL == ModelPtr) { return true; }
	TREGION4D RgnCadBox;
	RgnCadBox.Move(PinCadPos);
	BOOL bUsed = FALSE;
	int width = GetDlgItemInt(CPT_PIN_WIDTH_EDIT, &bUsed);
	if (FALSE == bUsed) { width = 500; }
	int height = GetDlgItemInt(CPT_PIN_HEIGHT_EDIT, &bUsed);
	if (FALSE == bUsed) { height = 500; }
	double angle = GetDlgItemInt(CPT_PIN_ANGLE_EDIT, &bUsed);
	if (FALSE == bUsed) { angle = 0.0; }
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(angle);
	RgnCadBox.SetSize(height, width);
	const MODEL_TYPE ModelType = ModelPtr->GetModelType();
	CString str;
	CAOILand *NewLandPtr = NULL;
	if (m_LandPtr == NULL)
	{
		LAND_TYPE LandType = LAND_TYPE_DIP_LEAD;
		CString LandTypeTxt = AOIDataDefine.GetLandTypeText(LandType);
		CAOILand *LandPtr = ModelPtr->CreateModelLand(ModelType, RgnCadBox, LandType);
		if (NULL == LandPtr)
		{
			str.Format(_T("Error, Create Model Land Ptr Fault [%s]"), LandTypeTxt);
			JetAPI::ShowMessageBox(str);
			return false;
		}
		LandPtr->SetLandToward(BOX_TOWARD_NULL);
		LogOperCtrl.SaveLogModelLandSelectedCreate(ModelPtr);
		m_LandPtr = NewLandPtr = LandPtr;
		ModelPtr->UnSelectModel();
		ModelPtr->AddModelLandPtr(NewLandPtr, false);
	}
	else
	{
		CAOIBox *BoxPtr = m_LandPtr->GetLandBoxPtr();
		if (BoxPtr == NULL) { return false; }
		TPOINT2D RefPt, MovePt;
		BoxPtr->GetBoxPos(RefPt);
		MovePt.x = PinCadPos.x - RefPt.x;
		MovePt.y = PinCadPos.y - RefPt.y;
		ModelPtr->CloneMoveModelLandSelected(MovePt.x, MovePt.y, true);
		CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
		NewLandPtr = LandPtr;
		ModelPtr->SetModelLandActived(false);
		ModelPtr->SetModelLandSelected(false);
		m_LandPtr->SetLandAllBoxActived(true);
		m_LandPtr->SetLandAllBoxSelected(true);
	}
	if (NewLandPtr == NULL) { return false; }
	str.Format(_T("R%dC%d"), nRow + 1, nCol + 1);
	NewLandPtr->SetLandName(str);
	return true;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::ExecAddDefaultWnd()
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if (NULL == ProjectPtr) { return false; }
	CAOIModel *ModelPtr = m_ModelPtr;
	if (NULL == ModelPtr) { return false; }
	if (ModelPtr->GetModelEditMode() == false) { return false; }
	if (AOIDataCollect.OperateLevelEditFuncAddModelWnd() == false) { return false; }

	TREGION4D Region;
	TPOINT2D CornorPos[4];
	TMODEL_DEFAULT_WND_PARAM Param;
	MODEL_TYPE ModelType = ModelPtr->GetModelType();
	CString GroupName = ModelPtr->GetModelGroupName();
	CHIP_SIZE_MODE ChipSizeMode = ModelPtr->GetModelChipSizeMode();
	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();
	const bool bUse3DLight = ProjectPtr->CheckProjectFrameUniqueID_3D();
	const size_t WndCount = ModelPtr->GetModelWndCount();

	ModelPtr->GetModelBodyBox().GetBoxCornerPos(CornorPos);
	JetAPI::RotateCornerPos(-ComponentAngle, 0, 0, CornorPos);
	JetAPI::PointsToRegion(CornorPos, 4, Region);
	const double ModelBodyHeight = ModelPtr->GetModelBodyHeight();
	const double ModelBodyMissing = JetAPI::AdjustValue(ModelBodyHeight * 0.25, 10);
	if (CHIP_SIZE_NONE == ChipSizeMode)
	{
		ChipSizeMode = CAOIModel::FindModelChipSizeMode(ModelType, Region);
	}

	Param.eVersion = MDW_VERSION_1;
	Param.sGroupName = GroupName;
	Param.bUse3DLight = bUse3DLight;
	Param.eChipSizeMode = ChipSizeMode;
	Param.dBodyMissingHeightTolerance = MAX(ModelBodyMissing, 50);
	Param.SetAll(false);
	Param.SetEnable(WND_DEFECT_LEAD_ADJUST, true);

	unsigned int DefaultFrameIndex = 0;
	unsigned int DefaultFrameUniqueID = 0;
	std::vector<unsigned int> FrameIndexMapList;
	ProjectPtr->BuildProjectFrameIndexMapParam(FrameIndexMapList, DefaultFrameIndex, DefaultFrameUniqueID);
	if (ProjectPtr->ModifyProjectModelDefaultWndParam(Param) == false)
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return false;
	}

	ModelPtr->AddModelDefaultWnd(Param);
	AddModelWnd_MeasureConnector(Param, -1);
	ModelPtr->InvisibleModelWnd();
	ModelPtr->UnSelectModel();
	ModelPtr->SetModelBodyBoxActived(true);
	ModelPtr->UpdateModelFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);
	std::vector<CColorGroup> ColorGroupList;
	ProjectPtr->CloneProjectColorGroupList(ColorGroupList);
	ModelPtr->UpdateModelColorGroupLinkIndex(ColorGroupList);
	CAOIWnd *WndPtr = ModelPtr->GetModelWndPtr(WndCount, true);
	if (NULL == WndPtr) { WndPtr = ModelPtr->GetModelWndPtr(0, true); }
	if (NULL != WndPtr)
	{
		const int WndGroupID = WndPtr->GetWndGroupID();
		ModelPtr->SetModelWndVisibledByWndGroupID(WndGroupID, -1, true);
		WndPtr->SetWndSelected(true);
	}
	ModelPtr->SetModelWndActived(WndPtr);
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	CAOIComponent *pComponent = ProjectPtr->GetProjectActiveComponent();
	AOIDataCollect.CloseActiveComponent(pComponent);
	return true;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::AddModelWnd_MeasureConnector(const TMODEL_DEFAULT_WND_PARAM &Param, int RefLandGroupID)
{
	CAOIModel *ModelPtr = m_ModelPtr;
	if (ModelPtr == NULL) { return false; }
	if (m_WndPtr == NULL) { return false; }

	const MODEL_TYPE ModelType = ModelPtr->GetModelType();
	const ALG_TYPE AlgType = ALG_MEASURE_CONNECTOR_PIN;
	const WND_DEFECT_ID WndDefectID = WND_DEFECT_USER_DEFINE_01;
	const int DefectGroupID = ModelPtr->GetModelFreeDefectGroupID(WndDefectID);
	const bool UseExtendBox = CAlgParam::CheckAlgUseExtendBox(AlgType);
	WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	WND_RGN_LINK_MODE WndRgnLinkMode = CAOIModel::ObtainModelDefaultWndRegionLinkMode(ModelType, WndDefectID);
	const int WndGouprID = ModelPtr->GetModelWndFreeGroupID();
	const int WndBandID = ModelPtr->GetModelWndFreeBandID(WndGouprID);

	TREGION4D RgnWnd;
	m_WndPtr->GetWndBox().GetBoxRegion(RgnWnd);
	CAOIWnd *WndPtr = AOIObjManager.CreateWndObj();
	if (NULL == WndPtr) { return false; }

	WndPtr->SetWndToward(BOX_TOWARD_NULL);
	WndPtr->SetWndBandID(WndBandID);
	WndPtr->SetWndGroupID(WndGouprID);
	WndPtr->SetWndRgnLinkAuto(true);
	WndPtr->SetWndRgnLinkMode(WndRgnLinkMode);
	WndPtr->SetWndDefectID(WndDefectID);
	WndPtr->SetWndDefectGroupID(DefectGroupID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->SetWndRegion(RgnWnd);
	WndPtr->SetWndExtendBoxUsed(UseExtendBox);
	WndPtr->UpdateWndExtendBox();
	WndPtr->SetWndAlgType(AlgType);
	WndPtr->GetWndAlgParam().CheckAlgPatternFileUsed();

	CAlgParam &AlgParamOri = m_WndPtr->GetWndAlgParam();
	TALG_PARAM_MEASURE_CONNECTOR &mcParam = AlgParamOri.GetAlgParamMeasureConnector();
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	AlgParam.SetAlgParamMeasureConnector(mcParam);
	ModelPtr->AddModelWndPtr(WndPtr, false);
	return true;
}
//---------------------------------------------------------------------------------//
bool CConnectorPinSettingsWnd::UpdateToMainFrame()
{
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if (NULL == ProjectPtr) { return false; }
	ProjectPtr->SetProjectActiveModelWnd(NULL);
	AOIDataCollect.SendMainFrameWndMessage(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
	AOIDataCollect.SendMainFrameWndMessage(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, TREE_CTRL_UPDATE_NULL);
	AOIDataCollect.SendMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_UPDATE_WND_SELECTED, NULL);
	return true;
}
//---------------------------------------------------------------------------------//
