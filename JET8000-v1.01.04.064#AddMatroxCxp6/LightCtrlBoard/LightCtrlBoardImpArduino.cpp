// LightCtrlBoardFpga.cpp: implementation of the CLightCtrlBoardImpArduino class.
//
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "LightCtrlBoard.h"
#include "LightCtrlBoardImpArduino.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//-------------------------------------------------------------------------------------//
#define ARDUINO_SOCKET_TIME_OUT       10000

#define ARDUINO_PARSER_COUNT_PC_FPGA     1
#define ARDUINO_PARSER_COUNT_FPGA_CCD    2
#define ARDUINO_PARSER_COUNT_FPGA_DLP    3
#define ARDUINO_PARSER_COUNT_DLP_FPGA    4
//-------------------------------------------------------------------------------------//
#define ARDUINO_STR_POS_TABLE_TYPE           1//表格樣式, LED, DLP
#define ARDUINO_STR_POS_DLP_TRG_CNT          2//DLP觸發次數
#define ARDUINO_STR_POS_DLP_CHANNEL          3//DLP通道數量
#define ARDUINO_STR_POS_DLP_ENABLE_RDY       4//DL啟用Ready訊號
#define ARDUINO_STR_POS_NEXT_TABLE_TIME      5//下張表格間隔時間
#define ARDUINO_STR_POS_LED_ON_TIME         13//LED燈點亮時間
#define ARDUINO_STR_POS_CCD_DELAY_TIME      21//相機延遲時間
#define ARDUINO_STR_POS_LED_CHANNEL_PWR     25//LED通道強度-第1個
#define ARDUINO_STR_POS_CCD_ENABLE_CHN      57//相機啟用通道
#define ARDUINO_STR_POS_DLP_PULSE_TIME      59//DLP脈波寬度時間
#define ARDUINO_STR_POS_DLP_CALLBACK_TIME   63//DLP回傳時間
#define ARDUINO_STR_POS_DLP_EXPOSURE_TIME   67//DLP曝光(點亮)時間
#define ARDUINO_STR_POS_DLP_READY_TIME      71//DLP準備好時間
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoard::CreateImp_Arduino()//建立_Imp指標	
{
	try
	{
		_Imp = new CLightCtrlBoardImpArduino();
		if ( NULL == _Imp )
		{
			m_ErrorString=_T("Error, CLightCtrlBoard CreateImp_Arduino Fault");
			return false;
		}
	}
	catch (...)
	{
		m_ErrorString=_T("Error, CLightCtrlBoard CreateImp_Arduino Fault");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------//
CLightCtrlBoardImpArduino::CLightCtrlBoardImpArduino():CLightCtrlBoardImp()
{	
#ifndef LIGHT_CTRL_DISABLE
	::InitializeCriticalSection(&m_csSocket);	

	m_IPPort = 23;
	m_IPAdress = "192.168.2.177";		
	SetTableMaxCount(80);
	SetSocketTimeout(5000);
	ClearAllCountValue();	
	SetTriggerTableCount(0);
	SetTriggerTableIndex(0);
	LoadLightCtrlBoardINI();
	SaveLightCtrlBoardINI();
	ResetSubBoardLedChannelValid();
#endif//LIGHT_CTRL_DISABLE
}
//-------------------------------------------------------------------------------------//
CLightCtrlBoardImpArduino::~CLightCtrlBoardImpArduino()
{
#ifndef LIGHT_CTRL_DISABLE
	CLightCtrlBoardImpArduino::DisConnect();	
	::DeleteCriticalSection(&m_csSocket);
#endif//LIGHT_CTRL_DISABLE		
}
//-------------------------------------------------------------------------------------//
CString CLightCtrlBoardImpArduino::GetSocketErrorString()
{
	return CString(m_Socket.GetErrorString().c_str());
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::LockBoard()//同步化
{
	::EnterCriticalSection(&m_csSocket);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::UnlockBoard()//同步化
{
	::LeaveCriticalSection(&m_csSocket);
	return true;
}
//-------------------------------------------------------------------------------------//
unsigned int CLightCtrlBoardImpArduino::GetSocketTimeout() const//網路逾時(ms)
{
	return m_SocketTimeout;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardImpArduino::SetSocketTimeout(unsigned int val)//網路逾時(ms)
{
	m_SocketTimeout = val;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardImpArduino::ResetDLPNotReadySignal()//復歸DLP沒有準備訊號
{
	for ( int i=0; i<ARDUINO_MAX_DLP_COUNT; i++ )
	{	m_DLPNotReadySignal[i] = false;	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::CheckDLPNotReadySignalFault(int DlpIdx) const//確認DLP沒有準備訊號
{
	if ( DlpIdx<0 || DlpIdx>=ARDUINO_MAX_DLP_COUNT ) { return false; }
	return m_DLPNotReadySignal[DlpIdx];
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardImpArduino::DecodeDLPNotReadySignal(const std::string &Str)//解碼DLP沒有準備訊號
{
	size_t Pos=0;
	char str[32]="";		
	for ( int i=0; i<ARDUINO_MAX_DLP_COUNT; i++ )
	{
		::sprintf(str, "DLP%d Not Ready", i+1);
		Pos = Str.find(str);
		if ( std::string::npos == Pos ) { continue; }
		m_DLPNotReadySignal[i] = true;		
	}
	return;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ShowTrace(const std::string &Str)//顯示訊息
{
//#ifdef _DEBUG
	std::string s=Str+std::string("\n");	
	//TRACE0(CString(s.c_str()));
	TRACE(CString(s.c_str()));
//#endif //_DEBUG	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::CheckSocketTimeout(long time)//確認Socket逾時
{
	//return false;
	//if ( time > ARDUINO_SOCKET_TIME_OUT )
	if ( time > m_SocketTimeout )
	{
		m_ErrorString=_T("Error, Socket Timeout");
		return true; 
	}
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::CheckStringValid(const char *Str)//確認字串是否有效
{
	if ( NULL == Str ) 
	{
		m_ErrorString=_T("Error, Input String Fault");
		return false; 
	}
	return CheckStringValid(std::string(Str));	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::CheckStringValid(const std::string &Str)//確認字串是否有效	
{
	if ( Str.length() == 0 )
	{
		m_ErrorString=_T("Error, Input String Fault");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::CheckStringLen(size_t Pos, size_t Len)//確認字串長度
{
	if ( Pos > Len )
	{
		m_ErrorString=_T("Error, String Length Fault");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::CheckStringLen(const std::string &Str, size_t Pos)//確認字串長度
{
	if ( Pos > Str.length() )
	{
		m_ErrorString=_T("Error, String Length Fault");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImpArduino::HexStrToInt(const std::string &Str) const//16進位制字串轉成整數
{
	return std::stoi(Str, nullptr, 16);	
}
//-------------------------------------------------------------------------------------//
std::string CLightCtrlBoardImpArduino::IntToBinStr(int Val, int Cnt, char profix) const//整數轉成02進位制字串	
{
	int i=0;
	int base=1;
	std::string str(Cnt, profix);
	for ( i=0; i<Cnt; i++ )
	{
		if ( (base&Val) != 0 )
		{	str[Cnt-i-1]='1';	}
		base = base << 1;
	}
	return str;
}
//-------------------------------------------------------------------------------------//
std::string CLightCtrlBoardImpArduino::IntToHexStr(int Val, int Cnt, char profix) const//整數轉成16進位制字串	
{
	std::stringstream stream;
	stream << std::hex << std::setw(Cnt) << std::setfill(profix) << Val;
	return std::string(stream.str());	
}
//-------------------------------------------------------------------------------------//
size_t CLightCtrlBoardImpArduino::FindString(const std::string &str, const std::string &Sub, int Num, int Start)//尋找子字串
{
	size_t Pos=Start-1;
	for ( int i=0; i<Num; i++ )
	{	Pos=str.find(Sub, Pos+1);	}
	return Pos;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::PacketCommandString(const std::string &Str, std::string &Packet)//封裝成命令字串//<Cmd>	
{
	if ( CheckStringValid(Str) == false ) { return false; }
	Packet=std::string("<")+Str+std::string(">");
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::UnpacketCommandString(const std::string &Str, std::string &Packet)//解封成命令字串//<Cmd>--Cmd
{
	const size_t Len=Str.length();
	if ( Len < 2 )
	{
		m_ErrorString.Format(_T("Error, UnpacketCommandString Fault [%s]"), CString(Str.c_str()));
		return false;
	}
	Packet=std::string(Str.begin()+1, Str.end()-1);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::PacketUploadEndString(const std::string &Str, std::string &Packet)//封裝成上傳結束字串//[Table1][Table2]...[TableN]@
{
	if ( CheckStringValid(Str) == false ) { return false; }
	Packet=Str+std::string("@");	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::CopySubString(const char *Str, size_t Len, size_t Pos, size_t SubLen, char *Buf)//複製子字串
{
	if ( CheckStringLen(Pos, Len) == false ) { return false; }
	if ( CheckStringLen(Pos+SubLen, Len) == false ) { return false; }
	size_t i=0;
	for ( i=0; i<SubLen; i++ )
	{	Buf[i] = Str[Pos+i];	}
	Buf[i]='\0';
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::CopySubString(const std::string &Str, size_t Pos, size_t SubLen, std::string &Buf)//複製子字串
{
	if ( CheckStringLen(Str, Pos) == false ) { return false; }
	if ( CheckStringLen(Str, Pos+SubLen) == false ) { return false; }
	Buf=Str.substr(Pos, SubLen);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SocketWaitAck()//等待板子回應
{
	std::string Result;
	if ( SocketRead(Result) == false )
	{	return false; }
	if ( CheckFaultString(Result) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SocketRead(std::string &Result)//讀取Socket
{
	if ( CheckIsConnected() == false ) { return false; }
	std::string Result_All;
	std::string Result_Each;			
	clock_t start_t, end_t, timelength = 0;	

	start_t = clock();	
	do
	{			
		if ( m_Socket.ReceiveBytes(Result_Each) == false )
		{
			CString err=GetSocketErrorString();
			m_ErrorString.Format(_T("%s [%s]"), err, _T("SocketRead"));
			return false; 
		}
		Result_All += Result_Each;		
		const size_t len=Result_Each.length();
		if ( len > 0 )
		{
			char End=Result_Each[len-1];
			if ( End == '>')
			{	break; }
		}

		end_t = clock();
		timelength = end_t - start_t;
		if ( CheckSocketTimeout(timelength) == true )
		{	return false; }

	} while ( true );
	Result=Result_All;
	ShowTrace(Result_All);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::CheckFaultString(const std::string &Result)//確認回傳錯誤
{
	std::vector<std::string> OKList;	
	OKList.push_back("Ack");	
	OKList.push_back("Ready");	
	const size_t OKCount=OKList.size();
	for ( size_t i=0; i<OKCount; i++ )
	{
		const std::string &OK=OKList[i];
		size_t Pos=Result.find(OK.c_str());
		if ( Pos!=Result.npos )
		{	return true; }		
	}	
	m_ErrorString=(std::string("Error, ")+Result).c_str();
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SocketWriteCmd(const std::string &Str, bool bWaitAck)//寫入命令至Socket
{
	std::string str;	
	if ( CheckIsConnected() == false ) { return false; }	
	if ( PacketCommandString(Str, str) == false ) { return false; }
	if ( SocketWriteRaw(str, bWaitAck) == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SocketWriteData(const std::string &Str, bool bWaitAck)//寫入資料至Socket
{
	if ( SocketWriteRaw(Str, bWaitAck) == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SocketWriteRaw(const std::string &Str, bool bWaitAck)//寫入至Socket
{	
	if ( CheckIsConnected() == false ) { return false; }		
	ShowTrace(Str);		
	if ( m_Socket.SendBytes(Str) == false )
	{		
		CString err=GetSocketErrorString();
		m_ErrorString.Format(_T("%s [Write::%s]"), err, CString(Str.c_str()));
		return false;
	}
	if ( true == bWaitAck )
	{
		if ( SocketWaitAck() == false )
		{	return false; }
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ExecBoardCmd(const std::string &Cmd)//寫入控制板資料
{
	bool IsOK=true;
	LockBoard();
	IsOK = ExecBoardCmdFn(Cmd);
	UnlockBoard();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ExecBoardCmdFn(const std::string &Cmd)//執行控制板命令	
{	
	const bool bWaitAck=true;	
	return SocketWriteCmd(Cmd, bWaitAck);
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ReadBoardData(const std::string &Cmd, std::string &Result)//讀取控制板資料
{
	bool IsOK=true;
	LockBoard();
	IsOK = ReadBoardDataFn(Cmd, Result);
	UnlockBoard();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ReadBoardDataFn(const std::string &Cmd, std::string &Result)//讀取控制板資料
{	
	const bool bWaitAck=false;	
	if ( SocketWriteCmd(Cmd, bWaitAck) == false )
	{	return false; }
	if ( SocketRead(Result) == false )
	{	return false; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::WriteBoardData(const std::string &Cmd, const std::string &Result)//寫入控制板資料
{
	return ExecBoardCmd(Cmd+Result);
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::WriteBoardAllTable()//寫入控制板全部表格
{
	if ( WriteBoardAllTable(m_AllTableStringW) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::WriteBoardAllTable(const std::string &Data)//寫入控制板全部表格
{
	bool IsOK = true;
	LockBoard();
	IsOK = WriteBoardAllTableFn(Data);
	UnlockBoard();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::WriteBoardAllTableFn(const std::string &Data)//寫入控制板全部表格
{	
	std::string Str;
	const bool bWaitAck=true;
	if ( PacketUploadEndString(Data, Str) == false ) 
	{ return false; }
	if ( ExecBoardCmd("upload") == false )		
	{	return false; }	
	//BuildDebugTableString(Str);		
	if ( SocketWriteData(Str, bWaitAck) == false )		
	{	return false; }			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ReadBoardAllChannel()//讀取控制板所有通道
{
	std::string Result;		
	ResetSubBoardLedChannelValid();
	const unsigned int SocketTimeout=GetSocketTimeout();
	const unsigned int ReadAllChannelTimeout=MAX(10000, SocketTimeout);
	SetSocketTimeout(ReadAllChannelTimeout);
	const bool bSucc=ReadBoardData("scanchn", Result);
	SetSocketTimeout(SocketTimeout);
	if ( false == bSucc )
	{	return false; }	
	const size_t Len=Result.length();
	const size_t SubCount=ARDUINO_SUB_BOARD_COUNT;
	const size_t LedCount=ARDUINO_MAX_LED_COUNT;	
	const size_t StartPos=Result.find(":");
	if ( StartPos == Result.npos )
	{
		m_ErrorString.Format(_T("Error, Read Board All Channel Fault[%s]"), CString(Result.c_str()));
		return false;
	}
	const size_t ValidLen=StartPos+LedCount+SubCount+2;//<>
	if ( Len < ValidLen )
	{
		m_ErrorString.Format(_T("Error, Read Board All Channel Fault[%s]"), CString(Result.c_str()));
		return false;
	}

	//xxxxyyyyyyyyyyyyyyyy
	size_t i=0;
	const size_t SubStartIdx=StartPos+1;	
	for ( i=0; i<SubCount; i++ )
	{
		if ( '0' != Result[i+SubStartIdx] )	{ m_SubBoardValid[i]=true; }
	}
	const size_t LedStartIdx=SubStartIdx+SubCount;
	for ( i=0; i<LedCount; i++ )
	{
		if ( '0' != Result[i+LedStartIdx] )	{ m_LedChannelValid[i]=true; }		
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ReadBoardAllTable(std::vector<TLCB_TRIG_TABLE> &rList)//讀取控制板所有表格	
{
	bool IsOK=true;
	LockBoard();
	IsOK=ReadBoardAllTableFn(rList);
	UnlockBoard();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ReadBoardAllTableFn(std::vector<TLCB_TRIG_TABLE> &rList)//讀取控制板所有表格	
{
	std::string Result;	
	rList.clear();
	if ( ReadBoardData("loadall", Result) == false )
	{	return false; }
	//Result="<[000100000000000013880064200000100000000000000000000000000101FFFFFF000000000000]>";
	if ( ParserTableString(Result, rList)==false )//解析表格字串
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ReadBoardTableCount(int &Count)//讀取控制板的表格數
{
	bool IsOK=true;
	LockBoard();
	IsOK=ReadBoardTableCountFn(Count);
	UnlockBoard();
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ReadBoardTableCountFn(int &Count)//讀取控制板的表格數
{
	std::string sTableCount;
	if ( ReadBoardData("tabcoun", sTableCount) == false )	
	{	return false; }	
	if ( ParserTableCount(sTableCount, Count) == false )
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ParserTableCount(const std::string &str, int &Count)//剖析數量
{
	Count = 0;
	const size_t Pos1=str.find("<");
	if ( str.npos == Pos1 ) { return false; }
	const size_t Pos2=str.find(">");
	if ( str.npos == Pos2 ) { return false; }
	if ( Pos2 < Pos1 ) { return Count; }		
	std::string subStr=str.substr(Pos1+1, Pos2-Pos1-1);
	Count=::atoi(subStr.c_str());
	return Count;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ParserBoardError(const std::string &Str, std::string &Error, bool &IsError)//剖析子板錯誤
{	//B1:1123;B2:11023;
	Error="";
	IsError = false;
	const size_t Len=Str.length();
	const size_t PosS=Str.find(":", 0);
	const size_t PosE=Str.find("2", PosS+1);
	if ( PosS == Str.npos ) { return false; }
	if ( PosE == Str.npos ) { return false; }

	std::string err, tmp, tmp2;
	std::string sBoard=Str.substr(0, PosS);
	std::string sCurFault=Str.substr(PosS+2, PosE-PosS-2);
	std::string sVolFault=Str.substr(PosE+1, Len-PosE-1);	
	int nCurFault=HexStrToInt(sCurFault);
	int nVolFault=HexStrToInt(sVolFault);	
	sCurFault = IntToBinStr(nCurFault, 16);
	sVolFault = IntToBinStr(nVolFault, 16);	

	if ( 0!=nCurFault || 0!=nVolFault )
	{	IsError = true;	}

	tmp=std::string("A_")+sCurFault;
	if ( err.length() == 0 )
	{	err = tmp; }
	else
	{
		tmp2 = err;
		err=tmp2+std::string(", ")+tmp;
	}

	tmp=std::string("V_")+sVolFault;
	if ( err.length() == 0 )
	{	err = tmp; }
	else
	{
		tmp2 = err;
		err=tmp2+std::string(", ")+tmp;
	}
	if ( err.length() != 0 )
	{	Error=sBoard+std::string("[")+err+std::string("]");	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::DecodeBoardError(const std::string &Str, std::string &Error, bool &IsError, size_t &EndPos)//剖析子板錯誤
{
	IsError = false;
	const size_t StrLen=Str.length();
	if ( 0 == StrLen ) { return true; }

	bool bErr=false;
	size_t PosS=0, PosE=0, SubLen=0;	
	std::string SubStr, BoardErr, AllErr;	
	while ( true )
	{
		if ( 0!=PosS || Str[0]!='B' )		
		{	
			PosS=Str.find("B", PosS+1);
			if ( PosS == Str.npos ) { break; }
		}
		PosE=Str.find(";", PosS+1);
		if ( PosE == Str.npos ) { break; }
		SubStr=Str.substr(PosS, PosE-PosS+1);
		PosS = PosE;		
		
		bErr=false;
		BoardErr="";
		ParserBoardError(SubStr, BoardErr, bErr);
		if ( true == bErr )
		{	IsError = true; }

		if ( 0 == AllErr.length() )
		{	AllErr = BoardErr; }
		else
		{
			SubStr=AllErr;
			AllErr=SubStr+std::string("\n")+BoardErr;
		}		
	}
	Error = AllErr;
	EndPos = PosE;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::DecodeOtherError(const std::string &Str, size_t LastPos, std::string &Error, bool &IsError)//剖析其餘錯誤
{
	IsError = false;
	const size_t PosE = LastPos;
	const size_t StrLen=Str.length();
	if ( StrLen == PosE ) { return true; }
	
	IsError = true;
	Error=Str.substr(PosE, StrLen-PosE);	
	return true;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImpArduino::ParserStringCount(int Mode, int Channel, const std::string &str)//剖析數量
{
	int Count=0;
	std::string subStr;		
	size_t First=0, End=0;	
	size_t Pos1=0, Pos2=0;
	
	if ( ARDUINO_PARSER_COUNT_PC_FPGA == Mode )
	{
		Pos1=str.find("<");
		if ( str.npos == Pos1 ) { return Count; }
		Pos2=str.find(",");
		if ( str.npos == Pos2 ) { return Count; }
		if ( Pos2 < Pos1 ) { return Count; }		
		subStr=str.substr(Pos1+1, Pos2-Pos1-1);
		Count=::atoi(subStr.c_str());
		return Count;
	}
	if ( ARDUINO_PARSER_COUNT_FPGA_CCD == Mode )
	{
		First=str.find(",");		
		if ( str.npos == First ) { return Count; }
		End=str.find(";");
		if ( str.npos == End ) { return Count; }
		if ( End < First ) { return Count; }		
		Pos1=Pos2=First;
		for ( int i=0; i<Channel+1; i++ )
		{
			Pos1=Pos2;
			Pos2=str.find(",", Pos1+1);
			if ( str.npos==Pos2 || Pos2>End )			
			{
				Pos2=str.find(";", Pos1+1);
				if ( str.npos == Pos2 ) { return Count; }
			}
		}
		subStr=str.substr(Pos1+1, Pos2-Pos1-1);	
		Count=::atoi(subStr.c_str());
		return Count;
	}
	if ( ARDUINO_PARSER_COUNT_FPGA_DLP == Mode )
	{		
		First=FindString(str, ";", 1);
		if ( str.npos == First ) { return Count; }
		End=str.find(";", First+1);
		if ( str.npos == End ) { return Count; }
		if ( End < First ) { return Count; }		
		Pos2=First;
		for ( int i=0; i<Channel+1; i++ )
		{
			Pos1=Pos2;
			Pos2=str.find(",", Pos1+1);
			if ( str.npos==Pos2 || Pos2>End )			
			{
				Pos2=str.find(";", Pos1+1);
				if ( str.npos == Pos2 ) { return Count; }
			}
		}
		subStr=str.substr(Pos1+1, Pos2-Pos1-1);	
		Count=::atoi(subStr.c_str());		
		return Count;
	}
	if ( ARDUINO_PARSER_COUNT_DLP_FPGA == Mode )
	{
		First=FindString(str, ";", 2);
		if ( str.npos == First ) { return Count; }
		End=str.find(";", First+1);
		if ( str.npos == End ) { return Count; }
		if ( End < First ) { return Count; }		
		Pos2=First;
		for ( int i=0; i<Channel+1; i++ )
		{
			Pos1=Pos2;
			Pos2=str.find(",", Pos1+1);
			if ( str.npos==Pos2 || Pos2>End )			
			{
				Pos2=str.find(";", Pos1+1);
				if ( str.npos == Pos2 ) { return Count; }
			}			
		}
		subStr=str.substr(Pos1+1, Pos2-Pos1-1);	
		Count=::atoi(subStr.c_str());
		return Count;
	}	
	return Count;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ParserTableString(const std::string &Str, std::vector<TLCB_TRIG_TABLE> &rTableList)//解析表格字串
{		
	std::string SubStr;
	size_t PosS=0, PosE=0;
	TLCB_TRIG_TABLE tmpTable;
	const size_t Len=Str.length();		

	PosE=-1;
	rTableList.clear();
	while ( true )
	{
		PosS=PosE;
		PosS=Str.find("[", PosS+1);
		if ( PosS == Str.npos ) { break; }
		PosE=Str.find("]", PosS+1);
		if ( PosE == Str.npos ) { break; }

		SubStr=Str.substr(PosS, PosE-PosS+1);
		if ( String2Table(SubStr, tmpTable) == false )
		{	return false; }

		tmpTable.sTableID=(short)(rTableList.size());
		rTableList.push_back(tmpTable);
	};
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Parser2LedItemString(const std::string &Str, TLCB_LED_ITEM &PWM1, TLCB_LED_ITEM &PWM2)//解析兩個LED字串
{
	const size_t Len=Str.length();
	if ( 4 != Len )
	{
		m_ErrorString.Format(_T("Error, Parser LED Item String Fault\n%s"), CString(Str.c_str()));
		return false;
	}

	unsigned int TempI=0;
	const unsigned int Value=HexStrToInt(Str);
	
	TempI = Value&0x00ff;
	if( TempI&0x80 ) { PWM2.sIsON = false; } else { PWM2.sIsON = true; }
	PWM2.sPower = (TempI&0x7f);
	
	TempI = (Value&0xff00)>>8;
	if( TempI&0x80 ) { PWM1.sIsON = false; } else { PWM1.sIsON = true; }
	PWM1.sPower = (TempI&0x7f);
	return true;

	/*
	TempI = Value&0x00ff;
	if( TempI&0x80 ) { PWM1.sIsON = true; } else { PWM1.sIsON = false; }
	PWM1.sPower = (TempI&0x7f);	
	
	TempI = (Value&0xff00)>>8;
	if( TempI&0x80 ) { PWM2.sIsON = true; } else { PWM2.sIsON = false; }
	PWM2.sPower = (TempI&0x7f);
	*/
	return true;
}
//-------------------------------------------------------------------------------------//
unsigned int CLightCtrlBoardImpArduino::GetTriggerTableCount() const//觸發表格數量
{
	return m_TriggerTableCount;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardImpArduino::SetTriggerTableCount(unsigned int val)//觸發表格數量
{
	m_TriggerTableCount=val;
}
//-------------------------------------------------------------------------------------//
unsigned int CLightCtrlBoardImpArduino::GetTriggerTableIndex() const//觸發表格起始引數
{
	return m_TriggerTableIndex;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardImpArduino::SetTriggerTableIndex(unsigned int val)//觸發表格起始引數
{
	m_TriggerTableIndex = val;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ClearAllCountValue()//清除所有計數
{
	m_PCtoFPGATrigCount = 0;
	::memset(m_FPGAtoCCDTrigCount, 0x00, sizeof(m_FPGAtoCCDTrigCount));
	::memset(m_FPGAtoDLPTrigCount, 0x00, sizeof(m_FPGAtoDLPTrigCount));
	::memset(m_DLPtoFPGATrigCount, 0x00, sizeof(m_DLPtoFPGATrigCount));
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::UpdateSupportFuncByVersion()//依據版本號更新支援功能
{
	CString Version = m_FPGAVersion;
	bool bMoreThan64Table=true;
	bool bModifyTriggerFirstIndex = true;
	SetTableMaxCount(MAX_TABLE_COUNT);
	SetSupportMoreThan64Table(bMoreThan64Table);
	SetSupportModifyTriggerFirstIndex(bModifyTriggerFirstIndex);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ResetSubBoardLedChannelValid()//清除子板與LED通道有效
{
	::memset(m_SubBoardValid, 0x00, sizeof(m_SubBoardValid));
	::memset(m_LedChannelValid, 0x00, sizeof(m_LedChannelValid));
	return true;
}
//-------------------------------------------------------------------------------------//	
bool CLightCtrlBoardImpArduino::CheckUploadData(const std::string &Str, char ch)//確認上傳資料未填滿
{
	size_t Pos=Str.find(ch);
	if ( Str.npos != Pos )
	{		
		m_ErrorString.Format(_T("Error, Check Upload Data Fault\n%s"), CString(Str.c_str()));
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
unsigned int CLightCtrlBoardImpArduino::CombineLedItem2(const TLCB_LED_ITEM &PWM1, const TLCB_LED_ITEM &PWM2)//合併兩個LED電源參數
{
	unsigned int data=0;
	unsigned int tmpData=0;

	//(bit0~7)
	tmpData = PWM2.sPower&0x7f;
	if( PWM2.sIsON == false )	{ tmpData += 0x80; }
	data = tmpData;

	//(bit15~8)	
	tmpData = PWM1.sPower&0x7f;
	if( PWM1.sIsON == false )	{ tmpData += 0x80; }
	data += (tmpData<<8);
	return data;

	/*
	//(bit0~7)
	tmpData = PWM1.sPower&0x7f;
	if( PWM1.sIsON == true )	{ tmpData += 0x80; }
	data = tmpData;

	//(bit15~8)	
	tmpData = PWM2.sPower&0x7f;
	if( PWM2.sIsON == true )	{ tmpData += 0x80; }
	data += (tmpData<<8);	
	*/
	return data;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardImpArduino::ClearAllTableStringW()//清除表格文字-寫入
{	
	m_AllTableStringW.clear();
}
//-------------------------------------------------------------------------------------//
const std::string& CLightCtrlBoardImpArduino::GetAllTableStringW() const//取得表格文字-寫入
{
	return m_AllTableStringW;
}
//-------------------------------------------------------------------------------------//
void  CLightCtrlBoardImpArduino::AddAllTableStringW(const std::string &Str)//加入新的表格文字-寫入
{
	m_AllTableStringW += Str;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Table2String(const TLCB_TRIG_TABLE &rTable, std::string &rStr)//表格轉字串
{
	const char ch='*';
	const size_t Len=78;	
	rStr.resize(Len+1, ch);

	rStr[0]='[';
	const size_t Len2=rStr.length();
	if ( Table2String_Type(rTable, rStr)==false ) { return false; }
	if ( Table2String_DlpTrgCnt(rTable, rStr)==false ) { return false; }
	if ( Table2String_DlpChanne(rTable, rStr)==false ) { return false; }
	if ( Table2String_DlpEnbRdy(rTable, rStr)==false ) { return false; }
	if ( Table2String_NextTableTime(rTable, rStr)==false ) { return false; }
	if ( Table2String_LedOnTime(rTable, rStr)==false ) { return false; }
	if ( Table2String_CcdDelayTime(rTable, rStr)==false ) { return false; }
	if ( Table2String_LedChPwr(rTable, rStr)==false ) { return false; }
	if ( Table2String_CameraEnable(rTable, rStr)==false ) { return false; }
	if ( Table2String_DlpPulseTime(rTable, rStr)==false ) { return false; }
	if ( Table2String_DlpCallbackTime(rTable, rStr)==false ) { return false; }
	if ( Table2String_DlpExposureTime(rTable, rStr)==false ) { return false; }
	if ( Table2String_DlpReadyTime(rTable, rStr)==false ) { return false; }
	rStr[Len]=']';

	if ( CheckUploadData(rStr, ch) == false )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Table2String_Type(const TLCB_TRIG_TABLE &rTable, std::string &rStr)//D:DLP, Others:LED
{	//[1]
	const int Pos=ARDUINO_STR_POS_TABLE_TYPE;	
	if ( TABLE_TYPE_DLP == rTable.sTableType )
	{	rStr[Pos] = 'D';	}
	else//TABLE_TYPE_LED
	{	rStr[Pos] = '8';	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Table2String_DlpTrgCnt(const TLCB_TRIG_TABLE &rTable, std::string &rStr)//DLP Trigger Count:[1]
{	//[2]
	const int Pos=ARDUINO_STR_POS_DLP_TRG_CNT;
	rStr[Pos] = '1';
	rStr[Pos] = IntToHexStr(rTable.sDLPTrigOutNumber, 1)[0];		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Table2String_DlpChanne(const TLCB_TRIG_TABLE &rTable, std::string &rStr)//DLP Channel:[1~8]
	{//[3]
	const int Pos=ARDUINO_STR_POS_DLP_CHANNEL;	
	rStr[Pos] = '4';
	rStr[Pos] = IntToHexStr(rTable.sDLPActiveChannel, 1)[0];			
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Table2String_DlpEnbRdy(const TLCB_TRIG_TABLE &rTable, std::string &rStr)//DLP Ready Mode[1,0];
{	//[4]
	const int Pos=ARDUINO_STR_POS_DLP_ENABLE_RDY;	
	rStr[Pos] = '0';
	rStr[Pos] = IntToHexStr(rTable.sDLPReadySignalEnable, 1)[0];			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Table2String_NextTableTime(const TLCB_TRIG_TABLE &rTable, std::string &rStr)//Table Between Time
{	//[5~12]
	const int Cnt=8;
	const int Pos=ARDUINO_STR_POS_NEXT_TABLE_TIME;	
	std::string sTime=IntToHexStr(rTable.sNextTableTime, Cnt);
	const size_t Len=sTime.length();
	const size_t LenUse=MIN(Len, Cnt);
	::memcpy(&rStr[Pos], sTime.c_str(), sizeof(char)*LenUse);	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Table2String_LedOnTime(const TLCB_TRIG_TABLE &rTable, std::string &rStr)//LED Turn On Time
{	//[13~20]
	const int Cnt=8;
	const int Pos=ARDUINO_STR_POS_LED_ON_TIME;
	std::string sTime=IntToHexStr(rTable.sLEDTableTotalTime, Cnt);
	const size_t Len=sTime.length();
	const size_t LenUse=MIN(Len, Cnt);
	::memcpy(&rStr[Pos], sTime.c_str(), sizeof(char)*LenUse);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Table2String_CcdDelayTime(const TLCB_TRIG_TABLE &rTable, std::string &rStr)//CCD Delay Time
{	//[21~24]
	const int Cnt=4;
	const int Pos=ARDUINO_STR_POS_CCD_DELAY_TIME;
	std::string sTime=IntToHexStr(rTable.sCCDDelayTime, Cnt);
	const size_t Len=sTime.length();
	const size_t LenUse=MIN(Len, Cnt);
	::memcpy(&rStr[Pos], sTime.c_str(), sizeof(char)*LenUse);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Table2String_LedChPwr(const TLCB_TRIG_TABLE &rTable, std::string &rStr)//LED Channel Power
{	//[25~26], [27~28], ... [55~56]//Total 16 channels	
	int Pos=0;
	std::string sData;
	unsigned int data=0;
	const int Size=2;
	const int Size2=Size*2;
	const int Start=ARDUINO_STR_POS_LED_CHANNEL_PWR;		
	
	Pos = Start;
	//01-02
	data=CombineLedItem2(rTable.sPWM1, rTable.sPWM2);	
	sData=IntToHexStr(data, Size2);
	::memcpy(&rStr[Pos], sData.c_str(), sizeof(char)*Size2);	
	Pos += Size2;

	//03-04
	data=CombineLedItem2(rTable.sPWM3, rTable.sPWM4);	
	sData=IntToHexStr(data, Size2);
	::memcpy(&rStr[Pos], sData.c_str(), sizeof(char)*Size2);	
	Pos += Size2;

	//05-06
	data=CombineLedItem2(rTable.sPWM5, rTable.sPWM6);	
	sData=IntToHexStr(data, Size2);
	::memcpy(&rStr[Pos], sData.c_str(), sizeof(char)*Size2);	
	Pos += Size2;

	//07-08
	data=CombineLedItem2(rTable.sPWM7, rTable.sPWM8);	
	sData=IntToHexStr(data, Size2);
	::memcpy(&rStr[Pos], sData.c_str(), sizeof(char)*Size2);	
	Pos += Size2;

	//09-10
	data=CombineLedItem2(rTable.sPWM9, rTable.sPWM10);	
	sData=IntToHexStr(data, Size2);
	::memcpy(&rStr[Pos], sData.c_str(), sizeof(char)*Size2);	
	Pos += Size2;

	//11-12
	data=CombineLedItem2(rTable.sPWM11, rTable.sPWM12);	
	sData=IntToHexStr(data, Size2);
	::memcpy(&rStr[Pos], sData.c_str(), sizeof(char)*Size2);	
	Pos += Size2;

	//13-14
	data=CombineLedItem2(rTable.sPWM13, rTable.sPWM14);	
	sData=IntToHexStr(data, Size2);
	::memcpy(&rStr[Pos], sData.c_str(), sizeof(char)*Size2);	
	Pos += Size2;

	//15-16
	data=CombineLedItem2(rTable.sPWM15, rTable.sPWM16);	
	sData=IntToHexStr(data, Size2);
	::memcpy(&rStr[Pos], sData.c_str(), sizeof(char)*Size2);	
	Pos += Size2;
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Table2String_CameraEnable(const TLCB_TRIG_TABLE &rTable, std::string &rStr)//Camera Enable
{	//[57~58]
	const int Cnt=2;
	const int Pos=ARDUINO_STR_POS_CCD_ENABLE_CHN;
	std::string sTime=IntToHexStr(rTable.sCameraEnable, Cnt);			
	const size_t Len=sTime.length();
	const size_t LenUse=MIN(Len, Cnt);
	::memcpy(&rStr[Pos], sTime.c_str(), sizeof(char)*LenUse);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Table2String_DlpPulseTime(const TLCB_TRIG_TABLE &rTable, std::string &rStr)//DLP Pulse Time
{	//[59~62]	
	const int Cnt=4;
	const int Pos=ARDUINO_STR_POS_DLP_PULSE_TIME;
	std::string sTime=IntToHexStr(rTable.sDLPPulseTime, Cnt);		
	const size_t Len=sTime.length();
	const size_t LenUse=MIN(Len, Cnt);
	::memcpy(&rStr[Pos], sTime.c_str(), sizeof(char)*LenUse);	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Table2String_DlpCallbackTime(const TLCB_TRIG_TABLE &rTable, std::string &rStr)//DLP Callback Time
{	//[63~66]	
	const int Cnt=4;
	const int Pos=ARDUINO_STR_POS_DLP_CALLBACK_TIME;
	std::string sTime=IntToHexStr(rTable.sDLPCallbackTime, Cnt);	
	const size_t Len=sTime.length();
	const size_t LenUse=MIN(Len, Cnt);
	::memcpy(&rStr[Pos], sTime.c_str(), sizeof(char)*LenUse);	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Table2String_DlpExposureTime(const TLCB_TRIG_TABLE &rTable, std::string &rStr)//DLP Exposure Time
{	//[67~70]	
	const int Cnt=4;
	const int Pos=ARDUINO_STR_POS_DLP_EXPOSURE_TIME;	
	std::string sTime=IntToHexStr(rTable.sDLPCCDExpTime, Cnt);
	const size_t Len=sTime.length();
	const size_t LenUse=MIN(Len, Cnt);
	::memcpy(&rStr[Pos], sTime.c_str(), sizeof(char)*LenUse);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Table2String_DlpReadyTime(const TLCB_TRIG_TABLE &rTable, std::string &rStr)//DLP Wait Ready Time
{	//[71~78]	
	const int Cnt=8;
	const int Pos=ARDUINO_STR_POS_DLP_READY_TIME;
	std::string sTime=IntToHexStr(rTable.sDLPReadySignalDelayTime, Cnt);	
	const size_t Len=sTime.length();
	const size_t LenUse=MIN(Len, Cnt);
	::memcpy(&rStr[Pos], sTime.c_str(), sizeof(char)*LenUse);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::String2Table(const std::string &rStr, TLCB_TRIG_TABLE &rTable)//字串轉表格
{
	if ( CheckStringValid(rStr) == false ) { return false; }
	if ( String2Table_Type(rStr, rTable) == false ) { return false; }
	if ( String2Table_DlpTrgCnt(rStr, rTable) == false ) { return false; }
	if ( String2Table_DlpChannel(rStr, rTable) == false ) { return false; }
	if ( String2Table_DlpEnbRdy(rStr, rTable) == false ) { return false; }
	if ( String2Table_NextTableTime(rStr, rTable) == false ) { return false; }
	if ( String2Table_LedOnTime(rStr, rTable) == false ) { return false; }
	if ( String2Table_CcdDelayTime(rStr, rTable) == false ) { return false; }
	if ( String2Table_LedChPwr(rStr, rTable) == false ) { return false; }
	if ( String2Table_CameraEnable(rStr, rTable) == false ) { return false; }
	if ( String2Table_DlpPulseTime(rStr, rTable) == false ) { return false; }	
	if ( String2Table_DlpCallbackTime(rStr, rTable) == false ) { return false; }
	if ( String2Table_DlpExposureTime(rStr, rTable) == false ) { return false; }
	if ( String2Table_DlpReadyTime(rStr, rTable) == false ) { return false; }		
	//return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::String2Table"));
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::String2Table_Type(const std::string &rStr, TLCB_TRIG_TABLE &rTable)//D:DLP, Others:LED
{	//[1]
	const int Pos=ARDUINO_STR_POS_TABLE_TYPE;	
	if ( CheckStringLen(rStr, Pos) == false ) { return false; }
	if ( 'D' == rStr[Pos] )
	{	rTable.sTableType = TABLE_TYPE_DLP; }
	else
	{	rTable.sTableType = TABLE_TYPE_LED; }	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::String2Table_DlpTrgCnt(const std::string &rStr, TLCB_TRIG_TABLE &rTable)//DLP Trigger Count:[1]
{	//[2]
	std::string Temp;
	const int Pos=ARDUINO_STR_POS_DLP_TRG_CNT;	
	if ( CopySubString(rStr, Pos, 1, Temp) == false ) { return false; }	
	rTable.sDLPTrigOutNumber = HexStrToInt(Temp);//16進位制字串轉成整數
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::String2Table_DlpChannel(const std::string &rStr, TLCB_TRIG_TABLE &rTable)//DLP Channel:[1~8]
{	//[3]
	std::string Temp;
	const int Pos=ARDUINO_STR_POS_DLP_CHANNEL;
	if ( CopySubString(rStr, Pos, 1, Temp) == false ) { return false; }	
	rTable.sDLPActiveChannel = HexStrToInt(Temp);//16進位制字串轉成整數	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::String2Table_DlpEnbRdy(const std::string &rStr, TLCB_TRIG_TABLE &rTable)//DLP Ready Mode[1,0];
{	//[4]
	std::string Temp;
	const int Pos=ARDUINO_STR_POS_DLP_ENABLE_RDY;	
	if ( CopySubString(rStr, Pos, 1, Temp) == false ) { return false; }	
	rTable.sDLPReadySignalEnable = HexStrToInt(Temp);//16進位制字串轉成整數	
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::String2Table_NextTableTime(const std::string &rStr, TLCB_TRIG_TABLE &rTable)//Table Between Time
{	//[5~12]
	std::string Temp;
	const int Pos=ARDUINO_STR_POS_NEXT_TABLE_TIME;	
	if ( CopySubString(rStr, Pos, 8, Temp) == false ) { return false; }	
	rTable.sNextTableTime = HexStrToInt(Temp);//16進位制字串轉成整數		
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::String2Table_LedOnTime(const std::string &rStr, TLCB_TRIG_TABLE &rTable)//LED Turn On Time
{	//[13~20]
	std::string Temp;
	const int Pos=ARDUINO_STR_POS_LED_ON_TIME;
	if ( CopySubString(rStr, Pos, 8, Temp) == false ) { return false; }	
	rTable.sLEDTableTotalTime = HexStrToInt(Temp);//16進位制字串轉成整數			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::String2Table_CcdDelayTime(const std::string &rStr, TLCB_TRIG_TABLE &rTable)//CCD Delay Time
{	//[21~24]
	std::string Temp;
	const int Pos=ARDUINO_STR_POS_CCD_DELAY_TIME;
	if ( CopySubString(rStr, Pos, 4, Temp) == false ) { return false; }	
	rTable.sCCDDelayTime = HexStrToInt(Temp);//16進位制字串轉成整數			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::String2Table_LedChPwr(const std::string &rStr, TLCB_TRIG_TABLE &rTable)//LED Channel Power
{	//[25~26], [27~28], ... [55~56]//Total 16 channels
	//Parser2LedItemString
	int Pos = 0;
	std::string Temp;
	const int Size=2;
	const int Size2=Size*2;
	const int Start=ARDUINO_STR_POS_LED_CHANNEL_PWR;
	
	Pos = Start;
	//01-02
	if ( CopySubString(rStr, Pos, Size2, Temp) == false ) { return false; }	
	if ( Parser2LedItemString(Temp, rTable.sPWM1, rTable.sPWM2) == false ) { return false; }
	Pos += Size2;

	//03-04
	if ( CopySubString(rStr, Pos, Size2, Temp) == false ) { return false; }	
	if ( Parser2LedItemString(Temp, rTable.sPWM3, rTable.sPWM4) == false ) { return false; }
	Pos += Size2;

	//05-06
	if ( CopySubString(rStr, Pos, Size2, Temp) == false ) { return false; }	
	if ( Parser2LedItemString(Temp, rTable.sPWM5, rTable.sPWM6) == false ) { return false; }
	Pos += Size2;

	//07-08
	if ( CopySubString(rStr, Pos, Size2, Temp) == false ) { return false; }	
	if ( Parser2LedItemString(Temp, rTable.sPWM7, rTable.sPWM8) == false ) { return false; }
	Pos += Size2;

	//09-10
	if ( CopySubString(rStr, Pos, Size2, Temp) == false ) { return false; }	
	if ( Parser2LedItemString(Temp, rTable.sPWM9, rTable.sPWM10) == false ) { return false; }
	Pos += Size2;

	//11-12
	if ( CopySubString(rStr, Pos, Size2, Temp) == false ) { return false; }	
	if ( Parser2LedItemString(Temp, rTable.sPWM11, rTable.sPWM12) == false ) { return false; }
	Pos += Size2;

	//13-14
	if ( CopySubString(rStr, Pos, Size2, Temp) == false ) { return false; }	
	if ( Parser2LedItemString(Temp, rTable.sPWM13, rTable.sPWM14) == false ) { return false; }
	Pos += Size2;

	//15-16
	if ( CopySubString(rStr, Pos, Size2, Temp) == false ) { return false; }	
	if ( Parser2LedItemString(Temp, rTable.sPWM15, rTable.sPWM16) == false ) { return false; }
	Pos += Size2;
	return true;
	//return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::String2Table_LedChPwr"));
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::String2Table_CameraEnable(const std::string &rStr, TLCB_TRIG_TABLE &rTable)//Camera Enable
{	//[57~58]
	std::string Temp;
	const int Pos=ARDUINO_STR_POS_CCD_ENABLE_CHN;
	if ( CopySubString(rStr, Pos, 2, Temp) == false ) { return false; }	
	rTable.sCameraEnable = HexStrToInt(Temp);//16進位制字串轉成整數			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::String2Table_DlpPulseTime(const std::string &rStr, TLCB_TRIG_TABLE &rTable)//DLP Pulse Time
{	//[59~62]	
	std::string Temp;
	const int Pos=ARDUINO_STR_POS_DLP_PULSE_TIME;	
	if ( CopySubString(rStr, Pos, 4, Temp) == false ) { return false; }	
	rTable.sDLPPulseTime = HexStrToInt(Temp);//16進位制字串轉成整數			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::String2Table_DlpCallbackTime(const std::string &rStr, TLCB_TRIG_TABLE &rTable)//DLP Callback Time
{	//[63~66]	
	std::string Temp;
	const int Pos=ARDUINO_STR_POS_DLP_CALLBACK_TIME;
	if ( CopySubString(rStr, Pos, 4, Temp) == false ) { return false; }	
	rTable.sDLPCallbackTime = HexStrToInt(Temp);//16進位制字串轉成整數			
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::String2Table_DlpExposureTime(const std::string &rStr, TLCB_TRIG_TABLE &rTable)//DLP Exposure Time
{	//[67~70]	
	std::string Temp;
	const int Pos=ARDUINO_STR_POS_DLP_EXPOSURE_TIME;	
	if ( CopySubString(rStr, Pos, 4, Temp) == false ) { return false; }	
	rTable.sDLPCCDExpTime = HexStrToInt(Temp);//16進位制字串轉成整數
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::String2Table_DlpReadyTime(const std::string &rStr, TLCB_TRIG_TABLE &rTable)//DLP Wait Ready Time
{	//[71~78]	
	std::string Temp;
	const int Pos=ARDUINO_STR_POS_DLP_READY_TIME;
	if ( CopySubString(rStr, Pos, 8, Temp) == false ) { return false; }	
	rTable.sDLPReadySignalDelayTime = HexStrToInt(Temp);//16進位制字串轉成整數
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Connect()
{
#ifndef LIGHT_CTRL_DISABLE
	const int Port=GetIPPort();
	const char *IP=GetIPAddress();	
	return Connect(IP, Port);
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::Connect(const char *IPAddress, int IPPort)//連接到控制板-網址
{
#ifndef LIGHT_CTRL_DISABLE	
	const bool bThread=false;
	if ( m_Socket.Connect(IPAddress, IPPort, bThread) == false )
	{		
		CString err=GetSocketErrorString();
		m_ErrorString.Format(_T("%s [Connect:%s]"), err, CString(IPAddress));
		return false;
	}		

	SetTableRunCount(0);
	m_ConveyerDirection = 0;
	m_ConveyerSpeedSlow = false;
	m_FPGAVersion==_T("Arduion");	
	ResetDLPNotReadySignal();
	ReadFPGAVersion(m_FullVersion);
	ReadBoardAllChannel();
	//m_FullVersion.Format(_T("%s%s (Version:%s)"), strName, strCode, m_FPGAVersion);		
	//m_FullVersion.Format(_T("%s#%s%s"), m_FPGAVersion, strName, strCode);	
	UpdateSupportFuncByVersion();
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::GetIsConnected()
{		
#ifndef LIGHT_CTRL_DISABLE		
	return m_Socket.GetConnected();	
#endif//LIGHT_CTRL_DISABLE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::CheckIsConnected()
{
	if ( GetIsConnected() == false )
	{
		m_ErrorString=_T("Error, Not Connected");
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::DisConnect()
{
#ifndef LIGHT_CTRL_DISABLE	
	ResetSubBoardLedChannelValid();
	if ( m_Socket.GetConnected() == true )
	{	m_Socket.CloseSocket(); }	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardImpArduino::SetIPPort(int val)
{
	m_IPPort = val;
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImpArduino::GetIPPort() const
{
	return m_IPPort;
}
//-------------------------------------------------------------------------------------//
void CLightCtrlBoardImpArduino::SetIPAddress(const char *str)
{
	m_IPAdress = str;
}
//-------------------------------------------------------------------------------------//
const char* CLightCtrlBoardImpArduino::GetIPAddress() const
{
	return m_IPAdress.c_str();
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImpArduino::GetCameraCount() const//取得相機數量
{
	return m_CameraCount;	
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImpArduino::GetDLPCastCount() const//取得DLP投光數量
{
	return m_DLPCastCount;	
}
//-------------------------------------------------------------------------------------//
int CLightCtrlBoardImpArduino::GetLEDChannelCount() const
{
	return m_LEDChannelCount;	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::UpdateLightCtrlBoardType()//更新控制板USB命令文字
{
	if ( CLightCtrlBoardImp::UpdateLightCtrlBoardType() == false )
	{	return false; }
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::LoopBackTestAll()
{	
#ifndef LIGHT_CTRL_DISABLE
	return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::LoopBackTestAll"));
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::LoopBackTest(int data, char DataWS[], char DataRA[], char DataRB[])
{
#ifndef LIGHT_CTRL_DISABLE
	return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::LoopBackTest"));	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::LoopBackTestA(int data, char DataWS[], char DataR[])
{
#ifndef LIGHT_CTRL_DISABLE
	return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::LoopBackTestA"));	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::LoopBackTestB(int data, char DataWS[], char DataR[])
{
#ifndef LIGHT_CTRL_DISABLE
	return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::LoopBackTestB"));	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ReadLightCtrlBorad(const std::string &Address, std::string &Data)
{
	return ReadBoardData(Address, Data);	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::WriteLightCtrlBorad(const std::string &Address, const std::string &Data)
{	
	return WriteBoardData(Address, Data);
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SaveLightCtrlBoardINI()//寫參數至檔案
{
	int     i=0;
	bool    IsOK = true;
	CString FileName;
	CString KeyName;
	CString KeyString;
	CString Section;	
	const LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType = GetLightCtrlBoardType();

	if ( CLightCtrlBoardImp::SaveLightCtrlBoardINI() == false )
	{	return false; }
	
	FileName = GetLCBIniFileName();
	Section = GetLCBIniSectionName(LightCtrlBoardType);	

	//網路埠
	KeyName.Format(_T("IP Port")); KeyString.Format(_T("%d"), m_IPPort);
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	//網址
	KeyName.Format(_T("IP Address")); KeyString.Format(_T("%s"), CString(m_IPAdress.c_str()));	
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }

	//網路逾時
	KeyName.Format(_T("Socket Timeout")); KeyString.Format(_T("%d"), m_SocketTimeout);	
	if ( SaveINIData(Section, KeyName, KeyString, FileName, m_ErrorString) == false )
	{	IsOK = false; }
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::LoadLightCtrlBoardINI()//從檔案讀取參數
{
	int     i=0;
	int     nValue=0;	
	CString FileName;
	CString TempStr;
	CString KeyName;
	CString KeyString;
	CString Section;		
	const size_t StringSize = 128;
	TCHAR ReturnString[StringSize];
	LIGHT_CTRL_BOARD_TYPE LightCtrlBoardType;	

	if ( CLightCtrlBoardImp::LoadLightCtrlBoardINI() == false )
	{	return false; }

	FileName = GetLCBIniFileName();
	LightCtrlBoardType = GetLightCtrlBoardType();//讀完INI檔案會變更控制板版本
	Section = GetLCBIniSectionName(LightCtrlBoardType);		

	//網路埠
	KeyName.Format(_T("IP Port")); KeyString.Format(_T("%d"), m_IPPort);
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	
		nValue = ::_ttoi(ReturnString); 		
		if ( nValue >= 0 ) 
		{	m_IPPort = nValue; }
	}

	//網址	
	KeyName.Format(_T("IP Address")); KeyString.Format(_T("%s"), CString(m_IPAdress.c_str()));	
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{	JetAPI::TCHAR2string(ReturnString, m_IPAdress);	}

	//網路逾時
	KeyName.Format(_T("Socket Timeout")); KeyString.Format(_T("%d"), m_SocketTimeout);	
	if ( LoadINIData(Section, KeyName, KeyString, ReturnString, StringSize, FileName, true, this->m_ErrorString) == true )	
	{
		nValue = ::_ttoi(ReturnString); 		
		if ( nValue >= 0 ) 
		{	m_SocketTimeout = nValue; }		
	}

	UpdateLightCtrlBoardType();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ReadRAMData(char data[])
{
#ifndef LIGHT_CTRL_DISABLE
	return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::ReadRAMData"));
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ReadRAMAddress(char data[])
{
#ifndef LIGHT_CTRL_DISABLE
	return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::ReadRAMAddress"));	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::CheckLightCtrlBoardError(bool &IsError, CString &ErrorStr)
{	//return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::CheckLightCtrlBoardError"));
#ifndef LIGHT_CTRL_DISABLE
	std::string Str;
	ResetDLPNotReadySignal();
	if ( ReadErrorString(Str) == false )
	{	return false; }			
	
	bool bErr=false;
	std::string SubStr;
	std::string AllErr;	
	size_t PosE=0, SubLen=0;
	const size_t StrLen=Str.length();
	if ( 0 == StrLen ) { return true; }
	if ( DecodeBoardError(Str, AllErr, bErr, PosE) == false )
	{	return false; }
	if ( true == bErr ) 
	{	IsError = true; }

	PosE ++;
	if ( DecodeOtherError(Str, PosE, SubStr, bErr) == false )
	{	return false;	}
	if ( true == bErr ) 
	{	
		IsError = true; 
		DecodeDLPNotReadySignal(SubStr);
		if ( 0 == AllErr.length() )
		{	AllErr = SubStr; }
		else
		{
			std::string TmpStr=AllErr;
			AllErr=TmpStr+std::string("\n")+SubStr;
		}	
	}
	ErrorStr=CString(AllErr.c_str());
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ReadErrorString(CString &Error)
{
	Error = _T("");
#ifndef LIGHT_CTRL_DISABLE
	std::string Err;
	if ( ReadErrorString(Err) == false )
	{	return false; }
	const size_t subLen=Err.length();
	if ( 0 == subLen )
	{	Error = _T("OK");	}
	else
	{	Error=CString(Err.c_str());	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ReadErrorString(std::string &Err)
{
	Err = "";
#ifndef LIGHT_CTRL_DISABLE		
	std::string result;
	std::string subStr;
	if ( ReadBoardData("readerr", result) == false )
	{	return false; }
	if ( UnpacketCommandString(result, subStr) == false )
	{	return false;	}	
	Err = subStr;	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::DecodeErrorCodeText(int ErrorCode, CString &ErrorStr)
{
	return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::DecodeErrorCodeText"));
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::GetTriggerCountText(CString &TrigCount)
{	
#ifndef LIGHT_CTRL_DISABLE
	UINT cntPC_FPGA=0;
	UINT cntFPG_CCD=0;	
	if ( ReadBoardAllCount() == false ) { return false; }
	if( GetPCtoFPGATrigCount(cntPC_FPGA) == false ) { return false; }	
	if( GetFPGAtoCCDTrigCount(cntFPG_CCD) == false ) { return false; }			
#ifndef PHASE_CTRL_DISABLE
	UINT cntFPG_DLP1=0, cntFPG_DLP2=0, cntFPG_DLP3=0, cntFPG_DLP4=0;
	UINT cntFPG_DLP5=0, cntFPG_DLP6=0, cntFPG_DLP7=0, cntFPG_DLP8=0;
	UINT cntDLP1_FPG=0, cntDLP2_FPG=0, cntDLP3_FPG=0, cntDLP4_FPG=0;
	UINT cntDLP5_FPG=0, cntDLP6_FPG=0, cntDLP7_FPG=0, cntDLP8_FPG=0;
	if( GetFPGAtoDLPTrigCount(DLP_CHANNEL_1, cntFPG_DLP1) == false ) { return false; }
	if( GetFPGAtoDLPTrigCount(DLP_CHANNEL_2, cntFPG_DLP2) == false ) { return false; }
	if( GetFPGAtoDLPTrigCount(DLP_CHANNEL_3, cntFPG_DLP3) == false ) { return false; }
	if( GetFPGAtoDLPTrigCount(DLP_CHANNEL_4, cntFPG_DLP4) == false ) { return false; }
	if( GetFPGAtoDLPTrigCount(DLP_CHANNEL_5, cntFPG_DLP5) == false ) { return false; }
	if( GetFPGAtoDLPTrigCount(DLP_CHANNEL_6, cntFPG_DLP6) == false ) { return false; }
	if( GetFPGAtoDLPTrigCount(DLP_CHANNEL_7, cntFPG_DLP7) == false ) { return false; }
	if( GetFPGAtoDLPTrigCount(DLP_CHANNEL_8, cntFPG_DLP8) == false ) { return false; }

	if( GetDLPtoFPGATrigCount(DLP_CHANNEL_1, cntDLP1_FPG) == false ) { return false; }
	if( GetDLPtoFPGATrigCount(DLP_CHANNEL_2, cntDLP2_FPG) == false ) { return false; }
	if( GetDLPtoFPGATrigCount(DLP_CHANNEL_3, cntDLP3_FPG) == false ) { return false; }
	if( GetDLPtoFPGATrigCount(DLP_CHANNEL_4, cntDLP4_FPG) == false ) { return false; }
	if( GetDLPtoFPGATrigCount(DLP_CHANNEL_5, cntDLP5_FPG) == false ) { return false; }
	if( GetDLPtoFPGATrigCount(DLP_CHANNEL_6, cntDLP6_FPG) == false ) { return false; }
	if( GetDLPtoFPGATrigCount(DLP_CHANNEL_7, cntDLP7_FPG) == false ) { return false; }
	if( GetDLPtoFPGATrigCount(DLP_CHANNEL_8, cntDLP8_FPG) == false ) { return false; }	

	TrigCount.Format(_T("PC_FPGA(%d), FPGA_CCD(%d)\nFPGA_DLP(%d, %d, %d, %d, %d, %d, %d, %d)\nDLP_FPGA(%d, %d, %d, %d, %d, %d, %d, %d)"), 
		cntPC_FPGA, cntFPG_CCD, 
		cntFPG_DLP1, cntFPG_DLP2, cntFPG_DLP3, cntFPG_DLP4, cntFPG_DLP5, cntFPG_DLP6, cntFPG_DLP7, cntFPG_DLP8, 
		cntDLP1_FPG, cntDLP2_FPG, cntDLP3_FPG, cntDLP4_FPG, cntDLP5_FPG, cntDLP6_FPG, cntDLP7_FPG, cntDLP8_FPG); 
#else
	TrigCount.Format(_T("PC_FPGA(%d), FPGA_CCD(%d)"), cntPC_FPGA, cntFPG_CCD); 
#endif//PHASE_CTRL_DISABLE
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ReadFPGAVersion(CString &Version)
{	
#ifndef LIGHT_CTRL_DISABLE
	std::string Result;
	if ( ReadBoardData("version", Result) == false )
	{	return false; }
	Version=CString(Result.c_str());	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::GetIsWorkingNow(bool &Working)
{
#ifndef LIGHT_CTRL_DISABLE
	Working=false;	
	std::string Str;	
	if ( ReadErrorString(Str) == false )
	{	return false; }		

	bool bErr=false;
	std::string SubStr;	
	size_t PosE=0, SubLen=0;
	const size_t StrLen=Str.length();
	if ( 0 == StrLen ) { return true; }
	if ( DecodeBoardError(Str, SubStr, bErr, PosE) == false )
	{	return false; }
	
	PosE ++;
	if ( DecodeOtherError(Str, PosE, SubStr, bErr) == false )
	{	return false;	}
	if ( true == bErr ) 
	{	Working = false;	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::GetIsFPGAOK(bool &IsOK)
{
#ifndef LIGHT_CTRL_DISABLE
	IsOK = true;
	std::string Str;	
	if ( ReadErrorString(Str) == false )
	{	return false; }		

	bool bErr=false;
	std::string SubStr;	
	size_t PosE=0, SubLen=0;
	const size_t StrLen=Str.length();
	if ( 0 == StrLen ) { return true; }
	if ( DecodeBoardError(Str, SubStr, bErr, PosE) == false )
	{	return false; }
	if ( true == bErr ) 
	{	IsOK = false; }
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SetDLPEnable(UINT DLPChannel)
{
	//等同於EnableDLPChannel	
	return true;
#ifndef LIGHT_CTRL_DISABLE
	return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::SetDLPEnable"));	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ReadBoardAllCount()
{
#ifndef LIGHT_CTRL_DISABLE
	std::string strAllCount;
	const bool bWaitAck=false;
	ClearAllCountValue();
	if ( ReadBoardData("getcoun", strAllCount) == false )	
	{	return false; }

	int i=0;
	m_PCtoFPGATrigCount=ParserStringCount(ARDUINO_PARSER_COUNT_PC_FPGA, 0, strAllCount);//PC送給控制板觸發次數
	for ( i=0; i<ARDUINO_MAX_CCD_COUNT; i++ )//控制板送給相機觸發次數
	{	m_FPGAtoCCDTrigCount[i]=ParserStringCount(ARDUINO_PARSER_COUNT_FPGA_CCD, i, strAllCount);	}
	for ( i=0; i<ARDUINO_MAX_DLP_COUNT; i++ )//控制板送給DLP觸發次數
	{	m_FPGAtoDLPTrigCount[i]=ParserStringCount(ARDUINO_PARSER_COUNT_FPGA_DLP, i, strAllCount);	}
	for ( i=0; i<ARDUINO_MAX_DLP_COUNT; i++ )//DLP送給控制板觸發次數
	{	m_DLPtoFPGATrigCount[i]=ParserStringCount(ARDUINO_PARSER_COUNT_DLP_FPGA, i, strAllCount);	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::GetPCtoFPGATrigCount(UINT &count)
{
#ifndef LIGHT_CTRL_DISABLE
	count=m_PCtoFPGATrigCount;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::GetFPGAtoCCDTrigCount(UINT &count)
{
#ifndef LIGHT_CTRL_DISABLE
	count=m_FPGAtoCCDTrigCount[0];	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::GetFPGAtoCCDTrigCount(UINT CameraID, UINT &count)
{
#ifndef LIGHT_CTRL_DISABLE
	int idx=0;
	switch ( CameraID )
	{
	case CAMERA_ID_1:	idx=0;	break;
	case CAMERA_ID_2:	idx=1;	break;
	case CAMERA_ID_3:	idx=2;	break;
	case CAMERA_ID_4:	idx=3;	break;
	case CAMERA_ID_5:	idx=4;	break;
	case CAMERA_ID_6:	idx=5;	break;
	case CAMERA_ID_7:	idx=6;	break;
	case CAMERA_ID_8:	idx=7;	break;
	default:
		m_ErrorString.Format(_T("Error, CCD ID Error!(%d)"), CameraID); 
		return false;
		break;
	}
	count = m_FPGAtoCCDTrigCount[idx];	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::GetFPGAtoDLPTrigCount(UINT DLPChannel, UINT &count)
{
#ifndef LIGHT_CTRL_DISABLE
	int idx=0;
	switch ( DLPChannel )
	{
	case DLP_CHANNEL_1: idx=0; break;
	case DLP_CHANNEL_2: idx=1; break;
	case DLP_CHANNEL_3: idx=2; break;
	case DLP_CHANNEL_4: idx=3; break;
	case DLP_CHANNEL_5: idx=4; break;
	case DLP_CHANNEL_6: idx=5; break;
	case DLP_CHANNEL_7: idx=6; break;
	case DLP_CHANNEL_8: idx=7; break;
	default:	
		m_ErrorString.Format(_T("Error, DLP CH Error!(%d)"), DLPChannel); 
		return false;
	}
	count = m_FPGAtoDLPTrigCount[idx];	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::GetDLPtoFPGATrigCount(UINT DLPChannel, UINT &count)
{
#ifndef LIGHT_CTRL_DISABLE
	int idx=0;
	switch ( DLPChannel )
	{
	case DLP_CHANNEL_1: idx=0; break;
	case DLP_CHANNEL_2: idx=1; break;
	case DLP_CHANNEL_3: idx=2; break;
	case DLP_CHANNEL_4: idx=3; break;
	case DLP_CHANNEL_5: idx=4; break;
	case DLP_CHANNEL_6: idx=5; break;
	case DLP_CHANNEL_7: idx=6; break;
	case DLP_CHANNEL_8: idx=7; break;
	default:	
		m_ErrorString.Format(_T("Error, DLP CH Error!(%d)"), DLPChannel); 
		return false;
	}
	count = m_DLPtoFPGATrigCount[idx];
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::TriggerStart()//開始觸發
{
#ifndef LIGHT_CTRL_DISABLE
	int   TableCount=0;	
	const int RunCount=GetTableRunCount();	
	const int TrgIndex=GetTriggerTableIndex();	
	if ( ReadBoardTableCount(TableCount) == false )
	{	return false;	}

	std::string Cmd;
	if ( 0!=TrgIndex || RunCount!=TableCount )
	{
		int PosS=TrgIndex;
		int PosE=PosS+RunCount-1;
		if ( PosE >= TableCount )
		{	
			m_ErrorString.Format(_T("Error, Trigger Table Range Exceptoin (%d ~ %d)"), PosS, PosE);
			return false;
		}
		
		char sStr[64]="";
		::sprintf(sStr, "tgrfrom%.02X%.02X", PosS, PosE);		
		Cmd=sStr;
	}
	else
	{	Cmd = "tgrall";	}
	SetTriggerTableIndex(0);
	SetTriggerTableCount(TableCount);
	if ( ExecBoardCmd(Cmd) == false )	
	{	return false; }	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ClearAllCount()//清除所有數量
{
#ifndef LIGHT_CTRL_DISABLE	
	ClearAllCountValue();
	if ( ExecBoardCmd("clrcoun") == false )	
	{	return false; }	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ClearAll()//清除全部資料
{
#ifndef LIGHT_CTRL_DISABLE	
	//if ( ExecBoardCmd("clrcoun") == false )	
	if ( ExecBoardCmd("clrerr") == false )	
	{	return false; }	
	ResetDLPNotReadySignal();
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SwitchToFPGA()
{
#ifndef LIGHT_CTRL_DISABLE
	if( FPGA_MODE_FPGA == m_FPGAMode ) { return true; }
	if( SetMode_FPGA() == false ) 
	{	return false; }
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SwitchToRead()
{
#ifndef LIGHT_CTRL_DISABLE
	if( FPGA_MODE_READ == m_FPGAMode ) { return true; }
	if( SetMode_PCRead() == false ) 
	{	return false; }
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SwitchToWrite()
{
#ifndef LIGHT_CTRL_DISABLE
	if( FPGA_MODE_WRITE == m_FPGAMode ) { return true; }
	if( SetMode_PCWrite() == false ) 
	{	return false; }
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SwitchToAssign()
{
#ifndef LIGHT_CTRL_DISABLE
	if( FPGA_MODE_ASSIGN == m_FPGAMode ) { return true; }
	if( SetMode_PCAssign() == false ) 
	{	return false; }
	return true;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SetMode_FPGA()//FPGA Run
{
#ifndef LIGHT_CTRL_DISABLE
	//return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::SetMode_FPGA"));	
	m_FPGAMode = FPGA_MODE_FPGA;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SetMode_PCWrite()//寫入
{
#ifndef LIGHT_CTRL_DISABLE
	//return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::SetMode_PCWrite"));		
	m_FPGAMode = FPGA_MODE_WRITE;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SetMode_PCRead()//讀取
{
#ifndef LIGHT_CTRL_DISABLE
	//return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::SetMode_PCRead"));
	m_FPGAMode = FPGA_MODE_READ;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SetMode_PCAssign()//指定位址
{
#ifndef LIGHT_CTRL_DISABLE
	//return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::SetMode_PCAssign"));	
	m_FPGAMode = FPGA_MODE_ASSIGN;
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::WriteTableRunCount(int nTable)//Trigger後要跑幾張TABLE
{	//return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::WriteTableRunCount"));		
#ifndef LIGHT_CTRL_DISABLE	
	SetTableRunCount(nTable);
	//SetTriggerTableCount(nTable);	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ReadTableRunCount(int &nTable)
{
#ifndef LIGHT_CTRL_DISABLE
	nTable = GetTableRunCount();	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SetCCDTrigEdge(int TrigEdge)//CCD Trigger Edge
{
#ifndef LIGHT_CTRL_DISABLE	
	std::string str;	
	switch ( TrigEdge )
	{
	case CCD_TRIG_EDGE_L: str="camtrix0"; break;
	case CCD_TRIG_EDGE_H: str="camtrix1"; break;
	default:
		m_ErrorString.Format(_T("Error, CCD Trigger Edge Fault[%d]"), TrigEdge);
		break;
	}	
	if ( ExecBoardCmd(str) == false ) 
	{	return false; }
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::EnableDLPChannel(int DLPMask)//新增加的DLP開啟功能
{	//return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::EnableDLPChannel"));	
#ifndef LIGHT_CTRL_DISABLE
	//backint		
	std::string sData=IntToHexStr(DLPMask, 2);
	std::string sCmd=std::string("backint")+sData;	
	if ( ExecBoardCmd(sCmd) == false )
	{	return false; }		
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::GetCCDTrigEdge(int &TrigEdge)
{
#ifndef LIGHT_CTRL_DISABLE	
	return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::GetCCDTrigEdge"));
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::SetTriggerFirstIndex(UINT Index)//處發表格第1張引數
{	//return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::SetTriggerFirstIndex"));	
#ifndef LIGHT_CTRL_DISABLE
	const int MaxTableCount=GetTableMaxCount();
	if ( Index<0 || Index>=MaxTableCount )
	{	return false; }
	SetTriggerTableIndex(Index);	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::TableRAMAssign(UINT AssignAddress)//指定RAM位址
{
#ifndef LIGHT_CTRL_DISABLE
	return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::TableRAMAssign"));		
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::TableAssign(UINT TableIndex)//指定RAM位址至某個Table的起始位址
{
#ifndef LIGHT_CTRL_DISABLE
	return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::TableAssign"));
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::TableRAMAddressAdd()//RAM位址加1
{
#ifndef LIGHT_CTRL_DISABLE
	return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::TableRAMAddressAdd"));	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::TableSingleWrite(TLCB_TRIG_TABLE &Table)//寫入一個Table
{	//return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::TableSingleWrite"));	
#ifndef LIGHT_CTRL_DISABLE	
	const int MaxTableCount=GetTableMaxCount();	
	//寫入一個Table	
	if( Table.sTableID<0 || Table.sTableID>=MaxTableCount )
	{
		this->m_ErrorString.Format(_T("Error, Table ID Error!(%d)"), Table.sTableID);
		return false; 
	}	
	std::string strTable;
	if ( Table2String(Table, strTable) == false )	
	{	return false; }
	AddAllTableStringW(strTable);
	if ( WriteBoardAllTable() == false )
	{	return false; }
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::TableSingleRead(TLCB_TRIG_TABLE &Table)//讀出一個Table
{	//return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::TableSingleRead"));
#ifndef LIGHT_CTRL_DISABLE
	const int TableCount=GetLCBTableCount();
	if ( Table.sTableID >= TableCount )
	{		
		if ( ReadBoardAllTable(m_LCBTableList) == false )
		{	return false; }
	}
	const int MaxTableCount=GetLCBTableCount();
	//讀出一個Table	
	if( Table.sTableID<0 || Table.sTableID>=MaxTableCount )
	{
		this->m_ErrorString.Format(_T("Error, Table ID Error!(%d)"), Table.sTableID);
		return false; 
	}	
	Table = m_LCBTableList[Table.sTableID];
#endif//LIGHT_CTRL_DISABLE
	return true;	
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::TableListRead(std::vector<TLCB_TRIG_TABLE> &TableList, int nTable)
{	//return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::TableListRead"));		
	TableList.clear();
#ifndef LIGHT_CTRL_DISABLE
	int             i=0;
	CString         str;	
	TLCB_TRIG_TABLE tmpTable;
	std::vector<TLCB_TRIG_TABLE> tTableList;

	ClearLCBTableList();
	if ( ReadBoardAllTable(tTableList) == false ) { return false; }	
	const int TableCount=MIN(tTableList.size(), nTable);
	TableList=std::vector<TLCB_TRIG_TABLE>(tTableList.begin(), tTableList.begin()+TableCount);	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::TableListWrite(const std::vector<TLCB_TRIG_TABLE> &TableList)
{	//return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::TableListWrite"));	
#ifndef LIGHT_CTRL_DISABLE	
	const int PreCheckTableList = GetPreCheckTableListEnabled();	
	if ( FN_ENABLE == PreCheckTableList )
	{
		if ( TableListCompare(TableList) == true ) 
		{	return true; }
	}

	size_t  i=0;
	int     tmpData=0;	
	std::string strTable;	
	DWORD   DLPEnableMask=0;	
	TLCB_TRIG_TABLE tmpTable;
	TLCB_TRIG_TABLE tmpTableA;
	TLCB_TRIG_TABLE tmpTableB;
	std::vector<TLCB_TRIG_TABLE>  TableListTotal = TableList;
	const size_t nTable = TableList.size();		
	
	//將後面給予填入空的表格
	DefaultTable(tmpTable);
	const size_t MaxTableCount = GetTableMaxCount();
	for ( i=nTable; i<MaxTableCount; i++ )
	{
		tmpTable.sTableID = i;
		TableListTotal.push_back(tmpTable);
	}
	
	const size_t NTableWrite=nTable;//MaxTableCount	//只寫入要的資料
	ClearAllTableStringW();
	for ( i=0; i<NTableWrite; i++)
	{		
		tmpTable = TableListTotal[i];
		tmpTable.sTableID = i;			
		if ( Table2String(tmpTable, strTable) == false )
		{	return false; }
		AddAllTableStringW(strTable);

		if ( TABLE_TYPE_DLP == tmpTable.sTableType )
		{
			tmpData = tmpTable.sDLPActiveChannel;
			if ( tmpData>0 && tmpData<DLP_CHANNEL_COUNT )
			{	tmpData = m_DLPChannelMap[tmpData];	}
			switch ( tmpData )
			{
			case 1:	DLPEnableMask |= 0x01;	break;
			case 2:	DLPEnableMask |= 0x02;	break;
			case 3:	DLPEnableMask |= 0x04;	break;
			case 4:	DLPEnableMask |= 0x08;	break;
			case 5:	DLPEnableMask |= 0x10;	break;
			case 6:	DLPEnableMask |= 0x20;	break;
			case 7:	DLPEnableMask |= 0x40;	break;
			case 8:	DLPEnableMask |= 0x80;	break;
			}
		}
		else
		{	i = i; }
	}	
	SetTriggerTableIndex(0);
	SetTableRunCount(nTable);
	SetTriggerTableCount(nTable);
	SetLCBTableList(TableList);
	if ( WriteBoardAllTable() == false )
	{	return false; }

	//sDLPActiveChannel	
	SetDLPEnable(0);
	if ( EnableDLPChannel(DLPEnableMask) == false )
	{	return false;	}
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::TableReset(int nTable)
{
#ifndef LIGHT_CTRL_DISABLE	
	int i=0;
	std::string strTable;	
	TLCB_TRIG_TABLE tmpTable;	
	const int MaxTableCount=GetTableMaxCount();
	
	if ( nTable > MaxTableCount ) 
	{	nTable = MaxTableCount; }

	ClearAllTableStringW();
	for( i=0 ; i<nTable ; i++ )
	{
		DefaultTable(tmpTable);
		tmpTable.sTableID = i;		
		if ( Table2String(tmpTable, strTable) == false )
		{	return false; }
		AddAllTableStringW(strTable);		
	}
	if ( WriteBoardAllTable() == false )
	{	return false; }

	SetTableRunCount(0);
	ClearLCBTableList();
	ClearAllTableStringW();
	SetTriggerTableCount(0);
	SetTriggerTableIndex(0);
	EnableDLPChannel(0);
	return true;
	return ReturnNotImplement(_T("CLightCtrlBoardImpArduino::TableReset"));		
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ExecConveyorMotorStop(LANE_ID LaneID)//執行軌道停止
{
#ifndef LIGHT_CTRL_DISABLE
	return ExecConveyorMotorRunning(LaneID, false, true, false);	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//
bool CLightCtrlBoardImpArduino::ExecConveyorMotorRunning(LANE_ID LaneID, bool On, bool bPositive, bool Slow)//執行軌道運轉	
{
#ifndef LIGHT_CTRL_DISABLE
	std::string str;	
	const int Forward=1;
	const int Bakward=2;	
	if ( false == On )
	{	
		str = "stpconv";	
		if ( ExecBoardCmd(str) == false ) 
		{	return false; }
		m_ConveyerDirection = 0;
		m_ConveyerSpeedSlow = false;
	}
	else
	{		
		if ( true==Slow && false==m_ConveyerSpeedSlow )
		{	
			str = "slwdown";	
			if ( ExecBoardCmd(str) == false ) 
			{	return false; }
			m_ConveyerSpeedSlow = true;
		}		
		if ( true==bPositive && Forward!=m_ConveyerDirection )
		{	
			str = "forward";	
			if ( ExecBoardCmd(str) == false ) 
			{	return false; }
			m_ConveyerDirection = Forward;
		}
		if ( false==bPositive && Bakward!=m_ConveyerDirection )
		{	
			str = "bakward";	
			if ( ExecBoardCmd(str) == false ) 
			{	return false; }
			m_ConveyerDirection = Bakward;
		}		
	}	
#endif//LIGHT_CTRL_DISABLE
	return true;
}
//-------------------------------------------------------------------------------------//