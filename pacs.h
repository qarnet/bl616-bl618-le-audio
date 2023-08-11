#ifndef _PACS_H_
#define _PACS_H_

#include "port/include/config.h"

#define BT_UUID_SVC_BLE_PACS BT_UUID_DECLARE_16(0x1850)
#define BT_UUID_CHRC_SINK_PAC BT_UUID_DECLARE_16(0x2bc9)
#define BT_UUID_CHRC_SINK_AUDIO_LOCATIONS BT_UUID_DECLARE_16(0x2bca)
#define BT_UUID_CHRC_AVAILABLE_AUDIO_CONTEXTS BT_UUID_DECLARE_16(0x2bcd)
#define BT_UUID_CHRC_SUPPORTED_AUDIO_CONTEXTS BT_UUID_DECLARE_16(0x2bce)


//indicate value handle offset 6
#define BT_CHRC_SINK_PACK_NOTIFY_ATTR_VAL_INDEX	(1)
//notity value handle offset 9
#define BT_CHRC_SINK_AUDIO_LOCATIONS_NOTIFY_ATTR_VAL_INDEX	(6)
//indicate value handle offset 6
#define BT_CHRC_AVAILABLE_AUDIO_CONTEXTS_NOTIFY_ATTR_VAL_INDEX	(5)
//notity value handle offset 9
#define BT_CHRC_SUPPORTED_AUDIO_CONTEXTS_NOTIFY_ATTR_VAL_INDEX	(4)

int setAvailableAudioContexts(uint16_t _available_sink_contexts, uint16_t _available_source_contexts);
int pacs_init();

#endif 