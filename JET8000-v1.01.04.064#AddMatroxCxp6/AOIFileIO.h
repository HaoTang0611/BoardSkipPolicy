// AOIFileIO.h: interface for the CAOIFileIO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AOIFILEIO_H__19F62D6B_9508_435A_83EB_E66ABB2F16F3__INCLUDED_)
#define AFX_AOIFILEIO_H__19F62D6B_9508_435A_83EB_E66ABB2F16F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-------------------------------------------------------------------------------------//
#include "AOIProject.h"
#include "AOIFileIODef.h"
//-------------------------------------------------------------------------------------//
class CAOIFileIO  
{
private:
	//---------------------------------------------------------------------------------//
	FILE                      *m_FilePtr;
	//---------------------------------------------------------------------------------//
	CString                    m_FnName;
	CString                    m_ErrorString;
	//---------------------------------------------------------------------------------//
	FILE_MODE                  m_FileMode;	
	//---------------------------------------------------------------------------------//
	FILE_TARGET                m_FileTarget;
	//---------------------------------------------------------------------------------//
	FILE_IO_MODE               m_FileIOMode;
	//---------------------------------------------------------------------------------//
	FILE_READ_MODE             m_FileReadMode;
	FILE_WRITE_MODE            m_FileWriteMode;
	//---------------------------------------------------------------------------------//
	bool                       m_LoadWStr;
	bool                       m_SaveWStr;
	//---------------------------------------------------------------------------------//
	CHUNK_TYPE                 m_ChunkType;
	TCHUNK_INT                 m_BinChunkINT;
	TCHUNK_DBL                 m_BinChunkDBL;
	TCHUNK_INT_64              m_BinChunkINT64;
	TCHUNK_STR_016             m_BinChunkSTR016;
	TCHUNK_STR_032             m_BinChunkSTR032;
	TCHUNK_STR_064             m_BinChunkSTR064;
	TCHUNK_STR_128             m_BinChunkSTR128;
	TCHUNK_STR_256             m_BinChunkSTR256;
	TCHUNK_STR_512             m_BinChunkSTR512;

	TCHUNK_WSTR_016            m_BinChunkWSTR016;
	TCHUNK_WSTR_032            m_BinChunkWSTR032;
	TCHUNK_WSTR_064            m_BinChunkWSTR064;
	TCHUNK_WSTR_128            m_BinChunkWSTR128;
	TCHUNK_WSTR_256            m_BinChunkWSTR256;
	TCHUNK_WSTR_512            m_BinChunkWSTR512;
	//---------------------------------------------------------------------------------//
	int                        m_TextSize;
	char                       m_TextLine[2048];
	char                       m_IndexLine[2048];
	char                       m_DataLine[2048];	
	
	int                        m_wTextSize;
	wchar_t                    m_wTextLine[2048];
	wchar_t                    m_wIndexLine[2048];
	wchar_t                    m_wDataLine[2048];	

	int                        m_Data_INT;
	double                     m_Data_DBL;	
	__int64                    m_Data_INT64;
	size_t                     m_RowCount;
	//---------------------------------------------------------------------------------//
	//Read File in Memory	
	size_t                     m_MapPos;
	size_t                     m_MapSize;
	size_t                     m_MapCount;
	char                      *m_MapBufA;
	wchar_t                   *m_MapBufW;
	bool                       m_MapAllocByPoolA;
	bool                       m_MapAllocByPoolW;
	//---------------------------------------------------------------------------------//
	int                        m_TotalNodes;
	int                        m_NNodeCount;
	int                        m_TotalModelObjs;
	int                        m_TotalComponents;
	CString                    m_FileName;
	CString                    m_FdFolder;
	CString                    m_FileFolder;
	CString                    m_LibraryFolder;
	CString                    m_PartLibraryFolder;
	//---------------------------------------------------------------------------------//
	CProgressCtrl              m_ProgressWnd;
	//---------------------------------------------------------------------------------//	
protected:
	//---------------------------------------------------------------------------------//
	CAOIFileIO(const CAOIFileIO &file);
	CAOIFileIO& operator=(const CAOIFileIO &file);
	//---------------------------------------------------------------------------------//
	void                       SetFileMode(FILE_MODE Mode); 
	void                       SetFileIOMode(FILE_IO_MODE Mode);
	void                       SetFileReadMode(FILE_READ_MODE Mode);	
	void                       SetFileWriteMode(FILE_WRITE_MODE Mode);		
	void                       SetLoadWStr(bool val);
	void                       SetSaveWStr(bool val);	
	//---------------------------------------------------------------------------------//
	bool                       CheckFileMode(FILE_MODE Mode);	
	bool                       CheckFileIOMode(FILE_IO_MODE Mode);//確定檔案存取模式
	//---------------------------------------------------------------------------------//
	bool                       CheckMapBufA();//確認緩存記憶體
	bool                       CheckMapBufW();//確認緩存記憶體
	//---------------------------------------------------------------------------------//
	bool                       CheckTxtPrintf(int val);//確認Txt存檔結果
	bool                       CheckUtfPrintf(int val);//確認Utf存檔結果
	bool                       CheckTxtBufPrintf(int val);//確認Txt緩存結果
	bool                       CheckUtfBufPrintf(int val);//確認Utf緩存結果
	//---------------------------------------------------------------------------------//
	bool                       SaveBinChunk_BOL(FILE *pfile, int index, bool value);
	bool                       SaveBinChunk_INT(FILE *pfile, int index, int value);
	bool                       SaveBinChunk_DBL(FILE *pfile, int index, double value);
	bool                       SaveBinChunk_STR(FILE *pfile, int index, const char *value);	
	bool                       SaveBinChunk_INT64(FILE *pfile, int index, __int64 value);
	bool                       SaveBinChunk_STR_016(FILE *pfile, int index, const char *value);
	bool                       SaveBinChunk_STR_032(FILE *pfile, int index, const char *value);
	bool                       SaveBinChunk_STR_064(FILE *pfile, int index, const char *value);
	bool                       SaveBinChunk_STR_128(FILE *pfile, int index, const char *value);
	bool                       SaveBinChunk_STR_256(FILE *pfile, int index, const char *value);
	bool                       SaveBinChunk_STR_512(FILE *pfile, int index, const char *value);

	bool                       SaveBinChunk_STR(FILE *pfile, int index, const wchar_t *value);
	bool                       SaveBinChunk_WSTR_016(FILE *pfile, int index, const wchar_t *value);
	bool                       SaveBinChunk_WSTR_032(FILE *pfile, int index, const wchar_t *value);
	bool                       SaveBinChunk_WSTR_064(FILE *pfile, int index, const wchar_t *value);
	bool                       SaveBinChunk_WSTR_128(FILE *pfile, int index, const wchar_t *value);
	bool                       SaveBinChunk_WSTR_256(FILE *pfile, int index, const wchar_t *value);
	bool                       SaveBinChunk_WSTR_512(FILE *pfile, int index, const wchar_t *value);
	
	bool                       LoadBinChunk(FILE *pfile, int &index, int &n32, double &f64, char *sValue, wchar_t *wValue, __int64 &n64);
	bool                       LoadBinChunk_INT(FILE *pfile, int &index, int &value);
	bool                       LoadBinChunk_DBL(FILE *pfile, int &index, double &value);
	bool                       LoadBinChunk_INT64(FILE *pfile, int &index, __int64 &value);

	bool                       LoadBinChunk_STR_016(FILE *pfile, int &index, char *value);
	bool                       LoadBinChunk_STR_032(FILE *pfile, int &index, char *value);
	bool                       LoadBinChunk_STR_064(FILE *pfile, int &index, char *value);
	bool                       LoadBinChunk_STR_128(FILE *pfile, int &index, char *value);
	bool                       LoadBinChunk_STR_256(FILE *pfile, int &index, char *value);
	bool                       LoadBinChunk_STR_512(FILE *pfile, int &index, char *value);

	bool                       LoadBinChunk_WSTR_016(FILE *pfile, int &index, wchar_t *value);
	bool                       LoadBinChunk_WSTR_032(FILE *pfile, int &index, wchar_t *value);
	bool                       LoadBinChunk_WSTR_064(FILE *pfile, int &index, wchar_t *value);
	bool                       LoadBinChunk_WSTR_128(FILE *pfile, int &index, wchar_t *value);
	bool                       LoadBinChunk_WSTR_256(FILE *pfile, int &index, wchar_t *value);
	bool                       LoadBinChunk_WSTR_512(FILE *pfile, int &index, wchar_t *value);
	//---------------------------------------------------------------------------------//
	bool                       SaveTxtChunk_BOL(FILE *pfile, FILE_IO_ID index, bool value);
	bool                       SaveTxtChunk_INT(FILE *pfile, FILE_IO_ID index, int value);
	bool                       SaveTxtChunk_DBL(FILE *pfile, FILE_IO_ID index, double value);
	bool                       SaveTxtChunk_STR(FILE *pfile, FILE_IO_ID index, const char *value);
	bool                       SaveTxtChunk_STR(FILE *pfile, FILE_IO_ID index, const wchar_t *value);
	bool                       SaveTxtChunk_INT64(FILE *pfile, FILE_IO_ID index, __int64 value);
	//---------------------------------------------------------------------------------//	
	bool                       SaveUtfChunk_BOL(FILE *pfile, FILE_IO_ID index, bool value);
	bool                       SaveUtfChunk_INT(FILE *pfile, FILE_IO_ID index, int value);
	bool                       SaveUtfChunk_DBL(FILE *pfile, FILE_IO_ID index, double value);
	bool                       SaveUtfChunk_STR(FILE *pfile, FILE_IO_ID index, const char *value);
	bool                       SaveUtfChunk_STR(FILE *pfile, FILE_IO_ID index, const wchar_t *value);
	bool                       SaveUtfChunk_INT64(FILE *pfile, FILE_IO_ID index, __int64 value);
	//---------------------------------------------------------------------------------//	
	bool                       FlushMapBufA(FILE *pfile);
	bool                       AddMapBufA(FILE *pfile, const char *Buf, size_t Size);//加入內存緩衝
	//---------------------------------------------------------------------------------//	
	bool                       CheckFlushMapBufW();
	bool                       FlushMapBufW(FILE *pfile);	
	bool                       AddMapBufW(FILE *pfile, const wchar_t *Buf, size_t Size);//加入內存緩衝	
	//---------------------------------------------------------------------------------//	
	bool                       SaveTxtBufChunk_BOL(FILE *pfile, FILE_IO_ID index, bool value);
	bool                       SaveTxtBufChunk_INT(FILE *pfile, FILE_IO_ID index, int value);
	bool                       SaveTxtBufChunk_DBL(FILE *pfile, FILE_IO_ID index, double value);
	bool                       SaveTxtBufChunk_STR(FILE *pfile, FILE_IO_ID index, const char *value);
	bool                       SaveTxtBufChunk_STR(FILE *pfile, FILE_IO_ID index, const wchar_t *value);
	bool                       SaveTxtBufChunk_INT64(FILE *pfile, FILE_IO_ID index, __int64 value);
	//---------------------------------------------------------------------------------//	
	bool                       SaveUtfBufChunk_BOL(FILE *pfile, FILE_IO_ID index, bool value);
	bool                       SaveUtfBufChunk_INT(FILE *pfile, FILE_IO_ID index, int value);
	bool                       SaveUtfBufChunk_DBL(FILE *pfile, FILE_IO_ID index, double value);
	bool                       SaveUtfBufChunk_STR(FILE *pfile, FILE_IO_ID index, const char *value);
	bool                       SaveUtfBufChunk_STR(FILE *pfile, FILE_IO_ID index, const wchar_t *value);
	bool                       SaveUtfBufChunk_INT64(FILE *pfile, FILE_IO_ID index, __int64 value);
	//---------------------------------------------------------------------------------//	
	bool                       OpenSaveFile_TXT(LPCTSTR filename);//開啟儲存檔案
	bool                       OpenSaveFile_BIN(LPCTSTR filename);//開啟儲存檔案
	bool                       OpenSaveFile_UTF(LPCTSTR filename);//開啟儲存檔案
	//---------------------------------------------------------------------------------//	
	bool                       OpenLoadFile_TXT(LPCTSTR filename);//開啟載入檔案	
	bool                       OpenLoadFile_BIN(LPCTSTR filename);//開啟載入檔案	
	bool                       OpenLoadFile_UTF(LPCTSTR filename);//開啟載入檔案	
	//---------------------------------------------------------------------------------//
	bool                       ReleaseMapBuf();
	bool                       CreateMapBufA(size_t sz);
	bool                       CreateMapBufW(size_t sz);
	size_t                     CalcFileSize();
	bool                       CopyFileToMapBufA();
	bool                       CopyFileToMapBufW();
	bool                       mgets(char *Buffer, int len, size_t &BufPos);
	bool                       mgetws(wchar_t *Buffer, int len, size_t &BufPos);	
	//---------------------------------------------------------------------------------//
public:
	//---------------------------------------------------------------------------------//
	CAOIFileIO();
	virtual ~CAOIFileIO();
	//---------------------------------------------------------------------------------//
	void                       SetFnName(LPCTSTR str);
	//---------------------------------------------------------------------------------//	
	void                       SetErrorString(LPCTSTR str);	
	LPCTSTR                    GetErrorString();
	//---------------------------------------------------------------------------------//	
	FILE_MODE                  GetFileMode() const { return m_FileMode;	}
	//---------------------------------------------------------------------------------//	
	bool                       GetLoadWStr() const { return m_LoadWStr; }
	bool                       GetSaveWStr() const { return m_SaveWStr; }	
	//---------------------------------------------------------------------------------//	
	bool                       CreateProgressWnd();
	bool                       DestroyProgressWnd();
	//---------------------------------------------------------------------------------//
	bool                       CheckFileEnd();//確認檔案尾點
	bool                       CheckFileMode();//確認檔案模式
	bool                       CheckFileOpened();//確認檔案開檔	
	bool                       CloseFile();//關閉檔案
	bool                       OpenSaveFile(LPCTSTR filename, FILE_MODE FileMode=FILE_MODE_UNICODE);//開啟儲存檔案
	bool                       OpenLoadFile(LPCTSTR filename, FILE_MODE FileMode=FILE_MODE_UNICODE);//開啟載入檔案	
	//---------------------------------------------------------------------------------//	
	bool                       SaveChunk_BOL(FILE_IO_ID index, bool value);
	bool                       SaveChunk_INT(FILE_IO_ID index, int value);
	bool                       SaveChunk_DBL(FILE_IO_ID index, double value);
	bool                       SaveChunk_STR(FILE_IO_ID index, const char *value);
	bool                       SaveChunk_STR(FILE_IO_ID index, const wchar_t *value);
	bool                       SaveChunk_INT64(FILE_IO_ID index, __int64 value);
	//---------------------------------------------------------------------------------//
	bool                       LoadChunk(int &index);
	//---------------------------------------------------------------------------------//
	bool                       GetData_BOL();
	int                        GetData_INT();
	float                      GetData_FLT();
	double                     GetData_DBL();	
	char*                      GetData_STR();
	wchar_t*                   GetData_WSTR();
	__int64                    GetData_INT64();	
	//---------------------------------------------------------------------------------//
	void                       SetFileName(LPCTSTR value) { m_FileName = value; }
	LPCTSTR                    GetFileName() const { return m_FileName; }
	//---------------------------------------------------------------------------------//	
	void                       SetFdFolder(LPCTSTR value) { m_FdFolder = value; }
	LPCTSTR                    GetFdFolder() const { return m_FdFolder; }
	//---------------------------------------------------------------------------------//
	void                       SetFileFolder(LPCTSTR value) { m_FileFolder = value; }
	LPCTSTR                    GetFileFolder() const { return m_FileFolder; }
	//---------------------------------------------------------------------------------//
	void                       SetLibraryFolder(LPCTSTR value) { m_LibraryFolder = value; }
	LPCTSTR                    GetLibraryFolder() const { return m_LibraryFolder; }
	//---------------------------------------------------------------------------------//
	void                       SetPartLibraryFolder(LPCTSTR value) { m_PartLibraryFolder = value; }
	LPCTSTR                    GetPartLibraryFolder() const { return m_PartLibraryFolder; }
	//---------------------------------------------------------------------------------//		
	void                       SetTotalNodes(int value) { m_TotalNodes = value; }
	int                        GetTotalNodes() const { return m_TotalNodes; }
	//---------------------------------------------------------------------------------//		
	void                       SetNNodeCount(int value) { m_NNodeCount = value; }
	int                        GetNNodeCount() const { return m_NNodeCount; }
	//---------------------------------------------------------------------------------//		
	void                       SetTotalModelObjs(int value) { m_TotalModelObjs = value; }
	int                        GetTotalModelObjs() const { return m_TotalModelObjs; }
	//---------------------------------------------------------------------------------//		
	void                       SetTotalComponents(int value) { m_TotalComponents = value; }
	int                        GetTotalComponents() const { return m_TotalComponents; }
	//---------------------------------------------------------------------------------//
	void                       SetFileTarget(FILE_TARGET value) { m_FileTarget = value; }
	FILE_TARGET                GetFileTarget() const { return m_FileTarget; }
	//---------------------------------------------------------------------------------//
	bool                       ReadIntListFile(std::vector<int> &List);
	bool                       WriteIntListFile(const std::vector<int> &List);
	//---------------------------------------------------------------------------------//
	bool                       ReadBoolListFile(std::vector<bool> &List);
	bool                       WriteBoolListFile(const std::vector<bool> &List);
	//---------------------------------------------------------------------------------//
	bool                       ReadDoubleListFile(std::vector<double> &List);
	bool                       WriteDoubleListFile(const std::vector<double> &List);
	//---------------------------------------------------------------------------------//
	bool                       ReadDoubleArrayFile(int Count, double List[]);
	bool                       WriteDoubleArrayFile(int Count, const double List[]);
	//---------------------------------------------------------------------------------//
	bool                       ReadStrListFile(std::vector<std::string> &List);
	bool                       WriteStrListFile(const std::vector<std::string> &List);
	//---------------------------------------------------------------------------------//
	bool                       ReadStrListFile(std::vector<std::wstring> &List);
	bool                       WriteStrListFile(const std::vector<std::wstring> &List);
	//---------------------------------------------------------------------------------//
	bool                       ReadPointFile(POINT &Pt);
	bool                       WritePointFile(const POINT &Pt);
	bool                       ReadPointListFile(std::vector<POINT> &PtList);
	bool                       WritePointListFile(const std::vector<POINT> &PtList);
	//---------------------------------------------------------------------------------//	
	bool                       ReadRectFile(RECT &Rect);
	bool                       WriteRectFile(const RECT &Rect);
	bool                       ReadRectListFile(std::vector<RECT> &RectList);
	bool                       WriteRectListFile(const std::vector<RECT> &RectList);
	//---------------------------------------------------------------------------------//
	bool                       ReadVersionCodeFile(TVersionCode &VersionCode);
	bool                       WriteVersionCodeFile(const TVersionCode &VersionCode);
	bool                       ReadVersionCodeListFile(std::vector<TVersionCode> &VersionList);
	bool                       WriteVersionCodeListFile(const std::vector<TVersionCode> &VersionList);
	//---------------------------------------------------------------------------------//
	bool                       ReadVersionParamFile(TVersionParam &VersionParam);
	bool                       WriteVersionParamFile(const TVersionParam &VersionParam);
	bool                       ReadVersionParamListFile(std::vector<TVersionParam> &VersionList);
	bool                       WriteVersionParamListFile(const std::vector<TVersionParam> &VersionList);
	//---------------------------------------------------------------------------------//
	bool                       ReadSpaceBasePlaneParamFile_II(TBasePlaneParam &BasePlaneParam);
	bool                       WriteSpaceBasePlaneParamFile_II(const TBasePlaneParam &BasePlaneParam);
	//---------------------------------------------------------------------------------//
	bool                       ReadSpaceNoiseFilterParamFile(TNoiseFilterParam &NoiseFilterParam);
	bool                       WriteSpaceNoistFilterParamFile(const TNoiseFilterParam &NoiseFilterParam);
	//---------------------------------------------------------------------------------//	
	bool                       ReadSpaceNoiseFilterParamFile_I(TNoiseFilterParam &NoiseFilterParam);
	//---------------------------------------------------------------------------------//
	bool                       ReadSpaceNoiseFilterParamFile_II(TNoiseFilterParam &NoiseFilterParam);
	bool                       WriteSpaceNoistFilterParamFile_II(const TNoiseFilterParam &NoiseFilterParam);
	//---------------------------------------------------------------------------------//	
	bool                       ReadXYDotNodeCaliFile(int &CountX, int &CountY, std::vector<TDotNode> &List);//讀取XY校正檔案
	bool                       WriteXYDotNodeCaliFile(int CountX, int CountY, const std::vector<TDotNode> &List);//寫入XY校正檔案
	//---------------------------------------------------------------------------------//
	bool                       ReadPhaseFactorTableFile(TPhaseFactorTable &GridTable);//讀取相位高度比例參數檔案
	bool                       WritePhaseFactorTableFile(const TPhaseFactorTable &GridTable);//寫入相位高度比例參數檔案
	//---------------------------------------------------------------------------------//
	bool                       ReadGroundEquationFile(CJetGroundEquation &GroundEquation);//讀取底面方程式參數檔案
	bool                       WriteGroundEquationFile(const CJetGroundEquation &GroundEquation);//讀取底面方程式參數檔案
	//---------------------------------------------------------------------------------//
	bool                       ReadGrrSigmaItemFile(TGrrSigmaItem &GrrSigmaItem);//讀取Grr標準差項目檔案
	bool                       WriteGrrSigmaItemFile(const TGrrSigmaItem &GrrSigmaItem);//讀取Grr標準差項目檔案
	//---------------------------------------------------------------------------------//
	bool                       ReadSigmaItemFile(TSigmaItem &SigmaItem);//讀取標準差項目檔案
	bool                       WriteSigmaItemFile(const TSigmaItem &SigmaItem);//讀取標準差項目檔案
	//---------------------------------------------------------------------------------//
	bool                       ReadSigmaTempFile(TSigmaTemp &SigmaTemp);//讀取標準差參數檔案
	bool                       WriteSigmaTempFile(const TSigmaTemp &SigmaTemp);//讀取標準差參數檔案
	//---------------------------------------------------------------------------------//
};
//-------------------------------------------------------------------------------------//
#endif // !defined(AFX_AOIFILEIO_H__19F62D6B_9508_435A_83EB_E66ABB2F16F3__INCLUDED_)
