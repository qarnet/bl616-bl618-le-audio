#ifndef _ASCS_SVC_H_
#define _ASCS_SVC_H_


#define BT_UUID_SVC_BLE_ASCS BT_UUID_DECLARE_16(0x184E)

#include "port/include/config.h"

void ascs_init();
struct bt_gatt_attr *get_attr(u8_t index);

#endif 

