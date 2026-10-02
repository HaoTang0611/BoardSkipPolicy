// ColorRGBV.h: interface for the CColorRGBV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORRGBV_H__E0FEF2E9_6450_4E32_9350_5DDAA729B6E1__INCLUDED_)
#define AFX_COLORRGBV_H__E0FEF2E9_6450_4E32_9350_5DDAA729B6E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
const int COLOR_RGBV_MAX = 255;
const int COLOR_RGBV_MIN =   0;
//-------------------------------------------------------------------------------------//
enum COLOR_RGBV_MODE
{
	COLOR_RGBV_NONE            = 0,
	COLOR_RGBV_RED             = 1,
	COLOR_RGBV_GREEN           = 2,
	COLOR_RGBV_BLUE            = 3,
	COLOR_RGBV_VALUE           = 4,
	COLOR_RGBV_RETURN
};
//-------------------------------------------------------------------------------------//
enum COLOR_LOGIC_MODE
{
	COLOR_LOGIC_INCLUDE        = 1,
	COLOR_LOGIC_EXCLUDE        = 2 
};
//-------------------------------------------------------------------------------------//
class CAOIFileIO;
//-------------------------------------------------------------------------------------//
class CColorRGBV  
{
private:
	//---------------------------------------------------------------------------------//
	COLOR_RGBV_MODE            m_RGBVColorMode;              //色彩模式
	COLOR_LOGIC_MODE           m_RGBVLogicMode;              //邏輯模式	
	//---------------------------------------------------------------------------------//
	int                        m_RGBVRedMax;                 //紅色上限
	int                        m_RGBVRedMin;                 //紅色下限 
	bool                       m_RGBVRedEnabled;             //紅色啟用

	int                        m_RGBVGreenMax;               //綠色上限
	int                        m_RGBVGreenMin;               //綠色下限 
	bool                       m_RGBVGreenEnabled;           //綠色啟用

	int                        m_RGBVBlueMax;                //藍色上限
	int                        m_RGBVBlueMin;                //藍色下限 
	bool                       m_RGBVBlueEnabled;            //藍色啟用

	int                        m_RGBVValueMax;                //灰色上限
	int                        m_RGBVValueMin;                //灰色下限 
	bool                       m_RGBVValueEnabled;            //灰色啟用
	//---------------------------------------------------------------------------------//
	bool                       m_RGBVUsed;                    //是否使用
	bool                       m_RGBVActived;                 //是否主要操作
	COLORREF                   m_RGBVShowColor;               //顯示的顏色
	//---------------------------------------------------------------------------------//
protected:
	//---------------------------------------------------------------------------------//
	void                       PreInitColorRGBV();
	void                       InitialColorRGBV();
	void                       CloneColorRGBV(const CColorRGBV &rgbv);
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CColorRGBV();
	CColorRGBV(const CColorRGBV &rgbv);
	virtual ~CColorRGBV();
	CColorRGBV&                operator=(const CColorRGBV &rgbv);
	//---------------------------------------------------------------------------------//
	bool                       WriteColorRGBVFile(CAOIFileIO &FileIO);
	bool                       ReadColorRGBVFile(CAOIFileIO &FileIO);
	//---------------------------------------------------------------------------------//
	void                       SetColorMode(COLOR_RGBV_MODE value) { m_RGBVColorMode = value; }
	COLOR_RGBV_MODE            GetColorMode() const { return m_RGBVColorMode; }
	//---------------------------------------------------------------------------------//
	void                       SetLogicMode(COLOR_LOGIC_MODE value) { m_RGBVLogicMode = value; }
	COLOR_LOGIC_MODE           GetLogicMode() const { return m_RGBVLogicMode; }
	//---------------------------------------------------------------------------------//	
	void                       SetRedMax(int value) { m_RGBVRedMax = value; }
	int                        GetRedMax() const { return m_RGBVRedMax; }
	//---------------------------------------------------------------------------------//
	void                       SetRedMin(int value) { m_RGBVRedMin = value; }
	int                        GetRedMin() const { return m_RGBVRedMin; }
	//---------------------------------------------------------------------------------//
	void                       SetRedEnabled(bool value) { m_RGBVRedEnabled = value; }
	bool                       GetRedEnabled() const { return m_RGBVRedEnabled; }
	//---------------------------------------------------------------------------------//
	void                       SetGreenMax(int value) { m_RGBVGreenMax = value; }
	int                        GetGreenMax() const { return m_RGBVGreenMax; }
	//---------------------------------------------------------------------------------//
	void                       SetGreenMin(int value) { m_RGBVGreenMin = value; }
	int                        GetGreenMin() const { return m_RGBVGreenMin; }
	//---------------------------------------------------------------------------------//
	void                       SetGreenEnabled(bool value) { m_RGBVGreenEnabled = value; }
	bool                       GetGreenEnabled() const { return m_RGBVGreenEnabled; }
	//---------------------------------------------------------------------------------//	
	void                       SetBlueMax(int value) { m_RGBVBlueMax = value; }
	int                        GetBlueMax() const { return m_RGBVBlueMax; }
	//---------------------------------------------------------------------------------//
	void                       SetBlueMin(int value) { m_RGBVBlueMin = value; }
	int                        GetBlueMin() const { return m_RGBVBlueMin; }
	//---------------------------------------------------------------------------------//
	void                       SetBlueEnabled(bool value) { m_RGBVBlueEnabled = value; }
	bool                       GetBlueEnabled() const { return m_RGBVBlueEnabled; }
	//---------------------------------------------------------------------------------//
	void                       SetValueMax(int value) { m_RGBVValueMax = value; }
	int                        GetValueMax() const { return m_RGBVValueMax; }
	//---------------------------------------------------------------------------------//
	void                       SetValueMin(int value) { m_RGBVValueMin = value; }
	int                        GetValueMin() const { return m_RGBVValueMin; }
	//---------------------------------------------------------------------------------//
	void                       SetValueEnabled(bool value) { m_RGBVValueEnabled = value; }
	bool                       GetValueEnabled() const { return m_RGBVValueEnabled; }
	//---------------------------------------------------------------------------------//	
	void                       SetUsed(bool value) { m_RGBVUsed = value; }
	bool                       GetUsed() const { return m_RGBVUsed; }
	//---------------------------------------------------------------------------------//
	void                       SetActived(bool value) { m_RGBVActived = value; }
	bool                       GetActived() const { return m_RGBVActived; }
	//---------------------------------------------------------------------------------//	
	void                       SetColorMax(COLOR_RGBV_MODE Mode, int value);
	int                        GetColorMax(COLOR_RGBV_MODE Mode) const;
	//---------------------------------------------------------------------------------//	
	void                       SetColorMin(COLOR_RGBV_MODE Mode, int value);
	int                        GetColorMin(COLOR_RGBV_MODE Mode) const;
	//---------------------------------------------------------------------------------//	
	void                       SetColorEnabled(COLOR_RGBV_MODE Mode, bool value);
	bool                       GetColorEnabled(COLOR_RGBV_MODE Mode) const;
	//---------------------------------------------------------------------------------//	
	void                       CopyColorRGBV(const CColorRGBV &rgbv);
	void                       MoveColorBrightness(int value);
	void                       MoveColorRGBV(int value, bool OnlyColor);	
	void                       ExpandColorBrightness(int value);
	void                       ExpandColorRGBV(int value, bool OnlyColor);	
	//---------------------------------------------------------------------------------//	
	bool                       GetUsed();
	bool                       CheckUsed();
	//---------------------------------------------------------------------------------//	
	void                       ResetColor();//復歸顏色
	//---------------------------------------------------------------------------------//	
	bool                       CheckColorInside(int IR, int IG, int IB, int IV) const;//確認顏色在範圍內
	//---------------------------------------------------------------------------------//	
	bool                       GrayColor();//灰階顏色
	bool                       CheckMerge();//確認是否可以合併	
	bool                       MergeColor(bool chkMerge, const CColorRGBV *rgbvPtr);//合併顏色		
	bool                       CheckMergeColor(const CColorRGBV *rgbvPtr);//確認是否為可以合併顏色	
	//---------------------------------------------------------------------------------//
	bool                       CalcShowColor();//計算出顯示的顏色 
	COLORREF                   GetShowColor() const;//取得顯示的顏色
	void                       SetShowColor(COLORREF clr);//設定顯示的顏色	
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_COLORRGBV_H__E0FEF2E9_6450_4E32_9350_5DDAA729B6E1__INCLUDED_)
