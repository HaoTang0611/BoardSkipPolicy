#ifndef _BARCODE_TYPE_DEF_H_
#define _BARCODE_TYPE_DEF_H_

#include <vector>

#define BARCODE_TYPE_NULL                    0

enum BarcodeDeviceType
{
	BARCODE_MICROSCAN_GROUP = 100,
	BARCODE_MICROSCAN_MS3,
	BARCODE_MICROSCAN_MINI,
	BARCODE_MICROSCAN_MINI_VELOCITY,
	BARCODE_MICROSCAN_MINI3,
	BARCODE_MICROSCAN_MINI_HAWK,

	BARCODE_DATALOGIC_GROUP = 200,
	BARCODE_DATALOGIC_M1000,
	BARCODE_DATALOGIC_MATRIX_200,
	BARCODE_DATALOGIC_MATRIX_210,
	BARCODE_DATALOGIC_GFS4400,

	BARCODE_HONEYWELL_GROUP = 300,
	BARCODE_HONEYWELL_3310GHD,
	BARCODE_HONEYWELL_1900,

	BARCODE_SICK_GROUP = 500,
	BARCODE_SICK_442,		
	BARCODE_SICK_ICR840,
};

//#define BARCODE_SOFTWARE                   400//軟體解碼
//#define BARCODE_HANDHELD                   800//手持式條碼機-單機型AOI使用

//----------------------------------------------------------------------------//
enum BarcodeIdSerial
{
	BIDS_ID_01,//第1個條碼
	BIDS_ID_02,//第2個條碼
	BIDS_ID_03,//第3個條碼
	BIDS_ID_04,//第4個條碼
	BIDS_TOTAL
};

class CBarcodeParameter
{
	void Swap(const CBarcodeParameter &other)
	{
		m_BarcodeType = other.m_BarcodeType;
		m_COMPort	  = other.m_COMPort;	
		m_WaitTimeoutMs = other.m_WaitTimeoutMs;
		m_BarcodeID		= other.m_BarcodeID;
		m_iniPath		= other.m_iniPath;
		m_InitializeSW	=other.m_InitializeSW;
	}

	void init()
	{
		m_BarcodeType = 0;
		m_COMPort = 0;
		m_WaitTimeoutMs = -1;
		m_BarcodeID = BIDS_ID_01;
		m_iniPath = "";
		m_InitializeSW = true;
	}

public:
	int  m_BarcodeType;		//條碼機n號的條碼機的種類
	int  m_COMPort;			//條碼機n號的條碼機的COM埠號碼		
	int  m_WaitTimeoutMs;	//條碼機n號的等待時間, -1: wait for infinite
	int  m_BarcodeID;		//條碼Id
	std::string m_iniPath;	//INI path
	bool m_InitializeSW;	//initialize by software

	CBarcodeParameter()
	{
		init();
	}

	~CBarcodeParameter()
	{
	}

	CBarcodeParameter(const CBarcodeParameter &other)
	{
		Swap(other);
	}

	CBarcodeParameter &operator=(const CBarcodeParameter &rhs)
	{
		Swap(CBarcodeParameter(rhs) );
		return *this;
	}
};


#endif