// WndDefectItem.h: interface for the CWndDefectItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WNDDEFECTITEM_H__5E140662_98FA_4CE9_8928_BA64CF3C2C93__INCLUDED_)
#define AFX_WNDDEFECTITEM_H__5E140662_98FA_4CE9_8928_BA64CF3C2C93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#define WND_DEFECT_ITEM_DISABLE       0//檢測框瑕疵項目關閉
#define WND_DEFECT_ITEM_ENABLE        1//檢測框瑕疵項目啟用
#define WND_DEFECT_ITEM_NO_SHOW       2//檢測框瑕疵項目不顯示
//-------------------------------------------------------------------------------------//
class CAOIFileIO;
//-------------------------------------------------------------------------------------//
class CWndDefectItem  
{
private:
	//---------------------------------------------------------------------------------//
	LANE_ID                    m_LaneID;//軌道編號
	RESULT_ID                  m_ResultID;//結果編號
	int                        m_None;//無定義	
	int                        m_PadAlign;//焊盤定位
	int                        m_PartAlign;//本體定位
	int                        m_PadAdjust;//焊盤調整
	int                        m_LeadAdjust;//管腳調整

	int                        m_ClassCheck;//類別確認
	int                        m_BaseValue;//基準數值

	int                        m_BodyMissing;//缺件
	int                        m_BodyOffset;//偏移
	int                        m_BodyTilt;//本體傾斜
	int                        m_BodyPolarity;//極反
	int                        m_BodyTurnOver;//反件
	int                        m_BodyMount;//錯件-裝貼
	int                        m_BodyWrongCode;//錯件-條碼
	int                        m_BodyWrongText;//錯件-文字
	int                        m_BodyTombstone;//立碑
	int                        m_BodyBillboard;//側立
	int                        m_BodyDamaged;//破損
	
	int                        m_SolderPoor;//焊錫不足
	int                        m_SolderOpen;//焊錫空焊
	int                        m_SolderPadExposed;//焊錫沒有-漏銅
	int                        m_SolderBridge;//焊錫短路
	int                        m_SolderBead;//焊錫錫珠
	int                        m_SolderExcess;//焊錫過量

	int                        m_LeadLifted;//引腳翹起
	int                        m_LeadBended;//引腳彎曲
	int                        m_LeadProtruded;//引腳凸出

	int                        m_PadScratch;//焊盤刮傷
	int                        m_ForeignBody;//異物

	int                        m_UserDefine_01;//使用者定義-01
	int                        m_UserDefine_02;//使用者定義-02
	int                        m_UserDefine_03;//使用者定義-03
	int                        m_UserDefine_04;//使用者定義-04
	int                        m_UserDefine_05;//使用者定義-05
	int                        m_UserDefine_06;//使用者定義-06
	int                        m_UserDefine_07;//使用者定義-07
	int                        m_UserDefine_08;//使用者定義-08
	int                        m_UserDefine_09;//使用者定義-09
	int                        m_UserDefine_10;//使用者定義-10
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitWndDefectItem();
	void                       InitialWndDefectItem();
	void                       CloneWndDefectItem(const CWndDefectItem &rhs);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//		
	CWndDefectItem();
	virtual ~CWndDefectItem();
	//---------------------------------------------------------------------------------//	
	bool                       ReadWndDefectItemFile(CAOIFileIO &FileIO);
	bool                       WriteWndDefectItemFile(CAOIFileIO &FileIO) const;
	//---------------------------------------------------------------------------------//
	LANE_ID                    GetLaneID() const;
	void                       SetLaneID(LANE_ID val);
	//---------------------------------------------------------------------------------//	
	RESULT_ID                  GetResultID() const;
	void                       SetResultID(RESULT_ID val);
	//---------------------------------------------------------------------------------//
	void                       BuildWndDefectItemAlarm();//建成警報項目
	void                       BuildModelBasicTestItem(MODEL_TYPE Type);//建立模組基本檢測項目	
	//---------------------------------------------------------------------------------//	
	void                       SetAll(int Count) { SetAllItemCount(Count); }
	void                       SetAllItemCount(int Count);//設定所有項目數量	
	int                        CalcAllItemCount() const;//計算所有項目數量
	int                        CalcSum() const { return CalcAllItemCount(); }
	//---------------------------------------------------------------------------------//	
	bool                       AddItemCount(WND_DEFECT_ID DefectID);//增加特定瑕疵項目數量
	int                        GetItemCount(WND_DEFECT_ID DefectID) const;//取得特定瑕疵項目數量
	bool                       SetItemCount(WND_DEFECT_ID DefectID, int Count);//設定特定瑕疵項目數量	
	void                       AddWndDefectItemCount(const CWndDefectItem &rhs);//加入瑕疵項目數量
	//---------------------------------------------------------------------------------//	
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_WNDDEFECTITEM_H__5E140662_98FA_4CE9_8928_BA64CF3C2C93__INCLUDED_)
