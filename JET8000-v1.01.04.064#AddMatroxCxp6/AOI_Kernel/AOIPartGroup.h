// AOIPartGroup.h: interface for the AOIGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIPARTGROUP_H__380C3FEC_F41C_41A4_97DF_2B8CB7B8FDBA__INCLUDED_)
#define AFX_AOIPARTGROUP_H__380C3FEC_F41C_41A4_97DF_2B8CB7B8FDBA__INCLUDED_
//-------------------------------------------------------------------------------------//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "AOIObj.h"
#include "AOIPartGroupDef.h"
//-------------------------------------------------------------------------------------//
class CAOIBoard;
class CAOIPanel;
class CAOIProject;
//-------------------------------------------------------------------------------------//
class CAOIPartGroup : public CAOIObj  
{
	//---------------------------------------------------------------------------------//
	DECLARE_DYNAMIC(CAOIPartGroup)	
	//---------------------------------------------------------------------------------//	
private:	
	//---------------------------------------------------------------------------------//
	CAOIProject               *m_PartGroupProjectPtr;//零件群組的專案指標
	//---------------------------------------------------------------------------------//
	PART_GROUP_MODE            m_PartGroupMode;//零件群組模式
	std::wstring               m_PartGroupName;//零件群組名稱
	unsigned int               m_PartGroupIndex;//零件群組引數
	int                        m_PartGroupUniqueID;//零件群組唯一碼
	int                        m_PartGroupGroupID;//零件群組零件群組編號
	bool                       m_PartGroupInOneBoard;//零件群組同個單板內
	//---------------------------------------------------------------------------------//	
	bool                       m_PartGroupDeleted;//零件群組刪除掉
	bool                       m_PartGroupSelected;//零件群組選取到
	//---------------------------------------------------------------------------------//
	TPartGroupNode              m_PartGroupNode1;//第1個資料點
	TPartGroupNode              m_PartGroupNode2;//第2個資料點
	TPartGroupNode              m_PartGroupNode3;//第3個資料點
	TPartGroupNode              m_PartGroupNode4;//第4個資料點
	std::vector<TPartGroupNode> m_PartGroupNodeList;//資料點列表
	//---------------------------------------------------------------------------------//	
	//Colinearity Parameter
	PART_GROUP_COLINEARITY_MODE m_ColinearityMode;//共線性模式
	PART_GROUP_TARGET_MODE     m_ColinearityTargetMode;//共線性目標值模式
	double                     m_ColinearityGapStd;//共線性偏差標準
	double                     m_ColinearityGapUSL;//共線性偏差上限
	double                     m_ColinearityGapLSL;//共線性偏差下限
	//---------------------------------------------------------------------------------//
	//Distance
	bool                       m_DistanceGapEnbX;//距離偏差啟用-X
	bool                       m_DistanceGapEnbY;//距離偏差啟用-Y
	bool                       m_DistanceGapEnbL;//距離偏差啟用-L
	double                     m_DistanceGapUSLX;//距離偏差上限-X
	double                     m_DistanceGapLSLX;//距離偏差下限-X
	double                     m_DistanceGapUSLY;//距離偏差上限-Y
	double                     m_DistanceGapLSLY;//距離偏差下限-Y
	double                     m_DistanceGapUSLL;//距離偏差上限-L=sqrt(XX+YY)
	double                     m_DistanceGapLSLL;//距離偏差下限-L=sqrt(XX+YY)
	bool                       m_DistanceGapStdEnb;//距離偏差啟用標準
	double                     m_DistanceGapStdX;//距離偏差標準-X
	double                     m_DistanceGapStdY;//距離偏差標準-Y
	double                     m_DistanceGapStdL;//距離偏差標準-L
	double                     m_DistanceGapAddX;//距離偏差加值-X
	double                     m_DistanceGapAddY;//距離偏差加值-Y
	bool                       m_DistanceGapEnbAbs;//距離偏差啟用絕對值
	double                     m_DistanceGapScaleX;//距離偏差倍率-X
	double                     m_DistanceGapScaleY;//距離偏差倍率-Y
	double                     m_DistanceGapScaleL;//距離偏差倍率-L
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//		
	void                       PreInitPartGroup();
	void                       InitialPartGroup();
	void                       ClonePartGroup(const CAOIPartGroup &others);
	//---------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------//	
public:
	//---------------------------------------------------------------------------------//
	CAOIPartGroup();
	CAOIPartGroup(const CAOIPartGroup &others);
	virtual ~CAOIPartGroup();
	CAOIPartGroup& operator=(const CAOIPartGroup &others);
	//---------------------------------------------------------------------------------//
	CAOIPartGroup* ClonePartGroupObj() const;//建立且複製一個零件群組物件
	//---------------------------------------------------------------------------------//	
	bool                       WritePartGroupFile(CAOIFileIO &FileIO);//儲存零件群組檔案
	bool                       ReadPartGroupFile(CAOIFileIO &FileIO);//載入零件群組檔案
	//---------------------------------------------------------------------------------//	
	bool                       WritePartGroupNodeFile(const TPartGroupNode &Node, CAOIFileIO &FileIO);//儲存零件群組節點檔案
	bool                       ReadPartGroupNodeFile(TPartGroupNode &Node, CAOIFileIO &FileIO);//載入零件群組節點檔案	
	//---------------------------------------------------------------------------------//	
	bool                       WritePartGroupNodeListFile(const std::vector<TPartGroupNode> &NodeList, CAOIFileIO &FileIO);//儲存節點列表檔案
	bool                       ReadPartGroupNodeListFile(std::vector<TPartGroupNode> &NodeList, CAOIFileIO &FileIO);//載入節點列表檔案	
	//---------------------------------------------------------------------------------//	
	bool                       WritePartGroupSpcFile_JSON_VRS(FILE *pfile, CAOIProject *ProjectPtr);//儲存檢測結果檔案JSON-VRS
	bool                       WritePartGroupNodeSpcFile_JSON_VRS(FILE *pfile, const TPartGroupNode &Node, CAOIProject *ProjectPtr);//儲存檢測結果檔案JSON-VRS
	//---------------------------------------------------------------------------------//		
	bool                       WritePartGroupSpcFile_JSON_RSM(FILE *pfile, bool &bFirst);//儲存檢測結果檔案JSON-RSM
	bool                       WritePartGroupNodeSpcFile_JSON_RSM(FILE *pfile, const TPartGroupNode &Node, bool &bFirst);//儲存檢測結果檔案JSON-RSM
	bool                       WritePartGroupNodeSpcFile_JSON_RSM(FILE *pfile, wchar_t *pName, double Val, double Std, double Usl, double Lsl, wchar_t *pUnit, wchar_t *pText, bool &bFirst);//儲存檢測結果檔案JSON-RSM
	//---------------------------------------------------------------------------------//	
	CAOIProject*               GetPartGroupProjectPtr() const ;//取得零件群組專案指標
	void                       SetPartGroupProjectPtr(CAOIProject* Ptr);//設定零件群組專案指標
	//---------------------------------------------------------------------------------//		
	PART_GROUP_MODE            GetPartGroupMode() const ;//取得零件群組模式
	void                       SetPartGroupMode(PART_GROUP_MODE Mode);//設定零件群組模式	
	//---------------------------------------------------------------------------------//
	const wchar_t*             GetPartGroupName() const ;//取得零件群組名稱
	void                       SetPartGroupName(const char *Name);//設定零件群組名稱
	void                       SetPartGroupName(const wchar_t *Name);//設定零件群組名稱
	//---------------------------------------------------------------------------------//
	unsigned int               GetPartGroupIndex() const ;//取得零件群組引數
	void                       SetPartGroupIndex(unsigned int val);//設定零件群組引數
	//---------------------------------------------------------------------------------//
	int                        GetPartGroupUniqueID() const ;//取得零件群組唯一碼
	void                       SetPartGroupUniqueID(int val);//設定零件群組唯一碼
	//---------------------------------------------------------------------------------//
	int                        GetPartGroupGroupID() const ;//取得零件群組編號
	void                       SetPartGroupGroupID(int val);//設定零件群組編號
	//---------------------------------------------------------------------------------//
	int                        GetPartGroupGroupID_UI() const ;//取得零件群組編號-介面
	void                       SetPartGroupGroupID_UI(int val);//設定零件群組編號-介面
	//---------------------------------------------------------------------------------//		
	bool                       GetPartGroupInOneBoard() const;//取得零件群組同個單板內
	void                       SetPartGroupInOneBoard(bool val);//設定零件群組同個單板內
	//---------------------------------------------------------------------------------//	
	bool                       GetPartGroupDeleted() const ;//取得零件群組刪除掉
	void                       SetPartGroupDeleted(bool val);//設定零件群組刪除掉
	//---------------------------------------------------------------------------------//
	bool                       GetPartGroupSelected() const ;//取得零件群組選取到
	void                       SetPartGroupSelected(bool val);//設定零件群組選取到
	//---------------------------------------------------------------------------------//
	void                       ClearPartGroupNode1();//清除資料點1
	TPartGroupNode*            GetPartGroupNodePtr1();//取得資料點1
	void                       GetPartGroupNode1(TPartGroupNode &Node);//取得資料點1
	void                       SetPartGroupNode1(const TPartGroupNode &Node);//設定資料點1
	//---------------------------------------------------------------------------------//
	void                       ClearPartGroupNode2();//清除資料點2
	TPartGroupNode*            GetPartGroupNodePtr2();//取得資料點2
	void                       GetPartGroupNode2(TPartGroupNode &Node);//取得資料點2
	void                       SetPartGroupNode2(const TPartGroupNode &Node);//設定資料點2
	//---------------------------------------------------------------------------------//
	void                       ClearPartGroupNode3();//清除資料點3
	TPartGroupNode*            GetPartGroupNodePtr3();//取得資料點3
	void                       GetPartGroupNode3(TPartGroupNode &Node);//取得資料點3
	void                       SetPartGroupNode3(const TPartGroupNode &Node);//設定資料點3
	//---------------------------------------------------------------------------------//
	void                       ClearPartGroupNode4();//清除資料點4
	TPartGroupNode*            GetPartGroupNodePtr4();//取得資料點4
	void                       GetPartGroupNode4(TPartGroupNode &Node);//取得資料點4
	void                       SetPartGroupNode4(const TPartGroupNode &Node);//設定資料點4
	//---------------------------------------------------------------------------------//
	void                       ClearPartGroupNodeList();//清除零件群組節點列表
	size_t                     GetPartGroupNodeCount() const;//取得零件群組節點數
	void                       RemovePartGroupNodePanelSelected();//清除選取到的零件群組節點列表
	void                       RemovePartGroupNodeBoardSelected();//清除選取到的零件群組節點列表
	void                       RemovePartGroupNodeComponentSelected();//清除選取到的零件群組節點列表
	void                       AddPartGroupNode(const TPartGroupNode &Node);//新增零件群組節點
	void                       AddPartGroupNodeList(const std::vector<TPartGroupNode> &NodeList);//新增零件群組節點
	void                       SetPartGroupNodeList(const std::vector<TPartGroupNode> &NodeList);//新增零件群組節點
	TPartGroupNode*            GetPartGroupNodePtr(size_t idx, bool bCheck);//取得零件群組節點指標
	bool                       ArrangePartGroupNodeList();//整理零件群組節點列表
	bool                       SortPartGroupNodeListByPosX();//排序零件群組節點列表-X座標
	bool                       SortPartGroupNodeListByPosY();//排序零件群組節點列表-Y座標
	bool                       SetPartGroupNodeListWndIndex(unsigned int WndIndex);//設定零件群組節點列表-檢測框編號
	//---------------------------------------------------------------------------------//	
	bool                       SetPartGroupParameterStringByID(PART_GROUP_PARAM_ID ParamID, LPCTSTR String);//設定零件群組參數
	bool                       GetPartGroupParameterStringByID(PART_GROUP_PARAM_ID ParamID, CString &String);//取得零件群組參數	
	//---------------------------------------------------------------------------------//
	bool                       CheckPartGroupNodeParamID(PART_GROUP_PARAM_ID ParamID) const;//確認是零件群組節點參數編號
	bool                       SetPartGroupNodeParameterStringByID(TPartGroupNode &Node, PART_GROUP_PARAM_ID ParamID, LPCTSTR String);//設定零件群組節點參數
	bool                       GetPartGroupNodeParameterStringByID(TPartGroupNode &Node, PART_GROUP_PARAM_ID ParamID, CString &String);//取得零件群組節點參數	
	//---------------------------------------------------------------------------------//
	PART_GROUP_COLINEARITY_MODE GetColinearityMode() const;//取得共線性模式
	void                       SetColinearityMode(PART_GROUP_COLINEARITY_MODE val);//設定共線性模式
	//---------------------------------------------------------------------------------//
	PART_GROUP_TARGET_MODE     GetColinearityTargetMode() const;//取得共線性目標值模式
	void                       SetColinearityTargetMode(PART_GROUP_TARGET_MODE val);//設定共線性目標值模式
	//---------------------------------------------------------------------------------//
	double                     GetColinearityGapStd() const;//取得共線性偏差標準
	void                       SetColinearityGapStd(double val);//設定共線性偏差標準
	//---------------------------------------------------------------------------------//
	double                     GetColinearityGapUSL() const;//取得共線性偏差上限
	void                       SetColinearityGapUSL(double val);//設定共線性偏差上限
	//---------------------------------------------------------------------------------//
	double                     GetColinearityGapLSL() const;//取得共線性偏差下限
	void                       SetColinearityGapLSL(double val);//設定共線性偏差下限	
	//---------------------------------------------------------------------------------//	
	bool                       GetDistanceGapEnbX() const;//取得距離偏差啟用-X
	void                       SetDistanceGapEnbX(bool val);//設定距離偏差啟用-X
	//---------------------------------------------------------------------------------//
	bool                       GetDistanceGapEnbY() const;//取得距離偏差啟用-Y
	void                       SetDistanceGapEnbY(bool val);//設定距離偏差啟用-Y
	//---------------------------------------------------------------------------------//
	bool                       GetDistanceGapEnbL() const;//取得距離偏差啟用-L
	void                       SetDistanceGapEnbL(bool val);//設定距離偏差啟用-L
	//---------------------------------------------------------------------------------//
	double                     GetDistanceGapUSLX() const;//取得距離偏差上限-X
	void                       SetDistanceGapUSLX(double val);//設定距離偏差上限-X
	//---------------------------------------------------------------------------------//
	double                     GetDistanceGapLSLX() const;//取得距離偏差下限-X
	void                       SetDistanceGapLSLX(double val);//設定距離偏差下限-X
	//---------------------------------------------------------------------------------//
	double                     GetDistanceGapUSLY() const;//取得距離偏差上限-Y
	void                       SetDistanceGapUSLY(double val);//設定距離偏差上限-Y
	//---------------------------------------------------------------------------------//
	double                     GetDistanceGapLSLY() const ;//取得距離偏差下限-Y
	void                       SetDistanceGapLSLY(double val);//設定距離偏差下限-Y
	//---------------------------------------------------------------------------------//
	double                     GetDistanceGapUSLL() const;//取得距離偏差上限-L
	void                       SetDistanceGapUSLL(double val);//設定距離偏差上限-L
	//---------------------------------------------------------------------------------//
	double                     GetDistanceGapLSLL() const;//取得距離偏差下限-L
	void                       SetDistanceGapLSLL(double val);//設定距離偏差下限-L
	//---------------------------------------------------------------------------------//	
	bool                       GetDistanceGapStdEnb() const;//取得距離偏差標準啟用
	void                       SetDistanceGapStdEnb(bool val);//設定距離偏差標準啟用
	//---------------------------------------------------------------------------------//
	double                     GetDistanceGapStdX() const;//取得距離偏差標準-X
	void                       SetDistanceGapStdX(double val);//設定距離偏差標準-X
	//---------------------------------------------------------------------------------//
	double                     GetDistanceGapStdY() const;//取得距離偏差標準-Y
	void                       SetDistanceGapStdY(double val);//設定距離偏差標準-Y
	//---------------------------------------------------------------------------------//
	double                     GetDistanceGapStdL() const;//取得距離偏差標準-L
	void                       SetDistanceGapStdL(double val);//設定距離偏差標準-L
	//---------------------------------------------------------------------------------//	
	double                     GetDistanceGapAddX() const;//取得距離偏差加值-X
	void                       SetDistanceGapAddX(double val);//設定距離偏差加值-X
	//---------------------------------------------------------------------------------//	
	double                     GetDistanceGapAddY() const;//取得距離偏差加值-Y
	void                       SetDistanceGapAddY(double val);//設定距離偏差加值-Y
	//---------------------------------------------------------------------------------//
	bool                       GetDistanceGapEnbAbs() const;//取得距離偏差啟用絕對值
	void                       SetDistanceGapEnbAbs(bool val);//設定距離偏差啟用絕對值
	//---------------------------------------------------------------------------------//		
	double                     GetDistanceGapScaleX() const;//距離偏差倍率-X
	void                       SetDistanceGapScaleX(double val);//距離偏差倍率-X
	//---------------------------------------------------------------------------------//
	double                     GetDistanceGapScaleY() const;//距離偏差倍率-Y
	void                       SetDistanceGapScaleY(double val);//距離偏差倍率-Y
	//---------------------------------------------------------------------------------//
	double                     GetDistanceGapScaleL() const;//距離偏差倍率-L
	void                       SetDistanceGapScaleL(double val);//距離偏差倍率-L
	//---------------------------------------------------------------------------------//
	bool                       InitPartGroupInspection();//初始化零件群組檢測
	bool                       SetGroupComponentKeepImage();//設定零件群組保留圖像
	CString                    BuildPartGroupDefectText(LPCTSTR Text) const;//建立零件群組瑕疵文字	
	//---------------------------------------------------------------------------------//
	CAOIBoard*                 GetPartGroupNodeBoardPtr();//取得零件群組節點單板指標
	bool                       CheckPartGroupNodeInOneBoard();//確認零件群組節點在同一個單板內
	bool                       ChangePartGroupNodeBoardPtr(CAOIBoard *BoardPtr);//變更零件群組節點的單板
	//---------------------------------------------------------------------------------//
	bool                       CopyPartGroupParam(const CAOIPartGroup &PartGroup);//複製其他同群組的資料
	bool                       CopyPartNodeParam(TPartGroupNode &Node, const TPartGroupNode &RefNode);//複製其他同群組節點的資料
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIPARTGROUP_H__380C3FEC_F41C_41A4_97DF_2B8CB7B8FDBA__INCLUDED_)
