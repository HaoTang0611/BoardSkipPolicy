// FilenameSyntax.h: interface for the CFilenameSyntax class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILENAMESYNTAX_H__38CB5B93_72E6_4D0E_AD47_F1C33BFEF578__INCLUDED_)
#define AFX_FILENAMESYNTAX_H__38CB5B93_72E6_4D0E_AD47_F1C33BFEF578__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include <vector>
//-------------------------------------------------------------------------------------//
class CAOIBoard;
class CAOIPanel;
class CAOIProject;
//-------------------------------------------------------------------------------------//
enum FILE_NAME_SYNTAX_MODE//檔案名稱語句模式
{	
	FILE_NAME_SYNTAX_NULL                      =  0,//無字串	
	FILE_NAME_SYNTAX_MACHINE_NAME              =  1,//機台別名Alias Name
	FILE_NAME_SYNTAX_MACHINE_LINE              =  2,//線名
	FILE_NAME_SYNTAX_MACHINE_STATION           =  3,//站別	
	//FILE_NAME_SYNTAX_MACHINE_HOST_IP           =  4,//電腦網址

	FILE_NAME_SYNTAX_PROJECT_NAME              = 11,//專案名
	FILE_NAME_SYNTAX_PROJECT_MODULE            = 12,//機種名
	FILE_NAME_SYNTAX_PROJECT_LOT               = 13,//工單名
	FILE_NAME_SYNTAX_PROJECT_LANE              = 14,//軌道名	
	
	FILE_NAME_SYNTAX_TIME_TEST_YYYYMMDD        = 21,//檢測時間-年年年年月月日日
	FILE_NAME_SYNTAX_TIME_TEST_YYYYMMDDHHMMSS  = 22,//檢測時間-年年年年月月日日時時分分秒秒	
	
	//FILE_NAME_SYNTAX_BARCODE_SCOPE             = 31,//範圍條碼-自動切換專案, 整板, 單板
	FILE_NAME_SYNTAX_BARCODE_PROJECT           = 32,//專案條碼
	FILE_NAME_SYNTAX_BARCODE_PANEL             = 33,//整板條碼
	FILE_NAME_SYNTAX_BARCODE_BOARD             = 34,//單板條碼		

	FILE_NAME_SYNTAX_INDEX_OBJECT              = 41,//物件序號
	FILE_NAME_SYNTAX_INDEX_PANEL               = 42,//整板序號
	FILE_NAME_SYNTAX_INDEX_BOARD               = 43,//單板序號

	//標準符號-Punctuation
	FILE_NAME_SYNTAX_PUNC_HYPHEN               = 51,//標準符號-連字號-
	FILE_NAME_SYNTAX_PUNC_UNDER_LINE           = 52,//標準符號-下底線_
	FILE_NAME_SYNTAX_PUNC_AT_SIGN              = 53,//標準符號-@
	FILE_NAME_SYNTAX_PUNC_NUMBER_SIGN          = 54,//標準符號-#

	FILE_NAME_SYNTAX_USER_DEFINE_1             = 71,//自定義
	FILE_NAME_SYNTAX_USER_DEFINE_2             = 72,//自定義
	FILE_NAME_SYNTAX_USER_DEFINE_3             = 73,//自定義

	FILE_NAME_SYNTAX_RETURN
};
//-------------------------------------------------------------------------------------//
class CFilenameSyntaxNode
{	
private:	
	std::string           m_TextA;
	std::wstring          m_TextW;
	FILE_NAME_SYNTAX_MODE m_SyntaxMode;
	bool                  m_FolderMode;

public:
	CFilenameSyntaxNode()
	{	
		m_FolderMode = false;
		m_SyntaxMode = FILE_NAME_SYNTAX_NULL;
	}

	void Clear()
	{
		m_TextA.clear();
		m_TextW.clear();
		m_FolderMode = false;
		m_SyntaxMode = FILE_NAME_SYNTAX_NULL;
	}

	bool GetFolderMode() { return m_FolderMode; }
	void SetFolderMode(bool Mode) { m_FolderMode=Mode; }

	FILE_NAME_SYNTAX_MODE GetSyntaxMode() { return m_SyntaxMode; }
	void SetSyntaxMode(FILE_NAME_SYNTAX_MODE Mode) { m_SyntaxMode=Mode; }

	void SetText(const TCHAR* str)
	{
	#ifdef _UNICODE
		SetTextW(str);
	#else
		SetTextA(str);
	#endif//_UNICODE
	}
	const TCHAR* GetText()
	{
	#ifdef _UNICODE
		return GetTextW();
	#else
		return GetTextA();
	#endif//_UNICODE
		return NULL;
	}

	void SetTextA(const char* str)
	{	m_TextA = str;	}
	const char* GetTextA()
	{	return m_TextA.c_str(); }

	void SetTextW(const wchar_t* str)
	{	m_TextW = str;	}
	const wchar_t* GetTextW()
	{	return m_TextW.c_str(); }
};
//-------------------------------------------------------------------------------------//
class CFilenameSyntax  //檔名句法
{
private:		
	CAOIBoard                       *m_BoardPtr;
	CAOIPanel                       *m_PanelPtr;
	CAOIProject                     *m_ProjectPtr;
	
	CString                          m_Tag;
	CString                          m_IniSection;
	CString                          m_IniFilename;

	bool                             m_Enabled;//是否啟用句法	
	size_t                           m_ObjectIndex;//物件引數	
	int                              m_PanelIndexWidth;//整板引數寬度
	int                              m_BoardIndexWidth;//單板引數寬度
	int                              m_ObjectIndexWidth;//物件引數寬度
	CString                          m_NodeText;//內部使用
	CString                          m_ExtName;//副檔名
	CString                          m_ErrorString;//錯誤訊息
	CString                          m_UserDefineText1;//自訂義-1
	CString                          m_UserDefineText2;//自訂義-2
	CString                          m_UserDefineText3;//自訂義-3
	std::vector<CFilenameSyntaxNode> m_SyntaxNodeList;

protected:	
	CAOIBoard*                 GetBoardPtr();	
	CAOIPanel*                 GetPanelPtr();
	CAOIProject*               GetProjectPtr();

	bool                       CheckBoardPtr(CAOIBoard* Ptr);
	bool                       CheckPanelPtr(CAOIPanel* Ptr);
	bool                       CheckProjectPtr(CAOIProject* Ptr);

	CString                    GetIndexText(size_t idx, int size);
	//---------------------------------------------------------------------------------//
	bool                       SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error);
	bool                       LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error);	
	//---------------------------------------------------------------------------------//
public:
	CFilenameSyntax();
	virtual ~CFilenameSyntax();	
	
	bool                       SetBoardPtr(CAOIBoard *Ptr);
	bool                       SetPanelPtr(CAOIPanel *Ptr);
	bool                       SetProjectPtr(CAOIProject *Ptr);

	LPCTSTR                    GetErrorString() const;
	bool                       SaveFilenameSyntaxIni();
	bool                       LoadFilenameSyntaxIni();
	bool                       SetFilenameSyntaxIni(LPCTSTR Filename, LPCTSTR Section);
	bool                       SaveFilenameSyntaxIni(LPCTSTR Filename, LPCTSTR Section);
	bool                       LoadFilenameSyntaxIni(LPCTSTR Filename, LPCTSTR Section);
	bool                       GetSyntaxEnumList(std::vector<FILE_NAME_SYNTAX_MODE> &List);
	
	void                       ClearSyntaxList();	
	void                       ClearSyntaxParam();		
	size_t                     GetSyntaxNodeCount() const;
	bool                       DelSyntaxNodeUnused();
	bool                       DelSyntaxNode(size_t idx);
	bool                       ResizeSyntaxList(size_t Cnt);		
	CFilenameSyntaxNode*       GetSyntaxNodePtr(size_t idx, bool bCheck);	
	bool                       AddSyntaxNode(const CFilenameSyntaxNode &Node);			
	bool                       InsertSyntaxNode(size_t idx, const CFilenameSyntaxNode &Node);

	bool                       BuildFilename(LPCTSTR Folder, bool bCreateFile, CString &Filename);//建立檔案名稱		

	bool                       GetNodeText(CFilenameSyntaxNode &Node);		
	bool                       GetSyntaxContent(FILE_NAME_SYNTAX_MODE Mode, CString &Text);	

	LPCTSTR                    GetFilenameSyntaxTag() const;
	void                       SetFilenameSyntaxTag(LPCTSTR Tag);

	bool                       GetFilenameSyntaxEnabled() const;
	void                       SetFilenameSyntaxEnabled(bool bEnable);

	size_t                     GetObjectIndex() const;
	void                       SetObjectIndex(size_t idx);

	int                        GetObjectIndexWidth() const;
	void                       SetObjectIndexWidth(int idx);

	int                        GetPanelIndexWidth() const;
	void                       SetPanelIndexWidth(int idx);

	int                        GetBoardIndexWidth() const;
	void                       SetBoardIndexWidth(int idx);
	
	LPCTSTR                    GetMachineName();
	LPCTSTR                    GetMachineLine();
	LPCTSTR                    GetMachineStation();
	
	LPCTSTR                    GetProjectFilename();
	LPCTSTR                    GetProjectModuleName();
	LPCTSTR                    GetProjectLotName();
	LPCTSTR                    GetProjectLaneName();

	LPCTSTR                    GetProjectTimeTest_YYYYMMDD();
	LPCTSTR                    GetProjectTimeTest_YYYYMMDDhhmmss();

	LPCTSTR                    GetBarcodeScope();
	LPCTSTR                    GetBarcodeProject();	
	LPCTSTR                    GetBarcodePanel();
	LPCTSTR                    GetBarcodeBoard();

	LPCTSTR                    GetIndexObject();
	LPCTSTR                    GetIndexPanel();
	LPCTSTR                    GetIndexeBoard();

	LPCTSTR                    GetPuncHyphen();//連字號-
	LPCTSTR                    GetPuncUnderLine();//下底線_
	LPCTSTR                    GetPuncAtSign();//@
	LPCTSTR                    GetPuncNumberSign();//#

	LPCTSTR                    GetUserdefineText1();
	void                       SetUserdefineText1(LPCTSTR Name);

	LPCTSTR                    GetUserdefineText2();
	void                       SetUserdefineText2(LPCTSTR Name);

	LPCTSTR                    GetUserdefineText3();
	void                       SetUserdefineText3(LPCTSTR Name);
	
	LPCTSTR                    GetExtensionName();
	void                       SetExtensionName(LPCTSTR Name);
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_FILENAMESYNTAX_H__38CB5B93_72E6_4D0E_AD47_F1C33BFEF578__INCLUDED_)
