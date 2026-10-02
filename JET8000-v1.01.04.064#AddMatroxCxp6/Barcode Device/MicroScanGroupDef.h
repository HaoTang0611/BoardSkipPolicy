#ifndef _MICRO_SCAN_GROUP_DEF_H_
#define _MICRO_SCAN_GROUP_DEF_H_

#define TIMEOUT                 '0'
#define NEW_TRIGGER				'1'	
#define TIMEOUT_TRIGGER			'2'
#define CONTINUE_READ			'0'
#define CONTINUE_READ_1			'1'
#define SERIAL_DATA			    '4'
#define SERIAL_DATA_AND_EDGE	'5'

#define ENABLE_LASER_SCANNINT    "H"
#define DISABLE_LASER_SCANNINT   "I"

const std::string STX1 =         "!";
const std::string ETX1 =         "*";
const std::string g_SEPARATOR =	 "+";

#endif