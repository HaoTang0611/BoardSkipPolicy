#ifndef _BARCODE_UNIT_FILE_H_
#define _BARCODE_UNIT_FILE_H_

#include "barcodetypedef.h"

#define _ONLY_HONEYWELL_3310 (0)

class CBarcodeUnit
{
	class CBarcodeUnitImpl *_pImpl;

	explicit CBarcodeUnit();
public:
	~CBarcodeUnit();	

	//return barcode id
	int CreateBarcodeObject(const CBarcodeParameter &rParas);

	bool DelectBarcodeObject(int BarcodeId);

	//*set number of code each scan
	//BarcodeId: Id created by "CreateBarcodeObject"
	bool SetBarcodeMultiCode(int BarcodeId, int Count);

	//*start barcode reader
	bool TriggerBarcode(int BarcodeId);

	//stop barcode reader
	bool TurnOffBarcode(int BarcodeId);

	//*set timeout for each scan
	bool SetTriggerTimeoutMs(int BarcodeId, int TimeoutMs);

	//flag for wait data into the buffer after scan finish
	bool WaitBarcodeReading(int BarcodeId);//µ¥«Ý±ø½X¾÷Åª­È

	//get barcode data of the buffer
	bool GetBarcodeData(int BarcodeId, std::vector<std::string> &rBarcodeList);

	//clear barcode data of the buffer
	bool ClearBarcodeData(int BarcodeId);

	//get last error string
	const std::string *GetErrorString(int BarcodeId);

	//
	const std::string GetBarcodeDeviceName(BarcodeDeviceType Type);

	friend CBarcodeUnit &theBarcodeUnitCtrl();
};

CBarcodeUnit &theBarcodeUnitCtrl();

#endif