// FilenameSyntax.cpp: implementation of the CFilenameSyntax class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "FilenameSyntax.h"
//-------------------------------------------------------------------------------------//
#include "AOIProject.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CFilenameSyntax::CFilenameSyntax()
{	
	m_BoardPtr = NULL;
	m_PanelPtr = NULL;
	m_ProjectPtr = NULL;
	ClearSyntaxParam();
}
//-------------------------------------------------------------------------------------//
CFilenameSyntax::~CFilenameSyntax()
{

}
//-------------------------------------------------------------------------------------//
CAOIBoard* CFilenameSyntax::GetBoardPtr()
{
	return m_BoardPtr;
}
//-------------------------------------------------------------------------------------//
CAOIPanel* CFilenameSyntax::GetPanelPtr()
{
	return m_PanelPtr;
}
//-------------------------------------------------------------------------------------//
CAOIProject* CFilenameSyntax::GetProjectPtr()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::CheckBoardPtr(CAOIBoard* Ptr)
{
	if ( NULL == Ptr ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::CheckPanelPtr(CAOIPanel* Ptr)
{
	if ( NULL == Ptr ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::CheckProjectPtr(CAOIProject* Ptr)
{
	if ( NULL == Ptr ) { return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
CString CFilenameSyntax::GetIndexText(size_t idx, int width)
{
	CString str;	
	switch ( width )
	{
	case 0: str.Format(_T("%d"), idx); break;
	case 1: str.Format(_T("%01d"), idx); break;
	case 2: str.Format(_T("%02d"), idx); break;
	case 3: str.Format(_T("%03d"), idx); break;
	case 4: str.Format(_T("%04d"), idx); break;
	case 5: str.Format(_T("%05d"), idx); break;
	case 6: str.Format(_T("%06d"), idx); break;
	case 7: str.Format(_T("%07d"), idx); break;
	case 8: str.Format(_T("%08d"), idx); break;
	case 9: str.Format(_T("%09d"), idx); break;
	case 10: str.Format(_T("%010d"), idx); break;
	default:
		str.Format(_T("%016d"), idx);
		break;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::SaveINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pString, LPCTSTR pfilename, CString &Error)
{
	if ( JetAPI::SaveINIData(pSection, pKeyName, pString, pfilename, Error) == false ) 
	{	return false;	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::LoadINIData(LPCTSTR pSection, LPCTSTR pKeyName, LPCTSTR pDefault, TCHAR *pString, int StringSize, LPCTSTR pfilename, bool IsCheckLens, CString &Error)
{
	if ( JetAPI::LoadINIData(pSection, pKeyName, pDefault, pString, StringSize, pfilename, IsCheckLens, Error) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::SetBoardPtr(CAOIBoard *Ptr)
{
	m_BoardPtr = Ptr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::SetPanelPtr(CAOIPanel *Ptr)
{
	m_PanelPtr = Ptr;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::SetProjectPtr(CAOIProject *Ptr)
{
	m_ProjectPtr = Ptr;
	return true;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetErrorString() const
{
	return m_ErrorString;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::SaveFilenameSyntaxIni()
{
	return SaveFilenameSyntaxIni(m_IniFilename, m_IniSection);
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::LoadFilenameSyntaxIni()
{
	return LoadFilenameSyntaxIni(m_IniFilename, m_IniSection);
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::SetFilenameSyntaxIni(LPCTSTR Filename, LPCTSTR Section)
{
	m_IniSection = Section;
	m_IniFilename = Filename;;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::SaveFilenameSyntaxIni(LPCTSTR Filename, LPCTSTR Section)
{	
	CString str;
	CString NodeName;
	CString KeyName = _T("");
	CString String = _T("");

	SetFilenameSyntaxIni(Filename, Section);	

	KeyName = _T("Enable");	
	String.Format(_T("%d"), GetFilenameSyntaxEnabled());
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false;	}

	KeyName = _T("Extension Name");	
	String = m_ExtName;
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false;	}

	KeyName = _T("Panel Index Width");	
	String.Format(_T("%d"), GetPanelIndexWidth());
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false;	}

	KeyName = _T("Board Index Width");	
	String.Format(_T("%d"), GetBoardIndexWidth());
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false;	}

	KeyName = _T("Object Index Width");	
	String.Format(_T("%d"), GetObjectIndexWidth());
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false;	}

	KeyName = _T("User Define Text 01");
	String = m_UserDefineText1;
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false;	}

	KeyName = _T("User Define Text 02");
	String = m_UserDefineText2;
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false;	}

	KeyName = _T("User Define Text 03");
	String = m_UserDefineText3;
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false;	}

	size_t  i=0;
	CFilenameSyntaxNode *NodePtr=NULL;
	const size_t NodeCount=GetSyntaxNodeCount();

	KeyName = _T("Syntax Count");
	String.Format(_T("%d"), NodeCount);
	if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
	{	return false;	}
	for ( i=0; i<NodeCount; i++ )
	{
		NodePtr = GetSyntaxNodePtr(i, false);
		if ( NULL == NodePtr ) { continue; }
		NodeName.Format(_T("%s_%03d"), _T("Syntax"), i+1);
		KeyName.Format(_T("%s %s"), NodeName, _T("Mode"));
		String.Format(_T("%d"), NodePtr->GetSyntaxMode());
		if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
		{	return false; }

		KeyName.Format(_T("%s %s"), NodeName, _T("Folder Mode"));
		String.Format(_T("%d"), NodePtr->GetFolderMode());
		if ( SaveINIData(Section, KeyName, String, Filename, m_ErrorString) == false )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::LoadFilenameSyntaxIni(LPCTSTR Filename, LPCTSTR Section)
{
	ClearSyntaxParam();	

	int     TempI=0;
	const size_t textlen = 128;
	CString str;
	CString NodeName;
	CString KeyName = _T("");
	CString Default = _T("");	
	TCHAR   String[textlen]=_T("");		
	const bool bCheckLen = false;

	SetFilenameSyntaxIni(Filename, Section);

	KeyName = _T("Enable");	
	Default.Format(_T("%d"), GetFilenameSyntaxEnabled());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, bCheckLen, m_ErrorString) == true )
	{	
		TempI = ::_ttoi(String);
		if ( FN_DISABLE == TempI ) { SetFilenameSyntaxEnabled(false); }
		else { SetFilenameSyntaxEnabled(true); }
	}

	KeyName = _T("Extension Name");
	Default = m_ExtName;
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, bCheckLen, m_ErrorString) == true )
	{	m_ExtName = String;	}

	KeyName = _T("Panel Index Width");	
	Default.Format(_T("%d"), GetPanelIndexWidth());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, bCheckLen, m_ErrorString) == true )
	{	SetPanelIndexWidth(::_ttoi(String));	}

	KeyName = _T("Board Index Width");	
	Default.Format(_T("%d"), GetBoardIndexWidth());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, bCheckLen, m_ErrorString) == true )
	{	SetBoardIndexWidth(::_ttoi(String));	}

	KeyName = _T("Object Index Width");	
	Default.Format(_T("%d"), GetObjectIndexWidth());
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, bCheckLen, m_ErrorString) == true )
	{	SetObjectIndexWidth(::_ttoi(String));	}

	KeyName = _T("User Define Text 01");
	Default = m_UserDefineText1;
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, bCheckLen, m_ErrorString) == true )
	{	m_UserDefineText1 = String;	}

	KeyName = _T("User Define Text 02");
	Default = m_UserDefineText2;
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, bCheckLen, m_ErrorString) == true )
	{	m_UserDefineText2 = String;	}

	KeyName = _T("User Define Text 03");
	Default = m_UserDefineText3;
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, bCheckLen, m_ErrorString) == true )
	{	m_UserDefineText3 = String;	}

	size_t  i=0;
	size_t  NodeCount=0;
	CFilenameSyntaxNode SyntaxNode;

	KeyName = _T("Syntax Count");
	Default.Format(_T("%d"), NodeCount);
	if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, bCheckLen, m_ErrorString) == true )
	{	NodeCount = ::_ttoi(String);	}
	for ( i=0; i<NodeCount; i++ )
	{	
		SyntaxNode.Clear();
		NodeName.Format(_T("%s_%03d"), _T("Syntax"), i+1);
		KeyName.Format(_T("%s %s"), NodeName, _T("Mode"));
		Default.Format(_T("%d"), FILE_NAME_SYNTAX_NULL);
		if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, bCheckLen, m_ErrorString) == false )
		{	continue; }
		SyntaxNode.SetSyntaxMode((FILE_NAME_SYNTAX_MODE)(::_ttoi(String)));

		KeyName.Format(_T("%s %s"), NodeName, _T("Folder Mode"));
		Default.Format(_T("%d"), FN_DISABLE);
		if ( LoadINIData(Section, KeyName, Default, String, textlen, Filename, bCheckLen, m_ErrorString) == false )
		{	continue; }
		TempI = ::_ttoi(String);
		if ( FN_DISABLE == TempI ) { SyntaxNode.SetFolderMode(false); }
		else { SyntaxNode.SetFolderMode(true); }

		AddSyntaxNode(SyntaxNode);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::GetSyntaxEnumList(std::vector<FILE_NAME_SYNTAX_MODE> &List)
{
	List.clear();

	List.push_back(FILE_NAME_SYNTAX_NULL);

	List.push_back(FILE_NAME_SYNTAX_MACHINE_NAME);
	List.push_back(FILE_NAME_SYNTAX_MACHINE_LINE);
	List.push_back(FILE_NAME_SYNTAX_MACHINE_STATION);
	//List.push_back(FILE_NAME_SYNTAX_MACHINE_HOST_IP);

	List.push_back(FILE_NAME_SYNTAX_PROJECT_NAME);
	List.push_back(FILE_NAME_SYNTAX_PROJECT_MODULE);	
	List.push_back(FILE_NAME_SYNTAX_PROJECT_LOT);
	List.push_back(FILE_NAME_SYNTAX_PROJECT_LANE);

	List.push_back(FILE_NAME_SYNTAX_TIME_TEST_YYYYMMDD);	
	List.push_back(FILE_NAME_SYNTAX_TIME_TEST_YYYYMMDDHHMMSS);
	
	//List.push_back(FILE_NAME_SYNTAX_BARCODE_SCOPE);
	List.push_back(FILE_NAME_SYNTAX_BARCODE_PROJECT);
	List.push_back(FILE_NAME_SYNTAX_BARCODE_PANEL);
	List.push_back(FILE_NAME_SYNTAX_BARCODE_BOARD);

	List.push_back(FILE_NAME_SYNTAX_INDEX_OBJECT);
	List.push_back(FILE_NAME_SYNTAX_INDEX_PANEL);
	List.push_back(FILE_NAME_SYNTAX_INDEX_BOARD);

	//標準符號-Punctuation	        
	List.push_back(FILE_NAME_SYNTAX_PUNC_HYPHEN);
	List.push_back(FILE_NAME_SYNTAX_PUNC_UNDER_LINE);
	List.push_back(FILE_NAME_SYNTAX_PUNC_AT_SIGN);
	List.push_back(FILE_NAME_SYNTAX_PUNC_NUMBER_SIGN);	

	List.push_back(FILE_NAME_SYNTAX_USER_DEFINE_1);
	List.push_back(FILE_NAME_SYNTAX_USER_DEFINE_2);
	List.push_back(FILE_NAME_SYNTAX_USER_DEFINE_3);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntax::ClearSyntaxList()
{
	m_SyntaxNodeList.clear();
	return;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntax::ClearSyntaxParam()
{
	m_Enabled = false;	
	m_PanelIndexWidth = 0;
	m_BoardIndexWidth = 0;
	m_ObjectIndexWidth = 0;
	m_ExtName=_T("TXT");
	m_NodeText=_T("");
	m_UserDefineText1=_T("");
	m_UserDefineText2=_T("");
	m_UserDefineText3=_T("");
	ClearSyntaxList();
	return;
}
//-------------------------------------------------------------------------------------//
size_t CFilenameSyntax::GetSyntaxNodeCount() const
{
	return m_SyntaxNodeList.size();
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::DelSyntaxNodeUnused()
{
	size_t i=0;
	std::vector<CFilenameSyntaxNode> TmpSyntaxNodeList=m_SyntaxNodeList;
	m_SyntaxNodeList.clear();
	const size_t Cnt = TmpSyntaxNodeList.size();
	for ( i=0; i<Cnt; i++ )
	{
		CFilenameSyntaxNode &NodeRef=TmpSyntaxNodeList[i];
		if ( FILE_NAME_SYNTAX_NULL == NodeRef.GetSyntaxMode() ) { continue; }
		AddSyntaxNode(NodeRef);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::DelSyntaxNode(size_t idx)
{	
	size_t Cnt=GetSyntaxNodeCount();
	if ( idx >= Cnt ) { return false; }

	size_t i=0;
	std::vector<CFilenameSyntaxNode> TmpSyntaxNodeList=m_SyntaxNodeList;
	m_SyntaxNodeList.clear();
	Cnt = TmpSyntaxNodeList.size();
	for ( i=0; i<Cnt; i++ )
	{
		if ( i == idx ) { continue; }
		AddSyntaxNode(TmpSyntaxNodeList[i]);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::ResizeSyntaxList(size_t Cnt)
{
	const size_t Count=GetSyntaxNodeCount();
	if ( Cnt == Count ) { return true; }
	m_SyntaxNodeList.resize(Cnt);
	return true;
}
//-------------------------------------------------------------------------------------//
CFilenameSyntaxNode* CFilenameSyntax::GetSyntaxNodePtr(size_t idx, bool bCheck)
{
	if ( true == bCheck )
	{
		const size_t Cnt=m_SyntaxNodeList.size();
		if ( idx >= Cnt )
		{	return NULL; }
	}
	return &(m_SyntaxNodeList[idx]);
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::AddSyntaxNode(const CFilenameSyntaxNode &Node)
{
	m_SyntaxNodeList.push_back(Node);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::InsertSyntaxNode(size_t idx, const CFilenameSyntaxNode &Node)
{
	size_t i=0;
	size_t Cnt=GetSyntaxNodeCount();
	if ( idx >= Cnt ) 
	{ 
		AddSyntaxNode(Node);
		return true; 
	}

	std::vector<CFilenameSyntaxNode> TmpSyntaxNodeList=m_SyntaxNodeList;
	m_SyntaxNodeList.clear();
	Cnt = TmpSyntaxNodeList.size();
	for ( i=0; i<Cnt; i++ )
	{
		if ( i == idx )
		{	AddSyntaxNode(Node);	}
		AddSyntaxNode(TmpSyntaxNodeList[i]);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::BuildFilename(LPCTSTR Folder, bool bCreateFile, CString &Filename)//建立檔案名稱	
{
	size_t  i=0;	
	CString strTmp;
	CString strText;	
	CString FolderName;	
	bool    bFolder=false;	
	CFilenameSyntaxNode*  NodePtr=NULL;	
	const size_t NodeCount=GetSyntaxNodeCount();

	Filename=_T("");
	for ( i=0; i<NodeCount; i++ )
	{
		NodePtr = GetSyntaxNodePtr(i, false);
		if ( NULL == NodePtr ) { continue; }
		if ( GetNodeText(*NodePtr) == false ) { continue; }		
		strText = NodePtr->GetText();
		bFolder = NodePtr->GetFolderMode();				
		if ( strText.GetLength() == 0 ) { continue; }
		if ( Filename.GetLength() == 0 )
		{	Filename = strText; }
		else
		{
			strTmp = Filename;
			Filename=strTmp+strText;
		}
		if ( true == bFolder )
		{
			if ( true == bCreateFile )
			{
				FolderName.Format(_T("%s\\%s"), Folder, Filename);
				if ( JetAPI::CreateFolder(FolderName) == false )
				{
					m_ErrorString.Format(_T("Error, Create Folder Fault\n%s"), FolderName);
					return false;
				}
			}			
			Filename += CString("\\");
		}
	}

	if ( Filename.GetLength() > 0 )
	{
		if ( true == bCreateFile )
		{
			FolderName.Format(_T("%s\\%s.%s"), Folder, Filename, _T("@#&"));
			FILE *pfile=::_tfopen(FolderName, _T("w+"));
			if ( NULL == pfile )
			{	
				FolderName.Format(_T("%s\\%s.%s"), Folder, Filename, m_ExtName);
				m_ErrorString.Format(_T("Error, Create File Fault\n%s"), FolderName);
				return false;
			}
			::fclose(pfile); pfile=NULL;
			::DeleteFile(FolderName);
		}	
		Filename += CString(_T("."))+m_ExtName;
		if ( NULL!=Folder && ::_tcslen(Folder)>0 )
		{	
			FolderName = Filename;
			Filename.Format(_T("%s\\%s"), Folder, FolderName);
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::GetNodeText(CFilenameSyntaxNode &Node)
{
	Node.SetText(_T(""));
	CString str;
	bool bValid=true;
	FILE_NAME_SYNTAX_MODE Mode=Node.GetSyntaxMode();	
	bValid = GetSyntaxContent(Mode, str);	
	Node.SetText(str);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::GetSyntaxContent(FILE_NAME_SYNTAX_MODE Mode, CString &Text)
{
	bool bValid=true;
	Text = _T("");
	switch ( Mode )
	{
	case FILE_NAME_SYNTAX_NULL:
		Text = _T("");
		bValid = false;
		break;

	case FILE_NAME_SYNTAX_MACHINE_NAME: Text=GetMachineName(); break;
	case FILE_NAME_SYNTAX_MACHINE_LINE: Text=GetMachineLine(); break;		
	case FILE_NAME_SYNTAX_MACHINE_STATION: Text=GetMachineStation(); break;				
	//case FILE_NAME_SYNTAX_MACHINE_HOST_IP: Text=_T("HostIP"); break;				
		
	case FILE_NAME_SYNTAX_PROJECT_NAME: Text=GetProjectFilename(); break;
	case FILE_NAME_SYNTAX_PROJECT_MODULE: Text=GetProjectModuleName(); break;
	case FILE_NAME_SYNTAX_PROJECT_LOT: Text=GetProjectLotName(); break;
	case FILE_NAME_SYNTAX_PROJECT_LANE: Text=GetProjectLaneName(); break;		

	case FILE_NAME_SYNTAX_TIME_TEST_YYYYMMDD: Text=GetProjectTimeTest_YYYYMMDD(); break;
	case FILE_NAME_SYNTAX_TIME_TEST_YYYYMMDDHHMMSS: Text=GetProjectTimeTest_YYYYMMDDhhmmss(); break;

	//case FILE_NAME_SYNTAX_BARCODE_SCOPE: Text=GetBarcodeScope(); break;		
	case FILE_NAME_SYNTAX_BARCODE_PROJECT: Text=GetBarcodeProject(); break;		
	case FILE_NAME_SYNTAX_BARCODE_PANEL: Text=GetBarcodePanel(); break;
	case FILE_NAME_SYNTAX_BARCODE_BOARD: Text=GetBarcodeBoard(); break;

	case FILE_NAME_SYNTAX_INDEX_OBJECT: Text=GetIndexObject(); break;
	case FILE_NAME_SYNTAX_INDEX_PANEL: Text=GetIndexPanel(); break;
	case FILE_NAME_SYNTAX_INDEX_BOARD: Text=GetIndexeBoard(); break;

	case FILE_NAME_SYNTAX_PUNC_HYPHEN: Text=GetPuncHyphen(); break;		
	case FILE_NAME_SYNTAX_PUNC_UNDER_LINE: Text=GetPuncUnderLine(); break;		
	case FILE_NAME_SYNTAX_PUNC_AT_SIGN: Text=GetPuncAtSign(); break;		
	case FILE_NAME_SYNTAX_PUNC_NUMBER_SIGN: Text=GetPuncNumberSign(); break;			

	case FILE_NAME_SYNTAX_USER_DEFINE_1: Text=GetUserdefineText1(); break;		
	case FILE_NAME_SYNTAX_USER_DEFINE_2: Text=GetUserdefineText2(); break;		
	case FILE_NAME_SYNTAX_USER_DEFINE_3: Text=GetUserdefineText3(); break;		
	default:
		bValid = false;
		Text.Format(_T("Exception[%d]"), Mode);
		break;
	}
	return bValid;	
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetFilenameSyntaxTag() const
{
	return m_Tag;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntax::SetFilenameSyntaxTag(LPCTSTR Tag)
{
	m_Tag = Tag;
}
//-------------------------------------------------------------------------------------//
bool CFilenameSyntax::GetFilenameSyntaxEnabled() const
{
	return m_Enabled;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntax::SetFilenameSyntaxEnabled(bool bEnable)
{
	m_Enabled = bEnable;
}
//-------------------------------------------------------------------------------------//
size_t CFilenameSyntax::GetObjectIndex() const
{
	return m_ObjectIndex;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntax::SetObjectIndex(size_t idx)
{
	m_ObjectIndex = idx;
}
//-------------------------------------------------------------------------------------//
int CFilenameSyntax::GetObjectIndexWidth() const
{
	return m_ObjectIndexWidth;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntax::SetObjectIndexWidth(int idx)
{
	m_ObjectIndexWidth = idx;
}
//-------------------------------------------------------------------------------------//
int CFilenameSyntax::GetPanelIndexWidth() const
{
	return m_PanelIndexWidth;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntax::SetPanelIndexWidth(int idx)
{
	m_PanelIndexWidth = idx;
}
//-------------------------------------------------------------------------------------//
int CFilenameSyntax::GetBoardIndexWidth() const
{
	return m_BoardIndexWidth;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntax::SetBoardIndexWidth(int idx)
{
	m_BoardIndexWidth = idx;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetMachineName()
{
	return AOIDataCollect.GetMachineAlias();
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetMachineLine()
{
	return AOIDataCollect.GetMachineLine(AOIDataCollect.GetActiveLaneID());
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetMachineStation()
{
	return AOIDataCollect.GetMachineStation(AOIDataCollect.GetActiveLaneID());
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetProjectFilename()
{
	m_NodeText=_T("ProjectName");
	CAOIProject *ProjectPtr=GetProjectPtr();
	if ( CheckProjectPtr(ProjectPtr) == false ) { return m_NodeText; }
	return ProjectPtr->GetProjectFileMainName();
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetProjectModuleName()
{
	m_NodeText=_T("ProjectModule");
	CAOIProject *ProjectPtr=GetProjectPtr();
	if ( CheckProjectPtr(ProjectPtr) == false ) { return m_NodeText; }
	m_NodeText = CString(ProjectPtr->GetProjectModuleName());
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetProjectLotName()
{
	m_NodeText=_T("ProjectLot");
	CAOIProject *ProjectPtr=GetProjectPtr();
	if ( CheckProjectPtr(ProjectPtr) == false ) { return m_NodeText; }
	m_NodeText = CString(ProjectPtr->GetProjectWorkNumber());
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetProjectLaneName()
{
	m_NodeText=_T("ProjectLane");
	CAOIProject *ProjectPtr=GetProjectPtr();
	if ( CheckProjectPtr(ProjectPtr) == false ) { return m_NodeText; }
	LANE_ID LaneID = ProjectPtr->GetProjectActLaneID();
	switch ( LaneID )
	{
	case LANE_ID_B: m_NodeText=_T("B"); break;
	default:
	case LANE_ID_A: 	
		m_NodeText=_T("A"); 
		break;
	}
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetProjectTimeTest_YYYYMMDD()
{	
	m_NodeText=_T("YYYYMMDD");
	CAOIProject *ProjectPtr=GetProjectPtr();
	if ( CheckProjectPtr(ProjectPtr) == false ) { return m_NodeText; }
	m_NodeText = ProjectPtr->GetProjectInspectionDateTime();	
	m_NodeText = m_NodeText.Left(8);
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetProjectTimeTest_YYYYMMDDhhmmss()
{
	m_NodeText=_T("YYYYMMDDhhmmss");
	CAOIProject *ProjectPtr=GetProjectPtr();
	if ( CheckProjectPtr(ProjectPtr) == false ) { return m_NodeText; }
	m_NodeText = ProjectPtr->GetProjectInspectionDateTime();	
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetBarcodeScope()//自動切換條碼
{	
	m_NodeText=_T("ScopeBarcode");
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetBarcodeProject()
{	
	m_NodeText=_T("ProjectBarcode");
	CAOIProject *ProjectPtr=GetProjectPtr();
	if ( CheckProjectPtr(ProjectPtr) == false ) { return m_NodeText; }
	m_NodeText = CString(ProjectPtr->GetProjectBarcode());
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetBarcodePanel()
{	
	m_NodeText=_T("PanelBarcode");
	CAOIPanel *PanelPtr=GetPanelPtr();
	if ( CheckPanelPtr(PanelPtr) == false ) { return false; }	
	m_NodeText = CString(PanelPtr->GetPanelBarcode());
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetBarcodeBoard()
{	
	m_NodeText=_T("BoardBarcode");
	CAOIBoard *BoardPtr=GetBoardPtr();
	if ( CheckBoardPtr(BoardPtr) == false ) { return false; }
	m_NodeText = CString(BoardPtr->GetBoardBarcode());
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetIndexObject()
{	
	size_t Index = GetObjectIndex();
	const int IndexWidth=GetObjectIndexWidth();
	m_NodeText=GetIndexText(Index, IndexWidth);
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetIndexPanel()
{	
	size_t     Index=0;		
	CAOIPanel *PanelPtr=GetPanelPtr();
	const int IndexWidth=GetPanelIndexWidth();	
	if ( CheckPanelPtr(PanelPtr) == true )	
	{	Index=PanelPtr->GetPanelIndex_Project()+1;	}
	m_NodeText=GetIndexText(Index, IndexWidth);
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetIndexeBoard()
{	
	size_t     Index=0;		
	CAOIBoard *BoardPtr=GetBoardPtr();
	const int IndexWidth=GetBoardIndexWidth();
	if ( CheckBoardPtr(BoardPtr) == true )	
	{	Index=BoardPtr->GetBoardIndex_Panel()+1;	}
	m_NodeText=GetIndexText(Index, IndexWidth);
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetPuncHyphen()//連字號-
{
	m_NodeText = "-";
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetPuncUnderLine()//下底線_
{
	m_NodeText = "_";
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetPuncAtSign()//@
{
	m_NodeText = "@";
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetPuncNumberSign()//#
{
	m_NodeText = "#";
	return m_NodeText;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetUserdefineText1()
{
	return m_UserDefineText1;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntax::SetUserdefineText1(LPCTSTR Name)
{
	m_UserDefineText1 = Name;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetUserdefineText2()
{
	return m_UserDefineText2;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntax::SetUserdefineText2(LPCTSTR Name)
{
	m_UserDefineText2 = Name;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetUserdefineText3()
{
	return m_UserDefineText3;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntax::SetUserdefineText3(LPCTSTR Name)
{
	m_UserDefineText3 = Name;
}
//-------------------------------------------------------------------------------------//
LPCTSTR CFilenameSyntax::GetExtensionName()
{
	return m_ExtName;
}
//-------------------------------------------------------------------------------------//
void CFilenameSyntax::SetExtensionName(LPCTSTR Name)
{
	m_ExtName = Name;
}
//-------------------------------------------------------------------------------------//