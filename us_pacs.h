#ifndef _PACS_H_
#define _PACS_H_

#include "port/include/config.h"

#define BT_UUID_SVC_BLE_PACS BT_UUID_DECLARE_16(0x1850)
#define BT_UUID_CHRC_SINK_PAC BT_UUID_DECLARE_16(0x2bc9)
#define BT_UUID_CHRC_SINK_AUDIO_LOCATIONS BT_UUID_DECLARE_16(0x2bca)
#define BT_UUID_CHRC_AVAILABLE_AUDIO_CONTEXTS BT_UUID_DECLARE_16(0x2bcd)
#define BT_UUID_CHRC_SUPPORTED_AUDIO_CONTEXTS BT_UUID_DECLARE_16(0x2bce)

//read value handle offset 2
#define BT_CHAR_BLE_TP_RD_ATTR_VAL_INDEX    	(2)
//write value handle offset 4
#define BT_CHAR_BLE_TP_WR_ATTR_VAL_INDEX	(4)
//indicate value handle offset 6
#define BT_CHAR_BLE_TP_IND_ATTR_VAL_INDEX	(6)
//notity value handle offset 9
#define BT_CHAR_BLE_TP_NOT_ATTR_VAL_INDEX	(9)

int pacs_init();

#endif 