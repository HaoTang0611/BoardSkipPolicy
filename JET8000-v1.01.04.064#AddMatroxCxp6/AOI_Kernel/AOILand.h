// AOILand.h: interface for the CAOILand class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOILAND_H__48E8A7BE_D534_439E_9031_522989C76C85__INCLUDED_)
#define AFX_AOILAND_H__48E8A7BE_D534_439E_9031_522989C76C85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "AOIObj.h"
#include "AOIBox.h"
#include "AOIWnd.h"
#include "AOILogic.h"
#include "AOIModelDef.h"
//-------------------------------------------------------------------------------------//
class CAOIModel;
class CAOIFileIO;
//-------------------------------------------------------------------------------------//
#define LAND_BOX_PAD                1
#define LAND_BOX_LEAD               2
#define LAND_BOX_LEAD_TIP           3
#define LAND_BOX_LEAD_SHOULDER      4
#define LAND_BOX_BODY_EDGE          5
//-------------------------------------------------------------------------------------//
typedef struct tagLandProperty
{
	int        nIndex;
	int        nLandGroupID;
	int        nLandCount;
	LAND_TYPE  eLandType;
	BOX_TOWARD eLandToward;
	bool       bPadAlign;
	bool       bPartAlign;
	double     dLeadSizeX;
	double     dLeadSizeY;
	double     dLeadHeight;
	double     dLeadTipSizeX;
	double     dLeadTipSizeY;
	double     dLeadTipHeight;
	double     dLeadShoulderSizeX;
	double     dLeadShoulderSizeY;
	double     dLeadShoulderHeight;

	tagLandProperty()
	{		
		nIndex = -1;
		nLandGroupID = -1;
		eLandType = LAND_TYPE_NULL;
		eLandToward = BOX_TOWARD_NULL;
		bPadAlign = true;
		bPartAlign = true;
		nLandCount = 0;
		dLeadSizeX = 0;
		dLeadSizeY = 0;
		dLeadHeight = 0;
		dLeadTipSizeX = 0;
		dLeadTipSizeY = 0;
		dLeadTipHeight = 0;
		dLeadShoulderSizeX = 0;
		dLeadShoulderSizeY = 0;
		dLeadShoulderHeight = 0;		
	}
} TLandProperty, *PLandProperty;
//-------------------------------------------------------------------------------------//
class CAOILand : public CAOIObj  
{
	//---------------------------------------------------------------------------------//
	DECLARE_DYNAMIC(CAOILand)		
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//	
	static bool                GetLandTypeUsePad(LAND_TYPE LandType);//確認焊接樣式使用焊盤
	static bool                GetLandTypeUseLead(LAND_TYPE LandType);//確認焊接樣式使用引腳
	static bool                GetLandTypeUseLeadTip(LAND_TYPE LandType);//確認焊接樣式使用引腳前端
	static bool                GetLandTypeUseLeadShoulder(LAND_TYPE LandType);//確認焊接樣式使用引腳肩部
	static bool                GetLandTypeUseLeadTipShoulder(LAND_TYPE LandType);//確認焊接樣式使用引腳前端+肩部
	//---------------------------------------------------------------------------------//	
private:
	//---------------------------------------------------------------------------------//
	LAND_TYPE                  m_LandType;                   //特徵框的樣式
	unsigned int               m_LandIndex;                  //特徵框的引數編號
	CString                    m_LandName;                   //特徵框的名稱
	int                        m_LandGroupID;                //特徵框的群組編號
	int                        m_LandAlignID;                //特徵框的對齊編號	
	bool                       m_LandModified;               //檢測框是否變更過
	bool                       m_LandFirstOne;               //特徵框的單側第一個位置
	bool                       m_LandLastOne;                //特徵框的單側第末個位置
	bool                       m_LandIncludePadAlign;        //特徵框是否加入焊盤定位
	bool                       m_LandIncludePartAlign;       //特徵框是否加入零件定位		
	int                        m_LandTempInt[4];             //特徵框的暫存整數	
	//---------------------------------------------------------------------------------//	
	double                     m_LandLeadSizeX;              //特徵框引腳尺寸-X
	double                     m_LandLeadSizeY;              //特徵框引腳尺寸-Y
	double                     m_LandLeadHeight;             //特徵框引腳高度
	//---------------------------------------------------------------------------------//	
	double                     m_LandLeadTipSizeX;           //特徵框引腳前端尺寸-X
	double                     m_LandLeadTipSizeY;           //特徵框引腳前端尺寸-Y
	double                     m_LandLeadTipHeight;          //特徵框引腳前端高度
	//---------------------------------------------------------------------------------//	
	double                     m_LandLeadShoulderSizeX;      //特徵框引腳根部尺寸-X
	double                     m_LandLeadShoulderSizeY;      //特徵框引腳根部尺寸-Y
	double                     m_LandLeadShoulderHeight;     //特徵框引腳根部高度
	//---------------------------------------------------------------------------------//	
	CColorGroup                m_LandLeadColorGroup;         //特徵框引腳顏色	
	//---------------------------------------------------------------------------------//	
	CAOIModel                 *m_LandModelPtr;               //特徵框所屬的模組
	//---------------------------------------------------------------------------------//	
	CAOIBox                   *m_LandBoxPtr;                 //特徵框的主框(焊盤框或者電極框)
	CAOIBox                    m_LandPadBox;                 //特徵框內的焊盤框
	CAOIBox                    m_LandLeadBox;                //特徵框內的引腳框或者電極框
	CAOIBox                    m_LandLeadTipBox;             //特徵框內的引腳前端框
	CAOIBox                    m_LandLeadShoulderBox;        //特徵框內的引腳根部框
	CAOIBox                    m_LandBodyEdgeBox;             //特徵框內的本體框
	//---------------------------------------------------------------------------------//	
	std::vector<CAOIWnd*>      m_LandWndList;                //特徵框內的檢測框列表
	std::vector<CAOILogic*>    m_LandLogicList;              //特徵框內的邏輯閘的列表
	//---------------------------------------------------------------------------------//
	TREGION4D                  m_LandRegion;               //特徵框的範圍-模組內
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitLand();
	void                       InitialLand();
	void                       InitialLandColorGroup();
	void                       CloneLand(const CAOILand &Land);
	//---------------------------------------------------------------------------------//
	bool                       LayoutLeadBox(bool bResult);
	bool                       LayoutLeadTipBox(bool bResult);
	bool                       LayoutLeadShoulderBox(bool bResult);
	//---------------------------------------------------------------------------------//
	void                       UpdateLandBoxPtr();//更新特徵框的主框
	void                       EnableLandBasicBox(CAOIBox &Box);//啟用特徵基本框
	void                       DisableLandBasicBox(CAOIBox &Box);//關閉特徵基本框
	//---------------------------------------------------------------------------------//
	size_t                     GetLandWndCount_Inline() const;//取得特徵框的檢測框數量
	void                       AddLandWndPtr_Inline(CAOIWnd *WndPtr);//加入特徵框的檢測框
	void                       RemoveLandWndList_Inline();//移除特徵框的檢測框
	CAOIWnd*                   GetLandWndPtr_Inline(size_t idx) const;//取得特徵框的檢測框指標	
	//---------------------------------------------------------------------------------//
	size_t                     GetLandLogicCount_Inline() const;//取得特徵框的邏輯閘數量
	void                       AddLandLogicPtr_Inline(CAOILogic *LogicPtr);//加入特徵框的邏輯閘
	void                       RemoveLandLogicList_Inline();//移除特徵框的邏輯閘
	CAOILogic*                 GetLandLogicPtr_Inline(size_t index) const;//取得特徵框的邏輯閘指標	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CAOILand();
	CAOILand(const CAOILand &Land);
	virtual ~CAOILand();
	CAOILand& operator=(const CAOILand &Land);
	//---------------------------------------------------------------------------------//
	void                       ResetLandObj();//復歸Land物件
	void                       ReleaseLandObj();//釋放Land物件
	CAOILand* CloneLandObj() const;//建立且複製一個特徵框
	//---------------------------------------------------------------------------------//		
	bool                       WriteLandFile(CAOIFileIO &FileIO);//儲存特徵框檔案
	bool                       ReadLandFile(CAOIFileIO &FileIO);//載入特徵框檔案
	//---------------------------------------------------------------------------------//		
	bool                       WriteLandLeadSpcHeader_JSON_detail(FILE *pfile, CAOIProject *ProjectPtr);
	bool                       WriteLandPadSpcHeader_JSON_detail(FILE *pfile, CAOIProject *ProjectPtr);
	//---------------------------------------------------------------------------------//		
	bool                       SwitchLandType(LAND_TYPE Type);//切換特徵框樣式
	LAND_TYPE                  GetLandType() const { return m_LandType; }
	//---------------------------------------------------------------------------------//		
	void                       SetLandIndex(unsigned int value) { m_LandIndex = value; }
	unsigned int               GetLandIndex() const { return m_LandIndex; }
	//---------------------------------------------------------------------------------//
	void                       SetLandName(CString str) { m_LandName = str; }
	CString                    GetLandName() { return m_LandName; }
	//---------------------------------------------------------------------------------//
	void                       SetLandGroupID(int value) { m_LandGroupID = value; }
	int                        GetLandGroupID() const { return m_LandGroupID; }
	//---------------------------------------------------------------------------------//
	void                       SetLandAlignID(int value) { m_LandAlignID = value; }
	int                        GetLandAlignID() const { return m_LandAlignID; }
	//---------------------------------------------------------------------------------//	
	void                       SetLandModified(bool value) { m_LandModified = value; }
	bool                       GetLandModified() const { return m_LandModified; }
	//---------------------------------------------------------------------------------//
	void                       SetLandFirstOne(bool value) { m_LandFirstOne = value; }
	bool                       GetLandFirstOne() const { return m_LandFirstOne; }
	//---------------------------------------------------------------------------------//	
	void                       SetLandLastOne(bool value) { m_LandLastOne = value; }
	bool                       GetLandLastOne() const { return m_LandLastOne; }
	//---------------------------------------------------------------------------------//	
	//特徵框是否加入焊盤定位
	void                       SetLandIncludePadAlign(bool value) { m_LandIncludePadAlign = value; }
	bool                       GetLandIncludePadAlign() const { return m_LandIncludePadAlign; }
	//---------------------------------------------------------------------------------//		
	//特徵框是否加入零件定位
	void                       SetLandIncludePartAlign(bool value) { m_LandIncludePartAlign = value; }
	bool                       GetLandIncludePartAlign() const { return m_LandIncludePartAlign; }
	//---------------------------------------------------------------------------------//	
	//特徵框暫存變數
	void                       SetLandTempInt1(int value) { m_LandTempInt[0] = value; }
	int                        GetLandTempInt1() const { return m_LandTempInt[0]; }
	void                       SetLandTempInt2(int value) { m_LandTempInt[1] = value; }
	int                        GetLandTempInt2() const { return m_LandTempInt[1]; }
	void                       SetLandTempInt3(int value) { m_LandTempInt[2] = value; }
	int                        GetLandTempInt3() const { return m_LandTempInt[2]; }
	void                       SetLandTempInt4(int value) { m_LandTempInt[3] = value; }
	int                        GetLandTempInt4() const { return m_LandTempInt[3]; }
	void                       SetLandTempInt(int value, int idx=0) { m_LandTempInt[idx] = value; }
	int                        GetLandTempInt(int idx=0) const { return m_LandTempInt[idx]; }	
	//---------------------------------------------------------------------------------//	
	//特徵框引腳尺寸-X
	void                       SetLandLeadSizeX(double value) { m_LandLeadSizeX = value; }
	double                     GetLandLeadSizeX() const { return m_LandLeadSizeX; }
	//---------------------------------------------------------------------------------//	
	//特徵框引腳尺寸-Y
	void                       SetLandLeadSizeY(double value) { m_LandLeadSizeY = value; }
	double                     GetLandLeadSizeY() const { return m_LandLeadSizeY; }
	//---------------------------------------------------------------------------------//	
	//特徵框引腳高度
	void                       SetLandLeadHeight(double value) { m_LandLeadHeight = value; }
	double                     GetLandLeadHeight() const { return m_LandLeadHeight; }
	//---------------------------------------------------------------------------------//	
	//特徵框引腳前端尺寸-X
	void                       SetLandLeadTipSizeX(double value) { m_LandLeadTipSizeX = value; }
	double                     GetLandLeadTipSizeX() const { return m_LandLeadTipSizeX; }
	//---------------------------------------------------------------------------------//	
	//特徵框引腳前端尺寸-Y
	void                       SetLandLeadTipSizeY(double value) { m_LandLeadTipSizeY = value; }
	double                     GetLandLeadTipSizeY() const { return m_LandLeadTipSizeY; }
	//---------------------------------------------------------------------------------//	
	//特徵框引腳前端高度
	void                       SetLandLeadTipHeight(double value) { m_LandLeadTipHeight = value; }
	double                     GetLandLeadTipHeight() const { return m_LandLeadTipHeight; }
	//---------------------------------------------------------------------------------//	
	//特徵框引腳根部尺寸-X
	void                       SetLandLeadShoulderSizeX(double value) { m_LandLeadShoulderSizeX = value; }
	double                     GetLandLeadShoulderSizeX() const { return m_LandLeadShoulderSizeX; }
	//---------------------------------------------------------------------------------//	
	//特徵框引腳根部尺寸-Y
	void                       SetLandLeadShoulderSizeY(double value) { m_LandLeadShoulderSizeY = value; }
	double                     GetLandLeadShoulderSizeY() const { return m_LandLeadShoulderSizeY; }
	//---------------------------------------------------------------------------------//	
	//特徵框引腳根部高度
	void                       SetLandLeadShoulderHeight(double value) { m_LandLeadShoulderHeight = value; }
	double                     GetLandLeadShoulderHeight() const { return m_LandLeadShoulderHeight; }
	//---------------------------------------------------------------------------------//	
	TLandProperty              GetLandProperty() const;
	void                       GetLandProperty(TLandProperty &Propty) const;
	void                       SetLandProperty(const TLandProperty &Propty);	
	//---------------------------------------------------------------------------------//	
	bool                       CheckLandLinkPos(CAOILand *LandPtr) const;//確認特徵框位置連動
	bool                       CheckLandLinkSize(CAOILand *LandPtr) const;//確認特徵框尺寸連動
	//---------------------------------------------------------------------------------//	
	//特徵框引腳顏色	
	void                       SetLandLeadColorGroup(const CColorGroup &value) { m_LandLeadColorGroup = value; }
	CColorGroup&               GetLandLeadColorGroup() { return m_LandLeadColorGroup; }
	//---------------------------------------------------------------------------------//	
	void                       SetLandModelPtr(CAOIModel *Ptr) { m_LandModelPtr=Ptr; }	
	CAOIModel*                 GetLandModelPtr() { return m_LandModelPtr; }
	//---------------------------------------------------------------------------------//	
	CAOIBox*                   GetLandBasicBoxPtrSelected();
	int                        GetLandBasicBoxID(const CAOIBox *BoxPtr) const;	
	CAOIBox*                   GetLandBasicBoxPtr(int BasicBoxID);	
	//---------------------------------------------------------------------------------//	
	CAOIBox*                   GetLandBoxPtr() { return m_LandBoxPtr; }
	//---------------------------------------------------------------------------------//	
	void                       SetLandPadBox(const CAOIBox &Box) { m_LandPadBox=Box; }	
	CAOIBox&                   GetLandPadBox() { return m_LandPadBox; }
	CAOIBox*                   GetLandPadBoxPtr() { return &m_LandPadBox; }
	//---------------------------------------------------------------------------------//	
	void                       SetLandLeadBox(const CAOIBox &Box) { m_LandLeadBox=Box; }	
	CAOIBox&                   GetLandLeadBox() { return m_LandLeadBox; }
	CAOIBox*                   GetLandLeadBoxPtr() { return &m_LandLeadBox; }
	//---------------------------------------------------------------------------------//	
	void                       SetLandLeadTipBox(const CAOIBox &Box) { m_LandLeadTipBox=Box; }	
	CAOIBox&                   GetLandLeadTipBox() { return m_LandLeadTipBox; }
	CAOIBox*                   GetLandLeadTipBoxPtr() { return &m_LandLeadTipBox; }
	//---------------------------------------------------------------------------------//	
	void                       SetLandLeadShoulderBox(const CAOIBox &Box) { m_LandLeadShoulderBox=Box; }	
	CAOIBox&                   GetLandLeadShoulderBox() { return m_LandLeadShoulderBox; }
	CAOIBox*                   GetLandLeadShoulderBoxPtr() { return &m_LandLeadShoulderBox; }
	//---------------------------------------------------------------------------------//	
	void                       SetLandBodyEdgeBox(const CAOIBox &Box) { m_LandBodyEdgeBox=Box; }	
	CAOIBox&                   GetLandBodyEdgeBox() { return m_LandBodyEdgeBox; }
	CAOIBox*                   GetLandBodyEdgeBoxPtr() { return &m_LandBodyEdgeBox; }
	//---------------------------------------------------------------------------------//	
	bool                       LayoutLeadBox(CAOIBox *BoxPtr, bool bResult);	
	//---------------------------------------------------------------------------------//
	void                       SetLandAllBoxActived(bool value);//設定是否焦點狀態
	void                       SetLandAllBoxSelected(bool value);//設定是否選取狀態
	void                       SetLandAllBoxVisibled(bool value);//設定是否顯示狀態
	bool                       GetLandActived();//取得是否為焦點狀態
	bool                       GetLandSelected();//取得是否為選取狀態
	
	bool                       UnSelectLand(bool ToWnd);//取消特徵框選取狀態	
	bool                       VisibleLand(bool ToWnd);//開?特徵框顯示狀態
	bool                       InvisibleLand(bool ToWnd);//取消特徵框顯示狀態
	//---------------------------------------------------------------------------------//
	bool                       SetLandToward(BOX_TOWARD value);//設定特徵框朝向
	BOX_TOWARD                 GetLandToward() const { return CAOILand::m_LandPadBox.GetBoxToward(); }
	//---------------------------------------------------------------------------------//
	size_t                     GetLandWndCount() const;//取得特徵框的檢測框數量	
	bool                       AddLandWndPtr(CAOIWnd *WndPtr);//增加特徵框的檢測框		
	CAOIWnd*                   GetLandWndPtrLastOne() const;//取得特徵框的最後個檢測框指標	
	CAOIWnd*                   GetLandWndPtrFirstOne() const;//取得特徵框的第1個檢測框指標	
	CAOIWnd*                   GetLandWndPtr(size_t Index, bool Check) const;//取得特徵框的檢測框指標		
	CAOIWnd*                   GetLandWndPtrByGroupID(int WndGroupID, int WndBandID) const;	
	void                       SetLandWndSelected(bool val); 
	CAOIWnd*                   GetLandWndSelected() const; 	
	void                       RemoveLandWndList();//移除特徵框的檢測框
	bool                       RemoveLandWndSelected();	
	bool                       LayoutLandWndList();
	//---------------------------------------------------------------------------------//
	size_t                     GetLandLogicCount() const;
	CAOILogic*                 GetLandLogicPtr(size_t index, bool Check) const;
	CAOILogic*                 GetLandLogicPtrByGroupID(int LogicGroupID) const;
	void                       SetLandLogicSelected(bool val); 
	CAOILogic*                 GetLandLogicSelected() const; 
	bool                       AddLandLogicPtr(CAOILogic *LogicPtr);
	void                       RemoveLandLogicList();
	bool                       RemoveLandLogicSelected();
	//---------------------------------------------------------------------------------//	
	void                       UpdateLandTotalRegion();//更新特徵框的整個範圍
	void                       CalcLandTotalRegionStage(TREGION4D &Region);//計算特徵框的整個基台範圍-
	void                       GetLandTotalRegion(TREGION4D &Region) const { Region=m_LandRegion; }
	//---------------------------------------------------------------------------------//
	void                       AlignLand();
	void                       AlignLandU();//特徵框的軸向相同
	void                       AlignLandV();
	bool                       CheckLandLeadFollowPad() const;//確認引腳跟焊盤移動
	void                       ScaleLandProperty(double sx, double sy);//縮放特徵框屬性
	void                       ScaleLand(double sx, double sy, bool bIncludeRes=true);//縮放特徵框
	void                       MoveLand(double x, double y, bool bIncludeRes=true);	
	void                       MoveLandResult(double x, double y);
	void                       MoveLandPad(double x, double y, bool bIncludeRes=true);
	void                       MoveLandLead(double x, double y, bool bIncludeRes=true);
	void                       MoveLandPadResult(double x, double y);
	void                       MoveLandLeadResult(double x, double y);	
	void                       ModifyLandLeadSize(const CAOIBox *BoxPtr, const TREGION4D &dPos, bool bIncludeRes=true);
	void                       ModifyLandLeadSizeRes(const CAOIBox *BoxPtr, const TREGION4D &dPos);
	void                       RotateLand(double Angle, double CPX, double CPY);
	void                       MirrorLandXAxis(double CPY);
	void                       MirrorLandYAxis(double CPX);
	//---------------------------------------------------------------------------------//
	void                       SetLandAttachedAngle(double Angle);
	void                       SetLandAttachedPosCad(const TPOINT2D &Pos);
	void                       SetLandAttachedPosCad(double PosX, double PosY);
	void                       SetLandAttachedPosStage(const TPOINT2D &Pos);
	void                       SetLandAttachedPosStage(double PosX, double PosY);
	//---------------------------------------------------------------------------------//
	bool                       InitLandInspection();//初始化特徵框檢測
	//---------------------------------------------------------------------------------//
	bool                       ApplyLand(const CAOILand *RefLandPtr);//套用相同群組的特徵框
	bool                       SynchronousLand(const CAOILand *RefLandPtr);//同步化同一個特徵框
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOILAND_H__48E8A7BE_D534_439E_9031_522989C76C85__INCLUDED_)
