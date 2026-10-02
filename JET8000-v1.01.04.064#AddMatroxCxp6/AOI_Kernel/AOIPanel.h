// AOIPanel.h: interface for the CAOIPanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIPANEL_H__849E16D3_81AC_489D_B5C9_8C44A046B873__INCLUDED_)
#define AFX_AOIPANEL_H__849E16D3_81AC_489D_B5C9_8C44A046B873__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include <vector>
#include "AOIObj.h"
#include "AOIBoard.h"
#include "MapCoordinate.h"
#include "JetFieldDivider.h"
//-------------------------------------------------------------------------------------//
class CAOIFileIO;
class CAOIProject;
//-------------------------------------------------------------------------------------//
enum PANEL_TYPE
{
	PANEL_TYPE_DUMMY      = 0,	
	PANEL_TYPE_NORMAL     = 1,	
	PANEL_TYPE_RETURN
};
//-------------------------------------------------------------------------------------//
class CAOIPanel;
typedef struct tagPanelRect
{		
	unsigned int   PanelIndex;	
	CAOIPanel     *PanelPtr;
	TREGION4D      PanelRgn;	
	tagPanelRect()
	{
		PanelIndex = -1;		
		PanelPtr = NULL;
		PanelRgn = TREGION4D();
	}
} TPanelRect, *PPanelRect; 
//-------------------------------------------------------------------------------------//
class CAOIPanel : public CAOIObj  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CAOIPanel)
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//
	CAOIProject*               m_PanelProjectPtr;//整板所屬的專案指標
	//---------------------------------------------------------------------------------//		
	unsigned int               m_PanelIndex_Project;//整板的引數編號
	//---------------------------------------------------------------------------------//	
	int                        m_PanelTempInt[4];//整板暫存整數
	//---------------------------------------------------------------------------------//	
	CString                    m_PanelErrorString;//整板的錯誤訊息
	bool                       m_PanelDeleted;//是否刪除
	bool                       m_PanelSelected;//是否選取到		
	bool                       m_PanelBypassed;//是否不檢測
	bool                       m_PanelModified;//整板變更過
	bool                       m_PanelBarcodeEnabled;//整板條碼啟用
	bool                       m_PanelMultiDistrictMode;//整板多區段模式
	PANEL_TYPE                 m_PanelType;	
	TRECT4D                    m_PanelMapRect;//整板在底圖的範圍
	//---------------------------------------------------------------------------------//
	int                        m_PanelBoardRowCount;//整板的單板列數
	int                        m_PanelBoardColCount;//整板的單板欄數
	int                        m_PanelBoardColBlockCount;//整板的單板欄數區塊數
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  m_PanelResultID_AOI;//整板檢測結果
	RESULT_ID                  m_PanelResultID_AOI_LA;//整板檢測結果-A軌
	RESULT_ID                  m_PanelResultID_AOI_LB;//整板檢測結果-B軌
	//---------------------------------------------------------------------------------//
	RESULT_ID                  m_PanelResultID_ARS;//整板檢測結果-ARS
	RESULT_ID                  m_PanelResultID_ARS_LA;//整板檢測結果-ARS-A軌
	RESULT_ID                  m_PanelResultID_ARS_LB;//整板檢測結果-ARS-B軌
	//---------------------------------------------------------------------------------//
	RESULT_ID                  m_PanelResultID_Alarm;//整板檢測結果-警報
	DISTRICT_ID                m_PanelActDistrictID;//整板分段編號
	BOARD_FD_GRAB_MODE         m_PanelBoardFdGrabMode;//單板定位點取像模式
	//---------------------------------------------------------------------------------//
	bool                       m_PanelIsGetBarcode;//整板條碼是否已經取得
	std::wstring               m_PanelBarcode;//整板條碼
	std::wstring               m_PanelBarcodeBackup;//整板條碼備份
	unsigned int               m_PanelBarcodeDeviceIndex;//整板條碼機編號
	unsigned int               m_PanelBarcodeDeviceCodeIndex;//整板條碼機第幾碼
	BARCODE_BELONG_MODE        m_PanelBarcodeBelongMode;//整板條碼屬於模式
	//---------------------------------------------------------------------------------//
	TREGION4D                  m_PanelRgnCad;//整板範圍-Cad
	TREGION4D                  m_PanelRgnStage_DA;//整板範圍-Stage
	TREGION4D                  m_PanelRgnStage_DB;//整板範圍-Stage	
	//---------------------------------------------------------------------------------//	
	TPOINT3D                   m_PanelBasePlaneNormal_DA;//整板基準面法向量
	TPOINT3D                   m_PanelBasePlaneNormal_DB;//整板基準面法向量	
	//---------------------------------------------------------------------------------//	
	CMapCoordinate             m_PanelMapCTS;//整板的座標轉換-Cad to Stage
	CMapCoordinate             m_PanelMapSTC;//整板的座標轉換-Stage to Cad
	CMapCoordinate             m_PanelMapCTS_DB;//整板的座標轉換-Cad to Stage
	CMapCoordinate             m_PanelMapSTC_DB;//整板的座標轉換-Stage to Cad
	bool                       m_PanelCalcMapFinish;//計算整板的座標轉換完成
	//---------------------------------------------------------------------------------//
	std::vector<CAOIFd*>       m_PanelFdPtrList;//整板內的定位點指標列表	
	std::vector<CAOIMark*>     m_PanelMarkPtrList;//整板內的特徵指標列表
	std::vector<CAOIBoard*>    m_PanelBoardPtrList;//整板內的單板指標列表
	std::vector<CAOIField*>    m_PanelFieldPtrList;//整板內的檢測區域指標列表
	std::vector<CAOIBarcode*>  m_PanelBarcodePtrList;//整板內的軟體條碼指標列表
	std::vector<CAOIComponent*> m_PanelComponentPtrList;//整板內的零件指標列表
	//---------------------------------------------------------------------------------//
	double                     m_PanelSkew;//檢測與專案的角偏差
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitPanel();
	void                       InitialPanel();
	void                       ClonePanel(const CAOIPanel &panel);
	//---------------------------------------------------------------------------------//
	size_t                     GetPanelFdCount_Inline() const;//取得整板的定位點數量
	void                       AddPanelFdPtr_Inline(CAOIFd *FdPtr);//增加整板的定位點
	CAOIFd*                    GetPanelFdPtr_Inline(size_t index) const;//取得整板的定位點指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetPanelBarcodeCount_Inline() const;//取得整板的軟體條碼數量
	void                       AddPanelBarcodePtr_Inline(CAOIBarcode *BarcodePtr);//增加整板的軟體條碼
	CAOIBarcode*               GetPanelBarcodePtr_Inline(size_t index) const;//取得整板的軟體條碼指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetPanelMarkCount_Inline() const;//取得整板的特徵點數量
	void                       AddPanelMarkPtr_Inline(CAOIMark *MarkPtr);//增加整板的特徵點
	CAOIMark*                  GetPanelMarkPtr_Inline(size_t index) const;//取得整板的特徵點指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetPanelBoardCount_Inline() const;//取得整板的單板數量
	void                       AddPanelBoardPtr_Inline(CAOIBoard *BoardPtr);//增加整板的單板
	CAOIBoard*                 GetPanelBoardPtr_Inline(size_t index) const;//取得整板的單板指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetPanelFieldCount_Inline() const;//取得整板的區域數量
	void                       AddPanelFieldPtr_Inline(CAOIField *FieldPtr);//增加整板的區域
	CAOIField*                 GetPanelFieldPtr_Inline(size_t index) const;//取得整板的區域指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetPanelComponentCount_Inline() const;//取得整板的零件數量
	void                       AddPanelComponentPtr_Inline(CAOIComponent *ComponentPtr);//增加整板的零件
	CAOIComponent*             GetPanelComponentPtr_Inline(size_t index) const;//取得整板的零件指標	
	//---------------------------------------------------------------------------------//		
public:
	//---------------------------------------------------------------------------------//
	CAOIPanel();
	CAOIPanel(const CAOIPanel &panel);
	virtual ~CAOIPanel();
	CAOIPanel& operator=(const CAOIPanel &panel);
	//---------------------------------------------------------------------------------//	
	CAOIPanel*                 ClonePanelObj() const;//建立且複製一個整板
	//---------------------------------------------------------------------------------//	
	bool                       WritePanelFile(CAOIFileIO &FileIO);//儲存整板檔案
	bool                       ReadPanelFile(CAOIFileIO &FileIO);//載入整板檔案
	//---------------------------------------------------------------------------------//
	bool                       WritePanelSpcFile_JSON_VRS(FILE *pfile, CAOIProject *ProjectPtr);
	bool                       WritePanelSpcFile_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr);
	bool                       WritePanelSpcHeader_JSON_detail(FILE *pfile, CAOIProject *ProjectPtr);
	//---------------------------------------------------------------------------------//
	bool                       ConvertToSpcPanel(DISTRICT_ID DistrictID, TSpcPanel &SpcPanel);//轉成Spc整板圖
	//---------------------------------------------------------------------------------//
	void                       RemovePanelAllObjects();//移除整板所有物件
	//---------------------------------------------------------------------------------//	
	void                       SelectPanelAllObjects(bool value);//選取整板所有物件
	void                       RemovePanelObjectSelected();//移除整板內選取到的物件
	bool                       SetPanelAllObjectTempInt(int val, int idx=0);//設定整板內物件暫存參數
	bool                       SetPanelAllObjectTempInt(DISTRICT_ID DistrictID, int val, int idx=0);//設定整板內物件暫存參數
	//---------------------------------------------------------------------------------//	
	CAOIProject*               GetPanelProjectPtr() const;
	void                       SetPanelProjectPtr(CAOIProject* Ptr);	
	//---------------------------------------------------------------------------------//			
	LPCTSTR                    GetPanelErrorString() const;
	void                       SetPanelErrorString(LPCTSTR val);
	//---------------------------------------------------------------------------------//
	unsigned int               GetPanelIndex_Project() const;
	void                       SetPanelIndex_Project(unsigned int value);	
	//---------------------------------------------------------------------------------//
	int                        GetPanelTempInt() const;
	void                       SetPanelTempInt(int value);	
	//---------------------------------------------------------------------------------//
	int                        GetPanelTempInt_01() const;
	void                       SetPanelTempInt_01(int value);	
	//---------------------------------------------------------------------------------//
	bool                       GetPanelDeleted() const;
	void                       SetPanelDeleted(bool value);	
	//---------------------------------------------------------------------------------//		
	bool                       GetPanelSelected() const;
	void                       SetPanelSelected(bool value);	
	//---------------------------------------------------------------------------------//		
	bool                       GetPanelBypassed() const;
	void                       SetPanelBypassed(bool value);	
	bool                       UpdatePanelBypassed();
	bool                       ExecPanelBypassed(LANE_ID LaneID);//執行整板是為不檢測
	//---------------------------------------------------------------------------------//
	bool                       GetPanelSkiped() const;	
	bool                       CheckPanelBypassedSkipped() const;
	//---------------------------------------------------------------------------------//
	PANEL_TYPE                 GetPanelType() const;
	void                       SetPanelType(PANEL_TYPE value);	
	//---------------------------------------------------------------------------------//	
	PANEL_SIDE_MODE            CheckPanelSideMode() const;
	//---------------------------------------------------------------------------------//	
	void                       SetPanelMapRect(const TRECT4D val);	
	void                       GetPanelMapRect(TRECT4D &val) const;
	//---------------------------------------------------------------------------------//
	//整板的單板列數	
	int                        GetPanelBoardRowCount() const;
	void                       SetPanelBoardRowCount(int value);	
	//---------------------------------------------------------------------------------//	
	//整板的單板欄數
	int                        GetPanelBoardColCount() const;
	void                       SetPanelBoardColCount(int value);	
	//---------------------------------------------------------------------------------//	
	//整板的單板欄區塊數
	int                        GetPanelBoardColBlockCount() const;
	void                       SetPanelBoardColBlockCount(int value);	
	//---------------------------------------------------------------------------------//	
	unsigned int               GetPanelBarcodeDeviceIndex() const;//整板條碼機編號
	void                       SetPanelBarcodeDeviceIndex(unsigned int value);//整板條碼機編號
	//---------------------------------------------------------------------------------//		
	unsigned int               GetPanelBarcodeDeviceCodeIndex() const;//整板條碼機第幾碼
	void                       SetPanelBarcodeDeviceCodeIndex(unsigned int value);//整板條碼機第幾碼	
	//---------------------------------------------------------------------------------//		
	BARCODE_BELONG_MODE        GetPanelBarcodeBelongMode() const;//整板條碼屬於模式
	void                       SetPanelBarcodeBelongMode(BARCODE_BELONG_MODE value);//整板條碼屬於模式	
	//---------------------------------------------------------------------------------//		
	bool                       GetPanelModified() const;//整板變更過
	void                       SetPanelModified(bool value);//整板變更過
	//---------------------------------------------------------------------------------//	
	bool                       GetPanelBarcodeEnabled() const;//整板條碼啟用
	void                       SetPanelBarcodeEnabled(bool value);//整板條碼啟用
	//---------------------------------------------------------------------------------//
	bool                       GetPanelMultiDistrictMode() const;//整板多區段模式
	void                       SetPanelMultiDistrictMode(bool value);//整板多區段模式	
	//---------------------------------------------------------------------------------//
	RESULT_ID                  GetPanelResultID() const;//整板檢測結果
	void                       SetPanelResultID(RESULT_ID value);//整板檢測結果
	//---------------------------------------------------------------------------------//
	RESULT_ID                  GetPanelResultID_AOI_LA() const;//整板檢測結果-A軌
	void                       SetPanelResultID_AOI_LA(RESULT_ID value);//整板檢測結果-A軌
	//---------------------------------------------------------------------------------//
	RESULT_ID                  GetPanelResultID_AOI_LB() const;//整板檢測結果-B軌
	void                       SetPanelResultID_AOI_LB(RESULT_ID value);//整板檢測結果-B軌
	//---------------------------------------------------------------------------------//	
	void                       UpdatePanelResultID_AOI_Lane(LANE_ID LaneID);//更新整板檢測結果-軌道
	RESULT_ID                  GetPanelResultID_AOI_Lane(LANE_ID LaneID) const;//整板檢測結果-軌道	
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetPanelResultID_ARS() const;//整板檢測結果-ARS
	void                       SetPanelResultID_ARS(RESULT_ID value);//整板檢測結果-ARS	
	//---------------------------------------------------------------------------------//
	RESULT_ID                  GetPanelResultID_ARS_LA() const;//整板檢測結果-ARS-A軌
	void                       SetPanelResultID_ARS_LA(RESULT_ID value);//整板檢測結果-ARS-A軌
	//---------------------------------------------------------------------------------//
	RESULT_ID                  GetPanelResultID_ARS_LB() const;//整板檢測結果-ARS-B軌
	void                       SetPanelResultID_ARS_LB(RESULT_ID value);//整板檢測結果-ARS-B軌
	//---------------------------------------------------------------------------------//	
	void                       UpdatePanelResultID_ARS_Lane(LANE_ID LaneID);//更新整板檢測結果-ARS-軌道
	RESULT_ID                  GetPanelResultID_ARS_Lane(LANE_ID LaneID) const;//整板檢測結果-ARS-軌道	
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetPanelResultID_Alarm() const;//整板檢測結果-警報
	void                       SetPanelResultID_Alarm(RESULT_ID value);//整板檢測結果-警報	
	//---------------------------------------------------------------------------------//		
	DISTRICT_ID                GetPanelActDistrictID() const;//整板分段編號
	void                       SetPanelActDistrictID(DISTRICT_ID value);//整板分段編號
	//---------------------------------------------------------------------------------//		
	BOARD_FD_GRAB_MODE         GetPanelBoardFdGrabMode() const;//單板定位點取像模式
	void                       SetPanelBoardFdGrabMode(BOARD_FD_GRAB_MODE value);//單板定位點取像模式
	//---------------------------------------------------------------------------------//	
	TREGION4D                  GetPanelRgnCad() const;
	void                       GetPanelRgnCad(TREGION4D &Rgn) const;
	void                       SetPanelRgnCad(const TREGION4D &Rgn);
	//---------------------------------------------------------------------------------//	
	TREGION4D                  CalcPanelRgnCad() const;
	TREGION4D                  CalcPanelRgnCad(DISTRICT_ID DistrictID) const;
	//---------------------------------------------------------------------------------//		
	TREGION4D                  GetPanelRgnStage(DISTRICT_ID DistrictID) const;
	void                       GetPanelRgnStage(DISTRICT_ID DistrictID, TREGION4D &Rgn) const;
	void                       SetPanelRgnStage(DISTRICT_ID DistrictID, const TREGION4D &Rgn);
	//---------------------------------------------------------------------------------//		
	void                       SetPanelBarcode(const char *barcode);//設定整板條碼
	void                       SetPanelBarcode(const wchar_t *barcode);//設定整板條碼
	const wchar_t*             GetPanelBarcode() const;//取得單板條碼
	//---------------------------------------------------------------------------------//	
	void                       BackupPanelBarcode();//備份整板條碼
	void                       RestorePanelBarcode();//恢復整板條碼	
	//---------------------------------------------------------------------------------//		
	bool                       GetPanelIsGetBarcode() const;//整板條碼是否已經取得
	void                       SetPanelIsGetBarcode(bool value);//整板條碼是否已經取得	
	//---------------------------------------------------------------------------------//	
	double                     GetPanelSkew() const;
	void                       SetPanelSkew(double value) ;
	//---------------------------------------------------------------------------------//	
	size_t                     GetPanelFdCount() const;//取得整板的定位點數量
	size_t                     GetPanelFdCount(DISTRICT_ID DistrictID) const;//取得整板的定位點數量
	CAOIFd*                    GetPanelFdPtr(size_t index, bool check) const;//取得整板的定位點指標
	CAOIFd*                    GetPanelFdPtr(size_t index, DISTRICT_ID DistrictID) const;//取得整板的定位點指標
	bool                       AddPanelFdPtr(CAOIFd *FdPtr);//增加整板的定位點
	bool                       SelectPanelAllFds(bool Select);//選取整板的定位點
	bool                       RemovePanelFdSelected();//移除選取到的整板的定位點
	bool                       RemovePanelAllFds();//移除整板的定位點
	bool                       LayoutPanelFdList();//重整整板的定位點列表
	bool                       CheckPanelFdCalculated(DISTRICT_ID DistrictID);//確認整板的定位點都計算過	
	bool                       ClearPanelFdImageBuffer(DISTRICT_ID DistrictID);//清除整板定位點影像資料
	size_t                     CalcPanelFdCount(DISTRICT_ID DistrictID);//計算只屬於整板定位點的數量
	size_t                     CalcPanelFdDefectCount(DISTRICT_ID DistrictID);//計算只屬於整板定位點瑕疵的數量
	unsigned int               CalcPanelFdInex(const CAOIFd *RefFdPtr);//確認整板的定位點引數-會忽略掛在單板下的定位點
	bool                       ExecPanelBeXBoard(LANE_ID LaneID);//執行整板是為報廢板	
	//---------------------------------------------------------------------------------//
	size_t                     GetPanelMarkCount() const;//取得整板的特徵點數量
	CAOIMark*                  GetPanelMarkPtr(size_t index, bool check) const;//取得整板的特徵點指標
	bool                       AddPanelMarkPtr(CAOIMark *MarkPtr);//增加整板的特徵點
	CAOIMark*                  GetPanelMarkPtrBySelected();//取得整板的選取的特徵點
	bool                       SelectPanelAllMarks(bool Select);//選取整板的特徵點
	bool                       RemovePanelMarkSelected();//移除選取到的整板的特徵點
	bool                       RemovePanelAllMarks();//移除整板的特徵點
	bool                       LayoutPanelMarkList();//重整整板的特徵點列表	
	size_t                     CalcPanelMarkCount();//計算只屬於整板特徵點的數量
	unsigned int               CalcPanelMarkInex(const CAOIMark *RefMarkPtr);//確認整板的特徵點引數-會忽略掛在單板下的特徵點
	//---------------------------------------------------------------------------------//
	size_t                     GetPanelBarcodeCount() const;//取得整板的軟體條碼數量
	CAOIBarcode*               GetPanelBarcodePtr(size_t index, bool check) const;//取得整板的軟體條碼指標
	bool                       AddPanelBarcodePtr(CAOIBarcode *BarcodePtr);//增加整板的軟體條碼
	bool                       SelectPanelAllBarcodes(bool Select);//選取整板的軟體條碼
	bool                       RemovePanelBarcodeSelected();//移除選取到的整板的軟體條碼
	bool                       RemovePanelAllBarcodes();//移除整板的軟體條碼
	bool                       LayoutPanelBarcodeList();//重整整板的軟體條碼列表	
	size_t                     CalcPanelBarcodeCount();//計算只屬於整板軟體條碼的數量
	unsigned int               CalcPanelBarcodeInex(const CAOIBarcode *RefBarcodePtr);//確認整板的軟體條碼引數-會忽略掛在單板下的條碼
	//---------------------------------------------------------------------------------//
	size_t                     GetPanelBoardCount() const;//取得整板的單板數量
	CAOIBoard*                 GetPanelBoardPtr(size_t index, bool check) const;//取得整板的單板指標
	bool                       AddPanelBoardPtr(CAOIBoard *BoardPtr);//增加整板的單板
	bool                       SelectPanelAllBoards(bool Select);//選取整板的單板
	bool                       RemovePanelBoard(CAOIBoard *BoardPtr);//移除整板的單板
	bool                       RemovePanelBoardSelected();//移除選取到的整板的單板
	bool                       RemovePanelAllBoards();//移除整板的單板
	bool                       LayoutPanelBoardList();//重整整板的單板列表
	bool                       LayoutPanelBoardListRegion();//重整整板的單板區域列表	
	bool                       LayoutPanelBoardListRegion(DISTRICT_ID DistrictID);//重整整板的單板區域列表	
	bool                       AnalyzePanelBoardOrientationMode();//分析整板內單板的方向性
	bool                       CompareComponentList(bool CmpPos, bool CmpAngle, const std::vector<CAOIComponent*> &List1, const std::vector<CAOIComponent*> &List2) const;
	//---------------------------------------------------------------------------------//	
	size_t                     GetPanelFieldCount() const;//取得整板的區域數量
	CAOIField*                 GetPanelFieldPtr(size_t index, bool check) const;//取得整板的區域指標
	bool                       AddPanelFieldPtr(CAOIField *FieldPtr);//增加整板的區域
	bool                       SelectPanelAllFields(bool Select);//選取整板的區域
	bool                       RemovePanelFieldSelected();//移除選取到的整板的區域
	bool                       RemovePanelAllFields();//移除整板的區域
	bool                       RemovePanelAllFields(DISTRICT_ID DistrictID);//移除整板的區域
	bool                       LayoutPanelFieldList();//重整整板的區域列表
	//---------------------------------------------------------------------------------//
	size_t                     GetPanelComponentCount() const;//取得整板的零件數量
	size_t                     GetPanelComponentNGCount() const;//取得整板的瑕疵零件數量	
	bool                       GetPanelComponentNGList(std::vector<CAOIComponent*> &List) const;//取得整板的瑕疵零件列表
	CAOIComponent*             GetPanelComponentPtr(size_t index, bool check) const;//取得整板的零件指標
	CAOIComponent*             GetPanelComponentPtrByName(LPCSTR ComponentName) const;//取得整板的零件指標
	CAOIComponent*             GetPanelComponentPtrByName(LPCWSTR ComponentName) const;//取得整板的零件指標
	bool                       AddPanelComponentPtr(CAOIComponent *ComponentPtr);//增加整板的零件
	bool                       AddPanelComponentList(std::vector<CAOIComponent*> &List);//增加整板的零件列表 
	bool                       SelectPanelAllComponents(bool Select);//選取整板的零件
	bool                       RemovePanelComponentSelected();//移除選取到的整板的零件
	bool                       RemovePanelAllComponents();//移除整板的零件	
	bool                       LayoutPanelComponentList();//重整整板的零件列表
	bool                       InvertSelectPanelComponent();//反向選取零件	
	bool                       SetPanelComponentOrgCadPos();//設定整板零件Cad座標為原始Cad座標
	bool                       ReplacePanelComponentModelName(std::vector<TAliasNode> &AliasList);//取代整板零件的模組名稱
	//---------------------------------------------------------------------------------//	
	bool                       SpinPanel(double Angle);//自轉整板
	void                       MovePanelPos(double dX, double dY);//移動整板	
	void                       MovePanelStagePos(double dX, double dY, DISTRICT_ID DistrictID);//移動整板	
	bool                       RotatePanel(double Angle, double CpX, double CpY);//旋轉整板	
	bool                       MirrorXPanel(double CpX);//鏡射整板-X值
	bool                       MirrorYPanel(double CpY);//鏡射整板-Y值
	//---------------------------------------------------------------------------------//	
	bool                       CheckPanelBePickByCad(const TPOINT2D &PickPos);//確認整板被點擊到
	bool                       CheckPanelBePickByStage(DISTRICT_ID DistrictID, const TPOINT2D &PickPos);//確認整板被點擊到

	bool                       CheckPanelInRegionByCad(const TREGION4D &SelRgn, bool bEntireIn);//確認整板在範圍內
	bool                       CheckPanelInRegionByStage(DISTRICT_ID DistrictID, const TREGION4D &SelRgn, bool bEntireIn);//確認整板在範圍內
	//---------------------------------------------------------------------------------//	
	bool                       CalcPanelBasePlaneParam(DISTRICT_ID DistrictID);//計算整板基準面	
	void                       SetPanelBasePlaneParam(DISTRICT_ID DistrictID, double nX, double nY, double nZ);//設定整板基準面向量
	void                       GetPanelBasePlaneParam(DISTRICT_ID DistrictID, double &nX, double &nY, double &nZ) const;//取得整板基準面向量	
	//---------------------------------------------------------------------------------//
	void                       SetPanelCalcMapFinish(bool value) { m_PanelCalcMapFinish=value; }//計算整板的座標轉換完成
	bool                       GetPanelCalcMapFinish() const { return m_PanelCalcMapFinish; }//計算整板的座標轉換完成
	//---------------------------------------------------------------------------------//
	bool                       BuildPanelDefaultMap(DISTRICT_ID DistrictID);//建立整板基本座標轉換參數	
	CMapCoordinate*            GetPanelMapCTSPtr(DISTRICT_ID DistrictID);//整板的座標轉換-Cad to Stage
	CMapCoordinate*            GetPanelMapSTCPtr(DISTRICT_ID DistrictID);//整板的座標轉換-Stage to Cad
	bool                       GetPanelMapCTS(DISTRICT_ID DistrictID, CMapCoordinate &Map);
	bool                       GetPanelMapSTC(DISTRICT_ID DistrictID, CMapCoordinate &Map);	
	bool                       CheckPanelMapCoordinate(DISTRICT_ID DistrictID, double dChkRatio);//確認整板的座標轉換機制
	bool                       CalcPanelMapParam();//計算整板座標轉換參數
	bool                       CalcPanelMapParam(DISTRICT_ID DistrictID);//計算整板座標轉換參數
	bool                       CalcPanelStagePosition();//計算整板座標
	bool                       CalcPanelStagePosition(DISTRICT_ID DistrictID);//計算整板座標
	bool                       CalcPanelCadResPosition_All();//計算整板Cad結果座標
	bool                       CalcPanelCadResPosition(DISTRICT_ID DistrictID);//計算整板Cad結果座標
	bool                       CalcPanelSkew(DISTRICT_ID DistrictID);//計算整板座標轉換參數
	bool                       AssignPanelMapParamToBoards();//將整板的座標轉換轉至各自單板內
	bool                       AssignPanelMapParamToBoards(DISTRICT_ID DistrictID);//將整板的座標轉換轉至各自單板內
	bool                       GetPanelCornerComponent(CAOIComponent *&Cp1, CAOIComponent *&Cp2, CAOIComponent *&Cp3, CAOIComponent *&Cp4, DISTRICT_ID DistrictID);//取得整板最靠近四端點的零件
	//---------------------------------------------------------------------------------//
	bool                       LayoutPanelRegion();//重整整板範圍
	bool                       LayoutPanelRegion(DISTRICT_ID DistrictID);//重整整板範圍	
	bool                       LayoutPanelRegionCad();//重整整板範圍	
	bool                       LayoutPanelRegionStage();//重整整板範圍	
	bool                       LayoutPanelRegionStage(DISTRICT_ID DistrictID);//重整整板範圍	
	//---------------------------------------------------------------------------------//	
	bool                       SetPanelCadPos(double PanelCadPosX, double PanelCadPosY);//設定整板的機台中心
	bool                       SetPanelCadPos(double PanelCadPosX, double PanelCadPosY, DISTRICT_ID DistrictID);//設定整板的機台中心
	//---------------------------------------------------------------------------------//
	bool                       SetPanelStagePos(double PanelStagePosX, double PanelStagePosY);//設定整板的機台中心
	bool                       SetPanelStagePos(double PanelStagePosX, double PanelStagePosY, DISTRICT_ID DistrictID);//設定整板的機台中心
	//---------------------------------------------------------------------------------//
	bool                       AssignPanelFieldList(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, FIELD_BUILD_AREA_MODE AreaMode);//分配區域列表	
	bool                       CreatePanelFieldList(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, std::vector<CAOIField*> &FieldPtrList, FIELD_DIVISION_MODE DivMode, bool ByCadRegion, FIELD_BUILD_AREA_MODE AreaMode);//建立整板區域列表
	bool                       CreatePanelFieldListKernel(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, FIELD_DIVISION_MODE DivisionMode, bool ByCadRegion, bool CheckOldField, FIELD_BUILD_AREA_MODE AreaMode, TJetRgnList &RgnList, TJetFieldList &FieldList);//建立區塊分割資料
	CAOIField*                 MatchPanelFieldPtr(const TREGION4D &Region, bool CadMode, bool Inner, DISTRICT_ID DistrictID);
	//---------------------------------------------------------------------------------//
	bool                       ResetPanelFrameImageRect(DISTRICT_ID DistrictID);//重新設定整板下物件的畫面影像位置
	//---------------------------------------------------------------------------------//
	bool                       UpdatePanelComponentToModel();//更新整板內的零件至零件模組內
	//---------------------------------------------------------------------------------//
	
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIPANEL_H__849E16D3_81AC_489D_B5C9_8C44A046B873__INCLUDED_)
