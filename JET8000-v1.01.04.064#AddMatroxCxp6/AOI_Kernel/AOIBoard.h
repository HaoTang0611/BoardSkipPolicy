// AOIBoard.h: interface for the CAOIBoard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIBOARD_H__6FE649C2_9C7F_4EF4_A532_CEED100B2A43__INCLUDED_)
#define AFX_AOIBOARD_H__6FE649C2_9C7F_4EF4_A532_CEED100B2A43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include <vector>
#include "AOIObj.h"
#include "AOIFd.h"
#include "AOIMark.h"
#include "AOIBarcode.h"
#include "AOIComponent.h"
#include "AOIPartGroup.h"
#include "MapCoordinate.h"
#include "JetFieldDivider.h"
//-------------------------------------------------------------------------------------//
class CAOIBoard;
class CAOIPanel;
class CAOIFileIO;
class CAOIProject;
//-------------------------------------------------------------------------------------//
enum BOARD_TYPE
{
	BOARD_TYPE_DUMMY      = 0,	
	BOARD_TYPE_NORMAL     = 1,	
	BOARD_TYPE_RETURN
};
//-------------------------------------------------------------------------------------//
typedef struct tagBoardRect
{	
	unsigned int   PanelIndex;
	unsigned int   BoardIndex;
	CAOIPanel     *PanelPtr;
	CAOIBoard     *BoardPtr;	
	bool           Visibled;
	TREGION4D      BoardRgn;
	TRECT4D        BoardMapRect;	
	tagBoardRect()
	{
		PanelIndex = -1;
		BoardIndex = -1;		
		PanelPtr = NULL;
		BoardPtr = NULL;
		Visibled = true;
		BoardRgn = TREGION4D();
		BoardMapRect = TRECT4D();
	}
} TBoardRect, *PBoardRect; 
//-------------------------------------------------------------------------------------//
class CAOIBoard : public CAOIObj  
{
	//---------------------------------------------------------------------------------//	
	DECLARE_DYNAMIC(CAOIBoard)
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//		
	CAOIProject               *m_BoardProjectPtr;//單板的專案指標
	//---------------------------------------------------------------------------------//
	CAOIPanel                 *m_BoardPanelPtr;//單板的整板指標
	unsigned int               m_BoardPanelIndex_Project;//單板的整板引數編號
	//---------------------------------------------------------------------------------//
	CAOIField                 *m_BoardPanelFieldPtr_DA;//單板的整板區域指標-A區
	CAOIField                 *m_BoardPanelFieldPtr_DB;//單板的整板區域指標-B區	
	//---------------------------------------------------------------------------------//
	CString                    m_BoardErrorString;//單板的錯誤訊息
	unsigned int               m_BoardIndex_Project;//單板在專案的引數編號
	unsigned int               m_BoardIndex_Panel;//單板在整板的引數編號
	//---------------------------------------------------------------------------------//
	int                        m_BoardTempInt[4];//單板暫存整數
	//---------------------------------------------------------------------------------//	
	bool                       m_BoardDeleted;//單板是否刪除
	bool                       m_BoardSelected;//單板是否選取到	
	bool                       m_BoardBypassed;//單板是否不檢測		
	double                     m_BoardRoatedAngle;//單板旋轉角度	
	BOARD_TYPE                 m_BoardType;	
	TRECT4D                    m_BoardMapRect;//單板在底圖的範圍
	BOARD_SIDE_MODE            m_BoardSideMode;//單板板面方向
	BOARD_ORIENTATION_MODE     m_BoardOrientationMode;//單板方向定義
	bool                       m_BoardModified;//單板變更過
	bool                       m_BoardBarcodeEnabled;//單板條碼啟用
	double                     m_BoardXBoardCheckRatio;//單板X板確認比例
	bool                       m_BoardMultiDistrictMode;//單板多段模式
	//---------------------------------------------------------------------------------//
	RESULT_ID                  m_BoardResultID_AOI;//單板檢測結果-AOI
	RESULT_ID                  m_BoardResultID_AOI_LA;//單板檢測結果-AOI-A軌
	RESULT_ID                  m_BoardResultID_AOI_LB;//單板檢測結果-AOI-B軌
	//---------------------------------------------------------------------------------//
	RESULT_ID                  m_BoardResultID_ARS;//單板檢測結果-ARS
	RESULT_ID                  m_BoardResultID_ARS_LA;//單板檢測結果-ARS-A軌
	RESULT_ID                  m_BoardResultID_ARS_LB;//單板檢測結果-ARS-B軌
	//---------------------------------------------------------------------------------//
	RESULT_ID                  m_BoardResultID_Alarm;//單板檢測結果-警報
	DISTRICT_ID                m_BoardActDistrictID;//單板分段編號
	BOARD_FD_GRAB_MODE         m_BoardBoardFdGrabMode;//單板定位點取像模式
	//---------------------------------------------------------------------------------//
	size_t                     m_BoardXBoardUnitCount;//報廢件的單位數量
	size_t                     m_BoardXBoardUnitTotalCount;//報廢件的單位總數量
	//---------------------------------------------------------------------------------//
	bool                       m_BoardIsGetBarcode;//單板條碼是否已經取得
	std::wstring               m_BoardBarcode;//單板條碼
	std::wstring               m_BoardBarcodeBackup;//單板條碼備份
	unsigned int               m_BoardBarcodeDeviceIndex;//單板條碼機編號
	unsigned int               m_BoardBarcodeDeviceCodeIndex;//單板條碼機第幾碼
	BARCODE_BELONG_MODE        m_BoardBarcodeBelongMode;//單板條碼屬於模式
	//---------------------------------------------------------------------------------//
	TREGION4D                  m_BoardRgnCad;//單板範圍-Cad
	TREGION4D                  m_BoardRgnStage_DA;//單板範圍-Stage	
	TREGION4D                  m_BoardRgnStage_DB;//單板範圍-Stage
	//---------------------------------------------------------------------------------//	
	bool                       m_BoardMapEnable;//單板的座標轉換啟用
	CMapCoordinate             m_BoardMapCTS;//單板的座標轉換-Cad to Stage
	CMapCoordinate             m_BoardMapSTC;//單板的座標轉換-Stage to Cad
	CMapCoordinate             m_BoardMapCTS_DB;//單板的座標轉換-Cad to Stage
	CMapCoordinate             m_BoardMapSTC_DB;//單板的座標轉換-Stage to Cad
	bool                       m_BoardCalcMapFinish;//計算整板的座標轉換完成
	//---------------------------------------------------------------------------------//	
	std::vector<CAOIFd*>       m_BoardFdPtrList;//單板內的定位點列表	
	std::vector<CAOIMark*>     m_BoardMarkPtrList;//單板內的特徵點指標列表
	std::vector<CAOIField*>    m_BoardFieldPtrList;//單板內的檢測區域指標列表
	std::vector<CAOIBarcode*>  m_BoardBarcodePtrList;//單板內的軟體條碼列表
	std::vector<CAOIComponent*> m_BoardComponentPtrList;//單板內的零件列表
	//---------------------------------------------------------------------------------//
	std::vector<CAOIRgn*>      m_BoardRgnPtrListTemp;//單板內的檢測區域指標列表-暫時用
	std::vector<CAOIField*>    m_BoardFieldPtrListTemp;//單板內的區域指標列表-暫時用
	std::vector<CAOIComponent*> m_BoardComponentPtrListTemp;//單板內的零件列表-暫時用
	//---------------------------------------------------------------------------------//
	std::vector<CAOIPartGroup*> m_BoardPartGroupPtrList;//單板內的零件群組列表
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitBoard();
	void                       InitialBoard();
	void                       CloneBoard(const CAOIBoard &board);
	//---------------------------------------------------------------------------------//
	size_t                     GetBoardFdCount_Inline() const;//取得單板的定位點數量
	void                       AddBoardFdPtr_Inline(CAOIFd *FdPtr);//增加單板的定位點
	CAOIFd*                    GetBoardFdPtr_Inline(size_t index)  const;//取得單板的定位點指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetBoardMarkCount_Inline() const;//取得單板的特徵點數量
	void                       AddBoardMarkPtr_Inline(CAOIMark *MarkPtr);//增加單板的特徵點
	CAOIMark*                  GetBoardMarkPtr_Inline(size_t index) const;//取得單板的特徵點指標	
	//---------------------------------------------------------------------------------//	
	size_t                     GetBoardBarcodeCount_Inline() const;//取得單板的軟體條碼數量
	void                       AddBoardBarcodePtr_Inline(CAOIBarcode *BarcodePtr);//增加單板的軟體條碼
	CAOIBarcode*               GetBoardBarcodePtr_Inline(size_t index) const;//取得單板的軟體條碼指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetBoardFieldCount_Inline() const;//取得單板的區域數量
	void                       AddBoardFieldPtr_Inline(CAOIField *FieldPtr);//增加單板的區域
	CAOIField*                 GetBoardFieldPtr_Inline(size_t index) const;//取得單板的區域指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetBoardComponentCount_Inline() const;//取得單板的零件數量
	void                       AddBoardComponentPtr_Inline(CAOIComponent *ComponentPtr);//增加單板的零件
	CAOIComponent*             GetBoardComponentPtr_Inline(size_t index) const;//取得單板的零件指標	
	//---------------------------------------------------------------------------------//	
	size_t                     GetBoardPartGroupCount_Inline() const;//取得單板的零件群組數量
	void                       AddBoardPartGroupPtr_Inline(CAOIPartGroup *PartGroupPtr);//增加單板的零件群組
	CAOIPartGroup*             GetBoardPartGroupPtr_Inline(size_t index) const;//取得單板的零件群組指標	
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	CAOIBoard();
	CAOIBoard(const CAOIBoard &board);
	virtual ~CAOIBoard();
	CAOIBoard& operator=(const CAOIBoard &board);
	//---------------------------------------------------------------------------------//	
	CAOIBoard*                 CloneBoardObj() const;//建立且複製一個單板
	//---------------------------------------------------------------------------------//
	bool                       WriteBoardFile(CAOIFileIO &FileIO);//儲存單板檔案
	bool                       ReadBoardFile(CAOIFileIO &FileIO);//載入單板檔案
	//---------------------------------------------------------------------------------//	
	bool                       WriteBoardSpcFile_JSON_VRS(FILE *pfile, CAOIProject *ProjectPtr);
	bool                       WriteBoardSpcFile_JSON_RSM(FILE *pfile, CAOIProject *ProjectPtr);
	bool                       WriteBoardSpcHeader_JSON_detail(FILE *pfile, CAOIProject *ProjectPtr);
	//---------------------------------------------------------------------------------//
	bool                       ConvertToSpcBoard(DISTRICT_ID DistrictID, TSpcBoard &SpcBoard);//轉成SPC單板
	//---------------------------------------------------------------------------------//	
	void                       RemoveBoardAllObjects();//移除單板的所有物件
	//---------------------------------------------------------------------------------//
	void                       SelectBoardAllObjects(bool value);//選取單板內所有物件
	void                       RemoveBoardObjectSelected();//移除單板內選取到的物件
	bool                       SetBoardAllObjectTempInt(int val, int idx=0);//設定單板內物件暫存參數
	bool                       SetBoardAllObjectTempInt(DISTRICT_ID DistrictID, int val, int idx=0);//設定單板內物件暫存參數
	bool                       CheckBoardAllObjectInOneField(DISTRICT_ID DistrictID) const;//確認單板所有物件視野是同一個
	bool                       BuildBoardObjectListInRegion(DISTRICT_ID DistrictID, const TREGION4D &FieldRgn, bool ByCadRegion, std::vector<CAOIRgn*> &RgnList);
	//---------------------------------------------------------------------------------//		
	void                       SetBoardProjectPtr(CAOIProject *Ptr);//單板的專案指標
	CAOIProject*               GetBoardProjectPtr() const;//單板的專案指標
	//---------------------------------------------------------------------------------//	
	void                       SetBoardPanelPtr(CAOIPanel *Ptr);//單板的整板指標
	CAOIPanel*                 GetBoardPanelPtr() const;//單板的整板指標
	//---------------------------------------------------------------------------------//
	void                       ResetBoardPanelFieldParam(DISTRICT_ID DistrictID);//清除單板的整板區域參數
	//---------------------------------------------------------------------------------//
	void                       SetBoardPanelFieldPtr(CAOIField *Ptr, DISTRICT_ID DistrictID);//單板的整板區域指標
	CAOIField*                 GetBoardPanelFieldPtr(DISTRICT_ID DistrictID) const;//單板的整板區域指標
	//---------------------------------------------------------------------------------//
	void                       SetBoardPanelFieldPtr_DA(CAOIField *Ptr);//單板的整板區域指標
	CAOIField*                 GetBoardPanelFieldPtr_DA() const;//單板的整板區域指標
	//---------------------------------------------------------------------------------//
	void                       SetBoardPanelFieldPtr_DB(CAOIField *Ptr);//單板的整板區域指標
	CAOIField*                 GetBoardPanelFieldPtr_DB() const;//單板的整板區域指標
	//---------------------------------------------------------------------------------//
	bool                       SetBoardPanelFieldToObj(CAOIField *FieldPtr);//設定單板的整板區域指標至底層物件
	//---------------------------------------------------------------------------------//			
	LPCTSTR                    GetBoardErrorString() const;
	void                       SetBoardErrorString(LPCTSTR val);
	//---------------------------------------------------------------------------------//	
	unsigned int               GetBoardIndex_Project() const;
	void                       SetBoardIndex_Project(unsigned int value);	
	//---------------------------------------------------------------------------------//	
	unsigned int               GetBoardIndex_Panel() const;
	void                       SetBoardIndex_Panel(unsigned int value);		
	//---------------------------------------------------------------------------------//
	unsigned int               GetBoardPanelIndex_Project() const;
	void                       SetBoardPanelIndex_Project(unsigned int value);	
	//---------------------------------------------------------------------------------//		
	void                       UpdateBoardIndexToObjList();//更新單板引數至單板內的物件
	//---------------------------------------------------------------------------------//		
	int                        GetBoardTempInt() const;//單板暫存整數
	void                       SetBoardTempInt(int value);	//單板暫存整數
	//---------------------------------------------------------------------------------//	
	int                        GetBoardTempInt_01() const;//單板暫存整數
	void                       SetBoardTempInt_01(int value);//單板暫存整數
	//---------------------------------------------------------------------------------//		
	bool                       GetBoardDeleted() const;
	void                       SetBoardDeleted(bool value);	
	//---------------------------------------------------------------------------------//	
	bool                       GetBoardSelected() const;
	void                       SetBoardSelected(bool value);	
	//---------------------------------------------------------------------------------//	
	bool                       GetBoardBypassed() const;
	void                       SetBoardBypassed(bool value);	
	bool                       UpdateBoardBypassed();//更新單板不檢測至零件內
	bool                       ExecBoardBypassed(LANE_ID LaneID);//執行單板是為不檢測
	//---------------------------------------------------------------------------------//	
	bool                       GetBoardSkipped() const;//取得單板是否跳過檢測
	bool                       CheckBoardBypassedSkipped() const;//確認單板是否不用檢測
	//---------------------------------------------------------------------------------//	
	bool                       ExecBoardBeXBoard(LANE_ID LaneID);//執行單板是為報廢板	
	bool                       AnalyzeBoardIsXBoard();//分析單板是否為報廢板
	bool                       AnalyzeBoardXBoardUnitCount();//分析單板報廢板數量	
	//---------------------------------------------------------------------------------//		
	size_t                     GetBoardXBoardUnitCount() const;
	void                       SetBoardXBoardUnitCount(size_t val);	
	void                       IncrementBoardXBoardUnitCount();//增加單板報廢件數量
	//---------------------------------------------------------------------------------//	
	size_t                     GetBoardXBoardUnitTotalCount() const;
	void                       SetBoardXBoardUnitTotalCount(size_t val);	
	//---------------------------------------------------------------------------------//	
	double                     GetBoardRoatedAngle() const;
	void                       SetBoardRoatedAngle(double value);	
	//---------------------------------------------------------------------------------//		
	BOARD_TYPE                 GetBoardType() const;
	void                       SetBoardType(BOARD_TYPE value);	
	//---------------------------------------------------------------------------------//		
	void                       GetBoardMapRect(TRECT4D &val) const;//單板在底圖的範圍
	void                       SetBoardMapRect(const TRECT4D &val);	//單板在底圖的範圍
	//---------------------------------------------------------------------------------//	
	BOARD_SIDE_MODE            GetBoardSideMode() const;//單板板面方向
	void                       SetBoardSideMode(BOARD_SIDE_MODE value);//單板板面方向
	//---------------------------------------------------------------------------------//
	BOARD_ORIENTATION_MODE     GetBoardOrientationMode() const;//單板方向定義
	void                       SetBoardOrientationMode(BOARD_ORIENTATION_MODE value);//單板方向定義	
	//---------------------------------------------------------------------------------//		
	bool                       GetBoardModified() const;//單板變更過
	void                       SetBoardModified(bool value);//單板變更過	
	//---------------------------------------------------------------------------------//	
	bool                       GetBoardBarcodeEnabled() const;//單板條碼啟用
	void                       SetBoardBarcodeEnabled(bool value);//單板條碼啟用
	//---------------------------------------------------------------------------------//
	double                     GetBoardXBoardCheckRatio() const;//單板X板確認比例
	void                       SetBoardXBoardCheckRatio(double value);//單板X板確認比例	
	//---------------------------------------------------------------------------------//		
	bool                       GetBoardMultiDistrictMode() const;//單板多段模式
	void                       SetBoardMultiDistrictMode(bool value);//單板多段模式	
	//---------------------------------------------------------------------------------//		
	void                       SetBoardResultID(RESULT_ID value);//單板檢測結果
	RESULT_ID                  GetBoardResultID() const;//單板檢測結果
	//---------------------------------------------------------------------------------//	
	void                       SetBoardResultID_AOI_LA(RESULT_ID value);//單板檢測結果-AOI-A軌
	RESULT_ID                  GetBoardResultID_AOI_LA() const;//單板檢測結果-AOI-A軌
	//---------------------------------------------------------------------------------//	
	void                       SetBoardResultID_AOI_LB(RESULT_ID value);//單板檢測結果-AOI-B軌
	RESULT_ID                  GetBoardResultID_AOI_LB() const;//單板檢測結果-AOI-B軌
	//---------------------------------------------------------------------------------//	
	void                       UpdateBoardResultID_AOI_Lane(LANE_ID LaneID);//單板檢測結果-AOI-軌道
	RESULT_ID                  GetBoardResultID_AOI_Lane(LANE_ID LaneID) const;//單板檢測結果-AOI-軌道
	//---------------------------------------------------------------------------------//
	RESULT_ID                  GetBoardResultID_ARS() const;//單板檢測結果-ARS
	void                       SetBoardResultID_ARS(RESULT_ID value);//單板檢測結果-ARS	
	//---------------------------------------------------------------------------------//
	RESULT_ID                  GetBoardResultID_ARS_LA() const;//單板檢測結果-ARS-A軌
	void                       SetBoardResultID_ARS_LA(RESULT_ID value);//單板檢測結果-ARS-A軌
	//---------------------------------------------------------------------------------//
	RESULT_ID                  GetBoardResultID_ARS_LB() const;//單板檢測結果-ARS-B軌
	void                       SetBoardResultID_ARS_LB(RESULT_ID value);//單板檢測結果-ARS-B軌
	//---------------------------------------------------------------------------------//
	void                       UpdateBoardResultID_ARS_Lane(LANE_ID LaneID);//單板檢測結果-ARS-軌道
	RESULT_ID                  GetBoardResultID_ARS_Lane(LANE_ID LaneID) const;//單板檢測結果-ARS--軌道
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetBoardResultID_Alarm() const;//單板檢測結果-警報
	void                       SetBoardResultID_Alarm(RESULT_ID value);//單板檢測結果-警報	
	//---------------------------------------------------------------------------------//	
	DISTRICT_ID                GetBoardActDistrictID() const;//單板分段編號
	void                       SetBoardActDistrictID(DISTRICT_ID value);//單板分段編號
	//---------------------------------------------------------------------------------//	
	BOARD_FD_GRAB_MODE         GetBoardBoardFdGrabMode() const;//單板定位點取像模式
	void                       SetBoardBoardFdGrabMode(BOARD_FD_GRAB_MODE value);//單板定位點取像模式
	//---------------------------------------------------------------------------------//	
	TREGION4D                  GetBoardRgnCad() const;//單板範圍-Cad	
	void                       GetBoardRgnCad(TREGION4D &Rgn) const;//單板範圍-Cad	
	void                       SetBoardRgnCad(const TREGION4D &Rgn);//單板範圍-Cad	
	//---------------------------------------------------------------------------------//	
	TREGION4D                  GetBoardRgnStage(DISTRICT_ID DistrictID) const;//單板範圍-Stage
	void                       GetBoardRgnStage(DISTRICT_ID DistrictID, TREGION4D &Rgn) const;//單板範圍-Stage
	void                       SetBoardRgnStage(DISTRICT_ID DistrictID, const TREGION4D &Rgn);//單板範圍-Stage
	//---------------------------------------------------------------------------------//
	bool                       CheckBoardBePickByCad(const TPOINT2D &PickPos);//確認單板被點擊到
	bool                       CheckBoardBePickByStage(DISTRICT_ID DistrictID, const TPOINT2D &PickPos);//確認單板被點擊到

	bool                       CheckBoardInRegionByCad(const TREGION4D &SelRgn, bool bEntireIn);//確認單板在範圍內
	bool                       CheckBoardInRegionByStage(DISTRICT_ID DistrictID, const TREGION4D &SelRgn, bool bEntireIn);//確認單板在範圍內
	//---------------------------------------------------------------------------------//
	void                       SetBoardBarcode(const char *barcode);//設定單板條碼
	void                       SetBoardBarcode(const wchar_t *barcode);//設定單板條碼
	const wchar_t*             GetBoardBarcode() const;//取得單板條碼
	//---------------------------------------------------------------------------------//
	void                       BackupBoardBarcode();//備份單板條碼
	void                       RestoreBoardBarcode();//恢復單板條碼	
	//---------------------------------------------------------------------------------//		
	bool                       GetBoardIsGetBarcode() const;//單板條碼是否已經取得
	void                       SetBoardIsGetBarcode(bool value);//單板條碼是否已經取得
	//---------------------------------------------------------------------------------//	
	unsigned int               GetBoardBarcodeDeviceIndex() const;//單板條碼機編號
	void                       SetBoardBarcodeDeviceIndex(unsigned int value);//單板條碼機編號	
	//---------------------------------------------------------------------------------//	
	unsigned int               GetBoardBarcodeDeviceCodeIndex() const;//單板條碼機第幾碼
	void                       SetBoardBarcodeDeviceCodeIndex(unsigned int value);//單板條碼機第幾碼	
	//---------------------------------------------------------------------------------//
	BARCODE_BELONG_MODE        GetBoardBarcodeBelongMode() const;//單板條碼屬於模式
	void                       SetBoardBarcodeBelongMode(BARCODE_BELONG_MODE value);//單板條碼屬於模式
	//---------------------------------------------------------------------------------//	
	bool                       CheckBoardNeedToCalculate(DISTRICT_ID DistrictID) const;//確認單板需要檢測	
	//---------------------------------------------------------------------------------//	
	bool                       ChangeBoardPanel(CAOIPanel *PanelPtr);//變更單板的整板
	//---------------------------------------------------------------------------------//
	size_t                     GetBoardFdCount() const;//取得單板的定位點數量
	size_t                     CalcBoardFdCount(DISTRICT_ID DistrictID);//計算只屬於單板定位點的數量
	size_t                     CalcBoardFdDefectCount(DISTRICT_ID DistrictID);//計算單板定位點瑕疵的數量
	CAOIFd*                    GetBoardFdPtr(size_t index, bool check) const;//取得單板的定位點指標
	bool                       AddBoardFdPtr(CAOIFd *FdPtr);//增加單板的定位點
	bool                       SelectBoardAllFds(bool Select);//選取單板的定位點
	bool                       RemoveBoardFdSelected();//移除選取到的單板的定位點
	bool                       RemoveBoardAllFds();//移除單板的定位點
	bool                       LayoutBoardFdList();//重整單板的定位點列表	
	bool                       CheckBoardFdCalculated(DISTRICT_ID DistrictID);//確認單板的定位點都計算過	
	bool                       ClearBoardFdImageBuffer(DISTRICT_ID DistrictID);//清除單板定位點影像資料
	//---------------------------------------------------------------------------------//
	size_t                     GetBoardMarkCount() const;//取得單板的特徵點數量
	CAOIMark*                  GetBoardMarkPtr(size_t index, bool check) const;//取得單板的特徵點指標	
	bool                       AddBoardMarkPtr(CAOIMark *MarkPtr);//增加單板的特徵點
	CAOIMark*                  GetBoardMarkPtrBySelected();//取得單板的選到的特徵點
	int                        GetBoardMarkMaxLocalBasePlaneID();//取得單板的特徵點最大局部基準面編號
	bool                       SelectBoardAllMarks(bool Select);//選取單板的特徵點
	bool                       RemoveBoardMarkSelected();//移除選取到的單板的特徵點
	bool                       RemoveBoardAllMarks();//移除單板的特徵點
	bool                       LayoutBoardMarkList();//重整單板的特徵點列表
	//---------------------------------------------------------------------------------//
	size_t                     GetBoardBarcodeCount() const;//取得單板的軟體條碼數量
	CAOIBarcode*               GetBoardBarcodePtr(size_t index, bool check) const;//取得單板的軟體條碼指標
	bool                       AddBoardBarcodePtr(CAOIBarcode *BarcodePtr);//增加單板的軟體條碼
	bool                       SelectBoardAllBarcodes(bool Select);//選取單板的軟體條碼
	bool                       RemoveBoardBarcodeSelected();//移除選取到的單板的軟體條碼
	bool                       RemoveBoardAllBarcodes();//移除單板的軟體條碼
	bool                       LayoutBoardBarcodeList();//重整單板的軟體條碼列表
	//---------------------------------------------------------------------------------//
	size_t                     GetBoardFieldCount() const;//取得單板的區域數量
	CAOIField*                 GetBoardFieldPtr(size_t index, bool check) const;//取得單板的區域指標
	bool                       AddBoardFieldPtr(CAOIField *FieldPtr);//增加單板的區域
	bool                       SelectBoardAllFields(bool Select);//選取單板的區域
	bool                       RemoveBoardFieldSelected();//移除選取到的單板的區域
	bool                       RemoveBoardAllFields();//移除單板的區域
	bool                       RemoveBoardAllFields(DISTRICT_ID DistrictID);//移除單板的區域	
	bool                       LayoutBoardFieldList();//重整單板的區域列表
	size_t                     CalcBoardLastFieldGrabIndex(bool FdFirst) const;//計算單板最後區域取像的引數	
	//---------------------------------------------------------------------------------//	
	size_t                     GetBoardRgnTempCount() const;//取得單板的暫時檢測區域數量
	CAOIRgn*                   GetBoardRgnTempPtr(size_t index, bool check) const;//取得單板的暫時檢測區域指標
	bool                       AddBoardRgnTempPtr(CAOIRgn *Rgn, bool chkExist);//增加單板的暫時檢測區域
	bool                       ClearBoardRgnTempList();//移除單板的暫時檢測區域	
	bool                       LayoutBoardRgnTempList();//排列單板的暫時檢測區域	
	//---------------------------------------------------------------------------------//
	size_t                     GetBoardFieldTempCount() const;//取得單板的暫時區域數量
	CAOIField*                 GetBoardFieldTempPtr(size_t index, bool check) const;//取得單板的暫時區域指標
	bool                       AddBoardFieldTempPtr(CAOIField *FieldPtr, bool chkExist);//增加單板的暫時區域
	bool                       ClearBoardFieldTempList();//移除單板的暫時區域	
	//---------------------------------------------------------------------------------//
	std::vector<CAOIComponent*>& GetBoardComponentPtrTempList();//取得單板暫時的零件列表	
	bool                         ClearBoardComponentPtrTempList();//清除單板暫時的零件列表	
	size_t                       GetBoardComponentPtrTempCount() const;//取得單板暫時的零件數量
	CAOIComponent*               GetBoardComponentPtrTempPtr(size_t index, bool check);//取得單板暫時的零件指標
	bool                         SetBoardComponentPtrTempPtr(size_t index, CAOIComponent *Ptr);//設定單板暫時的零件指標
	void                         SetBoardComponentPtrTempList(const std::vector<CAOIComponent*> &List);//設定單板暫時的零件列表	
	//---------------------------------------------------------------------------------//
	size_t                     GetBoardComponentCount() const;//取得單板的零件數量
	size_t                     GetBoardComponentNGCount() const;//取得單板的瑕疵零件數量
	bool                       GetBoardComponentNGList(std::vector<CAOIComponent*> &List) const;//取得單板的瑕疵零件列表
	size_t                     GetBoardComponentNotAgentCount() const;//取得單板的非代理零件數量	
	size_t                     CalcBoardComponentBypassCount() const;//計算單板的不檢測零件數量
	size_t                     CalcBoardComponentXBoardUnitCount() const;//計算單板的報廢板零件數量
	CAOIComponent*             GetBoardComponentPtr(size_t index, bool check) const;//取得單板的零件指標
	CAOIComponent*             GetBoardComponentPtrByName(LPCSTR ComponentName) const;//取得單板的零件指標
	CAOIComponent*             GetBoardComponentPtrByName(LPCWSTR ComponentName) const;//取得單板的零件指標
	CAOIComponent*             GetBoardComponentPtrByName(LPCSTR ComponentName, size_t Count) const;//取得單板的零件指標
	CAOIComponent*             GetBoardComponentPtrByName(LPCWSTR ComponentName, size_t Count) const;//取得單板的零件指標
	size_t                     GetBoardComponentFreeNameIdx(LPCTSTR BaseName) const;//取得單板的零件無使用的引數
	bool                       GetBoardComponentMaxFreeNameIdx(LPCTSTR BaseName, size_t &MaxIdx, size_t &FreeIdx) const;//取得單板的零件最大名稱引數
	bool                       AddBoardComponentList(std::vector<CAOIComponent*> &List);//加入單板的零件列表
	void                       CloneBoardComponentList(std::vector<CAOIComponent*> &List) const;//複製單板的零件列表
	bool                       AddBoardComponentPtr(CAOIComponent *ComponentPtr);//增加單板的零件
	bool                       SelectBoardAllComponents(bool Select);//選取單板的零件
	bool                       RemoveBoardComponentSelected();//移除選取到的單板的零件
	bool                       RemoveBoardAllComponents();//移除單板的零件
	bool                       LayoutBoardComponentList();//重整單板的零件列表
	bool                       InvertSelectBoardComponent();//反向選取零件	
	bool                       ChceckBoardComponentNameExist(LPCSTR ComponentName);//確認單板零件名稱存在
	bool                       ChceckBoardComponentNameExist(LPCWSTR ComponentName);//確認單板零件名稱存在
	bool                       ChceckBoardComponentNameExist(LPCSTR ComponentName, size_t Count);//確認單板零件名稱存在
	bool                       ChceckBoardComponentNameExist(LPCWSTR ComponentName, size_t Count);//確認單板零件名稱存在
	//---------------------------------------------------------------------------------//
	size_t                     GetBoardPartGroupCount() const;//取得單板的零件群組數量
	void                       AddBoardPartGroupPtr(CAOIPartGroup *PartGroupPtr);//增加單板的零件群組
	CAOIPartGroup*             GetBoardPartGroupPtr(size_t index, bool check) const;//取得單板的零件群組指標	
	bool                       RemoveBoardAllPartGroups();//移除單板的零件群組
	//---------------------------------------------------------------------------------//
	bool                       GetBoardCalcMapFinish() const;//計算整板的座標轉換完成
	void                       SetBoardCalcMapFinish(bool value);//計算整板的座標轉換完成	
	//---------------------------------------------------------------------------------//
	bool                       GetBoardMapEnable() const;
	void                       SetBoardMapEnable(bool value);	
	//---------------------------------------------------------------------------------//
	bool                       BuildBoardDefaultMap(DISTRICT_ID DistrictID);//建立整板基本座標轉換參數
	CMapCoordinate*            GetBoardMapCTSPtr(DISTRICT_ID DistrictID);//整板的座標轉換-Cad to Stage
	CMapCoordinate*            GetBoardMapSTCPtr(DISTRICT_ID DistrictID);//整板的座標轉換-Stage to Cad
	bool                       GetBoardMapCTS(DISTRICT_ID DistrictID, CMapCoordinate &Map);//整板的座標轉換-Cad to Stage
	bool                       GetBoardMapSTC(DISTRICT_ID DistrictID, CMapCoordinate &Map);//整板的座標轉換-Stage to Cad
	bool                       SetBoardMapCTS(DISTRICT_ID DistrictID, CMapCoordinate *MapPtr);
	bool                       SetBoardMapSTC(DISTRICT_ID DistrictID, CMapCoordinate *MapPtr);
	bool                       CheckBoardMapCoordinate();//確認整板的座標轉換機制
	bool                       CheckBoardMapCoordinate(DISTRICT_ID DistrictID);//確認整板的座標轉換機制
	bool                       CalcBoardMapParam();//計算單板座標轉換參數
	bool                       CalcBoardMapParam(DISTRICT_ID DistrictID);//計算單板座標轉換參數
	bool                       CalcBoardStagePosition(bool bCalcFov=true);//計算單板座標
	bool                       CalcBoardStagePosition(DISTRICT_ID DistrictID, bool bCalcFov);//計算單板座標
	bool                       CalcBoardStagePosition(DISTRICT_ID DistrictID, const CMapCoordinate &Map, bool bCalcFov);////計算單板座標
	//---------------------------------------------------------------------------------//
	bool                       CalcBoardCadResPosition_All();//計算單板Cad結果座標
	bool                       CalcBoardCadResPosition(DISTRICT_ID DistrictID);//計算單板Cad結果座標
	bool                       CalcBoardCadResPosition(DISTRICT_ID DistrictID, const CMapCoordinate &Map);////計算單板Cad結果座標	
	//---------------------------------------------------------------------------------//
	bool                       SpinBoard(double Angle);//自轉單板
	void                       MoveBoardPos(double dX, double dY);//移動單板	
	bool                       RotateBoard(double Angle, double CpX, double CpY);//旋轉單板	
	bool                       MirrorXBoard(double CpX);//鏡射單板-X值
	bool                       MirrorYBoard(double CpY);//鏡射單板-Y值	
	//---------------------------------------------------------------------------------//
	bool                       LayoutBoardRegion();//重整單板範圍
	bool                       LayoutBoardRegion(DISTRICT_ID DistrictID);//重整單板範圍	
	bool                       LayoutBoardRegionCad();//重整單板範圍	
	bool                       LayoutBoardRegionStage();//重整單板範圍	
	bool                       LayoutBoardRegionStage(DISTRICT_ID DistrictID);//重整單板範圍		
	//---------------------------------------------------------------------------------//
	bool                       AssignBoardFieldList(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID);//分配區域列表	
	bool                       CreateBoardFieldList(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, std::vector<CAOIField*> &FieldPtrList, FIELD_DIVISION_MODE DivMode, bool ByCadRegion);//建立整板區域列表
	bool                       CreateBoardFieldListKernel(FIELD_BUILD_MODE BuildMode, DISTRICT_ID DistrictID, FIELD_DIVISION_MODE DivisionMode, bool ByCadRegion, bool CheckOldField, TJetRgnList &RgnList, TJetFieldList &FieldList);//建立區塊分割資料
	CAOIField*                 MatchBoardFieldPtr(const TREGION4D &Region, bool CadMode, bool Inner, DISTRICT_ID DistrictID);
	//---------------------------------------------------------------------------------//
	bool                       ResetBoardFrameImageRect(DISTRICT_ID DistrictID);//重新設定單板下物件的畫面影像位置
	//---------------------------------------------------------------------------------//	
	bool                       AssignBoardLocalPlaneParam(int GroupID, bool bInvertY, const CJetGroundEquation &BoardGround);//曲面參數	
	//---------------------------------------------------------------------------------//
	bool                       BuildBoardPartBarcodeList(std::vector<TPartBarcode> &PartBarcodeList);//建立單板內零件條碼列表
	//---------------------------------------------------------------------------------//		
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIBOARD_H__6FE649C2_9C7F_4EF4_A532_CEED100B2A43__INCLUDED_)
