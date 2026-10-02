// ParamUni.h: interface for the CParamUni class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAMUNI_H__9069A156_A56C_457C_8612_349F8744D441__INCLUDED_)
#define AFX_PARAMUNI_H__9069A156_A56C_457C_8612_349F8744D441__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
enum PARAM_DATA_TYPE
{
	PARAM_DATA_STR        = 1,
	PARAM_DATA_INT        = 2,
	PARAM_DATA_DBL        = 3,
	PARAM_DATA_CLR        = 4,
	PARAM_DATA_SEL        = 5
}; 
//-------------------------------------------------------------------------------------//
class CParamUni  
{
private:
	//---------------------------------------------------------------------------------//
	TCHAR                      m_Delimiter;
	//---------------------------------------------------------------------------------//
	CString                    m_sSection;//INI的區間名稱
	CString                    m_sKeyName;//INI的關鍵名稱
	CString                    m_sCaption;//變數的標頭名稱
	CString                    m_sTempText;//暫存的文字
	//---------------------------------------------------------------------------------//
	UINT                       m_WndCtrlID;//控制項編號
	int                        m_nItem;//列表的引數
	int                        m_nSubItem;//列表的子引數	
	CWnd                      *m_BtnWndPtr;//按鈕視窗
	CWnd                      *m_BtnWndPtr2;//按鈕視窗-2
	CWnd                      *m_ListCtrlPtr;//控制視窗
	CString                    m_ParamText;//顯示的文字
	COLORREF                   m_TextColor;//文字顏色
	//---------------------------------------------------------------------------------//
	bool                       m_bReadOnly;//唯讀狀態	
	int                        m_Precision;//精度
	bool                       m_bChanged;//是否變更	
	bool                       m_bReStart;//需要重啟
	UINT                       m_ParamID;//特定的資料
	size_t                     m_ParamIndex;//特定的引數
	PARAM_DATA_TYPE            m_DataType;//資料格式		
	CString                    m_Description;//說明提示
	//---------------------------------------------------------------------------------//
	CString                    m_Value_STR;	
	CString                    m_Default_STR;	
	//---------------------------------------------------------------------------------//		
	int                        m_Value_INT;
	int                        m_Default_INT;	
	//---------------------------------------------------------------------------------//	
	double                     m_Value_DBL;
	double                     m_Default_DBL;	
	//---------------------------------------------------------------------------------//
	COLORREF                   m_Value_CLR;
	COLORREF                   m_Default_CLR;	
	//---------------------------------------------------------------------------------//	
	DWORD                      m_SelParam;
	DWORD                      m_SelDefault;	
	std::vector<CString>       m_SelTextList;
	//---------------------------------------------------------------------------------//
	union uRange //Data Range
	{  
	   int    INT;
	   double DBL;
	} m_Max, m_Min;
	//-------------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//	
	void                       PreInitParam();
	void                       InitialParam();
	void                       CloneParam(const CParamUni &Param);
	//---------------------------------------------------------------------------------//		
public:
	//---------------------------------------------------------------------------------//	
	CParamUni();
	CParamUni(const CParamUni &Param);
	virtual ~CParamUni();
	//---------------------------------------------------------------------------------//	
	CParamUni& operator=(const CParamUni &Param);
	//---------------------------------------------------------------------------------//
	void                       SetItemIndex(int val) { m_nItem = val; }	
	int                        GetItemIndex() const { return m_nItem; }
	//---------------------------------------------------------------------------------//
	void                       SetSubItemIndex(int val) { m_nSubItem = val; }	
	int                        GetSubItemIndex() const { return m_nSubItem; }
	//---------------------------------------------------------------------------------//	
	void                       SetParamID(UINT val) { m_ParamID = val; }
	UINT                       GetParamID() const { return m_ParamID; }
	//---------------------------------------------------------------------------------//	
	void                       SetParamIndex(size_t val) { m_ParamIndex = val; }
	size_t                     GetParamIndex() const { return m_ParamIndex; }
	//---------------------------------------------------------------------------------//		
	void                       SetReadOnly(bool val) { m_bReadOnly = val; }
	bool                       GetReadOnly() const { return m_bReadOnly; }
	//---------------------------------------------------------------------------------//	
	void                       SetBtnWndPtr(CWnd *Ptr) { m_BtnWndPtr = Ptr; }	
	CWnd*                      GetBtnWndPtr() { return m_BtnWndPtr; }
	//---------------------------------------------------------------------------------//
	void                       SetBtnWndPtr2(CWnd *Ptr) { m_BtnWndPtr2 = Ptr; }	
	CWnd*                      GetBtnWndPtr2() { return m_BtnWndPtr2; }
	//---------------------------------------------------------------------------------//
	void                       SetListCtrl(CWnd *Ptr) { m_ListCtrlPtr = Ptr; }	
	CWnd*                      GetListCtrl() { return m_ListCtrlPtr; }
	//---------------------------------------------------------------------------------//	
	void                       SetWndCtrlID(UINT val) { m_WndCtrlID = val; }	
	UINT                       GetWndCtrlID() { return m_WndCtrlID; }
	//---------------------------------------------------------------------------------//
	PARAM_DATA_TYPE            GetDataType() const { return m_DataType; }
	//---------------------------------------------------------------------------------//	
	void                       SetTextColor(COLORREF val) { m_TextColor = val; }	
	COLORREF                   GetTextColor() { return m_TextColor; }
	//---------------------------------------------------------------------------------//
	//ini 區間名稱
	void                       SetSection(LPCTSTR str);
	LPCTSTR                    GetSection() const;
	//---------------------------------------------------------------------------------//
	//ini 關鍵名稱
	void                       SetKeyName(LPCTSTR str);
	LPCTSTR                    GetKeyName() const;
	//---------------------------------------------------------------------------------//	
	//標題
	void                       SetCaption(LPCTSTR str);
	LPCTSTR                    GetCaption() const;
	//---------------------------------------------------------------------------------//
	//說明提示
	void                       SetDesction(LPCTSTR str);
	LPCTSTR                    GetDesction() const;
	//---------------------------------------------------------------------------------//
	//暫存文字
	void                       SetTempText(LPCTSTR str);
	LPCTSTR                    GetTempText() const;
	//---------------------------------------------------------------------------------//
	bool                       GetChanged();
	LPCTSTR                    GetParamText();
	//---------------------------------------------------------------------------------//
	bool                       SetNewValue(LPCTSTR Value);//設定新的參數值
	//---------------------------------------------------------------------------------//	
	bool                       GetReStart() const;
	void                       SetReStart(bool Value);
	//---------------------------------------------------------------------------------//	
	LPCTSTR                    GetValue_STR() const;
	void                       SetValue_STR(LPCSTR Value);
	void                       SetValue_STR(LPCWSTR Value);
	void                       SetNewValue_STR(LPCTSTR Value);	
	//---------------------------------------------------------------------------------//
	bool                       SetNewValue_INT(int Value);
	int                        GetValue_INT() const;
	void                       SetValue_INT(int Value, int Min=INT_MIN, int Max=INT_MAX);
	//---------------------------------------------------------------------------------//
	bool                       SetNewValue_DBL(double Value);
	double                     GetValue_DBL() const;
	void                       SetValue_DBL(double Value, int Precision=2, double Min=-DBL_MAX, double Max=DBL_MAX);
	//---------------------------------------------------------------------------------//	
	COLORREF                   GetValue_CLR() const;
	void                       SetValue_CLR(COLORREF Value);
	bool                       SetNewValue_CLR(COLORREF Value);
	//---------------------------------------------------------------------------------//
	void                       ClearSelList();	
	void                       AddSelItem(DWORD Param);
	void                       AddSelItem(DWORD Param, LPCTSTR Text);
	size_t                     GetSelItemCount();
	bool                       GetSelItem(size_t idx, bool check, int &Param, CString &Text);
	//---------------------------------------------------------------------------------//
	bool                       SetValue_SEL(DWORD Param);
	bool                       SetValue_SEL(LPCTSTR Text);
	bool                       SetNewValue_SEL(DWORD Param);
	bool                       SetNewValue_SEL(LPCTSTR Text);
	DWORD                      GetSelParam();
	//---------------------------------------------------------------------------------//

};
//-------------------------------------------------------------------------------------//
typedef std::vector<CParamUni>   CParamList;
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_PARAMUNI_H__9069A156_A56C_457C_8612_349F8744D441__INCLUDED_)
