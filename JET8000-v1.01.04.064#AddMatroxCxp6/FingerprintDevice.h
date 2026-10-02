#pragma once
#include <memory>
#include "FingerprintDefine.h"
#include "Fingerprint_Base.h"
#include "Fingerprint_AS608.h"
#include "Fingerprint_PQIFPS_Reader.h"

class FingerprintDevice {
public:
	static std::shared_ptr<Fingerprint_Base> FingerprintDevice::CreateDevice(FPS_DEVICE type) {
		switch (type) {
#ifndef WINBIO_DISABLE
		case FPS_DEVICE_PQIFPS_READER:
			return std::make_shared<Fingerprint_PQIFPS_Reader>();
#endif // WINBIO_DISABLE
		default:
			return std::make_shared<Fingerprint_AS608>();
		}
	}
};